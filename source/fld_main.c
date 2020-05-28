//======================================================
//    fld_main.c                                           
//                                                      
//    Copyright (C) 2000 GAME FREAK inc.
//======================================================


#include "common.h"
#include "field.h"
#include "intr.h"
#include "screen.h"
#include "fld_talk.h"
#include "task.h"
#include "initgame.h"


#define FLD_MAIN_H_GLOBAL
#include "fld_main.h"

#define MAP_H_GLOBAL
#include "map.h"


#include "fld_sio.h"
#include "mul_play.h"

#include "evobj.h"
#include "fieldmap.h"
#include "mapchr.h"		/* InitNowMapVisual */
#include "mapbg.h"
#include "mapparam.h"	// �}�b�v�^�C�v�R�[�h

#include "bg_anm.h"
#include "palanm.h"
#include "jiki.h"
#include "saveload.h"
#include "fld_tool.h"
#include "fld_task.h"
#include "script.h"
#include "ev_check.h"
#include "ev_flag.h"
#include "weather.h"
#include "wea_ctrl.h"
#include "laster.h"
#include "mapname.h"
#include "menu.h"
#include "playtime.h"		/* PlayTimeStart */
#include "ev_trai.h"		/* EvTrainerSightCheck */
#include "record.h"			/* IncRecord */
#include "ev_time.h"		/* UpdateTimeEvent */
#include "fld_menu.h"		/* StartFieldMenuEvent */
#include "mus_tool.h"

#include "multisio.h"
#include "pokesio.h"
#include "map_attr.h"
#include "ev_sub.h"
#include "ev_comm.h"		//CommExitSet
#include "seed.h"			//CheckSeedInScreen
#include "r_door.h"			//DoorObjSet
#include "scr_tool.h"		//CyclingRoadEventInit
#include "safari.h"			//ClearFieldSafariMode

#include "scroll.tbl"					// �ړ��x�N�^�[�f�[�^


//------------------------- �O���Q�� -----------------------------
extern u16 RecvData[];

extern SIOTRAINER_INFO SioTrainerInfo[];

//------------------------- �f�[�^ -----------------------------




//---------------------- �ϐ� --------------------------

pFunc pFieldRecoverFunc;		// �t�B�[���h���A�����̊֐��|�C���^


//---------------------- ���[�J���ϐ� --------------------------





//-------------------- �T�u���[�`���錾 ------------------------
static void Field_clear_intr(void);
static void Field_set_intr(void);
static void init_laster( void );
static void set_disp_register( void );


static void FieldMainCommStart(void);

static void field_start_init( u8 * seq );
static int make_normal_field(u8 * seq, int is_comm_mode);
static int make_comm_field( u8 * seq );
static int recover_comm_field( u8 * seq );
static int recover_normal_field( u8 * seq );

static void FieldInitControlData(int is_comm_mode);
static void FieldInitScreen( void );
static void FieldSetScreen(void);

static void init_comm_info( void );

static void init_normal_obj(void);
static void recover_normal_obj( void );
static void init_normal_scroll( void );
static void init_comm_obj(void);
static void init_comm_scroll( void );
static void recover_comm_scroll( void );
static void init_comm_player( void );
static void recover_comm_player( void );

void NormalFieldControl( void );

void CommFieldControl( void );




extern void FirstTruckEventStart( void );	//�ŏ��̃g���b�N���G�t�F�N�g�J�n


extern u8 BaseNameDraw(void);
//-------------------------------------------------------------
//-------------------------------------------------------------
typedef int (*COMM_CTRL_FUNC)(int);

u8 comm_player_num;			//�ʐM�������̐l��
u8 comm_my_id;				//�ʐM�������̎�����ID
u16 my_command;				//�ʐM���̓���R�[�h�󂯓n���p�ϐ�

static u8 comm_status[COMM_PLAYER_MAX];
static COMM_CTRL_FUNC comm_ctrl_func;

static u8 field_recv_fifo_clear_flag;	//��MFIFO�������[�h����t���O

static void update_all_comm_control( u16 * Recv, int my_id );
static void clear_all_comm_status( void );
static int check_all_comm_status( u16 mode );
static int check_any_comm_status( u16 mode );

static void clear_recv_data( u16 * Recv );
static void set_send_data( u16 cmd );



//==================================================================================
//
//	�ʐM�f�o�b�O�֘A
//
//==================================================================================

#if defined(FIELD_TUUSHIN_DEBUG) && defined(PM_DEBUG)

extern void DebugNumPrintInit2(u8 pal, u8 select, u8 screen, u8 chara);
extern void kanshi_network(u8 taskno);

u8 debug_comm_info[DB_INFO_MAX][COMM_PLAYER_MAX] = {};

u16 debug_player_cmd[COMM_PLAYER_MAX] = {};


//-------------------------------------------------------------
//-------------------------------------------------------------
void DebugNumPrintInitField( void )
{
	DebugNumPrintInit2(
		12,					//palette
		MSG_BG_NO,			//BG
		MSG_SCRN_BASE,		//scrn offset
		MSG_CHR_BASE		//chr offset
	);

	if ( CheckTask(kanshi_network) ) return;
	AddTask(kanshi_network,0);
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void debug_info_init( void )
{
	int i,j;
	for(i=0;i<COMM_PLAYER_MAX;i++) {
		debug_player_cmd[i] = 0;
		for(j=0;j<DB_INFO_MAX;j++) {
			debug_comm_info[j][i] = 0;
		}
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void debug_info_set( void )
{
	int i;

	for(i=0;i<comm_player_num;i++){
		if ( RecvData[i] == INVALID_DATA ) {
			debug_comm_info[0][i]++;
		}
		if ( sys.Cont & L_BUTTON ) {
			debug_player_cmd[i] = (debug_comm_info[1][i] << 8) | debug_comm_info[0][i];
		} else if ( sys.Cont & B_BUTTON ) {
			debug_player_cmd[i] = (comm_status[i] << 8 ) | ( RecvData[i] & 0xff );
		} else {
			debug_player_cmd[i] = (debug_comm_info[2][i] << 8) | debug_comm_info[3][i];
		}
	}
}
#endif

#if defined(FIELD_TUUSHIN_DEBUG) && defined(PM_DEBUG)

#define DEBUG_SIO_PRINT_INIT()	DebugNumPrintInitField()
#define DEBUG_SIO_INFO_SET()	debug_info_set()
#define DEBUG_SIO_INFO_INIT()	debug_info_init()
#define DEBUG_SIO_INFO_INC(type,id)	{ debug_comm_info[(type)][(id)]++; }

#else

#define	DEBUG_SIO_PRINT_INIT()
#define DEBUG_SIO_INFO_SET()
#define DEBUG_SIO_INFO_INIT()
#define DEBUG_SIO_INFO_INC(type,id)

#endif




static int comm_normal_control( int );
static int comm_event_control( int );
static int comm_rfifo_event_control( int n );
static int comm_sfifo_event_control( int n );
static int comm_start_control( int );
static int comm_exit_control( int );

//==================================================================================
//                       ���C�����[�`��
//==================================================================================

static void init_field_mode( void )
{
	ClearFieldSafariMode();
}

//-------------------------------------------------------------
//	�t�B�[���h�ʐM���[�h���ǂ����̃`�F�b�N
//	�����͊֐��|�C���^�����Ă��邪�t���O�ɕύX���邩��
//-------------------------------------------------------------
int IsFieldCommMode( void )
{
	if ( sys.pFirstProc == CommFieldControl ) {
		return TRUE;
	} else {
		return FALSE;
	}
}


//==============================================================
//	�t�B�[���h���C���V�[�P���X
//==============================================================

//-------------------------------------------------------------
//	�ʏ펞�̃t�B�[���h�C�x���g����
//-------------------------------------------------------------
void MineControl( u16 key_trg, u16 key_cont )
{
	EV_REQUEST my_req;

	HeroMvStateSet();					// ���@����󋵾��

	ClearRequest( &my_req );
	SetRequest( &my_req, key_trg, key_cont );

	if ( CheckForceEvent() == FALSE ) {
		if ( CheckRequest( &my_req ) == TRUE ) {
			SetForceEvent();
			EraseMapName();
		} else {
			HeroMoveControl( my_req.Site, key_trg, key_cont );	// ���@���쐧��
		}
	}
}

void NormalFieldControl( void )
{
	if ( sys.pMainProc != FieldMain ) return;
	MineControl( sys.Trg, sys.Cont );
}




//-------------------------------------------------------------
//		�t�B�[���h�@���C��
//-------------------------------------------------------------
static void FieldMainCore(void)
{
	ControlFieldScript();
	MainTask();

	JumpActor();	
	ControlFieldScroll();
	MapViewControl();
	SetActor();	
	PaletteAnime();
	MapBGanimeMain();
}

void FieldEffectMain(void)
{
	FieldMainCore();
}

void FieldMain(void)
{
	u8 sw;
	sw = FadeData.fade_sw;

	if (sw) SetVBlankFunc( NULL );

	FieldMainCore();

	if (sw) Field_set_intr();
}


//==============================================================
//	�t�B�[���h�������V�[�P���X
//==============================================================
//-------------------------------------------------------------
//	����v���Z�X�̐؂�ւ�
//-------------------------------------------------------------
void SetControlFunc( pFunc fnc )
{
	sys.pFirstProc = fnc;
}

//-------------------------------------------------------------
//	�ʐM���䕔���̐؂�ւ�
//-------------------------------------------------------------
void SetControlCommFunc( COMM_CTRL_FUNC func )
{
	comm_ctrl_func = func;
}

//-------------------------------------------------------------
//	�������p�R�[���o�b�N�֐��̌Ăяo��
//-------------------------------------------------------------
void call_recover_sub(void)
{
	if ( pFieldRecoverFunc != NULL ) {
		pFieldRecoverFunc();
	} else {
		AddFieldInTask();
	}
	pFieldRecoverFunc = NULL;
}


//==================================================================================
//
//	�t�B�[���h�J�n���C������
//
//==================================================================================
//--------------------------------------------------------------
//	�t�B�[���h�J�n�F�ʏ�Q�[���X�^�[�g
//--------------------------------------------------------------
void NormalGameStart( void )
{
	Field_clear_intr();
	BGM_PlayStop();

	init_field_mode();
	InitGame();
	InitHeroLastStatus();	//���@�`�Ԃ̏���������
	PlayTimeStart();
	InitFieldScript();
	ResetForceEvent();

	pFieldRecoverFunc = FirstTruckEventStart;
	field_start_init( &sys.InitSeqWork );

	Field_set_intr();
	SetControlFunc( NormalFieldControl );
	MainProcChange( FieldMain );
}

#ifdef	PM_DEBUG
//--------------------------------------------------------------
//	�t�B�[���h�J�n�F�f�o�b�O�Q�[���X�^�[�g
//--------------------------------------------------------------
void DebugGameStart(void)
{
	Field_clear_intr();
	BGM_PlayStop();

	init_field_mode();

	InitHeroLastStatus();	//���@�`�Ԃ̏���������
	PlayTimeStart();
	InitFieldScript();
	ResetForceEvent();

	if ( sys.Cont & R_BUTTON ) {
		pFieldRecoverFunc = FirstTruckEventStart;
	} else {
		pFieldRecoverFunc = AddFieldBlackInTask;
	}
	field_start_init( &sys.InitSeqWork );

	Field_set_intr();
	SetControlFunc( NormalFieldControl );
	MainProcChange( FieldMain );
}
#endif	PM_DEBUG




//--------------------------------------------------------------
//	�t�B�[���h�J�n:�Q�[���I�[�o�[��̍ĊJ
//--------------------------------------------------------------
void GameOverRestart(void)
{
	u8 count;

	sys.InitSeqWork ++;
	if (sys.InitSeqWork < 120)
		return;

	Field_clear_intr();
	BGM_PlayStop();

	init_field_mode();
	RestartGame();
	InitHeroLastStatus();	//���@�`�Ԃ̏���������
	InitFieldScript();
	ResetForceEvent();

	pFieldRecoverFunc = AddFieldBlackInTask;
	count = 0;
	field_start_init(&count);

	Field_set_intr();
	SetControlFunc( NormalFieldControl );
	MainProcChange( FieldMain );
}

//--------------------------------------------------------------
//	�t�B�[���h�J�n�F�}�b�v�J�ڒ���
//--------------------------------------------------------------
extern void ChangeEffectInit(void);

static void FieldMainStart( void );

void FieldMainInit(void)
{
	Field_clear_intr();

	InitFieldScript();
	ResetForceEvent();
	SetControlFunc( NULL );
	MainProcChange( ChangeEffectInit );
	sys.pReturnProc = FieldMainStart;		//	�t�B�[���h�J�n���C������
}


static void FieldMainStart(void)
{
	field_start_init( &sys.InitSeqWork );

	Field_set_intr();
	SetControlFunc( NormalFieldControl );
	MainProcChange( FieldMain );
}


//--------------------------------------------------------------
//	�t�B�[���h�J�n�F�}�b�v�J�ڒ���i�R���e�X�g��p�j
//--------------------------------------------------------------
void FieldContestInit( void )
{
	if ( sys.InitSeqWork == 0 ) {
		Field_clear_intr();

		InitFieldScript();
		ResetForceEvent();
		SetControlFunc( NULL );
	}

	if (make_normal_field(&sys.InitSeqWork, TRUE) == FALSE)
		return;

	Field_set_intr();
	SetControlFunc( NormalFieldControl );
	MainProcChange( FieldMain );
}


//--------------------------------------------------------------
//	�t�B�[���h�J�n�F�}�b�v�J�ڒ���i�ʐM�����p�j
//--------------------------------------------------------------
void FieldMainCommInit(void)
{
	Field_clear_intr();

	pFieldRecoverFunc = AddCommFieldStartTask;
//	pFieldRecoverFunc = AddCommFieldInTask;
	MainProcChange( FieldMainCommStart );
}

static void FieldMainCommStart(void)
{
	if ( make_comm_field( &sys.InitSeqWork ) == FALSE ) return;

	Field_set_intr();
	SetControlFunc( CommFieldControl );
	SetControlCommFunc( comm_normal_control );
	MainProcChange( FieldMain );
}

//==============================================================
//	�t�B�[���h���A���C������
//==============================================================
static void return_normal_field( void );
static void return_comm_field( void );



void FieldMainRecover(void)
{
	if ( IsFieldCommMode() == TRUE ) {
		MainProcChange( return_comm_field );
	} else {
		Field_clear_intr();
		MainProcChange( return_normal_field );
	}
}

//--------------------------------------------------------------
//--------------------------------------------------------------
static void return_normal_field(void)
{

	if ( recover_normal_field( &sys.InitSeqWork ) == FALSE ) return;

	Field_set_intr();
	MainProcChange( FieldMain );
}



//--------------------------------------------------------------
//--------------------------------------------------------------
static void return_comm_field( void )
{
	if ( FieldRecvFifoClearWait() ) return;

	if ( recover_comm_field( &sys.InitSeqWork ) == FALSE ) return;

	MainProcChange( FieldMain );
}

//--------------------------------------------------------------
//	�t�B�[���h���A�F�C�x���g����ʐM������Ԃɕ��A
//--------------------------------------------------------------
void FieldCommRecover(void)
{
	Field_clear_intr();
	BGM_PlayStop();

	clear_all_comm_status();
	SetControlFunc( CommFieldControl );
	SetControlCommFunc( comm_normal_control );
	pFieldRecoverFunc = AddCommFieldInTask;

	InitFieldScript();
	ResetForceEvent();
	
	FieldMainRecover();
}

//--------------------------------------------------------------
//	�t�B�[���h���A�F���j���[�I���ւ̕��A
//--------------------------------------------------------------
void FieldMenuRecover(void)
{
	Field_clear_intr();

	pFieldRecoverFunc = AddFieldMenuInTask;
	FieldMainRecover();
}

//--------------------------------------------------------------
//	�t�B�[���h���A�F�X�N���v�g���ĊJ
//--------------------------------------------------------------
void FieldScriptRecoverBGMNotStart( void )
{
	Field_clear_intr();

	pFieldRecoverFunc = AddScriptFieldInBGMNotStartTask;
	FieldMainRecover();
}

//--------------------------------------------------------------
//	�t�B�[���h���A�F�X�N���v�g���ĊJ
//--------------------------------------------------------------
void FieldScriptRecover( void )
{
	Field_clear_intr();

	pFieldRecoverFunc = AddScriptFieldInTask;
	FieldMainRecover();
}

//--------------------------------------------------------------
//	�t�B�[���h���A�F�Ȃɂ����Ȃ�
//--------------------------------------------------------------
void FieldNormalRecover( void )
{
	Field_clear_intr();

	pFieldRecoverFunc = AddFieldBlackInTask;
	FieldMainRecover();
}

//--------------------------------------------------------------
//	�t�B�[���h���A�F�Z�[�u�f�[�^����R���e�B�j���[
//--------------------------------------------------------------
static void continue_recover_func( void )
{
	if ( NowMap.disp_sw == 1 && BaseNameDraw() == TRUE ) {
		DispMapName();
	}
	AddFieldBlackInTask();
}

void ContinueGameStart(void)
{
	Field_clear_intr();
	BGM_PlayStop();

#ifdef	PM_DEBUG
	if ( sys.Cont & R_BUTTON ) DebugMainFlag = ON;
#endif

	init_field_mode();

	RecoverNowMapData();		//NowMap�̃��J�o������
	SxyObjDataRecover();		//�o�C�i���A�h���X����h�~
	EvObjPauseClrAll();			//�Z�[�u���̃|�[�Y��������

	UpdateTimeEvent();				//���ԃC�x���g�t���O����
	InitEventFlagAtContnue();		//�t���O�E���[�N�̍X�V

	MakeContinueVirtualMap();

	PlayTimeStart();
	InitFieldScript();
	ResetForceEvent();

	if ( CheckContinueStatus() == 1 ) {
		InitContinueStatus();
		NextMapSetByContinueMapID();
		SetNextWorldPosition();
		MainProcChange( FieldMainInit );
		return;
	}

	pFieldRecoverFunc = continue_recover_func;

	SetControlFunc( NormalFieldControl );
	FieldMainRecover();
}


//==================================================================================
//                    ���荞�݃��[�`��
//==================================================================================

static void Field_v_blank_intr(void);

//--------------------------------------------------------------
//		���荞�݋��̐ݒ�
//--------------------------------------------------------------
static void Field_clear_intr(void)
{
	REG_IE_CHG( V_BLANK_INTR_FLAG, H_BLANK_INTR_FLAG );
	SetVBlankFunc(NULL);
	SetHBlankFunc(NULL);
}

static void Field_set_intr(void)
{
	SetVBlankFunc( Field_v_blank_intr );
}

//--------------------------------------------------------------
//                    �u�u�����N�֐�
//--------------------------------------------------------------
static void Field_v_blank_intr(void)
{
		actOAMtrans();
		actDMAtrans();
		
		LasterVblank();
		MapBGVsync();

		PaletteFadeTrans();
		BGanimeDMAtrans();
}


//--------------------------------------------------------------
//					H-DMA�֘A
//--------------------------------------------------------------

static const LasterData FieldHLaster = {
	(void *)REG_WIN0H,		//�ް��]������ڽ(REG_BG0VOFS��)
	H_DMA_ENABLE16,			//32�r�b�g�]��
	LASTER_NORMAL,			//�]���^�C�v�i0:���N�G�X�g�Ȃ�1:�m�[�}��2:���[�v�^�j
	0						//�]���^�C�v�����[�v�̎��̃J�E���g
};

static void init_laster( void )
{
	int r;
	r = GetBright();
	if ( r != 0 ) {
		MakeFieldLasterData(r);
		LasterDmaSet( FieldHLaster );
	}
}


//==================================================================================
//                      ���������[�`��
//==================================================================================

//--------------------------------------------------------------
//	�ʐM���̃}�b�v�J�ڒ���̏�����
//--------------------------------------------------------------
static int make_comm_field( u8 * seq )
{
	switch ( *seq ) {
	case 0:
		Field_clear_intr();
		InitFieldScript();
		ResetForceEvent();
		clear_all_comm_status();
		FieldInitScreen();
		(*seq)++;break;

	case 1:
		FieldInitData(TRUE);
		(*seq)++;break;

	case 2:
		FieldInitControlData(TRUE);
		(*seq)++;break;

	case 3:
		init_comm_info();
		init_comm_obj();
		init_comm_player();
		init_comm_scroll();
		(*seq)++;break;

	case 4:
	/*	FieldSetScreen(); */
		init_laster();							//���X�^�[�iHDMA)�ݒ�
		set_disp_register();					//�\���֘A���W�X�^�ݒ�
		PrintScreenInit(DEFAULT_TYPE);			//MSG�\���ݒ�
		NMenuScreenInit(FIELD_MENU_TYPE);		//���j���[�V�X�e���ݒ�
		(*seq)++;break;

	case 5:
	/*	InitNowMapVisual(); */
		InitMapBG();				// BG ������( ���W,��аVram ������ )
		(*seq)++;break;
	case 6:
		TransMapChar1stData(NowMap.Screen);
		(*seq)++;break;
	case 7:
		TransMapChar2ndData(NowMap.Screen);
		(*seq)++;break;
	case 8:
		TransMapPaletteData(NowMap.Screen);
		(*seq)++;break;
	case 9:
		MakeMapBG();						// ��аVram �W�J
		(*seq)++;break;
	case 10:
		MapBGanimeInit();
		(*seq)++;break;

	case 11:
		DEBUG_SIO_PRINT_INIT();
		(*seq)++;break;

	case 12:
		call_recover_sub();
		(*seq)++;break;

	case 13:
		return TRUE;
	}
	return FALSE;
}


//--------------------------------------------------------------
//	�ʏ�̃}�b�v�J�ڒ���̏�����
//	<����>	seq				�V�[�P���X�ێ��̃��[�N�A�h���X
//			is_comm_mode	�ʐM�����ǂ���
//							�i�ʐM�R���e�X�g���̂݁j
//--------------------------------------------------------------
static int make_normal_field(u8 * seq, int is_comm_mode)
{
	switch ( *seq ) {
	case 0:
		Field_clear_intr();
		FieldInitData(is_comm_mode);
		(*seq)++;break;

	case 1:
		FieldInitScreen();
		(*seq)++;break;

	case 2:
		FieldInitControlData(is_comm_mode);
		(*seq)++;break;

	case 3:
		init_normal_obj();
		init_normal_scroll();
		(*seq)++;break;

	case 4:
	/*	FieldSetScreen(); */
		init_laster();							//���X�^�[�iHDMA)�ݒ�
		set_disp_register();					//�\���֘A���W�X�^�ݒ�
		PrintScreenInit(DEFAULT_TYPE);			//MSG�\���ݒ�
		NMenuScreenInit(FIELD_MENU_TYPE);		//���j���[�V�X�e���ݒ�
		(*seq)++;break;

	case 5:
	/*	InitNowMapVisual();						//�}�b�v�\���Ȃ� */
		InitMapBG();
		(*seq)++;break;
	case 6:
		TransMapChar1stData(NowMap.Screen);
		(*seq)++;break;
	case 7:
		TransMapChar2ndData(NowMap.Screen);
		(*seq)++;break;
	case 8:
		TransMapPaletteData(NowMap.Screen);
		(*seq)++;break;
	case 9:
		MakeMapBG();
		(*seq)++;break;
	case 10:
		MapBGanimeInit();
		(*seq)++;break;

	case 11:
		if ( NowMap.disp_sw == 1 && BaseNameDraw() == TRUE ) {
			DispMapName();
		}
		(*seq)++;break;
	case 12:
		call_recover_sub();
		(*seq)++;break;
	case 13:
		return TRUE;
	}

	return FALSE;
}


//--------------------------------------------------------------
//	�ʏ펞�̃t�B�[���h���A������
//--------------------------------------------------------------
static int recover_normal_field( u8 * seq )
{
	switch ( *seq ) {
	case 0:
		FieldInitScreen();
		FieldInitControlData(FALSE);
		recover_normal_obj();
		init_normal_scroll();
		(*seq)++;break;

	case 1:
		FieldSetScreen();
		(*seq)++;break;

	case 2:
		call_recover_sub();
		(*seq)++;break;

	case 3:
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
//	�ʐM���̃t�B�[���h���A������
//--------------------------------------------------------------
static int recover_comm_field( u8 * seq ) {

	switch ( *seq ) {

	case 0:
		Field_clear_intr();
		FieldInitScreen();
		(*seq)++;break;

	case 1:
		FieldInitControlData(TRUE);
		(*seq)++;break;

	case 2:
		recover_comm_player();
		recover_normal_obj();
		recover_comm_scroll();
		(*seq)++;break;

	case 3:
/* static void FieldSetScreen(void) */
		init_laster();							//���X�^�[�iHDMA)�ݒ�
		set_disp_register();					//�\���֘A���W�X�^�ݒ�
		PrintScreenInit(DEFAULT_TYPE);			//MSG�\���ݒ�
		NMenuScreenInit(FIELD_MENU_TYPE);		//���j���[�V�X�e���ݒ�
		(*seq)++;break;

	case 4:
	/*	InitNowMapVisual();						//�}�b�v�\���Ȃ� */
		InitMapBG();				// BG ������( ���W,��аVram ������ )
		(*seq)++;break;
	case 5:
		TransMapChar1stData(NowMap.Screen);
		(*seq)++;break;
	case 6:
		TransMapChar2ndData(NowMap.Screen);
		(*seq)++;break;
	case 7:
		TransMapPaletteData(NowMap.Screen);
		(*seq)++;break;
	case 8:
		MakeMapBG();						// ��аVram �W�J
		(*seq)++;break;
	case 9:
		MapBGanimeInit();
		(*seq)++;break;

	case 10:
		(*seq)++;break;

	case 11:
		DEBUG_SIO_PRINT_INIT();
		(*seq)++;break;
	case 12:
		call_recover_sub();
		(*seq)++;break;
	
	case 13:
		Field_set_intr();
		(*seq)++;
		return TRUE;
	}
	return FALSE;
}


static void field_start_init( u8 * seq )
{
	while ( make_normal_field(seq, FALSE) == FALSE );
}

//--------------------------------------------------------------
// VRAM���\�����W�X�^������
//--------------------------------------------------------------
static void FieldInitScreen( void )
{
    *(vu16 *)REG_DISPCNT =	0;
	LasterInit();
//	DmaStop(0);								//HDMA ������~
	DIV_DMACLEAR(3, 0, PLTT+2,PLTT_SIZE-2, 16);
	DIV_DMACLEAR(3, 0, VRAM,VRAM_SIZE, 16);		// �SVRAM������
	actOAMinit(0, 128);		//����OAM�ر
	actOAMtrans();
}

//--------------------------------------------------------------
// �\�����W�X�^�Z�b�g
//--------------------------------------------------------------
static void FieldSetScreen(void)
{
	init_laster();							//���X�^�[�iHDMA)�ݒ�
	set_disp_register();					//�\���֘A���W�X�^�ݒ�
	PrintScreenInit(DEFAULT_TYPE);			//MSG�\���ݒ�
	NMenuScreenInit(FIELD_MENU_TYPE);		//���j���[�V�X�e���ݒ�
	InitNowMapVisual();						//�}�b�v�\���Ȃ�
}

static void set_disp_register( void )
{
		*(vu16*)REG_MOSAIC	= 0;
		*(vu16*)REG_WININ	= 0x1f1f;
		*(vu16*)REG_WINOUT	= 0x0101;
		*(vu16*)REG_WIN0H	= 0x00ff;
		*(vu16*)REG_WIN0V	= 0x00ff;
		*(vu16*)REG_WIN1H	= 0xffff;
		*(vu16*)REG_WIN1V	= 0xffff;


		*(vu16*)REG_BLDCNT =
			//	BGblend1stValue[MSG_BG_NO] |
				BGblend2ndValue[MAP_UP_BG_NO] |
				BGblend2ndValue[MAP_LOW_BG_NO] |
				BGblend2ndValue[MAP_BTM_BG_NO] |
				BLD_OBJ_2ND | BLD_A_BLEND_MODE;
	
		*(vu16*)REG_BLDALPHA = 0x070d;


		*(RegBGHofs[MSG_BG_NO]) = 0;
		*(RegBGVofs[MSG_BG_NO]) = 0;
		*(RegBGctrl[MSG_BG_NO]) = 0;

		*(RegBGHofs[MAP_UP_BG_NO]) = 0;
		*(RegBGVofs[MAP_UP_BG_NO]) = 0;
		*(RegBGctrl[MAP_UP_BG_NO]) =
				BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_MOS_ON
				| MAP_UP_BG_PRI << BG_PRIORITY_SHIFT
				| MAP_UP_SCRN_BASE << BG_SCREEN_BASE_SHIFT
				| MAP_UP_CHR_BASE << BG_CHAR_BASE_SHIFT
				;

		*(RegBGHofs[MAP_LOW_BG_NO]) = 0;
		*(RegBGVofs[MAP_LOW_BG_NO]) = 0;
		*(RegBGctrl[MAP_LOW_BG_NO]) =
				BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_MOS_ON
				| MAP_LOW_BG_PRI << BG_PRIORITY_SHIFT
				| MAP_LOW_SCRN_BASE << BG_SCREEN_BASE_SHIFT
				| MAP_LOW_CHR_BASE << BG_CHAR_BASE_SHIFT
				;

		*(RegBGHofs[MAP_BTM_BG_NO]) = 0;
		*(RegBGVofs[MAP_BTM_BG_NO]) = 0;
		*(RegBGctrl[MAP_BTM_BG_NO]) =
				BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_MOS_ON
				| MAP_BTM_BG_PRI << BG_PRIORITY_SHIFT
				| MAP_BTM_SCRN_BASE << BG_SCREEN_BASE_SHIFT
				| MAP_BTM_CHR_BASE << BG_CHAR_BASE_SHIFT
				;

    	*(vu16 *)REG_DISPCNT =	DISP_MODE_0
				| DISP_OBJ_HOFF
				| DISP_WIN0_ON
				| DISP_WIN1_ON
				| DISP_OBJ_CHAR_1D_MAP
				| DISP_OBJ_ON
				| BGvisibleSwitch[MAP_UP_BG_NO]
				| BGvisibleSwitch[MAP_LOW_BG_NO]
				| BGvisibleSwitch[MSG_BG_NO]
				| BGvisibleSwitch[MAP_BTM_BG_NO]
				;

};


//--------------------------------------------------------------
// �t�B�[���h�J�n�������ݒ�
//--------------------------------------------------------------

//-------------------------------------------------------------
//
//	�t�B�[���h����V�X�e���̏�����
//
//	�����ۂ̉�ʔ��f�Ȃǂ͂��̒��ł͍s��Ȃ��B
//	�@�����ŏ����������f�[�^�͐���V�X�e����
//	�@�Ăяo�����^�C�~���O�ŊԐړI�ɔ��f����
//
//-------------------------------------------------------------
static void FieldInitControlData(int is_comm_mode)
{
	InitTask();						// �^�X�N�V�X�e��������
	InitActor();					// Actor�V�X�e��������
	PaletteAnimeInit();				// �p���b�g�V�X�e��������
	LasterWorkInit();				// ���X�^�[�V�X�e��������

	InitFieldScroll();				// Scroll���䏉����
	MapViewInit();					// ���E���䏉����

 	EvObjGraphicInit(0);			// �G�֘A������
	FeCodeInit();					// ̨���޴̪�ĺ���
	FieldMsgInit();					// ���b�Z�[�W�E�B���h�E���䏉����
	WeatherInit();
	UpdateFieldWeatherDirect();

	if (is_comm_mode == FALSE) {
		InitMapEffectControl();		// �D�A�苴�Aetc.�̐��䏉����
	}

	InitControlScriptCheck();
}




//-------------------------------------------------------------
//-------------------------------------------------------------
static void init_comm_obj(void)
{
	FieldActOX = 0;
	FieldActOY = 0;
	EvObjInit();									// Ev �p ����������
	/* �ʏ�̎��@�̃Z�b�g�������Ȃ�Ȃ� */
	EvObjAddControl(0,0);							// ̨���ޱ������

	ObjControlScriptCheck();
}

static void init_normal_obj(void)
{
	u16 x,y;
	const HERO_INIT_INFO * first;

	FieldActOX = 0;
	FieldActOY = 0;
	EvObjInit();									// Ev �p ����������

	GetMapCenterPos(&x,&y);
	first = GetHeroFirstStatus();					// ���@��ԏ����擾
	SetJikiActor(x, y, first->site, MyData.my_sex);	// ���@Actor( ���S���W )
	SetHeroMoveRequest(first->form);				// ���@�̌`�Ԃ��w��
	InitHeroLastStatus();							// ���@��ԏ���������

	EvObjAddControl(0,0);							// ̨���ޱ������
	CheckSeedInScreen();							// ���݂̂̐����}�~�`�F�b�N

	ObjControlScriptCheck();
}

static void recover_normal_obj( void )
{
	EvObjRecover(0,0);								// ����� OBJ ���A
	DoorObjSet();
}

static void init_normal_scroll( void )
{
	EvObjScrSwOn(MyMoveState.evobjno);		// ̨���� OBJ ��۰�Ҳ�
	SetFieldScroll(MyMoveState.actno);		// Scroll�Z�b�g(���@)
}

static void init_comm_scroll( void )
{
	SetFieldScroll( GetPlayerActorNumber( comm_my_id ) );		// Scroll�Z�b�g(���@)
}

static void recover_comm_scroll( void )
{
	SetFieldScroll( GetPlayerActorNumber( comm_my_id ) );		// Scroll�Z�b�g(���@)
}

//-------------------------------------------------------------
//	���������̃Z�b�g
//-------------------------------------------------------------
static void init_comm_info( void )
{
	u16 x,y;

	//��ʒ��S�ʒu��ύX
	GetMapCenterPos(&x,&y);
	SetMapCenterPos( x+comm_my_id, y );
}

static void init_comm_player( void )
{
	u16 i;
	u16 x,y;
	GetMapCenterPos(&x,&y);
	x -= comm_my_id;

	for( i=0; i<comm_player_num; i++ ) {
		SetPlayer( i, x+i, y, SioTrainerInfo[i].sex );
		SetPlayerActor( i );
	}

	clear_recv_data(RecvData);
}

//-------------------------------------------------------------
//	���A���̃Z�b�g
//-------------------------------------------------------------
static void recover_comm_player( void )
{
	u16 i;

	for( i=0; i<comm_player_num; i++ ) {
		SetPlayerActor( i );
	}
}




//-------------------------------------------------------------
//-------------------------------------------------------------

extern void AddDebugMenuTask(void);		// �f�o�b�O���j���[�N��

//==================================================================================
//
//	�ʐM���̃t�B�[���h���C������
//
//==================================================================================

enum{
	MPSTAT_NOTHING = 0x80,
	MPSTAT_EVENT,
	MPSTAT_WAIT,
	MPSTAT_EXIT,
};

enum{
	FLD_COMM_START = 0x10,

	FLD_COMM_NOTHING,
	FLD_COMM_MOVE_DOWN,
	FLD_COMM_MOVE_UP,
	FLD_COMM_MOVE_LEFT,
	FLD_COMM_MOVE_RIGHT,
	FLD_COMM_EVENT_WAIT,
	FLD_COMM_EVENT_EXIT,
	FLD_COMM_EVENT_MENU,
	FLD_COMM_EVENT_TALK,
	FLD_COMM_CLEAR_EVENT,
	FLD_COMM_EVENT_RFIFO,
	FLD_COMM_EVENT_SFIFO,
	FLD_COMM_EVENT_CANCEL,

	FLD_COMM_END,

};


typedef struct {
	u8 number;
	u8 is_myself;
	u8 mv_state;
	u8 site;
	EV_POS	pos;
	u16 attr;
}P_STAT;


static u16 conv_cmd_to_HCMD( u16 cmd );

extern const VM_CODE ev_comm_exit[];
extern const VM_CODE ev_comm_exit_check[];
extern const VM_CODE ev_comm_exit_start[];

extern const VM_CODE ev_comm_look_normal_card[];
extern const VM_CODE ev_comm_look_special_card[];
extern const VM_CODE ev_comm_talk_busy[];


static void get_p_stat( int number, int my_id, P_STAT * ps );

static int comm_fifo_event_check( const P_STAT * ps );
static int comm_menu_event_check( const P_STAT * ps );
static const VM_CODE * comm_pos_event_check( const P_STAT * ps );
static int comm_exit_event_check( const P_STAT * ps );
static const VM_CODE * comm_talk_event_check( const P_STAT * ps );
static u16 comm_get_site_by_pos_event( const VM_CODE * scr );

static void comm_fifo_event_start( void );
static void comm_menu_event_start( void );
static void comm_pos_event_start( const VM_CODE * script );
static void comm_exit_event_start( void );
static void comm_talk_event_start( const VM_CODE * script );
static void comm_end_event_start( void );

static u16 get_send_cmd_by_key( int number );


//��ڲ԰���ޯĒP�ʂ̈ړ��ʂ��擾
//s16 GetPlayerMoveAmount( u8 num );
//In: num	; ���@ܰ�����
//Out: s16; ���݂̈ړ���(0~16)




//-------------------------------------------------------------
//-------------------------------------------------------------
static void clear_all_comm_status( void )
{
	int i;
	for (i=0;i<COMM_PLAYER_MAX; i++ ) {
		comm_status[i] = MPSTAT_NOTHING;
	}

	DEBUG_SIO_INFO_INIT();
}
//-------------------------------------------------------------
//-------------------------------------------------------------
static int check_all_comm_status( u16 mode )
{
	int i,member;
	member = comm_player_num;
	for ( i=0; i<member; i++ ) {
		if ( comm_status[i] != mode ) return FALSE;
	}
	return TRUE;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static int check_any_comm_status( u16 mode )
{
	int i,member;
	member = comm_player_num;
	for ( i=0; i<member; i++ ) {
		if ( comm_status[i] == mode ) return TRUE;
	}
	return FALSE;
}




//-------------------------------------------------------------
//-------------------------------------------------------------
static void update_comm_status( int number, u16 data, const P_STAT * ps, u16 * cmd )
{

	if ( comm_status[number] == MPSTAT_NOTHING ) {

		const VM_CODE * scr;
		scr = comm_pos_event_check(ps);
		if ( scr != NULL ) {
			*cmd = comm_get_site_by_pos_event(scr);
			comm_status[number] = MPSTAT_EVENT;
			if ( ps->is_myself ) {
				SetControlCommFunc( comm_event_control );
				comm_pos_event_start( scr );
			}
			return;
		}

		if ( check_any_comm_status( MPSTAT_EXIT ) == TRUE ) {
			comm_status[number] = MPSTAT_EVENT;
			if ( ps->is_myself ) {
				SetControlCommFunc( comm_event_control );
				comm_end_event_start();
			}
			return;
		}

		
		switch ( data ) {
		case FLD_COMM_EVENT_MENU:
			if ( comm_menu_event_check(ps) == FALSE ) break;

			comm_status[number] = MPSTAT_EVENT;
			if ( ps->is_myself ) {
				SetControlCommFunc( comm_event_control );
				comm_menu_event_start();
			}
			break;

		case FLD_COMM_MOVE_DOWN:
			if ( comm_exit_event_check(ps) == TRUE ) {
				comm_status[number] = MPSTAT_EVENT;
				if ( ps->is_myself ) {
					SetControlCommFunc( comm_event_control );
					comm_exit_event_start();
				}
				break;
			}
			break;

		case FLD_COMM_EVENT_TALK:
			scr = comm_talk_event_check(ps);
			if ( scr == NULL ) break;

			comm_status[number] = MPSTAT_EVENT;
			if ( ps->is_myself ) {
				SetControlCommFunc( comm_event_control );
				comm_talk_event_start(scr); 
			}
			break;

		case FLD_COMM_EVENT_RFIFO:
			DEBUG_SIO_INFO_INC(3,number);
			if ( comm_fifo_event_check(ps) == FALSE ) break;

			comm_status[number] = MPSTAT_EVENT;
			if ( ps->is_myself ) {
				SetControlCommFunc( comm_rfifo_event_control );
				comm_fifo_event_start(); 
			}
			break;

		case FLD_COMM_EVENT_SFIFO:
			DEBUG_SIO_INFO_INC(2,number);
			if ( comm_fifo_event_check(ps) == FALSE ) break;

			comm_status[number] = MPSTAT_EVENT;
			if ( ps->is_myself ) {
				SetControlCommFunc( comm_sfifo_event_control );
				comm_fifo_event_start(); 
			}
			break;

		}
	}


	switch ( data ) {

	case FLD_COMM_EVENT_EXIT:
		comm_status[number] = MPSTAT_EXIT;
		break;

	case FLD_COMM_EVENT_WAIT:
		comm_status[number] = MPSTAT_WAIT;
		break;

	case FLD_COMM_CLEAR_EVENT:
		comm_status[number] = MPSTAT_NOTHING;
		if ( ps->is_myself ) {
			SetControlCommFunc( comm_normal_control );
		}
		break;

	case FLD_COMM_EVENT_CANCEL:
		if ( comm_status[number] == MPSTAT_WAIT ) {
			comm_status[number] = MPSTAT_EVENT;
		}
		break;
	}
}


//-------------------------------------------------------------
//-------------------------------------------------------------
static void update_all_comm_control( u16 * Recv, int my_id )
{
	int i;
	u16 code;
	u16 cmd;
	P_STAT p_stat;

	for (i=0;i<COMM_PLAYER_MAX; i++ ) {

		code = Recv[i];
		cmd = HCMD_NOTHING;
		get_p_stat( i, my_id, &p_stat );
		update_comm_status( i, code, &p_stat, &cmd );

		if ( comm_status[i] == MPSTAT_NOTHING ) {
			cmd = conv_cmd_to_HCMD(code);
		}
		UpdatePlayer( i, cmd );
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void set_send_data( u16 cmd )
{
	if ( FLD_COMM_START < cmd && cmd < FLD_COMM_END ) {
		my_command = cmd;
	} else {
		my_command = FLD_COMM_NOTHING;
	}
}

//-------------------------------------------------------------
//	�L�[���͂���R�}���h�𐶐�
//-------------------------------------------------------------
static u16 get_send_cmd_by_key( int number )
{
	if ( sys.Cont & U_KEY ) return FLD_COMM_MOVE_UP;
	if ( sys.Cont & D_KEY ) return FLD_COMM_MOVE_DOWN;
	if ( sys.Cont & L_KEY ) return FLD_COMM_MOVE_LEFT;
	if ( sys.Cont & R_KEY ) return FLD_COMM_MOVE_RIGHT;
	if ( sys.Trg & START_BUTTON ) return FLD_COMM_EVENT_MENU;
	if ( sys.Trg & A_BUTTON ) return FLD_COMM_EVENT_TALK;
	return FLD_COMM_NOTHING;
}

//-------------------------------------------------------------
//	�ʐM�R�[�h���v���C���[����R�[�h�̕ϊ�
//-------------------------------------------------------------

static u16 conv_cmd_to_HCMD( u16 cmd )
{
	switch ( cmd ) {
	case FLD_COMM_MOVE_RIGHT: return HCMD_RIGHT;
	case FLD_COMM_MOVE_LEFT: return HCMD_LEFT;
	case FLD_COMM_MOVE_UP: return HCMD_UP;
	case FLD_COMM_MOVE_DOWN: return HCMD_DOWN;

	default:
		return HCMD_NOTHING;
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void clear_recv_data( u16 * Recv )
{
	int i;
	for ( i=0 ; i<COMM_PLAYER_MAX; i++ ) {
		Recv[i] = FLD_COMM_NOTHING;
	}
}


//==============================================================
//	���䃁�C��
//	�� sys.pFirstProc�ɐݒ肳��Asys.pMainProc����ɌĂ΂��
//==============================================================

void CommFieldControl( void )
{
	int n;
	u16 cmd;

	DEBUG_SIO_INFO_SET();

	n = comm_my_id;

	update_all_comm_control(RecvData, n);

	cmd = comm_ctrl_func(n);

	set_send_data( cmd );

	clear_recv_data(RecvData);
}

//-------------------------------------------------------------
//	�ʐM�����C���i�ʏ�j
//-------------------------------------------------------------
int comm_normal_control( int n )
{
	u16 cmd;

	if ( CheckForceEvent() == TRUE ) {
		cmd = FLD_COMM_NOTHING;
	}
	else if ( M_s.RecvBuf.Size > FLD_RFIFO_CLEAR_VALUE ) {
		cmd = FLD_COMM_EVENT_RFIFO;
	}
	else if ( M_s.SendBuf.Size > FLD_SFIFO_CLEAR_VALUE ) {
		cmd = FLD_COMM_EVENT_SFIFO;
	}
	else {
		cmd = get_send_cmd_by_key(n);
	}

	return( cmd );
}

//-------------------------------------------------------------
//	�ʐM�����C���i�C�x���g���j
//-------------------------------------------------------------
int comm_event_end_control( int n )
{
	return( FLD_COMM_NOTHING );
}

int comm_event_control( int n )
{
	u16 cmd;

	if ( CheckForceEvent() == TRUE ) {
		cmd = FLD_COMM_NOTHING;
	} else {
		cmd = FLD_COMM_CLEAR_EVENT;
		SetControlCommFunc(comm_event_end_control);
	}
	return( cmd );
}

//-------------------------------------------------------------
//	�ʐM�����C���iFIFO������)
//	���V�X�e�����x���̓��삾���C�x���g�Ƃ��Ĉ����Ă���
//-------------------------------------------------------------
//��MFIFO����
int comm_rfifo_event_control( int n )
{
	u16 cmd;

	if ( M_s.RecvBuf.Size > FLD_RFIFO_LIMIT_VALUE ) {
		cmd = FLD_COMM_NOTHING;
	} else {
		cmd = FLD_COMM_CLEAR_EVENT;
		ResetForceEvent();
		SetControlCommFunc(comm_event_end_control);
	}
	return( cmd );
}

//���MFIFO����
int comm_sfifo_event_control( int n )
{
	u16 cmd;

	if ( M_s.SendBuf.Size > FLD_SFIFO_LIMIT_VALUE ) {
		cmd = FLD_COMM_NOTHING;
	} else {
		cmd = FLD_COMM_CLEAR_EVENT;
		ResetForceEvent();
		SetControlCommFunc(comm_event_end_control);
	}
	return( cmd );
}

//-------------------------------------------------------------
//	�ʐM�����C���i�����E�ΐ�etc...�J�n�҂��j
//-------------------------------------------------------------
int comm_start_cancel_control( int n )
{
	return ( FLD_COMM_NOTHING );
}

int comm_start_wait_control( int n )
{
	if ( comm_status[n] == MPSTAT_WAIT ) {
		if ( sys.Trg & B_BUTTON ) {
			SetControlCommFunc( comm_start_cancel_control );
			return FLD_COMM_EVENT_CANCEL;
		}
	}
	return( FLD_COMM_NOTHING );
}

int comm_start_control( int n )
{
	SetControlCommFunc( comm_start_wait_control );
	return( FLD_COMM_EVENT_WAIT );
}

//-------------------------------------------------------------
//	�ʐM�����C���i�I���҂�)
//-------------------------------------------------------------
int comm_exit_end_control( int n )
{
	return( FLD_COMM_NOTHING );
}

int comm_exit_wait_control( int n )
{
	if ( check_all_comm_status( MPSTAT_EXIT ) == TRUE ) {
		StartFieldScript( ev_comm_exit );
		SetControlCommFunc( comm_exit_end_control );
	}
	return( FLD_COMM_NOTHING );
}

int comm_exit_control( int n )
{
	SetControlCommFunc( comm_exit_wait_control );
	return( FLD_COMM_EVENT_EXIT );
}

//==============================================================
//==============================================================
//-------------------------------------------------------------
//�C�x���g�J�n�҂��`�F�b�N
//	<�߂�l>
//	0		�҂����
//	1		�C�x���g�J�n
//	2		�C�x���g�L�����Z���i�o�������ցj
//-------------------------------------------------------------
int CommEventWaitCheck( void )
{
	/* ���ꂩ���O�ɏo�� */
	if ( check_any_comm_status( MPSTAT_EXIT ) == TRUE ) return 2;

	/* �J�n�҂������܂��J�n�R�[�h����M���Ă��Ȃ� */
	if ( comm_ctrl_func == comm_start_wait_control
			&& comm_status[comm_my_id] != MPSTAT_WAIT ) return 0;

	/* �L�����Z���R�[�h����M���� */
	if ( comm_ctrl_func == comm_start_cancel_control
			&& comm_status[comm_my_id] == MPSTAT_EVENT ) return 2;

	return check_all_comm_status( MPSTAT_WAIT );
}

//-------------------------------------------------------------
//	�ʐM�I���`�F�b�N
//	<�߂�l>	TRUE	���ꂩ����߂�������
//				FALSE
//-------------------------------------------------------------
int CommEventEndCheck( void )
{
	return check_any_comm_status( MPSTAT_EXIT );
}

//-------------------------------------------------------------
//�C�x���g�J�n�҂��X�^�[�g
//-------------------------------------------------------------
int CommEventWaitStart( void )
{
	SetControlCommFunc( comm_start_control );
	return 0;
}

//-------------------------------------------------------------
//�C�x���g�J�n�҂��L�����Z��
//-------------------------------------------------------------
int CommEventWaitCancel( void )
{
	SetControlCommFunc( comm_event_control );
	return 0;
}

//-------------------------------------------------------------
//�ʐM�I���X�^�[�g
//-------------------------------------------------------------
int CommExitWaitStart( void )
{
	SetControlCommFunc( comm_exit_control );
	return 0;
}

//==============================================================
//	�ȈՃC�x���g�N���`�F�b�N
//==============================================================
//-------------------------------------------------------------
//	�v���C���[��Ԃ�ݒ�
//-------------------------------------------------------------
static void get_p_stat( int number, int my_id, P_STAT * ps )
{
	s16 x,y;

	ps->number = number;
	ps->is_myself = ( number==my_id ) ? TRUE : FALSE ;
	ps->mv_state = Player[number].mv_status;
	ps->site = GetPlayerSite( number );
	GetPlayerPos( number, &x, &y );
	ps->pos.x = x;
	ps->pos.y = y;
	ps->pos.z = GetPlayerHeight( number );
	ps->attr = GetCellAttribute( x, y );
}



//	FIFO�����C�x���g�`�F�b�N
static int comm_fifo_event_check( const P_STAT * ps )
{
	if ( ps->mv_state == EV_M_END || ps->mv_state == EV_M_OFF ) return TRUE;
	return FALSE;
}

//	���j���[�C�x���g�`�F�b�N
static int comm_menu_event_check( const P_STAT * ps )
{
	if ( ps->mv_state == EV_M_END || ps->mv_state == EV_M_OFF ) return TRUE;
	return FALSE;
}

//	POS�C�x���g�`�F�b�N
static const VM_CODE * comm_pos_event_check( const P_STAT * ps )
{
	if ( ps->mv_state != EV_M_END ) return NULL;
	return EventPosEasyCheck( &(ps->pos) );
}

//	�o���C�x���g�`�F�b�N
static int comm_exit_event_check( const P_STAT * ps )
{
	if ( ps->mv_state == EV_M_END || ps->mv_state == EV_M_OFF ) {
		if ( ATR_ExitMatDownEnableCheck(ps->attr) && ps->site == SITE_DOWN ) {
			return TRUE;
		}
	}
	return FALSE;
}

//	�b�������C�x���g�`�F�b�N
//	��CommSetTalkTrainerInfo�őΏۂ��w�肵�Ă���B����
static const VM_CODE * comm_talk_event_check( const P_STAT * ps )
{
	const VM_CODE * event;
	EV_POS	ev_pos;
	u8 no;

	if ( ps->mv_state != EV_M_OFF && ps->mv_state != EV_M_END ) return NULL;
	ev_pos = ps->pos;
	ev_pos.x += SiteVector[ps->site].dx;
	ev_pos.y += SiteVector[ps->site].dy;
	ev_pos.z = 0;

	no = CheckTalkPlayer(ev_pos.x,ev_pos.y);
	if ( no != COMM_PLAYER_MAX ) {
		if (ps->is_myself == FALSE) {
			//�Ȃ�ł��������ǎ�������Ȃ��̂�OK�T�C���Ƃ��ăX�N���v�g��Ԃ�
			return ev_comm_talk_busy;
		}
		else if ( comm_status[no] != MPSTAT_NOTHING ) {
			//��荞�ݒ�
			return ev_comm_talk_busy;
		}
		else if ( CommSetTalkTrainerInfo(no) == 0 ) {
			//�P�Ȃ�J�[�h
			return ev_comm_look_normal_card;
		}
		else {
			//�����N�t���J�[�h���I
			return ev_comm_look_special_card;
		}
	}

	event = EvObjEasyTalkCheck(&ev_pos,ps->attr,ps->site);

	return event;
}

//==============================================================
//	POS�C�x���g�̎�ނŋ����I�Ɍ�������������擾����
//==============================================================
extern const u8	ev_comm_battle0_start[];
extern const u8	ev_comm_battle1_start[];
extern const u8	ev_comm_trade0_start[];
extern const u8	ev_comm_trade1_start[];
extern const u8	ev_comm_record0_start[];
extern const u8	ev_comm_record2_start[];
extern const u8	ev_comm_record1_start[];
extern const u8	ev_comm_record3_start[];
extern const u8	ev_comm_multi_battle0_start[];
extern const u8	ev_comm_multi_battle2_start[];
extern const u8	ev_comm_multi_battle1_start[];
extern const u8	ev_comm_multi_battle3_start[];

static u16 comm_get_site_by_pos_event( const VM_CODE * scr )
{
	if (scr == ev_comm_multi_battle0_start)	return HCMD_RIGHT_SITE;
	if (scr == ev_comm_multi_battle1_start)	return HCMD_LEFT_SITE;
	if (scr == ev_comm_multi_battle2_start)	return HCMD_RIGHT_SITE;
	if (scr == ev_comm_multi_battle3_start)	return HCMD_LEFT_SITE;

	if (scr == ev_comm_record0_start)		return HCMD_RIGHT_SITE;
	if (scr == ev_comm_record1_start)		return HCMD_LEFT_SITE;
	if (scr == ev_comm_record2_start)		return HCMD_RIGHT_SITE;
	if (scr == ev_comm_record3_start)		return HCMD_LEFT_SITE;

	if (scr == ev_comm_battle0_start)		return HCMD_RIGHT_SITE;
	if (scr == ev_comm_battle1_start)		return HCMD_LEFT_SITE;

	if (scr == ev_comm_trade0_start)		return HCMD_RIGHT_SITE;
	if (scr == ev_comm_trade1_start)		return HCMD_LEFT_SITE;

	return HCMD_NOTHING;
}

//==============================================================
//	�ʐM���C�x���g�N��
//==============================================================
// FIFO�����C�x���g
static void comm_fifo_event_start( void )
{
	SetForceEvent();
}

// ���j���[�C�x���g
static void comm_menu_event_start( void )
{
	SePlay(SE_WIN_OPEN);
	StartFieldMenuEvent();
	SetForceEvent();
}

// POS�C�x���g
static void comm_pos_event_start( const VM_CODE * script )
{
	SePlay( SE_SELECT );
	StartFieldScript( script );
	SetForceEvent();
}

//�o���C�x���g
static void comm_exit_event_start( void )
{
	SePlay(SE_WIN_OPEN);
	StartFieldScript( ev_comm_exit_check );
	SetForceEvent();
}

//�b�������C�x���g
static void comm_talk_event_start( const VM_CODE * script )
{
	SePlay( SE_SELECT );
	StartFieldScript( script );
	SetForceEvent();
}

//�I���C�x���g
static void comm_end_event_start( void )
{
	StartFieldScript( ev_comm_exit_start );
	SetForceEvent();
}


//==============================================================
//
//	FIFO�����\���ǂ����̃`�F�b�N
//
//==============================================================
extern int IsMultiSioFieldSyncMode(void);	//cable2.c

//-------------------------------------------------------------
//��MFIFO�̉����҂�
//<�߂�l>	TRUE	�����҂���
//			FALSE	�����҂��I��
//-------------------------------------------------------------
int FieldRecvFifoClearWait( void )
{
	if ( IsFieldCommMode() == FALSE ) return FALSE;

	if ( M_s.RecvBuf.Size > FLD_RFIFO_LIMIT_VALUE ) {
		field_recv_fifo_clear_flag = TRUE;
	} else {
		field_recv_fifo_clear_flag = FALSE;
	}
	return field_recv_fifo_clear_flag;
}

//-------------------------------------------------------------
//��MFIFO�̉����������邩?
//-------------------------------------------------------------
int IsEnableClearRecvFifo( void )
{
	if ( IsFieldCommMode() != TRUE ) return FALSE;
	if ( IsMultiSioFieldSyncMode() != TRUE ) return FALSE;

	// FIFO�������[�h���I
	if ( comm_ctrl_func == comm_rfifo_event_control ) return TRUE;

	// �C�x���g��
	if ( comm_ctrl_func == comm_event_control ) {
		u8 flag = field_recv_fifo_clear_flag;
		field_recv_fifo_clear_flag = FALSE;
		if ( flag == TRUE ) {
			return TRUE;
		}

		if ( FadeData.fade_sw == 1 && FadeData.fade_end == 1 ) {
			return TRUE;
		}
	}

	return FALSE;
}

//-------------------------------------------------------------
//���MFIFO�̉����������邩?
//-------------------------------------------------------------
int IsEnableClearSendFifo( void )
{
	if ( IsFieldCommMode() != TRUE ) return FALSE;
	if ( IsMultiSioFieldSyncMode() != TRUE ) return FALSE;

	// FIFO�������[�h���I
	if ( comm_ctrl_func == comm_sfifo_event_control ) {
		return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------
//���C�������̃X�L�b�v�������邩?
//-------------------------------------------------------------
int IsEnableSkipFieldMain( void )
{
	if ( IsMultiSioFieldSyncMode() == FALSE ) return FALSE;
	DEBUG_SIO_INFO_INC(1,comm_my_id);
	return TRUE;
}

