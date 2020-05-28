/***********************************************************************
**	�{�b�N�X��ʋ��ʒ萔
***********************************************************************/
#ifndef  _BOX_H_
#define  _BOX_H_


#include "pokeicon.h"
#include "print.h"
#include "menutool.h"
#include "pasocom.h"
#include "boxef.h"
#include "boxsel.h"

/*====================================================================*/
/* �{�b�N�X������                                                     */
/*====================================================================*/
u8 InitBoxCallMenu(void);
void InitBoxTureteiku(void);
void InitBoxAzukeru(void);
void InitBoxSeiri(void);

// �f�o�b�O���j���[����̌Ăяo���p
u8 InitBoxTureteiku_debug(void);
u8 InitBoxAzukeru_debug(void);
u8 InitBoxSeiri_debug(void);

//----------------------------------------------------------
//	���[�N�\����
//----------------------------------------------------------
typedef struct {
	u8   buf[256];
	u8   label_buf[512];
	u16  wp_cnt;

	u8 eva, evb, bldy;
	s8 select_menu;
	u8 comeback_flg;        // ����ʂ���̕��A���ǂ����H
	u8 boxmode;             //

// �}�[�L���O�֘A
	u16 mark_map_buf[8];
	u16 mark_win_map_buf[8];
	s8  mark_cursor, mark_cursor_before;
	u8  mark_bit, mark_bit_before;

	// 2001/09/28 �`

// box.c ---------------------------------------------------------------
	u8 ProgNo;              //  1 bytes  �e�v���Z�X�̐i����
	MENU_ITEM  BoxMenu[5];  // 40 bytes  ���j���[�A�C�e���p�o�b�t�@
	u8 MsgPokeName[11];     // 11 bytes  ���b�Z�[�W�\���Ɏg���|�P�������o�b�t�@
	u8 CatchSelectNo;       //  1 bytes  �|�P������͂񂾎��̑I��ԍ�

	u8 MineOn;              //  1 bytes  �莝���\���I���^�I�t�w��
	u8 MineCnt;             //  1 bytes  �莝���\���E�F�C�g�J�E���g
	u8 MineDispEnd;         //  1 bytes  �莝���\�������I���t���O

	u8 BoxBGTransPos;       //  1 bytes  �{�b�N�X�ǎ��]���̈�i0 or 1�j

	u8 kabegami_thema;
	// total = 57 bytes

	SCAN_WORK scan_work;

// boxobj.c ------------------------------------------------------------
	actWork *ButtonActor[2];      //  8 bytes  �؂�ւ��{�^���A�N�^�[
	actWork *WaveActor[2];        //  8 bytes  �g�`�A�N�^�[
	actWork *CloseButtonActor[3]; // 12 bytes  ����{�^���A�N�^�[
	actWork *StatusPokeActor;     //  4 bytes  �X�e�[�^�X�|�P�����A�N�^�[
	actWork *ShadowActor;         //  4 bytes  �J�[�\���e�A�N�^�[
	actWork *BoxNameActor[2];     //  8 bytes  �{�b�N�X���A�N�^�[
	actWork *NextBoxNameActor[2]; //  8 bytes  �X�N���[����{�b�N�X���A�N�^�[

	u16 StatusPoke_PalID;         //  2 bytes  �X�e�[�^�X�|�P�����p���b�g�h�c
	u16 StatusPoke_BeforeNo;      //  2 bytes  �X�e�[�^�X�|�P�����O��\�������X�^�[

	// �J�[�\���ړ�����
	u8 ModeChangeReserve;         //  1 bytes  �J�[�\�����샂�[�h�ύX�\��t���O
	u8 ReserveMode;               //  1 bytes  �\��J�[�\�����샂�[�h
	u8 ReserveSel;                //  1 bytes  �\��J�[�\���I��ԍ�
	s32 CursorAx,CursorAy,CursorPx,CursorPy;  // 16 bytes  �J�[�\���ʒu�����x
	u8 FrameMenuNo;               //  1 bytes  �t���[����ł̈ʒu
	u8 CursorMoving;              //  1 bytes  �J�[�\���ړ����t���O
	u8 CursorNextNo;              //  1 bytes  �J�[�\���ړ���i���o�[
	u8 CursorNextMode;            //  1 bytes  �J�[�\���ړ���̃��[�h

	u8 *BoxNameCGXAddr;           //  4 bytes �{�b�N�X���A�N�^�[�̂b�f�w�A�h���X
	u8 *NextBoxNameCGXAddr;       //  4 bytes �X�N���[����{�b�N�X���A�N�^�[�̂b�f�w�A�h���X
	u16 BoxNamePalNo;             //  2 bytes �{�b�N�X���A�N�^�[�p�p���b�g�i���o�[
	u16 NextBoxNamePalNo;         //  2 bytes �X�N���[����{�b�N�X���A�N�^�[�p���b�g�i���o�[
	u32 BoxNamePalMask;           //  4 bytes �{�b�N�X���p���b�g�A�j���p�}�X�N
	u32 NextBoxNamePalMask;       //  4 bytes �X�N���[����{�b�N�X���p���b�g�A�j���p�}�X�N

	// total = 67 bytes

// boxsel.c ------------------------------------------------------------
	BOXSEL_WORK  SelWork;
#if 0
	actWork *SelActor;            //  4 bytes  �I���{�b�N�X�{�̃A�N�^�[
	actWork *SelSideActor[4];     // 16 bytes  �I���{�b�N�X�ӂ��A�N�^�[
	actWork *SelTabActor[3];      // 12 bytes  �I���{�b�N�X�^�u�A�N�^�[
	u8 SelBoxNo;                  //  1 bytes  ���݂̑I���{�b�N�X�i���o�[
	s8 BoxPmCnt;                  //  1 bytes  �I���{�b�N�X�ɂ���|�P������
#endif
	// total = 34 bytes

// boxicon.c -----------------------------------------------------------
	actWork *MineIcon[6];          //  24 bytes  �莝���A�C�R���A�N�^�[
	actWork *BoxIcon[BOX_PM_KAZU]; // 120 bytes  �{�b�N�X�A�C�R���A�N�^�[
	u8 CellID_Cnt[410];            // 410 bytes  ����A�C�R���o�^�J�E���^

	// �A�C�R�����{�b�N�X�摜�X�N���[������
	s8 ScrollSpeed;       // 1 bytes  �X�N���[������
	u8 ScrollTargetBox;   // 1 bytes  �X�N���[����{�b�N�X�i���o�[
	u8 ScrollIconLines;   // 1 bytes  �X�N���[�����A�C�R���������C����
	u8 ScrollWait;        // 1 bytes  �X�N���[�����E�F�C�g�^�C�}�[
	u8 ScrollEnd;         // 1 bytes  �X�N���[���I���t���O
	u16 ScrollCnt;        // 2 bytes  �X�N���[���h�b�g���J�E���^

	// total = 561 bytes

	// �X�e�[�^�X�|�P�����摜�p�̈�i�\���̏I�[�ɒu�����Ɓ������A�h���X����J�n���邱�Ɓj
	u16 dmy;
	u8 PokeCellWork[0x800]; // �|�P�����Z���摜�W�J�p�̈�
	u8 PokeCell[0x800*7];     // �|�P�����Z���摜�p�̈�

}BOX_WORK;

extern BOX_WORK* const BoxWork;

//----------------------------------------------------------
//	�萔
//----------------------------------------------------------
#define NON_POKE          (0)   // �����|�P�����i���o�[
#define BOX_SCROLL_SPEED  (4)   // �{�b�N�X�X�N���[�����x
#define BOX_SCROLL_WIDTH  (168)  // �{�b�N�X�X�N���[����

// �Z���h�c���p���b�g�h�c
enum BOX_CELL_ID {
	CELLID_STATUS_POKE = 0,
	CELLID_BOXCURSOR,
	CELLID_BUTTON,
	CELLID_CLOSEBUTTON,
	CELLID_WAVE,
	CELLID_SHADOW,
	CELLID_BOXSEL,
	CELLID_BOXSELSIDE,
	CELLID_BOXSELTAB,
	CELLID_BOXNAME,
	CELLID_NEXTBOXNAME,
	CELLID_SCANLINE,

	CELLID_ICON_MIN,  // �A�C�R���Ɏg�p����Z���i���o�[�̍Œ�l
};
enum BOX_PAL_ID {
	PALID_BOXCURSOR =  (PALID_POKEICON_5 + 1),
	PALID_BOXSEL,
	PALID_BOXNAME,
	PALID_NEXTBOXNAME,
	PALID_SCANLINE,
};
// �J�[�\�����샂�[�h
enum CURSOR_MODE {
	CURSORMODE_MINE,
	CURSORMODE_BOX,
	CURSORMODE_FRAME,
	CURSORMODE_BOXSEL,
};
// �J�[�\���A�N�V�����p�^�[��
enum CURSOR_ACTION {
	CURSORACTION_CATCH,
	CURSORACTION_RELEASE,
	CURSORACTION_SWAP,
};
// �{�^���A�N�V�����p�^�[��
enum BUTTON_ACTION {
	BUTTONACTION_WAIT,
	BUTTONACTION_NORMAL,
	BUTTONACTION_SCROLL_L,
	BUTTONACTION_SCROLL_R,
};

/*===============================================*/
/* �r�d                                          */
/*===============================================*/
#define BOXSE_CURSOR_MOVE  (SE_SELECT)
#define BOXSE_WIN_OPEN     (SE_WIN_OPEN)
#define BOXSE_PC_ON        (SE_PC_LOGIN)
#define BOXSE_PC_OFF       (SE_PC_OFF)
#define BOXSE_TRAY_CHANGE  (SE_SELECT)
#define BOXSE_WARN         (SE_WALL_HIT)
//#define BOXSE_CHATCH       (
//#define BOXSE_RELEASE      (
//#define BOXSE_SWAP         (

/*===============================================*/
/* �v���C�I���e�B                                */
/*===============================================*/
#define  STATUS_POKE_PRI    (0)
#define  STATUS_POKE_ACTPRI (0)

#define  BOXSEL_PRI            (1)
#define  BOXSEL_ACTPRI         (3)
#define  BOXSEL_TAB_ACTPRI     (BOXSEL_ACTPRI+1)
#define  BOXSEL_SIDETAB_ACTPRI (BOXSEL_TAB_ACTPRI+1)

#define  CURSOR_PRI         (1)
#define  CURSOR_ACTPRI      (1)
#define  CATCHICON_PRI      (1)
#define  CATCHICON_ACTPRI   (2)

#define  MINEICON_PRI       (1)
#define  MINEICON_ACTPRI    (BOXSEL_SIDETAB_ACTPRI+1)
#define  MINESHADOW_ACTPRI  (MINEICON_ACTPRI+1)
#define  BOXICON_PRI        (2)
#define  BOXICON_ACTPRI     (MINESHADOW_ACTPRI+1)
#define  BOXSHADOW_ACTPRI   (BOXICON_ACTPRI+YOKO_KAZU+1)

#define  BOX_CHANGE_BUTTON_PRI     (2)
#define  BOX_CHANGE_BUTTON_ACTPRI  (BOXSHADOW_ACTPRI+1)

#define  CLOSEBUTTON_PRI       (3)
#define  CLOSEBUTTON_ACTPRI    (BOX_CHANGE_BUTTON_ACTPRI+1)

#define  BOXNAME_PRI     (2)
#define  BOXNAME_ACTPRI  (CLOSEBUTTON_ACTPRI)

/*===============================================*/
/* ���C�A�E�g                                    */
/*===============================================*/
// �A�C�R��
#define  ICON_HEIGHT (24)
#define  ICON_WIDTH  (24)

#define  MINE_ICON_XPOS      (148)   // �莝���|�P�����̂P�ԃA�C�R����\������w���W
#define  MINE_ICON_YPOS      (26)    // �莝���|�P�����̂P�ԃA�C�R����\������x���W
#define  MINE_TOP_ICON_XPOS  (102)   // �莝���|�P�����̂O�ԃA�C�R����\������w���W
#define  MINE_TOP_ICON_YPOS  (MINE_ICON_YPOS + 24*2)   // �莝���|�P�����̂O�ԃA�C�R����\������x���W
#define  BOX_ICON_XPOS       (100)   // �{�b�N�X�̐擪�i����j�A�C�R���w���W
#define  BOX_ICON_YPOS       (44)    // �{�b�N�X�̐擪�i����j�A�C�R���x���W

// ���j���[�E�B���h�E
#define  BOX_MENU_X1      (20)   // �E�B���h�E�쐻���� x1 �l
#define  BOX_MENU_WIDTH   (6)    // ���j���[�̍ő啶����
#define  BOX_MENU_X2      (BOX_MENU_X1+BOX_MENU_WIDTH+3) // �E�B���h�E�쐻���� x2 �l
#define  BOX_MENU_Y2      (15)   // ���j���[�E�B���h�E�쐻���� y2 �l

// ���b�Z�[�W�E�B���h�E
#define  BOX_MSGWND_X1    (10)
#define  BOX_MSGWND_X2    (29)
#define  BOX_MSGWND_Y1    (16)
#define  BOX_MSGWND_Y2    (19)
#define  BOX_MSG_X        (BOX_MSGWND_X1+1)
#define  BOX_MSG_Y        (BOX_MSGWND_Y1+1)

// �t���[���I�u�W�F
#define  BOX_FRAMEOBJ_XPOS   (92)   // ���֐؂�ւ��{�^���w���W
#define  BOX_FRAMEOBJ_YPOS   (26)   // ���֐؂�ւ��{�^���x���W
#define  FRAMEOBJ_INTERVAL   (70)


// �X�e�[�^�X�E�B���h�E
#define STATUS_POKE_X   (40) // ���̓��{�|�P�\������X���W
#define STATUS_POKE_Y   (48) //���̓��{�|�P�\������Y���W
#define BOX_WAVE_XPOS   (8)
#define BOX_WAVE_DIFFX  (64)
#define BOX_WAVE_YPOS   (8)

// �J�[�\��
#define CURSOR_ICON_DIFF_X        (-8)   // �J�[�\���ƃA�C�R���̂w���W��
#define CURSOR_ICON_DIFF_Y        (-24)  // �J�[�\���ƃA�C�R���̂x���W��
#define CURSOR_CATCH_DOWN_HEIGHT  (8)    // ���܂��鎞�Ɉړ����鍂��
#define CURSOR_SHADOW_DIFF_X      (8)    // �J�[�\���Ɖe�̂w���W��
#define CURSOR_SHADOW_DIFF_Y      (32)   // �J�[�\���Ɖe�̂x���W��

// �Ƃ���{�^��
#define CLOSEBUTTON_Y      (8)
#define CLOSEBUTTON_CX     (200)
#define CLOSEBUTTON_X      (CLOSEBUTTON_CX-24)

// �{�b�N�X��
#define  BOXNAMELABEL_X    (136)
#define  BOXNAMELABEL_Y    (28)

#endif /* #ifndef _BOX_H_ */
