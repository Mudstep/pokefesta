//**********************************************************************
//		evobjeff.c
//		̨����OBJ �̪��
//**********************************************************************
#define		EVOBJEFF_H_PROTO

#include	<agb.h>
#include 	"common.h"
#include	"actor.h"
#include	"actanm.h"
#include	"calctool.h"
#include	"map.h"
#include	"mapbg.h"
#include	"evobj.h"
#include	"jiki.h"
#include	"fieldmap.h"
#include	"map_attr.h"
#include	"task.h"
#include	"seed.h"
#include	"mus_tool.h"
#include	"weather.h"

//----------------------------------------------------------------------
//		�O���Q��
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//		define
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//		����
//----------------------------------------------------------------------
static void EvObjChkActPriSet( actWork *, u8, u8 );

//======================================================================
//		�f�荞�ݱ���
//======================================================================
static s16 ReflectOffset( EventOBJ * );
static void ReflectPalette( EventOBJ *, actWork * );
static void ReflectPaletteSet( EventOBJ *, u8 );
static void ReflectPaletteBridgeSet( EventOBJ *, u8 );
static void EvActReflectMove( actWork * );

//------------------------------------------
//	�f�荞�ݱ����ǉ�
//	In:		ev,act			; ��Ĳ����OBJ,����
//			kind			; 0 = �g�k,1 = ��g�k
//------------------------------------------
void EvActReflectSet( EventOBJ *ev, actWork *act, u8 kind )
{
	u8 i;
	actWork *refact;

	i = CopyActPri1( act, act->x, act->y, EVOBJ_BELONG_ACTPRI );

	refact = &ActWork[i];

	refact->move = EvActReflectMove;
	refact->oamData.Priority = BGPRI3;
	refact->oamData.Pltt = EvObjRefPalNoTbl[refact->oamData.Pltt];
	refact->t_mode = 1;									// ��]���^��

	refact->anm_tbl = DummyActAnmTbl;					// ������-��
	ActAnmChg( refact, 0 );
	refact->aff_tbl = DummyActAffTbl;
	refact->affine_sw = 1;

	refact->oampat_sw = OAMPAT_OFF;						// �߯�OFF

	refact->work[0] = act->work[0];						// �e��
	refact->work[1] = ev->objID;
	refact->work[7] = kind;								// ���

	ReflectPalette( ev, refact );						// �f�荞����گ�

	if( kind == EOE_REF_NORMAL )
	{
		refact->oamData.AffineMode = 1;
	}
}

//------------------------------------------
//	�f�肱�� Y �̾��
//------------------------------------------
static s16 ReflectOffset( EventOBJ *ev )
{
	SXYObjHead *objst;

	objst = SxyObjHeadGet( ev->obj_code );
	return( (s16)objst->y_size - 2 );					// -2 = ���
}

//------------------------------------------
//	�f�肱����گ�
//------------------------------------------
static void ReflectPalette( EventOBJ *ev, actWork *act )
{
	s16 bridge[3] = { 12, 28, 44 };
	SXYObjHead *objst;
	u8 attr;

	act->work[2] = 0;

	objst = SxyObjHeadGet( ev->obj_code );

	if( objst->reflect_change_sw == 0 )					// �f�荞�ݑ����׸�OFF
	{
		attr = ATR_GetBridgeCode( ev->old_attr );

		if( attr )										// ����ޭ�ċ�
		{
			act->work[2] = bridge[attr-1];
			ReflectPaletteBridgeSet( ev, act->oamData.Pltt );
			return;
		}

		attr = ATR_GetBridgeCode( ev->now_attr );

		if( attr )										// ����ޭ�ċ�
		{
			act->work[2] = bridge[attr-1];
			ReflectPaletteBridgeSet( ev, act->oamData.Pltt );
			return;
		}
	}

	ReflectPaletteSet( ev, act->oamData.Pltt );
}

//------------------------------------------
//	�f�肱�� �ʏ���گľ��
//------------------------------------------
static void ReflectPaletteSet( EventOBJ *ev, u8 palno )
{
	SXYObjHead *objst;

	objst = SxyObjHeadGet( ev->obj_code );

	if( objst->sp_reflect_palNo != NOT_SP_REF_PALID )	// ������گĎg�p
	{
		if( objst->oampalNo == MINE_OBJ_PALNO )			// ���@
		{
			EvObjHeroPalSet( objst->palNo, palno );		// ��l����گľ��
		}
		else if( objst->oampalNo == SP_OBJ_PALNO )		// ����OBJ
		{
			EvObjSpPalSet( objst->palNo, palno );		// ������گľ��
		}
		else											// �ʏ���گ�
		{
			EvObjStayPalSet( EvObjPalIdGet(palno), palno );
		}

		FieldWeather_AdjustObjPal( palno );
	}
}

//------------------------------------------
//	�f�肱�� ����گľ��
//------------------------------------------
static void ReflectPaletteBridgeSet( EventOBJ *ev, u8 palno )
{
	SXYObjHead *objst;

	objst = SxyObjHeadGet( ev->obj_code );

	if( objst->sp_reflect_palNo != NOT_SP_REF_PALID )	// ����f�荞����گı�
	{
		EvObjStayPalSet( objst->sp_reflect_palNo, palno );
		FieldWeather_AdjustObjPal( palno );
	}
}

//------------------------------------------
//	�f�荞�ݱ�������
//------------------------------------------
static void EvActReflectMove( actWork *act )
{
	EventOBJ *m_ev;
	actWork *m_act;

	m_ev = &EvObj[act->work[0]];
	m_act = &ActWork[m_ev->actno];

	if( m_ev->sw == 0 || m_ev->attr_reflect_sw == 0 ||
		m_ev->objID != act->work[1] )
	{
		act->act_sw = 0;
		return;
	}

	act->oamData.Pltt = EvObjRefPalNo( m_act->oamData.Pltt );
	act->oamData.Shape = m_act->oamData.Shape;
	act->oamData.Size = m_act->oamData.Size;
	act->oamData.AffineParamNo = m_act->oamData.AffineParamNo | 0x10;
	act->oamData.CharNo = m_act->oamData.CharNo;
	act->oamPat = m_act->oamPat;
	act->oampat_no = m_act->oampat_no;
	act->banish = m_act->banish;
	act->x = m_act->x;
	act->y = m_act->y + ReflectOffset( m_ev ) + act->work[2];
	act->cx = m_act->cx;
	act->cy = m_act->cy;
	act->dx = m_act->dx;
	act->dy = -m_act->dy;
	act->scr = m_act->scr;

	if( act->work[7] == EOE_REF_NORMAL )					// �f�荞�ݎ��
	{
		act->oamData.AffineParamNo = 0;
		
		if( m_act->oamData.AffineParamNo & 8 )
		{
			act->oamData.AffineParamNo = 1;
		}
	}
}

//======================================================================
//		�o������
//======================================================================
//------------------------------------------
//	�o������ �ǉ�
//	Out:	u8				; ����ܰ�����
//------------------------------------------
u8 EvActExitCursorSet( void )
{
	u8 i;
	actWork *act;

	i = AddActorBottom( EvActHeadGet(EVACT_EXIT_CURSOR), 0, 0, EVOBJ_HI_ACTPRI );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->oamData.Priority = BGPRI1;
		act->scr = 1;
		act->banish = 1;
	}

	return( i );
}

//------------------------------------------
//	�o������ ��\��
//	In:		actno			; ���ٱ���ܰ�����
//------------------------------------------
void EvActExitCursorOFF( u8 actno )
{
	ActWork[actno].banish = 1;
}

//------------------------------------------
//	�o������ �o��
//	In:		actno			; ���ٱ���ܰ�����
//			site			; ����
//			mx,my			; �\��ϯ�ߍ��W
//------------------------------------------
void EvActExitCursorON( u8 actno, u8 site, s16 mx, s16 my )
{
	s16 x,y;
	actWork *act;

	act = &ActWork[actno];

	if( act->banish ||									// OFF
		act->work[0] != mx || act->work[1] != my )		// �X�V
	{
		MapCellSizePosDiff( mx, my, &x, &y );
		act = &ActWork[actno];
		act->x = x + 8;
		act->y = y + 8;
		act->banish = 0;
		act->work[0] = mx;
		act->work[1] = my;
		ActAnmChg( act, site - 1 );						// site - 1 = ��� No
	}
}

//======================================================================
//		�e
//======================================================================
//------------------------------------------
//	�e���
//------------------------------------------
static const u8 ShadeKind[4] =
{ EVACT_SHADE8x8, EVACT_SHADE16x8, EVACT_SHADE32x8, EVACT_SHADE64x32 };

static const s16 ShadeOffset[4] =
{ 4, 4, 4, 16 };

//------------------------------------------
//	̨���޴̪�� �e�������
//	In:		work0,1,2			; OBJ,MAP,DIV ID
//	Out:	����				; ����ܰ�����
//------------------------------------------
u32 FeShadeSet( void )
{
	u8 i;
	SXYObjHead *objst;

	i = EvObjIdSeek( FeSetWork[0], FeSetWork[1], FeSetWork[2] );
	objst = SxyObjHeadGet( EvObj[i].obj_code );
	i = AddActorBottom( EvActHeadGet(ShadeKind[objst->shade]), 0, 0, EVOBJ_LOW_ACTPRI0 );

	if( i != ACT_MAX )
	{
		ActWork[i].scr = 1;
		ActWork[i].work[0] = FeSetWork[0];
		ActWork[i].work[1] = FeSetWork[1];
		ActWork[i].work[2] = FeSetWork[2];
		ActWork[i].work[3] = (objst->y_size >> 1) - ShadeOffset[objst->shade];
	}

	return( 0 );
}

//------------------------------------------
//	�e��������
//------------------------------------------
void EvActShadeMove( actWork *act )
{
	u8 i;
	EventOBJ *ev;
	actWork *ev_act;

	if( EvObjIdPosGet(act->work[0],act->work[1],act->work[2],&i) )
	{
		FeDelActCode( act, FECODE_Shade );
		return;
	}

	ev = &EvObj[i];
	ev_act = &ActWork[ev->actno];

	act->oamData.Priority = ev_act->oamData.Priority;
	act->x = ev_act->x;
	act->y = ev_act->y + act->work[3];

	if( ev->sw == 0 ||
		ev->shade_act_sw == 0 ||
		ATR_IsGrassCheck(ev->now_attr) ||
		ATR_IsWaterCheck(ev->now_attr) ||
		ATR_IsWaterCheck(ev->old_attr) ||
		ATR_IsReflectCheck(ev->now_attr) ||
		ATR_IsReflectCheck(ev->old_attr) )
	{
		FeDelActCode( act, FECODE_Shade );
	}
}

//======================================================================
//		�Z����
//======================================================================
//------------------------------------------
//	̨���޴̪�� �Z�������
//	In:		work0,1			; ϯ��X,Y
//			work2,3			; ����,BG��ײ��è
//			work4			; OBJID<<8|OBJ_MAP_ID
//			work5			; OBJ_DIV_ID
//			work6			; MAP_ID<<8|MAP_DIV_ID
//			work7			; ON = ��Җ���
//	Out:	nothing	
//------------------------------------------
u32 FeShortGrassShakeSet( void )
{
	u8	i;
	s16 x,y;
	actWork *act;

	x = FeSetWork[0];
	y = FeSetWork[1];
	MapCellSizePosDiffOffs( &x, &y, 8, 8 );

	i = AddActorBottom( EvActHeadGet(EVACT_SHORTGRASS_SHAKE), x, y, 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FeSetWork[2];					// ����
		act->work[1] = FeSetWork[0];					// X
		act->work[2] = FeSetWork[1];					// Y
		act->work[3] = FeSetWork[4];					// ID
		act->work[4] = FeSetWork[5];
		act->work[5] = FeSetWork[6];

		if( FeSetWork[7] )
		{
			ActAnmOffsChg( act, GRASS_ANMOFFS_ENDNO );
		}
	}

	return( 0 );
}

//------------------------------------------
//	�Z�����h�� ����
//------------------------------------------
void FeShortGrassShakeMove( actWork *act )
{
	u8 no,work,obj_id,map_id,div_id;
	EventOBJ *ev;

	map_id = act->work[5] >> 8;							// map ID
	div_id = act->work[5] & 0xff;

	if( MapChange.chg_flag && (Fld.MapID.map_id != map_id || Fld.MapID.div_id != div_id) )
	{
		act->work[1] -= MapChange.diffx;
		act->work[2] -= MapChange.diffy;
		act->work[5] = ((u8)Fld.MapID.map_id << 8) | (u8)Fld.MapID.div_id;
	}

	obj_id = act->work[3] >> 8;							// obj ID
	map_id = act->work[3] & 0xff;
	div_id = act->work[4];
	work = GetCellAttribute( act->work[1], act->work[2] );

	if( EvObjIdPosGet(obj_id,map_id,div_id,&no) ||		// �e,����,���
		ATR_ShortGrassCheck(work) == 0 || (act->work[7] && act->anmend_sw) )
	{
		FeDelActCode( act, FECODE_ShortGrassShake );
		return;
	}

	ev = &EvObj[no];

	if( (ev->gx != act->work[1] || ev->gy != act->work[2]) &&
		(ev->old_gx != act->work[1] || ev->old_gy != act->work[2]) )
	{
		act->work[7] = 1;								// �e����
	}

	work = 0;
	if( act->anm_offs == 0 ) work = 4;					// �X�ɂ����Ă�

	EvActScrollVanish( act, 0 );
	EvObjChkActPriSet( act, act->work[0], work );
}

//------------------------------------------
//	̨���޴̪�� �Z������
//	In:		work0,1			; ϯ��X,Y
//			work2,3			; ����,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeShortGrassFallSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 12 );
	i = AddActorBottom( EvActHeadGet(EVACT_SHORTGRASS_FALL), FeSetWork[0], FeSetWork[1], 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FeSetWork[2];
		act->work[1] = FECODE_ShortGrassFall;
	}

	return( 0 );
}

//------------------------------------------
//	����������
//	In: 	obj_id,map_id,div_id	; OBJ�ݒ�eID
//			gx,gy					; ����ϯ�ߍ��W
//	Out:	u8						; ACT_MAX = ���݂��Ȃ�
//------------------------------------------
u8 CheckFeShortGrass( u8 obj_id, u8 map_id, u8 div_id, s16 gx, s16 gy )
{
	u8 i;
	actWork *act;

	for( i = 0; i < ACT_MAX; i++ )
	{
		if( ActWork[i].act_sw )
		{
			act = &ActWork[i];

			if( act->move == FeShortGrassShakeMove &&
				gx == act->work[1] &&
				gy == act->work[2] &&
				obj_id == (act->work[3] >> 8) &&
				map_id == (act->work[3] & 0xff) &&
				div_id == act->work[4] )
			{
				return( i );
			}
		}
	}

	return( ACT_MAX );
}

//======================================================================
//		������
//======================================================================
//------------------------------------------
//	̨���޴̪�� �������h��
//	In:		work0,1			; ϯ��x,y
//			work2,3			; ����,BG��ײ��è
//			work4			; OBJ_ID<<8|OBJ_MAP_ID
//			work5			; OBJ_DIV_ID
//			work6			; MAP_ID<<8|DIV_ID
//	Out:	nothing
//------------------------------------------
u32 FeLongGrassShakeSet( void )
{
	u8	i;
	s16 x,y;
	actWork *act;

	x = FeSetWork[0];
	y = FeSetWork[1];
	MapCellSizePosDiffOffs( &x, &y, 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_LONGGRASS_SHAKE), x, y, 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = EvObjHeightBgPri( FeSetWork[2] );
		act->work[0] = FeSetWork[2];
		act->work[1] = FeSetWork[0];
		act->work[2] = FeSetWork[1];
		act->work[3] = FeSetWork[4];
		act->work[4] = FeSetWork[5];
		act->work[5] = FeSetWork[6];

		if( FeSetWork[7] )
		{
			ActAnmOffsChg( act, LONG_GRASS_ANMOFFS_ENDNO );
		}
	}

	return( 0 );
}

//------------------------------------------
//	�������h�� ����
//------------------------------------------
void FeLongGrassShakeMove( actWork *act )
{
	u8 no,work,obj_id,map_id,div_id;
	EventOBJ *ev;

	map_id = act->work[5] >> 8;							// map ID
	div_id = act->work[5] & 0xff;

	if( MapChange.chg_flag && (Fld.MapID.map_id != map_id || Fld.MapID.div_id != div_id) )
	{
		act->work[1] -= MapChange.diffx;
		act->work[2] -= MapChange.diffy;
		act->work[5] = ((u8)Fld.MapID.map_id << 8) | (u8)Fld.MapID.div_id;
	}

	obj_id = act->work[3] >> 8;							// obj ID
	map_id = act->work[3] & 0xff;
	div_id = act->work[4];
	work = GetCellAttribute( act->work[1], act->work[2] );

	if( EvObjIdPosGet(obj_id,map_id,div_id,&no) ||		// �e,����,���
		ATR_LongGrassCheck(work) == 0 || (act->work[7] && act->anmend_sw) )
	{
		FeDelActCode( act, FECODE_LongGrassShake );
		return;
	}

	ev = &EvObj[no];

	if( (ev->gx != act->work[1] || ev->gy != act->work[2]) &&
		(ev->old_gx != act->work[1] || ev->old_gy != act->work[2]) )
	{
		act->work[7] = 1;								// �e����
	}

	EvActScrollVanish( act, 0 );
	EvObjChkActPriSet( act, act->work[0], 0 );
//	EvActPriControl( act->work[0], act, 0 );
}

//------------------------------------------
//	̨���޴̪�� ��������
//	In:		work0,1			; ϯ��X,Y
//			work2,3			; ����,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeLongGrassFallSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_LONGGRASS_FALL), FeSetWork[0], FeSetWork[1], 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FeSetWork[2];
		act->work[1] = FECODE_LongGrassFall;
	}

	return( 0 );
}

//======================================================================
//		��������
//======================================================================
//------------------------------------------
//	̨���޴̪�� ��������
//	In:		work0,1,2		; OBJ,MAP,DIV ID
//	Out:	nothing
//------------------------------------------
u32 FeSmallGrassSet( void )
{
	u8	i;
	EventOBJ *ev;
	actWork *act;

	ev = &EvObj[EvObjIdSeek(FeSetWork[0],FeSetWork[1],FeSetWork[2])];
	i = AddActorBottom( EvActHeadGet(EVACT_SMALLGRASS), 0, 0, 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = ActWork[ev->actno].oamData.Priority;
		act->work[0] = FeSetWork[0];
		act->work[1] = FeSetWork[1];
		act->work[2] = FeSetWork[2];
		act->work[3] = ActWork[ev->actno].x;
		act->work[4] = ActWork[ev->actno].y;
	}

	return( 0 );
}

//------------------------------------------
//	������������
//------------------------------------------
void EvActSmallGrassMove( actWork *act )
{
	u8 i;
	s16 x,y;
	SXYObjHead *objst;
	actWork *ev_act;

	if( EvObjIdPosGet(act->work[0],act->work[1],act->work[2],&i) ||
		EvObj[i].attr_grass_sw == 0 )
	{
		FeDelActCode( act, FECODE_SmallGrass );
		return;
	}

	objst = SxyObjHeadGet( EvObj[i].obj_code );
	ev_act = &ActWork[EvObj[i].actno];

	x = ev_act->x;
	y = ev_act->y;

	if( x != act->work[3] || y != act->work[4] )
	{
		act->work[3] = x;
		act->work[4] = y;

		if( act->anmend_sw )
		{
			ActAnmChg( act, 0 );
		}
	}

	act->x = x;
	act->y = y;
	act->dy = (objst->y_size >> 1) - 8;
	act->pri = ev_act->pri - 1;
	act->oamData.Priority = ev_act->oamData.Priority;
	EvActScrollVanish( act, ev_act->banish );
}

//======================================================================
//		����
//======================================================================
static void EvActFootPrint0( actWork * );
static void EvActFootPrint1( actWork * );

//------------------------------------------
//	̨���޴̪�� ����
//	In:		work0,1			; ϯ��x,y
//			work2,3			; ���,BG��ײ��è
//			work4			; ����
//	Out:	nothing
//------------------------------------------
u32 FeFootPrintSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_FOOTPRINT),
					FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[7] = FECODE_FootPrint;
		ActAnmChg( act, FeSetWork[4] );
	}

	return( 0 );
}

//------------------------------------------
//	̨���޴̪�� �[������
//	In:		work0,1			; ϯ��x,y
//			work2,3			; ���,BG��ײ��è
//			work4			; ����
//	Out:	nothing
//------------------------------------------
u32 FeFootPrintDeepSet( void )
{
	u8 i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_FOOTPRINT_DEEP),
					FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[7] = FECODE_FootPrintDeep;
		ActAnmChg( &ActWork[i], FeSetWork[4] );
	}

	return( (u32)i );
}

//------------------------------------------
//	̨���޴̪�� ���]�ԑ���
//	In:		work0,1			; ϯ��x,y
//			work2,3			; ���,BG��ײ��è
//			work4			; ����
//	Out:	u32				; ���ܰ�����
//------------------------------------------
u32 FeFootPrintCycleSet( void )
{
	u8 i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_FOOTPRINT_CYCLE),
					FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[7] = FECODE_FootPrintCycle;
		ActAnmChg( act, FeSetWork[4] );
	}

	return( (u32)i );
}

//------------------------------------------
//	����ð���
//------------------------------------------
static void (* const FootPrintMoveTbl[])(actWork*) =
{ EvActFootPrint0, EvActFootPrint1 };
	
//------------------------------------------
//	���Ջ��ʓ���
//------------------------------------------
void EvActFootPrintMove( actWork *act )
{
	FootPrintMoveTbl[act->work[0]]( act );
}

//------------------------------------------
//	���� ����0
//------------------------------------------
static void EvActFootPrint0( actWork *act )
{
	act->work[1]++;
	
	if( act->work[1] > 40 )
	{
		act->work[0] = 1;
	}

	EvActScrollVanish( act, 0 );
}

//------------------------------------------
//	����1 ����1
//------------------------------------------
static void EvActFootPrint1( actWork *act )
{
	act->banish ^= 1;
	act->work[1]++;

	EvActScrollVanish( act, act->banish );

	if( act->work[1] > 56 )
	{
		FeDelActCode( act, act->work[7] );
	}
}

//======================================================================
//		����,��,�g��
//======================================================================
static void EvActShoalMove( actWork * );

//------------------------------------------
//	̨���޴̪�� ����
//	In:		work0,1,2		; OBJ,MAP,DIV ID
//------------------------------------------
u32 FeSplashSet( void )
{
	u8	i;
	EventOBJ *ev;
	actWork *act;
	SXYObjHead *objst;

	ev = &EvObj[EvObjIdSeek(FeSetWork[0],FeSetWork[1],FeSetWork[2])];
	i = AddActorBottom( EvActHeadGet(EVACT_SPLASH), 0, 0, 0 );

	if( i != ACT_MAX )
	{
		objst = SxyObjHeadGet( ev->obj_code );

		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = ActWork[ev->actno].oamData.Priority;
		act->work[0] = FeSetWork[0];
		act->work[1] = FeSetWork[1];
		act->work[2] = FeSetWork[2];
		act->dy = (objst->y_size >> 1) - 4;
		SePlay( SE_MIZU );
	}

	return( 0 );
}

//------------------------------------------
//	���� ����
//------------------------------------------
void EvActSplashMove( actWork *act )
{
	u8 i;
	
	if( act->anmend_sw ||
		EvObjIdPosGet(act->work[0],act->work[1],act->work[2],&i) )
	{
		FeDelActCode( act, FECODE_Splash );
		return;
	}

	act->x = ActWork[EvObj[i].actno].x;
	act->y = ActWork[EvObj[i].actno].y;

	EvActScrollVanish( act, 0 );
}

//------------------------------------------
//	̨���޴̪�� ���� 2
//	In:		work0,1			; ϯ��x,y
//			work2,3			; ����,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeSplashWaterSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 12 );
	i = AddActorBottom( EvActHeadGet(EVACT_SPLASH_WATER), FeSetWork[0], FeSetWork[1], 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FeSetWork[2];
		act->work[1] = FECODE_SplashWater;
	}

	return( 0 );
}

//------------------------------------------
//	̨���޴̪�� �C����
//	In:		work0,1			; ϯ��x,y
//			work2,3			; ����,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeSplashSeaSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_SPLASH_SEA), FeSetWork[0], FeSetWork[1], 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FeSetWork[2];
		act->work[1] = FECODE_SplashSea;
	}

	return( 0 );
}

//------------------------------------------
//	̨���޴̪�� �󐣾��
//	In:		work0,1,2		; OBJ,MAP,DIV ID
//------------------------------------------
u32 FeShoalSet( void )
{
	u8	i;
	EventOBJ *ev;
	actWork *act;
	SXYObjHead *objst;

	ev = &EvObj[EvObjIdSeek(FeSetWork[0],FeSetWork[1],FeSetWork[2])];
	i = AddActorBottom( EvActHeadGet(EVACT_SPLASH), 0, 0, 0 );

	if( i != ACT_MAX )
	{
		objst = SxyObjHeadGet( ev->obj_code );

		act = &ActWork[i];
		act->move = EvActShoalMove;
		act->scr = 1;
		act->oamData.Priority = ActWork[ev->actno].oamData.Priority;
		act->work[0] = FeSetWork[0];
		act->work[1] = FeSetWork[1];
		act->work[2] = FeSetWork[2];
		act->work[3] = 0xffff;
		act->work[4] = 0xffff;
		act->dy = (objst->y_size >> 1) - 4;
		ActAnmChg( act, 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�󐣓���
//------------------------------------------
static void EvActShoalMove( actWork *act )
{
	u8 i;
	actWork *ev_act;
	EventOBJ *ev;

	if( EvObjIdPosGet(act->work[0],act->work[1],act->work[2],&i) ||
		EvObj[i].attr_shoal_sw == 0 )
	{
		FeDelActCode( act, FECODE_Shoal );
		return;
	}

	ev = &EvObj[i];
	ev_act = &ActWork[EvObj[i].actno];
	act->x = ev_act->x;
	act->y = ev_act->y;
	act->pri = ev_act->pri;

	EvActScrollVanish( act, 0 );
	
	if( ev->gx != act->work[3] || ev->gy != act->work[4] )
	{
		act->work[3] = ev->gx;
		act->work[4] = ev->gy;

		if( act->banish == 0 )
		{
			SePlay( SE_MIZU );
		}
	}
}

//------------------------------------------
//	̨���޴̪��	���g�䱸�����
//	In:		work0,1			; x,y
//			work2,3			; ���,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32	FeRippleSet( void )
{
	u8 i;
	actWork *act;

	i = AddActorBottom( EvActHeadGet(EVACT_RIPPLE), FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FECODE_Ripple;
	}

	return( 0 );
}

//======================================================================
//		����
//======================================================================
//------------------------------------------
//	̨���޴̪�� ����
//	In:		work0,1,2		; OBJ,MAP,DIV ID
//	Out:	nothing
//------------------------------------------
u32 FeOnsenSet( void )
{
	u8	i;
	EventOBJ *ev;
	actWork *act;

	ev = &EvObj[EvObjIdSeek(FeSetWork[0],FeSetWork[1],FeSetWork[2])];
	i = AddActorBottom( EvActHeadGet(EVACT_ONSEN), 0, 0, 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = ActWork[ev->actno].oamData.Priority;
		act->work[0] = FeSetWork[0];
		act->work[1] = FeSetWork[1];
		act->work[2] = FeSetWork[2];
		act->work[3] = ActWork[ev->actno].x;
		act->work[4] = ActWork[ev->actno].y;
	}

	return( 0 );
}

//------------------------------------------
//	���򓮍�
//------------------------------------------
void EvActOnsenMove( actWork *act )
{
	u8 i;
	SXYObjHead *objst;
	actWork *ev_act;

	if( EvObjIdPosGet(act->work[0],act->work[1],act->work[2],&i) ||
		EvObj[i].attr_onsen_sw == 0 )
	{
		FeDelActCode( act, FECODE_Onsen );
		return;
	}

	objst = SxyObjHeadGet( EvObj[i].obj_code );
	ev_act = &ActWork[EvObj[i].actno];

	act->x = ev_act->x;
	act->y = ev_act->y + (objst->y_size >> 1) - 8;
	act->pri = ev_act->pri - 1;
	EvActScrollVanish( act, 0 );
}

//======================================================================
//		�ݶ��ı���
//======================================================================
//------------------------------------------
//	̨���޴̪�� �Z�����ݶ���
//	In:		work0,1			; ϯ��X,Y
//			work2,3			; ���,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeShortGrassEncountSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_SHORTGRASS_ENCOUNT),
					FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FECODE_ShortGrassEncount;
	}

	return( 0 );
}

//------------------------------------------
//	̨���޴̪�� �������ݶ���
//	In:		work0,1			; ϯ��X,Y
//			work2,3			; ���,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeLongGrassEncountSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_LONGGRASS_ENCOUNT),
					FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FECODE_LongGrassEncount;
	}

	return( 0 );
}

//------------------------------------------
//	̨���޴̪�� ���ݶ���
//	In:		work0,1			; ϯ��X,Y
//			work2,3			; ���,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeSandEncountSet( void )
{
	u8 i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_SAND_ENCOUNT),
						FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		ActWork[i].scr = 1;
		ActWork[i].oamData.Priority = FeSetWork[3];
		ActWork[i].work[0] = FECODE_SandEncount;
	}

	return( 0 );
}

//------------------------------------------
//	̨���޴̪�� ���ݶ���
//	In:		work0,1			; ϯ�� X,Y
//			work2,3			; ���,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeWaterEncountSet( void )
{
	u8 i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_WATER_ENCOUNT),
					FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FECODE_WaterEncount;
	}

	return( 0 );
}

//======================================================================
//		�D
//======================================================================
static void AshMove0( actWork * );
static void AshMove1( actWork * );
static void AshMove2( actWork * );

//------------------------------------------
//	�D���
//	In:		mx,my			; ���ϯ�ߍ��W
//			cell			; ��ݼ޾�
//			wait			; ����������܂ł̳���
//------------------------------------------
void FeAshMake( s16 mx, s16 my, u16 cell, s16 wait )
{
	FeSetWork[0] = mx;
	FeSetWork[1] = my;
	FeSetWork[2] = EVOBJ_HI_ACTPRI;
	FeSetWork[3] = BGPRI1;
	FeSetWork[4] = cell;
	FeSetWork[5] = wait;

	FldEffSet( FECODE_Ash );
}

//------------------------------------------
//	̨���޴̪�� �D���
//	In:		work0,1			; X,Y(ϯ��)
//			work2,3			; ���,BG��ײ��è
//			work4			; ��ݼ޾�����
//			work5			; �J�n����
//	Out:	nothing
//------------------------------------------
u32 FeAshSet( void )
{
	u8	i;
	s16 x,y;
	actWork *act;
	
	x = FeSetWork[0];
	y = FeSetWork[1];
	MapCellSizePosDiffOffs( &x, &y, 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_ASH), x, y, FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[1] = FeSetWork[0];
		act->work[2] = FeSetWork[1];
		act->work[3] = FeSetWork[4];
		act->work[4] = FeSetWork[5];
	}

	return( 0 );
}

//------------------------------------------
//	�D ����ð���
//------------------------------------------
static void (* const AshMoveTbl[])(actWork*) =
{
	AshMove0,
	AshMove1,
	AshMove2,
};

//------------------------------------------
//	�D ����
//------------------------------------------
void EvActAshMove( actWork *act )
{
	AshMoveTbl[act->work[0]]( act );
}

//------------------------------------------
//	�D ����0
//------------------------------------------
static void AshMove0( actWork *act )
{
	act->banish = 1;
	act->anm_pause = 1;

	act->work[4]--;

	if( act->work[4] == 0 )
	{
		act->work[0] = 1;
	}
}

//------------------------------------------
//	�D ����1
//------------------------------------------
static void AshMove1( actWork *act )
{
	act->banish = 0;
	act->anm_pause = 0;

	SetCell( act->work[1], act->work[2], act->work[3] );
	RewriteSingleBG( act->work[1], act->work[2] );
	EvObj[MyMoveState.evobjno].move_start_sw = 1;

	act->work[0] = 2;
}

//------------------------------------------
//	�D ����2
//------------------------------------------
static void AshMove2( actWork *act )
{
	EvActScrollVanish( act, 0 );

	if( act->anmend_sw )
	{
		FeDelActCode( act, FECODE_Ash );
	}
}

//======================================================================
//		�g����߹��
//======================================================================
static void set_namipoke_site( EventOBJ *, actWork * );
static void check_namipoke_pos( EventOBJ *, actWork * );
static void set_namipoke_yure( EventOBJ *, actWork *, actWork * );

//------------------------------------------
//	�g����߹��
//	In:		work0,1			; ϯ�ߍ��W
//			work2			; �eEvObjܰ�����
//	Out:	u32				; ����ܰ�����
//	Info:	����ܰ�0
//			0 = ��������, 1 = ����
//			2 = �������� & �����
//			����ܰ�1
//			0 = �����ϊ�, 1 = �ϊ�����
//------------------------------------------
u32 FePokeSwimSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );
	i = AddActorBottom( EvActHeadGet(EVACT_POKE_SWIM),
				FeSetWork[0], FeSetWork[1], EVOBJ_LOW_ACTPRI2 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Pltt = MINE_OBJ_PALNO;				// ��l����گĂɈˑ�
		act->work[2] = FeSetWork[2];
		act->work[3] = -1;
		act->work[6] = 0xffff;
		act->work[7] = 0xffff;
	}

	FeCodeDel( FECODE_PokeSwim );
	return( (u32)i );
}

//------------------------------------------
//	�g����߹�ݓ����׸޾��
//	In:		no			; �g����߹�ݱ������
//			flag		; �����׸�
//------------------------------------------
void SetPokeSwimMoveFlag( u8 no , u8 flag )
{
	ActWork[no].work[0] = (ActWork[no].work[0] & 0xfff0) | (flag & 0x0f);
}

//------------------------------------------
//	�g����߹�ݕ����׸޾��
//	In:		no			; �g����߹�ݱ������
//			flag		; ON = �����Œ�
//------------------------------------------
void SetPokeSwimSiteFlag( u8 no , u8 flag )
{
	ActWork[no].work[0] = (ActWork[no].work[0] & 0xff0f) | ((flag & 0x0f) << 4);
}

//------------------------------------------
//	�g����߹�ݵ̾���׸޾��
//	In:		no			; �g����߹�ݱ������
//			flag		; ON = �̾�Ďw��
//			offset		; �̾��
//------------------------------------------
void SetPokeSwimOffsetFlag( u8 no , u8 flag, s16 offset )
{
	ActWork[no].work[0] = (ActWork[no].work[0] & 0xf0ff) | ((flag & 0x0f) << 8);
	ActWork[no].work[1] = offset;
}

//------------------------------------------
//	�����׸ގ擾
//------------------------------------------
static u8 GetPokeSwimMoveFlag( actWork *act )
{
	return( act->work[0] & 0x000f );
}

//------------------------------------------
//	�����׸ގ擾
//------------------------------------------
static u8 GetPokeSwimSiteFlag( actWork *act )
{
	return( (act->work[0] & 0x00f0) >> 4 );
}

//------------------------------------------
//	�̾���׸ގ擾
//------------------------------------------
static u8 GetPokeSwimOffsetFlag( actWork *act )
{
	return( (act->work[0] & 0x0f00) >> 8 );
}

//------------------------------------------
//	�g����߹�ݓ���
//------------------------------------------
void EvActPokeSwimMove( actWork *act )
{
	EventOBJ *ev;
	actWork *evact;

	ev = &EvObj[act->work[2]];
	evact = &ActWork[ev->actno];

	set_namipoke_site( ev, act );
	check_namipoke_pos( ev, act );
	set_namipoke_yure( ev, evact, act );

	act->oamData.Priority = evact->oamData.Priority;
}

//------------------------------------------
//	�������
//------------------------------------------
static void set_namipoke_site( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8	anm_code[] = {0,0,1,2,3};

	if( GetPokeSwimSiteFlag(act) == NAMIPOKE_SITE_NOCHG )
	{
		site = ev->mv_site;
		ActAnmChkChg( act, anm_code[site] );
	}
}

//------------------------------------------
//	���W
//------------------------------------------
static void check_namipoke_pos( EventOBJ *ev, actWork *act )
{
	u8 site;
	s16 x,y;

	x = ev->gx;
	y = ev->gy;

	if( act->dy == 0 && (x != act->work[6] || y != act->work[7]) )
	{
		act->work[5] = 0;
		act->work[6] = x;
		act->work[7] = y;

		for( site = SITE_D; site <= SITE_R; site++, x = act->work[6], y = act->work[7] )
		{
			SitePosMove( site, &x, &y );

			if( GetCellHeight(x,y) == 3 )
			{
				act->work[5]++;
				break;
			}
		}
	}
}

//------------------------------------------
//	���
//------------------------------------------
static void set_namipoke_yure( EventOBJ *ev, actWork *evact, actWork *act )
{
	u8 flag;
	s16 yure[2] = { 0x03, 0x07 };
	
	flag = GetPokeSwimMoveFlag( act );

	if( flag == NAMIPOKE_NOWITH )
	{
		return;
	}

	act->work[4]++;

	if( (act->work[4] & yure[act->work[5]]) == 0 )
	{
		act->dy += act->work[3];
	}

	if( (act->work[4] & 0x0f) == 0 )
	{
		act->work[3] = -(act->work[3]);
	}

	if( flag == NAMIPOKE_NOWITH_SWAY )
	{
		return;
	}
	
	if( GetPokeSwimOffsetFlag(act) == NAMIPOKE_VOFFSET_OFF )
	{
		evact->dy = act->dy;
	}
	else												// �w��̾�ı�
	{
		evact->dy = act->work[1] + act->dy;
	}

	act->x = evact->x;
	act->y = evact->y + 8;
}

//======================================================================
//		�޲��ݸ�
//======================================================================
static void EvActDivingSwayMove( actWork * );

//------------------------------------------
//	�޲��ݸޗh�걸�����
//	In:		actno			; �e����ܰ�����
//	Out:	u8				; ����ܰ�����
//------------------------------------------
u8 EvActDivingSwaySet( u8 actno )
{
	u8 i;
	actWork *act;
	
	i = AddActorBottom( &DummyActHeader, 0, 0, 0xff );

	act = &ActWork[i];
	act->move = EvActDivingSwayMove;
	act->banish = 1;
	act->work[0] = actno;
	act->work[1] = 1;

	return( i );
}

//------------------------------------------
//	�޲��ݸޱ�������
//------------------------------------------
static void EvActDivingSwayMove( actWork *act )
{
	actWork *evact;

	evact = &ActWork[act->work[0]];

	if( (act->work[2]++ & 0x03) == 0 )
	{
		evact->dy += act->work[1];
	}

	if( (act->work[2] & 0x0f) == 0 )
	{
		act->work[1] = -act->work[1];
	}
}

//======================================================================
//		��
//======================================================================
//------------------------------------------
//	̨���޴̪�� �������
//	In:		work0,1			; ϯ��x,y
//			work2,work3		; ����,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeHokoriSet( void )
{
	u8 i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 12 );
	i = AddActorBottom( EvActHeadGet(EVACT_HOKORI), FeSetWork[0], FeSetWork[1], 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->work[0] = FeSetWork[2];
		act->work[1] = FECODE_Hokori;
	}

	return( 0 );
}

//------------------------------------------
//	̨���޴̪�� �[����
//	In:		work0,1,2		; OBJ,MAP,DIV ID
//	Out:	nothing
//------------------------------------------
u32 FeDeepSandSet( void )
{
	u8 i;
	EventOBJ *ev;
	actWork *act;
	SXYObjHead *objst;

	ev = &EvObj[EvObjIdSeek(FeSetWork[0],FeSetWork[1],FeSetWork[2])];
	i = AddActorBottom( EvActHeadGet(EVACT_DEEPSAND), 0, 0, 0 );

	if( i != ACT_MAX )
	{
		objst = SxyObjHeadGet( ev->obj_code );

		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = ActWork[ev->actno].oamData.Priority;
		act->work[0] = FeSetWork[0];
		act->work[1] = FeSetWork[1];
		act->work[2] = FeSetWork[2];
		act->work[3] = ActWork[ev->actno].x;
		act->work[4] = ActWork[ev->actno].y;
		act->dy = (objst->y_size >> 1) - 2;
		ActAnmOffsChg( act, SAND_ANMOFFS_ENDNO );
	}

	return( 0 );
}

//------------------------------------------
//	��4����
//------------------------------------------
void EvActDeepSandMove( actWork *act )
{
	u8 i;
	s16 x,y;

	if( EvObjIdPosGet(act->work[0],act->work[1],act->work[2],&i) ||
		EvObj[i].attr_sand_sw == 0 )
	{
		FeDelActCode( act, FECODE_DeepSand );
		return;
	}

	x = ActWork[EvObj[i].actno].x;
	y = ActWork[EvObj[i].actno].y;

	if( x != act->work[3] || y != act->work[4] )
	{
		act->work[3] = x;
		act->work[4] = y;

		if( act->anmend_sw )
		{
			ActAnmChg( act, 0 );
		}
	}

	act->x = x;
	act->y = y;
	act->pri = ActWork[EvObj[i].actno].pri;
	EvActScrollVanish( act, 0 );
}

//======================================================================
//		�A
//======================================================================
//------------------------------------------
//	̨���޴̪�� �A���
//	In:		work0,1			; ϯ��x,y
//	Out:	nothing
//------------------------------------------
u32 FeAwaSet( void )
{
	u8 i;
	actWork *act;
	
	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 0 );
	i = AddActorBottom( EvActHeadGet(EVACT_AWA), FeSetWork[0], FeSetWork[1], EVOBJ_HI_ACTPRI );
	
	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = BGPRI1;
	}

	return( 0 );
}

//------------------------------------------
//	�A����
//------------------------------------------
void EvActAwaMove( actWork *act )
{
	act->work[0] += 0x0080;
	act->work[0] &= 0x0100;
	act->y -= act->work[0] >> 8;

	EvActScrollVanish( act, 0 );

	if( act->banish || act->anmend_sw )
	{
		FeDelActCode( act, FECODE_Awa );
	}
}

//======================================================================
//		��̪��
//======================================================================
//------------------------------------------
//	̨���޴̪�� ��̪��
//	In:		work0,1			; ϯ��X,Y
//			work2,3			; ���,BG��ײ��è
//	Out:	u32				; ��������
//------------------------------------------
u32 FeSeedEffectSet( void )
{
	u8	i;
	actWork *act;

	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 4 );
	i = AddActorBottom( EvActHeadGet(EVACT_SEEDEFFECT),
						FeSetWork[0], FeSetWork[1], FeSetWork[2] );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr = 1;
		act->oamData.Priority = FeSetWork[3];
		act->oamData.Pltt = OTHERS_OBJ_PALNO_3;
		act->work[0] = FECODE_SeedEffect;
	}

	return( 0 );
}

//======================================================================
//		�ڰŰ�B�ꖪ
//======================================================================
static u32 SetTrainerHideActor( u8, u8, u8 );

//------------------------------------------
//	̨���޴̪�� �ڰŰ�ؾ��
//	In:		work0,1,2		; OBJ_ID, MAP_ID, DIV_ID
//	Out:	u32				; ����ܰ�����
//------------------------------------------
u32 FeTrainerHideWoodSet( void )
{
	return( SetTrainerHideActor(FECODE_TrainerHideWood,EVACT_HIDEWOOD,OTHERS_OBJ_PALNO_2) );
}

//------------------------------------------
//	̨���޴̪�� �ڰŰ�⾯�
//	In:		work0,1,2		; OBJ_ID, MAP_ID, DIV_ID
//	Out:	u32				; ����ܰ�����
//------------------------------------------
u32 FeTrainerHideRockSet( void )
{
	return( SetTrainerHideActor(FECODE_TrainerHideRock,EVACT_HIDEROCK,OTHERS_OBJ_PALNO_1) );
}

//------------------------------------------
//	̨���޴̪�� �ڰŰ�����
//	In:		work0,1,2		; OBJ_ID, MAP_ID, DIV_ID
//	Out:	u32				; ����ܰ�����
//------------------------------------------
u32 FeTrainerHideSandSet( void )
{
	return( SetTrainerHideActor(FECODE_TrainerHideSand,EVACT_HIDESAND,OTHERS_OBJ_PALNO_0) );
}

//------------------------------------------
//	�ڰŰ�B�ꖪ���
//------------------------------------------
static u32 SetTrainerHideActor( u8 fcode, u8 acode, u8 palno )
{
	u8 i;
	actWork *act;

	if( EvObjIdPosGet(FeSetWork[0],FeSetWork[1],FeSetWork[2],&i) )
	{
		FeCodeDel( fcode );
		return( ACT_MAX );
	}

	i = AddActorBottom( EvActHeadGet(acode), 0, 0, 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->scr++;
		act->oamData.Pltt = palno;
		act->work[1] = fcode;
		act->work[2] = FeSetWork[0];
		act->work[3] = FeSetWork[1];
		act->work[4] = FeSetWork[2];
	}

	return( i );
}

//------------------------------------------
//	�ڰŰ�B�ꖪ����
//------------------------------------------
void EvActTrainerHideMove( actWork *act )
{
	u8 i;
	actWork *m_act;
	SXYObjHead *objst;

	if( EvObjIdPosGet(act->work[2],act->work[3],act->work[4],&i) )
	{
		FeDelActCode( act, act->work[1] );
	}

	objst = SxyObjHeadGet( EvObj[i].obj_code );
	m_act = &ActWork[EvObj[i].actno];

	act->banish = m_act->banish;
	act->x = m_act->x;
	act->y = m_act->y + (objst->y_size >> 1) - 16;
	act->pri = m_act->pri - 1;

	if( act->work[0] == 1 )
	{
		act->work[0]++;
		ActAnmChg( act, 1 );
	}
	
	if( act->work[0] == 2 )
	{
		if( act->anmend_sw )
		{
			act->work[7] = 1;
		}
	}
	
	if( act->work[0] == 3 )
	{
		FeDelActCode( act, act->work[1] );
	}
}

//------------------------------------------
//	�B�ꖪ�������쾯�
//	In:		ev			; �B�ꖪ�g�pOBJ
//------------------------------------------
void EvActHideClsSet( EventOBJ *ev )
{
	if( ev->work0 == HIDE_ON )
	{
		ActWork[ev->joint_actno[0]].work[0]++;
	}
}

//------------------------------------------
//	�B�ꖪ��������
//	In:		ev			; �B�ꖪ�g�pOBJ
//	Out:	u8			; On = �I��
//------------------------------------------
u8 EvActHideClsChk( EventOBJ *ev )
{
	actWork *act;

	if( ev->work0 == HIDE_OFF ||
		ev->work0 == HIDE_NOTHING )
	{
		return( 1 );
	}

	act = &ActWork[ev->joint_actno[0]];

	if( act->work[7] )
	{
		ev->work0 = HIDE_OFF;
		act->work[0]++;
		return( 1 );
	}

	return( 0 );
}

//======================================================================
//		�����
//======================================================================
//------------------------------------------
//	̨���޴̪�� ����ؾ��
//	In:		work0,1,2		// ϯ��x,y,BG��ײ��è
//	Out:	nothing
//------------------------------------------
u32 FeKirariSet( void )
{
	u8 i;

	FeSetWork[0] += MAP_LEFT_MARGIN;
	FeSetWork[1] += MAP_UP_MARGIN;
	MapCellSizePosDiffOffs( (s16*)&FeSetWork[0], (s16*)&FeSetWork[1], 8, 8 );

	i = AddActorBottom( EvActHeadGet(EVACT_KIRARI),
			FeSetWork[0], FeSetWork[1], EVOBJ_HI_ACTPRI );
	
	if( i != ACT_MAX )
	{
		ActWork[i].oamData.Priority = (u8)FeSetWork[2];
		ActWork[i].scr = 1;
	}

	return( 0 );
}

//------------------------------------------
//	����ؓ���
//------------------------------------------
void EvActKirariMove( actWork *act )
{
	if( act->work[0] == 0 && act->anmend_sw )
	{
		act->banish = 1;
		act->work[0]++;
	}
	
	if( act->work[0] && ++act->work[1] >= 35 )
	{
		FeDelActCode( act, FECODE_Kirari );
	}
}

//======================================================================
//		�������ʓ���
//======================================================================
//------------------------------------------
//	��Ұ��ݏI��&��Ư��&��ײ��è�C��
//	work[0] = ����, work[1] = ����
//------------------------------------------
void EvActCommonMove0( actWork *act )
{
	if( act->anmend_sw )
	{
		FeDelActCode( act, act->work[1] );
		return;
	}
	
	EvActScrollVanish( act, 0 );
	EvActPriControl( act->work[0], act, 0 );
}

//------------------------------------------
//	��Ұ��ݏI��&��Ư��
//	work[0] = ����
//------------------------------------------
void EvActCommonMove1( actWork *act )
{
	if( act->anmend_sw )
	{
		FeDelActCode( act, act->work[0] );
		return;
	}
	
	EvActScrollVanish( act, 0 );
}

//======================================================================
//		���
//======================================================================
//------------------------------------------
//	�����OBJ����,������ײ��è���
//------------------------------------------
static void EvObjChkActPriSet( actWork *act, u8 height, u8 offs_pri )
{
	u8 i;
	s16 p0,p1,p2,p3;
	EventOBJ *ev;
	actWork *evact;
	SXYObjHead *objst;

	EvActPriControl( height, act, offs_pri );

	for( i = 0; i < EV_OBJ_MAX; i++ )						// �����蔻��
	{
		ev = &EvObj[i];

		if( ev->sw )
		{
			objst = SxyObjHeadGet( ev->obj_code );
			evact = &ActWork[ev->actno];

			p0 = act->x + act->cx;
			p1 = act->x - act->cx;
			
			if( p0 < evact->x && p1 > evact->x )			// ����
			{
				p0 = evact->y + evact->cy;					// OBJ �㔼�g
				p1 = evact->y;
				p3 = act->y - act->cy;
				p2 = p3 + evact->cy;

				if( ((p0 < p2 && p1 > p2) || (p0 < p3 && p1 > p2)) &&
					act->pri <= evact->pri )
				{
					act->pri = evact->pri + 2;
					break;
				}
			}
		}
	}
}

