//=========================================================================
//	�u�����h�}�V�[���Q�[��
//
//    Copyright (C) 2001 GAME FREAK inc.
//											By Akito Mori
//=========================================================================

#define DEBUG_NPC_CHANGE
#define DEBUG_NUMPRINT
//#define DEBUG_PARAMETA			//�L���[�u�̃p�����[�^��\��

//#define SEEQUENCE_DEBUG

#include "common.h"
#include "evobj.h"
#include "config.h"
#include "multisio.h"
#include "pokesio.h"
#include "cable.h"
#include "mus_tool.h"

#include "intr.h"


#include "calctool.h"
#include "decord.h"
#include "objdata.h"
#include "svld_ini.h"
#include "task.h"
#include "print.h"
#include "fcontext.h"
#include "window.h"
#include "menu.h"
#include "menutool.h"
#include "boxsub.h"

#include "pocketno.h"
#include "item.h"
#include "seed.h"
#include "cube_def.h"
#include "cubecase.h"
#include "actor.h"
#include "actanm.h"
#include "Madefine.h"
#include "palanm.h"
#include "param.h"
#include "fld_main.h"
#include "ev_flag.h"
#include "record.h"

#include "pokeicon.h"
#include "wazatool.h"

#include "blend_m.h"


#include "..\scaddata\blend_r.acl"		//��]�ʉ摜
//#include "..\scaddata\b_r.ach"
#include "..\scaddata\b_r.asc"

#include "..\scaddata\blendbak.acl"		//�w�i
//#include "..\scaddata\blendbak.ach"
//#include "..\scaddata\blendbak.asc"

#include "..\scaddata\blendpat.acl"		//�u�����h�Q�[���p�[�c
#include "..\scaddata\blendpat.ach"

#include "blendmsg.h"
//=========================================================================
//	��`
//=========================================================================
//#define name value

#define ROTATE_CENTER_X 	(120)							//��]���SX
#define ROTATE_CENTER_Y 	(80)							//��]���SY
#define ROTATE_MAP			(8)								//BGMAP��̉�]����ꏊ
#define ROTATE_CHARA		(0)								
#define BACK_MAP			(12)							//�w�iBGMAP��ذݔԍ�
#define BACK_CHARA			(2)								//�w�iBG��הԍ�
#define BLEND_BACK_MAP		(BG_VRAM+0x800*BACK_MAP)		//�w�iBGMAP��ذݱ��ڽ
#define METER1_PAT_0		(0x80e1)						//Ұ���󔒷�הԍ�
#define METER1_POS_X 		(0xb)							//Ұ�����WX
#define METER1_POS_Y 		(0)								//Ұ�����WY
#define METER1_POS_INDEX	(METER1_POS_X+METER1_POS_Y*32)	//Ұ����הz�u�ʒu
#define METER1_MAP_MAX		(8)								//Ұ����׍ő吔
#define METER1_DOT_MAX		(METER1_MAP_MAX*8)				//Ұ���h�b�g��MAX

#define RESULT_X1			(4)
#define RESULT_Y1			(2)
#define RESULT_X2			(25)
#define RESULT_Y2			(17)

#define BLEND_TIME_MAX		((99*60+59)*60)					//�X�X���T�X�b�܂�

#define SPEED_PAT_0			(0x8072)						//��߰��Ұ����הԍ�
#define SPEED_POS_X 		(12)							//��߰��Ұ�����WX
#define SPEED_POS_Y 		(17)                            //��߰��Ұ�����WY
#define SPEED_POS_INDEX		(SPEED_POS_X+SPEED_POS_Y*32)	//��߰��Ұ���ʒu

#define ROTATE_DEMO_RATIO		(0x50)
#define ROTATE_FIRST_RATIO		(0x100)
#define ROTATE_DEMO_VSPEED		(512)
#define ROTATE_DEMO_STEP		(4)
#define ROTATE_DEMO_SPEED		((ROTATE_FIRST_RATIO-ROTATE_DEMO_RATIO)/ROTATE_DEMO_STEP)

// ----------÷�ĕ\���p�̾�Ďw��---------------------
#define	NAME_CHR_SIZE	(10*2)				//���O�̾��
#define	YESNO_CHR_SIZE	(NELEMS(yesno)*2)	//�͂��E�������̾��
#define CURSOR_CHR_SIZE (2)					//���ٵ̾��
#define BLANK_CHR_SIZE (2)					//�󔒷�׵̾��
#define WINMSG_CHR_SIZE	(100)				//�E�C���h�E��ү���޵̾��

#define V_KAKUDO_MIN	(128)				//��]���xMIN
#define V_KAKUDO_MAX	(1500)				//��]���xMAX�i������JUST���o���Ə������オ��j
#define PLUS_OVER_MAX	(0x80)				//MAX�𒴂����Ƃ���JUST���o�������p�̐ݒ�
#define PLUS_V_KAKUDO	(0x100)				//�����̎�����׽��߰�ށi���l���j
#define PLUS_JUST_V_KAKUDO	(0x180)			//JUST�̎������ٽ�߰�ށi���l���j
#define MINUS_V_KAKUDO	(0x100)				//���s�̎���ϲŽ��߰�ށi���l���j
#define ENDMETER_MAX	(100*10)			//�I�����[�^�[�̕�(�����v�Z�p)
#define JUST_PLUS_DIV	(55)				//JUST�̎��̌v�Z����i�I�����[�^�[�p�j
#define PLUS_DIV		(70)				//�����̎��̌v�Z����i�I�����[�^�[�p�j


#define NAME_CHR_START_NO	0				
#define YESNO_CHR_START_NO	(NAME_CHR_START_NO+NAME_CHR_SIZE*4)		//�͂��E�������̃I�t�Z�b�g
#define CURSOR_CHR_START_NO	(YESNO_CHR_START_NO+YESNO_CHR_SIZE)		//�J�[�\���̃I�t�Z�b�g
#define BLANK_CHR_START_NO	(CURSOR_CHR_START_NO+CURSOR_CHR_SIZE)	//�󔒂̃I�t�Z�b�g
#define WINMSG_CHR_START_NO	(BLANK_CHR_START_NO+BLANK_CHR_SIZE)		//TALK���߂ŕ\���̃e�L�X�g�̃I�t�Z�b�g

//-------------�ʐM�p�R�}���h��`-----------------------------------------------
#define COMMAND_BOO		(0x2345)			//�ʐM�p�{�^���������s�R�}���h
#define COMMAND_OK		(0x5432)			//�ʐM�p�{�^�����������R�}���h
#define COMMAND_JUST	(0x4523)			//�ʐM�p�{�^�������W���X�g�R�}���h
#define COMMAND_END		(0x2534)			//�ʐM�p�u�����h�I���R�}���h

#define	BLEND_END				(0x1111)	//�e����u�����h�}�V�[�����I������R�}���h
#define	BLEND_CONTINUE			(0x2222)	//�e����p�����ăv���C����Ƃ����R�}���h
#define	REASON_NO				(0x3333)	//NO��I�������l������
#define	REASON_NOKINOMI			(0x4444)	//���ꂩ���������݂̂��Ȃ�
#define	REASON_FULLCUBE			(0x5555)	//���ꂩ���L���[�u�P�[�X�������ς�
#define	REASON_ALLNO			(0x6666)	//�S��NO��I������
#define	SELECT_YES				(0x7779)	//�u�͂��v��I��
#define	SELECT_NO				(0x8888)	//�u�������v��I��
#define	SELECT_YES_BUT_NOKINOMI	(0x9999)	//�u�͂��v��I�������񂾂��ǂ��݂̂��Ȃ�
#define	SELECT_YES_BUT_FULLCUBE (0xaaaa)	//�u�͂��v��I�������񂾂��ǃL���[�u�����������ς�



static const s8 arrowmove[4][2]={
	{-1,-1},
	{ 1,-1},
	{-1, 1},
	{ 1, 1},
};

static const u8 namepos[2][4][2]={			//���O�̈ʒu	
	{
		{2,6},{23,6},{2,12},{23,12},			//�T�����p
	},
	{
		{1,6},{22,6},{1,12},{22,12},			//�V�����p
	},
};

static const u8 arrowpos[4][2]={		//���i��]�p�j�̈ʒu
	{120-48,80-48},
	{120+48,80-48},
	{120-48,80+48},
	{120+48,80+48},
};

static const u8 table_table[3][4]={		//�Q�`�S�l�v���C�̎��̔z�u�e�[�u��
	{0xff, 0, 1,0xff},
	{0xff, 0, 1,   2},
	{   0, 1, 2,   3},
};

//static const u16 start_point[]={0xb480,0x7100,0xf600,0x3200};		//�����ʒu�̃z�C�[���p�x
//static const u16 start_point[]={0xa000,0x6000,0xe000,0x2000};		//�����ʒu�̃z�C�[���p�x
static const u16 start_point[]={0x0000,0xc000,0x4000,0x8000};		//�����ʒu�̃z�C�[���p�x
static const u8  start_tbl[]={1,1,0};								//�����ʒu�̂Ƃ�ꏊ


//static const u8 hit_table[]={5,1,9,13};
//        0            1
//          C0       80
//�@�@�@�@�@�@�~�\�~
//�@�@�@�@�@�~�@�@�@�~
//�@�@�@�@�@�b�@�@�@�b
//�@�@�@�@�@�~�@�@�@�~
//�@�@�@�@�@�@�~�\�~
//          0        40
//        2            3

//        0            1
//          C0       80
//�@�@�@�@�@�@�~�\�~
//�@�@�@�@�@�~�@�@�@�~
//�@�@�@�@�@�b�@�@�@�b
//�@�@�@�@�@�~�@�@�@�~
//�@�@�@�@�@�@�~�\�~
//          0        40
//        2            3


static const u8 hit_table[]={0x20, 0xe0, 0x60, 0xa0};





//static const u8 hit_table[]={0xc0, 0x80, 0, 0x40};


void BlendTestInit(void);
void BlendTestMain(void);
void ItemJingle2ContestBgm(u8 no);

extern const u32 DecordAdrs[];
extern void BlendMachineInit();
extern void CubeBlendBagInit();		//�ޯ�މ�ʌĂяo���֐�
extern u16 ReturnItemNo;			//�I�����ꂽ�^�l���i�[
extern void kanshi_num_set(u16 w0, u16 w1);
extern void DebugNumPrintInit(u8 pal, u8 select, u8 screen, u8 chara);
extern u32 GetBlendMachineCount();

extern void BlendMachineFunction_Set();	//�L�[�����֐�
extern void BlendMachineStartSend();	//�u�����h�}�V�[���X�^�[�g�����p
extern u8 SoftResetDisable;				//�\�t�g���Z�b�g���s����

extern u8   SendXydataFlag;
extern SIOTRAINER_INFO SioTrainerInfo[4];
extern u16  RecvBuf[MULTI_SIO_RECV_MAX][MULTI_SIO_PLAYERS_MAX];
extern u16  SendBuf[MULTI_SIO_SEND_MAX];
extern u8	SendPoolBuf[];

extern void PokemonMultiSioInit();
extern void kanshi_network(u8 no);

extern u8 SetTagSeedActEx( u8 no , u8 x ,u8 y, u8 flag);

// =================================================================
// BlendMachineTypeSet
// �T�v  : �^�l�u�����_�[�̐ݒ������i��l�p�̂Ƃ��ɂQ�l���S�l���j
// ����  : type
//			PLAYER_SIO	0			//�ʐM�Ńv���C
//          PLAYER_2	1			//�R���s���[�^�ƃv���C�E�Q�l
//          PLAYER_3	2			//�R���s���[�^�ƃv���C�E�R�l
//          PLAYER_4	3			//�R���s���[�^�ƃv���C�E�S�l
// �߂�l: extern void
// =================================================================
void BlendMachineTypeSet(u8 type);

// =================================================================
// BlendMachineTitleInit
// �T�v  : �u�����h�}�V�[���̃X�^�[�g
// ����  : none
// �߂�l: extern void
// =================================================================
void BlendMachineTitleInit();


// =================================================================
// BlendMachineInit
// �T�v  : �o�b�O�Ń^�l��I��������ɌĂԃ��C�����[�`���ւ̏�����
// ����  : none
// �߂�l: GLOBAL void
// =================================================================
void BlendMachineInit();


void BlendMachineMain();
void BlendMachineTitle();

void BlendMachineEnd();
void BlendMachineEnd_Sio();
void BlendMachineEnd_Offline();


void WaitBlendMachineStart(u8 no);
void BlendMachineMemberItem(u8 Id, u16 Item);


static void SioFunc();
static void MeterFunc(u16 meter, u16 max);
static void MeterFuncSub(u16 meter, u16 max);
static void SpeedMeterFunc(u16 v);
static void rotate_func();
static void BlendBGAffineSet(	BgAffineDestData *des);
static void BlendMachineInit_Sio();
static void BlendMachineInit_Offline();

static void hibana_actor_add();
static void hibana_act_move(actWork *act);
static void HanteiMove(actWork *act);
static void HanteiMove2(actWork *act);
static void VsyncRotateSet();
static void VsyncBgOffset();

static void NPC_hantei0(u8 no);
static void NPC_hantei1(u8 no);
static void NPC_hantei2(u8 no);
static void OfflineSendRecvBufInit();


// NPC�̃u�����_�[AI�e�[�u��
static void (* const  NPC_functable[])(u8 no)={
	NPC_hantei0,
	NPC_hantei1,
	NPC_hantei2,
};


static void SpeedUpVibration();
static void SpeedUpVibrationSet(s16 *n, u16 offset);
static void CubeComentSet(CUBE *p, u8 *buf);

static u8 BlendMachineResultPut();
static u8 BlendMachineResultPut2();

static void CountActSeq(actWork *Xreg);			
static void StartActSeq(actWork *Xreg);			

static u8 BlendMachineVibration();


u8 BlendTalkMsgPut( void );
void BlendTalkMsgSet( const u8 * msg, u16 chr_no, u8 x, u8 y );
void BlendMsgPrint( const u8 * msg, u16 chr_no, u8 x, u8 y );
void BlendWindMsgPrint(const u8 *msg, u16 chr_no,u8 x1, u8 y1, u8 x2, u8 y2);
static void BlendMachineNamePrint();
static void PositionTableMake();


//=========================================================================
//	�ϐ�
//=========================================================================
//extern type
//-------------------- extern�ϐ�  ---------------------------------

//GLOBAL type valiable;
/*-------------------- �O���[�o�� �ϐ�  ----------------------------*/

//LOCAL type valiable;
/*-------------------- ���[�J�� �ϐ� -------------------------------*/
typedef struct{
	u16 no;
	u8 name[SEED_NAME_SIZE+EOM_SIZE];		//���O
	u8 para[7];								//�p�����[�^
}KINOMIWORK;

typedef struct{
	u32 timer;
	u16	bpm_max;
	u16 etc;
}BLEND_RESULT;

typedef struct{
	u8 			initflag;
	u8			graphic_init;
	TALK_WIN 	tw;					//÷�ĺ�÷��
	u8			windowwork;
	u8 			end_ok[2];
	u16			sendwork[10];
	u8			exchangeflag;		//
	u8			exchangeold;
	u16			exchangecount;
	u16			endwait;
	u8			actno[3];
	u16 		kakudo;				//��]�p	�i����8bit�j
	s16 		v_kakudo;			//��]��߰��
	u16			oldkakudo;
	u16			bpm_max;			//�ō���]�X�s�[�h�̕ۑ��ޯ̧
	u8			cursoractno[4];	
	u8			cursorwork[4];
	u8			barno;
	u8			barpoint[10];
	u8			endflag;
	u16			siowork[4];
	u16			sio_reason[2];
	u16			selectwork;
	u8			down_count;
	u16         memberitem[4];
	u8			member_num;
	u16			oldpad[4];
	u16			pad[4];
	u16			table[4];			//�Q�`�S�l�v���C�̔z�u�e�[�u��
	u16			reverse[4];			//�t�����p�̃e�[�u��
	u8			cursor;
	u8			strbuf[50];
	u32			timer;
	s32			wait;
	u32			end_watch;			//�J�n���ɒʐM���R�O�b�q�����Ƃ��ł��Ȃ�������I��
	u8			bound[4];			//�o�E���h���Ē����ɓ���؂̎������ԍ�
	u8			boundnum;			//�o�E���h������
	u16			end_meter;			//�I���܂ł̃��[�^�[
	u16			now_meter;			//�A�j�������邽�߂̃��[�^�[�p�̃e���|����
	u16			Ratio;				//��]�ʗp�̊֐�
	s16			BgOffsetX;			//��ʗh��l�̕ϐ�X
	s16			BgOffsetY;			//		�h		  Y
	u8			NPC_taskno[3];		//NPC�v���C���[����̔ԍ�
	u8			NPC_flag;			//NPC�v���C���[���X�[�p�[�T�C���l�ɂȂ�t���O(L_BUTTON)
	u16			hitcount[4][3];		//�W���X�g�E�����E���s�𐔂���
	u8			sort[4];			//�{�^�������̌��ʂŃ\�[�g
	BgAffineDestData Rotate;		//��]�ʾ�ėp���ޯ̧
	u16 		nowsong;			//���̂݃u�����_�[���n�܂�܂łȂ��Ă���Ȃ̔ԍ�
	KINOMIWORK	Kinomi[4];			//���̂݃p�����[�^�iRAM���̂ݑΉ��j
	BLEND_RESULT BlendResult;
	u32			saveseq;
#if SEEQUENCE_DEBUG
	u16			seqlognum;			//
	u8			seqlog[1000];		//�V�[�P���X���O
#endif
}BLENDMACHINE_WORK;

BLENDMACHINE_WORK *bmw;
u8	BlendMachineType;

//#define bmw ((BLENDMACHINE_WORK*)&UserWork[0x18000])



// =============================================
// �A�N�^�[��`
// =============================================

#include "blendact.h"


static void motor_pitch()
{
	m4aMPlayPitchControl(&m4a_mplay002,0xffff,(bmw->v_kakudo-128)*2);
}

/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static void VInterFunc( void )
{
	VsyncRotateSet();					//��]��ڼ޽��ݒ�
	VsyncBgOffset();					//BG1�̾�Đݒ�

	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}

static void VInterFunc2( void )
{

	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}


#if SEEQUENCE_DEBUG
static SeekLogCheck()
{
	if(bmw->seqlognum < 1000){
		bmw->seqlog[bmw->seqlognum] = bmw->initflag;
		bmw->seqlognum++;
	}
}
#endif

//===================================================================
//  �������ް�p�摜�]��
//===================================================================
static u8 BlendMachineGraphicSet()
{
	switch(bmw->graphic_init){
		case 0:
			//--------��]�ʗp�f�[�^�Z�b�g--------------
			DecordWram((void*)b_r_pch_ADRS, &UserWork[0x10000]);
			bmw->graphic_init++;
			break;
		case 1:
			DIV_DMAARRAYCOPY(3, b_r_Map, BG_VRAM+0x4000, 16);
			PaletteWorkSet(blend_r_Palette, 0, 16*8*2);
			bmw->graphic_init++;
			break;
		case 2:
			DIV_DMACOPY(3, &UserWork[0x10000], BG_VRAM, 0x2000,16)
			bmw->graphic_init++;
			break;
		case 3:
			DecordWram((void*)blendbak_pch_ADRS, &UserWork[0x10000]);
			bmw->graphic_init++;
			break;
		case 4:
			DecordWram((void*)blendbak_psc_ADRS, &UserWork[0x13000]);
			bmw->graphic_init++;
			break;
		case 5:
			//--------�w�i�Z�b�g-----------------------
			DIV_DMACOPY(3, &UserWork[0x10000], BG_VRAM+0x8000, 0x1000,16)
			bmw->graphic_init++;
			break;
		case 6:
			//--------�w�i�Z�b�g-----------------------
			DIV_DMACOPY(3, &UserWork[0x10000+0x1000], BG_VRAM+0x8000+0x1000, 0x1000,16)
			bmw->graphic_init++;
			break;
		case 7:
			DIV_DMACOPY(3, &UserWork[0x13000], BLEND_BACK_MAP, 0x500,16)
			PaletteWorkSet(blendbak_Palette, 16*8, 16*2);
			bmw->graphic_init++;
			break;

		case 8:
			//--------OBJ�Z�b�g----------------------
			CellSet((CellData *)&CursorActCell);	//�Z���Z�b�g
			CellSet((CellData *)&HibanaActCell);	//�Z���Z�b�g
			CellSet((CellData *)&HanteiActCell);	//�Z���Z�b�g
			bmw->graphic_init++;
			break;

		case 9:
			CellSet((CellData *)&CountActCell);	//�Z���Z�b�g
			CellSet((CellData *)&StartActCell);	//�Z���Z�b�g
			ObjPalSet((PalData *)&CursorActPal);	//�p���b�g�Z�b�g
			ObjPalSet((PalData *)&BlendPatActPal);	//�p���b�g�Z�b�g
			bmw->graphic_init=0;
			return 1;
			break;
	}
	return 0;
}



//===================================================================
//  �������ް�p��ʃ��W�X�^�ݒ�
//===================================================================
static void BlendMachineGraphicRegSet()
{
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_1		// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG0_ON					// BG0�L��
					  | DISP_BG1_ON					// BG0�L��
//					  | DISP_BG2_ON					// BG2�L��
						;

			//��]�ʐݒ�
			*(vu16 *)REG_BG2CNT = BG_COLOR_256			// 256�F�I��
						| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
						| BG_PRIORITY_0					// BG�D�揇��
						| ROTATE_MAP << BG_SCREEN_BASE_SHIFT		// �X�N���[���x�[�X�u���b�N
						| ROTATE_CHARA << BG_CHAR_BASE_SHIFT 		// �L�����N�^�x�[�X�u���b�N
						| BG_LOOP_OFF
						;

			//�w�i�ʐݒ�
			*(vu16 *)REG_BG1CNT = BG_COLOR_16			// 16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_1					// BG�D�揇��
						| 12 << BG_SCREEN_BASE_SHIFT	// �X�N���[���x�[�X�u���b�N
						| 2 << BG_CHAR_BASE_SHIFT 		// �L�����N�^�x�[�X�u���b�N
						;

			*(vu16 *)REG_BG0HOFS = 0;
			*(vu16 *)REG_BG0VOFS = 0;
            *(vu16 *)REG_BG1HOFS = 0;
            *(vu16 *)REG_BG1VOFS = 0;
}
//------------------------------------------------------------------
//  �^�C�g�����
//------------------------------------------------------------------
void BlendMachineTitleInit()
{
	bmw = (BLENDMACHINE_WORK*)&UserWork[0x18000];		//�߲���𓖂Ă�����
	bmw->endflag  = 0;
	bmw->initflag = 0;
	bmw->endflag=0;

	BlendMachineTypeSet(ScriptParameter0);
	MainProcChange(BlendMachineTitle);
}

//------------------------------------------------------------------
//  �^�C�g����ʂ̂��߂̏�q��
//------------------------------------------------------------------
void BlendMachineTitle()
{
	int i;
	switch(bmw->initflag){
		case 0:
			*(vu16*)REG_DISPCNT = 0;
//			PokemonMultiSioEnd();
			InitActor();				//�A�N�^�[������
			ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
			SetVBlankFunc(NULL);
			PrintScreenInit(BLENDMACHINE_TYPE);
			MsgControlInit(&(bmw->tw), BLENDMACHINE_TYPE);
			bmw->windowwork = NWinFrameInit(256);
//			NWinFrameDataSet(&(bmw->tw));
			NMenuScreenInitEx2(BLENDMACHINE_TYPE,1);			
			
			bmw->initflag++;
			bmw->now_meter = 0;			//�A�j�������邽�߂̃��[�^�[�p�̃e���|����
			bmw->end_meter = 0;
			bmw->Ratio     = ROTATE_DEMO_RATIO;
			bmw->BgOffsetX = 0;			//��ʗh��l�̕ϐ�X
			bmw->BgOffsetY = 0;			//		�h		  Y
			bmw->graphic_init = 0;
			rotate_func();
			break;
		case 1:
			//--------��]�ʁE�w�i�p�f�[�^�Z�b�g--------------
			if(BlendMachineGraphicSet()){		//�摜�]��
				//���ٱ����\��
				for(i=0;i<4;i++){
					bmw->cursoractno[i]=AddActor(&CursorHeader,arrowpos[i][0],arrowpos[i][1],1);
					ActAnmChg(&ActWork[bmw->cursoractno[i]],8+i);
				}
				SetVBlankFunc(VInterFunc);
				bmw->initflag++;
			}
			break;
		case 2:
			PaletteFadeReq(0xffffffff,0,16,0,0);			//�t�F�[�h�C��
			rotate_func();
			bmw->initflag++;
			break;
		case 3:
			BlendMachineGraphicRegSet();					//��ʃ��W�X�^�ݒ�
			if( FadeData.fade_sw == 0 )					
				bmw->initflag++;
			break;
		case 4:
			NMenuWinBoxWrite(0,14,29,19);
			BlendTalkMsgSet( title_msg, WINMSG_CHR_START_NO, 2, 15);

			//��ʕ\���ׂ̈Ɉ�񂾂���]�v�Z���Z�b�g
			bmw->initflag++;
			break;
		case 5:
			if(BlendTalkMsgPut()){		//���b�Z�[�W�̕\���I���҂�
				bmw->initflag++;
				PaletteFadeReq(0xffffffff,0,0,16,0);			//�t�F�[�h�C��
			}
			break;
		case 6:
			if( FadeData.fade_sw == 0 ){								//�t�B�[���h�֕��A
				CubeBlendBagInit();			//�^�l�I����ʂ�
				bmw->initflag=0;
			}
			break;
	}


	JumpActor();
	SetActor();
	PaletteAnime();
}


enum{
	TANE_HOZONY=0,
	TANE_MOVEX,
	TANE_MOVEY,
	TANE_FIRSTPOWER,
	TANE_POWER,
	TANE_BOUND,
	TANE_MUKI,
	TANE_MUKI2,
};
//===============================================================================
//  �o�E���h���邽�˂̃A�N�^�[����֐�
//===============================================================================
static void BoundSeedActMove(actWork *Xreg)
{
	Xreg->work[TANE_MOVEX] += Xreg->work[TANE_MUKI];
	Xreg->work[TANE_MOVEY] -= Xreg->work[TANE_POWER];
	Xreg->work[TANE_MOVEY] += 1  * Xreg->work[TANE_MUKI2];
	Xreg->work[TANE_HOZONY] += 1 * Xreg->work[TANE_MUKI2];
	Xreg->work[TANE_POWER] -= 1;
	if(Xreg->work[TANE_HOZONY]<Xreg->work[TANE_MOVEY]){
		Xreg->work[TANE_POWER] = Xreg->work[TANE_FIRSTPOWER] - 1;
		Xreg->work[TANE_FIRSTPOWER] -= 1;
		Xreg->work[TANE_BOUND]++;
		if(Xreg->work[TANE_BOUND]>3){
			DelActor(Xreg);
		}else{
			SePlay( SE_TB_KARA);
		}
	}
	Xreg->x = Xreg->work[TANE_MOVEX];
	Xreg->y = Xreg->work[TANE_MOVEY];
}


//===============================================================================
//  �o�E���h���邽�˂̃Z�b�g�֐�
//===============================================================================
static void SetBoundSeedActMove(actWork *p,s16 x, s16 y, s16 power, s16 muki,s16 muki2)
{
	p->work[TANE_HOZONY] = y;
	p->work[TANE_MOVEX] = x;
	p->work[TANE_MOVEY] = y;
	p->work[TANE_FIRSTPOWER] = power;
	p->work[TANE_POWER] = 10;
	p->work[TANE_BOUND]	= 0;
	p->work[TANE_MUKI] = muki;
	p->work[TANE_MUKI2] = muki2;
	p->move = BoundSeedActMove;
}

static const s16 BoundActTbl[][5]={
	{-10, 20, 10, 2, 1},
	{250, 20, 10,-2, 1},
	{-10,140, 10, 2,-1},
	{250,140, 10,-2,-1},
};


//=========================================================================
//	�o�E���h���邽�˂�o�^
//=========================================================================
static void SetBoundActor(u16 tane, u8 no)
{
	u8 work;
	
	work = SetTagSeedActEx( tane - ITEM_SEED_START,0,80,no%2);
	SetBoundSeedActMove(&ActWork[work],BoundActTbl[no][0], BoundActTbl[no][1],BoundActTbl[no][2], 
		BoundActTbl[no][3],BoundActTbl[no][4]);
}

// =================================================================
// KinomiWorkSet
// �T�v  : ���̂ݑ��M�p���[�N���Z�b�g
// ����  : kp
//       : Item
// �߂�l: none
// =================================================================
static void KinomiWorkSet(KINOMIWORK *kp, u16 Item)
{
	const SEED_PARAM	*sp;

	sp = GetSeedParam(Item - ITEM_SEED_START+1);
	kp->no = Item;					//�A�C�e���ԍ�

	PM_strcpy(kp->name, sp->name);	//���O

	kp->para[0] = sp->karai;			//�h��
	kp->para[1] = sp->sibui;			//�a��
	kp->para[2] = sp->amai;				//�Â�
	kp->para[3] = sp->nigai;			//�ꂳ
	kp->para[4] = sp->suppai;			//�_���ς�
	kp->para[5] = sp->umai;				//�|��
	
}


// =================================================================
// BlendMachineTypeSet
// �T�v  : �^�l�u�����_�[�̒ʐM�E��l�p�̐ݒ���s���܂�
// ����  : type
// �߂�l: none
// =================================================================
void BlendMachineTypeSet(u8 type)
{
	switch(type){
		case PLAYER_SIO:
			BlendMachineType = PLAYER_SIO;
			break;
		case PLAYER_2:
			BlendMachineType = PLAYER_2;
			bmw->member_num = 2;
			PM_strcpy(SioTrainerInfo[0].name,MyData.my_name);
			PM_strcpy(SioTrainerInfo[1].name,npc_name[0]);
			break;

		case PLAYER_3:
			BlendMachineType = PLAYER_3;
			bmw->member_num = 3;
			PM_strcpy(SioTrainerInfo[0].name,MyData.my_name);
			PM_strcpy(SioTrainerInfo[1].name,npc_name[0]);
			PM_strcpy(SioTrainerInfo[2].name,npc_name[1]);
			break;
		case PLAYER_4:
			BlendMachineType = PLAYER_4;
			bmw->member_num = 4;
			PM_strcpy(SioTrainerInfo[0].name,MyData.my_name);
			PM_strcpy(SioTrainerInfo[1].name,npc_name[0]);
			PM_strcpy(SioTrainerInfo[2].name,npc_name[1]);
			PM_strcpy(SioTrainerInfo[3].name,npc_name[2]);
//			BlendMachineMemberItem(0, ITEM_SEED_START);
			break;
	}
}


//------------------------------------------------------------------
//  �u�����h�}�V�[���������֐�
//------------------------------------------------------------------
void BlendMachineInit()
{
	int i;
	*(vu16*)REG_DISPCNT = 0;
	bmw = (BLENDMACHINE_WORK*)&UserWork[0x18000];		//�߲���𓖂Ă�����
	bmw->initflag = 0;
	bmw->end_watch = 0;
#if SEEQUENCE_DEBUG
	bmw->seqlognum = 0;
#endif
	
	for(i=0;i<4;i++) bmw->memberitem[i] = 0;
	BlendMachineTypeSet(ScriptParameter0);
	

	if(ScriptParameter0==PLAYER_SIO)	MainProcChange(BlendMachineInit_Sio);		//�ʐM�p
	else								MainProcChange(BlendMachineInit_Offline);		//��l�p�i�Q�C�R�C�S�j
}

enum{
	BLEND_INIT=0,
	BLEND_GRAPHIC_INIT,
	BLEND_ADD_ACTOR,
	BLEND_FADEIN_START,
	BLEND_FADEIN_WAIT,
	BLEND_SIO_INIT,
	BLEND_SIO_START_WAIT,
	BLEND_SIO_START,
	BLEND_SIO_WAIT,
	BLEND_SIO_SEND,
	BLEND_SIO_RECV,
	BLEND_TANE_BOUND,
	BLEND_TANE_BOUND_WAIT,
	BLEND_TOP_CHECK,
	BLEND_SET_DISK,
	BLEND_VIBRATION,
	BLEND_COUNTDOWN_START,
	BLEND_COUNTDOWN,
	BLEND_START,
	BLEND_ITEM_RECV,
	BLEND_TEXT_INIT,
	BLEND_GAMEN_INIT,
};

//------------------------------------------------------------------
//  ���������̏���(�}���`�v���C���[��)
//------------------------------------------------------------------
static void BlendMachineInit_Sio()
{
	int i,w;
	
	switch(bmw->initflag){
		case BLEND_INIT:
			InitActor();				//�A�N�^�[������
			ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
			InitTask();
			
			SetVBlankFunc(VInterFunc);

			PrintScreenInit(BLENDMACHINE_TYPE);
			MsgControlInit(&(bmw->tw), BLENDMACHINE_TYPE);
			bmw->windowwork = NWinFrameInit(256);
			NWinFrameDataSet(&(bmw->tw));
//			PaletteWorkSet(Test_Palette,(bmw->tw.context->BasePalette)*16,32);
			NMenuScreenInitEx2(BLENDMACHINE_TYPE,1);			
			PokemonSioNinshouNo = NINSHOU_BLEND_GAME;
			bmw->initflag++;
			bmw->endwait = 0;
			bmw->down_count = 0;
			bmw->BgOffsetX = 0;			//��ʗh��l�̕ϐ�X
			bmw->BgOffsetY = 0;			//		�h		  Y
			for(i=0;i<4;i++){
				bmw->siowork[i]    = 0;
				for(w=0;w<3;w++) 
					bmw->hitcount[i][w] = 0;
			}
			bmw->selectwork = 0;
//			PokemonMultiSioEnd();


			//--------��]�ʁE�w�i�p�f�[�^�Z�b�g--------------

			bmw->v_kakudo = 0;
			bmw->kakudo   = 0;
			bmw->bpm_max = 0;
			bmw->graphic_init = 0;

//			DebugNumPrintInit(0,1,12,2);
			break;
		case BLEND_GRAPHIC_INIT:
			if(BlendMachineGraphicSet()){
				bmw->initflag++;	//�摜�]��
				rotate_func();
			}
			break;
		case BLEND_ADD_ACTOR:		//�X�C�b�`�̓����ĂȂ��J�[�\���z�u
			for(i=0;i<4;i++){
					bmw->cursorwork[i] = AddActor(&CursorHeader,
						arrowpos[i][0],arrowpos[i][1],1);
					ActAnmChg(&ActWork[bmw->cursorwork[i]],8+i);
			}
			bmw->initflag++;
			break;

		case BLEND_FADEIN_START:
			PaletteFadeReq(0xffffffff,0,16,0,0);			//�t�F�[�h�C��
			bmw->initflag++;
			break;
		case BLEND_FADEIN_WAIT:
			BlendMachineGraphicRegSet();	//���ڼ޽��ݒ�
			if( FadeData.fade_sw == 0 ){								//�t�B�[���h�֕��A
				bmw->initflag++;
			}
			break;

		case BLEND_SIO_INIT:			//���Ƃ͂����ŒʐM�ڑ����Ă܂������A��߂܂����B
										//���݂͎�t���炸���Ɛڑ������ςȂ��ł��B
			BlendWindMsgPrint(taiki_msg,WINMSG_CHR_START_NO,0,13,29,19);
			bmw->initflag = BLEND_SIO_WAIT;
			bmw->wait = 0;
			break;
		case BLEND_SIO_WAIT:
			bmw->initflag++;
			bmw->boundnum = 0;
			KinomiWorkSet(&(bmw->Kinomi[0]),ReturnItemNo);
			memcpy(SendPoolBuf,&(bmw->Kinomi[0]),sizeof(KINOMIWORK));
			
			SetSioSynchronize();				//�ʐM�����J�n
			bmw->wait = 0;
			break;
		case BLEND_SIO_SEND:
				if(SplitDataSendStatus()){
					SplitDataResultReset();
					if(GetSioConnectId()==0){				//�e�̓f�[�^���M
						SplitDataExchangeStart(4);			//40�o�C�g����
					}
					bmw->initflag++;
				}
			break;
		case BLEND_SIO_RECV:
			if(++bmw->wait > 20){
				NMenuScreenClear();
				if(SplitDataResult()==GetConnectMemberBitData()){
					for(i=0;i<GetConnectSioMenberNum();i++){
						memcpy(&(bmw->Kinomi[i]), RecvPoolBuf[i],sizeof(KINOMIWORK));
						bmw->memberitem[i] = bmw->Kinomi[i].no;
					}
					SplitDataResultReset();
					bmw->initflag++;
				}
			}
			break;
		case BLEND_TANE_BOUND:
			bmw->member_num = GetConnectSioMenberNum();
			for(i=0;i<4;i++){
				if(bmw->boundnum==table_table[bmw->member_num-2][i]){
					SetBoundActor(bmw->memberitem[bmw->boundnum],i);
					break;
				}
			}
			bmw->wait = 0;
			bmw->initflag++;
			bmw->boundnum++;
			break;
		case BLEND_TANE_BOUND_WAIT:
			if(++bmw->wait>60){
				if(bmw->boundnum>=bmw->member_num){
					bmw->initflag++;
					bmw->kakudo = start_point[start_tbl[bmw->member_num-2]]-ROTATE_DEMO_SPEED*ROTATE_DEMO_VSPEED;
				}else{
					bmw->initflag--;
				}
				bmw->wait = 0;
			}
			break;
		case BLEND_TOP_CHECK:
			if(SplitDataSendStatus()){
				bmw->initflag++;
				BlendBGAffineSet(&bmw->Rotate);				//��]�v�Z
			}
			break;
		case BLEND_SET_DISK:
			*(vu16 *)REG_DISPCNT |= DISP_BG2_ON;			// BG2�L��
			bmw->kakudo+=ROTATE_DEMO_VSPEED;
			bmw->Ratio+=ROTATE_DEMO_STEP;
			if(bmw->Ratio>=ROTATE_FIRST_RATIO){
				bmw->initflag++;
				bmw->Ratio=ROTATE_FIRST_RATIO;
				bmw->kakudo = start_point[start_tbl[bmw->member_num-2]];
				//��]�ʐݒ�
				*(vu16 *)REG_BG2CNT = BG_COLOR_256			// 256�F�I��
						| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					// BG�D�揇��
						| ROTATE_MAP << BG_SCREEN_BASE_SHIFT		// �X�N���[���x�[�X�u���b�N
						| ROTATE_CHARA << BG_CHAR_BASE_SHIFT 		// �L�����N�^�x�[�X�u���b�N
						| BG_LOOP_OFF
						;
				bmw->wait = 0;
				PositionTableMake();
				BlendMachineNamePrint();
			}
			BlendBGAffineSet(&bmw->Rotate);				//��]�v�Z
			break;
		case BLEND_VIBRATION:
			if(BlendMachineVibration()){
				bmw->wait = 0;
				bmw->initflag++;
			}
			BlendBGAffineSet(&bmw->Rotate);				//��]�v�Z
			break;

		case BLEND_COUNTDOWN_START:			//�J�E���g�_�E���X�^�[�g
				AddActor(&ActCountHeader,120,-16,3);
				bmw->initflag++;
			break;

		case BLEND_COUNTDOWN:				//ActCountMove���Ńv���X���Ă��炤
			
//			bmw->initflag++;
			break;
		case BLEND_START:
			bmw->initflag++;
			break;
		case BLEND_ITEM_RECV:
			
			SetSioSynchronize();				//�ʐM�����J�n
			bmw->initflag++;
			break;
		case BLEND_TEXT_INIT:
			if(SplitDataSendStatus()){
				BlendMachineFunction_Set();
				bmw->initflag++;
			}
			break;
		case BLEND_GAMEN_INIT:

			bmw->v_kakudo=V_KAKUDO_MIN;
			bmw->timer = 0;

			MainProcChange(BlendMachineMain);
			if(BGM_GetNowNo()!=MUS_CYCLING)
				bmw->nowsong = BGM_GetNowNo();
			MusicPlay(MUS_CYCLING);
			break;
		case 100:
			NMenuWinBoxWrite(0,13,29,19);
			BlendTalkMsgSet( sioend_msg, WINMSG_CHR_START_NO, 2, 15);
			bmw->initflag++;
			break;
		case 101:
			if(BlendTalkMsgPut()){
				bmw->initflag++;
//				PaletteFadeReq(u32 fade_bit, s8 wait, u8 start_evy, u8 end_evy, u16 next_rgb);
//				LightFadeReq(N_to_B);										//�t�F�[�h�A�E�g
//				m4aMPlayAllContinue();
			}
			break;
		case 102:
			if( FadeData.fade_sw == 0 ){								//�t�B�[���h�֕��A
//				MusicPlay(bmw->nowsong);
				MainProcChange(FieldScriptRecover);
			}
			break;
	}

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
}


static void InitBlendMachineGraphic()
{
			*(vu16 *)REG_DISPCNT = 	0;		// BG OFF 
			InitActor();				//�A�N�^�[������
			ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
			InitTask();
			
			SetVBlankFunc(VInterFunc);

			PrintScreenInit(BLENDMACHINE_TYPE);
			MsgControlInit(&(bmw->tw), BLENDMACHINE_TYPE);
			bmw->windowwork = NWinFrameInit(256);
			NWinFrameDataSet(&(bmw->tw));
			NMenuScreenInitEx2(BLENDMACHINE_TYPE,1);			
			PokemonSioNinshouNo = NINSHOU_BLEND_GAME;
			bmw->endwait = 0;

			//�w�i�摜�͏d���̂ŕʏꏊ�Ɉړ�

			bmw->v_kakudo = 0;
			bmw->kakudo   = 0;
			bmw->bpm_max = 0;
			bmw->BgOffsetX = 0;			//��ʗh��l�̕ϐ�X
			bmw->BgOffsetY = 0;			//		�h		  Y

//			DebugNumPrintInit(0,1,12,2);
			bmw->initflag++;

}


#define HIT_RANGE		(48)
#define HIT_RANGE_HALF	(HIT_RANGE/2)
#define JUST_RANGE		(8)
#define JUST_START		(HIT_RANGE_HALF-(JUST_RANGE/2))
#define JUST_END		(JUST_START+JUST_RANGE)
// =================================================================
// hit_check
// �T�v  : �z�C�[���̂����蔻����s��
// ����  : kakudo=�z�C�[���̊p�x	id=�ʒu
// �߂�l: static u8:	0=�Ȃ�	1=������	2=�W���X�g������
// =================================================================
static u8 hit_check(u16 kakudo, u8 id)
{
	u8  pos;
	u32 hantei_kakudo;
//			hantei_kakudo = bmw->kakudo+0x1800;
		hantei_kakudo = (kakudo>>8)+HIT_RANGE_HALF;
		pos = bmw->reverse[id];
		if(hantei_kakudo >= hit_table[pos] && hantei_kakudo < (hit_table[pos]+HIT_RANGE)){	//�΂�����
			if(hantei_kakudo >= (hit_table[pos]+JUST_START) 
			&& hantei_kakudo< (hit_table[pos]+JUST_END)){		//���΂�����I
						return 2;
			}else{											//������
						return 1;
			}
		}													//�_��
		return 0;
}

//NPC������邫�݂̂̂��߂̃e�[�u��
static const u8 NpcItemTable[][3]={
	{4,3,2},
	{0,4,3},
	{1,0,4},
	{2,1,0},
	{3,2,1},
	{0,2,3},
	{1,3,4},
	{2,4,0},
	{3,0,1},
	{4,1,2},
};

// =================================================================
// NPC_Item_Set
// �T�v  : NPC�����݂̂����鏈��
// ����  : MyItem
// �߂�l: none
// =================================================================
static void NPC_ItemSet(u16 MyItem, u8 num, KINOMIWORK *kk)
{
	u16 i,work;

	work = 0;
	if(MyItem==ITEM_NAZONOMI){			//�������݂̂��H
		//�������̂�
		for(i=0;i<5;i++)
			if(kk->para[work] > kk->para[i]) work = i;
		work += 5;
	}else{
		//�ʏ킫�̂�
		work = MyItem-ITEM_SEED_START;	//�v���C���[���I���������݂̂��L�[�ɂ���
		if(work >= 5){
			work = (work%5) + 5;		//0-4�܂ł̓e�[�u���ʂ�
		}								//5�ȏ�͊������]��+5���e�[�u����������Ă���	
	}

	for(i=0;i<num-1;i++){			//�R���s���[�^�̑I����ʒm
		BlendMachineMemberItem(i+1, ITEM_SEED_START + NpcItemTable[work][i]);
	}

}


// =================================================================
// PositionTableMake
// �T�v  : �z�C�[���Ƃ̈ʒu�֌W��\���e�[�u�����쐬
// ����  : none
// �߂�l: none
// =================================================================
static void PositionTableMake()
{
	int i,w;
	
	for(i=0;i<4;i++){					//ID�̓e�[�u���z�u�I�ɉ��ԂɂȂ邩�B
		bmw->reverse[i] = 0xff;
		bmw->table[i] = table_table[bmw->member_num-2][i];
	}
	for(w=0;w<4;w++){					//ID����e�[�u���z�u�̋t�����e�[�u���쐬
		for(i=0;i<4;i++){
			if(bmw->table[i]==w){
				bmw->reverse[w] = i;
			}
		}
	}

}

// =================================================================
// BlendMachineNamePrint
// �T�v  : ���̂݃u�����_�[�ɎQ�����Ă�l�̖��O��\��
// ����  : none
// �߂�l: none
// =================================================================
static void BlendMachineNamePrint()
{
	int i;
	u8 strbuf[20],flag;
	for(i=0;i<4;i++){
		if(bmw->table[i]!=0xff){
			bmw->cursoractno[bmw->table[i]]=bmw->cursorwork[i];
			ActAnmChg(&ActWork[bmw->cursoractno[bmw->table[i]]],i);
			strbuf[0] = EOM_;
			if(GetSioConnectId()==bmw->table[i])	PM_strcpy(strbuf,colorchange_msg);	//�����̏ꍇ�͐F��ς���
			PM_strcat(strbuf,SioTrainerInfo[bmw->table[i]].name);
			flag = 0;
			if(PM_strlen(SioTrainerInfo[bmw->table[i]].name)>5) flag=1;		//�C�O�Ή�
			BlendMsgPrint( strbuf,NAME_CHR_START_NO+i*NAME_CHR_SIZE,
				namepos[flag][i][0],namepos[flag][i][1]);
		}
	}

}
//------------------------------------------------------------------
//  ���������̏���(��l�p)
//------------------------------------------------------------------
static void BlendMachineInit_Offline()
{
	int i,w;
	switch(bmw->initflag){
		case BLEND_INIT:
			InitBlendMachineGraphic();
			BlendMachineMemberItem(0, ReturnItemNo);		//�����őI���������݂̂��i�[
			KinomiWorkSet(&(bmw->Kinomi[0]),ReturnItemNo);	//���݂̂̃p�����[�^���擾
			NPC_ItemSet(ReturnItemNo, bmw->member_num,&(bmw->Kinomi[0]));
			for(i=0;i<4;i++){
				bmw->siowork[i]    = 0;
				for(w=0;w<3;w++) 
					bmw->hitcount[i][w] = 0;
			}
			bmw->selectwork = 0;
			bmw->graphic_init = 0;

			break;
		case BLEND_GRAPHIC_INIT:
			if(BlendMachineGraphicSet()){
				bmw->initflag++;	//�摜�]��
				rotate_func();
			}
			break;

		case BLEND_ADD_ACTOR:		//�X�C�b�`�̓����ĂȂ��J�[�\���z�u
			for(i=0;i<4;i++){
					bmw->cursorwork[i] = AddActor(&CursorHeader,
						arrowpos[i][0],arrowpos[i][1],1);
					ActAnmChg(&ActWork[bmw->cursorwork[i]],8+i);
			}
			bmw->initflag++;
			break;
		case BLEND_FADEIN_START:
			PaletteFadeReq(0xffffffff,0,16,0,0);			//�t�F�[�h�C��
			bmw->initflag++;
			bmw->wait = 0;
			break;


		case BLEND_FADEIN_WAIT:
			bmw->wait++;
			if(bmw->wait==2){
				BlendMachineGraphicRegSet();	//���ڼ޽��ݒ�
			}
			if( FadeData.fade_sw == 0 ){								//�t�B�[���h�֕��A
				bmw->initflag = BLEND_SIO_WAIT;
			}
			break;

		case BLEND_SIO_WAIT:
				bmw->initflag = BLEND_TANE_BOUND;
				bmw->boundnum = 0;
			break;
		case BLEND_TANE_BOUND:
			for(i=0;i<4;i++){
				w = table_table[bmw->member_num-2][i];
				if(bmw->boundnum==w){
//					SetBoundActor(ITEM_SEED_START+bmw->memberitem[i],i);
					SetBoundActor(bmw->memberitem[bmw->boundnum],i);
					break;
				}
			}
			bmw->wait = 0;
			bmw->initflag++;
			bmw->boundnum++;
			break;
		case BLEND_TANE_BOUND_WAIT:
			if(++bmw->wait>60){
				if(bmw->boundnum>=bmw->member_num){
					bmw->kakudo = start_point[start_tbl[bmw->member_num-2]]-ROTATE_DEMO_SPEED*ROTATE_DEMO_VSPEED;
					bmw->initflag++;
//					SetSioSynchronize();
				}else{
					bmw->initflag--;
				}
				bmw->wait = 0;
			}
			break;
		case BLEND_TOP_CHECK:
			bmw->initflag++;
			PositionTableMake();
			SePlay(SE_RU_HYUU);
			BlendBGAffineSet(&bmw->Rotate);				//��]�v�Z
			break;
		case BLEND_SET_DISK:							//��]���Ȃ���z�C�[���o��
			*(vu16 *)REG_DISPCNT |= DISP_BG2_ON;			// BG2�L��
			bmw->kakudo+=ROTATE_DEMO_VSPEED;
			bmw->Ratio+=ROTATE_DEMO_STEP;
			if(bmw->Ratio>=ROTATE_FIRST_RATIO){
				bmw->initflag++;
				bmw->Ratio=ROTATE_FIRST_RATIO;
			 	bmw->kakudo = start_point[start_tbl[bmw->member_num-2]];
				//��]�ʐݒ�
				*(vu16 *)REG_BG2CNT = BG_COLOR_256			// 256�F�I��
						| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					// BG�D�揇��
						| ROTATE_MAP << BG_SCREEN_BASE_SHIFT		// �X�N���[���x�[�X�u���b�N
						| ROTATE_CHARA << BG_CHAR_BASE_SHIFT 		// �L�����N�^�x�[�X�u���b�N
						| BG_LOOP_OFF
						;
				bmw->wait = 0;
				SePlay(SE_TRACK_DOOR);
				BlendMachineNamePrint();
			}
			BlendBGAffineSet(&bmw->Rotate);				//��]�v�Z
			break;
		case BLEND_VIBRATION:
			if(BlendMachineVibration()){
				bmw->initflag++;
			}
			BlendBGAffineSet(&bmw->Rotate);				//��]�v�Z
			break;
		case BLEND_COUNTDOWN_START:			//�J�E���g�_�E���X�^�[�g
				AddActor(&ActCountHeader,120,-16,3);
				bmw->initflag++;
			break;

		case BLEND_COUNTDOWN:				//ActCountMove���Ńv���X���Ă��炤
			
			break;
		case BLEND_START:
			bmw->initflag++;
			break;
		case BLEND_ITEM_RECV:
				bmw->initflag++;
			break;
		case BLEND_TEXT_INIT:

//			FontScrnAreaClear(&bmw->tw, 0, 0, 30, 20);
			bmw->initflag++;
			break;
		case BLEND_GAMEN_INIT:

			OfflineSendRecvBufInit();
			bmw->v_kakudo=V_KAKUDO_MIN;
			bmw->timer = 0;
			bmw->NPC_flag = 0;
			bmw->down_count = 0;
		

			MainProcChange(BlendMachineMain);					//��l�p�̏ꍇ��NPC��u��
			for(i=0;i<ScriptParameter0;i++)
				bmw->NPC_taskno[i] = AddTask(NPC_functable[i],10+i);	//NPC������o�^
			if(BGM_GetNowNo()!=MUS_CYCLING)
				bmw->nowsong = BGM_GetNowNo();
			MusicPlay(MUS_CYCLING);			//BGM
			SePlay(SE_MOTER);				//���[�^�[��
			motor_pitch();					//�s�b�`����
			break;
	}

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
}

// =================================================================
// OfflineSendRecvBufInit
// �T�v  : ��M�o�b�t�@�̃N���A�i��l�p�̎��̂݁j
// ����  : none
// �߂�l: none
// =================================================================
static void OfflineSendRecvBufInit()
{
	int i;
	for(i=0;i<4;i++){						//����R�}���h�̏�����
		SendBuf[0] = 0;
		SendBuf[2] = 0;
		RecvBuf[0][i] = 0;
		RecvBuf[2][i] = 0;
	}
}

// =================================================================
// CommandBooTask
// �T�v  : ���V���N����Ă���uBOO�v�R�}���h�𔭍s����
// ����  : no
// �߂�l: none
// =================================================================
static void CommandBooTask(u8 no)
{
	if(++TaskTable[no].work[0]>TaskTable[no].work[1]){
		RecvBuf[2][TaskTable[no].work[2]] = COMMAND_BOO;
		DelTask(no);
	}
}

// =================================================================
// CommandBooSend
// �T�v  : 
// ����  : id
//       : late
// �߂�l: none
// =================================================================
static void CommandBooSend(u8 id, u8 late)
{
	u8 no;
	no = AddTask(CommandBooTask,80);
	TaskTable[no].work[1] = late;
	TaskTable[no].work[2] = id;
}

// =================================================================
// NPC_hantei0
// �T�v  : �m���v���C���[�L�����N�^�[�̔���^�X�N�i��������j
// ����  : none
// �߂�l: none
// =================================================================
static void NPC_hantei0(u8 no)
{
	u8 result,r;

		result=hit_check(bmw->kakudo,1);
		if(result==2){
			if(TaskTable[no].work[0]==0){
				if(bmw->NPC_flag==0){					//�f�o�b�O��������
					r = pp_rand()/(RAND_MAX/100);
					if(bmw->v_kakudo < V_KAKUDO_MAX/3){			//���Ղ͕K����
						if(r>75){
							RecvBuf[2][1] = COMMAND_JUST;
						}else if(r<=75){
							RecvBuf[2][1] = COMMAND_OK;
						}
						RecvBuf[2][1] = COMMAND_OK;
					}else if(bmw->v_kakudo<V_KAKUDO_MAX){		//���E���x�܂�
						if(r>80){
							RecvBuf[2][1] = COMMAND_JUST;
						}else if(r<=80 && r>20){
							RecvBuf[2][1] = COMMAND_OK;
						}else if(r<10){
							CommandBooSend(1, 5);
						}
					}else if(bmw->v_kakudo>=V_KAKUDO_MAX){		//���E���x�ȏ�
						if(r>90){
							RecvBuf[2][1] = COMMAND_JUST;
						}else if(r<=90&&r>70){
							RecvBuf[2][1] = COMMAND_OK;
						}else if(r<30){
							CommandBooSend(1, 5);
						}
					}
				}else{
					RecvBuf[2][1] = COMMAND_JUST;					//�f�o�b�O�t���O�����Ɩ���JUST
				}
				TaskTable[no].work[0] = 1;
			}
		}else{
			TaskTable[no].work[0] = 0;
		}
}

// =================================================================
// NPC_hantei1
// �T�v  : �Q�l�ڂ�NPC�̔����[����i���Ƃ��̂��j
// ����  : no
// �߂�l: none
// =================================================================
static void NPC_hantei1(u8 no)
{
	u8 i,w,r;
	u32 hantei_kakudo;

	hantei_kakudo = bmw->kakudo+0x1800;
	hantei_kakudo &= 0xffff;
	w = 2;
	i = bmw->reverse[w];
	if(w!=0xff){

		if(((hantei_kakudo)>>8) > hit_table[i]+20 && 
			((hantei_kakudo)>>8)<hit_table[i]+0x28){	//�΂�����
			if(	TaskTable[no].work[0]==0){
				if(bmw->NPC_flag==0){									//�f�o�b�O�p��
					r = pp_rand()/(RAND_MAX/100);
					if(bmw->v_kakudo < V_KAKUDO_MAX/3){
						if(r > 66){
							RecvBuf[2][2] = COMMAND_JUST;
						}else if(r <= 66){
							RecvBuf[2][2] = COMMAND_OK;
						}
					}else if(bmw->v_kakudo>=V_KAKUDO_MAX/3){			//���Ղ͂��Ȃ炸��
						if(r>65)	
							RecvBuf[2][2] = COMMAND_JUST;
						if(r<=65 && r>40)								
							RecvBuf[2][2] = COMMAND_OK;	
						if(r<10)
							CommandBooSend(2, 5);
					}
					TaskTable[no].work[0] = 1;
				}else{														//�t���O������
					RecvBuf[2][2] = COMMAND_JUST;							//����W���X�g��
					TaskTable[no].work[0] = 1;
				}
			}
		}else{
			TaskTable[no].work[0] = 0;
		}
	}
	
}

// =================================================================
// NPC_hantei2
// �T�v  : 3�l�ڂ�NPC�L�����N�^�[�̔��胋�[�`���i����Ȃ̂��j
// ����  : no
// �߂�l: none
// =================================================================
static void NPC_hantei2(u8 no)
{
	u8 i,w,r;
	u32 hantei_kakudo;

	hantei_kakudo = bmw->kakudo+0x1800;
	hantei_kakudo &= 0xffff;
	w = 3;
	i = bmw->reverse[w];
	if(w!=0xff){

		if(((hantei_kakudo)>>8) > hit_table[i]+20 && 
			((hantei_kakudo)>>8)<hit_table[i]+0x28){	//�΂�����
			if(	TaskTable[no].work[0]==0){
				if(	bmw->NPC_flag==0){							//�f�o�b�O�p
					r = pp_rand()/(RAND_MAX/100);
					if(bmw->v_kakudo < V_KAKUDO_MAX/3){
						if(r > 88){
							RecvBuf[2][3] = COMMAND_JUST;
						}else if(r <= 88){
							RecvBuf[2][3] = COMMAND_OK;
						}
					}else if(bmw->v_kakudo>=V_KAKUDO_MAX/3){	//���Ղ͂��Ȃ炸��
						if(r>60){
							RecvBuf[2][3] = COMMAND_JUST;
						}else if(r<=60 && r>55){
							RecvBuf[2][3] = COMMAND_OK;
						}
						if(r<5)
							CommandBooSend(3, 5);
					}
					TaskTable[no].work[0] = 1;
				}else{														//�f�o�b�O�t���O������
					RecvBuf[2][3] = COMMAND_JUST;							//����W���X�g
					TaskTable[no].work[0] = 1;
				}
			}
		}else{
			TaskTable[no].work[0] = 0;
		}
	}
	
}

// =================================================================
// HanteiActorCall
// �T�v  : ����A�N�^�[��\���E�r�d�����E�Ήԕ\��
// ����  : hantei	���茋��
//       : id		�����ʒu
// �߂�l: none
// =================================================================
static void HanteiActorCall(u16 hantei, u8 id)
{
	u8 no;

	no=AddActor(&HanteiHeader,								//����}�[�N������
		arrowpos[id][0]-arrowmove[id][0]*10,arrowpos[id][1]-arrowmove[id][1]*10,1);
	if(hantei==COMMAND_JUST){
		ActAnmChg(&ActWork[no],2);
		ActWork[no].move = HanteiMove2;
		SePlay(SE_RU_GASHIN);
	}else if(hantei==COMMAND_OK){
		ActAnmChg(&ActWork[no],0);
		SePlay(SE_SEIKAI);
	}else if(hantei==COMMAND_BOO){
		ActAnmChg(&ActWork[no],1);
		SePlay(SE_HAZURE);
	}
	hibana_actor_add();
}

static const u8 BlendDivTbl[]={1,1,2,3,4};
// =================================================================
// BlenderSpeedControl
// �T�v  : �z�C�[���̃X�s�[�h���グ��������
// ����  : command
// �߂�l: none
// =================================================================
static void BlenderSpeedControl(u16 command)
{
	motor_pitch();
	switch(command){
		case COMMAND_JUST:
			if(bmw->v_kakudo<V_KAKUDO_MAX){							//���E�X�s�[�h���H
				bmw->v_kakudo+=PLUS_JUST_V_KAKUDO/BlendDivTbl[bmw->member_num];	//�܂�
			}else{												
				bmw->v_kakudo+=PLUS_OVER_MAX/BlendDivTbl[bmw->member_num];		//�������E�����E�z���Ă�
				SpeedUpVibrationSet(&bmw->BgOffsetX,(bmw->v_kakudo/100)-10);	
				SpeedUpVibrationSet(&bmw->BgOffsetY,(bmw->v_kakudo/100)-10);	
			}
			break;
		case COMMAND_OK:
			if(bmw->v_kakudo<V_KAKUDO_MAX)
				bmw->v_kakudo+=PLUS_V_KAKUDO/BlendDivTbl[bmw->member_num];//�l���������Ɖ����x���������ĉ�������
//			if(bmw->end_meter>=ENDMETER_MAX){
//				bmw->end_meter = ENDMETER_MAX;
//			}
			break;
		case COMMAND_BOO:
			bmw->v_kakudo-=MINUS_V_KAKUDO/BlendDivTbl[bmw->member_num];
			if(bmw->v_kakudo<V_KAKUDO_MIN)
				bmw->v_kakudo = V_KAKUDO_MIN;
			break;
	}
}

// ==========================================================================
// HanteiCheck
// �T�v  : ���茋�ʂ�ʐM�Ŏ󂯎���ď�������
// ����  : none
// �߂�l: static void
// ==========================================================================
static void HanteiCheck()
{

	int i,w,work;
	
	if(ScriptParameter0)									//��l�p�̏ꍇ��SendBuf��RecvBuf�𗘗p����
	{
		if(SendBuf[2]!=INVALID_DATA){						//SendBuf�̓��e��RecvBuf�Ɉڂ��Ă��
			RecvBuf[2][0] = SendBuf[2];
			RecvBuf[0][0] = SEND_KEYDATA;
			SendBuf[2]    = INVALID_DATA;
		}
		for(i=1;i<4;i++){
			if(RecvBuf[2][i] != INVALID_DATA)
				RecvBuf[0][i] = SEND_KEYDATA;
		}
	}
	
	
	for(i=0;i<bmw->member_num;i++){
		if(RecvBuf[0][i]==SEND_KEYDATA){
			w = bmw->reverse[i];
			if(RecvBuf[2][i]==COMMAND_JUST){				//�W���X�g����
				BlenderSpeedControl(COMMAND_JUST);
				bmw->end_meter += bmw->v_kakudo/JUST_PLUS_DIV;			//���[�^�[��i�߂�
				if(bmw->end_meter>=ENDMETER_MAX){
					bmw->end_meter = ENDMETER_MAX;
				}
				HanteiActorCall(COMMAND_JUST, w);						//�A�N�^�[�\��
				bmw->hitcount[i][0]++;
			}else if(RecvBuf[2][i]==COMMAND_OK){			//��]�����R�}���h���͂���
				BlenderSpeedControl(COMMAND_OK);
				bmw->end_meter += bmw->v_kakudo/PLUS_DIV;
				HanteiActorCall(COMMAND_OK, w);						//�A�N�^�[�\��
				bmw->hitcount[i][1]++;
			}else if (RecvBuf[2][i]==COMMAND_BOO){			//��]���s�R�}���h���͂���
				HanteiActorCall(COMMAND_BOO, w);					//�A�N�^�[�\��
				BlenderSpeedControl(COMMAND_BOO);
				if(bmw->hitcount[i][2]<999)		bmw->hitcount[i][2]++;
			}
			if(RecvBuf[2][i]==COMMAND_BOO||RecvBuf[2][i]==COMMAND_JUST||RecvBuf[2][i]==COMMAND_OK){
				if(bmw->v_kakudo>V_KAKUDO_MAX){
					work = (bmw->v_kakudo-(V_KAKUDO_MAX/2))/20+256;
					m4aMPlayTempoControl(&m4a_mplay000, work);	
				}else{
					m4aMPlayTempoControl(&m4a_mplay000, 256);	
				}
			
			}
		}
	}

	
	if(ScriptParameter0){
		for(i=0;i<bmw->member_num;i++){						//����R�}���h�̏�����
			RecvBuf[0][i] = 0;
			RecvBuf[2][i] = INVALID_DATA;
		}
	}

}

// =================================================================
// pad_control
// �T�v  : �^�l�u�����_�[�Q�[�����̃p�b�h����
// ����  : none
// �߂�l: none
// =================================================================
static void pad_control()
{
	u8 result,i,pad;
	pad = 0;
	i   = bmw->reverse[GetSioConnectId()];
	if(bmw->endflag==0){			//�I�����ĂȂ����
//			if(sys.Trg&A_BUTTON){		//�����̖��O�̋߂���A�������Ɖ�]���i

			if(MyData.key_config == CNT_MD_KATATE){
				if(sys.Trg&A_BUTTON && 
					((sys.RealCont & (A_BUTTON|L_BUTTON))!=(A_BUTTON|L_BUTTON))) pad = 1;
			}else{
				if(sys.Trg&A_BUTTON ) pad = 1;
			}

			if(pad){
				ActAnmChg(&ActWork[bmw->cursoractno[bmw->table[i]]],i+4);		//���̐F��ς���
				result=hit_check(bmw->kakudo,GetSioConnectId());
				if(result==2){
					SendBuf[2] = COMMAND_JUST;				//���΂�����I
				}else if(result==1){
					SendBuf[2] = COMMAND_OK;				//����
				}else{
					SendBuf[2] = COMMAND_BOO;				//�_��
				}
			}
//		if(sys.Trg&START_BUTTON){
//			bmw->endflag++;
//			MainProcChange(BlendMachineEnd);
//		}
	}
	if(++bmw->down_count>5){						//5sync�ɂ��������X�s�[�h��������
		if(bmw->v_kakudo>V_KAKUDO_MIN)
			bmw->v_kakudo--;
		bmw->down_count=0;
	}


#ifdef DEBUG_NPC_CHANGE
	if(DebugMainFlag){
		if(sys.Trg&L_BUTTON) bmw->NPC_flag^=1;
	}
#endif		

}


//------------------------------------------------------------------
//  �^�l�u�����_�[���C��
//------------------------------------------------------------------
void BlendMachineMain()
{
	rotate_func();									//��]�ʏ���

	if(bmw->timer < BLEND_TIME_MAX)
		bmw->timer++;								//��Ѷ���

	pad_control();									//�߯�ޏ���
	
	kanshi_num_set(bmw->v_kakudo, bmw->end_meter);	//���ޯ�ޗp
	HanteiCheck();									//�ʐM�ő��l�̔��茋�ʂ���M
	MeterFunc(bmw->end_meter,ENDMETER_MAX);			//���[�^�[����
	SpeedMeterFunc(bmw->v_kakudo);					//��߰��Ұ������
	SpeedUpVibration();								//�U���v�Z
	SioFunc();										//�ʐM�p����

	if(bmw->endflag==0){						//�I������
		if(bmw->now_meter>=ENDMETER_MAX){
			bmw->end_meter=ENDMETER_MAX;
			bmw->endflag=1;
			MainProcChange(BlendMachineEnd);
		}
	}


	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}



// =================================================================
// NazonomiHantei
// �T�v  : �i�]�݂̂̃p�����[�^���r���ē������ǂ����𔻒肷��
// ����  : mix	:���̂݃p�����[�^�̔z��
//       : k1	:1�ڂ̏ꏊ
//       : k2	:2�ڂ̏ꏊ
// �߂�l: static u8	0:�Ⴄ	1:����(=���낢�|���b�N)
// =================================================================
static u8 NazonomiHantei(KINOMIWORK	*mix , u8 k1, u8 k2)
{
	if(mix[k1].no != mix[k2].no) return 1;		//�����܂ŗ��ăA�C�e���ԍ����Ⴄ�̂͂��肦�Ȃ����ǔO�̂���
	
	if(PM_strcmp(mix[k1].name, mix[k2].name)!=0) return 0;
	if(mix[k1].para[0]==mix[k2].para[0]
	     &&mix[k1].para[1]==mix[k2].para[1]
	     &&mix[k1].para[2]==mix[k2].para[2]
	     &&mix[k1].para[3]==mix[k2].para[3]
	     &&mix[k1].para[4]==mix[k2].para[4]
	     &&mix[k1].para[5]==mix[k2].para[5])
	{
		return 1;
	}
	return 0;
}

#define CUBE_PARAM_MAX	(255)				//�|���b�N�̈�̃p�����[�^�̍ō��l
enum{
	DUMMY_CUBE=0,// 00:�_�~�[
	AKAI_CUBE,// 01:�������L���[�u
	AOI_CUBE,// 02:�������L���[�u
	MOMOIRO_CUBE,// 03:��������L���[�u
	MIDORI_CUBE,// 04:�݂ǂ�̃L���[�u
	KIIRO_CUBE,// 05:������̃L���[�u
	MURASAKI_CUBE,// 06:�ނ炳���L���[�u
	KONIRO_CUBE,// 07:���񂢂�L���[�u
	CHAIRO_CUBE,// 08:���Ⴂ��L���[�u
	SORAIRO_CUBE,// 09:���炢��L���[�u
	KIMIDORI_CUBE,// 10:���݂ǂ�L���[�u
	HAIIRO_CUBE,// 11:�͂�����L���[�u
	KURO_CUBE,// 12:���낢�L���[�u
	SIRO_CUBE,// 13:���낢�L���[�u
	KINIRO_CUBE,// 14:���񂢂�L���[�u

	
};


static s16 param[6],work[6], minusline, plusline;
// =========================================================================
// CalcCubeType
// �T�v  : �^�l�u�����_�[�Ɏg��ꂽ���˂̎�ނɂ���ăL���[�u�̎�ނ�����
// ����  : mix			���݂̂̔ԍ��̊i�[���ꂽ�z��
//       : table		�p�����[�^�̊i�[���ꂽ�z��
//       : num			���̂݃u�����_�[�ɎQ�������l��
//		 : minus		�}�C�i�X�J�E���g
// �߂�l: static int
// =========================================================================
static int CalcCubeType(KINOMIWORK	*mix, s16 *table,u8 num, u8 minuscount)
{
	int i,w;
	u8 plus;
	s16 param[6];

	for(i=0;i<6;i++)
		param[i] = table[i];


	//���낢�L���[�u�`�F�b�N						//�S���p�����[�^��0
	w = 0;
	for(i=0;i<5;i++){
		if(param[i]==0) w++;
	}
	if(w==5) return KURO_CUBE;
	if(minuscount>=4) return KURO_CUBE;				//�}�C�i�X�p����4�ӏ�
	
	for(i=0;i<num;i++){								
		for(w=0;w<num;w++){
			if(mix[i].no==mix[w].no && i!=w){	//�������˂��͂����Ă���
				if(mix[i].no==ITEM_NAZONOMI){			//�i�]�̎��̏ꍇ�͕ʏ���
					if(NazonomiHantei(mix,i,w)){
						return KURO_CUBE;			//�i�]�̎��̃p�����[�^������
					}else{
						continue;					//�i�]�̎��̒��g�͈����
					}
				}else{
					return KURO_CUBE;				//���낢�L���[�u
				}
			}
		}
	}
//	if(KuroiCubeCheck(mix,num)) return KURO_CUBE;	//���낢�L���[�u�`�F�b�N(���Ȃ����Ұ�����)

	
	plus  = 0;									//�͂�����L���[�u�E���낢�L���[�u�`�F�b�N
	for(i=0;i<5;i++){
		if(param[i]>0) plus++;					//�v���X�ɍs������ނ��J�E���g
	}
	if(plus>=4) return SIRO_CUBE;				//�v���X���S��ނ������炵�낢�L���[�u(�ȏ�ɂ��Ă�͔̂O�̂���)
	if(plus==3) return HAIIRO_CUBE;				//�v���X���R��ނ�������D�F�L���[�u

	//���񂢂�L���[�u�`�F�b�N
	for(i=0;i<5;i++){								//�p�����[�^�̂ǂꂩ���T�O���z���Ă�����
		if(param[i]>50) return KINIRO_CUBE;
	}


	//�v���X�l�����ނ̏ꍇ�̔���
	if(plus==1){
		if(param[0]>0) return AKAI_CUBE;		//�h���v���X�Ȃ�@�������L���[�u
		if(param[1]>0) return AOI_CUBE;			//�a���v���X�Ȃ�@�������L���[�u
		if(param[2]>0) return MOMOIRO_CUBE;		//�Â��v���X�Ȃ�@��������L���[�u
		if(param[3]>0) return MIDORI_CUBE;		//�ꂪ�v���X�Ȃ�@�݂ǂ�L���[�u
		if(param[4]>0) return KIIRO_CUBE;		//�_���v���X�Ȃ�@������L���[�u
	}
	
	//�v���X�l�����ނ̏ꍇ�̔���
	if(plus==2){
		w = 0;
		for(i=0;i<5;i++){			//�v���X�̓�̂����o���Ă݂�
			if(param[i]>0){
				work[w] = i;
				w++;
			}
		}
		if(param[work[0]]>=param[work[1]]){			//�O�҂̃p�����[�^�̕����傫�����ꏏ
			if(work[0]==0) return (MURASAKI_CUBE|(work[1]<<16));	//�ނ炳��
			if(work[0]==1) return (KONIRO_CUBE|(work[1]<<16));		//���񂢂�
			if(work[0]==2) return (CHAIRO_CUBE|(work[1]<<16));		//���Ⴂ��
			if(work[0]==3) return (SORAIRO_CUBE|(work[1]<<16));		//���炢��
			if(work[0]==4) return (KIMIDORI_CUBE|(work[1]<<16));	//���݂ǂ�
		}else{										//��҂̃p�����[�^�̕����傫��
			if(work[1]==0) return (MURASAKI_CUBE|(work[0]<<16));	//�ނ炳��
			if(work[1]==1) return (KONIRO_CUBE|(work[0]<<16));		//���񂢂�
			if(work[1]==2) return (CHAIRO_CUBE|(work[0]<<16));		//���Ⴂ��
			if(work[1]==3) return (SORAIRO_CUBE|(work[0]<<16));		//���炢��
			if(work[1]==4) return (KIMIDORI_CUBE|(work[0]<<16));	//���݂ǂ�
		}
	}
	return DUMMY_CUBE;												//�����ɂ͂��Ȃ��n�Y
}

void SetMinusLine(s16 num){
	minusline = num;
}

s16 GetMinusLine()
{
	return minusline;
}
void SetPlusLine(s16 num){
	plusline = num;
}

s16 GetPlusLine()
{
	return plusline;
}

#define FIVE_TO_THREE		(10)		//�T�̒�����R�I�ԏꍇ�̐�
static const u8 five2three[]={
				//  �����̃r�b�g�f�[�^��1�o�C�g�ŕ\��
	0x1c,0x16,0x13,0x1a,0x19,		//	11100	10110	10011	11010	11001
	0x0e,0x0d,0x0b,0x07,0x15,		//	01110	01101	01011	00111	10101
};


static s32 paramtest[5];
static s32 paramtest2[5];
static s32 paramwork;
// ================================================================================
// Seed2Cube
// �T�v  : ���˂ƃ^�l�u�����_�[�̌��ʂ���L���[�u���Z�o
// ����  : mix			�^�l�ԍ��̓������z��
//       : newcube		�L���[�u�f�[�^���i�[�����߲��
//       : num			���̂݃u�����_�[����ڲ�����l��
//       : calc			�p�����[�^�̌��ʂ��i�[���ĕԂ��z��̃|�C���^
// �߂�l: none
// ================================================================================
static void Seed2Cube(KINOMIWORK *mix, CUBE *newcube, u8 num, u8 *calc, u16 bpm)
{
	int i,type,w;
	s32 work,mod;
	s32 calcwork;
	u8  minuscount;
	
	for(i=0;i<6;i++) param[i] = 0;					//�p�����[�^������

	for(i=0;i<num;i++){								//�p�����[�^���v
		for(w=0;w<6;w++)
			param[w] += mix[i].para[w];

	}
	
	
										//���Z����
	work = param[0];					//			�h�ۑ�
	param[0] -= param[1];				//�h - �a
	param[1] -= param[2];				//�a - ��
	param[2] -= param[3];				//�� - ��
	param[3] -= param[4];				//�� - �_
	param[4] -= work;					//�_ - �h

	work = 0;							//�}�C�i�X�l�ɂȂ����p�����[�^�𐔂���
	for(i=0;i<5;i++){
		if(param[i]<0){
			param[i] = 0;					//�}�C�i�X�l��0��
			work++;
		}
	}
	minuscount = work;
										//�v���X�̒l���}�C�i�X�J�E���g������(�}�C�i�X�ɓ]������0��)
	for(i=0;i<5;i++){
		if(param[i]>0){					//�p�����[�^���v���X
			if(param[i]<work){				//�}�C�i�X�J�E���g�����v���X�l��������
				param[i] = 0;					//0��
			}else{							//�}�C�i�X�J�E���g�����v���X�l���傫��
				param[i] -=work;				//�}�C�i�X�J�E���g������
			}
		}
	}
	
	for(i=0;i<5;i++) paramtest[i] = param[i];
	
//	work = 100+bpm/1000;				//20002/08/28�@�C��		1������3.33����
	work = 100+bpm/333;
	paramwork = work;
	for(i=0;i<5;i++){					//�ō�BPM����|�������Z�o���p�����[�^�ɂ������킹��
		calcwork = param[i];
		calcwork = (calcwork*work)/10;	//�l�̌ܓ��p��100�Ŋ���Ȃ�
		mod = calcwork%10;				//�l�̌ܓ�����
		calcwork /=10;
		if(mod>=5)	calcwork++;
		param[i] = calcwork;
	}
	for(i=0;i<5;i++) paramtest2[i] = param[i];

	
	type = CalcCubeType(mix, param,num,minuscount);		//���˂���L���[�u�̎�ނ�����
	newcube->type = type&0xff;

	param[5] /=num;										//�|���p�����[�^�͎Q���l���̕��ς����
	param[5] -=num;										//���̌�A�Q���l���ň���
	if(param[5]<0) param[5] = 0;
	
	
	if(newcube->type==KURO_CUBE){						//���낢�L���[�u�̎���
		work = pp_rand()%FIVE_TO_THREE;					//�T�̒�����R�I��Œl���Q�ɂ���
		for(i=0;i<5;i++){
			if((five2three[work]>>i)&1) param[i] = 2;
			else						param[i] = 0;
		}
	}
	
	for(i=0;i<6;i++){									//�p�����[�^MAX�𒴂��Ă���ꍇ��MAX�Ŏ~�߂�
		if(param[i] > CUBE_PARAM_MAX) 
			param[i] = CUBE_PARAM_MAX;
	}
	
	newcube->karai  = param[0];							//�L���[�u�Ƀp�����[�^���i�[
	newcube->sibui  = param[1];
	newcube->amai   = param[2];
	newcube->nigai  = param[3];
	newcube->suppai = param[4];
	newcube->umai   = param[5];

	for(i=0;i<6;i++)
		calc[i] = param[i];

}

//------------------------------------------------------------------
//  ���˂����L���[�u���O��������Ăׂ�悤��
//------------------------------------------------------------------
void FuncSeed2Cube(KINOMIWORK *mix, CUBE *newcube, u8 num, u8 *calc, u16 bpm)
{
	Seed2Cube(mix, newcube,num,calc,bpm);
}


static void SetBorderLine()
{
	u16 time,vmax;
	s16 ret;
	time = bmw->timer;
	vmax = bmw->bpm_max;

	ret = 0;
	if(time<15*60)						ret = 5;
	else if(time>=15*60&&time<25*60)	ret = 4;
	else if(time>=25*60&&time<35*60)	ret = 3;
	else if(time>=35*60&&time<50*60)	ret = 2;
	else if(time>=55*60&&time<60*60)	ret = 1;
	else if(time>=60*60)				ret = 0;
	SetPlusLine(ret);			//���낢�L���[�u�{�[�_�[���C���ݒ�


	ret = 0;
	if(vmax>64)						ret = 0;
	else if(vmax>= 50 &&  vmax< 100)	ret = -1;
	else if(vmax>= 100 &&  vmax<150)	ret = -2;
	else if(vmax>= 150 &&  vmax< 200)	ret = -3;
	else if(vmax>= 200 &&  vmax< 250)	ret = -4;
	else if(vmax>= 250 &&  vmax< 300)	ret = -5;
	else if(vmax>= 350 &&  vmax< 400)	ret = -6;
	else if(vmax>= 400 &&  vmax< 450)	ret = -7;
	else if(vmax>= 500 &&  vmax< 550)	ret = -8;
	else if(vmax>= 550 &&  vmax< 600)	ret = -9;
	else if(vmax>= 600)		ret = -10;
	SetMinusLine(ret);			//���낢�L���[�u�{�[�_�[����C���ݒ�
	
}

// =================================================================
// BlendYesNoWrite
// �T�v  : �͂��E�������I��\��
// ����  : none
// �߂�l: none
// =================================================================
static void BlendYesNoWrite()
{
	bmw->cursor = 0;
	NMenuWinBoxWrite(23,8,28,13);
	BlendMsgPrint( yesno,YESNO_CHR_START_NO,23+2,8+1);									//�u�͂��@�������v
	BlendMsgPrint( yajirushi,CURSOR_CHR_START_NO,23+1,bmw->cursor*2+8+1);
}

// =================================================================
// BlendYesNoCursorMove
// �T�v  : �͂��E����������ł̶��وړ�
// ����  : no
// �߂�l: none
// =================================================================
static void BlendYesNoCursorMove(u8 no)
{
	bmw->cursor=no;
	BlendMsgPrint( yajirushi,CURSOR_CHR_START_NO,23+1,bmw->cursor*2+8+1);
	BlendMsgPrint( blank,BLANK_CHR_START_NO,23+1,((bmw->cursor)^1)*2+8+1);
	
}

enum{
	BLEND_END_INIT=1,
	BLEND_WHEEL_STOP,

	BLEND_END_KEKKA_TUUSHIN,			//�ʐM���̂�
	BLEND_END_KEKKA_TUUSHIN_WAIT,		//�ʐM���̂�

	BLEND_END_KEKKA,
	BLEND_END_KEKKA2,
	BLEND_END_KEKKA_WAIT,
	BLEND_END_RETRY,
	BLEND_END_SELECT,
	BLEND_END_DECIDE,
	BLEND_END_CHECK,
	BLEND_END_SIO_WAIT,
	BLEND_ENS_MSG_WAIT,
};

static const u8 rpmmaxmsg[]={sa_,ko_,u_,so_,ku_,EOM_};
//------------------------------------------------------------------
//  �ʐM���s���ăf���̏I���̃^�C�~���O��}��
//------------------------------------------------------------------
void BlendMachineEnd()
{
	u8 Id,i;
	BLEND_RESULT *p;
	
	rotate_func();

	Id=GetSioConnectId();

	switch(bmw->endflag){
		case BLEND_END_INIT:							//��]�I���҂�
			BlendMachineFunctionClear();				//�L�[��񑗐M�I��
			m4aMPlayTempoControl(&m4a_mplay000, 256);	
			for(i=0;i<ScriptParameter0;i++)				//NPC��������I��
				DelTask(bmw->NPC_taskno[i]);
			bmw->endflag++;
			break;
		case BLEND_WHEEL_STOP:
			bmw->v_kakudo-=0x20;
			if(bmw->v_kakudo<=0){			//��]�X�g�b�v
				bmw->v_kakudo = 0;
				if(pokemon_sio_flag){		//�ʐM���Ă��鎞�̓f�[�^�󂯎�菈����
					bmw->endflag++;
				}else{
					bmw->endflag = BLEND_END_KEKKA;
				}
				bmw->initflag = 0;
//				m4aMPlayVolumeControl(&m4a_mplay002,0xffff,bmw->wait);
//				m4aSongNumStop(SE_MOTER);
				m4aMPlayStop(&m4a_mplay002);
			}
			motor_pitch();
			break;
		case BLEND_END_KEKKA_TUUSHIN:
			if(GetSioConnectId()==0){
				if(SplitDataSendStatus()){
					bmw->BlendResult.timer   = bmw->timer;
					bmw->BlendResult.bpm_max = bmw->bpm_max;
					SplitDataSendStart(0, (u8*)&(bmw->BlendResult),40);
					bmw->endflag++;
				}
			}else{
				bmw->endflag++;
			}
			break;
		case BLEND_END_KEKKA_TUUSHIN_WAIT:
			if(SplitDataResult()){
				p = (BLEND_RESULT*)&RecvPoolBuf[0][0];
				bmw->bpm_max = p->bpm_max;
				bmw->timer   = p->timer;
				bmw->endflag++;
				SplitDataResultReset();
			}
			break;
		case BLEND_END_KEKKA:
			if(BlendMachineResultPut2()){			//���ʕ\����ʂQ
//				m4aMPlayStop(&m4a_mplay002);
				bmw->endflag++;
			}
			break;
		case BLEND_END_KEKKA2:						//���ʕ\�����
			if(BlendMachineResultPut()){
				if(BlendMachineType == PLAYER_SIO){
					IncRecord(CNT_COMM_CUBE_MAKE);		//�ʐM�Ń|���b�N��������񐔂��C���N�������g
				}else{
					IncRecord(CNT_MAKE_CUBE);			//�|���b�N��������񐔂��C���N�������g
				}
				bmw->endflag++;
			}

			break;
		case BLEND_END_KEKKA_WAIT:
			bmw->endflag++;
			BlendTalkMsgSet( retry_msg,WINMSG_CHR_START_NO,2,15);			//�u���������ǂ��܂����H�v
			break;
		case BLEND_END_RETRY:
			if(BlendTalkMsgPut()){
				bmw->endflag++;
			}
			break;
		case BLEND_END_SELECT:															//�u���������ǁ@���܂����H�v
			bmw->cursor=0;
			BlendYesNoWrite();
			bmw->endflag++;															
			break;
		case BLEND_END_DECIDE:
			if(sys.Trg&U_KEY){
				if(bmw->cursor!=0) SePlay(SE_SELECT);
				BlendYesNoCursorMove(0);
			}else if(sys.Trg&D_KEY){
				if(bmw->cursor!=1) SePlay(SE_SELECT);
				BlendYesNoCursorMove(1);
			}else if(sys.Trg&A_BUTTON) {
					SePlay(SE_SELECT);
					bmw->endflag++;
//					MainProcChange(BlendMachineTitleInit);
			}else if(sys.Trg&B_BUTTON){
					SePlay(SE_SELECT);
					bmw->endflag++;
					BlendYesNoCursorMove(1);
			}
			break;
		case BLEND_END_CHECK:
			SendBuf[0] = USER_COMMAND;
			if(bmw->cursor==0){									//�͂�
				if(PocketCheck(SEED_POCKET)==0){					//�c��̃^�l�̊m�F
					bmw->selectwork = 2;
					SendBuf[1] = SELECT_YES_BUT_NOKINOMI;
				}else if(CheckCube()==-1){								//�L���[�u�P�[�X�̒����m�F
					bmw->selectwork = 3;				//�L���[�u�����ς�
					SendBuf[1] = SELECT_YES_BUT_FULLCUBE;
				}else{
					bmw->selectwork = 0;
					SendBuf[1] = SELECT_YES;
//					MainProcChange(BlendMachineTitleInit);
				}
				bmw->endflag++;
			}else{												//������
				bmw->selectwork = 1;
				SendBuf[1] = SELECT_NO;
				bmw->endflag++;
			}
			break;
		case BLEND_END_SIO_WAIT:
			if(BlendMachineType!=0){
				MainProcChange(BlendMachineEnd_Offline);
				bmw->endflag=0;
				bmw->initflag = 0;
			}else{
				BlendTalkMsgSet( taiki_msg,WINMSG_CHR_START_NO,2,15);			//�����񂽂������イ�ł�
				bmw->endflag++;
			}
			break;
		case 	BLEND_ENS_MSG_WAIT:
			if(BlendTalkMsgPut()){
				MainProcChange(BlendMachineEnd_Sio);
				bmw->endflag=0;
				bmw->initflag = 0;
			}
			break;

	}
	SpeedUpVibration();								//�U���v�Z
	SpeedMeterFunc(bmw->v_kakudo);					//��߰��Ұ������

	SioFunc();

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

enum{
	BLEND_END_SWITCH=0,
	BLEND_FULLCUBE_MSG,
	BLEND_NOSEED_MSG,
	BLEND_NOSEED_WAIT,
	BLEND_MSG_END,
	BLEND_END_FADEOUT_BEFORE,
	BLEND_END_SAVE_START,
	BLEND_END_SAVE,
	BLEND_END_SAVE_WAIT,
	BLEND_END_FADEOUT,
	BLEND_END_FADEWAIT,
	BLEND_END_SIO_END_BEFORE,
	BLEND_END_SIO_END,
};

static const u8 tuusin_msg[]={
	PO_,KE_,MO_,N_,RE_,PO_,bou_,TO_,ni_,spc_,ka_,ki_,ko_,n_,de_,i_,ma_,su_,spc_,spc_,spc_,spc_,CR_,
	de_,n_,ge_,n_,wo_,spc_,ki_,ra_,na_,i_,de_,spc_,ku_,da_,sa_,i_,spc_,spc_,spc_,spc_,spc_,spc_,EOM_,
};


// =================================================================
// KoukanDemoSaveSeq
// �T�v  : �ʐM�|�[�Y�@�\��҂��Z�[�u���s�����A���鏈��
// ����  : none
// �߂�l: u8		0�͓r��	1�ŏI��
// =================================================================
u8 BlendEndSave()
{

		switch(bmw->saveseq){
			case 0:
					SetSioSynchronize();				//�ʐM����
					bmw->saveseq = 1;
					bmw->wait = 0;
				break;
			case 1:
				if(SplitDataSendStatus()){				//�����҂�
					bmw->saveseq++;
					SoftResetDisable = 1;				//�\�t�g���Z�b�g�����𖳌���
				}
				break;
			case 2:
				PokeAGBSaveFieldDivInit();				//ALL�Z�[�u������
				bmw->saveseq++;
				bmw->wait = 0;
				break;
			case 3:										//�Z�[�u�̊Ԋu���
				bmw->wait++;
				if(bmw->wait==10){
					SetSioSynchronize();				//�ʐM����
					bmw->saveseq++;
				}
				break;
			case 4:
				if(SplitDataSendStatus()){				//�����҂�
					if(PokeAGBSaveFieldDiv()){				//�P�Z�N�^�Z�[�u
						bmw->saveseq = 5;					//���ޏI�����Ă���5��
					}else{
						bmw->wait = 0;			
						bmw->saveseq = 3;					//�I�����ĂȂ�������3������
					}
				}
				break;
			case 5:
					bmw->saveseq++;
					bmw->wait = 0;
				break;
			case 6:										//�҂�
				if(++bmw->wait>5){
					SoftResetDisable = 0;					//�\�t�g���Z�b�g������L����
					return 1;
				}
				break;
		}
	return 0;
}

// =================================================================
// BlendMachineEnd_Sio
// �T�v  : �ʐM���̏I���֐�
// ����  : none
// �߂�l: none
// =================================================================
void BlendMachineEnd_Sio()
{
	switch(bmw->endflag){
		case BLEND_END_SWITCH:
			if(bmw->siowork[0]==BLEND_CONTINUE){
				bmw->endflag = BLEND_END_FADEOUT_BEFORE;
			}else if(bmw->siowork[0]==BLEND_END){
				if(bmw->sio_reason[0]==SELECT_YES_BUT_NOKINOMI){
					bmw->endflag = BLEND_NOSEED_MSG;
				}else if(bmw->sio_reason[0]==SELECT_YES_BUT_FULLCUBE){
					bmw->endflag = BLEND_FULLCUBE_MSG;
				}else{
					bmw->endflag = BLEND_END_FADEOUT_BEFORE;
				}
			}
//			if(bmw->selectwork==3){
//				bmw->endflag = BLEND_NOSEED_MSG;
//			}
			
			break;
		case BLEND_FULLCUBE_MSG:
			bmw->endflag=BLEND_NOSEED_WAIT;
			NMenuBoxClear(23,8,28,13);
			PM_strcpy(MsgExpandBuffer, SioTrainerInfo[bmw->sio_reason[1]].name);
			PM_strcat(MsgExpandBuffer,sio_fullcube_msg);
			BlendTalkMsgSet( MsgExpandBuffer,WINMSG_CHR_START_NO,2,15);			//�L���[�u�P�[�X�������ς��ł�
			break;
		case BLEND_NOSEED_MSG:
			bmw->endflag++;
			NMenuBoxClear(23,8,28,13);
			PM_strcpy(MsgExpandBuffer, SioTrainerInfo[bmw->sio_reason[1]].name);
			PM_strcat(MsgExpandBuffer,sio_noseed_msg);
			BlendTalkMsgSet( MsgExpandBuffer,WINMSG_CHR_START_NO,2,15);			//�������݂̂�����܂���
			break;
		case BLEND_NOSEED_WAIT:
			if(BlendTalkMsgPut()){
				bmw->wait = 0;
				bmw->endflag++;
			}
			break;
		case BLEND_MSG_END:
			if(++bmw->wait > 60){
				bmw->endflag = BLEND_END_FADEOUT_BEFORE;
			}
			break;
		case BLEND_END_FADEOUT_BEFORE:
//			PaletteFadeReq(0xffffffff,0,0,16,0);			//�t�F�[�h�C��
//			NMenuBoxClear(23,8,28,13);
			BlendMsgPrint( tuusin_msg,WINMSG_CHR_START_NO+2,2,15);
			SetSioSynchronize();
			bmw->endflag++;
			break;
		case BLEND_END_SAVE_START:
			if(SplitDataSendStatus()){
//				PokeAGBSave( POKEAGB_SVFLD );			//�Z�[�u
//				bmw->endflag++;
				bmw->wait = 0;
				bmw->endflag++;
				bmw->saveseq = 0;
			}
			break;
		case BLEND_END_SAVE:
			if(BlendEndSave()){
				SePlay( SE_SAVE );
				bmw->endflag++;
			}
			break;
		case BLEND_END_SAVE_WAIT:
			bmw->endflag++;
			SetSioSynchronize();
			break;
		case BLEND_END_FADEOUT:
			if(SplitDataSendStatus()){
				PaletteFadeReq(0xffffffff,0,0,16,0);			//�t�F�[�h�C��
				bmw->endflag++;
			}
			break;
		case BLEND_END_FADEWAIT:
			if( FadeData.fade_sw == 0 ){							
				if(bmw->siowork[0]==BLEND_CONTINUE){					//�ċN�����t�B�[���h���A��
					MainProcChange(BlendMachineTitleInit);
				}else{
					bmw->wait = 0;
					bmw->endflag++;
				}
			}
			break;
		case BLEND_END_SIO_END_BEFORE:									//FIFO�����̂��ߑҋ@
			if(++bmw->wait>30){
				SetSioEndDataStart();
				bmw->endflag++;
			}
			break;
		case BLEND_END_SIO_END:											//�t�B�[���h�֕��A
			if(pokemon_sio_flag==0){
				MainProcChange(FieldScriptRecover);
			}
			break;
	}
	SioFunc();

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

// =================================================================
// BlendMachineEnd_Offline
// �T�v  : ��l�p�̎��̏I���֐�
// ����  : none
// �߂�l: none
// =================================================================
void BlendMachineEnd_Offline()
{
	switch(bmw->endflag){
		case BLEND_END_SWITCH:
			if(bmw->selectwork==0||bmw->selectwork==1){
				bmw->endflag = BLEND_END_FADEOUT;
			}
			if(bmw->selectwork==2){
				bmw->endflag = BLEND_NOSEED_MSG;
			}
			if(bmw->selectwork==3){
				bmw->endflag = BLEND_FULLCUBE_MSG ;
			}
			break;
		case BLEND_FULLCUBE_MSG:
			bmw->endflag=BLEND_NOSEED_WAIT;
			NMenuBoxClear(23,8,28,13);
			BlendTalkMsgSet( fullcube_msg,WINMSG_CHR_START_NO,2,15);			//�L���[�u�P�[�X�������ς��ł�
			break;
		case BLEND_NOSEED_MSG:
			bmw->endflag++;
			NMenuBoxClear(23,8,28,13);
			BlendTalkMsgSet( noseed_msg,WINMSG_CHR_START_NO,2,15);			//�������݂̂�����܂���
			break;
		case BLEND_NOSEED_WAIT:
			if(BlendTalkMsgPut()){
				bmw->endflag=BLEND_END_FADEOUT;
			}
			break;
		case BLEND_END_FADEOUT:
			LightFadeReq(N_to_B);										//�t�F�[�h�A�E�g
//			m4aMPlayAllContinue();
			bmw->endflag++;
			break;
		case BLEND_END_FADEWAIT:
			if( FadeData.fade_sw == 0 ){								//�ʐM�I������
				if(bmw->selectwork==0){
					MainProcChange(BlendMachineTitleInit);
				}else{
//					MusicPlay(bmw->nowsong);
					MainProcChange(FieldScriptRecover);
				}

			}
			break;
	}
	SioFunc();

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();

}
//------------------------------------------------------------------
//  �ʐM����
//------------------------------------------------------------------
static void SioFunc()
{
	u8 i;

//	if(!ScriptParameter0)	return;
	if(!pokemon_sio_flag)	return;

	if(RecvBuf[0][0]==USER_COMMAND){		//�e���疽�߂��͂�
		if(RecvBuf[1][0]==BLEND_END){			//�I��
			switch(RecvBuf[2][0]){
				case SELECT_NO:					//NO��I�������l������
					bmw->sio_reason[0] = SELECT_NO;
					bmw->sio_reason[1] = RecvBuf[3][0];
					break;
					
				case SELECT_YES_BUT_NOKINOMI:			//���ꂩ���������݂̂��Ȃ�
					bmw->sio_reason[0] = SELECT_YES_BUT_NOKINOMI;
					bmw->sio_reason[1] = RecvBuf[3][0];
					break;
					
				case SELECT_YES_BUT_FULLCUBE:			//���ꂩ���L���[�u�P�[�X�������ς�
					bmw->sio_reason[0] = SELECT_YES_BUT_FULLCUBE;
					bmw->sio_reason[1] = RecvBuf[3][0];
					break;
			}
			bmw->siowork[0] = BLEND_END;
		}else if(RecvBuf[1][0]==BLEND_CONTINUE){//������
			bmw->siowork[0] = BLEND_CONTINUE;
		}
	}

	if(GetSioConnectId()==0){								//�S������̕ԓ���e���󂯎��
		if(bmw->siowork[0]==BLEND_END||bmw->siowork[0]==BLEND_CONTINUE) return;
		for(i=0;i<GetConnectSioMenberNum();i++){					//�W�v
			if(RecvBuf[0][i]==USER_COMMAND){
				switch(RecvBuf[1][i]){
					case SELECT_YES:								//�͂�
						bmw->siowork[i] = SELECT_YES;
						break;
					case SELECT_NO:
						bmw->siowork[i] = SELECT_NO;				//������
						break;
					case SELECT_YES_BUT_NOKINOMI:
						bmw->siowork[i] = SELECT_YES_BUT_NOKINOMI;	//���݂̂��Ȃ�
						break;
					case SELECT_YES_BUT_FULLCUBE:
						bmw->siowork[i] = SELECT_YES_BUT_FULLCUBE;	//�L���[�u�������ς�
						break;
				}
			}
		}
		for(i=0;i<GetConnectSioMenberNum();i++){			//�S������ԓ���������
			if(bmw->siowork[i]==0) break;
		}
		if(i!=GetConnectSioMenberNum()) return;

		for(i=0;i<GetConnectSioMenberNum();i++){			//�S���u�͂��v��
			if(bmw->siowork[i]!=SELECT_YES) break;
		}
		SendBuf[0] = USER_COMMAND;							//���[�U�[�R�}���h���s����
		if(i==GetConnectSioMenberNum()){
			SendBuf[1] = BLEND_CONTINUE;						//�p���R�}���h���s
		}else{
			SendBuf[1] = BLEND_END;								//�I�����܂��
			SendBuf[2] = bmw->siowork[i];
			SendBuf[3] = i;
		}

//		for(i=0;i<GetConnectSioMenberNum();i++){			//�������Ȃ����R��
//		}
	}
}



// =================================================================
// BlendBGAffineSet
// �T�v  : ��]�ʂ̐ݒ���s��(ڼ޽��ݒ��VSync����)
// ����  : des
// �߂�l: none
// =================================================================
static void BlendBGAffineSet(	BgAffineDestData *des)
{
	BgAffineSrcData src;

	src.SrcCenterX = (0x0f*8)<<8;
	src.SrcCenterY = (0x0a*8)<<8;
	src.DispCenterX = (ROTATE_CENTER_X)-bmw->BgOffsetX;
	src.DispCenterY = (ROTATE_CENTER_Y)-bmw->BgOffsetY;
	src.RatioX      = bmw->Ratio;
	src.RatioY      = bmw->Ratio;
	src.Theta       = bmw->kakudo;
	
	BgAffineSet(&src, des, 1);					//��]BG�ʃ��W�X�^�Z�o
												//��]BG�ʃ��W�X�^�i�[,Vsync���荞�ݒ��ɃZ�b�g�����
	
}
//------------------------------------------------------------------
//  ��]����
//------------------------------------------------------------------
static void rotate_func()
{
	bmw->oldkakudo = bmw->kakudo;				
	bmw->kakudo+=bmw->v_kakudo;					//��]�����x�������ĉ�]�p���X�V

	
	BlendBGAffineSet(&bmw->Rotate);				//��]�v�Z
												//���W�X�^�ւ̊i�[��VSync����


}


static void VsyncRotateSet()
{
	*(vu16*)REG_BG2PA	 = bmw->Rotate.H_DiffX;			//��]BG�ʃ��W�X�^�i�[
	*(vu16*)REG_BG2PB	 = bmw->Rotate.V_DiffX;
	*(vu16*)REG_BG2PC	 = bmw->Rotate.H_DiffY;
	*(vu16*)REG_BG2PD	 = bmw->Rotate.V_DiffY;
	*(vu32*)REG_BG2X 	 = bmw->Rotate.StartX ;
	*(vu32*)REG_BG2Y 	 = bmw->Rotate.StartY ;

}

static void VsyncBgOffset()
{
	*(vu16*)REG_BG1HOFS = bmw->BgOffsetX;
	*(vu16*)REG_BG1VOFS = bmw->BgOffsetY;
	*(vu16*)REG_BG0HOFS = bmw->BgOffsetX;
	*(vu16*)REG_BG0VOFS = bmw->BgOffsetY;
}

//=========================================================================
//  �ΉԂ̓���
//=========================================================================
static void hibana_act_move(actWork *act)
{

	act->work[2] += act->work[0];
	act->work[3] += act->work[1];	
	act->dx = act->work[2]/8;
	act->dy = act->work[3]/8;
	if(act->anmend_sw) DelActor(act);
}

//=========================================================================
//  �ΉԂ��U�点��
//=========================================================================
static void hibana_actor_add()
{

	int work,wx,wy,i,no;
	u16	kaku;
	
//	work = pp_rand()%5+2;
	work = pp_rand()%2+1;
	for(i=0;i<work;i++){
		kaku=bmw->kakudo+pp_rand()%20;
		wx = (_Cos(kaku&0xff)*64)/256;
		wy = (_Sin(kaku&0xff)*64)/256;
		no=AddActor(&HibanaHeader,wx+120,wy+80,1);
		ActWork[no].work[0] = 8*2-pp_rand()%(4*8);
		ActWork[no].work[1] = 8*2-pp_rand()%(4*8);
		ActWork[no].move = hibana_act_move;
	}

}


//=========================================================================
//  ����A�C�R���̓���
//=========================================================================
static void HanteiMove(actWork *act)
{
	act->work[0]++;
	act->dy = -(act->work[0]/3);
	if(act->anmend_sw){
		DelActor(act);
	}
}

static void HanteiMove2(actWork *act)
{
	act->work[0]++;
	act->dy = -(act->work[0]*2);
	if(act->dy<-12){
		act->dy=-12;
	}
	if(act->anmend_sw){
		DelActor(act);
	}
}


//=========================================================================
//  �ʐM�����o�[�̑I���A�C�e�����Z�b�g����
//=========================================================================
void BlendMachineMemberItem(u8 Id, u16 Item)
{
	bmw->memberitem[Id]= Item;
	KinomiWorkSet(&bmw->Kinomi[Id],Item);
}



//--------------------------------------------------------------------------------
//  �u�����h�}�V�[���̊J�n����
//
//
//  �e�͈�莞�ԂɈ��ʐM�����������q�̐ڑ���Ԃ��Ď�����B
//  �܂��e�̃L�[����Ńu�����h�}�V�[���̃X�^�[�g����B
//
//	0:��莞�Ԃŏ���������J�E���^
//--------------------------------------------------------------------------------
void WaitBlendMachineStart(u8 no)
{
	int i;
	
	TASK_TABLE *my;
	my = &TaskTable[no];

	if(pokemon_sio_flag){					//�ʐM���Ă���
		if(GetSioConnectId()==0){			//�e
			my->work[0]++;
			if(my->work[0]>120){
//				PokemonMultiSioInit();
				my->work[0]=0;
			}
			if(my->work[0]==100){
				FontScrnAreaClear(&bmw->tw, 0, 0, 16, 20);

				NMenuWinBoxWrite(4,4,4+6,4+8);

				//�V�K�ʐM�e�X�g�p�ɉ��X�g�b�v
				for(i=0;i<3;i++){
					if(SioTrainerInfo[i+1].Id!=0) BlendMsgPrint( ok_msg[i],WINMSG_CHR_START_NO+i*10,5,5+i*2);
					NMenuWinBoxWrite(0,13,29,19);
					BlendMsgPrint( start_msg,WINMSG_CHR_START_NO+10*4,1,15);
				}
			}
			if(sys.Trg&A_BUTTON){				//�e�����̓u�����h�}�V�[���X�^�[�g�𑖂点��
//				BlendMachineFunction_Set();
				BlendMachineStartSend();
				DelTask(no);
			}
		}else{								//�q
			if(my->work[0]==10)
					BlendMsgPrint( wait_msg,100,3,10);
			if(++my->work[0]>120) my->work[0]=0;
			
			if(SendXydataFlag>4&&pokemon_sio_flag==1){
				DelTask(no);
			}
		}
	}
}


//=========================================================================
//  �ꕶ�����W�J���C�����[�`��(��b��)
//=========================================================================
u8 BlendTalkMsgPut( void )
{
	return TalkMsgPut( &(bmw->tw) );
}

//=========================================================================
//  �ꕶ�����W�J���b�Z�[�W�\��(��b��)
//=========================================================================
void BlendTalkMsgSet( const u8 * msg, u16 chr_no, u8 x, u8 y )
{
	TalkMsgSet(&(bmw->tw), msg, MenuFontStartNo + chr_no, x, y );
}

//=========================================================================
//  �ꊇ������\��
//
//  �E�C���h�E�t�H���g�E�V�X�e���t�H���g�̌�납��\�����܂�
//=========================================================================
void BlendMsgPrint( const u8 * msg, u16 chr_no, u8 x, u8 y )
{
	MsgPrint(&bmw->tw, msg, MenuFontStartNo + chr_no, x, y);
}

//=========================================================================
//  �E�C���h�E�t���ꊇ�\��
//=========================================================================
void BlendWindMsgPrint(const u8 *msg, u16 chr_no, u8 x1, u8 y1, u8 x2, u8 y2)
{
	NMenuWinBoxWrite(x1,y1,x2,y2);
	MsgPrint(&bmw->tw, msg, MenuFontStartNo + chr_no, x1+1, y1+1);

}


//=========================================================================
//  �J�E���^�[�̃A�j��
//
//	work[0]:  ���
//  work[1]:  Y���W
//  work[2]:  ����
//	work[3]:  �A�j���ԍ�
//
//=========================================================================
static void CountActSeq(actWork *Xreg)
{
	switch(Xreg->work[0]){
		case 0:
			Xreg->work[1]+=8;					//��ʏォ�牺��
			if(Xreg->work[1]>88){
				Xreg->work[1]=88;
				Xreg->work[0]++;
				SePlay(SE_KON);
			}
			break;
		case 1:
			Xreg->work[2]++;
			if(Xreg->work[2]>20){
				Xreg->work[0]++;
				Xreg->work[2]=0;
			}
			break;
		case 2:
			Xreg->work[1]+=4;					//��ʏォ�牺��
			if(Xreg->work[1]>160+16){			//��ʉ��ɏ�������
				Xreg->work[3]++;
				if(Xreg->work[3]==3){
					DelActor(Xreg);			//�P�̎��́uSTATRT�v
					AddActor(&ActStartHeader,120,-20,2);
				}else{
					Xreg->work[0]=0;
					Xreg->work[1]=-16;			//OBJ�̊G��ύX���Ă܂���ʏ㕔��
					ActAnmChg(Xreg,Xreg->work[3]);
				}
			}
			break;
	}
	Xreg->dy = Xreg->work[1];
}

//=========================================================================
//  �u�X�^�[�g�v�̃A�j��
//	work[0]:  ���
//  work[1]:  Y���W
//  work[2]:  ����
//=========================================================================
static void StartActSeq(actWork *Xreg)
{
	switch(Xreg->work[0]){
		case 0:
			Xreg->work[1]+=8;					//��ʏォ�牺��
			if(Xreg->work[1]>92){
				Xreg->work[1]=92;
				Xreg->work[0]++;
				SePlay(SE_PIN);
			}
			break;
		case 1:
			Xreg->work[2]++;
			if(Xreg->work[2]>20){
				Xreg->work[0]++;
			}
			break;
		case 2:
			Xreg->work[1]+=4;					//��ʏォ�牺��
			if(Xreg->work[1]>160+16){			//��ʉ��ɏ�������
					bmw->initflag++;
					DelActor(Xreg);				//
			}
			break;
	}
	Xreg->dy = Xreg->work[1];

}


// =============================================
// MeterFunc
// �T�v  : �A�j���[�V�����Ή����[�^�[���f���[�`��
// ����  : meter
//       : max
// �߂�l: static void
// =============================================
static void MeterFunc(u16 meter,u16 max){
	
	if(bmw->now_meter<meter){						//���[�^�[�����񂮂�L�тĂ����悤��
		bmw->now_meter+=2;
		MeterFuncSub(bmw->now_meter,max);
	}
}

// =============================================
// MeterFuncSub
// �T�v  : ���[�^�[�̂a�f������s��
// ����  : ���݂̃��[�^�[��
// ����  : ���[�^�[�̍ő�l
// =============================================
static void MeterFuncSub(u16 meter, u16 max)
{
	int i,work,w1,w2;
	vu16*	p;
	p = (vu16*)(BLEND_BACK_MAP);

	work = (METER1_DOT_MAX*meter)/max;			//�ڐ���̗ʂ��Z�o
	w1 = work/8;								//�W�Ŗ��܂��Ă�Z�����Z�o���Z�b�g
	for(i=0;i<w1;i++){
		p[METER1_POS_INDEX   +i] = METER1_PAT_0+8;
		p[METER1_POS_INDEX+32+i] = METER1_PAT_0+8+16;	//��񉺂̃Z�������߂�
	}

	w2 = work%8;										//�[���̖ڐ�����Z�o���Z�b�g
	if(w2!=0){
		p[METER1_POS_INDEX   +i] = METER1_PAT_0+w2;
		p[METER1_POS_INDEX+32+i] = METER1_PAT_0+w2+16;		//��񉺂̃Z����
		i++;
	}
														//��̃Z���͋󔒂Ŗ��߂�
	for(;i<METER1_MAP_MAX;i++){
		p[METER1_POS_INDEX   +i] = METER1_PAT_0;
		p[METER1_POS_INDEX+32+i] = METER1_PAT_0+16;	
	
	}
}

//
// 60/0x10000/60 =
// 
//     60           60                        60               60        60*60
// ---------- =  -------------------  =  ---------------   x -----  =  ------------
// 0x10000/60     0x10000x    1            0x10000x   1        60       0x10000
//                         --------                 ----
//                            60                     60

#define BPM_TIME_SECOND		(6000*60)
#define BPM_TIME_AROUND		(0x10000)
// =================================================================
// RotateSpeed2rpm
// �T�v  : ���̂݃u�����_�[�p��]���x�����ۂ̉�]���x�i�������j�֕ϊ�
// ����  : v_kakudo
// �߂�l: u32		��]���x���P�O�O�i�����_2���j
// =================================================================
u32 RotateSpeed2rpm(u16 v_kakudo)
{
	u32 work;
	work = (BPM_TIME_SECOND*v_kakudo)/BPM_TIME_AROUND;
	return work;
}


// =================================================================
// SpeedMeterFunc
// �T�v  : �X�s�[�h���[�^�[�̕\��
// ����  : v
// �߂�l: none
// =================================================================
static void SpeedMeterFunc(u16 v)
{
	u8 work[5],i;
	u16 *p;
	u32 bpm;
	
	bpm = RotateSpeed2rpm(v);		//���̂݃u�����_�[�p�̉�]�W������]���x�i�������j�֕ϊ�
	
	if(bmw->bpm_max<bpm)			//�ō���]���x��ۑ�
		bmw->bpm_max = bpm;


	for(i=0;i<5;i++){				//���ʂ���\���p�̃��[�N�ɂ���Ă���
		work[i] = bpm%10;
		bpm /= 10;
	}
	p=(u16*)(BLEND_BACK_MAP);
	p[SPEED_POS_INDEX+0] = work[4]+SPEED_PAT_0;		//BG�Ƀ��[�^�[�𔽉f������
	p[SPEED_POS_INDEX+1] = work[3]+SPEED_PAT_0;		//�蔲��
	p[SPEED_POS_INDEX+2] = work[2]+SPEED_PAT_0;
	p[SPEED_POS_INDEX+4] = work[1]+SPEED_PAT_0;
	p[SPEED_POS_INDEX+5] = work[0]+SPEED_PAT_0;
}

static void SpeedUpVibrationSet(s16 *n, u16 offset)
{
	if(*n==0){			//�h�ꂪ�i�[����ĂȂ�������
		*n = pp_rand()%offset-(offset/2);			//����тŊi�[
	}
}


static void VibrationSub(s16 *n)
{
		if(*n<0) (*n)++;				//0�ɂȂ�܂Ō��炷
		if(*n>0) (*n)--;
}

static void SpeedUpVibration()
{
	VibrationSub(&bmw->BgOffsetX);
	VibrationSub(&bmw->BgOffsetY);
}

// =================================================================
// VibrationCalc
// �T�v  : �U���v�Z
// ����  : n		�U���̾�Ă��i�[�����߲��
//       : wait		��ϰ����
// �߂�l: none
// =================================================================
static void VibrationCalc(s16 *n, u16 wait)
{
	u8 w;
	w = 8;
	if(wait<10){		//�h��Ă���Ԃ̑O���͑傫�����
		w = 16;
	}

	if(*n==0){			//�h�ꂪ�i�[����ĂȂ�������
		*n = pp_rand()%w-(w/2);			//����тŊi�[
	}else{
		if(*n<0) (*n)++;				//0�ɂȂ�܂Ō��炷
		if(*n>0) (*n)--;
	}
	
}

// =================================================================
// BlendMachineVibration
// �T�v  : ��ʂ̐U������
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 BlendMachineVibration()
{
	if(bmw->wait==0){								//�h�ꃂ�[�h�̏�����
		bmw->BgOffsetX = 0;
		bmw->BgOffsetY = 0;
	}
	bmw->wait++;
	VibrationCalc(&bmw->BgOffsetX,bmw->wait);		//���h��
	VibrationCalc(&bmw->BgOffsetY,bmw->wait);		//�c�h��


	if(bmw->wait==20){								//20�ݸ�h�ꂽ�炨�I��
		bmw->BgOffsetX = 0;
		bmw->BgOffsetY = 0;
		return 1;
	}
	return 0;
}

// =================================================================
// BlendCursorActMove
// �T�v  : �J�[�\���̃A�N�^�[���[�`��
// ����  : act
// �߂�l: none
// =================================================================
static void BlendCursorActMove(actWork *act)
{
	act->dx = -bmw->BgOffsetX;		//�w�i�̐U���ɃJ�[�\�����Ή����邽��
	act->dy = -bmw->BgOffsetY;
}


// =================================================================
// RankingRPMSet
// �T�v  : �q�o�l�̃����L���O��o�^
// ����  : none
// �߂�l: none
// =================================================================
static void RankingRPMSet()
{
	//�ʐM�ƒʏ펞�ōō����x�𕪂���̂͂Ȃ��Ȃ�܂����B
//	if(BlendMachineType==PLAYER_SIO)
//	{
//		if(Fld.SioBPMRecord[bmw->member_num-2] < bmw->bpm_max){
//			Fld.SioBPMRecord[bmw->member_num-2] = bmw->bpm_max;
//		}
//	}else{
		if(Fld.NormalBPMRecord[bmw->member_num-2] < bmw->bpm_max){
			Fld.NormalBPMRecord[bmw->member_num-2] = bmw->bpm_max;
		}
//	}
}

// =================================================================
// BlendMachineResultPut
// �T�v  : ���̂݃u�����_�[���ʕ\����ʂ��̂P
// ����  : none
// �߂�l: static u8
// =================================================================
#define RESULT_X_LINE	6
#define TITLE_X_LINE	12

#define KEKKA1_1LINE_CELL		(i*(10+8)*2)
#define KEKKA1_NAMEPUT_CELL		(WINMSG_CHR_START_NO+24+KEKKA1_1LINE_CELL)
#define KEKKA1_KINOMI_CELL		(KEKKA1_NAMEPUT_CELL+10*2)
#define KEKKA1_MAXSPEED_CELL	(KEKKA1_NAMEPUT_CELL)
#define KEKKA1_MAXPEEDNUM_CELL	(KEKKA1_MAXSPEED_CELL+7*2)
#define KEKKA1_TIME_CELL		(KEKKA1_MAXPEEDNUM_CELL+20)
#define KEKKA1_TIMENUM_CELL		(KEKKA1_TIME_CELL+2*6)

static const u8 strytbl[]={6,6,6,6,5};
static const u8 posytbl[]={3,3,3,2,2};
static u8 BlendMachineResultPut()
{
	u8 calc[6],strwork[20],*p;
	u16 table[4],i,no,byou,hun;
	CUBE cubework;


	switch(bmw->initflag)
	{
		case 0:
			bmw->initflag++;
			bmw->wait = 0x11;
			break;
		case 1:
			bmw->wait-=10;
			if(bmw->wait<0){
				bmw->wait=0;
				bmw->initflag++;
			}
			break;
		case 2:
			if(++bmw->wait>20){
				for(i=0;i<3;i++) DelActor(&ActWork[bmw->actno[i]]);		//�O�̉�ʂŕ\������Ă���A�N�^�[������
				bmw->wait = 0;
				bmw->initflag++;
			}
			break;
		case 3:

			NMenuWinBoxWrite(RESULT_X1,RESULT_Y1,RESULT_X2,RESULT_Y2);
			BlendMsgPrint( resultmsg0,WINMSG_CHR_START_NO,				TITLE_X_LINE,3);	//�܂���������

			for(i=0;i<bmw->member_num;i++){
				no = bmw->sort[i];
				PM_NumMsgSet(bmw->strbuf, i+1, NUM_MODE_LEFT, 1);
				PM_strcat(bmw->strbuf,konmamsg);
				PM_strcat(bmw->strbuf,SioTrainerInfo[no].name);
				BlendMsgPrint( bmw->strbuf,KEKKA1_NAMEPUT_CELL,
					RESULT_X_LINE+1,strytbl[bmw->member_num]+i*posytbl[bmw->member_num]);	//���O
//				PM_strcpy(bmw->strbuf,SeedParameter[bmw->memberitem[no]-ITEM_SEED_START].name);

//				GetItemName(bmw->memberitem[no],bmw->strbuf);
				PM_strcpy(bmw->strbuf,bmw->Kinomi[no].name);


//				bmw->strbuf[3] = EOM_;
				PM_strcat(bmw->strbuf,nomimsg);
				BlendMsgPrint( bmw->strbuf,KEKKA1_KINOMI_CELL,
					RESULT_X_LINE+11,strytbl[bmw->member_num]+i*posytbl[bmw->member_num]);	//���ꂽ�^�l
			}
			i=4;
			BlendMsgPrint( resultmsg3,KEKKA1_MAXSPEED_CELL,RESULT_X_LINE,13);	//��������������
			PM_NumMsgSet(bmw->strbuf, bmw->bpm_max/100, NUM_MODE_SPACE, 3);
			PM_strcat(bmw->strbuf,konmamsg);
			PM_NumMsgSet(strwork, bmw->bpm_max%100, NUM_MODE_ZERO, 2);
			PM_strcat(bmw->strbuf,strwork);
			PM_strcat(bmw->strbuf,kmmsg);
			BlendMsgPrint( bmw->strbuf,KEKKA1_MAXPEEDNUM_CELL,RESULT_X_LINE+8,13);	//��������������
//+12+10
//+12+10+16
			BlendMsgPrint( resultmsg2,KEKKA1_TIME_CELL,RESULT_X_LINE,15);	//������
			byou = (bmw->timer/60)%60;
			hun  = bmw->timer/(60*60);
			PM_NumMsgSet(bmw->strbuf, hun, NUM_MODE_ZERO, 2);
			p = PM_strcat(bmw->strbuf,hunmsg);
			PM_NumMsgSet(p, byou, NUM_MODE_ZERO, 2);
			PM_strcat(bmw->strbuf,byoumsg);
			BlendMsgPrint( bmw->strbuf,KEKKA1_TIMENUM_CELL,RESULT_X_LINE+8,15);	//������
				
			bmw->wait = 0;
			bmw->initflag++;
			break;
		case 4:
			if(sys.Trg&A_BUTTON){
	//			SePlay(SE_SELECT);
				bmw->initflag++;
			}
			break;
		case 5:
				NMenuScreenClear();
				NMenuWinBoxWrite(0,14,29,19);
				for(i=0;i<4;i++){
					if(bmw->memberitem[i]>0){				//�I�����˂̕ϊ����
						table[i] = bmw->memberitem[i] - ITEM_SEED_START;
					}
				}
				SetBorderLine();
				Seed2Cube(bmw->Kinomi, &cubework, bmw->member_num, calc,bmw->bpm_max);		//���ː����v�Z
		
				CubeComentSet(&cubework,bmw->strbuf);				//�ł����|���b�N�̃R�����g���Z�b�g
				
				AddTask(ItemJingle2ContestBgm,6);
#ifdef PM_DEBUG
				PM_HexNumMsgSet(strwork, GetBlendMachineCount(), NUM_MODE_LEFT, 4);
				PM_strcat(strwork,normalwait_msg);
				PM_strcat(bmw->strbuf,strwork);
#endif
				BlendTalkMsgSet(bmw->strbuf, WINMSG_CHR_START_NO, 2, 15);		//�u�L���[�u���ł��܂����v
				SubItem(ReturnItemNo,1);										//�^�l�����炷

				AddCube(&cubework);
				bmw->initflag++;
				
			break;
		case 6:
			if(BlendTalkMsgPut()){									//�e�L�X�g�̃{�^�������҂�
				RankingRPMSet();
				return 1;
			}
			break;
	}

	return 0;
}

// =================================================================
// CubeComentSet
// �T�v  : �ł����������|���b�N�̃R�����g���Z�b�g
// ����  : p
//       : buf
// �߂�l: none
// =================================================================
static void CubeComentSet(CUBE *p, u8 *strbuf)
{
	u8 strwork[10],level,nameraka;

	strbuf[0]=EOM_;
	PM_strcpy(strbuf,cube_name[p->type]);				//�H�H�H�̃|���b�N���ł��܂����I
	PM_strcat(strbuf,end_msg);
	PM_strcat(strbuf,crmsg);
#ifdef DEBUG_PARAMETA
	PM_NumMsgSet(strwork, p->karai, NUM_MODE_LEFT, 3);	//�f�o�b�O�p�ɖ��̃p�����[�^���L
	PM_strcat(strbuf,strwork);
	PM_strcat(strbuf,spcmsg);
	PM_NumMsgSet(strwork, p->sibui, NUM_MODE_LEFT, 3);
	PM_strcat(strbuf,strwork);
	PM_strcat(strbuf,spcmsg);
	PM_NumMsgSet(strwork, p->amai, NUM_MODE_LEFT, 3);
	PM_strcat(strbuf,strwork);
	PM_strcat(strbuf,spcmsg);
	PM_NumMsgSet(strwork, p->nigai, NUM_MODE_LEFT, 3);
	PM_strcat(strbuf,strwork);
	PM_strcat(strbuf,spcmsg);
	PM_NumMsgSet(strwork, p->suppai, NUM_MODE_LEFT, 3);
	PM_strcat(strbuf,strwork);
	PM_strcat(strbuf,spcmsg);
	PM_NumMsgSet(strwork, p->umai, NUM_MODE_LEFT, 3);
	PM_strcat(strbuf,strwork);
	PM_strcat(strbuf,normalwait_msg);
	PM_strcat(strbuf,crmsg);
#endif				
	level    = CubeLevelGet(p);								//�|���b�N�̃��x���擾
	nameraka = CubeNamerakasaGet(p);						//�|���b�N�̂Ȃ߂炩���擾
	PM_strcat(strbuf,level_msg);
	PM_NumMsgSet(strwork, level, NUM_MODE_LEFT, 3);			//�u���x���H�H�v
	PM_strcat(strbuf,strwork);
	
	
	PM_strcat(strbuf,nameraka_msg);							//�u�ŁA�Ȃ߂炩���́H�H���v
	PM_NumMsgSet(strwork, nameraka, NUM_MODE_LEFT, 3);
	PM_strcat(strbuf,strwork);
	PM_strcat(strbuf,comentend_msg);
	PM_strcat(strbuf,normalwait_msg);
	
}

// =================================================================
// KeySort
// �T�v  : �f�[�^���\�[�g
// ����  : work		���ёւ��鏇��
//       : member	�l��
//       : data		���̔z��̐��т�work����ёւ���
// �߂�l: none
// =================================================================
static void KeySort(u8 *work, u8 member, u32 *data){
	int i,w;
	u8 tmp;
	
	for(i=0;i<member;i++){						//�蔲���o�u���\�[�g��͂�
		for(w=0;w<member;w++){
			if(data[work[i]]>data[work[w]]){
				tmp = work[i];
				work[i] = work[w];
				work[w] = tmp;
			}
		}
	}
}

// =================================================================
// SortHitResult
// �T�v  : ���̂݃u�����_�[�̐��т��\�[�g����
// ����  : none
// �߂�l: none
// =================================================================
static void SortHitResult()
{
	u8  i;
	u8  work[4];
	u32 keydata[4];
	
	for(i=0;i<bmw->member_num;i++) work[i] = i;
	for(i=0;i<bmw->member_num;i++){								//�\�[�g�p�Ƀf�[�^���W�v
		keydata[i]  = bmw->hitcount[i][0]*1000000;
		keydata[i] += bmw->hitcount[i][1]*1000;
		keydata[i] += 1000-bmw->hitcount[i][2];
	}

	
	KeySort( work, bmw->member_num, keydata);					//�\�[�g

	for(i=0;i<bmw->member_num;i++) bmw->sort[i] = work[i];
}

// =================================================================
// BlendMachineResultPut
// �T�v  : ���̂݃u�����_�[���ʉ�ʂ��̂Q
// ����  : none
// �߂�l: static u8
// =================================================================
static const u8 narabi[]={3,3,3,3,2};

#define KEKKA2_TITLE	(WINMSG_CHR_START_NO)
#define KEKKA2_NAME		(KEKKA2_TITLE+24+i*(11+9)*2)
#define KEKKA2_COUNT	(WINMSG_CHR_START_NO+24+((i*(11+9))+9+w*3)*2)

#define HANTEI_ICON_X	128
#define HANTEI_ICON_Y	52

static u8 BlendMachineResultPut2()
{
	u16 i,w,no;


	switch(bmw->initflag)
	{
		case 0:
			bmw->initflag++;
			bmw->wait = 0xff;
			break;
		case 1:
			bmw->wait-=10;
			if(bmw->wait<0){
				bmw->wait=0;
				bmw->initflag++;
			}
			break;
		case 2:
			if(++bmw->wait>20){
//				m4aMPlayVolumeControl(&m4a_mplay002,0xffff,bmw->wait);
//				m4aSongNumStop(SE_MOTER);
				bmw->wait = 0;
				bmw->initflag++;
			}
			break;
		case 3:

			NMenuWinBoxWrite(RESULT_X1,RESULT_Y1,RESULT_X2,RESULT_Y2);
			BlendMsgPrint( rankingmsg,KEKKA2_TITLE,	TITLE_X_LINE,3);	//�����L���O

			bmw->actno[0] = AddActor(&HanteiHeader,HANTEI_ICON_X,HANTEI_ICON_Y,0);
			ActAnmChg(&ActWork[bmw->actno[0]],3);
			ActWork[bmw->actno[0]].move = DummyActMove;
			bmw->actno[1] = AddActor(&HanteiHeader,HANTEI_ICON_X+32,HANTEI_ICON_Y,0);
			ActWork[bmw->actno[1]].move = DummyActMove;
			bmw->actno[2] = AddActor(&HanteiHeader,HANTEI_ICON_X+64,HANTEI_ICON_Y,0);
			ActAnmChg(&ActWork[bmw->actno[2]],1);
			ActWork[bmw->actno[2]].move = DummyActMove;

			SortHitResult();										//���ʂ��\�[�g bmw->sort[]�Ɋi�[
			for(i=0;i<bmw->member_num;i++){
				no = bmw->sort[i];
				PM_NumMsgSet(bmw->strbuf, i+1, NUM_MODE_LEFT, 1);
				PM_strcat(bmw->strbuf,konmamsg);
				PM_strcat(bmw->strbuf,SioTrainerInfo[no].name);
				BlendMsgPrint( bmw->strbuf,KEKKA2_NAME,
					RESULT_X_LINE-1,8+i*narabi[bmw->member_num]);	//���O
				for(w=0;w<3;w++){
					PM_NumMsgSet(bmw->strbuf, bmw->hitcount[no][w], NUM_MODE_SPACE, 3);
					BlendMsgPrint( bmw->strbuf,	KEKKA2_COUNT,
						RESULT_X_LINE+8+4*w,8+i*narabi[bmw->member_num]);	//���ꂽ�^�l
				}
			}
				
			bmw->wait = 0;
			bmw->initflag++;
			break;
		case 4:
			if(++bmw->wait>20){
				bmw->initflag++;
			}
			break;
		case 5:
			if(sys.Trg&A_BUTTON){
				SePlay(SE_SELECT);
				bmw->initflag++;
			}
			break;
		case 6:
				bmw->initflag=0;
				return 1;
			break;
	}

	return 0;
}




typedef struct{
	s8		cursorpos;
	s8		seeds[4];
	CUBE	cube;
	u8		rewrite;
	u8		work[6];
	s8      minus;
	s8      plus;
	s16		bpm;
}SEED2CUBE;
static SEED2CUBE	s2c;
//=========================================================================
//���̂݁��L���[�u�e�X�g��ʏ�����
//=========================================================================
void BlendTestInit(void)
{
	InitActor();				//�A�N�^�[������
	ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
	InitTask();


	SetVBlankFunc(VInterFunc2);

	PrintScreenInit(FIELD_MENU_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);

	pp_srand(sys.main_timer);

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_BG2_ON					// BG2�L��
						  | DISP_OBJ_ON;

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
	s2c.bpm = 8000;
	s2c.rewrite++;

	MainProcChange(BlendTestMain);
};




static const u8 kuuhaku[]={spc_,EOM_};
static const u8 bpm_msg[]={B__,P__,M__,EOM_};
static const u8 minus_kigou[]={bou_,EOM_,};

// =================================================================
// write_info
// �T�v  : �u�L���[�u�e�X�g�v���\��
// ����  : none
// �߂�l: none
// =================================================================
static void write_info()
{
	u8 strbuf[128],i;

	PM_strcpy(strbuf,bpm_msg);
	NMenuMsgWrite(strbuf,2,0);
	
	PM_NumMsgSet(strbuf,s2c.bpm/100,NUM_MODE_ZERO,3);
	NMenuMsgWrite(strbuf,6,0);

	for(i=0;i<4;i++){
		if(s2c.cursorpos==i){
			strbuf[0] = cursor_;
//			memcpy(&strbuf[1],SeedParameter[s2c.seeds[i]].name,SEED_NAME_SIZE);
			GetItemName(s2c.seeds[i]+ITEM_SEED_START,&strbuf[1]);
//			strbuf[SEED_NAME_SIZE+1] = EOM_;
		}else{
//			memcpy(strbuf,SeedParameter[s2c.seeds[i]].name,SEED_NAME_SIZE);
			GetItemName(s2c.seeds[i]+ITEM_SEED_START,strbuf);
			strbuf[SEED_NAME_SIZE] = spc_;
			strbuf[SEED_NAME_SIZE+1] = EOM_;
		}
		
		NMenuMsgWrite(strbuf,2,3+3*i);

		PM_NumMsgSet(strbuf,SeedParameter[s2c.seeds[i]].karai,NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_NumMsgSet(strbuf+3,SeedParameter[s2c.seeds[i]].sibui,NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_NumMsgSet(strbuf+6,SeedParameter[s2c.seeds[i]].amai,NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_NumMsgSet(strbuf+9,SeedParameter[s2c.seeds[i]].nigai,NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_NumMsgSet(strbuf+12,SeedParameter[s2c.seeds[i]].suppai,NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_NumMsgSet(strbuf+15,SeedParameter[s2c.seeds[i]].umai,NUM_MODE_ZERO,2);
		strbuf[17] = EOM_;

		NMenuMsgWrite(strbuf,7,3+3*i);
		
	}

	if(s2c.cube.type!=0){
		PM_strcpy(strbuf,cube_name[s2c.cube.type]);
		NMenuMsgWrite(strbuf,2,3+3*4);

		PM_HexNumMsgSet(strbuf,s2c.work[0],NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_HexNumMsgSet(strbuf+3,s2c.work[1],NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_HexNumMsgSet(strbuf+6,s2c.work[2],NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_HexNumMsgSet(strbuf+9,s2c.work[3],NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_HexNumMsgSet(strbuf+12,s2c.work[4],NUM_MODE_ZERO,2);
		PM_strcat(strbuf,kuuhaku);
		PM_HexNumMsgSet(strbuf+15,s2c.work[5],NUM_MODE_ZERO,2);
		strbuf[17] = EOM_;

		NMenuMsgWrite(strbuf,7,3+3*4+2);
	}

//	PM_strcat(strbuf,setuzoku_text[no]);
//	NMenuWinBoxWrite(x,y,x+14,y+5);
	

}

u8  randomtestflag = 0;
u32 randomcount = 0;
u16	randbunpu[10];
u16 firstrandom;
u32 randomhit = 0;
u8  randombit = 0;
static const u8 cr_msg[]={CR_,EOM_};

// =================================================================
// randomtestfunc
// �T�v  : ������Ƃ��������̃e�X�g
// ����  : none
// �߂�l: none
// =================================================================
static void randomtestfunc2()
{
	u16 w,i,*p;
	u8 strbuf[70],strwork[10];
	if(randomtestflag==1){
		for(w=0;w<10;w++) randbunpu[w] = 0;
		firstrandom = pp_rand();
		randomcount=0;
		randomtestflag=2;
		for(i=0;i<100*2;i++) UserWork[i] = 0;
		randombit = 0;
	}
//	for(i=0;i<150;i++){
//		w=pp_srand(sys.main_timer);
//		w=pp_rand();
//		if(w==firstrandom){
//			randomtestflag=3;
//			randomhit = randomcount;
//		}
//		randomcount++;
//	}
	for(i=0;i<100;i++){
		w=pp_rand();
//		w /= (RAND_MAX/100);
//		p = (u16*)UserWork;
//		p[w]++;
		if(((w>>15)&1)==randombit){		//�r�b�g���ꏏ��������
			randomcount++;
		}else{						//������甽�]�����ĕۑ�
			p = (u16*)UserWork;
			p[randomhit] = randomcount;
			randomhit++;
			randomcount = 0;
			randombit ^= 1;
		}
	}
	
	strbuf[0] = EOM_;
	
	PM_HexNumMsgSet(strwork,firstrandom,NUM_MODE_ZERO,8);
	PM_strcat(strbuf,strwork);
	PM_strcat(strbuf,kuuhaku);
	PM_HexNumMsgSet(strwork,randomcount,NUM_MODE_ZERO,8);
	PM_strcat(strbuf,strwork);

	PM_strcat(strbuf,cr_msg);
	if(randomtestflag==3){
		PM_HexNumMsgSet(strwork,randomhit,NUM_MODE_ZERO,16);
		PM_strcat(strbuf,strwork);
		randomtestflag = 0;
	}
	NMenuMsgWrite(strbuf,2,3+3*4);

}

static void seed2cubepadfunc()
{
	u16 mix[4],i;
	u16 num;
	KINOMIWORK kinomi[4];
	
	if(sys.Trg&R_BUTTON){
		if((s2c.bpm+=1000)>30000) s2c.bpm = 1000;
		s2c.rewrite++;
	}

	if(sys.Trg&L_BUTTON){
		if((s2c.bpm-=1000)<0) s2c.bpm = 30000;
		s2c.rewrite++;
	}
	
	if(sys.Trg&U_KEY){				//�J�[�\����
		if(--s2c.cursorpos<0) s2c.cursorpos = 3;
		s2c.rewrite++;
	}
	if(sys.Trg&D_KEY){				//�J�[�\��down
		if(++s2c.cursorpos>3) s2c.cursorpos = 0;
		s2c.rewrite++;
	}
	if(sys.Trg&L_KEY){				//�J�[�\��left	���ˑI��
		if(--s2c.seeds[s2c.cursorpos]<0) s2c.seeds[s2c.cursorpos] = NORMAL_SEED_TYPE_MAX;
		s2c.rewrite++;
	}
	if(sys.Trg&R_KEY){				//�J�[�\��right ���ˑI��
		if(++s2c.seeds[s2c.cursorpos]>NORMAL_SEED_TYPE_MAX) s2c.seeds[s2c.cursorpos] = 0;
		s2c.rewrite++;
	}
	if(sys.Trg&A_BUTTON){			//�L���[�u�v�Z����
		num=0;
		for(i=0;i<4;i++){
			if(s2c.seeds[i]!=NORMAL_SEED_TYPE_MAX){
				num++;
				mix[i] = s2c.seeds[i];
				KinomiWorkSet(&kinomi[i],s2c.seeds[i]+ITEM_SEED_START);
			}else{
				break;
			}
		}
		if(num>=2){
			FuncSeed2Cube(kinomi,&s2c.cube,num,s2c.work,(u16)s2c.bpm);
			s2c.rewrite++;
		}else{
			s2c.cube.type=0xff;
		}
	}
	if(s2c.rewrite){
		write_info();
		s2c.rewrite = 0;
	}

	//�Z���N�g�{�^���������Ɨ����e�X�g���J�n�����	
	if(sys.Trg&SELECT_BUTTON){
		if(!randomtestflag){
			randomtestflag++;
			randomcount = 0;
			pp_srand(sys.main_timer);
		}
	}
	if(randomtestflag)
		randomtestfunc2();
}


static const u8 RET[2] = {CR_,EOM_};

//-------------------------------------------------------------------------
//�u�L���[�u�e�X�g�v��ʃ��C�����[�`��
//-------------------------------------------------------------------------
void BlendTestMain(void)
{
	

	//�L���[�u�v�Z�f�o�b�O
	seed2cubepadfunc();

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
};

#define B_SCORE_WIN_X	6
#define B_SCORE_WIN_Y	3
#define B_SCORE_WIN_X2	23
#define B_SCORE_WIN_Y2	16

#define B_SCORE_TITLE_X	8
#define B_SCORE_TITLE_Y	4

#define B_SCORE_LIST_X	8
#define B_SCORE_LIST_Y	9

#define B_SCORE_SCORE_X	(B_SCORE_LIST_X+5)
#define B_SCORE_SCORE_Y	B_SCORE_LIST_Y

static const u8 str_title[]={
	ki_,no_,mi_,BU_,RE_,N_,DA_,bou_,CR_,sa_,i_,ko_,u_,so_,ku_,do_,spc_,RA_,N_,KI_,N_,GU_,gyoe_,EOM_,
};

static const u8 str_koumoku[]={
	hu_,ta_,ri_,CR_,sa_,n_,ni_,n_,CR_,yo_,ni_,n_,EOM_,
};


//-------------------------------------------------------------------------
//	�ΐ퐬�т̕\��
//-------------------------------------------------------------------------
void PutBlendMachineScoreWindow( void )
{
	int i;
	u32 work;
	u8  strbuf[30];
	u8 *p;

	NMenuWinBoxWrite( B_SCORE_WIN_X, B_SCORE_WIN_Y, B_SCORE_WIN_X2, B_SCORE_WIN_Y2 );

	NMenuMsgWrite( str_title, B_SCORE_TITLE_X, B_SCORE_TITLE_Y );

	NMenuMsgWrite( str_koumoku, B_SCORE_LIST_X, B_SCORE_LIST_Y );
	for ( i=0; i< 3; i++ ) {
//		work = RotateSpeed2rpm(Fld.NormalBPMRecord[i]);
		work = Fld.NormalBPMRecord[i];
		p = PM_NumMsgSet(strbuf, work/100, NUM_MODE_SPACE, 3);
		p = PM_strcat(p,konmamsg);
		p = PM_NumMsgSet(p, work%100, NUM_MODE_ZERO, 2);
		PM_strcat(p,kmmsg);
		NMenuMsgWrite( strbuf, B_SCORE_SCORE_X, B_SCORE_SCORE_Y+2*i);
	}
}


// =================================================================
// ItemJingle2ContestBgm
// �T�v  : �|���b�N���󂯎��W���O����炵����A�R���e�X�g���̂a�f�l�ɖ߂�
// ����  : no
// �߂�l: none
// =================================================================
void ItemJingle2ContestBgm(u8 no)
{
	if(TaskTable[no].work[0]==0){
		JinglePlay(MUS_FANFA1);
		TaskTable[no].work[0]++;
	}
	
	if(JingleWait()){
		MusicPlay(bmw->nowsong);
		DelTask(no);
	}
}