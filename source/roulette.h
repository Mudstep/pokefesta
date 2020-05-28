//========================================================================================
//								�~�j�Q�[���E���[���b�g
//								2002/03/28 by nakahiro
//								2002/05/30 iwasawa's job,a job form nakahiro
//========================================================================================

#ifndef _ROULETTE_H_
#define _ROULETTE_H_

//�f�o�b�O�t���O
#define	_IF_RLT_DEBUG_
#ifdef	_IF_RLT_DEBUG_
	#define RLT_DEBUG	(1)
#else
	#define RLT_DEBUG	(0)
#endif	//IF_RLT_DEBUG

#include "mitool.h"

//�֐��̃G�N�X�^�[��
extern void RouletteSet(void);		// ���[���b�g�Ăяo��
extern s16 fix_mul( s16, s16 );		// a*b �����߂�
extern s16 fix_inverse( s16 );		// 1/b �����߂�

/////////////////////////////////////////////////////////////////////////////
//	�V���{����`
////////////////////////////////////////////////////////////////////////////
//�g�p���郆�[�U�[���[�N�G���A
//#define RLT_UWORK_GPPAL	(0x00000000)
//#define RLT_UWORK_GPBG	(0x00008000)
//#define RLT_UWORK_GROBJ	(0x00012000)
//#define RLT_UWORK_PROG	(0x00016000)
#define RLT_UWORK_GPPAL	(0x00000000)
#define RLT_UWORK_GROBJ	(0x00017000)
#define RLT_UWORK_GPBG	(0x00018800)
#define RLT_UWORK_PROG	(0x00019000)

#define	RLT_ACT_MAX		64			//�g�p�������A�N�^�[��
#define RLT_ACTID_MAX	64			//2���g�p�������A�N�^�[ID�̐�
#define RLT_CELL_MAX	12			//�e�[�u���̐�
#define RLT_BET_MAX		6			//�q��������̍ō���

/////////////////////////////////////////////////////////////////
//	�f�[�^�^��`
/////////////////////////////////////////////////////////////////
typedef struct {
	u8	cID;		// �Z��ID
	u8	odds:4;		// �{��
	u8	poke:4;		// �|�P����
	u8	color:4;	// �F
	u8	dummy:4;	// �_�~�[

	u8	posx;		// �Z���̕\��XPOS 
	u8	posy;		// �Z���̕\��YPOS
	u8	dx;			// �Z��BG�f�[�^�̃G���AX
	u8	dy;			// �Z��BG�f�[�^�̃G���AY

	u32	flg;		// �q�b�g�����ꏊ�̕ێ��t���O
	u32 bet_flg;	// �q�����ꏊ�̃`�F�b�N�p�t���O
	u16	canm;		// �J���[�A�j���[�V�����̋N���t���O
}BET_PARAM;

typedef struct{
	u8		uc_no;			//�ʂ̃i���o�[
	u8		uc_flg;			//�t���O�G���A
	u8		uc_cell;		//�ʂ�����Z���i���o�[
	u8		uc_area;		//�ʂ̂���G���A�i���o�[
	u16		us_syncMax;		//�ʂ��X�g�b�v����܂ł�Sync
	u16		us_syncA0;		//�ʂ��X�g�b�v����܂ł�Sync
	u16		us_syncA1;
	u16		us_syncA2;

	float	fr;				//���݂̊p�x
	float	frv;			//�p���x
	float	fra;			//�p�����x

	float	fw;				//���݂̔��a
	float	fwv;			//���a�ɑ΂��鑬�x
	float	fwa;			//���a�ɑ΂�������x
	float	fwo;			//�X�^�b�N�����Ƃ��Ɏg���l
}BALL_PARAM;

typedef struct{
	u8		uc_no;		//�Z���i���o�[
	u8		uc_area;	//�G���A�i���o�[
	u8		uc_bit;		//���r�b�g�ڂ�ON�ɂ��邩
	u32		ui_flg;		//���ۂ̃t���O
}RLT_CELL_PARAM;

//�e�[�u���̃p�����[�^
typedef struct{
	u8		uc_rate;		//���[�g
	u8		uc_rBase;		//1�Z����ROT
	u8		uc_rBaseH;		//
	
	u8		uc_rSpd;		//��]�̃X�s�[�h
	u8		uc_rWait;		//��]�p�̃E�F�C�g

	struct{					//�C�x���g�p�̃p�����[�^
		u16	prm0;
		u16	prm1;
		u16	prm2;
	}ev[2];
	
	u16		w_v0;			//�{�[���̏���
	u16		w_t0;			//��{�^�C��
	float	f_invV;
}RLT_TABLE_PARAM;

typedef struct{
	//�E�F�C�g����
	u16			us_wait;			//�E�F�C�g
	u16			us_cKey;			//�L�����Z���L�[
	pTaskFunc	wait_ret_adrs;		//�E�F�C�g����߂��Ă���^�X�N�A�h���X
	pTaskFunc	menu_ret_adrs;		//���j���[����߂��Ă���^�X�N�A�h���X
	pTaskFunc	back_adrs;			//�O�A�h���X�̕ۑ�	
}TASK_CNT;

typedef struct {

	// �Q�[���֘A
	u8	time_flg;				// �^�C���t���O 1 = �J�E���g��, 0 = �I��
	u8	state_flg;
	u8	mons_flg;				//����̃����X�^�[���莝���ɓ���Ă��邩�ǂ���
	
	//�C�x���g�p�t���O
	u8	ev_no :5;				//�C�x���g�i���o�[
	u8	ev_on :1;				//�X�^�b�N�C�x���g���������Ă��邩�ǂ���
	u8	ev_end:1;				//�C�x���g�̏I���������t���O
	u8	ev_bact:1;				//�{�[�����A�N�e�B�u���ǂ����̃t���O

	//ScriptParameter0�̉���1Byte���X�N���v�g����̃t���O�Ŏ擾
	u8	rlt_ver :2;				//��̃��[�g�̎��ʃt���O
	u8	:5;						//�_�~�[
	u8	rlt_bonus:1;			//�{�[�i�X���Ԓ����ǂ����H

	// �q�b�g�����Z���֘A
	u32 rlt_hp;					// �������ꏊ�t���O
	u8	hit_cell[RLT_BET_MAX];	// �q�b�g�����Z���i���o�[�����Ԃɕۑ�
	u8  hit_vLine[4];			// �c�̃��C�����������܂��Ă��邩
	u8	hit_hLine[3];			// ���̃��C�����������܂��Ă��邩
	
	// �x�b�g���ꂽ�ꏊ�̋L��
	u8	uc_rate;				//���[�g�͂������H
	u8	nBet:4;					//����ڂ̓q�����H
	u8	betAll:4;				//���ӏ��ɓq�����邩�H
	u8	uc_aBet[RLT_BET_MAX];	//�ǂ��ɓq�������H
	
	// BG�֘A
	u8	bg_ct;					// ��]����J�E���^
	u8	bg_spd;					// ��]�̃X�s�[�h
	u8	bg_wait;				// ��]����E�F�C�g
	s16	bg_rotate;				// ��]�p�x
	s16	bg1_scrX;				// bg1�̃X�N���[������
	s16 bg0_cnt;
	s16 bg1_cnt;
	s16	bg_sPa;
	s16	bg_sPb;
	s16	bg_sPc;
	s16	bg_sPd;
	u16	w_alpha;

	// OBJ�֘A
	actWork* pAct;					// �ėp�A�N�^�[ID�ۑ�
	u8	act_id[ RLT_ACTID_MAX ];	// �A�N�^�[�ԍ�

	//�{�[���֘A
	BALL_PARAM	ball;

	//�^�X�N����
	u8			sTask_id[2];	//�T�u�^�X�NID�ۑ�
	TASK_CNT	task;

	//�J���[�A�j������
	ColorAnime	colanm;
}ROULETTE;

#endif //_ROULETTE_H_

