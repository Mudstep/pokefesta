/***********************************************************************
**	�}�[�L���O�E�B���h�E����
**	masao taya
***********************************************************************/
#ifndef _MARKING_H_
#define _MARKING_H_


typedef struct {
	u16 cell_id; // �g�p���Ă悢�Z���h�c�̐擪���l�i��������R�g�p�j
	u16 pal_id;  // �g�p���Ă悢�p���b�g�h�c�i��������Q�g�p�j
	u8  mark;    // �}�[�N��ԁi������Ԃ𔽉f����j

	s8  cursor;
	u8  mark_work[4];

	actWork *win_actor[2];
	actWork *mark_actor[4];
	actWork *cursor_actor;
	actWork *kettei_actor;

	const u8 *win_char;
	const u16 *win_pltt;
	u8  win_cell[32*128];
	u8  fontcell_work[32*4];
	u8  cellmake_line;

}MARKING_WORK;


void MarkingWorkInit(MARKING_WORK *work);
void MarkingCellMakeInit(void);
u8 MarkingCellMake(void);
void MarkingCellMakeNow(void);
void MarkingWindowDisp(u8 mark, s16 x, s16 y);
void MarkingWindowQuit(void);
u8  MarkingControl(void);
actWork* MarkingStateAnmObjAdd(u16 cell_id, u16 pal_id, const u16 *pal_addr);
actWork* MarkingStateObjAdd(u16 cell_id, u16 pal_id, const u16 *pal_addr);
void MarkingCharDataCopy(u8 mark, u8 *addr);

/* ---------------------------------------------------------------------
	�g����

�P�DMARKING_WORK�\���̂��P�p�ӂ��Amark, cell_id, pal_id �ɕK�v��
    �l���Z�b�g����B

�Q�DMarkingSelInit(u16 x, u16 y, MARKING_WORK *work)���ĂԁB
    x, y .... �}�[�L���O�E�B���h�E������̍��W�i�h�b�g�P�ʁj
    work ...  �P�D�ŗp�ӂ����\���̂̃A�h���X

    ���}�[�L���O�E�B���h�E���\�������

�R�D�ȍ~�A�O���A��܂� MarkingControl ��Sync�ĂԁB
    �O���A������AMarkingSelQuit() �Ń��\�[�X�J��

�S�D�E�B���h�E�����̃}�[�N��Ԃ��A�P�D�ŗp�ӂ��� MARKING_WORK�\����
    �� mark �ɃZ�b�g�����B

�T�D�}�[�N��Ԃ������n�a�i��

    MarkingStateObjAdd
    MarkingStateAnmObjAdd

    �́A�ǂ��炩�̊֐��ō쐬����B

    pal �́A�P�Ԃɔw�i�F�A�Q�Ԃɔ�I��F�A�R�ԂɑI��F�̓������p���b�g�B
    �A�N�^�[�ւ̃|�C���^���A��̂ŁA�ʒu��v���C�I���e�B���Z�b�g�B

    AnmObjAdd �ō쐬���� �A�N�^�[�́A
    �A�j���i���o�[�� MARKING_WORK�\���̂� mark �l�ɃZ�b�g���邱�ƂōX�V�B

    ObjAdd �ō쐬�����A�N�^�[�́A
    �L�����f�[�^�A�h���X��
    MarkingCharDataCopy �֓n���čX�V�B

	

���K�v�ȃ��\�[�X

�E�A�N�^�[�W��
�EOBJ VRAM     169�L�������i5,408 bytes�j
�EOBJ PALETTE  �P�{��

----------------------------------------------------------------------*/


#endif /* #ifndef _MARKING_H_ */
