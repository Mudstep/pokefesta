//=========================================================================
//	�|�P�����������񃂃W���[��
//
//    Copyright (C) 2000 GAME FREAK inc.
//											By Akito Mori
//=========================================================================

//KoukanWork�̃A�h���X��UserWork���犄��t���Ă܂���
//�A�h���X�w��̐������K���ł��B����\������̂ŋC�����悤

//���́@�|�P�������@�������񂷂��		//������
//����Ƃ��@�ł��Ȃ��@�Ȃ����Ⴄ��I

//
//

#include "common.h"
#include "evobj.h"

#include "calctool.h"
#include "poketool.h"
#include "multisio.h"
#include "pokesio.h"
#include "cable.h"
#include "mus_tool.h"

#include "intr.h"

#include "script.h"
#include "fight.h"
#include "weather.h"

#include "param.h"
#include "fld_main.h"
#include "fld_task.h"
#include "fld_tool.h"
#include "task.h"
#include "print.h"
#include "fcontext.h"
#include "window.h"
#include "menu.h"
#include "menutool.h"
#include "boxsub.h"
#include "p_status.h"

#include "actor.h"
#include "actanm.h"
#include "Madefine.h"
#include "palanm.h"
#include "mail.h"
#include "server.h"

#include "pokeicon.h"
#include "wazatool.h"

#include "gauge.h"
#include "extanm.h"

#include "..\scaddata\kk_bg.acl"		
#include "..\scaddata\kk_bg.ach"		
#include "..\scaddata\kk_bg2.asc"		//Ҳ݂̳���޳
#include "..\scaddata\kk_bg2a.asc"		//�����޳�̎����߰�
#include "..\scaddata\kk_bg2a2.asc"		//Ҳݳ���޳�ɖ߂����߂��߰�
#include "..\scaddata\kk_bg2b.asc"		//��۰ق���w�i�iBG2���j
#include "..\scaddata\kk_bg3b.asc"		//��۰ق���w�i�iBG3���j
#include "..\scaddata\kk_bg4.asc"		//�|�P�����̏���Ƴ���޳

#include "..\scaddata\kk_cusor.ach"

#include "koukan.h"	//�A�N�^�[��`


//#define DEBUG_NUM_PRINT
//#define DEBUG_PRINT
//#define KOUKAN_DEBUG
//#define DEBUG_PLAY


extern u8 multisiotestdata[];
extern const u8 testdata[][256];						//ý��ް�
extern const u8	PokeNameData[][6];
extern const u8	WazaNameData[][8];
extern SIOTRAINER_INFO SioTrainerInfo[];

extern void KoukanDemoInit();
extern void InitFight();
extern void JidouNinshouWatchDog(u8 no);


static u8 count;

extern void CommBattleEnd(void);
extern void CommTradeEnd(void);
extern u8 *PM_SexStrcat(u8 *buf,u8 sex);

extern void kanshi_network(u8 taskno);
extern void kanshi_num_set();
extern void KoukanPokeItemIconActorSet(u8 pokenum[2], u8 iconlist[2][6], u8 side);
extern void ItmeIconCellPalSet(void);


static void KoukanMainInitSub(void);
static u8 RenzokuCellSet();
static void PokeStatusGet(u8 no);
static void PokeIconActionSearch(u8 no);
static void PokeIconActionSet();
static void RibbonInfoCheck();
static u8 StatusSubCheck(u8 *tbl, u8 num, u8 pos);

//=========================================================================
//	�O�����[�N���m��
//=========================================================================
MAIL_DATA	EnemyMail[6]={};


//=========================================================================
//	��`
//=========================================================================
//#define name value
#define OFFSET_NAME			
#define OFFSET_YESNO
#define OFFSET_ARROW
#define OFFSET_STATUS_NAME
#define OFFSET_STATUS_WAZA

#define YESNO_POS_X1		(24)
#define YESNO_POS_Y1		(14)
#define YESNO_POS_W			(5)
#define YESNO_POS_H			(5)
#define YESNO_POS_X2		(YESNO_POS_X1+YESNO_POS_W)
#define YESNO_POS_Y2		(YESNO_POS_Y1+YESNO_POS_H)

//				NMenuWinBoxWrite(23,14,28,19);

#define STATUSMENU_X1		(18)
#define STATUSMENU_Y1		(14)
#define STATUSMENU_W		(10)
#define STATUSMENU_H		(5)
#define STATUSMENU_X2		(STATUSMENU_X1+STATUSMENU_W)
#define STATUSMENU_Y2		(STATUSMENU_Y1+STATUSMENU_H)


static const u8 yameru_msg[]={ya_,me_,ru_,EOM_,};				//�u��߂�v
static const u8 select_msg[]={PO_,KE_,MO_,N_,wo_,spc_,e_,ra_,n_,de_,spc_,ku_,da_,sa_,i_,EOM_,};//�߹�݂��@�����Ł@��������
static const u8 status_msg[]={tu_,yo_,sa_,wo_,mi_,ru_,EOM_,};	//�悳���݂�
static const u8 koukan_msg[]={ko_,u_,ka_,n_,ni_,da_,su_,EOM_,};			//��������ɂ���
static const u8 yamemasuka_msg[]={ko_,u_,ka_,n_,spc_,wo_,spc_,ya_,me_,ma_,su_,ka_,hate_,EOM_,};//�����������߂܂���
static const u8 bbotande_msg[]={B__,BO_,TA_,N_,spc_,de_,spc_,mo_,do_,ri_,ma_,su_,EOM_,};	//�a�{�^���ł��ǂ�܂�

//���b�Z�[�W�e�[�u��
static const u8  * const kk_msg_tbl[]={
	yameru_msg,				//��߂�
	select_msg,				//�߹�݂�  ������  ��������
	status_msg,				//�X�^�[�^�X���݂�
	koukan_msg,				//��������ɂ���
	yamemasuka_msg,			//�����������߂܂����H
	bbotande_msg,			//�a�{�^���Ŗ߂�܂��B
};


//�J�[�\���ړ��p�̃e�[�u��
//�㉺���E�̏��ɒ�`���Ă���܂�
//  �O �P    �U  �V
//  �Q �R    �W  �X
//  �S �T  �P�O�P�P
//             �P�Q
static const u8 movepostbl[][4][6]={
									//   0 �̈ʒu�����4�����̍s������̗D��xð���(�㉺���E�̏�)
									//   ���Ȃ������̕����D��x������
	{{ 4, 2,12,12},{ 2, 4,12,12},{ 7, 6, 1, 0},{ 1, 6, 7, 0},},		//0
	{{ 5, 3,12,12},{ 3, 5,12,12},{ 0, 7, 6, 1},{ 6, 7, 0, 1},},		
	{{ 0, 0, 0, 0},{ 4, 0, 0, 0},{ 9, 8, 7, 6},{ 3, 1,     },},		
	{{ 1, 1, 1, 1},{ 5, 1, 1, 1},{ 2, 9, 8, 7},{ 8, 9, 6, 6},},		
	{{ 2, 2, 2, 2},{ 0, 0, 0, 0},{11,10,9,8,7,6},{ 5,3,1,  },},		
	{{ 3, 3, 3, 3},{ 1, 1, 1, 1},{ 4, 4, 4, 4},{10, 8, 6,  },},		//5
	{{10, 8,12,  },{ 8,10,12,  },{ 1, 0,     },{ 7, 0, 1,  },},		
	{{12,        },{ 9,12,     },{ 6,        },{ 0,        },},		
	{{ 6,        },{10, 6,     },{ 3, 2, 1, 0},{ 9, 7,     },},		
	{{ 7,        },{11,12,     },{ 8,        },{ 2, 1, 0,  },},		
	{{ 8,        },{ 6,        },{ 5, 4, 3, 2,1,0},{11, 9, 7, },},	//10
	{{ 9,        },{12,        },{10,        },{ 4, 2, 0,  },},		
	{{11, 9, 7, 6},{ 7, 6,     },{12,        },{12,        },},		

};


//�|�P�����̖��O���W�e�[�u��
static const u8 namepos[13][2]={
	{ 1, 5     },			{ 8, 5     },
	{ 1, 5+5   },			{ 8, 5+5   },
	{ 1, 5+5*2 },			{ 8, 5+5*2 },

	{16, 5     },			{23, 5     },
	{16, 5+5   },			{23, 5+5   },
	{16, 5+5*2 },			{23, 5+5*2 },
	{23, 5+5*2+3},
};

//�|�P�����̃��x���\���e�[�u��
static const u8 levelpos[12][2]={
	{5,4},		{5+7,4},
	{5,4+5},	{5+7,4+5},
	{5,4+5*2},	{5+7,4+5*2},

	{20,4},		{20+7,4},
	{20,4+5},	{20+7,4+5},
	{20,4+5*2},	{20+7,4+5*2},
};

//�|�P�����̃��x���\���̃E�B���h�E�̃e�[�u��
static const u8 levelwindowpos[12][2]={
	{1,3    },	{8,3    },
	{1,3+5  },	{8,3+5  },
	{1,3+10 },	{8,3+10 },

	{16,3    },	{16+7, 3    },
	{16,3+5  },	{16+7, 3+5  },
	{16,3+10 },	{16+7, 3+10 },
};



static const u8 playername[2][2][2]={
	{
		{60,10},
		{120+60,10},
	},
	{
		{48,10},
		{120+48,10},
	},
};


static const u8 str_status[] = {SU_,TE_,bou_,TA_,SU_,wo_,mi_,ru_,EOM_,};
static const u8 str_koukan[] = {ko_,u_,ka_,n_,ni_,da_,su_,EOM_,};


//-----------------------------------------------------------------------
//  system_message_window�p�̒�`
//-----------------------------------------------------------------------
static const u8 str_taiki[]  = {
	CTRL_,C_FCOL_,COL_BLACK,CTRL_,C_BCOL_,COL_WHITE,CTRL_,C_SCOL_,COL_DGRAY,
	tu_,u_,si_,n_,ta_,i_,ki_,ti_,yyu_,u_,gyoe_,CR_,
	si_,ba_,ra_,ku_,o_,ma_,ti_,ku_,da_,sa_,i_,EOM_,};


static const u8 str_cansel[]  = {
	CTRL_,C_FCOL_,COL_BLACK,CTRL_,C_BCOL_,COL_WHITE,CTRL_,C_SCOL_,COL_DGRAY,
	ko_,u_,ka_,n_,ha_,CR_,KI_,YYA_,N_,SE_,RU_,spc_,sa_,re_,ma_,si_,ta_,gyoe_,EOM_,
};

static const u8 str_notrade1[]={
	CTRL_,C_FCOL_,COL_BLACK,CTRL_,C_BCOL_,COL_WHITE,CTRL_,C_SCOL_,COL_DGRAY,
	so_,no_,PO_,KE_,MO_,N_,wo_,spc_,ko_,u_,ka_,n_,su_,ru_,to_,CR_,
	se_,n_,to_,u_,de_,ki_,na_,ku_,na_,ttu_,ti_,yya_,u_,yo_,gyoe_,EOM_,
};

static const u8 str_notrade2[]={
	CTRL_,C_FCOL_,COL_BLACK,CTRL_,C_BCOL_,COL_WHITE,CTRL_,C_SCOL_,COL_DGRAY,
	ko_,re_,de_,ko_,u_,ka_,n_,su_,ru_,to_,spc_,a_,i_,te_,ga_,CR_,
	se_,n_,to_,u_,de_,ki_,na_,ku_,na_,ttu_,ti_,yya_,u_,yo_,gyoe_,EOM_,
};

static const u8 str_endwait[]={
	CTRL_,C_FCOL_,COL_BLACK,CTRL_,C_BCOL_,COL_WHITE,CTRL_,C_SCOL_,COL_DGRAY,
	to_,mo_,da_,ti_,no_,spc_,si_,yyu_,u_,ri_,yyo_,u_,wo_,CR_,
	ma_,ttu_,te_,i_,ma_,su_,tenten_,tenten_,EOM_,
};

static const u8 str_tradecontinue[]={
	CTRL_,C_FCOL_,COL_BLACK,CTRL_,C_BCOL_,COL_WHITE,CTRL_,C_SCOL_,COL_DGRAY,
	to_,mo_,da_,ti_,ha_,spc_,PO_,KE_,MO_,N_,no_,CR_,
	ko_,u_,ka_,n_,wo_,spc_,si_,ta_,i_,yo_,u_,de_,su_,gyoe_,EOM_,
};


static const u8 * const system_msg[]={
	str_taiki,
	str_cansel,
	str_notrade1,
	str_notrade2,
	str_endwait,
	str_tradecontinue,
};

static const u8 sysmsg_tbl[][4]={		//������ɂ���ăE�C���h�E�̑傫����ς���
	{	8 , 7, 22, 12	},				//system_msg�̕�����ɑΉ������傫��
	{	8 , 7, 22, 12	},				
//	{	5 , 8, 25, 11,	},
	{	6 , 7, 24, 12	},
	{	6 , 7, 24, 12	},
	{	8 , 7, 22, 12	},
	{	7 , 7, 23, 12	},
};

static const u8 str_to[]={to_, spc_,EOM_};
static const u8 str_simasuka[]={ wo_,spc_,ko_,u_,ka_,n_,si_,ma_,su_,ka_,hate_,EOM_,};

static void Menustatus(void);
static void Menukoukan(void);

static const MENU_ITEM menu[]={
	{str_status,  Menustatus},
	{str_koukan,  Menukoukan},
};

//==========================================================================
static const u8 clear_area[2][2]={
	{0,14,},
	{15,29,},
};

static const u8 statuspos[4][2]={
	{3,5,},
	{3,7},
	{18,5,},
	{18,7},
};


static const u8 colchg[]={EOM_,};
//static const u8 colchg[]={CTRL_,C_FCOL_,COL_BLACK,CTRL_,C_BCOL_,COL_WHITE,EOM_,};
const u8 sex_msg[3][2]={
	{osu_,EOM_,},
	{mesu_, EOM_,},
	{EOM_,EOM_}
};

static const u8 waza_str[]={wa_,za_,EOM_};
static const u8 cr_str[]={CR_,EOM_};
static const u8 str_sura[] = {sura_,EOM_};
static const u8 str_lv[]={L__,v__,EOM_};
static const u8 str_bou[] = {bou_,bou_,bou_,EOM_,};
static const u8 str_hatena[] = {hate_,hate_,hate_,hate_,EOM_,};

typedef struct{
	u8	flg;
	u16 wait;
	u8  funcno;
}QUE_FUNC;


typedef struct{
	u8 x;
	u8 y;
	u16 *src;
	u16 *des;
	u8  w;
	u8  h;
	u16 mask;
	u8  transflag;
	u16	bgvram[32*32];
}BLOCK_COPY_WORK;

#define QUE_MAX	(4)
//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;
/*-------------------- �O���[�o�� �ϐ�  ----------------------------*/
u8 exchange_pokemon[2]={0,0};		//�����E����

//LOCAL type valiable;
/*-------------------- ���[�J�� �ϐ� -------------------------------*/


typedef struct{
	u8  		bgofs[2];			//�w�i��۰ق̍��W
	TALK_WIN 	tw;					//÷�ĺ�÷��
	u8  		iconno[2][6];		//�߹�ݱ��݂̱���NO
	u8			cursorno;			//���ق̱���NO
	u8			cursorposno;		//���ق̈ʒu
	u8			pokenum[2];			//�߹�݂̐�
	u8			existtbl[13];		//���ۂ��߹�݂����邩�ǂ������׸�
	u8			pokestatus[2][6];	//�|�P�����̏�ԁi1:�ʏ�E0:�ُ�i�m�����^�}�S�j�j
	u8			tamago[2][6];		//�^�}�S���ǂ����t���O
	u8			iconaction[2][6];	//�|�P�����A�C�R���p�̓���t���O
	u8			exchangeflag;		//
	u8			exchangeold;
	u16			exchangecount;
	u8			windowwork;
	u8			menuflag;
	u8			menupos;
	u16			inst_no;			//�������͂�OBJVRAM�ʒu
	u8			statuspage_flag[2];
	u8 			statuspage_num[2];

	u8			wait_flag[2];		//���������Ƃ����t���O
	u8			ok_flag[2];			//��������n�j�Ƃ����t���O
	u8			end_flag[2];		//�I���n�j�̃t���O
	u8			enemy_select;		//����̑I�������|�P����
	u16			sendwork[20];
	u8			wait;
	u8			RibbonTbl[16];		//����̃��{�����R�s�[
	BLOCK_COPY_WORK bcw;
	QUE_FUNC	que[QUE_MAX];
}KOUKAN_WORK;

KOUKAN_WORK *kw;

//=========================================================================
//  extern�ϐ�
//=========================================================================

/*---------------------- �T�u���[�`��  -----------------------------*/


//=========================================================================
//  extern�֐�
//=========================================================================

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);
void KoukanMainInit(void);
void KoukanMain(void);


//LOCAL	type function()
static void VInterFunc( void );
static void koukan_bg_set(u8 turn);
static void pokemonicon_refresh();
static u8 ExchangePoke();
static void PadFunc();
static void statuspage_set(u8 pos);
static u8 pokemon_info_set(u8 *buf, u8 group, u8 pos);
static void level_put(u8 side, u8 n, u8 lx, u8 ly, u8 wx, u8 wy);
static void pokelevel_put(u8 side);

static void statuspage_set_start(u8 pos);
static void statuspage_set(u8 pos);
static void pokemon_waza_set(u8 *buf, u8 group, u8 pos);
static void SetExplainFont32x16(const u8 *str, u8 *dst, u8 max);


void block_map_copy16(BLOCK_COPY_WORK *work,char x, char y, u16*src, char w,char h,u16 mask);
void block_map_copy16_main(BLOCK_COPY_WORK *work);
void block_map_copy_init(BLOCK_COPY_WORK *work, u16* des);
void block_map_refresh(BLOCK_COPY_WORK *work);


static void pokename_put(u8 side);
static void pokeicon_pos_set(u8 side);
static void return_statuspage(u8 side);
static void menu_erase();

void KoukanSioFunc();

static void parent_func(u8 id, u8 result);
static void child_func(u8 id, u8 result);

static void system_message_window(u8 mes);

static void QueFuncSet(u16 wait, u8 funcno);
static void QueFuncMain();

static void fadeout_func();
static void fadeout_func();

static void soujusin_msg(u8 souju, u8 num);



extern void kanshi_network(u8 taskno);


enum{
	KOUKAN_INIT_ENTER=0,
	KOUKAN_INIT_START,
	KOUKAN_SIO_START_WAIT,
	KOUKAN_SIO_START,
	KOUKAN_SIO_WAIT,
	KOUKAN_SIO_EXCHANGE,
	KOUKAN_INIT_POKEGRA,
	KOUKAN_INIT_POKEICON1,
	KOUKAN_INIT_POKEICON2,
	KOUKAN_INIT_TEXT,
	KOUKAN_INIT_CELLSET,
	KOUKAN_INIT_NAME,
	KOUKAN_INIT_GAMEN,
	KOUKAN_INIT_BG_CHECK1,
	KOUKAN_INIT_BG_CHECK2,
	KOUKAN_INIT_BG1,
	KOUKAN_INIT_BG2,
	KOUKAN_INIT_FADEIN,
	KOUKAN_INIT_BGREG,
	KOUKAN_INIT_SEARCH1,
	KOUKAN_INIT_SEARCH2,	
	KOUKAN_INIT_END,
};


//------------------------------------------------------------------------
//  �|�P����������ʏ�����
//------------------------------------------------------------------------
void KoukanMainInit(void)
{
	MainProcChange(KoukanMainInitSub);
}


//=========================================================================
//�u�|�P������������v��ʏ�����
//=========================================================================
static void KoukanMainInitSub(void)
{
	int i;
	u8 id,flag;
	actHeader acttemp;



	switch(sys.InitSeqWork)
	{
		case KOUKAN_INIT_ENTER:
			kw = (KOUKAN_WORK*)&UserWork[0x17000];		//�������UserWork�̱��ڽ�w��̎d����
			InitActor();				//�A�N�^�[������
			ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
			InitTask();

			block_map_copy_init(&kw->bcw,(u16*)(BG_VRAM+0x2800));	
			SetVBlankFunc(VInterFunc);

			NMenuScreenInit(FIELD_MENU_TYPE);

			PrintScreenInit(KOUKAN_TYPE);
			MsgControlInit(&(kw->tw), KOUKAN_TYPE);
			kw->windowwork = NWinFrameInit(20);
			NWinFrameDataSet(&(kw->tw));

			NMenuScreenClear();


			PokeIconPaletteAllSet();
			kw->exchangeflag = 0;
			kw->menuflag = 0;
			kw->menupos  = 0;
			kw->statuspage_flag[0] = 0;
			kw->statuspage_flag[1] = 0;
			kw->ok_flag[0] = 0;
			kw->ok_flag[1] = 0;
			kw->wait = 0;
			count = 0;
			sys.InitSeqWork++;
			system_message_window(0);		//�ʐM�ҋ@���I
			CpuClear(0,&UserWork[0x1d000],0x20*4*2*13,16);					//13��4x2��OBJ�̗̈���N���A
			for(i=0;i<13;i++){
				NameCellBuf[i] = (u8*)&UserWork[0x1d000+0x20*4*2*i];
			}
			break;
		case KOUKAN_INIT_START:
//			PaletteWorkSet(Test_Palette,(kw->tw.context->BasePalette)*16,32);
			PokemonSioNinshouNo = NINSHOU_KOUKANGAMEN;
			PokemonMultiSioInit();
			for(i=0;i<6;i++)		//�S�~�������Ă����Ƃ��΍�
				PokeParaSet(&PokeParaEnemy[i],0,0,POW_RND,RND_NO_SET,0,ID_NO_SET,0);
			sys.InitSeqWork++;
			kw->wait=0;
			AddTask(JidouNinshouWatchDog,1);			//�^�C���A�E�g�G���[����(�^�X�N)
			break;
		case KOUKAN_SIO_START_WAIT:
			kw->wait++;
			if(kw->wait>11){
				kw->wait = 0;
				sys.InitSeqWork++;
			}
			break;
		case KOUKAN_SIO_START:
			if(GetSioRealConnectNumber()>=GetSioReliableNumber()){		//��l�ڑ������Ȃ����ǂ�
				if(GetMyConnectIsParent()){			//�e��������ʐM�X�^�[�g�̏d�ׂ�
					if(++kw->wait>30){
						MultiSioSignalDecision();
						sys.InitSeqWork++;
					}
				}else{								//�q�͋C�y�ɒʐM�X�^�[�g�҂�
					sys.InitSeqWork++;
				}
			}
			break;
		case KOUKAN_SIO_WAIT:
			if(pokemon_sio_flag==1 && PokemonNinshouNoCheck()==1){
				PokeCountMineGet();
				sys.InitSeqWork++;
			}
			break;
		case KOUKAN_SIO_EXCHANGE:
			if(ExchangePoke()){
				RibbonInfoCheck();				//���{�����̃`�F�b�N
				sys.InitSeqWork++;
			}
			break;
		case KOUKAN_INIT_POKEGRA:
			PokeCountEnemyGet();
			FontScrnBoxFill(&(kw->tw),0,  0, 0, 29, 19);
			*(vu16 *)REG_DISPCNT = 	0;			//�\���� 
			kw->pokenum[0] = PokeCountMine;
			kw->pokenum[1] = PokeCountEnemy;

			for(i=0;i<kw->pokenum[0];i++){					//�|�P�����A�C�R���\��
				kw->iconno[0][i]=PokeIconSet2(
					PokeParaGet(&PokeParaMine[i],ID_monsno_egg), 
					PokeIconDmmyMove, namepos[i][0]*8+14, namepos[i][1]*8-12, 1,
					PokeParaGet(&PokeParaMine[i],ID_personal_rnd));
			}
			for(i=0;i<kw->pokenum[1];i++){
				kw->iconno[1][i]=PokeIconSet2(
				PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg,NULL), 
					PokeIconDmmyMove, namepos[i+6][0]*8+14,namepos[i+6][1]*8-12, 1,
					PokeParaGet(&PokeParaEnemy[i],ID_personal_rnd));
			}
			soujusin_msg(2,0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_POKEICON1:
			ItmeIconCellPalSet();
			KoukanPokeItemIconActorSet(kw->pokenum, kw->iconno, 0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_POKEICON2:
			KoukanPokeItemIconActorSet(kw->pokenum, kw->iconno, 1);
			sys.InitSeqWork++;
			break;

		case KOUKAN_INIT_TEXT:		//
			BoxDrawFont_32x16(MyData.my_name, NameCellBuf[0], DRAWTYPE_16TRANS, 0, UserWork);		//�����̖��O
			id = GetSioConnectId();
			BoxDrawFont_32x16(SioTrainerInfo[id^1].name, NameCellBuf[3], DRAWTYPE_16TRANS, 0, UserWork);		//����̖��O
			BoxDrawFont_32x16(kk_msg_tbl[0], NameCellBuf[6], DRAWTYPE_16TRANS, 0, UserWork);	//�u��߂�v
			SetExplainFont32x16(kk_msg_tbl[1], NameCellBuf[7], EXPLAIN_CELL);
			soujusin_msg(3,0);
			sys.InitSeqWork++;
			kw->wait = 0;
			break;
		case KOUKAN_INIT_CELLSET:
			soujusin_msg(4,0);
			if(RenzokuCellSet()){
				sys.InitSeqWork++;
			}
			break;
		case KOUKAN_INIT_NAME:
			if(PM_strlen(MyData.my_name)>5) flag = 1;
			else							flag = 0;
			for(i=0;i<3;i++){					//�����̖��O�\��
				acttemp = ActNameHeader;
				acttemp.cellNo+=i;
				AddActor(&acttemp,playername[flag][0][0]+i*32,playername[flag][0][1],1);
			}
			id = GetSioConnectId();
			if(PM_strlen(SioTrainerInfo[id^1].name)>5) flag = 1;
			else							flag = 0;
			for(i=0;i<3;i++){						//����̖��O�\��
				acttemp = ActNameHeader;
				acttemp.cellNo+=i+3;
				AddActor(&acttemp,playername[flag][1][0]+i*32,playername[flag][1][1],1);
			}
			soujusin_msg(5,0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_GAMEN:
			acttemp = ActNameHeader;				//�u��߂�v�\��
			acttemp.cellNo+=6;
			AddActor(&acttemp,8*0x1a+16,152,1);
			for(i=0;i<6;i++){
				acttemp = ActNameHeader;			//�u�|�P������I��ł��������v�\��
				acttemp.cellNo+=7+i;
				AddActor(&acttemp,8+i*32+16,150,1);
			}
													//�J�[�\���\��
			kw->cursorno = AddActor(&CursorHeader,namepos[0][0]*8+32,namepos[0][1]*8,2);
			kw->cursorposno = 0;
			sys.InitSeqWork++;
			soujusin_msg(6,0);
			break;
		case KOUKAN_INIT_BG_CHECK1:
			PokeStatusGet(0);							//�|�P�����̐���ُ���`�F�b�N
			pokename_put(0);					//�|�P�������O�\��
			kw->bgofs[0] = 0;
			kw->bgofs[1] = 0;
			pokemonicon_refresh();
			sys.InitSeqWork++;
			soujusin_msg(7,0);
			
			MusicPlay(MUS_P_SCHOOL);
			break;
		case KOUKAN_INIT_BG_CHECK2:
			PokeStatusGet(1);							//�|�P�����̐���ُ���`�F�b�N
			pokename_put(1);
			sys.InitSeqWork++;
		case KOUKAN_INIT_BG1:
			koukan_bg_set(0);							//BG�Z�b�g1���
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_BG2:
			koukan_bg_set(1);							//BG�Z�b�g2���
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_FADEIN:
			PaletteFadeReq(0xffffffff,0,16,0,0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_BGREG:

			/*---------��ʕ\���J�n----------*/
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_BG1_ON					// BG0�L��
						  | DISP_BG2_ON					// BG2�L��
						  | DISP_BG3_ON					// BG3�L��
						  | DISP_OBJ_ON;

#ifdef DEBUG_NUM_PRINT
			DebugNumPrintInit(1,2,6,0);
			AddTask(kanshi_network,0);
#endif
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_SEARCH1:	
			PokeIconActionSearch(0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_SEARCH2:	
			PokeIconActionSearch(1);
			PokeIconActionSet();
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_END:
			if( FadeData.fade_sw == 0 ){
//				PushFirstProc  = sys.pFirstProc;
				sys.pFirstProc = KoukanSioFunc;			//�ʐM�������풓������
				MainProcChange(KoukanMain);
				count = 0;
			}
			break;
	}

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
	
};


// =================================================================
// KoukanMainReturnInit
// �T�v  : �X�e�[�^�X��ʂ���߂��Ă����Ƃ��̕��A����
// ����  : none
// �߂�l: none
// =================================================================
void KoukanMainReturnInit(void)
{

	int i;
	u8 id,flag;
	actHeader acttemp;
	P_StatusSys *pss;


	switch(sys.InitSeqWork)
	{
		case KOUKAN_INIT_ENTER:
			kw = (KOUKAN_WORK*)&UserWork[0x17000];		//�������UserWork�̱��ڽ�w��̎d����
			InitActor();				//�A�N�^�[������
			ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
			InitTask();

			block_map_copy_init(&kw->bcw,(u16*)(BG_VRAM+0x2800));	
			SetVBlankFunc(VInterFunc);

			NMenuScreenInit(FIELD_MENU_TYPE);

			PrintScreenInit(KOUKAN_TYPE);
			MsgControlInit(&(kw->tw), KOUKAN_TYPE);
			kw->windowwork = NWinFrameInit(20);
			NWinFrameDataSet(&(kw->tw));

			NMenuScreenClear();


			PokeIconPaletteAllSet();
			kw->exchangeflag = 0;
			kw->menuflag = 0;
			kw->menupos  = 0;
			kw->statuspage_flag[0] = 0;
			kw->statuspage_flag[1] = 0;
			kw->ok_flag[0] = 0;
			kw->ok_flag[1] = 0;
			kw->wait = 0;
			count = 0;
			sys.InitSeqWork++;
			for(i=0;i<13;i++){
				NameCellBuf[i] = (u8*)&UserWork[0x1d000+0x20*4*2*i];
			}
			break;
		case KOUKAN_INIT_START:
//			PaletteWorkSet(Test_Palette,(kw->tw.context->BasePalette)*16,32);
			sys.InitSeqWork++;
			kw->wait=0;
			break;
		case KOUKAN_SIO_START_WAIT:
				sys.InitSeqWork++;
			break;
		case KOUKAN_SIO_START:
				sys.InitSeqWork++;
			break;
		case KOUKAN_SIO_WAIT:
			PokeCountMineGet();
			sys.InitSeqWork++;
			break;
		case KOUKAN_SIO_EXCHANGE:
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_POKEGRA:
			PokeCountEnemyGet();
			*(vu16 *)REG_DISPCNT = 	0;			//�\���� 
			kw->pokenum[0] = PokeCountMine;
			kw->pokenum[1] = PokeCountEnemy;

			pokename_put(0);					//�|�P�������O�\��
			pokename_put(1);
			for(i=0;i<kw->pokenum[0];i++){					//�|�P�����A�C�R���\��
				kw->iconno[0][i]=PokeIconSet2(
					PokeParaGet(&PokeParaMine[i],
					ID_monsno_egg,NULL), PokeIconDmmyMove, namepos[i][0]*8+14, namepos[i][1]*8-12, 1,
					PokeParaGet(&PokeParaMine[i],ID_personal_rnd));
			}
			for(i=0;i<kw->pokenum[1];i++){
				kw->iconno[1][i]=PokeIconSet2(
					PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg,NULL), 
					PokeIconDmmyMove, namepos[i+6][0]*8+14,namepos[i+6][1]*8-12, 1,
					PokeParaGet(&PokeParaEnemy[i],ID_personal_rnd));
			}
			soujusin_msg(2,0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_POKEICON1:
			ItmeIconCellPalSet();
			KoukanPokeItemIconActorSet(kw->pokenum, kw->iconno, 0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_POKEICON2:
			KoukanPokeItemIconActorSet(kw->pokenum, kw->iconno, 1);
			sys.InitSeqWork++;
			break;

		case KOUKAN_INIT_TEXT:		//
			BoxDrawFont_32x16(MyData.my_name, NameCellBuf[0], DRAWTYPE_16TRANS, 0, UserWork);		//�����̖��O
			id = GetSioConnectId();
			BoxDrawFont_32x16(SioTrainerInfo[id^1].name, NameCellBuf[3], DRAWTYPE_16TRANS, 0, UserWork);		//����̖��O
			BoxDrawFont_32x16(kk_msg_tbl[0], NameCellBuf[6], DRAWTYPE_16TRANS, 0, UserWork);	//�u��߂�v
			SetExplainFont32x16(kk_msg_tbl[1], NameCellBuf[7], EXPLAIN_CELL);
			soujusin_msg(3,0);
			sys.InitSeqWork++;
			kw->wait = 0;
			break;
		case KOUKAN_INIT_CELLSET:
			soujusin_msg(4,0);
			if(RenzokuCellSet()){
				sys.InitSeqWork++;
			}
			break;
		case KOUKAN_INIT_NAME:
			if(PM_strlen(MyData.my_name)>5) flag = 1;
			else							flag = 0;
			for(i=0;i<3;i++){					//�����̖��O�\��
				acttemp = ActNameHeader;
				acttemp.cellNo+=i;
				AddActor(&acttemp,playername[flag][0][0]+i*32,playername[flag][0][1],1);
			}
			id = GetSioConnectId();
			if(PM_strlen(SioTrainerInfo[id^1].name)>5) flag = 1;
			else							flag = 0;
			for(i=0;i<3;i++){						//����̖��O�\��
				acttemp = ActNameHeader;
				acttemp.cellNo+=i+3;
				AddActor(&acttemp,playername[flag][1][0]+i*32,playername[flag][1][1],1);
			}
			soujusin_msg(5,0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_GAMEN:
			acttemp = ActNameHeader;				//�u��߂�v�\��
			acttemp.cellNo+=6;
			AddActor(&acttemp,8*0x1a+16,152,1);
			for(i=0;i<6;i++){
				acttemp = ActNameHeader;			//�u�|�P������I��ł��������v�\��
				acttemp.cellNo+=7+i;
				AddActor(&acttemp,8+i*32+16,150,1);
			}
													//�J�[�\�����A
			pss = (P_StatusSys*)&UserWork[P_STATUS_USERADRS];
			if(kw->cursorposno < 6) kw->cursorposno = pss->put_no;
			else					kw->cursorposno = pss->put_no+6;
			kw->cursorno = AddActor(&CursorHeader,
				namepos[kw->cursorposno][0]*8+32,namepos[kw->cursorposno][1]*8,2);
			sys.InitSeqWork=KOUKAN_INIT_BG1;
			soujusin_msg(6,0);
			break;
		case KOUKAN_INIT_BG1:
			koukan_bg_set(0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_BG2:
			koukan_bg_set(1);							//BG�Z�b�g2���
			kw->bgofs[0] = 0;
			kw->bgofs[1] = 0;
			pokemonicon_refresh();
			soujusin_msg(7,0);
			sys.InitSeqWork++;
			break;

		case KOUKAN_INIT_FADEIN:
			PaletteFadeReq(0xffffffff,0,16,0,0);
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_BGREG:
			/*---------��ʕ\���J�n----------*/
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_BG1_ON					// BG0�L��
						  | DISP_BG2_ON					// BG2�L��
						  | DISP_BG3_ON					// BG3�L��
						  | DISP_OBJ_ON;

#ifdef DEBUG_NUM_PRINT
			DebugNumPrintInit(1,2,6,0);
			AddTask(kanshi_network,0);
#endif
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_SEARCH1:	
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_SEARCH2:	
			PokeIconActionSet();
			sys.InitSeqWork++;
			break;
		case KOUKAN_INIT_END:
			if( FadeData.fade_sw == 0 ){
				MainProcChange(KoukanMain);
				count = 0;
			}
			break;
	}
	

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static void VInterFunc( void )
{
	block_map_copy16_main(&kw->bcw);

	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}


//=========================================================================
//  �t�F�[�h�A�E�g�Z�b�g
//=========================================================================
static void fadeout_func()
{
//	if(SplitDataSendStatus()){
	if(++kw->wait>15){
		PaletteFadeReq(0xffffffff,0,0,16,0);
//		LightFadeReq(N_to_B);
		kw->menuflag = 10;
	}
//	}
}
//=========================================================================
//  �t�F�[�h�A�E�g�҂��i�����f���s���j
//=========================================================================
static void fadeout_wait_func()
{
	if( FadeData.fade_sw == 0 ){
//			PokemonMultiSioEnd();
			exchange_pokemon[0] = kw->cursorposno;
			exchange_pokemon[1] = kw->enemy_select;
			SetSioEndDataStart();		//�ʐM�I��
			kw->menuflag = 13;
	}	
}

static void multisio_end_wait()
{
	if(pokemon_sio_flag==0){
//		sys.pFirstProc = PushFirstProc;
		sys.pFirstProc = NULL;
		MainProcChange(KoukanDemoInit);
	}
}

//-------------------------------------------------------------------------
//�u��������������v��ʃ��C�����[�`��
//-------------------------------------------------------------------------
void KoukanMain(void)
{
	PadFunc();			//������


	QueFuncMain();		//QUE����

	statuspage_set(0);	//�X�e�[�^�X��ʃZ�b�g���[�`������
	statuspage_set(1);	//�X�e�[�^�X��ʃZ�b�g���[�`���E��

	*(vu16*)REG_BG2HOFS = kw->bgofs[0]++;		//�w�i�X�N���[��
	*(vu16*)REG_BG3HOFS = kw->bgofs[1]--;
	MainTask();
	JumpActor();	
	SetActor();
	PaletteAnime();

};



//-------------------------------------------------------------------------
//  ����������BG �ݒ�
//-------------------------------------------------------------------------
// =================================================================
// koukan_bg_set
// �T�v  : ����������BG �ݒ�
// ����  : turn	:	0=���������P���	1=���������Q���
// �߂�l: none
// =================================================================
static void koukan_bg_set(u8 turn)
{
	int i;

	switch(turn){
		case 0:
			for(i=0;i<48;i++){
				PaletteWork[i] = kk_bg_Palette[i];
				PaletteWorkTrans[i] = kk_bg_Palette[i];
			}
			DIV_DMAARRAYCOPY(3 , kk_bg_Character , 	BG_VRAM,16);
#ifdef DEBUG_NUM_PRINT
			CpuClear(0, kw->bcw.bgvram,0x800, 32);
#else
			for(i=0;i<32*32;i++){
				kw->bcw.bgvram[i] = kk_bg2_Map[i];
			}
#endif
			DIV_DMAARRAYCOPY(3 , kk_bg2b_Map , 		BG_VRAM+0x3000,16);
			break;

		case 1:
			DIV_DMAARRAYCOPY(3 , kk_bg3b_Map , 		BG_VRAM+0x3800,16);

			pokelevel_put(0);
			pokelevel_put(1);

			block_map_refresh(&kw->bcw);

			*(vu16 *)REG_BG0CNT &= 0xfffc;						//�e�L�X�g�ʂ̃v���C�I���e�B���ō���

			*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_1					//BG�D�揇��
						| 5 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
			*(vu16 *)REG_BG2CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 6 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
			*(vu16 *)REG_BG3CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| 7 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

			*(vu16*)REG_BG0HOFS = 0;
		    *(vu16*)REG_BG1HOFS = 0;
			*(vu16*)REG_BG2HOFS = 0;
		    *(vu16*)REG_BG3HOFS = 0;

			*(vu16*)REG_BG0VOFS = 0;
		    *(vu16*)REG_BG1VOFS = 0;
			*(vu16*)REG_BG2VOFS = 0;
		    *(vu16*)REG_BG3VOFS = 0;
			break;
	}

}

//-------------------------------------------------------------------------
//  �|�P�����A�C�R���̏�Ԃ������Ȃ���
//-------------------------------------------------------------------------
static void pokemonicon_refresh()
{
	int i;
	for(i=0;i<6;i++){
		if(i<kw->pokenum[0]){			//������
			ActWork[kw->iconno[0][i]].banish=0;		//�߹�ݱ��݂�\��
			kw->existtbl[i] = 1;					//���݃e�[�u�����׸ނ𗧂Ă�
		}else{
			kw->existtbl[i] = 0;					//����ð��ق��׸ނ�������
		}

		if(i<kw->pokenum[1]){			//���葤
			ActWork[kw->iconno[1][i]].banish=0;		//�߹�ݱ��݂�\��
			kw->existtbl[i+6] = 1;                  //���݃e�[�u�����׸ނ𗧂Ă�
		}else{                                      
			kw->existtbl[i+6] = 0;                  //����ð��ق��׸ނ�������
		}
	}
	kw->existtbl[12] = 1;
	
}


#define SOUSIN	(0)
#define JUSIN	(1)

static const u8 souju_msg[][13]={
	{ka_,i_,me_,no_,spc_,so_,u_,si_,n_,EOM_,},
	{ka_,i_,me_,no_,spc_,zi_,yyu_,si_,n_,EOM_,},
{PO_,KE_,MO_,N_,A_,I_,KO_,N_,SE_,TTU_,TO_,EOM_,},
{O__,B__,J__,TE_,KI_,SU_,TO_,SE_,TTU_,TO_,EOM_,},
{SE_,RU_,SE_,TTU_,TO_,EOM_,},
{O__,B__,J__,TE_,KI_,SU_,TO_,A__,D__,D__,EOM_,},
{SI_,SU_,TE_,MU_,ME_,TTU_,SE_,bou_,ZI_,A__,D__,D__,EOM_,},
{ha_,i_,ke_,i_,SE_,TTU_,TO_,EOM_,},	
};

static void soujusin_msg(u8 souju, u8 num)
{

#ifdef DEBUG_PRINT
	u8 strbuf[20];
	PM_NumMsgSet(strbuf,num,NUM_MODE_LEFT,1);
	PM_strcat(strbuf,souju_msg[souju]);
	NMenuMsgWrite(strbuf,1,18);	//����M��ԕ\��

#endif
}

void memcopy(void *des, void *src, u32 size)
{
	int i;
	u8 *d, *s;
	d = (u8*)des;
	s = (u8*)src;
	for(i=0;i<size;i++)	d[i] = s[i];
}

extern u8 SendPoolBuf[];
//==========================================================================
//  �߹�݂��ް���ʐM�Ō�������
//==========================================================================
static u8 ExchangePoke()
{
	u8 id;
	id = GetSioConnectId();
	
	kanshi_num_set(kw->exchangeflag/100,kw->exchangeflag%100);
	

	switch(kw->exchangeflag){
		case 0:												//�����̃|�P�����̏������[�N�ɃZ�b�g
			memcopy(SendPoolBuf,&PokeParaMine[0],200);
			kw->exchangeflag++;
			break;
		case 1:
			if(SplitDataSendStatus()){
				if(SplitDataResult()==0){
					kw->exchangeflag++;
				}else{
					SplitDataResultReset();
					kw->exchangeflag++;
				}
			}
			break;
		case 2:
			if(id==0){										//�������e��������P��ڂ̌����J�n
				SplitDataExchangeStart(1);
			}
			kw->exchangeflag++;
			break;
		case 3:
			if(SplitDataResult()==0x3){						//�����Ƒ���̃f�[�^��M�I���҂�
															//��M��������̃f�[�^�������o��
				memcopy(&PokeParaEnemy[0],&RecvPoolBuf[id^1][0],sizeof(PokemonParam)*2);
				SplitDataResultReset();
				kw->exchangeflag++;
			}
			break;
		case 4:
			memcopy(SendPoolBuf,&PokeParaMine[2],200);
			kw->exchangeflag++;
			break;

		case 5:
			if(id==0){										//�������e��������Q��ڂ̌����J�n
				SplitDataExchangeStart(1);
			}
			kw->exchangeflag++;
			break;
		case 6:
			if(SplitDataResult()==0x3){						//�����Ƒ���̃f�[�^��M�I���҂�
															//��M��������̃f�[�^�������o��
				memcopy(&PokeParaEnemy[2],&RecvPoolBuf[id^1][0],sizeof(PokemonParam)*2);
				SplitDataResultReset();
				kw->exchangeflag++;
			}
			break;


		case 7:
			memcopy(SendPoolBuf,&PokeParaMine[4],200);
			kw->exchangeflag++;
			break;

		case 8:
			if(id==0){										//�������e��������R��ڂ̌����J�n
				SplitDataExchangeStart(1);
			}
			kw->exchangeflag++;
			break;
		case 9:
			if(SplitDataResult()==0x3){						//�����Ƒ���̃f�[�^��M�I���҂�
															//��M��������̃f�[�^�������o��
				memcopy(&PokeParaEnemy[4],&RecvPoolBuf[id^1][0],sizeof(PokemonParam)*2);
				SplitDataResultReset();
				kw->exchangeflag++;
			}
			break;
		case 10:
			memcopy(SendPoolBuf,&Fld.MailData[0],220);		//���[���f�[�^���R�s�[
			kw->exchangeflag++;
			break;
		case 11:
			if(id==0){										//�������e������������X�^�[�g
				SplitDataExchangeStart(3);
			}
			kw->exchangeflag++;
			break;
		case 12:
			if(SplitDataResult()==0x3){						//�����Ƒ���̃f�[�^��M�I���҂�
															//��M��������̃f�[�^�������o��
				memcopy(&EnemyMail[0],&RecvPoolBuf[id^1][0],sizeof(MAIL_DATA)*6);
				SplitDataResultReset();
				kw->exchangeflag++;
			}
			break;

		case 13:
			memcopy(SendPoolBuf,&Fld.ExRibbonNo,EXTRA_RIBBON_MAX);		//���{���f�[�^���R�s�[
			kw->exchangeflag++;
			break;
		case 14:
			if(id==0){										//�������e������������X�^�[�g
				SplitDataExchangeStart(4);
			}
			kw->exchangeflag++;
			break;
		case 15:
			if(SplitDataResult()==0x3){						//�����Ƒ���̃f�[�^��M�I���҂�
															//��M��������̃f�[�^�������o��
				memcopy(kw->RibbonTbl,&RecvPoolBuf[id^1][0],EXTRA_RIBBON_MAX);
				SplitDataResultReset();
				kw->exchangeflag++;
			}
			break;

		case 16:

			return 1;
			break;
	}


	return 0;
}	



//==========================================================================
//  �������񂵂܂����H
//==========================================================================
static void koukan_select_set()
{
	u8 strbuf[25];
//	u8 work[10];
	u8 minename[10];
	u8 enemyname[10];


	PokeParaGet(&PokeParaMine[kw->cursorposno],ID_nickname,strbuf);
	NickNameCopy(minename,strbuf);
	PokeParaGet(&PokeParaEnemy[kw->enemy_select%6],ID_nickname,strbuf);
	NickNameCopy(enemyname,strbuf);
//	PM_strcpy(strbuf,PokeNameData[PokeParaGet(&PokeParaMine[kw->cursorposno],ID_monsno,NULL)]);
//	PM_strcpy(work,PokeNameData[PokeParaGet(&PokeParaEnemy[kw->enemy_select%6],ID_monsno,NULL)]);
	PM_strcpy(strbuf,minename);
	PM_strcat(strbuf,str_to);
	PM_strcat(strbuf,enemyname);
	PM_strcat(strbuf,str_simasuka);
	SetExplainFont32x16(strbuf, (u8*)(OBJ_MODE0_VRAM+(kw->inst_no<<5)),EXPLAIN_CELL);


}


//==========================================================================
//  �ʐM���Ă�Ƃ��̐e���̏���
//==========================================================================
static void parent_func(u8 id, u8 result)
{
	if(result&0x1){			//�����ő��M�����f�[�^
		switch(RecvPoolBuf[id][0]){
			case KOUKAN_END_SELECT:
				kw->wait_flag[0] = SELECT_END;
				break;
			case KOUKAN_WAIT_IN:
														//�����Łu��������ҋ@�ɓ���܂����v
//				kw->my_select = RecvPoolBuf[id][1]+6;	//�����͉���I�������̂��擾
				kw->wait_flag[0] = SELECT_KOUKAN;
				break;
			case KOUKAN_SEND_YES:							//�����͂x�d�r
				kw->ok_flag[0] = KOUKAN_YES;
				break;
			case KOUKAN_SEND_NO:							//�����͂m�n
				kw->ok_flag[0] = KOUKAN_NO;
				break;
		}
		SplitDataResultReset2(0);
	}
	if(result&0x02){		//�q�����M�����f�[�^
		switch(RecvPoolBuf[1][0]){
			case KOUKAN_END_SELECT:
				kw->wait_flag[1] = SELECT_END;
				break;
			case KOUKAN_WAIT_IN:
														//�q�@����u��������ҋ@�ɓ���܂����v
				kw->enemy_select = RecvPoolBuf[1][1]+6;	//�q�@�͉���I�������̂��擾
				kw->wait_flag[1] = SELECT_KOUKAN;
				break;
			case KOUKAN_SEND_YES:							//�q�@�͂x�d�r
				kw->ok_flag[1] = KOUKAN_YES;
				break;
			case KOUKAN_SEND_NO:							//�q�@�͂m�n
				kw->ok_flag[1] = KOUKAN_NO;
				break;
		}
	
		SplitDataResultReset2(1);
	}

}

//==========================================================================
//  �ʐM���Ă�Ƃ��̎q���̏���
//==========================================================================
static void child_func(u8 id, u8 result)
{
	if(result&0x1){
		switch(RecvPoolBuf[0][0]){				
			case KOUKAN_END_OK:					//�����I�����m�F���ꂽ
				PaletteFadeReq(0xffffffff,0,0,16,0);
				system_message_window(4);		//�Ƃ������̂��イ��傤��҂��Ă��܂��B
				kw->menuflag = 11;
				break;
			case KOUKAN_END_STOP:
				system_message_window(5);		//���������ʏI�����L�����Z�����ꂽ
				kw->menuflag = 8;
				break;
			case KOUKAN_NEXT_TERM:				//�����x�d�r�m�n��ʂ�
				kw->enemy_select = RecvPoolBuf[0][1]+6;
				statuspage_set_start(kw->cursorposno);
				statuspage_set_start(kw->enemy_select);
//				koukan_select_set();
				kw->menuflag=7;
				break;
			case KOUKAN_EXCHANGE:				//�������񐬗�
				PaletteFadeReq(0xffffffff,0,0,16,0);
//				LightFadeReq(N_to_B);
				kw->menuflag = 10;
				break;
			case KOUKAN_STOP:					//�������񒆎~
				system_message_window(1);
				kw->menuflag = 8;
				break;
		}
	
		SplitDataResultReset2(0);
	}

	if(result&0x2){
		
		SplitDataResultReset2(1);
	}
}


static const u8 seikou_msg[]={ko_,u_,ka_,n_,se_,i_,ri_,tu_,spc_,spc_,spc_,spc_,spc_,EOM_,};
static const u8 dame_msg[]={da_,me_,da_,ta_,ra_,si_,i_,yo_,spc_,spc_,spc_,spc_,spc_,EOM_,};
//==========================================================================
//  �ʐM���A�哱���������Ă���e���̏���
//==========================================================================
static void koukan_sio_func()
{
	
		if(kw->wait_flag[0]!=0 && kw->wait_flag[1]!=0){
			if(kw->wait_flag[0] == SELECT_KOUKAN && kw->wait_flag[1] == SELECT_KOUKAN){	//�������������񂷂�|�P������I�������B
				//�u�`�@�Ɓ@�a���������񂵂܂����H�v
				//�u�x�d�r�v�u�m�n�v	�\��
				kw->menuflag=6;
				kw->sendwork[0]  = KOUKAN_NEXT_TERM;//���̏����Ɉڂ�悤�Ɏq�@�Ɏw��
				kw->sendwork[1]  = kw->cursorposno;	//���̍ۂɐe�͂ǂ̃|�P������I������������
				QueFuncSet(5,0);					//sendwork��SplitDataSendStart��5sync��ɑ��M
				kw->wait_flag[0] = kw->wait_flag[1] = 0;
			}else if(kw->wait_flag[0] == SELECT_KOUKAN && kw->wait_flag[1] == SELECT_END){
				system_message_window(1);			//�u��������͒��~�v
				kw->sendwork[0] = KOUKAN_END_STOP;	//�I�����~�R�}���h���s
				kw->sendwork[1] = 0;
				QueFuncSet(5,0);					//sendwork��SplitDataSendStart��5sync��ɑ��M
				kw->ok_flag[0]   = kw->ok_flag[1]   = 0;
				kw->wait_flag[0] = kw->wait_flag[1] = 0;
				kw->menuflag = 8;
			}else if(kw->wait_flag[0] == SELECT_END && kw->wait_flag[1] == SELECT_KOUKAN){
				system_message_window(5);			//�I���̓L�����Z��
				kw->sendwork[0] = KOUKAN_STOP;		//�������񒆎~�R�}���h���s
				kw->sendwork[1] = 0;
				QueFuncSet(5,0);					//sendwork��SplitDataSendStart��5sync��ɑ��M
				kw->ok_flag[0]   = kw->ok_flag[1]   = 0;
				kw->wait_flag[0] = kw->wait_flag[1] = 0;
				kw->menuflag = 8;
			}else if(kw->wait_flag[0] == SELECT_END && kw->wait_flag[1] == SELECT_END){
				kw->sendwork[0] = KOUKAN_END_OK;	//���������ʏI�����s
				kw->sendwork[1] = 0;
				QueFuncSet(5,0);					//sendwork��SplitDataSendStart��5sync��ɑ��M
				PaletteFadeReq(0xffffffff,0,0,16,0);
				kw->wait_flag[0] = kw->wait_flag[1] = 0;
				kw->menuflag = 11;
			}
			
		}
		if(kw->ok_flag[0]!=0&&kw->ok_flag[1]!=0){		//�������u�������񂷂邩�H�v�̑I���ɓ�����
			if(kw->ok_flag[0] == KOUKAN_YES && kw->ok_flag[1] == KOUKAN_YES){	//�����x�d�r
				//��������R�}���h���s
				kw->sendwork[0] = KOUKAN_EXCHANGE;
				kw->sendwork[1] = 0;
				QueFuncSet(5,0);						//sendwork��SplitDataSendStart��5sync��ɑ��M
				kw->ok_flag[0]=0;
				kw->ok_flag[1]=0;
				kw->menuflag = 9;
			}
			if(kw->ok_flag[0] == KOUKAN_NO || kw->ok_flag[1] == KOUKAN_NO){	//�ǂ��炩���m�n�ƑI�������B
				system_message_window(1);	//�u��������͒��~�v
														//�������񒆎~�R�}���h���s
				kw->sendwork[0] = KOUKAN_STOP;
				kw->sendwork[1] = 0;
				QueFuncSet(5,0);						//sendwork��SplitDataSendStart��5sync��ɑ��M
				kw->ok_flag[0]=0;kw->ok_flag[1]=0;
				kw->menuflag = 8;
			}
		}

}
//==========================================================================
//  �ʐM�ŃR�}���h�̂���������
//
//  �Z�[�u����ꏊ��ʒm����				�i�e�j
//  �ʐM�ҋ@��Ԃɂ͂���܂����B			�i�q�j
//  �����ɑ΂��āu�͂��v��������܂�		�i�q�j
//  �����ɑ΂��āu�������v�𑗂�܂�		�i�q�E�e�j
//  �����̉��ԂƑ���̉��Ԃ��������܂��B	�i�e�j
//==========================================================================
void KoukanSioFunc()
{

	u8 id,result;
	
	id = GetSioConnectId();

	if((result=SplitDataResult())){			//�f�[�^���󂯂Ƃ����玩���̗��ꂲ�Ƃɏ���
		if(id==0){
			parent_func(id,result);		//�e�̏���
		}else{
			
			child_func(id,result);		//�q�̏���
		}
		SplitDataResultReset();
	}
	
	if(id==0)						//�ʐM�̐e���哱���������Đi�s��i�߂�
		koukan_sio_func();
}

//==========================================================================
//  ���ق̎��̈ړ���͑��݂��Ă��邩���ō�6�܂ŒT��
//==========================================================================
static u8  CursorSearch(u8 pos, u8 arrow)
{
	int i,w;
	w = 0;
	for(i=0;i<6;i++){
		if(kw->existtbl[movepostbl[pos][arrow][i]]==1){
			w = movepostbl[pos][arrow][i];
			break;
		}
	}
	return w;
}


//==========================================================================
//  ���وړ�����
//==========================================================================
static void CursorForward(u8 *pos, u8 arrow)
{
	u8 w;

	w = CursorSearch(*pos,arrow);
	if(w==12){
		ActAnmChg(&ActWork[kw->cursorno],1);
		ActWork[kw->cursorno].x = 25* 8+32;
		ActWork[kw->cursorno].y = 20* 8;
	}else{
		ActAnmChg(&ActWork[kw->cursorno],0);
		ActWork[kw->cursorno].x = ( namepos[w][0]    )* 8+32;
		ActWork[kw->cursorno].y = ( namepos[w][1]    )* 8;
	}
	if(*pos!=w)					//�ړ����Ă����SE�炷
		SePlay(SE_SELECT);

	*pos = w;					//���W�X�V
}


//==========================================================================
//  ��������|�P���������A����̒ʐM�҂���
//==========================================================================
static void koukan_ready()
{
	system_message_window(0);		//�ʐM�ҋ@���I
	kw->menuflag=5;
	if(GetSioConnectId()==1){	//�q�͐e�Ƀf�[�^�𑗐M
		kw->sendwork[0] = KOUKAN_WAIT_IN;
		kw->sendwork[1] = kw->cursorposno;
		SplitDataSendStart(GetAnotherSendTo(),(u8*)kw->sendwork,20);
	}else{						//�e�͎����̃t���O�𗧂Ă�
		kw->wait_flag[0] = 1;
	}


}

//==========================================================================
//  �ʏ펞�߯�ޏ���
//==========================================================================
static void menu0pad_func()
{
	int i;
	if(sys.Repeat & U_KEY){
		CursorForward(&(kw->cursorposno),0);
	}else if(sys.Repeat & D_KEY){
		CursorForward(&(kw->cursorposno),1);
	}else if(sys.Repeat & L_KEY){
		CursorForward(&(kw->cursorposno),2);
	}else if(sys.Repeat & R_KEY){
		CursorForward(&(kw->cursorposno),3);
	}
	if(sys.Trg&A_BUTTON){
		SePlay(SE_SELECT);
		if(kw->cursorposno<6){					//���̗�̎���A���݉���
				NWinFrameWrite(&kw->tw, STATUSMENU_X1,STATUSMENU_Y1,STATUSMENU_X2,STATUSMENU_Y2);
//				NMenuWinBoxWrite(18,14,28,19);
				NMenuFixListWrite(STATUSMENU_X1+2, STATUSMENU_Y1+1, 2, menu);
				NMenuVCursorInit(19, 15, 2, 0);
				kw->menuflag=1;					//���j���[�I����
		}else if(kw->cursorposno<12){			//�E�̗�̎���A���݉���
//				statuspage_set_start(kw->cursorposno);
				PaletteFadeReq(0xffffffff,0,0,16,0);
				kw->menuflag = 2;				//�X�e�[�^�X�\����
		}else if(kw->cursorposno==12){			//�u��߂�v��A���݉���
				NWinFrameWrite(&kw->tw, YESNO_POS_X1,YESNO_POS_Y1,YESNO_POS_X2,YESNO_POS_Y2);
//				NMenuWinBoxWrite(23,14,28,19);
				YesNoSelectStrInit(YESNO_POS_X1,YESNO_POS_Y1);
				kw->menuflag=4;					//�I�����j���[��
				SetExplainFont32x16(kk_msg_tbl[4], (u8*)(OBJ_MODE0_VRAM+(kw->inst_no<<5)),EXPLAIN_CELL);
//				BoxDrawFont_32x16(kk_msg_tbl[4], (u8*)(OBJ_MODE0_VRAM+(kw->inst_no<<5)), DRAWTYPE_16TRANS, 0, UserWork);
		}
	}
	if(sys.Trg&R_BUTTON){
		for(i=0;i<10;i++){
			kw->sendwork[i] = i;
		}
		SplitDataSendStart(GetAnotherSendTo(),(u8*)kw->sendwork, 20);
	}

}

//==========================================================================
//  �I���ƭ�����̉�ʕ��A
//==========================================================================
static void return_endmenu()
{
	menu_erase();
	kw->menuflag = 0;
	ActWork[kw->cursorno].banish = 0;
	SetExplainFont32x16(kk_msg_tbl[1], (u8*)(OBJ_MODE0_VRAM+(kw->inst_no<<5)),EXPLAIN_CELL);
//	BoxDrawFont_32x16(kk_msg_tbl[1], (u8*)(OBJ_MODE0_VRAM+(kw->inst_no<<5)), DRAWTYPE_16TRANS, 0, UserWork);
}

//==========================================================================
//  �u�X�e�[�^�X�v�u��������ɂ����v�I��
//==========================================================================
static void menu1pad_func()
{
	if(sys.Repeat & U_KEY){
		SePlay(SE_SELECT);
		kw->menupos=NMenuVCursorSet(-1);
	}else if(sys.Repeat & D_KEY){
		SePlay(SE_SELECT);
		kw->menupos=NMenuVCursorSet(1);
	}
	if(sys.Trg&A_BUTTON){
		SePlay(SE_SELECT);
		if(NMenuVCursorPosGet()==0){			//�u�X�e�[�^�X��\���v��I��
//			statuspage_set_start(kw->cursorposno);
			PaletteFadeReq(0xffffffff,0,0,16,0);
			kw->menuflag=2;						//�X�e�[�^�X�\��
		}else{
			
			if(StatusSubCheck(kw->pokestatus[0],kw->pokenum[0],kw->cursorposno)==0){
				QueFuncSet(3,2);			//ү���ޕ\��                        //�퓬���ł��Ȃ��Ȃ��Ă��܂�Ȃ����H
				kw->menuflag = 8;
			}else{
				koukan_ready();						//�ʐM�҂��ɂ͂���
				ActWork[kw->cursorno].banish = 1;		//�J�[�\��������
			}
		}
	}else if(sys.Trg&B_BUTTON){					//�I�����L�����Z��
		SePlay(SE_SELECT);
		return_endmenu();
	}

}

static void menu_key_wait()
{
	if(sys.Trg & A_BUTTON || sys.Trg & B_BUTTON){
		SePlay(SE_SELECT);
		return_endmenu();
	}
}

//==========================================================================
//  �X�e�[�^�X��ʕ\����
//==========================================================================
static void menu2pad_func()
{
	if(FadeData.fade_sw == 0){
		if(kw->cursorposno<6){
			PokeStatusCall2(PokeParaMine,kw->cursorposno,
				kw->pokenum[0]-1,KoukanMainReturnInit,BTL_WAZA);
		}else{
			PokeStatusCall2(PokeParaEnemy,kw->cursorposno-6,
				kw->pokenum[1]-1,KoukanMainReturnInit,BTL_WAZA);
		}
	}
//	if(kw->statuspage_flag[0]==5||kw->statuspage_flag[1]==5)	//�A�C�R���ړ����I�����Ă�����
//		if(sys.Trg&B_BUTTON){									
//			SePlay(SE_SELECT);
//			B_button_func();
//		}

}

static u8 AllStatusAdd(u8 *tbl,u8 num)
{
	int i;
	u8 ret = 0;
	for(i=0;i<num;i++) ret += tbl[i];

	return ret;
}

static u8 StatusSubCheck(u8 *tbl, u8 num, u8 pos)
{
	int i;
	u8 ret = 0;
	for(i=0;i<num;i++){
		if(pos!=i)
			ret += tbl[i];
	}
	return ret;
}

// ======================================================================
// SendYesMessageCheck
// �T�v  : �u�͂��v�𑗂邽�߂ɍs���`�F�b�N
//
// �퓬���������Ȃ������͔����Ȃ���΂����Ȃ��B
// �莝�����^�}�S�����ɂȂ��Ă��܂��Ƃ��A�^�}�S�ƕm���̃|�P���������Ƃ�
// ����  : none
// �߂�l: none
// =====================================================================
static void SendYesMessageCheck()
{
	int i;
	u8 work[2][6];
	
	for(i=0;i<kw->pokenum[0];i++) work[0][i] = kw->pokestatus[0][i];	//�����̃f�[�^���W�v
//	work[0][kw->cursorposno] = kw->pokestatus[1][kw->enemy_select%6];	

//	for(i=0;i<kw->pokenum[1];i++) work[1][i] = kw->pokestatus[1][i];	//����̃f�[�^���������Ă݂�
//	work[1][kw->enemy_select%6] = kw->pokestatus[0][kw->cursorposno];		

//	if(AllStatusAdd(work[0],kw->pokenum[0])==0){						//���̌���������Ǝ�����
//		QueFuncSet(3,2);			//ү���ޕ\��                        //�퓬���ł��Ȃ��Ȃ��Ă��܂�Ȃ����H
//		kw->sendwork[0] = KOUKAN_SEND_NO;
//		QueFuncSet(180,0);			//�L�����Z���f�[�^�x���ő��M
		
//	}else if(AllStatusAdd(work[1],kw->pokenum[1])==0){					//���̌���������Ƒ����
//		QueFuncSet(3,3);			//ү���ޕ\��                        //�퓬���ł��Ȃ��Ȃ��Ă��܂�Ȃ����H
//		kw->sendwork[0] = KOUKAN_SEND_NO;
//		QueFuncSet(180,0);			//�L�����Z���f�[�^�x���ő��M
	if(StatusSubCheck(work[0],kw->pokenum[0],kw->cursorposno)==0){
		QueFuncSet(3,2);			//ү���ޕ\��                        //�퓬���ł��Ȃ��Ȃ��Ă��܂�Ȃ����H
		kw->sendwork[0] = KOUKAN_SEND_NO;
		QueFuncSet(180,0);			//�L�����Z���f�[�^�x���ő��M
	}else{																//�S�̃`�F�b�N���ʂ�΂n�j
		QueFuncSet(3,1);			//ү���ޕ\��
		kw->sendwork[0] = KOUKAN_SEND_YES;
		if(SplitDataSendStatus()){										//���M
			SplitDataSendStart(GetAnotherSendTo(),(u8 *)kw->sendwork,20);
		}
	}

}

//==========================================================================
//  �������񂵂܂����HYES�^NO�I��
//==========================================================================
static void menu3pad_func()
{

	if(sys.Repeat & U_KEY){
		SePlay(SE_SELECT);
		kw->menupos=NMenuVCursorSet(-1);
	}else if(sys.Repeat & D_KEY){
		SePlay(SE_SELECT);
		kw->menupos=NMenuVCursorSet(1);
	}
	if(sys.Trg&A_BUTTON){
		
		SePlay(SE_SELECT);
		if(NMenuVCursorPosGet()==0){	//�u�͂��v��I��
			//�͂��𑗂邽�߂̏���
			SendYesMessageCheck();
		}else{							//�u�������v��I��
			QueFuncSet(3,1);			//ү���ޕ\��
			if(SplitDataSendStatus()){
				kw->sendwork[0] = KOUKAN_SEND_NO;
				SplitDataSendStart(GetAnotherSendTo(),(u8 *)kw->sendwork,20);
			}
		}
		kw->menuflag = 100;						//��܂킵���邽�߂̔ԍ�
	}else if(sys.Trg&B_BUTTON){
		QueFuncSet(3,1);			//ү���ޕ\��
		if(NMenuVCursorPosGet()==0){
			kw->menupos=NMenuVCursorSet(1);
		}
		kw->sendwork[0] = KOUKAN_SEND_NO;
		SplitDataSendStart(GetAnotherSendTo(),(u8 *)kw->sendwork,20);
		kw->menuflag = 100;						//��܂킵���邽�߂̔ԍ�
	}
}



//==========================================================================
//  �I�����邩�I��
//==========================================================================
static void menu4pad_func()
{
	if(sys.Repeat & U_KEY){
		SePlay(SE_SELECT);
		kw->menupos=NMenuVCursorSet(-1);
	}else if(sys.Repeat & D_KEY){
		SePlay(SE_SELECT);
		kw->menupos=NMenuVCursorSet(1);
	}
	if(sys.Trg&A_BUTTON){
			SePlay(SE_SELECT);
			if(NMenuVCursorPosGet()==0){	//���������ʏI��
//				PaletteFadeReq(0xffffffff,0,0,16,0);
//				LightFadeReq(N_to_B);
				system_message_window(4);			//�Ƃ������̂��イ��傤��҂��Ă��܂��B
				kw->sendwork[0]  = KOUKAN_END_SELECT;//���̏����Ɉڂ�悤�Ɏq�@�Ɏw��
				kw->sendwork[1]  = 0;				//���̍ۂɐe�͂ǂ̃|�P������I������������
				QueFuncSet(5,0);					//sendwork��SplitDataSendStart��5sync��ɑ��M
				ActWork[kw->cursorno].banish = 1;	//�J�[�\��������

				kw->menuflag = 100;
			}else{
				return_endmenu();
			}
	}else if(sys.Trg&B_BUTTON){
		SePlay(SE_SELECT);
		return_endmenu();
	}

}


//==========================================================================
//  �e�̏ꍇ�A�T�u�y�[�W�̃A�j�����J�n������
//==========================================================================
static void menu6_func()
{
	if(GetSioConnectId()==0){
			statuspage_set_start(kw->cursorposno);
			statuspage_set_start(kw->enemy_select);
	}
	kw->menuflag=7;
}


//==========================================================================
//  �T�u�y�[�W�̏������I��������u�������񂵂܂����H�v�I����
//==========================================================================
static void menu7_func()
{
	if(kw->statuspage_flag[0]==5&&kw->statuspage_flag[1]==5){
		koukan_select_set();
		kw->menuflag=14;
	}
}

//==========================================================================
//  �T�u�y�[�W�̏������I��������u�������񂵂܂����H�v�I����
//==========================================================================
static void menu7b_func()
{
	kw->wait++;
	if(kw->wait>120){
		NWinFrameWrite(&kw->tw, YESNO_POS_X1,YESNO_POS_Y1,YESNO_POS_X2,YESNO_POS_Y2);
		YesNoSelectStrInit(YESNO_POS_X1,YESNO_POS_Y1);
		kw->wait = 0;
		kw->menuflag = 3;
	}
}

//==========================================================================
//  �������񂪃L�����Z�����ꂽ�ꍇ�̃{�^�������҂�
//==========================================================================
static void menu8_func()
{
	if(sys.Trg&A_BUTTON){
		SePlay(SE_SELECT);
		return_statuspage(0);
		return_statuspage(1);
//		for(i=0;i<2;i++){
//			kw->wait_flag[i] = 0;
//			kw->ok_flag[i]   = 0;
//		}
		kw->menuflag=0;
		ActWork[kw->cursorno].banish=0;
	}
}

//==========================================================================
//  ���������ʏI���t�F�[�h�A�E�g�҂�
//==========================================================================
static void endfadeout_func()
{
	if( FadeData.fade_sw == 0 )
	{
		SetSioEndDataStart();
//		PokemonMultiSioEnd();
	//	ContinueFieldScript();
	//	pFieldRecoverFunc = AddFieldInTask;
	//	SetNextWorldPosition();
	//	sys.pMainProc = CommTradeEnd;
		kw->menuflag=12;
	};

}

static void endsio_func()
{
	if(pokemon_sio_flag==0){
//		sys.pFirstProc = PushFirstProc;			//�ʐM�������풓������
		MainProcChange(FieldCommRecover);
	}
}
//==========================================================================
//  �߯�ޏ���
//==========================================================================
static void PadFunc()
{

	switch(kw->menuflag){
		case 0:		menu0pad_func();		break;		//�ʏ펞
		case 1:		menu1pad_func();		break;		//����I����
		case 2:		menu2pad_func();		break;		//�u�X�e�[�^�X�v�I����
		case 3:		menu3pad_func();		break;		//�u��������v�I��
		case 4:		menu4pad_func();		break;		//�u�����v�I����
		case 6:		menu6_func();			break;		//�T�u�y�[�W�̃A�j�����J�n������
		case 7:		menu7_func();			break;		//�T�u�y�[�W�̃A�j���I���҂�
		case 8:		menu8_func();			break;		//��������L�����Z����
		case 9:		fadeout_func();			break;		//�t�F�[�h�A�E�g�J�n
		case 10: 	fadeout_wait_func();	break;		//��������f���J�n
		case 11:	endfadeout_func();		break;		//���������ʏI��
		case 12:	endsio_func();			break;		//�ʐM�I������
		case 13:	multisio_end_wait();	break;		//��������f���p�ʐM�I������
		case 14:	menu7b_func();			break;		//��������̂͂��E�������ɍs���O�ɑ҂�
		case 15:	menu_key_wait();		break;		//�L�[�҂����Ēʏ��ʂɖ߂�
	}

#if 0
	if(sys.Trg&L_BUTTON){
			kw->sendwork[0] = KOUKAN_NEXT_TERM;	//���̏����Ɉڂ�悤�Ɏq�@�Ɏw��
			kw->sendwork[1] = kw->cursorposno;	//���̍ۂɐe�͂ǂ̃|�P������I������������
			if(SplitDataSendStatus())
				SplitDataSendStart(GetAnotherSendTo(),(u8 *)kw->sendwork,20);
			PaletteWorkSet(Test_Palette,F_PAL*16,32);
	}
	for(i=0;i<200;i++)
		multisiotestdata[i] = testdata[count%3][i];
	SplitDataSendStart((0x01<<(GetSioConnectId()^1)),(u8*)multisiotestdata,200);
	count++;
#endif

}


//==========================================================================
//�X�e�[�^�X��ʏ����J�n
//==========================================================================
static void statuspage_set_start(u8 pos)
{
	u8 side;

	side = pos/6;
	if(kw->statuspage_flag[side]==0){
		kw->statuspage_flag[side] = 1;
		kw->statuspage_num[side]  = pos;
	}
}


static const u8 subwindowpos[2][2]={
	{4,3},	{4+15,3}
};


#define MONS_NAME_INFO	((MONS_NAME_SIZE+6)*2)
//#define MONS_INFO_1SET	(4*WAZA_NAME_SIZE*2+MONS_NAME_INFO)
#define MONS_INFO_1SET	(MONS_NAME_INFO*6)

//==========================================================================
//  �X�e�[�^�X��ʕ\��
//==========================================================================
static void statuspage_set(u8 side)
{
	u8 g,p,i,pos;
	s8 len;
	u8 buf[20],buf2[8*4+5];

	pos = kw->statuspage_num[side];
	if(pos<6)	g = 0;
	else		g = 1;
	p = pos%6;
	len = 0;

	switch(kw->statuspage_flag[side]){
		case 1:
			//�I�����������߹�ݱ��݂����ׂĉB��
			for(i=0;i<kw->pokenum[side];i++)
				ActWork[kw->iconno[g][i]].banish = 1;

			ActWork[kw->iconno[g][p]].banish = 0;							//�I�������߹�݂������ݕ\��
			ActWork[kw->iconno[g][p]].work[0] = 20;
			ActWork[kw->iconno[g][p]].work[2] = ((namepos[g*6][0]+namepos[g*6+1][0])/2)*8+14;	//����޳������ɕ\��
			ActWork[kw->iconno[g][p]].work[4] = namepos[g*6][1]*8-12;
			SetWorkSeq(&ActWork[kw->iconno[g][p]], PokeIconDmmyMove);
			kw->statuspage_flag[side]++;
			W_StraightMove2(&ActWork[kw->iconno[g][p]]);
			FontScrnBoxFill( &(kw->tw) , 0, clear_area[g][0], 0, clear_area[g][1], 19);	//���������̈悾��÷�ĸر
			block_map_copy16(&kw->bcw,15*side,0,(u16*)kk_bg2a2_Map,15,17,0x0000);
			if(g==0)
				menu_erase();
			break;

		case 2:
			if(ActWork[kw->iconno[g][p]].move==PokeIconDmmyMove)
				kw->statuspage_flag[side]++;
			break;
		case 3:
			//�w�i����޳��ð���p�ɓ���ւ���(�������E���葤���ʂ�MAP�ύX����)
			block_map_copy16(&kw->bcw,g*15,0,(u16*)kk_bg2a_Map,15,17,0x0000);

			ActWork[kw->iconno[g][p]].x = ((namepos[g*6][0]+namepos[g*6+1][0])/2)*8+14;	//����޳������ɕ\��
			ActWork[kw->iconno[g][p]].y = namepos[g*6][1]*8-12;
			ActWork[kw->iconno[g][p]].dx = 0;
			ActWork[kw->iconno[g][p]].dy = 0;

			
			len = pokemon_info_set(buf,g,p);								//���O�E���ʁE���ٕ�����쐬
			if(len>10) len  =-1;
			else		len = 0;

			MsgPrint(&(kw->tw),buf,kw->windowwork+g*MONS_INFO_1SET,
				statuspos[g*2+0][0]+len,statuspos[g*2+0][1]);	//���O�\��
//			NMenuMsgWrite(buf,statuspos[g*2+0][0],statuspos[g*2+0][1]);	//���O�\��
			
			pokemon_waza_set(buf2,g,p);											//�킴ؽĕ�����쐬
			
			MsgPrint(&(kw->tw),waza_str,kw->windowwork+g*MONS_INFO_1SET+MONS_NAME_INFO,
				statuspos[g*2+1][0],statuspos[g*2+1][1]+1);	//�u�킴�v�\��
			MsgPrint(&(kw->tw),buf2,kw->windowwork+g*MONS_INFO_1SET+MONS_NAME_INFO+6,
				statuspos[g*2+1][0]+3,statuspos[g*2+1][1]+1);	//�킴ؽĕ\��
//			NMenuMsgWrite(waza_str,statuspos[g*2+1][0],statuspos[g*2+1][1]+1);	//�u�킴�v�\��
//			NMenuMsgWrite(buf2,statuspos[g*2+1][0]+3,statuspos[g*2+1][1]+1);	//�킴ؽĕ\��
			kw->statuspage_flag[side]++;
			break;
		case 4:
			SetExplainFont32x16(kk_msg_tbl[5], (u8*)(OBJ_MODE0_VRAM+(kw->inst_no<<5)),EXPLAIN_CELL);
//			BoxDrawFont_32x16(kk_msg_tbl[5], (u8*)(OBJ_MODE0_VRAM+(kw->inst_no<<5)), DRAWTYPE_16TRANS, 0, UserWork);
			level_put(side, p, subwindowpos[side][0]+4,subwindowpos[side][1]+1,subwindowpos[side][0],subwindowpos[side][1]);
			kw->statuspage_flag[side]++;
			break;
	}
}

//==========================================================================
//  �I���|�P������񕶎���쐬
//==========================================================================
static u8 pokemon_info_set(u8 *buf, u8 group, u8 pos)
{
	u8 w,lv,work[MONS_NAME_SIZE+EOM_SIZE];

	if(group==0){				//������
		PokeParaGet(&PokeParaMine[pos],ID_nickname,work);
		NickNameCopy(buf,work);
		w  = PokeSexGet(&PokeParaMine[pos]);
		lv = PokeParaGet(&PokeParaMine[pos],ID_level);
	}else{						//���葤
		PokeParaGet(&PokeParaEnemy[pos],ID_nickname,work);
		NickNameCopy(buf,work);
		w=PokeSexGet(&PokeParaEnemy[pos]);
		lv = PokeParaGet(&PokeParaEnemy[pos],ID_level);
	}
	if(kw->tamago[group][pos%6]==0){
		PM_SexStrcat(buf,w);
	}
	PM_strcat(buf,str_sura);
	if(kw->tamago[group][pos%6]==0){
		PM_strcat(buf,str_lv);
		PM_NumMsgSet(work, lv, NUM_MODE_LEFT, 3);
		PM_strcat(buf,work);
	}else{
		PM_strcat(buf,str_bou);
	}
	return PM_strlen(buf);
}

//==========================================================================
//  �|�P�����̎����Ă�킴���X�g�쐬
//==========================================================================
static void pokemon_waza_set(u8 *buf, u8 group, u8 pos)
{
	u16 waza[4],i;

	if(kw->tamago[group][pos]==0){
		for(i=0;i<4;i++){
			if(group==0){
				waza[i]=PokeParaGet(&PokeParaMine[pos],ID_waza1+i,NULL);
			}else{
				waza[i]=PokeParaGet(&PokeParaEnemy[pos],ID_waza1+i,NULL);
			}
		}
		PM_strcpy(buf,colchg);
		for(i=0;i<4;i++){
			if(waza[i]!=0)
				PM_strcat(buf,WazaNameData[waza[i]]);
			PM_strcat(buf,cr_str);
		}
	}else{
		PM_strcpy(buf,colchg);
		PM_strcat(buf,str_hatena);
	}
}

//==========================================================================
//  �|�P�����̖��O��\���i�ō��U�j
//
//
//  ���͒l�F0=������ 1=���葤
//==========================================================================
static void pokename_put(u8 side)
{
	u8 i,w;
	u8  work[20];
	u8	name[20];
	
	for(i=0;i<kw->pokenum[side];i++){					//�|�P�����A�C�R���\���E�|�P�������O�\��
		if(!side){
			PokeParaGet(&PokeParaMine[i],ID_nickname,work);				
			NickNameCopy(name,work);
			w=PokeSexGet(&PokeParaMine[i]);
		}else{
			PokeParaGet(&PokeParaEnemy[i],ID_nickname,work);				
			NickNameCopy(name,work);
			w=PokeSexGet(&PokeParaEnemy[i]);
		}
		if(kw->tamago[side][i]==0){
			PM_SexStrcat(name,w);
		}
		MsgPrint(&(kw->tw),name,kw->windowwork+(side*MONS_NAME_SIZE*2*6)+i*MONS_NAME_SIZE*2,
			namepos[i+side*6][0],namepos[i+side*6][1]);
	}

}


#define LEVEL_LEFT_CELLNO (6*16)
#define LEVEL_RIGHT_CELLNO (7*16)
//==========================================================================
//  ��̕��̃|�P�����̃��x����\������
//==========================================================================
static void level_put(u8 side, u8 n, u8 lx, u8 ly, u8 wx, u8 wy)
{
	u8 level;


	block_map_copy16(&kw->bcw,wx, wy,(u16*)kk_bg4_Map,6,3,0x0000);
	if(!side)	level=PokeParaGet(&PokeParaMine[n],ID_level,NULL);				
	else		level=PokeParaGet(&PokeParaEnemy[n],ID_level,NULL);				
	
	if(kw->tamago[side][n]==0){				//�^�}�S�̓��x����\�����Ȃ�
		if((level/10)!=0)
			kw->bcw.bgvram[lx+ly*32]   = LEVEL_LEFT_CELLNO+level/10;
		kw->bcw.bgvram[lx+ly*32+1]     = LEVEL_RIGHT_CELLNO + level%10;
	}else{
		kw->bcw.bgvram[lx+ly*32-32]    = kw->bcw.bgvram[lx+ly*32-32-1];			//�uLV�v��ׂ����߂�
		kw->bcw.bgvram[lx+ly*32-32+1]  = kw->bcw.bgvram[lx+ly*32-32-4]|(1<<10);	//����MAP����q��

	}							
}
//==========================================================================
//  �|�P�����̃��x���\��(BG1)
//==========================================================================
static void pokelevel_put(u8 side)
{
	int i,w;
//	block_map_copy16(&kw->bcw,16+7*(i/3),3+5*(i%3),(u16*)kk_bg4_Map,6,3,0x0000);

	for(i=0;i<kw->pokenum[side];i++){
		w = i+side*6;
		level_put(side,i,levelpos[w][0],levelpos[w][1],levelwindowpos[w][0],levelwindowpos[w][1]);
	}


}

//==========================================================================
// �|�P�����A�C�R���̃��X�g�\���p�̍��W��ݒ肷��
//==========================================================================
static void pokeicon_pos_set(u8 side)
{
	int i;
	for(i=0;i<kw->pokenum[side];i++){
		ActWork[kw->iconno[side][i]].banish = 0;
		ActWork[kw->iconno[side][i]].x  = namepos[i+6*side][0]*8+14;
		ActWork[kw->iconno[side][i]].y  = namepos[i+6*side][1]*8-12;
		ActWork[kw->iconno[side][i]].dx = 0;
		ActWork[kw->iconno[side][i]].dy = 0;
	}
}


//==========================================================================
//
//	�u�X�e�[�^�X�E��������ɂ����v�̃��j���[������
//
//==========================================================================
static void menu_erase()
{
	FontScrnBoxFill( &(kw->tw) , 0, clear_area[1][0], 0, clear_area[1][1], 19);	//���������̈悾��÷�ĸر
	pokename_put(1);
}

//==========================================================================
//
//	��������YESNO��ʂ̏���
//
//==========================================================================
static void return_statuspage(u8 side)
{

	if(side==0){
		FontScrnBoxFill( &(kw->tw) , 0, clear_area[0][0], 0, clear_area[0][1], 19);	//���������̈悾��÷�ĸر
		block_map_copy16(&kw->bcw,0,0,(u16*)kk_bg2a2_Map,15,17,0x0000);
		pokelevel_put(0);
		pokename_put(0);
		pokeicon_pos_set(0);
		block_map_refresh(&kw->bcw);
	}else{
		FontScrnBoxFill( &(kw->tw) , 0, clear_area[1][0], 0, clear_area[1][1], 19);	//���������̈悾��÷�ĸر
		block_map_copy16(&kw->bcw,15,0,(u16*)kk_bg2a2_Map,15,17,0x0000);
		pokelevel_put(1);
		pokename_put(1);
		pokeicon_pos_set(1);
		block_map_refresh(&kw->bcw);
	}
	SetExplainFont32x16(kk_msg_tbl[1], (u8*)(OBJ_MODE0_VRAM+(kw->inst_no<<5)),EXPLAIN_CELL);
	kw->statuspage_flag[side] = 0;

}

/*==================================================================*/
/*  ���zVRAM�̓��e��]��											*/
/*==================================================================*/
void block_map_refresh(BLOCK_COPY_WORK *work)
{
	work->transflag=1;
}

/*==================================================================*/
/*  block_map_copy16()��VSync���p�̊֐�             by  AkitoMori   */
/*																	*/
/*==================================================================*/
void block_map_copy16_main(BLOCK_COPY_WORK *work)
{
	if(work->transflag){
		CpuCopy(work->bgvram,work->des,32*32*2,16);
		work->transflag = 0;
	}
}

/*==================================================================*/
/*  block_map_copy �̏�����											*/
/*																	*/
/*																	*/
/*	work:  BLOCK_COPY_WORK�\���̂��߲�����w��						*/
/*	des :  �]������w�肷��											*/
/*==================================================================*/
void block_map_copy_init(BLOCK_COPY_WORK *work, u16* des)
{
	work->transflag = 0;
	work->des = des;
}

/*===================================================================*/
/*  block_map_copy16()                              by  AkitoMori    */
/* 32*20��BGϯ���ް���O��Ƃ�����Ԃɂ���ȉ���MAP�ް���]�����閽��*/
/* ���16�F�p														 */
/*    x:�]�������Ԃւ�ϯ�߈ʒu��x����                              */
/*    y:�]�������Ԃւ�ϯ�߈ʒu��y����                              */
/*  src:�]�������ް�                                                 */
/*    w:src�̉��̑傫��                                              */
/*    h:src�̏c�̑傫��                                              */
/*  mask:�]�����ɂ��̒l�����������i��گĎw�肷�邽�߁j             */
/*  32x20��ϯ�߂ɌŒ�												 */
/*																	 */
/*	���ۂ�VSYNC���ŌĂ΂��֐�block_map_copy16_main�œ]������		 */
/*===================================================================*/
void block_map_copy16(BLOCK_COPY_WORK *work,char x, char y, u16*src, char w,char h,u16 mask)
{
	int m,n;

	for(m=0;m<h;m++){				/*ϯ���ް���ۯ���߰*/
		for(n=0;n<w;n++){
			work->bgvram[y*32+x+m*32+n] = src[w*m+n]|mask;
		}
	}
		
	work->transflag = 1;
}


// =================================================================
// Menustatus
// �T�v  : ��ʏ���[���j���[���ڂ̑I����]
// ����  : none
// �߂�l: none
// =================================================================
static void Menustatus()
{
	NMenuScreenClear();
}

// =================================================================
// Menukoukan
// �T�v  : ��ʏ���[���j���[���ڂ̑I����]
// ����  : none
// �߂�l: none
// =================================================================
static void Menukoukan()
{
	NMenuScreenClear();
}


/*==================================================================*/
/*  ��ʒ����ɕ\������V�X�e���n�̃��b�Z�[�W						*/
/*==================================================================*/
static void system_message_window(u8 mes)
{

	NWinFrameWrite(&kw->tw, 
		sysmsg_tbl[mes][0],sysmsg_tbl[mes][1],sysmsg_tbl[mes][2],sysmsg_tbl[mes][3]);
	MsgPrint(&(kw->tw),system_msg[mes],kw->windowwork+(MONS_NAME_INFO*6)*2,
		sysmsg_tbl[mes][0]+1,sysmsg_tbl[mes][1]+1);		
	
}

//==========================================================================
//  �K���Ȃp�t�d�����Z�b�g���[�`��
//==========================================================================
static void QueFuncSet(u16 wait, u8 funcno)
{
	int i;
	for(i=0;i<QUE_MAX;i++){
		if(kw->que[i].flg==0){
			kw->que[i].wait = wait;
			kw->que[i].funcno = funcno;
			kw->que[i].flg=1;
			break;
		}
	}
}

//==========================================================================
//  �K���Ȃp�t�d�������C�����[�`��
//==========================================================================
static void QueFuncMain()
{
	int i;
	for(i=0;i<QUE_MAX;i++){
		if(kw->que[i].flg){
			if(kw->que[i].wait!=0){
				kw->que[i].wait--;
			}else{
				switch(kw->que[i].funcno){
					case 0:
						SplitDataSendStart(GetAnotherSendTo(),(u8 *)kw->sendwork,20);
						break;
					case 1:
						system_message_window(0);		//�u������ҋ@���I�v
						break;
					case 2:
						system_message_window(2);		//�u���̂�������������Ȃ��Ɓc�v
						break;
					case 3:
						system_message_window(3);		//�u�����Ă͐퓬�ł��Ȃ��Ȃ����Ⴄ��v
						break;
					case 4:
						system_message_window(3);		//�u�����Ă͐퓬�ł��Ȃ��Ȃ����Ⴄ��v
						break;
					case 5:
						system_message_window(3);		//�u�����Ă͐퓬�ł��Ȃ��Ȃ����Ⴄ��v
						break;
				}
				kw->que[i].flg=0;
			}
		}
	}
}



// =================================================================
// RenzokuCellSet
// �T�v  : �L������������Ƃ���VRAM�ɓ]������
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 RenzokuCellSet()
{
	switch(kw->wait){
		case 0:	CellSet((CellData*)&NameActCell[0]); kw->wait++;		break;
		case 1:	CellSet((CellData*)&NameActCell[1]); kw->wait++;		break;
		case 2:	CellSet((CellData*)&NameActCell[2]); kw->wait++;		break;
		case 3:	CellSet((CellData*)&NameActCell[3]); kw->wait++;		break;
		case 4:	CellSet((CellData*)&NameActCell[4]); kw->wait++;		break;
		case 5:	CellSet((CellData*)&NameActCell[5]); kw->wait++;		break;
		case 6:	CellSet((CellData*)&NameActCell[6]); kw->wait++;		break;
		case 7:
			kw->inst_no = CellSet((CellData *)&NameActCell[7]);
			kw->wait++;
			break;
		case 8:	CellSet((CellData*)&NameActCell[8]); kw->wait++;		break;
		case 9:	CellSet((CellData*)&NameActCell[9]); kw->wait++;		break;
		case 10:CellSet((CellData*)&NameActCell[10]); kw->wait++;	break;
		case 11:CellSet((CellData*)&NameActCell[11]); kw->wait++;	break;
		case 12:CellSet((CellData*)&NameActCell[12]); kw->wait++;	break;
		case 13:ObjPalSet((PalData *)&NameActPal);	 kw->wait++;		break;
		case 14:ObjPalSet((PalData *)&CursorActPal); kw->wait++;		break;
		case 15:CellSet((CellData*)&CursorActCell);  kw->wait++;	break;
		case 16:
			kw->wait = 0;
			return 1;
			break;
	}

	return 0;
}


// ======================================================================
// SetExplainFont32x16
// �T�v  : BoxDrawFont_32x16�����������ʂ̐������͂ɗ��p���邽�߂̊֐�
// ����  : str �\�����镶����
//       : dst �Z���ʒu���߲��
//       : max  ������̍ő啶����
// �߂�l: none
// ======================================================================
static void SetExplainFont32x16(const u8 *str, u8 *dst, u8 max)
{
	int i;
	u8 STrBuf[(LCD_WIDTH/8)+1];
	
	PM_strclear(STrBuf,spc_,max);
	for(i=0;i<max;i++){
		if(str[i]!=EOM_) STrBuf[i] = str[i];
		else			break;
	}
	BoxDrawFont_32x16(STrBuf, dst, DRAWTYPE_16TRANS, 0, UserWork);
}



// ==================================================================
// PokeStatusGet
// �T�v  : �|�P�������m����������^�}�S�������肵�Ȃ������`�F�b�N
// ����  : none
// �߂�l: kw->pokestatus[2][6]�ɐ��킾������P�A�ُ킾������O���i�[
// ==================================================================
static void PokeStatusGet(u8 no)
{
	int i;

	switch(no){
		case 0:											//�������̃|�P�����𒲍�
			for(i=0;i<kw->pokenum[no];i++){
				if(PokeParaGet(&PokeParaMine[i],ID_tamago_flag)==1){	//���܂���
					kw->pokestatus[no][i] = 0;
					kw->tamago[no][i] = 1;
					continue;
				}else if(PokeParaGet(&PokeParaMine[i],ID_hp)==0){		//�m����
					kw->pokestatus[no][i] = 0;
					kw->tamago[no][i] = 0;
				}else{
					kw->pokestatus[no][i] = 1;
					kw->tamago[no][i] = 0;
				}
			}
			break;
		case 1:
			for(i=0;i<kw->pokenum[no];i++){				//���葤�̃|�P�����𒲍�
				if(PokeParaGet(&PokeParaEnemy[i],ID_tamago_flag)==1){	//���܂���
					kw->pokestatus[no][i] = 0;
					kw->tamago[no][i] = 1;
					continue;
				}else if(PokeParaGet(&PokeParaEnemy[i],ID_hp)==0){		//�m����
					kw->pokestatus[no][i] = 0;
					kw->tamago[no][i] = 0;
				}else{
					kw->pokestatus[no][i] = 1;
					kw->tamago[no][i] = 0;
				}
			}
			break;
	}
}


// =================================================================
// PokeIconActionSearch
// �T�v  : �|�P�����A�C�R���̓��������邽�߂ɏ�������
// ����  : no
// �߂�l: kw->iconaction[2][6]�ɓ����̒l������
// =================================================================
static void PokeIconActionSearch(u8 no)
{
	u16 life,max,i;
	switch(no){
		case 0:											//�������̃|�P�����𒲍�
			for(i=0;i<kw->pokenum[0];i++){
				life = PokeParaGet(&PokeParaMine[i],ID_hp);
				max  = PokeParaGet(&PokeParaMine[i],ID_hpmax);
				kw->iconaction[0][i] = GetGaugeDottoColor(life,max);
			}
			break;
		case 1:
			for(i=0;i<kw->pokenum[1];i++){
				life = PokeParaGet(&PokeParaEnemy[i],ID_hp);
				max  = PokeParaGet(&PokeParaEnemy[i],ID_hpmax);
				kw->iconaction[1][i] = GetGaugeDottoColor(life,max);
			}
			break;
	}
	
}

// =================================================================
// PokeIconActionSet
// �T�v  : �|�P�����A�C�R���̓�����Z�b�g����
// ����  : none
// �߂�l: none
// =================================================================
static void PokeIconActionSet()
{
	int i,w;
	for(w=0;w<2;w++)
		for(i=0;i<kw->pokenum[w];i++){
			ExtAnmChg(&ActWork[kw->iconno[w][i]],4 - kw->iconaction[w][i]);
		}
}

// =================================================================
// RibbonInfoCheck
// �T�v  : ���{�����̃`�F�b�N�E���L
// ����  : none
// �߂�l: none
// =================================================================
static void RibbonInfoCheck()
{
	int i;
	for(i=0;i<EXTRA_RIBBON_MAX;i++){
		if(Fld.ExRibbonNo[i] == 0){							//���������̃��{�����BOX�����
			if(kw->RibbonTbl[i] != 0){						//�����BOX�ɂ͏�񂪂�������
				Fld.ExRibbonNo[i] = kw->RibbonTbl[i];		//���炤
			}	
		}
	}
}
