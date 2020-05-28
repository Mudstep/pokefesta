//******************************************************************
//          title.c                                                  
//                                                                  
//          Copyright (C) 2001 GAME FREAK inc.
//******************************************************************
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "poketool.h"			//add soga 2001.07.27
#include "mail.h"				//add matsuda 2001.08.29(��)
#include "mus_tool.h"
#include "task.h"
#include "decord.h"
#include "calctool.h"
#include "laster.h"
#include "fight.h"
#include "ev_flag.h"

#include "title.dat"

const u16 TitleColEffTbl[0x20] = {
0x0010,0x0110,0x0210,0x0310,0x0410,0x0510,0x0610,0x0710,
0x0810,0x0910,0x0A10,0x0B10,0x0C10,0x0D10,0x0E10,0x0F10,
0x100F,0x100E,0x100D,0x100C,0x100B,0x100A,0x1009,0x1008,
0x1007,0x1006,0x1005,0x1004,0x1003,0x1002,0x1001,0x1000,
};


extern	u16 pp_rand(void);
extern void DebugFieldInit(void);
#ifdef	PM_DEBUG
extern void testMainInit(void);
#endif
extern void	OPdemoInitReturn(void);
extern void	BackUpDelete(void);
extern void TimeInputInit(void);
extern void	BGAffineCalcSet(u16 x,u16 y,u16 ratio,u16 theta);

void	TitleInit(void);
void	TitleMain(void);
void	GoNewGame(void);
#ifdef	PM_DEBUG
void	GoDebugMenu(void);
#endif
void	ReturnTitle(void);
void	GoBackUpDelete(void);
void	GoChangeTimer(void);

u16			TitleWork[4]={0};

#define	TIT_TIMER	(0)
#define	TIT_SKIP_F	(1)
#define	TIT_SCR2_X	(2)
#define	TIT_SCR2_Y	(3)
#define	TIT_SCR1_Y	(4)

#define	TIT_OB_TIMER	(0)
#define	TIT_OB_TASK		(1)

#define	TIT_VERSION_X	(192)
#define	TIT_VERSION_Y	(48)
#define	TIT_VEROFFS1	(-0x10)
#define	TIT_VEROFFS2	(0x20)
/********************************************************************/
/*                                                                  */
/*				�n�a�i�A�N�^�[										*/
/*                                                                  */
/********************************************************************/
/*------------------------------------------------------------------*/
/*				�u�d�q�}�[�N										*/
/*------------------------------------------------------------------*/
#define	CELLID_TITLE_VER	(1000)
#define	PALID_TITLE_VER		(1000)
//VPos,AffineMode,ObjMode,Mosaic,ColorMode,Shape,
//HPos,AffineParamNo,Size,CharNo,Priority,Pltt,(Affine)
static const ActOamData		TitleVerOam1 = {160,0,0,0,1,1,0,0,3,0,0,0,0};
static const ActOamData		TitleVerOam2 = {160,0,0,0,1,0,0,0,2,0,0,0,0};
static const actAnm			TitleVerAnm1[] = {{0x00,30,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm			TitleVerAnm2[] = {{0x40,30,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm* const	TitleVerAnmTbl1[]={TitleVerAnm1};
static const actAnm* const	TitleVerAnmTbl2[]={TitleVerAnm2};

static void					TitleVerActMove1(actWork* aw)
{
	if(TaskTable[aw->work[TIT_OB_TASK]].work[TIT_SKIP_F]){
		aw->oamData.ObjMode= 0;
		aw->x = TIT_VERSION_X + TIT_VEROFFS1;
	}else{
		if(aw->x != (TIT_VERSION_X + TIT_VEROFFS1)) aw->x+=2;
		if(aw->work[TIT_OB_TIMER])	aw->work[TIT_OB_TIMER]--;
		*(vu16 *)REG_BLDALPHA = TitleColEffTbl[aw->work[TIT_OB_TIMER]/2];
	}
}
static void					TitleVerActMove2(actWork* aw)
{
	if(TaskTable[aw->work[TIT_OB_TASK]].work[TIT_SKIP_F]){
		aw->oamData.ObjMode= 0;
		aw->x = TIT_VERSION_X + TIT_VEROFFS2;
	}else{
		if(aw->x != (TIT_VERSION_X + TIT_VEROFFS2)) aw->x+=2;
	}
}
static const actHeader		TitleVerActHeader1 ={
CELLID_TITLE_VER,PALID_TITLE_VER,&TitleVerOam1,
TitleVerAnmTbl1,ACT_NTRANS,DummyActAffTbl,TitleVerActMove1,
};
static const actHeader		TitleVerActHeader2 ={
CELLID_TITLE_VER,PALID_TITLE_VER,&TitleVerOam2,
TitleVerAnmTbl2,ACT_NTRANS,DummyActAffTbl,TitleVerActMove2,
};

/*------------------------------------------------------------------*/
/*				�Z�����p���b�g�ݒ�f�[�^							*/
const	CellData	TitleOBJcell1[] = {
	{ titlelogo_obj_LZ,	0x1000	,CELLID_TITLE_VER },
	{ CELL_END, 0, 0 }
};

/*------------------------------------------------------------------*/
//		�f�`�l�d�e�q�d�`�j���S�@���@�o�t�r�g�@�r�s�`�q�s�@�j�d�x
/*------------------------------------------------------------------*/
#define	CELLID_TITLE_INFO	(1001)
#define	PALID_TITLE_INFO	(1001)
//VPos,AffineMode,ObjMode,Mosaic,ColorMode,Shape,
//HPos,AffineParamNo,Size,CharNo,Priority,Pltt,(Affine)
static const ActOamData		TitleInfoOam1 = {160,0,0,0,0,1,0,0,1,0,0,0,0};
static const actAnm			TitleInfoAnm1[] = {{0x01,4,0,0},{ACT_ANMEND,0,0,0}};//PUSH
static const actAnm			TitleInfoAnm2[] = {{0x05,4,0,0},{ACT_ANMEND,0,0,0}};// STA
static const actAnm			TitleInfoAnm3[] = {{0x09,4,0,0},{ACT_ANMEND,0,0,0}};//RT B 
static const actAnm			TitleInfoAnm4[] = {{0x0d,4,0,0},{ACT_ANMEND,0,0,0}};//UTTO 
static const actAnm			TitleInfoAnm5[] = {{0x11,4,0,0},{ACT_ANMEND,0,0,0}};//N
static const actAnm			TitleInfoAnm6[] = {{0x15,4,0,0},{ACT_ANMEND,0,0,0}};//(c)200
static const actAnm			TitleInfoAnm7[] = {{0x19,4,0,0},{ACT_ANMEND,0,0,0}};//2 GA
static const actAnm			TitleInfoAnm8[] = {{0x1d,4,0,0},{ACT_ANMEND,0,0,0}};//MEFR
static const actAnm			TitleInfoAnm9[] = {{0x21,4,0,0},{ACT_ANMEND,0,0,0}};//EAK
static const actAnm			TitleInfoAnm10[]= {{0x25,4,0,0},{ACT_ANMEND,0,0,0}};//inc.
static const actAnm* const	TitleInfoAnmTbl1[] = {
TitleInfoAnm1,TitleInfoAnm2,TitleInfoAnm3,TitleInfoAnm4,TitleInfoAnm5,
TitleInfoAnm6,TitleInfoAnm7,TitleInfoAnm8,TitleInfoAnm9,TitleInfoAnm10,
};

static void					TitleInfoActMove1(actWork* aw)
{
	//�\���i����^�C�v�P(work[0]=1)�͓_�Łj
	if(aw->work[0] == 1){
		aw->work[1]++;	//�^�C�}�[
		if(aw->work[1] & 0x0010)	aw->banish = 0;
		else						aw->banish = 1;
	}else{
		aw->banish = 0;
	}
}
static const actHeader		TitleInfoActHeader1 ={
CELLID_TITLE_INFO,PALID_TITLE_INFO,&TitleInfoOam1,
TitleInfoAnmTbl1,ACT_NTRANS,DummyActAffTbl,TitleInfoActMove1,
};

/*------------------------------------------------------------------*/
/*				�A�N�^�[�ݒ�										*/
static void			TitleInfoActSetKeyPart(s16 x,s16 y)
{
	u8	i,actnum;

	x -= 32*2;
	for(i=0;i<5;i++,x+=32){
		actnum = AddActor(&TitleInfoActHeader1,x,y,0);
		ActAnmChg(&ActWork[actnum],i);
		ActWork[actnum].work[0] = 1;
	}
}

static void			TitleInfoActSetGF_Part(s16 x,s16 y)
{
	u8	i,actnum;

	x -= 32*2;
	for(i=0;i<5;i++,x+=32){
		actnum = AddActor(&TitleInfoActHeader1,x,y,0);
		ActAnmChg(&ActWork[actnum],i+5);
	}
}

/*------------------------------------------------------------------*/
/*				�Z�����p���b�g�ݒ�f�[�^							*/
const	CellData	TitleInfoOBJCell[] = {
	{ tit_info_LZ,	0x0520	,CELLID_TITLE_INFO },
	{ CELL_END, 0, 0 }
};
const	PalData		TitleInfoOBJPal[] = {
	{ tit_info_Palette	,PALID_TITLE_INFO },
	{ OBJPAL_END, 0 },
};

/*------------------------------------------------------------------*/
//		�t���b�V���G�t�F�N�g
/*------------------------------------------------------------------*/
#define	CELLID_TITLE_FLASH	(1002)
//#define	PALID_TITLE_FLASH	(1002)//�p���b�g�́AINFO�Ɠ���
//VPos,AffineMode,ObjMode,Mosaic,ColorMode,Shape,
//HPos,AffineParamNo,Size,CharNo,Priority,Pltt,(Affine)
static const ActOamData		TitleFlashOam = {160,0,0,0,0,0,0,0,3,0,0,0,0};
static const actAnm			TitleFlashAnm[] = {{0x00,4,0,0},{ACT_ANMEND,0,0,0}};//PUSH
static const actAnm* const	TitleFlashAnmTbl[] = {TitleFlashAnm};

static void		TitleFlashActMove(actWork* aw)
{
	u16	poffs,pal;

	if(aw->x <240+32){
		if(aw->work[0]){
			if(aw->x < 120){
				if(aw->work[1] < 0x1f)	aw->work[1]++;
				if(aw->work[1] < 0x1f)	aw->work[1]++;
			}else{
				if(aw->work[1])			aw->work[1]--;
				if(aw->work[1])			aw->work[1]--;
			}
			poffs = aw->work[1] & 0x1f;
			pal = (poffs<<10)+(poffs<<5)+(poffs);
			PaletteWorkTrans[0x0000] = pal;
		}
		aw->x += 4;
	}else{
		PaletteWorkTrans[0x0000] = 0x0000;
		DelActor(aw);
	}
}

static const actHeader		TitleFlashActHeader ={
CELLID_TITLE_FLASH,PALID_TITLE_INFO,&TitleFlashOam,
TitleFlashAnmTbl,ACT_NTRANS,DummyActAffTbl,TitleFlashActMove,
};

static void		TitleFlashActSet(u8 mode)
{
	u8	actnum;

	actnum = AddActor(&TitleFlashActHeader,0,0x44,0);
	ActWork[actnum].oamData.ObjMode = 2;
	ActWork[actnum].work[0] = mode;
}

const	CellData	TitleFlashOBJCell[] = {
	{ titflash_LZ,	0x0800	,CELLID_TITLE_FLASH },
	{ CELL_END, 0, 0 }
};






/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static void VInterFunc( void )
{
	LasterVblank();
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
	*(vu16 *)REG_BG1VOFS = ScrY1;
}

/********************************************************************/
/*				������												*/
/********************************************************************/
static	void	TitleTask0(u8 id);
static	void	TitleTask1(u8 id);
static	void	TitleTask2(u8 id);
static	void	TitleBGPaletteAnm(u8 timer);

void	TitleInit(void)
{
	u8	taskID;

	switch(sys.InitSeqWork){
	default:
	case 0:
		SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

		*(vu16 *)REG_BLDCNT		= 0;	//�������ݒ�
		*(vu16 *)REG_BLDALPHA	= 0;
		*(vu16 *)REG_BLDY		= 0;
		*(vu16 *)BG_PLTT		= 0x7fff;
		*(vu16 *)REG_DISPCNT	= 0;
		*(vu16 *)REG_BG2CNT		= 0;
		*(vu16 *)REG_BG1CNT		= 0;
		*(vu16 *)REG_BG0CNT		= 0;

		*(vu16 *)REG_BG2HOFS	= 0;
		*(vu16 *)REG_BG2VOFS	= 0;
		*(vu16 *)REG_BG1HOFS	= 0;
		*(vu16 *)REG_BG1VOFS	= 0; 
		*(vu16 *)REG_BG0HOFS	= 0;
		*(vu16 *)REG_BG0VOFS	= 0; 

		DmaClear(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
		DmaClear(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
		DmaClear(3, 0, PLTT+2, PLTT_SIZE-2,       16);	// ��گ�  ������(BackGround>��)

		PaletteAnimeInit();

		sys.InitSeqWork = 1;
		break;
	case 1:
		LZ77UnCompVram((void*)titlelogo_ch_LZ,(void*)(BG_VRAM+0x0000));	//BG CGX�]���i���S��(c)�j
		LZ77UnCompVram((void*)titlelogo_sc_LZ,(void*)(BG_VRAM+0x4800));	//BG SCR�]���i���S�j
		PaletteWorkSet(titlelogo_Palette,PA_BG0,0x120);					//BG COL�]���i���S�j
#if (PM_VERSION == VERSION_SAPPHIRE)
		LZ77UnCompVram((void*)tits_ch_LZ,(void*)(BG_VRAM + 0x8000));	//BG CGX�]���i�w�i�j
		LZ77UnCompVram((void*)tits_sc_LZ,(void*)(BG_VRAM + 0xc000));	//BG SCR�]���i�w�i�P�j
		LZ77UnCompVram((void*)titsb_sc_LZ,(void*)(BG_VRAM + 0xc800));	//BG SCR�]���i�w�i�Q�j
		PaletteWorkSet(tit_saph_Palette,PA_BG14,0x40);					//BG COL�]���i�w�i�j
#elif (PM_VERSION == VERSION_RUBY)
		LZ77UnCompVram((void*)titr_ch_LZ,(void*)(BG_VRAM + 0x8000));	//BG CGX�]���i�w�i�j
		LZ77UnCompVram((void*)titr_sc_LZ,(void*)(BG_VRAM + 0xc000));	//BG SCR�]���i�w�i�P�j
		LZ77UnCompVram((void*)titrb_sc_LZ,(void*)(BG_VRAM + 0xc800));	//BG SCR�]���i�w�i�Q�j
		PaletteWorkSet(tit_ruby_Palette,PA_BG14,0x40);					//BG COL�]���i�w�i�j
#endif
		LasterInit();									// HDMA������
		InitTask();										// ����������
		InitActor();									// ����������
		ObjPalManInit();								// ��޼ު��گ��Ȱ�ެ������
		ObjPalManStart = 9;								// ��گ��Ȱ�ެNo�J�n
		DecordCellSet((CellData*)TitleOBJcell1);
		DecordCellSet((CellData*)TitleInfoOBJCell);
		DecordCellSet((CellData*)TitleFlashOBJCell);
		PaletteWorkSet(titlelogo_Palette,PA_OBJ0,0x120);// ��گ��Ȱ�ެ�Ǘ��O
		ObjPalSet((PalData*)TitleInfoOBJPal);			// ��گ��Ȱ�ެ�Ǘ���

		sys.InitSeqWork = 2;
		break;
	case 2:
		taskID = AddTask(TitleTask0, 0);
		TaskTable[taskID].work[TIT_TIMER] = 0x100;
		TaskTable[taskID].work[TIT_SKIP_F] = 0;	//�X�L�b�v�t���O
		TaskTable[taskID].work[TIT_SCR2_X] = -0x10;	//�X�N���[�����[�N
		TaskTable[taskID].work[TIT_SCR2_Y] = -0x20;	//�X�N���[�����[�N

		sys.InitSeqWork = 3;
		break;
	case 3:
		PaletteFadeReq(0xffffffff,1,16,0,0xffff);
		SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)

		sys.InitSeqWork = 4;
		break;
	case 4:
		BGAffineCalcSet(120,80,0x100,0);
		*(vu32 *)REG_BG2X_L	= (-0x10)<<8;
		*(vu32 *)REG_BG2Y_L	= (-0x20)<<8;

		*(vu16 *)REG_WIN0H		= 0;
		*(vu16 *)REG_WIN0V		= 0;
		*(vu16 *)REG_WIN1H		= 0;
		*(vu16 *)REG_WIN1V		= 0;
		*(vu16 *)REG_WININ		= 0x1f1f;
		*(vu16 *)REG_WINOUT		= 0x3f1f;
		*(vu16 *)REG_BLDCNT		= 0x0084;	//�������ݒ�
		*(vu16 *)REG_BLDALPHA	= 0;
		*(vu16 *)REG_BLDY		= 0x0008;

		*(vu16 *)REG_BG0CNT = BG_COLOR_16					//16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_3					//BG�D�揇��
							| 24 << BG_SCREEN_BASE_SHIFT		//�X�N���[���x�[�X�u���b�N
							| 2 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
		*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_2					//BG�D�揇��
							| 25 << BG_SCREEN_BASE_SHIFT		//�X�N���[���x�[�X�u���b�N
							| 2 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
		*(vu16 *)REG_BG2CNT = BG_COLOR_256					//16�F�I��
							| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
							| BG_PRIORITY_1					//BG�D�揇��
							| 9 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
							| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

		REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
		*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

		*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
							|	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							|	DISP_BG2_ON					// BG1�L��
							|	DISP_WIN0_ON
							|	DISP_OBJWIN_ON
							|	DISP_OBJ_ON;
		m4aSongNumStart(MUS_TITLE3);

		sys.InitSeqWork = 5;
		break;
	case 5:
		if(PaletteAnime())	break;

		TitleFlashActSet(0);
		LasterScrollSet(0,160,4,4,0,LS_BG1H,LS_HOSEI_ON);
		MainProcChange(TitleMain);
		break;
	}
}

/********************************************************************/
/*				Ҳ�													*/
/********************************************************************/
void	TitleMain(void)
{
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

static	void	TitleTask0(u8 id)
{
	u8	actnum;
	u16	timer;

	if((sys.Trg & (A_BUTTON|B_BUTTON|START_BUTTON|SELECT_BUTTON))
			||(TaskTable[id].work[TIT_SKIP_F])){
		TaskTable[id].work[TIT_SKIP_F] = 1;	//�X�L�b�v�t���O
		TaskTable[id].work[TIT_TIMER] = 0;
	}
	if(TaskTable[id].work[TIT_TIMER]){
		timer = TaskTable[id].work[TIT_TIMER];
		if((timer == 0x00a0)||(timer == 0x040)){
			TitleFlashActSet(1);
		}
		TaskTable[id].work[TIT_TIMER]--;
	}else{
		*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
							|	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							|	DISP_BG2_ON					// BG1�L��
							|	DISP_OBJ_ON;
		*(vu16 *)REG_WININ		= 0;
		*(vu16 *)REG_WINOUT		= 0;

		*(vu16 *)REG_BLDCNT   = 0x3f50;	//�������ݒ�
		*(vu16 *)REG_BLDALPHA = 0x001f;
		*(vu16 *)REG_BLDY	  = 0;
		actnum = AddActor(&TitleVerActHeader1,TIT_VERSION_X+TIT_VEROFFS1 - 0x80,TIT_VERSION_Y,0);
		ActWork[actnum].work[TIT_OB_TIMER] = 0x40;
		ActWork[actnum].work[1] = id;
		actnum = AddActor(&TitleVerActHeader2,TIT_VERSION_X+TIT_VEROFFS2 - 0x80,TIT_VERSION_Y,0);
		ActWork[actnum].work[1] = id;

		TaskTable[id].work[TIT_TIMER] = 0x90;
		TaskTable[id].TaskAdrs	= TitleTask1;
	}
}

static	void	TitleTask1(u8 id)
{
	if((sys.Trg & (A_BUTTON|B_BUTTON|START_BUTTON|SELECT_BUTTON))
			||(TaskTable[id].work[TIT_SKIP_F])){
		TaskTable[id].work[TIT_SKIP_F] = 1;	//�X�L�b�v�t���O
		TaskTable[id].work[TIT_TIMER] = 0;
	}
	if(TaskTable[id].work[TIT_TIMER]){
		TaskTable[id].work[TIT_TIMER]--;
	}else{
		TaskTable[id].work[TIT_SKIP_F] = 1;	//�X�L�b�v�t���O
		*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
		                  |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG2_ON					// BG2�L��
						  | DISP_BG1_ON					// BG1�L��
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_OBJ_ON;
		TitleInfoActSetKeyPart(128,108);
		TitleInfoActSetGF_Part(128,148);
		TaskTable[id].work[TIT_SCR1_Y] = 0;
		TaskTable[id].TaskAdrs	= TitleTask2;
	}
	if((!(TaskTable[id].work[TIT_TIMER]&0x0003))&&(TaskTable[id].work[TIT_SCR2_X])){
		TaskTable[id].work[TIT_SCR2_X] +=1;
	}
	if((!(TaskTable[id].work[TIT_TIMER]&0x0001))&&(TaskTable[id].work[TIT_SCR2_Y])){
		TaskTable[id].work[TIT_SCR2_Y] +=1;
	}
	*(vu32 *)REG_BG2X_L	= (s16)(TaskTable[id].work[TIT_SCR2_X])<<8;
	*(vu32 *)REG_BG2Y_L	= (s16)(TaskTable[id].work[TIT_SCR2_Y])<<8;
}

static	void	TitleTask2(u8 id)
{
	*(vu16 *)REG_BLDCNT   = 0x2142;	//�������ݒ�
	*(vu16 *)REG_BLDALPHA = 0x1f0f;
	*(vu16 *)REG_BLDY	  = 0;

	if((sys.Trg & A_BUTTON)||(sys.Trg & START_BUTTON)){
		MusicFadeOutEnd(4);
		PaletteFadeReq(0xffffffff,0,0,16,0xffff);
		MainProcChange(GoNewGame);
		return;
	}
	if((sys.Cont&(B_BUTTON|U_KEY|SELECT_BUTTON)) == (B_BUTTON|U_KEY|SELECT_BUTTON)){
		MainProcChange(GoBackUpDelete);
	}
	if((sys.Cont&(B_BUTTON|L_KEY|SELECT_BUTTON)) == (B_BUTTON|L_KEY|SELECT_BUTTON) &&
			GameTimeSpecialFlagCheck() == TRUE){
		MusicFadeOutEnd(4);
		PaletteFadeReq(0xffffffff,0,0,16,0x0000);
		MainProcChange(GoChangeTimer);
		return;
	}
#ifdef	PM_DEBUG
	if(sys.Cont == SELECT_BUTTON){
		PaletteFadeReq(0xffffffff,0,0,16,0);
		MainProcChange(GoDebugMenu);
		return;
	}
#endif
	*(vu32 *)REG_BG2X_L	= 0;
	*(vu32 *)REG_BG2Y_L	= 0;

	TaskTable[id].work[TIT_TIMER]++;
	if(TaskTable[id].work[TIT_TIMER]&0x0001){
		TaskTable[id].work[TIT_SCR1_Y]++;
		ScrY1 = TaskTable[id].work[TIT_SCR1_Y];
		ScrX1 = 0;
	}
	TitleBGPaletteAnm(TaskTable[id].work[TIT_TIMER]);

	if(!(m4a_mplay000.stat & MUSICPLAYER_STATUS_TRACK)){
		PaletteFadeReq(0xffffffff,0,0,16,0xffff);
		MainProcChange(ReturnTitle);
	}
}

/********************************************************************/
/*				�I��												*/
/********************************************************************/
extern	void FestaGameStart( void );//�t�F�X�^�p
extern	void OpeningInit( void );

void	GoNewGame(void)
{
	if(!(PaletteAnime())){
		MainProcChange(OpeningInit);	//NEW GAME
	}
}

#ifdef	PM_DEBUG
void	GoDebugMenu(void)
{
	if(!(PaletteAnime())){
		MainProcChange(testMainInit);		//ma:Ҳ݂���Ăяo���֐����Z�b�g
	}
}
#endif

void	ReturnTitle(void)
{
	if(!(PaletteAnime())){
		MainProcChange( OPdemoInitReturn );
	}
}

void	GoBackUpDelete(void)
{
	if(!(PaletteAnime())){
		MainProcChange(BackUpDelete);
	}
}

void	GoChangeTimer(void)
{
	if(!(PaletteAnime())){
		MainProcChange(TimeInputInit);
	}
}

/********************************************************************/
/*				�a�f�p���b�g�A�j��									*/
/********************************************************************/
static	void	TitleBGPaletteAnm(u8 timer)
{
#if (PM_VERSION == VERSION_SAPPHIRE)
	u16	pal;
	u8	f,data;

	if(!(timer&0x0003)){
		data	= (timer >>2)&0x001f;
		f		= (timer >>2)&0x0020;

		if(!f)	pal = data;		//����
		else	pal = 0x1f-data;//����
		PaletteWorkSet(&pal,PA_BG14+PA_COL15,2);
	}
#elif (PM_VERSION == VERSION_RUBY)
	u16	pal;
	u8	f,data;

	if(!(timer&0x0003)){
		data	= (timer >>2)&0x001f;
		f		= (timer >>2)&0x0020;

		if(!f)	pal = (data<<10);		//����
		else	pal = ((0x1f-data)<<10);//����
		PaletteWorkSet(&pal,PA_BG14+PA_COL15,2);
	}
#endif
}


