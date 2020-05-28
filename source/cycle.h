//**********************************************************************
//		cycle.h
//**********************************************************************
#ifndef	CYCLE_H
#define CYCLE_H

#undef	EXT
#ifdef	CYCLE_H_PROTO
#define	EXT	/**/
#else
#define	EXT	extern
#endif

enum
{
	CYCLE_S_NORMAL = 0,											// �ʏ�
	CYCLE_S_TURN,												// �U�����
	CYCLE_S_FTUP,												// ����
	CYCLE_S_DANIEL,												// ��ƴ�
	CYCLE_S_WHEELIE,											// ��ذ
	CYCLE_S_KANI,												// ��
	CYCLE_S_JUMP_TURN,											// �ެ������
};

#define HERO_MOVE_SPEED_1	(0x01)								// ���@���x
#define HERO_MOVE_SPEED_2	(0x02)
#define HERO_MOVE_SPEED_3	(0x03)
#define HERO_MOVE_SPEED_4	(0x04)
#define HERO_MOVE_SPEED_MAX HERO_MOVE_SPEED_4

EXT void HeroMoveCycle(u8,u16,u16);								// ���]�ԓ��쐧��
EXT void CycleCommandSet(u16,u16);								// ���]�Ժ���޾��

EXT void MyCycleWorkInit(u32,u32);								// ���]��ܰ�������
EXT void MyCycleRequest(u8);									// ���]��ظ���
EXT u8 CheckRideCycle(void);									// ���]�ԏ������
EXT u8 CheckFtDownEnableCycle(void);							// ���]�������޳݉\����
EXT u8 CycleBatMoveAttrCheck(u8);								// ���]�Ԉړ��\����
EXT s16 GetHeroMoveSpeed(void);									// ���@�ړ����x�擾
EXT void CycleSpeedBrakeSet(u8);								// ���]�ԑ��x���
EXT void SetHeroCycleStatus(void);								// ���]�Խð�����

#endif	// CYCLE_H
