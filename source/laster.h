#ifndef __LASTER_H__
#define __LASTER_H__


typedef struct{
	void	*DestAdrs;		//�ް��]������ڽ(REG_BG0VOFS��)
	u32		DMAControl;
	u8		TransType;		//�]���^�C�v�i0:���N�G�X�g�Ȃ�1:�m�[�}��2:���[�v�^�j
	u8		LoopCount;		//�]���^�C�v�����[�v�̎��̃J�E���g
}LasterData;

typedef struct{
	void	*SrcAdrs[2];
	void	*DestAdrs;
	u32		DMAControl;	
	pFunc	LineZeroSet;
	u8		BufFlag;
	u8		TransType;
	u8		LoopCount;
	u8		LoopCountTmp;
	u8		TaskNo;
}LasterSystem;

//=========================================================================
//	�O���ϐ��錾
//=========================================================================
extern u16 LasterBuffer[2][320*3];
extern LasterSystem LasterSys;

//=========================================================================
//	�O���֐��錾
//=========================================================================

extern void LasterInit(void);
extern void LasterWorkInit(void);
extern void LasterDmaSet(LasterData);
extern void LasterVblank(void);

extern u8 LasterScrollSet(u8 start,u8 end,u8 angle,u8 radius,u8 wait,u8 bg,u8 flag);

//=========================================================================
//	�萔
//=========================================================================


#define H_DMA_ENABLE16	DMA_ENABLE|DMA_TIMMING_H_BLANK|DMA_SRC_INC|DMA_DEST_RELOAD|DMA_CONTINUOUS_ON|DMA_16BIT_BUS|1

#define H_DMA_ENABLE32	DMA_ENABLE|DMA_TIMMING_H_BLANK|DMA_SRC_INC|DMA_DEST_RELOAD|DMA_CONTINUOUS_ON|DMA_32BIT_BUS|1

enum{
	LASTER_OFF=0,
	LASTER_NORMAL,
	LASTER_LOOP,
	LASTER_END,
};

//LasterScroll�p��enum
enum{
	LS_START=0,
	LS_END,
	LS_LOOPCNT,
	LS_GETDATA,
	LS_WAIT,
	LS_WAIT_TMP,
	LS_BGNO_HV,
	LS_FLAG,
};

#define	LS_BG0H 0x00
#define	LS_BG0V 0x02
#define	LS_BG1H 0x04
#define	LS_BG1V 0x06
#define	LS_BG2H 0x08
#define	LS_BG2V 0x0a
#define	LS_BG3H 0x0c
#define	LS_BG3V 0x0e

#define	LS_HOSEI_OFF	0
#define	LS_HOSEI_ON		1

#endif	//__LASTER_H__
