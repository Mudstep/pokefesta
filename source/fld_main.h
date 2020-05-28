#ifndef FLD_MAIN_H
#define FLD_MAIN_H

//=========================================================================
//	
//	�t�B�[���h���C������
//
//=========================================================================


#undef GLOBAL
#ifdef FLD_MAIN_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

extern pFunc pFieldRecoverFunc;		// �t�B�[���h���A�����̊֐��|�C���^

extern u8 comm_player_num;			//�ʐM�������̐l��
extern u8 comm_my_id;				//�ʐM�������̎�����ID
extern u16 my_command;				//�ʐM���̓���R�[�h�󂯓n���p�ϐ�

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

GLOBAL void NormalGameStart(void);		//�ʏ�Q�[���J�n
GLOBAL void DebugGameStart(void);		//�f�o�b�O���[�h�Q�[���J�n
GLOBAL void GameOverRestart(void);		//�Q�[���I�[�o�[���̍ĊJ
GLOBAL void ContinueGameStart(void);	//�u�Â�����v�J�n
GLOBAL void FieldMainInit(void);		//�}�b�v�J�ڎ��̃��X�^�[�g
GLOBAL void FieldMainCommInit(void);
GLOBAL void FieldMainRecover(void);		//��ʐ؂�ւ����̕��A
GLOBAL void FieldMenuRecover(void);		//���j���[�ւ̕��A
GLOBAL void FieldScriptRecover(void);	//�X�N���v�g���s�ւ̕��A
GLOBAL void FieldScriptRecoverBGMNotStart(void);	//�X�N���v�g���s�ւ̕��A(BGM���̂܂�)
GLOBAL void FieldCommRecover(void);		//�ʐM������Ԃւ̕��A
GLOBAL void FieldNormalRecover( void );	//���������Ƀt�B�[���h���A

GLOBAL void FieldContestInit( void );	//�}�b�v�J�ڂ���i�R���e�X�g��p�j

GLOBAL void FieldMain(void);
GLOBAL void FieldEffectMain(void);

//-------------------------------------------------------------
//	�t�B�[���h�ʐM���[�h���ǂ����̃`�F�b�N
//	�����͊֐��|�C���^�����Ă��邪�t���O�ɕύX���邩��
//-------------------------------------------------------------
GLOBAL int IsFieldCommMode( void );

// �ʐM���̐���
//-------------------------------------------------------------
//�C�x���g�J�n�҂��`�F�b�N
//	<�߂�l>
//	0		�҂����
//	1		�C�x���g�J�n
//	2		�C�x���g�L�����Z���i�o�������ցj
//-------------------------------------------------------------
GLOBAL int CommEventWaitCheck(void);

//-------------------------------------------------------------
//	�ʐM�I���`�F�b�N
//	<�߂�l>	TRUE	���ꂩ����߂�������
//				FALSE
//-------------------------------------------------------------
GLOBAL int CommEventEndCheck(void);

GLOBAL int CommEventWaitStart( void );		//�C�x���g�J�n�҂��X�^�[�g
GLOBAL int CommEventWaitCancel( void );		//�C�x���g�J�n�҂��L�����Z��
GLOBAL int CommExitWaitStart( void );		//�ʐM�I���X�^�[�g


#undef GLOBAL
#endif	/*FLD_MAIN_H*/

