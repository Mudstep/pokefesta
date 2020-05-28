/*==========================================================================*/
/*	cable.c																	*/
/*																			*/
/*												GameFreak inc.				*/
/*												2001/08/??					*/
/*==========================================================================*/


//�I���̓L�����Z������܂����B

//------------------------------------------------------------------------------------
//  �f�o�b�O�p�̒�`
//------------------------------------------------------------------------------------
#define  CABLE_C						//extern�錾�����̉������Ȃ����߂̐錾
#define  DEBUG_PRINT					//���ޯ�ޏ���\��������錾
#define  DEBUG_NUMPRINT					//���ޯ�ޏ����O������Ăяo���ĕ\��������錾
#define  ERROR_CHECK					//�ʐM�G���[���N�����ۂɑ��G���[��ʂɔ�΂����ǂ���
//#define  USERWORK_TEST

//------------------------------------------------------------------------------
//include
//------------------------------------------------------------------------------
#include "common.h"
#include "evobj.h"


#include "multisio.h"
#include "intr.h"
#include "jiki.h"

#include "print.h"
#include "menu.h"
#include "pokesio.h"
#include "calctool.h"
#include "decord.h"
#include "..\graphic\pokegra.h"
#include "mus_tool.h"
#include "pm_str.h"

#include "task.h"
#include "actor.h"
#include "actanm.h"
#include "Madefine.h"
#include "palanm.h"

#include "seed.h"
#include "cubecase.h"
#include "cube_def.h"
#include "screen.h"
#include "svld_ini.h"
#include "svld_sys.h"
#include "server.h"
#include "fight.h"

#include "ex_load.h"
#include "fld_sio.h"


//----------------------------------------------------------------------------
//  �f�o�b�O�\���p�̐錾
//----------------------------------------------------------------------------
#ifdef DEBUG_NUMPRINT
#include "numfont.acl"
#include "numfont.ach"
u32 debugnum[MULTI_SIO_PLAYERS_MAX];
u32  DebugWork0;
u32  DebugWork1;
u8   TimerPutFlag = 0;
u8   RenzokuExchangeFlag = 0;
u16  RenzokuExchangeWait = 0;
u16  RenzokuExchangeStat = 0;
u16	 checksumwork[4];
u8	 SioEndFlag;
u8 	 SioMemberNumberCheck;
#endif
u32 	BlendMachineCount = 0;

//----------------------------------------------------------------------------
//  UserWork�𗘗p����f�o�b�O��`�̐錾
//----------------------------------------------------------------------------


/*/=========================================================================*/
/*	extern �錾																*/
/*/=========================================================================*/
extern s16  GetPlusLine();
extern s16  GetMinusLine();
extern void SetPlusLine(s16 no);
extern void SetMinusLine(s16 no);
extern void FuncSeed2Cube(u16 *mix, CUBE *newcube, u8 num, u8 *calc);
extern u8   SioStopFlag;
extern u8 	CasetteLanguage;
extern u8 	SoftResetDisable;

extern u8		SaveExtraSector(u8 sector_num, u8 *p);
extern u8		LoadExtraSector(u8 sector_num, u8 *p);

/*/=========================================================================*/
/*	��`																	*/
//==========================================================================*/




//�e�X�g�p�ɑ��M�f�[�^�̃T�C�Y
#define TESTSEND_SIZE (200)


/*==========================================================================*/
/*  ������錾																*/
/*==========================================================================*/
//	#define name value	

//typedef ...


static const u8 setuzoku_text[][12]={
	{se_,tu_,zo_,ku_,spc_,ti_,yyu_,u_,de_,su_,EOM_},		//�u�ڑ����ł��v
	{se_,tu_,zo_,ku_,spc_,de_,ki_,ma_,se_,n_,EOM_},			//�u�ڑ��ł��܂���v
	{ka_,ku_,ni_,n_,spc_,ti_,yyu_,u_,de_,su_,EOM_},			//�u�m�F���ł��v
	{ka_,ku_,ni_,n_,spc_,de_,ki_,ma_,si_,ta_,EOM_},			//�u�m�F�ł��܂����v
	{ka_,ku_,ni_,n_,spc_,de_,ki_,ma_,se_,n_,EOM_},			//�u�m�F�ł��܂���v
	{ka_,ku_,ni_,n_,spc_,wo_,spc_,ka_,ku_,ni_,n_,EOM_},		//�u�m�F���m�F�v
	{ka_,ku_,ni_,n_,spc_,ha_,spc_,si_,ttu_,pa_,i_,EOM_},	//�u�m�F�͎��s�v
};

//�u������G���[���͂��������܂����v
static const u8 err_msg[]={CTRL_,C_BCOL_,COL_NUKE,CTRL_,C_FCOL_,COL_WHITE,tu_,u_,si_,n_,E_,RA_,bou_,ga_,spc_,ha_,ttu_,se_,i_,si_,ma_,si_,ta_,CR_,
							KE_,bou_,BU_,RU_,wo_,spc_,se_,tu_,zo_,ku_,si_,te_,CR_,
							de_,n_,ge_,n_,wo_,spc_,i_,re_,na_,o_,si_,te_,ku_,da_,sa_,i_,EOM_,};

//�G���[�����J���[�w��R�}���h
static const u8 err_num[]={CTRL_,C_BCOL_,COL_NUKE,CTRL_,C_FCOL_,COL_WHITE,EOM_};



//---------------------------------------------------------------------------
//  ��ʃf�[�^����M�Ǘ��p���[�Nstruct��`
//---------------------------------------------------------------------------
typedef struct{
	u16 count;					//���݂̑��M��
	u16 max;					//�ő呗�M��
	u8 *p;						//���M����f�[�^���߲��
	u8  flag;					//�쓮�����ǂ����̃t���O
	u8  Id;						//ID�i���ݖ��g�p�j
	u16 etc;					//�S�o�C�g���E�ɍ��킹�邽�߂̂߂���
}NUMEROUS_DATA;


//---------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//---------------------------------------------------------------------------
static NUMEROUS_DATA NumerousInfo;								//�����̑��M���
static NUMEROUS_DATA OtherNumerousInfo[MULTI_SIO_PLAYERS_MAX];	//���l�̑��M���
static u32	NumerousWait;						//�f�[�^�������M�p�E�F�C�g
static u32 	MultiSioPauseWait;
static u32	MultiSioPauseFlag;					//�Ǘ���K�v�ɂȂ�܂����B
static u32	PokemonSioCheckFlag;
static u32  HozonSioFlags;
static u32  HozonRecvSize;
static u32  HozonSendSize;
static u32	count;


u8 MultiSioMainCheck(void);
u8 SioParentConnectCheck(void);
u8 SioChildConnectCheck(void);
void SplitDataResultSet(u8 id);
void kanshi_network(u8 taskno);
void SetSioEndDataStart();
void SetSioPauseStart();
void HusiginaDekigotoGamenMain();

#ifdef PM_DEBUG
extern void SioCheckFlagClear();
extern void SioCheckFlagOn(u8 id);
#endif
//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;
//-------------------- �O���[�o�� �ϐ�  ----------------------------

    u16  SendBuf[MULTI_SIO_SEND_MAX];   						// ���[�U���M�o�b�t�@
    u16  RecvBuf[MULTI_SIO_RECV_MAX][MULTI_SIO_PLAYERS_MAX];	//�ʐM�f�[�^��M�o�b�t�@

	u16  RecvPoolBuf[MULTI_SIO_PLAYERS_MAX][SPLITDATA_POOL_SIZE/2];	//NumerousSend�p���M�o�b�t�@
	u8   SendPoolBuf[SPLITDATA_POOL_SIZE];							//NumerousSend�p��M�o�b�t�@
	
	u8   Signal;							// �ʐM��Ԓʒm�t���O

	u8   pokemon_sio_flag;				//�|�P�����ʐM�t���O

    u32  SioFlags;                          // �ʐM��ԃt���O
    u32  SioFlagsBak;
	u16  RecvData[MULTI_SIO_PLAYERS_MAX];	//�ʐM�ɂ���Ă�����Ă�L�[�f�[�^(0-3)
	
	u8   multisio_start_flag=OFF;			//�ʐM�����J�n�t���O�i�ʐMOK�ł͂Ȃ������n�߂�t���O�j
	u8   SendXydataFlag;					//�ꏊ�����t���O
	u8   sio_windowclear_flag;				//�ڑ��m�F�E�C���h�E�����t���O
	u16  keydat;
	u8   TransListNo;						//�f�[�^�����R�}���h�Ō������e���w��(TransList[])
	u8   SioReliableNumber;					//�O��̒ʐM�Ŏ擾�����M���ł���ڑ��l��
	u8   SioReliableId;						//�S�J�̒ʐM�ł���Ƃ������M���ł���ڑ��h�c


	u16		PokemonSioNinshouNo;				//�ʐM�J�n�̍ۂɔF�؁i��ɕ������w��j�Ɏg�p
	u16		PokemonSioCheckCount;
	u8		PokemonSioCheckTask;

	SIOTRAINER_INFO SioTrainerInfo[MULTI_SIO_PLAYERS_MAX];		//�ʐM�F�؎��Ɏ擾����g���[�i�[���
	SIOTRAINER_INFO ReliableMemberInfo[MULTI_SIO_PLAYERS_MAX];	//���ڂ̒ʐM�F�؂Ŏ擾�����M���ł��郁���o�[
	u8		MultiSioInitialFlag[MULTI_SIO_PLAYERS_MAX];
	u8		MultiSioEndFlag[MULTI_SIO_PLAYERS_MAX];
	u8		MultiSioSynchroFlag[MULTI_SIO_PLAYERS_MAX];

	SIOTRAINER_INFO MyTrainerInfo;
	SEND_SIO_CODE	SendTrainerInfo;

	u32  	SioRecoverWait;
	u8		TestCasetteLanguage;
	u8		SioErrorPermitFlag;								//�ʐM�G���[��ʂɔ�΂��Ă������t���O
	u8 		SioErrorFlag;									//�ʐM�G���[�t���O
	u8      SplitDataFuncFlag[4];						//�f�[�^�����ʐM�����I���t���O
#ifdef PM_DEBUG
	u16 SioWatchCount=0;
#endif

//--------------------------------------------------------------------------
//  �f�[�^�����i�������M�j�p���X�g�f�[�^�\����
//--------------------------------------------------------------------------
typedef struct{
	const u8  *pointer;
	      u16 size;
}TRANSLIST;

static const u32	TransList[][2]=
{
	{(u32)SendPoolBuf,	200	},
	{(u32)SendPoolBuf,	200	},
	{(u32)SendPoolBuf,	100	},
	{(u32)SendPoolBuf,	220	},	
	{(u32)SendPoolBuf,	40	},

};


//=========================================================================
//  extern�ϐ�
//=========================================================================
extern u8    dmy_string[];
extern const ActOamData Oam32x32;
extern u16   ReturnItemNo;

//---------------------- �T�u���[�`��  -----------------------------


//=========================================================================
//  extern�֐�
//=========================================================================
extern void SetOthersJikiAct( MvCmd *myst, s16 px, s16 py );
extern void make_key_string();
extern void BlendMachineMemberItem(u8 Id, u16 ItemNo);

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

void Cable2Main(void);
void BlendMachineFunction_Set();
u16 CheckSumMake(u16 *p, u16 size);
void kanshi_num_set(u32 w0, u32 w1);
void ErrGamenInit();
void ErrGamenMain();



static void RecvDataFunc(u8	Id);
static void	SetSendBuf(u16 command);
//static void counter_kanshi(u8 id);
static void VInterFunc( void );
static void PokemonIdKoukanOya();
static void SetMultiSioMyData();
static void SioErrCheck();
static void NumerousSendFuncEnd();


//static void TestSendDataFunc();

//--------------------------------
//
//--------------------------------
static void FieldCommandExchangeFunc();
//static void 	get_TrainerInfo(u8 w);

static void NumerousInfoInit();
static u8 SetNumerousData(u8 *pointer, u32 size);
static void NumerousSendFunc();
static void NumerousSendFuncInit();


static void SioEndDataWait();
static void SetSioEndDataSend();
static void SioSynchronizeWait();
static void SioSynchronize();


#ifdef DEBUG_NUMPRINT
static void numput(u8 num, u8 x, u8 y);
void numprint(u32 num, u8 x, u8 y, u8 keta);
#endif


//----------------------------------
//�ʐM�^�X�N���s�p�֐��|�C���^		
//----------------------------------
void (*PokemonMultiSioFunc)();



//-----------------------------------------------------------
//    �A�N�^�[�f�[�^
//-----------------------------------------------------------





//-------------------------------------------------------------------------
//  �ʐM�ڑ��O�ɃP�[�u���̑}����Ԃ���e�q�𔻒f
//-------------------------------------------------------------------------
void CheckParent(u8 id)
{
	u16 flag;

	flag = ((SioMultiCnt *)REG_SIOCNT)->SI;

	DelTask(id);
}


static const u8 name_name[]={TE_,SU_,TO_,na_,EOM_};

//-------------------------------------------------------------------------
//
//  ���ޯ�ޗp������\��ٰ�ݏ�����
//
//-------------------------------------------------------------------------
void DebugNumPrintInit(u8 pal, u8 select, u8 screen, u8 chara)
{
//	PaletteWorkSet(numfont_Palette,pal*32,32);
//	PaletteWorkSet(numfont_Palette,pal*(16)*2,2);
//	PaletteWorkSet(&numfont_Palette[15],pal*(16+15)*2,2);
	PaletteWorkSet(numfont_Palette,pal*(16),32);
	DmaArrayCopy(3 , numfont_Character , 	(BG_VRAM+0x4000*chara),16);
	debugnum[0] = screen;
	debugnum[1] = pal;

	switch(select){
		case 1:
		*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
					| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
					| BG_PRIORITY_1					//BG�D�揇��
					| screen << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
					| chara << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
		break;
		case 2:
		*(vu16 *)REG_BG2CNT = BG_COLOR_16					//16�F�I��
					| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
					| BG_PRIORITY_1					//BG�D�揇��
					| screen << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
					| chara << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
		break;
		case 3:
		*(vu16 *)REG_BG3CNT = BG_COLOR_16					//16�F�I��
					| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
					| BG_PRIORITY_1					//BG�D�揇��
					| screen << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
					| chara << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
		break;
	}
}

void DebugNumPrintInit2(u8 pal, u8 select, u8 screen, u8 chara)
{
//	PaletteWorkSet(numfont_Palette,pal*(16)*2,2);
//	PaletteWorkSet(&numfont_Palette[15],pal*(16+15)*2,2);
	PaletteWorkSet(numfont_Palette,pal*(16),32);
	DmaArrayCopy(3 , numfont_Character , 	(BG_VRAM+0x4000*chara),16);
	debugnum[0] = screen;
	debugnum[1] = pal;
	*(RegBGctrl[select]) = BG_COLOR_16
					| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
					| BG_PRIORITY_0					//BG�D�揇��
					| screen << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
					| chara << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
}

//=========================================================================
//�u��������������v��ʏ�����
//=========================================================================
void Cable2MainInit(void)
{
	int i;

	InitActor();				//�A�N�^�[������
	ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
	InitTask();


	SetVBlankFunc(VInterFunc);

	PrintScreenInit(FIELD_MENU_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);



	NumerousInfoInit();

	PokemonSioNinshouNo = NINSHOU_KOUKAN;
	PokemonMultiSioInit();

	pp_srand(sys.main_timer);
	for(i=0;i<MULTI_SIO_PLAYERS_MAX;i++)
		MyData.ID[i] = pp_rand()%256;

#ifdef DEBUG_NUMPRINT
		DebugNumPrintInit(0,2,4,0);
		
#endif
	

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_BG2_ON					// BG2�L��
						  | DISP_OBJ_ON;

	AddTask(CheckParent,0);




	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
	count = 0;
	
	
	SetMultiSioMyData();
	AddTask(kanshi_network,0);
	MainProcChange(Cable2Main);
};


// =================================================================
// SetMyTrainerInfoStandPos
// �T�v  : �����ʒu�����Z�b�g
// ����  : num
// �߂�l: none
// =================================================================
void SetMyTrainerInfoStandPos(u8 num)
{
	MyTrainerInfo.StandPos = num;

}

//=========================================================================
//  �����p�f�[�^�Z�b�g
//=========================================================================
static void SetMultiSioMyData()
{
	int i;
	MyTrainerInfo.Id = MyData.ID[0]|MyData.ID[1]<<8|MyData.ID[2]<<16|MyData.ID[3]<<24;		//�e�@�p
	for(i=0;i<11;i++)
		MyTrainerInfo.name[i] = MyData.my_name[i];
	MyTrainerInfo.sex       = MyData.my_sex;
	MyTrainerInfo.ninshou   = PokemonSioNinshouNo;
//	MyTrainerInfo.INCode    = CasetteLanguage+TestCasetteLanguage;			//���ۃR�[�h���߂���
	MyTrainerInfo.INCode    = CasetteLanguage;								//���ۃR�[�h���߂���
	MyTrainerInfo.Version   = CasetteVersion+POKEMON_GENERATION_CODE;		//�J�Z�b�g�o�[�W����
	MyTrainerInfo.ExtraCode = ZUKAN_CODE;
	
	//�����ʒu��񂪂��̑O�ɃZ�b�g����Ă��܂��B
}

/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static void VInterFunc( void )
{
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}


void MultiSioStartFunc()
{
	int i;
	for(i=0;i<MULTI_SIO_SEND_MAX;i++) SendBuf[i] = EMPTY_DATA;	//���M�ޯ̧�͖����ް��Ŗ��߂Ă���
	multisio_start_flag=ON;
	Multi_SioInit();


}


//-------------------------------------------------------------------------
//  �ʐM�J�n���h���C�o�ɓ`���邽�߂̃^�X�N
//-------------------------------------------------------------------------
static void MultiSioSignalOn(u8 task)
{
	if(++TaskTable[task].work[0]==WAIT_MULTISIO_START){
		Signal = SIGNAL_DECISION;							//�ʐM�F�؊J�n
		DelTask(task);
	}
}
//-------------------------------------------------------------------------
//  �|�P�����ʐM�J�n�֐�
//-------------------------------------------------------------------------
void PokemonMultiSioInit()
{
	int i;
	Multi_SioGameInit();
	MultiSioStartFunc();
	PokemonMultiSioFunc = PokemonIdKoukanOya;
	SioStopFlag  = 0;
	SioErrorFlag = 0;
	SioErrorPermitFlag = 0;
	SplitDataResultReset();
#ifdef PM_DEBUG
	CpuClear(0,UserWork,16*0x400,16);
	SioWatchCount=0;
	SioCheckFlagClear();
#endif

	MultiSioPauseWait = 0;
	SendXydataFlag    = 0;
	SioEndFlag = 0;

	pokemon_sio_flag=0;
	for(i=0;i<MULTI_SIO_PLAYERS_MAX;i++){
		MultiSioInitialFlag[i] = 1;
		MultiSioEndFlag[i]     = 0;
	}
	AddTask(MultiSioSignalOn,2);
//	Signal = SIGNAL_DECISION;
}

//-------------------------------------------------------------------------
//  �|�P�����ʐM�I���֐�
//-------------------------------------------------------------------------
void PokemonMultiSioEnd()
{
	pokemon_sio_flag = 0;
	multisio_start_flag=OFF;
	Multi_SioStop();
//	for(i=0;i<MULTI_SIO_PLAYERS_MAX;i++)
//		MultiSioEndFlag[i] = 0;
}
static u8 mycount;
static u8 othercount[MULTI_SIO_PLAYERS_MAX];

//-------------------------------------------------------------------------
//  �ʐM�f�[�^�\���E�C���h�E
//-------------------------------------------------------------------------
static void MakeCableInfoWindow(u8 x, u8 y, u16 count)
{
	u8  i, result;
	
	if(mycount!=NumerousInfo.count){
		numprint(NumerousInfo.count, 2,3,2);
		mycount = NumerousInfo.count;
	}

	for(i=0;i<4;i++){
		if(othercount[i]!=OtherNumerousInfo[i].count){
			numprint(OtherNumerousInfo[i].count, 2,4+i,2);
			othercount[i] = OtherNumerousInfo[i].count;
		}
	}
	
	result = SplitDataResult();
	if(result==0xf){
		for(i=0;i<4;i++){
			if((result>>i)&1){
				checksumwork[i] = CheckSumMake(RecvPoolBuf[i],OtherNumerousInfo[i].max);
				SplitDataResultReset2(i);
				if(checksumwork[i]!=0x342){
					TimerPutFlag = 0;
					RenzokuExchangeFlag = 0;
				}
			}
		}
		
	}

};



// =================================================================
// mainpadfunc
// �T�v  : �u�����񂶂�����Q�v�p�p�b�h����
// ����  : none
// �߂�l: none
// =================================================================
static void mainpadfunc()
{
	if(sys.Trg & A_BUTTON){
		Signal = SIGNAL_DECISION;
//		ConnectInfoWindowPut(0, 10, 0);
//		MultiSioStartFunc();
	}
	if(sys.Cont&B_BUTTON){
//		memcpy((u8*)&UserWork[0x4000],ExtraDataStartMugenTicketScript, 
//			(u32)(ExtraDataEndMugenTicketScript-ExtraDataStartMugenTicketScript));
		SetNumerousData((u8*)&UserWork[0x4000],0x2004);
//		TimerPutFlag = 1;
//		RenzokuExchangeFlag=1;
//		SplitDataExchangeStart(0);
	}
	if(sys.Trg&L_BUTTON){
//		SplitDataResultReset();
		PaletteFadeReq(0xffffffff,0,16,0,2);
	}
	if(sys.Trg&START_BUTTON){
		MultiSioErrorPermit(SIO_ERROR_NO_JUMP);

//		for(i=0;i<TESTSEND_SIZE/2;i++)
//		{
//			RecvPoolBuf[0][i] = 0;
//			RecvPoolBuf[1][i] = 0;
//			RecvPoolBuf[2][i] = 0;
//			RecvPoolBuf[3][i] = 0;
//		}
//		Signal = SIGNAL_DECISION;
//		AddTask(ExchangeCampaignFlag,0);
	}
	if(sys.Trg&R_BUTTON){
		PokeAGBSave( POKEAGB_SVFLD );			//�Z�[�u
//		ClearSioTrainerInfo();
	}
	if(sys.Trg&SELECT_BUTTON){
		SetSioEndDataStart();
	}
	if(TimerPutFlag)
//		kanshi_num_set(sys.main_timer,SioMemberNumberCheck);
		kanshi_num_set(sys.main_timer,SioStopFlag|(PokemonMultiSioFunc==NULL)<<4);
}




//-------------------------------------------------------------------------
//�u��������������v��ʃ��C�����[�`��
//-------------------------------------------------------------------------
void Cable2Main(void)
{
	

	mainpadfunc();

	MakeCableInfoWindow( 1, 1,0);

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();

};


/*==================================================================*/
/*�ʐMҲ݊֐�														*/
/*==================================================================*/
u16 multi_sio_func(u16	*key)
{
	u8 	i,Id;

	if(multisio_start_flag==OFF) return 0;
	
//    SioFlagsBak = SioFlags;
    

	for(i=0;i<MULTI_SIO_RECV_MAX;i++) SendBuf[i] = INVALID_DATA;

	keydat = *key;									//�g�����ǂ����킩��Ȃ����ǂƂɂ����L�[�f�[�^��ۑ�
	/*-------�ʐM���ʊĎ�����----------*/
		if(SioFlags&MULTI_SIO_MAIN_SEQ){
		    Id = ((SioMultiCnt *)REG_SIOCNT)->ID;	//ID���ꎞ�ϐ���
			RecvDataFunc(Id);						//��M����
			if(PokemonMultiSioFunc!=NULL){			//�ʐM�p�������
				(*PokemonMultiSioFunc)();
			}
#ifdef ERROR_CHECK
			SioErrCheck();								//�ʐM�G���[�E���������G���[�`�F�b�N
#endif
		}
		
//      SendBuf[0]=keydata;
//        MultiSioSendDataSet((void*)SendBuf);        // �}���`�v���C�ʐM ���M�f�[�^�Z�b�g


	return SioFlags;
}


// =================================================================
// WaitFirstInfoEnd
// �T�v  : �ʐM�S�䂪�����f�[�^�𑗂��Ă�������҂�
// ����  : Id
// �߂�l: none
// =================================================================
static void WaitFirstInfoEnd(u8 Id)
{
	int i,w;

	w=0;
	MultiSioInitialFlag[Id]=0;
	for(i=0;i<GetSioRealConnectNumber();i++){
		w+=MultiSioInitialFlag[i];
	}
	if(w==0){
		if(pokemon_sio_flag==OFF) pokemon_sio_flag = ON;
	}


}
#ifdef USERWORK_TEST
// =================================================================
// PutUserWorkTest
// �T�v  : UserWork�ɒʐM�f�[�^��W�J����e�X�g
// ����  : none
// �߂�l: none
// =================================================================
static void PutUserWorkTest()
{
	u16 i,*p;
	if(SioWatchCount<0x400){	
		p = (u16*)&UserWork[16*SioWatchCount];
		for(i=0;i<4;i++){
			p[i*2]   = RecvBuf[0][i];
			p[i*2+1] = RecvBuf[1][i];
		}
		SioWatchCount++;
	}
}
#endif
/*==================================================================*/
/*      �ʐM�f�[�^�̓��e�ɂ���ď����𕪊򂳂���					*/
/*==================================================================*/
static void RecvDataFunc(u8	Id)
{
	u16 w,i,*p;
	SEND_SIO_CODE *ssc;

#ifdef USERWORK_TEST
	PutUserWorkTest();
#endif
	for(w=0;w<4;w++){
		RecvData[w] = INVALID_DATA;
		//�����M�f�[�^���i�[����Ă����珈�������̉�̓f�[�^����M����Ȃ������Ƃ��Ĕ�΂�
		if(RecvBuf[0][w]==INVALID_DATA)	continue;
			
		switch(RecvBuf[0][w])		//�R�}���h���ɕ���
		{
			case NINSHOU_REQ:									//�|�P�����ʐM�F�؃R�}���h�Ȃ�
				SetMultiSioMyData();							//SIOTRAINER_INFO�̑��M����
				SendTrainerInfo.STInfo = MyTrainerInfo;
				strcpy(SendTrainerInfo.CompanyName1,COMPANY_NAME);
				strcpy(SendTrainerInfo.CompanyName2,COMPANY_NAME);
				SetNumerousData((u8*)&SendTrainerInfo,sizeof(SEND_SIO_CODE));	//SEND_SIO_CODE���M
				break;
			case SEND_KEYDATA:								//���M�f�[�^�̓L�[�f�[�^�̂�
					RecvData[w] = RecvBuf[1][w];
				break;

			case SEND_POSITION_DATA:						//���̃f�[�^�͎q���󂯎��
				SendXydataFlag = ON;
				break;

			case RECV_POSITION_DATA:						//���̃f�[�^�͐e���󂯎��
				SendXydataFlag = ON;
				break;

			case NUMEROUS_SEND:								//��e�ʓ]���R�}���h��M
				OtherNumerousInfo[w].count = 0;			
				OtherNumerousInfo[w].max   = RecvBuf[1][w];	//���M�o�C�g��
				OtherNumerousInfo[w].Id    = RecvBuf[2][w];	//���M�҂�ID+0x80
				break;

			case SPLITDATA_SEND:							//��ʃf�[�^��M
				if(OtherNumerousInfo[w].max>SPLITDATA_POOL_SIZE){				//�ǃf�J�C��M�����i�J�[�h�d�p�j
					p = (u16*)UserWork;
					for(i=0;i<MULTI_SIO_SEND_MAX-1;i++){
						p[OtherNumerousInfo[w].count/2+i] = RecvBuf[1+i][w];	
					}
				}else{
					for(i=0;i<MULTI_SIO_SEND_MAX-1;i++){
						RecvPoolBuf[w][OtherNumerousInfo[w].count/2+i] = RecvBuf[1+i][w];	
					}
				}
				OtherNumerousInfo[w].count+=(MULTI_SIO_SEND_MAX-1)*2;
				if(OtherNumerousInfo[w].count>=OtherNumerousInfo[w].max){
					if(MultiSioInitialFlag[w]==1){
						ssc = (SEND_SIO_CODE*)&RecvPoolBuf[w];
						memcpy((u8*)&SioTrainerInfo[w],(u8*)&(ssc->STInfo),sizeof(SIOTRAINER_INFO));
						if(strcmp(ssc->CompanyName1,COMPANY_NAME) || strcmp(ssc->CompanyName2,COMPANY_NAME)){
							MainProcChange(ErrGamenInit);
						}else{
							WaitFirstInfoEnd(w);
						}
					}else{
						SplitDataResultSet(w);					//�I���t���O�𗧂Ă�
#ifdef PM_DEBUG
						SioCheckFlagOn(w);
#endif
					}
				}
				break;
			case SIO_END_DATA:								//�I�������R�}���h
				MultiSioEndFlag[w] = 1;
				break;
			case SEND_SYNCRO_DATA:
				MultiSioSynchroFlag[w] = 1;					//FIFO�x�����������R�}���h
				break;

			case SEND_ITEM_START:							//�u�����h�}�V�[���p�������R�}���h
					BlendMachineFunction_Set();
				break;

			case SEND_ITEM_DATA:							//�u�����h�}�V�[���p�������R�}���h
				BlendMachineMemberItem(w,RecvBuf[1][w]);
				break;
			case EXCHANGE_DATA:								//�f�[�^���������J�n�R�}���h
				SplitDataSendStart(0,(u8*)TransList[RecvBuf[1][w]][0],(u16)TransList[RecvBuf[1][w]][1]);
				break;

			case SEND_FLDCMD_DATA:							//����R�[�h�����R�}���h
					RecvData[w] = RecvBuf[1][w];
				break;
		}
	}
	


}


/*==================================================================*/
//�R�}���h�𑗐M����
//
//
//  SendBuf[0]:	�f�[�^�̈Ӗ��������R�}���h������
//  SendBuf[1]: [0]�̃R�}���h�ɂ���ĈӖ����ς��
//  SendBuf[2]-[]:  �����������f�[�^������
/*==================================================================*/
extern u8 speed_up_flag;
extern u16 my_command;
static void	SetSendBuf(u16 command)
{
	u8 i;

	switch(command){
		case NINSHOU_REQ:						//�F��ظ���
			SendBuf[0] = command;
			SendBuf[1] = PokemonSioNinshouNo;
			break;
		case SEND_KEYDATA:						//���ް����M
			SendBuf[0] = command;
			SendBuf[1] = sys.Cont;
			break;
		case SEND_POSITION_DATA:				//��l�����W�ް�
			SendBuf[0] = command;
			break;
		case SEND_SYNCHRO_KEYDATA:				//�����ŷ��ް�
			SendBuf[0] = command;
			SendBuf[1] = 0;
			break;
		case TESTDATA_SEND:						//�e�X�g�f�[�^���M
			SendBuf[0] = command;
			for(i=0;i<5;i++)
				SendBuf[1+i] = 0xee;
			break;
		case RECV_POSITION_DATA:				//�����쓯�����̏������W���������
			SendBuf[0] = command;
			break;	
		case NUMEROUS_SEND:						//��e�ʓ]���R�}���h
			SendBuf[0] = command;
			SendBuf[1] = NumerousInfo.max;
			SendBuf[2] = NumerousInfo.Id+0x80;
			break;
		case SEND_ITEM_START:					//�A�C�e�������J�n�R�}���h�i���̂݃u�����_�[�j
			SendBuf[0] = command;
			break;
		case SEND_ITEM_DATA:					//�A�C�e�������R�}���h�i���̂݃u�����_�[�j
			SendBuf[0] = command;
			SendBuf[1] = ReturnItemNo;
			break;
		case EXCHANGE_DATA:						//��ʃf�[�^�����J�n�R�}���h
			SendBuf[0] = command;
			SendBuf[1] = TransListNo;
			break;
		case SIO_END_DATA:							//�ʐM�I���R�}���h
			SendBuf[0] = command;
			break;
		case SEND_SYNCRO_DATA:
			SendBuf[0] = command;
			break;
		case SEND_FLDCMD_DATA:					//����R�[�h�����R�}���h
			if(my_command!=INVALID_DATA &&
					speed_up_flag == FALSE ){
				SendBuf[0] = command;
				SendBuf[1] = my_command;
			}
	}
}

/*==================================================================*/
/*		    �t�B�[���h�ʐM���[�`���ɐ؂�ւ���						*/
/*==================================================================*/
void MultiSioFieldSyncStart()
{
	PokemonMultiSioFunc = FieldCommandExchangeFunc;
}

// =================================================================
// IsMultiSioFieldSyncMode
// �T�v  : �t�B�[���h�������[�h��
// ����  : none
// �߂�l: int	TRUE��FALSE
// =================================================================
int IsMultiSioFieldSyncMode()
{
	if ( PokemonMultiSioFunc == FieldCommandExchangeFunc )
		return TRUE;
	else 
		return FALSE;
}

/*------------------------------------------------------------------*/
/*	�t�B�[���h�v���C���[�̓���R�[�h��������						*/
/*------------------------------------------------------------------*/
static void FieldCommandExchangeFunc()
{
	if ( pokemon_sio_flag == ON ) {
		SetSendBuf( SEND_FLDCMD_DATA ); //����R�[�h�����R�}���h
	}
}


// =================================================================
// BlendMachineFunctionClear
// �T�v  : �ʐM�p�֐��|�C���^���N���A����(���̂݃u�����_�[�p)
// ����  : none
// �߂�l: none
// =================================================================
void BlendMachineFunctionClear()
{
	PokemonMultiSioFunc = NULL;
}

// =================================================================
// MultiSioFunctionClear
// �T�v  : PokemonMultiSioFunc��NULL��������
// ����  : none
// �߂�l: none
// =================================================================
void MultiSioFunctionClear()
{
	PokemonMultiSioFunc = NULL;
}

/*----------------------------------------------------------------------------------*/
/*  ���ݒʐM�ڑ����Ă���l����Ԃ��܂��B											*/
/*																					*/
/*																					*/
/*  pokemon_sio_flag��1�ɂȂ��������m�F���Ă�����s���Ă�������.					*/
/*----------------------------------------------------------------------------------*/
u8 GetConnectSioMenberNum()
{
		return ((SioFlags & MULTI_SIO_PLAYERS_NUM_MASK) >> MULTI_SIO_PLAYERS_NUM_SHIFT);
}

/*----------------------------------------------------------------------------------*/
/*	�ʐM���[�h�m�F����������														*/
/*----------------------------------------------------------------------------------*/
void PokemonMultiSioCheckInit()
{
	PokemonSioCheckFlag  = 0;
	PokemonSioCheckCount = 0;
	PokemonMultiSioInit();
}

/*----------------------------------------------------------------------------------*/
/*	�ʐM���[�h�m�F�����i�Ⴄ��ʂŒʐM���s���Ȃ��悤�ɂ��邽�߂�����)				*/
/*----------------------------------------------------------------------------------*/
u8 PokemonMultiSioCheck()
{
	int i,w;
	w = 0;
	if(pokemon_sio_flag==1){							//�ʐM�m������

		if(GetConnectSioMenberNum()==0){
			SioErrorFlag = 1;
			PokemonMultiSioEnd();
		}
		for(i=0;i<GetConnectSioMenberNum();i++)			//�ʐM���[�h�͈ꏏ���H
			if(SioTrainerInfo[i].ninshou == SioTrainerInfo[0].ninshou) w++;
		if(w==GetConnectSioMenberNum()){
			PokemonSioCheckFlag = 1;						//�ʐM���[�h���ꏏ
		}else{
			PokemonSioCheckFlag = 3;						//�ʐM���[�h���Ⴄ
		}
		
	}else{
		if(++PokemonSioCheckCount>10*60){				//�^�C���A�E�g
			PokemonSioCheckFlag = 2;
		}
	}
	return PokemonSioCheckFlag;
}


/*----------------------------------------------------------------------------------*/
/*  ���̐ڑ����Ă��郁���o�[�ƔF�ؔԍ������H*/
/*----------------------------------------------------------------------------------*/
u8 PokemonNinshouNoCheck()
{
	u8 ret,i,w;

	ret = 0;
	w   = 0;
	for(i=0;i<GetConnectSioMenberNum();i++){				//�ʐM���[�h�͈ꏏ���H
		if(SioTrainerInfo[i].ninshou == SioTrainerInfo[0].ninshou) 
			w++;
	}
	if(w==GetConnectSioMenberNum()){
		ret = 1;
		PokemonSioCheckFlag = 1;						//�ʐM���[�h���ꏏ
	}else{
		ret = 0;
		PokemonSioCheckFlag = 3;						//�ʐM���[�h���Ⴄ
	}

	return ret;
}



/*----------------------------------------------------------------------------------*/
/*  �ʐM�̏��������Ɏ擾��������g���[�i�[ID��Ԃ�									*/
/*----------------------------------------------------------------------------------*/
u32 GetSioTrainerId(u8 id)
{
	return SioTrainerInfo[id].Id;

}


// =================================================================
// ClearSioTrainerInfo
// �T�v  : �ʐM�J�n���̔F�؃f�[�^���N���A
// ����  : none
// �߂�l: none
// =================================================================
void ClearSioTrainerInfo()
{
	int i;
	for(i=0;i<MULTI_SIO_PLAYERS_MAX;i++){
		memset(&SioTrainerInfo[i],0,sizeof(SIOTRAINER_INFO));
	}
}
/*==================================================================*/
/*  ��ʃf�[�^�]������������										*/
/*==================================================================*/
static void NumerousInfoInit()
{
	NumerousInfo.flag  = 0;
	NumerousInfo.count = 0;
	NumerousInfo.max   = 0;
	NumerousInfo.p	   = NULL;
	
}


/*==================================================================*/
/*  ��ʃf�[�^�]�������J�n											*/
/*==================================================================*/
static u8 SetNumerousData(u8 *pointer, u32 size)
{
	if(NumerousInfo.flag) return 0;

	NumerousInfo.Id = GetSioConnectId();
	NumerousInfo.flag  = 1;
	NumerousInfo.max   = size;
	NumerousInfo.count = 0;

	if(size>SPLITDATA_POOL_SIZE){
		NumerousInfo.p 		= pointer;
	}else{
		if(SendPoolBuf!=pointer)
			memcpy(SendPoolBuf,pointer,size);
		NumerousInfo.p     = SendPoolBuf;
	}
	
	SetSendBuf(NUMEROUS_SEND);
	PokemonMultiSioFunc = NumerousSendFuncInit;
	NumerousWait = 0;
	return 1;
}

// =================================================================
// NumerousSendFuncInit
// �T�v  : �f�[�^�������M���[�`��������
// ����  : none
// �߂�l: none
// =================================================================
static void NumerousSendFuncInit()
{
	if(++NumerousWait>2){
		PokemonMultiSioFunc = NumerousSendFunc;
	}
}


/*==================================================================*/
/*	�f�[�^�������M���C��											*/
/*==================================================================*/
static void NumerousSendFunc()
{
	int i;
	u8 *p;
	
	p = NumerousInfo.p;				//�\�L��|�C���^�����Ă�

	SendBuf[0] = SPLITDATA_SEND;

	for(i=0;i<(MULTI_SIO_SEND_MAX-1);i++){
		SendBuf[1+i] = p[NumerousInfo.count+1+i*2]<<8|p[NumerousInfo.count+0+i*2];
//		SendBuf[2] = p[NumerousInfo.count+3]<<8|p[NumerousInfo.count+2];
	}
	NumerousInfo.count+=(MULTI_SIO_SEND_MAX-1)*2;

	if(NumerousInfo.max<=NumerousInfo.count){	//���M�񐔑�������I��
		NumerousInfo.flag  = 0;
		PokemonMultiSioFunc = NumerousSendFuncEnd;
	}
}

// =================================================================
// NumerousSendFuncEnd
// �T�v  : �f�[�^�������M�I��
// ����  : none
// �߂�l: none
// =================================================================
static void NumerousSendFuncEnd()
{
	PokemonMultiSioFunc = NULL;
}

/*------------------------------------------------------------------*/
//�@�ŏ��Ƀf�[�^������������A�L�[�f�[�^�̋��L������
//  
//  ReturnItemNo�ɑI���������˔ԍ��������Ă�i�ł��������X�g�̔ԍ��j;
/*------------------------------------------------------------------*/
static void BlendMachine_Func()
{
	u8 Id;
	
	
	Id = GetSioConnectId();
														//�ʐM�J�n���̈��ڂōs������(�����ǉ���)
//	if((pokemon_sio_flag==ON)&&(SendXydataFlag<5)){	//�T��A���őI���A�C�e���i���o�[�𑗂��Ă܂��B
//		SetSendBuf(SEND_ITEM_DATA);
//		SendXydataFlag++;
//	}else{
			SetSendBuf(SEND_KEYDATA);				//���ް����M
//	}
	BlendMachineCount++;

}
/*------------------------------------------------------------------*/
//  ������ϼ�ݗp�ʐM�֐�
/*------------------------------------------------------------------*/
void BlendMachineFunction_Set()
{
	BlendMachineCount   = 0;
	PokemonMultiSioFunc = BlendMachine_Func;
}


u32 GetBlendMachineCount()
{
	return BlendMachineCount;
}
void BlendMachineStartSend()
{
	SetSendBuf(SEND_ITEM_START);
}

/*------------------------------------------------------------------*/
/*    �ʐM����ID��Ԃ�*/
//
//
//    �ʐM�I�����Ɏ擾�����ID��Ԃ�
/*------------------------------------------------------------------*/
u8 GetSioConnectId()
{
	return ((SioMultiCnt *)REG_SIOCNT)->ID;
}

/*----------------------------------------------------------------------------------*/
/*�����ȊO�̑��M����擾����*/
//
//
//�Ԃ�l�F�����ȊO�̑��M��
/*----------------------------------------------------------------------------------*/
u8 GetAnotherSendTo()
{
	return (0x0f^(0x01<<GetSioConnectId()));	//������ID���擾����XOR����i��ʂ͖��������j
}

/*----------------------------------------------------------------------------------*/
/*�f�[�^�����]���J�n�R�}���h���s�֐�												*/
//
//����
//  SendTo: ���MID��BIT�P�ʂŎw��i0-3bit�����̂܂�ID�̎w��ƂȂ�j	�����@���݈Ӗ��Ȃ�
//          bit�������Ă���ꍇ�͑��M����							����
//  buf:	���M�f�[�^�i�[�|�C���^
//  size:	���M�f�[�^�T�C�Y�i�ō�256�޲āj
//	
//�Ԃ�l
//  �o�^�����̏ꍇ��SUCCESS,�ʐM���s����BUSY�i�ēo�^���K�v�j
/*----------------------------------------------------------------------------------*/
u8 SplitDataSendStart(u8 SendTo, u8 *buf,u16 size)
{
	return SetNumerousData(buf, size);
	
}


/*----------------------------------------------------------------------------------*/
/*�f�[�^���������J�n�R�}���h���s�֐�												*/
//
//����
//  No�F�f�[�^�������X�g�Q�Ɣԍ�
//	    pokesio.h����TransListNo[][2]�̉��Ԃ̃��X�g���g�p���邩���w��
//	    TransList���߲���ƻ��ނ��w�肵�Ă���z��
//	
//�Ԃ�l
//  �o�^�����̏ꍇ��SUCCESS,�ʐM���s����BUSY�i�ēo�^���K�v�j
/*----------------------------------------------------------------------------------*/
u8 SplitDataExchangeStart(u8 No)
{
	if(PokemonMultiSioFunc==NULL){
		TransListNo = No;
		SetSendBuf(EXCHANGE_DATA);
		
		return 1;
	}
	return 0;
}
/*----------------------------------------------------------------------------------*/
/*  SplitDataSendStart��錾�ł��邩�ǂ���											*/
/*----------------------------------------------------------------------------------*/
u8 SplitDataSendStatus()
{
	return (PokemonMultiSioFunc==NULL);
}

/*----------------------------------------------------------------------------------*/
/*�f�[�^�����ʐM���̃t���O��Ԃ�													*/
/*																					*/
/*	0:�������܂��͏�������	1:�����I��												*/
/*----------------------------------------------------------------------------------*/
u8 SplitDataResult()
{
//	return SplitDataFuncFlag;
	return (SplitDataFuncFlag[3]<<3)|(SplitDataFuncFlag[2]<<2)|(SplitDataFuncFlag[1]<<1)|SplitDataFuncFlag[0];
}


/*----------------------------------------------------------------------------------*/
/*  �f�[�^�����ʐM�I�����Z�b�g														*/
/*----------------------------------------------------------------------------------*/
void SplitDataResultSet(u8 id)
{
//	SplitDataFuncFlag |= 1<<id;		
	SplitDataFuncFlag[id] = 1;
}
/*----------------------------------------------------------------------------------*/
/*  �f�[�^�����ʐM���̃t���O�����Z�b�g����											*/
/*----------------------------------------------------------------------------------*/
void SplitDataResultReset()
{
	int i;
	for(i=0;i<4;i++)
		SplitDataFuncFlag[i] = 0;
//	SplitDataFuncFlag = 0;		
}

/*----------------------------------------------------------------------------------*/
/*  �f�[�^�����ʐM���̃t���O��ID�P�ʂŃ��Z�b�g����									*/
/*----------------------------------------------------------------------------------*/
void SplitDataResultReset2(u8 id)
{
//	if((SplitDataFuncFlag&(1<<id)))
//		SplitDataFuncFlag ^= 1<<id; 
	if(SplitDataFuncFlag[id])
		SplitDataFuncFlag[id] = 0;
}

/*----------------------------------------------------------------------------------*/
/*  �e�̏ꍇ�ɒʐM�J�n�̃t���O���Z�b�g����*/
/*----------------------------------------------------------------------------------*/
void MultiSioSignalDecision()
{
	if(SioFlags&MULTI_SIO_PARENT){
		if(((SioFlags&MULTI_SIO_PLAYERS_NUM_MASK)>>MULTI_SIO_PLAYERS_NUM_SHIFT)>1){
			Signal = SIGNAL_DECISION;
		}
	}
}

/*----------------------------------------------------------------------------------*/
/*  �`�F�b�N�T���쐬																*/
/*																					*/
/*  u16�̔z���size�̉񐔑����Z���Đi�߂��l���`�F�b�N�T���Ƃ��ĕԂ��܂��B			*/
/*  p     : u16�̔z��																*/
/*  size  : �T�C�Y																	*/
/*  �Ԃ�l: �`�F�b�N�T��															*/
/*----------------------------------------------------------------------------------*/
u16 CheckSumMake(u16 *p, u16 size)
{
	u16 i,checksum;
	
	checksum = 0;				//�`�F�b�N�T���̏����l�͂O����n�߂Ă܂�
	for(i=0;i<size/2;i++){
		checksum += p[i];
	}
	
	return checksum;
}

#ifdef DEBUG_NUMPRINT
/*================================================================================*/
/*�f�o�b�O�p����put*/
/*================================================================================*/
static void numput(u8 num, u8 x, u8 y)
{
	u16 *p;
	
	p = (u16*)(BG_VRAM+0x800*debugnum[0]);
	p[y*32+x] = (u16)((debugnum[1]<<12)|(num+1));
}

/*================================================================================*/
/*�f�o�b�O�p����print*/
/*================================================================================*/
void numprint(u32 num, u8 x, u8 y, u8 keta)
{
	int i;
	u8  work[16];
	
	for(i=0;i<keta;i++){
		work[i] = num&0xf;
		num    /= 0x10;
	}
	for(i=keta-1;i>=0;i--){
		numput(work[i],x,y);
		x++;
	}
}

static void PokemonIdKoukanOya()
{
	if(SioFlags&MULTI_SIO_PARENT)
		SetSendBuf(NINSHOU_REQ);
	PokemonMultiSioFunc = NULL;
}


#endif



/*----------------------------------------------------------------------------------*/
/*  ��ԕ\��*/
/*----------------------------------------------------------------------------------*/
void kanshi_network(u8 taskno)
{

#ifdef DEBUG_NUMPRINT
	int i;
	numprint(Signal, 		2,	1,	2);
	numprint(SioFlags, 		15,	1,	8);
	numprint(M_s.State, 	2,	10,	2);
	numprint(((SioFlags & MULTI_SIO_PLAYERS_NUM_MASK) >> MULTI_SIO_PLAYERS_NUM_SHIFT), 15,10,2);
	numprint(GetSioConnectId(), 15,12,2);
	numprint(Send_Buf_Size,	25,	1,	2);
	numprint(Recv_Buf_Size,	25,	2,	2);

	numprint(SplitDataResult(),15,5,2);
	numprint(DebugWork0,		2,12,8);
	numprint(DebugWork1,	2,13,8);

	numprint(SioParentConnectCheck(),	25,5,1);
	numprint(SioChildConnectCheck(),	25,6,1);
	numprint(MultiSioMainCheck(),		25,7,1);
	numprint(MultiSioErrorCheck(),		25,8,1);

	for(i=0;i<4;i++)
		numprint(checksumwork[i], 10,4+i,4);
#if	defined(FIELD_TUUSHIN_DEBUG) && defined(PM_DEBUG)
	for(i=0;i<4;i++)
		numprint(debug_player_cmd[i],20,12+i,4);
#endif

//	numprint(RecvBuf[1][0],3,5,4);
//	numprint(RecvBuf[1][1],3,6,4);
//	numprint(RecvBuf[1][2],3,7,4);
//	numprint(RecvBuf[1][3],3,8,4);
#endif
}

#ifdef DEBUG_NUMPRINT
// =============================================
// kanshi_num_set
// �T�v  : �Ď��֐�kanshi_network�ŕ\��������Ă���
// ����  : w0	
//       : w1
// �߂�l: none
// =============================================
void kanshi_num_set(u32 w0, u32 w1)
{
	DebugWork0 = w0;
	DebugWork1 = w1;
}
#endif


// =================================================================
// GetConnectMemberBitData
// �T�v  : ���ݐڑ����̐l�����ޯĂł���킵�����ŕԂ�
// ����  : none
// �߂�l: u8(0�Ƃ�f�Ƃ�)
// =================================================================
u8 GetConnectMemberBitData()
{
	int i;
	u8  ret;
	
	ret = 0;



//	for(i=0;i<GetConnectSioMenberNum();i++){
	for(i=0;i<SioReliableNumber;i++){
		ret |= 1<<i;
	}

	return ret;
}

/*----------------------------------------------------------------------------------*/
/*  ���ݐڑ����Ă���l����ێ�����													*/
/*----------------------------------------------------------------------------------*/
void SetSioReliableNumber(u8 num)
{
	int i;

	SioReliableNumber = num;				//�ڑ��l���̕ۑ�
	SioReliableId	  = GetSioConnectId();	//�����̂h�c�̕ۑ�

	for(i=0;i<4;i++){					//�ڑ������o�[�̏���ۑ�
		ReliableMemberInfo[i] = SioTrainerInfo[i];
	}
}

/*----------------------------------------------------------------------------------*/
/*  �ڑ����Ă����l����Ԃ�															*/
/*----------------------------------------------------------------------------------*/
u8 GetSioReliableNumber()
{
	return SioReliableNumber;
}


/*----------------------------------------------------------------------------------*/
/*  �ڑ����Ă����h�c��Ԃ�															*/
/*----------------------------------------------------------------------------------*/
u8 GetSioReliableId()
{
	return SioReliableId;
}
/*----------------------------------------------------------------------------------*/
/*  �ێ����Ă��郁���o�[�̏�񂪍Đڑ����ɂ����Ă��邩�m�F����						*/
/*----------------------------------------------------------------------------------*/
u8 CheckSioReliableMember()
{
	int i,w;

	w = 0;
	for(i=0;i<SioReliableNumber;i++){
		if( SioTrainerInfo[i].Id == ReliableMemberInfo[i].Id)
//		&& PM_strcmp(SioTrainerInfo[i].name,ReliableMemberInfo[i].name)==0)
		{
			w++;
		}
	}
	if(w==SioReliableNumber) return 1;
	return 0;

}

// =================================================================
// CheckMyTrainerInfo
// �T�v  : ID�A���O����O��ʐM���ƕς���Ă��Ȃ����`�F�b�N
//			�����F�ؗp
// ����  : none
// �߂�l: u8
// =================================================================
void CheckMyTrainerInfo()
{
	u8 id;

	for(id = 0;id<SioReliableNumber;id++){
		if( ReliableMemberInfo[id].Id != SioTrainerInfo[id].Id
			|| PM_strcmp(ReliableMemberInfo[id].name,SioTrainerInfo[id].name)!=0){
	
			SioErrorFlag = 1;
			PokemonMultiSioEnd();
			MainProcChange(ErrGamenInit);
		}
	}
}

/*----------------------------------------------------------------------------------*/
/*  ���ݐڑ����Ă���l����ر����													*/
/*----------------------------------------------------------------------------------*/
void ClearSioReliableNumber()
{
	SioReliableNumber = 0;
	SioReliableId	  = 0;
}


/*----------------------------------------------------------------------------------*/
/*  ����(���A���^�C����)�ڑ����Ă���l����Ԃ�										*/
/*----------------------------------------------------------------------------------*/
u8 GetSioRealConnectNumber()
{
	return ((SioFlags & MULTI_SIO_PLAYERS_NUM_MASK) >> MULTI_SIO_PLAYERS_NUM_SHIFT);
}


/*----------------------------------------------------------------------------------*/
/*  �e�ڑ����q�ڑ���																*/
/*----------------------------------------------------------------------------------*/
u8 GetMyConnectIsParent()
{
	return ((SioFlags&MULTI_SIO_PARENT)!=0);
}

/*----------------------------------------------------------------------------------*/
/*  �ʐM�pFIFO�ޯ̧�ر�����ǂ�����Ԃ�												*/
/*																					*/
/*  1:�ر�����߰�ނ��������Ă���r��												*/
/*  0:�ʐM��																		*/
/*----------------------------------------------------------------------------------*/
u8 GetPokemonSioPauseFlag()
{
	return MultiSioPauseFlag;
}


// =================================================================
// SetEndDataStart
// �T�v  : �I���R�}���h���s���J�n����
// ����  : none
// �߂�l: none
// =================================================================
void SetSioEndDataStart()
{
	if(PokemonMultiSioFunc!=NULL) return;
	PokemonMultiSioFunc = SetSioEndDataSend;
	SioEndFlag = 0;
}

// =================================================================
// SetSioEndDataSend
// �T�v  : ��M�o�b�t�@���O�ɂȂ�̂��m�F������I���R�[�h�𑗐M
// ����  : none
// �߂�l: none
// =================================================================
static void SetSioEndDataSend()
{
	if(Recv_Buf_Size==0){						//�Ȃɂ��f�[�^�𑗐M���Ȃ���FIFO���N���A�����̂�҂�
		SetSendBuf(SIO_END_DATA);
		PokemonMultiSioFunc = SioEndDataWait;
	}
}
// =================================================================
// SetSioEndData
// �T�v  : �I���R�}���h�𔭍s���Â���E�݂��END_DATA�ɂȂ�����t���O���Ă�
// ����  : none
// �߂�l: none
// =================================================================
static void SioEndDataWait()
{
	int i,num,work;
	
	num = GetConnectSioMenberNum();

	work = 0;
	for(i=0;i<num;i++){						//�ʐM���Ă���S������G���h�f�[�^������̂�҂�
		if(MultiSioEndFlag[i]) work++;
	}
	if(work==num){							//�S�����󂯎������ʐM�ؒf
		FightType &= (FIGHT_TYPE_TUUSHIN_OK ^ 0xffff);		//�ʐM�퓬�p�̃t���O�N���A
		SioStopFlag = 1;					//����1sync���ɋN����ʐM�G���[�t���O�͖���
		PokemonMultiSioEnd();
		PokemonMultiSioFunc = NULL;
		SioEndFlag = 1;						
	}
}




// =================================================================
// SetSioSynchronize
// �T�v  : �ʐM�����J�n���[�`��
// ����  : none
// �߂�l: none
// =================================================================
void SetSioSynchronize()
{
	if(PokemonMultiSioFunc == NULL){
		PokemonMultiSioFunc = SioSynchronizeWait;
	}
	SioEndFlag=0;
}


// =================================================================
// SioSynchronizeWait
// �T�v  : 
// ����  : none
// �߂�l: none
// =================================================================
static void SioSynchronizeWait()
{
	if(Recv_Buf_Size==0){					//�Ȃɂ��f�[�^�𑗐M����FIFO�����������̂�҂�
		SetSendBuf(SEND_SYNCRO_DATA);
		PokemonMultiSioFunc = SioSynchronize;
	}
}

// =================================================================
// SioSynchronize
// �T�v  : �ʐM�����҂�
// ����  : none
// �߂�l: none
// =================================================================
static void SioSynchronize()
{
	u8 num,i,w;

	num = GetConnectSioMenberNum();			//�S�����瓯���R�}���h�������Ă�����I��
	for(i=0;i<num;i++){
		if(MultiSioSynchroFlag[i] == 0){
			break;
		}
	}
	if(i==num){
		for(w=0;w<4;w++)
		MultiSioSynchroFlag[w] = 0;			//�����t���O�N���A
		PokemonMultiSioFunc = NULL;
	}

}


#ifdef ERROR_CHECK


// =================================================================
// SioErrCheck
// �T�v  : �ʐM�G���[���󂯎������G���[���m��ʂɔ�΂�
// ����  : none
// �߂�l: none
// =================================================================
static void SioErrCheck()
{
	if(multisio_start_flag==OFF) return;
	if(SioFlags&(MULTI_SIO_HARD_ERROR|MULTI_SIO_CHECKSUM_ERROR|MULTI_SIO_SEND_OVER_FLOW_ERROR|
				MULTI_SIO_RECV_OVER_FLOW_ERROR|MULTI_SIO_INTERVAL_ERROR|MULTI_SIO_ID_OVER_ERROR
				|MULTI_SIO_INTR_STOP_ERROR)){

		if(SioErrorPermitFlag==SIO_ERROR_PERMIT_JUMP){						//�ʐM�G���[��ʂɂƂ΂��Ă���
			HozonSioFlags = SioFlags;
			HozonRecvSize = Recv_Buf_Size;
			HozonSendSize = Send_Buf_Size;
			MainProcChange(ErrGamenInit);
		}											
		SioErrorFlag = 1;
		PokemonMultiSioEnd();
	}
	
}

// =================================================================
// ErrGamenInit
// �T�v  : �ʐM�G���[��ʏ�����
// ����  : none
// �߂�l: none
// =================================================================
void ErrGamenInit()
{
	InitActor();				//�A�N�^�[������
	ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
	FadeWorkInit();				//�p���b�g�A�j���֌W�̏���������
	PaletteWorkClear(0,0,2);	//�p���b�g���[�N�̏�����
	InitTask();


	SetVBlankFunc(VInterFunc);

	PrintScreenInit(FIELD_MENU_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);
	NMenuScreenClear();
	
	*(vu16 *)REG_BLDALPHA = 0;
	*(vu16 *)REG_BG0VOFS  = 0;
	*(vu16 *)REG_BG0HOFS  = 0;

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
							;
	SoftResetDisable = 0;					//�\�t�g���Z�b�g������L����

	AddTask(CheckParent,0);


	BGM_PlayStop();

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
	MainProcChange(ErrGamenMain);

	
}

static const u8 spc_msg[]={spc_,EOM_,};

// =================================================================
// ErrGamenMain
// �T�v  : �ʐM�G���[��ʃ��C��
// ����  : none
// �߂�l: none
// =================================================================
void ErrGamenMain()
{
	u8 strbuf[30];
	u8 numbuf[30];
	switch(sys.InitSeqWork){
		case 0:
			NMenuMsgWrite(err_msg,7,7);

#ifdef PM_DEBUG
				PM_strcpy(strbuf,err_num);
				PM_HexNumMsgSet(numbuf, HozonSioFlags, NUM_MODE_ZERO, 8);
				PM_strcat(strbuf,numbuf);
				PM_strcat(strbuf,spc_msg);
				PM_HexNumMsgSet(numbuf, HozonSendSize, NUM_MODE_ZERO, 2);
				PM_strcat(strbuf,numbuf);
				PM_strcat(strbuf,spc_msg);
				PM_HexNumMsgSet(numbuf, HozonRecvSize, NUM_MODE_ZERO, 2);
				PM_strcat(strbuf,numbuf);
				NMenuMsgWrite(strbuf,2,15);
#endif
			break;
		case 30:
			SePlay(SE_BOO);
			break;
		case 60:
			SePlay(SE_BOO);
			break;
		case 90:
			SePlay(SE_BOO);
			break;
	}

	if(sys.InitSeqWork!=200) sys.InitSeqWork++;
}
#else
void ErrGamenInit(){}
#endif



// =================================================================
// SioParentConnectCheck
// �T�v  : �P�[�u����������Ă��Ȃ����`�F�b�N�i�ʐM�̐e�̏ꍇ�̂݁j
// ����  : none
// �߂�l: u8	0:����		1:�ُ�i�����ꂽ�j
// =================================================================
u8 SioParentConnectCheck(void)
{
//	return !(SioFlags&MULTI_SIO_PARENT);
	return ((*(vu16*)REG_SIOCNT&(1<<2)) != 0);
}

// =================================================================
// SioChildConnectCheck
// �T�v  : �P�[�u���������ւ����Ă��Ȃ����`�F�b�N�i�q�̏ꍇ�̂݁j
// ����  : none
// �߂�l: 0:����		1:�q���̂͂��Ȃ̂ɐe�̃R�l�N�^���������܂ꂽ
// =================================================================
u8 SioChildConnectCheck(void)
{
//	u8 flag = 0;
//	if((*(vu16*)REG_SIOCNT & (1<<3))!=0){
//		if( (*(vu16*)REG_SIOCNT & (1<<2))==0) flag = 1;
//	}
//	return flag;
	return (  (*(vu16*)REG_SIOCNT & (1<<3))!=0 &&  (*(vu16*)REG_SIOCNT&(1<<2))==0);
}

// =================================================================
// MultiSioMainCheck
// �T�v  : �ʐM���C�������`�F�b�N
// ����  : none
// �߂�l: 0:�ʐM����		1:�ʐM��
// =================================================================
u8 MultiSioMainCheck(void)
{
	return ((SioFlags&MULTI_SIO_MAIN_SEQ)!=0);
}

// =================================================================
// MultiSioErrorPermit
// �T�v  : �ʐM�G���[���N�����Ƃ��ɃG���[��ʂɔ�΂����ǂ���������
// ����  : flag	
//		SIO_ERROR_PERMIT_JUMP:�ʐM�G���[��ʂɔ�΂�(�f�t�H���g)
//		SIO_ERROR_NO_JUMP	 :��΂��Ȃ�(�׸ތ��o�͉\)
// �߂�l: none
// =================================================================
void MultiSioErrorPermit(u8 flag)
{
	SioErrorPermitFlag = flag;
}

// =================================================================
// MultiSioErrorCheck
// �T�v  : �ʐM�G���[���N�������ǂ�����Ԃ�
// ����  : none
// �߂�l: u8	0:�G���[����	1:�G���[����
// =================================================================
u8 MultiSioErrorCheck(void)
{
	return SioErrorFlag;
}