//**********************************************************************
//		jiki.h
//**********************************************************************
#ifndef	JIKI_H
#define JIKI_H

#include "evobj.h"
#include "cycle.h"

#undef	EXT
#ifdef	JIKI_H_PROTO
#define	EXT	/**/
#else
#define	EXT	extern
#endif

EXT void EvObj_HeroMove(actWork*);								// ���@����
EXT void HeroMoveControl(u8,u16,u16);							// ���쐧��
EXT u8 HeroMoveCheckSite(u8);									// �ړ���������
EXT u8 HeroMoveCheck(EventOBJ*,s16,s16,u8,u8);					// �ړ�����
EXT void SetHeroMoveRequest(u16);								// ظ��ľ��
EXT void HeroMoveRequest(void);									// ظ���
EXT void HeroMvStateSet(void);									// ����󋵾��

EXT u8 HeroAcmdSetChk(void);									// ���������
EXT u8 HeroAcmdEndChk(void);									// ����ޏI������
EXT void HeroAcmdModeSet(u8);									// �����Ӱ�޾��
EXT u8 HeroAcmdMode(void);										// ��Ԃ�Ԃ�
EXT void HeroAcmdForceSet(u8);									// ��������޾��
EXT void HeroAcmdSet(u8,u8);									// ����޾��
EXT void HeroAcmdWalk1(u8);										// 1�ޯĈړ�
EXT void HeroAcmdWalk2(u8);										// 2�ޯĈړ�
EXT void HeroAcmdWalk3(u8);										// 3�ޯĈړ�
EXT void HeroAcmdWalk4(u8);										// 4�ޯĈړ�
EXT void HeroAcmdDash2(u8);										// 2�ޯ��ޯ���ړ�
EXT void HeroAcmdStopWalk(u8);									// ���̏����
EXT void HeroAcmdStopWalk05(u8);								// ���̏����0.5
EXT void HeroAcmdStop(u8);										// ��~
EXT void HeroAcmdWalkTurn(u8);									// �U�����
EXT void HeroAcmdJump(u8);										// �ެ���
EXT void HeroAcmdForceStop(void);								// ������~
EXT void HeroAcmdMtbFtUpSite(u8);								// ���ı���
EXT void HeroAcmdMtbFtUp(u8);									// ���ı���
EXT void HeroAcmdMtbFtDown(u8);									// �����޳�
EXT void HeroAcmdMtbDaniel(u8);									// ��ƴ�
EXT void HeroAcmdMtbDaniel1(u8);								// ��ƴ�1
EXT void HeroAcmdMtbDaniel2(u8);								// ��ƴ�2
EXT void HeroAcmdJumpTurn(u8);									// �ެ������
EXT void HeroAcmdMtbWheelie(u8);								// ��ذ
EXT void HeroAcmdMtbWheelieStart2(u8);							// ��ذ
EXT void HeroAcmdMtbWheelie2(u8);								// ��ذ
EXT void HeroAcmdMtbWheelieEnd2(u8);							// ��ذ

EXT void GetHeroFrontCellPos(s16*,s16*);						// ���@���ʍ��W�擾
EXT void GetHeroCellPos(s16*,s16*);								// ���W�擾
EXT u8 GetHeroSite(void);										// �����擾
EXT u8 GetHeroMoveSite(void);									// �ړ������擾
EXT u8 GetHeroHeight(void);										// �����擾
EXT void SetHeroMapPos(s16,s16);								// ���W�X�V
EXT u8 CheckHeroState(u8);										// ���@�������
EXT u8 GetHeroState(void);										// ���@��Ԏ擾
EXT void ResetHeroForceMove(void);								// �����ړ�����
EXT void SetHeroMoveStateInit(void);							// ����󋵏�����
EXT u8 GetHeroSexObjCode(u8,u8);								// ���ʕ���OBJ����
EXT u8 GetMainHeroSexObjCode(u8,u8);							// ���ʕ���OBJ����
EXT u8 GetMainHeroObjCode(u8);									// ���ʕ���OBJ����
EXT u8 GetHeroObjCodeSex(u8);									// OBJ����->����
EXT u8 GetMainHeroObjCodeSex(u8);								// OBJ����->����
EXT u8 CheckHeroNaminoriStart(void);							// �g���J�n����
EXT u8 CheckHeroTakinoboriStart(void);							// ��o��J�n����
EXT u8 CheckHeroAttrNaminoriStart(void);						// �g���J�n����ޭ������
EXT u8 GetHeroActorNumber(void);								// ����ܰ����ގ擾

EXT void MyMoveStateInit(void);									// MyMoveState������
EXT void MyMoveStateMvFlagSet(u8);								// �����׸޾��
EXT u8 MyMoveStateObjCodeFlag(u8,u8);							// OBJ����->�����׸�
EXT u8 MyMoveStateFlagObjCode(void);							// �����׸�->OBJ����
EXT void MyMoveStateObjCodeRequest(u8,u8);						// �׸޾��&ظ���

EXT void SetJikiActor(s16,s16,u8,u8);							// ���@�������
EXT void SetHeroObjVanish(u8);									// ���@OBJ�\��ON,OFF
EXT void ChangeHeroActorHiden(void);							// ��`�Z�����֕ύX
EXT void ChangeHeroActorFishing(u8);							// �ނ豸���֕ύX
EXT void ChangeHeroActorCycleFtUp(u8);							// ���]�����ı��ߕύX
EXT void ChangeHeroActorJorro(u8);								// �@�J�I�����֕ύX

EXT void SetHeroFishingTask(u8);								// �ނ�

#ifdef PM_DEBUG
EXT u8 HeroDebugMove(u8);										// ���ޯ��ړ�
#endif

#undef	EXT
#endif	// JIKI_H
