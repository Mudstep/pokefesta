//=========================================================================================
//									�閧��n�E�O�b�Y��߂�
//						base_gds.c �𕪊�	2002/02/07 by nakahiro
//=========================================================================================
#include "common.h"
#include "fieldmap.h"
#include "fld_main.h"
#include "script.h"
#include "task.h"
#include "menu.h"
#include "mapbg.h"
#include "map_attr.h"
#include "palanm.h"
#include "jiki.h"
#include "itemtool.h"
#include "gds_main.h"
#include "gds_put.h"
#include "gds_get.h"
#include "goods.h"
#include "message.h"
#include "weather.h"
#include "fld_task.h"

#include "../data/gds_sym.h"

#include "../scaddata/moyojiki.acl"

//=========================================================================================
//									�V���{����`
//=========================================================================================
#define	GDS_HERO_OBJ_PAL	8		// OBJ�p���b�g�ԍ�

//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
static u8	GoodsSetCheck(u8);				// �O�b�Y���z�u����Ă��邩
static void GoodsGetFadeTask(u8);			// �O�b�Y�����
static void GetGoodsMsgWait(u8);			// ���b�Z�[�W�E�F�C�g��A�O�b�Y���������
static void ReturnGetGoodsMove(u8);			// �O�b�Y����X�N���[���֖߂�
void GoodsGetSet(u8);				// �X�N���[�����̏����@A_BUTTON
void GoodsGetCancel(u8);				// �X�N���[�����̏����@B_BUTTON
static void CheckGetGoods(u8);				// �����ɃO�b�Y���u����Ă��邩
static void GetGoodsPositionCheck(u8);		// �w��ꏊ�̑S�O�b�Y���擾
static void GetGoodsInit(u8);				// �߂���
static void GetGoodsYes(u8);				// YES
static void CancelGetInit(u8);				// �߂��̂���߂�
static void GetCancelYes(u8);				// YES
static void ReturnMenuInit(u8);				// ���j���[�I���֖߂�
static void ReturnMenuFade(u8);
static void ReturnMenuSelect(void);

static void GetCursorActMove( actWork * );	// �J�[�\������

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
extern const VM_CODE obj_goods_get_init[];

static GetGoodsData	get_goods[16] = {};	// �������O�b�Y���L��
static u8	get_max = 0;				// ��������

// �͂��E����������
static const SELECT_ITEM	YesNoSelectFunc[] =
{
	{ GetGoodsYes,  ReturnGetGoodsMove },	// 00:�߂�
	{ GetCancelYes, ReturnGetGoodsMove },	// 01:�߂��̂���߂�
};


static const u8 moyoh_Character[]={
                                                        /*  0  =  0(0x0)  */
	0xe0,0xee,0xee,0xee,0xee,0x00,0x00,0x00,
	0x0e,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
	0x0e,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
	0x0e,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
                                                        /*  1  =  32(0x20)  */
	0xee,0xee,0xee,0x0e,0x00,0x00,0x00,0xee,
	0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0xe0,
	0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0xe0,
	0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0xe0,
                                                        /*  2  =  64(0x40)  */
	0x0e,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
	0x0e,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
	0x0e,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,
	0xee,0x00,0x00,0x00,0xe0,0xee,0xee,0xee,
                                                        /*  3  =  96(0x60)  */
	0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0xe0,
	0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0xe0,
	0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0xe0,
	0x00,0x00,0x00,0xee,0xee,0xee,0xee,0x0e,
};

static const PalData GDS_HeroPal   = { &moyohjiki_Palette[0],  GDS_HERO_OBJ_PAL };
static const PalData GDS_HeroinPal = { &moyohjiki_Palette[16], GDS_HERO_OBJ_PAL };

static const ActOamData oam_16x16 =
{
	0,		//V�߼޼��
	0,		//�g��k��Ӱ�ސݒ�
	0,		//OBJӰ�ސݒ�(NORMAL)
	0,		//ӻ޲��ݒ�(OFF)
	0,		//�װӰ�ސݒ�(16�F)
	0,		//OBJ�`��ݒ�(�����`)
	0,		//H�߼޼��
	0,		//�g��k�����Ұ����ް�ݒ�
	1,		//OBJ�T�C�Y�ݒ�	16x16
	0,		//��׸����
	1,		//�\����ײ��è��ݒ�(��2�D��)
	0,		//��گ����ް
	0,		//AffineParam
};
static const actAnm anm00[] =
{
	{ 0, 0, 0, 0 },
	{ ACT_ANMEND, 0, 0, 0 },
};
static const actAnm * const AnmTbl[] = 
{
	anm00,
};

static const actAnmData celltrans[] =
{
	{ (const u8 *)moyoh_Character, 0x20 * 4 },	// ��
};

// ��
static const actHeader ActGetCursorHeader =
{
	ACT_TMODE,
	GDS_HERO_OBJ_PAL,
	&oam_16x16,
	AnmTbl,
	celltrans,
	DummyActAffTbl,
	GetCursorActMove,
};



//=========================================================================================
//										�������
//=========================================================================================
//---------------------
//		�����ݒ�
//---------------------
void GoodsGetInit( u8 id )
{
	if( GoodsSetCheck(id) == TRUE ){
//		PaletteFadeReq(0xffffffff,0,0,16,0);
		FieldFadeControl( FIELD_BLACK_OUT, 0 );
		TaskTable[id].work[ FADE_WORK ] = 0;
		TaskTable[id].TaskAdrs = GoodsGetFadeTask;
	}else{
		SetTalkMsg( id, msg_goods_no_put, NoGoods2, 0 );
	}
}


//----------------------------
//		�O�b�Y�𔠂ɖ߂�
//----------------------------
void DelGoods( u8 no )
{
	FURNITURE->id[ no ] = 0;
	FURNITURE->pos[ no ] = 0;
}

void ObjGoodsReturn2(void)
{
	u8	type;
	u16	i;

	ScriptParameter1 = 0;
	AnswerWork = 0;
	
	if( ScriptParameter0 == get_max ){
		AnswerWork = 1;
		return;
	}

	type = base_goods[ FURNITURE->id[get_goods[ScriptParameter0].num] ].type;

	if( type == DESK_TOP ){

		ScriptParameter1 = get_goods[ScriptParameter0].flg;

		DelGoods( get_goods[ScriptParameter0].num );

		for( i=0; i<(NowMap.Sxy->ObjMax); i++){
			if( NowMap.Sxy->ObjData[i].ev_flag == ScriptParameter1 ){
				ScriptParameter2 = NowMap.Sxy->ObjData[i].id;
				return;
			}
		}
	}
}

void ObjGoodsReturn(void)
{
	u8	i;

	for( i=0; i<(NowMap.Sxy->ObjMax); i++){
		if( NowMap.Sxy->ObjData[i].ev_flag == ScriptParameter0 ){
			ScriptParameter1 = NowMap.Sxy->ObjData[i].id;
			return;
		}
	}
}

void GoodsBoxReturn(void)
{
	u8	h,i,j;
	u8	type,x,y;

	for( h=0; h<get_max; h++ ){

		type = base_goods[ FURNITURE->id[get_goods[h].num] ].type;
		x = FURNITURE->pos[ get_goods[h].num ] >> 4;
		y = FURNITURE->pos[ get_goods[h].num ] & 0xf;

		if( type != DESK_TOP ){
			for( i=0; i<get_goods[h].Ysize; i++ ){
				for( j=0; j<get_goods[h].Xsize; j++ ){
					SetCellDirect(
						x+MAP_LEFT_MARGIN+j,
						y+MAP_UP_MARGIN-i,
						NowMap.Screen->ScreenData[x+j+(y-i)*NowMap.Screen->Xsize] | ( 3 << CELL_HEIGHT_SHIFT ) );
				}
			}
			DelGoods( get_goods[h].num );
		}
	}
}

// �t�F�[�h����
static void GoodsReturnFade( u8 id )
{
	switch( TaskTable[id].work[ FADE_WORK ] ){
	case 0:
		GoodsBoxReturn();
		TaskTable[id].work[ FADE_WORK ] = 1;
		break;
	case 1:
		if( !( FadeData.fade_sw ) ){
			MakeMapBG();
			StartFieldScript( obj_goods_get_init );
			NMenuBoxClear( 0, 0, 29, 19 );
			TaskTable[id].work[ FADE_WORK ] = 2;
		}
		break;
	case 2:
		SetForceEvent();
		SetPutGoodsNo(id);
//		PaletteFadeReq(0xffffffff,0,16,0,0);
		FieldFadeBlackInSet();
		TaskTable[id].work[ FADE_WORK ] = 3;
		break;
	case 3:
//		if( !( FadeData.fade_sw ) ){
		if( FieldFadeinCheck() == 1 ){
			TaskTable[id].work[ NEXT_GG ] = -1;
			SetTalkMsg( id, msg_goods_get03, GetGoodsMsgWait, 0 );
		}
	}
}

//------------------------------------
//		�O�b�Y���z�u����Ă��邩
//------------------------------------
static u8	GoodsSetCheck( u8 id )
{
	u16	i;

	for( i=0; i<FURNITURE->max; i++ ){
		if( FURNITURE->id[i] != 0 ){
			TaskTable[id].work[ NEXT_GG ] = i;
			return	TRUE;
		}
	}
	return	FALSE;
}

//----------------------------------
//		�J�[�\���ʒu�����ݒ�
//----------------------------------
static void CursorOBJInit(void)
{
	u8	site;

	site = GetHeroSite();		// ����

	NMenuBoxClear( 0, 0, 29, 19 );
	scr_act = ActWork[ scroll.work ].work[0];

	Set_GDS_CursorPal();

	scroll.work = AddActor( &ActGetCursorHeader, 120, 80, 0 );

	if( MyData.my_sex == 0 ){
		hero_act = EvObjMakeAddAct( HEROREMODEL, DummyActMove, 136, 72, 0 );
	}else{
		hero_act = EvObjMakeAddAct( HINEREMODEL, DummyActMove, 136, 72, 0 );
	}
	ActWork[ hero_act ].oamData.Priority = 1;

	DelActor( &ActWork[ scr_act ] );	// ��l��OBJ����
	scr_act = scroll.work;

	ActWork[scr_act].oamData.Priority = 1;
}

//------------------------
//		�O�b�Y�����
//------------------------
static void GoodsGetFadeTask( u8 id )
{
	s16 * wk = TaskTable[id].work;

	switch( wk[ FADE_WORK ] ){
	case 0:
		if( !( FadeData.fade_sw ) ){
			GoodsFuncGlobalPosition(id);
			wk[ FADE_WORK ] = 1;
			wk[ GOODS_V ]   = 1;
			wk[ GOODS_H ]   = 1;
		}
		break;
	case 1:
		CursorOBJInit();
//		PaletteFadeReq(0xffffffff,0,16,0,0);
		FieldFadeBlackInSet();
		wk[ FADE_WORK ] = 2;
		break;
	case 2:
//		if( !( FadeData.fade_sw ) ){
		if( FieldFadeinCheck() == 1 ){
			wk[ MENU_NO ] = 1;
			ReturnGetGoodsMove(id);
		}
	}
}

//---------------------------------------
//		�O�b�Y����X�N���[���֖߂�
//---------------------------------------
extern void GoodsMoveTask(u8);
extern void ClearMoveParam(void);		// �p�����[�^������
static void HeroMoveGoodsPos(u8,u8);

static void ReturnGetGoodsMove( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	ActWork[ scr_act ].work[7] = 0;
	ActWork[scr_act].banish = 0;
	ActWork[scr_act].move = GetCursorActMove;
	ActWork[hero_act].x = 136;
	ActWork[hero_act].y = 72;
	TaskTable[id].work[ BUTTON_FLG ] = 0;
	TaskTable[id].TaskAdrs = GoodsMoveTask;
}

//------------------------------
//		�X�N���[�����̏���
//------------------------------
// A_BUTTON
void GoodsGetSet( u8 id )
{
	TaskTable[id].work[ BUTTON_FLG ] = 0;
	ClearMoveParam();
	CheckGetGoods(id);
}
// B_BUTTON
void GoodsGetCancel( u8 id )
{
	TaskTable[id].work[ BUTTON_FLG ] = 0;
	ClearMoveParam();
	ActWork[scr_act].banish = 0;
	ActWork[scr_act].move = DummyActMove;
	SetTalkMsg( id, msg_goods_get00, CancelGetInit, 0 );	// ���胁�b�Z�[�W
}

//----------------------------------------
//		�����ɃO�b�Y���u����Ă��邩
//----------------------------------------
static void CheckGetGoods( u8 id )
{
	s16 * wk;
	u8    attr=0;

	GetGoodsPositionCheck(id);

	if( get_max != 0 ){
		SetTalkMsg( id, msg_goods_get02, GetGoodsInit, 0 );		// �u����Ă���
	}else{
		wk = TaskTable[id].work;
		attr = GetCellAttribute( wk[CELL_POSX], wk[CELL_POSY] );
		if( ATR_IsBaseSecretBoxCheck( attr ) == TRUE ||
			ATR_IsMyRoomPCCheck( attr ) == TRUE )
		{
			ActWork[scr_act].banish = 0;
			ActWork[scr_act].move = DummyActMove;
			SetTalkMsg( id, msg_goods_get00, CancelGetInit, 0 );	// �߂��̂���߂邩
		}else
			SetTalkMsg( id, msg_goods_get01, GetGoodsMsgWait, 0 );	// �u����Ă��Ȃ�
	}
}

//----------------------------------------------------
//		���b�Z�[�W�E�F�C�g��A�O�b�Y���������
//----------------------------------------------------
static void GetGoodsMsgWait( u8 id )
{
	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
		ReturnGetGoodsMove(id);
	}
}

//------------------------------------
//		�߂��O�b�Y�̃T�C�Y���擾
//------------------------------------
static void GetGoodsSizeSet( u8 num, GetGoodsData * data )
{
	if( base_goods[ num ].size == size_16x16 ){
		data->Xsize = 1;
		data->Ysize = 1;
		return;
	}
	if( base_goods[ num ].size == size_32x16 ){
		data->Xsize = 2;
		data->Ysize = 1;
		return;
	}
	if( base_goods[ num ].size == size_48x16 ){
		data->Xsize = 3;
		data->Ysize = 1;
		return;
	}
	if( base_goods[ num ].size == size_64x32 ){
		data->Xsize = 4;
		data->Ysize = 2;
		return;
	}
	if( base_goods[ num ].size == size_32x32 ){
		data->Xsize = 2;
		data->Ysize = 2;
		return;
	}
	if( base_goods[ num ].size == size_16x32 ){
		data->Xsize = 1;
		data->Ysize = 2;
		return;
	}
	if( base_goods[ num ].size == size_16x48 ){
		data->Xsize = 1;
		data->Ysize = 3;
		return;
	}
	if( base_goods[ num ].size == size_32x64 ){
		data->Xsize = 2;
		data->Ysize = 4;
		return;
	}
	if( base_goods[ num ].size == size_48x48 ){
		data->Xsize = 3;
		data->Ysize = 3;
		return;
	}
	if( base_goods[ num ].size == size_48x32 ){
		data->Xsize = 3;
		data->Ysize = 2;
		return;
	}
}

//------------------------------------
//		�w��ꏊ�ɃO�b�Y�����邩
//------------------------------------
static void HeroMoveGoodsPos( u8 px, u8 py )
{
	ActWork[scr_act].banish = 1;
	ActWork[scr_act].move = DummyActMove;
	ActWork[hero_act].x = 136 + ( px << 4 );
	ActWork[hero_act].y = 72 + ( py << 4 );
}

static u8	PositionCheckSize( u8 id, u8 no, GetGoodsData * data )
{
	u8 gx,gy,px,py;

	px = TaskTable[id].work[CELL_POSX]-MAP_LEFT_MARGIN;
	py = TaskTable[id].work[CELL_POSY]-MAP_UP_MARGIN;
	gx = FURNITURE->pos[no] >> 4;
	gy = FURNITURE->pos[no] & 0xf;

	if( FURNITURE->id[no] == GOODS_SUNANOOKIMONO &&
		GetCellCode( MAP_LEFT_MARGIN+gx, MAP_UP_MARGIN+gy ) == 652 )
	{
		data->Ysize -=1;
	}

	if( px >= gx && px < gx+data->Xsize && py > gy-data->Ysize && py <= gy ){
		HeroMoveGoodsPos( data->Xsize-1-(px-gx), gy-py );
		return	TRUE;
	}

	return	FALSE;
}

//-------------------------------------------
//		�������OBJ�O�b�Y�̃t���O���擾
//-------------------------------------------
static void GetOBJGoodsFlg(void)
{
	u16	i;
	u8	x,y;

	x = FURNITURE->pos[ get_goods[get_max].num ] >> 4;
	y = FURNITURE->pos[ get_goods[get_max].num ] & 0xf;

	for( i=0; i<SXY_OBJ_MAX; i++ ){
		if( Fld.SvSxyObj[i].pos_x == x && Fld.SvSxyObj[i].pos_y == y ){
			if( EventFlagCheck( Fld.SvSxyObj[i].ev_flag ) == 0 ){
				get_goods[get_max].flg = Fld.SvSxyObj[i].ev_flag;
				break;
			}
		}
	}
}

//-------------------------------------------
//		�w��ꏊ��OBJ�̃O�b�Y���𒲂ׂ�
//-------------------------------------------
static u8 GetOBJGoodsPositionCheck( u8 id )
{
	u16	i;

	for( i=0; i<FURNITURE->max; i++ ){
		if( FURNITURE->id[i] != 0 ){
			if( base_goods[ FURNITURE->id[i] ].type == DESK_TOP ){
				GetGoodsSizeSet( FURNITURE->id[i], &get_goods[0] );
				if( PositionCheckSize( id, i, &get_goods[0] ) == TRUE ){
					get_goods[0].num = i;
					GetOBJGoodsFlg();
					get_max = 1;
					return	TRUE;
				}
			}
		}
	}

	return	FALSE;
}

//--------------------------------
//		�͈͓��̃O�b�Y���擾
//--------------------------------
static void SizePosGoodsCheck( u8 px, u8 py, u8 mx, u8 my )
{
	u8	i;
	u8	num,x,y;

	for( i=0; i<FURNITURE->max; i++ ){
		num = FURNITURE->id[i];
		x   = FURNITURE->pos[i] >> 4;
		y   = FURNITURE->pos[i] & 0xf;

		if( num != 0 && base_goods[num].type == DESK_TOP ){
			if( px <= x && py <= y && mx >= x && my >= y ){
				get_goods[get_max].num = i;
				GetOBJGoodsFlg();
				get_max++;
			}
		}
	}
}

//------------------------------------
//		�w��ꏊ�̑S�O�b�Y���擾
//------------------------------------
static void GetGoodsPositionCheck( u8 id )
{
	u8	i;
	u8	num;

	get_max = 0;

	if( GetOBJGoodsPositionCheck(id) == TRUE )	return;

	for( i=0; i<FURNITURE->max; i++ ){
		num = FURNITURE->id[i];
		if( num != 0 ){
			GetGoodsSizeSet( num, &get_goods[0] );
			if( PositionCheckSize( id, i, &get_goods[0] ) == TRUE ){
				get_goods[0].num = i;
				get_max++;
				break;
			}
		}
	}

	if( get_max != 0 ){
		num = FURNITURE->pos[get_goods[0].num];
		SizePosGoodsCheck(
			num >> 4, ( num & 0xf )-get_goods[0].Ysize+1,
			( num >> 4 )+get_goods[0].Xsize-1, num & 0xf	);
	}
}

//------------------
//		�߂���
//------------------
static void GetGoodsInit( u8 id )
{
	YesNoSelectInit( YESNO_WINX, YESNO_WINY );
	SelectToolInit( id, &YesNoSelectFunc[0] );
}

// YES
static void GetGoodsYes( u8 id )
{
//	PaletteFadeReq(0xffffffff,0,0,16,0);
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
	TaskTable[id].work[ FADE_WORK ] = 0;
	TaskTable[id].TaskAdrs = GoodsReturnFade;
}


//-------------------------
//		�߂��̂���߂�
//-------------------------
static void CancelGetInit( u8 id )
{
	YesNoSelectInit( YESNO_WINX, YESNO_WINY );
	SelectToolInit( id, &YesNoSelectFunc[1] );
}

// YES
static void GetCancelYes( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	ReturnMenuInit(id);
}

//-----------------------------
//		���j���[�I���֖߂�
//-----------------------------
static void ReturnMenuInit( u8 id )
{
//	PaletteFadeReq(0xffffffff,0,0,16,0);
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
	TaskTable[id].work[ FADE_WORK ] = 0;
	TaskTable[id].TaskAdrs = ReturnMenuFade;
}

// �t�F�[�h�A�E�g����
static void ReturnMenuFade( u8 id )
{
	switch( TaskTable[id].work[ FADE_WORK ] ){
	case 0:
		if( !( FadeData.fade_sw ) ){
			SetHeroGlobalPosition(id);
			TaskTable[id].work[ FADE_WORK ] = 1;
		}
		break;
	case 1:
		Del_GDS_CursorPal();
		pFieldRecoverFunc = ReturnMenuSelect;
		MainProcChange( FieldMainRecover );
		DelTask(id);
	}
}

static void ReturnMenuSelect2( u8 id )
{
	if( FieldFadeinCheck() == 1 )
		TaskTable[id].TaskAdrs = RearrangeTaskMain;
}

static void ReturnMenuSelect(void)
{
//	PaletteFadeReq(0xffffffff,0,16,0,0);
	FieldFadeBlackInSet();
	NMenuTalkWinWrite();
	RaarrangeParamInt();
	AddTask( ReturnMenuSelect2, 8 );
}


//--------------------------------------
//	�J�[�\������
//--------------------------------------
static void GetCursorActMove( actWork * act )
{
	act->work[0] = ( act->work[0] + 1 ) & 0x1f;

	if( act->work[0] >= 16 )	act->banish = 1;
	else						act->banish = 0;
}


//--------------------------------------
//	�J�[�\���p���b�g
//--------------------------------------
void Set_GDS_CursorPal(void)
{
	if( MyData.my_sex == 0 )	ObjPalSet( (PalData *)&GDS_HeroPal );
	else						ObjPalSet( (PalData *)&GDS_HeroinPal );
}
void Del_GDS_CursorPal(void)
{
	ObjPalRegNumDel( GDS_HERO_OBJ_PAL );
}
