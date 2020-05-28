
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

void	ClientSBitReset(void);	//�͈͊O�̒l�̎��̏���
void	ClientSDummy(void);
void	ClientSInit(void);
void	ClientSCommandWait(void);

void	ClientSCommandSelect(void);
void	ClientSWazaSelect(void);
void	ClientSMessageWait(void);

void	ClientCubeCaseWait(void);
void	ClientCubeCaseReturnWait(void);

void	ClientSBSSPokeparaGet(void);
void	ClientSBSSPokeparaGet2(void);
void	ClientSBSSPokeparaPut(void);
void	ClientSBSSPokeparaPut2(void);
void	ClientSBSSPokeEncount(void);
void	ClientSBSSPokeAppear(void);
void	ClientSBSSPokeReturn(void);
void	ClientSBSSTrainerEncount(void);
void	ClientSBSSTrainerAppear(void);
void	ClientSBSSTrainerReturn(void);
void	ClientSBSSPokeKizetu(void);
void	ClientSBSSZenmetsu(void);
void	ClientSBSSGetSuccess(void);
void	ClientSBSSGetFail(void);
void	ClientSBSSPokeLevelUp(void);
void	ClientSBSSWazaEffect(void);
void	ClientSBSSMessage(void);
void	ClientSBSSAlertMessage(void);
void	ClientSBSSCommandSelect(void);
void	ClientSBSSCommandSelectCopy(void);
void	ClientSBSSWazaSelect(void);
void	ClientSBSSItemSelect(void);
void	ClientSBSSPokeSelect(void);
void	ClientSBSSEscape(void);
void	ClientSBSSHpGauge(void);
void	ClientSBSSExpGauge(void);
void	ClientSBSSStatusSet(void);
void	ClientSBSSStatusEffect(void);
void	ClientSBSSStatusClear(void);
void	ClientSBSSDataTrans(void);
void	ClientSBSSDataRamTrans(void);
void	ClientSBSSBgmTrans(void);
void	ClientSBSSVoiceTrans(void);
void	ClientSBSSSelectReturn(void);
void	ClientSBSSReshuffle(void);
void	ClientSBSSUseItem(void);
void	ClientSBSSUseBall(void);
void	ClientSBSSLoopCountClear(void);
void	ClientSBSSLoopCountSet(void);
void	ClientSBSSSwitchFlagClear(void);
void	ClientSBSSSwitchFlagSet(void);
void	ClientSBSSPokeDamageBlink(void);
void	ClientSBSSGaugeOff(void);
void	ClientSBSSSEPlay(void);
void	ClientSBSSMEPlay(void);
void	ClientSBSSVoicePlay(void);
void	ClientSBSSEncountEffect(void);
void	ClientSBSSPokeSendOut(void);
void	ClientSBSSBallGaugeSet(void);
void	ClientSBSSBallGaugeOff(void);
void	ClientSBSSGaugeYureStop(void);
void	ClientSBSSVanishOff(void);
void	ClientSBSSFightEffect(void);
void	ClientSBSSTuushinWait(void);
void	ClientSBSSCsrPosClear(void);
void	ClientSBSSFightEnd(void);
void	ClientSBSSEndID(void);

int		ClientSBSSPokeparaGetAct(u8,u8 *);
void	ClientSBSSPokeparaPutAct(u8);

extern	actHeader PokeFTbl[];
extern	TALK_WIN	FightMsg;
extern u16	ReturnItemNo;
//extern	u8	FightMsgBuf[64];

//----------------------------------------------------------
//	�o�g���T�[�o�[���ߌQ�e�[�u��
//----------------------------------------------------------
const pFunc ClientSBSSTbl[] =
{
	ClientSBSSPokeparaGet,		// BSS_POKEPARA_GET=0,	
	ClientSBSSPokeparaGet2,	   	// BSS_POKEPARA_GET2,	
	ClientSBSSPokeparaPut,		// BSS_POKEPARA_PUT,	
	ClientSBSSPokeparaPut2,		// BSS_POKEPARA_PUT2,	
	ClientSBSSPokeEncount,		// BSS_POKEPARA_ENCOUNT,	add
	ClientSBSSPokeAppear,		// BSS_POKE_APPEAR,	
	ClientSBSSPokeReturn,		// BSS_POKE_RETURN,	
	ClientSBSSTrainerEncount,	// BSS_TRAINER_ENCOUNT,	add
	ClientSBSSTrainerAppear,		// BSS_TRAINER_APPEAR,	
	ClientSBSSTrainerReturn,		// BSS_TRAINER_RETURN,	
	ClientSBSSPokeKizetu,		// BSS_POKE_KIZETSU,	
	ClientSBSSZenmetsu,			// BSS_POKE_ZENMETSU,	
	ClientSBSSGetSuccess,		// BSS_GET_SUCCESS,	
	ClientSBSSGetFail,			// BSS_GET_FAIL,		
	ClientSBSSPokeLevelUp,		// BSS_POKE_LEVELUP,	
	ClientSBSSWazaEffect,		// BSS_WAZA_EFFECT,	
	ClientSBSSMessage,			// BSS_MESSAGE,		
	ClientSBSSAlertMessage,		// BSS_ALERT_MESSAGE,		
								
	ClientSBSSCommandSelect,		// BSS_COMMAND_SELECT, 

	ClientSBSSCommandSelectCopy,		// BSS_COMMAND_SELECT_COPY, 

	ClientSBSSWazaSelect,		// BSS_WAZA_SELECT,	
													
	ClientSBSSItemSelect,		// BSS_ITEM_SELECT,	
													
	ClientSBSSPokeSelect,		// BSS_POKE_SELECT,	
													
	ClientSBSSEscape,			// BSS_ESCAPE,			
	ClientSBSSHpGauge,			// BSS_HP_GAUGE,		
	ClientSBSSExpGauge,			// BSS_EXP_GAUGE,		
	ClientSBSSStatusSet,			// BSS_STATUS_SET,		
	ClientSBSSStatusEffect,		// BSS_STATUS_EFFECT,		
	ClientSBSSStatusClear,		// BSS_STATUS_CLEAR,	
	ClientSBSSDataTrans,			// BSS_DATA_TRANS,		
	ClientSBSSDataRamTrans,		// BSS_DATA_RAM_TRANS,	
													
	ClientSBSSBgmTrans,			// BSS_BGM_TRANS,		
	ClientSBSSVoiceTrans,		// BSS_VOICE_TRANS,	
	ClientSBSSSelectReturn,		// BSS_SELECT_RETURN,	
	ClientSBSSReshuffle,			// BSS_POKE_RESHUFFLE,	
	ClientSBSSUseItem,			// BSS_USE_ITEM,		
	ClientSBSSUseBall,			// BSS_USE_BALL,		
	ClientSBSSLoopCountClear,	//	BSS_LOOPCOUNT_CLEAR
	ClientSBSSLoopCountSet,	//	BSS_LOOPCOUNT_SET
	ClientSBSSSwitchFlagClear,	//	BSS_SWITCHFLAG_CLEAR
	ClientSBSSSwitchFlagSet,	//	BSS_SWITCHFLAG_SET
	ClientSBSSPokeDamageBlink,	// BSS_POKEDAMAGE_BLINK,
	ClientSBSSGaugeOff,		// BSS_GAUGE_OFF,
	ClientSBSSSEPlay,			// BSS_SE_PLAY,
	ClientSBSSMEPlay,			// BSS_SE_PLAY,
	ClientSBSSVoicePlay,			// BSS_VOICE_PLAY,
	ClientSBSSEncountEffect,	// BSS_ENCOUNT_EFFECT,
	ClientSBSSPokeSendOut,		// BSS_POKE_SENDOUT,
	ClientSBSSBallGaugeSet,	// BSS_BALL_GAUGE_SET,
	ClientSBSSBallGaugeOff,	// BSS_BALL_GAUGE_OFF,
	ClientSBSSGaugeYureStop,	// BSS_BALL_GAUGE_OFF,
	ClientSBSSVanishOff,	// BSS_VANISH_OFF,
	ClientSBSSFightEffect,	// BSS_FIGHT_EFFECT,
	ClientSBSSTuushinWait,	// BSS_TUUSHIN_WAIT,
	ClientSBSSCsrPosClear,		// BSS_CSRPOS_CLEAR,
	ClientSBSSFightEnd,		// BSS_FIGHT_END,

	ClientSBSSEndID,				// BSS_END_ID,		����Ȃ����ǈꉞ
};

//----------------------------------------------------------
//	�v���g�^�C�v�錾	�֐����ް�
//----------------------------------------------------------

#define	CAI_LVNUM_X			84
#define	CAI_LVNUM_Y			28

//==============================================================
//                       ���C�����[�`��
//==============================================================

void	ClientSDummy(void)
{
}

void	ClientSInit(void)
{
	ClientSequence[ClientNo]=ClientSCommandWait;

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

void	ClientSCommandWait(void)
{
	if((ClientBit&no2bittbl[ClientNo]) != 0)
	{
		if(ServerBuffer[ClientNo][0] <= BSS_END_ID)// && ServerBuffer[ClientNo][0] > -1)
		{
			ClientSBSSTbl[ServerBuffer[ClientNo][0]]();
		}
		else
		{
			ClientSBitReset();	//�͈͊O�̒l�̎��̏���
		}
	}
}

//=========================================================================
//	�E�F�C�g���[�`��
//=========================================================================

void	ClientSCommandSelect(void)
{
//	SelectMotionSet(ClientNo, MOTION_GAUGE, GAUGE_MOTION_ADDSEC, GAUGE_MOTION_FURIHABA);
//	SelectMotionSet(ClientNo, MOTION_POKE, POKE_MOTION_ADDSEC, POKE_MOTION_FURIHABA);

	if((sys.Trg&A_BUTTON)==A_BUTTON){
		SePlay(SE_SELECT);
		switch(CommandCsrPos[ClientNo]){
			case BALL_CSRPOS:	//�T�t�@���{�[��
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_BALL,0);
				break;
			case CUBE_CSRPOS:	//�L���[�u
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_CUBE,0);
				break;
			case APPROACH_CSRPOS:	//�����Â�
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_APPROACH,0);
				break;
			case ESCAPE_CSRPOS:	//�ɂ���
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_SAFARI_ESCAPE,0);
				break;
		}
		ClientSBitReset();
		return;
	}
	if((sys.Trg&L_KEY)==L_KEY){
		if(CommandCsrPos[ClientNo]&1){
			SePlay(SE_SELECT);
			CommandCsrOff(CommandCsrPos[ClientNo]);
			CommandCsrPos[ClientNo]^=1;
			CommandCsrOn(CommandCsrPos[ClientNo],0);
		}
		return;
	}
	if((sys.Trg&R_KEY)==R_KEY){
		if((CommandCsrPos[ClientNo]&1)==0){
			SePlay(SE_SELECT);
			CommandCsrOff(CommandCsrPos[ClientNo]);
			CommandCsrPos[ClientNo]^=1;
			CommandCsrOn(CommandCsrPos[ClientNo],0);
		}
		return;
	}
	if((sys.Trg&U_KEY)==U_KEY){
		if(CommandCsrPos[ClientNo]&2){
			SePlay(SE_SELECT);
			CommandCsrOff(CommandCsrPos[ClientNo]);
			CommandCsrPos[ClientNo]^=2;
			CommandCsrOn(CommandCsrPos[ClientNo],0);
		}
		return;
	}
	if((sys.Trg&D_KEY)==D_KEY){
		if((CommandCsrPos[ClientNo]&2)==0){
			SePlay(SE_SELECT);
			CommandCsrOff(CommandCsrPos[ClientNo]);
			CommandCsrPos[ClientNo]^=2;
			CommandCsrOn(CommandCsrPos[ClientNo],0);
		}
		return;
	}
#ifdef PM_DEBUG
	if((sys.Trg&R_BUTTON)==R_BUTTON){
		if((CliSys[ClientNo].fight_eff)==0)
			SetFightEffect(ClientNo, ClientNo, ClientNo, FE_ID_POROC, 0);
		return;
	}

	if(sys.Trg&START_BUTTON){
		HPNumGaugeChangePut();
		return;
	}
#endif
}

void ClientSTrainerEncountWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientSBitReset();
	}
}

void	ClientSMessageWait(void)
{
	if(FightMsg.status==MSG_END){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientSCommandWait;
		ClientSBitReset();
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
#if 0
//	if(ActWork[ClientStruct[ClientNo]].act_sw == 0)
	if(ClientEffectWait == 0)
//	if(CliSys[ClientNo].ball_eff_wait == 0)
	{
//		AllDelActor(&ActWork[ClientStruct[ClientNo]]);
		ClientSBitReset();
	}
#else
	if(ClientEffectWait == 0 || CliSys[ClientNo].etc_eff == 0)
		ClientSBitReset();
#endif
}

void ClientCubeCaseWait(void)
{
	if(FadeData.fade_sw == 0)
	{
		ClientSequence[ClientNo] = ClientCubeCaseReturnWait;
		BtlCubeCaseCall();
	}
}

void ClientCubeCaseReturnWait(void)
{
	if((sys.pMainProc==MainFight)&&
	   (FadeData.fade_sw == 0))
	{
		BSS_USE_ITEM_SET(BSS_CLIENT, ReturnItemNo);
		ClientSBitReset();
	}
}

void ClientSFightEffWait(void)
{
	if(CliSys[ClientNo].fight_eff == 0)
		ClientSBitReset();
}


//==============================================================
//                       �T�u���[�`��
//==============================================================

void ClientSBitReset(void)
{
	u8	id;

	ClientSequence[ClientNo] = ClientSCommandWait;
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
void ClientSStatusEffWait(void)
{
	if(CliSys[ClientNo].status_eff == 0)
		ClientSBitReset();
}



//==============================================================
//	�N���C�A���g���ߊ֐�
//==============================================================

//----------------------------------------------------------
//	�w�肵���߹���ް����擾����
//----------------------------------------------------------

void	ClientSBSSPokeparaGet(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�w�肵�����ڽ���߹���ް����擾����
//----------------------------------------------------------

void ClientSBSSPokeparaGet2(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�w�肵���߹���ް���ύX����
//----------------------------------------------------------

void ClientSBSSPokeparaPut(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�w�肵�����ڽ���߹���ް���ύX����
//----------------------------------------------------------

void ClientSBSSPokeparaPut2(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------

void ClientSBSSPokeEncount(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ްق����яo��)
//----------------------------------------------------------

void ClientSBSSPokeAppear(void)
{
	ClientSBitReset();
}


//----------------------------------------------------------
//	�߹�݂�߂��̪�Ă��N������
//----------------------------------------------------------
void ClientSBSSPokeReturn(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------
#define TRAINER_IN_DX	(200)		//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_IN_SP	(-4)	//�݂��Ă���Ƃ��̽�߰��

void ClientSBSSTrainerEncount(void)
{
	TrainerBObjCellPalSet(MyData.my_sex, ClientNo);

//	PokeActHeaderMake(MyData.my_sex+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(MyData.my_sex,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							C_TRN_X,
							C_TRN_Y+4*(8-trbpara[MyData.my_sex].size),
							C_TRN_PRI);	//�ڰŰ

	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = ENCOUNT_DX;
	ActWork[PokemonStruct[ClientNo]].work[0] = -ENCOUNT_SPEED;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;

	ClientSequence[ClientNo] = ClientSTrainerEncountWait;
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ΐ�I����)
//----------------------------------------------------------

void ClientSBSSTrainerAppear(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�ڰŰ��߂��̪�Ă��N������
//----------------------------------------------------------


void ClientSBSSTrainerReturn(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�߹�݂��C�₷��̪�Ă��N������
//----------------------------------------------------------

void ClientSBSSPokeKizetu(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�߹�݂��S�ł����Ƃ��̴̪�Ă��N������
//----------------------------------------------------------

void ClientSBSSZenmetsu(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl�����̴̪�Ă��o��
//----------------------------------------------------------

void ClientSBSSGetSuccess(void)
{
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

void ClientSBSSGetFail(void)
{
	u8 rate;
	
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

void ClientSBSSPokeLevelUp(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�w�肵���Z���ް�̴̪�Ă��o��
//----------------------------------------------------------

void ClientSBSSWazaEffect(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

void ClientSBSSMessage(void)
{
	u16	*MsgID;

	ScrX0=0;
	ScrY0=0;

	MsgID=(u16 *)&ServerBuffer[ClientNo][2];
	FightMsgID(MsgID[0]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientSMessageWait;
}

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

void ClientSBSSAlertMessage(void)
{
	if(MineEnemyCheck(ClientNo)==SIDE_MINE)
		ClientSBSSMessage();
	else
		ClientSBitReset();
}

//----------------------------------------------------------
//	����ޑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

void ClientSBSSCommandSelect(void)
{
	int	i;

	ScrX0=0;
	ScrY0=0x0a0;

	FightMsg.palette=0;
	FontScrnBoxFill(&FightMsg,SPACE_CHR_D,2,15,2+25,15+3);	//add 2001.08.07(��)
	FontScrnBoxFill(&FightMsg,SPACE_CHR_D,2,35,2+14,35+1);	//add 2001.08.07(��)
	ClientSequence[ClientNo]=ClientSCommandSelect;
	MsgPutSet(&FightMsg, SafariCommandMsg, CGX_START2+0x10,  19, 35);
	MsgPut(&FightMsg);
	for(i=0;i<4;i++) CommandCsrOff(i);
	CommandCsrOn(CommandCsrPos[ClientNo],0);
	FightMsgCreate((u8 *)&SafariDousuruMsg);
	MsgPutSet(&FightMsg, FightMsgBuf, CGX_START2+0x38,  2, 35);
	MsgPut(&FightMsg);
}

//----------------------------------------------------------
//	�w�肳�ꂽClientNo�̃V�[�P���X�i���o�[�Ɠ������Ƃ�
//----------------------------------------------------------

void ClientSBSSCommandSelectCopy(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�Z�I����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

void ClientSBSSWazaSelect(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	���ёI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

void ClientSBSSItemSelect(void)
{
	PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
	ClientSequence[ClientNo] = ClientCubeCaseWait;
	TempClientNo = ClientNo;
}

//----------------------------------------------------------
//	�߹�ݑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

void ClientSBSSPokeSelect(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	������̪�Ă��N������
//----------------------------------------------------------

void ClientSBSSEscape(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	HP�ް�ނ𑝌�����
//----------------------------------------------------------

void	ClientSBSSHpGauge(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	EXP�ް�ނ𑝌�����
//----------------------------------------------------------
void ClientSBSSExpGauge(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ�̪�Ă��N������
//----------------------------------------------------------

void ClientSBSSStatusSet(void)
{
	GaugeTransChrSet(ClientGaugeSt[ClientNo],&PokeParaMine[SelMonsNo[ClientNo]],G_SAFARIBALL);
	ClientSBitReset();
}

void ClientSBSSStatusEffect(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ��ر����
//----------------------------------------------------------

void ClientSBSSStatusClear(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	���炩���ߗp�ӂ��ꂽ�ޯ̧���ް����i�[����
//----------------------------------------------------------

void ClientSBSSDataTrans(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�w����ڽ(RAM,VRAM,CGRAM)���ް���]������
//----------------------------------------------------------

void ClientSBSSDataRamTrans(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	���ް���]������
//----------------------------------------------------------

void ClientSBSSBgmTrans(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�����ް���]������
//----------------------------------------------------------

void ClientSBSSVoiceTrans(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�ƭ��őI����������ID��Ԃ�
//----------------------------------------------------------

void ClientSBSSSelectReturn(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�߹�ݓ���ւ����s�������Ƃ�m�点��
//----------------------------------------------------------

void ClientSBSSReshuffle(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	���юg�p��m�点��
//----------------------------------------------------------

void ClientSBSSUseItem(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�ްَg�p��m�点��
//----------------------------------------------------------

void ClientSBSSUseBall(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃N���A
//----------------------------------------------------------

void	ClientSBSSLoopCountClear(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃J�E���^�̃Z�b�g
//----------------------------------------------------------

void	ClientSBSSLoopCountSet(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃N���A
//----------------------------------------------------------

void	ClientSBSSSwitchFlagClear(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//�Z�G�t�F�N�g�p�̃t���O�̃Z�b�g
//----------------------------------------------------------

void	ClientSBSSSwitchFlagSet(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	��Ұ�ނ��󂯂��Ƃ����߹�ݓ_��
//----------------------------------------------------------

void ClientSBSSPokeDamageBlink(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�Q�[�W������
//----------------------------------------------------------

void ClientSBSSGaugeOff(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	SE��炷
//----------------------------------------------------------

void ClientSBSSSEPlay(void)
{
	s8 pan;
	
//	SePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	if(MineEnemyCheck(ClientNo) == SIDE_MINE)
		pan = WAZA_SE_L;
	else
		pan = WAZA_SE_R;
	SePlayPan(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8), pan);
	ClientSBitReset();
}

//----------------------------------------------------------
//	ME��炷
//----------------------------------------------------------

void ClientSBSSMEPlay(void)
{
	JinglePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	ClientSBitReset();
}

//----------------------------------------------------------
//	�Ȃ�������炷
//----------------------------------------------------------

void ClientSBSSVoicePlay(void)
{
	VoicePlay(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno),25);
	ClientSBitReset();
}

//----------------------------------------------------------
//	�G���J�E���g�G�t�F�N�g���Z�b�g
//----------------------------------------------------------

void ClientSBSSEncountEffect(void)
{
	EncountEffectSet(ServerBuffer[ClientNo][1]);
	//OBJ���ړ��̃X�g�b�p�[
	EncountEffectFlag|=ENCOUNT_EFFECT_FLAG_OBJSTOP;	
	ClientSBitReset();
}

//----------------------------------------------------------
//	�ŏ��̃|�P�����o��(�ڰŰ�������ށA�߹�ݏo���Aү���ޏo���B�܂ł̈�A�̓���
//----------------------------------------------------------

void ClientSBSSPokeSendOut(void)
{
	GaugeTransChrSet(ClientGaugeSt[ClientNo],&PokeParaMine[SelMonsNo[ClientNo]],G_SAFARI);
	GaugeAppearEffect(ClientNo);
	GaugeON(ClientGaugeSt[ClientNo]);
	ClientSBitReset();
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W��\������
//----------------------------------------------------------

void	ClientSBSSBallGaugeSet(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W���\���ɂ���
//----------------------------------------------------------

void	ClientSBSSBallGaugeOff(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�Q�[�W�̂����~�߂�
//----------------------------------------------------------

void ClientSBSSGaugeYureStop(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�|�P������VANISH��OFF����
//----------------------------------------------------------

void	ClientSBSSVanishOff(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�퓬�G�t�F�N�g���o��
//----------------------------------------------------------

void	ClientSBSSFightEffect(void)
{
	u8 eff_no;
	u16 monsno;
	
	eff_no = ServerBuffer[ClientNo][1];
	monsno = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8);
	
	if(SetFightEffect(ClientNo, ClientNo, ClientNo, eff_no, monsno))
		ClientSBitReset();
	else
		ClientSequence[ClientNo] = ClientSFightEffWait;
}

//----------------------------------------------------------
//	�ʐM�ҋ@��ʂ��o��
//----------------------------------------------------------

void	ClientSBSSTuushinWait(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�L�����Ă���J�[�\���ʒu�̃N���A
//----------------------------------------------------------

void	ClientSBSSCsrPosClear(void)
{
	ClientSBitReset();
}

//----------------------------------------------------------
//	�ΐ�I����m�点��
//----------------------------------------------------------

void ClientSBSSFightEnd(void)
{
	WinLoseFlag=ServerBuffer[ClientNo][1];
	BGM_FadeOutSet(5);
	LightFadeReq(N_to_B);
	ClientSBitReset();
	if((FightType&FIGHT_TYPE_TUUSHIN)&&((FightType&FIGHT_TYPE_OYA)==0))
		ClientSequence[ClientNo] = ClientFightEndWait;
}

//----------------------------------------------------------
//	��а
//----------------------------------------------------------
void ClientSBSSEndID(void)
{
//	ClientSBitReset();
}

