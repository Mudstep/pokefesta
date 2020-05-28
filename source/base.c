//=========================================================================================
//										�閧��n
//								2001/10/04 by nakahiro
//=========================================================================================
#include "common.h"
#include "base.h"
#include "jiki.h"
#include "fieldmap.h"
#include "fld_main.h"
#include "mapbg.h"
#include "mapparam.h"
#include "script.h"
#include "palanm.h"
#include "task.h"
#include "pm_str.h"
#include "fntequ.h"
#include "menu.h"
#include "fld_tool.h"
#include "fld_task.h"
#include "map_attr.h"
#include "poketool.h"
#include "fight.h"
#include "sxytype.h"
#include "pokesio.h"
#include "gds_main.h"
#include "gds_put.h"
#include "gds_get.h"
#include "mus_tool.h"
#include "fan.h"
#include "fe_base.h"
#include "mapname.h"
#include "itemtool.h"
#include "message.h"
#include "record.h"
#include "weather.h"

#include "../evdata/maparray.h"
#include "../evdata/sxysym.h"

#include "../data/gds_sym.h"

#include "syswork.h"
#include "sysflag.h"		//TMFLG_...
#include "../script/savework.h"		//OBJCHRWORK�Q�Ƃ̂���
#include "../script/saveflag.h"		//FE_...

//=========================================================================================
//									�V���{����`
//=========================================================================================
#define	BASE_ENTRY_MAX		10		// ��n�o�^�ő吔
#define	SECRET_BOX_CELL		544		// �閧�̔��Z���R�[�h�i�����̊�n�̃p�\�R���j
#define	BASE_BOARD_CELL		545		// �f���̃Z���R�[�h�i���l�̊�n�̃p�\�R���j
#define	BASE_NORMAL_CELL	522		// ��n�̒n�ʂ̃Z���R�[�h

#define CAVE_MAKE_CELL_CODE		0x90	// �y�̕���������ꏊ�̃Z���R�[�h
#define CAVE_BASE_CELL_CODE		0x91	// �y�̕����̃Z���R�[�h
#define ROCK_MAKE_CELL_CODE		0x92	// �΂̕���������ꏊ�̃Z���R�[�h
#define ROCK_BASE_CELL_CODE		0x93	// �΂̕����̃Z���R�[�h
#define SAND_MAKE_CELL_CODE		0x94	// ���̕���������ꏊ�̃Z���R�[�h
#define SAND_BASE_CELL_CODE		0x95	// ���̕����̃Z���R�[�h
#define TREE_L_MAKE_CELL_CODE	0x96	// �؂̕����i���j������ꏊ�̃Z���R�[�h
#define TREE_L_BASE_CELL_CODE	0x97	// �؂̕����i���j�̃Z���R�[�h
#define TREE_R_MAKE_CELL_CODE	0x9c	// �؂̕����i�E�j������ꏊ�̃Z���R�[�h
#define TREE_R_BASE_CELL_CODE	0x9d	// �؂̕����i�E�j�̃Z���R�[�h
#define GRASS_MAKE_CELL_CODE	0x98	// ���̕���������ꏊ�̃Z���R�[�h
#define GRASS_BASE_CELL_CODE	0x99	// ���̕����̃Z���R�[�h
#define SIME_MAKE_CELL_CODE		0x9a	// ���̕���������ꏊ�̃Z���R�[�h
#define SIME_BASE_CELL_CODE		0x9b	// ���̕����̃Z���R�[�h

// �o�^���X�g�@�^�X�N���[�N
#define	WK_ENTRY_MAX	0	// �o�^��
#define	WK_ENTRY_POS	1	// �I���ʒu
#define	WK_ENTRY_SCR	2	// �X�N���[���J�E���^
#define	WK_ENTRY_POSMAX	3	// �I��
#define	WK_RESET_NO		4	// �I�����ꂽ��n�ԍ�

// �o�^���X�g�\�����W
#define	ENTRY_LIST_WINX		17
#define	ENTRY_LIST_WINY		0
#define	ENTRY_LIST_WINEX	( ENTRY_LIST_WINX + 12 )
#define	ENTRY_LIST_WINEY	19

#define	LIST_SCR_CUR_X		( ( ENTRY_LIST_WINX + ( ENTRY_LIST_WINEX-ENTRY_LIST_WINX ) / 2 ) * 8 + 4 )
#define	LIST_SCR_CUR_UY		( ENTRY_LIST_WINY * 8 + 8 )
#define	LIST_SCR_CUR_DY		( ENTRY_LIST_WINEY * 8 )

// �o�^�������j���[���W
#define	ENTRY_MENU_WINX		1
#define	ENTRY_MENU_WINY		0
#define	ENTRY_MENU_WINEX	( ENTRY_MENU_WINX + 11 )
#define	ENTRY_MENU_WINEY	( ENTRY_MENU_WINY + 5 )

// �o�^�����͂��E���������W
#define	ENTRY_YESNO_WINX	20
#define ENTRY_YESNO_WINY	8
#define	ENTRY_YESNO_WINEX	( ENTRY_YESNO_WINX + 6 )
#define ENTRY_YESNO_WINEY	( ENTRY_YESNO_WINY + 5 )


//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void SetNowExitPos(void);

static void EntryListTaskInit( u8 id );
static void EntryListEnd( u8 id );
static void	DrawEntryListWin( u8 id );
static void EntryListTaskMain( u8 id );
static u8	EntryListSelectBaseNoGet( u8 pos );
static void EntryResetInit( u8 id );
static void ReturnEntrySelect( u8 id );
static void EntryResetMenuInit( u8 id );
static void EntryResetMenuMain( u8 id );
static void EntryResetYesNoInit( u8 id );
static void EntryResetYes( u8 id );
static void EntryResetNo( u8 id );

u8 GetOrusubanCharID( u8 no );	// ������ԃL������ID���擾

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
extern u16	field_CellAtbData[];
extern u16	AnswerWork;
extern const VM_CODE base_in_script[];
extern const VM_CODE base_in_obj_goods_init[];
extern const VM_CODE return_mine_base_board[];
extern const VM_CODE return_other_base_pc[];


// ��n�̓�����Z���ԍ�
static const BaseExit	base_exit[] =
{
	{  38,  54 },	// �؁i���j
	{  39,  55 },	// �؁i�E�j
	{ 416, 417 },	// �y
	{ 424, 425 },	// ��
	{ 432, 433 },	// ��
	{ 520, 528 },	// ��
	{ 625, 632 },	// ��
};

static u8	BaseNo = 0;			// ��n�ԍ�

// ��n�̃}�b�v�f�[�^�e�[�u��
// ID. Exit, PC_X, PC_Y
static const s8 BaseMap_Tbl[] = {
	SP_BASE01, SP_BASE01_MAT00,  1,  3,
	SP_BASE07, SP_BASE07_MAT00,  5,  9,
	SP_BASE13, SP_BASE13_MAT00,  1,  3,
	SP_BASE19, SP_BASE19_MAT00,  7, 13,

	SP_BASE02, SP_BASE02_MAT00,  2,  3,
	SP_BASE08, SP_BASE08_MAT00,  9,  2,
	SP_BASE14, SP_BASE14_MAT00, 13,  4,
	SP_BASE20, SP_BASE20_MAT00,  1,  2,

	SP_BASE03, SP_BASE03_MAT00,  1,  3,
	SP_BASE09, SP_BASE09_MAT00,  1,  2,
	SP_BASE15, SP_BASE15_MAT00,  3, 15,
	SP_BASE21, SP_BASE21_MAT00,  3, 14,

	SP_BASE04, SP_BASE04_MAT00,  9,  3,
	SP_BASE10, SP_BASE10_MAT00,  8,  7,
	SP_BASE16, SP_BASE16_MAT00,  3,  6,
	SP_BASE22, SP_BASE22_MAT00,  5,  9,

	SP_BASE05, SP_BASE05_MAT00,  2,  3,
	SP_BASE11, SP_BASE11_MAT00,  5,  6,
	SP_BASE17, SP_BASE17_MAT00, 15,  3,
	SP_BASE23, SP_BASE23_MAT00,  4, 10,

	SP_BASE06, SP_BASE06_MAT00,  3,  3,
	SP_BASE12, SP_BASE12_MAT00,  1,  2,
	SP_BASE18, SP_BASE18_MAT00,  7,  8,
	SP_BASE24, SP_BASE24_MAT00,  9,  6,
};

static const MENU_ITEM entry_list_menu[] =
{
	{ str_baselist_kaizyo, EntryResetInit    },	// �Ƃ��낭��������
	{ str_modoru,          ReturnEntrySelect },	// ���ǂ�
};

static const SELECT_ITEM EntryReset = { EntryResetYes, EntryResetNo };

// ������ԃL�����R�[�h
static const u8	orusuban_char[] = {
	CAPBOY1,		// ����ς�@������
	CAPBOY2,		// �ނ��Ƃ�@���傤�˂�
	BOY4,			// ���ڂ������
	BOYSCOUT1,		// �L�����v�{�[�C
	TRAINERM1,		// �G���[�g�g���[�i�[

	SCHOOLW1,		// �~�j�X�J�[�g
	GIRL3,			// ���キ������
	WOMAN1,			// �����傤����
	GIRLSCOUT1,		// �s�N�j�b�N�K�[��
	TRAINERW1		// �G���[�g�g���[�i�[
};


//---------------------------
//		�閧��n������
//---------------------------
static void InitBaseAdrs( SECRET_BASE_DATA * base )
{
	u16	i,j;

	base->pos = 0;

	for( i=0; i<PERSON_NAME_SIZE; i++ )
		base->name[i] = EOM_;

	for( i=0; i<4; i++ )
		base->id[i] = 0;

	base->org_count     = 0;
	base->enter_count   = 0;
	base->exchange_item = 0;

	base->entry_flag = 0;
	base->fight_flag = 0;
	base->sex = 0;
	base->flg = 0;

	for( i=0; i<16; i++){
		base->furniture_id[i]  = 0;
		base->furniture_pos[i] = 0;
	}

	for( i=0; i<TEMOTI_POKEMAX; i++ ){
		for( j=0; j<4; j++ ){
			base->battle.waza[i*4+j] = 0;
		}
		base->battle.p_rnd[i]   = 0;
		base->battle.exp[i]     = 0;
		base->battle.mons_no[i] = 0;
		base->battle.item[i]    = 0;
		base->battle.level[i]   = 0;
	}
}

void InitBase( u8 base_no )
{
	InitBaseAdrs( &Fld.SecretBase[ base_no ] );
}

//�S������
void InitAllBase(void)
{
	u16	i;

	for( i=0; i<SECRET_BASE_MAX; i++ )
		InitBase( i );
}

//---------------------------
//		��n�ԍ����Z�b�g
//---------------------------
void SetBaseCheck(void)
{
	BaseNo = ScriptParameter0;
}

//--------------------------------
//		��n����Ȃ��`�F�b�N
//--------------------------------
void BaseCheck(void)
{
	u16	i;

	AnswerWork = 0;

	for( i=0; i<SECRET_BASE_MAX; i++ ){
		if(	BaseNo == Fld.SecretBase[i].pos ){
			AnswerWork = 1;
			SetEventWorkValue( WK_BASE_POSNO, i );
			break;
		}
	}
}

//-----------------------------------------
//		�����̊�n�̂���Ȃ��`�F�b�N
//-----------------------------------------
void MyBaseCheck(void)
{
	if(	Fld.SecretBase[0].pos != 0 )	AnswerWork = 1;
	else								AnswerWork = 0;
}


//--------------------------
//		��n�̎�ސݒ�
//--------------------------
static u8 BaseTypeSet(void)
{
	s16	px,py,atb;

	GetHeroFrontCellPos(&px,&py);
	atb = GetCellAttribute(px,py) & 0xfff;

	//�y�������쐬
	if( atb == CAVE_MAKE_CELL_CODE || atb == CAVE_BASE_CELL_CODE )
		return	1;
	//�΂̕������쐬
	if( atb == ROCK_MAKE_CELL_CODE || atb == ROCK_BASE_CELL_CODE )
		return	2;
	//���̕������쐬
	if( atb == SIME_MAKE_CELL_CODE || atb == SIME_BASE_CELL_CODE )
		return	3;
	//���̕������쐬
	if( atb == SAND_MAKE_CELL_CODE || atb == SAND_BASE_CELL_CODE )
		return	4;
	//�؂̕������쐬
	if( atb == TREE_L_MAKE_CELL_CODE || atb == TREE_L_BASE_CELL_CODE ||
		atb == TREE_R_MAKE_CELL_CODE || atb == TREE_R_BASE_CELL_CODE )
		return	5;
	//���̕������쐬
	if( atb == GRASS_MAKE_CELL_CODE || atb == GRASS_BASE_CELL_CODE )
		return	6;

	return	0;
}

void ScriptBaseTypeSet(void)
{
	ScriptParameter3 = BaseTypeSet();
}

//----------------------------------------------
//		�w�肵���A�g���r���[�g�̃Z�����擾
//----------------------------------------------
s16	GetCellForAttribute(u16 *cell,u8 attr)
{
	u16	i;

	for( i=0; i<512; i++ ){
		if( (cell[i] & 0xfff) == attr )		return	i;
	}

	return	-1;
}

//--------------------------------
//		�w��Z���̏ꏊ���擾
//--------------------------------
static void CheckCellPos( s16 * px, s16 * py, u16 cell_no )
{
	s16	i,j;

	for( i=0; i<NowMap.Screen->Ysize; i++ ){
		for( j=0; j<NowMap.Screen->Xsize; j++ ){
			if((NowMap.Screen->ScreenData[i*NowMap.Screen->Xsize+j] & 0x3ff)==cell_no){
				*px = j;
				*py = i;
				return;
			}
		}
	}
}

//-----------------------------
//		�Z���𒣂肩����
//-----------------------------
void BaseSetCell(void)
{
	s16	px,py,cell;
	u16	i;

	GetHeroFrontCellPos(&px,&py);

	cell = GetCellCode( px, py );

	for( i=0; i<7; i++ ){
		if( base_exit[i].before == cell ){
			SetCell( px, py, (base_exit[i].after |  CELL_HITCHECK_MASK) );
			RewriteSingleBG( px, py );
			return;
		}
	}

	for( i=0; i<7; i++ ){
		if( base_exit[i].after == cell ){
			SetCell( px, py, (base_exit[i].before |  CELL_HITCHECK_MASK) );
			RewriteSingleBG( px, py );
			return;
		}
	}
}

//------------------------------------------
//		��n�̖��O�f�[�^�̍Ō�����擾
//------------------------------------------
u8	GetNameEndPos( u8 * name )
{
	u8	i;

	for( i=0; i<PERSON_NAME_SIZE; i++ ){
		if( name[i] == EOM_ )	return	i;
	}
	return	PERSON_NAME_SIZE;
}

//----------------------------
//		�����̊�n���쐬
//----------------------------
void BaseSet(void)
{
	u16	i;

	Fld.SecretBase[0].pos = BaseNo;

	for( i=0; i<4; i++ ){
		Fld.SecretBase[0].id[i] = MyData.ID[i];
	}

	SetEventWorkValue( WK_BASE_POSNO, 0 );

	PM_strncpy( Fld.SecretBase[0].name, MyData.my_name, GetNameEndPos( MyData.my_name ) );

	Fld.SecretBase[0].sex = MyData.my_sex;

	//�����̊�n������}�b�v�i���o�[��ۑ�
	SetEventWorkValue( WK_BASE_MAPNO, NowMap.WorldPos );
}

//----------------------------------------
//		�}�b�v�؂�ւ����̊�n�m�F
//----------------------------------------
void ChangeBaseCell( const SXY_DATA *bg )
{
	u16	i,j,k;
	s16	cell;

	for( i=0; i<bg->evBgMax; i++){
		if( bg->BgData[i].type == BG_BASE ){
			for( j=0; j<SECRET_BASE_MAX; j++ ){
				if( Fld.SecretBase[j].pos == (u32)bg->BgData[i].script ){
					cell = GetCellCode(	MAP_LEFT_MARGIN + bg->BgData[i].posX,
										MAP_UP_MARGIN + bg->BgData[i].posY	);
					for( k=0; k<7; k++ ){
						if( base_exit[k].before == cell ){
							SetCell(MAP_LEFT_MARGIN + bg->BgData[i].posX,
									MAP_UP_MARGIN + bg->BgData[i].posY,
									(base_exit[k].after | CELL_HITCHECK_MASK) );
							break;
						}
					}
					break;
				}
			}
		}
	}
}

//-------------------------------------
//		�閧��n�փ}�b�v�؂�ւ�
//-------------------------------------
static void SetNextMapBaseID(void)
{
	s8	type;

	type = ( BaseNo / 10 ) << 2;

	NextMapSetByID( SP, BaseMap_Tbl[type], BaseMap_Tbl[type+1] );
}

static void ChangeBaseFadeOut(u8 id)
{
	u16	pos = 0;

	switch( TaskTable[id].work[0] ){
	case 0:
//		SetForceEvent();
		TaskTable[id].work[0] = 1;
		break;
	case 1:
		if(!(FadeData.fade_sw))	TaskTable[id].work[0] = 2;
		break;
	case 2:
		pos = GetEventWorkValue( WK_BASE_POSNO );

		if(	Fld.SecretBase[ pos ].enter_count < 0xff)
			Fld.SecretBase[ pos ].enter_count++;

		SetNextMapBaseID();

		SetNextWorldPosition();

//		pFieldRecoverFunc = AddFieldInTask;
		pFieldRecoverFunc = AddScriptFieldInTask;

		MainProcChange( FieldMainInit );
		DelTask(id);
		break;
	}
}

void ChangeBaseMap(void)
{
	AddTask( ChangeBaseFadeOut, 0 );
//	PaletteFadeReq(0xffffffff,0,0,16,0);
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
	/* ���݈ʒu�����ڑ��Ƃ��ēo�^ */
	SetSpecialMapID( 0, Fld.MapID.div_id, Fld.MapID.map_id, -1 );
}

//-------------------------------------------------
//		�閧��n�쐬����̓}�b�v����\�����Ȃ�
//-------------------------------------------------
u8	BaseNameDraw(void)
{
	if( NowMap.type == MAPTYPE_BASE && GetEventWorkValue(WK_SCENE_BASE_MAKE) == 0 )
		return	FALSE;
	return	TRUE;
}

//---------------------------------
//		�쐬���ɔ閧�̔���u��
//---------------------------------
static void MakeBaseFinal3( u8 id )
{
	EvObjChgSite( &EvObj[MyMoveState.evobjno], SITE_U );

//	if( !(FadeData.fade_sw) ){
	if( FieldFadeinCheck() == 1 ){
		ContinueFieldScript();
		DelTask(id);
	}
}

static void MakeBaseFinal2(void)
{
	s16	i,j;

	SetForceEvent();

	EraseMapName();
	CheckCellPos( &i, &j, SECRET_BOX_CELL );
	SetCell( MAP_LEFT_MARGIN+i, MAP_UP_MARGIN+j, SECRET_BOX_CELL | CELL_HITCHECK_MASK );
	RewriteSingleBG( MAP_LEFT_MARGIN+i, MAP_UP_MARGIN+j );
//	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
	FieldFadeBlackInSet();
	AddTask( MakeBaseFinal3, 0 );
}

static void MakeBaseFinalTask( u8 id )
{
	s8	type;

	if( !(FadeData.fade_sw) ){
		type = ( BaseNo / 10 ) << 2;
		NextMapSetDirect(
			Fld.MapID.div_id, Fld.MapID.map_id, NO_EXIT_ID,
			BaseMap_Tbl[type+2], BaseMap_Tbl[type+3] );
		SetNextWorldPosition();
		pFieldRecoverFunc = MakeBaseFinal2;
		MainProcChange( FieldMainInit );
		DelTask(id);
	}
}
void MakeBaseFinal(void)
{
	u8	id;

	id = AddTask( MakeBaseFinalTask, 0 );
	TaskTable[id].work[0] = 0;
//	PaletteFadeReq(0xffffffff,0,0,16,0);
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
}

//----------------------------
//		�O�b�Y��z�u����
//----------------------------
static u8 CheckBaseMap(void)
{
	if( Fld.MapID.div_id == SP &&
		( Fld.MapID.map_id == SP_BASE01 || Fld.MapID.map_id == SP_BASE02 ||
		  Fld.MapID.map_id == SP_BASE03 || Fld.MapID.map_id == SP_BASE04 ||
		  Fld.MapID.map_id == SP_BASE05 || Fld.MapID.map_id == SP_BASE06 ||
		  Fld.MapID.map_id == SP_BASE07 || Fld.MapID.map_id == SP_BASE08 ||
		  Fld.MapID.map_id == SP_BASE09 || Fld.MapID.map_id == SP_BASE10 ||
		  Fld.MapID.map_id == SP_BASE11 || Fld.MapID.map_id == SP_BASE12 ||
		  Fld.MapID.map_id == SP_BASE13 || Fld.MapID.map_id == SP_BASE14 ||
		  Fld.MapID.map_id == SP_BASE15 || Fld.MapID.map_id == SP_BASE16 ||
		  Fld.MapID.map_id == SP_BASE17 || Fld.MapID.map_id == SP_BASE18 ||
		  Fld.MapID.map_id == SP_BASE19 || Fld.MapID.map_id == SP_BASE20 ||
		  Fld.MapID.map_id == SP_BASE21 || Fld.MapID.map_id == SP_BASE22 ||
		  Fld.MapID.map_id == SP_BASE23 || Fld.MapID.map_id == SP_BASE24 ) )
	{
		return	TRUE;	// ��n
	}

	return	FALSE;		// ��n�ȊO
}

// �Z����������
void SetBaseGoods( u8 pc_del )
{
	u16	i, j, pos;

	if( CheckBaseMap() == FALSE )	return;

//	BaseCheck();	// �z��ԍ��擾

	pos = GetEventWorkValue( WK_BASE_POSNO );

	// BG�O�b�Y
	for( i=0; i<16; i++ ){

		if( Fld.SecretBase[pos].furniture_id[i] != 0 &&
			Fld.SecretBase[pos].furniture_id[i] <= GOODS_DATA_MAX &&
			base_goods[ Fld.SecretBase[pos].furniture_id[i] ].type != DESK_TOP )
		{
			SetGoodsCell(
				MAP_LEFT_MARGIN + ( Fld.SecretBase[pos].furniture_pos[i] >> 4 ),
				MAP_UP_MARGIN + ( Fld.SecretBase[pos].furniture_pos[i] & 0xf ),
				Fld.SecretBase[pos].furniture_id[i] );
		}
	}

	// �閧�̔�
	if( pos != 0 ){
		CheckCellPos( &i, &j, SECRET_BOX_CELL );
		SetCell( MAP_LEFT_MARGIN+i, MAP_UP_MARGIN+j, BASE_BOARD_CELL | CELL_HITCHECK_MASK );
	}else{
		if( pc_del == 1 && GetEventWorkValue( WK_SCENE_BASE_GOODS ) == 1 ){
			CheckCellPos( &i, &j, SECRET_BOX_CELL );
			SetCell( MAP_LEFT_MARGIN+i, MAP_UP_MARGIN+j, BASE_NORMAL_CELL | CELL_HITCHECK_MASK );
		}
	}
}


//02/03/04 changed nohara
#define		GOODS_FLAG_START	(FE_BASE_GOODS_FLAG01)
void SetOBJGoods(void)
{
	u8	 type;
	u8	 i,j;
	u8 * id;
	u8 * pos;
	u8   max;
	u8   atb;
	u16  base_pos;

	base_pos = GetEventWorkValue( WK_BASE_POSNO );

	if( CheckBaseMap() == FALSE ){
		id  = Fld.MyRoomFurnitureID;
		pos = Fld.MyRoomFurniturePos;
		max = 12;
	}else{
		id  = Fld.SecretBase[ base_pos ].furniture_id;
		pos = Fld.SecretBase[ base_pos ].furniture_pos;
		max = 16;
	}

	for( j=0; j<max; j++ ){
		if( id[j] != 0 ){
			type = base_goods[ id[j] ].type;

			if( type == DESK_TOP ){
				for( i=0; i<(NowMap.Sxy->ObjMax) ; i++ ){
					if( NowMap.Sxy->ObjData[i].ev_flag == (GOODS_FLAG_START+ScriptParameter0) )
						break;
				}

				if( i != NowMap.Sxy->ObjMax ){

					ScriptParameter2 = pos[j] >> 4;
					ScriptParameter3 = pos[j] & 0xf;

					atb = GetCellAttribute(
						ScriptParameter2+MAP_LEFT_MARGIN,
						ScriptParameter3+MAP_UP_MARGIN );

					if( ATR_IsGoodsPutGoodsCheck( atb ) == TRUE ||
						ATR_IsBigDollPutCheck( atb ) == TRUE )
					{
						AnswerWork = OBJCHRWORK1+NowMap.Sxy->ObjData[i].obj_code-WKOBJCODE1;
						SetEventWorkValue( AnswerWork, base_goods[ id[j] ].cell[0] );

						AnswerWork = NowMap.Sxy->ObjData[i].id;

						EventFlagReset( GOODS_FLAG_START+ScriptParameter0 );
						EvObjAddId( (u8)AnswerWork, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id );
						EvObjPosAllChgId(
							(u8)AnswerWork, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id,
							(s16)ScriptParameter2, (s16)ScriptParameter3 );
						EvObjSxyIdPosChange( (u8)AnswerWork, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id );
						ScriptParameter0++;
					}
				}
			}
		}
	}
}

// ������ԃL�����̃L�����R�[�h���Z�b�g
void SetOrusubanOBJCode(void)
{
	u16	pos = GetEventWorkValue( WK_BASE_POSNO );

	SetEventWorkValue( OBJCHRWORK16, orusuban_char[ GetOrusubanCharID(pos) ] );
}


//------------------------
//		��n�ԍ��擾
//------------------------
void GetBaseNo( EV_POS *pos, const SXY_DATA *bg )
{
	s16	i;

	for( i=0; i<bg->evBgMax; i++){
		if( bg->BgData[i].type == BG_BASE ){
			if( pos->x == MAP_LEFT_MARGIN+bg->BgData[i].posX &&
				pos->y == MAP_UP_MARGIN+bg->BgData[i].posY )
			{
				BaseNo = (u8)( (u32)bg->BgData[i].script );
				break;
			}
		}
	}
}

//--------------------------------
//		�\���L�[�Ŋ�n�̒���
//--------------------------------
void BaseDoorIn( EV_POS *pos, const SXY_DATA *bg )
{
	GetBaseNo( pos, bg );				// ��n�ԍ��擾
	BaseCheck();						// �z��ԍ��擾
	StartFieldScript( base_in_script ); // �X�N���v�g�N��(�^�X�N�Ő���)
}

//---------------------------
//		�b�������̊m�F
//---------------------------
u8	TalkBaseDoorCheck(void)
{
	SetBaseCheck();
	BaseCheck();
	if( AnswerWork == 1 )	return	FALSE;
	return	TRUE;
}

//----------------------------------
//		��n���o�ă}�b�v�֕��A
//----------------------------------
static void BaseReturnFadeOut(u8 id)
{
	switch( TaskTable[id].work[0] ){
	case 0:
		SetForceEvent();
		TaskTable[id].work[0] = 1;
		break;

	case 1:
		if(!(FadeData.fade_sw))	TaskTable[id].work[0] = 2;
		break;

	case 2:
		NextMapSetBySpID( SPFIELD_MAP_EXIT02 );	/* �������Ƃ��ɓo�^��������ڑ�����Z�b�g */
		SetNextWorldPosition();						/* �}�b�v�J�ڏ��� */

		pFieldRecoverFunc = AddFieldInTask;
		MainProcChange( FieldMainInit );
		ResetForceEvent();
		DelTask(id);
		break;
	}
}

void BaseOut(void)
{
	AddTask( BaseReturnFadeOut, 0 );
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
//	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}

//------------------------------
//		�����̊�n���ǂ���
//------------------------------
void BaseBoardCheck(void)
{
	if(	Fld.SecretBase[0].pos != BaseNo )	AnswerWork = 1;
	else									AnswerWork = 0;
}

//----------------------------
//		��n�̏ꏊ���擾
//----------------------------
u8	GetBasePos( u8 no )
{
	s16	i;

	for( i=0; i<SECRET_BASE_MAX; i++ ){
		if( Fld.SecretBase[i].pos == no )
			return	i;
	}

	return	0;	//�_�~�[
}

//---------------------------
//		��n�̖��O�쐬
//---------------------------
u8 * MakeBaseName2( u8 * str, u8 no )
{
	u8 * eom;
	u8 * name;

	name = Fld.SecretBase[ no ].name;
	eom  = PM_strncpy( str, name, GetNameEndPos( name ) );
	*eom = EOM_;
	return	PM_strcat( str, str_kiti_name );
}

u8 * MakeBaseName( u8 * str )
{
	return	MakeBaseName2( str, GetEventWorkValue( WK_BASE_POSNO ) );
}

void MakeBaseNameScript(void)
{
	u8 * eom;
	u8 * name;
	u8   pos;
	
	pos  = GetEventWorkValue( WK_BASE_POSNO );
	name = Fld.SecretBase[pos].name;
	eom  = PM_strncpy( StrTempBuffer0, name, GetNameEndPos( name ) );
	*eom = EOM_;
}

//---------------------------------
//		��n���o�^����Ă��邩
//---------------------------------
static u8 CheckBaseFlag( u8 no )
{
	if( Fld.SecretBase[no].flg != 0 )	return	TRUE;
	return	FALSE;
}

//---------------------------------
//		�莝���|�P�������Z�b�g
//---------------------------------
// �w�͒l�̕��ς��擾
static u8 GetPokeExpAverage( PokemonParam * pp )
{
	u16	exp;

	exp = PokeParaGet( pp, ID_hp_exp );			// �q�b�g�|�C���g�w�͒l
	exp += PokeParaGet( pp, ID_pow_exp );		// �U���͓w�͒l
	exp += PokeParaGet( pp, ID_def_exp );		// �h��͓w�͒l
	exp += PokeParaGet( pp, ID_agi_exp );		// �f�����w�͒l
	exp += PokeParaGet( pp, ID_spepow_exp );	// ����U���w�͒l
	exp += PokeParaGet( pp, ID_spedef_exp );	// ����h��w�͒l

	return	(u8)( exp / 6 );
}

void BasePokeSet(void)
{
	u16	i,j,no=0;

	BASE_BATTLE_DATA * btl = &Fld.SecretBase[0].battle;

	for( i=0; i<TEMOTI_POKEMAX; i++ ){

		for( j=0; j<4; j++ )
			btl->waza[i*4+j] = 0;
		btl->mons_no[i] = 0;
		btl->item[i]    = 0;
		btl->level[i]   = 0;
		btl->p_rnd[i]   = 0;
		btl->exp[i]     = 0;

		if( PokeParaGet( &PokeParaMine[i], ID_monsno ) != 0 &&
			PokeParaGet( &PokeParaMine[i], ID_tamago_flag ) == 0 )
		{
			for( j=0; j<4; j++ )
				btl->waza[no*4+j] = PokeParaGet( &PokeParaMine[i], ID_waza1 + j );
			btl->mons_no[no] = PokeParaGet( &PokeParaMine[i], ID_monsno );
			btl->item[no]    = PokeParaGet( &PokeParaMine[i], ID_item );
			btl->level[no]   = PokeParaGet( &PokeParaMine[i], ID_level );
			btl->p_rnd[no]   = PokeParaGet( &PokeParaMine[i], ID_personal_rnd );
			btl->exp[no]     = GetPokeExpAverage( &PokeParaMine[i] );
			no++;
		}

	}
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

//-----------------------
//		���z������
//-----------------------
// ��n���������Ƃ�
void BaseMove(void)
{
	u16	org_count;

	org_count = Fld.SecretBase[0].org_count;
	InitBase(0);
	Fld.SecretBase[0].org_count = org_count;
	BaseOut();
}
// ���z����I�񂾂Ƃ�
void BaseMove2(void)
{
	IncRecord( CNT_AGITTRANS );
	BaseMove();
}


//--------------------------------------------------
//	�ʂ̏ꏊ�Ɋ�n���쐬����Ƃ��̈��z������
//--------------------------------------------------
// ���}�b�v���̃Z������ւ�
static void MyBaseNewMakeMoveMapChangeCell(void)
{
	u16	i,j;
	s16	cell;

	const SXY_DATA * bg = NowMap.Sxy;

	for( i=0; i<bg->evBgMax; i++){
		if( bg->BgData[i].type == BG_BASE ){

			if( Fld.SecretBase[0].pos == (u32)bg->BgData[i].script ){
				cell = GetCellCode(	MAP_LEFT_MARGIN + bg->BgData[i].posX,
									MAP_UP_MARGIN + bg->BgData[i].posY	);

				for( j=0; j<7; j++ ){
					if( base_exit[j].after == cell ){
						SetCell( MAP_LEFT_MARGIN + bg->BgData[i].posX,
								 MAP_UP_MARGIN + bg->BgData[i].posY,
								 (base_exit[j].before | CELL_HITCHECK_MASK) );
						break;
					}
				}

				MakeMapBG();
				break;
			}
		}
	}
}

// ���C��
void MyBaseNewMakeMove(void)
{
	u16	org_count;

	MyBaseNewMakeMoveMapChangeCell();

	IncRecord( CNT_AGITTRANS );

	org_count = Fld.SecretBase[0].org_count;
	InitBase(0);
	Fld.SecretBase[0].org_count = org_count;
}

//----------------------------
//		�o�^�󋵂𒲂ׂ�
//----------------------------
static u8 CheckEntryNo( void )
{
	u8	no = 0;
	s16	i;

	for( i=1; i<SECRET_BASE_MAX; i++ ){
		if( CheckBaseFlag( (u8)i ) == TRUE )
			no++;
	}
	return	no;
}

void CheckBaseEntry( void )
{
	if( CheckBaseFlag( GetBasePos( BaseNo ) ) == TRUE ){
		AnswerWork = 1;			//�o�^����Ă���
	}else{
		if( CheckEntryNo() >= BASE_ENTRY_MAX )
			AnswerWork = 2;		//�o�^MAX
		else
			AnswerWork = 0;		//�o�^�\
	}
}

//-------------------------------
//		�o�^������/��������
//-------------------------------
void ChangeBaseEntry( void )
{
	Fld.SecretBase[ GetBasePos( BaseNo ) ].flg ^= 1;
	EventFlagSet( FE_BASE_ENTRY );
}

//-----------------------
//		���悤����
//-----------------------
void BaseRearrangeCheck(void)
{
	AddTask( RearrangeTaskInit, 0 );
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

//----------------------
//		�o�^���X�g
//----------------------
void BaseEntryListInit(void)
{
	AddTask( EntryListTaskInit, 0 );
}

//---------------------------
//		�o�^���X�g�쐬
//---------------------------
static void EntryListTaskInit( u8 id )
{
	s16 * wk;

	SetForceEvent();
	ScrActInit();
	AddMarkPal();
	
	wk = TaskTable[id].work;

	wk[ WK_ENTRY_MAX ] = (s16)CheckEntryNo();

	if( wk[ WK_ENTRY_MAX ] != 0 ){
		if( wk[ WK_ENTRY_MAX ] > 7 )	wk[ WK_ENTRY_POSMAX ] = 7;
		else	wk[ WK_ENTRY_POSMAX ] = wk[ WK_ENTRY_MAX ];
		wk[ WK_ENTRY_POS ] = 0;
		wk[ WK_ENTRY_SCR ] = 0;
		NMenuBoxClear( 0, 0, 29, 19 );
		DrawEntryListWin(id);
		TaskTable[id].TaskAdrs = EntryListTaskMain;
	}else{
		SetTalkMsg( id, msg_base_no_entry_list, EntryListEnd, 0 );
	}
}

//----------------------------
//		�o�^���X�g�\��
//----------------------------
static void	DrawEntryList( u8 id )
{
	s16 * wk = TaskTable[id].work;
	u8	start = 0;
	u8	cnt = 0;
	u8	i;

	for( i=1; i<SECRET_BASE_MAX; i++ ){
		if( start == wk[ WK_ENTRY_SCR ] ){
			start = i;
			break;
		}
		if( CheckBaseFlag(i) == TRUE )	start++;
	}

	for( i=start; i<SECRET_BASE_MAX; i++ ){
		if( CheckBaseFlag(i) == TRUE ){
			MakeBaseName2( StrTempBuffer0, i );
			NMenuBoxWrite( ENTRY_LIST_WINX+2, ENTRY_LIST_WINY+2+(cnt<<1), ENTRY_LIST_WINEX-1, ENTRY_LIST_WINY+3+(cnt<<1) );
			NMenuMsgWrite( StrTempBuffer0, ENTRY_LIST_WINX+2, ENTRY_LIST_WINY+2+(cnt<<1) );
			cnt++;
			if( cnt == 8 )	break;
		}
	}

	if( cnt < 8 ){
		NMenuBoxWrite( ENTRY_LIST_WINX+2, ENTRY_LIST_WINY+2+(cnt<<1), ENTRY_LIST_WINEX-1, ENTRY_LIST_WINY+3+(cnt<<1) );
		NMenuMsgWrite( str_modoru, ENTRY_LIST_WINX+2, ENTRY_LIST_WINY+2+(cnt<<1) );
		DelScrCurActor( MARK_ACT_D_CURSOR );
		if( cnt != 7 )
			NMenuBoxWrite( ENTRY_LIST_WINX+2, ENTRY_LIST_WINY+2+((cnt+1)<<1), ENTRY_LIST_WINEX-1, ENTRY_LIST_WINEY-1 );
	}else{
		AddScrCurActor( MARK_ACT_D_CURSOR, LIST_SCR_CUR_X, LIST_SCR_CUR_DY );
	}

	if( wk[ WK_ENTRY_SCR ] == 0 )
		DelScrCurActor( MARK_ACT_U_CURSOR );
	else
		AddScrCurActor( MARK_ACT_U_CURSOR, LIST_SCR_CUR_X, LIST_SCR_CUR_UY );

}

static void	DrawEntryListWin( u8 id )
{
	s16 * wk = TaskTable[id].work;

	NMenuWinBoxWrite( ENTRY_LIST_WINX, ENTRY_LIST_WINY, ENTRY_LIST_WINEX, ENTRY_LIST_WINEY );
	NMenuVCursorInit( ENTRY_LIST_WINX+1, ENTRY_LIST_WINY+2, wk[ WK_ENTRY_POSMAX ]+1, wk[ WK_ENTRY_POS ] );
	DrawEntryList( id );
}

//---------------------------
//		�o�^���X�g�I��
//---------------------------
static void EntryListTaskMain( u8 id )
{
	s16 * wk = TaskTable[id].work;

	if( sys.Repeat & U_KEY ){
		if( wk[ WK_ENTRY_POS ] != 0 ){
			SePlay( SE_SELECT );
			wk[ WK_ENTRY_POS ] = (s16)NMenuVCursorSet(-1);
		}else{
			if( wk[ WK_ENTRY_SCR ] != 0 ){
				SePlay( SE_SELECT );
				wk[ WK_ENTRY_SCR ]--;
				DrawEntryList(id);
			}
		}
		return;
	}

	if( sys.Repeat & D_KEY ){
		if( wk[ WK_ENTRY_POS ] == wk[ WK_ENTRY_POSMAX ] &&
			wk[ WK_ENTRY_SCR ] + wk[ WK_ENTRY_POS ] != wk[ WK_ENTRY_MAX ] )
		{
			SePlay( SE_SELECT );
			wk[ WK_ENTRY_SCR ]++;
			DrawEntryList(id);
		}else{
			if( wk[ WK_ENTRY_POS ] != wk[ WK_ENTRY_POSMAX ] ){
				SePlay( SE_SELECT );
				wk[ WK_ENTRY_POS ] = (s16)NMenuVCursorSet(1);
			}
		}
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		if( wk[ WK_ENTRY_POS ] + wk[ WK_ENTRY_SCR ] == wk[ WK_ENTRY_MAX ] ){
			NMenuBoxClear( 0, 0, 29, 19 );
			EntryListEnd(id);
		}else{
			wk[WK_RESET_NO] = (s16)EntryListSelectBaseNoGet( (u8)(wk[WK_ENTRY_POS]+wk[WK_ENTRY_SCR]) );
			EntryResetMenuInit(id);
		}
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		NMenuBoxClear( 0, 0, 29, 19 );
		EntryListEnd(id);
	}
}

//----------------------------------
//		�I�𒆂̊�n�ԍ����擾
//----------------------------------
static u8 EntryListSelectBaseNoGet( u8 pos )
{
	u8	i;
	u8	cnt = 0;

	for( i=1; i<SECRET_BASE_MAX; i++ ){
		if( CheckBaseFlag(i) == TRUE ){
			if( pos == cnt )	return	i;
			cnt++;
		}
	}
	return	0;
}

//-------------------------------
//		�����E�B���h�E�\��
//-------------------------------
static void EntryResetMenuInit( u8 id )
{
	ScrCurActAnmStopInit( MARK_ACT_U_CURSOR );
	ScrCurActAnmStopInit( MARK_ACT_D_CURSOR );
	NMenuWinBoxWrite( ENTRY_MENU_WINX, ENTRY_MENU_WINY, ENTRY_MENU_WINEX, ENTRY_MENU_WINEY );
	NMenuFixListWrite( ENTRY_MENU_WINX+2, ENTRY_MENU_WINY+1, 2, entry_list_menu );
	NMenuVCursorInit( ENTRY_MENU_WINX+1, ENTRY_MENU_WINY+1, 2, 0 );

	TaskTable[id].TaskAdrs = EntryResetMenuMain;
}

//-------------------------------
//		�����E�B���h�E�I��
//-------------------------------
static void EntryResetMenuMain( u8 id )
{
	MENUFUNC	func;

	if( sys.Repeat & U_KEY ){
		if( NMenuVCursorPosGet() != 0 ){
			SePlay( SE_SELECT );
			NMenuVCursorSet(-1);
		}
		return;
	}

	if( sys.Repeat & D_KEY ){
		if( NMenuVCursorPosGet() != 1 ){
			SePlay( SE_SELECT );
			NMenuVCursorSet(1);
		}
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		func = entry_list_menu[ NMenuVCursorPosGet() ].ptr;
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		ReturnEntrySelect(id);
	}
}



//----------------------------------------
//		"�Ƃ��낭�������債�܂����H"
//----------------------------------------
static void EntryResetInit( u8 id )
{
	s16 * wk = TaskTable[id].work;

	DelScrCurActor( MARK_ACT_U_CURSOR );
	DelScrCurActor( MARK_ACT_D_CURSOR );
	NMenuBoxClear( 0, 0, 29, 19 );
	MakeBaseName2( StrTempBuffer0, (u8)wk[ WK_RESET_NO ] );
	PM_MsgExpand( MsgExpandBuffer, msg_base_reset01 );
	SetTalkMsg( id, MsgExpandBuffer, EntryResetYesNoInit, 0 );
}

// �͂��E�������Z�b�g
static void EntryResetYesNoInit( u8 id )
{
	YesNoSelectInit( ENTRY_YESNO_WINX, ENTRY_YESNO_WINY );
	SelectToolInit( id, &EntryReset );
}

// �͂�
static void EntryResetMain( u8 id )
{
	s16 * wk = TaskTable[id].work;

	NMenuBoxClear( 0, 0, 29, 19 );

	Fld.SecretBase[ wk[ WK_RESET_NO ] ].flg = 0;

	wk[ WK_ENTRY_MAX ] -= 1;
	if( wk[ WK_ENTRY_SCR ] > 0 )	wk[ WK_ENTRY_SCR ] -= 1;
	if( wk[ WK_ENTRY_MAX ] < 8 )	wk[ WK_ENTRY_POSMAX ] -= 1;

	DrawEntryListWin(id);
	TaskTable[id].TaskAdrs = EntryListTaskMain;
}
static void EntryResetYes( u8 id )
{
	NMenuBoxClear( ENTRY_YESNO_WINX, ENTRY_YESNO_WINY, ENTRY_YESNO_WINEX, ENTRY_YESNO_WINEY );
	SetTalkMsg( id, msg_base_reset02, EntryResetMain, 0 );
}

// ������
static void EntryResetNo( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	DrawEntryListWin(id);
	TaskTable[id].TaskAdrs = EntryListTaskMain;
};

//----------------------------------
//		�o�^���X�g�I���ɂ��ǂ�
//----------------------------------
static void ReturnEntrySelect( u8 id )
{
	s16 * wk = TaskTable[id].work;

	NMenuVCursorInit( ENTRY_LIST_WINX+1, ENTRY_LIST_WINY+2, wk[ WK_ENTRY_POSMAX ]+1, wk[ WK_ENTRY_POS ] );
	NMenuBoxClear( ENTRY_MENU_WINX, ENTRY_MENU_WINY, ENTRY_MENU_WINEX, ENTRY_MENU_WINEY );
	ScrCurActAnmPlay( MARK_ACT_U_CURSOR );
	ScrCurActAnmPlay( MARK_ACT_D_CURSOR );
	TaskTable[id].TaskAdrs = EntryListTaskMain;
}

//------------------------------
//		�o�^���X�g�����I��
//------------------------------
static void EntryListEnd( u8 id )
{
	u16	pos = GetEventWorkValue( WK_BASE_POSNO );

	DelMarkPal();
	DelScrCurActor( MARK_ACT_U_CURSOR );
	DelScrCurActor( MARK_ACT_D_CURSOR );

	if( pos == 0 )	StartFieldScript( return_mine_base_board );
	else			StartFieldScript( return_other_base_pc );

	DelTask(id);
}


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

//-------------------------------------
//	������ԃL������ID���擾
//-------------------------------------
u8 GetOrusubanCharID( u8 no )
{
	return	( Fld.SecretBase[no].id[0] % 5 ) + ( Fld.SecretBase[no].sex * 5 );
}

//---------------------------------------
//	������ԑΐ�̔s�k���b�Z�[�W���擾
//---------------------------------------
extern const VM_CODE msg_boy_01_p04_sp_base[];
extern const VM_CODE msg_boy_02_p04_sp_base[];
extern const VM_CODE msg_boy_03_p04_sp_base[];
extern const VM_CODE msg_boy_04_p04_sp_base[];
extern const VM_CODE msg_boy_05_p04_sp_base[];
extern const VM_CODE msg_girl_01_p04_sp_base[];
extern const VM_CODE msg_girl_02_p04_sp_base[];
extern const VM_CODE msg_girl_03_p04_sp_base[];
extern const VM_CODE msg_girl_04_p04_sp_base[];
extern const VM_CODE msg_girl_05_p04_sp_base[];

const u8 * GetOrusubanLoseMsg(void)
{
	u8	id;

	id = GetOrusubanCharID( GetEventWorkValue( WK_BASE_POSNO ) );

	if( id == 0 )	return	(const u8 *)msg_boy_01_p04_sp_base;
	if( id == 1 )	return	(const u8 *)msg_boy_02_p04_sp_base;
	if( id == 2 )	return	(const u8 *)msg_boy_03_p04_sp_base;
	if( id == 3 )	return	(const u8 *)msg_boy_04_p04_sp_base;
	if( id == 4 )	return	(const u8 *)msg_boy_05_p04_sp_base;
	if( id == 5 )	return	(const u8 *)msg_girl_01_p04_sp_base;
	if( id == 6 )	return	(const u8 *)msg_girl_02_p04_sp_base;
	if( id == 7 )	return	(const u8 *)msg_girl_03_p04_sp_base;
	if( id == 8 )	return	(const u8 *)msg_girl_04_p04_sp_base;
	return	(const u8 *)msg_girl_05_p04_sp_base;
}

//-------------------------------------
//		�_�~�[������ԃL�����Z�b�g
//-------------------------------------
void SetDummyOrusubanData( u8 no )
{
	u16	i;

	for( i=0; i<1; i++ ){
		Fld.SecretBase[no].battle.p_rnd[i]   = i+1;		// ������
		Fld.SecretBase[no].battle.waza[i*4]  = i+1;		// �킴
		Fld.SecretBase[no].battle.mons_no[i] = 277;		// �|�P�����i���o�[
		Fld.SecretBase[no].battle.item[i]    = i+1;		// �ǂ���
		Fld.SecretBase[no].battle.level[i]   = i+5;		// ���x��
		Fld.SecretBase[no].battle.exp[i]     = i*5;		// �w�͒l�̕���
	}
}

//-------------------------------
//		������ԃL�����ΐ�
//-------------------------------
void OrusubanBattle(void)
{
	u16	pos = GetEventWorkValue( WK_BASE_POSNO );

	FanPointCalc( FAN_BASE_TYPE );
	OrusubanTaisenSet( &Fld.SecretBase[pos] );
}

//--------------------------------------
//		������ԑΐ�t���O�Z�b�g
//--------------------------------------
void OrusubanBtlFlgSet(void)
{
	Fld.SecretBase[ GetEventWorkValue( WK_BASE_POSNO ) ].fight_flag = AnswerWork;
}

//------------------------
//		�ΐ�`�F�b�N
//------------------------
void OrusubanFlgCheck(void)
{
	u8	i;
	u16	pos;

	pos = GetEventWorkValue( WK_BASE_POSNO );

	if( EventFlagCheck( TMFLG_ORUSUBAN_BATTLE ) == 0 ){
		for( i=0; i<SECRET_BASE_MAX; i++ )
			Fld.SecretBase[i].fight_flag = 0;
		EventFlagSet( TMFLG_ORUSUBAN_BATTLE );
	}

	ScriptParameter0 = (u16)GetOrusubanCharID( pos );
	AnswerWork       = (u16)Fld.SecretBase[pos].fight_flag;
}


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
enum {
	SEQ_INIT = 0,
	SEQ_WAIT,
	SEQ_EFFECT,
};

// �^�X�N���[�N
#define	WK_SEQ		1
#define	WK_PX		2
#define	WK_PY		3
#define	WK_EF		4
#define	WK_FADE		5	// �^���Ã}�b�g�p


//----------------------------
//		�g���b�v�O�b�Y
//----------------------------
void BaseGoodsTrap( u8 id )
{
	u8	  atr;
	s16   px,py;
	s16 * wk = TaskTable[id].work;

	switch( wk[WK_SEQ] ){
	case SEQ_INIT:
		GetHeroCellPos( &wk[WK_PX], &wk[WK_PY] );
		wk[WK_SEQ] = SEQ_WAIT;
		break;
	case SEQ_WAIT:
		GetHeroCellPos( &px, &py );
		if( px == wk[WK_PX] && py == wk[WK_PY] )	break;

		wk[WK_PX] = px;
		wk[WK_PY] = py;

		atr = GetCellAttribute( px, py );

		// ���炫��}�b�g
		if( ATR_IsBaseDarkMatCheck( atr ) == TRUE ){
			BaseKiraKiraMat();
			break;
		}

		// �����D�E�D�c�q
		if( ATR_IsBreakGoodsCheck( atr ) == TRUE ){
			SetBreakGoodsEffect( GetCellCode( px, py ), px, py );
			break;
		}

		// ����Ԃ��h�A
		if( ATR_IsBaseBreakDoorCheck( atr ) == TRUE ){
			BaseBreakDoor( px, py );
			break;
		}

		// ���}�b�g
		if( ATR_IsBaseSoundMatCheck( atr ) == TRUE ){
			BaseSoundMat( GetCellCode( px, py ) );
			break;
		}

		break;

	case SEQ_EFFECT:
		if( FeCodeChk( (u8)wk[WK_EF] ) == 0 )
			wk[WK_SEQ] = SEQ_WAIT;
		break;
	}
}


//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
#define	SIO_BASE_MAX	20
#define	SIO_BASE		SECRET_BASE_DATA

//----------------------------------------------------
//	��n���쐬
//----------------------------------------------------
static void MakeBase( u8 p, SECRET_BASE_DATA * base )
{
	Fld.SecretBase[p] = *base;
	Fld.SecretBase[p].flg = 2;	// �㏑������Ȃ��悤�ɂ���
}

//----------------------------------------------------
//	�����l�̊�n��T��
//----------------------------------------------------
// ID
static u8 Sio_SameCheckID( SECRET_BASE_DATA * b1, SECRET_BASE_DATA * b2 )
{
	u8	i;

	for( i=0; i<4; i++ ){
		if( b1->id[i] != b2->id[i] )	return	FALSE;
	}

	return	TRUE;
}
// NAME
static u8 Sio_SameCheckName( SECRET_BASE_DATA * b1, SECRET_BASE_DATA * b2 )
{
	u8	i;

	for( i=0; i<PERSON_NAME_SIZE; i++ ){
		if( b1->name[i] == EOM_ && b2->name[i] == EOM_ )
			break;
		if( b1->name[i] != b2->name[i] )	return	FALSE;
	}

	return	TRUE;

}
// ID & NAME & SEX
static u8 Sio_SameCheckMain( SECRET_BASE_DATA * b1, SECRET_BASE_DATA * b2 )
{
	if( b1->sex != b2->sex )					return	FALSE;
	if( Sio_SameCheckID( b1, b2 ) == FALSE )	return	FALSE;
	if( Sio_SameCheckName( b1, b2 ) == FALSE )	return	FALSE;

	return	TRUE;
}

//----------------------------------------------------
//	���̏ꏊ�Ɋ�n�����邩���ׂ�
//----------------------------------------------------
static s16 CheckBasePos( u8 pos )
{
	s16	i;

	for( i=0; i<SECRET_BASE_MAX; i++ ){
		if( Fld.SecretBase[i].pos == pos )
			return	i;
	}

	return	-1;
}

//----------------------------------------------------
//	�󂫃X�y�[�X��T��
//----------------------------------------------------
static u8 CheckBaseSpace(void)
{
	s16	i;

	for( i=1; i<SECRET_BASE_MAX; i++ ){
		if( Fld.SecretBase[i].pos == 0 )
			return	i;
	}

	return	0;
}

//----------------------------------------------------
//	�㏑���ł���ꏊ��T��
//----------------------------------------------------
static u8 CheckBaseReset(void)
{
	s16	i;

	for( i=1; i<SECRET_BASE_MAX; i++ ){
		if( Fld.SecretBase[i].flg == 0 && Fld.SecretBase[i].entry_flag == 0 )
			return	i;
	}

	return	0;
}

//----------------------------------------------------
//	���C��
//----------------------------------------------------
static u8 SioBaseSet( SECRET_BASE_DATA * base )
{
	s16	p;

	if( base->pos == 0 )	return	0;	//��n���Ȃ��Ȃ�I��

	p = CheckBasePos( base->pos );		//���łɊ�n�����邩

	if( p == 0 )	return	0;			//�����̊�n�Ɠ����ꏊ�Ȃ�I��

	//���łɊ�n������
	if( p != -1 ){
		if( Fld.SecretBase[p].entry_flag == 1 )
			return	0;						//�o�^����Ă���Ȃ�I��
		else{
			if( Fld.SecretBase[p].flg != 2 || base->entry_flag == 1 ){
				MakeBase( (u8)p, base );		//�o�^����Ă��Ȃ��Ȃ�㏑�����ďI��
				return	(u8)p;
			}
		}
	}else{
		p = (s16)CheckBaseSpace();			//�󂫃X�y�[�X�𒲂ׂ�

		if( p != 0 ){
			MakeBase( (u8)p, base );		//�󂫃X�y�[�X������Γ����
			return	(u8)p;
		}else{
			p = (s16)CheckBaseReset();		//�㏑���ł���ꏊ�𒲂ׂ�

			if( p != 0 ){
				MakeBase( (u8)p, base );	//�㏑���ł���Ȃ�㏑��
				return	(u8)p;
			}
		}
	}

	return	0;
}

//----------------------------------------------------
//	��n���\�[�g
//	�����̊�n > �o�^��n > �Â���n > �V�K�̊�n
//----------------------------------------------------
static void BaseSortOld(void)
{
	u8	i, j;
	SECRET_BASE_DATA   dummy;
	SECRET_BASE_DATA * my_data;

	my_data = Fld.SecretBase;

	for( i=1; i<SIO_BASE_MAX-1; i++ ){
		for( j=i+1; j<SIO_BASE_MAX; j++ ){
			if( ( my_data[i].flg == 0 && my_data[j].flg == 1 ) ||
				( my_data[i].flg == 2 && my_data[j].flg != 2 ) )
			{
				dummy = my_data[i];
				my_data[i] = my_data[j];
				my_data[j] = dummy;
			}
		}
	}
}

// �{�l�̈ȊO�̊�n��z�u
static void SioAllBaseSetEntry( SIO_BASE * base, u8 flg )
{
	u16	i;

	for( i=1; i<SIO_BASE_MAX; i++ ){
		if( base[i].flg == flg )	SioBaseSet( &base[i] );
	}
}

//----------------------------------------------------
//	���葤�̎����̊�n���폜
//----------------------------------------------------
static u8 SioGetMyBaseCheck( SIO_BASE * b )
{
	u8	i;

	// ��n�Ȃ�
	if( b->pos == 0 )	return	FALSE;

	// ����
	if( b->sex != MyData.my_sex )	return	FALSE;

	// ID
	for( i=0; i<4; i++ ){
		if( b->id[i] != MyData.ID[i] )	return	FALSE;
	}

	// Name
	for( i=0; i<PERSON_NAME_SIZE; i++ ){
		if( b->name[i] == EOM_ && MyData.my_name[i] == EOM_ )
			break;
		if( b->name[i] != MyData.my_name[i] )	return	FALSE;
	}
	return	TRUE;
}

static void SioGetMyBaseDel( SIO_BASE * b1, SIO_BASE * b2, SIO_BASE * b3 )
{
	u8	i, del=0;

	for( i=0; i<SECRET_BASE_MAX; i++ ){
		if( ( del & 1 ) == 0 ){
			if( SioGetMyBaseCheck( &b1[i] ) == TRUE ){
				InitBaseAdrs( &b1[i] );
				del |= 1;
			}
		}
		if( ( del & 2 ) == 0 ){
			if( SioGetMyBaseCheck( &b2[i] ) == TRUE ){
				InitBaseAdrs( &b2[i] );
				del |= 2;
			}
		}
		if( ( del & 4 ) == 0 ){
			if( SioGetMyBaseCheck( &b3[i] ) == TRUE ){
				InitBaseAdrs( &b3[i] );
				del |= 4;
			}
		}
		if( del == 7 )	break;
	}
}

//----------------------------------------------------
//	������n���폜
//----------------------------------------------------
static u8 SameBaseDel( SIO_BASE * b1, SIO_BASE * b2, u8 flg )
{
	u8	i;

	for( i=0; i<SECRET_BASE_MAX; i++ ){
		if( b2[i].pos != 0 ){
			if( Sio_SameCheckMain( b1, &b2[i] ) == TRUE ){

				if( flg == 0 ){
					InitBaseAdrs( &b2[i] );
					return	FALSE;
				}

				// �I���W�i���J�E���g�`�F�b�N
				if( b1->org_count > b2[i].org_count ){
					InitBaseAdrs( &b2[i] );
					return	FALSE;
				}else{
					b2[i].entry_flag = b1->entry_flag;
					InitBaseAdrs( b1 );
					return	TRUE;
				}
			}
		}
	}

	return	FALSE;
}

static void SameBaseDelMain( SIO_BASE * b1, SIO_BASE * b2, SIO_BASE * b3, SIO_BASE * b4 )
{
	u8	i;

	for( i=1; i<SECRET_BASE_MAX; i++ ){
		if( b1[i].pos != 0 ){
			if( b1[i].flg == 1 )	b1[i].entry_flag = 1;
			if( SameBaseDel( &b1[i], b2, i ) == FALSE ){
				if( SameBaseDel( &b1[i], b3, i ) == FALSE )
					SameBaseDel( &b1[i], b4, i );
			}
		}
	}
	for( i=0; i<SECRET_BASE_MAX; i++ ){
		if( b2[i].pos != 0 ){
			b2[i].fight_flag = 0;
			if( SameBaseDel( &b2[i], b3, i ) == FALSE )
				SameBaseDel( &b2[i], b4, i );
		}
	}
	for( i=0; i<SECRET_BASE_MAX; i++ ){
		if( b3[i].pos != 0 ){
			b3[i].fight_flag = 0;
			SameBaseDel( &b3[i], b4, i );
		}
		if( b4[i].pos != 0 )	b4[i].fight_flag = 0;
	}
}

//-------------------------------------------------------
//	��n�������C������
//-------------------------------------------------------
static void SioAllBaseSetMain( SIO_BASE * b2, SIO_BASE * b3, SIO_BASE * b4 )
{
	SioGetMyBaseDel( b2, b3, b4 );					// �����Ă��������̊�n���폜
	SameBaseDelMain( Fld.SecretBase, b2, b3, b4 );	// ������n���폜

	SioBaseSet( b2 );				// ��������̊�n
	SioBaseSet( b3 );
	SioBaseSet( b4 );
	SioAllBaseSetEntry( b2, 1 );	// �������肪�o�^���Ă����n
	SioAllBaseSetEntry( b3, 1 );
	SioAllBaseSetEntry( b4, 1 );
	SioAllBaseSetEntry( b2, 0 );	// �������肪�o�^���Ă��Ȃ���n
	SioAllBaseSetEntry( b3, 0 );
	SioAllBaseSetEntry( b4, 0 );
}

void SioAllBaseSet( void * base, u32 size, u8 id )
{
	u16	i;

#ifdef	PM_DEBUG
	if( DebugMainFlag == 0 && EventFlagCheck( FE_HIMITSU_GET ) == FALSE )
		return;
#else
	// �ʏ�́u�Ђ݂̂�����v��������ĂȂ��ƌ����ł��Ȃ�
	if( EventFlagCheck( FE_HIMITSU_GET ) == FALSE )
		return;
#endif	PM_DEBUG


	// �q�����Ă��Ȃ��l�̃f�[�^�͍폜
	switch( GetConnectSioMenberNum() ){
	case 2:
		memset( (SIO_BASE *)(base+size*2), 0, size );
		memset( (SIO_BASE *)(base+size*3), 0, size );
		break;
	case 3:
		memset( (SIO_BASE *)(base+size*3), 0, size );
	}

	// ID�ɂ���ėD��x��ݒ�
	switch( id ){
	case 0:
		SioAllBaseSetMain(
			(SIO_BASE *)(base+size),
			(SIO_BASE *)(base+size*2),
			(SIO_BASE *)(base+size*3) );
		break;
	case 1:
		SioAllBaseSetMain(
			(SIO_BASE *)(base+size*2),
			(SIO_BASE *)(base+size*3),
			(SIO_BASE *)(base)	);
		break;
	case 2:
		SioAllBaseSetMain(
			(SIO_BASE *)(base+size*3),
			(SIO_BASE *)(base),
			(SIO_BASE *)(base+size) );
		break;
	case 3:
		SioAllBaseSetMain(
			(SIO_BASE *)(base),
			(SIO_BASE *)(base+size),
			(SIO_BASE *)(base+size*2) );
		break;
	}

	// �_�~�[�̃t���O��������1
	for( i=1; i<SECRET_BASE_MAX; i++ ){
		if( Fld.SecretBase[i].entry_flag == 1 ){
			Fld.SecretBase[i].flg = 1;
			Fld.SecretBase[i].entry_flag = 0;
		}
	}

	BaseSortOld();

	// �_�~�[�̃t���O��������2
	for( i=1; i<SECRET_BASE_MAX; i++ ){
		if( Fld.SecretBase[i].flg == 2 )
			Fld.SecretBase[i].flg = 0;
	}

	// �I���W�i���J�E���g
	if( Fld.SecretBase[0].org_count != 0xffff )
		Fld.SecretBase[0].org_count++;
}
