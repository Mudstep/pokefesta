//=========================================================================
//	
//	�h���S���n �Z�G�t�F�N�g
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
#include "laster.h"
#include "fight.h"


//=========================================================================
//
//		�u�������v
//														by nohara
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
void SeqWazaGekiInit(actWork *Xreg);
void SeqWazaGekihiInit(actWork *Xreg);
void SeqWazaGekiMain(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define FURIHABA	10
#define GEKIRIN_Y_HOSEI	8
#define ANM_WAIT_G	4

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��	�|�P�����p
//----------------------------------------------------------
const	actHeader ActGekiHeader={
	0,
	0,
	&DummyActOam,
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaGekiInit,
};

//----------------------------------------------------------
//	���ł�����
//----------------------------------------------------------
static const actAnm GekihiAnm[]={
	{OBJ16_32x32 * 0, ANM_WAIT_G, 0, 0},
	{OBJ16_32x32 * 1, ANM_WAIT_G, 0, 0},
	{OBJ16_32x32 * 2, ANM_WAIT_G, 0, 0},
	{OBJ16_32x32 * 3, ANM_WAIT_G, 0, 0},
	{OBJ16_32x32 * 4, ANM_WAIT_G, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm *const GekihiAnmTbl[]={
		GekihiAnm,
};
const	actHeader ActGekihiHeader={
	WAZACHR_ID_FIRE,
	WAZACHR_ID_FIRE,
	&WazaOamDataNormal[2],
	GekihiAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaGekihiInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------
void SeqWazaGekiInit(actWork *Xreg)
{
	Xreg->banish = 1;
	
	Xreg->work[5] = PokemonStruct[AttackNo];
	Xreg->work[0] = 128;
	Xreg->work[1] = FURIHABA;
	Xreg->work[2] = WazaEffWork[0];	//zouka
	Xreg->work[3] = WazaEffWork[1];	//loop

	Xreg->move = SeqWazaGekiMain;

	ActWork[Xreg->work[5]].y+=GEKIRIN_Y_HOSEI;
}

void SeqWazaGekiMain(actWork *Xreg)
{
	if( Xreg->work[3] )
	{
		Xreg->work[3]--;

		//�~��`��
		ActWork[Xreg->work[5]].dx = SinMove(Xreg->work[0], Xreg->work[1]);
		ActWork[Xreg->work[5]].dy = CosMove(Xreg->work[0], Xreg->work[1]);

		//�p�x�ω�
		Xreg->work[0]+=Xreg->work[2];

		if( Xreg->work[0] > 255 )
			Xreg->work[0]-=256;
	}else
	{
		ActWork[Xreg->work[5]].dx=0;
		ActWork[Xreg->work[5]].dy=0;

		ActWork[Xreg->work[5]].y-=GEKIRIN_Y_HOSEI;

		Xreg->move = W_AffCheckEndSeq;
	}
}

//-------------------------------------------------------------
// ���ł�����
//-------------------------------------------------------------
void SeqWazaGekihiInit(actWork *Xreg)
{
	Xreg->x = GetWazaEffPos(AttackNo, PM_X);
	Xreg->y = GetWazaEffPos(AttackNo, PM_Y);
	if(MineEnemyCheck(AttackNo))
	{
		Xreg->x -= WazaEffWork[0];
		WazaEffWork[3] = -WazaEffWork[3];
		WazaEffWork[4] = -WazaEffWork[4];
	}
	else
		Xreg->x += WazaEffWork[0];

	Xreg->y += WazaEffWork[1];

	Xreg->work[0] = WazaEffWork[2];		//loop
	Xreg->work[1] = WazaEffWork[3];		//work[2]�͌v�Z�p
	Xreg->work[3] = WazaEffWork[4];		//work[4]��
	Xreg->work[5] = WazaEffWork[5];		//�\��on,off�̊Ԋu

	Xreg->banish = 1;					//chg_02.08.28_iwasawa
	SetWorkSeq(Xreg, W_AffCheckEndSeq);
	Xreg->move=W_ParallelMoveSeq2;
}














//=========================================================================
//
//		�u��イ�̂�����v�u��イ�̂��Ԃ��v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void SeqWazaHibasiraInit(actWork *Xreg);
static void SeqWazaRyuunoIbukiInit(actWork *Xreg);
static void SeqWazaRyuunoIkariInit(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define ANM_WAIT_RY	3
#define HIBASIRA_ANM_WAIT	5

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
static const actAnm RyuunoIbukiAnm0[]={
//		{OBJ16_32x32*0,ANM_WAIT_RY,0,0},
		{OBJ16_32x32*1,ANM_WAIT_RY,0,0},
		{OBJ16_32x32*2,ANM_WAIT_RY,0,0},
		{OBJ16_32x32*3,ANM_WAIT_RY,0,0},
//		{OBJ16_32x32*4,ANM_WAIT_RY,0,0},
		{ACT_ANMLOOP,0,0,0}
};
static const actAnm RyuunoIbukiAnm1[]={
//		{OBJ16_32x32*0,ANM_WAIT_RY,1,1},
		{OBJ16_32x32*1,ANM_WAIT_RY,1,1},
		{OBJ16_32x32*2,ANM_WAIT_RY,1,1},
		{OBJ16_32x32*3,ANM_WAIT_RY,1,1},
//		{OBJ16_32x32*4,ANM_WAIT_RY,1,1},
		{ACT_ANMLOOP,0,0,0}
};
static const actAnm *const RyuunoIbukiAnmTbl[]={
		RyuunoIbukiAnm0,
		RyuunoIbukiAnm1,
};
static const actAffAnm RyuunoIbukiAff0[] =
{
	{0x50, 0x50, 127, ACT_AFSET},
	{13, 13, 0, 100},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm RyuunoIbukiAff1[] =
{
	{0x50, 0x50, 0, ACT_AFSET},
	{13, 13, 0, 100},
	{ACT_AFEND,0,0,0},
};
const actAffAnm * const RyuunoIbukiAffTbl[] = {
	RyuunoIbukiAff0,
	RyuunoIbukiAff1,
};
const	actHeader ActRyuunoIbukiHeader={
	WAZACHR_ID_FIRE,
	WAZACHR_ID_FIRE,
	&WazaOamDataAffine2[2],
	RyuunoIbukiAnmTbl,
	0,
	RyuunoIbukiAffTbl,
	SeqWazaRyuunoIkariInit,
};

//-- �Β� --//
static const actAnm HibasiraAnm0[]={
		{OBJ16_32x32*0,HIBASIRA_ANM_WAIT,0,0},
		{OBJ16_32x32*1,HIBASIRA_ANM_WAIT,0,0},
		{OBJ16_32x32*2,HIBASIRA_ANM_WAIT,0,0},
		{OBJ16_32x32*3,HIBASIRA_ANM_WAIT,0,0},
		{OBJ16_32x32*4,HIBASIRA_ANM_WAIT,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm *const HibasiraAnmTbl[]={
		HibasiraAnm0,
};
const	actHeader ActHibasiraHeader={
	WAZACHR_ID_HIBASIRA,
	WAZACHR_ID_HIBASIRA,
	&WazaOamDataNormal[2],
	HibasiraAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaHibasiraInit,
};


//-- ��イ�̂����� --//
static const actAnm RyuunoIkariAnm0[]={
		{OBJ16_32x32*1,ANM_WAIT_RY,0,0},
		{OBJ16_32x32*2,ANM_WAIT_RY,0,0},
		{OBJ16_32x32*3,ANM_WAIT_RY,0,0},
		{ACT_ANMLOOP,0,0,0}
};
static const actAnm *const RyuunoIkariAnmTbl[]={
		RyuunoIkariAnm0,
		RyuunoIkariAnm0,
};

static const actAffAnm RyuunoIkariAff0[] =
{
	{ 100, 100, 127, 1},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm RyuunoIkariAff1[] =
{
	{ 100, 100, 0, 1},
	{ACT_AFEND,0,0,0},
};
const actAffAnm * const RyuunoIkariAffTbl[] = {
	RyuunoIkariAff0,
	RyuunoIkariAff1,
};

const	actHeader ActRyuunoIkariHeader={
	WAZACHR_ID_FIRE,
	WAZACHR_ID_FIRE,
	&WazaOamDataAffine2[2],
	RyuunoIkariAnmTbl,
	0,
	RyuunoIkariAffTbl,
	SeqWazaRyuunoIkariInit,
};


//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:�n�_X�̾��  1:�n�_Y�̾�ā@�@2:�I�_X�̾��  3:�I�_Y�̾��
//     �@ 4:��߰��(ٰ�߉�)
//-------------------------------------------------------------
static void SeqWazaRyuunoIbukiInit(actWork *Xreg)
{
	EffPositionReset(Xreg);
	
	Xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	Xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	if(MineEnemyCheck(AttackNo))
	{
		Xreg->x -= WazaEffWork[1];
		Xreg->y += WazaEffWork[1];
		Xreg->work[2] -= WazaEffWork[2];
		Xreg->work[4] += WazaEffWork[3];
	}
	else
	{
		Xreg->x += WazaEffWork[0];
		Xreg->y += WazaEffWork[1];
		Xreg->work[2] += WazaEffWork[2];
		Xreg->work[4] += WazaEffWork[3];
		ActAnmChg(Xreg, 1);
	}
	Xreg->work[0] = WazaEffWork[4];
//	Xreg->work[1] = Xreg->x;
//	Xreg->work[3] = Xreg->y;

//	Xreg->move = W_StraightMove;
	Xreg->move = W_StraightSyncMove;
	SetWorkSeq(Xreg, W_AffCheckEndSeq);
}



//=========================================================================
//	�Β�
//=========================================================================
//----------------------------------------------------------
//	offset 0: 0=�U�����ɏo��   1=�h�䑤�ɏo��
//		�@ 1:X�̾��   2:Y�̾��
//----------------------------------------------------------
static void SeqWazaHibasiraInit(actWork *Xreg)
{
//	if(WazaEffWork[0] == 0)
//		EffPositionReset(Xreg);
	if(WazaEffWork[0] == 0){
		Xreg->x = GetWazaEffPos(AttackNo, EFF_X);
		Xreg->y = GetWazaEffPos(AttackNo, EFF_Y);
	}
	else{
		Xreg->x = GetWazaEffPos(DefenceNo, EFF_X);
		Xreg->y = GetWazaEffPos(DefenceNo, EFF_Y);
	}
	EffXpositionSet(Xreg, WazaEffWork[1]);
	Xreg->y += WazaEffWork[2];
	Xreg->move = W_AnmEndWait;
	SetWorkSeq(Xreg, W_AffCheckEndSeq);
}


//=========================================================================
//	��イ�̂�����
//=========================================================================
//-------------------------------------------------------------
// offset 0:�n�_X�̾��  1:�n�_Y�̾�ā@�@2:�I�_X�̾��  3:�I�_Y�̾��
//     �@ 4:��߰��(ٰ�߉�)
//-------------------------------------------------------------
static void SeqWazaRyuunoIkariInit(actWork *Xreg)
{
	if(MineEnemyCheck(AttackNo))
		ActAffAnmChg(Xreg, 1);
	SeqWazaRyuunoIbukiInit(Xreg);
}















//=========================================================================
//
//	��イ�̂܂�
//														TAYA 2002.07.25
//=========================================================================
#define RYUNOMAI_SP_INI  (1)
#define RYUNOMAI_SP_ADD  (1)
#define RYUNOMAI_SP_ADD_WAIT  (5)
#define RYUNOMAI_SP_MAX  (16)
#define RYUNOMAI_R_MAX   (150)

static void RyunoMaiActMove(actWork *act);

//==============================================
// �~  0:�����p�x
//==============================================
static void RyuNoMaiActInit(actWork *act)
{
	u16 h, w;

	act->x = GetWazaEffPos(AttackNo, PM_X);
	act->y = GetWazaEffPos(AttackNo, PM_Y);

	act->work[4] = 0;
	act->work[5] = RYUNOMAI_SP_INI;
	act->work[6] = WazaEffWork[0];  // 6:�����p�x

	h = W_GetPokeSize(AttackClient,POKESIZE_HEIGHT);
	w = W_GetPokeSize(AttackClient,POKESIZE_WIDTH);
	if(h > w){
		act->work[7] = h / 2;
	}else{
		act->work[7] = w / 2;
	}

	act->dx = CosMove(act->work[6], act->work[7]);
	act->dy = SinMove(act->work[6], act->work[7]);

	act->move = RyunoMaiActMove;
}
static void RyunoMaiActMove(actWork *act)
{
	switch(act->work[0]){
	case 0:
		act->work[6] -= act->work[5];
		act->work[6] &= 0xff;

		act->dx = CosMove(act->work[6], act->work[7]);
		act->dy = SinMove(act->work[6], act->work[7]);

		if(++(act->work[4]) > RYUNOMAI_SP_ADD_WAIT){
			act->work[4] = 0;
			if(act->work[5] < RYUNOMAI_SP_MAX){
				act->work[5] += RYUNOMAI_SP_ADD;
				if(act->work[5] >= RYUNOMAI_SP_MAX){
					act->work[5] = RYUNOMAI_SP_MAX;
				}
			}
		}

		if(++(act->work[3]) > 60)
		{
			act->work[3] = 0;
			act->work[0]++;
		}
		break;

	case 1:
		act->work[6] -= act->work[5];
		act->work[6] &= 0xff;

		if(act->work[7] < RYUNOMAI_R_MAX){
			act->work[7] += 8;
			if(act->work[7] >= RYUNOMAI_R_MAX){
				act->work[7] = RYUNOMAI_R_MAX;
			}
		}
		

		act->dx = CosMove(act->work[6], act->work[7]);
		act->dy = SinMove(act->work[6], act->work[7]);

		if(++(act->work[4]) > RYUNOMAI_SP_ADD_WAIT){
			act->work[4] = 0;
			if(act->work[5] < RYUNOMAI_SP_MAX){
				act->work[5] += RYUNOMAI_SP_ADD;
				if(act->work[5] >= RYUNOMAI_SP_MAX){
					act->work[5] = RYUNOMAI_SP_MAX;
				}
			}
		}

		if(++(act->work[3]) > 20)
		{
			DelWazaEffect(act);
		}
		break;
	}
}
const actHeader RyuNoMaiHeader = {
	WAZACHR_ID_HUUIN,
	WAZACHR_ID_HUUIN,
	&WazaOamDataNormal[1],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	RyuNoMaiActInit,
};

//==============================================
// ׽����۰�
//==============================================
#define RYUNOMAI_SIN_SPEED  (9)
#define RYUNOMAI_SIN_STEP   (8)
#define RYUNOMAI_SINRATIO_MAX  (3)

enum RYU_WORK {
	RW_SEQ,
	RW_TIMER,
	RW_SCRX,
	RW_TOPLINE,
	RW_BOTTOMLINE,
	RW_SINPOS,
	RW_SINRATIO,
	RW_SINRATIO_WAIT,
};
static void TaskRyunomaiMain(u8 id);
static void ryunomai_laster_set(TASK_TABLE *my);

void TaskRyunomai(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];
	LasterData  LD;
	u16 i;

	if(GetPokeBGNo(AttackNo)==1){
		LD.DestAdrs=(void *)REG_BG1HOFS;
		my->work[RW_SCRX] = ScrX1;
	}else{
		LD.DestAdrs=(void *)REG_BG2HOFS;
		my->work[RW_SCRX] = ScrX2;
	}
	LD.DMAControl=H_DMA_ENABLE16;
	LD.TransType=LASTER_NORMAL;
	LD.LoopCount=0;

	my->work[RW_TOPLINE] = GetPokeShadowYPos(AttackNo) - 32;
	my->work[RW_BOTTOMLINE] = my->work[RW_TOPLINE] + 64;
	if(my->work[RW_TOPLINE] < 0){
		my->work[RW_TOPLINE] = 0;
	}

	for(i = my->work[RW_TOPLINE]; i <= my->work[RW_BOTTOMLINE]; i++)
	{
		LasterBuffer[0][i] = my->work[RW_SCRX];
		LasterBuffer[1][i] = my->work[RW_SCRX];
	}

	LasterDmaSet(LD);

	my->TaskAdrs = TaskRyunomaiMain;
}

static void TaskRyunomaiMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[RW_SEQ]){
	case 0:
		if(++(my->work[RW_SINRATIO_WAIT]) > 1){
			my->work[RW_SINRATIO_WAIT] = 0;
			if(++(my->work[RW_SINRATIO]) == RYUNOMAI_SINRATIO_MAX){
				my->work[RW_SEQ]++;
			}
		}
		ryunomai_laster_set(my);
		break;

	case 1:
		if(++(my->work[RW_TIMER]) > 60){
			my->work[RW_SEQ]++;
		}
		ryunomai_laster_set(my);
		break;

	case 2:
		if(++(my->work[RW_SINRATIO_WAIT]) > 1){
			my->work[RW_SINRATIO_WAIT] = 0;
			if(--(my->work[RW_SINRATIO]) == 0){
				my->work[RW_SEQ]++;
			}
		}
		ryunomai_laster_set(my);
		break;

	case 3:
		LasterSys.TransType = LASTER_END;
		my->work[RW_SEQ]++;
		break;

	case 4:
		DelTaskEffect(id);
		break;
	}

}
// LasterBuffer ���
static void ryunomai_laster_set(TASK_TABLE *my)
{
	u16 i, pos;

	pos = my->work[RW_SINPOS];
	for(i = my->work[RW_TOPLINE]; i<= my->work[RW_BOTTOMLINE]; i++)
	{
		LasterBuffer[LasterSys.BufFlag][i] = my->work[RW_SCRX] + ((sin_cos_table[pos] * my->work[RW_SINRATIO]) >> 7);
		pos = (pos + RYUNOMAI_SIN_STEP)&0xff;
	}
	my->work[RW_SINPOS] = (my->work[RW_SINPOS] + RYUNOMAI_SIN_SPEED) & 0xff;
}


//=========================================================================
//
//		�I�[�o�[�q�[�g
//														mori 2002.07.30
//=========================================================================
static void SeqWazaOverHeatInit(actWork *Xreg);
static void SeqWazaOverHeatMain(actWork *Xreg);
const	actHeader ActOverHeatHeader={
	WAZACHR_ID_FIRE,
	WAZACHR_ID_FIRE,
	&WazaOamDataNormal[2],
	GekihiAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaOverHeatInit,
};

static s16 DebugEffWork[7];
//======================================================================
//
//	���s���̂���L��������鉊
//	
//	EffWork[0]	���h�b�g�ڂ���J�n��
//	EffWork[1]	�i��CosSin�p�x(0-255)
//	EffWork[2]	�i�ޑ��x�i�����܂Ŗڈ��j
//	EffWork[3]	��SYNC�����邩
//	EffWork[4]  Yoffset
//======================================================================
static void SeqWazaOverHeatInit(actWork *Xreg)
{
	s32 wy,i;
	wy            = (WazaEffWork[2]*3)/5;
	Xreg->x       = GetWazaEffPos(AttackNo, PM_X);
	Xreg->y       = GetWazaEffPos(AttackNo, PM_Y)+WazaEffWork[4];
	Xreg->work[1] = CosMove(WazaEffWork[1],WazaEffWork[2]);
	Xreg->work[2] = SinMove(WazaEffWork[1],wy);
	Xreg->x      += Xreg->work[1] * WazaEffWork[0];
	Xreg->y      += Xreg->work[2] * WazaEffWork[0];
	Xreg->work[3] = WazaEffWork[3];
	Xreg->move    = SeqWazaOverHeatMain;

	for(i=0;i<7;i++) DebugEffWork[i] = Xreg->work[i];
}

static void SeqWazaOverHeatMain(actWork *Xreg)
{
	Xreg->work[4] += Xreg->work[1];
	Xreg->work[5] += Xreg->work[2];

	Xreg->dx = Xreg->work[4]/10;
	Xreg->dy = Xreg->work[5]/10;
	
	if(++Xreg->work[0] > Xreg->work[3]){
		DelWazaEffect(Xreg);
	}
}
