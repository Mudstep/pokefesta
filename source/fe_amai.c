//=========================================================================================
//								�t�B�[���h�G�t�F�N�g�E�Â�����
//									2002/06/05 by nakahiro
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
#include "palanm.h"
#include "f_pmlist.h"
#include "mus_tool.h"


//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void StartExtraInit(void);	//�t�B�[���h�G�t�F�N�g�����ݒ�
extern void (*ExtraInit)(void);
extern u8	AmaikaoriEncount(void);	// �G���J�E���g�`�F�b�N

static void SetFeAmaikaori(void);	// �Â�����G�t�F�N�g�Z�b�g
static void SetAmaikaoriAnm(void);	// �Â�����G�t�F�N�g
static void AmaikaoriAnm(u8);
static void AmaikaoriEnd(u8);


//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
extern const VM_CODE amaikaori_not_encount[];


//=========================================================================================
//									�Â����菈���Ăяo��
//=========================================================================================
//----------------------------------------------------
//		�|�P�������X�g����g�p����ꍇ�̃`�F�b�N
//----------------------------------------------------
u8	AmaikaoriCheck(void)
{
	pFieldRecoverFunc = StartExtraInit;
	ExtraInit = SetFeAmaikaori;
	return	TRUE;
}

//-------------------------------------
//		�Â�����G�t�F�N�g�Z�b�g
//-------------------------------------
static void SetFeAmaikaori(void)
{
	FldEffSet( FECODE_Amaikaori );
	FeSetWork[0] = FPokeNo;
}

u32 FeAmaikaori(void)
{
	u8	id;

	id = EffectTaskSet();

	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H(	SetAmaikaoriAnm );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L(	SetAmaikaoriAnm );

	return	0;
}

//------------------------------
//		�Â�����G�t�F�N�g
//------------------------------
static void SetAmaikaoriAnm(void)
{
	u8	id;

	SePlay( SE_W230 );
	PaletteFadeReq(
		0xffffffff ^ ( 1 << ( ActWork[ GetHeroActorNumber() ].oamData.Pltt+16 ) ),
		4, 0, 8, 0x1f );
	id = AddTask( AmaikaoriAnm, 0 );
	TaskTable[id].work[0] = 0;
	FeCodeDel( FECODE_Amaikaori );
}

static void AmaikaoriAnm( u8 id )
{
	if( !(FadeData.fade_sw) ){
		if( TaskTable[id].work[0] == 64 ){
			TaskTable[id].work[0] = 0;
			if( AmaikaoriEncount() == TRUE )	DelTask(id);
			else{
				TaskTable[id].TaskAdrs = AmaikaoriEnd;
				PaletteFadeReq(
					0xffffffff ^ ( 1 << ( ActWork[ GetHeroActorNumber() ].oamData.Pltt+16 ) ),
					4, 8, 0, 0x1f );
			}
		}else	TaskTable[id].work[0]++;
	}
}

static void AmaikaoriEnd( u8 id )
{
	if( !(FadeData.fade_sw) ){
		StartFieldScript( amaikaori_not_encount );	// �X�N���v�g�N��(�^�X�N�Ő���)
		DelTask(id);
	}
}
