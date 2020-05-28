//<d_ropeway.h>
///////////////////////////////////////////////////////////////////
//
//	���[�v�E�F�C�f���p	�w�b�_�t�@�C��
//
//
//
///////////////////////////////////////////////////////////////////

#ifndef _DEBUG_ROPEWAY_DEMO_H_
#define _DEBUG_ROPEWAY_DEMO_H_

//�ˑ����Ă���w�b�_�t�@�C��
#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "madefine.h"

#include "mitool.h"

//UserWork�g�p�G���A��`
#define RWD_WORK0_AREA	(0x17000)
#define RWD_WORK1_AREA	(0x19000)
#define RWD_WORK0_ADDR	(&(UserWork[RWD_WORK0_AREA])
#define RWD_WORK1_ADDR	(&(UserWork[RWD_WORK1_AREA]))

//Actor
#define RWD_OBJCELL_NUM		(3)
#define RWD_OBJPAL_NUM		(1)

typedef struct 
{
	u8		uc_subTaskNo;		//�T�u�^�X�N�̃^�X�N�i���o�[�ۑ�
	u8		uc_state;			//���݂̏�ԊǗ�
	u8		uc_weather;			//�V��Ǘ��̃t���O
	u16		us_weaTime;			//�V��`�F���W�̃^�C��
	u16		us_time;			//�^�C�}�[	
	
	_scroll	scroll[4];			//BG�X�N���[���l�̊Ǘ�
	
	struct{	//�R���̃X�N���[���Ǘ��p�̃p�����[�^
		char	cBlockCt;		//�u���b�N�J�E���^
		
		u8		uc_sLine;		//�Q�Ƃ��Ă��鍶�E�[�̃L�����i���o�[
		u8		uc_dLine;		//�Q�Ƃ��Ă��鉺�[�̃L�����i���o�[
		u8		uc_blockNo;		//�g�p�����A�Ō�̃u���b�N�i���o�[
		u8		uc_scrScroll;	//�X�N���[���X�N���[���̃J�E���g

		u8		uc_dx;			//uc_oldX����̃I�t�Z�b�g
		u8		uc_dy;
		u8		uc_oldX;		//�Â��ʒu�̕ۑ�
		u8		uc_oldY;

		u16		us_buf[9][12];	//��ԗp�̃X�N���[���f�[�^���i�[����o�b�t�@
	}MtScrl;
	
	_tMap		map[2];			//BG�X�N���[���f�[�^�̓W�J�ꏊ

}RWDEMO_WORK;

//�֐��̃G�N�X�^�[��
extern void DemoRopewaySetUp(void);
extern void DemoRopewaySetDown(void);
extern void DemoRopewayCall(void);

#endif //_DEBUG_ROPEWAY_DEMO_H_
