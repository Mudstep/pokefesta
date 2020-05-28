//=========================================================================
//	
//			�|�P�����`�f�a�e�X�g��
//
//=========================================================================

#include "common.h"
#include "multisio.h"
#include "pokesio.h"
#include "cable.h"
#include "print.h"
#include "mus_tool.h"

#include "palanm.h"
#include "actor.h"
#include "task.h"
#include "intr.h"
#include "menu.h"

#include "fld_main.h"
#include "script.h"
#include "weather.h"

#include "fld_tool.h"
#include "ev_comm.h"
#include "script.h"

#include "poketool.h"
#include "sodateya.h"

#include "cubecase.h"
extern void MakeCableInfoWindow(u8 x, u8 y, u8 count);
extern void KoukanMainInit();

extern void MultiSioFieldSyncStart(void);
extern void WazaOshieManiaCall(void);


//static void TestViewCommunicationTask(u8 no);
//static void AddTestViewCommunicationTask(void);
void ChangeToKoukan( void );


extern void PokeSodateSet(void);


//static void AddTestMultiSioTask(void);
//static void TestMultiSioTask(u8 no);


extern 	  u16      RecvData[4];						//�ʐM�ɂ���Ă�����Ă�L�[�f�[�^

extern	u8	pokemon_sio_flag;
extern	u8	send_xydata_flag;
extern	u8	sio_windowclear_flag;
extern	u8	synchro_counter;
extern  u8	SioStartFlag;
extern  u8  exchange_pokemon[2];



extern u8 (*MenuFunc)();
extern const u8 PokeNameData[][6];

//--------------------------------------------------------------------
//
//	�f�o�b�O�p���\���^�X�N
//
//--------------------------------------------------------------------
u8 dmy_string[22];		// ������W�J�p�e���|����
const u8 padtext[][3]={
#if 0
	" A",
	" B",
	"SL",
	"ST",
	"RK",
	"LK",
	"UK",
	"DK",
	"RT",
	"LT",
#endif
	{spc_,A__,EOM_},
	{spc_,B__,EOM_},
	{S__,L__,EOM_},
	{S__,T__,EOM_},
	{R__,K__,EOM_},
	{L__,K__,EOM_},
	{U__,K__,EOM_},
	{D__,K__,EOM_},
	{R__,T__,EOM_},
	{L__,T__,EOM_},
};

static const u8 no_msg[]={N__,D__,EOM_};
void make_key_string()
{
	int i,Id;
    Id = ((SioMultiCnt *)REG_SIOCNT)->ID;			//ID���ꎞ�ϐ���

	dmy_string[0] = EOM_;
	PM_strcat(dmy_string,no_msg);
	for(i=0;i<10;i++){
		if(((RecvData[Id^1]>>i)&0x1)==1){
			PM_strcat(dmy_string,padtext[i]);
		}
	}
}


static void view_ko_comm_task(u8 no)
{
	if ( pokemon_sio_flag == 1 && PokemonNinshouNoCheck() == 1 ) {
		SePlay( SE_BOO );
		MultiSioFieldSyncStart();
		DelTask(no);
	}
}

static void view_oya_comm_task(u8 no )
{
	if ( pokemon_sio_flag == 1 && PokemonNinshouNoCheck() == 1 ) {
		SePlay( SE_PIN );
		MultiSioFieldSyncStart();
		DelTask(no);
	}
}

static void view_oya_wait_task(u8 no)
{
	if ( GetSioReliableNumber() == GetSioRealConnectNumber() ) {
	//	SePlay( SE_WIN_OPEN );
		MultiSioSignalDecision();
		TaskTable[no].TaskAdrs = view_oya_comm_task;
	}
}

static void test_view_task( u8 no )
{
	if ( GetMyConnectIsParent() == 1 ) {
		TaskTable[no].TaskAdrs = view_oya_wait_task;
	} else {
		TaskTable[no].TaskAdrs = view_ko_comm_task;
	}
}




// =================================================================
// DebugTestEndWait
// �T�v  : �f�o�b�O���j���[����I���҂�
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 DebugTestEndWait()
{
	if(sys.Trg&A_BUTTON){
		MenuEnd();
		return 1;
	}
	return 0;
}

static const u8 TestMes[]={
	no_,spc_,ko_,do_,mo_,ha_,CR_,EOM_,
	
};
static const u8 TestEndMes[]={spc_,EOM_,};
// =================================================================
// DebugTestSearchChild
// �T�v  : �Ă����̃|�P�����̎q�����T�[�`���ĕ\��
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 DebugTestSearchChild()
{
	
	u8 strbuf[50];
	u16 w1,w2;
	w1 = PokeParaGet(&PokeParaMine[0],ID_monsno);
	w2 = PokeSearchChild(w1);
	PM_strcpy(strbuf,PokeNameData[w1]);
	PM_strcat(strbuf,TestMes);
	PM_strcat(strbuf,PokeNameData[w2]);
	PM_strcat(strbuf,TestEndMes);
	NMenuScreenClear();
	NMenuWinBoxWrite(0,14,30,19);
	NMenuMsgWrite(strbuf, 1, 15 );
	MenuFunc = DebugTestEndWait;
	return 0;
}


// =================================================================
// DebugTestTamagoMake
// �T�v  : ��ĉ��ɂ���|�P��������^�}�S�����
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 DebugTestTamagoMake()
{
	if(PokeAzukeruCheck(&Fld.SodateyaWork)==2){		//2�̂���
		if(SodateyaPokeAffinity()!=0){				//�q�������܂�Ă����������Ȃ�
			PokeTamagoSet();						//�^�}�S�������Z�b�g
		}
	}
	MenuEnd();
	return 1;

}

extern void PokeNidoranOsuSet();
static u8 DebugTestTamagoMake2()
{
	if(PokeAzukeruCheck(&Fld.SodateyaWork)==2){		//2�̂���
		if(SodateyaPokeAffinity()!=0){				//�q�������܂�Ă����������Ȃ�
			PokeNidoranOsuSet();
		}
	}
	MenuEnd();
	return 1;

}



// =================================================================
// DebugTest1000po
// �T�v  : �P�O�O�O�������邢�����Ƃɂ���
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 DebugTest1000po()
{
	SodateyaPlusWalk(1000);
	MenuEnd();
	return 1;
}

// =================================================================
// DebugTest10000po
// �T�v  : �P�O�O�O�O�������邢�����Ƃɂ���
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 DebugTest10000po()
{
	SodateyaPlusWalk(10000);
	MenuEnd();
	return 1;
}

// =================================================================
// DebugTestOshieMania
// �T�v  : ���U�������}�j�A�ɔ��
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 DebugTestOshieMania()
{
	WazaOshieManiaCall();
	MenuEnd();
	return 1;
}


// =================================================================
// DebugTestOshieMania
// �T�v  : �^�}�S���܂�钼�O�ɂ���
// ����  : none
// �߂�l: u8
// =================================================================
static u8 DebugTestUmare()
{
	u8 i,work;
	for(i=0;i<6;i++){
		if(PokeParaGet(&PokeParaMine[i],ID_tamago_flag)){
			work = 0;
			PokeParaPut(&PokeParaMine[i],ID_friend,&work);
		}
	}
	Fld.SodateyaWork.EggCount = 253;
	MenuEnd();
	return 1;
}

const u8 LongNameTest[]={na_,ga_,i_,na_,ma_,e_,po_,ke_,mo_,n_,EOM_,};

static u8 DebugTestName()
{
	PokeParaPut(&PokeParaMine[0],ID_nickname,LongNameTest);
	MenuEnd();
	return 1;
}

static u8 DebugPorokuDel()
{
	int i;
	s8  work;
	for(i=0;i<40;i++){
			DelCube(i);
	}
	MenuEnd();
	return 1;
}
/*====================================================================*/
/*  ���C��                                                            */
/*====================================================================*/
static const u8 Menu0_Msg[] = { ko_, wo_,  SA_,  bou_,  TI_,  spc_, EOM_ };
static const u8 Menu1_Msg[] = { TA_, MA_,  GO_,  spc_ , spc_, spc_, EOM_ };
static const u8 Menu1b_Msg[] = { O_,  SU_,  TA_,  MA_,   GO_,  spc_, EOM_ };
static const u8 Menu2_Msg[] = { n1_, n0_,  n0_,  n0_,   po_, spc_, EOM_ };
static const u8 Menu3_Msg[] = { n1_, n0_,  n0_,  n0_, n0_, po_, spc_, EOM_ };
static const u8 Menu4_Msg[] = { o_,si_,e_,MA_,NI_,A_,spc_, EOM_ };
static const u8 Menu5_Msg[] = { TA_,MA_,GO_,u_,ma_,re_,EOM_};
static const u8 Menu6_Msg[] = { RO_,N_,GU_,NE_,bou_,MU_,EOM_,};
static const u8 Menu7_Msg[] = { PO_,RO_,TTU_,KU_,ke_,su_,EOM_,};

static const MENU_ITEM DebugMenuTbl[] =
{
	{ Menu0_Msg, DebugTestSearchChild  },
	{ Menu1_Msg, DebugTestTamagoMake      },
	{ Menu1b_Msg,DebugTestTamagoMake2      },
	{ Menu2_Msg, DebugTest1000po       },
	{ Menu3_Msg, DebugTest10000po       },
	{ Menu4_Msg, DebugTestOshieMania       },
	{ Menu5_Msg, DebugTestUmare,},
	{ Menu6_Msg, DebugTestName,},
	{ Menu7_Msg, DebugPorokuDel,},
};

#define DEBUG_MENU_MAX  (sizeof(DebugMenuTbl)/sizeof(DebugMenuTbl[0]))
#define MENU_X  (9)
#define MENU_Y  (DEBUG_MENU_MAX*2+1)

static u8 DebugMoriMenu(void)
{
	s8 num;

	num = NMenuVCursorControl();
	switch(num){
	case CURSOR_DEFAULT_VALUE:
		return 0;

	case CURSOR_CANCEL_VALUE:
		MenuEnd();
		return 1;

	default:
		break;
	}

	MenuFunc = DebugMenuTbl[num].ptr;
	return 0;
}

//--------------------------------------------------------------------
//
//	�f�o�b�O���j���[����̋@�\�Ăяo��
//
//--------------------------------------------------------------------
u8 DebugMoriTest(void)
{
//	AddTestViewCommunicationTask();
	NMenuScreenClear();
	NMenuWinBoxWrite(0, 0, MENU_X, MENU_Y);
	NMenuFixListWrite(2, 1, DEBUG_MENU_MAX, DebugMenuTbl);
	NMenuVCursorInit(1, 1, DEBUG_MENU_MAX, 0);
	MenuFunc = DebugMoriMenu;
	
	return 0;
};

