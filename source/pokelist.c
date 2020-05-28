//=========================================================================
//
//		�|�P�����莝�����X�g
//														by matsuda
//=========================================================================
#include "common.h"
#include "ef_tool.h"
#include "calctool.h"
#include "actor.h"
#include "actanm.h"

#include "poketool.h"
#include "madefine.h"
#include "decord.h"
#include "cli_num.h"
#include "cli_def.h"
#include "gauge.h"
#include "client_t.h"
#include "print.h"
#include "palanm.h"
#include "pokelist.h"

#include "menu.h"
#include "param.h"
#include "bss.h"
#include "task.h"
#include "intr.h"
#include "pokeicon.h"
#include "extanm.h"
#include "window.h"
#include "server.h"
#include "fight.h"
#include "wazatool.h"
#include "itemtool.h"
#include "mus_tool.h"

#include "field.h"
#include "mail.h"
#include "item.h"
#include "bag.h"
#include "b_pmlist.h"
#include "f_pmlist.h"
#include "cubecase.h"
#include "shinka.h"
#include "p_status.h"
#include "my_pc.h"
#include "seed.h"
#include "message.h"
#include "laster.h"

#include "monsno.def"
#include "wazano.def"
#include "waza_mcn.def"


//=========================================================================
//	�O���[�o���ϐ�
//=========================================================================
extern TALK_WIN	FightMsg;
extern TALK_WIN * pWin;
extern u8	UseFldPokeFlg;

u8 ListStatus = 0;	//�߹��ؽĂ��ǂ̏������s���ďI���������Ԃ�
u8 ListBuffer = 0;	//�߹��ؽĂ̖߂�l
u8 ListMsgStatus = 0;	//0:ү���ޕ\�����ł͂Ȃ��@�@1:ү���ޕ\����

// �A�C�e���g�p���� 2002/02/18 by nakahiro
// id = Ҳ������ID  itemno = ���єԍ�  ReturnAdrs = �߂��̱��ڽ
void (*PokeListItemUseFunc)( u8 id, u16 itemno, pTaskFunc ReturnAdrs );


//=========================================================================
//	�O���֐��錾
//=========================================================================
extern void MakeMenuWindow(u8 x,u8 y,u8 w,u8 max,const MENU_ITEM *menu,const u8 *List);


//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
static void PokeListInit(void);	// ���������[�v
static u8	DefoListInit(void);	// ���ʏ����`�揈��

static void PokeListRegisterInit(void);
static void PokeListNumberPut(u16 number, u8 TransCharNo, u8 yose, u8 xpos, u8 ypos);
void PokemonSort(u8 taskno);
static u8	GetPataChrActNo(u8 taskno, u8 patachr_no);
static void SetPataChrActNo(u8 taskno, u8 patachr_no, u8 actno);
static u8	GetNowCursorActNo(u8 taskno);
static void SetNowCursorActNo(u8 taskno, u8 actno);
static void PokemonSortEnd(u8 taskno);
void PokemonSortEnd2(u8 taskno);
static void StatusAreaClear(u8 pos);
static void PokeScrMsgPut(u8 id);
static u8	GetItemIconActNo(u8 taskno, u8 get_pos);
static void ItemIconSeq(actWork *Xreg);
static void ScrMsgWait(u8 id);
static void ItemKoukanYesNoInit(u8 id);
static void MailTensouYesNoInit(u8 id);
static void WazaKoukanYesNoInit(u8 id);
static void WazaKoukanYesNoMain(u8 id);
static void WazaAkirameYesNoInit(u8 id);
static void WazaAkirameYesNoMain(u8 id);
static void ListPokeWazaSet2(u8 id);
static void DelWazaSelectInit(u8 id);
static void WazaOboetaSeq(u8 id);
static void WazaAkirameSelectSeq(u8 id);
static void LifeScrMsgWait(u8 id);
static void LifeScrMsgWait2(u8 id);
static void LifeGaugeUpMain(u8 id);
static void WazaPPRecoverSet(u8);
static void WazaPPRecoverCancel(u8);
static void SetAzukaruMsg( u8, u16, u8 );	// "��������"���b�Z�[�W���Z�b�g
static void PokeActSelectMove( actWork * );
static void PokeActNotSelectMove( actWork * );
static void PokeActAnmChenge( u8, u8, u8 );
static void PokeSelectCard( const u8 * cp, u8 pos, u8 pal, u8 flg );
static void WazaOboeNextCheck( u8 id );
static u8 VidoroCheck( u16 item );	//	�r�[�h���`�F�b�N

static void PokeCardVBlank(void);	// VBlank���ŃJ�[�h��]��

//=========================================================================
//	�萔��`
//=========================================================================

//--------- ������ ----------//
#define P_FONT_SIZE		2					// 8x16
#define P_MONS_NAME_LEN	(6 * P_FONT_SIZE)	// EOM���܂߂��߹�݂̖��O�̒���
#define P_POKE_MAX		TEMOTI_POKEMAX		// �莝���߹��MAX
#define P_KETA			(4 * P_FONT_SIZE)	// ���l�\������(EOM���܂߂�4����(3��))
#define P_SPACE_LEN		P_FONT_SIZE
#define P_NAME_LEN		(P_MONS_NAME_LEN * P_POKE_MAX)
#define P_LV_LEN		(P_KETA * P_POKE_MAX)
#define P_HP_LEN		(P_KETA * P_POKE_MAX)
#define P_HPMAX_LEN		(P_KETA * P_POKE_MAX)

//--------- �����ް��]���ʒu ---------//
#define P_NAME_TRANS_CHARNO		(P_SPACE_LEN)
#define P_LV_TRANS_CHARNO		(P_NAME_TRANS_CHARNO + P_NAME_LEN)
#define P_HP_TRANS_CHARNO		(P_LV_TRANS_CHARNO + P_LV_LEN)
#define P_HPMAX_TRANS_CHARNO	(P_HP_TRANS_CHARNO + P_HP_LEN)

// �p���b�g
#define	CARD_MOJI_PAL	0
#define	CARD_B_PAL		3
#define	CARD_Y_PAL		4
#define	CARD_R_PAL		5
#define	CARD_G_PAL		6
#define	CARD_FLASH_PAL	4
#define	MODORU_DEF_PAL	1
#define	MODORU_POS_PAL	2
#define	CONDITION_PAL	0xb

#define	HP_GAUGE_HP_PAL	3
#define	HP_GAUGE_G_PAL	4
#define	HP_GAUGE_Y_PAL	5
#define	HP_GAUGE_R_PAL	6


//=========================================================================
//	�ް�
//=========================================================================
#include "..\scaddata\itemicon.ach"
#include "..\scaddata\itemicon.acl"
#include "pokelist.dat"

//-- �Z�I������ --//
static const SELECT_ITEM waza_select = { WazaPPRecoverSet, WazaPPRecoverCancel };


//==================================================================
//	�v���O����
//==================================================================






//========================================================================================
//							�S�|�P�������X�g���ʌĂяo��
//========================================================================================
extern void FieldPokeListTask(u8);		// �t�B�[���h�|�P�����I���^�X�N
extern void CN_PokeListTask(u8);		// �R���e�X�g / �������f �|�P�����I���^�X�N
extern u8	ContestListInit(void);		// �R���e�X�g�����`�揈��
extern void BattlePokeListMain(u8);		// �o�g���|�P�����I���^�X�N
extern u8	BtlListInit(void);			// �o�g�������`�揈��
extern void MultiPokeSelectTask(u8);	// �}���`�o�g���Q���|�P�����I���^�X�N
extern u8	MultiListInit(void);		// �}���`�o�g���Q���|�P�����I�������`�揈��
extern void ScrollInPokeListTask( u8 id );
extern u8	ScrollInListInit(void);
extern void GrowPokeSelectTask( u8 id );
extern u8	GrowListInit(void);
extern void ManiaPokeListTask(u8);
extern u8	ManiaListInit(void);


// ����������
typedef struct {
	void (* const MainTask)(u8);	// ���C���^�X�N
	u8 (* const InitFunc)(void);	// �`�揈��

	const u8 msg;		// ���b�Z�[�W
}POKELIST_INIT;

static const POKELIST_INIT InitTbl[] =
{
	{ FieldPokeListTask,    DefoListInit,     ERANDE_MSG },			// �t�B�[���h
	{ BattlePokeListMain,   BtlListInit,      ERANDE_MSG },			// �o�g��
	{ CN_PokeListTask,      ContestListInit,  ERANDE_MSG },			// �R���e�X�g
	{ CN_PokeListTask,      DefoListInit,     ERANDE_MSG },			// �������f
	{ MultiPokeSelectTask,  MultiListInit,    ERANDE_MSG },			// �}���`�o�g���Q���I��
	{ ScrollInPokeListTask, ScrollInListInit, MSG_NOT_PUT },		// �}���`�o�g���O
	{ GrowPokeSelectTask,   DefoListInit,     AZUKERU_POKE_MSG },	// ��ĉ�
	{ ManiaPokeListTask,    ManiaListInit,    ERANDE_MSG },			// �Z�����}�j�A
};

//----------------------------------------------------------
//	���C��
//----------------------------------------------------------
void PokeListMain(void)
{
	JumpActor();
	SetActor();
	MainTask();
	PaletteAnime();
}

//----------------------------------------------------------
//	VBlank���̏���
//----------------------------------------------------------
static void PokeListVblank(void)
{
	actOAMtrans();
	actDMAtrans();
	PaletteFadeTrans();
	PokeCardVBlank();		// VBlank���ŃJ�[�h��]��
}

//----------------------------------------------------------
//	�|�P�������X�g�Ăяo��
//----------------------------------------------------------
// �������p�����[�^�ݒ�
void PokeListCallParamSet( u8 id, u8 type, void * task, u8 msg_no )
{
	if( type != DEFAULT_LIST )	FightType = type;

	P_LIST->init_id	 = id;
	P_LIST->MainTask = task;
	P_LIST->init_msg = msg_no;
}

// �^�X�N�A���b�Z�[�W�w��
void PokeListCallEx( u8 id, u8 type, void * task, u8 msg_no )
{
	PokeListCallParamSet( id, type, task, msg_no );
	MainProcChange( PokeListInit );
}

// �m�[�}��
void PokeListCall( u8 id, u8 type )
{
	PokeListCallEx( id, type, InitTbl[ id ].MainTask, InitTbl[ id ].msg );
}

//----------------------------------------------------------
//	���ʏ����`�揈��
//----------------------------------------------------------
static u8 DefoListInit(void)
{
	switch( P_LIST->work[0] ){
	case 0:		// �|�P�����A�C�R���Z�b�g
		if( P_LIST->work[1] < PokeCountMine ){
			PataChrActorSet( P_LIST->task_id,
							 P_LIST->work[1],
							 &PokeParaMine[ P_LIST->work[1] ] );
			P_LIST->work[1]++;
		}else{
			P_LIST->work[1] = 0;
			P_LIST->work[0]++;
		}
		break;

	case 1:		// �A�C�e���A�C�R���̃Z���ƃp���b�g���Z�b�g
		ItmeIconCellPalSet();
		P_LIST->work[0]++;
		break;

	case 2:		// �A�C�e���A�C�R���Z�b�g
		ItemIconActorSet( P_LIST->task_id );
		P_LIST->work[0]++;
		break;

	case 3:		// �J�[�\��OBJ���Z�b�g( ��\�� )
		if( PokeCursorSet2( P_LIST->task_id, P_LIST->work[1] ) == 1 ){
			P_LIST->work[1] = 0;
			P_LIST->work[0]++;
		}else	P_LIST->work[1]++;
		break;

	case 4:		// Lv�\��
		PokeLvPutAll();
		P_LIST->work[0]++;
		break;

	case 5:		// �j�b�N�l�[���\��
		PokeNamePutAll();
		P_LIST->work[0]++;
		break;

	case 6:		// HP�\��
		PokeHpPutAll();
		P_LIST->work[0]++;
		break;

	case 7:		// �ő�HP�\��
		PokeMaxHpPutAll();
		P_LIST->work[0]++;
		break;

	case 8:		// HP�Q�[�W�\��
		PokeListGaugeSetAll();
		P_LIST->work[0]++;
		break;

	case 9:		// �J�[�h�\��
		if( PokeListBGCardInit2( P_LIST->work[1] ) == 1 ){
			P_LIST->work[1] = 0;
			P_LIST->work[0] = 0;
			return	TRUE;
		}else	P_LIST->work[1]++;
	}

	return	FALSE;
}

//----------------------------------------------------------
//	�����ݒ�
//----------------------------------------------------------
u8 PokeListInitMain(void)
{
	switch( sys.InitSeqWork ){
	case 0:
		SetVBlankFunc( NULL );
		DIV_DMACLEAR( 3, 0, VRAM, VRAM_SIZE, 16 );	// VRAM   ������
		DIV_DMACLEAR( 3, 0, OAM,  OAM_SIZE,  32 );	// OAM    ������
		DIV_DMACLEAR( 3, 0, PLTT, PLTT_SIZE, 16 );	// ��گ�  ������
		FadeData.trans_stop = 1;
		sys.InitSeqWork++;
		break;

	case 1:
		LasterInit();
		sys.InitSeqWork++;
		break;

	case 2:
		PokeListRegisterInit();
		P_LIST->work[0] = 0;
		P_LIST->work[1] = 0;
		P_LIST->work[2] = 0;
		sys.InitSeqWork++;
		break;

	case 3:
		InitActor();
		sys.InitSeqWork++;
		break;

	case 4:
		if( P_LIST->init_id	!= BATTLE_POKELIST_ID &&
			P_LIST->init_id	!= SCROLL_POKELIST_ID )
		{
			InitTask();
		}
		sys.InitSeqWork++;
		break;

	case 5:
		ObjPalManInit();
		sys.InitSeqWork++;
		break;

	case 6:
		P_LIST->task_id = AddTask( P_LIST->MainTask, 0 );
		sys.InitSeqWork++;
		break;

	case 7:
		PrintScreenInit( POKELIST2_TYPE );	// 8x8�Œ�̫�Đݒ�
		sys.InitSeqWork++;
		break;

	case 8:
		MsgControlInit( &FightMsg, POKELIST2_TYPE );	//MSG����\���̏�����
		PRT_SetSplitFontInit( &FightMsg, POKELIST_FONT_START );
		sys.InitSeqWork++;
		break;

	case 9:
		if ( PRT_SetSplitFontMain() == FALSE )	break;
		P_LIST->work[0] = 1;
		sys.InitSeqWork++;
		break;

	case 10:
		if( PokeListBgSet( P_LIST->work[0] ) == 1 ){	//BG�Z�b�g BG�֘Aڼ޽�������
			P_LIST->work[0] = 0;
			sys.InitSeqWork++;
		}else	P_LIST->work[0]++;
		break;

	case 11:
		PokeIconPaletteAllSet();
		sys.InitSeqWork++;
		break;

	case 12:	// �e���X�g���Ƃ̏���
		if( InitTbl[ P_LIST->init_id ].InitFunc() == TRUE )
			sys.InitSeqWork++;
		break;

	case 13:
		NMenuScreenInitStart( MENUPOKE_TYPE );
		sys.InitSeqWork++;
		break;

	case 14:
		if ( NMenuScreenInitMain() == FALSE )	break;
		sys.InitSeqWork++;
		break;

	case 15:
		PokeListWinFixMsgPut2( P_LIST->init_msg, 0 );
		sys.InitSeqWork++;
		break;

	case 16:
		PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
		FadeData.trans_stop = 0;
		sys.InitSeqWork++;
		break;

	case 17:
		SetVBlankFunc( PokeListVblank );
		return	TRUE;
	}

	return	FALSE;
}

//----------------------------------------------------------
// ���������[�v
//----------------------------------------------------------
static void PokeListInit(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			if( P_LIST->init_id != SCROLL_POKELIST_ID )
				PokeNowCursorMove2( P_LIST->task_id, 0 );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}


//----------------------------------------------------------
//	ڼ޽������ݒ�
//----------------------------------------------------------
static void PokeListRegisterInit(void)
{
	SetHBlankFunc(NULL);
	
	*(vu16 *)REG_DISPCNT =
				DISP_MODE_0 | DISP_OBJ_CHAR_1D_MAP | DISP_BG1_ON |
				DISP_BG0_ON | DISP_BG2_ON | DISP_BG3_ON | DISP_OBJ_ON;

	// 8x8�t�H���g
	*(vu16 *)REG_BG0CNT =
				BG_SCREEN_SIZE_0 | BG_PRIORITY_1
				| 30 << BG_SCREEN_BASE_SHIFT | 1 << BG_CHAR_BASE_SHIFT
				| BG_COLOR_16 | BG_MOS_OFF;
	// �w�i
	*(vu16 *)REG_BG1CNT =
				BG_SCREEN_SIZE_0 | BG_PRIORITY_3
				| 7 << BG_SCREEN_BASE_SHIFT | 0 << BG_CHAR_BASE_SHIFT
				| BG_COLOR_16 | BG_MOS_OFF;
	// 8x16�t�H���g
	*(vu16 *)REG_BG2CNT =
				BG_SCREEN_SIZE_0 | BG_PRIORITY_0
				| 15 << BG_SCREEN_BASE_SHIFT | 2 << BG_CHAR_BASE_SHIFT
				| BG_COLOR_16 | BG_MOS_OFF;
	// �J�[�h
	*(vu16 *)REG_BG3CNT =
				BG_SCREEN_SIZE_0 | BG_PRIORITY_2
				| 6 << BG_SCREEN_BASE_SHIFT | 0 << BG_CHAR_BASE_SHIFT
				| BG_COLOR_16 | BG_MOS_OFF;

	*(vu16 *)REG_BLDCNT = 0;

	*(vu16 *)REG_BG0HOFS = 0;
	*(vu16 *)REG_BG0VOFS = 0;
	*(vu16 *)REG_BG1HOFS = 0;
	*(vu16 *)REG_BG1VOFS = 0;
	*(vu16 *)REG_BG2HOFS = 0;
	*(vu16 *)REG_BG2VOFS = 0;
	*(vu16 *)REG_BG3HOFS = 0;
	*(vu16 *)REG_BG3VOFS = 0;
	*(vu16 *)REG_BG3VOFS = -1;
}

//----------------------------------------------------------
//	�}���`�o�g�����`�F�b�N
//----------------------------------------------------------
#define	MULTI_BTL_FLG	(FIGHT_TYPE_2vs2 | FIGHT_TYPE_TRAINER | FIGHT_TYPE_TUUSHIN | FIGHT_TYPE_4_2vs2)

u8 MultiCheck(void)
{
	if( ( FightType & MULTI_BTL_FLG ) == MULTI_BTL_FLG )
		return	TRUE;	// �}���`�o�g��
	return	FALSE;		// �}���`�o�g���ȊO
}

//----------------------------------------------------------
//	VBlank���ŃJ�[�h��]��
//----------------------------------------------------------
static void PokeCardVBlank(void)
{
	if( P_LIST->card_flg != 0 ){
		DIV_DMACOPY( 3, BG2_data, BG_VRAM+0x3000, 0x800, 16 );
		if( P_LIST->card_flg == 2 )	P_LIST->card_flg = 0;
	}
}



//=========================================================================
//	�J�[�h�֘A
//=========================================================================
static void DrawPoke00Card( s16 x, s16 y, u8 pal );
static void DelPoke00Card( s16 x, s16 y );
static void DrawPoke01Card( s16 x, s16 y, u8 flg, u8 pal );
static void DelPoke01Card( s16 x, s16 y );

//----------------------------------------------------------
//	���������\��
//----------------------------------------------------------
u8 PokeListBGCardInit2( u8 cnt )
{
	u8 * pos;

	if( FightCheck() == 0 )	pos = (u8 *)&PokeCardPos[0];			// 1 vs 1
	else{
		if( MultiCheck() == TRUE )	pos = (u8 *)&PokeCardPos[24];	// �}���`
		else						pos = (u8 *)&PokeCardPos[12];	// 2 vs 2
	}

	switch( cnt ){
	case 0:
		memset( BG2_data, 0, sizeof(u16)*1024 );
		break;

	// �J�[�h�Z�b�g
	case 1:
		DrawPoke00Card( pos[0], pos[1], CARD_B_PAL );
		PokeSelectCard( &pos[0], 0, CARD_B_PAL, 0 );
		break;
	case 2:
		if( FightCheck() == 0 ){		// 1 vs 1
			if( 1 < PokeCountMine ){
				DrawPoke01Card( pos[2], pos[3], 0, CARD_B_PAL );
				PokeSelectCard( &pos[2], 1, CARD_B_PAL, 0 );
			}else
				DrawPoke01Card( pos[2], pos[3], 1, CARD_B_PAL );
		}else{
			if( MultiCheck() == TRUE ){	// �}���`
				DrawPoke00Card( pos[2], pos[3], CARD_Y_PAL );
				PokeSelectCard( &pos[2], 1, CARD_Y_PAL, 0 );
			}else{						// 2 vs 2
				DrawPoke00Card( pos[2], pos[3], CARD_B_PAL );
				PokeSelectCard( &pos[2], 1, CARD_B_PAL, 0 );
			}
		}
		break;
	case 3:
		if( FightCheck() == 0 ){		// 1 vs 1
			if( 2 < PokeCountMine ){
				DrawPoke01Card( pos[4], pos[5], 0, CARD_B_PAL );
				PokeSelectCard( &pos[4], 2, CARD_B_PAL, 0 );
			}else
				DrawPoke01Card( pos[4], pos[5], 1, CARD_B_PAL );
		}else{
			if( MultiCheck() == TRUE ){	// �}���`
				if( PokeParaGet( &PokeParaMine[2], ID_monsno) != 0 ){
					DrawPoke01Card( pos[4], pos[5], 0, CARD_B_PAL );
					PokeSelectCard( &pos[4], 2, CARD_B_PAL, 0 );
				}else
					DrawPoke01Card( pos[4], pos[5], 1, CARD_B_PAL );
			}else{						// 2 vs 2
				if( 2 < PokeCountMine ){
					DrawPoke01Card( pos[4], pos[5], 0, CARD_B_PAL );
					PokeSelectCard( &pos[4], 2, CARD_B_PAL, 0 );
				}else
					DrawPoke01Card( pos[4], pos[5], 1, CARD_B_PAL );
			}
		}
		break;
	case 4:
		if( FightCheck() == 0 ){		// 1 vs 1
			if( 3 < PokeCountMine ){
				DrawPoke01Card( pos[6], pos[7], 0, CARD_B_PAL );
				PokeSelectCard( &pos[6], 3, CARD_B_PAL, 0 );
			}else
				DrawPoke01Card( pos[6], pos[7], 1, CARD_B_PAL );
		}else{
			if( MultiCheck() == TRUE ){	// �}���`
				if( PokeParaGet( &PokeParaMine[3], ID_monsno) != 0 ){
					DrawPoke01Card( pos[6], pos[7], 0, CARD_B_PAL );
					PokeSelectCard( &pos[6], 3, CARD_B_PAL, 0 );
				}else
					DrawPoke01Card( pos[6], pos[7], 1, CARD_B_PAL );
			}else{						// 2 vs 2
				if( 3 < PokeCountMine ){
					DrawPoke01Card( pos[6], pos[7], 0, CARD_B_PAL );
					PokeSelectCard( &pos[6], 3, CARD_B_PAL, 0 );
				}else
					DrawPoke01Card( pos[6], pos[7], 1, CARD_B_PAL );
			}
		}
		break;
	case 5:
		if( FightCheck() == 0 ){		// 1 vs 1
			if( 4 < PokeCountMine ){
				DrawPoke01Card( pos[8], pos[9], 0, CARD_B_PAL );
				PokeSelectCard( &pos[8], 4, CARD_B_PAL, 0 );
			}else
				DrawPoke01Card( pos[8], pos[9], 1, CARD_B_PAL );
		}else{
			if( MultiCheck() == TRUE ){	// �}���`
				if( PokeParaGet( &PokeParaMine[4], ID_monsno) != 0 ){
					DrawPoke01Card( pos[8], pos[9], 0, CARD_Y_PAL );
					PokeSelectCard( &pos[8], 4, CARD_Y_PAL, 0 );
				}else
					DrawPoke01Card( pos[8], pos[9], 1, CARD_Y_PAL );
			}else{						// 2 vs 2
				if( 4 < PokeCountMine ){
					DrawPoke01Card( pos[8], pos[9], 0, CARD_B_PAL );
					PokeSelectCard( &pos[8], 4, CARD_B_PAL, 0 );
				}else
					DrawPoke01Card( pos[8], pos[9], 1, CARD_B_PAL );
			}
		}
		break;
	case 6:
		if( FightCheck() == 0 ){		// 1 vs 1
			if( 5 < PokeCountMine ){
				DrawPoke01Card( pos[10], pos[11], 0, CARD_B_PAL );
				PokeSelectCard( &pos[10], 5, CARD_B_PAL, 0 );
			}else
				DrawPoke01Card( pos[10], pos[11], 1, CARD_B_PAL );
		}else{
			if( MultiCheck() == TRUE ){	// �}���`
				if( PokeParaGet( &PokeParaMine[5], ID_monsno) != 0 ){
					DrawPoke01Card( pos[10], pos[11], 0, CARD_Y_PAL );
					PokeSelectCard( &pos[10], 5, CARD_Y_PAL, 0 );
				}else
					DrawPoke01Card( pos[10], pos[11], 1, CARD_Y_PAL );
			}else{						// 2 vs 2
				if( 5 < PokeCountMine ){
					DrawPoke01Card( pos[10], pos[11], 0, CARD_B_PAL );
					PokeSelectCard( &pos[10], 5, CARD_B_PAL, 0 );
				}else
					DrawPoke01Card( pos[10], pos[11], 1, CARD_B_PAL );
			}
		}
		break;

	case 7:
		if( P_LIST->init_id == MULTI_POKELIST_ID )
			DrawBG2KETTEI( MODORU_DEF_PAL );
		DrawBG2MODORU( MODORU_DEF_PAL );
		 break;

	case 8:
		P_LIST->card_flg = 2;
		return	1;
	}

	return	0;
}

//----------------------------------------------------------
//	�}���`�Q���m�F���̏����\��
//----------------------------------------------------------
void ScrollInPokeListBGCardInit(void)
{
	PokemonParam * pp;
	u8 * pos;

	memset( BG2_data, 0, sizeof(u16)*1024 );

	pos = (u8 *)&PokeCardPos[24];

//	if( GetTeamPos() == 0 )	pp = &PokeParaMine[2];
//	else					pp = &PokeParaMine[4];

	pp = &PokeParaMine[1];

	DrawPoke00Card( 0x01, 0x01, CARD_B_PAL );

	if( PokeParaGet( &pp[0], ID_monsno) != 0 )
		DrawPoke01Card( pos[4], pos[5], 0, CARD_B_PAL );
	else	DrawPoke01Card( pos[4], pos[5], 1, CARD_B_PAL );

	if( PokeParaGet( &pp[1], ID_monsno) != 0 )
		DrawPoke01Card( pos[6], pos[7], 0, CARD_B_PAL );
	else	DrawPoke01Card( pos[6], pos[7], 1, CARD_B_PAL );

	P_LIST->card_flg = 2;
}

//----------------------------------------------------------
//	�擪�̃|�P�����\��
//----------------------------------------------------------
static void DrawPoke00Card( s16 x, s16 y, u8 pal )
{
	u8	i,j;
	u16	py;

	py = y<<5;

	for( i=0; i<7; i++ ){
		for( j=0; j<10; j++ ){
			if( x + j >= 32 )	break;
			if( x + j >= 0 )
				BG2_data[ x+py+(i<<5)+j ] = (pal<<12)+card_BG02_00[ i*10+j ];
		}
	}
}
static void DelPoke00Card( s16 x, s16 y )
{
	u8	i,j;
	u16	py;

	py = y<<5;

	for( i=0; i<7; i++ ){
		for( j=0; j<10; j++ ){
			if( x + j >= 32 )	break;
			if( x + j >= 0 )	BG2_data[ x+py+(i<<5)+j ] = 0;
		}
	}
}

//----------------------------------------------------------
//	�擪�ȊO�̃|�P�����\��
//----------------------------------------------------------
static void DrawPoke01Card( s16 x, s16 y, u8 flg, u8 pal )
{
	u8   i,j;
	u16  py;
	u8 * chr;

	if( flg == 0 )	chr = (u8 *)card_BG02_01;
	else			chr = (u8 *)card_BG02_02;

	py = y<<5;

	for( i=0; i<3; i++ ){
		for( j=0; j<18; j++ ){
			if( x + j >= 32 )	break;
			if( x + j >= 0 )
				BG2_data[ x+py+(i<<5)+j ] = (pal<<12)+chr[ i*18+j ];
		}
	}
}
static void DelPoke01Card( s16 x, s16 y )
{
	u8	i,j;
	u16	py;

	py = y<<5;

	for( i=0; i<3; i++ ){
		for( j=0; j<18; j++ ){
			if( x + j >= 32 )	break;
			if( x + j >= 0 )	BG2_data[ x+py+(i<<5)+j ] = 0;
		}
	}
}

//----------------------------------------------------------
// "�����Ă�"
//----------------------------------------------------------
void DrawBG2KETTEI( u8 pal )
{
	u8	i;

	u16	chr[] = {
		0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
		0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
	};

	u32   trans_pos;
	u16 * ptrans;

	trans_pos = SCREEN_BASE7_ADDR + ( 0x18 << 1 ) + ( 0x10 << 6 );
	ptrans    = (void *)( trans_pos );

	for( i=0; i<6; i++ ){
		ptrans[i]    = ( pal << 12 ) + chr[i];
		ptrans[32+i] = ( pal << 12 ) + chr[6+i];
	}
}

//----------------------------------------------------------
//	"���ǂ�"
//----------------------------------------------------------
void DrawBG2MODORU( u8 pal )
{
	u8	i;

	u16	chr[] = {
		0x2a,0x0b,0x0c,0x0d,0x0e,0x2f,
		0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
	};

	u32   trans_pos;
	u16 * ptrans;

	trans_pos = SCREEN_BASE7_ADDR + ( 0x18 << 1 ) + ( 0x12 << 6 );
	ptrans    = (void *)( trans_pos );

	for( i=0; i<6; i++ ){
		ptrans[i]    = ( pal << 12 ) + chr[i];
		ptrans[32+i] = ( pal << 12 ) + chr[6+i];
	}
}


//=========================================================================
//	�J�[�h�̏�ɕ�����\��
//=========================================================================
void ListCardMoji( u8 pos, u8 no )
{
	u32   trans_pos;
	u16 * ptrans;
	u8    i;
	u8    pal;
	u16   chr;

	trans_pos = CardMojiDefaultPos[FightCheck()][pos];
	ptrans    = (void *)( trans_pos );
	pal       = CARD_MOJI_PAL << 12;

	for( i=0; i<CARD_MSG_LEN; i++ ){
		chr = i+no;
		ptrans[i]    = pal | ( LIST_MOJI_TOPCHARNO + card_msg_BG02[chr] );
		ptrans[32+i] = pal | ( LIST_MOJI_TOPCHARNO + card_msg_BG02[chr+CARD_MSG_LEN] );
	}
}

void ListCardMojiAll( u8 no )
{
	u8	i;
	
	for( i=0; i<PokeCountMine; i++ )
		ListCardMoji( i, no );
}

//----------------------------------------------------------
//	���ʕ\��
//----------------------------------------------------------
void ListCardSexPut(void)
{
	u8	i;

	for( i=0; i<PokeCountMine; i++ ){
		if( PokeParaGet( &PokeParaMine[i], ID_tamago_flag ) == 0 ){
			switch( PokeSexGet( &PokeParaMine[i] ) ){
			case MONS_MALE:
				ListCardMoji( i, CARD_OSU );
				break;
			case MONS_FEMALE:
				ListCardMoji( i, CARD_MESU );
				break;
			default:
				ListCardMoji( i, CARD_HUMEI );
			}
		}else	ListCardMoji( i, CARD_HUMEI );
	}
}


//=========================================================================
//	�J�[�\���֘A
//=========================================================================
//----------------------------------------------------------
//	�J�[�\��OBJ���Z�b�g
//----------------------------------------------------------
u8 PokeCursorSet2( u8 taskno, u8 cnt )
{
	u8 actno;

	actno = AddBanishActor( DummyActMove );
	SetNowCursorActNo( taskno, actno );

	return	1;
}

//----------------------------------------------------------
//	�|�P�����I��
//	�߂�l : �����ꂽ�{�^��
//----------------------------------------------------------
u16 PokeCursorSys( u8 taskno )
{
	s8 move = 0;
	
	switch( sys.Repeat ){
	case U_KEY:
		move = -1;
		break;
	case D_KEY:
		move = 1;
		break;
	case L_KEY:
		move = -2;
		break;
	case R_KEY:
		move = 2;
		break;
	}

	// LR Mode
	if( move == 0 ){
		switch( LR_RepeatCheck() ){
		case LRM_L:
			move = -1;
			break;
		case LRM_R:
			move = 1;
		}
	}

	if( move != 0 ){
		PokeNowCursorMove2( taskno, move );
		return sys.Repeat;
	}else{
		if(	sys.Trg & A_BUTTON &&
			ActWork[ GetNowCursorActNo( taskno ) ].work[C_POS] == PL_POS_MODORU )
		{
			return	B_BUTTON;
		}
		return ( sys.Trg & ( A_BUTTON | B_BUTTON ) );
	}
}

//----------------------------------------------------------
//	�}���`�o�g���̃|�P�����I�������p
//	�߂�l : �����ꂽ�{�^��
//----------------------------------------------------------
u16 PokeCursorSys2( u8 taskno )
{
	s8 move = 0;
	
	switch( sys.Repeat ){
	case U_KEY:
		move = -1;
		break;
	case D_KEY:
		move = 1;
		break;
	case L_KEY:
		move = -2;
		break;
	case R_KEY:
		move = 2;
	}

	// LR Mode
	if( move == 0 ){
		switch( LR_RepeatCheck() ){
		case LRM_L:
			move = -1;
			break;
		case LRM_R:
			move = 1;
		}
	}

	if( sys.Trg & START_BUTTON ){
		PokeSelectPosJumpKettei( taskno );
		return	START_BUTTON;
	}

	if(move != 0){
		PokeNowCursorMove3( taskno, move );
		return sys.Repeat;
	}else{
		if(	sys.Trg & A_BUTTON &&
			ActWork[ GetNowCursorActNo( taskno ) ].work[C_POS] == PL_POS_MODORU )
		{
			return	B_BUTTON;
		}
		return ( sys.Trg & ( A_BUTTON | B_BUTTON ) );
	}
}

//----------------------------------------------------------
//	�J�[�h�̐F��ς���
//	cp = �\�����W	pal = �p���b�g�ԍ�
//----------------------------------------------------------
static void PokeCardColorChange( const u8 * cp, u8 pal )
{
	// �퓬�ɏo�Ă��Ȃ��|�P�����̃J�[�h
	if( cp[0] != 1 )	DrawPoke01Card( (s16)cp[0], (s16)cp[1], 0, pal );
	// �퓬�ɏo�Ă���|�P�����̃J�[�h
	else				DrawPoke00Card( (s16)cp[0], (s16)cp[1], pal );
}

//----------------------------------------------------------
// �J�[�\���ړ��ɂ��J�[�h�̐F�ύX
//	cp  = �\�����W
//	pos = �|�P�����̈ʒu
//	pal = �p���b�g�ԍ�
//	flg = �I������Ă��邩 ( 1 = �I�� )
//----------------------------------------------------------
static void PokeSelectCard( const u8 * cp, u8 pos, u8 pal, u8 flg )
{
	if( PokeParaGet( &PokeParaMine[pos], ID_monsno ) != 0 &&
		PokeParaGet( &PokeParaMine[pos], ID_hp ) == 0 )
	{
		pal = CARD_R_PAL;
	}

	if( flg == 1 )	pal += CARD_FLASH_PAL;

	PokeCardColorChange( cp, pal );
}


//----------------------------------------------------------
//	�J�[�\�����ړ�������
// taskno = TaskID
// move = �ړ���(-1=��ɁA+1=���ցA-2=�����̗�� +2=�E���̗��
//               0=���݂̍��W�����߂ăZ�b�g)
//----------------------------------------------------------
static void PokeCursorMove1vs1( u8 act, u8 pos, s8 move );
static void PokeCursorMove2vs2( u8 act, u8 pos, s8 move );
static void PokeCursorMoveMulti( u8 act, u8 pos, s8 move );

void PokeNowCursorMove2( u8 taskno, s8 move )
{
	u8	act;
	u8	pos;
	u8	type;

	act = GetNowCursorActNo( taskno );	// �J�[�\���̃A�N�^�[�ԍ��擾
	pos = ActWork[act].work[C_POS];		// ���݂̃J�[�\���ʒu���擾

	PokeActAnmChenge( taskno, pos, 0 );	// �I�𒆂̃|�P�����A�j����؂�ւ���

	// �}���`�o�g��
	if( MultiCheck() == TRUE ){

		// ���݂̃J�[�\���ʒu���Â�����
		if( pos == PL_POS_POKE0 || pos == PL_POS_POKE2 || pos == PL_POS_POKE3 )
			PokeSelectCard( &PokeCardPos[ (2*12) + (pos << 1) ], pos, CARD_B_PAL, 0 );
		if( pos == PL_POS_POKE1 || pos == PL_POS_POKE4 || pos == PL_POS_POKE5 )
			PokeSelectCard( &PokeCardPos[ (2*12) + (pos << 1) ], pos, CARD_Y_PAL, 0 );
		if( pos == PL_POS_MODORU )	DrawBG2MODORU( MODORU_DEF_PAL );

		PokeCursorMoveMulti( act, pos, move );	// �ړ�

		// �V�����ʒu�𖾂邭����
		if( ActWork[act].work[C_POS] == PL_POS_POKE0 ||
			ActWork[act].work[C_POS] == PL_POS_POKE2 ||
			ActWork[act].work[C_POS] == PL_POS_POKE3 )
		{
			PokeSelectCard( &PokeCardPos[ (2*12) + (ActWork[act].work[C_POS] << 1) ],
							ActWork[act].work[C_POS], CARD_B_PAL, 1 );
		}
		if( ActWork[act].work[C_POS] == PL_POS_POKE1 ||
			ActWork[act].work[C_POS] == PL_POS_POKE4 ||
			ActWork[act].work[C_POS] == PL_POS_POKE5 )
		{
			PokeSelectCard( &PokeCardPos[ (2*12) + (ActWork[act].work[C_POS] << 1) ],
							ActWork[act].work[C_POS], CARD_Y_PAL, 1 );
		}
		if( ActWork[act].work[C_POS] == PL_POS_MODORU )
			DrawBG2MODORU( MODORU_POS_PAL );

		P_LIST->card_flg = 2;

		// �J�[�\���X�V
		ActWork[act].x = PokeCursorPos[ 2 ][ ActWork[act].work[C_POS] ].x;
		ActWork[act].y = PokeCursorPos[ 2 ][ ActWork[act].work[C_POS] ].y;
	}else{
		type = FightCheck();		// �퓬�`�ԏ���

		// ���݂̃J�[�\���ʒu���Â�����
		if( pos <= PL_POS_POKE5 ){
			PokeSelectCard( &PokeCardPos[ (type*12) + (pos << 1) ], pos, CARD_B_PAL, 0 );
		}else	DrawBG2MODORU( MODORU_DEF_PAL );

		// �ړ�
		if( type == 0 )	PokeCursorMove1vs1( act, pos, move );	// 1 vs 1
		else			PokeCursorMove2vs2( act, pos, move );	// 2 vs 2

		// �V�����ʒu�𖾂邭����
		if( ActWork[act].work[C_POS] <= PL_POS_POKE5 ){
			PokeSelectCard( &PokeCardPos[ (type*12) + (ActWork[act].work[C_POS] << 1) ],
							ActWork[act].work[C_POS], CARD_B_PAL, 1 );
		}else	DrawBG2MODORU( MODORU_POS_PAL );

		P_LIST->card_flg = 2;

		// �J�[�\���X�V
		ActWork[act].x = PokeCursorPos[ type ][ ActWork[act].work[C_POS] ].x;
		ActWork[act].y = PokeCursorPos[ type ][ ActWork[act].work[C_POS] ].y;
	}

	// �I�����ꂽ�|�P�����A�j����؂�ւ���
	PokeActAnmChenge( taskno, (u8)ActWork[act].work[C_POS], 1 );

	if( pos != ActWork[act].work[C_POS] )	SePlay(SE_SELECT);
}

// 1 vs 1
static void PokeCursorMove1vs1( u8 act, u8 pos, s8 move )
{
	switch( move ){
	case 0:		// ������
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case -1:	// ��
		if( pos == PL_POS_POKE0 )
			ActWork[act].work[C_POS] = PL_POS_MODORU;
		else if( pos == PL_POS_MODORU )
			ActWork[act].work[C_POS] = PokeCountMine-1;
		else
			ActWork[act].work[C_POS] += move;
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;
	case 1:		// ��
		if( pos == PokeCountMine-1 )
			ActWork[act].work[C_POS] = PL_POS_MODORU;
		else if( pos == PL_POS_MODORU )
			ActWork[act].work[C_POS] = PL_POS_POKE0;
		else
			ActWork[act].work[C_POS] += move;
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case 2:		// �E
		if( PokeCountMine >= 2 ){
			if( pos == PL_POS_POKE0 ){
				if( ActWork[act].work[C_OLDPOS] == PL_POS_POKE0 )
					ActWork[act].work[C_OLDPOS] = PL_POS_POKE1;
				ActWork[act].work[C_POS] = ActWork[act].work[C_OLDPOS];
			}
		}
		break;

	case -2:	// ��
		if( pos >= PL_POS_POKE1 && pos <= PL_POS_POKE5 ){
			ActWork[act].work[C_POS] = PL_POS_POKE0;
			ActWork[act].work[C_OLDPOS] = pos;
		}
	}
}

// 2 vs 2
static void PokeCursorMove2vs2( u8 act, u8 pos, s8 move )
{
	switch( move ){
	case 0:		// ������
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case 1:		// ��
		if( pos == PL_POS_MODORU )
			ActWork[act].work[C_POS] = PL_POS_POKE0;
		else if( pos == PokeCountMine-1 )
			ActWork[act].work[C_POS] = PL_POS_MODORU;
		else
			ActWork[act].work[C_POS] += move;
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case -1:	// ��
		if( pos == PL_POS_POKE0 )
			ActWork[act].work[C_POS] = PL_POS_MODORU;
		else if( pos == PL_POS_MODORU )
			ActWork[act].work[C_POS] = PokeCountMine-1;
		else
			ActWork[act].work[C_POS] += move;
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case 2:		// �E
		if( pos == PL_POS_POKE0 ){
			if( PokeCountMine >= 3 ){
				if( ActWork[act].work[C_OLDPOS] == PL_POS_POKE2 ||
					ActWork[act].work[C_OLDPOS] == PL_POS_POKE3 )
				{
					ActWork[act].work[C_POS] = ActWork[act].work[C_OLDPOS];
				}else	ActWork[act].work[C_POS] = PL_POS_POKE2;
			}
		}else if( pos == 1 ){
			if( PokeCountMine >= 5 ){
				if( ActWork[act].work[C_OLDPOS] == PL_POS_POKE4 ||
					ActWork[act].work[C_OLDPOS] == PL_POS_POKE5 )
				{
					ActWork[act].work[C_POS] = ActWork[act].work[C_OLDPOS];
				}else	ActWork[act].work[C_POS] = PL_POS_POKE4;
			}
		}
		break;

	case -2:	// ��
		if( pos == PL_POS_POKE2 || pos == PL_POS_POKE3 ){
			ActWork[act].work[C_POS] = PL_POS_POKE0;
			ActWork[act].work[C_OLDPOS] = pos;
		}else if( pos == PL_POS_POKE4 || pos == PL_POS_POKE5 ){
			ActWork[act].work[C_POS] = PL_POS_POKE1;
			ActWork[act].work[C_OLDPOS] = pos;
		}
	}
}

// �}���`
static void PokeCursorMoveMulti( u8 act, u8 pos, s8 move )
{
	switch( move ){
	case 0:		// ������
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case 1:		// ��
		if( pos == PL_POS_MODORU )	ActWork[act].work[C_POS] = PL_POS_POKE0;
		else{
			while(1){
				if( pos == PL_POS_POKE5 ){
					ActWork[act].work[C_POS] = PL_POS_MODORU;
					break;
				}
				pos++;
				if( PokeParaGet( &PokeParaMine[pos], ID_monsno) != 0 ){
					ActWork[act].work[C_POS] = pos;
					break;
				}

			}
		}
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case -1:	// ��
		if( pos == PL_POS_POKE0 )	ActWork[act].work[C_POS] = PL_POS_MODORU;
		else{
			while(1){
				if( pos == PL_POS_POKE0 ){
					ActWork[act].work[C_POS] = PL_POS_MODORU;
					break;
				}
				pos--;
				if( pos != PL_POS_KETTEI ){
					if( PokeParaGet( &PokeParaMine[pos], ID_monsno) != 0 ){
						ActWork[act].work[C_POS] = pos;
						break;
					}
				}
				
			}
		}
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case 2:		// �E
		if( pos == PL_POS_POKE0 ){
			if( ActWork[act].work[C_OLDPOS] == PL_POS_POKE2 ||
				ActWork[act].work[C_OLDPOS] == PL_POS_POKE3 )
			{
				ActWork[act].work[C_POS] = ActWork[act].work[C_OLDPOS];
				break;
			}
			if( PokeParaGet( &PokeParaMine[2], ID_monsno) != 0 ){
				ActWork[act].work[C_POS] = PL_POS_POKE2;
				break;
			}
			if( PokeParaGet( &PokeParaMine[3], ID_monsno) != 0 ){
				ActWork[act].work[C_POS] = PL_POS_POKE3;
				break;
			}
		}else if( pos == PL_POS_POKE1 ){
			if( ActWork[act].work[C_OLDPOS] == PL_POS_POKE4 ||
				ActWork[act].work[C_OLDPOS] == PL_POS_POKE5 )
			{
				ActWork[act].work[C_POS] = ActWork[act].work[C_OLDPOS];
				break;
			}
			if( PokeParaGet( &PokeParaMine[4], ID_monsno ) != 0 ){
				ActWork[act].work[C_POS] = PL_POS_POKE4;
				break;
			}
			if( PokeParaGet( &PokeParaMine[5], ID_monsno ) != 0 ){
				ActWork[act].work[C_POS] = PL_POS_POKE5;
				break;
			}
		}
		break;

	case -2:	// ��
		if( pos == PL_POS_POKE2 || pos == PL_POS_POKE3 ){
			ActWork[act].work[C_POS] = PL_POS_POKE0;
			ActWork[act].work[C_OLDPOS] = pos;
		}else if( pos == PL_POS_POKE4 || pos == PL_POS_POKE5 ){
			ActWork[act].work[C_POS] = PL_POS_POKE1;
			ActWork[act].work[C_OLDPOS] = pos;
		}
	}
}

// �}���`�o�g���֎Q������|�P������I��
void PokeNowCursorMove3( u8 taskno, s8 move )
{
	u8	act;
	u8	pos;

	act  = GetNowCursorActNo( taskno );	// �J�[�\���̃A�N�^�[�ԍ��擾
	pos  = ActWork[act].work[C_POS];	// ���݂̃J�[�\���ʒu���擾

	PokeActAnmChenge( taskno, pos, 0 );	// �I�𒆂̃|�P�����̃A�j����ύX

	// ���݂̃J�[�\���ʒu���Â�����
	if( pos <= PL_POS_POKE5 )
		PokeSelectCard( &PokeCardPos[pos << 1], pos, CARD_B_PAL, 0 );
	else if( pos == PL_POS_KETTEI )
		DrawBG2KETTEI( MODORU_DEF_PAL );
	else
		DrawBG2MODORU( MODORU_DEF_PAL );

	// �ړ�
	switch( move ){
	case 0:		// ������
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case -1:	// ��
		if( pos == PL_POS_POKE0 )
			ActWork[act].work[C_POS] = PL_POS_MODORU;
		else if( pos == PL_POS_KETTEI )
			ActWork[act].work[C_POS] = PokeCountMine-1;
		else
			ActWork[act].work[C_POS] += move;
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;
	case 1:		// ��
		if( pos == PokeCountMine-1 )
			ActWork[act].work[C_POS] = PL_POS_KETTEI;
		else if( pos == PL_POS_MODORU )
			ActWork[act].work[C_POS] = PL_POS_POKE0;
		else
			ActWork[act].work[C_POS] += move;
		ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
		break;

	case 2:		// �E
		if( PokeCountMine >= 2 ){
			if( pos == PL_POS_POKE0 ){
				if( ActWork[act].work[C_OLDPOS] == PL_POS_POKE0 )
					ActWork[act].work[C_OLDPOS] = PL_POS_POKE1;
				ActWork[act].work[C_POS] = ActWork[act].work[C_OLDPOS];
			}
		}
		break;

	case -2:	// ��
		if( pos >= PL_POS_POKE1 && pos <= PL_POS_POKE5 ){
			ActWork[act].work[C_POS] = PL_POS_POKE0;
			ActWork[act].work[C_OLDPOS] = pos;
		}
	}

	// �J�[�\���X�V
	ActWork[act].x = PokeCursorPos[ 0 ][ ActWork[act].work[C_POS] ].x;
	ActWork[act].y = PokeCursorPos[ 0 ][ ActWork[act].work[C_POS] ].y;

	// �V�����J�[�\���ʒu�𖾂邭����
	if( ActWork[act].work[C_POS] <= PL_POS_POKE5 )
		PokeSelectCard( &PokeCardPos[ActWork[act].work[C_POS] << 1],
						ActWork[act].work[C_POS], CARD_B_PAL, 1 );
	else if( ActWork[act].work[C_POS] == PL_POS_KETTEI )
		DrawBG2KETTEI( MODORU_POS_PAL );
	else
		DrawBG2MODORU( MODORU_POS_PAL );

	P_LIST->card_flg = 2;

	// �I�����ꂽ�|�P�����̃A�j����ύX
	PokeActAnmChenge( taskno, ActWork[act].work[C_POS], 1 );

	if( pos != ActWork[act].work[C_POS] )	SePlay(SE_SELECT);
}

//-------------------------------------------
//	�����I�ɃJ�[�\���������
//-------------------------------------------
void PokeSelectPosJumpKettei( u8 taskno )
{
	u8	act;
	u8	pos;

	act = GetNowCursorActNo( taskno );	// �J�[�\���̃A�N�^�[�ԍ��擾
	pos = ActWork[act].work[C_POS];		// ���݂̃J�[�\���ʒu���擾

	if( pos == PL_POS_KETTEI )	return;

	PokeActAnmChenge( taskno, pos, 0 );	// �I�𒆂̃|�P�����̃A�j����ύX

	// ���݂̃J�[�\���ʒu���Â�����
	if( pos <= PL_POS_POKE5 )
		PokeSelectCard( &PokeCardPos[pos << 1], pos, CARD_B_PAL, 0 );
	else
		DrawBG2MODORU( MODORU_DEF_PAL );

	ActWork[act].work[C_OLDPOS] = PL_POS_POKE0;
	ActWork[act].work[C_POS]    = PL_POS_KETTEI;

	// �J�[�\���X�V
	ActWork[act].x = PokeCursorPos[ 0 ][ ActWork[act].work[C_POS] ].x;
	ActWork[act].y = PokeCursorPos[ 0 ][ ActWork[act].work[C_POS] ].y;

	DrawBG2KETTEI( MODORU_POS_PAL );

	P_LIST->card_flg = 2;

	SePlay(SE_SELECT);
}


//----------------------------------------------------------
//	���ݶ��وʒu�Ƌ����وʒu������ײݏ�ɂȂ����������ďC������
// ����:���ق̱������ް
//----------------------------------------------------------
static void CursorNowOldCheck(u8 actno)
{
	u8 now, old;
	
	now = ActWork[actno].work[C_POS];
	old = ActWork[actno].work[C_OLDPOS];
	
	if( FightCheck() == 0 ){
		if(now == 0 && old == 0)
			old = 1;
		else if(now > 0 && old > 0)
			old = 0;
	}else{	//2vs2
		if(now < 2 && old < 2)
			old = 2;
		else if(now > 1 && old > 1)
			old = 0;
	}
	ActWork[actno].work[C_OLDPOS] = old;
}

//----------------------------------------------------------
//	���ٍ��W���
//----------------------------------------------------------
void PokeCursorPosSet(u8 taskno, u8 pos)
{
	u8 actno;
	
	actno = GetNowCursorActNo(taskno);
	ActWork[actno].work[C_POS] = pos;
	
	CursorNowOldCheck(actno);
}

//----------------------------------------------------------
//	��޶��ق̍��W��Ă���
// taskno = ��Ă�taskno,   actno = ��޶��ق�actno
//----------------------------------------------------------
void PokeSubCursorPosSet(u8 taskno, u8 actno)
{
	u8 actno2;
	
	actno2 = GetNowCursorActNo(taskno);

	ActWork[actno].x = ActWork[actno2].x;
	ActWork[actno].y = ActWork[actno2].y;
	ActWork[actno].work[0] = ActWork[actno2].work[0];
}

//----------------------------------------------------------
//	���݂̃J�[�\����actno���擾����
//----------------------------------------------------------
static u8 GetNowCursorActNo(u8 taskno)
{
	return TaskTable[taskno].work[M_CURSOR] >> 8;
}
//----------------------------------------------------------
//	���̶݂��ق�actno������̏����ܰ��־�Ă���
//----------------------------------------------------------
static void SetNowCursorActNo(u8 taskno, u8 actno)
{
	TaskTable[taskno].work[M_CURSOR] &= 0x00ff;
	TaskTable[taskno].work[M_CURSOR] |= actno << 8;
}

//----------------------------------------------------------
//	���̶݂��وʒu���擾����
// taskno = Ҳ����ID
//----------------------------------------------------------
u8 GetNowCursorPos(u8 taskno)
{
	u8 actno;
	
	actno = GetNowCursorActNo(taskno);
	return ActWork[actno].work[C_POS];
}


//========================================================================================
//								���ёւ��i�J�[�h�𓮂����j
//========================================================================================
#define	ACT_XCNT	0
#define	ACT_XMAX	2
//#define	XMAX_PARAM	152		// 8x18
#define	XMAX_PARAM	168		// 8x18

static void PokeCardChange1X_0(u8);
static void PokeCardChange1X_1(u8);
static void PokeCardChange2X_0(u8);
static void PokeCardChange2X_1(u8);
static void PokeCardChangeParam(u8);
static void PokeIconMove(actWork *);



//------------------------------------------------------
//				�Q�̖ڂ̃|�P������I��
//------------------------------------------------------
//---------------------
//		�����ݒ�
//---------------------
void PokemonSortInit( u8 id )
{
	// ���C���^�X�N�ޔ�
	TaskTable[id].TaskAdrs = DummyTaskSeq;
	P_CARD->main_id = id;
	AddTask( PokemonSort, 0 );

	// �I���ς݃J�[�\���Z�b�g
	P_CARD->cur[0] = AddBanishActor( DummyActMove );
	PokeSubCursorPosSet( id, P_CARD->cur[0] );		//�������W�ʒu�Z�b�g

	// ���C���J�[�\��
	P_CARD->cur[1] = GetNowCursorActNo( id );

	// ���b�Z�[�W�\��
//	PokeListWinClear2();
	PokeListWinFixMsgPut2( P_LIST->work[7], 0 );

	// �J�[�h�̐F�ύX
	PokeSelectCard(
		&PokeCardPos[ ActWork[ P_CARD->cur[0] ].work[C_POS] << 1 ],
		ActWork[ P_CARD->cur[0] ].work[C_POS], CARD_G_PAL, 0 );
	P_LIST->card_flg = 2;
}

//-------------------------
//		�L�[�`�F�b�N
//-------------------------
static void PokemonSortMove( u8 id, s8 move )
{
	actWork * p0 = &ActWork[ P_CARD->cur[0] ];
	actWork * p1 = &ActWork[ P_CARD->cur[1] ];

	PokeNowCursorMove2( id, move );

	if( p0->work[C_POS] != p1->work[C_POS] )
		PokeSelectCard( &PokeCardPos[ p0->work[C_POS] << 1 ], p0->work[C_POS], CARD_G_PAL, 1 );
	else
		PokeSelectCard( &PokeCardPos[ p0->work[C_POS] << 1 ], p0->work[C_POS], CARD_G_PAL, 0 );

/*
	if( ActWork[P_CARD->cur[0]].work[C_POS] != ActWork[P_CARD->cur[1]].work[C_POS] )
		PokeSelectCard( &PokeCardPos[ ActWork[ P_CARD->cur[0] ].work[C_POS] << 1 ],
						ActWork[ P_CARD->cur[0] ].work[C_POS], CARD_G_PAL, 1 );
*/
}

void PokemonSort( u8 id )
{
	switch( sys.Repeat ){
	case U_KEY:
		PokemonSortMove( P_CARD->main_id, -1 );
		break;
	case D_KEY:
		PokemonSortMove( P_CARD->main_id, 1 );
		break;
	case L_KEY:
		PokemonSortMove( P_CARD->main_id, -2 );
		break;
	case R_KEY:
		PokemonSortMove( P_CARD->main_id, 2 );
	}

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		TaskTable[id].TaskAdrs = P_CARD->UseTask;
		TaskTable[id].TaskAdrs(id);
		return;
	}

	if( sys.Trg == B_BUTTON ){
		SePlay(SE_SELECT);
		PokemonSortEnd2(id);
	}
}

//---------------------------
//		���ёւ��I��
//---------------------------
static void PokeSortEndParam( u8 id )
{
	// �J�[�\�����A
	DelActor( &ActWork[ P_CARD->cur[0] ] );

	// ���b�Z�[�W�\��
//	PokeListWinClear2();
	PokeListWinFixMsgPut2( ERANDE_MSG, 0 );

	// �J�[�h�]���I��
	P_LIST->card_flg = 2;

	// ���C���^�X�N���A
	ChangeBackTask( P_CARD->main_id );
	DelTask( id );
}

static void PokemonSortEnd( u8 id )
{
	PokeSelectCard( &PokeCardPos[ ActWork[ P_CARD->cur[1] ].work[C_POS] << 1 ],
					ActWork[ P_CARD->cur[1] ].work[C_POS], CARD_B_PAL, 1 );
	PokeSelectCard( &PokeCardPos[ ActWork[ P_CARD->cur[0] ].work[C_POS] << 1 ],
					ActWork[ P_CARD->cur[0] ].work[C_POS], CARD_B_PAL, 0 );

	PokeSortEndParam(id);
}

void PokeSortEndCancelParam(void)
{
	u8	pos1 = (u8)ActWork[ P_CARD->cur[0] ].work[C_POS];
	u8	pos2 = (u8)ActWork[ P_CARD->cur[1] ].work[C_POS];

	if( pos1 <= PL_POS_POKE5 )
		PokeSelectCard( &PokeCardPos[ pos1 << 1 ], pos1, CARD_B_PAL, 0 );
	if( pos2 <= PL_POS_POKE5 )
		PokeSelectCard( &PokeCardPos[ pos2 << 1 ], pos2, CARD_B_PAL, 1 );

}

void PokemonSortEnd2( u8 id )
{
	PokeSortEndCancelParam();
	PokeSortEndParam(id);
}

//------------------------------------------------------
//					�J�[�h�𓮂���
//------------------------------------------------------
//---------------------
//		�����ݒ�
//---------------------
void PokeCardChangeInit( u8 id )
{
	u8	pos1,pos2;

	// �J�[�\���ʒu
	pos1 = ActWork[ P_CARD->cur[0] ].work[0];
	pos2 = ActWork[ P_CARD->cur[1] ].work[0];

	// �����Ƃ��낪�I�����ꂽ�Ƃ�
	if( pos1 == pos2 || ( pos1 == PL_POS_MODORU || pos2 == PL_POS_MODORU ) ){
		PokemonSortEnd2(id);
		return;
	}

	// ���O�ALV�AHP�A�ް�ޏ���
	StatusAreaClear( pos1 );
	StatusAreaClear( pos2 );

	// �������J�[�h���w��
	if( pos1 > pos2 ){
		P_CARD->move[0] = pos2;
		P_CARD->move[1] = pos1;
	}else{
		P_CARD->move[0] = pos1;
		P_CARD->move[1] = pos2;
	}

	// �A�C�R���̃A�N�^�[�ԍ����擾
	P_CARD->act[0] = GetPataChrActNo( P_CARD->main_id, P_CARD->move[0] );
	P_CARD->act[1] = GetPataChrActNo( P_CARD->main_id, P_CARD->move[1] );

	// �A�C�R���A�N�^�[�ݒ�
	// �擪�Ɠ���ւ�
	if( P_CARD->move[0] == 0 ){
		ActWork[ P_CARD->act[0] ].work[ ACT_XCNT ] = -8;
		ActWork[ P_CARD->act[0] ].work[ ACT_XMAX ] = -XMAX_PARAM;

		P_CARD->m_cnt[0] = 1;
		P_CARD->m_cnt[1] = 0x0c;
		TaskTable[id].TaskAdrs = PokeCardChange1X_0;
		P_LIST->card_flg = 1;
	// �擪�ȊO�Ɠ���ւ�
	}else{
		ActWork[ P_CARD->act[0] ].work[ ACT_XCNT ] = 8;
		ActWork[ P_CARD->act[0] ].work[ ACT_XMAX ] = XMAX_PARAM;

		P_CARD->m_cnt[0] = 0x0c;
		P_CARD->m_cnt[1] = 0x0c;
		TaskTable[id].TaskAdrs = PokeCardChange2X_0;
		P_LIST->card_flg = 1;
	}
	ActWork[ P_CARD->act[0] ].move = PokeIconMove;

	ActWork[ P_CARD->act[1] ].work[ ACT_XCNT ] = 8;
	ActWork[ P_CARD->act[1] ].work[ ACT_XMAX ] = XMAX_PARAM;
	ActWork[ P_CARD->act[1] ].move = PokeIconMove;
	
	ActWork[ P_CARD->act[0] ].move( &ActWork[ P_CARD->act[0] ] );
	ActWork[ P_CARD->act[1] ].move( &ActWork[ P_CARD->act[1] ] );
}

//----------------------------------
//		�A�C�R�������ւ���
//----------------------------------
static void ChangeActPos( s16 * p1, s16 * p2 )
{
	s16	copy;

	copy = *p1;
	*p1  = *p2;
	*p2  = copy;
}
static void PokeCardChangeActReverse(void)
{
	ChangeActPos( &ActWork[ P_CARD->act[0] ].x, &ActWork[ P_CARD->act[1] ].x );
	ChangeActPos( &ActWork[ P_CARD->act[0] ].y, &ActWork[ P_CARD->act[1] ].y );
	ChangeActPos( &ActWork[ P_CARD->act[0] ].dx, &ActWork[ P_CARD->act[1] ].dx );
	ChangeActPos(
		&ActWork[ P_CARD->act[0] ].work[ ACT_XCNT ],
		&ActWork[ P_CARD->act[1] ].work[ ACT_XCNT ] );

	ActWork[ P_CARD->act[0] ].move = PokeIconMove;
	ActWork[ P_CARD->act[1] ].move = PokeIconMove;
}

//------------------------------------
//		�擪�̃|�P�����Ɠ���ւ�
//------------------------------------
// �J�[�h�𓮂���
static void PokeCardChange1X_X( u8 id, u8 flg )
{
	u8	y;

	y = ( P_CARD->move[1] -1 ) * 3 + 1;

	DelPoke00Card( P_CARD->m_cnt[0], 3 );
	DelPoke01Card( P_CARD->m_cnt[1], y );

	if( flg == 0 ){
		P_CARD->m_cnt[0]--;
		P_CARD->m_cnt[1]++;
	}else{
		P_CARD->m_cnt[0]++;
		P_CARD->m_cnt[1]--;
	}

	DrawPoke00Card( P_CARD->m_cnt[0], 3, CARD_G_PAL+CARD_FLASH_PAL );
	DrawPoke01Card( P_CARD->m_cnt[1], y, 0, CARD_G_PAL+CARD_FLASH_PAL );
}

// �J�[�h�A�E�g
static void PokeCardChange1X_0( u8 id )
{
	PokeCardChange1X_X( id, 0 );

	if( P_CARD->m_cnt[0] < -12 && P_CARD->m_cnt[1] > 0x20 ){
		PokeCardChangeActReverse();
		TaskTable[id].TaskAdrs = PokeCardChange1X_1;
	}
}

// �J�[�h�C��
static void PokeCardChange1X_1( u8 id )
{
	PokeCardChange1X_X( id, 1 );

	if( P_CARD->m_cnt[0] == 1 && P_CARD->m_cnt[1] == 0x0c )
		TaskTable[id].TaskAdrs = PokeCardChangeParam;
}

//-----------------------------------------
//		�擪�ȊO�̃|�P���������ւ�
//-----------------------------------------
// �J�[�h�𓮂���
static void PokeCardChange2X_X2( u8 id, u8 flg )
{
	u8	y1,y2;

	y1 = ( P_CARD->move[0] -1 ) * 3 + 1;
	y2 = ( P_CARD->move[1] -1 ) * 3 + 1;

	DelPoke01Card( P_CARD->m_cnt[0], y1 );
	DelPoke01Card( P_CARD->m_cnt[1], y2 );

	if( flg == 0 ){
		P_CARD->m_cnt[0]++;
		P_CARD->m_cnt[1]++;
	}else{
		P_CARD->m_cnt[0]--;
		P_CARD->m_cnt[1]--;
	}

	DrawPoke01Card( P_CARD->m_cnt[0], y1, 0, CARD_G_PAL+CARD_FLASH_PAL );
	DrawPoke01Card( P_CARD->m_cnt[1], y2, 0, CARD_G_PAL+CARD_FLASH_PAL );
}

// �J�[�h�A�E�g
static void PokeCardChange2X_0( u8 id )
{
	PokeCardChange2X_X2( id, 0 );

	if( P_CARD->m_cnt[0] > 0x20 && P_CARD->m_cnt[1] > 0x20 ){
		PokeCardChangeActReverse();
		TaskTable[id].TaskAdrs = PokeCardChange2X_1;
	}
}

// �J�[�h�C��
static void PokeCardChange2X_1( u8 id )
{
	PokeCardChange2X_X2( id, 1 );

	if( P_CARD->m_cnt[0] == 0x0c && P_CARD->m_cnt[1] == 0x0c )
		TaskTable[id].TaskAdrs = PokeCardChangeParam;
}

//-------------------------------
//		�f�[�^�����ւ���
//-------------------------------
static void PokeCardChangeParam( u8 id )
{
	u8	main_act;

	SetPataChrActNo( P_CARD->main_id, P_CARD->move[0], P_CARD->act[1] );
	SetPataChrActNo( P_CARD->main_id, P_CARD->move[1], P_CARD->act[0] );

	ActWork[ P_CARD->act[0] ].x    = PataChrDefaultPos[ FightCheck() ][ P_CARD->move[1] ].x;
	ActWork[ P_CARD->act[0] ].y    = PataChrDefaultPos[ FightCheck() ][ P_CARD->move[1] ].y;
	ActWork[ P_CARD->act[0] ].dx   = 0;
	ActWork[ P_CARD->act[0] ].dy   = 0;
	ActWork[ P_CARD->act[0] ].move = PokeActNotSelectMove;
	ActWork[ P_CARD->act[1] ].x    = PataChrDefaultPos[ FightCheck() ][ P_CARD->move[0] ].x;
	ActWork[ P_CARD->act[1] ].y    = PataChrDefaultPos[ FightCheck() ][ P_CARD->move[0] ].y;
	ActWork[ P_CARD->act[1] ].dx   = 0;
	ActWork[ P_CARD->act[1] ].dy   = 0;
	ActWork[ P_CARD->act[1] ].move = PokeActNotSelectMove;

	main_act = GetPataChrActNo( P_CARD->main_id, ActWork[ P_CARD->cur[1] ].work[C_POS] );
	ActWork[ main_act ].move = PokeActSelectMove;

	SwapPokemon( &PokeParaMine[ P_CARD->move[0] ], &PokeParaMine[ P_CARD->move[1] ] );

	ListPokeLvPut( P_CARD->move[0], &PokeParaMine[ P_CARD->move[0] ] );
	ListPokeNamePut( P_CARD->move[0], &PokeParaMine[ P_CARD->move[0] ] );
	ListPokeHpPut( P_CARD->move[0], &PokeParaMine[ P_CARD->move[0] ] );
	ListPokeMaxHpPut( P_CARD->move[0], &PokeParaMine[ P_CARD->move[0] ] );

	ListPokeLvPut( P_CARD->move[1], &PokeParaMine[ P_CARD->move[1] ] );
	ListPokeNamePut( P_CARD->move[1], &PokeParaMine[ P_CARD->move[1] ] );
	ListPokeHpPut( P_CARD->move[1], &PokeParaMine[ P_CARD->move[1] ] );
	ListPokeMaxHpPut( P_CARD->move[1], &PokeParaMine[ P_CARD->move[1] ] );

	PokeListGaugeSetAll();
	PokemonSortEnd(id);
}

//-----------------------------
//		�A�C�R���𓮂���
//-----------------------------
static void PokeIconMove( actWork * act )
{
	ExtAnm( act );

	if( act->dx == act->work[ ACT_XMAX ]  ){
		act->work[ ACT_XCNT ] *= -1;
		act->work[ ACT_XMAX ] = 0;
		act->move = PokeActNotSelectMove;
		return;
	}
	act->dx += act->work[ ACT_XCNT ];
}



//=======================================================================================
//						�}���`�o�g���O�ɃJ�[�h���X�N���[��
//=======================================================================================
// �J�[�h�{�̂̃X�N���[��
void ScrollInPokeListCardMove( u8 id, u16 mons1, u16 mons2 )
{
	u8 * pos = (u8 *)&PokeCardPos[24];

	// �O��̈ʒu�̃J�[�h������
	DelPoke00Card( pos[2] + TaskTable[id].work[0],  pos[3] );
	DelPoke01Card( pos[8] + TaskTable[id].work[0],  pos[9] );
	DelPoke01Card( pos[10] + TaskTable[id].work[0], pos[11] );

	// �ʒu���X�V���ĕ\��
	TaskTable[id].work[0]--;

	DrawPoke00Card( pos[2] + TaskTable[id].work[0], pos[3], CARD_Y_PAL );

	if( mons1 != 0 )
		DrawPoke01Card( pos[8] + TaskTable[id].work[0], pos[9], 0, CARD_Y_PAL );
	else	DrawPoke01Card( pos[8] + TaskTable[id].work[0], pos[9], 1, CARD_Y_PAL );

	if( mons2 != 0 )
		DrawPoke01Card( pos[10] + TaskTable[id].work[0], pos[11], 0, CARD_Y_PAL );
	else	DrawPoke01Card( pos[10] + TaskTable[id].work[0], pos[11], 1, CARD_Y_PAL );
}

// �|�P�����A�C�R�����X�N���[��
void SetPokeIconScroll( u8 id, u16 monsno, u8 no )
{
	u8	act;

	if( monsno != 0 ){
		act = GetPataChrActNo( id, no+3 );
		ActWork[ act ].work[ ACT_XCNT ] = -8;
		ActWork[ act ].work[ ACT_XMAX ] = -8 * TaskTable[id].work[0];
		ActWork[ act ].move = PokeIconMove;
	}
}

void ScrollInActXPosPlus( u8 id, u8 pos )
{
	ActWork[ GetPataChrActNo( id, pos ) ].x += 30 * 8;
}




//=========================================================================
//	��^���b�Z�[�W�\�����[�`��
//=========================================================================
//----------------------------------------------------------
//	��ʉ��̃E�B���h�E�A��^���b�Z�[�W�\�����[�`��
//----------------------------------------------------------
void PokeListWinFixMsgPut2( u8 msg_no, u8 flg )
{
	if( msg_no == MSG_NOT_PUT )	return;

	if( flg == 0 )	NMenuWinBoxWrite( 0, 16, 23, 19 );	// �m�[�}��
	if( flg == 1 )	NMenuWinBoxWrite( 0, 16, 19, 19 );	// ���j���[
	if( flg == 2 )	NMenuWinBoxWrite( 0, 16, 22, 19 );	// ��������/��������
	NMenuMsgWrite( PokeListWinFixMsg[msg_no], 1, 17 );
}

//----------------------------------------------------------
//	��ʉ��̃E�B���h�E���A���b�Z�[�W�N���A
//----------------------------------------------------------
void PokeListWinClear2(void)
{
	NMenuBoxClear( 0, 16, 29, 19 );
}


//----------------------------------------------------------
//	���O�AHP�ALV������
// pos = ���̖ڂ̃|�P�����̽ð����������(0�ؼ��)
//----------------------------------------------------------
static void StatusAreaClear(u8 pos)
{
	u8 s_x, s_y, e_x, e_y;
	
	s_x = StAreaClearPos[FightCheck()][pos].s_x;
	s_y = StAreaClearPos[FightCheck()][pos].s_y;
	e_x = StAreaClearPos[FightCheck()][pos].e_x;
	e_y = StAreaClearPos[FightCheck()][pos].e_y;
	FontScrnAreaClear(&FightMsg, s_x, s_y, e_x, e_y);
}

//----------------------------------------------------------
//	HP������
// pos = ���̖ڂ̃|�P�����̽ð����������(0�ؼ��)
//----------------------------------------------------------
void StHpAreaClear( u8 pos )
{
	u8 s_x, s_y, e_x, e_y;
	
	s_x = StHPAreaClearPos[FightCheck()][pos].s_x;
	s_y = StHPAreaClearPos[FightCheck()][pos].s_y;
	e_x = StHPAreaClearPos[FightCheck()][pos].e_x;
	e_y = StHPAreaClearPos[FightCheck()][pos].e_y;
	FontScrnAreaClear(&FightMsg, s_x, s_y, e_x, e_y);
}



//=======================================================================================
//=======================================================================================
//=======================================================================================
//=======================================================================================



//----------------------------------------------------------
//	����: 0=�ꊇ�S�]��	
//		  1�`=��Sync�ɕ������Ȃ���]��(1���߂�܂Ő�����傫�����Ė�Sync�ĂтÂ���)
//----------------------------------------------------------
u8 PokeListBgSet( u8 trans_type )
{
	u16 white_pal = 0x7fff;
	u8 ret = 0;
	
	// else if�������0�őS���Z�b�g����Ȃ��Ȃ��Ă��܂��̂őS�����ʂ�if�ł��
	if(trans_type == 1 || trans_type == 0)
		DecordVram((void*)p_list_pch_ADRS, (void*)CHAR_BASE0_ADDR);
	if(trans_type == 2 || trans_type == 0)
		DecordVram((void*)p_list03_psc_ADRS, (void*)SCREEN_BASE7_ADDR);
	if(trans_type == 3 || trans_type == 0)
		DecordPaletteWork((void*)p_list_pcl_ADRS, PA_BG0+PA_COL0, 0x160);
	if(trans_type == 4 || trans_type == 0)
		PaletteWorkSet(&white_pal, 0, 2);
	if(trans_type == 5 || trans_type == 0)
		DecordVram((void*)p_listg2_pch_ADRS, LISTGAUGE_TRANS_ADDR);
	if(trans_type == 6 || trans_type == 0)
		DecordVram((void*)p_list_m_pch_ADRS, LIST_MOJI_TRANS_ADDR);
	if(trans_type == 7 || trans_type == 0)
		DecordVram((void*)st_cnd_pch_ADRS, LIST_CONDITION_TRANS_ADDR);
	if(trans_type == 8 || trans_type == 0){
		DecordPaletteWork((void*)st_cnd_pcl_ADRS, PA_BG11+PA_COL0, 0x20);
		ret = 1;
	}
	return ret;
}
//----------------------------------------------------------
//	��������ׂ�AddActor
// pos = ���Ԗڂɕ\�����邩
//----------------------------------------------------------
static void PokeIconAnmSetParam( u8 act, u16 hp, u16 mhp )
{
	u8	color;
	u8	anmno;
	
	color = GetGaugeDottoColor( hp, mhp );

	if( hp == mhp )		anmno = 0;		// HP Full
	else if(color == 3)	anmno = 1;		// green
	else if(color == 2)	anmno = 2;		// yellow
	else if(color == 1)	anmno = 3;		// red
	else				anmno = 4;		// dead

	ExtAnmChg( &ActWork[act], anmno );
}

static void PokeIconAnmSet( u8 act, PokemonParam * pp )
{
	PokeIconAnmSetParam(
		act,
		PokeParaGet( pp, ID_hp ),
		PokeParaGet( pp, ID_hpmax ) );
}

void PataChrActorSet2( u8 taskno, u8 pos, u8 type, PokemonParam * pp )
{
	u8	xpos, ypos;
	u8	actno;

	xpos = PataChrDefaultPos[ type ][ pos ].x;
	ypos = PataChrDefaultPos[ type ][ pos ].y;

	actno = PokeIconSet2(
			PokeParaGet( pp, ID_monsno_egg ),
			PokeIconDmmyMove, xpos, ypos, POKEPATA_PRI,
			PokeParaGet( pp, ID_personal_rnd ) );
	SetPataChrActNo(taskno, pos, actno);

	PokeIconAnmSet( actno, pp );
}

void PataChrActorSet(u8 taskno, u8 pos, PokemonParam *pp)
{
	if( PokeParaGet(pp, ID_monsno) == 0 )	return;

	if( MultiCheck() == TRUE )	PataChrActorSet2( taskno, pos, 2, pp );
	else						PataChrActorSet2( taskno, pos, FightCheck(), pp );
}

void PataChrActorSetAll(u8 taskno)
{
	u8 i;
	
	for( i=0; i<PokeCountMine; i++ )
		PataChrActorSet( taskno, i, &PokeParaMine[i] );
}

// �}���`�o�g���Q���m�F�p
void PataChrActorSetParam( u8 taskno, u8 pos, u8 type, MultiPokeListData * mpld )
{
	u8	xpos, ypos;
	u8	actno;

	xpos = PataChrDefaultPos[ type ][ pos ].x;
	ypos = PataChrDefaultPos[ type ][ pos ].y;

	actno = PokeIconSet2(
				mpld->monsno, PokeIconDmmyMove,
				xpos, ypos, POKEPATA_PRI, mpld->personal_rnd );
	SetPataChrActNo( taskno, pos, actno );

	PokeIconAnmSetParam( actno, mpld->hp, mpld->hpmax );
}






//---------------------------------------------
//		�I�𒆂̃|�P�����A�C�R���̃A�j��
//---------------------------------------------
static void PokeActSelectMove( actWork * act )
{
	u8	anm;

	anm = ExtAnm( act );

	if( anm != 0 ){
		if( anm & 1 )	act->dy = -3;
		else			act->dy = 1;
	}
}

static void PokeActNotSelectMove( actWork * act )
{
	ExtAnm( act );
}

//-------------------------------------------
//		�|�P�����A�C�R���̃A�j����ύX
//-------------------------------------------
static void PokeActAnmChenge( u8 task_id, u8 pos, u8 flg )
{
	u8	act;

	if( pos >= TEMOTI_POKEMAX )	return;

	act = GetPataChrActNo( task_id, pos );
	ActWork[ act ].dy      = 0;
	ActWork[ act ].work[0] = 0;
	if( flg == 0 )	ActWork[ act ].move = PokeActNotSelectMove;
	else			ActWork[ act ].move = PokeActSelectMove;

}

//----------------------------------------------------------
//	���ѱ��݂�CellSet&PalSet
//----------------------------------------------------------
void ItmeIconCellPalSet(void)
{
	CellSet((CellData*)&ItemIconCellTbl);
	ObjPalSet((PalData*)&ItemIconPalTbl);
}



// =================================================================
// ItemIconSeq2
// �T�v  : �ʐM���������ʗp�A�C�e���A�C�R���V�[�P���X
// ����  : Xreg
// �߂�l: none
// =================================================================
static void ItemIconSeq2(actWork *Xreg)
{
	u8 pataact;
	
	pataact = Xreg->work[7];
	if(ActWork[pataact].banish){		//�|�P�������B����Ă���A�C�e���A�C�R�����B��
		Xreg->banish = 1;
	}else{
		Xreg->banish = 0;
		Xreg->x = ActWork[pataact].x + ActWork[pataact].dx;
		Xreg->y = ActWork[pataact].y + ActWork[pataact].dy;
	}
}

// =================================================================
// KoukanPokeItemIconActorSetSub
// �T�v  : �A�C�e���A�C�R���o�^�i�ʐM���������ʗp�j
// ����  : actno
//       : item
// �߂�l: none
// =================================================================
static void KoukanPokeItemIconActorSetSub(u8 pokeactno, u8 item)
{
	u8 actno,pri;
	
	pri = ActWork[pokeactno].pri;
	actno = AddActor(&ItemIconHeader, 250,170, pri-1);		//�|�P�����A�C�R��������O��
	ActWork[actno].dx = ITEMICON_OFFSET_X;
	ActWork[actno].dy = ITEMICON_OFFSET_Y;
	ActWork[actno].move = ItemIconSeq2;						//���������ʗp�̃V�[�P���X
	ActWork[actno].work[7] = pokeactno;
	ActAnmChg(&ActWork[actno],item);
	ActWork[actno].move(&ActWork[actno]);
	
}


// =================================================================
// ItemIconActorSetSub
// �T�v  : �|�P������̂ɂ��A�C�e���A�C�R����o�^����
// ����  : id		:	
//       : pataact	:
// �߂�l: none
// =================================================================
void KoukanPokeItemIconActorSet(u8 pokenum[2], u8 iconlist[2][6], u8 side)
{
	u16 item,i;

	switch(side){												
		case 0:														//�����p
			for( i=0; i<pokenum[0]; i++ ){
				if((item=PokeParaGet(&PokeParaMine[i], ID_item)) != NON_ITEM){
					KoukanPokeItemIconActorSetSub(iconlist[0][i],MailItemNoCheck(item));		
				}
			}
			break;
		case 1:														//����p
			for( i=0; i<pokenum[1]; i++){
				if((item=PokeParaGet(&PokeParaEnemy[i], ID_item)) != NON_ITEM){
					KoukanPokeItemIconActorSetSub(iconlist[1][i],MailItemNoCheck(item));
				}
			}
			break;
	}
}



//----------------------------------------------------------
//	���ѱ��݂�AddActor	����������ׂ�AddActor���Ă������ƁI
//----------------------------------------------------------
void ItemIconActorSet( u8 id )
{
	u8 i;
	u8 actno;
	u8 pataact;
	
	for( i=0; i<TEMOTI_POKEMAX; i++ ){
		if( PokeParaGet( &PokeParaMine[i], ID_monsno ) != 0 ){
			pataact = GetPataChrActNo(id, i);
			actno = AddActor(&ItemIconHeader, 250,170, ITEMICON_PRI);
			ActWork[actno].dx = ITEMICON_OFFSET_X;
			ActWork[actno].dy = ITEMICON_OFFSET_Y;
			ActWork[actno].work[7] = pataact;
			ActWork[pataact].work[7] = actno;
			ItemIconStatusSet(id, i);
			ActWork[actno].move(&ActWork[actno]);
		}
	}
}

void ItemIconActSetParam( u8 id, u8 pos, u16 item )
{
	u8	item_act;
	u8	poke_act;

	poke_act = GetPataChrActNo( id, pos );
	item_act = AddActor( &ItemIconHeader, 250, 170, ITEMICON_PRI );

	ActWork[item_act].dx = ITEMICON_OFFSET_X;
	ActWork[item_act].dy = ITEMICON_OFFSET_Y;
	ActWork[item_act].work[7] = poke_act;
	ActWork[poke_act].work[7] = item_act;

	if( item == 0 )	ActWork[item_act].banish = 1;
	else if( MailItemNoCheck( item ) )
	{
		ActAnmChg( &ActWork[item_act], 1 );
		ActWork[item_act].banish = 0;
	}else{
		ActAnmChg( &ActWork[item_act], 0 );
		ActWork[item_act].banish = 0;
	}

	ActWork[item_act].move( &ActWork[item_act] );
}

//----------------------------------------------------------
//	���ѱ��ݱ������ݽ
//----------------------------------------------------------
static void ItemIconSeq(actWork *Xreg)
{
	u8 pataact;
	
	pataact = Xreg->work[7];
	Xreg->x = ActWork[pataact].x + ActWork[pataact].dx;
//	Xreg->y = ActWork[pataact].y + ActWork[pataact].dy;
	Xreg->y = ActWork[pataact].y;
}


//----------------------------------------------------------
//	3���̐����܂ŕ\��
// yose = 0:����  1:�E��
//----------------------------------------------------------
#define NUM_BUF_LEN		4	// 3�� + EOM_

static void PokeListNumberPut( u16 number, u8 TransCharNo, u8 yose, u8 xpos, u8 ypos )
{
	u8	i;
	u8	MsgBuf[NUM_BUF_LEN];
	u8	put_num;

	u32   trans_pos;
	u16 * ptrans;

	if( yose == 0 )	PM_NumMsgSet( MsgBuf, number, NUM_MODE_LEFT, 3 );
	else			PM_NumMsgSet( MsgBuf, number, NUM_MODE_SPACE, 3 );

	trans_pos = SCREEN_BASE30_ADDR + ( xpos << 1 ) + ( (ypos+1) << 6 );
	ptrans    = (void *)( trans_pos );

	for( i=0; MsgBuf[i] != EOM_; i++ ){

		if( MsgBuf[i] == spc_ )	put_num = spc_8x8;
		else					put_num = n0_8x8 + MsgBuf[i] - n0_;

		ptrans[i] = 0xf000 + POKELIST_FONT_START + put_num;
	}
}













//=========================================================================
//	�c�[����
//=========================================================================
//----------------------------------------------------------
//	�����ׂ�actno�����o��
// get_pos: �擪����get_pos�Ԗڂ������ׂ�actno�����o��(0�ؼ��)
//----------------------------------------------------------
static u8 GetPataChrActNo(u8 taskno, u8 get_pos)
{
	switch(get_pos)
	{
		case 0:
			return TaskTable[taskno].work[M_PATA_ACTNO0_1] >> 8;
		case 1:
			return TaskTable[taskno].work[M_PATA_ACTNO0_1] & 0xff;
		case 2:
			return TaskTable[taskno].work[M_PATA_ACTNO2_3] >> 8;
		case 3:
			return TaskTable[taskno].work[M_PATA_ACTNO2_3] & 0xff;
		case 4:
			return TaskTable[taskno].work[M_PATA_ACTNO4_5] >> 8;
		case 5:
			return TaskTable[taskno].work[M_PATA_ACTNO4_5] & 0xff;
	}
	return TaskTable[taskno].work[0] >> 8;	//error �ꉞ0�Ԗڂ̂�Ԃ�
}
//----------------------------------------------------------
//	�����ׂ�actno�����ܰ��־�Ă���
// set_pos = �擪���牽�̖ڂ������ׂ�(0�ؼ��)
//----------------------------------------------------------
static void SetPataChrActNo(u8 taskno, u8 set_pos, u8 actno)
{
	switch(set_pos)
	{
		case 0:
			TaskTable[taskno].work[M_PATA_ACTNO0_1] &= 0x00ff;
			TaskTable[taskno].work[M_PATA_ACTNO0_1] |= actno << 8;
			break;
		case 1:
			TaskTable[taskno].work[M_PATA_ACTNO0_1] &= 0xff00;
			TaskTable[taskno].work[M_PATA_ACTNO0_1] |= actno;
			break;
		case 2:
			TaskTable[taskno].work[M_PATA_ACTNO2_3] &= 0x00ff;
			TaskTable[taskno].work[M_PATA_ACTNO2_3] |= actno << 8;
			break;
		case 3:
			TaskTable[taskno].work[M_PATA_ACTNO2_3] &= 0xff00;
			TaskTable[taskno].work[M_PATA_ACTNO2_3] |= actno;
			break;
		case 4:
			TaskTable[taskno].work[M_PATA_ACTNO4_5] &= 0x00ff;
			TaskTable[taskno].work[M_PATA_ACTNO4_5] |= actno << 8;
			break;
		case 5:
			TaskTable[taskno].work[M_PATA_ACTNO4_5] &= 0xff00;
			TaskTable[taskno].work[M_PATA_ACTNO4_5] |= actno;
			break;
	}
}
//----------------------------------------------------------
//	���ѱ��݂�actno�����o��
// taskno : Ҳ������ID
// get_pos: �擪����get_pos�Ԗڂ������ׂ�actno�����o��(0�ؼ��)
//----------------------------------------------------------
static u8 GetItemIconActNo(u8 taskno, u8 get_pos)
{
	u8 actno;
	
	actno = GetPataChrActNo(taskno, get_pos);
	return ActWork[actno].work[7];
}
//----------------------------------------------------------
//	�߹�݂̎莝�����т𒲂ׂı��݂̏�Ԃ�ς���
// ����: id = Ҳ������ID
//       pokepos = ���Ԗڂ��߹�݂�(0�ؼ��)
// 		 status = ITEM_ICON(����)  MAIL_ICON(Ұ�)  BANISH_ICON(��Ư������)
//----------------------------------------------------------
void ItemIconStatusSet( u8 id, u8 pokepos )
{
	u8	actno;
	
	actno = GetItemIconActNo(id, pokepos);

	if(PokeParaGet(&PokeParaMine[pokepos], ID_item) == NON_ITEM)
		ActWork[actno].banish = 1;
	else if(PokeMailCheck(&PokeParaMine[pokepos]))
	{
		ActAnmChg(&ActWork[actno], 1);
		ActWork[actno].banish = 0;
	}
	else
	{
		ActAnmChg(&ActWork[actno], 0);
		ActWork[actno].banish = 0;
	}
}


//=========================================================================
//	�j�b�N�l�[���\��
//	pos = �ʒu	type = ���X�g�^�C�v
//=========================================================================
//----------------------------------------------------------
//	������w��
//	naem = ������̃|�C���^
//----------------------------------------------------------
void ListPokeNamePutParam( u8 pos, u8 type, u8 * name )
{
	u8 xpos, ypos;

	xpos = NameDefaultPos[ type ][ pos ].x;
	ypos = NameDefaultPos[ type ][ pos ].y;

	MsgPutSet(
		&FightMsg,					// TALK_WIN�\���̱̂��ڽ
		name,						// �\������ү���ނ̐擪���ڽ
		POKELIST_FONT_START,		// 8x8���ތŒ�̫��Ӱ�ނɕύX�̂���
		xpos, ypos );				// �\�����W

	MsgPut( &FightMsg );
}
//----------------------------------------------------------
//	�|�P�����p�����[�^�w��
//	pp = �|�P�����p�����[�^�̃|�C���^
//----------------------------------------------------------
void ListPokeNamePut2( u8 pos, u8 type, PokemonParam * pp )
{
	u8 name[ MONS_NAME_SIZE + EOM_SIZE ];

	PokeNickNameSet( pp, name );
	ListPokeNamePutParam( pos, type, name );
}

void ListPokeNamePut( u8 pos, PokemonParam * pp )
{
	if( PokeParaGet(pp, ID_monsno) == 0 )	return;

	if( MultiCheck() == TRUE )	ListPokeNamePut2( pos, 2, pp );
	else						ListPokeNamePut2( pos, FightCheck(), pp );
}
//----------------------------------------------------------
//	�莝���|�P�������ׂẴj�b�N�l�[����\��
//----------------------------------------------------------
void PokeNamePutAll(void)
{
	u8 i;
	
	for( i=0; i<TEMOTI_POKEMAX; i++ )
		ListPokeNamePut( i, &PokeParaMine[i] );
}

//----------------------------------------------------------
//	�|�P�����̃j�b�N�l�[���Z�b�g
//	�߂�l�@EOM_�A�h���X
//----------------------------------------------------------
u8 * PokeNickNameSet( PokemonParam * pp, u8 * str )
{
	PokeParaGet( pp, ID_nickname, str );
	return	NickNameCutOff( str );
}


//----------------------------------------------------------
//		��Ԉُ�\��
//----------------------------------------------------------
void PokeConditionPutCardMoji( u8 pos, u8 type, u8 condition )
{
	u32   trans;
	u16 * ptrans;
	u8    i;
	u8	  xpos,ypos;

	xpos = LvDefaultPos[ type ][ pos ].x-1;
	ypos = LvDefaultPos[ type ][ pos ].y+1;

	trans = SCREEN_BASE30_ADDR + ( xpos + ypos * 32 ) * 2;

	ptrans = (void *)( trans );
	condition *= 4;

	for( i=0; i<4; i++ )
		ptrans[i] = ( CONDITION_PAL << 12 ) | ( LIST_CONDITION_TOPCHARNO + condition + i );
}

//----------------------------------------------------------
//		��Ԉُ�\��������
//----------------------------------------------------------
static void PokeConditionDelCardMoji( u8 pos )
{
	u32   trans;
	u16 * ptrans;
	u8    i;
	u8	  xpos,ypos;
	u8    type;

	if( MultiCheck() == TRUE )	type = 2;
	else						type = FightCheck();

	xpos = LvDefaultPos[ type ][ pos ].x-1;
	ypos = LvDefaultPos[ type ][ pos ].y+1;

	trans = SCREEN_BASE30_ADDR + ( xpos + ypos * 32 ) * 2;

	ptrans = (void *)(trans);

	for( i=0; i<4; i++ )
		ptrans[i] = 0;
}

//=========================================================================
//	���x���\��
//	pos = �ʒu	type = ���X�g�^�C�v
//=========================================================================
void ListLvMojiPut( u8 chr, u8 x, u8 y )
{
	u32   trans;
	u16 * ptrans;

	trans  = SCREEN_BASE30_ADDR + ( x + y * 32 ) * 2;
	ptrans = (void *)( trans );

	ptrans[0] = LIST_MOJI_TOPCHARNO + chr;
}
//----------------------------------------------------------
//	���l�w��
//	lv = ���x��
//----------------------------------------------------------
void ListPokeLvPutParam( u8 pos, u8 type, u8 lv )
{
	u8 xpos, ypos;

	xpos = LvDefaultPos[ type ][ pos ].x;
	ypos = LvDefaultPos[ type ][ pos ].y;
	
	ListLvMojiPut( LV_MOJI_CHAR_NO, xpos-1, ypos+1 );

	PokeListNumberPut( lv, P_LV_TRANS_CHARNO + pos * P_KETA, 0, xpos, ypos );
}
//----------------------------------------------------------
//	�|�P�����p�����[�^�w��
//	pp = �|�P�����p�����[�^�̃|�C���^
//----------------------------------------------------------
void ListPokeLvPut2( u8 pos, u8 type, PokemonParam * pp )
{
	ListPokeLvPutParam( pos, type, (u8)PokeParaGet( pp, ID_level ) );
}

void ListPokeLvPut( u8 pos, PokemonParam * pp )
{
	u8	con, type;

	if( PokeParaGet( pp, ID_monsno ) == 0 )			return;
	if( PokeParaGet( pp, ID_tamago_flag ) != 0 )	return;

	con = GetPokeCondition( pp );

	if( MultiCheck() == TRUE )	type = 2;
	else						type = FightCheck();

	if( con != ST_NORMAL && con != ST_POKERUS )
		PokeConditionPutCardMoji( pos, type, con-1 );
	else
		ListPokeLvPut2( pos, type, pp );

	PokeListSexPut( pos, type, pp );
}
//----------------------------------------------------------
//	�莝���|�P�������ׂĂ�Lv��\��
//----------------------------------------------------------
void PokeLvPutAll(void)
{
	u8 i;

	for( i=0; i<TEMOTI_POKEMAX; i++ )
		ListPokeLvPut( i, &PokeParaMine[i] );
}

//=========================================================================
//	���ʕ\��
//=========================================================================
void PokeListSexPutParam( u16 mons, u8 sex, u8 type, u8 pos, u8 * name )
{
	u8	x, y;

	if( mons == MONSNO_NIDORAN_M || mons == MONSNO_NIDORAN_F ){
		if( PM_strcmp( name, PokeNameData[mons] ) == 0 )
			return;
	}

	x = LvDefaultPos[ type ][ pos ].x+3;
	y = LvDefaultPos[ type ][ pos ].y+1;

	switch( sex ){
	case MONS_MALE:
		ListLvMojiPut( OSU_MOJI_CHAR_NO, x, y );
		break;
	case MONS_FEMALE:
		ListLvMojiPut( MESU_MOJI_CHAR_NO, x, y );
	}
}

void PokeListSexPut( u8 pos, u8 type, PokemonParam * pp )
{
	PokeNickNameSet( pp, StrTempBuffer0 );

	PokeListSexPutParam(
		PokeParaGet( pp, ID_monsno_egg ), PokeSexGet( pp ),
		type, pos, StrTempBuffer0 );
}

//=========================================================================
//	HP�\��
//	pos = �ʒu	type = ���X�g�^�C�v
//=========================================================================
//----------------------------------------------------------
//	���l�w��
//	hp = HP
//----------------------------------------------------------
void ListPokeHpPutParam( u8 pos, u8 type, u16 hp )
{
	u8 xpos, ypos;
	
	xpos = HpDefaultPos[ type ][ pos ].x;
	ypos = HpDefaultPos[ type ][ pos ].y;

	PokeListNumberPut( hp, P_HP_TRANS_CHARNO + pos * P_KETA, 1, xpos, ypos);
}
//----------------------------------------------------------
//	�|�P�����p�����[�^�w��
//	pp = �|�P�����p�����[�^�̃|�C���^
//----------------------------------------------------------
void ListPokeHpPut2( u8 pos, u8 type, PokemonParam *pp )
{
	ListPokeHpPutParam( pos, type, PokeParaGet(pp, ID_hp) );
}

void ListPokeHpPut( u8 pos, PokemonParam * pp )
{
	if( PokeParaGet( pp, ID_monsno ) == 0 )			return;
	if( PokeParaGet( pp, ID_tamago_flag ) != 0 )	return;

	if( MultiCheck() == TRUE )	ListPokeHpPut2( pos, 2, pp );
	else						ListPokeHpPut2( pos, FightCheck(), pp );
}
//----------------------------------------------------------
//	�莝���|�P�������ׂĂ�HP��\��
//----------------------------------------------------------
void PokeHpPutAll(void)
{
	u8 i;

	for( i=0; i<TEMOTI_POKEMAX; i++ )
		ListPokeHpPut( i, &PokeParaMine[i] );
}


//=========================================================================
//	�ő�HP�\��
//	pos = �ʒu	type = ���X�g�^�C�v
//=========================================================================
//----------------------------------------------------------
//	���l�w��
//	mhp = �ő�HP
//----------------------------------------------------------
void ListPokeMaxHpPutParam( u8 pos, u8 type, u16 mhp )
{
	u8 xpos, ypos;
	
	xpos = MaxHpDefaultPos[ type ][ pos ].x;
	ypos = MaxHpDefaultPos[ type ][ pos ].y;

	MsgPutSet( &FightMsg, str_surassyu, POKELIST_FONT_START, xpos-1, ypos );
	MsgPut( &FightMsg );

	PokeListNumberPut(
		mhp, P_HPMAX_TRANS_CHARNO + pos * P_KETA, 1, xpos, ypos );
}
//----------------------------------------------------------
//	�|�P�����p�����[�^�w��
//	pp = �|�P�����p�����[�^�̃|�C���^
//----------------------------------------------------------
void ListPokeMaxHpPut2( u8 pos, u8 type, PokemonParam * pp )
{
	ListPokeMaxHpPutParam( pos, type, PokeParaGet( pp, ID_hpmax ) );
}

void ListPokeMaxHpPut(u8 pos, PokemonParam *pp)
{
	if( PokeParaGet( pp, ID_monsno ) == 0 )			return;
	if( PokeParaGet( pp, ID_tamago_flag ) != 0 )	return;

	if( MultiCheck() == TRUE )	ListPokeMaxHpPut2( pos, 2, pp );
	else						ListPokeMaxHpPut2( pos, FightCheck(), pp );
}
//----------------------------------------------------------
//	�莝���|�P�������ׂĂ̍ő�HP��\��
//----------------------------------------------------------
void PokeMaxHpPutAll(void)
{
	u8 i;
	
	for( i=0; i<TEMOTI_POKEMAX; i++ )
		ListPokeMaxHpPut( i, &PokeParaMine[i] );
}


//=========================================================================
//	HP�Q�[�W��\��
//	pos = �ʒu	type = ���X�g�^�C�v
//=========================================================================
//----------------------------------------------------------
//	���l�w��
//	hp = HP		mhp = MAX HP
//----------------------------------------------------------
void ListGaugeScreenSetParam( u8 pos, u8 type, u16 hp, u16 mhp )
{
	GaugeDataBG gdata;
	u32 trans_pos;
	s32 hp_work;
	u16 *ptrans;
	u8	color;

	hp_work = HP_WORK_INIT_VALUE;
	gdata.MaxHP = mhp;
	gdata.NowHP = hp;
	gdata.beHP = 0;
	color = GetGaugeDottoColor( gdata.NowHP, gdata.MaxHP );
	if( color >= 3 )	gdata.color = HP_GAUGE_G_PAL;
	if( color == 2 )	gdata.color = HP_GAUGE_Y_PAL;
	if( color <= 1 )	gdata.color = HP_GAUGE_R_PAL;
	gdata.CharNo = LISTGAUGE_TOPCHRNO;
	trans_pos = GaugeDefaultPos[ type ][ pos ];
	GaugeBG(&gdata, &hp_work, (void*)trans_pos, 0);	

	//-- �ް�ނ̊O�g���Z�b�g --//
	ptrans = (void*)(trans_pos - 2 * 2);
	ptrans[0] = ( HP_GAUGE_HP_PAL << 12 ) | ( LISTGAUGE_TOPCHRNO + 9 );
	ptrans[1] = ( HP_GAUGE_HP_PAL << 12 ) | ( LISTGAUGE_TOPCHRNO + 10 );
	ptrans[8] = ( HP_GAUGE_HP_PAL << 12 ) | ( LISTGAUGE_TOPCHRNO + 11 );
}
//----------------------------------------------------------
//	�|�P�����p�����[�^�w��
//	pp = �|�P�����p�����[�^�̃|�C���^
//----------------------------------------------------------
void ListGaugeScreenSet2( u8 pos, u8 type, PokemonParam * pp )
{
	ListGaugeScreenSetParam(
		pos, type, PokeParaGet( pp, ID_hp ), PokeParaGet( pp, ID_hpmax ) );
}

void ListGaugeScreenSet( u8 pos, PokemonParam * pp )
{
	if( PokeParaGet( pp, ID_monsno ) == 0 )			return;
	if( PokeParaGet( pp, ID_tamago_flag ) != 0 )	return;

	if( MultiCheck() == TRUE )	ListGaugeScreenSet2( pos, 2, pp );
	else						ListGaugeScreenSet2( pos, FightCheck(), pp );
}
//----------------------------------------------------------
//	�莝���|�P�������ׂĂ�HP�Q�[�W�\��
//----------------------------------------------------------
void PokeListGaugeSetAll(void)
{
	u8 i;
	
	for( i=0; i<TEMOTI_POKEMAX; i++ )
		ListGaugeScreenSet(i, &PokeParaMine[i]);
}


//=========================================================================
//	�莝���߹�݂̃f�[�^����ꂩ����
//=========================================================================
void SwapPokemon( PokemonParam * pp1, PokemonParam * pp2 )
{
	PokemonParam temp;
	
	memcpy( &temp, pp1,   sizeof(PokemonParam) );
	memcpy( pp1,   pp2,   sizeof(PokemonParam) );
	memcpy( pp2,   &temp, sizeof(PokemonParam) );
}


//=========================================================================
//	���j���[�c�[��
//=========================================================================
//----------------------------------------------------------
//	�ƭ������߂ɂ���ĳ���޳�̍��W(�n�_X,Y)���擾
//----------------------------------------------------------
static void GetMenuWinPos(u8 menu_type, u8 *xpos, u8 *ypos, const MenuListData *pmd)
{
	u8 x_len, y_len;
	
	y_len = pmd[menu_type].menu_max * 2 + 2;
	x_len = pmd[menu_type].x_len + 1;
	*xpos = MENU_END_X - x_len;
	*ypos = MENU_END_Y - y_len;
}

//----------------------------------------------------------
//	���j���[����޳�`��
//----------------------------------------------------------
void PokeListMenuWindowPut( u8 menu_type, const MenuListData * pmd,
							const MENU_ITEM * mi, u8 init_pos )
{
	u8 xpos, ypos;
	
	GetMenuWinPos(menu_type, &xpos, &ypos, pmd);
	MakeMenuWindow(xpos, ypos, pmd[menu_type].x_len,
		 pmd[menu_type].menu_max,
			mi, pmd[menu_type].menu_list);

	//���ُ����ݒ�
	NMenuVCursorInit( xpos+1, ypos+1, pmd[menu_type].menu_max, init_pos );
}

//----------------------------------------------------------
//	�ƭ�����޳����
//----------------------------------------------------------
void PokeListMenuWinClear(u8 menu_type, const MenuListData *pmd)
{
	u8 xpos, ypos;
	
	GetMenuWinPos(menu_type, &xpos, &ypos, pmd);
	NMenuBoxClear(xpos, ypos, xpos + pmd[menu_type].x_len, 
		ypos + pmd[menu_type].menu_max * 2 + 1);
}

//----------------------------------------------------------
//	���ّI���ʒu�̊֐���ق���
//----------------------------------------------------------
pTaskFunc ListMenuCall(u8 menu_type, const MenuListData *pmd, const MENU_ITEM *mi, u8 pos)
{
	pTaskFunc ptask;
	
	ptask = mi[pmd[menu_type].menu_list[pos]].ptr;
	return ptask;
}




//=========================================================================
//	ү���޳���޳���o����ү���ނ𗬂�
// ����: msg = ү�����ް��ւ̱��ڽ    clear_flg = 0:ү���ގ����ر  1:�ر��Ȃ�
//=========================================================================
u8 PokeListScrMsgSet(const u8 msg[], u8 clear_flg)
{
	u8 id;
	
	ListMsgStatus = 1;
	NMenuWinBoxWrite( SCR_MSG_WINSX, SCR_MSG_WINSY, SCR_MSG_WINEX, SCR_MSG_WINEY );
	NMenuTalkMsgSet( msg, SCR_MSG_WINSX+1, SCR_MSG_WINSY+1 );
	id = AddTask(PokeScrMsgPut, 1);
	TaskTable[id].work[0] = clear_flg;
	return id;
}
//--  --//
static void PokeScrMsgPut(u8 id)
{
	if(NMenuTalkMsgPut())
	{
		ListMsgStatus = 0;
		if(TaskTable[id].work[0] == 0)
			NMenuBoxClear( SCR_MSG_WINSX, SCR_MSG_WINSY, SCR_MSG_WINEX, SCR_MSG_WINEY );
		DelTask(id);
	}
}



//=======================================================================================
//	�A�C�e���֘A
//=======================================================================================
//-------------------------------------
//		�A�C�e���g�p�f�[�^��ۑ�
//-------------------------------------
static void SetItemUseData( u8 id, u16 item, pTaskFunc returnAdrs )
{
	P_ITEM->returnAdrs = returnAdrs;
	P_ITEM->main_id    = id;
	P_ITEM->cursor     = GetNowCursorPos(id);
	P_ITEM->item       = item;
	P_ITEM->pp         = &PokeParaMine[ P_ITEM->cursor ];
}

//----------------------------------------------------------
//	�߹�݂ɓ������������(Ұُ�������)
// �߂�l: 0 = ���폈��    1 = �G���[
//----------------------------------------------------------
u8 PokeAddItem(PokemonParam *pp, u16 itemno)
{
	u8 buffer[2];
	u8 temp;
	
	if(MailItemNoCheck(itemno) == 1)
	{
		temp = PokeAddNewMail(pp, itemno);
		if(temp == 0xff)
			return 1;
		ListStatus = 2;
	}
	else
		ListStatus = 1;
	buffer[0] = itemno & 0xff;
	buffer[1] = itemno >> 8;
	PokeParaPut(pp, ID_item, buffer);
	return 0;
}

//----------------------------------------------------------
//	�߹�݂ɓ������������Ƃ��̏���(ү���ޏ����t)
// ����:id = Ҳ������ID   itemno = �������鱲�т����ް
//		returnAdrs = �������I��������Ҳ������TaskAdrs�ɾ�Ă����
//
// �������������ꂽ���ǂ����̌��ʂ�ListStatus�ɓ���܂��B
//	ListStatus = 0:���������Ȃ�����	1:ҰوȊO�̱��т���������   2:Ұق���������
//----------------------------------------------------------
void ListPokeAddItem( u8 id, u16 itemno, pTaskFunc returnAdrs )
{
	u16	pokeitem;

	TaskTable[id].TaskAdrs = DummyTaskSeq;
	SetItemUseData( id, itemno, returnAdrs );
	pokeitem = PokeParaGet( P_ITEM->pp, ID_item );
	ListStatus = 0;

	// ���łɓ���������Ă���
	if( pokeitem != NON_ITEM ){

		// ���[���������Ă��Ƃ�
		if( MailItemNoCheck( pokeitem ) == 1 ){
			PokeListScrMsgSet( msg_poke_item00, 1 );
			AddTask( ScrMsgWait, 5 );
		// ���[���ȊO
		}else{
			PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
			GetItemName( pokeitem, StrTempBuffer1 );
			PM_MsgExpand( MsgExpandBuffer, msg_poke_item02 );
			PokeListScrMsgSet( MsgExpandBuffer, 1 );
			AddTask( ItemKoukanYesNoInit, 5 );
		}
		return;
	}

	PokeAddItem( P_ITEM->pp, itemno );
	SubItem( itemno, 1 );

	// �������铹����[��
	if( MailItemNoCheck( itemno ) ){
		TaskTable[id].TaskAdrs = returnAdrs;
		return;
	}

	SetMotasetaMsg( P_ITEM->cursor, itemno, 1 );
	AddTask( ScrMsgWait, 5 );
}

static void ScrMsgWait( u8 id )
{
	if( ListMsgStatus == 0 ){
		ItemIconStatusSet( P_ITEM->main_id, P_ITEM->cursor );
		TaskTable[ P_ITEM->main_id ].TaskAdrs = P_ITEM->returnAdrs;
		DelTask(id);
	}
}

static void ItemKoukanYesNoMain( u8 id )
{
	s8	temp;
	u16 hold_itemno;
	
	temp = YesNoSelectMain();
	if( temp == CURSOR_YES_VALUE ){
		NMenuBoxClear( 23, 8, 29, 13 );
		hold_itemno = PokeParaGet( P_ITEM->pp, ID_item );
		SubItem( P_ITEM->item, 1 );
		if( AddItem( hold_itemno, 1 ) == TRUE ){
			PokeAddItem( P_ITEM->pp, P_ITEM->item );
			if( MailItemNoCheck( P_ITEM->item ) )
				SetAzukaruMsg( P_ITEM->cursor, hold_itemno, 1 );
			else{
				GetItemName( P_ITEM->item, StrTempBuffer0 );
				PM_MsgExpand( MsgExpandBuffer, msg_poke_item06 );
				PokeListScrMsgSet( MsgExpandBuffer, 1 );
			}
		}else{
			PokeListScrMsgSet( msg_poke_item09, 0 );
			AddItem( P_ITEM->item, 1 );
		}

		TaskTable[id].TaskAdrs = ScrMsgWait;
		return;
	}

	if( temp == CURSOR_NO_VALUE || temp == CURSOR_CANCEL_VALUE ){
		if( temp == CURSOR_CANCEL_VALUE )	SePlay(SE_SELECT);
		NMenuBoxClear( 23, 8, 29, 13 );
		TaskTable[id].TaskAdrs = ScrMsgWait;
	}
}

static void ItemKoukanYesNoInit( u8 id )
{
	if( ListMsgStatus == 0 ){
		YesNoSelectInit( 23, 8 );
		TaskTable[id].TaskAdrs = ItemKoukanYesNoMain;
	}
}

// "�A�C�e������������"���b�Z�[�W���Z�b�g
void SetMotasetaMsg( u8 poke, u16 item, u8 clear )
{
	PokeNickNameSet( &PokeParaMine[ poke ], StrTempBuffer0 );
	GetItemName( item, StrTempBuffer1 );
	PM_MsgExpand( MsgExpandBuffer, msg_poke_item01 );
	PokeListScrMsgSet( MsgExpandBuffer, clear );
}


//----------------------------------------------------------
//	���[���{�b�N�X�̃��[�����Z�b�g
//----------------------------------------------------------
void ListPokeAddMailBox( u8 id, pTaskFunc returnAdrs )
{
	u16	pokeitem;
	MAIL_DATA * mail;

	TaskTable[id].TaskAdrs = DummyTaskSeq;
	SetItemUseData( id, 0, returnAdrs );
	pokeitem = PokeParaGet( P_ITEM->pp, ID_item );
	ListStatus = 0;

	mail = &Fld.MailData[ MAIL_TEMOTI_STOCK + MYPC_SYS->scr + MYPC_SYS->pos ];

	// ���łɓ���������Ă���
	if( pokeitem != NON_ITEM ){
		PokeListScrMsgSet( msg_poke_item07, 1 );
		AddTask( ScrMsgWait, 5 );
		return;
	}

	PokeAddWrittenMail( P_ITEM->pp, mail );
	MailDataInit( mail );

	PokeListScrMsgSet( msg_poke_item08, 1 );
	AddTask( ScrMsgWait, 5 );
}



//----------------------------------------------------------
//	�߹�݂������Ă铹���a����Ƃ��̏���
// ����:id = Ҳ������ID
//	returnAdrs = �������I��������Ҳ������TaskAdrs�ɾ�Ă����
//----------------------------------------------------------
void ListPokeDecItem( u8 id, pTaskFunc returnAdrs )
{
	u8	buffer[2];
	u16 pokeitem;
	
	TaskTable[id].TaskAdrs = DummyTaskSeq;
	SetItemUseData( id, 0, returnAdrs );
	pokeitem = PokeParaGet( P_ITEM->pp, ID_item );

	if( pokeitem == NON_ITEM ){
		PM_MsgExpand( MsgExpandBuffer, msg_poke_item03 );
		PokeListScrMsgSet( MsgExpandBuffer, 0 );
		AddTask( ScrMsgWait, 5 );
	}else{
		buffer[0] = 0;	buffer[1] = 0;
		if( AddItem( pokeitem, 1 ) == TRUE ){
			if( MailItemNoCheck( pokeitem ) == 1 )	PokeDelMail( P_ITEM->pp );
			SetAzukaruMsg( P_ITEM->cursor, pokeitem, 0 );
			PokeParaPut( P_ITEM->pp, ID_item, buffer );
		}else
			PokeListScrMsgSet( msg_poke_item09, 0 );
		AddTask( ScrMsgWait, 5 );
	}
}

// "��������"���b�Z�[�W���Z�b�g
static void SetAzukaruMsg( u8 poke, u16 item, u8 clear )
{
	PokeNickNameSet( &PokeParaMine[ poke ], StrTempBuffer0 );
	GetItemName( item, StrTempBuffer1 );
	PM_MsgExpand( MsgExpandBuffer, msg_poke_item04 );
	PokeListScrMsgSet( MsgExpandBuffer, clear );
}

//----------------------------------------------------------
//	�߹�݂������Ă郁�[������鎞�̏���
// ����:id = Ҳ������ID
//	returnAdrs = �������I��������Ҳ������TaskAdrs�ɾ�Ă����
//----------------------------------------------------------
void ListPokeDecMail( u8 id, pTaskFunc returnAdrs )
{
	TaskTable[id].TaskAdrs = DummyTaskSeq;
	SetItemUseData( id, 0, returnAdrs );
	P_ITEM->item = PokeParaGet( P_ITEM->pp, ID_item );
	PokeListScrMsgSet( msg_poke_pc_tensou00, 1 );
	AddTask( MailTensouYesNoInit, 5 );
}

static void MailDelYesNoMain( u8 id )
{
	s8 temp;
	
	temp = YesNoSelectMain();
	if( temp == CURSOR_YES_VALUE ){

		if( AddItem( P_ITEM->item, 1 ) == TRUE ){
			PokeDelMail( P_ITEM->pp );
			PokeListScrMsgSet( msg_poke_item05, 0 );
		}else
			PokeListScrMsgSet( msg_poke_item09, 0 );

		NMenuBoxClear( 23, 8, 29, 13 );
		TaskTable[id].TaskAdrs = ScrMsgWait;

		return;
	}

	if( temp == CURSOR_NO_VALUE || temp == CURSOR_CANCEL_VALUE ){
		if( temp == CURSOR_CANCEL_VALUE )	SePlay(SE_SELECT);
		NMenuBoxClear( 23, 8, 29, 13 );
		NMenuBoxClear( 0, 14, 29, 19 );
		TaskTable[id].TaskAdrs = ScrMsgWait;
	}
}

static void MailDelYesNoInit( u8 id )
{
	if( ListMsgStatus == 0 ){
		YesNoSelectInit( 23, 8 );
		TaskTable[id].TaskAdrs = MailDelYesNoMain;
	}
}

static void MailTensouYesNoMain( u8 id )
{
	s8	temp;
	
	temp = YesNoSelectMain();
	if( temp == CURSOR_YES_VALUE ){
		NMenuBoxClear( 23, 8, 29, 13 );
		if( MailTrans( P_ITEM->pp ) != 0xff )
			PokeListScrMsgSet( msg_poke_pc_tensou01, 0 );
		else
			PokeListScrMsgSet( msg_poke_pc_tensou02, 0 );
		TaskTable[id].TaskAdrs = ScrMsgWait;
		return;
	}

	if( temp == CURSOR_NO_VALUE || temp == CURSOR_CANCEL_VALUE ){
		if( temp == CURSOR_CANCEL_VALUE )	SePlay(SE_SELECT);
		NMenuBoxClear( 23, 8, 29, 13 );
		PokeListScrMsgSet( msg_poke_pc_tensou03, 1 );
		TaskTable[id].TaskAdrs = MailDelYesNoInit;
	}
}

static void MailTensouYesNoInit( u8 id )
{
	if(ListMsgStatus == 0){
		YesNoSelectInit( 23, 8 );
		TaskTable[id].TaskAdrs = MailTensouYesNoMain;
	}
}



//=======================================================================================
//									�Z�}�V���g�p����
//=======================================================================================
static void WazaMachinScrMsgWait( u8 id );
static void WazaMachinJingleWait( u8 id );

extern void ReturnNewWazaSet(void);
extern void NewWazaReturnBag(u8);
static void DelWazaSelectEnd2( u8 id );
extern u16	ReturnItemNo;

//-----------------------------------------------
//	�Z�}�V���Ŋo������Z���擾
//-----------------------------------------------
u16 GetWazaNoItem( u16 itemno )
{
	itemno -= ITEM_WAZAMASIN01;
	return	MachineNo[itemno];
}

//-----------------------------------------------
//	�Z��2�d�o�^�`�F�b�N
//	�߂�l : 1 = ���łɊo���Ă���
//		     0 = �o���Ă��Ȃ�
//-----------------------------------------------
u8 TemotiNijyuuCheck( PokemonParam * pp, u16 wazano )
{
	u8 i;
	
	for( i=0; i<4; i++ ){
		if( PokeParaGet( pp, ID_waza1+i ) == wazano )
			return	1;
	}
	return	0;
}

//----------------------------------------------------------
//	�Z�}�V�����g�p
//	���� : id         = ���C���^�X�N��ID
//		   itemno     = �A�C�e���ԍ�
//		   returnAdrs = �����I���チ�C���^�X�N�ɃZ�b�g
//----------------------------------------------------------
void ListPokeWazaSet( u8 id, u16 itemno, pTaskFunc returnAdrs )
{
	SePlay( SE_SELECT );
	TaskTable[id].TaskAdrs = DummyTaskSeq;
	SetItemUseData( id, itemno, returnAdrs );
	AddTask( ListPokeWazaSet2, 5 );
}

static void ListPokeWazaSet2( u8 id )
{
	PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
	P_ITEM->new_waza = GetWazaNoItem( P_ITEM->item );
	PM_strcpy( StrTempBuffer1, WazaNameData[ P_ITEM->new_waza ] );

	P_LIST->work[15] = 0;

	// �����Z����
	if( TemotiNijyuuCheck( P_ITEM->pp, P_ITEM->new_waza ) ){
		PM_MsgExpand( MsgExpandBuffer, msg_poke_waza07 );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );
		TaskTable[id].TaskAdrs = ScrMsgWait;
		return;
	}

	// ����������
#ifdef	PM_DEBUG
	if( DebugMainFlag == 0 &&
		PokeWazaMachineCheck( P_ITEM->pp, P_ITEM->item-ITEM_SKILL_START ) == 0 )
	{
		PM_MsgExpand( MsgExpandBuffer, msg_poke_waza01 );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );
		TaskTable[id].TaskAdrs = ScrMsgWait;
		return;
	}
#else
	if( PokeWazaMachineCheck( P_ITEM->pp, P_ITEM->item-ITEM_SKILL_START ) == 0 ){
		PM_MsgExpand( MsgExpandBuffer, msg_poke_waza01 );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );
		TaskTable[id].TaskAdrs = ScrMsgWait;
		return;
	}
#endif	PM_DEBUG

	// �Z���o����
	if( PokeWazaSet( P_ITEM->pp, P_ITEM->new_waza ) != 0xffff ){
		WazaOboetaSeq(id);
		return;
	}

	// �Z�������ς�
	PM_MsgExpand( MsgExpandBuffer, msg_poke_waza02 );
	PokeListScrMsgSet( MsgExpandBuffer, 1 );
	TaskTable[id].TaskAdrs = WazaKoukanYesNoInit;//ScrMsgWait;
}

//-----------------------------------------------
//	�Z���o����
//-----------------------------------------------
static void WazaOboetaSeq( u8 id )
{
	PM_strcpy( StrTempBuffer1, WazaNameData[ P_ITEM->new_waza ] );
	PM_MsgExpand( MsgExpandBuffer, msg_poke_waza00 );
	PokeListScrMsgSet( MsgExpandBuffer, 1 );

	FriendCalc( P_ITEM->pp, FRIEND_WAZA_MACHINE );

	if( P_LIST->work[15] == 0 &&
		P_ITEM->item < ITEM_HIDENMASIN01 )
	{
		SubItem( P_ITEM->item, 1 );
	}

	TaskTable[id].TaskAdrs = WazaMachinScrMsgWait;
}
// ���b�Z�[�W�I���҂�
static void WazaMachinScrMsgWait( u8 id )
{
	if( ListMsgStatus == 0 ){
		JinglePlay( MUS_FANFA1 );
		TaskTable[id].TaskAdrs = WazaMachinJingleWait;
	}
}
// �W���O���I���҂�
static void WazaMachinJingleWait( u8 id )
{
	if( JingleWait() == 0 )		return;

	if( ( sys.Trg & A_BUTTON ) || ( sys.Trg & B_BUTTON ) ){
		ItemIconStatusSet( P_ITEM->main_id, P_ITEM->cursor );

		if( P_LIST->work[15] == 1 ){
			WazaOboeNextCheck(id);
			return;
		}

		TaskTable[ P_ITEM->main_id ].TaskAdrs = P_ITEM->returnAdrs;
		DelTask(id);
	}
}

static void ScrWazaOboeMsgWait( u8 id )
{
	if( ListMsgStatus == 0 ){
		ItemIconStatusSet( P_ITEM->main_id, P_ITEM->cursor );

		if( P_LIST->work[15] == 1 ){
			WazaOboeNextCheck(id);
			return;
		}

		TaskTable[ P_ITEM->main_id ].TaskAdrs = P_ITEM->returnAdrs;
		DelTask(id);
	}
}

//-----------------------------------------------
//	�Z��Y��ĐV�����Z���o���邩
//-----------------------------------------------
static void WazaKoukanYesNoInit( u8 id )
{
	if( ListMsgStatus == 0 ){
		YesNoSelectInit( 23, 8 );
		TaskTable[id].TaskAdrs = WazaKoukanYesNoMain;
	}
}

static void WazaKoukanYesNoMain( u8 id )
{
	s8 temp;
	
	temp = YesNoSelectMain();
	if( temp == CURSOR_YES_VALUE ){
		NMenuBoxClear( 23, 8, 29, 13 );
		PokeListScrMsgSet( msg_poke_waza05, 1 );
		TaskTable[id].TaskAdrs = DelWazaSelectInit;
		return;
	}

	if( temp == CURSOR_NO_VALUE || temp == CURSOR_CANCEL_VALUE ){
		if( temp == CURSOR_CANCEL_VALUE )	SePlay(SE_SELECT);
		WazaAkirameSelectSeq(id);
	}
}

//-----------------------------------------------
//	�X�e�[�^�X��ʂ�
//-----------------------------------------------
static void NewWazaSetFade( u8 id )
{
	if( FadeData.fade_sw == 0 ){
		PokeNewWazaSet( PokeParaMine, P_ITEM->cursor,
						PokeCountMine-1, ReturnNewWazaSet, P_ITEM->new_waza );
		DelTask(id);
	}
}

static void DelWazaSelectInit( u8 id )
{
	if( ListMsgStatus == 0 ){
//		LightFadeReq( N_to_B );
		PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
		TaskTable[id].TaskAdrs = NewWazaSetFade;
	}
}

//-----------------------------------------------
//	�X�e�[�^�X��ʂ��畜�A
//-----------------------------------------------
// �Y���Z���I�����ꂽ
void WazaDelSet2( u8 id )
{
	u8	pos;
	u16	waza;

	if( FadeData.fade_sw != 0 )	return;
	
	TaskTable[id].TaskAdrs = DummyTaskSeq;
	SetItemUseData( id, ReturnItemNo, NewWazaReturnBag );
	
	pos  = GetNewWazaSelectPos();
	waza = PokeParaGet( P_ITEM->pp, ID_waza1 + pos );

	PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
	PM_strcpy( StrTempBuffer1, WazaNameData[waza] );
	PM_MsgExpand( MsgExpandBuffer, msg_poke_waza06 );
	PokeListScrMsgSet( MsgExpandBuffer, 1 );
	AddTask( DelWazaSelectEnd2, 5 );
}
// �V�����Z���Z�b�g
static void DelWazaSelectEnd2( u8 id )
{
	if( ListMsgStatus == 0 ){
		PokePPCountClear( P_ITEM->pp, GetNewWazaSelectPos() );
		PokeWazaSetPos( P_ITEM->pp, P_ITEM->new_waza, GetNewWazaSelectPos() );
		WazaOboetaSeq(id);
	}
}

// �Y���Z���I������Ă��Ȃ�
void WazaAkirameSelectSeq2( u8 id )
{
	if( FadeData.fade_sw != 0 )	return;
	
	TaskTable[id].TaskAdrs = DummyTaskSeq;
	SetItemUseData( id, ReturnItemNo, NewWazaReturnBag );
	
	PM_strcpy( StrTempBuffer1, WazaNameData[ P_ITEM->new_waza ] );
	PM_MsgExpand( MsgExpandBuffer, msg_poke_waza03 );
	PokeListScrMsgSet( MsgExpandBuffer, 1 );
	AddTask( WazaAkirameYesNoInit, 5 );
}

//-----------------------------------------------
//	�Z���o����̂�������߂�
//-----------------------------------------------
// ���b�Z�[�W�\��
static void WazaAkirameSelectSeq( u8 id )
{
	NMenuBoxClear( 23, 8, 29, 13 );
	PM_strcpy( StrTempBuffer1, WazaNameData[ P_ITEM->new_waza ] );
	PM_MsgExpand( MsgExpandBuffer, msg_poke_waza03 );
	PokeListScrMsgSet( MsgExpandBuffer, 1 );
	TaskTable[id].TaskAdrs = WazaAkirameYesNoInit;
}
// �͂�/�������Z�b�g
static void WazaAkirameYesNoInit( u8 id )
{
	if( ListMsgStatus == 0 ){
		YesNoSelectInit( 23, 8 );
		TaskTable[id].TaskAdrs = WazaAkirameYesNoMain;
	}
}
// �͂�/����������
static void WazaAkirameYesNoMain( u8 id )
{
	s8	temp;
	
	temp = YesNoSelectMain();
	if( temp == CURSOR_YES_VALUE ){
		NMenuBoxClear( 23, 8, 29, 13 );
		PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
		PM_strcpy( StrTempBuffer1, WazaNameData[ P_ITEM->new_waza ] );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_waza04 );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );

		TaskTable[id].TaskAdrs = ScrWazaOboeMsgWait;
		return;
	}

	if( temp == CURSOR_NO_VALUE || temp == CURSOR_CANCEL_VALUE ){
		if( temp == CURSOR_CANCEL_VALUE )	SePlay(SE_SELECT);
		NMenuBoxClear( 23, 8, 29, 13 );
		PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
		PM_strcpy( StrTempBuffer1, WazaNameData[ P_ITEM->new_waza ] );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_waza02 );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );
		TaskTable[id].TaskAdrs = WazaKoukanYesNoInit;
	}
}

//-----------------------------------------------
//	�I�����ꂽ�Z����`�Z���ǂ���
//-----------------------------------------------
u8 DelWazaHidenCheck( u16 waza )
{
	u8	i;

	for( i=0; i<8; i++ ){
		if( MachineNo[50+i] == waza )
			return	TRUE;
	}
	return	FALSE;
}


//=======================================================================================
//									�X�e�[�^�X��
//=======================================================================================
extern u8 * ItemPokeParmTbl[];

//-----------------------------------------------
//	HP�Q�[�W���Z�b�g
//-----------------------------------------------
static s16 HPRecoverGaugeSet( u8 id, GaugeDataBG * data, s8 flg )
{
	u32   trans_pos;
	u8    color;

	s16 * wk = TaskTable[id].work;

	data->MaxHP  = wk[ I_TEMP0 ];
	data->NowHP  = wk[ I_TEMP1 ];
	data->beHP   = wk[ I_TEMP2 ] * flg;
	data->CharNo = LISTGAUGE_TOPCHRNO;

	color = GetGaugeDottoColor( P_LIST->work[15], data->MaxHP );
	if( color >= 3 )	data->color = HP_GAUGE_G_PAL;
	if( color == 2 )	data->color = HP_GAUGE_Y_PAL;
	if( color <= 1 )	data->color = HP_GAUGE_R_PAL;

	trans_pos    = GaugeDefaultPos[ FightCheck() ][ P_ITEM->cursor ];

	return	GaugeBG( data, &P_ITEM->gauge_work, (void *)trans_pos, 0 );
}

//-----------------------------------------------
//	HP�Q�[�W���񕜂���
//-----------------------------------------------
static void LifeGaugeUpMain( u8 id )
{
	GaugeDataBG gdata;
	u16 kaifuku_hp;
	u8	xpos, ypos;

	P_LIST->work[15] = HPRecoverGaugeSet( id, &gdata, -1 );

	if( P_LIST->work[15] == -1 ){

		P_ITEM->gauge_work = 0;	//2002.09.14(�y) by matsuda
		
		if( -gdata.beHP + gdata.NowHP > gdata.MaxHP )
			kaifuku_hp = gdata.MaxHP - gdata.NowHP;
		else
			kaifuku_hp = -gdata.beHP;

		WazaDamage = -kaifuku_hp;	//�퓬�ł��̒l������PSP�̒l��ς���̂�
		
		PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
		PM_NumMsgSet( StrTempBuffer1, kaifuku_hp, NUM_MODE_LEFT, 3 );

		// 2002/02/19 by nakahiro
		if( TaskTable[id].work[I_TEMP4] == 0 )
			PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv00 );
		else	PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv07 );

		PokeIconAnmSet( GetPataChrActNo( P_ITEM->main_id, P_ITEM->cursor ), P_ITEM->pp );
		PokeCardColorChange(
			&PokeCardPos[ FightCheck() * 12 + P_ITEM->cursor * 2 ],
			CARD_B_PAL+CARD_FLASH_PAL );
		P_LIST->card_flg = 2;

		PokeListScrMsgSet( MsgExpandBuffer, 1 );
		kaifuku_hp += gdata.NowHP;
		PokeParaPut( P_ITEM->pp, ID_hp, (u8*)&kaifuku_hp );
		SubItem( P_ITEM->item, 1 );
		
		PinchSeCheckStop();	// add by matsuda 2002.07.17(��)
	
		TaskTable[id].TaskAdrs = LifeScrMsgWait2;
	}else{
		xpos = HpDefaultPos[ FightCheck() ][ P_ITEM->cursor ].x;
		ypos = HpDefaultPos[ FightCheck() ][ P_ITEM->cursor ].y;
		PokeListNumberPut( P_LIST->work[15], P_HP_TRANS_CHARNO+P_ITEM->cursor * P_KETA, 1, xpos, ypos);
	}
}

//-----------------------------------------------
//	HP�Q�[�W�����炷
//-----------------------------------------------
void LifeGaugeDownMain( u8 id )
{
	GaugeDataBG gdata;
	u8	xpos, ypos;

	P_LIST->work[15] = HPRecoverGaugeSet( id, &gdata, 1 );

	if( P_LIST->work[15] == -1 ){
		SePlay( SE_KAIFUKU );

		P_ITEM->gauge_work = 0;	//2002.09.14(�y) by matsuda

		TaskTable[id].work[I_TEMP1] -= TaskTable[id].work[I_TEMP2];
		PokeParaPut( P_ITEM->pp, ID_hp, (u8*)&TaskTable[id].work[I_TEMP1] );

		PokeIconAnmSet( GetPataChrActNo( P_ITEM->main_id, P_CARD->cur[0] ), P_ITEM->pp );

		P_ITEM->cursor = ActWork[ P_CARD->cur[1] ].work[0];
		P_ITEM->pp     = &PokeParaMine[P_ITEM->cursor];

		TaskTable[id].work[I_TEMP0] = PokeParaGet( P_ITEM->pp, ID_hpmax );
		TaskTable[id].work[I_TEMP1] = PokeParaGet( P_ITEM->pp, ID_hp );

		P_ITEM->gauge_work = HP_WORK_INIT_VALUE;	// TYPE
//		TaskTable[id].work[I_TEMP3] = HP_WORK_INIT_VALUE;	// TYPE

		P_ITEM->SubReturn = P_ITEM->returnAdrs;
		TaskTable[id].TaskAdrs = LifeGaugeUpMain;
//		P_LIST->work[15] = 0;
		P_LIST->work[15] = TaskTable[id].work[I_TEMP1];
	}else{
		xpos = HpDefaultPos[ FightCheck() ][ P_ITEM->cursor ].x;
		ypos = HpDefaultPos[ FightCheck() ][ P_ITEM->cursor ].y;
		PokeListNumberPut( P_LIST->work[15], P_HP_TRANS_CHARNO+P_ITEM->cursor * P_KETA, 1, xpos, ypos);
	}
}

//-----------------------------------------------
//	���b�Z�[�W�I����҂��ă��C���֕��A
//-----------------------------------------------
static void LifeScrMsgWait( u8 id )
{
	if( ListMsgStatus )	return;

	TaskTable[ P_ITEM->main_id ].TaskAdrs = P_ITEM->returnAdrs;
	DelTask(id);
}

// HP�Q�[�W�p
static void LifeScrMsgWait2( u8 id )
{
	if( ListMsgStatus )	return;

	TaskTable[ P_ITEM->main_id ].TaskAdrs = P_ITEM->SubReturn;
	DelTask(id);
}

//-----------------------------------------------
//	HP���񕜂��邩
//-----------------------------------------------
static u8 LifeRecoverItemCheck2( u16 itemno )
{
	u8 * param;

	if( itemno == ITEM_NAZONOMI )	param = (u8 *)&Fld.RamSeed.item_param;
	else		param = ItemPokeParmTbl[ itemno - ITEM_NORMAL_START ];

	if( ( param[4] &  0x04 ) != 0 )	return	1;
	return	0;
}

//-----------------------------------------------
//	�m���񕜎���Lv�ĕ\��
//-----------------------------------------------
static void StRecoverLvPut( PokemonParam * pp, u8 pos )
{
	PokeConditionDelCardMoji( pos );
	ListPokeLvPut( pos, pp );
}

//-----------------------------------------------
//	�A�C�e�����ƂɃ��b�Z�[�W���Z�b�g
//-----------------------------------------------
static void SetStatusRecoverMsg( u16 itemno )
{
	switch( ItemRecoverCheck( itemno ) ){
	case ITEM_TYPE_DOKU_RCV:	// �ŉ�
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv01 );
		break;
	case ITEM_TYPE_NEMURI_RCV:	// �����
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv03 );
		break;
	case ITEM_TYPE_YAKEDO_RCV:	// �Ώ���
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv04 );
		break;
	case ITEM_TYPE_KOORI_RCV:	// �X��
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv05 );
		break;
	case ITEM_TYPE_MAHI_RCV:	// ��჉�
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv02 );
		break;

	case ITEM_TYPE_KONRAN_RCV:	// ������
		PM_MsgExpand( MsgExpandBuffer, msg_konran_rcv );
		break;
	case ITEM_TYPE_MEROMERO_RCV:	// ����������
		PM_MsgExpand( MsgExpandBuffer, msg_meromero_rcv );
		break;
	case ITEM_TYPE_ALL_ST_RCV:	// �X�e�[�^�X�ُ�S��
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv08 );
		break;
	case ITEM_TYPE_HP_UP:		// �h�[�s���O�A�C�e�� HP�w�͒lUP
		PM_strcpy( StrTempBuffer1, str_tairyoku );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv11 );
		break;
	case ITEM_TYPE_ATC_UP:		// �h�[�s���O�A�C�e�� �U���w�͒lUP
		PM_strcpy( StrTempBuffer1, str_pst_kougeki );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv11 );
		break;
	case ITEM_TYPE_DEF_UP:		// �h�[�s���O�A�C�e�� �h��w�͒lUP
		PM_strcpy( StrTempBuffer1, str_pst_bougyo );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv11 );
		break;
	case ITEM_TYPE_AGL_UP:		// �h�[�s���O�A�C�e�� �f�����w�͒lUP
		PM_strcpy( StrTempBuffer1, str_pst_subayasa );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv11 );
		break;
	case ITEM_TYPE_SPA_UP:		// �h�[�s���O�A�C�e�� ���U�w�͒lUP
		PM_strcpy( StrTempBuffer1, str_tokusyukougeki );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv11 );
		break;
	case ITEM_TYPE_SPD_UP:		// �h�[�s���O�A�C�e�� ���h�w�͒lUP
		PM_strcpy( StrTempBuffer1, str_tokusyubougyo );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv11 );
		break;
	case ITEM_TYPE_PP_UP:		// PPUp
	case ITEM_TYPE_PP_3UP:		// PP3Up
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv09 );
		break;
	case ITEM_TYPE_PP_RCV:		// PP��
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv06 );
		break;
	default:
		PM_MsgExpand( MsgExpandBuffer, msg_poke_use_error00 );
	}
}

//-----------------------------------------------
//	�k�P�j���̃`�F�b�N
//-----------------------------------------------
static u8 StRcvItem_NUKENIN_Check( PokemonParam * pp, u16 item )
{
	if( ItemRecoverCheck( item ) != ITEM_TYPE_HP_UP )		return	FALSE;
	if( PokeParaGet( pp, ID_monsno ) != MONSNO_NUKENIN )	return	FALSE;
	return	TRUE;
}

//-----------------------------------------------
//	StatusRecover���Z�b�g
//-----------------------------------------------
u8 SetStatusRecover( u8 cur, u16 item, u8 menu )
{
	if(sys.FieldBattleFlag == 1)	// �퓬
		return	StatusRecover( &PokeParaMine[cur], item, GetListRow(cur), menu );
	return	StatusRecover( &PokeParaMine[cur], item, cur, menu );
}

//-----------------------------------------------------------------------
//	�X�e�[�^�X�ϓ��A�C�e�����g�p
//	���� : id         = ���C���^�X�N��ID
//		   itemno     = �A�C�e���ԍ�
//		   returnAdrs = �����I���チ�C���^�X�N�ɃZ�b�g
//  
//	ListStatus�F���т��g�p�o�����ꍇ = 1;	�g�p�o���Ȃ������ꍇ = 0
//-----------------------------------------------------------------------
void PokeListStatusRecover( u8 id, u16 itemno, pTaskFunc returnAdrs )
{
	u8	newtask;
	u8	temp = 0, temp2 = 0;
	
	TaskTable[id].TaskAdrs = DummyTaskSeq;
	
	newtask = AddTask( DummyTaskSeq, 5 );

	SetItemUseData( id, itemno, returnAdrs );

	if( StRcvItem_NUKENIN_Check( P_ITEM->pp, itemno ) == FALSE ){

		temp2 = LifeRecoverItemCheck2( itemno );

		if( temp2 == 1 ){
			TaskTable[newtask].work[I_TEMP0] = PokeParaGet( P_ITEM->pp, ID_hpmax );
			TaskTable[newtask].work[I_TEMP1] = PokeParaGet( P_ITEM->pp, ID_hp );
			if( TaskTable[newtask].work[I_TEMP0] == TaskTable[newtask].work[I_TEMP1] ){
				temp2 = 0;
				TaskTable[newtask].work[I_TEMP0] = 0;
				TaskTable[newtask].work[I_TEMP1] = 0;
			}
		}

		temp = SetStatusRecover( P_ITEM->cursor, itemno, 0 );

	}else	temp = 1;

	if( temp != 0 ){
		ListStatus = 0;
		SePlay(SE_SELECT);
		PokeListScrMsgSet( msg_poke_use_error00, 1 );
		TaskTable[newtask].TaskAdrs = LifeScrMsgWait;
	}else{
		ListStatus = 1;

		if( VidoroCheck( itemno ) == FALSE )
			SePlay( SE_KAIFUKU );
		else
			SePlay( SE_BIDORO );

		// ��Ԉُ�`�F�b�N
		temp = GetPokeCondition( P_ITEM->pp );
		if( temp == ST_POKERUS || temp == ST_NORMAL )
			StRecoverLvPut( P_ITEM->pp, P_ITEM->cursor );

		if( temp2 == 1 ){
			TaskTable[newtask].work[I_TEMP2] =
				PokeParaGet( P_ITEM->pp, ID_hp ) - TaskTable[newtask].work[I_TEMP1];
			P_ITEM->gauge_work = HP_WORK_INIT_VALUE;	// TYPE
//			TaskTable[newtask].work[I_TEMP3] = HP_WORK_INIT_VALUE;
			if( TaskTable[newtask].work[I_TEMP1] == 0 )
				TaskTable[newtask].work[I_TEMP4] = 1;
			else	TaskTable[newtask].work[I_TEMP4] = 0;
			P_ITEM->SubReturn = P_ITEM->returnAdrs;
			TaskTable[newtask].TaskAdrs = LifeGaugeUpMain;
//			P_LIST->work[15] = 0;
			P_LIST->work[15] = TaskTable[newtask].work[I_TEMP1];
			return;
		}

		PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );

		if( VidoroCheck( itemno ) == FALSE )	SubItem( itemno, 1 );

		SetStatusRecoverMsg( itemno );

		PokeListScrMsgSet( MsgExpandBuffer, 1 );

		ListPokeNamePut( P_ITEM->cursor, P_ITEM->pp );

		TaskTable[newtask].TaskAdrs = LifeScrMsgWait;
	}
}

//----------------------------------------
//	�r�[�h���`�F�b�N
//----------------------------------------
static u8 VidoroCheck( u16 item )
{
	if( item == ITEM_AOIBIIDORO ||
		item == ITEM_AKAIBIIDORO ||
		item == ITEM_KIIROBIIDORO )
	{
		return	TRUE;
	}
	return	FALSE;
}


//-----------------------------------------------------------------------
//	�S���m���񕜃A�C�e�����g�p
//	���� : id         = ���C���^�X�N��ID
//		   itemno     = �A�C�e���ԍ�
//		   returnAdrs = �����I���チ�C���^�X�N�ɃZ�b�g
//  
//	ListStatus�F���т��g�p�o�����ꍇ = 1;	�g�p�o���Ȃ������ꍇ = 0
//-----------------------------------------------------------------------
static void PokeAllDethRcvMain( u8 id );
static void PokeAllDethRcvNext( u8 id );

void PokeListAllDethRecover( u8 id, u16 item, pTaskFunc returnAdrs )
{
	P_ITEM->returnAdrs = returnAdrs;
	P_ITEM->main_id    = id;
	P_ITEM->item       = item;
	P_ITEM->cursor     = 0;
	P_ITEM->SubReturn  = PokeAllDethRcvNext;

	P_LIST->work[13] = 0;
	P_LIST->work[14] = 0;

	PokeAllDethRcvMain(id);
}

static void PokeAllDethRcvMain( u8 id )
{
	u8    newtask;
	u8    temp;
	s16	* wk;

	TaskTable[id].TaskAdrs = DummyTaskSeq;

	if( PokeParaGet( &PokeParaMine[ P_ITEM->cursor ], ID_monsno ) == 0 ){
		TaskTable[id].TaskAdrs = PokeAllDethRcvNext;
		return;
	}

	newtask = AddTask( DummyTaskSeq, 5 );

	wk = TaskTable[newtask].work;

	P_ITEM->pp = &PokeParaMine[ P_ITEM->cursor ];

	wk[I_TEMP0] = PokeParaGet( P_ITEM->pp, ID_hpmax );
	wk[I_TEMP1] = PokeParaGet( P_ITEM->pp, ID_hp );

	temp = SetStatusRecover( P_ITEM->cursor, P_ITEM->item, 0 );

	if( temp != 0 ){
		DelTask( newtask );
		TaskTable[id].TaskAdrs = PokeAllDethRcvNext;
	}else{
		ListStatus = 1;
		NMenuBoxClear( SCR_MSG_WINSX, SCR_MSG_WINSY, SCR_MSG_WINEX, SCR_MSG_WINEY );
		SePlay( SE_KAIFUKU );

		StRecoverLvPut( P_ITEM->pp, P_ITEM->cursor );

		PokeCardColorChange(
			&PokeCardPos[ FightCheck() * 12 + P_ITEM->cursor * 2 ],
			CARD_R_PAL+CARD_FLASH_PAL );
		P_LIST->card_flg = 2;

		wk[I_TEMP2] = PokeParaGet( P_ITEM->pp ,ID_hp ) - wk[I_TEMP1];
//		wk[I_TEMP3] = HP_WORK_INIT_VALUE;
		wk[I_TEMP4] = 1;
		P_LIST->work[13] = 1;	// �A�C�e���g�p
		P_LIST->work[14] = 1;	// �񕜂���
//		P_LIST->work[15] = 0;
		P_LIST->work[15] = wk[I_TEMP1];
		P_ITEM->gauge_work = HP_WORK_INIT_VALUE;	// TYPE
		TaskTable[newtask].TaskAdrs = LifeGaugeUpMain;
	}
}

static void PokeAllDethRcvNext( u8 id )
{
	if( P_LIST->work[13] == 1 ){
		AddItem( P_ITEM->item, 1 );

		if( PokeParaGet( &PokeParaMine[ P_ITEM->cursor ], ID_monsno ) != 0 ){
			PokeCardColorChange(
				&PokeCardPos[ FightCheck() * 12 + P_ITEM->cursor * 2 ],
				CARD_B_PAL );
			P_LIST->card_flg = 2;
		}

		P_LIST->work[13] = 0;
	}

	P_ITEM->cursor += 1;

	if( P_ITEM->cursor == TEMOTI_POKEMAX ){
		ListStatus = 0;
		if( P_LIST->work[14] == 0 ){
			TaskTable[id].TaskAdrs = DummyTaskSeq;
			PokeListScrMsgSet( msg_poke_use_error00, 1 );
			AddTask( LifeScrMsgWait, 8 );
		}else{
			SubItem( P_ITEM->item, 1 );
			TaskTable[id].TaskAdrs = P_ITEM->returnAdrs;
		}
		FPokeNo = 0;
		return;
	}

	PokeAllDethRcvMain(id);
}


//=========================================================================================
//		PP�񕜏���	2002/02/14 by nakahiro
//=========================================================================================
extern void FieldBagPokeListTask(u8);
extern void BattlePokeListMain(u8);
static void PokeListPPRecover(u8);

//-----------------------------------------------
//	�킴�E�B���h�E�\��
//-----------------------------------------------
static void DrawWazaSelectWin( u8 no )
{
	u8	i,j=0;
	u16	waza;

	NMenuWinBoxWrite( 20, 10, 29, 19 );
	for( i=0; i<4; i++ ){
		waza = PokeParaGet( &PokeParaMine[no], ID_waza1+i );
		NMenuMsgWrite( WazaNameData[ waza ], 22, 11+(i<<1) );
		if( waza != 0 )	j++;
	}
	NMenuVCursorInit( 21, 11, j, 0 );
}

//-----------------------------------------------
//	�Z�I��
//-----------------------------------------------
static void WazaSelectMain( u8 id )
{
	if( sys.Trg & U_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet( -1 );
		return;
	}
	if( sys.Trg & D_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet( 1 );
		return;
	}
	if( sys.Trg & A_BUTTON ){
//		SePlay( SE_SELECT );
		waza_select.set_func(id);
		return;
	}
	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		waza_select.cancel_func(id);
	}
}


// �����ݒ�
void PPRecoverInit( u8 id, u16 itemno, pTaskFunc returnAdrs )
{
	u8   newtask;
	u8 * param;

	if( itemno == ITEM_NAZONOMI )	param = (u8 *)&Fld.RamSeed.item_param;
	else		param = ItemPokeParmTbl[ itemno - ITEM_NORMAL_START ];

	TaskTable[id].TaskAdrs = DummyTaskSeq;

	newtask = AddTask( DummyTaskSeq, 5 );

	SetItemUseData( id, itemno, returnAdrs );

	// ���ׂĂ̋Z
	if( ( param[4] & 0x10 ) == 0 ){
		TaskTable[newtask].work[I_TEMP1] = 0;
		PokeListPPRecover( newtask );
	}else{
		SePlay( SE_SELECT );
//		PokeListWinClear2();
		PokeListWinFixMsgPut2( WAZARECOVER_MSG, 1 );
		DrawWazaSelectWin( P_ITEM->cursor );
		TaskTable[ newtask ].TaskAdrs = WazaSelectMain;
		sys.Trg = 0;	// ?
	}
}

// �񕜂���Z���I�����ꂽ
static void WazaPPRecoverSet( u8 id )
{
	NMenuBoxClear( 19, 10, 29, 19 );
	PokeListWinClear2();
	TaskTable[id].work[I_TEMP1] = NMenuVCursorPosGet();
	PokeListPPRecover(id);
}

// �L�����Z��
static void WazaPPRecoverCancel( u8 id )
{
	NMenuBoxClear( 19, 10, 29, 19 );
	if(sys.FieldBattleFlag == 1)	// �퓬
		TaskTable[ P_ITEM->main_id ].TaskAdrs = BattlePokeListMain;
	else							// �t�B�[���h
		TaskTable[ P_ITEM->main_id ].TaskAdrs = FieldBagPokeListTask;
	PokeListWinFixMsgPut2( TUKAIMASUKA_MSG, 0 );
	DelTask(id);
}

// �񕜏���
static void PokeListPPRecover( u8 id )
{
	u8	temp;
	u16	waza=0;

	temp = SetStatusRecover(
				P_ITEM->cursor,
				P_ITEM->item,
				(u8)TaskTable[id].work[I_TEMP1] );

	if( temp != 0 ){
		ListStatus = 0;
		SePlay( SE_SELECT );
		PokeListScrMsgSet( msg_poke_use_error00, 1 );
	}else{
		ListStatus = 1;
		SePlay( SE_KAIFUKU );
		SubItem( P_ITEM->item, 1 );
		waza = PokeParaGet(
					P_ITEM->pp,
					ID_waza1+TaskTable[id].work[I_TEMP1]	);
		PM_strcpy( StrTempBuffer0, WazaNameData[waza] );
		SetStatusRecoverMsg( P_ITEM->item );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );
	}

	TaskTable[id].TaskAdrs = LifeScrMsgWait;
}

//=========================================================================================
//		�킴�|�C���g�A�b�v	2002/02/15 by nakahiro
//=========================================================================================
void WazaPointUpInit( u8 id, u16 itemno, pTaskFunc returnAdrs )
{
	TaskTable[id].TaskAdrs = DummyTaskSeq;
	SetItemUseData( id, itemno, returnAdrs );

	SePlay( SE_SELECT );
//	PokeListWinClear2();
	PokeListWinFixMsgPut2( WAZAPOINTUP_MSG, 1 );
	DrawWazaSelectWin( P_ITEM->cursor );
	AddTask( WazaSelectMain, 5 );
	sys.Trg = 0;	// ?
}


//=========================================================================================
//		�ӂ����ȃA��	2002/02/20 by nakahiro
//=========================================================================================
#define	PARAM_WIN_SX	11
#define	PARAM_WIN_SY	0
#define	PARAM_WIN_EX	( PARAM_WIN_SX + 18 )
#define	PARAM_WIN_EY	( PARAM_WIN_SY + 7 )

enum{
	WK_MHP = 0,		// �ő�HP
	WK_POW,			// �U��
	WK_DEF,			// �h��
	WK_SPP,			// ���U
	WK_SPD,			// ���h
	WK_AGL,			// �f����

	WK_MHP2,		// �ő�HP
	WK_POW2,		// �U��
	WK_DEF2,		// �h��
	WK_SPP2,		// ���U
	WK_SPD2,		// ���h
	WK_AGL2,		// �f����
};

extern void FieldMenuBagInit(void);

static void LvUpScrMsgWait(u8);
static void LvUpParamSet(u8,PokemonParam *);
static void StatusWinWait(u8);
static void LvUpEnd(u8);
static void DrawStPlusWindow( u8 id );
static void DrawNewStWindow( u8 id );
static void StatusPlusWinWait( u8 id );



static const u8 * const str_param[] = {
	str_pst_HP,			// HP
	str_pst_kougeki,	// ��������
	str_pst_bougyo,		// �ڂ�����
	str_pst_tokukou,	// �Ƃ�����
	str_pst_tokubou,	// �Ƃ��ڂ�
	str_pst_subayasa	// ���΂₳
};
static const u8 param_ID[] = {
	ID_hpmax,
	ID_pow,
	ID_def,
	ID_spepow,
	ID_spedef,
	ID_agi
};


// ��������
void PokeListLvUpInit(  u8 id, u16 itemno, pTaskFunc returnAdrs )
{
	u8	temp;
	u8	Lv;
	u8	i;

	TaskTable[id].TaskAdrs = DummyTaskSeq;
	
	SetItemUseData( id, itemno, returnAdrs );

	if( PokeParaGet( P_ITEM->pp, ID_level ) != 100 ){
		for( i=0; i<6; i++ )
			P_LIST->work[ WK_MHP + i ] = PokeParaGet( P_ITEM->pp, param_ID[i] );

		temp = SetStatusRecover( P_ITEM->cursor, itemno, 0 );
	}else	temp = 1;

	if( temp != 0 ){
		ListStatus = 0;
		SePlay( SE_SELECT );
		PokeListScrMsgSet( msg_poke_use_error00, 1 );
		AddTask( LifeScrMsgWait, 5 );
	}else{
		ListStatus = 1;
		MePlay(0);
		LvUpParamSet( P_ITEM->cursor, P_ITEM->pp );
		SubItem( itemno, 1 );
		PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
		Lv = PokeParaGet( P_ITEM->pp, ID_level );
		PM_NumMsgSet( StrTempBuffer1, Lv, NUM_MODE_LEFT, 3);
		PM_MsgExpand( MsgExpandBuffer, msg_poke_rcv10 );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );
		AddTask( LvUpScrMsgWait, 5 );
	}
}

// ���b�Z�[�W�E�F�C�g
static void LvUpScrMsgWait( u8 id )
{
	if( MeWait(0) == 0 )	return;
	if( ListMsgStatus )		return;

	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		DrawStPlusWindow( id );
		TaskTable[id].TaskAdrs = StatusPlusWinWait;
	}
}

static void StatusPlusWinWait( u8 id )
{
	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		DrawNewStWindow(id);
		TaskTable[id].TaskAdrs = StatusWinWait;
	}
}

// �X�e�[�^�X�\��
static void DrawStPlusWindow( u8 id )
{
	u8	i, x, y;

	NMenuWinBoxWrite( PARAM_WIN_SX, PARAM_WIN_SY, PARAM_WIN_EX, PARAM_WIN_EY );

	for( i=0; i<6; i++ ){
		P_LIST->work[ WK_MHP2+i ] = PokeParaGet( P_ITEM->pp, param_ID[i] );
		P_LIST->work[ WK_MHP+i ]  = P_LIST->work[ WK_MHP2+i ] - P_LIST->work[ WK_MHP+i ];

		x = PARAM_WIN_SX + ( i / 3 * 9 );
		y = PARAM_WIN_SY + 1 + ( ( i % 3 ) << 1 );

		NMenuMsgWrite( str_param[i], x+1, y );	// �p�����[�^��
		NMenuMsgWrite( str_plus, x+6, y );		// '+'

		// plus param
		PM_NumMsgSet( StrTempBuffer0, P_LIST->work[ WK_MHP+i ], NUM_MODE_SPACE, 2 );
		NMenuMsgWrite( StrTempBuffer0, x+7, y );
	}
}

static void DrawNewStWindow( u8 id )
{
	u8	i, x, y;

	for( i=0; i<6; i++ ){
		P_LIST->work[ WK_MHP2+i ] = PokeParaGet( P_ITEM->pp, param_ID[i] );

		x = PARAM_WIN_SX + ( i / 3 * 9 );
		y = PARAM_WIN_SY + 1 + ( ( i % 3 ) << 1 );

		// new param
		PM_NumMsgSet( StrTempBuffer0, P_LIST->work[ WK_MHP2+i ], NUM_MODE_SPACE, 3 );
		NMenuBoxWrite( x+6, y, x+8, y+1 );
		NMenuMsgWrite( StrTempBuffer0, x+6, y );
	}
}

// Lv,HP,MHP�X�V
static void LvUpParamSet( u8 pos, PokemonParam * pp )
{
	u8	xpos, ypos;
	u8	cond;

	// Lv
	cond = GetPokeCondition( pp );
	if( cond == ST_NORMAL || cond == ST_POKERUS ){
		StRecoverLvPut( pp, pos );
	}
	// HP
	xpos = HpDefaultPos[FightCheck()][pos].x;
	ypos = HpDefaultPos[FightCheck()][pos].y;
	PokeListNumberPut(
		PokeParaGet( pp, ID_hp ),
		P_HP_TRANS_CHARNO + pos * P_KETA, 1, xpos, ypos );
	// MHP
	xpos = MaxHpDefaultPos[FightCheck()][pos].x;
	ypos = MaxHpDefaultPos[FightCheck()][pos].y;
	PokeListNumberPut(
		PokeParaGet(pp, ID_hpmax),
		P_HPMAX_TRANS_CHARNO + pos * P_KETA, 1, xpos, ypos );

	// HP�o�[�X�V
	ListGaugeScreenSet( pos, pp );

	// �A�C�R���A�j��
	PokeIconAnmSet( GetPataChrActNo( P_ITEM->main_id, pos ), pp );

	// �J�[�h���
	PokeCardColorChange(
			&PokeCardPos[ FightCheck() * 12 + pos * 2 ],
			CARD_B_PAL+CARD_FLASH_PAL );
	P_LIST->card_flg = 2;
}

// �L�[�E�F�C�g
static void StatusWinWait( u8 id )
{
	u16 waza, aftermons;

	if( MeWait(0) == 0 )	return;

	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
		NMenuBoxClear( PARAM_WIN_SX, PARAM_WIN_SY, PARAM_WIN_EX, PARAM_WIN_EY );

		waza = PokeWazaOboeCheck( P_ITEM->pp ,1 );
		P_LIST->work[15] = 1;
		switch( waza ){
		case 0:
			aftermons = PokeShinkaCheck( P_ITEM->pp, LEVELUP_SHINKA, 0 );
			if( aftermons != 0 ){
				ShinkaReturnProc = FieldMenuBagInit;
				InitShinka( P_ITEM->pp, aftermons, SHINKA_CANCEL_ON , P_ITEM->cursor );
				DelTask(id);
				return;
			}

			LvUpEnd(id);
			break;

		case NO_WAZA_SET:
			PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
			PM_strcpy( StrTempBuffer1, WazaNameData[ OboeWazaNo ] );
			PM_MsgExpand( MsgExpandBuffer, msg_poke_waza02 );
			PokeListScrMsgSet( MsgExpandBuffer, 1 );
			P_ITEM->new_waza = OboeWazaNo;
			TaskTable[id].TaskAdrs = WazaKoukanYesNoInit;
			break;

		case SAME_WAZA_SET:
			TaskTable[id].TaskAdrs = WazaOboeNextCheck;
			break;

		default:
			PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
			PM_strcpy( StrTempBuffer1, WazaNameData[waza] );
			PM_MsgExpand( MsgExpandBuffer, msg_poke_waza00 );
			PokeListScrMsgSet( MsgExpandBuffer, 1 );
			TaskTable[id].TaskAdrs = WazaMachinScrMsgWait;
		}
	}
}

// �o������Z�������Ȃ�܂Ń`�F�b�N
static void WazaOboeNextCheck( u8 id )
{
	u16 waza, aftermons;

	waza = PokeWazaOboeCheck( P_ITEM->pp ,0 );
	switch( waza ){
	case 0:
		aftermons = PokeShinkaCheck( P_ITEM->pp, LEVELUP_SHINKA, 0 );
		if( aftermons != 0 ){
			ShinkaReturnProc = FieldMenuBagInit;
			InitShinka( P_ITEM->pp, aftermons, SHINKA_CANCEL_ON , P_ITEM->cursor );
			DelTask(id);
			return;
		}

		LvUpEnd(id);
		break;

	case NO_WAZA_SET:
		PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
		PM_strcpy( StrTempBuffer1, WazaNameData[ OboeWazaNo ] );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_waza02 );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );
		P_ITEM->new_waza = OboeWazaNo;
		TaskTable[id].TaskAdrs = WazaKoukanYesNoInit;
		break;

	case SAME_WAZA_SET:
		WazaOboeNextCheck(id);
		break;

	default:
		PokeNickNameSet( P_ITEM->pp, StrTempBuffer0 );
		PM_strcpy( StrTempBuffer1, WazaNameData[waza] );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_waza00 );
		PokeListScrMsgSet( MsgExpandBuffer, 1 );
		TaskTable[id].TaskAdrs = WazaMachinScrMsgWait;
	}
}

// �I��
static void LvUpEnd( u8 id )
{
	TaskTable[ P_ITEM->main_id ].TaskAdrs = P_ITEM->returnAdrs;
	DelTask(id);
}


//=========================================================================================
//		�i���A�C�e��	2002/04/24 by nakahiro
//=========================================================================================
// ��������
void PokeListEvolutionInit( u8 id, u16 item, pTaskFunc returnAdrs )
{
	u8	temp;

	SePlay( SE_SELECT );

	TaskTable[id].TaskAdrs = DummyTaskSeq;
	
	SetItemUseData( id, item, returnAdrs );
	
	ShinkaReturnProc = FieldMenuBagInit;
	temp = SetStatusRecover( P_ITEM->cursor, item, 0 );

	if( temp != 0 ){
		ListStatus = 0;
		PokeListScrMsgSet( msg_poke_use_error00, 1 );
		AddTask( LifeScrMsgWait, 5 );
	}else{
		SubItem( item, 1 );
	}
}


//=========================================================================================
//	�A�C�e�����ʂ𒲂ׂ�
//=========================================================================================
//-----------------------------------------------------------------------------------------
// static const u8 ItemParam000[] = { flg1, flg2, flg3, flg4, flg5, flg6, p0, p1, ...... };
//
// flg1 = 1�ޯāF���ۉ� / 1�ޯāF�S���m���� / 2�ޯāF��è��(0-3) / 4�ޯāF�U����(0-7)
// flg2 = ���4�ޯāF�h���(0-15) / ����4�ޯāF�f����  (0-15)
// flg3 = ���4�ޯāF����  (0-15) / ����4�ޯāF����U��(0-15)
// flg4 = �\�̓K�[�h/LvUP/�����/�ŉ�/�Ώ���/�X��/��჉�/������
// flg5 = �i��/�m����/pp�lUP/pp��flg/pp�񕜒l/HP��/�U���w�͒l/HP�w�͒l
// flg6 = �Ȃ��x3/�Ȃ��x2/�Ȃ��x1/pp�l3UP/���U�w�͒l/���h�w�͒l/�f�����w�͒l/�h��w�͒l
//
// p0,p1,...... = �ϓ��l
// �D��x�FHP�w�͒l > �U���w�͒l > �h��w�͒l > �f�����w�͒l > HP�� > pp�񕜒l >
//         ���h�w�͒l > ���U�w�͒l > �Ȃ��x1 > �Ȃ��x2 > �Ȃ��x3
//-----------------------------------------------------------------------------------------
#ifdef	PM_DEBUG
const u8 debug_ram_item[] = { 0x40, 0x00, 0x00, 0x00, 0x44, 0x00, 0xff, };
/*	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//static const u8 ItemParam045[] = { 0x40, 0x00, 0x00, 0x00, 0x44, 0x00, 0xff, };
*/
#endif	PM_DEBUG

u8 ItemRecoverCheck( u16 item )
{
	u8 * param;
	u8   st;

	if( item == ITEM_NAZONOMI )	param = (u8 *)&Fld.RamSeed.item_param;
	else	param = ItemPokeParmTbl[ item - ITEM_NORMAL_START ];

	// �퓬�p�X�e�[�^�X�A�b�v�n
	if( ( param[0] & 0x3f ) != 0 || param[1] != 0 ||
		param[2] != 0 || ( param[3] & 0x80 ) != 0 )
	{
		return	ITEM_TYPE_BTL_ST_UP;
	}
	// �S���m����
	if( ( param[0] & 0x40 ) != 0 )	return	ITEM_TYPE_ALLDETH_RCV;
	// LvUp�n
	if( ( param[3] & 0x40 ) != 0 )	return	ITEM_TYPE_LV_UP;
	// Status�񕜌n
	st = ( param[3] & 0x3f );
	if( st != 0 || ( param[0] >> 7 ) != 0 ){
		if( st == 0x20 )	return	ITEM_TYPE_NEMURI_RCV;	// ����
		if( st == 0x10 )	return	ITEM_TYPE_DOKU_RCV;		// ��
		if( st == 0x08 )	return	ITEM_TYPE_YAKEDO_RCV;	// �Ώ�
		if( st == 0x04 )	return	ITEM_TYPE_KOORI_RCV;	// �X
		if( st == 0x02 )	return	ITEM_TYPE_MAHI_RCV;		// ���
		if( st == 0x01 )	return	ITEM_TYPE_KONRAN_RCV;	// ����
		if( ( param[0] >> 7 ) != 0 && st == 0 )
			return	ITEM_TYPE_MEROMERO_RCV;		// ����������
		return	ITEM_TYPE_ALL_ST_RCV;			// �S��
	}
	// HP�� ( �m���� )
	if( ( param[4] & 0x44 ) != 0 )	return	ITEM_TYPE_HP_RCV;
	// �h�[�s���O�n
	if( ( param[4] & 0x02 ) != 0 )	return	ITEM_TYPE_ATC_UP;	// �U���w�͒lUP
	if( ( param[4] & 0x01 ) != 0 )	return	ITEM_TYPE_HP_UP;	// HP�w�͒lUP
	if( ( param[5] & 0x08 ) != 0 )	return	ITEM_TYPE_SPA_UP;	// ���U�w�͒lUP
	if( ( param[5] & 0x04 ) != 0 )	return	ITEM_TYPE_SPD_UP;	// ���h�w�͒lUP
	if( ( param[5] & 0x02 ) != 0 )	return	ITEM_TYPE_AGL_UP;	// �f�����w�͒lUP
	if( ( param[5] & 0x01 ) != 0 )	return	ITEM_TYPE_DEF_UP;	// �h��w�͒lUP
	// �i���n
	if( ( param[4] & 0x80 ) != 0 )	return	ITEM_TYPE_EVO;
	// ppUp�n
	if( ( param[4] & 0x20 ) != 0 )	return	ITEM_TYPE_PP_UP;
	// pp3Up�n
	if( ( param[5] & 0x10 ) != 0 )	return	ITEM_TYPE_PP_3UP;
	// pp�񕜌n
	if( ( param[4] & 0x18 ) != 0 )	return	ITEM_TYPE_PP_RCV;

	return	ITEM_TYPE_ETC;
}
