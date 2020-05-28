//=========================================================================
//
// �C�x���g�����`�F�b�N�V�X�e��
//	
//		2001.03.29 by tama
//
//=========================================================================

#include "common.h"
#include "field.h"
#include "map.h"		// �����\���̎Q�Ƃ̂���
#include "evobj.h"		// ���@�Q�Ƃ̂���
#include "fieldmap.h"
#include "script.h"
#include "fld_main.h"
#include "task.h"
#include "palanm.h"
#include "map_attr.h"
#include "jiki.h"
#include "fld_task.h"
#include "weather.h"
#include "fld_tool.h"
#include "mapparam.h"
#include "mul_play.h"
#include "mus_tool.h"
#include "pokescr.h"
#include "initgame.h"

#define EV_CHECK_H_GLOBAL
#include "ev_check.h"

#include "sxytype.h"
#include "vmstat.h"
#include "ev_flag.h"
#include "ev_sub.h"

#include "base.h"
#include "sysflag.h"
#include "syswork.h"
#include "scr_tool.h"
#include "record.h"
#include "pokelist.h"
#include "poketool.h"
#include "fld_menu.h"		/* StartFieldMenuEvent */

#include "sodateya.h"
#include "safari.h"
#include "ev_fight.h"

#include "ecnt_dat.h"

#include "..\script\saveflag.h"

//=========================================================================
//=========================================================================

enum{
	//�Ń_���[�W���󂯂����
	POISON_STEP_MAX		= 4,

	//�Ȃ��x�`�F�b�N�������
	FRIENDLY_STEP_MAX	= 128,

	//�퓬�㤃G���J�E���g�`�F�b�N���Ȃ�����
	WALK_LIMIT_MAX		= 4,
};

//--------------------------------------------------------------
//--------------------------------------------------------------
u8 TargetObjNo;


//--------------------------------------------------------------
//	���[�J���֐��v���g�^�C�v
//--------------------------------------------------------------
static const VM_CODE * EvTalkCheck( EV_POS * p, u8 attr, u8 site );
static const VM_CODE * EvBGTalkCheck( EV_POS * p, u8 attr, u8 site );
static const VM_CODE * EvObjTalkCheck( EV_POS * p, u8 attr, u8 site );
static const VM_CODE * EvAttrTalkCheck( EV_POS * p, u8 attr, u8 site );
static const VM_CODE * EvHidenAttrTalkCheck( const EV_POS * p, u8 attr, u8 site );

static u8 EventTalkCheck(EV_POS * p, u16 attr, u8 site);// �b�������̃C�x���g�N���`�F�b�N
static u8 EventMoveCheck(EV_POS * p, u16 attr, u8 site);// ������Ƃ̃C�x���g�N���`�F�b�N
static u8 EventStepCheck(u16 attr);						// ���or�U��������̃C�x���g�N���`�F�b�N
static u8 EventMatCheck(EV_POS * p, u16 attr, u8 site);	// �}�b�g�̏o������`�F�b�N
static u8 EventPushCheck( EV_POS * p, u16 attr, u8 site );	// �L�[�����ł̑O���`�F�b�N

static u8 PosEventCheck(EV_POS * p);
static u8 ExitCheck(EV_POS * p, u16 attr);
static u8 PosAttrCheck( u16 attr );
static u8 StepCountCheck( void );
static int EvFloatUpCheck(void);
static int EvDiveDownCheck(void);

static const VM_CODE * SXY_EventPosCheck(const MapTotalData * Map, u16 px, u16 py, u8 height);
static s8 SXY_EvExitCheck(MapTotalData * Map, u16 px, u16 py, u8 height);

static const SXY_BG * SXY_EventBGCheck(const MapTotalData * Map, u16 px, u16 py, u8 height);

static u16 GetHeroPosAttribute( void );
static void GetHeroFrontPosition( EV_POS * p );

//--------------------------------------------------------------
//	�X�N���v�g�A�h���X�Q��
//--------------------------------------------------------------
extern const VM_CODE test_script[];
extern const VM_CODE test_bg_script[];
extern const VM_CODE test_pos_script[];
extern const VM_CODE base_script2[];
extern const VM_CODE ev_tv_broadcast_start[];
extern const VM_CODE paso_script[];
extern const VM_CODE mine_base_board_init[];
extern const VM_CODE other_base_board[];
extern const VM_CODE naminori_use_msg[];
extern const VM_CODE takinobori_look[];
extern const VM_CODE takinobori_use[];
extern const VM_CODE diving_below_use[];
extern const VM_CODE diving_up_use[];
//extern const VM_CODE diving_up_err[];
extern const VM_CODE locked_door_script[];
extern const VM_CODE ev_roulette_p01_c103_r0401[];
extern const VM_CODE ev_taisen_score[];
extern const VM_CODE ev_safari_cube_table[];
extern const VM_CODE hide_item_script[];
extern const VM_CODE karakuri_gate_script[];
extern const VM_CODE town_map_script[];
extern const VM_CODE ev_sp_ship_demo_end[];
extern const VM_CODE snd_obj_goods_script[];
extern const VM_CODE tate_goods_script[];
extern const VM_CODE dash_note_script[];
extern const VM_CODE ev_fall_script[];

//BG�X�N���v�g
extern const VM_CODE ev_bookrack_01[];
extern const VM_CODE ev_bookrack_02[];
extern const VM_CODE ev_bookrack_03[];
extern const VM_CODE ev_pot_01[];
extern const VM_CODE ev_trashbox_01[];
extern const VM_CODE ev_glasscase_01[];
extern const VM_CODE ev_blueprint_01[];

extern const VM_CODE ev_pasokon1_01_p01_t101_r0102[];
extern const VM_CODE ev_pasokon1_01_p01_t101_r0202[];

//==================================================================================
//
//	�t�B�[���h�C�x���g�N���`�F�b�N�V�X�e��
//
//==================================================================================
#ifdef	PM_DEBUG
extern void AddDebugMenuTask(void);		// �f�o�b�O���j���[�N��
#endif
extern int CheckCnvButton(void);	//�֗��{�^���`�F�b�N

//-------------------------------------------------------------
//	���L�[���͂��`�F�b�N�A�C�x���g�������N�G�X�g�𐶐�����
//	�@�^�X�N��X�N���v�g�Ȃǂ̎��ۂ̋N���͂����Ȃ�Ȃ�
//-------------------------------------------------------------

//-------------------------------------------------------------
//-------------------------------------------------------------
void ClearRequest(EV_REQUEST * req)
{
		req->TalkCheck = FALSE;
		req->StepCheck = FALSE;
		req->MenuOpen = FALSE;
		req->CnvButton = FALSE;
		req->MatCheck = FALSE;
		req->PushCheck = FALSE;
		req->MoveCheck = FALSE;
		req->FloatCheck = FALSE;
		req->DebugMapChange = FALSE;
		req->DebugHook = FALSE;
		req->DebugMenu = FALSE;
		req->DebugDiving = FALSE;

		req->Site = SITE_NOTHING;
}


//-------------------------------------------------------------
//-------------------------------------------------------------
void SetRequest( EV_REQUEST * req, u16 trg, u16 cont )
{
	u8 st = MyMoveState.mv_state;
	u8 val = MyMoveState.mv_val;
	u8 flag = ATR_IsForceMoveCheck(GetHeroPosAttribute());

	if ( (st==EV_M_END && flag==FALSE) || st==EV_M_OFF )
	{
		if ( GetHeroMoveSpeed() != HERO_MOVE_SPEED_MAX ) {
			if ( trg & START_BUTTON ) req->MenuOpen = TRUE;
			if ( trg & SELECT_BUTTON ) req->CnvButton = TRUE;
			if ( trg & A_BUTTON ) req->TalkCheck = TRUE;
			if ( trg & B_BUTTON ) req->FloatCheck = TRUE;
		}
		if ( cont & (U_KEY|D_KEY|L_KEY|R_KEY) )req->MatCheck = TRUE;
		if ( cont & (U_KEY|D_KEY|L_KEY|R_KEY) )req->PushCheck = TRUE;
	}

	if (flag == FALSE && st == EV_M_END && val == EV_M_WALK){
		req->MoveCheck = TRUE;
	}

	if (flag == FALSE && st == EV_M_END) {
		// ����ړ��I���^�C�~���O���H
		req->StepCheck = TRUE;
	}

	/* �ړ������̃��N�G�X�g */
	if ( cont & U_KEY ) { req->Site = SITE_UP; }
	else if ( cont & D_KEY ) { req->Site = SITE_DOWN; }
	else if ( cont & L_KEY ) { req->Site = SITE_LEFT; }
	else if ( cont & R_KEY ) { req->Site = SITE_RIGHT; }


#ifdef	PM_DEBUG

#ifdef	FESTA_VER_ROM
	if ( FestaModeFlag == TRUE && DebugMainFlag == 0 ) return;
#endif

	if (cont & R_BUTTON ) { 
		if ( req->MenuOpen == TRUE) {
			req->DebugMenu = TRUE;
			req->MenuOpen = FALSE;
		}
	}

	if ( DebugMainFlag == 0 ) return;
	/* �ȉ��̓f�o�b�O�@�\�̔����`�F�b�N */

	if (cont & R_BUTTON )
	{ 
		req->DebugHook = TRUE;

		req->MoveCheck = FALSE;
		req->StepCheck = FALSE;
		req->MatCheck = FALSE;
		req->PushCheck = FALSE;

		if ( trg & SELECT_BUTTON )
		{
			req->DebugMapChange = TRUE;
			req->CnvButton = FALSE;
		}
	}

	if ( cont & L_BUTTON )
	{
		req->DebugDiving = TRUE;
	}
#endif	PM_DEBUG
}


//==============================================================
//	�����N�G�X�g�����Ɂi�K�v�ȏꍇ�́j�`�F�b�N�v���O������
//	�@�Ăяo���A�C�x���g���N������
//
//==============================================================

//-------------------------------------------------------------
//-------------------------------------------------------------
int CheckRequest( EV_REQUEST * req )
{
	EV_POS	ev_pos;				// �C�x���g�̂��߂̍��W�ێ�
	u16 attr;
	u8 site;
	site = GetHeroSite();


	/* ��l���̐^���̈ʒu */
	GetHeroPosition(&ev_pos);
	attr = GetCellAttribute(ev_pos.x, ev_pos.y);


#ifdef PM_DEBUG
	if (req->DebugDiving && DivingMapMoveCheck(&ev_pos, attr) == TRUE) {
		return TRUE;
	}

	if (req->DebugHook == FALSE && EvTrainerSightCheck() == TRUE) return TRUE;
	if (req->DebugHook == FALSE && SceneEventCheck() == TRUE) return TRUE;
#else
	if (EvTrainerSightCheck() == TRUE) return TRUE;
	if (SceneEventCheck() == TRUE) return TRUE;
#endif


	if (req->FloatCheck && EvFloatUpCheck() == TRUE)
		return TRUE;


	if ( req->MoveCheck ) {
		IncRecord( CNT_WALK );
		if ( EventMoveCheck(&ev_pos, attr, site) == TRUE ) {
			return TRUE;
		}
	}

	if ( req->StepCheck && EventStepCheck(attr) == TRUE )
	{
		return TRUE;
	}

	if ( req->MatCheck && req->Site == site )
	{
		if ( EventMatCheck(&ev_pos, attr, site) == TRUE )return TRUE;
	}


	/* ��l���̑O�̈ʒu */
	GetHeroFrontPosition(&ev_pos);
	attr = GetCellAttribute(ev_pos.x, ev_pos.y);

	if ( req->TalkCheck && EventTalkCheck(&ev_pos, attr, site) == TRUE )
	{
		return TRUE;
	}

	if ( req->PushCheck && req->Site == site && EventPushCheck( &ev_pos, attr, site ) == TRUE )
	{
		return TRUE;
	}


	if (req->TalkCheck && EvDiveDownCheck() == TRUE)
		return TRUE;

	/* ��l���̈ʒu�����֌W�ȃC�x���g */
	if ( req->MenuOpen )
	{
		SePlay(SE_WIN_OPEN);
		StartFieldMenuEvent();
		return TRUE;
	}

	if ( req->CnvButton && CheckCnvButton() == TRUE )
	{
		return TRUE;
	}
	
#ifdef	PM_DEBUG
	if ( req->DebugMapChange )
	{
			AddDebugMapChangeTask();
			return TRUE;	
	}

	if ( req->DebugMenu )
	{
		SePlay(SE_WIN_OPEN);
		AddDebugMenuTask();
		return TRUE;
	}
#endif	PM_DEBUG


	return FALSE;
}

//=========================================================================
//	���@�̍��W���擾����
//=========================================================================
void GetHeroPosition( EV_POS * p )
{
	GetHeroCellPos( &(p->x), &(p->y) );
	p->z = GetHeroHeight();
}

static void GetHeroFrontPosition( EV_POS * p )
{
	s16 x,y;
	GetHeroFrontCellPos( &(p->x), &(p->y) );
	GetHeroCellPos( &x, &y );
	if ( GetCellHeight(x,y) == 0 ) {
		//�����Ă���ʒu�̃Z���̍�����0�̏ꍇ�A�������O�ɂ���
		p->z = 0;
	} else {
		p->z = GetHeroHeight();
	}
}

static u16 GetHeroPosAttribute( void )
{
	s16 x,y;
	GetHeroCellPos( &x, &y );
	return GetCellAttribute( x, y );
}


//=========================================================================
//
// �b�������ɂ��C�x���g�����`�F�b�N
//
//=========================================================================


//-------------------------------------------------------------
//	�C�x���g�����`�F�b�N�F�b������
//-------------------------------------------------------------

u8 EventTalkCheck(EV_POS * p, u16 attr, u8 site)
{
	const VM_CODE * ret;

	ret = EvTalkCheck(p,attr,site);
	if (ret == NULL) return FALSE;

	//�p�\�R���̎���SE�Ȃ�
	if ( ret != ev_pasokon1_01_p01_t101_r0102 &&
			ret != ev_pasokon1_01_p01_t101_r0202 &&
			ret != mine_base_board_init &&
			ret != other_base_board &&
			ret != paso_script) {
		SePlay(SE_SELECT);
	}

	StartFieldScript(ret);
	return TRUE;
}


//--------------------------------------------------------------
//
//--------------------------------------------------------------
const VM_CODE * EvTalkCheck(EV_POS * p, u8 attr, u8 site)
{
	const VM_CODE * ret;

	if( (ret = EvObjTalkCheck(p, attr, site) ) != NULL ) return ret;
	if( (ret = EvBGTalkCheck(p, attr, site) ) != NULL ) return ret;
	if( (ret = EvAttrTalkCheck(p, attr, site) ) != NULL ) return ret;
	if( (ret = EvHidenAttrTalkCheck(p, attr, site) ) != NULL ) return ret;
	return NULL;
}


//--------------------------------------------------------------
//
//--------------------------------------------------------------
const VM_CODE * EvObjEasyTalkCheck( EV_POS * p, u8 attr, u8 site )
{
	const VM_CODE * event;
	u8	no;
	int i;

	if ( ATR_IsCounterCheck(GetCellAttribute(p->x,p->y)) == FALSE ) {
		no = EvObjHitCheck( p->x, p->y, p->z );
	} else {
		no = EvObjHitCheck( p->x+SiteVector[site].dx, p->y+SiteVector[site].dy, p->z );
	}
	if( no == EV_OBJ_MAX || EvObjId(no) == EV_OBJ_SPID ) return NULL;

	for ( i=0; i<COMM_PLAYER_MAX; i++ ) {
		if ( Player[i].sw == 1 && Player[i].evobjno == no ) {
			return NULL;
		}
	}

	TargetObjNo = no;
	TargetObjID = EvObjId(no);
	TalkSiteWork = site;
	event = EvObjNoEvLabelGet(no);
	return event;
}



const VM_CODE * EvObjTalkCheck(EV_POS * p, u8 attr, u8 site)
{
	const VM_CODE * event;
	u8	no;

	no = EvObjHitCheck( p->x, p->y, p->z );
	if ( no == EV_OBJ_MAX || EvObjId(no) == EV_OBJ_SPID ) {
		if ( ATR_IsCounterCheck(attr) != TRUE ) return NULL;
		no = EvObjHitCheck( p->x + SiteVector[site].dx,
				p->y + SiteVector[site].dy, p->z );
		if(no == EV_OBJ_MAX || EvObjId(no) == EV_OBJ_SPID ) return NULL;
	}

	TargetObjNo = no;
	TargetObjID = EvObjId(no);
	TalkSiteWork = site;

	event = EvObjNoEvLabelGet(no);					//�C�x���g���x���擾
	event = ExScriptObjCheck(TargetObjID,event);	//�O���X�N���v�g�`�F�b�N
	return event;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const VM_CODE * EvBGTalkCheck(EV_POS * p, u8 attr, u8 site)
{
	const SXY_BG * bg;

	bg = SXY_EventBGCheck(&NowMap, p->x - MAP_X_MARGIN, p->y - MAP_Y_MARGIN, p->z);
	if(bg == NULL )return NULL;

	if(bg->script == NULL )return test_bg_script;

	switch ( bg->type ) {
	case BG_ALL_SITE:
		break;

	case BG_D_SITE:
		if( site != SITE_UP )return NULL;
		break;
	case BG_U_SITE:
		if( site != SITE_DOWN )return NULL;
		break;
	case BG_L_SITE:
		if( site != SITE_RIGHT )return NULL;
		break;
	case BG_R_SITE:
		if( site != SITE_LEFT )return NULL;
		break;

	case BG_FLG_ON:
	case BG_FLG_OFF:
	case BG_ITEMPICK:
		//PARAM0 = �t���O�i���o�[
		//PARAM1 = �A�C�e���i���o�[
		ScriptParameter0 = 0xffff & ( (u32)bg->script >> 16 );
		ScriptParameter0 += FH_FLAG_START;
		ScriptParameter1 = 0xffff & (u32)bg->script;
		if( EventFlagCheck(ScriptParameter0) == TRUE )return NULL;

		return hide_item_script;

	case BG_BASE:
		if( site != SITE_UP )				return	NULL;
		ScriptParameter0 = 0xffff & (u32)bg->script;
		if( TalkBaseDoorCheck() == FALSE )	return	NULL;
		return base_script2;
	}
	return bg->script;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
static const VM_CODE * EvAttrTalkCheck( EV_POS * p, u8 attr, u8 site )
{
	if( ATR_IsTVCheck(attr,site) == TRUE )			return ev_tv_broadcast_start;
	if( ATR_IsPasoCheck(attr) == TRUE )				return paso_script;
	if( ATR_IsLockedDoorCheck(attr) == TRUE )		return locked_door_script;
	if( ATR_IsScoreBoardCheck(attr) == TRUE )		return ev_taisen_score;
	if( ATR_IsCubeTableCheck(attr) == TRUE )		return ev_safari_cube_table;
	if( ATR_IsKarakuriGateCheck(attr) == TRUE )		return karakuri_gate_script;
	if( ATR_IsTownMapCheck(attr) == TRUE )			return town_map_script;
	if( ATR_IsDashNoteCheck(attr) == TRUE )			return dash_note_script;

	//BG�X�N���v�g
	if( ATR_IsBookRack1Check(attr) == TRUE )		return ev_bookrack_01;
	if( ATR_IsBookRack2Check(attr) == TRUE )		return ev_bookrack_02;
	if( ATR_IsBookRack3Check(attr) == TRUE )		return ev_bookrack_03;
	if( ATR_IsPotCheck(attr) == TRUE )				return ev_pot_01;
	if( ATR_IsTrashBoxCheck(attr) == TRUE )			return ev_trashbox_01;
	if( ATR_IsGlassCaseCheck(attr) == TRUE )		return ev_glasscase_01;
	if( ATR_IsBluePrintCheck(attr) == TRUE )		return ev_blueprint_01;
	
	
	// �������`�F�b�N����
	if( p->z == GetCellHeight( p->x, p->y ) ){
		if( ATR_IsBaseSecretBoxCheck(attr) == TRUE )	return mine_base_board_init;
		if( ATR_IsBaseBoardCheck(attr) == TRUE )		return other_base_board;
		if( ATR_IsBaseSndObjCheck(attr) == TRUE ) 		return snd_obj_goods_script;
		if( ATR_IsBaseTateCheck(attr) == TRUE ) 		return tate_goods_script;
	}

	return NULL;
}

static const VM_CODE * EvHidenAttrTalkCheck( const EV_POS * p, u8 attr, u8 site )
{
	//�Ȃ݂̂�̃`�F�b�N
	if( EventFlagCheck(SYS_BATCH05_GET) == TRUE &&
		CheckHeroNaminoriStart() == TRUE &&
		CheckHeroAttrNaminoriStart() == TRUE )		return naminori_use_msg;

	//�����̂ڂ�̃`�F�b�N
	if( ATR_IsWaterFallCheck(attr) == TRUE )
	{
		if( EventFlagCheck(SYS_BATCH08_GET) == TRUE && CheckHeroTakinoboriStart() == TRUE ) {
			return takinobori_use;
		} else {
			return takinobori_look;
		}
	}

#if 0
	//�_�C�r���O�̃`�F�b�N
	if( EventFlagCheck(SYS_BATCH07_GET) == TRUE )
	{
		if( NowMap.type == MAPTYPE_WATER && ATR_DisableFloatCheck(attr) )
		{
			return diving_up_err;
		}

		site = DivingMapCheck();
		if( site == 1 ) return diving_up_use;
		if( site == 2 ) return diving_below_use;
	}
#endif

	return NULL;
}


//-------------------------------------------------------------
//�_�C�r���O�E�����`�F�b�N
//-------------------------------------------------------------
static int EvDiveDownCheck(void)
{
	if (EventFlagCheck(SYS_BATCH07_GET) == FALSE)
		return FALSE;

	if (DivingMapCheck() != 2)
		return FALSE;

	StartFieldScript(diving_below_use);
	return TRUE;
}

//-------------------------------------------------------------
//�_�C�r���O�E����`�F�b�N
//-------------------------------------------------------------
static int EvFloatUpCheck(void)
{
	if (EventFlagCheck(SYS_BATCH07_GET) == FALSE)
		return FALSE;

	if (NowMap.type != MAPTYPE_WATER)
		return FALSE;

	if (DivingMapCheck() != 1)
		return FALSE;

	StartFieldScript(diving_up_use);
	return TRUE;
}

//=========================================================================
//
// ����ړ����Ƃ̃C�x���g�����`�F�b�N
//
//=========================================================================

static void FriendlyStepCountUp( void );
static u8 PoisonStepCheck( void );

//-------------------------------------------------------------
//	�C�x���g�����`�F�b�N�F ����ړ�
//-------------------------------------------------------------

u8 EventMoveCheck( EV_POS * p, u16 attr, u8 site)
{
	if ( PosEventCheck(p) == TRUE ) return TRUE;		// ���W�����C�x���g�`�F�b�N
	if ( ExitCheck( p, attr ) == TRUE ) return TRUE;	// �o������`�F�b�N
	if ( PosAttrCheck(attr) == TRUE ) return TRUE;		// �A�g���r���[�g�����`�F�b�N
	if ( StepCountCheck() == TRUE ) return TRUE;		// �����J�E���g�֘A�`�F�b�N
	if ( SprayCheck() == TRUE )	return	TRUE;			// �X�v���[�����`�F�b�N

	return FALSE;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
u8 PosEventCheck(EV_POS * p)
{
	const VM_CODE * ret;

	ret = SXY_EventPosCheck(&NowMap, p->x - MAP_X_MARGIN, p->y - MAP_Y_MARGIN, p->z);
	if ( ret == NULL ) return FALSE;
	StartFieldScript( ret );
	return TRUE;
}

//-------------------------------------------------------------
//	����ړ��ɂ��C�x���g�`�F�b�N�F�A�g���r���[�g
//-------------------------------------------------------------
extern const VM_CODE ev_pitfall_script[];
u8 PosAttrCheck( u16 attr )
{
	if ( ATR_IsBrokenFloorCheck(attr) ) {
		StartFieldScript( ev_pitfall_script );
		return TRUE;
	}
	return FALSE;
}

//-------------------------------------------------------------
//	����ړ��ɂ��C�x���g�`�F�b�N�F�����J�E���g�֘A
//-------------------------------------------------------------
extern const VM_CODE ev_sodateya_tamago_huka[];
extern const VM_CODE ev_poison_dead[];

u8 StepCountCheck( void )
{

	RevengeStepCountUp();						//�Đ�g���[�i�[�����`�F�b�N
	FriendlyStepCountUp();						//�Ȃ��x�A�b�v�����`�F�b�N

	if (PoisonStepCheck() == TRUE) {			//�łɂ��_���[�W�E�m���`�F�b�N
		StartFieldScript( ev_poison_dead );
		return TRUE;
	}
	if (PokeSodateya()){							//��ĉ������E�^�}�S�z������
		IncRecord( CNT_TAMAGO );
		StartFieldScript( ev_sodateya_tamago_huka );		
		return TRUE;
	}
	if ( SafariStepCheck() == TRUE ) {			 // �T�t�@�����[�h�ł̕����`�F�b�N
		return TRUE;
	}
	if ( ShipStepCountCheck(1) == TRUE ) {		// �A�N�A���̃f���I���`�F�b�N
		StartFieldScript( ev_sp_ship_demo_end );
		return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------
//�ꂠ�邫�ł̂Ȃ��x�㏸�`�F�b�N
//-------------------------------------------------------------
//������
void FriendlyStepInit(void)
{
	SetEventWorkValue(WK_FRIENDLY_STEP_COUNT, 0);
}

//�`�F�b�N
static void FriendlyStepCountUp( void )
{
	u16 * step;
	int i;
	PokemonParam * pp;

	step = GetEventWorkAdrs(WK_FRIENDLY_STEP_COUNT);
	++ *step; 
	*step %= FRIENDLY_STEP_MAX;
	if (*step != 0) return;

	pp = PokeParaMine;
	for ( i=0; i<STOCK_POKE_MAX; i++ ) {
		FriendCalc( pp, FRIEND_TSUREARUKI );
		pp++;
	}
}

//-------------------------------------------------------------
//	�����ɂ��Ń_���[�W
//-------------------------------------------------------------
//������
void PoisonStepInit(void)
{
	SetEventWorkValue(WK_POISON_STEP_COUNT, 0);
}

//�`�F�b�N
static u8 PoisonStepCheck( void )
{
	u16 * poison_count;

	//�閧��n�ł͓ł̌��ʂ͂Ȃ�
	if (NowMap.type == MAPTYPE_BASE)
		return FALSE;

	poison_count = GetEventWorkAdrs(WK_POISON_STEP_COUNT);
	++ *poison_count;
	*poison_count %= POISON_STEP_MAX;
	if (*poison_count != 0)
		return FALSE;

	switch ( PoisonDeadCheck() ) {
	case 0:
		return FALSE;
	case 1:
		return FALSE;
	case 2:
		return TRUE;
	default:
		return FALSE;
	}
}


//=========================================================================
//	�C�x���g�����`�F�b�N�F ����ړ��A���̏�U�����
//=========================================================================
static u8	walk_cnt = 0;
static u16	old_encount_attr = 0;

//-------------------------------------------------------------
//	�G���J�E���g���䏉����
//-------------------------------------------------------------
void InitEncountStep(void)
{
	walk_cnt = 0;
}

//-------------------------------------------------------------
//	�G���J�E���g�C�x���g�`�F�b�N
//-------------------------------------------------------------
u8 EventStepCheck(u16 attr)
{
	if( walk_cnt < WALK_LIMIT_MAX ){
		walk_cnt++;
		old_encount_attr = attr;
		return	FALSE;
	}

	if( FieldEncount( attr, old_encount_attr ) == TRUE ) {
		walk_cnt = 0;
		old_encount_attr = attr;
		return TRUE;	// �G���J�E���g�`�F�b�N
	}

	old_encount_attr = attr;
	return FALSE;
}



//=========================================================================
//
// �o������`�F�b�N�֘A
//
//=========================================================================

static u8 MatExitConditionCheck(u16 attr, u8 site);
static u8 ExitConditionCheck(u16 attr);
static s8 ExitDataCheck( MapTotalData * Map, EV_POS * p );
static void ExitDataSet( MapTotalData * Map, s8 id, EV_POS * p);


//-------------------------------------------------------------
//	�C�x���g�����`�F�b�N�F �}�b�g�o�����
//-------------------------------------------------------------

u8 EventMatCheck(EV_POS * p, u16 attr, u8 site)
{
	s8 id;

	id = ExitDataCheck( &NowMap, p );

	if ( MatExitConditionCheck(attr,site) == TRUE && id != -1 ) {
		SetHeroLastStatus();
		ExitDataSet(&NowMap, id, p);
		AddMapChangeTask();
		return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------
//	 �o������̃`�F�b�N���f�[�^�Z�b�g
//-------------------------------------------------------------
static u8 ExitCheck(EV_POS * p, u16 attr)
{
		s8 id;

		id = ExitDataCheck( &NowMap, p );

		if (id != -1 && ExitConditionCheck(attr) == TRUE) {

			SetHeroLastStatus();
			ExitDataSet(&NowMap, id, p);
			
			if( ATR_IsEscalatorCheck(attr) == TRUE )
			{
				AddMapChangeEscalatorTask( attr );
				return TRUE;
			}
			
			if( ATR_IsGayserCheck(attr) == TRUE )
			{
				AddMapChangeGayserTask();
				return TRUE;
			}

			if( ATR_IsHidePitFallCheck(attr) == TRUE )
			{
				AddMapChangeHidePitFallTask();
				return TRUE;
			}

			if( ATR_IsWarpCheck(attr) == TRUE )
			{
				AddMapChangeWarpTask();
				return TRUE;
			}

			if( ATR_IsPitFallCheck(attr) == TRUE )
			{
				StartFieldScript(ev_fall_script);
			//	AddMapChangeFallTask();
				return TRUE;
			}

			AddMapChangeTask();
			return TRUE;
		}

		return FALSE;

}

//-------------------------------------------------------------
//	�h�A�E�K�i�̏o����������`�F�b�N
//-------------------------------------------------------------
u8 ExitConditionCheck(u16 attr)
{
	if (ATR_IsDoorCheck(attr) == TRUE) return TRUE;
	if (ATR_IsLadderCheck(attr) == TRUE) return TRUE;
	if (ATR_IsEscalatorCheck(attr) == TRUE) return TRUE;
	if (ATR_IsWallExitCheck(attr) == TRUE) return TRUE;
	if (ATR_IsGayserCheck(attr) == TRUE) return TRUE;
	if (ATR_IsHidePitFallCheck(attr) == TRUE) return TRUE;
	if (ATR_IsWarpCheck(attr) == TRUE) return TRUE;
	if (ATR_IsPitFallCheck(attr) == TRUE) return TRUE;

	return FALSE;
}

//-------------------------------------------------------------
//	�}�b�g�̏o����������`�F�b�N
//-------------------------------------------------------------
u8 MatExitConditionCheck(u16 attr, u8 site)
{
		switch( site ){

		case SITE_UP:
				return ATR_ExitMatUpEnableCheck(attr);

		case SITE_DOWN:
				return ATR_ExitMatDownEnableCheck(attr);

		case SITE_LEFT:
				return ATR_ExitMatLeftEnableCheck(attr);

		case SITE_RIGHT:
				return ATR_ExitMatRightEnableCheck(attr);
		}

		return FALSE;
}

//-------------------------------------------------------------
//	�o������f�[�^�����邩�ǂ����̃`�F�b�N
//-------------------------------------------------------------
s8 ExitDataCheck( MapTotalData * Map, EV_POS * p)
{
		return  SXY_EvExitCheck(Map, p->x - MAP_X_MARGIN, p->y - MAP_Y_MARGIN, p->z);
}

//-------------------------------------------------------------
//	�����N����o������f�[�^���Z�b�g����
//-------------------------------------------------------------

void ExitDataSet(MapTotalData * Map, s8 id, EV_POS * p)
{
	const SXY_CONNECT * exit;
	const MapTotalData * next;

	exit = &(NowMap.Sxy->ConnectData[id]);
	if( exit->mapID == SPFIELD_MAP )	/* ����ڑ��̏o������ɓ������ꍇ */
	{
		NextMapSetBySpID( exit->doorID );
		return;
	};

	NextMapSetByID( exit->divisionID, exit->mapID, exit->doorID );
	NextEscapePosSet(p->x, p->y);		//���Ȃʂ��J�ڐ�̐ݒ�

	next = GetTotalMapData( exit->divisionID, exit->mapID );
	if( next->Sxy->ConnectData[exit->doorID].mapID == SPFIELD_MAP )
	{
		/* �ڑ��悪����ڑ��o������̏ꍇ�A���������o������̏���o�^���� */
		SetSpecialMapID(
				next->Sxy->ConnectData[id].doorID,		// �ۑ���R�[�h
				Fld.MapID.div_id,						// �����h�c
				Fld.MapID.map_id,						// �������h�c
				id										// �o�����h�c
				);
	}

}

//=========================================================================
//
//	����ȏo������Ȃǂ̃`�F�b�N
//
//=========================================================================
u8 EventPushCheck( EV_POS * p, u16 attr, u8 site )
{
	/* �閧��n�̏o������`�F�b�N */
	if( site == SITE_UP && ATR_IsBaseDoorCheck(attr) == TRUE )
	{
		BaseDoorIn( p, NowMap.Sxy );
		return TRUE;
	};

	/* �h�A�̃`�F�b�N */
	if( site == SITE_UP && ATR_IsDoorCheck(attr) == TRUE )
	{
		s8 id;

		id = ExitDataCheck( &NowMap, p );

		if (id != -1 && ExitConditionCheck(attr) == TRUE) {
			SetHeroLastStatus();
			ExitDataSet(&NowMap, id, p);
			AddDoorMapChangeTask();
			return TRUE;
		}
		return FALSE;
	}

	return FALSE;
}


//=========================================================================
//
//	SXY�f�[�^�Q�Ɨp���[�`��
//
//=========================================================================
//-------------------------------------------------------------
//	SXY �o������f�[�^�`�F�b�N�V�X�e��
//-------------------------------------------------------------

static s8 SXY_EvExitCheck(MapTotalData * Map, u16 px, u16 py, u8 height)
{
		const SXY_CONNECT * exit = Map->Sxy->ConnectData;
		u8 max = Map->Sxy->ConnectMax;
		int i;

		for( i=0 ; i<max ; i++,exit++ )
		{
				if( exit->posX == px && exit->posY == py
						&& ( (exit->height == height) || (exit->height == 0) ) )
				{
						return i;
				}
		}
		return -1;
}

//-------------------------------------------------------------
//	SXY POS�C�x���g�`�F�b�N�V�X�e��
//-------------------------------------------------------------

static const VM_CODE * check_pos_event( const SXY_POS * dt )
{
	if ( dt == NULL ) return NULL;
	if ( dt->script == NULL ) {
		EventPosProgramCall(dt->work);
		return NULL;
	}
	if ( dt->work == 0 ) {
		DoSpecialScript( dt->script );
		return NULL;
	}
	if ( GetEventWorkValue(dt->work) != dt->number ) return NULL;

	return dt->script;
}

static const VM_CODE * SXY_EventPosCheck(const MapTotalData * Map, u16 px, u16 py, u8 height)
{
	int i,max;
	const SXY_POS * dt;
	const VM_CODE * script;

	dt = Map->Sxy->PosData;
	max = Map->Sxy->evPosMax;

	for ( i = 0; i<max; i++ ) {
		if(dt[i].posX == px && dt[i].posY == py
				&& ( (dt[i].height == height) || (dt[i].height == 0) ) )
		{
			script = check_pos_event( &dt[i] );
			if ( script != NULL ) return script;
		};
	};
	return NULL;
}

//-------------------------------------------------------------
//	�Ȉ�POS�C�x���g�`�F�b�N
//-------------------------------------------------------------
const VM_CODE * EventPosEasyCheck( const EV_POS * p )
{
	return SXY_EventPosCheck(&NowMap, p->x - MAP_X_MARGIN, p->y - MAP_Y_MARGIN, p->z);
}

//-------------------------------------------------------------
//	SXY BG�C�x���g�`�F�b�N�V�X�e��
//-------------------------------------------------------------

static const SXY_BG * SXY_EventBGCheck(const MapTotalData * Map, u16 px, u16 py, u8 height)
{
	u8 i,max;
	const SXY_BG * BgData;

	BgData = Map->Sxy->BgData;
	max = Map->Sxy->evBgMax;

	for(i = 0;i<max;i++)
	{
		if(BgData[i].posX == px && BgData[i].posY == py
				&& ( (BgData[i].height == height) || (BgData[i].height == 0) ))
		{
			return &BgData[i];
		}
	}
	return NULL;
}


//=========================================================================
//
//	�V�n�����̃t�B�[���h�ڑ�
//
//=========================================================================
u8 DivingMapMoveCheck( EV_POS * pos, u16 attr )
{
	if( NowMap.type == MAPTYPE_WATER && ATR_DisableFloatCheck(attr) == FALSE ){
		if( NextMapSetAbove( pos->x - MAP_X_MARGIN, pos->y - MAP_Y_MARGIN ) == FALSE )
			return FALSE;
		SetHeroLastStatus();
		AddMapChangeSeNotTask();
		SePlay(SE_W291);
		return TRUE;
	}else if( ATR_IsDeepWaterCheck( attr ) == TRUE ){
		if( NextMapSetBelow( pos->x - MAP_X_MARGIN, pos->y - MAP_Y_MARGIN ) == FALSE )
			return FALSE;
		SetHeroLastStatus();
		AddMapChangeSeNotTask();
		SePlay(SE_W291);
		return TRUE;
	}
	return FALSE;
}

//-------------------------------------------------------------
//	�޲��ݸ�����
//-------------------------------------------------------------
u8 DivingMapCheck( void )
{
	s16 x,y;
	u8 attr;

	GetHeroCellPos( &x, &y );
	attr = GetCellAttribute( x, y );

	if( NowMap.type == MAPTYPE_WATER && ATR_DisableFloatCheck(attr) == FALSE )
	{
		if( NextMapSetAbove(x-MAP_X_MARGIN, y-MAP_Y_MARGIN) == TRUE )
		{
			return( 1 );			// ����
		}
	}
	else if( ATR_IsDeepWaterCheck(attr) == TRUE )
	{
		if( NextMapSetBelow(x-MAP_X_MARGIN,y-MAP_Y_MARGIN) == TRUE )
		{
			return( 2 );			// ����
		}
	}

	return( 0 );					// �s��
}

//=========================================================================
//=========================================================================
//--------------------------------------------------------------
//	�����̑O�ɂ���n�a�i�̃`�F�b�N
//	���ǂ����g�p�̃`�F�b�N�ȂǂɎg��
//	���߂�l�͂��̂n�a�i�̃X�N���v�g�A�h���X�i���j
//	��������TargetObjNo���ݒ肳���
//--------------------------------------------------------------
const VM_CODE * FrontObjCheck(void)
{
	u8 site;

	u16 attr;
	EV_POS pos;

	site = GetHeroMoveSite();
	GetHeroFrontPosition( &pos );
	attr = GetCellAttribute( pos.x, pos.y );

	return EvObjTalkCheck( &pos, attr, site );
}

//-------------------------------------------------------------
//	�}�b�v�J�ڏ����̋����Z�b�g
//-------------------------------------------------------------
u8 ForceExitSet( void )
{
	s8 id;
	EV_POS pos;
	u16 attr;
	u8 site;

	site = GetHeroMoveSite();
	GetHeroPosition( &pos );
	attr = GetCellAttribute( pos.x, pos.y );

	id = ExitDataCheck( &NowMap, &pos );
	ExitDataSet(&NowMap, id, &pos);
	return 0;
}

//=========================================================================
//=========================================================================
