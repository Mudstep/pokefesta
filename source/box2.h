/**********************************************************************/
/*  �p�\�R���{�b�N�X������                                            */
/**********************************************************************/
#ifndef _BOX_2_H_
#define _BOX_2_H_

#include "MaDefine.h"
#include "boxsel.h"
#include "boxef.h"
#include "maptrans.h"
#include "marking.h"
#include "pokeicon.h"
#include "pasocom.h"
#include "pokelist.h"
#include "poketool.h"
#include "box_mode.h"
#include "monsno.def"

/*====================================================================*/
/*  ��۰��ٕϐ�                                                       */
/*====================================================================*/
#ifdef PM_DEBUG
extern u8 BoxDebugMode;
#endif

/*====================================================================*/
/*  �}�N���E�萔                                                      */
/*====================================================================*/
#define MAP_ADDR(b)   (BG_VRAM + (b) * 0x800)
#define CHAR_ADDR(b)  (BG_VRAM + (b) * 0x4000)

#define BOX_MENU_MAX   (7)  // �P�x�ɕ\���ł��郁�j���[���ڐ�
#define NON_POKE       (0)

// ����ʂɐ��䂪�ڂ��Ă��鎞�̃|�P�����f�[�^�ꎞ�ۑ��̈�
extern PokemonParam  PokeParaTmp;
#define POKETMP_ADDR  ((PokemonParam*)&PokeParaTmp)

/*-- ���C�A�E�g --*/
#define ICON_WIDTH     (24)
#define ICON_HEIGHT    (24)
#define TRAYICON_XORG  (100)
#define TRAYICON_YORG  (44)
#define MINEICON_XORG  (152)
#define MINEICON_YORG  (16)
#define TOPICON_XPOS   (104)
#define TOPICON_YPOS   (MINEICON_YORG+ICON_WIDTH*2)

#define MINEBOX_SCROLL_SYNC   (20)
#define MINEBOX_MAP_WIDTH     (12)
#define MINEBOX_MAP_HEIGHT    (22)
#define MINEBOX_CURSOR_MAXPOS (TEMOTI_POKEMAX)

#define MINEBOX_MODORU_XPOS  (MINEICON_XORG)
#define MINEBOX_MODORU_YPOS  (144)

/*-- �g���C --*/
#define TRAY_BODY_CHARS    (20)
#define TRAY_MARGIN_CHARS  (4)
#define TRAY_XCHARS        (TRAY_BODY_CHARS+TRAY_MARGIN_CHARS)  // 24
#define TRAY_SCROLL_WIDTH  (TRAY_XCHARS*8)    // 192
#define TRAY_SCROLL_SPEED  (6)  // TRAY_SCROLL_WIDTH �̖񐔂ɂ��邱�ƁI
#define TRAY_SCROLL_SYNC   (TRAY_SCROLL_WIDTH/TRAY_SCROLL_SPEED)
#define TRAY_NEXT_APPEAR_SYNC   ((TRAY_MARGIN_CHARS*8)/TRAY_SCROLL_SPEED)

/*-- �J�[�\�� --*/
#define CURSOR_ICON_DIFF    (12)  // �J�[�\���|�A�C�R�����W��
#define CURSOR_SHADOW_DIFF  (20)  // �J�[�\���|�e���W��
#define CURSOR_TUKAMU_DIPTH (8)   // �u���ށv���ɉ��h�b�g������H
#define CURSOR_CATCTHICON_DIFF  (CURSOR_ICON_DIFF-CURSOR_TUKAMU_DIPTH) // ���񂾃A�C�R���Ƃ̍��W��

/*-- �A�C�R���Z���}�l�[�W�� --*/
#define ICON_CELLMAN_MAX  (40)  // �A�C�R���Z���}�l�[�W���ŊǗ��ł���A�C�R����
#define CELLMAN_CHAR_ST  (ICON_CELLMAN_MAX*16)  // �]���̃Z���}�l�[�W���ŊǗ��ł���L�����i���o�J�n�l


/*-- �}�b�v�]�� --*/
#define MAP_TRANS_MAX     (8)

/*-- �Z���h�c --*/
enum CELL_ID {
	CELLID_CURSOR = 0,
	CELLID_SHADOW,
	CELLID_STATUS_POKE,
	CELLID_BOXNAME,
	CELLID_NEXTBOXNAME,
	CELLID_WAVE,
	CELLID_ARROW,

	CELLID_BOXSEL,   // �I���E�B���h�E�p�ɂR�K�v
	CELLID_BOXSEL2,
	CELLID_BOXSEL3,

	CELLID_MARKING,  // �}�[�L���O�E�B���h�E�p�ɂR�K�v
	CELLID_MARKING2,
	CELLID_MARKING3,
	CELLID_MARKING_STATE, // �}�[�L���O��ԕ\���p

	CELLID_SCANLINE,
	CELLID_ICON_MIN,  // �A�C�R���p�Z���h�c�̍ŏ��l
};
/*-- �p���b�g�h�c --*/
enum PAL_ID {
	PALID_CURSOR = (PALID_POKEICON_5+1),
	PALID_STATUS_POKE,
	PALID_BOXNAME,
	PALID_NEXTBOXNAME,
	PALID_BOXSEL,

	PALID_MARKING,   // �}�[�L���O�E�B���h�E�p�ɂQ�K�v
	PALID_MARKING2,

	PALID_OBJCOM,
	PALID_MARKING_STATE,
	PALID_ARROW,
	PALID_SCANLINE,
	PALID_QUICK_CURSOR,
};
/*-- �L�[���[�h���� --*/
enum CURSOR_ACT {
	ACT_NOTHING,      // ����Ȃ�
	ACT_CURSOR_MOVE,  // �ʏ�ړ�
	ACT_TRAY_CHANGE,  // �g���C�؂�ւ�
	ACT_WALLPAPER,    // �ǎ��ύX
	ACT_BOXOFF,       // �X�C�b�`�I�t�m�F
	ACT_MINEBOX_ON,   // �Ă����{�b�N�X�\��
	ACT_MINEBOX_OFF,  // �Ă����{�b�N�X�������g���C
	ACT_TRAYMENU,     // �g���C���j���[�\��
	ACT_POKEMENU,     // �|�P�������j���[�\��
	ACT_SCROLL_NEXT,  // �g���C�X�N���[������
	ACT_SCROLL_PREV,  // �g���C�X�N���[���O��
	ACT_DIRECT_AZUKERU,   // ���j���[������u��������v
	ACT_DIRECT_TURETEIKU, // ���j���[������u��Ă����v
	ACT_DIRECT_TUKAMU,    // ���j���[������u���ށv
	ACT_DIRECT_IREKAERU,  // ���j���[������u���ꂩ����v
	ACT_DIRECT_OKU,       // ���j���[������u�����v
	ACT_ISCONTINUE,   // �p�����܂����H�̊m�Fү����
};

/*-- BG��گ� --*/
#define STATUS_OFF_PAL    PA_BG0   // �ð������޳�i�P�x�޳�)
#define STATUS_PAL        PA_BG1   // �ð������޳
#define MINE_PAL          PA_BG2   // �莝���ޯ��
#define CLOSE_PAL         PA_BG3   // �Ƃ�������

#define TRAY_PAL_MAX     (3)     // �P���̕ǎ��Ŏg������گĖ{��
#define TRAY_PAL_NUM     (4)     // 
#define TRAY_PAL         PA_BG4  // ��������(TRAY_PAL_MAX*2)�����g�p

#define MALE_PAL          PA_BG11  // ���}�[�N
#define FEMALE_PAL        PA_BG12  // ���}�[�N

#define BG_PAL            PA_BG13  // �w�i
#define FONT_PAL          PA_BG15  // ̫��


/*-- �A�N�^�[�v���C�I���e�B --*/
enum ACTPRI {
	ACTPRI_STATUSPOKE,
	ACTPRI_MARKING,
	ACTPRI_WAVE,
	ACTPRI_BOXSEL,    // �I���E�B���h�E�p�ɂR�K�v
	ACTPRI_BOXSEL_2,  // 
	ACTPRI_BOXSEL_3,  //
	ACTPRI_CURSOR,
	ACTPRI_CATCHICON,
	ACTPRI_BOXSEL_WINDOW,
	ACTPRI_BOXSEL_CENTERTAB,
	ACTPRI_BOXSEL_SIDETAB,
	ACTPRI_MINEICON,
	ACTPRI_MINESHADOW,
	ACTPRI_TRAYICON,
	ACTPRI_TRAYSHADOW = ACTPRI_TRAYICON+YOKO_KAZU+1,
	ACTPRI_ARROW,
	ACTPRI_CLOSEBUTTON,
	ACTPRI_BOXNAME,
};

/*-- �J�[�\������ --*/
enum CURSOR_OP {
	CURSOR_OP_TUKAMU,
	CURSOR_OP_OKU,
	CURSOR_OP_IREKAERU,
	CURSOR_OP_AZUKERU,
	CURSOR_OP_NIGASU,
	CURSOR_OP_TURETEIKU,
};

/*-- �A�C�R���^�C�v --*/
enum ICONTYPE {
	ICONTYPE_MINE,
	ICONTYPE_TRAY,
	ICONTYPE_CATCH,
};

/*-- ��ʐ؂�ւ� --*/
enum CHANGEMAIN {
	CHANGEMAIN_YOUSU,  // �悤�����݂�
	CHANGEMAIN_NAMAE,  // �Ȃ܂�
};

/*-- ���j���[ --*/
enum MENU {
	MENU_YAMERU = 0,
	MENU_AZUKERU,
	MENU_TURETEIKU,
	MENU_TUKAMU,
	MENU_IREKAERU,
	MENU_OKU,
	MENU_YOUSUMIRU,
	MENU_NIGASU,
	MENU_MARKING,
	MENU_BOXCHANGE,
	MENU_WALLPAPER,
	MENU_BOXNAME,
	MENU_WPTHEME0,
	MENU_WPTHEME1,
	MENU_WPTHEME2,
	MENU_WPTHEME3,
	MENU_WPTYPE0_0,  // ���ǎ����j���[�͘A�����Ēu�����ƁI
	MENU_WPTYPE0_1,
	MENU_WPTYPE0_2,
	MENU_WPTYPE0_3,
	MENU_WPTYPE1_0,
	MENU_WPTYPE1_1,
	MENU_WPTYPE1_2,
	MENU_WPTYPE1_3,
	MENU_WPTYPE2_0,
	MENU_WPTYPE2_1,
	MENU_WPTYPE2_2,
	MENU_WPTYPE2_3,
	MENU_WPTYPE3_0,
	MENU_WPTYPE3_1,
	MENU_WPTYPE3_2,
	MENU_WPTYPE3_3,
						  // ------------------------------------

	MENU_DEBUG_POKESET,  // �f�o�b�O�p
};

/*====================================================================*/
/*  �\���̒�`                                                        */
/*====================================================================*/
typedef struct {
	u16 monsno;
	s16 x;
	s16 y;
	u16 pokeno;
	s16 w1;
	s16 w2;
	u8 oampri;
	u8 actpri;
}SCROLLICON_ADDPARAM;

#define SCROLLICON_ADD_MAX  (TATE_KAZU)

/*====================================================================*/
/*  ���[�N�̈�                                                        */
/*====================================================================*/
typedef struct {
	// �T�u�v���Z�X
	pFunc  sub_proc;
	u8     prog;
	u8     mode;  // ���샂�[�h�i��������E��Ă����E�����肷��j
	u8     changemain_mode;  // ��ʐ؂�ւ����[�h�i�悤�����݂�E�Ȃ܂��j


	// �w�i�X�N���[��
	u8  bg_scr_cnt;
	u16 background_v;
	u16 background_h;

	// �X�C�b�`�I���E�I�t�G�t�F�N�g
	SCAN_WORK  scanwk;

	// �Ă���
	MAP_TRANS_WORK  mtwk;
	MAP_TRANS_PARAM mtparam[MAP_TRANS_MAX];
	u16 mine_map[MINEBOX_MAP_WIDTH*MINEBOX_MAP_HEIGHT];
	u16 mine_sy;
	u16 mine_height;
	s8  mine_h_add;
	u8  mine_on_cnt;
	u8  gotomine_prog;

	// �Ƃ���{�^��
	u8  close_anm_on;
	u8  close_anm_timer;
	u8  close_anm_state;

	// �g���C
	s16 scr_boxno;
	u16 tray_h;
	s16 tray_scroll_speed;
	u16 tray_scroll_cnt;
	u8  tray_trans_pos;

	u8  tray_iconadd_box;
	s16 tray_iconadd_line;
	s16 tray_iconadd_lineadd;
	s16 tray_iconadd_xorg;
	s16 tray_iconadd_x;
	s16 tray_iconadd_xadd;
	s16 tray_iconadd_y;
	s16 tray_iconadd_timer;

	u8  fontobj_cell[0x200];  // 32x16 obj x 2
	u8  fontobj_work[0x200];
	u8  boxname_toggle;
	u16 boxname_pal[16];
	u16 boxname_palno, nextboxname_palno;
	actWork *boxname_actor[2];
	actWork *nextboxname_actor[2];
	actWork *arrow_actor[2];
	u32 tray_fade_mask;
	SCROLLICON_ADDPARAM  scricon_addparam[SCROLLICON_ADD_MAX];
	u16                  scricon_req_cnt;

	// �ǎ��ύX
	s16  wp_theme, wp_type;
	u16  wpbg_work[20*18];
	u8   wp_prog;

	// �A�C�R������
	actWork *catch_icon;
	actWork *mine_icon[TEMOTI_POKEMAX];
	actWork *tray_icon[BOX_PM_KAZU];
	actWork **swap_icon;
	actWork **nigasu_icon;
	u16  icon_cell_cnt[ICON_CELLMAN_MAX];
	u16  icon_cell_use[ICON_CELLMAN_MAX];
	u8   icon_swap_prog;
	u8   tsume_move_icons;

	u16  icon_scroll_timer;
	s16  icon_scroll_xpos;
	s16  icon_scroll_speed;
	u16  icon_scroll_actcnt;
	u8   icon_scroll_line;
	s8   icon_scroll_vec;
	u8   icon_scroll_seq;
	u8   icon_scroll_box;


	// ���j���[
	MENU_ITEM  menu[BOX_MENU_MAX];
	u8         menu_cnt;
	u8         menu_maxlen;
	u16        menu_x1, menu_y1;

	// �J�[�\��
	actWork *cursor_actor;
	actWork *shadow_actor;

	s32  cursor_x, cursor_y, cursor_ax, cursor_ay;
	s16  cursor_end_x, cursor_end_y;
	u16  cursor_move_sync;
	s8   cursor_vloop;
	s8   cursor_hloop;
	s8   cursor_nextmode;
	s8   cursor_nextpos;
	u8   mine_pos_save;
	u8   cursor_vflip;
	u8   cursor_palno[2];

	void* status_pal_addr;
	u32   status_rnd;
	u16   status_monsno;
	u16   status_itemno;
	u16   status_hp;
	u8    point_poke_update;
	u8    status_mark;
	u8    status_lv;
	u8    status_tamago_flg;
	u8    status_pokename[MONS_NAME_SIZE+EOM_SIZE];
	u8    status_str[3][36];

	u8 (* cursor_op)(void);
	u8  cursor_op_prog;
	u8  swap_poke_box;

	// �}�[�L���O
	actWork *marking_actor;
	actWork *wave_actor[2];
	u8      *marking_char_addr;
	MARKING_WORK  MarkWork;

	// �{�b�N�X�I���E�B���h�E
	BOXSEL_WORK SelWork;

	// ����ł���|�P�����̃f�[�^
	PokemonParam     PP;
	PokemonParam     PPtmp;  // ���ꂩ���p

	// ��������`�F�b�N
	u8  nigaseru_chk_result;
	u8  nigaseru_chk_end;
	u8  nigaseru_naminori_chk;
	u8  nigaseru_diving_chk;
	s8  nigaseru_chk_box;
	s8  nigaseru_chk_pos;
	s8  nigaseru_target_box;
	s8  nigaseru_target_pos;
	u16 nigaseru_chk_prog;
	u16 nigaseru_chk_wazano[3];

	// �悤�����݂�p�����[�^
	u8  yousu_array_max;
	u8  yousu_array_pos;
	u8  yousu_type;
	void *yousu_array_addr;

	// �����񏈗�
	u8 msg_buf[40];
	u8 boxname_buf[40];
	u8 nigashita_pokename[MONS_NAME_SIZE+EOM_SIZE];

	// �f�o�b�O�p�|�P�����Z�b�g���[�h�t���O
	u8  DebugPokeSetMode;

	// �|�P�����摜
	u16  status_poke_palno;
	u32  status_poke_celladrs;
	actWork *status_poke_actor;
	u16 PokePal[16*4];      // �|�����p�ɂS�̕�
	u8  PokeCell[0x2000];   // �|�����p�ɂS�̕�
	u8  PokeCellWork[0x800];

}BOX2WORK;

extern BOX2WORK * const Work;

/*====================================================================*/
/*  �v���g�^�C�v                                                      */
/*====================================================================*/
/*-- box2.c --*/
u8 GetCurrentBox(void);

/*-- box2_tray.c --*/
void InitBoxTray(u8 current_box);
void BoxTray_InitScroll(u8 box_no);
u8 BoxTray_WaitScroll(void);
void BoxTray_WallPaperChangeInit(u8 wp_no);
u8 BoxTray_WallPaperChangeWait(void);
void BoxArrowMove(u8 on);

/*-- box2_icon.c --*/
void InitBox2Icon(void);
void AddTrayIcon(u8 box);
void AddMineIcon(u8 disp);
void DelMineIcon(u8 pos);
void DelMineIconAll(void);
void ScrollMineIcon(s16 vector);
void TrayIconScrollOut(s16 vector);
void TrayIconScrollIn(u8 box, u8 line, s16 xpos, s16 speed);
void TrayIconScrollIn2(u8 box, u8 x, u8 y, u8 wait, s16 speed);
void BoxIconCatch(u8 icon_type, u8 pos);
void BoxIconPut(u8 box, u8 pos);
void BoxIconSwapInit(u8 box, u8 pos);
u8 BoxIconSwapWait(void);
void BoxIcon_TrayUpdate(u8 pos);
void BoxIcon_DelCatch(void);
void BoxIcon_NigasuInit(u8 icon_type, u8 pos);
u8 BoxIcon_NigasuWait(void);
void BoxIcon_NigasuFinish(void);
void BoxIcon_ModoruInit(void);
u8 BoxIcon_ModoruWait(void);

void RecoverBoxCatchIcon(void);
void BoxIconCellTrans(void);
void BoxIcon_TumeInit(void);
u8 BoxIcon_TumeWait(void);

void TrayIconScrollInit(u8 box, s8 vec);
u8  TrayIconScrollWait(void);



/*-- box2_cursor.c --*/
void InitBoxCursor(void);
void RecoverBoxCursor(void);
u8 BoxCursorKeyRead(void);
u8 BoxCursorMoveWait(void);
void BoxCursorMoveToMine(void);
void BoxCursorMoveToTray(u8 pos);
void BoxDispMenu(void);
s16 BoxSelectMenu(void);
void BoxCursorOP_Init(u8 op);
u8 BoxCursorOP_Exe(void);
u8 BoxAzukeruInit(u8 box);
void BoxAzukeruFinish(void);
s16 BoxTumeMineData(void);
void BoxNigasuAnmInit(void);
u8 BoxNigasuAnmWait(void);
void BoxNigasuFinish(void);
void BoxSetTmpPokeData(void);
void BoxGetTmpPokeData(void);
void BoxSetMarkingParam(u8 mark);
u8 CheckCatchIcon(void);
u8 CheckLastOneMine(void);
u8 CheckIrekaeOK(void);
u8 CheckArrowMoveState(void);
u8 CheckBoxCursorOnCloseButton(void);
void TrayReturnPosInit(void);
void TrayReturnPosSave(void);
u8 TrayReturnPosLoad(void);
actWork* MakeBoxArrowActor(u16 x, u16 y, u8 vector, u8 oampri, u8 actpri);
void BoxUpdateStatusParam(void);
void BoxNigaseruCheckInit(void);
s8  BoxNigaseruCheck(void);
void BoxModoruFinish(void);
void BoxYousuParamSet(void);
void BoxYousuParamRecover(void);

/*-- box2_menu.c --*/
void BoxMenuInit(void);
void BoxMenuAdd(u8 menu_type);
void BoxMenuDisp(void);
s16 BoxMenuSelect(void);
s8 BoxMenuCheck(u8 pos);

#endif
