//=========================================================================================
//										�閧��n
//												2001/10/04 by nakahiro
//=========================================================================================
#ifndef	__BASE_H__
#define __BASE_H__


#include "sxy.h"
#include "ev_check.h"	/* EV_POS��`�Q�Ƃ̂��� */

extern void ChangeBaseCell(const SXY_DATA *);			// �}�b�v�؂�ւ����̊�n�m�F
extern void SetBaseGoods(u8);							// �O�b�Y��z�u����
extern void BaseOut(void);								// ��n���o�ă}�b�v�֕��A
extern u8 * MakeBaseName(u8 *);							// ��n�̖��O�쐬
extern void BaseDoorIn( EV_POS *, const SXY_DATA *);	// �\���L�[�Ŋ�n�̒���
extern void GetBaseNo(EV_POS *,const SXY_DATA *);		// ��n�ԍ��擾
extern void OrusubanBattle(void);						// ������ԃL�����ΐ�
extern u8	TalkBaseDoorCheck(void);					// �b�������̊m�F
extern void MyBaseCheck(void);							// �����̊�n�̂���Ȃ��`�F�b�N
extern u8	GetNameEndPos(u8 *);						// ��n�̖��O�f�[�^�̍Ō�����擾

extern void SetDummyOrusubanData(u8);			// �_�~�[������ԃL�����Z�b�g

// ��n�̓�����Z��
typedef struct{
	const u16	before;	// ���̃Z��
	const u16	after;	// �쐬��̃Z��
}BaseExit;


#endif /*__BASE_H__*/
