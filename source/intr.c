//******************************************************************
//
//			intr.c
//
//			�����݊֘A�v���O����
//			Copyright (C) 2001 GAME FREAK inc.
//******************************************************************

#include "common.h"
#include "multisio.h"
#include "intr.h"
#include "../library/AgbFlash1M.h"
#include "calctool.h"
#include "../library/AgbRtc.h"
#include "laster.h"

#include "db_watch.h"				// DEBUG_WATCH�֘A


extern void intr_main();

extern void MultiSioVsyncFunc( void );
extern	SoundArea m4a_sound;

//-------------------- �ϐ�  ----------------------------
u32	IntrMainBuf[0x200];         // ���荞�݃��C�����[�`���p�o�b�t�@
IntrFuncp IntrTable[14];		// ���荞�݃e�[�u���p�o�b�t�@
s8	DmaCount;					// �T�E���h�p(�g�`�̃o�b�t�@�A�h���X�擾�p�j
u8  SioStopFlag;				// �ʐM�X�g�b�v�p�t���O

//-------------------- �T�u���[�`�� ----------------------------
//void InterruptInit(void);
//void SetVBlankFunc(pFunc);
//void SetHBlankFunc(pFunc);
//void V_Wait(void);

//-------------------- �T�u���[�`��(���[�J��) ----------------------------
static void VBlankIntr(void);
static void HBlankIntr(void);
static void IntrDummy(void);

static void VCountIntr(void);
#ifdef GCDOWNLOAD
static void SioIntr(void);
#endif GCDOWNLOAD

//------------------------------------------------------------------
//                      ���荞�݃e�[�u��                            
//------------------------------------------------------------------

const IntrFuncp IntrTable_0[14] = {
#ifndef GCDOWNLOAD
	Multi_SioIntr,      // �ʐM���荞��
#else	GCDOWNLOAD
	SioIntr,
#endif	GCDOWNLOAD
	Multi_SioTimerIntr, //�^�C�}�[�R���荞��
	HBlankIntr,         // �g�u�����N���荞��
	VBlankIntr,         // �u�u�����N���荞��
	VCountIntr,         // �u�J�E���^��v���荞��
	IntrDummy,          // �^�C�}�[�O���荞��
	IntrDummy,          // �^�C�}�[�P���荞��
	IntrDummy,          // �^�C�}�[�Q���荞��
	IntrDummy,          // �c�l�`�O���荞��
	IntrDummy,          // �c�l�`�P���荞��
	IntrDummy,          // �c�l�`�Q���荞��
	IntrDummy,          // �c�l�`�R���荞��
	IntrDummy,          // �L�[���荞��
	IntrDummy,          // �J�Z�b�g���荞��
};

// �t���b�V���A�N�Z�X�Ɏg�p����^�C�}���荞�݃x�N�^�̃A�h���X
const void * FlashTimerIntrAdrs = &(IntrTable[6]);

//==================================================================
//                      ���荞�ݐݒ胋�[�`��                            
//==================================================================
//------------------------------------------------------------------
//		�����݃e�[�u��������
//------------------------------------------------------------------
void InterruptInit(void)
{
	int i;
	for(i=0;i<14;i++)IntrTable[i]=IntrTable_0[i];
	DmaCopy(3, intr_main, IntrMainBuf, sizeof(IntrMainBuf), 32);// ���荞�݃��C�����[�`�� �Z�b�g
	IntrAddrSet(IntrMainBuf);				// *(vu32 *)INTR_VECTOR_BUF = (vu32 )IntrMainBuf;

	SetVBlankFunc(NULL);
	SetHBlankFunc(NULL);
#ifdef GCDOWNLOAD
	SetSioFunc(NULL);
#endif GCDOWNLOAD

	*(vu16 *)REG_IME = 1;
	*(vu16 *)REG_IE = V_BLANK_INTR_FLAG;// | H_BLANK_INTR_FLAG;
	*(vu16 *)REG_STAT = STAT_V_BLANK_IF_ENABLE; //|STAT_H_BLANK_IF_ENABLE;
	//*(vu16 *)REG_IE = V_BLANK_INTR_FLAG | H_BLANK_INTR_FLAG;
	//*(vu16 *)REG_STAT = STAT_V_BLANK_IF_ENABLE|STAT_H_BLANK_IF_ENABLE;
    *(vu16 *)REG_IE |= V_BLANK_INTR_FLAG;
};


//------------------------------------------------------------------
//		VBlank�����݃Z�b�g
//------------------------------------------------------------------
//void SetVBlankFunc(void (*func)(void) )

void SetVBlankFunc(pFunc func)
{
	sys.pVBlank = func;
};

//------------------------------------------------------------------
//		HBlank�����݃Z�b�g
//------------------------------------------------------------------
//void SetHBlankFunc(void (*func)(void) )

void SetHBlankFunc(pFunc func)
{
	sys.pHBlank = func;
};

//------------------------------------------------------------------
//		VCount�����݃Z�b�g
//------------------------------------------------------------------
//void SetVCountFunc(void (*func)(void) )

void SetVCountFunc(pFunc func)
{
	sys.pVCount = func;
};

#ifdef GCDOWNLOAD
//------------------------------------------------------------------
//			   Sio�����݃Z�b�g
//------------------------------------------------------------------
//void SetSioFunc(void (*func)(void) )

void SetSioFunc(pFunc func)
{
	sys.pSio = func;
};
#endif GCDOWNLOAD

//==================================================================
//                      ���荞�݃��[�`��
//==================================================================
//------------------------------------------------------------------
//                      �u�u�����N���� 
//------------------------------------------------------------------
#ifdef DEBUG_WATCH
unsigned short LYstart;
unsigned short LYend;
unsigned short LYVstart;
unsigned short LYVend;
void MActorSet(void);
#endif

static void VBlankIntr(void)
{
	u16 ImeBak;
	
#ifdef DEBUG_WATCH
    LYVstart= (*(vu16 *)REG_VCOUNT)&0x00ff;
#endif
//	MultiSioVsyncFunc();

	if(!SioStopFlag)									//�X�g�b�v�t���O�������ĂȂ����
		MltSioVblkIntr();								//�ʐM��e���J�n����

	ImeBak = *(vu16 *)REG_IME;							//IME�̑ޔ�
	*(vu16 *)REG_IME = 0;								// IME ���Z�b�g
	m4aSoundVSync();
	*(vu16 *)REG_IME = ImeBak;							//IME�̑ޔ�

	sys.main_timer ++ ;

	if(sys.pVBlank!=NULL)(sys.pVBlank)();
	sys.vsync_counter++;

	DmaCount = m4a_sound.DmaCount;


	m4aSoundMain();


	TuushinRecvSet();			//insert by soga 2002.03.04

	pp_rand();		//��sync������������т������Ƃ�
					//�Ȃ�ׂ��l���΂�΂�ɂ���

#ifdef DEBUG_WATCH
    LYVend  = (*(vu16 *)REG_VCOUNT)&0x00ff;
    MActorSet();
#endif

	*(u16 *)INTR_CHECK_BUF |= V_BLANK_INTR_FLAG;     // V�u�����N�����`�F�b�N�̃Z�b�g
	sys.IntrCheck |= V_BLANK_INTR_FLAG;
};

//------------------------------------------------------------------
//------------------------------------------------------------------

void FlashIntrInit(void)
{
//	SetFlashTimerIntr(2,&(IntrTable[6]));	// TIMER3
	SetFlashTimerIntr( 2, (void(**)())FlashTimerIntrAdrs );
}

//------------------------------------------------------------------
//                      �g�u�����N���� 
//------------------------------------------------------------------
static void HBlankIntr(void)
{
	if(sys.pHBlank!=NULL)(sys.pHBlank)();
	*(u16 *)INTR_CHECK_BUF |= H_BLANK_INTR_FLAG;     // V�u�����N�����`�F�b�N�̃Z�b�g
	sys.IntrCheck |= H_BLANK_INTR_FLAG;
};

//------------------------------------------------------------------
//                      �u�J�E���g���� 
//------------------------------------------------------------------
static void VCountIntr(void)
{
	if(sys.pVCount!=NULL)(sys.pVCount)();
	*(u16 *)INTR_CHECK_BUF |= V_COUNT_INTR_FLAG;     // V�u�����N�����`�F�b�N�̃Z�b�g
	sys.IntrCheck |= V_COUNT_INTR_FLAG;
};

#ifdef GCDOWNLOAD
//------------------------------------------------------------------
//			 �V���A�����荞�ݏ���
//------------------------------------------------------------------
static void SioIntr(void)
{
  if(sys.pSio != NULL)	(sys.pSio)();
  *(u16 *)INTR_CHECK_BUF |= SIO_INTR_FLAG;     // �V���A�������`�F�b�N�̃Z�b�g
  sys.IntrCheck |= SIO_INTR_FLAG;
}
#endif GCDOWNLOAD

//------------------------------------------------------------------
//                      ���荞�� �_�~�[���[�`��
//------------------------------------------------------------------

static void IntrDummy(void)
{
};



void V_Wait(void)
{
#if 0
	sys.IntrCheck &= V_BLANK_INTR_FLAG ^ 0xffff;
	while(1) {
		if( (sys.IntrCheck & V_BLANK_INTR_FLAG) !=0){
			break;
		}
	}
#else
	sys.IntrCheck &= V_BLANK_INTR_FLAG ^ 0xffff;
	VBlankIntrWait();
#endif
}

#ifdef DEBUG_WATCH
/*==================================================================*/
/*                      ���ޯ�ފ֘A      �@                         */
/*==================================================================*/
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
static const u8 mactor_Character[64] = {
0x00,0x20,0x00,0x00,0x00,0x22,0x00,0x00,
0x20,0x22,0x00,0x00,0x22,0x22,0x22,0x22,
0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,
0x00,0x00,0x20,0x20,0x00,0x00,0x00,0x02,
0x00,0x40,0x00,0x00,0x00,0x44,0x00,0x00,
0x40,0x44,0x00,0x00,0x44,0x44,0x44,0x44,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

const u16 sys_obj_Palette[16] = {
0x0000,0x0000,0x001F,0x03E0,0x7C00,0x7FFF,0x7FFF,0x7FFF,
0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,
};

static const OamData MOamData ={
0,0,0,0,0,0,4,0,0,0,0,5,0,15,0,
};

void MActorSet(void)
{
    u16 posy,posyv;

    CpuArrayCopy(mactor_Character,OBJ_MODE0_VRAM+0x7fc0,16);//OBJ CHR�]��
    CpuArrayCopy(sys_obj_Palette,OBJ_PLTT+0x1e0,16);	       //OBJ COL�]��

    if(LYend<LYstart) posy=(LYend+228)-LYstart;
    else              posy=LYend-LYstart;
    if(LYVend<LYVstart) posyv=(LYVend+228)-LYVstart;
    else              posyv=LYVend-LYVstart;

    sys.OAMbuf[127] = *(SysOamData *)&MOamData;
    sys.OAMbuf[127].VPos = posy-3;
    sys.OAMbuf[127].CharNo = 0x3ff;
    sys.OAMbuf[126] = *(SysOamData *)&MOamData;
    sys.OAMbuf[126].VPos = posyv-3;
    sys.OAMbuf[126].CharNo = 0x3fe;

    CpuCopy(&(sys.OAMbuf[126]),OAM+(sizeof(OamData))*126,(sizeof(OamData))*2,16);
}

#endif


//==================================================================
//
//		�\�t�g���Z�b�g
//
//		�����Z�b�g����DMA�\���h�~�����������ɒǉ�����\��
//
//==================================================================

void PM_SoftReset( void )
{
	*(vu16 *)REG_IME = 0;			//���荞�݂��֎~����
	m4aSoundVSyncOff();				//�T�E���hDMA�̋N�����Ƃ߂�
	LasterInit();					//���X�^�[�@�\���~�߂�(DMA0�Ƃ߂Ă���)
	DmaStop(1);						//DMA1���Ƃ߂�
	DmaStop(2);						//DMA2���Ƃ߂�
	DmaStop(3);						//DMA3���Ƃ߂�

	RtcIoDisable();					//RTC���W�X�^���g�p�s�\�ɂ���

	SoftReset( RESET_ALL_FLAG );
}

//==================================================================
//
//		�����W�J���[�N�i�H�j�N���A
//
//		�������I�����̖\�����
//
extern	u8	ImmSng[104];
//==================================================================
void	ImmSngClear(void)
{
	CpuClear(0,ImmSng,104,16);
}






















