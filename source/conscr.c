//=========================================================================
//
//		�R���e�X�g�̃X�N���v�g�Ŏg�p����֐�
//												by matsuda 2001.10.15(��)
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
#include "conscr.h"
#include "fld_main.h"
#include "consio.h"
#include "script.h"
#include "evobj.h"
#include "pokesio.h"
#include "multisio.h"
#include "cable.h"
#include "syswork.h"
#include "menutool.h"
#include "objdata.h"
#include "client_t.h"
#include "d_matsu.h"
#include "mus_tool.h"

//#include "../sxydata/t105_r0102.sym"
#include "../sxydata/sp_contest.sym"	//shimoyamada(2001.12.12)


//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void ContSyokikaReturn(u8 id);
static void RandTaneTrans(u8 id);
static void ContestTypeTrans(u8 id);
static void ContSyokikaEnd(u8 id);
//static void BCanKansi(u8 id);
static void ContestTypeGetCheck(u8 id);
static void ContSioEnd(u8 id);
static void ContSioEndInit(u8 id);
static void ContestVersionTrans(u8 id);
u8 ContestServerVisionCheck(u8 version[]);
static void ContestPokeCGTask( u8 no );
static void BrdSortInitDataTrans(u8 id);




//----------------------------------------------------------
//	�w�肵���R���e�X�g�ɎQ���ł��邩�o���Ȃ�������
// �����Ftype = �Q�������ýĂ̎��  rank = ��ýĂ��ݸ
//		 pp = �Q�������߹�݂����Ұ����ڽ
// �߂�l�F 1 = �Q���ł���	0 = �Q���ł��Ȃ�
//----------------------------------------------------------
void ContestSankaCheck(void)	//u8 type, u8 rank, PokemonParam *pp)
{
	u8 kekka;
	PokemonParam *pp;
	pp = &PokeParaMine[ConTemotiNo];

	kekka = ContestParticipationCheck(pp);

	if(kekka)
	{
		SetBreederData(ContestKind, ContestRank);	//�Q�������ýĂɂ�������ذ�ް�ް���Ă���
		GetBPAll(ContestKind);
//		ContestRank = rank;
//		ContestKind = type;
//		SetJudgeParam();
	}
	
//	return kekka;
	AnswerWork = kekka;
}

//----------------------------------------------------------
//	���ݎQ�����Ă�R���e�X�g�̃��{���������Ă��邩�`�F�b�N
//	�߂�l: 0=�����ĂȂ�	1=�����Ă�
//----------------------------------------------------------
u8 RibbonCheck(void)
{
	u8 kekka = 0;
	PokemonParam *pp;

	pp = &PokeParaMine[ConTemotiNo];
	switch(ContestKind)
	{
		case STYLE_CONTEST:				//�����߹�݂̊i�D�悳
			if(PokeParaGet(pp, ID_stylemedal) > ContestRank)
				kekka = 1;
			break;
		case BEAUTIFUL_CONTEST:			//�����߹�݂̔�����
			if(PokeParaGet(pp, ID_beautifulmedal) > ContestRank)
				kekka = 1;
			break;
		case CUTE_CONTEST:				//�����߹�݂̉���
			if(PokeParaGet(pp, ID_cutemedal) > ContestRank)
				kekka = 1;
			break;
		case CLEVER_CONTEST:				//�����߹�݂̌���
			if(PokeParaGet(pp, ID_clevermedal) > ContestRank)
				kekka = 1;
			break;
		case STRONG_CONTEST:				//�����߹�݂̂����܂���
			if(PokeParaGet(pp, ID_strongmedal) > ContestRank)
				kekka = 1;
			break;
	}
	return kekka;
}

//----------------------------------------------------------
//	�Q�����Ă�R���e�X�g���߂̃��{���׸ނ�Ă���
//----------------------------------------------------------
void RibbonSet(void)
{
	u8 ribbon;
	
	if(ContestJyunni[MyBreederNo] == 0)	//1�ʂ���Ȃ��Ȃ�͂���
	{
		switch(ContestKind)
		{
			case STYLE_CONTEST:				//�����߹�݂̊i�D�悳
				ribbon = PokeParaGet(&PokeParaMine[ConTemotiNo], ID_stylemedal);
				if(ribbon <= ContestRank && ribbon < 4){
					ribbon++;
					PokeParaPut(&PokeParaMine[ConTemotiNo], ID_stylemedal, &ribbon);
				}
				break;
			case BEAUTIFUL_CONTEST:			//�����߹�݂̔�����
				ribbon = PokeParaGet(&PokeParaMine[ConTemotiNo], ID_beautifulmedal);
				if(ribbon <= ContestRank && ribbon < 4){
					ribbon++;
					PokeParaPut(&PokeParaMine[ConTemotiNo], ID_beautifulmedal, &ribbon);
				}
				break;
			case CUTE_CONTEST:				//�����߹�݂̉���
				ribbon = PokeParaGet(&PokeParaMine[ConTemotiNo], ID_cutemedal);
				if(ribbon <= ContestRank && ribbon < 4){
					ribbon++;
					PokeParaPut(&PokeParaMine[ConTemotiNo], ID_cutemedal, &ribbon);
				}
				break;
			case CLEVER_CONTEST:			//�����߹�݂̌���
				ribbon = PokeParaGet(&PokeParaMine[ConTemotiNo], ID_clevermedal);
				if(ribbon <= ContestRank && ribbon < 4){
					ribbon++;
					PokeParaPut(&PokeParaMine[ConTemotiNo], ID_clevermedal, &ribbon);
				}
				break;
			case STRONG_CONTEST:			//�����߹�݂̂����܂���
				ribbon = PokeParaGet(&PokeParaMine[ConTemotiNo], ID_strongmedal);
				if(ribbon <= ContestRank && ribbon < 4){
					ribbon++;
					PokeParaPut(&PokeParaMine[ConTemotiNo], ID_strongmedal, &ribbon);
				}
				break;
		}
	}
}

//----------------------------------------------------------
//	���ذ���ް������ذ�ް�����擾����
// �����F entry_no = ���ذ���ް 	buff = �擾�������O������ܰ�
//----------------------------------------------------------
void GetBreederName()
{
	PM_strcpy( StrTempBuffer0, BreederParam[ScriptParameter2].name );
}

//----------------------------------------------------------
//	���ذ���ް������ذ�ް�̎����߹�݂�Ư�Ȱт��擾����
// �����F entry_no = ���ذ���ް 	buff = �擾�������O������ܰ�
//----------------------------------------------------------
void GetBreederNickName()
{
	PM_strcpy( StrTempBuffer2, BreederParam[ScriptParameter2].nickname );
}


//----------------------------------------------------------
//	���ذ���ް����ꎟ�R���̏��ʂ��擾����
// �����F entry_no = ���ذ���ް
// �߂�l�F����(0�ؼ��)
//----------------------------------------------------------
/*
u8 GetPrimaryRank(u8 entry_no)
{
	u8 i, counter;
	
	for(i = 0, counter = 0; i < BREEDER_MAX; i++)
	{
		if(ContBP[entry_no] < ContBP[i])
			counter++;
	}
	return counter;
}
*/
void GetPrimaryRank()
{
	u8 i, counter;
	
	for(i = 0, counter = 0; i < BREEDER_MAX; i++)
	{
		if(ContBP[ScriptParameter2] < ContBP[i])
			counter++;
	}
	ScriptParameter0 = (u16)counter;
	return;
}

//----------------------------------------------------------
//	���ذ���ް����BP�l���擾����
// �����F ScriptParameter2 = ���ذ���ް
// �߂�l�FScriptParameter0 = BP�l
//----------------------------------------------------------
void GetBPNum(void)
{
	ScriptParameter0 = ContBP[ScriptParameter2];
}

//=========================================================================
//	�D��������ذ�ް�̴��ذ���ް���擾����
//=========================================================================
void GetChampEntryNumber(void)
{
	u8 i;
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		if(ContestJyunni[i] == 0)
			break;
	}
	ScriptParameter1 = (u16)i;
	return;
}

//----------------------------------------------------------
//	�D��������ذ�ް�̖��O���擾����
// �����Fbuff = �擾�������O������ܰ�
//----------------------------------------------------------
void GetChampBreederName()
{
	u8 i;

	for(i = 0; i < BREEDER_MAX; i++)
		if(ContestJyunni[i] == 0)
			break;
	PM_strcpy( StrTempBuffer2, BreederParam[i].name );
}

//----------------------------------------------------------
//	�D�������߹�݂̖��O���擾����
// �����Fbuff = �擾�������O������ܰ�
//----------------------------------------------------------
void GetChampPokeName()
{
	u8 i;

	for(i = 0; i < BREEDER_MAX; i++)
		if(ContestJyunni[i] == 0)
			break;
	PM_strcpy( StrTempBuffer0, BreederParam[i].nickname );
}

//=========================================================================
//	2���R����ʌĂяo��
//=========================================================================
static void ContestYobidasi(void)
{
	MainProcChange(InitContest);
}

static void FadeWaitContestCall(u8 id)
{
	if(FadeData.fade_sw == 0)
	{
		DelTask(id);
		//ContestTuusin = 0;	//�ʐM�׸�OFF
		//InitContest();
//		sys.pMainProc = InitContest;
		MainProcChange(ContestYobidasi);
	}
}
void ContestNijiAppeal(void)
{
	SetForceEvent();	//�����C�x���g�׸޾��

	AddTask(FadeWaitContestCall, 10);
	PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
}

//=========================================================================
//	�G���g���[�i���o�[���珊�����Ă���|�P������monsno�����o��
//=========================================================================
/*
u16 GetMonsNo(u8 entry_no)
{
	return BreederParam[entry_no].monsno;
}
*/
void GetMonsNo()
{
	ScriptParameter0 = BreederParam[ScriptParameter2].monsno;
	return;
}

//=========================================================================
//	���ʔ��\��ʌĂяo��
//=========================================================================
static void FadeWaitContestResultCall(u8 id)
{
	if(FadeData.fade_sw == 0)
	{
		DelTask(id);
		MainProcChange(ContestResultInit);
	}
}
void ContestResultScr(void)
{
	SetForceEvent();	//�����C�x���g�׸޾��

	AddTask(FadeWaitContestResultCall, 10);
	PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
}


//=========================================================================
//	�ʐM���̎����̴��ذ���ް���擾����
//=========================================================================
void GetEntryNo(void)
{
	ScriptParameter0 = (u16)MyBreederNo;
	return;
}

//=========================================================================
//	�ʐM����4�䂪�I�񂾺�ýĂ̎�ނ������ƃR���e�X�g�ʐM������
// �����F�I����޳�őI�񂾈ʒu(0�ؼ��)
// �Ԃ�l�F�ʐM�������ScriptParameter0�ɒl������܂��B
//			0 = ����I��	1 = �I�񂾺�ýĂ̎�ނ��Ⴄ		2 = �ʐM���s
//=========================================================================
void ContestTuusinInit(u8 list_no)
{
	u8 id;
//	u8 kansi;
	SetForceEvent();	//�����C�x���g�׸޾��
	//�ʐM������
//	PokemonSioNinshouNo = NINSHOU_CONTEST;
	id = AddTask(ContestSioInit, 0);
	ChangeTask(id, ContestSioInit, ContSyokikaReturn);
	TaskTable[id].work[9] = list_no;
}

//-- ��ذ�ް�ް��]�� --//
static void ContSyokikaReturn(u8 id)
{
	SetMyBreederData(ConTemotiNo);//���̏���������ذ�ްNo������̂ŕK���������̌�ɌĂԎ��I
	ChangeTask(id, BreederTransData, RandTaneTrans);
}

//-- ����т��Ȃ�]�� --//
static void RandTaneTrans(u8 id)
{
//	ChangeTask(id, RandTaneTransData, ContestTypeTrans);
	ChangeTask(id, RandTaneTransData, ContestVersionTrans);
}

//-- ��ýĂ̻��ް�ް�ޮ݂�]�� --//
static void ContestVersionTrans(u8 id)
{
	ChangeTask(id, ContestVersionGetAllData, ContestTypeTrans);
}

//-- ��ý����߂�]�� --//
static void ContestTypeTrans(u8 id)
{
	ChangeTask(id, ContestTypeGetAllData, ContestTypeGetCheck);
}

//-- �]������Ă�����ý����߂����� --//
static void ContestTypeGetCheck(u8 id)
{
	u8 type[BREEDER_MAX];
	u8 i;
	u8 version[BREEDER_MAX];
	
	for(i = 0; i < BREEDER_MAX; i++)
		type[i] = TaskTable[id].work[1 + i];
	for(i = 0; i < BREEDER_MAX; i++)
	{
		if(type[0] != type[i])
			break;
	}
	if(i == BREEDER_MAX)
		ScriptParameter0 = 0;	//�I�񂾺�ý����߂�����
	else
		ScriptParameter0 = 1;	//�I�񂾺�ý����߂��Ⴄ
	//temp_rand = r_next;
	
	//-- ���ްVersion���� --//
	for(i = 0; i < BREEDER_MAX; i++){
		version[i] = TaskTable[id].work[5+i];
	}
	ContestSioOya = ContestServerVisionCheck(version);
	//-- Version�����I�� --//
	
	//-- ContBP�]�� ���K���e(��ԍŐV�̻��ް)�����܂��Ă����邱�ƁI --//
	GetBPAll(ContestKind);
//	ChangeTask(id, ContBPAllTransData, ContSyokikaEnd);
	ChangeTask(id, ContBPAllTransData, BrdSortInitDataTrans);
	
//	TaskTable[id].TaskAdrs = ContSyokikaEnd;
}

//-- �����BrdSort��]�� --//
static void BrdSortInitDataTrans(u8 id)
{
	BreederDataSort(0);
	ChangeTask(id, BrdSortInitAllTransData, ContSyokikaEnd);
}

//-- ���ް�ް�ޮ������F�߂�l�A�e�̒ʐMID --//
u8 ContestServerVisionCheck(u8 version[])
{
	int i;
	u8 oya;
	
	oya = 0;
	for(i = 1; i < BREEDER_MAX; i++){
		if(version[oya] < version[i]){
			oya = i;
			continue;
		}
		else if(version[oya] == version[i]){
			//����Version�Ȃ�ʐMID���Ⴂ�����Ȃ�
			continue;
		}
	}
	return oya;
}

//-- �ʐM�I�� --//
static void ContSyokikaEnd(u8 id)
{
	if(ScriptParameter0 == 1){	//�I�񂾺�ýĂ��Ⴄ�Ȃ�ʐM�ؒf
		if(SplitDataSendStatus())
			TaskTable[id].TaskAdrs = ContSioEndInit;
	}
	else{
//		DelTask(TaskTable[id].work[10]);	//B��݊Ď����
		DelTask(id);
		ResetForceEvent();	//����������׸މ���
		ContinueFieldScript();
	}
}

static void ContSioEndInit(u8 id)
{
	SetSioEndDataStart();
	TaskTable[id].TaskAdrs = ContSioEnd;
}

static void ContSioEnd(u8 id)
{
	if(pokemon_sio_flag == 0){
//		DelTask(TaskTable[id].work[10]);	//B��݊Ď����
		DelTask(id);
		//PokemonMultiSioEnd();
		
		ResetForceEvent();	//����������׸މ���
		ContinueFieldScript();
		//GetBPAll(ContestKind);
	}
}


#include "../script/savework.h"		//OBJCHRWORK�Q�Ƃ̂���
#include "sysflag.h"				//SVWK_START�Q�Ƃ̂���
//=========================================================================
//	�u���[�_�[�f�[�^�ɍ��킹���g���[�i�[��ׂ�\������
// �����Fmap = ��ýĉ���ϯ��ID    DIV = ��ýĉ���DIV ID
//		 site = ��׸�����ŏ��Ɍ����Ă������(0:��, 1:�� 2:�� 3:�E)
//=========================================================================
//void SetBreederObj(u8 map, u8 div)//, u8 site)
void SetBreederObj()//, u8 site)
{
	Fld.EventWork[OBJCHRWORK1-SVWK_START] = (u16)BreederParam[0].pattern;
	Fld.EventWork[OBJCHRWORK2-SVWK_START] = (u16)BreederParam[1].pattern;
	Fld.EventWork[OBJCHRWORK3-SVWK_START] = (u16)BreederParam[2].pattern;

	//�����������ɔz�u���Ă����΂悢
	/*
	EvObjChgSiteId(STUDYM1_01_SP_CONTEST, map, div, site);
	EvObjChgSiteId(FIGHTER1_01_SP_CONTEST, map, div, site);
	EvObjChgSiteId(GENTLEM1_01_SP_CONTEST, map, div, site);
	*/
}

//=========================================================================
//	�D�������u���[�_�[��OBJ ID���擾
//=========================================================================
void GetChampObjID(void)
{
	u8 i, id;
	
	//i = GetChampEntryNumber();
	i = (u8)ScriptParameter1;
	switch(i)
	{
		/*
		case 0:
			id = HERO_01_SP_CONTEST;
			break;
		case 1:
			id = HERO_02_SP_CONTEST;
			break;
		case 2:
			id = HERO_03_SP_CONTEST;
			break;
		default:
			id = HERO;
			break;
		*/

		case 0:
			id = STUDYM1_01_SP_CONTEST;
			break;
		case 1:
			id = FIGHTER1_01_SP_CONTEST;
			break;
		case 2:
			id = GENTLEM1_01_SP_CONTEST;
			break;
		default:
			id = HERO;
			break;
	}
	ScriptParameter0 = (u16)id;
	return;
}


//=========================================================================
//	���ذ���ް������ذ�ް�����擾����
//	���ذ���ް������ذ�ް�̎����߹�݂�Ư�Ȱт��擾����
//	�G���g���[�i���o�[���珊�����Ă���|�P������monsno�����o��
//=========================================================================
void SetBrdName(void)
{
	GetBreederName();
	GetBreederNickName();
	GetMonsNo();
}

//----------------------------------------------------------
//	���Q��������ýĎ�ނ̔��p�ٗp�̊G�����ɏ����Ă邩���ׂ�
// ScriptParameter0�Ɍ��ʂ�����܂��B
//	0 = �����(�����ĂȂ�)	1 = ���ɊG�������Ă�
//----------------------------------------------------------
void ArtMuseumPortraitCheck(void)
{
	u8 no;
	
	switch(ContestKind)
	{
		case STYLE_CONTEST:
			no = 8;
			break;
		case BEAUTIFUL_CONTEST:
			no = 9;
			break;
		case CUTE_CONTEST:
			no = 10;
			break;
		case CLEVER_CONTEST:
			no = 11;
			break;
		case STRONG_CONTEST:
		default:
			no = 12;
			break;
	}
	if(Fld.PortraitData[no].monsno == 0)
		ScriptParameter0 = 0;
	else
		ScriptParameter0 = 1;
}

//----------------------------------------------------------
//	���p�قɐV�����G���ް���Ă���
//----------------------------------------------------------
void ArtMuseumPortraitSet(void)
{
	PortraitDataSet(0xff);
}

//----------------------------------------------------------
//	Ͻ���ݸ�A�����_�ŗD�����������ׂ�
//	ScriptParameter0�Ɍ��ʂ�����܂�
//	0 = �����𖞂����ĂȂ�		1 = �S�Ă̏����𖞂����Ă�
//----------------------------------------------------------
void ArtMuseumCondCheck(void)
{
	if(ContestJyunni[MyBreederNo] == 0 && ContestRank == MASTER_RANK
		&& ContEvaluation[MyBreederNo] >= ARTMUSEUM_POINT)
	{
		ScriptParameter0 = 1;
	}
	else
		ScriptParameter0 = 0;
}

//----------------------------------------------------------
//	���p�قɉ����̊G�������Ă��邩���ׂ�
//  0�`5 (0����1���������ĂȂ�)
//----------------------------------------------------------
u8 ArtMuseumCountCheck(void)
{
	int i;
	u8 counter = 0;
	
	for(i = 8; i < 13; i++)
	{
		if(Fld.PortraitData[i].monsno != 0)
			counter++;
	}
	return counter;
}

//----------------------------------------------------------
//	BP���ʂ����߹�݂�Ư�ȰсA��ذ�ް�������o��
//	����:ScriptParameter2�ɉ��ʂ���ذ�ް�ް����擾���邩�n���Ă�������
//			(0:1��  1:2��  2:3��  3:4��)
//	�߂�l:�������ʂ̎�������̂ŁAScriptParameter2�ɉ��߂Č��݂̏��ʂ����Ȃ����ĕԂ��܂�
//			(0:1�ʁ@1:2�ʁ@2:3�ʁ@3:4�ʁ@4:����1�ʁ@5:����2�ʁ@6:����3�ʁ@7:����4��)
//			StrTempBuffer0�Ɏw�菇�ʂ��߹�݂�Ư�Ȱ�
//			StrTempBuffer1�Ɏw�菇�ʂ���ذ�ްȰ�
//			������܂��B
//----------------------------------------------------------
void GetBPBreederData(void)
{
	s16 TempBP[BREEDER_MAX];
	int i,j,t;
	s8 douritu;
	u8 MyJyunni, MyCount, tempMyCount;
	s16 MyBP;
	
	for(i = 0; i < BREEDER_MAX; i++)
		TempBP[i] = ContBP[i];
	
	for(i = 0; i < BREEDER_MAX - 1; i++){	//BP���~���Ƀ\�[�g
		for(j = BREEDER_MAX-1; j > i; j--){
			if(TempBP[j-1] < TempBP[j]){
				t = TempBP[j];
				TempBP[j] = TempBP[j-1];
				TempBP[j-1] = t;
			}
		}
	}
	
	MyBP = TempBP[ScriptParameter2];
	douritu = 0;
	MyCount = 0;
	for(i = 0; i < BREEDER_MAX; i++){
		if(TempBP[i] == MyBP){	//����BP�l�̐l�����l���邩����
			douritu++;			//����BP�l�������l����
			if(i == ScriptParameter2)
				MyCount = douritu;	//����BP�l���������̉��Ԗڂ�˯Ă��������ۑ�
		}
	}
	
	//-- �������l���������ۂ̏��ʂ͂����Ȃ̂����ׂ� --//
	for(i = 0; i < BREEDER_MAX; i++){
		if(TempBP[i] == MyBP)
			break;
	}
	MyJyunni = i;
	
	//-- �w�菇�ʂ���ذ�ް�ް����ް�����o�� --//
	tempMyCount = MyCount;
	for(i = 0; i < BREEDER_MAX; i++){
		if(MyBP == ContBP[i]){
			if(tempMyCount == 1)
				break;
			tempMyCount--;
		}
	}
	PM_strcpy(StrTempBuffer0, BreederParam[i].nickname);
	PM_strcpy(StrTempBuffer1, BreederParam[i].name);
	if(douritu == 1)	//�����̐l�����Ȃ�
		ScriptParameter2 = MyJyunni;
	else{				//�����̐l������
		if(MyCount == douritu)//�����̐l�����Ă������̕������ɂ������Ȃ�Ε��ʂ̏��ʂ�Ԃ�
			ScriptParameter2 = MyJyunni;
		else
			ScriptParameter2 = MyJyunni + BREEDER_MAX;
	}
}

//=========================================================================
//	����G�摜����ʂɕ\������
//=========================================================================
extern void contest_picture_init(void);

void AfterPutPortrait(void)
{
	MainProcChange(FieldScriptRecover);
}

void PortraitPicturePut(void)
{
	if(DebugContestFlag){
		KoutokutenDataSet();	//��t�őI���������߂̊G���\�������悤���ް����ľ��
		UserWork[TEMP_PORTRAIT_MSGFLG] = 1;	//����ł͓����ް����Ȃ��̂�0
		UserWork[TEMP_PORTRAIT_PLACE] = PortraitDataSpaceSearch(0xfe, 0);
		PortraitDataSet(MASTER_RANK);			//����G�ް��o�^
		DebugContestFlag = 0;
	}
	MainProcChange(contest_picture_init);
	sys.pReturnProc = AfterPutPortrait;
}


//=========================================================================
//	�R���e�X�g�ʐM���AOBJCHRWORK1�`��OBJ����ް����
//=========================================================================
void ContestSioObjChrSet(void)
{
	SetEventWorkValue(OBJCHRWORK1, BreederParam[0].pattern);
	SetEventWorkValue(OBJCHRWORK2, BreederParam[1].pattern);
	SetEventWorkValue(OBJCHRWORK3, BreederParam[2].pattern);
	SetEventWorkValue(OBJCHRWORK4, BreederParam[3].pattern);
}


//=========================================================================
//	Ͻ���ݸ�A�����_�ŗD�����A�Ȃ��������ϲ�����݂������Ă��邩�`�F�b�N���Č��ʂ�Ԃ�
//
//	�S�Ă̏����𖞂����Ă��āA���ϲ�����݂������Ă��Ȃ��Ȃ�A���ϲ�����݂�Ă���
//	1��Ԃ��B
//�@�����𖞂����Ă��Ȃ��A���͊��Ƀu���}�C�h���{���������Ă���ꍇ��0��Ԃ��B
//=========================================================================
u8 BromideRibbonCheckSet(void)
{
	u8 ribon;
	
	ribon = PokeParaGet(&PokeParaMine[ConTemotiNo], ID_bromide_ribbon);
	if(ribon)
		return 0;
	
	if(ContestJyunni[MyBreederNo] == 0 && ContestRank == MASTER_RANK
		&& ContEvaluation[MyBreederNo] >= ARTMUSEUM_POINT)
	{
		ribon = 1;
		PokeParaPut(&PokeParaMine[ConTemotiNo], ID_bromide_ribbon, &ribon);
		return 1;
	}
	else
		return 0;
}

//=========================================================================
//	�f�o�b�N�t���O�������Ă�����1��Ԃ�
//	�����Ă��Ȃ�����0��Ԃ�
//=========================================================================
u8 CheckDebugContestFlag(void)
{
	return DebugContestFlag;
}



//=========================================================================
//	��ýĂł̃|�P����CG�\���Z�b�g
//	ScriptParameter2�ɕ\������u���[�_�[�̴��ذNo�������Ă���
//=========================================================================
void ContestPokeCGWinOpen(void)
{
	u8 no,act;
	u16 monsno;
	u32 personal_rnd;
	PalData *PD;
	u32 id_no;
	s16 x,y;
	
	if( CheckTaskNo( ContestPokeCGTask ) != NOT_EXIST_TASK_ID )
		return;

	x = 10;
	y = 3;
	NMenuWinBoxWrite( x, y, x+9, y+10 );
	
	monsno = BreederParam[ScriptParameter2].monsno;
	personal_rnd = BreederParam[ScriptParameter2].personal_rnd;
	id_no = BreederParam[ScriptParameter2].id_no;
	no = AddTask( ContestPokeCGTask, 80 );
	TaskTable[no].work[0] = 0;
	TaskTable[no].work[1] = monsno;

//	act = FePokeSet( mons_no, (x+1)*8+32, (y+1)*8+32, POKECG_ACTPRI );
	DecordPokeCellMakeBuchi( &PM2FObjDataCell[monsno],
		pm2fpara[monsno].size, pm2fpara[monsno].patcnt,
		(void *)DecordAdrs[0],
		(void *)DecordAdrs[1], monsno, personal_rnd);
	//DecordPalSet( (PalData*)&PM2NObjDataPal[no] );
	PD = (PalData*)PokePalDataGetPara(monsno, id_no, personal_rnd);
	DecordPalSet(PD);
	PokeActHeaderMake(monsno, 1 );
	PokeActHeader.palNo = PD->reg_num;
	act = AddActor(&PokeActHeader, (x+1)*8+32, (y+1)*8+32, 0);

	TaskTable[no].work[2] = act;
	TaskTable[no].work[3] = x;
	TaskTable[no].work[4] = y;
	ActWork[act].move = DummyActMove;
	ActWork[act].oamData.Priority = 0;
};

//----------------------------------------------------------
//	�|�P����CG�\����Del����
//----------------------------------------------------------
void ContestPokeCGWinClose( void )
{
	u8 res;
	
	res = CheckTaskNo( ContestPokeCGTask );
	if( res == NOT_EXIST_TASK_ID )
		return;
	TaskTable[res].work[0]++;
}

//----------------------------------------------------------
//	CGwin�|�P�����Ǘ����
//----------------------------------------------------------
static void ContestPokeCGTask( u8 no )
{
	TASK_TABLE * my;
	actWork *act;
	
	my = &TaskTable[no];
	switch( my->work[0] )
	{
	case 0:
		my->work[0]++;
		break;

	case 1:
		/* �O�����瑀�삳���܂ł��̂܂� */
		break;

	case 2:
		//FePokeDel( &ActWork[ my->work[2] ] );
		act = &ActWork[my->work[2]];
		ObjPalRegNumDel( ObjPalNumIdGet(act->oamData.Pltt) );
		if( act->oamData.AffineMode )
		{
			AffineWorkNumDel( act->oamData.AffineParamNo );
		}
		DelActor( act );

		my->work[0]++;
		break;
	
	case 3:
		//NMenuBoxClear( POKECG_WINX1, POKECG_WINY1, POKECG_WINX2, POKECG_WINY2 );
		NMenuBoxClear( my->work[3], my->work[4], my->work[3]+9, my->work[4]+10 );
		DelTask( no );
		break;
	};
};


//=========================================================================
//	�ʐM���ɖ炷�Ȃ��׸ނ�Ԃ�
//=========================================================================
void ContestMusSet(void)
{
	if(ContestTuusin & C_TUUSIN_FLG){
		AnswerWork = GetSioConnectId();
	}
	else{
		AnswerWork = 4;
	}
}


//=========================================================================
//	1���R���̉�ʂŎg�p���Ă��郉���_���̃^�l��ʐM���ł����L�o����悤�ɂ���
//	AnsWork�Ƀ����_���͈̔͂���������ԂŌĂ΂�AAnsWork�Ɍ��ʂ����̂܂ܕԂ�
//=========================================================================
void GetContRnd(void)
{
	u16 rand;
	
	if(ContestTuusin & C_TUUSIN_FLG){
		temp_rand = temp_rand *1103515245L + 24691;
		rand = (u16)(temp_rand / 65536L);
		AnswerWork = rand % AnswerWork;
	}
	else
		AnswerWork = pp_rand() % AnswerWork;
}

