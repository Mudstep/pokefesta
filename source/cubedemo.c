/***********************************************************************
**	�L���[�u�f��
**                                                               taya
***********************************************************************/
#include "tnavi_com.h"
#include "fld_main.h"
#include "poketool.h"
#include "objdata.h"
#include "a_enemy.h"
#include "decord.h"
#include "mus_tool.h"
#include "laster.h"
#include "fld_sio.h"
#include "window.h"

#include "cubecase.h"
#include "cubedemo.dat"

#include "message.h"

/*------------------------------------*/
/*  �萔                              */
/*------------------------------------*/

// �Z���E�p���b�g�h�c
//�i�g���i�r�̃|�P�����摜�Əd�Ȃ�Ȃ��悤�Ɂj
#define CELLID_UPDOWN  (0)
#define PALID_UPDOWN   (0)

// ���b�Z�[�W�E�B���h�E���W
#define MSGWIN_X1  (0)
#define MSGWIN_X2  (29)
#define MSGWIN_Y1  (16)
#define MSGWIN_Y2  (19)

// �͂��E�������E�B���h�E���W
#define YESNOWIN_X1  (23)
#define YESNOWIN_Y1  (10)

// �|�P�����E�B���h�E�㏸��
#define POKEWIN_UP  (40)

// �f�[�^�\�����㏸��
#define DATAAREA_UP  (6)

enum CHECK_TYPE {
	CHECK_CANCEL,
	CHECK_GIVE,
};

/*------------------------------------*/
/*  �O���[�o��                        */
/*------------------------------------*/
const int ConditionID[] = {
	ID_style, ID_strong, ID_clever, ID_cute, ID_beautiful,
};

static const u8 TasteID[] = {
	TASTE_KARAI, TASTE_SUPPAI, TASTE_NIGAI, TASTE_AMAI, TASTE_SIBUI
};

static CUBEDEMO_WORK *work = NULL;

static pFunc ReturnFunc = NULL;
static CUBE *CubeData = NULL;

extern u16	ReturnItemNo;		// �I�������L���[�u�̔z��ԍ����i�[


u8  CubeDemoPokeNo = 0;   // ���Ԗڂ̃|�P������I�񂾂��H
s16 CubeDemoLike = 0;     // �I�񂾃L���[�u���D�����ǂ����H

/*------------------------------------*/
/*  �v���g�^�C�v                      */
/*------------------------------------*/
static void change_sub(pFunc sub);
static void cubedemo_main(void);
static void cubedemo_v_intr(void);
static void init_result_message(void);
static u8 next_result_message(void);
static void add_updown_actor(void);
static void calc_cube_param(void);
static void calc_add_param(CUBE *cube, PokemonParam *poke);

static void init_check(void);
static s8 wait_check(void);

static u8 is_fur_max(void);
static void disp_tabenai_msg(void);
static void erace_tabenai_msg(void);
static u8 selno_to_pokeno(u8 selno);
static u8 pokeno_to_selno(u8 pokeno);

extern void PolockEatDemoInit(void);

// --- �T�u�v���Z�X -- 
static void sub_init(void);
static void sub_init_comb(void);
static void sub_fadein(void);
static void sub_select(void);
static void sub_todemo(void);
static void sub_demo(void);
static void sub_fadeout(void);
static void sub_cubedemo_recover(void);

/*==============================================================
==  ������
==
==  cube       �L���[�u�f�[�^�\���̂ւ̃|�C���^
==	ret_func   ���A�֐�
==
==============================================================*/
void CubeDemoInit(CUBE *cube,  pFunc ret_func)
{
	work = &(NaviWork->CubeDemoWork);

	work->cube = cube;
	work->ret_func = ret_func;

	NaviWork->font_base_pal = 2;

	change_sub(sub_init);
	MainProcChange(cubedemo_main);
}
/*==============================================================
==  ���A
==
==============================================================*/
static void cubedemo_recover_main1(void);
static void cubedemo_recover_main2(void);

static void CubeDemoRecover(void)
{
	work->cube = CubeData;
	work->ret_func = ReturnFunc;

	CubeDemoPokeNo = pokeno_to_selno(CubeDemoPokeNo);

	work->recover_vec = (CubeDemoPokeNo > 3)?  1 : 0; //4�Ԉȍ~���߹�݂Ȃ�㑗���

	NaviWork->font_base_pal = 2;

	change_sub(sub_init);
	MainProcChange(cubedemo_recover_main1);
}
static void cubedemo_recover_main1(void)
{
	work->sub();
	JumpActor();
	SetActor();
	PaletteAnime();

	if(work->sub == sub_fadein)
	{
		*(vu16*)REG_DISPCNT = 0;
		work->prog = 0;
		MainProcChange(cubedemo_recover_main2);
	}

}
static void cubedemo_recover_main2(void)
{
	sub_cubedemo_recover();
	JumpActor();
	SetActor();
	PaletteAnime();
}

/*====================================================*/
/*  ���C���v���Z�X                                    */
/*====================================================*/
static void cubedemo_main(void)
{
	work->sub();
	JumpActor();
	SetActor();
	PaletteAnime();
}


/*====================================================*/
/*  V Blank                                           */
/*====================================================*/
static void cubedemo_v_intr(void)
{
	actOAMtrans();
	actDMAtrans();
	PaletteFadeTrans();
	CubeDemo_CalcRador(DATAAREA_UP);
	LasterVblank();
}
/*====================================================*/
/*  �T�u�v���Z�X�ύX                                  */
/*====================================================*/
static void change_sub(pFunc sub)
{
	work->sub = sub;
	work->prog = 0;
}
/***********************************************************************
**  �T�u�v���Z�X                                                      **
***********************************************************************/
#define TEXT_CHAR_BLOCK   (0)
#define TEXT_MAP_BLOCK    (31)
#define TEXT_MAP_ADDR     (BG_VRAM+TEXT_MAP_BLOCK*0x800)

/*====================================================*/
/*  ��ʏ�����                                        */
/*====================================================*/
static void sub_init(void)
{
	switch(work->prog){
	case 0:
		NaviWork->comm_flg = IsFieldCommMode();
		if(NaviWork->comm_flg == FALSE){
			work->init_end = 0;
			change_sub(sub_init_comb);
			work->prog++;
		}
		break;

	case 1:
		InitActor();
		ObjPalManInit();
		work->prog++;
		break;

	case 2:
		SetVBlankFunc(NULL);
		work->prog++;
		break;

	case 3:
		PrintScreenInit(TNAVI_TYPE);
		work->prog++;
		break;

	case 4:
		NMenuScreenInitStart(TNAVI_TYPE);
		work->prog++;
		break;

	case 5:
		if(NMenuScreenInitMain()){
			work->prog++;
		}
		break;

	case 6:
		NaviWork->condition_change_func = CONDITION_MODE_STOCK;
		NaviWork->combg_vofs = 32;
		work->prog++;
		break;

	case 7:
		TNaviObj_LabelInitBegin();
		work->prog++;
	case 8:
		if(!TNaviObj_LabelInit()){
			work->prog++;
		}
		break;

	case 9:
		TNaviObj_AddLabel_Begin(LABEL_CONDITION);
		work->prog++;
	case 10:
		if(!TNaviObj_AddLabel(LABEL_CONDITION)){
			work->prog++;
		}
		break;

	case 11:
		KeyRepeatStartWait= 20;

		NaviWork->stock_poke_cnt = PokeCountMineGet();
		NaviWork->calc_win_flag = 0;
		NaviWork->poke_actor = NULL;

		TNaviSub_ConditionInitStock();
		NaviWork->initbg_prog = 0;
		work->prog++;
		break;

	case 12:
		if(!TNaviBG_CubeDemoInit()){
			*(vu16*)REG_BG2VOFS = DATAAREA_UP;
			*(vu16*)REG_BG3VOFS = DATAAREA_UP;
			work->prog++;
		}
		break;

	case 13:
		TNaviObj_UpdatePokeImg(0);
		NaviWork->poke_actor->dy = -(POKEWIN_UP);
		work->prog++;
		break;

	case 14:
		if(!TNaviBG_PokeWinUp()){
			work->prog++;
		}
		break;

	case 15:
		TNaviObj_StockConInit();
		work->prog++;
		break;

	case 16:
		DIV_DMACLEAR(3, 0, TEXT_MAP_ADDR, 0x800, 32);
		*(vu16 *)REG_BG1VOFS = 0;
		*(vu16 *)REG_BG1HOFS = 0;
		*(vu16 *)REG_BG1CNT = BG_COLOR_16
							| BG_SCREEN_SIZE_0
							| BG_PRIORITY_0
							| TEXT_MAP_BLOCK << BG_SCREEN_BASE_SHIFT
							| 0 << BG_CHAR_BASE_SHIFT
							;
		work->prog++;
		break;

	case 17:
		TNaviSub_ParamToPos(NaviWork->poke_param[0], NaviWork->point[0]);
		TNaviSub_LadorLasterInitBegin();
		work->prog++;
		break;

	case 18:
		if(!TNaviSub_LadorLasterInitMain()){
			work->prog++;
		}
		break;

	case 19:
		TNaviSub_SetRadorPoint(NaviWork->point[0]);
		work->prog++;
		break;

	case 20:
		TNaviBG_PokeSeikakuWrite();
		work->prog++;
		break;

	case 21:
		*(vu16*)REG_WIN0H = 240;
		*(vu16*)REG_WIN1H = RADOR_CX;
		*(vu16*)REG_WIN0V = ((RADOR_TOP-DATAAREA_UP) << 8) | (RADOR_BOTTOM-DATAAREA_UP);
		*(vu16*)REG_WIN1V = ((RADOR_TOP-DATAAREA_UP) << 8) | (RADOR_BOTTOM-DATAAREA_UP);
		*(vu16*)REG_WININ = 0x3f3f;   // �����F�S�ĕ\��
		*(vu16*)REG_WINOUT = 0x001B;  // �O���FBG2 �ȊO��S�ĕ\��

		*(vu16*)REG_BG0VOFS = POKEWIN_UP;

		*(vu16 *)REG_DISPCNT = DISP_MODE_0 | DISP_OBJ_CHAR_1D_MAP 
		| DISP_BG0_ON
		| DISP_BG1_ON
		| DISP_BG2_ON
		| DISP_BG3_ON
		| DISP_OBJ_ON
		| DISP_WIN01_ON
		;

	case 22:
		work->init_end = 1;
		change_sub(sub_fadein);
		break;
	}

}
// �������Ȃ�������
static void sub_init_comb(void)
{
	while(work->init_end == 0)
	{
		sub_init();
	}
}


/*====================================================*/
/*  �t�F�[�h�C��                                      */
/*====================================================*/
static void sub_fadein(void)
{
	switch(work->prog){
	case 0:
		PaletteFadeReq(0xffffffff, 0, 16, 0, 0x0000);
		SetVBlankFunc(cubedemo_v_intr);
		work->prog++;
		break;

	case 1:
		if(FadeData.fade_sw == 0){
			TNaviObj_KiraInit();
			TNaviObj_KiraStart();
			change_sub(sub_select);
		}
		break;
	}
}
/*====================================================*/
/*  �|�P�����I��                                      */
/*====================================================*/
static void sub_select(void)
{
	switch(work->prog){
	case 0:
		if(sys.Cont & U_KEY){
			SePlay(SE_SELECT);
			TNaviSub_ConditionChangeInit(1);
			TNaviObj_KiraStop();
			work->prog = 1;
			break;
		}
		if(sys.Cont & D_KEY){
			SePlay(SE_SELECT);
			TNaviSub_ConditionChangeInit(0);
			TNaviObj_KiraStop();
			work->prog = 1;
			break;
		}
		if(sys.Trg&B_BUTTON){
			SePlay(SE_SELECT);
			work->prog = 3;
			break;
		}
		if(sys.Trg&A_BUTTON){
			SePlay(SE_SELECT);
			if(NaviWork->condition_poke_sel == NaviWork->condition_poke_cnt - 1){
				work->prog = 3;
			}else{
				work->prog = 5;
			}
			break;
		}
		break;


	// �|�P�����؂�ւ��҂���FIFO�J���҂�
	case 1:
		if(!TNaviSub_ConditionChange()){
			work->prog++;
		}
		break;
	case 2:
		if(FieldRecvFifoClearWait() == FALSE){
			TNaviBG_PokeSeikakuWrite();
			TNaviObj_KiraStart();
			work->prog = 0;
		}
		break;


	// ���~�m�F
	case 3:
		change_sub(sub_fadeout);
		break;

	// ������m�F
	case 5:
		init_check();
		work->prog++;
		break;

	case 6:
		switch(wait_check()){
		case CURSOR_NO_VALUE:
		case CURSOR_CANCEL_VALUE:
			work->prog = 0;
			break;

		case CURSOR_YES_VALUE:
			if(is_fur_max()){
				disp_tabenai_msg();
				work->prog = 7;
			}else{
				change_sub(sub_todemo);
			}
			break;
		}
		break;

	case 7:
		if(sys.Trg&(A_BUTTON|B_BUTTON)){
			erace_tabenai_msg();
			work->prog = 0;
		}
		break;
	}
}
/*====================================================*/
/*  �|�P�������|���b�N��H�ׂ�f����ʂ֐؂�ւ�      */
/*====================================================*/
static void sub_todemo(void)
{
	switch(work->prog){
	case 0:
		CubeDemoPokeNo = selno_to_pokeno(NaviWork->condition_poke_sel);
		ReturnFunc = work->ret_func;
		CubeData = work->cube;
		PaletteFadeReq(0xffffffff,0,0,16,0x0000);
		work->prog++;
		break;

	case 1:
		if(FadeData.fade_sw==0){
			sys.pReturnProc = CubeDemoRecover;
			PolockEatDemoInit();
		}
		break;
	}
}
/*====================================================*/
/*  ���A��A�I������Ă����|�P�����\����              */
/*====================================================*/
static void sub_cubedemo_recover(void)
{
	switch(work->prog){
	case 0:
		if(NaviWork->condition_poke_sel != CubeDemoPokeNo)
		{
			TNaviSub_ConditionChangeInit(work->recover_vec);
			work->prog++;
		}else{
			work->prog = 3;
		}
		break;

	// �|�P�����؂�ւ��҂���FIFO�J���҂�
	case 1:
		if(!TNaviSub_ConditionChange()){
			work->prog++;
		}
		break;
	case 2:
		if(FieldRecvFifoClearWait() == FALSE){
			TNaviBG_PokeSeikakuWrite();
			work->prog = 0;
		}
		break;

	case 3:
		ColorConceChange(0xffffffff,16,0x0000);
		work->prog++;
		break;

	case 4:
		*(vu16 *)REG_DISPCNT = DISP_MODE_0 | DISP_OBJ_CHAR_1D_MAP 
		| DISP_BG0_ON
		| DISP_BG1_ON
		| DISP_BG2_ON
		| DISP_BG3_ON
		| DISP_OBJ_ON
		| DISP_WIN01_ON
		;
		work->prog++;
		break;

	case 5:
		SetVBlankFunc(cubedemo_v_intr);
		PaletteFadeReq(0xffffffff,0,16,0,0x0000);
		work->prog++;
		break;

	case 6:
		if(FadeData.fade_sw==0){
			TNaviObj_KiraInit();
			TNaviObj_KiraStart();
			change_sub(sub_demo);
			MainProcChange(cubedemo_main);
		}
		break;

	}
}

/*====================================================*/
/*  �\�͕ϓ��f��                                      */
/*====================================================*/
static void sub_demo(void)
{
	switch(work->prog){
	case 0:
		work->pokepara = PokeParaMine;
		work->pokepara += NaviWork->sort_data[NaviWork->condition_poke_sel].poke;
//		write_reaction_message();
		TNaviObj_KiraStop();
		work->prog++;
		break;

	case 1:
		if(sys.Trg&(A_BUTTON|B_BUTTON))
		{
			work->prog++;
		}
		break;

	case 2:
		calc_cube_param();
		TNaviSub_ParamToPos(work->param_after, NaviWork->point[3]);
		TNaviSub_InitMorph(NaviWork->point[NaviWork->PokeSelNow], NaviWork->point[3]);
		add_updown_actor();
		work->prog++;
		break;

	case 3:
		if(!TNaviSub_ProgMorph()){
			u8 pokeno = selno_to_pokeno(NaviWork->condition_poke_sel);
			TNaviSub_UpdateFurLv(pokeno);
			TNaviObj_KiraStart();
			work->counter = 0;
			work->prog++;
		}
		break;

	case 4:
		if(++(work->counter) > 16){
			init_result_message();
			work->prog++;
		}
		break;

	case 5:
		if(sys.Trg&(A_BUTTON|B_BUTTON))
		{
			if(!next_result_message()){
				DelCube((u8)ReturnItemNo);  // �L���[�u����
				change_sub(sub_fadeout);
			}
		}
		break;
	}
}
/*====================================================*/
/*  �t�F�[�h�A�E�g                                    */
/*====================================================*/
static void sub_fadeout(void)
{
	switch(work->prog){
	case 0:
		PaletteFadeReq(0xffffffff, 0, 0, 16, 0x0000);
		work->prog++;
		break;

	case 1:
		if(FadeData.fade_sw==0){
			work->prog++;
		}
		break;

	case 2:
		TNaviSub_LadorLasterQuit();
		work->prog++;
		break;

	case 3:
		MainProcChange(work->ret_func);
		break;
	}
}
/***********************************************************************
**  �m�F�E�B���h�E                                                    **
***********************************************************************/
/*====================================================*/
/*  ������                                            */
/*====================================================*/
static void init_check(void)
{
	u8 pokeno;

	pokeno = selno_to_pokeno(NaviWork->condition_poke_sel);
	PokeParaGet(PokeParaMine + pokeno, ID_nickname, work->buffer);
	NickNameCutOff(work->buffer);
	PM_strcat(work->buffer, PolockMsg_CheckGive);

	NMenuContextChange(TNAVI2_TYPE);
	NMenuWinBoxWrite(MSGWIN_X1, MSGWIN_Y1, MSGWIN_X2, MSGWIN_Y2);
	NMenuMsgWrite(work->buffer, MSGWIN_X1+1, MSGWIN_Y1+1);
	YesNoSelectInit(YESNOWIN_X1, YESNOWIN_Y1);
	NMenuVCursorSet(0);
}
/*====================================================*/
/*  ����҂�                                          */
/*  return:  �ʏ�̃��j���[�I���Ɠ���                 */
/*====================================================*/
static s8 wait_check(void)
{
	s8 ret = NMenuVCursorControlNoloop();

	if(ret == CURSOR_NO_VALUE
	|| ret == CURSOR_YES_VALUE
	|| ret == CURSOR_CANCEL_VALUE
	){
		NMenuScreenClear();
		NMenuContextChange(TNAVI_TYPE);
	}

	return ret;
}
/***********************************************************************
**  ���ʃ��b�Z�[�W�\��                                                **
***********************************************************************/
static void make_result_message(u8 *buf, u8 pos, s16 val);
static void disp_result_message(const u8 *msg);

/*====================================================*/
/* ���������ŏ��̃��b�Z�[�W�\��                       */
/*====================================================*/
static void init_result_message(void)
{
	NMenuContextChange(TNAVI2_TYPE);
	NMenuWinBoxWrite(MSGWIN_X1, MSGWIN_Y1, MSGWIN_X2, MSGWIN_Y2);

	for(work->result_pos = 0; work->result_pos < 5; work->result_pos++)
	{
		if(work->param_up[work->result_pos]){ break; }
	}

	if(work->result_pos < 5){
		make_result_message(work->buffer, work->result_pos, work->param_up[work->result_pos]);
	}else{
		make_result_message(work->buffer, work->result_pos, 0);
	}

	disp_result_message(work->buffer);

}
/*====================================================*/
/* ���̃��b�Z�[�W�\��                                 */
/* return:  ����������� 0,  ����� 1                 */
/*====================================================*/
static u8 next_result_message(void)
{
	while(1)
	{
		work->result_pos++;
		if(work->result_pos < 5){
			if(work->param_up[work->result_pos]){ break; }
		}else{
			work->result_pos = 5;
			return 0;
		}
	}
	make_result_message(work->buffer, work->result_pos, work->param_up[work->result_pos]);
	disp_result_message(work->buffer);
	return 1;
}
/*====================================================*/
/* ����ȏ�H�ׂȂ����b�Z�[�W�\��������               */
/*====================================================*/
static void disp_tabenai_msg(void)
{
	NMenuContextChange(TNAVI2_TYPE);
	NMenuWinBoxWrite(MSGWIN_X1, MSGWIN_Y1, MSGWIN_X2, MSGWIN_Y2);
	NMenuMsgWrite(PolockMsg_Tabenai, MSGWIN_X1+1, MSGWIN_Y1+1);
}
static void erace_tabenai_msg(void)
{
	NMenuScreenClear();
	NMenuContextChange(TNAVI_TYPE);
}

/*====================================================*/
/* ���b�Z�[�W�`��                                     */
/*====================================================*/
static void disp_result_message(const u8 *msg)
{
	NMenuWinBoxWrite(MSGWIN_X1, MSGWIN_Y1, MSGWIN_X2, MSGWIN_Y2);
	NMenuMsgWrite(msg, MSGWIN_X1+1, MSGWIN_Y1+1);
}
/*====================================================*/
/* ���b�Z�[�W������쐬                               */
/*====================================================*/
static const u8 * const MsgTable1[] = {
	PolockMsg_Style,
	PolockMsg_Strong,
	PolockMsg_Clever,
	PolockMsg_Cute,
	PolockMsg_Beautiful,
};

static void make_result_message(u8 *buf, u8 pos, s16 val)
{
	if(val){
		if(val > 0){ val = 0; }
		if(val < 0){ val = 1; }

		PM_strcpy(buf, MsgTable1[pos]);
		PM_strcat(buf, PolockMsg_Up);

	}else{
		PM_strcpy(buf, PolockMsg_Unchange);
	}
}
/***********************************************************************
**  �L���[�u�f�[�^�E�p�����[�^����                                    **
***********************************************************************/
/*====================================================*/
/*  �|�P�����̃R���f�B�V�����f�[�^�l��z��Ɏ擾      */
/*                                                    */
/*  poke   �|�P�����f�[�^                             */
/*  param  �f�[�^�l���󂯎��z��                     */
/*====================================================*/
static void GetPokeCondition(PokemonParam *poke, u8 *param)
{
	u16 i;

	for(i = 0; i < 5; i++)
	{
		param[i] = PokeParaGet(poke, ConditionID[i]);
	}
}
/*====================================================*/
/*  �L���[�u�f�[�^����p�����[�^�����l���v�Z����      */
/*  �|�P�����f�[�^�ɔ��f����                          */
/*                                                    */
/*  cube   �L���[�u�f�[�^                             */
/*  poke   �|�P�����f�[�^                             */
/*                                                    */
/*====================================================*/
static void CubeToPoke(CUBE *cube, PokemonParam *poke)
{
	u8  poke_param, fur;
	s16 param_work;
	u16 i;

	// ���Â�l���ő�Ȃ牽���ς��Ȃ�
	if(PokeParaGet(poke, ID_fur) == 255)
	{
		return;
	}

	calc_add_param(cube, poke);

	// �͈̓I�[�o�[����悤�Ȑ��l�͐؂�l�߂�
	for(i = 0; i < 5; i++)
	{
		poke_param = PokeParaGet(poke, ConditionID[i]);
		param_work = poke_param + work->add_param[i];
		if(param_work < 0)
		{
			param_work = 0;
		}
		if(param_work > 255)
		{
			param_work = 255;
		}
		poke_param = param_work;
		PokeParaPut(poke, ConditionID[i], &poke_param);
	}

	// �тÂ�𑝉�������
	fur = PokeParaGet(poke, ID_fur);
	param_work = fur + cube->umai;
	if(param_work > 255)
	{
		param_work = 255;
	}
	poke_param = param_work;
	PokeParaPut(poke, ID_fur, &poke_param);
}
/*====================================================*/
/*  �L���[�u��^������̃��[�_�[�f�[�^�Ȃǂ��v�Z      */
/*====================================================*/
static void calc_cube_param(void)
{
	PokemonParam *poke;
	u16 i;

	poke = PokeParaMine;
	poke += NaviWork->sort_data[NaviWork->condition_poke_sel].poke;

	GetPokeCondition(poke, work->param_before);
	CubeToPoke(work->cube, poke);
	GetPokeCondition(poke, work->param_after);

	for(i = 0; i < 5; i++)
	{
		work->param_up[i] = work->param_after[i] - work->param_before[i];
	}
}
/*====================================================*/
/*  �L���[�u��^�������̑����l���v�Z                  */
/*                                                    */
/*  cube   �L���[�u�f�[�^                             */
/*  poke   �|�P�����f�[�^                             */
/*                                                    */
/*  �� work->add_param �z��ɂ��ꂼ��̒l������       */
/*                                                    */
/*====================================================*/
static void calc_add_param(CUBE *cube, PokemonParam *poke)
{
	s8  like, taste_like, add, i;

	work->add_param[0] = cube->karai;
	work->add_param[1] = cube->suppai;
	work->add_param[2] = cube->nigai;
	work->add_param[3] = cube->amai;
	work->add_param[4] = cube->sibui;

	if(CubeDemoLike>0){
		like = 1;
	}else if(CubeDemoLike < 0){
		like = -1;
	}else{
		return;  // �D���ł������ł��Ȃ��ꍇ�͉������Ȃ�
	}

	// �D����������v���閡�́A�l�𑝌�������
	for(i = 0; i < 5; i++)
	{
		// �l�̌ܓ����Ă���̂P�����v�Z
		add = work->add_param[i] / 10;
		if((work->add_param[i] % 10) > 4){
			add++;
		}

		taste_like = PokeLikeTasteCheck(poke, TasteID[i]);
		if(taste_like == like)
		{
			work->add_param[i] += (like * add);
		}
	}
}
/*====================================================*/
/*  �I�������|�P�����̖тÂ⃌�x�����ő傩�H          */
/*====================================================*/
static u8 is_fur_max(void)
{
	PokemonParam *poke;
	poke = PokeParaMine;
	poke += NaviWork->sort_data[NaviWork->condition_poke_sel].poke;

	// ���Â�l���ő�Ȃ牽���ς��Ȃ�
	if(PokeParaGet(poke, ID_fur) == 255)
	{
		return 1;
	}

	return 0;
}
/*====================================================*/
/*  �I��ԍ������Ă����ԍ����ݕϊ�                    */
/*�i�I��ԍ��̓^�}�S���X�L�b�v���Ă��邽�߁j          */
/*====================================================*/
static u8 selno_to_pokeno(u8 selno)
{
	u8 i;

	for(i = 0; i < STOCK_POKE_MAX; i++){
		if(PokeParaGet(PokeParaMine+i, ID_tamago_flag)==0){
			if(selno==0){
				return i;
			}else{
				selno--;
			}
		}
	}
	return 0;  // ���蓾�Ȃ�����
}
static u8 pokeno_to_selno(u8 pokeno)
{
	u8 i, cnt;

	for(i = 0, cnt = 0; i < pokeno; i++){
		if(PokeParaGet(PokeParaMine+i, ID_tamago_flag)){
			cnt++;
		}
	}
	return pokeno - cnt;
}

u8 CubedemoSelnoToPokeno(u8 selno)
{
	return selno_to_pokeno(selno);
}

/***********************************************************************
**  �A�N�^�[�\��                                                      **
***********************************************************************/
#define UPDOWN_X1  (156)  // �������悳
#define UPDOWN_Y1  (36 - DATAAREA_UP)
#define UPDOWN_X2  (117)  // ��������
#define UPDOWN_Y2  (59 - DATAAREA_UP)
#define UPDOWN_X3  (117)  // ���킢��
#define UPDOWN_Y3  (118 - DATAAREA_UP)
#define UPDOWN_X4  (197)  // ��������
#define UPDOWN_Y4  (118 - DATAAREA_UP)
#define UPDOWN_X5  (197)  // �����܂���
#define UPDOWN_Y5  (59 - DATAAREA_UP)

static const actHeader UpDownHeader;
static void actmove_up(actWork *act);

static void add_updown_actor(void)
{
	static const CellData cell = { updown_Character, sizeof(updown_Character), CELLID_UPDOWN };
	static const PalData  pal = { updown_Palette, PALID_UPDOWN };
	static const u16 pos[5][2] = {
		{ UPDOWN_X1, UPDOWN_Y1 },
		{ UPDOWN_X2, UPDOWN_Y2 },
		{ UPDOWN_X3, UPDOWN_Y3 },
		{ UPDOWN_X4, UPDOWN_Y4 },
		{ UPDOWN_X5, UPDOWN_Y5 },
	};
	u16 i, actno;

	CellSet(&cell);
	ObjPalSet(&pal);
	work->updown_act_cnt = 0;

	for(i = 0; i < 5; i++)
	{
		if(work->param_up[i])
		{
			actno = AddActor(&UpDownHeader, pos[i][0], pos[i][1], 0);
			if(actno != ACT_MAX)
			{
				if(work->param_up[i] > 0){
					ActWork[actno].move = actmove_up;

#if 0   // �����邱�Ƃ͖����Ȃ���
				}else{
					ActAnmChg(&ActWork[actno], 1);
					ActWork[actno].move = actmove_down;
#endif

				}
				work->updown_act_cnt++;
			}
		}
	}
}

/*============================================*/
/*  ����֐�                                  */
/*============================================*/
// ���ǂ����60sync���炢�\�����Ă���
static void actmove_up(actWork *act)
{
	if(act->work[0] < 6)
	{
		act->dy -= 2;
	}else if(act->work[0] < 12){
		act->dy += 2;
	}

	if(++(act->work[0]) > 60)
	{
		DelActor(act);
		work->updown_act_cnt--;
	}
}
/*============================================*/
/*  �A�N�^�[�f�[�^                            */
/*============================================*/
static const ActOamData UpDownOam = {
	0,		//u32 VPos:8;             // �x���W
	0,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	1,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;    // �A�t�B���ϊ��p�����[�^No & H,VFlip
	2,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	1,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
static const actAnm UpDownAnm[]={
	{0,5,0,0}, {ACT_ANMEND,0,0,0},
	{8,5,0,0}, {ACT_ANMEND,0,0,0},
};
static const actAnm *const UpDownAnmTbl[]={
	UpDownAnm + 0,
	UpDownAnm + 2,
};
static const actHeader UpDownHeader = {
	CELLID_UPDOWN,
	PALID_UPDOWN,
	&UpDownOam,
	UpDownAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	DummyActMove,
};

