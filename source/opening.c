//******************************************************************
//          opening.c                                                  
//                                                                  
//          Copyright (C) 2001 GAME FREAK inc.
//******************************************************************
#include	"common.h"
#include	"intr.h"
#include	"actor.h"
#include	"actanm.h"
#include	"palanm.h"
#include	"task.h"
#include	"print.h"
#include	"task.h"
#include	"poketool.h"
#include	"decord.h"
#include	"objdata.h"
#include	"menu.h"
#include	"window.h"
#include	"saveload.h"
#include	"namein.h"
#include	"fld_main.h"
#include	"fld_eff.h"
#include	"mystatus.h"
#include	"cli_ef.h"
#include	"mus_tool.h"
#include	"monsno.def"
#include	"ev_flag.h"
#include	"message.h"
#include	"calctool.h"
#include	"laster.h"
#include	"rtctool.h"

#include	"svld_ini.h"
#include	"svld_sys.h"

#include	"opening.dat"
#include	"../msgdata/opening.h"

void OpeningMain( void );
void OpeningInit( void );
void OpeningReturn( void );

extern	void	DebugFieldInit(void);
extern	void	ConfigInit(void);
extern	void	TitleInit(void);
extern	const	u32	DecordAdrs[];
extern	const	u16	pokeg_t_Palette[];		//Dummy
extern	void	VoicePlay( u16 no, s8 pan );
extern	u8		UpDateReport(u8* updata);
extern void HusiginaDekigotoGamenInit();

static	u8		OpeningInitSub(u8 fade_pat);	//0:�Z�[�u�f�[�^�Ȃ�,1:�Z�[�u�f�[�^����

static	void	OpeningMenuInit0Task(u8 id);
static	void	OpeningMenuInit0_1Task(u8 id);
static	void	OpeningMenuInitRTCwaitTask(u8 id);
static	void	OpeningMenuInitRTCTask(u8 id);
static	void	OpeningMenuInitTask(u8 id);
static	void	OpeningMenuScreenTask(u8 id);
static	void	OpeningMenuMainTask(u8 id);
static	void	OpeningMenuExitTask(u8 id);
static	void	OpeningMenuCancelTask(u8 id);

static	void	OpeningMenuMainCursorSet(u8 mode,u8 pos);
static	void	OpeningMenuWrite(const u8* index,u8 x,u8 y);

static	void	FirstGameInitTask(u8 id);
static	void	FirstGameNameEntrySetTask(u8 id);

static	void	FirstGameOp_Odamaki1_0_Task(u8 id);
static	void	FirstGameOp_Odamaki1_1_Task(u8 id);
static	void	FirstGameOp_Odamaki1_2_Task(u8 id);
static	void	FirstGameOp_Odamaki1_2_1_Task(u8 id);
static	void	FirstGameOp_Odamaki1_3_Task(u8 id);
static	void	FirstGameOp_Odamaki1_4_Task(u8 id);
static	void	FirstGameOp_Odamaki1_4_1_Task(u8 id);
static	void	FirstGameOp_Odamaki1_4_2_Task(u8 id);
static	void	FirstGameOp_Odamaki1_5_Task(u8 id);
static	void	FirstGameOp_Odamaki1_6_Task(u8 id);

static	void	FirstGameOp_StatusSet1_Task(u8 id);
static	void	FirstGameOp_StatusSet1_1_Task(u8 id);
static	void	FirstGameOp_StatusSet2_Task(u8 id);
static	void	FirstGameOp_StatusSet2_1_Task(u8 id);
static	void	FirstGameOp_StatusSet3_Task(u8 id);
static	void	FirstGameOp_StatusSet3_1_Task(u8 id);
static	void	FirstGameOp_StatusSet3_2_Task(u8 id);
static	void	FirstGameOp_StatusSet4_Task(u8 id);
static	void	FirstGameOp_StatusSet5_Task(u8 id);
static	void	FirstGameOp_StatusSet6_Task(u8 id);
static	void	FirstGameOp_StatusSet7_Task(u8 id);
static	void	FirstGameOp_StatusSet8_Task(u8 id);
static	void	FirstGameOp_StatusSet9_Task(u8 id);
static	void	FirstGameOp_StatusSet10_Task(u8 id);

static	void	FirstGameOp_Odamaki2_1_Task(u8 id);
static	void	FirstGameOp_Odamaki2_2_Task(u8 id);

static	void	FirstGameNameEntryExit1Task(u8 id);
static	void	FirstGameNameEntryExit2Task(u8 id);

static	void	FirstGameExitEffectInitTask(u8 id);
static	void	FirstGameExitEffectTask(u8 id);
static	void	FirstGameExitTask(u8 id);

void	SexSelectInit(u8	x, u8 y);
s8		SexSelectMain(void);
void	NameSelectInit(u8	x, u8 y);
s8		NameSelectMain(void);
static	void	DefaultNameSet(u8	num);

static	void	HeroStatusWrite(void);
//static  void	StandHeroDel(u8);	/* �g���Ă��Ȃ��̂Ńt�b�N���� */

static	u8		OpeningStandPokeSet(u8 x,u8 y);
static	void	OpeningNoMove(actWork* aw);
static	void	HeroMoveAffine(actWork* aw);
static	const	actAffAnm*	const	OpEffAffTable[];

static	void	OpeningObjFadeOutTaskSet(u8 flg_task_id,u8 wait);
static	void	OpeningObjFadeInTaskSet(u8 flg_task_id,u8 wait);
static	void	OpeningBgFadeOutTaskSet(u8 flg_task_id,u8 wait);
static	void	OpeningBgFadeInTaskSet(u8 flg_task_id,u8 wait);

static	const u16	op_back_Palette[32];

static	const u8 spotch_LZ[0x1a8];
static	const u8 spotsc_LZ[0x128];

/* BG(Spot) */
static	const u16	OpeningBGFadeColTable[] = {
0x53FF,0x37FF,0x2F7B,0x26F7,0x1E73,0x19EF,0x116B,0x08E7,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

static	const u16 opening_Palette[16] = {
0x7E51,0x7FFF,0x28E6,0x398B,0x0821,0x5672,0x7779,0x5ED6,
0x6F37,0x1884,0x460F,0x3D46,0x61E5,0x6A27,0x728B,0x7B11,
};
#define	MODE	(0)
#define	CPOS	(1)
#define	ACTNUM	(2)
#define	ACTNUM2	(3)
#define	BG1_X	(4)
#define	FADE_SW	(5)
#define	SEX_SW	(6)
#define	WAIT	(7)
#define	OP_ACTSTR_ODMK	(8)
#define	OP_ACTSTR_PKMN	(9)
#define	OP_ACTSTR_MALE	(10)
#define	OP_ACTSTR_FEML	(11)

#define	NO_ACT	(0xff)

#define	OPENING_POKEMON (MONSNO_RURIRI)

/********************************************************************/
/*                                                                  */
/*				Ҳ�													*/
/*                                                                  */
/********************************************************************/
void OpeningMain( void )
{
		MainTask();
		JumpActor();
		SetActor();
		PaletteAnime();
}

/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static void VInterFunc( void )
{
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}

/********************************************************************/
/*				������												*/
/********************************************************************/
void OpeningInit( void )			//���S�������֐�
{
	OpeningInitSub(0);
}

void OpeningInitReturn( void )		//���A�p�������֐�
{
	OpeningInitSub(1);
}

static	u8 OpeningInitSub(u8 fade_pat)	//0:�Z�[�u�f�[�^�Ȃ�,1:�Z�[�u�f�[�^����
{
	u8	taskID;

	SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

    *(vu16 *)REG_DISPCNT	= 0;
    *(vu16 *)REG_BG2CNT		= 0;
    *(vu16 *)REG_BG1CNT		= 0;
    *(vu16 *)REG_BG0CNT		= 0;

	*(vu16 *)REG_BG2HOFS	= 0;
	*(vu16 *)REG_BG2VOFS	= 0;
    *(vu16 *)REG_BG1HOFS	= 0;
    *(vu16 *)REG_BG1VOFS	= 0; 
    *(vu16 *)REG_BG0HOFS	= 0;
    *(vu16 *)REG_BG0VOFS	= 0; 

    DmaClear(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
    DmaClear(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
    DmaClear(3, 0, PLTT+2, PLTT_SIZE-2,       16);	// ��گ�  ������

	PaletteAnimeInit();
	PaletteWorkSet( &opening_Palette[0x00], PA_BG0, 0x0020 );	//DUMMY

	LasterInit();									// HDMA������
	InitTask();										// ���  ������
	InitActor();									// ���� ������
	ObjPalManInit();									// ��گ��Ȱ�ެ
	PrintScreenInit(DEFAULT_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);

	if(fade_pat){
		PaletteFadeReq(0xffffffff,0,16,0,0);		//���t�F�[�h
	}else{
		PaletteFadeReq(0xffffffff,0,16,0,0xffff);	//���t�F�[�h
	}
	*(vu16 *)REG_WIN0H	= 0;
	*(vu16 *)REG_WIN0V	= 0;

	*(vu16 *)REG_WININ	= 0;
	*(vu16 *)REG_WINOUT	= 0;

   	*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 0;

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(OpeningMain);

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_WIN0_ON				// WIN0 ON
						  | DISP_OBJ_ON;
	taskID = AddTask(OpeningMenuInit0Task, 0);
	TaskTable[taskID].work[CPOS] = 0;

	return 0;
}



/********************************************************************/
/*				�o�b�N�A�b�v�����x��								*/
extern	const	u8	save_full_msg[];
/********************************************************************/
#if 0
static	u8*		upld_msg_get(u8 flg)
{
	switch(flg){	// �A�b�v�f�[�^���\��
	case SVLD_RET_UPD_OK:
		return	(u8*)load_upd_ok_msg;
	case SVLD_RET_UPD_FULL:
		return	(u8*)load_upd_full_msg;
	case SVLD_RET_UPD_NG:
		return	(u8*)load_upd_ng_msg;
	}
	return	0;
}
#endif

static	void	OpeningMenuInit0Task(u8 id)
{
//	u8	upld_flg;
//	u8*	msg;

	if(FadeData.fade_sw == 0){
		*(vu16 *)REG_WIN0H	= 0;
		*(vu16 *)REG_WIN0V	= 0;

		*(vu16 *)REG_WININ	= 0x1111;
		*(vu16 *)REG_WINOUT	= 0x0031;

   		*(vu16 *)REG_BLDCNT   = 0x00f1;	//�������ݒ�
   		*(vu16 *)REG_BLDALPHA = 0;
   		*(vu16 *)REG_BLDY	  = 7;

		switch(SaveDataCheckInfo){

		case SVLD_RET_OK:
			// �Z�[�u�f�[�^�����݂���ꍇ
			if ( ExDataSpecialFlagCheck() == TRUE ) {
				TaskTable[id].work[MODE] = 2;
			} else {
				TaskTable[id].work[MODE] = 1;
			}
			TaskTable[id].TaskAdrs = OpeningMenuInitRTCTask;
//2002.9.19	�K�v�Ȃ��̂ō폜
#if 0
			//�A�b�v�f�[�^����
			upld_flg = UpLoadDataGet((u8*)(UserWork+0x00000000));	//�f�[�^�i�[
			if(upld_flg == SVLD_RET_UPD_NULL)	break; //�A�b�v�f�[�^�Ȃ�

			NMenuWinBoxWrite( 2, 14,27, 19);
			NMenuMsgWrite(load_upd_on_msg,3,15);
			*(vu16 *)REG_WIN0H	= (17*0x100)+223;
			*(vu16 *)REG_WIN0V	= (113*0x100)+159;

			//�A�b�v�f�[�^����
			UpDateReport((u8*)(UserWork+0x00000000));

			//���|�[�g�X�V
			upld_flg = UpLoadDataSet();
			msg = upld_msg_get(upld_flg);
			if(msg){	// �x���֘A�\��
				NMenuWinBoxWrite( 2, 14,27, 19);
				NMenuTalkMsgSet(msg,3,15);
				TaskTable[id].TaskAdrs = OpeningMenuInit0_1Task;
			}
#endif
			break;
		case SVLD_RET_DESTROY:
			// �Z�[�u�f�[�^�����݂��邪���S�ɔj�󂳂�Ă���i���A�s�j
			NMenuWinBoxWrite( 2, 14,27, 19);
			NMenuTalkMsgSet((u8*)load_destroy_msg,3,15);
			*(vu16 *)REG_WIN0H	= (17*0x100)+223;
			*(vu16 *)REG_WIN0V	= (113*0x100)+159;
			TaskTable[id].work[MODE] = 0;
			TaskTable[id].TaskAdrs = OpeningMenuInit0_1Task;
			break;
		case SVLD_RET_NG:
			// �Z�[�u�f�[�^�����݂��邪�j�󂳂�Ă���i�O��f�[�^���A�j
			NMenuWinBoxWrite( 2, 14,27, 19);
			NMenuTalkMsgSet((u8*)load_ng_msg,3,15);
			*(vu16 *)REG_WIN0H	= (17*0x100)+223;
			*(vu16 *)REG_WIN0V	= (113*0x100)+159;
			TaskTable[id].work[MODE] = 1;
			TaskTable[id].TaskAdrs = OpeningMenuInit0_1Task;
//2002.9.19
			// �Z�[�u�f�[�^�����݂���ꍇ
			if ( ExDataSpecialFlagCheck() == TRUE ) {
				TaskTable[id].work[MODE] = 2;
			} else {
				TaskTable[id].work[MODE] = 1;
			}
//2002.9.19
			break;
		default:
		case SVLD_RET_NULL:
			// �Z�[�u�f�[�^�����݂��Ȃ��ꍇ
			TaskTable[id].work[MODE] = 0;
			TaskTable[id].TaskAdrs = OpeningMenuInitRTCTask;
			break;
		//�f�o�b�O-----------------------
		case SVLD_RET_HARDERROR:
			// �P�l�T�u��Ղ��������Ă��Ȃ�
			NMenuWinBoxWrite( 2, 14,27, 19);
			NMenuTalkMsgSet((u8*)load_debug_msg,3,15);
			*(vu16 *)REG_WIN0H	= (17*0x100)+223;
			*(vu16 *)REG_WIN0V	= (113*0x100)+159;
			TaskTable[id].work[MODE] = 0;
			TaskTable[id].TaskAdrs = OpeningMenuInit0_1Task;
			break;
		//-------------------------------
		}
	}
}

static	void	OpeningMenuInit0_1Task(u8 id)
{
	if(NMenuTalkMsgPut()&&(sys.Trg & A_BUTTON)){
		NMenuBoxClear( 2, 14,27, 19);
		TaskTable[id].TaskAdrs = OpeningMenuInitRTCTask;
	}
}

/********************************************************************/
static	void	OpeningMenuInitRTCTask(u8 id)
{
	if(FadeData.fade_sw == 0){
		*(vu16 *)REG_WIN0H	= 0;
		*(vu16 *)REG_WIN0V	= 0;

		*(vu16 *)REG_WININ	= 0x1111;
		*(vu16 *)REG_WINOUT	= 0x0031;

   		*(vu16 *)REG_BLDCNT   = 0x00f1;	//�������ݒ�
   		*(vu16 *)REG_BLDALPHA = 0;
   		*(vu16 *)REG_BLDY	  = 7;

		if( (RTC_GetStatus() & RTCST_ERR_MASK) == 0 ) {
			// RTC�����퓮�삵�Ă���
			TaskTable[id].TaskAdrs = OpeningMenuInitTask;

		} else if (SVLD_RET_DESTROY) {
			// RTC���s���ȏꍇ�i�d�r�؂�Ȃǁj
			NMenuWinBoxWrite( 2, 14,27, 19);
			NMenuTalkMsgSet(rtc_ng_msg,3,15);
			*(vu16 *)REG_WIN0H	= (17*0x100)+223;
			*(vu16 *)REG_WIN0V	= (113*0x100)+159;
			TaskTable[id].TaskAdrs = OpeningMenuInitRTCwaitTask;
		}
	}
}

static	void	OpeningMenuInitRTCwaitTask(u8 id)
{
	if(NMenuTalkMsgPut()&&(sys.Trg & A_BUTTON)){
		NMenuBoxClear( 2, 14,27, 19);
		TaskTable[id].TaskAdrs = OpeningMenuInitTask;
	}
}

/********************************************************************/
/*				����												*/
/********************************************************************/
static	void	OpeningMenuInitTask(u8 id)
{
	u16	col;

	if(FadeData.fade_sw == 0){
		*(vu16 *)REG_WIN0H	= 0;
		*(vu16 *)REG_WIN0V	= 0;

		*(vu16 *)REG_WININ	= 0x1111;
		*(vu16 *)REG_WINOUT	= 0x0031;

   		*(vu16 *)REG_BLDCNT   = 0x00f1;	//�������ݒ�
   		*(vu16 *)REG_BLDALPHA = 0;
   		*(vu16 *)REG_BLDY	  = 7;

		col = 0;	//��
		PaletteWorkSet( &col, PA_BG15+PA_COL14,2 );	//�p���b�g�ݒ�
		if(MyData.my_sex == MALE){
			col = 0x7E04;	//���F
			PaletteWorkSet( &col, PA_BG15+PA_COL1,2 );	//�p���b�g�ݒ�
		}else{
			col = 0x547F;	//�s���N
			PaletteWorkSet( &col, PA_BG15+PA_COL1,2 );	//�p���b�g�ݒ�
		}
		switch(TaskTable[id].work[MODE]){
		default:
		case 0:
			NMenuWinBoxWrite( 1, 0,28, 3);
			OpeningMenuWrite(stmenu_indexstr1,2,1);
			NMenuWinBoxWrite( 1, 4,28, 7);
			OpeningMenuWrite(stmenu_indexstr3,2,5);
			break;
		case 1:
			NMenuWinBoxWrite( 1, 0,28, 7);
			OpeningMenuWrite(stmenu_indexstr2,2,1);
			NMenuWinBoxWrite( 1, 8,28,11);
			OpeningMenuWrite(stmenu_indexstr1,2, 9);
			NMenuWinBoxWrite( 1,12,28,15);
			OpeningMenuWrite(stmenu_indexstr3,2,13);
			HeroStatusWrite();			// 01/10/23 kaga
			break;
		case 2:
			NMenuWinBoxWrite( 1, 0,28, 7);
			OpeningMenuWrite(stmenu_indexstr2,2,1);
			NMenuWinBoxWrite( 1, 8,28,11);
			OpeningMenuWrite(stmenu_indexstr1,2, 9);
			NMenuWinBoxWrite( 1,12,28,15);
			OpeningMenuWrite(stmenu_indexstr4,2,13);
			NMenuWinBoxWrite( 1,16,28,19);
			OpeningMenuWrite(stmenu_indexstr3,2,17);
			HeroStatusWrite();			// 01/10/23 kaga
			break;
		}
		TaskTable[id].TaskAdrs = OpeningMenuScreenTask;
	}
}


/********************************************************************/
/*				��ذݕ`��											*/
/********************************************************************/
static	void	OpeningMenuScreenTask(u8 id)
{
	OpeningMenuMainCursorSet(TaskTable[id].work[MODE],TaskTable[id].work[CPOS]);
	TaskTable[id].TaskAdrs = OpeningMenuMainTask;
}

/********************************************************************/
/*				Ҳݐ���												*/
/********************************************************************/
/*				������												*/
static u8	OpeningMenuMainKeyCheck(u8 task_id)
{
	u8	listcount;
		//����
	if(sys.Trg & A_BUTTON){
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[task_id].TaskAdrs = OpeningMenuExitTask;
		return 0;
	}
		//�߂�(->TITLE)
	if(sys.Trg & B_BUTTON){
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0xffff);
		*(vu16 *)REG_WIN0H	= (0*0x100)+240;;		//�z���C�g�A�E�g���邽��WINDOW �S�J
		*(vu16 *)REG_WIN0V	= (0*0x100)+160;
		TaskTable[task_id].TaskAdrs = OpeningMenuCancelTask;
		return 0;
	}
		//���ڐ��w��
	switch(TaskTable[task_id].work[MODE]){
	default:
	case 0:
		listcount = 2;
		break;
	case 1:
		listcount = 3;
		break;
	case 2:
		listcount = 4;
		break;
	}
		//���وړ�
	if((sys.Trg & U_KEY)&&(TaskTable[task_id].work[CPOS] > 0)){
		TaskTable[task_id].work[CPOS]--;
		return 1;
	}
	if((sys.Trg & D_KEY)&&(TaskTable[task_id].work[CPOS] < (listcount-1))){
		TaskTable[task_id].work[CPOS]++;
		return 1;
	}
	return 0;
}

/********************************************************************/
static	void	OpeningMenuMainTask(u8 id)
{
	if(OpeningMenuMainKeyCheck(id)){
		TaskTable[id].TaskAdrs = OpeningMenuScreenTask;
	}
}

/********************************************************************/
/*				�I��												*/
/********************************************************************/
static	void	OpeningMenuExitTask(u8 id)
{
	u8	go_proc;

	if(FadeData.fade_sw)	return;

	switch(TaskTable[id].work[MODE]){
	default:
	case 0:			//�����ް��Ȃ�
		switch(TaskTable[id].work[CPOS]){
		default:
		case 0:
			go_proc = 0;
			break;
		case 1:
			go_proc = 2;
			break;
		}
		break;
	case 1:		//�����ް�����
		switch(TaskTable[id].work[CPOS]){
		default:
		case 0:
			go_proc = 1;
			break;
		case 1:
			go_proc = 0;
			break;
		case 2:
			go_proc = 2;
			break;
		}
		break;
	case 2:		//�����ް����聕�ӂ����Ȃł�����
		switch(TaskTable[id].work[CPOS]){
		default:
		case 0:
			go_proc = 1;
			break;
		case 1:
			go_proc = 0;
			break;
		case 2:
			go_proc = 3;
			break;
		case 3:
			go_proc = 2;
			break;
		}
		break;
	}
	switch(go_proc){
	default:
	case 0:
		//�u�������傩��v
		PaletteWork[PA_BG0 + PA_COL0] = 0x0000;
		PaletteWorkTrans[PA_BG0 + PA_COL0] = 0x0000;
		TaskTable[id].TaskAdrs = FirstGameInitTask;	//�X�^�[�g�f����
		break;
	case 1:
		//�u�Â�����v
		PaletteWork[PA_BG0 + PA_COL0] = 0x0000;
		PaletteWorkTrans[PA_BG0 + PA_COL0] = 0x0000;
		MainProcChange(ContinueGameStart);
		DelTask(id);
		break;
	case 2:
		//�u�����Ă���������v
		sys.pReturnProc = OpeningInitReturn;
		MainProcChange(ConfigInit);
		DelTask(id);
		break;
	case 3:
		//�u�ӂ����Ȃł����Ɓv
		/* �� */
		MainProcChange(HusiginaDekigotoGamenInit);
		DelTask(id);
		break;

	}
}

/********************************************************************/
/*				��ݾ�												*/
/********************************************************************/
static	void	OpeningMenuCancelTask(u8 id)
{
	if(FadeData.fade_sw == 0){
		MainProcChange(TitleInit);
		DelTask(id);
	}
}


/********************************************************************/
/*				���̑�												*/
/********************************************************************/
static	void	OpeningMenuMainCursorSet(u8 mode,u8 pos)
{
	*(vu16 *)REG_WIN0H	= ((8+1)*0x100)+(232-1);	//�w���͋���
	switch(mode){
	default:
	case 0:
		switch(pos){
		default:
		case 0:
			*(vu16 *)REG_WIN0V	= ((0+1)*0x100)+(32-1);
			break;
		case 1:
			*(vu16 *)REG_WIN0V	= ((32+1)*0x100)+(64-1);
			break;
		}
		break;
	case 1:
		switch(pos){
		default:
		case 0:
			*(vu16 *)REG_WIN0V	= ((0+1)*0x100)+(64-1);
			break;
		case 1:
			*(vu16 *)REG_WIN0V	= ((64+1)*0x100)+(96-1);
			break;
		case 2:
			*(vu16 *)REG_WIN0V	= ((96+1)*0x100)+(128-1);
			break;
		}
		break;
	case 2:
		switch(pos){
		default:
		case 0:
			*(vu16 *)REG_WIN0V	= ((0+1)*0x100)+(64-1);
			break;
		case 1:
			*(vu16 *)REG_WIN0V	= ((64+1)*0x100)+(96-1);
			break;
		case 2:
			*(vu16 *)REG_WIN0V	= ((96+1)*0x100)+(128-1);
			break;
		case 3:
			*(vu16 *)REG_WIN0V	= ((128+1)*0x100)+(160-1);
			break;
		}
	}
}

/********************************************************************/
static	void	OpeningMenuWrite(const u8* index,u8 x,u8 y)
{
	u8	i,indexstr[3+26+1];	//�P���C�����f�[�^�{�p���b�g�w�b�_�{�I���R�[�h
	
	indexstr[0] = CTRL_;	//�R���g���[���R�[�h
	indexstr[1] = C_FCOL_;	//�����F�ύX�R�[�h
	indexstr[2] = 14;		//�����F�w��i���o�[
	for(i=0;i<26;i++)	indexstr[3+i] = index[i];	//�\���f�[�^�R�s�[
	indexstr[3+26] = EOM_;		//�G���h�R�[�h
	NMenuMsgWrite(indexstr,x,y);
}

/********************************************************************/
/*				�Â����糨��޳�̎�l���ð��						*/
/********************************************************************/
static void HeroNameWrite( void );
static void HeroZukanNumWrite( void );
static void HeroPlayTimeWrite( void );
static void HeroBadgeWrite( void );

static void HeroStatusWrite( void )
{
	HeroNameWrite();
	HeroZukanNumWrite();
	HeroPlayTimeWrite();
	HeroBadgeWrite();
}

static void HeroNameWrite( void )
{
	NMenuMsgWrite( HeroInfoNameMsg, 2, 3 );
	FontHeroNameWrite( 9, 3 );
}

static void HeroZukanNumWrite( void )
{
	NMenuMsgWrite( HeroInfoZukanMsg, 2, 5 );
	FontOpPokeZukanNumWrite( 9, 5 );
}

static void HeroPlayTimeWrite( void )
{
	NMenuMsgWrite( HeroInfoTimeMsg, 0x10, 3 );
	FontOpPlayTimeWrite( 0x16, 3 );
}

static void HeroBadgeWrite( void )
{
	NMenuMsgWrite( HeroInfoBadgeMsg, 0x10, 5 );
	FontBatchWrite( 0x19, 5 );
	NMenuMsgWrite(count_msg, 0x1a, 5 );
}

/********************************************************************/
/*                                                                  */
/*				�u�������傩��͂��߂�v�����ݸ�					*/
/*                                                                  */
/********************************************************************/
static	void	OpeningStartActorSet(u8 id);
static	const	u8	msg_odamaki_01_p02_opening_test1[];
static	const	u8	msg_odamaki_01_p02_opening_test2[];

//������
static	void	FirstGameInitTask(u8 id)
{
	PrintScreenInit(DEFAULT_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);

	*(vu16 *)REG_WIN0H	= 0;
	*(vu16 *)REG_WIN0V	= 0;
	*(vu16 *)REG_WININ	= 0;
	*(vu16 *)REG_WINOUT	= 0;

   	*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 0;

	LZ77UnCompVram((void*)spotch_LZ,(void*)(BG_VRAM+0x0000));	//BG CGX�]��
	LZ77UnCompVram((void*)spotsc_LZ,(void*)(BG_VRAM+0x3800));	//BG MAP�]��
	PaletteWorkSet( op_back_Palette, PA_BG0, 0x0040 );			//BG COL�]��
	PaletteWorkSet(&OpeningBGFadeColTable[8],PA_BG0+1,2*8);

	LasterInit();									// HDMA������
	InitActor();									// ���� ������
	ObjPalManInit();									// ��گ��Ȱ�ެ
	OpeningStartActorSet(id);

	PaletteFadeReq(0xffffffff,0,16,0,0);

	*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| 7 << BG_SCREEN_BASE_SHIFT		//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_BG1_ON					// BG1�L��
						  | DISP_OBJ_ON;
	TaskTable[id].work[BG1_X] = 0;
	TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_0_Task;
	TaskTable[id].work[ACTNUM] 	= NO_ACT;
	TaskTable[id].work[ACTNUM2] = NO_ACT;
	TaskTable[id].work[WAIT]	= 0xd8;
	MusicPlay(MUS_DOORO_X4);
}

//�a�f�l�E�F�C�g
static	void	FirstGameOp_Odamaki1_0_Task(u8 id)
{
	u8	actnum;

	if(TaskTable[id].work[WAIT]){
		TaskTable[id].work[WAIT]--;
		return;
	}
	actnum = TaskTable[id].work[OP_ACTSTR_ODMK];
	ActWork[actnum].x = 136;
	ActWork[actnum].y =  60;
	ActWork[actnum].banish = 0;
	ActWork[actnum].oamData.ObjMode = 1;

	OpeningObjFadeInTaskSet(id,10);
	OpeningBgFadeInTaskSet(id,20);
	TaskTable[id].work[WAIT]	= 0x50;
	TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_1_Task;
}

//�I�_�}�L�o��
static	void	FirstGameOp_Odamaki1_1_Task(u8 id)
{
	if(!TaskTable[id].work[FADE_SW])	return;

	ActWork[TaskTable[id].work[OP_ACTSTR_ODMK]].oamData.ObjMode = 0;

	if(TaskTable[id].work[WAIT]){
		TaskTable[id].work[WAIT]--;
		return;
	}
	NMenuWinBoxWrite( 2, 13,27, 18);
	NMenuTalkMsgSet(msg_odamaki_01_p01_opening,4,14);

	TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_2_Task;
}

//��b�P�u����`���܂����v
static	void	FirstGameOp_Odamaki1_2_Task(u8 id)
{
	if(FadeData.fade_sw == 0){
		if(NMenuTalkMsgPut()){
			TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_2_1_Task;
			NMenuTalkMsgSet(msg_odamaki_01_p02_opening_test1,4,14);
		}
	}
}

//��b�Q�u�|�P�b�g�����X�^�[���Ȃ킿�v
static	void	FirstGameOp_Odamaki1_2_1_Task(u8 id)
{
	if(NMenuTalkMsgPut()){
		TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_3_Task;
	}
}

//�|�P�����o��
static	void	FirstGameOp_Odamaki1_3_Task(u8 id)
{
	u8	actnum;

	actnum = TaskTable[id].work[OP_ACTSTR_PKMN];
	ActWork[actnum].x = 104;
	ActWork[actnum].y =  68+4;
	ActWork[actnum].banish = 0;
	ActWork[actnum].work[0] = 0;

	MonsterBallAppearEffSet(actnum,ActWork[actnum].oamData.Pltt,112,58,0,0,32,0x0000ffff);

	TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_4_Task;
	TaskTable[id].work[WAIT] = 0;
}

//�����ݒ�
static	void	FirstGameOp_Odamaki1_4_Task(u8 id)
{
	if((VoicePlayWait())&&(TaskTable[id].work[WAIT] >= 0x0060)){
		NMenuTalkMsgSetContinue(msg_odamaki_01_p02_opening_test2);
		TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_4_1_Task;
	}
	if(TaskTable[id].work[WAIT] < 0x4000){
		TaskTable[id].work[WAIT]++;
		if(TaskTable[id].work[WAIT] == 0x20){
			VoicePlay(OPENING_POKEMON,0);
		}
	}
}

//�|�P�����o��҂�
static	void	FirstGameOp_Odamaki1_4_1_Task(u8 id)
{
	if((NMenuTalkMsgPut())){
		NMenuTalkMsgSet(msg_odamaki_01_p02_1_opening,4,14);
		TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_4_2_Task;
	}
}

//��b�R�u���̂������ɂ́v
static	void	FirstGameOp_Odamaki1_4_2_Task(u8 id)
{
	if((NMenuTalkMsgPut())){
		NMenuWinBoxWrite( 2, 13,27, 18);
		NMenuTalkMsgSet(msg_odamaki_01_p03_opening,4,14);

		TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_5_Task;
	}
}

//�ݒ蓱���u�Ƃ���ŃL�~�́H�v
static	void	FirstGameOp_Odamaki1_5_Task(u8 id)
{
	if(NMenuTalkMsgPut()){
		ActWork[TaskTable[id].work[OP_ACTSTR_ODMK]].oamData.ObjMode = 1;
		ActWork[TaskTable[id].work[OP_ACTSTR_PKMN]].oamData.ObjMode = 1;

		OpeningObjFadeOutTaskSet(id,2);
		OpeningBgFadeOutTaskSet(id,1);
		TaskTable[id].work[WAIT] = 0x40;
		TaskTable[id].TaskAdrs = FirstGameOp_Odamaki1_6_Task;
	}
}

//�X�N���[���G�t�F�N�g
static	void	FirstGameOp_Odamaki1_6_Task(u8 id)
{
	if(((s16)TaskTable[id].work[BG1_X])!= -60){
		TaskTable[id].work[BG1_X]-=2;
    	*(vu16 *)REG_BG1HOFS = TaskTable[id].work[BG1_X];
		return;
	}
	(s16)TaskTable[id].work[BG1_X]= -60;

	TaskTable[id].TaskAdrs = FirstGameOp_StatusSet1_Task;
}

//��l���o��
static	void	FirstGameOp_StatusSet1_Task(u8 id)
{
	u8	actnum;

	if(!TaskTable[id].work[FADE_SW])	return;
	ActWork[TaskTable[id].work[OP_ACTSTR_ODMK]].banish = 1;
	ActWork[TaskTable[id].work[OP_ACTSTR_PKMN]].banish = 1;

	if(TaskTable[id].work[WAIT]){
		TaskTable[id].work[WAIT]--;
		return;
	}
	actnum = TaskTable[id].work[OP_ACTSTR_MALE];
	ActWork[actnum].x = 180;
	ActWork[actnum].y =  60;
	ActWork[actnum].banish = 0;
	ActWork[actnum].oamData.ObjMode = 1;

	TaskTable[id].work[ACTNUM] = actnum;
	TaskTable[id].work[SEX_SW] = 0;

	OpeningObjFadeInTaskSet(id,2);
	OpeningBgFadeInTaskSet(id,1);

	TaskTable[id].TaskAdrs = FirstGameOp_StatusSet1_1_Task;
}

static	void	FirstGameOp_StatusSet1_1_Task(u8 id)
{
	if(!TaskTable[id].work[FADE_SW])	return;

	ActWork[TaskTable[id].work[ACTNUM]].oamData.ObjMode = 0;
	TaskTable[id].TaskAdrs = FirstGameOp_StatusSet2_Task;
}

//���ʐݒ蓱���u���Ƃ��̂��H�v
static	void	FirstGameOp_StatusSet2_Task(u8 id)
{
	NMenuWinBoxWrite( 2, 13,27, 18);
	NMenuTalkMsgSet(msg_odamaki_01_p04_opening,4,14);

	TaskTable[id].TaskAdrs = FirstGameOp_StatusSet2_1_Task;
}

//���ʐݒ菉����
static	void	FirstGameOp_StatusSet2_1_Task(u8 id)
{
	if(NMenuTalkMsgPut()){
		SexSelectInit(2,4);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet3_Task;
	}
}

//���ʐݒ�
static	void	FirstGameOp_StatusSet3_Task(u8 id)
{
	s8	res;
	u8	c_pos;

	res = SexSelectMain();

	switch(res){

	case	CURSOR_YES_VALUE:	//���Ƃ�		
		SePlay(SE_SELECT);
		MyData.my_sex = MALE;
		NMenuBoxClear( 2, 4, 8, 9);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet4_Task;
		break;
	case	CURSOR_NO_VALUE:	//�����
		SePlay(SE_SELECT);
		MyData.my_sex = FEMALE;
		NMenuBoxClear( 2, 4, 8, 9);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet4_Task;
		break;
	}
	c_pos = NMenuVCursorPosGet();
	if(c_pos!=TaskTable[id].work[SEX_SW]){		//�L�����`�F���W
		TaskTable[id].work[SEX_SW] = c_pos;
		ActWork[TaskTable[id].work[ACTNUM]].oamData.ObjMode = 1;

		OpeningObjFadeOutTaskSet(id,0);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet3_1_Task;
	}
}

//���ʐݒ莞�̎�l���؂�ւ��i��ʊO�ցj
static	void	FirstGameOp_StatusSet3_1_Task(u8 id)
{
	u8	actnum;

	actnum = TaskTable[id].work[ACTNUM];

	if(!TaskTable[id].work[FADE_SW]){
		ActWork[actnum].x += 4;
		return;
	}
	ActWork[actnum].banish = 1;

	if(TaskTable[id].work[SEX_SW])	actnum = TaskTable[id].work[OP_ACTSTR_FEML];
	else							actnum = TaskTable[id].work[OP_ACTSTR_MALE];
	ActWork[actnum].x = 240;
	ActWork[actnum].y =  60;
	ActWork[actnum].banish = 0;
	TaskTable[id].work[ACTNUM] = actnum;
	ActWork[actnum].oamData.ObjMode = 1;

	OpeningObjFadeInTaskSet(id,0);

	TaskTable[id].TaskAdrs = FirstGameOp_StatusSet3_2_Task;
}

//���ʐݒ莞�̎�l���؂�ւ��i��ʓ��ցj
static	void	FirstGameOp_StatusSet3_2_Task(u8 id)
{
	u8	actnum;

	actnum = TaskTable[id].work[ACTNUM];
	if(ActWork[actnum].x > 180){
		ActWork[actnum].x -= 4;
		return;
	}
	ActWork[actnum].x = 180;
	if(!TaskTable[id].work[FADE_SW])	return;

	ActWork[actnum].oamData.ObjMode = 0;
	TaskTable[id].TaskAdrs = FirstGameOp_StatusSet3_Task;
}

//���O�ݒ蓱���u�Ȃ܂����������āv
static	void	FirstGameOp_StatusSet4_Task(u8 id)
{
	NMenuWinBoxWrite( 2, 13,27, 18);
	NMenuTalkMsgSet(msg_odamaki_01_p05_opening,4,14);

	TaskTable[id].TaskAdrs = FirstGameOp_StatusSet5_Task;
}

//���O�ݒ菉����
static	void	FirstGameOp_StatusSet5_Task(u8 id)
{
	if(NMenuTalkMsgPut()){
		NameSelectInit(2,1);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet6_Task;
	}
}

//���O�ݒ�
static	void	FirstGameOp_StatusSet6_Task(u8 id)
{
	s8	res;

	res = NameSelectMain();

	switch(res){

	case	0:	//�����œ���
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs = FirstGameNameEntrySetTask;
		break;
	case	1:
	case	2:
	case	3:
	case	4:
		SePlay(SE_SELECT);
		NMenuBoxClear(2,1,22,12);
		DefaultNameSet(res);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet7_Task;
		break;
	case	CURSOR_CANCEL_VALUE:
		SePlay(SE_SELECT);
		NMenuBoxClear(2,1,22,12);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet2_Task;
		break;
	}
}

//���O�ݒ��ʂցu���Ԃ�ł��߂�v
static	void	FirstGameNameEntrySetTask(u8 id)
{
	if(FadeData.fade_sw == 0){
		DefaultNameSet(1);		//default���ݒ�
		
		NameIn(NAMEIN_HERO,(u8 *)MyData.my_name,MyData.my_sex,0,0,OpeningReturn);
	}
}

//�ݒ�m�F�����u�~�~�~�~�~���񂾂ˁH�v
static	void	FirstGameOp_StatusSet7_Task(u8 id)
{
	NMenuWinBoxWrite( 2, 13,27, 18);
	PM_MsgExpand(MsgExpandBuffer,msg_odamaki_01_p06_opening);
	NMenuTalkMsgSet(MsgExpandBuffer,4,14);
	TaskTable[id].TaskAdrs = FirstGameOp_StatusSet8_Task;
}

//�ݒ�m�F������
static	void	FirstGameOp_StatusSet8_Task(u8 id)
{
	if(NMenuTalkMsgPut()){
		YesNoSelectInit(2,1);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet9_Task;
	}
}

//�ݒ�m�F
static	void	FirstGameOp_StatusSet9_Task(u8 id)
{
	s8	res;

	res = YesNoSelectMain();
	switch(res){

	case	CURSOR_YES_VALUE:	//�͂�
		SePlay(SE_SELECT);
		NMenuBoxClear( 2, 1, 2+6, 2+5);
	
		ActWork[TaskTable[id].work[ACTNUM]].oamData.ObjMode = 1;

		OpeningObjFadeOutTaskSet(id,2);
		OpeningBgFadeOutTaskSet(id,1);

		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet10_Task;
		break;
	case	CURSOR_NO_VALUE:	//������
	case	CURSOR_CANCEL_VALUE:	//�L�����Z�����ꂽ
		SePlay(SE_SELECT);
		NMenuBoxClear( 2, 1, 2+6, 2+5);
		TaskTable[id].TaskAdrs = FirstGameOp_StatusSet2_Task;
		break;
	}
}

//�X�N���[���G�t�F�N�g
static	void	FirstGameOp_StatusSet10_Task(u8 id)
{
	if(TaskTable[id].work[BG1_X]!=0){
		TaskTable[id].work[BG1_X]+=2;
    	*(vu16 *)REG_BG1HOFS = TaskTable[id].work[BG1_X];
		return;
	}
	TaskTable[id].TaskAdrs = FirstGameOp_Odamaki2_1_Task;
}

//�I�_�}�L���|�P�����ēo��
static	void	FirstGameOp_Odamaki2_1_Task(u8 id)
{
	u8	actnum;

	if(!TaskTable[id].work[FADE_SW])	return;

	ActWork[TaskTable[id].work[OP_ACTSTR_MALE]].banish = 1;
	ActWork[TaskTable[id].work[OP_ACTSTR_FEML]].banish = 1;

	actnum = TaskTable[id].work[OP_ACTSTR_ODMK];
	ActWork[actnum].x = 136;
	ActWork[actnum].y =  64;
	ActWork[actnum].banish = 0;
	ActWork[actnum].oamData.ObjMode = 1;
	actnum = TaskTable[id].work[OP_ACTSTR_PKMN];
	ActWork[actnum].x = 104;
	ActWork[actnum].y =  68+4;
	ActWork[actnum].banish = 0;
	ActWork[actnum].oamData.ObjMode = 1;

	OpeningObjFadeInTaskSet(id,2);
	OpeningBgFadeInTaskSet(id,1);

	NMenuWinBoxWrite( 2, 13,27, 18);
	PM_MsgExpand(MsgExpandBuffer,msg_odamaki_01_p07_opening);
	NMenuTalkMsgSet(MsgExpandBuffer,4,14);
	TaskTable[id].TaskAdrs = FirstGameOp_Odamaki2_2_Task;
}

//��b�S�u�������I�L�~���v
static	void	FirstGameOp_Odamaki2_2_Task(u8 id)
{
	if(!TaskTable[id].work[FADE_SW])	return;
	
	ActWork[TaskTable[id].work[OP_ACTSTR_ODMK]].oamData.ObjMode = 0;
	ActWork[TaskTable[id].work[OP_ACTSTR_PKMN]].oamData.ObjMode = 0;

	if(NMenuTalkMsgPut()){
		ActWork[TaskTable[id].work[OP_ACTSTR_ODMK]].oamData.ObjMode = 1;
		ActWork[TaskTable[id].work[OP_ACTSTR_PKMN]].oamData.ObjMode = 1;

		OpeningObjFadeOutTaskSet(id,2);
		OpeningBgFadeOutTaskSet(id,1);

		TaskTable[id].work[WAIT] = 0x40;
		TaskTable[id].TaskAdrs = FirstGameNameEntryExit1Task;
	}
}

//��l���ēo��
static	void	FirstGameNameEntryExit1Task(u8 id)
{
	u8	actnum;

	if(!TaskTable[id].work[FADE_SW])	return;
	ActWork[TaskTable[id].work[OP_ACTSTR_ODMK]].banish = 1;
	ActWork[TaskTable[id].work[OP_ACTSTR_PKMN]].banish = 1;

	if(TaskTable[id].work[WAIT]){
		TaskTable[id].work[WAIT]--;
		return;
	}
	if(MyData.my_sex)	actnum = TaskTable[id].work[OP_ACTSTR_FEML];
	else				actnum = TaskTable[id].work[OP_ACTSTR_MALE];
	ActWork[actnum].x = 120;
	ActWork[actnum].y =  60;
	ActWork[actnum].banish = 0;
	ActWork[actnum].oamData.ObjMode = 1;
	TaskTable[id].work[ACTNUM] = actnum;

	OpeningObjFadeInTaskSet(id,2);
	OpeningBgFadeInTaskSet(id,1);

	NMenuWinBoxWrite( 2, 13,27, 18);
	NMenuTalkMsgSet(msg_odamaki_01_p08_opening,4,14);

	TaskTable[id].TaskAdrs = FirstGameNameEntryExit2Task;
}

//��b�T�u��[���@�����́v
static	void	FirstGameNameEntryExit2Task(u8 id)
{
	u8	actnum;

	if(!TaskTable[id].work[FADE_SW])	return;
	ActWork[TaskTable[id].work[ACTNUM]].oamData.ObjMode = 0;

	if(NMenuTalkMsgPut()){
		actnum = TaskTable[id].work[ACTNUM];

		ActWork[actnum].oamData.AffineMode = 1;
		ActWork[actnum].aff_tbl = OpEffAffTable;
		AffineNumSet(&ActWork[actnum]);
		ActAffAnmChg(&ActWork[actnum],0);
		ActWork[actnum].move = HeroMoveAffine;
		PaletteFadeReq(0x0000ffff,0,0,16,0);
		MusicFadeOutEnd(4);

		TaskTable[id].TaskAdrs = FirstGameExitEffectInitTask;
	}
}

//�I���G�t�F�N�g������
static	void	FirstGameExitEffectInitTask(u8 id)
{
	u8	actnum;

	actnum = TaskTable[id].work[ACTNUM];

	if(ActWork[actnum].affend_sw){
		TaskTable[id].TaskAdrs = FirstGameExitEffectTask;
	}
}

//�I���G�t�F�N�g
static	void	FirstGameExitEffectTask(u8 id)
{
	u8	actnum;

	if(FadeData.fade_sw == 0){
		actnum = TaskTable[id].work[ACTNUM];
		ActWork[actnum].move = OpeningNoMove;

		*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                 	         	|DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							  	|DISP_OBJ_ON;
		PaletteFadeReq(0xffff0000,0,0,16,0xffff);
		TaskTable[id].TaskAdrs = FirstGameExitTask;
	}
}

//�Q�[���J�n
static	void	FirstGameExitTask(u8 id)
{
	if(FadeData.fade_sw == 0){
		MainProcChange(NormalGameStart);
		DelTask(id);
	}
}

/********************************************************************/
/*				���O���͂���̋A��									*/
/********************************************************************/
void OpeningReturn( void )
{
	u8	taskID,actnum;

	SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

    *(vu16 *)REG_DISPCNT	= 0;
    *(vu16 *)REG_BG2CNT		= 0;
    *(vu16 *)REG_BG1CNT		= 0;
    *(vu16 *)REG_BG0CNT		= 0;

	*(vu16 *)REG_BG2HOFS	= 0;
	*(vu16 *)REG_BG2VOFS	= 0;
    *(vu16 *)REG_BG1HOFS	= 0;
    *(vu16 *)REG_BG1VOFS	= 0; 
    *(vu16 *)REG_BG0HOFS	= 0;
    *(vu16 *)REG_BG0VOFS	= 0; 

    DmaClear(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
    DmaClear(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
    DmaClear(3, 0, PLTT, PLTT_SIZE,           16);	// ��گ�  ������

	PaletteAnimeInit();

	LZ77UnCompVram((void*)spotch_LZ,(void*)(BG_VRAM+0x0000));	//BG CGX�]��
	LZ77UnCompVram((void*)spotsc_LZ,(void*)(BG_VRAM+0x3800));	//BG MAP�]��
	PaletteWorkSet( op_back_Palette, PA_BG0, 0x0040 );			//BG COL�]��

	InitTask();										// ���  ������
	taskID = AddTask(FirstGameOp_StatusSet7_Task, 0);
	TaskTable[taskID].work[BG1_X]=-60;
	
	LasterInit();									// HDMA������
	InitActor();									// ���� ������
	ObjPalManInit();									// ��گ��Ȱ�ެ
	OpeningStartActorSet(taskID);

	PrintScreenInit(DEFAULT_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);

	if(MyData.my_sex){
		TaskTable[taskID].work[SEX_SW]		= 1;
		actnum = TaskTable[taskID].work[OP_ACTSTR_FEML];
	}else{
		TaskTable[taskID].work[SEX_SW]		= 0;
		actnum = TaskTable[taskID].work[OP_ACTSTR_MALE];
	}
	ActWork[actnum].x = 180;
	ActWork[actnum].y =  60;
	ActWork[actnum].banish = 0;
	TaskTable[taskID].work[ACTNUM]	= actnum;

    *(vu16 *)REG_BG1HOFS	= -60;

	PaletteFadeReq(0xffffffff,0,16,0,0);

	*(vu16 *)REG_WIN0H	= 0;
	*(vu16 *)REG_WIN0V	= 0;
	*(vu16 *)REG_WININ	= 0;
	*(vu16 *)REG_WINOUT	= 0;

   	*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 0;

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(OpeningMain);

	*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| 7 << BG_SCREEN_BASE_SHIFT		//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_BG1_ON					// BG1�L��
						  | DISP_OBJ_ON;
}


/********************************************************************/
/*                                                                  */
/*				OBJ�p����֐�										*/
/*                                                                  */
/********************************************************************/
static void OpeningNoMove(actWork* aw){}
static void HeroMoveAffine(actWork* aw)
{
	u32	y;

	y = (aw->y<<16) + aw->work[0];
	y += 0xc000;
	aw->y		= y>>16;
	aw->work[0] = y;
}

static	const	actAffAnm	OpEffAffPat0[] = {
	{-0x02,-0x02,0,0x30},
	{ACT_AFEND,0,0,0},
};

static	const	actAffAnm*	const	OpEffAffTable[] = {
	OpEffAffPat0,
};

static	u8	OpeningStandPokeSet(u8 x,u8 y)
{
	u8	actnum;

//POKEMON�\��
	DecordPokeCellMake( &PM2FObjDataCell[OPENING_POKEMON],
						pm2fpara[OPENING_POKEMON].size,
						pm2fpara[OPENING_POKEMON].patcnt,
						(void *)DecordAdrs[0],
						(void *)DecordAdrs[1],
						OPENING_POKEMON);

	DecordPalSet( (PalData*)&PM2NObjDataPal[OPENING_POKEMON] );

	PokeActHeaderMake( OPENING_POKEMON, 1 );
	actnum = AddActor( &PokeActHeader, x, y, 0 );
	return	actnum;
}

static void OpeningStartActorSet(u8 id)
{
	u8	actnum;

	actnum = StandOdamakiSet(136,60,1);
	ActWork[actnum].move = OpeningNoMove;
	ActWork[actnum].oamData.Priority = 0;
	ActWork[actnum].banish = 1;
	TaskTable[id].work[OP_ACTSTR_ODMK]	= actnum;

	actnum = OpeningStandPokeSet(104,68+4);
	ActWork[actnum].move = OpeningNoMove;
	ActWork[actnum].oamData.Priority = 0;
	ActWork[actnum].banish = 1;
	TaskTable[id].work[OP_ACTSTR_PKMN]	= actnum;

	actnum = SetStandTrainerActor(0,120,60,0,UserWork);
	ActWork[actnum].move = OpeningNoMove;
	ActWork[actnum].banish = 1;
	ActWork[actnum].oamData.Priority = 0;
	TaskTable[id].work[OP_ACTSTR_MALE]	= actnum;

	actnum = SetStandTrainerActor(1,120,60,0,UserWork+0x800);
	ActWork[actnum].move = OpeningNoMove;
	ActWork[actnum].banish = 1;
	ActWork[actnum].oamData.Priority = 0;
	TaskTable[id].work[OP_ACTSTR_FEML]	= actnum;
}

/********************************************************************/
/*                                                                  */
/*				�t�F�[�h�֐�										*/
/*                                                                  */
/********************************************************************/
/* OBJ */
static	void OpeningObjFadeOutTask(u8 id)
{
	if(!TaskTable[id].work[1]){
		TaskTable[TaskTable[id].work[0]].work[FADE_SW] = 1;
		DelTask(id);
		return;
	}
	if(TaskTable[id].work[4]){
		TaskTable[id].work[4]--;
		return;
	}
	TaskTable[id].work[4] = TaskTable[id].work[3];
	TaskTable[id].work[1]--;
	TaskTable[id].work[2]++;
   	*(vu16 *)REG_BLDALPHA = (u16)(TaskTable[id].work[2]*0x100 + TaskTable[id].work[1]);
}

static	void	OpeningObjFadeOutTaskSet(u8 flg_task_id,u8 wait)
{
	u8	taskID;

   	*(vu16 *)REG_BLDCNT   = 0x0250;	//�������ݒ�(OBJ->BG1)
   	*(vu16 *)REG_BLDALPHA = 0x0010;
   	*(vu16 *)REG_BLDY	  = 0;

	TaskTable[flg_task_id].work[FADE_SW] = 0;
	taskID = AddTask(OpeningObjFadeOutTask, 0);
	TaskTable[taskID].work[0] 	= flg_task_id;
	TaskTable[taskID].work[1] 	= 0x10;
	TaskTable[taskID].work[2] 	= 0x00;
	TaskTable[taskID].work[3] 	= wait;
	TaskTable[taskID].work[4] 	= wait;
}

static	void OpeningObjFadeInTask(u8 id)
{
	if(TaskTable[id].work[1]==0x10){
		TaskTable[TaskTable[id].work[0]].work[FADE_SW] = 1;
		DelTask(id);
		return;
	}
	if(TaskTable[id].work[4]){
		TaskTable[id].work[4]--;
		return;
	}
	TaskTable[id].work[4] = TaskTable[id].work[3];
	TaskTable[id].work[1]++;
	TaskTable[id].work[2]--;
   	*(vu16 *)REG_BLDALPHA = (u16)(TaskTable[id].work[2]*0x100 + TaskTable[id].work[1]);
}

static	void	OpeningObjFadeInTaskSet(u8 flg_task_id,u8 wait)
{
	u8	taskID;

   	*(vu16 *)REG_BLDCNT   = 0x0250;	//�������ݒ�(OBJ->BG1)
   	*(vu16 *)REG_BLDALPHA = 0x1000;
   	*(vu16 *)REG_BLDY	  = 0;

	TaskTable[flg_task_id].work[FADE_SW] = 0;
	taskID = AddTask(OpeningObjFadeInTask, 0);
	TaskTable[taskID].work[0] 	= flg_task_id;
	TaskTable[taskID].work[1] 	= 0x00;
	TaskTable[taskID].work[2] 	= 0x10;
	TaskTable[taskID].work[3] 	= wait;
	TaskTable[taskID].work[4] 	= wait;
}

static	void OpeningBgFadeOutTask(u8 id)
{
	if(TaskTable[id].work[2]){
		TaskTable[id].work[2]--;
		return;
	}
	if(TaskTable[id].work[1]==8){
		DelTask(id);
		return;
	}
	if(TaskTable[id].work[4]){
		TaskTable[id].work[4]--;
		return;
	}
	TaskTable[id].work[4] = TaskTable[id].work[3];
	TaskTable[id].work[1]++;
	PaletteWorkSet(&OpeningBGFadeColTable[TaskTable[id].work[1]],PA_BG0+1,2*8);
}

static	void	OpeningBgFadeOutTaskSet(u8 flg_task_id,u8 wait)
{
	u8	taskID;

	taskID = AddTask(OpeningBgFadeOutTask, 0);
	TaskTable[taskID].work[0] 	= flg_task_id;
	TaskTable[taskID].work[1] 	= 0x00;
	TaskTable[taskID].work[2] 	= 0x08;
	TaskTable[taskID].work[3] 	= wait;
	TaskTable[taskID].work[4] 	= wait;
}

static	void OpeningBgFadeInTask(u8 id)
{
	if(TaskTable[id].work[2]){
		TaskTable[id].work[2]--;
		return;
	}
	if(!TaskTable[id].work[1]){
		DelTask(id);
		return;
	}
	if(TaskTable[id].work[4]){
		TaskTable[id].work[4]--;
		return;
	}
	TaskTable[id].work[4] = TaskTable[id].work[3];
	TaskTable[id].work[1]--;
	PaletteWorkSet(&OpeningBGFadeColTable[TaskTable[id].work[1]],PA_BG0+1,2*8);
}

static	void	OpeningBgFadeInTaskSet(u8 flg_task_id,u8 wait)
{
	u8	taskID;

	taskID = AddTask(OpeningBgFadeInTask, 0);
	TaskTable[taskID].work[0] 	= flg_task_id;
	TaskTable[taskID].work[1] 	= 0x8;
	TaskTable[taskID].work[2] 	= 0x08;
	TaskTable[taskID].work[3] 	= wait;
	TaskTable[taskID].work[4] 	= wait;
}


/********************************************************************/
/*                                                                  */
/*				�I����޳�֌W										*/
/*                                                                  */
/********************************************************************/
//----------------------------------------------------------------------------------
//	�I���E�B���h�E�̐���
// <�߂�l>
//	CURSOR_CANCEL_VALUE		�L�����Z�����ꂽ
//	CURSOR_DEFAULT_VALUE	�܂��I������Ă��Ȃ�
//	CURSOR_YES_VALUE		�͂���I��(����1)
//	CURSOR_NO_VALUE			��������I��(����2)
//----------------------------------------------------------------------------------
/*		����	*/
const MENU_ITEM	sexcheck_list[]=
{
	{ sex1_msg,	NULL },	//00:���Ƃ�
	{ sex2_msg,	NULL }	//01:�����
};

#define SEX_SELECT_MAX	NELEMS(sexcheck_list)

void SexSelectInit(u8	x, u8 y)
{
	NMenuWinBoxWrite( x, y, x+6, y+5 );
	NMenuFixListWrite(x+2,y+1,SEX_SELECT_MAX,sexcheck_list);
	NMenuVCursorInit(x+1,y+1,SEX_SELECT_MAX,0);
}

s8 SexSelectMain(void)
{
	return NMenuVCursorControlNoloop();
}


const MENU_ITEM	namecheck_list_m[]=
{
	{ op_name0_msg,	NULL },	//00:�����œ���
	{ op_name1_msg,	NULL },	//���O01
	{ op_name2_msg,	NULL },	//���O02
	{ op_name3_msg,	NULL },	//���O03
	{ op_name4_msg,	NULL }	//���O04
};

const MENU_ITEM	namecheck_list_f[]=
{
	{ op_name0_msg,	NULL },	//00:�����œ���
	{ op_name5_msg,	NULL },	//���O05
	{ op_name6_msg,	NULL },	//���O06
	{ op_name7_msg,	NULL },	//���O07
	{ op_name8_msg,	NULL }	//���O08
};

void NameSelectInit(u8	x, u8 y)
{
	NMenuWinBoxWrite( x, y, x+10, y+11 );
	if(MyData.my_sex == MALE){
		NMenuFixListWrite(x+2,y+1,5,namecheck_list_m);
	}else{
		NMenuFixListWrite(x+2,y+1,5,namecheck_list_f);
	}
	NMenuVCursorInit(x+1,y+1,5,0);
}

s8	NameSelectMain(void)
{
	return NMenuVCursorControl();
}

static	void	DefaultNameSet(u8	num)
{
	u8	i;
	const	u8*	namedata;

	if(MyData.my_sex == MALE){
		namedata = namecheck_list_m[num].msg;
	}else{
		namedata = namecheck_list_f[num].msg;
	}
	for(i=0;i<5;i++)	MyData.my_name[i] = namedata[i];
	MyData.my_name[5] = EOM_;
}
























