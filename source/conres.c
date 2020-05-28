//=========================================================================
//
//		�R���e�X�g���ʔ��\���
//											by matsuda	2001.10.11(��)
//=========================================================================

#include "common.h"
#include "intr.h"
#include "poketool.h"
#include "contest.h"
#include "con_tool.h"
#include "fight.h"
#include "bss.h"
#include "print.h"
#include "decord.h"
#include "palanm.h"
#include "laster.h"
#include "task.h"
#include "actanm.h"
#include "server.h"
#include "agbmons.h"
#include "calctool.h"
#include "waza_eff.h"
#include "con_waza.h"
#include "conres.h"
#include "pokeicon.h"
#include "madefine.h"
#include "wazatool.h"
#include "fld_main.h"
#include "script.h"
#include "objdata.h"
#include "conscr.h"
#include "consio.h"
#include "multisio.h"
#include "pokesio.h"
#include "letter.h"
#include "cable.h"
#include "fan.h"
#include "ef_tool.h"
#include "mus_tool.h"
#include "zukan.h"


//----------------------------------------------------------
//	�O���[�o���ϐ�
//----------------------------------------------------------

//----------------------------------------------------------
//	�O���ϐ�
//----------------------------------------------------------
//extern	u8	FightMsgBuf[64];
extern	TALK_WIN	win[4];

//----------------------------------------------------------
//	�O���ް�
//----------------------------------------------------------
extern const u32 DecordAdrs[];
extern const u16 p_icon_Palette[];

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void	MainContestResult(void);
static void conresult_v_blank_intr(void);
static void InitFadeWaitSeq(u8 id);
static void ContestEndSeq(u8);
static void ContestEndFadeWait(u8);
//static void FadeInWaitSeq(void);
//static void FadeInEndCheck(u8 id);
static void EvaluationGetWait(u8 id);
static void TuusinCheckEnd(u8);
static void EvaluationGetInit(u8 id);
//static void ConnectNumCheck(u8);
//static void Sio10SyncWait(u8 id);
static void SioEndCodeGetWait(u8 id);
static void EvaluationGetWait(u8 id);
//-- �V�K 2002.06.24(��) --//
static void PokeIconBgSet(u16 monsno, u8 posno, u8 anmno, u8 scrn_sw, u32);
static void PokeIconBgAllSet(u8 anmno, u8 scrn_sw);
static void PokeIconBgPaletteSet(void);
static void HaikeiScrollTask(u8 id);
static void StarHeartFadeTask(u8 id);
static void HyoudaiWriteScrn(void);
static u8 GetStarNum(u8 brd, u8);
static s8 GetHeartNum(u8 brd, u8);
static void MsgWinObjMake(const u8 *msg, u8);
static void MsgWinObjSet(void);
static void MsgWinObjSlidInSet(s16 x, s16 y, u16 wait, u16 sp);
static void MsgWinObjSlidOutSet(u16 sp);
static void MsgWinObjDefaultSet(actWork *xreg);
static void MsgWinObjSlidInSeq(actWork *xreg);
static void MsgWinObjWaitSeq(actWork *xreg);
static void MsgWinObjSlideOutSeq(actWork *xreg);
static void ItijiSinsaGaugeInit(u8 id);
static void KekkaHappyouMsgSlidIn(u8 id);
static void JyunniScrnWriteSeq(u8 id);
static void ChampWinPaletteAnime(u8 id);
static void ChampWinPaletteAnimeMain(u8 id);
static void PokeSlidInAppear(actWork *xreg);
static void PokeSlideOut(actWork *xreg);
static void KamifubukiTask(u8 id);
static void CresKamifubukiSeq(actWork *xreg);
static void EndKeyWait(u8 id);
static void ChampAppearEffect(u8 id);
static void NijiSinsaMsgInit(u8 id);
static void NijiSinsaGaugeInit(u8 id);
static void JyunniNumPut(u8 id);
static u16 GetMsgWinCenterPos(const u8 *msg);
static void SioWinMsgPut(const u8 *msg);
static void SioWinMsgElase(void);
static void ResultTuusinEndWait(u8 id);
static void PokeIconBGAnmSet(u8 brd, u8 anm_wait);
static void PokeIconBGAnmSeq(u8 id);
static void ResultMeterCalc(void);

static void StarHeartFadeEndCheck(u8 hosi_heart, u8 count);
static void MeterUpDownSeq(u8 id);

//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
#include "..\scaddata\kekka_w.ach"
#include "..\scaddata\kekka_w.acl"
#include "conres.dat"

//----------------------------------------------------------
//	���b�Z�[�W�f�[�^
//----------------------------------------------------------
#include "..\msgdata\conmsg.h"


//----------------------------------------------------------
//	�v���O����
//----------------------------------------------------------

//----------------------------------------------------------
//	ڼ޽��������
//----------------------------------------------------------
static void RegisterInit(void)
{
	*(vu16 *)REG_DISPCNT = DISP_MODE_0 | DISP_OBJ_CHAR_1D_MAP;

	PrintScreenInit(CONRES1_TYPE);
	MsgControlInit(&win[0],CONRES1_TYPE);		//ү���ގg�pBGڼ޽�������

	//-- ������ --//
	*(vu16 *)REG_BG0CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_0
	| CRES_EFF_SCRN_NO << BG_SCREEN_BASE_SHIFT 
	| CRES_EFF_CHARBASE_NO << BG_CHAR_BASE_SHIFT
	| BG_COLOR_16 | BG_MOS_OFF | BG_LOOP_ON;

	//�t�H���g��
	*(vu16 *)REG_BG1CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_3
	| CRES_FONT_SCRN_NO << BG_SCREEN_BASE_SHIFT 
	| CRES_FONT_CHARBASE_NO << BG_CHAR_BASE_SHIFT
	| BG_COLOR_16 | BG_MOS_OFF;

	//����޳
	*(vu16 *)REG_BG2CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_3
	| CRES_WIN_SCRN_NO << BG_SCREEN_BASE_SHIFT 
	| CRES_HAIKEI_CHARBASE_NO << BG_CHAR_BASE_SHIFT
	| BG_COLOR_16 | BG_MOS_OFF;

	//�w�i
	*(vu16 *)REG_BG3CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_3
	| CRES_HAIKEI_SCRN_NO << BG_SCREEN_BASE_SHIFT 
	| CRES_HAIKEI_CHARBASE_NO << BG_CHAR_BASE_SHIFT
	| BG_COLOR_16 | BG_MOS_OFF | BG_LOOP_ON;

	*(vu16*)REG_MOSAIC = 0;

	*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
	*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | (WIN_ALL_ON^(WIN_OBJ_ON|WIN_BG0_ON));

	*(vu16*)REG_WIN0H = 0;
	*(vu16*)REG_WIN0V = 0;
	*(vu16*)REG_WIN1H = 0;
	*(vu16*)REG_WIN1V = 0;

	*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
	*(vu16 *)REG_BLDALPHA = 0;
	*(vu16 *)REG_BLDY	  = 0;

	*(vu16 *)REG_BG0HOFS = 0;
	*(vu16 *)REG_BG0VOFS = 0;
	*(vu16 *)REG_BG1HOFS = 0;
	*(vu16 *)REG_BG1VOFS = 0;
	*(vu16 *)REG_BG2HOFS = 0;
	*(vu16 *)REG_BG2VOFS = 0;
	*(vu16 *)REG_BG3HOFS = 0;
	*(vu16 *)REG_BG3VOFS = 0;

	*(vu16 *)REG_DISPCNT |= DISP_BG0_ON | DISP_BG1_ON | DISP_BG2_ON 
							| DISP_BG3_ON | DISP_OBJ_ON | DISP_WIN01_ON;
	
	ScrX0=0;
	ScrY0=0;
	ScrX1=0;
	ScrY1=0;
	ScrX2=0;
	ScrY2=0;
	ScrX3=0;
	ScrY3=0;
	Win0H = 0;
	Win0V = 0;
	Win1H = 0;
	Win1V = 0;
}

//----------------------------------------------------------
//	����BG��ׁA��ذ݁A�װ�]��
//----------------------------------------------------------
static void BGinit(void)
{
	int i, x;
	u16 heart_scrn, star_scrn;
	s8 star_num, heart_num;
	
	DIV_DMACLEAR(3,0,VRAM,(VRAM_END-VRAM),32);

	//�L����
	DecordVram((void*)kekka_bg_pch_ADRS, (void*)CRES_HAIKEI_VRAM);	//�S��
	
	//�X�N���[��
	DecordVram((void*)kekka_bg3_psc_ADRS, (void*)CRES_HAIKEI_SCRN);	//�w�i
	DecordVram((void*)kekka_bg2_psc_ADRS, (void*)CRES_WIN_SCRN);	//����޳
	DecordVram((void*)kekka_bg0_psc_ADRS, (void*)CRES_EFF_SCRN);	//���ӂԂ�
	HyoudaiWriteScrn();
	
	//�p���b�g
	DecordPaletteWork((void*)kekka_pcl_ADRS, PA_BG0, 0x200);
	PrintPaletteInit(CONRES1_TYPE);	//BG��گē]����̫����گĂ��㏑������Ă��܂��̂�

	//-- ���A�n�[�g�̃L������z�u --//
	for(i = 0; i < BREEDER_MAX; i++){
		star_num = GetStarNum(i, 1);
		heart_num = GetHeartNum(i, 1);
		for(x = 0; x < 10; x++){
			if(x < star_num)
				star_scrn = CRES_STAR_FADE;
			else
				star_scrn = CRES_STAR_SPACE;
			
			if(x < abs(heart_num)){
				if(heart_num < 0)
					heart_scrn = CRES_HEARTBL_FADE;
				else
					heart_scrn = CRES_HEART_FADE;
			}
			else
				heart_scrn = CRES_HEART_SPACE;
			
			*(vu16*)(CRES_FONT_SCRN+
				(0x20*(CRES_STAR_Y0+i*3) + CRES_STAR_X0+x)*2) = star_scrn;
			*(vu16*)(CRES_FONT_SCRN+
				(0x20*(CRES_HEART_Y0+i*3) + CRES_HEART_X0+x)*2) = heart_scrn;
		}
	}
}

//----------------------------------------------------------
//	�u���[�_�[�ƃ|�P�����̖��O��\��
//----------------------------------------------------------
#define CRES_POKENAME_X	7
#define CRES_BRDNAME_X		12	// "/"�ƃu���[�_�[�̖��O
static const u8 RedFont[] = {CTRL_,C_FCOL_,COL_RED,EOM_};
static const u8 sura_msg[] = {sura_,EOM_};
static void BreederNamePut(u8 brd)
{
	u16 y;
	u8 *pmsg;
	
	y = 4;
	y += 3*brd;
	if(brd == MyBreederNo)
		pmsg = PM_strcpy(FightMsgBuf, RedFont);
	else
		pmsg = FightMsgBuf;
	PM_strcpy(pmsg, BreederParam[brd].nickname);
	MsgPrint(&win[0], FightMsgBuf, 
			CRES_POKE_NAMECGX + brd*CRES_POKENAME_LEN, CRES_POKENAME_X, y);
	if(brd == MyBreederNo)
		pmsg = PM_strcpy(FightMsgBuf, RedFont);
	else
		pmsg = FightMsgBuf;
	PM_strcpy(pmsg, sura_msg);
	PM_strcat(pmsg, BreederParam[brd].name);
	MsgPrint(&win[0], FightMsgBuf, CRES_BRD_NAMECGX + brd*CRES_BRDNAME_LEN,
				CRES_BRDNAME_X, y);
}

static void BreederNamePutAll(void)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++)
		BreederNamePut(i);
}

//=========================================================================
//	�����ݒ�
//=========================================================================
void ContestResultInit(void)
{
	FadeData.trans_stop = 1;
	
//	r_next = temp_rand;
	
	SetVBlankFunc(NULL);
	RegisterInit();
	LasterWorkInit();
	PaletteAnimeInit();
	InitActor();
	InitTask();
	ObjPalManInit();
//	ObjPalManStart=4;				// ��گ��Ȱ�ެNo�J�n
	BGinit();

	PokeIconBgPaletteSet();
	PokeIconBgAllSet(0, 1);
	BreederNamePutAll();

	memset(cres, 0, sizeof(CresSys));	//���э\���̏�����
	memset(cgraph, 0, sizeof(ConGraph)*BREEDER_MAX);
	
	MsgWinObjSet();
	
	PaletteFadeReq(0xffffffff, 0,16,0,0x0000);
	FadeData.trans_stop = 0;
	
	cres->main_id = AddTask(InitFadeWaitSeq, 5);
	MainProcChange(MainContestResult);
	
	Win1H = (0 << 8) | 240;		//ү���޳���޳������镔������޳�ŕ���
	Win1V = (128 << 8) | 160;
	
	AddTask(HaikeiScrollTask, 20);	//�w�i�X�N���[��

	ResultMeterCalc();			//Ұ���v�Z
	
	MusicPlay(MUS_CON_K);
	SetVBlankFunc(conresult_v_blank_intr);
}

//----------------------------------------------------------
//	���C�����[�v
//----------------------------------------------------------
static void	MainContestResult(void)
{
	JumpActor();
	SetActor();
	MainTask();
	PaletteAnime();
}

//----------------------------------------------------------
//	V�u�����N�֐�
//----------------------------------------------------------
static void conresult_v_blank_intr(void)
{
	*(vu16 *)REG_BG0HOFS = ScrX0;
	*(vu16 *)REG_BG0VOFS = ScrY0;
	*(vu16 *)REG_BG1HOFS = ScrX1;
	*(vu16 *)REG_BG1VOFS = ScrY1;
	*(vu16 *)REG_BG2HOFS = ScrX2;
	*(vu16 *)REG_BG2VOFS = ScrY2;
	*(vu16 *)REG_BG3HOFS = ScrX3;
	*(vu16 *)REG_BG3VOFS = ScrY3;
	*(vu16 *)REG_WIN0H = Win0H;
	*(vu16 *)REG_WIN0V = Win0V;
	*(vu16 *)REG_WIN1H = Win1H;
	*(vu16 *)REG_WIN1V = Win1V;
	actOAMtrans();
	actDMAtrans();
	PaletteFadeTrans();
	LasterVblank();
}


//=========================================================================
//	�V�[�P���X
//=========================================================================

//----------------------------------------------------------
//	̪��޲ݏI���܂ő҂�
//----------------------------------------------------------
static void InitFadeWaitSeq(u8 id)
{
	if(FadeData.fade_sw == 0)
	{
		if(ContestTuusin & C_TUUSIN_FLG)
		{
			/*
			ScrY1 = 48;
			MsgPrint(&win[0], tuusintyuu_msg, CRES_WINMSG, 1, 0x0f);//0x15);
			UserWork[0] = 0;
			*/
			SioWinMsgPut(tuusin_taiki_msg);
			TaskTable[id].TaskAdrs = EvaluationGetInit;
		}
		else
			TaskTable[id].TaskAdrs = KekkaHappyouMsgSlidIn;
	}
}

//+++++++++++++ �ʐM ++++++++++++++++++++++

//----------------------------------------------------------
//	�]���l��ʐM�Ŏ擾����܂ų���
//----------------------------------------------------------
static void EvaluationGetInit(u8 id)
{
	u8 task;
	
	if(pokemon_sio_flag == OFF)
		return;
	
	if(GetConnectSioMenberNum() == 4)
	{
	//	task = AddTask(EvaluationSioGet, 0);
	//	ChangeTask(task, EvaluationSioGet, EvaluationGetWait);
		task = AddTask(EvaluationGetWait, 0);//2���R���I�����ɒʐM����悤�ɕύX�����̂�
		
		TaskTable[id].TaskAdrs = DummyTaskSeq;
	}
}

static void EvaluationGetWait(u8 id)
{
//	ChangeTask(id, SioEndCodeSend, SioEndCodeGetWait);
	ChangeTask(id, ContestStandbyCheck, SioEndCodeGetWait);
}

static void SioEndCodeGetWait(u8 id)
{
	if(SplitDataSendStatus()){
		//PokemonMultiSioEnd();		2002.06.27(��)
		DelTask(id);
		TaskTable[cres->main_id].TaskAdrs = KekkaHappyouMsgSlidIn;
		SioWinMsgElase();
//		FontScrnBoxFill(&win[0], CRES_SPACE, 1, 0x0f, 19, 0x0f+3);//0x15+3);
//		ScrY1 = 0;

	//	ContReceptionBuffClear();
	//	SplitDataResultReset();
	}
}

// ++++++++++++++++++++++++++


//----------------------------------------------------------
//	�u�������͂��҂傤�v�̕������X���C�h�C��
//----------------------------------------------------------
static void KekkaHappyouMsgSlidIn(u8 id)
{
	u16 x;
	
	if(TaskTable[id].work[MAIN_SEQ] == 0){
		AddTask(StarHeartFadeTask, 20);	//����ʰĂ�̪���
		MsgWinObjMake(kekkahappyou_msg, cres->win_act);
		x = GetMsgWinCenterPos(kekkahappyou_msg);
		MsgWinObjSlidInSet(x, MSGWINOBJ_DEFAULT_Y, 60*2, MSGWINOBJ_INOUT_SP);
		TaskTable[id].work[MAIN_SEQ]++;
	}
	else if(TaskTable[id].work[MAIN_SEQ] == 1){	//ү���� �ײ�޲ݑ҂�
		if(cres->win_status == WO_TAIKI_OUT){
			TaskTable[id].work[MAIN_TEMP] = 0;
			TaskTable[id].work[MAIN_SEQ]++;
		}
	}
	else if(TaskTable[id].work[MAIN_SEQ] == 2){	//�ײ�ޱ�Č�A������Ƴ���
		if(TaskTable[id].work[MAIN_TEMP]++ == NEXT_MSG_WAIT){
			TaskTable[id].work[MAIN_TEMP] = 0;
			TaskTable[id].work[MAIN_SEQ]++;
		}
	}
	else if(TaskTable[id].work[MAIN_SEQ] == 3){	//�u�ꎞ�R���̌��ʁv��ү���� �ײ�޲�
		MsgWinObjMake(itiji_kekka_msg, cres->win_act);
		x = GetMsgWinCenterPos(itiji_kekka_msg);
		MsgWinObjSlidInSet(x, MSGWINOBJ_DEFAULT_Y, 0xffff, MSGWINOBJ_INOUT_SP);
		TaskTable[id].work[MAIN_SEQ]++;
	}
	else if(TaskTable[id].work[MAIN_SEQ] == 4){	//�ҋ@�ɂȂ�̂�҂�
		if(cres->win_status == WO_TAIKI_IN){
			TaskTable[id].work[MAIN_SEQ] = 0;
			TaskTable[id].TaskAdrs = ItijiSinsaGaugeInit;
		}
	}
}

//----------------------------------------------------------
//	1���R���̃Q�[�W�v�Z
//----------------------------------------------------------
enum{
	ISG_0,
	ISG_1,
	ISG_2,
	ISG_3,
};
static void ItijiSinsaGaugeInit(u8 id)
{
	switch(TaskTable[id].work[MAIN_SEQ]){
		case ISG_0:
			if(cres->gauge_fade == 0){
				StarHeartFadeEndCheck(0, TaskTable[id].work[MAIN_WORK0]++);
				if(cres->meter_eff == 0)
					TaskTable[id].work[MAIN_SEQ] = ISG_2;
				else
					TaskTable[id].work[MAIN_SEQ]++;
			}
			break;
		case ISG_1:
			if(cres->meter_eff == 0)
				TaskTable[id].work[MAIN_SEQ] = ISG_0;
			break;
		case ISG_2:
			MsgWinObjSlidOutSet(MSGWINOBJ_INOUT_SP);
			TaskTable[id].work[MAIN_SEQ] = 0;
			TaskTable[id].work[MAIN_WORK0] = 0;
			TaskTable[id].TaskAdrs = NijiSinsaMsgInit;
			break;
	}
}

//----------------------------------------------------------
//	�Q���R���̃��b�Z�[�W�\��
//----------------------------------------------------------
static void NijiSinsaMsgInit(u8 id)
{
	u16 x;
	
	if(cres->win_status == WO_TAIKI_OUT){
		if(TaskTable[id].work[MAIN_TEMP]++ == NEXT_MSG_WAIT){
			TaskTable[id].work[MAIN_TEMP] = 0;
			MsgWinObjMake(niji_kekka_msg, cres->win_act);
			x = GetMsgWinCenterPos(niji_kekka_msg);
			MsgWinObjSlidInSet(x, MSGWINOBJ_DEFAULT_Y, 0xffff, MSGWINOBJ_INOUT_SP);
		}
	}
	else if(cres->win_status == WO_TAIKI_IN)
		TaskTable[id].TaskAdrs = NijiSinsaGaugeInit;
}

//----------------------------------------------------------
//	�Q���R���̃Q�[�W�v�Z
//----------------------------------------------------------
enum{
	NSG_0 = 0,
	NSG_1,
	NSG_2,
};
static void NijiSinsaGaugeInit(u8 id)
{
	switch(TaskTable[id].work[MAIN_SEQ]){
		case NSG_0:
			if(cres->gauge_fade == 0){
				StarHeartFadeEndCheck(1, TaskTable[id].work[MAIN_WORK0]++);
				if(cres->meter_eff == 0)
					TaskTable[id].work[MAIN_SEQ] = NSG_2;
				else
					TaskTable[id].work[MAIN_SEQ]++;
			}
			break;
		case NSG_1:
			if(cres->meter_eff == 0)
				TaskTable[id].work[MAIN_SEQ] = NSG_0;
			break;
		case NSG_2:
			MsgWinObjSlidOutSet(MSGWINOBJ_INOUT_SP);
			TaskTable[id].work[MAIN_SEQ] = 0;
			TaskTable[id].TaskAdrs = JyunniNumPut;
			break;
	}
}

//----------------------------------------------------------
//	�ŏI�I�ȃ����L���O��\��
//----------------------------------------------------------
enum{
	JNP_0 = 0,
	JNP_1,
	JNP_2,
	JNP_3,
	JNP_4,
	JNP_5,
	JNP_6,
};
static void JyunniNumPut(u8 id)
{
	u8 task;
	int i;
	u16 x;
	u8 msgbuf[100];
	
	switch(TaskTable[id].work[MAIN_SEQ]){
		case JNP_0:
			if(cres->win_status == WO_TAIKI_OUT){
				//GetContestJyunni();
				TaskTable[id].work[MAIN_SEQ]++;
			}
			break;
		case JNP_1:	//������Ƴ���
			if(TaskTable[id].work[MAIN_TEMP]++ == 30){
				TaskTable[id].work[MAIN_TEMP] = 0;
				TaskTable[id].work[MAIN_SEQ]++;
			}
			break;
		case JNP_2:	//���ʂ�\�����Ă���
			for(i = 0; i < 4; i++){
				task = AddTask(JyunniScrnWriteSeq, 10);
				TaskTable[task].work[0] = ContestJyunni[i];
				TaskTable[task].work[1] = i;
			}
			TaskTable[id].work[MAIN_SEQ]++;
			break;
		case JNP_3:	//�D���҂̳���޳����گı��
			if(cres->jyunni_put == BREEDER_MAX){
				if(TaskTable[id].work[MAIN_TEMP]++ == 30){
					TaskTable[id].work[MAIN_TEMP] = 0;
					AddTask(ChampWinPaletteAnime, 10);
					TaskTable[id].work[MAIN_SEQ]++;

					//-- �D���|�P�����̃A�C�R����BG�A�j�� --//
					for(i = 0; i < BREEDER_MAX; i++)
					{
						if(ContestJyunni[i] == 0)
							break;
					}
					PokeIconBGAnmSet(i, 14);
				}
			}
			break;
		case JNP_4:
			if(TaskTable[id].work[MAIN_TEMP]++ == NEXT_MSG_WAIT){
				TaskTable[id].work[MAIN_TEMP] = 0;
				for(i = 0; i < BREEDER_MAX; i++)
					if(ContestJyunni[i] == 0)
						break;
				PM_strcpy(StrTempBuffer0, BreederParam[i].name);
				PM_strcpy(StrTempBuffer1, BreederParam[i].nickname);
				PM_MsgExpand(msgbuf, yuusyou_msg);
				MsgWinObjMake(msgbuf, cres->win_act);
				x = GetMsgWinCenterPos(msgbuf);
				MsgWinObjSlidInSet(x, MSGWINOBJ_DEFAULT_Y, 0xffff, MSGWINOBJ_INOUT_SP);
				TaskTable[id].work[MAIN_SEQ]++;
			}
			break;
		case JNP_5:
			//if(cres->win_status == WO_TAIKI_IN){
				TaskTable[id].work[MAIN_SEQ] = 0;
				TaskTable[id].TaskAdrs = ChampAppearEffect;
			//}
			break;
	}
}

//----------------------------------------------------------
//	�D���|�P�����o��G�t�F�N�g
//----------------------------------------------------------
#define WIN_OPEN_SP		2
#define WIN_OPEN_ADDWAIT	0
#define WIN_OPEN_V		32	//48
#define CRES_POKE_OUT_WAIT	120
static void ChampAppearEffect(u8 id)
{
	int i;
	u16 monsno;
	u8 actno;
	u32 personal_rnd, id_no;
	u8 open;
	u8 close;
	PalData *PD;
	
	switch(TaskTable[id].work[MAIN_SEQ]){
		case 0:
			Win0H = (0<<8) | 240;
			Win0V = (80<<8) | 80;

			for(i = 0; i < BREEDER_MAX; i++)
			{
				if(ContestJyunni[i] == 0)
					break;
			}
			monsno = BreederParam[i].monsno;
			personal_rnd = BreederParam[i].personal_rnd;
			id_no = BreederParam[i].id_no;
			
			//-- �|�P�������Z�b�g --//
			DecordPokeCellMakeBuchi(&PM2FObjDataCell[monsno], pm2fpara[monsno].size,
				pm2fpara[monsno].patcnt, &UserWork[0],
				(void*)DecordAdrs[1], monsno, personal_rnd);
			
			//DecordPalSet((PalData*)&PM2NObjDataPal[monsno]);
			PD=(PalData*)PokePalDataGetPara(monsno,id_no,personal_rnd);
			DecordPalSet(PD);

			PokeActHeaderMake(monsno, 1);
			PokeActHeader.palNo = PD->reg_num;
//			actno = AddActor(&PokeActHeader, 240+32, 80+pm2bpara[monsno].patcnt, 10);
			actno = AddActor(&PokeActHeader, 240+32, 80, 10);
			ActWork[actno].work[1] = monsno;
			ActWork[actno].oamData.Priority = 0;
			ActWork[actno].move = PokeSlidInAppear;
			cres->poke_act = actno;
			
			//-- ���ӂԂ����Z�b�g --//
			DecordCellSet((void*)&KamifubukiDataCell);
			DecordPalSet((void*)&KamifubukiDataPal);
			AddTask(KamifubukiTask, 10);
			
			TaskTable[id].work[MAIN_SEQ]++;
			break;
		case 1:		//Window��傫���J���Ă���
			if(TaskTable[id].work[MAIN_WORK1]++ == WIN_OPEN_ADDWAIT){
				TaskTable[id].work[MAIN_WORK1] = 0;
				TaskTable[id].work[MAIN_WORK0] += WIN_OPEN_SP;
				if(TaskTable[id].work[MAIN_WORK0] > WIN_OPEN_V)
					TaskTable[id].work[MAIN_WORK0] = WIN_OPEN_V;
				open = TaskTable[id].work[MAIN_WORK0];
				Win0V = ((80-open)<<8) | (80+open);

				if(open == WIN_OPEN_V)
					TaskTable[id].work[MAIN_SEQ]++;
			}
			break;
		case 2:
			if(cres->poke_appear == 1)
				TaskTable[id].work[MAIN_SEQ]++;
			break;
		case 3:		//�������Ă����߹�݂�ײ�ޱ�Ă�����
			if(TaskTable[id].work[MAIN_TEMP]++ == CRES_POKE_OUT_WAIT){
				TaskTable[id].work[MAIN_TEMP] = 0;
				ActWork[cres->poke_act].move = PokeSlideOut;
				TaskTable[id].work[MAIN_SEQ]++;
			}
			break;
		case 4:
			if(cres->poke_appear != 2)
				break;
				
			close = Win0V >> 8;
			close += 2;
			if(close > 80)
				close = 80;
			Win0V = (close << 8) | (80+80-close);
			if(close == 80)
				TaskTable[id].work[MAIN_SEQ]++;
			break;
		case 5:
			if(cres->poke_appear == 2){
				//���ӂԂ����~�߂�(�ʐM�p��ү���ނƈꏏ�ɏo�Ȃ��悤�ɂ��邽��)
				cres->kamifubuki_end = 1;
/*
				//-- �D���|�P�����̃A�C�R����BG�A�j�� --//
				for(i = 0; i < BREEDER_MAX; i++)
				{
					if(ContestJyunni[i] == 0)
						break;
				}
				PokeIconBGAnmSet(i, 14);
*/
				TaskTable[id].work[MAIN_SEQ] = 0;
				TaskTable[id].TaskAdrs = EndKeyWait;
			}
			break;
	}
}

//----------------------------------------------------------
//	���ʉ�ʏI���̃L�[�҂�
//----------------------------------------------------------
static void EndKeyWait(u8 id)
{
	int i;
	
	if(sys.Trg & A_BUTTON){
		//�}�ӂɌ����t���O�Z�b�g
		if((ContestTuusin & C_TUUSIN_FLG) == 0){
			for(i = 0; i < BREEDER_MAX; i++){
				ZukanCheck(PokeMonsNo2ZukanNoGet(BreederParam[i].monsno),Z_SEE_SET);
			}
		}

		TaskTable[id].TaskAdrs = TuusinCheckEnd;
	}
}

//----------------------------------------------------------
//	�ʐM�����ׂăt�B�[���h���A�����֐i��
//----------------------------------------------------------
static void TuusinCheckEnd(u8 id)
{
	if(ContestTuusin & C_TUUSIN_FLG)
	{
		SioWinMsgPut(tuusin_taiki_msg);
		SetSioEndDataStart();
		TaskTable[id].TaskAdrs = ResultTuusinEndWait;
		//ContestTuusin = 0;	//�ʐM�׸�OFF
		//PokemonMultiSioEnd();		�ŏ��̒ʐM���I���������_�̏��Ɉړ�
	}
	else
		TaskTable[id].TaskAdrs = ContestEndSeq;
}

//----------------------------------------------------------
//	�ʐM �I���҂�
//----------------------------------------------------------
static void ResultTuusinEndWait(u8 id)
{
	if(pokemon_sio_flag == 0){
		ContestTuusin = 0;	//�ʐM�׸�OFF
		SioWinMsgElase();
		TaskTable[id].TaskAdrs = ContestEndSeq;
	}
}

//----------------------------------------------------------
//	�R���e�X�g�I���@̨���ނɕ��A
//----------------------------------------------------------
static void ContestEndSeq(u8 id)
{
//	u8 cont_type;
	
	//TVConDataSet(ContestJyunni[MyBreederNo]);	//TV�ް��o�^
	TVConZyuniSet(ContestJyunni[MyBreederNo]);	//TV�ް��o�^
	FanPointCalc(FAN_CONTEST_TYPE);			//̧��ް��o�^
	PortraitDataSet(ContestRank);			//����G�ް��o�^
	//��UserWork�ɔ��p���ް��`���B�������ް��Ȃ̂Ŏ������D�����ĂȂ��Ă��`�����Ă�
	PortraitDataSet(0xfe);
	UserWork[TEMP_PORTRAIT_MSGFLG] = 1;	//����ł͓����ް����Ȃ��̂�0
	UserWork[TEMP_PORTRAIT_PLACE] = PortraitDataSpaceSearch(0xfe, 0);
	
//	PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
	HardFadeReq(0xff, 0, 0, 16, 0);	//����ߵݳ���޳����گı�҂��Ă�̂�
	TaskTable[id].TaskAdrs = ContestEndFadeWait;
}

static void ContestEndFadeWait(u8 id)
{
	if(FadeData.fade_sw == 0)
	{
		if(TaskTable[id].work[MAIN_TEMP] == 0){
			DelTask(cres->champ_winpal_task);
			ColorConceChange(0x0000ffff, 16, 0);
			TaskTable[id].work[MAIN_TEMP]++;
		}
		else if(TaskTable[id].work[MAIN_TEMP] == 1){
			ColorConceChange(0xffff0000, 16, 0);
			TaskTable[id].work[MAIN_TEMP]++;
		}
		else{	//��گĂ���Ăō����Ȃ��Ă���ʰ�ނ�̪��ނ�ر����
			*(vu16 *)REG_BLDCNT = 0;
			*(vu16 *)REG_BLDY = 0;
	//		sys.pMainProc = ClientPokeListReturn;
			DelTask(id);
			MainProcChange(FieldScriptRecover);
	//		ResetForceEvent();
	//		ContinueFieldScript();
		}
	}
}










//=========================================================================
//
//		���C���̗���ȊO�̃V�[�P���X
//												by matsuda 2002.06.25(��)
//=========================================================================
//----------------------------------------------------------
//	�w�i�ʃX�N���[���Ǘ��^�X�N
//----------------------------------------------------------
static void HaikeiScrollTask(u8 id)
{
	ScrX3 += 2;
	ScrY3 += 1;
	if(ScrX3 > 255)
		ScrX3 -= 255;
	if(ScrY3 > 255)
		ScrY3 -= 255;
}

//----------------------------------------------------------
//	����ʰĂ̓_�ŊǗ��^�X�N
//----------------------------------------------------------
enum{
	SH_WAIT = 0,
	SH_EVY,
	SH_MUKI,
};
#define SH_FADE_WAIT	1
#define HEART_FADE_PALNO		(PA_BG6+PA_COL8)	//̪��ނ�����ʰĂ���گ�No
#define HEARTBL_FADE_PALNO		(PA_BG6+PA_COL14)	//̪��ނ�����ʰĂ���گ�No
#define STAR_FADE_PALNO			(PA_BG6+PA_COL11)	//̪��ނ����鐯����گ�No
static void StarHeartFadeTask(u8 id)
{
	if(TaskTable[id].work[SH_WAIT]++ == SH_FADE_WAIT){
		TaskTable[id].work[SH_WAIT] = 0;
		if(TaskTable[id].work[SH_MUKI] == 0)
			TaskTable[id].work[SH_EVY]++;
		else
			TaskTable[id].work[SH_EVY]--;
		if(TaskTable[id].work[SH_EVY] == 16)
			TaskTable[id].work[SH_MUKI] = 1;
		else if(TaskTable[id].work[SH_EVY] == 0)
			TaskTable[id].work[SH_MUKI] = 0;
		SoftFade(STAR_FADE_PALNO, 1, TaskTable[id].work[SH_EVY], 0x2ede);//0x02dc);
		SoftFade(HEART_FADE_PALNO, 1, TaskTable[id].work[SH_EVY], 0x7fff);//0x5bff);
		SoftFade(HEARTBL_FADE_PALNO, 1, TaskTable[id].work[SH_EVY], 0x77be);
	}
	
	if(TaskTable[id].work[SH_EVY] == 0)
		cres->gauge_fade = 0;
	else
		cres->gauge_fade = 1;
}



//=========================================================================
//
//		�c�[����
//												by matsuda 2002.06.24(��)
//=========================================================================

//=========================================================================
//	�|�P�����A�C�R����BGVRAM�ɓ]�����A��ذ��ް����Z�b�g
//
// posno = ���Ԗڂɔz�u���邩	anmno = 0:1����ݖڂ̱�ҁA1:2����ݖڂ̱��
// scrn_sw = 0:��ذ��ް��̏������������Ȃ�	1:��ذ��ް��̏�������������
// rnd = ������
//----------------------------------------------------------
static void PokeIconBgSet(u16 monsno, u8 posno, u8 anmno, u8 scrn_sw, u32 rnd)
{
	u16 scrn;
	int x, y;
	u16 scrn_offs;
	
	if(scrn_sw){
//		DIV_DMACOPY(3, 
//			IconCharAdrs[monsno]+anmno*OBJ_SIZE16_32x32+ 0x20*4, //0x20*4=���s�ڂ̋�
//			CRES_FONT_VRAM+OBJ_SIZE16_32x32*posno, OBJ_SIZE16_32x32-0x20*4, 32);
		DIV_DMACOPY(3, 
			GetPokeIconCharAdrs(monsno,rnd)+anmno*OBJ_SIZE16_32x32+ 0x20*4,
			CRES_FONT_VRAM+OBJ_SIZE16_32x32*posno, OBJ_SIZE16_32x32-0x20*4, 32);

		scrn = (CRES_POKEICON_BGPALNO + posno) << 12;	//color
		scrn = (OBJ16_32x32 * posno + CRES_POKEICON_TOPCHRNO) | scrn;
		scrn_offs = 0x20*4+3 + posno*0x20*3;
		for(y = 0; y < (4-1); y++){	//-1 = ��1�s�͋󔒂Ȃ̂Ō�����]�����ĂȂ�
			for(x = 0; x < 4; x++){
				*(vu16*)(CRES_FONT_SCRN + (0x20*y + x + scrn_offs)*2) = scrn;
				scrn++;
			}
		}
	}
	else{	//��ذ��ް��̏����������s�Ȃ�Ȃ����ͱ�҂Ȃ̂�ظ��Ăœ]������
//		DmaTransReq((void*)IconCharAdrs[monsno]+anmno*OBJ_SIZE16_32x32+ 0x20*4,
//			CRES_FONT_VRAM+OBJ_SIZE16_32x32*posno, OBJ_SIZE16_32x32-0x20*4);
		DmaTransReq((void*)GetPokeIconCharAdrs(monsno,rnd) + anmno*OBJ_SIZE16_32x32+ 0x20*4,
			CRES_FONT_VRAM+OBJ_SIZE16_32x32*posno, OBJ_SIZE16_32x32-0x20*4);
	}
}

static void PokeIconBgAllSet(u8 anmno, u8 scrn_sw)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++)
		PokeIconBgSet(BreederParam[i].monsno, i, anmno, scrn_sw, BreederParam[i].personal_rnd);
}

static void PokeIconBgPaletteSet(void)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++){
//		PaletteWorkSet(&p_icon_Palette[IconPalAtr[BreederParam[i].monsno] * 16],
//			(CRES_POKEICON_BGPALNO+i)*16, 0x20);
		PaletteWorkSet(
			&p_icon_Palette[IconPalAtr[GetPokeIconCharID(BreederParam[i].monsno, 0x00)] * 16],
			(CRES_POKEICON_BGPALNO+i)*16, 0x20);
	}
}

//=========================================================================
//	���b�Z�[�W�E�B���h�E
//=========================================================================
//----------------------------------------------------------
//	���b�Z�[�W�E�B���h�E�����b�Z�[�WOBJ�쐬
//----------------------------------------------------------
static const u8 mwo_init_msg[] = {
	CTRL_,C_FCHG_,FONT8x16sTYPE,CTRL_,C_ACOL_,0xf,0x1,0xe,EOM_};

static void MsgWinObjMake(const u8 *msg, u8 actno)
{
	actWork *xreg;
	u16 cellNo[4];
//	u8 msgbuf[100];
	u8 msglen;
	u8 *buff;
	u8 *adrs;
	int i;
	
	xreg = &ActWork[actno];
	cellNo[0] = xreg->oamData.CharNo;
	cellNo[1] = ActWork[xreg->work[0]].oamData.CharNo;
	cellNo[2] = ActWork[xreg->work[1]].oamData.CharNo;
	cellNo[3] = ActWork[xreg->work[2]].oamData.CharNo;
	
	//-- �S�ر --//
	for(i = 0; i < 4; i++)
		DIV_DMACLEAR(3, 0, OBJ_MODE0_VRAM + cellNo[i]*32, OBJ_SIZE16_32x64, 32);
	
	//-- ���b�Z�[�W�f�[�^�W�J --//
	buff = PM_strcpy(FightMsgBuf, mwo_init_msg);
	PM_strcpy(buff, msg);
	MsgCGXDataSet((void*)CRES_MSG_DECORD_ADR, FightMsgBuf);
	
	msglen = PM_strlen(msg);
	adrs = (void*)(OBJ_MODE0_VRAM + cellNo[0]*32);
	CpuCopy(&kekka_w_Character[0x20*0], adrs, 0x20, 32);	//�g:��
	CpuCopy(&kekka_w_Character[0x20*4], &adrs[32*8*1], 0x20, 32);
	CpuCopy(&kekka_w_Character[0x20*4], &adrs[32*8*2], 0x20, 32);
	CpuCopy(&kekka_w_Character[0x20*2], &adrs[32*8*3], 0x20, 32);
	for(i = 0; i <= msglen; i++){
		if(i < 7)
			adrs = (void*)(OBJ_MODE0_VRAM + cellNo[0]*32 + 0x20*(i+1));
		else if(i < 7+8)
			adrs = (void*)(OBJ_MODE0_VRAM + cellNo[1]*32 + 0x20*(i-7));
		else if(i < 7+8+8)
			adrs = (void*)(OBJ_MODE0_VRAM + cellNo[2]*32 + 0x20*(i-(7+8)));
		else
			adrs = (void*)(OBJ_MODE0_VRAM + cellNo[3]*32 + 0x20*(i-(7+8+8)));
		
		if(i == msglen)
			break;
		
		CpuCopy(&kekka_w_Character[0x20*6], adrs, 0x20, 32);	//�g:��
		CpuCopy(&kekka_w_Character[0x20*7], &adrs[32*8*3], 0x20, 32);	//�g:��
		CpuCopy(CRES_MSG_DECORD_ADR + 0x40*i, &adrs[32*8*1], 0x20, 32);	//ү���ޏ�i
		CpuCopy(CRES_MSG_DECORD_ADR + 0x40*i+0x20, &adrs[32*8*2], 0x20, 32);//ү���މ��i
	}
	CpuCopy(&kekka_w_Character[0x20*1], adrs, 0x20, 32);	//�g:��
	CpuCopy(&kekka_w_Character[0x20*5], &adrs[32*8*1], 0x20, 32);	//�g:��
	CpuCopy(&kekka_w_Character[0x20*5], &adrs[32*8*2], 0x20, 32);	//�g:��
	CpuCopy(&kekka_w_Character[0x20*3], &adrs[32*8*3], 0x20, 32);	//�g:��
}

//----------------------------------------------------------
//	���b�Z�[�W�E�B���h�E��Cell,Pal,Actor�̃Z�b�g
//----------------------------------------------------------
static void MsgWinObjSet(void)
{
	int i;
	actHeader msgHeader;
	u8 actno[8];
	
	memcpy(&msgHeader, &MsgWinObjHeader, sizeof(msgHeader));
	for(i = 0; i < 8; i++)	//��ʂ̒[����[�܂Ŗ��܂镪 �̈�m��
		CellSet(&MsgWinDataCell[i]);
	ObjPalSet(&MsgWinDataPal);
	
	for(i = 0; i < 8; i++){
		actno[i] = AddActor(&msgHeader, MSGWINOBJ_DEFAULT_X, MSGWINOBJ_DEFAULT_Y, 10);
		msgHeader.cellNo++;
	}
	
	//�q��ActNo��e�Ɏ�������
	ActWork[actno[0]].work[0] = actno[1];
	ActWork[actno[0]].work[1] = actno[2];
	ActWork[actno[0]].work[2] = actno[3];
	
	//�ʐMү���ޗp�̳���޳
	ActWork[actno[4]].work[0] = actno[5];
	ActWork[actno[4]].work[1] = actno[6];
	ActWork[actno[4]].work[2] = actno[7];
	
	cres->win_act = actno[0];
	cres->win_status = WO_TAIKI_OUT;
	
	cres->siowin_act = actno[4];
	SioWinMsgElase();
}

//----------------------------------------------------------
//	���b�Z�[�W�̒�������ײ�ތ�A���S�Ɉʒu���邽�߂�X���W���擾����
//----------------------------------------------------------
static u16 GetMsgWinCenterPos(const u8 *msg)
{
	u8 len;
	u16 x;
	
	len = PM_strlen(msg);
	x = 120 - 4*len;
	x -= 8;		//�g�̕�
	return x;
}

/*-----------
//----------------------------------------------------------
//	�q�̓���֐�	�e�̍��W��ǔ�����
//----------------------------------------------------------
static void MsgWinObjTailing(actWork *xreg)
{
	xreg->x = ActWork[xreg->work[0]].x + ActWork[xreg->work[0]].dx + 64;
	xreg->y = ActWork[xreg->work[0]].y + ActWork[xreg->work[0]].dy;
}
------------*/

enum{
	MWO_X = 4,
	MWO_WAIT,
	MWO_SP,
	MWO_SPWORK,
};
//----------------------------------------------------------
//	Msg�E�B���h�E���w����W�܂Žײ�޲݂�����
//	wait = �w����W�܂ōs�������莞�ԑ҂��Ď����Žײ�ޱ�Ă���
//		   (0xffff���Ǝ����ײ�ޱ�Ă͂��Ȃ��� �����Ǝw����W�őҋ@)
//	sp = �ړ����x(����8�ޯď���)
//----------------------------------------------------------
static void MsgWinObjSlidInSet(s16 x, s16 y, u16 wait, u16 sp)
{
	actWork *xreg;
	
	xreg = &ActWork[cres->win_act];
	xreg->x = MSGWINOBJ_DEFAULT_X;
	xreg->y = y;
	xreg->dx = 0;
	xreg->dy = 0;
	xreg->work[MWO_X] = x + 32;
	xreg->work[MWO_WAIT] = wait;
	xreg->work[MWO_SP] = sp;
	xreg->work[MWO_SPWORK] = 0;
	xreg->move = MsgWinObjSlidInSeq;

	cres->win_status = WO_SLIDEIN;
}

//----------------------------------------------------------
//	Msg�E�B���h�E����ʊO�܂Žײ�ޱ�Ă�����
//	sp = �ړ����x(����8�ޯď���)
//----------------------------------------------------------
static void MsgWinObjSlidOutSet(u16 sp)
{
	actWork *xreg;
	
	xreg = &ActWork[cres->win_act];
	xreg->x += xreg->dx;
	xreg->y += xreg->dy;
	xreg->dx = xreg->dy = 0;

	xreg->work[MWO_SP] = sp;
	xreg->work[MWO_SPWORK] = 0;
	xreg->move = MsgWinObjSlideOutSeq;

	cres->win_status = WO_SLIDEOUT;
}

//----------------------------------------------------------
//	Msg����޳�̓��켰�ݽ����а�ɂ��āA�\�����W�������ʒu�ɖ߂�
//----------------------------------------------------------
static void MsgWinObjDefaultSet(actWork *xreg)
{
	xreg->x = MSGWINOBJ_DEFAULT_X;
	xreg->y = MSGWINOBJ_DEFAULT_Y;
	xreg->dx = xreg->dy = 0;
	xreg->move = DummyActMove;

	cres->win_status = WO_TAIKI_OUT;
}

//----------------------------------------------------------
//	ү���޳���޳���켰�ݽ
//----------------------------------------------------------
//-- �X���C�h�C�� --//
static void MsgWinObjSlidInSeq(actWork *xreg)
{
	int i;
	
	xreg->work[MWO_SPWORK] += xreg->work[MWO_SP];
	xreg->x -= xreg->work[MWO_SPWORK] >> 8;
	xreg->work[MWO_SPWORK] &= 0x00ff;	//�����c���ĸر
	if(xreg->x < xreg->work[MWO_X])
		xreg->x = xreg->work[MWO_X];
	for(i = 0; i < 3; i++)
		ActWork[xreg->work[i]].x = xreg->x + xreg->dx + 64*(i+1);
	
	if(xreg->x == xreg->work[MWO_X])
		xreg->move = MsgWinObjWaitSeq;
}

//-- �E�F�C�g --//
static void MsgWinObjWaitSeq(actWork *xreg)
{
	cres->win_status = WO_TAIKI_IN;

	if((u16)xreg->work[MWO_WAIT] == 0xffff)
		return;
	
	if(xreg->work[MWO_WAIT]-- == 0)
		MsgWinObjSlidOutSet(xreg->work[MWO_SP]);
}

//-- �X���C�h�A�E�g --//
static void MsgWinObjSlideOutSeq(actWork *xreg)
{
	int i;
	
	xreg->work[MWO_SPWORK] += xreg->work[MWO_SP];
	xreg->x -= xreg->work[MWO_SPWORK] >> 8;
	xreg->work[MWO_SPWORK] &= 0x00ff;
	for(i = 0; i < 3; i++)
		ActWork[xreg->work[i]].x = xreg->x + xreg->dx + 64*(i+1);
	
	if(xreg->x + xreg->dx < -32+(-64*3))
		MsgWinObjDefaultSet(xreg);
}

//----------------------------------------------------------
//	�ʐMү���޳���޳�\��
//----------------------------------------------------------
static void SioWinMsgPut(const u8 *msg)
{
	int i;
	actWork *xreg;
	u16 x;
	
	MsgWinObjMake(msg, cres->siowin_act);
	x = GetMsgWinCenterPos(msg);
	
	xreg = &ActWork[cres->siowin_act];
	xreg->x = x + 32;
	xreg->y = MSGWINOBJ_SIOWAIT_Y;
	xreg->banish = 0;
	for(i = 0; i < 3; i++){
		ActWork[xreg->work[i]].x = xreg->x + xreg->dx + 64*(i+1);
		ActWork[xreg->work[i]].y = xreg->y;
		ActWork[xreg->work[i]].banish = 0;
	}
	
	Win0H = (0 << 8) | 240;
	Win0V = ((xreg->y - 16) << 8) | (xreg->y + 16);
	*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | (WIN_ALL_ON^WIN_BG0_ON);
}

//-- ���� --//
static void SioWinMsgElase(void)
{
	int i;
	actWork *xreg;
	
	xreg = &ActWork[cres->siowin_act];
	xreg->banish = 1;
	for(i = 0; i < 3; i++)
		ActWork[xreg->work[i]].banish = 1;
	Win0H = 0;
	Win0V = 0;
	*(vu16 *)REG_WIN0H = Win0H;
	*(vu16 *)REG_WIN0V = Win0V;
	*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
}

//=========================================================================
//	��ʏ㕔�̕\��̃X�N���[��������
//=========================================================================
#define RANK_LEN	8
#define TYPE_LEN	5
#define CON_LEN		5
enum{
	STYLE_COLOR = 0,
	BEAUTIFUL_COLOR,
	CUTE_COLOR,
	CLEVER_COLOR,
	STRONG_COLOR,
};
static void HyoudaiWriteScrn(void)
{
	u16 x,y;
	s16 x_offs = 0;
	u16 color;
	int i;
	
	//-- �����N --//
	x = 5*2;
	y = 0x20*1*2;
	if(ContestTuusin & C_TUUSIN_FLG){
		x_offs++;
		CpuCopy(CresSio_Map, CRES_WIN_SCRN+x+y, RANK_LEN*2, 16);	//��i �����i
		CpuCopy(&CresSio_Map[RANK_LEN], CRES_WIN_SCRN+x+y+0x40, RANK_LEN*2, 16);
	}
	else if(ContestRank == NORMAL_RANK){
		CpuCopy(CresNormal_Map, CRES_WIN_SCRN+x+y, RANK_LEN*2, 16);
		CpuCopy(&CresNormal_Map[RANK_LEN], CRES_WIN_SCRN+x+y+0x40, RANK_LEN*2, 16);
	}
	else if(ContestRank == SUPER_RANK){
		CpuCopy(CresSuper_Map, CRES_WIN_SCRN+x+y, RANK_LEN*2, 16);
		CpuCopy(&CresSuper_Map[RANK_LEN], CRES_WIN_SCRN+x+y+0x40, RANK_LEN*2, 16);
	}
	else if(ContestRank == HYPER_RANK){
		CpuCopy(CresHyper_Map, CRES_WIN_SCRN+x+y, RANK_LEN*2, 16);
		CpuCopy(&CresHyper_Map[RANK_LEN], CRES_WIN_SCRN+x+y+0x40, RANK_LEN*2, 16);
	}
	else{// if(ContestRank == MASTER_RANK){
		CpuCopy(CresMaster_Map, CRES_WIN_SCRN+x+y, RANK_LEN*2, 16);
		CpuCopy(&CresMaster_Map[RANK_LEN], CRES_WIN_SCRN+x+y+0x40, RANK_LEN*2, 16);
	}
	
	//-- �R���e�X�g�^�C�v(�������悳��) --//
	x = 104/8*2;
	x += x_offs*2;
	x_offs = 0;
	if(ContestKind == STYLE_CONTEST){
		color = STYLE_COLOR;
		CpuCopy(CresStyle_Map, CRES_WIN_SCRN+x+y, TYPE_LEN*2, 16);
		CpuCopy(&CresStyle_Map[TYPE_LEN], CRES_WIN_SCRN+x+y+0x40, TYPE_LEN*2, 16);
	}
	else if(ContestKind == BEAUTIFUL_CONTEST){
		color = BEAUTIFUL_COLOR;
		CpuCopy(CresBeautiful_Map, CRES_WIN_SCRN+x+y, TYPE_LEN*2, 16);
		CpuCopy(&CresBeautiful_Map[TYPE_LEN], CRES_WIN_SCRN+x+y+0x40, TYPE_LEN*2, 16);
	}
	else if(ContestKind == CUTE_CONTEST){
		color = CUTE_COLOR;
		x_offs--;
		CpuCopy(CresCute_Map, CRES_WIN_SCRN+x+y, TYPE_LEN*2, 16);
		CpuCopy(&CresCute_Map[TYPE_LEN], CRES_WIN_SCRN+x+y+0x40, TYPE_LEN*2, 16);
	}
	else if(ContestKind == CLEVER_CONTEST){
		color = CLEVER_COLOR;
		x_offs--;
		CpuCopy(CresClever_Map, CRES_WIN_SCRN+x+y, TYPE_LEN*2, 16);
		CpuCopy(&CresClever_Map[TYPE_LEN], CRES_WIN_SCRN+x+y+0x40, TYPE_LEN*2, 16);
	}
	else{// if(ContestKind == STRONG_CONTEST){
		color = STRONG_COLOR;
		CpuCopy(CresStrong_Map, CRES_WIN_SCRN+x+y, TYPE_LEN*2, 16);
		CpuCopy(&CresStrong_Map[TYPE_LEN], CRES_WIN_SCRN+x+y+0x40, TYPE_LEN*2, 16);
	}
	
	//-- �R���e�X�g --//
	x = 152/8*2;
	x += x_offs*2;
	x_offs = 0;
	CpuCopy(CresContest_Map, CRES_WIN_SCRN+x+y, CON_LEN*2, 16);
	CpuCopy(&CresContest_Map[CON_LEN], CRES_WIN_SCRN+x+y+0x40, CON_LEN*2, 16);
	
	//-- �J���[�A�g���r���[�g�ύX --//
	for(i = 0; i < 0x20*4; i++){
		*(vu16*)(CRES_WIN_SCRN + i*2) &= 0x0fff;
		*(vu16*)(CRES_WIN_SCRN + i*2) |= color << 12;
	}
}


//=========================================================================
//	�Q�[�W�v�Z
//=========================================================================
#define ONE_BP	63	//76	//��1�ɑ΂���BP�l
#define ONE_AP	80//40	//ʰ�1�ɑ΂���AP�l
#define CRES_GAUGE_MAX	88	//�ް�ނ̍ő�h�b�g��
#define ONE_PERSENT_DOTTO	90	// 90=0.9*100	1%�ňړ�����h�b�g��

//----------------------------------------------------------
//	��(�ꎟ�R��)�̐����擾
//	over_check = 1:�ő启���𒴂��Ă���ő吔�ŕԂ�
//				 0:�ő吔�𒴂��ĂĂ����̂܂܂̐��l�ŕԂ�
//----------------------------------------------------------
static u8 GetStarNum(u8 brd, u8 over_check)
{
	u32 bp, num;
	
	bp = ContBP[brd] << 16;
	num = bp / ONE_BP;
	if((num & 0x0000ffff) > 0)	//�����؂�グ
		num += 0x00010000;
//	if((num & 0x0000ffff) > 0x00008000)
//		num += 0x00010000;		//���� �l�̌ܓ�
	num >>= 16;
	if(num == 0 && bp != 0)
		num = 1;
	if(over_check && num > 10)
		num = 10;
	return num;
}

//----------------------------------------------------------
//	ʰ�(�񎟐R��)�̐����擾
// ����ʰĂŕ\������ꍇ�̓}�C�i�X�ŕԂ�
//	over_check = 1:�ő启���𒴂��Ă���ő吔�ŕԂ�
//				 0:�ő吔�𒴂��ĂĂ����̂܂܂̐��l�ŕԂ�
//----------------------------------------------------------
static s8 GetHeartNum(u8 brd, u8 over_check)
{
	u32 eval, num;
	s8 ret;
	
//	eval = abs(ContEvaluation[brd]) << 16;
	eval = abs(NijiPoint[brd]) << 16;
	num = eval / ONE_AP;
	if((num & 0x0000ffff) > 0)	//�����؂�グ
		num += 0x00010000;
//	if((num & 0x0000ffff) > 0x00008000)
//		num += 0x00010000;		//���� �l�̌ܓ�
	num >>= 16;
	if(num == 0 && eval != 0)
		num = 1;
	if(over_check && num > 10)
		num = 10;
	//if(ContEvaluation[brd] < 0)
	if(NijiPoint[brd] < 0)
		ret = -num;
	else
		ret = num;
	return ret;
}


//=========================================================================
//	�e��ذ�ް�̏��ʂ�ذ��ް��ɏ���
//=========================================================================
#define JYUNNI_PUTWAIT		40
#define JYUNNI_FAST_SCRN	0x5043
static void JyunniScrnWriteSeq(u8 id)
{
	u16 scrn;
	
	if(TaskTable[id].work[10] == 0){
		TaskTable[id].work[11] = (BREEDER_MAX-1-TaskTable[id].work[0]) * JYUNNI_PUTWAIT;
		TaskTable[id].work[10]++;
	}
	else if(TaskTable[id].work[10] == 1){
		if(TaskTable[id].work[11]-- == 0){
			scrn = JYUNNI_FAST_SCRN + TaskTable[id].work[0] * 2;
			*(vu16*)(CRES_WIN_SCRN + (TaskTable[id].work[1]*0x60 + 0x20*5+1)*2) = scrn;
			*(vu16*)(CRES_WIN_SCRN + (TaskTable[id].work[1]*0x60 + 0x20*5+1 + 1)*2) = scrn+1;
			*(vu16*)(CRES_WIN_SCRN + (TaskTable[id].work[1]*0x60 + 0x20*5+1 +0x20)*2) = scrn+0x10;
			*(vu16*)(CRES_WIN_SCRN + (TaskTable[id].work[1]*0x60 + 0x20*5+1 + 0x21)*2) = scrn+0x11;
			
			cres->jyunni_put++;
			DelTask(id);
			SePlay(SE_JYUNI);
		}
	}
}

//=========================================================================
//	�D�������u���[�_�[�̃E�B���h�E����گı�҂�����
//=========================================================================
static const u16 WinPalAnmData[] = {
	0x0000,0x7f74,0x6f8d,0x5fa6,0x4fe0,0x4fe0,0x33ea,0x1bf4,
	0x03ff,0x0abf,0x117f,0x1c3f,0x34fc,0x4dd9,0x6696,0x7f74,
};
#define CHAMP_PALANM_WAIT	0	//4
static void ChampWinPaletteAnime(u8 id)
{
	int champ;
	u16 scrn;
	int x, y;
	
	for(champ = 0; champ < BREEDER_MAX; champ++){
		if(ContestJyunni[champ] == 0)
			break;
	}
	
	for(y = 0; y < 3; y++){
		for(x = 0; x < 240/8; x++){
			scrn = *(vu16*)(CRES_WIN_SCRN + (champ*0x60 + 0x20*4 + x + 0x20*y)*2);
			scrn &= 0x0fff;
			scrn |= 0x9000;	//����߂̳���޳����گı�җp����گĂɕύX����
			*(vu16*)(CRES_WIN_SCRN + (champ*0x60 + 0x20*4 + x+ 0x20*y)*2) = scrn;
		}
	}
	TaskTable[id].work[10] = champ;
	TaskTable[id].work[12] = 1;
	TaskTable[id].TaskAdrs = ChampWinPaletteAnimeMain;
	cres->champ_winpal_task = id;
}

static void ChampWinPaletteAnimeMain(u8 id)
{
	if(TaskTable[id].work[11]++ == CHAMP_PALANM_WAIT){
		TaskTable[id].work[11] = 0;
		SoftFade(16*9+1, 1, TaskTable[id].work[12], 0x6f8d);
		if(TaskTable[id].work[13] == 0){
			if(++TaskTable[id].work[12] == 16)
				TaskTable[id].work[13] = 1;
		}
		else{
			if(--TaskTable[id].work[12] == 0)
				TaskTable[id].work[13] = 0;
		}
	}
}





//=========================================================================
//
//		�|�P�����̓���֐�
//												by matsuda 2002.06.26(��)
//=========================================================================
#define POKE_SLIDE_SP	0x0600
//-- �ײ�޲� --//
static void PokeSlidInAppear(actWork *xreg)
{
	if(xreg->work[0] < 10){
		xreg->work[0]++;
		if(xreg->work[0] == 10){
			VoicePlay(xreg->work[1],0);
			xreg->work[1] = 0;
		}
		return;
	}
	
	xreg->work[1] += POKE_SLIDE_SP;
	xreg->x -= xreg->work[1] >> 8;
	xreg->work[1] &= 0x00ff;
	if(xreg->x < 120)
		xreg->x = 120;
	if(xreg->x == 120){
		xreg->move = DummyActMove;
		xreg->work[1] = 0;
		cres->poke_appear = 1;
	}
}

//-- �ײ�ޱ�� --//
static void PokeSlideOut(actWork *xreg)
{
	xreg->work[1] += POKE_SLIDE_SP;
	xreg->x -= xreg->work[1] >> 8;
	xreg->work[1] &= 0x00ff;
	if(xreg->x < -32){
		xreg->move = DummyActMove;
		xreg->banish = 1;
		cres->poke_appear = 2;
	}
}


//=========================================================================
//	���ӂԂ�
//=========================================================================
//----------------------------------------------------------
//	���ӂԂ��A�N�^�[�Ǘ��^�X�N
//----------------------------------------------------------
#define KAMIFUBUKI_ADD_WAIT		4	//�V���Ɏ��ӂԂ���Add���Ă����Ԋu
#define KAMIFUBUKI_ADD_X		0x0030
#define KAMIFUBUKI_ADD_Y		1
static void KamifubukiTask(u8 id)
{
	u8 actno;
	
	if(TaskTable[id].work[0]++ == KAMIFUBUKI_ADD_WAIT){
		TaskTable[id].work[0] = 0;
		if(cres->kamifubuki_num < 40)	//���������ް���Ȃ��悤��Add���Ă鐔������
		{
			actno = AddActor(&CresKamifubukiHeader, 
				(pp_rand() % 240)-20, 80-WIN_OPEN_V-4, 5);
			ActWork[actno].work[0] = pp_rand() % 0x0200;
			ActWork[actno].work[1] = (pp_rand() % 24) + 16;
			ActWork[actno].work[2] = (pp_rand() % 0x0100) + KAMIFUBUKI_ADD_X;
			ActWork[actno].oamData.CharNo += pp_rand() % 17;
			cres->kamifubuki_num++;
		}
	}
	
	if(cres->kamifubuki_end)
		DelTask(id);
}

//----------------------------------------------------------
//	���ӂԂ������̓���֐�
//----------------------------------------------------------
static void CresKamifubukiSeq(actWork *xreg)
{
	xreg->work[3] += xreg->work[0];
	xreg->dx = SinMove(xreg->work[3] >> 8, xreg->work[1]);
	
	xreg->work[4] += xreg->work[2];
	xreg->x += xreg->work[4] >> 8;
	xreg->work[4] &= 0x00ff;
	xreg->y += KAMIFUBUKI_ADD_Y;
	
	if(cres->kamifubuki_end)
		xreg->banish = 1;
		
	if(xreg->x > 248 || xreg->y > 80+WIN_OPEN_V+4){
		DelActor(xreg);
		cres->kamifubuki_num--;
	}
}


//=========================================================================
//	�|�P�����A�C�R�����A�j��������
// anm_wait = �A�j�������鎞�̳���(���l���傫���قǂ������A�j������)
//=========================================================================
static void PokeIconBGAnmSet(u8 brd, u8 anm_wait)
{
	u8 id;
	
	id = AddTask(PokeIconBGAnmSeq, 8);
	TaskTable[id].work[0] = brd;
	TaskTable[id].work[1] = anm_wait;
	TaskTable[id].work[2] = BreederParam[brd].monsno;
}

//----------------------------------------------------------
//	�|�P�����A�C�R��BG�A�j������^�X�N
//----------------------------------------------------------
static void PokeIconBGAnmSeq(u8 id)
{
	u8 brd;
	
	brd = TaskTable[id].work[0];
	if(TaskTable[id].work[10]++ == TaskTable[id].work[1]){
		TaskTable[id].work[10] = 0;

		PokeIconBgSet(TaskTable[id].work[2], brd, 
			TaskTable[id].work[11], 0, BreederParam[brd].personal_rnd);
		TaskTable[id].work[11] ^= 1;
	}
}





//=========================================================================
//	���[�^�[�v�Z
//=========================================================================
static void ResultMeterCalc(void)
{
	s16 top_eval;
	u8 top_brd;
	s32 itiji, niji;
	u32 itiji_dotto, niji_dotto;
	int brd;
	s16 dotto1, dotto2;
	
	//-- ��{�ő�l�擾 --//
	top_eval = ContEvaluation[0];
	top_brd = 0;
	for(brd = 1; brd < BREEDER_MAX; brd++){
		if(top_eval < ContEvaluation[brd]){
			top_eval = ContEvaluation[brd];
			top_brd = brd;
		}
	}

	//-- �ő�]���l��ϲŽ�̎��͈�ԕ]���l���Ⴂ�̂��ő�Ƃ��čĎ擾���� --//
	if(top_eval < 0){
		top_eval = ContEvaluation[0];
		top_brd = 0;
		for(brd = 1; brd < BREEDER_MAX; brd++){
			if(top_eval > ContEvaluation[brd]){
				top_eval = ContEvaluation[brd];
				top_brd = brd;
			}
		}
	}

	for(brd = 0; brd < BREEDER_MAX; brd++){
		//-- �ꎟ�R���̃p�[�Z���e�[�W���擾 --//
		itiji = (ContBP[brd] * 1000) / abs(top_eval);
		if((itiji % 10) >= 5)	//�l�̌ܓ�
			itiji += 10;
		cgraph[brd].itiji = itiji / 10;
		
		//-- �񎟐R���̃p�[�Z���e�[�W���擾 --//
		niji = (abs(NijiPoint[brd]) * 1000) / abs(top_eval);
		if((niji % 10) >= 5)	//�l�̌ܓ�
			niji += 10;
		cgraph[brd].niji = niji / 10;
		if(NijiPoint[brd] < 0)
			cgraph[brd].niji_mainasu = 1;	//ϲŽ�׸�

#if 0	//���ް�۰���N�������̂Ł@2002.10.11(��)
		//-- �ꎟ�R���|�C���g�����L����h�b�g�����擾 --//
		itiji_dotto = (CRES_GAUGE_MAX << 16) * cgraph[brd].itiji / 100;
		if((itiji_dotto & 0x0000ffff) >= 0x00008000)	//�l�̌ܓ�
			itiji_dotto += 0x00010000;
		cgraph[brd].itiji_dotto = itiji_dotto >> 16;
		
		//-- �񎟐R���|�C���g�����L����h�b�g�����擾 --//
		niji_dotto = (CRES_GAUGE_MAX << 16) * cgraph[brd].niji / 100;
		if((niji_dotto & 0x0000ffff) >= 0x00008000)		//�l�̌ܓ�
			niji_dotto += 0x00010000;
		cgraph[brd].niji_dotto = niji_dotto >> 16;
#else
		//-- �ꎟ�R���|�C���g�����L����h�b�g�����擾 --//
		itiji_dotto = (CRES_GAUGE_MAX << 8) * cgraph[brd].itiji / 100;
		if((itiji_dotto & 0x00ff) >= 0x0080)	//�l�̌ܓ�
			itiji_dotto += 0x0100;
		cgraph[brd].itiji_dotto = itiji_dotto >> 8;
		
		//-- �񎟐R���|�C���g�����L����h�b�g�����擾 --//
		niji_dotto = (CRES_GAUGE_MAX << 8) * cgraph[brd].niji / 100;
		if((niji_dotto & 0x00ff) >= 0x0080)		//�l�̌ܓ�
			niji_dotto += 0x0100;
		cgraph[brd].niji_dotto = niji_dotto >> 8;
#endif

		//-- �n�[�g�A���̐����擾 --//
		cgraph[brd].itiji_starnum = GetStarNum(brd, 1);
		cgraph[brd].niji_heartnum = abs(GetHeartNum(brd, 1));
		
		//-- �ޯ����� �D���ȊO���߹�݂͍ŏI�I��Ұ����Max�ɂȂ�Ȃ��悤�ɂ��� --//
		if(ContestJyunni[brd] != 0){
			dotto1 = cgraph[brd].itiji_dotto;
			dotto2 = cgraph[brd].niji_dotto;
			if(cgraph[brd].niji_mainasu)
				dotto2 = -dotto2;
			if(dotto1 + dotto2 == CRES_GAUGE_MAX){
				if(dotto2 > 0)		//�񎟐R�����{�߲�ĂȂ�񎟐R�����ޯėD��ŉ�����
					cgraph[brd].niji_dotto--;
				else if(dotto1 > 0)	//������ɂł�0�������ꍇ�ׂ̈�ϲŽ�������Ă���
					cgraph[brd].itiji_dotto--;
			}
		}
	}
}


//=========================================================================
//	���ݶ������Q�Ƃ��āA̪��ނ��Ă��鐯�AʰĂ�̪��ނ��Ȃ����̂ɒu��������
//	Ұ�����グ��
// hosi_heart = 0:�� 1:ʰ�		count = ��납�炢���߂̐���������
//=========================================================================
static void StarHeartFadeEndCheck(u8 hosi_heart, u8 count)
{
	int i;
	u8 task;
	u8 se_up = 0, se_down = 0;
	u16 fix_scrn;
	u8 star;
	s8 heart;
	u32 now_dotto;
	
	if(hosi_heart == 0){
		for(i = 0; i < BREEDER_MAX; i++){
			star = cgraph[i].itiji_starnum;
			if(count < star){
				*(vu16*)(CRES_FONT_SCRN+
					(0x20*(CRES_STAR_Y0+i*3) 
					+ CRES_STAR_X0 + star - count-1)*2) = CRES_STAR_FIX;
				
				//���[�^�[�㏸������
				task = AddTask(MeterUpDownSeq, 10);
				now_dotto = (cgraph[i].itiji_dotto << 16) 
					/ cgraph[i].itiji_starnum * (count+1);
				if((now_dotto & 0x0000ffff) >= 0x00008000)
					now_dotto += 0x00010000;	//�l�̌ܓ�
				TaskTable[task].work[0] = i;
				TaskTable[task].work[1] = now_dotto >> 16;
				cres->meter_eff++;
				se_up++;
			}
		}
	}
	else{	//ʰ�
		for(i = 0; i < BREEDER_MAX; i++){
			heart = cgraph[i].niji_heartnum;
			if(cgraph[i].niji_mainasu)
				fix_scrn = CRES_HEARTBL_FIX;
			else
				fix_scrn = CRES_HEART_FIX;
			
			if(count < heart){
				*(vu16*)(CRES_FONT_SCRN+
					(0x20*(CRES_HEART_Y0+i*3) 
					+ CRES_HEART_X0 + heart - count-1)*2) = fix_scrn;
				task = AddTask(MeterUpDownSeq, 10);
				now_dotto = (cgraph[i].niji_dotto << 16) 
					/ cgraph[i].niji_heartnum * (count+1);
				if((now_dotto & 0x0000ffff) >= 0x00008000)
					now_dotto += 0x00010000;	//�l�̌ܓ�
				TaskTable[task].work[0] = i;
				if(cgraph[i].niji_mainasu){
					TaskTable[task].work[2] = 1;
					se_down++;
				}
				else
					se_up++;
				if(cgraph[i].niji_mainasu)
					TaskTable[task].work[1] =(-(now_dotto >> 16))+cgraph[i].itiji_dotto;
				else
					TaskTable[task].work[1] = (now_dotto >> 16) + cgraph[i].itiji_dotto;
				cres->meter_eff++;
			}
		}
	}
	
	if(se_down)
		SePlay(SE_BOO);
	if(se_up)
		SePlay(SE_PIN);
}


//----------------------------------------------------------
//	�e��ذ�ް��Ұ�����グ��
//----------------------------------------------------------
#define METER_FAST_X	7
#define METER_FAST_Y	6
#define METER_DOTTO_SCRN0	0x504c
#define METER_DOTTO_SCRN4	0x505b
static void MeterUpDownSeq(u8 id)
{
	u8 brd, add;
	int i;
	u16 scrn;
	s16 now_dotto;
	s16 mainus;
	u8 skip = 0, end = 0;
	
	brd = TaskTable[id].work[0];
	now_dotto = TaskTable[id].work[1];
	mainus = TaskTable[id].work[2];
	
	if(mainus && cres->meter[brd] <= 0)
		skip = 1;
	else if(mainus == 0 && cres->meter[brd] >= CRES_GAUGE_MAX)
		skip = 1;
	
	if(cres->meter[brd] == now_dotto)
		end = 1;
	
	if(end == 0){
		if(skip)
			cres->meter[brd] = now_dotto;
		else if(mainus)
			cres->meter[brd]--;
		else
			cres->meter[brd]++;
	}
	
	if(skip == 0 && end == 0){
		for(i = 0; i < (CRES_GAUGE_MAX / 8); i++){
			if(cres->meter[brd] >= 8*(i+1))
				add = 8;
			else{
				if(cres->meter[brd] >= 8*i)
					add = cres->meter[brd] % 8;
				else
					add = 0;
			}
			
			if(add < 4)
				scrn = METER_DOTTO_SCRN0 + add;
			else
				scrn = METER_DOTTO_SCRN4 + (add - 4);
			
			*(vu16*)(CRES_WIN_SCRN+
				(0x20*(METER_FAST_Y + brd*3) + METER_FAST_X + i)*2) = scrn;
		}
	}
	
	if(end){
		cres->meter_eff--;
		DelTask(id);
	}
}

