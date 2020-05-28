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

#include "task.h"
#include "bss.h"
#include "cli_def.h"
#include "fight.h"
#include "palanm.h"
#include "laster.h"


//=========================================================================
//
//		�u���܂������v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void WazaTaskPokeToumeiExeMain(u8 id);
static void WazaTaskPokeToumeiRecoveryMain(u8 id);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------



//=========================================================================
//	���񂾂��߹�݂𓧖��ɂ��Ă���	
//			��������g������͕K��PokeToumeiRecovery���Ă�Ō��ɖ߂�����!!
//=========================================================================
//----------------------------------------------------------
// offset 0:����
//----------------------------------------------------------
void WazaTaskPokeToumeiExe(u8 id)
{
	u8 client;
	
	TaskTable[id].work[0] = WazaEffWork[0];
	client = AttackNo;
	
	TaskTable[id].work[1] = 0 << 8 | 16;
	*(vu16 *)REG_BLDALPHA = TaskTable[id].work[1];
	if(GetPokeBGNo(client) == 1)
		*(vu16 *)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL | BLD_BG1_1ST;
	else
		*(vu16 *)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL | BLD_BG2_1ST;
	TaskTable[id].TaskAdrs = WazaTaskPokeToumeiExeMain;
}

static void WazaTaskPokeToumeiExeMain(u8 id)
{
	u8 bld_h, bld_l;
	u8 wait;
	
	bld_h = TaskTable[id].work[1] >> 8;
	bld_l = TaskTable[id].work[1] & 0xff;
	wait = TaskTable[id].work[0];
		
	if(TaskTable[id].work[2] == wait)
	{
		bld_h++;
		bld_l--;
		TaskTable[id].work[1] = bld_h << 8 | bld_l;
		*(vu16 *)REG_BLDALPHA = TaskTable[id].work[1];
		TaskTable[id].work[2] = 0;

		if(bld_h == 16)
		{
			ActWork[PokemonStruct[AttackNo]].banish = 1;
			DelTaskEffect(id);
		}
	}
	else
		TaskTable[id].work[2]++;
}


//=========================================================================
//	PokeToumeiExe���g���ē����ɂȂ����߹�݂����̏�Ԃɖ߂�
//=========================================================================
//----------------------------------------------------------
//	offset 0:���ɖ߂����x(1�`16)
//----------------------------------------------------------
void WazaTaskPokeToumeiRecovery(u8 id)
{
	u8 client;
	
	TaskTable[id].work[0] = WazaEffWork[0];
	client = AttackNo;
	
	TaskTable[id].work[1] = 16 << 8 | 0;
	TaskTable[id].TaskAdrs = WazaTaskPokeToumeiRecoveryMain;
	*(vu16 *)REG_BLDALPHA = TaskTable[id].work[1];
}

static void WazaTaskPokeToumeiRecoveryMain(u8 id)
{
	u8 bld_h, bld_l;
	u8 wait;
	u8 client;
	
	client = AttackNo;
	
	bld_h = TaskTable[id].work[1] >> 8;
	bld_l = TaskTable[id].work[1] & 0xff;
	wait = TaskTable[id].work[0];
	
	if(TaskTable[id].work[2] == wait)
	{
		bld_h--;
		bld_l++;
		TaskTable[id].work[1] = bld_h << 8 | bld_l;
		*(vu16 *)REG_BLDALPHA = TaskTable[id].work[1];
		TaskTable[id].work[2] = 0;

		if(bld_h == 0)
		{
			*(vu16 *)REG_BLDCNT = 0;
			*(vu16 *)REG_BLDALPHA = 0;
			DelTaskEffect(id);
		}
	}
	else
		TaskTable[id].work[2]++;
}

//=========================================================================
//	���܂������p��BLD���W�X�^�Z�b�g
//=========================================================================
void DamasiutiBldCntSet(u8 id)
{
	*(vu16 *)REG_BLDALPHA = 16 << 8 | 0;
	if(GetPokeBGNo(AttackNo) == 1)
		*(vu16 *)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL | BLD_BG1_1ST;
	else
		*(vu16 *)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL | BLD_BG2_1ST;
	DelTaskEffect(id);
}










//=========================================================================
//
//		�u�ǂ�ڂ��v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void SeqWazaDorobouInit(actWork *Xreg);
static void SeqWazaDorobouMain(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define SUB_FURIHABA	(-20)
#define DEFAULT_FURIHABA	(-40)
#define SEC_SP			3
#define BAUND_NUM		3
#define LOOP_NUM		(127 / SEC_SP * BAUND_NUM)
#define FAST_BAUND_LOOP	(LOOP_NUM / BAUND_NUM * (BAUND_NUM - 1))

#define CETA	30
#define AFF_TBL_NUM		3

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
const	actHeader ActDorobouHeader={
	WAZACHR_ID_FUKURO,
	WAZACHR_ID_FUKURO,
	&WazaOamDataNormal[1],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaDorobouInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:
//-------------------------------------------------------------
static void SeqWazaDorobouInit(actWork *Xreg)
{
	Xreg->work[1] = GetWazaEffPos(DefenceNo, PM_X);
	Xreg->work[2] = GetWazaEffPos(AttackNo, PM_X);
	Xreg->work[3] = GetWazaEffPos(DefenceNo, PM_Y);
	Xreg->work[4] = GetWazaEffPos(AttackNo, PM_Y);
	
	Xreg->work[0] = LOOP_NUM;
	W_GetStraightMoveLoop(Xreg);

	Xreg->work[3] = -Xreg->work[1];
	Xreg->work[4] = -Xreg->work[2];
	Xreg->work[6] = DEFAULT_FURIHABA;
	Xreg->move = SeqWazaDorobouMain;
	Xreg->move(Xreg);
}

static void SeqWazaDorobouMain(actWork *Xreg)
{
	Xreg->work[3] += Xreg->work[1];
	Xreg->work[4] += Xreg->work[2];
	Xreg->dx = Xreg->work[3] >> 8;
	Xreg->dy = Xreg->work[4] >> 8;
	if(Xreg->work[7] == 0)
//	if(Xreg->work[7] < FAST_BAUND_LOOP)
	{
		Xreg->work[3] += Xreg->work[1];
		Xreg->work[4] += Xreg->work[2];
		Xreg->dx = Xreg->work[3] >> 8;
		Xreg->dy = Xreg->work[4] >> 8;
//		Xreg->work[7]++;
		Xreg->work[0]--;
	}
	
	Xreg->dy += SinMove(Xreg->work[5], Xreg->work[6]);
	Xreg->work[5] = 0xff & (Xreg->work[5] + SEC_SP);
	if(Xreg->work[5] > 127)
	{
		Xreg->work[5] = 0;
		Xreg->work[6] -= SUB_FURIHABA;
		Xreg->work[7]++;	//�o�E���h�񐔶���
	}
	
	Xreg->work[0]--;
	if(Xreg->work[0] == 0)
//	if(Xreg->work[7] > Xreg->work[0])
	{
		DelWazaEffect(Xreg);
	}
//	else
//		Xreg->work[7]++;
}















//=========================================================================
//
//		���݂�
//												by matsuda 2002.01.17(��)
//=========================================================================
static void SeqWazaKamitukuInit(actWork *xreg);
static void SeqWazaKamitukuMain(actWork *xreg);
static void SeqWazaKamitukuBack(actWork *xreg);

static const actAffAnm KamitukuAff0[] = {	//��
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,0,1},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm KamitukuAff1[] = {	//����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,32,1},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm KamitukuAff2[] = {	//��
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,64,1},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm KamitukuAff3[] = {	//����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,96,1},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm KamitukuAff4[] = {	//��
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,128,1},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm KamitukuAff5[] = {	//�E��
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,160,1},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm KamitukuAff6[] = {	//�E
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,192,1},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm KamitukuAff7[] = {	//�E��
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0,224,1},
	{ACT_AFEND, 0, 0, 0 },
};
const actAffAnm * const KamitukuAffTbl[] = {
	KamitukuAff0,
	KamitukuAff1,
	KamitukuAff2,
	KamitukuAff3,
	KamitukuAff4,
	KamitukuAff5,
	KamitukuAff6,
	KamitukuAff7,
};

const	actHeader ActKamitukuHeader={
	WAZACHR_ID_KAMITUKU,
	WAZACHR_ID_KAMITUKU,
	&WazaOamDataBldAffine[3],
	DummyActAnmTbl,
	0,
	KamitukuAffTbl,
	SeqWazaKamitukuInit,
};

//-- �k(�L�k�Ȃǂ�) --//
const	actHeader ActKaraHeader={
	WAZACHR_ID_KARA3,
	WAZACHR_ID_KARA3,
	&WazaOamDataBldAffine[3],
	DummyActAnmTbl,
	0,
	KamitukuAffTbl,
	SeqWazaKamitukuInit,
};

//=========================================================================
//	offset	0:Xoffset  1:Yoffset 
//			 2:���̌���(0:��  1:����  2:��  3:����  4:��  5:�E��  6:�E  7:�E��
//			3:Dx���x(����)   4:Dy���x(����)   5:Dx,Dy�𑫂���
//=========================================================================
static void SeqWazaKamitukuInit(actWork *xreg)
{
	//WazaDefaultPosSet1(xreg, PM_MODE);
	xreg->x += WazaEffWork[0];
	xreg->y += WazaEffWork[1];
	ActAffAnmChg(xreg, WazaEffWork[2]);
	xreg->work[0] = WazaEffWork[3];
	xreg->work[1] = WazaEffWork[4];
	xreg->work[2] = WazaEffWork[5];
	xreg->move = SeqWazaKamitukuMain;
}

static void SeqWazaKamitukuMain(actWork *xreg)
{
	xreg->work[4] += xreg->work[0];
	xreg->work[5] += xreg->work[1];
	xreg->dx = xreg->work[4] >> 8;
	xreg->dy = xreg->work[5] >> 8;
	if(++xreg->work[3] == xreg->work[2])
		xreg->move = SeqWazaKamitukuBack;
}

static void SeqWazaKamitukuBack(actWork *xreg)
{
	xreg->work[4] -= xreg->work[0];
	xreg->work[5] -= xreg->work[1];
	xreg->dx = xreg->work[4] >> 8;
	xreg->dy = xreg->work[5] >> 8;
	if(--xreg->work[3] == 0)
		W_AffCheckEndSeq(xreg);
}












//=========================================================================
//
//		�����Ȃ�
//														TAYA  2002.07.22
//=========================================================================
static void UsonakiActMove(actWork *act);
//================================================
// 0:(0:�U���� 1:�h�䑤)
// 1:�ʒu  0 ... �E���O   2 ... �����O
//         1 ... �E�㉜     3 ... ���㉜
//================================================
static void UsonakiActInit(actWork *act)
{
	u8 client = (WazaEffWork[0]==0)? AttackNo : DefenceNo;
	s8  xofs = 20;

	act->oamData.CharNo += 4;


	switch(WazaEffWork[1]){
	case 0:
		act->x = W_GetPokeSize(client, POKESIZE_RIGHT) - 8;
		act->y = W_GetPokeSize(client, POKESIZE_TOP) + 8;
		break;
	case 1:
		act->x = W_GetPokeSize(client, POKESIZE_RIGHT) - 14;
		act->y = W_GetPokeSize(client, POKESIZE_TOP) + 16;
		break;

	case 2:
		act->x = W_GetPokeSize(client, POKESIZE_LEFT) + 8;
		act->y = W_GetPokeSize(client, POKESIZE_TOP) + 8;
		ActAffAnmChg(act,1);
		xofs *= -1;
		break;

	case 3:
		act->x = W_GetPokeSize(client, POKESIZE_LEFT) + 14;
		act->y = W_GetPokeSize(client, POKESIZE_TOP) + 16;
		ActAffAnmChg(act,1);
		xofs *= -1;
		break;

	}

	act->work[0] = 32;
	act->work[2] = act->x + xofs;
	act->work[4] = act->y + 12;
	act->work[5] = -12;

	YamanariMoveInit(act);
	act->move =  UsonakiActMove;

}
static void UsonakiActMove(actWork *act)
{
	if(YamanariMoveMain(act)){
		W_AffCheckEndSeq(act);
	}
}

static const actAffAnm UsonakiAffAnm0[] = {  // �E����
	{0xc0,0xc0,80,ACT_AFSET},
	{0,0,-2,8},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm UsonakiAffAnm1[] = {  // ������
	{0xc0,0xc0,176,ACT_AFSET},
	{0,0,2,8},
	{ACT_AFEND, 0, 0, 0 },
};
const actAffAnm * const UsonakiAffAnmTbl[] = {
	UsonakiAffAnm0,
	UsonakiAffAnm1,
};

const actHeader UsonakiHeader = {
	WAZACHR_ID_MIZU2,
	WAZACHR_ID_MIZU2,
	&WazaOamDataAffine[1],
	DummyActAnmTbl,
	0,
	UsonakiAffAnmTbl,
	UsonakiActInit,

};













//=========================================================================
//
//		�����݂₰
//														TAYA  2002.07.22
//=========================================================================
#define OKIMIYAGE_COL  (0x0000)
#define OKIMIYAGE_EVA  (0x0c)
#define OKIMIYAGE_EVB  (0x08)
#define OKIMIYAGE_LINE_SPEED  (8)
#define OKIMIYAGE_TOPLINE_MINE   (-12)
#define OKIMIYAGE_TOPLINE_ENEMY  (-64)

enum OKIMIYAGE_WORK {
	OW_SEQ,
	OW_C1,
	OW_C2,
	OW_BGNO,
	OW_EFTOP,
	OW_EFBOTTOM,
	OW_POKETOP,
	OW_POKEBOTTOM,
	OW_TOPLINE,
	OW_SCRX,
	OW_SCRY,
	OW_EVA,
	OW_EVB,
	OW_DIFFY,
	OW_WIN_L,
	OW_WIN_R,
};

static void allpoke_oampri_set(u8 pri);
static void okimiyage_lasterset(TASK_TABLE *my);

//===========================================
// �U�����̪��
//===========================================
static void TaskOkimiyageAttackMain(u8 id);

void TaskOkimiyageAttack(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];
	LasterData LD;
	u16 i, winout_mask;

//	my->work[OW_POKEBOTTOM] = W_GetPokeSize(AttackNo, POKESIZE_REAL_BOTTOM);
	my->work[OW_POKEBOTTOM] = GetWazaEffPos(AttackNo, EFF_Y)+31;
	my->work[OW_POKETOP] = W_GetPokeSize(AttackNo, POKESIZE_TOP) -7;
	my->work[OW_EFBOTTOM] = my->work[OW_POKEBOTTOM];
	my->work[OW_EFTOP] = my->work[OW_POKETOP];

	my->work[OW_DIFFY] = (my->work[OW_POKEBOTTOM] - my->work[OW_POKETOP]) << 8;

	my->work[OW_WIN_L] = GetWazaEffPos(AttackNo, EFF_X) - 32;
	my->work[OW_WIN_R] = my->work[OW_WIN_L] + 64;

	if(MineEnemyCheck(AttackNo)==SIDE_MINE){
		my->work[OW_TOPLINE] = OKIMIYAGE_TOPLINE_MINE;
	}else{
		my->work[OW_TOPLINE] = OKIMIYAGE_TOPLINE_ENEMY;
	}

	my->work[OW_BGNO] = GetPokeBGNo(AttackNo);
	if(my->work[OW_BGNO]==1){
		WazaBgData wazabg;

		GetWazaEffBgAdrs(&wazabg);

		my->work[OW_SCRY] = ScrY1;
		*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_BG1_1ST | BLD_2ND_ALL;
		PaletteWorkClear(OKIMIYAGE_COL, 16*wazabg.PlttNo, 0x20);

		LD.DestAdrs = (void*)REG_BG1VOFS;

		winout_mask = WIN_BG1_ON;


		if(ContestCheck()==0){
			ScrX2 += 240;  // �����߹�݂�BG����ʊO��
		}

	}else{
		my->work[OW_SCRY] = ScrY2;
		*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_BG2_1ST | BLD_2ND_ALL;
		PaletteWorkClear(OKIMIYAGE_COL, 16*BTL_BG_PAL2, 0x20);

		LD.DestAdrs = (void*)REG_BG2VOFS;

		winout_mask = WIN_BG2_ON;

		if(ContestCheck()==0){
			ScrX1 += 240;  // �����߹�݂�BG����ʊO��
		}

	}

	LD.DMAControl = H_DMA_ENABLE16;
	LD.TransType = LASTER_NORMAL;
	LD.LoopCount = 0;

	my->work[OW_EVA] = 0x00;
	my->work[OW_EVB] = 0x10;

	my->work[OW_SEQ] = 0;
	my->work[OW_C1] = 0;
	my->work[OW_C2] = 0;

	allpoke_oampri_set(3);

	for(i = 0; i <  112; i++)
	{
		LasterBuffer[0][i] = my->work[OW_SCRY];
		LasterBuffer[1][i] = my->work[OW_SCRY];
	}

	LasterDmaSet(LD);

	*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | (WIN_ALL_ON ^ winout_mask);
	*(vu16*)REG_WININ  = (WIN_ALL_ON << 8) | WIN_ALL_ON;

	Win0H = (my->work[OW_WIN_L]<<8)|my->work[OW_WIN_R];
	Win0V = (0<<8) | 160;

	my->TaskAdrs = TaskOkimiyageAttackMain;
}
static void TaskOkimiyageAttackMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[OW_SEQ]){
	case 0:
		if(++(my->work[OW_C1]) > 1){
			my->work[OW_C1] = 0;
			my->work[OW_C2]++;
			if(my->work[OW_C2]&1){
				if(my->work[OW_EVA] != OKIMIYAGE_EVA) my->work[OW_EVA]++;
			}else{
				if(my->work[OW_EVB] != OKIMIYAGE_EVB) my->work[OW_EVB]--;
			}
			*(vu16*)REG_BLDALPHA = (my->work[OW_EVB]<<8)|my->work[OW_EVA];
			if(my->work[OW_EVA] == OKIMIYAGE_EVA
			&& my->work[OW_EVB] == OKIMIYAGE_EVB
			){
				my->work[OW_SEQ]++;
			}
		}
		break;

	case 1:
		my->work[OW_EFTOP] -= OKIMIYAGE_LINE_SPEED;

		okimiyage_lasterset(my);

		if(my->work[OW_EFTOP] < my->work[OW_TOPLINE]){
			my->work[OW_SEQ]++;
		}
		break;

	case 2:
		my->work[OW_EFTOP] -= OKIMIYAGE_LINE_SPEED;

		okimiyage_lasterset(my);

		my->work[OW_WIN_L] += 4;
		my->work[OW_WIN_R] -= 4;
		if(my->work[OW_WIN_L] >= my->work[OW_WIN_R])
		{
			my->work[OW_WIN_L] = my->work[OW_WIN_R];
		}

		Win0H = (my->work[OW_WIN_L]<<8)|my->work[OW_WIN_R];

		if(my->work[OW_WIN_L] == my->work[OW_WIN_R])
		{
			my->work[OW_SEQ]++;
		}
		break;

	case 3:
		LasterSys.TransType = LASTER_END;
		my->work[OW_SEQ]++;
		break;

	case 4:
		DelTaskEffect(id);
		break;
	}
}
//===========================================
// �h�䑤�̪��  ������
//===========================================
static void TaskOkimiyageDefenceMain(u8 id);

void TaskOkimiyageDefence(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[OW_SEQ]){
	case 0:
		// ��ýĂ������牽�����܂���
		if(ContestCheck()==1){
			Win0H = 0;
			Win0V = 0;
			*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
			*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | WIN_ALL_ON;

			DelTaskEffect(id);
			return;
		}

		my->work[OW_BGNO] = GetPokeBGNo(DefenceNo);
		if(my->work[OW_BGNO]==1){
			*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_BG1_1ST | BLD_2ND_ALL;
			ScrX2 += 240;  // �����߹�݂�BG����ʊO��
		}else{
			*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_BG2_1ST | BLD_2ND_ALL;
			ScrX1 += 240;  // �����߹�݂�BG����ʊO��
		}
		my->work[OW_SEQ]++;
		break;

	case 1:
		if(my->work[OW_BGNO]==1){
			WazaBgData wazabg;
			GetWazaEffBgAdrs(&wazabg);

			my->work[OW_SCRY] = ScrY1;
			PaletteWorkClear(OKIMIYAGE_COL, 16*wazabg.PlttNo, 0x20);
		}else{
			my->work[OW_SCRY] = ScrY2;
			PaletteWorkClear(OKIMIYAGE_COL, 16*BTL_BG_PAL2, 0x20);
		}
		allpoke_oampri_set(3);
		my->work[OW_SEQ]++;
		break;

	case 2:
		my->work[OW_POKEBOTTOM] = GetWazaEffPos(DefenceNo, EFF_Y)+31;
		my->work[OW_POKETOP] = W_GetPokeSize(DefenceNo, POKESIZE_TOP) - 7;
		my->work[OW_DIFFY] = (my->work[OW_POKEBOTTOM] - my->work[OW_POKETOP]) << 8;
		my->work[OW_WIN_L] = GetWazaEffPos(DefenceNo, EFF_X) - 4;
		my->work[OW_WIN_R] = my->work[OW_WIN_L] + 8;
		if(MineEnemyCheck(DefenceNo)==SIDE_MINE){
			my->work[OW_TOPLINE] = OKIMIYAGE_TOPLINE_MINE;
		}else{
			my->work[OW_TOPLINE] = OKIMIYAGE_TOPLINE_ENEMY;
		}
		my->work[OW_EFTOP] = my->work[OW_TOPLINE];
		my->work[OW_EFBOTTOM] = my->work[OW_TOPLINE];
		my->work[OW_EVA] = OKIMIYAGE_EVA;
		my->work[OW_EVB] = OKIMIYAGE_EVB;
		my->work[OW_SEQ]++;
		break;

	case 3:
		{
			LasterData LD;
			u16 i;

			if(my->work[OW_BGNO]==1){
				LD.DestAdrs = (void*)REG_BG1VOFS;
			}else{
				LD.DestAdrs = (void*)REG_BG2VOFS;
			}
	
			for(i = 0; i <  112; i++)
			{
				LasterBuffer[0][i] = my->work[OW_SCRY] + (159-i);
				LasterBuffer[1][i] = my->work[OW_SCRY] + (159-i);
			}

			LD.DMAControl = H_DMA_ENABLE16;
			LD.TransType = LASTER_NORMAL;
			LD.LoopCount = 0;

			LasterDmaSet(LD);

			my->work[OW_SEQ]++;
		}
		break;

	case 4:
		if(my->work[OW_BGNO]==1){
			*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | (WIN_ALL_ON ^ WIN_BG1_ON);
		}else{
			*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | (WIN_ALL_ON ^ WIN_BG2_ON);
		}
		*(vu16*)REG_WININ  = (WIN_ALL_ON << 8) | WIN_ALL_ON;

		Win0H = (my->work[OW_WIN_L]<<8)|my->work[OW_WIN_R];
		Win0V = (0<<8) | 160;

		my->work[OW_SEQ] = 0;
		my->work[OW_C1] = 0;
		my->work[OW_C2] = 0;

		*(vu16*)REG_BLDALPHA = (OKIMIYAGE_EVB << 8) | OKIMIYAGE_EVA;
		my->TaskAdrs = TaskOkimiyageDefenceMain;
		break;
	}
}
static void TaskOkimiyageDefenceMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[OW_SEQ]){
	case 0:
		my->work[OW_EFBOTTOM] += OKIMIYAGE_LINE_SPEED;
		if(my->work[OW_EFBOTTOM] >= my->work[OW_POKEBOTTOM]){
			my->work[OW_EFBOTTOM] = my->work[OW_POKEBOTTOM];
		}

		okimiyage_lasterset(my);

		if(my->work[OW_EFBOTTOM] == my->work[OW_POKEBOTTOM]){
			my->work[OW_SEQ]++;
		}
		break;

	case 1:
		if((my->work[OW_WIN_R] - my->work[OW_WIN_L]) < 64){
			my->work[OW_WIN_L] -= 4;
			my->work[OW_WIN_R] += 4;
		}else{
			my->work[OW_C1] = 1;
		}

		Win0H = (my->work[OW_WIN_L]<<8)|my->work[OW_WIN_R];


		my->work[OW_EFTOP] += OKIMIYAGE_LINE_SPEED;
		if(my->work[OW_EFTOP] >= my->work[OW_POKETOP]){
			my->work[OW_EFTOP] = my->work[OW_POKETOP];
		}

		okimiyage_lasterset(my);

		if(my->work[OW_EFTOP] == my->work[OW_POKETOP]
		&& my->work[OW_C1])
		{
			my->work[OW_C1] = 0;
			my->work[OW_SEQ]++;
		}
		break;

	case 2:
		if(++(my->work[OW_C1]) > 1){
			my->work[OW_C1] = 0;
			my->work[OW_C2]++;
			if(my->work[OW_C2] & 1){
				if(my->work[OW_EVA])  my->work[OW_EVA]--;
			}else{
				if(my->work[OW_EVB] < 0x10) my->work[OW_EVB]++;
			}
			*(vu16*)REG_BLDALPHA = (my->work[OW_EVB]<<8) | my->work[OW_EVA];
			if(my->work[OW_EVA] == 0
			&& my->work[OW_EVB] == 0x10
			){
				my->work[OW_SEQ]++;
			}
		}
		break;

	case 3:
		LasterSys.TransType = LASTER_END;
		my->work[OW_SEQ]++;
		break;

	case 4:
		Win0H = 0;
		Win0V = 0;
		*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
		*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | WIN_ALL_ON;

		DelTaskEffect(id);
		break;
	}
}

static void okimiyage_lasterset(TASK_TABLE *my)
{
	s32 diffy, y;
	s16 i, height;

	height = my->work[OW_EFBOTTOM] - my->work[OW_EFTOP];

	if(height){
		diffy = my->work[OW_DIFFY] / height;
		y = my->work[OW_POKETOP] << 8;
		for(i = 0; i < my->work[OW_EFTOP]; i++)
		{
			LasterBuffer[LasterSys.BufFlag][i] = my->work[OW_SCRY] + (159-i);
		}
		for(i = my->work[OW_EFTOP]; i <= my->work[OW_EFBOTTOM]; i++)
		{
			if(i >= 0){
				height = (y >> 8) - i;
				LasterBuffer[LasterSys.BufFlag][i] = my->work[OW_SCRY] + height;
			}
			y += diffy;
		}
		y = my->work[OW_SCRY] + (159 - i);
		while(i < my->work[OW_POKEBOTTOM]){
			if(i >= 0){
				LasterBuffer[LasterSys.BufFlag][i] = y--;
			}
			i++;
		}
	}else{
		y = my->work[OW_SCRY] + 159;
		// top �� bottom �̍��������Ȃ��������
		for(i = 0; i < 112; i++){
			LasterBuffer[0][i] = y;
			LasterBuffer[1][i] = y;
			y--;
		}
	}

}
//==============================================================
// �S�߹�ݱ�����OAM��ײ��è���
//==============================================================
static void allpoke_oampri_set(u8 pri)
{
	u16 i, no;
	for(i = 0; i < 4; i++){
		no = W_GetPokeActNo(i);
		if(no != 0xff){
			ActWork[no].oamData.Priority = pri;
		}
	}
}


//=========================================================================
//	1�̖ڂ̃|�P����������łĂ�BG�ɗ���
//	add by matsuda 2002.09.04(��)
//=========================================================================
void PokeBGDeadDrop(u8 id)
{
	u8 bg_no;
	
	if(GetPokeBGNo(AttackNo) == 1)
		bg_no = 0;	//��ײ��è�̊֌W��A1�̖ځA���ͺ�ýĂȂ�BG1�ɗ���
	else
		bg_no = 1;
	PokeBGDrop(AttackNo, bg_no);	//��̖ڂ͕K��BG�ɗ���
	ActWork[PokemonStruct[AttackNo]].banish = 0;
	if(PokeBG2taimeCheck(AttackNo^2)){
		PokeBGDrop(AttackNo^2, bg_no^1);
		ActWork[PokemonStruct[AttackNo^2]].banish = 0;
	}
	DelTaskEffect(id);
}

//----------------------------------------------------------
//	PokeBGDeadDrop�ŗ�����BG��ر����
//----------------------------------------------------------
void PokeBGDeadDropClear(u8 id)
{
	u8 bg_no;
	
	if(GetPokeBGNo(AttackNo) == 1)
		bg_no = 0;	//��ײ��è�̊֌W��A1�̖ځA���ͺ�ýĂȂ�BG1�ɗ���
	else
		bg_no = 1;
	PokeBGDropClear(bg_no);
	if(PokeBG2taimeCheck(AttackNo^2))
		PokeBGDropClear(bg_no^1);
	DelTaskEffect(id);
}

