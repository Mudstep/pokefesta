//=========================================================================================
//									�A�C�e���ʏ���
//								2001/10/26 by nakahiro
//=========================================================================================
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "fld_main.h"
#include "script.h"
#include "pokelist.h"
#include "menu.h"
#include "f_pmlist.h"
#include "b_pmlist.h"
#include "map_attr.h"
#include "fieldmap.h"
#include "task.h"
#include "bag.h"
#include "poketool.h"
#include "weather.h"
#include "evobj.h"
#include "jiki.h"
#include "client.h"
#include "gold.h"
#include "item.h"
#include "item_def.h"
#include "seed.h"
#include "base.h"
#include "mapparam.h"
#include "mail.h"
#include "itemtool.h"
#include "fight.h"
#include "fld_tool.h"
#include "cubecase.h"
#include "fld_task.h"
#include "fld_talk.h"
#include "syswork.h"		//OBJCHRWORK�Q�Ƃ̂���
#include "letter.h"			//TVFishingCount
#include "mus_tool.h"
#include "sxytype.h"
#include "initgame.h"
#include "coin.h"
#include "message.h"
#include "record.h"
#include "server.h"

#include "../data/itemmsg.h"	//�A�C�e���g�p���b�Z�[�W
#include "../evdata\maparray.h"
#include "../script\saveflag.h"

#include "sysflag.h"

//=========================================================================================
//											�V���{��
//=========================================================================================
#define	INFOWINX	0		//�����޳X���W
#define	INFOWINY	13		//�����޳Y���W
#define	INFOWINH	13		//�����޳��
#define	INFOWINV	7		//�����޳����

#define	YESNO_WINX	7		// �͂��E������X���W
#define	YESNO_WINY	7		// �͂��E������Y���W
#define	YESNO_WINH	5		// �͂��E��������
#define	YESNO_WINV	4		// �͂��E����������

#define	WHITE_CHAR	1

//�^�X�N���[�N�ԍ�
#define	RETURN_ADR_H	8
#define	RETURN_ADR_L	9
#define	FADE_WORK		10
#define	RETURN_WORK		15

#define GET_ADR_H(n) ( ( (u32)n & 0xffff0000)  >> 16 )
#define GET_ADR_L(n) ( (u32)n & 0x0000ffff) 


//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void BagExitTask(u8);
extern void FieldItemUseInit(void);		//�t�B�[���h�֕��A��̏����ݒ�

extern void FieldBagPokeListInit(void);

extern void PokeListStatusRecover( u8, u16, pTaskFunc );
extern void PokeListAllDethRecover( u8, u16, pTaskFunc );

extern void BagItemUseSubSet(void);

//=========================================================================================
//										�O���[�o���ϐ�
//=========================================================================================
extern u16	ReturnItemNo;			//�A�C�e���ԍ�
extern u8	UseFldPokeFlg;			//����/��������

extern void (*FieldReturnItemUse)(u8);		//�t�B�[���h���A��̔�ѐ�

//�t�B�[���h�߂��
void (* const FieldReturn[])() = {
	FieldBagPokeListInit,	// �|�P�������X�g
	FieldMainRecover,		// ���C��
	CubeCaseInit,			// �L���[�u�P�[�X
};


//----------------------------------------------------
//		�߂��A�h���X���Z�b�g���ăo�b�O���I��
//----------------------------------------------------
static void SetReturnAdrsMain( u8 id )
{
	u8	no;

	if( ReturnItemNo == ITEM_NAZONOMI )
		no = (u8)( TaskTable[id].work[ RETURN_WORK ] - 1 );
	else
		no = ItemFldSysGet( ReturnItemNo ) - 1;

	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H( FieldReturn[ no ] );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L( FieldReturn[ no ] );
	TaskTable[id].TaskAdrs = BagExitTask;
}

static void SetReturnAddress( u8 id )
{
	SetReturnAdrsMain(id);
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}

// �֗��{�^������ʉ�ʂɐ؂�ւ�铹��p
static void SetReturnAddress2( u8 id )
{
	SetReturnAdrsMain(id);
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
}

//---------------------------------
//		��؂Ȃ��̗p�g�p����
//---------------------------------
static void ExtraItemUse( u8 id )
{
	if(TaskTable[id].work[2] != 1){
		pFieldRecoverFunc = FieldItemUseInit;
		SetReturnAddress(id);
	}else	FieldReturnItemUse(id);
}

//----------------------------
//		�G���[���b�Z�[�W
//----------------------------
static void SetErrorMsgMain( u8 id, u8 flg, const u8 * error_msg )
{
	PM_MsgExpand( MsgExpandBuffer, error_msg );

	if( flg == 0 ){
		NMenuBoxClear( INFOWINX, INFOWINY, INFOWINX+INFOWINH, INFOWINY+INFOWINV );
		SetTalkMsg( id, MsgExpandBuffer, EndBagItemError, 1 );
	}else{
		SetTalkMsg( id, MsgExpandBuffer, EndFldItemError, 0 );
	}
}

static void SetErrorMsg2( u8 id, u8 flg )
{
	SetErrorMsgMain( id, flg, msg_item_use_error );
}

static void SetErrorMsgCyclingRoad( u8 id, u8 flg )
{
	SetErrorMsgMain( id, flg, msg_cycling_road_error );
}

//----------------------------------
//		�A�C�e���̎�ނ𒲂ׂ�
//----------------------------------
u8	ItemTypeCheck( u16 no )
{
	if( ItemFldFuncGet( no ) == ITEMPG_FLD_SKILL )
		return	SKILL_ITEM;
	if( ItemFldFuncGet( no ) == ITEMPG_FLD_EVOLUTION )
		return	EVO_ITEM;

	return	DUMMY_ITEM;
}


//=========================================================================================
//										���[��
//=========================================================================================
extern void ReturnMailUseInit(void);	// ���[���g�p��̕��A����
extern void MailViewInit( MAIL_DATA *, pFunc, u8 );

static void FieldBagMailUse( u8 id )
{
	MAIL_DATA	mail;

	if( !(FadeData.fade_sw) ){
		mail.design_no = ReturnItemNo;
		MailViewInit( &mail, ReturnMailUseInit, 0 );
		DelTask(id);
	}
}

void FieldBagMailUseInit( u8 id )
{
	PaletteFadeReq(0xffffffff,0,0,16,0);
	TaskTable[id].TaskAdrs = FieldBagMailUse;
}


//=========================================================================================
//										���]��
//=========================================================================================

static void FieldSetCycle(u8);

//--------------------------------
//		���]�Ԃ��g�p�ł��邩
//--------------------------------
void CheckFieldCycle(u8 id)
{
	s16	px, py;
	u8	attr;

	GetHeroCellPos( &px, &py );
	attr = GetCellAttribute( px, py );

	if( EventFlagCheck( SYS_CYCLING_ROAD ) == TRUE ||
		ATR_IsVBridgeCheck( attr ) == TRUE ||
		ATR_IsHBridgeCheck( attr ) == TRUE ||
		ATR_IsVBoardCheck( attr ) == TRUE ||
		ATR_IsHBoardCheck( attr ) == TRUE )
	{
		SetErrorMsgCyclingRoad( id, TaskTable[id].work[2] );
		return;
	}else if( IsEnableCycleNowMap() == TRUE && CheckRideCycle() == 0 )
	{
		FieldReturnItemUse = FieldSetCycle;
		ExtraItemUse(id);
	}else{
		SetErrorMsg2( id, TaskTable[id].work[2] );
	}
}

//-------------------------
//		���]�ԃZ�b�g
//-------------------------
static void FieldSetCycle(u8 id)
{
	if( ItemWorkGet( ReturnItemNo ) == 0 )	MyCycleRequest(MY_MV_BIT_CYCLE_R);
	if( ItemWorkGet( ReturnItemNo ) == 1 )	MyCycleRequest(MY_MV_BIT_CYCLE_S);
	TalkEnd();
	ResetForceEvent();
	DelTask(id);
}



//=========================================================================================
//										�肴��
//=========================================================================================
#include "fld_talk.h"

static void FishingStart( u8 id );		// ��l���A�j����


//---------------------------------
//		�ނ�Ƃ��g�p�ł��邩
//---------------------------------
static int check_fishing_condition( void )
{
	s16	px,py;
	u16 attr;

	GetHeroFrontCellPos(&px,&py);
	attr = GetCellAttribute(px,py);

	// ��ɂ͂肪�ł��Ȃ�
	if( ATR_IsWaterFallCheck(attr) ) return FALSE;

	// �_�C�r���O���͂肪�ł��Ȃ�
	if( CheckHeroState(MY_MV_BIT_DIVING) ) return FALSE;

	if( CheckHeroState(MY_MV_BIT_SWIM) == 0 ) {
		// �n�ォ��Ȃ݂̂�ł�������Ȃ�肪�ł���
		if ( CheckHeroAttrNaminoriStart() ) return TRUE;
	} else {
		// �Ȃ݂̂��Ԃ���ڂ̑O�����Ői���\�Ȃ�肪�ł���
		if ( ATR_IsWaterCheck(attr)
				&& GetCellHitCheck(px,py) == 0 ) return TRUE;
		// �g����ԂŖڂ̑O�����̃A�g���r���[�g�Ȃ�肪�ł���
		if (ATR_BridgeCheck(attr) == TRUE) return TRUE;
	}
	return FALSE;
}

void CheckFieldFishing(u8 id)
{
	if ( check_fishing_condition() == TRUE ) {
		FieldReturnItemUse = FishingStart;
		ExtraItemUse(id);
	}else{
		SetErrorMsg2( id, TaskTable[id].work[2] );
	}
}

//--------------------------
//		��l���A�j����
//--------------------------
static void FishingStart( u8 id )
{
	SetHeroFishingTask( ItemWorkGet( ReturnItemNo ) );
	DelTask(id);
}


//=========================================================================================
//									�_�E�W���O�}�V��
//=========================================================================================
static void DowsingStart( u8 id );
static void DowsingMsg( u8 id );
static void DowsingEnd( u8 id );
static u8	DowsingCheck( const SXY_DATA * bg, u8 id );

static void ItemRangeSet( u8 id, s16 rx, s16 ry );
static u8	ItemSiteGet( s16 rx, s16 ry );

static void DowsingHeroSiteChange( u8 site );
static void DowsingSiteChangeWait( u8 id );
static void DowsingPointAnm( u8 id );

static void DowsingOtherMap(u8 id);

#define	WK_RANGE_X		0
#define	WK_RANGE_Y		1
#define	WK_RANGE_FLG	2
#define	WK_CNT			3
#define	WK_SE_CNT		4
#define	WK_HERO_SITE	5

static const u8 dm_point_site[] = { SITE_U, SITE_R, SITE_D, SITE_L };

//--------------------------------
//		"����"���I�����ꂽ
//--------------------------------
void FldItemDowsingMachine( u8 id )
{
	IncRecord( CNT_DOWSING );			//�_�E�W���O�}�V�����g�����񐔂��J�E���g
	FieldReturnItemUse = DowsingStart;
	ExtraItemUse(id);
}

//---------------------------------------------
//		�A�C�e�������܂��Ă��邩�`�F�b�N
//---------------------------------------------
static void DowsingStart( u8 id )
{
	if( DowsingCheck( NowMap.Sxy, id ) == TRUE )
		TaskTable[id].TaskAdrs = DowsingMsg;
	else
		SetTalkMsg( id, msg_dgm_not, DowsingEnd, 0 );
}

//----------------------------------------------
//		�����S��炷���ă��b�Z�[�W�\��
//----------------------------------------------
static void DowsingMsg( u8 id )
{
	u8	item_site;
	u8	site;
	u8	i;

	s16 * wk = TaskTable[id].work;

	if( wk[WK_CNT] == 0 ){
		if( wk[WK_SE_CNT] == 4 ){

			item_site = ItemSiteGet( wk[WK_RANGE_X], wk[WK_RANGE_Y] );
			if( item_site != 0 ){
				DowsingHeroSiteChange( dm_point_site[item_site-1] );
				TaskTable[id].TaskAdrs = DowsingSiteChangeWait;
			}else{
				site = GetHeroSite();
				for( i=0; i<4; i++ ){
					if( site == dm_point_site[i] )
						wk[WK_HERO_SITE] = ( i+1 ) & 3;
				}
				TaskTable[id].TaskAdrs = DowsingPointAnm;
				wk[WK_CNT]       = 0;
				wk[WK_RANGE_FLG] = 0;
			}

			return;
		}
		SePlay( SE_DAUGI );
		wk[WK_SE_CNT]++;
	}

	wk[WK_CNT] = ( wk[WK_CNT] + 1 ) & 0x1f;
}

//----------------
//		�I��
//----------------
static void DowsingEnd( u8 id )
{
	NMenuBoxClear( 0, 14, 29, 19 );
	TalkEnd();
	ResetForceEvent();
	DelTask(id);
}

//--------------------------------
//		�B���A�C�e���`�F�b�N
//--------------------------------
static u8 DowsingCheck( const SXY_DATA * bg, u8 id )
{
	s16	i,x,y,px,py;
	u16	flg;

	GetHeroCellPos( &x, &y );

	TaskTable[id].work[WK_RANGE_FLG] = 0;

	for( i=0; i<bg->evBgMax; i++ ){
		if( bg->BgData[i].type == BG_ITEMPICK ){
			flg = ( 0xffff & ( (u32)bg->BgData[i].script >> 16 ) ) + FH_FLAG_START;
			if( EventFlagCheck( flg ) == FALSE ){
				px = ( MAP_LEFT_MARGIN + bg->BgData[i].posX ) - x;
				py = ( MAP_UP_MARGIN + bg->BgData[i].posY ) - y;

				if( px >= -7 && px <= 7 && py >= -5 && py <= 5 )

					ItemRangeSet( id, px, py );

			}
		}
	}

	DowsingOtherMap(id);

	if( TaskTable[id].work[WK_RANGE_FLG] == 1 )
		return	TRUE;
	
	return	FALSE;
}


//--------------------------------
//	�אڃ}�b�v�̉B���A�C�e���`�F�b�N
//--------------------------------
static u8 check_hide_item(const SXY_DATA * sxy, s16 x, s16 y)
{
	const SXY_BG * bg;
	int i, max;
	u16 flg;

	max = sxy->evBgMax;
	bg = sxy->BgData;
	for (i = 0; i < max; i++) {
		if (bg[i].type != BG_ITEMPICK)
			continue;
		if (x == bg[i].posX && y == bg[i].posY) {
			flg = ( 0xffff & ( (u32)bg[i].script >> 16 ) ) + FH_FLAG_START;
			// �擾�t���O��FALSE���܂��A�C�e��������
			if (EventFlagCheck(flg) == FALSE)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}

static u8 check_linkmap_item(const MLink * link, int x, int y)
{
	const MTD * next;
	s16 cx, cy;

	next = GetNextLinkMapData(link);
	switch (link->direction) {
	case SITE_UP:
		cx = x - MAP_LEFT_MARGIN - link->PosDiff;
		cy = next->Screen->Ysize + (y - MAP_UP_MARGIN);
		break;
	case SITE_DOWN:
		cx = x - MAP_LEFT_MARGIN - link->PosDiff;
		cy = y - MAP_UP_MARGIN - NowMap.Screen->Ysize;
		break;
	case SITE_LEFT:
		cx = next->Screen->Xsize + (x - MAP_LEFT_MARGIN);
		cy = y - MAP_UP_MARGIN - link->PosDiff;
		break;
	case SITE_RIGHT:
		cx =  x - MAP_LEFT_MARGIN - NowMap.Screen->Xsize;
		cy = y - MAP_UP_MARGIN - link->PosDiff;
		break;
	default:
		return FALSE;
	}
	return check_hide_item(next->Sxy, cx, cy);
}

static void DowsingOtherMap(u8 id)
{
	s16 x, y;
	s16 sx, sy, ex, ey;
	s16 hx, hy;
	const MLink * link;

	sx = MAP_LEFT_MARGIN;
	sy = MAP_UP_MARGIN;
	ex = MAP_LEFT_MARGIN + NowMap.Screen->Xsize;
	ey = MAP_UP_MARGIN + NowMap.Screen->Ysize;

	GetHeroCellPos(&hx, &hy);
	for (x = hx - 7; x<= hx + 7; x ++) {
		for (y = hy - 5; y <= hy + 5; y ++) {
			if (sx <= x && x < ex && sy <= y && y < ey) continue;
			link = GetLinkMap(x,y);
			if (link == NULL) continue;
			if (check_linkmap_item(link, x, y) == TRUE) {
				ItemRangeSet( id, x - hx, y - hy );
			}
		}
	}
}

//----------------------------------------------
//	�A�C�e���܂ł̋������Z�b�g
//----------------------------------------------
static void ItemRangeSet( u8 id, s16 rx, s16 ry )
{
	s16   x1, x2, y1, y2;
	s16 * wk = TaskTable[id].work;

	if( wk[WK_RANGE_FLG] == 0 ){
		wk[WK_RANGE_X]   = rx;
		wk[WK_RANGE_Y]   = ry;
		wk[WK_RANGE_FLG] = 1;
		return;
	}

	if( wk[WK_RANGE_X] < 0 )	x1 = wk[WK_RANGE_X] * (-1);
	else						x1 = wk[WK_RANGE_X];
	if( wk[WK_RANGE_Y] < 0 )	y1 = wk[WK_RANGE_Y] * (-1);
	else						y1 = wk[WK_RANGE_Y];

	if( rx < 0 )	x2 = rx * (-1);
	else			x2 = rx;
	if( ry < 0 )	y2 = ry * (-1);
	else			y2 = ry;

	if( x1+y1 > x2+y2 ){
		wk[WK_RANGE_X] = rx;
		wk[WK_RANGE_Y] = ry;
		return;
	}

	if( x1+y1 == x2+y2 ){
		if( y1 > y2 ){
			wk[WK_RANGE_X] = rx;
			wk[WK_RANGE_Y] = ry;
			return;
		}
		if( y1 == y2 ){
			if( wk[WK_RANGE_Y] < ry ){
				wk[WK_RANGE_X] = rx;
				wk[WK_RANGE_Y] = ry;
			}
		}
	}
}

//----------------------------------------------
//	�A�C�e��������������擾
//----------------------------------------------
static u8 ItemSiteGet( s16 rx, s16 ry )
{
	s16	px, py;

	if( rx == 0 && ry == 0 )	return	0;	// ���̏�

	if( rx < 0 )	px = rx * (-1);
	else			px = rx;

	if( ry < 0 )	py = ry * (-1);
	else			py = ry;

	if( px > py ){
		if( rx < 0 )	return	4;	// ��
		else			return	2;	// �E
	}
	if( px < py ){
		if( ry < 0 )	return	1;	// ��
		else			return	3;	// ��
	}

	if( px == py ){
		if( ry < 0 )	return	1;	// ��
		else			return	3;	// ��
	}

	return	0;
}

//----------------------------------------------
//	��l���̌�����ς���
//----------------------------------------------
static void DowsingHeroSiteChange( u8 site )
{
	AcmdEnd( EvObjSpWorkAdrs(EV_OBJ_SPID) );			// ���@�����A�j������
	AcmdFreeWork( EvObjSpWorkAdrs(EV_OBJ_SPID) );
	EvObjPauseClr( EvObjSpWorkAdrs(EV_OBJ_SPID) );
	HeroAcmdWalkTurn( site );
}

//----------------------------------------------
//	�����]���҂�
//----------------------------------------------
static void DowsingSiteChangeWait( u8 id )
{
	if( AcmdEndChk( EvObjSpWorkAdrs(EV_OBJ_SPID) ) == 1 )
		SetTalkMsg( id, msg_dgm_hit, DowsingEnd, 0 );
}

//----------------------------------------------
//	���̏��]
//----------------------------------------------
static void DowsingPointAnm( u8 id )
{
	s16 * wk = TaskTable[id].work;

	if( AcmdEndChk( EvObjSpWorkAdrs(EV_OBJ_SPID) ) == 1 || wk[WK_RANGE_FLG] == 0 ){
		DowsingHeroSiteChange( dm_point_site[ wk[WK_HERO_SITE] ] );
		wk[WK_RANGE_FLG] = 1;
		wk[WK_HERO_SITE] = ( wk[WK_HERO_SITE] + 1 ) & 3;
		wk[WK_CNT]++;
		if( wk[WK_CNT] == 4 )
			SetTalkMsg( id, msg_dgm_hit_point, DowsingEnd, 0 );
	}
}


//=========================================================================================
//									�L���[�u�P�[�X
//=========================================================================================
extern void ReturnBagInit(void);

void FldItemCubeCase( u8 id )
{
	if( ItemToolSioCheck() == TRUE ){
		SetErrorMsg2( id, TaskTable[id].work[2] );
		return;
	}

	if(TaskTable[id].work[2] != 1){
		CubeCaseCall( BAG_CUBECASE_ID );	// �L���[�u�P�[�X�Ăяo��
		SetReturnAddress(id);
	}else{
		pFieldRecoverFunc = AddFieldEventEndFadeInTask;
		CubeCaseCall( CNV_CUBECASE_ID );	// �L���[�u�P�[�X�Ăяo��
		SetReturnAddress2(id);
	}
}

//=========================================================================================
//									�R�C���P�[�X
//=========================================================================================
void FldItemCoinCase( u8 id )
{
	PM_NumMsgSet( StrTempBuffer0, G_CheckCoin(), NUM_MODE_LEFT, 4 );
	PM_MsgExpand( MsgExpandBuffer, msg_coincase );

	if( TaskTable[id].work[2] == 0 ){
		NMenuBoxClear( INFOWINX, INFOWINY, INFOWINX+INFOWINH, INFOWINY+INFOWINV );
		SetTalkMsg( id, MsgExpandBuffer, EndBagItemError, 1 );
	}else{
		SetTalkMsg( id, MsgExpandBuffer, EndFldItemError, 0 );
	}
}


//=========================================================================================
//								���b�Z�[�W��\������A�C�e��
//=========================================================================================
//--------------------
//		�I���҂�
//--------------------
static void EndBagUseMsg( u8 id )
{
	if( sys.Trg & A_BUTTON ){
		EndBagItemError(id);
	}
}

static void EndFldUseMsg( u8 id )
{
	if( sys.Trg & A_BUTTON ){
		EndFldItemError(id);
	}
}

//--------------------------------------
//		�A�C�e���g�p���b�Z�[�W�\��
//--------------------------------------
void SetUseMsg( u8 id )
{
	if( TaskTable[id].work[2] == 0 ){
		NMenuBoxClear( INFOWINX, INFOWINY, INFOWINX+INFOWINH, INFOWINY+INFOWINV );
		SetTalkMsg( id, FldUseItemMsg[ ItemWorkGet(ReturnItemNo) ], EndBagUseMsg, 1 );
	}else{
		SetTalkMsg( id, FldUseItemMsg[ ItemWorkGet(ReturnItemNo) ], EndFldUseMsg, 0 );
	}
};


//=========================================================================================
//											�^�l
//=========================================================================================
extern const VM_CODE seed_set_script[];
static void SeedItemUseInit(u8);

//-------------------------------
//		�^�l���g�p�ł��邩
//-------------------------------
void CheckSeedUse( u8 id )
{
	if( SeedSetEnableCheck() == TRUE ){
		FieldReturnItemUse = SeedItemUseInit;
		pFieldRecoverFunc = FieldItemUseInit;

		TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H( FieldMainRecover );
		TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L( FieldMainRecover );
		TaskTable[id].TaskAdrs = BagExitTask;
		PaletteFadeReq(0xffffffff,0,0,16,0);

	}else{
		ItemFldFuncGet( ReturnItemNo )(id);
	}
}

//---------------------------------------
//		�^�l��A����X�N���v�g�N��
//---------------------------------------
static void SeedItemUseInit(u8 no)
{
	SubItem( ReturnItemNo, 1 );
	SetForceEvent();
	StartFieldScript(seed_set_script);
	DelTask(no);
}

//=========================================================================================
//										���傤��
//=========================================================================================
extern const VM_CODE seed_watering_script[];
static void ZyouroItemUseInit( u8 id );

//---------------------------------------
//---------------------------------------
void CheckZyouroUse( u8 id )
{
	if( ZyouroUseEnableCheck() == TRUE ){
		FieldReturnItemUse = ZyouroItemUseInit;
		ExtraItemUse(id);
	}else{
		SetErrorMsg2( id, TaskTable[id].work[2] );
	}
}

//---------------------------------------
//	���傤��������X�N���v�g���N��
//---------------------------------------
void ZyouroItemUseInit( u8 id )
{
	SetForceEvent();
	StartFieldScript( seed_watering_script );
	DelTask(id);
}


//=========================================================================================
//										�񕜃A�C�e��
//=========================================================================================
// �t�B�[���h�Ń|�P�����Ɏg�p���鋤�ʏ���
static void ItemUseFldPoke( u8 id )
{
	UseFldPokeFlg = PM_ITEM_USE_MODE;
	SetReturnAddress(id);
}

void FldPokeRecoverInit( u8 id )
{
	PokeListItemUseFunc = PokeListStatusRecover;
	ItemUseFldPoke(id);
}


//=========================================================================================
//									�S���m���񕜃A�C�e��
//=========================================================================================
void FldPokeAllDethRecoverInit( u8 id )
{
	u8	i;

	FPokeNo = 0;

	for( i=0; i<TEMOTI_POKEMAX; i++ ){
		if( PokeParaGet( &PokeParaMine[i], ID_monsno ) != 0 &&
			PokeParaGet( &PokeParaMine[i], ID_hp ) == 0 )
		{
			FPokeNo = i;
			break;
		}
	}

	PokeListItemUseFunc = PokeListAllDethRecover;
	UseFldPokeFlg = PM_ITEM_USE_ALLDETH_RCV_MODE;
	SetReturnAddress(id);
}


//=========================================================================================
//										PP�񕜃A�C�e��
//=========================================================================================
void FldPokePPRecoverInit( u8 id )
{
	PokeListItemUseFunc = PPRecoverInit;
	ItemUseFldPoke(id);
}


//=========================================================================================
//										�|�C���g�A�b�v
//=========================================================================================
void FldPokeWazaPointUpInit( u8 id )
{
	PokeListItemUseFunc = WazaPointUpInit;
	ItemUseFldPoke(id);
}


//=========================================================================================
//										�ӂ����ȃA��
//=========================================================================================
void FldPokeListLvUptUpInit( u8 id )
{
	PokeListItemUseFunc = PokeListLvUpInit;
	ItemUseFldPoke(id);
}


//=========================================================================================
//										�킴�}�V��
//=========================================================================================
static void CallSoundEffect(u8);
static void SkillKeyWait1(u8);
static void WazaSetYesNoInit(u8);
static void WazaSetYes(u8);

static const SELECT_ITEM	YesNoFunc = { WazaSetYes, EndBagItemError };

//-----------------------------
//		�N�����b�Z�[�W�\��
//-----------------------------
void SetPokeSkillInit( u8 id )
{
	NMenuBoxClear( INFOWINX, INFOWINY, INFOWINX+INFOWINH, INFOWINY+INFOWINV );

	if( ReturnItemNo >= ITEM_HIDENMASIN01 )
		SetTalkMsg( id, msg_waza_machine01, CallSoundEffect, 1 );
	else
		SetTalkMsg( id, msg_waza_machine00, CallSoundEffect, 1 );
}

//--------------------------
//		�N������炷
//--------------------------
static void CallSoundEffect( u8 id )
{
	SePlay( SE_PC_LOGIN );
	TaskTable[id].TaskAdrs = SkillKeyWait1;
}

//-----------------------------
//		���b�Z�[�W�E�F�C�g
//-----------------------------
static void SkillKeyWait1( u8 id )
{
	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
		PM_strcpy( StrTempBuffer0, &WazaNameData[GetWazaNoItem(ReturnItemNo)][0] );
		PM_MsgExpand( MsgExpandBuffer, msg_waza_machine02 );
		SetTalkMsg( id, MsgExpandBuffer, WazaSetYesNoInit, 1 );
	}
}

//------------------------
//		�͂��E������
//------------------------
static void WazaSetYesNoInit( u8 id )
{
	YesNoSelectInit( YESNO_WINX, YESNO_WINY );
	ItemBGBoxWrite( BG1_data, YESNO_WINX+1, YESNO_WINY+1, YESNO_WINH, YESNO_WINV, WHITE_CHAR );
	SelectToolInit( id, &YesNoFunc );
}
// �͂�
static void WazaSetYes( u8 id )
{
	PokeListItemUseFunc = ListPokeWazaSet;
	ItemUseFldPoke(id);
}


//=========================================================================================
//										�X�v���[
//=========================================================================================
static void UseMsgItemNoSet(void)
{
	SubItem( ReturnItemNo, 1 );
	BagItemUseSubSet();
	GetItemName( ReturnItemNo, StrTempBuffer1 );
	PM_MsgExpand( MsgExpandBuffer, msg_item_use );
}

void FldUseSpray( u8 id )
{
	if( GetEventWorkValue( WK_SPRAY_COUNT ) == 0 ){
		SetEventWorkValue( WK_SPRAY_COUNT, (u16)ItemAttackGet( ReturnItemNo ) );
		UseMsgItemNoSet();
		SetTalkMsg( id, MsgExpandBuffer, EndBagItemError, 1 );
	}else{
		SetTalkMsg( id, msg_spray_error, EndBagItemError, 1 );
	}
};


//=========================================================================================
//							�r�[�h���ɂ��G���J�E���g���ω�
//=========================================================================================
static void VidroUseMsgSet(void)
{
//	SubItem( ReturnItemNo, 1 );
	BagItemUseSubSet();
	GetItemName( ReturnItemNo, StrTempBuffer1 );
}


static void FldUseEncountVidroSeWait( u8 id )
{
	TaskTable[id].work[15]++;
	if( TaskTable[id].work[15] < 8 )	return;

	SePlay( SE_BIDORO );
	SetTalkMsg( id, MsgExpandBuffer, EndBagItemError, 1 );
}

void FldUseEncountVidro( u8 id )
{
	// ���낢�r�[�h��
	if( ReturnItemNo == ITEM_SIROIBIIDORO ){
		EventFlagSet( SYS_ENC_UP_ITEM );
		EventFlagReset( SYS_ENC_DOWN_ITEM );
		VidroUseMsgSet();
		PM_MsgExpand( MsgExpandBuffer, msg_w_vidro );
		TaskTable[id].TaskAdrs = FldUseEncountVidroSeWait;
		TaskTable[id].work[15] = 0;
		return;
	}

	// ���낢�r�[�h��
	if( ReturnItemNo == ITEM_KUROIBIIDORO ){
		EventFlagSet( SYS_ENC_DOWN_ITEM );
		EventFlagReset( SYS_ENC_UP_ITEM );
		VidroUseMsgSet();
		PM_MsgExpand( MsgExpandBuffer, msg_b_vidro );
		TaskTable[id].TaskAdrs = FldUseEncountVidroSeWait;
		TaskTable[id].work[15] = 0;
	}
}


//=========================================================================================
//									���Ȃʂ��̂Ђ�
//=========================================================================================
//----
#if 1
//----
void EscapeDungionMain( u8 id )
{
	InitHeroLastStatus();
	SetMapChangeAnanuke();
	DelTask( id );
}
//----
#else	// ��
//----
void EscapeDungionMain( u8 id )
{
	s16 * wk = TaskTable[id].work;

	switch ( wk[0] ) {
	case 0:
		PaletteFadeReq(0xffffffff,0,0,16,0);
		InitHeroLastStatus();
		wk[0]++;
		break;

	case 1:
		if ( FadeData.fade_sw ) break;

		NextMapSetByEscMapID();
		SetNextWorldPosition();
		pFieldRecoverFunc = AddFieldInTask;
		MainProcChange( FieldMainInit );
		DelTask(id);
		break;
	}
}
//----
#endif
//----

static void FldAnanukeMsg(u8 id)
{
	InitEventFlagAtEscape();		//���Ȃ��ق�ɂ��t���O������
	UseMsgItemNoSet();
	TaskTable[id].work[0] = 0;
	SetTalkMsg( id, MsgExpandBuffer, EscapeDungionMain, 0 );
}

u8 EscapeDungionCheck(void)
{
	if( NowMap.type == MAPTYPE_CAVE ) return TRUE;
	return	FALSE;
}

void FldAnanukeInit(u8 id)
{
	if( EscapeDungionCheck() == TRUE ){
		FieldReturnItemUse = FldAnanukeMsg;
		ExtraItemUse(id);
	}else{
		SetErrorMsg2( id, TaskTable[id].work[2] );
	}
}


//=========================================================================================
//										�i���A�C�e��
//=========================================================================================
void FldUseEvolutionItem( u8 id )
{
	PokeListItemUseFunc = PokeListEvolutionInit;
	ItemUseFldPoke(id);
}


//=========================================================================================
//									�����X�^�[�{�[���i�퓬�j
//=========================================================================================
extern void SetBattleReturnAddress(u8);		// �߂��A�h���X���Z�b�g���ăo�b�O���I��

void BtlMonsterBallUse(u8 id)
{
	if( BallThrowCheck() == 0 ){
		SubItem( ReturnItemNo, 1 );
		SetBattleReturnAddress( id );
	}else{
		NMenuBoxClear( INFOWINX, INFOWINY, INFOWINX+INFOWINH, INFOWINY+INFOWINV );
		SetTalkMsg( id, msg_pc_mons_box_max, EndBagItemError, 1 );
	}
}


//=========================================================================================
//								�퓬�p�X�e�[�^�X�A�b�v�A�C�e��
//=========================================================================================
static void BtlBagUseItamKeyWait( u8 id )
{
	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON )
		SetBattleReturnAddress(id);
}

static void BtlBagUseItemSeWait( u8 id )
{
	TaskTable[id].work[15]++;
	if( TaskTable[id].work[15] < 8 )	return;

	SePlay( SE_KAIFUKU );
	SubItem( ReturnItemNo, 1 );
	SetTalkMsg( id, FightItemUseMsgGet(ReturnItemNo), BtlBagUseItamKeyWait, 1 );
}

void BtlBagUseItem( u8 id )
{
	u16	pos = SelMonsNo[ TempClientNo ];

	NMenuBoxClear( INFOWINX, INFOWINY, INFOWINX+INFOWINH, INFOWINY+INFOWINV );

	if( StatusRecover( &PokeParaMine[pos], ReturnItemNo, pos, 0 ) != 0 ){
		SetTalkMsg( id, msg_poke_use_error00, EndBagItemError, 1 );
	}else{
		TaskTable[id].TaskAdrs = BtlBagUseItemSeWait;
		TaskTable[id].work[15] = 0;
	}
}



//=========================================================================================
//						�|�P�������X�g�Ŏg�p����A�C�e���̋��ʏ����i�퓬�j
//=========================================================================================
static void BtlPokeListItemUseMain( u8 id )
{
	if( !(FadeData.fade_sw) ){
		BagBattlePokeListInit();
		EvObjPalManInit();
		DelTask(id);
	}
}

static void BtlPokeListItemUseInit( u8 id )
{
	TaskTable[id].TaskAdrs = BtlPokeListItemUseMain;
	PaletteFadeReq(0xffffffff,0,0,16,0);
}


//=========================================================================================
//									�񕜃A�C�e���i�퓬�j
//=========================================================================================
void BtlStRecoverItem( u8 id )
{
	PokeListItemUseFunc = PokeListStatusRecover;
	BtlPokeListItemUseInit(id);
}


//=========================================================================================
//								�S���m���񕜃A�C�e���i�퓬�j
//=========================================================================================
void BtlAllDethRecoverItem( u8 id )
{
	PokeListItemUseFunc = PokeListAllDethRecover;
	BtlPokeListItemUseInit(id);
}


//=========================================================================================
//									PP�񕜃A�C�e���i�퓬�j
//=========================================================================================
void BtlPPRecoverItem( u8 id )
{
	PokeListItemUseFunc = PPRecoverInit;
	BtlPokeListItemUseInit(id);
}

//=========================================================================================
//									�ɂ������̂݁i�퓬�j
//=========================================================================================
void BtlItemUseNigaiKinomi( u8 id )
{
	NMenuBoxClear( INFOWINX, INFOWINY, INFOWINX+INFOWINH, INFOWINY+INFOWINV );

	if( SetStatusRecover( 0, ReturnItemNo, 0 ) == 0 ){
		SubItem( ReturnItemNo, 1 );
		PokeNickNameSet( &PokeParaMine[0], StrTempBuffer0 );
		PM_MsgExpand( MsgExpandBuffer, msg_konran_rcv );
		SetTalkMsg( id, MsgExpandBuffer, SetBattleReturnAddress, 1 );
	}else{
		SetTalkMsg( id, msg_poke_use_error00, EndBagItemError, 1 );
	}
}

//=========================================================================================
//								�s�b�s�l�`�i�����j�i�퓬�j
//=========================================================================================
void BtlEscapeItem( u8 id )
{
	NMenuBoxClear( INFOWINX, INFOWINY, INFOWINX+INFOWINH, INFOWINY+INFOWINV );

	if( ( FightType & FIGHT_TYPE_TRAINER ) == 0 ){
		UseMsgItemNoSet();
		SetTalkMsg( id, MsgExpandBuffer, SetBattleReturnAddress, 1 );
	}else{
		SetErrorMsg2( id, TaskTable[id].work[2] );
	}
}





//=========================================================================================
//								�i�]�̎��i RAM�؂̎� �j
//=========================================================================================
// �t�B�[���h
void FldItemUseRAMKinomi( u8 id )
{
	switch( ItemRecoverCheck( ReturnItemNo ) ){

	case ITEM_TYPE_HP_RCV:			// HP��
	case ITEM_TYPE_DOKU_RCV:		// �ŉ�
	case ITEM_TYPE_NEMURI_RCV:		// �����
	case ITEM_TYPE_YAKEDO_RCV:		// �Ώ���
	case ITEM_TYPE_KOORI_RCV:		// �X��
	case ITEM_TYPE_MAHI_RCV:		// ��჉�
	case ITEM_TYPE_ALL_ST_RCV:		// �X�e�[�^�X�ُ�S��
	case ITEM_TYPE_HP_UP:			// �h�[�s���O�A�C�e�� HP�w�͒lUP
	case ITEM_TYPE_ATC_UP:			// �h�[�s���O�A�C�e�� �U���w�͒lUP
	case ITEM_TYPE_DEF_UP:			// �h�[�s���O�A�C�e�� �h��w�͒lUP
	case ITEM_TYPE_AGL_UP:			// �h�[�s���O�A�C�e�� �f�����w�͒lUP
	case ITEM_TYPE_SPA_UP:			// �h�[�s���O�A�C�e�� ���U�w�͒lUP
	case ITEM_TYPE_SPD_UP:			// �h�[�s���O�A�C�e�� ���h�w�͒lUP
		TaskTable[id].work[ RETURN_WORK ] = FIELD_POKELIST;
		FldPokeRecoverInit( id );
		break;

	case ITEM_TYPE_ALLDETH_RCV:	// �S���m����
		TaskTable[id].work[ RETURN_WORK ] = FIELD_POKELIST;
		FldPokeAllDethRecoverInit(id);
		break;

	case ITEM_TYPE_LV_UP:		// LvUp�n
		TaskTable[id].work[ RETURN_WORK ] = FIELD_POKELIST;
		FldPokeListLvUptUpInit( id );
		break;

	case ITEM_TYPE_PP_UP:		// PPUp
	case ITEM_TYPE_PP_3UP:		// PP3Up
		TaskTable[id].work[ RETURN_WORK ] = FIELD_POKELIST;
		FldPokeWazaPointUpInit( id );
		break;

	case ITEM_TYPE_PP_RCV:		// PP��
		TaskTable[id].work[ RETURN_WORK ] = FIELD_POKELIST;
		FldPokePPRecoverInit( id );
		break;

	default:
		TaskTable[id].work[ RETURN_WORK ] = FIELD_BAG;
		DummyItemUseFunc( id );
	}
}

// �퓬
void BtlItemUseRAMKinomi( u8 id )
{
	switch( ItemRecoverCheck( ReturnItemNo ) ){

	case ITEM_TYPE_BTL_ST_UP:	// �퓬�p�X�e�[�^�X�A�b�v�n
		BtlBagUseItem( id );
		break;

	case ITEM_TYPE_HP_RCV:			// HP��
	case ITEM_TYPE_DOKU_RCV:		// �ŉ�
	case ITEM_TYPE_NEMURI_RCV:		// �����
	case ITEM_TYPE_YAKEDO_RCV:		// �Ώ���
	case ITEM_TYPE_KOORI_RCV:		// �X��
	case ITEM_TYPE_MAHI_RCV:		// ��჉�
	case ITEM_TYPE_KONRAN_RCV:		// ������
	case ITEM_TYPE_MEROMERO_RCV:	// ����������
	case ITEM_TYPE_ALL_ST_RCV:		// �X�e�[�^�X�ُ�S��
		BtlStRecoverItem( id );
		break;
/*
	case ITEM_TYPE_KONRAN_RCV:	// ������
		BtlItemUseNigaiKinomi( id );
		break;

	case ITEM_TYPE_ALLDETH_RCV:	// �S���m����
		BtlAllDethRecoverItem(id);
		break;
*/
	case ITEM_TYPE_PP_RCV:		// PP��
		BtlPPRecoverItem( id );
		break;

	default:
		DummyItemUseFunc( id );
	}
}


//=========================================================================================
//								�g�p�ł��Ȃ��A�C�e���i���j
//=========================================================================================
void DummyItemUseFunc( u8 id )
{
	SetErrorMsg2( id, TaskTable[id].work[2] );
}
