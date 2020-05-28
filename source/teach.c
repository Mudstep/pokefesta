//======================================================
//
//    teach.c
//
//    �������}�j�A                                  
//                                                      
//    Copyright (C) 2001 GAME FREAK inc.
//======================================================

//�������O�́u�|�v
//�_���[�W�P���O�́u�|�v

#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "madefine.h"
#include "palanm.h"
#include "task.h"
#include "agbmons.h"
#include "calctool.h"
#include "mus_tool.h"
#include "print.h"
#include "pm_str.h"
#include "script.h"
#include "wazatool.h"
#include "gamedata.h"
#include "config.h"
#include "ev_fight.h"
#include "..\script\saveflag.h"
#include "fld_main.h"
#include "fld_task.h"
#include "contest.h"
#include "con_tool.h"

#include "server.h"
#include "pokelist.h"
#include "f_pmlist.h"
#include "poketool.h"
#include "zukan.h"
#include "p_status.h"

#include "..\scaddata\teachcur.acl"
#include "..\scaddata\teachcur.ach"



// =================================================================
// extern
// =================================================================
extern const	u8	WazaNameData[][8];
extern const 	u8	ZokuseiMsg[][5];
extern const	u8	WazaInfo[][56];
extern const	WazaTblDat	WTD[];
extern const    u8  str_bou3[];
extern const    u8  dummy_string[];

// =================================================================
// �֐��v���g�^�C�v
// =================================================================
void OshieManiaInit();
static void FightListPut(u8 flg);
static void ContestListPut(u8 flg);
static void FightListMain();
static void ContestListMain();
static void SeqMain();
void OshieManiaMain();
static void teachwork_init();
static void TeachActorSet();
static void WazaListPut();
static void WazaListControl();
static void GetWazaParameter();
static void WazaInfoWrite(u8 info);
static void TeachWindowPut();
static void InitFadeWait( u8 id );


// =================================================================
// ��`
// =================================================================
#define WAZANAME_MAX	8			//���O�̍ő�
#define WAZALIST_MAX	25			//���X�g���ڂ̍ő�
#define WAZALIST_APPEAR	6			//��x�ɕ\���ł���ő�

static const u8 commonwindow_tbl[][4]={		//�E�C���h�E�`��ʒu
	{ 0, 0,19,13},
	{20, 0,29,13},
	{ 2,14,27,19},
};

static const u8 explain1_mes[]={ta_,ta_,ka_,u_,wa_,za_,EOM_,};
static const u8 explain2_mes[]={KO_,N_,TE_,SU_,TO_,wa_,za_,EOM_,};
static const u8 type_mes[]={TA_,I_,PU_,sura_,EOM_,};
static const u8 pp_mes[]={P__,P__,sura_,EOM_,};
static const u8 iryoku_mes[]={i_,ri_,yyo_,ku_,sura_,EOM_,};
static const u8 hit_mes[]={me_,i_,ti_,yyu_,u_,sura_,EOM_,};
static const u8 apear_mes[]={A_,PI_,bou_,RU_,EOM_,};
static const u8 bougai_mes[]={bo_,u_,ga_,i_,EOM_,};

typedef struct{
	const u8 	*mes;
	u8 			x,y;
	u16			flag;
	
}TEACH_ITEM;

static const TEACH_ITEM hyouji_item[2][5]={		//���b�Z�[�W�\���p���X�g
	{
		{explain1_mes,	7,1,0},
		{type_mes,		 1, 4, 1},
		{iryoku_mes,	11, 4, 2},
		{pp_mes,	     2, 6, 3},
		{hit_mes,		10, 6, 4},
	},
	{
		{explain2_mes,	 7, 1, 0},
		{type_mes,		 1, 4, 5},
//		{pp_mes,		11, 4, 6},
		{apear_mes,		 1, 6, 7},
		{bougai_mes,	11, 6, 8},
	},
};

static const u8 teach1_mes[]={I_MSG_,STR_TEMP_BUF0,ni_,spc_,do_,no_,wa_,za_,wo_,spc_,o_,bo_,e_,sa_,se_,ru_,hate_,EOM_,};
static const u8 teach2_mes[]={
	I_MSG_,STR_TEMP_BUF1,wo_,spc_,o_,bo_,e_,sa_,se_,ma_,su_,ka_,hate_,EOM_,
};

static const u8 teach3_mes[]={
	I_MSG_,STR_TEMP_BUF0,ha_,spc_,a_,ta_,ra_,si_,ku_,CR_,
	I_MSG_,STR_TEMP_BUF1,wo_,spc_,o_,bo_,e_,ta_,gyoe_,EOM_,
};


static const u8 teach4_mes[]={
	I_MSG_,STR_TEMP_BUF0,ha_,spc_,a_,ta_,ra_,si_,ku_,spc_,CR_,
	I_MSG_,STR_TEMP_BUF1,wo_,spc_,o_,bo_,e_,ta_,i_,tenten_,gyoe_,NORMAL_WAIT_,
	si_,ka_,si_,spc_,I_MSG_,STR_TEMP_BUF0,ha_,spc_,wa_,za_,wo_,spc_,n4_,tu_,CR_,
	o_,bo_,e_,te_,i_,ru_,no_,de_,spc_,se_,i_,i_,ttu_,pa_,i_,da_,gyoe_,NORMAL_WAIT_,
	I_MSG_,STR_TEMP_BUF1,no_,spc_,ka_,wa_,ri_,ni_,spc_,ho_,ka_,no_,spc_,wa_,za_,wo_,CR_,wa_,su_,re_,sa_,se_,ma_,su_,ka_,hate_,EOM_,
};

static const u8 teach5_mes[]={
	so_,re_,de_,ha_,tenten_,tenten_,spc_,I_MSG_,STR_TEMP_BUF1,wo_,CR_,
	o_,bo_,e_,ru_,no_,wo_,spc_,a_,ki_,ra_,me_,ma_,su_,ka_,hate_,EOM_,
};

static const u8 teach6_mes[]={
	CTRL_,C_WAIT_,0x20,
	n1_,spc_,CTRL_,C_WAIT_,0xf,
	n2_,no_,spc_,CTRL_,C_WAIT_,0xf,
	tenten_,CTRL_,C_WAIT_,0xf,
	tenten_,CTRL_,C_WAIT_,0xf,
	tenten_,spc_,CTRL_,C_WAIT_,0xf,
//	CTRL_,C_MUSPLAY_,(SE_KON&0xff),((SE_KON&0xff00)>>8),PO_,KA_,N_,gyoe_,NORMAL_WAIT_,
//	C_MUSPLAY��MusicPlay���R�[�����Ă���̂ŁASE��SePlay�Ŗ炵�����̂ŁA
//	C_SEPLAY_�Ƃ����R�}���h��ǉ�
//	changed by soga 2002.09.29
	CTRL_,C_SEPLAY_,(SE_KON&0xff),((SE_KON&0xff00)>>8),PO_,KA_,N_,gyoe_,NORMAL_WAIT_,
	EOM_,
};
static const u8 teach6_1_mes[]={
	I_MSG_,STR_TEMP_BUF0,ha_,spc_,I_MSG_,STR_TEMP_BUF2,no_,spc_,tu_,ka_,i_,ka_,ta_,wo_,CR_,ki_,re_,i_,ni_,spc_,wa_,su_,re_,ta_,gyoe_,NORMAL_WAIT_,
	so_,si_,te_,tenten_,gyoe_,NORMAL_WAIT_,
	I_MSG_,STR_TEMP_BUF0,ha_,spc_,a_,ta_,ra_,si_,ku_,CR_,I_MSG_,STR_TEMP_BUF1,wo_,spc_,o_,bo_,e_,ta_,gyoe_,EOM_,
};

static const u8 teach7_mes[]={
	I_MSG_,STR_TEMP_BUF0,ha_,spc_,I_MSG_,STR_TEMP_BUF1,wo_,spc_,o_,bo_,e_,zu_,ni_,spc_,o_,wa_,ttu_,ta_,gyoe_,EOM_,
};

static const u8 teach8_mes[]={
	I_MSG_,STR_TEMP_BUF0,ni_,spc_,wa_,za_,wo_,spc_,o_,bo_,e_,sa_,se_,ru_,no_,wo_,CR_,a_,ki_,ra_,me_,ma_,su_,ka_,hate_,EOM_,
};
static const u8 teach9_mes[]={
	do_,no_,spc_,wa_,za_,wo_,CR_,wa_,su_,re_,sa_,se_,ta_,i_,hate_,NORMAL_WAIT_,EOM_,
};

static const u8 modoru_mes[]={	mo_,do_,ru_,EOM_};
static const u8 arrow_mes[][2]={	{cursor_,EOM_,},	{spc_, EOM_}};

// =================================================================
// �\����
// =================================================================
typedef struct{				//�I�����X�g�p�\����
	u8 pos;				//�\���ʒu
	u8 top;				//���X�g�\���擪
	u8 max;				//���X�g���ڐ�MAX
	u8 select;			//���ݑI���A�C�e��
	u8 old;				//�ߋ��\���ꏊ
	u8 rewrite_cursor;	//�J�[�\���ĕ`��t���O
	u8 rewrite;			//���X�g�ĕ`��t���O
}LIST;

typedef struct{
	u8  	seq;		//���C���V�[�P���X
	u8  	end;		
	u8		yesno;		
	u8		actno[4+16];	//�A�N�^�[�ԍ��ۑ�(enum�ł�����)
	LIST	list;		//���X�g�\����
	u16		wazalist[WAZALIST_MAX];					//�|�P�������o������Z�ԍ�
	u8      wazaname[WAZALIST_MAX][WAZANAME_MAX];	//�|�P�������o������Z�̖��O
	u8		inforewrite;							//������ʍĕ`��t���O
	u8		info;									//�R���e�X�g�������퓬������
	u8		pokeno;									//�Ă����߹�݂̉��Ԗڂ�
	u8		forget;									//�Y��邱�Ƃɂ������U�̔ԍ�
	u8		returnnum;						
}TEACHMANIA_WORK;

static TEACHMANIA_WORK *tmw;		//���[�N��`



// =================================================================
// �A�N�^�[��`
// =================================================================
static void teacharrowMove(actWork *Xreg);


//�A�N�^�[�p�f�[�^
static const ActOamData TeachCursorOamData[] = {
	{0,0,0,OFF,0,0,0,0,0,0,0,0,0,},	//8x16
	{0,0,0,OFF,0,2,0,0,0,0,0,0,0,},	//8x16
	{0,0,0,OFF,0,1,0,0,0,0,0,0,0,},	//16x8
};

static const actAnm cursorAnm0[]={		//���ړ��p�J�[�\����
		{OBJ16_8x16*1,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm1[]={		//���ړ��p�J�[�\���E
		{OBJ16_8x16*0,5,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm *const CursorAnmTbl[]={
		cursorAnm0,
		cursorAnm1,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const CellData CursorActCell[]={
	{teachcur_Character,(0x20*2)*4+0x20*4,5525},
};

static const PalData CursorActPal = 
{
	teachcur_Palette,
	5526,
};

//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------

static const	actHeader CursorHeader={		//���ړ����
		5525,
		5526,
		&TeachCursorOamData[1],
		CursorAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		teacharrowMove,
};

//-------------------------------------------------------------
//		�A�j����`
//-------------------------------------------------------------

static const actAnm cursorAnm4[]={		//�c�ړ���󉺌���
		{OBJ16_16x8*2,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm5[]={		//�c�ړ��������
		{OBJ16_16x8*3,5,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm *const CursorAnmTbl2[]={
	cursorAnm4,
	cursorAnm5,
};

//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------

static const	actHeader CursorHeader2={		//�c�ړ����
		5525,
		5526,
		&TeachCursorOamData[2],
		CursorAnmTbl2,
		ACT_NTRANS,
		DummyActAffTbl,
		teacharrowMove,
};

//-------------------------------------------------------------
//		�A�j����`
//-------------------------------------------------------------

static const actAnm HeartAnm0[]={		//�Ԃ����g�̖����n�[�g
		{OBJ16_8x8*8,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm HeartAnm1[]={		//�Ԃ��n�[�g
		{OBJ16_8x8*9,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm HeartAnm2[]={		//�������g�̖����n�[�g
		{OBJ16_8x8*10,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm HeartAnm3[]={		//�����n�[�g
		{OBJ16_8x8*11,5,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm *const HeartAnmTbl[]={
	HeartAnm0,
	HeartAnm1,
	HeartAnm2,
	HeartAnm3,
};

//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------

static const	actHeader HeartHeader={		//�n�[�g
		5525,
		5526,
		&TeachCursorOamData[0],
		HeartAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		teacharrowMove,
};

// ============================================================
// �A�N�^�[�ԍ�
// ============================================================
enum{
	LEFTARROW_NO = 0,
	RIGHTARROW_NO,
	UPARROW_NO,
	DOWNARROW_NO,
	HEART1_NO,		//��
	HEART2_NO,		//��
	HEART3_NO,		//��
	HEART4_NO,		//��
	HEART5_NO,		//��
	HEART6_NO,		//��
	HEART7_NO,		//��
	HEART8_NO,		//��
	BLACKHEART1_NO, 
	BLACKHEART2_NO, 
	BLACKHEART3_NO, 
	BLACKHEART4_NO, 
	BLACKHEART5_NO, 
	BLACKHEART6_NO, 
	BLACKHEART7_NO, 
	BLACKHEART8_NO, 
};



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


// =================================================================
// WazaWasureCall
// �T�v  : ���U�������}�j�A���O���X�N���v�g����̌Ăяo��
// ����  : none
// �߂�l: none
// =================================================================
void WazaOshieManiaCall(void)
{
	u8	id;

	SetForceEvent();
	id = AddTask( InitFadeWait, 10 );
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}



static void InitFadeWait( u8 id )
{
	if( !(FadeData.fade_sw) ){
		MainProcChange(OshieManiaInit);
		pFieldRecoverFunc = AddScriptFieldInTask;
//		FadeData.trans_stop = 1;
		DelTask(id);
	}
}




static const u8 testname []={a_,ki_,to_,EOM_};
// =================================================================
// OshieManiaInit
// �T�v  : �Z�������}�j�A��ʏ�����
// ����  : none
// �߂�l: none
// =================================================================
void OshieManiaInit()
{
	*(vu16 *)REG_DISPCNT = 	0;	// BG���[�h 

	InitActor();				//�A�N�^�[������
	ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
	InitTask();

	tmw = (TEACHMANIA_WORK*)&UserWork[0x17000];

	teachwork_init();		//���[�N������
	tmw->pokeno = ScriptParameter0;			//�X�N���v�g�Ăяo�����̃p�����[�^���擾

	GetWazaParameter();		//�X�N���v�g����̌Ăяo���ꂽ���̃p�����[�^���擾
	

	SetVBlankFunc(VInterFunc);


	PrintScreenInit(FIELD_MENU_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);
	NMenuScreenClear();
	


	*(vu16 *)REG_BG0VOFS = 0;
	*(vu16 *)REG_BG0HOFS = 0;


	CellSet((CellData *)&CursorActCell);	//�Z���Z�b�g
	ObjPalSet((PalData *)&CursorActPal);	//�p���b�g�Z�b�g
	
	TeachActorSet();

//	AddTask(CheckParent,0);

	PaletteWorkClear(0x0000, 0, 2);
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
	MainProcChange(OshieManiaMain);

	
}

// =================================================================
// OshieManiaReturnInit
// �T�v  : �߂��Ă������p
// ����  : none
// �߂�l: none
// =================================================================
void OshieManiaReturnInit()
{
	InitActor();				//�A�N�^�[������
	ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
	InitTask();

	tmw = (TEACHMANIA_WORK*)&UserWork[0x17000];

	GetWazaParameter();			//
	
	tmw->forget = ScriptParameter1;	//���U�킷�ꃊ�X�g�̌��ʂ��擾

	SetVBlankFunc(VInterFunc);


	PrintScreenInit(FIELD_MENU_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);
	NMenuScreenClear();
	

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_OBJ_ON					// OBJ�L��
						;

	*(vu16 *)REG_BG0VOFS = 0;
	*(vu16 *)REG_BG0HOFS = 0;


	CellSet((CellData *)&CursorActCell);	//�Z���Z�b�g
	ObjPalSet((PalData *)&CursorActPal);	//�p���b�g�Z�b�g
	
	TeachActorSet();

	PaletteWorkClear(0x0000, 0, 2);

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
	MainProcChange(OshieManiaMain);

}

// =================================================================
// OshieManiaMain
// �T�v  : �Z�������}�j�A��ʃ��C��
// ����  : none
// �߂�l: none
// =================================================================
void OshieManiaMain()
{
	SeqMain();
	if(tmw->list.rewrite_cursor){
		tmw->list.rewrite_cursor = 0;
		NMenuMsgWrite(arrow_mes[1],21,tmw->list.old*2+1);
		NMenuMsgWrite(arrow_mes[0],21,tmw->list.pos*2+1);
	}
	if(tmw->list.rewrite){
		tmw->list.rewrite = 0;
		WazaListPut();
	}
	if(tmw->inforewrite){
		WazaInfoWrite(tmw->info);
		tmw->inforewrite = 0;
	}


	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
}


enum{
	TEACHMAIN_INIT,
	TEACHMAIN_FADEIN,
	TEACHMAIN_MESWAIT,

	TEACHMAIN_FIGHT_SET,	
	TEACHMAIN_FIGHT,	
	TEACHMAIN_CONTEST_SET,	
	TEACHMAIN_CONTEST,	
	TEACHMAIN_MAIN,
	TEACHMAIN_YESNO_MES,
	TEACHMAIN_YESNO,
	TEACHMAIN_REWRITE,
	TEACHMAIN_MES,
//	TEACHMAIN_MESWAIT,
	TEACHMAIN_END_MES,
	TEACHMAIN_END_YESNO,
	TEACHMAIN_FADEOUT,
	TEACHMAIN_FADEOUTWAIT,


	TEACHMAIN_FULL_MES_SET,
	TEACHMAIN_FULL_MES,
	TEACHMAIN_FULL_YESNO,
	WAZAWASURE_MES_WAIT,
	WAZAWASURE_FADEOUT,
	TEACHMAIN_SUCCESS_MES,
	WAZAWASURE_FADEIN,
	WAZAWASURE_FADEIN_WAIT,


	WAZAAKIRAME_MES_SET,
	WAZAAKIRAME_MES,
	WAZAAKIRAME_YESNO,							//���U���o����̂�������߂܂����H
	TEACHMAIN_BACK,

	TEACHRETURN_INIT,
	TEACHRETURN_WAIT,
	TEACHRETURN_WASURE_MES,
	TEACHRETURN_OBOE_MES,
	TEACHRETURN_JINGLE_WAIT,
	TEACHRETURN_ENDBUTTON_WAIT,

};


// =================================================================
// PutSystemMes
// �T�v  : ���i�̃V�X�e�����b�Z�[�W��Talk�ŕ\������
// ����  : mes
// �߂�l: none
// =================================================================
static void PutSystemMes(const u8 mes[])
{
	PM_MsgExpand(MsgExpandBuffer,mes);	
	NMenuTalkMsgSet(MsgExpandBuffer,3,15);
}

// =================================================================
// SeqMain
// �T�v  : �V�[�P���X����
// ����  : none
// �߂�l: none
// =================================================================
static void SeqMain()
{
	s8  no;
	u16 waza;
	switch(tmw->seq){
		case TEACHMAIN_INIT:			//������
			tmw->seq++;
			TeachWindowPut();
			FightListPut(0);
			WazaListPut();
			ActWork[RIGHTARROW_NO].x = 112;
			tmw->inforewrite = 1;
			PaletteFadeReq(0xffffffff,0,16,0,0);
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_OBJ_ON					// OBJ�L��
						;
			break;
		case TEACHMAIN_FADEIN:			//�t�F�[�h�C��
			if(FadeData.fade_sw == 0){
				tmw->seq = TEACHMAIN_FIGHT;
			}
			break;
		case TEACHMAIN_MESWAIT:
//			if(NMenuTalkMsgPut()) 
				tmw->seq++;
			break;
		case TEACHMAIN_FIGHT_SET:		//�퓬�p���U�����\��
			FightListPut(0);
			WazaListPut();
			tmw->inforewrite = 1;
			tmw->seq++;
			ActWork[RIGHTARROW_NO].x = 112;
			break;
		case TEACHMAIN_FIGHT:			//�퓬��ʂőI��
			FightListMain();
			WazaListControl();
			break;
		case TEACHMAIN_CONTEST_SET:		//�R���e�X�g�p���U�����\��
			ContestListPut(0);
			WazaListPut();
			tmw->inforewrite = 1;
			ActWork[RIGHTARROW_NO].x = 112+8;
			tmw->seq++;
			break;
		case TEACHMAIN_CONTEST:			//�R���e�X�g��ʂőI��
			ContestListMain();
			WazaListControl();
			break;
		case TEACHMAIN_YESNO_MES:		//�킴�����ڂ��܂����H�\��
			if(NMenuTalkMsgPut()){
				YesNoSelectInit(21,7);
				tmw->seq++;
			}
			break;
		case TEACHMAIN_YESNO:			//�킴���o���邩�I��
				no = YesNoSelectMain();
				//���U���o����
				if(no==CURSOR_YES_VALUE){
					if(PokeWazaSet(&PokeParaMine[tmw->pokeno],tmw->wazalist[tmw->list.select])!=NO_WAZA_SET){
						PutSystemMes(teach3_mes);	//�o�����B
						ScriptParameter0 = 1;	
						tmw->seq = TEACHRETURN_OBOE_MES;
					}else{							//�����ς��Ŋo�����Ȃ�����
						WazaListPut();
						tmw->seq = TEACHMAIN_FULL_MES_SET;
					}
				}else if (no==CURSOR_CANCEL_VALUE || no==CURSOR_NO_VALUE){
					if(tmw->info==0) tmw->seq = TEACHMAIN_FIGHT_SET;
					if(tmw->info==1) tmw->seq = TEACHMAIN_CONTEST_SET;
				}
				break;

		case TEACHMAIN_END_MES:				//��߂܂����H�\��
			if(NMenuTalkMsgPut()){
				YesNoSelectInit(21,7);	
				tmw->seq++;
			}
			break;
		case TEACHMAIN_END_YESNO:			//��߂܂����H�I��
			no = YesNoSelectMain();
			if(no==CURSOR_YES_VALUE){
				ScriptParameter0 = 0;			//���U��������߂��B
				tmw->seq = TEACHMAIN_FADEOUT;
			}else if (no==CURSOR_CANCEL_VALUE || no==CURSOR_NO_VALUE){
				if(tmw->info==0) tmw->seq = TEACHMAIN_FIGHT_SET;
				if(tmw->info==1) tmw->seq = TEACHMAIN_CONTEST_SET;
			}
			break;

		case TEACHMAIN_FULL_MES_SET:
			PutSystemMes(teach4_mes);					//���U�������ς��Ŋo�����Ȃ�
			tmw->seq++;
			break;

		case TEACHMAIN_FULL_MES:
			if(NMenuTalkMsgPut()){
				YesNoSelectInit(21,7);	
				tmw->seq = TEACHMAIN_FULL_YESNO;
			}
			break;
		case TEACHMAIN_FULL_YESNO:					//���U�������ς��ł��A�Y��܂����H
			no = YesNoSelectMain();
			if(no==CURSOR_YES_VALUE){									//�͂�
				PutSystemMes(teach9_mes);
				tmw->seq = WAZAWASURE_MES_WAIT;
			}else if (no==CURSOR_CANCEL_VALUE || no==CURSOR_NO_VALUE){	//������
				WazaListPut();
				tmw->seq = WAZAAKIRAME_MES_SET;	
			}
			break;
		
		case WAZAAKIRAME_MES_SET:
			PutSystemMes(teach5_mes);	
			tmw->seq++;
			break;
		case WAZAAKIRAME_MES:
			if(NMenuTalkMsgPut()){
				tmw->seq = WAZAAKIRAME_YESNO;
				YesNoSelectInit(21,7);	
			}
			break;
		case WAZAAKIRAME_YESNO:							//���U���o����̂�������߂܂����H
			no = YesNoSelectMain();
			if(no==CURSOR_YES_VALUE){									//�͂�
				tmw->seq = TEACHMAIN_BACK;
			}else if (no==CURSOR_CANCEL_VALUE || no==CURSOR_NO_VALUE){	//������
				WazaListPut();
				if(tmw->info==0) tmw->seq = TEACHMAIN_FIGHT_SET;
				if(tmw->info==1) tmw->seq = TEACHMAIN_CONTEST_SET;
				tmw->seq = TEACHMAIN_FULL_MES_SET;
			}
			break;
		case TEACHMAIN_BACK:
			if(NMenuTalkMsgPut()){
				if(tmw->info==0) tmw->seq = TEACHMAIN_FIGHT_SET;
				if(tmw->info==1) tmw->seq = TEACHMAIN_CONTEST_SET;
			}
			break;
		
		
		case WAZAWASURE_MES_WAIT:
			if(NMenuTalkMsgPut()){
				tmw->seq = WAZAWASURE_FADEOUT;
				PaletteFadeReq(0xffffffff,0,0,16,0);
			}
			break;
		case WAZAWASURE_FADEOUT:
			if(FadeData.fade_sw == 0){
				PokeNewWazaSet(PokeParaMine,tmw->pokeno,
					PokeCountMine-1,OshieManiaReturnInit,tmw->wazalist[tmw->list.select]);
				tmw->seq = TEACHRETURN_INIT;
			}
			break;
		case TEACHMAIN_SUCCESS_MES:
			if(NMenuTalkMsgPut()){
				tmw->seq = TEACHMAIN_FADEOUT;
			}
			break;
		case WAZAWASURE_FADEIN:
			PaletteFadeReq(0xffffffff,0,16,0,0);
			break;
		case WAZAWASURE_FADEIN_WAIT:
			break;

		case TEACHMAIN_FADEOUT:
			PaletteFadeReq(0xffffffff,0,0,16,0);
			tmw->seq++;
			break;
		case TEACHMAIN_FADEOUTWAIT:
			if(FadeData.fade_sw == 0){
				MainProcChange(FieldMainRecover);
			}
			break;
		case TEACHRETURN_INIT:
			PaletteFadeReq(0xffffffff,0,16,0,0);
			tmw->seq++;
			TeachWindowPut();
			WazaListPut();
			if(tmw->info==0) FightListPut(1);
			if(tmw->info==1){
				ActWork[RIGHTARROW_NO].x = 112+8;
				ContestListPut(1);
			}
			WazaInfoWrite(tmw->info);
			break;
		case TEACHRETURN_WAIT:
			if(FadeData.fade_sw == 0){
				if(tmw->forget==4){
					tmw->seq = WAZAAKIRAME_MES_SET;
				}else{
					//�w��̏ꏊ�Ƀ��U������
					waza = PokeParaGet(&PokeParaMine[tmw->pokeno],ID_waza1+tmw->forget);
					PM_strcpy(StrTempBuffer2,WazaNameData[waza]);

					PokePPCountClear(&PokeParaMine[tmw->pokeno],tmw->forget);		//�o�o���N���A
					PokeWazaSetPos(&PokeParaMine[tmw->pokeno],tmw->wazalist[tmw->list.select],tmw->forget);
					PM_strcpy(StrTempBuffer1,WazaNameData[tmw->wazalist[tmw->list.select]]);
					PutSystemMes(teach6_mes);					//�P�C�Q�̃|�J���I�c
					tmw->seq = TEACHRETURN_WASURE_MES;
					ScriptParameter0 = 1;	
				}
			}
			break;
		case TEACHRETURN_WASURE_MES:
			if(NMenuTalkMsgPut()){
//				SePlay(SE_KON);
				PutSystemMes(teach6_1_mes);
				tmw->seq = TEACHRETURN_OBOE_MES;
				JinglePlay(MUS_FANFA1);
			}
			break;
		case TEACHRETURN_OBOE_MES:
			if(NMenuTalkMsgPut()){
				JinglePlay(MUS_FANFA1);
				tmw->seq = TEACHRETURN_JINGLE_WAIT;
			}
			break;
		case TEACHRETURN_JINGLE_WAIT:
			if(JingleWait()){
				tmw->seq = TEACHRETURN_ENDBUTTON_WAIT;
			}
			break;
		case TEACHRETURN_ENDBUTTON_WAIT:
			if(sys.Trg&A_BUTTON){
				SePlay(SE_SELECT);
				tmw->seq = TEACHMAIN_FADEOUT;
			}
			break;
	}
}

// =================================================================
// TeachWindowPut
// �T�v  : �킴�������}�j�A��ʗp�̃E�C���h�E��\��
// ����  : none
// �߂�l: none
// =================================================================
static void TeachWindowPut()
{
	int i;
	for(i=0;i<3;i++)
		NMenuWinBoxWrite(commonwindow_tbl[i][0], commonwindow_tbl[i][1], 
						commonwindow_tbl[i][2],commonwindow_tbl[i][3]);
}
// =================================================================
// FightListPut
// �T�v  : �퓬�p�Z���X�g�\��
// ����  : none
// �߂�l: none
// =================================================================
static void FightListPut(u8 flg)
{
	int i;


	for(i=0;i<1;i++)
		NMenuWinBoxWrite(commonwindow_tbl[i][0], commonwindow_tbl[i][1], 
						commonwindow_tbl[i][2],commonwindow_tbl[i][3]);
	ActWork[tmw->actno[LEFTARROW_NO]].banish = 0;
	ActWork[tmw->actno[RIGHTARROW_NO]].banish = 0;
	for(i=0;i<16;i++)
		ActWork[tmw->actno[HEART1_NO+i]].banish = 1;

	for(i=0;i<5;i++){
		NMenuMsgWrite(hyouji_item[0][i].mes, hyouji_item[0][i].x, hyouji_item[0][i].y);
	}
	NMenuBoxWrite(1,9,18,9+3);
	NMenuBoxWrite(3,15,26,18);
	if(flg==0){
		PM_MsgExpand(MsgExpandBuffer,teach1_mes);
		NMenuMsgWrite(MsgExpandBuffer,3,15);
	}

}



// =================================================================
// FightListMain
// �T�v  : ���������킴�̃��X�g��ʂ̃L�[�R���g���[��
// ����  : none
// �߂�l: none
// =================================================================
static void FightListMain()
{
	u8 flag;
	flag = 0;
	if(sys.Trg&L_KEY || sys.Trg&R_KEY)
		flag++;
	
	if(MyData.key_config == CNT_MD_LR)
		if(sys.Trg&R_BUTTON || sys.Trg&L_BUTTON)
			flag++;

	if(flag){
		SePlay(SE_SELECT);
		tmw->seq = TEACHMAIN_CONTEST_SET;
		tmw->info = 1;
	}
}
// =================================================================
// ContestListPut
// �T�v  : �R���e�X�g�p�Z���X�g�\��
// ����  : none
// �߂�l: none
// =================================================================
static void ContestListPut(u8 flg)
{
	int i;

	for(i=0;i<1;i++)
		NMenuWinBoxWrite(commonwindow_tbl[i][0], commonwindow_tbl[i][1], 
						commonwindow_tbl[i][2],commonwindow_tbl[i][3]);

	ActWork[tmw->actno[LEFTARROW_NO]].banish  = 0;
	ActWork[tmw->actno[RIGHTARROW_NO]].banish = 0;
	for(i=0;i<16;i++)
		ActWork[tmw->actno[HEART1_NO+i]].banish = 0;

	for(i=0;i<4;i++){
		NMenuMsgWrite(hyouji_item[1][i].mes, hyouji_item[1][i].x, hyouji_item[1][i].y);
	}
	NMenuBoxWrite(1,9,18,9+3);
	NMenuBoxWrite(3,15,26,18);
	if(flg==0){
		PM_MsgExpand(MsgExpandBuffer,teach1_mes);
		NMenuMsgWrite(MsgExpandBuffer,3,15);
	}
}


// =================================================================
// ContestListMain
// �T�v  : �R���e�X�g�Z�̎��̃L�[�R���g���[��
// ����  : none
// �߂�l: none
// =================================================================
static void ContestListMain()
{
	u8 flag;
	flag = 0;
	if(sys.Trg&L_KEY||sys.Trg&R_KEY)
		flag++;

	if(MyData.key_config == CNT_MD_LR)
		if(sys.Trg&R_BUTTON || sys.Trg&L_BUTTON)
			flag++;

	if(flag){
		SePlay(SE_SELECT);
		tmw->seq = TEACHMAIN_FIGHT_SET;
		tmw->info = 0;
	}

}

// =================================================================
// teachwork_init
// �T�v  : ���[�N������
// ����  : none
// �߂�l: none
// =================================================================
static void teachwork_init()
{
	int i;
	
	tmw->seq   = 0;
	tmw->yesno = 0;

	tmw->list.top = 0;
	tmw->list.pos = 0;
	tmw->list.old = 0;
	tmw->list.max = 0;
	tmw->list.select = 0;
	tmw->list.rewrite_cursor = 0;
	tmw->list.rewrite = 0;
	tmw->inforewrite = 0;
	tmw->info = 0;
	for(i=0;i<20;i++) tmw->wazalist[i] = 0;

}

// =================================================================
// teacharrowMove
// �T�v  : �A�N�^�[�V�[�P���X
// ����  : Xreg
// �߂�l: none
// =================================================================
static void teacharrowMove(actWork *Xreg)
{
	u16 work;
	work = (Xreg->work[1]*10)&0xff;
	switch(Xreg->work[0]){
		case 0:			//��������
			break;
		case 1:			//���h��
			Xreg->dx = SinMove(work, 3)*Xreg->work[2];
			
			break;
		case 2:			//�c�h��
			Xreg->dy = SinMove(work, 1)*Xreg->work[2];
			break;
	}
	Xreg->work[1]++;

}

// =================================================================
// 	TeachActorSet
// �T�v  : ��ʕ\���p�̃A�N�^�[�����ׂăZ�b�g
// ����  : none
// �߂�l: none
// =================================================================
static void 	TeachActorSet()
{
	int i;
	
	tmw->actno[0] = AddActor(&CursorHeader,48,16,0);		//���ړ����
	ActWork[tmw->actno[0]].work[0] = 1;
	ActWork[tmw->actno[0]].work[2] = -1;
	
	tmw->actno[1] = AddActor(&CursorHeader,112,16,0);		//���ړ����E
	ActAnmChg(&ActWork[tmw->actno[1]],1);
	ActWork[tmw->actno[1]].work[0] = 1;
	ActWork[tmw->actno[1]].work[2] = 1;

	tmw->actno[2] = AddActor(&CursorHeader2,200,4,0);		//�c�ړ�����
	ActAnmChg(&ActWork[tmw->actno[2]],1);
	ActWork[tmw->actno[2]].work[0] = 2;
	ActWork[tmw->actno[2]].work[2] = -1;
	tmw->actno[3] = AddActor(&CursorHeader2, 200,108,0);	//�c�ړ����
	ActWork[tmw->actno[3]].work[0] = 2;
	ActWork[tmw->actno[3]].work[2] = 1;

	//�n�[�g�i�ԁj
	for(i=0;i<8;i++){
		tmw->actno[HEART1_NO+i] = AddActor(&HeartHeader, 44+(i%4)*8, 52+(i/4)*8,0);		
//		ActAnmChg(&ActWork[tmw->actno[HEART1_NO+i]],2);
	}
	
	//�n�[�g�i���j
	for(i=0;i<8;i++){
		tmw->actno[BLACKHEART1_NO+i] = AddActor(&HeartHeader, 124+(i%4)*8,52+(i/4)*8,0);		
		ActAnmChg(&ActWork[tmw->actno[BLACKHEART1_NO+i]],2);
	}

	//�S���B��
	for(i=0;i<4+16;i++){
		ActWork[tmw->actno[i]].banish = 1;
	}
}

// =================================================================
// GetWazaParameter
// �T�v  : �����p�����[�^���擾
// ����  : none
// �߂�l: none
// =================================================================
static void GetWazaParameter()
{
	int i;
	u8 strbuf[MONS_NAME_SIZE+EOM_SIZE];

	tmw->list.max	= PokeOboeruWazaGet(&PokeParaMine[tmw->pokeno],tmw->wazalist);
	for(i=0;i<tmw->list.max;i++)
		PM_strcpy(tmw->wazaname[i],WazaNameData[tmw->wazalist[i]]);

	PokeParaGet(&PokeParaMine[tmw->pokeno],ID_nickname,strbuf);	//�|�P���������i�[
	NickNameCopy(StrTempBuffer0,strbuf);
	PM_strcpy(tmw->wazaname[tmw->list.max],modoru_mes);							//�u���ǂ�v�ǉ�
	tmw->list.max++;															//���X�g�����{�P

}

// =================================================================
// SelectCursorPut
// �T�v  : �J�[�\�����ړ������ĕ\������
// ����  : move
// �߂�l: none
// =================================================================
static void SelectCursorPut(s8 move)
{
	tmw->list.old = tmw->list.pos;
	tmw->list.pos += move;		
	tmw->list.rewrite_cursor=1;
}

// =================================================================
// ListControl
// �T�v  : ���X�g�̃L�[�R���g���[��
// ����  : none
// �߂�l: none
// =================================================================
static void WazaListControl()
{
	if(sys.Repeat&U_KEY){
		if(tmw->list.select!=0){
			SePlay(SE_SELECT);
			tmw->list.select--;			//�I�����ڎ��̂͂Ƃɂ�������
			tmw->inforewrite = 1;
			if(tmw->list.pos!=0){		//�J�[�\���ʒu�͍ŏ�i���H
				SelectCursorPut(-1);
			}else{
				if(tmw->list.top!=0){	//�X�N���[���ʒu�͍ŏ�i���H
					tmw->list.top--;
					tmw->list.rewrite++;
				}
			}
		}
	}else if(sys.Repeat&D_KEY){
		if(tmw->list.select<tmw->list.max-1){
			SePlay(SE_SELECT);
			tmw->list.select++;			//�I�����ڎ��̂͂Ƃɂ�������
			tmw->inforewrite = 1;
			if(tmw->list.pos!=WAZALIST_APPEAR-1){		
				SelectCursorPut(1);		//�J�[�\���ʒu�͍ŉ��i���H
			}else{
				if(tmw->list.top!=(tmw->list.max-WAZALIST_APPEAR)){	//�X�N���[���ʒu�͍ŉ��i���H
					tmw->list.top++;
					tmw->list.rewrite++;
				}
			}
		}
	
	}else if(sys.Trg&A_BUTTON){					//���������o�������܂����H
		SePlay(SE_SELECT);
		if(tmw->list.select!=tmw->list.max-1){
			tmw->seq = TEACHMAIN_YESNO_MES;
			PM_strcpy(StrTempBuffer1,tmw->wazaname[tmw->list.select]);
			PM_MsgExpand(MsgExpandBuffer,teach2_mes);
			NMenuTalkMsgSet(MsgExpandBuffer,3,15);
		}else{												//�u���ǂ�v��I��
			PM_MsgExpand(MsgExpandBuffer,teach8_mes);
			NMenuTalkMsgSet(MsgExpandBuffer,3,15);
			tmw->seq = TEACHMAIN_END_MES;
		}
	}else if(sys.Trg&B_BUTTON){					//�킴�I���L�����Z��
		SePlay(SE_SELECT);
		tmw->seq = TEACHMAIN_END_MES;
		PM_MsgExpand(MsgExpandBuffer,teach8_mes);
		NMenuTalkMsgSet(MsgExpandBuffer,3,15);
	}
	
	if(tmw->list.max>WAZALIST_APPEAR){				//�Z���X�g�̏㉺�̖��̕\���𐧌�
		ActWork[UPARROW_NO].banish = 0;
		ActWork[DOWNARROW_NO].banish = 0;
		if(tmw->list.top==0) ActWork[UPARROW_NO].banish = 1;
		else	if(tmw->list.top==(tmw->list.max-WAZALIST_APPEAR)){
			ActWork[DOWNARROW_NO].banish = 1;
		}
	}
}

// =================================================================
// WazaListPut
// �T�v  : �Z���X�g�\������
// ����  : none
// �߂�l: none
// =================================================================
static void WazaListPut()
{
	int i,work;

	NMenuBoxWrite(commonwindow_tbl[1][0]+1,commonwindow_tbl[1][1]+1,
					commonwindow_tbl[1][2]-1,commonwindow_tbl[1][3]-1);
	
	if(tmw->list.max<WAZALIST_APPEAR) work = tmw->list.max;
	else							  work = WAZALIST_APPEAR;
	for(i=0;i<work;i++){
		NMenuMsgWrite(tmw->wazaname[i+tmw->list.top],22,1+i*2);
	}
	SelectCursorPut(0);
}

static const u8 infotbl[][3]={
	{5,4,1},		//type
	{16,4,2},		//�З�
	{5,6,3},		//pp
	{16,6,4},		//������
	{5,4,5},		//�R���e�X�gtype
	{8,6,6},		//�A�s�[��
	{18,6,7},		//�W�Q
};

static const u8 infolist[]={0,1,2,3};
static const u8 infolist2[]={4,5,6};

// =================================================================
// PrintInfoMsg
// �T�v  : �w��̐��l��\��
// ����  : tbl[]
// �߂�l: none
// =================================================================
static void PrintInfoMsg(u16 no, const u8 tbl[])
{
	u8 strbuf[50],work,i;
	PM_strcpy(strbuf,dummy_string);
	switch(tbl[2]){
		case 1:
			NMenuBoxWrite(tbl[0],tbl[1],tbl[0]+4,tbl[1]+1);
			PM_strcpy(strbuf,ZokuseiMsg[WTD[no].wazatype]);	//����
			break;	
		case 2:
			NMenuBoxWrite(tbl[0],tbl[1],tbl[0]+2,tbl[1]+1);
			if(WTD[no].damage<=1){			//�Z�_���[�W���P���O�Ȃ�u---�v
				PM_strcpy(strbuf,str_bou3);
			}else{
				PM_NumMsgSet(strbuf, WTD[no].damage, NUM_MODE_LEFT, 3);	//�_���[�W
			}
			break;
		case 3:
			
			NMenuBoxWrite(tbl[0],tbl[1],tbl[0]+1,tbl[1]+1);
			PM_NumMsgSet(strbuf, WTD[no].pp, NUM_MODE_LEFT, 2);	//PP
			break;
		case 4:
			NMenuBoxWrite(tbl[0],tbl[1],tbl[0]+2,tbl[1]+1);
			if(WTD[no].hitprobability == 0){	//�������O�̎��́u---�v
				PM_strcpy(strbuf,str_bou3);
			}else{
				PM_NumMsgSet(strbuf, WTD[no].hitprobability, NUM_MODE_LEFT, 3);	//�߂����イ���
			}
			break;
		case 5:
			NMenuBoxWrite(tbl[0],tbl[1],tbl[0]+4,tbl[1]+1);
			PM_strcpy(strbuf,ConTypeMsgTbl[ConWazaTbl[no].con_type]);	//�R���e�X�g����
			break;
		case 6:
			NMenuBoxWrite(tbl[0],tbl[1],tbl[0],tbl[1]+1);
			work = WazaKoukaTbl[ConWazaTbl[no].apno].ap / METER_ONE_AP;
			if(work==0xff) work = 0;
//			PM_NumMsgSet(strbuf, work, NUM_MODE_LEFT, 1);	//�A�s�[��
			for(i=0;i<8;i++){
				if(i<work)		ActAnmChg(&ActWork[tmw->actno[HEART1_NO+i]],1);
				else			ActAnmChg(&ActWork[tmw->actno[HEART1_NO+i]],0);
			}
			break;
		case 7:
			NMenuBoxWrite(tbl[0],tbl[1],tbl[0],tbl[1]+1);
			work = WazaKoukaTbl[ConWazaTbl[no].apno].bougai / METER_ONE_AP;
			if(work==0xff) work = 0;
//			PM_NumMsgSet(strbuf, work, NUM_MODE_LEFT, 1);	//�ڂ�����
			for(i=0;i<8;i++){
				if(i<work)		ActAnmChg(&ActWork[tmw->actno[BLACKHEART1_NO+i]],3);
				else			ActAnmChg(&ActWork[tmw->actno[BLACKHEART1_NO+i]],2);
			}
			break;
	}
	
	NMenuMsgWrite(strbuf,tbl[0],tbl[1]);
}
// =================================================================
// WazaInfoWrite
// �T�v  : �Z���̕\��
// ����  : info
// �߂�l: none
// =================================================================
static void WazaInfoWrite(u8 info)
{
	u16 wazano,i;

	if(tmw->list.select!=tmw->list.max-1){						//�u���ǂ�v����Ȃ�������
		wazano = tmw->wazalist[tmw->list.select];				//���U������\��
		if(info){	//�R���e�X�g
			for(i=0;i<16;i++)
				ActWork[tmw->actno[HEART1_NO+i]].banish = 0;
			for(i=0;i<3;i++){
				PrintInfoMsg(wazano,infotbl[infolist2[i]]);
			}
			NMenuBoxWrite(11,4,13,5);		//�uPP/�v������
			NMenuBoxWrite(1,9,18,9+3);		//�������͂�����
			NMenuMsgWrite(WazaSetumeiMsgTbl[ConWazaTbl[wazano].apno],1,9);//�����\��
		}else{		//�퓬�p
			for(i=0;i<4;i++){
				PrintInfoMsg(wazano,infotbl[infolist[i]]);
			}
			NMenuBoxWrite(1,9,18,9+3);		//�������͂�����
			NMenuMsgWrite(WazaInfo[wazano-1],1,9);	//�����\��
		}
	}else{														//�u���ǂ�v�I���̎��͏���
		if(info){	//�R���e�X�g
			NMenuBoxWrite(11,4,13,5);		//�uPP/�v������
			NMenuBoxWrite(infotbl[4][0],infotbl[4][1],infotbl[4][0]+4,infotbl[4][1]+1);
			NMenuBoxWrite(infotbl[5][0],infotbl[5][1],infotbl[5][0],infotbl[5][1]+1);
			NMenuBoxWrite(infotbl[6][0],infotbl[6][1],infotbl[6][0],infotbl[6][1]+1);
			for(i=0;i<16;i++)
				ActWork[tmw->actno[HEART1_NO+i]].banish = 1;
		}else{		//�퓬
			NMenuBoxWrite(infotbl[0][0],infotbl[0][1],infotbl[0][0]+4,infotbl[0][1]+1);
			NMenuBoxWrite(infotbl[1][0],infotbl[1][1],infotbl[1][0]+2,infotbl[1][1]+1);
			NMenuBoxWrite(infotbl[2][0],infotbl[2][1],infotbl[2][0]+2,infotbl[2][1]+1);
			NMenuBoxWrite(infotbl[3][0],infotbl[3][1],infotbl[3][0]+2,infotbl[3][1]+1);
		}
		NMenuBoxWrite(1,9,18,9+3);
	}
}
