
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

void	ClientCLBitReset(void);	//�͈͊O�̒l�̎��̏���
void	ClientCLDummy(void);
void	ClientCLInit(void);
void	ClientCLCommandWait(void);
void	ClientCLPokeEncountWait(void);

void	ClientCLCommandSelect(void);
void	ClientCLWazaSelect(void);
void	ClientCLHpGaugeWait(void);
void	ClientCLPokeKizetsuWait(void);
void	ClientCLMessageWait(void);

void	ClientCLBSSPokeparaGet(void);
void	ClientCLBSSPokeparaGet2(void);
void	ClientCLBSSPokeparaPut(void);
void	ClientCLBSSPokeparaPut2(void);
void	ClientCLBSSPokeEncount(void);
void	ClientCLBSSPokeAppear(void);
void	ClientCLBSSPokeReturn(void);
void	ClientCLBSSTrainerEncount(void);
void	ClientCLBSSTrainerAppear(void);
void	ClientCLBSSTrainerReturn(void);
void	ClientCLBSSPokeKizetu(void);
void	ClientCLBSSZenmetsu(void);
void	ClientCLBSSGetSuccess(void);
void	ClientCLBSSGetFail(void);
void	ClientCLBSSPokeLevelUp(void);
void	ClientCLBSSWazaEffect(void);
void	ClientCLBSSMessage(void);
void	ClientCLBSSAlertMessage(void);
void	ClientCLBSSCommandSelect(void);
void	ClientCLBSSCommandSelectCopy(void);
void	ClientCLBSSWazaSelect(void);
void	ClientCLBSSItemSelect(void);
void	ClientCLBSSPokeSelect(void);
void	ClientCLBSSEscape(void);
void	ClientCLBSSHpGauge(void);
void	ClientCLBSSExpGauge(void);
void	ClientCLBSSStatusSet(void);
void	ClientCLBSSStatusEffect(void);
void	ClientCLBSSStatusClear(void);
void	ClientCLBSSDataTrans(void);
void	ClientCLBSSDataRamTrans(void);
void	ClientCLBSSBgmTrans(void);
void	ClientCLBSSVoiceTrans(void);
void	ClientCLBSSSelectReturn(void);
void	ClientCLBSSReshuffle(void);
void	ClientCLBSSUseItem(void);
void	ClientCLBSSUseBall(void);
void	ClientCLBSSLoopCountClear(void);
void	ClientCLBSSLoopCountSet(void);
void	ClientCLBSSSwitchFlagClear(void);
void	ClientCLBSSSwitchFlagSet(void);
void	ClientCLBSSPokeDamageBlink(void);
void	ClientCLBSSGaugeOff(void);
void	ClientCLBSSSEPlay(void);
void	ClientCLBSSMEPlay(void);
void	ClientCLBSSVoicePlay(void);
void	ClientCLBSSEncountEffect(void);
void	ClientCLBSSPokeSendOut(void);
void	ClientCLBSSBallGaugeSet(void);
void	ClientCLBSSBallGaugeOff(void);
void	ClientCLBSSGaugeYureStop(void);
void	ClientCLBSSVanishOnOff(void);
void	ClientCLBSSFightEffect(void);
void	ClientCLBSSTuushinWait(void);
void	ClientCLBSSCsrPosClear(void);
void	ClientCLBSSFightEnd(void);
void	ClientCLBSSEndID(void);

int		ClientCLBSSPokeparaGetAct(u8,u8 *);
void	ClientCLBSSPokeparaPutAct(u8);
static void CLPokeAppearCommon(u8 client_no, u8);
static void CLTrainerDelEnd(actWork *xreg);
static void CLSendOutTask(u8 id);
static void ClientCLWazaEffectSet(void);
static void ClientCLAppearMigawariEffWait(void);
static void ClientCLBallGaugeWait(void);
static void ClientCLPokeReturnMigawariChk(void);

extern	actHeader PokeFTbl[];
extern	TALK_WIN	FightMsg;
//extern	u8	FightMsgBuf[64];
extern SIOTRAINER_INFO SioTrainerInfo[4];


//----------------------------------------------------------
//	�o�g���T�[�o�[���ߌQ�e�[�u��
//----------------------------------------------------------
const pFunc ClientCLBSSTbl[] =
{
	ClientCLBSSPokeparaGet,		// BSS_POKEPARA_GET=0,	
	ClientCLBSSPokeparaGet2,	   	// BSS_POKEPARA_GET2,	
	ClientCLBSSPokeparaPut,		// BSS_POKEPARA_PUT,	
	ClientCLBSSPokeparaPut2,		// BSS_POKEPARA_PUT2,	
	ClientCLBSSPokeEncount,		// BSS_POKEPARA_ENCOUNT,	add
	ClientCLBSSPokeAppear,		// BSS_POKE_APPEAR,	
	ClientCLBSSPokeReturn,		// BSS_POKE_RETURN,	
	ClientCLBSSTrainerEncount,	// BSS_TRAINER_ENCOUNT,	add
	ClientCLBSSTrainerAppear,		// BSS_TRAINER_APPEAR,	
	ClientCLBSSTrainerReturn,		// BSS_TRAINER_RETURN,	
	ClientCLBSSPokeKizetu,		// BSS_POKE_KIZETSU,	
	ClientCLBSSZenmetsu,			// BSS_POKE_ZENMETSU,	
	ClientCLBSSGetSuccess,		// BSS_GET_SUCCESS,	
	ClientCLBSSGetFail,			// BSS_GET_FAIL,		
	ClientCLBSSPokeLevelUp,		// BSS_POKE_LEVELUP,	
	ClientCLBSSWazaEffect,		// BSS_WAZA_EFFECT,	
	ClientCLBSSMessage,			// BSS_MESSAGE,		
	ClientCLBSSAlertMessage,	// BSS_ALERT_MESSAGE,		
								
	ClientCLBSSCommandSelect,		// BSS_COMMAND_SELECT, 

	ClientCLBSSCommandSelectCopy,		// BSS_COMMAND_SELECT_COPY, 

	ClientCLBSSWazaSelect,		// BSS_WAZA_SELECT,	
													
	ClientCLBSSItemSelect,		// BSS_ITEM_SELECT,	
													
	ClientCLBSSPokeSelect,		// BSS_POKE_SELECT,	
													
	ClientCLBSSEscape,			// BSS_ESCAPE,			
	ClientCLBSSHpGauge,			// BSS_HP_GAUGE,		
	ClientCLBSSExpGauge,			// BSS_EXP_GAUGE,		
	ClientCLBSSStatusSet,			// BSS_STATUS_SET,		
	ClientCLBSSStatusEffect,			// BSS_STATUS_EFFECT,		
	ClientCLBSSStatusClear,		// BSS_STATUS_CLEAR,	
	ClientCLBSSDataTrans,			// BSS_DATA_TRANS,		
	ClientCLBSSDataRamTrans,		// BSS_DATA_RAM_TRANS,	
													
	ClientCLBSSBgmTrans,			// BSS_BGM_TRANS,		
	ClientCLBSSVoiceTrans,		// BSS_VOICE_TRANS,	
	ClientCLBSSSelectReturn,		// BSS_SELECT_RETURN,	
	ClientCLBSSReshuffle,			// BSS_POKE_RESHUFFLE,	
	ClientCLBSSUseItem,			// BSS_USE_ITEM,		
	ClientCLBSSUseBall,			// BSS_USE_BALL,		
	ClientCLBSSLoopCountClear,	//	BSS_LOOPCOUNT_CLEAR
	ClientCLBSSLoopCountSet,	//	BSS_LOOPCOUNT_SET
	ClientCLBSSSwitchFlagClear,	//	BSS_SWITCHFLAG_CLEAR
	ClientCLBSSSwitchFlagSet,	//	BSS_SWITCHFLAG_SET
	ClientCLBSSPokeDamageBlink,	// BSS_POKEDAMAGE_BLINK,
	ClientCLBSSGaugeOff,		// BSS_GAUGE_OFF,
	ClientCLBSSSEPlay,			// BSS_SE_PLAY,
	ClientCLBSSMEPlay,			// BSS_SE_PLAY,
	ClientCLBSSVoicePlay,			// BSS_VOICE_PLAY,
	ClientCLBSSEncountEffect,	// BSS_ENCOUNT_EFFECT,
	ClientCLBSSPokeSendOut,		// BSS_POKE_SENDOUT,
	ClientCLBSSBallGaugeSet,	// BSS_BALL_GAUGE_SET,
	ClientCLBSSBallGaugeOff,	// BSS_BALL_GAUGE_OFF,
	ClientCLBSSGaugeYureStop,	// BSS_BALL_GAUGE_OFF,
	ClientCLBSSVanishOnOff,		// BSS_VANISH_OFF,
	ClientCLBSSFightEffect,		// BSS_FIGHT_EFFECT,
	ClientCLBSSTuushinWait,		// BSS_TUUSHIN_WAIT,
	ClientCLBSSCsrPosClear,		// BSS_CSRPOS_CLEAR,
	ClientCLBSSFightEnd,		// BSS_FIGHT_END,

	ClientCLBSSEndID,				// BSS_END_ID,		����Ȃ����ǈꉞ
};

//----------------------------------------------------------
//	�v���g�^�C�v�錾	�֐����ް�
//----------------------------------------------------------

#define	CAI_LVNUM_X			84
#define	CAI_LVNUM_Y			28

//==============================================================
//                       ���C�����[�`��
//==============================================================

void	ClientCLDummy(void)
{
}

void	ClientCLInit(void)
{
	ClientSequence[ClientNo]=ClientCLCommandWait;

#if 0	//�����]���p�Ɉ�ɂ܂Ƃ߂�
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

void	ClientCLCommandWait(void)
{
	if((ClientBit&no2bittbl[ClientNo]) != 0)
	{
		if(ServerBuffer[ClientNo][0] <= BSS_END_ID)// && ServerBuffer[ClientNo][0] > -1)
		{
			ClientCLBSSTbl[ServerBuffer[ClientNo][0]]();
		}
		else
		{
			ClientCLBitReset();	//�͈͊O�̒l�̎��̏���
		}
	}
}

//=========================================================================
//	�E�F�C�g���[�`��
//=========================================================================
//
void ClientCLTrainerEncountWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientCLBitReset();
	}
}

void ClientCLTrainerReturnWait(void)
{
#if 0
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
		
		ClientCLBitReset();
	}
}

void ClientCLSendOutWaitBitReset(void)
{
	if(CliSys[ClientNo].send_out_wait-- == 0){
		CliSys[ClientNo].send_out_wait = 0;
		ClientCLBitReset();
	}
}

void ClientCLSendOutGaugeWait(void)
{
#if 0	//2002.05.02(��)
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove
		&& ActWork[ClientGaugeSt[ClientNo^2]].move == DummyActMove)
	{
		ClientCLBitReset();
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
		if(ClientTypeGet(ClientNo) == BSS_CLIENT_ENEMY){
			if(CliSys[ClientNo].rare_eff_end && CliSys[ClientNo^2].rare_eff_end){
				CliSys[ClientNo].rare_eff = 0;
				CliSys[ClientNo].rare_eff_end = 0;
				CliSys[ClientNo^2].rare_eff = 0;
				CliSys[ClientNo^2].rare_eff_end = 0;
				CellDel(WAZACHR_ID_SHOOTINGSTAR);
				ObjPalRegNumDel(WAZACHR_ID_SHOOTINGSTAR);
			}
			else
				return;
		}
		
		if(FightType&FIGHT_TYPE_4_2vs2){
			if(ClientTypeGet(ClientNo) == BSS_CLIENT_ENEMY){
				m4aMPlayContinue(&m4a_mplay000);
			}
		}
		else
			m4aMPlayVolumeControl(&m4a_mplay000,0xffff,256);
//		ClientCLBitReset();
		CliSys[ClientNo].send_out_wait = 3;
		ClientSequence[ClientNo] = ClientCLSendOutWaitBitReset;
	}
#endif
}

void ClientCLPokeSendOutWait(void)
{
//	if(ActWork[ClientStruct[ClientNo]].move == DummyActMove)	//2�����ɓ����Ă�̂�1�����������Ă����Ζ��Ȃ�����

	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo].rare_eff == 0)
		RareEffectCheckSet(ClientNo, &PokeParaEnemy[SelMonsNo[ClientNo]]);
	if(CliSys[ClientNo^2].ball_eff_wait == 0 && CliSys[ClientNo^2].rare_eff == 0)
		RareEffectCheckSet(ClientNo^2, &PokeParaEnemy[SelMonsNo[ClientNo^2]]);

	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo^2].ball_eff_wait == 0)
	{
		if((FightType & FIGHT_TYPE_4_2vs2) && ClientTypeGet(ClientNo) == BSS_CLIENT_ENEMY2){
			if(CliSys[ClientNo].send_out_wait++ == 0)
				return;
			CliSys[ClientNo].send_out_wait = 0;
		}

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
		
		DelActor(&ActWork[ClientStruct[ClientNo]]);		//�̪�ĊĎ�����DEL
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
		
//		ClientCLBitReset();
		BtlWork->sendout_eff = 0;
		ClientSequence[ClientNo] = ClientCLSendOutGaugeWait;
	}
}

void	ClientCLPokeEncountWait(void)
{
#if 0
	if((ActWork[PokemonStruct[ClientNo]].anmend_sw==1)&&
	(ActWork[PokemonStruct[ClientNo]].dx==0)){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientCLCommandWait;
		ClientCLBitReset();
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
			ClientCLBitReset();
		}
	}
#endif
}

void	ClientCLHpGaugeWait(void)
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
		ClientCLBitReset();
	}
}


void	ClientCLPokeKizetsuWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].act_sw==0){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientCLCommandWait;
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientCLBitReset();
	}
}

void ClientCLPokeReturnWait(void)
{
	if(CliSys[ClientNo].etc_eff == 0)
	{
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		PokeShadowBanishSeqSet(ClientNo);

		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientCLBitReset();
	}
}

void	ClientCLMessageWait(void)
{
	if(FightMsg.status==MSG_END){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientCLCommandWait;
		ClientCLBitReset();
	}
}

#if 0	//del by matsuda 2002.07.22(��)
void	ClientCLWazaEffectWait(void)
{
	WazaEffectSequence();
	if(WazaEffectFlag==0){
		ClientCLBitReset();
	}
}
#endif

void ClientCLPokeDamageBlinkWait(void)
{
	u8 pokeact;
	
	pokeact = PokemonStruct[ClientNo];
	if(ActWork[pokeact].work[1] == DAMAGE_BLINK_WAIT)
	{
		ActWork[pokeact].work[1] = 0;
		ActWork[pokeact].banish = 0;
		ClientEffectWait = 0;	//Blink�̪�āA�t���O��ؾ��
		ClientCLBitReset();
	}
	else
	{
		if(ActWork[pokeact].work[1] % BLINK_INTERVAL == 0)
			ActWork[pokeact].banish ^= 1;
		ActWork[pokeact].work[1]++;
	}
}

void ClientCLGaugeAppearWait(void)
{
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove){
		//�g����`�F�b�N
		if(Climem[ClientNo].migawari)
			SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
		ClientSequence[ClientNo] = ClientCLAppearMigawariEffWait;
	}
}

static void ClientCLAppearMigawariEffWait(void)
{
	if(CliSys[ClientNo].etc_eff == 0){
		AddTask(VoiceCheckTask, 10);
		ClientCLBitReset();
	}
}

void ClientCLPokeAppearWaitSub(void)
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
//		ClientCLBitReset();
		ClientSequence[ClientNo] = ClientCLGaugeAppearWait;
	}
}
void ClientCLPokeAppearWait(void)
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

		ClientSequence[ClientNo] = ClientCLPokeAppearWaitSub;
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

//----------------------------------------------------------
//	�ð���ُ�̪�ďI���҂�
//----------------------------------------------------------
void ClientCLStatusEffWait(void)
{
	if(CliSys[ClientNo].status_eff == 0)
		ClientCLBitReset();
}

//----------------------------------------------------------
//	�퓬�̪�ďI���҂�
//----------------------------------------------------------
void ClientCLFightEffWait(void)
{
	if(CliSys[ClientNo].fight_eff == 0)
		ClientCLBitReset();
}


//==============================================================
//                       �T�u���[�`��
//==============================================================

void ClientCLBitReset(void)
{
	u8	id;

	ClientSequence[ClientNo] = ClientCLCommandWait;
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

//==============================================================
//	�N���C�A���g���ߊ֐�
//==============================================================

//----------------------------------------------------------
//	�w�肵���߹���ް����擾����
//----------------------------------------------------------

void	ClientCLBSSPokeparaGet(void)
{
	u8 temp[256];
	int	length=0;
	int	i;
	u8	flag;

	if(ServerBuffer[ClientNo][2] == 0){
		length+=ClientCLBSSPokeparaGetAct(SelMonsNo[ClientNo],&temp[length]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				length+=ClientCLBSSPokeparaGetAct(i,&temp[length]);
			}
			flag=flag>>1;
		}
	}
	BSS_DATA_TRANS_SET(BSS_CLIENT,length,(u8 *)&temp);
	ClientCLBitReset();
}

int	ClientCLBSSPokeparaGetAct(u8 SelectNo,u8 *temp)
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
void ClientCLBSSPokeparaGet2(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�w�肵���߹���ް���ύX����
//----------------------------------------------------------

void ClientCLBSSPokeparaPut(void)
{
	u8	i,flag;

	if(ServerBuffer[ClientNo][2] == 0){
		ClientCLBSSPokeparaPutAct(SelMonsNo[ClientNo]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				ClientCLBSSPokeparaPutAct(i);
			}
			flag=flag>>1;
		}
	}
	ClientCLBitReset();
}

void ClientCLBSSPokeparaPutAct(u8 SelectNo)
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
void ClientCLBSSPokeparaPut2(void)
{
	u8 *pPP;
	u8 i;
	
	pPP = ((u8 *)(&PokeParaEnemy[SelMonsNo[ClientNo]])) + ServerBuffer[ClientNo][1];
	for(i = 0; i < ServerBuffer[ClientNo][2]; i++)
	{
		pPP[i] = ServerBuffer[ClientNo][3+i];
	}

	ClientCLBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------

extern const u8 PokeEnemyPosX[2][2];
extern const u8 PokeEnemyPosY[2][2];

void ClientCLBSSPokeEncount(void)
{
	u16	monsno;

	monsno=PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_monsno);

	PokemonFObjCellPalSet(&PokeParaEnemy[SelMonsNo[ClientNo]], ClientNo);
	//PokeActHeaderMake(PokeParaEnemy[SelMonsNo[ClientNo]].PPP.monsno,ClientNo);
	PokeActHeaderMake(monsno,ClientTypeGet(ClientNo));	//�ύX by matsuda 2001.09.26(��)
#if 0
	PokemonStruct[ClientNo]=AddActor(&PokeActHeader,
//		PokeEnemyPosX[FightType][ClientNo>>1],
//		PokeEnemyPosY[FightType][ClientNo>>1]+4*(8-pm2fpara[PokeParaEnemy[SelMonsNo[ClientNo]].PPP.monsno].size),
//		changed by soga 2001.09.26
		PokeEnemyPosX[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1],
//		PokeEnemyPosY[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1]+4*(8-pm2fpara[PokeParaEnemy[SelMonsNo[ClientNo]].PPP.monsno].size),
		PokeEnemyPosY[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(ClientNo)>>1]+pm2fpara[PokeParaEnemy[SelMonsNo[ClientNo]].PPP.monsno].patcnt,
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

	//�e���
	PokeShadowSeqSet(ClientNo, 
		PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]], ID_monsno));

	ClientSequence[ClientNo]=ClientCLPokeEncountWait;
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ްق����яo��)
//----------------------------------------------------------
void ClientCLBSSPokeAppear(void)
{
	SelMonsNo[ClientNo] = ServerBuffer[ClientNo][1];
	CLPokeAppearCommon(ClientNo, ServerBuffer[ClientNo][2]);

//	ClientCLBitReset();
	ClientSequence[ClientNo] = ClientCLPokeAppearWait;
}

//-- PokeAppear�ASendOut���ʏ��� --//
static void CLPokeAppearCommon(u8 client_no, u8 baton_flg)
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
//							4*(8-pm2fpara[PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_monsno)].size),
							pm2fpara[PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_monsno)].patcnt,
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
		MBSmokeSet(PokeEnemyPosX[FightCheck()][client_no>>1], PokeEnemyPosY[FightCheck()][ClientTypeGet(client_no)>>1]+20, 1);
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
void ClientCLBSSPokeReturn(void)
{
	if(ServerBuffer[ClientNo][1] == 0)
	{
		CliSys[ClientNo].WazaEffSeqNo = 0;
		ClientSequence[ClientNo] = ClientCLPokeReturnMigawariChk;
//		SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_ENEMY);
//		ClientSequence[ClientNo] = ClientCLPokeReturnWait;
	}
	else
	{
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		PokeShadowBanishSeqSet(ClientNo);
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientCLBitReset();
	}
}

enum{
	CL_RET_MIGAWARICHK,
	CL_RET_POKERETURN,
};
static void ClientCLPokeReturnMigawariChk(void)
{
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case CL_RET_MIGAWARICHK:
			if(Climem[ClientNo].migawari == 1)	//�u�݂����v�`�F�b�N
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			CliSys[ClientNo].WazaEffSeqNo = CL_RET_POKERETURN;
			break;
		case CL_RET_POKERETURN:					//�Z�G�t�F�N�g�Z�b�g
			if(CliSys[ClientNo].etc_eff)
				break;
			CliSys[ClientNo].WazaEffSeqNo = 0;
			SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_ENEMY);
			ClientSequence[ClientNo] = ClientCLPokeReturnWait;
			break;
	}
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------
#define TRAINER_IN_DX	(-200)		//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_IN_SP	4	//�݂��Ă���Ƃ��̽�߰��

void ClientCLBSSTrainerEncount(void)
{
	int	no;
	s16	ofsx;
//	if(ServerStatusFlag&SERVER_STATUS_FLAG_WAIT)
//		ServerStatusFlag&=SERVER_STATUS_FLAG_WAIT_OFF;
//	else{
//
	if(FightType&FIGHT_TYPE_4_2vs2){
		if(ClientTypeGet(ClientNo)&2)
			ofsx=-16;
		else
			ofsx=16;
		no=SioTrainerInfo[GetTeamPosID(ClientNo)].sex;
	}
	else{
		ofsx=0;
		no=SioTrainerInfo[GetSioConnectId()^1].sex;
	}

	TrainerFObjCellPalSet(no, ClientNo);

//	PokeActHeaderMake(no+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(no,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							CAI_POKE_X+ofsx,
							CAI_POKE_Y+4*(8-trfpara[no].size),
							//C_TRN_PRI);	//�ڰŰ
							GetPokeSoftPri(ClientNo));

//	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = -ENCOUNT_DX;
	ActWork[PokemonStruct[ClientNo]].work[0] = ENCOUNT_SPEED;
	ActWork[PokemonStruct[ClientNo]].oamData.Pltt 
		= ObjPalNumGet(TRFObjDataPal[no].reg_num);
	ActWork[PokemonStruct[ClientNo]].work[5]	//�]���^�Ȃ̂�Del���鎞��CharNo��
		= ActWork[PokemonStruct[ClientNo]].oamData.CharNo;	//�߂��K�v������
	ActWork[PokemonStruct[ClientNo]].oamData.CharNo
		= CellCharNumGet(TRFObjDataCell[no].act_num);
	ActWork[PokemonStruct[ClientNo]].oamData.AffineParam = no;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;
	
	ClientSequence[ClientNo] = ClientCLTrainerEncountWait;
//	}
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ΐ�I����)
//----------------------------------------------------------
void ClientCLBSSTrainerAppear(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�ڰŰ��߂��̪�Ă��N������
//----------------------------------------------------------

#define	TRAINER_OUT_SP2	4

void ClientCLBSSTrainerReturn(void)
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
	ClientSequence[ClientNo] = ClientCLTrainerReturnWait;
}

//----------------------------------------------------------
//	�߹�݂��C�₷��̪�Ă��N������
//----------------------------------------------------------
void ClientCLBSSPokeKizetu(void)
{
#if 0	//2002.09.03(��)
	SePlayPan(SE_POKE_DEAD, WAZA_SE_R);
	ActWork[PokemonStruct[ClientNo]].move=SeqEnemyPokeKizetsuInit;
	ClientSequence[ClientNo]=ClientCLPokeKizetsuWait;
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
			ClientSequence[ClientNo]=ClientCLPokeKizetsuWait;
			break;
	}
#endif
}

//----------------------------------------------------------
//	�߹�݂��S�ł����Ƃ��̴̪�Ă��N������
//----------------------------------------------------------
void ClientCLBSSZenmetsu(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl�����̴̪�Ă��o��
//----------------------------------------------------------
void ClientCLBSSGetSuccess(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl���s�̴̪�Ă��o��
//----------------------------------------------------------
void ClientCLBSSGetFail(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�߹�����ٱ��߂̴̪�Ă��o��
//----------------------------------------------------------
void ClientCLBSSPokeLevelUp(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�w�肵���Z���ް�̴̪�Ă��o��
//----------------------------------------------------------
#if 0	// 2002.07.22(��)
void ClientCLBSSWazaEffect(void)
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
		ClientCLBitReset();
		return;
	}

	if(FightCheck() && (AttackClient^2) == DefenceClient && WazaDamage > 0)
		wazano = WAZANO_MAX+1;	//�����ւ̍U���Ȃ�΁A�ȈՓI�ȴ̪�Ă��o��
	
	WazaEffectSet(wazano);
	ClientSequence[ClientNo] = ClientCLWazaEffectWait;
}
#else
enum{
	CL_MIGAWARI_CHK,
	CL_EFF_PUTCHK,
	CL_EFF_WAIT,
	CL_EFF_END,
};
void ClientCLBSSWazaEffect(void)
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
		ClientCLBitReset();
		return;
	}
	
	CliSys[ClientNo].WazaEffSeqNo = CL_MIGAWARI_CHK;
	ClientSequence[ClientNo] = ClientCLWazaEffectSet;
}

static void ClientCLWazaEffectSet(void)
{
	u16 wazano;
	u8 renzoku_cnt;
	
	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	renzoku_cnt = ServerBuffer[ClientNo][11];	//0:ɰ�� 1:�A���Z�I�� 2:�A���Z��
	
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case CL_MIGAWARI_CHK:	//�u�݂����v�`�F�b�N
			if(Climem[ClientNo].migawari == 1 && Climem[ClientNo].migawari_out == 0){
				Climem[ClientNo].migawari_out = 1;
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			}
			CliSys[ClientNo].WazaEffSeqNo = CL_EFF_PUTCHK;
			break;
		case CL_EFF_PUTCHK:					//�Z�G�t�F�N�g�Z�b�g
			if(CliSys[ClientNo].etc_eff)
				break;
			
//			if(FightCheck() && (AttackClient^2) == DefenceClient && WazaDamage > 0)
//				wazano = WAZANO_MAX+1;	//�����ւ̍U���Ȃ�΁A�ȈՓI�ȴ̪�Ă��o��

			PokeAffineON_OFF(0);
			WazaEffectSet(wazano);
			CliSys[ClientNo].WazaEffSeqNo = CL_EFF_WAIT;
			break;
		case CL_EFF_WAIT:					//�Z�G�t�F�N�g�I���҂�
			WazaEffectSequence();
			if(WazaEffectFlag==0){
				PokeAffineON_OFF(1);
				if(Climem[ClientNo].migawari && renzoku_cnt <= 1){	//�g����`�F�b�N
					SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
					Climem[ClientNo].migawari_out = 0;
				}
				CliSys[ClientNo].WazaEffSeqNo = CL_EFF_END;
			}
			break;
		case CL_EFF_END:
			if(CliSys[ClientNo].etc_eff)
				break;

			//�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
			PokeBanishMemory();
			wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
			MigawariFlgCheckSet(ClientNo, wazano);
			CliSys[ClientNo].WazaEffSeqNo = 0;
			ClientCLBitReset();
			break;
	}
}
#endif

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------
#if 0
void ClientCLBSSMessage(void)
{
#if 1
	ScrX0=0;
	ScrY0=0;
//	MsgPutSet(&FightMsg,&ServerBuffer[ClientNo][2],0x10,1,15);
//	TalkMsgSet(&FightMsg,&ServerBuffer[ClientNo][2],CGX_START+0x10,2,15);
	FightMsgCreate((u8 *)&ServerBuffer[ClientNo][2]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientCLMessageWait;
#else
//���Ȃ��΂�Ƃ��A���������Ƃ��̃A���[�g���b�Z�[�W���o���Ȃ��悤�ɂ���
	ClientCLBitReset();
#endif
}
#else
void ClientCLBSSMessage(void)
{
	u16	*MsgID;

	ScrX0=0;
	ScrY0=0;

	MsgID=(u16 *)&ServerBuffer[ClientNo][2];
	FightMsgID(MsgID[0]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientCLMessageWait;
}
#endif

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

void ClientCLBSSAlertMessage(void)
{
//	if(MineEnemyCheck(ClientNo)==SIDE_MINE)
//		ClientCLBSSMessage();
//	else
		ClientCLBitReset();
}

//----------------------------------------------------------
//	����ޑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientCLBSSCommandSelect(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽClientNo�̃V�[�P���X�i���o�[�Ɠ������Ƃ�
//----------------------------------------------------------
void ClientCLBSSCommandSelectCopy(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�Z�I����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientCLBSSWazaSelect(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	���ёI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientCLBSSItemSelect(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�߹�ݑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientCLBSSPokeSelect(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	������̪�Ă��N������
//----------------------------------------------------------
void ClientCLBSSEscape(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	HP�ް�ނ𑝌�����
//----------------------------------------------------------
void	ClientCLBSSHpGauge(void)
{
	s16 beHP;	//��������l

	BattleGaugeTransWram(0);
	beHP = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3] << 8);
	if(beHP!=JIBAKU_HP)
		GaugeDataSet(ClientNo, ClientGaugeSt[ClientNo],
		PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_hpmax),
		PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_hp), beHP);
	else
		GaugeDataSet(ClientNo, ClientGaugeSt[ClientNo],
		PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_hpmax),
		0,beHP);
	ClientSequence[ClientNo] = ClientCLHpGaugeWait;
}

//----------------------------------------------------------
//	EXP�ް�ނ𑝌�����
//----------------------------------------------------------
void ClientCLBSSExpGauge(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ�̪�Ă��N������
//----------------------------------------------------------
void ClientCLBSSStatusSet(void)
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

	ClientSequence[ClientNo] = ClientCLStatusEffWait;
}

void ClientCLBSSStatusEffect(void)
{
	u32 condition;
	u8 condition_flg;

	if(ClientSEWait(ClientNo))
		return;
	
	condition_flg = ServerBuffer[ClientNo][1];
	condition = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8) | 
				(ServerBuffer[ClientNo][4]<<16) | (ServerBuffer[ClientNo][5]<<24);
	ClientStatusEffSet(condition_flg, condition);
	ClientSequence[ClientNo] = ClientCLStatusEffWait;
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ��ر����
//----------------------------------------------------------
void ClientCLBSSStatusClear(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	���炩���ߗp�ӂ��ꂽ�ޯ̧���ް����i�[����
//----------------------------------------------------------
void ClientCLBSSDataTrans(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�w����ڽ(RAM,VRAM,CGRAM)���ް���]������
//----------------------------------------------------------
void ClientCLBSSDataRamTrans(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	���ް���]������
//----------------------------------------------------------
void ClientCLBSSBgmTrans(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�����ް���]������
//----------------------------------------------------------
void ClientCLBSSVoiceTrans(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�ƭ��őI����������ID��Ԃ�
//----------------------------------------------------------
void ClientCLBSSSelectReturn(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�߹�ݓ���ւ����s�������Ƃ�m�点��
//----------------------------------------------------------
void ClientCLBSSReshuffle(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	���юg�p��m�点��
//----------------------------------------------------------
void ClientCLBSSUseItem(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�ްَg�p��m�点��
//----------------------------------------------------------
void ClientCLBSSUseBall(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃N���A
//----------------------------------------------------------
void	ClientCLBSSLoopCountClear(void)
{
	WEW.loopcnt = 0;
	ClientCLBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃Z�b�g
//----------------------------------------------------------
void	ClientCLBSSLoopCountSet(void)
{
	WEW.loopcnt = ServerBuffer[ClientNo][1];
	ClientCLBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃N���A
//----------------------------------------------------------
void	ClientCLBSSSwitchFlagClear(void)
{
	WEW.flag = 0;
	ClientCLBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃Z�b�g
//----------------------------------------------------------
void	ClientCLBSSSwitchFlagSet(void)
{
	WEW.flag ^= 1;
	ClientCLBitReset();
}

//----------------------------------------------------------
//	��Ұ�ނ��󂯂��Ƃ����߹�ݓ_��
//----------------------------------------------------------
void ClientCLBSSPokeDamageBlink(void)
{
	//�߹�݂���Ư����ԂȂ��Blink���Ȃ�
	if(ActWork[PokemonStruct[ClientNo]].banish == 1){
		ClientCLBitReset();
		return;
	}

	ClientEffectWait = 1;	//Blink�̪�ĊJ�n�t���O�Z�b�g
	ActWork[PokemonStruct[ClientNo]].work[1] = 0;

	//-- �ް�ނ̃J���[�ύX & �h�炵�G�t�F�N�g�Z�b�g --//
	GaugeSwayEffect(ClientNo);

	ClientSequence[ClientNo] = ClientCLPokeDamageBlinkWait;
}

//----------------------------------------------------------
//	�Q�[�W������
//----------------------------------------------------------
void ClientCLBSSGaugeOff(void)
{
//	GaugeOFF(ClientGaugeSt[ClientNo]);
	ClientCLBitReset();
}

//----------------------------------------------------------
//	SE��炷
//----------------------------------------------------------
void ClientCLBSSSEPlay(void)
{
	s8 pan;
	
//	SePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	if(MineEnemyCheck(ClientNo) == SIDE_MINE)
		pan = WAZA_SE_L;
	else
		pan = WAZA_SE_R;
	SePlayPan(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8), pan);
	ClientCLBitReset();
}

//----------------------------------------------------------
//	ME��炷
//----------------------------------------------------------
void ClientCLBSSMEPlay(void)
{
	JinglePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�Ȃ�������炷
//----------------------------------------------------------
void ClientCLBSSVoicePlay(void)
{
//	VoicePlay(PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_monsno),25);
	//�C�⎞�̐��Ŗ��悤�ɕύX 2002.08.27(��) by matsuda
	VoicePlay3(PokeParaGet(&PokeParaEnemy[SelMonsNo[ClientNo]],ID_monsno),25,5);
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�G���J�E���g�G�t�F�N�g���Z�b�g
//----------------------------------------------------------
void ClientCLBSSEncountEffect(void)
{
	EncountEffectSet(ServerBuffer[ClientNo][1]);
	//OBJ���ړ��̃X�g�b�p�[
	EncountEffectFlag|=ENCOUNT_EFFECT_FLAG_OBJSTOP;	
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�ŏ��̃|�P�����o��(�ڰŰ�������ށA�߹�ݏo���Aү���ޏo���B�܂ł̈�A�̓���
//----------------------------------------------------------
void ClientCLBSSPokeSendOut(void)
{
	u8 id;
	
	DxDyAddClear(&ActWork[PokemonStruct[ClientNo]]);
	ActWork[PokemonStruct[ClientNo]].work[0] = ENEMYTRAINER_RETURN_SYNCNUM;
	ActWork[PokemonStruct[ClientNo]].work[2] = 240+40;
	ActWork[PokemonStruct[ClientNo]].work[4] = ActWork[PokemonStruct[ClientNo]].y;
	ActWork[PokemonStruct[ClientNo]].move = W_StraightSyncMove;
	SetWorkSeq(&ActWork[PokemonStruct[ClientNo]], CLTrainerDelEnd);
//	ActAnmChg(&ActWork[PokemonStruct[ClientNo]],1);

	//-- �߹�� --//
	id = AddTask(CLSendOutTask, 5);
	TaskTable[id].work[0] = ClientNo;

	//-- �莝���ް�ނ����� --//
	if(CliSys[ClientNo].temoti_gauge_flg)
		TaskTable[TemotiGauge[ClientNo]].TaskAdrs = TemotiBallOff;

	BtlWork->sendout_eff = 1;
	ClientSequence[ClientNo] = ClientCLDummy;
}

static void CLSendOutTask(u8 id)
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
		CLPokeAppearCommon(ClientNo, 0);	//ClientCLBSSPokeAppear();
	}
	else if(FightType&FIGHT_TYPE_4_2vs2)
	{	//2vs2 4�l�ʐM
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		CLPokeAppearCommon(ClientNo, 0);
	}
	else
	{	//2vs2
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		CLPokeAppearCommon(ClientNo, 0);	//ClientCLBSSPokeAppear();
		ClientNo ^= 2;
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		CLPokeAppearCommon(ClientNo, 0);	//ClientCLBSSPokeAppear();
		ClientNo ^= 2;	//�ύX����ClientNo�����ɖ߂�
	}

	ClientSequence[ClientNo] = ClientCLPokeSendOutWait;
	ClientNo = clientno_temp;
	DelTask(id);
}

static void CLTrainerDelEnd(actWork *xreg)
{
	TrainerFObjCellPalDel(xreg->oamData.AffineParam);
	xreg->oamData.CharNo = xreg->work[5];//DelActor���鎞�ɓ]����
							//����Ă����̈悪�J�������悤��CharNo�����ɖ߂�
	AffineDelActor(xreg);
	DelActor(xreg);
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W��\������
//----------------------------------------------------------

void	ClientCLBSSBallGaugeSet(void)
{
	BallGaugePara *bgp;
	
//�t���O���[�N��true�̎��́A�ǂ����T�C�h�Ȃ̂��`�F�b�N���āA�������̂Ƃ��͕\�������Ȃ��悤�ɂ���
	if(ServerBuffer[ClientNo][1]){
		if(MineEnemyCheck(ClientNo)==SIDE_MINE){
			ClientCLBitReset();
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
	ClientSequence[ClientNo] = ClientCLBallGaugeWait;
///	ClientCLBitReset();
}

static void ClientCLBallGaugeWait(void)
{
	if(CliSys[ClientNo].wait_work++ > TEMOTI_BALL_WAIT){
		CliSys[ClientNo].wait_work = 0;
		ClientCLBitReset();
	}
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W���\���ɂ���
//----------------------------------------------------------

void	ClientCLBSSBallGaugeOff(void)
{
	//-- �莝���ް�ނ����� --//
	if(CliSys[ClientNo].temoti_gauge_flg)
		TaskTable[TemotiGauge[ClientNo]].TaskAdrs = TemotiBallOff;
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�Q�[�W�̂����~�߂�
//----------------------------------------------------------

void ClientCLBSSGaugeYureStop(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�|�P������VANISH��ON/OFF����
//----------------------------------------------------------

void	ClientCLBSSVanishOnOff(void)
{
	if(ClientPokeExistCheck(ClientNo)){
		ActWork[PokemonStruct[ClientNo]].banish = ServerBuffer[ClientNo][1];
		PokeClientBanishMemory(ClientNo);
	}
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�퓬�G�t�F�N�g���o��
//----------------------------------------------------------

void	ClientCLBSSFightEffect(void)
{
	u8 eff_no;
	u16 monsno;
	
	if(ClientSEWait(ClientNo))
		return;
	
	eff_no = ServerBuffer[ClientNo][1];
	monsno = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8);
	
	if(SetFightEffect(ClientNo, ClientNo, ClientNo, eff_no, monsno))
		ClientCLBitReset();
	else
		ClientSequence[ClientNo] = ClientCLFightEffWait;
}

//----------------------------------------------------------
//	�ʐM�ҋ@��ʂ��o��
//----------------------------------------------------------

void	ClientCLBSSTuushinWait(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�L�����Ă���J�[�\���ʒu�̃N���A
//----------------------------------------------------------

void	ClientCLBSSCsrPosClear(void)
{
	ClientCLBitReset();
}

//----------------------------------------------------------
//	�ΐ�I����m�点��
//----------------------------------------------------------
void ClientCLBSSFightEnd(void)
{
#if 0
	WinLoseFlag=ServerBuffer[ClientNo][1]^0x3;
	BGM_FadeOutSet(5);
	LightFadeReq(N_to_B);
	ClientCLBitReset();
	if((FightType&FIGHT_TYPE_TUUSHIN)&&((FightType&FIGHT_TYPE_OYA)==0))
		ClientSequence[ClientNo] = ClientFightEndWait;
#else
	if(ServerBuffer[ClientNo][1]==FIGHT_DRAW)
		WinLoseFlag=ServerBuffer[ClientNo][1];
	else
		WinLoseFlag=ServerBuffer[ClientNo][1]^0x3;
	BGM_FadeOutSet(5);
	LightFadeReq(N_to_B);
	ClientCLBitReset();
	ClientSequence[ClientNo] = ClientFightEndWait;
#endif
}

//----------------------------------------------------------
//	��а
//----------------------------------------------------------
void ClientCLBSSEndID(void)
{
//	ClientCLBitReset();
}

