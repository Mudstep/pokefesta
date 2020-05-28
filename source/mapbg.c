//=============================================================================
//
//	mapbg.c
//		2001.02.15 by tama
//
//	BG ����֘A
//=============================================================================

#include "common.h"

#define MAPBG_H_GLOBAL
#include "mapbg.h"

#include "screen.h"
#include "field.h"			/* ���\�[�X��` */
#include "fieldmap.h"		/* �Z���Q�� */
#include "evobj.h"
#include "jiki.h"
#include "seed.h"			//CheckSeedInScreen
#include "r_door.h"			//DoorObjControl

#include "map_attr.h"


//-----------------------------------------------------------------------------
//	�^��`�E�萔��`
//-----------------------------------------------------------------------------

enum {
		CELLCHRSIZE			=	2,				//�Z���̂P�ӂ��\������̂ɕK�v�ȃL������
		BG_X_SIZE			=	32,				//�a�f�w�T�C�Y�i�L�����P�ʁj
		BG_Y_SIZE			=	32,				//�a�f�x�T�C�Y�i�L�����P�ʁj

		BGPUTX_DEFAULT		=	0,				//�����\���w�ʒu�i�L�����P�ʁj
		BGPUTY_DEFAULT		=	0,				//�����\���x�ʒu�i�L�����P�ʁj

		/* ��ʒ[�݂̂�`�悷��ꍇ�̏C���l */
		UP_EDGE_OFFSET		=	(0),
		DOWN_EDGE_OFFSET	=	(BG_Y_SIZE - 2 - 2),
		LEFT_EDGE_OFFSET	=	(0),
		RIGHT_EDGE_OFFSET	=	(BG_X_SIZE - 2 - 2),
};

/* �a�f����\���̒�` */
typedef struct {
		u8	dotx;							// BG�\���w�ʒu(�h�b�g�P�ʁj
		u8	doty;							// BG�\���x�ʒu(�h�b�g�P�ʁj
		u8	chrx;							// BG�`��w�ʒu(�L�����P��)
		u8	chry;							// BG�`��x�ʒu(�L�����P��)
		u8	trans_req;						// BG�]�����N�G�X�g
}BGstruct;


//-----------------------------------------------------------------------------
//	�ϐ���`
//-----------------------------------------------------------------------------
static BGstruct MainMapBG;
FieldScrollCtrl scroll;
s16 FieldActOX;
s16 FieldActOY;

#define MapLowBG	(dmy_BG[ MAP_LOW_BG_NO ])
#define MapUpBG		(dmy_BG[ MAP_UP_BG_NO ])
#define MapBtmBG	(dmy_BG[ MAP_BTM_BG_NO])

//-------------------------------------------------------------
//-------------------------------------------------------------

static void MakeDmyScreenDirect(int map_x, int map_y, const MapScreenData * scrn);

static void BGwriteCellSub( const MapScreenData * scrn, u16 offset, int x, int y );


//=============================================================================
//
//	���E����֘A��`
//
//=============================================================================
enum {
		DOT_OFF_Y		= 8,
		BASE_OFF_X		= 0,
		BASE_OFF_Y		= (CELL_SIZE * VIEW_UP_MARGIN),

		CHANGE_SPEED	= 2,						/* ���E�ύX�̑��x(dot�P��) */
		MOVE_LENGTH		= (CELL_SIZE * 2 + 8),		/* ���E�ړ��͈̔� */
};


u8 MapViewControlFlag = 0;
static s16 offset_x, offset_y;
static u8 TestOffsetFlag;



//=============================================================================
//
//	BG�\���̊֘A���[�`��
//
//=============================================================================

//-----------------------------------------------------------------------------
//	����������
//-----------------------------------------------------------------------------
static void BGinit(BGstruct * bg)
{
	bg->chrx = BGPUTX_DEFAULT;							// �\���ʒu( ��גP�� )
	bg->chry = BGPUTY_DEFAULT;
	bg->dotx = BGPUTX_DEFAULT*8;						// �\���ʒu( �ޯĒP�� )
	bg->doty = BGPUTY_DEFAULT*8;
	bg->trans_req = TRUE;
}

//-----------------------------------------------------------------------------
//	�`��ʒu�̕ύX
//-----------------------------------------------------------------------------
static void BGmoveChrPosition(BGstruct * bg, int dx, int dy)
{
		bg->chrx += dx;
		bg->chrx %= BG_X_SIZE;

		bg->chry += dy;
		bg->chry %= BG_Y_SIZE;
}

static void BGmoveDotPos(BGstruct * bg, int dx, int dy)
{
	bg->dotx += dx;
	bg->doty += dy;
}


//=============================================================================
//
//	�t�B�[���h�}�b�v�a�f������
//
//=============================================================================
void InitMapBG(void)
{
	BGinit(&MainMapBG);
	CpuClear(0, MapLowBG, sizeof(MapLowBG), 16);	// ���z BG RAM �ر
	CpuClear(0, MapUpBG, sizeof(MapUpBG), 16);	// ���z BG RAM �ر
	CpuClear(WATER_CHR_CODE, MapBtmBG, sizeof(MapBtmBG), 16);
}

//-----------------------------------------------------------------------------
//	Vsync����BG�f�[�^����
//-----------------------------------------------------------------------------
void MapBGVsync(void)
{
		*(RegBGHofs[MAP_UP_BG_NO]) = MainMapBG.dotx + offset_x;
		*(RegBGVofs[MAP_UP_BG_NO]) = MainMapBG.doty + offset_y + DOT_OFF_Y;

		*(RegBGHofs[MAP_LOW_BG_NO]) = MainMapBG.dotx + offset_x;
		*(RegBGVofs[MAP_LOW_BG_NO]) = MainMapBG.doty + offset_y + DOT_OFF_Y; 

		*(RegBGHofs[MAP_BTM_BG_NO]) = MainMapBG.dotx + offset_x; 
		*(RegBGVofs[MAP_BTM_BG_NO]) = MainMapBG.doty + offset_y + DOT_OFF_Y; 

		if( MainMapBG.trans_req == FALSE )return;

		DmaArrayCopy(3 , MapUpBG , BASE_SCRN_ADRS(MAP_UP_SCRN_BASE),16);
		DmaArrayCopy(3 , MapLowBG , BASE_SCRN_ADRS(MAP_LOW_SCRN_BASE),16);
		DmaArrayCopy(3 , MapBtmBG , BASE_SCRN_ADRS(MAP_BTM_SCRN_BASE),16);

		MainMapBG.trans_req = FALSE;
}

//-----------------------------------------------------------------------------
//	ڼ޽��־�Ă���l���擾
//-----------------------------------------------------------------------------
void GetMapRegPos( s16 *x, s16 *y )
{
	*x = MainMapBG.dotx + offset_x;
	*y = MainMapBG.doty + offset_y + DOT_OFF_Y;
}

//=============================================================================
//
//	�S�a�f���̕`�揈��
//
//=============================================================================


void MakeMapBG(void)
{
	MakeDmyScreenDirect(Fld.MapXpos, Fld.MapYpos, NowMap.Screen);
	MainMapBG.trans_req = TRUE;
}

//-----------------------------------------------------------------------------
// ��ʃT�C�Y���̃}�b�v�f�[�^�W�J
//-----------------------------------------------------------------------------
static void MakeDmyScreenDirect(int map_x, int map_y, const MapScreenData * scrn )
{
	u16 pos;
	u8	bg_x, bg_y;
	u8	dx, dy;

	for (dy = 0; dy < BG_Y_SIZE ; dy += CELLCHRSIZE) {
		bg_y = MainMapBG.chry + dy;
		if (bg_y >= BG_Y_SIZE) bg_y -= BG_Y_SIZE;
		pos = BG_X_SIZE * bg_y;

		for (dx = 0; dx < BG_X_SIZE; dx += CELLCHRSIZE) {
			bg_x = MainMapBG.chrx + dx;
			if (bg_x >= BG_X_SIZE) bg_x -= BG_X_SIZE;

			BGwriteCellSub(scrn, pos + bg_x,
					map_x + dx/CELLCHRSIZE, map_y + dy/CELLCHRSIZE);
		}
	}
}


//=============================================================================
//
//	�X�N���[�������̉�ʒ[�݂̂̕`�揈��
//
//=============================================================================


static void	BGwriteRightEdgeBG(BGstruct * bg, const MapScreenData * scrn);
static void BGwriteLeftEdgeBG(BGstruct * bg, const MapScreenData * scrn);
static void BGwriteDownEdgeBG(BGstruct * bg, const MapScreenData * scrn);
static void BGwriteUpEdgeBG(BGstruct * bg, const MapScreenData * scrn);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static void MakeMapEdgeBG(BGstruct * bg, int mx, int my)
{
	const MapScreenData * scrn = NowMap.Screen;

	if (mx > 0) BGwriteRightEdgeBG(bg, scrn);
	if (mx < 0) BGwriteLeftEdgeBG(bg, scrn);
	if (my > 0) BGwriteDownEdgeBG(bg, scrn);
	if (my < 0) BGwriteUpEdgeBG(bg, scrn);
	bg->trans_req = TRUE;
}

//-----------------------------------------------------------------------------
//	
//-----------------------------------------------------------------------------
void BGwriteDownEdgeBG(BGstruct * bg, const MapScreenData * scrn)
{
	u8 bg_x, bg_y, cx;
	u16 pos;

	bg_y = bg->chry + DOWN_EDGE_OFFSET;
	if (bg_y >= BG_Y_SIZE) bg_y -= BG_Y_SIZE;
	pos = bg_y * BG_X_SIZE;

	for (cx = 0; cx < BG_X_SIZE;cx += CELLCHRSIZE) {
		bg_x = bg->chrx + cx;
		if (bg_x >= BG_X_SIZE) bg_x -= BG_X_SIZE;

		BGwriteCellSub(scrn, pos + bg_x,
				Fld.MapXpos + cx/CELLCHRSIZE,
				Fld.MapYpos + DOWN_EDGE_OFFSET/CELLCHRSIZE);
	}
}

//-----------------------------------------------------------------------------
//	
//-----------------------------------------------------------------------------
void BGwriteUpEdgeBG(BGstruct * bg, const MapScreenData * scrn)
{
	u8 bg_x, bg_y, cx;
	u16 pos;

	bg_y = bg->chry + UP_EDGE_OFFSET;
	pos = bg_y * BG_X_SIZE;

	for (cx = 0; cx < BG_X_SIZE; cx += CELLCHRSIZE) {
		bg_x = bg->chrx + cx;
		if (bg_x >= BG_X_SIZE) bg_x -= BG_X_SIZE;
		BGwriteCellSub(scrn, pos + bg_x,
				Fld.MapXpos + cx/CELLCHRSIZE,
				Fld.MapYpos + UP_EDGE_OFFSET/CELLCHRSIZE);
	}
}

//-----------------------------------------------------------------------------
//	
//-----------------------------------------------------------------------------
void BGwriteLeftEdgeBG(BGstruct * bg, const MapScreenData * scrn)
{
	u8 bg_x, bg_y, cy;
	u16 pos;

	bg_x = bg->chrx + LEFT_EDGE_OFFSET;

	for (cy = 0; cy < BG_Y_SIZE; cy += CELLCHRSIZE) {
		bg_y = bg->chry + cy;
		if (bg_y >= BG_Y_SIZE) bg_y -= BG_Y_SIZE;

		pos = bg_y * BG_X_SIZE + bg_x;
		BGwriteCellSub(scrn, pos,
				Fld.MapXpos + LEFT_EDGE_OFFSET/CELLCHRSIZE,
				Fld.MapYpos + cy/CELLCHRSIZE);
	}
}

//-----------------------------------------------------------------------------
//	
//-----------------------------------------------------------------------------
void BGwriteRightEdgeBG(BGstruct * bg, const MapScreenData * scrn)
{
	u8 bg_x, bg_y, cy;
	u16 pos;

	bg_x = bg->chrx + RIGHT_EDGE_OFFSET;
	if (bg_x >= BG_X_SIZE) bg_x -= BG_X_SIZE;

	for (cy = 0; cy < BG_Y_SIZE; cy += CELLCHRSIZE) {
		bg_y = bg->chry + cy;
		if(bg_y >= BG_Y_SIZE) bg_y -= BG_Y_SIZE;

		pos = bg_y * BG_X_SIZE + bg_x;
		BGwriteCellSub(scrn, pos,
				Fld.MapXpos + RIGHT_EDGE_OFFSET/CELLCHRSIZE,
				Fld.MapYpos + cy/CELLCHRSIZE);
	}
}

//=============================================================================
//	�Z���ЂƂ��̂a�f��`�悷��
//=============================================================================
static void write_dmy_bg( int trans_type, const u16 * cellchr, u16 dst );
static int get_write_pos_offset( BGstruct * bg, int x, int y );
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void RewriteSingleBG(int map_x, int map_y)
{
	int offset;

	offset = get_write_pos_offset(&MainMapBG,map_x,map_y);
	if (offset < 0) return;
	BGwriteCellSub( NowMap.Screen,  offset, map_x, map_y );
	MainMapBG.trans_req = TRUE;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void WriteMapBGdirect(int map_x, int map_y, const u16 * celldata)
{
	int offset;

	offset = get_write_pos_offset(&MainMapBG,map_x,map_y);
	if (offset < 0) return;
	write_dmy_bg( 1, celldata, offset );
	MainMapBG.trans_req = TRUE;
}

//-----------------------------------------------------------------------------
//	�w��Z���ʒu�̂a�f�`��
//	�����W�ȊO��offset�������ɂƂ�̂̓��[�v������Ăяo�����ꍇ��
//	�@����offset���v�Z����ƒx���Ȃ�̂ŌĂяo�����Ōv�Z���Ă���
//-----------------------------------------------------------------------------
static void BGwriteCellSub( const MapScreenData * scrn, u16 offset, int x, int y )
{
	u16 code;
	const u16 * celldata;

	code = GetCellCode(x,y);
	if (code > MAP_CELL_2ND_NO + MAP_CELL_2ND_SIZE) code = 0;	//�G���[�f�[�^����

	if (code < MAP_CELL_2ND_NO) {
		celldata = scrn->MainCharData->CellCharData;
	} else {
		celldata = scrn->SubCharData->CellCharData;
		code -= MAP_CELL_2ND_NO;
	}

	write_dmy_bg( GetCellLayer(x,y), &(celldata[code*8]), offset );
}


//-----------------------------------------------------------------------------
//	<����>
//		trans_type	��������BG�̎w��
//					0:�㒆��BG�ɕ`��	1:������BG�ɕ`��	2:�㉺��BG�ɕ`��
//		cellchr		�������ރf�[�^�̐擪�A�h���X
//		dst			BG�̏������݈ʒu�ւ̃I�t�Z�b�g
//-----------------------------------------------------------------------------
static void write_dmy_bg( int trans_type, const u16 * cellchr, u16 dst )
{
	switch (trans_type) {
	case 2:
		MapBtmBG[dst				]	= cellchr[0];
		MapBtmBG[dst+1				]	= cellchr[1];
		MapBtmBG[dst+BG_X_SIZE		]	= cellchr[2];
		MapBtmBG[dst+BG_X_SIZE+1	]	= cellchr[3];

		MapLowBG[dst				]	= 0;
		MapLowBG[dst+1				]	= 0;
		MapLowBG[dst+BG_X_SIZE		] 	= 0;
		MapLowBG[dst+BG_X_SIZE+1	]	= 0;

		MapUpBG[dst					]	= cellchr[4];
		MapUpBG[dst+1				]	= cellchr[5];
		MapUpBG[dst+BG_X_SIZE		] 	= cellchr[6];
		MapUpBG[dst+BG_X_SIZE+1		]	= cellchr[7];
		break;

	case 1:
		MapBtmBG[dst				]	= cellchr[0];
		MapBtmBG[dst+1				]	= cellchr[1];
		MapBtmBG[dst+BG_X_SIZE		]	= cellchr[2];
		MapBtmBG[dst+BG_X_SIZE+1	]	= cellchr[3];

		MapLowBG[dst				]	= cellchr[4];
		MapLowBG[dst+1				]	= cellchr[5];
		MapLowBG[dst+BG_X_SIZE		] 	= cellchr[6];
		MapLowBG[dst+BG_X_SIZE+1	]	= cellchr[7];

		MapUpBG[dst					]	= 0;
		MapUpBG[dst+1				]	= 0;
		MapUpBG[dst+BG_X_SIZE		]	= 0;
		MapUpBG[dst+BG_X_SIZE+1		]	= 0;
		break;

	case 0:
		MapBtmBG[dst				]	= WATER_CHR_CODE;
		MapBtmBG[dst+1				]	= WATER_CHR_CODE;
		MapBtmBG[dst+BG_X_SIZE		]	= WATER_CHR_CODE;
		MapBtmBG[dst+BG_X_SIZE+1	]	= WATER_CHR_CODE;

		MapLowBG[dst				]	= cellchr[0];
		MapLowBG[dst+1				]	= cellchr[1];
		MapLowBG[dst+BG_X_SIZE		]	= cellchr[2];
		MapLowBG[dst+BG_X_SIZE+1	]	= cellchr[3];

		MapUpBG[dst					]	= cellchr[4];
		MapUpBG[dst+1				]	= cellchr[5];
		MapUpBG[dst+BG_X_SIZE		] 	= cellchr[6];
		MapUpBG[dst+BG_X_SIZE+1		]	= cellchr[7];
		break;
	}
}

//-------------------------------------------------------------
//	���������ʒu�̃I�t�Z�b�g�v�Z
//	<�߂�l>	�I�t�Z�b�g
//				-1�̏ꍇ�͔͈͊O�G���[
//-------------------------------------------------------------
static int get_write_pos_offset( BGstruct * bg, int x, int y )
{
	int dx,dy;

	dx = (x - Fld.MapXpos) * CELLCHRSIZE;
	if (dx < 0 || dx >= BG_X_SIZE) return -1;
	dx += bg->chrx;
	if (dx >= BG_X_SIZE ) dx -= BG_X_SIZE;
	dy = (y - Fld.MapYpos) * CELLCHRSIZE;
	if (dy < 0 || dy >= BG_Y_SIZE) return -1;
	dy += bg->chry;
	if (dy >= BG_Y_SIZE) dy -= BG_Y_SIZE;
		 
	return BG_X_SIZE * dy + dx;
}



//=============================================================================
//
//	�X�N���[������֘A���[�`��
//
//=============================================================================
//-------------------------------------------------------------
//-------------------------------------------------------------
static void NormalTraceActor(FieldScrollCtrl * scrl)
{
	if(scrl->work == 0)
	{
		return;										// �����̾�Ă�����Ă��Ȃ�
	}

	scrl->dx = ActWork[scrl->work].work[2];			// ��۰ْl X
	scrl->dy = ActWork[scrl->work].work[3];			// Y
}

//-------------------------------------------------------------
//	SCROLL�R���g���[��������
//-------------------------------------------------------------
void InitFieldScroll(void)
{
	scroll.dx = 0;
	scroll.dy = 0;
	scroll.xdiff = 0;
	scroll.ydiff = 0;
	scroll.work = 0;
	scroll.controler = NULL;
}

//-------------------------------------------------------------
//	����A�N�^�[�����X�N���[���Z�b�g
//-------------------------------------------------------------
int SetFieldScroll(u8 actno)
{
	if (scroll.work != 0 )								// ��۰�ܰ�
	{
		DelActor(&ActWork[scroll.work]);				// ���ݎg�p���Ă��铯����������
	}

	scroll.work = SetScrollTaskAct( actno );			// work = ��������
	scroll.controler = NormalTraceActor;				// ��۰�����

	return 0;
}

//-------------------------------------------------------------
//	Scroll����	
//-------------------------------------------------------------
void ControlFieldScroll(void)
{
	int dx,dy;
	int x_chg,y_chg;
	int oldx,oldy;

	if(scroll.controler != NULL)
	{
		scroll.controler(&scroll);						// ���䃋�[�`���Ăяo��
	}
	
	dx = scroll.dx;										// ��۰ٍ��W
	dy = scroll.dy;

	x_chg = 0;
	y_chg = 0;

	oldx = scroll.xdiff;
	oldy = scroll.ydiff;

//�@xdiff/ydiff���O����ω������ꍇ�A���邢�͂O�ɕω������ꍇ�̂�
//	���� x_chg/y_chg �𔭐�������

	if(oldx == 0 && dx != 0)
	{
		x_chg = dx > 0 ? 1 : -1;
	}

	if(oldy == 0 && dy!=0)
	{
		y_chg = dy > 0 ? 1 : -1;
	}

	if(oldx != 0 && (oldx + dx) == 0)
	{
		x_chg = dx > 0 ? 1 : -1;
	}

	if(oldy != 0 && (oldy + dy) == 0)
	{
		x_chg = dy > 0 ? 1 : -1;
	}

	scroll.xdiff += dx;
	scroll.xdiff %= CELL_SIZE;
	scroll.ydiff += dy;
	scroll.ydiff %= CELL_SIZE;

	if( x_chg != 0 || y_chg != 0 )
	{
		MoveVMapPosition(x_chg,y_chg);
		EvObjMainControl(x_chg, y_chg);
		DoorObjControl( x_chg, y_chg );
		CheckSeedInScreen();							// ���݂̂̐����}�~�`�F�b�N
		
		BGmoveChrPosition(&MainMapBG, x_chg * CELLCHRSIZE, y_chg * CELLCHRSIZE);
		MakeMapEdgeBG(&MainMapBG, x_chg * CELLCHRSIZE, y_chg * CELLCHRSIZE);		// BG�`��
	}

	BGmoveDotPos(&MainMapBG,dx,dy);

	FieldActOX -= dx;
	FieldActOY -= dy;
}

void ChangeFieldScroll( int dx, int dy )
{
	MoveVMapPosition( dx, dy );
	EvObjMainControl( dx, dy );
	
	MakeMapBG();
	FieldActOX -= dx * 16;
	FieldActOY -= dy * 16;
}

//==================================================================================
//
//	���@�����ɂ���ʕ\���ʒu�R���g���[��
//
//==================================================================================

static pFunc map_view_func;

//-------------------------------------------------------------
//	BG ���W�������
//-------------------------------------------------------------
void SetMapViewFunc( pFunc func )
{
	map_view_func = func;
}

//-------------------------------------------------------------
//	�̾�Ēl���
//-------------------------------------------------------------
void SetMapViewOffset( s16 x, s16 y )
{
	offset_x = BASE_OFF_X + x;
	offset_y = BASE_OFF_Y + y;
}

static void MapViewOffsetCalc(void);

//-------------------------------------------------------------
//-------------------------------------------------------------
void MapViewInit(void)
{
	map_view_func = MapViewOffsetCalc;
	TestOffsetFlag = 0;
	offset_x = BASE_OFF_X;
	offset_y = BASE_OFF_Y;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
void MapViewControl(void)
{
	if( map_view_func != NULL ) map_view_func();

	ActOX = FieldActOX - offset_x;
	ActOY = FieldActOY - offset_y - DOT_OFF_Y;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void MapViewOffsetCalc(void)
{
		u8 site;

		if(MapViewControlFlag == 0)
		{
				MapViewInit();
				return ;
		};
#if 1
		if(MyMoveState.mv_state == EV_M_ON)
		{
				TestOffsetFlag ^= 1;
				if(TestOffsetFlag==0)return;
		}else{
				TestOffsetFlag = 0;
		};
#endif

		site = GetHeroMoveSite();

		if(site == SITE_UP)
		{
				if(offset_y > BASE_OFF_Y - MOVE_LENGTH)
				{
						offset_y -= CHANGE_SPEED;
				};
		}
		else if(site == SITE_DOWN)
		{
				if(offset_y < BASE_OFF_Y + MOVE_LENGTH)
				{
						offset_y += CHANGE_SPEED;
				};
		}
		else if(offset_y < BASE_OFF_Y)
		{
				offset_y+= CHANGE_SPEED;
		}
		else if( offset_y > BASE_OFF_Y)
		{
				offset_y -= CHANGE_SPEED;
		};
};

