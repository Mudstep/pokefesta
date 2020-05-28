//=========================================================================
//
//			�^�X�N
//											01/05/09 by matsuda
//=========================================================================

#include "common.h"
#include "task.h"

#include "print.h"
#include "intr.h"

#include "d_matsu.h"

extern	TALK_WIN	FightMsg;

//----------------------------------------------------------
//	�O���[�o���ϐ�
//----------------------------------------------------------
TASK_TABLE	TaskTable[TASK_MAX];

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define DEFAULT_PRI		0xff;	//������������ײ��è

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
void DummyTaskSeq(u8 taskno);
static u8 GetFastTask(void);
static void pri_insertion(u8 taskno);

//----------------------------------------------------------
//	�^�X�N�����ް�������̃G���[���b�Z�[�W
//----------------------------------------------------------
static const u8 task_over_msg[] = {T__,A__,S__,K__,spc_,O__,V__,E__,R__,CR_,
	TA_,SU_,KU_,ga_,O_,bou_,BA_,bou_,si_,ma_,si_,ta_,EOM_};



//=========================================================================
//	
//=========================================================================

//----------------------------------------------------------
//	�^�X�N�e�[�u���S������
//----------------------------------------------------------
void InitTask(void)
{
	u8 i;
	
	for(i = 0; i < TASK_MAX;i++)
	{
		TaskTable[i].occ = 0;
		TaskTable[i].TaskAdrs = DummyTaskSeq;
		TaskTable[i].prev = i;
		TaskTable[i].next = i+1;
		TaskTable[i].pri = DEFAULT_PRI;
		memset(TaskTable[i].work, 0, TASKWORK_MAX_SIZEOF);
	}
	TaskTable[0].prev = FAST_TASK;
	TaskTable[TASK_MAX-1].next = END_TASK;
}

//----------------------------------------------------------
//	�^�X�N�֐��o�^
// ����:TaskAdrs=�o�^����֐����߲��   pri=�֐����s�̗D�揇��(0(��) �` 0xff(��))
// �߂�l:�o�^���ꂽTaskTable�̔z��ԍ�
//----------------------------------------------------------
u8 AddTask(pTaskFunc TaskAdrs, u8 pri)
{
	u8 i;
	
	for(i = 0; i < TASK_MAX; i++)
	{
		if(TaskTable[i].occ == 0)
		{
			TaskTable[i].TaskAdrs = TaskAdrs;
			TaskTable[i].pri = pri;
			pri_insertion(i);
			memset(TaskTable[i].work, 0, TASKWORK_MAX_SIZEOF);
			TaskTable[i].occ = 1;	//����͍Ō��
			return i;
		}
	}
	
#ifdef PM_DEBUG
	//exit();	//error
	ErrorMsgPut(task_over_msg);	//error
#endif
	return 0;	//0xff;	error�����ǈꉞ�i�߂�悤��0��Ԃ�
}

//----------------------------------------------------------
//	��ײ��è�ɂ���������prev,next��K�؂Ȉʒu�ɑ}��
//----------------------------------------------------------
static void pri_insertion(u8 taskno)
{
	u8 i;
	
	i = GetFastTask();
	if(i == TASK_MAX)	//�����1���o�^����Ă��Ȃ������Ƃ�
	{
		TaskTable[taskno].prev = FAST_TASK;
		TaskTable[taskno].next = END_TASK;
		return;
	}
	
	while(1)
	{
		if(TaskTable[taskno].pri < TaskTable[i].pri)
		{
			TaskTable[taskno].prev = TaskTable[i].prev;
			TaskTable[taskno].next = i;
			if(TaskTable[i].prev != FAST_TASK)	//����2�sAdd 2001.07.16(��)
				TaskTable[TaskTable[i].prev].next = taskno;
			TaskTable[i].prev = taskno;
			break;
		}
		else
		{
			if(TaskTable[i].next == END_TASK)
			{
				TaskTable[taskno].prev = i;
				TaskTable[taskno].next = TaskTable[i].next;
				TaskTable[i].next = taskno;
				break;
			}
			else
				i = TaskTable[i].next;
		}
	}
}

//----------------------------------------------------------
//	�o�^�֐��폜
//----------------------------------------------------------
void DelTask(u8 taskno)
{
	if(TaskTable[taskno].occ == 0)
		return;
	
	TaskTable[taskno].occ = 0;
	if(TaskTable[taskno].prev == FAST_TASK)
	{
		if(TaskTable[taskno].next == END_TASK)
			return;
		else
			TaskTable[TaskTable[taskno].next].prev = FAST_TASK;
	}
	else if(TaskTable[taskno].next == END_TASK)
	{	//prev��FAST�Ȃ��ň���������̂�prev�������͂����ł͂���Ȃ�
		TaskTable[TaskTable[taskno].prev].next = END_TASK;
	}
	else
	{
		TaskTable[TaskTable[taskno].prev].next = TaskTable[taskno].next;
		TaskTable[TaskTable[taskno].next].prev = TaskTable[taskno].prev;
	}
}

//----------------------------------------------------------
//	���C��
//----------------------------------------------------------
void MainTask(void)
{
	u8 taskno;
	
	taskno = GetFastTask();
	if(taskno == TASK_MAX)	//������S�Ĕ�ғ��̏ꍇ
		return;
	
	do
	{
		TaskTable[taskno].TaskAdrs(taskno);
		taskno = TaskTable[taskno].next;
	}while(taskno != END_TASK);
}

//----------------------------------------------------------
//	
//----------------------------------------------------------
static u8 GetFastTask(void)
{
	u8 taskno;

	for(taskno = 0; taskno < TASK_MAX; taskno++)
	{
		if(TaskTable[taskno].occ == 1 && TaskTable[taskno].prev == FAST_TASK)
			break;
	}
	return taskno;
}

//----------------------------------------------------------
//	Dummy�֐�
//----------------------------------------------------------
void DummyTaskSeq(u8 taskno)
{
	;
}














//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================

//=========================================================================
//	�c�[����
//=========================================================================

//----------------------------------------------------------
//	�����TaskAdrs��ύX���āA�߂��̱��ڽ���Ō����work2�ɾ�Ă���
// NextAdrs = TaskAdrs�ɾ�Ă���֐��̱��ڽ
// ReturnAdrs = work�̍Ō���ɾ�Ă�����ڽ
//----------------------------------------------------------
void ChangeTask(u8 taskno, pTaskFunc NextAdrs, pTaskFunc ReturnAdrs)
{
	u8 workno = TASK_WORK_MAX - 2;
	
	TaskTable[taskno].work[workno] = (s16)((u32)ReturnAdrs);
	TaskTable[taskno].work[workno+1] = (s16)((u32)ReturnAdrs >> 16);
	TaskTable[taskno].TaskAdrs = NextAdrs;
}

//----------------------------------------------------------
//	ChangeTask���g�p����ܰ��ɾ�Ă��Ă��������ڽ�����o���āATaskAdrs�ɾ�Ă���
//----------------------------------------------------------
void ChangeBackTask(u8 taskno)
{
	u8 workno = TASK_WORK_MAX - 2;
	
	TaskTable[taskno].TaskAdrs = 
		(pTaskFunc)((0xffff & TaskTable[taskno].work[workno])
		 | (TaskTable[taskno].work[workno+1] << 16));
}

//-------------------------------------------------------------------------
//	�o�^�ς݂̃^�X�N���ǂ����̃`�F�b�N
//
// TRUE		�o�^����Ă���
// FALSE	�o�^����Ă��Ȃ�
//-------------------------------------------------------------------------
u8 CheckTask(pTaskFunc func)
{
		u8 i;
		for( i=0;i<TASK_MAX;i++)
		{
				if(TaskTable[i].occ == 1 && TaskTable[i].TaskAdrs == func)return TRUE;
		};
		return FALSE;
};

//-------------------------------------------------------------------------
//	�^�X�N�v���O�����A�h���X����^�X�N�i���o�[��Ԃ�
//
// NOT_EXIST_TASK_ID	�o�^����Ă��Ȃ�
// ����ȊO				�o�^����Ă���
//-------------------------------------------------------------------------
u8 CheckTaskNo( pTaskFunc func )
{
	int i;
	for( i=0 ; i<TASK_MAX ; i++ )
	{
		if( TaskTable[i].occ == 1 && TaskTable[i].TaskAdrs == func )return i;
	};
	return NOT_EXIST_TASK_ID;
};

//----------------------------------------------------------
//	��������������g�p�������ׂ�@���ޯ��p���ȁH
// �߂�l: �g�p��������̐�
//----------------------------------------------------------
u8 CheckMoveTask(void)
{
	u8 i;
	u8 count = 0;
	
	for(i = 0; i < TASK_MAX; i++)
	{
		if(TaskTable[i].occ == 1)
			count++;
	}
	return count;
}

