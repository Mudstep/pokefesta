#ifndef __SXY_H__
#define __SXY_H__

//=========================================================================
//	
//	�r�w�x�f�[�^�p�\���̒�`
//
//=========================================================================

#include "evobjsv.def"

typedef evObjData SXY_OBJ;	/* evobj.def�Œ�`���Ă��� */

#undef GLOBAL
#ifdef SXY_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================

//�o������f�[�^�\��
typedef struct {
		u16 posX;					//�w�ʒu
		u16 posY;					//�x�ʒu
		u8	height;					//����
		u8 doorID;					//�o������h�c�i�}�b�v���œ���)
		u8 mapID;					//�}�b�v�h�c
		u8 divisionID;				//���ʂh�c
}SXY_CONNECT;

//�ʒu�����C�x���g�f�[�^�\��
typedef struct {
		u16 posX;					//�w�ʒu
		u16 posY;					//�x�ʒu
		u8	height;					//����
		u16	work;					//�Q�ƃ��[�N�R�[�h
		u8	number;					//��������ꍇ�̃i���o�[
		const VM_CODE * script;		//��������X�N���v�g�ւ̃|�C���^
}SXY_POS;

//BG�b�����������C�x���g
typedef struct {
		u16 posX;					//�w�ʒu
		u16 posY;					//�x�ʒu
		u8	height;					//����
		u8	type;					//�C�x���g�^�C�v
		const VM_CODE * script;		//��������X�N���v�g�ւ̃|�C���^
}SXY_BG;

//�r�w�x�f�[�^�\��
typedef struct {
		u8	ObjMax;
		u8	ConnectMax;
		u8	evPosMax;
		u8	evBgMax;
		const SXY_OBJ * ObjData;
		const SXY_CONNECT * ConnectData;
		const SXY_POS * PosData;
		const SXY_BG * BgData;
}SXY_DATA;


#undef GLOBAL
#endif	/*__SXY_H__*/
