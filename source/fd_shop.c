//========================================================================================
//						�t�����h���B�V���b�v / �O�b�Y�V���b�v����
//								2001/09/20	by nakahiro
//========================================================================================
#include "common.h"
#include "intr.h"
#include "task.h"
#include "pm_str.h"
#include "menu.h"
#include "palanm.h"
#include "fld_main.h"
#include "field.h"
#include "map_attr.h"
#include "script.h"
#include "gold.h"
#include "pm_str.h"
#include "jiki.h"
#include "bag.h"
#include "item.h"
#include "letter.h"
#include "cm.h"
#include "fd_shop.h"
#include "base.h"
#include "itemtool.h"
#include "map.h"
#include "mapbg.h"
#include "fieldmap.h"
#include "mus_tool.h"
#include "gds_main.h"
#include "gds_del.h"
#include "goods.h"
#include "calctool.h"
#include "decord.h"
#include "message.h"
#include "record.h"
#include "laster.h"
#include "weather.h"
#include "fld_task.h"

#include "../data/itemsym.h"


//========================================================================================
//										�V���{��
//========================================================================================
#define	SHOP_MENUWINX	0	//�V���b�v���j���[�E�B���h�E��X���W
#define	SHOP_MENUWINY	0	//�V���b�v���j���[�E�B���h�E��Y���W

#define ITEM_WINX		14	//�A�C�e���\���E�B���h�EX���W
#define	ITEM_WINY		0	//�A�C�e���\���E�B���h�EY���W
#define ITEM_WINH		15	//�A�C�e���\���E�B���h�E��
#define ITEM_WINV		19	//�A�C�e���\���E�B���h�E����

#define	SHOP_SCR_CUR_X	( ( (ITEM_WINX+7) << 3 ) + 4 )	// �X�N���[���J�[�\�����W
#define	SHOP_SCR_CUR_UY	12
#define	SHOP_SCR_CUR_DY	148

#define MYGOLD_WINX		0	//�������E�B���h�EX���W
#define MYGOLD_WINY		0	//�������E�B���h�EY���W
#define MYGOLD_WINH		13	//�������E�B���h�E��
#define MYGOLD_WINV		3	//�������E�B���h�E����

#define INFO_WINX		0	//�A�C�e�����E�B���h�EX���W
#define INFO_WINY		12	//�A�C�e�����E�B���h�EY���W
#define INFO_WINH		13	//�A�C�e�����E�B���h�E��
#define INFO_WINV		7	//�A�C�e�����E�B���h�E����

#define PRICE_WINX		0	//���l�E�B���h�EX���W
#define PRICE_WINY		10	//���l�E�B���h�EY���W
#define PRICE_WINH		13	//���l�E�B���h�E��
#define PRICE_WINV		3	//���l�E�B���h�E����

#define	BLACK_CHAR		0xb300
//#define	BG1_PLUS_CHAR	384
#define	BG1_PLUS_CHAR	1024-32
#define	BG1_WIN_PAL		12
#define	BG1_WHITE_CHAR	BG1_PLUS_CHAR+1+( BG1_WIN_PAL << 12 )

//�^�X�N���[�N�ԍ�
#define PRICE			6	// �l�i
#define	RETURN_ADR_H	8	// ��ѐ�A�h���X�i��ʁj
#define	RETURN_ADR_L	9	// ��ѐ�A�h���X�i���ʁj

#define GET_ADR_H(n)	( ( (u32)n & 0xffff0000 )  >> 16 )
#define GET_ADR_L(n)	( (u32)n & 0x0000ffff ) 
#define JUMP_ADR(h,l)	( (u16)(h) << 16 | (u16)(l) )

#define	SHOP_MAP_ADRS	( (u16 *)( &UserWork[0x18000] ) )

enum {
	SHOP_OBJ_ID = 0,
	SHOP_OBJ_POSX,
	SHOP_OBJ_POSY,
	SHOP_OBJ_SITE,
};

//========================================================================================
//									�v���g�^�C�v�錾
//========================================================================================
static void ShopMenuTask(u8);		//�V���b�v���j���[�I��
static void ShopMenuFadeOut(u8);	//���j���[�I�����̃t�F�[�h�A�E�g

static void BuyItem(u8);			//���j���[�@"�����ɂ���"
static void SaleItem(u8);			//���j���[�@"����ɂ���"
static void EndShop(u8);			//���j���[�@"�Ȃ�ł��Ȃ��ł�"

//����
static void FdShopBuyInit(void);		//�����ݒ�
static void ShopScrCursorInit(void);	//�X�N���[���J�[�\���Z�b�g
static void ShopPaletteMake(void);		//�p���b�g�쐬
static void FdShopBuyMainTask(u8);		//�A�C�e����I��
static void FdShopBuyEndInit(u8);		//���������I���ݒ�
static void FdShopBuyEndMain(u8);		//���������I��
static void DrawItemName(void);			//�A�C�e�����\��
static void DrawInfoMsg(void);			//���\��
static void DrawShopItemWin(void);		//�A�C�e���E�B���h�E�\��
static void DrawItemWindow(void);		//�A�C�e���E�B���h�E�\��
static void DrawInfoWindow(void);		//�A�C�e�����E�B���h�E�\��
static void ReturnFdShopBuyMainTask(u8);//�A�C�e���I���֖߂�

static void CheckBuyItemPrice(u8);		//�A�C�e�����w���ł��邩�`�F�b�N
static void ShopBuyNo(u8);				//������

static void MakeShopBuyDisp2(void);		//�w����ʍ쐬
static void GetEvOBJData(void);
static void SetShopEvOBJ(void);

static void TVItemFunc(u8);				//TV�p�f�[�^�m��

extern void ShopSaleBagInit(void);		//�o�b�O��ʂ�
extern u8	SetItemNoKeyCheck(u8,u8);


//========================================================================================
//										�O���ϐ�
//========================================================================================
u32	shop_price = 0;		// ���l�E���l�̍��v

static s16	ev_obj_data[16][4] = {};	// EvOBJ Data

static const MENU_ITEM	menu[] =
{
	{ str_shop_kau,     BuyItem  },		//00:�����ɂ���
	{ str_shop_uru,     SaleItem },		//01:����ɂ���
	{ str_shop_nandemo, EndShop  },		//02:�Ȃ�ł��Ȃ��ł�
};

static const u8 fd_menu_list[3] = { 0, 1, 2 };	// �t�����h���B�V���b�v
static const u8 gd_menu_list[3] = { 0, 2 };		// �O�b�Y�V���b�v

static const u16 test_shop_item[] = {
	ITEM_HAIPAABOORU,	//�n�C�p�[�{�[��
	ITEM_SUUPAABOORU,	//�X�[�p�[�{�[��
	ITEM_MONSUTAABOORU,	//�����X�^�[�{�[��
	ITEM_KIZUGUSURI,	//����
	ITEM_WAZAMASIN01,	//�ŏ���
	ITEM_DOKUKESI,		//�ŏ���
	ITEM_DOKUKESI,		//�ŏ���
	ITEM_DOKUKESI,		//�ŏ���
	ITEM_DOKUKESI,		//�ŏ���
	ITEM_DOKUKESI,		//�ŏ���
	ITEM_DOKUKESI,		//�ŏ���
	0,
};

static const SELECT_ITEM	YesNoFunc = { CheckBuyItemPrice, ShopBuyNo };

static SHOP_WORK	Shop;


//=======================================================================
//					�v���O����
//=======================================================================

//-----------------------------
//		�V���b�v�����ݒ�
//-----------------------------
static u8 FdShopInit( u8 type )
{
	SetForceEvent();
	Shop.type = type;
	Shop.pos = 0;

	if( type == FRIEND_SHOP ){
		Shop.pos_max = 2;
		NMenuWinBoxWrite( SHOP_MENUWINX, SHOP_MENUWINY, SHOP_MENUWINX+10, SHOP_MENUWINY+7 );
		NMenuFreeListWrite( SHOP_MENUWINX+2, SHOP_MENUWINY+1, 3, menu, fd_menu_list );
	}else{
		Shop.pos_max = 1;
		NMenuWinBoxWrite( SHOP_MENUWINX, SHOP_MENUWINY, SHOP_MENUWINX+10, SHOP_MENUWINY+5 );
		NMenuFreeListWrite( SHOP_MENUWINX+2, SHOP_MENUWINY+1, 2, menu, gd_menu_list );
	}
	NMenuVCursorInit( SHOP_MENUWINX+1, SHOP_MENUWINY+1, Shop.pos_max+1, 0);

	return AddTask( ShopMenuTask, 8 );
}

//---------------------
//	�I���������Z�b�g
//---------------------
void FdShopEndFuncInit( pFunc func )
{
	Shop.EndFunc = func;
};

//---------------------
//		�i���Z�b�g
//---------------------
void FdShopItemInit( const u16 * item )
{
	u16	i = 0;

	Shop.item = item;	//���蕨�f�[�^

	Shop.max = 0;		//�i��

	while(1){
		if( Shop.item[i] == 0 )	break;
		Shop.max++;
		i++;
	}
}

//---------------------------------
//		�V���b�v���j���[�I��
//---------------------------------
static void ShopMenuTask(u8 id)
{
	MENUFUNC	func;

	if( sys.Repeat & U_KEY ){
		if( Shop.pos != 0 ){
			SePlay(SE_SELECT);
			Shop.pos = NMenuVCursorSet(-1);
		}
		return;
	}

	if( sys.Repeat & D_KEY ){
		if( Shop.pos != Shop.pos_max ){
			SePlay(SE_SELECT);
			Shop.pos = NMenuVCursorSet(1);
		}
		return;
	}
	
	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		if( Shop.type == FRIEND_SHOP )
			func = menu[ fd_menu_list[Shop.pos] ].ptr;
		else	func = menu[ gd_menu_list[Shop.pos] ].ptr;
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay(SE_SELECT);
		EndShop(id);
	}
}

//------------------------------------
//		���j���[�@"�����ɂ���"
//------------------------------------
static void BuyItem(u8 id)
{
	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H( FdShopBuyInit );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L( FdShopBuyInit );
	TaskTable[id].TaskAdrs = ShopMenuFadeOut;
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
}

//------------------------------------
//		���j���[�@"����ɂ���"
//------------------------------------
static void SaleItem(u8 id)
{
	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H( ShopSaleBagInit );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L( ShopSaleBagInit );
	TaskTable[id].TaskAdrs = ShopMenuFadeOut;
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
}

//----------------------------------------
//		���j���[�@"�Ȃ�ł��Ȃ��ł�"
//----------------------------------------
static void EndShop(u8 id)
{
	NMenuBoxClear( SHOP_MENUWINX, SHOP_MENUWINY, SHOP_MENUWINX+11, SHOP_MENUWINY+8 );
	TVShopDataSet();
	ResetForceEvent();
	DelTask(id);
	if( Shop.EndFunc != NULL ) Shop.EndFunc();
}

//------------------------------------------
//		���j���[�I�����̃t�F�[�h�A�E�g
//------------------------------------------
static void ShopMenuFadeOut(u8 id)
{
	if(!(FadeData.fade_sw)){
		MainProcChange( (void *)JUMP_ADR( TaskTable[id].work[ RETURN_ADR_H ],
										  TaskTable[id].work[ RETURN_ADR_L ] ) );
		DelTask(id);
	}
}

//----------------------------------
//		�V���b�v���j���[�֕��A
//----------------------------------
static void ReturnShopEnd( u8 id )
{
	FdShopInit( Shop.type );
	DelTask(id);
}

static void ReturnShopMain(u8 id)
{
	if( FieldFadeinCheck() != 1 )	return;

	if( Shop.type == FLOWER_SHOP )
		SetTalkMsg( id, msg_shop_buy58, ReturnShopEnd, 0 );
	else
		SetTalkMsg( id, msg_shop_buy08, ReturnShopEnd, 0 );
}

void ReturnShopInit(void)
{
//	PaletteFadeReq(0xffffffff,0,16,0,0);
	FieldFadeBlackInSet();
	AddTask( ReturnShopMain, 8);
}

//---------------------------
//		���T�C�N���I��
//---------------------------
void ShopRecycleEnd( u8 id )
{
	ReturnShopMain(id);
}

//--------------------------------------
//		�O�b�Y���������Ă��Ȃ��Ƃ�
//--------------------------------------
void ShopNoGoods( u8 id )
{
	TaskTable[id].TaskAdrs = ReturnShopMain;
}


//==========================================================================
//								"����"����
//==========================================================================

//============================================
//				���C������
//============================================
static void FdShopBuyMain(void)
{
	JumpActor();
	SetActor();
	MainTask();
	PaletteAnime();
}

//============================================
//					���荞��
//============================================
static void VInterFunc( void )
{
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
	DIV_DMAARRAYCOPY( 3, BG1_data, BASE_SCRN_ADRS( MAP_UP_SCRN_BASE  ), 16 );	//BG�}�b�v�]��
	DIV_DMAARRAYCOPY( 3, BG2_data, BASE_SCRN_ADRS( MAP_LOW_SCRN_BASE ), 16 );	//BG�}�b�v�]��
	DIV_DMAARRAYCOPY( 3, BG3_data, BASE_SCRN_ADRS( MAP_BTM_SCRN_BASE ), 16 );	//BG�}�b�v�]��
}


//============================================
//					�����ݒ�
//============================================
static void FdShopBuyInit(void)
{
	InitHVBlankFunc();				// HVBlank������
	LasterInit();

	*(vu16 *)REG_BG1HOFS = 0;
	*(vu16 *)REG_BG1VOFS = 0;
	*(vu16 *)REG_BG2HOFS = 0;
	*(vu16 *)REG_BG2VOFS = 0;
	*(vu16 *)REG_BG3HOFS = 0;
	*(vu16 *)REG_BG3VOFS = 0;

	FadeData.trans_stop = 1;

	DIV_DMACLEAR( 3, 0, OAM,  OAM_SIZE,  32 );	// OAM������

	DecordVram(
		(void *)shopwin_pch_ADRS,
		(void * )( BASE_CHR_ADRS( MAP_LOW_CHR_BASE )+MAP_CHR_2ND_NO*0x20+0x3c00 ) );
	DecordWram( (void *)shopwin_psc_ADRS, SHOP_MAP_ADRS );
	DecordPaletteWork( (void *)shopwin_pcl_ADRS, PA_BG12, 0x20 );

	ObjPalManInit();
	PaletteAnimeInit();			// ��گı�� ������
	InitActor();				// ����������

	InitTask();

	PrintScreenInit(BAG_TYPE);		//ү���ޕ\���̈揉����

	NMenuScreenInit(BAG_TYPE);			//�����f�[�^������

	MakeShopBuyDisp2();

	Shop.pos = 0;
	Shop.scr = 0;

	NMenuBoxClear( 0, 0, 32, 32 );
	DrawGoldWin( Fld.my_gold, MYGOLD_WINX, MYGOLD_WINY );
	DrawItemWindow();	//�A�C�e���E�B���h�E�\��
	DrawInfoWindow();	//�A�C�e�����E�B���h�E�\��
	ShopScrCursorInit();

	AddTask( FdShopBuyMainTask, 8 );

	ShopPaletteMake();	//�p���b�g�쐬

	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
	FadeData.trans_stop = 0;

	SetVBlankFunc(VInterFunc);	// �����݊֐��ݒ�(VBLANK)

	MainProcChange( FdShopBuyMain );
}

//------------------------
//		�p���b�g�쐬
//------------------------
static void ShopPaletteMake(void)
{
	u16	pal[] = { 0x41ee, 0x7fff };

	PaletteWorkSet( &pal[1], 13 * 16 + COL_BLACK, 2 );	// ����
	PaletteWorkSet( &pal[0], 13 * 16 + COL_DGRAY, 2 );	// �e
}

//--------------------------------------------
//	�X�N���[���J�[�\���Z�b�g
//--------------------------------------------
static void ShopScrCursorInit(void)
{
	ScrActInit();

	if( Shop.max >= 8 ){
		AddScrCurActor( MARK_ACT_U_CURSOR, SHOP_SCR_CUR_X, SHOP_SCR_CUR_UY );
		AddScrCurActor( MARK_ACT_D_CURSOR, SHOP_SCR_CUR_X, SHOP_SCR_CUR_DY );
		BanishScrCurActor( MARK_ACT_U_CURSOR, 1 );
	}
}

static void ShopScrCurControl(void)
{
	if( Shop.scr == 0 )	BanishScrCurActor( MARK_ACT_U_CURSOR, 1 );
	else				BanishScrCurActor( MARK_ACT_U_CURSOR, 0 );

	if( Shop.scr + 7 >= Shop.max )
		BanishScrCurActor( MARK_ACT_D_CURSOR, 1 );
	else	BanishScrCurActor( MARK_ACT_D_CURSOR, 0 );
}


//-------------------------
//		�w����ʍ쐬
//-------------------------
// ���L�������w��BG�ɃR�s�[
static void SetShopCharWhite( u16 * BG, s16 x, s16 y )
{
	BG[ x   + y ] = BG1_WHITE_CHAR;
	BG[ x+1 + y ] = BG1_WHITE_CHAR;
}
// BG�L�������w��BG�ɃR�s�[
static void SetShopBuyChar( u16 * BG, s16 x, s16 y, const u16 * chr )
{
	BG[ x   + y ]    = chr[0];
	BG[ x+1 + y ]    = chr[1];
	BG[ x   + y+32 ] = chr[2];
	BG[ x+1 + y+32 ] = chr[3];
}
// BG1,2,3�ɃR�s�[
static void SetShopBuyCell( s16 x, s16 y, const u16 * chr, u8 layer )
{
	x = x<<1;
	y = ( y << 6 ) + 64;

	switch( layer ){
	case 0:
		SetShopBuyChar( BG2_data, x, y, &chr[0] );
		SetShopBuyChar( BG1_data, x, y, &chr[4] );
		break;
	case 1:
		SetShopBuyChar( BG3_data, x, y, &chr[0] );
		SetShopBuyChar( BG2_data, x, y, &chr[4] );
		break;
	case 2:
		SetShopBuyChar( BG3_data, x, y, &chr[0] );
		SetShopBuyChar( BG1_data, x, y, &chr[4] );
	}
}
// BG2,3�ɋ����R�s�[
static void SetShopBuyCell2( s16 x, s16 y, const u16 * chr )
{
	x = x<<1;
	y = ( y << 6 ) + 64;

	SetShopBuyChar( BG3_data, x, y, &chr[0] );
	SetShopBuyChar( BG2_data, x, y, &chr[4] );
}
// BG���R�s�[
static void SetShopBuyBG(void)
{
	s16	px,py;
	s16	i,j;
	u16	cell;
	u8	layer;

	GetHeroFrontCellPos( &px, &py );	// ��l���̑O�̈ʒu

	px -= 3;
	py -= 3;

	for( i=0; i<6; i++ ){
		for( j=0; j<7; j++ ){
			cell  = GetCellCode( px+j, py+i );
			if( i != 5 && j != 6 ){
				layer = GetCellLayer( px+j, py+i );
				if( cell < 512 )
					SetShopBuyCell( j, i, &NowMap.Screen->MainCharData->CellCharData[cell*8], layer );
				else
					SetShopBuyCell( j, i, &NowMap.Screen->SubCharData->CellCharData[(cell-512)*8], layer );
			}else{
				if( cell < 512 )
					SetShopBuyCell2( j, i, &NowMap.Screen->MainCharData->CellCharData[cell*8] );
				else
					SetShopBuyCell2( j, i, &NowMap.Screen->SubCharData->CellCharData[(cell-512)*8] );
			}
			if( i == 0 && j != 0 && j != 6 )
				SetShopCharWhite( BG1_data, j<<1, (i<<6)+64 );
		}
	}
}

static void MakeShopBuyDisp2(void)
{
	BGDataInit();

	GetEvOBJData();
	SetShopEvOBJ();

	SetShopBuyBG();
}

//----------------------------------------
//		��ʂɔ��f������EvOBJ���擾
//----------------------------------------
static void GetEvOBJData(void)
{
	s16	x,y;
	u8	z,i,j,obj,pos=0;

	GetHeroFrontCellPos( &x, &y );
	z = GetHeroHeight();

	for( i=0; i<16; i++ ){
		ev_obj_data[i][0] = EV_OBJ_MAX;
	}

	for( i=0; i<5; i++ ){
		for( j=0; j<7; j++ ){
			obj = EvObjHitCheck( x-3+j, y-2+i, z );
			if( obj != EV_OBJ_MAX ){
				ev_obj_data[pos][0] = obj;
				ev_obj_data[pos][1] = j;
				ev_obj_data[pos][2] = i;
				if( EvObj[obj].site == SITE_D )	ev_obj_data[pos][3] = STOP_D_ST;
				if( EvObj[obj].site == SITE_U )	ev_obj_data[pos][3] = STOP_U_ST;
				if( EvObj[obj].site == SITE_L )	ev_obj_data[pos][3] = STOP_L_ST;
				if( EvObj[obj].site == SITE_R )	ev_obj_data[pos][3] = STOP_R_ST;
				pos++;
			}
		}
	}
}

//------------------------------
//		EvOBJ����ʂɔ��f
//------------------------------
static void SetShopEvOBJ(void)
{
	u8	i,act;

	for( i=0; i<16; i++ ){
		if( ev_obj_data[i][SHOP_OBJ_ID] != EV_OBJ_MAX ){
			act = EvObjMakeAddAct(
						EvObj[ ev_obj_data[i][SHOP_OBJ_ID] ].obj_code,
						DummyActMove,
						8 + (ev_obj_data[i][SHOP_OBJ_POSX] << 4),
						32 + (ev_obj_data[i][SHOP_OBJ_POSY] << 4),
						2 );
			ActAnmChg( &ActWork[act], ev_obj_data[i][SHOP_OBJ_SITE] );	
		}
	}
}

//----------------------------------
//		�A�C�e���E�B���h�E�\��
//----------------------------------
static void DrawShopItemWin(void)
{
	s16	i;

	for( i=0; i<32*32; i++ ){
		if( SHOP_MAP_ADRS[i] != 0 )
			BG1_data[i] = SHOP_MAP_ADRS[i] + BG1_PLUS_CHAR + ( BG1_WIN_PAL << 12 );
	}
}

static void DrawItemWindow(void)
{
//	NMenuBoxClear( ITEM_WINX, ITEM_WINY, ITEM_WINX+ITEM_WINH, ITEM_WINY+ITEM_WINV );
	DrawShopItemWin();
	DrawItemName();
	NMenuVCursorInit( ITEM_WINX, ITEM_WINY+2, 8, Shop.pos );
}

//---------------------------------------
//		�A�C�e�����E�B���h�E�\��
//---------------------------------------
static void DrawInfoWin(void)
{
	u16	i,j;

	for( i=0; i<8; i++ ){
		for( j=0; j<14; j++ ){
			BG1_data[ (i+12)*32+j ] = SHOP_MAP_ADRS[ (i+12)*32+j ] + BG1_PLUS_CHAR + ( BG1_WIN_PAL << 12 );
		}
	}
}

static void DrawInfoWindow(void)
{
//	NMenuBoxClear( INFO_WINX, INFO_WINY, INFO_WINX+INFO_WINH, INFO_WINY+INFO_WINV );
	DrawInfoMsg();
}

//------------------------------
//		���l�E�B���h�E�\��
//------------------------------
static void DrawPriceGoldWindow(u8 id)
{
	DrawGold(
		(ItemPriceGet(Shop.item[Shop.scr+Shop.pos])>>SALE_FLG)*TaskTable[id].work[1],
		6, PRICE_WINX+6, PRICE_WINY+1 );

	PM_NumMsgSet( StrTempBuffer0, TaskTable[id].work[1], NUM_MODE_SPACE, 2 );

	NMenuMsgWrite( str_batu, PRICE_WINX+1, PRICE_WINY+1 );
	ItemBGBoxWrite( BG1_data, PRICE_WINX+1, PRICE_WINY+1, PRICE_WINH-1, PRICE_WINV-1, BG1_WHITE_CHAR );
}

//---------------------------
//		�A�C�e�����\��
//---------------------------
static void DrawItemName2( u16 no, u8 y )
{
	GetItemName( no, StrTempBuffer0 );
	NMenuMsgWrite( StrTempBuffer0, ITEM_WINX+1, y );
	DrawGold( ItemPriceGet(no) >> SALE_FLG, 4, ITEM_WINX+10, y );
}
static void DrawGoodsName( u16 no, u8 y )
{
	NMenuMsgWrite( base_goods[no].name, ITEM_WINX+1, y );
	DrawGold( base_goods[no].price, 5, ITEM_WINX+9, y );
}

static void DrawItemName(void)
{
	u8	i;

	for( i=0; i<8; i++ ){
		if( Shop.scr + i == Shop.max )	break;
		NMenuBoxWrite( ITEM_WINX+1, i*2+ITEM_WINY+2, ITEM_WINX+ITEM_WINH-1, i*2+ITEM_WINY+3 );
		if( Shop.type == FRIEND_SHOP )
			DrawItemName2( Shop.item[ Shop.scr+i ], i*2+ITEM_WINY+2 );
		else
			DrawGoodsName( Shop.item[ Shop.scr+i ], i*2+ITEM_WINY+2 );
	}

	if( i != 8 ){
		NMenuBoxWrite( ITEM_WINX+1, i*2+ITEM_WINY+2, ITEM_WINX+ITEM_WINH-1, i*2+ITEM_WINY+3 );
		NMenuMsgWrite( str_yameru, ITEM_WINX+1, i*2+ITEM_WINY+2 );
	}
}

//--------------------
//		���\��
//--------------------
static void DrawInfoMsg(void)
{
	NMenuBoxWrite( INFO_WINX+1, INFO_WINY+1, INFO_WINX+INFO_WINH-1, INFO_WINY+INFO_WINV-1 );
	if( Shop.scr+Shop.pos != Shop.max ){
		if( Shop.type == FRIEND_SHOP )
			NMenuMsgWrite( ItemMessageGet(Shop.item[Shop.scr+Shop.pos]), INFO_WINX+1, INFO_WINY+1 );
		else
			NMenuMsgWrite( base_goods[Shop.item[Shop.scr+Shop.pos]].msg, INFO_WINX+1, INFO_WINY+1 );
	}else	NMenuMsgWrite( msg_shop_buy_end, INFO_WINX+1, INFO_WINY+1 );
}

//----------------
//		����
//----------------
//�w��
static void AddMyItemWait(u8 id)
{
	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		if(	Shop.item[Shop.scr+Shop.pos] == ITEM_MONSUTAABOORU &&
			TaskTable[id].work[1] >= 10 )
		{
			if( AddItem( ITEM_PUREMIABOORU, 1 ) == TRUE ){
				SetTalkMsg( id, msg_shop_buy09, ReturnFdShopBuyMainTask, BG1_WHITE_CHAR );
				return;
			}
		}
		ReturnFdShopBuyMainTask(id);
	}
}

static void AddMyItem(u8 id)
{
	IncRecord( CNT_SHOPPING );
	SubGold( &Fld.my_gold, shop_price );
	SePlay( SE_REGI );
	DrawWinOkozukai( Fld.my_gold, MYGOLD_WINX, MYGOLD_WINY );
	TaskTable[id].TaskAdrs = AddMyItemWait;
}

//����������Ȃ�/�|�P�b�g�ɋ󂫂������Ƃ�
static void NotBuyItem( u8 id )
{
	ReturnFdShopBuyMainTask( id );
}

//�A�C�e�����w���ł��邩�`�F�b�N
static void CheckBuyItemPrice(u8 id)
{
	NMenuBoxClear( PRICE_WINX+7, PRICE_WINY-2, PRICE_WINX+PRICE_WINH, PRICE_WINY+PRICE_WINV );
	ItemBGBoxWrite( BG1_data, PRICE_WINX+8, PRICE_WINY-1, 4, 4, 0 );
	DrawInfoWin();
	SetShopBuyBG();

	if( CheckGold( Fld.my_gold, shop_price ) ){
		if( Shop.type == FRIEND_SHOP ){
			if(	AddItem( Shop.item[Shop.scr+Shop.pos], TaskTable[id].work[1] ) ){
				SetTalkMsg( id, msg_shop_buy03, AddMyItem, BG1_WHITE_CHAR );
				TVItemFunc(id);
			}else	SetTalkMsg( id, msg_shop_buy06, NotBuyItem, BG1_WHITE_CHAR );
		}else{
			if(	AddGoods( (u8)Shop.item[Shop.scr+Shop.pos] ) ){
				if( Shop.type == GOODS_SHOP )
					SetTalkMsg( id, msg_shop_buy04, AddMyItem, BG1_WHITE_CHAR );
				else
					SetTalkMsg( id, msg_shop_buy54, AddMyItem, BG1_WHITE_CHAR );
			}else{
				PM_MsgExpand( MsgExpandBuffer, msg_shop_buy07 );
				SetTalkMsg( id, MsgExpandBuffer, NotBuyItem, BG1_WHITE_CHAR );
			}
		}
	}else{
		SetTalkMsg( id, msg_shop_buy05, NotBuyItem, BG1_WHITE_CHAR );
	}
}

//-----------------------
//		�͂��^������
//-----------------------
static void ShopBuyYesNoInit(u8 id)
{
	YesNoSelectInit( PRICE_WINX+7, PRICE_WINY-2 );
	ItemBGBoxWrite( BG1_data, PRICE_WINX+8, PRICE_WINY-1, 4, 4, BG1_WHITE_CHAR );
	SelectToolInit( id, &YesNoFunc );
}

// ������
static void ShopBuyNo( u8 id )
{
	NMenuBoxClear( PRICE_WINX+7, PRICE_WINY-2, PRICE_WINX+PRICE_WINH, PRICE_WINY+PRICE_WINV );
	ItemBGBoxWrite( BG1_data, PRICE_WINX+8, PRICE_WINY-1, 4, 4, 0 );
	ReturnFdShopBuyMainTask(id);
}

//-------------------------
//		�������I��
//-------------------------
static void ShopBuyNoTask(u8 id)
{
	if( SetItemNoKeyCheck( id, (u16)Shop.buy_max ) == TRUE )	DrawPriceGoldWindow(id);

	if(	sys.Trg & A_BUTTON ){
		shop_price = (ItemPriceGet(Shop.item[Shop.scr+Shop.pos])>>SALE_FLG)*TaskTable[id].work[1];

		NMenuBoxClear( PRICE_WINX, PRICE_WINY, PRICE_WINX+PRICE_WINH, PRICE_WINY+PRICE_WINV );
		ItemBGBoxWrite(BG1_data,PRICE_WINX+1,PRICE_WINY+1,PRICE_WINH-1,PRICE_WINV-1,0);
		DrawInfoWin();
		SetShopBuyBG();

		GetItemName( Shop.item[ Shop.scr+Shop.pos], StrTempBuffer0 );
		PM_NumMsgSet( StrTempBuffer1, TaskTable[id].work[1], NUM_MODE_LEFT, 2 );
		PM_NumMsgSet( StrTempBuffer2, shop_price, NUM_MODE_LEFT, 8 );
		PM_MsgExpand( MsgExpandBuffer, msg_shop_buy01 );

		SetTalkMsg( id, MsgExpandBuffer, ShopBuyYesNoInit, BG1_WHITE_CHAR );
		return;
	}

	if(	sys.Trg & B_BUTTON ){
		ReturnFdShopBuyMainTask(id);
	}
}

static void ShopBuyNoInit(u8 id)
{
	u16	max;

	TaskTable[id].work[1] = 1;
	NMenuWinBoxWrite( PRICE_WINX, PRICE_WINY, PRICE_WINX+PRICE_WINH, PRICE_WINY+PRICE_WINV );
	DrawPriceGoldWindow(id);

	max = Fld.my_gold / ( ItemPriceGet(Shop.item[Shop.scr+Shop.pos]) >> SALE_FLG );
	if( max > 99 )	Shop.buy_max = 99;
	else			Shop.buy_max = (u8)max;

	TaskTable[id].TaskAdrs = ShopBuyNoTask;
}

//-------------------------------
//		�����A�C�e����I��
//-------------------------------
static void FdShopBuyMainTask( u8 id )
{
	if( FadeData.fade_sw )	return;

	if( ( sys.Repeat & PLUS_KEY_MASK ) == U_KEY ){
		if( Shop.pos == 0 ){
			if( Shop.scr != 0 ){
				SePlay( SE_SELECT );
				Shop.scr--;
				DrawItemName();
				DrawInfoMsg();
				ShopScrCurControl();
			}
		}else{
			SePlay( SE_SELECT );
			Shop.pos = NMenuVCursorSet( -1 );
			DrawInfoMsg();
		}
		return;
	}

	if( ( sys.Repeat & PLUS_KEY_MASK ) == D_KEY ){
		if(Shop.pos == 7 ){
			if( Shop.scr + Shop.pos != Shop.max ){
				SePlay( SE_SELECT );
				Shop.scr++;
				DrawItemName();
				DrawInfoMsg();
				ShopScrCurControl();
			}
		}else{
			if( Shop.pos != Shop.max){
				SePlay( SE_SELECT );
				Shop.pos = NMenuVCursorSet( 1 );
				DrawInfoMsg();
			}
		}
		return;
	}

	if(	sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		if( Shop.scr + Shop.pos != Shop.max ){
			ScrCurActAnmStopInit( MARK_ACT_U_CURSOR );
			ScrCurActAnmStopInit( MARK_ACT_D_CURSOR );
			BanishScrCurActor( MARK_ACT_D_CURSOR, 1 );
			PalChangeCursorPut( ITEM_WINX, ITEM_WINY+2+Shop.pos*2, 13 );
			NMenuBoxClear( INFO_WINX, INFO_WINY, INFO_WINX+INFO_WINH, INFO_WINY+INFO_WINV );

			if( Shop.type == FRIEND_SHOP ){
				shop_price = ItemPriceGet(Shop.item[Shop.scr+Shop.pos]) >> SALE_FLG;
				if( CheckGold( Fld.my_gold, shop_price ) == FALSE ){
					SetTalkMsg( id, msg_shop_buy05, NotBuyItem, BG1_WHITE_CHAR );
					return;
				}
				GetItemName( Shop.item[ Shop.scr+Shop.pos ], StrTempBuffer0 );
				PM_MsgExpand( MsgExpandBuffer, msg_shop_buy00 );
				SetTalkMsg( id, MsgExpandBuffer, ShopBuyNoInit, BG1_WHITE_CHAR );
			}else{
				shop_price = base_goods[ Shop.item[ Shop.scr+Shop.pos ] ].price;
				if( CheckGold( Fld.my_gold, shop_price ) == FALSE ){
					SetTalkMsg( id, msg_shop_buy05, NotBuyItem, BG1_WHITE_CHAR );
					return;
				}
				PM_strcpy( StrTempBuffer0, base_goods[Shop.item[Shop.scr+Shop.pos]].name );
				PM_NumMsgSet( StrTempBuffer1, shop_price, NUM_MODE_LEFT, 8 );
				if( Shop.type == GOODS_SHOP )
					PM_MsgExpand( MsgExpandBuffer, msg_shop_buy02 );
				else
					PM_MsgExpand( MsgExpandBuffer, msg_shop_buy52 );
				SetTalkMsg( id, MsgExpandBuffer, ShopBuyYesNoInit, BG1_WHITE_CHAR );
			}
		}else	FdShopBuyEndInit(id);
		return;
	}

	if(	sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		FdShopBuyEndInit(id);
	}
}

//------------------------------
//		�A�C�e���I���֖߂�
//------------------------------
static void ReturnFdShopBuyMainTask(u8 id)
{
	NMenuBoxClear( 1, 14, 28, 19 );
	NMenuBoxClear( PRICE_WINX, PRICE_WINY, PRICE_WINX+PRICE_WINH, PRICE_WINY+PRICE_WINV );
	ItemBGBoxWrite( BG1_data, PRICE_WINX+1, PRICE_WINY+1, PRICE_WINH-1, PRICE_WINV-1, 0 );
	SetShopBuyBG();
	DrawItemWindow();	//�A�C�e���E�B���h�E�\��
	DrawInfoWindow();	//�A�C�e�����E�B���h�E�\��
	ScrCurActAnmPlay( MARK_ACT_U_CURSOR );
	ScrCurActAnmPlay( MARK_ACT_D_CURSOR );
	ShopScrCurControl();
	TaskTable[id].TaskAdrs = FdShopBuyMainTask;
}

//----------------------------
//		���������I���ݒ�
//----------------------------
static void FdShopBuyEndInit(u8 id)
{
	pFieldRecoverFunc = ReturnShopInit;
	PaletteFadeReq(0xffffffff,0,0,16,0);
	TaskTable[id].TaskAdrs = FdShopBuyEndMain;
}

//------------------------
//		���������I��
//------------------------
static void FdShopBuyEndMain(u8 id)
{
	if( FadeData.fade_sw )	return;

	DelOkozukaiWin( MYGOLD_WINX, MYGOLD_WINY );
	DelMarkPal();
	MainProcChange( FieldMainRecover );
	DelTask(id);
}





//==========================================================================
//							�e���r�p�A�C�e���f�[�^
//==========================================================================
#define	TV_CNT_MAX	3	// �L�^��

MINEITEM	TVItemData[TV_CNT_MAX] = {};	//�e���r�p�A�C�e���f�[�^
u8	TVItemCnt = 0;							//�e���r�p�J�E���g

//---------------------------
//		TV�p�f�[�^�m��
//---------------------------
static void TVItemFunc( u8 id )
{
	u16	i;

	for( i=0; i<TV_CNT_MAX; i++ ){
		if( TVItemData[i].item == Shop.item[ Shop.scr+Shop.pos ] &&
			TVItemData[i].no != 0	)
		{
			if( TVItemData[i].no+TaskTable[id].work[1] > 255 ){
				TVItemData[i].no = 255;
			}else{
				TVItemData[i].no = TVItemData[i].no + TaskTable[id].work[1];
			}
			return;
		}
	}

	if( TVItemCnt < TV_CNT_MAX ){
		TVItemData[ TVItemCnt ].item = Shop.item[ Shop.scr+Shop.pos ];
		TVItemData[ TVItemCnt ].no = TaskTable[id].work[1];
		TVItemCnt++;
	}
}

//-----------------------------
//		TV�p�f�[�^������
//-----------------------------
static void TVItemDataInit(void)
{
	TVItemCnt = 0;
	ItemDataInit( TVItemData, TV_CNT_MAX );
}





//==========================================================================
//							�V���b�v��т���
//==========================================================================
// �t�����h���B�V���b�v
void FdShopSet( u32 adrs )
{
	FdShopInit( FRIEND_SHOP );
	FdShopItemInit( (const u16 *)adrs );
	TVItemDataInit();
	FdShopEndFuncInit( ContinueFieldScript );
};

// �O�b�Y�V���b�v
void GoodsShopSet( u32 adrs )
{
	FdShopInit( GOODS_SHOP );
	FdShopItemInit( (const u16 *)adrs );
	FdShopEndFuncInit( ContinueFieldScript );
};

// �t�����[�V���b�v
void FlowerShopSet( u32 adrs )
{
	FdShopInit( FLOWER_SHOP );
	FdShopItemInit( (const u16 *)adrs );
	FdShopEndFuncInit( ContinueFieldScript );
};


// �f�o�b�O�p
#ifdef	PM_DEBUG

void DebugShopDataSet(void)
{
	FdShopInit( FRIEND_SHOP );
	FdShopItemInit( test_shop_item );
	FdShopEndFuncInit( NULL );
}

#endif	PM_DEBUG






//========================================================================================
//========================================================================================
//==================================================
//	�G�X�J���[�^�[�i��)
//==================================================
#define	CNT_WORK	0
#define	PTN_WORK	1
#define	FLG_WORK	2
#define	ANM_WORK	3
#define	POSX_WORK	4
#define	POSY_WORK	5

static const s16 cell_tbl[] = {
	644, 642, 640,
	645, 643, 641,
	652, 650, 648,
	653, 651, 649,

	672, 674, 676,
	673, 675, 677,
	680, 682, 684,
};

static u8 escalator_id = 0;

static void CheckCellAnm( u8 id, const s16 * cell, u16 hit_check )
{
	s16	i,j,no;
	s16	px,py,cnt;

	px  = TaskTable[id].work[ POSX_WORK ]-1;
	py  = TaskTable[id].work[ POSY_WORK ]-1;
	cnt = TaskTable[id].work[ PTN_WORK ];

	if( TaskTable[id].work[ FLG_WORK ] == 0 ){
		for( i=0; i<3; i++ ){
			for( j=0; j<3; j++ ){
				no = GetCellCode( px+j, py+i );
				if( cell[cnt] == no ){
					if( cnt != 2 )	SetCell( px+j, py+i, ( cell[cnt+1] | hit_check ) );
					else			SetCell( px+j, py+i, ( cell[0] | hit_check ) );
				}
			}
		}
	}else{
		for( i=0; i<3; i++ ){
			for( j=0; j<3; j++ ){
				no = GetCellCode( px+j, py+i );
				if( cell[2-cnt] == no ){
					if( cnt != 2 )	SetCell( px+j, py+i, ( cell[2-cnt-1] | hit_check ) );
					else			SetCell( px+j, py+i, ( cell[2] | hit_check ) );
				}
			}
		}
	}
}

static void PCCellAnmMain( u8 id )
{
	s16 * wk = TaskTable[id].work;

	wk[ ANM_WORK ] = 1;

	switch( wk[ CNT_WORK ] ){
	case 0:
		CheckCellAnm( id, &cell_tbl[0], 0 );
		break;
	case 1:
		CheckCellAnm( id, &cell_tbl[3], 0 );
		break;
	case 2:
		CheckCellAnm( id, &cell_tbl[6], CELL_HITCHECK_MASK );
		break;
	case 3:
		CheckCellAnm( id, &cell_tbl[9], 0 );
		break;
	case 4:
		CheckCellAnm( id, &cell_tbl[12], CELL_HITCHECK_MASK );
		break;
	case 5:
		CheckCellAnm( id, &cell_tbl[15], 0 );
		break;
	case 6:
		CheckCellAnm( id, &cell_tbl[18], 0 );
	}

	wk[ CNT_WORK ] = ( wk[ CNT_WORK ]+1 ) & 0x7;

	if( wk[ CNT_WORK ] == 0 ){
		MakeMapBG();
		wk[ PTN_WORK ] = ( wk[ PTN_WORK ]+1 ) % 3;
		wk[ ANM_WORK ] = 0;
	}
}

static u8 PCCellAnmSet( u16 flg )
{
	u8	id;
	s16 * wk;

	id = AddTask( PCCellAnmMain, 0 );
	wk = TaskTable[id].work;

	GetHeroCellPos( &wk[ POSX_WORK ], &wk[ POSY_WORK ] );

	wk[ CNT_WORK ] = 0;
	wk[ PTN_WORK ] = 0;
	wk[ FLG_WORK ] = flg;

	PCCellAnmMain(id);

	return	id;
}

void AddEscalatorBGAnm( u8 no )		// no : 0 = DOWN, 1 = UP
{
	escalator_id = PCCellAnmSet(no);
}

void DelEscalatorBGAnm(void)
{
	DelTask( escalator_id );
}

u8	CheckEscalatorBGAnm(void)
{
	if( TaskTable[ escalator_id ].work[ ANM_WORK ] == 0 &&
		TaskTable[ escalator_id ].work[ PTN_WORK ] == 2 )
	{
		return	FALSE;
	}
	return	TRUE;
}



//---------------------------------------
//	�f�o�b�O�p
//---------------------------------------
#ifdef	PM_DEBUG

u8 DebugPCDownAnmTest(void)
{
	u8	id;

	id = PCCellAnmSet(0);
	TaskTable[id].work[ POSX_WORK ]--;
	MenuEnd();
	return	1;
}
u8 DebugPCUpAnmTest(void)
{
	u8	id;

	id = PCCellAnmSet(1);
	TaskTable[id].work[ POSX_WORK ]--;
	MenuEnd();
	return	1;
}

#endif	PM_DEBUG
