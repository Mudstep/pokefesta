//=========================================================================
//
//		�R���e�X�g�ʐM�֘A�֐�
//												by matsuda 2001.10.22(��)
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
#include "conscr.h"
#include "fld_task.h"
#include "fld_main.h"
#include "multisio.h"
#include "pokesio.h"
#include "cable.h"
#include "consio.h"

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void SioFlagWait(u8 id);
//static void BreederDataGetWait(u8 id);
void RandTaneTransData(u8 id);
//static void RandTaneTransToSyncWait(u8 id);
//static void RandTaneGetWait(u8 id);
void ContWazaNoSioGet(u8 id);
//static void ContWazaNoGetWait(u8 id);
void EvaluationSioGet(u8 id);
//static void EvaluationGetWait(u8 id);
//static void ContestTypeGetAllWait(u8 id);
//static void WazaNoOkurimakuriEnd(u8 id);
static void Sio10SyncWait(u8 id);
#if CONTEST_SIOINIT_DEL
static void ConnectNumCheck(u8 id);
#endif

//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern void kanshi_network(u8 taskno);
extern void DebugNumPrintInit(u8 pal, u8 select, u8 screen, u8 chara);
extern void kanshi_num_set(u8 w0, u8 w1);

//----------------------------------------------------------
//	�O���ϐ�
//----------------------------------------------------------
extern u8 SendPoolBuf[];





//----------------------------------------------------------
//	�c�[����
//----------------------------------------------------------

//=========================================================================
//	�ʐM�����ް���Ă��đ��M����(�S������)
// src = �����ް��̱��ڽ	size = �]���޲Ļ���
//=========================================================================
static void SetContestSendData(u8 *src, u16 size)
{
	u8 *buff;
	
	buff = &UserWork[0x1e000];
	memcpy(&buff[0], src, size);

	SplitDataSendStart(GetAnotherSendTo(), 
		&UserWork[0x1e000], size);
}

//=========================================================================
//	�w��ID����̃f�[�^����M����������
// connect_id = ���M�҂̒ʐMID
// �߂�l�F1 = �ް���M   0 = �ް����M
//=========================================================================
static u8 ReceptionDataCheck(u8 connect_id)
{
	u8 send_id;
	
	send_id = 1 << connect_id;
	
	if((SplitDataResult() & send_id) == 0)
		return 0;
	
	SplitDataResultReset2(send_id);
	return 1;
}

//=========================================================================
//	�S�������ް�����M�ł���������
//	�߂�l: 1 = �S�����ް���M����		0 = �܂��S�����ް�����M�ł��ĂȂ�
//=========================================================================
u8 GetAllSendData(void)
{
	int i;
	
	for(i = 0; i < BREEDER_MAX; i++){
		if((SplitDataResult() & (1 << i)) == 0)
			return 0;	//�S�������ް�����M�ł��ĂȂ��Ȃ�I��
	}
	SplitDataResultReset();
	return 1;
}












//=========================================================================
//
//		
//												by matsuda 2002.03.05(��)
//=========================================================================

//----------------------------------------------------------
//	�^�X�N�V�[�P���X
//----------------------------------------------------------

//=========================================================================
//	���������Ď����̒ʐMID�擾
//=========================================================================
#define CONTEST_SIOINIT_DEL		0	//�F�؏����Őڑ���؂�Ȃ��Ȃ����̂ŕs�v�ȕ����폜
void ContestSioInit(u8 id)
{
	u8 i;
	
//	if(ContestTuusin & C_TUUSIN_FLG)

#if CONTEST_SIOINIT_DEL
	PokemonMultiSioInit();
#endif
//	DebugNumPrintInit(0, 0, 31, 2);
//	AddTask(kanshi_network, 1);
	for(i = 0; i < MULTI_SIO_PLAYERS_MAX; i++)
		RecvPoolBuf[i][0] = 0xff;	//�Ƃ肠���� ID�̓���Ƃ�����
	TaskTable[id].work[0] = 0;
//	TaskTable[id].TaskAdrs = SioFlagWait;
	TaskTable[id].TaskAdrs = Sio10SyncWait;
}

//-- SioInit��A10�ݸ�҂� --//
static void Sio10SyncWait(u8 id)
{
#if CONTEST_SIOINIT_DEL
	if(TaskTable[id].work[0] < 10){
		TaskTable[id].work[0]++;
		return;
	}
	
	if(GetSioRealConnectNumber() < BREEDER_MAX)
		return;

	TaskTable[id].work[0] = 0;
	if(GetMyConnectIsParent())	//�e
		TaskTable[id].TaskAdrs = ConnectNumCheck;
	else{						//�q
		TaskTable[id].TaskAdrs = SioFlagWait;
//		SePlay(SE_SEIKAI);	//sio�׸ޑ҂��ɂȂ������}
	}
#else
	TaskTable[id].TaskAdrs = SioFlagWait;
#endif
}

#if CONTEST_SIOINIT_DEL
//-- �e�̂݁F�ڑ��l������ --//
static void ConnectNumCheck(u8 id)
{
	if(TaskTable[id].work[0]++ == 100)
	{
		MultiSioSignalDecision();
		TaskTable[id].work[0] = 0;
		TaskTable[id].TaskAdrs = SioFlagWait;
	}
}
#endif

//-- �ʐM�׸ނ����܂ų��� --//
static void SioFlagWait(u8 id)
{
	if(pokemon_sio_flag == OFF)
		return;
	
	MyBreederNo = GetSioConnectId();
	if(GetConnectSioMenberNum() == 4)	//4�l�q�����Ă邩����
	{
		ContestTuusin = C_TUUSIN_FLG;	//�ʐM�׸�ON
		ChangeBackTask(id);
	}
}

//=========================================================================
//	�S�Ă���ذ�ް�ް����擾����(�e�A�q�A����)
//=========================================================================
void BreederTransData(u8 id)
{
	int i;
	
	switch(TaskTable[id].work[0]){
		case 0:
			if(GetSioConnectId() == 0){
				if(SplitDataSendStatus()){
					memcpy(SendPoolBuf,(u8*)&BreederParam[MyBreederNo],sizeof(BreederData));
					SplitDataExchangeStart(2);
					TaskTable[id].work[0]++;
				}
			}
			else{
				memcpy(SendPoolBuf, (u8*)&BreederParam[MyBreederNo], sizeof(BreederData));
				TaskTable[id].work[0]++;
			}
			break;
		case 1:
			if(GetAllSendData()){
				for(i = 0; i < BREEDER_MAX; i++)
					memcpy(&BreederParam[i], RecvPoolBuf[i], sizeof(BreederData));
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
	}
}



//=========================================================================
//	�e������т��Ȃ��q�ɓ]������
//=========================================================================
void RandTaneTransData(u8 id)
{
	switch(TaskTable[id].work[0]){
		case 0:
			if(GetSioConnectId() == 0){
				if(SplitDataSendStatus()){
					SetContestSendData((u8*)&r_next, sizeof(r_next));
					TaskTable[id].work[0]++;
				}
			}
			else
				TaskTable[id].work[0]++;
			break;
		case 1:
			if(ReceptionDataCheck(0)){
				memcpy(&r_next, RecvPoolBuf[0], sizeof(r_next));
				memcpy(&temp_rand, RecvPoolBuf[0], sizeof(temp_rand));
				TaskTable[id].work[0]++;
//				SplitDataResultReset();
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
	}
}




//=========================================================================
//	�S���̃R���e�X�g�̎�ނ��擾����
//=========================================================================
void ContestTypeGetAllData(u8 id)
{
	int i;
	
	switch(TaskTable[id].work[0]){
		case 0:
			SendPoolBuf[0] = TaskTable[id].work[9];
			if(GetSioConnectId() == 0){
				if(SplitDataSendStatus()){
					SplitDataExchangeStart(2);
					TaskTable[id].work[0]++;
				}
			}
			else
				TaskTable[id].work[0]++;
			break;
		case 1:
			if(GetAllSendData()){
				for(i = 0; i < BREEDER_MAX; i++)
					TaskTable[id].work[1+i] = RecvPoolBuf[i][0];
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
	}
}



//=========================================================================
//	�R���e�X�g�J�n����4�䓯���ɊJ�n���邽�ߓ��������
//=========================================================================
void ContestStandbyCheck(u8 id)
{
	switch(TaskTable[id].work[0]){
		//�ް��͑��M���邪�󂯎��͂��Ȃ��@��������邾���Ȃ̂�
		case 0:
			if(SplitDataSendStatus()){
				SetContestSendData((u8*)&MyBreederNo, 1);
				TaskTable[id].work[0]++;
			}
			break;
		case 1:
			if(GetAllSendData()){
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
	}
}


//=========================================================================
//	�Z���ް���擾����
//=========================================================================
void ContWazaNoSioGet(u8 id)
{
	int i;

	switch(TaskTable[id].work[0]){
		case 0:
			if(SplitDataSendStatus()){
				SetContestSendData((u8*)&apsys[MyBreederNo].use_waza, 2);
				TaskTable[id].work[0]++;
			}
			break;
		case 1:
			if(GetAllSendData()){
				for(i = 0; i < BREEDER_MAX; i++)
					memcpy(&apsys[i].use_waza, RecvPoolBuf[i], 2);
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
	}
}



//=========================================================================
//	�]���l�AContAP�l�A2���R���߲�āA��ýď��ʁA���擾����
//=========================================================================
enum{
	EVALUATION_TRANS = 0,
	EVALUATION_WAIT,
	EVALUATION_ENDWAIT,
	CONTAP_TRANS,
	CONTAP_WAIT,
	CONTAP_ENDWAIT,
	NIJIPOINT_TRANS,
	NIJIPOINT_WAIT,
	NIJIPOINT_ENDWAIT,
	JYUNNI_TRANS,
	JYUNNI_WAIT,
	JYUNNI_ENDWAIT,
};
#define EVALUATION_TRANS_WAIT	10
void EvaluationSioGet(u8 id)
{
	switch(TaskTable[id].work[0]){
		case EVALUATION_TRANS:		//�]���l��]��
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)ContEvaluation, 2*BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case EVALUATION_WAIT:
			if(GetAllSendData()){
				memcpy(ContEvaluation, RecvPoolBuf[ContestSioOya], 2*BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case EVALUATION_ENDWAIT:
		case CONTAP_ENDWAIT:
		case NIJIPOINT_ENDWAIT:
		case JYUNNI_ENDWAIT:
			if(TaskTable[id].work[1]++ > EVALUATION_TRANS_WAIT){
				TaskTable[id].work[1] = 0;
				TaskTable[id].work[0]++;
			}
			break;
		case CONTAP_TRANS:		//ContAP��]��
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)ContAP, 2*BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case CONTAP_WAIT:
			if(GetAllSendData()){
				memcpy(ContAP, RecvPoolBuf[ContestSioOya], 2*BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case NIJIPOINT_TRANS:		//NijiPoint��]��
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)NijiPoint, 2*BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case NIJIPOINT_WAIT:
			if(GetAllSendData()){
				memcpy(NijiPoint, RecvPoolBuf[ContestSioOya], 2*BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case JYUNNI_TRANS:		//ContestJyunni��]��
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)ContestJyunni, BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case JYUNNI_WAIT:
			if(GetAllSendData()){
				memcpy(ContestJyunni, RecvPoolBuf[ContestSioOya], BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
	}
}


//=========================================================================
//	�Z�̌v�Z���ʂ̊e���׸ނ��������\���̂��擾����
//=========================================================================
enum{
	APSYS_TRANS = 0,
	APSYS_WAIT,
	APSYS_ENDWAIT,
	CONWAZA_TRANS,
	CONWAZA_WAIT,
	CONWAZA_ENDWAIT,
	CCW_TRANS,
	CCW_WAIT,
	CCW_ENDWAIT,
	BRDSORT_TRANS,
	BRDSORT_WAIT,
	BRDSORT_ENDWAIT,
};
#define CONWAZA_TRANS_WAIT	10	//�����ް����M�܂łɏ������Ă�����
void ContWazaCalcDataSioGet(u8 id)
{
	switch(TaskTable[id].work[0]){
		case APSYS_TRANS:		//apsys��]��
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)apsys, sizeof(ContestAPSys)*BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case APSYS_WAIT:
			if(GetAllSendData()){
				memcpy(apsys, RecvPoolBuf[ContestSioOya], 
					sizeof(ContestAPSys)*BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case APSYS_ENDWAIT:
		case CONWAZA_ENDWAIT:
		case CCW_ENDWAIT:
		case BRDSORT_ENDWAIT:
			if(TaskTable[id].work[1]++ > CONWAZA_TRANS_WAIT){
				TaskTable[id].work[1] = 0;
				TaskTable[id].work[0]++;
			}
			break;
		case CONWAZA_TRANS:		//CONWAZA��]��
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)CONWAZA, sizeof(WazaKoukaWork));
				TaskTable[id].work[0]++;
			}
			break;
		case CONWAZA_WAIT:
			if(GetAllSendData()){
				memcpy(CONWAZA, RecvPoolBuf[ContestSioOya], sizeof(WazaKoukaWork));
				TaskTable[id].work[0]++;
			}
			break;
		case CCW_TRANS:		//ccw��]��
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)ccw, sizeof(ContestCalcWork));
				TaskTable[id].work[0]++;
			}
			break;
		case CCW_WAIT:
			if(GetAllSendData()){
				memcpy(ccw, RecvPoolBuf[ContestSioOya], sizeof(ContestCalcWork));
				TaskTable[id].work[0]++;
			}
			break;
		case BRDSORT_TRANS:		//BrdSort��]��
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)BrdSort, BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case BRDSORT_WAIT:
			if(GetAllSendData()){
				memcpy(BrdSort, RecvPoolBuf[ContestSioOya], BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
	}
}

//=========================================================================
//	�S���̃R���e�X�g�T�[�o�[��Version���擾����
//=========================================================================
void ContestVersionGetAllData(u8 id)
{
	int i;
	
	switch(TaskTable[id].work[0]){
		case 0:
			SendPoolBuf[0] = CONTEST_VERSION;
			if(GetSioConnectId() == 0){
				if(SplitDataSendStatus()){
					SplitDataExchangeStart(2);
					TaskTable[id].work[0]++;
				}
			}
			else
				TaskTable[id].work[0]++;
			break;
		case 1:
			if(GetAllSendData()){
				for(i = 0; i < BREEDER_MAX; i++)
					TaskTable[id].work[5+i] = RecvPoolBuf[i][0];
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
	}
}

//=========================================================================
//	�ꎟ�R���̌v�Z���ʂ�]������
//=========================================================================
void ContBPAllTransData(u8 id)
{
	switch(TaskTable[id].work[0]){
		case 0:
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)ContBP, 8);
				TaskTable[id].work[0]++;
			}
			break;
		case 1:
			if(GetAllSendData()){
				memcpy(ContBP, RecvPoolBuf[ContestSioOya], 8);
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
			break;
	}
}

//=========================================================================
//	�����BrdSort�f�[�^��]������
//=========================================================================
void BrdSortInitAllTransData(u8 id)
{
	switch(TaskTable[id].work[0]){
		case 0:
			if(SplitDataSendStatus()){	//�S���ް��𑗂邯�ǎ擾����̂͐e���ް�����
				SetContestSendData((u8*)BrdSort, BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		case 1:
			if(GetAllSendData()){
				memcpy(&BrdSort, RecvPoolBuf[ContestSioOya], BREEDER_MAX);
				TaskTable[id].work[0]++;
			}
			break;
		default:
			TaskTable[id].work[0] = 0;
			ChangeBackTask(id);
			break;
	}
}

