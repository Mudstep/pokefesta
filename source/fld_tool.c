//=========================================================================
//	
//	fld_tool.c
//
//	�t�B�[���h�p�c�[���v���O����
//=========================================================================

#include "common.h"

#include "field.h"

#define FLD_TOOL_H_GLOBAL
#include "fld_tool.h"

#include "fieldmap.h"
#include "mapchr.h"
//#include "weather.h"
#include "wea_ctrl.h"
#include "mapbg.h"
#include "bg_anm.h"
#include "mapname.h"
#include "ev_flag.h"
#include "ev_check.h"
#include "task.h"
#include "fld_task.h"
#include "sxytype.h"
#include "mapparam.h"
#include "warpdata.h"
#include "sysflag.h"
#include "script.h"
#include "map_attr.h"

#include "base.h"				//2001/10/05 by nakahiro
#include "tv.h"					//01/10/18 nohara
#include "ev_time.h"			// UpdateTimeEvent()
#include "ev_fight.h"			// SetRandomRevenger()
#include "jiki.h"				// MyMoveStateMvFlagCheck()
#include "scr_tool.h"			// CyclingRoadEventInit()
#include "mv_poke.h"			// 2001/06/28 by nakahiro
#include "ev_sub.h"
#include "weather.h"

#include "../evdata/maparray.h"



//==================================================================================
//	�萔��`
//==================================================================================



//==================================================================================
//	�ϐ���`
//==================================================================================

MapID BeforeMapID = {};
MapID NextMapID = {};
static MapID DivingMapID = {};
static MapID PitfallMapID = {};

static const MapID NULL_MAPID = {-1,-1,-1,-1,-1};




//==================================================================================
//
//	�}�b�v�f�[�^�ݒ�֘A
//		�}�b�v����S��
//
//==================================================================================

//-----------------------------------------------------------------------------
//	SXY�f�[�^�̃��[�h����
//-----------------------------------------------------------------------------
//-------------------------------------------------------------
//	SXY�f�[�^�����[�h����
//-------------------------------------------------------------
void SxyObjDataSet(void)
{
	CpuArrayClear( 0, Fld.SvSxyObj, 32 );
	CpuCopy( NowMap.Sxy->ObjData, Fld.SvSxyObj, sizeof(evObjData) * NowMap.Sxy->ObjMax, 32 );
}

//-------------------------------------------------------------
//	SXY�f�[�^�𕜊�������
//	���R���e�B�j���[���Ƀo�[�W�����ύX�ŕs����������Ȃ��悤��
//�@�@�A�h���X�Ȃǂ��܂ރf�[�^���㏑������
//-------------------------------------------------------------
void SxyObjDataRecover(void)
{
	int i;
	const evObjData * src = NowMap.Sxy->ObjData;
	evObjData * dst = Fld.SvSxyObj;

	for( i=0; i<SXY_OBJ_MAX; i++ )
	{
		dst[i].ev_label = src[i].ev_label;
	}

}

//-------------------------------------------------------------
//	SXY�̍��W�f�[�^������������
//-------------------------------------------------------------
void SxyObjDataPosChange( u8 id, s16 x, s16 y )
{
	int i;
	evObjData * sxy = Fld.SvSxyObj;

	for ( i=0; i<SXY_OBJ_MAX; i++ ) {
		if ( sxy[i].id == id ) {
			sxy[i].pos_x = x;
			sxy[i].pos_y = y;
			return;
		}
	}
}

//-------------------------------------------------------------
//	SXY�̈ړ�����������������
//-------------------------------------------------------------
void SxyObjDataSiteChange( u8 id, u8 site )
{
	int i;
	evObjData * sxy = Fld.SvSxyObj;

	for ( i=0; i<SXY_OBJ_MAX; i++ ) {
		if ( sxy[i].id == id ) {
			sxy[i].site= site;
			return;
		}
	}
}

//-----------------------------------------------------------------------------
//	�\���֘A�Ăяo��
//-----------------------------------------------------------------------------
//-------------------------------------------------------------
//	�}�b�v�J�ڎ��̃L�����f�[�^�Ȃǂ�������
//-------------------------------------------------------------
void InitNowMapVisual(void)
{
	InitMapBG();				// BG ������( ���W,��аVram ������ )
	TransMapCharData(NowMap.Screen);
	TransMapPaletteData(NowMap.Screen);
	MakeMapBG();						// ��аVram �W�J
	MapBGanimeInit();
}



//==================================================================================
//==================================================================================
//-------------------------------------------------------------
//-------------------------------------------------------------
extern const MapScreenData * const MapScreenTable[];

static const MapScreenData * get_nowmap_screen( void )
{
	u16 id = Fld.MapScreenNo;

	if ( id != 0 ) {
		return MapScreenTable[id-1];
	} else {
		return NULL;
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void update_now_map_id( void )
{
	BeforeMapID = Fld.MapID;
	Fld.MapID = NextMapID;
	DivingMapID = NULL_MAPID;
	PitfallMapID = NULL_MAPID;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static void map_id_set( MapID *id, s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y)
{
	id->div_id = div_id;
	id->map_id = map_id;
	id->exit_id = exit_id;
	id->x = x;
	id->y = y;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static int is_null_mapid( MapID *id )
{
	if ( id->div_id != -1 ) return FALSE;
	if ( id->map_id != -1 ) return FALSE;
	if ( id->exit_id != -1 ) return FALSE;
	if ( id->x != -1 ) return FALSE;
	if ( id->y != -1 ) return FALSE;
	return TRUE;
}

//-----------------------------------------------------------------------------
//	�}�b�v�f�[�^�ւ̃|�C���^�擾
//-----------------------------------------------------------------------------
const MapTotalData * GetTotalMapData(u16 div_id, u16 map_id)
{
	return (DivisionList[div_id])[map_id];
}

//-----------------------------------------------------------------------------
//	���̃}�b�v�f�[�^�ւ̃|�C���^�擾
//	��NextMapID���Z�b�g���ꂽ����̂ݗL���A�Ăяo���ۂɒ��ӁB
//-----------------------------------------------------------------------------
const MapTotalData * GetNextMapData( void )
{
	return GetTotalMapData( NextMapID.div_id, NextMapID.map_id );
}


//-----------------------------------------------------------------------------
//	�}�b�v�\���̂��Z�b�g(�}�b�v���������j
//-----------------------------------------------------------------------------
void SetNowMapData(void)
{
	NowMap = * GetTotalMapData(Fld.MapID.div_id, Fld.MapID.map_id);
	Fld.MapScreenNo = NowMap.screen_id;
	NowMap.Screen = get_nowmap_screen();
}

//-----------------------------------------------------------------------------
//	�}�b�v�\���̂��Z�b�g(�R���e�B�j���[���j
//-----------------------------------------------------------------------------
void RecoverNowMapData( void )
{
	NowMap = * GetTotalMapData(Fld.MapID.div_id, Fld.MapID.map_id);
	NowMap.Screen = get_nowmap_screen();
}

//-----------------------------------------------------------------------------
//	�o������h�c������W��ݒ�
//	�@���o������h�c������Ȃ�΂��ꂩ��f�[�^���Z�b�g
//	�@�����W�����̒l�Ȃ�΂��̂܂܃Z�b�g
//	�@�����W���ُ�Ȓl�Ȃ�}�b�v�̒����̈ʒu�ɃZ�b�g
//-----------------------------------------------------------------------------
static void set_now_position(void)
{
	if(Fld.MapID.exit_id >= 0 && Fld.MapID.exit_id < NowMap.Sxy->ConnectMax )
	{
		Fld.MapXpos = NowMap.Sxy->ConnectData[Fld.MapID.exit_id].posX;
		Fld.MapYpos = NowMap.Sxy->ConnectData[Fld.MapID.exit_id].posY;
	}
	else if( Fld.MapID.x >= 0 && Fld.MapID.y >= 0 )
	{
		Fld.MapXpos = Fld.MapID.x;
		Fld.MapYpos = Fld.MapID.y;
	}else{
		Fld.MapXpos = NowMap.Screen->Xsize / 2;
		Fld.MapYpos = NowMap.Screen->Ysize / 2;
	};
}

//-----------------------------------------------------------------------------
//	���̃}�b�v�ւ̑J�ڏ���
//-----------------------------------------------------------------------------
void SetNextWorldPosition(void)
{
	update_now_map_id();

	SetNowMapData();
	set_now_position();
}

//-------------------------------------------------------------
//	�J�ڐ���Z�b�g
//-------------------------------------------------------------
void NextMapSetDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y)
{
	map_id_set( &NextMapID, div_id, map_id, exit_id, x, y );
}

//-------------------------------------------------------------
//	�J�ڐ���Z�b�g�i�}�b�v�h�c�̂݁j
//-------------------------------------------------------------
void NextMapSetByID( s8 div_id, s8 map_id, s8 exit_id )
{
	NextMapSetDirect( div_id, map_id, exit_id, -1, -1 );
}

//-------------------------------------------------------------
// ����ڑ��w��̃Z�b�g�i�}�b�v�h�c�{���ݍ��W�j
//-------------------------------------------------------------
void SetSpecialMapID( u8 sp, s8 div_id, s8 map_id, s8 exit_id )
{
	map_id_set( &Fld.SpMapID, div_id, map_id, exit_id, Fld.MapXpos, Fld.MapYpos );
}

//-------------------------------------------------------------
// ����ڑ��w��̃Z�b�g
//-------------------------------------------------------------
void SetSpecialMapDirect( u8 sp, s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y )
{
	map_id_set( &Fld.SpMapID, div_id, map_id, exit_id, x, y );
}


//-------------------------------------------------------------
// �J�ڐ�����ڑ��w��ŃZ�b�g
//-------------------------------------------------------------
void NextMapSetBySpID( u8 sp_id )
{
	NextMapID = Fld.SpMapID;
}

//-------------------------------------------------------------
// �J�ڐ�����[�v�h�c�ŃZ�b�g
//-------------------------------------------------------------
void NextMapSetByWarpID( u8 id )
{
	const WARP_POS * wp;
	wp = GetWarpPosByNo( id );
	if ( wp == NULL ) return;
	NextMapSetDirect( wp->div_id, wp->map_id, NO_EXIT_ID, wp->x, wp->y );
}

//-------------------------------------------------------------
//	�J�ڐ���e���|�[�g�w��ŃZ�b�g
//-------------------------------------------------------------
void NextMapSetByTelMapID( void )
{
	NextMapID = Fld.TelMapID;
}

//-------------------------------------------------------------
//	�e���|�[�g�w������[�v�h�c�ŃZ�b�g
//-------------------------------------------------------------
void TeleportMapSetByWarpID( u8 id )
{
	const WARP_POS * wp;
	wp = GetWarpPosByNo( id );
	if ( wp == NULL ) return;

	map_id_set( &Fld.TelMapID, wp->div_id, wp->map_id, NO_EXIT_ID, wp->x, wp->y );
}

//-------------------------------------------------------------
//	NextMapID/Fld.MapID���炠�Ȃʂ��J�ڐ���Z�b�g
//	�����̑J�ڐ��ݒ肵������ɌĂԂ���
//-------------------------------------------------------------
void NextEscapePosSet(s16 x, s16 y)
{
//	EV_POS p;
	u8 now_type,next_type;
	now_type = GetNowMapType();
	next_type = GetMapTypeByID( NextMapID.div_id, NextMapID.map_id );

	if ( MTYPE_IsFieldCheck(now_type) == FALSE ) return;
	if ( MTYPE_IsFieldCheck(next_type) == TRUE ) return;

//	GetHeroPosition( &p );
	EscapeMapSetDirect(
			Fld.MapID.div_id, Fld.MapID.map_id, -1,
			x - MAP_X_MARGIN,
			y - MAP_Y_MARGIN + 1	/* ����� */
			);
}


//-------------------------------------------------------------
//	���Ȃʂ��w����Z�b�g
//-------------------------------------------------------------
void EscapeMapSetDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y)
{
	map_id_set( &Fld.EscMapID, div_id, map_id, exit_id, x, y );
}

//-------------------------------------------------------------
//	�J�ڐ�����Ȃʂ��w��ŃZ�b�g
//-------------------------------------------------------------
void NextMapSetByEscMapID( void )
{
	NextMapID = Fld.EscMapID;
}

//-------------------------------------------------------------
//	�_�C�r���O�J�ڐ�̃Z�b�g
//-------------------------------------------------------------
void SetDivingMapDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y )
{
	map_id_set( &DivingMapID, div_id, map_id, exit_id, x, y );
}

void NextMapSetByDivingID( void )
{
	NextMapID = DivingMapID;
}

//-------------------------------------------------------------
//	���Ƃ����ڑ���̃Z�b�g
//-------------------------------------------------------------
void SetPitfallMapDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y )
{
	map_id_set( &PitfallMapID, div_id, map_id, exit_id, x, y );
}

void NextMapSetByPitfall( s16 x, s16 y )
{
	if ( is_null_mapid(&PitfallMapID) == TRUE ) {
		NextMapID = BeforeMapID;
		return;
	}
	NextMapSetDirect( PitfallMapID.div_id, PitfallMapID.map_id, NO_EXIT_ID, x, y );
}

//-------------------------------------------------------------
//	�R���e�B�j���[�ł̓��ʃ}�b�v�J�ڊ֘A
//-------------------------------------------------------------
void NextMapSetByContinueMapID( void )
{
	NextMapID = Fld.ContinueMapID;
}

void SetContinueMapDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y )
{
	map_id_set( &Fld.ContinueMapID, div_id, map_id, exit_id, x, y );
}

void SetContinueMapByWarpID( u8 id )
{
	const WARP_POS * wp;
	wp = GetWarpPosByNo( id );
	if ( wp == NULL ) return;
	map_id_set( &Fld.ContinueMapID, wp->div_id, wp->map_id, NO_EXIT_ID, wp->x, wp->y );
}

void SetContinueMapBySpID( u8 id )
{
	Fld.ContinueMapID = Fld.SpMapID;
}

//==================================================================================
//
//	�V�n�����ւ̃����N�ɂ��J��
//
//==================================================================================
//-------------------------------------------------------------
//-------------------------------------------------------------
static const MLink * search_nextlink_by_site( u8 id )
{
	int i,count;
	const MLink * lnk;

	count = NowMap.Link->count;
	lnk = NowMap.Link->Links;
	
	if( lnk == NULL ) return NULL;
	for( i=0; i<count; i++, lnk++ ){
		if( lnk->direction == id ){
			return lnk;
		}
	}
	return NULL;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
static u8 set_nextlink_by_site( u8 id, u16 x, u16 y )
{
	const MLink * lnk;
	
	lnk = search_nextlink_by_site( id );
	if ( lnk != NULL ) {
		NextMapSetDirect( lnk->div_id, lnk->map_id, NO_EXIT_ID, x, y );
		return TRUE;
	}

	LinkControlScriptCheck();	//�_�C�r���O�ڑ��̕ύX�`�F�b�N
	if ( is_null_mapid(&DivingMapID) == FALSE ) {
		NextMapSetByDivingID();
		return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------
//	�V�i��j�����ւ̃}�b�v�J�ڃZ�b�g
//-------------------------------------------------------------
u8 NextMapSetAbove( u16 x, u16 y )
{
	return set_nextlink_by_site( SITE_ABOVE, x, y );
}

//-------------------------------------------------------------
//	�n�i���j�����ւ̃}�b�v�J�ڃZ�b�g
//-------------------------------------------------------------
u8 NextMapSetBelow( u16 x, u16 y )
{
	return set_nextlink_by_site( SITE_BELOW, x, y );
}


//==================================================================================
//
//	�}�b�v�J�ڂ̏���������
//
//==================================================================================
//-----------------------------------------------------------------------------
//	�t�B�[���h�ł̗אڃ}�b�v�ړ����̏���
//-----------------------------------------------------------------------------
void ChangeMapData(u8 div_id, u8 map_id)
{
	NextMapSetDirect( div_id, map_id, NO_EXIT_ID, -1, -1 );
	MapBGM_FadeNextSet();
	update_now_map_id();

	SetNowMapData();
	SxyObjDataSet();

	LocalEventFlagClear();
	CyclingRoadEventInit();
	InitEncountStep();
	SetRandomRevenger( div_id, map_id );
	UpdateTimeEvent();
	InitFieldWeatherValue();
	InitFieldVoice();
	InitBright();
	MapBGM_ClearSpecial();
	FlagControlScriptCheck();

	MakeNowVirtualMap();

	TransMapChar2ndData(NowMap.Screen);
	TransMapPalette2ndData(NowMap.Screen);
	{
		int i;
		for (i=MAP_PLTT_2ND_NO;i<MAP_PLTT_2ND_NO+MAP_PLTT_2ND_SIZE;i++)
			FieldWeather_AdjustBGPal(i);
	}
	MapBGanimeChange();

	MvPokeHeroPosSet();
	MvPokeMove();

	UpdateFieldWeatherSmooth();
	UpdateMapEffectControl();
	InitControlScriptCheck();
	DispMapName();
}

//-----------------------------------------------------------------------------
//	�}�b�v�J�ڎ��̃f�[�^������
//-----------------------------------------------------------------------------
void FieldInitData(int is_comm_mode)
{
	u8 is_field,is_indoor;
	SetNowMapData();
	SxyObjDataSet();

	is_field = MTYPE_IsFieldCheck(NowMap.type);
	is_indoor = MTYPE_IsIndoorCheck(NowMap.type);

	LocalEventFlagClear();				// �}�b�v������̃t���O��������
	CyclingRoadEventInit();
	InitEncountStep();
	SetRandomRevenger( Fld.MapID.div_id, Fld.MapID.map_id );
	if (is_comm_mode != TRUE) {
		UpdateTimeEvent();
	}
	InitFieldWeatherValue();			// �V��`�F���W
	InitFieldVoice();					// �����`�F���W
	if ( is_field ) EventFlagReset( SYS_USE_WAZA_FLASH );
	InitBright();
	MapBGM_ClearSpecial();
	FlagControlScriptCheck();

	MvPokeHeroPosSet();
	MvPokeMoveRnd();

	MakeNowVirtualMap();
	if (is_comm_mode != TRUE) {
		if( is_indoor ) ChangeTVCell(VMap.Xsize,VMap.Ysize);
		if( is_indoor ) SetBaseGoods(1);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




//==================================================================================
//
//	�}�b�v�o�����̎��@�̏�Ԑݒ�
//
//	SetHeroLastStatus()			�}�b�v�J�ڑO�̎��@�̌`�Ԃ�ۑ�
//		��
//	�}�b�v�J�ڏ���
//		��
//	GetHeroFirstStatus()		�o�����̏󋵂�ȑO�̌`�Ԃ���ŐV�̌`�Ԃ�����
//==================================================================================

static HERO_INIT_INFO last_info = {};


static u8 get_hero_first_form(HERO_INIT_INFO * last, u16 attr, u8 map_type);
static u8 get_hero_first_site(HERO_INIT_INFO * last, u8 new_form, u16 attr, u8 map_type);

//-------------------------------------------------------------
//	���@�`�ԏ���������
//-------------------------------------------------------------
void InitHeroLastStatus(void)
{
	last_info.site = SITE_DOWN;
	last_info.form = MY_MV_BIT_NORMAL;
}

//-------------------------------------------------------------
//	���݂̎��@�`�ԏ���ۑ�
//-------------------------------------------------------------
void SetHeroLastStatus( void )
{
	last_info.site = GetHeroSite();

	if (CheckHeroState(MY_MV_BIT_CYCLE_R)) {
		last_info.form = MY_MV_BIT_CYCLE_R;

	} else if (CheckHeroState(MY_MV_BIT_CYCLE_S)) {
		last_info.form = MY_MV_BIT_CYCLE_S;

	} else if (CheckHeroState(MY_MV_BIT_SWIM)) {
		last_info.form = MY_MV_BIT_SWIM;

	} else if (CheckHeroState(MY_MV_BIT_DIVING)) {
		last_info.form = MY_MV_BIT_DIVING;

	} else {
		last_info.form = MY_MV_BIT_NORMAL;
	}
}

//-------------------------------------------------------------
//�}�b�v�o�����̎��@��ԏ����擾
//	���}�b�v���W�n�̃��[�`���Ɉˑ����Ă���̂ł����̕ύX�ɂ͒��ӁI�I
//	���߂�l��const�ւ̃|�C���^�Ȃ̂͏���������h������
//-------------------------------------------------------------
const HERO_INIT_INFO * GetHeroFirstStatus(void)
{
	HERO_INIT_INFO new;
	u8 map_type;
	u16 attr;

	map_type = GetNowMapType();
	attr = GetFirstAttr();

	new.form = get_hero_first_form(&last_info, attr, map_type);
	new.site = get_hero_first_site(&last_info, new.form, attr, map_type);

	last_info = new;

	return &last_info;
}

//-------------------------------------------------------------
//	�}�b�v�o�����̌`�Ԃ��w��
//���w��R�[�h�̒�`��evobj.def���Q��
//-------------------------------------------------------------
static u8 get_hero_first_form(HERO_INIT_INFO * last, u16 attr, u8 map_type)
{
	/* �D�����[�h�Ńt�B�[���h�ɂ���ꍇ�͒ʏ�`�Ԃɂ��Ă��� */
	if (map_type != MAPTYPE_ROOM && EventFlagCheck(SYS_CRUISE_MODE)) {
			return MY_MV_BIT_NORMAL;
	}

	if (map_type == MAPTYPE_WATER) return MY_MV_BIT_DIVING;
	if (ATR_IsWaterCheck(attr) == TRUE) return MY_MV_BIT_SWIM;

	if (IsEnableCycleNowMap() == TRUE) {
		if (last->form == MY_MV_BIT_CYCLE_R) return MY_MV_BIT_CYCLE_R;
		if (last->form == MY_MV_BIT_CYCLE_S) return MY_MV_BIT_CYCLE_S;
	}
	return MY_MV_BIT_NORMAL;
}

//-------------------------------------------------------------
//
//	�}�b�v�o�����̌����̐���
//
//-------------------------------------------------------------
static u8 get_hero_first_site(HERO_INIT_INFO * last, u8 new_form, u16 attr, u8 map_type)
{
	if ( EventFlagCheck(SYS_CRUISE_MODE) && map_type == MAPTYPE_SEA ) {
		return SITE_RIGHT;
	}
	
	if (ATR_IsWallExitUpCheck(attr) == TRUE) return SITE_UP;
	if (ATR_IsWallExitCheck(attr) == TRUE) return SITE_DOWN;
	if (ATR_DoorEffectCheck(attr) == TRUE) return SITE_DOWN;

	if (ATR_ExitMatDownEnableCheck(attr) == TRUE) return SITE_UP;
	if (ATR_ExitMatUpEnableCheck(attr) == TRUE) return SITE_DOWN;
	if (ATR_ExitMatLeftEnableCheck(attr) == TRUE) return SITE_RIGHT;
	if (ATR_ExitMatRightEnableCheck(attr) == TRUE) return SITE_LEFT;

	/* ����������A���と�����̏ꍇ�͌�����ς��Ȃ� */
	if (last->form == MY_MV_BIT_DIVING && new_form == MY_MV_BIT_SWIM)
		return last->site;
	if (last->form == MY_MV_BIT_SWIM && new_form == MY_MV_BIT_DIVING)
		return last->site;

	/* �͂����̏�ł͌����������Ȃ� */
	if (ATR_IsLadderCheck(attr) == TRUE)
		return last->site;

	return SITE_DOWN;	/* default */
}

//-------------------------------------------------------------
//	�}�b�v�o�����̃A�g���r���[�g���擾����
//-------------------------------------------------------------
u16 GetFirstAttr( void )
{
	return GetCellAttribute( Fld.MapXpos+MAP_LEFT_MARGIN, Fld.MapYpos+MAP_UP_MARGIN );
}

//-------------------------------------------------------------
//	���]�Ԃɏ���}�b�v���ǂ����̃`�F�b�N
//-------------------------------------------------------------
int IsEnableCycleNowMap(void)
{
	//�T�C�N�����O���[�h�̃Q�[�g�͎��]�Ԃɏ�邱�Ƃ��ł���
	if (Fld.MapID.div_id == R110 &&
			(Fld.MapID.map_id == R110_R0201 || Fld.MapID.map_id == R110_R0301))
			return TRUE;

	if (NowMap.type == MAPTYPE_ROOM) return FALSE;
	if (NowMap.type == MAPTYPE_BASE) return FALSE;
	if (NowMap.type == MAPTYPE_WATER) return FALSE;
	//�C�ꓴ�A�ŉ��w�A���Ñ�|�P�������ڊo�߂�ꏊ�ł͎��]�Ԃɏ��Ȃ�
	if (Fld.MapID.div_id == CAVE &&
			Fld.MapID.map_id == CAVE_D1111) return FALSE;
	//�Ւd�̓��A�ŉ��w�����]�Ԃɏ��Ȃ�
	if (Fld.MapID.div_id == CAVE &&
			Fld.MapID.map_id == CAVE_D1206) return FALSE;
	return TRUE;
}

//==================================================================================
//
//	�t�B�[���h���E�����w��֘A
//
//==================================================================================

//-------------------------------------------------------------
//	���邳�̏�����
//-------------------------------------------------------------
void InitBright(void)
{
	if ( NowMap.lighting == 0 ) {
		//���邳�ő�i���E�����Ȃ��j
		Fld.Brightness = 0;
	} else if ( EventFlagCheck( SYS_USE_WAZA_FLASH ) ) {
		//���E��������Ńt���b�V�����g���Ă���ꍇ�͎��E�ő�
		Fld.Brightness = 1;
	} else {
		//���E��������Ńt���b�V�����g�p�Ȃ̂Ŏ��E�ŏ�
		Fld.Brightness = FIELD_BRIGHT_MAX;
	}
}

//-------------------------------------------------------------
//	���邳�̃Z�b�g
//-------------------------------------------------------------
void SetBright( int r )
{
	if ( r < 0 || r > FIELD_BRIGHT_MAX ) r = 0;
	Fld.Brightness = r;
}

//-------------------------------------------------------------
//	���邳�̎擾
//-------------------------------------------------------------
u8 GetBright(void)
{
	return Fld.Brightness;
}


//==================================================================================
//==================================================================================

//-------------------------------------------------------------
//	�}�b�v�X�N���[����ύX
//-------------------------------------------------------------
void ChangeNowMapScreen( u16 id )
{
	Fld.MapScreenNo = id;
	NowMap.Screen = get_nowmap_screen();
}

