//=========================================================================================
//								�t�B�[���h�G�t�F�N�g�E�e���|�[�g
//									2002/08/24 by nakahiro
//=========================================================================================
#include "common.h"
#include "map_attr.h"
#include "fieldmap.h"
#include "mapbg.h"
#include "task.h"
#include "jiki.h"
#include "fld_main.h"
#include "fntequ.h"
#include "script.h"
#include "ev_check.h"
#include "ev_flag.h"
#include "MaDefine.h"
#include "fe_extra.h"
#include "initgame.h"
#include "f_pmlist.h"
#include "sysflag.h"
#include "fld_task.h"
#include "fld_tool.h"
#include "mapparam.h"


//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void StartExtraInit(void);	//�t�B�[���h�G�t�F�N�g�����ݒ�
extern void (*ExtraInit)(void);

static void SetFeTeleport(void);
static void SetTeleportEff(void);

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================

//-------------------------------------------
//	�`�F�b�N
//-------------------------------------------
u8 TeleportCheck(void)
{
	if( MTYPE_IsEnableFlyCheck( NowMap.type ) == TRUE ){
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeTeleport;
		return	TRUE;
	}

	return	FALSE;
}

//-------------------------------------------
//	�G�t�F�N�g�Z�b�g
//-------------------------------------------
static void SetFeTeleport(void)
{
	InitEventFlagAtTeleport();		//�e���|�[�g�ɂ��t���O������
	FldEffSet( FECODE_Teleport );
	FeSetWork[0] = FPokeNo;
}

u32 FeTeleport(void)
{
	u8	id;

	id = EffectTaskSet();

	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H(	SetTeleportEff );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L(	SetTeleportEff );

//	InitHeroLastStatus();
	SetHeroMoveRequest( MY_MV_BIT_NORMAL );

	return	0;
}

//-------------------------------------------
//	�G�t�F�N�g����
//-------------------------------------------
//----
#if 1
//----
static void SetTeleportEff( void )
{
	FeCodeDel( FECODE_Teleport );
	SetMapChangeTeleport();
}
//----
#else	// ��
//----
static void SetTeleportEff(void)		// ��
{
	FeCodeDel( FECODE_Teleport );
	NextMapSetByTelMapID();
	AddMapChangeWarpTask();
}
//----
#endif
//----
