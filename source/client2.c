
//=================================================================
//    client2.c                                           
//                                                      
//    Copyright (C) 2000 GAME FREAK inc.
//=================================================================

#include "common.h"
#include "bss.h"		//add
#include "calctool.h"
#include "ef_tool.h"
#include "poketool.h"		//add

#include "client.h"
#include "client_t.h"
#include "decord.h"
#include "objdata.h"
#include "actanm.h"
#include "actor.h"
#include "cli_def.h"
#include "a_mine.h"
#include "cli_ef.h"
#include "pm_aff.h"
#include "server.h"
#include "a_status.h"
#include "waza_eff.h"
#include "task.h"
#include "wazano.def"
#include "mus_tool.h"


//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
void CA_PokeAppearWait(actWork *actst);
void CAS_TrainerSlideIn(actWork *actst);
void CAC_PokeAppearDifferentInit(actWork *actst, u8);
static u8 MigawariFightEffPutChk(u8 eff_no);

//=========================================================================
//	�O���ް��錾
//=========================================================================

//=========================================================================
//	�f�[�^
//=========================================================================
extern actHeader TRBTbl[];

//=========================================================================
//
//	�v���O����
//
//=========================================================================


//=========================================================================
//	CA = �ײ��ı���
//	CAC = �ײ��ı������(�ײ��ı������纰ق����ٰ��)
//	CAS = �ײ��ı������(�ײ��ı����ű��ޱ������ꂽ������move�֐�)
//=========================================================================

//=========================================================================
//	�߹�� �ްق���o��
// �I�������Ƃ���ActWork[�߂�l].move == DummyActMove�ɂȂ�
//=========================================================================
//----------------------------------------------------------
//	�̪�ďI���܂ŊĎ�
//----------------------------------------------------------
void CA_PokeAppearWait(actWork *actst)
{
	u8 pokeact;
	
	pokeact = actst->work[1];
//	if(ActWork[actst->work[0]].act_sw == 0 && ActWork[pokeact].affend_sw == 1)
	if(ActWork[pokeact].affend_sw == 1 && ActWork[pokeact].banish == 0)
	{
		if(ActWork[pokeact].anm_pause == 1)	//�߹�ݱ�Ұ��݂܂�
		{
			ActWork[pokeact].anm_pause = 0;	//�߹�ݱ�ҊJ�n
		}
		else if(ActWork[pokeact].anmend_sw == 1)	//�߹�ݱ�Ұ��ݏI��
		{
//			AllDelActor(&ActWork[actst->work[0]]);	//�ݽ���ް�DEL
			ActWork[pokeact].move = SeqMinePokeWaitInit;
			ActAffAnmChg(&ActWork[pokeact], PAB_NORMAL);
			actst->move = DummyActMove;
		}
	}
}
//----------------------------------------------------------
//	�߹�݂̱������g�k�p�ɕύX
// front_back = 0:��O(�����|�P����)�@�@1:��(�G�|�P����)
//----------------------------------------------------------
void CAC_PokeAppearDifferentInit(actWork *actst, u8 front_back)
{
	actst->anm_pause = 1;
	actst->move = DummyActMove;
//	actst->oamData.AffineMode = 1;
//	actst->aff_tbl = BallPokeAffTbl;
//	AffineNumSet(actst);
	if(front_back == 0)
		ActAffAnmChg(actst, PAB_BALLOUT);
	else
		ActAffAnmChg(actst, PAF_BALLOUT);
	ActAnm(actst);			//���قł͂��̌��JumpActor��ʂ�Ȃ�����
}

//=========================================================================
//	�ڰŰ�o��̪��
//=========================================================================
//----------------------------------------------------------
//	�ڰŰ�ײ�ވړ�
//----------------------------------------------------------
void CAS_TrainerSlideIn(actWork *actst)
{
	if(EncountEffectFlag&ENCOUNT_EFFECT_FLAG_OBJSTOP) return;	//�ړ��̃X�g�b�p�[
	actst->dx += actst->work[0];
	if(actst->dx == 0)
	{
		actst->move = DummyActMove;
	}
}

















//=========================================================================
//
//		�S�N���C�A���g���ʃ��[�`��
//												by matsuda 2002.05.29(��)
//=========================================================================

//----------------------------------------------------------
//	�X�e�[�^�X�ُ�̪�Ă�Ă���	���S�ײ��ċ���ٰ��
//----------------------------------------------------------
void ClientStatusEffSet(u8 condition_flg, u32 condition)
{
	CliSys[ClientNo].status_eff = 1;
	
	if(condition_flg == 0){
		if(condition == CONDITION_KOORI)
			//SetKooriStatusEffect(ClientNo);
			SetStatusEffect(ClientNo, STEFF_KOORI);
		else if(condition == CONDITION_DOKU || (condition & CONDITION_DOKUDOKU))
			SetStatusEffect(ClientNo, STEFF_DOKU);
		else if(condition == CONDITION_YAKEDO)
			SetStatusEffect(ClientNo, STEFF_YAKEDO);
		else if(condition & CONDITION_NEMURI)
			SetStatusEffect(ClientNo, STEFF_NEMURI);
		else if(condition == CONDITION_MAHI)
			SetStatusEffect(ClientNo, STEFF_MAHI);
		else
			CliSys[ClientNo].status_eff = 0;
	}
	else
	{	//condition2
		if(condition & CONDITION2_MEROMERO)		//condition2
			SetStatusEffect(ClientNo, STEFF_MEROMERO);
		else if(condition & CONDITION2_KONRAN)
			SetStatusEffect(ClientNo, STEFF_KONRAN);
		else if(condition & CONDITION2_NOROI)
			SetStatusEffect(ClientNo, STEFF_NOROI);
		else if(condition & CONDITION2_AKUMU)
			SetStatusEffect(ClientNo, STEFF_AKUMU);
		else if(condition & CONDITION2_SHIME)
			SetStatusEffect(ClientNo, STEFF_SIMETUKE);
		else
			CliSys[ClientNo].status_eff = 0;
	}
}











//=========================================================================
//
//		�퓬�G�t�F�N�g�Z�b�g(�T�[�o�[����Ă΂��Z�̪�āA�ð���ُ�̪�ĈȊO�̴̪��)
//												by matsuda 2002.07.02(��)
//=========================================================================
extern u32 FightEffSeqTbl[];

static void FightEffectEndCheck(u8 id);

u8 SetFightEffect(u8 client_no, u8 attack, u8 defence, u8 eff_no, u16 data)
{
	u8 id;
	u8	ret=0;
	
	//�Ă񂫂�̃G�t�F�N�g�ł݂���肪�o�Ă���Ƃ��́AAnmNo����������ăG�t�F�N�g�͏o���Ȃ�
	if((eff_no==FE_ID_HENGE)&&(data&BW_MIGAWARI_FLAG)){
		BattlePokeAnmNo[client_no]=(data&BW_MIGAWARI_FLAG_OFF);
		return 1;
	}
	//�g���莞�́A��g���跬׋C��̪�āv�ȊO�͏o���Ȃ� 2002.08.27(��) matsuda
	if(Climem[client_no].migawari == 1 && MigawariFightEffPutChk(eff_no) == 0)
		return 1;
	//�g�����ԂŁA��Ư���׸ނ������Ă��ċC��G�t�F�N�g�Ȃ�΁A�̪�Ă͏o���Ȃ�
	if(Climem[client_no].migawari == 1 && eff_no == FE_ID_MIGAWARI_KIZETU
			&& ActWork[PokemonStruct[client_no]].banish){
		MigawariChange(client_no, 1, PokemonStruct[client_no]);
		MigawariFlgReset(client_no);
		return 1;
	}
	
	AttackNo = attack;
	DefenceNo = defence;
	
	BtlWork->work[0] = data;
	WazaEffectCommonSet(FightEffSeqTbl, eff_no, 0);
	id = AddTask(FightEffectEndCheck, 10);
	TaskTable[id].work[0] = client_no;
	CliSys[TaskTable[id].work[0]].fight_eff = 1;

	return	ret;
}

static void FightEffectEndCheck(u8 id)
{
	WazaEffectSequence();
	if(WazaEffectFlag == 0){
		CliSys[TaskTable[id].work[0]].fight_eff = 0;
		DelTask(id);
	}
}

//-- �g���肪�o�Ă��鎞�ɏo���Ă����Z���`�F�b�N --//
static u8 MigawariFightEffPutChk(u8 eff_no)
{
	switch(eff_no){
		case FE_ID_MIGAWARI_KIZETU:
		case FE_ID_AMAGOI:
		case FE_ID_NIHONBARE:
		case FE_ID_SUNAARASI:
		case FE_ID_ARARE:
		case FE_ID_YOKODORI:
			return 1;
		default:
			return 0;
	}
}







//=========================================================================
//
//		���̑��̐퓬�G�t�F�N�g�Z�b�g(���ٱ��߂��ްٹޯĂȂ�)
//												by matsuda 2002.07.02(��)
//=========================================================================
extern u32 EtcEffSeqTbl[];

static void EtcEffectEndCheck(u8 id);

void SetEtcEffect(u8 client_no, u8 attack, u8 defence, u8 eff_no)
{
	u8 id;
	
	AttackNo = attack;
	DefenceNo = defence;
	
	WazaEffectCommonSet(EtcEffSeqTbl, eff_no, 0);
	id = AddTask(EtcEffectEndCheck, 10);
	TaskTable[id].work[0] = client_no;
	CliSys[TaskTable[id].work[0]].etc_eff = 1;
}

static void EtcEffectEndCheck(u8 id)
{
	WazaEffectSequence();
	if(WazaEffectFlag == 0){
		CliSys[TaskTable[id].work[0]].etc_eff = 0;
		DelTask(id);
	}
}







//=========================================================================
//
//		�Z�̪�Ă��o���Ă��������`�F�b�N
//												by matsuda 2002.07.05(��)
//=========================================================================
//----------------------------------------------------------
//	�߂�l�F0=�Z���o���Ă�OK	1=�Z���o���Ă͑ʖ�
//----------------------------------------------------------
u8 PutWazaEffectCheck(u16 wazano, u8 waza_counter)
{
#if 0	// Server�ŊǗ�����悤�ɂȂ��� 2002.07.25(��)
	u8 ret = 0;
	
	//�����߹�݂���Ư����ԂȂ�̪�Ă��o���Ȃ�
	if(ActWork[PokemonStruct[DefenceClient]].banish == 1){
		if(wazano == WAZANO_SORAWOTOBU || wazano == WAZANO_ANAWOHORU){
			if(waza_counter == 0)
				ret = 0;	//����ɍU������킯�ł͂Ȃ��̂ŏo����
			else
				ret = 1;
		}
		else
			ret = 1;
	}
	return ret;
#else
	return 0;
#endif
}







//=========================================================================
//
//		SE�����Ă��鎞�̏I���҂�(��ѱ�ď�������)
//	�߂�l: 1=SE�����Ă���  0=SE�����Ă��Ȃ�(��ѱ�Ċ܂�)
//												by matsuda 2002.09.01(��)
//=========================================================================
u8 ClientSEWait(u8 client_no)
{
	u8 wait = 0;
	
	if(SeEndCheck()){
		CliSys[client_no].setime_wait++;
		if(CliSys[ClientNo].setime_wait < EFF_SETIME_OUT_COUNT)
			wait = 1;
		else{
			m4aMPlayStop( &m4a_mplay001 );
			m4aMPlayStop( &m4a_mplay002 );
		}
	}
	
	if(wait)	//SE����I���܂ő҂�
		return 1;
	CliSys[client_no].setime_wait = 0;
	return 0;
}

