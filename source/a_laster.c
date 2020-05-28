

#if 0	//****************************************
			�S����폜
	



//=========================================================================
//	
//	�Z�G�t�F�N�gSub�F���X�^�[
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

#include "laster.h"
#include "task.h"

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void WazaTaskLasterInit(u8 id);
static void WazaTaskLasterSet(u8 id);
static void WazaTaskLasterWait(u8 id);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define TARGET_BG	((void*)REG_BG1HOFS)

//----------------------------------------------------------
//	���X�^�[�e�[�u��
//----------------------------------------------------------
static const HLasterData WazaLasterData = 
{
	(void*)REG_BG1VOFS,
	30,
	90,
	0,
	0,
	600,
};


//-------------------------------------------------------------
//		�^�X�N�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:׽���J�nײ�  1:׽���I��ײ�  
//		  2:���8�ޯāA׽���O�����l1 ����8�ޯāA׽���O�����l2
//        3:ٰ�߉�  4:���8�ޯāAH�p�x�����l ����8�ޯāAH�p�x�����l
//        5:H�U�蕝  6:���8�ޯāAV�p�x�����l ����8�ޯāAV�p�x�����l
//		  7:V�U�蕝
//-------------------------------------------------------------
void WazaTaskLaster(u8 id)
{
	TaskTable[id].work[0] = WazaEffWork[0];
	TaskTable[id].work[1] = WazaEffWork[1];
	TaskTable[id].work[2] = WazaEffWork[2];
	TaskTable[id].work[3] = WazaEffWork[3];
	TaskTable[id].work[4] = WazaEffWork[4];
	TaskTable[id].work[5] = WazaEffWork[5];
	TaskTable[id].work[6] = WazaEffWork[6];
	TaskTable[id].work[7] = WazaEffWork[7];
	
	TaskTable[id].TaskAdrs = WazaTaskLasterInit;
	TaskTable[id].TaskAdrs(id);
}

static void WazaTaskLasterInit(u8 id)
{
	HLasterData ldata;
	LMD lmd;
	
	if(LasterSys.req == 1)
		return;
	
	ldata.DestAdrs = TARGET_BG;
	ldata.start_line = TaskTable[id].work[0];
	ldata.end_line = TaskTable[id].work[1];
	ldata.init_data1 = TaskTable[id].work[2] >> 8;
	ldata.init_data2 = TaskTable[id].work[2] & 0xff;
	ldata.loop = TaskTable[id].work[3];
//	LasterMake((const HLasterData *)&ldata, 0, TaskTable[id].work[5] >> 8,
//			TaskTable[id].work[5] & 0xff);
	lmd.h_angle = TaskTable[id].work[4] >> 8;
	lmd.h_add_angle = TaskTable[id].work[4] & 0xff;
	lmd.h_furihaba = TaskTable[id].work[5];
	lmd.v_angle = TaskTable[id].work[6] >> 8;
	lmd.v_add_angle = TaskTable[id].work[6] & 0xff;
	lmd.v_furihaba = TaskTable[id].work[7];
	LasterMake(&ldata, &lmd);
	TaskTable[id].TaskAdrs = WazaTaskLasterSet;
}

//----------------------------------------------------------
//	����ȑO��׽������Ă���Ă��ďI���҂����������ɁA����������
// ��Ɏ��s����āA�uDelTask���o���Ȃ��v�Ƃ������ƂɂȂ�Ȃ��悤��
// �킴��1WAIT�󂯂����Ă���HdmaSet���s��
//----------------------------------------------------------
static void WazaTaskLasterSet(u8 id)
{
	HdmaSet();
	TaskTable[id].TaskAdrs = WazaTaskLasterWait;
}

static void WazaTaskLasterWait(u8 id)
{
	if(LasterSys.req == 0)
		DelTaskEffect(id);
}






#endif	//**********************************************





