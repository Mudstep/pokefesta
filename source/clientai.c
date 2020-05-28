
//======================================================
//    clientai.c                                           
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
#include "field.h"
#include "ev_fight.h"
#include "waza_tbl.h"
#include "trainer.h"
#include "pm_aff.h"
#include "cli_num.h"

#include "multisio.h"		//add
#include "pokesio.h"
#include "cable.h"

#include "fight_ef.h"

#include "waza_ai.h"
#include "waza_ai.def"
#include "mus_tool.h"
#include "fld_main.h"
#include "task.h"
#include "fightmsg.h"
#include "trfenum.h"
#include "wazano.def"
#include "b_tower.h"
#include "waza_dat.h"
#include "a_fight.h"
#include "monsno.def"
#include "tokusyu.def"

#include "item.h"

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

void	ClientAIBitReset(void);	//�͈͊O�̒l�̎��̏���
void	ClientAIDummy(void);
void	ClientAIInit(void);
void	ClientAICommandWait(void);
void	ClientAIPokeEncountWait(void);
void	ClientAIPokeAppearWait(void);

void	ClientAICommandSelect(void);
void	ClientAIWazaSelect(void);
void	ClientAIHpGaugeWait(void);
void	ClientAIPokeKizetsuWait(void);
void	ClientAIMessageWait(void);

void	ClientAICommandSelectAI(void);
u8		ClientAIPokeReshuffleAI(void);
u8		ClientAIPokeSelectAI(void);
u8		ClientAIItemUseAI(void);

u8		ClientAIHorobinoutaCheck(void);
u8		ClientAIHusiginamamoriCheck(void);
u8		ClientAIHPRecoverTokusyuCheck(void);
u8		ClientAIShizenkaifukuCheck(void);
u8		ClientAIBatsugunCheck(u8);
u8		ClientAIStatusUpCheck(void);
u8		ClientAIWazaKoukaCheck(u8,u8);
u8		ClientAIWazaKoukaCheck(u8,u8);

void ClientAIBSSPokeparaGet(void);
void ClientAIBSSPokeparaGet2(void);
void ClientAIBSSPokeparaPut(void);
void ClientAIBSSPokeparaPut2(void);
void ClientAIBSSPokeEncount(void);
void ClientAIBSSPokeAppear(void);
void ClientAIBSSPokeReturn(void);
void ClientAIBSSTrainerEncount(void);
void ClientAIBSSTrainerAppear(void);
void ClientAIBSSTrainerReturn(void);
void ClientAIBSSPokeKizetu(void);
void ClientAIBSSZenmetsu(void);
void ClientAIBSSGetSuccess(void);
void ClientAIBSSGetFail(void);
void ClientAIBSSPokeLevelUp(void);
void ClientAIBSSWazaEffect(void);
void ClientAIBSSMessage(void);
void ClientAIBSSAlertMessage(void);
void ClientAIBSSCommandSelect(void);
void ClientAIBSSCommandSelectCopy(void);
void ClientAIBSSWazaSelect(void);
void ClientAIBSSItemSelect(void);
void ClientAIBSSPokeSelect(void);
void ClientAIBSSEscape(void);
void ClientAIBSSHpGauge(void);
void ClientAIBSSExpGauge(void);
void ClientAIBSSStatusSet(void);
void ClientAIBSSStatusEffect(void);
void ClientAIBSSStatusClear(void);
void ClientAIBSSDataTrans(void);
void ClientAIBSSDataRamTrans(void);
void ClientAIBSSBgmTrans(void);
void ClientAIBSSVoiceTrans(void);
void ClientAIBSSSelectReturn(void);
void ClientAIBSSReshuffle(void);
void ClientAIBSSUseItem(void);
void ClientAIBSSUseBall(void);
void ClientAIBSSLoopCountClear(void);
void ClientAIBSSLoopCountSet(void);
void ClientAIBSSSwitchFlagClear(void);
void ClientAIBSSSwitchFlagSet(void);
void ClientAIBSSPokeDamageBlink(void);
void ClientAIBSSGaugeOff(void);
void ClientAIBSSSEPlay(void);
void ClientAIBSSMEPlay(void);
void ClientAIBSSVoicePlay(void);
void ClientAIBSSEncountEffect(void);
void ClientAIBSSPokeSendOut(void);
void ClientAIBSSBallGaugeSet(void);
void ClientAIBSSBallGaugeOff(void);
void ClientAIBSSGaugeYureStop(void);
void ClientAIBSSVanishOnOff(void);
void ClientAIBSSFightEffect(void);
void ClientAIBSSFightEnd(void);
void ClientAIBSSTuushinWait(void);
void ClientAIBSSCsrPosClear(void);
void ClientAIBSSEndID(void);

int	ClientAIBSSPokeparaGetAct(u8,u8 *);
void ClientAIBSSPokeparaPutAct(u8);
static void AIPokeAppearCommon(u8 client_no, u8);
static void AISendOutTask(u8 id);
static void AITrainerDelEnd(actWork *xreg);
static void ClientAIWazaEffectSet(void);
static void ClientAIAppearMigawariEffWait(void);
static void ClientAIBallGaugeWait(void);
static void ClientAIPokeReturnMigawariChk(void);

extern	actHeader PokeFTbl[];
extern	TALK_WIN	FightMsg;
extern	const	u8	TypeCheckTable[];
//extern	u8	FightMsgBuf[64];


//----------------------------------------------------------
//	�o�g���T�[�o�[���ߌQ�e�[�u��
//----------------------------------------------------------
const pFunc ClientAIBSSTbl[] =
{
	ClientAIBSSPokeparaGet,		// BSS_POKEPARA_GET=0,	
	ClientAIBSSPokeparaGet2,	   	// BSS_POKEPARA_GET2,	
	ClientAIBSSPokeparaPut,		// BSS_POKEPARA_PUT,	
	ClientAIBSSPokeparaPut2,		// BSS_POKEPARA_PUT2,	
	ClientAIBSSPokeEncount,		// BSS_POKEPARA_ENCOUNT,	add
	ClientAIBSSPokeAppear,		// BSS_POKE_APPEAR,	
	ClientAIBSSPokeReturn,		// BSS_POKE_RETURN,	
	ClientAIBSSTrainerEncount,	// BSS_TRAINER_ENCOUNT,	add
	ClientAIBSSTrainerAppear,		// BSS_TRAINER_APPEAR,	
	ClientAIBSSTrainerReturn,		// BSS_TRAINER_RETURN,	
	ClientAIBSSPokeKizetu,		// BSS_POKE_KIZETSU,	
	ClientAIBSSZenmetsu,			// BSS_POKE_ZENMETSU,	
	ClientAIBSSGetSuccess,		// BSS_GET_SUCCESS,	
	ClientAIBSSGetFail,			// BSS_GET_FAIL,		
	ClientAIBSSPokeLevelUp,		// BSS_POKE_LEVELUP,	
	ClientAIBSSWazaEffect,		// BSS_WAZA_EFFECT,	
	ClientAIBSSMessage,			// BSS_MESSAGE,		
	ClientAIBSSAlertMessage,	// BSS_ALERT_MESSAGE,		
								
	ClientAIBSSCommandSelect,		// BSS_COMMAND_SELECT, 

	ClientAIBSSCommandSelectCopy,		// BSS_COMMAND_SELECT_COPY, 

	ClientAIBSSWazaSelect,		// BSS_WAZA_SELECT,	
													
	ClientAIBSSItemSelect,		// BSS_ITEM_SELECT,	
													
	ClientAIBSSPokeSelect,		// BSS_POKE_SELECT,	
													
	ClientAIBSSEscape,			// BSS_ESCAPE,			
	ClientAIBSSHpGauge,			// BSS_HP_GAUGE,		
	ClientAIBSSExpGauge,			// BSS_EXP_GAUGE,		
	ClientAIBSSStatusSet,			// BSS_STATUS_SET,		
	ClientAIBSSStatusEffect,		// BSS_STATUS_EFFECT,		
	ClientAIBSSStatusClear,		// BSS_STATUS_CLEAR,	
	ClientAIBSSDataTrans,			// BSS_DATA_TRANS,		
	ClientAIBSSDataRamTrans,		// BSS_DATA_RAM_TRANS,	
													
	ClientAIBSSBgmTrans,			// BSS_BGM_TRANS,		
	ClientAIBSSVoiceTrans,		// BSS_VOICE_TRANS,	
	ClientAIBSSSelectReturn,		// BSS_SELECT_RETURN,	
	ClientAIBSSReshuffle,			// BSS_POKE_RESHUFFLE,	
	ClientAIBSSUseItem,			// BSS_USE_ITEM,		
	ClientAIBSSUseBall,			// BSS_USE_BALL,		
	ClientAIBSSLoopCountClear,	//	BSS_LOOPCOUNT_CLEAR
	ClientAIBSSLoopCountSet,	//	BSS_LOOPCOUNT_SET
	ClientAIBSSSwitchFlagClear,	//	BSS_SWITCHFLAG_CLEAR
	ClientAIBSSSwitchFlagSet,	//	BSS_SWITCHFLAG_SET
	ClientAIBSSPokeDamageBlink,	// BSS_POKEDAMAGE_BLINK,
	ClientAIBSSGaugeOff,		// BSS_GAUGE_OFF,
	ClientAIBSSSEPlay,			// BSS_SE_PLAY,
	ClientAIBSSMEPlay,			// BSS_SE_PLAY,
	ClientAIBSSVoicePlay,			// BSS_VOICE_PLAY,
	ClientAIBSSEncountEffect,	// BSS_ENCOUNT_EFFECT,
	ClientAIBSSPokeSendOut,		// BSS_POKE_SENDOUT,
	ClientAIBSSBallGaugeSet,	// BSS_BALL_GAUGE_SET,
	ClientAIBSSBallGaugeOff,	// BSS_BALL_GAUGE_OFF,
	ClientAIBSSGaugeYureStop,	// BSS_BALL_GAUGE_OFF,
	ClientAIBSSVanishOnOff,		// BSS_VANISH_OFF,
	ClientAIBSSFightEffect,		// BSS_FIGHT_EFFECT,
	ClientAIBSSTuushinWait,		// BSS_TUUSHIN_WAIT,
	ClientAIBSSCsrPosClear,		// BSS_CSRPOS_CLEAR,
	ClientAIBSSFightEnd,		// BSS_FIGHT_END,

	ClientAIBSSEndID,				// BSS_END_ID,		����Ȃ����ǈꉞ
};

//----------------------------------------------------------
//	�v���g�^�C�v�錾	�֐����ް�
//----------------------------------------------------------

#define	CAI_LVNUM_X			84
#define	CAI_LVNUM_Y			28

//==============================================================
//                       ���C�����[�`��
//==============================================================

void	ClientAIDummy(void)
{
}

void	ClientAIInit(void)
{
	ClientSequence[ClientNo]=ClientAICommandWait;
	#if 0
	//�ް�޷�� ýĂȂ̂łƂ肠�����A���ۂ͏풓�Ȃ̂ł����ł͂��Ȃ��@��
	
	ClientGaugeSt[ClientNo] = GaugeActorSet(ClientNo);	//�ް�ޱ����o�^ ������Ƃ肠����������
	GaugeDefaultPosSet(ClientNo);
	GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaEnemy[SelMonsNo[ClientNo]], G_ALL);
	GaugeOFF(ClientGaugeSt[ClientNo]);
	if(ClientNo == BSS_CLIENT_ENEMY)
		GaugePriSet(ClientGaugeSt[ClientNo], 0);
	else
		GaugePriSet(ClientGaugeSt[ClientNo], 1);
	#endif
}

void	ClientAICommandWait(void)
{
	if((ClientBit&no2bittbl[ClientNo]) != 0)
	{
		if(ServerBuffer[ClientNo][0] <= BSS_END_ID)// && ServerBuffer[ClientNo][0] > -1)
		{
			ClientAIBSSTbl[ServerBuffer[ClientNo][0]]();
		}
		else
		{
			ClientAIBitReset();	//�͈͊O�̒l�̎��̏���
		}
	}
}

//=========================================================================
//	�E�F�C�g���[�`��
//=========================================================================
//
void ClientAITrainerEncountWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientAIBitReset();
	}
}

void ClientAITrainerAppearWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientAIBitReset();
	}
}

void ClientAITrainerReturnWait(void)
{
#if 0	//chaged by matsuda 2002.01.15(��)
	if(ActWork[PokemonStruct[ClientNo]].dx > 64)
#else
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
#endif
	{
		TrainerFObjCellPalDel(ActWork[PokemonStruct[ClientNo]].oamData.AffineParam);
		ActWork[PokemonStruct[ClientNo]].oamData.CharNo //DelActor���鎞�ɓ]����
			= ActWork[PokemonStruct[ClientNo]].work[5];//����Ă����̈悪�J�������悤��CharNo�����ɖ߂�
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);	//�ꉞ(�Ă�ł����͂Ȃ���)
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		ClientAIBitReset();
	}
}

void ClientAISendOutWaitBitReset(void)
{
	if(CliSys[ClientNo].send_out_wait-- == 0){
		CliSys[ClientNo].send_out_wait = 0;
		ClientAIBitReset();
	}
}

void ClientAISendOutGaugeWait(void)
{
#if 0	//2002.05.02(��)
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove
		&& ActWork[ClientGaugeSt[ClientNo^2]].move == DummyActMove)
	{
		ClientAIBitReset();
	}
#else
	u8 end = 0;
	
	if(FightCheck() == 0 || (FightCheck() && (FightType & FIGHT_TYPE_4_2vs2)))
	{	//1vs1�ΐ� ���� 4�l�ʐM
		if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove)
			end = 1;
	}
	else
	{	//2vs2�ΐ�
		if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove
				&& ActWork[ClientGaugeSt[ClientNo^2]].move == DummyActMove)
			end = 1;
	}

	if(VoiceEndCheck())	//add 2002.09.30(��)
		end = 0;
	
	if(end && CliSys[ClientNo].rare_eff_end && CliSys[ClientNo^2].rare_eff_end){
		CliSys[ClientNo].rare_eff = 0;
		CliSys[ClientNo].rare_eff_end = 0;
		CliSys[ClientNo^2].rare_eff = 0;
		CliSys[ClientNo^2].rare_eff_end = 0;
		CellDel(WAZACHR_ID_SHOOTINGSTAR);
		ObjPalRegNumDel(WAZACHR_ID_SHOOTINGSTAR);
		
//		AddTask(VoiceCheckTask, 10);
		if(FightType&FIGHT_TYPE_4_2vs2)
			m4aMPlayContinue(&m4a_mplay000);
		else
			m4aMPlayVolumeControl(&m4a_mplay000,0xffff,256);
//		ClientAIBitReset();
		CliSys[ClientNo].send_out_wait = 3;
		ClientSequence[ClientNo] = ClientAISendOutWaitBitReset;
	}
#endif
}

void ClientAIPokeSendOutWait(void)
{
//	if(ActWork[ClientStruct[ClientNo]].move == DummyActMove)	//2�����ɓ����Ă�̂�1�����������Ă����Ζ��Ȃ�����
//	if(CliSys[ClientNo].ball_eff_wait == 0 && VoiceEndCheck() == 0)

	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo].rare_eff == 0)
		RareEffectCheckSet(ClientNo, &PokeParaEnemy[SelMonsNo[ClientNo]]);
	if(CliSys[ClientNo^2].ball_eff_wait == 0 && CliSys[ClientNo^2].rare_eff == 0)
		RareEffectCheckSet(ClientNo^2, &PokeParaEnemy[SelMonsNo[ClientNo^2]]);

	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo^2].ball_eff_wait == 0)
	{
		if(FightCheck() && 	(FightType & FIGHT_TYPE_4_2vs2) == 0)
		{
			DelActor(&ActWork[ClientStruct[ClientNo^2]]);		//�̪�ĊĎ�����DEL
			GaugeTransChrSet(ClientGaugeSt[ClientNo^2],
				&PokeParaEnemy[SelMonsNo[ClientNo^2]], G_ALL);
			GaugeAppearEffect(ClientNo^2);
			GaugeON(ClientGaugeSt[ClientNo^2]);
			
			//�ڰŰ�͎�����Del�����̂�
//			AffineDelActor(&ActWork[PokemonStruct[ClientNo^2]]);//�ꉞ(�Ă�ł����͂Ȃ���)
//			DelActor(&ActWork[PokemonStruct[ClientNo^2]]);

			//�e���
			PokeShadowSeqSet(ClientNo^2, 
				PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo^2]], ID_monsno));
		}
		
		DelActor(&ActWork[ClientStruct[ClientNo]]);			//�̪�ĊĎ�����DEL
		GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaEnemy[SelMonsNo[ClientNo]], G_ALL);
		GaugeAppearEffect(ClientNo);
		GaugeON(ClientGaugeSt[ClientNo]);

		//�ڰŰ�͎�����Del�����̂�
//		AffineDelActor(&ActWork[PokemonStruct[ClientNo^2]]);//�ꉞ(�Ă�ł����͂Ȃ���)
//		DelActor(&ActWork[PokemonStruct[ClientNo^2]]);

		//�e���
		PokeShadowSeqSet(ClientNo, 
			PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]], ID_monsno));
		
//		ClientAIBitReset();
		BtlWork->sendout_eff = 0;
		ClientSequence[ClientNo] = ClientAISendOutGaugeWait;
	}
}

void	ClientAIPokeEncountWait(void)
{
#if 0	// 2002.07.25(��)
	if((ActWork[PokemonStruct[ClientNo]].anmend_sw==1)&&
	(ActWork[PokemonStruct[ClientNo]].dx==0)){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientAICommandWait;
		ClientAIBitReset();
	}
#else
	if((ActWork[PokemonStruct[ClientNo]].anmend_sw==1)&&
	(ActWork[PokemonStruct[ClientNo]].dx==0)){
		if(CliSys[ClientNo].rare_eff == 0)
			RareEffectCheckSet(ClientNo, &PokeParaEnemy[SelMonsNo[ClientNo]]);
		else if(CliSys[ClientNo].rare_eff_end){
			CliSys[ClientNo].rare_eff = 0;
			CliSys[ClientNo].rare_eff_end = 0;
			CellDel(WAZACHR_ID_SHOOTINGSTAR);
			ObjPalRegNumDel(WAZACHR_ID_SHOOTINGSTAR);
			
			//DelActor(&ActWork[PokemonStruct[ClientNo]]);
			ClientAIBitReset();
		}
	}
#endif
}


void	ClientAIHpGaugeWait(void)
{
	s16 work;
//	u8 parts;
	
	work = GaugeOBJ(ClientNo, ClientGaugeSt[ClientNo], 0, 0);
	GaugeON(ClientGaugeSt[ClientNo]);
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
		ClientAIBitReset();
	}
}


void	ClientAIPokeKizetsuWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].act_sw==0){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientAICommandWait;
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientAIBitReset();
	}
}

void ClientAIPokeReturnWait(void)
{
	if(CliSys[ClientNo].etc_eff == 0)
	{
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		PokeShadowBanishSeqSet(ClientNo);
		
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientAIBitReset();
	}
}

void	ClientAIMessageWait(void)
{
	if(FightMsg.status==MSG_END){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientAICommandWait;
		ClientAIBitReset();
	}
}

#if 0	// 2002.07.22(��)	by matsuda
void	ClientAIWazaEffectWait(void)
{
	WazaEffectSequence();
	if(WazaEffectFlag==0){
		ClientAIBitReset();
	}
}
#endif

void ClientAIPokeDamageBlinkWait(void)
{
	u8 pokeact;
	
	pokeact = PokemonStruct[ClientNo];
	if(ActWork[pokeact].work[1] == DAMAGE_BLINK_WAIT)
	{
		ActWork[pokeact].work[1] = 0;
		ActWork[pokeact].banish = 0;
		ClientEffectWait = 0;	//Blink�̪�āA�t���O��ؾ��
		ClientAIBitReset();
	}
	else
	{
		if(ActWork[pokeact].work[1] % BLINK_INTERVAL == 0)
			ActWork[pokeact].banish ^= 1;
		ActWork[pokeact].work[1]++;
	}
}

void ClientAIGaugeAppearWait(void)
{
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove){
		//�g����`�F�b�N
		if(Climem[ClientNo].migawari)
			SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
		ClientSequence[ClientNo] = ClientAIAppearMigawariEffWait;
	}
}

static void ClientAIAppearMigawariEffWait(void)
{
	if(CliSys[ClientNo].etc_eff == 0){
		AddTask(VoiceCheckTask, 10);
		ClientAIBitReset();
	}
}

void ClientAIPokeAppearWaitSub(void)
{
//	if(ActWork[PokemonStruct[ClientNo]].anmend_sw == 1)		��Ҏ~�߂� 2002.02.28(��)
	if(CliSys[ClientNo].rare_eff_end)
	{
		CliSys[ClientNo].rare_eff = 0;
		CliSys[ClientNo].rare_eff_end = 0;
		CellDel(WAZACHR_ID_SHOOTINGSTAR);
		ObjPalRegNumDel(WAZACHR_ID_SHOOTINGSTAR);

		ActAnmChg(&ActWork[PokemonStruct[ClientNo]], 0);
		GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaEnemy[SelMonsNo[ClientNo]], G_ALL);
		GaugeAppearEffect(ClientNo);
		GaugeON(ClientGaugeSt[ClientNo]);
		PokeClientBanishMemory(ClientNo);
		//ClientAIBitReset();
		ClientSequence[ClientNo] = ClientAIGaugeAppearWait;
	}
}
void ClientAIPokeAppearWait(void)
{
	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo].rare_eff == 0)
		RareEffectCheckSet(ClientNo, &PokeParaEnemy[SelMonsNo[ClientNo]]);

	if(ActWork[ClientStruct[ClientNo]].move == DummyActMove && CliSys[ClientNo].ball_eff_wait == 0)
	{
		DelActor(&ActWork[ClientStruct[ClientNo]]);					//�̪�ĊĎ�����DEL
//		ActAnmChg(&ActWork[PokemonStruct[ClientNo]], 1);	��Ҏ~�߂� 2002.02.28(��)
//		VoicePlay(PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_monsno),25);
		
		//�e���
		PokeShadowSeqSet(ClientNo, PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]], ID_monsno));
		
		ClientSequence[ClientNo] = ClientAIPokeAppearWaitSub;
	}
}

//----------------------------------------------------------
//	�ð���ُ�̪�ďI���҂�
//----------------------------------------------------------
void ClientAIStatusEffWait(void)
{
	if(CliSys[ClientNo].status_eff == 0)
		ClientAIBitReset();
}

//----------------------------------------------------------
//	�퓬�̪�ďI���҂�
//----------------------------------------------------------
void ClientAIFightEffWait(void)
{
	if(CliSys[ClientNo].fight_eff == 0)
		ClientAIBitReset();
}


//==============================================================
//                       �T�u���[�`��
//==============================================================

void ClientAIBitReset(void)
{
	ClientSequence[ClientNo] = ClientAICommandWait;
	ClientBit &= (no2bittbl[ClientNo] ^ 0xffffffff);
}

//==============================================================
//	�N���C�A���g���ߊ֐�
//==============================================================

//----------------------------------------------------------
//	�w�肵���߹���ް����擾����
//----------------------------------------------------------

void	ClientAIBSSPokeparaGet(void)
{
	u8 temp[256];
	int	length=0;
	int	i;
	u8	flag;

	if(ServerBuffer[ClientNo][2] == 0){
		length+=ClientAIBSSPokeparaGetAct(SelMonsNo[ClientNo],&temp[length]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				length+=ClientAIBSSPokeparaGetAct(i,&temp[length]);
			}
			flag=flag>>1;
		}
	}
	BSS_DATA_TRANS_SET(BSS_CLIENT,length,(u8 *)&temp);
	ClientAIBitReset();
}

int	ClientAIBSSPokeparaGetAct(u8 SelectNo,u8 *temp)
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
			PCP.monsno		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_monsno);
			PCP.item		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_item);
			for(i=0;i<4;i++){
				PCP.waza[i]	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_waza1+i);
				PCP.pp[i]	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_pp1+i);
			}
			PCP.pp_count	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_pp_count);
			PCP.friend		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_friend);
			PCP.exp			=PokeParaGet(&PokeParaEnemy[SelectNo],ID_exp);
			PCP.hp_rnd		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_hp_rnd);
			PCP.pow_rnd		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_pow_rnd);
			PCP.def_rnd		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_def_rnd);
			PCP.agi_rnd		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_agi_rnd);
			PCP.spepow_rnd	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_spepow_rnd);
			PCP.spedef_rnd	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_spedef_rnd);
			PCP.personal_rnd=PokeParaGet(&PokeParaEnemy[SelectNo],ID_personal_rnd);
			PCP.condition	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_condition);
			PCP.level		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_level);
			PCP.hp			=PokeParaGet(&PokeParaEnemy[SelectNo],ID_hp);
			PCP.hpmax		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_hpmax);
			PCP.pow			=PokeParaGet(&PokeParaEnemy[SelectNo],ID_pow);
			PCP.def			=PokeParaGet(&PokeParaEnemy[SelectNo],ID_def);
			PCP.agi			=PokeParaGet(&PokeParaEnemy[SelectNo],ID_agi);
			PCP.spepow		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_spepow);
			PCP.spedef		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_spedef);
			PCP.tamago_flag	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_tamago_flag);
			PCP.speabi		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_speabi);
			PCP.id_no		=PokeParaGet(&PokeParaEnemy[SelectNo],ID_id_no);
		#if 0	//���[�J���C�Y�Ή��̂��ߕύX�@2002.08.09(��)
			PokeParaGet(&PokeParaEnemy[SelectNo],ID_nickname,PCP.nickname);
		#else
			PokeParaGet(&PokeParaEnemy[SelectNo],ID_nickname,buff);
			NickNameCopy(PCP.nickname, buff);
		#endif
			PokeParaGet(&PokeParaEnemy[SelectNo],ID_oyaname,PCP.oyaname);
			//BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(PCP),(u8 *)&PCP);
			adr=(u8 *)&PCP;
//			for(i=0;i<XtOffset(PokemonServerParam *, work);i++){
			for(i=0;i<sizeof(PokemonServerParam);i++){
				temp[i]=adr[i];
			}
			break;
		case BSS_PARA_MONSNO:			//�����X�^�[�ԍ�
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_monsno);
			temp[0]=(buf16&0x00ff)>> 0;
			temp[1]=(buf16&0xff00)>> 8;
			i=2;
			break;
		case BSS_PARA_ITEM:				//�����A�C�e��
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_item);
			temp[0]=(buf16&0x00ff)>> 0;
			temp[1]=(buf16&0xff00)>> 8;
			i=2;
			break;
		case BSS_PARA_WAZA:
			for(i=0;i<4;i++){
				PWP.waza[i]	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_waza1+i);
				PWP.pp[i]	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_pp1+i);
			}
			PWP.pp_count	=PokeParaGet(&PokeParaEnemy[SelectNo],ID_pp_count);
			adr=(u8 *)&PWP;
			for(i=0;i<sizeof(PokemonWazaParam);i++){
				temp[i]=adr[i];
			}
			break;
		case BSS_PARA_WAZA1:				//�Z
		case BSS_PARA_WAZA2:
		case BSS_PARA_WAZA3:
		case BSS_PARA_WAZA4:
			buf16=
			PokeParaGet(&PokeParaEnemy[SelectNo],ID_waza1+ServerBuffer[ClientNo][1]-BSS_PARA_WAZA1);
			temp[0]=(buf16&0x00ff)>> 0;
			temp[1]=(buf16&0xff00)>> 8;
			i=2;
			break;
		case BSS_PARA_PP:
			for(i=0;i<4;i++){
				temp[i] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_pp1+i);
			}
			temp[i] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_pp_count);
			i++;
			break;
		case BSS_PARA_PP1:				//�Z�|�C���g
		case BSS_PARA_PP2:
		case BSS_PARA_PP3:
		case BSS_PARA_PP4:
			temp[0]=
			PokeParaGet(&PokeParaEnemy[SelectNo],ID_pp1+ServerBuffer[ClientNo][1]-BSS_PARA_PP1);
			i=1;
			break;
		case BSS_PARA_PPCOUNT1:
		case BSS_PARA_PPCOUNT2:
		case BSS_PARA_PPCOUNT3:
		case BSS_PARA_PPCOUNT4:
#if 0
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo].PPP.pp_count1;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo].PPP.pp_count2;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo].PPP.pp_count3;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo].PPP.pp_count4;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
#endif
			break;
		case BSS_PARA_ID:				//�h�c�i���o�[
			buf32=PokeParaGet(&PokeParaEnemy[SelectNo],ID_id_no);
			temp[0]=(buf32&0x000000ff)>> 0;
			temp[1]=(buf32&0x0000ff00)>> 8;
			temp[2]=(buf32&0x00ff0000)>>16;
			i=3;
			break;
		case BSS_PARA_EXP:				//��{�o���l
			buf32=PokeParaGet(&PokeParaEnemy[SelectNo],ID_exp);
			temp[0]=(buf32&0x000000ff)>> 0;
			temp[1]=(buf32&0x0000ff00)>> 8;
			temp[2]=(buf32&0x00ff0000)>>16;
			i=3;
			break;
		case BSS_PARA_HPEXP:			//�g�o�w�͒l
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_hp_exp);
			i=1;
			break;
		case BSS_PARA_POWEXP:			//�U���͓w�͒l
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_pow_exp);
			i=1;
			break;
		case BSS_PARA_DEFEXP:			//�h��͓w�͒l
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_def_exp);
			i=1;
			break;
		case BSS_PARA_AGIEXP:			//�f�����w�͒l
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_agi_exp);
			i=1;
			break;
		case BSS_PARA_SPEPOWEXP:		//����U���w�͒l
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_spepow_exp);
			i=1;
			break;
		case BSS_PARA_SPEDEFEXP:		//����h��w�͒l
			temp[0]=PokeParaGet(&PokeParaEnemy[SelectNo],ID_spedef_exp);
			i=1;
			break;
		case BSS_PARA_FRIEND:			//�F�D�l
			temp[0]=PokeParaGet(&PokeParaEnemy[SelectNo],ID_friend);
			i=1;
			break;
		case BSS_PARA_POKERUS:			//�|�P���X
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_pokerus);
			i=1;
			break;
		case BSS_PARA_GET_PLACE:		//�ߊl�ꏊ
			temp[0]=PokeParaGet(&PokeParaEnemy[SelectNo],ID_get_place);
			i=1;
			break;
		case BSS_PARA_GET_LEVEL:		//�ߊl���x��
			temp[0]=PokeParaGet(&PokeParaEnemy[SelectNo],ID_get_level);
			i=1;
			break;
		case BSS_PARA_GET_CASSETTE:		//�ߊl�J�Z�b�g
			temp[0]=PokeParaGet(&PokeParaEnemy[SelectNo],ID_get_cassette);
			i=1;
			break;
		case BSS_PARA_GET_BALL:			//�ߊl�{�[��
			temp[0]=PokeParaGet(&PokeParaEnemy[SelectNo],ID_get_ball);
			i=1;
			break;
		case BSS_PARA_POWERRND:
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_hp_rnd);
			temp[1] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_pow_rnd);
			temp[2] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_def_rnd);
			temp[3] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_agi_rnd);
			temp[4] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_spepow_rnd);
			temp[5] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_spedef_rnd);
			i=6;
			break;
		case BSS_PARA_HPRND:			//HP�p���[����
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_hp_rnd);
			i=1;
			break;
		case BSS_PARA_POWRND:			//�U���̓p���[����
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_pow_rnd);
			i=1;
			break;
		case BSS_PARA_DEFRND:			//�h��̓p���[����
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_def_rnd);
			i=1;
			break;
		case BSS_PARA_AGIRND:			//�f�����p���[����
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_agi_rnd);
			i=1;
			break;
		case BSS_PARA_SPEPOWRND:		//����U���̓p���[����
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_spepow_rnd);
			i=1;
			break;
		case BSS_PARA_SPEDEFRND:		//����h��̓p���[����
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_spedef_rnd);
			i=1;
			break;
		case BSS_PARA_CHRRND:			//������
			buf32 = PokeParaGet(&PokeParaEnemy[SelectNo],ID_personal_rnd);
			temp[0]=(buf32&0x000000ff)>> 0;
			temp[1]=(buf32&0x0000ff00)>> 8;
			temp[2]=(buf32&0x00ff0000)>>16;
			temp[3]=(buf32&0xff000000)>>24;
			i=4;
			break;
		case BSS_PARA_SUM:				//�`�F�b�N�T��
			buf16 = PokeParaGet(&PokeParaEnemy[SelectNo],ID_checksum);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_CONDITION:		//�R���f�B�V����
			buf32=PokeParaGet(&PokeParaEnemy[SelectNo],ID_condition);
			temp[0]=(buf32&0x000000ff)>> 0;
			temp[1]=(buf32&0x0000ff00)>> 8;
			temp[2]=(buf32&0x00ff0000)>>16;
			temp[3]=(buf32&0xff000000)>>24;
			i=4;
			break;
		case BSS_PARA_LEVEL:			//���x��
			temp[0]=PokeParaGet(&PokeParaEnemy[SelectNo],ID_level);
			i=1;
			break;
		case BSS_PARA_HP:				//�g�o�c��
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_hp);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_HPMAX:			//�g�o�l�`�w
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_hpmax);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_POW:				//�U����
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_pow);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_DEF:				//�h���
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_def);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_AGI:				//�f����
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_agi);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_SPEPOW:			//����U����
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_spepow);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_SPEDEF:			//����h���
			buf16=PokeParaGet(&PokeParaEnemy[SelectNo],ID_spedef);
			temp[0]=(buf16&0x000000ff)>> 0;
			temp[1]=(buf16&0x0000ff00)>> 8;
			i=2;
			break;
		case BSS_PARA_STYLE:			//�������悳
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_style);
			i=1;
			break;
		case BSS_PARA_BEAUTIFUL:		//������
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_beautiful);
			i=1;
			break;
		case BSS_PARA_CUTE:				//���킢��
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_cute);
			i=1;
			break;
		case BSS_PARA_CLEVER:			//����
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_clever);
			i=1;
			break;
		case BSS_PARA_STRONG:			//�����܂���
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_strong);
			i=1;
			break;
		case BSS_PARA_FUR:				//�щ�
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_fur);
			i=1;
			break;
		case BSS_PARA_STYLEMEDAL:		//�������悳�M��
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_stylemedal);
			i=1;
			break;
		case BSS_PARA_BEAUTIFULMEDAL:	//�������M��
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_beautifulmedal);
			i=1;
			break;
		case BSS_PARA_CUTEMEDAL:		//���킢���M��
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_cutemedal);
			i=1;
			break;
		case BSS_PARA_CLEVERMEDAL:		//�����M��
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_clevermedal);
			i=1;
			break;
		case BSS_PARA_STRONGMEDAL:		//�����܂����M��
			temp[0] = PokeParaGet(&PokeParaEnemy[SelectNo],ID_strongmedal);
			i=1;
			break;
	}
	return	i;
}


//----------------------------------------------------------
//	�w�肵�����ڽ���߹���ް����擾����
//----------------------------------------------------------
void ClientAIBSSPokeparaGet2(void)
{
	PokemonServerParam	PCP;
	u8 *pPP, *pPCP;
	u8 i;
	
	pPP = ((u8 *)(&PokeParaEnemy[SelMonsNo[ClientNo]])) + ServerBuffer[ClientNo][1];
	pPCP = ((u8 *)(&PCP)) + ServerBuffer[ClientNo][1];
	
	for(i = 0; i < ServerBuffer[ClientNo][2]; i++)
	{
		pPCP[i] = pPP[i];
	}
	BSS_DATA_TRANS_SET(BSS_CLIENT, ServerBuffer[ClientNo][2], &pPCP[0]);

	ClientAIBitReset();
}

//----------------------------------------------------------
//	�w�肵���߹���ް���ύX����
//----------------------------------------------------------

void ClientAIBSSPokeparaPut(void)
{
	u8	i,flag;
	
	if(ServerBuffer[ClientNo][2] == 0){
		ClientAIBSSPokeparaPutAct(SelMonsNo[ClientNo]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				ClientAIBSSPokeparaPutAct(i);
			}
			flag=flag>>1;
		}
	}
	ClientAIBitReset();
}

void ClientAIBSSPokeparaPutAct(u8 SelectNo)
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
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_monsno, 
						(u8 *)&PCP->monsno);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_item,
						(u8 *)&PCP->item);
			for(i=0;i<4;i++){
				PokeParaPut(&PokeParaEnemy[SelectNo],ID_waza1+i,
							(u8 *)&PCP->waza[i]);
				PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp1+i,
							(u8 *)&PCP->pp[i]);
			}
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp_count,
						(u8 *)&PCP->pp_count);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_friend,
						(u8 *)&PCP->friend);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_exp,
						(u8 *)&PCP->exp);
			buf=PCP->hp_rnd;
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_hp_rnd,
						(u8 *)&buf);
			buf=PCP->pow_rnd;
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pow_rnd,
						(u8 *)&buf);
			buf=PCP->def_rnd;
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_def_rnd,
						(u8 *)&buf);
			buf=PCP->agi_rnd;
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_agi_rnd,
						(u8 *)&buf);
			buf=PCP->spepow_rnd;
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spepow_rnd,
						(u8 *)&buf);
			buf=PCP->spedef_rnd;
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spedef_rnd,
						(u8 *)&buf);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_personal_rnd,
						(u8 *)&PCP->personal_rnd);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_condition,
						(u8 *)&PCP->condition);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_level,
						(u8 *)&PCP->level);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_hp,
						(u8 *)&PCP->hp);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_hpmax,
						(u8 *)&PCP->hpmax);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pow,
						(u8 *)&PCP->pow);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_def,
						(u8 *)&PCP->def);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_agi,
						(u8 *)&PCP->agi);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spepow,
						(u8 *)&PCP->spepow);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spedef,
						(u8 *)&PCP->spedef);
			break;
		case BSS_PARA_MONSNO:			//�����X�^�[�ԍ�
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_monsno, 
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_ITEM:				//�����A�C�e��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_item,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_WAZA:
			for(i=0;i<4;i++){
				PokeParaPut(&PokeParaEnemy[SelectNo],ID_waza1+i,
							(u8 *)&PWP->waza[i]);
				PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp1+i,
							(u8 *)&PWP->pp[i]);
			}
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp_count,
						(u8 *)&PWP->pp_count);
			break;
		case BSS_PARA_WAZA1:				//�Z
		case BSS_PARA_WAZA2:
		case BSS_PARA_WAZA3:
		case BSS_PARA_WAZA4:
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_waza1+ServerBuffer[ClientNo][1]-BSS_PARA_WAZA1,&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_PP:
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp1,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp2,
						&ServerBuffer[ClientNo][BSS_DATASTART+1]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp3,
						&ServerBuffer[ClientNo][BSS_DATASTART+2]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp4,
						&ServerBuffer[ClientNo][BSS_DATASTART+3]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp_count,
						&ServerBuffer[ClientNo][BSS_DATASTART+4]);
			break;
		case BSS_PARA_PP1:				//�Z�|�C���g
		case BSS_PARA_PP2:
		case BSS_PARA_PP3:
		case BSS_PARA_PP4:
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pp1+ServerBuffer[ClientNo][1]-BSS_PARA_PP1,&ServerBuffer[ClientNo][BSS_DATALOW]);
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
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_id_no,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			break;
		case BSS_PARA_EXP:				//��{�o���l
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_exp,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			break;
		case BSS_PARA_HPEXP:			//�g�o�w�͒l
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_hp_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_POWEXP:			//�U���͓w�͒l
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pow_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_DEFEXP:			//�h��͓w�͒l
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_def_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_AGIEXP:			//�f�����w�͒l
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_agi_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEPOWEXP:		//����U���w�͒l
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spepow_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEDEFEXP:		//����h��w�͒l
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spedef_exp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_FRIEND:			//�F�D�l
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_friend,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_POKERUS:			//�|�P���X
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pokerus,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_GET_PLACE:		//�ߊl�ꏊ
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_get_place,
						&ServerBuffer[ClientNo][BSS_DATASTART]); 
			break;
		case BSS_PARA_GET_LEVEL:		//�ߊl���x��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_get_level,
						&ServerBuffer[ClientNo][BSS_DATASTART]); 
			break;
		case BSS_PARA_GET_CASSETTE:		//�ߊl�J�Z�b�g
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_get_cassette,
						&ServerBuffer[ClientNo][BSS_DATASTART]); 
			break;
		case BSS_PARA_GET_BALL:			//�ߊl�{�[��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_get_ball,
						&ServerBuffer[ClientNo][BSS_DATASTART]); 
			break;
		case BSS_PARA_POWERRND:
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_hp_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pow_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+1]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_def_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+2]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_agi_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+3]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spepow_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+4]);
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spedef_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART+5]);
			break;
		case BSS_PARA_HPRND:			//HP�p���[����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_hp_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_POWRND:			//�U���̓p���[����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pow_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_DEFRND:			//�h��̓p���[����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_def_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_AGIRND:			//�f�����p���[����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_agi_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEPOWRND:		//����U���̓p���[����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spepow_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEDEFRND:		//����h��̓p���[����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spedef_rnd,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CHRRND:			//������
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_personal_rnd,
						&ServerBuffer[ClientNo][BSS_DATASTART]);
			break;
		case BSS_PARA_SUM:				//�`�F�b�N�T��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_checksum,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CONDITION:		//�R���f�B�V����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_condition,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_LEVEL:			//���x��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_level,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_HP:				//�g�o�c��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_hp,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_HPMAX:			//�g�o�l�`�w
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_hpmax,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_POW:				//�U����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_pow,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_DEF:				//�h���
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_def,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_AGI:				//�f����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_agi,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEPOW:			//����U����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spepow,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_SPEDEF:			//����h���
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_spedef,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_STYLE:			//�������悳
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_style,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_BEAUTIFUL:		//������
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_beautiful,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CUTE:				//���킢��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_cute,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CLEVER:			//����
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_clever,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_STRONG:			//�����܂���
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_strong,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_FUR:				//�щ�
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_fur,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_STYLEMEDAL:		//�������悳�M��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_stylemedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_BEAUTIFULMEDAL:	//�������M��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_beautifulmedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CUTEMEDAL:		//���킢���M��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_cutemedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_CLEVERMEDAL:		//�����M��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_clevermedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
		case BSS_PARA_STRONGMEDAL:		//�����܂����M��
			PokeParaPut(&PokeParaEnemy[SelectNo],ID_strongmedal,
						&ServerBuffer[ClientNo][BSS_DATALOW]);
			break;
	}
}

//----------------------------------------------------------
//	�w�肵�����ڽ���߹���ް���ύX����
//----------------------------------------------------------
void ClientAIBSSPokeparaPut2(void)
{
	u8 *pPP;
	u8 i;
	
	pPP = ((u8 *)(&PokeParaEnemy[SelMonsNo[ClientNo]])) + ServerBuffer[ClientNo][1];
	for(i = 0; i < ServerBuffer[ClientNo][2]; i++)
	{
		pPP[i] = ServerBuffer[ClientNo][3+i];
	}

	ClientAIBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------

const u8 PokeEnemyPosX[2][2]={
		{CAI_POKE_X,CAI_POKE_X},
		{CAI_POKE1X,CAI_POKE2X},
};
const u8 PokeEnemyPosY[2][2]={
		{CAI_POKE_Y,CAI_POKE_Y},
		{CAI_POKE1Y,CAI_POKE2Y},
};

void ClientAIBSSPokeEncount(void)
{
	u16	monsno;

	monsno=PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_monsno);

	PokemonFObjCellPalSet(&PokeParaEnemy[SelMonsNo[ClientNo]], ClientNo);
	//PokeActHeaderMake(PokeParaEnemy[SelMonsNo[ClientNo]].PPP.monsno,ClientNo);
	PokeActHeaderMake(monsno,
		ClientTypeGet(ClientNo));	//�ύX by matsuda 2001.09.26(��)
#if 0
	PokemonStruct[ClientNo]=AddActor(&PokeActHeader,
		PokeEnemyPosX[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1],
//		PokeEnemyPosY[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1]+4*(8-pm2fpara[monsno].size),
		PokeEnemyPosY[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1]+pm2fpara[monsno].patcnt,
		CAI_POKE1PRI);
#else
	PokemonStruct[ClientNo]=
		AddActor(&PokeActHeader,
				 GetWazaEffPos(ClientNo,PM_X),
				 GetPokemonYPos(ClientNo),
				 GetPokeSoftPri(ClientNo));
#endif

	ActWork[PokemonStruct[ClientNo]].dx=-ENCOUNT_DX;
	ActWork[PokemonStruct[ClientNo]].work[0]=ClientNo;
	ActWork[PokemonStruct[ClientNo]].work[2]=monsno;
	ActWork[PokemonStruct[ClientNo]].oamData.Pltt=ClientNo;
	ActAnmChg(&ActWork[PokemonStruct[ClientNo]],BattlePokeAnmNo[ClientNo]);

	//�e���
	PokeShadowSeqSet(ClientNo, 
		PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]], ID_monsno));

	ClientSequence[ClientNo]=ClientAIPokeEncountWait;
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ްق����яo��)
//----------------------------------------------------------
void ClientAIBSSPokeAppear(void)
{
	SelMonsNo[ClientNo] = ServerBuffer[ClientNo][1];
	AIPokeAppearCommon(ClientNo, ServerBuffer[ClientNo][2]);

//	ClientAIBitReset();
	ClientSequence[ClientNo] = ClientAIPokeAppearWait;
}

//-- PokeAppear�ASendOut���ʏ��� --//
static void AIPokeAppearCommon(u8 client_no, u8 baton_flg)
{
	u16	monsno;

	ClientPokeSysFlagClear(client_no, baton_flg);

	SelMonsNo[client_no]=ServerBuffer[client_no][1];
	monsno=PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_monsno);
	ClientStruct[client_no] = AddBanishActor(CA_PokeAppearWait);
	PokemonFObjCellPalSet(&PokeParaEnemy[SelMonsNo[client_no]], client_no);

	PokeActHeaderMake(monsno,ClientTypeGet(client_no));

#if 0
	PokemonStruct[client_no] = AddActor(&PokeActHeader,
							PokeEnemyPosX[FightCheck()][ClientTypeGet(client_no)>>1],
							PokeEnemyPosY[FightCheck()][ClientTypeGet(client_no)>>1]+
//							4*(8-pm2fpara[monsno].size),
							pm2fpara[monsno].patcnt,
							C_POKE1PRI);	//�߹��
#else
	PokemonStruct[client_no]=
		AddActor(&PokeActHeader,
				 GetWazaEffPos(client_no,PM_X),
				 GetPokemonYPos(client_no),
				 GetPokeSoftPri(client_no));
#endif
	ActWork[PokemonStruct[client_no]].work[0] = client_no;
	ActWork[PokemonStruct[client_no]].work[2] = monsno;
	ActWork[ClientStruct[client_no]].work[1] = PokemonStruct[client_no];
	ActWork[PokemonStruct[client_no]].oamData.Pltt = client_no;
	ActAnmChg(&ActWork[PokemonStruct[client_no]],BattlePokeAnmNo[client_no]);

#if 0	//changed by matsuda 2002.01.11(��)
	ActWork[ClientStruct[client_no]].work[0] = 
		MBSmokeSet(PokeEnemyPosX[FightCheck()][ClientTypeGet(client_no)>>1], PokeEnemyPosY[FightCheck()][ClientTypeGet(client_no)>>1]+20, 1);
	CAC_PokeAppearDifferentInit(&ActWork[PokemonStruct[client_no]], 1);
#else
	ActWork[PokemonStruct[client_no]].banish = 1;
	ActWork[PokemonStruct[client_no]].move = DummyActMove;
	ActWork[ClientStruct[client_no]].work[0] = MonsterBallSet(0,0xfe);
#endif
}

//----------------------------------------------------------
//	�߹�݂�߂��̪�Ă��N������
//----------------------------------------------------------
void ClientAIBSSPokeReturn(void)
{
	if(ServerBuffer[ClientNo][1] == 0)
	{
		CliSys[ClientNo].WazaEffSeqNo = 0;
		ClientSequence[ClientNo] = ClientAIPokeReturnMigawariChk;
//		SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_ENEMY);
//		ClientSequence[ClientNo] = ClientAIPokeReturnWait;
	}
	else
	{
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		PokeShadowBanishSeqSet(ClientNo);
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientAIBitReset();
	}
}

enum{
	CAI_RET_MIGAWARICHK,
	CAI_RET_POKERETURN,
};
static void ClientAIPokeReturnMigawariChk(void)
{
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case CAI_RET_MIGAWARICHK:
			if(Climem[ClientNo].migawari == 1)	//�u�݂����v�`�F�b�N
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			CliSys[ClientNo].WazaEffSeqNo = CAI_RET_POKERETURN;
			break;
		case CAI_RET_POKERETURN:					//�Z�G�t�F�N�g�Z�b�g
			if(CliSys[ClientNo].etc_eff)
				break;
			CliSys[ClientNo].WazaEffSeqNo = 0;
			SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_ENEMY);
			ClientSequence[ClientNo] = ClientAIPokeReturnWait;
			break;
	}
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------
#define TRAINER_IN_DX	(-200)		//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_IN_SP	4	//�݂��Ă���Ƃ��̽�߰��
void ClientAIBSSTrainerEncount(void)
{
	int	type;

#ifdef PM_DEBUG
	if(DebugFightFlag&DEBUG_FIGHT_TRAINER)
		type=UserWork[DEBUG_TRAINER];
	else if(FieldTrainerID==ORUSUBAN_TRAINER)
		type=OrusubanChrGet();				//������ԑΐ�p
#else
	if(FieldTrainerID==ORUSUBAN_TRAINER)
		type=OrusubanChrGet();				//������ԑΐ�p
#endif
	else if(FightType&FIGHT_TYPE_BATTLE_TOWER)
		type=BattleTowerChrGet();				//�o�g���^���[�p
	else if(FightType&FIGHT_TYPE_CARD_E)
		type=BattleCardEChrGet();				//�J�[�h���p
	else
		type=TD[FieldTrainerID].chrnum;

	TrainerFObjCellPalSet(type, ClientNo);

//	PokeActHeaderMake(type+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(type,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							CAI_POKE_X,
							CAI_POKE_Y+4*(8-trfpara[type].size),
							//C_TRN_PRI);	//�ڰŰ
							GetPokeSoftPri(ClientNo));

//	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = -ENCOUNT_DX;
	ActWork[PokemonStruct[ClientNo]].work[0] = ENCOUNT_SPEED;
	ActWork[PokemonStruct[ClientNo]].oamData.Pltt 
		= ObjPalNumGet(TRFObjDataPal[type].reg_num);
	ActWork[PokemonStruct[ClientNo]].work[5]	//�]���^�Ȃ̂�Del���鎞��CharNo��
		= ActWork[PokemonStruct[ClientNo]].oamData.CharNo;	//�߂��K�v������
	ActWork[PokemonStruct[ClientNo]].oamData.CharNo
		= CellCharNumGet(TRFObjDataCell[type].act_num);
	ActWork[PokemonStruct[ClientNo]].oamData.AffineParam = type;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;

	ClientSequence[ClientNo] = ClientAITrainerEncountWait;
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ΐ�I����)
//----------------------------------------------------------
#define TRAINER_AP_IN_DX (96)//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_AP_IN_SP	(-2)	//�݂��Ă���Ƃ��̽�߰��
#define TRAINER_OFFSET_X	32	//���S�ɓo��ʒu�܂Žײ�ނ����Ȃ��悤�ɵ̾�Ăō���������
void ClientAIBSSTrainerAppear(void)
{
	int	type;

	if(FieldTrainerID==ORUSUBAN_TRAINER)
		type=OrusubanChrGet();				//������ԑΐ�p
	else if(FightType&FIGHT_TYPE_BATTLE_TOWER)
		type=BattleTowerChrGet();				//�o�g���^���[�p
	else if(FightType&FIGHT_TYPE_CARD_E)
		type=BattleCardEChrGet();				//�J�[�h���p
	else
		type=TD[FieldTrainerID].chrnum;

	TrainerFObjCellPalSet(type, ClientNo);

//	PokeActHeaderMake(type+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(type,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							CAI_POKE_X,
							CAI_POKE_Y+4*(8-trfpara[type].size),
							C_TRN_PRI);	//�ڰŰ

//	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = TRAINER_AP_IN_DX;
	ActWork[PokemonStruct[ClientNo]].x += TRAINER_OFFSET_X;
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_AP_IN_SP;
	ActWork[PokemonStruct[ClientNo]].oamData.Pltt 
		= ObjPalNumGet(TRFObjDataPal[type].reg_num);
	ActWork[PokemonStruct[ClientNo]].work[5]	//�]���^�Ȃ̂�Del���鎞��CharNo��
		= ActWork[PokemonStruct[ClientNo]].oamData.CharNo;	//�߂��K�v������
	ActWork[PokemonStruct[ClientNo]].oamData.CharNo
		= CellCharNumGet(TRFObjDataCell[type].act_num);
	ActWork[PokemonStruct[ClientNo]].oamData.AffineParam = type;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;

	ClientSequence[ClientNo] = ClientAITrainerAppearWait;
}

//----------------------------------------------------------
//	�ڰŰ��߂��̪�Ă��N������
//----------------------------------------------------------

#define	TRAINER_OUT_SP2	4

void ClientAIBSSTrainerReturn(void)
{
#if 0	//changed by matsuda 2002.01.15(��)
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_OUT_SP2;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideOut;
#else
	DxDyAddClear(&ActWork[PokemonStruct[ClientNo]]);
	ActWork[PokemonStruct[ClientNo]].work[0] = ENEMYTRAINER_RETURN_SYNCNUM;	//VsyncNum
	ActWork[PokemonStruct[ClientNo]].work[2] = 240+40;
	ActWork[PokemonStruct[ClientNo]].work[4] = ActWork[PokemonStruct[ClientNo]].y;
	ActWork[PokemonStruct[ClientNo]].move = W_StraightSyncMove;
	SetWorkSeq(&ActWork[PokemonStruct[ClientNo]], DummyActMove);
#endif
	ClientSequence[ClientNo] = ClientAITrainerReturnWait;
}

//----------------------------------------------------------
//	�߹�݂��C�₷��̪�Ă��N������
//----------------------------------------------------------
void ClientAIBSSPokeKizetu(void)
{
#if 0	//2002.09.03(��)
	SePlayPan(SE_POKE_DEAD, WAZA_SE_R);
	ActWork[PokemonStruct[ClientNo]].move=SeqEnemyPokeKizetsuInit;
	ClientSequence[ClientNo]=ClientAIPokeKizetsuWait;
#else
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case 0:
			if(Climem[ClientNo].migawari == 1)	//�u�݂����v�`�F�b�N
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			CliSys[ClientNo].WazaEffSeqNo++;
			break;
		case 1:
		default:
			if(CliSys[ClientNo].etc_eff)
				break;
			CliSys[ClientNo].WazaEffSeqNo = 0;

			SePlayPan(SE_POKE_DEAD, WAZA_SE_R);
			ActWork[PokemonStruct[ClientNo]].move=SeqEnemyPokeKizetsuInit;
			ClientSequence[ClientNo]=ClientAIPokeKizetsuWait;
			break;
	}
#endif
}

//----------------------------------------------------------
//	�߹�݂��S�ł����Ƃ��̴̪�Ă��N������
//----------------------------------------------------------
void ClientAIBSSZenmetsu(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl�����̴̪�Ă��o��
//----------------------------------------------------------
void ClientAIBSSGetSuccess(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl���s�̴̪�Ă��o��
//----------------------------------------------------------
void ClientAIBSSGetFail(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�߹�����ٱ��߂̴̪�Ă��o��
//----------------------------------------------------------
void ClientAIBSSPokeLevelUp(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�w�肵���Z���ް�̴̪�Ă��o��
//----------------------------------------------------------
#if 0	// 2002.07.22(��)
void ClientAIBSSWazaEffect(void)
{
	u16	wazano;

	wazano=ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8);
	WazaCounter = ServerBuffer[ClientNo][3];
	WazaEffPower = ServerBuffer[ClientNo][4] | (ServerBuffer[ClientNo][5]<<8);
	WazaEffDamage = ServerBuffer[ClientNo][6] | (ServerBuffer[ClientNo][7]<<8)
		| (ServerBuffer[ClientNo][8]<<16) | (ServerBuffer[ClientNo][9]<<24);
	WazaEffNatukido = ServerBuffer[ClientNo][10] | (ServerBuffer[ClientNo][11]<<8);
	pwkc = (WazaKoukaCount*)&ServerBuffer[ClientNo][12];

	if(PutWazaEffectCheck(wazano, WazaCounter)){
		ClientAIBitReset();
		return;
	}

	if(FightCheck() && (AttackClient^2) == DefenceClient && WazaDamage > 0)
		wazano = WAZANO_MAX+1;	//�����ւ̍U���Ȃ�΁A�ȈՓI�ȴ̪�Ă��o��

	WazaEffectSet(wazano);
	ClientSequence[ClientNo] = ClientAIWazaEffectWait;
}
#else
enum{
	AI_MIGAWARI_CHK,
	AI_EFF_PUTCHK,
	AI_EFF_WAIT,
	AI_EFF_END,
};
void ClientAIBSSWazaEffect(void)
{
	u16 wazano;

	if(ClientSEWait(ClientNo))
		return;

	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	WazaCounter = ServerBuffer[ClientNo][3];
	WazaEffPower = ServerBuffer[ClientNo][4] | (ServerBuffer[ClientNo][5]<<8);
	WazaEffDamage = ServerBuffer[ClientNo][6] | (ServerBuffer[ClientNo][7]<<8)
		| (ServerBuffer[ClientNo][8]<<16) | (ServerBuffer[ClientNo][9]<<24);
	WazaEffNatukido = ServerBuffer[ClientNo][10];
	WazaEffWeather = ServerBuffer[ClientNo][12] | (ServerBuffer[ClientNo][13]<<8);
	pwkc = (WazaKoukaCount*)&ServerBuffer[ClientNo][16];

	HenshinRnd[ClientNo]=pwkc->henshin_rnd;	//�ւ񂵂�p�̌�����
	
	if(PutWazaEffectCheck(wazano, WazaCounter)){
		ClientAIBitReset();
		return;
	}
	
	CliSys[ClientNo].WazaEffSeqNo = AI_MIGAWARI_CHK;
	ClientSequence[ClientNo] = ClientAIWazaEffectSet;
}

static void ClientAIWazaEffectSet(void)
{
	u16 wazano;
	u8 renzoku_cnt;
	
	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	renzoku_cnt = ServerBuffer[ClientNo][11];	//0:ɰ�� 1:�A���Z�I�� 2:�A���Z��

	switch(CliSys[ClientNo].WazaEffSeqNo){
		case AI_MIGAWARI_CHK:	//�u�݂����v�`�F�b�N
			if(Climem[ClientNo].migawari == 1 && Climem[ClientNo].migawari_out == 0){
				Climem[ClientNo].migawari_out = 1;
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			}
			CliSys[ClientNo].WazaEffSeqNo = AI_EFF_PUTCHK;
			break;
		case AI_EFF_PUTCHK:					//�Z�G�t�F�N�g�Z�b�g
			if(CliSys[ClientNo].etc_eff)
				break;
			
//			if(FightCheck() && (AttackClient^2) == DefenceClient && WazaDamage > 0)
//				wazano = WAZANO_MAX+1;	//�����ւ̍U���Ȃ�΁A�ȈՓI�ȴ̪�Ă��o��
			
			PokeAffineON_OFF(0);
			WazaEffectSet(wazano);
			CliSys[ClientNo].WazaEffSeqNo = AI_EFF_WAIT;
			break;
		case AI_EFF_WAIT:					//�Z�G�t�F�N�g�I���҂�
			WazaEffectSequence();
			if(WazaEffectFlag==0){
				PokeAffineON_OFF(1);
				if(Climem[ClientNo].migawari && renzoku_cnt <= 1){	//�g����`�F�b�N
					SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
					Climem[ClientNo].migawari_out = 0;
				}
				CliSys[ClientNo].WazaEffSeqNo = AI_EFF_END;
			}
			break;
		case AI_EFF_END:
			if(CliSys[ClientNo].etc_eff)
				break;

			//�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
			PokeBanishMemory();
			wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
			MigawariFlgCheckSet(ClientNo, wazano);
			CliSys[ClientNo].WazaEffSeqNo = 0;
			ClientAIBitReset();
			break;
	}
}
#endif

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

#if 0
void ClientAIBSSMessage(void)
{
#if 1
	ScrX0=0;
	ScrY0=0;
//	MsgPutSet(&FightMsg,&ServerBuffer[ClientNo][2],0x10,1,15);
//	TalkMsgSet(&FightMsg,&ServerBuffer[ClientNo][2],CGX_START+0x10,2,15);
	FightMsgCreate((u8 *)&ServerBuffer[ClientNo][2]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientAIMessageWait;
#else
//���Ȃ��΂�Ƃ��A���������Ƃ��̃A���[�g���b�Z�[�W���o���Ȃ��悤�ɂ���
	ClientAIBitReset();
#endif
}
#else
void ClientAIBSSMessage(void)
{
	u16	*MsgID;

	ScrX0=0;
	ScrY0=0;

	MsgID=(u16 *)&ServerBuffer[ClientNo][2];
	FightMsgID(MsgID[0]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientAIMessageWait;
}
#endif

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

void ClientAIBSSAlertMessage(void)
{
//	if(MineEnemyCheck(ClientNo)==SIDE_MINE)
//		ClientAIBSSMessage();
//	else
		ClientAIBitReset();
}

//----------------------------------------------------------
//	����ޑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientAIBSSCommandSelect(void)
{
	ClientAICommandSelectAI();
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽClientNo�̃V�[�P���X�i���o�[�Ɠ������Ƃ�
//----------------------------------------------------------
void ClientAIBSSCommandSelectCopy(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�Z�I����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientAIBSSWazaSelect(void)
{
	u8	i;
	u16	wazano;
	u16	dir;
	WazaList	*WL;

	WL=(WazaList *)&ServerBuffer[ClientNo][4];

#ifdef PM_DEBUG
	if(DebugFightFlag&DEBUG_FIGHT_WAZA_DEBUG){
		wazano=0;
		while(wazano==0){
//			wazano=PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],
//							   ID_waza1+UserWork[WAZA_DEBUG_WORK+((ClientNo&2)>>1)]);
			wazano=WL->wazano[UserWork[WAZA_DEBUG_WORK+((ClientNo&2)>>1)]];
			i=UserWork[WAZA_DEBUG_WORK+((ClientNo&2)>>1)];
			if(wazano)
				UserWork[WAZA_DEBUG_WORK+((ClientNo&2)>>1)]++;
			else
				UserWork[WAZA_DEBUG_WORK+((ClientNo&2)>>1)]=0;

			if(UserWork[WAZA_DEBUG_WORK+((ClientNo&2)>>1)]>3)
				UserWork[WAZA_DEBUG_WORK+((ClientNo&2)>>1)]=0;
		}
		if(WTD[wazano].attackrange&(RANGE_ALWAYS|RANGE_MINE))
			dir=ClientNo;
		else{
			if((FightType&FIGHT_TYPE_2vs2)&&
			   (DebugFightFlag&DEBUG_FIGHT_PARTY_ATTACK)){
				if(WTD[wazano].attackrange)
					dir=ClientNoGet(BSS_CLIENT_MINE);
				else
					dir=ClientNoGet(ClientTypeGet(ClientNo)^2);
			}
			else if(FightType&FIGHT_TYPE_2vs2)
				dir=pp_rand()&2;
			else
				dir=ClientNoGet(BSS_CLIENT_MINE);
		}

		BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,i|(dir<<8));
		ClientAIBitReset();
	}
	else if(FightType&FIGHT_TYPE_AI){
#else
	if(FightType&FIGHT_TYPE_AI){
#endif
		WazaAIInit2();
		i=WazaAIMain();
		switch(i){
			case AI_ENEMY_SAFARI:
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_SAFARI,0);
				break;
			case AI_ENEMY_ESCAPE:
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_ESCAPE,0);
				break;
			default:
//				if(WTD[PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_waza1+i)].attackrange&(RANGE_ALWAYS|RANGE_MINE))
				if(WTD[WL->wazano[i]].attackrange&(RANGE_ALWAYS|RANGE_MINE))
					DefenceClient=ClientNo;
				if(WTD[WL->wazano[i]].attackrange&RANGE_DOUBLE){
					DefenceClient=ClientNoGet(BSS_CLIENT_MINE);
					if(NoReshuffleClient&no2bittbl[DefenceClient])
						DefenceClient=ClientNoGet(BSS_CLIENT_MINE2);
				}
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,i|(DefenceClient<<8));
				break;
		}
		ClientAIBitReset();
	}
	else{
		while(1){
			i=pp_rand()&0x03;
//			wazano=PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_waza1+i);
			wazano=WL->wazano[i];
			if(wazano!=0) break;
		}
		if(WTD[wazano].attackrange&(RANGE_ALWAYS|RANGE_MINE))
			BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,i|ClientNo<<8);
		else{
			if(FightType&FIGHT_TYPE_2vs2){
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,i|ClientNoGet(BSS_CLIENT_MINE+(pp_rand()&2))<<8);
			}
			else
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,i|ClientNoGet(BSS_CLIENT_MINE)<<8);
		}
		ClientAIBitReset();
	}
}

//----------------------------------------------------------
//	���ёI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientAIBSSItemSelect(void)
{
	BSS_USE_ITEM_SET(BSS_CLIENT,UserWork[AI_ITEM_NO+(ClientNo>>1)*2]);
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�߹�ݑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientAIBSSPokeSelect(void)
{
	int i;
	u8	no1,no2;

	if(UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]==6){
		if((i=ClientAIPokeSelectAI())==6){
			if((FightType&FIGHT_TYPE_2vs2)==0)
				no2=no1=ClientNoGet(BSS_CLIENT_ENEMY);
			else{
				no1=ClientNoGet(BSS_CLIENT_ENEMY);
				no2=ClientNoGet(BSS_CLIENT_ENEMY2);
				}
			for(i=0;i<6;i++){
				if((PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
				(i!=SelMonsNo[no1])&&(i!=SelMonsNo[no2])) break;
			}
		}
	}
	else{
		i=UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)];
		UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=6;
	}
	UserWork[AFTER_RESHUFFLE_SELMONSNO+ClientNo]=i;
	BSS_POKE_RESHUFFLE_SET(BSS_CLIENT,i,0);
	ClientAIBitReset();
}

//----------------------------------------------------------
//	������̪�Ă��N������
//----------------------------------------------------------
void ClientAIBSSEscape(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	HP�ް�ނ𑝌�����
//----------------------------------------------------------
void	ClientAIBSSHpGauge(void)
{
	s16 beHP;	//��������l
	
	BattleGaugeTransWram(0);
	beHP = (ServerBuffer[ClientNo][3] << 8) | ServerBuffer[ClientNo][2];

	if(beHP!=JIBAKU_HP)
		GaugeDataSet(ClientNo, ClientGaugeSt[ClientNo],
				 PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_hpmax),
				 PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_hp),
				 beHP);
	else
		GaugeDataSet(ClientNo, ClientGaugeSt[ClientNo],
				 PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_hpmax),
				 0, beHP);

	ClientSequence[ClientNo] = ClientAIHpGaugeWait;
}

//----------------------------------------------------------
//	EXP�ް�ނ𑝌�����
//----------------------------------------------------------
void ClientAIBSSExpGauge(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ�̪�Ă��N������
//----------------------------------------------------------
void ClientAIBSSStatusSet(void)
{
	u32 condition;
	u8 condition_flg;

	if(ClientSEWait(ClientNo))
		return;

	GaugeTransChrSet(ClientGaugeSt[ClientNo], &PokeParaEnemy[SelMonsNo[ClientNo]], G_STATUS);
	condition_flg = ServerBuffer[ClientNo][1];
	condition = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8) | 
				(ServerBuffer[ClientNo][4]<<16) | (ServerBuffer[ClientNo][5]<<24);

//	ClientStatusEffSet(condition_flg, condition);	2002.09.07(�y)
	CliSys[ClientNo].status_eff = 0;
	
	ClientSequence[ClientNo] = ClientAIStatusEffWait;
}

void ClientAIBSSStatusEffect(void)
{
	u32 condition;
	u8 condition_flg;

	if(ClientSEWait(ClientNo))
		return;

	condition_flg = ServerBuffer[ClientNo][1];
	condition = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8) | 
				(ServerBuffer[ClientNo][4]<<16) | (ServerBuffer[ClientNo][5]<<24);
	ClientStatusEffSet(condition_flg, condition);
	ClientSequence[ClientNo] = ClientAIStatusEffWait;
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ��ر����
//----------------------------------------------------------
void ClientAIBSSStatusClear(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	���炩���ߗp�ӂ��ꂽ�ޯ̧���ް����i�[����
//----------------------------------------------------------
void ClientAIBSSDataTrans(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�w����ڽ(RAM,VRAM,CGRAM)���ް���]������
//----------------------------------------------------------
void ClientAIBSSDataRamTrans(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	���ް���]������
//----------------------------------------------------------
void ClientAIBSSBgmTrans(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�����ް���]������
//----------------------------------------------------------
void ClientAIBSSVoiceTrans(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�ƭ��őI����������ID��Ԃ�
//----------------------------------------------------------
void ClientAIBSSSelectReturn(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�߹�ݓ���ւ����s�������Ƃ�m�点��
//----------------------------------------------------------
void ClientAIBSSReshuffle(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	���юg�p��m�点��
//----------------------------------------------------------
void ClientAIBSSUseItem(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�ްَg�p��m�点��
//----------------------------------------------------------
void ClientAIBSSUseBall(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃N���A
//----------------------------------------------------------
void	ClientAIBSSLoopCountClear(void)
{
	WEW.loopcnt = 0;
	ClientAIBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃Z�b�g
//----------------------------------------------------------
void	ClientAIBSSLoopCountSet(void)
{
	WEW.loopcnt = ServerBuffer[ClientNo][1];
	ClientAIBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃N���A
//----------------------------------------------------------
void	ClientAIBSSSwitchFlagClear(void)
{
	WEW.flag = 0;
	ClientAIBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃Z�b�g
//----------------------------------------------------------
void	ClientAIBSSSwitchFlagSet(void)
{
	WEW.flag ^= 1;
	ClientAIBitReset();
}

//----------------------------------------------------------
//	��Ұ�ނ��󂯂��Ƃ����߹�ݓ_��
//----------------------------------------------------------
void ClientAIBSSPokeDamageBlink(void)
{
	//�߹�݂���Ư����ԂȂ��Blink���Ȃ�
	if(ActWork[PokemonStruct[ClientNo]].banish == 1){
		ClientAIBitReset();
		return;
	}

	ClientEffectWait = 1;	//Blink�̪�ĊJ�n�t���O�Z�b�g
	ActWork[PokemonStruct[ClientNo]].work[1] = 0;

	//-- �ް�ނ̃J���[�ύX & �h�炵�G�t�F�N�g�Z�b�g --//
	GaugeSwayEffect(ClientNo);

	ClientSequence[ClientNo] = ClientAIPokeDamageBlinkWait;
}

//----------------------------------------------------------
//	�Q�[�W������
//----------------------------------------------------------
void ClientAIBSSGaugeOff(void)
{
//	GaugeOFF(ClientGaugeSt[ClientNo]);
	ClientAIBitReset();
}

//----------------------------------------------------------
//	SE��炷
//----------------------------------------------------------
void ClientAIBSSSEPlay(void)
{
	s8 pan;

//	SePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	if(MineEnemyCheck(ClientNo) == SIDE_MINE)
		pan = WAZA_SE_L;
	else
		pan = WAZA_SE_R;
	SePlayPan(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8), pan);
	ClientAIBitReset();

}

//----------------------------------------------------------
//	ME��炷
//----------------------------------------------------------
void ClientAIBSSMEPlay(void)
{
	JinglePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	ClientAIBitReset();
}

//----------------------------------------------------------
//	������炷
//----------------------------------------------------------

void ClientAIBSSVoicePlay(void)
{
//	VoicePlay(PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_monsno),25);
	//�C�⎞�̐��Ŗ��悤�ɕύX 2002.08.27(��) by matsuda
	VoicePlay3(PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_monsno),25,5);
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�G���J�E���g�G�t�F�N�g���Z�b�g
//----------------------------------------------------------
void ClientAIBSSEncountEffect(void)
{
	EncountEffectSet(ServerBuffer[ClientNo][1]);
	//OBJ���ړ��̃X�g�b�p�[
	EncountEffectFlag|=ENCOUNT_EFFECT_FLAG_OBJSTOP;	
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�ŏ��̃|�P�����o��(�ڰŰ�������ށA�߹�ݏo���Aү���ޏo���B�܂ł̈�A�̓���
//----------------------------------------------------------
void ClientAIBSSPokeSendOut(void)
{
	u8 id;
	
	//-- �ڰŰ --//
	DxDyAddClear(&ActWork[PokemonStruct[ClientNo]]);
	ActWork[PokemonStruct[ClientNo]].work[0] = ENEMYTRAINER_RETURN_SYNCNUM;
	ActWork[PokemonStruct[ClientNo]].work[2] = 240+40;
	ActWork[PokemonStruct[ClientNo]].work[4] = ActWork[PokemonStruct[ClientNo]].y;
	ActWork[PokemonStruct[ClientNo]].move = W_StraightSyncMove;
	SetWorkSeq(&ActWork[PokemonStruct[ClientNo]], AITrainerDelEnd);
//	ActAnmChg(&ActWork[PokemonStruct[ClientNo]],1);

	//-- �߹�� --//
	id = AddTask(AISendOutTask, 5);
	TaskTable[id].work[0] = ClientNo;

	//-- �莝���ް�ނ����� --//
	if(CliSys[ClientNo].temoti_gauge_flg)
		TaskTable[TemotiGauge[ClientNo]].TaskAdrs = TemotiBallOff;

	BtlWork->sendout_eff = 1;
	ClientSequence[ClientNo] = ClientAIDummy;
}

static void AITrainerDelEnd(actWork *xreg)
{
	TrainerFObjCellPalDel(xreg->oamData.AffineParam);
	xreg->oamData.CharNo = xreg->work[5];	//DelActor���鎞�ɓ]����
						//����Ă����̈悪�J�������悤��CharNo�����ɖ߂�
	AffineDelActor(xreg);
	DelActor(xreg);
}

static void AISendOutTask(u8 id)
{
	u8 clientno_temp;
	
/*	if(TaskTable[id].work[1] < 24){		//�߹�ݓo��܂ł�wait
		TaskTable[id].work[1]++;
		return;
	}
*/

	clientno_temp = ClientNo;
	ClientNo = TaskTable[id].work[0];
	//-- �߹�� --//
	if(FightCheck() == 0)
	{	//1vs1
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		AIPokeAppearCommon(ClientNo, 0);	//ClientAIBSSPokeAppear();
	}
	else if(FightType&FIGHT_TYPE_4_2vs2)
	{	//2vs2 4�l�ʐM
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		AIPokeAppearCommon(ClientNo, 0);
	}
	else
	{	//2vs2
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		AIPokeAppearCommon(ClientNo, 0);	//ClientAIBSSPokeAppear();
		ClientNo ^= 2;
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		AIPokeAppearCommon(ClientNo, 0);	//ClientAIBSSPokeAppear();
		ClientNo ^= 2;	//�ύX����ClientNo�����ɖ߂�
	}

	ClientSequence[ClientNo] = ClientAIPokeSendOutWait;
	ClientNo = clientno_temp;
	DelTask(id);
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W��\������
//----------------------------------------------------------

void	ClientAIBSSBallGaugeSet(void)
{
	BallGaugePara *bgp;
	
//�t���O���[�N��true�̎��́A�ǂ����T�C�h�Ȃ̂��`�F�b�N���āA�������̂Ƃ��͕\�������Ȃ��悤�ɂ���
	if(ServerBuffer[ClientNo][1]){
		if(MineEnemyCheck(ClientNo)==SIDE_MINE){
			ClientAIBitReset();
			return;
		}
	}
	CliSys[ClientNo].temoti_gauge_flg = 1;

	//-- ENEMY����̏��� --//
	if(ServerBuffer[ClientNo][2]){	//����̎��͏����x�点�ďo��
		if(CliSys[ClientNo].temoti_wait < ENEMY_TEMOTI_INIT_WAIT){
			CliSys[ClientNo].temoti_wait++;
			return;
		}
		CliSys[ClientNo].temoti_wait = 0;
	}

	bgp = (BallGaugePara*)&ServerBuffer[ClientNo][4];
	TemotiGauge[ClientNo] = TemotiBallPut(ClientNo, bgp, ServerBuffer[ClientNo][1], ServerBuffer[ClientNo][2]);
	CliSys[ClientNo].wait_work = 0;
	if(ServerBuffer[ClientNo][2]){	//�׸ނ������Ă����糪�ĂȂ�
		CliSys[ClientNo].wait_work = TEMOTI_BALL_WAIT+1;
	}
	ClientSequence[ClientNo] = ClientAIBallGaugeWait;
//	ClientAIBitReset();
}

static void ClientAIBallGaugeWait(void)
{
	if(CliSys[ClientNo].wait_work++ > TEMOTI_BALL_WAIT){
		CliSys[ClientNo].wait_work = 0;
		ClientAIBitReset();
	}
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W���\���ɂ���
//----------------------------------------------------------

void	ClientAIBSSBallGaugeOff(void)
{
	//-- �莝���ް�ނ����� --//
	if(CliSys[ClientNo].temoti_gauge_flg)
		TaskTable[TemotiGauge[ClientNo]].TaskAdrs = TemotiBallOff;
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�Q�[�W�̂����~�߂�
//----------------------------------------------------------

void ClientAIBSSGaugeYureStop(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�|�P������VANISH��ON/OFF����
//----------------------------------------------------------

void	ClientAIBSSVanishOnOff(void)
{
	if(ClientPokeExistCheck(ClientNo)){
		ActWork[PokemonStruct[ClientNo]].banish = ServerBuffer[ClientNo][1];
		PokeClientBanishMemory(ClientNo);
	}
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�퓬�G�t�F�N�g���o��
//----------------------------------------------------------

void	ClientAIBSSFightEffect(void)
{
	u8 eff_no;
	u16 monsno;

	if(ClientSEWait(ClientNo))
		return;
	
	eff_no = ServerBuffer[ClientNo][1];
	monsno = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8);
	
	if(SetFightEffect(ClientNo, ClientNo, ClientNo, eff_no, monsno))
		ClientAIBitReset();
	else
		ClientSequence[ClientNo] = ClientAIFightEffWait;
}

//----------------------------------------------------------
//	�ʐM�ҋ@��ʂ��o��
//----------------------------------------------------------

void	ClientAIBSSTuushinWait(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�L�����Ă���J�[�\���ʒu�̃N���A
//----------------------------------------------------------

void	ClientAIBSSCsrPosClear(void)
{
	ClientAIBitReset();
}

//----------------------------------------------------------
//	�ΐ�I����m�点��
//----------------------------------------------------------
void ClientAIBSSFightEnd(void)
{
	if((FightType&FIGHT_TYPE_TUUSHIN)&&((FightType&FIGHT_TYPE_OYA)==0)){
		sys.FieldBattleFlag=0;			//�퓬��Flag�����Ƃ�
		sys.pFirstProc=PushFirstProc;
		MainProcChange(sys.pReturnProc);
	}
	ClientAIBitReset();
}

//----------------------------------------------------------
//	��а
//----------------------------------------------------------
void ClientAIBSSEndID(void)
{
//	ClientAIBitReset();
}

//----------------------------------------------------------
//			AI���[�`��
//----------------------------------------------------------

//�ق�т̂����J�E���^�`�F�b�N
u8	ClientAIHorobinoutaCheck(void)
{
	if((WazaKouka[ClientNo]&WAZAKOUKA_HOROBINOUTA)&&
	   (WKC[ClientNo].horobinoutacnt==0)){
		UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=6;
		BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_POKE,0);
		return 1;
	}
	return 0;
}

//�ӂ����Ȃ܂���`�F�b�N
u8	ClientAIHusiginamamoriCheck(void)
{
	int	i,j;
	u16	monsno;
	u16	wazano;
	u8	speabino;
	u8	clientno;
	u8	flag;

	if(FightType&FIGHT_TYPE_2vs2)
		return 0;

	if(PSP[ClientNoGet(BSS_CLIENT_MINE)].speabino==TOKUSYU_HUSIGINAMAMORI){
		clientno=ClientNoGet(BSS_CLIENT_MINE);
		for(i=0;i<4;i++){
			wazano=PSP[ClientNo].waza[i];
			if(wazano){
				flag=TypeCheckAct2(wazano,
								   PSP[clientno].monsno,
								   PSP[clientno].speabino);
				if(flag&WAZASTATUSFLAG_BATSUGUN)
					return 0;
			}
		}
		for(i=0;i<6;i++){
			if((PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
			   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=0)&&
			   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=MONSNO_TAMAGO)&&
			   (i!=SelMonsNo[ClientNo])){
				monsno=PokeParaGet(&PokeParaEnemy[i],ID_monsno);
				if(PokeParaGet(&PokeParaEnemy[i],ID_speabi))
					speabino=PPD[monsno].speabi2;
				else
					speabino=PPD[monsno].speabi1;
				clientno=ClientNoGet(BSS_CLIENT_MINE);
				for(j=0;j<4;j++){
					wazano=PokeParaGet(&PokeParaEnemy[i],ID_waza1+j);
					if(wazano){
						flag=TypeCheckAct2(wazano,
										   PSP[clientno].monsno,
										   PSP[clientno].speabino);
						if(flag&WAZASTATUSFLAG_BATSUGUN){
							if((pp_rand()%3)<2){
								UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=i;
								BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_POKE,0);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

//�_���[�W�Z�ɑ΂���HP�񕜂�������������Ă���|�P�����̃`�F�b�N
u8	ClientAIHPRecoverTokusyuCheck(void)
{
	int	i;
	u8	no1,no2;
	u16	monsno;
	u8	speabino;
	u8	chkspeabino;

	if((ClientAIBatsugunCheck(1))&&(pp_rand()%3!=0))
		return 0;

	if((WazaNoHit[ClientNo]==0)||(WazaNoHit[ClientNo]==0xffff))
		return 0;
	if(WTD[WazaNoHit[ClientNo]].damage==0)
		return 0;

	if(FightType&FIGHT_TYPE_2vs2){
		no1=ClientNo;
		if(NoReshuffleClient&no2bittbl[ClientNoGet(ClientTypeGet(ClientNo)^2)])
			no2=ClientNo;
		else
			no2=ClientNoGet(ClientTypeGet(ClientNo)^2);
	}
	else{
		no1=ClientNo;
		no2=ClientNo;
	}

	if(WTD[WazaNoHit[ClientNo]].wazatype==FIRE_TYPE)
		chkspeabino=TOKUSYU_MORAIBI;
	else if(WTD[WazaNoHit[ClientNo]].wazatype==WATER_TYPE)
		chkspeabino=TOKUSYU_TYOSUI;
	else if(WTD[WazaNoHit[ClientNo]].wazatype==ELECTRIC_TYPE)
		chkspeabino=TOKUSYU_TIKUDEN;
	else
		return 0;

	if(PSP[ClientNo].speabino==chkspeabino)
		return 0;

	for(i=0;i<6;i++){
		if((PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=MONSNO_TAMAGO)&&
		   (i!=SelMonsNo[no1])&&(i!=SelMonsNo[no2])&&
		   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no1])&&
		   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no2])){
			monsno=PokeParaGet(&PokeParaEnemy[i],ID_monsno);
			if(PokeParaGet(&PokeParaEnemy[i],ID_speabi)){
				speabino=PPD[monsno].speabi2;
			}
			else
				speabino=PPD[monsno].speabi1;
			if((chkspeabino==speabino)&&(pp_rand()&1)){
				UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=i;
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_POKE,0);
				return 1;
			}
		}
	}
	return 0;
}

//���o�Ă���|�P�����������Ă��Ă����񂩂��ӂ��������Ă���ꍇ�̃`�F�b�N
u8		ClientAIShizenkaifukuCheck(void)
{
	if(((PSP[ClientNo].condition&CONDITION_NEMURI)==0)||
		(PSP[ClientNo].speabino!=TOKUSYU_SIZENKAIHUKU)||
		(PSP[ClientNo].hp<(PSP[ClientNo].hpmax/2)))
		return 0;

	if((WazaNoHit[ClientNo]==0)||(WazaNoHit[ClientNo]==0xffff)){
		if(pp_rand()&1){
			UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=6;
			BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_POKE,0);
			return 1;
		}
	}
	if(WTD[WazaNoHit[ClientNo]].damage==0){
		if(pp_rand()&1){
			UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=6;
			BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_POKE,0);
			return 1;
		}
	}

	if(ClientAIWazaKoukaCheck(WAZASTATUSFLAG_KOUKANAI,1))
		return 1;
	if(ClientAIWazaKoukaCheck(WAZASTATUSFLAG_IMAHITOTSU,1))
		return 1;

	if(pp_rand()&1){
		UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=6;
		BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_POKE,0);
		return 1;
	}
	return 0;
}

//���o�Ă���|�P���������ʔ��Q�̋Z�������Ă���ꍇ�͌�サ�Ȃ��`�F�b�N
//chkflag��1�̂Ƃ������Ă��邩�����`�F�b�N���Ȃ�
u8	ClientAIBatsugunCheck(u8 chkflag)
{
	int	i;
	u8	flag;
	u8	clientno;
	u16	wazano;

	clientno=ClientNoGet(BSS_CLIENT_MINE);
	if((NoReshuffleClient&no2bittbl[clientno])==0){
		for(i=0;i<4;i++){
			wazano=PSP[ClientNo].waza[i];
			if(wazano){
				flag=TypeCheckAct2(wazano,
								   PSP[clientno].monsno,
								   PSP[clientno].speabino);
				if(flag&WAZASTATUSFLAG_BATSUGUN){
					if(chkflag)
						return 1;
					else{
						if(pp_rand()%10!=0)
							return 1;
					}
				}
			}
		}
	}
	if((FightType&FIGHT_TYPE_2vs2)==0)
		return 0;
	clientno=ClientNoGet(BSS_CLIENT_MINE2);
	if((NoReshuffleClient&no2bittbl[clientno])==0){
		for(i=0;i<4;i++){
			wazano=PSP[ClientNo].waza[i];
			if(wazano){
				flag=TypeCheckAct2(wazano,
								   PSP[clientno].monsno,
								   PSP[clientno].speabino);
				if(flag&WAZASTATUSFLAG_BATSUGUN){
					if(chkflag)
						return 1;
					else{
						if(pp_rand()%10!=0)
							return 1;
					}
				}
			}
		}
	}
	return 0;
}

//���o�Ă���|�P�����̃X�e�[�^�X�A�b�v�g�[�^����4�ȏ�͌�サ�Ȃ��`�F�b�N
u8	ClientAIStatusUpCheck(void)
{
	int	i;
	u8	cnt;

	cnt=0;
	for(i=COND_HP;i<COND_MAX;i++){
		if(PSP[ClientNo].abiritycnt[i]>6)
			cnt+=PSP[ClientNo].abiritycnt[i]-6;
	}
	return (cnt>=4);
}


u8	ClientAIWazaKoukaCheck(u8 wazakouka,u8 kakuritu)
{
	int	i,j;
	u8	no1,no2;
	u16	monsno;
	u16	wazano;
	u8	speabino;
	u8	flag;

	if((WazaNoHit[ClientNo]==0)||
	   (WazaNoHit[ClientNo]==0xffff)||
	   (WazaNoAttackClient[ClientNo]==0xff))
		return 0;
	if(WTD[WazaNoHit[ClientNo]].damage==0)
		return 0;

	if(FightType&FIGHT_TYPE_2vs2){
		no1=ClientNo;
		if(NoReshuffleClient&no2bittbl[ClientNoGet(ClientTypeGet(ClientNo)^2)])
			no2=ClientNo;
		else
			no2=ClientNoGet(ClientTypeGet(ClientNo)^2);
	}
	else{
		no1=ClientNo;
		no2=ClientNo;
	}

	for(i=0;i<6;i++){
		if((PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=MONSNO_TAMAGO)&&
		   (i!=SelMonsNo[no1])&&(i!=SelMonsNo[no2])&&
		   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no1])&&
		   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no2])){
			monsno=PokeParaGet(&PokeParaEnemy[i],ID_monsno);
			if(PokeParaGet(&PokeParaEnemy[i],ID_speabi))
				speabino=PPD[monsno].speabi2;
			else
				speabino=PPD[monsno].speabi1;
			flag=TypeCheckAct2(WazaNoHit[ClientNo],monsno,speabino);
			if(flag&wazakouka){
				no1=WazaNoAttackClient[ClientNo];
				for(j=0;j<4;j++){
					wazano=PokeParaGet(&PokeParaEnemy[i],ID_waza1+j);
					if(wazano){
						flag=TypeCheckAct2(wazano,
										   PSP[no1].monsno,
										   PSP[no1].speabino);
						if(flag&WAZASTATUSFLAG_BATSUGUN){
							if((pp_rand()%kakuritu)==0){
								UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=i;
								BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_POKE,0);
								return 1;
							}

						}
					}
				}
			}
		}
	}
	return 0;
}

//�|�P��������ւ�AI
u8	ClientAIPokeReshuffleAI(void)
{
	int	i;
	int	cnt;
	u8	no1,no2,selected;

	if((PSP[ClientNo].condition2&(CONDITION2_SHIME|CONDITION2_KUROIMANAZASHI))||
	   (WazaKouka[ClientNo]&WAZAKOUKA_NEWOHARU)||
	   (SpecialAbirityCheck(SPEABI_ENEMYHAVE_SPEABI,ClientNo,TOKUSYU_KAGEHUMI,0,0)||
	   (SpecialAbirityCheck(SPEABI_ENEMYHAVE_SPEABI,ClientNo,TOKUSYU_ARIZIGOKU,0,0))||
	  ((SpecialAbirityCheck(SPEABI_HAVE_SPEABI,0,TOKUSYU_ZIRYOKU,0,0))&&
	  ((PSP[ClientNo].type1==METAL_TYPE)||(PSP[ClientNo].type2==METAL_TYPE)))))
		return 0;

	cnt=0;
	if(FightType&FIGHT_TYPE_2vs2){
		no1=ClientNo;
		selected=6;
		if(NoReshuffleClient&no2bittbl[ClientNoGet(ClientTypeGet(ClientNo)^2)])
			no2=ClientNo;
		else
			no2=ClientNoGet(ClientTypeGet(ClientNo)^2);
	}
	else{
		no1=ClientNo;
		no2=ClientNo;
	}

	for(i=0;i<6;i++){
		if((PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=MONSNO_TAMAGO)&&
		   (i!=SelMonsNo[no1])&&(i!=SelMonsNo[no2])&&
		   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no1])&&
		   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no2]))
			cnt++;
	}
	if(cnt){
		if(ClientAIHorobinoutaCheck())
			return 1;
		if(ClientAIHusiginamamoriCheck())
			return 1;
		if(ClientAIHPRecoverTokusyuCheck())
			return 1;
		if(ClientAIShizenkaifukuCheck())
			return 1;
		if(ClientAIBatsugunCheck(0))
			return 0;
		if(ClientAIStatusUpCheck())
			return 0;
		if(ClientAIWazaKoukaCheck(WAZASTATUSFLAG_KOUKANAI,2))
			return 1;
		if(ClientAIWazaKoukaCheck(WAZASTATUSFLAG_IMAHITOTSU,3))
			return 1;
	}

	return 0;
}

//�R�}���h�I��AI
void	ClientAICommandSelectAI(void)
{
	int	i;
	u8	no1,no2;

	if(FightType&FIGHT_TYPE_TRAINER){
		if(ClientAIPokeReshuffleAI()){
			if(UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]==6){
				if((i=ClientAIPokeSelectAI())==6){
					if((FightType&FIGHT_TYPE_2vs2)==0)
						no2=no1=ClientNoGet(BSS_CLIENT_ENEMY);
					else{
						no1=ClientNoGet(BSS_CLIENT_ENEMY);
						no2=ClientNoGet(BSS_CLIENT_ENEMY2);
						}
					for(i=0;i<6;i++){
						if((PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
						   (i!=SelMonsNo[no1])&&(i!=SelMonsNo[no2])&&
						   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no1])&&
						   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no2]))
							break;
					}
				}
				UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)]=i;
			}
			UserWork[AFTER_RESHUFFLE_SELMONSNO+ClientNo]=
				UserWork[AI_RESHUFFLE_SELNO+(ClientTypeGet(ClientNo)>>1)];
			return;
		}
#ifdef PM_DEBUG
		if((DebugFightFlag&DEBUG_FIGHT_NO_ITEM_AI)==0){
			if(ClientAIItemUseAI())
				return;
		}
#else
		if(ClientAIItemUseAI())
			return;
#endif
	}
	BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_FIGHT,0|((ClientNo^1)<<8));
}

static	void	ClientAIPokeAisyouCheck(u8 type,u8 type1,u8 type2,u8 *damage)
{
	int	i=0;

	while(TypeCheckTable[i]!=0xff){
		if(TypeCheckTable[i]==0xfe){		//�݂�Ԃ�`�F�b�N
			i+=3;
			continue;
		}
		if(TypeCheckTable[i]==type){
			if(TypeCheckTable[i+1]==type1)
				damage[0]=damage[0]*TypeCheckTable[i+2]/10;
			if((TypeCheckTable[i+1]==type2)&&(type1!=type2))
				damage[0]=damage[0]*TypeCheckTable[i+2]/10;
		}
		i+=3;
	}
}

//�|�P�����I��AI
u8		ClientAIPokeSelectAI(void)
{
	int	i,j;
	u8	clientno;
	u8	type1;
	u8	type2;
	u16	monsno;
	u16	wazano;
	u8	checkbit;
	u8	damage;
	u8	damagetmp;
	u8	topselmons=6;
	u8	no1,no2;

	if(FightType&FIGHT_TYPE_2vs2){
		no1=ClientNo;
		if(NoReshuffleClient&no2bittbl[ClientNoGet(ClientTypeGet(ClientNo)^2)])
			no2=ClientNo;
		else
			no2=ClientNoGet(ClientTypeGet(ClientNo)^2);
		clientno=pp_rand()&2;
		if(NoReshuffleClient&no2bittbl[clientno])
			clientno^=2;
	}
	else{
		clientno=ClientNoGet(BSS_CLIENT_MINE);
		no1=ClientNo;
		no2=ClientNo;
	}

	checkbit=0;
	while(checkbit!=0x3f){
		damagetmp=0;
		topselmons=6;
		for(i=0;i<6;i++){
			monsno=PokeParaGet(&PokeParaEnemy[i],ID_monsno);
			if((monsno!=0)&&
			   (PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
			  ((checkbit&no2bittbl[i])==0)&&
			   (SelMonsNo[no1]!=i)&&
			   (SelMonsNo[no2]!=i)&&
			   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no1])&&
			   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no2])){
				type1=PPD[monsno].type1;
				type2=PPD[monsno].type2;
				damage=10;
				ClientAIPokeAisyouCheck(PSP[clientno].type1,
										type1,
										type2,
										&damage);
				ClientAIPokeAisyouCheck(PSP[clientno].type2,
										type1,
										type2,
										&damage);
				if(damagetmp<damage){
					damagetmp=damage;
					topselmons=i;
				}
			}
			else
				checkbit|=no2bittbl[i];
		}
		if(topselmons!=6){
			for(i=0;i<4;i++){
				wazano=PokeParaGet(&PokeParaEnemy[topselmons],ID_waza1+i);
				if(wazano){
					if(TypeCheckAct(wazano,ClientNo,clientno)&WAZASTATUSFLAG_BATSUGUN)
						break;
				}
			}
			if(i==4)
				checkbit|=no2bittbl[topselmons];
			else
				return topselmons;
		}
		else
			checkbit=0x3f;
	}

	DamagePowWork=0;
	UserWork[DAMAGE_TYPE_WORK]=0;
	UserWork[DAMAGE_VALUE]=1;
	WazaStatusFlag=0;
	CriticalValue=1;

	damagetmp=0;
	topselmons=6;

	for(i=0;i<6;i++){
		monsno=PokeParaGet(&PokeParaEnemy[i],ID_monsno);
		if((monsno!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
		   (SelMonsNo[no1]!=i)&&(SelMonsNo[no2]!=i)&&
		   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no1])&&
		   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no2])){
			for(j=0;j<4;j++){
				wazano=PokeParaGet(&PokeParaEnemy[i],ID_waza1+j);
				WazaDamage=0;
				if((wazano)&&(WTD[wazano].damage!=1)){
					DamageCalcAct(ClientNo,clientno);
					TypeCheckAct(wazano,ClientNo,clientno);
				}
				if(damagetmp<WazaDamage){
					damagetmp=WazaDamage;
					topselmons=i;
				}
			}
		}
	}
	return topselmons;
}

extern	const u8 * const ItemPokeParmTbl[];

static	u8	ItemTypeGet(u8 itemno,u8 *adr)
{
	if(itemno==ITEM_KAIHUKUNOKUSURI)
		return	ITEMTYPE_KAIHUKUNOKUSURI;
	if(adr[4]&0x04)
		return	ITEMTYPE_HP_KAIHUKU;
	if(adr[3]&0x3f)
		return	ITEMTYPE_CONDITION_KAIHUKU;
	if((adr[0]&0x3f)||(adr[1])||(adr[2]))
		return	ITEMTYPE_ABIRITY_UP;
	if(adr[3]&0x80)
		return	ITEMTYPE_EFFECT_GUARD;

	return	ITEMTYPE_UNKOWN;
}

//�A�C�e���g�pAI
u8		ClientAIItemUseAI(void)
{
	int	i;
	u8	monscnt=0;
	u16	itemno;
	u8	para;
	u8	ret=0;
	u8	*adr;
	u8	dir;


	for(i=0;i<6;i++){
		if((PokeParaGet(&PokeParaEnemy[i],ID_hp)!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=0)&&
		   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)!=MONSNO_TAMAGO))
			monscnt++;
	}

	for(i=0;i<4;i++){
		if((i==0)||(monscnt<=AIWT2->AI_ITEMCNT-i+1)){
			itemno=AIWT2->AI_HAVEITEM[i];
			if(itemno==0)
				continue;
			if(ItemPokeParmTbl[itemno-ITEM_NORMAL_START]==NULL)
				continue;
			if(itemno==ITEM_NAZONOMI)
				adr=(u8 *)&Fld.RamSeed.item_param;
			else
				adr=(u8 *)ItemPokeParmTbl[itemno-ITEM_NORMAL_START];

			UserWork[AI_ITEM_TYPE+(ClientNo>>1)]=ItemTypeGet(itemno,adr);
			switch(UserWork[AI_ITEM_TYPE+(ClientNo>>1)]){
				case ITEMTYPE_KAIHUKUNOKUSURI:
					if((PSP[ClientNo].hp<(PSP[ClientNo].hpmax/4))&&
					   (PSP[ClientNo].hp))
						ret=1;
					break;
				case ITEMTYPE_HP_KAIHUKU:
					para=ItemParamPosGet(itemno,4,0x04);
					if(para==0)
						break;
					if((PSP[ClientNo].hp)&&
					  ((PSP[ClientNo].hp<(PSP[ClientNo].hpmax/4))||
					  ((PSP[ClientNo].hpmax-PSP[ClientNo].hp)>adr[para])))
						ret=1;
					break;
				case ITEMTYPE_CONDITION_KAIHUKU:
					UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]=0;
					//����
					if(adr[3]&0x20){
						if(PSP[ClientNo].condition&CONDITION_NEMURI){
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x20;
							ret=1;
						}
					}
					//��
					if(adr[3]&0x10){
						if((PSP[ClientNo].condition&CONDITION_DOKU)||
						   (PSP[ClientNo].condition&CONDITION_DOKUDOKU)){
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x10;
							ret=1;
						}
					}
					//�Ώ�
					if(adr[3]&0x08){
						if(PSP[ClientNo].condition&CONDITION_YAKEDO){
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x08;
							ret=1;
						}
					}
					//�X
					if(adr[3]&0x04){
						if(PSP[ClientNo].condition&CONDITION_KOORI){
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x04;
							ret=1;
						}
					}
					//�}�q
					if(adr[3]&0x02){
						if(PSP[ClientNo].condition&CONDITION_MAHI){
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x02;
							ret=1;
						}
					}
					//����
					if(adr[3]&0x01){
						if(PSP[ClientNo].condition2&CONDITION2_KONRAN){
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x01;
							ret=1;
						}
					}
					break;
				case ITEMTYPE_ABIRITY_UP:
					UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]=0;
					if(WKC[ClientNo].nekodamashicnt){
						if(adr[0]&0x0f)
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x01;
						if(adr[1]&0xf0)
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x02;
						if(adr[1]&0x0f)
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x04;
						if(adr[2]&0x0f)
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x08;
						if(adr[2]&0xf0)
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x20;
						if(adr[0]&0x30)
							UserWork[AI_ITEM_CONDITION+(ClientNo>>1)]|=0x80;
						ret=1;
					}
					break;
				case ITEMTYPE_EFFECT_GUARD:
					dir=MineEnemyCheck(ClientNo);
					if((WKC[ClientNo].nekodamashicnt)&&
					   (SCC[dir].shiroikiricnt==0))
						ret=1;
					break;
				case ITEMTYPE_UNKOWN:
					return 0;
					break;
			}
			if(ret){
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_ITEM,0);
				UserWork[AI_ITEM_NO+(ClientNo>>1)*2]=itemno;
				AIWT2->AI_HAVEITEM[i]=0;
				return ret;
			}
		}
	}
	return 0;
}

