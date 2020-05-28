//-------------------------------------------------------------
//				�p�\�R���{�b�N�X��define
//-------------------------------------------------------------
#ifndef  _PASOCOM_H_
#define  _PASOCOM_H_

#include "param.h"

#define YOKO_KAZU	(6)			//���ɉ��̃|�P�\�����邩
#define TATE_KAZU	(5)			//�c�ɉ��̃|�P�\�����邩
#define BOX_KAZU	(14)		//�{�b�N�X�̐�
#define BOX_PM_KAZU (YOKO_KAZU*TATE_KAZU)//��̃{�b�N�X�̃|�P�̐� 
#define PM_TAIHI	(BOX_PM_KAZU*2)//BoxPMStruct�̈�Ԍ��
#define BOXNAME_MAX  (8)


typedef struct{
	u8 CurrentBoxNo;
	PokemonPasoParam	PPPBox[BOX_KAZU][BOX_PM_KAZU];
	u8 BoxName[BOX_KAZU][9];
	u8 BGNum[BOX_KAZU];
}POKEBOX_SAVE_DATA;

extern POKEBOX_SAVE_DATA  BoxData;

/*
extern u8 CurrentBoxNo;		//���ݑI�����Ă���{�b�N�X�̃i���o�[
extern PokemonPasoParam	PPPBox[BOX_KAZU][BOX_PM_KAZU];//�{�b�N�X�̗̈���m��
extern u8 PasoBoxName[BOX_KAZU][BOXNAME_MAX+1];
extern u8 BoxBGNumber[BOX_KAZU];
*/

extern void SaveBoxData( POKEBOX_SAVE_DATA * box );
extern void LoadBoxData( POKEBOX_SAVE_DATA * box );

#endif

