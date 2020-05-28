//======================================================
//    client.c                                           
//                                                      
//    Copyright (C) 2000 GAME FREAK inc.
//======================================================

#include "common.h"
#include "bss.h"		//add
#include "ef_tool.h"
#include "calctool.h"
#include "poketool.h"		//add
#include "decord.h"		//add
#include "objdata.h"		//add

#include "client2.h"
#include "client_t.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "cli_def.h"
#include "madefine.h"
#include "gauge.h"
#include "a_mine.h"
#include "a_enemy.h"
#include "cli_ef.h"
#include "pokelist.h"
#include "fight.h"
#include "print.h"
#include "waza_eff.h"
#include "b_return.h"
#include "pm_aff.h"
#include "server.h"
#include "a_status.h"
#include "b_pmlist.h"
#include "poketool.h"
#include "task.h"
#include "wazatool.h"
#include "field.h"
#include "ev_fight.h"
#include "waza_tbl.h"
#include "cli_num.h"

#include "multisio.h"		//add
#include "pokesio.h"
#include "cable.h"

#include "client.h"
#include "fight_ef.h"
#include "pm_str.h"
#include "mus_tool.h"
#include "clientclm.h"
#include "fightmsg.h"
#include "wazano.def"
#include "a_fight.h"
#include "waza_dat.h"
#include "item.h"
#include "..\data\itemsym.h"

enum{
		MSG_END,
		MSG_START,
		MSG_WRITE,
		MSG_WAIT,
		MSG_KEYWAIT,
		MSG_CR,
};


//=========================================================================
//	�O���[�o���ϐ�
//=========================================================================
u8	ClientStruct[CLIENT_MAX];	//���������Ď��p
u8	ClientGaugeSt[CLIENT_MAX];	//�ް�ޱ����Ď��p
u8	CommandCsrPos[CLIENT_MAX]={};
u8	WazaCsrPos[CLIENT_MAX]={};
u8	TemotiGauge[CLIENT_MAX] = {};	//�莝���ް�ނ̊Ǘ�TaskNo
u8	ClientCsrPos2;
u8	ClientCsrPosMax;
extern u16	ReturnItemNo;
u8 TempClientNo = 0;
u8 ClientEffectWait = 0;

u32	HenshinRnd[4]={};

//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
void	ClientDummy(void);
void	ClientInit(void);
void	ClientCommandWait(void);
void	ClientWazaSelect(void);
u8		ClientWazaSelectShinka(void);
void	ClientWazaPosSwap(void);
void	WazaListMake(void);
/*--------------------------------------------
void	ClientPokeEncountWait(void);

void	ClientCommandSelect(void);

void	ClientBSSPokeparaGet(void);
void	ClientBSSPokeEncount(void);
--------------------------------------------*/
void ClientBSSPokeparaGet(void);
void ClientBSSPokeparaGet2(void);
void ClientBSSPokeparaPut(void);
void ClientBSSPokeparaPut2(void);
void ClientBSSPokeEncount(void);
void ClientBSSPokeAppear(void);
void ClientBSSPokeReturn(void);
void ClientBSSTrainerEncount(void);
void ClientBSSTrainerAppear(void);
void ClientBSSTrainerReturn(void);
void ClientBSSPokeKizetu(void);
void ClientBSSZenmetsu(void);
void ClientBSSGetSuccess(void);
void ClientBSSGetFail(void);
void ClientBSSPokeLevelUp(void);
void ClientBSSWazaEffect(void);
void ClientBSSMessage(void);
void ClientBSSAlertMessage(void);
void ClientBSSCommandSelect(void);
void ClientBSSCommandSelectCopy(void);
void ClientBSSWazaSelect(void);
void ClientBSSWazaSelectAct(void);
void ClientBSSItemSelect(void);
void ClientBSSPokeSelect(void);
void ClientBSSEscape(void);
void ClientBSSHpGauge(void);
void ClientBSSExpGauge(void);
void ClientBSSStatusSet(void);
void ClientBSSStatusEffect(void);
void ClientBSSStatusClear(void);
void ClientBSSDataTrans(void);
void ClientBSSDataRamTrans(void);
void ClientBSSBgmTrans(void);
void ClientBSSVoiceTrans(void);
void ClientBSSSelectReturn(void);
void ClientBSSReshuffle(void);
void ClientBSSUseItem(void);
void ClientBSSUseBall(void);
void ClientBSSLoopCountClear(void);
void ClientBSSLoopCountSet(void);
void ClientBSSSwitchFlagClear(void);
void ClientBSSSwitchFlagSet(void);
void ClientBSSPokeDamageBlink(void);
void ClientBSSGaugeOff(void);
void ClientBSSSEPlay(void);
void ClientBSSMEPlay(void);
void ClientBSSVoicePlay(void);
void ClientBSSEncountEffect(void);
void ClientBSSPokeSendOut(void);
void ClientBSSBallGaugeSet(void);
void ClientBSSBallGaugeOff(void);
void ClientBSSGaugeYureStop(void);
void ClientBSSVanishOnOff(void);
void ClientBSSFightEffect(void);
void ClientBSSTuushinWait(void);
void ClientBSSCsrPosClear(void);
void ClientBSSFightEnd(void);
void ClientBSSEndID(void);
static void ClientPokeReturnMigawariChk(void);

int	ClientBSSPokeparaGetAct(u8,u8 *);
void ClientBSSPokeparaPutAct(u8);

void	WazaCsrOn(u8,u8);
void	WazaCsrOff(u8);
void	CommandCsrOn(u8,u8);
void	CommandCsrOff(u8);
void	PPMsgPut(void);
void	WazaTypeMsgPut(void);
void	ClientMessageWait(void);
void ClientPokeListReturn(void);
static void ClientPokeListReturnWait(void);
void ClientItemReturnWait(void);
void	TuushinTaikiCheck(void);
static void ClientEXPGaugeWaitSeq(u8 id);
static void ClientEXPGaugeInitSeq(u8 id);
//static void WazaOboeWaitSeq(u8 id);
//static void WazaOboeInitSeq(u8 id);
static void ClientLvUpMsgInitSeq(u8 id);
static void ClientLvUpEffect(u8 id);
static void ClientFightEndWait(void);
static void SendOutTask(u8 id);
void TrainerDelEnd(actWork *xreg);
static void PokeAppearCommon(u8 client_no, u8);
void	ClientDirSelectInit(void);
void	ClientDirSelect(void);
void ClientStatusEffWait(void);
static void ClientLvUpEffectWait(u8 id);
static void ClientWazaEffectSet(void);
static void ClientAppearMigawariEffWait(void);
static void ClientBallGaugeWait(void);
#ifdef PM_DEBUG
static void ClientWazaEffectDebug(void);
static void DebugWazaEffWait(void);
#endif

//=========================================================================
//	�O���֐��錾
//=========================================================================
extern void BtlRuckInit(void);

//=========================================================================
//	�f�[�^
//=========================================================================
extern const u32 GrowTbl[][101];
extern	TALK_WIN	FightMsg;
//extern	u8	FightMsgBuf[64];
//extern	u8	WazaNameData[][8];		//soga 2001.10.03
extern	u8	ZokuseiMsg[][5];

#if 0
static const u8 KeikentiGet_msg[] = {
	I_MSG_,STR_TEMP_BUF0,ha_,CR_,
	I_MSG_,STR_TEMP_BUF1,spc_,ke_,i_,ke_,n_,ti_,wo_,spc_,mo_,ra_,ttu_,ta_,gyoe_,TRG_WAIT_,EOM_
};
static const u8 LevelUp_msg[] = {
	I_MSG_,STR_TEMP_BUF0,ha_,CR_,
	RE_,BE_,RU_,I_MSG_,STR_TEMP_BUF1,spc_,ni_,spc_,a_,ga_,ttu_,ta_,gyoe_,TRG_WAIT_,EOM_
};
#endif

static const u8 turn_msg[] = {TA_,bou_,N_,EOM_};	//�f�o�b�O�p���b�Z�[�W

//----------------------------------------------------------
//	�o�g���T�[�o�[���ߌQ�e�[�u��
//----------------------------------------------------------
const pFunc ClientBSSTbl[] =
{
	ClientBSSPokeparaGet,		// BSS_POKEPARA_GET=0,	
	ClientBSSPokeparaGet2,	   	// BSS_POKEPARA_GET2,	
	ClientBSSPokeparaPut,		// BSS_POKEPARA_PUT,	
	ClientBSSPokeparaPut2,		// BSS_POKEPARA_PUT2,	
	ClientBSSPokeEncount,		// BSS_POKEPARA_ENCOUNT,	add
	ClientBSSPokeAppear,		// BSS_POKE_APPEAR,	
	ClientBSSPokeReturn,		// BSS_POKE_RETURN,	
	ClientBSSTrainerEncount,	// BSS_TRAINER_ENCOUNT,	add
	ClientBSSTrainerAppear,		// BSS_TRAINER_APPEAR,	
	ClientBSSTrainerReturn,		// BSS_TRAINER_RETURN,	
	ClientBSSPokeKizetu,		// BSS_POKE_KIZETSU,	
	ClientBSSZenmetsu,			// BSS_POKE_ZENMETSU,	
	ClientBSSGetSuccess,		// BSS_GET_SUCCESS,	
	ClientBSSGetFail,			// BSS_GET_FAIL,		
	ClientBSSPokeLevelUp,		// BSS_POKE_LEVELUP,	
	ClientBSSWazaEffect,		// BSS_WAZA_EFFECT,	
	ClientBSSMessage,			// BSS_MESSAGE,		
	ClientBSSAlertMessage,		// BSS_ALERT_MESSAGE,
								
	ClientBSSCommandSelect,		// BSS_COMMAND_SELECT, 
	ClientBSSCommandSelectCopy,	// BSS_COMMAND_SELECT_COPY, 
													
	ClientBSSWazaSelect,		// BSS_WAZA_SELECT,	
													
	ClientBSSItemSelect,		// BSS_ITEM_SELECT,	
													
	ClientBSSPokeSelect,		// BSS_POKE_SELECT,	
													
	ClientBSSEscape,			// BSS_ESCAPE,			
	ClientBSSHpGauge,			// BSS_HP_GAUGE,		
	ClientBSSExpGauge,			// BSS_EXP_GAUGE,		
	ClientBSSStatusSet,			// BSS_STATUS_SET,		
	ClientBSSStatusEffect,		// BSS_STATUS_EFFECT,		
	ClientBSSStatusClear,		// BSS_STATUS_CLEAR,	
	ClientBSSDataTrans,			// BSS_DATA_TRANS,		
	ClientBSSDataRamTrans,		// BSS_DATA_RAM_TRANS,	
													
	ClientBSSBgmTrans,			// BSS_BGM_TRANS,		
	ClientBSSVoiceTrans,		// BSS_VOICE_TRANS,	
	ClientBSSSelectReturn,		// BSS_SELECT_RETURN,	
	ClientBSSReshuffle,			// BSS_POKE_RESHUFFLE,	
	ClientBSSUseItem,			// BSS_USE_ITEM,		
	ClientBSSUseBall,			// BSS_USE_BALL,		
	ClientBSSLoopCountClear,	// BSS_LOOPCOUNT_CLEAR,
	ClientBSSLoopCountSet,		// BSS_LOOPCOUNT_SET,
	ClientBSSSwitchFlagClear,	// BSS_SWITCHFLAG_CLEAR,
	ClientBSSSwitchFlagSet,		// BSS_SWITCHFLAG_SET,
	ClientBSSPokeDamageBlink,	// BSS_POKEDAMAGE_BLINK,
	ClientBSSGaugeOff,			// BSS_GAUGE_OFF,
	ClientBSSSEPlay,			// BSS_SE_PLAY,
	ClientBSSMEPlay,			// BSS_ME_PLAY,
	ClientBSSVoicePlay,			// BSS_VOICE_PLAY,
	ClientBSSEncountEffect,		// BSS_ENCOUNT_EFFECT,
	ClientBSSPokeSendOut,		// BSS_POKE_SENDOUT,
	ClientBSSBallGaugeSet,		// BSS_BALL_GAUGE_SET,
	ClientBSSBallGaugeOff,		// BSS_BALL_GAUGE_OFF,
	ClientBSSGaugeYureStop,		// BSS_BALL_GAUGE_OFF,
	ClientBSSVanishOnOff,			// BSS_VANISH_OFF,
	ClientBSSFightEffect,		// BSS_FIGHT_EFFECT,
	ClientBSSTuushinWait,		// BSS_TUUSHIN_WAIT,
	ClientBSSCsrPosClear,		// BSS_CSRPOS_CLEAR,
	ClientBSSFightEnd,			// BSS_FIGHT_END,
								
	ClientBSSEndID,				// BSS_END_ID,		����Ȃ����ǈꉞ
};


//==============================================================
//                       ���C�����[�`��
//==============================================================

void	ClientDummy(void)
{
}

void	ClientInit(void)
{
	ClientSequence[ClientNo] = ClientCommandWait;
	ClientEffectWait = 0;
	
#if 0	//�����]���p�Ɉ�ɂ܂Ƃ߂�
	//�퓬�p��ؽĉ�ʂŎg�p����ܰ��̏�����
	BattleListWorkInit();

	//�ް�ޖ{��
	ClientGaugeSt[ClientNo] = GaugeActorSet(ClientNo);
	GaugeDefaultPosSet(ClientNo);
	GaugePriSet(ClientGaugeSt[ClientNo], 0);
	//�ް�ޓ]����׾��
	GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaMine[SelMonsNo[ClientNo]], G_ALL);
	GaugeOFF(ClientGaugeSt[ClientNo]);
	
	if(FightCheck() && ClientNo == BSS_CLIENT_MINE2)
		GaugePriSet(ClientGaugeSt[ClientNo], 1);
#endif
}

void ClientBitReset(void)
{
	u8	id;

	ClientSequence[ClientNo] = ClientCommandWait;
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

void	ClientCommandWait(void)
{
	if((ClientBit&no2bittbl[ClientNo]) != 0)
	{
		if(ServerBuffer[ClientNo][0] <= BSS_END_ID)// && ServerBuffer[ClientNo][0] > -1)
			ClientBSSTbl[ServerBuffer[ClientNo][0]]();
		else
			ClientBitReset();	//�͈͊O�̒l�̎��̏���
	}
}

//=========================================================================
//	�E�F�C�g���[�`��
//=========================================================================
void	ClientPokeEncountWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].dx==0)
		ClientBitReset();
}

void	ClientCommandSelect(void)
{
	u16	item;

	item=ServerBuffer[ClientNo][2]|(ServerBuffer[ClientNo][3]<<8);

	//GaugeArrowCheckON(ClientNo);
	SelectMotionSet(ClientNo, MOTION_GAUGE, GAUGE_MOTION_ADDSEC, GAUGE_MOTION_FURIHABA);
	SelectMotionSet(ClientNo, MOTION_POKE, POKE_MOTION_ADDSEC, POKE_MOTION_FURIHABA);

	if((sys.Trg&A_BUTTON)==A_BUTTON){
		SePlay(SE_SELECT);
		switch(CommandCsrPos[ClientNo])
		{
			case FIGHT_CSRPOS:	//��������
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_FIGHT,0);
				break;
			case ITEM_CSRPOS:	//�����b�N
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_ITEM,0);
				break;
			case POKE_CSRPOS:	//�|�P����
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_POKE,0);
				break;
			case ESCAPE_CSRPOS:	//�ɂ���
				BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_ESCAPE,0);
				break;
		}
		ClientBitReset();
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
	if((sys.Trg&B_BUTTON)==B_BUTTON){
		if((FightType&FIGHT_TYPE_2vs2)&&
		   (ClientTypeGet(ClientNo)==BSS_CLIENT_MINE2)&&
		   ((NoReshuffleClient&no2bittbl[ClientNoGet(BSS_CLIENT_MINE)])==0)&&
		  ((FightType&FIGHT_TYPE_4_2vs2)==0)){
			if((ServerBuffer[ClientNo][1]==BSS_SELECT_RETURN_ITEM)&&(item>ITEM_BALL_END))
				return;
			else if((ServerBuffer[ClientNo][1]==BSS_SELECT_RETURN_ITEM)&&(item<=ITEM_BALL_END))
			//�I������SubItem����Ă���̂ŁA�߂�
				AddItem(item,1);
			SePlay(SE_SELECT);
			BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_B_CANCEL,0);
			ClientBitReset();
		}
		return;
	}
	
	if(sys.Trg&START_BUTTON)
		HPNumGaugeChangePut();
}

void	ClientDirSelectInit(void)
{
	SelectMotionStop(ClientNo, MOTION_GAUGE);
	SelectMotionStop(ClientNo, MOTION_POKE);
	ClientSequence[ClientNo] = ClientDirSelect;
}

void	ClientDirSelect(void)
{
	int i;
	int	flag=0,work=0;
	u8	ClientTbl[]={BSS_CLIENT_MINE,
				 	 BSS_CLIENT_MINE2,
				 	 BSS_CLIENT_ENEMY2,
				 	 BSS_CLIENT_ENEMY,
	};

	SelectMotionSet(ClientCsrPos2, MOTION_GAUGE,
		GAUGE_ENEMY_MOTION_ADDSEC, GAUGE_ENEMY_MOTION_FURIHABA);
	for(i = 0; i < ClientSetMax; i++){	//�I�𒆈ȊO���߹�ݑS�Ăɑ΂��čs��
		if(i != ClientCsrPos2)
			SelectMotionStop(i, MOTION_GAUGE);
	}
	
	if((sys.Trg&A_BUTTON)==A_BUTTON){
		SePlay(SE_SELECT);
		ActWork[PokemonStruct[ClientCsrPos2]].move=SeqEnemyPokeBlinkEnd;
		BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,WazaCsrPos[ClientNo]|(ClientCsrPos2<<8));
		SelectMotionStop(ClientCsrPos2, MOTION_GAUGE);
		ClientBitReset();
		return;
	}
	if((sys.Trg&B_BUTTON)==B_BUTTON){
		SePlay(SE_SELECT);
		ActWork[PokemonStruct[ClientCsrPos2]].move=SeqEnemyPokeBlinkEnd;
		ClientSequence[ClientNo] = ClientWazaSelect;
		SelectMotionSet(ClientNo, MOTION_GAUGE,
			GAUGE_MOTION_ADDSEC, GAUGE_MOTION_FURIHABA);
		SelectMotionSet(ClientNo, MOTION_POKE,
			POKE_MOTION_ADDSEC, POKE_MOTION_FURIHABA);
		SelectMotionStop(ClientCsrPos2, MOTION_GAUGE);
		return;
	}
	if(((sys.Trg&L_KEY)==L_KEY)||
	   ((sys.Trg&U_KEY)==U_KEY)){
		SePlay(SE_SELECT);
		ActWork[PokemonStruct[ClientCsrPos2]].move=SeqEnemyPokeBlinkEnd;
		do{
			work=ClientTypeGet(ClientCsrPos2);
			for(flag=0;flag<4;flag++){
				if(work==ClientTbl[flag]) break;
			}
			do{
				flag--;
				if(flag<0) flag=4;
				ClientCsrPos2=ClientNoGet(ClientTbl[flag]);
			}while(ClientCsrPos2==ClientSetMax);
			flag=0;
			switch(ClientTypeGet(ClientCsrPos2)){
				case BSS_CLIENT_MINE:
				case BSS_CLIENT_MINE2:
					if(ClientNo!=ClientCsrPos2){
						flag++;
						break;
					}
					if((WTD[PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+WazaCsrPos[ClientNo])].attackrange&RANGE_ALWAYS)!=0)
						flag++;
					break;
				case BSS_CLIENT_ENEMY:
				case BSS_CLIENT_ENEMY2:
					flag++;
					break;
			}
			if(NoReshuffleClient&no2bittbl[ClientCsrPos2])
				flag=0;
		}while(flag==0);
		ActWork[PokemonStruct[ClientCsrPos2]].move=SeqEnemyPokeBlinkInit;
		return;
	}
	if(((sys.Trg&R_KEY)==R_KEY)||
	   ((sys.Trg&D_KEY)==D_KEY)){
		SePlay(SE_SELECT);
		ActWork[PokemonStruct[ClientCsrPos2]].move=SeqEnemyPokeBlinkEnd;
		do{
			work=ClientTypeGet(ClientCsrPos2);
			for(flag=0;flag<4;flag++){
				if(work==ClientTbl[flag]) break;
			}
			do{
				flag++;
				if(flag>3) flag=0;
				ClientCsrPos2=ClientNoGet(ClientTbl[flag]);
			}while(ClientCsrPos2==ClientSetMax);
			flag=0;
			switch(ClientTypeGet(ClientCsrPos2)){
				case BSS_CLIENT_MINE:
				case BSS_CLIENT_MINE2:
					if(ClientNo!=ClientCsrPos2){
						flag++;
						break;
					}
					if((WTD[PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+WazaCsrPos[ClientNo])].attackrange&RANGE_ALWAYS)!=0)
						flag++;
					break;
				case BSS_CLIENT_ENEMY:
				case BSS_CLIENT_ENEMY2:
					flag++;
					break;
			}
			if(NoReshuffleClient&no2bittbl[ClientCsrPos2])
				flag=0;
		}while(flag==0);
		ActWork[PokemonStruct[ClientCsrPos2]].move=SeqEnemyPokeBlinkInit;
	}
}

void	ClientWazaSelect(void)
{
	int	flag=0;			//�`�F�b�N�p�t���O
	u8	range;
//	u16	monsno;
	WazaList	*WL;
#ifdef PM_DEBUG
	int i;
	u8 count = 0;
#endif

	
	WL=(WazaList *)&ServerBuffer[ClientNo][4];

	if((sys.Trg&A_BUTTON)==A_BUTTON){
		SePlay(SE_SELECT);
//		if(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+WazaCsrPos[ClientNo])==WAZANO_NOROI){
		if(WL->wazano[WazaCsrPos[ClientNo]]==WAZANO_NOROI){
//			monsno=PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno);
//			if((PPD[WL->monsno].type1!=GHOST_TYPE)&&(PPD[WL->monsno].type2!=GHOST_TYPE))
			if((WL->type1!=GHOST_TYPE)&&(WL->type2!=GHOST_TYPE))
				range=RANGE_MINE;
			else
				range=RANGE_NORMAL;
		}
		else
//			range=WTD[PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+WazaCsrPos[ClientNo])].attackrange;
			range=WTD[WL->wazano[WazaCsrPos[ClientNo]]].attackrange;
		if(range&RANGE_MINE)
			ClientCsrPos2=ClientNo;
		else
			ClientCsrPos2=ClientNoGet((ClientTypeGet(ClientNo)&1)^1);
		if(ServerBuffer[ClientNo][1]==0){
			if((range&RANGE_ALWAYS)&&(ServerBuffer[ClientNo][2]==BSS_WAZA_SELECT_NORMAL)){
				flag++;
			}
		}
		//2vs2�̎��̏���
		else{
			if((range&RANGE_NO_DIR)==0){
				flag++;
			}
			//PP��0�̎��͑I�������Ȃ�
			if(WL->pp[WazaCsrPos[ClientNo]]==0)
				flag=0;
			//�����I���n�̓`�F�b�N���Ȃ��Ă悢
			else if((range&(RANGE_MINE|RANGE_ALWAYS))==0){
				if(HitCountCheck(HCC_NORMAL)<2){
					ClientCsrPos2=GetDefenceClient(ClientNo);
					flag=0;
				}
			}
		}
		if(flag==0){
			BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,WazaCsrPos[ClientNo]|(ClientCsrPos2<<8));
			ClientBitReset();
		}
		else{
			ClientSequence[ClientNo] = ClientDirSelect;
			if(range&(RANGE_MINE|RANGE_ALWAYS))
				ClientCsrPos2=ClientNo;
			else{
				if(NoReshuffleClient&no2bittbl[ClientNoGet(BSS_CLIENT_ENEMY)])
					ClientCsrPos2 = ClientNoGet(BSS_CLIENT_ENEMY2);
				else
					ClientCsrPos2 = ClientNoGet(BSS_CLIENT_ENEMY);
			}
			ActWork[PokemonStruct[ClientCsrPos2]].move=SeqEnemyPokeBlinkInit;
		}
		return;
	}
	if((sys.Trg&B_BUTTON)==B_BUTTON){
		SePlay(SE_SELECT);
		ScrX0 = 0;
		ScrY0 = 0x140;
		BSS_SELECT_RETURN_SET(BSS_CLIENT,BSS_SELECT_RETURN_WAZA,0xffff);
		ClientBitReset();
		return;
	}
	if((sys.Trg&L_KEY)==L_KEY){
		if(WazaCsrPos[ClientNo]&1){
			WazaCsrOff(WazaCsrPos[ClientNo]);
			WazaCsrPos[ClientNo]^=0x01;
			SePlay(SE_SELECT);
			WazaCsrOn(WazaCsrPos[ClientNo],0);
			PPMsgPut();
			WazaTypeMsgPut();
		}
		return;
	}
	if((sys.Trg&R_KEY)==R_KEY){
		if(((WazaCsrPos[ClientNo]&1)==0)&&
			((WazaCsrPos[ClientNo]^0x01)<ClientCsrPosMax)){
			WazaCsrOff(WazaCsrPos[ClientNo]);
			WazaCsrPos[ClientNo]^=0x01;
			SePlay(SE_SELECT);
			WazaCsrOn(WazaCsrPos[ClientNo],0);
			PPMsgPut();
			WazaTypeMsgPut();
		}
		return;
	}
	if((sys.Trg&U_KEY)==U_KEY){
		if(WazaCsrPos[ClientNo]&2){
			WazaCsrOff(WazaCsrPos[ClientNo]);
			WazaCsrPos[ClientNo]^=0x02;
			SePlay(SE_SELECT);
			WazaCsrOn(WazaCsrPos[ClientNo],0);
			PPMsgPut();
			WazaTypeMsgPut();
		}
		return;
	}
	if((sys.Trg&D_KEY)==D_KEY){
		if(((WazaCsrPos[ClientNo]&2)==0)&&
			((WazaCsrPos[ClientNo]^0x02)<ClientCsrPosMax)){
			WazaCsrOff(WazaCsrPos[ClientNo]);
			WazaCsrPos[ClientNo]^=0x02;
			SePlay(SE_SELECT);
			WazaCsrOn(WazaCsrPos[ClientNo],0);
			PPMsgPut();
			WazaTypeMsgPut();
		}
		return;
	}
	if((sys.Trg&SELECT_BUTTON)==SELECT_BUTTON){
		if((ClientCsrPosMax>1)&&((FightType&FIGHT_TYPE_TUUSHIN)==0)){
			WazaCsrOn(WazaCsrPos[ClientNo],0x1d);
			if(WazaCsrPos[ClientNo])
				ClientCsrPos2=0;
			else
				ClientCsrPos2=WazaCsrPos[ClientNo]+1;
			WazaCsrOn(ClientCsrPos2,0x1b);
			FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,0x13,0x37,0x13+9,0x3a);
			MsgPutSet(&FightMsg,WazaSwapMsg,CGX_START3+0x10,0x13,0x37);
			MsgPut(&FightMsg);
			ClientSequence[ClientNo] = ClientWazaPosSwap;
		}
		return;
	}

#ifdef PM_DEBUG
	if(DebugMainFlag == 1){
		if((sys.Trg&START_BUTTON)==START_BUTTON){
			FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,1,0x37,0x10,0x37+3);
			MsgPrint(&FightMsg, WazaNameData[
				PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1)],
				CGX_START+0x80, 2, 0x37);
			PM_NumMsgSet(FightMsgBuf, PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],
				ID_waza1), NUM_MODE_ZERO, 3);
			MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16, 10, 0x37);
			MsgPrint(&FightMsg, turn_msg, CGX_START+0x80+16+6, 1, 0x39);
			PM_NumMsgSet(FightMsgBuf, WazaCounter, NUM_MODE_ZERO, 3);
			MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16+6+6, 4, 0x39);
			for(i = 0; i < ACT_MAX; i++){	//���݂̱������𒲂ׂ�
				if(ActWork[i].act_sw)
					count++;
			}
			PM_NumMsgSet(FightMsgBuf, count, NUM_MODE_ZERO, 2);
			MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16+6+6+6, 8, 0x39);
			count = CheckMoveTask();
			PM_NumMsgSet(FightMsgBuf, count, NUM_MODE_ZERO, 2);
			MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16+6+6+6+4, 11, 0x39);
			for(i = 0, count = 0; i < 32; i++){	//�g�pAffine���𒲂ׂ�
				if(AffineWorkUse & (1 << i))
					count++;
			}
			PM_NumMsgSet(FightMsgBuf, count, NUM_MODE_ZERO, 2);
			MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16+6+6+6+4+4, 14, 0x39);
			ClientSequence[ClientNo] = ClientWazaEffectDebug;
		}
	}
#endif
}

#if 0
u8	ClientWazaSelectShinka(u8 *pos)
{
	int	flag=0;			//�`�F�b�N�p�t���O
	u8	ret;

	if((sys.Trg&A_BUTTON)==A_BUTTON){
		SePlay(SE_SELECT);
		ret++;
	}
	if((sys.Trg&B_BUTTON)==B_BUTTON){
		SePlay(SE_SELECT);
		ScrX0 = 0;
		ScrY0 = 0x140;
		ret=0xff;
	}
	if((sys.Trg&L_KEY)==L_KEY){
		if(pos&1){
			WazaCsrOff(pos);
			pos^=0x01;
			SePlay(SE_SELECT);
			WazaCsrOn(pos,0);
		}
	}
	if((sys.Trg&R_KEY)==R_KEY){
		if(((pos&1)==0)&&
			((pos^0x01)<ClientCsrPosMax)){
			WazaCsrOff(pos);
			pos^=0x01;
			SePlay(SE_SELECT);
			WazaCsrOn(pos,0);
		}
	}
	if((sys.Trg&U_KEY)==U_KEY){
		if(pos&2){
			WazaCsrOff(pos);
			pos^=0x02;
			SePlay(SE_SELECT);
			WazaCsrOn(pos,0);
		}
	}
	if((sys.Trg&D_KEY)==D_KEY){
		if(((pos&2)==0)&&
			((pos^0x02)<ClientCsrPosMax)){
			WazaCsrOff(pos);
			pos^=0x02;
			SePlay(SE_SELECT);
			WazaCsrOn(pos,0);
		}
	}
	return ret;
}
#else
u8	ClientWazaSelectShinka(void)
{
	u8	ret=0;

	if((sys.Trg&A_BUTTON)==A_BUTTON){
		SePlay(SE_SELECT);
		ret++;
	}
	if((sys.Trg&B_BUTTON)==B_BUTTON){
		SePlay(SE_SELECT);
		ScrX0 = 0;
		ScrY0 = 0x140;
		ret=0xff;
	}
	if((sys.Trg&L_KEY)==L_KEY){
		if(WazaCsrPos[ClientNo]&1){
			WazaCsrOff(WazaCsrPos[ClientNo]);
			WazaCsrPos[ClientNo]^=0x01;
			SePlay(SE_SELECT);
			WazaCsrOn(WazaCsrPos[ClientNo],0);
		}
	}
	if((sys.Trg&R_KEY)==R_KEY){
		if(((WazaCsrPos[ClientNo]&1)==0)&&
			((WazaCsrPos[ClientNo]^0x01)<ClientCsrPosMax)){
			WazaCsrOff(WazaCsrPos[ClientNo]);
			WazaCsrPos[ClientNo]^=0x01;
			SePlay(SE_SELECT);
			WazaCsrOn(WazaCsrPos[ClientNo],0);
		}
	}
	if((sys.Trg&U_KEY)==U_KEY){
		if(WazaCsrPos[ClientNo]&2){
			WazaCsrOff(WazaCsrPos[ClientNo]);
			WazaCsrPos[ClientNo]^=0x02;
			SePlay(SE_SELECT);
			WazaCsrOn(WazaCsrPos[ClientNo],0);
		}
	}
	if((sys.Trg&D_KEY)==D_KEY){
		if(((WazaCsrPos[ClientNo]&2)==0)&&
			((WazaCsrPos[ClientNo]^0x02)<ClientCsrPosMax)){
			WazaCsrOff(WazaCsrPos[ClientNo]);
			WazaCsrPos[ClientNo]^=0x02;
			SePlay(SE_SELECT);
			WazaCsrOn(WazaCsrPos[ClientNo],0);
		}
	}
	return ret;
}
#endif

void	ClientWazaPosSwap(void)
{
	int	i;
	u8	ppcnt[4];
	u8	ppcnttmp;

	WazaList	*WL;
	WazaList	WL2;

	if(((sys.Trg&A_BUTTON)==A_BUTTON)||
	   ((sys.Trg&SELECT_BUTTON)==SELECT_BUTTON)){
		SePlay(SE_SELECT);
		if(WazaCsrPos[ClientNo]!=ClientCsrPos2){
			WL=(WazaList *)&ServerBuffer[ClientNo][4];
			i=WL->wazano[WazaCsrPos[ClientNo]];
			WL->wazano[WazaCsrPos[ClientNo]]=WL->wazano[ClientCsrPos2];
			WL->wazano[ClientCsrPos2]=i;

			i=WL->pp[WazaCsrPos[ClientNo]];
			WL->pp[WazaCsrPos[ClientNo]]=WL->pp[ClientCsrPos2];
			WL->pp[ClientCsrPos2]=i;

			i=WL->ppmax[WazaCsrPos[ClientNo]];
			WL->ppmax[WazaCsrPos[ClientNo]]=WL->ppmax[ClientCsrPos2];
			WL->ppmax[ClientCsrPos2]=i;

			if(WKC[ClientNo].monomanebit&no2bittbl[WazaCsrPos[ClientNo]]){
				WKC[ClientNo].monomanebit&=
					no2bittbl[WazaCsrPos[ClientNo]]^0xffffffff;
				WKC[ClientNo].monomanebit|=
					no2bittbl[ClientCsrPos2];
			}

			WazaListMake();
			for(i=0;i<4;i++)
				ppcnt[i]=(PSP[ClientNo].pp_count&(0x3<<(i*2)))>>(i*2);

			ppcnttmp=ppcnt[WazaCsrPos[ClientNo]];
			ppcnt[WazaCsrPos[ClientNo]]=ppcnt[ClientCsrPos2];
			ppcnt[ClientCsrPos2]=ppcnttmp;

			ppcnttmp=0;
			for(i=0;i<4;i++)
				ppcnttmp|=(ppcnt[i]<<(i*2));

			PSP[ClientNo].pp_count=ppcnttmp;

			for(i=0;i<4;i++){
				PSP[ClientNo].waza[i]=WL->wazano[i];
				PSP[ClientNo].pp[i]=WL->pp[i];
			}
			if((PSP[ClientNo].condition2&CONDITION2_HENSHIN)==0){
#if 0
				i=WL->wazano[WazaCsrPos[ClientNo]];
				WL->wazano[WazaCsrPos[ClientNo]]=WL->wazano[ClientCsrPos2];
				WL->wazano[ClientCsrPos2]=i;

				i=WL->pp[WazaCsrPos[ClientNo]];
				WL->pp[WazaCsrPos[ClientNo]]=WL->pp[ClientCsrPos2];
				WL->pp[ClientCsrPos2]=i;

				ppcnttmp=PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],
									 ID_pp_count);
				ppcnt=0;
				for(i=0;i<4;i++){
					if((i==WazaCsrPos[ClientNo])&&(i<ClientCsrPos2))
						ppcnt|=(ppcnttmp&(0x3<<(i*2)))<<((ClientCsrPos2-i)*2);
					else if((i==WazaCsrPos[ClientNo])&&(i>ClientCsrPos2))
						ppcnt|=(ppcnttmp&(0x3<<(i*2)))<<((i-ClientCsrPos2)*2);
					else if((i==ClientCsrPos2)&&(i<WazaCsrPos[ClientNo]))
						ppcnt|=(ppcnttmp&(0x3<<(i*2)))<<((WazaCsrPos[ClientNo]-i)*2);
					else if((i==ClientCsrPos2)&&(i>WazaCsrPos[ClientNo]))
						ppcnt|=(ppcnttmp&(0x3<<(i*2)))<<((i-WazaCsrPos[ClientNo])*2);
					else
						ppcnt|=(ppcnttmp&(0x3<<(i*2)));
				}
				for(i=0;i<4;i++){
					WL->wazano[i]=PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],
												ID_waza1+i);
					WL->pp[i]=PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],
											ID_pp1+i);
				}
#endif
				for(i=0;i<4;i++){
					WL2.wazano[i]=PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+i);
					WL2.pp[i]=PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_pp1+i);
				}
				ppcnttmp=PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_pp_count);
				for(i=0;i<4;i++)
					ppcnt[i]=(ppcnttmp&(0x3<<(i*2)))>>(i*2);

				i=WL2.wazano[WazaCsrPos[ClientNo]];
				WL2.wazano[WazaCsrPos[ClientNo]]=WL2.wazano[ClientCsrPos2];
				WL2.wazano[ClientCsrPos2]=i;

				i=WL2.pp[WazaCsrPos[ClientNo]];
				WL2.pp[WazaCsrPos[ClientNo]]=WL2.pp[ClientCsrPos2];
				WL2.pp[ClientCsrPos2]=i;

				ppcnttmp=ppcnt[WazaCsrPos[ClientNo]];
				ppcnt[WazaCsrPos[ClientNo]]=ppcnt[ClientCsrPos2];
				ppcnt[ClientCsrPos2]=ppcnttmp;

				ppcnttmp=0;
				for(i=0;i<4;i++)
					ppcnttmp|=(ppcnt[i]<<(i*2));

				for(i=0;i<4;i++){
					PokeParaPut(&PokeParaMine[SelMonsNo[ClientNo]],
								ID_waza1+i,
								(u8 *)&WL2.wazano[i]);
					PokeParaPut(&PokeParaMine[SelMonsNo[ClientNo]],
								ID_pp1+i,
								(u8 *)&WL2.pp[i]);
				}
				PokeParaPut(&PokeParaMine[SelMonsNo[ClientNo]],
							ID_pp_count,
							(u8 *)&ppcnttmp);
			}
		}
		ClientSequence[ClientNo] = ClientWazaSelect;
		WazaCsrPos[ClientNo]=ClientCsrPos2;
		WazaCsrOn(WazaCsrPos[ClientNo],0);
		FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,0x13,0x37,0x13+9,0x3a);
		MsgPutSet(&FightMsg,WazaInfoMsg,CGX_START3+0x10,0x13,0x37);
		MsgPut(&FightMsg);
		PPMsgPut();
		WazaTypeMsgPut();
	}
	if(((sys.Trg&SELECT_BUTTON)==SELECT_BUTTON)||
	   ((sys.Trg&B_BUTTON)==B_BUTTON)){
		SePlay(SE_SELECT);
		WazaCsrOff(ClientCsrPos2);
		WazaCsrOn(WazaCsrPos[ClientNo],0);
		ClientSequence[ClientNo] = ClientWazaSelect;
		FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,0x13,0x37,0x13+9,0x3a);
		MsgPutSet(&FightMsg,WazaInfoMsg,CGX_START3+0x10,0x13,0x37);
		MsgPut(&FightMsg);
		PPMsgPut();
		WazaTypeMsgPut();
	}
	if((sys.Trg&L_KEY)==L_KEY){
		if(ClientCsrPos2&1){
			if(ClientCsrPos2==WazaCsrPos[ClientNo])
				WazaCsrOn(WazaCsrPos[ClientNo],0x1d);
			else
				WazaCsrOff(ClientCsrPos2);
			ClientCsrPos2^=0x01;
			SePlay(SE_SELECT);
			if(ClientCsrPos2==WazaCsrPos[ClientNo])
				WazaCsrOn(ClientCsrPos2,0);
			else
				WazaCsrOn(ClientCsrPos2,0x1b);
		}
	}
	if((sys.Trg&R_KEY)==R_KEY){
		if(((ClientCsrPos2&1)==0)&&
		   ((ClientCsrPos2^0x01)<ClientCsrPosMax)){
			if(ClientCsrPos2==WazaCsrPos[ClientNo])
				WazaCsrOn(WazaCsrPos[ClientNo],0x1d);
			else
				WazaCsrOff(ClientCsrPos2);
			ClientCsrPos2^=0x01;
			SePlay(SE_SELECT);
			if(ClientCsrPos2==WazaCsrPos[ClientNo])
				WazaCsrOn(ClientCsrPos2,0);
			else
				WazaCsrOn(ClientCsrPos2,0x1b);
		}
	}
	if((sys.Trg&U_KEY)==U_KEY){
		if(ClientCsrPos2&2){
			if(ClientCsrPos2==WazaCsrPos[ClientNo])
				WazaCsrOn(WazaCsrPos[ClientNo],0x1d);
			else
				WazaCsrOff(ClientCsrPos2);
			ClientCsrPos2^=0x02;
			SePlay(SE_SELECT);
			if(ClientCsrPos2==WazaCsrPos[ClientNo])
				WazaCsrOn(ClientCsrPos2,0);
			else
				WazaCsrOn(ClientCsrPos2,0x1b);
		}
	}
	if((sys.Trg&D_KEY)==D_KEY){
		if(((ClientCsrPos2&2)==0)&&
		   ((ClientCsrPos2^0x02)<ClientCsrPosMax)){
			if(ClientCsrPos2==WazaCsrPos[ClientNo])
				WazaCsrOn(WazaCsrPos[ClientNo],0x1d);
			else
				WazaCsrOff(ClientCsrPos2);
			ClientCsrPos2^=0x02;
			SePlay(SE_SELECT);
			if(ClientCsrPos2==WazaCsrPos[ClientNo])
				WazaCsrOn(ClientCsrPos2,0);
			else
				WazaCsrOn(ClientCsrPos2,0x1b);
		}
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
//	�Z�G�t�F�N�g�����邽�߂̃f�o�b�N
//----------------------------------------------------------
#ifdef PM_DEBUG
static void ClientWazaEffectDebug(void)
{
	s16 wazano;
	u8 i;
	
	wazano = PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]], ID_waza1);
	switch(sys.Repeat)
	{
		case START_BUTTON:
			SelectMotionStop(ClientNo, MOTION_GAUGE);
			SelectMotionStop(ClientNo, MOTION_POKE);
			AttackClient = ClientNo;
			if((sys.RealCont & B_BUTTON) && (FightType&FIGHT_TYPE_2vs2))
				DefenceClient = ClientNo ^ 2;
			else if((sys.RealCont & A_BUTTON) && (FightType&FIGHT_TYPE_2vs2))
				DefenceClient = ClientNoGet(BSS_CLIENT_ENEMY2);
			else
				DefenceClient = ClientNoGet(BSS_CLIENT_ENEMY);
			PokeAffineON_OFF(0);
			WazaEffectSet(wazano);
			ClientSequence[ClientNo] = DebugWazaEffWait;
			break;
		case SELECT_BUTTON:
			SelectMotionStop(ClientNo, MOTION_GAUGE);
			SelectMotionStop(ClientNo, MOTION_POKE);
			DefenceClient = ClientNo;
			if((sys.RealCont & B_BUTTON) && (FightType&FIGHT_TYPE_2vs2))
				AttackClient = ClientNo ^ 2;
			else if((sys.RealCont & A_BUTTON) && (FightType&FIGHT_TYPE_2vs2))
				AttackClient = ClientNoGet(BSS_CLIENT_ENEMY2);
			else
				AttackClient = ClientNoGet(BSS_CLIENT_ENEMY);
			PokeAffineON_OFF(0);
			WazaEffectSet(wazano);
			ClientSequence[ClientNo] = DebugWazaEffWait;
			break;
		case R_BUTTON:
			if((sys.RealCont & B_BUTTON) && (FightType&FIGHT_TYPE_2vs2)){
				SelectMotionStop(ClientNo, MOTION_GAUGE);
				SelectMotionStop(ClientNo, MOTION_POKE);
				AttackClient = ClientNoGet(BSS_CLIENT_ENEMY2);
				DefenceClient = ClientNoGet(BSS_CLIENT_ENEMY);
				PokeAffineON_OFF(0);
				WazaEffectSet(wazano);
				ClientSequence[ClientNo] = DebugWazaEffWait;
				break;
			}
			else
				wazano += 9;
		case R_KEY:
			if(++wazano > WAZANO_MAX)
				wazano = 1;
			PokeParaPut(&PokeParaMine[SelMonsNo[ClientNo]], ID_waza1, (u8*)&wazano);
			PSP[ClientNo].waza[0] = wazano;
		//	FontScrnBoxFill(&FightMsg,SPACE_CHR_B,2,0x37,8,0x37+1);
			FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,1,0x37,0x10,0x37+1);
			MsgPrint(&FightMsg, WazaNameData[wazano],
				CGX_START+0x80, 2, 0x37);
			PM_NumMsgSet(FightMsgBuf, wazano, NUM_MODE_ZERO, 3);
			MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16, 10, 0x37);
			break;
		case L_BUTTON:
			if((sys.RealCont & B_BUTTON) && (FightType&FIGHT_TYPE_2vs2)){
				SelectMotionStop(ClientNo, MOTION_GAUGE);
				SelectMotionStop(ClientNo, MOTION_POKE);
				AttackClient = ClientNoGet(BSS_CLIENT_ENEMY);
				DefenceClient = ClientNoGet(BSS_CLIENT_ENEMY2);
				PokeAffineON_OFF(0);
				WazaEffectSet(wazano);
				ClientSequence[ClientNo] = DebugWazaEffWait;
				break;
			}
			else
				wazano -= 9;
		case L_KEY:
			if(--wazano <= 0)
				wazano = WAZANO_MAX;
			PokeParaPut(&PokeParaMine[SelMonsNo[ClientNo]], ID_waza1, (u8*)&wazano);
			PSP[ClientNo].waza[0] = wazano;
			FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,1,0x37,0x10,0x37+1);
			MsgPrint(&FightMsg, WazaNameData[wazano],
				CGX_START+0x80, 2, 0x37);
			PM_NumMsgSet(FightMsgBuf, wazano, NUM_MODE_ZERO, 3);
			MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16, 10, 0x37);
			break;
		/*
		case B_BUTTON:
			for(i=0;i<4;i++){
				PM_strcpy((u8 *)&FightMsgBuf,ColBlueMsg);
				PM_strcat((u8 *)&FightMsgBuf,
						WazaNameData[
						PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+i)]);
				MsgPutSet(
				&FightMsg,
				FightMsgBuf,
				CGX_START+0x80+i*16,
				((i&1)?0x0a:2),
				((i<2)?0x37:0x39));
				MsgPut(&FightMsg);
			}
			ClientSequence[ClientNo] = ClientWazaSelect;
			break;
		*/
		case U_KEY:
		case D_KEY:
			if(sys.Repeat == U_KEY)
				WazaCounter--;
			else
				WazaCounter++;
			PM_NumMsgSet(FightMsgBuf, WazaCounter, NUM_MODE_ZERO, 3);
			MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16+6+6, 4, 0x39);
			break;
	}
	
	if((sys.RealCont & L_BUTTON) && (sys.RealCont & R_BUTTON)){
		for(i=0;i<4;i++){
			PM_strcpy((u8 *)&FightMsgBuf,ColBlueMsg);
			PM_strcat((u8 *)&FightMsgBuf,
					WazaNameData[
					PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+i)]);
			MsgPutSet(
			&FightMsg,
			FightMsgBuf,
			CGX_START+0x80+i*16,
			((i&1)?0x0a:2),
			((i<2)?0x37:0x39));
			MsgPut(&FightMsg);
		}
		ClientSequence[ClientNo] = ClientWazaSelect;
	}
}

static void DebugWazaEffWait(void)
{
	int i;
	u8 count = 0;
	
	WazaEffectSequence();
	if(WazaEffectFlag == 0){
		PokeAffineON_OFF(1);
		SelectMotionSet(ClientNo, MOTION_GAUGE,
			GAUGE_MOTION_ADDSEC, GAUGE_MOTION_FURIHABA);
		SelectMotionSet(ClientNo, MOTION_POKE,
			POKE_MOTION_ADDSEC, POKE_MOTION_FURIHABA);

		for(i = 0; i < ACT_MAX; i++){	//���݂̱������𒲂ׂ�
			if(ActWork[i].act_sw)
				count++;
		}
		PM_NumMsgSet(FightMsgBuf, count, NUM_MODE_ZERO, 2);
		MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16+6+6+6, 8, 0x39);
		count = CheckMoveTask();
		PM_NumMsgSet(FightMsgBuf, count, NUM_MODE_ZERO, 2);
		MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16+6+6+6+4, 11, 0x39);
		for(i = 0, count = 0; i < 32; i++){	//�g�pAffine���𒲂ׂ�
			if(AffineWorkUse & (1 << i))
				count++;
		}
		PM_NumMsgSet(FightMsgBuf, count, NUM_MODE_ZERO, 2);
		MsgPrint(&FightMsg, FightMsgBuf, CGX_START+0x80+16+6+6+6+4+4, 14, 0x39);

		ClientSequence[ClientNo] = ClientWazaEffectDebug;
	}
}

#endif	//PM_DEBUG	---------------------------------------------


void ClientTrainerEncountWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientBitReset();
	}
}

void ClientTrainerAppearWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		//DelActor(&ActWork[PokemonStruct[ClientNo]]);
		ClientBitReset();
	}
}

void ClientTrainerReturnWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].move == DummyActMove)
	{
		TrainerBObjCellPalDel(MyData.my_sex);
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);	//�ꉞ(�Ă�ł����͂Ȃ���)
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		ClientBitReset();
	}
}

void ClientSendOutWaitBitReset(void)
{
	if(CliSys[ClientNo].send_out_wait-- == 0){
		CliSys[ClientNo].send_out_wait = 0;
		ClientBitReset();
	}
}

void ClientSendOutGaugeWait(void)
{
#if 0	// 2002.05.02(��)
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove
		&& ActWork[ClientGaugeSt[ClientNo^2]].move == DummyActMove)
	{
		ClientBitReset();
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

		PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
		if(FightCheck())
			PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo^2]], ClientNo^2);
		
		//ClientBitReset();
		CliSys[ClientNo].send_out_wait = 3;
		ClientSequence[ClientNo] = ClientSendOutWaitBitReset;
	}
#endif
}

void ClientPokeSendOutWait(void)
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
		ClientSequence[ClientNo] = ClientSendOutGaugeWait;
//		ClientBitReset();
	}
}

void ClientGaugeAppearWait(void)
{
	if(ActWork[ClientGaugeSt[ClientNo]].move == DummyActMove && CliSys[ClientNo].rare_eff_end){
		CliSys[ClientNo].rare_eff = 0;
		CliSys[ClientNo].rare_eff_end = 0;
		CellDel(WAZACHR_ID_SHOOTINGSTAR);
		ObjPalRegNumDel(WAZACHR_ID_SHOOTINGSTAR);

		//�g����`�F�b�N
		if(Climem[ClientNo].migawari)
			SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
		ClientSequence[ClientNo] = ClientAppearMigawariEffWait;
	}
}

static void ClientAppearMigawariEffWait(void)
{
	if(CliSys[ClientNo].etc_eff == 0){
		AddTask(VoiceCheckTask, 10);
		PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
		ClientBitReset();
	}
}

void ClientPokeAppearWait(void)
{
	if(CliSys[ClientNo].ball_eff_wait == 0 && CliSys[ClientNo].rare_eff == 0)
		RareEffectCheckSet(ClientNo, &PokeParaMine[SelMonsNo[ClientNo]]);
	
	if(ActWork[ClientStruct[ClientNo]].move == DummyActMove && CliSys[ClientNo].ball_eff_wait == 0)
	{
		DelActor(&ActWork[ClientStruct[ClientNo]]);					//�̪�ĊĎ�����DEL
		GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaMine[SelMonsNo[ClientNo]], G_ALL);
		GaugeAppearEffect(ClientNo);
		GaugeON(ClientGaugeSt[ClientNo]);
		//PokeBanishMemory();
		PokeClientBanishMemory(ClientNo);
		ClientSequence[ClientNo] = ClientGaugeAppearWait;
//		VoicePlay(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno),-25);
//		ClientBitReset();
	}
}

//----------------------------------------------------------
//	�|�P�����̖������I�����Ă����牺���Ă������ʂ�������
// �ײ��ļ��ݽ�Ƃ͊֌W�Ȃ��Ɨ����Ă���^�X�N
//----------------------------------------------------------
void VoiceCheckTask(u8 id)
{
	if(VoiceEndCheck() == 0){
		m4aMPlayVolumeControl(&m4a_mplay000,0xffff,256);
		DelTask(id);
	}
}

void ClientHPGaugeWait(void)
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
		ClientBitReset();
	}
}

void ClientEXPGaugeEnd(void)
{
	if(FightMsg.status == MSG_END)
		ClientBitReset();
}

static void ClientEXPGetMsgWaitSeq(u8 id)
{
	u8 client_no, temotino, level;
	u32 myexp, nextexp;
	s16 beEXP;
	u8 temp;
	u16 monsno;
	
	temotino = TaskTable[id].work[0];
	client_no = TaskTable[id].work[2];
	beEXP = TaskTable[id].work[1];

	if(FightCheck() == 1 || temotino != SelMonsNo[client_no])
	{	//EXP�ް�ނ�\������K�v���Ȃ��ꍇ(���A�퓬�ɏo�ĂȂ��B2��2�̎�)
		monsno = PokeParaGet(&PokeParaMine[temotino], ID_monsno);
		level = PokeParaGet(&PokeParaMine[temotino], ID_level);
		myexp = PokeParaGet(&PokeParaMine[temotino], ID_exp);
		nextexp = GrowTbl[PPD[monsno].grow][level+1];
	
		if(myexp + beEXP >= nextexp)	//���ٱ�������
		{
			PokeParaPut(&PokeParaMine[temotino], ID_exp, (u8*)&nextexp);
			level++;
//			PokeParaPut(&PokeParaMine[temotino], ID_level, &level);
			PokeParaCalc(&PokeParaMine[temotino]);
			beEXP -= nextexp - myexp;	//�����ٌJ��グEXP
			temp = ClientNo;
			ClientNo = client_no;
			BSS_SELECT_RETURN_SET(BSS_CLIENT, BSS_SELECT_RETURN_LEVELUP, beEXP);
			ClientNo = temp;
			
			if(FightCheck() == 1 && (temotino == SelMonsNo[client_no] 
					|| temotino == SelMonsNo[client_no^2]))
				TaskTable[id].TaskAdrs = ClientLvUpEffect;
			else
				TaskTable[id].TaskAdrs = ClientLvUpMsgInitSeq;
		}
		else
		{
			myexp += beEXP;
			PokeParaPut(&PokeParaMine[temotino], ID_exp, (u8*)&myexp);
			//BSS_SELECT_RETURN_SET(BSS_CLIENT, BSS_SELECT_RETURN_LEVELUP, 0);
			ClientSequence[client_no] = ClientEXPGaugeEnd;
			DelTask(id);
		}
	}
	else
		TaskTable[id].TaskAdrs = ClientEXPGaugeInitSeq;
}

static void ClientEXPGaugeInitSeq(u8 id)
{
	u8 temotino, client_no;
//	s16 beEXP;
//	u16 nowEXP, nextEXP;
	s32 beEXP, nowEXP, nextEXP;
	u8 pokelevel;
	u16 monsno;
	
	temotino = TaskTable[id].work[0];
	beEXP = TaskTable[id].work[1];
	client_no = TaskTable[id].work[2];
	pokelevel = PokeParaGet(&PokeParaMine[temotino], ID_level);
	monsno = PokeParaGet(&PokeParaMine[temotino], ID_monsno);
	
	nowEXP = PokeParaGet(&PokeParaMine[temotino],ID_exp)
							- GrowTbl[PPD[monsno].grow][pokelevel];
	nextEXP = GrowTbl[PPD[monsno].grow][pokelevel + 1] - GrowTbl[PPD[monsno].grow][pokelevel];
	
	GaugeDataSet(client_no, ClientGaugeSt[client_no], nextEXP, nowEXP, -beEXP);
//	if(nowEXP + beEXP >= nextEXP)
//		TaskTable[id].work[1] = (nowEXP + beEXP) - nextEXP;
	
	SePlay(SE_EXP);
	TaskTable[id].TaskAdrs = ClientEXPGaugeWaitSeq;
}

static void ClientEXPGaugeWaitSeq(u8 id)	//��)������ݽ
{
	s16 work;
	u8 client_no,  level, temp;
	s32 myexp, nextexp;
	s16 beEXP;
	u8 temotino;
	u16 monsno;
	
	if(TaskTable[id].work[10] < 13)
	{
		TaskTable[id].work[10]++;
		return;
	}
	
	temotino = TaskTable[id].work[0];
	beEXP = TaskTable[id].work[1];
	client_no = TaskTable[id].work[2];
	work = GaugeOBJ(client_no, ClientGaugeSt[client_no], 1, 0);
	GaugeON(ClientGaugeSt[client_no]);
	if(work != -1)
	{
		//���l�\��
	}
	else
	{
		m4aSongNumStop(SE_EXP);
		level = PokeParaGet(&PokeParaMine[temotino], ID_level);
		myexp = PokeParaGet(&PokeParaMine[temotino], ID_exp);
		monsno = PokeParaGet(&PokeParaMine[temotino], ID_monsno);
		nextexp = GrowTbl[PPD[monsno].grow][level+1];
		
		if(myexp + beEXP >= nextexp)
		{
			PokeParaPut(&PokeParaMine[temotino], ID_exp, (u8*)&nextexp);
//			level++;
//			PokeParaPut(&PokeParaMine[temotino], ID_level, &level);
			PokeParaCalc(&PokeParaMine[temotino]);
			beEXP -= nextexp - myexp;	//�����ٌJ��グEXP
			temp = ClientNo;
			ClientNo = client_no;
			BSS_SELECT_RETURN_SET(BSS_CLIENT, BSS_SELECT_RETURN_LEVELUP, beEXP);
			ClientNo = temp;
			TaskTable[id].TaskAdrs = ClientLvUpEffect;
		}
		else
		{
			myexp += beEXP;
			PokeParaPut(&PokeParaMine[temotino], ID_exp, (u8*)&myexp);
			//BSS_SELECT_RETURN_SET(BSS_CLIENT, BSS_SELECT_RETURN_LEVELUP, 0);
			ClientSequence[client_no] = ClientEXPGaugeEnd;
			DelTask(id);
		}
	}
}

#if 0	//���x���A�b�v�G�t�F�N�g���쐬 2002.07.04(��)
static void ClientLvUpEffect(u8 id)
{
	u8 temotino, client_no;
	u8 level;
	
	temotino = TaskTable[id].work[0];
	client_no = TaskTable[id].work[2];
	level = PokeParaGet(&PokeParaMine[temotino], ID_level);

	GaugeTransChrSet(ClientGaugeSt[client_no], &PokeParaMine[temotino], G_ALL);
	TaskTable[id].TaskAdrs = ClientLvUpMsgInitSeq;
}
#else
//-- ���x���A�b�v�G�t�F�N�g --//
static void ClientLvUpEffect(u8 id)
{
	u8 client_no;
	u8 temotino;
	
	client_no = TaskTable[id].work[2];
	temotino = TaskTable[id].work[0];
	
	if(FightCheck() == 1 && temotino == SelMonsNo[client_no^2])
		client_no ^= 2;	//���������ٱ��߂������́A������ClientNo�ɕύX
	
	SetEtcEffect(client_no, client_no, client_no, FE_ID_LVUP);
	TaskTable[id].TaskAdrs = ClientLvUpEffectWait;
}

static void ClientLvUpEffectWait(u8 id)
{
	u8 temotino, client_no;
	u8 level;
	
	client_no = TaskTable[id].work[2];
	if(CliSys[client_no].etc_eff)
		return;

	temotino = TaskTable[id].work[0];
	level = PokeParaGet(&PokeParaMine[temotino], ID_level);

	if(FightCheck() == 1 && temotino == SelMonsNo[client_no^2])
		//���������ٱ��߂������́A������ClientNo�ɕύX
		GaugeTransChrSet(ClientGaugeSt[client_no^2], &PokeParaMine[temotino], G_ALL);
	else
		GaugeTransChrSet(ClientGaugeSt[client_no], &PokeParaMine[temotino], G_ALL);
	
	TaskTable[id].TaskAdrs = ClientLvUpMsgInitSeq;
}
#endif

static void ClientLvUpMsgInitSeq(u8 id)
{
	u8 client_no, temotino;
	s16 beEXP;
	u8 level;
//	u32 myexp;
//	u8 i;
	
	temotino = TaskTable[id].work[0];
	client_no = TaskTable[id].work[2];
	beEXP = TaskTable[id].work[1];
	level = PokeParaGet(&PokeParaMine[temotino], ID_level);
#if 1
	client_no = TaskTable[id].work[2];
	ClientSequence[client_no] = ClientEXPGaugeEnd;
	DelTask(id);
#else
	if(SelMonsNo[client_no] == temotino)
	{
		PM_NumMsgSet(StrTempBuffer1, level, NUM_MODE_LEFT, 3);
		PM_MsgExpand(MsgExpandBuffer, LevelUp_msg);
		TalkMsgSet(&FightMsg, MsgExpandBuffer, CGX_START+0x10,2,15);
		TaskTable[id].TaskAdrs = WazaOboeInitSeq;
	}
	else
	{
		myexp = PokeParaGet(&PokeParaMine[temotino], ID_exp);
		for(i = 1; myexp + beEXP >= GrowTbl[PPD[PokeParaGet(&PokeParaMine[temotino], ID_monsno].grow][level+i]; i++)
			;
		i--;
		PokeParaGet(&PokeParaMine[temotino], ID_nickname, StrTempBuffer0);
		NickNameCutOff(StrTempBuffer0);
		PM_NumMsgSet(StrTempBuffer1, level+i, NUM_MODE_LEFT, 3);
		PM_MsgExpand(MsgExpandBuffer, LevelUp_msg);
		TalkMsgSet(&FightMsg, MsgExpandBuffer, CGX_START+0x10,2,15);
		TaskTable[id].TaskAdrs = WazaOboeInitSeq;
	}
#endif
}

#if 0
static void WazaOboeInitSeq(u8 id)
{
//	u8 temotino;
	u8 client_no;
//	u8 level;
//	u32 exp;
	u16 monsno;
	
	if(FightMsg.status == MSG_END)
	{
		//�̪�Ă��I��������Z�o���������ݽ�Ăяo��
		//-- ���͂��̼��ݽ���Ȃ��̂Ŏ蓮�ő��₵�Ƃ� --//
		temotino = TaskTable[id].work[0];
		client_no = TaskTable[id].work[2];
		monsno = PokeParaGet(&PokeParaMine[temotino], ID_monsno);
		level = PokeParaGet(&PokeParaMine[temotino], ID_level);
		level++;
		exp = GrowTbl[PPD[monsno].grow][level];
		PokeParaPut(&PokeParaMine[temotino], ID_exp, (u8*)&exp);
		//PokeParaPut(&PokeParaMine[temotino], ID_level, &level);
		//-- �����܂� --//
	
		TaskTable[id].TaskAdrs = WazaOboeWaitSeq;
	}
}

static void WazaOboeWaitSeq(u8 id)
{
	s16 beEXP;
	u8 temotino, level, client_no;
	u16 exp;
	u16 monsno;
	
	temotino = TaskTable[id].work[0];
	beEXP = TaskTable[id].work[1];
	client_no = TaskTable[id].work[2];
	level = PokeParaGet(&PokeParaMine[temotino], ID_level);
	monsno = PokeParaGet(&PokeParaMine[temotino], ID_monsno);
	exp = GrowTbl[PPD[monsno].grow][level] - GrowTbl[PPD[monsno].grow][level-1];
	beEXP -= exp;
	TaskTable[id].work[1] = beEXP;

	if(SelMonsNo[client_no] == temotino)
		TaskTable[id].TaskAdrs = ClientEXPGaugeInitSeq;
	else
	{
		ClientSequence[client_no] = ClientEXPGaugeEnd;
		DelTask(id);
	}
}
#endif

void ClientPokeKizetuWait(void)
{
	if(ActWork[PokemonStruct[ClientNo]].y + ActWork[PokemonStruct[ClientNo]].dy > 160)
	{
//		PokemonBObjCellPalDel(PokeParaMine[SelMonsNo[ClientNo]].PPP.monsno);
//		changed by soga 2001.08.29
		PokemonBObjCellPalDel(
		PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno));
		AffineWorkNumDel(ActWork[PokemonStruct[ClientNo]].oamData.AffineParamNo);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientBitReset();
	}
}

void ClientPokeReturnWait(void)
{
#if 0	// 2002.07.09(��)	++++++++++++++++++++++++++++++
//	if(ActWork[ClientStruct].move == DummyActMove	//�� //anmend_sw == 1 
//						&& ActWork[PokemonStruct[ClientNo]].affend_sw == 1)	//�߹��
	if(ActWork[PokemonStruct[ClientNo]].affend_sw == 1)
	{
		//�߹��DEL
		//PokemonBObjCellPalDel(PokeParaMine[SelMonsNo[ClientNo]].PPP.monsno);
//		PalDelActor(&ActWork[PokemonStruct[ClientNo]]);
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		//���Ď�����Del
		//DelActor(&ActWork[ClientStruct]);
		
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientBitReset();
	}
	else
		ActWork[PokemonStruct[ClientNo]].dy += 2;

#else	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if(CliSys[ClientNo].etc_eff == 0)
	{
		//�߹��DEL
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientBitReset();
	}
#endif
}

void	ClientMessageWait(void)
{
	if(FightMsg.status==MSG_END){
//		ClientBit&=(no2bittbl[ClientNo]^0xffffffff);
//		ClientSequence[ClientNo]=ClientCommandWait;
		ClientBitReset();
	}
}

#if 0	// 2002.07.22(��)
void	ClientWazaEffectWait(void)
{
	WazaEffectSequence();
	if(WazaEffectFlag==0){
		//�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
		PokeBanishMemory();
		ClientBitReset();
	}
}
#elif 0
void	ClientWazaEffectWait(void)
{
	WazaEffectSequence();
	if(WazaEffectFlag==0){
		if(Climem[ClientNo].migawari)
			ClientSequence[ClientNo] = ClientMigawariSlidInEffect;
		else
			ClientSequence[ClientNo] = ClientWazaEffectEnd;
	}
}

static void ClientWazaEffectEnd(void)
{
	u16 wazano;
	
	//�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
	PokeBanishMemory();
	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	MigawariFlgCheckSet(ClientNo, wazano);
	ClientBitReset();
}
#endif

//-- �߹��ؽČĂяo�� --//
void ClientPokeSelectWait(void)
{
	u8 list_type;
	
	if(FadeData.fade_sw == 0)
	{
		ClientSequence[ClientNo] = ClientPokeListReturnWait;
		list_type = TaskTable[ClientStruct[ClientNo]].work[0];
		DelTask(ClientStruct[ClientNo]);
		BattlePokeListInit(list_type);
	}
}

//-- �߹��ؽĉ�ʂ��畜�A��̼��ݽ --//
static void ClientPokeListReturnWait(void)
{
//	if(FadeData.fade_sw == 0)
	if((sys.pMainProc==MainFight)&&
	   (FadeData.fade_sw == 0))
	{
		switch(ListStatus)
		{
			case P_CHANGE:
//				SelMonsNo[ClientNo] = ListBuffer;
				BSS_POKE_RESHUFFLE_SET(BSS_CLIENT,ListBuffer,ListRow);
				break;
			default:
				BSS_POKE_RESHUFFLE_SET(BSS_CLIENT,6,0);	//B��ݾ�
				break;
		}
//		PokeSelFlag=0;
		if((ServerBuffer[ClientNo][1]&0x0f)==PLT_B_USELESS)				//B�L�����Z���֎~
			TuushinTaikiCheck();
		ClientBitReset();
	}
}

void ClientItemWait(void)
{
	if(FadeData.fade_sw == 0)
	{
		ClientSequence[ClientNo] = ClientItemReturnWait;
		PushBattleWork();
		BtlRuckInit();
	}
}

void ClientItemReturnWait(void)
{
	if((sys.pMainProc==MainFight)&&
	   (FadeData.fade_sw == 0))
	{
		BSS_USE_ITEM_SET(BSS_CLIENT, ReturnItemNo);
		ClientBitReset();
	}
}

#if 0	//---------- �ۗ� ---------------//
void ClientPokeResuffleWait(void)
{
	u8 client_no;
	
	if(ActWork[PokemonStruct[ClientNo]].affend_sw == 1)
	{
		client_no = ActWork[PokemonStruct[ClientNo]].work[0];
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		PalDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		
		GaugeOFF(ClientGaugeSt[client_no]);
		SelMonsNo[client_no] = ListBuffer;
		
		ClientStruct = AddBanishActor(CA_PokeAppearWait);
		PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[client_no]], client_no);
		ActWork[ClientStruct].work[0] = MBSmokeSet(C_POKE1X, C_POKE1Y+20, 1);
		PokemonStruct[ClientNo] = AddActor(&PM2BTbl[PokeParaMine[SelMonsNo[client_no]].PPP.monsno],
						C_POKE1X,
						C_POKE1Y+4*(8-pm2bpara[PokeParaMine
								[SelMonsNo[client_no]].PPP.monsno].size),
						C_POKE1PRI);	//�߹��
		ActWork[PokemonStruct[ClientNo]].work[0] = client_no;
		
		CAC_PokeAppearDifferentInit(&ActWork[PokemonStruct[ClientNo]]);
		ClientSequence[client_no] = ClientPokeAppearWait;

		GaugeTransChrSet(ClientGaugeSt[client_no],
					&PokeParaMine[SelMonsNo[client_no]], G_ALL);

		ClientBitReset();
	}
	else
		ActWork[PokemonStruct[ClientNo]].dy += 2;
}
#endif	//----------------------------//



void ClientBallEffectWait(void)
{
#if 0	// 2002.07.18(��)
//	if(ActWork[ClientStruct[ClientNo]].act_sw == 0)
	if(ClientEffectWait == 0)
//	if(CliSys[ClientNo].ball_eff_wait == 0)
	{
//		AllDelActor(&ActWork[ClientStruct[ClientNo]]);
		ClientBitReset();
	}
#else
	//if(CliSys[ClientNo].etc_eff == 0)
	if(ClientEffectWait == 0 || CliSys[ClientNo].etc_eff == 0)
		ClientBitReset();
#endif
}

void ClientPokeDamageBlinkWait(void)
{
	u8 pokeact;
	
	pokeact = PokemonStruct[ClientNo];
	if(ActWork[pokeact].work[1] == DAMAGE_BLINK_WAIT)
	{
		ActWork[pokeact].work[1] = 0;
		ActWork[pokeact].banish = 0;
		ClientEffectWait = 0;	//Blink�̪�āA�t���O��ؾ��
		ClientBitReset();
	}
	else
	{
		if(ActWork[pokeact].work[1] % BLINK_INTERVAL == 0)
			ActWork[pokeact].banish ^= 1;
		ActWork[pokeact].work[1]++;
	}
}


//=========================================================================
//	�T�u���[�`��
//=========================================================================

void	WazaListMake()
{
	int	i;
	WazaList	*WL;

	WL=(WazaList *)&ServerBuffer[ClientNo][4];

	ClientCsrPosMax=0;

	FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,1,0x37,0x10,0x37+3);

	for(i=0;i<4;i++){
		WazaCsrOff(i);
		PM_strcpy((u8 *)&FightMsgBuf,ColBlueMsg);
		PM_strcat((u8 *)&FightMsgBuf,
//				WazaNameData[
//				PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+i)]);
//				WazaNameData[PSP[ClientNo].waza[i]]);
				WazaNameData[WL->wazano[i]]);
		MsgPutSet(
		&FightMsg,
		FightMsgBuf,
		CGX_START3+0x80+i*16,
		((i&1)?0x0a:2),
		((i<2)?0x37:0x39));
		MsgPut(&FightMsg);
//		if(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+i)!=0) ClientCsrPosMax++;
//		if((PSP[ClientNo].waza[i])!=0) ClientCsrPosMax++;
		if(WL->wazano[i]!=0) ClientCsrPosMax++;
	}
}

void	PPMsgPut()
{
	WazaList	*WL;
	u8	*adr;

	if(ServerBuffer[ClientNo][2]==BSS_WAZA_SELECT_WAZAWASURE) return;
	WL=(WazaList *)&ServerBuffer[ClientNo][4];
	adr=PM_strcpy((u8 *)&FightMsgBuf,ColBlueMsg);
	adr=PM_NumMsgSet(adr,
//					PokeParaMine[SelMonsNo[ClientNo]].PPP.pp[ClientCsrPos],
//					changed by soga 2001.08.29
//					PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_pp1+ClientCsrPos),
//					PSP[ClientNo].pp[ClientCsrPos],
					WL->pp[WazaCsrPos[ClientNo]],
					NUM_MODE_SPACE,2);
	*adr=sura_;
	adr++;
	PM_NumMsgSet(adr,
//				 WTD[PokeParaMine[SelMonsNo[ClientNo]].PPP.waza[ClientCsrPos]].pp,
//					changed by soga 2001.08.29
//				 WTD[
//				 PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+ClientCsrPos)].pp,
//				 PPMaxGet(PSP[ClientNo].waza[ClientCsrPos],PSP[ClientNo].pp_count,ClientCsrPos),
				 WL->ppmax[WazaCsrPos[ClientNo]],
				 NUM_MODE_SPACE,2);
//	MsgPutSet(&FightMsg,FightMsgBuf,0x20,0x18,0x37);
	MsgPutSet(&FightMsg,FightMsgBuf,CGX_START3+0x22,0x18,0x37);
	MsgPut(&FightMsg);
}

void	WazaTypeMsgPut()
{
	WazaList	*WL;

	if(ServerBuffer[ClientNo][2]!=BSS_WAZA_SELECT_WAZAWASURE){
		WL=(WazaList *)&ServerBuffer[ClientNo][4];
		FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,0x19,0x39,0x19+3,0x3a);

		PM_strcpy((u8 *)&FightMsgBuf,ColBlueMsg);
		PM_strcat((u8 *)&FightMsgBuf,
//				  ZokuseiMsg[WTD[
//				  PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_waza1+ClientCsrPos)].wazatype]);
//				  ZokuseiMsg[WTD[PSP[ClientNo].waza[ClientCsrPos]].wazatype]);
				  ZokuseiMsg[WTD[WL->wazano[WazaCsrPos[ClientNo]]].wazatype]);

		MsgPutSet(&FightMsg,FightMsgBuf,CGX_START3+0x40,0x19,0x39);
	}
	else{
		FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_B,0x13,0x37,0x13+9,0x3a);
		MsgPutSet(&FightMsg,WasureMsg,CGX_START3+0x10,0x13,0x37);
	}
	MsgPut(&FightMsg);
}

void	WazaCsrOn(u8 pos,u8 ofs)
{
	*(vu16 *)((BG_VRAM+CSR_SCR*0x800+((pos&1)?9*2:1*2)+
	((pos<2)?0x37*0x40:0x39*0x40)))=CSR_CHR_U+ofs;
	*(vu16 *)((BG_VRAM+CSR_SCR*0x800+((pos&1)?9*2:1*2)+
	((pos<2)?0x37*0x40:0x39*0x40)+0x40))=CSR_CHR_D+ofs;
}

void	WazaCsrOff(u8 pos)
{
//	*(vu16 *)((BG_VRAM+CSR_SCR*0x800+3*2+(0x34+pos*2)*0x40)-0x40)=SPACE_CHR_B;
//	*(vu16 *)(BG_VRAM+CSR_SCR*0x800+3*2+(0x34+pos*2)*0x40)=SPACE_CHR_B;
	*(vu16 *)((BG_VRAM+CSR_SCR*0x800+((pos&1)?9*2:1*2)+
	((pos<2)?0x37*0x40:0x39*0x40)))=SPACE_CHR_B;
	*(vu16 *)((BG_VRAM+CSR_SCR*0x800+((pos&1)?9*2:1*2)+
	((pos<2)?0x37*0x40:0x39*0x40)+0x40))=SPACE_CHR_B;
}

void	CommandCsrOn(u8 pos,u8 ofs)
{
	*(vu16 *)((BG_VRAM+CSR_SCR*0x800+0x12*2+(pos&1)*0x0c+
	(0x24+(pos&2))*0x40)-0x40)=CSR_CHR_U+ofs;
	*(vu16 *)(BG_VRAM+CSR_SCR*0x800+0x12*2+(pos&1)*0x0c+
	(0x24+(pos&2))*0x40)=CSR_CHR_D+ofs;
}

void	CommandCsrOff(u8 pos)
{
	*(vu16 *)((BG_VRAM+CSR_SCR*0x800+0x12*2+(pos&1)*0x0c+
	(0x24+(pos&2))*0x40)-0x40)=SPACE_CHR_B;
	*(vu16 *)(BG_VRAM+CSR_SCR*0x800+0x12*2+(pos&1)*0x0c+
	(0x24+(pos&2))*0x40)=SPACE_CHR_B;
}

void ClientPokeListReturn(void)
{
//	FadeData.trans_stop = 1;
	MainProcChange(BattleRecover);
#if 0	//BattleRecover���ł��悤�ɕύX����
	BattleRecover();
//	if((FightType&FIGHT_TYPE_TUUSHIN)==0)
		PaletteFadeReq(0xffffffff, 0, 16, 0, 0);
		//LightFadeReq(B_to_N);
	FadeData.trans_stop = 0;
#endif
}

void BtlItemReturn(void)
{
//	FadeData.trans_stop = 1;
	MainProcChange(BattleRecover);
#if 0	//BattleRecover���ł��悤�ɕύX����
	BattleRecover();
	PaletteFadeReq(0xffffffff, 0, 16, 0, 0);
	
	FadeData.trans_stop = 0;
#endif
}

//----------------------------------------------------------
//	�ð���ُ�̪�ďI���҂�
//----------------------------------------------------------
void ClientStatusEffWait(void)
{
	if(CliSys[ClientNo].status_eff == 0)
		ClientBitReset();
}

//----------------------------------------------------------
//	�퓬�̪�ďI���҂�
//----------------------------------------------------------
void ClientFightEffWait(void)
{
	if(CliSys[ClientNo].fight_eff == 0)
		ClientBitReset();
}


//=======================================================
//		�ʐM�ҋ@���b�Z�[�W�̗L��
//=======================================================

void TuushinTaikiCheck(void)
{
	if(FightType&FIGHT_TYPE_TUUSHIN){
		ScrX0=0;
		ScrY0=0;
		TalkMsgSet(&FightMsg,(u8 *)&TaikiMsg,CGX_START+0x10,2,15);
	}
}

//==============================================================
//	�N���C�A���g���ߊ֐�
//==============================================================

//----------------------------------------------------------
//	�w�肵���߹���ް����擾����
//----------------------------------------------------------
void	ClientBSSPokeparaGet(void)
{
	u8 temp[256];
	int	length=0;
	int	i;
	u8	flag;

	if(ServerBuffer[ClientNo][2] == 0){
		length+=ClientBSSPokeparaGetAct(SelMonsNo[ClientNo],&temp[length]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				length+=ClientBSSPokeparaGetAct(i,&temp[length]);
			}
			flag=flag>>1;
		}
	}
	BSS_DATA_TRANS_SET(BSS_CLIENT,length,(u8 *)&temp);
	ClientBitReset();
}

int	ClientBSSPokeparaGetAct(u8 SelectNo,u8 *temp)
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
void ClientBSSPokeparaGet2(void)
{
	PokemonServerParam	PCP;
	u8 *pPP, *pPCP;
	u8 i;
	
	pPP = ((u8 *)(&PokeParaMine[SelMonsNo[ClientNo]])) + ServerBuffer[ClientNo][1];
	pPCP = ((u8 *)(&PCP)) + ServerBuffer[ClientNo][1];
	
	for(i = 0; i < ServerBuffer[ClientNo][2]; i++)
	{
		pPCP[i] = pPP[i];
	}
	BSS_DATA_TRANS_SET(BSS_CLIENT, ServerBuffer[ClientNo][2], &pPCP[0]);

	ClientBitReset();
}

//----------------------------------------------------------
//	�w�肵���߹���ް���ύX����
//----------------------------------------------------------
void ClientBSSPokeparaPut(void)
{
	u8	i,flag;

	if(ServerBuffer[ClientNo][2] == 0){
		ClientBSSPokeparaPutAct(SelMonsNo[ClientNo]);
	}
	else{
		flag = ServerBuffer[ClientNo][2];
		for(i=0;i<6;i++){
			if((flag&1)!=0){
				ClientBSSPokeparaPutAct(i);
			}
			flag=flag>>1;
		}
	}
	ClientBitReset();
}

void ClientBSSPokeparaPutAct(u8 SelectNo)
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
void ClientBSSPokeparaPut2(void)
{
	u8 *pPP;
	u8 i;
	
	pPP = ((u8 *)(&PokeParaMine[SelMonsNo[ClientNo]])) + ServerBuffer[ClientNo][1];
	for(i = 0; i < ServerBuffer[ClientNo][2]; i++)
	{
		pPP[i] = ServerBuffer[ClientNo][3+i];
	}

	ClientBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------

void ClientBSSPokeEncount(void)
{
//	PokemonBObjCellPalSet(PokeParaMine[SelMonsNo[ClientNo]].PPP.monsno, ClientNo);
//	changed by soga 2001.08.29
	PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);

#if 0						//changed by soga 2001.07.18
	PokemonStruct[ClientNo] = AddActorJump(&PM2BTbl[PokeParaMine[SelMonsNo[ClientNo]].PPP.monsno],
						C_POKE1X,
						C_POKE1Y+4*(8-pm2bpara[PokeParaMine
								[SelMonsNo[ClientNo]].PPP.monsno].size),
						C_POKE1PRI);
#endif
	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ClientSequence[ClientNo] = ClientPokeEncountWait;

//	ClientBitReset();
}

//----------------------------------------------------------
//	�߹�ݓo��̪�Ă��N������(�ްق����яo��)
//----------------------------------------------------------

const u8 PokeMinePosX[2][2]={
		{C_POKE_X,C_POKE_X},
		{C_POKE1X,C_POKE2X},
};
const u8 PokeMinePosY[2][2]={
		{C_POKE_Y,C_POKE_Y},
		{C_POKE1Y,C_POKE2Y},
};

void ClientBSSPokeAppear(void)
{
	// add 2002.07.04(��)
	ClientPokeSysFlagClear(ClientNo, ServerBuffer[ClientNo][2]);
	SelMonsNo[ClientNo] = ServerBuffer[ClientNo][1];
	PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
	
	//����ւ�����J�[�\���ʒu�͏�����
	CommandCsrPos[ClientNo]=0;
	WazaCsrPos[ClientNo]=0;
	PokeAppearCommon(ClientNo, ServerBuffer[ClientNo][2]);
	ClientSequence[ClientNo] = ClientPokeAppearWait;
}

//-- PokeAppear�ASendOut ���ʏ��� --//
static void PokeAppearCommon(u8 client_no, u8 baton_flg)
{
	u16 monsno;

	ClientPokeSysFlagClear(client_no, baton_flg);
	
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

//----------------------------------------------------------
//	�߹�݂�߂��̪�Ă��N������
//----------------------------------------------------------
void ClientBSSPokeReturn(void)
{
	if(ServerBuffer[ClientNo][1] == 0)
	{
		CliSys[ClientNo].WazaEffSeqNo = 0;
		ClientSequence[ClientNo] = ClientPokeReturnMigawariChk;
		//SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_MINE);
		//ClientSequence[ClientNo] = ClientPokeReturnWait;
	}
	else
	{
		AffineDelActor(&ActWork[PokemonStruct[ClientNo]]);
		DelActor(&ActWork[PokemonStruct[ClientNo]]);
		GaugeOFF(ClientGaugeSt[ClientNo]);
		ClientBitReset();
	}
}

enum{
	C_RET_MIGAWARICHK,
	C_RET_POKERETURN,
};
static void ClientPokeReturnMigawariChk(void)
{
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case C_RET_MIGAWARICHK:
			if(Climem[ClientNo].migawari == 1)	//�u�݂����v�`�F�b�N
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			CliSys[ClientNo].WazaEffSeqNo = C_RET_POKERETURN;
			break;
		case C_RET_POKERETURN:					//�Z�G�t�F�N�g�Z�b�g
			if(CliSys[ClientNo].etc_eff)
				break;
			CliSys[ClientNo].WazaEffSeqNo = 0;
			SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_POKERETURN_MINE);
			ClientSequence[ClientNo] = ClientPokeReturnWait;
			break;
	}
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ݶ��Ď�)
//----------------------------------------------------------
#define TRAINER_IN_DX	200		//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_IN_SP	(-4)	//�݂��Ă���Ƃ��̽�߰��
void ClientBSSTrainerEncount(void)
{
//	AddBanishActor(CA_TrainerEncount);
//	if(SplitDataSendStatus()){
	s16	ofsx;

	if(FightType&FIGHT_TYPE_4_2vs2){
		if(ClientTypeGet(ClientNo)&2)
			ofsx=16;
		else
			ofsx=-16;
	}
	else
		ofsx=0;

	TrainerBObjCellPalSet(MyData.my_sex, ClientNo);

//	PokeActHeaderMake(MyData.my_sex+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(MyData.my_sex,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							C_TRN_X+ofsx,
							C_TRN_Y+4*(8-trbpara[MyData.my_sex].size),
							//C_TRN_PRI);	//�ڰŰ
							GetPokeSoftPri(ClientNo));

	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = ENCOUNT_DX;
	ActWork[PokemonStruct[ClientNo]].work[0] = -ENCOUNT_SPEED;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;

	ClientSequence[ClientNo] = ClientTrainerEncountWait;
//	}
}

//----------------------------------------------------------
//	�ڰŰ�o��̪�Ă��N������(�ΐ�I����)
//----------------------------------------------------------
#define TRAINER_AP_IN_DX (-96)//�݂��Ă���Ƃ��̏���DX ���K��TRAINER_IN_SP�̔{���ł��邱��
#define TRAINER_AP_IN_SP	(2)	//�݂��Ă���Ƃ��̽�߰��
void ClientBSSTrainerAppear(void)
{
	TrainerBObjCellPalSet(MyData.my_sex, ClientNo);

//	PokeActHeaderMake(MyData.my_sex+500,ClientTypeGet(ClientNo));
	TrainerActHeaderMake(MyData.my_sex,ClientTypeGet(ClientNo));
	PokemonStruct[ClientNo] = AddActor(&PokeActHeader,
							C_TRN_X,
							C_TRN_Y+4*(8-trbpara[MyData.my_sex].size),
							C_TRN_PRI);	//�ڰŰ

	ActWork[PokemonStruct[ClientNo]].oamData.Pltt = ClientNo;
	ActWork[PokemonStruct[ClientNo]].dx = TRAINER_AP_IN_DX;
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_AP_IN_SP;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideIn;

	ClientSequence[ClientNo] = ClientTrainerAppearWait;
}

//----------------------------------------------------------
//	�ڰŰ��߂��̪�Ă��N������
//----------------------------------------------------------
void ClientBSSTrainerReturn(void)
{
#if 0
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_OUT_SP;
//	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_OUT_SP/2;
	ActWork[PokemonStruct[ClientNo]].move = CAS_TrainerSlideOut;
#else
	DxDyAddClear(&ActWork[PokemonStruct[ClientNo]]);
	ActWork[PokemonStruct[ClientNo]].work[0] = TRAINER_RETURN_SYNCNUM;	//VsyncNum
	ActWork[PokemonStruct[ClientNo]].work[2] = -40;
	ActWork[PokemonStruct[ClientNo]].work[4] = ActWork[PokemonStruct[ClientNo]].y;
	ActWork[PokemonStruct[ClientNo]].move = W_StraightSyncMove;
	SetWorkSeq(&ActWork[PokemonStruct[ClientNo]], DummyActMove);
#endif
	ActAnmChg(&ActWork[PokemonStruct[ClientNo]],1);
	ClientSequence[ClientNo] = ClientTrainerReturnWait;
}

//----------------------------------------------------------
//	�߹�݂��C�₷��̪�Ă��N������
//----------------------------------------------------------
void ClientBSSPokeKizetu(void)
{
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case 0:	//�g�����Ԃł��łȂǂŋC�₷�邱�Ƃ�����̂�
			if(Climem[ClientNo].migawari == 1)	//�u�݂����v�`�F�b�N
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			CliSys[ClientNo].WazaEffSeqNo++;
			break;
		case 1:
		default:
			if(CliSys[ClientNo].etc_eff)
				break;
			CliSys[ClientNo].WazaEffSeqNo = 0;

			//AddBanishActor(CA_PokeKizetu);
			PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
			SePlayPan(SE_POKE_DEAD, WAZA_SE_L);
			ActWork[PokemonStruct[ClientNo]].work[1] = 0;
			ActWork[PokemonStruct[ClientNo]].work[2] = C_POKE_KIZETU_SP;
			ActWork[PokemonStruct[ClientNo]].move = SeqMinePokeMove;
			
			ClientSequence[ClientNo] = ClientPokeKizetuWait;
			break;
	}
}

//----------------------------------------------------------
//	�߹�݂��S�ł����Ƃ��̴̪�Ă��N������
//----------------------------------------------------------
void ClientBSSZenmetsu(void)
{
	PaletteFadeReq(0xffffffff, 2, 0, 16, 0);
	ClientBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl�����̴̪�Ă��o��
//----------------------------------------------------------
void ClientBSSGetSuccess(void)
{
#if 0	// 2002.07.18(��)
	ClientStruct[ClientNo] = MonsterBallSet(0,4);
#else
	BtlWork->get_rate = 4;
	ClientEffectWait = 1;	//�ްٹޯĴ̪�Ă͏I��������Ō��Ă�
	SetEtcEffect(ClientNo, ClientNo, ClientNoGet(BSS_CLIENT_ENEMY), FE_ID_POKEGET);
#endif
	ClientSequence[ClientNo] = ClientBallEffectWait;
	//ClientBitReset();
}

//----------------------------------------------------------
//	�߹�ݕߊl���s�̴̪�Ă��o��
//----------------------------------------------------------
void ClientBSSGetFail(void)
{
	u8 rate;
	
	rate = ServerBuffer[ClientNo][1];
#if 0	//2002.07.18(��)
	ClientStruct[ClientNo] = MonsterBallSet(0, rate);
#else
	BtlWork->get_rate = rate;
	ClientEffectWait = 1;	//�ްٹޯĴ̪�Ă͏I��������Ō��Ă�
	SetEtcEffect(ClientNo, ClientNo, ClientNoGet(BSS_CLIENT_ENEMY), FE_ID_POKEGET);
#endif
	ClientSequence[ClientNo] = ClientBallEffectWait;
//	ClientBitReset();
}

//----------------------------------------------------------
//	�߹�����ٱ��߂̴̪�Ă��o��
//----------------------------------------------------------
void ClientBSSPokeLevelUp(void)
{
	u8 i, count;
	u16	pow = 0;
	u16	def = 0;
	u16	agi = 0;
	u16	spepow = 0;
	u16	spedef = 0;

	count = 2;
	
	for(i = ServerBuffer[ClientNo][1]; i != 0; i--)
	{
		switch(ServerBuffer[ClientNo][count])
		{
			case BSS_PARA_POW:				//�U����
				pow = ServerBuffer[ClientNo][count + 1];
				break;
			case BSS_PARA_DEF:				//�h���
				def = ServerBuffer[ClientNo][count + 1];
				break;
			case BSS_PARA_AGI:				//�f����
				agi = ServerBuffer[ClientNo][count + 1];
				break;
			case BSS_PARA_SPEPOW:			//����U����
				spepow = ServerBuffer[ClientNo][count + 1];
				break;
			case BSS_PARA_SPEDEF:			//����h���
				spedef = ServerBuffer[ClientNo][count + 1];
				break;
		}
		count += 2;
	}
	ClientBitReset();
}

//----------------------------------------------------------
//	�w�肵���Z���ް�̴̪�Ă��o��
//----------------------------------------------------------
#if 0	// 2002.07.22(��)
void ClientBSSWazaEffect(void)
{
	u16 wazano;

	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	WazaCounter = ServerBuffer[ClientNo][3];
	WazaEffPower = ServerBuffer[ClientNo][4] | (ServerBuffer[ClientNo][5]<<8);
	WazaEffDamage = ServerBuffer[ClientNo][6] | (ServerBuffer[ClientNo][7]<<8)
		| (ServerBuffer[ClientNo][8]<<16) | (ServerBuffer[ClientNo][9]<<24);
	WazaEffNatukido = ServerBuffer[ClientNo][10] | (ServerBuffer[ClientNo][11]<<8);
	pwkc = (WazaKoukaCount*)&ServerBuffer[ClientNo][12];
	
	if(PutWazaEffectCheck(wazano, WazaCounter)){
		ClientBitReset();
		return;
	}
	
	if(FightCheck() && (AttackClient^2) == DefenceClient && WazaDamage > 0)
		wazano = WAZANO_MAX+1;	//�����ւ̍U���Ȃ�΁A�ȈՓI�ȴ̪�Ă��o��
	
	WazaEffectSet(wazano);
	ClientSequence[ClientNo] = ClientWazaEffectWait;
}
#else
enum{
	C_MIGAWARI_CHK,
	C_EFF_PUTCHK,
	C_EFF_WAIT,
	C_EFF_END,
};

void ClientBSSWazaEffect(void)
{
	u16 wazano;

	if(ClientSEWait(ClientNo))
		return;

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
		ClientBitReset();
		return;
	}
	
	CliSys[ClientNo].WazaEffSeqNo = C_MIGAWARI_CHK;
	ClientSequence[ClientNo] = ClientWazaEffectSet;
}

static void ClientWazaEffectSet(void)
{
	u16 wazano;
	u8 renzoku_cnt;
	
	wazano = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	renzoku_cnt = ServerBuffer[ClientNo][11];	//0:ɰ�� 1:�A���Z�I�� 2:�A���Z��
	switch(CliSys[ClientNo].WazaEffSeqNo){
		case C_MIGAWARI_CHK:	//�u�݂����v�`�F�b�N
			if(Climem[ClientNo].migawari == 1 && Climem[ClientNo].migawari_out == 0){
				Climem[ClientNo].migawari_out = 1;
				SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_OUT);
			}
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
				if(Climem[ClientNo].migawari && renzoku_cnt <= 1){	//�g����`�F�b�N
					SetEtcEffect(ClientNo, ClientNo, ClientNo, FE_ID_MIGAWARI_IN);
					Climem[ClientNo].migawari_out = 0;
				}
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
			ClientBitReset();
			break;
	}
}
#endif

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------
#if 0
void ClientBSSMessage(void)
{
	ScrX0=0;
	ScrY0=0;
//	MsgPutSet(&FightMsg,&ServerBuffer[ClientNo][2],0x10,1,15);
//	TalkMsgSet(&FightMsg,&ServerBuffer[ClientNo][2],CGX_START+0x10,2,15);
	FightMsgCreate((u8 *)&ServerBuffer[ClientNo][2]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientMessageWait;
}
#else
void ClientBSSMessage(void)
{
	u16	*MsgID;

	ScrX0=0;
	ScrY0=0;

	MsgID=(u16 *)&ServerBuffer[ClientNo][2];
	FightMsgID(MsgID[0]);
	TalkMsgSet(&FightMsg,FightMsgBuf,CGX_START+0x10,2,15);
	ClientSequence[ClientNo] = ClientMessageWait;
}
#endif

//----------------------------------------------------------
//	�������ް��𕶎��ް��Ɖ��߂���ү���ޕ\������
//----------------------------------------------------------

void ClientBSSAlertMessage(void)
{
	if(MineEnemyCheck(ClientNo)==SIDE_MINE)
		ClientBSSMessage();
	else
		ClientBitReset();
}

//----------------------------------------------------------
//	����ޑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientBSSCommandSelect(void)
{
	int	i;

	ScrX0=0;
	ScrY0=0x0a0;

	FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_D,2,15,2+25,15+3);	//add 2001.08.07(��)
	FontScrnBoxFillDirect(&FightMsg,SPACE_CHR_D,2,35,2+14,35+1);	//add 2001.08.07(��)
	ClientSequence[ClientNo]=ClientCommandSelect;
	MsgPutSet(&FightMsg, CommandMsg, CGX_START2+0x10,  19, 35);
	MsgPut(&FightMsg);
	for(i=0;i<4;i++) CommandCsrOff(i);
	CommandCsrOn(CommandCsrPos[ClientNo],0);
	FightMsgCreate((u8 *)&DousuruMsg);
	MsgPutSet(&FightMsg, FightMsgBuf, CGX_START2+0x38,  2, 35);
	MsgPut(&FightMsg);
//	ClientBitReset();
}

//----------------------------------------------------------
//	�w�肳�ꂽClientNo�̃V�[�P���X�i���o�[�Ɠ������Ƃ�
//----------------------------------------------------------
void ClientBSSCommandSelectCopy(void)
{
//	ClientBitReset();
}

//----------------------------------------------------------
//	�Z�I����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------

void ClientBSSWazaSelect(void)
{
	ClientBSSWazaSelectAct();
	ClientSequence[ClientNo] = ClientWazaSelect;
}

void ClientBSSWazaSelectAct(void)
{
	ScrX0=0;
	ScrY0=0x140;
	WazaListMake();
	ClientCsrPos2=0xff;	//changed by matsuda 2001.09.26(��)
	WazaCsrOn(WazaCsrPos[ClientNo],0);
	if(ServerBuffer[ClientNo][2]!=BSS_WAZA_SELECT_WAZAWASURE){
		MsgPutSet(&FightMsg,WazaInfoMsg,CGX_START3+0x10,0x13,0x37);
		MsgPut(&FightMsg);
	}
	PPMsgPut();
	WazaTypeMsgPut();
}

//----------------------------------------------------------
//	���ёI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientBSSItemSelect(void)
{
	int	i;

	PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
	ClientSequence[ClientNo] = ClientItemWait;
	TempClientNo = ClientNo;	//add 2001.12.21(��)
	//ListRow�́A��ɃT�[�o���瑗���Ă�����̂��g�p����i�|�P�������X�g�ɔ��ł����A�C�e���΍�j
	for(i = 0; i < TEMOTI_POKEMAX/2; i++)
		ListRow[i] = ServerBuffer[ClientNo][1+i];
	/*
	MonsterBallSet(0,3);
	AddPokemon();	//���b��
	PokeCountMine++;*/
	//ClientBitReset();
}

//----------------------------------------------------------
//	�߹�ݑI����޳��\�����āA�I�𼰹ݽ���쓮������
//----------------------------------------------------------
void ClientBSSPokeSelect(void)
{
	int i;
	
	ClientStruct[ClientNo] = AddTask(DummyTaskSeq, 0xff);	//ܰ��󂯓n���p��
	TaskTable[ClientStruct[ClientNo]].work[0] = ServerBuffer[ClientNo][1]&0x0f;
	//�����ł��ꂩ���ł��Ȃ��Ƃ���SelMonsNo
	UserWork[TOKUSEI_CLIENTNO] = (ServerBuffer[ClientNo][1]&0xf0)>>4;
	//�Q�΂Q�̎��ɂ���������|�P��������ւ���I��ł���Ƃ���SelMonsNo
	UserWork[NO_SEL_MONS] = ServerBuffer[ClientNo][2];
	//�����ł��ꂩ���ł��Ȃ��Ƃ��̓����i���o�[
	UserWork[SPEABINO] = ServerBuffer[ClientNo][3];

	//ListRow�́A��ɃT�[�o���瑗���Ă�����̂��g�p����
	for(i = 0; i < TEMOTI_POKEMAX/2; i++)
		ListRow[i] = ServerBuffer[ClientNo][4+i];
	
#if 0
	//-- �}���`�ʐM���݂̂�ListRow�̏��� --//
//	if(MultiCheck() == TRUE){
	#if 0
		ListRow[0] &= 0xf0;
		ListRow[0] |= ServerBuffer[ClientNo][2] >> 4;
		ListRow[2] = ServerBuffer[ClientNo][3];
	#else
		for(i = 0; i < TEMOTI_POKEMAX/2; i++)
			ListRow[i] = ServerBuffer[ClientNo][2+i];
	#endif
	}
#endif

//	if((FightType&FIGHT_TYPE_TUUSHIN)==0)
		PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
//		LightFadeReq(N_to_B);
		//HardFadeReq(0xff, 0, 0, 16, 0);
	ClientSequence[ClientNo] = ClientPokeSelectWait;
	TempClientNo = ClientNo;	//add 2002.02.25(��)
//	PokeSelFlag=1;
}

//----------------------------------------------------------
//	������̪�Ă��N������
//----------------------------------------------------------
void ClientBSSEscape(void)
{
	PinchSeStop();
	PaletteFadeReq(0xffffffff, 2, 0, 16, 0);
	ClientBitReset();
}

//----------------------------------------------------------
//	HP�ް�ނ𑝌�����
//----------------------------------------------------------
void ClientBSSHpGauge(void)
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
	ClientSequence[ClientNo] = ClientHPGaugeWait;
}

//----------------------------------------------------------
//	EXP�ް�ނ𑝌�����
//----------------------------------------------------------
void ClientBSSExpGauge(void)
{
	s16 beHP;	//��������l
	u16 pokeno;
	u8 temotino;
	u8 id;

	temotino = ServerBuffer[ClientNo][1];

	//��add 2002.02.27(��) ���ق�100�̎��͉��������ɏI��
	if(PokeParaGet(&PokeParaMine[temotino], ID_level) >= 100){
		ClientBitReset();
		return;
	}
	
	BattleGaugeTransWram(1);
	pokeno = PokeParaGet(&PokeParaMine[temotino],ID_monsno);
//	pokelevel = PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_level);
	beHP = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3] << 8);
//	temotino = ServerBuffer[ClientNo][1];

	id = AddTask(ClientEXPGetMsgWaitSeq, 10);
	TaskTable[id].work[0] = temotino;
	TaskTable[id].work[1] = beHP;
	TaskTable[id].work[2] = ClientNo;

//	ClientSequence[ClientNo] = ClientEXPGaugeWait;
	ClientSequence[ClientNo] = ClientDummy;	//EXP�̼��ݽ������ŉ�
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ�̪�Ă��N������(�Q�[�W�̍X�V������)
//----------------------------------------------------------
void ClientBSSStatusSet(void)
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
	CliSys[ClientNo].status_eff = 0;	//�Q�[�W�̍X�V�݂̂����邱�ƂɂȂ���

	ClientSequence[ClientNo] = ClientStatusEffWait;
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ�̪�Ă��N������(�Q�[�W�̍X�V�͂��Ȃ�)
//----------------------------------------------------------
void ClientBSSStatusEffect(void)
{
	u8 condition_flg;
	u32 condition;

	if(ClientSEWait(ClientNo))
		return;
	
	condition_flg = ServerBuffer[ClientNo][1];
	condition = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8) | 
				(ServerBuffer[ClientNo][4]<<16) | (ServerBuffer[ClientNo][5]<<24);
	ClientStatusEffSet(condition_flg, condition);
	ClientSequence[ClientNo] = ClientStatusEffWait;
}

//----------------------------------------------------------
//	�w�肳�ꂽ�ð���ُ��ر����
//----------------------------------------------------------
void ClientBSSStatusClear(void)
{
	u8	buf;
//	PokeParaMine[SelMonsNo[ClientNo]].condition ^= ServerBuffer[ClientNo][1];
//	changed by soga 2001.08.30
	buf=PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_condition)^ServerBuffer[ClientNo][1];
	PokeParaPut(&PokeParaMine[SelMonsNo[ClientNo]],ID_condition,&buf);
	ClientBitReset();
}

//----------------------------------------------------------
//	���炩���ߗp�ӂ��ꂽ�ޯ̧���ް����i�[����
//----------------------------------------------------------
void ClientBSSDataTrans(void)
{
	ClientBitReset();
}

//----------------------------------------------------------
//	�w����ڽ(RAM,VRAM,CGRAM)���ް���]������
//----------------------------------------------------------
void ClientBSSDataRamTrans(void)
{
	u32 destAdr;
	u16 size;
	
	destAdr = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8) |
			(ServerBuffer[ClientNo][3] << 16) | (ServerBuffer[ClientNo][4] << 24);
	size = ServerBuffer[ClientNo][5] | (ServerBuffer[ClientNo][6] << 8);
	
	DIV_DMACOPY(3, &ServerBuffer[ClientNo][7], destAdr, size, 16);

	ClientBitReset();
}

//----------------------------------------------------------
//	���ް���]������
//----------------------------------------------------------
void ClientBSSBgmTrans(void)
{
	u16 data;
	
	data = ServerBuffer[ClientNo][1] | (ServerBuffer[ClientNo][2] << 8);
	MusicPlay(data);

	ClientBitReset();
}

//----------------------------------------------------------
//	�����ް���]������
//----------------------------------------------------------
void ClientBSSVoiceTrans(void)
{
	ClientBitReset();
}

//----------------------------------------------------------
//	�ƭ��őI����������ID��Ԃ�	���ײ��Ăł͍쐬����K�v���Ȃ�����
//----------------------------------------------------------
void ClientBSSSelectReturn(void)
{
	u8 id = 0;
	u16 param = 0;
	
	BSS_SELECT_RETURN_SET(BSS_CLIENT, id, param);
	ClientBitReset();
}

//----------------------------------------------------------
//	�߹�ݓ���ւ����s�������Ƃ�m�点��	���ײ��Ăł͍쐬����K�v���Ȃ�����
//----------------------------------------------------------
void ClientBSSReshuffle(void)
{
	BSS_POKE_RESHUFFLE_SET(BSS_CLIENT,0,0);
	ClientBitReset();
}

//----------------------------------------------------------
//	���юg�p��m�点��	���ײ��Ăł͍쐬����K�v���Ȃ�����
//----------------------------------------------------------
void ClientBSSUseItem(void)
{
	u16 itemno = 0;
	
	BSS_USE_ITEM_SET(BSS_CLIENT, itemno);
	ClientBitReset();
}

//----------------------------------------------------------
//	�ްَg�p��m�点��		���ײ��Ăł͍쐬����K�v���Ȃ�����
//----------------------------------------------------------
void ClientBSSUseBall(void)
{
	BSS_USE_BALL_SET(BSS_CLIENT, 0);
	ClientBitReset();
}

//----------------------------------------------------------
//	�Z�G�t�F�N�g�p�̃J�E���^�̃N���A
//----------------------------------------------------------
void ClientBSSLoopCountClear(void)
{
	WEW.loopcnt = 0;
	ClientBitReset();
}

//----------------------------------------------------------
//	�Z�G�t�F�N�g�p�̃J�E���^�̃Z�b�g
//----------------------------------------------------------
void ClientBSSLoopCountSet(void)
{
	WEW.loopcnt = ServerBuffer[ClientNo][1];
	ClientBitReset();
}

//----------------------------------------------------------
//	�Z�G�t�F�N�g�p�̃t���O�̃N���A
//----------------------------------------------------------
void ClientBSSSwitchFlagClear(void)
{
	WEW.flag = 0;
	ClientBitReset();
}

//----------------------------------------------------------
//	�Z�G�t�F�N�g�p�̃t���O�̃Z�b�g
//----------------------------------------------------------
void ClientBSSSwitchFlagSet(void)
{
	WEW.flag ^= 1;
	ClientBitReset();
}

//----------------------------------------------------------
//	��Ұ�ނ��󂯂��Ƃ����߹�ݓ_��
//----------------------------------------------------------
void ClientBSSPokeDamageBlink(void)
{
	//�߹�݂���Ư����ԂȂ��Blink���Ȃ�
	if(ActWork[PokemonStruct[ClientNo]].banish == 1){
		ClientBitReset();
		return;
	}

	ClientEffectWait = 1;	//Blink�̪�ĊJ�n�t���O�Z�b�g
	ActWork[PokemonStruct[ClientNo]].work[1] = 0;
	
	//-- �ް�ނ̃J���[�ύX & �h�炵�G�t�F�N�g�Z�b�g --//
	GaugeSwayEffect(ClientNo);
	
	ClientSequence[ClientNo] = ClientPokeDamageBlinkWait;
}

//----------------------------------------------------------
//	�Q�[�W������
//----------------------------------------------------------
void ClientBSSGaugeOff(void)
{
//	GaugeOFF(ClientGaugeSt[ClientNo]);
	ClientBitReset();
}

//----------------------------------------------------------
//	SE��炷
//----------------------------------------------------------
void ClientBSSSEPlay(void)
{
	s8 pan;
	
//	SePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	if(MineEnemyCheck(ClientNo) == SIDE_MINE)
		pan = WAZA_SE_L;
	else
		pan = WAZA_SE_R;
	SePlayPan(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8), pan);
	ClientBitReset();
}

//----------------------------------------------------------
//	ME��炷
//----------------------------------------------------------
void ClientBSSMEPlay(void)
{
	JinglePlay(ServerBuffer[ClientNo][1]|(ServerBuffer[ClientNo][2]<<8));
	ClientBitReset();
}

//----------------------------------------------------------
//	������炷
//----------------------------------------------------------

void ClientBSSVoicePlay(void)
{
//	VoicePlay(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno),-25);
	//�C�⎞�̐��Ŗ��悤�ɕύX 2002.08.27(��) by matsuda
	VoicePlay3(PokeParaGet(&PokeParaMine[SelMonsNo[ClientNo]],ID_monsno),-25,5);
	ClientBitReset();
}

//----------------------------------------------------------
//	�G���J�E���g�G�t�F�N�g���Z�b�g
//----------------------------------------------------------
void ClientBSSEncountEffect(void)
{
	EncountEffectSet(ServerBuffer[ClientNo][1]);
	//OBJ���ړ��̃X�g�b�p�[
	EncountEffectFlag|=ENCOUNT_EFFECT_FLAG_OBJSTOP;	
	ClientBitReset();
}

//----------------------------------------------------------
//	�ŏ��̃|�P�����o��(�ڰŰ�������ށA�߹�ݏo���Aү���ޏo���B�܂ł̈�A�̓���
//----------------------------------------------------------
void ClientBSSPokeSendOut(void)
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
	DecordPaletteWork((void*)TRBObjDataPal[MyData.my_sex].pal_data,
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

void TrainerDelEnd(actWork *xreg)
{
	u8 client_no;
	
	client_no = xreg->work[5];	//add 2002.07.04(��)
	
//	TrainerBObjCellPalDel(MyData.my_sex);
	AffineDelActor(xreg);
	ObjPalRegNumDel(ObjPalNumIdGet(xreg->oamData.Pltt));
	DelActor(xreg);
	
	// add 2002.07.04(��)
	PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[client_no]], client_no);
	ActAnmChg(&ActWork[PokemonStruct[client_no]], 0);	//�ŏ��̊G��]�����邽��
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
	if(FightCheck() == 0)
	{	//1vs1
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		PokeAppearCommon(ClientNo, 0);	//ClientBSSPokeAppear();
	}
	else if(FightType&FIGHT_TYPE_4_2vs2)
	{	//2vs2 4�l�ʐM
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		PokeAppearCommon(ClientNo, 0);
	}
	else
	{	//2vs2
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		PokeAppearCommon(ClientNo, 0);	//ClientBSSPokeAppear();
		
		ClientNo ^= 2;
		ServerBuffer[ClientNo][1] = SelMonsNo[ClientNo];
		
		// add 2002.07.04(��)
		PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[ClientNo]], ClientNo);
		
		PokeAppearCommon(ClientNo, 0);	//ClientBSSPokeAppear();
		/*
		if(FightType & FIGHT_TYPE_TUUSHIN)
			ClientSequence[ClientNo] = ClientCLMCommandWait;
		else
			ClientSequence[ClientNo] = ClientCommandWait;
		*/
		ClientNo ^= 2;	//�ύX����ClientNo�����ɖ߂�
	}
	ClientSequence[ClientNo] = ClientPokeSendOutWait;
	ClientNo = clientno_temp;
	DelTask(id);
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W��\������
//----------------------------------------------------------

void	ClientBSSBallGaugeSet(void)
{
	BallGaugePara *bgp;
	
//�t���O���[�N��true�̎��́A�ǂ����T�C�h�Ȃ̂��`�F�b�N���āA�������̂Ƃ��͕\�������Ȃ��悤�ɂ���
	if(ServerBuffer[ClientNo][1]){
		if(MineEnemyCheck(ClientNo)==SIDE_MINE){
			ClientBitReset();
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
	ClientSequence[ClientNo] = ClientBallGaugeWait;
}

static void ClientBallGaugeWait(void)
{
	if(CliSys[ClientNo].wait_work++ > TEMOTI_BALL_WAIT){
		CliSys[ClientNo].wait_work = 0;
		ClientBitReset();
	}
}

//----------------------------------------------------------
//	�g���[�i�[�펞�̃{�[���Q�[�W���\���ɂ���
//----------------------------------------------------------

void	ClientBSSBallGaugeOff(void)
{
	//-- �莝���ް�ނ����� --//
	if(CliSys[ClientNo].temoti_gauge_flg)
		TaskTable[TemotiGauge[ClientNo]].TaskAdrs = TemotiBallOff;
	ClientBitReset();
}

//----------------------------------------------------------
//	�Q�[�W�̂����~�߂�
//----------------------------------------------------------

void ClientBSSGaugeYureStop(void)
{
	SelectMotionStop(ClientNo, MOTION_GAUGE);
	SelectMotionStop(ClientNo, MOTION_POKE);
	ClientBitReset();
}

//----------------------------------------------------------
//	�|�P������VANISH��ON/OFF����
//----------------------------------------------------------

void	ClientBSSVanishOnOff(void)
{
	if(ClientPokeExistCheck(ClientNo)){
		ActWork[PokemonStruct[ClientNo]].banish = ServerBuffer[ClientNo][1];
		PokeClientBanishMemory(ClientNo);
	}
	ClientBitReset();
}

//----------------------------------------------------------
//	�퓬�G�t�F�N�g���o��
//----------------------------------------------------------
void	ClientBSSFightEffect(void)
{
	u8 eff_no;
	u16 monsno;

	if(ClientSEWait(ClientNo))
		return;
	
	eff_no = ServerBuffer[ClientNo][1];
	monsno = ServerBuffer[ClientNo][2] | (ServerBuffer[ClientNo][3]<<8);
	
	if(SetFightEffect(ClientNo, ClientNo, ClientNo, eff_no, monsno))
		ClientBitReset();
	else
		ClientSequence[ClientNo] = ClientFightEffWait;
}

//----------------------------------------------------------
//	�ʐM�ҋ@��ʂ��o��
//----------------------------------------------------------

void	ClientBSSTuushinWait(void)
{
	switch(ServerBuffer[ClientNo][1]){
		case 0:
			TuushinTaikiCheck();
		case 1:
			SelectMotionStop(ClientNo, MOTION_GAUGE);
			SelectMotionStop(ClientNo, MOTION_POKE);
			break;
		case 2:
			TuushinTaikiCheck();
			break;
	}
	ClientBitReset();
}

//----------------------------------------------------------
//	�L�����Ă���J�[�\���ʒu�̃N���A
//----------------------------------------------------------

void	ClientBSSCsrPosClear(void)
{
	switch(ServerBuffer[ClientNo][1]){
		case BSS_CSRPOS_CLEAR_ALL:
			CommandCsrPos[ClientNo]=0;
			WazaCsrPos[ClientNo]=0;
			break;
		case BSS_CSRPOS_CLEAR_COMMAND:
			CommandCsrPos[ClientNo]=0;
			break;
		case BSS_CSRPOS_CLEAR_WAZA:
			WazaCsrPos[ClientNo]=0;
			break;
	}
	ClientBitReset();
}

//----------------------------------------------------------
//	�ΐ�I����m�点��
//----------------------------------------------------------
void ClientBSSFightEnd(void)
{
#if 0
	WinLoseFlag=ServerBuffer[ClientNo][1];
	BGM_FadeOutSet(5);
	LightFadeReq(N_to_B);
	ClientBitReset();
	if((FightType&FIGHT_TYPE_TUUSHIN)&&((FightType&FIGHT_TYPE_OYA)==0))
		ClientSequence[ClientNo] = ClientFightEndWait;
#else
	WinLoseFlag=ServerBuffer[ClientNo][1];
	BGM_FadeOutSet(5);
	LightFadeReq(N_to_B);
	ClientBitReset();
	ClientSequence[ClientNo] = ClientFightEndWait;
#endif
}

//----------------------------------------------------------
//	��а
//----------------------------------------------------------
void ClientBSSEndID(void)
{
//	ClientBitReset();
}

//-----------------------------------------------------------
//	��������_������I�Iby Soga
//-----------------------------------------------------------
#if 0
		case BSS_PARA_FIGHT:			//�퓬���ɕK�v�ȃp�����[�^�S��
			PCP.monsno		=PokeParaMine[SelectNo].PPP.monsno;
			PCP.item		=PokeParaMine[SelectNo].PPP.item;
			for(i=0;i<4;i++){
				PCP.waza[i]	=PokeParaMine[SelectNo].PPP.waza[i];
				PCP.pp[i]	=PokeParaMine[SelectNo].PPP.pp[i];
			}
			PCP.pp_count	=PokeParaMine[SelectNo].PPP.pp_count;
			PCP.friend		=PokeParaMine[SelectNo].PPP.friend;
			PCP.hp_rnd		=PokeParaMine[SelectNo].PPP.hp_rnd;
			PCP.pow_rnd		=PokeParaMine[SelectNo].PPP.pow_rnd;
			PCP.def_rnd		=PokeParaMine[SelectNo].PPP.def_rnd;
			PCP.agi_rnd		=PokeParaMine[SelectNo].PPP.agi_rnd;
			PCP.spepow_rnd	=PokeParaMine[SelectNo].PPP.spepow_rnd;
			PCP.spedef_rnd	=PokeParaMine[SelectNo].PPP.spedef_rnd;
			PCP.condition	=PokeParaMine[SelectNo].condition;
			PCP.level		=PokeParaMine[SelectNo].level;
			PCP.hp			=PokeParaMine[SelectNo].hp;
			PCP.hpmax		=PokeParaMine[SelectNo].hpmax;
			PCP.pow			=PokeParaMine[SelectNo].pow;
			PCP.def			=PokeParaMine[SelectNo].def;
			PCP.agi			=PokeParaMine[SelectNo].agi;
			PCP.spepow		=PokeParaMine[SelectNo].spepow;
			PCP.spedef		=PokeParaMine[SelectNo].spedef;
			//BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(PCP),(u8 *)&PCP);
			BSS_DATA_TRANS_SET(BSS_CLIENT,
						XtOffset(PokemonServerParam *, abiritycnt),(u8 *)&PCP);
			break;
		case BSS_PARA_MONSNO:			//�����X�^�[�ԍ�
			PCP.monsno		=PokeParaMine[SelectNo].PPP.monsno;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(PCP.monsno),(u8 *)&PCP.monsno);
			break;
		case BSS_PARA_ITEM:				//�����A�C�e��
			PCP.item		=PokeParaMine[SelectNo].PPP.item;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(PCP.item),(u8 *)&PCP.item);
			break;
		case BSS_PARA_WAZA:
			for(i=0;i<4;i++){
				PCP.waza[i] = PokeParaMine[SelectNo].PPP.waza[i];
			}
			BSS_DATA_TRANS_SET(BSS_CLIENT, 8, (u8 *)&PCP.waza);
			break;
		case BSS_PARA_WAZA1:				//�Z
		case BSS_PARA_WAZA2:
		case BSS_PARA_WAZA3:
		case BSS_PARA_WAZA4:
			PCP.waza[ServerBuffer[ClientNo][1]-BSS_PARA_WAZA1]=
			PokeParaMine[SelectNo].PPP.waza[ServerBuffer[ClientNo][1]-BSS_PARA_WAZA1];
			BSS_DATA_TRANS_SET(
			BSS_CLIENT,
			sizeof(PCP.waza),
			(u8 *)&PCP.waza[ServerBuffer[ClientNo][1]-BSS_PARA_WAZA1]);
			break;
		case BSS_PARA_PP:
			for(i=0;i<4;i++){
				temp[i] = PokeParaMine[SelectNo].PPP.pp[i];
			}
			temp[i] = PokeParaMine[SelectNo].PPP.pp_count;
			BSS_DATA_TRANS_SET(BSS_CLIENT, 5, temp);
			break;
		case BSS_PARA_PP1:				//�Z�|�C���g
		case BSS_PARA_PP2:
		case BSS_PARA_PP3:
		case BSS_PARA_PP4:
			PCP.pp[ServerBuffer[ClientNo][1]-BSS_PARA_PP1]=
			PokeParaMine[SelectNo].PPP.pp[ServerBuffer[ClientNo][1]-BSS_PARA_PP1];
			BSS_DATA_TRANS_SET(
			BSS_CLIENT,
			sizeof(PCP.pp),
			(u8 *)&PCP.pp[ServerBuffer[ClientNo][1]-BSS_PARA_PP1]);
			break;
		case BSS_PARA_PPCOUNT1:
		case BSS_PARA_PPCOUNT2:
		case BSS_PARA_PPCOUNT3:
		case BSS_PARA_PPCOUNT4:
#if 0
			temp[0] = PokeParaMine[SelectNo].PPP.pp_count1;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaMine[SelectNo].PPP.pp_count2;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaMine[SelectNo].PPP.pp_count3;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
			break;
			temp[0] = PokeParaMine[SelectNo].PPP.pp_count4;
			BSS_DATA_TRANS_SET(BSS_CLIENT,sizeof(temp[0]),&temp[0]);
#endif
			break;
		case BSS_PARA_ID:				//�h�c�i���o�[
			temp[0] = (u8)(PokeParaMine[SelectNo].PPP.id_no);
			temp[1] = (u8)(PokeParaMine[SelectNo].PPP.id_no >> 8);
			temp[2] = (u8)(PokeParaMine[SelectNo].PPP.id_no >> 16);
			BSS_DATA_TRANS_SET(BSS_CLIENT, 3, temp);
			break;
		case BSS_PARA_EXP:				//��{�o���l
			temp[0] = (u8)(PokeParaMine[SelectNo].PPP.exp);
			temp[1] = (u8)(PokeParaMine[SelectNo].PPP.exp >> 8);
			temp[2] = (u8)(PokeParaMine[SelectNo].PPP.exp >> 16);
			BSS_DATA_TRANS_SET(BSS_CLIENT, 3, temp);
			break;
		case BSS_PARA_HPEXP:			//�g�o�w�͒l
			temp[0] = PokeParaMine[SelectNo].PPP.hp_exp;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_POWEXP:			//�U���͓w�͒l
			temp[0] = PokeParaMine[SelectNo].PPP.pow_exp;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_DEFEXP:			//�h��͓w�͒l
			temp[0] = PokeParaMine[SelectNo].PPP.def_exp;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_AGIEXP:			//�f�����w�͒l
			temp[0] = PokeParaMine[SelectNo].PPP.agi_exp;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_SPEPOWEXP:		//����U���w�͒l
			temp[0] = PokeParaMine[SelectNo].PPP.spepow_exp;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_SPEDEFEXP:		//����h��w�͒l
			temp[0] = PokeParaMine[SelectNo].PPP.spedef_exp;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_FRIEND:			//�F�D�l
			PCP.friend		=PokeParaMine[SelectNo].PPP.friend;
			BSS_DATA_TRANS_SET(BSS_CLIENT,1,(u8 *)&PCP.friend);
			break;
		case BSS_PARA_POKERUS:			//�|�P���X
			temp[0] = PokeParaMine[SelectNo].PPP.pokerus;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_GETDATA:			//�ߊl�f�[�^
			temp[0] = (u8)(PokeParaMine[SelectNo].PPP.get);
			temp[1] = (u8)(PokeParaMine[SelectNo].PPP.get >> 8);
			temp[2] = (u8)(PokeParaMine[SelectNo].PPP.get >> 16);
			BSS_DATA_TRANS_SET(BSS_CLIENT, 3, temp);
			break;
		case BSS_PARA_POWERRND:
			temp[0] = PokeParaMine[SelectNo].PPP.hp_rnd;
			temp[1] = PokeParaMine[SelectNo].PPP.pow_rnd;
			temp[2] = PokeParaMine[SelectNo].PPP.def_rnd;
			temp[3] = PokeParaMine[SelectNo].PPP.agi_rnd;
			temp[4] = PokeParaMine[SelectNo].PPP.spepow_rnd;
			temp[5] = PokeParaMine[SelectNo].PPP.spedef_rnd;
			BSS_DATA_TRANS_SET(BSS_CLIENT, 6, temp);
			break;
		case BSS_PARA_HPRND:			//HP�p���[����
			temp[0] = PokeParaMine[SelectNo].PPP.hp_rnd;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_POWRND:			//�U���̓p���[����
			temp[0] = PokeParaMine[SelectNo].PPP.pow_rnd;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_DEFRND:			//�h��̓p���[����
			temp[0] = PokeParaMine[SelectNo].PPP.def_rnd;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_AGIRND:			//�f�����p���[����
			temp[0] = PokeParaMine[SelectNo].PPP.agi_rnd;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_SPEPOWRND:		//����U���̓p���[����
			temp[0] = PokeParaMine[SelectNo].PPP.spepow_rnd;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_SPEDEFRND:		//����h��̓p���[����
			temp[0] = PokeParaMine[SelectNo].PPP.spedef_rnd;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(temp[0]), &temp[0]);
			break;
		case BSS_PARA_CHRRND:			//������
			buf32 = PokeParaMine[SelectNo].PPP.personal_rnd;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(buf32), (u8 *)&buf32);
			break;
		case BSS_PARA_SUM:				//�`�F�b�N�T��
			buf16 = PokeParaMine[SelectNo].PPP.checksum;
			BSS_DATA_TRANS_SET(BSS_CLIENT, sizeof(buf16), (u8 *)&buf16);
			break;
		case BSS_PARA_CONDITION:		//�R���f�B�V����
			PCP.condition	=PokeParaMine[SelectNo].condition;
			BSS_DATA_TRANS_SET(BSS_CLIENT,1,(u8 *)&PCP.condition);
			break;
		case BSS_PARA_LEVEL:			//���x��
			PCP.level		=PokeParaMine[SelectNo].level;
			BSS_DATA_TRANS_SET(BSS_CLIENT,1,(u8 *)&PCP.level);
			break;
		case BSS_PARA_HP:				//�g�o�c��
			PCP.hp			=PokeParaMine[SelectNo].hp;
			BSS_DATA_TRANS_SET(BSS_CLIENT,2,(u8 *)&PCP.hp);
			break;
		case BSS_PARA_HPMAX:			//�g�o�l�`�w
			PCP.hpmax		=PokeParaMine[SelectNo].hpmax;
			BSS_DATA_TRANS_SET(BSS_CLIENT,2,(u8 *)&PCP.hpmax);
			break;
		case BSS_PARA_POW:				//�U����
			PCP.pow			=PokeParaMine[SelectNo].pow;
			BSS_DATA_TRANS_SET(BSS_CLIENT,2,(u8 *)&PCP.pow);
			break;
		case BSS_PARA_DEF:				//�h���
			PCP.def			=PokeParaMine[SelectNo].def;
			BSS_DATA_TRANS_SET(BSS_CLIENT,2,(u8 *)&PCP.def);
			break;
		case BSS_PARA_AGI:				//�f����
			PCP.agi			=PokeParaMine[SelectNo].agi;
			BSS_DATA_TRANS_SET(BSS_CLIENT,2,(u8 *)&PCP.agi);
			break;
		case BSS_PARA_SPEPOW:			//����U����
			PCP.spepow		=PokeParaMine[SelectNo].spepow;
			BSS_DATA_TRANS_SET(BSS_CLIENT,2,(u8 *)&PCP.spepow);
			break;
		case BSS_PARA_SPEDEF:			//����h���
			PCP.spedef		=PokeParaMine[SelectNo].spedef;
			BSS_DATA_TRANS_SET(BSS_CLIENT,2,(u8 *)&PCP.spedef);
			break;
#endif
#if 0
		case BSS_PARA_MONSNO:			//�����X�^�[�ԍ�
			PokeParaMine[SelectNo].PPP.monsno = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_ITEM:				//�����A�C�e��
			PokeParaMine[SelectNo].PPP.item =
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_WAZA:
			PokeParaMine[SelectNo].PPP.waza[0] = 
			ServerBuffer[ClientNo][BSS_DATASTART] |
			(ServerBuffer[ClientNo][BSS_DATASTART+1] << 8);

			PokeParaMine[SelectNo].PPP.waza[1] = 
			ServerBuffer[ClientNo][BSS_DATASTART+2] |
			(ServerBuffer[ClientNo][BSS_DATASTART+3] << 8);

			PokeParaMine[SelectNo].PPP.waza[2] = 
			ServerBuffer[ClientNo][BSS_DATASTART+4] |
			(ServerBuffer[ClientNo][BSS_DATASTART+5] << 8);

			PokeParaMine[SelectNo].PPP.waza[3] = 
			ServerBuffer[ClientNo][BSS_DATASTART+6] |
			(ServerBuffer[ClientNo][BSS_DATASTART+7] << 8);
			break;
		case BSS_PARA_WAZA1:				//�Z
		case BSS_PARA_WAZA2:
		case BSS_PARA_WAZA3:
		case BSS_PARA_WAZA4:
			PokeParaMine[SelectNo].PPP.waza[ServerBuffer[ClientNo][1]-BSS_PARA_WAZA1]=ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_PP:
			PokeParaMine[SelectNo].PPP.pp[0] = ServerBuffer[ClientNo][BSS_DATASTART];
			PokeParaMine[SelectNo].PPP.pp[1] = ServerBuffer[ClientNo][BSS_DATASTART+1];
			PokeParaMine[SelectNo].PPP.pp[2] = ServerBuffer[ClientNo][BSS_DATASTART+2];
			PokeParaMine[SelectNo].PPP.pp[3] = ServerBuffer[ClientNo][BSS_DATASTART+3];
			PokeParaMine[SelectNo].PPP.pp_count = ServerBuffer[ClientNo][BSS_DATASTART+4];
			break;
		case BSS_PARA_PP1:				//�Z�|�C���g
		case BSS_PARA_PP2:
		case BSS_PARA_PP3:
		case BSS_PARA_PP4:
			PokeParaMine[SelectNo].PPP.pp[ServerBuffer[ClientNo][1]-BSS_PARA_PP1]=ServerBuffer[ClientNo][BSS_DATALOW];
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
			PokeParaMine[SelectNo].PPP.id_no =
			ServerBuffer[ClientNo][BSS_DATASTART] | 
			(ServerBuffer[ClientNo][BSS_DATASTART+1] << 8) |
			(ServerBuffer[ClientNo][BSS_DATASTART+2] << 16);
			break;
		case BSS_PARA_EXP:				//��{�o���l
			PokeParaMine[SelectNo].PPP.exp =
			ServerBuffer[ClientNo][BSS_DATASTART] | 
			(ServerBuffer[ClientNo][BSS_DATASTART+1] << 8) |
			(ServerBuffer[ClientNo][BSS_DATASTART+2] << 16);
			break;
		case BSS_PARA_HPEXP:			//�g�o�w�͒l
			PokeParaMine[SelectNo].PPP.hp_exp = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_POWEXP:			//�U���͓w�͒l
			PokeParaMine[SelectNo].PPP.pow_exp = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_DEFEXP:			//�h��͓w�͒l
			PokeParaMine[SelectNo].PPP.def_exp = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_AGIEXP:			//�f�����w�͒l
			PokeParaMine[SelectNo].PPP.agi_exp = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_SPEPOWEXP:		//����U���w�͒l
			PokeParaMine[SelectNo].PPP.spepow_exp = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_SPEDEFEXP:		//����h��w�͒l
			PokeParaMine[SelectNo].PPP.spedef_exp = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_FRIEND:			//�F�D�l
			PokeParaMine[SelectNo].PPP.friend = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_POKERUS:			//�|�P���X
			PokeParaMine[SelectNo].PPP.pokerus = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_GETDATA:			//�ߊl�f�[�^
			PokeParaMine[SelectNo].PPP.get =
			ServerBuffer[ClientNo][BSS_DATASTART] | 
			(ServerBuffer[ClientNo][BSS_DATASTART+1] << 8) |
			(ServerBuffer[ClientNo][BSS_DATASTART+2] << 16);
			break;
		case BSS_PARA_POWERRND:
			PokeParaMine[SelectNo].PPP.hp_rnd =
			ServerBuffer[ClientNo][BSS_DATASTART];
			PokeParaMine[SelectNo].PPP.pow_rnd =
			ServerBuffer[ClientNo][BSS_DATASTART+1];
			PokeParaMine[SelectNo].PPP.def_rnd =
			ServerBuffer[ClientNo][BSS_DATASTART+2];
			PokeParaMine[SelectNo].PPP.agi_rnd =
			ServerBuffer[ClientNo][BSS_DATASTART+3];
			PokeParaMine[SelectNo].PPP.spepow_rnd =
			ServerBuffer[ClientNo][BSS_DATASTART+4];
			PokeParaMine[SelectNo].PPP.spedef_rnd =
			ServerBuffer[ClientNo][BSS_DATASTART+5];
			break;
		case BSS_PARA_HPRND:			//HP�p���[����
			PokeParaMine[SelectNo].PPP.hp_rnd = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_POWRND:			//�U���̓p���[����
			PokeParaMine[SelectNo].PPP.pow_rnd = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_DEFRND:			//�h��̓p���[����
			PokeParaMine[SelectNo].PPP.def_rnd = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_AGIRND:			//�f�����p���[����
			PokeParaMine[SelectNo].PPP.agi_rnd = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_SPEPOWRND:		//����U���̓p���[����
			PokeParaMine[SelectNo].PPP.spepow_rnd = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_SPEDEFRND:		//����h��̓p���[����
			PokeParaMine[SelectNo].PPP.spedef_rnd = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_CHRRND:			//������
			PokeParaMine[SelectNo].PPP.personal_rnd =
			ServerBuffer[ClientNo][BSS_DATASTART] |
			(ServerBuffer[ClientNo][BSS_DATASTART+1] << 8) |
			(ServerBuffer[ClientNo][BSS_DATASTART+2] << 16) |
			(ServerBuffer[ClientNo][BSS_DATASTART+3] << 24);
			break;
		case BSS_PARA_SUM:				//�`�F�b�N�T��
			PokeParaMine[SelectNo].PPP.checksum = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_CONDITION:		//�R���f�B�V����
			PokeParaMine[SelectNo].condition = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_LEVEL:			//���x��
			PokeParaMine[SelectNo].level = ServerBuffer[ClientNo][BSS_DATALOW];
			break;
		case BSS_PARA_HP:				//�g�o�c��
			PokeParaMine[SelectNo].hp = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_HPMAX:			//�g�o�l�`�w
			PokeParaMine[SelectNo].hpmax = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_POW:				//�U����
			PokeParaMine[SelectNo].pow = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_DEF:				//�h���
			PokeParaMine[SelectNo].def = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_AGI:				//�f����
			PokeParaMine[SelectNo].agi = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_SPEPOW:			//����U����
			PokeParaMine[SelectNo].spepow = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
		case BSS_PARA_SPEDEF:			//����h���
			PokeParaMine[SelectNo].spedef = 
							ServerBuffer[ClientNo][BSS_DATALOW] | (ServerBuffer[ClientNo][BSS_DATAHIGH] << 8);
			break;
#endif
