//**********************************************************************
//		evobjeff.h
//**********************************************************************
#ifndef EVOBJEFF_H
#define	EVOBJEFF_H

#include "evobj.def"

#undef	EXT
#ifdef 	EVOBJEFF_H_PROTO
#define	EXT	/**/
#else
#define	EXT extern
#endif

enum														// �f�荞�ݎ��
{
	EOE_REF_NORMAL,											// �g�k�f�荞��
	EOE_REF_ICE,											// ��g�k�f�荞��
};

enum														// �B�ꖪ����
{
	HIDE_NOTHING = 0,										// �B�ꖪ����
	HIDE_ON,												// �B�ꖪ�\��
	HIDE_OFF,												// �B�ꖪ��\��
};

enum														// �g�߹����
{
	NAMIPOKE_NOWITH = 0,									// ��������
	NAMIPOKE_WITH,											// ����
	NAMIPOKE_NOWITH_SWAY,									// ��������&�����
};

enum														// �g�߹�̾��
{
	NAMIPOKE_VOFFSET_OFF = 0,								// �w�薳��
	NAMIPOKE_VOFFSET_ON,									// �̾�ı�
};

enum														// �g�߹��������
{
	NAMIPOKE_SITE_NOCHG = 0,								// �����ύX
	NAMIPOKE_SITE_CHG,										// �ύX����
};

EXT void EvActReflectSet(EventOBJ*,actWork*,u8);			// �f�荞�ݾ��

EXT u8 EvActExitCursorSet(void);							// �o�����پ��
EXT void EvActExitCursorOFF(u8);							// ���ٔ�\��
EXT void EvActExitCursorON(u8,u8,s16,s16);					// ���ٕ\��

EXT void FeAshMake(s16,s16,u16,s16);						// �D���
EXT void EvActHideClsSet(EventOBJ*);						// �B�ꖪ����
EXT u8 EvActHideClsChk(EventOBJ*);							// �B�ꖪ��������

EXT void SetPokeSwimMoveFlag(u8,u8);						// �g�߹�����׸޾��
EXT void SetPokeSwimSiteFlag(u8,u8);						// �g�߹�����׸޾��
EXT void SetPokeSwimOffsetFlag(u8,u8,s16);					// �g�߹�̾���׸޾��

EXT u8 EvActDivingSwaySet(u8);								// �޲��ݸޗh�걸�����

EXT void EvActShadeMove(actWork*);							// �e��������
EXT void FeShortGrassShakeMove(actWork*);					// �Z�����h�ꓮ��
EXT u8 CheckFeShortGrass(u8,u8,u8,s16,s16);					// �Z�����h������
EXT void FeLongGrassShakeMove(actWork*);					// �������h�ꓮ��
EXT void EvActFootPrintMove(actWork*);						// ���Փ���
EXT void EvActSplashMove(actWork*);							// ���򖗓���
EXT void EvActAshMove(actWork*);							// �D����
EXT void EvActPokeSwimMove(actWork*);						// �g����߹�ݓ���
EXT void EvActDeepSandMove(actWork*);						// �[��������
EXT void EvActSmallGrassMove(actWork*);						// ������������
EXT void EvActOnsenMove(actWork*);							// ���򓮍�
EXT void EvActTrainerHideMove(actWork*);					// �B�ꖪ����
EXT void EvActAwaMove(actWork*);							// �A����
EXT void EvActKirariMove(actWork*);							// ����ؓ���

EXT void EvActCommonMove0(actWork*);						// ���ʓ���0
EXT void EvActCommonMove1(actWork*);						// ���ʓ���1

#undef EXT
#endif // EVOBJEFF_H
