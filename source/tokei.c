//=========================================================================
//	
//	���v�Z�b�g(�I�[�v�j���O�C�x���g)
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
#include "rtctool.h"
#include "ev_flag.h"
#include "mus_tool.h"
#include "laster.h"

#include "tokei.dat"

void	TokeiEvInit(void);
void	TokeiLookInit(void);
void	TokeiEvMain(void);
extern	void	affineWorkSet(u8 num,u16 H_X,u16 V_X,u16 H_Y,u16 V_Y);

static	const	CellData	ObjCell[];
static	const	PalData		ObjPal[];

static	void	TokeiEvSeq1InitTask(u8 id);
static	void	TokeiEvSeq1Task(u8 id);
static	void	TokeiEvSeqCheckInitTask(u8 id);
static	void	TokeiEvSeqCheckTask(u8 id);
static	void	TokeiEvSeqExitInitTask(u8 id);
static	void	TokeiEvSeqExitTask(u8 id);

static	void	TokeiLookSeq1InitTask(u8 id);
static	void	TokeiLookSeq1Task(u8 id);
static	void	TokeiLookSeqExitInitTask(u8 id);
static	void	TokeiLookSeqExitTask(u8 id);

static	u8		TokeiSpeedGet(u16 timer);
static	u16		TokeiMove(u16 pos,u8 dir,u8	sp_timer);
static	u16		TokeiCalc(u8 id,u8 dir);
static	void	PalCheckChange(u8 id,u8 dir);
static	void	TokeiLookTimeGet(u8 id);

static const	actHeader Needle_L_ActHeader;
static const	actHeader Needle_S_ActHeader;
static const	actHeader AmMarkActHeader;
static const	actHeader PmMarkActHeader;

static	const s8	tokei_center_table[][2];

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
static	void	TokeiEvVramInit1(void)
{
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

    DIV_DMACLEAR(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
    DIV_DMACLEAR(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
    DIV_DMACLEAR(3, 0, PLTT, PLTT_SIZE,           16);	// ��گ�  ������

	LZ77UnCompVram((void*)clock2ch_LZ,(void*)(BG_VRAM+0x0000));	//BG CGX�]��
	if(!ScriptParameter0){
		//�j
		PaletteWorkSet(tokei_Palette,PA_BG0,0x20);		//BG2&BG3 COL�]��
	}else{
		//��
		PaletteWorkSet(tokei_Palette2,PA_BG0,0x20);		//BG2&BG3 COL�]��
	}

	LasterInit();									// HDMA������
	InitTask();										// ���  ������
	InitActor();									// ���� ������
	PaletteAnimeInit();
	ObjPalManInit();								// ��޼ު��گ��Ȱ�ެ������

	DecordCellSet((CellData*)ObjCell);
	ObjPalSets((PalData*)ObjPal);

	PrintScreenInit(DEFAULT_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);
}

static	void	TokeiEvVramInit2(void)
{
	PaletteFadeReq(0xffffffff,0,16,0,0);

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
    *(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(TokeiEvMain);

   	*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 0;

	*(vu16 *)REG_BG3CNT = BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_PRIORITY_1
						| 7 << BG_SCREEN_BASE_SHIFT | 0 << BG_CHAR_BASE_SHIFT;
	*(vu16 *)REG_BG0CNT = BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_PRIORITY_0
						| 31 << BG_SCREEN_BASE_SHIFT | 2 << BG_CHAR_BASE_SHIFT;

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG3_ON					// BG3�L��
						  | DISP_BG0_ON					// BG3�L��
						  | DISP_OBJ_ON;
}

/********************************************************************/
/********************************************************************/
void	TokeiEvInit(void)
{
	u8	taskID;
	u16 act_no;

	TokeiEvVramInit1();
	LZ77UnCompVram((void*)clock2sc_LZ,(void*)(BG_VRAM+0x3800));	//BG MAP�]��

	taskID = AddTask(TokeiEvSeq1InitTask, 0);
	TaskTable[taskID].work[2] = 10;		//����(�����l)
	TaskTable[taskID].work[3] = 0;		//��(�����l)
	TaskTable[taskID].work[4] = 0;		//�ړ�����(0:STOP)
	TaskTable[taskID].work[5] = 0;		//���݂̃p���b�g(0:��(AM),1:�I�����W(PM))
	TaskTable[taskID].work[6] = 0;		//�j�X�s�[�h�Ǘ��^�C�}�[
	TaskTable[taskID].work[0] = TaskTable[taskID].work[3]*6;//Theta(���j)
	TaskTable[taskID].work[1] = ((TaskTable[taskID].work[2]%12)*30) 
								+ ((TaskTable[taskID].work[3]/10)*5);//Theta(�Z�j)

	act_no = AddActor(&Needle_L_ActHeader,120,80,1);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].oamData.AffineMode = 1;
	ActWork[act_no].oamData.AffineParamNo = 0;

	act_no = AddActor(&Needle_S_ActHeader,120,80,0);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].oamData.AffineMode = 1;
	ActWork[act_no].oamData.AffineParamNo = 1;

	act_no = AddActor(&AmMarkActHeader,120,80,2);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].work[1] = 45;

	act_no = AddActor(&PmMarkActHeader,120,80,2);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].work[1] = 90;

	TokeiEvVramInit2();
}

/********************************************************************/
/********************************************************************/
void	TokeiLookInit(void)
{
	u8	taskID;
	u8	theta1,theta2;
	u16 act_no;

	TokeiEvVramInit1();
	LZ77UnCompVram((void*)clock3sc_LZ,(void*)(BG_VRAM+0x3800));	//BG MAP�]��

	taskID = AddTask(TokeiLookSeq1InitTask, 0);
	TokeiLookTimeGet(taskID);

	if(!TaskTable[taskID].work[5]){
		theta1 = 45;
		theta2 = 90;
	}else{
		theta1 = 90;
		theta2 = 135;
	}
	act_no = AddActor(&Needle_L_ActHeader,120,80,1);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].oamData.AffineMode = 1;
	ActWork[act_no].oamData.AffineParamNo = 0;

	act_no = AddActor(&Needle_S_ActHeader,120,80,0);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].oamData.AffineMode = 1;
	ActWork[act_no].oamData.AffineParamNo = 1;

	act_no = AddActor(&AmMarkActHeader,120,80,2);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].work[1] = theta1;

	act_no = AddActor(&PmMarkActHeader,120,80,2);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].work[1] = theta2;

	TokeiEvVramInit2();
}



/********************************************************************/
/*                                                                  */
/*				Ҳ�													*/
/*                                                                  */
/********************************************************************/
void	TokeiEvMain(void)
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
/********************************************************************/
/*				���Ԑݒ�											*/
/********************************************************************/
/*==================================================================*/
//		������
/*==================================================================*/
static	void	TokeiEvSeq1InitTask(u8 id)
{
	if(FadeData.fade_sw == 0){
		TaskTable[id].TaskAdrs = TokeiEvSeq1Task;
	}
}

/*==================================================================*/
//		�L�[����
/*==================================================================*/
static	void	TokeiEvSeq1Task(u8 id)
{
	if(TaskTable[id].work[0]%6){		//���j�ړ���
		TaskTable[id].work[0] = TokeiMove(	TaskTable[id].work[0],	//���j�p�x
											TaskTable[id].work[4],	//����
											TaskTable[id].work[6]);	//�X�s�[�h�^�C�}�[
		return;
	}
	TaskTable[id].work[0] = TaskTable[id].work[3]*6;
	TaskTable[id].work[1] = ((TaskTable[id].work[2]%12)*30) + ((TaskTable[id].work[3]/10)*5);

	if(sys.Trg & A_BUTTON){
		TaskTable[id].TaskAdrs = TokeiEvSeqCheckInitTask;
		return;
	}

	TaskTable[id].work[4] = 0;		//�ړ�����������(0:STOP)

	if(sys.Cont & L_KEY){
		TaskTable[id].work[4] = 1;		//�ړ�����(1:��)
	}
	if(sys.Cont & R_KEY){
		TaskTable[id].work[4] = 2;		//�ړ�����(1:�E)
	}

	if(TaskTable[id].work[4]){		//�ړ��J�n����
		if(TaskTable[id].work[6] < (0x100-1)) TaskTable[id].work[6]++;
		TaskTable[id].work[0] = TokeiMove(	TaskTable[id].work[0],	//���j�p�x
											TaskTable[id].work[4],	//����
											TaskTable[id].work[6]);	//�X�s�[�h�^�C�}�[
		TokeiCalc(id,TaskTable[id].work[4]);
		return;
	}
	TaskTable[id].work[6] = 0;		//�j�X�s�[�h�Ǘ��^�C�}�[
}

/*==================================================================*/
//		�m�F
/*==================================================================*/
#define	TOKEI_YESNO_WX	(23)
#define	TOKEI_YESNO_WY	(8)
extern	const MENU_ITEM	yesno_msg[];

static	void	TokeiEvSeqCheckInitTask(u8 id)
{
	NMenuWinBoxWrite( 2, 16,27, 19);
	NMenuMsgWrite(tokei_yesno_msg,3,17);

	NMenuWinBoxWrite(TOKEI_YESNO_WX,TOKEI_YESNO_WY,TOKEI_YESNO_WX+6,TOKEI_YESNO_WY+5);
	NMenuFixListWrite(TOKEI_YESNO_WX+2,TOKEI_YESNO_WY+1,2,yesno_msg);
	NMenuVCursorInit(TOKEI_YESNO_WX+1,TOKEI_YESNO_WY+1,2,1);

	TaskTable[id].TaskAdrs = TokeiEvSeqCheckTask;
}

static	void	TokeiEvSeqCheckTask(u8 id)
{
	s8	res;

	res = YesNoSelectMain();
	switch(res){

	case	CURSOR_YES_VALUE:		//�͂�
		SePlay(SE_SELECT);
		TaskTable[id].TaskAdrs = TokeiEvSeqExitInitTask;
		break;
	case	CURSOR_NO_VALUE:		//������
	case	CURSOR_CANCEL_VALUE:	//�L�����Z�����ꂽ
		SePlay(SE_SELECT);
		NMenuBoxClear(TOKEI_YESNO_WX,TOKEI_YESNO_WY,TOKEI_YESNO_WX+6,TOKEI_YESNO_WY+5);
		NMenuBoxClear( 2, 16,27, 19);
		TaskTable[id].TaskAdrs = TokeiEvSeq1Task;
		break;
	}
}

/*==================================================================*/
//		�I��
/*==================================================================*/
static	void	TokeiEvSeqExitInitTask(u8 id)
{
	SetGameTime(TaskTable[id].work[2],TaskTable[id].work[3]);
	PaletteFadeReq(0xffffffff,0,0,16,0);
	TaskTable[id].TaskAdrs = TokeiEvSeqExitTask;
}

static	void	TokeiEvSeqExitTask(u8 id)
{
	if(FadeData.fade_sw == 0){
		MainProcChange(sys.pReturnProc);
	}
}

/********************************************************************/
/*				���Ԋm�F											*/
/********************************************************************/
/*==================================================================*/
//		������
/*==================================================================*/
static	void	TokeiLookSeq1InitTask(u8 id)
{
	if(FadeData.fade_sw == 0){
		TaskTable[id].TaskAdrs = TokeiLookSeq1Task;
	}
}

/*==================================================================*/
//		�L�[����
/*==================================================================*/
static	void	TokeiLookSeq1Task(u8 id)
{
	TokeiLookTimeGet(id);
	if(sys.Trg & (A_BUTTON|B_BUTTON)){
		TaskTable[id].TaskAdrs = TokeiLookSeqExitInitTask;
		return;
	}
}

/*==================================================================*/
//		�I��
/*==================================================================*/
static	void	TokeiLookSeqExitInitTask(u8 id)
{
	PaletteFadeReq(0xffffffff,0,0,16,0);
	TaskTable[id].TaskAdrs = TokeiLookSeqExitTask;
}

static	void	TokeiLookSeqExitTask(u8 id)
{
	if(FadeData.fade_sw == 0){
		MainProcChange(sys.pReturnProc);
	}
}



/********************************************************************/
/*                                                                  */
/*				�T�u												*/
/*                                                                  */
/********************************************************************/
/*==================================================================*/
//		���Ԑݒ�v�Z�֘A
/*==================================================================*/
static	u8	TokeiSpeedGet(u16 timer)
{
	u8	speed;

	if(timer > 60)	return 6;
	if(timer > 30)	return 3;
	if(timer > 10)	return 2;
	speed = 1;
	return speed;
}

static	u16	TokeiMove(u16 pos,u8 dir,u8	sp_timer)
{
	u8	speed;

	speed = TokeiSpeedGet(sp_timer);

	switch(dir){
	case 1:
		if(pos > 0){
			pos-=speed;
		}else{
			pos = THETA_360_DEF-speed;
		}
		break;
	case 2:
		if(pos < THETA_360_DEF-speed){
			pos+=speed;
		}else{
			pos = 0;
		}
	}
	return pos;
}

static	u16	TokeiCalc(u8 id,u8 dir)
{
	switch(dir){
	case 1:
		if(TaskTable[id].work[3] > 0){
			TaskTable[id].work[3]--;
		}else{
			TaskTable[id].work[3] = 60-1;
			if(TaskTable[id].work[2] > 0){
				TaskTable[id].work[2]--;
			}else{
				TaskTable[id].work[2] = 24-1;
			}
			PalCheckChange(id,dir);
		}
		break;
	case 2:
		if(TaskTable[id].work[3] < 60-1){
			TaskTable[id].work[3]++;
		}else{
			TaskTable[id].work[3] = 0;
			if(TaskTable[id].work[2] < 24-1){
				TaskTable[id].work[2]++;
			}else{
				TaskTable[id].work[2] = 0;
			}
			PalCheckChange(id,dir);
		}
		break;
	}
	return 0;
}

/*==================================================================*/
//		AM/PM�p���b�g�ύX
/*==================================================================*/
static	void	PalCheckChange(u8 id,u8 dir)
{
	u8	hour;

	hour = TaskTable[id].work[2];
	switch(dir){
	case 1:
		//��(-)����
		switch(hour){
		case 11:
			TaskTable[id].work[5] = 0;		//���݂̃p���b�g(0:��(AM),1:�I�����W(PM))
			break;
		case 23:
			TaskTable[id].work[5] = 1;
			break;
		default:
			break;
		}
		break;
	case 2:
		//�E(+)����
		switch(hour){
		case 0:
			TaskTable[id].work[5] = 0;
			break;
		case 12:
			TaskTable[id].work[5] = 1;
			break;
		default:
			break;
		}
		break;
	}
}

/*==================================================================*/
//		���ԃf�[�^�擾
/*==================================================================*/
static	void	TokeiLookTimeGet(u8 id)
{
	GetGameTime();

	TaskTable[id].work[2] = GameTime.hour;	//����(�����l)
	TaskTable[id].work[3] = GameTime.min;	//��(�����l)
	TaskTable[id].work[0] = TaskTable[id].work[3]*6;//Theta(���j)
	TaskTable[id].work[1] = ((TaskTable[id].work[2]%12)*30) 
								+ ((TaskTable[id].work[3]/10)*5);//Theta(�Z�j)
	if(GameTime.hour <12){
		TaskTable[id].work[5] = 0;	//�p���b�g(0:��(AM))
	}else{
		TaskTable[id].work[5] = 1;	//�p���b�g(1:�I�����W(PM))
	}
}


/********************************************************************/
/*                                                                  */
/*				OBJ���&����֘A										*/
/*                                                                  */
/********************************************************************/
#define	CELLID_OBJ	(0x1000)
#define	PALID_OBJ	(0x1000)
#define	PALID_OBJ2	(0x1001)


static	const	CellData	ObjCell[] = {
	{ clockob2_LZ, 0x2000	,CELLID_OBJ },
	{ CELL_END, 0, 0 }
};
static	const	PalData		ObjPal[] = {
	{ tokei_Palette	,PALID_OBJ },
	{ tokei_Palette2,PALID_OBJ2 },
	{ OBJPAL_END, 0 },
};

/*==================================================================*/
/*				�j													*/
/*==================================================================*/
/*------------------------------------------------------------------*/
/*                      OAM��{�ް�                                 */
static const ActOamData NeedleOam = {

160,	//V�߼޼��
0,  	//�g��k��Ӱ�ސݒ�(ON)
0,      //OBJӰ�ސݒ�(������)
0,	//ӻ޲��ݒ�(OFF)
0,	//�װӰ�ސݒ�(16�F)
0,	//OBJ�`��ݒ�(�����`)

0,	//H�߼޼��
0,	//�g��k�����Ұ����ް�ݒ�
3,	//OBJ�T�C�Y�ݒ�(64dot)

0x0,	//��׸�����ް
1,	//�\����ײ��è��ݒ�(��2�D��)
0,	//��گ����ް

0,	//�g��k�����Ұ�
};

/*------------------------------------------------------------------*/
/*                      MOVE�ް�                                    */
static void Needle_L_ActMove(actWork* aw)
{
	u16	p;
	s16	sin,cos;
	u16	dxw,dyw;

	p = TaskTable[aw->work[0]].work[0];

	sin = Sin_360_Get(p)/0x10;
	cos = Cos_360_Get(p)/0x10;
	affineWorkSet(0,cos,sin,-sin,cos);

	dxw = tokei_center_table[p][0];
	dyw = tokei_center_table[p][1];

	if(dxw > 0x80)	dxw |= 0xff00;
	if(dyw > 0x80)	dyw |= 0xff00;
	aw->dx = dxw;
	aw->dy = dyw;
}

static void Needle_S_ActMove(actWork* aw)
{
	u16	p;
	s16	sin,cos;
	u16	dxw,dyw;

	p = TaskTable[aw->work[0]].work[1];

	sin = Sin_360_Get(p)/0x10;
	cos = Cos_360_Get(p)/0x10;
	affineWorkSet(1,cos,sin,-sin,cos);

	dxw = tokei_center_table[p][0];
	dyw = tokei_center_table[p][1];

	if(dxw > 0x80)	dxw |= 0xff00;
	if(dyw > 0x80)	dyw |= 0xff00;
	aw->dx = dxw;
	aw->dy = dyw;
}

/*------------------------------------------------------------------*/
/*                      ANM�ް�                                     */
static const actAnm Needle_L_Anm[] = {{ 0,30,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm Needle_S_Anm[] = {{64,30,0,0},{ACT_ANMEND,0,0,0}};

/*------------------------------------------------------------------*/
/*                      ANMð���                                    */
static const actAnm* const Needle_L_AnmTbl[]={Needle_L_Anm};
static const actAnm* const Needle_S_AnmTbl[]={Needle_S_Anm};

/*------------------------------------------------------------------*/
/*                      ACTORͯ��                                   */
static const actHeader Needle_L_ActHeader ={
CELLID_OBJ,		//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
PALID_OBJ,		//��گ��Ȱ�ެ�o�^����
&NeedleOam,
Needle_L_AnmTbl,
ACT_NTRANS,
DummyActAffTbl,
Needle_L_ActMove,
};

static const actHeader Needle_S_ActHeader ={
CELLID_OBJ,		//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
PALID_OBJ,		//��گ��Ȱ�ެ�o�^����
&NeedleOam,
Needle_S_AnmTbl,
ACT_NTRANS,
DummyActAffTbl,
Needle_S_ActMove,
};


/*==================================================================*/
/*			�`�l�E�o�l�}�[�N										*/
/*==================================================================*/
/*------------------------------------------------------------------*/
/*                      OAM��{�ް�                                 */
static const ActOamData AmPmOam = {

160,	//V�߼޼��
0,  	//�g��k��Ӱ�ސݒ�(ON)
0,      //OBJӰ�ސݒ�(������)
0,	//ӻ޲��ݒ�(OFF)
0,	//�װӰ�ސݒ�(16�F)
0,	//OBJ�`��ݒ�(�����`)

0,	//H�߼޼��
0,	//�g��k�����Ұ����ް�ݒ�
1,	//OBJ�T�C�Y�ݒ�(16dot)

0x0,	//��׸�����ް
2,	//�\����ײ��è��ݒ�(��2�D��)
0,	//��گ����ް

0,	//�g��k�����Ұ�
};

/*------------------------------------------------------------------*/
/*                      MOVE�ް�                                    */
static void AmMarkActMove(actWork* aw)
{
	if(TaskTable[aw->work[0]].work[5]){		//���݂̃p���b�g(0:��(AM),1:�I�����W(PM))
		if((aw->work[1] < 90)&&(aw->work[1] >= 60))	aw->work[1]+=5;
		if(aw->work[1] < 60)	aw->work[1]++;
	}else{
		if((aw->work[1] > 45)&&(aw->work[1] <= 75))	aw->work[1]-=5;
		if(aw->work[1] > 75)	aw->work[1]--;
	}
	aw->dx = (0x20-2)*Cos_360_Get(aw->work[1])/0x1000;
	aw->dy = (0x20-2)*Sin_360_Get(aw->work[1])/0x1000;
}

static void PmMarkActMove(actWork* aw)
{
	if(TaskTable[aw->work[0]].work[5]){		//���݂̃p���b�g(0:��(AM),1:�I�����W(PM))
		if((aw->work[1] < 135)&&(aw->work[1] >= 105))	aw->work[1]+=5;
		if(aw->work[1] < 105)	aw->work[1]++;
	}else{
		if((aw->work[1] > 90)&&(aw->work[1] <= 120))	aw->work[1]-=5;
		if(aw->work[1] > 120)	aw->work[1]--;
	}
	aw->dx = (0x20-2)*Cos_360_Get(aw->work[1])/0x1000;
	aw->dy = (0x20-2)*Sin_360_Get(aw->work[1])/0x1000;
}

/*------------------------------------------------------------------*/
/*                      ANM�ް�                                     */
static const actAnm AmMarkAnm[] = {{132,30,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm PmMarkAnm[] = {{128,30,0,0},{ACT_ANMEND,0,0,0}};

/*------------------------------------------------------------------*/
/*                      ANMð���                                    */
static const actAnm* const AmMarkAnmTbl[]={AmMarkAnm};
static const actAnm* const PmMarkAnmTbl[]={PmMarkAnm};

/*------------------------------------------------------------------*/
/*                      ACTORͯ��                                   */
static const actHeader AmMarkActHeader ={
CELLID_OBJ,		//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
PALID_OBJ,		//��گ��Ȱ�ެ�o�^����
&AmPmOam,
AmMarkAnmTbl,
ACT_NTRANS,
DummyActAffTbl,
AmMarkActMove,
};

static const actHeader PmMarkActHeader ={
CELLID_OBJ,		//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
PALID_OBJ,		//��گ��Ȱ�ެ�o�^����
&AmPmOam,
PmMarkAnmTbl,
ACT_NTRANS,
DummyActAffTbl,
PmMarkActMove,
};



/********************************************************************/
/*                                                                  */
/*				��]�p���S�f�[�^�擾(data:dx,dy)					*/
/*                                                                  */
/********************************************************************/
static	const s8 tokei_center_table[][2] = {
{0x00,0xE8},{0x01,0xE7},{0x01,0xE7},{0x02,0xE7},{0x02,0xE7},{0x02,0xE7},
{0x03,0xE8},{0x03,0xE7},{0x04,0xE7},{0x04,0xE7},{0x04,0xE7},{0x05,0xE7},
{0x05,0xE7},{0x06,0xE8},{0x06,0xE8},{0x06,0xE8},{0x07,0xE8},{0x07,0xE8},
{0x07,0xE8},{0x08,0xE8},{0x08,0xE8},{0x09,0xE8},{0x09,0xE8},{0x0A,0xE9},
{0x0A,0xE9},{0x0B,0xEA},{0x0B,0xEA},{0x0B,0xEA},{0x0C,0xEA},{0x0C,0xEB},
{0x0D,0xEB},{0x0D,0xEB},{0x0D,0xEB},{0x0E,0xEB},{0x0E,0xEB},{0x0E,0xEC},
{0x0E,0xEC},{0x0F,0xEC},{0x0F,0xED},{0x10,0xED},{0x10,0xED},{0x10,0xED},
{0x10,0xEE},{0x10,0xEE},{0x11,0xEE},{0x11,0xEF},{0x11,0xEF},{0x12,0xEF},
{0x12,0xEF},{0x12,0xF0},{0x12,0xF0},{0x13,0xF0},{0x13,0xF1},{0x13,0xF1},
{0x14,0xF1},{0x14,0xF2},{0x14,0xF2},{0x14,0xF3},{0x14,0xF3},{0x15,0xF3},
{0x15,0xF3},{0x15,0xF4},{0x16,0xF4},{0x16,0xF4},{0x16,0xF5},{0x16,0xF5},
{0x16,0xF6},{0x17,0xF6},{0x17,0xF7},{0x17,0xF7},{0x17,0xF7},{0x17,0xF7},
{0x17,0xF8},{0x17,0xF8},{0x17,0xF9},{0x17,0xF9},{0x17,0xFA},{0x18,0xFA},
{0x18,0xFA},{0x19,0xFB},{0x19,0xFB},{0x18,0xFC},{0x19,0xFC},{0x18,0xFD},
{0x19,0xFD},{0x19,0xFD},{0x19,0xFE},{0x19,0xFE},{0x18,0xFF},{0x19,0xFF},

{0x18,0x00},{0x18,0x00},{0x18,0x00},{0x18,0x01},{0x18,0x01},{0x19,0x02},
{0x18,0x02},{0x19,0x02},{0x18,0x03},{0x18,0x03},{0x19,0x04},{0x18,0x04},
{0x18,0x05},{0x18,0x05},{0x18,0x05},{0x18,0x06},{0x17,0x06},{0x17,0x06},
{0x17,0x07},{0x17,0x08},{0x17,0x08},{0x17,0x08},{0x17,0x09},{0x17,0x09},
{0x17,0x0A},{0x16,0x0A},{0x16,0x0A},{0x16,0x0B},{0x16,0x0B},{0x16,0x0B},
{0x16,0x0C},{0x15,0x0C},{0x15,0x0C},{0x15,0x0D},{0x14,0x0D},{0x14,0x0D},
{0x13,0x0D},{0x13,0x0D},{0x13,0x0E},{0x13,0x0E},{0x13,0x0F},{0x13,0x0F},
{0x12,0x0F},{0x12,0x10},{0x11,0x10},{0x11,0x10},{0x11,0x11},{0x11,0x11},
{0x10,0x11},{0x10,0x12},{0x10,0x12},{0x0F,0x12},{0x0E,0x12},{0x0F,0x13},
{0x0E,0x13},{0x0E,0x13},{0x0D,0x13},{0x0D,0x14},{0x0D,0x14},{0x0D,0x14},
{0x0C,0x14},{0x0C,0x14},{0x0C,0x15},{0x0B,0x15},{0x0B,0x15},{0x0B,0x15},
{0x0A,0x15},{0x0A,0x16},{0x0A,0x16},{0x09,0x16},{0x09,0x16},{0x08,0x16},
{0x07,0x16},{0x07,0x17},{0x07,0x17},{0x06,0x17},{0x06,0x17},{0x05,0x17},
{0x05,0x17},{0x05,0x18},{0x04,0x18},{0x04,0x18},{0x04,0x18},{0x03,0x18},
{0x02,0x18},{0x02,0x18},{0x01,0x18},{0x01,0x18},{0x00,0x18},{0x00,0x18},

{0xFF,0x17},{0x00,0x18},{0x00,0x18},{0xFF,0x18},{0xFF,0x18},{0xFE,0x18},
{0xFE,0x18},{0xFD,0x18},{0xFD,0x18},{0xFC,0x18},{0xFC,0x18},{0xFB,0x18},
{0xFB,0x17},{0xFB,0x17},{0xFA,0x17},{0xFA,0x17},{0xF9,0x17},{0xF9,0x17},
{0xF9,0x17},{0xF8,0x17},{0xF8,0x16},{0xF7,0x16},{0xF7,0x16},{0xF6,0x16},
{0xF6,0x16},{0xF6,0x15},{0xF5,0x15},{0xF5,0x15},{0xF5,0x15},{0xF5,0x14},
{0xF4,0x14},{0xF4,0x14},{0xF3,0x14},{0xF3,0x14},{0xF3,0x13},{0xF2,0x13},
{0xF2,0x13},{0xF2,0x13},{0xF2,0x12},{0xF1,0x12},{0xF1,0x12},{0xF1,0x11},
{0xF0,0x11},{0xF0,0x11},{0xEF,0x11},{0xEF,0x10},{0xEF,0x10},{0xEE,0x10},
{0xEF,0x0F},{0xEE,0x0F},{0xEE,0x0F},{0xED,0x0F},{0xED,0x0E},{0xED,0x0E},
{0xED,0x0D},{0xED,0x0D},{0xEC,0x0D},{0xEC,0x0C},{0xEC,0x0C},{0xEB,0x0C},
{0xEB,0x0C},{0xEB,0x0B},{0xEB,0x0B},{0xEB,0x0A},{0xEB,0x0A},{0xEB,0x09},
{0xEA,0x09},{0xEA,0x09},{0xEA,0x08},{0xEA,0x08},{0xEA,0x07},{0xE9,0x07},
{0xE9,0x07},{0xE9,0x06},{0xE9,0x06},{0xE9,0x05},{0xE8,0x05},{0xE9,0x04},
{0xE9,0x04},{0xE8,0x04},{0xE8,0x04},{0xE8,0x03},{0xE8,0x03},{0xE8,0x02},
{0xE8,0x02},{0xE8,0x01},{0xE8,0x01},{0xE8,0x01},{0xE8,0x00},{0xE7,0x00},

{0xE8,0xFF},{0xE7,0xFF},{0xE8,0xFF},{0xE8,0xFE},{0xE8,0xFE},{0xE8,0xFD},
{0xE8,0xFD},{0xE8,0xFC},{0xE8,0xFC},{0xE8,0xFC},{0xE8,0xFB},{0xE8,0xFB},
{0xE8,0xFA},{0xE8,0xFA},{0xE9,0xFA},{0xE9,0xF9},{0xE9,0xF9},{0xE9,0xF8},
{0xE9,0xF8},{0xE9,0xF7},{0xE9,0xF7},{0xEA,0xF7},{0xEA,0xF7},{0xEA,0xF6},
{0xEA,0xF6},{0xEB,0xF6},{0xEB,0xF5},{0xEA,0xF5},{0xEA,0xF4},{0xEB,0xF4},
{0xEB,0xF3},{0xEB,0xF3},{0xEC,0xF3},{0xEB,0xF2},{0xEC,0xF2},{0xEC,0xF2},
{0xED,0xF2},{0xED,0xF1},{0xED,0xF1},{0xEE,0xF0},{0xEE,0xF0},{0xEE,0xF0},
{0xEE,0xEF},{0xEE,0xEF},{0xEF,0xEF},{0xEF,0xEE},{0xEF,0xEE},{0xF0,0xEE},
{0xF0,0xEE},{0xF0,0xED},{0xF0,0xED},{0xF1,0xED},{0xF1,0xED},{0xF1,0xEC},
{0xF2,0xEC},{0xF2,0xEC},{0xF2,0xEB},{0xF3,0xEB},{0xF3,0xEB},{0xF3,0xEB},
{0xF4,0xEB},{0xF4,0xEA},{0xF5,0xEA},{0xF5,0xEA},{0xF5,0xEA},{0xF6,0xEA},
{0xF6,0xEA},{0xF7,0xEA},{0xF7,0xE9},{0xF7,0xE9},{0xF8,0xE9},{0xF8,0xE9},
{0xF9,0xE9},{0xF9,0xE9},{0xF9,0xE8},{0xFA,0xE8},{0xFA,0xE8},{0xFB,0xE8},
{0xFB,0xE8},{0xFC,0xE8},{0xFC,0xE8},{0xFC,0xE8},{0xFC,0xE7},{0xFD,0xE7},
{0xFE,0xE7},{0xFE,0xE8},{0xFE,0xE8},{0xFF,0xE7},{0xFF,0xE7},{0x00,0xE7},
};

