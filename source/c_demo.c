
//======================================================
//    c_demo.c                                           
//                                                      
//    Copyright (C) 2000 GAME FREAK inc.
//======================================================

#include "common.h"
#include "bss.h"		//add
#include "calctool.h"		//add
#include "poketool.h"		//add
#include "decord.h"		//add
#include "objdata.h"		//add

#include "gauge.h"
#include "actor.h"
#include "actanm.h"
#include "client.h"
#include "clientai.h"
#include "a_enemy.h"
#include "fight.h"
#include "print.h"
#include "client_t.h"
#include "waza_eff.h"
#include "server.h"
#include "cli_def.h"	//add 2001.07.13(��) by matsuda
#include "wazatool.h"
#include "client2.h"
#include "cli_ef.h"
#include "pm_aff.h"
#include "cli_num.h"

#include "multisio.h"		//add
#include "pokesio.h"
#include "cable.h"

#include "fight_ef.h"
#include "mus_tool.h"
#include "fld_main.h"
#include "task.h"
#include "fightmsg.h"
#include "cubecase.h"

#include "b_return.h"
#include "trbenum.h"
#include "a_fight.h"
#include "waza_dat.h"

enum{
		MSG_END,
		MSG_START,
		MSG_WRITE,
		MSG_WAIT,
		MSG_KEYWAIT,
		MSG_CR,
};

//------------------------- �f�[�^ -----------------------------

//---------------------- ���[�J���ϐ� --------------------------

//-------------------- �T�u���[�`���錾 ------------------------

void	ClientDBitReset(void);	//�͈͊O�̒l�̎��̏���
void	ClientDDummy(void);
void	ClientDInit(void);
void	ClientDCommandWait(void);

void	ClientDCommandSelect(void);
void	ClientDWazaSelect(void);
void	ClientDMessageWait(void);
void	ClientDPokeSendOutWait(void);
void	ClientDSendOutGaugeWait(void);

static	void ClientItemWait(void);
static	void ClientItemReturnWait(void);

void ClientDHPGaugeWait(void);

void	ClientDBSSPokeparaGet(void);
void	ClientDBSSPokeparaGet2(void);
void	ClientDBSSPokeparaPut(void);
void	ClientDBSSPokeparaPut2(void);
void	ClientDBSSPokeEncount(void);
void	ClientDBSSPokeAppear(void);
void	ClientDBSSPokeReturn(void);
void	ClientDBSSTrainerEncount(void);
void	ClientDBSSTrainerAppear(void);
void	ClientDBSSTrainerReturn(void);
void	ClientDBSSPokeKizetu(void);
void	ClientDBSSZenmetsu(void);
void	ClientDBSSGetSuccess(void);
void	ClientDBSSGetFail(void);
void	ClientDBSSPokeLevelUp(void);
void	ClientDBSSWazaEffect(void);
void	ClientDBSSMessage(void);
void	ClientDBSSAlertMessage(void);
void	ClientDBSSCommandSelect(void);
void	ClientDBSSCommandSelectCopy(void);
void	ClientDBSSWazaSelect(void);
void	ClientDBSSItemSelect(void);
void	ClientDBSSPokeSelect(void);
void	ClientDBSSEscape(void);
void	ClientDBSSHpGauge(void);
void	ClientDBSSExpGauge(void);
void	ClientDBSSStatusSet(void);
void	ClientDBSSStatusEffect(void);
void	ClientDBSSStatusClear(void);
void	ClientDBSSDataTrans(void);
void	ClientDBSSDataRamTrans(void);
void	ClientDBSSBgmTrans(void);
void	ClientDBSSVoiceTrans(void);
void	ClientDBSSSelectReturn(void);
void	ClientDBSSReshuffle(void);
void	ClientDBSSUseItem(void);
void	ClientDBSSUseBall(void);
void	ClientDBSSLoopCountClear(void);
void	ClientDBSSLoopCountSet(void);
void	ClientDBSSSwitchFlagClear(void);
void	ClientDBSSSwitchFlagSet(void);
void	ClientDBSSPokeDamageBlink(void);
void	ClientDBSSGaugeOff(void);
void	ClientDBSSSEPlay(void);
void	ClientDBSSMEPlay(void);
void	ClientDBSSVoicePlay(void);
void	ClientDBSSEncountEffect(void);
void	ClientDBSSPokeSendOut(void);
void	ClientDBSSBallGaugeSet(void);
void	ClientDBSSBallGaugeOff(void);
void	ClientDBSSGaugeYureStop(void);
void	ClientDBSSVanishOff(void);
void	ClientDBSSFightEffect(void);
void	ClientDBSSTuushinWait(void);
void	ClientDBSSCsrPosClear(void);
void	ClientDBSSFightEnd(void);
void	ClientDBSSEndID(void);

int		ClientDBSSPokeparaGetAct(u8,u8 *);
void	ClientDBSSPokeparaPutAct(u8);

static	void	SendOutTask(u8 id);
static	void	ClientDWazaEffectSet(void);

extern	actHeader PokeFTbl[];
extern	TALK_WIN	FightMsg;
extern u16	ReturnItemNo;

extern void	BtlRuckInit(void);
extern void PokeGetDemoBagInit(void);	// 2002/07/05 by nakahiro

//extern	u8	FightMsgBuf[64];

//----------------------------------------------------------
//	�o�g���T�[�o�[���ߌQ�e�[�u��
//----------------------------------------------------------
const pFunc ClientDBSSTbl[] =
{
	ClientDBSSPokeparaGet,		// BSS_POKEPARA_GET=0,	
	ClientDBSSPokeparaGet2,	   	// BSS_POKEPARA_GET2,	
	ClientDBSSPokeparaPut,		// BSS_POKEPARA_PUT,	
	ClientDBSSPokeparaPut2,		// BSS_POKEPARA_PUT2,	
	ClientDBSSPokeEncount,		// BSS_POKEPARA_ENCOUNT,	add
	ClientDBSSPokeAppear,		// BSS_POKE_APPEAR,	
	ClientDBSSPokeReturn,		// BSS_POKE_RETURN,	
	ClientDBSSTrainerEncount,	// BSS_TRAINER_ENCOUNT,	add
	ClientDBSSTrainerAppear,		// BSS_TRAINER_APPEAR,	
	ClientDBSSTrainerReturn,		// BSS_TRAINER_RETURN,	
	ClientDBSSPokeKizetu,		// BSS_POKE_KIZETSU,	
	ClientDBSSZenmetsu,			// BSS_POKE_ZENMETSU,	
	ClientDBSSGetSuccess,		// BSS_GET_SUCCESS,	
	ClientDBSSGetFail,			// BSS_GET_FAIL,		
	ClientDBSSPokeLevelUp,		// BSS_POKE_LEVELUP,	
	ClientDBSSWazaEffect,		// BSS_WAZA_EFFECT,	
	ClientDBSSMessage,			// BSS_MESSAGE,		
	ClientDBSSAlertMessage,		// BSS_ALERT_MESSAGE,		
								
	ClientDBSSCommandSelect,		// BSS_COMMAND_SELECT, 

	ClientDBSSCommandSelectCopy,		// BSS_COMMAND_SELECT_COPY, 

	ClientDBSSWazaSelect,		// BSS_WAZA_SELECT,	
													
	ClientDBSSItemSelect,		// BSS_ITEM_SELECT,	
													
	ClientDBSSPokeSelect,		// BSS_POKE_SELECT,	
													
	ClientDBSSEscape,			// BSS_ESCAPE,			
	ClientDBSSHpGauge,			// BSS_HP_GAUGE,		
	ClientDBSSExpGauge,			// BSS_EXP_GAUGE,		
	ClientDBSSStatusSet,			// BSS_STATUS_SET,		
	ClientDBSSStatusEffect,		// BSS_STATUS_EFFECT,		
	ClientDBSSStatusClear,		// BSS_STATUS_CLEAR,	
	ClientDBSSDataTrans,			// BSS_DATA_TRANS,		
	ClientDBSSDataRamTrans,		// BSS_DATA_RAM_TRANS,	
													
	ClientDBSSBgmTrans,			// BSS_BGM_TRANS,		
	ClientDBSSVoiceTrans,		// BSS_VOICE_TRANS,	
	ClientDBSSSelectReturn,		// BSS_SELECT_RETURN,	
	ClientDBSSReshuffle,			// BSS_POKE_RESHUFFLE,	
	ClientDBSSUseItem,			// BSS_USE_ITEM,		
	ClientDBSSUseBall,			// BSS_USE_BALL,		
	ClientDBSSLoopCountClear,	//	BSS_LOOPCOUNT_CLEAR
	ClientDBSSLoopCountSet,	//	BSS_LOOPCOUNT_SET
	ClientDBSSSwitchFlagClear,	//	BSS_SWITCHFLAG_CLEAR
	ClientDBSSSwitchFlagSet,	//	BSS_SWITCHFLAG_SET
	ClientDBSSPokeDamageBlink,	// BSS_POKEDAMAGE_BLINK,
	ClientDBSSGaugeOff,		// BSS_GAUGE_OFF,
	ClientDBSSSEPlay,			// BSS_SE_PLAY,
	ClientDBSSMEPlay,			// BSS_SE_PLAY,
	ClientDBSSVoicePlay,			// BSS_VOICE_PLAY,
	ClientDBSSEncountEffect,	// BSS_ENCOUNT_EFFECT,
	ClientDBSSPokeSendOut,		// BSS_POKE_SENDOUT,
	ClientDBSSBallGaugeSet,	// BSS_BALL_GAUGE_SET,
	ClientDBSSBallGaugeOff,	// BSS_BALL_GAUGE_OFF,
	ClientDBSSGaugeYureStop,	// BSS_BALL_GAUGE_OFF,
	ClientDBSSVanishOff,	// BSS_VANISH_OFF,
	ClientDBSSFightEffect,	// BSS_FIGHT_EFFECT,
	ClientDBSSTuushinWait,	// BSS_TUUSHIN_WAIT,
	ClientDBSSCsrPosClear,		// BSS_CSRPOS_CLEAR,
	ClientDBSSFightEnd,		// BSS_FIGHT_END,

	ClientDBSSEndID,				// BSS_END_ID,		����Ȃ����ǈꉞ
};

//----------------------------------------------------------
//	�v���g�^�C�v�錾	�֐����ް�
//----------------------------------------------------------

#define	CAI_LVNUM_X			84
#define	CAI_LVNUM_Y			28

//==============================================================
//                       ���C�����[�`��
//==============================================================

void	ClientDDummy(void)
{
}

void	ClientDInit(void)
{
	ClientSequence[ClientNo]=ClientDCommandWait;
	UserWork[GET_DEMO_COM_SEL]=0;
	UserWork[GET_DEMO_WAZA_SEL]=0;
	UserWork[GET_DEMO_WORK0]=0;
	UserWork[GET_DEMO_WORK1]=0;

#if 0	//�����]���p�Ɉ�ɂ܂Ƃ߂�
	//�ް�޷�� ýĂȂ̂łƂ肠�����A���ۂ͏풓�Ȃ̂ł����ł͂��Ȃ��@��
	
	ClientGaugeSt[ClientNo] = GaugeActorSet(ClientNo);	//�ް�ޱ����o�^ ������Ƃ肠����������
	GaugeDefaultPosSet(ClientNo);
	GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaMine[SelMonsNo[ClientNo]], G_ALL);
	GaugeOFF(ClientGaugeSt[ClientNo]);
	if(ClientNo == BSS_CLIENT_ENEMY)
		GaugePriSet(ClientGaugeSt[ClientNo], 0);
	else
		GaugePriSet(ClientGaugeSt[ClientNo], 1);
#endif
}

void	ClientDCommandWait(void)
{
	if((ClientBit&no2bittbl[ClientNo]) != 0)
	{
		if(ServerBuffer[ClientNo][0] <= BSS_END_ID)// && ServerBuffer[ClientNo][0] > -1)
		{
			ClientDBSSTbl[ServerBuffer[ClientNo][0]]();
		}
		else
		{
			ClientDBitReset();	//�͈͊O�̒l�̎��̏���
		}
	}
}

//=========================================================================
//	�E�F�C�g���[�`��
//=========================================================================

enum{
	GETDEMO_SEQ_INIT=0,
	GETDEMO_SEQ_FIGHT_SELECT1,
	GETDEMO_SEQ_FIGHT_SELECT2,
	GETDEMO_SEQ_TRAINER_APPEAR,
	GETDEMO_SEQ_ITEM_SELECT_INIT,
	GETDEMO_SEQ_ITEM_SELECT,
};

#define	COM_WAIT	64

void	ClientDCommandSelect(void)
{
	switch(UserWork[GET_DEMO_COM_SEL]){
		case GETDEMO_SEQ_INIT:
			UserWork[GET_DEMO_WORK0]=COM_WAIT;
			UserWork[GET_DEMO_COM_SEL]++;
		case GETDEMO_SEQ_FIGHT_SELECT1:
			if(--UserWork[GET_DEMO_WORK0]==0){
				SePlay(SE_SELECT);
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_FIGHT,0);
				ClientDBitReset();
				UserWork[GET_DEMO_COM_SEL]++;
				UserWork[GET_DEMO_WAZA_SEL]=0;
				UserWork[GET_DEMO_WORK0]=COM_WAIT;
			}
			break;
		case GETDEMO_SEQ_FIGHT_SELECT2:
			if(--UserWork[GET_DEMO_WORK0]==0){
				SePlay(SE_SELECT);
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_FIGHT,0);
				ClientDBitReset();
				UserWork[GET_DEMO_COM_SEL]++;
				UserWork[GET_DEMO_WAZA_SEL]=0;
				UserWork[GET_DEMO_WORK0]=COM_WAIT;
			}
			break;
		case GETDEMO_SEQ_TRAINER_APPEAR:
			if(--UserWork[GET_DEMO_WORK0]==0){
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_GET_DEMO,0);
				ClientDBitReset();
				UserWork[GET_DEMO_COM_SEL]++;
				UserWork[GET_DEMO_WAZA_SEL]=0;
				UserWork[GET_DEMO_WORK0]=COM_WAIT;
			}
			break;
		case GETDEMO_SEQ_ITEM_SELECT_INIT:
			if(--UserWork[GET_DEMO_WORK0]==0){
				SePlay(SE_SELECT);
				CommandCsrOff(0);
				CommandCsrOn(1,0);
				UserWork[GET_DEMO_WORK0]=COM_WAIT;
				UserWork[GET_DEMO_COM_SEL]++;
			}
			break;
		case GETDEMO_SEQ_ITEM_SELECT:
			if(--UserWork[GET_DEMO_WORK0]==0){
				SePlay(SE_SELECT);
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_ITEM,0);
				ClientDBitReset();
			}
			break;
	}
}

void ClientDTrainerEncountWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientDBitReset();
	}
}

void	ClientDMessageWait(void)
{
	if(FightMsg.status==MSG_END){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientDCommandWait;
		ClientDBitReset();
	}
}

static void ClientFightEndWait(void)
{
	if(FadeData.fade_sw == 0){
		sys.FieldBattleFlag=0;			//�퓬��Flag�����Ƃ�
		sys.pFirstProc=PushFirstProc;
		MainProcChange(sys.pReturnProc);
	}
}

static	void ClientBallEffectWait(void)
{
//	if(ActWork[ClientStruct[ClientNo]].act_sw == 0)
	if(ClientEffectWait == 0)
//	if(CliSys[ClientNo].ball_eff_wait == 0)
	{
//		AllDelActor(&ActWork[ClientStruct[ClientNo]]);
		ClientDBitReset();
	}
}

static	void ClientItemWait(void)
{
	if(FadeData.fade_sw == 0)
	{
		ClientSequence[ClientNo] = ClientItemReturnWait;
		PushBattleWork();
		PokeGetDemoBagInit();
	}
}

static	void ClientItemReturnWait(void)
{
	if((sys.pMainProc==MainFight)&&
	   (FadeData.fade_sw == 0))
	{
		BSS_USE_ITEM_SET(BSS_CLIENT, ReturnItemNo);
		ClientDBitReset();
	}
}

void ClientDPokeSendOutWait(void)
{
//	if(ActWork[ClientStruct[ClientNo]].move == DummyActMove)	//2�����ɓ����Ă�̂�1�����������Ă����Ζ��Ȃ�����
//	if(ClientEffectWait == 0)
//	if(CliSys[ClientNo].ball_eff_wait == 0 && VoiceEndCheck() == 0)

	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo].rare_eff == 0)
		RareEffectCheckSet(ClientNo, &PokeParaMine[SelMonsNo[ClientNo]]);
	if(CliSys[ClientNo^2].ball_eff_wait == 0 && CliSys[ClientNo^2].rare_eff == 0)
		RareEffectCheckSet(ClientNo^2, &PokeParaMine[SelMonsNo[ClientNo^2]]);
	
	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo^2].ball_eff_wait == 0)
	{
		if(FightCheck() && 	(FightType & FIGHT_TYPE_4_2vs2) == 0)
		{
			DelActor(&ActWork[ClientStruct[ClientNo^2]]);		//�̪�ĊĎ�����DEL
			GaugeTransChrSet(ClientGaugeSt[ClientNo^2],
					&PokeParaMine[SelMonsNo[ClientNo^2]], G_ALL);
			GaugeAppearEffect(ClientNo^2);
			GaugeON(ClientGaugeSt[ClientNo^2]);
			
			//�ڰŰ�͎�����Del�����̂�
//			AffineDelActor(&ActWork[PokemonStruct[ClientNo^2]]);//�ꉞ(�Ă�ł����͂Ȃ���)
//			DelActor(&ActWork[PokemonStruct[ClientNo^2]]);
		}
		
		DelActor(&ActWork[ClientStruct[ClientNo]]);					//�̪�ĊĎ�����DEL
		GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaMine[SelMonsNo[ClientNo]], G_ALL);
		GaugeAppearEffect(ClientNo);
		GaugeON(ClientGaugeSt[ClientNo]);

		//�ڰŰ�͎�����Del�����̂�
//		AffineDelActor(&ActWork[PokemonStruct[ClientNo^2]]);//�ꉞ(�Ă�ł����͂Ȃ���)
//		DelActor(&ActWork[PokemonStruct[ClientNo^2]]);
		
		BtlWork->sendout_eff = 0;
		ClientSequence[ClientNo] = ClientDSendOutGaugeWait;
//		ClientBitReset();
	}
}

void ClientDSendOutGaugeWait(void)
{
	u8 end = 0;
	
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove)
		end = 1;
	
	if(end && CliSys[ClientNo].rare_eff_end && CliSys[ClientNo^2].rare_eff_end){
		CliSys[ClientNo].rare_eff = 0;
		CliSys[ClientNo].rare_eff_end = 0;
		CliSys[ClientNo^2].rare_eff = 0;
		CliSys[ClientNo^2].rare_eff_end = 0;
		CellDel(WAZACHR_ID_SHOOTINGSTAR);
		ObjPalRegNumDel(WAZACHR_ID_SHOOTINGSTAR);
		AddTask(VoiceCheckTask, 10);
		PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
		ClientDBitReset();
	}
}

void ClientDHPGaugeWait(void)
{
	s16 work;
//	u8 parts;
	
	work = GaugeOBJ(ClientNo, ClientGaugeSt[ClientNo], 0, 0);
	GaugeON(ClientGaugeSt[ClientNo]);
	//ScrY1 = 0x8D;
	if(work != -1)
	{
		GaugeHPTrans(ClientGaugeSt[ClientNo], work, 0);
	}
	else
	{
	#if 0
		//-- �ް�ޗh�炵�̎��ɕύX�����װ�����ɖ߂� --//
		ObjPalRegNumDel(CELLID_BattleGaugeColor);
		parts = ActWork[ClientGaugeSt[ClientNo]].work[5];
		ActWork[ClientGaugeSt[ClientNo]].oamData.Pltt = ActWork[parts].work[0];
		ActWork[parts].work[0] = 0;
	#endif
		PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
		ClientDBitReset();
	}
}

void ClientDPokeDamageBlinkWait(void)
{
	u8 pokeact;
	
	pokeact = PokemonStruct[ClientNo];
	if(ActWork[pokeact].work[1] == DAMAGE_BLINK_WAIT)
	{
		ActWork[pokeact].work[1] = 0;
		ActWork[pokeact].banish = 0;
		ClientEffectWait = 0;	//Blink�̪�āA�t���O��ؾ��
		ClientDBitReset();
	}
	else
	{
		if(ActWork[pokeact].work[1] % BLINK_INTERVAL == 0)
			ActWork[pokeact].banish ^= 1;
		ActWork[pokeact].work[1]++;
	}
}

void ClientDPokeReturnWait(void)
{
	if(CliSys[ClientNo].etc_eff == 0)
	{
		//�߹��DEL
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientDBitReset();
	}
}

void ClientDTrainerAppearWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientDBitReset();
	}
}

void ClientDFightEffWait(void)
{
	if(CliSys[ClientNo].fight_eff == 0)
		ClientDBitReset();
}


//==============================================================
//                       �T�u���[�`��
//==============================================================

void ClientDBitReset(void)
{
	u8	id;

	ClientSequence[ClientNo] = ClientDCommandWait;
#if 1
	if(FightType&FIGHT_TYPE_TUUSHIN){
		id=GetSioConnectId();
		TuushinReqSet(BSS_CLIENT_BIT,4,&id);
		ServerBuffer[ClientNo][0]=BSS_END_ID;
	}
	else
#endif
		ClientBit &= (no2bittbl[ClientNo] ^ 0xffffffff);
}

//----------------------------------------------------------
//	�ð���ُ�̪�ďI���҂�
//----------------------------------------------------------
void ClientDStatusEffWait(void)
{
	if(CliSys[ClientNo].status_eff == 0)
		ClientDBitReset();
}



//==============================================================
//	�N���C�A���g���ߊ֐�
//==============================================================

//----------------------------------------------------------
//	�w�肵���߹���ް����擾����
//----------------------------------------------------------

void	ClientDBSSPokeparaGet(void)
{
	u8 temp[256];
	int	length=0;
	int	i;
	u8	flag;

	if(ServerBuffer[ClientNo][2] == 0){
		length+=ClientDBSSPokeparaGetAct(SelMonsNo[ClientNo],&temp[length]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				length+=ClientDBSSPokeparaGetAct(i,&temp[length]);
			}
			flag=flag>>1;
		}
	}
	BSS_DATA_TRANS_SET(BSS_CLIENT,length,(u8 *)&temp);
	ClientDBitReset();
}

int	ClientDBSSPokeparaGetAct(u8 SelectNo,u8 *temp)
{
	PokemonServerParam	PCP;
	PokemonWazaParam	PWP;
	int	i=0;
	u8	*adr;
	u16	buf16;
	u32	buf32;
	u8 buff[20];
	
	switch(ServerBuffer[ClientNo][1])
	{
		case BSS_PARA_FIGHT:			//�퓬���ɕK�v�ȃp�����[�^�S��
			PCP.monsno		=PokeParaGet(&PokeParaMine[SelectNo],ID_monsno);
			PCP.item		=PokeParaGet(&PokeParaMine[SelectNo],ID_item);
			for(i=0;i<4;i++){
				PCP.waza[i]	=PokeParaGet(&PokeParaMine[SelectNo],ID_waza1+i);
				PCP.pp[i]	=PokeParaGet(&PokeParaMine[SelectNo],ID_pp1+i);
			}
			PCP.pp_count	=PokeParaGet(&PokeParaMine[SelectNo],ID_pp_count);
			PCP.friend		=PokeParaGet(&PokeParaMine[SelectNo],ID_friend);
			PCP.exp			=PokeParaGet(&PokeParaMine[SelectNo],ID_exp);
			PCP.hp_rnd		=PokeParaGet(&PokeParaMine[SelectNo],ID_hp_rnd);
			PCP.pow_rnd		=PokeParaGet(&PokeParaMine[SelectNo],ID_pow_rnd);
			PCP.def_rnd		=PokeParaGet(&PokeParaMine[SelectNo],ID_def_rnd);
			PCP.agi_rnd		=PokeParaGet(&PokeParaMine[SelectNo],ID_agi_rnd);
			PCP.spepow_rnd	=PokeParaGet(&PokeParaMine[SelectNo],ID_spepow_rnd);
			PCP.spedef_rnd	=PokeParaGet(&PokeParaMine[SelectNo],ID_spedef_rnd);
			PCP.personal_rnd=PokeParaGet(&PokeParaMine[SelectNo],ID_personal_rnd);
			PCP.condition	=PokeParaGet(&PokeParaMine[SelectNo],ID_condition);
			PCP.level		=PokeParaGet(&PokeParaMine[SelectNo],ID_level);
			PCP.hp			=PokeParaGet(&PokeParaMine[SelectNo],ID_hp);
			PCP.hpmax		=PokeParaGet(&PokeParaMine[SelectNo],ID_hpmax);
			PCP.pow			=PokeParaGet(&PokeParaMine[SelectNo],ID_pow);
			PCP.def			=PokeParaGet(&PokeParaMine[SelectNo],ID_def);
			PCP.agi			=PokeParaGet(&PokeParaMine[SelectNo],ID_agi);
			PCP.spepow		=PokeParaGet(&PokeParaMine[SelectNo],ID_spepow);
			PCP.spedef		=PokeParaGet(&PokeParaMine[SelectNo],ID_spedef);
			PCP.tamago_flag	=PokeParaGet(&PokeParaMine[SelectNo],ID_tamago_flag);
			PCP.speabi		=PokeParaGet(&PokeParaMine[SelectNo],ID_speabi);
			PCP.id_no		=PokeParaGet(&PokeParaMine[SelectNo],ID_id_no);
		#if 0	//���[�J���C�Y�Ή��̂��ߕύX 2002.08.09(��)
			PokeParaGet(&PokeParaMine[SelectNo],ID_nickname,PCP.nickname);
		#else
			PokeParaGet(&PokeParaMine[SelectNo],ID_nickname,buff);
			NickNameCopy(PCP.nickname, buff);
		#endif
			PokeParaGet(&PokeParaMine[SelectNo],ID_oyaname,PCP.oyaname);
			//BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(PCP),(u8 *)&PCP);
			adr=(u8 *)&PCP;
//			for(i=0;i<XtOffset(PokemonServerParam *, work);i++){
			for(i=0;i<sizeof(PokemonServerParam);i++){
				temp[i]=adr[i];
			}
			break;
		case BSS_PARA_MONSNO:			//�����X�^�[�ԍ�
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_monsno);
			temp[0]=(buf16&0x00ff)>> 0;
			temp[1]=(buf16&0xff00)>> 8;
			i=2;
			break;
		case BSS_PARA_ITEM:				//�����A�C�e��
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_item);
			temp[0]=(buf16&0x00ff)>> 0;
			temp[1]=(buf16&0xff00)>> 8;
			i=2;
			break;
		case BSS_PARA_WAZA:
			for(i=0;i<4;i++){
				PWP.waza[i] = PokeParaGet(&PokeParaMine[SelectNo],ID_waza1+i);
				PWP.pp[i]   = PokeParaGet(&PokeParaMine[SelectNo],ID_pp1+i);
			}
			PWP.pp_count=PokeParaGet(&PokeParaMine[SelectNo],ID_pp_count);
			adr=(u8 *)&PWP.waza[0];
			for(i=0;i<sizeof(PokemonWazaParam);i++){
				temp[i]=adr[i];
			}
			break;
		case BSS_PARA_WAZA1:				//�Z
		case BSS_PARA_WAZA2:
		case BSS_PARA_WAZA3:
		case BSS_PARA_WAZA4:
			buf16=
			PokeParaGet(&PokeParaMine[SelectNo],ID_waza1+ServerBuffer[ClientNo][1]-BSS_PARA_WAZA1);
			temp[0]=(buf16&0x00ff)>> 0;
			temp[1]=(buf16&0xff00)>> 8;
			i=2;
			break;
		case BSS_PARA_PP:
			for(i=0;i<4;i++){
				temp[i] = PokeParaGet(&PokeParaMine[SelectNo],ID_pp1+i);
			}
			temp[i] = PokeParaGet(&PokeParaMine[SelectNo],ID_pp_count);
			i++;
			break;
		case BSS_PARA_PP1:				//�Z�|�C���g
		case BSS_PARA_PP2:
		case BSS_PARA_PP3:
		case BSS_PARA_PP4:
			temp[0]=
			PokeParaGet(&PokeParaMine[SelectNo],ID_pp1+ServerBuffer[ClientNo][1]-BSS_PARA_PP1);
			i=1;
			break;
		case BSS_PARA_PPCOUNT1:
		case BSS_PARA_PPCOUNT2:
		case BSS_PARA_PPCOUNT3:
		case BSS_PARA_PPCOUNT4:
#if 0
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo].PPP.pp_count1;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo].PPP.pp_count2;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo].PPP.pp_count3;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo].PPP.pp_count4;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
#endif
			break;
		case BSS_PARA_ID:				//�h�c�i���o�[
			buf32=PokeParaGet(&PokeParaMine[SelectNo],ID_id_no);
			temp[0]=(buf32&0x000000ff)>> 0;
			temp[1]=(buf32&0x0000ff00)>> 8;
			temp[2]=(buf32&0x00ff0000)>>16;
			i=3;
			break;
		case BSS_PARA_EXP:				//��{�o���l
			buf32=PokeParaGet(&PokeParaMine[SelectNo],ID_exp);
			temp[0]=(buf32&0x000000ff)>> 0;
			temp[1]=(buf32&0x0000ff00)>> 8;
			temp[2]=(buf32&0x00ff0000)>>16;
			i=3;
			break;
		case BSS_PARA_HPEXP:			//�g�o�w�͒l
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_hp_exp);
			i=1;
			break;
		case BSS_PARA_POWEXP:			//�U���͓w�͒l
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_pow_exp);
			i=1;
			break;
		case BSS_PARA_DEFEXP:			//�h��͓w�͒l
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_def_exp);
			i=1;
			break;
		case BSS_PARA_AGIEXP:			//�f�����w�͒l
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_agi_exp);
			i=1;
			break;
		case BSS_PARA_SPEPOWEXP:		//����U���w�͒l
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_spepow_exp);
			i=1;
			break;
		case BSS_PARA_SPEDEFEXP:		//����h��w�͒l
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_spedef_exp);
			i=1;
			break;
		case BSS_PARA_FRIEND:			//�F�D�l
			temp[0]=PokeParaGet(&PokeParaMine[SelectNo],ID_friend);
			i=1;
			break;
		case BSS_PARA_POKERUS:			//�|�P���X
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_pokerus);
			i=1;
			break;
		case BSS_PARA_GET_PLACE:		//�ߊl�ꏊ
			temp[0]=PokeParaGet(&PokeParaMine[SelectNo],ID_get_place);
			i=1;
			break;
		case BSS_PARA_GET_LEVEL:		//�ߊl���x��
			temp[0]=PokeParaGet(&PokeParaMine[SelectNo],ID_get_level);
			i=1;
			break;
		case BSS_PARA_GET_CASSETTE:		//�ߊl�J�Z�b�g
			temp[0]=PokeParaGet(&PokeParaMine[SelectNo],ID_get_cassette);
			i=1;
			break;
		case BSS_PARA_GET_BALL:			//�ߊl�{�[��
			temp[0]=PokeParaGet(&PokeParaMine[SelectNo],ID_get_ball);
			i=1;
			break;
		case BSS_PARA_POWERRND:
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_hp_rnd);
			temp[1] = PokeParaGet(&PokeParaMine[SelectNo],ID_pow_rnd);
			temp[2] = PokeParaGet(&PokeParaMine[SelectNo],ID_def_rnd);
			temp[3] = PokeParaGet(&PokeParaMine[SelectNo],ID_agi_rnd);
			temp[4] = PokeParaGet(&PokeParaMine[SelectNo],ID_spepow_rnd);
			temp[5] = PokeParaGet(&PokeParaMine[SelectNo],ID_spedef_rnd);
			i=6;
			break;
		case BSS_PARA_HPRND:			//HP�p���[����
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_hp_rnd);
			i=1;
			break;
		case BSS_PARA_POWRND:			//�U���̓p���[����
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_pow_rnd);
			i=1;
			break;
		case BSS_PARA_DEFRND:			//�h��̓p���[����
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_def_rnd);
			i=1;
			break;
		case BSS_PARA_AGIRND:			//�f�����p���[����
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_agi_rnd);
			i=1;
			break;
		case BSS_PARA_SPEPOWRND:		//����U���̓p���[����
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_spepow_rnd);
			i=1;
			break;
		case BSS_PARA_SPEDEFRND:		//����h��̓p���[����
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_spedef_rnd);
			i=1;
			break;
		case BSS_PARA_CHRRND:			//������
			buf32 = PokeParaGet(&PokeParaMine[SelectNo],ID_personal_rnd);
			temp[0]=(buf32&0x000000ff)>> 0;
			temp[1]=(buf32&0x0000ff00)>> 8;
			temp[2]=(buf32&0x00ff0000)>>16;
			temp[3]=(buf32&0xff000000)>>24;
			i=4;
			break;
		case BSS_PARA_SUM:				//�`�F�b�N�T��
			buf16 = PokeParaGet(&PokeParaMine[SelectNo],ID_checksum);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_CONDITION:		//�R���f�B�V����
			buf32=PokeParaGet(&PokeParaMine[SelectNo],ID_condition);
			temp[0]=(buf32&0x000000ff)>> 0;
			temp[1]=(buf32&0x0000ff00)>> 8;
			temp[2]=(buf32&0x00ff0000)>>16;
			temp[3]=(buf32&0xff000000)>>24;
			i=4;
			break;
		case BSS_PARA_LEVEL:			//���x��
			temp[0]=PokeParaGet(&PokeParaMine[SelectNo],ID_level);
			i=1;
			break;
		case BSS_PARA_HP:				//�g�o�c��
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_hp);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_HPMAX:			//�g�o�l�`�w
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_hpmax);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_POW:				//�U����
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_pow);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_DEF:				//�h���
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_def);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_AGI:				//�f����
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_agi);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_SPEPOW:			//����U����
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_spepow);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_SPEDEF:			//����h���
			buf16=PokeParaGet(&PokeParaMine[SelectNo],ID_spedef);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_STYLE:			//�������悳
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_style);
			i=1;
			break;
		case BSS_PARA_BEAUTIFUL:		//������
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_beautiful);
			i=1;
			break;
		case BSS_PARA_CUTE:				//���킢��
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_cute);
			i=1;
			break;
		case BSS_PARA_CLEVER:			//����
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_clever);
			i=1;
			break;
		case BSS_PARA_STRONG:			//�����܂���
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_strong);
			i=1;
			break;
		case BSS_PARA_FUR:				//�щ�
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_fur);
			i=1;
			break;
		case BSS_PARA_STYLEMEDAL:		//�������悳�M��
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_stylemedal);
			i=1;
			break;
		case BSS_PARA_BEAUTIFULMEDAL:	//�������M��
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_beautifulmedal);
			i=1;
			break;
		case BSS_PARA_CUTEMEDAL:		//���킢���M��
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_cutemedal);
			i=1;
			break;
		case BSS_PARA_CLEVERMEDAL:		//�����M��
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_clevermedal);
			i=1;
			break;
		case BSS_PARA_STRONGMEDAL:		//�����܂����M��
			temp[0] = PokeParaGet(&PokeParaMine[SelectNo],ID_strongmedal);
			i=1;
			break;
	}
	return	i;
}


//----------------------------------------------------------
//	�w�肵�����ڽ���߹���ް����擾����
//----------------------------------------------------------

void ClientDBSSPokeparaGet2(void)
{
//	ClientDBitReset();
	ClientBSSPokeparaGet2();
}

//----------------------------------------------------------
//	�w�肵���߹���ް���ύX����
//----------------------------------------------------------

void ClientDBSSPokeparaPut(void)
{
	u8	i,flag;

	if(ServerBuffer[ClientNo][2] == 0){
		ClientDBSSPokeparaPutAct(SelMonsNo[ClientNo]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				ClientDBSSPokeparaPutAct(i);
			}
			flag=flag>>1;
		}
	}
	ClientDBitReset();
}

void ClientDBSSPokeparaPutAct(u8 SelectNo)
{
	PokemonServerParam *PCP;
	PokemonWazaParam *PWP;
	int	i;
	u8	buf;

	PCP=(PokemonServerParam *)&ServerBuffer[ClientNo][BSS_DATALOW];
	PWP=(PokemonWazaParam *)&ServerBuffer[ClientNo][BSS_DATALOW];

	switch(ServerBuffer[ClientNo][1])
	{
		case BSS_PARA_FIGHT:			//�퓬���ɕK�v�ȃp�����[�^�S��
			PokeParaPut(&PokeParaMine[SelectNo],ID_monsno, 
						(u8 *)&PCP->monsno);
			PokeParaPut(&PokeParaMine[SelectNo],ID_item,
						(u8 *)&PCP->item);
			for(i=0;i<4;i++){
				PokeParaPut(&PokeParaMine[SelectNo],ID_waza1+i,
							(u8 *)&PCP->waza[i]);
				PokeParaPut(&PokeParaMine[SelectNo],ID_pp1+i,
							(u8 *)&PCP->pp[i]);
			}
			PokeParaPut(&PokeParaMine[SelectNo],ID_pp_count,
						(u8 *)&PCP->pp_count);
			PokeParaPut(&PokeParaMine[SelectNo],ID_friend,
						(u8 *)&PCP->friend);
			PokeParaPut(&PokeParaMine[SelectNo],ID_exp,
						(u8 *)&PCP->exp);
			buf=PCP->hp_rnd;
			PokeParaPut(&PokeParaMine[SelectNo],ID_hp_rnd,
						(u8 *)&buf);
			buf=PCP->pow_rnd;
			PokeParaPut(&PokeParaMine[SelectNo],ID_pow_rnd,
						(u8 *)&buf);
			buf=PCP->def_rnd;
			PokeParaPut(&PokeParaMine[SelectNo],ID_def_rnd,
						(u8 *)&buf);
			buf=PCP->agi_rnd;
			PokeParaPut(&PokeParaMine[SelectNo],ID_agi_rnd,
						(u8 *)&buf);
			buf=PCP->spepow_rnd;
			PokeParaPut(&PokeParaMine[SelectNo],ID_spepow_rnd,
						(u8 *)&buf);
			buf=PCP->spedef_rnd;
			PokeParaPut(&PokeParaMine[SelectNo],ID_spedef_rnd,
						(u8 *)&buf);
			PokeParaPut(&PokeParaMine[SelectNo],ID_personal_rnd,
						(u8 *)&PCP->personal_rnd);
			PokeParaPut(&PokeParaMine[SelectNo],ID_condition,
						(u8 *)&PCP->condition);
			PokeParaPut(&PokeParaMine[SelectNo],ID_level,
						(u8 *)&PCP->level);
			PokeParaPut(&PokeParaMine[SelectNo],ID_hp,
						(u8 *)&PCP->hp);
			PokeParaPut(&PokeParaMine[SelectNo],ID_hpmax,
						(u8 *)&PCP->hpmax);
			PokeParaPut(&PokeParaMine[SelectNo],ID_pow,
						(u8 *)&PCP->pow);
			PokeParaPut(&PokeParaMine[SelectNo],ID_def,
						(u8 *)&PCP->def);
			PokeParaPut(&PokeParaMine[SelectNo],ID_agi,
						(u8 *)&PCP->agi);
			PokeParaPut(&PokeParaMine[SelectNo],ID_spepow,
						(u8 *)&PCP->spepow);
			PokeParaPut(&PokeParaMine[SelectNo],ID_spedef,
						(u8 *)&PCP->spedef);
			break;
		case BSS_PARA_MONSNO:			//�����X�^�[�ԍ�
			PokeParaPut(&PokeParaMine[SelectNo],ID_monsno, 
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_ITEM:				//�����A�C�e��
			PokeParaPut(&PokeParaMine[SelectNo],ID_item,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_WAZA:
			for(i=0;i<4;i++){
				PokeParaPut(&PokeParaMine[SelectNo],ID_waza1+i,
							(u8 *)&PWP->waza[i]);
				PokeParaPut(&PokeParaMine[SelectNo],ID_pp1+i,
							(u8 *)&PWP->pp[i]);
			}
			PokeParaPut(&PokeParaMine[SelectNo],ID_pp_count,
						(u8 *)&PWP->pp_count);
			break;
		case BSS_PARA_WAZA1:				//�Z
		case BSS_PARA_WAZA2:
		case BSS_PARA_WAZA3:
		case BSS_PARA_WAZA4:
			PokeParaPut(&PokeParaMine[SelectNo],ID_waza1+ServerBuffer[ClientNo][1]-BSS_PARA_WAZA1,&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_PP:
			PokeParaPut(&PokeParaMine[SelectNo],ID_pp1,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_pp2,
						&ServerBuffer[ClientNo][BSS_DATASTART+1]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_pp3,
						&ServerBuffer[ClientNo][BSS_DATASTART+2]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_pp4,
						&ServerBuffer[ClientNo][BSS_DATASTART+3]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_pp_count,
						&ServerBuffer[ClientNo][BSS_DATASTART+4]);
			break;
		case BSS_PARA_PP1:				//�Z�|�C���g
		case BSS_PARA_PP2:
		case BSS_PARA_PP3:
		case BSS_PARA_PP4:
			PokeParaPut(&PokeParaMine[SelectNo],ID_pp1+ServerBuffer[ClientNo][1]-BSS_PARA_PP1,&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_PPCOUNT1:
		case BSS_PARA_PPCOUNT2:
		case BSS_PARA_PPCOUNT3:
		case BSS_PARA_PPCOUNT4:
#if 0
			PokeParaEnemy[SelectNo].PPP.pp_count1 =
			ServerBuffer[ClientNo][BSS_DATASTART];
			break;
			PokeParaEnemy[SelectNo].PPP.pp_count2 =
			ServerBuffer[ClientNo][BSS_DATASTART];
			break;
			PokeParaEnemy[SelectNo].PPP.pp_count3 =
			ServerBuffer[ClientNo][BSS_DATASTART];
			break;
			PokeParaEnemy[SelectNo].PPP.pp_count4 =
			ServerBuffer[ClientNo][BSS_DATASTART];
#endif
			break;
		case BSS_PARA_ID:				//�h�c�i���o�[
			PokeParaPut(&PokeParaMine[SelectNo],ID_id_no,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			break;
		case BSS_PARA_EXP:				//��{�o���l
			PokeParaPut(&PokeParaMine[SelectNo],ID_exp,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			break;
		case BSS_PARA_HPEXP:			//�g�o�w�͒l
			PokeParaPut(&PokeParaMine[SelectNo],ID_hp_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_POWEXP:			//�U���͓w�͒l
			PokeParaPut(&PokeParaMine[SelectNo],ID_pow_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_DEFEXP:			//�h��͓w�͒l
			PokeParaPut(&PokeParaMine[SelectNo],ID_def_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_AGIEXP:			//�f�����w�͒l
			PokeParaPut(&PokeParaMine[SelectNo],ID_agi_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEPOWEXP:		//����U���w�͒l
			PokeParaPut(&PokeParaMine[SelectNo],ID_spepow_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEDEFEXP:		//����h��w�͒l
			PokeParaPut(&PokeParaMine[SelectNo],ID_spedef_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_FRIEND:			//�F�D�l
			PokeParaPut(&PokeParaMine[SelectNo],ID_friend,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_POKERUS:			//�|�P���X
			PokeParaPut(&PokeParaMine[SelectNo],ID_pokerus,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_GET_PLACE:		//�ߊl�ꏊ
			PokeParaPut(&PokeParaMine[SelectNo],ID_get_place,
						&ServerBuffer[ClientNo][BSS_DATASTART]); 
			break;
		case BSS_PARA_GET_LEVEL:		//�ߊl���x��
			PokeParaPut(&PokeParaMine[SelectNo],ID_get_level,
						&ServerBuffer[ClientNo][BSS_DATASTART]); 
			break;
		case BSS_PARA_GET_CASSETTE:		//�ߊl�J�Z�b�g
			PokeParaPut(&PokeParaMine[SelectNo],ID_get_cassette,
						&ServerBuffer[ClientNo][BSS_DATASTART]); 
			break;
		case BSS_PARA_GET_BALL:			//�ߊl�{�[��
			PokeParaPut(&PokeParaMine[SelectNo],ID_get_ball,
						&ServerBuffer[ClientNo][BSS_DATASTART]); 
			break;
		case BSS_PARA_POWERRND:
			PokeParaPut(&PokeParaMine[SelectNo],ID_hp_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_pow_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+1]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_def_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+2]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_agi_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+3]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_spepow_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+4]);
			PokeParaPut(&PokeParaMine[SelectNo],ID_spedef_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+5]);
			break;
		case BSS_PARA_HPRND:			//HP�p���[����
			PokeParaPut(&PokeParaMine[SelectNo],ID_hp_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_POWRND:			//�U���̓p���[����
			PokeParaPut(&PokeParaMine[SelectNo],ID_pow_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_DEFRND:			//�h��̓p���[����
			PokeParaPut(&PokeParaMine[SelectNo],ID_def_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_AGIRND:			//�f�����p���[����
			PokeParaPut(&PokeParaMine[SelectNo],ID_agi_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEPOWRND:		//����U���̓p���[����
			PokeParaPut(&PokeParaMine[SelectNo],ID_spepow_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEDEFRND:		//����h��̓p���[����
			PokeParaPut(&PokeParaMine[SelectNo],ID_spedef_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CHRRND:			//������
			PokeParaPut(&PokeParaMine[SelectNo],ID_personal_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			break;
		case BSS_PARA_SUM:				//�`�F�b�N�T��
			PokeParaPut(&PokeParaMine[SelectNo],ID_checksum,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CONDITION:		//�R���f�B�V����
			PokeParaPut(&PokeParaMine[SelectNo],ID_condition,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_LEVEL:			//���x��
			PokeParaPut(&PokeParaMine[SelectNo],ID_level,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_HP:				//�g�o�c��
			PokeParaPut(&PokeParaMine[SelectNo],ID_hp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_HPMAX:			//�g�o�l�`�w
			PokeParaPut(&PokeParaMine[SelectNo],ID_hpmax,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_POW:				//�U����
			PokeParaPut(&PokeParaMine[SelectNo],ID_pow,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_DEF:				//�h���
			PokeParaPut(&PokeParaMine[SelectNo],ID_def,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_AGI:				//�f����
			PokeParaPut(&PokeParaMine[SelectNo],ID_agi,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEPOW:			//����U����
			PokeParaPut(&PokeParaMine[SelectNo],ID_spepow,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEDEF:			//����h���
			PokeParaPut(&PokeParaMine[SelectNo],ID_spedef,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_STYLE:			//�������悳
			PokeParaPut(&PokeParaMine[SelectNo],ID_style,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_BEAUTIFUL:		//������
			PokeParaPut(&PokeParaMine[SelectNo],ID_beautiful,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CUTE:				//���킢��
			PokeParaPut(&PokeParaMine[SelectNo],ID_cute,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CLEVER:			//����
			PokeParaPut(&PokeParaMine[SelectNo],ID_clever,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_STRONG:			//�����܂���
			PokeParaPut(&PokeParaMine[SelectNo],ID_strong,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_FUR:				//�щ�
			PokeParaPut(&PokeParaMine[SelectNo],ID_fur,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_STYLEMEDAL:		//�������悳�M��
			PokeParaPut(&PokeParaMine[SelectNo],ID_stylemedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_BEAUTIFULMEDAL:	//�������M��
			PokeParaPut(&PokeParaMine[SelectNo],ID_beautifulmedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CUTEMEDAL:		//���킢���M��
			PokeParaPut(&PokeParaMine[SelectNo],ID_cutemedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CLEVERMEDAL:		//�����M��
			PokeParaPut(&PokeParaMine[SelectNo],ID_clevermedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_STRONGMEDAL:		//�����܂����M��
			PokeParaPut(&PokeParaMine[SelectNo],ID_strongmedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
	}
	PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
}

//----------------------------------------------------------
//	�w�肵�����ڽ���߹���ް���ύX����
//----------------------------------------------------------

void ClientDBSSPokeparaPut2(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------

void ClientDBSSPokeEncount(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ްق����яo��)
//----------------------------------------------------------

void ClientDBSSPokeAppear(void)
{
	ClientDBitReset();
}


//----------------------------------------------------------
//	�߹�݂�߂��̪�Ă��N������
//----------------------------------------------------------
void ClientDBSSPokeReturn(void)
{
	if(ServerBuffer[ClientNo][1] == 0)
	{
		SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_MINE);
		ClientSequence[ClientNo] = ClientDPokeReturnWait;
	}
	else
	{
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientDBitReset();
	}
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------
#define TRAINER_IN_DX	(200)		//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_IN_SP	(-4)	//�݂��Ă���Ƃ��̽�߰��

void ClientDBSSTrainerEncount(void)
{
	TrainerBObjCellPalSet(CELLID_tr3b_019, ClientNo);

//	PokeActHeaderMake(CELLID_tr3b_019+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(CELLID_tr3b_019,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							C_TRN_X,
							C_TRN_Y+4*(8-trbpara[CELLID_tr3b_019].size),
							C_TRN_PRI);	//�ڰŰ

	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = ENCOUNT_DX;
	ActWork[PokemonStruct[ClientNo]].work[0] = -ENCOUNT_SPEED;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;

	ClientSequence[ClientNo] = ClientDTrainerEncountWait;
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ΐ�I����)
//----------------------------------------------------------
#define TRAINER_AP_IN_DX (-96)//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_AP_IN_SP	(2)	//�݂��Ă���Ƃ��̽�߰��

void ClientDBSSTrainerAppear(void)
{
	TrainerBObjCellPalSet(CELLID_tr3b_019, ClientNo);

//	PokeActHeaderMake(MyData.my_sex+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(CELLID_tr3b_019,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							C_TRN_X,
							C_TRN_Y+4*(8-trbpara[CELLID_tr3b_019].size),
							C_TRN_PRI);	//�ڰŰ

	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = TRAINER_AP_IN_DX;
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_AP_IN_SP;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;

	ClientSequence[ClientNo] = ClientDTrainerAppearWait;
}

//----------------------------------------------------------
//	�ڰŰ��߂��̪�Ă��N������
//----------------------------------------------------------


void ClientDBSSTrainerReturn(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�߹�݂��C�₷��̪�Ă��N������
//----------------------------------------------------------

void ClientDBSSPokeKizetu(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�߹�݂��S�ł����Ƃ��̴̪�Ă��N������
//----------------------------------------------------------

void ClientDBSSZenmetsu(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl�����̴̪�Ă��o��
//----------------------------------------------------------

void ClientDBSSGetSuccess(void)
{
//	ClientStruct[ClientNo] = MonsterBallSet(0,4);
//	ClientSequence[ClientNo] = ClientBallEffectWait;
//	ClientDBitReset();
//	ClientStruct[ClientNo] = MonsterBallSet(0,4);
	BtlWork->get_rate = 4;
	ClientEffectWait = 1;	//�ްٹޯĴ̪�Ă͏I��������Ō��Ă�
	SetEtcEffect(ClientNo,ClientNo,ClientNoGet(BSS_CLIENT_ENEMY),FE_ID_POKEGET_SAFARI);

	ClientSequence[ClientNo] = ClientBallEffectWait;
//	ClientSBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl���s�̴̪�Ă��o��
//----------------------------------------------------------

void ClientDBSSGetFail(void)
{
	u8 rate;
	
//	rate = ServerBuffer[ClientNo][1];
//	ClientStruct[ClientNo] = MonsterBallSet(0, rate);
//	ClientSequence[ClientNo] = ClientBallEffectWait;
//	ClientDBitReset();
	rate = ServerBuffer[ClientNo][1];
//	ClientStruct[ClientNo] = MonsterBallSet(0, rate);
	BtlWork->get_rate = rate;
	ClientEffectWait = 1;	//�ްٹޯĴ̪�Ă͏I��������Ō��Ă�
	SetEtcEffect(ClientNo, ClientNo,ClientNoGet(BSS_CLIENT_ENEMY),FE_ID_POKEGET_SAFARI);

	ClientSequence[ClientNo] = ClientBallEffectWait;
//	ClientSBitReset();
}

//----------------------------------------------------------
//	�߹�����ٱ��߂̴̪�Ă��o��
//----------------------------------------------------------

void ClientDBSSPokeLevelUp(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�w�肵���Z���ް�̴̪�Ă��o��
//----------------------------------------------------------

enum{
	C_MIGAWARI_CHK,
	C_EFF_PUTCHK,
	C_EFF_WAIT,
	C_EFF_END,
};

void ClientDBSSWazaEffect(void)
{
	u16 wazano;

	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	WazaCounter = ServerBuffer[ClientNo][3];
	WazaEffPower = ServerBuffer[ClientNo][4] | (ServerBuffer[ClientNo][5]<<8);
	WazaEffDamage = ServerBuffer[ClientNo][6] | (ServerBuffer[ClientNo][7]<<8)
		| (ServerBuffer[ClientNo][8]<<16) | (ServerBuffer[ClientNo][9]<<24);
//	WazaEffNatukido = ServerBuffer[ClientNo][10] | (ServerBuffer[ClientNo][11]<<8);
	WazaEffNatukido = ServerBuffer[ClientNo][10] ;
	WazaEffWeather = ServerBuffer[ClientNo][12] | (ServerBuffer[ClientNo][13]<<8);
	pwkc = (WazaKoukaCount*)&ServerBuffer[ClientNo][16];

	HenshinRnd[ClientNo]=pwkc->henshin_rnd;	//�ւ񂵂�p�̌�����
	
	if(PutWazaEffectCheck(wazano, WazaCounter)){
		ClientDBitReset();
		return;
	}
	
	CliSys[ClientNo].WazaEffSeqNo = C_MIGAWARI_CHK;
	ClientSequence[ClientNo] = ClientDWazaEffectSet;
}

static void ClientDWazaEffectSet(void)
{
	u16 wazano;
	
	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case C_MIGAWARI_CHK:
			if(Climem[ClientNo].migawari == 1)	//�u�݂����v�`�F�b�N
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			CliSys[ClientNo].WazaEffSeqNo = C_EFF_PUTCHK;
			break;
		case C_EFF_PUTCHK:					//�Z�G�t�F�N�g�Z�b�g
			if(CliSys[ClientNo].etc_eff)
				break;
			
//			if(FightCheck() && (AttackClient^2) == DefenceClient && WazaDamage > 0)
//				wazano = WAZANO_MAX+1;	//�����ւ̍U���Ȃ�΁A�ȈՓI�ȴ̪�Ă��o��
			
			PokeAffineON_OFF(0);
			WazaEffectSet(wazano);
			CliSys[ClientNo].WazaEffSeqNo = C_EFF_WAIT;
			break;
		case C_EFF_WAIT:					//�Z�G�t�F�N�g�I���҂�
			WazaEffectSequence();
			if(WazaEffectFlag==0){
				PokeAffineON_OFF(1);
				if(Climem[ClientNo].migawari)	//�g����`�F�b�N
					SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
				CliSys[ClientNo].WazaEffSeqNo = C_EFF_END;
			}
			break;
		case C_EFF_END:
			if(CliSys[ClientNo].etc_eff)
				break;

			//�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
			PokeBanishMemory();
			wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
			MigawariFlgCheckSet(ClientNo, wazano);
			CliSys[ClientNo].WazaEffSeqNo = 0;
			ClientDBitReset();
			break;
	}
}

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

void ClientDBSSMessage(void)
{
	u16	*MsgID;

	ScrX0=0;
	ScrY0=0;

	MsgID=(u16 *)&ServerBuffer[ClientNo][2];
	FightMsgID(MsgID[0]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientDMessageWait;
}

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

void ClientDBSSAlertMessage(void)
{
	if(MineEnemyCheck(ClientNo)==SIDE_MINE)
		ClientDBSSMessage();
	else
		ClientDBitReset();
}

//----------------------------------------------------------
//	����ޑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

void ClientDBSSCommandSelect(void)
{
	int	i;

	ScrX0=0;
	ScrY0=0x0a0;

	FightMsg.palette=0;
	FontScrnBoxFill(&FightMsg,SPACE_CHR_D,2,15,2+25,15+3);	//add 2001.08.07(��)
	FontScrnBoxFill(&FightMsg,SPACE_CHR_D,2,35,2+14,35+1);	//add 2001.08.07(��)
	ClientSequence[ClientNo]=ClientDCommandSelect;
	MsgPutSet(&FightMsg, CommandMsg, CGX_START2+0x10,  19, 35);
	MsgPut(&FightMsg);
	for(i=0;i<4;i++) CommandCsrOff(i);
	CommandCsrOn(0,0);
	FightMsgCreate((u8 *)&DemoDousuruMsg);
	MsgPutSet(&FightMsg, FightMsgBuf, CGX_START2+0x38,  2, 35);
	MsgPut(&FightMsg);
}

//----------------------------------------------------------
//	�w�肳�ꂽClientNo�̃V�[�P���X�i���o�[�Ɠ������Ƃ�
//----------------------------------------------------------

void ClientDBSSCommandSelectCopy(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�Z�I����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

enum{
	WAZASEL_SEQ_INIT=0,
	WAZASEL_SEQ_WAZA_SELECT,
};

void ClientDBSSWazaSelect(void)
{
	switch(UserWork[GET_DEMO_WAZA_SEL]){
		case WAZASEL_SEQ_INIT:
			ClientBSSWazaSelectAct();
			UserWork[GET_DEMO_WAZA_SEL]++;
			UserWork[GET_DEMO_WORK1]=80;
		case WAZASEL_SEQ_WAZA_SELECT:
			if(--UserWork[GET_DEMO_WORK1]==0){
				SePlay(SE_SELECT);
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,0x100);
				ClientDBitReset();
			}
			break;
	}
}

//----------------------------------------------------------
//	���ёI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

void ClientDBSSItemSelect(void)
{
	PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
	ClientSequence[ClientNo] = ClientItemWait;
	TempClientNo = ClientNo;	//add 2001.12.21(��)
}

//----------------------------------------------------------
//	�߹�ݑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

void ClientDBSSPokeSelect(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	������̪�Ă��N������
//----------------------------------------------------------

void ClientDBSSEscape(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	HP�ް�ނ𑝌�����
//----------------------------------------------------------

void	ClientDBSSHpGauge(void)
{
	s16 beHP;	//��������l

	BattleGaugeTransWram(0);
	beHP = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3] << 8);
	if(beHP!=JIBAKU_HP)
		GaugeDataSet(ClientNo, ClientGaugeSt[ClientNo],
		PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_hpmax),
		PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_hp), beHP);
	else{
		GaugeDataSet(ClientNo, ClientGaugeSt[ClientNo],
		PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_hpmax),
		0,beHP);
		
		GaugeHPTrans(ClientGaugeSt[ClientNo], 0, 0);	//HP�̐��l��0�ɂ���
	}
	ClientSequence[ClientNo] = ClientDHPGaugeWait;
}

//----------------------------------------------------------
//	EXP�ް�ނ𑝌�����
//----------------------------------------------------------
void ClientDBSSExpGauge(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ�̪�Ă��N������
//----------------------------------------------------------

void ClientDBSSStatusSet(void)
{
	ClientDBitReset();
}

void ClientDBSSStatusEffect(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ��ر����
//----------------------------------------------------------

void ClientDBSSStatusClear(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	���炩���ߗp�ӂ��ꂽ�ޯ̧���ް����i�[����
//----------------------------------------------------------

void ClientDBSSDataTrans(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�w����ڽ(RAM,VRAM,CGRAM)���ް���]������
//----------------------------------------------------------

void ClientDBSSDataRamTrans(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	���ް���]������
//----------------------------------------------------------

void ClientDBSSBgmTrans(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�����ް���]������
//----------------------------------------------------------

void ClientDBSSVoiceTrans(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�ƭ��őI����������ID��Ԃ�
//----------------------------------------------------------

void ClientDBSSSelectReturn(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�߹�ݓ���ւ����s�������Ƃ�m�点��
//----------------------------------------------------------

void ClientDBSSReshuffle(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	���юg�p��m�点��
//----------------------------------------------------------

void ClientDBSSUseItem(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�ްَg�p��m�点��
//----------------------------------------------------------

void ClientDBSSUseBall(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃N���A
//----------------------------------------------------------

void	ClientDBSSLoopCountClear(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃Z�b�g
//----------------------------------------------------------

void	ClientDBSSLoopCountSet(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃N���A
//----------------------------------------------------------

void	ClientDBSSSwitchFlagClear(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃Z�b�g
//----------------------------------------------------------

void	ClientDBSSSwitchFlagSet(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	��Ұ�ނ��󂯂��Ƃ����߹�ݓ_��
//----------------------------------------------------------

void ClientDBSSPokeDamageBlink(void)
{
	//�߹�݂���Ư����ԂȂ��Blink���Ȃ�
	if(ActWork[PokemonStruct[ClientNo]].banish == 1){
		ClientDBitReset();
		return;
	}

	ClientEffectWait = 1;	//Blink�̪�ĊJ�n�t���O�Z�b�g
	ActWork[PokemonStruct[ClientNo]].work[1] = 0;
	
	//-- �ް�ނ̃J���[�ύX & �h�炵�G�t�F�N�g�Z�b�g --//
	GaugeSwayEffect(ClientNo);
	
	ClientSequence[ClientNo] = ClientDPokeDamageBlinkWait;
}

//----------------------------------------------------------
//	�Q�[�W������
//----------------------------------------------------------

void ClientDBSSGaugeOff(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	SE��炷
//----------------------------------------------------------

void ClientDBSSSEPlay(void)
{
	SePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	ClientDBitReset();
}

//----------------------------------------------------------
//	ME��炷
//----------------------------------------------------------

void ClientDBSSMEPlay(void)
{
	JinglePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	ClientDBitReset();
}

//----------------------------------------------------------
//	�Ȃ�������炷
//----------------------------------------------------------

void ClientDBSSVoicePlay(void)
{
	VoicePlay(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno),25);
	ClientDBitReset();
}

//----------------------------------------------------------
//	�G���J�E���g�G�t�F�N�g���Z�b�g
//----------------------------------------------------------

void ClientDBSSEncountEffect(void)
{
	EncountEffectSet(ServerBuffer[ClientNo][1]);
	//OBJ���ړ��̃X�g�b�p�[
	EncountEffectFlag|=ENCOUNT_EFFECT_FLAG_OBJSTOP;	
	ClientDBitReset();
}

//----------------------------------------------------------
//	�ŏ��̃|�P�����o��(�ڰŰ�������ށA�߹�ݏo���Aү���ޏo���B�܂ł̈�A�̓���
//----------------------------------------------------------

void ClientDBSSPokeSendOut(void)
{
	u8 id;
	u8 palno;
	
	//-- �ڰŰ --//
	DxDyAddClear(&ActWork[PokemonStruct[ClientNo]]);
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_RETURN_SYNCNUM;	//VsyncNum
	ActWork[PokemonStruct[ClientNo]].work[2] = -40;
	ActWork[PokemonStruct[ClientNo]].work[4] = ActWork[PokemonStruct[ClientNo]].y;
	ActWork[PokemonStruct[ClientNo]].move = W_StraightSyncMove;
	ActWork[PokemonStruct[ClientNo]].work[5] = ClientNo;
	SetWorkSeq(&ActWork[PokemonStruct[ClientNo]], TrainerDelEnd);
	ActAnmChg(&ActWork[PokemonStruct[ClientNo]],1);
	
	//SendOut�̎������߹�݂���گĂƂ��Ԃ�Ȃ��悤��PalSet���g�p����
	palno = ObjPalNumSet(MINE_TRAINER_PALID);
	DecordPaletteWork((void*)TRBObjDataPal[2].pal_data,
		PA_OBJ0 + 0x10 * palno, 0x20);
	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = palno;

	//-- �߹�� --//
	id = AddTask(SendOutTask, 5);
	TaskTable[id].work[0] = ClientNo;

	//-- �莝���ް�ނ����� --//
	if(CliSys[ClientNo].temoti_gauge_flg)
		TaskTable[TemotiGauge[ClientNo]].TaskAdrs = TemotiBallOff;
	
	BtlWork->sendout_eff = 1;
	ClientSequence[ClientNo] = ClientDummy;
}

static void PokeAppearCommon(u8 client_no)
{
	u16 monsno;

	Climem[client_no].hensin_monsno = 0;	//�ϐg�׸޸ر

	SelMonsNo[client_no] = ServerBuffer[client_no][1];
	monsno=PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_monsno);

	ClientStruct[client_no] = AddBanishActor(CA_PokeAppearWait);
	
#if 0	// 2002.07.04(��) SendOut�ł��ڰŰ��Del���鎞�ɁAPokeAppear�̎���Common�̑O�ɂ���Ă��܂��悤�ɕύX�����B
	PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[client_no]], client_no);
#endif
	PokeActHeaderMake(monsno,ClientTypeGet(client_no));

#if 0
	PokemonStruct[client_no] = AddActor(&PokeActHeader,
							PokeMinePosX[FightCheck()][ClientTypeGet(client_no)>>1],
							PokeMinePosY[FightCheck()][ClientTypeGet(client_no)>>1]+
//							4*(8-pm2bpara[monsno].size),
							pm2bpara[monsno].patcnt,
							C_POKE1PRI);	//�߹��
#else
	PokemonStruct[client_no]=
		AddActor(&PokeActHeader,
				 GetWazaEffPos(client_no,PM_X),
				 GetPokemonYPos(client_no),
				 GetPokeSoftPri(client_no));	//�߹��
#endif

	ActWork[ClientStruct[client_no]].work[1] = PokemonStruct[client_no];
	ActWork[PokemonStruct[client_no]].work[0] = client_no;	//22
	ActWork[PokemonStruct[client_no]].work[2] = monsno;
	ActWork[PokemonStruct[client_no]].oamData.Pltt = client_no;
	ActAnmChg(&ActWork[PokemonStruct[client_no]],BattlePokeAnmNo[client_no]);
	
#if 0	//changed by matsuda 2002.01.11(��)
	ActWork[ClientStruct[client_no]].work[0] = 
		MBSmokeSet(PokeMinePosX[FightCheck()][client_no>>1], C_POKE1Y+20, 1);
	CAC_PokeAppearDifferentInit(&ActWork[PokemonStruct[client_no]], 0);
#else
	ActWork[PokemonStruct[client_no]].banish = 1;
	ActWork[PokemonStruct[client_no]].move = DummyActMove;
	ActWork[ClientStruct[client_no]].work[0] = MonsterBallSet(0,0xff);
#endif
}

static void SendOutTask(u8 id)
{
	u8 clientno_temp;
	
	if(TaskTable[id].work[1] < 31){	//24){
		TaskTable[id].work[1]++;
		return;
	}
	
	clientno_temp = ClientNo;
	ClientNo = TaskTable[id].work[0];
	ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
	PokeAppearCommon(ClientNo);	//ClientBSSPokeAppear();
	ClientSequence[ClientNo] = ClientDPokeSendOutWait;
	ClientNo = clientno_temp;
	DelTask(id);
}


//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W��\������
//----------------------------------------------------------

void	ClientDBSSBallGaugeSet(void)
{
	BallGaugePara *bgp;
	
//�t���O���[�N��true�̎��́A�ǂ����T�C�h�Ȃ̂��`�F�b�N���āA�������̂Ƃ��͕\�������Ȃ��悤�ɂ���
	if(ServerBuffer[ClientNo][1]){
		if(MineEnemyCheck(ClientNo)==SIDE_MINE){
			ClientDBitReset();
			return;
		}
	}
	CliSys[ClientNo].temoti_gauge_flg = 1;
	bgp = (BallGaugePara*)&ServerBuffer[ClientNo][4];
	TemotiGauge[ClientNo] = TemotiBallPut(ClientNo, bgp, ServerBuffer[ClientNo][1], ServerBuffer[ClientNo][2]);
	ClientDBitReset();
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W���\���ɂ���
//----------------------------------------------------------

void	ClientDBSSBallGaugeOff(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�Q�[�W�̂����~�߂�
//----------------------------------------------------------

void ClientDBSSGaugeYureStop(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�|�P������VANISH��OFF����
//----------------------------------------------------------

void	ClientDBSSVanishOff(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�퓬�G�t�F�N�g���o��
//----------------------------------------------------------

void	ClientDBSSFightEffect(void)
{
	u8 eff_no;
	u16 monsno;
	
	eff_no = ServerBuffer[ClientNo][1];
	monsno = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8);
	
	if(SetFightEffect(ClientNo, ClientNo, ClientNo, eff_no, monsno))
		ClientDBitReset();
	else
		ClientSequence[ClientNo] = ClientDFightEffWait;
}

//----------------------------------------------------------
//	�ʐM�ҋ@��ʂ��o��
//----------------------------------------------------------

void	ClientDBSSTuushinWait(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�L�����Ă���J�[�\���ʒu�̃N���A
//----------------------------------------------------------

void	ClientDBSSCsrPosClear(void)
{
	ClientDBitReset();
}

//----------------------------------------------------------
//	�ΐ�I����m�点��
//----------------------------------------------------------

void ClientDBSSFightEnd(void)
{
	WinLoseFlag=ServerBuffer[ClientNo][1];
	BGM_FadeOutSet(5);
	LightFadeReq(N_to_B);
	ClientDBitReset();
	if((FightType&FIGHT_TYPE_TUUSHIN)&&((FightType&FIGHT_TYPE_OYA)==0))
		ClientSequence[ClientNo] = ClientFightEndWait;
}

//----------------------------------------------------------
//	��а
//----------------------------------------------------------
void ClientDBSSEndID(void)
{
//	ClientDBitReset();
}

