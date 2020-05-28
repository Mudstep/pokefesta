/***********************************************************************
**	�ȈՉ�b�i�����w�b�_�j
**
**	Masao Taya
**
***********************************************************************/
#ifndef _KAIWA_COM_H_
#define _KAIWA_COM_H_

/*====================================================================*/
/*	�C���N���[�h                                                      */
/*====================================================================*/
#include "common.h"
#include "intr.h"
#include "actanm.h"
#include "actor.h"
#include "palanm.h"
#include "menu.h"
#include "boxsub.h"
#include "calctool.h"
#include "namein.h"  // ���O���͂Ɠ����J�[�\���摜���g������
#include "kaiwa.h"
#include "kaiwa_bg.h"
#include "kaiwa_code.h"

/*====================================================================*/
/*	�萔                                                              */
/*====================================================================*/
#define KAIWA_CODE_MAX     (9)     // �R�[�h���̍ő�l
#define KAIWA_LINE_MAX     (3)     // �R�[�h���͗��̍ő�s��
#define KAIWA_ROW_MAX      (3)     // �R�[�h���͗��̍ő��
#define KAIWA_STR_MAX      (8)     // �P��̍ő啶����
#define KAIWA_MSGLEN_MAX   (22)    // �\�����b�Z�[�W�̍ő啶�����i�P�s������j
#define KAIWA_CATEGORYLEN_MAX  (6) // �J�e�S�����̍ő啶����
#define KAIWA_CATEGORYBUF_SIZE ((KAIWA_CATEGORYLEN_MAX+1)*3)

#define KAIWA_GROUP_COL_MAX  (8)  // �J�e�S���E�B���h�E�̍ő�s��

#define KAIWA_WORDLEN_MAX      (7)
#define KAIWA_WORDSPACE_LEN    (2)
#define KAIWA_WORDAREA_LEN     (KAIWA_WORDLEN_MAX+KAIWA_WORDSPACE_LEN)

#define KAIWA_WORDWIN_ROW      (3)
#define KAIWA_WORDLINEBUF_SIZE ((KAIWA_WORDAREA_LEN)*KAIWA_WORDWIN_ROW)
#define KAIWA_WORDWIN_COL_MAX   (86)
#define KAIWA_WORDWIN_COL       (4)
#define KAIWA_WORD_X  (1)
#define KAIWA_WORD_Y  (11)

#define KAIWA_WORDCURSOR_XORG  (20)
#define KAIWA_WORDCURSOR_YORG  (96)

#define KAIWA_WINGRAD_STEP  (9)   // �E�B���h�E�O���f�[�V�����̒i�K��
#define KAIWA_WINGRAD_CNT   (3)   // �O���f�[�V����������F��

#define KAIWA_INITIAL_CNT  (45)  // �������������[�h�̃O���[�v��
#define KAIWA_INITIAL_WORD_MAX  (120) // �������������[�h�̍ő�P�ꐔ

#define KAIWA_WORDLINE_BUFSIZE   (64)

/*============================================*/
/*	��ʃ��C�A�E�g�^�C�v                      */
/*============================================*/
enum KAIWA_LAYOUT_TYPE {
	KAIWA_LAYOUT_3x2,
	KAIWA_LAYOUT_3x3,
	KAIWA_LAYOUT_1x1,
	KAIWA_LAYOUT_2x1,
	KAIWA_LAYOUT_2x2,
	KAIWA_LAYOUT_2x2B,  // 2x2 ����ޭ���\��Ӱ��
};

/*============================================*/
/*	��ʃ��C�A�E�g                            */
/*============================================*/
#define KAIWA_MSGWIN_X1  (3)
#define KAIWA_MSGWIN_Y1  (14)
#define KAIWA_MSGWIN_X2  (26)
#define KAIWA_MSGWIN_Y2  (19)
#define KAIWA_MSG_X  (KAIWA_MSGWIN_X1 + 1)
#define KAIWA_MSG_Y  (KAIWA_MSGWIN_Y1 + 1)

#define KAIWA_COMMENU_X      (1)
#define KAIWA_COMMENU_Y      (9)
#define KAIWA_COMMENU_WIDTH  (28)
#define KAIWA_COMMENU_HEIGHT (4)

#define KAIWA_YESNO_X    (23)
#define KAIWA_YESNO_Y    (8)

#define KAIWA_2x2FRAME_X       (5)
#define KAIWA_2x2FRAME_Y       (3)
#define KAIWA_2x2FRAME_WIDTH   (20)
#define KAIWA_2x2FRAME_HEIGHT  (6)

#define KAIWA_2x2B_FRAME_X       (9)
#define KAIWA_2x2B_FRAME_Y       (3)
#define KAIWA_2x2B_FRAME_WIDTH   (20)
#define KAIWA_2x2B_FRAME_HEIGHT  (6)

#define KAIWA_3x2FRAME_X       (0)
#define KAIWA_3x2FRAME_Y       (3)
#define KAIWA_3x2FRAME_WIDTH   (30)
#define KAIWA_3x2FRAME_HEIGHT  (6)

#define KAIWA_3x3FRAME_X       (0)
#define KAIWA_3x3FRAME_Y       (1)
#define KAIWA_3x3FRAME_WIDTH   (30)
#define KAIWA_3x3FRAME_HEIGHT  (8)

#define KAIWA_1x1FRAME_X       (15)
#define KAIWA_1x1FRAME_Y       (3)
#define KAIWA_1x1FRAME_WIDTH   (11)
#define KAIWA_1x1FRAME_HEIGHT  (4)

#define KAIWA_2x1FRAME_X       (5)
#define KAIWA_2x1FRAME_Y       (2)
#define KAIWA_2x1FRAME_WIDTH   (20)
#define KAIWA_2x1FRAME_HEIGHT  (4)

#define KAIWA_CATEGORY_X   (2)
#define KAIWA_CATEGORY_Y   (11)

#define KAIWA_CATEGORYWIN_X      (1)
#define KAIWA_CATEGORYWIN_Y      (10)
#define KAIWA_CATEGORYWIN_WIDTH  (22)
#define KAIWA_CATEGORYWIN_HEIGHT (10)

/*============================================*/
/*	�\�����b�Z�[�W�^�C�v                      */
/*============================================*/
enum KAIWA_MSG {
	KAIWA_MSG_INFO,
	KAIWA_MSG_KAKUNIN,
	KAIWA_MSG_ERASE,
	KAIWA_MSG_YAMERU,
	KAIWA_MSG_YAMERU_WARN,
	KAIWA_MSG_KETTEI_WARN,
	KAIWA_MSG_ERASE_WARN,
	KAIWA_MSG_1WORD,
	KAIWA_MSG_POP_WARN,
	KAIWA_MSG_POP_BLANK_WARN,

	KAIWA_MSG_HIDE,
};
/*============================================*/
/*	�P��I�����[�h                            */
/*============================================*/
enum KAIWA_SELECT_MODE {
	KAIWA_SEL_CATEGORY, KAIWA_SEL_CHAR,
};
/*============================================*/
/*	��`�J�[�\�����샂�[�h                    */
/*============================================*/
enum KAIWA_SQUARE_MODE {
	SQUARE_MODE_CATEGORY,// �V����
	SQUARE_MODE_MENU,    // �S����
	SQUARE_MODE_3CHAR,   // �R����
	SQUARE_MODE_1CHAR,   // �P����
};
/*============================================*/
/*	�X�N���[����󓮍샂�[�h                  */
/*============================================*/
enum KAIWA_ARROW_MODE {
	ARROW_MODE_GROUP,  // �O���[�v�I����
	ARROW_MODE_WORD,   // �P��I����
};
/*============================================*/
/*	�J�[�\������ݒ�                          */
/*============================================*/
enum KAIWA_CURSOR {
	KAIWA_CURSOR_MOVE,
	KAIWA_CURSOR_STOP,
	KAIWA_CURSOR_BANISH,
};


/*====================================================================*/
/*	���[�N�̈�                                                        */
/*====================================================================*/
typedef struct {
	pFunc ret_func;    // �Ăяo������ʂւ̕��A�֐�
	u16*  code_addr;   // ��b�R�[�h�i�[�A�h���X
	u8    mode;        // ���̓��[�h
	u8    layout;       // ��ʃ��C�A�E�g�^�C�v
	u8    code_size;    // ��b�R�[�h�T�C�Y
	u8    interviewer;  // �C���^�r���A�[�^�C�v
	u16   code_work[KAIWA_CODE_MAX];  // �ҏW���R�[�h

	pFunc sub;         // �T�u�v���Z�X
	u16  prog;         // �V�[�N�G���X�̐i��

	u8   select_mode;  // �P��I�����[�h�i�J�e�S�� or �����������j
	u8   edit_word_no; // �ҏW���̒P��ԍ�
	u16 enable_category_cnt;  // �L���J�e�S����

	u8  category_tbl[KAIWA_GROUP_COL_MAX][3];
	u8  charcode_tbl[4][14];

	// �B���J�e�S���o���t���O
	u8  hidden_category_flg[KWC_HIDDEN_CNT];

	// �V��������
	u8   limit7_flg;   // �V������������^�Ȃ�
	u8   limitword_cnt[KAIWA_LINE_MAX];

	// �J�[�\������
	s8  cursor_xlimit, cursor_ylimit;
	s8  cursor_xpos, cursor_ypos;
	u8  cursorpos_update;
	u16 cursor_xorg, cursor_yorg;
	u8 cursor_xofs[KAIWA_LINE_MAX][KAIWA_ROW_MAX];

	// ��`�J�[�\������
	u8  squarepos_update;

	// �A�N�^�[
	actWork *cursor_actor;
	actWork *squareL_actor;
	actWork *squareR_actor;
	actWork *wordcursor_actor;
	actWork *arrow_actor[2];
	actWork *pagechange_actor[2];

	// �E�B���h�E�`��
	u8  trans_rect_flag;
	u16 draw_win_prog, draw_win_cnt;
	u16 draw_win_rectcnt, draw_win_roopcnt;
	TRECT    trect[8];
	SRC_ORG  src_org[8];

	// �O���[�v�I���E�B���h�E
	s8  group_col, group_row;                // ���ݍs�C��
	s8  group_max_row[KAIWA_GROUP_COL_MAX];  // �ő��
	s8  group_base_col;                      // �\����s
	s8  category_max_col;                    // �J�e�S�����[�h���̍ő�s
	s8  group_selmenu_flg;                   // ���j���[�I���t���O
	u8  select_group_num;                    // �I�������O���[�v�ԍ�
	u8 wordcursor_update;

	// �X�N���[������
	u16 scroll_vofs, scroll_end_vofs;
	u8  scroll_speed;
	s8  scroll_add;
	s8  scroll_height;
	pFunc scroll_ret_func;

	// �}�b�v�f�[�^�W�J�̈�
	u16 frame_bg_Map[30*20];  // 1200 bytes
	u16 frame_2x2_Map[20*6];  //  240 bytes
	u16 frame_3x2_Map[30*6];  //  360 bytes
	u16 frame_3x3_Map[30*8];  //  480 bytes
	u16 frame_2x1_Map[20*4];  //  160 bytes
	u16 frame_1x1_Map[11*4];  //   88 bytes
	u16 frame_selwin_Map[30*10];  // 600 bytes
	u16 frame_selwinsub_Map[6*8]; //  96 bytes
	u32 checksum;

	// �����������\�[�g
	u16 charsort_work[KWC_MAX];
	u16 charsort_tbl[KAIWA_INITIAL_CNT][KAIWA_INITIAL_WORD_MAX];
	u16 initial_word_cnt[KAIWA_INITIAL_CNT];
	u16 category_word_cnt[KWC_MAX];
	const u8 *word_tbl[KWC_MAX][256];

	// �P��I���E�B���h�E
	s8 word_col, word_row;
	s8 word_max_row[KAIWA_WORDWIN_COL_MAX];
	s8 word_max_col;
	s8 word_base_col;
	u16  word_code_tbl[KAIWA_WORDWIN_COL_MAX][KAIWA_WORDWIN_ROW];

	// �E�B���h�E�O���f�[�V����
	u8  grad_step;
	u16 grad_tbl[KAIWA_WINGRAD_CNT][KAIWA_WINGRAD_STEP];

	// �X�N���[�����
	s8 *arrow_base_col;
	u8 arrow_disp;
	s8 arrow_max_col;

	// ���[�h�W��
	actWork *mode_actor;

	// ��۰ٗ̈�S�����p
	u16  clear_line_prog;
	u16  clear_line_timer;

	// ���s����͗̈�
	u16 pop_word[2];

	// ���b�Z�[�W�̈�
	u8  info_msg1[KAIWA_MSGLEN_MAX + 1];
	u8  info_msg2[KAIWA_MSGLEN_MAX + 1];
	u8  kakunin_msg1[KAIWA_MSGLEN_MAX + 1];
	u8  kakunin_msg2[KAIWA_MSGLEN_MAX + 1];
	u8  spc_msg[KAIWA_MSGLEN_MAX + 1];

	u8  category_str[KAIWA_CATEGORYBUF_SIZE + 1];
	u8  word_buf[KAIWA_WORDAREA_LEN + 1];
	u8  word_buf2[KAIWA_WORDAREA_LEN + 1];  // ��r�p

	u8  wordline_buf[KAIWA_WORDLINE_BUFSIZE];  // �P�s�p
	u8  wordline_buf2[KAIWA_WORDLINE_BUFSIZE];  // �P�s�p

	u8  spaceword_buf[32];
	u8  caption_buf[48];

}KAIWA_WORK;

extern KAIWA_WORK * const KaiwaWork;

/*====================================================================*/
/*	�v���g�^�C�v                                                      */
/*====================================================================*/
// kaiwa_obj.c
void KaiwaObjInit(void);
void KaiwaCursorMove(u8 flg);
void KaiwaSquareAdd(void);
void KaiwaSquareDel(void);
void KaiwaSquareModeChange(void);
void KaiwaWordCursorDisp(u8 flg);
void KaiwaWordCursorMove(u8 flg);
void KaiwaArrowHide(void);
void KaiwaArrowModeChange(u8 mode);
void KaiwaObjInterviewerAdd(u8 type, u8 layout);
void KaiwaModeFlagInit(void);
void KaiwaModeFlagChange(void);
void KaiwaModeFlagAppear(u8 appear);

// kaiwa_bg.c
void KaiwaBGInit(void);
void KaiwaScrollBGParamInit(void);
void KaiwaScrollInit(u8 speed);
void DrawKaiwaComMenu(void);
void DrawKaiwaMsg(u8 type);
void DrawKaiwaGroupName(void);
void ReDrawKaiwaGroupName(void);
void KaiwaDrawWinIni(void);
void DrawKaiwaCategoryName(void);
u8 KaiwaScroll(void);
u8 KaiwaWin_GroupDraw(void);
u8 KaiwaWin_GroupErase(void);
u8 MinimizeKaiwaGroupWin(void);
u8 MaxmizeKaiwaGroupWin(void);
u8 KaiwaWin_GroupToWord(void);
u8 KaiwaWin_WordToGroup(void);
u8 KaiwaWin_WordErase(void);
void ClearKaiwaScrollBG(void);
void DrawKaiwaWordAll(void);
void DrawKaiwaWordScroll(void);
void RectTransMain(void);
void KaiwaBG_DrawCaption(u8 mode);
void WriteEditArea(void);
void WriteEditAreaKakunin(void);
void EraseEditArea(void);

// kaiwa_dat.c
void KaiwaInitWordSort(void);
void KaiwaInitWordPointerTbl(void);
void KaiwaInitWordWindowData(void);
u8* KaiwaCopyCategoryName(u8 *dst, u8 category, u16 spc_len);
u8 * KaiwaCodeToWord(u8 *dst, u16 code, u16 space_len);
u16 KaiwaCodeLen(u16 code);
u8 KaiwaPoke2CategoryCheck(void);
u8 KaiwaIsEnableCategory(u8 category);

#endif /* #ifndef _KAIWA_COM_H_ */

