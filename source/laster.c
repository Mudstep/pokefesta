//----------------------------------------------------------
//		laster.c		by soga	2001.11.13
//----------------------------------------------------------

#include "common.h"
#include "calctool.h"
#include "laster.h"
#include "task.h"
#include "fight.h"

//----------------------------------------------------------
//	�O���[�o���ϐ�
//----------------------------------------------------------
u16 LasterBuffer[2][320*3];
LasterSystem LasterSys;

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------

void LasterWorkInit(void);
void LasterDmaSet(LasterData);
void LasterVblank(void);

u8		LasterScrollSet(u8 start,u8 end,u8 angle,u8 radius,u8 wait,u8 bg,u8 flag);
void	SinLasterTableMake(u16 *,u8,u8,u8);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------


u8	LasterDelReq = {0};
//----------------------------------------------------------
//	�v���O����
//----------------------------------------------------------
static void LasterLineZeroSet16(void);
static void LasterLineZeroSet32(void);

//========================================
// ׽����~����  2002/08/08  taya
//========================================
void LasterInit(void)
{
	LasterSys.TransType=0;
//	*(vu16 *)REG_DMA0CNT_H = 0;
	DmaStop(0);
	if(LasterSys.TaskNo!=0xff){
		DelTask(LasterSys.TaskNo);
		LasterSys.TaskNo=0xff;
	}
}

void LasterWorkInit(void)
{
	CpuArrayClear(0, LasterBuffer, 16);
	LasterSys.SrcAdrs[0] = NULL;
	LasterSys.SrcAdrs[1] = NULL;
	LasterSys.DestAdrs	 = NULL;
	LasterSys.DMAControl=0;	
	LasterSys.BufFlag = 0;
	LasterSys.TransType = 0;
	LasterSys.LoopCount = 0;
	LasterSys.LoopCountTmp = 0;
	LasterSys.TaskNo=0xff;
}

void LasterDmaSet(LasterData LD)
{
	if(LD.DMAControl == (H_DMA_ENABLE16)){
		LasterSys.SrcAdrs[0]=&(LasterBuffer[0][1]);
		LasterSys.SrcAdrs[1]=&(LasterBuffer[1][1]);
		LasterSys.LineZeroSet = LasterLineZeroSet16;
	}else{
		LasterSys.SrcAdrs[0]=&(LasterBuffer[0][2]);
		LasterSys.SrcAdrs[1]=&(LasterBuffer[1][2]);
		LasterSys.LineZeroSet = LasterLineZeroSet32;
	}
	LasterSys.DMAControl=LD.DMAControl;	
	LasterSys.DestAdrs=LD.DestAdrs;	
	LasterSys.TransType=LD.TransType;
	LasterSys.LoopCount=LD.LoopCount;
	LasterSys.LoopCountTmp=LD.LoopCount;
}
void LasterVblank(void)
{
	if(LasterSys.TransType==0) return;
	if(LasterSys.TransType==LASTER_END){
		// ��~�������O������Ăׂ�悤�A�֐��Ƃ��ēƗ�������
		// 2002/08/08  taya
		LasterSys.TransType=0;
//		*(vu16 *)REG_DMA0CNT_H = 0;
		DmaStop(0);
		LasterDelReq = 1;	//�^�X�N�N�����ɏ���ɏ�������̂�
							//�܂����̂Ń��N�G�X�g���ă^�X�N���ŏ���
//		LasterInit();
		return;
	}
//	*(vu16 *)REG_DMA0CNT_H = 0;
	DmaStop(0);
	DmaSet(	0,
			LasterSys.SrcAdrs[LasterSys.BufFlag],
			LasterSys.DestAdrs,
			LasterSys.DMAControl);

	LasterSys.LineZeroSet();

	LasterSys.BufFlag^=1;	
}
//=====================================================
// LasterSys.LineZeroSet �ŌĂ΂��֐�
//=====================================================
static void LasterLineZeroSet16(void)
{
	*(vu16*)(LasterSys.DestAdrs) = *(vu16*)(LasterBuffer[LasterSys.BufFlag]);
}
static void LasterLineZeroSet32(void)
{
	*(vu32*)(LasterSys.DestAdrs) = *(vu32*)(LasterBuffer[LasterSys.BufFlag]);
}


//============================================================
//	TaskLasterScroll
//============================================================

void TaskLasterScroll(u8 no)
{
	int	i,j;
	int	ofs=0;

	if(LasterDelReq){
		DelTask(no);
		LasterSys.TaskNo=0xff;
		return;
	}
	if(TaskTable[no].work[LS_FLAG]){
		switch(TaskTable[no].work[LS_BGNO_HV]){
			case LS_BG0H:
				ofs=ScrX0;
				break;
			case LS_BG0V:
				ofs=ScrY0;
				break;
			case LS_BG1H:
				ofs=ScrX1;
				break;
			case LS_BG1V:
				ofs=ScrY1;
				break;
			case LS_BG2H:
				ofs=ScrX2;
				break;
			case LS_BG2V:
				ofs=ScrY2;
				break;
			case LS_BG3H:
				ofs=ScrX3;
				break;
			case LS_BG3V:
				ofs=ScrY3;
				break;
		}
	}
	else
		ofs=0;

	if(TaskTable[no].work[LS_WAIT]){
		TaskTable[no].work[LS_WAIT]--;
		j=320+TaskTable[no].work[LS_GETDATA];
		for(i=TaskTable[no].work[LS_START];i<TaskTable[no].work[LS_END];i++)
			LasterBuffer[LasterSys.BufFlag][i]=LasterBuffer[0][j++]+ofs;
	}
	else{
		TaskTable[no].work[LS_WAIT]=TaskTable[no].work[LS_WAIT_TMP];
		j=320+TaskTable[no].work[LS_GETDATA];
		for(i=TaskTable[no].work[LS_START];i<TaskTable[no].work[LS_END];i++)
			LasterBuffer[LasterSys.BufFlag][i]=LasterBuffer[0][j++]+ofs;
		if(++TaskTable[no].work[LS_GETDATA]==TaskTable[no].work[LS_LOOPCNT])
			TaskTable[no].work[LS_GETDATA]=0;
	}
}

void SinLasterTableMake(u16 *buf,u8 angle,u8 radius,u8 line)
{
	u16	i; 
	u8	theta;

	for(i=0,theta=0;i<256;i++){
		buf[i]=(sin_cos_table[theta]*radius/0x100);
		theta+=angle;
	}
}

//============================================================
//	LasterScrollSet
//
//	�����F
//	start	���X�^�[�J�n���C��
//	end		���X�^�[�I�����C��
//	angle	�p�x�i�T�C���J�[�u�j
//	radius	���a�i�T�C���J�[�u�j
//	wait	�E�F�C�g�i�X�s�[�h�F�O�ōő��j
//	bg		���X�^�[��������BGNo�ƕ����ilaster.h��define����Ă���j
//	flag	�X�N���[���␳��ON/OFF�i0�FOFF 1:ON�j
//============================================================

u8	LasterScrollSet(u8 start,u8 end,u8 angle,u8 radius,u8 wait,u8 bg,u8 flag)
{
	LasterData	LD;
	u8	no;
	int	i,j;

	LasterWorkInit();
	LD.DestAdrs=(void *)REG_BG0HOFS+bg;	//�ް��]������ڽ(REG_BG0VOFS��)
	LD.DMAControl=H_DMA_ENABLE16;		//16�r�b�g�]��
	LD.TransType=LASTER_NORMAL;
	LD.LoopCount=0;						//�]���^�C�v�����[�v�̎��̃J�E���g

	LasterDmaSet(LD);

	no=AddTask(TaskLasterScroll,0);
	TaskTable[no].work[LS_START]=start;
	TaskTable[no].work[LS_END]=end;
	TaskTable[no].work[LS_LOOPCNT]=256/angle;
	TaskTable[no].work[LS_GETDATA]=0;
	TaskTable[no].work[LS_WAIT]=wait;
	TaskTable[no].work[LS_WAIT_TMP]=wait;
	TaskTable[no].work[LS_BGNO_HV]=bg;
	TaskTable[no].work[LS_FLAG]=flag;
	LasterSys.TaskNo=no;
	LasterDelReq = 0;

	SinLasterTableMake(&LasterBuffer[0][320],angle,radius,end-start);

	j=320;
	for(i=start;i<end;i++){
		LasterBuffer[0][i]=LasterBuffer[0][j];
		LasterBuffer[1][i]=LasterBuffer[0][j++];
	}

	return no;
}

