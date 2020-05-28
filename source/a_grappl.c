//=========================================================================
//	
//	�i���n �Z�G�t�F�N�g
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

#include "task.h"
#include "bss.h"
#include "pm_aff.h"
#include "fight.h"






//=========================================================================
//
//		�u�g���v���L�b�N�v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void SeqWazaToripuruInit(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define DEL_WAIT	15

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
const	actHeader ActToripuruKikkuHeader={
	WAZACHR_ID_LEG,
	WAZACHR_ID_LEG,
	&WazaOamDataNormal[2],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaToripuruInit,
};


//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:X�̾��  1:Y�̾��
//-------------------------------------------------------------
static void SeqWazaToripuruInit(actWork *Xreg)
{
	EffXpositionSet(Xreg, WazaEffWork[0]);
	Xreg->y += WazaEffWork[1];
	
	Xreg->work[0] = DEL_WAIT;
	Xreg->move = W_WaitSeq;
	SetWorkSeq(Xreg, DelWazaEffect);
}















//=========================================================================
//
//		�i�� �����ړ��n ����
//												by matsuda 2002.02.12(��)
//=========================================================================
static void KakutouInitSeq(actWork *xreg);

static const actAnm KakutouAnm0[]={		//��
		{OBJ16_32x32*0,1,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm KakutouAnm1[]={		//�����̑�
		{OBJ16_32x32*1,1,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm KakutouAnm2[]={		//�����̑����̂Q
		{OBJ16_32x32*2,1,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm KakutouAnm3[]={		//�`���b�v
		{OBJ16_32x32*3,1,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm KakutouAnm4[]={		//�`���b�v�i���]�j
		{OBJ16_32x32*3,1,1,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm *const KakutouAnmTbl[]={
		KakutouAnm0,
		KakutouAnm1,
		KakutouAnm2,
		KakutouAnm3,
		KakutouAnm4,
};

const	actHeader ActKakutouHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataNormal[2],
	KakutouAnmTbl,
	0,
	DummyActAffTbl,
	KakutouInitSeq,
};

//----------------------------------------------------------
//	�w�肵���|�P�����̵̾�ĊԂŒ����ړ�����
//	0:�n�_Xoffset	1:�n�_Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//	4:Sync��	5:�U����(0)���h�䑤(1)��
//	6:�\������AnmNo
//	7:�G�����ɂ���Ďn�_�A�I�_��Y���W�ւ̵̾�Ă̔��]�ݵ�(0:���]���Ȃ�	1:���]����)
//----------------------------------------------------------
static void KakutouInitSeq(actWork *xreg)
{
	if(WazaEffWork[7] == 1)
	{
		if(MineEnemyCheck(AttackNo))
		{
			WazaEffWork[1] = -WazaEffWork[1];
			WazaEffWork[3] = -WazaEffWork[3];
		}
	}
	ActAnmChg(xreg, WazaEffWork[6]);
	WazaEffWork[6] = 0;
	W_OffsetSpaceMove(xreg);
}






//=========================================================================
//
//		�Ƃт���
//												by matsuda 2002.09.04(��)
//=========================================================================
static void TobigeriInitSeq(actWork *xreg);

const	actHeader TobigeriHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataNormal[2],
	KakutouAnmTbl,
	0,
	DummyActAffTbl,
	TobigeriInitSeq,
};

//----------------------------------------------------------
//	�w�肵���|�P�����̵̾�ĊԂŒ����ړ�����
//	0:�n�_Xoffset	1:�n�_Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//	4:Sync��	5:�U����(0)���h�䑤(1)��
//	6:�\������AnmNo
//	7:�G�����ɂ���Ďn�_�A�I�_��Y���W�ւ̵̾�Ă̔��]�ݵ�(0:���]���Ȃ�	1:���]����)
//----------------------------------------------------------
static void TobigeriInitSeq(actWork *xreg)
{
	if(ContestCheck()){
		WazaEffWork[1] = -WazaEffWork[1];
		WazaEffWork[3] = -WazaEffWork[3];
	}
	KakutouInitSeq(xreg);
}












//=========================================================================
//
//		�i�� �E�F�C�g�Î~�\�� ����
//												by matsuda 2002.02.13(��)
//=========================================================================
static void KakutouWaitInit(actWork *xreg);

const	actHeader ActKakutouWaitHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataNormal[2],
	KakutouAnmTbl,
	0,
	DummyActAffTbl,
	KakutouWaitInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:Del�܂ł̃E�F�C�g	3:0=�U���� 1=�h�䑤		4:���No
//----------------------------------------------------------
static void KakutouWaitInit(actWork *xreg)
{
	ActAnmChg(xreg, WazaEffWork[4]);
	if(WazaEffWork[3] == 0)
		WazaDefaultPosSet2(xreg, PM_MODE);
	else
		WazaDefaultPosSet1(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[2];
	xreg->move = W_WaitSeq;
	SetWorkSeq(xreg, DelWazaEffect);
}







//====================================================================
//  �i���q�b�g�}�[�N���A�����_���ʒu�ɕ\��
//  0:(0:�U����  1:�h�䑤)  1:Del�܂ł�wait  2:���No(ϲŽ�Ȃ������)
//====================================================================
extern const actHeader ActHitMarkHeader;
static void RandomKakutouActMove(actWork *act);

static void RandomKakutouActInit(actWork *act)
{
	s16 xofs, yofs, width, height;
	u8 client;

	client = (WazaEffWork[0]==0)? AttackNo : DefenceNo;

	if(WazaEffWork[2] < 0)
	{
		WazaEffWork[2] = pp_rand() % 5;
	}
	ActAnmChg(act, WazaEffWork[2]);

	act->x = GetWazaEffPos(client,PM_X);
	act->y = GetWazaEffPos(client,PM_Y);

	width = W_GetPokeSize(client, POKESIZE_WIDTH) / 2;
	height = W_GetPokeSize(client, POKESIZE_HEIGHT) / 4;

	xofs = pp_rand() % width;
	yofs = pp_rand() % height;

	if(pp_rand()&1){ xofs *= -1; }
	if(pp_rand()&1){ yofs *= -1; }

	//�N���C�A���g�����R���Ȃ�AY�ɵ̾�Ă�������_02.09.01_iwasawa
	if(!(ClientType[client]&1)) yofs -= 16;

	act->x += xofs;
	act->y += yofs;

	act->work[0] = WazaEffWork[1];
	act->work[7] = AddActor(&ActHitMarkHeader,act->x,act->y,act->pri+1);
	if(act->work[7] != ACT_MAX)
	{
		ActAffAnmChg(&ActWork[act->work[7]], 0);
		ActWork[act->work[7]].move = DummyActMove;
	}

	act->move = RandomKakutouActMove;

}
static void RandomKakutouActMove(actWork *act)
{
	if(act->work[0] == 0){
		if(act->work[7] !=ACT_MAX){
			AffineWorkNumDel(ActWork[act->work[7]].oamData.AffineParamNo);
			DelActor(&ActWork[act->work[7]]);
		}
		DelWazaEffect(act);
	}else{
		act->work[0]--;
	}
}

const actHeader RandomKakutouMarkHeader = {
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataNormal[2],
	KakutouAnmTbl,
	0,
	DummyActAffTbl,
	RandomKakutouActInit,
};

//=========================================================================
//
//		�N���X�`���b�v
//												by matsuda 2002.02.21(��)
//=========================================================================
static void KurosuTyoppuMain(actWork *xreg);
static void KurosuTyoppuInit(actWork *xreg);

const	actHeader ActKurosuTyoppuHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataNormal[2],
	&KakutouAnmTbl[3],
	0,
	DummyActAffTbl,
	KurosuTyoppuInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:����,�E��(0:��  1:�E)
//----------------------------------------------------------
#define KUROSU_UP_SYNC		30
#define KUROSU_UP_DX		20
#define KUROSU_UP_DY		20
#define KUROSU_WAIT			10
#define KUROSU_DOWN_SYNC	8
static void KurosuTyoppuInit(actWork *xreg)
{
	WazaDefaultPosSet1(xreg, PM_MODE);
	xreg->work[0] = KUROSU_UP_SYNC;
	if(WazaEffWork[2] == 0)
		xreg->work[2] = xreg->x - KUROSU_UP_DX;
	else
	{
		xreg->work[2] = xreg->x + KUROSU_UP_DX;
		xreg->Hflip = 1;
	}
	xreg->work[4] = xreg->y - KUROSU_UP_DY;
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, KurosuTyoppuMain);
}

static void KurosuTyoppuMain(actWork *xreg)
{
	if(xreg->work[5]++ == KUROSU_WAIT)
	{
		xreg->work[2] = xreg->x - xreg->dx;
		xreg->work[4] = xreg->y - xreg->dy;
		xreg->work[0] = KUROSU_DOWN_SYNC;
		xreg->x += xreg->dx;
		xreg->y += xreg->dy;
		xreg->dx = xreg->dy = 0;
		xreg->move = W_StraightSyncMove;
		SetWorkSeq(xreg, DelWazaEffect);
	}
}














//=========================================================================
//
//		�܂킵����		���Ƃ肠�����A���
//												by matsuda 2002.02.15(��)
//=========================================================================
static void MawasiGeriInit(actWork *xreg);
static void MawasiGeriMain(actWork *xreg);

const	actHeader ActMawasiGeriHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataNormal[2],
	&KakutouAnmTbl[1],
	0,
	DummyActAffTbl,
	MawasiGeriInit,
};

//-------------------------------------------------------------
// 0:X�̾��  1:Y�̾��	2:�I�_X�̾��	3:Sync��	4:Y�b���x(�Œ菭��)	5:�U�蕝Y
//-------------------------------------------------------------
static void MawasiGeriInit(actWork *xreg)
{
	// �����P�A�G�P�ւ̃p�[�e�B�A�^�b�N�͵̾�Ĕ��]
	if((AttackNo^2)==DefenceNo){
		u8 type = ClientTypeGet(DefenceNo);
		if(type==BSS_CLIENT_MINE || type==BSS_CLIENT_ENEMY){
			WazaEffWork[0] *= -1;
		}
	}
	WazaDefaultPosSet1(xreg, PM_MODE);

	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];

	xreg->work[0] = WazaEffWork[3];
	xreg->work[1] = xreg->x;
	xreg->work[2] = xreg->x + WazaEffWork[2];
	xreg->work[3] = xreg->y;
	xreg->work[4] = xreg->y;
	W_GetStraightSyncParam(xreg);
	xreg->work[5] = WazaEffWork[5];
	xreg->work[6] = WazaEffWork[4];
	xreg->work[7] = 0;	//�����p�x
	xreg->move = MawasiGeriMain;
}

static void MawasiGeriMain(actWork *xreg)
{
	if(W_ParallelSync(xreg) == 0)
	{
		xreg->dy += SinMove(xreg->work[7] >> 8, xreg->work[5]);
		xreg->work[7] += xreg->work[6];
//		xreg->work[7] = 0xff & (xreg->work[7] + xreg->work[6]);
	}
	else
		DelWazaEffect(xreg);
}













//=========================================================================
//
//		���K�g���p���`
//												by matsuda 2002.03.28(��)
//=========================================================================
static void MegatonPantiInit(actWork *xreg);
static void MegatonPantiEnd(actWork *xreg);

static const actAffAnm MegatonAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
//	{0x180, 0x180, 0, ACT_AFSET},
	{0x100, 0x100, 0, ACT_AFSET},
	{-0x8, -0x8, 20, 1},
	{ACT_AFLOOP,1,0,0},
};
static const	actAffAnm * const MegatonAffTbl[] = {
	MegatonAff0,
};
const	actHeader MegatonPantiHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataAffine2[2],
	KakutouAnmTbl,
	0,
	MegatonAffTbl,
	MegatonPantiInit,
};

static const actAffAnm MegatonAff2_0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0x100, 0x100, 0, ACT_AFSET},
	{-0x4, -0x4, 20, 1},
	{ACT_AFLOOP,1,0,0},
};
static const	actAffAnm * const MegatonAffTbl2[] = {
	MegatonAff2_0,
};
const	actHeader MegatonPantiHeader2={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataAffine2[2],
	KakutouAnmTbl,
	0,
	MegatonAffTbl2,
	MegatonPantiInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:AnmNo		3:Del�܂ł�Wait
//----------------------------------------------------------
#define MEGATON_DELWAIT		20//15
static void MegatonPantiInit(actWork *xreg)
{
	WazaDefaultPosSet1(xreg, PM_MODE);
	ActAnmChg(xreg, WazaEffWork[2]);
	xreg->work[0] = WazaEffWork[3];
	xreg->move = W_WaitSeq;
	SetWorkSeq(xreg, MegatonPantiEnd);
}

static void MegatonPantiEnd(actWork *xreg)
{
	ActAffAnmChg(xreg, 0);
	xreg->aff_pause = 1;
	xreg->work[0] = MEGATON_DELWAIT;
	xreg->move = W_WaitSeq;
	SetWorkSeq(xreg, DelWazaEffect);
}

















//=========================================================================
//
//		�ӂ݂�
//												by matsuda 2002.03.28(��)
//=========================================================================
static void FumitukeInit(actWork *xreg);
static void FumitukeWait(actWork *xreg);
static void FumitukeEndWait(actWork *xreg);

const	actHeader FumitukeHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataNormal[2],
	&KakutouAnmTbl[1],
	0,
	DummyActAffTbl,
	FumitukeInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:����J�n�܂ł̃E�F�C�g
//----------------------------------------------------------
#define FUMITUKE_SYNC_MOVE	6
#define FUMITUKE_END_WAIT	15
static void FumitukeInit(actWork *xreg)
{
	WazaDefaultPosSet1(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[2];
	xreg->move = FumitukeWait;
}

static void FumitukeWait(actWork *xreg)
{
	if(xreg->work[0]-- == 0){
		xreg->work[0] = FUMITUKE_SYNC_MOVE;
		xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
		xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
		xreg->move = W_StraightSyncMove;
		SetWorkSeq(xreg, FumitukeEndWait);
	}
}

static void FumitukeEndWait(actWork *xreg)
{
	xreg->work[0] = FUMITUKE_END_WAIT;
	xreg->move = W_WaitSeq;
	SetWorkSeq(xreg, DelWazaEffect);
}















//=========================================================================
//
//		�s���s���p���`
//												by matsuda 2002.05.23(��)
//=========================================================================

//=========================================================================
//	�͂�����q���R
//=========================================================================
static void PiyoHiyokoInit(actWork *xreg);

const	actHeader PiyoHiyokoHeader={
	WAZACHR_ID_HIYOKO,
	WAZACHR_ID_HIYOKO,
	&WazaOamDataNormal[1],
	&DummyActAnmTbl[0],
	0,
	DummyActAffTbl,
	PiyoHiyokoInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�ړ����x(�Œ菭��)	3:�U�蕝Y
//----------------------------------------------------------
#define PIYOHIYOKO_ADDSEC	3
static void PiyoHiyokoInit(actWork *xreg)
{
	if(xreg->work[0] == 0){
		WazaDefaultPosSet1(xreg, PM_MODE);
		xreg->work[1] = WazaEffWork[2];
		xreg->work[2] = WazaEffWork[3];
		xreg->work[0]++;
		return;
	}
	
	xreg->work[4] += xreg->work[1];
	xreg->dx = xreg->work[4] >> 8;
	xreg->dy = SinMove(xreg->work[3], xreg->work[2]);
	xreg->work[3] = 0xff & (xreg->work[3] + PIYOHIYOKO_ADDSEC);
	if(xreg->work[3] > 100){
		xreg->banish = xreg->work[3]%2;
	}
	
	if(xreg->work[3] > 120)
		DelWazaEffect(xreg);
}







//=========================================================================
//	�������
//
//																	taya
//=========================================================================

//======================================================
//	�����O�̋�
//======================================================
static void KawaraMirrorInit(actWork *act);
static void KawaraMirrorMove(actWork *act);

const actHeader ActKawaraWariMirror = {
	WAZACHR_ID_MIRROR1,
	WAZACHR_ID_MIRROR1,
	&WazaOamDataBldNormal[3],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	KawaraMirrorInit,
};

//==============================================
//	0:(0:�U���� 1:�h�䑤)  1:xofs  2:yofs
//	3:������܂ł�wait
//	4:������O�ɐk����wait�i0�Ȃ�k���Ȃ��j
//==============================================
static void KawaraMirrorInit(actWork *act)
{
	if(WazaEffWork[0] == 0){
		act->x = GetWazaEffPos(AttackNo, EFF_X);
		act->y = GetWazaEffPos(AttackNo, EFF_Y);
	}else{
		act->x = GetWazaEffPos(DefenceNo, EFF_X);
		act->y = GetWazaEffPos(DefenceNo, EFF_Y);
	}

	act->x += WazaEffWork[1];
	act->y += WazaEffWork[2];

	act->work[0] = 0;
	act->work[1] = WazaEffWork[3];
	act->work[2] = WazaEffWork[4];
	act->work[3] = 0;
	act->move = KawaraMirrorMove;
}
static void KawaraMirrorMove(actWork *act)
{
	switch(act->work[0]){
	case 0:
		if(--(act->work[1]) == 0){
			if(act->work[2] == 0){
				DelWazaEffect(act);
			}else{
				act->work[0]++;
			}
		}
		break;

	case 1:
		if(++(act->work[1]) > 1){
			act->work[1] = 0;
			act->work[3]++;
			if(act->work[3] & 1){
				act->dx = 2;
			}else{
				act->dx = -2;
			}
		}
		if(--(act->work[2]) == 0){
			DelWazaEffect(act);
		}
		break;
	}
}

//======================================================
//	���ꂽ��̋�
//======================================================
static void KawaraActInit(actWork *act);
static void KawaraActMove(actWork *act);

#define KAWARA_SPEED  (3)

const actHeader ActKawaraHeader = {
	WAZACHR_ID_KAWARA,
	WAZACHR_ID_KAWARA,
	&WazaOamDataNormal[2],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	KawaraActInit,
};
//==============================================
//	0:(0:�U����  1:�h�䑤)  1:������ް
//	2:���S�����XOffset  3:���S�����YOffset
//==============================================
static void KawaraActInit(actWork *act)
{
	if(WazaEffWork[0] == 0){
		act->x = GetWazaEffPos(AttackNo, EFF_X) + WazaEffWork[2];
		act->y = GetWazaEffPos(AttackNo, EFF_Y) + WazaEffWork[3];
	}else{
		act->x = GetWazaEffPos(DefenceNo, EFF_X) + WazaEffWork[2];
		act->y = GetWazaEffPos(DefenceNo, EFF_Y) + WazaEffWork[3];
	}
	act->oamData.CharNo += (WazaEffWork[1] * OBJ16_32x32);
	act->work[0] = 0;

	switch(WazaEffWork[1]){
	case 0:
		act->work[6] = -KAWARA_SPEED;
		act->work[7] = -KAWARA_SPEED;
		break;
	case 1:
		act->work[6] = KAWARA_SPEED;
		act->work[7] = -KAWARA_SPEED;
		break;
	case 2:
		act->work[6] = -KAWARA_SPEED;
		act->work[7] = KAWARA_SPEED;
		break;
	case 3:
		act->work[6] = KAWARA_SPEED;
		act->work[7] = KAWARA_SPEED;
		break;

	default:
		DelWazaEffect(act);
		return;
	}

	act->move = KawaraActMove;

}
static void KawaraActMove(actWork *act)
{
	act->x += act->work[6];
	act->y += act->work[7];

	act->work[0]++;
	if(act->work[0] > 40){
		DelWazaEffect(act);
	}
}












//=========================================================================
//
//		�΂�������
//													by TAYA  2002.07.12
//=========================================================================
#define BAKA_PUNCH_SYNC  (16)

//=========================================
// �������ތ��i0:�U���h  1:�h���U)
//=========================================
static void BakaHikariActMove(actWork *act);

#define BAKAHIKARI_WAIT  (180)
static void BakaHikariActInit(actWork *act)
{
	if(WazaEffWork[0]==0){
		act->x = GetWazaEffPos(AttackClient,PM_X);
		act->y = GetWazaEffPos(AttackClient,PM_Y);
		act->oamData.Priority = GetPokeBGPri(AttackNo);
		act->work[7] = DefenceNo;
	}else{
		act->oamData.Priority = GetPokeBGPri(DefenceNo);
		act->work[7] = AttackNo;
	}

	act->work[0] = 0;
	act->work[1] = 0x0c;  // eva
	act->work[2] = 0x08;  // evb
	act->move = BakaHikariActMove;
}
static void BakaHikariActMove(actWork *act)
{
	act->work[0]++;

	if(act->work[0] == BAKAHIKARI_WAIT)
	{
		*(vu16*)REG_BLDCNT = 0;  // ����������
		act->work[0] = BAKA_PUNCH_SYNC;
		act->work[1] = act->x;
		act->work[2] = GetWazaEffPos(act->work[7], PM_X);
		act->work[3] = act->y;
		act->work[4] = GetWazaEffPos(act->work[7], PM_Y);
		W_GetStraightSyncParam(act);
		SetWorkSeq(act,W_AffCheckEndSeq);
		act->move = W_ParallelSyncMove;
	}
}


static const actAffAnm BakaHikariAff[] = {
	{0x20, 0x20, 0, ACT_AFSET},
	{0x04, 0x04, 0, 64},
	{-0x6, -0x6, 0, 8},
	{ 0x6,  0x6, 0, 8},
	{ACT_AFLOOP,2,0,0},
};
static const actAffAnm * const BakaHikariAffTbl[] = {
	BakaHikariAff,
};
const actHeader BakaHikariHeader = {
	WAZACHR_ID_JUUDEN2,
	WAZACHR_ID_JUUDEN2,
	&WazaOamDataBldAffine2[3],
	DummyActAnmTbl,
	ACT_NTRANS,
	BakaHikariAffTbl,
	BakaHikariActInit,
};
//=========================================
// ����������K���L
// 0:X���W  1:1sync������̏㏸���x(8bit�Œ菭���j
// 2:���No  3:������Ԃ܂ł�wait
//=========================================
static void BakaGomiActMove(actWork *act);
static void BakaGomiFukitobiMove(actWork *act);

static void BakaGomiActInit(actWork *act)
{
	act->x = WazaEffWork[0];
	act->y = 120;

	act->work[0] = WazaEffWork[3];

	Divide32(&(act->work[4]),&(act->work[5]),(act->y << 8));
//	Divide32(&(act->work[6]),&(act->work[7]),WazaEffWork[1]);
	act->work[6] = WazaEffWork[1];

	act->oamData.CharNo += (WazaEffWork[2]*4);

	act->move = BakaGomiActMove;
}
static void BakaGomiActMove(actWork *act)
{
	if(act->work[0])
	{
		s32 y;//, ay;

		y = Unite32(act->work[4],act->work[5]);
//		ay = Unite32(act->work[4],act->work[5]);
//		y -= ay;
		y -= act->work[6];
		Divide32(&(act->work[4]),&(act->work[5]),y);
		act->y = y >> 8;

		if(act->y < -8){
			DelWazaEffect(act);
			return;
		}

		act->work[0]--;

	}else{
		s16 x1, y1, x2, y2;

		x1 = GetWazaEffPos(AttackNo,PM_X);
		y1 = GetWazaEffPos(AttackNo,PM_Y);
		x2 = GetWazaEffPos(DefenceNo,PM_X);
		y2 = GetWazaEffPos(DefenceNo,PM_Y);

		act->work[0] = x2-x1;
		act->work[1] = y2-y1;
		act->work[2] = act->x << 4;
		act->work[3] = act->y << 4;

		act->move = BakaGomiFukitobiMove;

	}
}

static void BakaGomiFukitobiMove(actWork *act)
{
	act->work[2] += act->work[0];
	act->work[3] += act->work[1];
	act->x = act->work[2] >> 4;
	act->y = act->work[3] >> 4;

	if(act->x < -8
	|| act->x > 248
	|| act->y < -8
	|| act->y > 120
	){
		DelWazaEffect(act);
	}
}

const actHeader BakaGomiHeader = {
	WAZACHR_ID_BAKAGOMI,
	WAZACHR_ID_BAKAGOMI,
	&WazaOamDataNormal[1],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	BakaGomiActInit,
};
//=========================================
// ����ȃp���`�i0:�U���h  1:�h���U)
//=========================================
static void BakaPunchActInit(actWork *act)
{
	u8 client_no;

	if(WazaEffWork[0]==0){
		act->x = GetWazaEffPos(AttackClient,PM_X);
		act->y = GetWazaEffPos(AttackClient,PM_Y);
		client_no = DefenceNo;
		act->oamData.Priority = GetPokeBGPri(AttackNo);
	}else{
		client_no = AttackNo;
		act->oamData.Priority = GetPokeBGPri(DefenceNo);
	}

	if(ContestCheck()){
		act->oamData.AffineParamNo |= 0x08;  // H flip
	}else{
		if(MineEnemyCheck(client_no)==SIDE_MINE){
			act->oamData.AffineParamNo |= 0x18;  // HV flip
		}
	}

	act->work[0] = BAKA_PUNCH_SYNC;
	act->work[1] = act->x;
	act->work[2] = GetWazaEffPos(client_no,PM_X);
	act->work[3] = act->y;
	act->work[4] = GetWazaEffPos(client_no,PM_Y);

	W_GetStraightSyncParam(act);
	SetWorkSeq(act,DelWazaEffect);
	act->move = W_ParallelSyncMove;

}

const actHeader BakaPunchHeader = {
	WAZACHR_ID_BAKA,
	WAZACHR_ID_BAKA,
	&WazaOamDataNormal[3],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	BakaPunchActInit,
};



//===================================================================
//���ς�
//===================================================================


//----------------------------------------------------------
//	�w�肵���|�P�����̵̾�ĊԂŒ����ړ�����u�����񐔔Łv
//	���E���]�����ʒu������s����
//
//	0:Xoffset	1:Yoffset
//	2:Sync��	3:�\������AnmNo
//----------------------------------------------------------

static void TuppariSeqMain(actWork *xreg)
{
//	xreg->dx = WazaEffWork[0] + ((WazaEffWork[2] - WazaEffWork[0])*WazaEffWork[7])/WazaEffWork[4];
//	xreg->dy = WazaEffWork[1] + ((WazaEffWork[3] - WazaEffWork[1])*WazaEffWork[7])/WazaEffWork[4];
//	xreg->dx = xreg->work[4];
//	xreg->dy = xreg->work[5];

	if(xreg->work[0] == xreg->work[4]){
		DelWazaEffect(xreg);
	}
	xreg->work[0]++;
}	

static void TuppariInitSeq(actWork *xreg)
{
	u8 wazacount;

	xreg->x = GetWazaEffPos(DefenceNo, PM_X);
	xreg->y = GetWazaEffPos(DefenceNo, PM_Y);

	
	xreg->work[1]  = WazaEffWork[3];				//�A�j���m�n�i�[
	xreg->work[2]  = WazaEffWork[0];
	xreg->work[3]  = WazaEffWork[1];
	xreg->work[4]  = WazaEffWork[2];
	wazacount = WazaCounter;
	if(MineEnemyCheck(DefenceNo)==SIDE_MINE) wazacount++;
	if(wazacount&1){							//�񐔂ɂ���č��E���]
		xreg->work[2]  = -xreg->work[2];
		xreg->work[1]++;						//���]�A�j���m�n�Z�b�g
	}

	ActAnmChg(xreg,xreg->work[1]);
	xreg->dx = xreg->work[2];
	xreg->dy = xreg->work[3];


	xreg->move = TuppariSeqMain;							//��͈ړ����s���Ă��炤
	
}

const	actHeader ActTuppariHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataNormal[2],
	KakutouAnmTbl,
	0,
	DummyActAffTbl,
	TuppariInitSeq,
};











//=========================================================================
//
//		���x���W
//												by matsuda 2002.07.19(��)
//=========================================================================
static void RevengeInitSeq(actWork *xreg);

#define REVENGE1_ANMWAIT	4
#define REVENGE2_ANMWAIT	6
#define REVENGE2_ANMWAIT2	6
static const actAnm Revenge1Anm0[]={
	{OBJ16_32x32*0,REVENGE1_ANMWAIT,0,0},
	{OBJ16_32x32*1,REVENGE1_ANMWAIT,0,0},
	{OBJ16_32x32*2,REVENGE1_ANMWAIT,0,0},
	{ACT_ANMEND,0,0,0}
};
static const actAnm Revenge1Anm1[]={
	{OBJ16_32x32*0,REVENGE1_ANMWAIT,0,1},
	{OBJ16_32x32*1,REVENGE1_ANMWAIT,0,1},
	{OBJ16_32x32*2,REVENGE1_ANMWAIT,0,1},
	{ACT_ANMEND,0,0,0}
};
static const actAnm Revenge1Anm2[]={
	{OBJ16_32x32*0,REVENGE1_ANMWAIT,1,0},
	{OBJ16_32x32*1,REVENGE1_ANMWAIT,1,0},
	{OBJ16_32x32*2,REVENGE1_ANMWAIT,1,0},
	{ACT_ANMEND,0,0,0}
};
static const actAnm *const Revenge1AnmTbl[]={
	Revenge1Anm0,
	Revenge1Anm1,
	Revenge1Anm2,
};
const	actHeader Revenge1Header={
	WAZACHR_ID_REVENGE1,
	WAZACHR_ID_REVENGE1,
	&WazaOamDataNormal[2],
	Revenge1AnmTbl,
	0,
	DummyActAffTbl,
	RevengeInitSeq,
};

static const actAnm Revenge2Anm0[]={
	{OBJ16_64x64*0,REVENGE2_ANMWAIT,0,0},
	{OBJ16_64x64*1,REVENGE2_ANMWAIT2,0,0},
	{ACT_ANMEND,0,0,0}
};
static const actAnm Revenge2Anm1[]={
	{OBJ16_64x64*0,REVENGE2_ANMWAIT,1,1},
	{OBJ16_64x64*1,REVENGE2_ANMWAIT2,1,1},
	{ACT_ANMEND,0,0,0}
};
static const actAnm Revenge2Anm2[]={
	{OBJ16_64x64*0,REVENGE2_ANMWAIT,1,0},
	{OBJ16_64x64*1,REVENGE2_ANMWAIT2,1,0},
	{ACT_ANMEND,0,0,0}
};
static const actAnm *const Revenge2AnmTbl[]={
	Revenge2Anm0,
	Revenge2Anm1,
	Revenge2Anm2,
};
const	actHeader Revenge2Header={
	WAZACHR_ID_REVENGE2,
	WAZACHR_ID_REVENGE2,
	&WazaOamDataNormal[3],
	Revenge2AnmTbl,
	0,
	DummyActAffTbl,
	RevengeInitSeq,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�ǂ���ɏo����(0=�U����  1=�h�䑤)
//----------------------------------------------------------
static void RevengeInitSeq(actWork *xreg)
{
	if(WazaEffWork[2] == 0)
		WazaDefaultPosSet2(xreg, EFF_MODE);
	else{
		WazaDefaultPosSet1(xreg, EFF_MODE);
		//ActAnmChg(xreg, 1);
	}
	
	if(ContestCheck()){
		ActAnmChg(xreg, 2);
	}
	else{
		if(MineEnemyCheck(AttackNo))
			ActAnmChg(xreg, 1);
	}
	
	xreg->move = W_AnmEndWait;
	SetWorkSeq(xreg, DelWazaEffect);
}


//=========================================================================
//
//		�������p���`
//												by mori 2002.07.25(��)
//=========================================================================
static void KiaiPunchActMove(actWork *xreg);

static const actAffAnm KiaiPunchAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
//	{0x180, 0x180, 0, ACT_AFSET},
	{0x200, 0x200, 0, ACT_AFSET},
	{-0x20,  -0x20, 0, 8},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const KiaiPunchAffTbl[] = {
	KiaiPunchAff0,
};

const	actHeader ActKiaiPunchHeader={
	WAZACHR_ID_KAKUTOU,
	WAZACHR_ID_KAKUTOU,
	&WazaOamDataAffine2[2],
	KakutouAnmTbl,
	0,
	KiaiPunchAffTbl,
	KiaiPunchActMove,
};


static void KiaiPunchActMove(actWork *Xreg)
{
	if(Xreg->affend_sw){
		Xreg->work[1]+=40;
		Xreg->work[1]&=0xff;
		Xreg->dx = SinMove(Xreg->work[1],2);
		if(++Xreg->work[0] > 40){
			DelWazaEffect(Xreg);
		}
	}
	
}







//=========================================================================
//
//		�X�J�C�A�b�p�[
//												by matsuda 2002.08.01(��)
//=========================================================================
//----------------------------------------------------------
//	�w�i�̃X�N���[���𐧌�
//		��WazaEffWork[7]��0xffff�������Ă�����Del���܂�
//	0:�΂߈ړ��J�n�܂ł̳���
//----------------------------------------------------------
#define SKY_DX		0x0500
#define SKY_DY		0x0b00
void SkyUpperHaikeiScrollTask(u8 id)
{
	TASK_TABLE *task;
	
	task = &TaskTable[id];
	switch(task->work[0]){
		case 0:
			HaikeiBGLoopSizeSet(0);	//�w�i��BG�ɑ΂��āA��荞���׸ށA���ނ�Ă���
			task->work[8] = WazaEffWork[0];
			task->work[0]++;
			break;
		case 1:
			if(task->work[8]-- == 0)
				task->work[0]++;
			break;
		case 2:
		default:
			task->work[9] += SKY_DX;
			break;
	}
	
	task->work[10] += SKY_DY;
	if(MineEnemyCheck(DefenceNo) == SIDE_MINE)
		ScrX3 += task->work[9] >> 8;
	else
		ScrX3 -= task->work[9] >> 8;
	ScrY3 += task->work[10] >> 8;
	task->work[9] &= 0x00ff;
	task->work[10] &= 0x00ff;
	
	if(WazaEffWork[7] == (s16)0xffff){
		ScrX3 = 0;
		ScrY3 = 0;
		HaikeiBGLoopSizeSet(1);
		DelTaskEffect(id);
	}
}

