//**********************************************************************
//		evobj.c
//		̨����OBJ
//**********************************************************************
#define		EVOBJ_H_PROTO
#include	<agb.h>
#include	"common.h"
#include	"actor.h"
#include	"actanm.h"
#include	"evobj.h"
#include	"jiki.h"
#include	"palanm.h"
#include	"map.h"
#include	"fieldmap.h"
#include	"field.h"
#include	"fld_tool.h"
#include	"ev_flag.h"
#include	"ev_fight.h"
#include	"seed.h"
#include	"mul_play.h"

//----------------------------------------------------------------------
//		��۰���
//----------------------------------------------------------------------
EventOBJ	EvObj[EV_OBJ_MAX];							// ����� OBJ ܰ�
MvCmd		MyMoveState =								// ���@ܰ�
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
};

#ifdef PM_DEBUG
u8			EvObjCount;									// ����� OBJ ����(���ޯ��)
#endif

//----------------------------------------------------------------------
//		۰��
//----------------------------------------------------------------------
static u8  EvObjColorMode;								// ���݂���گ�Ӱ��
static u16 EvObjSpPalID;								// �o�^������OBJ��گ�ID

//----------------------------------------------------------------------
//		���ޯ��
//----------------------------------------------------------------------
#define	EVOBJ_STAY_PAL_DEBUG	1						// �풓��گ�

//----------------------------------------------------------------------
//		define
//----------------------------------------------------------------------
#define	OBJ_MAPOUT_MARGIN_V_UP		(2)
#define OBJ_MAPOUT_MARGIN_V_DOWN	(2)
#define OBJ_MAPOUT_MARGIN_H			(2)

#define EVOBJ_MV_LIM_CHECK_ON		1

#define SXY_OBJDATA_GET(sxy,i) sxy=&(Fld.SvSxyObj[i])	// SXY�ް��擾

//----------------------------------------------------------------------
//		�O���Q�Ɗ֐�,�ϐ�
//----------------------------------------------------------------------
extern s16 FieldActOX;
extern s16 FieldActOY;
extern s16 TransAreaFlagChk( u16 );

//----------------------------------------------------------------------
//		����
//----------------------------------------------------------------------
static const evObjData * SxyObjDataGet( u8, u8, u8 );

//======================================================================
//		̨����OBJ ܰ�
//======================================================================
static u8 evobj_id_seek( u8, u8, u8 );
static u8 evobj_spid_seek( u8 );
static u8 evobj_work_seek( u16, u8, u8, u8 * );
static void EvObjStayActorSet( void );
static void evobj_delact( EventOBJ * );

//------------------------------------------
//	����� OBJ ܰ�������
//	In:	ev		; ܰ����ڽ
//------------------------------------------
void EvObjWorkInit( EventOBJ *ev )
{
	memset( (u8*)ev, 0, EVOBJ_SIZE );
	
	ev->objID = 0xff;
	ev->mapID = 0xff;
	ev->divID = 0xff;
	ev->acmd = NOT_ACMD;
}

//------------------------------------------
//	����� OBJ ܰ��S������
//------------------------------------------
void EvObjWorkAllInit( void )
{
	u8 i;

	for( i = 0; i < EV_OBJ_MAX; EvObjWorkInitNo(i), i++ );

#ifdef PM_DEBUG
	EvObjCount = 0;
#endif
}

//------------------------------------------
//	�����OBJ ܰ��S������,�풓�������
//------------------------------------------
void EvObjInit( void )
{
	InitAllPlayer();
	EvObjWorkAllInit();
	MyMoveStateInit();
	EvObjStayActorSet();
}

//------------------------------------------
//	�풓�������
//------------------------------------------
static void EvObjStayActorSet( void )
{
	u8 i;
	const actHeader *actH;

	actH = (const actHeader *)EvActHeadGet( EVACT_REFAFF );
	
	i = AddActorBottom( actH, 0, 0, 31 );

	ActWork[i].oamData.AffineMode = 1;
	AffineNumSet( &ActWork[i] );
	ActAffAnmChg( &ActWork[i], 0 );
	ActWork[i].banish = 1;
	
	i = AddActorBottom( actH, 0, 0, 31 );
	ActWork[i].oamData.AffineMode = 1;
	AffineNumSet( &ActWork[i] );
	ActAffAnmChg( &ActWork[i], 1 );
	ActWork[i].banish = 1;
}

//------------------------------------------
//	��ܰ����
//	Out:		u8		; ��ܰ�����
//						; EV_OBJ_MAX = ERROR
//------------------------------------------
u8 EvObjEmptyWorkSeek( void )
{
	u8 i;
	
	for( i = 0; i < EV_OBJ_MAX && EvObj[i].sw; i++ );
	
	return( i );
}

//------------------------------------------
//	�w�� ID -> ܰ�����
//	In:		objID			; OBJ ID
//			mapID			; MAP ID
//			divID			; DIV ID
//	Out:	u8				; ܰ�����
//							; EV_OBJ_MAX = ERROR
//------------------------------------------
u8 EvObjIdSeek( u8 objID, u8 mapID, u8 divID )
{
	u8 i;
	
	if( objID >= EV_OBJ_SPID )
	{
		i = evobj_spid_seek( objID );
	}
	else
	{
		i = evobj_id_seek( objID, mapID, divID );
	}

	return( i );
}

//------------------------------------------
//	ID -> ܰ�����, �׸�
//------------------------------------------
u8 EvObjIdPosGet( u8 objID, u8 mapID, u8 divID, u8 *pos )
{
	*pos = EvObjIdSeek( objID, mapID, divID );
	
	if( *pos == EV_OBJ_MAX )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	ϯ�ߍ��W -> ܰ�����
//------------------------------------------
u8 EvObjWorkMapPosGet( s16 x, s16 y )
{
	u8 i;
	
	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( EvObj[i].sw &&
			EvObj[i].gx == x && EvObj[i].gy == y )
		{
			return( i );
		}
	}
	
	return( i );
}

//------------------------------------------
//	ܰ����
//------------------------------------------
static u8 evobj_id_seek( u8 objID, u8 mapID, u8 divID )
{	
	u8	i;
	EventOBJ *ev;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		ev = &EvObj[i];
		
		if( ev->sw && ev->objID == objID &&
			ev->mapID == mapID && ev->divID == divID )
		{
			return( i );
		}
	}

	return( EV_OBJ_MAX );
}

//------------------------------------------
//	����ID (��l���Ȃ�)
//------------------------------------------
static u8 evobj_spid_seek( u8 objID )
{
	u8	i;
	EventOBJ *ev;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		ev = &EvObj[i];

		if( ev->sw && ev->objID == objID )
		{
			return( i );
		}
	}
	
	return( EV_OBJ_MAX );
}

//------------------------------------------
//	�����OBJ ���
//	In:		evH		; SXY OBJ �ð��
//			map_id	; Map ID
//			div_id	; Div ID
//	Out:	u8		; �o�^�ԍ�
//					; EV_OBJ_MAX = Error
//-----------------------------------------
u8 EvObjAddWork( const evObjData *evH, u8 map_id, u8 div_id )
{
	EventOBJ *ev;
	u8	no;
	s16	x,y;

	if( evobj_work_seek(evH->id,map_id,div_id,&no) )
	{
		return( EV_OBJ_MAX );
	}

	ev = &EvObj[no];

	EvObjWorkInit( ev );
	
	x = evH->pos_x + MAP_LEFT_MARGIN;						// ϯ�ߍ��W�C��
	y = evH->pos_y + MAP_UP_MARGIN;
	
	ev->sw = 1;
	ev->move_start_sw = 1;
	ev->obj_code = evH->obj_code;
	ev->mv_code = evH->site;
	ev->objID = evH->id;
	ev->mapID = map_id;
	ev->divID = div_id;
	ev->init_x = x;
	ev->init_y = y;
	ev->gx = x;
	ev->gy = y;
	ev->old_gx = x;
	ev->old_gy = y;
	ev->height = evH->height;
	ev->height_pri = evH->height;
	ev->mvlim_x = evH->mvlim_x;
	ev->mvlim_y = evH->mvlim_y;
	ev->ev_type = evH->ev_type;
	ev->ev_work = evH->work;
	ev->mapID = map_id;
	ev->old_site = EvObjDefaultSite( evH->site );
	EvObjSiteSet( ev, ev->old_site );
	EvObjCodeWorkChk( ev );

#if EVOBJ_MV_LIM_CHECK_ON								// SXY �ݒ�н�Ώ�
	if( EvObjMoveLimitFlagTbl[ev->mv_code] )
	{
		if( ev->mvlim_x == 0 ) ev->mvlim_x++;
		if( ev->mvlim_y == 0 ) ev->mvlim_y++;
	}
#endif

#ifdef PM_DEBUG
	EvObjCount++;
#endif

	return( no );
}

//------------------------------------------
//	�����OBJܰ��ǉ�
//	In:		objID	; OBJ ID
//	Out:	u8		; ܰ�����
//					; EV_OBJ_MAX = Error
//	Info:	���݂�ϯ�߂Œǉ�
//-----------------------------------------
u8 EvObjWorkAdd( u8 objID )
{
	u8	i,count;
	const evObjData *sxy;

	if( NowMap.Sxy == NULL ) return( EV_OBJ_MAX );
	count = NowMap.Sxy->ObjMax;								// �o�^�ő�

	for( i = 0; i < count; i++ )
	{
		SXY_OBJDATA_GET( sxy, i );

		if( sxy->id == objID && EventFlagCheck(sxy->ev_flag) == 0 )
		{
			return( EvObjAddWork(sxy,(u8)Fld.MapID.map_id,(u8)Fld.MapID.div_id) );
		}
	}

	return( EV_OBJ_MAX );
}

//------------------------------------------
//	�����OBJ ܰ� ����
//------------------------------------------
static u8 evobj_work_seek( u16 id, u8 map_id, u8 div_id, u8 *no )
{
	u8	i;
	
	for( i = 0; i < EV_OBJ_MAX && EvObj[i].sw; i++ )	// OBJ ܰ�����
	{
		if( EvObj[i].objID == id &&
			EvObj[i].mapID == map_id &&
			EvObj[i].divID == div_id )
		{
			return( 1 );								// ���݂��Ă���
		}
	}

	if( i >= EV_OBJ_MAX )
	{
		return( 1 );
	}

	*no = i;											// No

	for( ; i < EV_OBJ_MAX; i++ )						// �c��
	{
		if( EvObj[i].sw &&
			EvObj[i].objID == id &&
			EvObj[i].mapID == map_id &&
			EvObj[i].divID == div_id )
		{
			return( 1 );								// ���݂��Ă���
		}
	}

	return( 0 );
}

//------------------------------------------
//	�����OBJ �폜, ���ڽ
//	In:		ev		; �����ܰ�
//------------------------------------------
void EvObjDel( EventOBJ *ev )
{
	ev->sw = 0;
	evobj_delact( ev );

#ifdef PM_DEBUG
	EvObjCount--;										// ���ޯ��
#endif
}

//------------------------------------------
//	�����OBJ �폜,�׸޾��
//	In:		objID		; OBJ ID
//			mapID		; MAP ID
//			divID		; DIV ID
//------------------------------------------
void EvObjDelId( u8 objID, u8 mapID, u8 divID )
{
	u8 evno;

	if( EvObjIdPosGet(objID,mapID,divID,&evno) )
	{
		return;
	}

	EventFlagSet( EvObjNoEvFlagGet(evno) );

	EvObjDel( &EvObj[evno] );
}

//------------------------------------------
//	�����OBJ �t�������폜
//------------------------------------------
static void evobj_delact( EventOBJ *ev )
{
	SXYObjHead *objst;
	actAnmData dmy_t[1];

	objst = SxyObjHeadGet( ev->obj_code );
	dmy_t[0].size = objst->celltrans_size;
	ActWork[ev->actno].anm_trans = (const actAnmData*)dmy_t;
	DelActor( &ActWork[ev->actno] );
}

//------------------------------------------
//	�����OBJ �S�폜(��l���ȊO)
//------------------------------------------
void EvObjDelMap( void )
{
	u8	i;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( i != MyMoveState.evobjno )
		{
			EvObjDelNo( i );
		}
	}
}

//------------------------------------------
//	����� OBJ, ���� �o�^
//	In:		evH			; SXY OBJ �ð��
//			actH		; ����ͯ��
//			mapID,divID	; Map ID, Div ID
//			dx,dy		; ���W�C���l
//	Out:	u8			; �����OBJܰ�����
//						; EV_OBJ_MAX = Error
//------------------------------------------
u8 EvObjAdd( const evObjData *evH, actHeader *actH, u8 mapID, u8 divID, s16 dx, s16 dy )
{
	u8	evno, actno;
	SXYObjHead *objst;
	actWork *act;
	EventOBJ *ev;

	evno = EvObjAddWork( evH, mapID, divID );

	if( evno == EV_OBJ_MAX )
	{
		return( evno );
	}

	ev = &EvObj[evno];

	objst = SxyObjHeadGet( ev->obj_code );

	if( objst->oampalNo == MINE_OBJ_PALNO )
	{
		EvObjHeroPalSet( objst->palNo, objst->oampalNo );
	}
	else if( objst->oampalNo == SP_OBJ_PALNO )
	{
		EvObjSpPalSet( objst->palNo, objst->oampalNo );
	}

	if( ev->mv_code == MV_VANISH )
	{
		ev->banish_sw = 1;
	}

	*(u16*)&(actH->palNo) = ACT_OAMPAL;

	actno = AddActor( actH, 0, 0, 0 );

	if( actno == ACT_MAX )
	{
#ifdef PM_DEBUG
		EvObjCount--;
#endif
		EvObj[evno].sw = 0;
		return( EV_OBJ_MAX );
	}

	act = &ActWork[actno];
	
	MapCellSizePos( ev->gx + dx, ev->gy + dy, &act->x, &act->y );
	act->cx = -(objst->x_size >> 1);
	act->cy = -(objst->y_size >> 1);
	act->x += 8;										// ���S�␳
	act->y += 16 + act->cy;								// ���ꂩ��
	
	act->oamData.Pltt = objst->oampalNo;				// EVOBJ_STAY_PAL_DEBUG
	act->scr = 1;										// ��۰وˑ�
	act->work[0] = evno;								// ܰ�����

	ev->actno = actno;									// ����ܰ�����
	ev->anm_change_sw = objst->anm_change_sw;			// ����ݼ���̫�Ľ���

	if( ev->anm_change_sw == 0 )
	{
		ActAnmChg( act, EvActAnmNoStop(ev->site) );
	}

	EvActPriControl( ev->height_pri, act, 1 );
	EvObjBanish( ev, act );

	return( evno );
}

//------------------------------------------
//	SXY -> �����OBJ, ���� �o�^
//	In:		evH			; �����OBJ SXY �ް�
//			mapID,divID	; Map ID, Div ID
//			dx,dy		; ���W�C���l
//	Out:	u8			; �����OBJܰ�����
//						; EV_OBJ_MAX = Error
//------------------------------------------
u8 EvObjSxyAdd( const evObjData *evH, u8 mapID, u8 divID, s16 dx, s16 dy  )
{
	u8 pos;
	actHeader	actH;
	OAMPatSt	*pat;
	SXYObjHead	*objst;
	actAnmData	dmy_cell[1];

	pat = NOT_OAMPAT;
	objst = SxyObjHeadGet( evH->obj_code );
	MakeSxyActHead( evH, &actH, &pat );						// ����ͯ�ސ���
	dmy_cell[0].size = objst->celltrans_size;
	actH.anmTrans_p = (const actAnmData*)dmy_cell;
	
	pos = EvObjAdd( evH, &actH, mapID, divID, dx, dy );		// OBJ �o�^
	
	if( pos == EV_OBJ_MAX )
	{
		return( pos );
	}
	
	ActWork[EvObj[pos].actno].anm_trans = objst->anmTrans_p;

	if( pat != NOT_OAMPAT )									// �߯�
	{
		SetActOamPat( &ActWork[EvObj[pos].actno], (const OAMPatSt*)pat );
	}

	return( pos );
}

//------------------------------------------
//	SXY -> �����OBJ�o�^(�ȈՔ�)
//	In:		evH		; �����OBJ SXY �ް�
//	Out:	u8		; �����OBJܰ�����
//					; EV_OBJ_MAX = Error
//	Info:
//		���݂�ϯ�ߏ��œo�^
//------------------------------------------
u8 EvObjSxyAddNow( const evObjData *evH  )
{
	s16 dx,dy;
	
	MapCellSizeDiff( &dx, &dy );

	return( EvObjSxyAdd(evH,Fld.MapID.map_id,
				Fld.MapID.div_id, dx, dy) );
}

//------------------------------------------
//	SXY�ް����� -> �����OBJ�o�^
//	In:		obj_code		; OBJ��׺���
//			mv_code			; ���캰��
//			obj_id			; OBJ ID
//			mx,my			; ϯ�ߍ��W
//			height			; ����
//	Out:	u8				; �����OBJܰ�����
//							; EV_OBJ_MAX = Error
//	Info: ���݂�ϯ�ߏ��œo�^
//------------------------------------------
u8 EvObjMakeSxyAdd( u8 obj_code, u8 mv_code, u8 obj_id, s16 mx, s16 my, u8 height )
{
	evObjData data;

	mx -= MAP_LEFT_MARGIN;
	my -= MAP_UP_MARGIN;
	
	data.id = obj_id;
	data.obj_code = obj_code;
	data.col_code = 0;
	data.pos_x = mx;
	data.pos_y = my;
	data.height = height;
	data.site = mv_code;
	data.mvlim_x = 0;
	data.mvlim_y = 0;
	data.ev_type = OTYPE_NORMAL;
	data.work = 0;

	return( EvObjSxyAddNow((const evObjData*)&data) );
}

//------------------------------------------
//	ID -> �����OBJ �o�^
//	In:		objID		; OBJ ID
//			mapID		; MAP ID
//			divID		; DIV ID
//	Out:	u8			; �����OBJܰ�����
//						; EV_OBJ_MAX = ERROR
//------------------------------------------
u8 EvObjAddId( u8 objID, u8 mapID, u8 divID )
{
	s16 mx,my;
	evObjData *sxy;

	sxy = (evObjData *)SxyObjDataGet( objID, mapID, divID );

	if( sxy == 0 )
	{
		return( EV_OBJ_MAX );							// �Y������
	}
	
	MapCellSizeDiff( &mx, &my );						// ���W�C���l
	return( EvObjSxyAdd((const evObjData *)sxy,mapID,divID,mx,my) );
}

//------------------------------------------
//	����ͯ�ނ𐶐�
//	In:		code	; OBJ ����
//			move	; ����֐�
//			Head	; ������
//------------------------------------------
void EvMoveMakeHead( u16 code, void (*move)(actWork *), actHeader *Head, OAMPatSt **pat )
{
	SXYObjHead *SxyHead;
	
	SxyHead = SxyObjHeadGet( code );

	*(u16 *)&(Head->cellNo) = SxyHead->cellNo;				// const u16 -> u16 cast
	*(u16 *)&(Head->palNo) = SxyHead->palNo;
	Head->oamData_p = SxyHead->oamData_p;					// OAM
	Head->anmData_p = SxyHead->anmData_p;					// ���
	Head->anmTrans_p = SxyHead->anmTrans_p;					// �����ݽ
	Head->affData_p = SxyHead->affData_p;					// �̨�
	Head->moveData_p = move;								// ����
	*pat = (OAMPatSt*)SxyHead->oamPat;						// OAM �߯�
}

//------------------------------------------
//	����ͯ�ސ���, ����֐� ����
//	In:	code	; OBJ ����
//		mcode	; ���캰��
//		Head	; ������
//------------------------------------------
void EvMakeHead( u16 code, u16 mcode, actHeader *Head, OAMPatSt **pat )
{
	EvMoveMakeHead( code, EvObjMoveTbl[mcode], Head, pat );
}

//------------------------------------------
//	SXY �ް����� ����ͯ�ނ𐶐�
//	In:	sxy			; SXY �ް�
//		Head		; ͯ�ސ�����
//------------------------------------------
void MakeSxyActHead( const evObjData *sxy, actHeader *Head, OAMPatSt **pat )
{
	EvMakeHead( sxy->obj_code, sxy->site, Head, pat );

#if 0														// �]���^
	if( sxy->col_code )										// �װ�w��L��
	{
		*(u16 *)&(Head->palNo) = (u16)sxy->col_code;
	}
#endif
}

//------------------------------------------
//	����ͯ�ނ𐶐����ǉ�
//	In:		code	; OBJ ����
//			move	; ��������֐�
//			x,y,pri	; ���W,��ײ��è
//	Out:	u8		; ����ܰ�����
//	Info:	��گ��Ȱ�ެ�g�p
//------------------------------------------
u8 EvObjMakeAddAct( u16 code, void (*move)(actWork *), s16 x, s16 y, u8 pri )
{
	u8	i;
	actHeader head;
	OAMPatSt *pat;
	
	EvMoveMakeHead( code, move, &head, &pat );

	if( head.palNo != ACT_OAMPAL )
	{
		EvObjPalSet( head.palNo );							// ��گľ��
	}

	i =  AddActor( (const actHeader*)&head, x, y, pri );
	
	if( i != ACT_MAX && pat != NOT_OAMPAT )					// �߯�
	{
		SetActOamPat( &ActWork[i], (const OAMPatSt*)pat );
		ActWork[i].oampat_sw = OAMPAT_PRIUNIT;
	}
	
	return( i );
}

//------------------------------------------
//	̨����OBJ��а�����ǉ�
//	In:		code	; OBJ����
//			id		; ���� ID
//			x,y,h	; ϯ��X,Y���W,����
//			site	; ����
//	Out:	u8		; ���ܰ�����
//------------------------------------------
u8 EvObjDmyAddAct( u8 code, u8 id, s16 x, s16 y, u8 h, u8 site )
{
	u8 i;
	actHeader actH;
	actWork *act;
	OAMPatSt *pat;
	SXYObjHead *objst;
	
	objst = SxyObjHeadGet( code );
	EvMoveMakeHead( code, EvObjDmyActMove, &actH, &pat );
	*(u16*)&(actH.palNo) = ACT_OAMPAL;
	
	x += MAP_LEFT_MARGIN;
	y += MAP_UP_MARGIN;
	MapCellSizePosDiffOffs( &x, &y, 8, 16 );
//	i = AddActor( (const actHeader*)&actH, x, y, 0 );
	i = AddActorBottom( (const actHeader*)&actH, x, y, 0 );

	if( i != ACT_MAX )
	{
		act = &ActWork[i];
		act->cx = -(objst->x_size >> 1);
		act->cy = -(objst->y_size >> 1);
		act->y += act->cy;
		act->oamData.Pltt = objst->oampalNo;
		act->scr = 1;
		act->work[0] = id;
		act->work[1] = h;

		if( objst->oampalNo == SP_OBJ_PALNO )
		{
			EvObjSpPalSet( objst->palNo, objst->oampalNo );
		}

		if( pat != NOT_OAMPAT )
		{
			SetActOamPat( act, (const OAMPatSt*)pat );
			act->oampat_sw = OAMPAT_PRIUNIT;
		}

		EvActHeightSet( act, h );
		EvActPriControl( h, act, 1 );
		ActAnmChg( act, EvActAnmNoStop(site) );
	}

	return( i );
}

//------------------------------------------
//	ϯ���ް��@�����OBJ�o�^
//	In:		dx,dy			; �C�����W
//------------------------------------------
void EvObjAddControl( s16 dx, s16 dy )
{
	u8	i,count;
	s16 mx0,mx1,my0,my1,px,py;
	const evObjData *sxy;

	if( NowMap.Sxy == NULL )
	{
		return;												// �ݒ薳��
	}

	mx0 = Fld.MapXpos - OBJ_MAPOUT_MARGIN_H;
	mx1 = Fld.MapXpos + SCR_CELL_X + OBJ_MAPOUT_MARGIN_H;
	my0 = Fld.MapYpos + VIEW_UP_MARGIN - OBJ_MAPOUT_MARGIN_V_UP;
	my1 = Fld.MapYpos + VIEW_UP_MARGIN + SCR_CELL_Y + OBJ_MAPOUT_MARGIN_V_DOWN;
	count = NowMap.Sxy->ObjMax;								// �o�^�ő�

	for( i = 0; i < count; i++ )
	{
		SXY_OBJDATA_GET( sxy, i );
		
		px = sxy->pos_x + MAP_LEFT_MARGIN;
		py = sxy->pos_y + MAP_UP_MARGIN;
		
		if( my0 <= py && my1 >= py && mx0 <= px && mx1 >= px &&
			EventFlagCheck(sxy->ev_flag) == 0 )
		{
			EvObjSxyAdd( sxy, (u8)Fld.MapID.map_id,	(u8)Fld.MapID.div_id, dx, dy );
		}
	}
}

//------------------------------------------
//	ϯ�߽�۰� OBJ �폜
//------------------------------------------
static void evobj_delcont_chk( EventOBJ * );

static void EvObjDelControl( void )
{
	u8 i,pos,flag;
	EventOBJ *ev;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		for( pos = 0, flag = 0; pos < COMM_PLAYER_MAX; pos++ )	// �� �������׸ވʒu���܂Ƃ߂�
		{
			if( Player[pos].sw && i == Player[pos].evobjno )
			{
				flag = 1;
			}
		}

		if( flag == 0 )
		{
			ev = &EvObj[i];

			if( ev->sw && ev->scr_del_sw == 0 )
			{
				evobj_delcont_chk( ev );
			}
		}
	}
}

//------------------------------------------
//	OBJ �폜 ����
//------------------------------------------
static void evobj_delcont_chk( EventOBJ *ev )
{
#if 1
	s16 x0,y0,x1,y1;

	x0 = Fld.MapXpos - OBJ_MAPOUT_MARGIN_H;
	x1 = Fld.MapXpos + OBJ_MAPOUT_MARGIN_H + SCR_CELL_X;
	y0 = Fld.MapYpos + VIEW_UP_MARGIN - OBJ_MAPOUT_MARGIN_V_UP;
	y1 = Fld.MapYpos + VIEW_UP_MARGIN + OBJ_MAPOUT_MARGIN_V_DOWN + SCR_CELL_Y;

	if( (ev->gx < x0 || ev->gx > x1 || ev->gy < y0 || ev->gy > y1) &&
		(ev->init_x < x0 || ev->init_x > x1 || ev->init_y < y0 || ev->init_y > y1) )
	{
		EvObjDel( ev );
	}
#elif 0
	s16 x0,y0,x1,y1;

	x0 = Fld.MapXpos - OBJ_MAPOUT_MARGIN_H;
	x1 = Fld.MapXpos + OBJ_MAPOUT_MARGIN_H + SCR_CELL_X;
	y0 = Fld.MapYpos - OBJ_MAPOUT_MARGIN_V_UP;
	y1 = Fld.MapYpos + OBJ_MAPOUT_MARGIN_V_DOWN + SCR_CELL_Y;

	if( (ev->gx < x0 || ev->gx > x1 || ev->gy < y0 || ev->gy > y1) &&
		(ev->init_x < x0 || ev->init_x > x1 || ev->init_y < y0 || ev->init_y > y1) )
	{
		EvObjDel( ev );
	}
#else
	if( (ev->gx < Fld.MapXpos - OBJ_MAPOUT_MARGIN_H ||
			ev->gx > Fld.MapXpos + SCR_CELL_X + OBJ_MAPOUT_MARGIN_H  ||
			ev->gy < Fld.MapYpos - OBJ_MAPOUT_MARGIN_V_UP ||
			ev->gy > Fld.MapYpos + SCR_CELL_Y + OBJ_MAPOUT_MARGIN_V_DOWN) &&
		(ev->init_x < Fld.MapXpos - OBJ_MAPOUT_MARGIN_H ||
			ev->init_x > Fld.MapXpos + SCR_CELL_X + OBJ_MAPOUT_MARGIN_H  ||
			ev->init_y < Fld.MapYpos - OBJ_MAPOUT_MARGIN_V_UP ||
			ev->init_y > Fld.MapYpos + SCR_CELL_Y + OBJ_MAPOUT_MARGIN_V_DOWN) )
	{
		EvObjDel( ev );
	}
#endif
}

//======================================================================
//		���A����
//======================================================================
static void EvObjRecoverSet( u8, s16, s16 );
static void evobj_rec_work_init( EventOBJ * );
static void MyMoveStateSet( u8, u8 );

//------------------------------------------
//	����� OBJ ���A����
//	In:		dx,dy		; �\���C�����W
//------------------------------------------
void EvObjRecover( s16 dx, s16 dy )
{
	u8 i;

#ifdef PM_DEBUG
	EvObjCount = 0;
#endif

	MyMoveStateInit();

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( EvObj[i].sw )
		{
			EvObjRecoverSet( i, dx, dy );
#ifdef PM_DEBUG
			EvObjCount++;
#endif
		}	
	}

	EvObjStayActorSet();
}

//------------------------------------------
//	����� OBJ ���A���� ܰ����
//------------------------------------------
static void EvObjRecoverSet( u8 evpos, s16 dx, s16 dy )
{
	u8	pos;
	actHeader 	actH;
	OAMPatSt 	*pat;
	SXYObjHead	*objst;
	actWork *act;
	EventOBJ *ev;
	actAnmData	dmy_cell[1];
														
	for( pos = 0; pos < COMM_PLAYER_MAX; pos++ )			// �ʐM�pOBJ����
	{
		if( Player[pos].sw && evpos == Player[pos].evobjno )
		{
			return;
		}
	}

	ev = &EvObj[evpos];
	pat = NOT_OAMPAT;
	objst = SxyObjHeadGet( ev->obj_code );
	dmy_cell[0].size = objst->celltrans_size;
	EvMakeHead( ev->obj_code, ev->mv_code, &actH, &pat );	// ����ͯ�ސ���
	actH.anmTrans_p = (const actAnmData*)dmy_cell;
	*(u16*)&(actH.palNo) = ACT_OAMPAL;						// PALTEST

	if( objst->oampalNo == MINE_OBJ_PALNO )
	{
		EvObjHeroPalSet( objst->palNo, objst->oampalNo );
	}

	if( objst->oampalNo >= SP_OBJ_PALNO )
	{
		EvObjSpPalSet( objst->palNo, objst->oampalNo );
	}

	*(u16*)&(actH.palNo) = ACT_OAMPAL;						// EVOBJ_STAY_PAL_DEBUG

	pos = AddActor( &actH, 0, 0, 0 );						// �����ǉ�

	if( pos == ACT_MAX )
	{
		return;
	}

	act = &ActWork[pos];
	
	MapCellSizePos( ev->gx + dx, ev->gy + dy, &act->x, &act->y );
	act->cx = -(objst->x_size >> 1);
	act->cy = -(objst->y_size >> 1);
	act->x += 8;											// ���S�␳
	act->y += 16 + act->cy;									// ���ꂩ��

	act->anm_trans = objst->anmTrans_p;

	if( ev->mv_code == MV_HERO )
	{
		MyMoveStateSet( evpos, pos );
		ev->joint_actno[1] = EvActExitCursorSet();
	}

	if( pat != NOT_OAMPAT )
	{
		SetActOamPat( &ActWork[pos], (const OAMPatSt*)pat );
	}

	act->oamData.Pltt = objst->oampalNo;					// PALTEST
	act->scr = 1;
	act->work[0] = evpos;
	ev->actno = pos;
	
	if( ev->anm_change_sw == 0 && ev->mv_code != MV_HERO )
	{
		ActAnmChg( act, EvActAnmNoStop(ev->site) );
	}

	evobj_rec_work_init( ev );

	EvActPriControl( ev->height_pri, act, 1 );
}

//------------------------------------------
//	�׸ތn������
//------------------------------------------
static void evobj_rec_work_init( EventOBJ *ev )
{
	ev->move_sw = 0;
	ev->move_start_sw = 1;
	ev->shade_act_sw = 0;
	ev->attr_reflect_sw = 0;
	ev->attr_grass_sw = 0;
	ev->attr_shoal_sw = 0;
	ev->attr_sand_sw = 0;
	ev->attr_onsen_sw = 0;
	AcmdFreeWork( ev );
}

//------------------------------------------
//	MyMoveState ������
//------------------------------------------
static void MyMoveStateSet( u8 evpos, u8 actpos )
{
	MyMoveState.evobjno = evpos;
	MyMoveState.actno = actpos;
	MyMoveState.sex = GetMainHeroObjCodeSex( EvObj[evpos].obj_code );

	MyMoveStateObjCodeRequest( EvObj[evpos].obj_code, MY_MV_BIT_UNDER );
}

//======================================================================
//		���Ұ�
//======================================================================
//------------------------------------------
//	�����OBJ ������ύX
//	In:		ev			; �����OBJܰ�
//			code		; ��ݼ޺���
//------------------------------------------
void EvObjChgParam( EventOBJ *ev, u8 code )
{
	SXYObjHead *objst;
	actWork *act;

	objst = SxyObjHeadGet( code );
	act = &ActWork[ev->actno];

	if( objst->oampalNo == MINE_OBJ_PALNO )
	{
//		EvObjHeroPalSet( objst->palNo, objst->oampalNo );
		EvObjStayPalSet( objst->palNo, objst->oampalNo );
	}

	if( objst->oampalNo == SP_OBJ_PALNO )
	{
		EvObjSpPalSet( objst->palNo, objst->oampalNo );
	}

	act->oamData.Shape = objst->oamData_p->Shape;
	act->oamData.Size = objst->oamData_p->Size;
	act->anm_trans = objst->anmTrans_p;
	act->anm_tbl = objst->anmData_p;
	act->oamPat = objst->oamPat;
	act->oamData.Pltt = objst->oampalNo;
	
	ev->anm_change_sw = objst->anm_change_sw;
	ev->obj_code = code;
	
	MapCellSizePosDiff( ev->gx, ev->gy, &act->x, &act->y );
	act->cx = -(objst->x_size >> 1);
	act->cy = -(objst->y_size >> 1);
	act->x += 8;										// ���S�␳
	act->y += 16 + act->cy;								// ���ꂩ��

	if( ev->scr_main_sw )
	{
		FldScrollPosInit();
	}
}

//------------------------------------------
//	�����OBJ ������ύX, ID
//	In:		id			; OBJ ID
//			map, div	; MAP ID, DIV ID
//			code		; ��ݼ�OBJ����
//------------------------------------------
void EvObjChgParamId( u8 id, u8 map, u8 div, u8 code )
{
	u8 pos;
	
	if( EvObjIdPosGet(id,map,div,&pos) )
	{
		return;
	}
	
	EvObjChgParam( &EvObj[pos], code );
}

//------------------------------------------
//	�����OBJ �����ύX
//	In:		ev			; �����OBJܰ�
//			site		; �w�����
//------------------------------------------
void EvObjChgSite( EventOBJ *ev, u8 site )
{
	EvObjSiteSet( ev, site );
	
	if( ev->anm_change_sw == 0 )
	{
		ActAnmChg( &ActWork[ev->actno], EvActAnmNoStop(ev->site) );
		ActAnmOffsChg( &ActWork[ev->actno], 0 );
	}
}

//------------------------------------------
//	�����OBJ ������ύX, ID
//	In:		id			; OBJ ID
//			map, div	; MAP ID, DIV ID
//			code		; ����
//------------------------------------------
void EvObjChgSiteId( u8 id, u8 map, u8 div, u8 site )
{
	u8 pos;

	if( EvObjIdPosGet(id,map,div,&pos) )
	{
		return;
	}

	EvObjChgSite( &EvObj[pos], site );
}

//------------------------------------------
//	MvCmd �����^ �����ύX
//------------------------------------------
void MvCmdChgSite( MvCmd *myst, u8 site )
{
	EvObjChgSite( &EvObj[myst->evobjno], site );
}

//------------------------------------------
//	���˱�Ұ�����������
//------------------------------------------
void EvObjSeedSetParam( EventOBJ *ev, actWork *act )
{
	u8 seed,speed;

	ev->banish_sw = 1;
	act->banish = 1;
	speed = GetSeedGrowth( ev->ev_work );

	if( speed == SEEDSTAT_NOTHING )						// �A���ĂȂ�
	{
		return;
	}

	ev->banish_sw = 0;
	act->banish = 0;
	seed = GetSeedType( ev->ev_work ) - 1;	 			// ���ˎ�� 1 origin
	speed -= SEEDSTAT_UNDERGROUND;						// ���� - ����y = �e����

	if( seed > SEED_TYPE_MAX )							// error�Ώ�
	{
		seed = 0;
	}

	EvObjChgParam( ev, SeedObjCodeGet(seed,speed) );
	act->anm_trans = SeedCellTransGet( seed );
	act->oamData.Pltt = SeedPalNoGet( seed, speed );
	ActAnmChg( act, speed );
}

//------------------------------------------
//	OBJ �ð��ͯ��
//------------------------------------------
SXYObjHead * SxyObjHeadGet( u8 code )
{
	if( code >= WKOBJCODE1 )
	{
		code = GetEvDefineObjCode( code - WKOBJCODE1 );
	}

	if( code >= EVOBJCODEMAX )							// error
	{
		code = BOYA1;
	}

	return( SxyObjHeadAdrs(code) );
}

//------------------------------------------
//	OBJ ��������
//------------------------------------------
void EvObjCodeWorkChk( EventOBJ *ev )
{
	if( ev->obj_code >= WKOBJCODE1 )
	{
		ev->obj_code = GetEvDefineObjCode( ev->obj_code - WKOBJCODE1 );
	}
}

//------------------------------------------
//	OBJ ��Ư�����
//	In:		id			; OBJ ID
//			map, div	; MAP ID, DIV ID
//			sw			; ON = ��\��
//------------------------------------------
void EvObjBanishSet( u8 id, u8 map, u8 div, u8 sw )
{
	u8 pos;

	if( EvObjIdPosGet(id,map,div,&pos) )
	{
		return;
	}

	EvObj[pos].banish_sw = sw;
}

//------------------------------------------
//	OBJ ID �i�[
//	In:		ev			; �����OBJܰ�
//			work0,1,2	; ���ܰ�
//						; 0 = OBJ ID
//						; 1 = MAP ID
//						; 2 = DIV ID
//------------------------------------------
void EvObjIdWorkSet( EventOBJ *ev, void *work0, void *work1, void *work2 )
{
	*(u8*)work0 = ev->objID;
	*(u8*)work1 = ev->mapID;
	*(u8*)work2 = ev->divID;
}

//------------------------------------------
//	���̍��W�ɂ���OBJ�̓����׸�ON
//	In:		mx,my		; ����ϯ��x,y
//------------------------------------------
void EvObjMapPosMvFlagOn( s16 mx, s16 my )
{
	u8 i;
	EventOBJ *ev;

	i = EvObjWorkMapPosGet( mx, my );
	
	if( i != EV_OBJ_MAX )
	{
		ev = &EvObj[i];
		ev->move_start_sw = 1;
	}
}

//------------------------------------------
//	�\����ײ��è�ύX
//	In:		objID,mapID, divID	; OBJ,MAP,DIV ID
//			pri					; �\����ײ��è
//	Info	��ײ��è�׸� ON
//------------------------------------------
void EvObjPriChg( u8 objID, u8 mapID, u8 divID, u8 pri )
{
	u8 i;
	EventOBJ *ev;
	actWork *act;

	if( EvObjIdPosGet(objID,mapID,divID,&i) == 0 )
	{
		ev = &EvObj[i];
		act = &ActWork[ev->actno];
		ev->priority_hold_sw = 1;
		act->pri = pri;
	}
}

//------------------------------------------
//	�\����ײ��è�׸� OFF
//	In:		objID,mapID, divID	; OBJ,MAP,DIV ID
//------------------------------------------
void EvObjPriChgOff( u8 objID, u8 mapID, u8 divID )
{
	u8 i;
	EventOBJ *ev;

	if( EvObjIdPosGet(objID,mapID,divID,&i) == 0 )
	{
		ev = &EvObj[i];
		ev->priority_hold_sw = 0;
		ev->move_start_sw = 1;
	}
}

//------------------------------------------
//	�̾�č��W���
//	In:		objID,mapID,divID	; OBJ,MAP,DIV ID
//			x,y					; �ޯĵ̾��
//------------------------------------------
void EvObjSetOffset( u8 objID, u8 mapID, u8 divID, s16 x, s16 y )
{
	u8 i;
	actWork *act;
	
	if( EvObjIdPosGet(objID,mapID,divID,&i) == 0 )
	{
		act= &ActWork[EvObj[i].actno];
		act->dx = x;
		act->dy = y;
	}
}

//======================================================================
//		̨���� OBJ ��گ�
//======================================================================
#define	EvObjPalWorkSet(d,n) PaletteWorkSet(d,PA_OBJ0+((u16)n<<4),32);
static u8 evobj_palset( PalData * );					// ��گľ��

//------------------------------------------
//	��گ��Ȱ�ެ������
//------------------------------------------
void EvObjPalManInit( void )
{
	ObjPalManInit();
	ObjPalManStart = EVOBJ_PALMAN_START_NO;
}

//------------------------------------------
//	��گēo�^ �P��
//	In:		col		; ��گ� ID
//------------------------------------------
void EvObjPalSet( u16 id )
{
	u16	tbl_no;

	tbl_no = EvObjPalTblSeek( id );
	
	if( tbl_no == EVOBJ_PALEND )
	{
		return;
	}

	evobj_palset( (PalData *)&EvObjPalTbl[tbl_no] );
}

//------------------------------------------
//	��گēo�^�@����
//	In:		col		; ��گ� ID �z��
//------------------------------------------
void EvObjPalSets( u16 *data )
{
	u8	i;
	
	for( i = 0; data[i] != EVOBJ_PALEND; EvObjPalSet(data[i]), i++ );
}

//------------------------------------------
//	��گ�ܰ����
//	In:		pal			; �o�^��گ��ް�
//	Out:	u8			; �o�^��گ�����
//						; 0xff = Error
//------------------------------------------
static u8 evobj_palset( PalData *pal )
{
	if( ObjPalNumGet( pal->reg_num ) != 0xff )
	{
		return( 0xff );										// �o�^�ς�
	}
	
	return( ObjPalSet(pal) );
}

//------------------------------------------
//	�f�荞����گ�Noð���
//------------------------------------------
const u8 EvObjRefPalNoTbl[16] =
{
	MINE_REF_OBJ_PALNO,
	MINE_REF_OBJ_PALNO,
	OTHERS_REF_OBJ_PALNO_0,
	OTHERS_REF_OBJ_PALNO_1,
	OTHERS_REF_OBJ_PALNO_2,
	OTHERS_REF_OBJ_PALNO_3,
	OTHERS_REF_OBJ_PALNO_0,
	OTHERS_REF_OBJ_PALNO_1,
	OTHERS_REF_OBJ_PALNO_2,
	OTHERS_REF_OBJ_PALNO_3,
	SP_REF_OBJ_PALNO,
	SP_REF_OBJ_PALNO,
	0,
	0,
	0,
	0,
};

//------------------------------------------
//	�풓��گľ��
//	In:		id			; ��گ�ID
//			palno		; ��گ�No
//------------------------------------------
void EvObjStayPalSet( u16 id, u8 palno )
{
	u8 id_pos;

	id_pos = EvObjPalTblSeek( id );

	if( id_pos == EVOBJ_PALEND )						// �Y������
	{
		return;
	}

	EvObjPalWorkSet( EvObjPalTbl[id_pos].pal_data, palno );
}

//------------------------------------------
//	�풓��گĕ������
//------------------------------------------
void EvObjStayPalSets( const u16 *id, u8 s, u8 e )
{
	for( ; s < e; EvObjStayPalSet(*id,s), id++, s++ );
}

//------------------------------------------
//	��گ�ð��ټ��
//------------------------------------------
u8 EvObjPalTblSeek( u16 id )
{
	u8 i;

	for( i = 0; EvObjPalTbl[i].reg_num != EVOBJ_PALEND; i++ )
	{
		if( EvObjPalTbl[i].reg_num == id )
		{
			return( i );
		}
	}
	
	return( EVOBJ_PALEND );
}

//------------------------------------------
//	��l����گľ��
//------------------------------------------
void EvObjHeroPalSet( u16 id, u8 pos )
{
	u8 i;
	
	EvObjStayPalSet( id, pos );

	for( i = 0; EvObjHeroPalStTbl[i].id != EVOBJ_PALEND; i++ )
	{
		if( EvObjHeroPalStTbl[i].id == id )
		{
			EvObjStayPalSet( EvObjHeroPalStTbl[i].com_id[EvObjColorMode], EvObjRefPalNo(pos) );
			return;
		}
	}
}

//------------------------------------------
//	������گľ��
//------------------------------------------
void EvObjSpPalSet( u16 id, u8 pos )
{
	u8	i;
	
	EvObjSpPalID = id;									// ��������گ�
	EvObjStayPalSet( id, pos );							// �ʏ���گľ��

	for( i = 0; EvObjSpPalStTbl[i].id != EVOBJ_PALEND; i++ )
	{
		if( EvObjSpPalStTbl[i].id == id )				// �f�肱����گľ��
		{
			EvObjStayPalSet( EvObjSpPalStTbl[i].com_id[EvObjColorMode], EvObjRefPalNo(pos) );
			return;
		}
	}
}

//======================================================================
//		̨����OBJ ܰ� �ړ�,���W����
//======================================================================
//------------------------------------------
//	���W�ړ�,�X�V
//	In:		ev		; ܰ�
//			dx,dy	; ���W������
//------------------------------------------
void EvObjPosMove( EventOBJ *ev, s16 dx, s16 dy )
{
	ev->old_gx = ev->gx;
	ev->old_gy = ev->gy;
	ev->gx += dx;
	ev->gy += dy;
}

//------------------------------------------
//	���W�X�V
//	In:		ev		; ܰ�
//			x,y		; �V�K���W
//------------------------------------------
void EvObjPosChg( EventOBJ *ev, s16 x, s16 y )
{
	ev->old_gx = ev->gx;
	ev->old_gy = ev->gy;
	ev->gx = x;
	ev->gy = y;
}

//------------------------------------------
//	���W������
//------------------------------------------
void EvObjPosInit( EventOBJ *ev, s16 x, s16 y )
{
	ev->old_gx = x;
	ev->old_gy = y;
	ev->gx = x;
	ev->gy = y;
}

//------------------------------------------
//	���W�X�V, ϯ�ߍ��W, �\�����W
//	In:		ev		; ܰ�
//			x,y		; �w��ϯ�ߍ��W
//------------------------------------------
void EvObjPosAllChg( EventOBJ *ev, s16 x, s16 y )
{
	actWork *act;
	SXYObjHead *objst;

	act = &ActWork[ev->actno];
	objst = SxyObjHeadGet( ev->obj_code );

	EvObjPosInit( ev, x, y );
	MapCellSizePosDiff( ev->gx, ev->gy, &act->x, &act->y );
	act->cx = -(objst->x_size >> 1);
	act->cy = -(objst->y_size >> 1);
	act->x += 8;										// ���S�␳
	act->y += 16 + act->cy;								// ���ꂩ��

	evobj_rec_work_init( ev );
	
	if( ev->scr_main_sw )
	{
		FldScrollPosInit();
	}
}

//------------------------------------------
//	����ID ���W�X�V, ϯ�ߍ��W, �\�����W
//	In:		id,map,div	; OBJ ID, MAP ID, DIV ID
//			x,y			; �w��ϯ�ߍ��W
//------------------------------------------
void EvObjPosAllChgId( u8 id, u8 map, u8 div, s16 x, s16 y )
{
	u8 pos;
	
	if( EvObjIdPosGet(id,map,div,&pos) )
	{
		return;
	}
	
	x += MAP_LEFT_MARGIN;
	y += MAP_UP_MARGIN;

	EvObjPosAllChg( &EvObj[pos], x, y );
}

//------------------------------------------
//	�����W�ōX�V
//	In:		ev		; ܰ�
//------------------------------------------
void EvObjPosRenew( EventOBJ *ev )
{
	EvObjPosChg( ev, ev->gx, ev->gy );
}

//------------------------------------------
//	ϯ�ߐ؂�ւ����W�C��
//------------------------------------------
void EvObjMapChgCont( void )
{
	u8	i;
	s16	dx,dy;

	if( MapChange.chg_flag != TRUE )
	{
		return;											// �؂�ւ�薳��
	}

	dx = MapChange.diffx;								// �C���l
	dy = MapChange.diffy;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( EvObj[i].sw )
		{
			EvObj[i].init_x -= dx;
			EvObj[i].init_y -= dy;
			EvObj[i].gx -= dx;
			EvObj[i].gy -= dy;
			EvObj[i].old_gx -= dx;
			EvObj[i].old_gy -= dy;
		}
	}
}

//------------------------------------------
//	�w����WOBJ����
//	In:		px,py		; ϯ�ߍ��W
//			height		; ����
//	Out:	u8			; ܰ�����
//						; EV_OBJ_MAX = ���݂��Ȃ�
//------------------------------------------
static u8 ev_height_hitchk( EventOBJ *, u8 );

u8 EvObjHitCheck(u16 px, u16 py, u8 height )
{
	u8	i;
	
	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( EvObj[i].sw && EvObj[i].gx == px && EvObj[i].gy == py)
		{
			if( ev_height_hitchk(&EvObj[i],height) )
			{
				return( i );
			}
		}
	}

	return( EV_OBJ_MAX );
}

static u8 ev_height_hitchk( EventOBJ *ev, u8 h )
{
	if( ev->height == MAP_HEIGHT_FREE || h == MAP_HEIGHT_FREE )
	{
		return( 1 );
	}

	if( ev->height == h )
	{
		return( 1 );
	}

	return( 0 );
}

//======================================================================
//		��۰�
//======================================================================
static void ScrollActMove( actWork * );

static void	scr_init( actWork * );
static void	scr_ctrl( actWork * );
static void scr_stop( actWork * );

enum
{
	SCR_INIT = 0,
	SCR_CTRL,
	SCR_STOP,
};

//------------------------------------------
//	����� OBJ Ҳݺ��۰�
//------------------------------------------
void EvObjMainControl( s16 dx, s16 dy )
{
	EvObjMapChgCont();									// �}�b�v�ؑ֎��̍��W�C��
	EvObjAddControl(dx,dy);								// �ړ����̓o�^
	EvObjDelControl();									// �ړ����̍폜
}

//------------------------------------------
//	��۰ٱ���ͯ��
//------------------------------------------
static const actHeader ScrTaskActH =
{
	0,
	ACT_OAMPAL,
	&DummyActOam,
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	ScrollActMove,
};

//------------------------------------------
//	����������۰�������
//	In:	actno	; ���������鱸��ܰ� No
//------------------------------------------
u8 SetScrollTaskAct( u8 actno )
{
	u8	no;

	no = AddActor( &ScrTaskActH, 0, 0, 4 );
	ActWork[no].banish = 1;
	ActWork[no].work[0] = actno;

	return( no );
}

//------------------------------------------
//	�w�豸���Ƃ̓�������
//	work0	= �������
//	work1	= ����
//	work2,3	= x,y
//------------------------------------------
static void ScrollActMove( actWork *act )
{
	void (*tbl[])(actWork*) =
		{ scr_init, scr_ctrl, scr_stop };

	tbl[act->work[1]]( act );
}

static void scr_init( actWork *act )
{
	act->x = ActWork[act->work[0]].x;						// �w�豸�����W�擾
	act->y = ActWork[act->work[0]].y;
	act->banish = 1;
	act->work[1] = SCR_CTRL;

	scr_ctrl( act );
}

static void scr_ctrl( actWork *act )
{
	s16	x,y;

	x = ActWork[act->work[0]].x;
	y = ActWork[act->work[0]].y;

	act->work[2] = x - act->x;								// �ߋ����W�Ƃ̍�
	act->work[3] = y - act->y;

	act->x = x;
	act->y = y;
}

static void scr_stop( actWork *act )
{
	act->x = ActWork[act->work[0]].x;
	act->y = ActWork[act->work[0]].y;
	act->work[2] = 0;
	act->work[3] = 0;
}

//------------------------------------------
//	��۰ٱ������ڽ
//------------------------------------------
static actWork * scroll_act_adrs( void )
{
	u8 i;

	for( i = 0; i < ACT_MAX; i++ )
	{
		if( ActWork[i].act_sw && ActWork[i].move == ScrollActMove )
		{
			return( &ActWork[i] );
		}
	}
	
	return( 0 );
}

//------------------------------------------
//	��۰ٍ��W������
//------------------------------------------
void FldScrollPosInit( void )
{
	actWork *act;
	
	act = scroll_act_adrs();

	if( act == 0 )
	{
		return;
	}

	act->work[1] = SCR_INIT;
	act->move( act );
}

//------------------------------------------
//	��۰ٱ����ύX
//------------------------------------------
void FldScrollActChg( u8 no )
{
	actWork *act;

	act = scroll_act_adrs();
	
	if( act == 0 )
	{
		return;
	}
	
	act->work[0] = no;
	FldScrollPosInit();
}

//------------------------------------------
//	��۰ٱ�������
//------------------------------------------
u8 FldScrollActNo( void )
{
	actWork *act;
	
	act = scroll_act_adrs();
	
	if( act == 0 )
	{
		return( ACT_MAX );
	}
	
	return( act->work[0] );
}

//------------------------------------------
//	��۰ٽį��
//------------------------------------------
void FldScrollActStop( void )
{
	actWork *act;
	
	act = scroll_act_adrs();
	act->work[1] = SCR_STOP;
}
	
//======================================================================
//		����ܰ�
//======================================================================
//------------------------------------------
//	��߰���-�z�u�D��0
//	In:		act			; ��߰������
//			x,y			; ���W
//			pri			; ������ײ��è
//	Out:	u8			; ܰ�����
//						; ACT_MAX = Error
//------------------------------------------
u8 CopyActPri0( actWork *act, s16 x, s16 y, u8 pri )
{
	u8	i;
	actWork *act_p;
	
	for( i = 0; i < ACT_MAX; i++ )
	{
		if( ActWork[i].act_sw == 0 )
		{
			act_p = &ActWork[i];
	
			*act_p = *act;									// �S���
			act_p->x = x;
			act_p->y = y;
			act_p->pri = pri;
			break;
		}
	}

	return( i );
}

//------------------------------------------
//	��߰���-�z�u�D��1
//------------------------------------------
u8 CopyActPri1( actWork *act, s16 x, s16 y, u8 pri )
{
	s16	i;
	actWork *act_p;
	
	for( i = ACT_MAX - 1; i > -1; i-- )
	{
		if( ActWork[i].act_sw == 0 )
		{
			act_p = &ActWork[i];
	
			*act_p = *act;									// �S���
			act_p->x = x;
			act_p->y = y;
			act_p->pri = pri;
			return( i );
		}
	}
	
	return( ACT_MAX );
}

//======================================================================
//		����
//======================================================================
//------------------------------------------
//	�������
//	In:		ev			; ��Ĳ����OBJ
//			site		; ����
//------------------------------------------
void EvObjSiteSet( EventOBJ *ev, u8 site )
{
	ev->old_site = ev->site;

	if( ev->site_pause_sw == 0 )
	{
		ev->site = site;
	}
	
	ev->mv_site = site;
}

//======================================================================
//		SXY OBJ �ް� �擾
//======================================================================
static const evObjData *SxyObjAdrsGet( u8 obj, const evObjData *sxy, u8 Max );

//------------------------------------------
//	��������َ擾
//	In:		id			; OBJ ID
//	Out:	VM_CODE		; ����ĺ��ޱ��ڽ
//------------------------------------------
const VM_CODE * EvObjEvLabelGet( u8 obj, u8 map, u8 div )
{
	evObjData *sxy;
	
	sxy = (evObjData *)SxyObjDataGet( obj, map, div );
	
	return( sxy->ev_label );
}

//------------------------------------------
//	��������َ擾
//	In:		no			; OBJ ܰ�����
//	Out:	VM_CODE		; ����ĺ��ޱ��ڽ
//------------------------------------------
const VM_CODE * EvObjNoEvLabelGet( u8 no )
{
	return( EvObjEvLabelGet(EvObj[no].objID,EvObj[no].mapID,EvObj[no].divID) );
}

//------------------------------------------
//	������׸ގ擾
//	In:		obj			; OBJ ID
//			map			; MAP ID
//			div			; DIV ID
//	Out:	u16			; ������׸�No
//------------------------------------------
u16	EvObjEvFlagGet( u8 obj, u8 map, u8 div )
{
	evObjData *sxy;
	
	sxy = (evObjData *)SxyObjDataGet( obj, map, div );
	
	return( sxy->ev_flag );
}

//------------------------------------------
//	������׸ގ擾
//	In:		no			; OBJ ܰ�����
//	Out:	u16			; ������׸�����
//------------------------------------------
u16	EvObjNoEvFlagGet( u8 no )
{
	return( EvObjEvFlagGet(EvObj[no].objID,EvObj[no].mapID,EvObj[no].divID) );
}

//------------------------------------------
//	��������ߎ擾
//	In:		obj			; OBJ ID
//			map			; MAP ID
//			div			; DIV ID
//	Out:	u8			; ���������
//------------------------------------------
u8 EvObjEvTypeGet( u8 obj, u8 map, u8 div )
{
	u8	no;

	if( EvObjIdPosGet(obj,map,div,&no) )
	{
		return( 0xff );
	}

	return( EvObj[no].ev_type );
}

//------------------------------------------
//	��������ߎ擾
//	In:		no			; OBJ ܰ�����
//	Out:	u8			; ���������
//------------------------------------------
u8 EvObjNoEvTypeGet( u8 no )
{
	return( EvObj[no].ev_type );
}

//------------------------------------------
//	�����ܰ��擾
//	In:		obj			; OBJ ID
//			map			; MAP ID
//			div			; DIV ID
//	Out:	u8			; �����ܰ�
//------------------------------------------
u8 EvObjEvWorkGet( u8 obj, u8 map, u8 div )
{
	u8	no;

	if( EvObjIdPosGet(obj,map,div,&no) )
	{
		return( 0xff );
	}
	
	return( EvObj[no].ev_work );
}

//------------------------------------------
//	�����ܰ��擾
//------------------------------------------
u8 EvObjNoEvWorkGet( u8 no )
{
	return( EvObj[no].ev_work );
}

//------------------------------------------
//	SXY OBJ �ް�
//-----------------------------------------
static const evObjData * SxyObjDataGet( u8 obj_id, u8 map_id, u8 div_id )
{
	const MapTotalData *map_data;
	
	if( Fld.MapID.map_id == map_id && Fld.MapID.div_id == div_id )
	{
		return( SxyObjAdrsGet(obj_id,(const evObjData*)Fld.SvSxyObj,NowMap.Sxy->ObjMax) );
	}

	map_data = GetTotalMapData( div_id, map_id );

	return( SxyObjAdrsGet(obj_id,map_data->Sxy->ObjData,map_data->Sxy->ObjMax) );
}

//------------------------------------------
//	SXY OBJ ���ڽ
//------------------------------------------
static const evObjData * SxyObjAdrsGet( u8 obj_id, const evObjData *sxy, u8 max )
{
	u8 i;

	for( i = 0; i < max; i++ )
	{
		if( sxy[i].id == obj_id )
		{
			return( &(sxy[i]) );
		}
	}

	return( 0 );
}

//======================================================================
//		SXY OBJ �ް� �ύX
//		2001.12.21 by tama
//======================================================================
static evObjData * EvObjSxyDataAdrsGet( EventOBJ * obj )
{
	int i;

	if( obj->mapID != Fld.MapID.map_id
			|| obj->divID != Fld.MapID.div_id ) return NULL;//���̃}�b�v��OBJ�ȊO����

	for( i=0; i< SXY_OBJ_MAX; i++ ){
		if( obj->objID == Fld.SvSxyObj[i].id )return &Fld.SvSxyObj[i];
	}
	return NULL;
}

void EvObjSxyPosChange( EventOBJ * obj )
{
	evObjData * sxy;

	sxy = EvObjSxyDataAdrsGet( obj );
	if( sxy == NULL ) return;

	sxy->pos_x = obj->gx - MAP_LEFT_MARGIN;			// ϯ�ߍ��W�C��
	sxy->pos_y = obj->gy - MAP_UP_MARGIN;
}

void EvObjSxyMvCodeChange( EventOBJ * obj, u8 code )
{
	evObjData * sxy;

	sxy = EvObjSxyDataAdrsGet( obj );
	if( sxy == NULL ) return;

	sxy->site = code;
}

void EvObjSxyIdPosChange( u8 obj_id, u8 map_id, u8 div_id )
{
	u8 id;

	if( EvObjIdPosGet( obj_id, map_id, div_id, &id ) )return;
	EvObjSxyPosChange( &EvObj[id] );
}

//======================================================================
//		��������
//======================================================================
//----
#if 0
//----
// �����޺��۰ٍ\���� agbtypes.h
typedef struct {
    u16 Pixel_1st_Bg0_On:1;								// 1st�߸��
    u16 Pixel_1st_Bg1_On:1;
    u16 Pixel_1st_Bg2_On:1;
    u16 Pixel_1st_Bg3_On:1;
    u16 Pixel_1st_Obj_On:1;
    u16 Pixel_1st_Bd_On:1;
    u16 BlendMode:2;									// ������Ӱ��
    u16 Pixel_2nd_Bg0_On:1;								// 2nd�߸��
    u16 Pixel_2nd_Bg1_On:1;
    u16 Pixel_2nd_Bg2_On:1;
    u16 Pixel_2nd_Bg3_On:1;
    u16 Pixel_2nd_Obj_On:1;
    u16 Pixel_2nd_Bd_On:1;
    u16 Dummy_15_14:2;
    u16 Value_A:5;										// �����A
    u16 Dummy_23_21:3;
    u16 Value_B:5;										// �����B
    u16 Dummy_31_29:3;
    u16 Value_Y:5;										// �����Y
    u16 Dummy_39_37:3;
} BlendCnt;

typedef  vl BlendCnt    vBlendCnt;

#define ST_BLD_NORMAL_MODE	0							// ɰ��
#define ST_BLD_A_BLEND_MODE	1							// ������
#define ST_BLD_UP_MODE		2							// �P�xUp
#define ST_BLD_DOWN_MODE	3							// �P�xDown
//----
#endif
//----

//------------------------------------------
//	������ڼ޽����
//------------------------------------------
#if 0
void EvAlpBlendSet( void )
{
	*(vu16*)REG_BLDCNT =
// BG3 ������OBJ�̂ݔ��f
		BLD_BG3_2ND | BLD_OBJ_2ND | BLD_A_BLEND_MODE;
// BG3 �S OBJ ���f
//		BLD_BG3_2ND | BLD_OBJ_2ND | BLD_OBJ_1ST | BLD_A_BLEND_MODE;
	
	*(vu16*)REG_BLDALPHA = 0x080c;						// ��̂̒l �f�荞�݂ɂ͏d��
}
#endif

//======================================================================
//		̨����OBJ ���̨���֘A
//======================================================================
//------------------------------------------
//	OBJ ���̨���֘A������
//	In:		pal_mode		; ��گ�Ӱ��
//------------------------------------------
void EvObjGraphicInit( u8 pal_mode )
{
	EvObjPalManInit();									// ������
	EvObjSpPalID = EVOBJ_PALEND;						// ������گ�
	EvObjColorMode = pal_mode;							// ���݂���گ�Ӱ��
	EvObjStayPalSets( EvObjStayPalTbl[EvObjColorMode], 0, 10 );
}

//------------------------------------------
//	��گ�No -> ������گ�ID
//------------------------------------------
u16 EvObjPalIdGet( u8 palno )
{
	u8 i;

	if( palno < SP_OBJ_PALNO )							// �풓
	{
		return( EvObjStayPalTbl[EvObjColorMode][palno] );
	}

	for( i = 0; EvObjSpPalStTbl[i].id != EVOBJ_PALEND; i++ )
	{
		if( EvObjSpPalStTbl[i].id == EvObjSpPalID )
		{
			return( EvObjSpPalStTbl[i].com_id[EvObjColorMode] );
		}
	}

	return( EVOBJ_PALEND );
}

