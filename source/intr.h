#ifndef __INTR_H__
#define __INTR_H__

//******************************************************************
//
//			intr.h
//
//			�����݊֘A�v���O����
//			Copyright (C) 2001 GAME FREAK inc.
//******************************************************************

#include "common.h"


typedef void (*IntrFuncp)(void);	//�����݊֐��|�C���^�^

extern IntrFuncp IntrTable[14];		//�����݃e�[�u���i�[�p�z��
extern const void * FlashTimerIntrAdrs;

extern void InterruptInit(void);		//�����ݏ�����
extern void SetVBlankFunc(pFunc);		//Vsync�����݃��[�`���Z�b�g
extern void SetHBlankFunc(pFunc);		//Hsync�����݃��[�`���Z�b�g
extern void SetVCountFunc(pFunc);		//Vcount�����݃��[�`���Z�b�g
extern void V_Wait(void);				//Vsync�����ݑ҂�
extern void FlashIntrInit(void);

extern void PM_SoftReset( void );		//�Q�[�����̃\�t�g���Z�b�g

#ifdef GCDOWNLOAD
extern void SetSioFunc(pFunc);			//Sio���荞�݃��[�`���Z�b�g
#endif GCDOWNLOAD

//=========================================================================
//	IE���W�X�^����}�N��
//=========================================================================
//	IE���W�X�^�Ɏw�肵���t���O���Z�b�g
#define	REG_IE_SET( v )							\
	{											\
	u16 Reg_IME_Backup;							\
	Reg_IME_Backup = *(vu16 *)REG_IME;			\
	*(vu16 *)REG_IME = 0;						\
	*(vu16 *)REG_IE |= (v);						\
	*(vu16 *)REG_IME = Reg_IME_Backup;			\
	}

//	IE���W�X�^�Ɏw�肵���t���O�����Z�b�g
#define	REG_IE_RESET( v )						\
	{											\
	u16 Reg_IME_Backup;							\
	Reg_IME_Backup = *(vu16 *)REG_IME;			\
	*(vu16 *)REG_IME = 0;						\
	*(vu16 *)REG_IE &= ~(v);					\
	*(vu16 *)REG_IME = Reg_IME_Backup;			\
	}

//	IE���W�X�^�Ɏw�肵���t���O���Z�b�g�����Z�b�g
#define REG_IE_CHG(set,reset)					\
	{											\
	u16 Reg_IME_Backup;							\
	Reg_IME_Backup = *(vu16 *)REG_IME;			\
	*(vu16 *)REG_IME = 0;						\
	*(vu16 *)REG_IE &= ~(reset);				\
	*(vu16 *)REG_IE |= (set);					\
	*(vu16 *)REG_IME = Reg_IME_Backup;			\
	}

#endif __INTR_H__

