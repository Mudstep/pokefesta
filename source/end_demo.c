//******************************************************************
//          end_demo.c                                                  
//                                                                  
//          Copyright (C) 2001 GAME FREAK inc.
//******************************************************************
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "mus_tool.h"
#include "task.h"
#include "decord.h"
#include "calctool.h"
#include "print.h"
#include "menu.h"
#include "window.h"
#include "objdata.h"
#include "poketool.h"
#include "zukan.h"
#include "monsno.def"
#include "syswork.h"
#include "ev_flag.h"

#include "end_demo.dat"

#define	BACK_COLOR1	(0x328D)	//�t�F�[�h�l�ݒ�(org cyc_demo.dat)
#define	BACK_COLOR2	(0x1967)	//�t�F�[�h�l�ݒ�(org opselect.c)

typedef struct {

u8			xoffs;
const u8*	name;

}STAFFROLL_DATA;

typedef struct {

const STAFFROLL_DATA*	list[5];

}STAFFROLL_LIST;

//�X�^�b�t���[���f�[�^��`
#include "staff_rl.dat"

typedef struct {

u16			poke[60];
u16			put_p;
u16			poke_p;
u16			poke_max;

u16			tmp[POKE_NUM_MAX];
u16			tmpwork[8];

}ED_POKECUT_WORK;

#define	PUT_POKE_MAX	(56)
#define	LAST_WAIT		(7200)

extern	u16		pp_rand(void);
extern	void	MaruC_logoMake(u16 cgx_offs,u16 scr_offs,u16 pal_offs);
extern	const	CellData	CycDemoOBJcellBoy[];
extern	const	CellData	CycDemoOBJcellGirl[];
extern	const	CellData	CycDemoOBJcellJitensya[];
extern	const	PalData		CycDemoOBJpal[];
extern	const	CellData	CycDemoOBJcellBoyEnd[];
extern	const	CellData	CycDemoOBJcellGirlEnd[];
extern	void	affineWorkSet(u8 num,u16 H_X,u16 V_X,u16 H_Y,u16 V_Y);
extern	u16		FirstPokemonNumGet(u16 index);

extern	void	CycDemoEndBGSet(u8 mode);
extern	void	CycDemoEndRegSet(u8 mode);
extern	u8		CycDemoBGScrollSet(u8 mode,u16 BG1sp,u16 BG2sp,u16 BG3sp);
extern	void	CycDemoBGEffect(u8 mode);
extern	u8		CycDemoBoySet(s16 x,s16 y);
extern	u8		CycDemoGirlSet(s16 x,s16 y);
extern	s16		cyc_demo_y;
extern	s16		cyc_demo_dy;
extern	s16		cyc_demo_actdel_sw;

extern	const u16	opball_Palette[];
extern	const u8	opselch_LZ[];
extern	const u8	opselsc2_LZ[];

void			EDdemoInit(void);
void			EDdemoMain(void);

void			AllRight_logoMake(u16 cgx_offs,u16 scr_offs,u16 pal_offs);
void			THEEND_logoMake(u16 scr_offs,u16 pal_offs);

static	void	RegInit(void);

static	void	EDdemo_MainSceneInitTask(u8 id);
static	void	EDdemo_MainSceneMainTask(u8 id);
static	void	EDdemo_MainSceneChangeInitTask(u8 id);
static	void	EDdemo_MainSceneChangeTask(u8 id);
static	void	EDdemo_MainSceneCutInInitTask(u8 id);
static	void	EDdemo_MainSceneCutInTask(u8 id);
static	void	EDdemo_MainSceneEndWaitInitTask(u8 id);
static	void	EDdemo_MainSceneEndWaitTask(u8 id);

static	void	EDdemo_EndLogoTask(u8 id);
static	void	EDdemo_EndLogoWaitTask(u8 id);
static	void	EDdemo_EndMarkTask(u8 id);
static	void	EDdemo_EndMarkWaitTask(u8 id);
static	void	EDdemo_EndTask(u8 id);

static	void	StaffRollTask(u8 id);
static	void	OBJeventTask(u8 id);
static	void	BGPaletteEffectTask(u8 id);
static	void	PokeCutInTask(u8 id);

static	u8		EDdemoBackGroundSet(u8 scene,u8 id);
static	void	EDdemoBackTaskDel(u8 id);

static const actAnm* const	EDdemoJikiAnmTbl[];
static const actAnm* const	EDdemoSuportAnmTbl[];
static void					EDdemoJikiActMove(actWork* aw);
static void					EDdemoSuportActMove(actWork* aw);

static	u16		EDdemoPokeActorSet(u16 pnum,s16 x,s16 y,u16 decord_p);
static const	actHeader		EDpokeWinActHeader;

static	const	CellData	EDpokeWinCell[];
static	const	PalData		EDpokeWinPal[];

static	void	EDPokeListDataGet(void);

#define	ED_FIRSTWAIT	(0x100)
#define	ED_ENDWAIT		(0x100)
//���C���^�X�N���[�N��`
#define	ED_T1_SCRTASK	(0)
#define	ED_T1_EVENTTASK	(1)
#define	ED_T1_EFFTASK	(2)
#define	ED_T1_PCUTTASK	(3)
#define	ED_T1_ENDSW		(4)
#define	ED_T1_ACT1		(5)
#define	ED_T1_ACT2		(6)
#define	ED_T1_SCENE		(7)
#define	ED_T1_EFWORK	(8)
#define	ED_T1_XBAK		(9)
#define	ED_T1_XFBAK		(10)
#define	ED_T1_SCENESW	(11)
#define	ED_T1_WAIT		(12)
#define	ED_T1_MODE		(13)
//�X�^�b�t���[���^�X�N���[�N��`
#define	ED_T2_SEQ		(0)
#define	ED_T2_MAINTASK	(1)
#define	ED_T2_LISTP		(2)
#define	ED_T2_WAIT		(3)
//�n�a�i�C�x���g�^�X�N���[�N��`
#define	ED_T3_SEQ		(0)
#define	ED_T3_MAINTASK	(1)
#define	ED_T3_ACT1		(2)
#define	ED_T3_ACT2		(3)
#define	ED_T3_WAIT		(4)
#define	ED_T3_EFWORK	(5)
//�a�f�G�t�F�N�g�^�X�N���[�N��`
#define	ED_T4_SCENE		(0)
//�|�P�����J�b�g�C���^�X�N���[�N��`
#define	ED_T5_SEQ		(0)
#define	ED_T5_MAINTASK	(1)
#define	ED_T5_PAGE		(2)
#define	ED_T5_WAIT		(3)
#define	ED_T5_TIMER		(4)

#define	CELLID_EDPOKEWIN	(1001)
#define	PALID_EDPOKEWIN		(1001)

extern	u32	WaitWork;
u16		StaffRollWork	= {0};
u16		EndDemoMainTask = {0};
u8		GameClearFlag	= {0};
u8		EdDemoSndFadeSW = {0};
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

/********************************************************************/
/*                                                                  */
/*				���C��												*/
/*                                                                  */
/********************************************************************/
void	EDdemoMain(void)
{
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();

	if((sys.Cont & B_BUTTON)&&GameClearFlag){
		if(TaskTable[EndDemoMainTask].TaskAdrs == EDdemo_MainSceneMainTask){
			VInterFunc();

			MainTask();
			JumpActor();
			SetActor();
			PaletteAnime();
			EdDemoSndFadeSW = 1;
		}
	}
}

/********************************************************************/
/*                                                                  */
/*				������												*/
/*                                                                  */
/********************************************************************/
void	EDdemoInit(void)
{
	u8	taskID1,taskID2;
	ED_POKECUT_WORK*	edpw;

	RegInit();
	SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

	PaletteAnimeInit();
	InitTask();										// ���  ������

	taskID1 = AddTask(EDdemo_MainSceneInitTask, 0);
	TaskTable[taskID1].work[ED_T1_ENDSW]	= 0;
	TaskTable[taskID1].work[ED_T1_SCENE]	= 0;
	TaskTable[taskID1].work[ED_T1_SCENESW]	= 0;
	TaskTable[taskID1].work[ED_T1_MODE]		= 1;
	
	while(1){
		if(EDdemoBackGroundSet(0,taskID1))	break;
	}
	TaskTable[TaskTable[taskID1].work[ED_T1_EVENTTASK]].work[0] = 40;

	PrintScreenInit(STAFFROLL_TYPE);
	NMenuScreenInit(STAFFROLL_TYPE);
	PaletteWorkSet(StaffRoll_Palette,PA_BG8,0x40);
	CpuCopy(&andMark_Character[0x00],BG_VRAM+0x8000+0x3EE0,0x40,16);//���߂������ӁI
	*(vu16 *)REG_BG0VOFS = -4;
	taskID2 = AddTask(StaffRollTask, 0);
	TaskTable[taskID2].work[ED_T2_MAINTASK] = taskID1;

	PaletteFadeReq(0xffffffff,0,16,0,0);

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
	*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;
	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)

	m4aSongNumStart(MUS_THANKFOR);
	MainProcChange(EDdemoMain);
	EdDemoSndFadeSW = 0;

	edpw = (ED_POKECUT_WORK*)(UserWork + 0x1c000);

	EDPokeListDataGet();
	edpw->put_p		= 0;
	edpw->poke_p	= 0;

	EndDemoMainTask = taskID1;
}

/********************************************************************/
/*                                                                  */
/*				���C�������i�^�X�N�j								*/
/*                                                                  */
/********************************************************************/
/*------------------------------------------------------------------*/
/*				���C���V�[���i���]�ԁj								*/
/*------------------------------------------------------------------*/
static	void	EDdemo_MainSceneInitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	TaskTable[id].TaskAdrs	= EDdemo_MainSceneMainTask;
}

/*------------------------------------------------------------------*/
static	void	EDdemo_MainSceneMainTask(u8 id)
{
	if(TaskTable[id].work[ED_T1_ENDSW]){
		TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[0] = 30;
		TaskTable[id].work[ED_T1_WAIT]	= ED_ENDWAIT;
		TaskTable[id].TaskAdrs	= EDdemo_MainSceneEndWaitInitTask;
		return;
	}
	StaffRollWork = 0;

	if(TaskTable[id].work[ED_T1_SCENESW] == 1){
		TaskTable[id].work[ED_T1_MODE] = TaskTable[id].work[ED_T1_SCENESW];
		TaskTable[id].work[ED_T1_SCENESW] = 0;
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs	= EDdemo_MainSceneChangeInitTask;
		return;
	}
	if(TaskTable[id].work[ED_T1_SCENESW] == 2){
		TaskTable[id].work[ED_T1_MODE] = TaskTable[id].work[ED_T1_SCENESW];
		TaskTable[id].work[ED_T1_SCENESW] = 0;
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs	= EDdemo_MainSceneCutInInitTask;
		return;
	}
}

/*------------------------------------------------------------------*/
static	void	EDdemo_MainSceneChangeInitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	*(vu16 *)REG_DISPCNT = 0;
	EDdemoBackTaskDel(id);
	TaskTable[id].TaskAdrs	= EDdemo_MainSceneChangeTask;
}

/*------------------------------------------------------------------*/
static	void	EDdemo_MainSceneChangeTask(u8 id)
{
	SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

	if(EDdemoBackGroundSet(TaskTable[id].work[ED_T1_SCENE],id)){	//����
		PaletteFadeReq(0xffffffff,0,16,0,0);

		REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
		*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;
		SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)

		TaskTable[id].TaskAdrs	= EDdemo_MainSceneInitTask;
	}
}

/*------------------------------------------------------------------*/
static	void	EDdemo_MainSceneCutInInitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	*(vu16 *)REG_DISPCNT = 0;
	EDdemoBackTaskDel(id);
	TaskTable[id].TaskAdrs	= EDdemo_MainSceneCutInTask;
}

/*------------------------------------------------------------------*/
static	void	EDdemo_MainSceneCutInTask(u8 id)
{
	u16	i;

	switch(sys.InitSeqWork){
	default:
	case 0:
		InitActor();			// ����������
		ObjPalManInit();		// ��޼ު��گ��Ȱ�ެ������
		ObjPalManStart = 8;		// ��گ��Ȱ�ެNo�J�n

		LZ77UnCompVram((void*)opselch_LZ,(void*)(BG_VRAM+0x0000));	//BG  CGX�]��
		LZ77UnCompVram((void*)opselsc2_LZ,(void*)(BG_VRAM+0x3800));	//BG3 MAP�]��(�w�i)
		PaletteWorkSet(&(opball_Palette[0x0001]),PA_BG0+PA_COL1,0x40-2);	//BG2&BG3 COL�]��
		for(i=0;i<0x800;i++)	*(u8*)(UserWork + 0x1e000 + i) = 0x11;	//�E�C���h�E�L�����쐬�P
		for(i=0;i<0x800;i++)	*(u8*)(UserWork + 0x1e800 + i) = 0x22;	//�E�C���h�E�L�����쐬�Q
		for(i=0;i<0x800;i++)	*(u8*)(UserWork + 0x1f000 + i) = 0x33;	//�E�C���h�E�L�����쐬�R
		*(u16*)(UserWork + 0x1f800 + 0*2) = 0;	//�E�C���h�E�p���b�g�O�쐬
		*(u16*)(UserWork + 0x1f800 + 1*2) = 0x53FF;	//�E�C���h�E�p���b�g�P�쐬
		*(u16*)(UserWork + 0x1f800 + 2*2) = 0x529F;	//�E�C���h�E�p���b�g�Q�쐬
		*(u16*)(UserWork + 0x1f800 + 3*2) = 0x7E94;	//�E�C���h�E�p���b�g�R�쐬
		CellSet((CellData*)EDpokeWinCell);
		ObjPalSet((PalData*)EDpokeWinPal);
		sys.InitSeqWork++;
		break;
	case 1:
		TaskTable[id].work[ED_T1_PCUTTASK] = AddTask(PokeCutInTask, 0);
		TaskTable[TaskTable[id].work[ED_T1_PCUTTASK]].work[ED_T5_SEQ] = 1;
		TaskTable[TaskTable[id].work[ED_T1_PCUTTASK]].work[ED_T5_MAINTASK] = id;
		TaskTable[TaskTable[id].work[ED_T1_PCUTTASK]].work[ED_T5_PAGE]
				=TaskTable[id].work[ED_T1_SCENE];

		PaletteFadeReq(0xffffffff,0,16,0,0);
		*(vu16 *)REG_BG3HOFS	= 0;
		*(vu16 *)REG_BG3VOFS	= 0x20;
		*(vu16 *)REG_BG3CNT = BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_PRIORITY_3
							| 7 << BG_SCREEN_BASE_SHIFT | 0 << BG_CHAR_BASE_SHIFT;

		*(vu16 *)REG_DISPCNT =	DISP_MODE_0					// BG���[�h 
			                 |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							 |  DISP_BG3_ON					// BG3�L��
							 |  DISP_BG0_ON					// BG0�L��
							 |  DISP_OBJ_ON;
		sys.InitSeqWork = 0;
		cyc_demo_actdel_sw = 0;
		TaskTable[id].TaskAdrs	= EDdemo_MainSceneInitTask;
		break;
	}
}

/*------------------------------------------------------------------*/
static	void	EDdemo_MainSceneEndWaitInitTask(u8 id)
{
	if(TaskTable[id].work[ED_T1_WAIT]){
		TaskTable[id].work[ED_T1_WAIT]--;
	}else{
		PaletteFadeReq(0xffffffff,12,0,16,0);
		TaskTable[id].TaskAdrs	= EDdemo_MainSceneEndWaitTask;
	}
}

/*------------------------------------------------------------------*/
static	void	EDdemo_MainSceneEndWaitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	EDdemoBackTaskDel(id);
	TaskTable[id].TaskAdrs	= EDdemo_EndLogoTask;
}

/*------------------------------------------------------------------*/
/*				�����܂�											*/
/*------------------------------------------------------------------*/
static	void	EDdemo_EndLogoTask(u8 id)
{
	RegInit();

	PaletteAnimeInit();
	AllRight_logoMake(0x0000,0x3800,PA_BG0);

	InitActor();									// ����������
	ObjPalManInit();								// ��޼ު��گ��Ȱ�ެ������

	PaletteFadeReq(0xffffffff,8,16,0,0);

	*(vu16 *)REG_BG0CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_0					//BG�D�揇��
						| 7 << BG_SCREEN_BASE_SHIFT		//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
	*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0				// BG���[�h 
						|	DISP_OBJ_CHAR_1D_MAP	// OBJ1����ϯ��ݸ�
						|	DISP_BG0_ON				// BG0�L��
						;
	TaskTable[id].work[0]	= 200+0x68;
	TaskTable[id].TaskAdrs	= EDdemo_EndLogoWaitTask;
}

/*------------------------------------------------------------------*/
static	void	EDdemo_EndLogoWaitTask(u8 id)
{
	if(TaskTable[id].work[0]){
		TaskTable[id].work[0]--;
		return;
	}
	PaletteFadeReq(0xffffffff,6,0,16,0);
	TaskTable[id].TaskAdrs	= EDdemo_EndMarkTask;
}

/*------------------------------------------------------------------*/
static	void	EDdemo_EndMarkTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	THEEND_logoMake(0x3800,PA_BG0);

	PaletteFadeReq(0xffffffff,0,0,0,0);
	TaskTable[id].work[0]	= LAST_WAIT;
	TaskTable[id].TaskAdrs	= EDdemo_EndMarkWaitTask;
}

/*------------------------------------------------------------------*/
static	void	EDdemo_EndMarkWaitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	if((!TaskTable[id].work[0])||(sys.Trg)){
		MusicFadeOutEnd(4);
		PaletteFadeReq(0xffffffff,8,0,16,0xffff);
		TaskTable[id].TaskAdrs	= EDdemo_EndTask;
	}else{
		if(TaskTable[id].work[0] == LAST_WAIT-56){
			MusicFadeOutEnd(8);
		}
		if(TaskTable[id].work[0] == LAST_WAIT-360){
			m4aSongNumStart(MUS_END);
		}
		TaskTable[id].work[0]--;
	}
}

static	void	EDdemo_EndTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	SoftReset(RESET_ALL_FLAG);
}


/********************************************************************/
/*                                                                  */
/*				���C�������i�T�u�j									*/
/*                                                                  */
/********************************************************************/
static	void	RegInit(void)
{
	*(vu16 *)REG_DISPCNT	= 0;
	*(vu16 *)REG_BG3HOFS	= 0;
	*(vu16 *)REG_BG3VOFS	= 0;
	*(vu16 *)REG_BG2HOFS	= 0;
	*(vu16 *)REG_BG2VOFS	= 0;
	*(vu16 *)REG_BG1HOFS	= 0;
	*(vu16 *)REG_BG1VOFS	= 0;
	*(vu16 *)REG_BG0HOFS	= 0;
	*(vu16 *)REG_BG0VOFS	= 0;
	*(vu16 *)REG_BLDCNT		= 0;
	*(vu16 *)REG_BLDALPHA	= 0;
	*(vu16 *)REG_BLDY		= 0;

	DmaClear(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
	DmaClear(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
	DmaClear(3, 0, PLTT+2, PLTT_SIZE-2,       16);	// ��گ�  ������
}






/********************************************************************/
/*                                                                  */
/*				�X�^�b�t���[���^�X�N								*/
/*                                                                  */
static	u8	SceneControl(u8 page,u8 id);
/********************************************************************/
static	void	StaffRollTask(u8 id)
{
	u16	i,offs;
	const u8*	str;

	switch(TaskTable[id].work[ED_T2_SEQ]){

	default:
	case 0:
		if(FadeData.fade_sw)	break;

		//������
		TaskTable[id].work[ED_T2_SEQ] = 1;
		TaskTable[id].work[ED_T2_WAIT] = 0x58;
		StaffRollWork = 0;
		break;
	case 1:
		//�����E�F�C�g
		if(TaskTable[id].work[ED_T2_WAIT]){
			TaskTable[id].work[ED_T2_WAIT]--;
		}else{
			TaskTable[id].work[ED_T2_SEQ]++;
		}
		break;
	case 2:
		//�\��
		//���̏����Ɋ֌W�Ȃ����ő��点�悤�Ǝv�������A����������
		if(TaskTable[TaskTable[id].work[ED_T2_MAINTASK]].TaskAdrs == EDdemo_MainSceneMainTask){
			if(TaskTable[id].work[ED_T2_LISTP] < STAFFROLL_LISTCNT){
				for(i=0;i<5;i++){
					offs = StaffRollList[TaskTable[id].work[ED_T2_LISTP]].list[i]->xoffs;
					str  = StaffRollList[TaskTable[id].work[ED_T2_LISTP]].list[i]->name;
					NMenuMsgWrite(str,offs,9+i*2);
				}
				TaskTable[id].work[ED_T2_LISTP]++;
				TaskTable[id].work[ED_T2_SEQ]++;
				if(TaskTable[TaskTable[id].work[ED_T2_MAINTASK]].work[ED_T1_MODE] == 1){
					PaletteFadeReq(0x00000300,0,16,0,BACK_COLOR1);
				}else{
					PaletteFadeReq(0x00000300,0,16,0,BACK_COLOR2);
				}
			}else{
				TaskTable[id].work[ED_T2_SEQ] = 10;
			}
		}
		break;
	case 3:
		//�t�F�[�h�C��
		if(FadeData.fade_sw)	break;

		TaskTable[id].work[ED_T2_WAIT] = 180-8;
		TaskTable[id].work[ED_T2_SEQ]++;
		break;
	case 4:
		//�\���E�F�C�g
		if(TaskTable[id].work[ED_T2_WAIT]){
			TaskTable[id].work[ED_T2_WAIT]--;
		}else{
			if(SceneControl(TaskTable[id].work[ED_T2_LISTP],TaskTable[id].work[ED_T2_MAINTASK])){
				TaskTable[id].work[ED_T2_SEQ]++;
			}else{
				TaskTable[id].work[ED_T2_SEQ]++;
				if(TaskTable[TaskTable[id].work[ED_T2_MAINTASK]].work[ED_T1_MODE] == 1){
					PaletteFadeReq(0x00000300,0,0,16,BACK_COLOR1);
				}else{
					PaletteFadeReq(0x00000300,0,0,16,BACK_COLOR2);
				}
			}
		}
		break;
	case 5:
		//�t�F�[�h�A�E�g
		if(FadeData.fade_sw)	break;

		NMenuBoxClear(0,9,29,19);
		TaskTable[id].work[ED_T2_SEQ] = 2;
		break;
	case 10:
		TaskTable[TaskTable[id].work[ED_T2_MAINTASK]].work[ED_T1_ENDSW] = 1;
		DelTask(id);
		break;
	}
}


/********************************************************************/
/*			��ʐ���                                                */
/********************************************************************/
static	u8	SceneControl(u8 page,u8 id)
{
	if(page == 1){
		TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[0] = 20;
	}
//	if(page == 4){
	if(page == 5){
		TaskTable[id].work[ED_T1_SCENE]		= 5;	//�y�[�W��
		TaskTable[id].work[ED_T1_SCENESW]	= 2;
	}
	if(page == 9){
		TaskTable[id].work[ED_T1_SCENE]		= 1;
		TaskTable[id].work[ED_T1_SCENESW]	= 1;
	}
	if(page == 15){
		TaskTable[id].work[ED_T1_SCENE]		= 4;	//�y�[�W��
		TaskTable[id].work[ED_T1_SCENESW]	= 2;
	}
//	if(page == 20){
	if(page == 19){
		TaskTable[id].work[ED_T1_SCENE]		= 2;
		TaskTable[id].work[ED_T1_SCENESW]	= 1;
	}
	if(page == 22){
		TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[0] = 1;
	}
//	if(page == 25){
	if(page == 24){
		TaskTable[id].work[ED_T1_SCENE]		= 5;	//�y�[�W��
		TaskTable[id].work[ED_T1_SCENESW]	= 2;
	}
//	if(page == 29){
	if(page == 30){
		TaskTable[id].work[ED_T1_SCENE]		= 3;
		TaskTable[id].work[ED_T1_SCENESW]	= 1;
	}
	if(page == 32){
		TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[0] = 10;
	}
	if(page == 34){
		TaskTable[id].work[ED_T1_SCENE]		= 3;	//�y�[�W��
		TaskTable[id].work[ED_T1_SCENESW]	= 2;
	}
	if(page == 39){
		TaskTable[id].work[ED_T1_SCENE]		= 4;
		TaskTable[id].work[ED_T1_SCENESW]	= 1;
	}
	if(TaskTable[id].work[ED_T1_SCENESW]){
		return 1;
	}else{
		return 0;
	}
}




/********************************************************************/
/*                                                                  */
/*				�|�P�����J�b�g�C���^�X�N							*/
/*                                                                  */
static	const u8	PokeCutInPos[3][2] ={{120-16,32+4},{120,32+4},{120+16,32+4}};
/********************************************************************/
static	void	PokeCutInTask(u8 id)
{
	ED_POKECUT_WORK*	edpw;
	u8	actnum;

	edpw = (ED_POKECUT_WORK*)(UserWork + 0x1c000);

	switch(TaskTable[id].work[ED_T5_SEQ]){
	default:
	case 0:
		break;
	case 1:
		if(FadeData.fade_sw)	break;

		TaskTable[id].work[ED_T5_SEQ]++;
		break;
	case 2:
		//�\��
		if(edpw->put_p == PUT_POKE_MAX)		break;

		if(TaskTable[TaskTable[id].work[ED_T5_MAINTASK]].TaskAdrs == EDdemo_MainSceneMainTask){
			actnum = EDdemoPokeActorSet(	edpw->poke[edpw->poke_p],
								PokeCutInPos[edpw->put_p%3][0],
								PokeCutInPos[edpw->put_p%3][1],
								edpw->put_p%3);
			if(edpw->poke_p < edpw->poke_max-1){
				edpw->poke_p++;
				ActWork[actnum].work[3] = 0x40;
			}else{
				edpw->poke_p = 0;	
				ActWork[actnum].work[3] = 0x80;
			}
			edpw->put_p++;

			TaskTable[id].work[ED_T5_WAIT] = 0x40;
			TaskTable[id].work[ED_T5_SEQ]++;
		}
		break;
	case 3:
		if(TaskTable[id].work[ED_T5_WAIT]){
			TaskTable[id].work[ED_T5_WAIT]--;
		}else{
			TaskTable[id].work[ED_T5_SEQ] = 1;
		}
		break;
	}
}






/********************************************************************/
/*                                                                  */
/*				�n�a�i�C�x���g�^�X�N								*/
/*                                                                  */
/********************************************************************/
static	void	OBJeventTask(u8 id)
{
	switch(TaskTable[id].work[ED_T3_SEQ]){

	case 0:
		cyc_demo_dy = SinMove((u8)((TaskTable[id].work[ED_T3_EFWORK]>>1)&0x7f),0x0c);
		TaskTable[id].work[ED_T3_EFWORK]++;
		break;

	case 1:
		if(cyc_demo_dy){
			cyc_demo_dy = SinMove((u8)((TaskTable[id].work[ED_T3_EFWORK]>>1)&0x7f),0x0c);
			TaskTable[id].work[ED_T3_EFWORK]++;
		}else{
			//��l���U���������J�n
			ActWork[TaskTable[id].work[ED_T3_ACT1]].work[0] = 2;
			TaskTable[id].work[ED_T3_EFWORK] = 0;
			TaskTable[id].work[ED_T3_SEQ]++;
		}
		break;
	case 2:
		//��l���U�����
		if(TaskTable[id].work[ED_T3_EFWORK] < 0x40){
			TaskTable[id].work[ED_T3_EFWORK]++;
			cyc_demo_dy = SinMove((u8)(TaskTable[id].work[ED_T3_EFWORK]&0x7f),0x14);
		}else{
			TaskTable[id].work[ED_T3_SEQ]++;
		}
		break;
	case 3:
		//�T�|�[�g�삯����
		ActWork[TaskTable[id].work[ED_T3_ACT1]].work[0] = 3;
		ActWork[TaskTable[id].work[ED_T3_ACT2]].work[0] = 1;
		TaskTable[id].work[ED_T3_WAIT] = 120;
		TaskTable[id].work[ED_T3_SEQ]++;
		break;
	case 4:
		//�삯�����҂�
		if(TaskTable[id].work[ED_T3_WAIT]){
			TaskTable[id].work[ED_T3_WAIT]--;
		}else{
			TaskTable[id].work[ED_T3_EFWORK] = 0x040;
			TaskTable[id].work[ED_T3_SEQ]++;
		}
		break;
	case 5:
		//��l����ʒu���ǂ聕�ǂ�����
		if(TaskTable[id].work[ED_T3_EFWORK] > 0){
			TaskTable[id].work[ED_T3_EFWORK]--;
			cyc_demo_dy = SinMove((u8)(TaskTable[id].work[ED_T3_EFWORK]&0x7f),0x14);
		}else{
			ActWork[TaskTable[id].work[ED_T3_ACT1]].work[0] = 1;
			TaskTable[id].work[ED_T3_SEQ]++;
		}
		break;
	case 6:
		TaskTable[id].work[ED_T3_SEQ] = 50;
		break;

	case 10:
		//�T�|�[�g�ēo��
		ActWork[TaskTable[id].work[ED_T3_ACT2]].work[0] = 2;
		TaskTable[id].work[ED_T3_SEQ] = 50;
		break;

	case 20:
		//��l���o��
		ActWork[TaskTable[id].work[ED_T3_ACT1]].work[0] = 4;
		TaskTable[id].work[ED_T3_SEQ] = 50;
		break;

	case 30:
		//��l�����T�|�[�g�ޏ�
		ActWork[TaskTable[id].work[ED_T3_ACT1]].work[0] = 5;
		ActWork[TaskTable[id].work[ED_T3_ACT2]].work[0] = 3;
		TaskTable[id].work[ED_T3_SEQ] = 50;
		break;

	case 40:
		break;

	case 50:
		TaskTable[id].work[ED_T3_SEQ] = 0;
		break;
	}
}





/********************************************************************/
/*                                                                  */
/*				�a�f�p���b�g�G�t�F�N�g�^�X�N						*/
/*                                                                  */
/********************************************************************/
static	void	BGPaletteEffectTask(u8 id)
{
	switch(TaskTable[id].work[ED_T4_SCENE]){
	default:
	case 0:
	case 1:
		CycDemoBGEffect(0);
		break;
	case 2:
	case 3:
		CycDemoBGEffect(1);
		break;
	case 4:
		CycDemoBGEffect(2);
		break;
	}
}





/********************************************************************/
/*                                                                  */
/*				�w�i������Ԑݒ�i�n�a�i�܂ށj						*/
/*                                                                  */
/********************************************************************/
static	void	EDdemoBackGroundInit(u8 scene,u8 id)
{
	switch(scene){
	case 0:
		//�C�i���j
		ActWork[TaskTable[id].work[ED_T1_ACT1]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].x = 240+32;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].x = 240+32;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].work[0] = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].work[0] = 0;
		TaskTable[id].work[ED_T1_SCRTASK] = CycDemoBGScrollSet(0,0x2000,0x0020,0x0008);
		break;
	case 1:
		//�C�i�[�j
		ActWork[TaskTable[id].work[ED_T1_ACT1]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].x = 120;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].x = 240+32;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].work[0] = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].work[0] = 0;
		TaskTable[id].work[ED_T1_SCRTASK] = CycDemoBGScrollSet(0,0x2000,0x0020,0x0008);
		break;
	case 2:
		//�R�i�[�j
		ActWork[TaskTable[id].work[ED_T1_ACT1]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].x = 120;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].x = 240+32;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].work[0] = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].work[0] = 0;
		TaskTable[id].work[ED_T1_SCRTASK] = CycDemoBGScrollSet(1,0x2000,0x0200,0x0008);
		break;
	case 3:
		//�R�i��j
		ActWork[TaskTable[id].work[ED_T1_ACT1]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].x = 120;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].x = -32;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].work[0] = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].work[0] = 0;
		TaskTable[id].work[ED_T1_SCRTASK] = CycDemoBGScrollSet(1,0x2000,0x0200,0x0008);
		break;
	case 4:
		//�X�i��j
		ActWork[TaskTable[id].work[ED_T1_ACT1]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].banish = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].x = 120-32;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].x = 120+32;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].y = 80-34;
		ActWork[TaskTable[id].work[ED_T1_ACT1]].work[0] = 0;
		ActWork[TaskTable[id].work[ED_T1_ACT2]].work[0] = 0;
		TaskTable[id].work[ED_T1_SCRTASK] = CycDemoBGScrollSet(2,0x2000,0x0200,0x0008);
		break;
	}
	TaskTable[id].work[ED_T1_EFFTASK] = AddTask(BGPaletteEffectTask, 0);
	TaskTable[TaskTable[id].work[ED_T1_EFFTASK]].work[ED_T4_SCENE] = scene;

	TaskTable[id].work[ED_T1_EVENTTASK] = AddTask(OBJeventTask, 0);
	TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[ED_T3_SEQ] = 0;
	TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[ED_T3_MAINTASK] = id;
	TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[ED_T3_ACT1] 
		= TaskTable[id].work[ED_T1_ACT1];
	TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[ED_T3_ACT2] 
		= TaskTable[id].work[ED_T1_ACT2];
	TaskTable[TaskTable[id].work[ED_T1_EVENTTASK]].work[ED_T3_WAIT] = 0;
}

/********************************************************************/
/*                                                                  */
/*				�w�i�ݒ�i�n�a�i�܂ށj���������̂ŕ���				*/
/*                                                                  */
/********************************************************************/
static	u8		EDdemoBackGroundSet(u8 scene,u8 id)
{
	u8	actnum;

	switch(sys.InitSeqWork){
	default:
	case 0:
		//���W�X�^������
		*(vu16 *)REG_DISPCNT	= 0;
		*(vu16 *)REG_BG3HOFS	= 8;
		*(vu16 *)REG_BG3VOFS	= 0;
		*(vu16 *)REG_BG2HOFS	= 0;
		*(vu16 *)REG_BG2VOFS	= 0;
		*(vu16 *)REG_BG1HOFS	= 0;
		*(vu16 *)REG_BG1VOFS	= 0;
		*(vu16 *)REG_BLDCNT		= 0;
		*(vu16 *)REG_BLDALPHA	= 0;

		//���[�e�B���e�B�[������
		InitActor();					// ����������
		ObjPalManInit();				// ��޼ު��گ��Ȱ�ެ������
		sys.InitSeqWork = 1;
		break;
	case 1:
		//�a�f�f�[�^�]��
		cyc_demo_y = 34;
		cyc_demo_dy = 0;
		CycDemoEndBGSet(scene);
		sys.InitSeqWork++;
		break;
	case 2:
		//�n�a�i�f�[�^�]��
		if(!MyData.my_sex){
			//�j
			DecordCellSet((CellData*)CycDemoOBJcellBoy);
			DecordCellSet((CellData*)CycDemoOBJcellGirlEnd);
			DecordCellSet((CellData*)CycDemoOBJcellJitensya);
			ObjPalSets((PalData*)CycDemoOBJpal);

			actnum = CycDemoBoySet(120,80-34);
			TaskTable[id].work[ED_T1_ACT1] = actnum;
			ActWork[actnum].move = EDdemoJikiActMove;
			ActWork[actnum].anm_tbl = EDdemoJikiAnmTbl;
			actnum = CycDemoGirlSet(240+32,80-34);
			TaskTable[id].work[ED_T1_ACT2] = actnum;
			ActWork[actnum].move = EDdemoSuportActMove;
			ActWork[actnum].anm_tbl = EDdemoSuportAnmTbl;
		}else{
			//��
			DecordCellSet((CellData*)CycDemoOBJcellGirl);
			DecordCellSet((CellData*)CycDemoOBJcellBoyEnd);
			DecordCellSet((CellData*)CycDemoOBJcellJitensya);
			ObjPalSets((PalData*)CycDemoOBJpal);
	
			actnum = CycDemoGirlSet(120,80-34);
			TaskTable[id].work[ED_T1_ACT1] = actnum;
			ActWork[actnum].move = EDdemoJikiActMove;
			ActWork[actnum].anm_tbl = EDdemoJikiAnmTbl;
			actnum = CycDemoBoySet(240+32,80-34);
			TaskTable[id].work[ED_T1_ACT2] = actnum;
			ActWork[actnum].move = EDdemoSuportActMove;
			ActWork[actnum].anm_tbl = EDdemoSuportAnmTbl;
		}
		sys.InitSeqWork++;
		break;
	case 3:
		EDdemoBackGroundInit(scene,id);
		//�\�����W�X�^�ݒ�
		CycDemoEndRegSet(scene);

		sys.InitSeqWork = 0;
		return 1;
	}
	return 0;
}

/********************************************************************/
/*                                                                  */
/*				���^�X�N�I���i�X�^�b�t���[���^�X�N�����j			*/
/*		0 : �N���Ȃ��i��0 �̓��C���^�X�N�i���o�[�Ȃ̂ł��肦�Ȃ��j	*/
/*                                                                  */
/********************************************************************/
static	void	EDdemoBackTaskDel(u8 id)
{
	if(TaskTable[id].work[ED_T1_SCRTASK]){
		DelTask(TaskTable[id].work[ED_T1_SCRTASK]);
		TaskTable[id].work[ED_T1_SCRTASK] = 0;
	}
	if(TaskTable[id].work[ED_T1_EVENTTASK]){
		DelTask(TaskTable[id].work[ED_T1_EVENTTASK]);
		TaskTable[id].work[ED_T1_EVENTTASK] = 0;
	}
	if(TaskTable[id].work[ED_T1_EFFTASK]){
		DelTask(TaskTable[id].work[ED_T1_EFFTASK]);
		TaskTable[id].work[ED_T1_EFFTASK] = 0;
	}
	if(TaskTable[id].work[ED_T1_PCUTTASK]){
		DelTask(TaskTable[id].work[ED_T1_PCUTTASK]);
		TaskTable[id].work[ED_T1_PCUTTASK] = 0;
	}
	cyc_demo_actdel_sw = 1;
}










/********************************************************************/
/*                                                                  */
/*				���쌠�\��											*/
/*                                                                  */
extern	const u16 cp_light_Palette[16];
/********************************************************************/
void	AllRight_logoMake(u16 cgx_offs,u16 scr_offs,u16 pal_offs)
{
	u16	i,pal;

	LZ77UnCompVram((void*)allright_LZ,(void*)(BG_VRAM + cgx_offs));
	PaletteWorkSet(cp_light_Palette,pal_offs,0x20);

	pal = ((pal_offs/16)<<12);
	for(i=0;i<32*32;i++)	*(u16*)(BG_VRAM + scr_offs + i*2) = 0x01 + pal;
	for(i=0;i<21;i++)		*(u16*)(BG_VRAM + scr_offs + 7*64 + (i+4)*2) = 0x02 + i + pal;
	for(i=0;i<20;i++)		*(u16*)(BG_VRAM + scr_offs + 9*64 + (i+4)*2) = 0x17 + i + pal;
	for(i=0;i<22;i++)		*(u16*)(BG_VRAM + scr_offs +11*64 + (i+4)*2) = 0x2b + i + pal;
	for(i=0;i<12;i++)		*(u16*)(BG_VRAM + scr_offs +13*64 + (i+4)*2) = 0x41 + i + pal;
}

/********************************************************************/
/*                                                                  */
/*				�u�s�g�d�@�d�m�c�v�\��								*/
/*                                                                  */
//CGX COL �͕ʂɐݒ�i�����ł͒��쌠�\�����̂��̂��g�p�j
/********************************************************************/
static	u16	THEEND_logoDataGet(u8 dat)
{
	u16	ret_dat;

	ret_dat = (dat&0x3f) + 0x0050;

	if(dat == 0xff)	return 0x0001;	//spc
	if(dat & 0x80)	ret_dat |= 0x0800;
	if(dat & 0x40)	ret_dat |= 0x0400;
	return ret_dat;
}

static	void	THEEND_logoMakeSub(const u8* tbl,u8 x,u8 y,u16 scr_offs,u16 pal_offs)
{
	u8	xp,yp;
	u16	pal;

	pal = ((pal_offs/16)<<12);

	for(yp=0;yp<5;yp++){
		for(xp=0;xp<3;xp++){
			*(u16*)(BG_VRAM+scr_offs+(y+yp)*64+(x+xp)*2)
				= THEEND_logoDataGet(tbl[(yp*3)+xp]) + pal;
		}
	}
}

void	THEEND_logoMake(u16 scr_offs,u16 pal_offs)
{
	u16	i,pal;

	pal = ((pal_offs/16)<<12);
	for(i=0;i<32*32;i++)	*(u16*)(BG_VRAM + scr_offs + i*2) = 0x01 + pal;

	THEEND_logoMakeSub(TheEndDatTable1, 3,7,scr_offs,pal_offs);
	THEEND_logoMakeSub(TheEndDatTable2, 7,7,scr_offs,pal_offs);
	THEEND_logoMakeSub(TheEndDatTable3,11,7,scr_offs,pal_offs);
	THEEND_logoMakeSub(TheEndDatTable3,16,7,scr_offs,pal_offs);
	THEEND_logoMakeSub(TheEndDatTable4,20,7,scr_offs,pal_offs);
	THEEND_logoMakeSub(TheEndDatTable5,24,7,scr_offs,pal_offs);
}





/********************************************************************/
/*                                                                  */
/*				�n�a�i�R���g���[��									*/
/*                                                                  */
/********************************************************************/
/*------------------------------------------------------------------*/
/*				��l��												*/
/*------------------------------------------------------------------*/
static const actAnm			EDdemoJikiAnm0[] = {
{0x00,8,0,0},{0x40,8,0,0},{0x80,8,0,0},{0xc0,8,0,0},{ACT_ANMLOOP,0,0,0}
};
static const actAnm			EDdemoJikiAnm1[] = {
{0x00,4,0,0},{0x40,4,0,0},{0x80,4,0,0},{0xc0,4,0,0},{ACT_ANMLOOP,0,0,0}
};
static const actAnm			EDdemoJikiAnm2[] = {
{0x100,4,0,0},{0x140,4,0,0},{0x180,4,0,0},{ACT_ANMEND,0,0,0}
};
static const actAnm			EDdemoJikiAnm3[] = {
{0x180,30,0,0},{0x140,30,0,0},{0x100,30,0,0},{0x100,30,0,0},{ACT_ANMEND,0,0,0}
};
static const actAnm* const	EDdemoJikiAnmTbl[]={
EDdemoJikiAnm0,EDdemoJikiAnm1,EDdemoJikiAnm2,EDdemoJikiAnm3,
};

static void			EDdemoJikiActMove(actWork* aw)
{
	if(cyc_demo_actdel_sw){
		DelActor(aw);
		return;
	}
	switch(aw->work[0]){
	case 0:
		ActAnmChkChg(aw,0);
		break;
	case 1:
		ActAnmChkChg(aw,1);
		if(aw->x > -32)	aw->x-=1;
		break;
	case 2:
		ActAnmChkChg(aw,2);
		break;
	case 3:
		ActAnmChkChg(aw,3);
		break;
	case 4:
		ActAnmChkChg(aw,0);
		if(aw->x > 120)	aw->x-=1;
		break;
	case 5:
		ActAnmChkChg(aw,0);
		if(aw->x > -32)	aw->x-=1;
		break;
	}
}

/*------------------------------------------------------------------*/
/*				�T�|�[�g											*/
/*------------------------------------------------------------------*/
static const actAnm			EDdemoSuportAnm0[] = {
{0x00,8,0,0},{0x40,8,0,0},{0x80,8,0,0},{0xc0,8,0,0},{ACT_ANMLOOP,0,0,0}
};
static const actAnm			EDdemoSuportAnm1[] = {
{0x00,4,0,0},{0x40,4,0,0},{0x80,4,0,0},{0xc0,4,0,0},{ACT_ANMLOOP,0,0,0}
};
static const actAnm			EDdemoSuportAnm2[] = {
{0x00,4,0,0},{ACT_ANMEND,0,0,0}
};
static const actAnm* const	EDdemoSuportAnmTbl[]={
EDdemoSuportAnm0,EDdemoSuportAnm1,EDdemoSuportAnm2
};

static void			EDdemoSuportActMove(actWork* aw)
{
	if(cyc_demo_actdel_sw){
		DelActor(aw);
		return;
	}
	switch(aw->work[0]){
	case 0:
		aw->dy = 0;
		ActAnmChkChg(aw,0);
		break;
	case 1:
		if(aw->x > 200)	ActAnmChkChg(aw,1);
		else			ActAnmChkChg(aw,2);
		if(aw->x > -32)	aw->x-=2;
		aw->dy = -cyc_demo_dy;
		break;
	case 2:
		aw->work[7]++;	//�^�C�}�[

		ActAnmChkChg(aw,0);
		if(!(aw->work[7] & 0x0003))	aw->x++;
		break;
	case 3:
		ActAnmChkChg(aw,0);
		if(aw->x > -32)	aw->x-=1;
		break;
	}
}

/*------------------------------------------------------------------*/
/*				�|�P�����ݒ�										*/
extern	const u32	DendouDecordAdrs[];
extern	void		DendouPokeActHeaderMake(u16 palID,u8 setnum);
/*------------------------------------------------------------------*/
static void		EDdemoPokeActorMove(actWork* aw)
{
	if(cyc_demo_actdel_sw){
		DelActor(aw);
		return;
	}
	aw->work[7]++;	//�^�C�}�[

	switch(aw->work[0]){
	default:
	case 0:
		aw->oamData.AffineMode = 1;	//�A�t�B���}�l�[�W���[���N�������Ȃ�
		aw->oamData.AffineParamNo = aw->work[1];
		aw->work[2] = 0x10;			//�����g�k��
		affineWorkSet(aw->work[1],0x10000/aw->work[2],0,0,0x10000/aw->work[2]);
		aw->banish  = 0;

		aw->work[0] = 1;
		break;
	case 1:
		if(aw->work[2] < 0x100){
			aw->work[2]+=0x8;
			affineWorkSet(aw->work[1],0x10000/aw->work[2],0,0,0x10000/aw->work[2]);
		}else{
			aw->work[0]++;
		}
		switch(aw->work[1]){
		case 1:
			if(!(aw->work[7]&0x0003))	aw->y++;
			aw->x-=2;
			break;
		case 2:
			break;
		case 3:
			if(!(aw->work[7]&0x0003))	aw->y++;
			aw->x+=2;
			break;
		}
		break;
	case 2:
		if(aw->work[3]){		//�\���E�F�C�g�i�O���ݒ�j
			aw->work[3]--;
		}else{
			*(vu16 *)REG_BLDCNT		= 0x0f40;
			*(vu16 *)REG_BLDALPHA	= 0x0010;
			aw->oamData.ObjMode = 1;	//�������J�n
			aw->work[3] = 0x10;
			aw->work[0]++;
		}
		break;
	case 3:
		if(aw->work[3]){
			aw->work[3]--;
			*(vu16 *)REG_BLDALPHA	= ((0x10-aw->work[3])*0x100) + aw->work[3];
		}else{
			aw->banish = 1;
			aw->work[0] = 10;
		}
		break;
	case 10:
		*(vu16 *)REG_BLDCNT		= 0;
		*(vu16 *)REG_BLDALPHA	= 0;
		DelActor(aw);
		break;
	}
}

static	u16		EDdemoPokeActorSet(u16 pnum,s16 x,s16 y,u16 decord_p)
{
	u8	actnum,actnum2;
	u32	personal_rnd;

	pnum = PokeZukanNo2MonsNoGet(pnum);		//�S�����J���i���o�[

	switch(pnum){
	default:
		personal_rnd = 0;
		break;
	case MONSNO_PATTIIRU:
		personal_rnd = MyData.Zukan.PachiRand;
		break;
	case MONSNO_ANNOON:
		personal_rnd = MyData.Zukan.UnknownRand;
		break;
	}
	DecordPokeCellMakeRndFB( &PM2FObjDataCell[pnum],pm2fpara[pnum].size,pm2fpara[pnum].patcnt,
							(void *)0x2000000,(void *)DendouDecordAdrs[decord_p],
							pnum,personal_rnd,POKE_FRONT);
	DecordPaletteWork( (void*)PokePalGetPara(pnum,0,0x0000ffff),PA_OBJ0+(decord_p*16),0x20 );

	DendouPokeActHeaderMake( decord_p,decord_p );
	actnum = AddActor( &PokeActHeader, x, y, 0);
	ActWork[actnum].oamData.Pltt = decord_p;
	ActWork[actnum].oamData.Priority = 1;
	ActWork[actnum].work[1]	= decord_p + 1;	//�g��k���i���o�[�i�P�`�j
	ActWork[actnum].banish  = 1;
	ActWork[actnum].move  = EDdemoPokeActorMove;

	actnum2 = AddActor(&EDpokeWinActHeader,ActWork[actnum].x,ActWork[actnum].y,1);
	ActWork[actnum2].work[0] = actnum;
	ActAnmChkChg(&ActWork[actnum2],decord_p);

	return actnum;
}

/*------------------------------------------------------------------*/
/*				�|�P�����E�C���h�E�ݒ�								*/
/*------------------------------------------------------------------*/
static	const	CellData	EDpokeWinCell[] = {
	{ (&UserWork[0x1e000]),	0x1800	,CELLID_EDPOKEWIN },
	{ CELL_END, 0, 0 }
};
static	const	PalData		EDpokeWinPal[] = {
	{ (u16*)(&(UserWork[0x1f800])),PALID_EDPOKEWIN },
	{ OBJPAL_END, 0 },
};

//VPos,AffineMode,ObjMode,Mosaic,ColorMode,Shape,
//HPos,AffineParamNo,Size,CharNo,Priority,Pltt,(Affine)
static const ActOamData		EDpokeWinOam = {160,0,0,0,0,0,0,0,3,0,1,0,0};
static const actAnm			EDpokeWinAnm1[] = {{0x00,8,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm			EDpokeWinAnm2[] = {{0x40,8,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm			EDpokeWinAnm3[] = {{0x80,8,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm* const	EDpokeWinAnmTbl[]={EDpokeWinAnm1,EDpokeWinAnm2,EDpokeWinAnm3};

static void			EDpokeWinActMove(actWork* aw)
{
	if((ActWork[aw->work[0]].work[0] == 10)||(cyc_demo_actdel_sw)){
		DelActor(aw);
	}else{
		aw->banish					= ActWork[aw->work[0]].banish;
		aw->oamData.ObjMode			= ActWork[aw->work[0]].oamData.ObjMode;
		aw->oamData.AffineMode		= ActWork[aw->work[0]].oamData.AffineMode;
		aw->oamData.AffineParamNo	= ActWork[aw->work[0]].oamData.AffineParamNo;
		aw->x						= ActWork[aw->work[0]].x;
		aw->y						= ActWork[aw->work[0]].y;
	}
}

static const actHeader		EDpokeWinActHeader ={
CELLID_EDPOKEWIN,PALID_EDPOKEWIN,&EDpokeWinOam,
EDpokeWinAnmTbl,ACT_NTRANS,DummyActAffTbl,EDpokeWinActMove,
};





/********************************************************************/
/*                                                                  */
/*				�\���|�P�������X�g�f�[�^�擾						*/
/*    			���S���}�Ӄi���o�[                                  */
/********************************************************************/
static	void	EDPokeListDataGet(void)
{
	u16	i,rp,wp;
	u16	firstpoke;
	ED_POKECUT_WORK*	edpw;

	edpw = (ED_POKECUT_WORK*)(UserWork + 0x1c000);
	firstpoke = PokeMonsNo2ZukanNoGet(FirstPokemonNumGet(GetEventWorkValue(WK_FIRST_POKE)));

	//tmp �ɂ��܂����|�P�����f�[�^����ׁi�}�Ӕԍ����j�A�v�f����tmpwork[0]��
	for(i=1,wp=0;i<POKE_NUM_MAX;i++){
		if(ZukanCheck(i,Z_GET_CHK)){
			edpw->tmp[wp] = i;
			wp++;
		}
	}
	for(i=wp;i<POKE_NUM_MAX;i++)	edpw->tmp[i] = 0;	//������
	edpw->tmpwork[0] = wp;

	//���X�g�ő吔�ݒ�
	if(edpw->tmpwork[0] < PUT_POKE_MAX){
		edpw->poke_max = edpw->tmpwork[0];
	}else{
		edpw->poke_max = PUT_POKE_MAX;
	}

	//tmp ���� poke �ց@�f�[�^�ړ�
	wp = 0;
	do{
		rp = pp_rand()%edpw->tmpwork[0];
		edpw->poke[wp] = edpw->tmp[rp];
		wp++;
		edpw->tmp[rp]  = 0;	//�v�f�N���A
		//�v�f��������
		edpw->tmpwork[0]--;
		//�����ܗv�f�𖄂߂�
		if(rp != edpw->tmpwork[0]){
			edpw->tmp[rp]  = edpw->tmp[edpw->tmpwork[0]];
			edpw->tmp[edpw->tmpwork[0]] = 0;
		}
	}while((edpw->tmpwork[0]) && (wp < PUT_POKE_MAX));

	if(edpw->poke_max < PUT_POKE_MAX){
		//�ő吔���\������菭�Ȃ��ꍇ�A���[�v�ɂ�鐅����
		for(wp=edpw->poke_max,rp=0;wp<PUT_POKE_MAX;wp++){
			edpw->poke[wp] = edpw->poke[rp];
			rp++;
			if(rp == edpw->poke_max)	rp = 0;
		}
		//�ŏ��ɂ��܂����|�P�����f�[�^���Ō�̃|�P�����f�[�^�ɐݒ�
		edpw->poke[PUT_POKE_MAX-1] = firstpoke;
	}else{
		//�ŏ��ɂ��܂����|�P�����f�[�^���Ō�̃|�P�����f�[�^�Ɠ���ւ�or�����ݒ�
		i = 0;
		while((edpw->poke[i] != firstpoke)&&(i < PUT_POKE_MAX)){
			i++;
		}
		if(i < (edpw->poke_max-1)){
			//���łɏo�Ă��Ă���̂œ���ւ�
			edpw->poke[i] = edpw->poke[PUT_POKE_MAX-1];
			edpw->poke[PUT_POKE_MAX-1] = firstpoke;
		}else{
			//�o�Ă��Ă��Ȃ��̂ŋ����ݒ�
			edpw->poke[PUT_POKE_MAX-1] = firstpoke;
		}
	}
	edpw->poke_max = PUT_POKE_MAX;
}










