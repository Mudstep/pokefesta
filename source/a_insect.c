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
#include "waza_eff.h"
#include "server.h"
#include "calctool.h"
#include "bss.h"






//=========================================================================
//
//		���K�z�[��
//												by matsuda 2002.02.05(��)
//=========================================================================
static void MegaHoonInit(actWork *xreg);

#define MEGAHOON_DEFAULT_KAKUDO		30
static const actAffAnm MegaHoonAffAnm0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0x100, 0x100, MEGAHOON_DEFAULT_KAKUDO, ACT_AFSET},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm MegaHoonAffAnm1[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0x100, 0x100, 127 + MEGAHOON_DEFAULT_KAKUDO, ACT_AFSET},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm MegaHoonAffAnm2[] = {	//��ýėp
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0x100, 0x100, 64 + MEGAHOON_DEFAULT_KAKUDO, ACT_AFSET},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const MegaHoonAffTbl[] = {
	MegaHoonAffAnm0,
	MegaHoonAffAnm1,
	MegaHoonAffAnm2,
};
const	actHeader ActMegaHoonHeader={
	WAZACHR_ID_MEGAHOON,
	WAZACHR_ID_MEGAHOON,
	&WazaOamDataAffine2[6],
	DummyActAnmTbl,
	0,
	MegaHoonAffTbl,
	MegaHoonInit,
};

//-------------------------------------------------------------
//  0:Xoffset 1:Yoffset 2:�I�_Xoffset	3:�I�_Yoffset	4:Sync��
//-------------------------------------------------------------
static void MegaHoonInit(actWork *xreg)
{
//	if(MineEnemyCheck(AttackNo))	2002.09.04(��)
	if(ContestCheck()){
		ActAffAnmChg(xreg, 2);
		WazaEffWork[2] = -WazaEffWork[2];
		WazaEffWork[0] = -WazaEffWork[0];
	}
	else if(MineEnemyCheck(DefenceNo) == SIDE_MINE)
	{
		ActAffAnmChg(xreg, 1);
		WazaEffWork[1] = -WazaEffWork[1];
		WazaEffWork[2] = -WazaEffWork[2];
		WazaEffWork[3] = -WazaEffWork[3];
		WazaEffWork[0] = -WazaEffWork[0];
	}
	//WazaDefaultPosSet1(xreg, PM_MODE);
	xreg->x = GetWazaEffPos2(DefenceNo, PM_X) + WazaEffWork[0];
	xreg->y = GetWazaEffPos2(DefenceNo, PM_Y) + WazaEffWork[1];
	xreg->work[0] = WazaEffWork[4];
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X) + WazaEffWork[2];
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y) + WazaEffWork[3];
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}










//=========================================================================
//
//		���イ����
//												by matsuda 2002.02.15(��)
//=========================================================================
static void KyuuketuInit(actWork *xreg);

#if 1
static const actAffAnm KyuuketuAff0[] = {	//�����U����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0, 0, 255-32, 1},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm KyuuketuAff1[] = {	//�G�U����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0, 0, 255-32-127, 1},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm KyuuketuAff2[] = {	//��ý�
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0, 0, 255-32-127+64, 1},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const KyuuketuAffTbl[] = {
	KyuuketuAff0,
	KyuuketuAff1,
	KyuuketuAff2,
};
const	actHeader ActKyuuketuHeader={
	WAZACHR_ID_HARI2,
	WAZACHR_ID_HARI2,
	&WazaOamDataAffine[1],
	DummyActAnmTbl,
	0,
	KyuuketuAffTbl,
	KyuuketuInit,
};
#else
const	actHeader ActKyuuketuHeader={
	WAZACHR_ID_HARI2,
	WAZACHR_ID_HARI2,
	&WazaOamDataNormal[1],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	KyuuketuInit,
};
#endif

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:Sync��
//----------------------------------------------------------
static void KyuuketuInit(actWork *xreg)
{
	if(ContestCheck()){
		WazaEffWork[0] = -WazaEffWork[0];
		ActAffAnmChg(xreg, 2);
//		xreg->oamData.AffineParamNo |= 0x10;	//V�د��ON
	}
	else if(MineEnemyCheck(DefenceNo) == SIDE_MINE)
	{
		WazaEffWork[1] = -WazaEffWork[1];
		WazaEffWork[0] = -WazaEffWork[0];
	}
	xreg->x = GetWazaEffPos2(DefenceNo, PM_X) + WazaEffWork[0];
	xreg->y = GetWazaEffPos2(DefenceNo, PM_Y) + WazaEffWork[1];
	xreg->work[0] = WazaEffWork[2];
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}









//=========================================================================
//
//		���Ƃ��͂�
//												by matsuda 2002.03.27(��)
//=========================================================================
static void ItowoHakuInit(actWork *xreg);
static void ItowoHakuMain(actWork *xreg);

const	actHeader ItowoHakuHeader={
	WAZACHR_ID_ITODAMA,
	WAZACHR_ID_ITODAMA,
	&WazaOamDataNormal[0],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	ItowoHakuInit,
};

//----------------------------------------------------------
//	offset	0:Xoffset	1:Yoffset	2:X���x(�Œ菭��)	3:�U�蕝X
//		    4:0=�m�[�}�� 1=���W2�̑I�����[�h
//----------------------------------------------------------
#define ITOWOHAKU_ADD_SEC	13
static void ItowoHakuInit(actWork *xreg)
{
	if(ContestCheck())
		WazaEffWork[2] /= 2;	//��ýĂ̎��͑��x�𔼕���
		
	WazaDefaultPosSet2(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[2];
	xreg->work[1] = xreg->x;
	xreg->work[3] = xreg->y;
	if(WazaEffWork[4] == 0){
		xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
		xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	}
	else
		GetCenterEffPos(DefenceNo, PM_MODE, &xreg->work[2], &xreg->work[4]);
	//W_GetStraightSyncParam(xreg);
	W_GetStraightSpParam(xreg);
	xreg->work[5] = WazaEffWork[3];
	xreg->move = ItowoHakuMain;
}

static void ItowoHakuMain(actWork *xreg)
{
	if(W_ParallelSync(xreg)){
		DelWazaEffect(xreg);
		return;
	}
	
	xreg->dx += SinMove(xreg->work[6], xreg->work[5]);
	xreg->work[6] = 0xff & (xreg->work[6] + ITOWOHAKU_ADD_SEC);
}


//----------------------------------------------------------
//	���܂鎅
//	0:Xoffset	1:Yoffset
//----------------------------------------------------------
static void ItoSeq(actWork *xreg);
static void ItoMainSeq(actWork *xreg);

#define ITO_BANISH	2	//����_�ł����銴�o
#define ITO_ENDWAIT	50
const actHeader ItoHeader =
{
	WAZACHR_ID_ITO2,
	WAZACHR_ID_ITO2,
	&WazaOamDataNormal[7],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	ItoSeq,
};
static void ItoSeq(actWork *xreg)
{
//	WazaDefaultPosSet1(xreg, EFF_MODE);
	GetCenterEffPos(DefenceNo, EFF_MODE, &xreg->x, &xreg->y);
	if(MineEnemyCheck(AttackNo))
		xreg->x -= WazaEffWork[0];
	else
		xreg->x += WazaEffWork[0];
	xreg->y += WazaEffWork[1];
	
	if(MineEnemyCheck(DefenceNo) == SIDE_MINE)
		xreg->y += MINE_SIDE_OFFSET_Y;
	xreg->move = ItoMainSeq;
}

static void ItoMainSeq(actWork *xreg)
{
	if(xreg->work[0]++ == ITO_BANISH){
		xreg->work[0] = 0;
		xreg->banish ^= 1;
	}
	if(xreg->work[1]++ == ITO_ENDWAIT)
		DelWazaEffect(xreg);
}









//=========================================================================
//
//		�N���̂�
//												by matsuda 2002.03.27(��)
//=========================================================================
static void KumonosuInit(actWork *xreg);
static void KumonosuMain(actWork *xreg);
static void KumonosuEnd(actWork *xreg);

#define KUMONOSU_AFFADD	6
static const actAffAnm KumonosuAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0x10, 0x10, 0, ACT_AFSET},
	{KUMONOSU_AFFADD, KUMONOSU_AFFADD, 0, 1},
	{ACT_AFLOOP,1,0,0},
};
static const	actAffAnm * const KumonosuAffTbl[] = {
	KumonosuAff0,
};
const actHeader KumonosuHeader =
{
	WAZACHR_ID_KUMONOSU,
	WAZACHR_ID_KUMONOSU,
	&WazaOamDataBldAffine2[3],
	DummyActAnmTbl,
	ACT_NTRANS,
	KumonosuAffTbl,
	KumonosuInit,
};

//----------------------------------------------------------
//	offset �Ȃ�
//----------------------------------------------------------
static void KumonosuInit(actWork *xreg)
{
	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL;
	*(vu16*)REG_BLDALPHA = 16;
	xreg->work[0] = 16;
	xreg->move = KumonosuMain;
}

static void KumonosuMain(actWork *xreg)
{
	if(xreg->work[2] < 20){
		xreg->work[2]++;
		return;
	}

	if(xreg->work[1]++ & 1){
		xreg->work[0]--;
		*(vu16*)REG_BLDALPHA = xreg->work[0] | ((16-xreg->work[0]) << 8);
		if(xreg->work[0] == 0){
			xreg->banish = 1;
			xreg->move = KumonosuEnd;
		}
	}
}

static void KumonosuEnd(actWork *xreg)
{
	*(vu16*)REG_BLDCNT = 0;
	*(vu16*)REG_BLDALPHA = 0;
	DelWazaEffect(xreg);
}











//=========================================================================
//
//		�_�u���j�[�h��
//												by matsuda 2002.05.15(��)
//=========================================================================
static void DaburuNeedleInit(actWork *xreg);

const	actHeader DaburuNeedleHeader={
	WAZACHR_ID_HARI2,
	WAZACHR_ID_HARI2,
	&WazaOamDataAffine[1],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	DaburuNeedleInit,
};

//----------------------------------------------------------
//	offset	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset	4:SyncNum
//	(�I�_�Ɍ����Ă͓G�A�����ŵ̾�Ă̔��]���s��Ȃ�)
//----------------------------------------------------------
static void DaburuNeedleInit(actWork *xreg)
{
	u16 kakudo;
	s16 defence_x, defence_y;
	
//	WazaDefaultPosSet2(xreg, PM_MODE);
	
	if(ContestCheck()){
		WazaEffWork[2] = -WazaEffWork[2];
	}
	else if(MineEnemyCheck(AttackNo)){
		WazaEffWork[2] = -WazaEffWork[2];
		WazaEffWork[1] = -WazaEffWork[1];	//�n�_�A�I�_��Y���W�𔽓]
		WazaEffWork[3] = -WazaEffWork[3];
	}
	
	//�p�[�e�B�A�^�b�N���̏���
	if(ContestCheck() == 0 && MineEnemyCheck(AttackNo) == MineEnemyCheck(DefenceNo)){
		if(ClientTypeGet(DefenceNo) == BSS_CLIENT_MINE ||
				ClientTypeGet(DefenceNo) == BSS_CLIENT_ENEMY)
		{
			WazaEffWork[2] *= -1;
			WazaEffWork[0] *= -1;
		}
	}
	
	WazaDefaultPosSet2(xreg, PM_MODE);
	defence_x = GetWazaEffPos(DefenceNo, PM_X) + WazaEffWork[2];
	defence_y = GetWazaEffPos(DefenceNo, PM_Y) + WazaEffWork[3];

	kakudo = GetKakudo2(defence_x - xreg->x, defence_y - xreg->y);
	kakudo -= 0x4000;	//���̊G���c�Ȃ̂Ő^���ɂ���̵ׂ̾��
	KakusyukuSet(xreg, 0, 0x0100,0x0100, kakudo);
	
	xreg->work[0] = WazaEffWork[4];
	xreg->work[2] = defence_x;
	xreg->work[4] = defence_y;
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}










//=========================================================================
//
//		�~�T�C���΂�
//												by matsuda 2002.05.15(��)
//=========================================================================
static void MisairuBariInit(actWork *xreg);
static void MisairuBariMain(actWork *xreg);

const	actHeader MisairuBariHeader={
	WAZACHR_ID_HARI2,
	WAZACHR_ID_HARI2,
	&WazaOamDataAffine[1],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	MisairuBariInit,
};

//-- ���΂� --//
const	actHeader TuraraBariHeader={
	WAZACHR_ID_TURAHARI,
	WAZACHR_ID_TURAHARI,
	&WazaOamDataAffine[2],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	MisairuBariInit,
};

//----------------------------------------------------------
//	0:�n�_Xoffset	1:�n�_Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//	4:SyncNum	5:�U�蕝
//----------------------------------------------------------
static void MisairuBariInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];	//�n�_Y�𔽓]
	
	xreg->work[0] = WazaEffWork[4];
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X) + WazaEffWork[2];
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y) + WazaEffWork[3];
	xreg->work[5] = WazaEffWork[5];
	YamanariMoveInit(xreg);
	xreg->move = MisairuBariMain;
	xreg->banish = 1;
}

static void MisairuBariMain(actWork *xreg)
{
	s16 work[ACTOR_WORK_MAX];
	s16 xpos, ypos;
	int i;
	u16 kakudo;
	
	xreg->banish = 0;
	if(YamanariMoveMain(xreg)){
		DelWazaEffect(xreg);
		return;
	}
	
	//�p�x���擾����̂ɐi�s�����̵̾�Ă��K�v�Ȃ̂ŁA�����ł�����x�v�Z���A
	//����Sync�ňړ�����̾�Ă����߂�
	for(i = 0; i < ACTOR_WORK_MAX; i++)
		work[i] = xreg->work[i];
	xpos = xreg->x + xreg->dx;
	ypos = xreg->y + xreg->dy;
	if(YamanariMoveMain(xreg))
		return;
	kakudo = GetKakudo2(xreg->x+xreg->dx - xpos, xreg->y+xreg->dy - ypos);
	kakudo -= 0x4000;	//���̊G���c�Ȃ̂Ő^���ɂ���̵ׂ̾��
	KakusyukuSet(xreg, 0, 0x0100,0x0100, kakudo);
	//�p�x���擾���邽�߂Ɍv�Z�����̂�work�����ɖ߂�
	for(i = 0; i < ACTOR_WORK_MAX; i++)
		xreg->work[i] = work[i];
}





//=========================================================================
//
//		�ق����
//																	taya
//=========================================================================
static void HotarubiActInit(actWork *act);

static const actAffAnm HotarubiAffAnm[] = {
	{ 0x10, 0x10, 0, ACT_AFSET},
	{ 0x08, 0x08, 0, 18 },
	{ ACT_AFcLOOP, 0, 0, 0 },
	{ -0x05, -0x05, 0, 8 },
	{  0x05,  0x05, 0, 8 },
	{ ACT_AFcLOOP, 5, 0, 0 },
	{ACT_AFEND, 0, 0, 0 },
};
static const	actAffAnm * const HotarubiAffAnmTbl[] = {
	HotarubiAffAnm,
};

const actHeader ActHotarubi = {
	WAZACHR_ID_JUUDEN2,
	WAZACHR_ID_JUUDEN2,
	&WazaOamDataBldAffine[3],
	DummyActAnmTbl,
	0,
	HotarubiAffAnmTbl,
	HotarubiActInit,
};
//-------------------------------
// 0:(0:�U���� 1:�h�䑤)
//-------------------------------
static void HotarubiActInit(actWork *act)
{
	if(WazaEffWork[0] == 0){
		act->x = GetWazaEffPos(AttackNo, PM_X);
		act->y = GetWazaEffPos(AttackNo, PM_Y) + 18;
	}else{
		act->x = GetWazaEffPos(DefenceNo, PM_X);
		act->y = GetWazaEffPos(DefenceNo, PM_Y) + 18;
	}
	SetWorkSeq(act, W_AffCheckEndSeq);
	act->move = W_AffEndWait;
}

