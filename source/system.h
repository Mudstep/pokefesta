#ifndef __SYSTEM_H__
#define __SYSTEM_H__

//******************************************************************
//          system.h
//
//          Copyright (C) 2001 GAME FREAK inc.
//******************************************************************

#include "actor.h"

//------------------- ��` -----------------------------------------

typedef	struct {
	pFunc pFirstProc;		// ����v���Z�X�Ăяo���֐��|�C���^
	pFunc pMainProc;		// ���C���v���Z�X�Ăяo���֐��|�C���^
	pFunc pReturnProc;		// ���菈����ɕ��A�����̊֐��|�C���^
	pFunc pVBlank;			// �u�u�����N�����ݓ��Ăяo���֐��|�C���^
	pFunc pHBlank;			// �g�u�����N�����ݓ��Ăяo���֐��|�C���^
	pFunc pVCount;			// �u�J�E���g�����ݓ��Ăяo���֐��|�C���^
  //#ifdef GCDOWNLOAD
	pFunc pSio;			// �r�h�n�����ݓ��Ăяo���֐��|�C���^
  //#endif GCDOWNLOAD

	vu16 IntrCheck;			// �����ݏI���X�e�[�^�X

	u32 main_timer;			// ���C�����[�`���p�J�E���g
	u32 vsync_counter;		// �u�u�����N�p�J�E���^

	u16 RealCont;			// �L�[���
	u16 RealTrg;			// �L�[�g���K�[
	u16 Cont;				// �L�[���(�Q�[���ݒ�ɂ��ϊ�����������)
	u16 Trg;				// �L�[�g���K�[(�Q�[���ݒ�ɂ��ϊ�����������)
	u16 Repeat;				// �L�[���s�[�g
	u16 RepeatWait;			// �L�[���s�[�g�E�F�C�g
	u16 TrgFlag;			// �ŏ��̃g���K�[���s�t���O
	u16 TrgFlagMask;		// �ŏ��̃g���K�[���s�}�X�N

	SysOamData OAMbuf[128];	//add matsuda
	u8 InitSeqWork;			//�������V�[�P���X�p���[�N

	u8 OAMTransFlag	:1;		//OAM�]�������׸�(0:���� 1:�s����)	//add matsuda
	u8 FieldBattleFlag:1;	//���A�t�B�[���h�Ȃ̂��퓬�Ȃ̂�(0:Field 1:Battle)
							//add soga
	u8				:6;
}SystemArea;

#endif //__SYSTEM_H__

