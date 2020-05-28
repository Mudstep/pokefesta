
//======================================================
//    clientcl.c                                           
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
#include "a_mine.h"
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
#include "wazano.def"
#include "waza_dat.h"
#include "a_fight.h"

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

void	ClientCLMBitReset(void);	//�͈͊O�̒l�̎��̏���
void	ClientCLMDummy(void);
void	ClientCLMInit(void);
void	ClientCLMCommandWait(void);
void	ClientCLMPokeEncountWait(void);

void	ClientCLMCommandSelect(void);
void	ClientCLMWazaSelect(void);
void	ClientCLMHpGaugeWait(void);
void	ClientCLMPokeKizetsuWait(void);
void	ClientCLMMessageWait(void);

void	ClientCLMBSSPokeparaGet(void);
void	ClientCLMBSSPokeparaGet2(void);
void	ClientCLMBSSPokeparaPut(void);
void	ClientCLMBSSPokeparaPut2(void);
void	ClientCLMBSSPokeEncount(void);
void	ClientCLMBSSPokeAppear(void);
void	ClientCLMBSSPokeReturn(void);
void	ClientCLMBSSTrainerEncount(void);
void	ClientCLMBSSTrainerAppear(void);
void	ClientCLMBSSTrainerReturn(void);
void	ClientCLMBSSPokeKizetu(void);
void	ClientCLMBSSZenmetsu(void);
void	ClientCLMBSSGetSuccess(void);
void	ClientCLMBSSGetFail(void);
void	ClientCLMBSSPokeLevelUp(void);
void	ClientCLMBSSWazaEffect(void);
void	ClientCLMBSSMessage(void);
void	ClientCLMBSSAlertMessage(void);
void	ClientCLMBSSCommandSelect(void);
void	ClientCLMBSSCommandSelectCopy(void);
void	ClientCLMBSSWazaSelect(void);
void	ClientCLMBSSItemSelect(void);
void	ClientCLMBSSPokeSelect(void);
void	ClientCLMBSSEscape(void);
void	ClientCLMBSSHpGauge(void);
void	ClientCLMBSSExpGauge(void);
void	ClientCLMBSSStatusSet(void);
void	ClientCLMBSSStatusEffect(void);
void	ClientCLMBSSStatusClear(void);
void	ClientCLMBSSDataTrans(void);
void	ClientCLMBSSDataRamTrans(void);
void	ClientCLMBSSBgmTrans(void);
void	ClientCLMBSSVoiceTrans(void);
void	ClientCLMBSSSelectReturn(void);
void	ClientCLMBSSReshuffle(void);
void	ClientCLMBSSUseItem(void);
void	ClientCLMBSSUseBall(void);
void	ClientCLMBSSLoopCountClear(void);
void	ClientCLMBSSLoopCountSet(void);
void	ClientCLMBSSSwitchFlagClear(void);
void	ClientCLMBSSSwitchFlagSet(void);
void	ClientCLMBSSPokeDamageBlink(void);
void	ClientCLMBSSGaugeOff(void);
void	ClientCLMBSSSEPlay(void);
void	ClientCLMBSSMEPlay(void);
void	ClientCLMBSSVoicePlay(void);
void	ClientCLMBSSEncountEffect(void);
void	ClientCLMBSSPokeSendOut(void);
void	ClientCLMBSSBallGaugeSet(void);
void	ClientCLMBSSBallGaugeOff(void);
void	ClientCLMBSSGaugeYureStop(void);
void	ClientCLMBSSVanishOnOff(void);
void	ClientCLMBSSFightEffect(void);
void	ClientCLMBSSTuushinWait(void);
void	ClientCLMBSSCsrPosClear(void);
void	ClientCLMBSSFightEnd(void);
void	ClientCLMBSSEndID(void);

int		ClientCLMBSSPokeparaGetAct(u8,u8 *);
void	ClientCLMBSSPokeparaPutAct(u8);
static void SendOutTask(u8 id);
static void CLMAppearCommon(u8 client_no, u8);
static void ClientCLMWazaEffectSet(void);
static void ClientCLMAppearMigawariEffWait(void);
static void ClientCLMBallGaugeWait(void);
static void ClientCLMPokeReturnMigawariChk(void);

extern	actHeader PokeFTbl[];
extern	TALK_WIN	FightMsg;
//extern	u8	FightMsgBuf[64];

//----------------------------------------------------------
//	�o�g���T�[�o�[���ߌQ�e�[�u��
//----------------------------------------------------------
const pFunc ClientCLMBSSTbl[] =
{
	ClientCLMBSSPokeparaGet,		// BSS_POKEPARA_GET=0,	
	ClientCLMBSSPokeparaGet2,	   	// BSS_POKEPARA_GET2,	
	ClientCLMBSSPokeparaPut,		// BSS_POKEPARA_PUT,	
	ClientCLMBSSPokeparaPut2,		// BSS_POKEPARA_PUT2,	
	ClientCLMBSSPokeEncount,		// BSS_POKEPARA_ENCOUNT,	add
	ClientCLMBSSPokeAppear,		// BSS_POKE_APPEAR,	
	ClientCLMBSSPokeReturn,		// BSS_POKE_RETURN,	
	ClientCLMBSSTrainerEncount,	// BSS_TRAINER_ENCOUNT,	add
	ClientCLMBSSTrainerAppear,		// BSS_TRAINER_APPEAR,	
	ClientCLMBSSTrainerReturn,		// BSS_TRAINER_RETURN,	
	ClientCLMBSSPokeKizetu,		// BSS_POKE_KIZETSU,	
	ClientCLMBSSZenmetsu,			// BSS_POKE_ZENMETSU,	
	ClientCLMBSSGetSuccess,		// BSS_GET_SUCCESS,	
	ClientCLMBSSGetFail,			// BSS_GET_FAIL,		
	ClientCLMBSSPokeLevelUp,		// BSS_POKE_LEVELUP,	
	ClientCLMBSSWazaEffect,		// BSS_WAZA_EFFECT,	
	ClientCLMBSSMessage,			// BSS_MESSAGE,		
	ClientCLMBSSAlertMessage,		// BSS_ALERT_MESSAGE,		
								
	ClientCLMBSSCommandSelect,		// BSS_COMMAND_SELECT, 

	ClientCLMBSSCommandSelectCopy,		// BSS_COMMAND_SELECT_COPY, 

	ClientCLMBSSWazaSelect,		// BSS_WAZA_SELECT,	
													
	ClientCLMBSSItemSelect,		// BSS_ITEM_SELECT,	
													
	ClientCLMBSSPokeSelect,		// BSS_POKE_SELECT,	
													
	ClientCLMBSSEscape,			// BSS_ESCAPE,			
	ClientCLMBSSHpGauge,			// BSS_HP_GAUGE,		
	ClientCLMBSSExpGauge,			// BSS_EXP_GAUGE,		
	ClientCLMBSSStatusSet,			// BSS_STATUS_SET,		
	ClientCLMBSSStatusEffect,		// BSS_STATUS_EFFECT,		
	ClientCLMBSSStatusClear,		// BSS_STATUS_CLEAR,	
	ClientCLMBSSDataTrans,			// BSS_DATA_TRANS,		
	ClientCLMBSSDataRamTrans,		// BSS_DATA_RAM_TRANS,	
													
	ClientCLMBSSBgmTrans,			// BSS_BGM_TRANS,		
	ClientCLMBSSVoiceTrans,		// BSS_VOICE_TRANS,	
	ClientCLMBSSSelectReturn,		// BSS_SELECT_RETURN,	
	ClientCLMBSSReshuffle,			// BSS_POKE_RESHUFFLE,	
	ClientCLMBSSUseItem,			// BSS_USE_ITEM,		
	ClientCLMBSSUseBall,			// BSS_USE_BALL,		
	ClientCLMBSSLoopCountClear,	//	BSS_LOOPCOUNT_CLEAR
	ClientCLMBSSLoopCountSet,	//	BSS_LOOPCOUNT_SET
	ClientCLMBSSSwitchFlagClear,	//	BSS_SWITCHFLAG_CLEAR
	ClientCLMBSSSwitchFlagSet,	//	BSS_SWITCHFLAG_SET
	ClientCLMBSSPokeDamageBlink,	// BSS_POKEDAMAGE_BLINK,
	ClientCLMBSSGaugeOff,		// BSS_GAUGE_OFF,
	ClientCLMBSSSEPlay,			// BSS_SE_PLAY,
	ClientCLMBSSMEPlay,			// BSS_SE_PLAY,
	ClientCLMBSSVoicePlay,			// BSS_VOICE_PLAY,
	ClientCLMBSSEncountEffect,	// BSS_ENCOUNT_EFFECT,
	ClientCLMBSSPokeSendOut,		// BSS_POKE_SENDOUT,
	ClientCLMBSSBallGaugeSet,	// BSS_BALL_GAUGE_SET,
	ClientCLMBSSBallGaugeOff,	// BSS_BALL_GAUGE_OFF,
	ClientCLMBSSGaugeYureStop,	// BSS_BALL_GAUGE_OFF,
	ClientCLMBSSVanishOnOff,		// BSS_VANISH_OFF,
	ClientCLMBSSFightEffect,	// BSS_FIGHT_EFFECT
	ClientCLMBSSTuushinWait,	// BSS_TUUSHIN_WAIT
	ClientCLMBSSCsrPosClear,		// BSS_CSRPOS_CLEAR,
	ClientCLMBSSFightEnd,		// BSS_FIGHT_END,

	ClientCLMBSSEndID,				// BSS_END_ID,		����Ȃ����ǈꉞ
};

//----------------------------------------------------------
//	�v���g�^�C�v�錾	�֐����ް�
//----------------------------------------------------------

#define	CAI_LVNUM_X			84
#define	CAI_LVNUM_Y			28

//==============================================================
//                       ���C�����[�`��
//==============================================================

void	ClientCLMDummy(void)
{
}

void	ClientCLMInit(void)
{
	ClientSequence[ClientNo]=ClientCLMCommandWait;

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

void	ClientCLMCommandWait(void)
{
	if((ClientBit&no2bittbl[ClientNo]) != 0)
	{
		if(ServerBuffer[ClientNo][0] <= BSS_END_ID)// && ServerBuffer[ClientNo][0] > -1)
		{
			ClientCLMBSSTbl[ServerBuffer[ClientNo][0]]();
		}
		else
		{
			ClientCLMBitReset();	//�͈͊O�̒l�̎��̏���
		}
	}
}

//=========================================================================
//	�E�F�C�g���[�`��
//=========================================================================
//
void ClientCLMTrainerEncountWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientCLMBitReset();
	}
}

void ClientCLMTrainerReturnWait(void)
{
#if 0
	if(ActWork[PokemonStruct[ClientNo]].dx > 64)
#else
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
#endif
	{
		TrainerBObjCellPalDel(0);
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);	//�ꉞ(�Ă�ł����͂Ȃ���)
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		ClientCLMBitReset();
	}
}

void ClientCLMSendOutWaitBitReset(void)
{
	if(CliSys[ClientNo].send_out_wait-- == 0){
		CliSys[ClientNo].send_out_wait = 0;
		ClientCLMBitReset();
	}
}

void ClientCLMSendOutGaugeWait(void)
{
#if 0	//2002.05.02(��)
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove
		&& ActWork[ClientGaugeSt[ClientNo^2]].move == DummyActMove)
	{
		ClientCLMBitReset();
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
	
	if(end){
		//ClientCLMBitReset();
//		if(FightType&FIGHT_TYPE_4_2vs2)
//			m4aMPlayContinue(&m4a_mplay000);
//		else
//			m4aMPlayVolumeControl(&m4a_mplay000,0xffff,256);
		CliSys[ClientNo].send_out_wait = 3;
		ClientSequence[ClientNo] = ClientCLMSendOutWaitBitReset;
	}
#endif
}

void ClientCLMPokeSendOutWait(void)
{
//	if(ActWork[ClientStruct[ClientNo]].move == DummyActMove)	//2�����ɓ����Ă�̂�1�����������Ă����Ζ��Ȃ�����
	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo^2].ball_eff_wait == 0)
	{
		if(CliSys[ClientNo].send_out_wait++ == 0)
			return;
		CliSys[ClientNo].send_out_wait = 0;
		
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
		
		DelActor(&ActWork[ClientStruct[ClientNo]]);			//�̪�ĊĎ�����DEL
		GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaMine[SelMonsNo[ClientNo]], G_ALL);
		GaugeAppearEffect(ClientNo);
		GaugeON(ClientGaugeSt[ClientNo]);

		//�ڰŰ�͎�����Del�����̂�
//		AffineDelActor(&ActWork[PokemonStruct[ClientNo^2]]);//�ꉞ(�Ă�ł����͂Ȃ���)
//		DelActor(&ActWork[PokemonStruct[ClientNo^2]]);
		
//		ClientCLMBitReset();
		BtlWork->sendout_eff = 0;
		ClientSequence[ClientNo] = ClientCLMSendOutGaugeWait;
	}
}


void	ClientCLMPokeEncountWait(void)
{
	if((ActWork[PokemonStruct[ClientNo]].anmend_sw==1)&&
	(ActWork[PokemonStruct[ClientNo]].dx==0)){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientCLMCommandWait;
		ClientCLMBitReset();
	}
}

void	ClientCLMHpGaugeWait(void)
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
		PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
		ClientCLMBitReset();
	}
}


void	ClientCLMPokeKizetsuWait(void)
{
#if 0	//2002.08.24(�y) by matsuda
	if(ActWork[PokemonStruct[ClientNo]].act_sw==0){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientCLMCommandWait;
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientCLMBitReset();
	}
#else
	if(ActWork[PokemonStruct[ClientNo]].y + ActWork[PokemonStruct[ClientNo]].dy > 160)
	{
//		PokemonBObjCellPalDel(PokeParaMine[SelMonsNo[ClientNo]].PPP.monsno);
//		changed by soga 2001.08.29
		PokemonBObjCellPalDel(
			PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno));
		AffineWorkNumDel(ActWork[PokemonStruct[ClientNo]].oamData.AffineParamNo);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientCLMBitReset();
	}
#endif
}

void ClientCLMPokeReturnWait(void)
{
	if(CliSys[ClientNo].etc_eff == 0)
	{
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientCLMBitReset();
	}
}

void	ClientCLMMessageWait(void)
{
	if(FightMsg.status==MSG_END){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientCLMCommandWait;
		ClientCLMBitReset();
	}
}

#if 0	// 2002.07.22(��)
void	ClientCLMWazaEffectWait(void)
{
	WazaEffectSequence();
	if(WazaEffectFlag==0){
		ClientCLMBitReset();
	}
}
#endif

void ClientCLMPokeDamageBlinkWait(void)
{
	u8 pokeact;
	
	pokeact = PokemonStruct[ClientNo];
	if(ActWork[pokeact].work[1] == DAMAGE_BLINK_WAIT)
	{
		ActWork[pokeact].work[1] = 0;
		ActWork[pokeact].banish = 0;
		ClientEffectWait = 0;	//Blink�̪�āA�t���O��ؾ��
		ClientCLMBitReset();
	}
	else
	{
		if(ActWork[pokeact].work[1] % BLINK_INTERVAL == 0)
			ActWork[pokeact].banish ^= 1;
		ActWork[pokeact].work[1]++;
	}
}

void ClientCLMGaugeAppearWait(void)
{
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove){
		//�g����`�F�b�N
		if(Climem[ClientNo].migawari)
			SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
		ClientSequence[ClientNo] = ClientCLMAppearMigawariEffWait;
	}
}

static void ClientCLMAppearMigawariEffWait(void)
{
	if(CliSys[ClientNo].etc_eff == 0)
		ClientCLMBitReset();
}

void ClientCLMPokeAppearWaitSub(void)
{
//	if(ActWork[PokemonStruct[ClientNo]].anmend_sw == 1)		��Ҏ~�߂� 2002.02.28(��)
	if(CliSys[ClientNo].rare_eff_end)
	{
		CliSys[ClientNo].rare_eff = 0;
		CliSys[ClientNo].rare_eff_end = 0;
		CellDel(WAZACHR_ID_SHOOTINGSTAR);
		ObjPalRegNumDel(WAZACHR_ID_SHOOTINGSTAR);

		AddTask(VoiceCheckTask, 10);
		PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);

		ActAnmChg(&ActWork[PokemonStruct[ClientNo]], 0);
		GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaMine[SelMonsNo[ClientNo]], G_ALL);
		GaugeAppearEffect(ClientNo);
		GaugeON(ClientGaugeSt[ClientNo]);
		PokeClientBanishMemory(ClientNo);
//		ClientCLMBitReset();
		ClientSequence[ClientNo] = ClientCLMGaugeAppearWait;
	}
}
void ClientCLMPokeAppearWait(void)
{
	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo].rare_eff == 0)
		RareEffectCheckSet(ClientNo, &PokeParaMine[SelMonsNo[ClientNo]]);

	if(ActWork[ClientStruct[ClientNo]].move == DummyActMove && CliSys[ClientNo].ball_eff_wait == 0)
	{
		DelActor(&ActWork[ClientStruct[ClientNo]]);					//�̪�ĊĎ�����DEL
//		ActAnmChg(&ActWork[PokemonStruct[ClientNo]], 1);	��Ҏ~�߂� 2002.02.28(��)
//		VoicePlay(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno),25);
		ClientSequence[ClientNo] = ClientCLMPokeAppearWaitSub;
	}
}

static void ClientFightEndWaitTuushin(void)
{
	if(pokemon_sio_flag==0){
		m4aSongNumStop(SE_HINSI);		//HP���Ȃ��A���[�����Ƃ߂�
		sys.FieldBattleFlag=0;			//�퓬��Flag�����Ƃ�
		sys.pFirstProc=PushFirstProc;
//		MainProcChange(sys.pReturnProc);
		MainProcChange(FightEndWindowInit);
	}
}

static void ClientFightEndWait(void)
{
	if(FadeData.fade_sw == 0){
		if(FightType&FIGHT_TYPE_TUUSHIN){
			SetSioEndDataStart();
			ClientSequence[ClientNo]=ClientFightEndWaitTuushin;
			return;
		}
		m4aSongNumStop(SE_HINSI);		//HP���Ȃ��A���[�����Ƃ߂�
		sys.FieldBattleFlag=0;			//�퓬��Flag�����Ƃ�
		sys.pFirstProc=PushFirstProc;
		MainProcChange(sys.pReturnProc);
	}
}

//==============================================================
//                       �T�u���[�`��
//==============================================================

void ClientCLMBitReset(void)
{
	u8	id;

	ClientSequence[ClientNo] = ClientCLMCommandWait;
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
void ClientCLMStatusEffWait(void)
{
	if(CliSys[ClientNo].status_eff == 0)
		ClientCLMBitReset();
}

//----------------------------------------------------------
//	�퓬�̪�ďI���҂�
//----------------------------------------------------------
void ClientCLMFightEffWait(void)
{
	if(CliSys[ClientNo].fight_eff == 0)
		ClientCLMBitReset();
}



//==============================================================
//	�N���C�A���g���ߊ֐�
//==============================================================

//----------------------------------------------------------
//	�w�肵���߹���ް����擾����
//----------------------------------------------------------

void	ClientCLMBSSPokeparaGet(void)
{
	u8 temp[256];
	int	length=0;
	int	i;
	u8	flag;

	if(ServerBuffer[ClientNo][2] == 0){
		length+=ClientCLMBSSPokeparaGetAct(SelMonsNo[ClientNo],&temp[length]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				length+=ClientCLMBSSPokeparaGetAct(i,&temp[length]);
			}
			flag=flag>>1;
		}
	}
	BSS_DATA_TRANS_SET(BSS_CLIENT,length,(u8 *)&temp);
	ClientCLMBitReset();
}

int	ClientCLMBSSPokeparaGetAct(u8 SelectNo,u8 *temp)
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
void ClientCLMBSSPokeparaGet2(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�w�肵���߹���ް���ύX����
//----------------------------------------------------------

void ClientCLMBSSPokeparaPut(void)
{
	u8	i,flag;

	if(ServerBuffer[ClientNo][2] == 0){
		ClientCLMBSSPokeparaPutAct(SelMonsNo[ClientNo]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				ClientCLMBSSPokeparaPutAct(i);
			}
			flag=flag>>1;
		}
	}
	ClientCLMBitReset();
}

void ClientCLMBSSPokeparaPutAct(u8 SelectNo)
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
			PokeParaMine[SelectNo].PPP.pp_count1 =
			ServerBuffer[ClientNo][BSS_DATASTART];
			break;
			PokeParaMine[SelectNo].PPP.pp_count2 =
			ServerBuffer[ClientNo][BSS_DATASTART];
			break;
			PokeParaMine[SelectNo].PPP.pp_count3 =
			ServerBuffer[ClientNo][BSS_DATASTART];
			break;
			PokeParaMine[SelectNo].PPP.pp_count4 =
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
void ClientCLMBSSPokeparaPut2(void)
{
	u8 *pPP;
	u8 i;
	
	pPP = ((u8 *)(&PokeParaMine[SelMonsNo[ClientNo]])) + ServerBuffer[ClientNo][1];
	for(i = 0; i < ServerBuffer[ClientNo][2]; i++)
	{
		pPP[i] = ServerBuffer[ClientNo][3+i];
	}

	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------

extern const u8 PokeMinePosX[2][2];
extern const u8 PokeMinePosY[2][2];

void ClientCLMBSSPokeEncount(void)
{

	PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
	//PokeActHeaderMake(PokeParaMine[SelMonsNo[ClientNo]].PPP.monsno,ClientNo);
	PokeActHeaderMake(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],
								  ID_monsno),
								  ClientTypeGet(ClientNo));	//�ύX by matsuda 2001.09.26(��)
#if 0
	PokemonStruct[ClientNo]=AddActor(&PokeActHeader,
		PokeMinePosX[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1],
//		PokeMinePosY[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1]+4*(8-pm2bpara[PokeParaMine[SelMonsNo[ClientNo]].PPP.monsno].size),
		PokeMinePosY[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1]+pm2bpara[PokeParaMine[SelMonsNo[ClientNo]].PPP.monsno].patcnt,
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
	ActWork[PokemonStruct[ClientNo]].oamData.Pltt=ClientNo;
	ActAnmChg(&ActWork[PokemonStruct[ClientNo]],BattlePokeAnmNo[ClientNo]);

	ClientSequence[ClientNo]=ClientCLMPokeEncountWait;
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ްق����яo��)
//----------------------------------------------------------
void ClientCLMBSSPokeAppear(void)
{
	// add 2002.07.04(��)
	ClientPokeSysFlagClear(ClientNo, ServerBuffer[ClientNo][2]);
	SelMonsNo[ClientNo] = ServerBuffer[ClientNo][1];
	PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);

	CLMAppearCommon(ClientNo, ServerBuffer[ClientNo][2]);

	ClientSequence[ClientNo] = ClientCLMPokeAppearWait;

//	ClientCLMBitReset();
}

//-- PokeAppear�ASendOut ���ʏ��� --//
static void CLMAppearCommon(u8 client_no, u8 baton_flg)
{
	u16 monsno;
	
	ClientPokeSysFlagClear(client_no, baton_flg);

	SelMonsNo[client_no]=ServerBuffer[client_no][1];
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
//							4*(8-pm2bpara[PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_monsno)].size),
							pm2bpara[PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_monsno)].patcnt,
							C_POKE1PRI);	//�߹��
#else
	PokemonStruct[client_no]=
		AddActor(&PokeActHeader,
				 GetWazaEffPos(client_no,PM_X),
				 GetPokemonYPos(client_no),
				 GetPokeSoftPri(client_no));
#endif
	ActWork[ClientStruct[client_no]].work[1] = PokemonStruct[client_no];
	ActWork[PokemonStruct[client_no]].work[0] = client_no;
	ActWork[PokemonStruct[client_no]].work[2] = monsno;
	ActWork[PokemonStruct[client_no]].oamData.Pltt = client_no;
	ActAnmChg(&ActWork[PokemonStruct[client_no]],BattlePokeAnmNo[client_no]);
	
#if 0	//changed by matsuda 2002.01.11(��)
	ActWork[ClientStruct[client_no]].work[0] = 
		MBSmokeSet(PokeEnemyPosX[FightCheck()][ClientTypeGet(client_no)>>1], PokeEnemyPosY[FightCheck()][ClientTypeGet(client_no)>>1]+20, 1);
	CAC_PokeAppearDifferentInit(&ActWork[PokemonStruct[client_no]], 1);
#else
	ActWork[PokemonStruct[client_no]].banish = 1;
	ActWork[PokemonStruct[client_no]].move = DummyActMove;
	ActWork[ClientStruct[client_no]].work[0] = MonsterBallSet(0,0xff);
#endif
}

//----------------------------------------------------------
//	�߹�݂�߂��̪�Ă��N������
//----------------------------------------------------------
void ClientCLMBSSPokeReturn(void)
{
	if(ServerBuffer[ClientNo][1] == 0)
	{
		CliSys[ClientNo].WazaEffSeqNo = 0;
		ClientSequence[ClientNo] = ClientCLMPokeReturnMigawariChk;
//		SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_MINE);
//		ClientSequence[ClientNo] = ClientCLMPokeReturnWait;
	}
	else
	{
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientCLMBitReset();
	}
}

enum{
	CLM_RET_MIGAWARICHK,
	CLM_RET_POKERETURN,
};
static void ClientCLMPokeReturnMigawariChk(void)
{
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case CLM_RET_MIGAWARICHK:
			if(Climem[ClientNo].migawari == 1)	//�u�݂����v�`�F�b�N
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			CliSys[ClientNo].WazaEffSeqNo = CLM_RET_POKERETURN;
			break;
		case CLM_RET_POKERETURN:					//�Z�G�t�F�N�g�Z�b�g
			if(CliSys[ClientNo].etc_eff)
				break;
			CliSys[ClientNo].WazaEffSeqNo = 0;
			SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_MINE);
			ClientSequence[ClientNo] = ClientCLMPokeReturnWait;
			break;
	}
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------
#define TRAINER_IN_DX	(200)		//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_IN_SP	(-4)	//�݂��Ă���Ƃ��̽�߰��

void ClientCLMBSSTrainerEncount(void)
{
	int	no;
	s16	ofsx;
//	if(ServerStatusFlag&SERVER_STATUS_FLAG_WAIT)
//		ServerStatusFlag&=SERVER_STATUS_FLAG_WAIT_OFF;
//	else{
//	TrainerFObjCellPalSet(UserWork[TRAINER_DATA], ClientNo);
	if(FightType&FIGHT_TYPE_4_2vs2){
		if(ClientTypeGet(ClientNo)&2)
			ofsx=16;
		else
			ofsx=-16;
		no=SioTrainerInfo[GetTeamPosID(ClientNo)].sex;
	}
	else{
		ofsx=0;
		no=SioTrainerInfo[GetSioConnectId()^1].sex;
	}

	TrainerBObjCellPalSet(no, ClientNo);

//	PokeActHeaderMake(no+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(no,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							C_TRN_X+ofsx,
							C_TRN_Y+4*(8-trbpara[no].size),
							//C_TRN_PRI);	//�ڰŰ
							GetPokeSoftPri(ClientNo));

	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = ENCOUNT_DX;
	ActWork[PokemonStruct[ClientNo]].work[0] = -ENCOUNT_SPEED;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;

	ClientSequence[ClientNo] = ClientCLMTrainerEncountWait;
//	}
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ΐ�I����)
//----------------------------------------------------------
void ClientCLMBSSTrainerAppear(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�ڰŰ��߂��̪�Ă��N������
//----------------------------------------------------------

#define	TRAINER_OUT_SP2	4

void ClientCLMBSSTrainerReturn(void)
{
#if 0	//changed by matsuda 2002.01.15(��)
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_OUT_SP2;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideOut;
#else
	DxDyAddClear(&ActWork[PokemonStruct[ClientNo]]);
	ActWork[PokemonStruct[ClientNo]].work[0] = ENEMYTRAINER_RETURN_SYNCNUM;	//VsyncNum
	ActWork[PokemonStruct[ClientNo]].work[2] = -40;
	ActWork[PokemonStruct[ClientNo]].work[4] = ActWork[PokemonStruct[ClientNo]].y;
	ActWork[PokemonStruct[ClientNo]].move = W_StraightSyncMove;
	SetWorkSeq(&ActWork[PokemonStruct[ClientNo]], DummyActMove);
#endif
	ClientSequence[ClientNo] = ClientCLMTrainerReturnWait;
}

//----------------------------------------------------------
//	�߹�݂��C�₷��̪�Ă��N������
//----------------------------------------------------------
void ClientCLMBSSPokeKizetu(void)
{
#if 0	//2002.09.03(��)
	PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
	SePlayPan(SE_POKE_DEAD, WAZA_SE_L);
	ActWork[PokemonStruct[ClientNo]].work[1] = 0;
	ActWork[PokemonStruct[ClientNo]].work[2] = C_POKE_KIZETU_SP;
	ActWork[PokemonStruct[ClientNo]].move = SeqMinePokeMove;
	
	ClientSequence[ClientNo]=ClientCLMPokeKizetsuWait;
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

			PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
			SePlayPan(SE_POKE_DEAD, WAZA_SE_L);
			ActWork[PokemonStruct[ClientNo]].work[1] = 0;
			ActWork[PokemonStruct[ClientNo]].work[2] = C_POKE_KIZETU_SP;
			ActWork[PokemonStruct[ClientNo]].move = SeqMinePokeMove;
			
			ClientSequence[ClientNo]=ClientCLMPokeKizetsuWait;
			break;
	}
#endif
}

//----------------------------------------------------------
//	�߹�݂��S�ł����Ƃ��̴̪�Ă��N������
//----------------------------------------------------------
void ClientCLMBSSZenmetsu(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl�����̴̪�Ă��o��
//----------------------------------------------------------
void ClientCLMBSSGetSuccess(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl���s�̴̪�Ă��o��
//----------------------------------------------------------
void ClientCLMBSSGetFail(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�߹�����ٱ��߂̴̪�Ă��o��
//----------------------------------------------------------
void ClientCLMBSSPokeLevelUp(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�w�肵���Z���ް�̴̪�Ă��o��
//----------------------------------------------------------
#if 0	// 2002.07.22(��)
void ClientCLMBSSWazaEffect(void)
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
		ClientCLMBitReset();
		return;
	}

	if(FightCheck() && (AttackClient^2) == DefenceClient && WazaDamage > 0)
		wazano = WAZANO_MAX+1;	//�����ւ̍U���Ȃ�΁A�ȈՓI�ȴ̪�Ă��o��
	
	WazaEffectSet(wazano);
	ClientSequence[ClientNo] = ClientCLMWazaEffectWait;
}
#else
enum{
	CLM_MIGAWARI_CHK,
	CLM_EFF_PUTCHK,
	CLM_EFF_WAIT,
	CLM_EFF_END,
};
void ClientCLMBSSWazaEffect(void)
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
		ClientCLMBitReset();
		return;
	}
	
	CliSys[ClientNo].WazaEffSeqNo = CLM_MIGAWARI_CHK;
	ClientSequence[ClientNo] = ClientCLMWazaEffectSet;
}

static void ClientCLMWazaEffectSet(void)
{
	u16 wazano;
	u8 renzoku_cnt;
	
	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	renzoku_cnt = ServerBuffer[ClientNo][11];	//0:ɰ�� 1:�A���Z�I�� 2:�A���Z��
	
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case CLM_MIGAWARI_CHK:	//�u�݂����v�`�F�b�N
			if(Climem[ClientNo].migawari == 1 && Climem[ClientNo].migawari_out == 0){
				Climem[ClientNo].migawari_out = 1;
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			}
			CliSys[ClientNo].WazaEffSeqNo = CLM_EFF_PUTCHK;
			break;
		case CLM_EFF_PUTCHK:					//�Z�G�t�F�N�g�Z�b�g
			if(CliSys[ClientNo].etc_eff)
				break;
			
//			if(FightCheck() && (AttackClient^2) == DefenceClient && WazaDamage > 0)
//				wazano = WAZANO_MAX+1;	//�����ւ̍U���Ȃ�΁A�ȈՓI�ȴ̪�Ă��o��

			PokeAffineON_OFF(0);
			WazaEffectSet(wazano);
			CliSys[ClientNo].WazaEffSeqNo = CLM_EFF_WAIT;
			break;
		case CLM_EFF_WAIT:					//�Z�G�t�F�N�g�I���҂�
			WazaEffectSequence();
			if(WazaEffectFlag==0){
				PokeAffineON_OFF(1);
				if(Climem[ClientNo].migawari && renzoku_cnt <= 1){	//�g����`�F�b�N
					SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
					Climem[ClientNo].migawari_out = 0;
				}
				CliSys[ClientNo].WazaEffSeqNo = CLM_EFF_END;
			}
			break;
		case CLM_EFF_END:
			if(CliSys[ClientNo].etc_eff)
				break;

			//�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
			PokeBanishMemory();
			wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
			MigawariFlgCheckSet(ClientNo, wazano);
			CliSys[ClientNo].WazaEffSeqNo = 0;
			ClientCLMBitReset();
			break;
	}
}
#endif

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------
#if 0
void ClientCLMBSSMessage(void)
{
#if 1
	ScrX0=0;
	ScrY0=0;
//	MsgPutSet(&FightMsg,&ServerBuffer[ClientNo][2],0x10,1,15);
//	TalkMsgSet(&FightMsg,&ServerBuffer[ClientNo][2],CGX_START+0x10,2,15);
	FightMsgCreate((u8 *)&ServerBuffer[ClientNo][2]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientCLMMessageWait;
#else
//���Ȃ��΂�Ƃ��A���������Ƃ��̃A���[�g���b�Z�[�W���o���Ȃ��悤�ɂ���
	ClientCLMBitReset();
#endif
}
#else
void ClientCLMBSSMessage(void)
{
	u16	*MsgID;

	ScrX0=0;
	ScrY0=0;

	MsgID=(u16 *)&ServerBuffer[ClientNo][2];
	FightMsgID(MsgID[0]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientCLMMessageWait;
}
#endif

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

void ClientCLMBSSAlertMessage(void)
{
//	if(MineEnemyCheck(ClientNo)==SIDE_MINE)
//		ClientCLMBSSMessage();
//	else
		ClientCLMBitReset();
}

//----------------------------------------------------------
//	����ޑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientCLMBSSCommandSelect(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽClientNo�̃V�[�P���X�i���o�[�Ɠ������Ƃ�
//----------------------------------------------------------
void ClientCLMBSSCommandSelectCopy(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�Z�I����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientCLMBSSWazaSelect(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	���ёI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientCLMBSSItemSelect(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�߹�ݑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientCLMBSSPokeSelect(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	������̪�Ă��N������
//----------------------------------------------------------
void ClientCLMBSSEscape(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	HP�ް�ނ𑝌�����
//----------------------------------------------------------
void	ClientCLMBSSHpGauge(void)
{
	s16 beHP;	//��������l

	BattleGaugeTransWram(0);
	beHP = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3] << 8);
	if(beHP!=JIBAKU_HP)
		GaugeDataSet(ClientNo, ClientGaugeSt[ClientNo],
		PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_hpmax),
		PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_hp), beHP);
	else
		GaugeDataSet(ClientNo, ClientGaugeSt[ClientNo],
		PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_hpmax),
		0,beHP);
	ClientSequence[ClientNo] = ClientCLMHpGaugeWait;
}

//----------------------------------------------------------
//	EXP�ް�ނ𑝌�����
//----------------------------------------------------------
void ClientCLMBSSExpGauge(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ�̪�Ă��N������
//----------------------------------------------------------
void ClientCLMBSSStatusSet(void)
{
	u32 condition;
	u8 condition_flg;

	if(ClientSEWait(ClientNo))
		return;
	
	GaugeTransChrSet(ClientGaugeSt[ClientNo], &PokeParaMine[SelMonsNo[ClientNo]], G_STATUS);
	condition_flg = ServerBuffer[ClientNo][1];
	condition = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8) | 
				(ServerBuffer[ClientNo][4]<<16) | (ServerBuffer[ClientNo][5]<<24);
	
//	ClientStatusEffSet(condition_flg, condition);	2002.09.07(�y)
	CliSys[ClientNo].status_eff = 0;

	ClientSequence[ClientNo] = ClientCLMStatusEffWait;
}

void ClientCLMBSSStatusEffect(void)
{
	u32 condition;
	u8 condition_flg;

	if(ClientSEWait(ClientNo))
		return;
	
	condition_flg = ServerBuffer[ClientNo][1];
	condition = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8) | 
				(ServerBuffer[ClientNo][4]<<16) | (ServerBuffer[ClientNo][5]<<24);
	ClientStatusEffSet(condition_flg, condition);
	ClientSequence[ClientNo] = ClientCLMStatusEffWait;
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ��ر����
//----------------------------------------------------------
void ClientCLMBSSStatusClear(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	���炩���ߗp�ӂ��ꂽ�ޯ̧���ް����i�[����
//----------------------------------------------------------
void ClientCLMBSSDataTrans(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�w����ڽ(RAM,VRAM,CGRAM)���ް���]������
//----------------------------------------------------------
void ClientCLMBSSDataRamTrans(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	���ް���]������
//----------------------------------------------------------
void ClientCLMBSSBgmTrans(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�����ް���]������
//----------------------------------------------------------
void ClientCLMBSSVoiceTrans(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�ƭ��őI����������ID��Ԃ�
//----------------------------------------------------------
void ClientCLMBSSSelectReturn(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�߹�ݓ���ւ����s�������Ƃ�m�点��
//----------------------------------------------------------
void ClientCLMBSSReshuffle(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	���юg�p��m�点��
//----------------------------------------------------------
void ClientCLMBSSUseItem(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�ްَg�p��m�点��
//----------------------------------------------------------
void ClientCLMBSSUseBall(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃N���A
//----------------------------------------------------------
void	ClientCLMBSSLoopCountClear(void)
{
	WEW.loopcnt = 0;
	ClientCLMBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃Z�b�g
//----------------------------------------------------------
void	ClientCLMBSSLoopCountSet(void)
{
	WEW.loopcnt = ServerBuffer[ClientNo][1];
	ClientCLMBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃N���A
//----------------------------------------------------------
void	ClientCLMBSSSwitchFlagClear(void)
{
	WEW.flag = 0;
	ClientCLMBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃Z�b�g
//----------------------------------------------------------
void	ClientCLMBSSSwitchFlagSet(void)
{
	WEW.flag ^= 1;
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	��Ұ�ނ��󂯂��Ƃ����߹�ݓ_��
//----------------------------------------------------------
void ClientCLMBSSPokeDamageBlink(void)
{
	//�߹�݂���Ư����ԂȂ��Blink���Ȃ�
	if(ActWork[PokemonStruct[ClientNo]].banish == 1){
		ClientCLMBitReset();
		return;
	}

	ClientEffectWait = 1;	//Blink�̪�ĊJ�n�t���O�Z�b�g
	ActWork[PokemonStruct[ClientNo]].work[1] = 0;

	//-- �ް�ނ̃J���[�ύX & �h�炵�G�t�F�N�g�Z�b�g --//
	GaugeSwayEffect(ClientNo);

	ClientSequence[ClientNo] = ClientCLMPokeDamageBlinkWait;
}

//----------------------------------------------------------
//	�Q�[�W������
//----------------------------------------------------------
void ClientCLMBSSGaugeOff(void)
{
//	GaugeOFF(ClientGaugeSt[ClientNo]);
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	SE��炷
//----------------------------------------------------------
void ClientCLMBSSSEPlay(void)
{
	s8 pan;
	
//	SePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	if(MineEnemyCheck(ClientNo) == SIDE_MINE)
		pan = WAZA_SE_L;
	else
		pan = WAZA_SE_R;
	SePlayPan(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8), pan);
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	ME��炷
//----------------------------------------------------------
void ClientCLMBSSMEPlay(void)
{
	JinglePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�Ȃ�������炷
//----------------------------------------------------------
void ClientCLMBSSVoicePlay(void)
{
//	VoicePlay(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno),25);
	//�C�⎞�̐��Ŗ��悤�ɕύX 2002.08.27(��) by matsuda
	VoicePlay3(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno),-25,5);
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�G���J�E���g�G�t�F�N�g���Z�b�g
//----------------------------------------------------------
void ClientCLMBSSEncountEffect(void)
{
	EncountEffectSet(ServerBuffer[ClientNo][1]);
	//OBJ���ړ��̃X�g�b�p�[
	EncountEffectFlag|=ENCOUNT_EFFECT_FLAG_OBJSTOP;	
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�ŏ��̃|�P�����o��(�ڰŰ�������ށA�߹�ݏo���Aү���ޏo���B�܂ł̈�A�̓���
//----------------------------------------------------------
void ClientCLMBSSPokeSendOut(void)
{
	u8 palno, id;
	
	DxDyAddClear(&ActWork[PokemonStruct[ClientNo]]);
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_RETURN_SYNCNUM;	//VsyncNum
	ActWork[PokemonStruct[ClientNo]].work[2] = -40;
	ActWork[PokemonStruct[ClientNo]].work[4] = ActWork[PokemonStruct[ClientNo]].y;
	ActWork[PokemonStruct[ClientNo]].move = W_StraightSyncMove;
	ActWork[PokemonStruct[ClientNo]].work[5] = ClientNo;
	SetWorkSeq(&ActWork[PokemonStruct[ClientNo]], TrainerDelEnd);

	ActAnmChg(&ActWork[PokemonStruct[ClientNo]],1);

	//SendOut�̎������߹�݂���گĂƂ��Ԃ�Ȃ��悤��PalSet���g�p����
	palno = ObjPalNumSet(MINE_TRAINER_PALID2);
	DecordPaletteWork(
		(void*)TRBObjDataPal[SioTrainerInfo[GetTeamPosID(ClientNo)].sex].pal_data,
		PA_OBJ0 + 0x10 * palno, 0x20);
	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = palno;

	//-- �߹�� --//
	id = AddTask(SendOutTask, 5);
	TaskTable[id].work[0] = ClientNo;

	//-- �莝���ް�ނ����� --//
	if(CliSys[ClientNo].temoti_gauge_flg)
		TaskTable[TemotiGauge[ClientNo]].TaskAdrs = TemotiBallOff;

	BtlWork->sendout_eff = 1;
	ClientSequence[ClientNo] = ClientCLMDummy;
}

static void SendOutTask(u8 id)
{
	u8 clientno_temp;
	
	if(TaskTable[id].work[1] < 24){
		TaskTable[id].work[1]++;
		return;
	}
	
	clientno_temp = ClientNo;
	ClientNo = TaskTable[id].work[0];
	if(FightCheck() == 0)
	{	//1vs1
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		CLMAppearCommon(ClientNo,0);	//ClientCLMBSSPokeAppear();
	}
	else if(FightType&FIGHT_TYPE_4_2vs2)
	{	//2vs2 4�l�ʐM
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		CLMAppearCommon(ClientNo, 0);
	}
	else
	{	//2vs2
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		CLMAppearCommon(ClientNo,0);	//ClientCLMBSSPokeAppear();
		
		ClientNo ^= 2;
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		
		// add 2002.07.04(��)
		PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);

		CLMAppearCommon(ClientNo,0);	//ClientCLMBSSPokeAppear();
		/*
		if(FightType & FIGHT_TYPE_TUUSHIN)
			ClientSequence[ClientNo] = ClientCommandWait;
		else
			ClientSequence[ClientNo] = ClientCLMCommandWait;
		*/
		ClientNo ^= 2;	//�ύX����ClientNo�����ɖ߂�
	}
	ClientSequence[ClientNo] = ClientCLMPokeSendOutWait;
	ClientNo = clientno_temp;
	DelTask(id);
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W��\������
//----------------------------------------------------------

void	ClientCLMBSSBallGaugeSet(void)
{
	BallGaugePara *bgp;
	
//�t���O���[�N��true�̎��́A�ǂ����T�C�h�Ȃ̂��`�F�b�N���āA�������̂Ƃ��͕\�������Ȃ��悤�ɂ���
	if(ServerBuffer[ClientNo][1]){
		if(MineEnemyCheck(ClientNo)==SIDE_MINE){
			ClientCLMBitReset();
			return;
		}
	}
	CliSys[ClientNo].temoti_gauge_flg = 1;
	bgp = (BallGaugePara*)&ServerBuffer[ClientNo][4];
	TemotiGauge[ClientNo] = TemotiBallPut(ClientNo, bgp, ServerBuffer[ClientNo][1], ServerBuffer[ClientNo][2]);
	CliSys[ClientNo].wait_work = 0;
	if(ServerBuffer[ClientNo][2]){	//�׸ނ������Ă����糪�ĂȂ�
		CliSys[ClientNo].wait_work = TEMOTI_BALL_WAIT+1;
	}
	ClientSequence[ClientNo] = ClientCLMBallGaugeWait;
//	ClientCLMBitReset();
}

static void ClientCLMBallGaugeWait(void)
{
	if(CliSys[ClientNo].wait_work++ > TEMOTI_BALL_WAIT){
		CliSys[ClientNo].wait_work = 0;
		ClientCLMBitReset();
	}
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W���\���ɂ���
//----------------------------------------------------------

void	ClientCLMBSSBallGaugeOff(void)
{
	//-- �莝���ް�ނ����� --//
	if(CliSys[ClientNo].temoti_gauge_flg)
		TaskTable[TemotiGauge[ClientNo]].TaskAdrs = TemotiBallOff;
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�Q�[�W�̂����~�߂�
//----------------------------------------------------------

void ClientCLMBSSGaugeYureStop(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�|�P������VANISH��ON/OFF����
//----------------------------------------------------------

void	ClientCLMBSSVanishOnOff(void)
{
	if(ClientPokeExistCheck(ClientNo)){
		ActWork[PokemonStruct[ClientNo]].banish = ServerBuffer[ClientNo][1];
		PokeClientBanishMemory(ClientNo);
	}
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�퓬�G�t�F�N�g���o��
//----------------------------------------------------------

void	ClientCLMBSSFightEffect(void)
{
	u8 eff_no;
	u16 monsno;
	
	if(ClientSEWait(ClientNo))
		return;
	
	eff_no = ServerBuffer[ClientNo][1];
	monsno = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8);
	
	if(SetFightEffect(ClientNo, ClientNo, ClientNo, eff_no, monsno))
		ClientCLMBitReset();
	else
		ClientSequence[ClientNo] = ClientCLMFightEffWait;
}

//----------------------------------------------------------
//	�ʐM�ҋ@��ʂ��o��
//----------------------------------------------------------

void	ClientCLMBSSTuushinWait(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�L�����Ă���J�[�\���ʒu�̃N���A
//----------------------------------------------------------

void	ClientCLMBSSCsrPosClear(void)
{
	ClientCLMBitReset();
}

//----------------------------------------------------------
//	�ΐ�I����m�点��
//----------------------------------------------------------
void ClientCLMBSSFightEnd(void)
{
#if 0
	WinLoseFlag=ServerBuffer[ClientNo][1];
	BGM_FadeOutSet(5);
	LightFadeReq(N_to_B);
	ClientCLMBitReset();
	if((FightType&FIGHT_TYPE_TUUSHIN)&&((FightType&FIGHT_TYPE_OYA)==0))
		ClientSequence[ClientNo] = ClientFightEndWait;
#else
	WinLoseFlag=ServerBuffer[ClientNo][1];
	BGM_FadeOutSet(5);
	LightFadeReq(N_to_B);
	ClientCLMBitReset();
	ClientSequence[ClientNo] = ClientFightEndWait;
#endif
}

//----------------------------------------------------------
//	��а
//----------------------------------------------------------
void ClientCLMBSSEndID(void)
{
//	ClientCLMBitReset();
}

