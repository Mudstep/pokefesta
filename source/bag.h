//==========================================================
//					�A�C�e���f�[�^
//							2001/08/27	by nakahiro
//==========================================================

#include "common.h"
//#include "print.h"
#include "screen.h"		// dmy_BG �Q�Ƃ̂���

// �A�C�e����ʗp�_�~�[�a�f��`
#define BG1_data	(dmy_BG[1])
#define BG2_data	(dmy_BG[2])
#define BG3_data	(dmy_BG[3])

//�A�C�e���f�[�^
// item.h �Ɉړ�

//�莝���A�C�e���f�[�^
// ��gamedata.h ��MINEITEM���Q��

//�A�C�e�����X�g
typedef struct {

	u8	max;		//�R�}���h��
	const u8 *com;	//�R�}���h

}ITEMLIST;

//�|�P�b�g�f�[�^
typedef struct {

	MINEITEM	*item;	//�A�C�e���f�[�^
	const u8	max;	//�ő吔

}POCKET;

// �o�b�O�p�J�E���^
typedef struct {
	u8	pos;		// �A�C�e���I���J�[�\���|�C���g
	u8	scr;		// �X�N���[���J�E���^�[
	u8	item_max;	// �����A�C�e����ސ�
	u8	pos_max;	// �J�[�\���|�C���g�ő吔
}BAG_STATUS;

extern void ItemBGBoxWrite(u16 *,u8,u8,u8,u8,u16);	//BG���w��̃L�����œh��Ԃ�

extern void EndBagItemError(u8);	//�G���[��̕��A�����i�o�b�O�j
extern void EndFldItemError(u8);	//�G���[��̕��A�����i�t�B�[���h�j

