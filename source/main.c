//******************************************************************
//          main.c                                                  
//                                                                  
//          Copyright (C) 2001 GAME FREAK inc.
//******************************************************************

#include "common.h"
#include "intr.h"

#include "saveload.h"
#include "calctool.h"
#include "multisio.h"
#include "playtime.h"
#include "rtctool.h"
#include "mus_tool.h"

#include "db_watch.h"		//DEBUG_WATCH�֘A

#include "initgame.h"
#include "evobj.h"
#include "fld_sio.h"
#include "svld_ini.h"
#include "svld_sys.h"
#include "config.h"

//#define DEBUG_KEY_CONTROL

#ifdef DEBUG_KEY_CONTROL
static void INIT_DEBUG_KEY_DATA( void );
static void SET_DEBUG_KEY_DATA( void );
#else
#define INIT_DEBUG_KEY_DATA()
#define SET_DEBUG_KEY_DATA()
#endif

//---------------------- �T�u���[�`��  -----------------------------


void InitKeyRead(void);
void KeyRead(void);

static void MainInit(void);
static void do_main( void );
static void ProcMain(void);
static void init_random(void);

extern void TitleInit(void);
extern void	OPdemoInit(void);

extern void dmaTransInit(void);			//actor.c


extern void multi_sio_func(u16 *key);
extern u8 Signal;
extern u16 SendBuf[MULTI_SIO_SEND_MAX];
extern u16 RecvBuf[MULTI_SIO_RECV_MAX][MULTI_SIO_PLAYERS_MAX];
extern u32 SioFlags;

//------------------------------------------------------------------
//	�O���[�o���ϐ�
//------------------------------------------------------------------
SystemArea sys;

u16	KeyRepeatSpeed;
u16	KeyRepeatStartWait;

u8 speed_up_flag;
u8 SoftResetDisable;

#define CASETTE_SPEED3_1 ( CST_ROM0_1ST_3WAIT | CST_ROM0_2ND_1WAIT | CST_PREFETCH_ENABLE )
//==================================================================
//                      ���C�����[�`��                              
//==================================================================

void AgbMain(void)
{
//-------- ������ --------
	RegisterRamReset(RESET_ALL_FLAG);

	*(vu16 *)REG_WAITCNT = CASETTE_SPEED3_1;

	InitKeyRead();							// �L�[���[�h������
	InterruptInit();						// �����ݏ�����
	m4aSoundInit();							// �T�E���h�h���C�o������
	RTC_Init();								// ���A���^�C���N���b�N������

	SaveLoadSystemInit();					// �t���b�V�����䏉����
	MainInit();								// �O���[�o���ϐ�������
	BGM_SystemInit();						// �a�f�l���䏉����
	init_random();							// �����̏�����(RTC����^�l���擾)

	SoftResetDisable = 0;
#if 0	
	InitGameWork();

	//�f�[�^���[�h���f�[�^���݊m�F
	SaveLoadSysInit();
	PokeAGBLoad(POKEAGB_LDALL);
	if((SaveDataCheckInfo == SVLD_RET_NULL)			//�o�b�N�A�b�v�Ȃ��i�����j
	 ||(SaveDataCheckInfo == SVLD_RET_DESTROY)){	//�o�b�N�A�b�v���S�j��i�����s�j
		//�Z�[�u������
		SysSaveDataInit();
	}
	pokStereoMono(MyData.sound_mode);		// �T�E���h�ݒ�
#endif

#if 1	//���i�łł͂n�m�ɂ��邱�ƁI
	//�t���b�V���F�؎��s���̓Q�[���J�n�����Ȃ��ŁA���[�U�[�������I�ɓd����؂�悤�ɗU��
	if(SaveMemType != FLASH_1M_TYPE) MainProcChange( NULL );
#endif

//-------- ���C�� --------
	speed_up_flag = FALSE;
	INIT_DEBUG_KEY_DATA();

	while(TRUE) {
		DEBUG_WATCH_START();

		KeyRead();							// �L�[���擾

		SET_DEBUG_KEY_DATA();

        if(
			(!SoftResetDisable)&&
			(sys.RealCont&A_BUTTON)&&
			(sys.RealCont&B_BUTTON)&&
			(sys.RealCont&SELECT_BUTTON)&&
			(sys.RealCont&START_BUTTON)
			){
			PM_SoftReset();
        }


#if 0
		if ( M_s.RecvBuf.Size >= FLD_RFIFO_LIMIT_VALUE
					&& IsEnableClearRecvFifo() == TRUE ) {
			//�P����2�x�Ăяo��Recv--;Send++;�Ƃ������ʂɂȂ�
			speed_up_flag = FALSE;
			do_main();
			sys.Trg = 0;
			dmaTransInit();
			do_main();
		} else
#endif
		if ( M_s.SendBuf.Size >= FLD_SFIFO_LIMIT_VALUE
				&& IsEnableClearSendFifo() == TRUE ) {
			// ���MFIFO���������̂��ߑ��M�f�[�^�𐶐����Ȃ�
			speed_up_flag = TRUE;
			do_main();
			speed_up_flag = FALSE;

		}
		else
		{
			speed_up_flag = FALSE;
			do_main();
			if ( M_s.RecvBuf.Size >= FLD_RFIFO_LIMIT_VALUE
					&& IsEnableClearRecvFifo() == TRUE ) {
					
				sys.Trg = 0;
				dmaTransInit();

				//2��ڂ������M�f�[�^�𐶐����Ȃ�
				speed_up_flag = TRUE;
				do_main();
				speed_up_flag = FALSE;
			}
		}

		PlayTimeCount();

		BGM_Main();
		
		DEBUG_WATCH_END();

		V_Wait();							// V�u�����N�����I���҂�
    }
}

//------------------------------------------------------------------
//		���C�����[�v����
//------------------------------------------------------------------


static void do_main( void )
{
	SioFlags    = Multi_SioMain(&Signal,SendBuf, RecvBuf);			//�ʐM����

	multi_sio_func(&sys.Cont);

	if ( SioFlags & MULTI_SIO_FRAME_SKIP ) {
		if ( IsEnableSkipFieldMain() == TRUE ) {
			//�t�B�[���h�������őS�����疳���f�[�^����M�����ꍇ�A���C���������X�L�b�v����
			return;
		}
	}
	ProcMain();
}

//------------------------------------------------------------------
//		���������[�`��												
//------------------------------------------------------------------

void MainInit(void)
{
	sys.main_timer = 0;
	sys.vsync_counter = 0;
	sys.pFirstProc = NULL;
//	MainProcChange( TitleInit );
	MainProcChange( OPdemoInit );
}

//------------------------------------------------------------------
//		���C�����[�v������											
//------------------------------------------------------------------
void ProcMain(void)
{
	if ( sys.pFirstProc != NULL ) sys.pFirstProc();
	if ( sys.pMainProc != NULL ) sys.pMainProc();
}

//------------------------------------------------------------------
//		�v���Z�X�Ăяo���֐��ύX
//------------------------------------------------------------------
void MainProcChange(pFunc func)
{
	sys.pMainProc = func;
	sys.InitSeqWork = 0;			//�������V�[�P���X�p���[�N
}

//------------------------------------------------------------------
//		�����̏�����
//	��RTC�̌o�ߎ��Ԃ��^�l�ɂ��Ă���
//------------------------------------------------------------------
static void init_random(void)
{
	u32 seed;

	seed = GetRealTimeMinute();
	seed = (seed >> 16) ^ (seed & 0xffff);
	pp_srand(seed);
}

//==================================================================
//                      �T�u���[�`��                                
//==================================================================

//------------------------------------------------------------------
//                      �L�[�ǂݍ���                                
//------------------------------------------------------------------

void InitKeyRead(void)
{
	KeyRepeatSpeed=KEYREPEATSPEED;			//�L�[���s�[�g�E�F�C�g
	KeyRepeatStartWait=KEYREPEARSTARTWAIT;	//�L�[���s�[�g���n�܂�܂ł̃E�F�C�g
    sys.Cont	=	0;
    sys.Trg		=	0;
    sys.Repeat	=	0;
    sys.RealCont=	0;
    sys.RealTrg	=	0;
}

void KeyRead(void)
{
    u16 ReadData;

//	ReadData	=	(*(vu16 *)REG_KEYINPUT ^ 0xffff);
	ReadData	=	(*(vu16 *)REG_KEYINPUT ^ 0x03ff);
	
    sys.RealTrg	=	ReadData & (ReadData ^ sys.RealCont);        // �g���K ����

    sys.Trg		=	ReadData & (ReadData ^ sys.RealCont);        // �g���K ����
    sys.Repeat	=	ReadData & (ReadData ^ sys.RealCont);        // ���s�[�g ����

    if((ReadData!=0)&&(sys.Cont==ReadData)){
		if(--sys.RepeatWait==0){
			sys.Repeat		=	ReadData;
			sys.RepeatWait	=	KeyRepeatSpeed;
		}
	}
	else{
		sys.RepeatWait=KeyRepeatStartWait;
	}
    sys.RealCont=	ReadData;                                // �x�^ ����
    sys.Cont	=	ReadData;                                // �x�^ ����

	if(MyData.key_config == CNT_MD_KATATE){					//�Q�[���p�ϊ�
		if(sys.Trg & L_BUTTON)	sys.Trg	|= A_BUTTON;
		if(sys.Cont & L_BUTTON) sys.Cont |= A_BUTTON;
	}
    if(sys.Trg & sys.TrgFlagMask) sys.TrgFlag = 1;				// �ŏ��̃g���K�[���s����
}




#ifdef	DEBUG_KEY_CONTROL

//==================================================================
//
//	�L�[�f�[�^����
//
//	���f�o�b�O�p�ɓ���̃L�[����������Ԃ𐶐����Â���
//==================================================================

static u8 debug_key_mode = 0;

static unsigned long debug_rand_next = 0;

enum{
	DEBUGKEY_NO_MODE = 0,
	DEBUGKEY_A_TRIGER,
	DEBUGKEY_B_TRIGER,
	DEBUGKEY_RANDOM_UD,
	DEBUGKEY_RANDOM_LR,
	DEBUGKEY_START_TRIGER,
	DEBUGKEY_SELECT_TRIGER,
};

//------------------------------------------------------------------
//------------------------------------------------------------------


static u16 debug_rand(void)
{
    debug_rand_next = debug_rand_next *1103515245L + 12345;
    return (u16)(debug_rand_next / 65536L) % 32768U;
}

static void debug_srand(u16 seed)
{
    debug_rand_next = seed;
}

//------------------------------------------------------------------
//------------------------------------------------------------------

static void INIT_DEBUG_KEY_DATA( void )
{
	debug_key_mode = DEBUGKEY_NO_MODE;
}

//------------------------------------------------------------------
//	�L�[�f�[�^�ύX�������C��
//------------------------------------------------------------------
static void SET_DEBUG_KEY_DATA( void )
{
	if ( (sys.Cont&L_BUTTON) && (sys.Cont&R_BUTTON) ) {
		if ( sys.Cont & B_BUTTON ) {
			debug_srand( pp_rand() );
			debug_key_mode = DEBUGKEY_NO_MODE;
		}
		else if ( sys.Cont & A_BUTTON ) {
			debug_srand( pp_rand() );
			debug_key_mode = DEBUGKEY_A_TRIGER;
		}
		else if ( sys.Cont & U_KEY ) {
			debug_key_mode = DEBUGKEY_RANDOM_UD;
		}
		else if ( sys.Cont & L_KEY ) {
			debug_key_mode = DEBUGKEY_RANDOM_LR;
		}
		else if ( sys.Cont & START_BUTTON ) {
			debug_key_mode = DEBUGKEY_START_TRIGER;
		}
		else if ( sys.Cont & SELECT_BUTTON ) {
			debug_key_mode = DEBUGKEY_B_TRIGER;
			//debug_key_mode = DEBUGKEY_SELECT_TRIGER;
		}
	}

	switch ( debug_key_mode ) {

	case DEBUGKEY_NO_MODE:
		break;

	case DEBUGKEY_A_TRIGER:
		sys.Trg = A_BUTTON;
		sys.Cont = 0;
		break;

	case DEBUGKEY_B_TRIGER:
		if (debug_rand() & 0x1100) {
			sys.Trg = B_BUTTON;
		} else {
			sys.Trg = 0;
		}
		sys.Cont = 0;
		break;

	case DEBUGKEY_RANDOM_UD:
		sys.Trg = 0;
	//	if ( sys.vsync_counter % 64 < 32 ) {
		if ( debug_rand() & 0x100 ) {
			sys.Cont = U_KEY;
		} else {
			sys.Cont = D_KEY;
		}
		sys.Trg = B_BUTTON;
		break;

	case DEBUGKEY_RANDOM_LR:
		sys.Trg = 0;
	//	if ( sys.vsync_counter % 64 < 32 ) {
		if ( debug_rand() & 0x200 ) {
			sys.Cont = L_KEY;
		} else {
			sys.Cont = R_KEY;
		}
		break;

	case DEBUGKEY_START_TRIGER:
		sys.Trg = START_BUTTON;
		sys.Cont = 0;
		break;

	case DEBUGKEY_SELECT_TRIGER:
		if ( debug_rand() & 0x200 ) {
			sys.Trg = SELECT_BUTTON;
		} else {
			sys.Trg = START_BUTTON;
		}
		sys.Cont = 0;
		break;
	}
}

#endif

