#ifndef __COMMON_H__
#define __COMMON_H__

//******************************************************************
//
//			common.h
//
//			���ʒ�`�t�@�C��
//
//			Copyright (C) 2001 GAME FREAK inc.
//******************************************************************

#include <Agb.h>

#include "version.h"

//	������֘A�̒�����`
#define PERSON_NAME_SIZE	7		// �l���̖��O�̒����i�������܂ށj
#define MONS_NAME_SIZE		10		// �|�P�������̒���
#define	WAZA_NAME_SIZE		7		// �킴���̒���
#define	ITEM_NAME_SIZE		8		// �ǂ������̒���
#define SEED_NAME_SIZE		6		// �^�l�̖��O�̒���
#define	EOM_SIZE			1		// �I���R�[�h�̒���

//-- �|�P�����莝���ő吔 --//
#define TEMOTI_POKEMAX		6

//-- �|�P������ --//
#define	NEW_POKE_NUM_MAX	(202)	//�z�E�G���}�Ӑ�
#define	POKE_NUM_MAX		(386)	//�S���}�Ӑ�
#define	KAIHATSU_NUM_MAX	(411)	//�J���i���o�[�l�`�w��


#define NULL ((void *)0)			// NULL�|�C���^�i���Ŏ����Œ�`���Ȃ��Ƃ��߂Ȃ̂�?�j
#define   XtOffset(p_type,field)     ((unsigned int)&(((p_type )NULL)->field))
#define	NELEMS(array)	( sizeof(array) / sizeof(array[0]) )

#define	KEYREPEATSPEED		5		//�L�[���s�[�g�E�F�C�g
#define	KEYREPEARSTARTWAIT	40		//�L�[���s�[�g���n�܂�܂ł̃E�F�C�g

typedef void (*pFunc)(void);		// �֐��|�C���^�^

typedef	u8	VM_CODE;				// �X�N���v�g�A�C�x���g�f�[�^�̌^�錾

#include "../snd_file/m4aLib.h"		// �T�E���h���C�u����
#include "../snd_file/mus_def.h"

#include "system.h"					//�O���[�o���ϐ���`
#include "gamedata.h"				//�Q�[���i�s�f�[�^��`
#include "memtool.h"				//DMA�]���ʐ����}�N����`

extern void	MainProcChange(pFunc func);

//==================================================================
//==================================================================


//==================================================================
//	�O���[�o���ϐ�
//==================================================================

extern u8 DebugMainFlag;		//�f�o�b�O���[�h����p�t���O

extern SystemArea sys;			// �V�X�e�����\�[�X
extern u8 UserWork[];			// �ėp���[�N�G���A

extern	u16	KeyRepeatSpeed;
extern	u16	KeyRepeatStartWait;

extern SYS_SAVE_DATA MyData;		// �Z�[�u�f�[�^�F�������Ȃ�
extern FIELD_SAVE_DATA Fld;			// �Z�[�u�f�[�^�F�Q�[���i�s�Ȃ�

#define GCDOWNLOAD			// ��`�����GC����̃_�E�����[�h�@�\��L��

#endif //__COMMON_H__
