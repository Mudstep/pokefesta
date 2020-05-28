//**********************************************************************
//		evobjmv.c
//		̨����OBJ ����
//**********************************************************************
#define		EVOBJMV_H_PROTO

#include	<agb.h>
#include 	"common.h"
#include	"actor.h"
#include	"actanm.h"
#include	"calctool.h"
#include	"map.h"
#include	"mapbg.h"
#include	"evobj.h"
#include	"jiki.h"
#include	"fieldmap.h"
#include	"map_attr.h"
#include	"task.h"
#include	"seed.h"

//----------------------------------------------------------------------
//		�O���Q��
//----------------------------------------------------------------------
extern	s16	FieldActOX;
extern	s16	FieldActOY;

//----------------------------------------------------------------------
//		define
//----------------------------------------------------------------------
#if 0													// ����ܰ�����
work[0] = 	evobj ����
work[1] =	Ҳݼ��ݽܰ�
work[2] = 	��Ұ��ݼ��ݽܰ�
work[3] =	�������쐧��ܰ�0(���ݽ)
work[7] =	�ێ�ܰ�
#endif

#define PRI_DEBUG			1							// �D�揇�����ޯ��
#define SHORT_GRASS_DEBUG	0							// ON = �Z�����߯đ���
#define DEBUG_HEIGHT		1							// ON = �������ݒ艼�Ώ�
#define EVEFF_DEBUG			0							// evobjeff ̧�ٕ���
#define EVACT_WALK_TEST		1							// �ړ�ý�

#define SEED_ACTWORK		(7)
#define SEED_START_BIT		(0x01)
#define SEED_EFF_BIT		(0x02)
#define SEED_EFF_OFF_BIT	(0x04)
#define SEED_EFF_CLR_BIT	(~SEED_EFF_BIT)

#define SETOBJ_ShortGrassFirst			0x00000001
#define SETOBJ_ShortGrass				0x00000002
#define SETOBJ_LongGrassFirst			0x00000004
#define SETOBJ_LongGrass				0x00000008
#define SETOBJ_Reflect					0x00000010
#define SETOBJ_ReflectIce				0x00000020
#define SETOBJ_Shoal					0x00000040
#define SETOBJ_Foot						0x00000080
#define SETOBJ_FootDeep					0x00000100
#define SETOBJ_Hamon					0x00000200
#define SETOBJ_Pool						0x00000400
#define SETOBJ_DeepSand					0x00000800
#define SETOBJ_JumpEndShortGrass		0x00001000
#define SETOBJ_JumpEndLongGrass			0x00002000
#define SETOBJ_JumpEndPool				0x00004000
#define SETOBJ_JumpEndWater				0x00008000
#define SETOBJ_JumpEndDust				0x00010000
#define SETOBJ_SmallGrass				0x00020000
#define SETOBJ_Onsen					0x00040000
#define SETOBJ_Awa						0x00080000

#define SETOBJ_MAX 20

enum
{
	DIRSITE_TR_TYPE_ALL = 0,
	DIRSITE_TR_TYPE_V,
	DIRSITE_TR_TYPE_H,
	DIRSITE_TR_TYPE_UL,
	DIRSITE_TR_TYPE_UR,
	DIRSITE_TR_TYPE_DL,
	DIRSITE_TR_TYPE_DR,
	DIRSITE_TR_TYPE_LV,
	DIRSITE_TR_TYPE_RV,
	DIRSITE_TR_TYPE_UH,
	DIRSITE_TR_TYPE_DH,
};

//------------------------------------------
//	�̪�� ܰ����
//------------------------------------------
#define EvEffWorkSet4( code, w0, w1, w2, w3 ) \
	FeSetWork[0] = w0; \
	FeSetWork[1] = w1; \
	FeSetWork[2] = w2; \
	FeSetWork[3] = w3; \
	FldEffSet(code)

#define EvEffWorkSet5( code, w0, w1, w2, w3, w4 ) \
	FeSetWork[0] = w0; \
	FeSetWork[1] = w1; \
	FeSetWork[2] = w2; \
	FeSetWork[3] = w3; \
	FeSetWork[4] = w4; \
	FldEffSet(code)

#define EvEffWorkSet8( code, w0, w1, w2, w3, w4, w5, w6, w7 ) \
	FeSetWork[0] = w0; \
	FeSetWork[1] = w1; \
	FeSetWork[2] = w2; \
	FeSetWork[3] = w3; \
	FeSetWork[4] = w4; \
	FeSetWork[5] = w5; \
	FeSetWork[6] = w6; \
	FeSetWork[7] = w7; \
	FldEffSet(code)

//----------------------------------------------------------------------
//		����
//----------------------------------------------------------------------
static const POINT SitePosMoveTbl[];

static void EvObjMoveWorkInit( EventOBJ *, actWork * );
static void EvObjAnmPause( EventOBJ *, actWork * );
static void EvObjAnmPauseClr( EventOBJ *, actWork * );
static void EvObjMoveFirstMapAttr( EventOBJ *, u32 * );
static void EvObjMoveStartMapAttr( EventOBJ *, u32 * );
static void EvObjMoveEndMapAttr( EventOBJ *, u32 * );
static void EvObjMvStartFirst( EventOBJ *, actWork * );
static void EvObjMvStartSecond( EventOBJ *, actWork * );
static void EvObjMvEnd( EventOBJ *, actWork * );
static void EvObjMoveSetObject( EventOBJ *, actWork *, u32 );
static u8 HeightMoveChk( u8, s16, s16 );
static void EvObjHeightSet( EventOBJ *, actWork * );
static void EvObjPriControl( EventOBJ *, actWork * );
static void EvObjMapAttrPri( EventOBJ *, actWork * );

static u8 range_site( s16, s16, s16, s16 );

static void ev_act_move_walk_init( actWork *, u8 , u8 );
static u8 ev_act_move_walk( actWork * );

static void ev_act_move_walk_05_init( actWork *, u8 );
static u8 ev_act_move_walk_05( actWork * );

static void ev_act_move_jump_init( actWork *, u8, u8, u8 );
static u8 ev_act_move_jump( actWork * );
static u8 ev_act_move_jump_05( actWork * );

static void ev_act_move_wait_init( actWork *, s16 );
static u8 ev_act_move_wait( actWork * );

static void ev_act_move_anm_wait_init( actWork *, u8, u8 );
static u8 ev_act_move_anm_wait( actWork * );

static void objanm_walk_start( EventOBJ *, actWork *, u8 );
static void objanm_stop_start( EventOBJ *, actWork *, u8 );

static u32 EvEffWorkSetId( u8, EventOBJ * );
static void EvEffShadeMake( EventOBJ * );
static void EvEffMizutamaMake( EventOBJ *, actWork * );

//======================================================================
//		̨���� OBJ �ړ�����
//======================================================================
static u8 ObjDmyMove( EventOBJ *, actWork * );

static u8 ObjRandomMove( EventOBJ *, actWork * );
static u8 ObjRandomMove0( EventOBJ *, actWork * );
static u8 ObjRandomMove1( EventOBJ *, actWork * );
static u8 ObjRandomMove2( EventOBJ *, actWork * );
static u8 ObjRandomMove3( EventOBJ *, actWork * );
static u8 ObjRandomMove4( EventOBJ *, actWork * );
static u8 ObjRandomMove5( EventOBJ *, actWork * );
static u8 ObjRandomMove6( EventOBJ *, actWork * );

static u8 ObjDirectMove( EventOBJ *, actWork * );
static u8 ObjDirectMove0( EventOBJ *, actWork * );
static u8 ObjDirectMove1( EventOBJ *, actWork * );
static u8 ObjDirectMove2( EventOBJ *, actWork * );
static u8 ObjDirectMove3( EventOBJ *, actWork * );
static u8 ObjDirectMove4( EventOBJ *, actWork * );

static u8 ObjVMove( EventOBJ *, actWork * );
static u8 ObjVMove0( EventOBJ *, actWork * );
static u8 ObjVMove1( EventOBJ *, actWork * );
static u8 ObjVMove2( EventOBJ *, actWork * );
static u8 ObjVMove3( EventOBJ *, actWork * );
static u8 ObjVMove4( EventOBJ *, actWork * );
static u8 ObjVMove5( EventOBJ *, actWork * );
static u8 ObjVMove6( EventOBJ *, actWork * );

static u8 ObjHMove( EventOBJ *, actWork * );
static u8 ObjHMove0( EventOBJ *, actWork * );
static u8 ObjHMove1( EventOBJ *, actWork * );
static u8 ObjHMove2( EventOBJ *, actWork * );
static u8 ObjHMove3( EventOBJ *, actWork * );
static u8 ObjHMove4( EventOBJ *, actWork * );
static u8 ObjHMove5( EventOBJ *, actWork * );
static u8 ObjHMove6( EventOBJ *, actWork * );

static u8 ObjDirectSite( EventOBJ *, actWork * );
static u8 ObjDirectSite0( EventOBJ *, actWork * );
static u8 ObjDirectSite1( EventOBJ *, actWork * );
static u8 ObjDirectSite2( EventOBJ *, actWork * );

static u8 ObjSeed( EventOBJ *, actWork * );
static u8 ObjSeed0( EventOBJ *, actWork * );
static u8 ObjSeed1( EventOBJ *, actWork * );
static u8 ObjSeed2( EventOBJ *, actWork * );
static u8 ObjSeed3( EventOBJ *, actWork * );
static u8 ObjSeed4( EventOBJ *, actWork * );

static u8 ObjDirectVSite( EventOBJ *, actWork * );
static u8 ObjDirectVSite0( EventOBJ *, actWork * );
static u8 ObjDirectVSite1( EventOBJ *, actWork * );
static u8 ObjDirectVSite2( EventOBJ *, actWork * );
static u8 ObjDirectVSite3( EventOBJ *, actWork * );
static u8 ObjDirectVSite4( EventOBJ *, actWork * );

static u8 ObjDirectHSite( EventOBJ *, actWork * );
static u8 ObjDirectHSite0( EventOBJ *, actWork * );
static u8 ObjDirectHSite1( EventOBJ *, actWork * );
static u8 ObjDirectHSite2( EventOBJ *, actWork * );
static u8 ObjDirectHSite3( EventOBJ *, actWork * );
static u8 ObjDirectHSite4( EventOBJ *, actWork * );

static u8 ObjDirectULSite( EventOBJ *, actWork * );
static u8 ObjDirectULSite0( EventOBJ *, actWork * );
static u8 ObjDirectULSite1( EventOBJ *, actWork * );
static u8 ObjDirectULSite2( EventOBJ *, actWork * );
static u8 ObjDirectULSite3( EventOBJ *, actWork * );
static u8 ObjDirectULSite4( EventOBJ *, actWork * );

static u8 ObjDirectURSite( EventOBJ *, actWork * );
static u8 ObjDirectURSite0( EventOBJ *, actWork * );
static u8 ObjDirectURSite1( EventOBJ *, actWork * );
static u8 ObjDirectURSite2( EventOBJ *, actWork * );
static u8 ObjDirectURSite3( EventOBJ *, actWork * );
static u8 ObjDirectURSite4( EventOBJ *, actWork * );

static u8 ObjDirectDLSite( EventOBJ *, actWork * );
static u8 ObjDirectDLSite0( EventOBJ *, actWork * );
static u8 ObjDirectDLSite1( EventOBJ *, actWork * );
static u8 ObjDirectDLSite2( EventOBJ *, actWork * );
static u8 ObjDirectDLSite3( EventOBJ *, actWork * );
static u8 ObjDirectDLSite4( EventOBJ *, actWork * );

static u8 ObjDirectDRSite( EventOBJ *, actWork * );
static u8 ObjDirectDRSite0( EventOBJ *, actWork * );
static u8 ObjDirectDRSite1( EventOBJ *, actWork * );
static u8 ObjDirectDRSite2( EventOBJ *, actWork * );
static u8 ObjDirectDRSite3( EventOBJ *, actWork * );
static u8 ObjDirectDRSite4( EventOBJ *, actWork * );

static u8 ObjDirectLVSite( EventOBJ *, actWork * );
static u8 ObjDirectLVSite0( EventOBJ *, actWork * );
static u8 ObjDirectLVSite1( EventOBJ *, actWork * );
static u8 ObjDirectLVSite2( EventOBJ *, actWork * );
static u8 ObjDirectLVSite3( EventOBJ *, actWork * );
static u8 ObjDirectLVSite4( EventOBJ *, actWork * );

static u8 ObjDirectRVSite( EventOBJ *, actWork * );
static u8 ObjDirectRVSite0( EventOBJ *, actWork * );
static u8 ObjDirectRVSite1( EventOBJ *, actWork * );
static u8 ObjDirectRVSite2( EventOBJ *, actWork * );
static u8 ObjDirectRVSite3( EventOBJ *, actWork * );
static u8 ObjDirectRVSite4( EventOBJ *, actWork * );

static u8 ObjDirectUHSite( EventOBJ *, actWork * );
static u8 ObjDirectUHSite0( EventOBJ *, actWork * );
static u8 ObjDirectUHSite1( EventOBJ *, actWork * );
static u8 ObjDirectUHSite2( EventOBJ *, actWork * );
static u8 ObjDirectUHSite3( EventOBJ *, actWork * );
static u8 ObjDirectUHSite4( EventOBJ *, actWork * );

static u8 ObjDirectDHSite( EventOBJ *, actWork * );
static u8 ObjDirectDHSite0( EventOBJ *, actWork * );
static u8 ObjDirectDHSite1( EventOBJ *, actWork * );
static u8 ObjDirectDHSite2( EventOBJ *, actWork * );
static u8 ObjDirectDHSite3( EventOBJ *, actWork * );
static u8 ObjDirectDHSite4( EventOBJ *, actWork * );

static u8 ObjDirectSpinL( EventOBJ *, actWork * );
static u8 ObjDirectSpinL0( EventOBJ *, actWork * );
static u8 ObjDirectSpinL1( EventOBJ *, actWork * );
static u8 ObjDirectSpinL2( EventOBJ *, actWork * );
static u8 ObjDirectSpinL3( EventOBJ *, actWork * );

static u8 ObjDirectSpinR( EventOBJ *, actWork * );
static u8 ObjDirectSpinR0( EventOBJ *, actWork * );
static u8 ObjDirectSpinR1( EventOBJ *, actWork * );
static u8 ObjDirectSpinR2( EventOBJ *, actWork * );
static u8 ObjDirectSpinR3( EventOBJ *, actWork * );

static u8 ObjRoute2Pos( EventOBJ *, actWork * );
static u8 ObjRoute2Pos0( EventOBJ *, actWork * );
static u8 ObjRoute2Pos1( EventOBJ *, actWork * );
static u8 ObjRoute2Pos2( EventOBJ *, actWork * );
static u8 ObjRoute2Pos3( EventOBJ *, actWork * );

static u8 ObjRouteURLD( EventOBJ *, actWork * );
static u8 ObjRouteURLD1( EventOBJ *, actWork * );

static u8 ObjRouteRLDU( EventOBJ *, actWork * );
static u8 ObjRouteRLDU1( EventOBJ *, actWork * );

static u8 ObjRouteDURL( EventOBJ *, actWork * );
static u8 ObjRouteDURL1( EventOBJ *, actWork * );

static u8 ObjRouteLDUR( EventOBJ *, actWork * );
static u8 ObjRouteLDUR1( EventOBJ *, actWork * );

static u8 ObjRouteULRD( EventOBJ *, actWork * );
static u8 ObjRouteULRD1( EventOBJ *, actWork * );

static u8 ObjRouteLRDU( EventOBJ *, actWork * );
static u8 ObjRouteLRDU1( EventOBJ *, actWork * );

static u8 ObjRouteDULR( EventOBJ *, actWork * );
static u8 ObjRouteDULR1( EventOBJ *, actWork * );

static u8 ObjRouteRDUL( EventOBJ *, actWork * );
static u8 ObjRouteRDUL1( EventOBJ *, actWork * );

static u8 ObjRouteLUDR( EventOBJ *, actWork * );
static u8 ObjRouteLUDR1( EventOBJ *, actWork * );

static u8 ObjRouteUDRL( EventOBJ *, actWork * );
static u8 ObjRouteUDRL1( EventOBJ *, actWork * );

static u8 ObjRouteRLUD( EventOBJ *, actWork * );
static u8 ObjRouteRLUD1( EventOBJ *, actWork * );

static u8 ObjRouteDRLU( EventOBJ *, actWork * );
static u8 ObjRouteDRLU1( EventOBJ *, actWork * );

static u8 ObjRouteRUDL( EventOBJ *, actWork * );
static u8 ObjRouteRUDL1( EventOBJ *, actWork * );

static u8 ObjRouteUDLR( EventOBJ *, actWork * );
static u8 ObjRouteUDLR1( EventOBJ *, actWork * );

static u8 ObjRouteLRUD( EventOBJ *, actWork * );
static u8 ObjRouteLRUD1( EventOBJ *, actWork * );

static u8 ObjRouteDLRU( EventOBJ *, actWork * );
static u8 ObjRouteDLRU1( EventOBJ *, actWork * );

static u8 ObjRouteUL( EventOBJ *, actWork * );
static u8 ObjRouteUL1( EventOBJ *, actWork * );

static u8 ObjRouteDR( EventOBJ *, actWork * );
static u8 ObjRouteDR1( EventOBJ *, actWork * );

static u8 ObjRouteLD( EventOBJ *, actWork * );
static u8 ObjRouteLD1( EventOBJ *, actWork * );

static u8 ObjRouteRU( EventOBJ *, actWork * );
static u8 ObjRouteRU1( EventOBJ *, actWork * );

static u8 ObjRouteUR( EventOBJ *, actWork * );
static u8 ObjRouteUR1( EventOBJ *, actWork * );

static u8 ObjRouteDL( EventOBJ *, actWork * );
static u8 ObjRouteDL1( EventOBJ *, actWork * );

static u8 ObjRouteLU( EventOBJ *, actWork * );
static u8 ObjRouteLU1( EventOBJ *, actWork * );

static u8 ObjRouteRD( EventOBJ *, actWork * );
static u8 ObjRouteRD1( EventOBJ *, actWork * );

static u8 ObjCopyMove( EventOBJ *, actWork * );
static u8 ObjCopyMove0( EventOBJ *, actWork * );
static u8 ObjCopyMove1( EventOBJ *, actWork * );
static u8 ObjCopyMove2( EventOBJ *, actWork * );
static u8 ObjCopyMoveModeOff( EventOBJ *, actWork *, u8, u8 (*)(u8) );
static u8 ObjCopyMoveModeSite( EventOBJ *, actWork *, u8, u8 (*)(u8) );
static u8 ObjCopyMoveModeWalk1( EventOBJ *, actWork *, u8, u8 (*)(u8) );
static u8 ObjCopyMoveModeWalk2( EventOBJ *, actWork *, u8, u8 (*)(u8) );
static u8 ObjCopyMoveModeWalk4( EventOBJ *, actWork *, u8, u8 (*)(u8) );
static u8 ObjCopyMoveModeWalk8( EventOBJ *, actWork *, u8, u8 (*)(u8) );
static u8 ObjCopyMoveModeStayJump( EventOBJ *, actWork *, u8, u8 (*)(u8) );
static u8 ObjCopyMoveModeJump1( EventOBJ *, actWork *, u8, u8 (*)(u8) );
static u8 ObjCopyMoveModeJump2( EventOBJ *, actWork *, u8, u8 (*)(u8) );

static u8 ObjCopyMoveGrass( EventOBJ *, actWork * );
static u8 ObjCopyMoveGrass1( EventOBJ *, actWork * );

static u8 ObjHideWood( EventOBJ *, actWork * );

static u8 ObjHideSand( EventOBJ *, actWork * );
static u8 ObjHideSandMove0( EventOBJ *, actWork * );

static u8 ObjStayWalk1( EventOBJ *, actWork * );
static u8 ObjStayWalk1_0( EventOBJ *, actWork * );
static u8 ObjStayWalk2( EventOBJ *, actWork * );
static u8 ObjStayWalk2_0( EventOBJ *, actWork * );
static u8 ObjStayWalk4( EventOBJ *, actWork * );
static u8 ObjStayWalk4_0( EventOBJ *, actWork * );

static u8 ObjVanishOn( EventOBJ *, actWork * );
static u8 ObjVanishOn0( EventOBJ *, actWork * );
static u8 ObjVanishOn1( EventOBJ *, actWork * );
static u8 ObjVanishOn2( EventOBJ *, actWork * );

//------------------------------------------
//	�ړ�����
//------------------------------------------
void EvObj_DmyMove( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDmyMove );
}

static u8 ObjDmyMove( EventOBJ *ev, actWork *act )
{
	return( 0 );
}

//------------------------------------------
///	����шړ�	����ð���
//------------------------------------------
static const s16 RandomWaitNormal[] = { 32, 64, 96, 128 };
static const s16 RandomWaitSlow[] = { 32, 64, 128, 192 };
static const s16 RandomWaitQuick[] = { 32, 48, 64, 80 };

#define RND_WAIT_N(a)	(RandomWaitNormal[(a)&0x03])
#define RND_WAIT_S(a)	(RandomWaitSlow[(a)&0x03])
#define RND_WAIT_Q(a)	(RandomWaitQuick[(a)&0x03])

//------------------------------------------
//	����шړ�	ð���
//------------------------------------------
static u8 (* const ObjRandomMoveTbl[])(EventOBJ *,actWork *) =
{
	ObjRandomMove0,
	ObjRandomMove1,
	ObjRandomMove2,
	ObjRandomMove3,
	ObjRandomMove4,
	ObjRandomMove5,
	ObjRandomMove6,
};

//------------------------------------------
//	����шړ�	Ҳ�
//------------------------------------------
void EvObj_RandomMove( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRandomMove );
}

//------------------------------------------
//	����шړ�	����
//------------------------------------------
static u8 ObjRandomMove( EventOBJ *ev, actWork *act )
{
	return( ObjRandomMoveTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//�@����шړ�	������
//------------------------------------------
static u8 ObjRandomMove0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	
	return( 1 );
}

//------------------------------------------
//	����шړ�	�������
//------------------------------------------
static u8 ObjRandomMove1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	����шړ�	���ľ��
//------------------------------------------
static u8 ObjRandomMove2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) == 0 )
	{
		return( 0 );
	}

	ev_act_move_wait_init( act, RND_WAIT_N(pp_rand()) );
	act->work[1] = 3;

	return( 1 );
}

//------------------------------------------
//	����шړ�	����
//------------------------------------------
static u8 ObjRandomMove3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	����шړ�	�ړ��������
//------------------------------------------
static u8 ObjRandomMove4( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_D, SITE_U, SITE_L, SITE_R };
	u8 site;

	site = tbl[pp_rand() & 0x03];
	EvObjSiteSet( ev, site );

	act->work[1] = 5;

	if( EvObjMoveChkSite(ev,site) )
	{
		act->work[1] = 1;								// ���Ă֖߂�
	}

	return( 1 );
}

//------------------------------------------
//	����шړ�	�ړ��J�n
//------------------------------------------
static u8 ObjRandomMove5( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeWalk1(ev->mv_site) );
	ev->move_sw = 1;
	act->work[1] = 6;
	
	return( 1 );
}

//------------------------------------------
//	����шړ�	�ړ�
//------------------------------------------
static u8 ObjRandomMove6( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev->move_sw = 0;
		act->work[1] = 1;								// ��~�֖߂�
	}

	return( 0 );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ�����͈�����
//------------------------------------------
static u8 ObjDirectTrainerTypeRangeCheck( EventOBJ *ev )
{
	s16 x,y,x0,y0,x1,y1;

	if( CheckHeroState(MY_MV_BIT_B_DASH) == 0 ||
		(ev->ev_type != OTYPE_TRAINER && ev->ev_type != OTYPE_TRAINER_EYEALL) )
	{
		return( 0 );
	}

	GetHeroCellPos( &x, &y );

	x0 = ev->gx;
	y0 = ev->gy;
	x1 = x0;
	y1 = y0;
	x0 -= ev->ev_work;
	y0 -= ev->ev_work;
	x1 += ev->ev_work;
	y1 += ev->ev_work;

	if( x0 > x || x1 < x || y0 > y || y1 < y )			// �����͈͊O
	{
		return( 0 );
	}

	return( 1 );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ �S����
//------------------------------------------
static u8 GetObjDirectTrainerSiteAll( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	if( rx > ry )										// x���� > y����
	{
		site = SITE_R;
		if( x < 0 ) site = SITE_L;
	}
	else												// y���� > x���� || x == y
	{
		site = SITE_D;
		if( y < 0 ) site = SITE_U;
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ V
//------------------------------------------
static u8 GetObjDirectTrainerSiteV( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = SITE_D;
	if( y < 0 ) site = SITE_U;

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ H
//------------------------------------------
static u8 GetObjDirectTrainerSiteH( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = SITE_R;
	if( x < 0 ) site = SITE_L;

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ �㍶
//------------------------------------------
static u8 GetObjDirectTrainerSiteUL( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = GetObjDirectTrainerSiteAll( x, y, rx, ry );

	if( site == SITE_D )
	{
		site = GetObjDirectTrainerSiteH( x, y, rx, ry );
		if( site == SITE_R ) site = SITE_U;
	}
	else if( site == SITE_R )
	{
		site = GetObjDirectTrainerSiteV( x, y, rx, ry );
		if( site == SITE_D ) site = SITE_U;
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ ��E
//------------------------------------------
static u8 GetObjDirectTrainerSiteUR( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = GetObjDirectTrainerSiteAll( x, y, rx, ry );

	if( site == SITE_D )
	{
		site = GetObjDirectTrainerSiteH( x, y, rx, ry );
		if( site == SITE_L ) site = SITE_U;
	}
	else if( site == SITE_L )
	{
		site = GetObjDirectTrainerSiteV( x, y, rx, ry );
		if( site == SITE_D ) site = SITE_U;
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ ����
//------------------------------------------
static u8 GetObjDirectTrainerSiteDL( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = GetObjDirectTrainerSiteAll( x, y, rx, ry );

	if( site == SITE_U )
	{
		site = GetObjDirectTrainerSiteH( x, y, rx, ry );
		if( site == SITE_R ) site = SITE_D;
	}
	else if( site == SITE_R )
	{
		site = GetObjDirectTrainerSiteV( x, y, rx, ry );
		if( site == SITE_U ) site = SITE_D;
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ ���E
//------------------------------------------
static u8 GetObjDirectTrainerSiteDR( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = GetObjDirectTrainerSiteAll( x, y, rx, ry );

	if( site == SITE_U )
	{
		site = GetObjDirectTrainerSiteH( x, y, rx, ry );
		if( site == SITE_L ) site = SITE_D;
	}
	else if( site == SITE_L )
	{
		site = GetObjDirectTrainerSiteV( x, y, rx, ry );
		if( site == SITE_U ) site = SITE_D;
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ �㉺��
//------------------------------------------
static u8 GetObjDirectTrainerSiteLV( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = GetObjDirectTrainerSiteAll( x, y, rx, ry );

	if( site == SITE_R )
	{
		site = GetObjDirectTrainerSiteV( x, y, rx, ry );
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ �㉺�E
//------------------------------------------
static u8 GetObjDirectTrainerSiteRV( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = GetObjDirectTrainerSiteAll( x, y, rx, ry );

	if( site == SITE_L )
	{
		site = GetObjDirectTrainerSiteV( x, y, rx, ry );
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ �㍶�E
//------------------------------------------
static u8 GetObjDirectTrainerSiteUH( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = GetObjDirectTrainerSiteAll( x, y, rx, ry );

	if( site == SITE_D )
	{
		site = GetObjDirectTrainerSiteH( x, y, rx, ry );
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ �����E
//------------------------------------------
static u8 GetObjDirectTrainerSiteDH( s16 x, s16 y, s16 rx, s16 ry )
{
	u8 site;

	site = GetObjDirectTrainerSiteAll( x, y, rx, ry );

	if( site == SITE_U )
	{
		site = GetObjDirectTrainerSiteH( x, y, rx, ry );
	}

	return( site );
}

//------------------------------------------
//	�����؂�ւ��ڰŰ�����擾ð���
//------------------------------------------
static u8 (* const GetObjDirectTrainerSiteTbl[])( s16, s16, s16, s16 ) =
{
	GetObjDirectTrainerSiteAll,
	GetObjDirectTrainerSiteV,
	GetObjDirectTrainerSiteH,
	GetObjDirectTrainerSiteUL,
	GetObjDirectTrainerSiteUR,
	GetObjDirectTrainerSiteDL,
	GetObjDirectTrainerSiteDR,
	GetObjDirectTrainerSiteLV,
	GetObjDirectTrainerSiteRV,
	GetObjDirectTrainerSiteUH,
	GetObjDirectTrainerSiteDH,
};

//------------------------------------------
//	�����؂�ւ��^�ڰŰ��������
//------------------------------------------
static u8 ObjDirectTrainerTypeSiteCheck( EventOBJ *ev, u8 seq_no )
{
	s16 x,y,rx,ry;

	if( ObjDirectTrainerTypeRangeCheck(ev) == 0 )
	{
		return( SITE_N );
	}

	GetHeroCellPos( &x, &y );
	x = x - ev->gx;
	y = y - ev->gy;
	rx = x;
	ry = y;
	if( rx < 0 ) rx = -rx;
	if( ry < 0 ) ry = -ry;

	return( GetObjDirectTrainerSiteTbl[seq_no](x,y,rx,ry) );
}

//------------------------------------------
//	�����ς�	ð���
//------------------------------------------
static u8 (* const ObjDirectMoveTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectMove0,
	ObjDirectMove1,
	ObjDirectMove2,
	ObjDirectMove3,
	ObjDirectMove4,
};

//------------------------------------------
//	�����ς�	Ҳ�
//------------------------------------------
void EvObj_DirectMove( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectMove );
}
	
//------------------------------------------
//	�����ς�	����
//------------------------------------------
static u8 ObjDirectMove( EventOBJ *ev, actWork *act )
{
	return( ObjDirectMoveTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	�����ς�	������
//------------------------------------------
static u8 ObjDirectMove0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	�����ς�	�������
//------------------------------------------
static u8 ObjDirectMove1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	�����ς�	���ľ��
//------------------------------------------
static u8 ObjDirectMove2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_N(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	�����ς�	����
//------------------------------------------
static u8 ObjDirectMove3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�����ς�	�����ύX
//------------------------------------------
static u8 ObjDirectMove4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_D, SITE_U, SITE_L, SITE_R };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_ALL );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x03];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	�����ړ�	ð���
//------------------------------------------
static u8 (* const ObjVMoveTbl[])(EventOBJ *,actWork *) =
{
	ObjVMove0,
	ObjVMove1,
	ObjVMove2,
	ObjVMove3,
	ObjVMove4,
	ObjVMove5,
	ObjVMove6,
};

//------------------------------------------
//	�����ړ�	Ҳ�
//------------------------------------------
void EvObj_VMove( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjVMove );
}

//------------------------------------------
//	�����ړ�	����
//------------------------------------------
static u8 ObjVMove( EventOBJ *ev, actWork *act )
{
	return( ObjVMoveTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//�@�����ړ�	������
//------------------------------------------
static u8 ObjVMove0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	
	return( 1 );
}

//------------------------------------------
//	�����ړ�	�������
//------------------------------------------
static u8 ObjVMove1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;

	return( 1 );
}

//------------------------------------------
//	�����ړ�	���ľ��
//------------------------------------------
static u8 ObjVMove2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) == 0 )
	{
		return( 0 );
	}

	ev_act_move_wait_init( act, RND_WAIT_N(pp_rand()) );
	act->work[1] = 3;
	
	return( 1 );
}

//------------------------------------------
//	�����ړ�	����
//------------------------------------------
static u8 ObjVMove3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�����ړ�	�ړ��������
//------------------------------------------
static u8 ObjVMove4( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_D, SITE_U };
	u8 site;
	
	site = tbl[pp_rand() & 0x01];
	EvObjSiteSet( ev, site );
	
	act->work[1] = 5;

	if( EvObjMoveChkSite(ev,site) )
	{
		act->work[1] = 1;								// ���Ă֖߂�
	}
		
	return( 1 );
}

//------------------------------------------
//	�����ړ�	�ړ��J�n
//------------------------------------------
static u8 ObjVMove5( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeWalk1(ev->mv_site) );
	ev->move_sw = 1;
	act->work[1] = 6;
	
	return( 1 );
}

//------------------------------------------
//	�����ړ�	�ړ�
//------------------------------------------
static u8 ObjVMove6( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev->move_sw = 0;
		act->work[1] = 1;								// ��~�֖߂�
	}
	
	return( 0 );
}

//------------------------------------------
//	�����ړ�	ð���
//------------------------------------------
static u8 (* const ObjHMoveTbl[])(EventOBJ *,actWork *) =
{
	ObjHMove0,
	ObjHMove1,
	ObjHMove2,
	ObjHMove3,
	ObjHMove4,
	ObjHMove5,
	ObjHMove6,
};

//------------------------------------------
//	�����ړ�	Ҳ�
//------------------------------------------
void EvObj_HMove( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjHMove );
}

//------------------------------------------
//	�����ړ�	����
//------------------------------------------
static u8 ObjHMove( EventOBJ *ev, actWork *act )
{
	return( ObjHMoveTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//�@�����ړ�	������
//------------------------------------------
static u8 ObjHMove0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	
	return( 1 );
}

//------------------------------------------
//	�����ړ�	�������
//------------------------------------------
static u8 ObjHMove1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	
	return( 1 );
}

//------------------------------------------
//	�����ړ�	���ľ��
//------------------------------------------
static u8 ObjHMove2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) == 0 )
	{
		return( 0 );
	}
	
	ev_act_move_wait_init( act, RND_WAIT_N(pp_rand()) );
	act->work[1] = 3;
	
	return( 1 );
}

//------------------------------------------
//	�����ړ�	����
//------------------------------------------
static u8 ObjHMove3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�����ړ�	�ړ��������
//------------------------------------------
static u8 ObjHMove4( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_L, SITE_R };
	u8 site;
	
	site = tbl[pp_rand() & 0x01];
	EvObjSiteSet( ev, site );
	
	act->work[1] = 5;

	if( EvObjMoveChkSite(ev,site) )
	{
		act->work[1] = 1;								// ���Ă֖߂�
	}
		
	return( 1 );
}

//------------------------------------------
//	�����ړ�	�ړ��J�n
//------------------------------------------
static u8 ObjHMove5( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeWalk1(ev->mv_site) );
	ev->move_sw = 1;
	act->work[1] = 6;
	
	return( 1 );
}

//------------------------------------------
//	�����ړ�	�ړ�
//------------------------------------------
static u8 ObjHMove6( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev->move_sw = 0;
		act->work[1] = 1;								// ��~�֖߂�
	}
	
	return( 0 );
}

//------------------------------------------
//	�Œ����	����ð���
//------------------------------------------
static u8 (* const ObjDirectSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectSite0,
	ObjDirectSite1,
	ObjDirectSite2,
};

//------------------------------------------
//	�Œ����	Ҳ�
//------------------------------------------
void EvObj_DirectSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectSite );
}

//------------------------------------------
//	�Œ����	����
//------------------------------------------
static u8 ObjDirectSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectSiteTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	�Œ����	������
//------------------------------------------
static u8 ObjDirectSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	�Œ����	����
//------------------------------------------
static u8 ObjDirectSite1( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		act->work[1] = 2;
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	�Œ����	��~
//------------------------------------------
static u8 ObjDirectSite2( EventOBJ *ev, actWork *act )
{
	ev->move_sw = 0;
	return( 0 );
}

//------------------------------------------
//	����	ð���
//------------------------------------------
static u8 (* const ObjSeedTbl[])(EventOBJ *,actWork *) =
{
	ObjSeed0,
	ObjSeed1,
	ObjSeed2,
	ObjSeed3,
	ObjSeed4,
};

//------------------------------------------
//	����	Ҳ�
//------------------------------------------
void EvObj_Seed( actWork *act )
{
	EventOBJ *ev;
	
	ev = &EvObj[act->work[0]];

	if( (act->work[SEED_ACTWORK] & SEED_START_BIT) == 0 )
	{
		EvObjSeedSetParam( ev, act );
		act->work[SEED_ACTWORK] |= SEED_START_BIT;
	}

	AcmdBranch( ev, act, ObjSeed );
}

//------------------------------------------
//	����	����
//------------------------------------------
static u8 ObjSeed( EventOBJ *ev, actWork *act )
{
	return( ObjSeedTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//�@����	������
//------------------------------------------
static u8 ObjSeed0( EventOBJ *ev, actWork *act )
{
	u8 speed;

	EvObjMoveWorkInit( ev, act );

	ev->banish_sw = 1;
	act->banish = 1;

	speed = GetSeedGrowth( ev->ev_work );

	if( speed == SEEDSTAT_NOTHING &&
		((act->work[SEED_ACTWORK] & SEED_EFF_OFF_BIT) ||
		act->anm_no != (SEEDSTAT_FRUIT-SEEDSTAT_UNDERGROUND)) )
	{
		return( 0 );
	}

	if( speed == SEEDSTAT_NOTHING )
	{
		EvEffWorkSet4( FECODE_SeedEffect,
			ev->gx, ev->gy, act->pri-1, act->oamData.Priority );

		act->anm_no = 0;
		return( 0 );
	}

	ev->banish_sw = 0;
	act->banish = 0;

	speed -= SEEDSTAT_UNDERGROUND;

	if( act->anm_no != speed )
	{
		act->work[1] = 2;
		return( 1 );
	}

	EvObjSeedSetParam( ev, act );

	CmdActionSet( ev, act, AC_ANM_END_WAIT );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	����	��ҏI���҂�
//------------------------------------------
static u8 ObjSeed1( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		act->work[1] = 0;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	����	�̪�Ĕ���
//------------------------------------------
static u8 ObjSeed2( EventOBJ *ev, actWork *act )
{
	ev->move_sw = 1;
	act->work[1] = 3;
	act->work[2] = 0;
	act->work[SEED_ACTWORK] |= SEED_EFF_BIT;

	EvEffWorkSet4( FECODE_SeedEffect,
		ev->gx, ev->gy, act->pri-1, act->oamData.Priority );

	return( 1 );
}

//------------------------------------------
//	����	�̪��
//------------------------------------------
static u8 ObjSeed3( EventOBJ *ev, actWork *act )
{
	act->work[2]++;

	ev->banish_sw = (act->work[2] & 0x02) >> 1;
	act->anm_pause = 1;

	if( act->work[2] > 64 )
	{
		EvObjSeedSetParam( ev, act );
		act->work[1] = 4;
		act->work[2] = 0;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	����	�̪��
//------------------------------------------
static u8 ObjSeed4( EventOBJ *ev, actWork *act )
{
	act->work[2]++;

	ev->banish_sw = (act->work[2] & 0x02) >> 1;
	act->anm_pause = 1;

	if( act->work[2] > 64 )
	{
		act->work[1] = 0;
		act->work[SEED_ACTWORK] &= SEED_EFF_CLR_BIT;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�������������	ð���
//------------------------------------------
static u8 (* const ObjDirectVSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectVSite0,
	ObjDirectVSite1,
	ObjDirectVSite2,
	ObjDirectVSite3,
	ObjDirectVSite4,
};

//------------------------------------------
//	�������������	Ҳ�
//------------------------------------------
void EvObj_DirectVSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectVSite );
}

//------------------------------------------
//	�������������	����
//------------------------------------------
static u8 ObjDirectVSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectVSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	�������������	������
//------------------------------------------
static u8 ObjDirectVSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	�������������	�������
//------------------------------------------
static u8 ObjDirectVSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	�������������	���ľ��
//------------------------------------------
static u8 ObjDirectVSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_N(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	�������������	����
//------------------------------------------
static u8 ObjDirectVSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�������������	�����ύX
//------------------------------------------
static u8 ObjDirectVSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_D, SITE_U };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_V );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x01];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	�������������	ð���
//------------------------------------------
static u8 (* const ObjDirectHSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectHSite0,
	ObjDirectHSite1,
	ObjDirectHSite2,
	ObjDirectHSite3,
	ObjDirectHSite4,
};

//------------------------------------------
//	�������������	Ҳ�
//------------------------------------------
void EvObj_DirectHSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectHSite );
}

//------------------------------------------
//	�������������	����
//------------------------------------------
static u8 ObjDirectHSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectHSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	�������������	������
//------------------------------------------
static u8 ObjDirectHSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	�������������	�������
//------------------------------------------
static u8 ObjDirectHSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	�������������	���ľ��
//------------------------------------------
static u8 ObjDirectHSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_N(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	�������������	����
//------------------------------------------
static u8 ObjDirectHSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�������������	�����ύX
//------------------------------------------
static u8 ObjDirectHSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_L, SITE_R };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_H );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x01];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	�㍶���������	ð���
//------------------------------------------
static u8 (* const ObjDirectULSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectULSite0,
	ObjDirectULSite1,
	ObjDirectULSite2,
	ObjDirectULSite3,
	ObjDirectULSite4,
};

//------------------------------------------
//	�㍶���������	Ҳ�
//------------------------------------------
void EvObj_DirectULSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectULSite );
}

//------------------------------------------
//	�㍶���������	����
//------------------------------------------
static u8 ObjDirectULSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectULSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	�㍶���������	������
//------------------------------------------
static u8 ObjDirectULSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	�㍶���������	�������
//------------------------------------------
static u8 ObjDirectULSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	�㍶���������	���ľ��
//------------------------------------------
static u8 ObjDirectULSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_Q(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	�㍶���������	����
//------------------------------------------
static u8 ObjDirectULSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�㍶���������	�����ύX
//------------------------------------------
static u8 ObjDirectULSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_U, SITE_L };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_UL );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x01];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	��E���������	ð���
//------------------------------------------
static u8 (* const ObjDirectURSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectURSite0,
	ObjDirectURSite1,
	ObjDirectURSite2,
	ObjDirectURSite3,
	ObjDirectURSite4,
};

//------------------------------------------
//	��E���������	Ҳ�
//------------------------------------------
void EvObj_DirectURSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectURSite );
}

//------------------------------------------
//	��E���������	����
//------------------------------------------
static u8 ObjDirectURSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectURSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	��E���������	������
//------------------------------------------
static u8 ObjDirectURSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	��E���������	�������
//------------------------------------------
static u8 ObjDirectURSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	��E���������	���ľ��
//------------------------------------------
static u8 ObjDirectURSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_Q(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	��E���������	����
//------------------------------------------
static u8 ObjDirectURSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	��E���������	�����ύX
//------------------------------------------
static u8 ObjDirectURSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_U, SITE_R };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_UR );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x01];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	�������������	ð���
//------------------------------------------
static u8 (* const ObjDirectDLSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectDLSite0,
	ObjDirectDLSite1,
	ObjDirectDLSite2,
	ObjDirectDLSite3,
	ObjDirectDLSite4,
};

//------------------------------------------
//	�������������	Ҳ�
//------------------------------------------
void EvObj_DirectDLSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectDLSite );
}

//------------------------------------------
//	�������������	����
//------------------------------------------
static u8 ObjDirectDLSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectDLSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	�������������	������
//------------------------------------------
static u8 ObjDirectDLSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	�������������	�������
//------------------------------------------
static u8 ObjDirectDLSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	�������������	���ľ��
//------------------------------------------
static u8 ObjDirectDLSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_Q(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	�������������	����
//------------------------------------------
static u8 ObjDirectDLSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�������������	�����ύX
//------------------------------------------
static u8 ObjDirectDLSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_D, SITE_L };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_DL );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x01];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	���E���������	ð���
//------------------------------------------
static u8 (* const ObjDirectDRSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectDRSite0,
	ObjDirectDRSite1,
	ObjDirectDRSite2,
	ObjDirectDRSite3,
	ObjDirectDRSite4,
};

//------------------------------------------
//	���E���������	Ҳ�
//------------------------------------------
void EvObj_DirectDRSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectDRSite );
}

//------------------------------------------
//	���E���������	����
//------------------------------------------
static u8 ObjDirectDRSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectDRSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	���E���������	������
//------------------------------------------
static u8 ObjDirectDRSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	���E���������	�������
//------------------------------------------
static u8 ObjDirectDRSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	���E���������	���ľ��
//------------------------------------------
static u8 ObjDirectDRSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_Q(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	���E���������	����
//------------------------------------------
static u8 ObjDirectDRSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	���E���������	�����ύX
//------------------------------------------
static u8 ObjDirectDRSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_D, SITE_R };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_DR );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x01];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	�㉺�����������	ð���
//------------------------------------------
static u8 (* const ObjDirectLVSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectLVSite0,
	ObjDirectLVSite1,
	ObjDirectLVSite2,
	ObjDirectLVSite3,
	ObjDirectLVSite4,
};

//------------------------------------------
//	�㉺�����������	Ҳ�
//------------------------------------------
void EvObj_DirectLVSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectLVSite );
}

//------------------------------------------
//	�㉺�����������	����
//------------------------------------------
static u8 ObjDirectLVSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectLVSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	�㉺�����������	������
//------------------------------------------
static u8 ObjDirectLVSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	�㉺�����������	�������
//------------------------------------------
static u8 ObjDirectLVSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	�㉺�����������	���ľ��
//------------------------------------------
static u8 ObjDirectLVSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_Q(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}
	
	return( 0 );
}

//------------------------------------------
//	�㉺�����������	����
//------------------------------------------
static u8 ObjDirectLVSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�㉺�����������	�����ύX
//------------------------------------------
static u8 ObjDirectLVSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_U, SITE_D, SITE_L, SITE_D };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_LV );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x03];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	�㉺�E���������	ð���
//------------------------------------------
static u8 (* const ObjDirectRVSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectRVSite0,
	ObjDirectRVSite1,
	ObjDirectRVSite2,
	ObjDirectRVSite3,
	ObjDirectRVSite4,
};

//------------------------------------------
//	�㉺�E���������	Ҳ�
//------------------------------------------
void EvObj_DirectRVSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectRVSite );
}

//------------------------------------------
//	�㉺�E���������	����
//------------------------------------------
static u8 ObjDirectRVSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectRVSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	�㉺�E���������	������
//------------------------------------------
static u8 ObjDirectRVSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	�㉺�E���������	�������
//------------------------------------------
static u8 ObjDirectRVSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	�㉺�E���������	���ľ��
//------------------------------------------
static u8 ObjDirectRVSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_Q(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	�㉺�E���������	����
//------------------------------------------
static u8 ObjDirectRVSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�㉺�E���������	�����ύX
//------------------------------------------
static u8 ObjDirectRVSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_D, SITE_U, SITE_R, SITE_D };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_RV );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x03];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	���E����������	ð���
//------------------------------------------
static u8 (* const ObjDirectUHSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectUHSite0,
	ObjDirectUHSite1,
	ObjDirectUHSite2,
	ObjDirectUHSite3,
	ObjDirectUHSite4,
};

//------------------------------------------
//	���E����������	Ҳ�
//------------------------------------------
void EvObj_DirectUHSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectUHSite );
}

//------------------------------------------
//	���E����������	����
//------------------------------------------
static u8 ObjDirectUHSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectUHSiteTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	���E����������	������
//------------------------------------------
static u8 ObjDirectUHSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	���E����������	�������
//------------------------------------------
static u8 ObjDirectUHSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	���E����������	���ľ��
//------------------------------------------
static u8 ObjDirectUHSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_Q(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	���E����������	����
//------------------------------------------
static u8 ObjDirectUHSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	���E����������	�����ύX
//------------------------------------------
static u8 ObjDirectUHSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_U, SITE_L, SITE_R, SITE_U };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_UH );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x03];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	���E�����������	ð���
//------------------------------------------
static u8 (* const ObjDirectDHSiteTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectDHSite0,
	ObjDirectDHSite1,
	ObjDirectDHSite2,
	ObjDirectDHSite3,
	ObjDirectDHSite4,
};

//------------------------------------------
//	���E�����������	Ҳ�
//------------------------------------------
void EvObj_DirectDHSite( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectDHSite );
}

//------------------------------------------
//	���E�����������	����
//------------------------------------------
static u8 ObjDirectDHSite( EventOBJ *ev, actWork *act )
{
	return( ObjDirectDHSiteTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	���E�����������	������
//------------------------------------------
static u8 ObjDirectDHSite0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	���E�����������	�������
//------------------------------------------
static u8 ObjDirectDHSite1( EventOBJ *ev, actWork *act )
{
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 2;
	return( 1 );
}

//------------------------------------------
//	���E�����������	���ľ��
//------------------------------------------
static u8 ObjDirectDHSite2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, RND_WAIT_Q(pp_rand()) );
		ev->move_sw = 0;
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	���E�����������	����
//------------------------------------------
static u8 ObjDirectDHSite3( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 4;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	���E�����������	�����ύX
//------------------------------------------
static u8 ObjDirectDHSite4( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_L, SITE_R, SITE_D, SITE_D };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_DH );

	if( site == SITE_N )
	{
		site = tbl[pp_rand() & 0x03];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	������]����� ð���
//------------------------------------------
static u8 (* const ObjDirectSpinLTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectSpinL0,
	ObjDirectSpinL1,
	ObjDirectSpinL2,
	ObjDirectSpinL3,
};

//------------------------------------------
//	������]����� Ҳ�
//------------------------------------------
void EvObj_DirectSpinL( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectSpinL );
};

//------------------------------------------
//	������]����� ����
//------------------------------------------
static u8 ObjDirectSpinL( EventOBJ *ev, actWork *act )
{
	return( ObjDirectSpinLTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	������]����� ������
//------------------------------------------
static u8 ObjDirectSpinL0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	������]����� ���ľ��
//------------------------------------------
static u8 ObjDirectSpinL1( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, 48 );
		act->work[1] = 2;
	}

	return( 0 );
}

//------------------------------------------
//	������]����� ����
//------------------------------------------
static u8 ObjDirectSpinL2( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	������]����� �������
//------------------------------------------
static u8 ObjDirectSpinL3( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_D,								// ��а
				SITE_R, SITE_L, SITE_D, SITE_U };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_ALL );

	if( site == SITE_N )
	{
		site = tbl[ev->site];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 0;
	
	return( 1 );
}

//------------------------------------------
//	������]�E��� ð���
//------------------------------------------
static u8 (* const ObjDirectSpinRTbl[])(EventOBJ *, actWork *) =
{
	ObjDirectSpinR0,
	ObjDirectSpinR1,
	ObjDirectSpinR2,
	ObjDirectSpinR3,
};

//------------------------------------------
//	������]�E��� Ҳ�
//------------------------------------------
void EvObj_DirectSpinR( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjDirectSpinR );
};

//------------------------------------------
//	������]�E��� ����
//------------------------------------------
static u8 ObjDirectSpinR( EventOBJ *ev, actWork *act )
{
	return( ObjDirectSpinRTbl[act->work[1]]( ev, act ) );
}

//------------------------------------------
//	������]�E��� ������
//------------------------------------------
static u8 ObjDirectSpinR0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	������]�E��� ���ľ��
//------------------------------------------
static u8 ObjDirectSpinR1( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev_act_move_wait_init( act, 48 );
		act->work[1] = 2;
	}

	return( 0 );
}

//------------------------------------------
//	������]�E��� ����
//------------------------------------------
static u8 ObjDirectSpinR2( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_wait(act) || ObjDirectTrainerTypeRangeCheck(ev) )
	{
		act->work[1] = 3;
	}

	return( 0 );
}

//------------------------------------------
//	������]�E��� �������
//------------------------------------------
static u8 ObjDirectSpinR3( EventOBJ *ev, actWork *act )
{
	u8 site;
	u8 tbl[] = { SITE_D,								// ��а
				SITE_L, SITE_R, SITE_U, SITE_D };

	site = ObjDirectTrainerTypeSiteCheck( ev, DIRSITE_TR_TYPE_ALL );

	if( site == SITE_N )
	{
		site = tbl[ev->site];
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 0;

	return( 1 );
}

//------------------------------------------
//	ٰ�2 ð���
//------------------------------------------
static u8 (* const ObjRoute2PosTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute2Pos0,
	ObjRoute2Pos1,
	ObjRoute2Pos2,
	ObjRoute2Pos3,
};

//------------------------------------------
//	ٰ�2 Ҳ�
//------------------------------------------
void EvObj_Route2Pos( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRoute2Pos );
}

//------------------------------------------
//	ٰ�2 ����
//------------------------------------------
static u8 ObjRoute2Pos( EventOBJ *ev, actWork *act )
{
	return( ObjRoute2PosTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�2 ������
//------------------------------------------
static u8 ObjRoute2Pos0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	ٰ�2 ��������
//------------------------------------------
static u8 ObjRoute2Pos1( EventOBJ *ev, actWork *act )
{
	u8 site;

	site = EvObjDefaultSite( ev->mv_code );

	if( ev->work0 )										// �A��
	{
		site = SiteOpposite( site );
	}

	EvObjSiteSet( ev, site );
	act->work[1] = 2;

	return( 1 );
}

//------------------------------------------
//	ٰ�2 ���쾯�
//------------------------------------------
static u8 ObjRoute2Pos2( EventOBJ *ev, actWork *act )
{
	u8 ac,flag;

	if( ev->work0 && ev->init_x == ev->gx && ev->init_y == ev->gy )
	{
		ev->work0 = 0;
		EvObjSiteSet( ev, SiteOpposite(ev->mv_site) );
	}

	flag = EvObjMoveChkSite( ev, ev->mv_site );
	ac = AcmdCodeWalk1( ev->mv_site );

	if( flag == EVOBJHIT_LIMIT )
	{
		ev->work0++;
		EvObjSiteSet( ev, SiteOpposite(ev->mv_site) );
		ac = AcmdCodeWalk1( ev->mv_site );
		flag = EvObjMoveChkSite( ev, ev->mv_site );
	}

	if( flag )
	{
		ac = AcmdCodeStayWalk1( ev->site );
	}

	CmdActionSet( ev, act, ac );
	ev->move_sw = 1;
	act->work[1] = 3;
	
	return( 1 );
}

//------------------------------------------
//	ٰ�2 ����
//------------------------------------------
static u8 ObjRoute2Pos3( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev->move_sw = 0;
		act->work[1] = 1;
	}

	return( 0 );
}

//------------------------------------------
//	ٰ�3 ������
//------------------------------------------
static u8 ObjRoute3Pos0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	ٰ�3 ���
//------------------------------------------
static u8 ObjRoute3PosMoveSet( EventOBJ *ev, actWork *act, u8 *tbl )
{
	u8 ac,flag;
	
	if( ev->work0 == 3 && ev->init_x == ev->gx && ev->init_y == ev->gy )
	{
		ev->work0 = 0;
	}

	EvObjSiteSet( ev, tbl[ev->work0] );

	ac = AcmdCodeWalk1( ev->mv_site );
	flag = EvObjMoveChkSite( ev, ev->mv_site );
	
	if( flag == EVOBJHIT_LIMIT )
	{
		ev->work0++;
		EvObjSiteSet( ev, tbl[ev->work0] );
		ac = AcmdCodeWalk1( ev->mv_site );
		flag = EvObjMoveChkSite( ev, ev->mv_site );
	}

	if( flag )
	{
		ac = AcmdCodeStayWalk1( ev->site );
	}

	CmdActionSet( ev, act, ac );
	ev->move_sw = 1;
	act->work[1] = 2;

	return( 1 );
}

//------------------------------------------
//	ٰ�3 ����
//------------------------------------------
static u8 ObjRoute3Pos2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev->move_sw = 0;
		act->work[1] = 1;
	}

	return( 0 );
}

//------------------------------------------
//	ٰ�3 URLD ð���
//------------------------------------------
static u8 (* const ObjRouteURLDTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteURLD1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 URLD Ҳ�
//------------------------------------------
void EvObj_RouteURLD( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteURLD );
}

//------------------------------------------
//	ٰ�3 URLD ����
//------------------------------------------
static u8 ObjRouteURLD( EventOBJ *ev, actWork *act )
{
	return( ObjRouteURLDTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 URLD ���
//------------------------------------------
static u8 ObjRouteURLD1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_U, SITE_R, SITE_L, SITE_D };

	if( ev->work0 == 2 && ev->init_x == ev->gx )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 RLDU ð���
//------------------------------------------
static u8 (* const ObjRouteRLDUTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteRLDU1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 RLDU Ҳ�
//------------------------------------------
void EvObj_RouteRLDU( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteRLDU );
}

//------------------------------------------
//	ٰ�3 RLDU ����
//------------------------------------------
static u8 ObjRouteRLDU( EventOBJ *ev, actWork *act )
{
	return( ObjRouteRLDUTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 RLDU ���
//------------------------------------------
static u8 ObjRouteRLDU1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_R, SITE_L, SITE_D, SITE_U };

	if( ev->work0 == 1 && ev->init_x == ev->gx )
	{
		ev->work0 = 2;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 DURL ð���
//------------------------------------------
static u8 (* const ObjRouteDURLTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteDURL1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 DURL Ҳ�
//------------------------------------------
void EvObj_RouteDURL( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteDURL );
}

//------------------------------------------
//	ٰ�3 DURL ����
//------------------------------------------
static u8 ObjRouteDURL( EventOBJ *ev, actWork *act )
{
	return( ObjRouteDURLTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 DURL ���
//------------------------------------------
static u8 ObjRouteDURL1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_D, SITE_U, SITE_R, SITE_L };

	if( ev->work0 == 1 && ev->init_y == ev->gy )
	{
		ev->work0 = 2;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 LDUR ð���
//------------------------------------------
static u8 (* const ObjRouteLDURTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteLDUR1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 LDUR Ҳ�
//------------------------------------------
void EvObj_RouteLDUR( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteLDUR );
}

//------------------------------------------
//	ٰ�3 LDUR ����
//------------------------------------------
static u8 ObjRouteLDUR( EventOBJ *ev, actWork *act )
{
	return( ObjRouteLDURTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 LDUR ���
//------------------------------------------
static u8 ObjRouteLDUR1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_L, SITE_D, SITE_U, SITE_R };

	if( ev->work0 == 2 && ev->init_y == ev->gy )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 ULRD ð���
//------------------------------------------
static u8 (* const ObjRouteULRDTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteULRD1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 ULRD Ҳ�
//------------------------------------------
void EvObj_RouteULRD( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteULRD );
}

//------------------------------------------
//	ٰ�3 ULRD ����
//------------------------------------------
static u8 ObjRouteULRD( EventOBJ *ev, actWork *act )
{
	return( ObjRouteULRDTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 ULRD ���
//------------------------------------------
static u8 ObjRouteULRD1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_U, SITE_L, SITE_R, SITE_D };

	if( ev->work0 == 2 && ev->init_x == ev->gx )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 LRDU ð���
//------------------------------------------
static u8 (* const ObjRouteLRDUTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteLRDU1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 LRDU Ҳ�
//------------------------------------------
void EvObj_RouteLRDU( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteLRDU );
}

//------------------------------------------
//	ٰ�3 LRDU ����
//------------------------------------------
static u8 ObjRouteLRDU( EventOBJ *ev, actWork *act )
{
	return( ObjRouteLRDUTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 LRDU ���
//------------------------------------------
static u8 ObjRouteLRDU1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_L, SITE_R, SITE_D, SITE_U };

	if( ev->work0 == 1 && ev->init_x == ev->gx )
	{
		ev->work0 = 2;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 DULR ð���
//------------------------------------------
static u8 (* const ObjRouteDULRTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteDULR1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 DULR Ҳ�
//------------------------------------------
void EvObj_RouteDULR( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteDULR );
}

//------------------------------------------
//	ٰ�3 DULR ����
//------------------------------------------
static u8 ObjRouteDULR( EventOBJ *ev, actWork *act )
{
	return( ObjRouteDULRTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 DULR ���
//------------------------------------------
static u8 ObjRouteDULR1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_D, SITE_U, SITE_L, SITE_R };

	if( ev->work0 == 1 && ev->init_y == ev->gy )
	{
		ev->work0 = 2;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 RDUL ð���
//------------------------------------------
static u8 (* const ObjRouteRDULTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteRDUL1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 RDUL Ҳ�
//------------------------------------------
void EvObj_RouteRDUL( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteRDUL );
}

//------------------------------------------
//	ٰ�3 RDUL ����
//------------------------------------------
static u8 ObjRouteRDUL( EventOBJ *ev, actWork *act )
{
	return( ObjRouteRDULTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 RDUL ���
//------------------------------------------
static u8 ObjRouteRDUL1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_R, SITE_D, SITE_U, SITE_L };

	if( ev->work0 == 2 && ev->init_y == ev->gy )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 LUDR ð���
//------------------------------------------
static u8 (* const ObjRouteLUDRTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteLUDR1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 LUDR Ҳ�
//------------------------------------------
void EvObj_RouteLUDR( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteLUDR );
}

//------------------------------------------
//	ٰ�3 LUDR ����
//------------------------------------------
static u8 ObjRouteLUDR( EventOBJ *ev, actWork *act )
{
	return( ObjRouteLUDRTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 LUDR ���
//------------------------------------------
static u8 ObjRouteLUDR1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_L, SITE_U, SITE_D, SITE_R };

	if( ev->work0 == 2 && ev->init_y == ev->gy )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 UDRL ð���
//------------------------------------------
static u8 (* const ObjRouteUDRLTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteUDRL1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 UDRL Ҳ�
//------------------------------------------
void EvObj_RouteUDRL( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteUDRL );
}

//------------------------------------------
//	ٰ�3 UDRL ����
//------------------------------------------
static u8 ObjRouteUDRL( EventOBJ *ev, actWork *act )
{
	return( ObjRouteUDRLTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 UDRL ���
//------------------------------------------
static u8 ObjRouteUDRL1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_U, SITE_D, SITE_R, SITE_L };

	if( ev->work0 == 1 && ev->init_y == ev->gy )
	{
		ev->work0 = 2;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 RLUD ð���
//------------------------------------------
static u8 (* const ObjRouteRLUDTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteRLUD1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 RLUD Ҳ�
//------------------------------------------
void EvObj_RouteRLUD( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteRLUD );
}

//------------------------------------------
//	ٰ�3 RLUD ����
//------------------------------------------
static u8 ObjRouteRLUD( EventOBJ *ev, actWork *act )
{
	return( ObjRouteRLUDTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 RLUD ���
//------------------------------------------
static u8 ObjRouteRLUD1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_R, SITE_L, SITE_U, SITE_D };

	if( ev->work0 == 1 && ev->init_x == ev->gx )
	{
		ev->work0 = 2;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 DRLU ð���
//------------------------------------------
static u8 (* const ObjRouteDRLUTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteDRLU1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 DRLU Ҳ�
//------------------------------------------
void EvObj_RouteDRLU( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteDRLU );
}

//------------------------------------------
//	ٰ�3 DRLU ����
//------------------------------------------
static u8 ObjRouteDRLU( EventOBJ *ev, actWork *act )
{
	return( ObjRouteDRLUTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 DRLU ���
//------------------------------------------
static u8 ObjRouteDRLU1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_D, SITE_R, SITE_L, SITE_U };

	if( ev->work0 == 2 && ev->init_x == ev->gx )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 RUDL ð���
//------------------------------------------
static u8 (* const ObjRouteRUDLTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteRUDL1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 RUDL Ҳ�
//------------------------------------------
void EvObj_RouteRUDL( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteRUDL );
}

//------------------------------------------
//	ٰ�3 RUDL ����
//------------------------------------------
static u8 ObjRouteRUDL( EventOBJ *ev, actWork *act )
{
	return( ObjRouteRUDLTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 RUDL ���
//------------------------------------------
static u8 ObjRouteRUDL1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_R, SITE_U, SITE_D, SITE_L };

	if( ev->work0 == 2 && ev->init_y == ev->gy )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 UDLR ð���
//------------------------------------------
static u8 (* const ObjRouteUDLRTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteUDLR1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 UDLR Ҳ�
//------------------------------------------
void EvObj_RouteUDLR( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteUDLR );
}

//------------------------------------------
//	ٰ�3 UDLR ����
//------------------------------------------
static u8 ObjRouteUDLR( EventOBJ *ev, actWork *act )
{
	return( ObjRouteUDLRTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 UDLR ���
//------------------------------------------
static u8 ObjRouteUDLR1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_U, SITE_D, SITE_L, SITE_R };

	if( ev->work0 == 1 && ev->init_y == ev->gy )
	{
		ev->work0 = 2;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 LRUD ð���
//------------------------------------------
static u8 (* const ObjRouteLRUDTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteLRUD1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 LRUD Ҳ�
//------------------------------------------
void EvObj_RouteLRUD( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteLRUD );
}

//------------------------------------------
//	ٰ�3 LRUD ����
//------------------------------------------
static u8 ObjRouteLRUD( EventOBJ *ev, actWork *act )
{
	return( ObjRouteLRUDTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 LRUD ���
//------------------------------------------
static u8 ObjRouteLRUD1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_L, SITE_R, SITE_U, SITE_D };

	if( ev->work0 == 1 && ev->init_x == ev->gx )
	{
		ev->work0 = 2;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�3 DLRU ð���
//------------------------------------------
static u8 (* const ObjRouteDLRUTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteDLRU1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�3 DLRU Ҳ�
//------------------------------------------
void EvObj_RouteDLRU( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteDLRU );
}

//------------------------------------------
//	ٰ�3 DLRU ����
//------------------------------------------
static u8 ObjRouteDLRU( EventOBJ *ev, actWork *act )
{
	return( ObjRouteDLRUTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�3 DLRU ���
//------------------------------------------
static u8 ObjRouteDLRU1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_D, SITE_L, SITE_R, SITE_U };

	if( ev->work0 == 2 && ev->init_x == ev->gx )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�4 UL ð���
//------------------------------------------
static u8 (* const ObjRouteULTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteUL1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�4 UL Ҳ�
//------------------------------------------
void EvObj_RouteUL( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteUL );
}

//------------------------------------------
//	ٰ�4 UL ����
//------------------------------------------
static u8 ObjRouteUL( EventOBJ *ev, actWork *act )
{
	return( ObjRouteULTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�4 UL ���
//------------------------------------------
static u8 ObjRouteUL1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_U, SITE_L, SITE_D, SITE_R };

	if( ev->work0 == 2 && ev->init_y == ev->gy )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�4 DR ð���
//------------------------------------------
static u8 (* const ObjRouteDRTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteDR1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�4 DR Ҳ�
//------------------------------------------
void EvObj_RouteDR( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteDR );
}

//------------------------------------------
//	ٰ�4 DR ����
//------------------------------------------
static u8 ObjRouteDR( EventOBJ *ev, actWork *act )
{
	return( ObjRouteDRTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�4 DR ���
//------------------------------------------
static u8 ObjRouteDR1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_D, SITE_R, SITE_U, SITE_L };
	
	if( ev->work0 == 2 && ev->init_y == ev->gy )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�4 LD ð���
//------------------------------------------
static u8 (* const ObjRouteLDTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteLD1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�4 LD Ҳ�
//------------------------------------------
void EvObj_RouteLD( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteLD );
}

//------------------------------------------
//	ٰ�4 LD ����
//------------------------------------------
static u8 ObjRouteLD( EventOBJ *ev, actWork *act )
{
	return( ObjRouteLDTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�4 LD ���
//------------------------------------------
static u8 ObjRouteLD1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_L, SITE_D, SITE_R, SITE_U };
	
	if( ev->work0 == 2 && ev->init_x == ev->gx )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�4 RU ð���
//------------------------------------------
static u8 (* const ObjRouteRUTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteRU1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�4 RU Ҳ�
//------------------------------------------
void EvObj_RouteRU( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteRU );
}

//------------------------------------------
//	ٰ�4 RU ����
//------------------------------------------
static u8 ObjRouteRU( EventOBJ *ev, actWork *act )
{
	return( ObjRouteRUTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�4 RU ���
//------------------------------------------
static u8 ObjRouteRU1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_R, SITE_U, SITE_L, SITE_D };
	
	if( ev->work0 == 2 && ev->init_x == ev->gx )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�4 UR ð���
//------------------------------------------
static u8 (* const ObjRouteURTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteUR1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�4 UR Ҳ�
//------------------------------------------
void EvObj_RouteUR( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteUR );
}

//------------------------------------------
//	ٰ�4 UR ����
//------------------------------------------
static u8 ObjRouteUR( EventOBJ *ev, actWork *act )
{
	return( ObjRouteURTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�4 UR ���
//------------------------------------------
static u8 ObjRouteUR1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_U, SITE_R, SITE_D, SITE_L };
	
	if( ev->work0 == 2 && ev->init_y == ev->gy )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�4 DL ð���
//------------------------------------------
static u8 (* const ObjRouteDLTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteDL1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�4 DL Ҳ�
//------------------------------------------
void EvObj_RouteDL( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteDL );
}

//------------------------------------------
//	ٰ�4 DL ����
//------------------------------------------
static u8 ObjRouteDL( EventOBJ *ev, actWork *act )
{
	return( ObjRouteDLTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�4 DL ���
//------------------------------------------
static u8 ObjRouteDL1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_D, SITE_L, SITE_U, SITE_R };
	
	if( ev->work0 == 2 && ev->init_y == ev->gy )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�4 LU ð���
//------------------------------------------
static u8 (* const ObjRouteLUTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteLU1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�4 LU Ҳ�
//------------------------------------------
void EvObj_RouteLU( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteLU );
}

//------------------------------------------
//	ٰ�4 LU ����
//------------------------------------------
static u8 ObjRouteLU( EventOBJ *ev, actWork *act )
{
	return( ObjRouteLUTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�4 LU ���
//------------------------------------------
static u8 ObjRouteLU1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_L, SITE_U, SITE_R, SITE_D };
	
	if( ev->work0 == 2 && ev->init_x == ev->gx )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	ٰ�4 RD ð���
//------------------------------------------
static u8 (* const ObjRouteRDTbl[])(EventOBJ *, actWork *) =
{
	ObjRoute3Pos0,
	ObjRouteRD1,
	ObjRoute3Pos2,
};

//------------------------------------------
//	ٰ�4 RD Ҳ�
//------------------------------------------
void EvObj_RouteRD( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjRouteRD );
}

//------------------------------------------
//	ٰ�4 RD ����
//------------------------------------------
static u8 ObjRouteRD( EventOBJ *ev, actWork *act )
{
	return( ObjRouteRDTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	ٰ�4 RD ���
//------------------------------------------
static u8 ObjRouteRD1( EventOBJ *ev, actWork *act )
{
	u8 tbl[] = { SITE_R, SITE_D, SITE_L, SITE_U };
	
	if( ev->work0 == 2 && ev->init_x == ev->gx )
	{
		ev->work0 = 3;
	}

	return( ObjRoute3PosMoveSet(ev,act,tbl) );
}

//------------------------------------------
//	���^�� ð���
//------------------------------------------
static u8 (* const ObjCopyMoveTbl[])(EventOBJ *, actWork *) =
{
	ObjCopyMove0,
	ObjCopyMove1,
	ObjCopyMove2,
};

//------------------------------------------
//	���^�� Ҳ�
//------------------------------------------
void EvObj_CopyMove( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjCopyMove );
}

//------------------------------------------
//	���^�� ����
//------------------------------------------
static u8 ObjCopyMove( EventOBJ *ev, actWork *act )
{
	return( ObjCopyMoveTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	���^�� ������
//------------------------------------------
static u8 ObjCopyMove0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	
	if( ev->work0 == SITE_N )
	{
		ev->work0 = GetHeroSite();
	}

	act->work[1] = 1;

	return( 1 );
}

//------------------------------------------
//	���^�� ���ð���
//------------------------------------------
static u8 (* const ObjCopyMoveModeTbl[])( EventOBJ *, actWork *, u8, u8 (*)(u8) ) =
{
	ObjCopyMoveModeOff,
	ObjCopyMoveModeSite,
	ObjCopyMoveModeWalk1,
	ObjCopyMoveModeWalk2,
	ObjCopyMoveModeWalk4,
	ObjCopyMoveModeWalk8,
	ObjCopyMoveModeStayJump,
	ObjCopyMoveModeJump1,
	ObjCopyMoveModeJump2,
	ObjCopyMoveModeOff,
	ObjCopyMoveModeOff,
};

//------------------------------------------
//	���^�� ���
//------------------------------------------
static u8 ObjCopyMove1( EventOBJ *ev, actWork *act )
{
	u8 acmd;

	acmd = EvObj[MyMoveState.evobjno].acmd;

	if( acmd == NOT_ACMD || MyMoveState.mv_state == EV_M_END )
	{
		return( 0 );
	}

	return( ObjCopyMoveModeTbl[HeroAcmdMode()](ev,act,GetHeroMoveSite(),0) );
}

//------------------------------------------
//	���^�� ����
//------------------------------------------
static u8 ObjCopyMove2( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		ev->move_sw = 0;
		act->work[1] = 1;
	}

	return( 0 );
}

//------------------------------------------
//	���^�� OFF,SW,SP
//------------------------------------------
static u8 ObjCopyMoveModeOff( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	return( 0 );
}

//------------------------------------------
//	���^�� SITE
//------------------------------------------
static u8 ObjCopyMoveModeSite( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	site = FrontChgSite( EvObjDefaultSite(ev->mv_code), ev->work0, site );
	CmdActionSet( ev, act, AcmdCodeStopSite(site) );
	ev->move_sw = 1;
	act->work[1] = 2;

	return( 1 );
}

//------------------------------------------
//	���^�� WALK_1
//------------------------------------------
static u8 ObjCopyMoveModeWalk1( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	s16 x,y;

	site = FrontChgSite( EvObjDefaultSite(ev->mv_code), ev->work0, site );
	EvObjSitePosMoveWork( ev, site, &x, &y );

	CmdActionSet( ev, act, AcmdCodeWalk1(site) );

	if( EvObjMoveChk(ev,x,y,site) ||
		(attr_chk && attr_chk(GetCellAttribute(x,y)) == 0) )
	{
		CmdActionSet( ev, act, AcmdCodeStopSite(site) );
	}

	ev->move_sw = 1;
	act->work[1] = 2;
	
	return( 1 );
}

//------------------------------------------
//	���^�� WALK_2
//------------------------------------------
static u8 ObjCopyMoveModeWalk2( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	s16 x,y;

	site = FrontChgSite( EvObjDefaultSite(ev->mv_code), ev->work0, site );
	EvObjSitePosMoveWork( ev, site, &x, &y );

	CmdActionSet( ev, act, AcmdCodeWalk2(site) );

	if( EvObjMoveChk(ev,x,y,site) ||
		(attr_chk && attr_chk(GetCellAttribute(x,y)) == 0) )
	{
		CmdActionSet( ev, act, AcmdCodeStopSite(site) );
	}

	ev->move_sw = 1;
	act->work[1] = 2;
	
	return( 1 );
}

//------------------------------------------
//	���^�� WALK_4
//------------------------------------------
static u8 ObjCopyMoveModeWalk4( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	s16 x,y;

	site = FrontChgSite( EvObjDefaultSite(ev->mv_code), ev->work0, site );
	EvObjSitePosMoveWork( ev, site, &x, &y );

	CmdActionSet( ev, act, AcmdCodeWalk4(site) );

	if( EvObjMoveChk(ev,x,y,site) ||
		(attr_chk && attr_chk(GetCellAttribute(x,y)) == 0) )
	{
		CmdActionSet( ev, act, AcmdCodeStopSite(site) );
	}

	ev->move_sw = 1;
	act->work[1] = 2;
	
	return( 1 );
}

//------------------------------------------
//	���^�� WALK_8
//------------------------------------------
static u8 ObjCopyMoveModeWalk8( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	s16 x,y;

	site = FrontChgSite( EvObjDefaultSite(ev->mv_code), ev->work0, site );
	EvObjSitePosMoveWork( ev, site, &x, &y );

	CmdActionSet( ev, act, AcmdCodeWalk8(site) );

	if( EvObjMoveChk(ev,x,y,site) ||
		(attr_chk && attr_chk(GetCellAttribute(x,y)) == 0) )
	{
		CmdActionSet( ev, act, AcmdCodeStopSite(site) );
	}

	ev->move_sw = 1;
	act->work[1] = 2;
	
	return( 1 );
}

//------------------------------------------
//	���^�� STAY_JUMP
//------------------------------------------
static u8 ObjCopyMoveModeStayJump( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	site = FrontChgSite( EvObjDefaultSite(ev->mv_code), ev->work0, site );
	
	CmdActionSet( ev, act, AcmdCodeStayJump(site) );
	ev->move_sw = 1;
	act->work[1] = 2;

	return( 1 );
}

//------------------------------------------
//	���^�� JUMP_1
//------------------------------------------
static u8 ObjCopyMoveModeJump1( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	s16 x,y;

	site = FrontChgSite( EvObjDefaultSite(ev->mv_code), ev->work0, site );
	EvObjSitePosMoveWork( ev, site, &x, &y );

	CmdActionSet( ev, act, AcmdCodeJump1(site) );

	if( EvObjMoveChk(ev,x,y,site) ||
		(attr_chk && attr_chk(GetCellAttribute(x,y)) == 0) )
	{
		CmdActionSet( ev, act, AcmdCodeStopSite(site) );
	}

	ev->move_sw = 1;
	act->work[1] = 2;
	
	return( 1 );
}

//------------------------------------------
//	���^�� JUMP_2
//------------------------------------------
static u8 ObjCopyMoveModeJump2( EventOBJ *ev, actWork *act, u8 site, u8 (*attr_chk)(u8) )
{
	s16 x,y;

	site = FrontChgSite( EvObjDefaultSite(ev->mv_code), ev->work0, site );
	x = ev->gx;
	y = ev->gy;
	SitePosAdd( site, &x, &y, 2, 2 );

	CmdActionSet( ev, act, AcmdCodeJump2(site) );

	if( EvObjMoveChk(ev,x,y,site) ||
		(attr_chk && attr_chk(GetCellAttribute(x,y)) == 0) )
	{
		CmdActionSet( ev, act, AcmdCodeStopSite(site) );
	}

	ev->move_sw = 1;
	act->work[1] = 2;
	
	return( 1 );
}

//------------------------------------------
//	���^���� ð���
//------------------------------------------
static u8 (* const ObjCopyMoveGrassTbl[])(EventOBJ *, actWork *) =
{
	ObjCopyMove0,
	ObjCopyMoveGrass1,
	ObjCopyMove2,
};

//------------------------------------------
//	���^���� Ҳ�
//------------------------------------------
void EvObj_CopyMoveGrass( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjCopyMoveGrass );
}

//------------------------------------------
//	���^���� ����
//------------------------------------------
static u8 ObjCopyMoveGrass( EventOBJ *ev, actWork *act )
{
	return( ObjCopyMoveGrassTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	���^���� ���
//------------------------------------------
static u8 ObjCopyMoveGrass1( EventOBJ *ev, actWork *act )
{
	u8 acmd;

	acmd = EvObj[MyMoveState.evobjno].acmd;

	if( acmd == NOT_ACMD || MyMoveState.mv_state == EV_M_END )
	{
		return( 0 );
	}

	return( ObjCopyMoveModeTbl[HeroAcmdMode()]
			(ev,act,GetHeroMoveSite(),ATR_IsGrassCheck) );
}

//------------------------------------------
//	�B�ꖪ �� Ҳ�
//------------------------------------------
void EvObj_HideWood( actWork *act )
{
	EventOBJ *ev;

	ev = &EvObj[act->work[0]];
	
	if( ev->work0 == HIDE_NOTHING ||
		(ev->work0 == HIDE_ON && act->work[7] == 0) )
	{
		EvObjIdWorkSet( ev, &FeSetWork[0], &FeSetWork[1], &FeSetWork[2] );
		ev->joint_actno[0] = FldEffSet( FECODE_TrainerHideWood );
		ev->work0 = HIDE_ON;
		act->work[7]++;
	}

	AcmdBranch( &EvObj[act->work[0]], act, ObjHideWood );
}

//------------------------------------------
//	�B�ꖪ �� ����
//------------------------------------------
static u8 ObjHideWood( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	return( 0 );
}

//------------------------------------------
//	�B�ꖪ �� Ҳ�
//------------------------------------------
void EvObj_HideRock( actWork *act )
{
	EventOBJ *ev;

	ev = &EvObj[act->work[0]];

	if( ev->work0 == HIDE_NOTHING ||
		(ev->work0 == HIDE_ON && act->work[7] == 0) )
	{
		EvObjIdWorkSet( ev, &FeSetWork[0], &FeSetWork[1], &FeSetWork[2] );
		ev->joint_actno[0] = FldEffSet( FECODE_TrainerHideRock );
		ev->work0 = HIDE_ON;
		act->work[7]++;
	}
	
	AcmdBranch( &EvObj[act->work[0]], act, ObjHideWood );
}

//------------------------------------------
//	�B�ꖪ �� Ҳ�
//------------------------------------------
void EvObj_HideSand( actWork *act )
{
	EventOBJ *ev;

#if 0
	ev = &EvObj[act->work[0]];

	if( ev->work0 == HIDE_NOTHING ||
		(ev->work0 == HIDE_ON && act->work[7] == 0) )
	{
		EvObjIdWorkSet( ev, &FeSetWork[0], &FeSetWork[1], &FeSetWork[2] );
		ev->joint_actno[0] = FldEffSet( FECODE_TrainerHideSand );
		ev->work0 = HIDE_ON;
		act->work[7]++;
	}
#else
	if( act->work[7] == 0 )
	{
		ev = &EvObj[act->work[0]];
		ev->priority_hold_sw = 1;
		act->oampat_sw = OAMPAT_PRIUNIT;
		act->oamData.Priority = BGPRI3;
		act->work[7]++;
	}
#endif
	
	AcmdBranch( &EvObj[act->work[0]], act, ObjHideSand );
}

//------------------------------------------
//	�B�ꖪ �� ð���
//------------------------------------------
static u8 (* const ObjHideSandMoveTbl[])(EventOBJ *, actWork *) =
{
	ObjHideSandMove0,
};

//------------------------------------------
//	�B�ꖪ �� ����
//------------------------------------------
static u8 ObjHideSand( EventOBJ *ev, actWork *act )
{
	return( ObjHideSandMoveTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	�B�ꖪ �� ������
//------------------------------------------
static u8 ObjHideSandMove0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	return( 0 );
}

//------------------------------------------
//	���̏���� ����
//------------------------------------------
static u8 ObjStayWalk_1( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		act->work[1] = 0;
	}
	
	return( 0 );
}

//------------------------------------------
//	1�ޯĂ��̏��������ð���
//------------------------------------------
static u8 (* const ObjStayWalk1Tbl[])(EventOBJ *, actWork *) =
{
	ObjStayWalk1_0,
	ObjStayWalk_1,
};

//------------------------------------------
//	1�ޯĂ��̏���� Ҳ�
//------------------------------------------
void EvObj_StayWalk1( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjStayWalk1 );
}

//------------------------------------------
//	1�ޯĂ��̏����
//------------------------------------------
static u8 ObjStayWalk1( EventOBJ *ev, actWork *act )
{
	return( ObjStayWalk1Tbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	1�ޯĂ��̏���� ������
//------------------------------------------
static u8 ObjStayWalk1_0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	CmdActionSet( ev, act, AcmdCodeStayWalk1(ev->site) );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	2�ޯĂ��̏��������ð���
//------------------------------------------
static u8 (* const ObjStayWalk2Tbl[])(EventOBJ *, actWork *) =
{
	ObjStayWalk2_0,
	ObjStayWalk_1,
};

//------------------------------------------
//	2�ޯĂ��̏���� Ҳ�
//------------------------------------------
void EvObj_StayWalk2( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjStayWalk2 );
}

//------------------------------------------
//	2�ޯĂ��̏����
//------------------------------------------
static u8 ObjStayWalk2( EventOBJ *ev, actWork *act )
{
	return( ObjStayWalk2Tbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	2�ޯĂ��̏���� ������
//------------------------------------------
static u8 ObjStayWalk2_0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	CmdActionSet( ev, act, AcmdCodeStayWalk2(ev->site) );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	4�ޯĂ��̏��������ð���
//------------------------------------------
static u8 (* const ObjStayWalk4Tbl[])(EventOBJ *, actWork *) =
{
	ObjStayWalk4_0,
	ObjStayWalk_1,
};

//------------------------------------------
//	4�ޯĂ��̏���� Ҳ�
//------------------------------------------
void EvObj_StayWalk4( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjStayWalk4 );
}

//------------------------------------------
//	4�ޯĂ��̏����
//------------------------------------------
static u8 ObjStayWalk4( EventOBJ *ev, actWork *act )
{
	return( ObjStayWalk4Tbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	4�ޯĂ��̏���� ������
//------------------------------------------
static u8 ObjStayWalk4_0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	CmdActionSet( ev, act, AcmdCodeStayWalk4(ev->site) );
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	���� ð���
//------------------------------------------
static u8 (* const ObjVanishOnTbl[])( EventOBJ *, actWork * ) =
{
	ObjVanishOn0,
	ObjVanishOn1,
	ObjVanishOn2,
};

//------------------------------------------
//	���� Ҳ�
//------------------------------------------
void EvObj_Vanish( actWork *act )
{
	AcmdBranch( &EvObj[act->work[0]], act, ObjVanishOn );
}

//------------------------------------------
//	����
//------------------------------------------
static u8 ObjVanishOn( EventOBJ *ev, actWork *act )
{
	return( ObjVanishOnTbl[act->work[1]](ev,act) );
}

//------------------------------------------
//	���� ������
//------------------------------------------
static u8 ObjVanishOn0( EventOBJ *ev, actWork *act )
{
	EvObjMoveWorkInit( ev, act );
	CmdActionSet( ev, act, AcmdCodeStopSite(ev->site) );
	ev->banish_sw = 1;
	act->work[1] = 1;
	return( 1 );
}

//------------------------------------------
//	���� ����
//------------------------------------------
static u8 ObjVanishOn1( EventOBJ *ev, actWork *act )
{
	if( CmdAction(ev,act) )
	{
		act->work[1] = 2;
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	���� ��~
//------------------------------------------
static u8 ObjVanishOn2( EventOBJ *ev, actWork *act )
{
	ev->move_sw = 0;
	return( 0 );
}

//======================================================================
//		���
//======================================================================
//------------------------------------------
//	OBJ ܰ�������
//------------------------------------------
static void EvObjMoveWorkInit( EventOBJ *ev, actWork *act )
{
	ev->move_sw = 0;									// �����׸޸ر
	ev->acmd_sw = 0;									// ����޸ر
	ev->acmd_end_sw = 0;								// ����޴��޸ر
	ev->acmd = NOT_ACMD;								// ɯĺ����
	act->work[1] = 0;									// ���ݽܰ��ر
}

//------------------------------------------
//	���� -> ��~�������ð���
//------------------------------------------
static const u8 EvActAnmNoStopTbl[] =
{
	STOP_D_ST,											// SITE_N
	STOP_D_ST,STOP_U_ST,								// SITE_D, SITE_U
	STOP_L_ST,STOP_R_ST,								// SITE_L, SITE_R
	STOP_D_ST,STOP_D_ST,								// SITE_DL, SITE_DR
	STOP_U_ST,STOP_U_ST									// SITE_UL, SITE_UR
};
	
//------------------------------------------
//	���� -> ��~�������
//------------------------------------------
u8 EvActAnmNoStop( u8 site )
{
	return( EvActAnmNoStopTbl[site] );
}

//------------------------------------------
//	���� -> 1�ޯĈړ��������ð���
//------------------------------------------
static const u8 EvActAnmNoWalk1Tbl[] =
{
	WALK_D_1_ST,										// SITE_N
	WALK_D_1_ST,WALK_U_1_ST,							// SITE_D, SITE_U
	WALK_L_1_ST,WALK_R_1_ST,							// SITE_L, SITE_R
	WALK_D_1_ST,WALK_D_1_ST,							// SITE_DL, SITE_DR
	WALK_U_1_ST,WALK_U_1_ST								// SITE_UL, SITE_UR
};

//------------------------------------------
//	���� -> 1�ޯĈړ��������
//------------------------------------------
u8 EvActAnmNoWalk1( u8 site )
{
	return( EvActAnmNoWalk1Tbl[site] );
}

//------------------------------------------
//	���� -> 2�ޯĈړ��������ð���
//------------------------------------------
static const u8 EvActAnmNoWalk2Tbl[] =
{
	WALK_D_2_ST,										// SITE_N
	WALK_D_2_ST,WALK_U_2_ST,							// SITE_D, SITE_U
	WALK_L_2_ST,WALK_R_2_ST,							// SITE_L, SITE_R
	WALK_D_2_ST,WALK_D_2_ST,							// SITE_DL, SITE_DR
	WALK_U_2_ST,WALK_U_2_ST								// SITE_UL, SITE_UR
};

//------------------------------------------
//	���� -> 2�ޯĈړ��������
//------------------------------------------
u8 EvActAnmNoWalk2( u8 site )
{
	return( EvActAnmNoWalk2Tbl[site] );
}

//------------------------------------------
//	���� -> 4�ޯĈړ��������ð���
//------------------------------------------
static const u8 EvActAnmNoWalk4Tbl[] =
{
	WALK_D_4_ST,										// SITE_N
	WALK_D_4_ST,WALK_U_4_ST,							// SITE_D, SITE_U
	WALK_L_4_ST,WALK_R_4_ST,							// SITE_L, SITE_R
	WALK_D_4_ST,WALK_D_4_ST,							// SITE_DL, SITE_DR
	WALK_U_4_ST,WALK_U_4_ST								// SITE_UL, SITE_UR
};

//------------------------------------------
//	���� -> 4�ޯĈړ��������
//------------------------------------------
u8 EvActAnmNoWalk4( u8 site )
{
	return( EvActAnmNoWalk4Tbl[site] );
}

//------------------------------------------
//	���� -> 8�ޯĈړ��������ð���
//------------------------------------------
static const u8 EvActAnmNoWalk8Tbl[] =
{
	WALK_D_8_ST,										// SITE_N
	WALK_D_8_ST,WALK_U_8_ST,							// SITE_D, SITE_U
	WALK_L_8_ST,WALK_R_8_ST,							// SITE_L, SITE_R
	WALK_D_8_ST,WALK_D_8_ST,							// SITE_DL, SITE_DR
	WALK_U_8_ST,WALK_U_8_ST								// SITE_UL, SITE_UR
};

//------------------------------------------
//	���� -> 8�ޯĈړ��������
//------------------------------------------
u8 EvActAnmNoWalk8( u8 site )
{
	return( EvActAnmNoWalk8Tbl[site] );
}

//------------------------------------------
//	���� -> �j���ެ��߱������ð���
//------------------------------------------
static const u8 EvActAnmNoSwimJumpTbl[] =
{
	SWIM_JUMP_D_ST,
	SWIM_JUMP_D_ST, SWIM_JUMP_U_ST,
	SWIM_JUMP_L_ST, SWIM_JUMP_R_ST,
	SWIM_JUMP_D_ST, SWIM_JUMP_D_ST,
	SWIM_JUMP_U_ST, SWIM_JUMP_U_ST
};

//------------------------------------------
//	���� -> �j���ެ��߱������
//------------------------------------------
u8 EvActAnmNoSwimJump( u8 site )
{
	return( EvActAnmNoSwimJumpTbl[site] );
}

//------------------------------------------
//	���� -> FT ���߱������ð���
//------------------------------------------
static const u8 EvActAnmNoFtUpTbl[] =
{
	CYCLE_S_FTUP_D_ST,
	CYCLE_S_FTUP_D_ST, CYCLE_S_FTUP_U_ST,
	CYCLE_S_FTUP_L_ST, CYCLE_S_FTUP_R_ST,
	CYCLE_S_FTUP_D_ST, CYCLE_S_FTUP_D_ST,
	CYCLE_S_FTUP_U_ST, CYCLE_S_FTUP_U_ST
};

//------------------------------------------
//	���� -> FT ���߱������
//------------------------------------------
u8 EvActAnmNoFtUp( u8 site )
{
	return( EvActAnmNoFtUpTbl[site] );
}

//------------------------------------------
//	���� -> RT ���߱������ð���
//------------------------------------------
static const u8 EvActAnmNoRtUpTbl[] =
{
	CYCLE_S_RTUP_D_ST,
	CYCLE_S_RTUP_D_ST, CYCLE_S_RTUP_U_ST,
	CYCLE_S_RTUP_L_ST, CYCLE_S_RTUP_R_ST,
	CYCLE_S_RTUP_D_ST, CYCLE_S_RTUP_D_ST,
	CYCLE_S_RTUP_U_ST, CYCLE_S_RTUP_U_ST
};

//------------------------------------------
//	���� -> RT ���߱������
//------------------------------------------
u8 EvActAnmNoRtUp( u8 site )
{
	return( EvActAnmNoRtUpTbl[site] );
}

//------------------------------------------
//	���� -> FT �޳ݱ������ð���
//------------------------------------------
static const u8 EvActAnmNoFtDownTbl[] =
{
	CYCLE_S_FTDW_D_ST,
	CYCLE_S_FTDW_D_ST, CYCLE_S_FTDW_U_ST,
	CYCLE_S_FTDW_L_ST, CYCLE_S_FTDW_R_ST,
	CYCLE_S_FTDW_D_ST, CYCLE_S_FTDW_D_ST,
	CYCLE_S_FTDW_U_ST, CYCLE_S_FTDW_U_ST
};

//------------------------------------------
//	���� -> FT �޳ݱ������
//------------------------------------------
u8 EvActAnmNoFtDown( u8 site )
{
	return( EvActAnmNoFtDownTbl[site] );
}

//------------------------------------------
//	���� -> RT �޳ݱ������ð���
//------------------------------------------
static const u8 EvActAnmNoRtDownTbl[] =
{
	CYCLE_S_RTDW_D_ST,
	CYCLE_S_RTDW_D_ST, CYCLE_S_RTDW_U_ST,
	CYCLE_S_RTDW_L_ST, CYCLE_S_RTDW_R_ST,
	CYCLE_S_RTDW_D_ST, CYCLE_S_RTDW_D_ST,
	CYCLE_S_RTDW_U_ST, CYCLE_S_RTDW_U_ST
};

//------------------------------------------
//	���� -> RT �޳ݱ������
//------------------------------------------
u8 EvActAnmNoRtDown( u8 site )
{
	return( EvActAnmNoRtDownTbl[site] );
}

//------------------------------------------
//	���� -> ��ذ�������ð���
//------------------------------------------
static const u8 EvActAnmNoWheelieTbl[] =
{
	CYCLE_S_WHEELIE_D_ST,
	CYCLE_S_WHEELIE_D_ST, CYCLE_S_WHEELIE_U_ST,
	CYCLE_S_WHEELIE_L_ST, CYCLE_S_WHEELIE_R_ST,
	CYCLE_S_WHEELIE_D_ST, CYCLE_S_WHEELIE_D_ST,
	CYCLE_S_WHEELIE_U_ST, CYCLE_S_WHEELIE_U_ST
};

//------------------------------------------
//	���� -> ��ذ�������
//------------------------------------------
u8 EvActAnmNoWheelie( u8 site )
{
	return( EvActAnmNoWheelieTbl[site] );
}

//------------------------------------------
//	���� -> �ނ�J�n�������ð���
//------------------------------------------
static const u8 EvActAnmNoFishingStartTbl[] =
{
	FISH_START_D_ST,
	FISH_START_D_ST, FISH_START_U_ST,
	FISH_START_L_ST, FISH_START_R_ST,
	FISH_START_D_ST, FISH_START_D_ST,
	FISH_START_U_ST, FISH_START_U_ST
};

//------------------------------------------
//	���� -> �ނ�J�n�������
//------------------------------------------
u8 EvActAnmNoFishingStart( u8 site )
{
	return( EvActAnmNoFishingStartTbl[site] );
}

//------------------------------------------
//	���� -> �ނ�I���������ð���
//------------------------------------------
static const u8 EvActAnmNoFishingEndTbl[] =
{
	FISH_END_D_ST,
	FISH_END_D_ST, FISH_END_U_ST,
	FISH_END_L_ST, FISH_END_R_ST,
	FISH_END_D_ST, FISH_END_D_ST,
	FISH_END_U_ST, FISH_END_U_ST
};

//------------------------------------------
//	���� -> �ނ�I���������
//------------------------------------------
u8 EvActAnmNoFishingEnd( u8 site )
{
	return( EvActAnmNoFishingEndTbl[site] );
}

//------------------------------------------
//	���� -> �ނ�˯ı������ð���
//------------------------------------------
static const u8 EvActAnmNoFishingHitTbl[] =
{
	FISH_HIT_D_ST,
	FISH_HIT_D_ST, FISH_HIT_U_ST,
	FISH_HIT_L_ST, FISH_HIT_R_ST,
	FISH_HIT_D_ST, FISH_HIT_D_ST,
	FISH_HIT_U_ST, FISH_HIT_U_ST
};

//------------------------------------------
//	���� -> �ނ�˯ı������
//------------------------------------------
u8 EvActAnmNoFishingHit( u8 site )
{
	return( EvActAnmNoFishingHitTbl[site] );
}

//------------------------------------------
//	���� -> �ޯ���������ð���
//------------------------------------------
static const u8 EvActAnmNoDashTbl[] =
{
	HERO_DASH_D_ST,
	HERO_DASH_D_ST, HERO_DASH_U_ST,
	HERO_DASH_L_ST, HERO_DASH_R_ST,
	HERO_DASH_D_ST, HERO_DASH_D_ST,
	HERO_DASH_U_ST, HERO_DASH_U_ST
};

//------------------------------------------
//	���� -> �ޯ���������
//------------------------------------------
u8 EvActAnmNoDash( u8 site )
{
	return( EvActAnmNoDashTbl[site] );
}

//------------------------------------------
//	�ٱ�ҁ@���암������
//------------------------------------------
static void objanm_walk_start( EventOBJ *ev, actWork *act, u8 anm_no )
{
	if( ev->anm_change_sw == 0 )
	{
		act->anm_no = anm_no;

		if( act->anm_offs == EV_ANMNO_STOP0 )
		{
			act->anm_offs = EV_ANMNO_MOVE1;
		}
		else if( act->anm_offs == EV_ANMNO_STOP1 )
		{
			act->anm_offs = EV_ANMNO_MOVE0;
		}

		ActAnmOffsChg( act, act->anm_offs );
	}
}

//------------------------------------------
//	�ٱ�� ��~��������
//------------------------------------------
static void objanm_stop_start( EventOBJ *ev, actWork *act, u8 anm_no )
{
	u8 offs;
	
	if( ev->anm_change_sw == 0 )
	{
		act->anm_no = anm_no;
		offs = EV_ANMNO_STOP1;

		if( act->anm_offs <= EV_ANMNO_STOP0 )
		{
			offs = EV_ANMNO_STOP0;
		}
	
		ActAnmOffsChg( act, offs );
	}
}

//------------------------------------------
//	�����ԕ���
//------------------------------------------
static u8 range_site( s16 ax, s16 ay, s16 bx, s16 by )
{
	if( ax > bx )	return( SITE_L );
	if( ax < bx )	return( SITE_R );
	if( ay > by )	return( SITE_U );
	
	return( SITE_D );
}

//------------------------------------------
//	���캰�ޕύX
//	In:		ev			; ��� OBJ
//			code		; ���캰��
//------------------------------------------
void EvObjMvCodeChg( EventOBJ *ev, u8 code )
{
	ev->mv_code = code;
	ev->work0 = 0;
	ev->work1 = 0;
	ActWork[ev->actno].move = EvObjMoveTbl[code];
	ActWork[ev->actno].work[1] = 0;
}

//------------------------------------------
//	���� -> ��~���캰��ð���
//------------------------------------------
static const u8 EvObjMvCodeStopSiteTbl[] =
{
	MV_DOWN,
	MV_DOWN,MV_UP,
	MV_LEFT,MV_RIGHT,
	MV_DOWN,MV_DOWN,
	MV_UP,MV_UP
};

//------------------------------------------
//	���� -> ��~���캰��
//------------------------------------------
u8 EvObjMvCodeStopSite( u8 site )
{
	return( EvObjMvCodeStopSiteTbl[site] );
}

//======================================================================
//		�ړ�����
//======================================================================
static u8 lim_check( EventOBJ *, s16, s16 );
static u8 atttype_control_check( EventOBJ *ev, s16, s16, u8 );

//------------------------------------------
//	�ړ����� 1 ��
//	In:		act			; �������
//			site		; ����
//	Out:	u8			; ON = �ړ��s��
//------------------------------------------
u8 EvObjMoveChkSite( EventOBJ *ev, u8 site )
{
	s16 x,y;
	
	x = ev->gx;
	y = ev->gy;
	
	SitePosMove( site, &x, &y );
	
	return( EvObjMoveChk(ev,x,y,site) );
}

//------------------------------------------
//	�ړ�����
//	In:		ev				; ���������OBJ
//			x,y,site		; �ړ���,�ړ�����
//	Out:	0				; �ړ��\
//			EVOBJHIT_LIMIT	; �ړ�����˯�
//			EVOBJHIT_ATR	; ����ޭ��˯�
//			EVOBJHIT_HEIGHT	; ���፷˯�
//			EVOBJHIT_OBJ	; OBJ˯�
//------------------------------------------
u8 EvObjMoveChk( EventOBJ *ev, s16 x, s16 y, u8 site )
{
	if( lim_check(ev,x,y) )								// �ړ�����
	{
		return( EVOBJHIT_LIMIT );
	}

	if( GetCellHitCheck(x,y) )							// �i���\
	{
		return( EVOBJHIT_ATR );
	}

	if( CheckMapLimit(x,y) == NOT_MOVE_VALUE )			// ���Wϯ�ߓ�
	{
		return( EVOBJHIT_ATR );
	}
	
	if( atttype_control_check(ev,x,y,site) )			// �������ޭ������
	{
		return( EVOBJHIT_ATR );
	}

	if( ev->scr_main_sw )
	{
		if( CheckMapMove(site) == 0 )					// ��۰ى\
		{
			return( EVOBJHIT_ATR );
		}
	}
	
	if( HeightMoveChk(ev->height,x,y) )					// ����
	{
		return( EVOBJHIT_HEIGHT );
	}

	if( EvObjMoveHitChk(ev,x,y) )						// OBJ ���m
	{
		return( EVOBJHIT_OBJ );
	}

	return( 0 );
}

//------------------------------------------
//	�ړ���������
//------------------------------------------
static u8 lim_check( EventOBJ *evobj, s16 x, s16 y )
{
	s16	xlim0, ylim0, xlim1, ylim1;

	if( evobj->mvlim_x )								// �ړ������L��
	{
		xlim0 = evobj->init_x - (s16)evobj->mvlim_x;
		xlim1 = evobj->init_x + (s16)evobj->mvlim_x;
		
		if( xlim0 > x || xlim1 < x )
		{
			return( 1 );
		}
	}
	
	if( evobj->mvlim_y )
	{
		ylim0 = evobj->init_y - (s16)evobj->mvlim_y;
		ylim1 = evobj->init_y + (s16)evobj->mvlim_y;
		
		if( ylim0 > y || ylim1 < y )
		{
			return( 1 );
		}
	}

	return( 0 );
}

//------------------------------------------
//	����n����ޭ������ð���
//------------------------------------------
static u8 (* const atttype_control_check_now_tbl[])( u8 ) =
{
	ATR_IsBadMoveDownCheck,								// SITE_D
	ATR_IsBadMoveUpCheck,								// SITE_U
	ATR_IsBadMoveLeftCheck,								// SITE_L
	ATR_IsBadMoveRightCheck,							// SITE_R
};

static u8 (* const atttype_control_check_next_tbl[])( u8 ) =
{
	ATR_IsBadMoveUpCheck,								// SITE_D
	ATR_IsBadMoveDownCheck,								// SITE_U
	ATR_IsBadMoveRightCheck,							// SITE_L
	ATR_IsBadMoveLeftCheck,								// SITE_R
};

//------------------------------------------
//	����n����ޭ������
//------------------------------------------
static u8 atttype_control_check( EventOBJ *ev, s16 x, s16 y, u8 site )
{
	if( atttype_control_check_now_tbl[site-1](ev->now_attr) ||
		atttype_control_check_next_tbl[site-1](GetCellAttribute(x,y)) )
	{
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	OBJ ���m �����蔻��
//	In:		x, y, height; �������W, ����
//	Out:	u8			; ON = ˯�
//	Info:	����1�������̂�
//------------------------------------------
u8 EvObjMoveHitChk( EventOBJ *m_ev, s16 x, s16 y )
{
	u8	i;
	EventOBJ *ev;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		ev = &EvObj[i];

		if( ev->sw && (u32)ev != (u32)m_ev )			// ���� & �����ȊO
		{
			if( ((ev->gx == x && ev->gy == y) ||
				(ev->old_gx == x && ev->old_gy == y)) &&
				HeightHitChk(m_ev->height,ev->height) )
			{
				return( 1 );
			}
		}
	}

	return( 0 );
}

//------------------------------------------
//	����OBJ �̪������
//	In:		objID,mapID,divID	; OBJ,MAP,DIV ID
//	Out:	u8					; ON = �̪�Ē�
//------------------------------------------
u8 EvObjSeedEffectCheck( u8 objID, u8 mapID, u8 divID )
{
	u8 i;

	if( EvObjIdPosGet(objID,mapID,divID,&i) == 0 )
	{
		if( ActWork[EvObj[i].actno].work[SEED_ACTWORK] & SEED_EFF_BIT )
		{
			return( 1 );
		}
	}

	return( 0 );
}

//------------------------------------------
//	����OBJ �̪��OFF
//	In:		objID,mapID,divID	; OBJ,MAP,DIV ID
//------------------------------------------
void EvObjSeedEffectOff( u8 objID, u8 mapID, u8 divID )
{
	u8 i;

	if( EvObjIdPosGet(objID,mapID,divID,&i) == 0 )
	{
		ActWork[EvObj[i].actno].work[SEED_ACTWORK] |= SEED_EFF_OFF_BIT;
	}
}

//======================================================================
//		���W
//======================================================================
//------------------------------------------
//	���W�ړ�ð���
//------------------------------------------
static const POINT SitePosMoveTbl[] =
{
	{0,0},												// SITE_N
	{0,1},{0,-1},{-1,0},{1,0},							// SITE_D,SITE_U,SITE_L,SITE_R
	{-1,1},{1,1},{-1,-1},{1,-1}							// SITE_DL,SITE_DR,SITE_UL,SITE_UR
};				

//------------------------------------------
//	���W�ړ��@�ړ��� 1
//------------------------------------------
void SitePosMove( u8 site, s16 *x, s16 *y )
{
	*x += SitePosMoveTbl[site].x;
	*y += SitePosMoveTbl[site].y;
}

//------------------------------------------
//	���W�ړ��@�ړ��� 16
//------------------------------------------
void SitePosMoveFull( u8 site, s16 *x, s16 *y )
{
	*x += (SitePosMoveTbl[site].x << 4);
	*y += (SitePosMoveTbl[site].y << 4);
}

//------------------------------------------
//	�w������ɉ�����
//------------------------------------------
void SitePosAdd( u8 site, s16 *x, s16 *y, s16 dx, s16 dy )
{
	if( SitePosMoveTbl[site].x > 0 )	*x += dx;
	if( SitePosMoveTbl[site].x < 0 )	*x -= dx;
	if( SitePosMoveTbl[site].y > 0 )	*y += dy;
	if( SitePosMoveTbl[site].y < 0 )	*y -= dy;
}

//------------------------------------------
//	�ٻ���ϯ�ߍ��W�擾
//	In:		mx,my			; ϯ�ߍ��W
//			x,y				; ���W�i�[
//------------------------------------------
void MapCellSizePos( s16 mx, s16 my, s16 *x, s16 *y )
{
	*x = (mx - Fld.MapXpos) << 4;
	*y = (my - Fld.MapYpos) << 4;
	*x -= FieldActOX;
	*y -= FieldActOY;
}

//------------------------------------------
//	�ٻ���ϯ�ߍ��W�擾
//	In:		mx,my			; ϯ�ߍ��W
//			x,y				; ���W�i�[
//	Info:	��۰ُC����
//------------------------------------------
void MapCellSizePosDiff( s16 mx, s16 my, s16 *x, s16 *y )
{
	s16	dx,dy;

	dx = (-FieldActOX) - scroll.xdiff;					// �����C���l - ϯ�߾ٻ��ލ���
	dy = (-FieldActOY) - scroll.ydiff;

	if( scroll.xdiff > 0 ) dx += CELL_SIZE;				// ���� -> �ޯĒP�ʏC���l
	if( scroll.xdiff < 0 ) dx -= CELL_SIZE;
	if( scroll.ydiff > 0 ) dy += CELL_SIZE;
	if( scroll.ydiff < 0 ) dy -= CELL_SIZE;

	*x = ((mx - Fld.MapXpos) << 4) + dx;
	*y = ((my - Fld.MapYpos) << 4) + dy;
}

//------------------------------------------
//	ϯ�ߍ��W�ޯďC�� + �̾��
//	In:		mx,my		; ϯ�ߍ��W�i�[
//			dx,dy		; �̾��
//------------------------------------------
void MapCellSizePosDiffOffs( s16 *mx, s16 *my, s16 dx, s16 dy )
{
	MapCellSizePosDiff( *mx, *my, mx, my );
	*mx += dx;
	*my += dy;
}

//------------------------------------------
//	�ٻ��ޏC�����W�擾
//	In:		x,y				; ���W�i�[
//------------------------------------------
void MapCellSizeDiff( s16 *dx, s16 *dy )
{
	*dx = 0;
	*dy = 0;

	if( scroll.xdiff > 0 ) (*dx)++;						// ���� -> �ْP�ʏC���l
	if( scroll.xdiff < 0 ) (*dx)--;
	if( scroll.ydiff > 0 ) (*dy)++;
	if( scroll.ydiff < 0 ) (*dy)--;
}

//------------------------------------------
//	�����W�ړ�ܰ����
//	In:		ev				; ���OBJ
//			site,x,y,site	; ����,�i�[ܰ�
//------------------------------------------
void EvObjSitePosMoveWork( EventOBJ *ev, u8 site, s16 *x, s16 *y )
{
	*x = ev->gx;
	*y = ev->gy;
	SitePosMove( site, x, y );
}

//======================================================================
//		��Ұ��ݺ����
//======================================================================
//------------------------------------------
//	����޾������
//	In:		ev		; ���� OBJ
//	Out:	u8		; On = ��ĕs��
//------------------------------------------
u8 AcmdSetChk( EventOBJ *ev )
{
	if( ev->acmd_sw || ev->move_sw )					// ����� or ���쒆
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	����ޓ�������
//	In:		ev		; ���� OBJ
//	Out:	u8		; On = ���쒆
//------------------------------------------
u8 AcmdActionChk( EventOBJ *ev )
{
	if( ev->acmd_sw && ev->acmd != NOT_ACMD )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	����޾��
//	In:		ev			; �ݒ� OBJ
//			code		; ���캰��
//	Out:	u8			; ON = ��ĕs��
//------------------------------------------
u8 AcmdSet( EventOBJ *ev, u8 code )
{
	if( AcmdSetChk(ev) )
	{
		return( 1 );									// ��ĕs��
	}

	EvObjPauseClr( ev );
	
	ev->acmd = code;									// ����޺��޾��
	ev->acmd_sw = 1;									// ����� ON
	ev->acmd_end_sw = 0;								// ����� END OFF
	ActWork[ev->actno].work[2] = 0;						// ���ݽܰ��ر

	return( 0 );
}

//------------------------------------------
//	��������޾��
//	In:		ev			; �ݒ� OBJ
//			code		; ���캰��
//------------------------------------------
void AcmdForceSet( EventOBJ *ev, u8 code )
{
	AcmdFree( ev );										// �������
	AcmdSet( ev, code );								// ���
}

//------------------------------------------
//	����މ���
//	In:		ev			; ���� OBJ
//------------------------------------------
void AcmdFree( EventOBJ *ev )
{
	if( ev->acmd_sw )									// ����-��
	{
		AcmdFreeWork( ev );
	}
}

//------------------------------------------
//	�����ܰ��ر
//	In:		ev			; �ر OBJ
//------------------------------------------
void AcmdFreeWork( EventOBJ *ev )
{
	ev->acmd = NOT_ACMD;								// ������
	ev->acmd_sw = 0;									// ����޽���
	ev->acmd_end_sw = 0;								// ����޴���
	ActWork[ev->actno].work[1] = 0;						// Ҳݼ��ݽ
	ActWork[ev->actno].work[2] = 0;						// ����޼��ݽ
}

//------------------------------------------
//	����ޏI������
//	In:		ev			; ���� OBJ
//	Out:	u8			; 1 = �I��
//						; 0 = �p��
//						; EV_OBJ_MAX = ERROR
//------------------------------------------
u8 AcmdEndChk( EventOBJ *ev )
{
	if( ev->acmd_sw )
	{
		return( ev->acmd_end_sw );						// �׸�
	}

	return( EV_OBJ_MAX );								// ��
}

//------------------------------------------
//	����ޏI������ & ���A����
//	In:		ev			; ���� OBJ 
//	Out:	u8			; 1 = �I��
//						; EV_OBJ_MAX = ERROR
//------------------------------------------
u8 AcmdEnd( EventOBJ *ev )
{
	u8 flag;
	
	flag = AcmdEndChk( ev );							// �I������
	
	if( flag && flag != EV_OBJ_MAX )					// �I�� & �Y����
	{
		AcmdFree( ev );									// ܰ�������
	}
	
	return( flag );
}

//------------------------------------------
//	����޺��ނ�Ԃ�
//	In:		ev		; �擾OBJ
//	Out:	u8		; ����
//					; NOT_ACMD = ����ޖ���
//------------------------------------------
u8 AcmdCode( EventOBJ *ev )
{
	if( ev->acmd_sw )
	{	
		return( ev->acmd );
	}
	
	return( NOT_ACMD );
}

//------------------------------------------
//	�����ٰ�ݼ��ݽ
//	In:		ev		; ����� OBJ
//			act		; �t������
//			move	; ����� OBJ ����֐�
//------------------------------------------
void AcmdBranch( EventOBJ *ev, actWork *act, u8 (*move)(EventOBJ*,actWork*) )
{
	EvObjMvStartFirst( ev, act );
	EvObjAnmPauseClr( ev, act );

	if( AcmdActionChk(ev) )
	{
		AcmdAction( ev, act );							// ����ޱ����
	}
	else if( ev->pause_sw == 0 )
	{
		while( move(ev,act) );							// ����֐���
	}

	EvObjMvStartSecond( ev, act );
	EvObjMvEnd( ev, act );
	EvObjAnmPause( ev, act );
	EvObjBanish( ev, act );
	EvObjPriControl( ev, act );
}

//======================================================================
//		��Ұ��ݺ���޺���
//======================================================================
//------------------------------------------
//	���� -> ��������޺���
//------------------------------------------
u8 AcmdCodeStopSite( u8 site )
{
	u8	code[] = { AC_SITE_D,
					AC_SITE_D, AC_SITE_U,
					AC_SITE_L, AC_SITE_R };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
		
	return( code[site] );
}

//------------------------------------------
//	���� -> 0.5�ޯĈړ�����޺���
//------------------------------------------
u8 AcmdCodeWalk05( u8 site )
{
	u8	code[] = { AC_WALK_D_05,
					AC_WALK_D_05, AC_WALK_U_05,
					AC_WALK_L_05, AC_WALK_R_05 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> 1�ޯĈړ�����޺���
//------------------------------------------
u8 AcmdCodeWalk1( u8 site )
{
	u8	code[] = { AC_WALK_D_1,
					AC_WALK_D_1, AC_WALK_U_1,
					AC_WALK_L_1, AC_WALK_R_1 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> 2�ޯĈړ�����޺���
//------------------------------------------
u8 AcmdCodeWalk2( u8 site )
{
	u8	code[] = { AC_WALK_D_2,
					AC_WALK_D_2, AC_WALK_U_2,
					AC_WALK_L_2, AC_WALK_R_2 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> 3�ޯĈړ�����޺���
//------------------------------------------
u8 AcmdCodeWalk3( u8 site )
{
	u8	code[] = { AC_WALK_D_3,
					AC_WALK_D_3, AC_WALK_U_3,
					AC_WALK_L_3, AC_WALK_R_3 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> 4�ޯĈړ�����޺���
//------------------------------------------
u8 AcmdCodeWalk4( u8 site )
{
	u8	code[] = { AC_WALK_D_4,
					AC_WALK_D_4, AC_WALK_U_4,
					AC_WALK_L_4, AC_WALK_R_4 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> 8�ޯĈړ�����޺���
//------------------------------------------
u8 AcmdCodeWalk8( u8 site )
{
	u8	code[] = { AC_WALK_D_8,
					AC_WALK_D_8, AC_WALK_U_8,
					AC_WALK_L_8, AC_WALK_R_8 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> �ޯ��2�ޯĈړ�����޺���
//------------------------------------------
u8 AcmdCodeDash2( u8 site )
{
	u8	code[] = { AC_DASH_D_2,
					AC_DASH_D_2, AC_DASH_U_2,
					AC_DASH_L_2, AC_DASH_R_2 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> 2�ټެ��ߺ���޺���
//------------------------------------------
u8 AcmdCodeJump2( u8 site )
{
	u8	code[] = { AC_JUMP_D_2,
					AC_JUMP_D_2, AC_JUMP_U_2,
					AC_JUMP_L_2, AC_JUMP_R_2 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ���̏�ެ��ߺ���޺���
//------------------------------------------
u8 AcmdCodeStayJump( u8 site )
{
	u8	code[] = { AC_STAY_JUMP_D,
					AC_STAY_JUMP_D, AC_STAY_JUMP_U,
					AC_STAY_JUMP_L, AC_STAY_JUMP_R };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ���̏�ެ��ߐU���������޺���
//------------------------------------------
u8 AcmdCodeStayTurnJump( u8 site )
{
	u8	code[] = { AC_STAY_TURN_JUMP_U,
					AC_STAY_TURN_JUMP_U, AC_STAY_TURN_JUMP_D,
					AC_STAY_TURN_JUMP_R, AC_STAY_TURN_JUMP_L };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> 1�ټެ��ߺ���޺���
//------------------------------------------
u8 AcmdCodeJump1( u8 site )
{
	u8	code[] = { AC_JUMP_D_1,
					AC_JUMP_D_1, AC_JUMP_U_1,
					AC_JUMP_L_1, AC_JUMP_R_1 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> �g���ެ��ߺ���
//------------------------------------------
u8 AcmdCodeSwimJump( u8 site )
{
	u8	code[] = { AC_SWIM_JUMP_D,
					AC_SWIM_JUMP_D, AC_SWIM_JUMP_U,
					AC_SWIM_JUMP_L, AC_SWIM_JUMP_R };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ���̏����0.5�ޯĺ���޺���
//------------------------------------------
u8 AcmdCodeStayWalk05( u8 site )
{
	u8	code[] = { AC_STAY_WALK_D_05,
					AC_STAY_WALK_D_05, AC_STAY_WALK_U_05,
					AC_STAY_WALK_L_05, AC_STAY_WALK_R_05 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ���̏����1�ޯĺ���޺���
//------------------------------------------
u8 AcmdCodeStayWalk1( u8 site )
{
	u8	code[] = { AC_STAY_WALK_D_1,
					AC_STAY_WALK_D_1, AC_STAY_WALK_U_1,
					AC_STAY_WALK_L_1, AC_STAY_WALK_R_1 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ���̏����2�ޯĺ���޺���
//------------------------------------------
u8 AcmdCodeStayWalk2( u8 site )
{
	u8	code[] = { AC_STAY_WALK_D_2,
					AC_STAY_WALK_D_2, AC_STAY_WALK_U_2,
					AC_STAY_WALK_L_2, AC_STAY_WALK_R_2 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ���̏����4�ޯĺ���޺���
//------------------------------------------
u8 AcmdCodeStayWalk4( u8 site )
{
	u8	code[] = { AC_STAY_WALK_D_4,
					AC_STAY_WALK_D_4, AC_STAY_WALK_U_4,
					AC_STAY_WALK_L_4, AC_STAY_WALK_R_4 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	�U�������p����޾��
//------------------------------------------
u8 AcmdCodeTurnSiteSet( EventOBJ *ev, u8 site )
{
	u8 direct;
	u8 code;

	direct = SiteOpposite( site );
	code = AcmdCodeStopSite( direct );
	return( AcmdSet( ev, code ) );
}

//------------------------------------------
//	���� -> ��~���ı��ߺ���޺���
//------------------------------------------
u8 AcmdCodeMtbFtUpSite( u8 site )
{
	u8	code[] = { AC_MTB_FTUP_SITE_D,
					AC_MTB_FTUP_SITE_D, AC_MTB_FTUP_SITE_U,
					AC_MTB_FTUP_SITE_L, AC_MTB_FTUP_SITE_R };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ���ı��ߺ���޺���
//------------------------------------------
u8 AcmdCodeMtbFtUp( u8 site )
{
	u8	code[] = { AC_MTB_FTUP_D,
					AC_MTB_FTUP_D, AC_MTB_FTUP_U,
					AC_MTB_FTUP_L, AC_MTB_FTUP_R };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> �����޳ݺ���޺���
//------------------------------------------
u8 AcmdCodeMtbFtDown( u8 site )
{
	u8	code[] = { AC_MTB_FTDOWN_D,
					AC_MTB_FTDOWN_D, AC_MTB_FTDOWN_U,
					AC_MTB_FTDOWN_L, AC_MTB_FTDOWN_R };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ��ƴټެ��ߺ���޺���
//------------------------------------------
u8 AcmdCodeMtbDaniel( u8 site )
{
	u8	code[] = { AC_MTB_DANIEL_D,
					AC_MTB_DANIEL_D, AC_MTB_DANIEL_U,
					AC_MTB_DANIEL_L, AC_MTB_DANIEL_R };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ��ƴټެ���1����޺���
//------------------------------------------
u8 AcmdCodeMtbDaniel1( u8 site )
{
	u8	code[] = { AC_MTB_DANIEL_D_1,
					AC_MTB_DANIEL_D_1, AC_MTB_DANIEL_U_1,
					AC_MTB_DANIEL_L_1, AC_MTB_DANIEL_R_1 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ��ƴټެ���2����޺���
//------------------------------------------
u8 AcmdCodeMtbDaniel2( u8 site )
{
	u8	code[] = { AC_MTB_DANIEL_D_2,
					AC_MTB_DANIEL_D_2, AC_MTB_DANIEL_U_2,
					AC_MTB_DANIEL_L_2, AC_MTB_DANIEL_R_2 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ���̏곲ذ����
//------------------------------------------
u8 AcmdCodeMtbWheelie( u8 site )
{
	u8	code[] = { AC_MTB_WHEELIE_D,
					AC_MTB_WHEELIE_D, AC_MTB_WHEELIE_U,
					AC_MTB_WHEELIE_L, AC_MTB_WHEELIE_R };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ��ذ�J�n2����
//------------------------------------------
u8 AcmdCodeMtbWheelieStart2( u8 site )
{
	u8	code[] = { AC_MTB_WHEELIE_S_D_2,
					AC_MTB_WHEELIE_S_D_2, AC_MTB_WHEELIE_S_U_2,
					AC_MTB_WHEELIE_S_L_2, AC_MTB_WHEELIE_S_R_2 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ��ذ�ړ�����
//------------------------------------------
u8 AcmdCodeMtbWheelie2( u8 site )
{
	u8	code[] = { AC_MTB_WHEELIE_D_2,
					AC_MTB_WHEELIE_D_2, AC_MTB_WHEELIE_U_2,
					AC_MTB_WHEELIE_L_2, AC_MTB_WHEELIE_R_2 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ��ذ�I������
//------------------------------------------
u8 AcmdCodeMtbWheelieEnd2( u8 site )
{
	u8	code[] = { AC_MTB_WHEELIE_E_D_2,
					AC_MTB_WHEELIE_E_D_2, AC_MTB_WHEELIE_E_U_2,
					AC_MTB_WHEELIE_E_L_2, AC_MTB_WHEELIE_E_R_2 };
	
	if( site >= SITE_DL )
	{
		site = 0;
	}
	
	return( code[site] );
}

//------------------------------------------
//	���� -> ����������
//------------------------------------------
u8 SiteOpposite( u8 site )								// ���� -> ������������
{
	u8 tbl[] =
		{ SITE_U,SITE_D,SITE_R,SITE_L,
		  SITE_UR,SITE_UL,SITE_DR,SITE_DL };
	
	if( site == SITE_N || site > SITE_UR )
	{
		return( site );
	}
	
	return( tbl[site-1] );
}

//------------------------------------------
//	���ʕ���ð���
//------------------------------------------
static const u8 FrontSiteTbl[4][4] =
{
	{ SITE_U, SITE_D, SITE_R, SITE_L },
	{ SITE_D, SITE_U, SITE_L, SITE_R },
	{ SITE_L, SITE_R, SITE_U, SITE_D },
	{ SITE_R, SITE_L, SITE_D, SITE_U },
};

//------------------------------------------
//	���ʕ���
//------------------------------------------
u8 FrontSite( u8 front_site, u8 site )
{
	return( FrontSiteTbl[front_site - 1][site - 1] );
}

//------------------------------------------
//	���ʕ����ϊ�ð���
//------------------------------------------
static const u8 FrontChgSiteTbl[4][4] =
{
	{ SITE_U, SITE_D, SITE_R, SITE_L },
	{ SITE_D, SITE_U, SITE_L, SITE_R },
	{ SITE_R, SITE_L, SITE_D, SITE_U },
	{ SITE_L, SITE_R, SITE_U, SITE_D },
};

//------------------------------------------
//	���ʕ����ϊ�
//	In:		main_site		; ��{���ʕ���
//			front_site		; �ϊ����ʕ���
//			site			; �ύX����
//------------------------------------------
u8 FrontChgSite( u8 main_site, u8 front_site, u8 site )
{
	if( front_site == SITE_N || site == SITE_N ||
		front_site > SITE_R || site > SITE_R )
	{
		return( SITE_N );
	}

	site = FrontSite( front_site, site );				// �ϊ������̐��ʕ���

	return( FrontChgSiteTbl[main_site - 1][site - 1] );	// ��{�����̐��ʕ���
}

//======================================================================
//		��Ұ��ݺ���ޱ����
//======================================================================
static u8 (* const ac_site_down_tbl[])(EventOBJ *, actWork *);
static u8 (* const ac_site_up_tbl[])(EventOBJ *, actWork *);
static u8 (* const ac_site_left_tbl[])(EventOBJ *, actWork *);
static u8 (* const ac_site_right_tbl[])(EventOBJ *, actWork *);
static u8 (* const ac_walk_05_down_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_walk_05_up_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_walk_05_left_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_walk_05_right_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_walk_1_down_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_walk_1_up_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_walk_1_left_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_walk_1_right_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_jump_down_2_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_jump_up_2_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_jump_left_2_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_jump_right_2_tbl[])( EventOBJ *, actWork *);
static u8 (* const ac_wait_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_wait_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_wait_4_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_wait_8_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_wait_16_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_2_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_2_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_2_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_2_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_down_05_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_up_05_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_left_05_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_right_05_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_down_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_up_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_left_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_right_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_down_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_up_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_left_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_right_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_down_4_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_up_4_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_left_4_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_walk_right_4_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_3_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_3_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_3_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_3_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_4_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_4_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_4_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_4_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_8_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_8_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_8_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_walk_8_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_dash_2_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_dash_2_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_dash_2_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_dash_2_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_anm_end_wait_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_swim_jump_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_swim_jump_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_swim_jump_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_swim_jump_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_turn_hero_site_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_turn_rev_hero_site_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_site_pause_on_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_site_pause_off_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_jump_down_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_jump_up_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_jump_left_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_jump_right_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_jump_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_jump_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_jump_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_jump_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_turn_jump_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_turn_jump_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_turn_jump_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_stay_turn_jump_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_default_site_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_pc_bow_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_under_effect_on_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_under_effect_off_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_anm_chg_hold_on_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_anm_chg_hold_off_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_banish_sw_on_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_banish_sw_off_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mark_gyoe_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mark_hatena_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mark_heart_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_hide_pulloff_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_rock_crush_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_tree_cut_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_pri_hold_sw_on_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_pri_hold_sw_off_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_affuse_on_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_affuse_off_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_kiog_walk_s_05_down_tbl[])(EventOBJ *,actWork *);
static u8 (* const ac_kiog_walk_05_down_tbl[])(EventOBJ *,actWork *);
static u8 (* const ac_mtb_ftup_site_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftup_site_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftup_site_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftup_site_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftup_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftup_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftup_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftup_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftdown_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftdown_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftdown_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_ftdown_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_down_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_up_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_left_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_right_1_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_down_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_up_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_left_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_daniel_right_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_down_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_up_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_left_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_right_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_start_down_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_start_up_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_start_left_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_start_right_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_down_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_up_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_left_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_right_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_end_down_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_end_up_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_end_left_2_tbl[])( EventOBJ *, actWork * );
static u8 (* const ac_mtb_wheelie_end_right_2_tbl[])( EventOBJ *, actWork * );

//------------------------------------------
//	��Ұ��ݺ���ޓ���ð���
//------------------------------------------
static u8 (* const * const AcmdActionTbl[])(EventOBJ*, actWork *) =
{
	ac_site_down_tbl,									// AC_SITE_D
	ac_site_up_tbl,										// AC_SITE_U
	ac_site_left_tbl,									// AC_SITE_L
	ac_site_right_tbl,									// AC_SITE_R

	ac_walk_05_down_tbl,								// AC_WALK_D_05
	ac_walk_05_up_tbl,									// AC_WALK_U_05
	ac_walk_05_left_tbl,								// AC_WALK_L_05
	ac_walk_05_right_tbl,								// AC_WALK_R_05

	ac_walk_1_down_tbl,									// AC_WALK_D_1
	ac_walk_1_up_tbl,									// AC_WALK_U_1
	ac_walk_1_left_tbl,									// AC_WALK_L_1
	ac_walk_1_right_tbl,								// AC_WALK_R_1

	ac_jump_down_2_tbl,									// AC_JUMP_D_2
	ac_jump_up_2_tbl,									// AC_JUMP_U_2
	ac_jump_left_2_tbl,									// AC_JUMP_L_2
	ac_jump_right_2_tbl,								// AC_JUMP_R_2

	ac_wait_1_tbl,										// AC_WAIT_1
	ac_wait_2_tbl,										// AC_WAIT_2
	ac_wait_4_tbl,										// AC_WAIT_4
	ac_wait_8_tbl,										// AC_WAIT_8
	ac_wait_16_tbl,										// AC_WAIT_16

	ac_walk_2_down_tbl,									// AC_WALK_D_2
	ac_walk_2_up_tbl,									// AC_WALK_U_2
	ac_walk_2_left_tbl,									// AC_WALK_L_2
	ac_walk_2_right_tbl,								// AC_WALK_R_2

	ac_stay_walk_down_05_tbl,							// AC_STAY_WALK_D_05
	ac_stay_walk_up_05_tbl,								// AC_STAY_WALK_U_05
	ac_stay_walk_left_05_tbl,							// AC_STAY_WALK_L_05
	ac_stay_walk_right_05_tbl,							// AC_STAY_WALK_R_05

	ac_stay_walk_down_1_tbl,							// AC_STAY_WALK_D_1
	ac_stay_walk_up_1_tbl,								// AC_STAY_WALK_U_1
	ac_stay_walk_left_1_tbl,							// AC_STAY_WALK_L_1
	ac_stay_walk_right_1_tbl,							// AC_STAY_WALK_R_1

	ac_stay_walk_down_2_tbl,							// AC_STAY_WALK_D_2
	ac_stay_walk_up_2_tbl,								// AC_STAY_WALK_U_2
	ac_stay_walk_left_2_tbl,							// AC_STAY_WALK_L_2
	ac_stay_walk_right_2_tbl,							// AC_STAY_WALK_R_2
	
	ac_stay_walk_down_4_tbl,							// AC_STAY_WALK_D_4
	ac_stay_walk_up_4_tbl,								// AC_STAY_WALK_U_4
	ac_stay_walk_left_4_tbl,							// AC_STAY_WALK_L_4
	ac_stay_walk_right_4_tbl,							// AC_STAY_WALK_R_4
	
	ac_walk_3_down_tbl,									// AC_WALK_D_3
	ac_walk_3_up_tbl,									// AC_WALK_U_3
	ac_walk_3_left_tbl,									// AC_WALK_L_3
	ac_walk_3_right_tbl,								// AC_WALK_R_3

	ac_walk_4_down_tbl,									// AC_WALK_D_4
	ac_walk_4_up_tbl,									// AC_WALK_U_4
	ac_walk_4_left_tbl,									// AC_WALK_L_4
	ac_walk_4_right_tbl,								// AC_WALK_R_4

	ac_walk_8_down_tbl,									// AC_WALK_D_8
	ac_walk_8_up_tbl,									// AC_WALK_U_8
	ac_walk_8_left_tbl,									// AC_WALK_L_8
	ac_walk_8_right_tbl,								// AC_WALK_R_8

	ac_dash_2_down_tbl,									// AC_DASH_D_2
	ac_dash_2_up_tbl,									// AC_DASH_U_2
	ac_dash_2_left_tbl,									// AC_DASH_L_2
	ac_dash_2_right_tbl,								// AC_DASH_R_2

	ac_anm_end_wait_tbl,								// AC_ANM_END_WAIT

	ac_swim_jump_down_tbl,								// AC_SWIM_JUMP_D
	ac_swim_jump_up_tbl,								// AC_SWIM_JUMP_U
	ac_swim_jump_left_tbl,								// AC_SWIM_JUMP_L
	ac_swim_jump_right_tbl,								// AC_SWIM_JUMP_R

	ac_turn_hero_site_tbl,								// AC_TURN_HERO_SITE
	ac_turn_rev_hero_site_tbl,							// AC_TURN_REV_HERO_SITE

	ac_site_pause_on_tbl,								// AC_SITE_PAUSE_ON
	ac_site_pause_off_tbl,								// AC_SITE_PAUSE_OFF

	ac_jump_down_1_tbl,									// AC_JUMP_D_1
	ac_jump_up_1_tbl,									// AC_JUMP_U_1
	ac_jump_left_1_tbl,									// AC_JUMP_L_1
	ac_jump_right_1_tbl,								// AC_JUMP_R_1

	ac_stay_jump_down_tbl,								// AC_STAY_JUMP_D
	ac_stay_jump_up_tbl,								// AC_STAY_JUMP_U
	ac_stay_jump_left_tbl,								// AC_STAY_JUMP_L
	ac_stay_jump_right_tbl,								// AC_STAY_JUMP_R

	ac_stay_turn_jump_down_tbl,							// AC_STAY_TURN_JUMP_D
	ac_stay_turn_jump_up_tbl,							// AC_STAY_TURN_JUMP_U
	ac_stay_turn_jump_left_tbl,							// AC_STAY_TURN_JUMP_L
	ac_stay_turn_jump_right_tbl,						// AC_STAY_TURN_JUMP_R
	
	ac_default_site_tbl,								// AC_DEFAULT_SITE

	ac_pc_bow_tbl,										// AC_PC_BOW

	ac_under_effect_on_tbl,								// AC_UNDER_EFFECT_ON
	ac_under_effect_off_tbl,							// AC_UNDER_EFFECT_OFF

	ac_anm_chg_hold_on_tbl,								// AC_ANM_CHG_HOLD_ON
	ac_anm_chg_hold_off_tbl,							// AC_ANM_CHG_HOLD_OFF

	ac_banish_sw_on_tbl,								// AC_VANISH_ON
	ac_banish_sw_off_tbl,								// AC_VANISH_OFF

	ac_mark_gyoe_tbl,									// AC_MARK_GYOE
	ac_mark_hatena_tbl,									// AC_MARK_HATENA
	ac_mark_heart_tbl,									// AC_MARK_HEART

	ac_hide_pulloff_tbl,								// AC_HIDE_PULLOFF

	ac_rock_crush_tbl,									// AC_ROCK_CRUSH
	ac_tree_cut_tbl,									// AC_TREE_CUT
	
	ac_pri_hold_sw_on_tbl,								// AC_PRI_HOLD_SW_ON
	ac_pri_hold_sw_off_tbl,								// AC_PRI_HOLD_SW_OFF

	ac_affuse_on_tbl,									// AC_AFFUSE_ON
	ac_affuse_off_tbl,									// AC_AFFUSE_OFF

	ac_kiog_walk_s_05_down_tbl,							// AC_KIOG_WALK_S_D_05
	ac_kiog_walk_05_down_tbl,							// AC_KIOG_WALK_D_05

	ac_mtb_ftup_site_down_tbl,							// AC_MTB_FTUP_SITE_D
	ac_mtb_ftup_site_up_tbl,							// AC_MTB_FTUP_SITE_U
	ac_mtb_ftup_site_left_tbl,							// AC_MTB_FTUP_SITE_L
	ac_mtb_ftup_site_right_tbl,							// AC_MTB_FTUP_SITE_R

	ac_mtb_ftup_down_tbl,								// AC_MTB_FTUP_D
	ac_mtb_ftup_up_tbl,									// AC_MTB_FTUP_U
	ac_mtb_ftup_left_tbl,								// AC_MTB_FTUP_L
	ac_mtb_ftup_right_tbl,								// AC_MTB_FTUP_R
	
	ac_mtb_ftdown_down_tbl,								// AC_MTB_FTDOWN_D
	ac_mtb_ftdown_up_tbl,								// AC_MTB_FTDOWN_U
	ac_mtb_ftdown_left_tbl,								// AC_MTB_FTDOWN_L
	ac_mtb_ftdown_right_tbl,							// AC_MTB_FTDOWN_R

	ac_mtb_daniel_down_tbl,								// AC_MTB_DANIEL_D
	ac_mtb_daniel_up_tbl,								// AC_MTB_DANIEL_U
	ac_mtb_daniel_left_tbl,								// AC_MTB_DANIEL_L
	ac_mtb_daniel_right_tbl,							// AC_MTB_DANIEL_R

	ac_mtb_daniel_down_1_tbl,							// AC_MTB_DANIEL_D_1
	ac_mtb_daniel_up_1_tbl,								// AC_MTB_DANIEL_U_1
	ac_mtb_daniel_left_1_tbl,							// AC_MTB_DANIEL_L_1
	ac_mtb_daniel_right_1_tbl,							// AC_MTB_DANIEL_R_1

	ac_mtb_daniel_down_2_tbl,							// AC_MTB_DANIEL_D_2
	ac_mtb_daniel_up_2_tbl,								// AC_MTB_DANIEL_U_2
	ac_mtb_daniel_left_2_tbl,							// AC_MTB_DANIEL_L_2
	ac_mtb_daniel_right_2_tbl,							// AC_MTB_DANIEL_R_2

	ac_mtb_wheelie_down_tbl,							// AC_MTB_WHEELIE_D
	ac_mtb_wheelie_up_tbl,								// AC_MTB_WHEELIE_U
	ac_mtb_wheelie_left_tbl,							// AC_MTB_WHEELIE_L
	ac_mtb_wheelie_right_tbl,							// AC_MTB_WHEELIE_R

	ac_mtb_wheelie_start_down_2_tbl,					// AC_MTB_WHEELIE_S_D_2
	ac_mtb_wheelie_start_up_2_tbl,						// AC_MTB_WHEELIE_S_U_2
	ac_mtb_wheelie_start_left_2_tbl,					// AC_MTB_WHEELIE_S_L_2
	ac_mtb_wheelie_start_right_2_tbl,					// AC_MTB_WHEELIE_S_R_2

	ac_mtb_wheelie_down_2_tbl,							// AC_MTB_WHEELIE_D_2
	ac_mtb_wheelie_up_2_tbl,							// AC_MTB_WHEELIE_U_2
	ac_mtb_wheelie_left_2_tbl,							// AC_MTB_WHEELIE_L_2
	ac_mtb_wheelie_right_2_tbl,							// AC_MTB_WHEELIE_R_2
	
	ac_mtb_wheelie_end_down_2_tbl,						// AC_MTB_WHEELIE_E_D_2
	ac_mtb_wheelie_end_up_2_tbl,						// AC_MTB_WHEELIE_E_U_2
	ac_mtb_wheelie_end_left_2_tbl,						// AC_MTB_WHEELIE_E_L_2
	ac_mtb_wheelie_end_right_2_tbl,						// AC_MTB_WHEELIE_E_R_2
};

//------------------------------------------
//	��Ұ��ݱ����
//	work2 = ���ݽ����
//------------------------------------------
void AcmdAction( EventOBJ *ev, actWork *act )
{
	if( AcmdActionTbl[ev->acmd][act->work[2]](ev,act) )
	{
		ev->acmd_end_sw = 1;
	}
}

//------------------------------------------
//	����ޱ����
//------------------------------------------
u8 CmdAction( EventOBJ *ev, actWork *act )
{
	if( AcmdActionTbl[ev->acmd][act->work[2]](ev,act) )
	{
		ev->acmd = NOT_ACMD;
		act->work[2] = 0;
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	����޾��
//------------------------------------------
void CmdActionSet( EventOBJ *ev, actWork *act, u8 code )
{
	ev->acmd = code;
	act->work[2] = 0;
}

//======================================================================
//		��Ұ��ݺ���޺��ޓ���
//======================================================================
static u8 ac_move_end( EventOBJ *, actWork * );
static u8 ac_move_end_anm_pause( EventOBJ *, actWork * );

//------------------------------------------
//	�����Œ�n
//------------------------------------------
static void ac_direct_set( EventOBJ *ev, actWork *act, u8 site )
{
	EvObjSiteSet( ev, site );
	EvObjPosRenew( ev );
	objanm_stop_start( ev, act, EvActAnmNoWalk1(ev->site) );
	act->anm_pause = 1;
	act->work[2] = 1;
}

//------------------------------------------
//	AC_SITE_D
//------------------------------------------
static u8 ac_site_down( EventOBJ *ev, actWork *act )
{
	ac_direct_set( ev, act, SITE_D );
	return( 1 );
}

static u8 (* const ac_site_down_tbl[])(EventOBJ*,actWork*) =
{
	ac_site_down,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_SITE_U
//------------------------------------------
static u8 ac_site_up( EventOBJ *ev, actWork *act )
{
	ac_direct_set( ev, act, SITE_U );
	return( 1 );
}

static u8 (* const ac_site_up_tbl[])(EventOBJ*,actWork*) =
{
	ac_site_up,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_SITE_L
//------------------------------------------
static u8 ac_site_left( EventOBJ *ev, actWork *act )
{
	ac_direct_set( ev, act, SITE_L );
	return( 1 );
}

static u8 (* const ac_site_left_tbl[])(EventOBJ *,actWork*) =
{
	ac_site_left,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_SITE_R
//------------------------------------------
static u8 ac_site_right( EventOBJ *ev, actWork *act )
{
	ac_direct_set( ev, act, SITE_R );
	return( 1 );
}

static u8 (* const ac_site_right_tbl[])(EventOBJ *,actWork*) =
{
	ac_site_right,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	�ړ��n ܰ�������
//------------------------------------------
static void ac_walk_set_work_init( EventOBJ *ev, actWork *act, u8 site, u8 type )
{
	s16	x,y;
	
	x = ev->gx;
	y = ev->gy;

	EvObjSiteSet( ev, site );
	SitePosMove( site, &x, &y );
	EvObjPosChg( ev, x, y );

	ev_act_move_walk_init( act, site, type );

	act->anm_pause = 0;
	ev->move_start_sw = 1;
	act->work[2] = 1;
}

//------------------------------------------
//	�ړ��n ���쾯�
//------------------------------------------
static void ac_walk_set( EventOBJ *ev, actWork *act, u8 site, u8 type )
{
	u8 (*anm[])(u8) =
	{ EvActAnmNoWalk1, EvActAnmNoWalk2, EvActAnmNoWalk2, EvActAnmNoWalk4, EvActAnmNoWalk8 };

	ac_walk_set_work_init( ev, act, site, type );
	objanm_walk_start( ev, act, anm[type](ev->site) );
}

//------------------------------------------
//	�ޯ���ړ��n ���쾯�
//------------------------------------------
static void ac_walk_dash_set( EventOBJ *ev, actWork *act, u8 site )
{
	ac_walk_set_work_init( ev, act, site, EVACT_MV_2 );
	objanm_walk_start( ev, act, EvActAnmNoDash(ev->site) );
}

//------------------------------------------
//	�ړ��n ����
//------------------------------------------
static u8 ac_walk( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_walk(act) )
	{
		EvObjPosRenew( ev );
		ev->move_end_sw = 1;
		act->anm_pause = 1;
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	0.5�ړ��n ܰ�������
//------------------------------------------
static void ac_walk05_set_work_init( EventOBJ *ev, actWork *act, u8 site )
{
	s16	x,y;

	x = ev->gx;
	y = ev->gy;

	EvObjSiteSet( ev, site );
	SitePosMove( site, &x, &y );
	EvObjPosChg( ev, x, y );

	ev_act_move_walk_05_init( act, site );

	act->anm_pause = 0;
	ev->move_start_sw = 1;
	act->work[2] = 1;
}

//------------------------------------------
//	0.5�ړ��n ���쾯�
//------------------------------------------
static void ac_walk05_set( EventOBJ *ev, actWork *act, u8 site )
{
	ac_walk05_set_work_init( ev, act, site );
	objanm_walk_start( ev, act, EvActAnmNoWalk1(ev->site) );
}

//------------------------------------------
//	0.5�ړ��n ����
//------------------------------------------
static u8 ac_walk05( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_walk_05(act) )
	{
		EvObjPosRenew( ev );
		ev->move_end_sw = 1;
		act->anm_pause = 1;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	AC_WALK_05_D
//------------------------------------------
static u8 ac_walk_05_down1( EventOBJ *, actWork * );

static u8 ac_walk_05_down0( EventOBJ *ev, actWork *act )
{
	ac_walk05_set( ev, act, SITE_D );
	return( ac_walk_05_down1( ev, act ) );
}

static u8 ac_walk_05_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk05(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_05_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_05_down0,
	ac_walk_05_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_1_U
//------------------------------------------
static u8 ac_walk_05_up1( EventOBJ *, actWork * );

static u8 ac_walk_05_up0( EventOBJ *ev, actWork *act )
{
	ac_walk05_set( ev, act, SITE_U );
	return( ac_walk_05_up1( ev, act ) );
}

static u8 ac_walk_05_up1( EventOBJ *ev, actWork *act )
{
	if( ac_walk05(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_05_up_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_05_up0,
	ac_walk_05_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_05_L
//------------------------------------------
static u8 ac_walk_05_left1( EventOBJ *, actWork * );

static u8 ac_walk_05_left0( EventOBJ *ev, actWork *act )
{
	ac_walk05_set( ev, act, SITE_L );
	return( ac_walk_05_left1( ev, act ) );
}

static u8 ac_walk_05_left1( EventOBJ *ev, actWork *act )
{
	if( ac_walk05(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_05_left_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_05_left0,
	ac_walk_05_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_05_R
//------------------------------------------
static u8 ac_walk_05_right1( EventOBJ *, actWork * );

static u8 ac_walk_05_right0( EventOBJ *ev, actWork *act )
{
	ac_walk05_set( ev, act, SITE_R );
	return( ac_walk_05_right1( ev, act ) );
}

static u8 ac_walk_05_right1( EventOBJ *ev, actWork *act )
{
	if( ac_walk05(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_05_right_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_05_right0,
	ac_walk_05_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_1_D
//------------------------------------------
static u8 ac_walk_1_down1( EventOBJ *, actWork * );

static u8 ac_walk_1_down0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_D, EVACT_MV_1 );
	return( ac_walk_1_down1( ev, act ) );
}

static u8 ac_walk_1_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_1_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_1_down0,
	ac_walk_1_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_1_U
//------------------------------------------
static u8 ac_walk_1_up1( EventOBJ *, actWork * );

static u8 ac_walk_1_up0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_U, EVACT_MV_1 );
	return( ac_walk_1_up1( ev, act ) );
}

static u8 ac_walk_1_up1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_1_up_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_1_up0,
	ac_walk_1_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_1_L
//------------------------------------------
static u8 ac_walk_1_left1( EventOBJ *, actWork * );

static u8 ac_walk_1_left0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_L, EVACT_MV_1 );
	return( ac_walk_1_left1( ev, act ) );
}

static u8 ac_walk_1_left1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_1_left_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_1_left0,
	ac_walk_1_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_1_R
//------------------------------------------
static u8 ac_walk_1_right1( EventOBJ *, actWork * );

static u8 ac_walk_1_right0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_R, EVACT_MV_1 );
	return( ac_walk_1_right1( ev, act ) );
}

static u8 ac_walk_1_right1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_1_right_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_1_right0,
	ac_walk_1_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	�ެ��� ܰ�������
//------------------------------------------
static void ac_jump_set_work_init( EventOBJ *ev, actWork *act, u8 site, u8 type, u8 v_type )
{
	s16	add[] = { 0,1,1	};								// 0,1,2��
	s16 dx,dy;
	
	dx = 0;
	dy = 0;
	EvObjSiteSet( ev, site );
	SitePosAdd( site, &dx, &dy, add[type], add[type] );
	EvObjPosChg( ev, ev->gx + dx, ev->gy + dy );
	ev_act_move_jump_init( act, site, type, v_type );
	act->work[2] = 1;

	act->anm_pause = 0;
	ev->move_start_sw = 1;
	ev->move_jump_start_sw = 1;
}

//------------------------------------------
//	�ެ��� ���
//------------------------------------------
static void ac_jump_set( EventOBJ *ev, actWork *act, u8 site, u8 type, u8 v_type )
{
	ac_jump_set_work_init( ev, act, site, type, v_type );
	objanm_walk_start( ev, act, EvActAnmNoWalk1(ev->site) );
	EvEffShadeMake( ev );
}

//------------------------------------------
//	�ެ��� ����
//------------------------------------------
static u8 ac_jump_move_sub( EventOBJ *ev, actWork *act, u8 (*move)(actWork*) )
{
	s16	add[] = { 0,0,1 };								// 0,1,2��
	s16 dx,dy;
	u8 ret;

	ret = move( act );
	
	if( ret == 1 && add[act->work[4]] )					// ʰ� & �c���
	{
		dx = 0;
		dy = 0;
		SitePosAdd( ev->mv_site, &dx, &dy, add[act->work[4]], add[act->work[4]] );
		EvObjPosChg( ev, ev->gx + dx, ev->gy + dy );
		ev->move_start_sw = 1;
		ev->move_jump_start_sw = 1;
	}
	else if( ret == 0xff )								// �I��
	{
		EvObjPosRenew( ev );
		ev->move_end_sw = 1;
		ev->move_jump_end_sw = 1;
		act->anm_pause = 1;
	}

	return( ret );
}

static u8 ac_jump_move( EventOBJ *ev, actWork *act )	// ɰ��
{
	return( ac_jump_move_sub(ev,act,ev_act_move_jump) );
}

static u8 ac_jump_move_05( EventOBJ *ev, actWork *act )	// 05
{
	return( ac_jump_move_sub(ev,act,ev_act_move_jump_05) );
}

static u8 ac_jump( EventOBJ *ev, actWork *act )			// �ȗ�
{
	u8 flag;

	flag = ac_jump_move( ev, act );
	
	if( flag == 0xff )
	{
		return( 1 );
	}
	
	return( 0 );
}

static u8 ac_jump_05( EventOBJ *ev, actWork *act )		// 05 �ȗ�
{
	u8 flag;
	
	flag = ac_jump_move_05( ev, act );
	
	if( flag == 0xff )
	{
		return( 1 );
	}
	
	return( 0 );
}

static u8 ac_jump_half_opposite( EventOBJ *ev, actWork *act )
{
	u8 flag;
	
	flag = ac_jump_move( ev, act );

	if( flag == 1 )
	{
		EvObjSiteSet( ev, SiteOpposite(ev->mv_site) );
		objanm_stop_start( ev, act, EvActAnmNoWalk1(ev->site) );
	}
	else if( flag == 0xff )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
// AC_JUMP_D_2
//------------------------------------------
static u8 ac_jump_down2_1( EventOBJ *, actWork * );

static u8 ac_jump_down2_0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_D, EVACT_MV_JUMP_2, EVACT_MV_JUMP_V_12 );
	return( ac_jump_down2_1(ev,act) );
}

static u8 ac_jump_down2_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}

	return( 0 );
}

static u8 (* const ac_jump_down_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_jump_down2_0,
	ac_jump_down2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_JUMP_U_2
//------------------------------------------
static u8 ac_jump_up2_1( EventOBJ *, actWork * );

static u8 ac_jump_up2_0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_U, EVACT_MV_JUMP_2, EVACT_MV_JUMP_V_12 );
	return( ac_jump_up2_1(ev,act) );
}

static u8 ac_jump_up2_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_jump_up_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_jump_up2_0,
	ac_jump_up2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_JUMP_L_2
//------------------------------------------
static u8 ac_jump_left2_1( EventOBJ *, actWork * );

static u8 ac_jump_left2_0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_L, EVACT_MV_JUMP_2, EVACT_MV_JUMP_V_12 );
	return( ac_jump_left2_1(ev,act) );
}

static u8 ac_jump_left2_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_jump_left_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_jump_left2_0,
	ac_jump_left2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_JUMP_R_2
//------------------------------------------
static u8 ac_jump_right2_1( EventOBJ *, actWork * );

static u8 ac_jump_right2_0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_R, EVACT_MV_JUMP_2, EVACT_MV_JUMP_V_12 );
	return( ac_jump_right2_1(ev,act) );
}

static u8 ac_jump_right2_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_jump_right_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_jump_right2_0,
	ac_jump_right2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	���ĳ��Čn
//------------------------------------------
static void ac_wait_set( actWork *act, s16 wait )
{
	act->work[2] = 1;
	act->work[3] = wait;
}

static u8 ac_wait_move( EventOBJ *ev, actWork *act )
{
	act->work[3]--;
	
	if( act->work[3] == 0 )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	AC_WAIT_1
//------------------------------------------
static u8 ac_wait_1( EventOBJ *ev, actWork *act )
{
	ac_wait_set( act, 1 );
	return( ac_wait_move(ev,act) );
}

static u8 (* const ac_wait_1_tbl[])(EventOBJ *, actWork *) =
{
	ac_wait_1,
	ac_wait_move,
	ac_move_end,
};

//------------------------------------------
//	AC_WAIT_2
//------------------------------------------
static u8 ac_wait_2( EventOBJ *ev, actWork *act )
{
	ac_wait_set( act, 2 );
	return( ac_wait_move(ev,act) );
}

static u8 (* const ac_wait_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_wait_2,
	ac_wait_move,
	ac_move_end,
};

//------------------------------------------
//	AC_WAIT_4
//------------------------------------------
static u8 ac_wait_4( EventOBJ *ev, actWork *act )
{
	ac_wait_set( act, 4 );
	return( ac_wait_move(ev,act) );
}

static u8 (* const ac_wait_4_tbl[])(EventOBJ *, actWork *) =
{
	ac_wait_4,
	ac_wait_move,
	ac_move_end,
};

//------------------------------------------
//	AC_WAIT_8
//------------------------------------------
static u8 ac_wait_8( EventOBJ *ev, actWork *act )
{
	ac_wait_set( act, 8 );
	return( ac_wait_move(ev,act) );
}

static u8 (* const ac_wait_8_tbl[])(EventOBJ *, actWork *) =
{
	ac_wait_8,
	ac_wait_move,
	ac_move_end,
};

//------------------------------------------
//	AC_WAIT_16
//------------------------------------------
static u8 ac_wait_16( EventOBJ *ev, actWork *act )
{
	ac_wait_set( act, 16 );
	return( ac_wait_move(ev,act) );
}

static u8 (* const ac_wait_16_tbl[])(EventOBJ *, actWork *) =
{
	ac_wait_16,
	ac_wait_move,
	ac_move_end,
};

//------------------------------------------
//	AC_WALK_D_2
//------------------------------------------
static u8 ac_walk_2_down1( EventOBJ *, actWork * );

static u8 ac_walk_2_down0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_D, EVACT_MV_2 );
	return( ac_walk_2_down1( ev, act ) );
}

static u8 ac_walk_2_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_2_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_2_down0,
	ac_walk_2_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_U_2
//------------------------------------------
static u8 ac_walk_2_up1( EventOBJ *, actWork * );

static u8 ac_walk_2_up0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_U, EVACT_MV_2 );
	return( ac_walk_2_up1( ev, act ) );
}

static u8 ac_walk_2_up1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_2_up_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_2_up0,
	ac_walk_2_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_L_2
//------------------------------------------
static u8 ac_walk_2_left1( EventOBJ *, actWork * );

static u8 ac_walk_2_left0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_L, EVACT_MV_2 );
	return( ac_walk_2_left1( ev, act ) );
}

static u8 ac_walk_2_left1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_2_left_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_2_left0,
	ac_walk_2_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_R_2
//------------------------------------------
static u8 ac_walk_2_right1( EventOBJ *, actWork * );

static u8 ac_walk_2_right0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_R, EVACT_MV_2 );
	return( ac_walk_2_right1( ev, act ) );
}

static u8 ac_walk_2_right1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_2_right_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_2_right0,
	ac_walk_2_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	���̏����
//------------------------------------------
static void ac_stay_walk_set( EventOBJ *ev, actWork *act, u8 site, u8 anm_no, s16 wait )
{
	EvObjSiteSet( ev, site );
	objanm_walk_start( ev, act, anm_no );
	act->anm_pause = 0;
	act->work[2] = 1;
	act->work[3] = wait;
}

static u8 ac_stay_walk_1( EventOBJ *ev, actWork *act )
{
	act->work[3]--;
	
	if( act->work[3] == 0 )
	{
		act->work[2] = 2;
		act->anm_pause = 1;
		return( 1 );
	}
	
	return( 0 );
}

static u8 ac_stay_walk_05( EventOBJ *ev, actWork *act )
{
	if( act->work[3] & 0x01 ) act->anm_wait++;
	return( ac_stay_walk_1(ev,act) );
}

//------------------------------------------
//	AC_STAY_WALK_D_05
//------------------------------------------
static u8 ac_stay_walk_down_05_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_D, EvActAnmNoWalk1(SITE_D), 32 );
	return( ac_stay_walk_05(ev,act) );
}

static u8 (* const ac_stay_walk_down_05_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_down_05_0,
	ac_stay_walk_05,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_U_05
//------------------------------------------
static u8 ac_stay_walk_up_05_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_U, EvActAnmNoWalk1(SITE_U), 32 );
	return( ac_stay_walk_05(ev,act) );
}

static u8 (* const ac_stay_walk_up_05_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_up_05_0,
	ac_stay_walk_05,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_L_05
//------------------------------------------
static u8 ac_stay_walk_left_05_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_L, EvActAnmNoWalk1(SITE_L), 32 );
	return( ac_stay_walk_05(ev,act) );
}

static u8 (* const ac_stay_walk_left_05_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_left_05_0,
	ac_stay_walk_05,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_R_05
//------------------------------------------
static u8 ac_stay_walk_right_05_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_R, EvActAnmNoWalk1(SITE_R), 32 );
	return( ac_stay_walk_05(ev,act) );
}

static u8 (* const ac_stay_walk_right_05_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_right_05_0,
	ac_stay_walk_05,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_D_1
//------------------------------------------
static u8 ac_stay_walk_down_1_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_D, EvActAnmNoWalk1(SITE_D), 16 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_down_1_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_down_1_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_U_1
//------------------------------------------
static u8 ac_stay_walk_up_1_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_U, EvActAnmNoWalk1(SITE_U), 16 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_up_1_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_up_1_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_L_1
//------------------------------------------
static u8 ac_stay_walk_left_1_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_L, EvActAnmNoWalk1(SITE_L), 16 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_left_1_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_left_1_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_R_1
//------------------------------------------
static u8 ac_stay_walk_right_1_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_R, EvActAnmNoWalk1(SITE_R), 16 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_right_1_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_right_1_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_D_2
//------------------------------------------
static u8 ac_stay_walk_down_2_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_D, EvActAnmNoWalk2(SITE_D), 8 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_down_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_down_2_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_U_2
//------------------------------------------
static u8 ac_stay_walk_up_2_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_U, EvActAnmNoWalk2(SITE_U), 8 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_up_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_up_2_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_L_2
//------------------------------------------
static u8 ac_stay_walk_left_2_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_L, EvActAnmNoWalk2(SITE_L), 8 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_left_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_left_2_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_R_2
//------------------------------------------
static u8 ac_stay_walk_right_2_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_R, EvActAnmNoWalk2(SITE_R), 8 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_right_2_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_right_2_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_D_4
//------------------------------------------
static u8 ac_stay_walk_down_4_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_D, EvActAnmNoWalk4(SITE_D), 4 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_down_4_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_down_4_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_U_4
//------------------------------------------
static u8 ac_stay_walk_up_4_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_U, EvActAnmNoWalk4(SITE_U), 4 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_up_4_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_up_4_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_L_4
//------------------------------------------
static u8 ac_stay_walk_left_4_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_L, EvActAnmNoWalk4(SITE_L), 4 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_left_4_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_left_4_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_STAY_WALK_R_4
//------------------------------------------
static u8 ac_stay_walk_right_4_0( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_R, EvActAnmNoWalk4(SITE_R), 4 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_stay_walk_right_4_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_walk_right_4_0,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_3_D
//------------------------------------------
static u8 ac_walk_3_down1( EventOBJ *, actWork * );

static u8 ac_walk_3_down0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_D, EVACT_MV_3 );
	return( ac_walk_3_down1( ev, act ) );
}

static u8 ac_walk_3_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_3_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_3_down0,
	ac_walk_3_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_3_U
//------------------------------------------
static u8 ac_walk_3_up1( EventOBJ *, actWork * );

static u8 ac_walk_3_up0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_U, EVACT_MV_3 );
	return( ac_walk_3_up1( ev, act ) );
}

static u8 ac_walk_3_up1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_3_up_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_3_up0,
	ac_walk_3_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_3_L
//------------------------------------------
static u8 ac_walk_3_left1( EventOBJ *, actWork * );

static u8 ac_walk_3_left0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_L, EVACT_MV_3 );
	return( ac_walk_3_left1( ev, act ) );
}

static u8 ac_walk_3_left1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_3_left_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_3_left0,
	ac_walk_3_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_3_R
//------------------------------------------
static u8 ac_walk_3_right1( EventOBJ *, actWork * );

static u8 ac_walk_3_right0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_R, EVACT_MV_3 );
	return( ac_walk_3_right1( ev, act ) );
}

static u8 ac_walk_3_right1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_3_right_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_3_right0,
	ac_walk_3_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_4_D
//------------------------------------------
static u8 ac_walk_4_down1( EventOBJ *, actWork * );

static u8 ac_walk_4_down0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_D, EVACT_MV_4 );
	return( ac_walk_4_down1( ev, act ) );
}

static u8 ac_walk_4_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_4_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_4_down0,
	ac_walk_4_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_4_U
//------------------------------------------
static u8 ac_walk_4_up1( EventOBJ *, actWork * );

static u8 ac_walk_4_up0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_U, EVACT_MV_4 );
	return( ac_walk_4_up1( ev, act ) );
}

static u8 ac_walk_4_up1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_4_up_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_4_up0,
	ac_walk_4_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_4_L
//------------------------------------------
static u8 ac_walk_4_left1( EventOBJ *, actWork * );

static u8 ac_walk_4_left0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_L, EVACT_MV_4 );
	return( ac_walk_4_left1( ev, act ) );
}

static u8 ac_walk_4_left1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_4_left_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_4_left0,
	ac_walk_4_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_4_R
//------------------------------------------
static u8 ac_walk_4_right1( EventOBJ *, actWork * );

static u8 ac_walk_4_right0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_R, EVACT_MV_4 );
	return( ac_walk_4_right1( ev, act ) );
}

static u8 ac_walk_4_right1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_4_right_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_4_right0,
	ac_walk_4_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_8_D
//------------------------------------------
static u8 ac_walk_8_down1( EventOBJ *, actWork * );

static u8 ac_walk_8_down0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_D, EVACT_MV_8 );
	return( ac_walk_8_down1( ev, act ) );
}

static u8 ac_walk_8_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_8_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_8_down0,
	ac_walk_8_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_8_U
//------------------------------------------
static u8 ac_walk_8_up1( EventOBJ *, actWork * );

static u8 ac_walk_8_up0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_U, EVACT_MV_8 );
	return( ac_walk_8_up1( ev, act ) );
}

static u8 ac_walk_8_up1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_8_up_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_8_up0,
	ac_walk_8_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_8_L
//------------------------------------------
static u8 ac_walk_8_left1( EventOBJ *, actWork * );

static u8 ac_walk_8_left0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_L, EVACT_MV_8 );
	return( ac_walk_8_left1( ev, act ) );
}

static u8 ac_walk_8_left1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_8_left_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_8_left0,
	ac_walk_8_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_WALK_8_R
//------------------------------------------
static u8 ac_walk_8_right1( EventOBJ *, actWork * );

static u8 ac_walk_8_right0( EventOBJ *ev, actWork *act )
{
	ac_walk_set( ev, act, SITE_R, EVACT_MV_8 );
	return( ac_walk_8_right1( ev, act ) );
}

static u8 ac_walk_8_right1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_walk_8_right_tbl[])(EventOBJ *,actWork *) =
{
	ac_walk_8_right0,
	ac_walk_8_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_DASH_D_2
//------------------------------------------
static u8 ac_dash_2_down1( EventOBJ *, actWork * );

static u8 ac_dash_2_down0( EventOBJ *ev, actWork *act )
{
	ac_walk_dash_set( ev, act, SITE_D );
	return( ac_dash_2_down1( ev, act ) );
}

static u8 ac_dash_2_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_dash_2_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_dash_2_down0,
	ac_dash_2_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_DASH_U_2
//------------------------------------------
static u8 ac_dash_2_up1( EventOBJ *, actWork * );

static u8 ac_dash_2_up0( EventOBJ *ev, actWork *act )
{
	ac_walk_dash_set( ev, act, SITE_U );
	return( ac_dash_2_up1( ev, act ) );
}

static u8 ac_dash_2_up1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_dash_2_up_tbl[])(EventOBJ *,actWork *) =
{
	ac_dash_2_up0,
	ac_dash_2_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_DASH_L_2
//------------------------------------------
static u8 ac_dash_2_left1( EventOBJ *, actWork * );

static u8 ac_dash_2_left0( EventOBJ *ev, actWork *act )
{
	ac_walk_dash_set( ev, act, SITE_L );
	return( ac_dash_2_left1( ev, act ) );
}

static u8 ac_dash_2_left1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_dash_2_left_tbl[])(EventOBJ *,actWork *) =
{
	ac_dash_2_left0,
	ac_dash_2_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_DASH_R_2
//------------------------------------------
static u8 ac_dash_2_right1( EventOBJ *, actWork * );

static u8 ac_dash_2_right0( EventOBJ *ev, actWork *act )
{
	ac_walk_dash_set( ev, act, SITE_R );
	return( ac_dash_2_right1( ev, act ) );
}

static u8 ac_dash_2_right1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_dash_2_right_tbl[])(EventOBJ *,actWork *) =
{
	ac_dash_2_right0,
	ac_dash_2_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	�ٱ�Ҵ���, �������
//------------------------------------------
static void ac_anm_end_wait_site_set( EventOBJ *ev, actWork *act, u8 site, u8 no )
{
	ev_act_move_anm_wait_init( act, no, 0 );
	EvObjSiteSet( ev, site );
	act->work[2] = 1;
}

//------------------------------------------
//	AC_ANM_END_WAIT
//------------------------------------------
static u8 ac_anm_end_wait1( EventOBJ *, actWork * );

static u8 ac_anm_end_wait0( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, ev->mv_site, act->anm_no );
	return( 0 );
}

static u8 ac_anm_end_wait1( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_anm_wait(act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_anm_end_wait_tbl[])(EventOBJ *, actWork *) =
{
	ac_anm_end_wait0,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	�g���ެ��߾��
//------------------------------------------
static void ac_swim_jump_set( EventOBJ *ev, actWork *act, u8 site )
{
	ac_jump_set_work_init( ev, act, site, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_12 );
	ActAnmChg( act, EvActAnmNoSwimJump(site) );
}

//------------------------------------------
// AC_SWIM_JUMP_D
//------------------------------------------
static u8 ac_swim_jump_down1( EventOBJ *, actWork * );

static u8 ac_swim_jump_down0( EventOBJ *ev, actWork *act )
{
	ac_swim_jump_set( ev, act, SITE_D );
	return( ac_swim_jump_down1(ev,act) );
}

static u8 ac_swim_jump_down1( EventOBJ *ev, actWork *act )
{
	if( ac_jump_05(ev,act) )
	{
		act->work[2] = 2;
		ev->move_jump_end_sw = 0;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_swim_jump_down_tbl[])(EventOBJ *, actWork *) =
{
	ac_swim_jump_down0,
	ac_swim_jump_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_SWIM_JUMP_U
//------------------------------------------
static u8 ac_swim_jump_up1( EventOBJ *, actWork * );

static u8 ac_swim_jump_up0( EventOBJ *ev, actWork *act )
{
	ac_swim_jump_set( ev, act, SITE_U );
	return( ac_swim_jump_up1(ev,act) );
}

static u8 ac_swim_jump_up1( EventOBJ *ev, actWork *act )
{
	if( ac_jump_05(ev,act) )
	{
		act->work[2] = 2;
		ev->move_jump_end_sw = 0;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_swim_jump_up_tbl[])(EventOBJ *, actWork *) =
{
	ac_swim_jump_up0,
	ac_swim_jump_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_SWIM_JUMP_L
//------------------------------------------
static u8 ac_swim_jump_left1( EventOBJ *, actWork * );

static u8 ac_swim_jump_left0( EventOBJ *ev, actWork *act )
{
	ac_swim_jump_set( ev, act, SITE_L );
	return( ac_swim_jump_left1(ev,act) );
}

static u8 ac_swim_jump_left1( EventOBJ *ev, actWork *act )
{
	if( ac_jump_05(ev,act) )
	{
		act->work[2] = 2;
		ev->move_jump_end_sw = 0;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_swim_jump_left_tbl[])(EventOBJ *, actWork *) =
{
	ac_swim_jump_left0,
	ac_swim_jump_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_SWIM_JUMP_R
//------------------------------------------
static u8 ac_swim_jump_right1( EventOBJ *, actWork * );

static u8 ac_swim_jump_right0( EventOBJ *ev, actWork *act )
{
	ac_swim_jump_set( ev, act, SITE_R );
	return( ac_swim_jump_right1(ev,act) );
}

static u8 ac_swim_jump_right1( EventOBJ *ev, actWork *act )
{
	if( ac_jump_05(ev,act) )
	{
		act->work[2] = 2;
		ev->move_jump_end_sw = 0;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_swim_jump_right_tbl[])(EventOBJ *, actWork *) =
{
	ac_swim_jump_right0,
	ac_swim_jump_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_TURN_HERO_SITE
//------------------------------------------
static u8 ac_turn_hero_site0( EventOBJ *ev, actWork *act )
{
	u8 hero,site;
	
	if( EvObjIdPosGet(EV_OBJ_SPID,0,0,&hero) == 0 )
	{
		site = range_site( ev->gx, ev->gy, EvObj[hero].gx, EvObj[hero].gy );
		ac_direct_set( ev, act, site );
	}

	act->work[2] = 1;
	
	return( 1 );
}

static u8 (* const ac_turn_hero_site_tbl[])( EventOBJ *, actWork * ) =
{
	ac_turn_hero_site0,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_TURN_REV_HERO_SITE
//------------------------------------------
static u8 ac_turn_rev_hero_site( EventOBJ *ev, actWork *act )
{
	u8 hero,site;
	
	if( EvObjIdPosGet(EV_OBJ_SPID,0,0,&hero) == 0 )
	{
		site = range_site( ev->gx, ev->gy, EvObj[hero].gx, EvObj[hero].gy );
		site = SiteOpposite( site );
		ac_direct_set( ev, act, site );
	}

	act->work[2] = 1;
	
	return( 1 );
}

static u8 (* const ac_turn_rev_hero_site_tbl[])( EventOBJ *, actWork * ) =
{
	ac_turn_rev_hero_site,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_SITE_PAUSE_ON
//------------------------------------------
static u8 ac_site_pause_on0( EventOBJ *ev, actWork *act )
{
	ev->site_pause_sw = 1;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_site_pause_on_tbl[])( EventOBJ *, actWork * ) =
{
	ac_site_pause_on0,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_SITE_PAUSE_OFF
//------------------------------------------
static u8 ac_site_pause_off0( EventOBJ *ev, actWork *act )
{
	ev->site_pause_sw = 0;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_site_pause_off_tbl[])( EventOBJ *ev, actWork *act ) =
{
	ac_site_pause_off0,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_JUMP_D_1
//------------------------------------------
static u8 act_jump_down1_1( EventOBJ *, actWork * );

static u8 act_jump_down1_0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_D, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_8 );
	return( act_jump_down1_1(ev,act) );
}

static u8 act_jump_down1_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_jump_down_1_tbl[])(EventOBJ *, actWork *) =
{
	act_jump_down1_0,
	act_jump_down1_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_JUMP_U_1
//------------------------------------------
static u8 ac_jump_up1_1( EventOBJ *, actWork * );

static u8 ac_jump_up1_0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_U, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_8 );
	return( ac_jump_up1_1(ev,act) );
}

static u8 ac_jump_up1_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_jump_up_1_tbl[])(EventOBJ *, actWork *) =
{
	ac_jump_up1_0,
	ac_jump_up1_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_JUMP_L_1
//------------------------------------------
static u8 ac_jump_left1_1( EventOBJ *, actWork * );

static u8 ac_jump_left1_0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_L, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_8 );
	return( ac_jump_left1_1(ev,act) );
}

static u8 ac_jump_left1_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_jump_left_1_tbl[])(EventOBJ *, actWork *) =
{
	ac_jump_left1_0,
	ac_jump_left1_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_JUMP_R_1
//------------------------------------------
static u8 ac_jump_right1_1( EventOBJ *, actWork * );

static u8 ac_jump_right1_0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_R, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_8 );
	return( ac_jump_right1_1(ev,act) );
}

static u8 ac_jump_right1_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_jump_right_1_tbl[])(EventOBJ *, actWork *) =
{
	ac_jump_right1_0,
	ac_jump_right1_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_STAY_JUMP_D
//------------------------------------------
static u8 ac_stay_jump_down1( EventOBJ *, actWork * );

static u8 ac_stay_jump_down0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_D, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_12 );
	return( ac_stay_jump_down1(ev,act) );
}

static u8 ac_stay_jump_down1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_stay_jump_down_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_jump_down0,
	ac_stay_jump_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_STAY_JUMP_U
//------------------------------------------
static u8 ac_stay_jump_up1( EventOBJ *, actWork * );

static u8 ac_stay_jump_up0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_U, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_12 );
	return( ac_stay_jump_up1(ev,act) );
}

static u8 ac_stay_jump_up1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_stay_jump_up_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_jump_up0,
	ac_stay_jump_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_STAY_JUMP_L
//------------------------------------------
static u8 ac_stay_jump_left1( EventOBJ *, actWork * );

static u8 ac_stay_jump_left0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_L, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_12 );
	return( ac_stay_jump_left1(ev,act) );
}

static u8 ac_stay_jump_left1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_stay_jump_left_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_jump_left0,
	ac_stay_jump_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_STAY_JUMP_R
//------------------------------------------
static u8 ac_stay_jump_right1( EventOBJ *, actWork * );

static u8 ac_stay_jump_right0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_R, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_12 );
	return( ac_stay_jump_right1(ev,act) );
}

static u8 ac_stay_jump_right1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_stay_jump_right_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_jump_right0,
	ac_stay_jump_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_STAY_TURN_JUMP_D
//------------------------------------------
static u8 ac_stay_turn_jump_down1( EventOBJ *, actWork * );

static u8 ac_stay_turn_jump_down0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_D, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_8 );
	return( ac_stay_turn_jump_down1(ev,act) );
}

static u8 ac_stay_turn_jump_down1( EventOBJ *ev, actWork *act )
{
	if( ac_jump_half_opposite(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_stay_turn_jump_down_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_turn_jump_down0,
	ac_stay_turn_jump_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_STAY_TURN_JUMP_U
//------------------------------------------
static u8 ac_stay_turn_jump_up1( EventOBJ *, actWork * );

static u8 ac_stay_turn_jump_up0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_U, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_8 );
	return( ac_stay_turn_jump_up1(ev,act) );
}

static u8 ac_stay_turn_jump_up1( EventOBJ *ev, actWork *act )
{
	if( ac_jump_half_opposite(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_stay_turn_jump_up_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_turn_jump_up0,
	ac_stay_turn_jump_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_STAY_TURN_JUMP_L
//------------------------------------------
static u8 ac_stay_turn_jump_left1( EventOBJ *, actWork * );

static u8 ac_stay_turn_jump_left0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_L, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_8 );
	return( ac_stay_turn_jump_left1(ev,act) );
}

static u8 ac_stay_turn_jump_left1( EventOBJ *ev, actWork *act )
{
	if( ac_jump_half_opposite(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_stay_turn_jump_left_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_turn_jump_left0,
	ac_stay_turn_jump_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
// AC_STAY_TURN_JUMP_R
//------------------------------------------
static u8 ac_stay_turn_jump_right1( EventOBJ *, actWork * );

static u8 ac_stay_turn_jump_right0( EventOBJ *ev, actWork *act )
{
	ac_jump_set( ev, act, SITE_R, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_8 );
	return( ac_stay_turn_jump_right1(ev,act) );
}

static u8 ac_stay_turn_jump_right1( EventOBJ *ev, actWork *act )
{
	if( ac_jump_half_opposite(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_stay_turn_jump_right_tbl[])(EventOBJ *, actWork *) =
{
	ac_stay_turn_jump_right0,
	ac_stay_turn_jump_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_DEFAULT_SITE
//------------------------------------------
static u8 ac_default_site( EventOBJ *ev, actWork *act )
{
	ac_direct_set( ev, act, EvObjDefaultSite(ev->mv_code) );
	return( 1 );
}

static u8 (* const ac_default_site_tbl[])(EventOBJ *, actWork *) =
{
	ac_default_site,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_PC_BOW
//------------------------------------------
static u8 ac_pc_bow( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_D, PC_BOW_ST );
	return( 0 );
}

static u8 (* const ac_pc_bow_tbl[])( EventOBJ *, actWork * ) =
{
	ac_pc_bow,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_UNDER_EFFECT_ON
//------------------------------------------
static u8 ac_under_effect_on( EventOBJ *ev, actWork *act )
{
	ev->under_effect_sw = 0;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_under_effect_on_tbl[])( EventOBJ *, actWork * ) =
{
	ac_under_effect_on,
	ac_move_end,
};

//------------------------------------------
//	AC_UNDER_EFFECT_OFF
//------------------------------------------
static u8 ac_under_effect_off( EventOBJ *ev, actWork *act )
{
	ev->under_effect_sw = 1;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_under_effect_off_tbl[])( EventOBJ *, actWork * ) =
{
	ac_under_effect_off,
	ac_move_end,
};

//------------------------------------------
//	AC_ANM_CHANGE_HOLD_ON
//------------------------------------------
static u8 ac_anm_chg_hold_on( EventOBJ *ev, actWork *act )
{
	ev->anm_change_sw = 1;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_anm_chg_hold_on_tbl[])( EventOBJ *, actWork * ) =
{
	ac_anm_chg_hold_on,
	ac_move_end,
};

//------------------------------------------
//	AC_ANM_CHANGE_HOLD_OFF
//------------------------------------------
static u8 ac_anm_chg_hold_off( EventOBJ *ev, actWork *act )
{
	SXYObjHead *objst;
	
	objst = SxyObjHeadGet( ev->obj_code );
	ev->anm_change_sw = objst->anm_change_sw;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_anm_chg_hold_off_tbl[])( EventOBJ *, actWork * ) =
{
	ac_anm_chg_hold_off,
	ac_move_end,
};

//------------------------------------------
//	AC_VANISH_ON
//------------------------------------------
static u8 ac_banish_sw_on( EventOBJ *ev, actWork *act )
{
	ev->banish_sw = 1;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_banish_sw_on_tbl[])( EventOBJ *, actWork * ) =
{
	ac_banish_sw_on,
	ac_move_end,
};

//------------------------------------------
//	AC_VANISH_OFF
//------------------------------------------
static u8 ac_banish_sw_off( EventOBJ *ev, actWork *act )
{
	ev->banish_sw = 0;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_banish_sw_off_tbl[])( EventOBJ *, actWork * ) =
{
	ac_banish_sw_off,
	ac_move_end,
};

//------------------------------------------
//	AC_MARK_GYOE
//------------------------------------------
static u8 ac_mark_gyoe( EventOBJ *ev, actWork *act )
{
	EvObjIdWorkSet( ev, &FeSetWork[0], &FeSetWork[1], &FeSetWork[2] );
	FldEffSet( FECODE_Gyoe );
	act->work[2] = 1;

	return( 1 );
}

static u8 (* const ac_mark_gyoe_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mark_gyoe,
	ac_move_end,
};

//------------------------------------------
//	AC_MARK_HATENA
//------------------------------------------
static u8 ac_mark_hatena( EventOBJ *ev, actWork *act )
{
	EvObjIdWorkSet( ev, &FeSetWork[0], &FeSetWork[1], &FeSetWork[2] );
	FldEffSet( FECODE_Hatena );
	act->work[2] = 1;

	return( 1 );
}

static u8 (* const ac_mark_hatena_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mark_hatena,
	ac_move_end,
};

//------------------------------------------
//	AC_MARK_HEART
//------------------------------------------
static u8 ac_mark_heart( EventOBJ *ev, actWork *act )
{
	EvObjIdWorkSet( ev, &FeSetWork[0], &FeSetWork[1], &FeSetWork[2] );
	FldEffSet( FECODE_Heart );
	act->work[2] = 1;

	return( 1 );
}

static u8 (* const ac_mark_heart_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mark_heart,
	ac_move_end,
};

//------------------------------------------
//	AC_HIDE_PULLOFF
//------------------------------------------
static u8 ac_hide_pulloff1( EventOBJ *, actWork * );

static u8 ac_hide_pulloff0( EventOBJ *ev, actWork *act )
{
	if( ev->mv_code == MV_HIDES )						// �ʏ�����
	{
		SetHideSandTrainerPulloffTask( ev );
		return( 0 );
	}

	if( ev->mv_code != MV_HIDEW &&
		ev->mv_code != MV_HIDER )
	{
		act->work[2] = 2;
		return( 1 );
	}

	EvActHideClsSet( ev );
	act->work[2] = 1;
	
	return( ac_hide_pulloff1(ev,act) );
}

static u8 ac_hide_pulloff1( EventOBJ *ev, actWork *act )
{
	if( EvActHideClsChk(ev) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_hide_pulloff_tbl[])( EventOBJ *, actWork * ) =
{
	ac_hide_pulloff0,
	ac_hide_pulloff1,
	ac_move_end,
};

//------------------------------------------
//	AC_ROCK_CRUSH
//------------------------------------------
static u8 ac_rock_crush0( EventOBJ *ev, actWork *act )
{
	ev_act_move_anm_wait_init( act, ROCK_CRUSH_ST, 0 );
	act->work[2] = 1;
	return( 0 );
}

static u8 ac_rock_crush1( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_anm_wait(act) )
	{
		ev_act_move_wait_init( act, 32 );
		act->work[2] = 2;
	}
	
	return( 0 );
}

static u8 ac_rock_crush2( EventOBJ *ev, actWork *act )
{
	ev->banish_sw ^= 1;
	
	if( ev_act_move_wait(act) )
	{
		ev->banish_sw = 1;
		act->work[2] = 3;
	}
	
	return( 0 );
}

static u8 (* const ac_rock_crush_tbl[])( EventOBJ *, actWork * ) =
{
	ac_rock_crush0,
	ac_rock_crush1,
	ac_rock_crush2,
	ac_move_end,
};

//------------------------------------------
//	AC_TREE_CUT
//------------------------------------------
static u8 ac_tree_cut0( EventOBJ *ev, actWork *act )
{
	ev_act_move_anm_wait_init( act, CUT_TREE_ST, 0 );
	act->work[2] = 1;
	return( 0 );
}

static u8 ac_tree_cut1( EventOBJ *ev, actWork *act )
{
	if( ev_act_move_anm_wait(act) )
	{
		ev_act_move_wait_init( act, 32 );
		act->work[2] = 2;
	}
	
	return( 0 );
}

static u8 ac_tree_cut2( EventOBJ *ev, actWork *act )
{
	ev->banish_sw ^= 1;
	
	if( ev_act_move_wait(act) )
	{
		ev->banish_sw = 1;
		act->work[2] = 3;
	}
	
	return( 0 );
}

static u8 (* const ac_tree_cut_tbl[])( EventOBJ *, actWork * ) =
{
	ac_tree_cut0,
	ac_tree_cut1,
	ac_tree_cut2,
	ac_move_end,
};

//------------------------------------------
//	AC_PRI_HOLD_SW_ON
//------------------------------------------
static u8 ac_pri_hold_sw_on( EventOBJ *ev, actWork *act )
{
	ev->priority_hold_sw = 1;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_pri_hold_sw_on_tbl[])( EventOBJ *, actWork * ) =
{
	ac_pri_hold_sw_on,
	ac_move_end,
};

//------------------------------------------
//	AC_PRI_HOLD_SW_OFF
//------------------------------------------
static u8 ac_pri_hold_sw_off( EventOBJ *ev, actWork *act )
{
	ev->priority_hold_sw = 0;
	act->work[2] = 1;
	return( 1 );
}

static u8 (* const ac_pri_hold_sw_off_tbl[])( EventOBJ *, actWork * ) =
{
	ac_pri_hold_sw_off,
	ac_move_end,
};

//------------------------------------------
//	AC_AFFUSE_ON
//------------------------------------------
static u8 ac_affuse_on( EventOBJ *ev, actWork *act )
{
	act->oamData.AffineMode = 3;
	AffineNumSet( act );
	act->aff_pause = 1;
	act->oampat_sw = OAMPAT_OFF;
	return( 1 );
}

static u8 (* const ac_affuse_on_tbl[])( EventOBJ *, actWork * ) =
{
	ac_affuse_on,
	ac_move_end,
};

//------------------------------------------
//	AC_AFFUSE_OFF
//------------------------------------------
static u8 ac_affuse_off( EventOBJ *ev, actWork *act )
{
	AffineWorkNumDel( act->oamData.AffineParamNo );
	act->oamData.AffineMode = 0;
	actCenterCalc( act, act->oamData.Shape, act->oamData.Size, act->oamData.AffineMode );
	return( 1 );
}

static u8 (* const ac_affuse_off_tbl[])( EventOBJ *, actWork * ) =
{
	ac_affuse_off,
	ac_move_end,
};

//------------------------------------------
//	AC_KIOG_WALK_S_D_05
//------------------------------------------
static u8 ac_kiog_walk_s_05_down1( EventOBJ *, actWork * );

static u8 ac_kiog_walk_s_05_down0( EventOBJ *ev, actWork *act )
{
	ac_walk05_set( ev, act, SITE_D );
	act->aff_pause = 0;
	ActAffAnmChkChg( act, 0 );
	return( ac_kiog_walk_s_05_down1( ev, act ) );
}

static u8 ac_kiog_walk_s_05_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk05(ev,act) )
	{
		act->aff_pause = 1;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_kiog_walk_s_05_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_kiog_walk_s_05_down0,
	ac_kiog_walk_s_05_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_KIOG_WALK_D_05
//------------------------------------------
static u8 ac_kiog_walk_05_down1( EventOBJ *, actWork * );

static u8 ac_kiog_walk_05_down0( EventOBJ *ev, actWork *act )
{
	ac_walk05_set( ev, act, SITE_D );
	act->aff_pause = 0;
	ActAffAnmHoldChkChg( act, 1 );
	return( ac_kiog_walk_05_down1( ev, act ) );
}

static u8 ac_kiog_walk_05_down1( EventOBJ *ev, actWork *act )
{
	if( ac_walk05(ev,act) )
	{
		act->aff_pause = 1;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_kiog_walk_05_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_kiog_walk_05_down0,
	ac_kiog_walk_05_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	FTUP �Œ����
//------------------------------------------
static void ac_ftup_direct_set( EventOBJ *ev, actWork *act, u8 site )
{
	EvObjSiteSet( ev, site );
	EvObjPosRenew( ev );
	objanm_stop_start( ev, act, EvActAnmNoWheelie(site) );
	act->anm_pause = 1;
	act->work[2] = 1;
}

//------------------------------------------
//	AC_MTB_FTUP_SITE_D
//------------------------------------------
static u8 ac_mtb_ftup_site_down( EventOBJ *ev, actWork *act )
{
	ac_ftup_direct_set( ev, act, SITE_D );
	return( 1 );
}

static u8 (* const ac_mtb_ftup_site_down_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftup_site_down,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTUP_SITE_U
//------------------------------------------
static u8 ac_mtb_ftup_site_up( EventOBJ *ev, actWork *act )
{
	ac_ftup_direct_set( ev, act, SITE_U );
	return( 1 );
}

static u8 (* const ac_mtb_ftup_site_up_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftup_site_up,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTUP_SITE_L
//------------------------------------------
static u8 ac_mtb_ftup_site_left( EventOBJ *ev, actWork *act )
{
	ac_ftup_direct_set( ev, act, SITE_L );
	return( 1 );
}

static u8 (* const ac_mtb_ftup_site_left_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftup_site_left,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTUP_SITE_R
//------------------------------------------
static u8 ac_mtb_ftup_site_right( EventOBJ *ev, actWork *act )
{
	ac_ftup_direct_set( ev, act, SITE_R );
	return( 1 );
}

static u8 (* const ac_mtb_ftup_site_right_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftup_site_right,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTUP_D
//------------------------------------------
static u8 ac_mtb_ftup_down( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_D, EvActAnmNoFtUp(SITE_D) );
	return( 0 );
}

static u8 (* const ac_mtb_ftup_down_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftup_down,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTUP_U
//------------------------------------------
static u8 ac_mtb_ftup_up( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_U, EvActAnmNoFtUp(SITE_U) );
	return( 0 );
}

static u8 (* const ac_mtb_ftup_up_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftup_up,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTUP_L
//------------------------------------------
static u8 ac_mtb_ftup_left( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_L, EvActAnmNoFtUp(SITE_L) );
	return( 0 );
}

static u8 (* const ac_mtb_ftup_left_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftup_left,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTUP_R
//------------------------------------------
static u8 ac_mtb_ftup_right( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_R, EvActAnmNoFtUp(SITE_R) );
	return( 0 );
}

static u8 (* const ac_mtb_ftup_right_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftup_right,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTDOWN_D
//------------------------------------------
static u8 ac_mtb_ftdown_down( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_D, EvActAnmNoFtDown(SITE_D) );
	return( 0 );
}

static u8 (* const ac_mtb_ftdown_down_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftdown_down,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTDOWN_U
//------------------------------------------
static u8 ac_mtb_ftdown_up( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_U, EvActAnmNoFtDown(SITE_U) );
	return( 0 );
}

static u8 (* const ac_mtb_ftdown_up_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftdown_up,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTDOWN_L
//------------------------------------------
static u8 ac_mtb_ftdown_left( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_L, EvActAnmNoFtDown(SITE_L) );
	return( 0 );
}

static u8 (* const ac_mtb_ftdown_left_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftdown_left,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_FTDOWN_R
//------------------------------------------
static u8 ac_mtb_ftdown_right( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_R, EvActAnmNoFtDown(SITE_R) );
	return( 0 );
}

static u8 (* const ac_mtb_ftdown_right_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_ftdown_right,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_RTDOWN_D
//------------------------------------------
static u8 ac_mtb_rtdown_down( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_D, EvActAnmNoRtDown(SITE_D) );
	return( 0 );
}

static u8 (* const ac_mtb_rtdown_down_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_rtdown_down,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_RTDOWN_U
//------------------------------------------
static u8 ac_mtb_rtdown_up( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_U, EvActAnmNoRtDown(SITE_U) );
	return( 0 );
}

static u8 (* const ac_mtb_rtdown_up_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_rtdown_up,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_RTDOWN_L
//------------------------------------------
static u8 ac_mtb_rtdown_left( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_L, EvActAnmNoRtDown(SITE_L) );
	return( 0 );
}

static u8 (* const ac_mtb_rtdown_left_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_rtdown_left,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_RTDOWN_R
//------------------------------------------
static u8 ac_mtb_rtdown_right( EventOBJ *ev, actWork *act )
{
	ac_anm_end_wait_site_set( ev, act, SITE_R, EvActAnmNoRtDown(SITE_R) );
	return( 0 );
}

static u8 (* const ac_mtb_rtdown_right_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_rtdown_right,
	ac_anm_end_wait1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	��ƴپ��
//------------------------------------------
static void ac_daniel_jump_set( EventOBJ *ev, actWork *act, u8 site, u8 type, u8 v_type )
{
	ac_jump_set_work_init( ev, act, site, type, v_type );
	ActAnmChkChg( act, EvActAnmNoFtUp(site) );
	EvEffShadeMake( ev );
}

//------------------------------------------
//	AC_MTB_DANIEL_D
//------------------------------------------
static u8 ac_mtb_daniel_down1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_down0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_D, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_6 );
	return( ac_mtb_daniel_down1(ev,act) );
}

static u8 ac_mtb_daniel_down1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_down_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_down0,
	ac_mtb_daniel_down1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_U
//------------------------------------------
static u8 ac_mtb_daniel_up1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_up0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_U, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_6 );
	return( ac_mtb_daniel_up1(ev,act) );
}

static u8 ac_mtb_daniel_up1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_up_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_up0,
	ac_mtb_daniel_up1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_L
//------------------------------------------
static u8 ac_mtb_daniel_left1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_left0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_L, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_6 );
	return( ac_mtb_daniel_left1(ev,act) );
}

static u8 ac_mtb_daniel_left1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_left_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_left0,
	ac_mtb_daniel_left1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_R
//------------------------------------------
static u8 ac_mtb_daniel_right1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_right0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_R, EVACT_MV_STAY_JUMP, EVACT_MV_JUMP_V_6 );
	return( ac_mtb_daniel_right1(ev,act) );
}

static u8 ac_mtb_daniel_right1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_right_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_right0,
	ac_mtb_daniel_right1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_D_1
//------------------------------------------
static u8 ac_mtb_daniel_down_1_1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_down_1_0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_D, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_6 );
	return( ac_mtb_daniel_down_1_1(ev,act) );
}

static u8 ac_mtb_daniel_down_1_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_down_1_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_down_1_0,
	ac_mtb_daniel_down_1_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_U_1
//------------------------------------------
static u8 ac_mtb_daniel_up_1_1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_up_1_0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_U, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_6 );
	return( ac_mtb_daniel_up_1_1(ev,act) );
}

static u8 ac_mtb_daniel_up_1_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_up_1_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_up_1_0,
	ac_mtb_daniel_up_1_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_L_1
//------------------------------------------
static u8 ac_mtb_daniel_left_1_1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_left_1_0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_L, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_6 );
	return( ac_mtb_daniel_left_1_1(ev,act) );
}

static u8 ac_mtb_daniel_left_1_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_left_1_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_left_1_0,
	ac_mtb_daniel_left_1_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_R_1
//------------------------------------------
static u8 ac_mtb_daniel_right_1_1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_right_1_0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_R, EVACT_MV_JUMP_1, EVACT_MV_JUMP_V_6 );
	return( ac_mtb_daniel_right_1_1(ev,act) );
}

static u8 ac_mtb_daniel_right_1_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_right_1_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_right_1_0,
	ac_mtb_daniel_right_1_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_D_2
//------------------------------------------
static u8 ac_mtb_daniel_down_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_down_2_0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_D, EVACT_MV_JUMP_2, EVACT_MV_JUMP_V_12 );
	return( ac_mtb_daniel_down_2_1(ev,act) );
}

static u8 ac_mtb_daniel_down_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_down_2_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_down_2_0,
	ac_mtb_daniel_down_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_U_2
//------------------------------------------
static u8 ac_mtb_daniel_up_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_up_2_0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_U, EVACT_MV_JUMP_2, EVACT_MV_JUMP_V_12 );
	return( ac_mtb_daniel_up_2_1(ev,act) );
}

static u8 ac_mtb_daniel_up_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}

	return( 0 );
}

static u8 (* const ac_mtb_daniel_up_2_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_up_2_0,
	ac_mtb_daniel_up_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_L_2
//------------------------------------------
static u8 ac_mtb_daniel_left_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_left_2_0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_L, EVACT_MV_JUMP_2, EVACT_MV_JUMP_V_12 );
	return( ac_mtb_daniel_left_2_1(ev,act) );
}

static u8 ac_mtb_daniel_left_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_left_2_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_left_2_0,
	ac_mtb_daniel_left_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_DANIEL_R_2
//------------------------------------------
static u8 ac_mtb_daniel_right_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_daniel_right_2_0( EventOBJ *ev, actWork *act )
{
	ac_daniel_jump_set( ev, act, SITE_R, EVACT_MV_JUMP_2, EVACT_MV_JUMP_V_12 );
	return( ac_mtb_daniel_right_2_1(ev,act) );
}

static u8 ac_mtb_daniel_right_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_jump(ev,act) )
	{
		ev->shade_act_sw = 0;
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_daniel_right_2_tbl[])( EventOBJ *, actWork * ) =
{
	ac_mtb_daniel_right_2_0,
	ac_mtb_daniel_right_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_D
//------------------------------------------
static u8 ac_mtb_wheelie_down( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_D, EvActAnmNoWheelie(SITE_D), 8 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_mtb_wheelie_down_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_down,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_U
//------------------------------------------
static u8 ac_mtb_wheelie_up( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_U, EvActAnmNoWheelie(SITE_U), 8 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_mtb_wheelie_up_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_up,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_L
//------------------------------------------
static u8 ac_mtb_wheelie_left( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_L, EvActAnmNoWheelie(SITE_L), 8 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_mtb_wheelie_left_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_left,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_R
//------------------------------------------
static u8 ac_mtb_wheelie_right( EventOBJ *ev, actWork *act )
{
	ac_stay_walk_set( ev, act, SITE_R, EvActAnmNoWheelie(SITE_R), 8 );
	return( ac_stay_walk_1(ev,act) );
}

static u8 (* const ac_mtb_wheelie_right_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_right,
	ac_stay_walk_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	��ذ�ړ����ı���
//------------------------------------------
static void ac_walk_mtb_wheelie_start_set( EventOBJ *ev, actWork *act, u8 site, u8 type )
{
	ac_walk_set_work_init( ev, act, site, type );
	ActAnmChg( act, EvActAnmNoFtUp(ev->site) );
	ActAnmOffsChg( act, 0 );
}

//------------------------------------------
//	AC_MTB_WHEELIE_S_D_2
//------------------------------------------
static u8 ac_mtb_wheelie_start_down_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_start_down_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_start_set( ev, act, SITE_D, EVACT_MV_2 );
	return( ac_mtb_wheelie_start_down_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_start_down_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_start_down_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_start_down_2_0,
	ac_mtb_wheelie_start_down_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_S_U_2
//------------------------------------------
static u8 ac_mtb_wheelie_start_up_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_start_up_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_start_set( ev, act, SITE_U, EVACT_MV_2 );
	return( ac_mtb_wheelie_start_up_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_start_up_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_start_up_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_start_up_2_0,
	ac_mtb_wheelie_start_up_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_S_L_2
//------------------------------------------
static u8 ac_mtb_wheelie_start_left_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_start_left_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_start_set( ev, act, SITE_L, EVACT_MV_2 );
	return( ac_mtb_wheelie_start_left_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_start_left_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_start_left_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_start_left_2_0,
	ac_mtb_wheelie_start_left_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_S_R_2
//------------------------------------------
static u8 ac_mtb_wheelie_start_right_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_start_right_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_start_set( ev, act, SITE_R, EVACT_MV_2 );
	return( ac_mtb_wheelie_start_right_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_start_right_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_start_right_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_start_right_2_0,
	ac_mtb_wheelie_start_right_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	��ذ�ړ�
//------------------------------------------
static void ac_walk_mtb_wheelie_set( EventOBJ *ev, actWork *act, u8 site, u8 type )
{
	ac_walk_set_work_init( ev, act, site, type );
	objanm_walk_start( ev, act, EvActAnmNoWheelie(ev->site) );
}

//------------------------------------------
//	AC_MTB_WHEELIE_D_2
//------------------------------------------
static u8 ac_mtb_wheelie_down_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_down_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_set( ev, act, SITE_D, EVACT_MV_2 );
	return( ac_mtb_wheelie_down_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_down_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_down_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_down_2_0,
	ac_mtb_wheelie_down_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_U_2
//------------------------------------------
static u8 ac_mtb_wheelie_up_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_up_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_set( ev, act, SITE_U, EVACT_MV_2 );
	return( ac_mtb_wheelie_up_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_up_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_up_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_up_2_0,
	ac_mtb_wheelie_up_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_L_2
//------------------------------------------
static u8 ac_mtb_wheelie_left_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_left_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_set( ev, act, SITE_L, EVACT_MV_2 );
	return( ac_mtb_wheelie_left_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_left_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_left_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_left_2_0,
	ac_mtb_wheelie_left_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_R_2
//------------------------------------------
static u8 ac_mtb_wheelie_right_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_right_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_set( ev, act, SITE_R, EVACT_MV_2 );
	return( ac_mtb_wheelie_right_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_right_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_right_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_right_2_0,
	ac_mtb_wheelie_right_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	��ذ�ړ������޳�
//------------------------------------------
static void ac_walk_mtb_wheelie_end_set( EventOBJ *ev, actWork *act, u8 site, u8 type )
{
	ac_walk_set_work_init( ev, act, site, type );
	ActAnmChg( act, EvActAnmNoFtDown(ev->site) );
	ActAnmOffsChg( act, 0 );
}

//------------------------------------------
//	AC_MTB_WHEELIE_E_D_2
//------------------------------------------
static u8 ac_mtb_wheelie_end_down_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_end_down_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_end_set( ev, act, SITE_D, EVACT_MV_2 );
	return( ac_mtb_wheelie_end_down_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_end_down_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_end_down_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_end_down_2_0,
	ac_mtb_wheelie_end_down_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_E_U_2
//------------------------------------------
static u8 ac_mtb_wheelie_end_up_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_end_up_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_end_set( ev, act, SITE_U, EVACT_MV_2 );
	return( ac_mtb_wheelie_end_up_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_end_up_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_end_up_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_end_up_2_0,
	ac_mtb_wheelie_end_up_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_E_L_2
//------------------------------------------
static u8 ac_mtb_wheelie_end_left_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_end_left_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_end_set( ev, act, SITE_L, EVACT_MV_2 );
	return( ac_mtb_wheelie_end_left_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_end_left_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_end_left_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_end_left_2_0,
	ac_mtb_wheelie_end_left_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	AC_MTB_WHEELIE_E_R_2
//------------------------------------------
static u8 ac_mtb_wheelie_end_right_2_1( EventOBJ *, actWork * );

static u8 ac_mtb_wheelie_end_right_2_0( EventOBJ *ev, actWork *act )
{
	ac_walk_mtb_wheelie_end_set( ev, act, SITE_R, EVACT_MV_2 );
	return( ac_mtb_wheelie_end_right_2_1( ev, act ) );
}

static u8 ac_mtb_wheelie_end_right_2_1( EventOBJ *ev, actWork *act )
{
	if( ac_walk(ev,act) )
	{
		act->work[2] = 2;
		return( 1 );
	}
	
	return( 0 );
}

static u8 (* const ac_mtb_wheelie_end_right_2_tbl[])(EventOBJ *,actWork *) =
{
	ac_mtb_wheelie_end_right_2_0,
	ac_mtb_wheelie_end_right_2_1,
	ac_move_end_anm_pause,
};

//------------------------------------------
//	�I��
//------------------------------------------
static u8 ac_move_end( EventOBJ *ev, actWork *act )
{
	return( 1 );
}

//------------------------------------------
//	�I�� + �ٱ���߰��
//------------------------------------------
static u8 ac_move_end_anm_pause( EventOBJ *ev, actWork *act )
{
	act->anm_pause = 1;
	return( 1 );
}

//======================================================================
//		����
//======================================================================
static void scroll_banish( EventOBJ *, actWork * );
static void banish_set( EventOBJ *, actWork * );

//------------------------------------------
//	�ٱ���߰��
//------------------------------------------
static void EvObjAnmPause( EventOBJ *ev, actWork *act )
{
	if( ev->anm_pause_sw )
	{
		act->anm_pause = 1;
	}
}

//------------------------------------------
//	����߰�޸ر
//------------------------------------------
static void EvObjAnmPauseClr( EventOBJ *ev, actWork *act )
{
	if( ev->anm_pause_clr_sw )
	{
		act->anm_pause = 0;
		ev->anm_pause_sw = 0;
		ev->anm_pause_clr_sw = 0;
	}
}

//------------------------------------------
//	��Ư��
//------------------------------------------
void EvObjBanish( EventOBJ *ev, actWork *act )
{
	scroll_banish( ev, act );
	banish_set( ev, act );
}

//------------------------------------------
//	��۰���Ư��
//------------------------------------------
static void scroll_banish( EventOBJ *ev, actWork *act )
{
	SXYObjHead *objst;
	s16	x0,y0,x1,y1;

	ev->scr_banish_sw = 0;

	objst = SxyObjHeadGet( ev->obj_code );
	
	if( act->scr )
	{
		x0 = act->x + act->dx + act->cx + ActOX;
		y0 = act->y + act->dy + act->cy + ActOY;
	}
	else
	{
		x0 = act->x + act->dx + act->cx;
		y0 = act->y + act->dy + act->cy;
	}
	
	x1 = (s16)((u16)x0 + (u16)(objst->x_size));
	y1 = (s16)((u16)y0 + (u16)(objst->y_size));
	
	if( x0 >= 256 || x1 < -16 )
	{
		ev->scr_banish_sw = 1;
	}

	if( y0 >= 176 || y1 < -16 )
	{
		ev->scr_banish_sw = 1;
	}
}

//------------------------------------------
//	��Ư�����f
//------------------------------------------
static void banish_set( EventOBJ *ev, actWork *act )
{
	act->banish = 0;

	if( ev->banish_sw || ev->scr_banish_sw )
	{
		act->banish = 1;
	}
}

//======================================================================
//		ϯ�߱���ޭ��
//======================================================================
static void MapAttrSet( EventOBJ * );
static void MapAttrReflect_01( EventOBJ *, u32 * );
static void MapAttrGrass_0( EventOBJ *,  u32 * );
static void MapAttrGrass_1( EventOBJ *, u32 * );
static void MapAttrLongGrass_0( EventOBJ *, u32 * );
static void MapAttrLongGrass_1( EventOBJ *, u32 * );
static void MapAttrSand_1( EventOBJ *, u32 * );
static void MapAttrDeepSand_012( EventOBJ *, u32 * );
static void MapAttrShoal_012( EventOBJ *, u32 * );
static void MapAttrPool_12( EventOBJ *, u32 * );
static void MapAttrHamon_2( EventOBJ *, u32 * );
static void MapAttrSmallGrass_012( EventOBJ *, u32 * );
static void MapAttrOnsen_012( EventOBJ *, u32 * );
static void MapAttrAwa_2( EventOBJ *, u32 * );
static void MapAttrJumpEnd( EventOBJ *, u32 * );
static u8 MapAttrReflectCheck( EventOBJ * );
static u8 map_attr_reflect( u8 );

//------------------------------------------
//	ϯ�߱���ޭ�� ���쏉�� 0
//------------------------------------------
static void EvObjMoveFirstMapAttr( EventOBJ *ev, u32 *set )
{
	MapAttrSet( ev );
	MapAttrReflect_01( ev, set );
	MapAttrGrass_0( ev, set );
	MapAttrLongGrass_0( ev, set );
	MapAttrDeepSand_012( ev, set );
	MapAttrShoal_012( ev, set );
	MapAttrSmallGrass_012( ev, set );
	MapAttrOnsen_012( ev, set );
}

//------------------------------------------
//	ϯ�߱���ޭ�� ����J�n 1
//------------------------------------------
static void EvObjMoveStartMapAttr( EventOBJ *ev, u32 *set )
{
	MapAttrSet( ev );
	MapAttrReflect_01( ev, set );
	MapAttrGrass_1( ev, set );
	MapAttrLongGrass_1( ev, set );
	MapAttrSand_1( ev, set );
	MapAttrDeepSand_012( ev, set );
	MapAttrShoal_012( ev, set );
	MapAttrPool_12( ev, set );
	MapAttrSmallGrass_012( ev, set );
	MapAttrOnsen_012( ev, set );
}

//------------------------------------------
//	ϯ�߱���ޭ�� ����I�� 2
//------------------------------------------
static void EvObjMoveEndMapAttr( EventOBJ *ev, u32 *set )
{
	MapAttrSet( ev );
	MapAttrShoal_012( ev, set );
	MapAttrDeepSand_012( ev, set );
	MapAttrPool_12( ev, set );
	MapAttrHamon_2( ev, set );
	MapAttrSmallGrass_012( ev, set );
	MapAttrOnsen_012( ev, set );
	MapAttrAwa_2( ev, set );
	MapAttrJumpEnd( ev, set );
}

//------------------------------------------
//	ϯ�߱���ޭ�ľ��
//------------------------------------------
static void MapAttrSet( EventOBJ *ev )
{
	ev->old_attr = GetCellAttribute( ev->old_gx, ev->old_gy );
	ev->now_attr = GetCellAttribute( ev->gx, ev->gy );
}

//------------------------------------------
//	����ޭ�� �f�荞��
//------------------------------------------
static void MapAttrReflect_01( EventOBJ *ev, u32 *set )
{
	u32 tbl[] = { SETOBJ_ReflectIce, SETOBJ_Reflect };
	u8 flag;

	flag = MapAttrReflectCheck( ev );
	
	if( flag )
	{
		if( ev->attr_reflect_sw == 0 )
		{
			ev->attr_reflect_sw++;
			*set |= tbl[flag-1];
		}
	}
	else
	{
		ev->attr_reflect_sw = 0;
	}
}

//------------------------------------------
//	����ޭ�� �� �J�n
//------------------------------------------
static void MapAttrGrass_0( EventOBJ *ev,  u32 *set )
{
	if( ATR_ShortGrassCheck(ev->now_attr) )
	{
		*set |= SETOBJ_ShortGrassFirst;
	}
}

//------------------------------------------
//	����ޭ�� ��
//------------------------------------------
static void MapAttrGrass_1( EventOBJ *ev, u32 *set )
{
	if( ATR_ShortGrassCheck(ev->now_attr) )
	{
		*set |= SETOBJ_ShortGrass;
	}
}

//------------------------------------------
//	����ޭ�� ������ �J�n
//------------------------------------------
static void MapAttrLongGrass_0( EventOBJ *ev, u32 *set )
{
	if( ATR_LongGrassCheck(ev->now_attr) )
	{
		*set |= SETOBJ_LongGrassFirst;
	}
}

//------------------------------------------
//	����ޭ�� ������
//------------------------------------------
static void MapAttrLongGrass_1( EventOBJ *ev, u32 *set )
{
	if( ATR_LongGrassCheck(ev->now_attr) )
	{
		*set |= SETOBJ_LongGrass;
	}
}

//------------------------------------------
//	����ޭ�� ����
//------------------------------------------
static void MapAttrSand_1( EventOBJ *ev, u32 *set )
{
	if( ATR_IsDeepSandCheck(ev->old_attr) )
	{
		*set |= SETOBJ_FootDeep;
	}
	else if( ATR_IsSandCheck(ev->old_attr) ||
			ATR_AshGroundCheck(ev->old_attr) )
	{
		*set |= SETOBJ_Foot;
	}
}

//------------------------------------------
//	����ޭ�� �[����
//------------------------------------------
static void MapAttrDeepSand_012( EventOBJ *ev, u32 *set )
{
	if( ATR_IsDeepSandCheck(ev->now_attr) &&
		ATR_IsDeepSandCheck(ev->old_attr) )
	{
		if( ev->attr_sand_sw == 0 )
		{
			ev->attr_sand_sw++;
			*set |= SETOBJ_DeepSand;
		}
	}
	else
	{
		ev->attr_sand_sw = 0;
	}
}

//------------------------------------------
//	����ޭ�� ��
//------------------------------------------
static void MapAttrShoal_012( EventOBJ *ev, u32 *set )
{
	if( (ATR_ShoalCheck(ev->now_attr) && ATR_ShoalCheck(ev->old_attr)) ||
		(ATR_IsLogBridgeCheck(ev->now_attr) && ATR_IsLogBridgeCheck(ev->old_attr)) )
	{
		if( ev->attr_shoal_sw == 0 )
		{
			ev->attr_shoal_sw++;
			*set |= SETOBJ_Shoal;
		}
	}
	else
	{
		ev->attr_shoal_sw = 0;
	}
}

//------------------------------------------
//	����ޭ�� ������
//------------------------------------------
static void MapAttrPool_12( EventOBJ *ev, u32 *set )
{
	if( ATR_PoolCheck(ev->now_attr) &&
		ATR_PoolCheck(ev->old_attr) )
	{
		*set |= SETOBJ_Pool;
	}
}

//------------------------------------------
//	����ޭ�� �g��
//------------------------------------------
static void MapAttrHamon_2( EventOBJ *ev, u32 *set )
{
	if( ATR_HamonCheck(ev->now_attr) )
	{
		*set |= SETOBJ_Hamon;
	}
}

//------------------------------------------
//	����ޭ�� ��������
//------------------------------------------
static void MapAttrSmallGrass_012( EventOBJ *ev, u32 *set )
{
	if( ATR_IsSmallGrassCheck(ev->now_attr) &&
		ATR_IsSmallGrassCheck(ev->old_attr) )
	{
		if( ev->attr_grass_sw == 0 )
		{
			ev->attr_grass_sw++;
			*set |= SETOBJ_SmallGrass;
		}
	}
	else
	{
		ev->attr_grass_sw = 0;
	}
}

//------------------------------------------
//	����ޭ�� ����
//------------------------------------------
static void MapAttrOnsen_012( EventOBJ *ev, u32 *set )
{
	if( ATR_IsOnsenCheck(ev->now_attr) &&
		ATR_IsOnsenCheck(ev->old_attr) )
	{
		if( ev->attr_onsen_sw == 0 )
		{
			ev->attr_onsen_sw++;
			*set |= SETOBJ_Onsen;
		}
	}
	else
	{
		ev->attr_onsen_sw = 0;
	}
}

//------------------------------------------
//	����ޭ�� �C��
//------------------------------------------
static void MapAttrAwa_2( EventOBJ *ev, u32 *set )
{
	if( ATR_IsSeaWeedCheck(ev->now_attr) )
	{
		*set |= SETOBJ_Awa;
	}
}

//------------------------------------------
//	�ެ��ߴ��� ����ޭ������
//------------------------------------------
#define JUMPEND_MAP_ATTR_CHK_MAX 6

static u8 (* const jump_end_attr_check_tbl[])(u8) =
{
	ATR_ShortGrassCheck,
	ATR_LongGrassCheck,
	ATR_PoolCheck,
	ATR_IsWaterCheck,
	ATR_ShoalCheck,
	ATR_WalkEnableCheck,
};

static const u32 jump_end_obj_code_tbl[] =
{
	SETOBJ_JumpEndShortGrass,
	SETOBJ_JumpEndLongGrass,
	SETOBJ_JumpEndPool,
	SETOBJ_JumpEndWater,
	SETOBJ_JumpEndPool,
	SETOBJ_JumpEndDust
};

//------------------------------------------
//	����ޭ�� �ެ��ߴ���
//------------------------------------------
static void MapAttrJumpEnd( EventOBJ *ev, u32 *set )
{
	u8 i;

	if( ev->move_jump_end_sw == 0 || ev->under_effect_sw )
	{
		return;
	}

	for( i = 0; i < JUMPEND_MAP_ATTR_CHK_MAX; i++ )
	{
		if( jump_end_attr_check_tbl[i](ev->now_attr) )
		{
			*set |= jump_end_obj_code_tbl[i];
			return;
		}
	}
}

//------------------------------------------
//	����ޭ������	�f�荞��
//------------------------------------------
static u8 MapAttrReflectCheck( EventOBJ *ev )
{
	s16	x_calc,y_calc,x,y,cy;
	SXYObjHead *objst;
	u8 attr,flag;

	objst = SxyObjHeadGet( ev->obj_code );
	x_calc = (objst->x_size + 8) >> 4;					// size / 16
	y_calc = (objst->y_size + 8) >> 4;					// + 8 = 1 origin
//	cy = (y_calc + 1) >> 1;								// size / 2, + 1 = 1 origin
	cy = 1;												// under + 1

	for( y = 0; y < y_calc; y++ )
	{													// ���S
		attr = GetCellAttribute( ev->gx, ev->gy + cy + y );
		flag = map_attr_reflect( attr );
		if( flag ) return( flag );

		attr = GetCellAttribute( ev->old_gx, ev->old_gy + cy + y );
		flag = map_attr_reflect( attr );
		if( flag ) return( flag );

		for( x = 1; x < x_calc; x++ )					// ���E
		{
			attr = GetCellAttribute( ev->gx + x, ev->gy + cy + y );
			flag = map_attr_reflect( attr );
			if( flag ) return( flag );

			attr = GetCellAttribute( ev->gx - x, ev->gy + cy + y );
			flag = map_attr_reflect( attr );
			if( flag ) return( flag );

			attr = GetCellAttribute( ev->old_gx + x, ev->old_gy + cy + y );
			flag = map_attr_reflect( attr );
			if( flag ) return( flag );

			attr = GetCellAttribute( ev->old_gx - x, ev->old_gy + cy + y );
			flag = map_attr_reflect( attr );
			if( flag ) return( flag );
		}
	}

	return( 0 );
}

static u8 map_attr_reflect( u8 attr )
{
	if( ATR_IsReflectIceCheck(attr) ) return( 1 );
	if( ATR_IsReflectCheck(attr) ) return( 2 );
	return( 0 );
}

//======================================================================
//		����ޭ�Ċ֘A
//======================================================================
//------------------------------------------
//	�ެ��߱���ޭ������ð���
//------------------------------------------
static u8 (* const EvMapAttrJumpChkTbl[])( u8 ) =
{
	ATR_JumpDownEnableCheck, ATR_JumpUpEnableCheck,
	ATR_JumpLeftEnableCheck, ATR_JumpRightEnableCheck
};

//------------------------------------------
//	�ެ��߱���ޭ������
//	In:		x,y			; ϯ�ߍ��W
//			site		; �ړ�����
//	Out:	u8			; 0 = �ެ��ߕs��
//						; On = �ެ��ߕ�����Ԃ�
//------------------------------------------
u8 EvMapAttrJumpChk( s16 x, s16 y, u8 site )
{
	u8	atr;

	if( site == SITE_N )
	{
		return( 0 );
	}

	if( site >= SITE_DL )
	{
		site -= SITE_DL - 1;
	}

	site--;

	atr = GetCellAttribute( x, y );

	if( EvMapAttrJumpChkTbl[site](atr) == TRUE )
	{
		return( site + 1 );
	}

	return( 0 );
}

//------------------------------------------
//	����ޭ����ײ��è
//------------------------------------------
static void EvObjMapAttrPri( EventOBJ *ev, actWork *act )
{
	if( ev->move_jump_start_sw == 0 &&
		ATR_LongGrassCheck(ev->now_attr) && ATR_LongGrassCheck(ev->old_attr) )
	{
		act->oampat_no = EV_OAMPAT_LONG_GRASS_BGPRI2;

		if( EvObjHeightBgPri(ev->height_pri) == BGPRI1 )
		{
			act->oampat_no = EV_OAMPAT_LONG_GRASS_BGPRI1;
		}
	}
}

//======================================================================
//		����
//======================================================================
//------------------------------------------
//	���� ���� �� ��ײ��è
//------------------------------------------
static const u8 HeightActPriTbl[16] =
{
#if DEBUG_HEIGHT == 0
	0,					// 0 ����
#else
	EVOBJ_ACTPRI1,		// 0 ����(���Ώ�)
#endif
	EVOBJ_ACTPRI1,		// 1 ����
	EVOBJ_ACTPRI0,		// 2 ����(����\��)
	EVOBJ_ACTPRI1,		// 3 �n��
	EVOBJ_ACTPRI0,		// 4 ����
	EVOBJ_ACTPRI1,		// 5
	EVOBJ_ACTPRI0,		// 6
	EVOBJ_ACTPRI1,		// 7
	EVOBJ_ACTPRI0,		// 8
	EVOBJ_ACTPRI1,		// 9
	EVOBJ_ACTPRI0,		// A
	EVOBJ_ACTPRI1,		// B
	EVOBJ_ACTPRI0,		// C 
	0,					// D ����
	0,					// E ����
#if DEBUG_HEIGHT == 0
	0,					// F ����
#else
	EVOBJ_ACTPRI1,		// F ����(���Ώ�)
#endif
};

//------------------------------------------
//	�����T�O BG �� ��ײ��è
//------------------------------------------
static const u8 HeightBgPriTbl[16] =
{
#if DEBUG_HEIGHT == 0
	0,					// 0 ����
#else
	BGPRI2,				// 0 ����(���Ώ�)
#endif
	BGPRI2,				// 1 ����
	BGPRI2,				// 2 ����(����\��)
	BGPRI2,				// 3 �n��
	BGPRI1,				// 4 ����
	BGPRI2,				// 5
	BGPRI1,				// 6
	BGPRI2,				// 7
	BGPRI1,				// 8
	BGPRI2,				// 9
	BGPRI1,				// A
	BGPRI2,				// B
	BGPRI1,				// C 
	0,					// D ����
	0,					// E ����
#if DEBUG_HEIGHT == 0
	0,					// F ����
#else
	BGPRI2,				// F ����(���Ώ�)
#endif
};

//------------------------------------------
//	�����T�O OAM �߯� No
//------------------------------------------
static const u8 HeightOamPatTbl[16] =
{
#if DEBUG_HEIGHT == 0
	0,					// 0 ����
#else
	EV_OAMPAT_NORMAL,	// 0 ����(���Ώ�)
#endif
	EV_OAMPAT_NORMAL,	// 1 ����
	EV_OAMPAT_NORMAL,	// 2 ����(����\��)
	EV_OAMPAT_NORMAL,	// 3 �n��
	EV_OAMPAT_HEIGHT,	// 4 ����
	EV_OAMPAT_NORMAL,	// 5
	EV_OAMPAT_HEIGHT,	// 6
	EV_OAMPAT_NORMAL,	// 7
	EV_OAMPAT_HEIGHT,	// 8
	EV_OAMPAT_NORMAL,	// 9
	EV_OAMPAT_HEIGHT,	// A
	EV_OAMPAT_NORMAL,	// B
	EV_OAMPAT_HEIGHT,	// C 
	0,					// D ����
	0,					// E ����
#if DEBUG_HEIGHT == 0
	0,					// F ����
#else
	EV_OAMPAT_NORMAL,	// F ����(���Ώ�)
#endif
};

//------------------------------------------
//	�����Ԉړ�����
//------------------------------------------
static u8 HeightMoveChk( u8 height, s16 x, s16 y )
{
	u8 next_h;
	
	if( height == MAP_HEIGHT_FREE )
	{
		return( 0 );									// �ړ����Ȃ�
	}

	next_h = GetCellHeight( x, y );

	if( next_h == MAP_HEIGHT_FREE || next_h == MAP_HEIGHT_HOLD )
	{
		return( 0 );									// ���Ȃ�
	}

	if( next_h != height )
	{
		return( 1 );									// ���፷��
	}

	return( 0 );
}

//------------------------------------------
//	�������
//------------------------------------------
static void EvObjHeightSet( EventOBJ *ev, actWork *act )
{
	if( ev->priority_hold_sw == 0 )
	{
		EvObjHeight( ev );								// �������
		act->oampat_no = HeightOamPatTbl[ev->height_pri];
		act->oamData.Priority = HeightBgPriTbl[ev->height_pri];
	}
}

//------------------------------------------
//	������������
//	In:		act			; ��ı��
//			height		; ����
//------------------------------------------
void EvActHeightSet( actWork *act, u8 height )
{
	act->oampat_no = HeightOamPatTbl[height];
	act->oamData.Priority = HeightBgPriTbl[height];
}

//------------------------------------------
//	����BG��ײ��è�擾
//	In:		h			;	 ����
//------------------------------------------
u8 EvObjHeightBgPri( u8 h )
{
	return( HeightBgPriTbl[h] );
}

//------------------------------------------
//	�������f
//	In:		ev				; ���OBJ
//------------------------------------------
void EvObjHeight( EventOBJ *ev )
{
	u8 now_h,old_h;

	now_h = GetCellHeight( ev->gx, ev->gy );			// ����
	old_h = GetCellHeight( ev->old_gx, ev->old_gy );

	if( now_h == MAP_HEIGHT_HOLD || old_h == MAP_HEIGHT_HOLD )
	{
		return;											// �����ێ�
	}

	ev->height = now_h;

	if( now_h == MAP_HEIGHT_FREE || now_h == MAP_HEIGHT_HOLD )
	{
		return;											// �����ذ or �ێ�
	}

	ev->height_pri = now_h;								// �������
}

//------------------------------------------
//	������ײ��è���۰�
//	In:		height			; ����
//			act				; ��ı��
//			offs_pri		; �D�揇��(0,1)
//------------------------------------------
void EvActPriControl( u8 height, actWork *act, u8 offs_pri )
{
	u16 y;

	y = (act->y - act->cy + ActOY + 8) & 0xff;
	y = (16 - (y >> 4)) << 1;							// (y / 16) * 2

	act->pri = HeightActPriTbl[height] + y + offs_pri;
}

//------------------------------------------
//	�����OBJ ������ײ��è���۰�
//------------------------------------------
static void EvObjPriControl( EventOBJ *ev, actWork *act )
{
	if( ev->priority_hold_sw == 0 )
	{
		EvActPriControl( ev->height_pri, act, 1 );
	}
}

//------------------------------------------
//	������ �����蔻��
//	In:		h0			; �ړ�����
//			h1			; �ړ���̍���
//	Out:	u8			; On = ������
//------------------------------------------
u8 HeightHitChk( u8 h0, u8 h1 )
{
	if( h0 == MAP_HEIGHT_FREE || h1 == MAP_HEIGHT_FREE || h0 == h1 )
	{
		return( 1 );
	}
	
	return( 0 );
}


//======================================================================
//		���씭����޼ު��
//======================================================================
//------------------------------------------
//	�Z����,�J�n
//------------------------------------------
static void SetObjectShortGrassFirst( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet8( FECODE_ShortGrassShake, ev->gx, ev->gy, ev->height_pri, BGPRI2,
	(ev->objID<<8)|ev->mapID,ev->divID,((u8)Fld.MapID.map_id<<8)|(u8)Fld.MapID.div_id, 1 );
}

//------------------------------------------
//	�Z����
//------------------------------------------
static void SetObjectShortGrass( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet8( FECODE_ShortGrassShake, ev->gx, ev->gy, ev->height_pri, BGPRI2,
	(ev->objID<<8)|ev->mapID,ev->divID,((u8)Fld.MapID.map_id<<8)|(u8)Fld.MapID.div_id, 0 );
}

//------------------------------------------
//	������ �J�n
//------------------------------------------
static void SetObjectLongGrassFirst( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet8( FECODE_LongGrassShake, ev->gx, ev->gy, ev->height_pri, BGPRI2,
	(ev->objID<<8)|ev->mapID,ev->divID,((u8)Fld.MapID.map_id<<8)|(u8)Fld.MapID.div_id, 1 );
}

//------------------------------------------
//	������
//------------------------------------------
static void SetObjectLongGrass( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet8( FECODE_LongGrassShake, ev->gx, ev->gy, ev->height_pri, BGPRI2,
	(ev->objID<<8)|ev->mapID,ev->divID,((u8)Fld.MapID.map_id<<8)|(u8)Fld.MapID.div_id, 0 );
}

//------------------------------------------
//	�f�荞��
//------------------------------------------
static void SetObjectReflect( EventOBJ *ev, actWork *act )
{
	EvActReflectSet( ev, act, EOE_REF_NORMAL );
}

//------------------------------------------
//	�f�荞�� �X
//------------------------------------------
static void SetObjectReflectIce( EventOBJ *ev, actWork *act )
{
	EvActReflectSet( ev, act, EOE_REF_ICE );
}

//------------------------------------------
//	��
//------------------------------------------
static void SetObjectShoal( EventOBJ *ev, actWork *act )
{
	EvEffWorkSetId( FECODE_Shoal, ev );
}

//------------------------------------------
//	����ð���
//------------------------------------------
static void foot_nothing( EventOBJ *, actWork *, u8 );
static void foot_normal( EventOBJ *, actWork *, u8 );
static void foot_cycle( EventOBJ *, actWork *, u8 );

static void (* const FootTbl[])( EventOBJ *, actWork *, u8 ) =
{
	foot_nothing,
	foot_normal,
	foot_cycle,
};

//------------------------------------------
//	����
//------------------------------------------
static void SetObjectFoot( EventOBJ *ev, actWork *act )
{
	SXYObjHead *objst;
	
	objst = SxyObjHeadGet( ev->obj_code );
	
	FootTbl[objst->footprint]( ev, act, 0 );			// ��
}

static void SetObjectFootDeep( EventOBJ *ev, actWork *act )
{
	SXYObjHead *objst;
	
	objst = SxyObjHeadGet( ev->obj_code );
	
	FootTbl[objst->footprint]( ev, act, 1 );			// �[����
}

//------------------------------------------
//	���� ����
//------------------------------------------
static void foot_nothing( EventOBJ *ev, actWork *act, u8 kind )
{
}

//------------------------------------------
//	���� �l�^
//------------------------------------------
static void foot_normal( EventOBJ *ev, actWork *act, u8 kind )
{
	u16 tbl[] = { FECODE_FootPrint, FECODE_FootPrintDeep };

	EvEffWorkSet5( tbl[kind], ev->old_gx, ev->old_gy,
					EVOBJ_LOW_ACTPRI1, BGPRI2, ev->site );
}

//------------------------------------------
//	���� ���]��
//------------------------------------------
static const u8 footprint_cycle_tbl[4][4];

static void foot_cycle( EventOBJ *ev, actWork *act, u8 kind )
{
	if( ev->gx == ev->old_gx && ev->gy == ev->old_gy )
	{
		return;
	}

	EvEffWorkSet5( FECODE_FootPrintCycle, ev->old_gx, ev->old_gy,
		EVOBJ_LOW_ACTPRI1, BGPRI2, footprint_cycle_tbl[ev->old_site-1][ev->site-1] );
}

static const u8 footprint_cycle_tbl[4][4] =
{
	{ SITE_D, SITE_U, FOOT_SITE_DL, FOOT_SITE_LU },
	{ SITE_D, SITE_U, FOOT_SITE_RD, FOOT_SITE_UR },
	{ FOOT_SITE_UR, FOOT_SITE_LU, SITE_L, SITE_R },
	{ FOOT_SITE_RD, FOOT_SITE_DL, SITE_L, SITE_R },
};

//------------------------------------------
//	���g��
//------------------------------------------
static void SetObjectHamon( EventOBJ *ev, actWork *act )
{
	EvEffMizutamaMake( ev, act );
}

//------------------------------------------
//	������
//------------------------------------------
static void SetObjectPood( EventOBJ *ev, actWork *act )
{
	EvEffWorkSetId( FECODE_Splash, ev );
}

//------------------------------------------
//	�[����
//------------------------------------------
static void SetObjectDeepSand( EventOBJ *ev, actWork *act )
{
	EvEffWorkSetId( FECODE_DeepSand, ev );
}

//------------------------------------------
//	�Z������
//------------------------------------------
static void SetObjectJumpEndShortGrass( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet4( FECODE_ShortGrassFall, ev->gx, ev->gy, ev->height_pri, BGPRI2 );

	if( CheckFeShortGrass(ev->objID,ev->mapID,ev->divID,ev->gx,ev->gy) == ACT_MAX )
	{
		SetObjectShortGrassFirst( ev, act );
	}
}

//------------------------------------------
//	��������
//------------------------------------------
static void SetObjectJumpEndLongGrass( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet4( FECODE_LongGrassFall, ev->gx, ev->gy, ev->height_pri, BGPRI2 );
}

//------------------------------------------
//	�����萅��
//------------------------------------------
static void SetObjectJumpEndPool( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet4( FECODE_SplashWater, ev->gx, ev->gy, ev->height_pri, act->oamData.Priority );
}

//------------------------------------------
//	����
//------------------------------------------
static void SetObjectJumpEndWater( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet4( FECODE_SplashSea, ev->gx, ev->gy, ev->height_pri, act->oamData.Priority );
}

//------------------------------------------
//	��
//------------------------------------------
static void SetObjectJumpEndDust( EventOBJ *ev, actWork *act )
{
	EvEffWorkSet4( FECODE_Hokori, ev->gx, ev->gy, ev->height_pri, act->oamData.Priority );
}

//------------------------------------------
//	��������
//------------------------------------------
static void SetObjectSmallGrass( EventOBJ *ev, actWork *act )
{
	EvEffWorkSetId( FECODE_SmallGrass, ev );
}

//------------------------------------------
//	����
//------------------------------------------
static void SetObjectOnsen( EventOBJ *ev, actWork *act )
{
	EvEffWorkSetId( FECODE_Onsen, ev );
}

//------------------------------------------
//	�A
//------------------------------------------
static void SetObjectAwa( EventOBJ *ev, actWork *act )
{
	FeSetWork[0] = ev->gx;
	FeSetWork[1] = ev->gy;
	FldEffSet( FECODE_Awa );
}

//------------------------------------------
//	���씭��OBJð���
//------------------------------------------
static void (* const SetObjectTbl[SETOBJ_MAX])(EventOBJ*,actWork*) =
{
	SetObjectShortGrassFirst,
	SetObjectShortGrass,
	SetObjectLongGrassFirst,
	SetObjectLongGrass,
	SetObjectReflect,
	SetObjectReflectIce,
	SetObjectShoal,
	SetObjectFoot,
	SetObjectFootDeep,
	SetObjectHamon,
	SetObjectPood,
	SetObjectDeepSand,
	SetObjectJumpEndShortGrass,
	SetObjectJumpEndLongGrass,
	SetObjectJumpEndPool,
	SetObjectJumpEndWater,
	SetObjectJumpEndDust,
	SetObjectSmallGrass,
	SetObjectOnsen,
	SetObjectAwa,
};

//------------------------------------------
//	���씭��OBJ
//------------------------------------------
static void EvObjMoveSetObject( EventOBJ *ev, actWork *act, u32 set )
{
	u8 i;

	for( i = 0; i < SETOBJ_MAX; i++, set >>= 1 )
	{
		if( (set & 0x01) )
		{
			SetObjectTbl[i]( ev, act );
		}
	}
}

//------------------------------------------
//	�ެ��ߊJ�n������
//------------------------------------------
static void EvObjMoveJumpStartClsObject( EventOBJ *ev, u32 *set )
{
	if( ev->move_jump_start_sw )
	{
		ev->attr_grass_sw = 0;
		ev->attr_sand_sw = 0;
		ev->attr_shoal_sw = 0;
		ev->attr_onsen_sw = 0;

		*set &= 0xffffffff ^
				( SETOBJ_SmallGrass |
				SETOBJ_ShortGrass |
				SETOBJ_DeepSand |
				SETOBJ_Shoal |
				SETOBJ_Onsen );
			
	}
}

//------------------------------------------
//	�ެ��ߏI��������
//------------------------------------------
static void EvObjMoveJumpEndClsObject( EventOBJ *ev, u32 *set )
{
	if( ev->move_jump_end_sw )
	{
		*set &= 0xffffffff ^ SETOBJ_Pool;
	}
}

//======================================================================
//		����J�n,�I��
//======================================================================
//------------------------------------------
//	����J�n�Ŕ���	1st
//------------------------------------------
static void EvObjMvStartFirst( EventOBJ *ev, actWork *act )
{
	u32 set;

	if( ev->move_start_sw )
	{
		set = 0;

		EvObjHeightSet( ev, act );
		EvObjMoveFirstMapAttr( ev, &set );
		EvObjMapAttrPri( ev, act );
		EvObjMoveSetObject( ev, act, set );

		ev->move_start_sw = 0;
		ev->move_jump_start_sw = 0;
	}
}

//------------------------------------------
//	����J�n�Ŕ���	2nd
//------------------------------------------
static void EvObjMvStartSecond( EventOBJ *ev, actWork *act )
{
	u32 set;
	
	if( ev->move_start_sw )
	{
		set = 0;

		EvObjHeightSet( ev, act );
		EvObjMoveStartMapAttr( ev, &set );
		EvObjMapAttrPri( ev, act );
		EvObjMoveJumpStartClsObject( ev, &set );
		EvObjMoveSetObject( ev, act, set );

		ev->move_start_sw = 0;
		ev->move_jump_start_sw = 0;
	}
}

//------------------------------------------
//	����I���Ŕ���
//------------------------------------------
static void EvObjMvEnd( EventOBJ *ev, actWork *act )
{
	u32 set;

	if( ev->move_end_sw )
	{
		set = 0;

		EvObjHeightSet( ev, act );
		EvObjMoveEndMapAttr( ev, &set );
		EvObjMapAttrPri( ev, act );
		EvObjMoveJumpEndClsObject( ev, &set );
		EvObjMoveSetObject( ev, act, set );

		ev->move_end_sw = 0;
		ev->move_jump_end_sw = 0;
	}
}

//======================================================================
//		�߰��
//======================================================================
//------------------------------------------
//	�w�� OBJ �̓����~
//	In:		ev		; ��~������ OBJ
//	Out:	u8		; On = ��~�s��
//------------------------------------------
u8 EvObjPause( EventOBJ *ev )
{
	if( ev->acmd_sw || ev->pause_sw )
	{
		return( 1 );
	}

	ev->pause_sw = 1;
	ev->anm_pause_save_sw = ActWork[ev->actno].anm_pause;
	ev->aff_pause_save_sw = ActWork[ev->actno].aff_pause;
	ActWork[ev->actno].anm_pause = 1;
	ActWork[ev->actno].aff_pause = 1;

	return( 0 );
}

//------------------------------------------
//	�����~ �SOBJ	���� ��l���ȊO
//------------------------------------------
void EvObjPauseAll( void )
{
	u8 i;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( EvObj[i].sw && i != MyMoveState.evobjno )
		{
			EvObjPause( &EvObj[i] );
		}
	}
}

//------------------------------------------
//	�����~ ����OBJ�ȊO
//	In:		no			; ����OBJ,ܰ�����
//------------------------------------------
void EvObjPauseAlmost( u8 no )
{
	u8 i;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( i != no && EvObj[i].sw && i != MyMoveState.evobjno )
		{
			EvObjPause( &EvObj[i] );
		}
	}
}

//------------------------------------------
//	�w�� OBJ �̒�~����
//	In:		ev		; ���� OBJ
//------------------------------------------
void EvObjPauseClr( EventOBJ *ev )
{
	if( ev->sw && ev->pause_sw )
	{
		ev->pause_sw = 0;
		ActWork[ev->actno].anm_pause = ev->anm_pause_save_sw;
		ActWork[ev->actno].aff_pause = ev->aff_pause_save_sw;
	}
}

//------------------------------------------
//	��~���� �SOBJ
//------------------------------------------
void EvObjPauseClrAll( void )
{
	u8 i;
	
	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( EvObj[i].sw )
		{
			EvObjPauseClr( &EvObj[i] );
		}
	}
}

//======================================================================
//		�������쐧��
//======================================================================
//------------------------------------------
//	1 dot �ړ�
//------------------------------------------
void EvActMove1( actWork *act, u8 site )
{
	act->x += SitePosMoveTbl[site].x;
	act->y += SitePosMoveTbl[site].y;
}

//------------------------------------------
//	2 dot �ړ�
//------------------------------------------
void EvActMove2( actWork *act, u8 site )
{
	act->x += SitePosMoveTbl[site].x << 1;
	act->y += SitePosMoveTbl[site].y << 1;
}

//------------------------------------------
//	3 dot �ړ�
//------------------------------------------
void EvActMove3( actWork *act, u8 site )
{
	act->x += (SitePosMoveTbl[site].x << 1) + SitePosMoveTbl[site].x;
	act->y += (SitePosMoveTbl[site].y << 1) + SitePosMoveTbl[site].y;
}

//------------------------------------------
//	4 dot �ړ�
//------------------------------------------
void EvActMove4( actWork *act, u8 site )
{
	act->x += SitePosMoveTbl[site].x << 2;
	act->y += SitePosMoveTbl[site].y << 2;
}

//------------------------------------------
//	8 dot �ړ�
//------------------------------------------
void EvActMove8( actWork *act, u8 site )
{
	act->x += SitePosMoveTbl[site].x << 3;
	act->y += SitePosMoveTbl[site].y << 3;
}

//------------------------------------------
//	1dot 1�وړ�ð���
//------------------------------------------
static void (* const EvActMove1CellTbl[])( actWork *, u8 ) =
{
	EvActMove1,EvActMove1,EvActMove1,EvActMove1,
	EvActMove1,EvActMove1,EvActMove1,EvActMove1,
	EvActMove1,EvActMove1,EvActMove1,EvActMove1,
	EvActMove1,EvActMove1,EvActMove1,EvActMove1,
};

//------------------------------------------
//	2dot 1�وړ�ð���
//------------------------------------------
static void (* const EvActMove2CellTbl[])( actWork *, u8 ) =
{
	EvActMove2,EvActMove2,
	EvActMove2,EvActMove2,
	EvActMove2,EvActMove2,
	EvActMove2,EvActMove2,
};

//------------------------------------------
//	3dot 1�وړ�ð���
//------------------------------------------
static void (* const EvActMove3CellTbl[])( actWork *, u8 ) =
{
	EvActMove2,EvActMove3,EvActMove3,
	EvActMove2,EvActMove3,EvActMove3,
};

//------------------------------------------
//	4dot 1�وړ�ð���
//------------------------------------------
static void (* const EvActMove4CellTbl[])( actWork *, u8 ) =
{
	EvActMove4,EvActMove4,
	EvActMove4,EvActMove4,
};

//------------------------------------------
//	8dot 1�وړ�ð���
//------------------------------------------
static void (* const EvActMove8CellTbl[])( actWork *, u8 ) =
{
	EvActMove8,
	EvActMove8,
};

//------------------------------------------
//	�وړ�ð���
//------------------------------------------
static void (* const * const EvActMoveCellTbl[])( actWork *, u8 ) =
{
	EvActMove1CellTbl,
	EvActMove2CellTbl,
	EvActMove3CellTbl,
	EvActMove4CellTbl,
	EvActMove8CellTbl,
};

//------------------------------------------
//	1�وړ�����
//------------------------------------------
static const s16 EvActMoveCellCountTbl[] =
{
	16,
	8,
	6,
	4,
	2,
};

//------------------------------------------
//	���s�ړ��n
//	work3 = ����
//	work4 = ����
//	work5 = ܰ�
//------------------------------------------
static void ev_act_move_walk_init( actWork *act, u8 site, u8 type )
{
	act->work[3] = site;
	act->work[4] = type;
	act->work[5] = 0;
}

static u8 ev_act_move_walk( actWork *act )
{
	if( act->work[5] < EvActMoveCellCountTbl[act->work[4]] )
	{
		EvActMoveCellTbl[act->work[4]][act->work[5]]( act, act->work[3] );

		act->work[5]++;
		
		if( act->work[5] >= EvActMoveCellCountTbl[act->work[4]] )
		{
			return( 1 );
		}
	}
	
	return( 0 );
}

//----
#if 0
//----
//------------------------------------------
//	���s�ړ��n
//	work3 = ����
//	work4 = ����
//	work5 = ܰ�
//------------------------------------------
static void ev_act_move_walk_init( actWork *act, u8 site, u8 type )
{
	u8 count[] = { 16, 8, 4, 2 };

	act->work[3] = site;
	act->work[4] = type;
	act->work[5] = count[type];
}

static u8 ev_act_move_walk( actWork *act )
{
	void (*sub[])( actWork *, u8 ) =
		{ EvActMove1, EvActMove2, EvActMove4, EvActMove8 };
		
	if( act->work[5] )
	{
		sub[act->work[4]]( act, act->work[3] );
		act->work[5]--;

		if( act->work[5] == 0 )
		{
			return( 1 );
		}
	}
	
	return( 0 );
}
//----
#endif
//----

//------------------------------------------
//	���s�ړ��n0.5
//	work3 = ����
//	work4 = ܰ�
//------------------------------------------
static void ev_act_move_walk_05_init( actWork *act, u8 site )
{
	act->work[3] = site;
	act->work[4] = 0;	
	act->work[5] = 0;
}

static u8 ev_act_move_walk_05( actWork *act )
{
	if( (act->work[4]&0x01) == 0 )
	{
		EvActMove1( act, act->work[3] );
		act->work[5]++;
	}

	act->work[4]++;

	if( act->work[5] >= 16 )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	�ެ��ߵ̾��ð���
//------------------------------------------
static const s8 EvActJumpOffsTblType0[16] =				// 12
{
	 -4,  -6, -8, -10, -11, -12, -12, -12,
	-11, -10, -9,  -8,  -6,  -4,   0,   0
};

static const s8 EvActJumpOffsTblType1[16] =				// 6
{
	  0, -2, -3, -4, -5, -6, -6, -6,
	 -5, -5, -4, -3, -2,  0,  0,  0,
};

static const s8 EvActJumpOffsTblType2[16] =				// 8
{
#if 0
	 -2, -4, -5, -6, -7, -8, -8, -8,
	 -7, -7, -6, -5, -3, -2,  0,  0,
#else
	 -2, -4, -6, -8, -9, -10, -10, -10,
	 -9, -8, -6, -5, -3, -2,  0,  0,
#endif
};

static const s8 * const EvActJumpOffsTbl[] =
{
	EvActJumpOffsTblType0,								// EVACT_MV_JUMP_V_12
	EvActJumpOffsTblType1,								// EVACT_MV_JUMP_V_6
	EvActJumpOffsTblType2,								// EVACT_MV_JUMP_V_8
};

//------------------------------------------
//	�ެ��ߵ̾�Ď擾
//------------------------------------------
s16 EvActMoveJumpVoffs( s16 count, u8 type )
{
	return( (s16)EvActJumpOffsTbl[type][count] );
}

//------------------------------------------
//	�ެ��߈ړ�
//	work3 = ����
//	work4 = ������
//	work5 = �ެ�������
//	work6 = ����
//------------------------------------------
static void ev_act_move_jump_init( actWork *act, u8 site, u8 type, u8 v_type )
{
	act->work[3] = site;
	act->work[4] = type;
	act->work[5] = v_type;
	act->work[6] = 0;
}

static u8 ev_act_move_jump( actWork *act )
{
	s16	count[] = { 16,16,32 };							// 0,1,2��
	u8 shift[] = { 0,0,1 };
	u8 ret;

	ret = 0;

	if( act->work[4] != EVACT_MV_STAY_JUMP )			// ���̏�ެ��߈ȊO
	{
		EvActMove1( act, act->work[3] );
	}

	act->dy = EvActMoveJumpVoffs( act->work[6] >> shift[act->work[4]], act->work[5] );

	act->work[6]++;

	if( act->work[6] == (count[act->work[4]] >> 1) )
	{
		ret = 1;										// ʰ�
	}

	if( act->work[6] >= count[act->work[4]] )
	{
		act->dy = 0;
		ret = 0xff;
	}

	return( ret );
}

static u8 ev_act_move_jump_05( actWork *act )
{
	s16	count[] = { 32,32,64 };							// 0,1,2��
	u8 shift[] = { 1,1,2 };
	u8 ret;

	ret = 0;

	if( act->work[4] != EVACT_MV_STAY_JUMP )			// ���̏�ެ��߈ȊO
	{
		if( (act->work[6] & 0x01) == 0 )
		{
			EvActMove1( act, act->work[3] );
		}
	}

	act->dy = EvActMoveJumpVoffs( act->work[6] >> shift[act->work[4]], act->work[5] );

	act->work[6]++;

	if( act->work[6] == (count[act->work[4]] >> 1) )
	{
		ret = 1;										// ʰ�
	}

	if( act->work[6] >= count[act->work[4]] )
	{
		act->dy = 0;
		ret = 0xff;
	}

	return( ret );
}

//------------------------------------------
//	����
//	work3 = ����
//------------------------------------------
static void ev_act_move_wait_init( actWork *act, s16 count )
{
	act->work[3] = count;
}

static u8 ev_act_move_wait( actWork *act )
{
	act->work[3]--;
	
	if( act->work[3] == 0 )
	{
		return( 1 );
	}
		
	return( 0 );
}

//------------------------------------------
//	�ٱ�ҳ���
//------------------------------------------
static void ev_act_move_anm_wait_init( actWork *act, u8 anm_no, u8 anm_offs )
{
	act->anm_no = anm_no;
	act->anm_pause = 0;
	ActAnmOffsChg( act, anm_offs );
}

static u8 ev_act_move_anm_wait( actWork *act )
{
	if( act->anmend_sw )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	��Ư��
//------------------------------------------
void EvActScrollVanish( actWork *act, u8 def )
{
	s16	x0,y0,x1,y1;

	act->banish = def;

	if( act->scr )
	{
		x0 = act->x + act->dx + act->cx + ActOX;
		y0 = act->y + act->dy + act->cy + ActOY;
	}
	else
	{
		x0 = act->x + act->dx + act->cx;
		y0 = act->y + act->dy + act->cy;
	}
	
	x1 = (s16)((u16)x0 - (u16)(act->cx >> 1));
	y1 = (s16)((u16)y0 - (u16)(act->cy >> 1));
	
	if( x0 >= 256 || x1 < -16 )
	{
		act->banish = 1;
	}

	if( y0 >= 176 || y1 < -16 )
	{
		act->banish = 1;
	}
}

//------------------------------------------
//	̨����OBJ��а�������ʓ���
//------------------------------------------
void EvObjDmyActMove( actWork *act )
{
	EvActPriControl( act->work[1], act, 1 );
	EvActScrollVanish( act, 0 );
}

//------------------------------------------
//	��а���������ύX
//	In:		id			; ����ID
//			site		; ����
//------------------------------------------
void EvObjDmyActSiteChg( u8 id, u8 site )
{
	u8 i;
	actWork *act;

	for( i = 0; i < ACT_MAX; i++ )
	{
		act = &ActWork[i];

		if( act->act_sw && act->move == EvObjDmyActMove && (u8)act->work[0] == id )
		{
			ActAnmChg( act, EvActAnmNoStop(site) );
			break;
		}
	}
}

//======================================================================
//		�̪�ı������
//======================================================================
//------------------------------------------
//	ID ��� & �̪��
//------------------------------------------
static u32 EvEffWorkSetId( u8 code, EventOBJ *ev )
{
	EvObjIdWorkSet( ev, &FeSetWork[0], &FeSetWork[1], &FeSetWork[2] );
	return( FldEffSet(code) );
}

//------------------------------------------
//	�e��������
//------------------------------------------
static void EvEffShadeMake( EventOBJ *ev )
{
	if( ev->shade_act_sw == 0 )
	{
		ev->shade_act_sw = 1;
		EvEffWorkSetId( FECODE_Shade, ev );
	}
}

//------------------------------------------
//	���g�䐶��
//------------------------------------------
static void EvEffMizutamaMake( EventOBJ *ev, actWork *act )
{
	SXYObjHead *objst;
	
	objst = SxyObjHeadGet( ev->obj_code );

	FeSetWork[0] = act->x;
	FeSetWork[1] = act->y + (objst->y_size >> 1) - 2;
	FeSetWork[2] = EVOBJ_LOW_ACTPRI3;
	FeSetWork[3] = BGPRI3;
	
	FldEffSet( FECODE_Ripple );
}

