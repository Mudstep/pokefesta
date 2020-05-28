
#ifndef __INITGAME_H__
#define __INITGAME_H__

//=========================================================================
//	
//=========================================================================

#undef GLOBAL
#ifdef __INITGAME_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

//DebugInitGame �̃^�C�v�w��
enum{
	DEBUG_START_NORMAL,
	DEBUG_START_FOREIGNER,
};

//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

extern u8	FirstSaveFlag;			//�ŏ�����Q�[�����͂��߂����ǂ����̃t���O


//�f�o�b�O�o�[�W�����̂ݗL����f�o�b�O���[�h�t���O
#ifdef	PM_DEBUG
extern u8 DebugMainFlag;
#endif

#ifdef	FESTA_VER_ROM
extern u8 FestaModeFlag;
#endif

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

//-----------------------------------------------------------------
//	ID(32�r�b�g���j���Z�b�g����
//	<����>
//		id_value	ID�i���o�[(u32�̒l)
//		set_pos		�i�[�ꏊ�̃A�h���X(u8�̔z��)
//-----------------------------------------------------------------
GLOBAL void ID_Set( u32 id_value, u8 * set_pos );

//-----------------------------------------------------------------
//	ID(u8�~4�j���R�s�[����
//	<����>		to		�R�s�[��A�h���X
//				from	�R�s�[���A�h���X
//-----------------------------------------------------------------
GLOBAL void ID_Copy( u8 * to, u8 * from );

//	�Q�[���i�s�p���[�N�G���A�̏�����
//	���Z�[�u����Ȃ����[�N�̏�����
GLOBAL void InitGameWork( void );

//	�v���C���[�V�X�e���Z�[�u�f�[�^�G���A�̃N���A
GLOBAL void SysSaveDataInit( void );

//	�ʏ�Q�[���J�n�f�[�^��ݒ�
GLOBAL void InitGame( void );

#ifdef	PM_DEBUG
//	�f�o�b�O���[�h�ł̃Q�[���J�n��ݒ�
GLOBAL void DebugInitGame( int type );
#endif

//	�Q�[���I�[�o�[���̃p�����[�^�ݒ�
GLOBAL void RestartGame( void );

//	������ƂԎ��̃t���O���Z�b�g
GLOBAL void InitEventFlagAtFlying( void );

//	�e���|�[�g���̃t���O���Z�b�g
GLOBAL void InitEventFlagAtTeleport( void );

//	���Ȃʂ����̃t���O���Z�b�g
GLOBAL void InitEventFlagAtEscape( void );

//	�Q�[���I�[�o�[���̃t���O���Z�b�g
GLOBAL void InitEventFlagAtGameOver( void );

//	�R���e�B�j���[���̃t���O���Z�b�g
GLOBAL void InitEventFlagAtContnue( void );


// �}�b�v�ؑցidebug�p�j
GLOBAL void DebugSetNextMap(void);

//=========================================================================
//=========================================================================

#undef GLOBAL
#endif	/*__INITGAME_H__*/

