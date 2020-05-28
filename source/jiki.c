//**********************************************************************
//		jiki.c
//		���@
//**********************************************************************
#define		JIKI_H_PROTO
#include	<agb.h>
#include	"common.h"
#include	"evobj.h"
#include	"map.h"
#include	"map_attr.h"
#include	"fieldmap.h"
#include	"pokesio.h"
#include	"jiki.h"
#include	"fld_tool.h"
#include	"multisio.h"
#include	"sysflag.h"
#include	"ev_flag.h"
#include	"task.h"
#include	"poketool.h"
#include	"wazano.def"
#include	"r_door.h"			//DoorPushCheck
#include	"ecnt_dat.h"
#include	"script.h"
#include	"menu.h"
#include	"calctool.h"
#include	"letter.h"
#include	"mus_tool.h"
#include	"record.h"

//----------------------------------------------------------------------
//		�O���Q��
//----------------------------------------------------------------------
extern u8 DebugMainFlag;
extern u8 TemotiNijyuuCheck(PokemonParam *pp, u16 wazano);
extern void FishEncountSet( u8 );
extern u8 FishingEncount2( void );

//----------------------------------------------------------------------
//		define
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//		����
//----------------------------------------------------------------------
static u8 HeroUnderControl( void );
static void HeroMoveNormal( u8, u16 );
static void ExitCursorControl( EventOBJ * );
static void SetHeroPushRockTask( u8, u8 );
static void SetHeroJumpingMatTask( void );
static void SetHeroTurnMatTask( void );
static void SetHeroSwimEndTask( u8 );

//======================================================================
//		���@����
//======================================================================
static u8 HeroMove( EventOBJ *, actWork * );

//------------------------------------------
//	���@����
//------------------------------------------
void EvObj_HeroMove( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, HeroMove );
}

//------------------------------------------
//	���@����
//------------------------------------------
static u8 HeroMove( EventOBJ *ev, actWork *act )
{
	return( 0 );
}

//======================================================================
//		���쐧��
//======================================================================
static u8 HeroMoveStartCheck( EventOBJ *, u8 );
static void HeroMoveStartInit( EventOBJ * );
static void HeroMoveSet( u8, u16, u16 );
static void HeroUnderFlagReset( void );

//------------------------------------------
//	���@���쐧��
//	In:		site		; �ړ�����
//			trg			; �����ضް
//			cont		; ���ݺ��۰װ
//------------------------------------------
void HeroMoveControl( u8 site, u16 trg, u16 cont )
{
	EventOBJ *ev;

	ev = &EvObj[MyMoveState.evobjno];

	ExitCursorControl( ev );

	if( MyMoveState.mv_pause )
	{
		return;
	}

	CycleCommandSet( trg, cont );

	if( HeroMoveStartCheck(ev,site) )
	{
		return;
	}

	HeroMoveStartInit( ev );
	HeroMoveRequest();

	if( HeroUnderControl() )
	{
		return;
	}

	HeroMoveSet( site, trg, cont );
	HeroUnderFlagReset();
}

//------------------------------------------
//	�ړ��J�n����
//------------------------------------------
static u8 HeroMoveStartCheck( EventOBJ *ev, u8 site )
{
	u8 code;

	if( AcmdSetChk(ev) )
	{
		if( AcmdEnd(ev) )
		{
			return( 0 );
		}

		code = AcmdCode( ev );

		if( code >= AC_STAY_WALK_D_05 && code <= AC_STAY_WALK_R_05 )
		{
			if( site != SITE_N && ev->mv_site != site )
			{
				AcmdFreeWork( ev );
				return( 0 );
			}
		}

		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�J�n������
//------------------------------------------
static void HeroMoveStartInit( EventOBJ *ev )
{
	ev->anm_change_sw = 0;								// �Œ��׸޸ر
	ev->anm_pause_sw = 0;
	ev->site_pause_sw = 0;
	MyMoveState.flag &= (~MY_MV_BIT_B_DASH);			// B�ޯ���׸޸ر
}

//------------------------------------------
//	����ݒ�
//------------------------------------------
static void HeroMoveSet( u8 site, u16 trg, u16 cont )
{
	if( MyMoveState.flag & MY_MV_BIT_CYCLE_AND )
	{
		HeroMoveCycle( site, trg, cont );				// ���]�Ԉړ���
	}
	else
	{
		HeroMoveNormal( site, cont );					// �ʏ�ړ���
	}
}

//------------------------------------------
//	�����׸�
//------------------------------------------
static void HeroUnderFlagReset( void )
{
	if( MyMoveState.mv_val == EV_M_WALK )				// �ړ�
	{
		MyMoveState.flag &= ~MY_MV_BIT_UNDER;			// �����������׸޸ر
	}
}

//======================================================================
//		��������
//======================================================================
#define	UNDER_ATR_CHK_MAX	18
static u8 HeroUnderCheck( void );
static u8 (* const HeroUnderCheckTbl[])( u8 );
static u8 (* const HeroUnderSetTbl[])( void );

//------------------------------------------
//	��������
//------------------------------------------
#ifndef PM_DEBUG
static u8 HeroUnderControl( void )
{
	return( HeroUnderSetTbl[HeroUnderCheck()]() );
}
#endif

//------------------------------------------
//	�������� ���ޯ��
//------------------------------------------
#ifdef PM_DEBUG
static u8 HeroUnderControl( void )
{
	if( DebugMainFlag && (sys.Cont & R_BUTTON) )
	{
		return( 0 );
	}

	return( HeroUnderSetTbl[HeroUnderCheck()]() );
}
#endif

//------------------------------------------
//	��������
//------------------------------------------
static u8 HeroUnderCheck( void )
{
	u8 i,attr;

	if( (MyMoveState.flag & MY_MV_BIT_UNDER) == 0 )		// ����������
	{
		attr = EvObj[MyMoveState.evobjno].now_attr;

		for( i = 0; i < UNDER_ATR_CHK_MAX; i++ )
		{
			if( HeroUnderCheckTbl[i](attr) ) return( i + 1 );
		}
	}

	return( 0 );
}

//------------------------------------------
//	���� ���ɖ���
//------------------------------------------
static u8 non_force_set( void )
{
	EventOBJ *ev;

	if( MyMoveState.flag & MY_MV_BIT_FORCE )			// �����ړ����|�����Ă���
	{
		ev = &EvObj[MyMoveState.evobjno];
		ev->site_pause_sw = 0;
		ev->anm_pause_clr_sw = 1;
		EvObjSiteSet( ev, ev->site );
		MyMoveState.flag &= ~MY_MV_BIT_FORCE;
	}

	return( 0 );
}

//------------------------------------------
//	���� �����ړ����
//------------------------------------------
static u8 force_move_set( u8 site, void (*setcmd)(u8) )
{
	u8 flag;
	EventOBJ *ev;

	ev = &EvObj[MyMoveState.evobjno];
	flag = HeroMoveCheckSite( site );

	MyMoveState.flag |= MY_MV_BIT_FORCE;

	if( flag )
	{
		non_force_set();
		if( flag < HEROHIT_START ) return( 0 );
		if( flag == HEROHIT_JUMP ) HeroAcmdJump( site );
		MyMoveState.flag |= MY_MV_BIT_FORCE;
		MyMoveState.mv_val = EV_M_WALK;
		return( 1 );
	}

	MyMoveState.mv_val = EV_M_WALK;
	setcmd( site );

	return( 1 );
}

//------------------------------------------
//	���� ���ؾ��
//------------------------------------------
static u8 force_slip_move_set( void (*setcmd)(u8) )
{
	EventOBJ *ev;
	
	ev = &EvObj[MyMoveState.evobjno];
	ev->anm_pause_sw = 1;

	return( force_move_set(ev->mv_site,setcmd) );
}

//------------------------------------------
//	���� ���� 2
//------------------------------------------
static u8 force_slip_2_set( void )
{
	return( force_slip_move_set(HeroAcmdWalk2) );
}

//------------------------------------------
//	���� �����ړ� 1 ��
//------------------------------------------
static u8 force_1_down_set( void )
{
	return( force_move_set(SITE_D,HeroAcmdWalk1) );
}

//------------------------------------------
//	���� �����ړ� 1 ��
//------------------------------------------
static u8 force_1_up_set( void )
{
	return( force_move_set(SITE_U,HeroAcmdWalk1) );
}

//------------------------------------------
//	���� �����ړ� 1 ��
//------------------------------------------
static u8 force_1_left_set( void )
{
	return( force_move_set(SITE_L,HeroAcmdWalk1) );
}

//------------------------------------------
//	���� �����ړ� 1 �E
//------------------------------------------
static u8 force_1_right_set( void )
{
	return( force_move_set(SITE_R,HeroAcmdWalk1) );
}

//------------------------------------------
//	���� �����ړ� 3 ��
//------------------------------------------
static u8 force_3_down_set( void )
{
	return( force_move_set(SITE_D,HeroAcmdWalk3) );
}

//------------------------------------------
//	���� �����ړ� 3 ��
//------------------------------------------
static u8 force_3_up_set( void )
{
	return( force_move_set(SITE_U,HeroAcmdWalk3) );
}

//------------------------------------------
//	���� �����ړ� 3 ��
//------------------------------------------
static u8 force_3_left_set( void )
{
	return( force_move_set(SITE_L,HeroAcmdWalk3) );
}

//------------------------------------------
//	���� �����ړ� 3 �E
//------------------------------------------
static u8 force_3_right_set( void )
{
	return( force_move_set(SITE_R,HeroAcmdWalk3) );
}

//------------------------------------------
//	���� �Œ苭�� ���
//------------------------------------------
static u8 force_fix_move_set( u8 site, void (*setcmd)(u8) )
{
	EventOBJ *ev;

	ev = &EvObj[MyMoveState.evobjno];
	ev->anm_pause_sw = 1;
	ev->site_pause_sw = 1;

	return( force_move_set(site,setcmd) );
}

//------------------------------------------
//	���� �Œ苭�� 2 ��
//------------------------------------------
static u8 force_fix_2_down_set( void )
{
	return( force_fix_move_set(SITE_D,HeroAcmdWalk2) );
}

//------------------------------------------
//	���� �Œ苭�� 2 ��
//------------------------------------------
static u8 force_fix_2_up_set( void )
{
	return( force_fix_move_set(SITE_U,HeroAcmdWalk2) );
}

//------------------------------------------
//	���� �Œ苭�� 2 ��
//------------------------------------------
static u8 force_fix_2_left_set( void )
{
	return( force_fix_move_set(SITE_L,HeroAcmdWalk2) );
}

//------------------------------------------
//	���� �Œ苭�� 2 �E
//------------------------------------------
static u8 force_fix_2_right_set( void )
{
	return( force_fix_move_set(SITE_R,HeroAcmdWalk2) );
}

//------------------------------------------
//	����	�ެ���ݸ�ϯ�
//------------------------------------------
static u8 force_jumping_mat_set( void )
{
	SetHeroJumpingMatTask();
	return( 1 );
}

//------------------------------------------
//	����	�������ϯ�
//------------------------------------------
static u8 force_turn_mat_set( void )
{
	SetHeroTurnMatTask();
	return( 1 );
}

//------------------------------------------
//	����	�����
//------------------------------------------
static u8 force_slip_slope_set( void )
{
	EventOBJ *ev;
	
	ev = &EvObj[MyMoveState.evobjno];

	if( ev->mv_site != SITE_U ||
		GetHeroMoveSpeed() < HERO_MOVE_SPEED_MAX )
	{
		// �������������̏������K�v�ɂȂ�ł��낤
		CycleSpeedBrakeSet( 0 );						// ��
		ev->site_pause_sw = 1;
		return( force_move_set(SITE_D,HeroAcmdWalk2) );
	}

	return( 0 );
}

//------------------------------------------
//	����ޭ������ð���
//------------------------------------------
static u8 (* const HeroUnderCheckTbl[])( u8 ) =
{
	ATR_IsSlipFloorRoomCheck,							// �د�� 2
	ATR_IsSlipFloorIceCheck,							// �د�� 2

	ATR_IsDownForceMoveCheck,							// ���� 1
	ATR_IsUpForceMoveCheck,
	ATR_IsLeftForceMoveCheck,
	ATR_IsRightForceMoveCheck,

	ATR_IsDownForceMoveCurrentCheck,					// ���� 2
	ATR_IsUpForceMoveCurrentCheck,
	ATR_IsLeftForceMoveCurrentCheck,
	ATR_IsRightForceMoveCurrentCheck,

	ATR_IsDownForcePushCheck,							// �����Œ苭��
	ATR_IsUpForcePushCheck,
	ATR_IsLeftForcePushCheck,
	ATR_IsRightForcePushCheck,

	ATR_IsWaterFallCheck,								// ��

	ATR_IsJumpingMatCheck,								// �ެ���ݸ�ϯ�
	ATR_IsTurnMatCheck,									// ���邭��ϯ�

	ATR_IsSlipSlopeCheck,								// �����
};

//------------------------------------------
//	�����ړ�����ð���
//------------------------------------------
static u8 (* const HeroUnderSetTbl[])(void) =
{
	non_force_set,										// ��������

	force_slip_2_set,									// ���� 2
	force_slip_2_set,									// ���� 2

	force_1_down_set,									// �����ړ� 1
	force_1_up_set,
	force_1_left_set,
	force_1_right_set,

	force_3_down_set,									// �����ړ� 2
	force_3_up_set,
	force_3_left_set,
	force_3_right_set,

	force_fix_2_down_set,								// �Œ苭�� 2
	force_fix_2_up_set,
	force_fix_2_left_set,
	force_fix_2_right_set,

	force_3_down_set,									// �� ����������

	force_jumping_mat_set,								// �ެ���ݸ�ϯ�
	force_turn_mat_set,									// ���邭��ϯ�
	
	force_slip_slope_set,								// �����
};

//======================================================================
//		�ʏ�ړ�
//======================================================================
static u8 NormalMoveValue( u8 );
static void normal_move_stop( u8, u16 );
static void normal_move_turn( u8, u16 );
static void normal_move_on( u8, u16 );

//------------------------------------------
//	����ð���
//------------------------------------------
static void (* const NormalMoveTbl[])( u8, u16 ) =
{
	normal_move_stop,
	normal_move_turn,
	normal_move_on,
};

enum
{
	NORMAL_MOVE_STOP,
	NORMAL_MOVE_TURN,
	NORMAL_MOVE_ON,
};

//------------------------------------------
//	�ʏ�ړ�
//------------------------------------------
#ifndef PM_DEBUG
static void HeroMoveNormal( u8 site, u16 cont )
{
	NormalMoveTbl[NormalMoveValue(site)]( site, cont );
}
#endif

//------------------------------------------
//	�ʏ�ړ� ���ޯ��
//------------------------------------------
#ifdef PM_DEBUG
static void HeroMoveNormal( u8 site, u16 cont )
{
	if( DebugMainFlag && HeroDebugMove(site) )
	{
		return;
	}

	NormalMoveTbl[NormalMoveValue(site)]( site, cont );
}
#endif

//------------------------------------------
//	�ʏ�ړ� ��������
//------------------------------------------
static u8 NormalMoveValue( u8 site )
{
	u8 hsite;

	if( site == SITE_N )								// ���ߖ���
	{
		MyMoveState.mv_val = EV_M_STOP;					// ��~
		return( NORMAL_MOVE_STOP );
	}

	hsite = GetHeroMoveSite();

	if( site != hsite &&								// ���� != ���ߕ���
		MyMoveState.mv_val != EV_M_WALK )				// �ړ����ł͂Ȃ�
	{
		MyMoveState.mv_val = EV_M_TURN;					// �U�����
		return( NORMAL_MOVE_TURN );
	}

	MyMoveState.mv_val = EV_M_WALK;						// �ړ�
	return( NORMAL_MOVE_ON );
}

//------------------------------------------
//	�ʏ�ړ�	���ߖ���
//------------------------------------------
static void normal_move_stop( u8 site, u16 cont )
{
	HeroAcmdStop( GetHeroSite() );
}

//------------------------------------------
//	�ʏ�ړ�	�U�����
//------------------------------------------
static void normal_move_turn( u8 site, u16 cont )
{
	HeroAcmdWalkTurn( site );
}

//------------------------------------------
//	�ʏ�ړ�	�ړ�
//------------------------------------------
static void normal_move_on( u8 site, u16 cont )
{
	u8 flag;
	
	flag = HeroMoveCheckSite( site );
	
	if( flag )
	{
		if( flag == HEROHIT_JUMP )
		{
			HeroAcmdJump( site );
			return;
		}

		if( flag < HEROHIT_START || flag >= HEROHIT_CYCLE_START )
		{
			HeroAcmdStopWalk05( site );
		}

		return;
	}

	if( MyMoveState.flag & MY_MV_BIT_SWIM )
	{
		HeroAcmdWalk2( site );							// ���s�����
	}
	else
	{
		if( ((MyMoveState.flag & MY_MV_BIT_DIVING) == 0) &&
			(cont & B_BUTTON) && EventFlagCheck(SYS_B_DASH) &&
			CycleBatMoveAttrCheck(EvObj[MyMoveState.evobjno].now_attr) == 0 )
		{
			HeroAcmdDash2( site );
			MyMoveState.flag |= MY_MV_BIT_B_DASH;
		}
		else
		{
			HeroAcmdWalk1( site );						// ���s�����
		}
	}
}

//======================================================================
//		�ړ�����
//======================================================================
static u8 HeroMoveCheckSwim( s16, s16, u8 );
static u8 HeroMoveCheckJump( s16, s16, u8 );
static u8 HeroMoveCheckPushRock( s16, s16, u8 );
static void HeroMoveCheckCycleOnly( s16, s16, u8, u8 * );

//------------------------------------------
//	���@ �����ړ�����
//------------------------------------------
u8 HeroMoveCheckSite( u8 site )
{
	s16 x,y;
	EventOBJ *ev;
	
	ev = &EvObj[MyMoveState.evobjno];
	x = ev->gx;
	y = ev->gy;
	SitePosMove( site, &x, &y );

	return( HeroMoveCheck(ev,x,y,site,GetCellAttribute(x,y)) );
}

//------------------------------------------
//	���@ �ړ�����
//------------------------------------------
u8 HeroMoveCheck( EventOBJ *ev, s16 x, s16 y, u8 site, u8 attr )
{
	u8 flag;

	flag = EvObjMoveChk( ev, x, y, site );

	if( flag == EVOBJHIT_HEIGHT && HeroMoveCheckSwim(x,y,site) )
	{
		return( HEROHIT_SWIM );
	}

	if( HeroMoveCheckJump(x,y,site) )
	{
		IncRecord( CNT_JUMP );
		return( HEROHIT_JUMP );
	}

	if( flag == EVOBJHIT_OBJ && HeroMoveCheckPushRock(x,y,site) )
	{
		return( HEROHIT_KAIRIKI );
	}

	if( flag == EVOBJHIT_NOTHING )
	{
		if( DoorPushCheck(site,x,y) )
		{
			return( HEROHIT_DOOR );
		}

		HeroMoveCheckCycleOnly( x, y, attr, &flag );
	}
	
	return( flag );
}

//------------------------------------------
//	�g���ړ�����
//------------------------------------------
static u8 HeroMoveCheckSwim( s16 x, s16 y, u8 site )
{
	u8 height;

	if( (MyMoveState.flag & MY_MV_BIT_SWIM) )
	{
		height = GetCellHeight( x, y );

		if( height == 3 && EvObjHitCheck(x,y,height) == EV_OBJ_MAX )
		{
			SetHeroSwimEndTask( site );					// �㗤
			return( 1 );
		}
	}

	return( 0 );
}

//------------------------------------------
//	�ެ��߈ړ�����
//------------------------------------------
static u8 HeroMoveCheckJump( s16 x, s16 y, u8 site )
{
	if( EvMapAttrJumpChk(x,y,site) )
	{
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	���͈ړ�����
//------------------------------------------
static u8 HeroMoveCheckPushRock( s16 x, s16 y, u8 site )
{
	u8 i;
	EventOBJ *rock;

	if( EventFlagCheck(SYS_USE_WAZA_KAIRIKI) )			// ���͎g�p
	{
		i = EvObjWorkMapPosGet( x, y );

		if( i == EV_OBJ_MAX || EvObj[i].obj_code != PUSHROCK1 )
		{
			return( 0 );
		}

		rock = &EvObj[i];

		x = rock->gx;
		y = rock->gy;
		SitePosMove( site, &x, &y );

		if( EvObjMoveChk(rock,x,y,site) ||
			ATR_IsWallExitCheck(GetCellAttribute(x,y)) )
		{
			return( 0 );								// �����s��
		}

		SetHeroPushRockTask( i, site );

		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	���]�ԗp����ޭ������ð���
//------------------------------------------
#define HMCOC_MAX 5

static u8 (* const HeroMoveCheckCycleOnlyTbl[])(u8) =
{
	ATR_IsJumpStoneCheck,
	ATR_IsVBoardCheck,
	ATR_IsHBoardCheck,
	ATR_IsVBridgeCheck,
	ATR_IsHBridgeCheck,
};

static const u8 HeroMoveCheckCycleOnlyFlagTbl[] =
{
	HEROHIT_JSTONE,
	HEROHIT_VBOARD,
	HEROHIT_HBOARD,
	HEROHIT_VBRIDGE,
	HEROHIT_HBRIDGE,
};

//------------------------------------------
//	���]�Ԑ�p����ޭ������
//------------------------------------------
static void HeroMoveCheckCycleOnly( s16 x, s16 y, u8 attr, u8 *flag )
{
	u8 i;

	for( i = 0; i < HMCOC_MAX; i++ )
	{
		if( HeroMoveCheckCycleOnlyTbl[i](attr) )
		{
			*flag = HeroMoveCheckCycleOnlyFlagTbl[i];
			return;
		}
	}
}

//======================================================================
//		�v��
//======================================================================
static void hero_normal_req( EventOBJ * );
static void hero_cycle_r_req( EventOBJ * );
static void hero_cycle_s_req( EventOBJ * );
static void hero_swim_req( EventOBJ * );
static void hero_div_req( EventOBJ * );
static void hero_under_req( EventOBJ * );
static void hero_dmy_req( EventOBJ * );

//------------------------------------------
//	ظ����׸޾�� & ظ���
//	In:		req_flag	; MY_MV_BIT_SWIM ��
//------------------------------------------
void SetHeroMoveRequest( u16 req_flag )
{
	MyMoveState.req_flag |= req_flag;
	HeroMoveRequest();
}

//------------------------------------------
//	ظ���ð���
//------------------------------------------
static void (* const HeroRequestTbl[])( EventOBJ * ) =
{
	hero_normal_req,									// 0 �ʏ�ړ�
	hero_cycle_r_req,									// 1 ���]�� TYPE_R
	hero_cycle_s_req,									// 2 ���]�� TYPE_S
	hero_swim_req,										// 3 �g���
	hero_div_req,										// 4 �޲��ݸ�
	hero_under_req,										// 5 ����������
	hero_dmy_req,										// 6 �����ړ�
	hero_dmy_req,										// 7
};
	
//------------------------------------------
//	�׸�,���޺��۰�
//------------------------------------------
void HeroMoveRequest( void )
{
	u8 i,req;

	req = MyMoveState.req_flag;

	if( req )
	{
		for( i = 0; i < 8; i++, req >>= 1 )
		{
			if( (req & 0x01) ) HeroRequestTbl[i](&EvObj[MyMoveState.evobjno]);
		}

		MyMoveState.req_flag = 0;
	}
}

//------------------------------------------
//	��аظ���
//------------------------------------------
static void hero_dmy_req( EventOBJ *ev )
{
}

//------------------------------------------
//	ظ���	�ʏ�ړ�
//------------------------------------------
static void hero_normal_req( EventOBJ *ev )
{
	EvObjChgParam( ev, GetMainHeroObjCode(H_NORMAL) );
	EvObjChgSite( ev, ev->mv_site );
	MyMoveStateMvFlagSet( MY_MV_BIT_NORMAL );
}

//------------------------------------------
//	ظ���	���]������R
//------------------------------------------
static void hero_cycle_r_req( EventOBJ *ev )
{
	EvObjChgParam( ev, GetMainHeroObjCode(H_CYCLER) );
	EvObjChgSite( ev, ev->mv_site );
	MyMoveStateMvFlagSet( MY_MV_BIT_CYCLE_R );
	MyCycleWorkInit( 0, 0 );
}

//------------------------------------------
//	ظ���	���]������S
//------------------------------------------
static void hero_cycle_s_req( EventOBJ *ev )
{
	EvObjChgParam( ev, GetMainHeroObjCode(H_CYCLES) );
	EvObjChgSite( ev, ev->mv_site );
	MyMoveStateMvFlagSet( MY_MV_BIT_CYCLE_S );
	MyCycleWorkInit( 0, 0 );
	SetHeroCycleStatus();
}

//------------------------------------------
//	�g���ظ���
//------------------------------------------
static void hero_swim_req( EventOBJ *ev )
{
	u8	i;

	EvObjChgParam( ev, GetMainHeroObjCode(H_SWIM) );
	EvObjChgSite( ev, ev->mv_site );
	MyMoveStateMvFlagSet( MY_MV_BIT_SWIM );
	FeSetWork[0] = ev->gx;
	FeSetWork[1] = ev->gy;
	FeSetWork[2] = MyMoveState.evobjno;
	i = FldEffSet( FECODE_PokeSwim );
	ev->joint_actno[0] = i;
	SetPokeSwimMoveFlag( i, NAMIPOKE_WITH );
}

//------------------------------------------
//	�޲��ݸ�ظ���(����g���
//------------------------------------------
static void hero_div_req( EventOBJ *ev )
{
	EvObjChgParam( ev, GetMainHeroObjCode(H_DIV) );
	EvObjChgSite( ev, ev->mv_site );
	MyMoveStateMvFlagSet( MY_MV_BIT_DIVING );
	ev->joint_actno[0] = EvActDivingSwaySet( ev->actno );

#if 0
	FeSetWork[0] = ev->gx;
	FeSetWork[1] = ev->gy;
	FeSetWork[2] = MyMoveState.evobjno;
	i = FldEffSet( FECODE_PokeSwim );
	ev->joint_actno[0] = i;
	SetPokeSwimMoveFlag( i, NAMIPOKE_WITH );
	ActWork[i].oamData.Priority = BGPRI2;
#endif
}

//------------------------------------------
//	����������ظ���
//------------------------------------------
static void hero_under_req( EventOBJ *ev )
{
	MyMoveState.flag |= MY_MV_BIT_UNDER;
}

//======================================================================
//		�����
//======================================================================
static u8 HeroAcmdStopCheck0( void );
static u8 HeroAcmdStopCheck1( void );

//------------------------------------------
//	���@����󋵾��
//------------------------------------------
void HeroMvStateSet( void )
{
	MyMoveState.mv_state = EV_M_OFF;					// ��

	if( HeroAcmdSetChk() == 0 )							// ���������
	{
		return;											// ������
	}

	if( HeroAcmdEndChk() == 0 )							// �I������
	{
		if( HeroAcmdStopCheck0() == 0 )					// ���쒆,����޺�������
		{
			MyMoveState.mv_state = EV_M_ON;				// ���쒆
		}
	}
	else if( HeroAcmdStopCheck1() == 0 )				// ����I��
	{
		MyMoveState.mv_state = EV_M_END;				// ����I��
	}
}

//------------------------------------------
//	��Ұ��ݺ��� ��~��������
//------------------------------------------
static u8 HeroAcmdStopCheck0( void )
{
	u8 code;

	code = EvObj[MyMoveState.evobjno].acmd;

#if 0
	if(	code <= AC_SITE_R ||
		(code >= AC_WAIT_1 && code <= AC_WAIT_16) ||
		(code >= AC_STAY_WALK_D_05 && code <= AC_STAY_WALK_R_4) ||
		(code >= AC_MTB_FTUP_SITE_D && code <= AC_MTB_FTDOWN_R) ||
		(code >= AC_MTB_DANIEL_D && code <= AC_MTB_DANIEL_R) ||
		(code >= AC_MTB_WHEELIE_D && code <= AC_MTB_WHEELIE_R) )
	{
		return( 1 );
	}
#else
	if(	code <= AC_SITE_R ||
		(code >= AC_WAIT_1 && code <= AC_WAIT_16) ||
		(code >= AC_STAY_WALK_D_05 && code <= AC_STAY_WALK_R_4) ||
		(code >= AC_MTB_FTUP_SITE_D && code <= AC_MTB_FTDOWN_R) ||
		(code >= AC_MTB_WHEELIE_D && code <= AC_MTB_WHEELIE_R) )
	{
		return( 1 );
	}
#endif

	return( 0 );
}

//------------------------------------------
//	��Ұ��ݺ���� ��~�������� �U���������
//------------------------------------------
static u8 HeroAcmdStopCheck1( void )
{
	u8 code;

	if( HeroAcmdStopCheck0() )
	{
		code = EvObj[MyMoveState.evobjno].acmd;

		if( MyMoveState.mv_val != EV_M_TURN )
		{
			return( 1 );
		}
	}

	return( 0 );
}

//======================================================================
//		���@�����
//======================================================================
static void HeroWallHitSePlay( u8 );

//------------------------------------------
//	���@���������
//------------------------------------------
u8 HeroAcmdSetChk( void )
{
	return( AcmdSetChk(&EvObj[MyMoveState.evobjno]) );
}

//------------------------------------------
//	���@����ޏI������
//------------------------------------------
u8 HeroAcmdEndChk( void )
{
	return( AcmdEndChk(&EvObj[MyMoveState.evobjno]) );
}

//------------------------------------------
//	���@�����Ӱ��,�������
//------------------------------------------
void HeroAcmdModeSet( u8 mode )
{
	EvObj[MyMoveState.evobjno].work1 = mode;
}

//------------------------------------------
//	���@�����Ӱ��
//------------------------------------------
u8 HeroAcmdMode( void )
{
	return( EvObj[MyMoveState.evobjno].work1 );
}

//------------------------------------------
//	���@��������޾��
//------------------------------------------
void HeroAcmdForceSet( u8 code )
{
	AcmdForceSet( &EvObj[MyMoveState.evobjno], code );
}

//------------------------------------------
//	���@����޾��
//------------------------------------------
void HeroAcmdSet( u8 code, u8 mode )
{
	if( HeroAcmdSetChk() == 0 )
	{
		HeroAcmdModeSet( mode );
		AcmdSet( &EvObj[MyMoveState.evobjno], code );
	}
}
 
//------------------------------------------
//	���@�p���s����޾��
//------------------------------------------
void HeroAcmdWalk1( u8 site )
{
	HeroAcmdSet( AcmdCodeWalk1(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p���s����޾��
//------------------------------------------
void HeroAcmdWalk2( u8 site )
{
	HeroAcmdSet( AcmdCodeWalk2(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p���s����޾��
//------------------------------------------
void HeroAcmdWalk3( u8 site )
{
	HeroAcmdSet( AcmdCodeWalk3(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p4�ޯĈړ�����޾��
//------------------------------------------
void HeroAcmdWalk4( u8 site )
{
	HeroAcmdSet( AcmdCodeWalk4(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p2�ޯ��ޯ���ړ�����޾��
//------------------------------------------
void HeroAcmdDash2( u8 site )
{
	HeroAcmdSet( AcmdCodeDash2(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p���̏ꑫ���ݺ���޾��
//------------------------------------------
void HeroAcmdStopWalk( u8 site )
{
	HeroWallHitSePlay( site );
	HeroAcmdSet( AcmdCodeStayWalk1(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p���̏ꑫ����0.5����޾��
//------------------------------------------
void HeroAcmdStopWalk05( u8 site )
{
	HeroWallHitSePlay( site );
	HeroAcmdSet( AcmdCodeStayWalk05(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p��~����޾��
//------------------------------------------
void HeroAcmdStop( u8 site )
{
	HeroAcmdSet( AcmdCodeStopSite(site), ACMD_MODE_SITE );
}

//------------------------------------------
//	���@�p�U�������������޾��
//------------------------------------------
void HeroAcmdWalkTurn( u8 site )
{
	HeroAcmdSet( AcmdCodeStayWalk2(site), ACMD_MODE_SITE );
}

//------------------------------------------
//	���@�p�ެ��ߺ���޾��
//------------------------------------------
void HeroAcmdJump( u8 site )
{
	SePlay( SE_DANSA );
	HeroAcmdSet(AcmdCodeJump2(site), ACMD_MODE_JUMP_2 );
}

//------------------------------------------
//	���@�p ������~����޾��
//------------------------------------------
void HeroAcmdForceStop( void )
{
	if( (MyMoveState.mv_state == EV_M_END || MyMoveState.mv_state == EV_M_OFF) &&
		CheckFtDownEnableCycle() )
	{
		HeroAcmdForceSet( AcmdCodeStopSite(EvObj[MyMoveState.evobjno].site) );
	}
}

//------------------------------------------
//	���@�p ���ı��ߺ���޾��
//------------------------------------------
void HeroAcmdMtbFtUpSite( u8 site )
{
	HeroAcmdSet( AcmdCodeMtbFtUpSite(site), ACMD_MODE_SITE );
}

//------------------------------------------
//	���@�p ���ı��ߺ���޾��
//------------------------------------------
void HeroAcmdMtbFtUp( u8 site )
{
	HeroAcmdSet( AcmdCodeMtbFtUp(site), ACMD_MODE_SITE );
}

//------------------------------------------
//	���@�p �����޳ݺ���޾��
//------------------------------------------
void HeroAcmdMtbFtDown( u8 site )
{
	HeroAcmdSet( AcmdCodeMtbFtDown(site), ACMD_MODE_SITE );
}

//------------------------------------------
//	���@�p ��ƴٺ���޾��
//------------------------------------------
void HeroAcmdMtbDaniel( u8 site )
{
	SePlay( SE_JITE_PYOKO );
	HeroAcmdSet( AcmdCodeMtbDaniel(site), ACMD_MODE_SITE );
}

//------------------------------------------
//	���@�p ��ƴ�1����޾��
//------------------------------------------
void HeroAcmdMtbDaniel1( u8 site )
{
	SePlay( SE_JITE_PYOKO );
	HeroAcmdSet( AcmdCodeMtbDaniel1(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p ��ƴ�2����޾��
//------------------------------------------
void HeroAcmdMtbDaniel2( u8 site )
{
	SePlay( SE_JITE_PYOKO );
	HeroAcmdSet( AcmdCodeMtbDaniel2(site), ACMD_MODE_JUMP_2 );
}

//------------------------------------------
//	���@�p �ެ�����ݾ��
//------------------------------------------
void HeroAcmdJumpTurn( u8 site )
{
	SePlay( SE_JITE_PYOKO );
	HeroAcmdSet( AcmdCodeStayTurnJump(site), ACMD_MODE_SITE );
}

//------------------------------------------
//	���@�p ���̏곲ذ���
//------------------------------------------
void HeroAcmdMtbWheelie( u8 site )
{
	SePlay( SE_WALL_HIT );
	HeroAcmdSet( AcmdCodeMtbWheelie(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p ��ذ�J�n2���
//------------------------------------------
void HeroAcmdMtbWheelieStart2( u8 site )
{
	HeroAcmdSet( AcmdCodeMtbWheelieStart2(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p ��ذ2���
//------------------------------------------
void HeroAcmdMtbWheelie2( u8 site )
{
	HeroAcmdSet( AcmdCodeMtbWheelie2(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	���@�p ��ذ�I��2���
//------------------------------------------
void HeroAcmdMtbWheelieEnd2( u8 site )
{
	HeroAcmdSet( AcmdCodeMtbWheelieEnd2(site), ACMD_MODE_WALK_1 );
}

//------------------------------------------
//	�Փˉ���������ð���
//------------------------------------------
static u8 (* const HeroWallHitMatCheckTbl[])( u8 ) =
{
	ATR_ExitMatDownEnableCheck,							// SITE_D
	ATR_ExitMatUpEnableCheck,							// SITE_U
	ATR_ExitMatLeftEnableCheck,							// SITE_L
	ATR_ExitMatRightEnableCheck,						// SITE_R
};

//------------------------------------------
//	�Փˉ�����
//------------------------------------------
static void HeroWallHitSePlay( u8 site )
{
	u8 attr;
	s16 x,y;

	attr = EvObj[MyMoveState.evobjno].now_attr;

	if( HeroWallHitMatCheckTbl[site-1](attr) )
	{
		return;
	}

	if( site == SITE_U )
	{
		GetHeroCellPos( &x, &y );
		SitePosMove( site, &x, &y );
		attr = GetCellAttribute( x, y );
		
		if( ATR_IsDoorCheck(attr) )
		{
			return;
		}
	}

	SePlay( SE_WALL_HIT );
}

//======================================================================
//		���@���
//======================================================================
static const u8 HsexObjCodeTbl[][2];
static const u8 MainHsexObjCodeTbl[][2];

//------------------------------------------
//	���@�̑O�� �� �ʒu�擾
//	In:		x,y				; ���W�i�[
//------------------------------------------
void GetHeroFrontCellPos( s16 *x, s16 *y )
{
	*x = EvObj[MyMoveState.evobjno].gx;
	*y = EvObj[MyMoveState.evobjno].gy;
	SitePosMove( GetHeroSite(), x, y );
}

//------------------------------------------
//	���@���� �� �ʒu�擾
//	In:		x,y				; ���W�i�[
//------------------------------------------
void GetHeroCellPos( s16 *x, s16 *y )
{
	*x = EvObj[MyMoveState.evobjno].gx;
	*y = EvObj[MyMoveState.evobjno].gy;
}

//------------------------------------------
//	���@�̕\�������擾
//	Out:	u8				; ����
//------------------------------------------
u8 GetHeroSite( void )
{
	return( EvObj[MyMoveState.evobjno].site );
}

//------------------------------------------
//	���@�̈ړ������擾
//------------------------------------------
u8 GetHeroMoveSite( void )
{
	return( EvObj[MyMoveState.evobjno].mv_site );
}

//------------------------------------------
//	���@�̍����擾
//	Out:	u8				; ����
//------------------------------------------
u8 GetHeroHeight( void )
{
	return( EvObj[MyMoveState.evobjno].height_pri );
}

//------------------------------------------
//	���@�̍��W�X�V
//------------------------------------------
void SetHeroMapPos( s16 mx, s16 my )
{
	EvObjPosAllChg( &EvObj[MyMoveState.evobjno], mx, my );
}

//------------------------------------------
//	���@�̏������
//	In:		bit			; MY_MV_BIT_NORMAL��
//	Out:	u8			; On = ���̏�Ԃł���
//------------------------------------------
u8 CheckHeroState( u8 bit )
{
	return( MyMoveState.flag & bit );
}

//------------------------------------------
//	���@�̏�Ԏ擾
//	Out:	u8			; �����׸�
//------------------------------------------
u8 GetHeroState( void )
{
	return( MyMoveState.flag );
}

//------------------------------------------
//	���@�̱��ܰ����ގ擾
//------------------------------------------
u8 GetHeroActorNumber( void )
{
	return( MyMoveState.actno );
}

//------------------------------------------
//	�����ړ�����
//------------------------------------------
void ResetHeroForceMove( void )
{
	non_force_set();
}

//------------------------------------------
//	����󋵸ر
//------------------------------------------
void SetHeroMoveStateInit( void )
{
	EventOBJ *ev;

	ev = &EvObj[MyMoveState.evobjno];

	HeroMoveStartInit( ev );
	EvObjSiteSet( ev, ev->site );

	if( CheckHeroState(MY_MV_BIT_CYCLE_AND) )
	{
		SetHeroCycleStatus();
		CycleSpeedBrakeSet( 0 );
	}
}

//------------------------------------------
//	OBJ���ސ��ʕ���
//	In:		kind		; H_NORMAL��
//			sex			; ����
//------------------------------------------
u8 GetHeroSexObjCode( u8 kind, u8 sex )
{
	return( HsexObjCodeTbl[kind][sex] );
}

//------------------------------------------
//	OBJ���ސ��ʕ���(��ڲ԰)
//	In:		kind		; H_NORMAL��
//			sex			; ����
//------------------------------------------
u8 GetMainHeroSexObjCode( u8 kind, u8 sex )
{
	return( MainHsexObjCodeTbl[kind][sex] );
}

//------------------------------------------
//	OBJ���ސ��ʕ��� MyMoveState�^
//	In:		kind		; H_NORMAL��
//------------------------------------------
u8 GetMainHeroObjCode( u8 kind )
{
	return( GetMainHeroSexObjCode(kind,MyMoveState.sex) );
}

//------------------------------------------
//	OBJ���� -> ����
//	In:		code		; OBJ ����
//------------------------------------------
u8 GetHeroObjCodeSex( u8 code )
{
	switch( code )
	{
		case HEROINE:
		case HINECYCLER:
		case HINECYCLES:
		case HINESWIM:
		case HINEHIDEN:
		case MAINHINEDIV:
		case MAINHINEFISH:
		case MAINHINEJORRO:
			return( FEMALE );
	}

	return( MALE );
}

//------------------------------------------
//	OBJ���� -> ����
//	In:		code		; OBJ ����
//------------------------------------------
u8 GetMainHeroObjCodeSex( u8 code )
{
	switch( code )
	{
		case MAINHEROINE:
		case MAINHINECYCLER:
		case MAINHINECYCLES:
		case MAINHINESWIM:
		case MAINHINEHIDEN:
		case MAINHINEDIV:
		case MAINHINEFISH:
		case MAINHINEJORRO:
			return( FEMALE );
	}

	return( MALE );
}

//------------------------------------------
//	�߹�ݔg���J�n�\����
//	Out:	u8			; ON = �g���\
//------------------------------------------
u8 CheckHeroNaminoriStart( void )
{
	u8 i;
	
	if( CheckHeroState(MY_MV_BIT_SWIM) == 0 )
	{
		for( i = 0; i < 6; i++ )
		{
			if( PokeParaGet(&PokeParaMine[i],ID_monsno) == 0 )
			{
				break;
			}

			if( TemotiNijyuuCheck(&PokeParaMine[i],WAZANO_NAMINORI) )
			{
				return( 1 );
			}
		}
	}

	return( 0 );
}

//------------------------------------------
//	����J�n�\����
//	Out:		u8			; ON = ��o��\
//------------------------------------------
u8 CheckHeroTakinoboriStart( void )
{
	if( GetHeroMoveSite() == SITE_U &&					// �����
		CheckHeroState(MY_MV_BIT_SWIM) )				// �g��蒆
	{
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�g���n�`����
//	Out:	u8			; ON = �g���\�n�`
//	Info:	�肪�ł��邩�ǂ����̔���ɂ��g�p���Ă���
//			������ύX����Ƃ��͒���
//------------------------------------------
u8 CheckHeroAttrNaminoriStart( void )
{
	u8 flag;
	s16	x,y;
	EventOBJ *ev;

	ev = &EvObj[MyMoveState.evobjno];
	x = ev->gx;
	y = ev->gy;
	SitePosMove( ev->mv_site, &x, &y );
	
	flag = EvObjMoveChk( ev, x, y, ev->mv_site );

	if( flag == EVOBJHIT_HEIGHT && GetHeroHeight() == 3 )
	{
		if( ATR_IsNaminoriStartCheck(GetCellAttribute(x,y)) )
		{
			return( 1 );
		}
	}

	return( 0 );
}

//------------------------------------------
//	OBJ���� ����ð���
//------------------------------------------
static const u8 HsexObjCodeTbl[][2] =
{
	{ HERO,			HEROINE },
	{ HEROCYCLER,	HINECYCLER },
	{ HEROCYCLES,	HINECYCLES },
	{ HEROSWIM,		HINESWIM },
	{ MAINHERODIV,	MAINHINEDIV },
	{ HEROHIDEN,	HINEHIDEN },
	{ MAINHEROFISH, MAINHINEFISH },
	{ MAINHEROJORRO, MAINHINEJORRO },
};

//------------------------------------------
//	OBJ���� ����ð���(��ڲ԰)
//------------------------------------------
static const u8 MainHsexObjCodeTbl[][2] =
{
	{ MAINHERO,			MAINHEROINE },
	{ MAINHEROCYCLER,	MAINHINECYCLER },
	{ MAINHEROCYCLES,	MAINHINECYCLES },
	{ MAINHEROSWIM,		MAINHINESWIM },
	{ MAINHERODIV,		MAINHINEDIV },
	{ MAINHEROHIDEN,	MAINHINEHIDEN },
	{ MAINHEROFISH,		MAINHINEFISH },
	{ MAINHEROJORRO,	MAINHINEJORRO },
};

//======================================================================
//		MyMoveState
//======================================================================
#define HERO_OBJ_CODE_MAX 5
static const u8 MyMoveObjCodeFlagTbl[2][HERO_OBJ_CODE_MAX][2];

//------------------------------------------
//	MyMoveState ������
//------------------------------------------
void MyMoveStateInit( void )
{
	memset( (u8*)&MyMoveState, 0, MvCmd_SIZE );
}

//------------------------------------------
//	�����׸޾��
//------------------------------------------
void MyMoveStateMvFlagSet( u8 flag )
{
	MyMoveState.flag =										// �����׸ޏ��� | �����׸�
		(MyMoveState.flag & MY_MV_BIT_AND) | flag;
}

//------------------------------------------
//	OBJ ���� -> �����׸�
//------------------------------------------
u8 MyMoveStateObjCodeFlag( u8 obj_code, u8 sex )
{
	u8 i;

	for( i = 0; i < HERO_OBJ_CODE_MAX; i++ )
	{
		if( MyMoveObjCodeFlagTbl[sex][i][0] == obj_code )
		{
			return( MyMoveObjCodeFlagTbl[sex][i][1] );
		}
	}
	
	return( MY_MV_BIT_NORMAL );
}

//------------------------------------------
//	�����׸� -> OBJ ����
//------------------------------------------
u8 MyMoveStateFlagObjCode( void )
{
	u8 i,flag;
	
	flag = MyMoveState.flag;

	for( i = 0; i < HERO_OBJ_CODE_MAX; i++ )
	{
		if( MyMoveObjCodeFlagTbl[MyMoveState.sex][i][1] & flag )
		{
			return( MyMoveObjCodeFlagTbl[MyMoveState.sex][i][0] );
		}
	}

	return( MAINHERO );
}

//------------------------------------------
//	OBJ ���� -> �����׸޾��,ظ���
//------------------------------------------
void MyMoveStateObjCodeRequest( u8 obj_code, u8 set_flag )
{
	u8 flag;

	flag = MyMoveStateObjCodeFlag( obj_code, MyMoveState.sex );
	MyMoveState.req_flag |= flag | set_flag;
	HeroMoveRequest();
}

//------------------------------------------
//	OBJ����,�׸�ð���
//------------------------------------------
static const u8 MyMoveObjCodeFlagTbl[2][HERO_OBJ_CODE_MAX][2] =
{
	{
		{ MAINHERO, MY_MV_BIT_NORMAL },
		{ MAINHEROCYCLER, MY_MV_BIT_CYCLE_R },
		{ MAINHEROCYCLES, MY_MV_BIT_CYCLE_S },
		{ MAINHEROSWIM, MY_MV_BIT_SWIM },
		{ MAINHERODIV, MY_MV_BIT_DIVING },
	},
	{
		{ MAINHEROINE, MY_MV_BIT_NORMAL },
		{ MAINHINECYCLER, MY_MV_BIT_CYCLE_R },
		{ MAINHINECYCLES, MY_MV_BIT_CYCLE_S },
		{ MAINHINESWIM, MY_MV_BIT_SWIM },
		{ MAINHINEDIV, MY_MV_BIT_DIVING },
	},
};

//======================================================================
//		���@OBJ
//======================================================================
//------------------------------------------
//	��l��̨����OBJ�o�^
//	In:		px,py			; ϯ�ߍ��W
//			site,sex		; ����,����
//------------------------------------------
void SetJikiActor( s16 px, s16 py, u8 site, u8 sex )
{
	u8 no;
	EventOBJ *ev;
	evObjData HeroData;

	HeroData.id = EV_OBJ_SPID;
	HeroData.obj_code = GetMainHeroSexObjCode( H_NORMAL, sex );
	HeroData.pos_x = px - MAP_LEFT_MARGIN;
	HeroData.pos_y = py - MAP_UP_MARGIN;
	HeroData.height = 0;
	HeroData.site = MV_HERO;
	HeroData.mvlim_x = 0;
	HeroData.mvlim_y = 0;
	HeroData.ev_type = OTYPE_NORMAL;
	HeroData.work = 0;
	HeroData.ev_label = 0;
	HeroData.ev_flag = 0;

	no = EvObjSxyAddNow( &HeroData );
	
	ev = &EvObj[no];
	ev->scr_del_sw = 1;
	ev->joint_actno[1] = EvActExitCursorSet();
	EvObjChgSite( ev, site );

	MyMoveStateInit();
	MyMoveState.mv_val= EV_M_STOP;
	MyMoveState.mv_state = EV_M_OFF;
	MyMoveState.evobjno = no;
	MyMoveState.actno = ev->actno;
	MyMoveState.sex = sex;
	MyMoveStateMvFlagSet( MY_MV_BIT_NORMAL | MY_MV_BIT_UNDER );
}

//------------------------------------------
//	��l��OBJ �\��ON,OFF
//------------------------------------------
void SetHeroObjVanish( u8 sw )
{
	EvObjNoBanishSet( MyMoveState.evobjno, sw );

	if( CheckHeroState(MY_MV_BIT_SWIM) )
	{
		ActWork[EvObj[MyMoveState.evobjno].joint_actno[0]].banish = sw;
	}
}

//------------------------------------------
//	��l����`�Z�����֕ύX
//------------------------------------------
void ChangeHeroActorHiden( void )
{
	EvObjChgParam( &EvObj[MyMoveState.evobjno], GetMainHeroObjCode(H_HIDEN) );
	ActAnmChg( &ActWork[MyMoveState.actno], 0 );
}

//------------------------------------------
//	��l���ނ豸���֕ύX
//	In:		site			; ����
//------------------------------------------
void ChangeHeroActorFishing( u8 site )
{
	EvObjChgParam( &EvObj[MyMoveState.evobjno], GetMainHeroObjCode(H_FISHING) );
	ActAnmChg( &ActWork[MyMoveState.actno], EvActAnmNoFishingStart(site) );
}

//------------------------------------------
//	��l�����]�����ı��߂֕ύX
//------------------------------------------
void ChangeHeroActorCycleFtUp( u8 site )
{
	EvObjChgParam( &EvObj[MyMoveState.evobjno], GetMainHeroObjCode(H_CYCLES) );
	ActAnmChg( &ActWork[MyMoveState.actno], EvActAnmNoFtUp(site) );
	ActAnmOffsChg( &ActWork[MyMoveState.actno], 1 );		// 1->����قɕύX���悤
}

//------------------------------------------
//	��l���@�J�I�����֕ύX
//	In:		site			; ����
//------------------------------------------
void ChangeHeroActorJorro( u8 site )
{
	EvObjChgParam( &EvObj[MyMoveState.evobjno], GetMainHeroObjCode(H_JORRO) );
	ActAnmChg( &ActWork[MyMoveState.actno], EvActAnmNoStop(site) );
}

//======================================================================
//		�t������
//======================================================================
static u8 (* const ExitAttributeTbl[])( u8 );

//------------------------------------------
//	�o��������ٺ��۰�
//------------------------------------------
static void ExitCursorControl( EventOBJ *ev )
{
	u8 site,attr;
	s16 x,y;

	attr = ev->now_attr;
	
	for( x = 0, site = SITE_D; x < 4; x++, site++ )
	{
		if( ExitAttributeTbl[x](attr) && site == ev->mv_site )
		{
			x = ev->gx;
			y = ev->gy;
			SitePosMove( site, &x, &y );
			EvActExitCursorON( ev->joint_actno[1], site, x, y );
			return;
		}
	}

	EvActExitCursorOFF( ev->joint_actno[1] );
}

//------------------------------------------
//	�o��������ޭ������ð���
//------------------------------------------
static u8 (* const ExitAttributeTbl[])(u8) =
{
	ATR_ExitMatDownEnableCheck,
	ATR_ExitMatUpEnableCheck,
	ATR_ExitMatLeftEnableCheck,
	ATR_ExitMatRightEnableCheck
};

//======================================================================
//		��l���≟��
//======================================================================
static void HeroPushRockTask( u8 );
static u8 HeroPushRock0( TASK_TABLE *, EventOBJ *, EventOBJ * );
static u8 HeroPushRock1( TASK_TABLE *, EventOBJ *, EventOBJ * );
static u8 HeroPushRock2( TASK_TABLE *, EventOBJ *, EventOBJ * );

//------------------------------------------
//	�≟����������ǉ�
//------------------------------------------
static void SetHeroPushRockTask( u8 evno, u8 site )
{
	u8 i;
	
	i = AddTask( HeroPushRockTask, 0xff );
	TaskTable[i].work[1] = evno;
	TaskTable[i].work[2] = site;
	HeroPushRockTask( i );									// �Ăяo��
}

//------------------------------------------
//	�≟�����ð���
//------------------------------------------
static u8 (* const HeroPushRockTaskTbl[])( TASK_TABLE *, EventOBJ *, EventOBJ * ) =
{
	HeroPushRock0,
	HeroPushRock1,
	HeroPushRock2,
};

//------------------------------------------
//	�≟���������
//------------------------------------------
static void HeroPushRockTask( u8 no )
{
	while( HeroPushRockTaskTbl[TaskTable[no].work[0]]
	(&TaskTable[no],&EvObj[MyMoveState.evobjno], &EvObj[TaskTable[no].work[1]]) );
}

//------------------------------------------
//	�≟�� 0
//------------------------------------------
static u8 HeroPushRock0( TASK_TABLE *task, EventOBJ *h_ev, EventOBJ *r_ev )
{
	SetForceEvent();
	MyMoveState.mv_pause = 1;
	task->work[0]++;
	return( 0 );
}

//------------------------------------------
//	�≟�� 1
//------------------------------------------
static u8 HeroPushRock1( TASK_TABLE *task, EventOBJ *h_ev, EventOBJ *r_ev )
{
	if( AcmdSetChk(h_ev) || AcmdSetChk(r_ev) )
	{
		return( 0 );
	}

	AcmdEnd( h_ev );
	AcmdEnd( r_ev );

	AcmdSet( h_ev, AcmdCodeStayWalk1((u8)task->work[2]) );
	AcmdSet( r_ev, AcmdCodeWalk05((u8)task->work[2]) );

	FeSetWork[0] = r_ev->gx;
	FeSetWork[1] = r_ev->gy;
	FeSetWork[2] = r_ev->height_pri;
	FeSetWork[3] = ActWork[r_ev->actno].oamData.Priority;
	FldEffSet( FECODE_Hokori );
	
	SePlay( SE_W070 );

	task->work[0]++;

	return( 0 );
}

//------------------------------------------
//	�≟�� 2
//------------------------------------------
static u8 HeroPushRock2( TASK_TABLE *task, EventOBJ *h_ev, EventOBJ *r_ev )
{
	if( AcmdEndChk(h_ev) == 0 || AcmdEndChk(r_ev) == 0 )
	{
		return( 0 );									// 2�̂Ƃ��I����Ă��Ȃ�
	}
	
	AcmdEnd( h_ev );
	AcmdEnd( r_ev );
	
	MyMoveState.mv_pause = 0;
	ResetForceEvent();

	DelTask( CheckTaskNo(HeroPushRockTask) );
	
	return( 0 );
}

//======================================================================
//		�ެ���ݸ�ϯ�
//======================================================================
static void HeroJumpingMatTask( u8 );
static u8 HeroJumpingMat0( TASK_TABLE *, EventOBJ * );

//------------------------------------------
//	�ެ���ݸ�ϯ�����ǉ�
//------------------------------------------
static void SetHeroJumpingMatTask( void )
{
	u8 i;
	
	i = AddTask( HeroJumpingMatTask, 0xff );
	HeroJumpingMatTask( i );
}

//------------------------------------------
//	�ެ���ݸ�ϯ����ð���
//------------------------------------------
static u8 (* const HeroJumpingMatTaskTbl[])( TASK_TABLE *, EventOBJ * ) =
{
	HeroJumpingMat0,
};

//------------------------------------------
//	�ެ���ݸ����
//------------------------------------------
static void HeroJumpingMatTask( u8 no )
{
	while( HeroJumpingMatTaskTbl[TaskTable[no].work[0]]
				(&TaskTable[no],&EvObj[MyMoveState.evobjno]) );
}

//------------------------------------------
//	�ެ���ݸ� 0
//------------------------------------------
static u8 HeroJumpingMat0( TASK_TABLE *task, EventOBJ *ev )
{
	MyMoveState.mv_pause = 1;

	if( AcmdEnd(ev) )
	{
		SePlay( SE_DANSA );
		AcmdSet( ev, AcmdCodeStayJump(ev->site) );
		task->work[1]++;
	
		if( task->work[1] >= 2 )
		{
			MyMoveState.mv_pause = 0;
			MyMoveState.req_flag |= MY_MV_BIT_UNDER;
			DelTask( CheckTaskNo(HeroJumpingMatTask) );
		}
	}

	return( 0 );
}

//======================================================================
//		�������ϯ�
//======================================================================
static void HeroTurnMatTask( u8 );
static u8 HeroTurnMat0( TASK_TABLE *, EventOBJ * );
static u8 HeroTurnMat1( TASK_TABLE *, EventOBJ * );
static u8 HeroTurnMat2( TASK_TABLE *, EventOBJ * );
static u8 HeroTurnMat3( TASK_TABLE *, EventOBJ * );

//------------------------------------------
//	�������ϯ�����ǉ�
//------------------------------------------
static void SetHeroTurnMatTask( void )
{
	u8 i;
	
	i = AddTask( HeroTurnMatTask, 0xff );
	HeroTurnMatTask( i );
}

//------------------------------------------
//	�������ϯ����ð���
//------------------------------------------
static u8 (* const HeroTurnMatTaskTbl[])( TASK_TABLE *, EventOBJ * ) =
{
	HeroTurnMat0,
	HeroTurnMat1,
	HeroTurnMat2,
	HeroTurnMat3,
};

//------------------------------------------
//	����������
//------------------------------------------
static void HeroTurnMatTask( u8 no )
{
	while( HeroTurnMatTaskTbl[TaskTable[no].work[0]]
			(&TaskTable[no],&EvObj[MyMoveState.evobjno]) );
}

//------------------------------------------
//	������� 0
//------------------------------------------
static u8 HeroTurnMat0( TASK_TABLE *task, EventOBJ *ev )
{
	task->work[0]++;
	task->work[1] = ev->mv_site;
	MyMoveState.mv_pause = 1;
	SetForceEvent();
	SePlay( SE_TK_WARPIN );

	return( 1 );
}

//------------------------------------------
//	������� 1
//------------------------------------------
static u8 HeroTurnMat1( TASK_TABLE *task, EventOBJ *ev )
{
	u8 site;
	u8 site_tbl[] = { SITE_L, SITE_R, SITE_U, SITE_D };
	
	if( AcmdEnd(ev) == 0 )
	{
		return( 0 );
	}
	
	site = site_tbl[ev->mv_site - 1];
	AcmdSet( ev, AcmdCodeStopSite(site) );
	
	if( site == (u8)task->work[1] )						// ���
	{
		task->work[2]++;
	}
	
	task->work[0]++;

	if( task->work[2] >= 4 &&							// �l�� & �����ꏊ
		site == SiteOpposite(task->work[1]) )
	{
		task->work[0]++;
	}
	
	return( 0 );
}

//------------------------------------------
//	������� 2
//------------------------------------------
static u8 HeroTurnMat2( TASK_TABLE *task, EventOBJ *ev )
{
	u8 wait[5] = { AC_WAIT_1, AC_WAIT_1, AC_WAIT_2, AC_WAIT_4, AC_WAIT_8 };

	if( AcmdEnd(ev) == 0 )
	{
		return( 0 );
	}	

	AcmdSet( ev, wait[task->work[2]] );

	task->work[0] = 1;
	
	return( 0 );
}

//------------------------------------------
//	������� 3
//------------------------------------------
static u8 HeroTurnMat3( TASK_TABLE *task, EventOBJ *ev )
{
	if( AcmdEnd(ev) == 0 )
	{
		return( 0 );
	}

	AcmdSet( ev, AcmdCodeWalk05(SiteOpposite(task->work[1])) );

	ResetForceEvent();
	MyMoveState.mv_pause = 0;
//	MyMoveState.req_flag |= MY_MV_BIT_UNDER;			// ϯĔ�����̱���ޭ�Ăɔ���
	DelTask( CheckTaskNo(HeroTurnMatTask) );

	return( 0 );
}

//======================================================================
//		�g���I��
//======================================================================
static void HeroSwimEnd0( u8 );
static void HeroSwimEnd1( u8 );

//------------------------------------------
//	�g���I��������
//------------------------------------------
static void SetHeroSwimEndTask( u8 site )
{
	u8 i;

	SetForceEvent();
	MapBGM_ClearSpecial();
	MapBGM_FadeNowSet();
	MyMoveState.flag &= ~MY_MV_BIT_SWIM;
	MyMoveState.flag |= MY_MV_BIT_NORMAL;
	MyMoveState.mv_pause = 1;

	i = AddTask( HeroSwimEnd0, 0xff );
	TaskTable[i].work[0] = site;
	HeroSwimEnd0( i );
}

//------------------------------------------
//	�g���I�� 0
//------------------------------------------
static void HeroSwimEnd0( u8 no )
{
	EventOBJ *ev;
	
	ev = &EvObj[MyMoveState.evobjno];

	if( AcmdSetChk(ev) && AcmdEnd(ev) == 0 )
	{
		return;
	}

	SetPokeSwimMoveFlag( ev->joint_actno[0], NAMIPOKE_NOWITH_SWAY );

	AcmdSet( ev, AcmdCodeSwimJump((u8)TaskTable[no].work[0]) );
	TaskTable[no].TaskAdrs = HeroSwimEnd1;
}

//------------------------------------------
//	�g���I�� 1
//------------------------------------------
static void HeroSwimEnd1( u8 no )
{
	EventOBJ *ev;

	ev = &EvObj[MyMoveState.evobjno];

	if( AcmdEnd(ev) == 0 )
	{
		return;
	}

	EvObjChgParam( ev, GetMainHeroObjCode(H_NORMAL) );
	AcmdSet( ev, AcmdCodeStopSite(ev->site) );

	MyMoveState.mv_pause = 0;
	ResetForceEvent();

	DelActor( &ActWork[ev->joint_actno[0]] );
	DelTask( no );
}

//======================================================================
//		��
//======================================================================
static void HeroFishingTask( u8 );
static u8 HeroFishing0( TASK_TABLE * );
static u8 HeroFishing1( TASK_TABLE * );
static u8 HeroFishing2( TASK_TABLE * );
static u8 HeroFishing3( TASK_TABLE * );
static u8 HeroFishing4( TASK_TABLE * );
static u8 HeroFishing5( TASK_TABLE * );
static u8 HeroFishing6( TASK_TABLE * );
static u8 HeroFishing7( TASK_TABLE * );
static u8 HeroFishing8( TASK_TABLE * );
static u8 HeroFishing9( TASK_TABLE * );
static u8 HeroFishing10( TASK_TABLE * );
static u8 HeroFishing11( TASK_TABLE * );
static u8 HeroFishing12( TASK_TABLE * );
static u8 HeroFishing13( TASK_TABLE * );
static u8 HeroFishing14( TASK_TABLE * );
static u8 HeroFishing15( TASK_TABLE * );
static void HeroFishingOffset( void );
static const u8 fishing_kakattuta_msg[];
static const u8 fishing_turenai_msg[];
static const u8 fishing_nigerare_msg[];

enum
{
	FISH_START = 0,
	FISH_INIT,
	FISH_SAO_FURI_WAIT,
	FISH_WAIT_INIT,
	FISH_WAIT,
	FISH_HIT_CHECK,
	FISH_HIT,
	FISH_PUSH_A,
	FISH_AWASE_CHECK,
	FISH_ENCOUNT_START,
	FISH_ENCOUNT,
	FISH_TURENAI_SET,
	FISH_NIGASITA_SET,
	FISH_NOHIT_WINDOW_SET,
	FISH_NOHIT_END_SET,
	FISH_NOHIT_END,
};

//------------------------------------------
//	�ނ�����ǉ�
//	In:		sao				; �ނ�Ƃ̎��
//------------------------------------------
void SetHeroFishingTask( u8 sao )
{
	u8 i;

	i = AddTask( HeroFishingTask, 0xff );

	TaskTable[i].work[15] = sao;
	HeroFishingTask( i );
}

//------------------------------------------
//	�ނ�ð���
//------------------------------------------
static u8 (* const HeroFishingTaskTbl[])( TASK_TABLE * ) =
{
	HeroFishing0,
	HeroFishing1,
	HeroFishing2,
	HeroFishing3,
	HeroFishing4,
	HeroFishing5,
	HeroFishing6,
	HeroFishing7,
	HeroFishing8,
	HeroFishing9,
	HeroFishing10,
	HeroFishing11,
	HeroFishing12,
	HeroFishing13,
	HeroFishing14,
	HeroFishing15,
};

//------------------------------------------
//	�ނ����
//------------------------------------------
static void HeroFishingTask( u8 no )
{
	while( HeroFishingTaskTbl[TaskTable[no].work[0]](&TaskTable[no]) );
}

//------------------------------------------
//	�ނ� 0
//------------------------------------------
static u8 HeroFishing0( TASK_TABLE *task )
{
	SetForceEvent();
	MyMoveState.mv_pause = 1;
	task->work[0]++;

	return( 0 );
}

//------------------------------------------
//	�ނ� 1
//------------------------------------------
static u8 HeroFishing1( TASK_TABLE *task )
{
	EventOBJ *ev;
	s16 below[3] = { 1, 1, 1 };
	s16 mod[3] = { 1, 3, 6 };

	task->work[12] = 0;
	task->work[13] = below[task->work[15]] + (pp_rand() % mod[task->work[15]]);
	task->work[14] = EvObj[MyMoveState.evobjno].obj_code;

	ev = &EvObj[MyMoveState.evobjno];
	AcmdFree( ev );
	ev->anm_pause_clr_sw = 1;
	ChangeHeroActorFishing( ev->site );

	task->work[0]++;
	return( 0 );
}

//------------------------------------------
//	�ނ� 2
//------------------------------------------
static u8 HeroFishing2( TASK_TABLE *task )
{
	HeroFishingOffset();

	if( ++task->work[1] >= 60 )
	{
		task->work[0]++;
	}

	return( 0 );
}

//------------------------------------------
//	�ނ� 3
//------------------------------------------
static u8 HeroFishing3( TASK_TABLE *task )
{
	NMenuTalkWinWrite();

	task->work[0]++;
	task->work[1] = 0;
	task->work[2] = 0;
	task->work[3] = (pp_rand() % 10) + 1;				// ����
	if( task->work[12] == 0 ) task->work[3] += 3;		// ���� �Œ�l
	if( task->work[3] >= 10 ) task->work[3] = 10;

	return( 1 );
}

//------------------------------------------
//	�ނ� 4
//------------------------------------------
static u8 HeroFishing4( TASK_TABLE *task )
{
	u8 font[] = { nakag_, EOM_ };

	HeroFishingOffset();

	task->work[1]++;

	if( sys.Trg & A_BUTTON )
	{
		task->work[0] = FISH_TURENAI_SET;
		if( task->work[12] ) task->work[0]++;
		return( 1 );
	}

	if( task->work[1] >= 20 )
	{
		task->work[1] = 0;

		if( task->work[2] >= task->work[3] )
		{
			task->work[0]++;
			if( task->work[12] ) task->work[0]++;
			task->work[12]++;
		}
		else
		{
			NMenuMsgWrite( font, 0x04 + task->work[2], 0x0f );
			task->work[2]++;							// ����
		}
	}

	return( 0 );
}

//------------------------------------------
//	�ނ� 5	����
//------------------------------------------
static u8 HeroFishing5( TASK_TABLE *task )
{
	HeroFishingOffset();

	task->work[0]++;

	if( FishingEncount2() == 0 || (pp_rand() & 0x01) )
	{
		task->work[0] = FISH_TURENAI_SET;
	}
	else
	{
		ActAnmChg( &ActWork[MyMoveState.actno], EvActAnmNoFishingHit(GetHeroSite()) );
	}

	return( 1 );
}

//------------------------------------------
//	�ނ� 6
//------------------------------------------
static u8 HeroFishing6( TASK_TABLE *task )
{
	u8 font[] = { hi_,i_,te_,ru_,spc_,hi_,i_,te_,ru_,gyoe_,gyoe_,EOM_ };

	HeroFishingOffset();
	NMenuMsgWrite( font, 0x04, 0x11 );

	task->work[0]++;
	task->work[1] = 0;

	return( 0 );
}

//------------------------------------------
//	�ނ� 7
//------------------------------------------
static u8 HeroFishing7( TASK_TABLE *task )
{
	s16 wait[3] = { 36, 33, 30 };
	HeroFishingOffset();

	task->work[1]++;

	if( task->work[1] >= wait[task->work[15]] )
	{
		task->work[0] = FISH_NIGASITA_SET;
	}
	else if( sys.Trg & A_BUTTON )
	{
		task->work[0]++;
	}

	return( 0 );
}

//------------------------------------------
//	�ނ� 8
//------------------------------------------
static u8 HeroFishing8( TASK_TABLE *task )
{
	s16 hit;
	s16 awase[3][2]	= { { 0, 0 }, { 40, 10 }, { 70, 30 } };

	HeroFishingOffset();

	task->work[0]++;

	if( task->work[12] < task->work[13] )				// ���킹��
	{
		task->work[0] = FISH_WAIT_INIT;
	}
	else
	{
		if( task->work[12] < 2 )
		{
			hit = pp_rand() % 100;

			if( awase[task->work[15]][task->work[12]] > hit )
			{
				task->work[0] = FISH_WAIT_INIT;
			}
		}
	}

	return( 0 );
}

//------------------------------------------
//	�ނ� 9
//------------------------------------------
static u8 HeroFishing9( TASK_TABLE *task )
{
	HeroFishingOffset();

	NMenuTalkMsgSetEasy( fishing_kakattuta_msg );
	NMenuTalkWinWrite();

	task->work[0]++;
	task->work[1] = 0;

	return( 0 );
}

//------------------------------------------
//	�ނ� 10
//------------------------------------------
static u8 HeroFishing10( TASK_TABLE *task )
{
	EventOBJ *ev;

	if( task->work[1] == 0 )
	{
		HeroFishingOffset();
	}

	if( task->work[1] == 0 && NMenuTalkMsgPut() )
	{
		ev = &EvObj[MyMoveState.evobjno];
		EvObjChgParam( ev, (u8)task->work[14] );
		EvObjChgSite( ev, ev->mv_site );

		if( MyMoveState.flag & MY_MV_BIT_SWIM )
		{
			SetPokeSwimOffsetFlag(
				EvObj[MyMoveState.evobjno].joint_actno[0], 
				NAMIPOKE_VOFFSET_OFF, 0 );
		}

		ActWork[MyMoveState.actno].dx = 0;
		ActWork[MyMoveState.actno].dy = 0;
		NMenuScreenClear();

		task->work[1]++;
	}
	else if( task->work[1] )
	{
		MyMoveState.mv_pause = 0;
		ResetForceEvent();
		FishEncountSet( task->work[15] );
		TVFishingCount( 1 );
		DelTask( CheckTaskNo(HeroFishingTask) );
	}

	return( 0 );
}

//------------------------------------------
//	�ނ� 11
//------------------------------------------
static u8 HeroFishing11( TASK_TABLE *task )
{
	HeroFishingOffset();
	ActAnmChg( &ActWork[MyMoveState.actno], EvActAnmNoFishingEnd(GetHeroSite()) );

	NMenuTalkMsgSetEasy( fishing_turenai_msg );
	task->work[0] = FISH_NOHIT_WINDOW_SET;
	return( 1 );
}

//------------------------------------------
//	�ނ� 12
//------------------------------------------
static u8 HeroFishing12( TASK_TABLE *task )
{
	HeroFishingOffset();
	ActAnmChg( &ActWork[MyMoveState.actno], EvActAnmNoFishingEnd(GetHeroSite()) );

	NMenuTalkMsgSetEasy( fishing_nigerare_msg );
	task->work[0]++;
	return( 1 );
}

//------------------------------------------
//	�ނ� 13
//------------------------------------------
static u8 HeroFishing13( TASK_TABLE *task )
{
	HeroFishingOffset();

	NMenuTalkWinWrite();
	task->work[0]++;
	return( 0 );
}

//------------------------------------------
//	�ނ� 14
//------------------------------------------
static u8 HeroFishing14( TASK_TABLE *task )
{
	EventOBJ *ev;

	HeroFishingOffset();

	if( ActWork[MyMoveState.actno].anmend_sw )
	{
		ev = &EvObj[MyMoveState.evobjno];
		EvObjChgParam( ev, (u8)task->work[14] );
		EvObjChgSite( ev, ev->mv_site );

		if( MyMoveState.flag & MY_MV_BIT_SWIM )
		{
			SetPokeSwimOffsetFlag(
				EvObj[MyMoveState.evobjno].joint_actno[0], 
				NAMIPOKE_VOFFSET_OFF, 0 );
		}

		ActWork[MyMoveState.actno].dx = 0;
		ActWork[MyMoveState.actno].dy = 0;
		task->work[0]++;
	}

	return( 0 );
}

//------------------------------------------
//	�ނ� 15
//------------------------------------------
static u8 HeroFishing15( TASK_TABLE *task )
{
	if( NMenuTalkMsgPut() )
	{
		MyMoveState.mv_pause = 0;
		ResetForceEvent();
		EvObjPauseClrAll();
		NMenuScreenClear();
		TVFishingCount( 0 );
		DelTask( CheckTaskNo(HeroFishingTask) );
	}

	return( 0 );
}

//------------------------------------------
//	�ނ莩�@�̾��
//------------------------------------------
static void HeroFishingOffset( void )
{
	u8 no;
	actWork *act;
	
	act = &ActWork[MyMoveState.actno];
	ActAnm( act );
	
	act->dx = 0;
	act->dy = 0;
	no = act->anm_offs;

	if( act->anm_tbl[act->anm_no][no].pat != ACT_ANMEND )
	{
		act->anm_wait++;
	}

	if( act->anm_tbl[act->anm_no][no].pat == ACT_ANMEND )
	{
		no--;
	}
	
	no = act->anm_tbl[act->anm_no][no].pat;

	if( no == 1 || no == 2 || no == 3 )
	{
		act->dx = 8;

		if( GetHeroSite() == SITE_L )
		{
			act->dx = -8;
		}
	}
	
	if( no == 5 )
	{
		act->dy = -8;
	}
	
	if( no == 10 || no == 11 )
	{
		act->dy = 8;
	}
	
	if( MyMoveState.flag & MY_MV_BIT_SWIM )
	{
		SetPokeSwimOffsetFlag(
			EvObj[MyMoveState.evobjno].joint_actno[0], 
			NAMIPOKE_VOFFSET_ON, act->dy );
	}
}

//------------------------------------------
//	�ނ�ү����
//------------------------------------------
static const u8 fishing_kakattuta_msg[] =
{PO_,KE_,MO_,N_,wo_,spc_,tu_,ri_,a_,ge_,ta_,gyoe_,CTRL_,C_TRGWAIT_,EOM_};
static const u8 fishing_turenai_msg[] =
{tu_,re_,na_,i_,na_,a_,nakag_,nakag_,nakag_,CTRL_,C_TRGWAIT_,EOM_};
static const u8 fishing_nigerare_msg[] =
{ni_,ge_,ra_,re_,te_,spc_,si_,ma_,ttu_,ta_,nakag_,nakag_,nakag_,CTRL_,C_TRGWAIT_,EOM_};

//======================================================================
//		���ޯ�
//======================================================================
//----
#ifdef PM_DEBUG
//----
static u8 HeroDebugMove_R( u8 );

//------------------------------------------
//	���ޯ�ވړ� R
//------------------------------------------
u8 HeroDebugMove( u8 site )
{
	if( sys.Cont & R_BUTTON )							// R ����
	{
		return( HeroDebugMove_R(site) );
	}

	return( 0 );
}

//------------------------------------------
//	���ޯ��ړ�	R ����	˯����� ż
//------------------------------------------
static u8 HeroDebugMove_R( u8 site )
{
	if( site == SITE_N )
	{
		HeroAcmdStop( EvObj[MyMoveState.evobjno].mv_site );
		return( 1 );
	}

	if( EvObj[MyMoveState.evobjno].scr_main_sw && CheckMapMove(site) == 0 )
	{
		HeroAcmdStopWalk( site );						// �����ݺ���� 
		return( 1 );
	}

	HeroAcmdWalk4( site );								// 4�ޯĈړ�
	return( 1 );
}
//----
#endif	// PM_DEBUG
//----

