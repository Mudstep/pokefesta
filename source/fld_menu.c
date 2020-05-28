//======================================================
//    fld_menu.c                                           
//                                                      
//    Copyright (C) 2000 GAME FREAK inc.
//======================================================



#include "common.h"

#define	__FLD_MENU_H_GLOBAL__
#include "fld_menu.h"

#include "menu.h"
#include "fld_main.h"
#include "palanm.h"
#include "task.h"
#include "script.h"
#include "weather.h"
#include "sysflag.h"
#include "ev_flag.h"
#include "report.h"
#include "zukan.h"
#include "pokesio.h"
#include "mus_tool.h"
#include "mystatus.h"
#include "evobj.h"
#include "jiki.h"
#include "safari.h"
#include "initgame.h"
#include "record.h"
#include "fld_talk.h"
#include "fld_task.h"



//------------------------- ��` -----------------------------

//�t�B�[���h���j���[�\���Ɋւ����`
enum {
	FLDMENU_X1 = 22,			//���j���[���[�w���W
	FLDMENU_X2 = 29,			//���j���[�E�[�w���W
	FLDMENU_Y1 = 0,				//���j���[��[�x���W
	FLDMENU_Y2 = FLDMENU_Y1 + 3,

	FLDMENU_MSGX = FLDMENU_X1 + 2,
	FLDMENU_MSGY = FLDMENU_Y1 + 2,
	FLDMENU_CSRX = FLDMENU_X1 + 1,
	FLDMENU_CSRY = FLDMENU_Y1 + 2,

	FLDMENU_MSGHEIGHT = 2,		//8x16�t�H���g�ł̈�s�̍���

	FIELD_MENU_MAX = 9,			//���j���[���ڍő吔
};

//�T�t�@���]�[���̏��E�B���h�E�Ɋւ����`
enum {
	SAFARIWIN_X1 = 0,
	SAFARIWIN_Y1 = 0,
	SAFARIWIN_X2 = 9,
	SAFARIWIN_Y2 = 5,

	SAFARIWIN_MSGX = SAFARIWIN_X1 + 1,
	SAFARIWIN_MSGY = SAFARIWIN_Y1 + 1,
};

//------------------------- �f�[�^ -----------------------------


// �t�B�[���h���j���[�p�f�[�^

static const u8 str_zukan[]    = {zu_,ka_,n_,EOM_};
static const u8 str_pokemon[]  = {PO_,KE_,MO_,N_,EOM_};
static const u8 str_bag[]      = {BA_,TTU_,GU_,EOM_};
static const u8 str_pokegear[] = {PO_,KE_,NA_,BI_,EOM_};
static const u8 str_hero[]     = {I_MSG_,MY_NAME_BUF,EOM_};
static const u8 str_report[]   = {RE_,PO_,bou_,TO_,EOM_};
static const u8 str_config[]   = {se_,ttu_,te_,i_,EOM_};
static const u8 str_close[]    = {to_,zi_,ru_,EOM_};
static const u8 str_retire[]   = {RI_,TA_,I_,A_,EOM_};


// �T�t�@���p�\���f�[�^
static const u8 str_safari_window[] =
{
	SA_,HU_,AA_,RI_,BO_,bou_,RU_,
	CR_,
	no_,ko_,ri_,spc_,I_MSG_,STR_TEMP_BUF0,KO_,
	EOM_,
};



//------------------------- �ϐ� -----------------------------

u8	(*MenuFunc)();		//���j���[����֐��̃|�C���^


static u8 fieldPick = 0;						//�I�𒆃��j���[�ʒu
static u8 fieldMax = 0;							//���j���[�\���s��
static u8 fieldMenuList[ FIELD_MENU_MAX ] = {};	//���j���[���ڕێ����X�g




//-------------------- �T�u���[�`���錾 ------------------------

extern void TNaviInit(void);

extern void FieldMenuPokeListInit(void);
extern void ConfigInit(void);	//�����Ă�
extern void FieldMenuBagInit(void);		//�o�b�O


static void FieldMenuInit(void);

static void MakeFieldMenuCom(void);
static void make_normal_menu_command(void);
static void make_comm_menu_command(void);
static void make_safari_menu_command(void);
#ifdef	FESTA_VER_ROM
static void make_festa_menu_command(void);
#endif

static u8 NormalMenuSelect(void);

static u8 Menu00(void);
static u8 Menu01(void);
static u8 Menu02(void);
static u8 Menu03(void);
static u8 Menu04(void);
static u8 Menu05(void);
static u8 Menu06(void);
static u8 Menu07(void);
static u8 Menu08(void);
static u8 Menu09(void);


// �Z�[�u�֘A
static u8 FieldSaveSeqMainInit(void);
static u8 FieldSaveSeqMainFunc(void);



static const MENU_ITEM	FieldMenuItem[]=
{
	{ str_zukan,	Menu00 },	//00:������
	{ str_pokemon,	Menu01 },	//01:�|�P����
	{ str_bag,		Menu02 },	//02:�o�b�O
	{ str_pokegear,	Menu03 },	//03:�g���i�r
	{ str_hero,		Menu04 },	//04:�q�[���[
	{ str_report,	Menu05 },	//05:���|�[�g
	{ str_config,	Menu06 },	//06:�����Ă�
	{ str_close,	Menu07 },	//07:�Ƃ���
	{ str_retire,	Menu08 },	//08:���^�C�A
	{ str_hero,		Menu09 },	//09:�ʐM���̃g���[�i�[�J�[�h
};

//==================================================================================
//                       ���C�����[�`��
//==================================================================================

//==============================================================
//
//	���j���[���X�g���ڂ̐ݒ菈��
//
//==============================================================

//-----------------------------------------
//		�f�o�b�O�p���j���[�t���O�Z�b�g
//-----------------------------------------
void DebugMenuFlagSet(void)
{
	EventFlagSet( SYS_ZUKAN_GET );
	EventFlagSet( SYS_POKEMON_GET );
	EventFlagSet( SYS_POKEGEAR_GET );
}

//------------------------------
//		���j���[���X�g�쐬
//------------------------------
static void MakeFieldMenuCom(void)
{
	fieldMax = 0;

#ifdef	FESTA_VER_ROM
	if ( FestaModeFlag == TRUE ) {
		make_festa_menu_command();
		return;
	}
#endif
	if ( IsFieldCommMode() == TRUE ) {
		make_comm_menu_command();			//�ʐM�����j���[
	}
	else if ( IsFieldSafariMode() == TRUE ) {
		make_safari_menu_command();			//�T�t�@�����[�h�����j���[
	}
	else {
		make_normal_menu_command();			//�ʏ펞���j���[
	}
}

static void FieldMenuComSet( u8 no )
{
	MenuComSet( fieldMenuList, &fieldMax, no );
}


/* �ʏ펞�̃��j���[���X�g�쐬 */
static void make_normal_menu_command(void)
{
	//������
	if( EventFlagCheck( SYS_ZUKAN_GET ) == ON )
		FieldMenuComSet( 0 );

	//�|�P����
	if( EventFlagCheck( SYS_POKEMON_GET ) == ON )
		FieldMenuComSet( 1 );

	FieldMenuComSet( 2 );	//�o�b�O

	//�|�P�M�A
	if( EventFlagCheck( SYS_POKEGEAR_GET ) == ON )
		FieldMenuComSet( 3 );

	FieldMenuComSet( 4 );	//��l��
	FieldMenuComSet( 5 );	//���|�[�g
	FieldMenuComSet( 6 );	//�����Ă�
	FieldMenuComSet( 7 );	//�Ƃ���
}

/* �T�t�@�����[�h���̃��j���[���X�g�쐬 */
static void make_safari_menu_command( void )
{
	FieldMenuComSet( 8 );	//���^�C�A
	FieldMenuComSet( 0 );	//������
	FieldMenuComSet( 1 );	//�|�P����
	FieldMenuComSet( 2 );	//�o�b�O
	FieldMenuComSet( 4 );	//��l��
	FieldMenuComSet( 6 );	//�����Ă�
	FieldMenuComSet( 7 );	//�Ƃ���
}

/* �ʐM���̃��j���[���X�g�쐬 */
static void make_comm_menu_command(void)
{
	FieldMenuComSet( 1 );	//�|�P����
	FieldMenuComSet( 2 );	//�o�b�O
	if (EventFlagCheck(SYS_POKEGEAR_GET) == TRUE)
		FieldMenuComSet( 3 );	//�|�P�i�r
	FieldMenuComSet( 9 );	//�ʐM���̎�l��
	FieldMenuComSet( 6 );	//�����Ă�
	FieldMenuComSet( 7 );	//�Ƃ���
}

#ifdef	FESTA_VER_ROM
static void make_festa_menu_command(void)
{
	FieldMenuComSet( 1 );	//�|�P����
	FieldMenuComSet( 2 );	//�o�b�O
	FieldMenuComSet( 4 );	//��l��
	FieldMenuComSet( 7 );	//�Ƃ���
}
#endif


//==============================================================
//
//	���j���[��ʐ�������
//
//==============================================================
//-----------------------------------------
//�@�T�t�@���Ŏc��{�[���̐���\��
//-----------------------------------------
static void MakeSafariInfoWindow( void )
{
	PM_NumMsgSet( StrTempBuffer0, SafariBallCount, NUM_MODE_SPACE, 2 );
	NMenuWinBoxWrite( SAFARIWIN_X1, SAFARIWIN_Y1, SAFARIWIN_X2, SAFARIWIN_Y2 );
	NMenuMsgWrite( str_safari_window, SAFARIWIN_MSGX, SAFARIWIN_MSGY );
}

//-----------------------------------------
//	���j���[���X�g�̕����\��
//
//<����>	count	���݂̕\���s
//			step	��x�ɕ\������s��
//
//�����L�̌Ăяo���𕪉������֐��̂Ɠ���
//	NMenuFreeListWrite( FLDMENU_MSGX, FLDMENU_MSGY,
//	fieldMax,FieldMenuItem,fieldMenuList);
//-----------------------------------------
static int list_item_write( s16 * count, int step )
{
	int n;

	n = *count;

	while ( TRUE ) {
		NMenuMsgWrite( ( FieldMenuItem[fieldMenuList[n]] ).msg,
				FLDMENU_MSGX, FLDMENU_MSGY+(n*FLDMENU_MSGHEIGHT) );
		n++;
		if ( n >= fieldMax ) {
			*count = n;
			return TRUE;
		}
		step --;
		if ( step == 0 ) {
			*count = n;
			return FALSE;
		}
	}
}

//-----------------------------------------
//	���j���[�����������C��
//
//	���ʐM���̏�������������邽�߁A��������
//	�@��������悤�ɂȂ��Ă���
//-----------------------------------------
static int make_field_menu( s16 * wk1, s16 *wk2 )
{
	switch ( *wk1 ) {
	case 0:
		++ *wk1; break;

	case 1:
#if defined(FIELD_TUUSHIN_DEBUG) && defined(PM_DEBUG)
		DebugNumPrintInitField();
#endif
		MakeFieldMenuCom();
		++ *wk1; break;

	case 2:
		NMenuWinBoxWrite( FLDMENU_X1, FLDMENU_Y1,
				FLDMENU_X2, FLDMENU_Y2 + (fieldMax*FLDMENU_MSGHEIGHT) );
		*wk2 = 0;
		++ *wk1; break;

	case 3:
		if ( IsFieldSafariMode() ) MakeSafariInfoWindow();
		++ *wk1; break;

	case 4:
		if ( list_item_write( wk2, 2 ) == FALSE ) break;
		++ *wk1; break;

	case 5:
		fieldPick = NMenuVCursorInit(FLDMENU_CSRX,FLDMENU_CSRY,fieldMax,fieldPick);
		return TRUE;
	}
	return FALSE;
}


//-----------------------------------------
//	��C�Ƀt�B�[���h���j���[�𐶐�
//-----------------------------------------
static void FieldMenuInit(void)
{
	s16 wk1,wk2;
	wk1 = 0; wk2 = 0;

	while ( make_field_menu( &wk1, &wk2 ) == FALSE );
}

//-----------------------------------------
//	�t�B�[���h���j���[�����^�X�N
//-----------------------------------------
static void make_field_menu_task( u8 no )
{
	s16 * wk = TaskTable[no].work;

	if ( make_field_menu( &wk[0], &wk[1] ) == TRUE ) {
		wk[0] = 0;
		ChangeBackTask(no);
	}
}

//-----------------------------------------
//	�t�B�[���h���j���[������A�w��^�X�N�ɐ����n��
//	<����>	ret		�^�X�N�֐��A�h���X
//-----------------------------------------
void FieldMenuInitTaskSet( pTaskFunc ret )
{
	u8 no;

	NMenuScreenInit(FIELD_MENU_TYPE);
	no = AddTask( make_field_menu_task, TSK_PRI_EFFECT );
	ChangeTask( no, make_field_menu_task, ret );
}


//==============================================================
//
//		�t�B�[���h���j���[����
//
//==============================================================
//-----------------------------------------
//	�t�B�[���h���j���[����^�X�N
//-----------------------------------------
void FieldMenuFuncTask(u8 no)
{
	TASK_TABLE * my = &TaskTable[no];

	switch( my->work[0] ) {
	case 0:
		MenuFunc = NormalMenuSelect;
		my->work[0]++;
		break;

	case 1:
		if(MenuFunc()==1)DelTask(no);
	}
}

//-----------------------------------------
//	���j���[���������j���[�^�X�N�ɐ����n��
//-----------------------------------------
void StartFieldMenuEvent(void)
{
	if ( IsFieldCommMode() == FALSE ) {
		/* �ʏ��OBJ�̓�����Ƃ߂� */
		EvObjPauseAll();
		HeroAcmdForceStop();
		SetHeroMoveStateInit();
	}

	FieldMenuInitTaskSet( FieldMenuFuncTask );
	SetForceEvent();
}



//-------------------------------------------------------------
//						������
//-------------------------------------------------------------
static u8 NormalMenuSelect(void)
{
	if( sys.Trg & U_KEY )
	{
		SePlay(SE_SELECT);
		fieldPick = NMenuVCursorSet(-1);
	}
	if( sys.Trg & D_KEY )
	{
		SePlay(SE_SELECT);
		fieldPick = NMenuVCursorSet(1);
	}

	if( sys.Trg & A_BUTTON )
	{
		SePlay(SE_SELECT);

		if( FieldMenuItem[ fieldMenuList[fieldPick] ].ptr == Menu00 &&
			ZukanTotal( Z_SEE_CHK ) == 0	)
		{
			return	0;
		}

		MenuFunc = FieldMenuItem[fieldMenuList[fieldPick]].ptr;

		if( MenuFunc != Menu05 && MenuFunc != Menu07 && MenuFunc != Menu08 )
			FieldFadeControl(FIELD_FADE_OUT, 0);

		return	0;
	}

	if( sys.Trg & B_BUTTON || sys.Trg & START_BUTTON )
	{
		MenuEnd();
		return	1;
	}

	return	0;
}



//������
static u8	Menu00(void)
{
	if(!(FadeData.fade_sw)){
		IncRecord( CNT_POKEDEX );		//������������񐔂��J�E���g
		StopRainSound();
		MainProcChange( ZukanInit);
		return	1;
	}

	return	0;
}
//�|�P����
static u8	Menu01(void)
{
	if(!(FadeData.fade_sw)){
		StopRainSound();
		MainProcChange( FieldMenuPokeListInit);
		return 1;
	}

	return	0;
}
//�o�b�O
static u8	Menu02(void)
{
	if(!(FadeData.fade_sw)){
		StopRainSound();
		MainProcChange( FieldMenuBagInit);
		return	1;
	}

	return	0;
}
//�|�P�M�A
static u8	Menu03(void)
{
	if(!(FadeData.fade_sw)){
		StopRainSound();
		MainProcChange( TNaviInit);
		return 1;
	}

	return	0;
}
//�q�[���[
static u8	Menu04(void)
{
	if(!(FadeData.fade_sw)){
		StopRainSound();
		SetTrainerCardPlayer( FieldMenuRecover );
		return	1;
	}

	return	0;
}
//���|�[�g
static u8	Menu05(void)
{
	MenuFunc = FieldSaveSeqMainInit;
	return 0;
}
//�����Ă�
static u8	Menu06(void)
{
	if(!(FadeData.fade_sw)){
		StopRainSound();
		MainProcChange( ConfigInit);
		sys.pReturnProc = FieldMenuRecover;
		return	1;
	}

	return	0;
}
//�Ƃ���
u8	Menu07(void)
{
	MenuEnd();
	return	1;
}

//���^�C�A
u8	Menu08(void)
{
	MenuEnd();
	SafariCancelAskStart();
	return 1;
}

u8	Menu09(void)
{
	if(!(FadeData.fade_sw)){
		StopRainSound();
		SetTrainerCardFriend(comm_my_id, FieldMenuRecover);
		return	1;
	}

	return	0;
}

//============================================
//			�|�P�������|�[�g����
//============================================

//--------------------------
//	�Z�[�u����֐�
//--------------------------
static u8 FieldSaveSeqMainInit(void)
{
	SaveSeqInit();
	MenuFunc = FieldSaveSeqMainFunc;
	return 0;
}

static u8 FieldSaveSeqMainFunc(void)
{
	switch( SaveSeqMain() ){

	case 0:
		return 0;

	case SAVE_CANCEL:/* �L�����Z�������j���[�ɖ߂� */
	
		NMenuScreenClear();
		FieldMenuInit();
		MenuFunc = NormalMenuSelect;
		return 0;

	case SAVE_SUCCESS:/* �Z�[�u�������t�B�[���h�ɖ߂� */
	case SAVE_FAILURE:/* �Z�[�u���s���t�B�[���h�ɖ߂� */
	
		NMenuScreenClear();
		TalkEnd();
		ResetForceEvent();
		return 1;
	
	default:
		return 0;
	};
}

