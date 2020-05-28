#ifndef __EV_FLAG_H__
#define __EV_FLAG_H__

//=========================================================================
//
//	�C�x���g�t���O����֘A
//	
//=========================================================================

#undef GLOBAL
#ifdef EV_FLAG_H_GLOBAL
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

// �X�N���v�g�����ł̏����p
extern u16 ScriptRegister0;
extern u16 ScriptRegister1;
extern u16 ScriptRegister2;
extern u16 ScriptRegister3;

// �X�N���v�g�ƃv���O�����̃f�[�^�����p
extern u16 ScriptParameter0;
extern u16 ScriptParameter1;
extern u16 ScriptParameter2;
extern u16 ScriptParameter3;

// �X�N���v�g�ł̃e���|����
extern u16 ScriptTempWork0;
extern u16 ScriptTempWork1;
extern u16 ScriptTempWork2;
extern u16 ScriptTempWork3;

// �X�N���v�g�ɓ�����Ԃ��ėp���[�N
extern u16 AnswerWork;

// �b�������Ώۂ̂n�a�i�̂h�c
extern u16 TargetObjID;

// �b������������ێ����郏�[�N
extern u16 TalkSiteWork;

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

// �C�x���g�t���O�̑���֘A
GLOBAL void EventDataInit( void );		// �C�x���g�t���O�E���[�N�̏�����
GLOBAL void LocalEventFlagClear(void);	// �}�b�v������̃t���O���N���A
GLOBAL void OneDayFlagClear(void);		// ����o�ߌ���t���O�̃N���A

GLOBAL u8 EventFlagSet(u16 flag);		// �C�x���g�t���O�Z�b�g
GLOBAL u8 EventFlagReset(u16 flag);		// �C�x���g�t���O��Z�b�g
GLOBAL u8 EventFlagCheck(u16 flag);		// �C�x���g�t���O�`�F�b�N�AON/OFF��Ԃ�

GLOBAL u16 * GetEventWorkAdrs(u16 id);				// �C�x���g���[�N�A�h���X���擾
GLOBAL u16 GetEventWorkValue( u16 id );				// �C�x���g���[�N�̒l���擾
GLOBAL int SetEventWorkValue( u16 id, u16 value );	// �C�x���g���[�N�ɒl���Z�b�g

GLOBAL u8 GetEvDefineObjCode( u8 no );	// �X�N���v�g����w�肷��n�a�i�R�[�h���擾

//	������̑S�����[�h�A�z�E�G�����[�h�֘A
GLOBAL void ZukanSpecialFlagInit( void );		//������
GLOBAL void ZukanSpecialFlagSet( void );		//�Z�b�g
GLOBAL int ZukanSpecialFlagCheck( void );		//�`�F�b�N

//	�O���f�[�^�̎g�p���֘A
GLOBAL void ExDataSpecialFlagInit( void );		//������
GLOBAL void ExDataSpecialFlagSet( void );		//�Z�b�g
GLOBAL int ExDataSpecialFlagCheck( void );		//�`�F�b�N

//	�Q�[�������Ԃ̍Đݒ苖�֘A
GLOBAL void GameTimeSpecialFlagInit(void);		//������
GLOBAL void GameTimeSpecialFlagSet(void);		//�Z�b�g
GLOBAL int GameTimeSpecialFlagCheck(void);		//�`�F�b�N

#undef GLOBAL
#endif	/*__EV_FLAG_H__*/

