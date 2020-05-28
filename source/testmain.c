//======================================================
//    testmain.c                                           
//                                                      
//    Copyright (C) 2000 GAME FREAK inc.
//======================================================

#ifdef	PM_DEBUG

#include "common.h"
#include "intr.h"
#include "print.h"
#include "menu.h"
#include "palanm.h"
#include "task.h"
#include "laster.h"



extern	void StartMenuInit(void);


//-------------------- �T�u���[�`���錾 ------------------------
void testMainInit(void);
void testMain(void);
void testMainExit(void);

static void test_v_blank_intr(void);


//==============================================================
//                       ���C�����[�`��
//==============================================================
void testMainInit(void)
{
	m4aSoundVSyncOff();
	SetVBlankFunc(NULL);							// V ���ݸٰ�ݾ��(������)
	DmaClear(3, 0, VRAM,VRAM_SIZE, 32);		// �SVRAM������
	DmaClear(3, 0, PLTT,PLTT_SIZE, 32);		// �SVRAM������
	PaletteAnimeInit();
	InitActor();
	InitTask();
	LasterInit();
	PrintScreenInit(FIELD_MENU_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);
	StartMenuInit();
	PaletteFadeReq(0xffffffff,0,16,0,0);

    *(vu16 *)REG_IE |= V_BLANK_INTR_FLAG;			// V�u�����N�������t���O�̃Z�b�g
    *(vu16 *)REG_DISPCNT =	DISP_MODE_0 |
							DISP_OBJ_ON |
							DISP_BG0_ON |			// MSG Window
							DISP_OBJ_CHAR_1D_MAP;

	m4aSoundVSyncOn();
    SetVBlankFunc( test_v_blank_intr );

	m4aSongNumStart(MUS_TITLE3);

	MainProcChange( testMain );
}

void testMain(void)
{
	if(PaletteAnime() == 0){
		MainTask();
		JumpActor();
		SetActor();
	}
}

void testMainExit(void)
{
	if(PaletteAnime() == 0){
		SetVBlankFunc(NULL);
		DmaClear(3, 0, VRAM,VRAM_SIZE, 32);		// �SVRAM������
		DmaClear(3, 0, PLTT,PLTT_SIZE, 32);		// �SVRAM������
		MainProcChange(sys.pReturnProc);
	}
}

//==============================================================
//                    ���荞�݃��[�`��
//==============================================================

//--------------------------------------------------------------
//                    �u�u�����N�֐�
//--------------------------------------------------------------
static void test_v_blank_intr(void)
{
	actDMAtrans();
	actOAMtrans();
	PaletteFadeTrans();
}

#endif	PM_DEBUG

