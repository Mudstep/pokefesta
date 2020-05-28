/***********************************************************************
**	�{�b�N�X�|�X�C�b�`�I���E�I�t�G�t�F�N�g
**
***********************************************************************/
#ifndef _BOX_EF_H_
#define _BOX_EF_H_

/*======================================
==	�\���̒�`
======================================*/
typedef struct {
	/* InitBoxOnEffect, InitBoxOffEffect �Ăяo���O�ɏ��������Ă��� */

	u16 cell_id;     // �����A�N�^�[�Ɋ��蓖�Ă�Z���h�c
	u16 pal_id;      // �����A�N�^�[�Ɋ��蓖�Ă�p���b�g�h�c
	u16 line_speed;  // �����̃��C�����L�т�i�k�ށj���x�B 0 �Ȃ�f�t�H���g�l���g���܂�
	u16 open_speed;  // ��ʂ��J���i����j���x�B 0 �Ȃ�f�t�H���g�l���g���܂�

	/* �����ŏ���ɑ��삷�� */
	u16 prog_no;
	u16 del_act_cnt;
	s16 line;
	u32 pal_mask;

}SCAN_WORK;

/*======================================
==	�֐��v���g�^�C�v
======================================*/
void InitBoxOnEffect(SCAN_WORK *work);
u8 CheckBoxOnEffect(void);
void InitBoxOffEffect(SCAN_WORK *work);
u8 CheckBoxOffEffect(void);

#endif
