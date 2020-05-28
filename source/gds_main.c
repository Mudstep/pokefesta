//=========================================================================================
//									�閧��n�O�b�Y����
//						base_gds.c �𕪊�	2002/02/07 by nakahiro
//=========================================================================================
#include "common.h"
#include "fld_tool.h"
#include "script.h"
#include "task.h"
#include "menu.h"
#include "mapbg.h"
#include "palanm.h"
#include "jiki.h"
#include "sxytype.h"
#include "itemtool.h"
#include "fd_shop.h"
#include "gds_main.h"
#include "gds_put.h"
#include "gds_get.h"
#include "gds_del.h"
#include "goods.h"
#include "my_pc.h"
#include "mus_tool.h"
#include "oyaji03.h"
#include "message.h"

//=========================================================================================
//									�V���{����`
//=========================================================================================
#define	MENU_WINX		0
#define	MENU_WINY		0
#define	MENU_WINH		10
#define	MENU_WINV		9

#define	GOODS_WINX		0
#define	GOODS_WINY		0
#define	GOODS_WINH		14
#define	GOODS_WINV		19

#define	GOODS_MARK_PUT_X		( GOODS_WINX + 13 )

#define	GDS_TYPE_WINX		0
#define	GDS_TYPE_WINY		0
#define	GDS_TYPE_WINEX		( GDS_TYPE_WINX + 14 )
#define	GDS_TYPE_WINEY		( GDS_TYPE_WINY + 19 )

#define	GDS_TYPE_WINX2		17
#define	GDS_TYPE_WINY2		0
#define	GDS_TYPE_WINEX2		( GDS_TYPE_WINX2 + 12 )
#define	GDS_TYPE_WINEY2		( GDS_TYPE_WINY2 + 3 )

#define	GOODS_MINE_NUM_POS		6
#define	GOODS_MAX_MINE_SRA_POS	8
#define	GOODS_MAX_NUM_POS		9

#define	GDS_INFO_WINX		16
#define	GDS_INFO_WINY		12
#define	GDS_INFO_WINEX		( GDS_INFO_WINX + 13 )
#define	GDS_INFO_WINEY		( GDS_INFO_WINY + 7 )

#define	NOT_PUT_PAL			13

//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
static void GDS_PaletteMake(void);		// �p���b�g�쐬

static void DrawMenuMessage(void);		// ���j���[�����\��
static void ReturnScript(u8);			// ���j���["���ǂ�"

static void GoodsTypeSelectMain(u8);	// ��ޑI������
static void GoodsTypeSelectCancel(u8);	// ���ڃL�����Z��
static void ReturnRearrangeMenu(u8);	// ��ޑI�����I�����Ė͗l�ւ����j���[�֖߂�

static void DrawGoodsWin(u8);			// �O�b�Y�E�B���h�E�\��
static void DrawGoodsName( u8, u8 );	// �O�b�Y���\��
static void DrawGoodsTypeSWin(u8);		// ��ޖ��E�B���h�E�\��
static void GoodsInfPut(u8);			// �O�b�Y�����\��
static void GoodsSelectCancel(u8);		// �O�b�Y�I���L�����Z������

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
#include "../data/gds_name.h"
#include "../data/gds_sym.h"
#include "../data/gds_cell.h"
#include "../data/gds_msg.h"
#include "../data/gds_data.h"

extern const VM_CODE return_mine_base_board[];

static const u8 * const str_boods_type[] = {
	str_goods_type_tukue,		// 00:����
	str_goods_type_isu,			// 01:����
	str_goods_type_hatiue,		// 02:���A��
	str_goods_type_okimono,		// 03:���炭��
	str_goods_type_matto,		// 04:�}�b�g
	str_goods_type_posuta,		// 05:�|�X�^�[
	str_goods_type_nuigurumi,	// 06:�ʂ������
	str_goods_type_kussyon,		// 07:�N�b�V����
};

u8 * GoodsType = NULL;	// �I������O�b�Y�̃|�C���^

static const MENU_ITEM	menu[] =
{
	{ str_goods_menu_oku,     GoodsPutInit },	// 00:�O�b�Y������
	{ str_goods_menu_modosu,  GoodsGetInit },	// 01:�O�b�Y�����ǂ�
	{ str_goods_menu_recycle, RecycleInit  },	// 02:���T�C�N��
	{ str_modoru,             ReturnScript },	// 03:���ǂ�
};

static u8	menu_pos = 0;	// ���j���[�ԍ�

static const u8 * const str_menu_msg[] = {
	msg_goods_menu00,
	msg_goods_menu01,
	msg_goods_menu02,
	msg_menu_return,
};

// �V�[�����Ƃ̏���
static const SELECT_ITEM	GoodsSelectFunc[] =
{
	{ GoodsSelectPut,       GoodsSelectCancel },	// 00:�O�b�Y��u��
	{ GoodsSelectRecycle,   GoodsSelectCancel },	// 01:���T�C�N���i��n���j
	{ GoodsSelectRecycle,   GoodsSelectCancel },	// 02:���T�C�N���i�V���b�v�j
	{ TradeOyajiMyGoodsSet, GoodsSelectCancel },	// 03:�������₶
};

u8	goods_max = 0;			// �����O�b�Y��
u8	set_goods[16] = {};		// �u����Ă�O�b�Y���L���i��n�j
u8	set_room[12] = {};		// �u����Ă�O�b�Y���L���i�����̕����j

static u8	pos = 0;		// �J�[�\���ʒu
static u8	pos_max = 0;	// �J�[�\���I��
static u8	scr = 0;		// �O�b�Y�E�B���h�E�X�N���[���J�E���^�[
u8	select_goods_pos = 0;	// pos + scr

u8	type_pos = 0;			// ��ނ̃J�[�\���ʒu

u8 mark_act[8] = {};		// �z�u�ς݃A�N�^�[�ԍ�


//=========================================================================================
//									���j���[�I������
//=========================================================================================
//----------------------
//		�����ݒ�
//----------------------
static void DrawMenuWindow(void)
{
	GDS_PaletteMake();

	NMenuWinBoxWrite( MENU_WINX, MENU_WINY, MENU_WINX+MENU_WINH, MENU_WINY+MENU_WINV );
	NMenuFixListWrite( MENU_WINX+2, MENU_WINY+1, 4, menu );
	NMenuVCursorInit( MENU_WINX+1, MENU_WINY+1, 4, menu_pos );	//�J�[�\�������ݒ�
}

void RaarrangeParamInt(void)
{
	menu_pos = 0;
	SetForceEvent();
	DrawMenuWindow();
	DrawMenuMessage();
}

void RearrangeInit2( u8 id )
{
	RaarrangeParamInt();
	TaskTable[id].TaskAdrs = RearrangeTaskMain;
}

// �閧��n
void RearrangeTaskInit( u8 id )
{
	RearrangeInit2(id);
	FURNITURE->id   = Fld.SecretBase[0].furniture_id;
	FURNITURE->pos  = Fld.SecretBase[0].furniture_pos;
	FURNITURE->max  = 16;
	FURNITURE->type = BASE_GOODS_TYPE;
}

// ����
void MyRoomRearrangeInit( u8 id )
{
	RearrangeInit2(id);
	FURNITURE->id   = Fld.MyRoomFurnitureID;
	FURNITURE->pos  = Fld.MyRoomFurniturePos;
	FURNITURE->max  = 12;
	FURNITURE->type = MY_ROOM_GOODS_TYPE;
}

//-----------------------------------------
//	�p���b�g�쐬
//-----------------------------------------
static void GDS_PaletteMake(void)
{
	u16	pal[] = { 0x6318, 0x739c, 0x7fff };

	PaletteWorkSet( &pal[2], NOT_PUT_PAL * 16 + COL_WHITE, 2 );	// BG
	PaletteWorkSet( &pal[1], NOT_PUT_PAL * 16 + COL_BLACK, 2 );	// ����
	PaletteWorkSet( &pal[0], NOT_PUT_PAL * 16 + COL_DGRAY, 2 );	// �e
}

//-------------------------
//		���j���[�I��
//-------------------------
void RearrangeTaskMain( u8 id )
{
	MENUFUNC	func;

	if( FadeData.fade_sw != 0 )	return;

	if( sys.Trg & U_KEY ){
		SePlay(SE_SELECT);
		menu_pos = NMenuVCursorSet(-1);
		DrawMenuMessage();
	}
	if( sys.Trg & D_KEY ){
		SePlay(SE_SELECT);
		menu_pos = NMenuVCursorSet(1);
		DrawMenuMessage();
	}

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		func = menu[ menu_pos ].ptr;
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay(SE_SELECT);
		ReturnScript(id);
	}
}

//------------------------------
//		���j���[�����\��
//------------------------------
static void DrawMenuMessage(void)
{
	NMenuBoxWrite( 4, 15, 25, 18 );
	NMenuMsgWrite( str_menu_msg[ menu_pos ], 4, 15 );
}

//----------------------------
//		���j���["���ǂ�"
//----------------------------
static void ReturnScript( u8 id )
{
	NMenuBoxClear( MENU_WINX, MENU_WINY, MENU_WINX+MENU_WINH, MENU_WINY+MENU_WINV );
	NMenuBoxWrite( 4, 15, 25, 18 );
	ObjPalRegNumDel( MARK_OBJ_PAL );

	if( FURNITURE->type == BASE_GOODS_TYPE ){
		StartFieldScript( return_mine_base_board ); // �X�N���v�g�N��(�^�X�N�Ő���)
		DelTask(id);
	}else{
		MyPC_ReturnMenu(id);
	}
}

//--------------------------------------
//		�O�b�Y���������Ă��Ȃ��Ƃ�
//--------------------------------------
void NoGoods( u8 id )
{
	GoodsTypeSelectInit(id);
}
// ���ǂ�
void NoGoods2( u8 id )
{
	DrawMenuMessage();
	TaskTable[id].TaskAdrs = RearrangeTaskMain;
}


//=========================================================================================
//									�O�b�Y�̎�ޑI��
//=========================================================================================
//--------------------
//		��ޕ\��
//--------------------
void GoodsTypeStrPut( u8 no, u8 x, u8 y, u8 pal )
{
	PalChangeMojiPut( str_boods_type[no], pal, x, y );
	PalChangeNumPut( GoodsMaxCheck2(no), pal, 2, NUM_MODE_SPACE, x+GOODS_MINE_NUM_POS, y );
	PalChangeMojiPut( str_surassyu, pal, x+GOODS_MAX_MINE_SRA_POS, y );
	PalChangeNumPut( goods_box[no].max, pal, 2, NUM_MODE_SPACE, x+GOODS_MAX_NUM_POS, y );
}

void DrawGoodsType( u8 id )
{
	u8	i;

	NMenuWinBoxWrite( GDS_TYPE_WINX, GDS_TYPE_WINY, GDS_TYPE_WINEX, GDS_TYPE_WINEY );

	for( i=0; i<8; i++ ){
		if( FURNITURE->type == MY_ROOM_GOODS_TYPE &&
			TaskTable[id].work[SCENE_NO] == 0 &&
			i != GOODS_DOLL_FOLDER && i != GOODS_CUSHION_FOLDER )
		{
			GoodsTypeStrPut( i, GDS_TYPE_WINX+2, GDS_TYPE_WINY+1+(i<<1), NOT_PUT_PAL );
		}else{
			GoodsTypeStrPut( i, GDS_TYPE_WINX+2, GDS_TYPE_WINY+1+(i<<1), 0xff );
		}
	}

	NMenuMsgWrite( str_modoru, GDS_TYPE_WINX+2, GDS_TYPE_WINY+17 );	// ���ǂ�
}

//----------------------------
//		��ޑI�������ݒ�
//----------------------------
void GoodsTypeSelectInit( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	DrawGoodsType(id);
	NMenuVCursorInit( GDS_TYPE_WINX+1, GDS_TYPE_WINY+1, 9, type_pos );
	TaskTable[id].TaskAdrs = GoodsTypeSelectMain;
}

//---------------------
//		�I������
//---------------------
static void GoodsTypeSelectMain( u8 id )
{
	if( FadeData.fade_sw )	return;

	if( sys.Repeat & U_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet( -1 );
		return;
	}

	if( sys.Repeat & D_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet( 1 );
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		type_pos = NMenuVCursorPosGet();
		if( type_pos != 8 ){
			goods_max = GoodsMaxCheck2( type_pos );
			if( goods_max != 0 ){
				GoodsSort2( type_pos );
				GoodsType = goods_box[ type_pos ].box;
				SelectGoodsParamInit(id);
				ScrActInit();
				MarkActInit( mark_act, 8 );
				AddMarkPal();
				TaskTable[id].TaskAdrs = GoodsSelectInit;
			}else{
				NMenuBoxClear( GDS_TYPE_WINX, GDS_TYPE_WINY, GDS_TYPE_WINEX, GDS_TYPE_WINEY );
				SetTalkMsg( id, msg_no_goods00, NoGoods, 0 );
			}
		}else{
			GoodsTypeSelectCancel(id);
		}
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay(SE_SELECT);
		GoodsTypeSelectCancel(id);
	}
}

// ���ڃL�����Z��
static void GoodsTypeSelectCancel( u8 id )
{
	if( TaskTable[id].work[SCENE_NO] != 3 )
		ReturnRearrangeMenu(id);
	else
		TradeOyajiMyGoodsCancel(id);
}

//---------------------------------------------
//		��ޑI�����I�����đO�̏����֖߂�
//---------------------------------------------
static void ReturnRearrangeMenu( u8 id )
{
	NMenuBoxClear( GDS_TYPE_WINX, GDS_TYPE_WINY, GDS_TYPE_WINEX, GDS_TYPE_WINEY );

	if( TaskTable[id].work[ SCENE_NO ] != 2 ){
		DrawMenuWindow();
		NMenuTalkWinWrite();
		DrawMenuMessage();
		TaskTable[id].TaskAdrs = RearrangeTaskMain;
	}else	ShopRecycleEnd(id);
}

//---------------------------------------------
//	�������₶�E���ڑI����
//---------------------------------------------
void TradeOyajiMyGoodsSelectInit( u8 id )
{
	TaskTable[id].work[ SCENE_NO ] = 3;
	type_pos = 0;
	GoodsTypeSelectInit(id);

}

//---------------------------------------------
//	�������₶�E���ڕ�����擾
//---------------------------------------------
void GetGoodsTypeStr( u8 * str, u8 no )
{
	PM_strcpy( str, str_boods_type[no] );
}


//=========================================================================================
//										�O�b�Y�I������
//=========================================================================================
//-------------------------------
//		�O�b�Y�I�������ݒ�
//-------------------------------
void GoodsSelectParamInit( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	DrawGoodsWin(id);
	DrawGoodsTypeSWin( type_pos );
	GoodsInfPut( pos+scr );
	NMenuVCursorInit( GOODS_WINX+1, GOODS_WINY+2, pos_max+1, pos );	//�J�[�\�������ݒ�
}
void GoodsSelectInit( u8 id )
{
	GoodsSelectParamInit(id);
	TaskTable[id].TaskAdrs = GoodsSelectTask;
}

//------------------------
//		�O�b�Y�I��
//------------------------
static void GoodsSelectKey( u8 id, s8 cur_flg, s8 scr_flg )
{
	SePlay(SE_SELECT);

	if( cur_flg != 0 ){
		pos = NMenuVCursorSet( cur_flg );
	}
	if( scr_flg != 0 ){
		scr += scr_flg;
		DrawGoodsName( id, 1 );
	}

	if( pos+scr != goods_max )	GoodsInfPut( pos+scr );
	else
		NMenuBoxClear( GDS_INFO_WINX, GDS_INFO_WINY, GDS_INFO_WINEX, GDS_INFO_WINEY );
}

void GoodsSelectTask( u8 id )
{
	if( FadeData.fade_sw != 0 )	return;

	if( ( sys.Repeat & PLUS_KEY_MASK ) == U_KEY ){
		if( pos != 0 )	GoodsSelectKey( id, -1, 0 );
		else{
			if( scr != 0 )	GoodsSelectKey( id, 0, -1 );
		}
	}

	if( ( sys.Repeat & PLUS_KEY_MASK ) == D_KEY ){
		if( pos != pos_max )	GoodsSelectKey( id, 1, 0 );
		else{
			if( scr + pos != goods_max )	GoodsSelectKey( id, 0, 1 );
		}
	}

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		select_goods_pos = pos + scr;
		if( select_goods_pos == goods_max ){
			GoodsSelectFunc[ TaskTable[id].work[SCENE_NO] ].cancel_func(id);
		}else{
			GoodsSelectFunc[ TaskTable[id].work[SCENE_NO] ].set_func(id);
		}
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay(SE_SELECT);
		GoodsSelectFunc[ TaskTable[id].work[SCENE_NO] ].cancel_func(id);
	}
}


//-------------------------
//		�O�b�Y���\��
//-------------------------
static void DrawGoodsName( u8 id, u8 flg )
{
	u16	i,j;

	// ���
	if( scr == 0 )
		DelScrCurActor( MARK_ACT_U_CURSOR );
	if( scr != 0 )
		AddScrCurActor( MARK_ACT_U_CURSOR, GOODS_WINX+(((GOODS_WINH+1)>>1)<<3)+4, GOODS_WINY+8 );

	if( scr+7 == goods_max )
		DelScrCurActor( MARK_ACT_D_CURSOR );
	if( scr+7 < goods_max )
			AddScrCurActor( MARK_ACT_D_CURSOR, GOODS_WINX+(((GOODS_WINH+1)>>1)<<3)+4, (GOODS_WINY+GOODS_WINV)<<3 );

	// �O�b�Y��
	for( i=scr; i<scr+8; i++ ){

		if( mark_act[i-scr] != 0 )
			DelMarkActor( &mark_act[i-scr] );

		if( flg == 1 ){
			NMenuBoxWrite(
				GOODS_WINX+2, GOODS_WINY+2+(i-scr)*2,
				GOODS_WINX+GOODS_WINH-1, GOODS_WINY+3+(i-scr)*2	);
		}

		if( i == goods_max ){
			NMenuMsgWrite( str_modoru, GOODS_WINX+2, GOODS_WINY+2+(i-scr)*2 );
			break;
		}else{
			if( GoodsType[i] != 0 ){

				if( FURNITURE->type == MY_ROOM_GOODS_TYPE &&
					type_pos != GOODS_DOLL_FOLDER &&
					type_pos != GOODS_CUSHION_FOLDER &&
					TaskTable[id].work[SCENE_NO] == 0 )
				{
					PalChangeMojiPut(
						base_goods[ GoodsType[i] ].name,
						NOT_PUT_PAL,
						GOODS_WINX+2, GOODS_WINY+2+(i-scr)*2 );
				}else{
					NMenuMsgWrite(
						base_goods[ GoodsType[i] ].name,
						GOODS_WINX+2, GOODS_WINY+2+(i-scr)*2 );
				}

				for( j=0; j<16; j++ ){
					if( set_goods[j]-1 == i ){
						AddMarkActor(
							MARK_ACT_MARK1, &mark_act[i-scr],
							GOODS_MARK_PUT_X*8+4, (GOODS_WINY+3+(i-scr)*2)*8 );
						break;
					}
				}
				for( j=0; j<12; j++ ){
					if( set_room[j]-1 == i ){
						AddMarkActor(
							MARK_ACT_MARK2, &mark_act[i-scr],
							GOODS_MARK_PUT_X*8+4, (GOODS_WINY+3+(i-scr)*2)*8 );
						break;
					}
				}
			}
		}
	}
}

//---------------------------------
//		�O�b�Y�E�B���h�E�\��
//---------------------------------
static void DrawGoodsWin( u8 id )
{
	NMenuWinBoxWrite(GOODS_WINX,GOODS_WINY,GOODS_WINX+GOODS_WINH,GOODS_WINY+GOODS_WINV);
	DrawGoodsName( id, 0 );
}

//--------------------------------
//		��ޖ��E�B���h�E�\��
//--------------------------------
static void DrawGoodsTypeSWin( u8 no )
{
	NMenuWinBoxWrite( GDS_TYPE_WINX2, GDS_TYPE_WINY2, GDS_TYPE_WINEX2, GDS_TYPE_WINEY2 );
	GoodsTypeStrPut( no, GDS_TYPE_WINX2+1, GDS_TYPE_WINY2+1, 0xff );
}

//-----------------------------
//		�O�b�Y�����\��
//-----------------------------
static void GoodsInfPut( u8 no )
{
	NMenuWinBoxWrite( GDS_INFO_WINX, GDS_INFO_WINY, GDS_INFO_WINEX, GDS_INFO_WINEY );
	NMenuMsgWrite( base_goods[ GoodsType[no] ].msg, GDS_INFO_WINX+1, GDS_INFO_WINY+1 );
}

//--------------------------------------------
//		��ޖ��E�O�b�Y�����E�B���h�E����
//--------------------------------------------
void DelGoodsTypeInfoWin(void)
{
	NMenuBoxClear( GDS_TYPE_WINX2, GDS_TYPE_WINY2, GDS_TYPE_WINEX2, GDS_TYPE_WINEY2 );
	NMenuBoxClear( GDS_INFO_WINX, GDS_INFO_WINY, GDS_INFO_WINEX, GDS_INFO_WINEY );
}

//--------------------------------------
//		�O�b�Y�I���E�B���h�E�֖߂�
//--------------------------------------
void ReturnGoodsSelectWin( u8 id )
{
	AddMarkPal();
	TaskTable[id].TaskAdrs = GoodsSelectInit;
}

//-------------------------------------
//		�u����Ă���O�b�Y���L��
//-------------------------------------
static u8 BasePutCheck( u8 pos )
{
	u8	i;

	for( i=0; i<16; i++ ){
		if( set_goods[i] == pos )	return	TRUE;
	}
	return	FALSE;
}

void SetPutGoodsNo( u8 id )
{
	u16	h,i,j,k = 0;

	for( i=0; i<16; i++ ){
		set_goods[i] = 0;
		if( i < 12 )	set_room[i] = 0;
	}

	// ��n�z�u�O�b�Y���L��
	for( i=0; i<16; i++ ){
		if( Fld.SecretBase[0].furniture_id[i] != 0 ){
			for( j=0; j<goods_box[ type_pos ].max; j++ ){
				if( GoodsType[j] == Fld.SecretBase[0].furniture_id[i] ){
					for( h=0; h<k; h++ ){
						if( set_goods[h] == j+1 )	break;
					}
					if( h == k ){
						set_goods[k] = j+1;
						k++;
						break;
					}
				}
			}
		}
	}

	// ����z�u�O�b�Y���L��
	for( i=0; i<12; i++ ){
		if( Fld.MyRoomFurnitureID[i] != 0 ){
			for( j=0; j<goods_box[ type_pos ].max; j++ ){
				if( GoodsType[j] == Fld.MyRoomFurnitureID[i] &&
					BasePutCheck( j+1 ) == FALSE )
				{
					for( h=0; h<k; h++ ){
						if( set_room[h] == j+1 )
							break;
					}
					if( h == k ){
						set_room[k] = j+1;
						k++;
						break;
					}
				}
			}
		}
	}
}

//-----------------------------------
//		�ő�O�b�Y�I�𐔂̐ݒ�
//-----------------------------------
static void SetPosMax(void)
{
	if( goods_max <= 7 )	pos_max = goods_max;
	else					pos_max = 7;
}

//---------------------------------------
//		�O�b�Y�I��p�p�����[�^�ݒ�
//---------------------------------------
void SelectGoodsParamInit( u8 id )
{
	SetPutGoodsNo(id);
	SetPosMax();
	pos = 0;
	scr = 0;
}

//--------------------------------
//		�O�b�Y�E�B���h�E�폜
//--------------------------------
void DelGoodsWindow(void)
{
	DelMarkActorAll( mark_act, 8 );
	DelScrCurActor( MARK_ACT_U_CURSOR );
	DelScrCurActor( MARK_ACT_D_CURSOR );
	NMenuBoxClear(GOODS_WINX,GOODS_WINY,GOODS_WINX+GOODS_WINH,GOODS_WINY+GOODS_WINV);
}

//------------------------------------------
//		�I�������O�b�Y���u����Ă��邩
//------------------------------------------
u8	GoodsPutCheck(void)
{
	u16	i;

	for( i=0; i<16; i++ ){
		if( set_goods[i] == scr+pos+1 )
			return	FALSE;
		if( i < 12 ){
			if( set_room[i] == scr+pos+1 )
				return	FALSE;
		}
	}
	return	TRUE;
}

// �u����Ă���
void SelectGoodsPutNot( u8 id )
{
	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
		AddMarkPal();
		TaskTable[id].TaskAdrs = GoodsSelectInit;
	}
}

//-------------------------------------
//		�O�b�Y�I���L�����Z������
//-------------------------------------
static void GoodsSelectCancel2( u8 id )
{
	NMenuBoxClear(GOODS_WINX,GOODS_WINY,GOODS_WINX+GOODS_WINH,GOODS_WINY+GOODS_WINV);
	GoodsTypeSelectInit(id);
}

static void GoodsSelectCancel( u8 id )
{
	DelMarkActorAll( mark_act, 8 );
	DelScrCurActor( MARK_ACT_U_CURSOR );
	DelScrCurActor( MARK_ACT_D_CURSOR );
	DelMarkPal();
	TaskTable[id].TaskAdrs = GoodsSelectCancel2;
}

//-------------------------------------------------
//		�O�b�Y���������Ƃ��̃p�����[�^�Đݒ�
//-------------------------------------------------
void DelGoodsParamSet( u8 id )
{
	goods_max--;
	if( scr+7 > goods_max && scr != 0 )	scr--;
	GoodsSort2( type_pos );
	SetPutGoodsNo(id);
	SetPosMax();
}

//-----------------------------
//		������ʒu�ݒ�
//-----------------------------
void GoodsFuncGlobalPosition( u8 id )
{
	TaskTable[id].work[ HERO_POSX ] = Fld.MapXpos;
	TaskTable[id].work[ HERO_POSY ] = Fld.MapYpos;
	GetHeroCellPos(&TaskTable[id].work[CELL_POSX],&TaskTable[id].work[CELL_POSY]);
}

//------------------------------------------
//		�Z�������������Ď�l���𒆐S��
//------------------------------------------
void SetHeroGlobalPosition( u8 id )
{
	MakeMapBG();
	NextMapSetDirect(
		Fld.MapID.div_id,
		Fld.MapID.map_id,
		NO_EXIT_ID,
		TaskTable[id].work[ HERO_POSX ],
		TaskTable[id].work[ HERO_POSY ] );
	SetNextWorldPosition();
}
