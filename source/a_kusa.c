//=========================================================================
//	
//	���n �Z�G�t�F�N�g
//
//=========================================================================
#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "madefine.h"

#include "wazatool.h"
#include "waza_dat.h"
#include "server.h"
#include "bss.h"
#include "waza_eff.h"
#include "calctool.h"
#include "task.h"

#include "cli_def.h"
#include "ef_tool.h"

#include "mus_tool.h"



//=========================================================================
//
//		�u�˂ނ育�ȁv�A�u���тꂲ�ȁv�A�u�ǂ��̂��ȁv
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
void SeqWazaKonaChrInit(actWork *Xreg);
void SeqWazaKonaChrMain(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define ANM_WAIT	5

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
static const actAnm KonaChrAnm[]={
	{OBJ16_16x8 * 0, ANM_WAIT, 0, 0},
	{OBJ16_16x8 * 1, ANM_WAIT, 0, 0},
	{OBJ16_16x8 * 2, ANM_WAIT, 0, 0},
	{OBJ16_16x8 * 3, ANM_WAIT, 0, 0},
	{OBJ16_16x8 * 4, ANM_WAIT, 0, 0},
	{OBJ16_16x8 * 5, ANM_WAIT, 0, 0},
	{OBJ16_16x8 * 6, ANM_WAIT, 0, 0},
	{OBJ16_16x8 * 7, ANM_WAIT, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};

static const actAnm *const KonaChrAnmTbl[]={
		KonaChrAnm,
};

const	actHeader ActNemuriKonaChrHeader={
	WAZACHR_ID_NEMURIKONA,
	WAZACHR_ID_NEMURIKONA,
	&WazaOamDataNormal[8],
	KonaChrAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaKonaChrInit,
};
const	actHeader ActSibireKonaChrHeader={
	WAZACHR_ID_SIBIREKONA,
	WAZACHR_ID_SIBIREKONA,
	&WazaOamDataNormal[8],
	KonaChrAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaKonaChrInit,
};
const	actHeader ActDokuKonaChrHeader={
	WAZACHR_ID_DOKUKONA,
	WAZACHR_ID_DOKUKONA,
	&WazaOamDataNormal[8],
	KonaChrAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaKonaChrInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:Xoffset 1:Yoffset 2:wait(wait�ɒB������DEL) 3:speed(����8�ޯď����_�ȉ�)
//        4:�U�蕝  5:�b���x
//-------------------------------------------------------------
void SeqWazaKonaChrInit(actWork *Xreg)
{
	Xreg->x += WazaEffWork[0];
	Xreg->y += WazaEffWork[1];
	Xreg->work[0] = WazaEffWork[2];
	Xreg->work[1] = WazaEffWork[3];
	if(MineEnemyCheck(AttackNo))
		Xreg->work[3] = -WazaEffWork[4];
	else
		Xreg->work[3] = WazaEffWork[4];
	Xreg->work[4] = WazaEffWork[5];
	Xreg->move=SeqWazaKonaChrMain;
}

void SeqWazaKonaChrMain(actWork *Xreg)
{
	if(Xreg->work[0] > 0)
	{
		Xreg->work[0]--;
		Xreg->dy = Xreg->work[2] >> 8;
		Xreg->work[2] += Xreg->work[1];
	
		Xreg->dx = SinMove(Xreg->work[5], Xreg->work[3]);
		Xreg->work[5] = 0xff & (Xreg->work[5] + Xreg->work[4]);
	}
	else
	{
		DelWazaEffect(Xreg);
	}
}
















//=========================================================================
//
//		�\�[���[�r�[��
//												by matsuda 2002.01.25(��)
//=========================================================================
static void HikariKyuusyuuInit(actWork *Xreg);
static void SolarBeemInit(actWork *xreg);
static void SolarBeemSubInit(actWork *xreg);
static void SolarBeemSubMain(actWork *xreg);

static const actAnm SolarAnm0[] =	//�ް�
{
	{OBJ16_8x8 * 0, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm SolarAnm1[] =
{
	{OBJ16_8x8 * 1, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm SolarAnm2[] =
{
	{OBJ16_8x8 * 2, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm SolarAnm3[] =
{
	{OBJ16_8x8 * 3, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm SolarAnm4[] =
{
	{OBJ16_8x8 * 4, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm SolarAnm5[] =
{
	{OBJ16_8x8 * 5, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm SolarAnm6[] =
{
	{OBJ16_8x8 * 6, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm SolarAnm7[] =	//�ްт̎������鋅
{
	{OBJ16_8x8 * 7, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm SolarAnm8[] =	//��
{
	{OBJ16_8x8 * 8, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm * const SolarAnmTbl[] = 
{
	SolarAnm0,	//�ް�
	SolarAnm1,
	SolarAnm2,
	SolarAnm3,
	SolarAnm4,
	SolarAnm5,
	SolarAnm6,
	SolarAnm7,	//�ްт̎������鋅
	SolarAnm8,	//��
};

static const actAffAnm hikarikyuusyuu_aff_anm[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{-5,-5,0,1},
	{ACT_AFLOOP,0,0,0},
};
static const	actAffAnm * const HikariKyuusyuuAffTbl[] = {
	hikarikyuusyuu_aff_anm,
};
const	actHeader ActHikariKyuusyuuHeader={	//�z�������
	WAZACHR_ID_SOLAR,
	WAZACHR_ID_SOLAR,
	&WazaOamDataBldAffine[1],
	&SolarAnmTbl[8],
	0,
	HikariKyuusyuuAffTbl,
	HikariKyuusyuuInit,
};

const	actHeader ActSolarBeemHeader={	//�ްт̒��S�𑖂���
	WAZACHR_ID_SOLAR,
	WAZACHR_ID_SOLAR,
	&WazaOamDataNormal[0],
	SolarAnmTbl,
	0,
	DummyActAffTbl,
	SolarBeemInit,
};

const	actHeader ActSolarBeemSubHeader={	//�ްт̎�����ԋ�
	WAZACHR_ID_SOLAR,
	WAZACHR_ID_SOLAR,
	&WazaOamDataNormal[0],
	&SolarAnmTbl[7],
	0,
	DummyActAffTbl,
	SolarBeemSubInit,
};

//-- �u�����킦��v --//
static const actAffAnm TakuwaeruAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0x140,0x140,0,ACT_AFSET},
	{-14,-14,0,1},
	{ACT_AFLOOP,1,0,0},
};
static const	actAffAnm * const TakuwaeruAffTbl[] = {
	TakuwaeruAff0,
};
const	actHeader TakuwaeruHeader={
	WAZACHR_ID_TAKUWAE,
	WAZACHR_ID_TAKUWAE,
	&WazaOamDataAffine2[0],
	DummyActAnmTbl,
	0,
	TakuwaeruAffTbl,
	HikariKyuusyuuInit,
};


//----------------------------------------------------------
//		�z�������
//	0:Xoffset	1:Yoffset	2:���S���B�܂ł�Sync��
//----------------------------------------------------------
static void HikariKyuusyuuInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[2];
	xreg->work[2] = GetWazaEffPos(AttackNo, PM_X);
	xreg->work[4] = GetWazaEffPos(AttackNo, PM_Y);
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, W_AffCheckEndSeq);
}


//----------------------------------------------------------
//		�r�[���{��
//	0:Xoffset	1:Yoffset	2:Sync��	3:���No
//----------------------------------------------------------
static void SolarBeemInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	ActAnmChg(xreg, WazaEffWork[3]);
	xreg->work[0] = WazaEffWork[2];
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}


//----------------------------------------------------------
//		�r�[���̎�����ԋ�
//	0:Xoffset	1:Yoffset	2:Sync��	3:�����p�x
//----------------------------------------------------------
#define SOLAR_FURIHABA_X	5
#define SOLAR_FURIHABA_Y	14
#define SOLAR_ADDSEC		15
static void SolarBeemSubInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[2];
	xreg->work[1] = xreg->x;
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	xreg->work[3] = xreg->y;
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	W_GetStraightSyncParam(xreg);
	xreg->work[5] = WazaEffWork[3];
	xreg->move = SolarBeemSubMain;
	xreg->move(xreg);
}

static void SolarBeemSubMain(actWork *xreg)
{
	if(W_ParallelSync(xreg)){
	//	DelWazaEffect(xreg);
		DelActor(xreg);
		return;
	}

	
	if(xreg->work[5] > 127)
		xreg->pri = GetPokeSoftPri(DefenceNo) + 1;
	else
		xreg->pri = GetPokeSoftPri(DefenceNo) + 6;
	xreg->dx += SinMove(xreg->work[5], SOLAR_FURIHABA_X);
	xreg->dy += CosMove(xreg->work[5], SOLAR_FURIHABA_Y);
	xreg->work[5] = 0xff & (xreg->work[5] + SOLAR_ADDSEC);
}

//----------------------------------------------------------
//	�r�[���̎�����Ա������Ǘ��������
//----------------------------------------------------------
#define NEXT_SOLARSUB_WAIT		6
#define SOLAR_BEEMSUB_SYNC		80
#define SOLAR_BEEMSUB_MAX		15//21

void SolarBeemSubTask(u8 id)
{
	if(TaskTable[id].work[0]-- == 0)
	{
		TaskTable[id].work[1]++;
		TaskTable[id].work[0] = NEXT_SOLARSUB_WAIT;
		WazaEffWork[0] = 15;
		WazaEffWork[1] = 0;
		WazaEffWork[2] = SOLAR_BEEMSUB_SYNC;
		WazaEffWork[3] = 0;
		AddActorJump(&ActSolarBeemSubHeader, 0,0, GetPokeSoftPri(DefenceNo)+1);
	}
	
	if(TaskTable[id].work[1] == SOLAR_BEEMSUB_MAX)
		DelTaskEffect(id);
}








//=========================================================================
//
//		�����Ƃ�
//												by matsuda 2002.01.30(��)
//=========================================================================
static void SeqWazaSuitoruInit(actWork *xreg);
static void SeqWazaSuitoruMain(actWork *xreg);

static const actAffAnm suitoru_aff_anm[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ -5,-5,0,1},
	{ACT_AFLOOP,0,0,0},
};
static const	actAffAnm * const suitoru_aff_tbl[] = {
	suitoru_aff_anm,
};
const	actHeader ActSuitoruHeader={
	WAZACHR_ID_SOLAR,
	WAZACHR_ID_SOLAR,
	&WazaOamDataBldAffine[1],
	&SolarAnmTbl[8],
	0,
	suitoru_aff_tbl,
	SeqWazaSuitoruInit,
};

//-------------------------------------------------------------
//  0:Xoffset 1:Yoffset 2:�U�蕝Y	3:Sync��
//-------------------------------------------------------------
static void SeqWazaSuitoruInit(actWork *xreg)
{
	WazaDefaultPosSet1(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[3];
	xreg->work[2] = GetWazaEffPos(AttackNo, PM_X);
	xreg->work[4] = GetWazaEffPos(AttackNo, PM_Y);
	xreg->work[5] = WazaEffWork[2];
	YamanariMoveInit(xreg);
	xreg->move = SeqWazaSuitoruMain;
}

static void SeqWazaSuitoruMain(actWork *xreg)
{
	if(YamanariMoveMain(xreg))
		DelWazaEffect(xreg);
}










//=========================================================================
//
//		�͂�����������
//												by matsuda 2002.06.14(��)
//=========================================================================
static void HakaiKousenInit(actWork *xreg);
static void HakaiKousenMain(actWork *xreg);

const	actHeader HakaiKousenHeader={
	WAZACHR_ID_SOLAR,
	WAZACHR_ID_SOLAR,
	&WazaOamDataNormal[0],
	SolarAnmTbl,
//	&SolarAnmTbl[8],
	0,
	DummyActAffTbl,
	HakaiKousenInit,
};

//----------------------------------------------------------
//	offset	�Ȃ�
//----------------------------------------------------------
#define HAKAIKOUSEN_SP		0x0040	//����4�ޯď���
#define HAKAI_FURIHABA_X	8//23
#define HAKAI_FURIHABA_Y	12
#define HAKAI_ADDSEC		24
#define HAKAI_SPRND			0x20
static void HakaiKousenInit(actWork *xreg)
{
	s8 sp_rnd;
	
	ActAnmChg(xreg, pp_rand() % 8);
	
	xreg->x = GetWazaEffPos(AttackNo, PM_X);
	xreg->y = GetWazaEffPos(AttackNo, PM_Y);
#if 1	//2002.08.16(��)
	if(MineEnemyCheck(AttackNo))
		xreg->x -= 20;
	else
		xreg->x += 20;
#else	//���Ȃ񂩏�����������
	EffXpositionSet(xreg, 20);
#endif

	sp_rnd = pp_rand() % HAKAI_SPRND;
	xreg->work[0] = HAKAIKOUSEN_SP + sp_rnd;
	xreg->work[1] = xreg->x;
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	xreg->work[3] = xreg->y;
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	W_GetStraightSpParam4(xreg);
	
	xreg->work[5] = pp_rand() & 0xff;
/*	xreg->dx += SinMove(xreg->work[5], HAKAI_FURIHABA_X);
	xreg->dy += CosMove(xreg->work[5], HAKAI_FURIHABA_Y);
	xreg->work[5] = 0xff & (xreg->work[5] + HAKAI_ADDSEC);
*/	
	xreg->work[6] = xreg->pri;
	xreg->move = HakaiKousenMain;
	xreg->move(xreg);
}

static void HakaiKousenMain(actWork *xreg)
{
	if(W_ParallelSync4(xreg)){
		DelWazaEffect(xreg);
		return;
	}
	
//	xreg->dx += SinMove(xreg->work[5], HAKAI_FURIHABA_X);
	xreg->dy += CosMove(xreg->work[5], HAKAI_FURIHABA_Y);
	if(xreg->work[5] < 127)
		xreg->pri = xreg->work[6];
	else
		xreg->pri = xreg->work[6]+1;
	xreg->work[5] = 0xff & (xreg->work[5] + HAKAI_ADDSEC);
}














//=========================================================================
//
//		��ǂ肬�̃^�l
//												by matsuda 2002.01.30(��)
//=========================================================================
static void YadorigiInit(actWork *Xreg);
static void YadorigiMain(actWork *xreg);
static void YadorigiEndWait(actWork *xreg);

#define YADORIGI_ANMSYNC	60
#define YADORIGI_BANISH_WAIT	10
static const actAnm Yadori_anm0[] =
{
	{OBJ16_16x16 * 0, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm Yadori_anm1[] =
{
	{OBJ16_16x16 * 1, 7, 0, 0},
	{OBJ16_16x16 * 2, 7, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm * const YadoriAnmTbl[] = 
{
	Yadori_anm0,
	Yadori_anm1,
};
const	actHeader ActYadoriHeader={
	WAZACHR_ID_YADORIGI,
	WAZACHR_ID_YADORIGI,
	&WazaOamDataNormal[1],
	YadoriAnmTbl,
	0,
	DummyActAffTbl,
	YadorigiInit,
};

//-------------------------------------------------------------
//  0:�n�_Xoffset	1:�n�_Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//	4:Sync��		5:�U�蕝Y
//-------------------------------------------------------------
static void YadorigiInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];
	xreg->work[0] = WazaEffWork[4];
	xreg->work[2] = GetWazaEffPos(DefenceNo, EFF_X) + WazaEffWork[2];
	xreg->work[4] = GetWazaEffPos(DefenceNo, EFF_Y) + WazaEffWork[3];
	xreg->work[5] = WazaEffWork[5];
	YamanariMoveInit(xreg);
	xreg->move = YadorigiMain;
}

static void YadorigiMain(actWork *xreg)
{
	if(YamanariMoveMain(xreg))
	{
		xreg->banish = 1;
		xreg->work[0] = YADORIGI_BANISH_WAIT;
		xreg->move = W_WaitSeq;
		SetWorkSeq(xreg, YadorigiEndWait);
	}
}

static void YadorigiEndWait(actWork *xreg)
{
	xreg->banish = 0;
	ActAnmChg(xreg, 1);
	xreg->work[0] = YADORIGI_ANMSYNC;
	xreg->move = W_WaitSeq;
	SetWorkSeq(xreg, DelWazaEffect);
}













//=========================================================================
//
//		�킽�ق���
//												by matsuda 2002.02.13(��)
//=========================================================================
static void WatahousiInit(actWork *xreg);
static void WatahousiMain(actWork *xreg);

static const actAnm HousiAnm0[] =		//�����킽
{
	{OBJ16_16x16 * 0, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm HousiAnm1[] =		//�L�m�R�̖E�q
{
	{OBJ16_16x16 * 1, 7, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm * const HousiAnmTbl[] = 
{
	HousiAnm0,
	HousiAnm1,
};

const	actHeader ActHousiHeader={
	WAZACHR_ID_HOUSI,
	WAZACHR_ID_HOUSI,
	&WazaOamDataNormal[1],
	HousiAnmTbl,
	0,
	DummyActAffTbl,
	WatahousiInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�����p�x	3:Del����܂ł̳���		4:���No
//----------------------------------------------------------
#define WATAHOUSI_FURIHABA_X	32
#define WATAHOUSI_FURIHABA_Y	(-3)
#define WATAHOUSI_DY			0x0018	//���ɗ����Ă������x(�Œ菭��)
#define WATAHOUSI_ADDSEC		2
static void WatahousiInit(actWork *xreg)
{
	WazaDefaultPosSet1(xreg, PM_MODE);
	ActAnmChg(xreg, WazaEffWork[4]);
	if(WazaEffWork[4] == 1)	//�L�m�R�̖E�q�̏ꍇ�͔������׸ނ�ON�ɂ���
		xreg->oamData.ObjMode = 1;
	xreg->work[0] = WazaEffWork[3];
	xreg->work[1] = WazaEffWork[2];
	xreg->move = WatahousiMain;
	xreg->move(xreg);
}

static void WatahousiMain(actWork *xreg)
{
	u8 pri;
	
	xreg->dx = SinMove(xreg->work[1], WATAHOUSI_FURIHABA_X);
	xreg->dy = CosMove(xreg->work[1], WATAHOUSI_FURIHABA_Y);
	xreg->work[2] += WATAHOUSI_DY;
	xreg->dy += xreg->work[2] >> 8;
	
	if(xreg->work[1] > 63 && xreg->work[1] < 192)
		//xreg->pri = C_POKE1PRI - 1;
		xreg->oamData.Priority = GetPokeBGPri(DefenceNo);
	else{
		//xreg->pri = C_POKE1PRI + 1;
		pri = GetPokeBGPri(DefenceNo)+1;
		if(pri > 3)
			pri = 3;
		xreg->oamData.Priority = pri;
	}
	
	xreg->work[1] = 0xff & (xreg->work[1] + WATAHOUSI_ADDSEC);
	
	if(xreg->work[0]-- == 0)
		DelWazaEffect(xreg);
}


//----------------------------------------------------------
//	����ɏd�Ȃ炸�ɂ��܂���肱�߂�悤��BG�̃v���C�I���e�B�𑀍삷��
//----------------------------------------------------------
void HousiBGPriSet(u8 id)
{
	u8 bgno;
	
	if(ContestCheck() || FightCheck() == 0){
		DelTaskEffect(id);
		return;
	}
	
	bgno = GetPokeBGNo(DefenceNo);
	if(bgno == 1){
		(*(vBgCnt *)REG_BG2CNT).Priority = 3;
	}
	else{
		(*(vBgCnt *)REG_BG1CNT).Priority = 1;
	}
	DelTaskEffect(id);
}














//=========================================================================
//
//		�͂Ȃт�̂܂�
//												by matsuda 2002.02.14(��)
//=========================================================================
static void HanabiranoMaiInit(actWork *xreg);
static void HanabiranoMaiMain(actWork *xreg);
static void HanabiranoMaiSubInit(actWork *xreg);
static void HanabiranoMaiSubMain(actWork *xreg);

static const actAnm HanaAnm0[] =		//��(16x16)
{
	{OBJ16_16x16 * 0, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm HanaAnm1[] =		//�Ԃт�(8x8)
{
	{OBJ16_16x16 * 1, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm * const HanaAnmTbl[] = 
{
	HanaAnm0,
	HanaAnm1,
};

const	actHeader ActHanabiranoMaiHeader={	//��
	WAZACHR_ID_HANA,
	WAZACHR_ID_HANA,
	&WazaOamDataNormal[1],
	HanaAnmTbl,
	0,
	DummyActAffTbl,
	HanabiranoMaiInit,
};

const	actHeader ActHanabiranoMaiSubHeader={	//�Ԃт�
	WAZACHR_ID_HANA,
	WAZACHR_ID_HANA,
	&WazaOamDataNormal[0],
	&HanaAnmTbl[1],
	0,
	DummyActAffTbl,
	HanabiranoMaiSubInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�I�_Yoffset	3:Sync��
//----------------------------------------------------------
#define HANABIRA_DEFAULT_SEC	64		//�����p�x
#define HANABIRA_FURIHABA_X		32
#define HANABIRA_FURIHABA_Y		(-5)
#define HANABIRA_ADDSEC			5
static void HanabiranoMaiInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, EFF_MODE);
	xreg->work[0] = WazaEffWork[3];
	xreg->work[1] = xreg->x;
	xreg->work[2] = xreg->x;
	xreg->work[3] = xreg->y;
	xreg->work[4] = GetWazaEffPos(AttackNo, PM_Y) + WazaEffWork[2];
	W_GetStraightSyncParam(xreg);
	xreg->work[5] = HANABIRA_DEFAULT_SEC;
	xreg->move = HanabiranoMaiMain;
	xreg->move(xreg);
}

static void HanabiranoMaiMain(actWork *xreg)
{
	if(W_ParallelSync(xreg) == 0)
	{
		xreg->dx += SinMove(xreg->work[5], HANABIRA_FURIHABA_X);
		xreg->dy += CosMove(xreg->work[5], HANABIRA_FURIHABA_Y);

		if(xreg->work[5] > 63 && xreg->work[5] < 192)
			//xreg->pri = C_POKE1PRI - 1;
			xreg->pri = GetPokeSoftPri(AttackNo) - 1;
		else
			xreg->pri = GetPokeSoftPri(AttackNo) + 1;

		xreg->work[5] = 0xff & (xreg->work[5] + HANABIRA_ADDSEC);
	}
	else
		DelWazaEffect(xreg);
}

//----------------------------------------------------------
//	�Ԃ̂܂��𕑂��Ȃ��痎���Ă�Ԃт�
//	0:Xoffset	1:Yoffset	2:�I�_Yoffset	3:Sync��
//----------------------------------------------------------
#define HANABIRASUB_DEFAULT_SEC		64		//�����p�x
#define HANABIRASUB_FURIHABA_X		8
#define HANABIRASUB_ADDSEC			5
static void HanabiranoMaiSubInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[3];
	xreg->work[1] = xreg->x;
	xreg->work[2] = xreg->x;
	xreg->work[3] = xreg->y;
	xreg->work[4] = GetWazaEffPos(AttackNo, PM_Y) + WazaEffWork[2];
	W_GetStraightSyncParam(xreg);
	xreg->work[5] = HANABIRASUB_DEFAULT_SEC;
	xreg->move = HanabiranoMaiSubMain;
	xreg->move(xreg);
}

static void HanabiranoMaiSubMain(actWork *xreg)
{
	if(W_ParallelSync(xreg) == 0)
	{
		xreg->dx += SinMove(xreg->work[5], HANABIRASUB_FURIHABA_X);
		if((xreg->work[5] < 64 && xreg->work[5]+HANABIRASUB_ADDSEC >= 64)
			|| (xreg->work[5] < 192 && xreg->work[5]+HANABIRASUB_ADDSEC >= 192))
		{//�[�ɗ������҂𔽓]������
			xreg->oamData.AffineParamNo ^= 0x8;	//Hflip
		}
		xreg->work[5] = 0xff & (xreg->work[5] + HANABIRASUB_ADDSEC);
	}
	else
		DelWazaEffect(xreg);
}











//=========================================================================
//
//		�͂��σJ�b�^�[
//												by matsuda 2002.02.14(��)
//=========================================================================
static void SeqWazaHappaInit(actWork *Xreg);
static void SeqWazaHappaMain(actWork *Xreg);
static void HappaHira(actWork *Xreg);
//static void HappaSyu(actWork *Xreg);
//static void HappaEnd(actWork *Xreg);

#define LEFT_MOVE	1
#define RIGHT_MOVE	0
#define STOP_WAIT	18	//�ꎞ��~����
#define H_FURIHABA	25
#define ANM_WAIT	5

static const actAnm HappaAnm0[]={
	{OBJ16_16x16 * 0, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 1, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 2, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 3, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 4, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 5, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 4, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 3, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 2, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 1, ANM_WAIT, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm HappaAnm1[]={
	{OBJ16_16x16 * 6, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 7, ANM_WAIT, 0, 0},
	{OBJ16_16x16 * 8, ANM_WAIT, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};

static const actAnm *const HappaAnmTbl[]={
		HappaAnm0,
		HappaAnm1,
};

const	actHeader ActHappaHeader={				//���}�W�J�����[�t�ł��g�p
	WAZACHR_ID_HAPPA,
	WAZACHR_ID_HAPPA,
	&WazaOamDataNormal[1],
	HappaAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaHappaInit,
};

void SeqWazaTatumakiInit(actWork *Xreg);
const	actHeader ActTatumakiHappaHeader={
	WAZACHR_ID_HAPPA,
	WAZACHR_ID_HAPPA,
	&WazaOamDataNormal[1],
	HappaAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaTatumakiInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:vx, 1:vy, 2:up_loop
//-------------------------------------------------------------
static void SeqWazaHappaInit(actWork *Xreg)
{
	Xreg->x = GetWazaEffPos(AttackNo, PM_X);
	Xreg->y = GetWazaEffPos(AttackNo, PM_Y);
	Xreg->work[0] = WazaEffWork[0];
	Xreg->work[1] = WazaEffWork[1];
	Xreg->work[2] = WazaEffWork[2];
	Xreg->move=SeqWazaHappaMain;
}

static void SeqWazaHappaMain(actWork *Xreg)
{
	if(Xreg->work[2] == 0)
	{
		if(((Xreg->work[1]) & 1) == LEFT_MOVE)
		{
			Xreg->work[0] = 128;
			Xreg->work[1] = 0;
			Xreg->work[2] = 0;
		}
		else
		{
			Xreg->work[0] = 0;
			Xreg->work[1] = 0;
			Xreg->work[2] = 0;
		}
		Xreg->move = HappaHira;
	}
	else
	{
		Xreg->work[2]--;
		Xreg->x += Xreg->work[0];
		Xreg->y += Xreg->work[1];
	}
	//AnmActor((AnmData*)HappaAnmSeqTbl);
}

//----------------------------------------------------------
//	�Ђ�Ђ�Ɨt���ς������Ă��铮��  ���v�C����
// work[0] = �b(X)
// work[1] = ���̊֐��ɓ����Ă���ٰ�߉񐔂��Ă���    // �b(Y)
//----------------------------------------------------------
static void HappaHira(actWork *Xreg)
{
	if(MineEnemyCheck(AttackNo))
		Xreg->dx = -SinMove(Xreg->work[0], H_FURIHABA);
	else
		Xreg->dx = SinMove(Xreg->work[0], H_FURIHABA);
		//Xreg->dy = SinMove(Xreg->work[1], 30);

	Xreg->work[0] = 0xff & (Xreg->work[0] + 2);
	Xreg->work[1]++;	// += 2;

	if((Xreg->work[1] & 1) == 0)	//���������߰�ނ𒲐�
	{
		Xreg->dy++;
	}

	if(Xreg->work[1] > 80)	//120)
		DelWazaEffect(Xreg);
}


//=========================================================================
//	���ł����J�b�^�[
//=========================================================================
static void HappaCutterMain(actWork *xreg);
static void HappaCutterInit(actWork *xreg);

#define HAPPACUTER_ANMWAIT	3
static const actAnm HappaCutterAnm0[]={
	{OBJ16_16x32 * 0, HAPPACUTER_ANMWAIT, 0, 0},
	{OBJ16_16x32 * 0, HAPPACUTER_ANMWAIT, 1, 0},
	{OBJ16_16x32 * 0, HAPPACUTER_ANMWAIT, 1, 1},
	{OBJ16_16x32 * 0, HAPPACUTER_ANMWAIT, 0, 1},
	{ACT_ANMLOOP, 0, 0, 0},
};

static const actAnm *const HappaCutterAnmTbl[]={
		HappaCutterAnm0,
};
const	actHeader ActHappaCutterHeader={
	WAZACHR_ID_HAPPACUTTER,
	WAZACHR_ID_HAPPACUTTER,
	&WazaOamDataNormal[6],
	HappaCutterAnmTbl,
	0,
	DummyActAffTbl,
	HappaCutterInit,
};

//-- �X�s�[�h�X�^�[ --//
static const actAffAnm SpeedStarAffAnm0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0,0, 0,1},
	{ACT_AFLOOP,0,0,0},
};
static const	actAffAnm * const SpeedStarAffTbl[] = {
	SpeedStarAffAnm0,
};
const	actHeader SpeedStarHeader={
	WAZACHR_ID_SPEEDSTAR,
	WAZACHR_ID_SPEEDSTAR,
	&WazaOamDataAffine[2],
	DummyActAnmTbl,
	0,
	SpeedStarAffTbl,
	HappaCutterInit,
};

//----------------------------------------------------------
//	offset	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset	4:Sync��
//			5:Y�U�蕝	6:0=�ʏ�  1=2�̑I��
//----------------------------------------------------------
void HappaCutterInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];
	xreg->work[0] = WazaEffWork[4];

	if(WazaEffWork[6] == 0){
		xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X) + WazaEffWork[2];
		xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y) + WazaEffWork[3];
	}
	else{
		GetCenterEffPos(DefenceNo, PM_MODE, &xreg->work[2], &xreg->work[4]);
		xreg->work[2] += WazaEffWork[2];
		xreg->work[4] += WazaEffWork[3];
	}
	
	xreg->work[5] = WazaEffWork[5];
	YamanariMoveInit(xreg);
	if(MineEnemyCheck(AttackNo) == MineEnemyCheck(DefenceNo))
		xreg->work[0] = 1;
	else
		xreg->work[0] = 0;
	xreg->move = HappaCutterMain;
}

static void HappaCutterMain(actWork *xreg)
{
	s16 temp, before,after;
	u8 end = 0;
	
	temp = xreg->work[0];
	before = xreg->work[7];
	xreg->work[0] = 1;
	YamanariMoveMain(xreg);
	after = xreg->work[7];
	xreg->work[0] = temp;
	
	if(before > 200 && after < 56 && xreg->oamData.AffineParam==0){	//���]�`�F�b�N
		xreg->oamData.AffineParam++;
	}
	if(xreg->oamData.AffineParam && xreg->work[0]){
		xreg->banish ^= 1;	//���Ɉ��]���Ă���̂œ_�ł����ď���
		xreg->oamData.AffineParam++;
		if(xreg->oamData.AffineParam == 30)
			end = 1;	//����Del
	}
	if(xreg->x+xreg->dx > 240+16 || xreg->x+xreg->dx < -16 ||
			xreg->y+xreg->dy > 160 || xreg->y+xreg->dy < -16)
	{
		end = 1;
	}
	
	if(end)
		DelWazaEffect(xreg);
}




//=========================================================================
//
//		���܂�
//												by mori 2002.08.01(��)
//=========================================================================
static void SeqWazaTatumakiMain(actWork *Xreg);

//--------------------------------------------------------------------
//
//  ���ɕ����؂̗t�i�������͏��΁j
//
// [0]:��������	
// [1]:�����玝������������L�[�v���鍂��(0xff�͍������L�[�v���Ȃ�) 
// [2]:��]���鑬�� [3]:��]�̕� [4]:�ړ����͂��߂�^�C�~���O
//--------------------------------------------------------------------
void SeqWazaTatumakiInit(actWork *Xreg)
{
	if(ContestCheck()){
		//
	}else{
		if(FightCheck()==1){							//2x2�̎�
			GetCenterEffPos(DefenceNo, PM_MODE, &(Xreg->x), &(Xreg->y));
		}
	}
	Xreg->y += 32;
	Xreg->work[0] = WazaEffWork[0];			//��������
	Xreg->work[1] = WazaEffWork[1];			//�L�[�v���鍂��
	Xreg->work[2] = WazaEffWork[2];			//��]���鑬��
	Xreg->work[3] = WazaEffWork[3];			//��]�̕�
	Xreg->work[4] = WazaEffWork[4];			//�ړ����n�߂�^�C�~���O
	Xreg->move = SeqWazaTatumakiMain;
}

#define TATUMAKI_TATEHABA	5
static void SeqWazaTatumakiMain(actWork *Xreg)
{
	if(Xreg->work[1]==0xff){
		Xreg->y-=2;
	}else{
		if(Xreg->work[1] > 0){
			Xreg->y-=2;
			Xreg->work[1]-=2;
		}
	}
	Xreg->work[5] += Xreg->work[2];
	if(Xreg->work[0] < Xreg->work[4]) Xreg->work[5] += Xreg->work[2];
	Xreg->work[5] &= 0xff;
	Xreg->dx = CosMove(Xreg->work[5],Xreg->work[3]);
	Xreg->dy = SinMove(Xreg->work[5],TATUMAKI_TATEHABA);
	
	if(Xreg->work[5] < 128){
		Xreg->oamData.Priority = GetPokeBGPri(DefenceNo)-1;
	}else{
		Xreg->oamData.Priority = GetPokeBGPri(DefenceNo)+1;
	}
	if(--Xreg->work[0]==0)
		DelWazaEffect(Xreg);
}








//=========================================================================
//
//		����݂�
//												by matsuda 2002.05.01(��)
//=========================================================================
static void KaramitukuInit(actWork *xreg);
static void KaramitukuWait(actWork *xreg);
static void KaramitukuMain(actWork *xreg);

#define KARAMITUKU_ANMWAIT	4
static const actAnm KaramitukuAnm0[]={
	{OBJ16_64x32 * 0, KARAMITUKU_ANMWAIT, 0, 0},
	{OBJ16_64x32 * 1, KARAMITUKU_ANMWAIT, 0, 0},
	{OBJ16_64x32 * 2, KARAMITUKU_ANMWAIT, 0, 0},
	{OBJ16_64x32 * 3, KARAMITUKU_ANMWAIT, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm KaramitukuAnm1[]={
	{OBJ16_64x32 * 0, KARAMITUKU_ANMWAIT, 1, 0},
	{OBJ16_64x32 * 1, KARAMITUKU_ANMWAIT, 1, 0},
	{OBJ16_64x32 * 2, KARAMITUKU_ANMWAIT, 1, 0},
	{OBJ16_64x32 * 3, KARAMITUKU_ANMWAIT, 1, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm *const KaramitukuAnmTbl[]={
	KaramitukuAnm0,
	KaramitukuAnm1,
};

#define KARAMITUKU_AFF		(-11)
#define KARAMITUKU_AFFLOOP	6
static const actAffAnm KaramitukuAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0x0100, 0x0100, 0, ACT_AFSET},
	{KARAMITUKU_AFF, 0, 0, KARAMITUKU_AFFLOOP},
	{-KARAMITUKU_AFF, 0, 0, KARAMITUKU_AFFLOOP},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm KaramitukuAff1[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{-0x0100, 0x0100, 0, ACT_AFSET},
	{-KARAMITUKU_AFF, 0, 0, KARAMITUKU_AFFLOOP},
	{KARAMITUKU_AFF, 0, 0, KARAMITUKU_AFFLOOP},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const KaramitukuAffTbl[] = {
	KaramitukuAff0,
	KaramitukuAff1,
};

const	actHeader KaramitukuHeader={
	WAZACHR_ID_KARAMITUKU,
	WAZACHR_ID_KARAMITUKU,
	&WazaOamDataAffine[7],
	KaramitukuAnmTbl,
	0,
	KaramitukuAffTbl,
	KaramitukuInit,
};

//----------------------------------------------------------
//	offset	0:Xoffset	1:Yoffset	2:��҃p�^�[��	3:�i�߂�Affine��
//----------------------------------------------------------
static void KaramitukuInit(actWork *xreg)
{
	WazaDefaultPosSet1(xreg, EFF_MODE);
	xreg->aff_pause = 1;
//	ActAnmChg(xreg, WazaEffWork[2]);
	ActAffAnmChg(xreg, WazaEffWork[2]);
	xreg->work[6] = WazaEffWork[2];
	xreg->work[7] = WazaEffWork[3];
	xreg->move = KaramitukuWait;
}

static void KaramitukuWait(actWork *xreg)
{
	u8 poke;
	
	if((u16)WazaEffWork[7] == 0xffff){
		xreg->aff_pause = 0;
		poke = W_GetPokeActNo(1);
//		PokeKakusyukuInit(poke, 0);
		xreg->work[0] = 0x0100;
		xreg->move = KaramitukuMain;
	}
}

static void KaramitukuMain(actWork *xreg)
{
	u8 poke;
	
	poke = W_GetPokeActNo(1);
	
	if(xreg->work[2] == 0)
		xreg->work[0] -= KARAMITUKU_AFF;// << 8;
	else
		xreg->work[0] += KARAMITUKU_AFF;// << 8;
	xreg->work[1]++;
	if(xreg->work[1] == KARAMITUKU_AFFLOOP){
		xreg->work[1] = 0;
		xreg->work[2] ^= 1;
	}
	
//	PokeKakusyukuSet(poke, xreg->work[0], 0x0100, 0);
	
	if(xreg->affend_sw == 1){
		xreg->work[7]--;
		if(xreg->work[7] > 0){
			ActAffAnmChg(xreg, xreg->work[6]);
		}
		else{
//			PokeKakusyukuEnd(poke);
			DelWazaEffect(xreg);
		}
	}
}














//=========================================================================
//
//		���̂܂�
//
//												by matsuda 2002.06.04(��)
//=========================================================================
static void MonomaneMain(u8 id);
static void MonomaneEndWait(u8 id);

static void MonomaneInit(actWork *xreg);

static const actAffAnm MonomaneAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0,0,0,ACT_AFSET},
	{ 0x30,0x30,0,14},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm MonomaneAff1[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ -0x10,-0x10,0,1},
	{ACT_AFLOOP,0,0,0},
};
static const	actAffAnm * const MonomaneAffTbl[] = {
	MonomaneAff0,
	MonomaneAff1,
};
const	actHeader MonomaneHeader={
	WAZACHR_ID_SOLAR,
	WAZACHR_ID_SOLAR,
	&WazaOamDataAffine2[1],
	&SolarAnmTbl[8],
	0,
	MonomaneAffTbl,
	MonomaneInit,
};

//----------------------------------------------------------
// ��WazaEffWork[7]��0xffff�������Ă����瑊���߹�݂����ɖ߂��ďI��
//	0:�ړ����x(����4�ޯď���)	1:SyncNum
//----------------------------------------------------------
#define MONOMANE_ADDAFF		0x10
void MonomaneTask(u8 id)
{
	u8 poke;
	
	poke = W_GetPokeActNo(1);
	
	if(ActWork[poke].banish){	//add 2002.09.20(��) by matsuda
		DelTaskEffect(id);		//����ԁAۯ��݂Ȃǂő��肪���Ȃ��ꍇ�͏I��
		return;
	}
	
	PokeKakusyukuInit(poke, 1);	//�����߹�ݔ�����&�g�kON
	
	TaskTable[id].work[0] = WazaEffWork[0];
	TaskTable[id].work[1] = WazaEffWork[1];
	TaskTable[id].work[11] = 0x0100;	//affine�����l
	TaskTable[id].TaskAdrs = MonomaneMain;
}

static void MonomaneMain(u8 id)
{
	u8 poke;
	
	poke = W_GetPokeActNo(1);
	TaskTable[id].work[10] += TaskTable[id].work[0];
	ActWork[poke].dx = TaskTable[id].work[10] >> 8;
	if(MineEnemyCheck(DefenceNo))
		ActWork[poke].dx = -ActWork[poke].dx;
	
	TaskTable[id].work[11] += MONOMANE_ADDAFF;
	PokeKakusyukuSet(poke, TaskTable[id].work[11],TaskTable[id].work[11], 0);
//	W_AdjustKaitenYPos(poke);
	PokeKakusyukuYHosei(poke);
	
	if(--TaskTable[id].work[1] == 0){
		TaskTable[id].work[0] = 0;
		TaskTable[id].TaskAdrs = MonomaneEndWait;
	}
}

//-- WazaEffWork[7]��0xffff�������Ă����瑊���߹�݂����ɖ߂��ďI�� --//
static void MonomaneEndWait(u8 id)
{
	u8 poke;
	
	if((u16)WazaEffWork[7] == 0xffff){
		if(TaskTable[id].work[0] == 0){
			poke = W_GetPokeActNo(1);
			PokeKakusyukuEnd(poke);		//�����߹�݂����ɖ߂�
			ActWork[poke].dx = 0;
			ActWork[poke].dy = 0;
			TaskTable[id].work[0]++;
			return;
		}
	}
	
	if(TaskTable[id].work[0]){
		if(TaskTable[id].work[0]++ == 2){
			DelTaskEffect(id);
		}
	}
}


//----------------------------------------------------------
//	0:Xoffset	1:Yoffset
//----------------------------------------------------------
#define MONOMANE_MOVESYNC	25
static void MonomaneInit(actWork *xreg)
{
	switch(xreg->work[0]){
		case 0:
//			WazaDefaultPosSet1(xreg, EFF_MODE);
			if(MineEnemyCheck(DefenceNo) == SIDE_MINE)
				WazaEffWork[0] *= -1;
			xreg->x = GetWazaEffPos(DefenceNo, EFF_X) + WazaEffWork[0];
			xreg->y = GetWazaEffPos(DefenceNo, EFF_Y) + WazaEffWork[1];
			
			xreg->banish = 1;	//��ýĂň�u�������̂�1Sync��Ư��
			xreg->work[0]++;
			break;
		case 1:
			xreg->banish = 0;
			if(xreg->affend_sw){
				ActAffAnmHoldChg(xreg,1);
				xreg->work[0] = MONOMANE_MOVESYNC;
				xreg->work[2] = GetWazaEffPos(AttackNo, PM_X);
				xreg->work[4] = GetWazaEffPos(AttackNo, PM_Y);
				xreg->move = W_StraightSyncMove4;
				SetWorkSeq(xreg, DelWazaEffect);
			}
			break;
	}
}















#define ANM_WAIT2	(7)
//=========================================================================
//
//		�˂��͂�
//												by mori 2002.06.04
//=========================================================================
static void SeqWazaNekkoInit(actWork *Xreg);
static void SeqWazaNekkoInit2(actWork *Xreg);
static void SeqWazaNekkoMain(actWork *Xreg);

/*-----------�������A�N�^�[-----------------*/

static const actAnm NeAnm0[]={
	{OBJ16_32x32 * 0, ANM_WAIT2, 0, 0},
	{OBJ16_32x32 * 1, ANM_WAIT2, 0, 0},
	{OBJ16_32x32 * 2, ANM_WAIT2, 0, 0},
	{OBJ16_32x32 * 3, ANM_WAIT2, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm NeAnm1[]={
	{OBJ16_32x32 * 0, ANM_WAIT2, 1, 0},
	{OBJ16_32x32 * 1, ANM_WAIT2, 1, 0},
	{OBJ16_32x32 * 2, ANM_WAIT2, 1, 0},
	{OBJ16_32x32 * 3, ANM_WAIT2, 1, 0},
	{ACT_ANMEND, 0, 0, 0},
};

static const actAnm NeAnm2[]={
	{OBJ16_32x32 * 0, ANM_WAIT2, 0, 0},
	{OBJ16_32x32 * 1, ANM_WAIT2, 0, 0},
	{OBJ16_32x32 * 2, ANM_WAIT2, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm NeAnm3[]={
	{OBJ16_32x32 * 0, ANM_WAIT2, 1, 0},
	{OBJ16_32x32 * 1, ANM_WAIT2, 1, 0},
	{OBJ16_32x32 * 2, ANM_WAIT2, 1, 0},
	{ACT_ANMEND, 0, 0, 0},
};

static const actAnm *const NeAnmTbl[]={
		NeAnm0,
		NeAnm1,
		NeAnm2,
		NeAnm3,
};

const	actHeader ActNeHeader={			//������
	WAZACHR_ID_NEKKO,
	WAZACHR_ID_NEKKO,
	&WazaOamDataNormal[2],
	NeAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaNekkoInit,
};

const	actHeader ActNe2Header={			//������
	WAZACHR_ID_NEKKO,
	WAZACHR_ID_NEKKO,
	&WazaOamDataNormal[2],
	NeAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaNekkoInit2,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------


//-------------------------------------------------------------
//  [0] = �w�I�t�Z�b�g	
//	[1] = �x�I�t�Z�b�g
//	[2] = �v���C�I���e�B
//	[3] = �A�j���[�V�����ԍ�
//	[4] = ������܂ł̂u�r��������
//-------------------------------------------------------------
static void SeqWazaNekkoInit(actWork *Xreg)
{
	if(Xreg->work[0]==0){
		Xreg->x = GetWazaEffPos(AttackNo, PM_X);
		Xreg->y = GetWazaEffPos(AttackNo, EFF_Y);
		Xreg->dx = WazaEffWork[0];
		Xreg->dy = WazaEffWork[1];
		Xreg->pri = WazaEffWork[2] + C_POKE1PRI;
		ActAnmChg(Xreg,WazaEffWork[3]);
		Xreg->work[2] = WazaEffWork[4];
		Xreg->work[0]++;
		if((Xreg->y+Xreg->dy) > 120) Xreg->y -= 120-(Xreg->y+Xreg->dy);
	}
	Xreg->move=SeqWazaNekkoMain;
}

static s16 WazaDebugWork[4];

//-------------------------------------------------------------
//  �u�n�[�h�v�����g�v�p�������A�N�^�[
//  WazaEffWork
//  [0] = ����Ǝ����̊Ԃ̈ʒu�֌W�̌W���i0-100�j
//  [1] = �Z�o���ꂽ�ꏊ����̂w�I�t�Z�b�g
//  [2] = �Z�o���ꂽ�ꏊ����̂x�I�t�Z�b�g
//	[3] = �v���C�I���e�B
//	[4] = �A�j���[�V�����ԍ�
//	[5] = ������܂ł̂u�r��������
//-------------------------------------------------------------
static void SeqWazaNekkoInit2(actWork *Xreg)
{
	s16 wx1,wy1,wx2,wy2,w,h;

	wx1 = GetWazaEffPos(AttackNo, PM_X);	//������W�̎擾
	wy1 = GetWazaEffPos(AttackNo, PM_Y);	
	wx2 = GetWazaEffPos(DefenceNo, PM_X);	//������W�̎擾
	wy2 = GetWazaEffPos(DefenceNo, PM_Y);	

	w = wx2 - wx1;
	h = wy2 - wy1;
	

	Xreg->x       = wx1 + ( w * WazaEffWork[0]) / 100;
	Xreg->y       = wy1 + ( h * WazaEffWork[0]) / 100;
	Xreg->dx      = WazaEffWork[1];
	Xreg->dy      = WazaEffWork[2];
	Xreg->pri     = WazaEffWork[3] + C_POKE1PRI;
	ActAnmChg( Xreg, WazaEffWork[4] );
	Xreg->work[2] = WazaEffWork[5];
	Xreg->move    = SeqWazaNekkoMain;


	WazaDebugWork[0] = Xreg->x;
	WazaDebugWork[1] = Xreg->y;
	WazaDebugWork[2] = w;
	WazaDebugWork[3] = h;
}

static void SeqWazaNekkoMain(actWork *Xreg)
{
	if(++Xreg->work[0] > (Xreg->work[2]-10)){
		Xreg->banish = Xreg->work[0]%2;
	}
	if(Xreg->work[0] > Xreg->work[2]){
		DelWazaEffect(Xreg);
	}
}


static void SeqNewoHaruInit(actWork *Xreg);

/*-----------�z��������-----------------*/
static const actAnm NeBallAnm0[]={
	{OBJ16_8x8 * 3, 3, 0, 0},
	{OBJ16_8x8 * 0, 5, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};

static const actAnm *const NeBallAnmTbl[]={
		NeBallAnm0,
};

const	actHeader ActNewoHauHeader={	//��
	WAZACHR_ID_SOLAR,
	WAZACHR_ID_SOLAR,
	&WazaOamDataNormal[0],
	NeBallAnmTbl,
	0,
	DummyActAffTbl,
	SeqNewoHaruInit,
};


//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
//  [0] = �w�I�t�Z�b�g
//	[1] = �x�I�t�Z�b�g
//	[2] = ����
//	[3] = �U��
//	[4] = ������܂ł̂u�r��������
//-------------------------------------------------------------
static void SeqNewoHaruInit(actWork *Xreg)
{
	if(Xreg->work[0]==0){
		Xreg->x = GetWazaEffPos(AttackNo, PM_X) + WazaEffWork[0];
		Xreg->y = GetWazaEffPos(AttackNo, EFF_Y) + WazaEffWork[1];

		Xreg->work[1] = WazaEffWork[2];		//work[1]�͌���
		Xreg->work[2] = WazaEffWork[3];		//work[2]=�U��
		Xreg->work[3] = WazaEffWork[4];		//work[3]=�I���J�E���g
	}
	Xreg->work[0]++;
	
	Xreg->dx = Xreg->work[0]*Xreg->work[1];	//���ړ�
	Xreg->dy = SinMove((Xreg->work[0]*20)&0xff,Xreg->work[2]);
	
	if(Xreg->work[0] > Xreg->work[3]){
		DelWazaEffect(Xreg);
	}
}



//=========================================================================
//
//		�v���[���g
//												by mori 2002.06.04
//=========================================================================
/*-----------�v���[���g�A�N�^�[-----------------*/
static void BoundFukuroActInit(actWork *p);
static void FukuroActMove(actWork *Xreg);
static void OutFukuroActInit(actWork *p);

static const actAnm FukuroAnm0[]={
	{OBJ16_32x32 * 0, 30, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm *const FukuroAnmTbl[]={
		FukuroAnm0,
};



static const actAffAnm	FukuroAff0[]={
	{	0,	0,	-4,	10},
	{	0,	0,	 4,	20},
	{	0,	0,	-4,	10},
	{ACT_AFEND,0,0,0},
};

static const actAffAnm	FukuroAff1[]={
	{0,0,-1,2},
	{0,0, 1,4},
	{0,0,-1,4},
	{0,0, 1,4},
	{0,0,-1,4},
	{0,0, 1,2},
	{ACT_AFEND,0,0,0},
};

static const actAffAnm *const FukuroAffTbl[]={
	FukuroAff0,
	FukuroAff1,
};

const	actHeader ActFukuroHeader={			//�ӂ���
	WAZACHR_ID_FUKURO3,
	WAZACHR_ID_FUKURO3,
	&WazaOamDataAffine[2],
	FukuroAnmTbl,
	0,
	FukuroAffTbl,
	BoundFukuroActInit,
};

const actHeader ActOutFukuroHeader={
	WAZACHR_ID_FUKURO3,
	WAZACHR_ID_FUKURO3,
	&WazaOamDataAffine[2],
	FukuroAnmTbl,
	0,
	FukuroAffTbl,
	OutFukuroActInit
};
enum{
	TANE_HOZONY=0,
	TANE_MOVEX,
	TANE_MOVEY,
	TANE_FIRSTPOWER,
	TANE_POWER,
	TANE_BOUND,
	TANE_MUKI,
	TANE_MUKI2,
};


// =================================================================
// StraghtMoveInit
// �T�v  : �����ړ����鏉����
// ����  : p : �A�N�^�[���[�N
//			p->x , p->y �����ʒu
//			p->work[6],p->work[7] :�I�_�ʒu
//			p->work[5],�J�E���^
// �߂�l: none
// =================================================================
void StraghtMoveInit(actWork *p,u8 max)
{
	s32 work1,work2,work3;
	work1 = (p->x<<8) | p->y;
	work2 = (p->work[6] << 8) | p->work[7];
	work3 = max << 8;

	p->work[5] = work1;
	p->work[6] = work2;
	p->work[7] = work3;
}



// =================================================================
// StraghtMove
// �T�v  : �܂������I�_��MAXSync�񐔂ňړ�����
// ����  : p
// �߂�l: u8
// =================================================================
u8 StraghtMove(actWork *p)
{
	s32 wx1,wy1,wx2,wy2;
	s16 count,max,mx,my;
	s32 ww,rr;
	wx1 = (u8)(p->work[5] >> 8);
	wy1 = (u8)(p->work[5] & 0xff);
	wx2 = (u8)(p->work[6] >> 8);
	wy2 = (u8)(p->work[6] & 0xff);
	max = p->work[7] >> 8;
	count   = p->work[7] & 0xff;
	if(wx2==0  ) wx2 = -32;
	else if(wx2==255) wx2 = 240+32;

	mx = (s16)(wx2 - wx1);
	my = (s16)(wy2 - wy1);

	ww = (mx*count)/max;
	rr = (my*count)/max;
	
	p->x = wx1 + ww;
	p->y = wy1 + rr;

	if((++count)==max){
		return 1;
	}
	p->work[7] = count | (max<<8);
	return 0;

}


static void FukuroActMove(actWork *Xreg)
{
	if(Xreg->work[0]==10)
		ActAffAnmChg(Xreg,	1);	//�u���u������
		
	if(++Xreg->work[0]>50){
		DelWazaEffect(Xreg);
	}

}


//===============================================================================
//  �o�E���h����ӂ���̃A�N�^�[����֐�
//===============================================================================
static void BoundFukuroActMove2(actWork *Xreg)
{
	Xreg->work[0]+=(128*Xreg->work[3])/(Xreg->work[4]);
	if(Xreg->work[0]>=128){
		Xreg->work[1]++;
		Xreg->work[0] = 0;	
	}
	Xreg->dy = SinMove(Xreg->work[0]+128,30-(Xreg->work[1]*8));
	if( StraghtMove(Xreg)){
		Xreg->dy = 0;
		Xreg->work[0] = 0;
		Xreg->move = FukuroActMove;
	}
}


//-------------------------------------------------------------------------------
//  �o�E���h����ӂ���̃Z�b�g�֐�
//	
//-------------------------------------------------------------------------------
//
static void BoundFukuroActInit(actWork *p)
{
	u8 wx,wy;
	WazaDefaultPosSet2(p,EFF_MODE);

	wx = GetWazaEffPos(DefenceNo, EFF_X);	//������W�̎擾
	wy = GetWazaEffPos(DefenceNo, EFF_Y);	
	if((AttackNo^2) == DefenceNo){			//���ԍU��
		p->work[6] = wx;	//������W�̎擾
		p->work[7] = wy+10;	
		StraghtMoveInit(p,60);
		p->work[3] = 1;
		p->work[4] = 60;
		p->move = BoundFukuroActMove2;
	}else{									//�G�ɍU��
		p->work[6] = wx;	//������W�̎擾
		p->work[7] = wy+10;	
		StraghtMoveInit(p,60);
		p->work[3] = 3;
		p->work[4] = 60;
		p->move = BoundFukuroActMove2;
	}
}

//===============================================================================
//  �o�E���h���ĉ�ʊO�ɂł�ӂ���
//===============================================================================
static void BoundFukuroActMove3(actWork *Xreg)
{
	Xreg->work[0]+=(128*Xreg->work[3])/(Xreg->work[4]);
	if(Xreg->work[0]>=128){
		Xreg->work[1]++;
		Xreg->work[0] = 0;	
	}
	Xreg->dy = SinMove(Xreg->work[0]+128,30-(Xreg->work[1]*8));
	if( StraghtMove(Xreg)){
		Xreg->dy = 0;
		Xreg->work[0] = 0;
		DelWazaEffect(Xreg);
	}
}


//-------------------------------------------------------------------------------
//  �o�E���h���ĉ�ʊO�ɂł�ӂ���Z�b�g�֐�
//	
//	
//	
//-------------------------------------------------------------------------------
//
static void OutFukuroActInit(actWork *p)
{
	u8 wy;
//	WazaDefaultPosSet2(p,EFF_MODE);

//	wx = GetWazaEffPos(DefenceNo, EFF_X);	//������W�̎擾
	wy = GetWazaEffPos(DefenceNo, EFF_Y);	
	if(MineEnemyCheck(DefenceNo) == SIDE_MINE){		//���Ԃ������͎�����
		p->work[6] = 0;					//������W�̎擾
		p->work[7] = wy+10;	
		StraghtMoveInit(p,40);
		p->work[3] = 3;
		p->work[4] = 60;
		p->move = BoundFukuroActMove2;
	}else{									//�G�ɍU��
		p->work[6] = 255;					//������W�̎擾
		p->work[7] = wy+10;	
		if(ContestCheck()) p->work[6] = 0;
		StraghtMoveInit(p,40);
		p->work[3] = 3;
		p->work[4] = 60;
		p->move = BoundFukuroActMove3;
	}
}


//-------------------------------------------------------------------------------
//  �v���[���g�E�E�E�񕜂̏ꍇ
//-------------------------------------------------------------------------------
static void PresentOkActInit(actWork *act);


static const actAnm PresentOkAnm[]={
	{OBJ16_16x16*0, 4,0,0},
	{OBJ16_16x16*1, 4,0,0},
	{OBJ16_16x16*2, 4,0,0},
	{OBJ16_16x16*3, 4,0,0},
	{ACT_ANMEND,0,0,0}
};

static const actAnm *const PresentOkAnmTbl[]={
	PresentOkAnm,
};

static void PresentOkActInit(actWork *act);

const actHeader ActPresentOkHeader = {
	WAZACHR_ID_MOONLIGHT,
	WAZACHR_ID_MOONLIGHT,
	&WazaOamDataNormal[1],
	PresentOkAnmTbl,
	0,
	DummyActAffTbl,
	PresentOkActInit,
};

static void PresentOkActInit(actWork *act)
{
	if(act->work[0]==0){
		WazaDefaultPosSet1(act,EFF_MODE);
		act->work[1] = WazaEffWork[2];
	}

	act->work[0]++;
	act->dy = act->work[0]*act->work[1];

	if(act->anmend_sw==1){
		DelWazaEffect(act);
	}	
}




//=========================================================================
//
//		�u�ق�����v
//													by taya 2002.07.09
//=========================================================================
static void HoshigaruFukuroActInit(actWork *p);

const	actHeader ActHoshigaruFukuroHeader={			//�ӂ���i�ق�����p�j
	WAZACHR_ID_FUKURO3,
	WAZACHR_ID_FUKURO3,
	&WazaOamDataAffine[2],
	FukuroAnmTbl,
	0,
	FukuroAffTbl,
	HoshigaruFukuroActInit,
};
static void HosigaruFukuroActMove(actWork *Xreg);

static void HoshigaruFukuroActInit(actWork *p)
{
	u8 wx,wy;
//	WazaDefaultPosSet2(p,EFF_MODE);
	WazaDefaultPosSet1(p,EFF_MODE);

	wx = GetWazaEffPos(AttackNo, EFF_X);	//������W�̎擾
	wy = GetWazaEffPos(AttackNo, EFF_Y);	
	if((DefenceNo^2) == AttackNo){			//���ԍU��
		p->work[6] = wx;	//������W�̎擾
		p->work[7] = wy+10;	
		StraghtMoveInit(p,60);
		p->work[3] = 1;
		p->work[4] = 60;
		p->move = HosigaruFukuroActMove;
	}else{									//�G�ɍU��
		p->work[6] = wx;	//������W�̎擾
		p->work[7] = wy+10;	
		StraghtMoveInit(p,60);
		p->work[3] = 3;
		p->work[4] = 60;
		p->move = HosigaruFukuroActMove;
	}
}
static void HosigaruFukuroActMove(actWork *Xreg)
{
	Xreg->work[0]+=(128*Xreg->work[3])/(Xreg->work[4]);
	if(Xreg->work[0]>=128){
		Xreg->work[1]++;
		Xreg->work[0] = 0;	
	}
	Xreg->dy = SinMove(Xreg->work[0]+128,30-(Xreg->work[1]*8));
	if(Xreg->dy == 0)
	{
//		SePlayPan(SE_W166, GetWazaEffSePan(WAZA_SE_R));
		SePlayPan(SE_W145B, GetWazaEffSePan(WAZA_SE_R));
	}
	if( StraghtMove(Xreg)){
		Xreg->dy = 0;
		Xreg->work[0] = 0;
		Xreg->move = FukuroActMove;
		SePlayPan(SE_W145B, GetWazaEffSePan(WAZA_SE_L));
	}
}

//=========================================================================
//
//		�g���b�N
//												by mori 2002.06.04
//=========================================================================
static void TrickFukuroActInit(actWork *p);
static void TrickFukuroActMain0(actWork *Xreg);
static void TrickFukuroActMain1(actWork *Xreg);
static void TrickFukuroActMain2(actWork *Xreg);


static const actAffAnm	FukuroAff2[]={
	{	0,	0,	 0,	 3},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm	FukuroAff3[]={
	{	0,	-10, 0,	 3},
	{	0,	-6,	 0,	 3},
	{	0,	-2,	 0,	 3},
	{	0,	 0,	 0,	 3},
	{	0,	 2,	 0,	 3},
	{	0,	 6,	 0,	 3},
	{	0,	10,	 0,	 3},
	{ACT_AFEND,0,0,0},
};


static const actAffAnm *const TrickFukuroAffTbl[]={
	FukuroAff2,
	FukuroAff3,
	FukuroAff0,
	FukuroAff1,
};

const	actHeader ActTrickFukuroHeader={			//�ӂ���
	WAZACHR_ID_FUKURO3,
	WAZACHR_ID_FUKURO3,
	&WazaOamDataAffine[2],
	FukuroAnmTbl,
	0,
	TrickFukuroAffTbl,
	TrickFukuroActInit,
};

#define SHUFUL_END	(127)

static const s8 shufultbl[][3]={
	{128/24, 24, 1},		
	{    0, 4, 0},
	{128/16, 16, -1},		
	{    0, 2, 0},
	{128/16, 16, 1},		
	{    0, 2, 0},
	{128/16, 16, 1},		
	{    0, 2, 0},
	{128/16, 16, 1},		
	{    0, 16, 0},
	{	 0, 0, SHUFUL_END},
};

//---------------------------------------------------------------------------
//	�A�N�^�[�V�[�P���X
//  WazaEffWork[0] = ����		[1]=�����p�x
//---------------------------------------------------------------------------
static void TrickFukuroActInit(actWork *Xreg)
{
	if(Xreg->work[0]==0){
		if(ContestCheck()==0){				//�ʏ�퓬
			Xreg->work[1] = WazaEffWork[1];
			Xreg->x = 120;
		}else{
			Xreg->work[1] = (WazaEffWork[1]-32)%256;
			Xreg->x = 70;
		}
			Xreg->y = WazaEffWork[0];
			Xreg->work[2] = WazaEffWork[0];
			Xreg->work[4] = 20;
			Xreg->dx  = CosMove(Xreg->work[1],60);
			Xreg->dy  = SinMove(Xreg->work[1],20);
			Xreg->move = TrickFukuroActMain0;
			if(Xreg->work[1]>0&&Xreg->work[1]<192)		Xreg->pri = C_POKE1PRI +1;
			else										Xreg->pri = C_POKE1PRI -1;

	}
}


static void TrickFukuroActMain0(actWork *Xreg)
{
	switch(Xreg->work[3]){
		case 0:
			if(Xreg->work[2]>78){
				Xreg->work[3]=1;
				ActAffAnmChg(Xreg,1);
			}else{
				Xreg->work[2]+=Xreg->work[4]/10;
				Xreg->work[4]+=3;
				Xreg->y = Xreg->work[2];
			}
			break;
		case 1:
			if(Xreg->work[3] && Xreg->affend_sw){
				Xreg->work[0] = 0;
				Xreg->work[2] = 0;
				Xreg->move = TrickFukuroActMain1;
			}
			break;
	}

}


/*------------------------------------------------------------------*/
/*  ����ւ���]�V�[�P���X											*/
/*	work[0]=�|�C���g*/
/*	work[1]=�v�Z�p�p�x���[�N*/
/*	work[2]=�J�E���^*/
/*------------------------------------------------------------------*/
static void TrickFukuroActMain1(actWork *Xreg)
{

	if(Xreg->work[2]==shufultbl[Xreg->work[0]][1]){
		if(shufultbl[Xreg->work[0]][2]==SHUFUL_END){
			Xreg->work[0] = 0;
			Xreg->move    = TrickFukuroActMain2;
		}
		Xreg->work[2] = 0;
		Xreg->work[0]++;
	}else{
		Xreg->work[2]++;
		Xreg->work[1]  += shufultbl[Xreg->work[0]][0]*shufultbl[Xreg->work[0]][2];
		Xreg->work[1]  &= 0xff;
		if(ContestCheck()==0){				//�ʏ�퓬
			if(Xreg->work[1]>0&&Xreg->work[1]<192)		Xreg->pri = C_POKE1PRI +1;
			else										Xreg->pri = C_POKE1PRI -1;
		}
		Xreg->dx  = CosMove(Xreg->work[1],60);
		Xreg->dy  = SinMove(Xreg->work[1],20);
	}

}

static void TrickFukuroActMain2(actWork *Xreg)
{
	if(Xreg->work[0]>20){
		DelWazaEffect(Xreg);
	}
	Xreg->banish = Xreg->work[0]%2;
	Xreg->work[0]++;
}












//=========================================================================
//
//		���[�t�u���[�h
//													by taya 2002.07.27
//=========================================================================
#define LEAFBLADE_MOVE_SYNC  (10)
#define LEAFBLADE_OVER_WIDTH  (10)  // �߹�݂̒[���W����͂ݏo����
#define LEAFBLADE_YAMAHABA   (8)

static const actHeader LeafBladeHeader;
static void TaskLeafBladeMain(u8 id);
static void add_leafblade_zanzou(TASK_TABLE *my, u8 id);
static s16 leafblade_calc_yamahaba(actWork *act);

enum LEAFBLADE_WORK {
	LBW_SEQ,
	LBW_TIMER,
	LBW_ACTNO,
	LBW_ANMNO,
	LBW_ACTPRI,
	LBW_VEC,
	LBW_CX,
	LBW_CY,
	LBW_SX,
	LBW_SY,
	LBW_POKEWIDTH,
	LBW_POKEHEIGHT,
	LBW_ZANZOU_CNT,
	LBW_ZANZOU_NUM,
	LBW_ZANZOU_TIMER,
	LBW_NEXTSEQ,
};

void TaskLeafBlade(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	my->work[LBW_ACTPRI] = GetPokeSoftPri(DefenceNo) - 1;

	my->work[LBW_CX] = GetWazaEffPos(DefenceNo, PM_X);
	my->work[LBW_CY] = GetWazaEffPos(DefenceNo, PM_Y);

	my->work[LBW_POKEWIDTH] = W_GetPokeSize(DefenceNo, POKESIZE_WIDTH);
	my->work[LBW_POKEHEIGHT] = W_GetPokeSize(DefenceNo, POKESIZE_HEIGHT);

	my->work[LBW_VEC] = (MineEnemyCheck(DefenceNo)==SIDE_ENEMY)? 1 : -1;

	my->work[LBW_SY] = 56 - (64 * my->work[LBW_VEC]);
	my->work[LBW_SX] = my->work[LBW_CX] + (my->work[LBW_CY] - my->work[LBW_SY]);

	my->work[LBW_ACTNO] = AddActor(&LeafBladeHeader, my->work[LBW_SX], my->work[LBW_SY], my->work[LBW_ACTPRI]);
	if(my->work[LBW_ACTNO] == ACT_MAX)
	{
		DelTaskEffect(id);
	}

	ActWork[my->work[LBW_ACTNO]].work[0] = LEAFBLADE_MOVE_SYNC;
	ActWork[my->work[LBW_ACTNO]].work[1] = my->work[LBW_SX];
	ActWork[my->work[LBW_ACTNO]].work[2] = my->work[LBW_CX] - ((my->work[LBW_POKEWIDTH]/2)+LEAFBLADE_OVER_WIDTH) * my->work[LBW_VEC];
	ActWork[my->work[LBW_ACTNO]].work[3] = my->work[LBW_SY];
	ActWork[my->work[LBW_ACTNO]].work[4] = my->work[LBW_CY] + ((my->work[LBW_POKEHEIGHT]/2)+LEAFBLADE_OVER_WIDTH) * my->work[LBW_VEC];
	ActWork[my->work[LBW_ACTNO]].work[5] = leafblade_calc_yamahaba(&ActWork[my->work[LBW_ACTNO]]);

	YamanariMoveInit(&ActWork[my->work[LBW_ACTNO]]);

	my->TaskAdrs = TaskLeafBladeMain;
}
static void TaskLeafBladeMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];
	actWork *act = &ActWork[my->work[LBW_ACTNO]];

	switch(my->work[0]){
	// �O���P�\
	case 0:
		add_leafblade_zanzou(my, id);
		if(YamanariMoveMain(act)){
			my->work[LBW_NEXTSEQ] = 1;
			my->work[LBW_SEQ] = 0xff;
		}
		break;

	// �O���P�������Ұ����
	case 1:
		act->x += act->dx;
		act->y += act->dy;
		act->dx = 0;
		act->dy = 0;

		act->work[0] = LEAFBLADE_MOVE_SYNC;
		act->work[1] = act->x;
		act->work[2] = my->work[LBW_CX];
		act->work[3] = act->y;
		act->work[4] = my->work[LBW_CY];
		act->work[5] = leafblade_calc_yamahaba(act);
		my->work[LBW_ACTPRI] += 2;
		my->work[LBW_ANMNO] = 1;

		act->pri = my->work[LBW_ACTPRI];
		ActAnmChg(act, my->work[LBW_ANMNO]);

		YamanariMoveInit(act);

		my->work[LBW_SEQ]++;
		break;

	// �O���P��
	case 2:
		add_leafblade_zanzou(my, id);
		if(YamanariMoveMain(act)){
			my->work[LBW_NEXTSEQ] = 3;
			my->work[LBW_SEQ] = 0xff;
		}
		break;

	// �O��2-1�����Ұ����
	case 3:
		act->x += act->dx;
		act->y += act->dy;
		act->dx = 0;
		act->dy = 0;

		act->work[0] = LEAFBLADE_MOVE_SYNC;
		act->work[1] = act->x;
		act->work[2] = my->work[LBW_CX] - ((my->work[LBW_POKEWIDTH]/2)+LEAFBLADE_OVER_WIDTH) * my->work[LBW_VEC];
		act->work[3] = act->y;
		act->work[4] = my->work[LBW_CY] - ((my->work[LBW_POKEHEIGHT]/2)+LEAFBLADE_OVER_WIDTH) * my->work[LBW_VEC];
		act->work[5] = leafblade_calc_yamahaba(act);
		my->work[LBW_ANMNO] = 2;

		act->pri = my->work[LBW_ACTPRI];
		ActAnmChg(act, my->work[LBW_ANMNO]);
		YamanariMoveInit(act);

		my->work[LBW_SEQ]++;
		break;

	// �O��2-1
	case 4:
		add_leafblade_zanzou(my, id);
		if(YamanariMoveMain(act)){
			my->work[LBW_NEXTSEQ] = 5;
			my->work[LBW_SEQ] = 0xff;
		}
		break;

	// �O��2-2�����Ұ����
	case 5:
		act->x += act->dx;
		act->y += act->dy;
		act->dx = 0;
		act->dy = 0;

		act->work[0] = LEAFBLADE_MOVE_SYNC;
		act->work[1] = act->x;
		act->work[2] = my->work[LBW_CX] + ((my->work[LBW_POKEWIDTH]/2)+LEAFBLADE_OVER_WIDTH) * my->work[LBW_VEC];
		act->work[3] = act->y;
		act->work[4] = my->work[LBW_CY] + ((my->work[LBW_POKEHEIGHT]/2)+LEAFBLADE_OVER_WIDTH) * my->work[LBW_VEC];
		act->work[5] = leafblade_calc_yamahaba(act);
		my->work[LBW_ACTPRI] -= 2;
		my->work[LBW_ANMNO] = 3;

		act->pri = my->work[LBW_ACTPRI];
		ActAnmChg(act, my->work[LBW_ANMNO]);
		YamanariMoveInit(act);

		my->work[LBW_SEQ]++;
		break;

	// �O��2-2
	case 6:
		add_leafblade_zanzou(my, id);
		if(YamanariMoveMain(act)){
			my->work[LBW_NEXTSEQ] = 7;
			my->work[LBW_SEQ] = 0xff;
		}
		break;

	// �O��2-3�����Ұ����
	case 7:
		act->x += act->dx;
		act->y += act->dy;
		act->dx = 0;
		act->dy = 0;

		act->work[0] = LEAFBLADE_MOVE_SYNC;
		act->work[1] = act->x;
		act->work[2] = my->work[LBW_CX];
		act->work[3] = act->y;
		act->work[4] = my->work[LBW_CY];
		act->work[5] = leafblade_calc_yamahaba(act);
		my->work[LBW_ACTPRI] += 2;
		my->work[LBW_ANMNO] = 4;

		act->pri = my->work[LBW_ACTPRI];
		ActAnmChg(act, my->work[LBW_ANMNO]);
		YamanariMoveInit(act);

		my->work[LBW_SEQ]++;
		break;

	// �O��2-3
	case 8:
		add_leafblade_zanzou(my, id);
		if(YamanariMoveMain(act)){
			my->work[LBW_NEXTSEQ] = 9;
			my->work[LBW_SEQ] = 0xff;
		}
		break;

	// �O��3-1�����Ұ����
	case 9:
		act->x += act->dx;
		act->y += act->dy;
		act->dx = 0;
		act->dy = 0;

		act->work[0] = LEAFBLADE_MOVE_SYNC;
		act->work[1] = act->x;
		act->work[2] = my->work[LBW_CX] - ((my->work[LBW_POKEWIDTH]/2)+LEAFBLADE_OVER_WIDTH) * my->work[LBW_VEC];
		act->work[3] = act->y;
		act->work[4] = my->work[LBW_CY] + ((my->work[LBW_POKEHEIGHT]/2)+LEAFBLADE_OVER_WIDTH) * my->work[LBW_VEC];
		act->work[5] = leafblade_calc_yamahaba(act);
		my->work[LBW_ANMNO] = 5;

		act->pri = my->work[LBW_ACTPRI];
		ActAnmChg(act, my->work[LBW_ANMNO]);
		YamanariMoveInit(act);

		my->work[LBW_SEQ]++;
		break;

	// �O��3-1
	case 10:
		add_leafblade_zanzou(my, id);
		if(YamanariMoveMain(act)){
			my->work[LBW_NEXTSEQ] = 11;
			my->work[LBW_SEQ] = 0xff;
		}
		break;

	// �O��3-2�����Ұ����
	case 11:
		act->x += act->dx;
		act->y += act->dy;
		act->dx = 0;
		act->dy = 0;

		act->work[0] = LEAFBLADE_MOVE_SYNC;
		act->work[1] = act->x;
		act->work[2] = my->work[LBW_SX];
		act->work[3] = act->y;
		act->work[4] = my->work[LBW_SY];
		act->work[5] = leafblade_calc_yamahaba(act);
		my->work[LBW_ACTPRI] -= 2;
		my->work[LBW_ANMNO] = 6;

		act->pri = my->work[LBW_ACTPRI];
		ActAnmChg(act, my->work[LBW_ANMNO]);
		YamanariMoveInit(act);

		my->work[LBW_SEQ]++;
		break;

	// �O��3-2
	case 12:
		add_leafblade_zanzou(my, id);
		if(YamanariMoveMain(act)){
			DelActor(act);
			my->work[LBW_SEQ]++;
		}
		break;

	case 13:
		if(my->work[LBW_ZANZOU_CNT] == 0){
			DelTaskEffect(id);
		}
		break;

	case 0xff:  // wait
		if(++(my->work[LBW_TIMER]) >= 6){
			my->work[LBW_TIMER] = 0;
			my->work[LBW_SEQ] = my->work[LBW_NEXTSEQ];
		}
		break;
	}
}
//===========================================
// ��܂Ȃ�̂ӂ蕝�v�Z
//===========================================
static s16 leafblade_calc_yamahaba(actWork *act)
{
	s16 haba = LEAFBLADE_YAMAHABA;

	// ��ړ��Ȃ牺�����̌ʂ�`��
	if(act->work[4] < act->y){
		haba *= -1;
	}

	return haba;

}
//===========================================
// �c�������ǉ�
//===========================================
static void LBZanzouActMove(actWork *act);

static void add_leafblade_zanzou(TASK_TABLE *my, u8 id)
{
	if(++(my->work[LBW_ZANZOU_TIMER]) > 0){
		s16 x, y;
		u8 no;

		my->work[LBW_ZANZOU_TIMER] = 0;

		x = ActWork[my->work[LBW_ACTNO]].x + ActWork[my->work[LBW_ACTNO]].dx;
		y = ActWork[my->work[LBW_ACTNO]].y + ActWork[my->work[LBW_ACTNO]].dy;

		no = AddActor(&LeafBladeHeader, x, y, my->work[LBW_ACTPRI]);
		if(no != ACT_MAX)
		{
			ActWork[no].work[6] = id;
			ActWork[no].work[7] = LBW_ZANZOU_CNT;
			TaskTable[id].work[LBW_ZANZOU_CNT]++;
			ActWork[no].work[0] = (my->work[LBW_ZANZOU_NUM] & 1);
			TaskTable[id].work[LBW_ZANZOU_NUM]++;
			ActAnmChg(&ActWork[no], my->work[LBW_ANMNO]);
			ActWork[no].pri = my->work[LBW_ACTPRI];
			ActWork[no].move = LBZanzouActMove;
		}
	}
}
static void LBZanzouActMove(actWork *act)
{
	if(++(act->work[0]) > 1){
		act->work[0] = 0;
		act->banish ^= 1;
		if(++(act->work[1]) > 8){
			TaskTable[act->work[6]].work[act->work[7]]--;
			DelActor(act);
		}
	}
}
//===========================================
// �����ް�
//===========================================
static const actAnm LeafBladeAnm[]={
	{OBJ16_16x16 * 7, 1, 0, 0}, { ACT_ANMEND, 0, 0, 0 },
	{OBJ16_16x16 * 8, 1, 0, 0}, { ACT_ANMEND, 0, 0, 0 },
	{OBJ16_16x16 * 5, 1, 0, 0}, { ACT_ANMEND, 0, 0, 0 },
	{OBJ16_16x16 * 7, 1, 1, 0}, { ACT_ANMEND, 0, 0, 0 },
	{OBJ16_16x16 * 4, 1, 0, 0}, { ACT_ANMEND, 0, 0, 0 },
	{OBJ16_16x16 * 4, 1, 1, 0}, { ACT_ANMEND, 0, 0, 0 },
	{OBJ16_16x16 * 7, 1, 0, 0}, { ACT_ANMEND, 0, 0, 0 },
};

static const actAnm *const LeafBladeAnmTbl[]={
	LeafBladeAnm + 0,
	LeafBladeAnm + 2,
	LeafBladeAnm + 4,
	LeafBladeAnm + 6,
	LeafBladeAnm + 8,
	LeafBladeAnm + 10,
	LeafBladeAnm + 12,
};
static const actHeader LeafBladeHeader={
	WAZACHR_ID_HAPPA,
	WAZACHR_ID_HAPPA,
	&WazaOamDataNormal[1],
	LeafBladeAnmTbl,
	0,
	DummyActAffTbl,
	DummyActMove,
};


//=========================================================================
//
//		312:�A���}�Z���s�[
//		318:���񂢂�̂���
//												by iwasawa 2002.07.29	add
//														   2002.07.30
//=========================================================================
static void SeqAromaTherapyMain(actWork *xreg);
//-------------------------------------------------------------------------
//	0:����Y���W,1:V��,2:�����p�x,3:���x,4:�p���x,
//	5:�v���C�I���e�B(0:AromaUp,1:AromaDown,2:SilverUp,3:SilverDown)
//	6:�N���C�A���g(0:Attack,1:Defence)
//-------------------------------------------------------------------------
#define AromaR		0
#define AromaW		1
#define AromaV		2
#define AromaRV		3
#define AromaDir	4
#define AromaCnt	7
static void SeqAromaTherapyInit(actWork *xreg)
{
	u8	no = 0;
	u8	client = 0;

	//Client�̈ʒu�ɂ���āA�E���炩�����炩���߂�
	if(WazaEffWork[6] == 0) client = AttackNo;
	else					client = DefenceNo;
	
	if(MineEnemyCheck(client)){
		xreg->work[AromaDir] = 0;
		xreg->work[AromaV]	= WazaEffWork[3];
		xreg->x = -16;
	}else{
		xreg->work[AromaDir] = 1;
		xreg->work[AromaV]	= -WazaEffWork[3];
		xreg->x = 256;
	}

	xreg->work[AromaW] = WazaEffWork[1];
	xreg->work[AromaR]	= WazaEffWork[2];
	xreg->work[AromaRV]	= WazaEffWork[4];

	switch(WazaEffWork[5])
	{
	case 0:		//AromaTherapy��UP
		xreg->y = WazaEffWork[0];
		xreg->oamData.Priority = GetPokeBGPri(client);
		break;
	case 1:		//AromaTherapy��Down
		xreg->y = WazaEffWork[0];
		xreg->oamData.Priority = GetPokeBGPri(client)+1;
		break;
	case 2:		//SilverWind��Up
		xreg->y = GetWazaEffPos(client,PM_Y)+WazaEffWork[0];
		xreg->oamData.Priority = GetPokeBGPri(client);
		break;
	case 3:		//SilverWind��Down
		xreg->y = GetWazaEffPos(DefenceNo,PM_Y)+WazaEffWork[0];
		no = W_GetPokeActNo(1);	
		xreg->oamData.Priority = GetPokeBGPri(client)+1;
		break;
	}
	xreg->move = SeqAromaTherapyMain;
}
static void SeqAromaTherapyMain(actWork *xreg)
{   
	s32 temp = (s32)xreg->work[AromaCnt];
	
	xreg->work[AromaCnt]++;
	
	xreg->dy = (xreg->work[AromaW]*_Sin(xreg->work[AromaR]) >> 8);
	xreg->dx = (s16)(temp*xreg->work[AromaV]); 
	xreg->work[AromaR] = (s16)((temp*xreg->work[AromaRV])&0xFF);
	
	//��ʊO�֔�����܂�
	if(xreg->work[AromaDir] == 0){
		if(xreg->dx+xreg->x < 248) return;
	}else{
		if(xreg->dx+xreg->x > -16) return;
	}
	//�g�k�I�u�W�F�Ȃ�AAffineWorkNumDel���Ă�ł���ADelWazaEffect���Ă�
	W_AffCheckEndSeq(xreg);
}

//�Ԃт�̕��̉Ԃт�L�����N�^�g���܂킵
static const actAffAnm	AromaAffAnm00[] = {
	{0x0100,0x0100,0,ACT_AFSET},
	{0x0000,0x0000,4,1},
	{ACT_AFLOOP,1,0,0},
};
static const actAffAnm*	const AromaAffAnmTbl[] = {
	AromaAffAnm00,
};
const actHeader AromaTherapyHeader = {
	WAZACHR_ID_HANA,
	WAZACHR_ID_HANA,
	&WazaOamDataNormal[0],
	&HanaAnmTbl[1],
	0,
	DummyActAffTbl,
	SeqAromaTherapyInit,
};
const actHeader AromaTherapyHeader2 = {
	WAZACHR_ID_HANA,
	WAZACHR_ID_HANA,
	&WazaOamDataAffine[1],
	HanaAnmTbl,
	0,
	AromaAffAnmTbl,
	SeqAromaTherapyInit,
};
//==========================================================================
//	���񂢂�̂���
//		�A���}�Z���s�[�̃V�[�P���X���g���܂킷
//
//==========================================================================
static const actAffAnm	SilverWindAffAnm00[] = {
	{0x0100,0x0100,0,ACT_AFSET},
	{0x0000,0x0000,-10,1},
	{ACT_AFLOOP,1,0,0},
};
static const actAffAnm	SilverWindAffAnm01[] = {
	{0x00C0,0x00C0,0,ACT_AFSET},
	{0x0000,0x0000,-12,1},
	{ACT_AFLOOP,1,0,0},
};
static const actAffAnm	SilverWindAffAnm02[] = {
	{0x008F,0x008F,0,ACT_AFSET},
	{0x0000,0x0000,-15,1},
	{ACT_AFLOOP,1,0,0},
};
static const actAffAnm*	const SilverWindAffAnmTbl[] = {
	SilverWindAffAnm00,
	SilverWindAffAnm01,
	SilverWindAffAnm02,
};
const actHeader SilverWindHeader0 = {
	WAZACHR_ID_SILVERWIND,
	WAZACHR_ID_SILVERWIND,
	&WazaOamDataAffine[1],
	DummyActAnmTbl,
	0,
	SilverWindAffAnmTbl,
	SeqAromaTherapyInit,
};
const actHeader SilverWindHeader1 = {
	WAZACHR_ID_SILVERWIND,
	WAZACHR_ID_SILVERWIND,
	&WazaOamDataAffine[1],
	DummyActAnmTbl,
	0,
	&(SilverWindAffAnmTbl[1]),
	SeqAromaTherapyInit,
};
const actHeader SilverWindHeader2 = {
	WAZACHR_ID_SILVERWIND,
	WAZACHR_ID_SILVERWIND,
	&WazaOamDataAffine[1],
	DummyActAnmTbl,
	0,
	&(SilverWindAffAnmTbl[2]),
	SeqAromaTherapyInit,
};













//=========================================================================
//
//		�}�W�J�����[�t
//												by matsuda 2002.08.02(��)
//=========================================================================
static const u16 MagicalColorTbl[] = {
	0x001f, 0x027f, 0x03ff, 0x03e0, 0x7dc5, 0x7d56, 0x7eb6
};
//----------------------------------------------------------
//	�t���ς�̪��ނ�F�X�ȐF�ōs�Ȃ�
//		��WazaEffWork[7]��0xffff�������Ă�����I��
// offset �Ȃ�
//----------------------------------------------------------
#define MAGICAL_COLOR_WAIT		0
#define MAGICAL_COLOR_TBL_MAX	7
void MagicalReefColorTask(u8 id)
{
	TASK_TABLE *task;
	
	task = &TaskTable[id];
	switch(task->work[0]){
		case 0:
			task->work[8] = ObjPalNumGet(WAZACHR_ID_HAPPA) * 16 + 16*16;
			task->work[12] = ObjPalNumGet(WAZACHR_ID_HAPPACUTTER) * 16 + 16*16;
			task->work[0]++;
			break;
		case 1:
			if(++task->work[9] < MAGICAL_COLOR_WAIT)
				break;
			task->work[9] = 0;
			
			SoftFade(task->work[8], 16, task->work[10],MagicalColorTbl[task->work[11]]);
			SoftFade(task->work[12],16,task->work[10],MagicalColorTbl[task->work[11]]);
			task->work[10]++;
			if(task->work[10] == 17){
				task->work[10] = 0;
				task->work[11]++;
				if(task->work[11] == MAGICAL_COLOR_TBL_MAX)
					task->work[11] = 0;
			}
			break;
	}
	
	if(WazaEffWork[7] == (s16)0xffff)
		DelTaskEffect(id);
}









//=========================================================================
//
//	�j�[�h���A�[��
//														taya
//=========================================================================
static void NeedleArmActMove(actWork *act);

//==================================================
// 0:(0:�U����  1:�h�䑤)  1:(0:�W�܂�  1:��юU��)
// 2:xofs 3:yofs 4:sync
//==================================================
static void NeedleArmActInit(actWork *act)
{
	s16 ax, ay;
	u16 kakudo;

	if(WazaEffWork[4] == 0){
		DelWazaEffect(act);
		return;
	}

	if(WazaEffWork[0]==0){
		ax = GetWazaEffPos(AttackNo,PM_X);
		ay = GetWazaEffPos(AttackNo,PM_Y);
	}else{
		ax = GetWazaEffPos(DefenceNo,PM_X);
		ay = GetWazaEffPos(DefenceNo,PM_Y);
	}

	act->work[0] = WazaEffWork[4];  // 0:�ړ�sync
	if(WazaEffWork[1]==0)
	{
		act->x = ax + WazaEffWork[2];
		act->y = ay + WazaEffWork[3];
		act->work[5] = ax;             // 5:�ŏIX
		act->work[6] = ay;             // 6:�ŏIY
	}else{
		act->x = ax;
		act->y = ay;
		act->work[5] = ax + WazaEffWork[2];
		act->work[6] = ay + WazaEffWork[3];
	}

	act->work[1] = (act->x << 4);  // 1:�Œ菭��X
	act->work[2] = (act->y << 4);  // 2:�Œ菭��Y

	act->work[3] = ((act->work[5]-act->x)<<4) / WazaEffWork[4];  // 3:X���x
	act->work[4] = ((act->work[6]-act->y)<<4) / WazaEffWork[4];  // 4:Y���x

	kakudo = GetKakudo2(act->work[5]-act->x, act->work[6]-act->y);

	if(ContestCheck()){
		kakudo -= 0x8000;
	}
//	kakudo -= 0x4000;	//���̊G���c�Ȃ̂Ő^���ɂ���̵ׂ̾��
	KakusyukuSet(act, 0, 0x0100, 0x0100, kakudo);


	act->move = NeedleArmActMove;
}
static void NeedleArmActMove(actWork *act)
{
	if(act->work[0]){
		act->work[1] += act->work[3];
		act->work[2] += act->work[4];
		act->x = act->work[1] >> 4;
		act->y = act->work[2] >> 4;
		act->work[0]--;
	}else{
		W_AffCheckEndSeq(act);
	}
}

const actHeader NeedleArmHeader = {
	WAZACHR_ID_NEEDLEARM,
	WAZACHR_ID_NEEDLEARM,
	&WazaOamDataAffine[1],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	NeedleArmActInit,
};


