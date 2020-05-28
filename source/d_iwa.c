//<d_iwa.c>
//////////////////////////////////////////////////
//
//	PMAGB�@�f�o�b�O�p���C�����[�`��
//
/////////////////////////////////////////////////
#ifdef PM_DEBUG

#include "common.h"
#include "actor.h"		//�A�N�^�[�V�X�e���p���C���w�b�_
#include "actanm.h"		//�A�N�^�[�V�X�e���E�A�j���[�V�����w�b�_
#include "palanm.h"		//�p���b�g�A�j���[�V�����p�w�b�_
#include "intr.h"		//���荞�ݐ���p�w�b�_
#include "madefine.h"
#include "task.h"
#include "print.h"
#include "menu.h"
#include "fld_main.h"
#include "fld_task.h"
#include "script.h"

#include "d_ropeway.h"
#include "ev_flag.h"
#include "evobj.h"

typedef struct
{
	u8	uc_main;	//���C��
	u8	uc_next;	//���ɔ�ԃ^�X�N
	u8	uc_ret;		//�߂��
	u8	uc_err;		//�G���[���N�������Ƃ��ɔ��

	u8	uc_flg;		//�^�X�N�Ǘ��p�̃t���O�G���A
}_d_iwa_tsk;

//�f�o�b�O�V�X�e���ŃO���[�o���Ɏg�p����l���p�b�N
typedef struct
{
	u8	uc_flg;				//�ėp�t���O�G���A
	u8	uc_stPalFade;		//�p���b�g�t�F�[�h�A�j���[�V�����̃X�e�[�^�X
	
	_d_iwa_tsk	tsk;

	u16	us_aScreen[4][1024];
}_d_iwa_main;

//CellID
typedef const enum
{
	DIWA_CELL0 = 1000,
	DIWA_CELL1,
	DIWA_CELL2,
	DIWA_CELL3,
	DIWA_CELL4,
	DIWA_CELL5,
}DIwa_CellID;

extern u8	MailViewSet(void);
extern u8	(*MenuFunc)();

extern u8	g_rlt_debug_sw;
extern void RouletteSet(void);	//���[���b�g�Ăяo��
extern const VM_CODE ev_roulette_p01_c103_r0401[];
extern const VM_CODE ev_roulette_p02_c103_r0401[];
u8 RouletteSetUp1(void){
	g_rlt_debug_sw = 1;
	StartFieldScript( ev_roulette_p01_c103_r0401 );
	MenuEnd();
	return 1;
}
u8 RouletteSetUp3(void){
	g_rlt_debug_sw = 1;
	StartFieldScript( ev_roulette_p02_c103_r0401 );
	MenuEnd();
	return 1;
}
////////////////////////////////////////////////////////
//	�O���[�o���V�X�e���f�[�^
////////////////////////////////////////////////////////
static const u8	str_ropeUp[] = {RO_,bou_,PU_,U_,EE_,I_,U__,EOM_};
static const u8 str_ropeDown[] = {RO_,bou_,PU_,U_,EE_,I_,D__,EOM_};
static const u8 str_roulette1[] = {RU_,bou_,RE_,TTU_,TO_,n1_,EOM_};
static const u8 str_roulette3[] = {RU_,bou_,RE_,TTU_,TO_,n3_,EOM_};
static const u8	str_test[] = {ME_,bou_,RU_,V__,I__,E__,U__,EOM_};
static const MENU_ITEM	sg_diwa_menu[] =
{
	{ str_ropeUp,   DemoRopewaySetUp },		//00:���[�v�E�F�C�f��(�̂ڂ�)
	{ str_ropeDown, DemoRopewaySetDown},	//01:���[�v�E�F�C�f��(������)
	{ str_roulette1, RouletteSetUp1 },		//02:���[���b�g
	{ str_roulette3, RouletteSetUp3 },		//03:���[���b�g
	{ str_test,		MailViewSet },			//04:���[���\��
};

//////////////////////////////////////////////////////////
//�A�N�^�[�V�X�e���p�f�[�^
//////////////////////////////////////////////////////////
static const	CellData TestDataCell[];
static const	PalData TestDataPal[];
static const	actHeader TestHeader[];

/*-----------------------------------------
	�f�o�b�O���j���[	�L�[�擾

	static u8 DebugIwasawaKey(void);
-----------------------------------------------*/
static u8 DebugIwasawaKey(void){

	MENUFUNC	func;

	if(sys.Trg == U_KEY)	NMenuVCursorSet(-1);
	if(sys.Trg == D_KEY)	NMenuVCursorSet(1);
	
	if(sys.Trg == A_BUTTON){
		func = sg_diwa_menu[ NMenuVCursorPosGet() ].ptr;
		return	func();
	}
	else if(sys.Trg == (A_BUTTON | R_BUTTON))
	{
		ScriptParameter0 = 1;
		func = sg_diwa_menu[ NMenuVCursorPosGet() ].ptr;
		return	func();
	}

	if(sys.Trg == B_BUTTON){
		MenuEnd();
		return	1;
	}

	return	0;
}

/*--------------------------------------
	�f�o�b�O�V�X�e���˓����[�`��

	u8 DebugIwasawaTest(void);
----------------------------------------*/
u8 DebugIwasawaTest(void)
{
	ScriptParameter0 = 0;
	NMenuScreenClear();								//���j���[�ݒ�
	NMenuWinBoxWrite( 19, 0, 29, 12 );				//�E�B���h�E�\��
	NMenuFixListWrite( 21, 1, 5, sg_diwa_menu );	//���j���[�\��
	NMenuVCursorInit( 20, 1, 5, 0 );				//�J�[�\�������ݒ�

	MenuFunc = DebugIwasawaKey;
	
	return 0;
}

#endif	//ifdef PM_DEBUG
