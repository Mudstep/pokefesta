//=========================================================================================
//								�t�B�[���h�G�t�F�N�g�E�����@��
//									2002/02/25 by nakahiro
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
#include "tenji.h"


//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void StartExtraInit(void);	//�t�B�[���h�G�t�F�N�g�����ݒ�
extern void (*ExtraInit)(void);
extern u8 EscapeDungionCheck(void);
extern void EscapeDungionMain(u8);

static void SetFeAnawohoru(void);
static void SetAnawohoruEff(void);

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================

//---------------------
//		�`�F�b�N
//---------------------
u8	AnawohoruCheck(void)
{
	if( EscapeDungionCheck() == TRUE ){
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeAnawohoru;
		return	TRUE;
	}

	return	FALSE;
}

//-----------------------------------
//		�����@��G�t�F�N�g�Z�b�g
//-----------------------------------
static void SetFeAnawohoru(void)
{
	InitEventFlagAtEscape();		//���Ȃ��ق�ɂ��t���O������
	FldEffSet( FECODE_Anawohoru );
	FeSetWork[0] = FPokeNo;
}

u32 FeAnawohoru(void)
{
	u8	id;

	id = EffectTaskSet();

	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H(	SetAnawohoruEff );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L(	SetAnawohoruEff );

	if( !( TenjiAnawohoruChk() ) )
		SetHeroMoveRequest( MY_MV_BIT_NORMAL );

	return	0;
}

//---------------------------
//		�G�t�F�N�g����
//---------------------------
static void SetAnawohoruEff(void)		// ��
{
	u8	id;

	FeCodeDel( FECODE_Anawohoru );

	//�_���̓��A��������
	if( TenjiAnawohoruChk() )
	{
		TenjiAnawohoruSet();
	}else
	{
		id = AddTask( EscapeDungionMain, 8 );
		TaskTable[id].work[0] = 0;
	}

//	ResetForceEvent();
}

