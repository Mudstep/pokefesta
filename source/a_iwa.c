//=========================================================================
//	
//	��n �Z�G�t�F�N�g
//
//=========================================================================
#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "madefine.h"
#include "wazatool.h"
#include "waza_dat.h"
#include "waza_eff.h"
#include "server.h"
#include "calctool.h"

#include "bss.h"
#include "cli_ef.h"
#include "laster.h"
#include "task.h"
#include "pm_aff.h"
#include "gauge.h"
#include "client.h"
#include "ef_tool.h"
#include "fight.h"
#include "palanm.h"
#include "decord.h"
#include "contest.h"
#include "cli_def.h"
#include "client_t.h"
#include "poketool.h"
#include "mus_tool.h"




//=========================================================================
//
//		�u���킨�Ƃ��v�A�u���킭�����v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void SeqWazaIwaOtosiInit(actWork *Xreg);
static void SeqWazaIwaOtosiWait(actWork *Xreg);
static void SeqWazaIwaKudakiSubInit(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define IWA_DY_OFFSET	14
#define IWA_SEC_SP		4
#define IWA_LOOP1		(64 / IWA_SEC_SP)
#define IWA_FURIHABA_Y1	(-70)
#define IWA_LOOP2		(128 / IWA_SEC_SP)
#define IWA_FURIHABA_Y2	(-24)

//���킭����
#define SEC_SP				4			//128������؂�鐔���w��
#define LOOP_NUM			(128 / SEC_SP)

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
static const actAnm IwaOtosiAnm0[] = {
	{OBJ16_32x32 * 2, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm IwaOtosiAnm1[] = {
	{OBJ16_32x32 * 3, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm IwaOtosiAnm2[] = {
	{OBJ16_32x32 * 4, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm *const IwaOtosiAnmTbl[]={
	IwaOtosiAnm0,
	IwaOtosiAnm1,
	IwaOtosiAnm2,
};

const	actHeader ActIwaOtosiHeader={
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataNormal[2],
	IwaOtosiAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaIwaOtosiInit,
};

// ���킭����
const	actHeader ActIwaKudakiSubHeader={
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataNormal[2],
	IwaOtosiAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaIwaKudakiSubInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:Xoffset   1:�g�p����AnmNo	2:�޳��ތ��X�U�蕝
//		  3:0=�m�[�}�� 1=���W2�̑I�����[�h
//-------------------------------------------------------------
static void SeqWazaIwaOtosiInit(actWork *Xreg)
{
	if(WazaEffWork[3])
		GetCenterEffPos(DefenceNo, EFF_MODE, &Xreg->x, &Xreg->y);
	
	Xreg->x += WazaEffWork[0];
	Xreg->y += IWA_DY_OFFSET;
	ActAnmChg(Xreg, WazaEffWork[1]);
	ActAnm(Xreg);
	
	Xreg->work[0] = 0;
	Xreg->work[1] = 0;
	Xreg->work[2] = IWA_SEC_SP;
	Xreg->work[3] = IWA_LOOP1;
	Xreg->work[4] = IWA_FURIHABA_Y1;
	Xreg->work[5] = WazaEffWork[2];
	SetWorkSeq(Xreg, SeqWazaIwaOtosiWait);
	Xreg->move = PMObjKaiten4;
	Xreg->move(Xreg);
}

static void SeqWazaIwaOtosiWait(actWork *Xreg)
{
	Xreg->x += Xreg->work[5];
	Xreg->work[0] = 192;
	Xreg->work[1] = Xreg->work[5];
	Xreg->work[2] = IWA_SEC_SP;
	Xreg->work[3] = IWA_LOOP2;
	Xreg->work[4] = IWA_FURIHABA_Y2;
	SetWorkSeq(Xreg, W_AffCheckEndSeq);
	Xreg->move = PMObjKaiten4;
	Xreg->move(Xreg);
}

//=========================================================================
//	offset 0:�n�_Xoffset  1:�n�_Yoffset
//		   2:�I�_X�����̾��  3:�I�_Y�����̾��
//		   4:ٰ�߉�(�����������قǑ����Ȃ�)	  5:�g�p����AnmNo
//=========================================================================
static void SeqWazaIwaKudakiSubInit(actWork *Xreg)
{
	ActAnmChg(Xreg, WazaEffWork[5]);
	ActAnm(Xreg);
	
	if(MineEnemyCheck(AttackNo))
		Xreg->x -= WazaEffWork[0];
	else
		Xreg->x += WazaEffWork[0];
	Xreg->y += WazaEffWork[1];
	Xreg->work[0] = WazaEffWork[4];
	Xreg->work[1] = Xreg->x;
	Xreg->work[2] = Xreg->x + WazaEffWork[2];
	Xreg->work[3] = Xreg->y;
	Xreg->work[4] = Xreg->y + WazaEffWork[3];
	W_GetStraightMoveLoop(Xreg);
	Xreg->work[3] = 0;
	Xreg->work[4] = 0;
	Xreg->move = W_ParallelMove2Seq;
	SetWorkSeq(Xreg, W_AffCheckEndSeq);
}















//=========================================================================
//
//		���Ȃ��炵(���͍��n��2002.07.23(��))
//												by matsuda 2002.02.07(��)
//=========================================================================
static void SunaArasiInit(actWork *xreg);
static void SunaArasiMain(actWork *xreg);

const actHeader ActSunaArasiHeader = 
{
	WAZACHR_ID_SUNA,
	WAZACHR_ID_SUNA,
	&WazaOamDataNormal[0],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	SunaArasiInit,
};

//-- �������� --//
extern const actAnm *const YukiAnmTbl[];
#if 1	//chg_02.08.28_iwasawa
extern const actAnm *const HaidoroAnmTbl[];
static const actAffAnm UzusioAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0x00C0, 0x00C0,0,ACT_AFSET},
	{ 0x0002,-0x0003,0,5},
	{-0x0002, 0x0003,0,5},
	{ACT_AFLOOP,1,0,0},
};
static const	actAffAnm * const UzusioAffTbl[] = {
	UzusioAff0,
};
const actHeader ActUzusioHeader = 
{
	WAZACHR_ID_HAIDORO,
	WAZACHR_ID_HAIDORO,
	&WazaOamDataBldAffine[1],
	HaidoroAnmTbl,
	ACT_NTRANS,
	UzusioAffTbl,
	SunaArasiInit,
};
#else
const actHeader ActUzusioHeader = 
{
	WAZACHR_ID_YUKI2,
	WAZACHR_ID_YUKI2,
	&WazaOamDataNormal[0],
	&YukiAnmTbl[5],
	ACT_NTRANS,
	DummyActAffTbl,
	SunaArasiInit,
};
#endif

//-- �ق̂��̂��� --//
extern const actAnm *const FireAnmTbl[];
const	actHeader ActHonoonoUzuHeader={
	WAZACHR_ID_FIRE,
	WAZACHR_ID_FIRE,
	&WazaOamDataNormal[2],
	FireAnmTbl,
	0,
	DummyActAffTbl,
	SunaArasiInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�㏸���x(�Œ菭��)	3:Sync��	4:�b���x	5:�U�蕝X
//	6:0=�U�����ɏo��  1=�h�䑤�ɏo��
//----------------------------------------------------------
static void SunaArasiInit(actWork *xreg)
{
	if(WazaEffWork[6] == 0)
		WazaDefaultPosSet2(xreg, EFF_MODE);
	else
		WazaDefaultPosSet1(xreg, EFF_MODE);
	xreg->work[0] = WazaEffWork[3];
	xreg->work[1] = WazaEffWork[2];
	xreg->work[2] = WazaEffWork[4];
	xreg->work[3] = WazaEffWork[5];
	xreg->move = SunaArasiMain;
}

static void SunaArasiMain(actWork *xreg)
{
	xreg->work[4] += xreg->work[1];
	xreg->dy = -(xreg->work[4] >> 8);
	xreg->dx = SinMove(xreg->work[5], xreg->work[3]);
	xreg->work[5] = 0xff & (xreg->work[5] + xreg->work[2]);
	if(xreg->work[0]-- == 0)
		DelWazaEffect(xreg);
}











//=========================================================================
//
//		���Ȃ��炵
//												by matsuda 2002.07.23(��)
//=========================================================================
static void SunaArasiHaikeiMain(u8 id);

//----------------------------------------------------------
//	�t�F�[�h�C�����Ă��鍻���p�̔w�i
//	offset	0: 1=�G�����ňړ��̍��E���]������	0=���Ȃ�
//----------------------------------------------------------
#define SUNAARASI_HAIKEI_CHGWAIT		3	//�w�i̪��޲݁A��Ď��̳���
#define SUNAARASI_HAIKEI_ENDWAIT		100	//�w�i̪��ޱ�Ă���܂ł̳���
#define SUNAARASI_BLDMAX				7
void SunaArasiHaikeiTask(u8 id)
{
	WazaBgData wazabg;
	u8 muki = 0;
	
	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL | BLD_BG1_1ST;
	*(vu16*)REG_BLDALPHA = 16 << 8;

	(*(vBgCnt *)REG_BG1CNT).Priority = 1;
	(*(vBgCnt *)REG_BG1CNT).Size = 0;
	
	if(ContestCheck() == 0)//�̪�Ă̊Ԃ�����׸��ް���ύX(��ذ��ް��̊֌W��)
		(*(vBgCnt *)REG_BG1CNT).CharBasep = 1;
	ScrX1 = 0;
	ScrY1 = 0;
	*(vu16 *)REG_BG1HOFS = ScrX1;
	*(vu16 *)REG_BG1VOFS = ScrY1;
	GetWazaEffBgAdrs(&wazabg);
	DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x1000, 32);
	DecordVram((void*)s_arashi_psc_ADRS, wazabg.ScrnAdrs);
	DecordVram((void*)s_arashi_pch_ADRS, wazabg.CharAdrs);
	DecordPaletteWork((void*)s_arashi_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	if(ContestCheck())
		ScrnPalRewrite(wazabg.PlttNo, (u32)wazabg.ScrnAdrs, 0, 0);

	if(WazaEffWork[0]){
		if(MineEnemyCheck(AttackNo))
			muki = 1;
	}
	TaskTable[id].work[0] = muki;
	TaskTable[id].TaskAdrs = SunaArasiHaikeiMain;
}

static void SunaArasiHaikeiMain(u8 id)
{
	WazaBgData wazabg;

	if(TaskTable[id].work[0] == 0)
		ScrX1 += -6;
	else
		ScrX1 += 6;
	ScrY1 += -1;
	switch(TaskTable[id].work[12])
	{
		case 0:
			if(TaskTable[id].work[10]++ == SUNAARASI_HAIKEI_CHGWAIT){
				TaskTable[id].work[10] = 0;
				
				TaskTable[id].work[11]++;
				*(vu16*)REG_BLDALPHA = 
					(16-TaskTable[id].work[11])<<8 | TaskTable[id].work[11];
				if(TaskTable[id].work[11] == SUNAARASI_BLDMAX){
					TaskTable[id].work[12]++;
					TaskTable[id].work[11] = 0;
				}
			}
			break;
		case 1:
			if(TaskTable[id].work[11]++ == SUNAARASI_HAIKEI_ENDWAIT){
				TaskTable[id].work[11] = SUNAARASI_BLDMAX;
				TaskTable[id].work[12]++;
			}
			break;
		case 2:
			if(TaskTable[id].work[10]++ == SUNAARASI_HAIKEI_CHGWAIT){
				TaskTable[id].work[10] = 0;
				
				TaskTable[id].work[11]--;
				*(vu16*)REG_BLDALPHA = 
					(16-TaskTable[id].work[11])<<8 | TaskTable[id].work[11];
				if(TaskTable[id].work[11] == 0){
					TaskTable[id].work[12]++;
					TaskTable[id].work[11] = 0;
				}
			}
			break;
		case 3:
			GetWazaEffBgAdrs(&wazabg);
			DIV_DMACLEAR(3, 0, wazabg.CharAdrs, 0x2000, 32);
			DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x800, 32);
			if(ContestCheck() == 0)
				(*(vBgCnt *)REG_BG1CNT).CharBasep = 0;
			TaskTable[id].work[12]++;
		case 4:
			ScrX1 = 0;
			ScrY1 = 0;
			*(vu16*)REG_BLDCNT = 0;
			*(vu16*)REG_BLDALPHA = 0;
			(*(vBgCnt *)REG_BG1CNT).Priority = EFF_BG1_PRI;
			DelTaskEffect(id);
			break;
	}
}

//=========================================================================
//	��OBJ
//=========================================================================
static void SunaObjInit(actWork *xreg);

const	actHeader SunaArasiHeader={
	WAZACHR_ID_SUNAARASI,
	WAZACHR_ID_SUNAARASI,
	&WazaOamDataNormal[6],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	SunaObjInit,
};

static const OAMPatData SunaObjPatData[] = 
{
	{-16, 0, 1, 2, 0, 1},	//�������S�����Xoffs,Yoffs, Shape,Size,CharNoOffs, pri
	{+16, 0, 1, 2, 8, 1},
};
static const OAMPatSt SunaObjPat = {2, SunaObjPatData};

//----------------------------------------------------------
//	0:YPos	1:���xX(�Œ菬��)	2:���xY(�Œ菭��)	
//	3:1=�G�����ňړ��̍��E���]������	0=���Ȃ�
//----------------------------------------------------------
static void SunaObjInit(actWork *xreg)
{
	if(xreg->work[0] == 0){
		if(WazaEffWork[3] && MineEnemyCheck(AttackNo)){
			xreg->x = 240+64;
			WazaEffWork[1] = -WazaEffWork[1];
			xreg->work[5] = 1;
			xreg->oamData.AffineParamNo = 8;	//Hflip
		}
		else
			xreg->x = -64;
		xreg->y = WazaEffWork[0];
		SetActOamPat(xreg, &SunaObjPat);
		xreg->work[1] = WazaEffWork[1];
		xreg->work[2] = WazaEffWork[2];
		xreg->work[0]++;
	}
	else{
		xreg->work[3] += xreg->work[1];
		xreg->work[4] += xreg->work[2];
		xreg->dx += xreg->work[3] >> 8;
		xreg->dy += xreg->work[4] >> 8;
		xreg->work[3] &= 0x00ff;
		xreg->work[4] &= 0x00ff;
		if(xreg->work[5] == 0){
			if(xreg->x + xreg->dx > 240+32)
				xreg->move = DelWazaEffect;
		}
		else{
			if(xreg->x+xreg->dx < -32)
				xreg->move = DelWazaEffect;
		}
	}
}










//=========================================================================
//
//		���񂵂̂�����
//												by matsuda 2002.02.14(��)
//=========================================================================
static void GensinoTikaraInit(actWork *xreg);
//static void GensinoTikaraWait(actWork *xreg);
//static void GensinoTikaraMove(actWork *xreg);

static const actAnm IwaAnm0[] = {
	{OBJ16_32x32 * 0, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm IwaAnm1[] = {
	{OBJ16_32x32 * 1, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm IwaAnm2[] = {
	{OBJ16_32x32 * 2, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm IwaAnm3[] = {
	{OBJ16_32x32 * 3, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm IwaAnm4[] = {
	{OBJ16_32x32 * 4, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm IwaAnm5[] = {
	{OBJ16_32x32 * 5, 1,0,0},
	{ACT_ANMEND, 0, 0,0},
};
static const actAnm *const IwaAnmTbl[]={
	IwaAnm0,	//�ォ�珇�ɑ傫��
	IwaAnm1,
	IwaAnm2,
	IwaAnm3,
	IwaAnm4,
	IwaAnm5,
};

const	actHeader ActGensiHeader={
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataNormal[2],
	IwaAnmTbl,
	0,
	DummyActAffTbl,
	GensinoTikaraInit,
};

#if 1
//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�I�_Y�̾��
//	3:�㏸Sync��	4:���No
//----------------------------------------------------------
static void GensinoTikaraInit(actWork *xreg)
{
	ActAnmChg(xreg, WazaEffWork[4]);
	WazaDefaultPosSet2(xreg, EFF_MODE);
	xreg->work[0] = WazaEffWork[3];
	xreg->work[2] = xreg->x;
	xreg->work[4] = xreg->y + WazaEffWork[2];
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}

#else		// 2002.02.21(��) �ʂ̴̪�ĂŎg�p���邩������Ȃ��������Ă���
//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�I�_Y�̾��
//	3:�㏸Sync��	4:����ɔ��ł����܂ł̳���	5:���No
//----------------------------------------------------------
#define GENSINOTIKARA_ATTACKSYNC	15
static void GensinoTikaraInit(actWork *xreg)
{
	ActAnmChg(xreg, WazaEffWork[5]);
	WazaDefaultPosSet2(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[3];
	xreg->work[2] = xreg->x;
	xreg->work[4] = xreg->y + WazaEffWork[2];
	xreg->work[5] = WazaEffWork[4];
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, GensinoTikaraWait);
}

static void GensinoTikaraWait(actWork *xreg)
{
	xreg->work[0] = xreg->work[5];
	xreg->move = W_WaitSeq;
	SetWorkSeq(xreg, GensinoTikaraMove);
}

static void GensinoTikaraMove(actWork *xreg)
{
	xreg->x += xreg->dx;
	xreg->y += xreg->dy;
	xreg->dx = xreg->dy = 0;
	xreg->work[0] = GENSINOTIKARA_ATTACKSYNC;
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}
#endif




//=========================================================================
//	���낪��
//
//																	TAYA
//=========================================================================
enum {
	KOROGARU_SEQ,
	KOROGARU_COUNT,
	KOROGARU_X,
	KOROGARU_Y,
	KOROGARU_AX,
	KOROGARU_AY,
	KOROGARU_POKE_DX,
	KOROGARU_POKE_DY,
	KOROGARU_LOOP,
	KOROGARU_IWA_TIMER,
	KOROGARU_IWA_WAIT,
	KOROGARU_IWA_COUNT,
	KOROGARU_IWA_VECTOR,
	KOROGARU_PAN,
	KOROGARU_ADDPAN,
	KOROGARU_ACTNO,
};

static void TaskKorogaruMain(u8 id);
static void add_korogaru_suna_actor(TASK_TABLE *my);
static u8 get_korogaru_cnt(void);

extern void SePlayPan( u16 no, s8 pan );

void TaskKorogaru(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];
	u16 cnt, ax, ay, dx, dy;
	s16 end_pan, start_pan;

	ax = GetWazaEffPos(AttackNo, PM_X);
	ay = GetWazaEffPos(AttackNo, EFF_Y) + 24;
	dx = GetWazaEffPos(DefenceNo, PM_X);
	dy = GetWazaEffPos(DefenceNo, EFF_Y) + 24;

	// �����U���̎��͐����ɂ݈̂ړ�������
	if((AttackNo^2)==DefenceNo)
	{
		dy = ay;
	}


	cnt = get_korogaru_cnt();

	if(cnt == 1){
//		my->work[KOROGARU_SEQ] = 0;
		my->work[KOROGARU_LOOP] = 32;
	}else{
//		my->work[KOROGARU_SEQ] = 3;
		my->work[KOROGARU_LOOP] = 48 - (cnt * 8);
	}
	my->work[KOROGARU_SEQ] = 0;

	my->work[KOROGARU_IWA_COUNT] = 0;
	my->work[KOROGARU_IWA_TIMER] = 0;
	my->work[KOROGARU_IWA_VECTOR] = 1;
	my->work[KOROGARU_IWA_WAIT] = (my->work[KOROGARU_LOOP] / 8) - 1;

	my->work[KOROGARU_X] = ax << 3;
	my->work[KOROGARU_Y] = ay << 3;
	my->work[KOROGARU_AX] = ((dx-ax) << 3) / my->work[KOROGARU_LOOP];
	my->work[KOROGARU_AY] = ((dy-ay) << 3) / my->work[KOROGARU_LOOP];
	my->work[KOROGARU_POKE_DX] = 0;
	my->work[KOROGARU_POKE_DY] = 0;

	start_pan = GetWazaEffSePan(WAZA_SE_L);
	end_pan = GetWazaEffSePan(WAZA_SE_R);
	my->work[KOROGARU_PAN] = start_pan;
	my->work[KOROGARU_ADDPAN] = (end_pan - start_pan) / my->work[KOROGARU_LOOP];

	my->work[KOROGARU_COUNT] = cnt;
	my->work[KOROGARU_ACTNO] = W_GetPokeActNo(0);

	my->TaskAdrs = TaskKorogaruMain;

}
static void TaskKorogaruMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[KOROGARU_SEQ]){
	case 0:
		my->work[KOROGARU_POKE_DX] -= my->work[KOROGARU_AX];
		my->work[KOROGARU_POKE_DY] -= my->work[KOROGARU_AY];
		ActWork[my->work[KOROGARU_ACTNO]].dx = my->work[KOROGARU_POKE_DX] >> 3;
		ActWork[my->work[KOROGARU_ACTNO]].dy = my->work[KOROGARU_POKE_DY] >> 3;
		if(++(my->work[KOROGARU_IWA_TIMER]) == 10){
			my->work[KOROGARU_IWA_COUNT] = 20;
			my->work[KOROGARU_SEQ]++;
		}
		SePlayPan(SE_W029, my->work[KOROGARU_PAN]);
		break;

	case 1:
		if(--(my->work[KOROGARU_IWA_COUNT]) == 0){
			my->work[KOROGARU_SEQ]++;
		}
		break;

	case 2:
		my->work[KOROGARU_IWA_TIMER]--;
		if(my->work[KOROGARU_IWA_TIMER]){
			my->work[KOROGARU_POKE_DX] += my->work[KOROGARU_AX];
			my->work[KOROGARU_POKE_DY] += my->work[KOROGARU_AY];
		}else{
			my->work[KOROGARU_POKE_DX] = 0;
			my->work[KOROGARU_POKE_DY] = 0;
			my->work[KOROGARU_SEQ]++;
		}
		ActWork[my->work[KOROGARU_ACTNO]].dx = my->work[KOROGARU_POKE_DX] >> 3;
		ActWork[my->work[KOROGARU_ACTNO]].dy = my->work[KOROGARU_POKE_DY] >> 3;
		break;

	case 3:
		my->work[KOROGARU_X] += my->work[KOROGARU_AX];
		my->work[KOROGARU_Y] += my->work[KOROGARU_AY];
		if(++(my->work[KOROGARU_IWA_TIMER]) >= my->work[KOROGARU_IWA_WAIT]){
			my->work[KOROGARU_IWA_TIMER] = 0;
			add_korogaru_suna_actor(my);
			my->work[KOROGARU_PAN] += my->work[KOROGARU_ADDPAN];
			SePlayPan(SE_W091, my->work[KOROGARU_PAN]);
		}
		if(--(my->work[KOROGARU_LOOP]) == 0){
			my->work[KOROGARU_SEQ]++;
		}
		break;

	case 4:
		if(my->work[KOROGARU_IWA_COUNT] == 0){
			DelTaskEffect(id);
		}
		break;

	}

}

static void korogaru_suna_actmove(actWork *act);
static const actHeader KorogaruSunaHeader = 
{
	WAZACHR_ID_SUNA,
	WAZACHR_ID_SUNA,
	&WazaOamDataNormal[0],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	korogaru_suna_actmove,
};
static const actHeader KorogaruIwaHeader = {
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataNormal[2],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	korogaru_suna_actmove,
};

static void add_korogaru_suna_actor(TASK_TABLE *my)
{
	const actHeader *header;
	u16 x, y;
	u8 actno, char_no;

	switch(my->work[KOROGARU_COUNT]){
	case 1:
		header = &KorogaruSunaHeader;
		char_no = 0;
		break;
	case 2:
	case 3:
		header = &KorogaruIwaHeader;
		char_no = 16*5;
		break;
	case 4:
		header = &KorogaruIwaHeader;
		char_no = 16*4;
		break;
	case 5:
		header = &KorogaruIwaHeader;
		char_no = 16*3;
		break;

	default:
		return;
	}

	x = my->work[KOROGARU_X] >> 3;
	y = my->work[KOROGARU_Y] >> 3;
	x += (my->work[KOROGARU_IWA_VECTOR] * 4);

	actno = AddActor(header, x, y, C_POKE1PRI+5);
	if(actno != ACT_MAX){
		ActWork[actno].work[0] = 18;
		ActWork[actno].work[2] = x + (my->work[KOROGARU_IWA_VECTOR] * 20) + (my->work[KOROGARU_COUNT] * 3);
		ActWork[actno].work[4] = y;
		ActWork[actno].work[5] = -16 - (my->work[KOROGARU_COUNT]*2);
		ActWork[actno].oamData.CharNo += char_no;
		YamanariMoveInit(&ActWork[actno]);
		my->work[KOROGARU_IWA_COUNT]++;
	}

	my->work[KOROGARU_IWA_VECTOR] *= -1;

}

static void korogaru_suna_actmove(actWork *act)
{
	if(YamanariMoveMain(act)){
		u8 taskno;

		taskno = CheckTaskNo(TaskKorogaruMain);
		if(taskno != NOT_EXIST_TASK_ID){
			TaskTable[taskno].work[KOROGARU_IWA_COUNT]--;
		}
		DelActor(act);
	}
}
// �A������ڂ��H
static u8 get_korogaru_cnt(void)
{
	u8 cnt;
	cnt = pwkc->korogarucnttmp - pwkc->korogarucnt;
	if(cnt < 1 || cnt > 5){
		cnt = 1;
	}
	return cnt;
}


//=========================================================================
//	���񂹂��ӂ���
//
//																	mori
//=========================================================================
static void SeqWazaGansekiInit(actWork *Xreg);
static void SeqWazaGansekiMain(actWork *Xreg);
const	actHeader ActGansekiHeader={
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataNormal[2],
	IwaAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaGansekiInit,
};


// =================================================================
// SeqWazaGansekiInit
// �T�v  : 
// ����  : [0] �I�_Xoffset
// ����  : [1] �I�_Yoffset
// ����  : [2] ���h�b�g�ォ��J�n���邩
// ����  : [3] �I�_�ɂ��Ă��牽Sync�c�邩
//       : [4] ��̎w��
// �߂�l: none
// =================================================================
static void SeqWazaGansekiInit(actWork *Xreg)
{
	ActAnmChg(Xreg,WazaEffWork[4]);
	Xreg->dx = WazaEffWork[0];
	Xreg->work[2]  = WazaEffWork[1];
	Xreg->work[3] -= WazaEffWork[2];
	Xreg->work[0]  = 3;
	Xreg->work[1]  = WazaEffWork[3];
	Xreg->move     = SeqWazaGansekiMain;
	Xreg->banish   = 1;
}

static void SeqWazaGansekiMain(actWork *Xreg)
{
	Xreg->banish = 0;
	if(Xreg->work[3] !=0){
		Xreg->dy = Xreg->work[2] + Xreg->work[3];
		Xreg->work[3] += Xreg->work[0];
		Xreg->work[0]++;
		if(Xreg->work[3] > 0) Xreg->work[3] = 0;
	}else{
		if(--Xreg->work[1] == 0){
			DelWazaEffect(Xreg);
		}
	}
}

//=========================================================================
//	���b�N�u���X�g
//
//																	mori
//=========================================================================
//static void SeqWazaRockBlastInit(actWork *Xreg);
//static void SeqWazaRockBlastMain(actWork *Xreg);


//-- [���b�N�u���X�g] --//
static const actAffAnm RockBlastAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,-5,5},
	{ACT_AFLOOP,0,0,0},
};
static const actAffAnm RockBlastAff1[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,5,5},
	{ACT_AFLOOP,0,0,0},
};
static const	actAffAnm * const RockBlastAffTbl[] = {
	RockBlastAff0,
	RockBlastAff1,
};
static void SeqRockBlastInit(actWork *act)
{
	if(MineEnemyCheck(AttackNo)==SIDE_ENEMY) ActAffAnmChg(act,1);
	W_StraightMoveInit(act);
//	act->move = W_StraightMoveMain;
}

const	actHeader ActRockBlastHeader={
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataAffine[2],
	IwaAnmTbl,
	0,
	RockBlastAffTbl,
	SeqRockBlastInit,
};


//=========================================================================
//	�����イ�Ȃ�
//
//																	mori
//=========================================================================
static void ActIwaKakeraSeqInit(actWork *Xreg);
static void ActIwaKakeraSeqMain(actWork *Xreg);
const	actHeader ActIwaKakeraHeader={
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataAffine[2],
	IwaAnmTbl,
	0,
	RockBlastAffTbl,
	ActIwaKakeraSeqInit,
};


//=========================================================================
//  ��̂����炪�Ƃт���
//
// [0]:Xoffset
// [1]:Yoffset
// [2]:SinCos�̑傫��
// [3]:��̎w��
//=========================================================================
static void ActIwaKakeraSeqInit(actWork *Xreg)
{
	Xreg->x = GetWazaEffPos(DefenceNo,EFF_X);
	Xreg->y = GetWazaEffPos(DefenceNo,EFF_Y);

	Xreg->x      += WazaEffWork[0];
	Xreg->y      += WazaEffWork[1];
	Xreg->work[1] = WazaEffWork[0];
	Xreg->work[2] = WazaEffWork[1];
	Xreg->work[5] = WazaEffWork[2];
	ActAnmChg(Xreg,WazaEffWork[3]);
	Xreg->move = ActIwaKakeraSeqMain;
}

static void ActIwaKakeraSeqMain(actWork *Xreg)
{
	Xreg->work[0] += 8;
	Xreg->work[3] += Xreg->work[1];
	Xreg->work[4] += Xreg->work[2];
	Xreg->dx += Xreg->work[3]/40;
	Xreg->dy -= SinMove(Xreg->work[0],Xreg->work[5]);
	if(Xreg->work[0] > 140){
		DelWazaEffect(Xreg);
	}
}


//=========================================================================
//  �����イ�Ȃ��̕�����_���[�W��
//
//
//=========================================================================
void TaskTikyuunageCheck(u8 no)
{
	if(WazaEffDamage < 33) 							WazaEffWork[7] = 0;
	if(WazaEffDamage >= 33 && WazaEffDamage < 66) 	WazaEffWork[7] = 1;
	if(WazaEffDamage >=66) 							WazaEffWork[7] = 2;
	DelTaskEffect(no);
}


void  SeqWazaTaskTikyuuNageInit(u8 no)
{
	if(TaskTable[no].work[0]==0){
		HaikeiBGLoopSizeSet(0);	//�w�i��BG�ɑ΂��āA��荞���׸ށA���ނ�Ă���
		TaskTable[no].work[1]=200;
	}
	
	ScrY3 += TaskTable[no].work[1]/10;
	TaskTable[no].work[1]-=3;
	
	if(TaskTable[no].work[0]==120){
		HaikeiBGLoopSizeSet(1);	//�w�iBG�̃��W�X�^�����ɖ߂�
		DelTaskEffect(no);
	}
	TaskTable[no].work[0]++;
}

void SeqWazaTaskTikyuuNage2(u8 no)
{
	if(TaskTable[no].work[0]==0){
		HaikeiBGLoopSizeSet(0);	//�w�i��BG�ɑ΂��āA��荞���׸ށA���ނ�Ă���
		TaskTable[no].work[0]++;
		TaskTable[no].work[2] = ScrY3;
	}
	TaskTable[no].work[1] += 80;
	TaskTable[no].work[1] &= 0xff;
	ScrY3 = TaskTable[no].work[2] + CosMove(4,TaskTable[no].work[1]);

	if(WazaEffWork[7]==0x0fff){	//west.s�Ń��[�N��0xffff������̂��Ď�����
		ScrY3 = 0;
		HaikeiBGLoopSizeSet(1);	//�w�iBG�̃��W�X�^�����ɖ߂�
		DelTaskEffect(no);
	}
}
//=========================================================================
//	���܂�
//
//																	mori
//=========================================================================
extern void SeqWazaTatumakiInit(actWork *Xreg);
const	actHeader ActTatumakiIwaHeader={
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataNormal[2],
	&IwaAnmTbl[4],
	0,
	RockBlastAffTbl,
	SeqWazaTatumakiInit,
};

/*----------�E�F�U�[�{�[���p-----------------*/

extern void SeqWazaBallAttackInit(actWork *Xreg);
const	actHeader ActWeatherIwaHeader={
	WAZACHR_ID_IWA,
	WAZACHR_ID_IWA,
	&WazaOamDataAffine[2],
	&IwaAnmTbl[2],
	0,
	RockBlastAffTbl,
	SeqWazaBallAttackInit,
};
