//=========================================================================
//
//
//	���z�t�B�[���h�}�b�v�E�}�b�v�Z���֘A
//
//
//=========================================================================

#include "common.h"
#include "field.h"

#include "map.h"

#define FIELDMAP_H_GLOBAL
#include "fieldmap.h"

#include "mapchr.h"
#include "script.h"
#include "fld_tool.h"
#include "base.h"
#include "tv.h"


//==================================================================================
//	�萔��`
//==================================================================================
#define MAX_DMYSCR_X	(32+2)						// ���z��ʂw�T�C�Y�i�L�����P�ʁj
#define MAX_DMYSCR_Y	(32)						// ���z��ʂx�T�C�Y�i�L�����P�ʁj

#define VIRTUAL_MAP_SIZE	(1024 * 10)					// ���z�}�b�v�T�C�Y

typedef struct{
		u8 down:1;
		u8 up:1;
		u8 left:1;
		u8 right:1;
}MAP_LINK_STATUS;


//==================================================================================
//	�ϐ���`
//==================================================================================
//	�O��RAM(0x2000000-)
static u16 VirtualMap[VIRTUAL_MAP_SIZE] = {};

MapTotalData NowMap ={};				// ���݂̃}�b�v�̍\����

MapChg MapChange ={};

static MAP_LINK_STATUS MapLinkStatus = {};

//	����RAM(0x3000000-)
VMAP_STATUS VMap;

//	ROM
const MAP_LINK_STATUS NoLinkStatus = {OFF,OFF,OFF,OFF};



//==================================================================================
//		�v���O����
//==================================================================================


static void MakeVirtualMap( const MTD * now_map );

static void set_vmap_center(const u16 *scrn_data ,u16 Xsize, u16 Ysize);
static void set_vmap_side( const MTD * now_map );
static void set_vmap_down( const MTD * now_map, const MTD * NextMap, int gap_value);
static void set_vmap_up( const MTD * now_map, const MTD * NextMap, int gap_value);
static void set_vmap_left( const MTD * now_map, const MTD * NextMap, int gap_value);
static void set_vmap_right( const MTD * now_map, const MTD * NextMap, int gap_value);

static const MLink * get_next_link(u8 direction, int x ,int y);
static u8 MapLinkPosCheck(u8 direction, int x, int y, const MLink * Link);
static u8 MapLinkPosCheckSub(int now_pos, int now_len, int next_len, int diff);


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const MapTotalData * GetNextLinkMapData(const MLink * Link)
{
	return GetTotalMapData(Link->div_id,Link->map_id);
}

//==================================================================================
//
//	���z�}�b�v����
//
//	���}�b�v�J�ڂ��ƂɈ�x�����Ăяo�����
//
//==================================================================================

void MakeNowVirtualMap( void )
{
	MakeVirtualMap( &NowMap );

	ChangeBaseCell( NowMap.Sxy );

	BgControlScriptCheck();
}

void MakeContinueVirtualMap( void )
{
	MakeVirtualMap( &NowMap );

	SetBaseGoods(0);

	ChangeBaseCell( NowMap.Sxy );

	RecoverVirtualMap();

	BgControlScriptCheck();
	
	ChangeTVCell(VMap.Xsize, VMap.Ysize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static void MakeVirtualMap( const MTD * now_map )
{
	const MapScreenData * scrn;

	scrn = now_map->Screen;

	CpuFastArrayClear( OUTRANGE_CELL|(OUTRANGE_CELL<<16), VirtualMap );

	VMap.Data = VirtualMap;
	VMap.Xsize = MAP_LEFT_MARGIN + scrn->Xsize + MAP_RIGHT_MARGIN;
	VMap.Ysize = MAP_UP_MARGIN + scrn->Ysize + MAP_DOWN_MARGIN;

	/* �}�b�v���傫������ꍇ�A�G���[�΍� */
	if (VMap.Xsize*VMap.Ysize > VIRTUAL_MAP_SIZE) return;

	set_vmap_center(scrn->ScreenData ,scrn->Xsize ,scrn->Ysize);

	set_vmap_side( now_map );
}

//-----------------------------------------------------------------------------
//���C���ƂȂ�}�b�v��`�悷��
//-----------------------------------------------------------------------------
static void set_vmap_center(const u16 *scrn_data, u16 x_size, u16 y_size )
{
	int y;
	u16 * vmap;

	vmap = VMap.Data;
	vmap += VMap.Xsize * MAP_UP_MARGIN + MAP_LEFT_MARGIN;

	for ( y=0; y<y_size; y++ ) {
		CpuCopy( scrn_data, vmap, x_size*2, 16 );
		vmap += MAP_RIGHT_MARGIN + x_size + MAP_LEFT_MARGIN;
		scrn_data += x_size;
	}
}

//-----------------------------------------------------------------------------
// �����N������́A���ӂ̃}�b�v�̈ꕔ��`�悷��
//-----------------------------------------------------------------------------
static void set_vmap_side( const MTD * now_map )
{
	int i;
	int diff;
	int count;
	const MTD * next_map;
	const MLink * Link;

	count = now_map->Link->count;
	Link = now_map->Link->Links;

	MapLinkStatus = NoLinkStatus;

	for ( i=0; i<count; i++,Link++ ) {

		next_map = GetNextLinkMapData(Link);
		diff = Link->PosDiff;

		switch ( Link->direction ) {

		case SITE_DOWN:
			set_vmap_down( now_map, next_map, diff );
			MapLinkStatus.down = ON;
			break;

		case SITE_UP:
			set_vmap_up( now_map, next_map, diff );
			MapLinkStatus.up = ON;
			break;

		case SITE_LEFT:
			set_vmap_left( now_map, next_map, diff );
			MapLinkStatus.left = ON;
			break;

		case SITE_RIGHT:
			set_vmap_right( now_map, next_map, diff );
			MapLinkStatus.right = ON;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// �}�b�v�̈ꕔ��`�悷��
//-----------------------------------------------------------------------------
static void write_vmap_parts(
				int w_sx, int w_sy,
				const MTD * NextMap,int r_sx, int r_sy,
				int w_width, int w_height)
{
	int y;
	int r_map_x;
	const u16 * src;
	u16 * dst;

	r_map_x = NextMap->Screen->Xsize;
	src = &(NextMap->Screen->ScreenData[r_map_x * r_sy + r_sx]);
	dst = &(VMap.Data[VMap.Xsize*w_sy+w_sx]);

	for ( y=0; y<w_height; y++ ) {
		CpuCopy( src, dst, w_width*2, 16 );
		dst += VMap.Xsize;
		src += r_map_x;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static void set_vmap_down( const MTD * now_map, const MTD * next_map, int gap_value)
{
		int w_sx,w_sy,	r_sx,r_sy;
		int link_width,link_height;
		int w_width,w_height;

		if(next_map==NULL)return;

		link_width=next_map->Screen->Xsize;
		link_height=next_map->Screen->Ysize;

		w_sx=MAP_LEFT_MARGIN+gap_value;
		w_sy=now_map->Screen->Ysize+MAP_UP_MARGIN;
		r_sy = 0;//		r_sy = link_height;
		w_height = MAP_DOWN_MARGIN;

		if( w_sx<0 ){
				r_sx = -(w_sx);
				if(w_sx+link_width < VMap.Xsize){
						w_width = w_sx+link_width;
				}else{
						w_width = VMap.Xsize;
				}
				w_sx = 0;
		}else{
				r_sx = 0;
				if(w_sx+link_width < VMap.Xsize){
						w_width = link_width;
				}else{
						w_width = VMap.Xsize - w_sx;
				}
		};

	write_vmap_parts(w_sx,w_sy,
					next_map,r_sx,r_sy,
					w_width,w_height);
					
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void set_vmap_up( const MTD * now_map, const MTD * next_map, int gap_value)
{
		int w_sx,w_sy,	r_sx,r_sy;
		int link_width,link_height;
		int w_width,w_height;


		if(next_map==NULL)return;

		link_width=next_map->Screen->Xsize;
		link_height=next_map->Screen->Ysize;

		w_sx=0+MAP_LEFT_MARGIN+gap_value;
		w_sy=0;
		r_sy = link_height - MAP_UP_MARGIN;
		w_height = MAP_UP_MARGIN;

		if( w_sx<0 ){
				r_sx = -(w_sx);
				if(w_sx+link_width < VMap.Xsize){
						w_width = w_sx+link_width;
				}else{
						w_width = VMap.Xsize;
				}
				w_sx = 0;
		}else{
				r_sx = 0;
				if(w_sx+link_width < VMap.Xsize){
						w_width = link_width;
				}else{
						w_width = VMap.Xsize - w_sx;
				}
		};

	write_vmap_parts(w_sx,w_sy,
					next_map,r_sx,r_sy,
					w_width,w_height);
					
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void set_vmap_left( const MTD * now_map, const MTD * next_map, int gap_value)
{
		int w_sx,w_sy,	r_sx,r_sy;
		int link_width,link_height;
		int w_width,w_height;


		if(next_map==NULL)return;

		link_width=next_map->Screen->Xsize;
		link_height=next_map->Screen->Ysize;

		w_sx=0;
		w_sy=0+MAP_UP_MARGIN+gap_value;
		r_sx= link_width - MAP_LEFT_MARGIN;
		w_width = MAP_LEFT_MARGIN;
		if(w_sy<0){
				r_sy= -(w_sy);
				if(w_sy+link_height<VMap.Ysize){
						w_height = w_sy+link_height;
				}else{
						w_height = VMap.Ysize;
				};
				w_sy = 0;
		}else{
				r_sy= 0;
				if(w_sy+link_height<VMap.Ysize){
						w_height = link_height;
				}else{
						w_height = VMap.Ysize - w_sy;
				};
		};
	write_vmap_parts(w_sx,w_sy,
					next_map,r_sx,r_sy,
					w_width,w_height);
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void set_vmap_right( const MTD * now_map, const MTD * next_map, int gap_value)
{
		int w_sx,w_sy,	r_sx,r_sy;
		int link_width,link_height;
		int w_width,w_height;


		if(next_map==NULL)return;

		link_width=next_map->Screen->Xsize;
		link_height=next_map->Screen->Ysize;

		w_sx=now_map->Screen->Xsize+MAP_LEFT_MARGIN;
		w_sy=0+MAP_UP_MARGIN+gap_value;
		r_sx=0;
		w_width = MAP_RIGHT_MARGIN;
		if(w_sy<0){
				r_sy= -(w_sy);
				if(w_sy+link_height<VMap.Ysize){
						w_height = w_sy+link_height;
				}else{
						w_height = VMap.Ysize;
				};
				w_sy = 0;
		}else{
				r_sy= 0;
				if(w_sy+link_height<VMap.Ysize){
						w_height = link_height;
				}else{
						w_height = VMap.Ysize - w_sy;
				};
		};
	write_vmap_parts(w_sx,w_sy,
					next_map,r_sx,r_sy,
					w_width,w_height);
};





//==================================================================================
//
//	�}�b�v�Z���f�[�^�֘A���[�`��
//
//==================================================================================

#define Y_OFS	(MAP_UP_MARGIN % 2)
#define X_OFS	(MAP_LEFT_MARGIN % 2)

//	�Z���f�[�^�擾
#define GET_CELL(x,y)		(VMap.Data[(x) + VMap.Xsize * (y)])

//	���z�}�b�v�͈͓��`�F�b�N
#define	IS_INSIDE_VMAP(x,y)	(0<=(x) && (x)<VMap.Xsize && 0<=(y) && (y)<VMap.Ysize )

//-----------------------------------------------------------------------------
//	�͈͊O�̃Z����`���擾
//-----------------------------------------------------------------------------
#define	GetOutCell(x,y) ( (NowMap.Screen->OutRangeCell)\
		[ ( (x + X_OFS) & 1) + ( ( (y + Y_OFS) & 1)<<1 ) ] | CELL_HITCHECK_MASK )

//-----------------------------------------------------------------------------
// ���̃Z���f�[�^���擾
//-----------------------------------------------------------------------------

#define GetCell(x,y) \
	( IS_INSIDE_VMAP((x),(y)) ? \
	  GET_CELL((x),(y)) :		\
	  GetOutCell((x),(y))		\
	)



//-----------------------------------------------------------------------------
//	�w��ʒu�̍������擾
//-----------------------------------------------------------------------------
u8 GetCellHeight(int px, int py)
{
	int cell;
	cell = GetCell(px,py);
	if( cell == OUTRANGE_CELL )return 0;		// �Ƃ肠����
	return (CELL_HEIGHT_MASK & cell ) >> CELL_HEIGHT_SHIFT ;
}

//-----------------------------------------------------------------------------
//	�w��ʒu���i���\���ǂ����̏��擾
//	Return 0:�i���n�j
//		�@ 1:�i���m�f
//-----------------------------------------------------------------------------
u8 GetCellHitCheck(int px, int py)
{
	int cell;
	cell = GetCell(px,py);
	if( cell == OUTRANGE_CELL )return 1;		// �͈͊O�ړ��������Ȃ�
	return (CELL_HITCHECK_MASK & cell ) >> CELL_HITCHECK_SHIFT ;
}


//-----------------------------------------------------------------------------
//	�w��ʒu�̃Z���R�[�h�擾
//-----------------------------------------------------------------------------
int GetCellCode(int px, int py)
{
	int cell;
	cell = GetCell( px, py );
	if ( cell == OUTRANGE_CELL ) {
			return GetOutCell(px,py) & CELL_CODE_MASK;
	} else {
			return cell & CELL_CODE_MASK;
	}
}

//-----------------------------------------------------------------------------
//	�w��ʒu�̃Z���A�g���r���[�g���擾����
//-----------------------------------------------------------------------------
int GetCellAttribute(int px,int py)
{
	int cell;

	cell = GetCellCode(px,py);
	return	GetMapCellAttributeData(cell) & CELLATTR_DATA_MASK;
}

//-----------------------------------------------------------------------------
//	�w��ʒu�̃Z���`��BG�K�w���擾����
//-----------------------------------------------------------------------------
u8 GetCellLayer(int px, int py)
{
	int cell;

	cell = GetCellCode(px,py);
	return ( GetMapCellAttributeData(cell) & CELLATTR_LAYER_MASK ) >> CELLATTR_LAYER_SHIFT;
}



//-----------------------------------------------------------------------------
//	�w��ʒu�̃Z��������������
//-----------------------------------------------------------------------------
void SetCell(int map_x, int map_y ,u16 cell)
{
	if ( IS_INSIDE_VMAP(map_x,map_y) ) {
		VMap.Data[map_x + VMap.Xsize * map_y] &= CELL_HEIGHT_MASK;
		VMap.Data[map_x + VMap.Xsize * map_y] |= ( cell & (CELL_HITCHECK_MASK | CELL_CODE_MASK) );
	}
}
// 2002/07/22 by nakahiro
void SetCellDirect(int map_x, int map_y ,u16 cell)
{
	if ( IS_INSIDE_VMAP(map_x,map_y) )
		VMap.Data[map_x + VMap.Xsize * map_y] = cell;
}

//-----------------------------------------------------------------------------
//	�Z���R�[�h���Z���A�g���r���[�g�f�[�^
//-----------------------------------------------------------------------------
u16 GetMapCellAttributeData(u16 code)
{
	if ( code < MAP_CELL_2ND_NO ) {
		return NowMap.Screen->MainCharData->CellAttrData[code];
	}
	else if ( code < MAP_CELL_2ND_NO + MAP_CELL_2ND_SIZE ) {
		return NowMap.Screen->SubCharData->CellAttrData[code - MAP_CELL_2ND_NO];
	}
	else {
		return 0xff;
	}
}


//-----------------------------------------------------------------------------
//	���z�}�b�v�i�̈ꕔ)��ۑ�����
//-----------------------------------------------------------------------------
void BackupVirtualMap( void )
{
	int x,y,sx,sy;
	int x_width;
	u16 * p = Fld.VirtualMapSv;

	x_width = VMap.Xsize;
	sx = Fld.MapXpos;
	sy = Fld.MapYpos;

	for ( y=sy; y<sy+SCR_BG_CELL_Y; y++ ) {
		for ( x=sx; x<sx+SCR_BG_CELL_X; x++ ) {
			*p = VirtualMap[ x + x_width * y ];
			p++;
		}
	}
}

//-----------------------------------------------------------------------------
//	���z�}�b�v�i�̈ꕔ)�𕜋A����
//-----------------------------------------------------------------------------
static int is_empty_backup_vmap( void )
{
	u16 i;
	u16 value = 0;
	for ( i = 0; i < sizeof(Fld.VirtualMapSv); i ++ ) {
		value |= Fld.VirtualMapSv[i];
	}
	if ( value == 0 )
		return TRUE;
	else
		return FALSE;
}

static void clear_backup_vmap( void )
{
	CpuArrayClear( 0, Fld.VirtualMapSv, 16 );
}

void RecoverVirtualMap( void )
{
	int x,y,sx,sy;
	int x_width;
	u16 * p = Fld.VirtualMapSv;

	if ( is_empty_backup_vmap() ) return;

	x_width = VMap.Xsize;
	sx = Fld.MapXpos;
	sy = Fld.MapYpos;

	for ( y=sy; y<sy+SCR_BG_CELL_Y; y++ ) {
		for ( x=sx; x<sx+SCR_BG_CELL_X; x++ ) {
			VirtualMap[ x + x_width * y ] = *p;
			p++;
		}
	}

	clear_backup_vmap();
}

void RecoverVirtualMapAtScroll( u8 site )
{
	int x,y,sx,sy,ex,ey;
	int vx,vy;
	int x_width;
	u16 * p = Fld.VirtualMapSv;

	x_width = VMap.Xsize;
	vx = 0;
	vy = 0;
	sx = Fld.MapXpos;
	sy = Fld.MapYpos;
	ex = SCR_BG_CELL_X;
	ey = SCR_BG_CELL_Y;

	switch ( site ) {
	case SITE_UP:
		sy ++;
		ey = SCR_BG_CELL_Y - 1;
		break;
	case SITE_DOWN:
		vy ++;
		ey = SCR_BG_CELL_Y - 1;
		break;
	case SITE_LEFT:
		sx ++;
		ex = SCR_BG_CELL_X - 1;
		break;
	case SITE_RIGHT:
		vx ++;
		ex = SCR_BG_CELL_X - 1;
		break;
	}

	for ( y=0; y<ey; y++ ) {
		for ( x=0; x<ex; x++ ) {
			VirtualMap[ x+sx + x_width * (y+sy) ] = p[vx+x + SCR_BG_CELL_X *(vy+y)];
		}
	}

	clear_backup_vmap();
}

//==================================================================================
//
//	�}�b�v�ړ��`�F�b�N�֘A���[�`��
//
//==================================================================================
//-----------------------------------------------------------------------------
//	�w��ʒu���}�b�v�����ǂ����𒲂ׂ�
//
// 	Out:	SITE_NOTHING
// 				�}�b�v��
// 			NOT_MOVE_VALUE
// 				�}�b�v�O�A�אڃ}�b�v�Ȃ�
//	 		SITE_XXX
//	 			�}�b�v�O�A�אڃ}�b�v������̂ŕ�����Ԃ�
//-----------------------------------------------------------------------------

int CheckMapLimit(int nextXpos, int nextYpos)
{
		if( GetCell(nextXpos, nextYpos) == OUTRANGE_CELL )
		{
			return NOT_MOVE_VALUE;
		}

		// To Right
		if(nextXpos >= VMap.Xsize - MAP_RIGHT_MARGIN){
				if(MapLinkStatus.right == OFF)return NOT_MOVE_VALUE;
				return SITE_RIGHT;
		}
		// To Left
		if(nextXpos < MAP_LEFT_MARGIN){
				if(MapLinkStatus.left == OFF)return NOT_MOVE_VALUE;
				return SITE_LEFT;
		}
		// To Down
		if(nextYpos >= VMap.Ysize - MAP_DOWN_MARGIN){
				if(MapLinkStatus.down == OFF)return NOT_MOVE_VALUE;
				return SITE_DOWN;
		}
		// To Up
		if(nextYpos < MAP_UP_MARGIN){
				if(MapLinkStatus.up == OFF)return NOT_MOVE_VALUE;
				return SITE_UP;
		}
		return SITE_NOTHING;
};

//-----------------------------------------------------------------------------
//	�}�b�v���S�ʒu���ړ�������Ƃ��̃`�F�b�N
//	<����>		dx	�w�����ړ���
//				dy	�x�����ړ���
//	<�߂�l>	CheckMapLimit�Ɠ���
//-----------------------------------------------------------------------------
static int check_map_change( int dx, int dy )
{
	int nx,ny;

	nx = Fld.MapXpos + CENTER_X_MARGIN + dx;
	ny = Fld.MapYpos + CENTER_Y_MARGIN + dy;
	return CheckMapLimit( nx, ny );
}

//-----------------------------------------------------------------------------
//	�X�N���[���\���ǂ����̃`�F�b�N
//	<����>		����
//	<�߂�l>	1	�ړ��\
//				0	�ړ��s��
//-----------------------------------------------------------------------------
int CheckMapMove( int site )
{
	int res;
	int nx,ny;

	nx = Fld.MapXpos + CENTER_X_MARGIN + SiteVector[site].dx;
	ny = Fld.MapYpos + CENTER_Y_MARGIN + SiteVector[site].dy;
	res = CheckMapLimit( nx, ny);

	if( res == NOT_MOVE_VALUE ) {
		return 0; // �ړ��s��
	} else {
		return 1; // �ړ��\
	}
}


//-----------------------------------------------------------------------------
//	�X�N���[���ł̃}�b�v�؂�ւ��ɂ����W�C��
//-----------------------------------------------------------------------------
static void set_next_position( const MLink * NextLink, int site, int dx, int dy )
{
	const MTD * NextMap;

	NextMap = GetNextLinkMapData( NextLink );

	switch ( site ) {
		// To Right
		case SITE_RIGHT:
		Fld.MapXpos = -dx;
		Fld.MapYpos -= NextLink->PosDiff;
		break;

		// To Left
		case SITE_LEFT:
		Fld.MapXpos = NextMap->Screen->Xsize;
		Fld.MapYpos -= NextLink->PosDiff;
		break;

		// To Down
		case SITE_DOWN:
		Fld.MapXpos -= NextLink->PosDiff;
		Fld.MapYpos = -dy;
		break;

		// To Up
		case SITE_UP:
		Fld.MapXpos -= NextLink->PosDiff;
		Fld.MapYpos = NextMap->Screen->Ysize;
		break;
	};
}


//-----------------------------------------------------------------------------
//
//	���z�}�b�v��̍��W�ړ�
//	���}�b�v�؂�ւ��̏ꍇ�̓}�b�v�f�[�^�̏������������Ăяo��
//
//-----------------------------------------------------------------------------
int MoveVMapPosition( int dx, int dy )
{
	int site;

	MapChange.chg_flag = FALSE;

	site = check_map_change( dx, dy );

	if ( site == NOT_MOVE_VALUE || site == SITE_NOTHING ) {

		//�X�N���[�����邾���̏ꍇ
		Fld.MapXpos += dx;
		Fld.MapYpos += dy;

	} else {

		//�X�N���[���Ń}�b�v���؂�ւ��ꍇ
		int oldx,oldy;
		const MLink * NextLink;

		BackupVirtualMap();					//���͂̉��z�}�b�v��ۑ�

		oldx = Fld.MapXpos;
		oldy = Fld.MapYpos;

		NextLink = get_next_link( site, Fld.MapXpos, Fld.MapYpos );
		set_next_position( NextLink, site, dx, dy );
		ChangeMapData( NextLink->div_id, NextLink->map_id );

		MapChange.chg_flag = TRUE;
		MapChange.diffx = oldx - Fld.MapXpos;
		MapChange.diffy = oldy - Fld.MapYpos;

		Fld.MapXpos += dx;
		Fld.MapYpos += dy;

		RecoverVirtualMapAtScroll( site );	//���͂̉��z�}�b�v�𕜋A
	};

	return MapChange.chg_flag;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static const MLink * get_next_link(u8 direction, int x ,int y)
{
	int count;
	int max;
	const MLink * Link;

	max = NowMap.Link->count;
	Link = NowMap.Link->Links;

	for ( count=0; count<max; count++, Link++ ) {
		if ( Link->direction != direction ) continue;
		if ( MapLinkPosCheck(direction, x, y, Link) ==TRUE)
			return Link;
	}

	return NULL;
}

static u8 MapLinkPosCheck(u8 direction, int x, int y, const MLink * Link)
{
	const MTD * next;

	next = GetNextLinkMapData(Link);

	switch ( direction ) {
	case SITE_DOWN:
	case SITE_UP:
		return MapLinkPosCheckSub(x, NowMap.Screen->Xsize, next->Screen->Xsize, Link->PosDiff);

	case SITE_LEFT:
	case SITE_RIGHT:
		return MapLinkPosCheckSub(y, NowMap.Screen->Ysize, next->Screen->Ysize, Link->PosDiff);

	}
	return FALSE;	/* ���Ă͂܂�Ȃ��ꍇ */
}

static u8 MapLinkPosCheckSub(int now_pos, int now_len, int next_len, int diff)
{
	int min,max;

	if (diff < 0) {
		min = 0;
	} else {
		min = diff;
	}
	if (next_len + diff < now_len) {
		max = next_len + diff;
	} else {
		max = now_len;
	}
	if (min <= now_pos && now_pos <= max)
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------------------------
//	���W���炻�̏ꏊ��������}�b�v�̃����N�����擾����
//	<����>		x,y	�}�b�v���W
//	<�߂�l>	�����N�\���̂ւ̃|�C���^
//-----------------------------------------------------------------------------
static int check_link_range(int pos, int map_size)
{
	if (0 <= pos && pos < map_size)
		return TRUE;
	else
		return FALSE;
}

static int check_link_pos(const MLink * link, int x, int y)
{
	const MTD * next;

	next = GetNextLinkMapData(link);

	switch (link->direction) {
		case SITE_DOWN:
		case SITE_UP:
			return check_link_range(x - link->PosDiff, next->Screen->Xsize);

		case SITE_LEFT:
		case SITE_RIGHT:
			return check_link_range(y - link->PosDiff, next->Screen->Ysize);
	}
	return FALSE;
}

const MLink * GetLinkMap(s16 x, s16 y)
{
	int count;
	int max;
	const MLink * Link;
	u8 site;

	if (NowMap.Link == NULL) return NULL;
	max = NowMap.Link->count;
	Link = NowMap.Link->Links;

	for ( count=0; count<max; count++, Link++ ) {
		site = Link->direction;
		if (site == SITE_BELOW || site == SITE_ABOVE) continue;
		if (site == SITE_UP && y >= CENTER_Y_MARGIN) continue;
		if (site == SITE_DOWN && y < NowMap.Screen->Ysize+CENTER_Y_MARGIN) continue;
		if (site == SITE_LEFT && x >= CENTER_X_MARGIN) continue;
		if (site == SITE_RIGHT && x < NowMap.Screen->Xsize+CENTER_X_MARGIN) continue;
		if (check_link_pos(Link, x - CENTER_X_MARGIN, y - CENTER_Y_MARGIN) == TRUE)
			return Link;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SetMapCenterPos( u16 x, u16 y )
{
	Fld.MapXpos = x - CENTER_X_MARGIN;
	Fld.MapYpos = y - CENTER_X_MARGIN;
}

void GetMapCenterPos(u16* x, u16* y)
{
		*x = Fld.MapXpos+CENTER_X_MARGIN;
		*y = Fld.MapYpos+CENTER_Y_MARGIN;
};

void SetNowMapPos( u16 x, u16 y )
{
	Fld.MapXpos = x;
	Fld.MapYpos = y;
}

void GetNowMapPos(u16* x, u16* y)
{
		*x = Fld.MapXpos;
		*y = Fld.MapYpos;
};


