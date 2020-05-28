//=========================================================================
//	W_ParallelMoveSeq2-W_AffCheckEndSeq�����s		by nohara 01/05/30
//
//	���������X�s�� ���낪�� �܂邭�Ȃ�
//	��邠���� ��񂼂��p���`
//	�݂��� �݂˂��� ���΂��
//	�}�b�n�p���` �ɂǂ��� �Ƃт��� �ƂтЂ�����
//	�����̂ڂ� ���������� �N���u�n���}�[ ��������
//	������� �����݂킯 �{�[�����b�V�� �����񂮂��
//	�Ƃ����� ���K�g���p���` ���K�g���L�b�N
//	���΂� �����΂��͂� �����Ƃ�
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
#include "task.h"
#include "palanm.h"
#include "ef_tool.h"
#include "cli_def.h"

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
void SeqWazaMineInit(actWork *Xreg);	//�����̕��ɂł�
void SeqWazaEnemyInit(actWork *Xreg);	//����̕��ɂł�
void SeqWazaMikiriInit(actWork *Xreg);

//----------------------------------------------------------
//	�萔��`
//----------------------------------------------------------
#define MIKIRI_WAIT		3
#define MINEUTI_WAIT	5

//����
//Mine Hflip��Attack��Enemy�̎����Ă�
//Enemy ActAnmChg��WazaEffWork[6]�ɂ���Ă�

//----------------------------------------------------------
//	tama���낪��̋�	tama2�܂邭�Ȃ�̋�	tama3�E�F�U�[�{�[��
//----------------------------------------------------------
static const actAnm moveTamaAnm[]={
	{OBJ16_16x16 * 0, 3, 0, 0},
	{OBJ16_16x16 * 1, 3, 0, 0},
	{OBJ16_16x16 * 2, 3, 0, 0},
	{OBJ16_16x16 * 3, 3, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm *const moveTamaAnmTbl[]={
		moveTamaAnm,
};
const	actHeader ActMoveTamaHeader={
	WAZACHR_ID_TAMA,
	WAZACHR_ID_TAMA,
	&WazaOamDataNormal[1],
	moveTamaAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaEnemyInit,
};
const	actHeader ActMoveTama2Header={
	WAZACHR_ID_TAMA,
	WAZACHR_ID_TAMA,
	&WazaOamDataNormal[1],
	moveTamaAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaMineInit,
};


//----------------------------------------------------------
//
//	�E�F�U�[�{�[��
//
//----------------------------------------------------------
static const actAnm WeatherBallAnm[]={
	{OBJ16_32x32 * 0, 3, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm *const WeatherBallAnmTbl[]={
		WeatherBallAnm,
};

static void SeqWazaUpThrowInit(actWork *Xreg);
const	actHeader ActMoveTama3Header={
	WAZACHR_ID_WBALL,
	WAZACHR_ID_WBALL,
	&WazaOamDataNormal[2],
	WeatherBallAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaUpThrowInit,
};


void SeqWazaBallAttackInit(actWork *Xreg);
const	actHeader ActMoveTama4Header={
	WAZACHR_ID_WBALL,
	WAZACHR_ID_WBALL,
	&WazaOamDataNormal[2],
	WeatherBallAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaBallAttackInit,
};

//----------------------------------------------------------
//	�݂���̃L�� �݂˂����̃L��
//----------------------------------------------------------
static const actAnm MikiriAnm[]={
	{OBJ16_32x32 * 0, MIKIRI_WAIT, 0, 0},
	{OBJ16_32x32 * 1, MIKIRI_WAIT, 0, 0},
	{OBJ16_32x32 * 2, MIKIRI_WAIT, 0, 0},
	{OBJ16_32x32 * 3, MIKIRI_WAIT, 0, 0},
	{OBJ16_32x32 * 4, MIKIRI_WAIT, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm *const MikiriAnmTbl[]={
		MikiriAnm,
};
const	actHeader ActKiraHeader={
	WAZACHR_ID_L_HIKARI,
	WAZACHR_ID_L_HIKARI,
	&WazaOamDataNormal[2],
	MikiriAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaMikiriInit,
};


//----------------------------------------------------------
//	�L�b�N
//----------------------------------------------------------
const	actHeader ActKeriHeader={
	WAZACHR_ID_ANIMALLEG,
	WAZACHR_ID_ANIMALLEG,
	&WazaOamDataNormal[2],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaEnemyInit,
};

//----------------------------------------------------------
//	�_���[�W�̊G
//----------------------------------------------------------
const actAnm movehit_anm0[] =
{
	{OBJ16_32x32 * 0, 5, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
const actAnm movehit_anm1[] =
{
//	{OBJ16_32x32 * 1, 5, 0, 0},
	{OBJ16_32x32 * 0, 5, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
const actAnm movehit_anm2[] =
{
	{OBJ16_32x32 * 0, 5, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
const actAnm * const MovehitAnmTbl[] = 
{
	movehit_anm0,
	movehit_anm1,
	movehit_anm2,
};
// ��X�����͍폜����		2002.05.10(��) by matsuda
const actHeader ActMovehitHeader = 
{
	WAZACHR_ID_HITMARK,
	WAZACHR_ID_HITMARK,
	&WazaOamDataNormal[2],
	MovehitAnmTbl,
	ACT_NTRANS,		//��]���A�j��
	DummyActAffTbl,
	SeqWazaEnemyInit,
};

//----------------------------------------------------------
//	�����Ƃ�̋�
//----------------------------------------------------------
static const actAnm suitoruTamaAnm0[]={
	{OBJ16_16x16 * 0, 15, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm *const suitoruTamaAnmTbl[]={
		suitoruTamaAnm0,
};
static const actAffAnm suitoru_aff_anm0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0x60, 0x60, 0, ACT_AFSET },				// 
	{ 2, 2, 0, 1 },						// 
	{ACT_AFLOOP,1,0,0},					// ٰ�� , �߂��
};
static const	actAffAnm * const suitoru_aff_tbl[] = {
	suitoru_aff_anm0,
};
const	actHeader ActSuitoruTamaHeader={
	WAZACHR_ID_TAMA,
	WAZACHR_ID_TAMA,
	&WazaOamDataAffine2[1],
	suitoruTamaAnmTbl,
	0,
	suitoru_aff_tbl,
	SeqWazaMineInit,
};

//-------------------------------------------------------------
//  �A�N�^�[�V�[�P���X
//-------------------------------------------------------------
//-------------------------------------------------------------
//  �������ɂł�
//-------------------------------------------------------------
void SeqWazaMineInit(actWork *Xreg)
{
	EffPositionReset(Xreg);
	if(MineEnemyCheck(AttackNo))
	{
		Xreg->x -= WazaEffWork[0];
		WazaEffWork[3] =-WazaEffWork[3];
		Xreg->Hflip = 1;		//����
	}
	else
		Xreg->x += WazaEffWork[0];

	Xreg->y += WazaEffWork[1];

	Xreg->work[0] = WazaEffWork[2];		//LOOP
	Xreg->work[1] = WazaEffWork[3];		//work[2]�͌v�Z�p
	Xreg->work[3] = WazaEffWork[4];		//work[4]��
	Xreg->work[5] = WazaEffWork[5];		//�\��on,off�̊Ԋu

	SetWorkSeq(Xreg, W_AffCheckEndSeq);
	Xreg->move=W_ParallelMoveSeq2;

}

//-------------------------------------------------------------
//  ���葤�ɂł�
//-------------------------------------------------------------
void SeqWazaEnemyInit(actWork *Xreg)
{
	if(MineEnemyCheck(AttackNo))
	{
		Xreg->x -= WazaEffWork[0];
		WazaEffWork[3]*=-1;
	}
	else
		Xreg->x += WazaEffWork[0];

	Xreg->y += WazaEffWork[1];

	Xreg->work[0] = WazaEffWork[2];		//loop
	Xreg->work[1] = WazaEffWork[3];		//work[2]�͌v�Z�p
	Xreg->work[3] = WazaEffWork[4];		//work[4]��
	Xreg->work[5] = WazaEffWork[5];		//�\��on,off�̊Ԋu

	ActAnmChg(Xreg,WazaEffWork[6]);

	SetWorkSeq(Xreg, W_AffCheckEndSeq);
	Xreg->move=W_ParallelMoveSeq2;
}


//----------------------------------------------------------
//	add by matsuda
// offset 0:Xoffset  1:Yoffset
//----------------------------------------------------------
void SeqWazaMikiriInit(actWork *Xreg)
{
	EffPositionReset(Xreg);
//	EffXpositionSet(Xreg, WazaEffWork[0]);
	if(MineEnemyCheck(AttackNo))
		Xreg->x -= WazaEffWork[0];
	else
		Xreg->x += WazaEffWork[0];
	Xreg->y += WazaEffWork[1];
	Xreg->move = W_AnmEndWait;
	SetWorkSeq(Xreg, DelWazaEffect);
}


//======================================================================
//	�}�b�n�p���`
//																taya
//======================================================================
#define MACHPUNCH_SPD   (8)
#define MACHPUNCH_ZANZOU_PALID  (WAZACHR_ID_SPOON)  // �c���p����گ����ގ擾�p�ɁA�ԈႢ�Ȃ��g��Ȃ��ł��낤ID���g�p���Ă���
static void TaskMachPunchMain(u8 id);
static void machpunch_zanzou_add(TASK_TABLE *my, u8 task_id);

//==============================================
//	�������^�X�N�i0:�e�F  1:�eevy�j
//==============================================
void TaskMachPunch(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];
	u16 palno, poke_palno;

	my->work[0] = W_GetPokeActNo(0);  // 0:�߹�ݱ�������
	my->work[1] = MineEnemyCheck(AttackNo)? -MACHPUNCH_SPD : MACHPUNCH_SPD;  // 1:�ړ����x
	my->work[2] = 0;  // 2:���ݽ
	my->work[3] = 0;  // 3:�ړ�����
	ActWork[my->work[0]].dx -= my->work[0];

	
	my->work[4] = ObjPalNumSet(MACHPUNCH_ZANZOU_PALID); // 4:�c����گ�����
	my->work[5] = 0;  // 5:�c���̐�
	palno = (my->work[4] + 16) * 16;
	poke_palno = (ActWork[my->work[0]].oamData.Pltt + 16) * 16;

	my->work[6] = GetPokeSoftPri(AttackNo);
	if(my->work[6] == C_POKE2PRI || my->work[6] == CAI_POKE1PRI)
	{
		my->work[6] = 2;  // 6:�c����OAM��ײ��è
	}
	else
	{
		my->work[6] = 3;
	}

	CpuCopy(&PaletteWork[poke_palno], &PaletteWorkTrans[palno], 0x20, 32);
	SoftFade(palno, 16, WazaEffWork[1], WazaEffWork[0]);

	my->TaskAdrs = TaskMachPunchMain;

}
//==============================================
//	���C���^�X�N
//==============================================
static void TaskMachPunchMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[2]){
	case 0:
		machpunch_zanzou_add(my, id);
		ActWork[my->work[0]].dx += my->work[1];
		if(++(my->work[3]) == 5){
			my->work[3]--;
			my->work[2]++;
		}
		break;

	case 1:
		machpunch_zanzou_add(my, id);
		ActWork[my->work[0]].dx -= my->work[1];
		if(--(my->work[3]) == 0){
			ActWork[my->work[0]].dx = 0;
			my->work[2]++;
		}
		break;

	case 2:
		// �c���A�N�^�[���S�ď�������I��
		if(my->work[5] == 0){
			ObjPalRegNumDel(MACHPUNCH_ZANZOU_PALID);
			DelTaskEffect(id);
		}
		break;
	}
}
//==============================================================
//	�}�b�n�p���`�p�c���A�N�^�[���P�̕\��
//==============================================================
static void machpunch_zanzou_actmove(actWork *act);
static void machpunch_zanzou_add(TASK_TABLE *my, u8 task_id)
{
	s16 actno = W_PokeZanzouMake(0);
	if(actno < 0){ return; }
	ActWork[actno].oamData.Priority = my->work[6];
	ActWork[actno].oamData.Pltt = my->work[4];
	ActWork[actno].work[0] = 8;  // �c���̎���sync��
	ActWork[actno].work[1] = task_id;  // �����޸���Ď��̂���
	ActWork[actno].work[2] = actno;    // 
	ActWork[actno].dx = ActWork[my->work[0]].dx;
	ActWork[actno].move = machpunch_zanzou_actmove;
	my->work[5]++;
}
//==============================================================
//	�}�b�n�p���`�p�c���A�N�^�[�̓���֐�
//==============================================================
static void machpunch_zanzou_actmove(actWork *act)
{
	if(--(act->work[0]) == 0){
		TaskTable[act->work[1]].work[5]--;
		W_PokeZanzouDel(act);
	}
}



// =================================================================
// SeqWazaUpThrowMain
// �T�v  : �E�F�U�[�{�[����V����������i�E�F�U�[�{�[���p�����j
// ����  : Xreg
// �߂�l: none
// =================================================================
static void SeqWazaUpThrowMain(actWork *Xreg);

static void SeqWazaUpThrowInit(actWork *Xreg)
{
	Xreg->x = GetWazaEffPos(AttackNo,PM_X);
	Xreg->y = GetWazaEffPos(AttackNo,PM_Y);

	if(MineEnemyCheck(AttackNo) == SIDE_MINE){
		Xreg->work[0] =  5;
	}else{
		Xreg->work[0] = -10;
	}
	Xreg->work[1] = -40;
	Xreg->move = SeqWazaUpThrowMain;
}

static void SeqWazaUpThrowMain(actWork *Xreg)
{
	Xreg->work[2] += Xreg->work[0];
	Xreg->work[3] += Xreg->work[1];
	Xreg->dx  = Xreg->work[2]/10;
	Xreg->dy  = Xreg->work[3]/10;
//	if(Xreg->work[0]>0)	Xreg->work[0]++;
//	else				Xreg->work[0]--;
	if(Xreg->work[1] < -20)
		Xreg->work[1]++;

	if((Xreg->y+Xreg->dy)<-32)
		DelWazaEffect(Xreg);
}


// =================================================================
// SeqWazaBallAttackInit
// �T�v  : �����ē�����{�[��
// ����  : [0]:Xoffset [1]:YOffset [2]:Sync�� [3]:�U�������h�䑤��(0,1)
//		   [4]:�I��Xofset [5]:�I��Yoffset
// �߂�l: none
// =================================================================
#define HURI_X_HABA	(30)

static void SeqWazaBallAttackMain(actWork *Xreg);
 void SeqWazaBallAttackInit(actWork *Xreg)
{
//	if(Xreg->aff_tbl != DummyActAffTbl)
//		if(MineEnemyCheck(DefenceNo) == SIDE_MINE)
//			ActAffAnmChg(Xreg,1);

	Xreg->work[0] = WazaEffWork[2];
	Xreg->work[2] = Xreg->x + WazaEffWork[4];
	Xreg->work[4] = Xreg->y + WazaEffWork[5];

	if(MineEnemyCheck(DefenceNo) == SIDE_MINE){
		Xreg->x = Xreg->x+HURI_X_HABA+WazaEffWork[4];
		Xreg->y    = -20+WazaEffWork[5];
	}else{
		Xreg->x = Xreg->x-HURI_X_HABA+WazaEffWork[4];
		Xreg->y    = -80+WazaEffWork[5];
	}
	
	Xreg->move = W_StraightSyncMove;
	SetWorkSeq(Xreg, DelWazaEffect);
}

static void SeqWazaBallAttackMain(actWork *Xreg)
{
	
}

