//=========================================================================
//	�|�P������������f��
//
//    Copyright (C) 2000 GAME FREAK inc.
//											By Akito Mori
//=========================================================================

//KoukanWork�̃A�h���X��UserWork���犄��t���Ă܂���
//�A�h���X�w��̐������K���ł��B����\������̂ŋC�����悤

#define SIO_INTR_ON
//#define SIO_DEBUG_PRINT

#include "common.h"
#include "evobj.h"
#include "mus_tool.h"

#include "calctool.h"
#include "bss.h"
#include "poketool.h"
#include "client_t.h"
#include "multisio.h"
#include "pokesio.h"
#include "cable.h"

#include "intr.h"

#include "saveload.h"
#include "decord.h"
#include "objdata.h"

#include "print.h"
#include "fcontext.h"
#include "window.h"
#include "menu.h"
#include "menutool.h"
#include "boxsub.h"
#include "sysflag.h"
#include "ev_flag.h"
#include "box2.h"

#include "task.h"
#include "actor.h"
#include "actanm.h"
#include "Madefine.h"
#include "palanm.h"
#include "param.h"
#include "record.h"

#include "pokeicon.h"
#include "poketool.h"
#include "wazatool.h"
#include "zukan.h"

#include "fld_main.h"
#include "fld_task.h"
#include "script.h"
#include "client.h"
#include "cli_ef.h"
#include "shinka.h"
#include "mystatus.h"

#include "..\scaddata\ballani.acl"
#include "..\scaddata\ballani.ach"

#include "..\scaddata\koukan0.acl"
#include "..\scaddata\koukanb1.ach"
#include "..\scaddata\koukanb2.ach"
#include "..\scaddata\koukan0.asc"
#include "..\scaddata\koukan1.asc"
#include "..\scaddata\koukan2.asc"
#include "..\scaddata\koukan3.asc"
#include "..\scaddata\koukan_o.acl"
#include "..\scaddata\koukan_o.ach"
#include "..\scaddata\koukanb3.ach"
#include "..\scaddata\koukan4.asc"

#include "svld_ini.h"
#include "svld_sys.h"

#include "k_demo.h"
#include "kdemoact.h"
#include "..\data\itemsym.h"
#include "koukan.def"						//��������C�x���g�p�e�[�u��



#define TYPE_MINE		(0)
#define TYPE_ENEMY		(1)
#define PROCESS1		(0)
#define PROCESS2		(1)

#define FIRST_NATUKIDO  (70)				//�������ꂽ�|�P�����ɓ����Ȃ��x

//=========================================================================
//	��`
//=========================================================================
//#define name value


static const u8 endwait_msg[]={
	si_,yyu_,u_,ri_,yyo_,u_,ma_,ti_,de_,su_,spc_,spc_,spc_,EOM_,
};


static const u8 sousin1_msg[]={
	I_MSG_,STR_TEMP_BUF0,ni_,spc_,I_MSG_,STR_TEMP_BUF1,wo_,spc_,o_,ku_,ri_,ma_,su_,gyoe_,EOM_,
};

static const u8 sousin2_msg[]={
	ba_,i_,ba_,i_,spc_,I_MSG_,STR_TEMP_BUF1,gyoe_,EOM_,
};

static const u8 uketori1_msg[]={
	I_MSG_,STR_TEMP_BUF0,ka_,ra_,spc_,I_MSG_,STR_TEMP_BUF2,ga_,spc_,o_,ku_,ra_,re_,te_,ki_,ta_,gyoe_,EOM_,
};

static const u8 uketori2_msg[]={
	I_MSG_,STR_TEMP_BUF2,wo_,spc_,ka_,wa_,i_,ga_,ttu_,te_,spc_,a_,ge_,te_,ne_,gyoe_,EOM_,
};

extern const u8 report_writing_msg[];

extern const u8 TuushinTaikiMsg[];

extern const u32 DecordAdrs[];
extern const u8  PokeNameData[][6];

extern u8 SoftResetDisable;

extern void KoukanMainInit();
extern void DebugNumPrintInit(u8 pal, u8 select, u8 screen, u8 chara);


void KoukanDemoInit();
void KoukanDemoMain();
void KoukanDemoMainOffline();
void KoukanDemoSaveSeq();

void KoukanDemoEnd();
static void SioFunc();

//static u8 DemoPokemonKoukan();
static u8 DemoPokemonKoukan2();
static void CellPalSet();
static void KoukanDemoBGSet(u8 no);
static void StrTempSet();
static void InitFadeWait( u8 id );
static void KoukanEventEndNameSet();
static void RibbonCheck();
//static void IncTrCardKoukanCount(void);
static void KoukanMail2Mail(MAIL_DATA *p, KOUKAN_POKEMON *kp);

void KoukanDemoEndWait();

//=========================================================================
//	�ϐ�
//=========================================================================
//extern type
//-------------------- extern�ϐ�  ---------------------------------
extern u8 exchange_pokemon[2];						//���������ʂł��݂��I�������|�P������ۑ�
extern MAIL_DATA	EnemyMail[];					//����̃|�P�����̃��[���f�[�^

//GLOBAL type valiable;
/*-------------------- �O���[�o�� �ϐ�  ----------------------------*/

//LOCAL type valiable;
/*-------------------- ���[�J�� �ϐ� -------------------------------*/

KOUKANDEMO_WORK *kdw;

static void RotateBgRegSet()
{
	BgAffineDestData Dest;

	BgAffineCalcSet(&Dest,kdw->cen2x*0x100,kdw->cen2y*0x100,
								kdw->bg2x,kdw->bg2y,kdw->ratiobg2,kdw->ratiobg2,kdw->theta);

	*(vu16*)REG_BG2PA	 = Dest.H_DiffX;			//��]BG�ʃ��W�X�^�i�[
	*(vu16*)REG_BG2PB	 = Dest.V_DiffX;
	*(vu16*)REG_BG2PC	 = Dest.H_DiffY;
	*(vu16*)REG_BG2PD	 = Dest.V_DiffY;
	*(vu32*)REG_BG2X 	 = Dest.StartX ;
	*(vu32*)REG_BG2Y 	 = Dest.StartY ;

}

// =================================================================
// BgRegSet
// �T�v  : �u�V���N���肱�ݒ��ɂa�f�̕\���I�t�Z�b�g���W�X�^��ύX
// ����  : none
// �߂�l: none
// =================================================================
static void BgRegSet(void)
{
	u16 work;
	*(vu16*)REG_BG1VOFS = kdw->bg1v;
	*(vu16*)REG_BG1HOFS = kdw->bg1h;

	work = *(u16*)REG_DISPCNT;
	if((work&DISP_BG_MODE_MASK)==0){					//BG2���e�L�X�g�ʂ̎��Ɗg��k���ʂ̎��Ɋi�[���@�𕪂���
		*(vu16*)REG_BG2VOFS = kdw->bg2v;
		*(vu16*)REG_BG2HOFS = kdw->bg2h;
	}else{
		RotateBgRegSet();
	}
}

/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static void VInterFunc( void )
{
	BgRegSet();
	
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}



enum{
	KDEMO_INIT=0,
	KDEMO_SIO_INIT,
	KDEMO_SIO_START_WAIT,
	KDEMO_SIO_START,
	KDEMO_SIO_WAIT,
	KDEMO_INIT_GRAPHIC1,
	KDEMO_INIT_GRAPHIC2,
	KDEMO_INIT_GRAPHIC3,
	KDEMO_INIT_GRAPHIC4,
	KDEMO_INIT_DEBUG_PRINT,
	KDEMO_INIT_FADEIN_START,
	KDEMO_INIT_FADEIN,
	KDEMO_INIT_END,
};

static void watch_dog_init()
{
	kdw->exchangecount = 0;
	kdw->exchangeflag = 0;
	kdw->exchangeold  = 0;
}
static void watch_dog()
{
	if(kdw->exchangeflag==kdw->exchangeold){
		kdw->exchangecount++;
	}else{
		kdw->exchangecount=0;
	}
	if(kdw->exchangecount>60*3){
		kdw->exchangecount=0;
		kdw->exchangeold = 0;
		kdw->exchangeflag = 0;
	}

	kdw->exchangeold = kdw->exchangeflag;

}

// =================================================================
// GetMyId
// �T�v  : �ʐM�̎��ƃI�t���C���̎��悤�ɂh�c��Ԃ�
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 GetMyId(void)
{
	if(pokemon_sio_flag){
		return GetSioConnectId();
	}
	return 0;
}


extern void kanshi_network(u8 taskno);





// =================================================================
// PokemonActorSetProcess
// �T�v  : �|�P�����̃A�N�^�[��o�^����i�����E����j
//         Decord�̏d�����l����2��ĂԎ��ŃA�N�^�[�o�^���I������
//
// ����  : type		TYPE_MINE(����)��TYPE_ENEMY(����)
//       : process	PROCESS1,PROCESS2(���̏��ɌĂ�)
// �߂�l: none
// =================================================================
static void PokemonActorSetProcess( u8 type , u8 process)
{
	u16 monsno,clienttype;
	u32 personal_rnd;
	PokemonParam *pmp;
	PalData *pd;

	clienttype = 0;
	pmp = NULL;
	if(type == TYPE_MINE){					//������
		pmp = &PokeParaMine[exchange_pokemon[0]];
		clienttype = BSS_CLIENT_ENEMY;
	}
	if (type == TYPE_ENEMY){				//���葤
		pmp = &PokeParaEnemy[exchange_pokemon[1]%6];
		clienttype = BSS_CLIENT_ENEMY2;
	}

	switch(process){
		case PROCESS1:				//�P�p�X��
			monsno   = PokeParaGet(pmp,ID_monsno_egg);		//�߹�݉摜�ԍ��擾
			personal_rnd = PokeParaGet(pmp,ID_personal_rnd);
			DecordPokeCellMakeBuchi(&PM2FObjDataCell[monsno],
				pm2fpara[monsno].size,pm2fpara[monsno].patcnt,&UserWork[0],(void*)DecordAdrs[type*2+1],monsno,personal_rnd);
			DecordPalSet((PalData*)PokePalDataGet(pmp));
			kdw->monsno[type] = monsno;
			break;
		case PROCESS2:				//2�p�X��
			pd = (PalData*)PokePalDataGet(pmp);
			PokeActHeaderMake(pd->reg_num,clienttype);					//�|�P�����A�N�^�[��`����
			kdw->actno[type]=AddActor(&PokeActHeader,120,60,6);			//�|�P�������A�N�^�[�o�^
			ActWork[kdw->actno[type]].banish = 1;						//�B��
			ActWork[kdw->actno[type]].move   = DummyActMove;			//�~�߂�
			break;
	}
}


//------------------------------------------------------------------
//  ��������f���������֐�
//------------------------------------------------------------------
void KoukanDemoInit()
{

	switch(sys.InitSeqWork){
		case KDEMO_INIT:
			*(vu16*)REG_DISPCNT = 0;	//��ʂn�e�e
			InitTask();
			PokemonMultiSioEnd();
			kdw = (KOUKANDEMO_WORK*)&UserWork[0x1f000];
			InitActor();				//�A�N�^�[������
			ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������

			SetVBlankFunc(VInterFunc);

			watch_dog_init();

			PrintScreenInit(KOUKAN_TYPE);
			MsgControlInit(&(kdw->tw), KOUKAN_TYPE);
			kdw->windowwork = NWinFrameInit(2);
			NWinFrameDataSet(&(kdw->tw));
			NMenuScreenClear();

			PokemonSioNinshouNo = NINSHOU_KOUKANDEMO;
			sys.InitSeqWork++;
			//�w�i�f�[�^�Z�b�g
			DecordVram((void *)batt_bg1_pch_ADRS,(void *)BG_VRAM);
			DecordWram((void *)batt_bg1_psc_ADRS,(void *)UserWork);
			DIV_DMACOPY(3,UserWork,BG_VRAM+5*0x800,0x500,16);
			DecordPaletteWork((void *)batt_bg1_pcl_ADRS,PA_BG0,0x20);

			kdw->endwait  = 0;
			kdw->demoflag = 0;
			kdw->online   = 1;
			kdw->cen2x = 64;	kdw->cen2y = 64;
			kdw->bgx   = 0;		kdw->bgy   = 0;
			kdw->bg2x  = 120;	kdw->bg2y  = 80;
			kdw->ratiobg2 = 0x100;
			kdw->theta = 0;
			break;

		case KDEMO_SIO_INIT:
			PokemonMultiSioInit();
			sys.InitSeqWork++;
			kdw->wait = 0;
			break;
		case KDEMO_SIO_START_WAIT:
			if(++kdw->wait>60){
				kdw->wait = 0;
				sys.InitSeqWork++;
			}
			break;
		case KDEMO_SIO_START:
			if(GetMyConnectIsParent()){			//�e��������ʐM�X�^�[�g�̏d�ׂ�
				if(GetSioRealConnectNumber()>=GetSioReliableNumber()){		//��l�ڑ������Ȃ����ǂ�
					if(++kdw->wait>30){
//					if(sys.Trg&A_BUTTON){
						MultiSioSignalDecision();
						sys.InitSeqWork++;
					}
				}
			}else{								//�q�͋C�y�ɒʐM�X�^�[�g�҂�
				sys.InitSeqWork++;
			}
			
			break;
		case KDEMO_SIO_WAIT:
			watch_dog();
			if(pokemon_sio_flag==1&&PokemonNinshouNoCheck()==1){
				sys.InitSeqWork++;
			}
			break;
		case KDEMO_INIT_GRAPHIC1:				//�����̃|�P�����p�̃O���t�B�b�N�W�J
			kdw->end_ok[0] = 0;
			kdw->end_ok[1] = 0;
			kdw->sousin    = 0;
			PokemonActorSetProcess( TYPE_MINE, PROCESS1);

			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_GRAPHIC2:				//�����̃|�P�����A�N�^�[�o�^
			PokemonActorSetProcess( TYPE_MINE, PROCESS2);
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_GRAPHIC3:
			PokemonActorSetProcess( TYPE_ENEMY, PROCESS1);
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_GRAPHIC4:
			PokemonActorSetProcess( TYPE_ENEMY, PROCESS2);
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_DEBUG_PRINT:
			CellPalSet();
			CellSet((CellData *)&BallActCell);	//�Z���Z�b�g
			ObjPalSet((PalData *)&BallActPal);	//�p���b�g�Z�b�g
			
			*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 5 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
//			CpuClear(0,BG_VRAM+0x800*5,0x800,16);

#ifdef SIO_DEBUG_PRINT
			*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 5 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
			CpuClear(0,BG_VRAM+0x800*5,0x800,16);
			DebugNumPrintInit(0,1,5,0);
			AddTask(kanshi_network,0);
#endif
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_FADEIN_START:
			PaletteFadeReq(0xffffffff, 0, 16, 0, 0);
//			LightFadeReq(B_to_N);
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_FADEIN:
			KoukanDemoBGSet(5);						//�e�L�X�g��ʐݒ�
			KoukanDemoBGSet(0);						//�X�|�b�g���C�g�w�i�ݒ�
			StrTempSet();

//			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
//    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
//					  | DISP_OBJ_ON
//					  | DISP_BG0_ON					// BG0�L��
//					  | DISP_BG1_ON					// BG0�L��
#ifdef SIO_DEBUG_PRINT
//					  | DISP_BG1_ON					// BG0�L��
#endif
//					;
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_END:
			if(FadeData.fade_sw == 0){
				MainProcChange(KoukanDemoMain);
			}
			break;
	}

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}



// =================================================================
// KoukanDemoOfflineInit
// �T�v  : ��������f���ʐM����Ȃ��Ƃ��ŏ�����
// ����  : none
// �߂�l: none
// =================================================================
void KoukanDemoOfflineInit()
{
	u8	strbuf[11];

	switch(sys.InitSeqWork){
		case KDEMO_INIT:
			exchange_pokemon[0] = ScriptParameter1;
			exchange_pokemon[1] = 6;
//			PokeParaSet(&PokeParaEnemy[0],110,10,POW_RND);

			PM_strcpy(SioTrainerInfo[0].name,MyData.my_name);
			PokeParaGet(&PokeParaEnemy[0],ID_oyaname,strbuf);
			PM_strcpy(SioTrainerInfo[1].name,strbuf);


			*(vu16*)REG_DISPCNT = 0;	//��ʂn�e�e
			InitTask();
			kdw = (KOUKANDEMO_WORK*)&UserWork[0x1f000];
			InitActor();				//�A�N�^�[������
			ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������

			SetVBlankFunc(VInterFunc);

			watch_dog_init();

			PrintScreenInit(KOUKANDEMO_TYPE);
			MsgControlInit(&(kdw->tw), KOUKANDEMO_TYPE);
			kdw->windowwork = NWinFrameInit(2);
			NWinFrameDataSet(&(kdw->tw));
			NMenuScreenClear();

			PokemonSioNinshouNo = NINSHOU_KOUKANDEMO;
			//�w�i�f�[�^�Z�b�g

			kdw->online   = 0;		//�ʐM�����t���O�Z�b�g
			kdw->endwait  = 0;
			kdw->demoflag = 0;
			kdw->cen2x = 64;	kdw->cen2y = 64;
			kdw->bgx   = 0;		kdw->bgy   = 0;
			kdw->bg2x  = 120;	kdw->bg2y  = 80;
			kdw->ratiobg2 = 0x100;
			kdw->theta = 0;
			kdw->wait = 0;
			sys.InitSeqWork = KDEMO_INIT_GRAPHIC1;
			break;

		case KDEMO_INIT_GRAPHIC1:
			PokemonActorSetProcess(TYPE_MINE, PROCESS1);

			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_GRAPHIC2:
			PokemonActorSetProcess(TYPE_MINE, PROCESS2);

			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_GRAPHIC3:
			PokemonActorSetProcess(TYPE_ENEMY, PROCESS1);
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_GRAPHIC4:
			PokemonActorSetProcess(TYPE_ENEMY, PROCESS2);

			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_DEBUG_PRINT:
			CellPalSet();
			CellSet((CellData *)&BallActCell);	//�����X�^�[�{�[���̃Z���Z�b�g
			ObjPalSet((PalData *)&BallActPal);	//�����X�^�[�{�[���̃p���b�g�Z�b�g
			
			*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 5 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_FADEIN_START:
			PaletteFadeReq(0xffffffff, 0, 16, 0, 0);
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_FADEIN:
			KoukanDemoBGSet(5);						//�e�L�X�g��ʐݒ�
			KoukanDemoBGSet(0);						//�X�|�b�g���C�g�w�i�ݒ�
			StrTempSet();
			
			sys.InitSeqWork++;
			break;
		case KDEMO_INIT_END:
			if(FadeData.fade_sw == 0){
				MainProcChange(KoukanDemoMainOffline);
			}
			break;
	}

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
}


// =================================================================
// ZukanFlagSet
// �T�v  : �}�Ӄt���O���Ă�
// ����  : none
// �߂�l: none
// =================================================================
static void ZukanFlagSet(u8 pocketno)
{
	u16 pokenum,work;
																
	if(PokeParaGet(&PokeParaMine[pocketno],ID_tamago_flag)==0){		//�^�}�S����Ȃ�
		pokenum = PokeParaGet(&PokeParaMine[pocketno],ID_monsno,NULL);
		work = PokeMonsNo2ZukanNoGet(pokenum);						//�J���i���o�[����V�K�}�Ӄi���o�[��
		ZukanCheck(work,Z_SEE_SET);									//�G���J�E���g�����`�F�b�N
		ZukanCheck(work,Z_GET_SET);									//�Q�b�g�����`�F�b�N
	}
}

// =================================================================
// 	ZenkokuZukanCodeCheck
// �T�v  : �S���}�Ӄt���O�`�F�b�N
// ����  : none
// �߂�l: none
// =================================================================
static void ZukanAllOpenCodeCheck(void)
{
	u8 id;
	id = GetSioConnectId();		//�ʐMID
	if(SioTrainerInfo[id^1].ExtraCode == ZENKOKUZUKAN_OPEN_CODE){		//�S���}�ӃI�[�v���R�[�h�����邩
		ZukanSpecialFlagSet();											//�S���}��open
	}
}

//======================================================================
//  �|�P�����������
//
//  ����ւ�����E�}�Ӄt���O���Ă���E���[�����ړ�������
//======================================================================

static void PokemonKoukan(u8 p1, u8 p2)
{
//	MAIL_DATA Mail;
	u16 mailid1,mailid2;
	u8  natukido;
	
	mailid1 = PokeParaGet(&PokeParaMine[p1],ID_mailid);
	mailid2 = PokeParaGet(&PokeParaEnemy[p2],ID_mailid);
	

	if(mailid1!=0xff)
		MailDataInit(&Fld.MailData[mailid1]);						//�����̃��[�N�̃��[���폜

	kdw->PokeWork     = PokeParaMine[p1];			//����̃|�P�����Ɠ���ւ�
	PokeParaMine[p1]  = PokeParaEnemy[p2];
	PokeParaEnemy[p2] = kdw->PokeWork;

	natukido = FIRST_NATUKIDO;

	if(PokeParaGet(&PokeParaMine[p1],ID_tamago_flag)==0){		//�^�}�S����Ȃ�
		PokeParaPut(&PokeParaMine[p1],ID_friend,&natukido);	//�Ȃ��x��������Ԃ�(70)
	}

	if(mailid2!=0xff)
		PokeAddWrittenMail(&PokeParaMine[p1], &EnemyMail[mailid2]);	//���[�����֘A�t�����ăR�s�[


	ZukanFlagSet(p1);

	if(pokemon_sio_flag)				//�ʐM���Ȃ�
		ZukanAllOpenCodeCheck();		//�S���}�Ӄt���O�I�[�v���`�F�b�N

}


//==================================================================
//  ���������
//==================================================================
static void SousinOkFlag()
{
	switch(kdw->sousin){
		case 1:
			if(SplitDataSendStatus()){
				SplitDataSendStart(GetAnotherSendTo(),(u8*)kdw->sendwork,20);
				kdw->sousin++;
			}
		case 2:
			kdw->sousin = 0;
			break;	
	}

}

//------------------------------------------------------------------
//  ��������f�����C��2
//------------------------------------------------------------------
void KoukanDemoMainOffline()
{
	DemoPokemonKoukan2();					//��������f���̎��s{

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();

}


#define BG1_SCR_NUM 5
#define BG2_SCR_NUM (16+2)

// =================================================================
// KoukanDemoBGSet
// �T�v  : ��������f���p�̔w�i���Z�b�g
// ����  : no
// �߂�l: none
// =================================================================
static void KoukanDemoBGSet(u8 no)
{
	int i;
	u16 *p;
	switch(no){
		// -----------����|�P�����o��V�[�� -------------------
		case 0:
			kdw->bg2v = 0;
			kdw->bg2h = 180;
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG0_ON					// BG0�L��
					  | DISP_BG1_ON					// BG1�L��
					  | DISP_BG2_ON					// BG1�L��
						;
			*(vu16 *)REG_BG2CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| BG2_SCR_NUM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 1 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
			PaletteWorkSet(koukan0_Palette,16,5*32);
			DIV_DMAARRAYCOPY(3, koukanb1_Character, BG_VRAM+0x4000, 16);
			DIV_DMAARRAYCOPY(3, koukan0_Map, BG_VRAM+0x800*BG2_SCR_NUM, 16);
			break;
		// -----------�f�a�`��������������V�[�� -------------------
		case 1:
			kdw->bg1h = 0;
			kdw->bg1v = 348;
			*(vu16*)REG_BG1VOFS = 348;
			*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_2				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| BG1_SCR_NUM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT 		//�L�����N�^�x�[�X�u���b�N
						;
			DIV_DMAARRAYCOPY(3, koukan1_Map, BG_VRAM+0x800*BG1_SCR_NUM, 16);
			DIV_DMAARRAYCOPY(3, koukanb1_Character, BG_VRAM, 16);

			*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG1_ON					// BG1�L��
//					  | DISP_BG2_ON					// BG1�L��
						;
			break;

		// -----------�|�P����������Ⴄ�V�[�� -------------------
		case 2:
			kdw->bg1v = 0;
			kdw->bg1h = 0;
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG1_ON					// BG1�L��
						;
			DIV_DMAARRAYCOPY(3, koukan2_Map, BG_VRAM+0x800*BG1_SCR_NUM, 16);
			break;

		// ----------- �P�[�u���̉��ɑ傫�������X�^�[�{�[���̔w�i�����Â������яオ�� -------------------
		case 3:
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG1_ON					// BG1�L��
						;
			kdw->cen2x = 64; kdw->cen2y = 64;
			kdw->bg2x  = 120; kdw->bg2y = -70;
			kdw->theta = 0;
			DIV_DMAARRAYCOPY(3, koukanb2_Character, BG_VRAM+0x4000, 16);
			DIV_DMAARRAYCOPY(3, koukan3_Map, BG_VRAM+0x800*BG2_SCR_NUM, 16);
			break;
		// ----------- �g��k������f�a�`�V�[�� -------------------
		case 4:
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
//					  | DISP_BG0_ON					// BG0�L��
//					  | DISP_BG1_ON					// BG1�L��
					  | DISP_BG2_ON					// BG2�L��
						;
			*(vu16 *)REG_BG2CNT = BG_COLOR_256					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| BG2_SCR_NUM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 1 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

			kdw->cen2x = 64;kdw->cen2y = 8*11+4;
			kdw->ratiobg2  = 0x20;
			kdw->ratiowork = 0x400;
			kdw->theta = 0;
			DIV_DMAARRAYCOPY(3, koukanb3_Character, BG_VRAM+0x4000, 16);
			DIV_DMAARRAYCOPY(3, koukan4_Map, BG_VRAM+0x800*BG2_SCR_NUM, 16);
			break;
		
		// ----------- �퓬��ʂƓ����e�L�X�g�E�C���h�E -------------------
		case 5:
			kdw->bg1v = 0;
			kdw->bg1h = 0;
			*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 5 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
			DecordVram((void *)batt_bg1_pch_ADRS,(void *)BG_VRAM);
			DecordWram((void *)batt_bg1_psc_ADRS,(void *)UserWork);

			DecordPaletteWork((void *)batt_bg1_pcl_ADRS,PA_BG0+0x70,0x20);
			PaletteWorkClear(0x0000,0, 2);

			p = (u16*)UserWork;
			for(i=0;i<0x280;i++) p[i] |= 0x7000;
			DIV_DMACOPY(3,UserWork,BG_VRAM+5*0x800,0x500,16);
			
			NMenuBoxClear(CL_WINMSG_X, CL_WINMSG_Y, CL_WINMSG_X+25, CL_WINMSG_Y+3);
			break;

		// ----------- �g��k������f�a�`�V�[�� (�߂�)-------------------
		case 6:
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
//					  | DISP_BG0_ON					// BG0�L��
//					  | DISP_BG1_ON					// BG1�L��
					  | DISP_BG2_ON					// BG2�L��
						;
			*(vu16 *)REG_BG2CNT = BG_COLOR_256					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| BG2_SCR_NUM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 1 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

			kdw->cen2x = 64;kdw->cen2y = 8*11+4;
			kdw->ratiobg2  = 0x100;
			kdw->ratiowork = 0x80;
			kdw->bg2x = 120; kdw->bg2y = 80;
			kdw->theta = 0;
			DIV_DMAARRAYCOPY(3, koukanb3_Character, BG_VRAM+0x4000, 16);
			DIV_DMAARRAYCOPY(3, koukan4_Map, BG_VRAM+0x800*BG2_SCR_NUM, 16);
			break;

		// ----------- �󂯎�����|�P�������o�� -------------------
		case 7:
			kdw->bg2v = 0;
			kdw->bg2h = 0;
			*(vu16 *)REG_BG2CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| BG2_SCR_NUM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 1 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
			PaletteWorkSet(koukan0_Palette,16,5*32);
			DIV_DMAARRAYCOPY(3, koukanb1_Character, BG_VRAM+0x4000, 16);
			DIV_DMAARRAYCOPY(3, koukan0_Map, BG_VRAM+0x800*BG2_SCR_NUM, 16);
			break;

	}
}

// =================================================================
// MonsBallMove
// �T�v  : �����X�^�[�{�[���̔w�i�𓮂���
// ����  : no	0:���Ɉړ�		1:��Ɉړ�
// �߂�l: none
// =================================================================
static void MonsBallBackMove(u8 no)
{
	if(no==0){
		if(kdw->bg1v<348-32-50){
			kdw->bg2y++;
			kdw->theta+=0x40;
		}
	}else{
		if(kdw->bg2y>-64){
			kdw->bg2y--;
			kdw->theta+=0x40;
		}
	}
}

// =================================================================
// CellPalSet
// �T�v  : �A�N�^�[�p�̑O����
// ����  : none
// �߂�l: none
// =================================================================
static void CellPalSet()
{
	CellSet(&Light1ActCell);
	CellSet(&Light2ActCell);
	CellSet(&ConnectActCell);
	CellSet(&MonitorActCell);
	ObjPalSet(&Light1ActPal);
	ObjPalSet(&GBAPartsActPal);

}

// =================================================================
// StrTempSet
// �T�v  : �e�L�X�g�����p�̕�����i�[
// ����  : none
// �߂�l: none
// =================================================================
static void StrTempSet()
{
	u8 strbuf[20],id;
	KOUKAN_POKEMON *kp;

	if(kdw->online){							//�ʐM�̎�
		id = GetSioConnectId();
		PM_strcpy(StrTempBuffer0,SioTrainerInfo[id^1].name);
		PokeParaGet(&PokeParaEnemy[exchange_pokemon[1]%6],ID_nickname,strbuf);
//		PM_strcpy(StrTempBuffer2,strbuf);						//����g���[�i�[�̃|�P�����̖��O
		NickNameCopy(StrTempBuffer2,strbuf);						//����g���[�i�[�̃|�P�����̖��O
		PokeParaGet(&PokeParaMine[exchange_pokemon[0]],ID_nickname,strbuf);
//		PM_strcpy(StrTempBuffer1,strbuf);						//�莝���̃|�P�����̖��O
		NickNameCopy(StrTempBuffer1,strbuf);						//����g���[�i�[�̃|�P�����̖��O
	}else{								//�C�x���g�����̎�
		kp = (KOUKAN_POKEMON*)&EventKoukanPokemon[ScriptParameter0];
		PM_strcpy(StrTempBuffer0,kp->oyaname);					//����g���[�i�[�̖��O
//		PM_strcpy(StrTempBuffer2,kp->nickname);					//����g���[�i�[�̃|�P�����̖��O
		NickNameCopy(StrTempBuffer2,kp->nickname);						//����g���[�i�[�̃|�P�����̖��O
		PokeParaGet(&PokeParaMine[ScriptParameter1],ID_nickname,strbuf);
//		PM_strcpy(StrTempBuffer1,strbuf);						//�莝���̃|�P�����̖��O
		NickNameCopy(StrTempBuffer1,strbuf);						//�莝���̃|�P�����̖��O
	}
}


// =================================================================
// PokemonKoukanDemo
// �T�v  : �|�P���������f��
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 DemoPokemonKoukan2()
{

	u16 aftermons;

	switch(kdw->demoflag)
	{
		case 0:
			ActWork[kdw->actno[0]].banish = 0;						//�\����
			ActWork[kdw->actno[0]].dx     = -180;
			ActWork[kdw->actno[0]].dy     = pm2fpara[kdw->monsno[0]].patcnt;
			kdw->demoflag++;
			kdw->nowsong = BGM_GetNowNo();
			MusicPlay(MUS_SHINKA);
			break;
		case 1:										//�����瑗��|�P�����o��
			if(kdw->bg2h>0){
				ActWork[kdw->actno[0]].dx+=3;
				kdw->bg2h-=3;
			}else{
				ActWork[kdw->actno[0]].dx = 0;
				kdw->bg2h = 0;
				kdw->demoflag=10;
			}
			break;
		case 10:									//�u�΂��΂��@�������I�v
			PM_MsgExpand(MsgExpandBuffer, sousin1_msg);
//			TalkMsgSet(&kdw->tw, MsgExpandBuffer, kdw->windowwork, CL_WINMSG_X, CL_WINMSG_Y);
			MsgPrint( &kdw->tw, MsgExpandBuffer, kdw->windowwork, CL_WINMSG_X, CL_WINMSG_Y);
			kdw->demoflag = 11;
			kdw->wait = 0;
			break;
		case 11:									//���b�Z�[�W�\���҂�
//			if(TalkMsgPut(&kdw->tw)){
			if(++kdw->wait == 80){
				kdw->ballno[0]=MonsterBallReturnEffSet(kdw->actno[0],ActWork[kdw->actno[0]].oamData.Pltt,
					120,80-48,2,1,20,0x0000fffff);
				kdw->demoflag++;
				FontScrnAreaClear(&kdw->tw, 0, 0, 29, 19);
				PM_MsgExpand(MsgExpandBuffer, sousin2_msg);
//				TalkMsgSet(&kdw->tw, MsgExpandBuffer, kdw->windowwork, CL_WINMSG_X, CL_WINMSG_Y);
				MsgPrint( &kdw->tw, MsgExpandBuffer, kdw->windowwork, CL_WINMSG_X, CL_WINMSG_Y);
			}
			break;
		case 12:									//�����X�^�[�{�[���i�[�G�t�F�N�g
			if(ActWork[kdw->ballno[0]].move == DummyActMove && TalkMsgPut(&kdw->tw)==1){
				kdw->ballno[1] = AddActor(&BallHeader,120,80-48,0);
				ActWork[kdw->ballno[1]].move = BallActMove2;
				DelActor(&ActWork[kdw->ballno[0]]);
				kdw->demoflag++;
			}
			break;									//�o�E���h���ĉ�ʊO�ɏ�����҂�
		case 14:
				PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
				kdw->demoflag = 20;
			break;
		case 20:
			if(FadeData.fade_sw == 0){
				KoukanDemoBGSet(4);					//�w�i�؂�ւ�
				kdw->demoflag++;
			}
			break;
		case 21:
			PaletteFadeReq(0xffffffff, -1, 16, 0, 0);
			kdw->demoflag++;
			break;
		case 22:
			if(FadeData.fade_sw == 0){
				kdw->demoflag = 23;
			}
			break;
		case 23:
			if(kdw->ratiowork>0x100){
				kdw->ratiowork -=52;
			}else{
				KoukanDemoBGSet(1);
				kdw->ratiowork = 0x80;
				kdw->demoflag++;
				kdw->wait = 0;
			}				
			kdw->ratiobg2 = 0x8000/kdw->ratiowork;
			break;
		case 24:
			if(++kdw->wait>20){					//�A�h�o���X�̉�ʃ`�J�`�J
				KoukanDemoBGSet(3);				//��]���郂���X�^�[�{�[���̔w�i�Z�b�g
				RotateBgRegSet();
				kdw->actno[3] = AddActor(&MonitorHeader,120,80,0);
				kdw->demoflag++;
			}
			break;
		case 25:								//�f�a�`���X�N���[���ʂƍ����ւ�
			if(ActWork[kdw->actno[3]].anmend_sw==1){
				DelActor(&ActWork[kdw->actno[3]]);
				*(vu16 *)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_BG1_2ND | BLD_BG2_2ND; //BLD_OBJ_1ST | 
				*(vu16 *)REG_BLDALPHA = 12 | (4<<8);
				kdw->demoflag++;
			}
			break;
		case 26:								//�R�l�N�^�`�J�`�J
			if(--kdw->bg1v==348-32){
				kdw->demoflag++;
			}
			if(kdw->bg1v==348-20){
				kdw->actno[4] = AddActor(&ConnectHeader,128,65,0);
			}
			break;
		case 27:								//������
			kdw->actno[2] = AddActor(&Light1Header,128,80,3);
			kdw->actno[3] = AddActor(&Light2Header,128,80,0);
			ActAnmChg(&ActWork[kdw->actno[3]],1);
				kdw->demoflag++;
			break;
		case 28:
			kdw->bg1v-=2;
			if(kdw->bg1v==348-32-150){
				kdw->demoflag = 200;
			}
			MonsBallBackMove(0);
				*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG1_ON					// BG1�L��
					  | DISP_BG2_ON					// BG2�L��
						;
			break;
		case 200:
			ActWork[kdw->actno[2]].y -= 2;
			ActWork[kdw->actno[3]].y -= 2;
			MonsBallBackMove(0);
			if(ActWork[kdw->actno[2]].y < -8){
				kdw->demoflag = 29;
			}
			break;
		case 29:								//�t�F�[�h�A�E�g
			PaletteFadeReq(0xffffffff, -1, 0, 16, 0);
			kdw->demoflag = 30;
			break;
		case 30:								
			if(FadeData.fade_sw == 0){			//�g���ʃZ�b�g
				DelActor(&ActWork[kdw->actno[2]]);
				DelActor(&ActWork[kdw->actno[3]]);
				KoukanDemoBGSet(2);
				kdw->demoflag++;
			}
			break;
		case 31:								//����Ⴄ���o��
			PaletteFadeReq(0xffffffff, -1, 16, 0, 0);
			kdw->actno[2] = AddActor(&Light2Header,111,170,0);
			kdw->actno[3] = AddActor(&Light2Header,129,-10,0);
			kdw->demoflag++;
			break;
		case 32:							
			if(FadeData.fade_sw == 0){
				SePlay(SE_TK_WARPOUT);
				kdw->demoflag++;
			}
			ActWork[kdw->actno[2]].dy -=3;
			ActWork[kdw->actno[3]].dy +=3;
			break;
		case 33:								//����Ⴄ�u��
			ActWork[kdw->actno[2]].dy -=3;
			ActWork[kdw->actno[3]].dy +=3;

			if(ActWork[kdw->actno[2]].dy<=-90){
				ActWork[kdw->actno[2]].work[1] = 1;
				ActWork[kdw->actno[3]].work[1] = 1;
				kdw->demoflag++;
			}
			break;
		case 34:								//��ʃt���b�V��
			ColorConceChange(0x00000001, 16, 0xffff);			
				kdw->demoflag++;
			break;
		case 35:
			ColorConceChange(0x00000001, 0, 0xffff);			
				kdw->demoflag++;
			break;
		case 36:
			ColorConceChange(0x00000001, 16, 0xffff);			
				kdw->demoflag++;
			break;
		case 37:								//�|�P����������Ⴄ
			if(!PokeReverseCheck(kdw->monsno[0])){						//�������̃|�P�����͋������Ȃ�
				ActWork[kdw->actno[0]].aff_tbl = PokePutAffTbl;			//���E���]����i������Ȃ��̂͗Ⴆ�΃A���m�[���j
				ActWork[kdw->actno[0]].oamData.AffineMode = 3;
				actCenterCalc( &ActWork[kdw->actno[0]], 0, 3, 3 );
				ActAffAnmChg(&ActWork[kdw->actno[0]],0);
			}else{
				ActAffAnmChg(&ActWork[kdw->actno[0]],0);
			}
			ActAffAnmChg(&ActWork[kdw->actno[1]],0);
			ActWork[kdw->actno[0]].x = 60;
			ActWork[kdw->actno[1]].x = 120+60;
			ActWork[kdw->actno[0]].y = 160+32;
			ActWork[kdw->actno[1]].y = -32;
			ActWork[kdw->actno[0]].banish = 0;
			ActWork[kdw->actno[1]].banish = 0;
			kdw->demoflag++;
			break;
		case 38:								//�܂����������o��
			ActWork[kdw->actno[0]].dy -= 3;
			ActWork[kdw->actno[1]].dy += 3;
			if(ActWork[kdw->actno[0]].dy >= -163 && ActWork[kdw->actno[0]].dy < -160)
				SePlay(SE_TK_WARPIN);
				
			if(ActWork[kdw->actno[0]].dy <-222){
				ActWork[kdw->actno[2]].work[1] = 0;
				ActWork[kdw->actno[3]].work[1] = 0;
				kdw->demoflag++;
				ActWork[kdw->actno[0]].banish =1;
				ActWork[kdw->actno[1]].banish =1;
				ColorConceChange(0x00000001, 0, 0xffff);			
			}
			break;
		case 39:
			ActWork[kdw->actno[2]].dy -=3;
			ActWork[kdw->actno[3]].dy +=3;
			if(ActWork[kdw->actno[2]].dy<=-222){
				PaletteFadeReq(0xffffffff, -1, 0, 16, 0);
				kdw->demoflag++;
				DelActor(&ActWork[kdw->actno[2]]);
				DelActor(&ActWork[kdw->actno[3]]);
			}
			break;
		case 40:								//�����̂f�a�`�ɑ���̌�������Ă����ʃZ�b�g
			if(FadeData.fade_sw == 0){
				kdw->demoflag++;
				KoukanDemoBGSet(1);
				kdw->bg1v = 348-32-150;
				kdw->actno[2] = AddActor(&Light1Header,128,-20,3);
				kdw->actno[3] = AddActor(&Light2Header,128,-20,0);
				ActAnmChg(&ActWork[kdw->actno[3]],1);
			}
			break;
		case 41:
				PaletteFadeReq(0xffffffff, -1, 16, 0, 0);
				kdw->demoflag++;
			break;
		case 42:
				*(vu16 *)REG_DISPCNT = 	DISP_MODE_1					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG1_ON					// BG1�L��
					  | DISP_BG2_ON					// BG1�L��
					;
			MonsBallBackMove(1);
			if(FadeData.fade_sw == 0) kdw->demoflag++;
			
			break;
		case 43:								//�����^���܂ō~��Ă���
			MonsBallBackMove(1);
			ActWork[kdw->actno[2]].dy +=3;
			ActWork[kdw->actno[3]].dy +=3;
			if((ActWork[kdw->actno[2]].dy+ActWork[kdw->actno[2]].y)==64){
				kdw->demoflag++;
			}
			break;
		case 44:								//�a�f���X�N���[�����Ăf�a�`�������Ă���
			MonsBallBackMove(1);
			kdw->bg1v+=2;
			if(kdw->bg1v>348-32){
				kdw->bg1v = 348-32;
				kdw->demoflag++;
			}
			break;
		case 45:								//�R�l�N�^���`�J�`�J
			DelActor(&ActWork[kdw->actno[2]]);
			DelActor(&ActWork[kdw->actno[3]]);
//			kdw->actno[4] = AddActor(&ConnectHeader,128,65,0);
			kdw->demoflag++;
			kdw->wait = 0;
			break;
		case 46:
			if(++kdw->wait==10){
				kdw->demoflag++;
			}
			break;
		case 47:
			if(++kdw->bg1v>348){
				kdw->bg1v = 348;
				kdw->demoflag++;
			}
			if(kdw->bg1v==348-20)
				kdw->actno[4] = AddActor(&ConnectHeader,128,65,0);
				ActWork[kdw->actno[4]].move = ConnectActMove2;
			break;
		case 48:								//���j�^�[���`�J�`�J
			kdw->actno[3] = AddActor(&MonitorHeader,120,80,0);
			kdw->demoflag = 50;
			break;
		case 50:
			if(ActWork[kdw->actno[3]].anmend_sw==1){
				DelActor(&ActWork[kdw->actno[3]]);
				KoukanDemoBGSet(6);
				kdw->demoflag++;
				SePlay(SE_W028);
			}
			break;
		case 51:							
			if(kdw->ratiowork<0x400){
				kdw->ratiowork +=52;
			}else{
				kdw->ratiowork = 0x400;
				kdw->demoflag++;
			}				
			kdw->ratiobg2 = 0x8000/kdw->ratiowork;
			break;
		case 52:
			PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
			kdw->demoflag = 60;
			break;
		case 60:
			if(FadeData.fade_sw == 0){
				KoukanDemoBGSet(5);
				KoukanDemoBGSet(7);
				kdw->demoflag++;
			}
			break;
		case 61:
			PaletteFadeReq(0xffffffff, 0, 16, 0, 0);
			kdw->demoflag++;
			break;
		case 62:
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
//					  | DISP_BG0_ON					// BG0�L��
//					  | DISP_BG1_ON					// BG1�L��
					  | DISP_BG2_ON					// BG1�L��
						;
			if(FadeData.fade_sw == 0) kdw->demoflag++;
			break;
		case 63:
			kdw->ballno[1] = AddActor(&BallHeader,120,-8,0);
			ActWork[kdw->ballno[1]].work[3] = 74;
			ActWork[kdw->ballno[1]].move = BallActMove4;
			ActAnmChg(&ActWork[kdw->ballno[1]],1);
			ActAffAnmChg(&ActWork[kdw->ballno[1]],2);
			ColorConceChange(1<<(ActWork[kdw->ballno[1]].oamData.Pltt+16), 16, 0xffff);
			kdw->demoflag++;
			kdw->wait = 0;
			break;
		case 64:
			PaletteFadeReq(1<<(ActWork[kdw->ballno[1]].oamData.Pltt+16), 1, 16, 0, 0xffff);
			kdw->demoflag++;
			break;
		
		case 65:
			if(ActWork[kdw->ballno[1]].move==DummyActMove){
				kdw->demoflag++;
			}
			break;
		case 66:
				ActWork[kdw->actno[1]].x  = 120;
				ActWork[kdw->actno[1]].y  = 60+pm2fpara[kdw->monsno[1]].patcnt;
				ActWork[kdw->actno[1]].dx = 0;
				ActWork[kdw->actno[1]].dy = 0;
//				ActWork[kdw->actno[1]].dy = pm2fpara[kdw->monsno[1]].patcnt;
				MonsterBallAppearEffSet(kdw->actno[1],ActWork[kdw->actno[1]].oamData.Pltt,
					120,84,2,1,20,0x0000fffff);
				AffineDelActor(&ActWork[kdw->ballno[1]]);
				DelActor(&ActWork[kdw->ballno[1]]);
				kdw->demoflag++;
			break;
		case 67:
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG0_ON					// BG0�L��
					  | DISP_BG1_ON					// BG1�L��
					  | DISP_BG2_ON					// BG1�L��
						;
			FontScrnAreaClear(&kdw->tw, 0, 0, 29, 19);
			PM_MsgExpand(MsgExpandBuffer, uketori1_msg);
//			NMenuMsgWrite(MsgExpandBuffer, CL_WINMSG_X, CL_WINMSG_Y);
			MsgPrint( &kdw->tw, MsgExpandBuffer, kdw->windowwork, CL_WINMSG_X, CL_WINMSG_Y);
			kdw->demoflag++;
			kdw->wait=0;
			break;
		case 68:
			kdw->wait++;
			if(kdw->wait==4){
				JinglePlay( MUS_FANFA5 );
			}
			if(kdw->wait==240){
				kdw->demoflag++;
				FontScrnAreaClear(&kdw->tw, 0, 0, 29, 19);
//				NMenuBoxClear(CL_WINMSG_X, CL_WINMSG_Y, CL_WINMSG_X+25, CL_WINMSG_Y+3);
				PM_MsgExpand(MsgExpandBuffer, uketori2_msg);
//				NMenuMsgWrite(MsgExpandBuffer, CL_WINMSG_X, CL_WINMSG_Y);
				MsgPrint( &kdw->tw, MsgExpandBuffer, kdw->windowwork, CL_WINMSG_X, CL_WINMSG_Y);
				kdw->wait = 0;
			}	
			break;
		case 69:
			if(++kdw->wait==60){
				kdw->demoflag++;
			}
			break;
		case 70:
			RibbonCheck();									//���炤�|�P�����̃��{���`�F�b�N
			kdw->demoflag++;
			break;
		case 71:
			//�ʐM�̂Ƃ��ƕ���
			if(kdw->online){
				return 1;
			}else{
				if(sys.Trg&A_BUTTON){
					kdw->demoflag++;
				}
			}
			break;
		case 72:		//�C�x���g��������̂�
			PokemonKoukan(ScriptParameter1,0);
			ShinkaReturnProc = KoukanDemoMainOffline;
			if((aftermons=PokeShinkaCheck(&PokeParaMine[exchange_pokemon[0]],TUUSHIN_SHINKA,0))){			//�ʐM(�A�C�e��)�i���`�F�b�N
				InitShinkaTuushin(&PokeParaMine[exchange_pokemon[0]],aftermons,kdw->actno[TYPE_ENEMY],exchange_pokemon[0]);
			}
			kdw->demoflag++;
			break;
		case 73:
				PaletteFadeReq(0xffffffff, 0, 0, 16, 0x0000);
					
//				PokeParaMine[ScriptParameter1]  = PokeParaEnemy[0];		//�C�x���g�p��������
//				exchange_pokemon[0] = ScriptParameter1;
//				ZukanFlagSet();
			kdw->demoflag++;
			break;
		case 74:
			if(FadeData.fade_sw == 0){
				MusicPlay(kdw->nowsong);
				MainProcChange(FieldMainRecover);
				KoukanEventEndNameSet();
			}
			break;
	}
	return 0;
}

// =================================================================
// KoukanPreSaveSeq
// �T�v  : �|�P�����������ăZ�[�u���鏈��(�͂��Ȃ����ƂɂȂ�܂��� 2002/09/23 by Mori)
// ����  : none
// �߂�l: none
// =================================================================
void KoukanPreSaveSeq()
{
	u16 aftermons;

	switch(sys.InitSeqWork){
		case 0:
//			PushPokemonFunc(exchange_pokemon[0]);		//�|�P�����ޔ�
//			sys.InitSeqWork=99;
//			kdw->actno[2] = AddActor(&Light2Header,240-20,136,0);
//			kdw->wait = 0;
			sys.InitSeqWork = 4;
			SoftResetDisable = 1;					//�\�t�g���Z�b�g�����𖳌���
			break;
		case 4:
			ShinkaReturnProc = KoukanDemoSaveSeq;
			if((aftermons=PokeShinkaCheck(&PokeParaMine[exchange_pokemon[0]],TUUSHIN_SHINKA,0))){			//�ʐM(�A�C�e��)�i���`�F�b�N
				InitShinkaTuushin(&PokeParaMine[exchange_pokemon[0]],aftermons,kdw->actno[TYPE_ENEMY],exchange_pokemon[0]);
			}else{
				MainProcChange(KoukanDemoSaveSeq);				//�������i�����Ȃ������炽���̃Z�[�u
			}
			exchange_pokemon[0] = 0xff;
			break;
	}
	if(!MultiSioErrorCheck()){		//�G���[�������̓^�X�N�����s�����Ȃ�
		MainTask();
	}	
	JumpActor();
	SetActor();
	PaletteAnime();
	
}

//------------------------------------------------------------------
//  �ʐM����
//------------------------------------------------------------------
static void SioFunc()
{
	u8 Id,result;

	Id=GetMyId();
	result = SplitDataResult();
	if(result&0x1){					//�e�@����̎�M
		if(RecvPoolBuf[0][0]==0xDCBA){			//�e�@����Z�[�u�J�n�R�[�h�����炤
			MainProcChange(KoukanPreSaveSeq);
		}
		if(RecvPoolBuf[0][0]==0xABCD){			//�e�@����̑S�̏I����҂�
				kdw->end_ok[0]=1;
		}
		SplitDataResultReset2(0);
	}
	if(result&0x2){					//�q�@����̎�M
		if(RecvPoolBuf[1][0]==0xABCD){		//�q�@�̏I���ҋ@�R�[�h���m�F���炤
			kdw->end_ok[1]=1;
		}
		
		SplitDataResultReset2(1);
	}

}


#define BOUND_X		120					//�{�[���̎~�܂�ʒu
#define BOUND_Y		76

// =================================================================
// BallActMove
// �T�v  : �o�E���h���Ē����Ɏ~�܂�{�[���̃A�N�^�[
// ����  : act
// �߂�l: none
// work[0]:�����X�s�[�h(Y)
// work[1]:�w�����ړ��X�s�[�h
// work[2]:�����W��(Y)
// work[3]:�o�E���h��(Y)
// work[4]:�d��(Y)
// work[5]:X
// =================================================================
static void BallActMove(actWork *act)
{
	act->y += act->work[0]/10;
	act->work[5] += act->work[1];
	act->x = act->work[5]/10;	//�����X�ꌅ�Ōv�Z���Ă�̂�10�Ŋ���
	if(act->y>BOUND_Y){			//�ڒn�E����
		act->y = BOUND_Y;
		act->work[0] = -1*(act->work[0]*act->work[2])/100;
		act->work[3]++;
	}
	if(act->x==BOUND_X) act->work[1]=0;
	
	act->work[0] += act->work[4];
	if(act->work[3]==4){		//�S��o�E���h������~�܂�
		act->work[7] = 1;
		act->move = DummyActMove;
	}
}

static const s8 BallBoundTbl[]={
	 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4,	//21
	-4,-4,-4,-3,-3,-3,-3,-2,-2,-2,-2,-1,-1,-1,-1, 0,-1, 0,-1, 0, 0, 0,	//43
	 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4,	//65
	-4,-3,-3,-2,-2,-1,-1,-1, 0,-1, 0, 0, 0,	//78
	 0, 0, 0, 1, 0, 1, 1, 1, 2, 2, 3, 3, 4,	//91
	-4,-3,-2,-1,-1,-1, 0, 0,	//99
	 0, 0, 1, 0, 1, 1, 2, 3,	//107
	 
};

static void BallActMove2(actWork *act)
{
	u8 work;
	act->dy += BallBoundTbl[act->work[0]];
	
	if(act->work[0]==22){	//�o�E���h���鎞�u�R���v�Ɩ�
		SePlay(SE_KON);		
	}
	if(++act->work[0]==44){
		SePlay(SE_W025);
		act->move = BallActMove3;
		act->work[0] = 0;
		work = act->oamData.Pltt;
		PaletteFadeReq(1<<(work+16),-1,0,16,0xffff);
	}
}


static void BallActMove3(actWork *act)
{
	if(act->work[1]==20) ActAffAnmChg(act,1);
	if(++act->work[1]>20){
		act->dy -= BallBoundTbl[act->work[0]];
		
		if(++act->work[0]==23){
			DelActor(act);
			kdw->demoflag=14;
		}
	}
	
}

static void BallActMove4(actWork *act)
{
	if(act->work[2]==0){
		act->y += 4;
		if(act->y > act->work[3]){
			act->work[2]++;
			act->work[0] = 22;
			SePlay(SE_KON);		
		}
	}else{
		if(act->work[0]==66) SePlay(SE_KON2);
		if(act->work[0]==92) SePlay(SE_KON3);
		if(act->work[0]==107) SePlay(SE_KON4);
		

		act->dy += BallBoundTbl[act->work[0]];
	
		if(++act->work[0]==108){
			act->move = DummyActMove;
		}
	}
}

// =================================================================
// EventKoukanJunbi
// �T�v  : �����C�x���g�p�̃|�P�����̖��O�̏���[�X�N���v�g�p�֐�]
// ����  : none(ScriptParameter0�Ƀe�[�u���̔ԍ�)
// �߂�l: u16	�v�������|�P�����i���o�[
// =================================================================
u16 EventKoukanJunbi(void)
{
	KOUKAN_POKEMON *kp;
	
	kp = (KOUKAN_POKEMON*)&EventKoukanPokemon[ScriptParameter0];

	PM_strcpy(StrTempBuffer0, PokeNameData[kp->wantpoke]);
	PM_strcpy(StrTempBuffer1, PokeNameData[kp->monsno]);


	return kp->wantpoke;			//�v�������|�P�����i���o�[

}

// =================================================================
// KoukanEventEndNameSet
// �T�v  : ��������f���I�����ɃX�N���v�g�p�̕�������Z�b�g����
// ����  : none
// �߂�l: static
// =================================================================
static void KoukanEventEndNameSet()
{
	KOUKAN_POKEMON *kp;
	u8 strbuf[30];
	
	kp = (KOUKAN_POKEMON*)&EventKoukanPokemon[ScriptParameter0];

	PokeParaGet(&PokeParaMine[ScriptParameter1],ID_nickname,strbuf);
	NickNameCopy(StrTempBuffer0, strbuf);
	PM_strcpy(StrTempBuffer1, PokeNameData[kp->monsno]);

}

// =================================================================
// EventKoukanPokeMakeSub
// �T�v  : �����C�x���g�p�̃|�P�����f�[�^�𐶐��@�T�u���[�`��
// ����  : no �� �e�[�u���̔ԍ�
// �߂�l: u16
// =================================================================
void EventKoukanPokeMakeSub(u8 mine, u8 no)
{
	u8  mylevel,get_place,mailid;
	KOUKAN_POKEMON *kp;
	MAIL_DATA	mail;
	
	kp        = (KOUKAN_POKEMON*)&EventKoukanPokemon[no];
	mylevel   = PokeParaGet(&PokeParaMine[mine],ID_level);
	get_place = POS_CABLEKOUKAN;

//	PokeParaSetPowRndBuf(&PokeParaEnemy[0],kp->monsno, mylevel, kp->rnds, kp->id_no);	//�|�P�����쐬
	PokeParaSet(&PokeParaEnemy[0],
		kp->monsno,mylevel,POW_RND,1,kp->personal_rnd,1,kp->id_no);

	PokeParaPut(&PokeParaEnemy[0],ID_hp_rnd,(u8 *)&kp->rnds[0]);
	PokeParaPut(&PokeParaEnemy[0],ID_pow_rnd,(u8 *)&kp->rnds[1]);
	PokeParaPut(&PokeParaEnemy[0],ID_def_rnd,(u8 *)&kp->rnds[2]);
	PokeParaPut(&PokeParaEnemy[0],ID_agi_rnd,(u8 *)&kp->rnds[3]);
	PokeParaPut(&PokeParaEnemy[0],ID_spepow_rnd,(u8 *)&kp->rnds[4]);
	PokeParaPut(&PokeParaEnemy[0],ID_spedef_rnd,(u8 *)&kp->rnds[5]);



	PokeParaPut(&PokeParaEnemy[0],ID_nickname,(u8*)kp->nickname);
	PokeParaPut(&PokeParaEnemy[0],ID_oyaname, (u8*)kp->oyaname);
	PokeParaPut(&PokeParaEnemy[0],ID_oyasex, (u8*)&(kp->oyasex));
	PokeParaPut(&PokeParaEnemy[0],ID_speabi,  (u8*)&(kp->speabi));
	PokeParaPut(&PokeParaEnemy[0],ID_beautiful,  (u8*)&(kp->beautiful));
	PokeParaPut(&PokeParaEnemy[0],ID_cute,  (u8*)&(kp->cute));
	PokeParaPut(&PokeParaEnemy[0],ID_style,  (u8*)&(kp->style));
	PokeParaPut(&PokeParaEnemy[0],ID_clever,  (u8*)&(kp->clever));
	PokeParaPut(&PokeParaEnemy[0],ID_strong,  (u8*)&(kp->strong));
	PokeParaPut(&PokeParaEnemy[0],ID_fur,  (u8*)&(kp->fur));
	PokeParaPut(&PokeParaEnemy[0],ID_get_place, (u8*)&get_place);

	

	mailid = 0;
	if(kp->item!=NON_ITEM){			
		if(MailItemNoCheck(kp->item)){									//���[����������
			KoukanMail2Mail(&mail,kp);
			EnemyMail[0] = mail;
			PokeParaPut(&PokeParaEnemy[0],ID_mailid,  (u8*)&mailid);
			PokeParaPut(&PokeParaEnemy[0],ID_item,  (u8*)&(kp->item));
		}else{
			PokeParaPut(&PokeParaEnemy[0],ID_item,  (u8*)&(kp->item));	//�A�C�e���������玝������
		}
	}
	PokeParaCalc(&PokeParaEnemy[0]);
}

// =================================================================
// KoukanMail2Mail
// �T�v  : �Q�[���������f�[�^���烁�[�����쐬����
// ����  : p
//       : kp
// �߂�l: none
// =================================================================
static void KoukanMail2Mail(MAIL_DATA *p, KOUKAN_POKEMON *kp)
{
	int i;

	for(i=0;i<MAIL_MSG_LEN;i++) p->msg[i] = KoukanMail[kp->mailno].msg[i];	//���b�Z�[�W�Z�b�g

	PM_strcpy(p->writer_name,kp->oyaname);
	p->writer_id[0] = kp->id_no>>24;
	p->writer_id[1] = (kp->id_no>>16)&0xff;
	p->writer_id[2] = (kp->id_no>>8)&0xff;
	p->writer_id[3] = kp->id_no&0xff;
	p->hold_monsno = kp->monsno;
	p->design_no   = kp->item;

}

// =================================================================
// PokeMineMonsNumber
// �T�v  : �莝���i���o�[�̃|�P������monsno��Ԃ�[�X�N���v�g�p�֐�]
// ����  : none
// �߂�l: u16
// =================================================================
u16 PokeMineMonsNumber(void)
{
	if(PokeParaGet(&PokeParaMine[ScriptParameter1],ID_tamago_flag)){
		return 0;
	}
	return PokeParaGet(&PokeParaMine[ScriptParameter1],ID_monsno);
}

// =================================================================
// EventKoukanTest
// �T�v  : �|�P�����C�x���g��������b���[�X�N���v�g�p�֐�]
// ����  : none
// �߂�l: none
// =================================================================
void EventKoukanTest()
{
	EventKoukanPokeMakeSub(ScriptParameter1, ScriptParameter0);

//	PokeParaMine[ScriptParameter1]  = PokeParaEnemy[0];
}

//------------------------------------------------------------------
//  ��������f�����C��
//------------------------------------------------------------------
void KoukanDemoMain()
{

	if(DemoPokemonKoukan2()==1){					//��������f���̎��s{
		
//		if(sys.Trg&A_BUTTON){					//��������
//			SePlay(SE_SELECT);
			DelActor(&ActWork[kdw->actno[TYPE_MINE]]);
			AffineDelActor(&ActWork[kdw->actno[TYPE_ENEMY]]);
			PokemonKoukan(exchange_pokemon[0],(exchange_pokemon[1]%6));					//�|�P�����f�[�^������
			kdw->sendwork[0] = 0xABCD;			//�I���ҋ@�R�[�h���M
			kdw->sousin=1;
			MainProcChange(KoukanDemoEnd);
		
//		MsgPrint(&kdw->tw, endwait_msg,20,5,5);
//		NMenuWinBoxWrite(4,4,4+18,4+5);					
//		NMenuMsgWrite(endwait_msg,5,5);
//		exchange_pokemon[0] = 0xff;
//		}
	}
	SousinOkFlag();

	SioFunc();

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

//------------------------------------------------------------------
//  �ʐM���s���ăf���̏I���̃^�C�~���O��}��
//------------------------------------------------------------------
void KoukanDemoEnd()
{
	u8 Id;

	Id=GetMyId();

	SioFunc();

	if(Id==0){					//�q�@�E�e�@���ɏI���̃t���O����������q�@�ɏI����ʐM����
		if(kdw->end_ok[0]==1&&kdw->end_ok[1]==1){
			kdw->sendwork[0] = 0xDCBA;
			SplitDataSendStart(GetAnotherSendTo(),(u8*)kdw->sendwork,20);
			kdw->end_ok[0]=2;
			kdw->end_ok[1]=2;
		}
	}

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}


// =================================================================
// KoukanDemoSaveSeq
// �T�v  : �ʐM�|�[�Y�@�\��҂��Z�[�u���s�����A���鏈��
// ����  : none
// �߂�l: none
// =================================================================
void KoukanDemoSaveSeq()
{

		switch(sys.InitSeqWork){
			case 0:
				kdw = (KOUKANDEMO_WORK*)&UserWork[0x1f000];
				sys.InitSeqWork++;
					FontScrnAreaClear(&kdw->tw, 0, 0, 29, 19);
					PM_MsgExpand(MsgExpandBuffer, TuushinTaikiMsg);
					MsgPrint( &kdw->tw, MsgExpandBuffer, kdw->windowwork, CL_WINMSG_X, CL_WINMSG_Y);
				break;
			case 1:										//
					SetSioSynchronize();
					sys.InitSeqWork = 100;
					kdw->wait = 0;
				break;
			case 100:
				kdw->wait++;
				if(kdw->wait > 3*60){
					sys.InitSeqWork = 101;
					kdw->wait = 0;
				}
				if(SplitDataSendStatus()){
					sys.InitSeqWork = 2;
				}
				break;
			case 101:
				if(SplitDataSendStatus()){
					sys.InitSeqWork = 2;
				}
				break;
			case 2:										//
//				kdw->actno[2] = AddActor(&Light2Header,240-20,136,0);
//				sys.InitSeqWork++;
				sys.InitSeqWork = 50;
				FontScrnAreaClear(&kdw->tw, 0, 0, 29, 19);
				MsgPrint( &kdw->tw, report_writing_msg, kdw->windowwork, CL_WINMSG_X, CL_WINMSG_Y);
				break;
/*
			case 3:

				SetCommSaveFlag();						//���܂��Ȃ�
//				PokeAGBSave( POKEAGB_SVFLD );			//�Z�[�u
				PokeAGBSaveTrans(POKEAGB_SVMYD);		//�Z�[�u�����ύX 2002/09/17
				sys.InitSeqWork = 31;
				break;
			case 31:
				if(!PokeAGBSaveTransWait()){
					ClearCommSaveFlag();
					sys.InitSeqWork = 4;
					DelActor(&ActWork[kdw->actno[2]]);
				}
				break;
*/			
			case 50:
				SetCommSaveFlag();						//���܂��Ȃ�
				IncRecord(CNT_COMM_TRADE);				//�ʐM�����̉񐔂��C���N�������g
			//	IncTrCardKoukanCount();
				PokeAGBSaveSioDivInit();				//ALL�Z�[�u������
				sys.InitSeqWork++;
				kdw->wait = 0;
				break;
			case 51:
				kdw->wait++;
				if(kdw->wait==5) sys.InitSeqWork++;
				break;
			case 52:
				if(PokeAGBSaveSioDiv()){		
					ClearCommSaveFlag();
					sys.InitSeqWork = 4;		//���ޏI�����Ă���S��
//					DelActor(&ActWork[kdw->actno[2]]);
				}else{
					kdw->wait = 0;			
					sys.InitSeqWork = 51;		//�I�����ĂȂ�������51������
				}
				break;
			case 4:
					PokeAGBSaveSioDiv1();				//�S�̃Z�[�u�̃��X�g
					sys.InitSeqWork = 40;
					kdw->wait = 0;
				break;
			case 40:
				kdw->wait++;
				if(kdw->wait > 50){
					kdw->wait = 0;
					sys.InitSeqWork = 41;
				}
				break;
			case 41:
					SetSioSynchronize();
					sys.InitSeqWork = 42;
					break;
			case 42:
				if(SplitDataSendStatus()){
					PokeAGBSaveSioDiv2();				//�S�̃Z�[�u�̃��X�g
					SoftResetDisable = 0;					//�\�t�g���Z�b�g������L����
					sys.InitSeqWork = 5;
				}
				break;
			case 5:										//�҂�
				if(++kdw->wait>60){
					sys.InitSeqWork++;					//Flash�ɃZ�[�u���鎞�Ԃ���Ђɂ���ĈႤ����
					SetSioSynchronize();				//�t�F�[�h�A�E�g�̃^�C�~���O������邱�Ƃ�������������
														//�ڂŊm�F���đ��B�o�O���o����̂ŁA�����Ƀt�F�[�h�A�E�g����悤��
														//�ʐM��������邱�Ƃɂ���
				}
				break;
			case 6:										//�t�F�[�h�A�E�g
				if(SplitDataSendStatus()){
					PaletteFadeReq(0xffffffff,0,0,16,0);
					sys.InitSeqWork++;
				}
				break;
			case 7:	
				if(FadeData.fade_sw == 0){
//					BGM_FadeOutSet( 3);						//BGM�t�F�[�h�A�E�g
					MusicFadeOutEnd(3);
					sys.InitSeqWork++;
				}
				break;
			case 8:
				if(MusicFadeOutEndWait()==TRUE){
//				if(BGM_FadeOutCheck()==TRUE){
					SetSioEndDataStart();					//�ʐM�I���R�[�h���M�J�n
					sys.InitSeqWork++;
				}
				break;
			case 9:
				if(pokemon_sio_flag==0){					//�ʐM�I��
					MainProcChange(KoukanDemoEndWait);
				}
				break;
		}
	
	if(!MultiSioErrorCheck()){		//�G���[�������̓^�X�N�����s�����Ȃ�
		MainTask();
	}	
		JumpActor();
		SetActor();
		PaletteAnime();
}

//------------------------------------------------------------------
//  �f���I��
//------------------------------------------------------------------
void KoukanDemoEndWait()
{
	if(FadeData.fade_sw == 0){
//		PokemonMultiSioEnd();
		MainProcChange(KoukanMainInit);
	}

	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

// =================================================================
// KoukanDemoOfflineCall
// �T�v  : �C�x���g��������p�̃f����ʂ��Ăяo��
// ����  : none
// �߂�l: none
// =================================================================
void KoukanDemoOfflineCall(void)
{
	u8	id;

	SetForceEvent();
	id = AddTask( InitFadeWait, 10 );
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}



static void InitFadeWait( u8 id )
{
	if( !(FadeData.fade_sw) ){
		MainProcChange(KoukanDemoOfflineInit);
		pFieldRecoverFunc = AddScriptFieldInTask;
//		FadeData.trans_stop = 1;
		DelTask(id);
	}
}


// =================================================================
// RibbonCheck
// �T�v  : �����ŖႤ�|�P���������{���������Ă�����t���O�𗧂Ă�
// ����  : none
// �߂�l: none
// =================================================================
static void RibbonCheck()
{
	u8 i,work = 0;
	for(i=0;i<12;i++){
		work += PokeParaGet(&PokeParaEnemy[exchange_pokemon[1]%6],ID_champ_ribbon+i);
	}
	if(work){
		EventFlagSet(SYS_RIBBON_GET);
	}
}

#if 0
// =================================================================
// IncTrCardKoukanCount
// �T�v  : �ʐM�g���[�i�[�J�[�h�̌����񐔂��C���N�������g����
// =================================================================

static void IncTrCardKoukanCount(void)
{
	//�����͂Q�l�ōs���̂ŒP���ɂh�c�O�ƂP�̕����𑀍삷�邾��
	if (MultiTrainerCard[0].cable_poke_exe < TRCARD_POKE_EXE_MAX)
		MultiTrainerCard[0].cable_poke_exe ++;

	if (MultiTrainerCard[1].cable_poke_exe < TRCARD_POKE_EXE_MAX)
		MultiTrainerCard[1].cable_poke_exe ++;
}
#endif


void BgSetTuushinShinka()
{
	//�w�i�f�[�^�Z�b�g
	DecordVram((void *)batt_bg1_pch_ADRS,(void *)BG_VRAM);
	DecordWram((void *)batt_bg1_psc_ADRS,(void *)UserWork);
	DIV_DMACOPY(3,UserWork,BG_VRAM+5*0x800,0x500,16);
	DecordPaletteWork((void *)batt_bg1_pcl_ADRS,PA_BG0,0x20);
		*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
				| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
				| BG_PRIORITY_2					//BG�D�揇��
				| 5 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
				| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
}
