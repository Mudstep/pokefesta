/***********************************************************************
**	�{�b�N�X�I���E�B���h�E�p�w�b�_
**
**	Masao Taya
***********************************************************************/
#ifndef  _BOXSEL_H_
#define  _BOXSEL_H_

/*=====================================
==	���[�N�p�\����
=====================================*/
typedef struct {
	actWork *SelActor;
	actWork *SelSideActor[4];
	actWork *SelTabActor[3];
	actWork *ArrowActor[2];
	u8 buf[20]; // �{�b�N�X���p�o�b�t�@
	u8 label_buf[512]; // ���x���L�����f�[�^�p�o�b�t�@
	u8 SelBoxNo;
	s8 BoxPmCnt;
	u8 actpri;
	u16 cell_id;
	u16 pal_id;
}BOXSEL_WORK;

/*=====================================
==	�萔
=====================================*/
#define BOXSEL_NEVER  (200)
#define BOXSEL_CANCEL (201)

/*=====================================
==	�v���g�^�C�v
=====================================*/
void InitBoxSelWindow(BOXSEL_WORK *work_addr, u16 cell_id, u16 pal_id, u8 actpri);
void QuitBoxSelWindow(void);
void BeginBoxSel(u8 box_no);
void EndBoxSel(void);
u8 BoxSelControl(void);

#endif /* #ifndef  _BOXSEL_H_ */
