/***********************************************************************
**	�z�E�G���n���S�̃}�b�v�i���z�}�A����ƂԁA�g���i�r���j
**
**	masao taya
**
***********************************************************************/
#ifndef _TOWNMAP_H_
#define _TOWNMAP_H_

#include "MaDefine.h"
#include "mappos.h"

/*============================================================*/
/*   �萔                                                     */
/*============================================================*/

/* �\�����[�h */
enum TOWNMAP_MODE {
	TOWNMAP_WHOLE,  // �S�̕\��
	TOWNMAP_ZOOM,   // �g��\��
};

/* �J�[�\�����w���Ă���ꏊ */
enum TOWNMAP_CURSOR {
	MAPCURSOR_SPACE,    // �󔒁i���������ꏊ�j
	MAPCURSOR_ROAD,     // ���[�h
	MAPCURSOR_ARRIVE,   // �K�ꂽ���Ƃ̂���X
	MAPCURSOR_NOARRIVE, // �K�ꂽ���Ƃ̂Ȃ��X
	MAPCURSOR_FLYING,   // �K�ꂽ���Ƃ̂���X�ȊO�̏ꏊ�i�Ƃ�ł�����j
};

/* �L�[����̖߂�l */
enum TOWNMAP_KEY {
	MAPKEY_FREE,    // ����������Ă��Ȃ�
	MAPKEY_MOVE,    // �J�[�\���ړ��J�n
	MAPKEY_WAIT,    // �J�[�\���ړ��҂�
	MAPKEY_CHANGE,  // �J�[�\���ړ��I��
	MAPKEY_DECIDE,  // �`�{�^��
	MAPKEY_CANCEL,  // �a�{�^��
};

/* �n�於�̕����� */
#define MAPNAME_MAX     (18)   // �����񒷂̍ő�
#define MAPNAME_WIDTH   (10)   // �\���L�������ő�


#define CURSOR_CELL_SIZE   (OBJ_SIZE16_16x16 * 2)
#define ZCURSOR_CELL_SIZE  (OBJ_SIZE16_32x32 * 3)

/* �}�b�v�̃u���b�N���i�J�[�\���T�C�Y = �P�u���b�N�j */
#define BLOCK_XMIN    (1)
#define BLOCK_XMAX    (28)
#define BLOCK_YMIN    (2)
#define BLOCK_YMAX    (16)
#define BLOCK_WIDTH   (BLOCK_XMAX-BLOCK_XMIN+1)
#define BLOCK_HEIGHT  (BLOCK_YMAX-BLOCK_YMIN+1)


/*============================================================*/
/*   ���[�N�̈�                                               */
/*============================================================*/

typedef struct {
	// --- �O���ŎQ�Ƃ���
	u8   mapname[MAPNAME_MAX+1]; // �J�[�\�����w���Ă���ꏊ�̒n��
	u16  mapno;                  // �J�[�\�����w���Ă���ꏊ�̒n��ԍ�
	u8   cursor_state;           // �J�[�\�����w���Ă���ꏊ�̏�ԁienum TOWNMAP_CURSOR)
	u8   map_subno;              // �J�[�\���������Ă���ꏊ�̃T�u�i���o�[

	// ---- �����ł̂ݎg�p
	u8 (*keyfunc)(void);
	actWork * cursor_actor;
	actWork *jiki_actor;

	u32 bg2x, bg2y;
	s32 dx,dy,dmx,dmy;
	s32  map_px, map_py, map_ax, map_ay;
	s32  ratio, ratio_add;

	u16  cursor_x;
	u16  cursor_y;
	u16  cursor_cell_id;
	u16  cursor_pal_id;
	s16  map_ox;
	s16  map_oy;
	s16  map_end_x;
	s16  map_end_y;
	u16  zoom_x;
	u16  zoom_y;
	s16  map_aoy;
	s16  map_aox;
	u16  scr_cnt;
	u16  mc_cnt;
	u16  jiki_cell_id;
	u16  jiki_pal_id;
	u16  jiki_x;
	u16  jiki_y;

	u8   mode;
	u8   init_prog;
	s8   cursor_move_cnt, cursor_ax, cursor_ay;

	u8 bgscr_req;
	u8 jiki_tenmetsu_flg;
	u8 jiki_tenmetsu_ok;

	u8  buf[256];

	u8  cursor_cell[CURSOR_CELL_SIZE];
	u8  zcursor_cell[ZCURSOR_CELL_SIZE];

}TOWNMAP_WORK;

/*============================================================*/
/*   �v���g�^�C�v                                             */
/*============================================================*/
void TownMapInit(TOWNMAP_WORK *work, u8 mode);
u8 TownMapInitMain(void);
void TownMapInitBegin(TOWNMAP_WORK *work, u8 mode);

u8   TownMapKeyRead(void);
void TownMapAddCursor(u16 cell_id, u16 pal_id);
void TownMapAddJiki(u16 cell_id, u16 pal_id);
void TownMapDelCursor(void);
void TownMapQuit(void);
void TownMapModeChangeInit(void);
u8   TownMapModeChange(void);
void TownMapScroll(void);
u16 TownMap_BlockToWPos(u16 x, u16 y);
void TownMap_CursorAnmStart(void);
void TownMap_CursorAnmStop(void);
void TownMap_SetOfs(s16 xofs, s16 yofs);
void TownMap_JikiTenmetsu(void);
void TownMapGetMapPosition(u16 mapno, u16 *x, u16 *y, u16 *w, u16 *h);
u16  ChangeMarineToGroundMapNo(u16 mapno);

#endif
