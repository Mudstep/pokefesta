
#ifndef __SAFARI_H__
#define __SAFARI_H__

//=========================================================================
//	
//		�T�t�@���֘A�w�b�_
//
//=========================================================================

#undef GLOBAL
#ifdef __SAFARI_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

#define	SAFARI_BALL_MAX		30
#define	SAFARI_STEP_MAX		500


//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

//�T�t�@���{�[���̐�
extern u8 SafariBallCount;
extern u16 SafariStepCount;

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

GLOBAL int IsFieldSafariMode( void );//	�T�t�@�����[�h���ǂ����̃`�F�b�N
GLOBAL void SetFieldSafariMode( void );
GLOBAL void ClearFieldSafariMode( void );

GLOBAL void SafariModeStart( void );	//	�T�t�@�����[�h�J�n
GLOBAL void SafariModeEnd( void );		//	�T�t�@�����[�h�I��
GLOBAL u8 SafariStepCheck( void );		//	�T�t�@���̕����`�F�b�N

//	�T�t�@�����[�h����߂邩�ǂ����������˂�X�N���v�g���Ăяo��
GLOBAL void SafariCancelAskStart( void );	

//	�T�t�@���ł̃G���J�E���g��I����̏���
GLOBAL void SafariEncountEnd( void );

//-------------------------------------------------------------------------
//	�L���[�u�u����̃f�[�^��������
//-------------------------------------------------------------------------
GLOBAL void SafariCubeTableInit( u8 no );	// �X
GLOBAL void SafariCubeTableInitAll(void);	// �S��

//-------------------------------------------------------------------------
//	�L���[�u�u����ɃL���[�u���u����Ă��邩
//	�u����Ă���   : AnswerWork = i
//	�u����Ă��Ȃ� : AnswerWork = 0xffff
//-------------------------------------------------------------------------
GLOBAL void SafariCubeTableCheck(void);

//-------------------------------------------------------------------------
//	�L���[�u�u����̃L���[�u�̃p�����[�^���擾
//	�߂�l : �u����Ă���   = �L���[�u�f�[�^�̃A�h���X
//		   : �u����Ă��Ȃ� = NULL
//-------------------------------------------------------------------------
GLOBAL CUBE * SafariCubeTableGet(void);
GLOBAL CUBE * SafariCubeTableGet2(void);

//-------------------------------------------------------------------------
//	�L���[�u�u����ɃL���[�u���Z�b�g
//	���� : no = �莝���L���[�u�̔ԍ�
//-------------------------------------------------------------------------
GLOBAL void SafariCubeTableSet( u8 no );

//-------------------------------------------------------------------------
//	�f�o�b�O�p�L���[�u���擾
//	<�߂�l>	FALSE		�L���[�u�����݂��Ȃ�
//				TRUE		�L���[�u����
//	StrTempBuffer0 = ���O�AStrTempBuffer1 = �c�����
//-------------------------------------------------------------------------
GLOBAL int DebugSafariCubeInfo( void );

#undef GLOBAL
#endif	/*__SAFARI_H__*/

