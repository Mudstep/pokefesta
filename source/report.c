//=========================================================================
//	
//
//	�|�P�������|�[�g�C���^�[�t�F�C�X����
//
//
//	2001.11.15		fld_menu.c���烌�|�[�g�����������ړ��A
//		by tama		���낢�돑�������ă��j���[�ȊO��������p�\�ɂ���
//=========================================================================

#include "common.h"
#include "menu.h"
#include "print.h"
#include "saveload.h"
#include "mus_tool.h"
#include "task.h"
#include "script.h"
#include "mystatus.h"
#include "fieldmap.h"
#include "initgame.h"
#include "record.h"
#include "intr.h"
#include "window.h"
#include "message.h"
#include "laster.h"
#include "actor.h"
#include "palanm.h"

#include	"svld_ini.h"
#include	"svld_sys.h"

#define	__REPORT_H_GLOBAL
#include "report.h"
#include "message.h"

#include "../msgdata/report.h"


//=========================================================================
//	���[�J���ϐ�
//=========================================================================
static u8 (*report_func)(void);	//�e�V�[�P���X�֐��p�|�C���^
static u8 wait_count;			//�E�F�C�g�p�J�E���^
static u8 msg_sw;				//MSG�\�����̃X�C�b�`


//=========================================================================
//	�f�[�^�E��`�Ȃ�
//=========================================================================

enum {
	//�͂��E�������E�B���h�E�̕\���ʒu
	SAVE_YESNO_WINX	=	20,
	SAVE_YESNO_WINY	=	8,

	//�Z�[�u���E�B���h�E�̕\���ʒu
	SAVE_INFO_WINX	=	0,
	SAVE_INFO_WINY	=	0,

	//�Z�[�u�I�����MSG�\�����ԁi1/60�b�P��)
	SAVE_FIN_MSG_WAIT	= 60,
};


//=========================================================================
//	���[�J���֐��v���g�^�C�v
//=========================================================================
static void draw_msg_seq_change( const u8 *msg, void (*func) );
static void erase_info_win(void);
static void erase_yes_no_win(void);
static void key_or_wait_init(void);
static u8 key_or_wait_check(void);

static void save_ctrl_task(u8 no);

// �V�[�P���X
static u8 SaveStart(void);
static u8 SaveYesNoInit(void);
static u8 SaveYesNoMain(void);
static u8 OverwriteYesNoInit(void);
static u8 OverwriteYesNoInit2(void);
static u8 OverwriteYesNoMain(void);
static u8 SaveMainInit(void);
static u8 SaveMain(void);
static u8 SaveFailWait(void);
static u8 SaveFailEnd(void);
static u8 SaveSuccessWait(void);
static u8 SaveSuccessEnd(void);


//=========================================================================
//						�v���O����
//=========================================================================

//-------------------------------------------------------------
//	�Z�[�u����������
//-------------------------------------------------------------
void SaveSeqInit( void )
{
	BackupVirtualMap();
	report_func = SaveStart;
	msg_sw = 0;
}

//-------------------------------------------------------------
//	�Z�[�u�������C��
//	<�߂�l>	���w�b�_���̖߂�l�̒�`���Q��
//-------------------------------------------------------------
u8 SaveSeqMain( void )
{
	if( msg_sw && NMenuTalkMsgPut() == 0 ) return 0;
	msg_sw = 0;
	return report_func();
}

//-------------------------------------------------------------
//	�X�N���v�g����̃Z�[�u�V�[�P���X�Ăяo��
//-------------------------------------------------------------
void ReportSequenceStart(void)
{
	SaveSeqInit();
	AddTask( save_ctrl_task, 80 );
}

//=========================================================================
//
//=========================================================================
//-------------------------------------------------------------
//		���胁�b�Z�[�W����
//-------------------------------------------------------------
static void draw_msg_seq_change( const u8 *msg, void (*func) )
{
	PM_MsgExpand( MsgExpandBuffer, msg );
	NMenuTalkWinWrite();
	NMenuTalkMsgSetEasy( MsgExpandBuffer );

	msg_sw = 1;
	report_func = func;
}

//-------------------------------------------------------------
//		�X�N���v�g����̃Z�[�u�V�[�P���X�Ăяo���p�^�X�N
//-------------------------------------------------------------
static void save_ctrl_task(u8 no)
{
	switch( SaveSeqMain() ){
	case 0:
		return;

	case SAVE_SUCCESS:
		AnswerWork = TRUE;
		break;

	case SAVE_CANCEL:
	case SAVE_FAILURE:
		AnswerWork = FALSE;
		break;
	}
	DelTask(no);
	ContinueFieldScript();
}

//-------------------------------------------------------------
//	�⏕�֐�
//-------------------------------------------------------------
//--------------------------
//	�Z�[�u���E�B���h�E����
//--------------------------
static void erase_info_win(void)
{
	MyStatusWindowClear(SAVE_INFO_WINX, SAVE_INFO_WINY);
}
//--------------------------
//	�͂��E�������E�B���h�E����
//--------------------------
static void erase_yes_no_win(void)
{
	NMenuBoxClear( SAVE_YESNO_WINX, SAVE_YESNO_WINY, SAVE_YESNO_WINX+6, SAVE_YESNO_WINY+5 );
}

//--------------------------
//	�������ԕt���L�[�E�F�C�g�ݒ�
//--------------------------
static void key_or_wait_init(void)
{
	wait_count = SAVE_FIN_MSG_WAIT;
}

//--------------------------
//	�������ԕt���L�[�E�F�C�g
//--------------------------
static u8 key_or_wait_check(void)
{
	wait_count --;
	if( sys.Cont & A_BUTTON ) {
		SePlay( SE_SELECT );
		return TRUE;
	}
	if( wait_count == 0 ) return TRUE;
	return FALSE;
}


static u8 key_and_wait_check(void)
{
	if ( wait_count == 0 ) {
		if ( sys.Cont & A_BUTTON ) return TRUE;
	} else {
		wait_count --;
	}
	return FALSE;
}

//-------------------------------------------------------------
//	���|�[�g�����V�[�P���X
//-------------------------------------------------------------
//------------------------------
//		"�Z�[�u���܂����H"
//------------------------------
static u8 SaveStart(void)
{
	NMenuScreenClear();
	MyStatusWindow(SAVE_INFO_WINX, SAVE_INFO_WINY);
	draw_msg_seq_change( save_start_msg, SaveYesNoInit );
	return	0;
}

static u8 SaveYesNoInit(void)
{
	YesNoSelectInit( SAVE_YESNO_WINX, SAVE_YESNO_WINY );
	report_func = SaveYesNoMain ;
	return	0;
}

static u8 SaveYesNoMain(void)
{
	switch( YesNoSelectMain() ) {

	case CURSOR_YES_VALUE :
		erase_yes_no_win();
		if ((SaveDataCheckInfo!=SVLD_RET_NULL && SaveDataCheckInfo!=SVLD_RET_DESTROY) ||
				FirstSaveFlag == 0 ) {
			//�Z�[�u�f�[�^�����łɑ��݂��Ă���ꍇ
			report_func = OverwriteYesNoInit ;
		} else {
			report_func = SaveMainInit ;
		}
		break;

	case CURSOR_CANCEL_VALUE:
	case CURSOR_NO_VALUE :
		erase_yes_no_win();
		erase_info_win();
		return SAVE_CANCEL;
	};	

	return	0;
}

//-------------------------------
//		"�㏑�����܂����H"
//-------------------------------
static u8 OverwriteYesNoInit(void)
{
	const u8 * msg;

	if (FirstSaveFlag == 1) {
		//�Z�[�u�f�[�^�������āA�Ȃ������߂ăZ�[�u����ꍇ
		//���ׂ̃��|�[�g��������Ă��܂��I�`
		msg = save_other_check_msg;
	} else {
		msg = save_check_msg;
	}
	draw_msg_seq_change(msg, OverwriteYesNoInit2);
	return	0;
}

static u8 OverwriteYesNoInit2(void)
{
	YesNoSelectInit( SAVE_YESNO_WINX, SAVE_YESNO_WINY );
	report_func = OverwriteYesNoMain ;
	return	0;
}

static u8 OverwriteYesNoMain(void)
{
	switch( YesNoSelectMain() ) {
	case CURSOR_YES_VALUE :
		erase_yes_no_win();
		report_func = SaveMainInit ;
		break;

	case CURSOR_CANCEL_VALUE:
	case CURSOR_NO_VALUE :
		erase_yes_no_win();
		erase_info_win();
		return SAVE_CANCEL;
	};	

	return	0;
}

//-------------------------------
//		"�Z�[�u���Ă��܂�"
//-------------------------------
static u8 SaveMainInit(void)
{
//	BGM�t�F�[�h�A�E�g������ꍇ�����Ƀt�F�[�h�J�n�����ǉ�
	draw_msg_seq_change( save_active_msg, SaveMain );
	return	0;
}

static u8 SaveMain(void)
{
	u8	svret_code;
	IncRecord( CNT_REPORT );

//	BGM�t�F�[�h�A�E�g������ꍇ�����ɃE�F�C�g�����ǉ�

	if ( FirstSaveFlag == 1 ) {
		svret_code = PokeAGBSave(POKEAGB_SVCLRALL);
		FirstSaveFlag = 0;
	}else{
		svret_code = PokeAGBSave(POKEAGB_SVALL);
	}
	switch(svret_code){

		case SVLD_RET_OK:
			draw_msg_seq_change( save_end_msg, SaveSuccessWait );
			break;
		default:
		case SVLD_RET_NG:
			draw_msg_seq_change( save_fail2_msg,SaveFailWait );
			break;
	}
	key_or_wait_init();
//	BGM�t�F�[�h�A�E�g������ꍇ�����Ƀt�F�[�h�C�������ǉ�

	return	0;
}

//-------------------------------
//		�Z�[�u�I��
//-------------------------------
static u8 SaveSuccessWait(void)
{
	if ( NMenuTalkMsgPut() ) {
		SePlay( SE_SAVE );
		report_func = SaveSuccessEnd;
	}
	return 0;
}

static u8 SaveSuccessEnd(void)
{
	if(SeEndCheck())	return 0;
	if( key_or_wait_check() == FALSE )return 0;
	erase_info_win();
	return SAVE_SUCCESS;
}

static u8 SaveFailWait(void)
{
	if ( NMenuTalkMsgPut() ) {
		SePlay( SE_BOO );
		report_func = SaveFailEnd;
	}
	return 0;
}

static u8 SaveFailEnd(void)
{
	if( key_and_wait_check() == FALSE )return 0;
	erase_info_win();
	return SAVE_FAILURE;
}


//=========================================================================
//
//
//	�ʐM�ΐ풼��̃Z�[�u����
//
//
//=========================================================================
static void comm_battle_save_vblank(void);
static int comm_battle_save_init(u8 *seq);
static void CommBattleSaveMain(void);
static void comm_battle_save_task(u8 no);

//-------------------------------------------------------------
//-------------------------------------------------------------
static void comm_battle_save_vblank(void)
{
	PaletteFadeTrans();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static int comm_battle_save_init(u8 *seq)
{
	switch (*seq) {
		case 0:
			*(vu16*)REG_DISPCNT = 0;

			SetVBlankFunc(NULL);
			LasterInit();
			DIV_DMACLEAR(3, 0, PLTT,PLTT_SIZE, 16);
			DIV_DMACLEAR(3, 0, VRAM,VRAM_SIZE, 16);		// �SVRAM������
			break;

		case 1:
			InitActor();
			InitTask();
			PaletteAnimeInit();
			LasterWorkInit();
			break;

		case 2:
			PrintScreenInit(FIELD_MENU_TYPE);
			NMenuScreenInit(FIELD_MENU_TYPE);
			*(vu16 *)REG_DISPCNT = DISP_MODE_0 | DISP_BG0_ON;
			break;

		case 3:
			ColorConceChange(0xffffffff, 16, 0x0000);
			SetVBlankFunc(comm_battle_save_vblank);
			REG_IE_SET(V_BLANK_INTR_FLAG);
			break;

		case 4:
			return TRUE;
	}
	++ *seq;
	return FALSE;
}

//-------------------------------------------------------------
//	�ΐ��̃Q�[���f�[�^�Z�[�u
//-------------------------------------------------------------
void CommBattleSaveInit(void)
{
	if (comm_battle_save_init(&sys.InitSeqWork) == FALSE)
		return;
	AddTask(comm_battle_save_task, 80);
	MainProcChange(CommBattleSaveMain);
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void CommBattleSaveMain(void)
{
	MainTask();
	PaletteAnime();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void comm_battle_save_task(u8 no)
{
#if 0
	s16 * twk;
	twk = TaskTable[no].work;

	if (FadeData.fade_sw == 1) return;

	switch (twk[0]) {
	case 0:
		NMenuTalkWinWrite();
		NMenuMsgWrite(report_writing_msg, MWIN_MSGXPOS, MWIN_MSGYPOS);
		PaletteFadeReq(0xffffffff, 0, 16, 0, 0x0000);
		twk[0]++;
		break;
	case 1:
		SetCommSaveFlag();
		PokeAGBSave(POKEAGB_SVFLD);
		ClearCommSaveFlag();
		twk[0] ++;
		break;
	case 2:
		PaletteFadeReq(0xffffffff, 0, 0, 16, 0x0000);
		twk[0] ++;
		break;
	case 3:
		MainProcChange(sys.pReturnProc);
		DelTask(no);
		break;
	}
#else
	s16 * twk;
	twk = TaskTable[no].work;

	if (FadeData.fade_sw == 1) return;

	switch (twk[0]) {
	case 0:
		NMenuTalkWinWrite();
		NMenuMsgWrite(report_writing_msg, MWIN_MSGXPOS, MWIN_MSGYPOS);
		PaletteFadeReq(0xffffffff, 0, 16, 0, 0x0000);
		twk[0]++;
		break;
	case 1:
		SetCommSaveFlag();
		PokeAGBSaveFieldDivInit();
		twk[0] ++;
		break;
	case 2:
		if(PokeAGBSaveFieldDiv()){
			ClearCommSaveFlag();
			twk[0] ++;
		}
		break;
	case 3:
		PaletteFadeReq(0xffffffff, 0, 0, 16, 0x0000);
		twk[0] ++;
		break;
	case 4:
		MainProcChange(sys.pReturnProc);
		DelTask(no);
		break;
	}
#endif
}

