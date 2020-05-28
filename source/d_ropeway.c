//<demo_ropeway.c>
//////////////////////////////////////////////////////////////////////
//
//		�f�o�b�O�p���[�v�E�F�C�f����ʏ���
//		DebugIwasawa������̌Ăяo��
//
//												By Iwasawa 02/05/27
//////////////////////////////////////////////////////////////////////

#include "d_ropeway.h"
#include "intr.h"
#include "task.h"
#include "menu.h"
#include "fld_task.h"
#include "fld_tool.h"
#include "script.h"

#include "ev_flag.h"
#include "evobj.h"
#include "weather.h"
#include "wea_com.h"
#include "decord.h"
#include "mus_tool.h"
#include "laster.h"
#include "../graphic/pokegra.h"

//���C���^�X�N�v���g�^�C�v
void DemoRopewayInit(void);			//�f���̏�����
static void DemoRopewayMain(void);	//�f���̃��C���^�X�N
static void DemoRopewayExit(void);	//�f���̏I��

//�T�u�^�X�N�v���g�^�C�v
static void DemoRopewayProc(u8);	//���Z�p�̃T�u�^�X�N
static void DemoRopewayUp(u8);		//�̂ڂ�p�T�u�^�X�N
static void DemoRopewayDown(u8);	//����p�T�u�^�X�N

//VBlank���荞�݊֐��v���g�^�C�v
static void DemoRopewayDraw(void);

//�A�N�^�[����֐��v���g�^�C�v
static void DummyMoveSeq(actWork* xreg);		//�����Ȃ�
static void rwd_dondola_move(actWork* xreg);	//�S���h���{�̂̓���

//���[�J���֐��v���g�^�C�v
static void SetScreenState(u8	setflg);	//�X�N���[������֌W�̃��W�X�^�Z�b�g
static void demo_ropeway_init(void);		//�����ɉ����āA�I�u�W�F�N�g�p��

//�R�̃X�N���[������
static void MtScrollUp(void);			//�̂ڂ�p
static void MtScrollTransUp(void);
static void MtScrollDown(void);			//����p
static void MtScrollTransDown(void);
static void MtScrollInit(u8	flg);		//�X�N���[���l�̏�����

#include "d_ropeway.dat"
#include "d_rwAct.dat"

//�t�@�C�����O���[�o���f�[�^
static RWDEMO_WORK*	sg_pWork = NULL;

///////////////////////////////////////////////////
//	�X�N���v�g����̌Ăяo��
/////////////////////////////////////////////////////
static void rwd_fadewait(u8 tID)
{
	if(FadeData.fade_sw != 0) return;

	//�t�F�[�h�A�E�g���I�������A���C���^�X�N�؂�ւ�
	MainProcChange(DemoRopewayInit);
	DelTask(tID);	//���̃^�X�N�͂�������Ȃ��̂ŏ���

	return;
}
void DemoRopewayCall(void)
{
	u8	taskno;

	SetForceEvent();	//�����C�x���g�t���O�Z�b�g
	taskno = AddTask(rwd_fadewait,1);			//�t�F�[�h�҂��̃^�X�N��Add
	PaletteFadeReq(0xffffffff,0,0,16,0x0000);	//�t�F�[�h�A�E�g���N�G�X�g
	return;
}

///////////////////////////////////////////////////
//	�f�o�b�O���j���[����̌Ăяo��
//////////////////////////////////////////////////
#ifdef PM_DEBUG
void DemoRopewaySetUp(void)
{
	ScriptParameter0 = RWD_UP;
	MenuEnd();
	
	DemoRopewayCall();
	return;
}
void DemoRopewaySetDown(void)
{
	ScriptParameter0 = RWD_DOWN;
	MenuEnd();
	
	DemoRopewayCall();
	return;
}
#endif	//PM_DEBUG

////////////////////////////////////////////////////
//	���C���^�X�N
////////////////////////////////////////////////////
/*----------------------------------------------
	���[�v�E�F�C�f�� �������^�X�N

	void DemoRopewayInit(void);
------------------------------------------------*/
void DemoRopewayInit(void)
{
	u8	ct = 0;
	
	switch(sys.InitSeqWork)
	{
	default:
	case 0:
		SetVBlankFunc(NULL);
		LasterInit();							//HDMA������
		DIV_DMACLEAR(3,0,VRAM,VRAM_SIZE,16);	//VRAM�N���A
		DIV_DMACLEAR(3,0,OAM,OAM_SIZE,32);		//OAM�N���A
		DIV_DMACLEAR(3,0,PLTT,PLTT_SIZE,16);	//PlttRAM�N���A
		
		//�f���p���[�N�N���A
		sg_pWork =(RWDEMO_WORK *)( &(UserWork[RWD_WORK0_AREA]));
		DIV_DMACLEAR(3,0,sg_pWork,sizeof(RWDEMO_WORK),16);
		
		sys.InitSeqWork++;
		break;
	case 1:		//�^�X�N�E�A�N�^�[�EGR�f�[�^������
		InitActor();					//�A�N�^�[�V�X�e��������
		InitTask();						//�^�X�N�V�X�e��������
		ObjPalManInit();				//�I�u�W�F�N�g�p���b�g�}�l�[�W���[������
		PaletteAnimeInit();				//�p���b�g�A�j���[�V�����V�X�e��������

		WeatherInit();	//�V��G�t�F�N�g������
		for(ct = 0;ct < ASH_OBJ_MAX;ct++){
			wp->ash_actor[ct] = NULL; 
		}
		//�T�E���h������
		BGM_SystemInit();
		BGM_Init();
		
		ActOX = ActOY = 0;
		sys.InitSeqWork++;
		break;
	case 2:
		//�K�v�ȃZ���ƃp���b�g�f�[�^��p��
		for(ct = 0;ct < RWD_OBJCELL_NUM;ct++){
			DecordCellSet((CellData*)&(RWD_DataCell[ct]));
		}
		ObjPalSets(&RWD_DataPal[0]);	//�p���b�gS�ŁAPALLETRAM��

		//���k�X�N���[���f�[�^��UserWork�ɓW�J
		DecordWram((void *)GD_BG_MT_MAPSRC,(void*)GD_BG_MT_MAP);
		DecordWram((void *)GD_BG_TREE_MAPSRC,(void*)GD_BG_TREE_MAP);
		DecordWram((void *)GD_BG_SKY_MAPSRC,(void*)GD_BG_SKY_MAP);
		DecordWram((void *)GD_BG_POLE2_MAPSRC,(void*)GD_BG_POLE2_MAP);

		//BG�p�p���b�g�f�[�^�����[�N�ɓ]��
		PaletteWorkSet((const u16 *)rwd_bg01_pcl_ADRS,PA_BG0,0x20*4);

		//VRAM��BG�L�����N�^��W�J
		LZ77UnCompVram((void *)rwd_bg01_pch_ADRS,(void *)VRAM);

		sys.InitSeqWork++;
		break;
	case 3:
		//���@�̐��ʂ��艺��𔻒f���āA������
		//���@�̃I�u�W�F�N�g�������ŗp��
		demo_ropeway_init();
		MainTask();				//�V��G�t�F�N�g����
		sys.InitSeqWork++;
		break;
	case 4:
		if(sg_pWork->uc_weather == WEATHER_ASH){
			sys.InitSeqWork++;
			break;
		}

		if(wp->ash_actor[0] == NULL) break;
		
		for(ct = 0;ct < ASH_OBJ_MAX;ct++){
			if(wp->ash_actor[ct] != NULL){
				wp->ash_actor[ct]->oamData.Priority = 0;
			}
		}
		sys.InitSeqWork++;
		break;
	case 5:		//�o�b�t�@�ɏ���BG�f�[�^��p��
		//�X��
		SetChar16R((_tMap *)(SCREEN_BASE29_ADDR),(u16 *)GD_BG_TREE_MAP,0,17,32,15);
		//�w�i
		SetChar16R((_tMap *)(SCREEN_BASE30_ADDR),(u16 *)GD_BG_SKY_MAP,0,0,30,20);	//Screen2
		//�x��
		SetChar16R(&(sg_pWork->map[1]),(u16 *)GD_BG_POLE1_MAP,0,0,5,2);
		SetChar16R(&(sg_pWork->map[1]),(u16 *)GD_BG_POLE2_MAP,0,2,2,20);

		sys.InitSeqWork++;
		break;
	case 6:
		MtScrollInit(ScriptParameter0);
		//�R��
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[12*6]),0,14,12,3);		//Screen0
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[12*9]),12,17,12,3);	//Screen0
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[12*12]),24,20,12,3);
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[0]),0,17,12,3);
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[12*3]),0,20,12,3);
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[0]),12,20,12,3);
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[12*3]),12,23,12,3);
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[0]),24,23,12,3);
		
		sys.InitSeqWork++;
		break;
	case 7:
		PaletteFadeReq(0xffffffff,3,16,0,0);	//�t�F�[�h�C�����N�G�X�g		
		
		//BGM�Z�b�g
		MusicFadeInPlay(MUS_ROPEWAY,1);		

		//BG�R���g���[�����W�X�^��ݒ�
		SetScreenState(RWD_START);
		sys.InitSeqWork++;
		break;
	case 8:
		//VBlank���荞�݋���
		REG_IE_SET(V_BLANK_INTR_FLAG);
		SetVBlankFunc(DemoRopewayDraw);
		
		MainProcChange(DemoRopewayMain);
		AddTask(DemoRopewayProc,0);

		//���Ɖ������ʂ��āA�T�u�^�X�N���Z�b�g
		if(ScriptParameter0 == RWD_UP){
			sg_pWork->uc_subTaskNo = AddTask(DemoRopewayUp,1);
		}
		else{
			sg_pWork->uc_subTaskNo = AddTask(DemoRopewayDown,1);
		}
		break;
	}
	return;
}

/*-------------------------------------------
	���[�v�E�F�C�f�� ���C���^�X�N

	static void DemoRopewayMain(void);
------------------------------------------------*/
static void DemoRopewayMain(void)
{
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();

	//�T�E���h�Đ�
	BGM_Main();
	return;
}

/*----------------------------------------------
	���[�v�E�F�C�f��	�I���^�X�N
	
	static void DemoRopewayExit(void);
------------------------------------------------*/
static void DemoRopewayExit(void)
{
	u8	ct = 0;

	SetScreenState(RWD_CLEAR);			//���W�X�^�N���A

	//�V��G�t�F�N�g�N���A
	ActOX = 0;
	WeatherChangeDemo(WEATHER_OFF);
	for(ct = 0;ct < ASH_OBJ_MAX;ct++){
		wp->ash_actor[ct] = NULL; 
	}
	
	InitTask();
	InitActor();
	PaletteAnimeInit();

	//���[�U�[���[�N�N���A
	DIV_DMACLEAR(3,0,UserWork,sizeof(UserWork),32);
	sg_pWork = NULL;

	DIV_DMACLEAR(3,0,VRAM,VRAM_SIZE,16);	//VRAM�N���A
	DIV_DMACLEAR(3,0,OAM,OAM_SIZE,32);		//OAM�N���A
	DIV_DMACLEAR(3,0,PLTT,PLTT_SIZE,16);	//PlttRAM�N���A
	

	//�X�N���v�g�֕��A����
	SetNextWorldPosition();
	pFieldRecoverFunc = NULL;//AddFieldInTask;
	MainProcChange( FieldMainInit );
		
	return;
}

///////////////////////////////////////////////////////
//	�T�u�^�X�N
//////////////////////////////////////////////////////
static void DemoRopewayProc(u8 tID)
{
	u8	ct = 0;
	
	sg_pWork->us_time++;

	switch(sg_pWork->uc_state)
	{
	case 0x00:
		//�V��`�F���W�`�F�b�N
		if(sg_pWork->us_time == sg_pWork->us_weaTime){
			WeatherChangeSmooth(sg_pWork->uc_weather);
			sg_pWork->uc_state = 0x01;
		}
		break;
	case 0x01:
		switch(sg_pWork->uc_weather)
		{
		case WEATHER_ASH:
			if(wp->ash_actor[0] != NULL && wp->ash_actor[0]->oamData.Priority != 0){
				for(ct = 0;ct < ASH_OBJ_MAX;ct++){
					if(wp->ash_actor[ct] != NULL){
						wp->ash_actor[ct]->oamData.Priority = 0;
					}
				}
				sg_pWork->uc_state = 0x02;
			}
			break;
		case WEATHER_SHINE2:
			if(wp->status == WEATHER_SHINE2){
				sg_pWork->uc_state = 0x02;
				return;
			}
			if(sg_pWork->us_time < (sg_pWork->us_weaTime+8)){
				return;
			}
			for(ct = 0;ct < ASH_OBJ_MAX;ct++){
				if(wp->ash_actor[ct] != NULL){
					wp->ash_actor[ct]->banish ^= 1;
				}
			}
			break;
		default:
			break;
		}
		break;
	case 0x02:
		//�I���`�F�b�N
		if(sg_pWork->us_time == RWD_TIME_END){
			sg_pWork->uc_state = 0x03;
			PaletteFadeReq(0xFFFFFFFF,3,0,16,0x0000);	

			//BGM�t�F�[�h�A�E�g
			MusicFadeOutEnd(4);
			return;	
		}		
		break;
	case 0x03:	//�t�F�[�h�҂�
		if(FadeData.fade_sw == 0)sg_pWork->uc_state = 0xFF;
		break;
	case 0xFF:
		SetVBlankFunc(NULL);
		DelTask(tID);

		//�����ЂƂ̃T�u�^�X�N���폜
		DelTask(sg_pWork->uc_subTaskNo);
		MainProcChange(DemoRopewayExit);
		break;
	}
	return;	
}

static void DemoRopewayUp(u8 tID)
{
	switch(sg_pWork->uc_state)
	{
	case 0xFF:
		break;
	default:
		//�x���̃X�N���[��
		sg_pWork->scroll[RWD_BG_POLE].uc_x -=1;
		if(sg_pWork->us_time % 2 == 0)
		{
			sg_pWork->scroll[RWD_BG_POLE].uc_y--;
		}
		if(sg_pWork->us_time % 8 == 0)
		{
			//�X�̃X�N���[��
			sg_pWork->scroll[1].uc_x--;
			sg_pWork->scroll[RWD_BG_TREE].uc_y--;
		}
		
		//�x���̈ʒu���Z�b�g
		switch(sg_pWork->scroll[RWD_BG_POLE].uc_x)
		{
		case (256+RWD_U_POLE_X-1):
			FillChar16R(&(sg_pWork->map[1]),0x0000,0,22,2,10);
			break;
		case (256 - 27*8):
			FillChar16R(&(sg_pWork->map[1]),0x0000,3,0,2,2);
			break;
		case (256 - 28*8):
			FillChar16R(&(sg_pWork->map[1]),0x0000,2,0,1,2);
			break;
		case (16):
			SetChar16R(&(sg_pWork->map[1]),(u16 *)GD_BG_POLE1_MAP,0,0,5,2);
			SetChar16R(&(sg_pWork->map[1]),(u16 *)GD_BG_POLE2_MAP,0,2,2,30);
			sg_pWork->scroll[RWD_BG_POLE].uc_y  = 64;
			break;
		}
		break;
	}
	//�Ζʂ̃X�N���[��
	MtScrollUp();

	//�ΎR�D�̃X�N���[��
	ActOX = (ActOX+1)%128;
	return;
}

static void DemoRopewayDown(u8 tID)
{
	switch(sg_pWork->uc_state)
	{
	case 0xFF:
		break;
	default:
		//�x���̃X�N���[��
		sg_pWork->scroll[RWD_BG_POLE].uc_x += 1;
		if(sg_pWork->us_time% 2 == 0){
			sg_pWork->scroll[RWD_BG_POLE].uc_y++;
		}
		//�X�̃X�N���[��
		if(sg_pWork->us_time % 8 == 0){
				sg_pWork->scroll[1].uc_x++;
				sg_pWork->scroll[RWD_BG_TREE].uc_y++;
		}
		
		//�x���̈ʒu���Z�b�g
		switch(sg_pWork->scroll[RWD_BG_POLE].uc_x)
		{
		case (256+RWD_U_POLE_X):
			SetChar16R(&(sg_pWork->map[1]),(u16 *)GD_BG_POLE2_MAP,0,2,2,30);
			break;
		case (16):
			FillChar16R(&(sg_pWork->map[1]),0x0000,2,0,3,2);
			FillChar16R(&(sg_pWork->map[1]),0x0000,0,22,2,10);
			sg_pWork->scroll[RWD_BG_POLE].uc_y = 256-64;
			break;
		case (32):
			sg_pWork->map[1].usa[0][2] = ((u16 *)GD_BG_POLE1_MAP)[2];
			sg_pWork->map[1].usa[0][3] = ((u16 *)GD_BG_POLE1_MAP)[3];
			sg_pWork->map[1].usa[1][2] = ((u16 *)GD_BG_POLE1_MAP)[7];
			sg_pWork->map[1].usa[1][3] = ((u16 *)GD_BG_POLE1_MAP)[8];
			break;
		case (40):
			sg_pWork->map[1].usa[0][4] = ((u16 *)GD_BG_POLE1_MAP)[4];
			sg_pWork->map[1].usa[1][4] = ((u16 *)GD_BG_POLE1_MAP)[9];
			break;
		}
		break;
	}
	//�Ζʂ̃X�N���[��
	MtScrollDown();

	//�ΎR�D�̃X�N���[��
	if(sg_pWork->us_time < sg_pWork->us_weaTime)
		ActOX = (ActOX-1+248)%248;
	else	//�V��؂�ւ��t�F�[�h���������X�N���[�������邽�߁A���l���������Ă�
		wp->ash_xline = (wp->ash_xline-1+248)%248;
	
	return;
}

//////////////////////////////////////////////////////
//	VBlank���荞��	�`��֐�
//////////////////////////////////////////////////////
static void DemoRopewayDraw(void)
{
	//BG����]��
	DmaCopy(3,&(sg_pWork->map[0].usa[0][0]),SCREEN_BASE28_ADDR,2048,16);
	DmaCopy(3,&(sg_pWork->map[1].usa[0][0]),SCREEN_BASE31_ADDR,2048,16);
	
	*(vu16 *)REG_BG3HOFS = (vu16)(sg_pWork->scroll[RWD_BG_POLE].uc_x);
	*(vu16 *)REG_BG3VOFS = (vu16)(sg_pWork->scroll[RWD_BG_POLE].uc_y);
	*(vu16 *)REG_BG1HOFS = (vu16)(sg_pWork->scroll[RWD_BG_TREE].uc_x);
	*(vu16 *)REG_BG1VOFS = (vu16)(sg_pWork->scroll[RWD_BG_TREE].uc_y);
	*(vu16 *)REG_BG0HOFS = (vu16)(sg_pWork->scroll[RWD_BG_MT].uc_x);
	*(vu16 *)REG_BG0VOFS = (vu16)(sg_pWork->scroll[RWD_BG_MT].uc_y);
		
	actOAMtrans();
	actDMAtrans();
	PaletteFadeTrans();
	return;
}

//////////////////////////////////////////////////////////
//	�A�N�^�[�A�j���[�V�����֐�
//////////////////////////////////////////////////////////
static void DummyMoveSeq(actWork* xreg)
{
	return;	
}

//�S���h���̈ړ�
static void rwd_dondola_move(actWork* xreg)
{
	if(sg_pWork->uc_state == 0xFF) return;

	if(ScriptParameter0 == RWD_UP)
	{
		xreg->x = xreg->work[0] - (u8)((float)sg_pWork->us_time*0.14f);
		xreg->y = xreg->work[1] - (u8)((float)sg_pWork->us_time*0.067f);
	}
	else{
		xreg->x = xreg->work[0] + (u8)((float)sg_pWork->us_time*0.14f);
		xreg->y = xreg->work[1] + (u8)((float)sg_pWork->us_time*0.067f);	
	}
	return;
}

static void rwd_hero_move(actWork* xreg)
{
	if(sg_pWork->uc_state == 0xFF) return;
	
	if(ScriptParameter0 == RWD_UP)
	{
		xreg->x = xreg->work[0] - (u8)((float)sg_pWork->us_time*0.14f);
		xreg->y = xreg->work[1] - (u8)((float)sg_pWork->us_time*0.067f);
	}
	else{
		xreg->x = xreg->work[0] + (u8)((float)sg_pWork->us_time*0.14f);
		xreg->y = xreg->work[1] + (u8)((float)sg_pWork->us_time*0.067f);	
	}

	//�L�������㉺�ɂ҂傱�҂傱�����悤��
	switch(xreg->work[2])
	{
	case 0:
		xreg->dy = 17;
		if(xreg->work[3]++ >= 10)
		{
			xreg->work[3] = 0;
			xreg->work[2]++;
		}
		break;
	default:
		xreg->dy = 16;
		if(xreg->work[3]++ >= 10)
		{
			xreg->work[3] = 0;
			xreg->work[2] = 0;
		}
		break;	
	}
	return;
}

//���X�Ζʂ����؂�L�����N�^�̓���(�S���h������鎞�p)
static void rwd_man_moveUp(actWork* xreg)
{
	if(xreg->work[0] == 0){		//�L�����N�^�T�C�Y�ɂ��ʒu�␳
		xreg->x += xreg->cx*2;
		xreg->y += (xreg->cy+16);
	}
	
	xreg->work[0]++;	//�^�C�}�[�A�b�v
	if(xreg->work[0] < xreg->work[2])	return;
	
	switch(xreg->work[1])
	{
	case 0:
		xreg->x++;
		if(xreg->work[0] % 4 == 0) xreg->y++;

		break;
	case 1:
		if(xreg->work[0] % 2 == 0) break;
		xreg->x++;
		if(xreg->x % 4 == 0)xreg->y++;

		break;
	}

	if(xreg->y > 160) DelActor(xreg);
	return;
}
//���X�Ζʂ����؂�L�����N�^�̓���(�S���h�������鎞�p)
static void rwd_man_moveDown(actWork* xreg)
{
	if(xreg->work[0] == 0)	//�L�����N�^�T�C�Y�ɂ��ʒu�␳
	{
		xreg->y += (xreg->cy+16);
	}
	xreg->work[0]++;

	if(xreg->work[0] < xreg->work[2]) return;

	switch(xreg->work[1])
	{
	case 0:
		xreg->x--;	
		if(xreg->work[0]%4 == 0)xreg->y--;
		break;
	case 1:	
		if(xreg->work[0] % 2 == 0) break;
		xreg->x--;
		if(xreg->x % 4 == 0)xreg->y--;
	}
	if(xreg->y < 80) DelActor(xreg);
}


//////////////////////////////////////////////////////
//	�t�@�C����	���[�J���֐�
//////////////////////////////////////////////////////
/*----------------------------------------------------
	�S�X�N���[���̏�����
	
	static void SetScreenState(u8	setflg);
	
	i	u8	RWD_CLEAR	0:�S���W�X�^�N���A
			RWD_START	1:�g�p����l�ŃZ�b�g
------------------------------------------------------*/
static void SetScreenState(u8	setflg){
	
	switch(setflg)
	{
	default:
	case RWD_CLEAR:
		*(vu16 *)REG_WININ = 0;
		*(vu16 *)REG_WINOUT = 0;
		*(vu16 *)REG_WIN0H = 0;
		*(vu16 *)REG_WIN1H = 0;
		*(vu16 *)REG_WIN0V = 0;
		*(vu16 *)REG_WIN1V = 0;
		
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

		*(vu16 *)REG_BLDCNT = 0;
    	return;
    case RWD_START:

		*(vu16 *)REG_WININ = 0;
		*(vu16 *)REG_WINOUT = 0;
		*(vu16 *)REG_WIN0H = 0;
		*(vu16 *)REG_WIN1H = 0;
		*(vu16 *)REG_WIN0V = 0;
		*(vu16 *)REG_WIN1V = 0;

		//BG�X�N���[���̏����l���Z�b�g
		if(ScriptParameter0 == RWD_UP)
		{
			sg_pWork->scroll[RWD_BG_POLE].uc_x = (u8)RWD_U_POLE_X;
			sg_pWork->scroll[RWD_BG_POLE].uc_y = (u8)RWD_U_POLE_Y;
			sg_pWork->scroll[RWD_BG_TREE].uc_x = RWD_U_TREE_X;
			sg_pWork->scroll[RWD_BG_TREE].uc_y = RWD_U_TREE_Y;
			sg_pWork->scroll[RWD_BG_MT].uc_y = 0;
			sg_pWork->scroll[RWD_BG_MT].uc_y = 0;
		}
		else{
			sg_pWork->scroll[RWD_BG_POLE].uc_x = (u8)RWD_D_POLE_X;
			sg_pWork->scroll[RWD_BG_POLE].uc_y = (u8)RWD_D_POLE_Y;
			sg_pWork->scroll[RWD_BG_TREE].uc_x = RWD_D_TREE_X;
			sg_pWork->scroll[RWD_BG_TREE].uc_y = RWD_D_TREE_Y;
			sg_pWork->scroll[RWD_BG_MT].uc_y = 0;
			sg_pWork->scroll[RWD_BG_MT].uc_y = 0;
		}
		*(vu16 *)REG_BG3HOFS	= sg_pWork->scroll[RWD_BG_POLE].uc_x;
		*(vu16 *)REG_BG3VOFS	= sg_pWork->scroll[RWD_BG_POLE].uc_y;
		*(vu16 *)REG_BG2HOFS	= 0;
		*(vu16 *)REG_BG2VOFS	= 0;
		*(vu16 *)REG_BG1HOFS	= sg_pWork->scroll[RWD_BG_TREE].uc_x;
		*(vu16 *)REG_BG1VOFS	= sg_pWork->scroll[RWD_BG_TREE].uc_y; 
		*(vu16 *)REG_BG0HOFS	= sg_pWork->scroll[RWD_BG_MT].uc_x;
		*(vu16 *)REG_BG0VOFS	= sg_pWork->scroll[RWD_BG_MT].uc_y; 	
		
	  	*(vu16 *)REG_BG0CNT =	BG_SCREEN_SIZE_0 |			//256*256
								BG_LOOP_ON |				//��荞�ݕ\��ON:
								BG_COLOR_16 |				//16�F�p���b�g���[�h
								BG_PRIORITY_1 |				//�D��x�O
								BG_MOS_OFF |				//���U�C�NOFF
								28 << BG_SCREEN_BASE_SHIFT |	//ScreenBaseBlock_0
								0 << BG_CHAR_BASE_SHIFT;	//ChaeBase_3
		*(vu16 *)REG_BG1CNT =	BG_SCREEN_SIZE_0 |			//256*256
								BG_LOOP_ON |				//��荞�ݕ\��ON:
								BG_COLOR_16 |				//16�F�p���b�g���[�h
								BG_PRIORITY_2 |				//�D��x�O
								BG_MOS_OFF |				//���U�C�NOFF
								29 << BG_SCREEN_BASE_SHIFT |	//ScreenBaseBlock_0
								0 << BG_CHAR_BASE_SHIFT;	//ChaeBase_3
		*(vu16 *)REG_BG2CNT =	BG_SCREEN_SIZE_0 |			//256*256
								BG_LOOP_ON |				//��荞�ݕ\��ON:
								BG_COLOR_16 |				//16�F�p���b�g���[�h
								BG_PRIORITY_3 |				//�D��x�O
								BG_MOS_OFF |				//���U�C�NOFF
								30 << BG_SCREEN_BASE_SHIFT |	//ScreenBaseBlock_0
								0 << BG_CHAR_BASE_SHIFT;	//ChaeBase_3
		*(vu16 *)REG_BG3CNT =	BG_SCREEN_SIZE_0 |			//256*256
								BG_LOOP_ON |				//��荞�ݕ\��ON:
								BG_COLOR_16 |				//16�F�p���b�g���[�h
								BG_PRIORITY_0 |				//�D��x�O
								BG_MOS_OFF |				//���U�C�NOFF
								31 << BG_SCREEN_BASE_SHIFT |	//ScreenBaseBlock_0
								0 << BG_CHAR_BASE_SHIFT;	//ChaeBase_3

		//BGMode0,�\��
		*(vu16 *)REG_DISPCNT =	DISP_MODE_0 |			//BG_MODE0
								DISP_OBJ_CHAR_1D_MAP |	//OBJ_VRAM�P�����}�b�v
								DISP_BG0_ON |
								DISP_BG1_ON |
								DISP_BG2_ON |
								DISP_BG3_ON |
								DISP_OBJ_ON;			//OBJ�\��
		
		//�u�����f�B���O�p�����[�^�ݒ�
		*(vu16 *)REG_BLDCNT = BLD_2ND_ALL;
  		break;
  	}
    return;
}

typedef void (*pActMoveSeq)(actWork*);
/*-------------------------------------------
	�g�p����I�u�W�F�N�g�̏�����
	���Ɖ���A��l���̐��ʂ��݂āA�����ʒu�Ȃǂ�p��
	
	static void demo_ropeway_init(void);
--------------------------------------------*/
static void demo_ropeway_init(void)
{
	u8	uc_sex[2] = {HERO,HEROINE};
	u8	ct = 0,uc_no = 0;
	u16	rseed = pp_rand();
	u8	uc_aPerson[RWD_RND_MAX] = {HILL1,BOYSCOUT1,GIRLSCOUT1,ATTACK1};
	s16	us_aPos[2][2] = {{RWD_U_MAN_X,RWD_U_MAN_Y},{RWD_D_MAN_X,RWD_D_MAN_Y}};
	u8	uc_aTime[RWD_RND_MAX] = {0,60,120,170};
	pActMoveSeq	actTbl[2] = {rwd_man_moveUp,rwd_man_moveDown};

	switch(ScriptParameter0)
	{
	default:
	case RWD_UP:	//���
		//���ʂɉ����Ď��@OBJ�Z�b�g
		uc_no = EvObjMakeAddAct(uc_sex[MyData.my_sex],rwd_hero_move,
								RWD_U_G1_X+RWD_HERO_OFSX,
								RWD_U_G1_Y+RWD_HERO_OFSY,RWD_OPRI_HERO);
		if(uc_no != ACT_MAX){
			ActWork[uc_no].oamData.Priority = 2;
			ActWork[uc_no].dx = 8;	//OBJ�̍���̍��W�ōl������悤�ɃI�t�Z�b�g
			ActWork[uc_no].dy = 16;
			ActWork[uc_no].work[0] = RWD_U_G1_X+RWD_HERO_OFSX;	//�����ʒu�L��
			ActWork[uc_no].work[1] = RWD_U_G1_Y+RWD_HERO_OFSY;
		}

		//�S���h���̖{��
		uc_no = AddActor(&(RWD_Header[0]),	RWD_U_G1_X,				//�S���h���{��
											RWD_U_G1_Y,
											RWD_OPRI_G1);
		ActWork[uc_no].dx = ActWork[uc_no].dy = 32;
		ActWork[uc_no].work[0] = RWD_U_G1_X;
		ActWork[uc_no].work[1] = RWD_U_G1_Y;
		
		//�S���h���̉����L�����N�^
		uc_no = AddActor(&(RWD_Header[1]),	RWD_U_G1_X+RWD_G2_OFSX,
											RWD_U_G1_Y+RWD_G2_OFSY,
											RWD_OPRI_G2);
		ActWork[uc_no].dx = 8;
		ActWork[uc_no].dy = 4;
		ActWork[uc_no].work[0] = RWD_U_G1_X+RWD_G2_OFSX;
		ActWork[uc_no].work[1] = RWD_U_G1_Y+RWD_G2_OFSY;

		//�V��ݒ�
		sg_pWork->uc_weather = WEATHER_ASH;
		sg_pWork->us_weaTime = RWD_WEATHER_CHANGE;
		WeatherChangeDemo(WEATHER_SHINE2);
		break;
	case RWD_DOWN:		//������̏����l���Z�b�g
		SetChar16R(&(sg_pWork->map[0]),&(((u16 *)GD_BG_MT_MAP)[12*3]),24,26,12,3);
		
		//���ʂɉ����Ď��@OBJ�Z�b�g
		uc_no = EvObjMakeAddAct(uc_sex[MyData.my_sex],rwd_hero_move,
								RWD_D_G1_X+RWD_HERO_OFSX,
								RWD_D_G1_Y+RWD_HERO_OFSY,RWD_OPRI_HERO);
		if(uc_no != ACT_MAX){
			ActWork[uc_no].oamData.Priority = 2;
			ActWork[uc_no].dx = 8;
			ActWork[uc_no].dy = 16;
			ActWork[uc_no].work[0] = RWD_D_G1_X+RWD_HERO_OFSX;
			ActWork[uc_no].work[1] = RWD_D_G1_Y+RWD_HERO_OFSY;
		}
		
		//�S���h���̖{�̂�ADD
		uc_no = AddActor(&(RWD_Header[0]),	RWD_D_G1_X,				//�S���h���{��
											RWD_D_G1_Y,
											RWD_OPRI_G1);
		ActWork[uc_no].dx = ActWork[uc_no].dy = 32;
		ActWork[uc_no].work[0] = RWD_D_G1_X;
		ActWork[uc_no].work[1] = RWD_D_G1_Y;
		
		//�S���h���̉����p�[�c��ADD
		uc_no = AddActor(&(RWD_Header[1]),	RWD_D_G1_X+RWD_G2_OFSX,	//�S���h������
											RWD_D_G1_Y+RWD_G2_OFSY,
											RWD_OPRI_G2);
		ActWork[uc_no].dx = 8;
		ActWork[uc_no].dy = 4;
		ActWork[uc_no].work[0] = RWD_D_G1_X+RWD_G2_OFSX;
		ActWork[uc_no].work[1] = RWD_D_G1_Y+RWD_G2_OFSY;
		
		//�V��Z�b�g
		sg_pWork->uc_weather = WEATHER_SHINE2;
		sg_pWork->us_weaTime = RWD_TIME_END - RWD_WEATHER_CHANGE+45;
		WeatherChangeDemo(WEATHER_ASH);
		break;
	}
	//�P�[�u��
	for(ct = 0;ct < 9;ct++)
	{
		uc_no = AddActor(&(RWD_Header[2]),	16*ct+RWD_CABLE_X,
											8*ct+RWD_CABLE_Y,
											RWD_OPRI_CABLE);
		ActWork[uc_no].dx = 8;
		ActWork[uc_no].dy = 8;
	}
	//�����_���ŎΖʂ��L�����N�^���ʂ�߂���
	if(rseed%64 == 0)
	{
		uc_no = EvObjMakeAddAct(
			uc_aPerson[(rseed%3)],
			actTbl[ScriptParameter0],
			us_aPos[ScriptParameter0][0],
			us_aPos[ScriptParameter0][1],RWD_OPRI_MAN);
		
		if(uc_no != ACT_MAX)
		{
			ActWork[uc_no].oamData.Priority = 2;
			ActWork[uc_no].dx = -ActWork[uc_no].cx;
			ActWork[uc_no].dy = -ActWork[uc_no].cy;

			if(ScriptParameter0 == 0)
			{
				if(rseed&0x0001){
					ActAnmChg(&(ActWork[uc_no]),WALK_L_1_ST);
					ActWork[uc_no].work[1] = 1;
					ActWork[uc_no].y += 2;
				}
				else{
					ActAnmChg(&(ActWork[uc_no]),WALK_R_1_ST);
					ActWork[uc_no].work[1] = 0;
				}
			}
			else{
				if(rseed&0x0001){
					ActAnmChg(&(ActWork[uc_no]),WALK_R_1_ST);
					ActWork[uc_no].work[1] = 1;
					ActWork[uc_no].y += 2;
				}
				else{
					ActAnmChg(&(ActWork[uc_no]),WALK_L_1_ST);
					ActWork[uc_no].work[1] = 0;
				}
			}
			ActWork[uc_no].work[2] = uc_aTime[rseed%RWD_RND_MAX];
		}
	}
	return;
}

////////////////////////////////////////////////////////////////
//	�R���̃X�N���[���֘A
////////////////////////////////////////////////////////////////
/*-------------------------------------------------------
	�R���̃X�N���[�� �� BG��ԏ���
		��ԗp�̃}�b�v���o�b�t�@�ɗp�ӂ���

	static void MtScrollMakeBuf(void);
------------------------------------------------------*/
static void MtScrollMakeBuf(void)
{
	u8	ct = 0,count = 0;
	u8	i = 0,j = 0;

	i = 12*3*(sg_pWork->MtScrl.uc_blockNo+2);
	for(ct = 0;ct < 3;ct++)
	{
		for(count = 0;count < 12;count++)
		{
			sg_pWork->MtScrl.us_buf[ct][count] = ((u16 *)GD_BG_MT_MAP)[i++];
			sg_pWork->MtScrl.us_buf[ct+3][count] = ((u16 *)GD_BG_MT_MAP)[j];
			sg_pWork->MtScrl.us_buf[ct+6][count] = ((u16 *)GD_BG_MT_MAP)[j+36];
			j++;
		}
	}
	//�u���b�N�i���o�[�ύX
	sg_pWork->MtScrl.uc_blockNo = (sg_pWork->MtScrl.uc_blockNo+1)%3;
	
}

/*---------------------------------------------------
	�R���̃X�N���[�� �� BG��ԏ���
		�̂ڂ�̃X�N���[���A���C������

	static void MtScrollUp(void);
-----------------------------------------------------*/
static void MtScrollUp(void){
	//�X�N���[���X�N���[������
	sg_pWork->MtScrl.uc_scrScroll = (sg_pWork->MtScrl.uc_scrScroll+1)%96;
	
	sg_pWork->scroll[RWD_BG_MT].uc_x = 
		sg_pWork->MtScrl.uc_oldX - sg_pWork->MtScrl.uc_dx;
	sg_pWork->scroll[RWD_BG_MT].uc_y = 
		sg_pWork->MtScrl.uc_oldY - sg_pWork->MtScrl.uc_dy;
	
	sg_pWork->MtScrl.uc_dx++;
	if(sg_pWork->MtScrl.uc_dx % 4 == 0) sg_pWork->MtScrl.uc_dy++;

	if(sg_pWork->MtScrl.uc_dx <= 16) return;

	//BG�̕�ԏ���
	MtScrollTransUp();

}

/*---------------------------------------------------
	�R���̃X�N���[�� �� BG��ԏ���
		����̃X�N���[���A���C������

	static void MtScrollDown(void);
-----------------------------------------------------*/
static void MtScrollDown(void){
	//�X�N���[���X�N���[������
	sg_pWork->MtScrl.uc_scrScroll = (sg_pWork->MtScrl.uc_scrScroll+1)%96;
	
	sg_pWork->scroll[RWD_BG_MT].uc_x = 
		sg_pWork->MtScrl.uc_oldX + sg_pWork->MtScrl.uc_dx;
	sg_pWork->scroll[RWD_BG_MT].uc_y = 
		sg_pWork->MtScrl.uc_oldY + sg_pWork->MtScrl.uc_dy;
	
	sg_pWork->MtScrl.uc_dx++;
	if(sg_pWork->MtScrl.uc_dx % 4 == 0) sg_pWork->MtScrl.uc_dy++;

	if(sg_pWork->MtScrl.uc_dx <= 16) return;

	//BG�̕�ԏ���
	MtScrollTransDown();

}

/*---------------------------------------------------
	�R���̃X�N���[�� �� BG��ԏ���
		�̂ڂ�̃X�N���[���ABG��ԏ���

	static void MtScrollTransUp(void);
-----------------------------------------------------*/
static void MtScrollTransUp(void){

	u8	ct = 0;
	static u8	ni = 0;
	static u8	nj = 0;
	static u8	nk = 0;

	//�I�t�Z�b�g�ƌÂ��l���X�V
	sg_pWork->MtScrl.uc_dx = sg_pWork->MtScrl.uc_dy = 0;
	sg_pWork->MtScrl.uc_oldX = sg_pWork->scroll[RWD_BG_MT].uc_x;
	sg_pWork->MtScrl.uc_oldY = sg_pWork->scroll[RWD_BG_MT].uc_y;
	
	//���ɕ�Ԃ��郉�C�����v�Z
	sg_pWork->MtScrl.uc_sLine = (sg_pWork->MtScrl.uc_sLine-2+32)%32;
	sg_pWork->MtScrl.cBlockCt -= 2;

	//�摜��]��
	nk = (sg_pWork->MtScrl.uc_dLine-9+32)%32;
	for(ct = 0;ct < 9;ct++)
	{
		ni = sg_pWork->MtScrl.uc_sLine;
		nj = (nk+ct)%32;

		sg_pWork->map[0].usa[nj][ni] = 
			sg_pWork->MtScrl.us_buf[ct][(u8)sg_pWork->MtScrl.cBlockCt];
		
		ni = (ni+1)%32;
		sg_pWork->map[0].usa[nj][ni] = 
			sg_pWork->MtScrl.us_buf[ct][sg_pWork->MtScrl.cBlockCt+1];
	}
	//���ݑ|��
	ni = (sg_pWork->MtScrl.uc_sLine -2+32)%32;
	FillChar16R(&(sg_pWork->map[0]),0x0000,ni,0,2,32);
	
	//1�u���b�N���̓]��������ł��Ȃ���΋A��
	if(sg_pWork->MtScrl.cBlockCt != 0) return;

	//�]���p�o�b�t�@���X�V
	sg_pWork->MtScrl.uc_dLine = (sg_pWork->MtScrl.uc_dLine-3+32)%32;
	sg_pWork->MtScrl.cBlockCt = 12;
	MtScrollMakeBuf();
	
	//���ݑ|��
	ni = (sg_pWork->MtScrl.uc_dLine+1)%32;
	FillChar16R(&(sg_pWork->map[0]),0x0000,0,ni,32,9);
	return;
}

/*---------------------------------------------------
	�R���̃X�N���[�� �� BG��ԏ���
		����̃X�N���[���ABG��ԏ���

	static void MtScrollTransUp(void);
-----------------------------------------------------*/
static void MtScrollTransDown(void){

	u8	ct = 0;
	static u8	ni = 0;
	static u8	nj = 0;
	static u8	nk = 0;

	//�I�t�Z�b�g�ƌÂ��l���X�V
	sg_pWork->MtScrl.uc_dx = sg_pWork->MtScrl.uc_dy = 0;
	sg_pWork->MtScrl.uc_oldX = sg_pWork->scroll[RWD_BG_MT].uc_x;
	sg_pWork->MtScrl.uc_oldY = sg_pWork->scroll[RWD_BG_MT].uc_y;
	
	//���ɕ�Ԃ��郉�C�����v�Z
	sg_pWork->MtScrl.uc_sLine = (sg_pWork->MtScrl.uc_sLine+2)%32;
	sg_pWork->MtScrl.cBlockCt += 2;

	//�摜��]��
	nk = (sg_pWork->MtScrl.uc_dLine);//+9)%32;
	for(ct = 0;ct < 9;ct++)
	{
		ni = sg_pWork->MtScrl.uc_sLine;
		nj = (nk+ct)%32;

		sg_pWork->map[0].usa[nj][ni] = 
			sg_pWork->MtScrl.us_buf[ct][(u8)sg_pWork->MtScrl.cBlockCt];
		
		ni = (ni+1)%32;
		sg_pWork->map[0].usa[nj][ni] = 
			sg_pWork->MtScrl.us_buf[ct][sg_pWork->MtScrl.cBlockCt+1];
	}
	//���ݑ|��
	nj = (sg_pWork->MtScrl.uc_dLine -9+32)%32;
	FillChar16R(&(sg_pWork->map[0]),0x0000,sg_pWork->MtScrl.uc_sLine,nj,2,9);
	
	//1�u���b�N���̓]��������ł��Ȃ���΋A��
	if(sg_pWork->MtScrl.cBlockCt != 10) return;

	//�]���p�o�b�t�@���X�V
	sg_pWork->MtScrl.uc_dLine = (sg_pWork->MtScrl.uc_dLine+3)%32;
	sg_pWork->MtScrl.cBlockCt = -2;
	MtScrollMakeBuf();
	
	return;
}

/*---------------------------------------------------
	�R���̃X�N���[�� �� BG��ԏ���
		��肩���肩���p�����[�^�ɁA�X�N���[���̏����l����
		�ŏ��K�v�ɂȂ�f�[�^���p�ӂ��Ă���
		
	static void MtScrollInit(void);
-----------------------------------------------------*/
static void MtScrollInit(u8 flg)
{
	switch(flg)
	{
	default:
	case RWD_UP:
		sg_pWork->MtScrl.uc_blockNo = 2;
		sg_pWork->MtScrl.uc_sLine = 0;
		sg_pWork->MtScrl.uc_dLine = 20;
		sg_pWork->MtScrl.cBlockCt = 12;

		//�]���p�̃o�b�t�@��p��
		MtScrollMakeBuf();
		MtScrollTransUp();
		
		break;
	case RWD_DOWN:
		sg_pWork->MtScrl.uc_blockNo = 2;
		sg_pWork->MtScrl.uc_sLine = 28;
		sg_pWork->MtScrl.uc_dLine = 20;
		sg_pWork->MtScrl.cBlockCt = 4;

		//�]���p�̃o�b�t�@��p��
		MtScrollMakeBuf();
		MtScrollTransDown();
		break;
	}
	sg_pWork->MtScrl.uc_scrScroll = 0;
	return;
}

