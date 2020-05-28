
//=========================================================================
//	
//=========================================================================

#ifndef EV_CHECK_H
#define EV_CHECK_H

#include "evobj.def"

#undef GLOBAL
#ifdef EV_CHECK_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

/* �C�x���g�`�F�b�N�Ɏg�����W�n�̍\���� */
typedef struct {
		s16 x;
		s16 y;
		s8	z;
}EV_POS;

/* �C�x���g���N�G�X�g�\���� */
typedef struct {
		u16		TalkCheck:1;
		u16		StepCheck:1;
		u16		MenuOpen:1;
		u16		CnvButton:1;
		u16		MatCheck:1;
		u16		PushCheck:1;
		u16		MoveCheck:1;
		u16		FloatCheck:1;

		u16		DebugMapChange:1;
		u16		DebugHook:1;
		u16		DebugMenu:1;
		u16		DebugDiving:1;
		u16		:4;

		u8		Site;
}EV_REQUEST;


//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

extern u8 TargetObjNo;

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================



GLOBAL void ClearRequest(EV_REQUEST * req);
GLOBAL void SetRequest( EV_REQUEST * req, u16 trg, u16 cont );
GLOBAL int CheckRequest( EV_REQUEST * req );


// �C�x���g�N���`�F�b�N�֘A
GLOBAL u8 DivingMapMoveCheck( EV_POS * pos, u16 attr );	//�u������v�̃}�b�v�ړ��`�F�b�N
GLOBAL u8 DivingMapCheck( void );

// �ȈՂn�a�i�b�������`�F�b�N
GLOBAL const VM_CODE * EvObjEasyTalkCheck( EV_POS * p, u8 attr, u8 site );
// �Ȉ�POS�C�x���g�`�F�b�N
GLOBAL const VM_CODE * EventPosEasyCheck( const EV_POS * p );


GLOBAL void GetHeroPosition( EV_POS * p );

//�G���J�E���g���䏉����
GLOBAL void InitEncountStep(void);

//�Ń_���[�W����̕���������
GLOBAL void PoisonStepInit(void);

//--------------------------------------------------------------
//	�����̑O�ɂ���n�a�i�̃`�F�b�N
//	���ǂ����g�p�̃`�F�b�N�ȂǂɎg��
//	���߂�l�͂��̂n�a�i�̃X�N���v�g�A�h���X�i���j
//	��������TargetObjNo���ݒ肳���
//--------------------------------------------------------------
GLOBAL const VM_CODE * FrontObjCheck(void);

#undef GLOBAL
#endif	/*EV_CHECK_H*/

