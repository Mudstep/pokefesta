//=========================================================================
//
//		�ʐM��t�֘A
//
//	2001.10.29 GAME FREAK Inc.
//
//=========================================================================

#ifdef  PM_DEBUG
#define DEBUG_NUMPRINT
#endif
#include "common.h"
#include "task.h"
#include "fld_task.h"
#include "script.h"
#include "multisio.h"
#include "pokesio.h"
#include "cable.h"
#include "fight.h"
#include "weather.h"
#include "palanm.h"
#include "fld_main.h"
#include "fld_tool.h"
#include "report.h"
#include "sxytype.h"
#include "print.h"
#include "menutool.h"
#include "syswork.h"
#include "fld_talk.h"
#include "fan.h"
#include "mus_tool.h"
#include "pokescr.h"
#include "saveload.h"
#include "mystatus.h"
#include "score.h"			//UpdateTaisenScore
#include "pokelist.h"		// 2002/05/25 by nakahiro
#include "mystatus.h"
#include "poketool.h"

#define __EV_COMM_H_GLOBAL__
#include "ev_comm.h"

#ifdef PM_DEBUG
u32 AutoKeyTryNum;
u32 AutoResultNum;
u8   GetSioCheckFlag();
u16  VirtualKey;
#endif

enum {
	//�ʐM��t�̎Q���l����`
	
	MAX_PLAYER_MULTI_BATTLE = 4,
	MIN_PLAYER_MULTI_BATTLE = 4,

	MAX_PLAYER_SINGLE_BATTLE = 2,
	MIN_PLAYER_SINGLE_BATTLE = 2,

	MAX_PLAYER_DOUBLE_BATTLE = 2,
	MIN_PLAYER_DOUBLE_BATTLE = 2,

	MAX_PLAYER_RECORD = 4,
	MIN_PLAYER_RECORD = 2,

	MAX_PLAYER_TRADE	= 2,
	MIN_PLAYER_TRADE	= 2,

	MAX_PLAYER_BLEND = 4,
	MIN_PLAYER_BLEND = 2,

	MAX_PLAYER_CONTEST = 4,
	MIN_PLAYER_CONTEST = 4,
};

enum {
	//SIO�������㤎��ۂ̐ڑ��m�F���͂��߂�܂ł̃E�F�C�g
	SIO_CONNECT_WAIT	= 10,
	OYA_TIMEOUT_COUNT   = 10*60,
};

extern void InitFight(void);
extern void PokeAllKaifuku(void);

extern void MultiSioFieldSyncStart(void);
extern u8 SendPoolBuf[];

#ifdef PM_DEBUG
void AddAutoKeyTask();
#endif

//=========================================================================
//
//	�ʐM��t����
//
//=========================================================================


//=========================================================================
//  
//=========================================================================

static void sio_connect_init_task(u8 no);
static void sio_connect_next_task(u8 no);
static void sio_connect_msgwait_oya_task(u8 no);
static void sio_connect_keywait_task(u8 no);
static void sio_connect_wait_oya_task(u8 no);
static void sio_connect_ask_oya_task(u8 no);
static void sio_connect_wait_ko_task(u8 no);
static void sio_connect_end_wait_task(u8 no);
static void sio_connect_oya_end_start(u8 no);

static void sio_connect_end_task(u8 no);
static void sio_connect_cancel_task(u8 no);
static void sio_connect_error_task(u8 no);
static u8 sio_connect_oya_timeout(u8 no);

static int sio_start_cancel_check( u8 no );
static int sio_start_cancel_check2( u8 no );

extern const u8 msg_reception_wait_p01[];
extern const u8 msg_reception_wait_p02[];
extern const u8 msg_reception_wait_p03[];
extern const u8 msg_reception_wait_p04[];


enum{
	WK_CONNECT_WAIT = 0,
	WK_CONNECT_MIN,
	WK_CONNECT_MAX,
	WK_CONNECT_NOW,
	WK_CONNECT_TIMEOUT,
};

enum{
//	�w�`�ɂ�@�������x�̕\���֘A��`

	SIONUMWIN_X = 18,
	SIONUMWIN_Y = 10,
	SIONUMWIN_X_SIZE = 11,
	SIONUMWIN_Y_SIZE = 4,

	SIONUMWIN_X2 = SIONUMWIN_X+SIONUMWIN_X_SIZE-1,
	SIONUMWIN_Y2 = SIONUMWIN_Y+SIONUMWIN_Y_SIZE-1,

	SIONUMWIN_MSGX	= SIONUMWIN_X + 2,
	SIONUMWIN_MSGY	= SIONUMWIN_Y + 1,
};

static const u8 msg_number_win[] = { I_MSG_,STR_TEMP_BUF0,ni_,n_,spc_,se_,tu_,zo_,ku_,EOM_ };


extern void (*PokemonMultiSioFunc)();
extern  u8	MultiSioInitialFlag[];

#ifdef	PM_DEBUG
static u8 SearchNowTask( pTaskFunc adrs)
{
	if( adrs == sio_connect_next_task 			) return 0x01;
	if( adrs == sio_connect_msgwait_oya_task	) return 0x11;
	if( adrs == sio_connect_keywait_task		) return 0x12;
	if( adrs == sio_connect_wait_oya_task		) return 0x13;
	if( adrs == sio_connect_ask_oya_task		) return 0x14;
	if( adrs == sio_connect_wait_ko_task		) return 0x21;
	if( adrs == sio_connect_end_wait_task		) return 0x02;
	if( adrs == sio_connect_end_task			) return 0x03;

	return 0;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
static void debug_connect_watch_task(u8 no)
{
	int i;

	if (TaskTable[TaskTable[no].work[0]].occ == 0) {
		if(TaskTable[no].work[1]==5){
			DelTask(no);
		}
		TaskTable[no].work[1]++;
//		return;
	}
	numprint(Signal, 		2,	0,	2);							//�ʐM�h���C�o�[�V�O�i��

	numprint(SendPoolBuf[0],22,5,4);							//�������M�o�b�t�@
	for(i=0;i<4;i++){
		numprint(MultiSioInitialFlag[i], 5+i*2,0,1);			//�ʐM�F�؃f�[�^�󂯎��t���O
		numprint(RecvPoolBuf[i][0],22,6+i,4);					//������M�o�b�t�@
	}

	numprint(SioFlags, 		15,	0,	8);							//�ʐM�X�e�[�^�X
	numprint(M_s.State, 	2,	10,	2);							//�ʐM�h���C�o�e�q�t���O

	numprint(GetSioConnectId(),  7, 12, 2);						//�ʐM�h�c
	numprint(SplitDataResult(),  7, 10, 2);						//�����f�[�^�󂯎��I���r�b�g�f�[�^
	numprint(pokemon_sio_flag,	 2, 12, 1);						//�|�P�����ʐM�F�؍ς݃t���O
	numprint(AnswerWork, 11, 8, 2);								//�X�N���v�g�Ԃ�l���[�N
																//�ڑ��l��
	numprint(((SioFlags & MULTI_SIO_PLAYERS_NUM_MASK) >> MULTI_SIO_PLAYERS_NUM_SHIFT), 11,10,2);
																//�ʐM�h���C�o�V�[�P���X
	numprint(MultiSioMainCheck(),													   11,12,1);
																//�������M���t���O
	numprint(SplitDataSendStatus(),														15,10,1);
																//�ʐM��t�^�X�N�̌��݂̏��
	numprint(SearchNowTask(TaskTable[TaskTable[no].work[0]].TaskAdrs),				   15,12,2);
	numprint((u32)PokemonMultiSioFunc, 2,13,8);					//�ʐM�p�֐��|�C���^

	numprint(MultiSioErrorCheck(),	2, 2,1);					//�ʐM�G���[�t���O

	for (i=0; i<4; i++) {
		numprint(SioTrainerInfo[i].ninshou,	2+i*6,3,4);			//�V�[���F�ؔԍ�
	}
	numprint(*(vu16*)REG_SIOCNT,	2,6,4);						//�ʐM���W�X�^


	numprint(GetSioCheckFlag(),         25,3,1);					//�g���C��
}
#endif

//-------------------------------------------------------------
//	�ʐM��t����̐ڑ������^�X�N���X�^�[�g����
//
//<����>	min		�Œ�̎Q���l��
//			max		�ő�̎Q���l��
//-------------------------------------------------------------
static void add_sio_connect_task( u8 min, u8 max )
{
	u8 no;

#ifdef PM_DEBUG
	u8 no2;

	DebugNumPrintInit2(
		12,					//palette
		MSG_BG_NO,			//BG
		MSG_SCRN_BASE,		//scrn offset
		MSG_CHR_BASE		//chr offset
	);
#endif

	no = CheckTaskNo(sio_connect_init_task);
	if (no != NOT_EXIST_TASK_ID) return;

	no = AddTask(sio_connect_init_task, TSK_PRI_EFFECT);
	TaskTable[no].work[WK_CONNECT_MIN] = min;
	TaskTable[no].work[WK_CONNECT_MAX] = max;

#ifdef	PM_DEBUG
	no2 = AddTask(debug_connect_watch_task, TSK_PRI_EFFECT);
	TaskTable[no2].work[0] = no;
#endif
}

//-------------------------------------------------------------
//	�w�`�ɂ�@�������x�̃E�B���h�E����
//-------------------------------------------------------------
//�\��
static void put_connect_number_win(int number)
{
	PM_NumMsgSet(StrTempBuffer0, number, NUM_MODE_LEFT, 1);
	NMenuWinBoxWrite(SIONUMWIN_X, SIONUMWIN_Y, SIONUMWIN_X2, SIONUMWIN_Y2);
	NMenuMsgWrite(msg_number_win, SIONUMWIN_MSGX, SIONUMWIN_MSGY);
}

//����
static void clear_connect_number_win( void )
{
	NMenuBoxClear(SIONUMWIN_X, SIONUMWIN_Y, SIONUMWIN_X2, SIONUMWIN_Y2);
}

//�X�V
static void update_connect_info(u8 task_no, u8 now)
{
	s16 * wk;
	wk = &TaskTable[task_no].work[WK_CONNECT_NOW];

	//�ڑ��l���ɕϓ������������ʕ\���ύX
	if (now != *wk) {
		if (now < 2) {
			//�l�����O�`�P�̏ꍇ��E�B���h�E�͕\�����Ȃ�
			clear_connect_number_win();
		} else {
			put_connect_number_win(now);
		}
		*wk = now;
	}
}

//-------------------------------------------------------------
//	�ڑ��`�F�b�N
//-------------------------------------------------------------
static int sio_start_check( u8 min, u8 max )
{
	int num;
	
	switch( PokemonMultiSioCheck() ) {

	case 1:
		num = GetSioRealConnectNumber();
		if ( min <= num && num <= max ) {
			return COMM_RES_OK;			//�ڑ��n�j
		} else {
			PM_NumMsgSet( StrTempBuffer0, num, NUM_MODE_LEFT, 1);
			return COMM_RES_WRONG_NUM;	//�ڑ��l�����Ⴄ
		}

	case 2:
		return COMM_RES_WAIT;			//�^�C���A�E�g�����������Đڑ��҂�

	case 3:
		return COMM_RES_OTHERMODE;		//�ʐM���[�h���Ⴄ

	default:
		return COMM_RES_WAIT;			//�ڑ��҂�
	}
}

//-------------------------------------------------------------
//	�G���[�����̃`�F�b�N
//-------------------------------------------------------------
static int sio_error_connect_check(u8 no)
{
	if (MultiSioErrorCheck() == 1) {
		TaskTable[no].TaskAdrs = sio_connect_error_task;
		return TRUE;
	} else {
		return FALSE;
	}
}

//-------------------------------------------------------------
//	�a�{�^���L�����Z���`�F�b�N���L�����Z������
//-------------------------------------------------------------
static int sio_start_cancel_check( u8 no )
{
	if ( sys.Trg & B_BUTTON ) {
		if(MultiSioMainCheck()==0){
			TaskTable[no].TaskAdrs = sio_connect_cancel_task;
			return TRUE;
		}else{
			return FALSE;
		}
	} else {
		return FALSE;
	}
}

//-------------------------------------------------------------
//	�a�{�^���L�����Z���`�F�b�N���L�����Z������
//-------------------------------------------------------------
static int sio_start_cancel_check2( u8 no )
{
	if(MultiSioMainCheck())							//connect_next_task�ŒʐM�����C���t���[�ɓ����Ă�����
		MultiSioErrorPermit(SIO_ERROR_NO_JUMP);		//�ُ�ȏ�ԁB�G���[��ʂɔ�΂��Ȃ��悤�ɂ���

	if ( sys.Trg & B_BUTTON ) {
		TaskTable[no].TaskAdrs = sio_connect_cancel_task;
		return TRUE;
	} else {
		return FALSE;
	}
}

//-------------------------------------------------------------
//	�e�̏ꍇ�̃R�l�N�^�`�F�b�N
//-------------------------------------------------------------
static int sio_parant_connect_check(u8 no)
{
	if (SioParentConnectCheck() == 1) {
		TaskTable[no].TaskAdrs = sio_connect_error_task;
		return TRUE;
	} else {
		return FALSE;
	}
}


void LaterSioExchangeStart(u8 no)
{
	TaskTable[no].work[0]++;
	if(TaskTable[no].work[0]==10){
		SplitDataExchangeStart(2);
		DelTask(no);
	}
}
//-------------------------------------------------------------
//-------------------------------------------------------------

//���������E�F�C�g
static void sio_connect_init_task(u8 no)
{
	s16 * wk = TaskTable[no].work;

	if ( wk[0] == 0 ) {
		PokemonMultiSioCheckInit();
		ClearSioReliableNumber();
		ClearSioTrainerInfo();
	}
	else if ( wk[0] >= SIO_CONNECT_WAIT ) {
		TaskTable[no].TaskAdrs = sio_connect_next_task;
	}
	wk[0]++;
}

// �e���q���ŕ���
static void sio_connect_next_task(u8 no)
{
	u16 real_num;

	real_num = GetSioRealConnectNumber();

	if ( sio_start_cancel_check(no)  == TRUE ) return;			//�������͒ʏ�̐ڑ��ҋ@��Ԃ̎��̃L�����Z������
	if ( sio_start_cancel_check2(no) == TRUE ) return;			//�������͂��łɈُ�ȏ�ԁi�����ʐM���C���Ƀt���[���ڂ��Ă���)

	if ( real_num > 1 ) {
		//�G���[�ł�����̋����ύX�͂Ȃ�
		MultiSioErrorPermit(SIO_ERROR_NO_JUMP);

		TaskTable[no].work[WK_CONNECT_NOW] = 0;

		if ( GetMyConnectIsParent() == 1 ) {
			SePlay( SE_PIN );
			FieldMsgSetNoSkip( msg_reception_wait_p02 );
			TaskTable[no].TaskAdrs = sio_connect_msgwait_oya_task;
		} else {
			SePlay( SE_BOO );
			FieldMsgSetNoSkip( msg_reception_wait_p03 );
//			TaskTable[no].TaskAdrs = sio_connect_msgwait_ko_task;
/*---��������--2002/09/17 by Mori----*/
			TaskTable[no].TaskAdrs = sio_connect_wait_ko_task;
/*---�����܂�--------*/
		}
	}
}

//���b�Z�[�W�\���҂��i�e�j
static void sio_connect_msgwait_oya_task(u8 no)
{
	if ( sio_start_cancel_check(no) == TRUE ) return;

	if (sio_parant_connect_check(no) == TRUE) return;

	if (sio_error_connect_check(no) == TRUE) return;

	if ( FieldMsgStatusCheck() == FLDMSG_STAT_OFF ) {
		TaskTable[no].work[WK_CONNECT_NOW] = 0;
		TaskTable[no].TaskAdrs = sio_connect_keywait_task;
	}
}

// �`�{�^���ŒʐM�J�n�i���e�̂݁j
static void sio_connect_keywait_task(u8 no)
{
	u16 real_num;
	s16 * twk;

	twk = TaskTable[no].work;
	real_num = GetSioRealConnectNumber();

	if (sio_start_cancel_check(no) == TRUE) return;

	if (sio_parant_connect_check(no) == TRUE) return;

	if (sio_error_connect_check(no) == TRUE) return;

	//�ڑ��l���ɕϓ������������ʕ\���ύX
	update_connect_info(no, real_num);

	if ((sys.Trg & A_BUTTON) == 0) return;
	if (real_num >= twk[WK_CONNECT_MIN]) {
		SetSioReliableNumber(real_num);
		clear_connect_number_win();
		PM_NumMsgSet(StrTempBuffer0, real_num, NUM_MODE_LEFT, 1);
		FieldMsgSetNoSkip( msg_reception_wait_p04 );
		TaskTable[no].TaskAdrs = sio_connect_ask_oya_task;
//		SePlay(SE_SELECT);
//		MultiSioSignalDecision();
//		TaskTable[no].TaskAdrs = sio_connect_wait_oya_task;
	}
}

//�m�F�l�r�f�\��������҂�
static void sio_connect_ask_oya_task(u8 no)
{
	if (sio_start_cancel_check(no) == TRUE) return;
	if (sio_parant_connect_check(no) == TRUE) return;
	if (sio_error_connect_check(no) == TRUE) return;

	if (FieldMsgStatusCheck() != FLDMSG_STAT_OFF) return;

	if (GetSioReliableNumber() != GetSioRealConnectNumber()) {
		//�l�����ϓ������ꍇ
		FieldMsgSetNoSkip( msg_reception_wait_p02 );
		TaskTable[no].TaskAdrs = sio_connect_msgwait_oya_task;
		return;
	}
	if (sys.Cont & B_BUTTON) {
		//�a�{�^���L�����Z���̏ꍇ
		FieldMsgSetNoSkip( msg_reception_wait_p02 );
		TaskTable[no].TaskAdrs = sio_connect_msgwait_oya_task;
		return;
	}
	if (sys.Cont & A_BUTTON) {
		//�`�{�^���Ō��肵���ꍇ
		SePlay(SE_SELECT);
		MultiSioSignalDecision();
		TaskTable[no].TaskAdrs = sio_connect_wait_oya_task;
	}
}

// �F�؏����҂�(�e)
static void sio_connect_wait_oya_task(u8 no)
{
	u8 min,max;

	min = TaskTable[no].work[WK_CONNECT_MIN];
	max = TaskTable[no].work[WK_CONNECT_MAX];

//	if ( sio_start_cancel_check(no) == TRUE ) return;
//	if (sio_parant_connect_check(no) == TRUE) return;

	if (sio_error_connect_check(no) == TRUE) return;
	if (sio_connect_oya_timeout(no) == TRUE) return;	//�e�݂̂̃^�C���A�E�g�G���[����

	if (GetSioRealConnectNumber() != GetSioReliableNumber()) {
		//�ʐM�J�n��ł��l�����ϓ�������G���[�ɂ���
		TaskTable[no].TaskAdrs = sio_connect_error_task;
		return;
	}

	AnswerWork = sio_start_check(min,max);								//�ڑ��F�،��ʎ擾
	if ( AnswerWork != COMM_RES_WAIT ) {
		TaskTable[no].TaskAdrs = sio_connect_oya_end_start;
	}
}



// �F�؏����҂�(�q)
static void sio_connect_wait_ko_task(u8 no)
{
	u8 min,max;

	min = TaskTable[no].work[WK_CONNECT_MIN];
	max = TaskTable[no].work[WK_CONNECT_MAX];

	if ( sio_start_cancel_check(no) == TRUE ) return;

//	if (sio_child_connect_check(no) == TRUE) return;

	if (sio_error_connect_check(no) == TRUE) return;

#ifdef PM_DEBUG
	update_connect_info(no, GetSioRealConnectNumber());
#endif

	AnswerWork = sio_start_check(min,max);
	if ( AnswerWork != COMM_RES_WAIT ) {
	if(AnswerWork==COMM_RES_OTHERMODE){
		SetSioEndDataStart();									//�ʐM�I�������J�n
		FieldMsgClear();
		TaskTable[no].TaskAdrs = sio_connect_end_task;
	}else{
		comm_player_num = GetSioRealConnectNumber();
		comm_my_id = GetSioConnectId();
		SetSioReliableNumber(comm_player_num);
		SetTrainerCardWorkPlayer((TRCARD*)SendPoolBuf);			//�g���[�i�[�J�[�h�̏���
		TaskTable[no].TaskAdrs = sio_connect_end_wait_task;
	}
	}
}

// �F�؏����҂��I����̑J��(�e�̂�
static void sio_connect_oya_end_start(u8 no)
{
	if (sio_error_connect_check(no) == TRUE) return;

	if(AnswerWork==COMM_RES_OTHERMODE){
		SetSioEndDataStart();									//�ʐM�I�������J�n
		FieldMsgClear();
		TaskTable[no].TaskAdrs = sio_connect_end_task;
	}else{
		comm_player_num = GetSioRealConnectNumber();
		comm_my_id = GetSioConnectId();
		SetSioReliableNumber(comm_player_num);
		SetTrainerCardWorkPlayer((TRCARD*)SendPoolBuf);			//�g���[�i�[�J�[�h�̏���
		TaskTable[no].TaskAdrs = sio_connect_end_wait_task;
//		AddTask(LaterSioExchangeStart,80);
		SplitDataExchangeStart(2);
	}
}

static void sio_connect_end_wait_task(u8 no)
{
	u8 i;
	TRCARD	*p;

	if (sio_error_connect_check(no) == TRUE) return;

	if((SplitDataResult()==GetConnectMemberBitData())){			//�S���̃f�[�^���͂������H
		for(i=0;i<GetConnectSioMenberNum();i++){
			p = (TRCARD*)RecvPoolBuf[i];						//�g���[�i�[�J�[�h�����R�s�[
			MultiTrainerCard[i] = *p;
		}
		MultiSioErrorPermit(SIO_ERROR_PERMIT_JUMP);
		SplitDataResultReset();									
		FieldMsgClear();											//�u�����񂩂������܂��Ă��܂��v������
		if(AnswerWork==COMM_RES_OK){								//����ɐڑ����H
			if(PokemonSioNinshouNo==NINSHOU_BLEND_ENTER){			//�u�����h�}�V�[���̎���
				clear_connect_number_win();
				ContinueFieldScript();
				DelTask(no);
			}else{
//				SetSioEndDataStart();								//�ʐM�I�������J�n
//				TaskTable[no].TaskAdrs = sio_connect_end_task;
/*---��������--2002/09/17 by Mori----*/
				clear_connect_number_win();
				ContinueFieldScript();
				DelTask(no);
/*---�����܂�------*/
			}
		}else{													//�Ԉ���Ă���
			SetSioEndDataStart();									//�ʐM�I�������J�n
			TaskTable[no].TaskAdrs = sio_connect_end_task;
		}
	}
}	

static void sio_connect_end_task(u8 no)
{
	if(pokemon_sio_flag==0){
		clear_connect_number_win();
		ContinueFieldScript();
		DelTask(no);
	}
}

static void sio_connect_cancel_task(u8 no)
{
	AnswerWork = COMM_RES_CANCEL;
	clear_connect_number_win();
	FieldMsgClear();			//MSG�\�����ɃL�����Z�������Ƃ��̂���
	ContinueFieldScript();
	DelTask(no);
}

static void sio_connect_error_task(u8 no)
{
	AnswerWork = COMM_RES_ERROR;
	clear_connect_number_win();
	FieldMsgClear();			//MSG�\�����ɃG���[�ɂȂ����Ƃ��̂���
	ContinueFieldScript();
	DelTask(no);
}


// =================================================================
// sio_connect_oya_timeout
// �T�v  : �e���ڑ���Ԃɓ������Ƃ��Ɏq�@����̉������Ȃ������^�C���A�E�g�G���[
// ����  : no	taskno
// �߂�l: u8   TRUE=TIMEOUT  FALSE
// =================================================================
static u8 sio_connect_oya_timeout(u8 no)
{
	TaskTable[no].work[WK_CONNECT_TIMEOUT]++;
	if(TaskTable[no].work[WK_CONNECT_TIMEOUT] > OYA_TIMEOUT_COUNT){
		TaskTable[no].TaskAdrs  = sio_connect_error_task;
		return TRUE;
	}
	return FALSE;
}
//-------------------------------------------------------------
//	�ʐM�ΐ�̐ڑ������J�n
//-------------------------------------------------------------
void ConnectBattleSio( void )
{
	u8 min, max;

	//�Ƃ肠�����̃f�t�H���g�l
	min = MIN_PLAYER_SINGLE_BATTLE;
	max = MAX_PLAYER_SINGLE_BATTLE;

	switch ( ScriptParameter0 ) {
	case COMM_ID_1VS1BATTLE:
		min = MIN_PLAYER_SINGLE_BATTLE;
		max = MAX_PLAYER_SINGLE_BATTLE;
		PokemonSioNinshouNo = NINSHOU_BATTLE1VS1;
		break;
	case COMM_ID_2VS2BATTLE:
		min = MIN_PLAYER_DOUBLE_BATTLE;
		max = MAX_PLAYER_DOUBLE_BATTLE;
		PokemonSioNinshouNo = NINSHOU_BATTLE2VS2;
		break;
	case COMM_ID_4BATTLE:
		min = MIN_PLAYER_MULTI_BATTLE;
		max = MAX_PLAYER_MULTI_BATTLE;
		PokemonSioNinshouNo = NINSHOU_BATTLE_MULTI;
		break;
	}
	add_sio_connect_task(min,max);
}

//-------------------------------------------------------------
//	�ʐM�����̐ڑ������J�n
//-------------------------------------------------------------
void ConnectTradeSio( void )
{
	PokemonSioNinshouNo = NINSHOU_KOUKANENTER;
	FightType           = 0;
	add_sio_connect_task(MIN_PLAYER_TRADE, MAX_PLAYER_TRADE);
}

//-------------------------------------------------------------
//	���R�[�h�R�[�i�[�̐ڑ������J�n
//-------------------------------------------------------------
void ConnectRecordSio( void )
{
//	AddAutoKeyTask();
	AnswerWork          = 0;
	PokemonSioNinshouNo = NINSHOU_RECORD_ENTER;
	FightType           = 0;
	add_sio_connect_task(MIN_PLAYER_RECORD, MAX_PLAYER_RECORD);
}

//-------------------------------------------------------------
//	�u�����h�}�V�[���̐ڑ������J�n
//-------------------------------------------------------------
void ConnectBlendSio( void )
{
	PokemonSioNinshouNo = NINSHOU_BLEND_ENTER;
	FightType           = 0;
	add_sio_connect_task(MIN_PLAYER_BLEND, MAX_PLAYER_BLEND);
}

//-------------------------------------------------------------
//	�R���e�X�g�̐ڑ������J�n
//-------------------------------------------------------------
void ConnectContestSio(void)
{
	PokemonSioNinshouNo = NINSHOU_CONTEST;
	FightType           = 0;
	add_sio_connect_task(MIN_PLAYER_CONTEST, MAX_PLAYER_CONTEST);
}

//=========================================================================
//
//	�ʐM�����ɓ��������̔F�؏���
//
//=========================================================================
static void room_ninshou_init_task(u8 no);
static void room_ninshou_next_task(u8 no);
static void room_ninshou_wait_task(u8 no);
static void room_ninshou_check_task(u8 no);

//-------------------------------------------------------------
//-------------------------------------------------------------
u8 AddNinshouRoomTask(void)
{
	if ( CheckTask(room_ninshou_init_task) ) return -1;

	switch ( ScriptParameter0 ) {
	case COMM_ID_1VS1BATTLE:
		PokemonSioNinshouNo = NINSHOU_BATTLE1VS1;
		break;
	case COMM_ID_2VS2BATTLE:
		PokemonSioNinshouNo = NINSHOU_BATTLE2VS2;
		break;
	case COMM_ID_4BATTLE:
		PokemonSioNinshouNo = NINSHOU_BATTLE_MULTI;
		break;
	case COMM_ID_TRADE:
		PokemonSioNinshouNo = NINSHOU_KOUKAN;
		break;
	case COMM_ID_RECORD:
		PokemonSioNinshouNo = NINSHOU_RECORD_CORNER;
		break;
	}
	return AddTask( room_ninshou_init_task, TSK_PRI_EFFECT );
}

//-------------------------------------------------------------
//-------------------------------------------------------------
// ���������E�F�C�g
static void room_ninshou_init_task( u8 no )
{
	s16 * wk = TaskTable[no].work;

	if (wk[0] == 0 ) {
		PokemonMultiSioInit();
		ClearSioTrainerInfo();
		AddTask(JidouNinshouWatchDog, 80);
	}
	else if (wk[0] >= SIO_CONNECT_WAIT) {
		TaskTable[no].TaskAdrs = room_ninshou_next_task;
	}

	wk[0]++;
}

// ID���e���q���ŕ���
static void room_ninshou_next_task( u8 no )
{
	//�Q�l�ȏ�ڑ��ɂȂ�̂�҂�
	if (GetSioRealConnectNumber() <= 1)
		return;

	if (GetMyConnectIsParent() == 1) {
		TaskTable[no].TaskAdrs = room_ninshou_wait_task;
	} else {
		TaskTable[no].TaskAdrs = room_ninshou_check_task;
	}
}

// �l�������낤�̂�҂i���e�̂݁j
static void room_ninshou_wait_task( u8 no )
{
	if ( GetSioReliableNumber() == GetSioRealConnectNumber() ) {
		MultiSioSignalDecision();
		TaskTable[no].TaskAdrs = room_ninshou_check_task;
	}
}

// �F�؏����҂�
static void room_ninshou_check_task( u8 no )
{
	if ( pokemon_sio_flag == 1 && PokemonNinshouNoCheck() == 1 ) {
		CheckMyTrainerInfo();					//�Ȃ������΍�
		MultiSioFieldSyncStart();
		DelTask(no);
	}
}



//=========================================================================
//	�ʐM�O�̃Z�[�u����
//	������_�~�[�ł�
//=========================================================================
void ConnectSave( void )
{
	ReportSequenceStart();
}


#define COMM_BATTLE_START_WAIT (20)
//=========================================================================
//
//	�|�P�����ΐ탂�[�h�ւ̑J��
//
//=========================================================================
static void comm_battle_start_task( u8 no )
{
	TASK_TABLE * my;
	my = &TaskTable[no];
	switch ( my->work[0] ) {
	case 0:
		FieldFadeControl(FIELD_FADE_OUT, 0);
		PokemonSioNinshouNo = NINSHOU_BATTLEROOM;
		MultiSioFunctionClear();	//�ʐM�p�^�X�N���N���A
									//���̌�A�t�F�[�h�A�E�g����INVALID_DATA�𑗎�M����
									//���t�B�[���h���쒆�ɗݐς���FIFO�����������
		my->work[0]++;
		break;
	case 1:
		if( FadeData.fade_sw == 0 )
			my->work[0]++;
		break;
	case 2:							//FIFO�����̂��߂ɃE�F�C�g
		my->work[1]++;
		if(my->work[1] > COMM_BATTLE_START_WAIT) my->work[0]++;
		break;
	case 3:
		SetSioEndDataStart();
		my->work[0]++;
		break;
	case 4:
		if(pokemon_sio_flag==0){
			my->work[0]++;
		}
		break;
	case 5:
		if (SioTrainerInfo[0].Id & 1) {
			FightBGMSet2(MUS_BATTLE32);
		} else {
			FightBGMSet2(MUS_BATTLE20);
		}

		switch ( ScriptParameter0 ) {
		case COMM_ID_1VS1BATTLE:
			FightType = FIGHT_TYPE_TUUSHIN | FIGHT_TYPE_TRAINER | FIGHT_TYPE_1vs1;
			break;
		case COMM_ID_2VS2BATTLE:
			FightType = FIGHT_TYPE_TUUSHIN | FIGHT_TYPE_TRAINER | FIGHT_TYPE_2vs2;
			break;
		case COMM_ID_4BATTLE:
 			SortSelectPokemon();	//�莝���|�P�������ёւ�
			FightType = FIGHT_TYPE_2vs2 | FIGHT_TYPE_TRAINER |
				FIGHT_TYPE_TUUSHIN | FIGHT_TYPE_4_2vs2;
			break;
		}
		MainProcChange( InitFight );
		sys.pReturnProc = CommBattleEnd;
		DelTask(no);
		break;
	}
}

//=========================================================================
//	�|�P�����ΐ탂�[�h����̕��A����
//=========================================================================
void CommBattleEnd( void )
{
	MapBGM_Init();				// BGM No���N���A�A�ŏ������悤��
 
 	RecoverPokeParaMine();		//�莝���|�P�������ǂ�����
 	RecoverTemotiItem();		//�ʐM�J�n�O�̏�Ԃɂ���
 
	FanCommBattleWinLoseChk();	//�u�t�@���v����
	if ( ScriptParameter0 != COMM_ID_4BATTLE ) {
		UpdateTaisenScore( comm_my_id ^ 1 );
	}
	
	sys.pReturnProc = FieldCommRecover;
	MainProcChange(CommBattleSaveInit);
//	MainProcChange( FieldCommRecover );
}

//=========================================================================
//	�ʐM��������o��Ƃ��ɂ�΂�鏈��
//	���߂����Z�b�g�A�ΐ핔���̏ꍇ�̓|�P������ǂ��������ɖ߂�
//=========================================================================
void CommExitSet( void )
{
	if (ScriptParameter0 == COMM_ID_1VS1BATTLE ||
			ScriptParameter0 == COMM_ID_2VS2BATTLE ||
			ScriptParameter0 == COMM_ID_4BATTLE ) {
		RecoverPokeParaMine();		//�莝���|�P�������ǂ�����
		RecoverTemotiItem();		//�ʐM�J�n�O�̏�Ԃɂ���
	}
	NextMapSetBySpID( SPFIELD_MAP_EXIT01 );
}

//=========================================================================
//	�ʐM��������o��Ƃ��ɂ�΂�鏈��
//	���ʐM���̃t�B�[���h���䃁�C�����I���҂���Ԃɐ؂�ւ���
//=========================================================================
void CommExitWait( void )
{
	CommExitWaitStart();
}

//=========================================================================
//
//	�ʐM�C�x���g�J�n����
//	���ʐM�������ł̏���ʒu�ɂ���
//	�@�ʐM�C�x���g�J�n��҂��Ă���Ƃ��̐���
//
//=========================================================================


enum{
	MAIN_START,			//�J�n
	MAIN_STARTMSG_PUT,	//MSG�\���҂����ݒ�
	MAIN_WATCHDOG,		//�S�v���C���[��ԊĎ�
	MAIN_END,			//�I��
};


enum{
	WK_STAT,
	WK_MODE,
};


//-------------------------------------------------------------
//-------------------------------------------------------------
static void CommEventStartTask( u8 no )
{
	TASK_TABLE * my;
	my = &TaskTable[no];

	switch( my->work[WK_STAT] ) {

	case MAIN_START:
		FieldMsgSet( msg_reception_wait_p01 );
		my->work[WK_STAT] = MAIN_STARTMSG_PUT;
		break;

	case MAIN_STARTMSG_PUT:
		if ( FieldMsgWait() == FALSE ) break;

		CommEventWaitStart();		//�ʐM�����V�[�P���X�؂�ւ�
		 /* �����ʒu����ʐM�V�X�e���ɂ킽�� */
		SetMyTrainerInfoStandPos( ScriptParameter1 );
		my->work[WK_STAT] = MAIN_WATCHDOG;
		break;

	case MAIN_WATCHDOG:

		switch ( CommEventWaitCheck() ) {
		case 0:					//�҂����
			break;
		case 1:					//�J�n
			FieldMsgClear();
			my->work[WK_STAT] = 0;
			ChangeBackTask(no);
			break;
		case 2:					//�L�����Z��
			my->work[WK_STAT] = MAIN_END;
			break;
		}
		break;

	case MAIN_END:
		CommEventWaitCancel();		//�ʐM�����V�[�P���X�؂�ւ�
		FieldMsgClear();
		NMenuScreenClear();
		DelTask(no);
		ContinueFieldScript();
		break;
	}
}

//=========================================================================
//=========================================================================
//-------------------------------------------------------------
//	�ʐM�C�x���g�J�n�҂��Z�b�g
//	<����>	func	�J�n���ɐ�����킽���^�X�N
//-------------------------------------------------------------
void AddCommEventStartTask( pTaskFunc func )
{
	u8 no;
	no = AddTask( CommEventStartTask, TSK_PRI_EFFECT );
	ChangeTask( no, CommEventStartTask, func );
	StopFieldScript();
}

extern void KoukanMainInit();
extern u8 exchange_pokemon[];
//=========================================================================
//
//	�|�P�������������ʂւ̑J��
//
//=========================================================================
static void comm_start_task( u8 no )
{
	TASK_TABLE * my;
	my = &TaskTable[no];
	switch( my->work[0] )
	{
	case 0:
		SetForceEvent();
		FieldFadeControl(FIELD_FADE_OUT, 0);
		MultiSioFunctionClear();				//�ʐM�p�^�X�N���N���A
		my->work[0]++;
		break;
	case 1:
		if( FadeData.fade_sw == 0 ) my->work[0]++;
		break;
	case 2:
		exchange_pokemon[0]= 0;
		exchange_pokemon[1] = 0;
		m4aMPlayAllStop();
		SetSioEndDataStart();					//�ʐM�I�����������X�^�[�g
		my->work[0]++;
		break;
	case 3:
		if(pokemon_sio_flag==0){				//�ʐM�I���҂�
			MainProcChange(KoukanMainInit);
			DelTask(no);
		}
		break;
	};
};

//-------------------------------------------------------------
//	�����J�n�C�x���g
//-------------------------------------------------------------
void AddFadeToKoukanEventTask(void);
static void change_to_trade_task(u8 no)
{
	AddFadeToKoukanEventTask();
	DelTask(no);
}

void AddCommTradeStartTask( void )
{
	AddCommEventStartTask( change_to_trade_task );
}

void AddFadeToKoukanEventTask( void )
{
	AddTask( comm_start_task, 80 );
}

//-------------------------------------------------------------------------
//	�����J�n
//-------------------------------------------------------------------------
void ChangeToKoukan( void )
{
	AddFadeToKoukanEventTask();
	StopFieldScript();
}

//-------------------------------------------------------------
//	�ΐ�J�n�C�x���g
//-------------------------------------------------------------

//�ʏ�̑ΐ�J�n�C�x���g
void AddCommBattleStartTask( void )
{
	PokemonSioNinshouNo = NINSHOU_BATTLEROOM;
	AddCommEventStartTask( comm_battle_start_task );
}


//-------------------------------------------------------------
//	���R�[�h�R�[�i�[�J�n�C�x���g
//-------------------------------------------------------------
extern void CommMixMainTask(u8);
void AddCommRecordStartTask( void )
{
	u8 no;
	no = AddTask( CommEventStartTask, TSK_PRI_EFFECT );
	ChangeTask( no, CommEventStartTask, CommMixMainTask );
	StopFieldScript();
}

//-------------------------------------------------------------
//	�g���[�i�[�J�[�h�̕\��
//-------------------------------------------------------------
void CommTrainerCard( void )
{
	SetTrainerCardFriend( ScriptParameter2, FieldScriptRecover );
}

static const u8 rank_string[][5] = {
	{ BU_,RO_,N_,ZU_,EOM_ },
	{ KA_,TTU_,PA_,bou_,EOM_ },
	{ SI_,RU_,BA_,bou_,EOM_ },
	{ GO_,bou_,RU_,DO_,EOM_ },
};

//-------------------------------------------------------------
//	���̃v���[���[�̃g���[�i�[�J�[�h�\���ݒ�
//	<����>	no		�b���|���鑊��̒ʐMID
//-------------------------------------------------------------
int CommSetTalkTrainerInfo( u8 no )
{
	u8 rank;

	ScriptParameter2 = no;
	PM_strcpy( StrTempBuffer0, SioTrainerInfo[no].name );

	rank = GetMultiTrainerCardStar(no);
	if ( rank == 0 ) {
		return 0;
	} else {
		PM_strcpy( StrTempBuffer1, rank_string[rank-1] );
		return 1;
	}
}
extern void ErrGamenInit();

#define JIDOU_NINSHOU_TIMEOUT	(5*60)
// =================================================================
// JidouNinshouWatchDog
// �T�v  : �����F�؂̍ۂɂT�b�ȏ�o�߂����ꍇ�͒ʐM�G���[�Ƃ��鏈��
// ����  : no	�^�X�N�h�c
// �߂�l: none
// =================================================================
void JidouNinshouWatchDog(u8 no)
{
	TASK_TABLE *my;
	my = &TaskTable[no];

	my->work[0]++;
	
	if(my->work[0] > JIDOU_NINSHOU_TIMEOUT){			//�^�C���A�E�g����
		PokemonMultiSioEnd();
		MainProcChange(ErrGamenInit);
		DelTask(no);
	}
	if(pokemon_sio_flag){								//�ʐM�ڑ��F�؂��I�������玩������
		DelTask(no);
	}
}

#ifdef PM_DEBUG
void AutoKeyTask(u8 no);

// =================================================================
// AddAutoKeyTask
// �T�v  : �����L�[���������̓o�^
// ����  : none
// �߂�l: none
// =================================================================
void AddAutoKeyTask()
{
	if(CheckTask(AutoKeyTask) == 0){
		AddTask(AutoKeyTask,80);
	}
	AutoKeyTryNum++;
}

// =================================================================
// AutoKeyTask
// �T�v  : �����L�[���������i0.5�b�Ɉ��A�{�^���������܂�
// ����  : no
// �߂�l: none
// =================================================================
void AutoKeyTask(u8 no)
{
	TaskTable[no].work[0]++;
	if(TaskTable[no].work[0] == 30){
		TaskTable[no].work[0] = 0;
		VirtualKey |= A_BUTTON;
	}
}
#endif

// =================================================================
// NextTuushinSetudanTask
// �T�v  : �ʐM�ؒf�҂��^�X�N
// ����  : no
// �߂�l: none
// =================================================================
void NextTuushinSetudanTask(u8 no)
{
	if(pokemon_sio_flag==0){
		ContinueFieldScript();
		DelTask(no);
	}
}

// =================================================================
// TuusinSetuDanTask
// �T�v  : �ʐM�ؒf�J�n�^�X�N
// ����  : no
// �߂�l: none
// =================================================================
void TuusinSetuDanTask(u8 no)
{
	SetSioEndDataStart();
	TaskTable[no].TaskAdrs = NextTuushinSetudanTask;
}


#ifdef PM_DEBUG
u8 SioCheckFlag = 0;

void SioCheckFlagClear()
{
	SioCheckFlag = 0;
}
void SioCheckFlagOn(u8 id)
{
	SioCheckFlag |= (1<<id);
}
u8   GetSioCheckFlag()
{
	return SioCheckFlag;
}
#endif
