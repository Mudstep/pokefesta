//=========================================================================
//	
//	�\����(�}�ӊ����C�x���g)
//
//=========================================================================
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "task.h"
#include "print.h"
#include "poketool.h"
#include "decord.h"
#include "objdata.h"
#include "menu.h"
#include "window.h"
#include "zukan.h"
#include "fld_eff.h"
#include "calctool.h"
#include "ev_flag.h"
#include "mus_tool.h"
#include "monsno.def"
#include "laster.h"

#include "hyousyou.dat"

void	HyousyouEvInit(void);
void	HyousyouEvMain(void);

extern	void	FieldNormalRecover(void);

static	void	HyousyouEvInitTask(u8 id);
static	void	HyousyouEvSeq1Task(u8 id);
static	void	HyousyouEvExitTask(u8 id);

static	void		HyousyouScreenMake(void);

#define	COLCHG_DATASIZE	(6)
#define	JPN_NAMESIZE	(5)
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
/*				������												*/
/*                                                                  */
/********************************************************************/
void	HyousyouEvInit(void)
{
	u8	taskID;

	SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

   	*(vu16 *)REG_DISPCNT	= 0;
   	*(vu16 *)REG_BG3CNT		= 0;
   	*(vu16 *)REG_BG2CNT		= 0;
   	*(vu16 *)REG_BG1CNT		= 0;
   	*(vu16 *)REG_BG0CNT		= 0;

	*(vu16 *)REG_BG3HOFS	= 0;
	*(vu16 *)REG_BG3VOFS	= 0;
	*(vu16 *)REG_BG2HOFS	= 0;
	*(vu16 *)REG_BG2VOFS	= 0;
   	*(vu16 *)REG_BG1HOFS	= 0;
   	*(vu16 *)REG_BG1VOFS	= 0; 
   	*(vu16 *)REG_BG0HOFS	= 0;
   	*(vu16 *)REG_BG0VOFS	= 0; 

    DmaClear(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
    DmaClear(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
    DmaClear(3, 0, PLTT, PLTT_SIZE,           16);	// ��گ�  ������

	LZ77UnCompVram((void*)syoujou_ch_LZ,(void*)(BG_VRAM+0x0000));//BG  CGX�]��
	LZ77UnCompVram((void*)syoujou_sc_LZ,(void*)(BG_VRAM+0x3000));//BG2 MAP�]��

	LasterInit();
	InitTask();										// ���  ������
	InitActor();									// ���� ������
	PaletteAnimeInit();
	ObjPalManInit();								// ��޼ު��گ��Ȱ�ެ������

	PaletteWorkSet(syoujou_Palette,PA_BG0,0x40);		//BG2&BG3 COL�]��

	PrintScreenInit(DEFAULT_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);

	HyousyouScreenMake();

	PaletteFadeReq(0xffffffff,0,16,0,0);

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
    *(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(HyousyouEvMain);

   	*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 0;

	*(vu16 *)REG_BG3CNT = BG_COLOR_16 | BG_SCREEN_SIZE_1 | BG_PRIORITY_3
						| 6 << BG_SCREEN_BASE_SHIFT | 0 << BG_CHAR_BASE_SHIFT;
	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG3_ON					// BG3�L��
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_OBJ_ON;

	taskID = AddTask(HyousyouEvInitTask, 0);
}


/********************************************************************/
/*                                                                  */
/*				Ҳ�													*/
/*                                                                  */
/********************************************************************/
void	HyousyouEvMain(void)
{
		MainTask();
		JumpActor();
		SetActor();
		PaletteAnime();
}


/********************************************************************/
/*                                                                  */
/*				���C������											*/
/*                                                                  */
/********************************************************************/
/*==================================================================*/
//		������
/*==================================================================*/
static	void	HyousyouEvInitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	TaskTable[id].TaskAdrs = HyousyouEvSeq1Task;
}

/*==================================================================*/
//		�L�[����
/*==================================================================*/
static	void	HyousyouEvSeq1Task(u8 id)
{
	if(sys.Trg & (A_BUTTON|B_BUTTON)){
		PaletteFadeReq(0xffffffff,0,0,16,0);

		TaskTable[id].TaskAdrs = HyousyouEvExitTask;
		return;
	}
}

/*==================================================================*/
//		�I��
/*==================================================================*/
static	void	HyousyouEvExitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	DelTask(id);
	MainProcChange(FieldNormalRecover);
}

/********************************************************************/
/*                                                                  */
/*				�X�N���[���쐬										*/
/*                                                                  */
/********************************************************************/
static	void	HyousyouScreenMake(void)
{
	u8	i,str[COLCHG_DATASIZE+JPN_NAMESIZE+EOM_SIZE];

	NMenuMsgWrite(hyousyou_msg1,8,2);
	//���O�̕\��
	PM_strcpy(str,colchg_msg);
	for(i=0;i<JPN_NAMESIZE;i++)	str[i+COLCHG_DATASIZE] = MyData.my_name[i];
	str[i+COLCHG_DATASIZE] = EOM_;
	NMenuMsgWrite(str,14,2);
	NMenuMsgWrite(hyousyou_msg2,19,2);

	NMenuMsgWrite(hyousyou_msg5,8,6);
	if(ZukanHyousyouCheckZenkoku()){
		*(vu16 *)REG_BG3HOFS	= 0x0100;	//�S���p�X�N���[��
		NMenuMsgWrite(hyousyou_msg4,8,6);
	}else{
		*(vu16 *)REG_BG3HOFS	= 0x0000;	//�z�E�G���p�X�N���[��
		NMenuMsgWrite(hyousyou_msg3,8,6);
	}
	NMenuMsgWrite(hyousyou_msg6,14,16);
}



