//=========================================================================================
//								�t�B�[���h�G�t�F�N�g�E��ӂ�
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
#include "script.h"
#include "ev_check.h"
#include "ev_flag.h"
#include "MaDefine.h"
#include "fe_extra.h"
#include "mus_tool.h"
#include "f_pmlist.h"
#include "record.h"


//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void StartExtraInit(void);	//�t�B�[���h�G�t�F�N�g�����ݒ�
extern void (*ExtraInit)(void);

static void SetFeIwakudaki(void);	// ���킭�����G�t�F�N�g�Z�b�g
static void SetIwakudakiAnm(void);	// ������A�j���Z�b�g

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
extern const VM_CODE pokelist_iwakudaki_sutart[];

extern u8	TargetObjNo;


//=========================================================================================
//									���킭���������Ăяo��
//=========================================================================================
//------------------------------------------------------
//		�f�o�b�O���j���[����g�p����ꍇ�̃`�F�b�N
//------------------------------------------------------
#ifdef	PM_DEBUG

void TestIwakudaki(void)
{
	if( CheckFrontOBJCode( CRUSHROCK1 ) == TRUE ){
		FPokeNo = 0;
		SetFeIwakudaki();
		return;
	}

	ResetForceEvent();
}

#endif	PM_DEBUG

//----------------------------------------------------
//		�|�P�������X�g����g�p����ꍇ�̃`�F�b�N
//----------------------------------------------------
u8	IwakudakiCheck(void)
{
	if( CheckFrontOBJCode( CRUSHROCK1 ) == TRUE ){
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeIwakudaki;
		return	TRUE;
	}

	return	FALSE;
}

//--------------------------------------
//		���킭�����G�t�F�N�g�Z�b�g
//--------------------------------------
static void SetFeIwakudaki(void)
{
	FeSetWork[0] = FPokeNo;
	StartFieldScript( pokelist_iwakudaki_sutart );
}

u32 FeIwakudaki(void)
{
	u8	id;

	id = EffectTaskSet();

	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H(	SetIwakudakiAnm );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L(	SetIwakudakiAnm );

	IncRecord( CNT_HIDEN_IWAKUDAKI );

	return	0;
}

//------------------------------
//		������A�j���Z�b�g
//------------------------------
static void SetIwakudakiAnm(void)
{
	SePlay( SE_W088 );
	FeCodeDel( FECODE_Iwakudaki );
	ContinueFieldScript();
}
