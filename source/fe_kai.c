//=========================================================================================
//								�t�B�[���h�G�t�F�N�g�E���́i�����j
//									2002/02/20 by nakahiro
//=========================================================================================
#include "common.h"
#include "map_attr.h"
#include "fieldmap.h"
#include "mapbg.h"
#include "task.h"
#include "jiki.h"
#include "fld_main.h"
#include "fntequ.h"
#include "mus_tool.h"
#include "script.h"
#include "ev_check.h"
#include "ev_flag.h"
#include "MaDefine.h"
#include "fe_extra.h"
#include "poketool.h"
#include "pm_str.h"
#include "f_pmlist.h"
#include "tenji.h"
#include "pokelist.h"


//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void StartExtraInit(void);	// �t�B�[���h�G�t�F�N�g�����ݒ�
extern void (*ExtraInit)(void);

static void SetFeKairiki(void);		// ���̓G�t�F�N�g�Z�b�g
static void SetFeKairiki2(void);	// ���̓G�t�F�N�g�Z�b�g�i�_���p�j
static void SetKairikiAnm(void);	// ���͔����A�j���Z�b�g

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
extern const VM_CODE pokelist_kairiki_start[];

extern u8	TargetObjNo;

//=========================================================================================
//									���͏����Ăяo��
//=========================================================================================
//--------------------------------
//		�f�o�b�O���j���[����
//--------------------------------
#ifdef	PM_DEBUG

void TestKairiki(void)
{
	//�_���̓��A�`�F�b�N
	if( TenjiKairikiChk() )
	{
		FPokeNo = 0;
		AnswerWork = 0;
		SetFeKairiki2();
		return;
	}

	if( CheckFrontOBJCode( PUSHROCK1 ) == TRUE ){
		FPokeNo = 0;
		AnswerWork = 0;
		SetFeKairiki();
		return;
	}

	ResetForceEvent();
}

#endif	PM_DEBUG

//----------------------------------------------------
//		�|�P�������X�g����g�p����ꍇ�̃`�F�b�N
//----------------------------------------------------
u8	KairikiCheck(void)
{
	//�_���̓��A�`�F�b�N
	if( TenjiKairikiChk() )
	{
		AnswerWork = FPokeNo;
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeKairiki2;
		return	TRUE;
	}

	if( CheckFrontOBJCode( PUSHROCK1 ) == TRUE ){
		AnswerWork = FPokeNo;
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeKairiki;
		return	TRUE;
	}

	return	FALSE;
}

//--------------------------------------
//		���̓G�t�F�N�g�Z�b�g
//--------------------------------------
static void SetFeKairiki(void)
{
	FeSetWork[0] = FPokeNo;
	StartFieldScript( pokelist_kairiki_start );
}

// �_���p
static void SetFeKairiki2(void)
{
	FeSetWork[0] = FPokeNo;
	FldEffSet( FECODE_KairikiSet );
}

u32 FeKairikiSet(void)
{
	u8	id;

	id = EffectTaskSet();

	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H(	SetKairikiAnm );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L(	SetKairikiAnm );

	PokeNickNameSet( &PokeParaMine[FeSetWork[0]], StrTempBuffer0 );

	return	0;
}

//------------------------------
//		���͔����A�j���Z�b�g
//------------------------------
static void SetKairikiAnm(void)
{
	//�_���̓��A�`�F�b�N
	if( TenjiKairikiChk() ){
		TenjiKairikiSet();
	}else{
		FeCodeDel( FECODE_KairikiSet );
		ContinueFieldScript();
	}
}

