//======================================================
//                                                      
//    server.c                                           
//                                                      
//    Copyright (C) 2000 GAME FREAK inc.
//======================================================

#include "common.h"
#include "bss.h"			//add
#include "calctool.h"		//add
#include "actor.h"
#include "decord.h"
#include "actanm.h"
#include "objdata.h"
#include "poketool.h"
#include "fight.h"
#include "agbmons.h"
#include "print.h"
#include "zokusei.h"
#include "pm_str.h"
#include "waza_eff.h"
#define __SERVER_H_
#include "server.h"
#include "waza_seq.h"
#include "item_seq.h"
#include "pokelist.h"
#include "wazatool.h"
#include "field.h"
#include "ev_fight.h"
#include "trainer.h"
#include "waza_tbl.h"
#include "client.h"
#include "client_t.h"
#include "gauge.h"
#include "multisio.h"		//add
#include "pokesio.h"
#include "cable.h"
#include "palanm.h"
#include "namein.h"
#include "task.h"
#include "laster.h"
#include "fight_ef.h"
#include "wazano.def"
#include "letter.h"
#include "item.h"
#include "zukan.h"
#include "tokusyu.def"
#include "gold.h"
#include "record.h"
#include "fld_main.h"
#include "mus_tool.h"
#include "shinka.h"
#include "fightmsg.h"
#include "weather.h"
#include "trainer.h"
#include "tr_enum.h"
#include "b_pmlist.h"
#include "ev_flag.h"
#include "sysflag.h"
#include "safari.h"
#include "..\data\itemeqp.h"
#include "mv_poke.h"
#include "monsno.def"
#include "mapparam.h"
#include "seed.h"
#include "p_status.h"
#include "b_return.h"
#include "config.h"
#include "waza_ai.h"
#include "tr_money.dat"
#include "gauge.h"

#define	SERVER_SUB		0			//ServerFromTurnCheck�������̂��H
#define USE_SENDOUT		1			//POKE_SENDOUT�������̂��H
#define RND_NO_AMARI	0			//�����̎������������]��ɂ���̂��A���Ȃ��̂�

#define	WIN_SCR			24

//����\�̓`�F�b�N�p�V�[�P���X�i���o�[

#if 0
//WazaKoukaCheck�p�V�[�P���X�i���o�[

enum{
	WKC_IKARI_CHECK=0,
	WKC_TOKERU_CHECK,
	WKC_SPEABI_SINKURO_DEFENCE,
	WKC_SPEABI_CHECK,
	WKC_SPEABI_SINKURO_ATTACK,
	WKC_SOUBI_CHECK,
	WKC_VANISH_ATTACK_ON,
	WKC_VANISH_ATTACK_CHECK,
	WKC_VANISH_DEFENCE_CHECK,
	WKC_MIGAWARI_CHECK,
	WKC_END,
};
#endif

#define	WAZA_SEQ_ASM	1



typedef u32(*ASMADRS)(u8);

//------------------------- �f�[�^ -----------------------------

#include	"tokusyu.dat"

//---------------------- �O���[�o���ϐ� --------------------------

u8		ServerBuffer[CLIENT_MAX][512]={{0}};
u8		ClientBuffer[CLIENT_MAX][512]={{0}};
pFunc	ServerSequence;
pFunc	PushFirstProc;
pFunc	ClientSequence[CLIENT_MAX];
u8		LevelUpBit;

u8		ClientNo=0;
u32		ClientBit=0;
u8		ClientSetMax=0;
u16		SelMonsNo[CLIENT_MAX]={0};

u8		ClientType[CLIENT_MAX]={0};
u8		ActionNo[CLIENT_MAX]={0};
u8		ActionClientNo[CLIENT_MAX]={0};
u8		ActionCount=0;
u8		ActionSeqNo=0;

PokemonServerParam	PSP[CLIENT_MAX]={{0}};
u8		PokemonStruct[CLIENT_MAX]={0};

u8		WazaPosNo=0;
u8		WazaPosNoTmp=0;
u16		WazaNo=0;
u16		WazaNoTmp=0;
u16		PushWazaNo=0;
s32		WazaDamage=0;
s32		HitDamage=0;
s32		StoreDamage[CLIENT_MAX]={};

u16		ItemNo=0;
u8		SpeabiNo=0;
u8		AttackClient=0;
u8		DefenceClient=0;
u8		KizetsuClient=0;
u8		TsuikaClient=0;
u8		ItemClient=0;
u8		NoReshuffleClient=0;
u8		CriticalValue=0;
u8		WazaSeqLoopCount=0;
u8		*WazaSeqAdrs=0;
u8		*PushWazaSeqAdrs=0;
u8		CommandReturn[CLIENT_MAX]={0};
u8		*AlertSeqAdrs[CLIENT_MAX]={};

u16		WazaNoSketch[CLIENT_MAX]={0};
u16		WazaNoUse[CLIENT_MAX]={0};
u16		WazaNoHit[CLIENT_MAX]={0};
u16		WazaTypeHit[CLIENT_MAX]={0};
u16		WazaNoLast[CLIENT_MAX]={0};
u16		WazaNoKeep[CLIENT_MAX]={0};
u8		WazaNoAttackClient[CLIENT_MAX]={0};
//u8		WazaNoDefenceClient[CLIENT_MAX]={0};
u16		SelectWazaNo[CLIENT_MAX]={0};

u8		WazaStatusFlag=0;
u32		ServerStatusFlag=0;
u8		ClientStatusFlag[CLIENT_MAX]={0};
u8		ClientNoHit[CLIENT_MAX]={0};		//���܂�p

u8 		PokeSelFlag=0;

u16		SideCondition[2]={};
SideConditionCount	SCC[2]={{}};

u32		WazaKouka[CLIENT_MAX]={};
WazaKoukaCount	WKC[CLIENT_MAX]={{}};

u16		MojiWaitCount=0;

u16		KobanCounter=0;
u16		AgiRnd=0;

u8		ServerWork[8]={};					//�T�[�o�[�p�ėp���[�N
//ServerWork[3]==�Z�ǉ����ʂ̃i���o�[
//ServerWork[4]==WazaKoukaCheck�̃V�[�P���X�i���o�[
//ServerWork[5]==MESSAGE_TBL�̃C���f�b�N�X�i���o�[
//ServerWork[6]==WS_WAZAKOUKA_MESSAGE��NoHit���b�Z�[�W�i���o�[
//ServerWork[7]==���b�Z�[�W�E�F�C�g�̗L���^����

u8		WinLoseFlag=0;

//u8		*RetWazaSeqAdrs=0;	//�Z�V�[�P���X�̕Ԃ�l�i�T�u���[�`���W�����v�p�j
//u8		*RetSpeabiSeqAdrs=0;	//�Z�V�[�P���X�̕Ԃ�l�i����\�͗p�j

OneTurnFlag		OTF[CLIENT_MAX]={{}};
OneSelfTurnFlag	OSTF[CLIENT_MAX]={{}};

u16		FieldCondition=0;
FieldConditionCount	FCC={};

u16		EncountEffectFlag=0;

u8		FightJoinFlag[2]={};

u16		DamagePowWork=0;

u16		GakushuuExp=0;

FightTVWork	FTW;

FightRamSeed FRS[CLIENT_MAX]={{}};

//-------------------- �T�u���[�`���錾 ------------------------

void	ServerDummy(void);
void	ServerInit(void);
void	ServerMain(void);
void	ServerCounterInit(void);
void	ServerPokeParaGet(void);
void	ServerEncountEffect(void);
void	ServerResWait(void);
void	ServerEncountMessage(void);
void	ServerBallGaugeSet(void);
void	ServerBallGaugeMSet(void);
void	ServerPokemonEAppearMsg(void);
void	ServerPokemonEAppear(void);
void	ServerTrainerReturnE(void);
void	ServerTrainerReturnEWait(void);
void	ServerPokemonMAppearMsg(void);
void	ServerPokemonMAppear(void);
void	ServerTrainerReturnM(void);
void	ServerTrainerReturnMWait(void);
void	ServerSpecialAbirityCheck(void);
void	ServerTurnCheckInit(void);
void	ServerTurnCheck(void);
void	ServerCommandSelectInit(void);
void	ServerAgiCheck(void);
void	ServerWazaBeforeMsg(void);
void	ServerCommandAct(void);
void	ServerWin(void);
void	ServerLose(void);
void	ServerEscape(void);
void	ServerEnemyEscape(void);
void	ServerEnd(void);
void	ServerSubroutine(void);
void	ServerSpeabiSubroutine(void);
#if SERVER_SUB
void	ServereFromTurnCheckAct(void);
#endif

void	ServerShinkaCheckInit(void);
void	ServerShinkaCheck(void);
void	ServerShinkaWait(void);
void	FightEnd(void);

void	ClientBitSetAll(void);
void	ClientBitSet(u8);
void	ClientBitSet2(u8);

void	FightJoinFlagInit(void);
void	FightJoinFlagInitClient(u8);
void	FightJoinFlagSet(u8);

void	SCA_CommandBranch(void);
void	SCA_FightAct(void);
void	SCA_WazaSequence(void);
//void	SCA_HitCheck(void);
void	SCA_PokeAct(void);
void	SCA_ItemAct(void);
void	SCA_EscapeAct(void);
void	SCA_SafariAct(void);
void	SCA_BallAct(void);
void	SCA_CubeAct(void);
void	SCA_ApproachAct(void);
void	SCA_SafariEscapeAct(void);
void	SCA_GetDemoAct(void);
void	SCA_EndCheck(void);
void	SCA_DeadEnd(void);
void	SCA_End(void);

void	DamageCalcAct(u8 attack,u8 defence);
void	TypeCheckCalc(u8);
void	TypeCheckCalc2(u8,u16,u8 *);
u8		TypeCheckAct(u16,u8,u8);
u8		TypeCheckAct2(u16 wazano,u16 monsno,u8 speabino);
void	DamageLossAct(void);
void	FightMsgServerSet(u16,u8);

void	WindowOpen(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);

void	PushAdrsSet(u8 *);
void	PushAdrsAct();
void	PopAdrsAct();

void	YesNoCsrOn(void);
void	YesNoCsrOff(void);

u8	SawaguCheck(u8);

u8	FieldConditionCheck(void);
u8	SideConditionCheck(void);
u8	SideConditionCheck2(void);
//u8	WazaKoukaCheck(void);
u8	GetExpCheck(void);
void	ServerWazaOutBeforeCheck(void);
u8	PokeConditionCheck(void);
u8	PokeReshuffleCheck(u8,u8,u8);
u8	SpeabiTenkiyaAct(u8);
u8	SpecialAbirityCheck(u8,u8,u8,u8,u16);
u8	SoubiItemCheck(u8,u8,u8);

void	KyouryokuWazaCheck(void);

void	ServerSubroutineSet(u8 *);
void	ServerSpeabiSubroutineSet(u8 *);

u8		CondChgAct(s8 value,u8 type,u8 flag,u8 *adrs);

void	MichidureCheckAct(void);
u8	ReturnWazaNoCheck(void);
u8	WaruagakiCheck(void);
u8	GuardCheck(u16);
u8	HuuinCheck(u8,u16);
void	ServerKeepOff(u8 clientno);
u8		WazaOutCheck(u8 clientno);

u8	AgiCalc(u8,u8,u8);
void	SortAct(u8,u8);
u8	BadgeCheck(void);
u8	WaruagakiWazaNoCheck(u8,u8,u8);

void	ClearOneTurnCount(u8);
void	ClearOneSelfTurnCount(void);

u8	TameWazaCheck(u16);
u8	TameWazaTurnCheck(u8,u16);
u8	RendouWazaCheck(u16);

void	TsuikaAct(u8,u8);

void	OboeWazaNoSet(void);
u8		ActionNoGet(u8);
void	WazaNoHitTypeCheck(void);
u8		ServerDefenceClientGet(u16,u8);
void	PressurePPDecCheck(u8,u8,u16);
void	PressurePPDecCheckHuuin(u8);
void	PressurePPDecCheckHorobinouta(u8);

//================================================
//		�Z�V�[�P���X
//================================================

//typedef void (*pWazaSeqFunc)(u8 *);

const pFunc WazaSequenceTbl[];

//----------------------------------------------------------
//	�v���g�^�C�v�錾	�֐����ް�
//----------------------------------------------------------

extern	const	WazaTblDat	WTD[];
extern	void	DebugFightInit(void);
extern	const	PokePersonalData PPD[];

extern	TALK_WIN	FightMsg;
//extern	u8	FightMsgBuf[64];
extern	u8	EvStrBuf0[16];
extern	u8	EvStrBuf1[16];

const pFunc ServerCommandActTbl[] =
{
	SCA_FightAct,				//�����܂ł͏��Ԃ���ꊷ���Ă̓_���I�I
	SCA_ItemAct,				//
	SCA_PokeAct,				//
	SCA_EscapeAct,				//
	SCA_SafariAct,				//
	SCA_BallAct,				//
	SCA_CubeAct,				//
	SCA_ApproachAct,			//
	SCA_SafariEscapeAct,		//
	SCA_GetDemoAct,				//
	SCA_WazaSequence,
	SCA_EndCheck,
	SCA_End,
	SCA_DeadEnd,
};

enum{
	SCA_FightActNo,				//�����܂ł͏��Ԃ���ꊷ���Ă̓_���I�I
	SCA_ItemActNo,				//
	SCA_PokeActNo,				//
	SCA_EscapeActNo,			//
	SCA_SafariActNo,			//
	SCA_BallActNo,				//
	SCA_CubeActNo,				//
	SCA_ApproachActNo,			//
	SCA_SafariEscapeActNo,		//
	SCA_GetDemoActNo,			//
	SCA_WazaSequenceNo,
	SCA_EndCheckNo,
	SCA_EndNo,
	SCA_DeadEndNo,
};

const pFunc ServerBranchTbl[] =
{
//	ServerPokemonMAppear,
	ServerTurnCheckInit,		//0
	ServerWin,					//1
	ServerLose,					//2
	ServerLose,					//3
	ServerEscape,				//4
	ServerEnd,					//5
	ServerEnemyEscape,			//6
	ServerEnd,					//7
	ServerEnd,					//8
};


//==============================================================
//                       ���C�����[�`��
//==============================================================

void	ServerDummy(void)
{
}

void	ServerInit(void)
{
	ServerCounterInit();
	ServerWork[1]=0;
	ServerSequence=ServerPokeParaGet;
}

#ifdef PM_DEBUG
#if 0
enum{
	DKC_COMMAND_INIT=0,
	DKC_COMMAND,
	DKC_COMMAND_WAZA,
	DKC_COMMAND_POKE,
	DKC_COMMAND_POKE_BACK_INIT,
	DKC_COMMAND_POKE_BACK,
	DKC_COMMAND_RUCK,
	DKC_COMMAND_ITEM,
	DKC_COMMAND_YES,
	DKC_COMMAND_ITEM_POKE,
	DKC_COMMAND_RUCK_BACK_INIT,
	DKC_COMMAND_RUCK_BACK,
	DKC_COMMAND_RUCK_BACK2,
};

#define	DKC_WAIT1	0x40
#define	DKC_WAIT2	0xc0

extern	void	ClientCommandSelect(void);

void	DebugKeyControl()
{
	if(MineEnemyCheck(ClientNo)!=SIDE_MINE)
		return;
	switch(UserWork[DEBUG_KEY_CONTROL0]){
		case DKC_COMMAND_INIT:
			if(ClientSequence[ClientNo]==ClientCommandSelect)
				UserWork[DEBUG_KEY_CONTROL0]++;
			else{
				if(FightType&FIGHT_TYPE_TUUSHIN){
					sys.Cont=0;
					sys.Trg=0;
				}
			}
			break;
		case DKC_COMMAND:
			switch(UserWork[DEBUG_KEY_CONTROL1]){
				case 0:
					sys.Cont=A_BUTTON;
					sys.Trg=A_BUTTON;
					UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_WAZA;
					UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT1;
					break;
				case 1:
					sys.Cont=D_KEY;
					sys.Trg=D_KEY;
					UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_POKE;
					UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT1;
					break;
				case 2:
					sys.Cont=R_KEY;
					sys.Trg=R_KEY;
					UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_RUCK;
					UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT1;
					break;
			}
			if((FightType&FIGHT_TYPE_TUUSHIN)==0){
				UserWork[DEBUG_KEY_CONTROL1]++;
				if(UserWork[DEBUG_KEY_CONTROL1]==3)
					UserWork[DEBUG_KEY_CONTROL1]=0;
			}
			else
				UserWork[DEBUG_KEY_CONTROL1]^=1;
			break;
		case DKC_COMMAND_WAZA:
			if(--UserWork[DEBUG_KEY_CONTROL2]==0){
				sys.Cont=A_BUTTON;
				sys.Trg=A_BUTTON;
				UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_INIT;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
		case DKC_COMMAND_POKE:
			if(--UserWork[DEBUG_KEY_CONTROL2]==0){
				sys.Cont=A_BUTTON;
				sys.Trg=A_BUTTON;
				UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_POKE_BACK_INIT;
				UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT2;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
		case DKC_COMMAND_POKE_BACK_INIT:
			if(--UserWork[DEBUG_KEY_CONTROL2]==0){
				sys.Cont=B_BUTTON;
				sys.Trg=B_BUTTON;
				UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_POKE_BACK;
				UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT2;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
		case DKC_COMMAND_POKE_BACK:
			if(--UserWork[DEBUG_KEY_CONTROL2]==0){
				sys.Cont=U_KEY;
				sys.Trg=U_KEY;
				UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_INIT;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
		case DKC_COMMAND_RUCK:
			if(--UserWork[DEBUG_KEY_CONTROL2]==0){
				sys.Cont=A_BUTTON;
				sys.Trg=A_BUTTON;
				UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_ITEM;
				UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT2;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
		case DKC_COMMAND_ITEM:
		case DKC_COMMAND_YES:
		case DKC_COMMAND_ITEM_POKE:
			if(--UserWork[DEBUG_KEY_CONTROL2]==0){
				sys.Cont=A_BUTTON;
				sys.Trg=A_BUTTON;
				UserWork[DEBUG_KEY_CONTROL0]++;
				UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT1;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
		case DKC_COMMAND_RUCK_BACK_INIT:
			if(--UserWork[DEBUG_KEY_CONTROL2]==0){
				sys.Cont=B_BUTTON;
				sys.Trg=B_BUTTON;
				UserWork[DEBUG_KEY_CONTROL0]++;
				UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT2;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
		case DKC_COMMAND_RUCK_BACK:
			if(--UserWork[DEBUG_KEY_CONTROL2]==0){
				if(ClientSequence[ClientNo]!=ClientCommandSelect){
					UserWork[DEBUG_KEY_CONTROL0]--;
					UserWork[DEBUG_KEY_CONTROL2]=DKC_WAIT1;
				}
				else
					UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_RUCK_BACK2;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
		case DKC_COMMAND_RUCK_BACK2:
			if(ClientSequence[ClientNo]==ClientCommandSelect){
				sys.Cont=L_KEY;
				sys.Trg=L_KEY;
				UserWork[DEBUG_KEY_CONTROL0]=DKC_COMMAND_INIT;
			}
			else{
				sys.Cont=0;
				sys.Trg=0;
			}
			break;
	}
}
#endif
enum{
	DKC_COMMAND_INIT=0,
	DKC_COMMAND,
	DKC_COMMAND_ESCAPE_INIT,
	DKC_COMMAND_ESCAPE,
	DKC_COMMAND_ESCAPE_END,
};

#define	DKC_WAIT1	0x80

extern	void	ClientCommandSelect(void);

void	DebugKeyControl()
{
	if(MineEnemyCheck(ClientNo)!=SIDE_MINE)
		return;
	switch(UserWork[DEBUG_KEY_CONTROL0]){
		case DKC_COMMAND_INIT:
			if(ClientSequence[ClientNo]==ClientCommandSelect)
				UserWork[DEBUG_KEY_CONTROL0]++;
			break;
		case DKC_COMMAND:
			sys.Cont=A_BUTTON;
			sys.Trg=A_BUTTON;
			UserWork[DEBUG_KEY_CONTROL0]++;
			UserWork[DEBUG_KEY_CONTROL1]=DKC_WAIT1;
			break;
		case DKC_COMMAND_ESCAPE_INIT:
			if(--UserWork[DEBUG_KEY_CONTROL1]==0){
				sys.Cont=A_BUTTON;
				sys.Trg=A_BUTTON;
				UserWork[DEBUG_KEY_CONTROL0]++;
				UserWork[DEBUG_KEY_CONTROL1]=DKC_WAIT1;
			}
			break;
		case DKC_COMMAND_ESCAPE:
			if(--UserWork[DEBUG_KEY_CONTROL1]==0){
				sys.Cont=A_BUTTON;
				sys.Trg=A_BUTTON;
				UserWork[DEBUG_KEY_CONTROL0]++;
			}
			break;
		case DKC_COMMAND_ESCAPE_END:
			UserWork[DEBUG_KEY_CONTROL0]=0;
			break;
	}
}
#endif

void	ServerMain(void)
{
#ifdef PM_DEBUG
	if(DebugFightFlag&DEBUG_FIGHT_AUTO){
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++)
			DebugKeyControl();
		ServerSequence();
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++)
			ClientSequence[ClientNo]();
	}
	else{
		ServerSequence();
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++)
			ClientSequence[ClientNo]();
	}
#else
	ServerSequence();
	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++)
		ClientSequence[ClientNo]();
#endif
}

void	ServerCounterInit(void)
{
	int	i,j;
	u8	*adr;
	
	ClearOneTurnCount(0);
	ClearOneSelfTurnCount();

	for(i=0;i<CLIENT_MAX;i++){
		WazaKouka[i]=0;				//�Z���ʃt���O��������
		adr=(u8 *)&WKC[i];
		for(j=0;j<sizeof(WazaKoukaCount);j++)
			adr[j]=0;				//�Z���ʃJ�E���^������
		WKC[i].nekodamashicnt=2;
		ClientStatusFlag[i]=0;		//�N���C�A���g�X�e�[�^�X�t���O
		WazaNoUse[i]=0;				//1�^�[���O�Ɏg�p�����Z�i���o�[���[�N
		WazaNoHit[i]=0;				//1�^�[���O�Ƀq�b�g�����Z�i���o�[���[�N
		WazaTypeHit[i]=0;			//1�^�[���O�Ƀq�b�g�����Z�^�C�v���[�N
		WazaNoLast[i]=0;			//1�^�[���O�ɍŌ�Ɏg�p�����Z�i���o�[���[�N
		WazaNoAttackClient[i]=0xff;		//1�^�[���O�Ƀq�b�g���ꂽ�N���C�A���g
//		WazaNoDefenceClient[i]=0xff;	//1�^�[���O�Ƀq�b�g�����N���C�A���g
		WazaNoKeep[i]=0;			//���^�[���Â��Z�i���o�[���[�N
		WazaNoSketch[i]=0;			//�X�P�b�`�p�̋Z�i���o�[���[�N
		SAD_WORK->abirityflag[i]=0;	//����\�̓t���O��������
	}

	for(i=0;i<2;i++){
		SideCondition[i]=0;			//��ɂ�������ʃt���O��������
		adr=(u8 *)&SCC[i];
		for(j=0;j<sizeof(SideConditionCount);j++)
			adr[j]=0;
	}

	AttackClient=0;
	DefenceClient=0;

	FieldCondition=0;						//�V�C�𐳏��
	adr=(u8 *)&FCC;
	for(i=0;i<sizeof(FieldConditionCount);i++)
		adr[i]=0;					//�V��J�E���^�[�N���A

	ServerStatusFlag=0;				//2��2�p�̃X�e�[�^�X�t���O���N���A
	if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&(MyData.btl_eff_sw))
		ServerStatusFlag|=SERVER_STATUS_FLAG_EFF_ANM_OFF;

	UserWork[BTL_TYPE]=MyData.btl_type;

	WazaSeqLoopCount=0;				//���[�v���x���p�̃J�E���^���N���A
	WinLoseFlag=0;					//���������t���O�̏�����
	ClientBit=0;					//�N���C�A���g�r�b�g���N���A
	KobanCounter=0;					//�˂��ɂ��΂�J�E���^�[������
	PUSH_ADRS->PushAdrsCnt=0;		//�A�h���X�v�b�V���p�J�E���^�[������
	PUSH_SERVER_ADRS->PushServerAdrsCnt=0;	//�A�h���X�v�b�V���p�J�E���^�[������

	for(i=0;i<8;i++) ServerWork[i]=0;	//�T�[�o�p���[�N�̏�����

	MojiWaitCount=0;					//�����E�F�C�g�J�E���^�[�̏�����

	WazaDamage=0;					//�Z�_���[�W�N���A
	EncountEffectFlag=0;			//�G���J�E���g�G�t�F�N�g�p�̃t���O
	UserWork[WAZA_EFF_CNT]=0;		//�Z�G�t�F�N�g�p�J�E���^�̏�����
	UserWork[WAZA_EFF_CNT_TMP]=0;	//�Z�G�t�F�N�g�p�J�E���^�̏�����
	LevelUpBit=0;					//���x���A�b�v�����|�P�����̃r�b�g���[�N�N���A
	NoReshuffleClient=0;			//�|�P��������ւ��ł��Ȃ��N���C�A���g�i���o�[��
									//�r�b�g�Ŋi�[
	UserWork[ESCAPE_COUNT]=0;		//�������I��������
	UserWork[SAFARI_NEAR]=0;			//�T�t�@���Ń|�P�����ɂ����Â�����
	UserWork[SAFARI_CUBE]=0;			//�T�t�@���ŃL���[�u�𓊂�����
	UserWork[SAFARI_GET_COUNT]=PPD[PokeParaGet(&PokeParaEnemy[0],ID_monsno)].get_rate*100/255/5;
	//�T�t�@���p�|�P�����Q�b�g���e�[�u���̃J�E���^
//�{���́A�p�[�\�i�������������
	UserWork[SAFARI_ESCAPE_COUNT]=3;	//�T�t�@���p�|�P�����̓��𗦃e�[�u���̃J�E���^

	UserWork[JINGLE_FLAG]=0;		//�W���O���Ȃ炵��FLAG
	UserWork[OKANE2BAI]=1;			//���炦�邨����2�{�ɂȂ�
	
	for(i=0;i<8;i++){
		UserWork[OUMU_WAZANO+i]=0;		//�I�E���������p��WazaNo���[�N
		UserWork[RECYCLE_WORK+i]=0;		//���T�C�N���p�̃��[�N
		UserWork[KODAWARI_WAZANO+i]=0;	//�������n�`�}�L�p�̃��[�N
		UserWork[ITEMNO_STOCK+i]=0;		//�ǂ�ڂ��Ȃǂ̃A�C�e���i���o�[�ۑ����[�N
		UserWork[OUMU_HIT_WAZANO+i]=0;		//�I�E���������p��WazaNo���[�N
		UserWork[OUMU_HIT_WAZANO+8+i]=0;	//�I�E���������p��WazaNo���[�N
	}

	UserWork[AI_RESHUFFLE_SELNO+0]=6;	//����ւ�AI�őI�����ꂽ�i���o�[
	UserWork[AI_RESHUFFLE_SELNO+1]=6;	//����ւ�AI�őI�����ꂽ�i���o�[
	UserWork[EXP_SELMONS_BIT]=0;		//�o���l���Q�b�g�����r�b�g

	//TV���p���[�N�̏�����
	//�퓬���Ƀ{�[���𓊂�����(�{�[���̎�ނ͉��L���Q�Ɓj
	for(i=0;i<FTW_BALL_MAX;i++)	FTW.ThrowBall[i]=0;	
	FTW.FightTurn=0;			//�퓬�^�[����
	FTW.MineKizetsuCount=0;		//�|�P�������m���ɂȂ�����
	FTW.EnemyKizetsuCount=0;	//�퓬�œ|�����|�P�����̐�
	FTW.PokeReshuffleCount=0;	//�������|�P��������������
	FTW.ItemUseKaifukuCount=0;	//�A�C�e�����g�����񐔉񕜌n�i���������蓙�j
	FTW.ItemUseFukkatsu=0;		//�A�C�e�����g�����񐔕����i���񂫂̂����܂蓙�j
	FTW.DamageFlag=0;			//�_���[�W�t���O
	FTW.ThrowMasterBallFlag=0;	//�}�X�^�[�{�[���𓊂������̃t���O
	FTW.EnemyLastPokeNo=0;		//����̍Ō�̃|�P�����i���o�[
	FTW.MineLastWazaNo=0;		//�������Ō�Ɏg�����Z
	FTW.EnemyLastWazaNo=0;		//���肪�Ō�Ɏg�����Z
	FTW.MineLastPokeNo=0;		//�������Ō�ɏo���Ă����|�P�����i���o�[
	FTW.MineLastPokeNo2=0;		//�������Ō�ɏo���Ă����|�P�����i���o�[(2vs2)
	FTW.GetPokeNo=0;			//�߂܂����|�P�����̃|�P�����i���o�[

	for(i=0;i<10;i++){
		FTW.MineLastPokeNickname[i]=0;
						//�������Ō�ɏo���Ă����|�P�����̃j�b�N�l�[��
		FTW.MineLastPokeNickname2[i]=0;
						//�������Ō�ɏo���Ă����|�P�����̃j�b�N�l�[��)(2vs2)
		FTW.GetPokeNickname[i]=0;
						//�߂܂����|�P�����ɂ����j�b�N�l�[��
	}

#ifdef PM_DEBUG
	UserWork[WAZA_DEBUG_WORK]=0;	//�Z�f�o�b�O�p�̃��[�N
	UserWork[WAZA_DEBUG_WORK2]=0;	//�Z�f�o�b�O�p�̃��[�N
//	for(i=0;i<8;i++)
//		UserWork[YUBIWOHURU_DEBUG+i]=0;	//�Z�f�o�b�O�p�̃��[�N
#endif

}

//����ւ����̃��[�N�̏�����

void	ServerReshuffleWorkInit()
{
	int	i;
	u8	*adr;
	WazaKoukaCount	WKCTMP;

	WKCTMP=WKC[ClientNo];

	if(WTD[WazaNo].battleeffect!=127){
		//�o�g���^�b�`�ł͏��������Ȃ����[�N���w��
		for(i=0;i<8;i++){
			PSP[ClientNo].abiritycnt[i]=6;
		}
		for(i=0;i<ClientSetMax;i++){
//			if(MineEnemyCheck(ClientNo)!=MineEnemyCheck(i)){
				//�u���낢�܂Ȃ����v�A�u�N���̂��v���������������ւ������A���ʂ�����
				if((PSP[i].condition2&CONDITION2_KUROIMANAZASHI)&&
				   (WKC[i].manazashiclientno==ClientNo))
					PSP[i].condition2&=CONDITION2_KUROIMANAZASHI_OFF;
//			}
			//���b�N�I�����������������ւ����������A���ʂ�����
			if((WazaKouka[i]&WAZAKOUKA_LOCKON)&&
			   (WKC[i].lockonclientno==ClientNo)){
				WazaKouka[i]&=WAZAKOUKA_LOCKON_OFF;
			    WKC[i].lockonclientno=0;
			}
		}
	}

	if(WTD[WazaNo].battleeffect==127){
		PSP[ClientNo].condition2&=CONDITION2_BATTON_TOUCH_ON;
		WazaKouka[ClientNo]&=WAZAKOUKA_BATTON_TOUCH_ON;
		for(i=0;i<ClientSetMax;i++){
			if(MineEnemyCheck(ClientNo)!=MineEnemyCheck(i)){
				//���b�N�I��������������o�g���^�b�`������A�J�E���^�����ɖ߂��Č��ʂ��p��
				if((WazaKouka[i]&WAZAKOUKA_LOCKON)&&
				   (WKC[i].lockonclientno==ClientNo)){
					WazaKouka[i]&=WAZAKOUKA_LOCKON_OFF;
					WazaKouka[i]|=(LOCKON_COUNT*2);
				}
			}
		}
	}
	else{
		PSP[ClientNo].condition2=0;
		WazaKouka[ClientNo]=0;
	}

	for(i=0;i<ClientSetMax;i++){
//		if(MineEnemyCheck(ClientNo)!=MineEnemyCheck(i)){
	//�u���������v���������������ւ������A���ʂ�����
			if(PSP[i].condition2&(no2bittbl[ClientNo]<<MEROMERO_SHIFT))
				PSP[i].condition2&=
					((no2bittbl[ClientNo]<<MEROMERO_SHIFT)^0xffffffff);
	//���ߌn�̋Z���������������ւ������A���ʂ�����
			if((PSP[i].condition2&CONDITION2_SHIME)&&
			   (UserWork[SHIME_WAZACLIENT+i]==ClientNo))
				PSP[i].condition2&=CONDITION2_SHIME_OFF;
//		}
	}

	CommandCsrPos[ClientNo]=0;
	WazaCsrPos[ClientNo]=0;

	adr=(u8 *)&WKC[ClientNo];
	for(i=0;i<sizeof(WazaKoukaCount);i++)
		adr[i]=0;
	if(WTD[WazaNo].battleeffect==127){
		WKC[ClientNo].migawarihp=WKCTMP.migawarihp;
		WKC[ClientNo].lockonclientno=WKCTMP.lockonclientno;
		WKC[ClientNo].horobinoutacnt=WKCTMP.horobinoutacnt;
		WKC[ClientNo].horobinoutacnttmp=WKCTMP.horobinoutacnttmp;
	}
	WKC[ClientNo].nekodamashicnt=2;		//�l�R���܂��J�E���^������
	WazaNoUse[ClientNo]=0;
	WazaNoHit[ClientNo]=0;
	WazaTypeHit[ClientNo]=0;
	WazaNoLast[ClientNo]=0;
	WazaNoSketch[ClientNo]=0;			//�X�P�b�`�p�̋Z�i���o�[���[�N
	WazaNoAttackClient[ClientNo]=0xff;
//	WazaNoDefenceClient[ClientNo]=0xff;
	UserWork[OUMU_WAZANO+0+ClientNo*2]=0;
	UserWork[OUMU_WAZANO+1+ClientNo*2]=0;
	UserWork[OUMU_HIT_WAZANO+0+ClientNo*4]=0;
	UserWork[OUMU_HIT_WAZANO+1+ClientNo*4]=0;
	UserWork[OUMU_HIT_WAZANO+2+ClientNo*4]=0;
	UserWork[OUMU_HIT_WAZANO+3+ClientNo*4]=0;
	UserWork[KODAWARI_WAZANO+ClientNo*2]=0;
	UserWork[KODAWARI_WAZANO+ClientNo*2+1]=0;
	SAD_WORK->abirityflag[ClientNo]=0;
//	BattlePokeAnmNo[ClientNo]=0;		//���V�C�|�P�����p�̃A�j���[�V�����i���o�[������
	//������x�S�Ń`�F�b�N��ʂ��悤�ɂ���
//	UserWork[WKC_WORK0]=WKC_ZENMETSU_CHECK_INIT;
	WazaNo=0;					//�o�g���^�b�`���ꂽ�̂��C�₵���Ƃ���
								//�ēx�o�g���^�b�`��������Ȃ����߂̏�����
}

void	ServerKizetsuWorkInit()
{
	int	i;
	u8	*adr;

	for(i=0;i<8;i++)
		PSP[ClientNo].abiritycnt[i]=6;

	PSP[ClientNo].condition2=0;
	WazaKouka[ClientNo]=0;

	//�u���낢�܂Ȃ����v�A�u�N���̂��v������������C�₵����A���ʂ�����
	for(i=0;i<ClientSetMax;i++){
//		if(MineEnemyCheck(ClientNo)!=MineEnemyCheck(i)){
			if((PSP[i].condition2&CONDITION2_KUROIMANAZASHI)&&
			   (WKC[i].manazashiclientno==ClientNo))
				PSP[i].condition2&=CONDITION2_KUROIMANAZASHI_OFF;
	//�u���������v���������������ւ������A���ʂ�����
			if(PSP[i].condition2&(no2bittbl[ClientNo]<<MEROMERO_SHIFT))
				PSP[i].condition2&=
					((no2bittbl[ClientNo]<<MEROMERO_SHIFT)^0xffffffff);
	//���ߌn�̋Z���������������ւ������A���ʂ�����
			if((PSP[i].condition2&CONDITION2_SHIME)&&
			   (UserWork[SHIME_WAZACLIENT+i]==ClientNo))
				PSP[i].condition2&=CONDITION2_SHIME_OFF;
//		}
	}

	CommandCsrPos[ClientNo]=0;
	WazaCsrPos[ClientNo]=0;

	//���̃^�[���ɂ������Ă������ʌn��S�ď�����
	adr=(u8 *)&WKC[ClientNo];
	for(i=0;i<sizeof(WazaKoukaCount);i++)
		adr[i]=0;

//�������͂��Ƃ��ƁA�悱�ǂ�΍􂾂���܂�������������o���āA�N���A
#if 0
	adr=(u8 *)&OTF[ClientNo];
	for(i=0;i<sizeof(OneTurnFlag);i++)
		adr[i]=0;
#endif
	OTF[ClientNo].guardflag=0;
	OTF[ClientNo].koraeruflag=0;
	OTF[ClientNo].waruagakiflag=0;
	OTF[ClientNo].tedasukeflag=0;
	OTF[ClientNo].magiccortflag=0;
	OTF[ClientNo].yokodoriflag=0;
	OTF[ClientNo].michidureflag=0;		//�݂��Â�
	OTF[ClientNo].mahiflag=0;			//�܂ЂōU���ł��Ȃ�����
	OTF[ClientNo].konranflag=0;			//������Ŏ������U��
	OTF[ClientNo].koukanaiflag=0;		//���������Ȃ�����
	OTF[ClientNo].tamewazaflag=0;		//���ߌn�̋Z�̂��߃^�[��
	OTF[ClientNo].itemescapeflag=0;		//�A�C�e���œ�����
	OTF[ClientNo].huuinflag=0;			//�ӂ�����ŋZ���o���Ȃ�����
	OTF[ClientNo].meromeroflag=0;		//���������ŋZ���o���Ȃ�����
	OTF[ClientNo].kanashibariflag=0;	//���Ȃ��΂�ŋZ���o���Ȃ�����
	OTF[ClientNo].chouhatsuflag=0;		//���傤�͂ŋZ���o���Ȃ�����
	OTF[ClientNo].ichamonflag=0;		//���������ŋZ���o���Ȃ�����
	OTF[ClientNo].hirumuflag=0;			//�Ђ��ŋZ���o���Ȃ�����
	OTF[ClientNo].ppdecflag=0;			//PP_DEC��ʂ���

//�������͂ǂ����A�C���ɂ͂��������邩�珉�������Ȃ��Ă悢
#if 0
	adr=(u8 *)&OSTF[ClientNo];
	for(i=0;i<sizeof(OneSelfTurnFlag);i++)
		adr[i]=0;
#endif

	WKC[ClientNo].nekodamashicnt=2;		//�l�R���܂��J�E���^������
	WazaNoUse[ClientNo]=0;
	WazaNoHit[ClientNo]=0;
	WazaTypeHit[ClientNo]=0;
	WazaNoLast[ClientNo]=0;
	WazaNoSketch[ClientNo]=0;			//�X�P�b�`�p�̋Z�i���o�[���[�N
	WazaNoAttackClient[ClientNo]=0xff;
//	WazaNoDefenceClient[ClientNo]=0xff;
	UserWork[KODAWARI_WAZANO+ClientNo*2]=0;
	UserWork[KODAWARI_WAZANO+ClientNo*2+1]=0;
	UserWork[OUMU_WAZANO+0+ClientNo*2]=0;
	UserWork[OUMU_WAZANO+1+ClientNo*2]=0;
	UserWork[OUMU_HIT_WAZANO+0+ClientNo*4]=0;
	UserWork[OUMU_HIT_WAZANO+1+ClientNo*4]=0;
	UserWork[OUMU_HIT_WAZANO+2+ClientNo*4]=0;
	UserWork[OUMU_HIT_WAZANO+3+ClientNo*4]=0;
	SAD_WORK->abirityflag[ClientNo]=0;
	PSP[ClientNo].type1=PPD[PSP[ClientNo].monsno].type1;
	PSP[ClientNo].type2=PPD[PSP[ClientNo].monsno].type2;
}

void	ServerPokeParaGet(void)
{
//�w�肵���|�P�����f�[�^���擾����B
	switch(ServerWork[0]){
		case 0:
			ClientNo=ServerWork[1];
			BSS_POKEPARA_GET_SET(BSS_SERVER,BSS_PARA_FIGHT,BSS_SELMONSNO);
			ClientBitSet(ClientNo);
			ServerWork[0]++;
			break;
		case 1:
			if(ClientBit==0){
				if(++ServerWork[1]==ClientSetMax)
					ServerSequence=ServerEncountEffect;
				else
					ServerWork[0]=0;
			}
	}
}

void	ServerEncountEffect(void)
{
	if(ClientBit==0){
		ClientNo=ClientNoGet(BSS_CLIENT_MINE);
		BSS_ENCOUNT_EFFECT_SET(BSS_SERVER,GroundNo);
		ClientBitSet(ClientNo);
		ServerSequence=ServerResWait;
		ServerWork[0]=0;
		ServerWork[1]=0;
	}
}

void	ServerResWait(void)
{
	int	i;
	u8	*adr;
	u16	*hp;

	if(ClientBit==0){
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
			if((FightType&FIGHT_TYPE_SAFARI)&&(MineEnemyCheck(ClientNo)==SIDE_MINE)){
				adr=(u8 *)&PSP[ClientNo];
				for(i=0;i<sizeof(PokemonServerParam);i++)
					adr[i]=0;
			}
			else{
				adr=(u8 *)&PSP[ClientNo];
				for(i=0;i<sizeof(PokemonServerParam);i++)
					adr[i]=ClientBuffer[ClientNo][i+4];
				PSP[ClientNo].type1=PPD[PSP[ClientNo].monsno].type1;
				PSP[ClientNo].type2=PPD[PSP[ClientNo].monsno].type2;
				PSP[ClientNo].speabino=
					SpecialAbirityGet(PSP[ClientNo].monsno,PSP[ClientNo].speabi);
				hp=(u16 *)&UserWork[MONS_HP_TEMP+(MineEnemyCheck(ClientNo)*2)];
				hp[0]=PSP[ClientNo].hp;
				//���ۂ̓o�g���^�b�`�Ȃǂ̋Z�̎��̃`�F�b�N������
				for(i=0;i<8;i++){
					PSP[ClientNo].abiritycnt[i]	=6;
				}
				PSP[ClientNo].condition2=0;
			}
			if(ClientTypeGet(ClientNo)==BSS_CLIENT_MINE){
				BSS_TRAINER_ENCOUNT_SET(BSS_SERVER);
				ClientBitSet(ClientNo);
			}
			if(FightType&FIGHT_TYPE_TRAINER){
				if(ClientTypeGet(ClientNo)==BSS_CLIENT_ENEMY){
					BSS_TRAINER_ENCOUNT_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
				if((MineEnemyCheck(ClientNo)==SIDE_ENEMY)&&
				  ((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
				  ((FightType&FIGHT_TYPE_CARD_E)==0)&&
				  ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0))
					//�}�ӂɌ����t���O�Z�b�g
					ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[ClientNo].monsno),Z_SEE_SET);
			}
			else{
				if((MineEnemyCheck(ClientNo)==SIDE_ENEMY)&&
				  ((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
				  ((FightType&FIGHT_TYPE_CARD_E)==0)&&
				  ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0)){
					//�}�ӂɌ����t���O�Z�b�g
					ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[ClientNo].monsno),Z_SEE_SET);
					BSS_POKE_ENCOUNT_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
			}
			if(FightType&FIGHT_TYPE_4_2vs2){
				if((ClientTypeGet(ClientNo)==BSS_CLIENT_MINE2)||
				   (ClientTypeGet(ClientNo)==BSS_CLIENT_ENEMY2)){
					BSS_TRAINER_ENCOUNT_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
			}
		}
		ServerSequence=ServerEncountMessage;
	}		
}

void	ServerEncountMessage(void)
{
	int	i;
	BallGaugePara	BGP[6];

	if(ClientBit==0){
		if(FightType&FIGHT_TYPE_TRAINER){
//			ServerSequence=ServerPokemonEAppearMsg;
			for(i=0;i<6;i++){
				if((PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)==0)||
				   (PokeParaGet(&PokeParaEnemy[i],ID_monsno_egg)==MONSNO_TAMAGO)){
					BGP[i].hp=0xffff;
					BGP[i].condition=0;
				}
				else{
					BGP[i].hp=PokeParaGet(&PokeParaEnemy[i],ID_hp);
					BGP[i].condition=PokeParaGet(&PokeParaEnemy[i],ID_condition);
				}
			}
			ClientNo=ClientNoGet(BSS_CLIENT_ENEMY);
			BSS_BALL_GAUGE_SET_SET(BSS_SERVER,(u8 *)&BGP,0x80);
			ClientBitSet(ClientNo);

			for(i=0;i<6;i++){
				if((PokeParaGet(&PokeParaMine[i],ID_monsno_egg)==0)||
				   (PokeParaGet(&PokeParaMine[i],ID_monsno_egg)==MONSNO_TAMAGO)){
					BGP[i].hp=0xffff;
					BGP[i].condition=0;
				}
				else{
					BGP[i].hp=PokeParaGet(&PokeParaMine[i],ID_hp);
					BGP[i].condition=PokeParaGet(&PokeParaMine[i],ID_condition);
				}
			}
			ClientNo=ClientNoGet(BSS_CLIENT_MINE);
			BSS_BALL_GAUGE_SET_SET(BSS_SERVER,(u8 *)&BGP,0x80);
			ClientBitSet(ClientNo);
			ServerSequence=ServerBallGaugeSet;
		}
		else{
//			ServerSequence=ServerPokemonMAppearMsg;
//			FightMsgServerSet(ENCOUNTMSGNO,0);
			for(i=0;i<6;i++){
				if((PokeParaGet(&PokeParaMine[i],ID_monsno_egg)==0)||
				   (PokeParaGet(&PokeParaMine[i],ID_monsno_egg)==MONSNO_TAMAGO)){
					BGP[i].hp=0xffff;
					BGP[i].condition=0;
				}
				else{
					BGP[i].hp=PokeParaGet(&PokeParaMine[i],ID_hp);
					BGP[i].condition=PokeParaGet(&PokeParaMine[i],ID_condition);
				}
			}
#if 0
			ClientNo=ClientNoGet(BSS_CLIENT_MINE);
			BSS_BALL_GAUGE_SET_SET(BSS_SERVER,(u8 *)&BGP,0);
			ClientBitSet(ClientNo);
#endif
			ServerSequence=ServerBallGaugeMSet;
		}
	}
}

void	ServerBallGaugeSet(void)
{
//	int	i;
//	BallGaugePara	BGP[6];

	if(ClientBit==0){
#if 0
		for(i=0;i<6;i++){
			if(PokeParaGet(&PokeParaMine[i],ID_monsno)==0){
				BGP[i].hp=0xffff;
				BGP[i].condition=0;
			}
			else{
				BGP[i].hp=PokeParaGet(&PokeParaMine[i],ID_hp);
				BGP[i].condition=PokeParaGet(&PokeParaMine[i],ID_condition);
			}
		}
		ClientNo=ClientNoGet(BSS_CLIENT_MINE);
		BSS_BALL_GAUGE_SET_SET(BSS_SERVER,(u8 *)&BGP,0);
		ClientBitSet(ClientNo);
#endif
		ClientNo=ClientNoGet(BSS_CLIENT_ENEMY);
		FightMsgServerSet(ENCOUNTMSGNO,ClientNo);
		ServerSequence=ServerPokemonEAppearMsg;
	}
}

void	ServerBallGaugeMSet(void)
{
	if(ClientBit==0){
		ServerSequence=ServerPokemonMAppearMsg;
		FightMsgServerSet(ENCOUNTMSGNO,0);
	}
}

void	ServerPokemonEAppearMsg(void)
{
	if(ClientBit==0){
		FightMsgServerSet(APPEARMSGNO,ClientNoGet(BSS_CLIENT_ENEMY));
		ServerSequence=ServerTrainerReturnE;
	}
}


void	ServerTrainerReturnE(void)
{
	if(ClientBit==0){
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
			if(ClientTypeGet(ClientNo)==BSS_CLIENT_ENEMY){
				BSS_POKE_SENDOUT_SET(BSS_SERVER);
				ClientBitSet(ClientNo);
			}
			if((FightType&FIGHT_TYPE_4_2vs2)&&(ClientTypeGet(ClientNo)==BSS_CLIENT_ENEMY2)){
				BSS_POKE_SENDOUT_SET(BSS_SERVER);
				ClientBitSet(ClientNo);
			}
		}
		ServerSequence=ServerTrainerReturnEWait;
	}
}

void	ServerTrainerReturnEWait(void)
{
	if(ClientBit==0){
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
			if(MineEnemyCheck(ClientNo)==SIDE_ENEMY){
				//�}�ӂɌ����t���O�Z�b�g
				if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
				   ((FightType&FIGHT_TYPE_CARD_E)==0)&&
				   ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0))
					ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[ClientNo].monsno),Z_SEE_SET);
			}
		}
		ServerSequence=ServerPokemonMAppearMsg;
	}
}

void	ServerPokemonEAppear(void)
{
	if(ClientBit==0){
		ServerSequence=ServerPokemonMAppearMsg;
	}
}

void	ServerPokemonMAppearMsg(void)
{
	if(ClientBit==0){
		if((FightType&FIGHT_TYPE_NO_APPEAR_MSG)==0)
			FightMsgServerSet(APPEARMSGNO,ClientNoGet(BSS_CLIENT_MINE));
		ServerSequence=ServerTrainerReturnM;
	}
}

void	ServerTrainerReturnM(void)
{
	if(ClientBit==0){
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
			if(ClientTypeGet(ClientNo)==BSS_CLIENT_MINE){
				BSS_POKE_SENDOUT_SET(BSS_SERVER);
				ClientBitSet(ClientNo);
			}
			if((FightType&FIGHT_TYPE_4_2vs2)&&(ClientTypeGet(ClientNo)==BSS_CLIENT_MINE2)){
				BSS_POKE_SENDOUT_SET(BSS_SERVER);
				ClientBitSet(ClientNo);
			}
		}
		UserWork[SPEABI_CLIENTNO]=0;
		UserWork[SOUBI_CLIENTNO]=0;
		UserWork[TENKOU_CHECK_FLAG]=0;
		ServerSequence=ServerSpecialAbirityCheck;
	}
}

void	ServerTrainerReturnMWait(void)
{
	if(ClientBit==0){
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
			if(MineEnemyCheck(ClientNo)==SIDE_MINE){
				BSS_POKE_APPEAR_SET(BSS_SERVER,SelMonsNo[ClientNo],APPEAR_NORMAL);
				ClientBitSet(ClientNo);
			}
		}
		UserWork[SPEABI_CLIENTNO]=0;
		UserWork[SOUBI_CLIENTNO]=0;
		UserWork[TENKOU_CHECK_FLAG]=0;
		ServerSequence=ServerSpecialAbirityCheck;
	}
}

void	ServerSpecialAbirityCheck(void)
{
	int	i,j;
	u8	ret=0;

	if(ClientBit==0){
		if(UserWork[SPEABI_CLIENTNO]==0){
			for(i=0;i<ClientSetMax;i++)
					ActionClientNo[i]=i;
			for(i=0;i<ClientSetMax-1;i++){
				for(j=i+1;j<ClientSetMax;j++){
					if(AgiCalc(ActionClientNo[i],ActionClientNo[j],1))
						SortAct(i,j);
				}
			}
		}
		if(UserWork[TENKOU_CHECK_FLAG]==0){
			if(SpecialAbirityCheck(SPEABI_POKEAPPEAR,0,0,TOKUSYU_TENKOU,0)){
				UserWork[TENKOU_CHECK_FLAG]=1;
				return;
			}
		}
		while(UserWork[SPEABI_CLIENTNO]<ClientSetMax){
			i=ActionClientNo[UserWork[SPEABI_CLIENTNO]];
			if(SpecialAbirityCheck(SPEABI_POKEAPPEAR,i,0,0,0))
				ret++;
			UserWork[SPEABI_CLIENTNO]++;
			if(ret) return;
		}

		if(SpecialAbirityCheck(SPEABI_IKAKU,0,0,0,0))
			return;
		if(SpecialAbirityCheck(SPEABI_TRACE,0,0,0,0))
			return;

		while(UserWork[SOUBI_CLIENTNO]<ClientSetMax){
			i=ActionClientNo[UserWork[SOUBI_CLIENTNO]];
			if(SoubiItemCheck(SOUBI_POKEAPPEAR,i,0))
				ret++;
			UserWork[SOUBI_CLIENTNO]++;
			if(ret) return;
		}

		for(i=0;i<ClientSetMax;i++){
		}
		for(i=0;i<CLIENT_MAX;i++){
			UserWork[AFTER_RESHUFFLE_SELMONSNO+i]=6;
			CommandReturn[i]=0xff;
			SelectWazaNo[i]=0;
		}
		ClearOneTurnCount(0);
		ClearOneSelfTurnCount();
		UserWork[NO_RESHUFFLE_CLIENT]=NoReshuffleClient;
		ServerSequence=ServerCommandSelectInit;
		FightJoinFlagInit();
		for(i=0;i<8;i++) ServerWork[i]=0;
		for(i=0;i<ClientSetMax;i++){
			PSP[i].condition2&=CONDITION2_HIRUMU_OFF;	//�Ђ��
		}
		UserWork[SCC_WORK0]=0;
		UserWork[SCC_WORK1]=0;
		UserWork[SCC2_WORK0]=0;
		UserWork[SCC2_WORK1]=0;
		UserWork[WKC_WORK0]=0;
		UserWork[GEC_WORK0]=0;			//�o���l�Q�b�g�`�F�b�N�p�V�[�P���X�i���o�[�̏�����
		UserWork[FCC_WORK0]=0;
		WazaStatusFlag=0;				//�Z�X�e�[�^�X�t���O��������
		AgiRnd=pp_rand();
	}
}

void	ServerTurnCheckInit(void)
{
	int	i;

	if(ClientBit==0){
		ServerSequence=ServerTurnCheck;
		for(i=0;i<8;i++) ServerWork[i]=0;
		for(i=0;i<ClientSetMax;i++){
			PSP[i].condition2&=CONDITION2_HIRUMU_OFF;	//�Ђ��
			//�����Ă���Ƃ��ɂ��ߋZ�n���o�����Ƃ��́A���߂��͂����i�˂��Ƒ΍�j
			if((PSP[i].condition&CONDITION_NEMURI)&&(PSP[i].condition2&CONDITION2_KEEP))
//				PSP[i].condition2&=CONDITION2_KEEP_OFF;
				ServerKeepOff(i);
		}
		UserWork[SCC_WORK0]=0;
		UserWork[SCC_WORK1]=0;
		UserWork[SCC2_WORK0]=0;
		UserWork[SCC2_WORK1]=0;
		UserWork[FCC_WORK0]=0;
		WazaStatusFlag=0;				//�Z�X�e�[�^�X�t���O��������
//		if(SpecialAbirityCheck(SPEABI_IKAKU,0,0,0,0))
//			return;
	}
}


void	ServerTurnCheck(void)
{
	int	i;

	ClearOneTurnCount(1);				//�܂���A���炦�邾���N���A

	if(WinLoseFlag==0){
		if(FieldConditionCheck()) return;
		if(SideConditionCheck()) return;
	}
//	UserWork[WKC_WORK0]=0;			//�Z���ʃ`�F�b�N�p�V�[�P���X�i���o�[�̏�����

	if(GetExpCheck()) return;
	UserWork[GEC_WORK0]=0;			//�o���l�Q�b�g�`�F�b�N�p�V�[�P���X�i���o�[�̏�����

	if(SideConditionCheck2()) return;

	ClearOneTurnCount(0);

	//�����ɓ���Ă݂�
	ServerStatusFlag&=SERVER_STATUS_FLAG_DOUBLE_CHECK_OFF;
	ServerStatusFlag&=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK_OFF;
	ServerStatusFlag&=SERVER_STATUS_FLAG_MINE_KIZETSU_OFF;
	ServerStatusFlag&=SERVER_STATUS_FLAG_TURNEND_OFF;
	//===============

	UserWork[WAZA_EFF_CNT]=0;		//�Z�G�t�F�N�g�p�J�E���^�̏�����
	UserWork[WAZA_EFF_CNT_TMP]=0;	//�Z�G�t�F�N�g�p�J�E���^�̏�����
	UserWork[WKC_WORK0]=0;			//�Z���ʃ`�F�b�N�p�V�[�P���X�i���o�[�̏�����
//	UserWork[GEC_WORK0]=0;			//�o���l�Q�b�g�`�F�b�N�p�V�[�P���X�i���o�[�̏�����
	WazaDamage=0;					//�Z�_���[�W�N���A
	WazaStatusFlag=0;				//�Z�X�e�[�^�X�t���O��������

	for(i=0;i<5;i++) ServerWork[i]=0;

	if(WinLoseFlag!=0){
		ActionSeqNo=SCA_EndNo;
		ServerSequence=ServerCommandAct;
	}
	else{
		if(FTW.FightTurn<0xff)			//�퓬�^�[����
			FTW.FightTurn++;			//�퓬�^�[����
		for(i=0;i<ClientSetMax;i++){
			CommandReturn[i]=0xff;
			SelectWazaNo[i]=0;
		}
		for(i=0;i<CLIENT_MAX;i++)
			UserWork[AFTER_RESHUFFLE_SELMONSNO+i]=6;
		UserWork[NO_RESHUFFLE_CLIENT]=NoReshuffleClient;
		ServerSequence=ServerCommandSelectInit;
		AgiRnd=pp_rand();
	}
}

u8	EscapeNGCheck(void)
{
	int	i;
	u8	dir;
	u8	eqp;

	if(PSP[ClientNo].item==ITEM_NAZONOMI)
//		eqp=Fld.RamSeed.item_equip;
		eqp=FRS[ClientNo].item_equip;
	else
		eqp=ItemEquipGet(PSP[ClientNo].item);

	ItemClient=ClientNo;

	//�A�C�e���ŕK����������̑������ʂ�100%�������
	if((eqp==SOUBI_KANARAZUNIGERARERU)||
	//�ʐM�ΐ�͕K���������̂Ń`�F�b�N���Ȃ�
	   (FightType&FIGHT_TYPE_TUUSHIN)||
	//����\�͂ɂ������͕K���������̂Ń`�F�b�N���Ȃ�
	   (PSP[ClientNo].speabino==TOKUSYU_NIGEASI))
		return 0;

	dir=MineEnemyCheck(ClientNo);

	for(i=0;i<ClientSetMax;i++){
		if((dir!=MineEnemyCheck(i))&&
		   (PSP[i].speabino==TOKUSYU_KAGEHUMI)){
			UserWork[CLIENT_NO_WORK]=i;
			SpeabiNo=PSP[i].speabino;
			ServerWork[5]=2;
			return 2;
		}
		if((dir!=MineEnemyCheck(i))&&
		   (PSP[ClientNo].speabino!=TOKUSYU_HUYUU)&&
		  ((PSP[ClientNo].type1!=HIKOU_TYPE)&&
		   (PSP[ClientNo].type2!=HIKOU_TYPE))&&
		   (PSP[i].speabino==TOKUSYU_ARIZIGOKU)){
			UserWork[CLIENT_NO_WORK]=i;
			SpeabiNo=PSP[i].speabino;
			ServerWork[5]=2;
			return 2;
		}
	}
	if((i=SpecialAbirityCheck(SPEABI_HAVE_SPEABI_NOMINE,ClientNo,TOKUSYU_ZIRYOKU,0,0))&&
	  ((PSP[ClientNo].type1==METAL_TYPE)||
	   (PSP[ClientNo].type2==METAL_TYPE))){
		UserWork[CLIENT_NO_WORK]=i-1;
		SpeabiNo=PSP[i-1].speabino;
		ServerWork[5]=2;
		return 2;
	}
	if((PSP[ClientNo].condition2&
       (CONDITION2_SHIME|CONDITION2_KUROIMANAZASHI))||
	   (WazaKouka[ClientNo]&WAZAKOUKA_NEWOHARU)){
		ServerWork[5]=0;
		return 1;
	}
	if(FightType&FIGHT_TYPE_GUIDE){
		ServerWork[5]=1;
		return 1;
	}
	return	0;
}

void	ServerListRowSet(u8 clientno)
{
	int	i;
	u8	src_pos,dest_pos;

	for(i=0;i<3;i++)
		ListRow[i]=UserWork[SERVER_LIST_ROW+i+clientno*3];

	src_pos=SearchListRow(SelMonsNo[clientno]);
	dest_pos=SearchListRow(UserWork[AFTER_RESHUFFLE_SELMONSNO+clientno]);
	ChangeListRow(src_pos,dest_pos);

	if(FightType&FIGHT_TYPE_2vs2){
		for(i=0;i<3;i++){
			UserWork[SERVER_LIST_ROW+i+clientno*3]=ListRow[i];
			UserWork[SERVER_LIST_ROW+i+(clientno^2)*3]=ListRow[i];
		}
	}
	else{
		for(i=0;i<3;i++)
			UserWork[SERVER_LIST_ROW+i+clientno*3]=ListRow[i];
	}
}


enum{
	SCSI_COMMAND_INIT=0,
	SCSI_COMMAND_WAIT,
	SCSI_COMMAND_WAIT2,
	SCSI_COMMAND_TUUSHIN_WAIT,
	SCSI_COMMAND_END,
	SCSI_COMMAND_ALERT,
	SCSI_COMMAND_YURE_STOP_WAIT,
};

void	ServerCommandSelectInit(void)
{
	int	i;
	u8	clienttype;
	WazaList	WL;

	ServerWork[4]=0;
	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
		clienttype=ClientTypeGet(ClientNo);
		switch(ServerWork[ClientNo]){
			case SCSI_COMMAND_INIT:
				//�|�P�����I�������āA�L�����Z���Ŗ߂��Ă����Ƃ��̂��߂̏�����
				UserWork[AFTER_RESHUFFLE_SELMONSNO+ClientNo]=6;
				if((FightType&FIGHT_TYPE_4_2vs2)==0){
					if((clienttype&2)&&
					   ((UserWork[NO_RESHUFFLE_CLIENT]&no2bittbl[ClientNoGet(clienttype^2)])==0)&&
					   (ServerWork[ClientNoGet(clienttype^2)]!=SCSI_COMMAND_END))
					break;
				}
				//2vs2�Ń|�P�����C��
				if(UserWork[NO_RESHUFFLE_CLIENT]&no2bittbl[ClientNo]){
					CommandReturn[ClientNo]=SCA_DeadEndNo;
					if((FightType&FIGHT_TYPE_4_2vs2)==0)
						ServerWork[ClientNo]=SCSI_COMMAND_END;
					else
						ServerWork[ClientNo]=SCSI_COMMAND_TUUSHIN_WAIT;
				}
				else if((PSP[ClientNo].condition2&CONDITION2_KEEP)||
					    (PSP[ClientNo].condition2&CONDITION2_HANDOU)){
					CommandReturn[ClientNo]=BSS_SELECT_RETURN_FIGHT;
					ServerWork[ClientNo]=SCSI_COMMAND_TUUSHIN_WAIT;
					//���킢�ł���Ƃ�
					//���܂�
					//�́A�I���Ȃ�
				}
				else{
					BSS_COMMAND_SELECT_SET(BSS_SERVER,
										   CommandReturn[0],
										   ClientBuffer[0][1]|
										  (ClientBuffer[0][2]<<8));
					ClientBitSet(ClientNo);
					ServerWork[ClientNo]++;
				}
				break;
			case SCSI_COMMAND_WAIT:
				if((ClientBit&(no2bittbl[ClientNo]|CLIENT_BIT_TUUSHIN|
						      (no2bittbl[ClientNo]<<4)|
						      (no2bittbl[ClientNo]<<8)|
						      (no2bittbl[ClientNo]<<12)))==0){
					CommandReturn[ClientNo]=ClientBuffer[ClientNo][1];
					switch(ClientBuffer[ClientNo][1]){
						case	BSS_SELECT_RETURN_FIGHT:
							if(WaruagakiCheck()){
								ServerWork[ClientNo]=SCSI_COMMAND_ALERT;
								UserWork[ALERT_END+ClientNo]=0;
								UserWork[ALERT_RETURN_SEQ+ClientNo]=SCSI_COMMAND_TUUSHIN_WAIT;
								UserWork[DEFENCE_CLIENT+ClientNo]=ClientBuffer[ClientNo][3];
								return;
							}
							if(WKC[ClientNo].encorewazano!=0){
								SelectWazaNo[ClientNo]=WKC[ClientNo].encorewazano;
								ServerWork[ClientNo]=SCSI_COMMAND_TUUSHIN_WAIT;
								return;
							}
							WL.monsno=PSP[ClientNo].monsno;
							WL.type1=PSP[ClientNo].type1;
							WL.type2=PSP[ClientNo].type2;
							for(i=0;i<4;i++){
								WL.wazano[i]=PSP[ClientNo].waza[i];
								WL.pp[i]=PSP[ClientNo].pp[i];
								WL.ppmax[i]=PPMaxGet(PSP[ClientNo].waza[i],
													 PSP[ClientNo].pp_count,
													 i);
							}
							BSS_WAZA_SELECT_SET(BSS_SERVER,
									FightType&FIGHT_TYPE_2vs2,
									BSS_WAZA_SELECT_NORMAL,
									(u8 *)&WL);
							ClientBitSet(ClientNo);
							break;		
						case	BSS_SELECT_RETURN_ITEM:
							if(FightType&(FIGHT_TYPE_TUUSHIN|FIGHT_TYPE_BATTLE_TOWER|FIGHT_TYPE_CARD_E)){
								AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_DouguAlert;
								ServerWork[ClientNo]=SCSI_COMMAND_ALERT;
								UserWork[ALERT_END+ClientNo]=0;
								UserWork[ALERT_RETURN_SEQ+ClientNo]=
									SCSI_COMMAND_INIT;
								return;
							}
							else{
								BSS_ITEM_SELECT_SET(BSS_SERVER,
									(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
								ClientBitSet(ClientNo);
							}
							break;		
						case	BSS_SELECT_RETURN_POKE:
							UserWork[BEFORE_RESHUFFLE_SELMONSNO+ClientNo]=SelMonsNo[ClientNo];
							if((PSP[ClientNo].condition2&(CONDITION2_SHIME|CONDITION2_KUROIMANAZASHI))||
								(WazaKouka[ClientNo]&WAZAKOUKA_NEWOHARU))
								BSS_POKE_SELECT_SET(BSS_SERVER,
													PLT_NOT_CHANGE,
													6,
													0,
													(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
							else if((i=SpecialAbirityCheck(SPEABI_ENEMYHAVE_SPEABI,
													 ClientNo,
													 TOKUSYU_KAGEHUMI,0,0))||
								((i=SpecialAbirityCheck(SPEABI_ENEMYHAVE_SPEABI,
													 ClientNo,
													 TOKUSYU_ARIZIGOKU,0,0))&&
								 ((PSP[ClientNo].type1!=HIKOU_TYPE)&&
								  (PSP[ClientNo].type2!=HIKOU_TYPE)&&
								  (PSP[ClientNo].speabino!=TOKUSYU_HUYUU)))||
								((i=SpecialAbirityCheck(SPEABI_HAVE_SPEABI_NOMINE,
													 ClientNo,
													 TOKUSYU_ZIRYOKU,0,0))&&
								 ((PSP[ClientNo].type1==METAL_TYPE)||
								  (PSP[ClientNo].type2==METAL_TYPE))))
								BSS_POKE_SELECT_SET(BSS_SERVER,
													PLT_NOT_CHANGE_TOKUSEI|((i-1)<<4),
													6,
													SpeabiNo,
													(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
							else{
								if((ClientNo==2)&&
								   (CommandReturn[0]==BSS_SELECT_RETURN_POKE))
									BSS_POKE_SELECT_SET(BSS_SERVER,
														PLT_NORMAL,
														UserWork[AFTER_RESHUFFLE_SELMONSNO+0],
														0,
														(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
								else if((ClientNo==3)&&
								   (CommandReturn[1]==BSS_SELECT_RETURN_POKE))
									BSS_POKE_SELECT_SET(BSS_SERVER,
														PLT_NORMAL,
														UserWork[AFTER_RESHUFFLE_SELMONSNO+1],
														0,
														(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
								else
									BSS_POKE_SELECT_SET(BSS_SERVER,
														PLT_NORMAL,
														6,
														0,
														(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
							}
							ClientBitSet(ClientNo);
							break;		
						case	BSS_SELECT_RETURN_ESCAPE:
							break;		
						case	BSS_SELECT_RETURN_SAFARI:	//�T�t�@���]�[�����L�̃A�N�V����
							break;		
						case	BSS_SELECT_RETURN_BALL:		//�T�t�@���{�[���𓊂���
							if(BallThrowCheck()){
								AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_SafariBallThrowNG;
								ServerWork[ClientNo]=SCSI_COMMAND_ALERT;
								UserWork[ALERT_END+ClientNo]=0;
								UserWork[ALERT_RETURN_SEQ+ClientNo]=SCSI_COMMAND_INIT;
								return;
							}
							break;		
						case	BSS_SELECT_RETURN_CUBE:		//�L���[�u
							//�T�t�@���̂Ƃ��̓L���[�u�P�[�X���Ă�
							BSS_ITEM_SELECT_SET(BSS_SERVER,
												(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
							ClientBitSet(ClientNo);
							break;		
						case	BSS_SELECT_RETURN_APPROACH:	//�����Â�
							break;		
						case	BSS_SELECT_RETURN_SAFARI_ESCAPE:
							break;		
						case	BSS_SELECT_RETURN_GET_DEMO:
							break;		
						case	BSS_SELECT_RETURN_B_CANCEL:
//							ServerWork[ClientNo]=SCSI_COMMAND_INIT;
							ServerWork[ClientNo]=SCSI_COMMAND_YURE_STOP_WAIT;
							ServerWork[ClientNoGet(ClientTypeGet(ClientNo)^2)]=SCSI_COMMAND_INIT;
							BSS_GAUGE_YURE_STOP_SET(BSS_SERVER);
							ClientBitSet(ClientNo);
							return;
							break;		
					}
					if((FightType&FIGHT_TYPE_TRAINER)&&
					  ((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
					   (ClientBuffer[ClientNo][1]==BSS_SELECT_RETURN_ESCAPE)){
						ServerSubroutineSet((u8 *)&WAZA_TrainerEscape);
						ServerWork[ClientNo]=SCSI_COMMAND_INIT;
					}
					else if((EscapeNGCheck())&&
						(ClientBuffer[ClientNo][1]==BSS_SELECT_RETURN_ESCAPE)){
						AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_EscapeNG2;
						ServerWork[ClientNo]=SCSI_COMMAND_ALERT;
						UserWork[ALERT_END+ClientNo]=0;
						UserWork[ALERT_RETURN_SEQ+ClientNo]=SCSI_COMMAND_INIT;
						return;
					}
					else
						ServerWork[ClientNo]++;
				}
				break;
			case SCSI_COMMAND_WAIT2:
				if((ClientBit&(no2bittbl[ClientNo]|CLIENT_BIT_TUUSHIN|
						      (no2bittbl[ClientNo]<<4)|
						      (no2bittbl[ClientNo]<<8)|
						      (no2bittbl[ClientNo]<<12)))==0){
					switch(CommandReturn[ClientNo]){
						case	BSS_SELECT_RETURN_FIGHT:
							switch(ClientBuffer[ClientNo][1]){
								case BSS_SELECT_RETURN_ESCAPE:		//�ɂ���
								case BSS_SELECT_RETURN_SAFARI:		//�T�t�@���]�[�����L�̃A�N�V����
								case BSS_SELECT_RETURN_BALL:		//�T�t�@���{�[���𓊂���
								case BSS_SELECT_RETURN_CUBE:		//�L���[�u
								case BSS_SELECT_RETURN_APPROACH:	//�����Â�
								case BSS_SELECT_RETURN_SAFARI_ESCAPE:	//�ɂ���
								case BSS_SELECT_RETURN_GET_DEMO:
									CommandReturn[ClientNo]=ClientBuffer[ClientNo][1];
								return;
							}
							if((ClientBuffer[ClientNo][2]|(ClientBuffer[ClientNo][3]<<8))==0xffff)
								ServerWork[ClientNo]=SCSI_COMMAND_INIT;
							else{
								if(ReturnWazaNoCheck()){
									ServerWork[ClientNo]=SCSI_COMMAND_ALERT;
									UserWork[ALERT_END+ClientNo]=0;
									ClientBuffer[ClientNo][1]=BSS_SELECT_RETURN_FIGHT;
									UserWork[ALERT_RETURN_SEQ+ClientNo]=SCSI_COMMAND_WAIT;
									return;
								}
								UserWork[WAZA_POS_WORK+ClientNo]=ClientBuffer[ClientNo][2];
								SelectWazaNo[ClientNo]=
									PSP[ClientNo].waza[UserWork[WAZA_POS_WORK+ClientNo]];
								UserWork[DEFENCE_CLIENT+ClientNo]=ClientBuffer[ClientNo][3];
								ServerWork[ClientNo]++;
							}
							break;		
						case	BSS_SELECT_RETURN_ITEM:
							if((ClientBuffer[ClientNo][1]|(ClientBuffer[ClientNo][2]<<8))==0)
								ServerWork[ClientNo]=SCSI_COMMAND_INIT;
							else{
								ItemNo=ClientBuffer[ClientNo][1]|
									  (ClientBuffer[ClientNo][2]<<8);
								ServerWork[ClientNo]++;
							}
							break;		
						case	BSS_SELECT_RETURN_POKE:
							if(ClientBuffer[ClientNo][1]==6)
									ServerWork[ClientNo]=SCSI_COMMAND_INIT;
							else{
								UserWork[AFTER_RESHUFFLE_SELMONSNO+ClientNo]=
									ClientBuffer[ClientNo][1];
								if(FightType&FIGHT_TYPE_4_2vs2){
									UserWork[SERVER_LIST_ROW+ClientNo*3]&=0x0f;
									UserWork[SERVER_LIST_ROW+ClientNo*3]|=
										(ClientBuffer[ClientNo][2]&0xf0);
									UserWork[SERVER_LIST_ROW+1+ClientNo*3]=
										ClientBuffer[ClientNo][3];

									UserWork[SERVER_LIST_ROW+(ClientNo^2)*3]&=0xf0;
									UserWork[SERVER_LIST_ROW+(ClientNo^2)*3]|=
										((ClientBuffer[ClientNo][2]&0xf0)>>4);
									UserWork[SERVER_LIST_ROW+2+(ClientNo^2)*3]=
										ClientBuffer[ClientNo][3];
								}
								ServerWork[ClientNo]++;
							}
							break;		
						case	BSS_SELECT_RETURN_ESCAPE:
							ServerStatusFlag|=SERVER_STATUS_FLAG_ESCAPE;
							ServerWork[ClientNo]++;
							break;		
						case	BSS_SELECT_RETURN_SAFARI:	//�T�t�@���]�[�����L�̃A�N�V����
							ServerWork[ClientNo]++;
							break;		
						case	BSS_SELECT_RETURN_BALL:		//�T�t�@���{�[���𓊂���
							ServerWork[ClientNo]++;
							break;		
						case	BSS_SELECT_RETURN_CUBE:		//�L���[�u
							if((ClientBuffer[ClientNo][1]|(ClientBuffer[ClientNo][2]<<8))==0)
								ServerWork[ClientNo]=SCSI_COMMAND_INIT;
							else
								ServerWork[ClientNo]++;
							break;		
						case	BSS_SELECT_RETURN_APPROACH:	//�����Â�
							ServerWork[ClientNo]++;
							break;		
						case	BSS_SELECT_RETURN_SAFARI_ESCAPE:
							ServerStatusFlag|=SERVER_STATUS_FLAG_ESCAPE;
							ServerWork[ClientNo]++;
							break;		
						case BSS_SELECT_RETURN_GET_DEMO:
							ServerWork[ClientNo]++;
							break;		
					}
				}
				break;
			case SCSI_COMMAND_TUUSHIN_WAIT:
				if((ClientBit&(no2bittbl[ClientNo]|CLIENT_BIT_TUUSHIN|
						      (no2bittbl[ClientNo]<<4)|
						      (no2bittbl[ClientNo]<<8)|
						      (no2bittbl[ClientNo]<<12)))==0){
					if((FightType&FIGHT_TYPE_4_2vs2)||
					  ((FightType&FIGHT_TYPE_2vs2)==0)||
					   (clienttype&2))
						BSS_TUUSHIN_WAIT_SET(BSS_SERVER,0);
					else{
						if(((clienttype&2)==0)&&
						    (UserWork[NO_RESHUFFLE_CLIENT]&
						     no2bittbl[ClientNoGet(clienttype^2)]))
							BSS_TUUSHIN_WAIT_SET(BSS_SERVER,0);
						else
							BSS_TUUSHIN_WAIT_SET(BSS_SERVER,1);
					}
					ClientBitSet(ClientNo);
					ServerWork[ClientNo]++;
				}
				break;
			case SCSI_COMMAND_END:
				if((ClientBit&(no2bittbl[ClientNo]|CLIENT_BIT_TUUSHIN|
						      (no2bittbl[ClientNo]<<4)|
						      (no2bittbl[ClientNo]<<8)|
						      (no2bittbl[ClientNo]<<12)))==0)
					ServerWork[4]++;
				break;
			case SCSI_COMMAND_ALERT:
				if(UserWork[ALERT_END+ClientNo])
					ServerWork[ClientNo]=UserWork[ALERT_RETURN_SEQ+ClientNo];
				else{
					AttackClient=ClientNo;
					WazaSeqAdrs=AlertSeqAdrs[ClientNo];
					if((ClientBit&(no2bittbl[ClientNo]|CLIENT_BIT_TUUSHIN|
							      (no2bittbl[ClientNo]<<4)|
							      (no2bittbl[ClientNo]<<8)|
							      (no2bittbl[ClientNo]<<12)))==0)
						WazaSequenceTbl[*WazaSeqAdrs]();
					AlertSeqAdrs[ClientNo]=WazaSeqAdrs;
				}
				break;
			case SCSI_COMMAND_YURE_STOP_WAIT:
				if((ClientBit&(no2bittbl[ClientNo]|CLIENT_BIT_TUUSHIN|
						      (no2bittbl[ClientNo]<<4)|
						      (no2bittbl[ClientNo]<<8)|
						      (no2bittbl[ClientNo]<<12)))==0)
					ServerWork[ClientNo]=SCSI_COMMAND_INIT;
				break;
		}
	}
	if(ServerWork[4]==ClientSetMax){
		ServerSequence=ServerAgiCheck;
#if 0
		if((FightType&FIGHT_TYPE_4_2vs2)==0){
			for(i=0;i<ClientSetMax;i++){
				if(CommandReturn[i]==BSS_SELECT_RETURN_POKE)
					ServerListRowSet(i);
			}
		}
#endif
	}
}

void	SortAct(u8 i,u8 j)
{
	int	work;

	work=ActionNo[i];
	ActionNo[i]=ActionNo[j];
	ActionNo[j]=work;
	work=ActionClientNo[i];
	ActionClientNo[i]=ActionClientNo[j];
	ActionClientNo[j]=work;
}

u8	AgiCalc(u8 k,u8 l,u8 flg)
{
	int	agiofsk,agiofsl;
	u8	ret=0;
	u32	agik,agil;
	u16	wazanok,wazanol;
	u8	eqp;
	u8	atc;

	//����\�͂̂��Ă񂫂̃|�P�������퓬�ɏo�Ă���Ƃ��̓`�F�b�N�����Ȃ�
	if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_NOOTENKI,0,0)==0)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_EAROKKU,0,0)==0)){
		//����\�͂��������́A�J�̂Ƃ��f�������{
		//����\�͂悤��傭���́A����̂Ƃ��f�������{
		if(((PSP[k].speabino==TOKUSYU_SUISUI)&&
		    (FieldCondition&FIELD_CONDITION_AMEHURI))||
		   ((PSP[k].speabino==TOKUSYU_YOURYOKUSO)&&
		    (FieldCondition&FIELD_CONDITION_HIDERING)))
			agiofsk=2;
		else
			agiofsk=1;

		//����\�͂��������́A�J�̂Ƃ��f�������{
		//����\�͂悤��傭���́A����̂Ƃ��f�������{
		if(((PSP[l].speabino==TOKUSYU_SUISUI)&&
		    (FieldCondition&FIELD_CONDITION_AMEHURI))||
		   ((PSP[l].speabino==TOKUSYU_YOURYOKUSO)&&
		    (FieldCondition&FIELD_CONDITION_HIDERING)))
			agiofsl=2;
		else
			agiofsl=1;
	}
	else{
		agiofsk=1;
		agiofsl=1;
	}

	agik=(PSP[k].agi*agiofsk)*
		  CondChgTable[PSP[k].abiritycnt[COND_AGI]][0]/
		  CondChgTable[PSP[k].abiritycnt[COND_AGI]][1];

	if(PSP[k].item==ITEM_NAZONOMI){
//		eqp=Fld.RamSeed.item_equip;
//		atc=Fld.RamSeed.item_power;
		eqp=FRS[k].item_equip;
		atc=FRS[k].item_power;
	}
	else{
		eqp=ItemEquipGet(PSP[k].item);
		atc=ItemAttackGet(PSP[k].item);
	}

	//�_�C�i���o�b�W�������Ă���Ƃ��́A�f�������P�O���A�b�v
	if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
		(EventFlagCheck(SYS_BATCH03_GET))&&
		(MineEnemyCheck(k)==SIDE_MINE))
		agik=agik*110/100;

	//�A�C�e���œw�͒l�Q�{�̌��ʂ𑕔�����Ƒf������1/2�ɂ���
	if(eqp==SOUBI_DORYOKUTINIBAI)
		agik/=2;

	//�܂Ђ̎��́A�f������1/4�ɂ���
	if(PSP[k].condition&CONDITION_MAHI)
		agik/=4;

	//�A�C�e���Ő搧�U���̌��ʂ𑕔�����Ɛ搧�U���ɂ���
	if(eqp==SOUBI_SENSEIKOUGEKI){
		if(AgiRnd<(0xffff*atc/100))
			agik=0xffffffff;
	}

	agil=(PSP[l].agi*agiofsl)*
		  CondChgTable[PSP[l].abiritycnt[COND_AGI]][0]/
		  CondChgTable[PSP[l].abiritycnt[COND_AGI]][1];

	if(PSP[l].item==ITEM_NAZONOMI){
//		eqp=Fld.RamSeed.item_equip;
//		atc=Fld.RamSeed.item_power;
		eqp=FRS[l].item_equip;
		atc=FRS[l].item_power;
	}
	else{
		eqp=ItemEquipGet(PSP[l].item);
		atc=ItemAttackGet(PSP[l].item);
	}

	//�_�C�i���o�b�W�������Ă���Ƃ��́A�f�������P�O���A�b�v
	if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
		(EventFlagCheck(SYS_BATCH03_GET))&&
		(MineEnemyCheck(l)==SIDE_MINE))
		agil=agil*110/100;

	//�A�C�e���œw�͒l�Q�{�̌��ʂ𑕔�����Ƒf������1/2�ɂ���
	if(eqp==SOUBI_DORYOKUTINIBAI)
		agil/=2;

	//�܂Ђ̎��́A�f������1/4�ɂ���
	if(PSP[l].condition&CONDITION_MAHI)
		agil/=4;

	//�A�C�e���Ő搧�U���̌��ʂ𑕔�����Ɛ搧�U���ɂ���
	if(eqp==SOUBI_SENSEIKOUGEKI){
		if(AgiRnd<(0xffff*atc/100))
			agil=0xffffffff;
	}

	if(flg){
		wazanok=0;
		wazanol=0;
	}
	else{
		if(CommandReturn[k]==BSS_SELECT_RETURN_FIGHT){
			if(OTF[k].waruagakiflag)
				wazanok=WAZANO_WARUAGAKI;
			else
				wazanok=PSP[k].waza[UserWork[WAZA_POS_WORK+k]];
		}
		else
			wazanok=0;
		
		if(CommandReturn[l]==BSS_SELECT_RETURN_FIGHT){
			if(OTF[l].waruagakiflag)
				wazanol=WAZANO_WARUAGAKI;
			else
				wazanol=PSP[l].waza[UserWork[WAZA_POS_WORK+l]];
		}
		else
			wazanol=0;
	}

	if((WTD[wazanok].attackpri!=0)||
	   (WTD[wazanol].attackpri!=0)){
		if(WTD[wazanok].attackpri==
		   WTD[wazanol].attackpri){
			if((agik==agil)&&
			   (pp_rand()&1)){
				ret=2;
			}
			else if(agik<agil){
				ret=1;
			}
		}
		else if(WTD[wazanok].attackpri<
		        WTD[wazanol].attackpri){
			ret=1;
		}
	}
	else{
		if((agik==agil)&&
		   (pp_rand()&1)){
			ret=2;
		}
		else if(agik<agil){
			ret=1;
		}
	}
	return	ret;
}

#if 1
void	ServerAgiCheck(void)
{
	int	i,j,k,l;
	int	turn;
	int	ret;

//	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++)
//		SelectWazaNo[ClientNo]=0;

	turn=0;
	//�T�t�@���]�[���͕K����������U
	if(FightType&FIGHT_TYPE_SAFARI){
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){	
			ActionNo[turn]=CommandReturn[ClientNo];
			ActionClientNo[turn]=ClientNo;
			turn++;
		}
	}
	else{
		if(FightType&FIGHT_TYPE_TUUSHIN){
			for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){	
				if(CommandReturn[ClientNo]==BSS_SELECT_RETURN_ESCAPE){
					turn=5;
					break;
				}
			}
		}
		else{
			if(CommandReturn[0]==BSS_SELECT_RETURN_ESCAPE){
				ClientNo=0;
				turn=5;
			}
		}
		if(turn==5){
			ActionNo[0]=CommandReturn[ClientNo];
			ActionClientNo[0]=ClientNo;
			turn=1;
			for(i=0;i<ClientSetMax;i++){	
				if(i!=ClientNo){
					ActionNo[turn]=CommandReturn[i];
					ActionClientNo[turn]=i;
#if 0
					if(CommandReturn[i]==BSS_SELECT_RETURN_FIGHT){
						UserWork[WAZA_POS_WORK+i]=ClientBuffer[i][2];
						SelectWazaNo[i]=
							PSP[ClientNo].waza[UserWork[WAZA_POS_WORK+i]];
					}
#endif
					turn++;
				}
			}
			ServerSequence=ServerWazaBeforeMsg;
			UserWork[WAZA_BEFORE_MSG]=0;	//ServerWazaBeforeMsg�pClientNo�i�[���[�N
			return;
		}

		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){	
												//�Z�I���Ɠ�����ȊO�𒊏o
												//�Z�Ƃɂ���ȊO�͖������ɐ�U
			if((CommandReturn[ClientNo]==BSS_SELECT_RETURN_ITEM)||
			   (CommandReturn[ClientNo]==BSS_SELECT_RETURN_POKE)){
				ActionNo[turn]=CommandReturn[ClientNo];
				ActionClientNo[turn]=ClientNo;
				turn++;
			}
		}
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){	
												//�Z�I���𒊏o
			if((CommandReturn[ClientNo]!=BSS_SELECT_RETURN_ITEM)&&
			   (CommandReturn[ClientNo]!=BSS_SELECT_RETURN_POKE)){
#if 0
				if(CommandReturn[ClientNo]==BSS_SELECT_RETURN_FIGHT){
					UserWork[WAZA_POS_WORK+ClientNo]=ClientBuffer[ClientNo][2];
					SelectWazaNo[ClientNo]=PSP[ClientNo].waza[UserWork[WAZA_POS_WORK+ClientNo]];
				}
#endif
				ActionNo[turn]=CommandReturn[ClientNo];
				ActionClientNo[turn]=ClientNo;
				turn++;
			}
		}
		for(i=0;i<ClientSetMax-1;i++){
			for(j=i+1;j<ClientSetMax;j++){
				k=ActionClientNo[i];
				l=ActionClientNo[j];
				if((ActionNo[i]!=BSS_SELECT_RETURN_ITEM)&&
				   (ActionNo[j]!=BSS_SELECT_RETURN_ITEM)&&
				   (ActionNo[i]!=BSS_SELECT_RETURN_POKE)&&
				   (ActionNo[j]!=BSS_SELECT_RETURN_POKE)){
					ret=AgiCalc(k,l,0);
					if(ret)
						SortAct(i,j);
				}
			}
		}
	}
	ServerSequence=ServerWazaBeforeMsg;
	UserWork[WAZA_BEFORE_MSG]=0;	//ServerWazaBeforeMsg�pClientNo�i�[���[�N
}
#else
void	ServerAgiCheck(void)
{
	int	i,j,k,l;
	int	turn;
	int	ret;

	turn=0;
	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){	
											//������𒊏o
											//������͖������ɐ�U
		if((CommandReturn[ClientNo]==BSS_SELECT_RETURN_ESCAPE)||
		   (CommandReturn[ClientNo]==BSS_SELECT_RETURN_SAFARI_ESCAPE)){
			ActionNo[turn]=CommandReturn[ClientNo];
			ActionClientNo[turn]=ClientNo;
			turn++;
		}
	}
	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){	
											//�Z�I���Ɠ�����ȊO�𒊏o
											//�Z�Ƃɂ���ȊO�͖������ɐ�U
//		if(CommandReturn[ClientNo]!=BSS_SELECT_RETURN_WAZA){
		if((CommandReturn[ClientNo]!=BSS_SELECT_RETURN_FIGHT)&&
		   (CommandReturn[ClientNo]!=BSS_SELECT_RETURN_ESCAPE)&&
		   (CommandReturn[ClientNo]!=BSS_SELECT_RETURN_SAFARI_ESCAPE)){
			ActionNo[turn]=CommandReturn[ClientNo];
			ActionClientNo[turn]=ClientNo;
			turn++;
		}
	}
	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){	
											//�Z�I���𒊏o
//		if(CommandReturn[ClientNo]==BSS_SELECT_RETURN_WAZA){
		if(CommandReturn[ClientNo]==BSS_SELECT_RETURN_FIGHT){
//			SelectWazaNo[ClientNo]=PSP[ClientNo].waza[ClientBuffer[ClientNo][2]];
			ActionNo[turn]=CommandReturn[ClientNo];
			ActionClientNo[turn]=ClientNo;
			turn++;
		}
	}
	for(i=0;i<ClientSetMax-1;i++){
		for(j=i+1;j<ClientSetMax;j++){
			k=ActionClientNo[i];
			l=ActionClientNo[j];
			if((ActionNo[i]==BSS_SELECT_RETURN_FIGHT)&&
			   (ActionNo[j]==BSS_SELECT_RETURN_FIGHT)){
				ret=AgiCalc(k,l,0);
				if(ret)
					SortAct(i,j);
			}
		}
	}

	ServerSequence=ServerWazaBeforeMsg;
	UserWork[WAZA_BEFORE_MSG]=0;	//ServerWazaBeforeMsg�pClientNo�i�[���[�N
}
#endif

void	ClearOneTurnCount(u8 flg)
{
	int	i;
	u8	*adr;

	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
		if(flg){
			OTF[ClientNo].guardflag=0;
			OTF[ClientNo].koraeruflag=0;
		}
		else{
			adr=(u8 *)&OTF[ClientNo];
			for(i=0;i<sizeof(OneTurnFlag);i++)
				adr[i]=0;
			if(WKC[ClientNo].nekodamashicnt)
				WKC[ClientNo].nekodamashicnt--;
			//�͂�ǂ��J�E���^��DEC
			if(WKC[ClientNo].handoucnt){
				WKC[ClientNo].handoucnt--;
				if(WKC[ClientNo].handoucnt==0)
					//�J�E���^���O�Ȃ�͂�ǂ���OFF�i�����Ă���Ƃ��΍�j
					PSP[ClientNo].condition2&=CONDITION2_HANDOU_OFF;
			}
		}
		if(WKC[ClientNo].migawarihp==0)
			PSP[ClientNo].condition2&=CONDITION2_MIGAWARI_OFF;
	}
	//���������ɂ��C��̃t���O�̂��̃^�[���������ێ�������R
	//�S�Ń`�F�b�N���I���΁A�����I�ɗ�����̂ŁA���Ƃ��K�v�Ȃ� 2002.09.27
//	ServerStatusFlag&=((0x0000000f<<28)^0xffffffff);
	SCC[0].konoyubitomareflag=0;	//���̂�тƂ܂�����̃^�[�����͗L���ɂ���
	SCC[1].konoyubitomareflag=0;	//���̂�тƂ܂�����̃^�[�����͗L���ɂ���
}

void	ClearOneSelfTurnCount(void)
{
	int	i;
	u8	*adr;

	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
		adr=(u8 *)&OSTF[ClientNo];
		for(i=0;i<sizeof(OneSelfTurnFlag);i++)
			adr[i]=0;
	}
}

void	ServerWazaBeforeMsg(void)
{
	if((ServerStatusFlag&SERVER_STATUS_FLAG_ESCAPE)==0){
		while(UserWork[WAZA_BEFORE_MSG]<ClientSetMax){
			ClientNo=AttackClient=UserWork[WAZA_BEFORE_MSG];
			UserWork[WAZA_BEFORE_MSG]++;
			//�������p���`�`�F�b�N
			if((SelectWazaNo[ClientNo]==WAZANO_KIAIPANTI)&&
			  ((PSP[ClientNo].condition&CONDITION_NEMURI)==0)&&
			   (WKC[AttackClient].namakebit==0)&&
			   (OTF[ClientNo].waruagakiflag==0)){
				ServerSubroutineSet((u8 *)&WAZA_KiaiPunch);
				return;
			}
		}
	}
	ServerWazaOutBeforeCheck();
	ActionCount=0;
	ActionSeqNo=ActionNo[ActionCount];
	DamagePowWork=0;
	UserWork[DAMAGE_TYPE_WORK]=0;
	ServerSequence=ServerCommandAct;
	ServerWork[3]=0;
	ServerWork[4]=0;
	UserWork[SERVER_WORK3_TEMP]=0;
	PUSH_ADRS->PushAdrsCnt=0;		//�A�h���X�v�b�V���p�J�E���^�[������
									//�v�b�V�������̂Ƀv�����Ȃ���
									//�V�[�P���X��������Ă��܂��Z�΍�
}

void	ServerCommandAct(void)
{
	if(WinLoseFlag!=0)
		ActionSeqNo=SCA_EndNo;
	UserWork[ACTION_COUNT_TMP]=ActionCount;
	ServerCommandActTbl[ActionSeqNo]();
	if(ActionCount>=ClientSetMax){
		ServerStatusFlag&=SERVER_STATUS_FLAG_TURNEND_OFF;
		ServerSequence=ServerBranchTbl[WinLoseFlag&0x7f];
//		ClearOneTurnCount();
	}
	else if(UserWork[ACTION_COUNT_TMP]!=ActionCount){
		ServerStatusFlag&=SERVER_STATUS_FLAG_DOUBLE_CHECK_OFF;
		ServerStatusFlag&=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK_OFF;
	}
}

void	ServerWin(void)
{
	ActionSeqNo=0;
	if(FightType&FIGHT_TYPE_TUUSHIN){
		EvStrBuf0[0]=WinLoseFlag;
		AttackClient=ClientNoGet(BSS_CLIENT_MINE);
		WazaSeqAdrs=(u8 *)&WAZA_TuushinWin;
		WinLoseFlag&=0x7f;
	}
	else if(FightType&(FIGHT_TYPE_BATTLE_TOWER|FIGHT_TYPE_CARD_E))
		WazaSeqAdrs=(u8 *)&WAZA_BattleTowerWin;
	else if((FightType&FIGHT_TYPE_TRAINER)&&
			((FightType&FIGHT_TYPE_TUUSHIN)==0)){
		PinchSeStop();
		WazaSeqAdrs=(u8 *)&WAZA_TrainerWin;
		switch(TD[FieldTrainerID].type){
			case TRTYPE_SITENNOU:
			case TRTYPE_TYANPION:
				MusicPlay(MUS_KACHI5);
				break;
			case TRTYPE_AKUADANRIIDAA:
			case TRTYPE_AKUADAN:
			case TRTYPE_AKUADANKANBU:
			case TRTYPE_MAGUMADANRIIDAA:
			case TRTYPE_MAGUMADAN:
			case TRTYPE_MAGUMADANKANBU:
				MusicPlay(MUS_KACHI4);
				break;
			case TRTYPE_ZIMURIIDAA:
				MusicPlay(MUS_KACHI3);
				break;
			default:
				MusicPlay(MUS_KACHI1);
				break;
		}
	}
	else
		WazaSeqAdrs=(u8 *)&WAZA_Win;

#if 0
	for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
		if(MineEnemyCheck(ClientNo)==SIDE_MINE){
			if(FTW.MineLastPokeNo==0){
				FTW.MineLastPokeNo=PSP[ClientNo].monsno;
				PM_strcpy(FTW.MineLastPokeNickname,PSP[ClientNo].nickname);
			}
			else{
				FTW.MineLastPokeNo2=PSP[ClientNo].monsno;
				PM_strcpy(FTW.MineLastPokeNickname2,PSP[ClientNo].nickname);
			}
		}
	}
#endif
	ServerSequence=ServerEnd;
}

void	ServerLose(void)
{
	ActionSeqNo=0;
	if(FightType&FIGHT_TYPE_TUUSHIN){
		EvStrBuf0[0]=WinLoseFlag;
		AttackClient=ClientNoGet(BSS_CLIENT_MINE);
		WazaSeqAdrs=(u8 *)&WAZA_TuushinWin;
		WinLoseFlag&=0x7f;
	}
	else
		WazaSeqAdrs=(u8 *)&WAZA_Lose;
	ServerSequence=ServerEnd;
}

void	ServerEscape(void)
{
	ActionSeqNo=0;
	switch(OTF[AttackClient].itemescapeflag){
		case 1:
			WazaSeqAdrs=(u8 *)&WAZA_ItemEscape;
			break;
		case 2:
			WazaSeqAdrs=(u8 *)&WAZA_SpeabiEscape;
			break;
		default:
			WazaSeqAdrs=(u8 *)&WAZA_Escape;
			break;
	}
	ServerSequence=ServerEnd;
}

void	ServerEnemyEscape(void)
{
	ActionSeqNo=0;
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_NICKNAMEONLY;
	EvStrBuf0[2]=AttackClient;
	EvStrBuf0[3]=SelMonsNo[AttackClient];
	EvStrBuf0[4]=EOM_;
	WazaSeqAdrs=(u8 *)&WAZA_EnemyEscape;
	ServerSequence=ServerEnd;
}

void	ServerEnd(void)
{
	if((ActionSeqNo==SCA_EndNo)||
	   (ActionSeqNo==SCA_EndCheckNo)){
//�Ō�ɏo���Ă����|�P������ۑ��i�����Ă��A�����Ă��A�߂܂��Ă��j
		if((FightType&FIGHT_TYPE_NO_TV_DATA)==0){
			for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
				if(MineEnemyCheck(ClientNo)==SIDE_MINE){
					if(FTW.MineLastPokeNo==0){
						FTW.MineLastPokeNo=PSP[ClientNo].monsno;
						PM_strcpy(FTW.MineLastPokeNickname,PSP[ClientNo].nickname);
					}
					else{
						FTW.MineLastPokeNo2=PSP[ClientNo].monsno;
						PM_strcpy(FTW.MineLastPokeNickname2,PSP[ClientNo].nickname);
					}
				}
			}
			TVBtlDataSet();
		}
//		PokeParaEnemyInit();
//		PaletteFadeReq(0xffffffff, 0,0,16, 0);
		LightFadeReq(N_to_B);
		BGM_FadeOutSet(5);
		ServerSequence=ServerShinkaCheckInit;
		ShinkaReturnProc=MainFight;
	}
	else{
		if(ClientBit==0)
			WazaSequenceTbl[*WazaSeqAdrs]();
	}
}

void	ServerShinkaCheckInit(void)
{

	if(FadeData.fade_sw == 0){
		InitActor();
		if((LevelUpBit==0)||(WinLoseFlag!=FIGHT_WIN)){
			ServerSequence=FightEnd;
			return;
		}
		ServerSequence=ServerShinkaCheck;
	}
}

void	ServerShinkaCheck(void)
{
	int	i;
	u16 aftermons;

	while(LevelUpBit){
		for(i=0;i<6;i++){
			if(LevelUpBit&no2bittbl[i]){
				LevelUpBit&=(no2bittbl[i]^0xffffffff);
				if((aftermons=PokeShinkaCheck(&PokeParaMine[i],LEVELUP_SHINKA))){
					ServerSequence=ServerShinkaWait;
					InitShinkaAct(&PokeParaMine[i],
							   aftermons,
							   SHINKA_CANCEL_ON|SHINKA_BGM_RESTART_OFF,
							   i);
					return;
				}
			}
		}
	}
	ServerSequence=FightEnd;
}

void	ServerShinkaWait(void)
{
	if(sys.pMainProc==MainFight)
		ServerSequence=ServerShinkaCheck;
}
	
void	FightEnd(void)
{
	if((FightType&FIGHT_TYPE_TUUSHIN)==0){
		PokerusSetCheck(&PokeParaMine[0]);
		PokerusCatchCheck(&PokeParaMine[0]);
	}
	if((FightType&FIGHT_TYPE_TUUSHIN)&&
	   (pokemon_sio_flag))
		return;
	AnswerWork=WinLoseFlag;			//�o�g���^���[�̂��߂ɁA������������
									//�X�N���v�g�ɓn�����[�N�ɑ��
	sys.FieldBattleFlag=0;			//�퓬��Flag�����Ƃ�
	sys.pFirstProc=PushFirstProc;

	if(FightType&FIGHT_TYPE_MV_POKE){	//�ړ��|�P�����̎��̏���
		GetMovePokeParam(&PokeParaEnemy[0]);
		if((WinLoseFlag==FIGHT_WIN)||(WinLoseFlag==FIGHT_ENEMY_GET))
			MvPokeFlagReset();
	}
//	PokeParaEnemyInit();
	m4aSongNumStop(SE_HINSI);		//HP���Ȃ��A���[�����Ƃ߂�
	MainProcChange(sys.pReturnProc);
}

void	ServerSubroutine(void)
{
	if((ActionSeqNo==SCA_EndNo)||
	   (ActionSeqNo==SCA_EndCheckNo)){
		if(PUSH_SERVER_ADRS->PushServerAdrsCnt)
			PUSH_SERVER_ADRS->PushServerAdrsCnt--;
		ServerSequence=
			PUSH_SERVER_ADRS->
			RetServerSequence[PUSH_SERVER_ADRS->PushServerAdrsCnt];
	}
	else{
		if(ClientBit==0)
			WazaSequenceTbl[*WazaSeqAdrs]();
	}
}

void	ServerSpeabiSubroutine(void)
{
	if(ClientBit==0)
		WazaSequenceTbl[*WazaSeqAdrs]();
}

#if SERVER_SUB
void	ServereFromTurnCheckAct(void)
{
	if(ClientBit==0){
		ActionSeqNo=0;
		WazaSequenceTbl[*WazaSeqAdrs]();
		if((ActionSeqNo==SCA_EndNo)||
		   (ActionSeqNo==SCA_EndChedckNo))
			ServerSequence=ServerTurnCheck;
	}
}
#endif

//==============================================================
//                       �T�u���[�`��(ServerCommandAct)
//==============================================================

//==============================================================
//                       ���������T�u���[�`��
//==============================================================

//���������R�}���h�̃A�N�V����
void	SCA_FightAct(void)
{
		u8	hiraishinspeed=4;
		u8	dir;

		AttackClient=ActionClientNo[ActionCount];

		//�C�₵�Ă����|�P�����i���̃^�[���ŕ��������|�P�������܂ށj��
		//�U�����Ȃ�
		if(UserWork[NO_RESHUFFLE_CLIENT]&no2bittbl[AttackClient]){
			ActionSeqNo=SCA_EndNo;
			return;
		}

		CriticalValue=1;				//�N���e�B�J���_���[�W���P�{�ɖ߂�
		UserWork[DAMAGE_VALUE]=1;		//�Z�ɂ��{�����[�N���P�{�ɖ߂�
		UserWork[POKECONDITION_CHECK]=0;	//PokeConditionCheck��SeqNo��������
		WazaStatusFlag=0;				//�Z�X�e�[�^�X�t���O��������
		WazaSeqLoopCount=0;
		ServerWork[6]=0;				//NoHitMsg���[�N���N���A
//		WazaPosNo=ClientBuffer[AttackClient][2];
		WazaPosNo=WazaPosNoTmp=UserWork[WAZA_POS_WORK+AttackClient];

		if(OTF[AttackClient].waruagakiflag){
			OTF[AttackClient].waruagakiflag=0;
			WazaNo=WazaNoTmp=WAZANO_WARUAGAKI;
			ServerStatusFlag|=SERVER_STATUS_FLAG_NOPP_DEC;
			UserWork[DEFENCE_CLIENT+AttackClient]=ServerDefenceClientGet(WazaNo,0);
		}
		else if((PSP[AttackClient].condition2&CONDITION2_KEEP)||
		   (PSP[AttackClient].condition2&CONDITION2_HANDOU))
			WazaNo=WazaNoTmp=WazaNoKeep[AttackClient];
		else if((WKC[AttackClient].encorewazano!=0)&&
			    (WKC[AttackClient].encorewazano==
				 PSP[AttackClient].waza[WKC[AttackClient].encorewazapos])){
			WazaNo=WazaNoTmp=WKC[AttackClient].encorewazano;
			WazaPosNo=WazaPosNoTmp=WKC[AttackClient].encorewazapos;
//			UserWork[DEFENCE_CLIENT+AttackClient]=WKC[AttackClient].encoreclient;
			UserWork[DEFENCE_CLIENT+AttackClient]=ServerDefenceClientGet(WazaNo,0);
		}
		//�A���R�[������Ă���Z��Y�ꂽ��A�A���R�[��������
		else if((WKC[AttackClient].encorewazano!=0)&&
		   (WKC[AttackClient].encorewazano!=
			PSP[AttackClient].waza[WKC[AttackClient].encorewazapos])){
			WazaPosNo=WazaPosNoTmp=WKC[AttackClient].encorewazapos;
			WazaNo=WazaNoTmp=PSP[AttackClient].waza[WazaPosNo];
			WKC[AttackClient].encorewazano=0;
			WKC[AttackClient].encorewazapos=0;
			WKC[AttackClient].encorecnt=0;
			UserWork[DEFENCE_CLIENT+AttackClient]=ServerDefenceClientGet(WazaNo,0);
		}
		else if(PSP[AttackClient].waza[WazaPosNo]!=SelectWazaNo[AttackClient]){
			WazaNo=WazaNoTmp=PSP[AttackClient].waza[WazaPosNo];
			UserWork[DEFENCE_CLIENT+AttackClient]=ServerDefenceClientGet(WazaNo,0);
		}
		else
			WazaNo=WazaNoTmp=PSP[AttackClient].waza[WazaPosNo];


		if(MineEnemyCheck(AttackClient)==SIDE_MINE)
			FTW.MineLastWazaNo=WazaNo;		//�������Ō�Ɏg�����Z
		else
			FTW.EnemyLastWazaNo=WazaNo;		//���肪�Ō�Ɏg�����Z

		dir=MineEnemyCheck(AttackClient)^1;

		if((SCC[dir].konoyubitomareflag)&&
		   (WTD[WazaNo].attackrange==RANGE_NORMAL)&&
		   (MineEnemyCheck(AttackClient)!=MineEnemyCheck(SCC[dir].konoyubitomareno))&&
	   	   (PSP[SCC[dir].konoyubitomareno].hp)){
			DefenceClient=SCC[dir].konoyubitomareno;
		}
		else if((FightType&FIGHT_TYPE_2vs2)&&
			    (SCC[dir].konoyubitomareflag==0)&&
			   ((WTD[WazaNo].damage)||((WTD[WazaNo].damage==0)&&(WTD[WazaNo].attackrange!=RANGE_MINE)))&&
			    (PSP[UserWork[DEFENCE_CLIENT+AttackClient]].speabino!=TOKUSYU_HIRAISIN)&&
				(WTD[WazaNo].wazatype==ELECTRIC_TYPE)){
			dir=MineEnemyCheck(AttackClient);
			for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
				if((dir!=MineEnemyCheck(ClientNo))&&
				   (UserWork[DEFENCE_CLIENT+AttackClient]!=ClientNo)&&
				   (PSP[ClientNo].speabino==TOKUSYU_HIRAISIN)){
					if(ActionNoGet(ClientNo)<hiraishinspeed)
						hiraishinspeed=ActionNoGet(ClientNo);
				}
			}
//			if(ClientNo==ClientSetMax){
			if(hiraishinspeed==4){
				if(WTD[WazaNoTmp].attackrange&RANGE_RANDOM){
					if(MineEnemyCheck(AttackClient)==SIDE_MINE){
						if(pp_rand()&1)
							DefenceClient=ClientNoGet(BSS_CLIENT_ENEMY);
						else
							DefenceClient=ClientNoGet(BSS_CLIENT_ENEMY2);
					}
					else{
						if(pp_rand()&1)
							DefenceClient=ClientNoGet(BSS_CLIENT_MINE);
						else
							DefenceClient=ClientNoGet(BSS_CLIENT_MINE2);
					}
				}
				else
					DefenceClient=UserWork[DEFENCE_CLIENT+AttackClient];
				if(NoReshuffleClient&no2bittbl[DefenceClient]){
					if(MineEnemyCheck(AttackClient)!=MineEnemyCheck(DefenceClient))
						DefenceClient=ClientNoGet(ClientTypeGet(DefenceClient)^2);
					else{
						DefenceClient=ClientNoGet(ClientTypeGet(AttackClient)^1);
						if(NoReshuffleClient&no2bittbl[DefenceClient])
							DefenceClient=ClientNoGet(ClientTypeGet(DefenceClient)^2);
					}
				}
			}
			else{
				ClientNo=ActionClientNo[hiraishinspeed];
				TokusyuNoSet(ClientNo,PSP[ClientNo].speabino);
				OSTF[ClientNo].hiraishinflag=1;
				DefenceClient=ClientNo;
			}
		}
		else if((FightType&FIGHT_TYPE_2vs2)&&
				(WTD[WazaNoTmp].attackrange&RANGE_RANDOM)){
			if(MineEnemyCheck(AttackClient)==SIDE_MINE){
				if(pp_rand()&1)
					DefenceClient=ClientNoGet(BSS_CLIENT_ENEMY);
				else
					DefenceClient=ClientNoGet(BSS_CLIENT_ENEMY2);
			}
			else{
				if(pp_rand()&1)
					DefenceClient=ClientNoGet(BSS_CLIENT_MINE);
				else
					DefenceClient=ClientNoGet(BSS_CLIENT_MINE2);
			}
			if(NoReshuffleClient&no2bittbl[DefenceClient]){
				if(MineEnemyCheck(AttackClient)!=MineEnemyCheck(DefenceClient))
					DefenceClient=ClientNoGet(ClientTypeGet(DefenceClient)^2);
			}
		}
		else{
//			DefenceClient=ClientBuffer[AttackClient][3];
			DefenceClient=UserWork[DEFENCE_CLIENT+AttackClient];
//			if(ServerStatusFlag&no2bittbl[ClientTypeGet(DefenceClient)])
			if(NoReshuffleClient&no2bittbl[DefenceClient]){
				if(MineEnemyCheck(AttackClient)!=MineEnemyCheck(DefenceClient))
					DefenceClient=ClientNoGet(ClientTypeGet(DefenceClient)^2);
				else{
					DefenceClient=ClientNoGet(ClientTypeGet(AttackClient)^1);
					if(NoReshuffleClient&no2bittbl[DefenceClient])
						DefenceClient=ClientNoGet(ClientTypeGet(DefenceClient)^2);
				}
			}
		}
		WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
		ActionSeqNo=SCA_WazaSequenceNo;
}

//==============================================================
//                       �|�P��������ւ��T�u���[�`��
//==============================================================

void	SCA_PokeAct(void)
{
	AttackClient=ActionClientNo[ActionCount];
	ScrX0 = 0;
	ScrY0 = 0;
	CommandCsrPos[AttackClient]=0;
	WazaCsrPos[AttackClient]=0;
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_NICKNAMEONLY;
	EvStrBuf0[2]=AttackClient;
	EvStrBuf0[3]=UserWork[BEFORE_RESHUFFLE_SELMONSNO+AttackClient];
	EvStrBuf0[4]=EOM_;
	UserWork[CLIENT_NO_WORK]=AttackClient;	//���b�Z�[�W�\���̂��߂̃N���C�A���g�i���o�[���Z�b�g
	WazaSeqAdrs=(u8 *)&WAZA_PokeReshuffle;
	ActionSeqNo=SCA_WazaSequenceNo;
	if(FTW.PokeReshuffleCount<0xff)		//�������|�P��������������
		FTW.PokeReshuffleCount++;		//�������|�P��������������
}

//==============================================================
//                       �A�C�e���g�p�T�u���[�`��
//==============================================================

void	SCA_ItemAct(void)
{
	AttackClient=DefenceClient=ActionClientNo[ActionCount];
	ScrX0 = 0;
	ScrY0 = 0;

	WKC[AttackClient].renzokugiricnt=0;			//��񂼂�����J�E���^�͏���������
												//���ɂ����邩��
	ItemNo=ClientBuffer[AttackClient][1]|(ClientBuffer[AttackClient][2]<<8);
	if(ItemNo<=ITEM_BALL_END)
		WazaSeqAdrs=(u8 *)ItemBallTbl[ItemNo];
	else if((ItemNo==ITEM_PIPPININGYOU)||(ItemNo==ITEM_ENEKONOSIPPO))
		WazaSeqAdrs=(u8 *)ItemFightTbl[0];
	else{
		if(MineEnemyCheck(AttackClient)==SIDE_MINE)
			WazaSeqAdrs=(u8 *)ItemNormalTbl[0];
		else{
			UserWork[CLIENT_NO_WORK]=AttackClient;
			switch(UserWork[AI_ITEM_TYPE+(AttackClient>>1)]){
				case ITEMTYPE_KAIHUKUNOKUSURI:
				case ITEMTYPE_HP_KAIHUKU:
					break;
				case ITEMTYPE_CONDITION_KAIHUKU:
					ServerWork[5]=0;
					if((UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]&1)&&
					   (UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]&0x3e))
						ServerWork[5]=5;
					else{
						while((UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]&1)==0){
							UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]=
								UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]>>1;
							ServerWork[5]++;
						}
					}
					break;
				case ITEMTYPE_ABIRITY_UP:
					ServerWork[5]=4;
					if(UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]&0x80)
						ServerWork[5]=5;
					else{
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_STATUS;
						EvStrBuf0[2]=COND_POW;
						EvStrBuf0[3]=EOM_;
						EvStrBuf1[0]=I_MSG;
						EvStrBuf1[1]=EV_MSGID;
						EvStrBuf1[2]=AGATTAMSGNO&0xff;
						EvStrBuf1[3]=(AGATTAMSGNO&0xff00)>>8;
						EvStrBuf1[4]=EOM_;
						while((UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]&1)==0){
							UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]=
								UserWork[AI_ITEM_CONDITION+(AttackClient>>1)]>>1;
							EvStrBuf0[2]++;
						}
						UserWork[FIGHT_EFFECT_WORK+0]=COND2_POWUP+EvStrBuf0[2]-1;
						UserWork[FIGHT_EFFECT_WORK+1]=0;
					}
					break;
				case ITEMTYPE_EFFECT_GUARD:
					if(FightType&FIGHT_TYPE_2vs2)
						ServerWork[5]=2;
					else
						ServerWork[5]=0;
					break;
			}
			WazaSeqAdrs=(u8 *)ItemNormalTbl[UserWork[AI_ITEM_TYPE+(AttackClient>>1)]];
		}
	}

	ActionSeqNo=SCA_WazaSequenceNo;
}

//==============================================================
//                       �ɂ���T�u���[�`��
//==============================================================

static	u8	EscapeCheckAct(u8 clientno)
{
	u8	ret=0;
	u8	escape;
	u8	eqp;

	if(PSP[clientno].item==ITEM_NAZONOMI)
//		eqp=Fld.RamSeed.item_equip;
		eqp=FRS[clientno].item_equip;
	else
		eqp=ItemEquipGet(PSP[clientno].item);

	ItemClient=clientno;

	//�A�C�e���ŕK����������̑������ʂ�100%�������
	if(eqp==SOUBI_KANARAZUNIGERARERU){
		ItemNo=PSP[clientno].item;
		OTF[clientno].itemescapeflag=1;
		ret++;
	}
	//����\�͂ɂ�������100%�������
	else if(PSP[clientno].speabino==TOKUSYU_NIGEASI){
		SpeabiNo=TOKUSYU_NIGEASI;
		OTF[clientno].itemescapeflag=2;
		ret++;
	}
	else{
		if((FightType&FIGHT_TYPE_2vs2)==0){
			if(PSP[clientno].agi<PSP[clientno^1].agi){
				escape=PSP[clientno].agi*128/PSP[clientno^1].agi+UserWork[ESCAPE_COUNT]*30;
				if(escape>(pp_rand()&0xff))
					ret++;
			}
			else
				ret++;
		}
		UserWork[ESCAPE_COUNT]++;
	}
	if(ret){
//		SePlay(SE_NIGERU);
		ActionCount=ClientSetMax;
		WinLoseFlag=FIGHT_ESCAPE;
	}
	return ret;
}

void	SCA_EscapeAct(void)
{
	AttackClient=ActionClientNo[ActionCount];

	if(FightType&FIGHT_TYPE_TUUSHIN){
//		SePlay(SE_NIGERU);
		ActionCount=ClientSetMax;
		for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
			if(MineEnemyCheck(ClientNo)==SIDE_MINE){
				if(CommandReturn[ClientNo]==BSS_SELECT_RETURN_ESCAPE)
					WinLoseFlag|=FIGHT_LOSE;
			}
			else{
				if(CommandReturn[ClientNo]==BSS_SELECT_RETURN_ESCAPE)
					WinLoseFlag|=FIGHT_WIN;
			}
		}
		WinLoseFlag|=FIGHT_ESCAPE_SELECT;
		return;
	}

	if(MineEnemyCheck(AttackClient)==SIDE_MINE){
		if(EscapeCheckAct(AttackClient)==0){
			//������Ɏ��s�����Ƃ��́u�݂��Â�v�𔭓����Ȃ�
			PSP[AttackClient].condition2&=CONDITION2_MICHIDURE_OFF;
			ServerWork[5]=3;
			WazaSeqAdrs=(u8 *)&WAZA_EscapeNG;
			ActionSeqNo=SCA_WazaSequenceNo;
		}
	}
	else{
//		SePlay(SE_NIGERU);		
		if(PSP[AttackClient].condition2&(CONDITION2_SHIME|CONDITION2_KUROIMANAZASHI)){
			ServerWork[5]=4;
			WazaSeqAdrs=(u8 *)&WAZA_EscapeNG;
			ActionSeqNo=SCA_WazaSequenceNo;
		}
		else{
			ActionCount=ClientSetMax;
			WinLoseFlag=FIGHT_ENEMY_ESCAPE;
		}
	}
}

//==============================================================
//					�T�t�@���]�[�����L�̃A�N�V����
//==============================================================

void	SCA_SafariAct(void)
{
	AttackClient=ActionClientNo[ActionCount];
	ScrX0 = 0;
	ScrY0 = 0;
	WazaSeqAdrs=(u8 *)SpecialActionTbl[SPEACT_SAFARI_NO];
	ActionSeqNo=SCA_WazaSequenceNo;
}

//==============================================================
//					�T�t�@���{�[���𓊂���
//==============================================================

void	SCA_BallAct(void)
{
	AttackClient=ActionClientNo[ActionCount];
	ScrX0 = 0;
	ScrY0 = 0;
	SafariBallCount--;
	ItemNo=ITEM_SAFARIBOORU;
	WazaSeqAdrs=(u8 *)ItemBallTbl[ItemNo];
	ActionSeqNo=SCA_WazaSequenceNo;
}

//==============================================================
//					�L���[�u�𓊂���
//==============================================================

static const u8 CubeEscapeTbl[][3]={
	{0,0,0},
	{3,5,0},
	{2,3,0},
	{1,2,0},
	{1,1,0},
};


void	SCA_CubeAct(void)
{
	AttackClient=ActionClientNo[ActionCount];
	ScrX0 = 0;
	ScrY0 = 0;
	ServerWork[5]=ClientBuffer[AttackClient][1]-1;	//�|���b�N�Ƃ̑���
	ItemNo=ClientBuffer[AttackClient][2];			//�|���b�N�̐F���

	if(UserWork[SAFARI_CUBE]<3)
		UserWork[SAFARI_CUBE]++;

	if(UserWork[SAFARI_ESCAPE_COUNT]>1){
		if(UserWork[SAFARI_ESCAPE_COUNT]<CubeEscapeTbl[UserWork[SAFARI_CUBE]][ServerWork[5]])
			UserWork[SAFARI_ESCAPE_COUNT]=1;
		else
			UserWork[SAFARI_ESCAPE_COUNT]-=CubeEscapeTbl[UserWork[SAFARI_CUBE]][ServerWork[5]];
	}

	WazaSeqAdrs=(u8 *)SpecialActionTbl[SPEACT_CUBETHROW_NO];
	ActionSeqNo=SCA_WazaSequenceNo;
}

//==============================================================
//					�����Â�
//==============================================================

const	u8	NearGetCountUpTbl[]={
	4,3,2,1
};

const	u8	NearEscapeCountUpTbl[]={
	4,4,4,4
};

void	SCA_ApproachAct(void)
{
	AttackClient=ActionClientNo[ActionCount];
	ScrX0 = 0;
	ScrY0 = 0;

	UserWork[SAFARI_GET_COUNT]+=NearGetCountUpTbl[UserWork[SAFARI_NEAR]];
	if(UserWork[SAFARI_GET_COUNT]>20)
		UserWork[SAFARI_GET_COUNT]=20;
	UserWork[SAFARI_ESCAPE_COUNT]+=NearEscapeCountUpTbl[UserWork[SAFARI_NEAR]];
	if(UserWork[SAFARI_ESCAPE_COUNT]>20)
		UserWork[SAFARI_ESCAPE_COUNT]=20;

	if(UserWork[SAFARI_NEAR]<3){
		UserWork[SAFARI_NEAR]++;
		ServerWork[5]=0;
	}
	else
		ServerWork[5]=1;

	WazaSeqAdrs=(u8 *)SpecialActionTbl[SPEACT_APPROACH_NO];
	ActionSeqNo=SCA_WazaSequenceNo;
}

//==============================================================
//                       �ɂ���T�u���[�`���i�T�t�@���]�[���j
//==============================================================

void	SCA_SafariEscapeAct(void)
{
	AttackClient=ActionClientNo[ActionCount];
	SePlay(SE_NIGERU);
	ActionCount=ClientSetMax;
	WinLoseFlag=FIGHT_ESCAPE;
}

//==============================================================
//						�ߊl�f���T�u���[�`��
//==============================================================

void	SCA_GetDemoAct(void)
{
	AttackClient=ActionClientNo[ActionCount];
	ScrX0 = 0;
	ScrY0 = 0;
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_NICKNAMEONLY;
	EvStrBuf0[2]=AttackClient;
	EvStrBuf0[3]=SelMonsNo[AttackClient];
	EvStrBuf0[4]=EOM_;
	WazaSeqAdrs=(u8 *)SpecialActionTbl[SPEACT_GET_DEMO_NO];
	ActionSeqNo=SCA_WazaSequenceNo;
	ActionNo[1]=SCA_EndNo;
}

//==============================================================
//                       �I���T�u���[�`��
//==============================================================

void	SCA_EndCheck(void)
{
	if(GetExpCheck()) return;
	UserWork[GEC_WORK0]=0;			//�o���l�Q�b�g�`�F�b�N�p�V�[�P���X�i���o�[�̏�����
	ActionSeqNo=SCA_EndNo;
}

void	SCA_DeadEnd(void)
{
	ActionCount++;
	ActionSeqNo=ActionNo[ActionCount];
	ServerStatusFlag&=SERVER_STATUS_FLAG_SCAEND_OFF;
}

void	SCA_End(void)
{
//	int	i;

	ActionCount++;
	ActionSeqNo=ActionNo[ActionCount];

	ClearOneSelfTurnCount();

	ServerStatusFlag&=SERVER_STATUS_FLAG_SCAEND_OFF;

#if 0
	if(ServerStatusFlag&SERVER_STATUS_FLAG_A2D){
		ClientNo=AttackClient;
		AttackClient=DefenceClient;
		DefenceClient=ClientNo;
		ServerStatusFlag&=SERVER_STATUS_FLAG_A2D_OFF;
	}

	if((CommandReturn[AttackClient]==BSS_SELECT_RETURN_FIGHT)&&
	  ((NoReshuffleClient&no2bittbl[AttackClient])==0)&&
	  ((UserWork[NO_RESHUFFLE_CLIENT]&no2bittbl[AttackClient])==0)){

		if(ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT)
			WazaNoUse[AttackClient]=WazaNoTmp;
		else
			WazaNoUse[AttackClient]=0xffff;

		DefenceClient=UserWork[DEFENCE_CLIENT+AttackClient];
		if(NoReshuffleClient&no2bittbl[DefenceClient])
			DefenceClient=ClientNoGet(ClientTypeGet(DefenceClient)^2);

		WazaNoDefenceClient[AttackClient]=DefenceClient;
		WazaNoAttackClient[DefenceClient]=AttackClient;

		if((ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT)&&
		  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0))
			WazaNoHit[DefenceClient]=WazaNoTmp;
		else
			WazaNoHit[DefenceClient]=0xffff;		//�m�[�q�b�g�͂��̃i���o�[

		if((WTD[WazaNoTmp].flag&FLAG_OUMUGAESI)&&
		   (ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT)&&
		  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
			if(WTD[WazaNoTmp].attackrange&(RANGE_DOUBLE|RANGE_FIELD)){
				for(i=0;i<ClientSetMax;i++){
					if(MineEnemyCheck(AttackClient)!=
					   MineEnemyCheck(i)){
						UserWork[OUMU_WAZANO+0+i*2]=WazaNoTmp&0xff;
						UserWork[OUMU_WAZANO+1+i*2]=(WazaNoTmp&0xff00)>>8;
					}
				}
			}
			else if(WTD[WazaNoTmp].attackrange==RANGE_TRIPLE){
				for(i=0;i<ClientSetMax;i++){
					if(AttackClient!=i){
						UserWork[OUMU_WAZANO+0+i*2]=WazaNoTmp&0xff;
						UserWork[OUMU_WAZANO+1+i*2]=(WazaNoTmp&0xff00)>>8;
					}
				}
			}
			else if(AttackClient!=DefenceClient){
				UserWork[OUMU_WAZANO+0+DefenceClient*2]=WazaNoTmp&0xff;
				UserWork[OUMU_WAZANO+1+DefenceClient*2]=(WazaNoTmp&0xff00)>>8;
			}
		}
		else{
			if(AttackClient==DefenceClient){
				for(i=0;i<ClientSetMax;i++){
					if(MineEnemyCheck(AttackClient)!=
					   MineEnemyCheck(i)){
						UserWork[OUMU_WAZANO+0+i*2]=0;
						UserWork[OUMU_WAZANO+1+i*2]=0;
					}
				}
			}
			else{
				UserWork[OUMU_WAZANO+0+DefenceClient*2]=0;
				UserWork[OUMU_WAZANO+1+DefenceClient*2]=0;
			}
		}

		ServerStatusFlag&=SERVER_STATUS_FLAG_WAZA_OUT_OFF;
	}
#endif

	WazaDamage=0;					//�Z�_���[�W�N���A
	UserWork[WAZA_EFF_CNT]=0;		//�Z�G�t�F�N�g�p�J�E���^�̏�����
	UserWork[WAZA_EFF_CNT_TMP]=0;	//�Z�G�t�F�N�g�p�J�E���^�̏�����
	WazaNoHit[AttackClient]=0;
	WazaTypeHit[AttackClient]=0;

	UserWork[DAMAGE_TYPE_WORK]=0;
	DamagePowWork=0;

	UserWork[WKC_WORK0]=0;			//�Z���ʃ`�F�b�N�p�V�[�P���X�i���o�[�̏�����

	ServerWork[3]=0;
	ServerWork[4]=0;
	UserWork[SERVER_WORK3_TEMP]=0;
	PUSH_ADRS->PushAdrsCnt=0;		//�A�h���X�v�b�V���p�J�E���^�[������
									//�v�b�V�������̂Ƀv�����Ȃ���
									//�V�[�P���X��������Ă��܂��Z�΍�
}

//==============================================================
//                       �T�u���[�`��
//==============================================================

u8		SideClientNoGet(u8 side)
{
	u8	ret=0;

	switch(side){
		case SIDE_DEFENCE:
			ret=DefenceClient;
			break;
		case SIDE_ATTACK:
			ret=AttackClient;
			break;
		case SIDE_TSUIKA:
			ret=TsuikaClient;
			break;
		case SIDE_KIZETSU:
			ret=KizetsuClient;
			break;
		case SIDE_FIELD:
			ret=0;
			break;
		case SIDE_CLIENTNOWORK:
			ret=UserWork[CLIENT_NO_WORK];
			break;
		case SIDE_DOUBLE1:
			ret=KizetsuClient;
			break;
		case SIDE_ATTACK_DOUBLE:
		case SIDE_DOUBLE2:
		case SIDE_MINE_BENCH:
		case SIDE_ENEMY_BENCH:
			break;
	}
	return ret;
}

void	PressurePPDecCheck(u8 srcclient,u8 destclient,u16 wazano)
{
	int	i;

	if(PSP[srcclient].speabino!=TOKUSYU_PURESSYAA)
		return;
	for(i=0;i<4;i++){
		if(PSP[destclient].waza[i]==wazano)
			break;
	}
	if(i!=4){
		if(PSP[destclient].pp[i])
			PSP[destclient].pp[i]--;
		if(((PSP[destclient].condition2&CONDITION2_HENSHIN)==0)&&
		   ((WKC[destclient].monomanebit&no2bittbl[i])==0)){
			ClientNo=destclient;
			BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 	 BSS_PARA_PP1+i,
								 BSS_SELMONSNO,
								 1,
		 						 (u8 *)&PSP[ClientNo].pp[i]);
			ClientBitSet(ClientNo);
		}
	}
}

void	PressurePPDecCheckHuuin(u8 destclient)
{
	int	i,j=0,dir,pos=4;

	dir=MineEnemyCheck(destclient);
	for(i=0;i<ClientSetMax;i++){
		if(dir!=MineEnemyCheck(i)){
			if(PSP[i].speabino!=TOKUSYU_PURESSYAA)
				continue;
			for(j=0;j<4;j++){
				if(PSP[destclient].waza[j]==WAZANO_HUUIN)
					break;
			}
			if(j!=4){
				pos=j;
				if(PSP[destclient].pp[j])
					PSP[destclient].pp[j]--;
			}
		}
	}
	if(pos!=4){
		if(((PSP[destclient].condition2&CONDITION2_HENSHIN)==0)&&
		   ((WKC[destclient].monomanebit&no2bittbl[pos])==0)){
			ClientNo=destclient;
			BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 	 BSS_PARA_PP1+pos,
								 BSS_SELMONSNO,
								 1,
		 						 (u8 *)&PSP[ClientNo].pp[pos]);
			ClientBitSet(ClientNo);
		}
	}
}

void	PressurePPDecCheckHorobinouta(u8 destclient)
{
	int	i,j=0,pos=4;

	for(i=0;i<ClientSetMax;i++){
		if(PSP[i].speabino!=TOKUSYU_PURESSYAA)
			continue;
		if(i==destclient)
			continue;
		for(j=0;j<4;j++){
			if(PSP[destclient].waza[j]==WAZANO_HOROBINOUTA)
				break;
		}
		if(j!=4){
			pos=j;
			if(PSP[destclient].pp[j])
				PSP[destclient].pp[j]--;
		}
	}
	if(pos!=4){
		if(((PSP[destclient].condition2&CONDITION2_HENSHIN)==0)&&
		   ((WKC[destclient].monomanebit&no2bittbl[pos])==0)){
			ClientNo=destclient;
			BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 	 BSS_PARA_PP1+pos,
								 BSS_SELMONSNO,
								 1,
		 						 (u8 *)&PSP[ClientNo].pp[pos]);
			ClientBitSet(ClientNo);
		}
	}
}

void	ClientBitSetAll(void)
{
	int	i;
	if(FightType&FIGHT_TYPE_TUUSHIN)
		for(i=0;i<ClientSetMax;i++)
			ClientBit|=(no2bittbl[i]<<28);
	else
		for(i=0;i<ClientSetMax;i++) ClientBit|=no2bittbl[i];
}

void	ClientBitSet(u8 no)
{
	if(FightType&FIGHT_TYPE_TUUSHIN)
		ClientBit|=(no2bittbl[no]<<28);
	else
		ClientBit|=no2bittbl[no];
}

void	ClientBitSet2(u8 no)
{
#if 1
	int	i;

	for(i=0;i<GetConnectSioMenberNum();i++)
		ClientBit|=(no2bittbl[no]<<(i*4));

	ClientBit&=((CLIENT_BIT_TUUSHIN_OFF<<no)^0xffffffff);
#else
	ClientBit|=no2bittbl[no];
	ClientBit&=((CLIENT_BIT_TUUSHIN_OFF<<no)^0xffffffff);
#endif
}

void	ServerKeepOff(u8 clientno)
{
	PSP[clientno].condition2&=CONDITION2_KEEP_OFF;
	PSP[clientno].condition2&=CONDITION2_ABARERU_OFF;
	PSP[clientno].condition2&=CONDITION2_SAWAGU_OFF;
	PSP[clientno].condition2&=CONDITION2_GAMAN_OFF;
	WazaKouka[clientno]&=WAZAKOUKA_KIE_OFF;
	WKC[clientno].korogarucnt=0;
	WKC[clientno].renzokugiricnt=0;
}

u8		WazaOutCheck(u8 clientno)
{
	if((OTF[clientno].mahiflag)||
	   (OTF[clientno].koukanaiflag)||
	   (OTF[clientno].huuinflag)||
	   (OTF[clientno].meromeroflag)||
	   (OTF[clientno].kanashibariflag)||
	   (OTF[clientno].chouhatsuflag)||
	   (OTF[clientno].ichamonflag)||
	   (OTF[clientno].hirumuflag)||
	   (OTF[clientno].konranflag))
		return 1;

	return 0;
}

#if 0
void	FightMsgServerSet(int mojicnt)
{
	FightMsgClear();
#if 0
	ClientNo=BSS_CLIENT_ENEMY;
	BSS_MESSAGE_SET(BSS_SERVER,mojicnt,FightMsgBuf);
	ClientBitSet(BSS_CLIENT_ENEMY);
#else
	ClientNo=0;
	BSS_MESSAGE_SET(BSS_SERVER,mojicnt,FightMsgBuf);
	ClientBitSet(ClientNo);
#endif
}
#else
void	FightMsgServerSet(u16 MsgID,u8 clientno)
{
	ClientNo=clientno;
	BSS_MESSAGE_SET(BSS_SERVER,MsgID);
	ClientBitSet(ClientNo);
}
#endif

void	FightJoinFlagInit(void)
{
	int	i,j=0;

	FightJoinFlag[0]=0;
	FightJoinFlag[1]=0;

	for(i=0;i<ClientSetMax;i+=2)
		j|=no2bittbl[SelMonsNo[i]];

	for(i=1;i<ClientSetMax;i+=2)
		FightJoinFlag[(i&2)>>1]=j;
}

void	FightJoinFlagInitClient(u8 clientno)
{
	int	i=0,j=0;
	int	dir=0;

#if 0
	if(MineEnemyCheck(clientno)==SIDE_MINE){
		for(i=1;i<ClientSetMax;i++)
			FightJoinFlag[(i&2)>>1]&=(no2bittbl[SelMonsNo[clientno]]^0xffffffff);
	}
	else{
		dir=(clientno&2)>>1;
		FightJoinFlag[dir]=0;
		for(i=0;i<ClientSetMax;i+=2){
			if((NoReshuffleClient&no2bittbl[i])==0)
				j|=no2bittbl[SelMonsNo[i]];
		}
		FightJoinFlag[dir]=j;
	}
#else
	if(MineEnemyCheck(clientno)==SIDE_ENEMY){
		dir=(clientno&2)>>1;
		FightJoinFlag[dir]=0;
		for(i=0;i<ClientSetMax;i+=2){
			if((NoReshuffleClient&no2bittbl[i])==0)
				j|=no2bittbl[SelMonsNo[i]];
		}
		FightJoinFlag[dir]=j;
	}
#endif
}

void	FightJoinFlagSet(u8 clientno)
{
	int	i;
	if(MineEnemyCheck(clientno)==SIDE_ENEMY)
		FightJoinFlagInitClient(clientno);
	else{
		for(i=1;i<ClientSetMax;i++)
			FightJoinFlag[(i&2)>>1]|=no2bittbl[SelMonsNo[clientno]];
	}
}

void	PushAdrsSet(u8 *adrs)
{
	PUSH_ADRS->PushAdrsBuf[PUSH_ADRS->PushAdrsCnt++]=adrs;
}

void	PushAdrsAct()
{
	PUSH_ADRS->PushAdrsBuf[PUSH_ADRS->PushAdrsCnt++]=WazaSeqAdrs;
}

void	PopAdrsAct()
{
	WazaSeqAdrs=PUSH_ADRS->PushAdrsBuf[--PUSH_ADRS->PushAdrsCnt];
}

u8	ReturnWazaNoCheck(void)
{
	u16	wazano;
	u8	ret=0;
	u8	eqp;
	u16	*waza;

	wazano=PSP[ClientNo].waza[ClientBuffer[ClientNo][2]];
	waza=(u16 *)&UserWork[KODAWARI_WAZANO+ClientNo*2];

	if((WKC[ClientNo].kanashibariwazano==wazano)&&
	   (wazano!=0)){	//���Ȃ��΂�Z�i���o�[
		UserWork[CLIENT_NO_WORK]=ClientNo;
		WazaNo=wazano;
//		ServerSubroutineSet((u8 *)&WAZA_Kanashibari2);	
		AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_Kanashibari2;	
		ret++;
	}

	if((wazano==WazaNoUse[ClientNo])&&
	   (wazano!=WAZANO_WARUAGAKI)&&
	   (PSP[ClientNo].condition2&CONDITION2_ICHAMON)){ //���������`�F�b�N
//		ServerSubroutineSet((u8 *)&WAZA_Ichamon);
		ServerKeepOff(ClientNo);
		AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_Ichamon;
		ret++;
	}
	if((WKC[ClientNo].chouhatsucnt)&&
	  ((WTD[wazano].damage)==0)){						//���傤�͂`�F�b�N
		WazaNo=wazano;
//		ServerSubroutineSet((u8 *)&WAZA_Chouhatsu);
		AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_Chouhatsu;
		ret++;
	}


	if(HuuinCheck(ClientNo,wazano)){						//�ӂ�����`�F�b�N
		WazaNo=wazano;
//		ServerSubroutineSet((u8 *)&WAZA_Huuin2);
		AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_Huuin2;
		ret++;
	}

	if(PSP[ClientNo].item==ITEM_NAZONOMI)
//		eqp=Fld.RamSeed.item_equip;
		eqp=FRS[ClientNo].item_equip;
	else
		eqp=ItemEquipGet(PSP[ClientNo].item);

	ItemClient=ClientNo;

	//�������n�`�}�L�`�F�b�N
#if 0
	if((eqp==SOUBI_ONAZIWAZAONLY)&&
	   (WazaNoUse[ClientNo]!=0)&&
	   (WazaNoUse[ClientNo]!=0xffff)&&
	   (WazaNoUse[ClientNo]!=wazano)){
		WazaNo=WazaNoUse[ClientNo];
		ItemNo=PSP[ClientNo].item;
		AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_ItemOnaziWazaOnly;
		ret++;
	}
#else
	if((eqp==SOUBI_ONAZIWAZAONLY)&&
	   (waza[0]!=0)&&
	   (waza[0]!=0xffff)&&
	   (waza[0]!=wazano)){
		WazaNo=waza[0];
		ItemNo=PSP[ClientNo].item;
		AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_ItemOnaziWazaOnly;
		ret++;
	}
#endif

	//pp���Ȃ��`�F�b�N�͕K���Ō�ɂ���
	if(PSP[ClientNo].pp[ClientBuffer[ClientNo][2]]==0){		//pp���Ȃ�
//		ServerSubroutineSet((u8 *)&WAZA_PPNone);
		AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_PPNone;
		ret++;
	}

	return ret;
}

#define	WWNC_WAZA_NONE		0x01
#define	WWNC_PP_NONE		0x02
#define	WWNC_KANASHIBARI	0x04
#define	WWNC_ICHAMON		0x08
#define	WWNC_CHOUHATU		0x10
#define	WWNC_HUUIN			0x20
#define	WWNC_ALL			0xff

u8	WaruagakiWazaNoCheck(u8 clientno,u8 wazabit,u8 checkbit)
{
	int	i;
	u8	eqp;
	u16	*wazano;

	wazano=(u16 *)&UserWork[KODAWARI_WAZANO+clientno*2];

	if(PSP[clientno].item==ITEM_NAZONOMI)
//		eqp=Fld.RamSeed.item_equip;
		eqp=FRS[clientno].item_equip;
	else
		eqp=ItemEquipGet(PSP[clientno].item);

	ItemClient=clientno;

	for(i=0;i<4;i++){
		//�킴���Ȃ�
		if((PSP[clientno].waza[i]==0)&&
		   (checkbit&WWNC_WAZA_NONE))
			wazabit|=no2bittbl[i];
		//PP���Ȃ�
		if((PSP[clientno].pp[i]==0)&&
		   (checkbit&WWNC_PP_NONE))
			wazabit|=no2bittbl[i];
		//���Ȃ��΂肳��Ă�
		if((PSP[clientno].waza[i]==WKC[clientno].kanashibariwazano)&&
		   (checkbit&WWNC_KANASHIBARI))
			wazabit|=no2bittbl[i];
		//���������������Ă���
		if((PSP[clientno].waza[i]==WazaNoUse[clientno])&&
		   (checkbit&WWNC_ICHAMON)&&
		   (PSP[clientno].condition2&CONDITION2_ICHAMON))
			wazabit|=no2bittbl[i];
		//���傤�͂���Ă���
		if((WKC[clientno].chouhatsucnt)&&
		   (checkbit&WWNC_CHOUHATU)&&
	   	  ((WTD[PSP[clientno].waza[i]].damage)==0))	
			wazabit|=no2bittbl[i];
		//�ӂ����񂳂�Ă���
		if((HuuinCheck(clientno,PSP[clientno].waza[i]))&&
		   (checkbit&WWNC_HUUIN))
			wazabit|=no2bittbl[i];
		//�A���R�[��
		if((WKC[clientno].encorecnt)&&
		   (WKC[clientno].encorewazano!=PSP[clientno].waza[i]))
			wazabit|=no2bittbl[i];
#if 0
		//�������n�`�}�L�`�F�b�N
		if((eqp==SOUBI_ONAZIWAZAONLY)&&
		   (WazaNoUse[clientno]!=0)&&
		   (WazaNoUse[clientno]!=0xffff)&&
		   (WazaNoUse[clientno]!=PSP[clientno].waza[i]))
			wazabit|=no2bittbl[i];
#else
		//�������n�`�}�L�`�F�b�N
		if((eqp==SOUBI_ONAZIWAZAONLY)&&
		   (wazano[0]!=0)&&
		   (wazano[0]!=0xffff)&&
		   (wazano[0]!=PSP[clientno].waza[i]))
			wazabit|=no2bittbl[i];
#endif
	}
	return wazabit;
}

u8	WaruagakiCheck(void)
{
	u8	wazabit;

	wazabit=WaruagakiWazaNoCheck(ClientNo,0,WWNC_ALL);

	if(wazabit==0x0f){
		OTF[ClientNo].waruagakiflag=1;
#if 0
		if(MineEnemyCheck(ClientNo)==SIDE_MINE)
			ServerSubroutineSet((u8 *)&WAZA_Waruagaki);
		ServerWork[ClientNo]=3;
		AttackClient=ClientNo;
#else
		AlertSeqAdrs[ClientNo]=(u8 *)&WAZA_Waruagaki;
#endif
		if(FightType&FIGHT_TYPE_2vs2)
			ClientBuffer[ClientNo][3]=
				ClientNoGet((ClientTypeGet(ClientNo)^1)|(pp_rand()&0x02));
		else
			ClientBuffer[ClientNo][3]=ClientNoGet((ClientTypeGet(ClientNo)^1));
	}
	else
		OTF[ClientNo].waruagakiflag=0;

	return(wazabit==0x0f);
}

u8	HuuinCheck(u8 clientno,u16 wazano)
{
	int	i,j;
	u8	dir;
	u8	ret=0;

	dir=MineEnemyCheck(clientno);

	for(i=0;i<ClientSetMax;i++){
		if((dir!=MineEnemyCheck(i))&&
		   (WazaKouka[i]&WAZAKOUKA_HUUIN)){
			for(j=0;j<4;j++){
				if(wazano==PSP[i].waza[j]) break;
			}
			if(j<4) ret++;
		}
	}
			
	return ret;
}

//==============================================================
//		�V�C�`�F�b�N
//==============================================================

enum{
	FCC_AGICALC=0,
	FCC_REFLEC,
	FCC_HIKARI,
	FCC_SHIROI,
	FCC_SHINPI,
	FCC_NEGAIGOTO,
	FCC_AMAGOI,
	FCC_SUNAARASHI,
	FCC_NIHONBARE,
	FCC_ARARE,
	FCC_END,
};

u8	FieldConditionCheck(void)
{
	int	i,j;
	u8	ret=0;
	u8	dir=0;

	for(AttackClient=0;AttackClient<ClientSetMax;AttackClient++){
		if((NoReshuffleClient&no2bittbl[AttackClient])==0)
			break;
	}
	for(DefenceClient=0;DefenceClient<ClientSetMax;DefenceClient++){
		if((NoReshuffleClient&no2bittbl[DefenceClient])==0)
			break;
	}

	do{
		switch(UserWork[FCC_WORK0]){
			case FCC_AGICALC:
				for(i=0;i<ClientSetMax;i++)
					ActionClientNo[i]=i;
				for(i=0;i<ClientSetMax-1;i++){
					for(j=i+1;j<ClientSetMax;j++){
						if(AgiCalc(ActionClientNo[i],ActionClientNo[j],0))
							SortAct(i,j);
					}
				}
				UserWork[FCC_WORK0]++;
				UserWork[FCC_WORK1]=0;
			case FCC_REFLEC:	 //���t���N�^�[
#if 0
				while(UserWork[FCC_WORK1]<ClientSetMax){
					ClientNo=AttackClient=ActionClientNo[UserWork[FCC_WORK1]];
					if(MineEnemyCheck(ClientNo)==SIDE_MINE)
						type=BSS_CLIENT_MINE;
					else
						type=BSS_CLIENT_ENEMY;
					if((FightType&FIGHT_TYPE_2vs2)&&
					  ((NoReshuffleClient&no2bittbl[ClientNoGet(type)])==0)&&
					   (ClientTypeGet(ClientNo)==(type+2))){
						UserWork[FCC_WORK1]++;
						continue;
					}
					dir=MineEnemyCheck(ClientNo);
					if(SideCondition[dir]&SIDE_CONDITION_BUTSURIGUARD){
						if(--SCC[dir].butsuriguardcnt==0){
							SideCondition[dir]&=
									SIDE_CONDITION_BUTSURIGUARD_OFF;
							ServerSubroutineSet((u8 *)&WAZA_ReflecEnd);
							ServerWork[5]=ClientTypeGet(ClientNo)&0x01;
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_WAZANO;
							EvStrBuf0[2]=WAZANO_RIHUREKUTAA&0xff;
							EvStrBuf0[3]=(WAZANO_RIHUREKUTAA&0xff00)>>8;
							EvStrBuf0[4]=EOM_;
							ret++;
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0){
					UserWork[FCC_WORK0]++;
					UserWork[FCC_WORK1]=0;
				}
#endif
				while(UserWork[FCC_WORK1]<2){
					ClientNo=AttackClient=dir=UserWork[FCC_WORK1];
					if(SideCondition[dir]&SIDE_CONDITION_BUTSURIGUARD){
						if(--SCC[dir].butsuriguardcnt==0){
							SideCondition[dir]&=
									SIDE_CONDITION_BUTSURIGUARD_OFF;
							ServerSubroutineSet((u8 *)&WAZA_ReflecEnd);
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_WAZANO;
							EvStrBuf0[2]=WAZANO_RIHUREKUTAA&0xff;
							EvStrBuf0[3]=(WAZANO_RIHUREKUTAA&0xff00)>>8;
							EvStrBuf0[4]=EOM_;
							ret++;
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0){
					UserWork[FCC_WORK0]++;
					UserWork[FCC_WORK1]=0;
				}
				break;
			case FCC_HIKARI: //�Ђ���̂���
#if 0
				while(UserWork[FCC_WORK1]<ClientSetMax){
					ClientNo=AttackClient=ActionClientNo[UserWork[FCC_WORK1]];
					if(MineEnemyCheck(ClientNo)==SIDE_MINE)
						type=BSS_CLIENT_MINE;
					else
						type=BSS_CLIENT_ENEMY;
					if((FightType&FIGHT_TYPE_2vs2)&&
					  ((NoReshuffleClient&no2bittbl[ClientNoGet(type)])==0)&&
					   (ClientTypeGet(ClientNo)==(type+2))){
						UserWork[FCC_WORK1]++;
						continue;
					}
					dir=MineEnemyCheck(ClientNo);
					if(SideCondition[dir]&SIDE_CONDITION_TOKUSYUGUARD){
						if(--SCC[dir].tokusyuguardcnt==0){
							SideCondition[dir]&=
									SIDE_CONDITION_TOKUSYUGUARD_OFF;
							ServerSubroutineSet((u8 *)&WAZA_ReflecEnd);
							ServerWork[5]=ClientTypeGet(ClientNo)&0x01;
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_WAZANO;
							EvStrBuf0[2]=WAZANO_HIKARINOKABE&0xff;
							EvStrBuf0[3]=(WAZANO_HIKARINOKABE&0xff00)>>8;
							EvStrBuf0[4]=EOM_;
							ret++;
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0){
					UserWork[FCC_WORK0]++;
					UserWork[FCC_WORK1]=0;
				}
#endif
				while(UserWork[FCC_WORK1]<2){
					ClientNo=AttackClient=dir=UserWork[FCC_WORK1];
					if(SideCondition[dir]&SIDE_CONDITION_TOKUSYUGUARD){
						if(--SCC[dir].tokusyuguardcnt==0){
							SideCondition[dir]&=
									SIDE_CONDITION_TOKUSYUGUARD_OFF;
							ServerSubroutineSet((u8 *)&WAZA_ReflecEnd);
							ServerWork[5]=dir;
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_WAZANO;
							EvStrBuf0[2]=WAZANO_HIKARINOKABE&0xff;
							EvStrBuf0[3]=(WAZANO_HIKARINOKABE&0xff00)>>8;
							EvStrBuf0[4]=EOM_;
							ret++;
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0){
					UserWork[FCC_WORK0]++;
					UserWork[FCC_WORK1]=0;
				}
				break;
			case FCC_SHIROI: //���낢����
#if 0
				while(UserWork[FCC_WORK1]<ClientSetMax){
					ClientNo=AttackClient=ActionClientNo[UserWork[FCC_WORK1]];
					if(MineEnemyCheck(ClientNo)==SIDE_MINE)
						type=BSS_CLIENT_MINE;
					else
						type=BSS_CLIENT_ENEMY;
					if((FightType&FIGHT_TYPE_2vs2)&&
					  ((NoReshuffleClient&no2bittbl[ClientNoGet(type)])==0)&&
					   (ClientTypeGet(ClientNo)==(type+2))){
						UserWork[FCC_WORK1]++;
						continue;
					}
					dir=MineEnemyCheck(ClientNo);
					if(SCC[dir].shiroikiricnt){
						if(--SCC[dir].shiroikiricnt==0){
							SideCondition[dir]&=SIDE_CONDITION_SHIROIKIRI_OFF;
							ServerSubroutineSet((u8 *)&WAZA_ReflecEnd);
							ServerWork[5]=ClientTypeGet(ClientNo)&0x01;
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_WAZANO;
							EvStrBuf0[2]=WAZANO_SIROIKIRI&0xff;
							EvStrBuf0[3]=(WAZANO_SIROIKIRI&0xff00)>>8;
							EvStrBuf0[4]=EOM_;
							ret++;
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0){
					UserWork[FCC_WORK0]++;
					UserWork[FCC_WORK1]=0;
				}
#endif
				while(UserWork[FCC_WORK1]<2){
					ClientNo=AttackClient=dir=UserWork[FCC_WORK1];
					if(SCC[dir].shiroikiricnt){
						if(--SCC[dir].shiroikiricnt==0){
							SideCondition[dir]&=SIDE_CONDITION_SHIROIKIRI_OFF;
							ServerSubroutineSet((u8 *)&WAZA_ReflecEnd);
							ServerWork[5]=dir;
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_WAZANO;
							EvStrBuf0[2]=WAZANO_SIROIKIRI&0xff;
							EvStrBuf0[3]=(WAZANO_SIROIKIRI&0xff00)>>8;
							EvStrBuf0[4]=EOM_;
							ret++;
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0){
					UserWork[FCC_WORK0]++;
					UserWork[FCC_WORK1]=0;
				}
				break;
			case FCC_SHINPI: //����҂̂܂���
#if 0
				while(UserWork[FCC_WORK1]<ClientSetMax){
					ClientNo=AttackClient=ActionClientNo[UserWork[FCC_WORK1]];
					if(MineEnemyCheck(ClientNo)==SIDE_MINE)
						type=BSS_CLIENT_MINE;
					else
						type=BSS_CLIENT_ENEMY;
					if((FightType&FIGHT_TYPE_2vs2)&&
					  ((NoReshuffleClient&no2bittbl[ClientNoGet(type)])==0)&&
					   (ClientTypeGet(ClientNo)==(type+2))){
						UserWork[FCC_WORK1]++;
						continue;
					}
					dir=MineEnemyCheck(ClientNo);
					if(SideCondition[dir]&SIDE_CONDITION_SHINPI){
						if(--SCC[dir].shinpicnt==0){
							SideCondition[dir]&=
									SIDE_CONDITION_SHINPI_OFF;
							ServerSubroutineSet((u8 *)&WAZA_ShinpiEnd);
							ret++;
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0){
					UserWork[FCC_WORK0]++;
					UserWork[FCC_WORK1]=0;
				}
#endif
				while(UserWork[FCC_WORK1]<2){
					ClientNo=AttackClient=dir=UserWork[FCC_WORK1];
					if(SideCondition[dir]&SIDE_CONDITION_SHINPI){
						if(--SCC[dir].shinpicnt==0){
							SideCondition[dir]&=
									SIDE_CONDITION_SHINPI_OFF;
							ServerSubroutineSet((u8 *)&WAZA_ShinpiEnd);
							ret++;
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0){
					UserWork[FCC_WORK0]++;
					UserWork[FCC_WORK1]=0;
				}
				break;
			case FCC_NEGAIGOTO:		//�˂������ƃ`�F�b�N
				while(UserWork[FCC_WORK1]<ClientSetMax){
					ClientNo=ActionClientNo[UserWork[FCC_WORK1]];
					if(FCC.negaigotocnt[ClientNo]){
						if(--FCC.negaigotocnt[ClientNo]==0){
							if(PSP[ClientNo].hp){
								DefenceClient=ClientNo;
								ServerSubroutineSet((u8 *)&WAZA_Negaigoto);
								ret++;
							}
						}
					}
					UserWork[FCC_WORK1]++;
					if(ret)
						break;
				}
				if(ret==0)
					UserWork[FCC_WORK0]++;
				break;
			case FCC_AMAGOI:					//���܂����`�F�b�N
				if(FieldCondition&FIELD_CONDITION_AMEHURI){
					if((FieldCondition&FIELD_CONDITION_AMEHURASHI)==0){
						if(--FCC.FCCcnt==0){
							FieldCondition&=FIELD_CONDITION_AME_OFF;
							FieldCondition&=FIELD_CONDITION_OOAME_OFF;
							ServerWork[5]=2;
						}
						else{
							if(FieldCondition&FIELD_CONDITION_OOAME)
								ServerWork[5]=1;
							else
								ServerWork[5]=0;
						}
					}
					else{
						if(FieldCondition&FIELD_CONDITION_OOAME)
							ServerWork[5]=1;
						else
							ServerWork[5]=0;
					}
					ServerSubroutineSet((u8 *)&WAZA_Amefuri);
					ret++;
				}
				UserWork[FCC_WORK0]++;
				break;
			case FCC_SUNAARASHI:					//���Ȃ��炵�`�F�b�N
				if(FieldCondition&FIELD_CONDITION_SUNAARASHIING){
					if(FieldCondition&FIELD_CONDITION_SUNAARASHIW)
						WazaSeqAdrs=(u8 *)&WAZA_WeatherDamage;
					else{
						if(--FCC.FCCcnt==0){
							FieldCondition&=FIELD_CONDITION_SUNAARASHI_OFF;
							WazaSeqAdrs=(u8 *)&WAZA_WeatherDamageEnd;
						}
						else
							WazaSeqAdrs=(u8 *)&WAZA_WeatherDamage;
					}
					UserWork[FIGHT_EFFECT_WORK]=FE_ID_SUNAARASI;
					ServerWork[5]=0;
					ServerSubroutineSet(WazaSeqAdrs);
					ret++;
				}
				UserWork[FCC_WORK0]++;
				break;
			case FCC_NIHONBARE:					//�ɂق�΂�`�F�b�N
				if(FieldCondition&FIELD_CONDITION_HIDERING){
					if(FieldCondition&FIELD_CONDITION_HIDERI)
						WazaSeqAdrs=(u8 *)&WAZA_Hare;
					else{
						if(--FCC.FCCcnt==0){
							FieldCondition&=FIELD_CONDITION_HARE_OFF;
							WazaSeqAdrs=(u8 *)&WAZA_HareEnd;
						}
						else
							WazaSeqAdrs=(u8 *)&WAZA_Hare;
					}
					ServerSubroutineSet(WazaSeqAdrs);
					ret++;
				}
				UserWork[FCC_WORK0]++;
				break;
			case FCC_ARARE:						//�����`�F�b�N
				if(FieldCondition&FIELD_CONDITION_ARARE){
					if(--FCC.FCCcnt==0){
						FieldCondition&=FIELD_CONDITION_ARARE_OFF;
						WazaSeqAdrs=(u8 *)&WAZA_WeatherDamageEnd;
					}
					else
						WazaSeqAdrs=(u8 *)&WAZA_WeatherDamage;
					UserWork[FIGHT_EFFECT_WORK]=FE_ID_ARARE;
					ServerWork[5]=1;
					ServerSubroutineSet(WazaSeqAdrs);
					ret++;
				}
				UserWork[FCC_WORK0]++;
				break;
			case FCC_END:
				ret++;
				break;
		}
	}while(ret==0);

	return(ServerSequence!=ServerTurnCheck);
}

//==============================================================
//		�t�B�[���h�R���f�B�V�����`�F�b�N
//==============================================================

enum{
//HP�񕜌n���Ƀ`�F�b�N���ċC�₵�ɂ�������
	SIDE_CONDITION_CHECK_NEWOHARU=0,
	SIDE_CONDITION_CHECK_SPEABI,
	SIDE_CONDITION_CHECK_SOUBI,

	SIDE_CONDITION_CHECK_YADORIGI,
	SIDE_CONDITION_CHECK_DOKU,
	SIDE_CONDITION_CHECK_DOKUDOKU,
	SIDE_CONDITION_CHECK_YAKEDO,
	SIDE_CONDITION_CHECK_AKUMU,
	SIDE_CONDITION_CHECK_NOROI,
	SIDE_CONDITION_CHECK_SHIME,
	SIDE_CONDITION_CHECK_SAWAGU,
	SIDE_CONDITION_CHECK_ABARERU,
	SIDE_CONDITION_CHECK_KANASHIBARI,
	SIDE_CONDITION_CHECK_ENCORE,
	SIDE_CONDITION_CHECK_LOCKON,
	SIDE_CONDITION_CHECK_JUUDEN,
	SIDE_CONDITION_CHECK_CHOUHATSU,
	SIDE_CONDITION_CHECK_AKUBI,
	SIDE_CONDITION_CHECK_SOUBI2,
	SIDE_CONDITION_CHECK_END,
//	SIDE_CONDITION_CHECK_MIRAIYOCHI,
//	SIDE_CONDITION_CHECK_HOROBINOUTA,
};

u8	SideConditionCheck(void)
{
	int	i;
	u8	ret=0;
//	u8	dir;
//	u8	type;

	ServerStatusFlag|=(SERVER_STATUS_FLAG_NOGAMAN|SERVER_STATUS_FLAG_NO_MICHIDURE);
	//���܂�̃_���[�W���Z���s��Ȃ��悤�ɂ���
	//�݂��Â�͋N�����Ȃ��悤�ɂ���

	while((UserWork[SCC_WORK1]<ClientSetMax)&&
		  (UserWork[SCC_WORK0]<=SIDE_CONDITION_CHECK_END)){
		ClientNo=AttackClient=ActionClientNo[UserWork[SCC_WORK1]];
		if(NoReshuffleClient&no2bittbl[ClientNo]){
			UserWork[SCC_WORK1]++;
			continue;
		}
		switch(UserWork[SCC_WORK0]){
			case SIDE_CONDITION_CHECK_NEWOHARU:
				//�˂��͂�
				if((WazaKouka[ClientNo]&WAZAKOUKA_NEWOHARU)&&
				   (PSP[ClientNo].hp!=PSP[ClientNo].hpmax)&&
				   (PSP[ClientNo].hp!=0)){
					WazaDamage=PSP[ClientNo].hpmax/16;
					if(WazaDamage==0) WazaDamage=1;
					WazaDamage*=-1;
					ServerSubroutineSet((u8 *)&WAZA_Newoharu);
					ret++;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_SPEABI:
				//����\��
				if(SpecialAbirityCheck(SPEABI_TURN_CHECK,ClientNo,0,0,0))
					ret++;
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_SOUBI:
				//�����A�C�e��
				if(SoubiItemCheck(SOUBI_TURN_CHECK,ClientNo,0))
					ret++;
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_SOUBI2:
				//�����A�C�e��
				if(SoubiItemCheck(SOUBI_TURN_CHECK,ClientNo,1))
					ret++;
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_YADORIGI:
				//��ǂ肬�̂���
				if((WazaKouka[ClientNo]&WAZAKOUKA_YADORARE)&&
				   (PSP[WazaKouka[ClientNo]&WAZAKOUKA_YADORIGI].hp!=0)&&
				   (PSP[ClientNo].hp!=0)){
					DefenceClient=WazaKouka[ClientNo]&WAZAKOUKA_YADORIGI;
					WazaDamage=PSP[ClientNo].hpmax/8;
					if(WazaDamage==0) WazaDamage=1;
					UserWork[FIGHT_EFFECT_WORK+0]=DefenceClient;
					UserWork[FIGHT_EFFECT_WORK+1]=AttackClient;
					ServerSubroutineSet((u8 *)&WAZA_Yadorigi);
					ret++;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_DOKU:
				//�ǂ��`�F�b�N
				if((PSP[ClientNo].condition&CONDITION_DOKU)&&
				   (PSP[ClientNo].hp!=0)){
					WazaDamage=PSP[ClientNo].hpmax/8;
					if(WazaDamage==0) WazaDamage=1;
					ServerSubroutineSet((u8 *)&WAZA_Doku);
					ret++;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_DOKUDOKU:
				//�ǂ��ǂ��`�F�b�N
				if((PSP[ClientNo].condition&CONDITION_DOKUDOKU)&&
				   (PSP[ClientNo].hp!=0)){
					WazaDamage=PSP[ClientNo].hpmax/16;
					if(WazaDamage==0) WazaDamage=1;
					if((PSP[ClientNo].condition&CONDITION_DOKUDOKU_CNT)!=
						CONDITION_DOKUDOKU_CNT)
						PSP[ClientNo].condition+=DOKUDOKU_COUNT;
					WazaDamage*=((PSP[ClientNo].condition&
								CONDITION_DOKUDOKU_CNT)>>8);
					ServerSubroutineSet((u8 *)&WAZA_Dokudoku);
					ret++;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_YAKEDO:
				//�₯�ǃ`�F�b�N
				if((PSP[ClientNo].condition&CONDITION_YAKEDO)&&
				   (PSP[ClientNo].hp!=0)){
					WazaDamage=PSP[ClientNo].hpmax/8;
					if(WazaDamage==0) WazaDamage=1;
					ServerSubroutineSet((u8 *)&WAZA_Yakedo);
					ret++;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_AKUMU:
				//�����ރ`�F�b�N
				if((PSP[ClientNo].condition2&CONDITION2_AKUMU)&&
				   (PSP[ClientNo].hp!=0)){
//					WazaDamage=PSP[ClientNo].hpmax/8;
					WazaDamage=PSP[ClientNo].hpmax/4;
					if(WazaDamage==0) WazaDamage=1;
					ServerSubroutineSet((u8 *)&WAZA_Akumu);
					ret++;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_NOROI:
				//�̂낢�`�F�b�N
				if((PSP[ClientNo].condition2&CONDITION2_NOROI)&&
				   (PSP[ClientNo].hp!=0)){
//					WazaDamage=PSP[ClientNo].hpmax/8;
					WazaDamage=PSP[ClientNo].hpmax/4;
					if(WazaDamage==0) WazaDamage=1;
					ServerSubroutineSet((u8 *)&WAZA_Noroi);
					ret++;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_SHIME:
				//���ߌn�̋Z
				if((PSP[ClientNo].condition2&CONDITION2_SHIME)&&
				   (PSP[ClientNo].hp!=0)){
					PSP[ClientNo].condition2-=SHIME_COUNT;
					if(PSP[ClientNo].condition2&CONDITION2_SHIME){
						UserWork[FIGHT_EFFECT_WORK+0]=
							UserWork[SHIME_WAZANO+ClientNo*2];
						UserWork[FIGHT_EFFECT_WORK+1]=
							UserWork[SHIME_WAZANO+ClientNo*2+1];
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_WAZANO;
						EvStrBuf0[2]=UserWork[SHIME_WAZANO+ClientNo*2];
						EvStrBuf0[3]=UserWork[SHIME_WAZANO+ClientNo*2+1];
						EvStrBuf0[4]=EOM_;
						WazaSeqAdrs=(u8 *)&WAZA_Shime;
						WazaDamage=PSP[ClientNo].hpmax/16;
						if(WazaDamage==0)
							WazaDamage=1;
					}
					else{
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_WAZANO;
						EvStrBuf0[2]=UserWork[SHIME_WAZANO+ClientNo*2];
						EvStrBuf0[3]=UserWork[SHIME_WAZANO+ClientNo*2+1];
						EvStrBuf0[4]=EOM_;
						WazaSeqAdrs=(u8 *)&WAZA_ShimeEnd;
					}
					ServerSubroutineSet(WazaSeqAdrs);
					ret++;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_SAWAGU:
				//���킮�`�F�b�N
				if(PSP[ClientNo].condition2&CONDITION2_SAWAGU){
					for(AttackClient=0;AttackClient<ClientSetMax;AttackClient++){
						if((PSP[AttackClient].condition&CONDITION_NEMURI)&&
						   (PSP[AttackClient].speabino!=TOKUSYU_BOUON)){
							PSP[AttackClient].condition&=CONDITION_NEMURI_OFF;
							PSP[AttackClient].condition2&=CONDITION2_AKUMU_OFF;
							ServerWork[5]=1;
							ServerSubroutineSet((u8 *)&WAZA_SawaguMezame);
							ClientNo=AttackClient;
							BSS_POKEPARA_PUT_SET(BSS_SERVER,
												 BSS_PARA_CONDITION,
												 BSS_SELMONSNO,
												 4,
												 (u8 *)&PSP[ClientNo].condition);
							ClientBitSet(ClientNo);
							break;
						}
					}
					if(AttackClient!=ClientSetMax){
						ret=2;
						break;
					}
					AttackClient=ClientNo;
					PSP[ClientNo].condition2-=SAWAGU_COUNT;
//					if((OTF[ClientNo].mahiflag)||
//					   (OTF[ClientNo].koukanaiflag)||
//					   (OTF[ClientNo].konranflag)){
					if(WazaOutCheck(ClientNo)){
//						PSP[ClientNo].condition2&=CONDITION2_SAWAGU_OFF;
//						PSP[ClientNo].condition2&=CONDITION2_KEEP_OFF;
						ServerKeepOff(ClientNo);
						ServerWork[5]=1;
					}
					else if(PSP[ClientNo].condition2&CONDITION2_SAWAGU){
						ServerWork[5]=0;
						//�˂��ƂŁu���킮�v���o�����ꍇ��
						//KEEP�������Ă���̂ŗ��Ē���
						PSP[ClientNo].condition2|=CONDITION2_KEEP;
					}
					else{
						ServerWork[5]=1;
//						PSP[ClientNo].condition2&=CONDITION2_KEEP_OFF;
						ServerKeepOff(ClientNo);
					}
					ServerSubroutineSet((u8 *)&WAZA_Sawagu);
					ret=1;
				}
				if(ret!=2)
					UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_ABARERU:
				//���΂��`�F�b�N
				if(PSP[ClientNo].condition2&CONDITION2_ABARERU){
					PSP[ClientNo].condition2-=ABARERU_COUNT;
//�܂ЂōU�����o���Ȃ������Ƃ��́A���΂ꂨ���
//					if((OTF[ClientNo].mahiflag)||
//					   (OTF[ClientNo].koukanaiflag)||
//					   (OTF[ClientNo].konranflag)){
					if(WazaOutCheck(ClientNo)){
//						PSP[ClientNo].condition2&=CONDITION2_ABARERU_OFF;
//						PSP[ClientNo].condition2&=CONDITION2_KEEP_OFF;
						ServerKeepOff(ClientNo);
					}
//KEEP�������Ă���Ƃ��́A�����I�ɋZ���Ƃ߂��Ă���̂ŁA
//�������Ȃ��悤�ɂ���
					else if(((PSP[ClientNo].condition2&CONDITION2_ABARERU)==0)&&
						(PSP[ClientNo].condition2&CONDITION2_KEEP)){
						PSP[ClientNo].condition2&=CONDITION2_KEEP_OFF;
						if((PSP[ClientNo].condition2&CONDITION2_KONRAN)==0){
							ServerWork[3]=COND2_KONRAN|TSUIKA_ATTACK;
							TsuikaAct(TSUIKA_DIRECT,0);
							if(PSP[ClientNo].condition2&CONDITION2_KONRAN)
								ServerSubroutineSet((u8 *)&WAZA_AbareruEnd);
							ret++;
						}
					}
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_KANASHIBARI:
				//���Ȃ��΂�
				if(WKC[ClientNo].kanashibaricnt!=0){
					for(i=0;i<4;i++){
						if(WKC[ClientNo].kanashibariwazano==PSP[ClientNo].waza[i])
							break;
					}
					//�Z�Y��ŁA���Ȃ��΂肳��Ă����Z���Ȃ��Ƃ��̏���
					if(i==4){
						WKC[ClientNo].kanashibariwazano=0;
						WKC[ClientNo].kanashibaricnt=0;
					}
					else if(--WKC[ClientNo].kanashibaricnt==0){
						WKC[ClientNo].kanashibariwazano=0;
						ServerSubroutineSet((u8 *)&WAZA_KanashibariEnd);
						ret++;
					}
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_ENCORE:
				//�A���R�[��
				if(WKC[ClientNo].encorecnt!=0){
					//�Z�Y��ŁA�A���R�[������Ă����Z���Ȃ��Ƃ��̏���
					if(PSP[ClientNo].waza[WKC[ClientNo].encorewazapos]!=
					   WKC[ClientNo].encorewazano){
						WKC[ClientNo].encorewazano=0;
						WKC[ClientNo].encorecnt=0;
					}
					else if((--WKC[ClientNo].encorecnt==0)||
							(PSP[ClientNo].pp[WKC[ClientNo].encorewazapos]==0)){
						WKC[ClientNo].encorewazano=0;
						WKC[ClientNo].encorecnt=0;
						ServerSubroutineSet((u8 *)&WAZA_EncoreEnd);
						ret++;
					}
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_LOCKON:
				//���b�N�I��
				if(WazaKouka[ClientNo]&WAZAKOUKA_LOCKON)
					WazaKouka[ClientNo]-=LOCKON_COUNT;
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_JUUDEN:
				//���イ�ł�
				if(WKC[ClientNo].juudencnt){
					if(--WKC[ClientNo].juudencnt==0)
						WazaKouka[ClientNo]&=WAZAKOUKA_JUUDEN_OFF;
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_CHOUHATSU:
				//���傤�͂�
				if(WKC[ClientNo].chouhatsucnt)
					WKC[ClientNo].chouhatsucnt--;
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_AKUBI:
				//������
				if(WazaKouka[ClientNo]&WAZAKOUKA_AKUBI){
					WazaKouka[ClientNo]-=AKUBI_COUNT;
					if(((WazaKouka[ClientNo]&WAZAKOUKA_AKUBI)==0)&&
					   ((PSP[ClientNo].condition&CONDITION_BAD)==0)&&
						(PSP[ClientNo].speabino!=TOKUSYU_YARUKI)&&
						(PSP[ClientNo].speabino!=TOKUSYU_HUMIN)&&
					    (SawaguCheck(ClientNo)==0)){
//						PSP[ClientNo].condition2&=CONDITION2_KEEP_OFF;
						ServerKeepOff(ClientNo);
						PSP[ClientNo].condition|=((pp_rand()&0x03)+2);
						BSS_POKEPARA_PUT_SET(BSS_SERVER,
											 BSS_PARA_CONDITION,
											 BSS_SELMONSNO,
											 4,
											 (u8 *)&PSP[ClientNo].condition);
						ClientBitSet(ClientNo);
						TsuikaClient=ClientNo;
						ServerSubroutineSet((u8 *)&WAZA_AddNemuri2);
						ret++;
					}
				}
				UserWork[SCC_WORK0]++;
				break;
			case SIDE_CONDITION_CHECK_END:
				UserWork[SCC_WORK0]=0;
				UserWork[SCC_WORK1]++;
				break;
			default:
				break;
		}
		if(ret)
			return ret;
	}
#if 0
	if(UserWork[SCC_WORK0]==0){
		UserWork[SCC_WORK0]=SIDE_CONDITION_CHECK_MIRAIYOCHI;
		UserWork[SCC_WORK1]=0;
	}
	switch(UserWork[SCC_WORK0]){
		case SIDE_CONDITION_CHECK_MIRAIYOCHI:
			while(UserWork[SCC_WORK1]<ClientSetMax){
				ClientNo=UserWork[SCC_WORK1];
				if(NoReshuffleClient&no2bittbl[ClientNo]){
					UserWork[SCC_WORK1]++;
					continue;
				}
				UserWork[SCC_WORK1]++;
				if(FCC.miraiyochicnt[ClientNo]){
					if((--FCC.miraiyochicnt[ClientNo]==0)&&(PSP[ClientNo].hp!=0)){
						if(FCC.miraiyochiwazano[ClientNo]==WAZANO_MIRAIYOTI)
							ServerWork[5]=0;
						else
							ServerWork[5]=1;
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_WAZANO;
						EvStrBuf0[2]=FCC.miraiyochiwazano[ClientNo]&0xff;
						EvStrBuf0[3]=(FCC.miraiyochiwazano[ClientNo]&0xff00)>>8;
						EvStrBuf0[4]=EOM_;
						DefenceClient=ClientNo;
						AttackClient=FCC.miraiyochiattackclient[ClientNo];
						WazaDamage=FCC.miraiyochidamage[ClientNo];
						OSTF[DefenceClient].kaigaradamage=0xffff;	//��������̂����ŉ񕜂��Ȃ��悤�ɂ���
						ServerSubroutineSet((u8 *)&WAZA_Miraiyochi);
						return 1;
					}
				}
			}
			UserWork[SCC_WORK0]=SIDE_CONDITION_CHECK_HOROBINOUTA;
			UserWork[SCC_WORK1]=0;
		case SIDE_CONDITION_CHECK_HOROBINOUTA:
			while(UserWork[SCC_WORK1]<ClientSetMax){
				ClientNo=AttackClient=ActionClientNo[UserWork[SCC_WORK1]];
				if(NoReshuffleClient&no2bittbl[ClientNo]){
					UserWork[SCC_WORK1]++;
					continue;
				}
				UserWork[SCC_WORK1]++;
				//�ق�т̂���
				if(WazaKouka[ClientNo]&WAZAKOUKA_HOROBINOUTA){
					EvStrBuf0[0]=I_MSG;
					EvStrBuf0[1]=EV_NUM;
					EvStrBuf0[2]=1;
					EvStrBuf0[3]=1;
					EvStrBuf0[4]=WKC[ClientNo].horobinoutacnt;
					EvStrBuf0[5]=EOM_;
							
					if(WKC[ClientNo].horobinoutacnt==0){
						WazaKouka[ClientNo]&=WAZAKOUKA_HOROBINOUTA_OFF;
						WazaDamage=PSP[ClientNo].hp;
						WazaSeqAdrs=(u8 *)&WAZA_Horobinouta;
					}
					else{
						WKC[ClientNo].horobinoutacnt--;
						WazaSeqAdrs=(u8 *)&WAZA_Horobinoutaing;
					}
					ServerSubroutineSet(WazaSeqAdrs);
					return 1;
				}
			}
			break;
	}
#endif
	ServerStatusFlag&=(SERVER_STATUS_FLAG_NOGAMAN_OFF&SERVER_STATUS_FLAG_NO_MICHIDURE_OFF);
	return 0;
}

enum{
	SIDE_CONDITION_CHECK_MIRAIYOCHI=0,
	SIDE_CONDITION_CHECK_HOROBINOUTA,
};

u8	SideConditionCheck2(void)
{
	ServerStatusFlag|=(SERVER_STATUS_FLAG_NOGAMAN|SERVER_STATUS_FLAG_NO_MICHIDURE);
	//���܂�̃_���[�W���Z���s��Ȃ��悤�ɂ���
	//�݂��Â�͋N�����Ȃ��悤�ɂ���
	switch(UserWork[SCC2_WORK0]){
		case SIDE_CONDITION_CHECK_MIRAIYOCHI:
			while(UserWork[SCC2_WORK1]<ClientSetMax){
				ClientNo=UserWork[SCC2_WORK1];
				if(NoReshuffleClient&no2bittbl[ClientNo]){
					UserWork[SCC2_WORK1]++;
					continue;
				}
				UserWork[SCC2_WORK1]++;
				if(FCC.miraiyochicnt[ClientNo]){
					if((--FCC.miraiyochicnt[ClientNo]==0)&&(PSP[ClientNo].hp!=0)){
						if(FCC.miraiyochiwazano[ClientNo]==WAZANO_MIRAIYOTI)
							ServerWork[5]=0;
						else
							ServerWork[5]=1;
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_WAZANO;
						EvStrBuf0[2]=FCC.miraiyochiwazano[ClientNo]&0xff;
						EvStrBuf0[3]=(FCC.miraiyochiwazano[ClientNo]&0xff00)>>8;
						EvStrBuf0[4]=EOM_;
						DefenceClient=ClientNo;
						AttackClient=FCC.miraiyochiattackclient[ClientNo];
						WazaDamage=FCC.miraiyochidamage[ClientNo];
						OSTF[DefenceClient].kaigaradamage=0xffff;	//��������̂����ŉ񕜂��Ȃ��悤�ɂ���
						ServerSubroutineSet((u8 *)&WAZA_Miraiyochi);
						return 1;
					}
				}
			}
			UserWork[SCC2_WORK0]=SIDE_CONDITION_CHECK_HOROBINOUTA;
			UserWork[SCC2_WORK1]=0;
		case SIDE_CONDITION_CHECK_HOROBINOUTA:
			while(UserWork[SCC2_WORK1]<ClientSetMax){
				ClientNo=AttackClient=ActionClientNo[UserWork[SCC2_WORK1]];
				if(NoReshuffleClient&no2bittbl[ClientNo]){
					UserWork[SCC2_WORK1]++;
					continue;
				}
				UserWork[SCC2_WORK1]++;
				//�ق�т̂���
				if(WazaKouka[ClientNo]&WAZAKOUKA_HOROBINOUTA){
					EvStrBuf0[0]=I_MSG;
					EvStrBuf0[1]=EV_NUM;
					EvStrBuf0[2]=1;
					EvStrBuf0[3]=1;
					EvStrBuf0[4]=WKC[ClientNo].horobinoutacnt;
					EvStrBuf0[5]=EOM_;
							
					if(WKC[ClientNo].horobinoutacnt==0){
						WazaKouka[ClientNo]&=WAZAKOUKA_HOROBINOUTA_OFF;
						WazaDamage=PSP[ClientNo].hp;
						WazaSeqAdrs=(u8 *)&WAZA_Horobinouta;
					}
					else{
						WKC[ClientNo].horobinoutacnt--;
						WazaSeqAdrs=(u8 *)&WAZA_Horobinoutaing;
					}
					ServerSubroutineSet(WazaSeqAdrs);
					return 1;
				}
			}
			break;
	}
	ServerStatusFlag&=(SERVER_STATUS_FLAG_NOGAMAN_OFF&SERVER_STATUS_FLAG_NO_MICHIDURE_OFF);
	return 0;
}

//==============================================================
//		�Z���ʃ`�F�b�N
//==============================================================

#if 0
u8	WazaKoukaCheck(void)
{
	u8	ret=0;
	int	i;

	do{
		switch(UserWork[WKC_WORK0]){
			case WKC_IKARI_CHECK:
				if((PSP[DefenceClient].condition2&CONDITION2_IKARI)&&
				   (PSP[DefenceClient].hp!=0)){	//������`�F�b�N
					if((AttackClient!=DefenceClient)&&
					  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					  ((OTF[DefenceClient].butsurihitdamage)||
					   (OTF[DefenceClient].tokusyuhitdamage))&&
					   (WTD[WazaNo].damage)){
						if(PSP[DefenceClient].abiritycnt[COND_POW]<12){
							PSP[DefenceClient].abiritycnt[COND_POW]++;
							WazaSeqAdrs=(u8 *)&WAZA_Ikari;
							ret=1;
						}
					}
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_TOKERU_CHECK:
				if((PSP[DefenceClient].condition&CONDITION_KOORI)&&
				   (PSP[DefenceClient].hp!=0)){	//�Ƃ���`�F�b�N
					if((AttackClient!=DefenceClient)&&
						((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
						if(WTD[WazaNo].wazatype==FIRE_TYPE){
							PSP[DefenceClient].condition&=CONDITION_KOORI_OFF;
							ClientNo=DefenceClient;
							BSS_POKEPARA_PUT_SET(BSS_SERVER,
												BSS_PARA_CONDITION,
												BSS_SELMONSNO,
												4,
												(u8 *)&PSP[DefenceClient].condition);
							ClientBitSet(ClientNo);
							WazaSeqAdrs=(u8 *)&WAZA_Melt2;
							ret=1;
						}
					}
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_SPEABI_SINKURO_DEFENCE:
				if(SpecialAbirityCheck(SPEABI_SINKURO_DEFENCE,DefenceClient,0,0,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
			case WKC_SPEABI_CHECK:
				if(SpecialAbirityCheck(SPEABI_WAZA_HIT_AFTER,DefenceClient,0,0,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
			case WKC_SPEABI_SINKURO_ATTACK:
				if(SpecialAbirityCheck(SPEABI_SINKURO_ATTACK,AttackClient,0,0,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
			case WKC_SOUBI_CHECK:
				if(SoubiItemCheck(SOUBI_WAZA_HIT_AFTER,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
			case WKC_VANISH_ATTACK_ON:
				if((WazaKouka[AttackClient]&WAZAKOUKA_KIE)&&
				   (ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF)){
					ClientNo=AttackClient;
					BSS_VANISH_ON_OFF_SET(BSS_SERVER,BSS_VANISH_ON);
					ClientBitSet(ClientNo);
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_VANISH_ATTACK_CHECK:
				if((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)||
				  ((WazaKouka[AttackClient]&WAZAKOUKA_KIE)==0)||
				   (OTF[AttackClient].mahiflag)||
				   (OTF[AttackClient].konranflag)){
					ClientNo=AttackClient;
					BSS_VANISH_ON_OFF_SET(BSS_SERVER,BSS_VANISH_OFF);
					ClientBitSet(ClientNo);
					WazaKouka[AttackClient]&=WAZAKOUKA_KIE_OFF;
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_VANISH_DEFENCE_CHECK:
				if((PSP[DefenceClient].condition2&CONDITION2_HIRUMU)&&
				   (DefenceClient<ClientSetMax)){		//�Ђ�ރ`�F�b�N
					ClientNo=DefenceClient;
					BSS_VANISH_ON_OFF_SET(BSS_SERVER,BSS_VANISH_OFF);
					ClientBitSet(ClientNo);
					WazaKouka[DefenceClient]&=WAZAKOUKA_KIE_OFF;
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_MIGAWARI_CHECK:
				for(i=0;i<ClientSetMax;i++){
					if(WKC[i].migawarihp==0)
						PSP[i].condition2&=CONDITION2_MIGAWARI_OFF;
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_END:
				break;
		}
	}while((UserWork[WKC_WORK0]!=WKC_END)&&(ret==0));
	return ret;
}
#endif

//==============================================================
//		�o���l�Q�b�g�`�F�b�N
//==============================================================

enum{
	GEC_GET_EXP_CHECK_INIT=0,
	GEC_GET_EXP_CHECK,
	GEC_GET_EXP,
	GEC_ZENMETSU_CHECK_INIT,
	GEC_ZENMETSU_CHECK,
	GEC_ZENMETSU,
	GEC_APPEAR_AFTER_CHECK,
	GEC_END,
};

u8	GetExpCheck()
{
	int	i;
//�T�t�@���]�[���ɂ͕K�v�Ȃ������Ȃ̂�
	if(FightType&FIGHT_TYPE_SAFARI)
		return 0;

	do{
		switch(UserWork[GEC_WORK0]){
			case GEC_GET_EXP_CHECK_INIT:
				UserWork[GEC_WORK1]=0;
				UserWork[GEC_WORK0]++;
				for(i=0;i<ClientSetMax;i++){
					if((NoReshuffleClient&no2bittbl[i])&&(PokeReshuffleCheck(i,6,6)==0))
						NoReshuffleClient&=(no2bittbl[i]^0xffffffff);
				}
			case GEC_GET_EXP_CHECK:
				do{
					KizetsuClient=DefenceClient=UserWork[GEC_WORK1];
					if((PSP[UserWork[GEC_WORK1]].hp==0)&&
					  ((UserWork[EXP_SELMONS_BIT]&no2bittbl[SelMonsNo[UserWork[GEC_WORK1]]])==0)&&
//				      ((ServerStatusFlag&no2bittbl[ClientTypeGet(UserWork[GEC_WORK1])])==0)){
				      ((NoReshuffleClient&no2bittbl[UserWork[GEC_WORK1]])==0)){
//						WazaSeqAdrs=(u8 *)&WAZA_GetEXP;
						ServerSubroutineSet((u8 *)&WAZA_GetEXP);
						UserWork[GEC_WORK0]=GEC_GET_EXP;
						return 1;
					}
				}while(++UserWork[GEC_WORK1]!=ClientSetMax);
				UserWork[GEC_WORK0]=GEC_GET_EXP+1;
				break;
			case GEC_GET_EXP:
				FightJoinFlagInitClient(KizetsuClient);
				if(++UserWork[GEC_WORK1]==ClientSetMax)
					UserWork[GEC_WORK0]=GEC_ZENMETSU_CHECK_INIT;
				else
					UserWork[GEC_WORK0]=GEC_GET_EXP_CHECK;
				break;
			case GEC_ZENMETSU_CHECK_INIT:
				UserWork[GEC_WORK1]=0;
				UserWork[GEC_WORK0]++;
			case GEC_ZENMETSU_CHECK:
				do{
					KizetsuClient=DefenceClient=UserWork[GEC_WORK1];
					if((PSP[UserWork[GEC_WORK1]].hp==0)&&
					   (NoReshuffleClient&no2bittbl[UserWork[GEC_WORK1]])==0){
						ServerSubroutineSet((u8 *)&WAZA_Zenmetsu);
						UserWork[GEC_WORK0]=GEC_ZENMETSU;
						return 1;
					}
				}while(++UserWork[GEC_WORK1]!=ClientSetMax);
				UserWork[GEC_WORK0]=GEC_ZENMETSU+1;
				break;
			case GEC_ZENMETSU:
				if(++UserWork[GEC_WORK1]==ClientSetMax)
					UserWork[GEC_WORK0]=GEC_ZENMETSU+1;
				else
					UserWork[GEC_WORK0]=GEC_ZENMETSU_CHECK;
				break;
			case GEC_APPEAR_AFTER_CHECK:
				if(SpecialAbirityCheck(SPEABI_IKAKU,0,0,0,0)) return 1;
				if(SpecialAbirityCheck(SPEABI_TRACE,0,0,0,0)) return 1;
				if(SoubiItemCheck(SOUBI_TURN_CHECK,0,1)) return 1;
				if(SpecialAbirityCheck(SPEABI_WAZA_HIT_AFTER_TENKIYA,0,0,0,0)) return 1;
				UserWork[GEC_WORK0]++;
				break;
			case GEC_END:
				break;
		}
	}while(UserWork[GEC_WORK0]!=GEC_END);

	return	0;
}

//==============================================================
//		�|�P�����R���f�B�V�����`�F�b�N
//==============================================================
//�ǂ̃`�F�b�N�̃v���C�I���e�B�������邩�������ƌ�������悤�ɁI�I

void	ServerWazaOutBeforeCheck(void)
{
	int	i;

	for(i=0;i<ClientSetMax;i++){
		if(PSP[i].condition2&CONDITION2_IKARI){	//������`�F�b�N
			if(SelectWazaNo[i]!=WAZANO_IKARI)
				PSP[i].condition2&=CONDITION2_IKARI_OFF;
		}
	}
}

enum{
	PCC_INIT=0,		//�`�F�b�N������
	PCC_NEMURI,		//�˂ނ�`�F�b�N
	PCC_KOORI,		//������`�F�b�N
	PCC_NAMAKE,		//�Ȃ܂��`�F�b�N
	PCC_HANDOU,		//�͂�ǂ��`�F�b�N
	PCC_HIRUMU,		//�Ђ�ރ`�F�b�N
	PCC_KANASIBARI,	//���Ȃ��΂�`�F�b�N
	PCC_TYOUHATU,	//���傤�͂`�F�b�N
	PCC_HUUIN,		//�ӂ�����`�F�b�N
	PCC_KONRAN,		//������`�F�b�N
	PCC_MAHI,		//�܂Ѓ`�F�b�N
	PCC_MEROMERO,	//���������`�F�b�N
	PCC_GAMAN,		//���܂�`�F�b�N
	PCC_MELT,		//�Ƃ���`�F�b�N
	PCC_END,		//�`�F�b�N�G���h
};

u8	PokeConditionCheck(void)
{
	int	i=0;
	int	ret=0;
	s32	*gamandamage;

	gamandamage=(s32 *)&UserWork[GAMANDAMAGE];

	do{
		switch(UserWork[POKECONDITION_CHECK]){
			case PCC_INIT:			//�`�F�b�N������
				PSP[AttackClient].condition2&=CONDITION2_MICHIDURE_OFF;
				WazaKouka[AttackClient]&=WAZAKOUKA_ONNEN_OFF;
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_NEMURI:		//�˂ނ�`�F�b�N
				if(PSP[AttackClient].condition&CONDITION_NEMURI){		//�˂ނ�`�F�b�N
					if(SawaguCheck(AttackClient)){
						PSP[AttackClient].condition&=CONDITION_NEMURI_OFF;
						PSP[AttackClient].condition2&=CONDITION2_AKUMU_OFF;
						PushAdrsAct();
						ServerWork[5]=1;
						WazaSeqAdrs=(u8 *)&WAZA_Mezame;
						ret=2;
					}
					else{
			//����\�͂͂₨���́A�͂₭�˂ނ肩�炳�߂�
						if(PSP[AttackClient].speabino==TOKUSYU_HAYAOKI)
							i=2;
						else
							i=1;

						if((PSP[AttackClient].condition&CONDITION_NEMURI)<i)
							PSP[AttackClient].condition&=CONDITION_NEMURI_OFF;
						else
							PSP[AttackClient].condition-=i;

						if(PSP[AttackClient].condition&CONDITION_NEMURI){
							if((WazaNo!=WAZANO_IBIKI)&&(WazaNo!=WAZANO_NEGOTO)){
								WazaSeqAdrs=(u8 *)&WAZA_Nemuru;
								//2�̂ɂ�����Z���o���Ă���Ƃ���
								//DOUBLECHECK�������Ȃ����߂̏���
								ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
								ret=2;
							}
						}
						else{
							PSP[AttackClient].condition2&=CONDITION2_AKUMU_OFF;
							PushAdrsAct();
							ServerWork[5]=0;
							WazaSeqAdrs=(u8 *)&WAZA_Mezame;
							ret=2;
						}
					}
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_KOORI:			//������`�F�b�N
				if(PSP[AttackClient].condition&CONDITION_KOORI){		//������`�F�b�N
#if 0
					if((pp_rand()%5!=0)&&(WTD[WazaNo].battleeffect!=125)){
						WazaSeqAdrs=(u8 *)&WAZA_Koori;
						//2�̂ɂ�����Z���o���Ă���Ƃ���
						//DOUBLECHECK�������Ȃ����߂̏���
						ServerStatusFlag|=SERVER_STATUS_FLAG_DOUBLE_CHECK;
					}
					else{
						PSP[AttackClient].condition&=CONDITION_KOORI_OFF;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Melt;
						if(WTD[WazaNo].battleeffect!=125)
							ServerWork[5]=0;
						else
							ServerWork[5]=1;
					}
					ret=2;
#else
					if(pp_rand()%5!=0){
						if(WTD[WazaNo].battleeffect!=125){
							WazaSeqAdrs=(u8 *)&WAZA_Koori;
							//2�̂ɂ�����Z���o���Ă���Ƃ���
							//DOUBLECHECK�������Ȃ����߂̏���
							ServerStatusFlag|=SERVER_STATUS_FLAG_DOUBLE_CHECK;
						}
						else{
							UserWork[POKECONDITION_CHECK]++;
							break;
						}
					}
					else{
						PSP[AttackClient].condition&=CONDITION_KOORI_OFF;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Melt;
						ServerWork[5]=0;
					}
					ret=2;
#endif
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_NAMAKE:		//�Ȃ܂��`�F�b�N
//����\�͂Ȃ܂��́A2���1�񂵂��Z�������Ȃ�
				if(PSP[AttackClient].speabino==TOKUSYU_NAMAKE){
//					WKC[AttackClient].namakebit^=1;
					if(WKC[AttackClient].namakebit){
						ServerKeepOff(AttackClient);
						//2�̂ɂ�����Z���o���Ă���Ƃ���
						//DOUBLECHECK�������Ȃ����߂̏���
						ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
						ServerWork[5]=0;
						WazaSeqAdrs=(u8 *)&WAZA_Namakeru;		//���ߖ����i�Ȃ܂���j
						WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
						ret=1;
					}
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_HANDOU:		//�͂�ǂ��`�F�b�N
				if(PSP[AttackClient].condition2&CONDITION2_HANDOU){
					PSP[AttackClient].condition2&=CONDITION2_HANDOU_OFF;
					WKC[AttackClient].handoucnt=0;
					ServerKeepOff(AttackClient);
					WazaSeqAdrs=(u8 *)&WAZA_Handou;
					//2�̂ɂ�����Z���o���Ă���Ƃ���
					//DOUBLECHECK�������Ȃ����߂̏���
					ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_HIRUMU:		//�Ђ�ރ`�F�b�N
				if(PSP[AttackClient].condition2&CONDITION2_HIRUMU){
					PSP[AttackClient].condition2&=CONDITION2_HIRUMU_OFF;
					OTF[AttackClient].hirumuflag=1;
					ServerKeepOff(AttackClient);
					WazaSeqAdrs=(u8 *)&WAZA_Hirumu;
					//2�̂ɂ�����Z���o���Ă���Ƃ���
					//DOUBLECHECK�������Ȃ����߂̏���
					ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_KANASIBARI:	//���Ȃ��΂�`�F�b�N
				if((WKC[AttackClient].kanashibariwazano==WazaNo)&&(WazaNo!=0)){
					OTF[AttackClient].kanashibariflag=1;
					UserWork[CLIENT_NO_WORK]=AttackClient;
					ServerKeepOff(AttackClient);
					WazaSeqAdrs=(u8 *)&WAZA_Kanashibari;
					//2�̂ɂ�����Z���o���Ă���Ƃ���
					//DOUBLECHECK�������Ȃ����߂̏���
					ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_TYOUHATU:		//���傤�͂`�F�b�N
				if((WKC[AttackClient].chouhatsucnt)&&((WTD[WazaNo].damage)==0)){
					OTF[AttackClient].chouhatsuflag=1;
					ServerKeepOff(AttackClient);
					WazaSeqAdrs=(u8 *)&WAZA_Chouhatsu2;
					//2�̂ɂ�����Z���o���Ă���Ƃ���
					//DOUBLECHECK�������Ȃ����߂̏���
					ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
#if 0
			case PCC_ITYAMON:		//���������`�F�b�N
				if((WazaNo==WazaNoUse[AttackClient])&&
				   (PSP[AttackClient].condition2&CONDITION2_ICHAMON)){
					OTF[AttackClient].ichamonflag=1;
					ServerKeepOff(AttackClient);
					WazaSeqAdrs=(u8 *)&WAZA_Ichamon2;
					//2�̂ɂ�����Z���o���Ă���Ƃ���
					//DOUBLECHECK�������Ȃ����߂̏���
					ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
#endif
			case PCC_HUUIN:			//�ӂ�����`�F�b�N
				if(HuuinCheck(AttackClient,WazaNo)){
					OTF[AttackClient].huuinflag=1;
					ServerKeepOff(AttackClient);
					WazaSeqAdrs=(u8 *)&WAZA_Huuin;
					//2�̂ɂ�����Z���o���Ă���Ƃ���
					//DOUBLECHECK�������Ȃ����߂̏���
					ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_KONRAN:		//������`�F�b�N
				if(PSP[AttackClient].condition2&CONDITION2_KONRAN){		//������`�F�b�N
					PSP[AttackClient].condition2-=KONRAN_COUNT;
					if(PSP[AttackClient].condition2&CONDITION2_KONRAN){
						if(pp_rand()&1){
							ServerWork[5]=0;
							PushAdrsAct();
						}
						else{
							ServerWork[5]=1;
							DefenceClient=AttackClient;
							WazaDamage=WazaDamageCalc(
											&PSP[AttackClient],
											&PSP[AttackClient],
											WAZANO_HATAKU,
											0,
											40,
											0,
											AttackClient,
											AttackClient);
							//�A���n�A���ߌn�͉���
							OTF[AttackClient].konranflag=1;
							//�����ŌĂԂƏ����Ă���Ƃ��ł��A
							//�����G�t�F�N�g���o�Ă��܂��̂�
							//�Z�V�[�P���X�̒���ServerKeepOff���Ă�
//							ServerKeepOff(AttackClient);
							//2�̂ɂ�����Z���o���Ă���Ƃ���
							//DOUBLECHECK�������Ȃ����߂̏���
							ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
						}
						WazaSeqAdrs=(u8 *)&WAZA_Konran;
					}
					else{
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_KonranEnd;
					}
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_MAHI:			//�܂Ѓ`�F�b�N
				if(PSP[AttackClient].condition&CONDITION_MAHI){
					if(pp_rand()%4==0){
						OTF[AttackClient].mahiflag=1;
						ServerKeepOff(AttackClient);
						WazaSeqAdrs=(u8 *)&WAZA_Mahi;
						//2�̂ɂ�����Z���o���Ă���Ƃ���
						//DOUBLECHECK�������Ȃ����߂̏���
						ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
						ret=1;
					}
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_MEROMERO:		//���������`�F�b�N
				if(PSP[AttackClient].condition2&CONDITION2_MEROMERO){
					UserWork[CLIENT_NO_WORK]=
						Bit2No((PSP[AttackClient].condition2&CONDITION2_MEROMERO)>>MEROMERO_SHIFT);
					if(pp_rand()&1)
						PushAdrsAct();
					else{
						PushAdrsSet((u8 *)&WAZA_MeromeroNext);
						//2�̂ɂ�����Z���o���Ă���Ƃ���
						//DOUBLECHECK�������Ȃ����߂̏���
						ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
						OTF[AttackClient].meromeroflag=1;
						ServerKeepOff(AttackClient);
					}
					WazaSeqAdrs=(u8 *)&WAZA_Meromero;
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_GAMAN:			//���܂�`�F�b�N
				if(PSP[AttackClient].condition2&CONDITION2_GAMAN){
					PSP[AttackClient].condition2-=GAMAN_COUNT;
					if(PSP[AttackClient].condition2&CONDITION2_GAMAN)
						WazaSeqAdrs=(u8 *)&WAZA_Gaman;
					else{
						PSP[AttackClient].condition2&=CONDITION2_KEEP_OFF;
						if(StoreDamage[AttackClient]){
							WazaNo=WAZANO_GAMAN;
							gamandamage[0]=StoreDamage[AttackClient]*2;
							DefenceClient=ClientNoHit[AttackClient];
							if(NoReshuffleClient&no2bittbl[DefenceClient])
								DefenceClient=ServerDefenceClientGet(WAZANO_GAMAN,RANGE_NORMAL+1);
							WazaSeqAdrs=(u8 *)&WAZA_GamanOff;
						}
						else
							WazaSeqAdrs=(u8 *)&WAZA_GamanFaild;
					}
					ret=1;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_MELT:			//�Ƃ���`�F�b�N
				if(PSP[AttackClient].condition&CONDITION_KOORI){
					if(WTD[WazaNo].battleeffect==125){
						PSP[AttackClient].condition&=CONDITION_KOORI_OFF;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Melt;
						ServerWork[5]=1;
					}
					ret=2;
				}
				UserWork[POKECONDITION_CHECK]++;
				break;
			case PCC_END:			//�`�F�b�N�G���h
				break;
		}
	}while((UserWork[POKECONDITION_CHECK]!=PCC_END)&&(ret==0));
	if(ret==2){
		ClientNo=AttackClient;
		BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 BSS_PARA_CONDITION,
							 BSS_SELMONSNO,
							 4,
							 (u8 *)&PSP[ClientNo].condition);
		ClientBitSet(ClientNo);
	}
	return	ret;
}

#if 0
u8	PokeConditionCheck(void)
{
	int	i=0;
	int	ret=0;
	s32	*gamandamage;

	gamandamage=(s32 *)&UserWork[GAMANDAMAGE];

	if(ServerStatusFlag&SERVER_STATUS_FLAG_CHECKED) return 0;

	ServerStatusFlag|=SERVER_STATUS_FLAG_CHECKED;

	PSP[AttackClient].condition2&=CONDITION2_MICHIDURE_OFF;

	if(PSP[AttackClient].condition2&CONDITION2_HIRUMU){		//�Ђ�ރ`�F�b�N
		PSP[AttackClient].condition2&=CONDITION2_HIRUMU_OFF;
		OTF[AttackClient].hirumuflag=1;
		ServerKeepOff(AttackClient);
//		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		WazaSeqAdrs=(u8 *)&WAZA_Hirumu;
		//2�̂ɂ�����Z���o���Ă���Ƃ���
		//DOUBLECHECK�������Ȃ����߂̏���
		ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
		return 1;
	}

	if(PSP[AttackClient].condition&CONDITION_NEMURI){		//�˂ނ�`�F�b�N
		if(SawaguCheck()){
			PSP[AttackClient].condition&=CONDITION_NEMURI_OFF;
			PSP[AttackClient].condition2&=CONDITION2_AKUMU_OFF;
			PushAdrsAct();
			ServerWork[5]=1;
			WazaSeqAdrs=(u8 *)&WAZA_Mezame;
			ret=1;
		}
		else{
//����\�͂͂₨���́A�͂₭�˂ނ肩�炳�߂�
			if(PSP[AttackClient].speabino==TOKUSYU_HAYAOKI)
				i=2;
			else
				i=1;

			if((PSP[AttackClient].condition&CONDITION_NEMURI)<i)
				PSP[AttackClient].condition&=CONDITION_NEMURI_OFF;
			else
				PSP[AttackClient].condition-=i;

			if(PSP[AttackClient].condition&CONDITION_NEMURI){
				if((WazaNo!=WAZANO_IBIKI)&&
				   (WazaNo!=WAZANO_NEGOTO)){
					WazaSeqAdrs=(u8 *)&WAZA_Nemuru;
					//2�̂ɂ�����Z���o���Ă���Ƃ���
					//DOUBLECHECK�������Ȃ����߂̏���
					ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
					ret=1;
				}
			}
			else{
				PSP[AttackClient].condition2&=CONDITION2_AKUMU_OFF;
				PushAdrsAct();
				ServerWork[5]=0;
				WazaSeqAdrs=(u8 *)&WAZA_Mezame;
				ret=1;
			}
		}
	}
	if(PSP[AttackClient].condition&CONDITION_KOORI){		//������`�F�b�N
		if((pp_rand()%10!=0)&&(WTD[WazaNo].battleeffect!=125)){
			WazaSeqAdrs=(u8 *)&WAZA_Koori;
			ServerStatusFlag|=SERVER_STATUS_FLAG_DOUBLE_CHECK;	//2�̂ɂ�����Z���o���Ă���Ƃ���
																//DOUBLECHECK�������Ȃ����߂̏���
		}
		else{
			PSP[AttackClient].condition&=CONDITION_KOORI_OFF;
			PushAdrsAct();
			WazaSeqAdrs=(u8 *)&WAZA_Melt;
			if(WTD[WazaNo].battleeffect!=125)
				ServerWork[5]=0;
			else
				ServerWork[5]=1;
		}
		ret=1;
	}

	if(ret){
		ClientNo=AttackClient;
		BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 BSS_PARA_CONDITION,
							 BSS_SELMONSNO,
							 4,
							 (u8 *)&PSP[ClientNo].condition);
		ClientBitSet(ClientNo);
		return ret;
	}

	if((WKC[AttackClient].kanashibariwazano==WazaNo)&&
	   (WazaNo!=0)){	//���Ȃ��΂�`�F�b�N
		OTF[AttackClient].kanashibariflag=1;
		UserWork[CLIENT_NO_WORK]=AttackClient;
		ServerKeepOff(AttackClient);
		WazaSeqAdrs=(u8 *)&WAZA_Kanashibari;
		//2�̂ɂ�����Z���o���Ă���Ƃ���
		//DOUBLECHECK�������Ȃ����߂̏���
		ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
		ret=1;
	}
	if((WKC[AttackClient].chouhatsucnt)&&
	   ((WTD[WazaNo].damage)==0)){						//���傤�͂`�F�b�N
		OTF[AttackClient].chouhatsuflag=1;
		ServerKeepOff(AttackClient);
		WazaSeqAdrs=(u8 *)&WAZA_Chouhatsu2;
		//2�̂ɂ�����Z���o���Ă���Ƃ���
		//DOUBLECHECK�������Ȃ����߂̏���
		ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
		ret=1;
	}
	if((WazaNo==WazaNoUse[AttackClient])&&
	   (PSP[AttackClient].condition2&CONDITION2_ICHAMON)){ //���������`�F�b�N
		OTF[AttackClient].ichamonflag=1;
		ServerKeepOff(AttackClient);
		WazaSeqAdrs=(u8 *)&WAZA_Ichamon2;
		//2�̂ɂ�����Z���o���Ă���Ƃ���
		//DOUBLECHECK�������Ȃ����߂̏���
		ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
		ret=1;
	}
	if(HuuinCheck(AttackClient,WazaNo)){					//�ӂ�����`�F�b�N
		OTF[AttackClient].huuinflag=1;
		ServerKeepOff(AttackClient);
		WazaSeqAdrs=(u8 *)&WAZA_Huuin;
		//2�̂ɂ�����Z���o���Ă���Ƃ���
		//DOUBLECHECK�������Ȃ����߂̏���
		ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
		ret=1;
	}
	if(PSP[AttackClient].condition&CONDITION_MAHI){			//�܂Ѓ`�F�b�N
		if(pp_rand()%4==0){
			OTF[AttackClient].mahiflag=1;
//			PSP[AttackClient].condition2&=CONDITION2_KEEP_OFF;
			ServerKeepOff(AttackClient);
			WazaSeqAdrs=(u8 *)&WAZA_Mahi;
			//2�̂ɂ�����Z���o���Ă���Ƃ���
			//DOUBLECHECK�������Ȃ����߂̏���
			ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
			ret=1;
		}
	}

	if(PSP[AttackClient].condition2&CONDITION2_GAMAN){		//���܂�`�F�b�N
		PSP[AttackClient].condition2-=GAMAN_COUNT;
		if(PSP[AttackClient].condition2&CONDITION2_GAMAN)
			WazaSeqAdrs=(u8 *)&WAZA_Gaman;
		else{
			PSP[AttackClient].condition2&=CONDITION2_KEEP_OFF;
			if(StoreDamage[AttackClient]){
				WazaNo=WAZANO_GAMAN;
				gamandamage[0]=StoreDamage[AttackClient]*2;
				DefenceClient=ClientNoHit[AttackClient];
				WazaSeqAdrs=(u8 *)&WAZA_GamanOff;
			}
			else
				WazaSeqAdrs=(u8 *)&WAZA_GamanFaild;
		}
		return 1;
	}

	if(PSP[AttackClient].condition2&CONDITION2_MEROMERO){	//���������`�F�b�N
		UserWork[CLIENT_NO_WORK]=Bit2No((PSP[AttackClient].condition2&CONDITION2_MEROMERO)>>MEROMERO_SHIFT);
		if(pp_rand()&1)
			PushAdrsSet(WazaSeqAdrs++);
		else{
			PushAdrsSet((u8 *)&WAZA_MeromeroNext);
			//2�̂ɂ�����Z���o���Ă���Ƃ���
			//DOUBLECHECK�������Ȃ����߂̏���
			ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
			OTF[AttackClient].meromeroflag=1;
			ServerKeepOff(AttackClient);
		}
		WazaSeqAdrs=(u8 *)&WAZA_Meromero;
		return 1;
	}

	if(PSP[AttackClient].condition2&CONDITION2_KONRAN){		//������`�F�b�N
		PSP[AttackClient].condition2-=KONRAN_COUNT;
		if(PSP[AttackClient].condition2&CONDITION2_KONRAN){
			if(pp_rand()&1){
				ServerWork[5]=0;
				PushAdrsAct();
			}
			else{
				ServerWork[5]=1;
				DefenceClient=AttackClient;
				WazaDamage=WazaDamageCalc(
								&PSP[AttackClient],
								&PSP[AttackClient],
								WAZANO_HATAKU,
								0,
								40,
								0,
								AttackClient,
								AttackClient);
				//�A���n�A���ߌn�͉���
				OTF[AttackClient].konranflag=1;
//				PSP[AttackClient].condition2&=CONDITION2_KEEP_OFF;
				ServerKeepOff(AttackClient);
			}
			WazaSeqAdrs=(u8 *)&WAZA_Konran;
			//2�̂ɂ�����Z���o���Ă���Ƃ���
			//DOUBLECHECK�������Ȃ����߂̏���
			ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
		}
		else{
			PushAdrsAct();
			WazaSeqAdrs=(u8 *)&WAZA_KonranEnd;
		}
		return 1;
	}

	return	ret;
}
#endif

//==============================================================
//		�|�P��������ւ��\���ǂ����̃`�F�b�N
//==============================================================

u8	PokeReshuffleCheck(u8 clientno,u8 selmons1,u8 selmons2)
{
	int i;
	u8	no1,no2;
	PokemonParam	*PP;

	if((FightType&FIGHT_TYPE_2vs2)==0) return 0;
	if(FightType&FIGHT_TYPE_4_2vs2){
		no1=GetTeamPosID(clientno);	//���̃N���C�A���g��ID
//		if((no1==0)||(no1==2))
		if(MineEnemyCheck(clientno)==SIDE_MINE)
			PP=&PokeParaMine[0];
		else
			PP=&PokeParaEnemy[0];
		no2=GetTeamPos2(no1);		//�O�q����q��?
		for(i=0+3*no2;i<3+3*no2;i++){
			if((PokeParaGet(&PP[i],ID_hp)!=0)&&
			   (PokeParaGet(&PP[i],ID_monsno_egg)!=0)&&
			   (PokeParaGet(&PP[i],ID_monsno_egg)!=MONSNO_TAMAGO))
				break;
		}
		return(i==3+3*no2);
	}
	else{
		if(MineEnemyCheck(clientno)==SIDE_ENEMY){	//Enemy
			no1=ClientNoGet(BSS_CLIENT_ENEMY);
			no2=ClientNoGet(BSS_CLIENT_ENEMY2);
			PP=&PokeParaEnemy[0];
		}		
		else{								//Mine
			no1=ClientNoGet(BSS_CLIENT_MINE);
			no2=ClientNoGet(BSS_CLIENT_MINE2);
			PP=&PokeParaMine[0];
		}
		if(selmons1==6)
			selmons1=SelMonsNo[no1];
		if(selmons2==6)
			selmons2=SelMonsNo[no2];
		for(i=0;i<6;i++){
			if((PokeParaGet(&PP[i],ID_hp)!=0)&&
			   (PokeParaGet(&PP[i],ID_monsno_egg)!=0)&&
			   (PokeParaGet(&PP[i],ID_monsno_egg)!=MONSNO_TAMAGO)&&
			   (i!=selmons1)&&
			   (i!=selmons2)&&
			   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no1])&&
			   (i!=UserWork[AFTER_RESHUFFLE_SELMONSNO+no2]))
				break;
		}
		return(i==6);
	}
}

//==============================================================
//		����\�͂̃`�F�b�N
//==============================================================

const u16 BouonWazaTbl[]={
	WAZANO_NAKIGOE,		//�Ȃ�����
	WAZANO_HOERU,		//�ق���
	WAZANO_UTAU,		//������
	WAZANO_TYOUONPA,	//���傤�����
	WAZANO_IYANAOTO,	//����Ȃ���
	WAZANO_IBIKI,		//���т�
	WAZANO_SAWAGU,		//���킮
	WAZANO_KINZOKUON,	//���񂼂�����
	WAZANO_KUSABUE,		//�����Ԃ�
	WAZANO_HAIPAABOISU,	//�n�C�p�[�{�C�X
	0xffff,
};

u8	SpeabiTenkiyaAct(u8 clientno)
{
	u8	ret=0;

	//�|�����ȊO�͂��̓����������Ă��Ă��A�ω��Ȃ�
	if((PSP[clientno].monsno!=MONSNO_POWARUN)||
	   (PSP[clientno].speabino!=TOKUSYU_TENKIYA)||
	   (PSP[clientno].hp==0))
		return 0;

	//�Ă񂫂�ŁA�ω�������Ƀm�[�Ă񂫂������ꍇ�͂��Ƃɖ߂�
	if(((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,
							 0,
							 TOKUSYU_NOOTENKI,0,0))||
	    (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,
							 0,
							 TOKUSYU_EAROKKU,0,0)))&&
		(PSP[clientno].type1!=NORMAL_TYPE)&&
		(PSP[clientno].type2!=NORMAL_TYPE)){
		PSP[clientno].type1=NORMAL_TYPE;
		PSP[clientno].type2=NORMAL_TYPE;
		return 1;
	}
	//�����m�[�Ă񂫂́A�ω�����
	if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,
							0,
							TOKUSYU_NOOTENKI,0,0))||
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,
							0,
							TOKUSYU_EAROKKU,0,0)))
		return 0;

	if(((FieldCondition&FIELD_CONDITION_TENKI_CHG)==0)&&
		(PSP[clientno].type1!=NORMAL_TYPE)&&
		(PSP[clientno].type2!=NORMAL_TYPE)){
		PSP[clientno].type1=NORMAL_TYPE;
		PSP[clientno].type2=NORMAL_TYPE;
		ret=1;
	}
	if((FieldCondition&FIELD_CONDITION_HIDERING)&&
	   (PSP[clientno].type1!=FIRE_TYPE)&&
	   (PSP[clientno].type2!=FIRE_TYPE)){
		PSP[clientno].type1=FIRE_TYPE;
		PSP[clientno].type2=FIRE_TYPE;
		ret=2;
	}
	if((FieldCondition&FIELD_CONDITION_AMEHURI)&&
	   (PSP[clientno].type1!=WATER_TYPE)&&
	   (PSP[clientno].type2!=WATER_TYPE)){
		PSP[clientno].type1=WATER_TYPE;
		PSP[clientno].type2=WATER_TYPE;
		ret=3;
	}
	if((FieldCondition&FIELD_CONDITION_ARARE)&&
	   (PSP[clientno].type1!=KOORI_TYPE)&&
	   (PSP[clientno].type2!=KOORI_TYPE)){
		PSP[clientno].type1=KOORI_TYPE;
		PSP[clientno].type2=KOORI_TYPE;
		ret=4;
	}
	return ret;
}

u8	SpecialAbirityCheck(u8 seqno,u8 clientno,u8 CheckSpeabiNo,u8 speabino,u16 spewazano)
{
	u8	dir;
	u8	ret=0;
	int	i;
	u16	wazano;
	u8	no1=0,no2=0;
	u8	wazatype;
	PokemonParam	*PPA,*PPD;
	u16	MonsNoA;
	u16	MonsNoD;
	u32	P_RndA;
	u32	P_RndD;

	if(AttackClient>=ClientSetMax)
		AttackClient=clientno;

	if(MineEnemyCheck(AttackClient)==SIDE_MINE)
		PPA=&PokeParaMine[SelMonsNo[AttackClient]];
	else
		PPA=&PokeParaEnemy[SelMonsNo[AttackClient]];

	if(DefenceClient>=ClientSetMax)
		DefenceClient=clientno;

	if(MineEnemyCheck(DefenceClient)==SIDE_MINE)
		PPD=&PokeParaMine[SelMonsNo[DefenceClient]];
	else
		PPD=&PokeParaEnemy[SelMonsNo[DefenceClient]];

	MonsNoA=PokeParaGet(PPA,ID_monsno);
	P_RndA=PokeParaGet(PPA,ID_personal_rnd);
	MonsNoD=PokeParaGet(PPD,ID_monsno);
	P_RndD=PokeParaGet(PPD,ID_personal_rnd);

	//�T�t�@���]�[���ł́A�����͖���
	if(FightType&FIGHT_TYPE_SAFARI)
		return ret;

	if(speabino)
		SpeabiNo=speabino;
	else
		SpeabiNo=PSP[clientno].speabino;

	if(spewazano)
		wazano=spewazano;
	else
		wazano=WazaNo;

	if(UserWork[DAMAGE_TYPE_WORK])
		wazatype=UserWork[DAMAGE_TYPE_WORK]&0x3f;
	else
		wazatype=WTD[wazano].wazatype;

	switch(seqno){
		case SPEABI_POKEAPPEAR:
			if(AttackClient>=ClientSetMax)
				AttackClient=clientno;
			switch(SpeabiNo){
				case TOKUSYU_TENKOU:		//�V���퓬�ɔ��f������i����\�͂ł͂Ȃ��j
					switch(GetWeatherNow()){
						case WEATHER_RAIN: 
						case WEATHER_THUNDER: 
						case WEATHER_HEAVYRAIN:
							if((FieldCondition&FIELD_CONDITION_AMEHURI)==0){
								FieldCondition=(FIELD_CONDITION_AMEHURASHI|FIELD_CONDITION_AME);
								UserWork[FIGHT_EFFECT_WORK]=FE_ID_AMAGOI;
								UserWork[CLIENT_NO_WORK]=clientno;
								ret++;
							}
							break;
						case WEATHER_STORM:
							if((FieldCondition&FIELD_CONDITION_SUNAARASHIING)==0){
								FieldCondition=FIELD_CONDITION_SUNAARASHIING;
								UserWork[FIGHT_EFFECT_WORK]=FE_ID_SUNAARASI;
								UserWork[CLIENT_NO_WORK]=clientno;
								ret++;
							}
							break;
						case WEATHER_SHINE3:
							if((FieldCondition&FIELD_CONDITION_HIDERING)==0){
								FieldCondition=FIELD_CONDITION_HIDERING;
								UserWork[FIGHT_EFFECT_WORK]=FE_ID_NIHONBARE;
								UserWork[CLIENT_NO_WORK]=clientno;
								ret++;
							}
							break;
					}
					if(ret){
						ServerWork[5]=GetWeatherNow();
						ServerSpeabiSubroutineSet((u8 *)&WAZA_FieldWeather);
					}
					break;
				case TOKUSYU_AMEHURASI:
					if((FieldCondition&FIELD_CONDITION_AMEHURASHI)==0){
						FieldCondition=(FIELD_CONDITION_AME|FIELD_CONDITION_AMEHURASHI);
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Amehurashi);
						UserWork[CLIENT_NO_WORK]=clientno;
						ret++;
					}
					break;
				case TOKUSYU_SUNAOKOSI:
					if((FieldCondition&FIELD_CONDITION_SUNAARASHIW)==0){
						FieldCondition=FIELD_CONDITION_SUNAARASHIING;
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Sunaokosi);
						UserWork[CLIENT_NO_WORK]=clientno;
						ret++;
					}
					break;
				case TOKUSYU_HIDERI:
					if((FieldCondition&FIELD_CONDITION_HIDERI)==0){
						FieldCondition=FIELD_CONDITION_HIDERING;
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Hideri);
						UserWork[CLIENT_NO_WORK]=clientno;
						ret++;
					}
					break;
				case TOKUSYU_IKAKU:
//					ServerSpeabiSubroutineSet((u8 *)&WAZA_Ikaku);
//					UserWork[CLIENT_NO_WORK]=clientno;
//					ret++;
					if(OSTF[clientno].ikakuflag==0){
						WazaKouka[clientno]|=WAZAKOUKA_IKAKU;
						OSTF[clientno].ikakuflag=1;
					}
					break;
				case TOKUSYU_TENKIYA:
					ret=SpeabiTenkiyaAct(clientno);
					if(ret){
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Tenkiya);
						UserWork[CLIENT_NO_WORK]=clientno;
						UserWork[HENGE_NO]=ret-1;
					}
					break;
				case TOKUSYU_TOREESU:
					if(OSTF[clientno].traceflag==0){
						WazaKouka[clientno]|=WAZAKOUKA_TRACE;
						OSTF[clientno].traceflag=1;
					}
					break;
#if 0
					if(OTF[clientno].traceflag==0){
						dir=(ClientTypeGet(clientno)^1)&1;
						no1=ClientNoGet(dir);
						no2=ClientNoGet(dir+2);
						if(FightType&FIGHT_TYPE_2vs2){
							if(((PSP[no1].speabino)&&(PSP[no1].hp))&&
							   ((PSP[no2].speabino)&&(PSP[no2].hp))){
								ClientNo=ClientNoGet(dir+(pp_rand()%2)*2);
								PSP[clientno].speabino=PSP[ClientNo].speabino;
								SpeabiNo=PSP[ClientNo].speabino;
								ret++;
							}
							else if((PSP[no1].speabino)&&(PSP[no1].hp)){
								ClientNo=no1;
								PSP[clientno].speabino=PSP[ClientNo].speabino;
								SpeabiNo=PSP[ClientNo].speabino;
								ret++;
							}
							else if((PSP[no2].speabino)&&(PSP[no2].hp)){
								ClientNo=no2;
								PSP[clientno].speabino=PSP[ClientNo].speabino;
								SpeabiNo=PSP[ClientNo].speabino;
								ret++;
							}
						}
						else{
							ClientNo=no1;
							if((PSP[no1].speabino)&&(PSP[no1].hp)){
								PSP[clientno].speabino=PSP[no1].speabino;
								SpeabiNo=PSP[no1].speabino;
								ret++;
							}
						}
						if(ret){
							OTF[clientno].traceflag=1;
							ServerSpeabiSubroutineSet((u8 *)&WAZA_Trace);
							UserWork[CLIENT_NO_WORK]=clientno;
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_NICKNAME;
							EvStrBuf0[2]=ClientNo;
							EvStrBuf0[3]=SelMonsNo[ClientNo];
							EvStrBuf0[4]=EOM_;
						}
					}
					break;
#endif
				case TOKUSYU_NOOTENKI:
				case TOKUSYU_EAROKKU:
					for(no1=0;no1<ClientSetMax;no1++){
						ret=SpeabiTenkiyaAct(no1);
						if(ret){
							ServerSpeabiSubroutineSet((u8 *)&WAZA_Tenkiya);
							UserWork[CLIENT_NO_WORK]=no1;
							UserWork[HENGE_NO]=ret-1;
							break;
						}
					}
					break;
			}
			break;
		case SPEABI_TURN_CHECK:
//�񕜌n�̓���\�͂͐�ɔ��肷��悤�ɂ���
			if(PSP[clientno].hp==0)
				break;
			AttackClient=clientno;
			switch(SpeabiNo){
				case TOKUSYU_AMEUKEZARA:
					//�����m�[�Ă񂫂ł́A�񕜂����Ȃ�
					if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,
											0,
											TOKUSYU_NOOTENKI,0,0)==0)&&
					   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,
											0,
											TOKUSYU_EAROKKU,0,0)==0)){
						if(FieldCondition&FIELD_CONDITION_AMEHURI){
							if(PSP[clientno].hpmax>PSP[clientno].hp){
								SpeabiNo=TOKUSYU_AMEUKEZARA;
								ServerSpeabiSubroutineSet((u8 *)&WAZA_Ameukezara);
								WazaDamage=PSP[clientno].hpmax/16;
								if(WazaDamage==0)
									WazaDamage=1;
								WazaDamage*=-1;
								ret++;
							}
						}
					}
					break;
//GetExpCheck�̕��Ń`�F�b�N���Ă���̂ŁA�������͗v��Ȃ��͂�
#if 0
				case TOKUSYU_TENKIYA:
					ret=SpeabiTenkiyaAct(clientno);
					if(ret){
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Tenkiya);
						UserWork[CLIENT_NO_WORK]=clientno;
						UserWork[HENGE_NO]=ret-1;
					}
					break;
#endif
				case TOKUSYU_DAPPI:
					if((PSP[clientno].condition&CONDITION_BAD)&&
					   (pp_rand()%3==0)){
						if(PSP[clientno].condition&(CONDITION_DOKU|CONDITION_DOKUDOKU))
							PM_strcpy(EvStrBuf0,DokuMsg);
						if(PSP[clientno].condition&CONDITION_NEMURI)
							PM_strcpy(EvStrBuf0,NemuriMsg);
						if(PSP[clientno].condition&CONDITION_MAHI)
							PM_strcpy(EvStrBuf0,MahiMsg);
						if(PSP[clientno].condition&CONDITION_YAKEDO)
							PM_strcpy(EvStrBuf0,YakedoMsg);
						if(PSP[clientno].condition&CONDITION_KOORI)
							PM_strcpy(EvStrBuf0,KooriMsg);
						PSP[clientno].condition=0;
						UserWork[CLIENT_NO_WORK]=ClientNo=clientno;
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Dappi);
						BSS_POKEPARA_PUT_SET(BSS_SERVER,
											 BSS_PARA_CONDITION,
											 BSS_SELMONSNO,
											 4,
											 (u8 *)&PSP[clientno].condition);
						ClientBitSet(ClientNo);
						ret++;
					}
					break;
				case TOKUSYU_KASOKU:
					if((PSP[clientno].abiritycnt[COND_AGI]<12)&&
					   (WKC[clientno].nekodamashicnt!=2)){
						PSP[clientno].abiritycnt[COND_AGI]++;
						UserWork[FIGHT_EFFECT_WORK+0]=COND2_AGIUP;
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Kasokusouti);
						UserWork[CLIENT_NO_WORK]=clientno;
						ret++;
					}
					break;
				case TOKUSYU_NAMAKE:
					WKC[AttackClient].namakebit^=1;
					break;
			}
			break;
		case SPEABI_WAZA_HIT_BEFORE:
			switch(SpeabiNo){
				case TOKUSYU_BOUON:
					i=0;
					while(BouonWazaTbl[i]!=0xffff){
						if(BouonWazaTbl[i]==wazano)
							break;
						i++;
					}
					if(BouonWazaTbl[i]!=0xffff){
						if(PSP[AttackClient].condition2&CONDITION2_KEEP)
							ServerStatusFlag|=SERVER_STATUS_FLAG_NOPP_DEC;
						WazaSeqAdrs=(u8 *)&WAZA_Bouon;
						ret=1;
					}
					break;
			}
			break;
		case SPEABI_WAZA_HIT:
			if(wazano){
				switch(SpeabiNo){
					case TOKUSYU_TIKUDEN:
						if((wazatype==ELECTRIC_TYPE)&&
						   (WTD[wazano].damage!=0)){
							if(OTF[AttackClient].ppdecflag)
								WazaSeqAdrs=(u8 *)&WAZA_Tikuden;
							else
								WazaSeqAdrs=(u8 *)&WAZA_TikudenPPDec;
							ret=1;
						}
						break;
					case TOKUSYU_TYOSUI:
						if((wazatype==WATER_TYPE)&&
						   (WTD[wazano].damage!=0)){
							if(OTF[AttackClient].ppdecflag)
								WazaSeqAdrs=(u8 *)&WAZA_Tyosui;
							else
								WazaSeqAdrs=(u8 *)&WAZA_TyosuiPPDec;
							ret=1;
						}
						break;
					case TOKUSYU_MORAIBI:
						if((wazatype==FIRE_TYPE)&&
						  ((PSP[clientno].condition&CONDITION_KOORI)==0)){
							if((SAD_WORK->abirityflag[clientno]&SAD_MORAIBI)==0){
								ServerWork[5]=0;
								if(OTF[AttackClient].ppdecflag)
									WazaSeqAdrs=(u8 *)&WAZA_Moraibi;
								else
									WazaSeqAdrs=(u8 *)&WAZA_MoraibiPPDec;
							  	SAD_WORK->abirityflag[clientno]|=SAD_MORAIBI;
								ret=2;
							}
							else{
								ServerWork[5]=1;
								if(OTF[AttackClient].ppdecflag)
									WazaSeqAdrs=(u8 *)&WAZA_Moraibi;
								else
									WazaSeqAdrs=(u8 *)&WAZA_MoraibiPPDec;
								ret=2;
							}
						}
						break;
				}
				if(ret==1){
					if(PSP[clientno].hpmax==PSP[clientno].hp){
						if(OTF[AttackClient].ppdecflag)
							WazaSeqAdrs=(u8 *)&WAZA_NOKaifuku;
						else
							WazaSeqAdrs=(u8 *)&WAZA_NOKaifukuPPDec;
					}
					else{
						WazaDamage=PSP[clientno].hpmax/4;
						if(WazaDamage==0)
							WazaDamage=1;
						WazaDamage*=-1;
					}
				}
			}
			break;
		case SPEABI_WAZA_HIT_AFTER:
//			if(ServerStatusFlag&SERVER_STATUS_FLAG_NO_ZOKUSEI){
//				ServerStatusFlag&=SERVER_STATUS_FLAG_NO_ZOKUSEI_OFF;
//				return 0;
//			}
			switch(SpeabiNo){
				case TOKUSYU_HENSYOKU:
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						(wazano!=WAZANO_WARUAGAKI)&&
						(WTD[wazano].damage!=0)&&
					   ((OSTF[DefenceClient].butsurihitdamage)||
					    (OSTF[DefenceClient].tokusyuhitdamage))&&
						(PSP[clientno].type1!=wazatype)&&
						(PSP[clientno].type2!=wazatype)&&
//					   ((ServerStatusFlag&SERVER_STATUS_FLAG_HENSYOKU_NOHIT)==0)&&
						(PSP[clientno].hp!=0)){
						PSP[clientno].type1=wazatype;
						PSP[clientno].type2=wazatype;
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_ZOKUSEI;
						EvStrBuf0[2]=wazatype;
						EvStrBuf0[3]=EOM_;

						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Hensyoku;
						ret++;
					}
					break;
				case TOKUSYU_SAMEHADA:
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					   (PSP[AttackClient].hp)&&
					   (OTF[AttackClient].konranflag==0)&&
					  ((OSTF[DefenceClient].butsurihitdamage)||
					   (OSTF[DefenceClient].tokusyuhitdamage))&&
					   (WTD[wazano].flag&FLAG_DAGEKI)){
						WazaDamage=PSP[AttackClient].hpmax/16;
						if(WazaDamage==0)
							WazaDamage=1;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Samehada;
						ret++;
					}
					break;
				case TOKUSYU_HOUSI:
#ifdef PM_DEBUG
					if(DebugFightFlag&DEBUG_FIGHT_WAZA_DEBUG){
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (WTD[wazano].flag&FLAG_DAGEKI)){
							do{
								ServerWork[3]=pp_rand()&3;
							}while(ServerWork[3]==0);
							if(ServerWork[3]==3)
								ServerWork[3]+=2;
							ServerWork[3]+=TSUIKA_ATTACK;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Housi;
							ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
							ret++;
						}
					}
					else{
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						    (pp_rand()%10==0)){
							do{
								ServerWork[3]=pp_rand()&3;
							}while(ServerWork[3]==0);
							if(ServerWork[3]==3)
								ServerWork[3]+=2;
							ServerWork[3]+=TSUIKA_ATTACK;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Housi;
							ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
							ret++;
						}
					}
#else
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					    (PSP[AttackClient].hp)&&
					    (OTF[AttackClient].konranflag==0)&&
					   ((OSTF[DefenceClient].butsurihitdamage)||
					    (OSTF[DefenceClient].tokusyuhitdamage))&&
					    (WTD[wazano].flag&FLAG_DAGEKI)&&
					    (pp_rand()%10==0)){
						do{
							ServerWork[3]=pp_rand()&3;
						}while(ServerWork[3]==0);
						if(ServerWork[3]==3)
							ServerWork[3]+=2;
						ServerWork[3]+=TSUIKA_ATTACK;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Housi;
						ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
						ret++;
					}
#endif
					break;
				case TOKUSYU_DOKUNOTOGE:
#ifdef PM_DEBUG
					if(DebugFightFlag&DEBUG_FIGHT_WAZA_DEBUG){
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (WTD[wazano].flag&FLAG_DAGEKI)){
							ServerWork[3]=COND_DOKU+TSUIKA_ATTACK;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Dokunotoge;
							ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
							ret++;
						}
					}
					else{
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						    (pp_rand()%3==0)){
							ServerWork[3]=COND_DOKU+TSUIKA_ATTACK;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Dokunotoge;
							ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
							ret++;
						}
					}
#else
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					    (PSP[AttackClient].hp)&&
					    (OTF[AttackClient].konranflag==0)&&
					   ((OSTF[DefenceClient].butsurihitdamage)||
					    (OSTF[DefenceClient].tokusyuhitdamage))&&
					    (WTD[wazano].flag&FLAG_DAGEKI)&&
					    (pp_rand()%3==0)){
						ServerWork[3]=COND_DOKU+TSUIKA_ATTACK;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Dokunotoge;
						ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
						ret++;
					}
#endif
					break;
				case TOKUSYU_SEIDENKI:
#ifdef PM_DEBUG
					if(DebugFightFlag&DEBUG_FIGHT_WAZA_DEBUG){
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (WTD[wazano].flag&FLAG_DAGEKI)){
							ServerWork[3]=COND_MAHI+TSUIKA_ATTACK;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Seidenki;
							ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
							ret++;
						}
					}
					else{
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						    (pp_rand()%3==0)){
							ServerWork[3]=COND_MAHI+TSUIKA_ATTACK;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Seidenki;
							ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
							ret++;
						}
					}
#else
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					    (PSP[AttackClient].hp)&&
					    (OTF[AttackClient].konranflag==0)&&
					   ((OSTF[DefenceClient].butsurihitdamage)||
					    (OSTF[DefenceClient].tokusyuhitdamage))&&
					    (WTD[wazano].flag&FLAG_DAGEKI)&&
					    (pp_rand()%3==0)){

						ServerWork[3]=COND_MAHI+TSUIKA_ATTACK;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Seidenki;
						ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
						ret++;
					}
#endif
					break;
				case TOKUSYU_HONOONOKARADA:
#ifdef PM_DEBUG
					if(DebugFightFlag&DEBUG_FIGHT_WAZA_DEBUG){
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))){
							ServerWork[3]=COND_YAKEDO+TSUIKA_ATTACK;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Honoonokarada;
							ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
							ret++;
						}
					}
					else{
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (pp_rand()%3==0)){
							ServerWork[3]=COND_YAKEDO+TSUIKA_ATTACK;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Honoonokarada;
							ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
							ret++;
						}
					}
#else
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					    (PSP[AttackClient].hp)&&
					    (OTF[AttackClient].konranflag==0)&&
					    (WTD[wazano].flag&FLAG_DAGEKI)&&
					   ((OSTF[DefenceClient].butsurihitdamage)||
					    (OSTF[DefenceClient].tokusyuhitdamage))&&
					    (pp_rand()%3==0)){
						ServerWork[3]=COND_YAKEDO+TSUIKA_ATTACK;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Honoonokarada;
						ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
						ret++;
					}
#endif
					break;
				case TOKUSYU_MEROMEROBODHI:
#ifdef PM_DEBUG
					if(DebugFightFlag&DEBUG_FIGHT_WAZA_DEBUG){
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (PSP[DefenceClient].hp)){
							//����\�͂ǂ񂩂�́A���������ɂȂ�Ȃ�
							if(PSP[AttackClient].speabino!=TOKUSYU_DONKAN){
								if(((PokeSexGetMonsNo(MonsNoA,P_RndA))!=
									(PokeSexGetMonsNo(MonsNoD,P_RndD)))&&
							   	   ((PSP[AttackClient].condition2&CONDITION2_MEROMERO)==0)&&
									(PokeSexGetMonsNo(MonsNoA,P_RndA)!=MONS_UNKNOWN)&&
									(PokeSexGetMonsNo(MonsNoD,P_RndD)!=MONS_UNKNOWN)){
									PSP[AttackClient].condition2|=(no2bittbl[DefenceClient]<<MEROMERO_SHIFT);
									PushAdrsAct();
									WazaSeqAdrs=(u8 *)&WAZA_MeromeroBody;
									ret++;
								}
							}
						}
					}
					else{
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (PSP[DefenceClient].hp)&&
						    (pp_rand()%3==0)){
							//����\�͂ǂ񂩂�́A���������ɂȂ�Ȃ�
							if(PSP[AttackClient].speabino!=TOKUSYU_DONKAN){
								if(((PokeSexGetMonsNo(MonsNoA,P_RndA))!=
									(PokeSexGetMonsNo(MonsNoD,P_RndD)))&&
							   	   ((PSP[AttackClient].condition2&CONDITION2_MEROMERO)==0)&&
									(PokeSexGetMonsNo(MonsNoA,P_RndA)!=MONS_UNKNOWN)&&
									(PokeSexGetMonsNo(MonsNoD,P_RndD)!=MONS_UNKNOWN)){
									PSP[AttackClient].condition2|=(no2bittbl[DefenceClient]<<MEROMERO_SHIFT);
									PushAdrsAct();
									WazaSeqAdrs=(u8 *)&WAZA_MeromeroBody;
									ret++;
								}
							}
						}
					}
#else
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					    (PSP[AttackClient].hp)&&
					    (OTF[AttackClient].konranflag==0)&&
					    (WTD[wazano].flag&FLAG_DAGEKI)&&
					   ((OSTF[DefenceClient].butsurihitdamage)||
					    (OSTF[DefenceClient].tokusyuhitdamage))&&
					    (PSP[DefenceClient].hp)&&
					    (pp_rand()%3==0)){
						//����\�͂ǂ񂩂�́A���������ɂȂ�Ȃ�
						if(PSP[AttackClient].speabino!=TOKUSYU_DONKAN){
							if(((PokeSexGetMonsNo(MonsNoA,P_RndA))!=
								(PokeSexGetMonsNo(MonsNoD,P_RndD)))&&
						   	   ((PSP[AttackClient].condition2&CONDITION2_MEROMERO)==0)&&
								(PokeSexGetMonsNo(MonsNoA,P_RndA)!=MONS_UNKNOWN)&&
								(PokeSexGetMonsNo(MonsNoD,P_RndD)!=MONS_UNKNOWN)){
								PSP[AttackClient].condition2|=(no2bittbl[DefenceClient]<<MEROMERO_SHIFT);
								PushAdrsAct();
								WazaSeqAdrs=(u8 *)&WAZA_MeromeroBody;
								ret++;
							}
						}
					}
#endif
					break;
			}
			break;
#if 0
				case TOKUSYU_MEROMEROBODHI:
#ifdef PM_DEBUG
					if(DebugFightFlag&DEBUG_FIGHT_WAZA_DEBUG){
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (PSP[DefenceClient].hp)){
							//����\�͂ǂ񂩂�́A���������ɂȂ�Ȃ�
							if(PSP[AttackClient].speabino!=TOKUSYU_DONKAN){
								if(((PokeSexGetMonsNo
									(PSP[AttackClient].monsno,PSP[AttackClient].personal_rnd))!=
									(PokeSexGetMonsNo(
									PSP[DefenceClient].monsno,PSP[DefenceClient].personal_rnd)))&&
							   	  ((PSP[AttackClient].condition2&CONDITION2_MEROMERO)==0)&&
									(PokeSexGetMonsNo(
									PSP[DefenceClient].monsno,PSP[DefenceClient].personal_rnd)!=MONS_UNKNOWN)&&
									(PokeSexGetMonsNo(
									PSP[AttackClient].monsno,PSP[AttackClient].personal_rnd)!=MONS_UNKNOWN)
									){
									PSP[AttackClient].condition2|=(no2bittbl[DefenceClient]<<MEROMERO_SHIFT);
									PushAdrsAct();
									WazaSeqAdrs=(u8 *)&WAZA_MeromeroBody;
									ret++;
								}
							}
						}
					}
					else{
						if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
						    (PSP[AttackClient].hp)&&
						    (OTF[AttackClient].konranflag==0)&&
						    (WTD[wazano].flag&FLAG_DAGEKI)&&
						   ((OSTF[DefenceClient].butsurihitdamage)||
						    (OSTF[DefenceClient].tokusyuhitdamage))&&
						    (PSP[DefenceClient].hp)&&
						    (pp_rand()%3==0)){
							//����\�͂ǂ񂩂�́A���������ɂȂ�Ȃ�
							if(PSP[AttackClient].speabino!=TOKUSYU_DONKAN){
								if(((PokeSexGetMonsNo
									(PSP[AttackClient].monsno,PSP[AttackClient].personal_rnd))!=
									(PokeSexGetMonsNo(
									PSP[DefenceClient].monsno,PSP[DefenceClient].personal_rnd)))&&
							   	  ((PSP[AttackClient].condition2&CONDITION2_MEROMERO)==0)&&
									(PokeSexGetMonsNo(
									PSP[DefenceClient].monsno,PSP[DefenceClient].personal_rnd)!=MONS_UNKNOWN)&&
									(PokeSexGetMonsNo(
									PSP[AttackClient].monsno,PSP[AttackClient].personal_rnd)!=MONS_UNKNOWN)
									){
									PSP[AttackClient].condition2|=(no2bittbl[DefenceClient]<<MEROMERO_SHIFT);
									PushAdrsAct();
									WazaSeqAdrs=(u8 *)&WAZA_MeromeroBody;
									ret++;
								}
							}
						}
					}
#else
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					    (PSP[AttackClient].hp)&&
					    (OTF[AttackClient].konranflag==0)&&
					    (WTD[wazano].flag&FLAG_DAGEKI)&&
					   ((OSTF[DefenceClient].butsurihitdamage)||
					    (OSTF[DefenceClient].tokusyuhitdamage))&&
					    (PSP[DefenceClient].hp)&&
					    (pp_rand()%3==0)){
						//����\�͂ǂ񂩂�́A���������ɂȂ�Ȃ�
						if(PSP[AttackClient].speabino!=TOKUSYU_DONKAN){
							if(((PokeSexGetMonsNo
								(PSP[AttackClient].monsno,PSP[AttackClient].personal_rnd))!=
								(PokeSexGetMonsNo(
								PSP[DefenceClient].monsno,PSP[DefenceClient].personal_rnd)))&&
						   	  ((PSP[AttackClient].condition2&CONDITION2_MEROMERO)==0)&&
								(PokeSexGetMonsNo(
								PSP[DefenceClient].monsno,PSP[DefenceClient].personal_rnd)!=MONS_UNKNOWN)&&
								(PokeSexGetMonsNo(
								PSP[AttackClient].monsno,PSP[AttackClient].personal_rnd)!=MONS_UNKNOWN)
								){
								PSP[AttackClient].condition2|=(no2bittbl[DefenceClient]<<MEROMERO_SHIFT);
								PushAdrsAct();
								WazaSeqAdrs=(u8 *)&WAZA_MeromeroBody;
								ret++;
							}
						}
					}
#endif
					break;
			}
			break;
#endif
		case SPEABI_WAZA_HIT_AFTER_ALL:
			for(clientno=0;clientno<ClientSetMax;clientno++){
				switch(PSP[clientno].speabino){
					case TOKUSYU_MENEKI:
						if(PSP[clientno].condition&CONDITION_DOKUALL){
							PM_strcpy(EvStrBuf0,DokuMsg);
							ret=1;
						}
						break;
					case TOKUSYU_MAIPEESU:
						if(PSP[clientno].condition2&CONDITION2_KONRAN){
							PM_strcpy(EvStrBuf0,KonranMsg);
							ret=2;
						}
						break;
					case TOKUSYU_ZYUUNAN:
						if(PSP[clientno].condition&CONDITION_MAHI){
							PM_strcpy(EvStrBuf0,MahiMsg);
							ret=1;
						}
						break;
					case TOKUSYU_HUMIN:
					case TOKUSYU_YARUKI:
						if(PSP[clientno].condition&CONDITION_NEMURI){
							PSP[clientno].condition2&=CONDITION2_AKUMU_OFF;
							PM_strcpy(EvStrBuf0,NemuriMsg);
							ret=1;
						}
						break;
					case TOKUSYU_MIZUNOBEERU:
						if(PSP[clientno].condition&CONDITION_YAKEDO){
							PM_strcpy(EvStrBuf0,YakedoMsg);
							ret=1;
						}
						break;
					case TOKUSYU_MAGUMANOYOROI:
						if(PSP[clientno].condition&CONDITION_KOORI){
							PM_strcpy(EvStrBuf0,KooriMsg);
							ret=1;
						}
						break;
					case TOKUSYU_DONKAN:
						if(PSP[clientno].condition2&CONDITION2_MEROMERO){
							PM_strcpy(EvStrBuf0,MeromeroMsg);
							ret=3;
						}
						break;
//GetExpCheck�̕��ŁA�`�F�b�N���Ă���̂ł������͂���Ȃ��͂�
#if 0
					case TOKUSYU_TENKIYA:
						ret=SpeabiTenkiyaAct(clientno);
						if(ret){
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Tenkiya2;
							UserWork[CLIENT_NO_WORK]=clientno;
							UserWork[HENGE_NO]=ret-1;
							return ret;
						}
						break;
#endif
				}
				if(ret){
					switch(ret){
						case 1:
							PSP[clientno].condition=0;
							break;
						case 2: 
							PSP[clientno].condition2&=CONDITION2_KONRAN_OFF;
							break;
						case 3:
							PSP[clientno].condition2&=CONDITION2_MEROMERO_OFF;
							break;
					}
					PushAdrsAct();
					WazaSeqAdrs=(u8 *)&WAZA_SpeabiNoJoutaiijou;
					UserWork[CLIENT_NO_WORK]=clientno;
					ClientNo=clientno;
					BSS_POKEPARA_PUT_SET(BSS_SERVER,
										 BSS_PARA_CONDITION,
										 BSS_SELMONSNO,
										 4,
										 (u8 *)&PSP[ClientNo].condition);
					ClientBitSet(ClientNo);
					return ret;
				}
			}
			break;
		case SPEABI_WAZA_HIT_AFTER_TENKIYA:
			for(clientno=0;clientno<ClientSetMax;clientno++){
				if(PSP[clientno].speabino==TOKUSYU_TENKIYA){
					ret=SpeabiTenkiyaAct(clientno);
					if(ret){
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Tenkiya);
						UserWork[CLIENT_NO_WORK]=clientno;
						UserWork[HENGE_NO]=ret-1;
						return ret;
					}
				}
			}
			break;
		case SPEABI_SINKURO_DEFENCE:
			if(SpeabiNo==TOKUSYU_SINKURO){
				if(ServerStatusFlag&SERVER_STATUS_FLAG_TSUIKA){
					ServerStatusFlag&=SERVER_STATUS_FLAG_TSUIKA_OFF;
					UserWork[TSUIKA_PARA]&=0x3f;
					if(UserWork[TSUIKA_PARA]==COND_DOKUDOKU)
						UserWork[TSUIKA_PARA]=COND_DOKU;
					ServerWork[3]=UserWork[TSUIKA_PARA]+TSUIKA_ATTACK;
					UserWork[CLIENT_NO_WORK]=DefenceClient;
					PushAdrsAct();
					WazaSeqAdrs=(u8 *)&WAZA_Sinkuro;
					ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
					ret++;
				}
			}
			break;
		case SPEABI_SINKURO_ATTACK:
			if(SpeabiNo==TOKUSYU_SINKURO){
				if(ServerStatusFlag&SERVER_STATUS_FLAG_TSUIKA){
					ServerStatusFlag&=SERVER_STATUS_FLAG_TSUIKA_OFF;
					UserWork[TSUIKA_PARA]&=0x3f;
					if(UserWork[TSUIKA_PARA]==COND_DOKUDOKU)
						UserWork[TSUIKA_PARA]=COND_DOKU;
					ServerWork[3]=UserWork[TSUIKA_PARA];
					UserWork[CLIENT_NO_WORK]=AttackClient;
					PushAdrsAct();
					WazaSeqAdrs=(u8 *)&WAZA_Sinkuro;
					ServerStatusFlag|=SERVER_STATUS_FLAG_SPEABITSUIKA;
					ret++;
				}
			}
			break;
		case SPEABI_IKAKU:
			for(i=0;i<ClientSetMax;i++){
				if((PSP[i].speabino==TOKUSYU_IKAKU)&&
				   (WazaKouka[i]&WAZAKOUKA_IKAKU)){
					SpeabiNo=TOKUSYU_IKAKU;
					WazaKouka[i]&=WAZAKOUKA_IKAKU_OFF;
					ServerSpeabiSubroutineSet((u8 *)&WAZA_Ikaku);
					UserWork[IKAKU_CLIENTNO]=i;
					ret++;
					break;
				}
			}
			break;
		case SPEABI_TRACE:
			for(i=0;i<ClientSetMax;i++){
				if((PSP[i].speabino==TOKUSYU_TOREESU)&&
				   (WazaKouka[i]&WAZAKOUKA_TRACE)){
					dir=(ClientTypeGet(i)^1)&1;
					no1=ClientNoGet(dir);
					no2=ClientNoGet(dir+2);
					if(FightType&FIGHT_TYPE_2vs2){
						if(((PSP[no1].speabino)&&(PSP[no1].hp))&&
						   ((PSP[no2].speabino)&&(PSP[no2].hp))){
							ClientNo=ClientNoGet(dir+(pp_rand()%2)*2);
							PSP[i].speabino=PSP[ClientNo].speabino;
							SpeabiNo=PSP[ClientNo].speabino;
							ret++;
						}
						else if((PSP[no1].speabino)&&(PSP[no1].hp)){
							ClientNo=no1;
							PSP[i].speabino=PSP[ClientNo].speabino;
							SpeabiNo=PSP[ClientNo].speabino;
							ret++;
						}
						else if((PSP[no2].speabino)&&(PSP[no2].hp)){
							ClientNo=no2;
							PSP[i].speabino=PSP[ClientNo].speabino;
							SpeabiNo=PSP[ClientNo].speabino;
							ret++;
						}
					}
					else{
						ClientNo=no1;
						if((PSP[no1].speabino)&&(PSP[no1].hp)){
							PSP[i].speabino=PSP[no1].speabino;
							SpeabiNo=PSP[no1].speabino;
							ret++;
						}
					}
					if(ret){
						ServerSpeabiSubroutineSet((u8 *)&WAZA_Trace);
						WazaKouka[i]&=WAZAKOUKA_TRACE_OFF;
						UserWork[CLIENT_NO_WORK]=i;
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_NICKNAME;
						EvStrBuf0[2]=ClientNo;
						EvStrBuf0[3]=SelMonsNo[ClientNo];
						EvStrBuf0[4]=EOM_;
						EvStrBuf1[0]=I_MSG;
						EvStrBuf1[1]=EV_SPEABINO;
						EvStrBuf1[2]=SpeabiNo;
						EvStrBuf1[3]=EOM_;
						break;
					}
				}
			}
			break;
		case SPEABI_IKAKU2:
			for(i=0;i<ClientSetMax;i++){
				if((PSP[i].speabino==TOKUSYU_IKAKU)&&
				   (WazaKouka[i]&WAZAKOUKA_IKAKU)){
					SpeabiNo=TOKUSYU_IKAKU;
					WazaKouka[i]&=WAZAKOUKA_IKAKU_OFF;
					PushAdrsAct();
					WazaSeqAdrs=(u8 *)&WAZA_IkakuAct;
					UserWork[IKAKU_CLIENTNO]=i;
					ret++;
					break;
				}
			}
			break;
		case SPEABI_ENEMYHAVE_SPEABI:
			dir=MineEnemyCheck(clientno);
			for(i=0;i<ClientSetMax;i++){
				if((MineEnemyCheck(i)!=dir)&&
				   (PSP[i].speabino==CheckSpeabiNo)){
					SpeabiNo=CheckSpeabiNo;
					ret=i+1;
				}
			}
			break;
		case SPEABI_MYHAVE_SPEABI:
			dir=MineEnemyCheck(clientno);
			for(i=0;i<ClientSetMax;i++){
				if((MineEnemyCheck(i)==dir)&&
				   (PSP[i].speabino==CheckSpeabiNo)){
					SpeabiNo=CheckSpeabiNo;
					ret=i+1;
				}
			}
			break;
		case SPEABI_HAVE_SPEABI:
			switch(SpeabiNo){
				case TOKUSYU_DOROASOBI:
					for(i=0;i<ClientSetMax;i++){
						if(WazaKouka[i]&WAZAKOUKA_DOROASOBI)
							ret=i+1;
					}
					break;
				case TOKUSYU_MIZUASOBI:
					for(i=0;i<ClientSetMax;i++){
						if(WazaKouka[i]&WAZAKOUKA_MIZUASOBI)
							ret=i+1;
					}
					break;
				default:
					for(i=0;i<ClientSetMax;i++){
						if(PSP[i].speabino==CheckSpeabiNo){
							SpeabiNo=CheckSpeabiNo;
							ret=i+1;
						}
					}
					break;
			}
			break;
		case SPEABI_HAVE_SPEABI_HP:
			for(i=0;i<ClientSetMax;i++){
				if((PSP[i].speabino==CheckSpeabiNo)&&
				   (PSP[i].hp)){
					SpeabiNo=CheckSpeabiNo;
					ret=i+1;
				}
			}
			break;
		case SPEABI_HAVE_SPEABI_NOMINE:
			for(i=0;i<ClientSetMax;i++){
				if((PSP[i].speabino==CheckSpeabiNo)&&
				   (i!=clientno)){
					SpeabiNo=CheckSpeabiNo;
					ret=i+1;
				}
			}
			break;
		case SPEABI_ENEMYHAVE_SPEABI_CNT:
			dir=MineEnemyCheck(clientno);
			for(i=0;i<ClientSetMax;i++){
				if((MineEnemyCheck(i)!=dir)&&
				   (PSP[i].speabino==CheckSpeabiNo)){
					SpeabiNo=CheckSpeabiNo;
					ret++;
				}
			}
			break;
		case SPEABI_MYHAVE_SPEABI_CNT:
			dir=MineEnemyCheck(clientno);
			for(i=0;i<ClientSetMax;i++){
				if((MineEnemyCheck(i)==dir)&&
				   (PSP[i].speabino==CheckSpeabiNo)){
					SpeabiNo=CheckSpeabiNo;
					ret++;
				}
			}
			break;
		case SPEABI_HAVE_SPEABI_NOMINE_CNT:
			for(i=0;i<ClientSetMax;i++){
				if((PSP[i].speabino==CheckSpeabiNo)&&
				   (i!=clientno)){
					SpeabiNo=CheckSpeabiNo;
					ret++;
				}
			}
			break;
	}
	if((ret)&&
	   (seqno<SPEABI_ENEMYHAVE_SPEABI)&&
	   (SpeabiNo!=TOKUSYU_TENKOU))
		TokusyuNoSet(clientno,SpeabiNo);
	return ret;
}

void	ServerSubroutineSet(u8 *wazaadrs)
{
	WazaSeqAdrs=wazaadrs;
	PUSH_SERVER_ADRS->RetServerSequence[PUSH_SERVER_ADRS->PushServerAdrsCnt++]=
		ServerSequence;
	ServerSequence=ServerSubroutine;
	ActionSeqNo=0;
}

void	ServerSpeabiSubroutineSet(u8 *wazaadrs)
{
//	RetSpeabiSeqAdrs=WazaSeqAdrs;
	PushAdrsAct();
	WazaSeqAdrs=wazaadrs;
	PUSH_SERVER_ADRS->RetServerSequence[PUSH_SERVER_ADRS->PushServerAdrsCnt++]=
		ServerSequence;
	ServerSequence=ServerSpeabiSubroutine;
}

//==============================================================
//		�����A�C�e���̃`�F�b�N
//==============================================================

enum{
	SIC_CONDITION=1,
	SIC_CONDITION2,
	SIC_PP,
	SIC_HP,
	SIC_ABIRITYCNT,
};

u8	SoubiItemCheck(u8 seqno,u8 clientno,u8 flag)
{
	int	i=0,j;
	u8	ret=0;
	u8	eqpno;
	u8	atc;
	u8	attackeqpno;
	u8	attackatc;
	u8	defenceeqpno;
	u8	defenceatc;
	u16	attackitem;
	u16	defenceitem;
	u16	wazano;
	u8	pp=0;
	u8	ppcnt;
	PokemonParam	*PP;

	ItemNo=PSP[clientno].item;
	if(ItemNo==ITEM_NAZONOMI){
		eqpno=FRS[clientno].item_equip;
		atc=FRS[clientno].item_power;
	}
	else{
		eqpno=ItemEquipGet(ItemNo);
		atc=ItemAttackGet(ItemNo);
	}
	attackitem=PSP[AttackClient].item;
	if(attackitem==ITEM_NAZONOMI){
		attackeqpno=FRS[AttackClient].item_equip;
		attackatc=FRS[AttackClient].item_power;
	}
	else{
		attackeqpno=ItemEquipGet(attackitem);
		attackatc=ItemAttackGet(attackitem);
	}
	defenceitem=PSP[DefenceClient].item;
	if(defenceitem==ITEM_NAZONOMI){
		defenceeqpno=FRS[DefenceClient].item_equip;
		defenceatc=FRS[DefenceClient].item_power;
	}
	else{
		defenceeqpno=ItemEquipGet(defenceitem);
		defenceatc=ItemAttackGet(defenceitem);
	}

	switch(seqno){
		case SOUBI_POKEAPPEAR:
			switch(eqpno){
				case SOUBI_OKANENIBAI:
					UserWork[OKANE2BAI]=2;		//���炦�邨����2�{�ɂ���
					break;
				case SOUBI_NOURYOKUKAIHUKU:
					for(i=0;i<COND_MAX;i++){
						if(PSP[clientno].abiritycnt[i]<6){
							PSP[clientno].abiritycnt[i]=6;
							ret=SIC_ABIRITYCNT;
						}
					}
					if(ret){
						UserWork[CLIENT_NO_WORK]=clientno;
						ItemClient=clientno;
						ClientNo=AttackClient=clientno;
						ServerSubroutineSet((u8 *)&WAZA_ItemAbiKaifuku);
					}
					break;
			}
			break;
		case SOUBI_TURN_CHECK:
			if(PSP[clientno].hp==0)
				break;
			switch(eqpno){
				case SOUBI_HPKAIHUKU:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/2))&&
					   (flag==0)){
						WazaDamage=atc;
						if((PSP[clientno].hp+WazaDamage)>PSP[clientno].hpmax)
							WazaDamage=PSP[clientno].hpmax-PSP[clientno].hp;
						WazaDamage*=-1;
						ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifuku);
						ret=SIC_HP;
					}
					break;
				case SOUBI_PPKAIHUKU:
#if 0
					if(flag==0){
						for(i=0;i<4;i++){
							if((PSP[clientno].waza[i]!=0)&&(PSP[clientno].pp[i]==0))
								break;
						}
						if(i!=4){
							j=PPMaxGet(PSP[clientno].waza[i],
									   PSP[clientno].pp_count,
									   i);
							if(PSP[clientno].pp[i]+atc>j)
								PSP[clientno].pp[i]=j;
							else
								PSP[clientno].pp[i]+=atc;
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_WAZANO;
							EvStrBuf0[2]=PSP[clientno].waza[i]&0xff;
							EvStrBuf0[3]=(PSP[clientno].waza[i]&0xff00)>>8;
							EvStrBuf0[4]=EOM_;
							ServerSubroutineSet((u8 *)&WAZA_ItemPPKaifuku);
							ret=SIC_PP;
						}
					}
#endif
					if(flag==0){
						if(MineEnemyCheck(clientno)==SIDE_MINE)
							PP=&PokeParaMine[SelMonsNo[clientno]];
						else
							PP=&PokeParaEnemy[SelMonsNo[clientno]];
						for(i=0;i<4;i++){
							wazano=PokeParaGet(PP,ID_waza1+i);
							pp=PokeParaGet(PP,ID_pp1+i);
							ppcnt=PokeParaGet(PP,ID_pp_count);
							if((wazano!=0)&&(pp==0))
								break;
						}
						if(i!=4){
							j=PPMaxGet(wazano,ppcnt,i);
							if(pp+atc>j)
								pp=j;
							else
								pp+=atc;
							EvStrBuf0[0]=I_MSG;
							EvStrBuf0[1]=EV_WAZANO;
							EvStrBuf0[2]=wazano&0xff;
							EvStrBuf0[3]=(wazano&0xff00)>>8;
							EvStrBuf0[4]=EOM_;
							ServerSubroutineSet((u8 *)&WAZA_ItemPPKaifuku);
							BSS_POKEPARA_PUT_SET(BSS_SERVER,
												 BSS_PARA_PP1+i,
												 BSS_SELMONSNO,
												 1,
												 (u8 *)&pp);
							ClientBitSet(ClientNo);
							ret=SIC_PP;
						}
					}
					break;
				case SOUBI_NOURYOKUKAIHUKU:
					for(i=0;i<COND_MAX;i++){
						if(PSP[clientno].abiritycnt[i]<6){
							PSP[clientno].abiritycnt[i]=6;
							ret=SIC_ABIRITYCNT;
						}
					}
					if(ret){
						UserWork[CLIENT_NO_WORK]=clientno;
						ItemClient=clientno;
						ClientNo=AttackClient=clientno;
						ServerSubroutineSet((u8 *)&WAZA_ItemAbiKaifuku);
					}
					break;
				case SOUBI_HPKAIHUKUMAITURN:
					if((PSP[clientno].hp<PSP[clientno].hpmax)&&
					   (flag==0)){
						WazaDamage=PSP[clientno].hpmax/16;
						if(WazaDamage==0)
							WazaDamage=1;
						if(PSP[clientno].hp+WazaDamage>PSP[clientno].hpmax)
							WazaDamage=PSP[clientno].hpmax-PSP[clientno].hp;
						WazaDamage*=-1;
						ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifukuEveryTurn);
						ret=SIC_HP;
						SoubiItemSet(clientno,eqpno);
					}
					break;
				case SOUBI_KARAIKONRAN:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/2))&&
					   (flag==0)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_TASTE;
						EvStrBuf0[2]=TASTE_KARAI;
						EvStrBuf0[3]=EOM_;
						WazaDamage=PSP[clientno].hpmax/atc;
						if(WazaDamage==0)
							WazaDamage=1;
						if((PSP[clientno].hp+WazaDamage)>PSP[clientno].hpmax)
							WazaDamage=PSP[clientno].hpmax-PSP[clientno].hp;
						WazaDamage*=-1;
						if(PokeLikeTasteCheckRnd(PSP[clientno].personal_rnd,TASTE_KARAI)<0)
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifukuKonran);
						else
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifuku);
						ret=SIC_HP;
					}
					break;
				case SOUBI_SIBUIKONRAN:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/2))&&
					   (flag==0)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_TASTE;
						EvStrBuf0[2]=TASTE_SIBUI;
						EvStrBuf0[3]=EOM_;
						WazaDamage=PSP[clientno].hpmax/atc;
						if(WazaDamage==0)
							WazaDamage=1;
						if((PSP[clientno].hp+WazaDamage)>PSP[clientno].hpmax)
							WazaDamage=PSP[clientno].hpmax-PSP[clientno].hp;
						WazaDamage*=-1;
						if(PokeLikeTasteCheckRnd(PSP[clientno].personal_rnd,TASTE_SIBUI)<0)
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifukuKonran);
						else
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifuku);
						ret=SIC_HP;
					}
					break;
				case SOUBI_AMAIKONRAN:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/2))&&
					   (flag==0)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_TASTE;
						EvStrBuf0[2]=TASTE_AMAI;
						EvStrBuf0[3]=EOM_;
						WazaDamage=PSP[clientno].hpmax/atc;
						if(WazaDamage==0)
							WazaDamage=1;
						if((PSP[clientno].hp+WazaDamage)>PSP[clientno].hpmax)
							WazaDamage=PSP[clientno].hpmax-PSP[clientno].hp;
						WazaDamage*=-1;
						if(PokeLikeTasteCheckRnd(PSP[clientno].personal_rnd,TASTE_AMAI)<0)
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifukuKonran);
						else
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifuku);
						ret=SIC_HP;
					}
					break;
				case SOUBI_NIGAIKONRAN:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/2))&&
					   (flag==0)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_TASTE;
						EvStrBuf0[2]=TASTE_NIGAI;
						EvStrBuf0[3]=EOM_;
						WazaDamage=PSP[clientno].hpmax/atc;
						if(WazaDamage==0)
							WazaDamage=1;
						if((PSP[clientno].hp+WazaDamage)>PSP[clientno].hpmax)
							WazaDamage=PSP[clientno].hpmax-PSP[clientno].hp;
						WazaDamage*=-1;
						if(PokeLikeTasteCheckRnd(PSP[clientno].personal_rnd,TASTE_NIGAI)<0)
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifukuKonran);
						else
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifuku);
						ret=SIC_HP;
					}
					break;
				case SOUBI_SUPPAIKONRAN:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/2))&&
					   (flag==0)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_TASTE;
						EvStrBuf0[2]=TASTE_SUPPAI;
						EvStrBuf0[3]=EOM_;
						WazaDamage=PSP[clientno].hpmax/atc;
						if(WazaDamage==0)
							WazaDamage=1;
						if((PSP[clientno].hp+WazaDamage)>PSP[clientno].hpmax)
							WazaDamage=PSP[clientno].hpmax-PSP[clientno].hp;
						WazaDamage*=-1;
						if(PokeLikeTasteCheckRnd(PSP[clientno].personal_rnd,TASTE_SUPPAI)<0)
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifukuKonran);
						else
							ServerSubroutineSet((u8 *)&WAZA_ItemHPKaifuku);
						ret=SIC_HP;
					}
					break;
				case SOUBI_KOUGEKIUP:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/atc))&&
					   (flag==0)&&
					   (PSP[clientno].abiritycnt[COND_POW]<12)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_STATUS;
						EvStrBuf0[2]=COND_POW;
						EvStrBuf0[3]=EOM_;
						EvStrBuf1[0]=I_MSG;
						EvStrBuf1[1]=EV_MSGID;
						EvStrBuf1[2]=AGATTAMSGNO&0xff;
						EvStrBuf1[3]=(AGATTAMSGNO&0xff00)>>8;
						EvStrBuf1[4]=EOM_;
						TsuikaClient=clientno;
						UserWork[COND_CHG_WORK]=COND_POW+PARA_RANKU1;
						UserWork[FIGHT_EFFECT_WORK+0]=COND2_POWUP;
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						ServerSubroutineSet((u8 *)&WAZA_ItemCondUp);
						ret=SIC_ABIRITYCNT;
					}
					break;
				case SOUBI_BOUGYOUP:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/atc))&&
					   (flag==0)&&
					   (PSP[clientno].abiritycnt[COND_DEF]<12)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_STATUS;
						EvStrBuf0[2]=COND_DEF;
						EvStrBuf0[3]=EOM_;
						TsuikaClient=clientno;
						UserWork[COND_CHG_WORK]=COND_DEF+PARA_RANKU1;
						UserWork[FIGHT_EFFECT_WORK+0]=COND2_DEFUP;
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						ServerSubroutineSet((u8 *)&WAZA_ItemCondUp);
						ret=SIC_ABIRITYCNT;
					}
					break;
				case SOUBI_SUBAYASAUP:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/atc))&&
					   (flag==0)&&
					   (PSP[clientno].abiritycnt[COND_AGI]<12)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_STATUS;
						EvStrBuf0[2]=COND_AGI;
						EvStrBuf0[3]=EOM_;
						TsuikaClient=clientno;
						UserWork[COND_CHG_WORK]=COND_AGI+PARA_RANKU1;
						UserWork[FIGHT_EFFECT_WORK+0]=COND2_AGIUP;
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						ServerSubroutineSet((u8 *)&WAZA_ItemCondUp);
						ret=SIC_ABIRITYCNT;
					}
					break;
				case SOUBI_TOKUKOUUP:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/atc))&&
					   (flag==0)&&
					   (PSP[clientno].abiritycnt[COND_SPEPOW]<12)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_STATUS;
						EvStrBuf0[2]=COND_SPEPOW;
						EvStrBuf0[3]=EOM_;
						TsuikaClient=clientno;
						UserWork[COND_CHG_WORK]=COND_SPEPOW+PARA_RANKU1;
						UserWork[FIGHT_EFFECT_WORK+0]=COND2_SPEPOWUP;
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						ServerSubroutineSet((u8 *)&WAZA_ItemCondUp);
						ret=SIC_ABIRITYCNT;
					}
					break;
				case SOUBI_TOKUBOUUP:
					if((PSP[clientno].hp<=(PSP[clientno].hpmax/atc))&&
					   (flag==0)&&
					   (PSP[clientno].abiritycnt[COND_SPEDEF]<12)){
						EvStrBuf0[0]=I_MSG;
						EvStrBuf0[1]=EV_STATUS;
						EvStrBuf0[2]=COND_SPEDEF;
						EvStrBuf0[3]=EOM_;
						TsuikaClient=clientno;
						UserWork[COND_CHG_WORK]=COND_SPEDEF+PARA_RANKU1;
						UserWork[FIGHT_EFFECT_WORK+0]=COND2_SPEDEFUP;
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						ServerSubroutineSet((u8 *)&WAZA_ItemCondUp);
						ret=SIC_ABIRITYCNT;
					}
					break;
				case SOUBI_KURITHIKARUUP:
					if(((PSP[clientno].hp<=(PSP[clientno].hpmax/atc))&&
					    (flag==0)&&
					    (PSP[clientno].condition2&CONDITION2_KIAIDAME)==0)){
					    PSP[clientno].condition2|=CONDITION2_KIAIDAME;
						ServerSubroutineSet((u8 *)&WAZA_ItemCriticalUp);
						ret=SIC_CONDITION2;
					}
					break;
				case SOUBI_NOURYOKURNDUP:
					if(flag==0){
						if(PSP[clientno].hp<=(PSP[clientno].hpmax/atc)){
							for(i=0;i<5;i++){
								if(PSP[clientno].abiritycnt[COND_POW+i]<12)
									break;
							}
							if(i!=5){
								do{
									i=pp_rand()%5;
								}while(PSP[clientno].abiritycnt[COND_POW+i]==12);
								EvStrBuf0[0]=I_MSG;
								EvStrBuf0[1]=EV_STATUS;
								EvStrBuf0[2]=COND_POW+i;
								EvStrBuf0[3]=EOM_;
								EvStrBuf1[0]=I_MSG;
								EvStrBuf1[1]=EV_MSGID;
								EvStrBuf1[2]=GUNTOMSGNO&0xff;
								EvStrBuf1[3]=(GUNTOMSGNO&0xff00)>>8;
								EvStrBuf1[4]=EV_MSGID;
								EvStrBuf1[5]=AGATTAMSGNO&0xff;
								EvStrBuf1[6]=(AGATTAMSGNO&0xff00)>>8;
								EvStrBuf1[7]=EOM_;
								TsuikaClient=clientno;
								UserWork[COND_CHG_WORK]=COND_POW+PARA_RANKU2+i;
								UserWork[FIGHT_EFFECT_WORK+0]=COND2_POWUP2+i;
								UserWork[FIGHT_EFFECT_WORK+1]=0;
								ServerSubroutineSet((u8 *)&WAZA_ItemCondUp);
								ret=SIC_ABIRITYCNT;
							}
						}
					}
					break;
#if 1
				case SOUBI_MAHINAOSI:
					if(PSP[clientno].condition&CONDITION_MAHI){
						PSP[clientno].condition&=CONDITION_MAHI_OFF;
						ServerSubroutineSet((u8 *)&WAZA_ItemNoMahi);
						ret=SIC_CONDITION;
					}
					break;
				case SOUBI_DOKUKESI:
					if(PSP[clientno].condition&(CONDITION_DOKU|CONDITION_DOKUDOKU)){
						PSP[clientno].condition&=CONDITION_POISON_OFF;
						ServerSubroutineSet((u8 *)&WAZA_ItemNoDoku);
						ret=SIC_CONDITION;
					}
					break;
				case SOUBI_YAKEDONAOSI:
					if(PSP[clientno].condition&CONDITION_YAKEDO){
						PSP[clientno].condition&=CONDITION_YAKEDO_OFF;
						ServerSubroutineSet((u8 *)&WAZA_ItemNoYakedo);
						ret=SIC_CONDITION;
					}
					break;
				case SOUBI_KOORINAOSI:
					if(PSP[clientno].condition&CONDITION_KOORI){
						PSP[clientno].condition&=CONDITION_KOORI_OFF;
						ServerSubroutineSet((u8 *)&WAZA_ItemNoKoori);
						ret=SIC_CONDITION;
					}
					break;
				case SOUBI_NEMUKEZAMASI:
					if(PSP[clientno].condition&CONDITION_NEMURI){
						PSP[clientno].condition&=CONDITION_NEMURI_OFF;
						PSP[clientno].condition2&=CONDITION2_AKUMU_OFF;
						ServerSubroutineSet((u8 *)&WAZA_ItemNoNemuri);
						ret=SIC_CONDITION;
					}
					break;
				case SOUBI_KONRANNAOSI:
					if(PSP[clientno].condition2&CONDITION2_KONRAN){
						PSP[clientno].condition2&=CONDITION2_KONRAN_OFF;
						ServerSubroutineSet((u8 *)&WAZA_ItemNoKonran);
						ret=SIC_CONDITION2;
					}
					break;
				case SOUBI_GYOUTAIIGYOUNAOSI:
					if((PSP[clientno].condition&CONDITION_BAD)||
					   (PSP[clientno].condition2&CONDITION2_KONRAN)){
						i=0;
						if(PSP[clientno].condition&(CONDITION_DOKU|CONDITION_DOKUDOKU)){
							PM_strcpy(EvStrBuf0,DokuMsg);
							i++;
						}
						if(PSP[clientno].condition&CONDITION_NEMURI){
							PSP[clientno].condition2&=CONDITION2_AKUMU_OFF;
							PM_strcpy(EvStrBuf0,NemuriMsg);
							i++;
						}
						if(PSP[clientno].condition&CONDITION_MAHI){
							PM_strcpy(EvStrBuf0,MahiMsg);
							i++;
						}
						if(PSP[clientno].condition&CONDITION_YAKEDO){
							PM_strcpy(EvStrBuf0,YakedoMsg);
							i++;
						}
						if(PSP[clientno].condition&CONDITION_KOORI){
							PM_strcpy(EvStrBuf0,KooriMsg);
							i++;
						}
						if(PSP[clientno].condition2&CONDITION2_KONRAN){
							PM_strcpy(EvStrBuf0,KonranMsg);
							i++;
						}
						if(i<2)
							ServerWork[5]=0;
						else
							ServerWork[5]=1;
						PSP[clientno].condition=0;
						PSP[clientno].condition2&=CONDITION2_KONRAN_OFF;
						ServerSubroutineSet((u8 *)&WAZA_ItemNoJoutaiijou);
						ret=SIC_CONDITION;
					}
					break;
				case SOUBI_MEROMERONAOSI:
					if(PSP[clientno].condition2&CONDITION2_MEROMERO){
						PSP[clientno].condition2&=CONDITION2_MEROMERO_OFF;
						PM_strcpy(EvStrBuf0,MeromeroMsg);
						ServerSubroutineSet((u8 *)&WAZA_ItemNoJoutaiijou);
						ServerWork[5]=0;
						ret=SIC_CONDITION2;
					}
					break;
#endif
			}
			if(ret){
				UserWork[CLIENT_NO_WORK]=clientno;
				ItemClient=clientno;
				ClientNo=AttackClient=clientno;
				switch(ret){
					case SIC_CONDITION:		//condition
						BSS_POKEPARA_PUT_SET(BSS_SERVER,
											 BSS_PARA_CONDITION,
											 BSS_SELMONSNO,
											 4,
											 (u8 *)&PSP[clientno].condition);
						ClientBitSet(ClientNo);
						break;
					case SIC_PP:			//pp
#if 0
						BSS_POKEPARA_PUT_SET(BSS_SERVER,
											 BSS_PARA_PP1+i,
											 BSS_SELMONSNO,
											 1,
											 (u8 *)&PSP[clientno].pp[i]);
						ClientBitSet(ClientNo);
#endif
						if(((PSP[clientno].condition2&CONDITION2_HENSHIN)==0)&&
						   ((WKC[clientno].monomanebit&no2bittbl[i])==0))
							PSP[clientno].pp[i]=pp;
						break;
				}
			}
			break;
		case SOUBI_WAZA_HIT:
			break;
		case SOUBI_WAZA_HIT_AFTER_ALL:
			for(clientno=0;clientno<ClientSetMax;clientno++){
				ItemNo=PSP[clientno].item;
				if(ItemNo==ITEM_NAZONOMI){
					eqpno=FRS[clientno].item_equip;
					atc=FRS[clientno].item_power;
				}
				else{
					eqpno=ItemEquipGet(ItemNo);
					atc=ItemAttackGet(ItemNo);
				}
				switch(eqpno){
					case SOUBI_MAHINAOSI:
						if(PSP[clientno].condition&CONDITION_MAHI){
							PSP[clientno].condition&=CONDITION_MAHI_OFF;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_ItemNoMahi2;
							ret=SIC_CONDITION;
						}
						break;
					case SOUBI_DOKUKESI:
						if(PSP[clientno].condition&(CONDITION_DOKU|CONDITION_DOKUDOKU)){
							PSP[clientno].condition&=CONDITION_POISON_OFF;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_ItemNoDoku2;
							ret=SIC_CONDITION;
						}
						break;
					case SOUBI_YAKEDONAOSI:
						if(PSP[clientno].condition&CONDITION_YAKEDO){
							PSP[clientno].condition&=CONDITION_YAKEDO_OFF;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_ItemNoYakedo2;
							ret=SIC_CONDITION;
						}
						break;
					case SOUBI_KOORINAOSI:
						if(PSP[clientno].condition&CONDITION_KOORI){
							PSP[clientno].condition&=CONDITION_KOORI_OFF;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_ItemNoKoori2;
							ret=SIC_CONDITION;
						}
						break;
					case SOUBI_NEMUKEZAMASI:
						if(PSP[clientno].condition&CONDITION_NEMURI){
							PSP[clientno].condition&=CONDITION_NEMURI_OFF;
							PSP[clientno].condition2&=CONDITION2_AKUMU_OFF;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_ItemNoNemuri2;
							ret=SIC_CONDITION;
						}
						break;
					case SOUBI_KONRANNAOSI:
						if(PSP[clientno].condition2&CONDITION2_KONRAN){
							PSP[clientno].condition2&=CONDITION2_KONRAN_OFF;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_ItemNoKonran2;
							ret=SIC_CONDITION2;
						}
						break;
					case SOUBI_MEROMERONAOSI:
						if(PSP[clientno].condition2&CONDITION2_MEROMERO){
							PSP[clientno].condition2&=CONDITION2_MEROMERO_OFF;
							PM_strcpy(EvStrBuf0,MeromeroMsg);
							PushAdrsAct();
							ServerWork[5]=0;
							WazaSeqAdrs=(u8 *)&WAZA_ItemNoJoutaiijou2;
							ret=SIC_CONDITION2;
						}
						break;
					case SOUBI_GYOUTAIIGYOUNAOSI:
						if((PSP[clientno].condition&CONDITION_BAD)||
						   (PSP[clientno].condition2&CONDITION2_KONRAN)){
							if(PSP[clientno].condition&(CONDITION_DOKU|CONDITION_DOKUDOKU))
								PM_strcpy(EvStrBuf0,DokuMsg);
							if(PSP[clientno].condition&CONDITION_NEMURI){
								PSP[clientno].condition2&=CONDITION2_AKUMU_OFF;
								PM_strcpy(EvStrBuf0,NemuriMsg);
							}
							if(PSP[clientno].condition&CONDITION_MAHI)
								PM_strcpy(EvStrBuf0,MahiMsg);
							if(PSP[clientno].condition&CONDITION_YAKEDO)
								PM_strcpy(EvStrBuf0,YakedoMsg);
							if(PSP[clientno].condition&CONDITION_KOORI)
								PM_strcpy(EvStrBuf0,KooriMsg);
							if(PSP[clientno].condition2&CONDITION2_KONRAN)
								PM_strcpy(EvStrBuf0,KonranMsg);
							PSP[clientno].condition=0;
							PSP[clientno].condition2&=CONDITION2_KONRAN_OFF;
							PushAdrsAct();
							ServerWork[5]=0;
							WazaSeqAdrs=(u8 *)&WAZA_ItemNoJoutaiijou2;
							ret=SIC_CONDITION;
						}
						break;
					case SOUBI_NOURYOKUKAIHUKU:
						for(i=0;i<COND_MAX;i++){
							if(PSP[clientno].abiritycnt[i]<6){
								PSP[clientno].abiritycnt[i]=6;
								ret=SIC_ABIRITYCNT;
							}
						}
						if(ret){
							UserWork[CLIENT_NO_WORK]=clientno;
							ItemClient=clientno;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_ItemAbiKaifuku2;
							return ret;
						}
						break;
				}
				if(ret){
					UserWork[CLIENT_NO_WORK]=clientno;
					ItemClient=clientno;
					ClientNo=clientno;
					BSS_POKEPARA_PUT_SET(BSS_SERVER,
										 BSS_PARA_CONDITION,
										 BSS_SELMONSNO,
										 4,
										 (u8 *)&PSP[ClientNo].condition);
					ClientBitSet(ClientNo);
					break;
				}
			}
			break;
		case SOUBI_WAZA_HIT_AFTER:
			if(WazaDamage==0)
				break;
			switch(attackeqpno){
				case SOUBI_HIRUMASERU:
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					   ((OSTF[DefenceClient].butsurihitdamage)||
					    (OSTF[DefenceClient].tokusyuhitdamage))&&
					   ((pp_rand()%100)<atc)&&
						(WTD[WazaNo].flag&FLAG_OUZYANOSIRUSI)&&
						(PSP[DefenceClient].hp!=0)){
						ServerWork[3]=COND2_HIRUMU;
						PushAdrsAct();
						TsuikaAct(TSUIKA_INDIRECT,0);
						PopAdrsAct();
					}
					break;
				case SOUBI_HPKAIHUKUDAMEEZIRITU:
					if(((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					   ((OSTF[DefenceClient].kaigaradamage)&&
					    (OSTF[DefenceClient].kaigaradamage!=0xffff))&&
					    (AttackClient!=DefenceClient)&&
						(PSP[AttackClient].hp!=PSP[AttackClient].hpmax)&&
						(PSP[AttackClient].hp)){
						ItemNo=attackitem;
						ItemClient=AttackClient;
						UserWork[CLIENT_NO_WORK]=AttackClient;
						WazaDamage=-(OSTF[DefenceClient].kaigaradamage/attackatc);
						if(WazaDamage==0)
							WazaDamage=-1;
						OSTF[DefenceClient].kaigaradamage=0;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_ItemHPKaifukuEveryTurn2;
						ret++;
					}
					break;
			}
			break;
	}
	return ret;
}

#if 0
#define	SOUBI_HIRUMASERU					18
#endif

//==============================================================
//		���͋Z�̃`�F�b�N
//==============================================================

void	KyouryokuWazaCheck(void)
{
	int	i=0;
	u8	side;

	if((FightType&FIGHT_TYPE_2vs2)==0) return;

	do{
		side=0;
		do{
//			if((ServerStatusFlag&no2bittbl[ClientTypeGet(side)])||
//			   (ServerStatusFlag&no2bittbl[ClientTypeGet(side+2)])){
			if((NoReshuffleClient&no2bittbl[side])||
			   (NoReshuffleClient&no2bittbl[side+2])){
			   side++;
			}
			else{
				if((KW[i].waza1==SelectWazaNo[side])&&
				   (KW[i].waza2==SelectWazaNo[side+2])){
					SCC[ClientTypeGet(side)&1].kyouwazaclient=side|((side+2)<<4);
					SCC[ClientTypeGet(side)&1].kyouwazano=KW[i].wazano;
					SideCondition[ClientTypeGet(side)&1]|=SIDE_CONDITION_KYOUWAZA;
				}
				if((KW[i].waza1==SelectWazaNo[side+2])&&
				   (KW[i].waza2==SelectWazaNo[side])){
					SCC[ClientTypeGet(side)&1].kyouwazaclient=(side+2)|(side<<4);
					SCC[ClientTypeGet(side)&1].kyouwazano=KW[i].wazano;
					SideCondition[ClientTypeGet(side)&1]|=SIDE_CONDITION_KYOUWAZA;
				}
				side++;
			}
		}while(side<2);
		i++;
	}while(KW[i].waza1!=0xffff);
}

//==============================================================
//                       �킴�̃V�[�P���X
//==============================================================

void	WS_BADGECHECK(void);
void	WS_HITCHECK(void);
void	WS_ATTACK_MESSAGE(void);
void	WS_PP_DEC(void);
void	WS_CRITICAL_CHECK(void);
void	WS_DAMAGE_CALC(void);
void	WS_TYPE_CHECK(void);
void	WS_DAMAGE_LOSS(void);
void	WS_DAMAGE_LOSS_ONLY(void);
void	WS_WAZA_EFFECT(void);
void	WS_WAZA_EFFECT_WAIT(void);
void	WS_HP_DEC(void);
void	WS_HP_DEC_WAIT(void);
void	WS_CRITICAL_MESSAGE(void);
void	WS_WAZAKOUKA_SE(void);
void	WS_WAZAKOUKA_MESSAGE(void);
void	WS_MESSAGE(void);
void	WS_ALERT_MESSAGE(void);
void	WS_MESSAGE_WAIT(void);
void	WS_MESSAGE_TBL(void);
void	WS_ALERT_MESSAGE_TBL(void);
void	WS_TSUIKA_CHECK(void);
void	WS_TSUIKA_DIRECT_ACT(void);
void	WS_TSUIKA_INDIRECT_ACT(void);
void	WS_SAKUJO(void);
void	WS_KIZETSU_CHECK(void);
void	WS_KIZETSU(void);
void	WS_KIZETSU_WAIT(void);
void	WS_CONDITION_CHECK(void);
void	WS_CONDITION2_CHECK(void);
void	WS_SPEABI_CHECK(void);
void	WS_SIDECONDITION_CHECK(void);
void	WS_CONDCHG_CHECK(void);
void	WS_WAZAKOUKA_CHECK(void);
void	WS_CHECK_TYPE(void);
void	WS_GET_EXP(void);
void	WS_ZENMETSU_CHECK(void);
void	WS_LOOP_INIT(void);
void	WS_LOOPCNT(void);
void	WS_LOOP(void);
void	WS_JUMP(void);
void	WS_IF8(void);
void	WS_IF16(void);
void	WS_IF32(void);
void	WS_IFWEQ(void);
void	WS_IFWNE(void);
void	WS_SETVALUE(void);
void	WS_ADDVALUE(void);
void	WS_SUBVALUE(void);
void	WS_SETWORK(void);
void	WS_SETWORKOFS(void);
void	WS_SET8BIT(void);
void	WS_SET16BIT(void);
void	WS_SET32BIT(void);
void	WS_RESET8BIT(void);
void	WS_RESET16BIT(void);
void	WS_RESET32BIT(void);
void	WS_WAIT(void);
void	WS_CLIENT_BIT_WAIT(void);
void	WS_HP_GAUGE_REDRAW(void);
void	WS_SEQRET(void);
void	WS_WAZAEND(void);
void	WS_SEQEND(void);
void	WS_SPEABIEND(void);
void	WS_GUARDCHECK(void);
void	WS_GOSUB(void);
void	WS_MONS_TYPE(void);
void	WS_SPEABI_HAVE_CHECK(void);
void	WS_ALERTEND(void);
void	WS_FIGHT_EFFECT(void);
void	WS_FIGHT_EFFECT2(void);
void	WS_COND_CHG_WORK_SET(void);
void	WS_COND_CHG_BIT_CHECK(void);
void	WS_WKC_ACT(void);
void	WS_TYPE_CHECK_NO_HIT(void);

//����A�N�V����
void	WS_MODOSU_EFFECT(void);
void	WS_APPEAR_EFFECT_INIT(void);
void	WS_APPEAR_EFFECT(void);
void	WS_APPEAR_EFFECT_WAIT(void);
void	WS_RESHUFFLE_CHECK(void);
void	WS_POKE_RESHUFFLE(void);
void	WS_POKE_RESHUFFLE_WAIT(void);
void	WS_POKE_RESHUFFLE_END(void);
void	WS_TRAINER_APPEAR(void);
void	WS_SE_PLAY(void);
void	WS_ME_PLAY(void);
void	WS_VOICE(void);
void	WS_END_FIGHT(void);
void	WS_POKEDELACTOR(void);
void	WS_WAZAOBOE_CHECK(void);
void	WS_WAZAWASURE_CHECK(void);
void	WS_WAZAAKIRAME_CHECK(void);
void	WS_POKE_BLINK(void);
void	WS_OKODUKAI_CALC(void);
void	WS_WAZA_GET(void);
void	WS_SWITCH_A_D(void);
void	WS_INC_RECORD(void);
void	WS_BALL_GAUGE_SET(void);
void	WS_BALL_GAUGE_OFF(void);
void	WS_WAZASEQADRS(void);
void	WS_STATUS_EFFECT(void);
void	WS_STATUS_EFFECT2(void);
void	WS_STATUS_EFFECT_DIRECT(void);
void	WS_YES_NO_WINDOW(void);
void	WS_SCA_END_SET(void);
void	WS_KORAERU_CHECK(void);
void	WS_KILLITEM(void);
void	WS_RESHUFFLE_NICKNAME(void);
void	WS_STATUS_WINDOW(void);
void	WS_EXPFLAG_CLEAR(void);
void	WS_GET_RESHUFFLE_CLIENT(void);
void	WS_VANISH_OFF(void);
void	WS_TOKUSYUNO_SET(void);
void	WS_OBOEWAZANO_SET(void);
void	WS_ESCAPE_CHECK(void);
void	WS_POKE_HPMAX_RATE(void);
void	WS_POKE_HPDEC_RATE(void);
void	WS_STATUS_RECOVER(void);
void	WS_SUBROUTINE_CALL(void);

//�Z�V�[�P���X
void	WS_GUARD_SUCCESS_CHECK(void);
void	WS_JIBAKU_INIT(void);
void	WS_JIBAKU(void);
void	WS_JIBAKU_LOOP(void);
void	WS_KAIFUKU(void);
void	WS_OUMU_CHECK(void);
void	WS_AMAGOI(void);
void	WS_REFLEC(void);
void	WS_YADORIGI(void);
void	WS_DAMAGE_CHG(void);
void	WS_NEMURU(void);
void	WS_NEKODAMASHI(void);
void	WS_SAWAGU(void);
void	WS_SAWAGU_CHECK(void);
void	WS_TAKUWAERU(void);
void	WS_HAKIDASU(void);
void	WS_NOMIKOMU(void);
void	WS_SUITORI(void);
void	WS_CONDCHG(void);
void	WS_KUROIKIRI(void);
void	WS_GAMAN(void);
void	WS_ABARERU(void);
void	WS_RENZOKU(void);
void	WS_RENZOKU_NUM_INIT(void);
void	WS_HUKITOBASHI(void);
void	WS_TEXTURE(void);
void	WS_KOBAN_CHECK(void);
void	WS_HIKARINOKABE(void);
void	WS_ICHIGEKI(void);
void	WS_HPHALF(void);
void	WS_SUNAARASHI(void);
void	WS_WEATHER_KEEP(void);
void	WS_MEROMERO(void);
void	WS_CONDITION_GAUGE(void);
void	WS_SHIROIKIRI(void);
void	WS_KIAIDAME(void);
void	WS_HENSHIN(void);
void	WS_MIGAWARI(void);
void	WS_MONOMANE(void);
void	WS_YUBIWOHURU(void);
void	WS_CHIKYUUNAGE(void);
void	WS_PSYCHOWAVE(void);
void	WS_COUNTER(void);
void	WS_MIRRORCOAT(void);
void	WS_KANASHIBARI(void);
void	WS_ENCORE(void);
void	WS_ITAMIWAKE(void);
void	WS_TEXTURE2(void);
void	WS_LOCKON(void);
void	WS_SKETCH(void);
void	WS_NEGOTO(void);
void	WS_MICHIDURE(void);
void	WS_MICHIDURE_CHECK(void);
void	WS_JITABATA(void);
void	WS_URAMI(void);
void	WS_IYASHINOSUZU(void);
void	WS_NOROI(void);
void	WS_MAKIBISHI(void);
void	WS_MIYABURU(void);
void	WS_HOROBINOUTA(void);
void	WS_KOROGARU(void);
void	WS_IBARU(void);
void	WS_RENZOKUGIRI(void);
void	WS_NATSUKI(void);
void	WS_PRESENT(void);
void	WS_SHINPINOMAMORI(void);
void	WS_MAGNITUDE(void);
void	WS_OIUCHI_CHECK(void);
void	WS_NIHONBARE(void);
void	WS_HARADAIKO(void);
void	WS_JIKOANJI(void);
void	WS_KOUSOKUSPIN(void);
void	WS_MARUKUNARU(void);
void	WS_WEATHERKAIFUKU(void);
void	WS_MEZAMERUPOWER(void);
void	WS_JISIN(void);
void	WS_MIRAIYOCHI(void);
void	WS_HUKURODATAKI(void);
void	WS_KIERUTAME(void);
void	WS_KIERUTAME_AFTAR(void);
void	WS_CHIISAKUNARU(void);
void	WS_ARARE(void);
void	WS_OKIMIYAGE(void);
void	WS_KONOYUBITOMARE(void);
void	WS_JUUDEN(void);
void	WS_SHIZENNOCHIKARA(void);
void	WS_REFLESH(void);
void	WS_ICHAMON(void);
void	WS_KIAIPUNCH(void);
void	WS_CHOUHATSU(void);
void	WS_TEDASUKE(void);
void	WS_TRICK(void);
void	WS_NARIKIRI(void);
void	WS_NEGAIGOTO(void);
void	WS_NEWOHARU(void);
void	WS_REVENGE(void);
void	WS_AKUBI(void);
void	WS_GAMUSYARA(void);
void	WS_HUNKA(void);
void	WS_SKILLSWAP(void);
void	WS_HUUIN(void);
void	WS_ONNEN(void);
void	WS_KETAGURI(void);
void	WS_NEKONOTE(void);
void	WS_MAGICCORT(void);
void	WS_YOKODORI(void);
void	WS_IKAKU(void);
void	WS_SIZENKAIHUKU(void);
void	WS_HP_CHECK(void);
void	WS_HIMITUNOTIKARA(void);
void	WS_MONOHIROI(void);
void	WS_TENKIYA(void);
void	WS_TENKIYA_CHECK(void);
void	WS_ASOBI(void);
void	WS_WEATHER_BALL(void);
void	WS_RECYCLE(void);
void	WS_HOGOSYOKU(void);
void	WS_HUKITOBASHI_OIUCHI(void);
void	WS_YOKODORI_CLIENT_SET(void);
void	WS_KAWARAWARI(void);

//�A�C�e���V�[�P���X
void	WS_BALL_THROW(void);
void	WS_ADD_POKE(void);
void	WS_ZUKAN_CHECK(void);
void	WS_ZUKAN(void);
void	WS_NICKNAME(void);
void	WS_ITEM_HP_KAIFUKU(void);
void	WS_ITEM_CONDITION_RECOVER(void);
void	WS_ITEMEND(void);
void	WS_GETEND(void);


const pFunc WazaSequenceTbl[] =
{
	WS_BADGECHECK,
	WS_HITCHECK,
	WS_ATTACK_MESSAGE,
	WS_PP_DEC,
	WS_CRITICAL_CHECK,
	WS_DAMAGE_CALC,
	WS_TYPE_CHECK,
	WS_DAMAGE_LOSS,
	WS_DAMAGE_LOSS_ONLY,
	WS_WAZA_EFFECT,
	WS_WAZA_EFFECT_WAIT,
	WS_HP_DEC,
	WS_HP_DEC_WAIT,
	WS_CRITICAL_MESSAGE,
	WS_WAZAKOUKA_SE,
	WS_WAZAKOUKA_MESSAGE,
	WS_MESSAGE,
	WS_ALERT_MESSAGE,
	WS_MESSAGE_WAIT,
	WS_MESSAGE_TBL,
	WS_ALERT_MESSAGE_TBL,
	WS_TSUIKA_CHECK,
	WS_TSUIKA_DIRECT_ACT,
	WS_TSUIKA_INDIRECT_ACT,
	WS_SAKUJO,
	WS_KIZETSU_CHECK,
	WS_KIZETSU,
	WS_KIZETSU_WAIT,
	WS_CONDITION_CHECK,
	WS_CONDITION2_CHECK,
	WS_SPEABI_CHECK,
	WS_SIDECONDITION_CHECK,
	WS_CONDCHG_CHECK,
	WS_WAZAKOUKA_CHECK,
	WS_CHECK_TYPE,
	WS_GET_EXP,
	WS_ZENMETSU_CHECK,
	WS_LOOP_INIT,
	WS_LOOPCNT,
	WS_LOOP,
	WS_JUMP,
	WS_IF8,
	WS_IF16,
	WS_IF32,
	WS_IFWEQ,
	WS_IFWNE,
	WS_SETVALUE,
	WS_ADDVALUE,
	WS_SUBVALUE,
	WS_SETWORK,
	WS_SETWORKOFS,
	WS_SET8BIT,
	WS_SET16BIT,
	WS_SET32BIT,
	WS_RESET8BIT,
	WS_RESET16BIT,
	WS_RESET32BIT,
	WS_WAIT,
	WS_CLIENT_BIT_WAIT,
	WS_HP_GAUGE_REDRAW,
	WS_SEQRET,
	WS_WAZAEND,
	WS_SEQEND,
	WS_SPEABIEND,
	WS_GUARDCHECK,
	WS_GOSUB,
	WS_MONS_TYPE,
	WS_SPEABI_HAVE_CHECK,
	WS_ALERTEND,
	WS_FIGHT_EFFECT,
	WS_FIGHT_EFFECT2,
	WS_COND_CHG_WORK_SET,
	WS_COND_CHG_BIT_CHECK,
	WS_WKC_ACT,
	WS_TYPE_CHECK_NO_HIT,

//����A�N�V����
	WS_MODOSU_EFFECT,
	WS_APPEAR_EFFECT_INIT,
	WS_APPEAR_EFFECT,
	WS_APPEAR_EFFECT_WAIT,
	WS_RESHUFFLE_CHECK,
	WS_POKE_RESHUFFLE,
	WS_POKE_RESHUFFLE_WAIT,
	WS_POKE_RESHUFFLE_END,
	WS_TRAINER_APPEAR,
	WS_SE_PLAY,
	WS_ME_PLAY,
	WS_VOICE,
	WS_END_FIGHT,
	WS_POKEDELACTOR,
	WS_WAZAOBOE_CHECK,
	WS_WAZAWASURE_CHECK,
	WS_WAZAAKIRAME_CHECK,
	WS_POKE_BLINK,
	WS_OKODUKAI_CALC,
	WS_WAZA_GET,
	WS_SWITCH_A_D,
	WS_INC_RECORD,
	WS_BALL_GAUGE_SET,
	WS_BALL_GAUGE_OFF,
	WS_WAZASEQADRS,
	WS_STATUS_EFFECT,
	WS_STATUS_EFFECT2,
	WS_STATUS_EFFECT_DIRECT,
	WS_YES_NO_WINDOW,
	WS_SCA_END_SET,
	WS_KORAERU_CHECK,
	WS_KILLITEM,
	WS_RESHUFFLE_NICKNAME,
	WS_STATUS_WINDOW,
	WS_EXPFLAG_CLEAR,
	WS_GET_RESHUFFLE_CLIENT,
	WS_VANISH_OFF,
	WS_TOKUSYUNO_SET,
	WS_OBOEWAZANO_SET,
	WS_ESCAPE_CHECK,
	WS_POKE_HPMAX_RATE,
	WS_POKE_HPDEC_RATE,
	WS_STATUS_RECOVER,
	WS_SUBROUTINE_CALL,

//�Z�V�[�P���X
	WS_GUARD_SUCCESS_CHECK,
	WS_JIBAKU_INIT,
	WS_JIBAKU,
	WS_JIBAKU_LOOP,
	WS_KAIFUKU,
	WS_OUMU_CHECK,
	WS_AMAGOI,
	WS_REFLEC,
	WS_YADORIGI,
	WS_DAMAGE_CHG,
	WS_NEMURU,
	WS_NEKODAMASHI,
	WS_SAWAGU,
	WS_SAWAGU_CHECK,
	WS_TAKUWAERU,
	WS_HAKIDASU,
	WS_NOMIKOMU,
	WS_SUITORI,
	WS_CONDCHG,
	WS_KUROIKIRI,
	WS_GAMAN,
	WS_ABARERU,
	WS_RENZOKU,
	WS_RENZOKU_NUM_INIT,
	WS_HUKITOBASHI,
	WS_TEXTURE,
	WS_KOBAN_CHECK,
	WS_HIKARINOKABE,
	WS_ICHIGEKI,
	WS_HPHALF,
	WS_SUNAARASHI,
	WS_WEATHER_KEEP,
	WS_MEROMERO,
	WS_CONDITION_GAUGE,
	WS_SHIROIKIRI,
	WS_KIAIDAME,
	WS_HENSHIN,
	WS_MIGAWARI,
	WS_MONOMANE,
	WS_YUBIWOHURU,
	WS_CHIKYUUNAGE,
	WS_PSYCHOWAVE,
	WS_COUNTER,
	WS_MIRRORCOAT,
	WS_KANASHIBARI,
	WS_ENCORE,
	WS_ITAMIWAKE,
	WS_TEXTURE2,
	WS_LOCKON,
	WS_SKETCH,
	WS_NEGOTO,
	WS_MICHIDURE,
	WS_MICHIDURE_CHECK,
	WS_JITABATA,
	WS_URAMI,
	WS_IYASHINOSUZU,
	WS_NOROI,
	WS_MAKIBISHI,
	WS_MIYABURU,
	WS_HOROBINOUTA,
	WS_KOROGARU,
	WS_IBARU,
	WS_RENZOKUGIRI,
	WS_NATSUKI,
	WS_PRESENT,
	WS_SHINPINOMAMORI,
	WS_MAGNITUDE,
	WS_OIUCHI_CHECK,
	WS_NIHONBARE,
	WS_HARADAIKO,
	WS_JIKOANJI,
	WS_KOUSOKUSPIN,
	WS_MARUKUNARU,
	WS_WEATHERKAIFUKU,
	WS_MEZAMERUPOWER,
	WS_JISIN,
	WS_MIRAIYOCHI,
	WS_HUKURODATAKI,
	WS_KIERUTAME,
	WS_KIERUTAME_AFTAR,
	WS_CHIISAKUNARU,
	WS_ARARE,
	WS_OKIMIYAGE,
	WS_KONOYUBITOMARE,
	WS_JUUDEN,
	WS_SHIZENNOCHIKARA,
	WS_REFLESH,
	WS_ICHAMON,
	WS_KIAIPUNCH,
	WS_CHOUHATSU,
	WS_TEDASUKE,
	WS_TRICK,
	WS_NARIKIRI,
	WS_NEGAIGOTO,
	WS_NEWOHARU,
	WS_REVENGE,
	WS_AKUBI,
	WS_GAMUSYARA,
	WS_HUNKA,
	WS_SKILLSWAP,
	WS_HUUIN,
	WS_ONNEN,
	WS_KETAGURI,
	WS_NEKONOTE,
	WS_MAGICCORT,
	WS_YOKODORI,
	WS_IKAKU,
	WS_SIZENKAIHUKU,
	WS_HP_CHECK,
	WS_HIMITUNOTIKARA,
	WS_MONOHIROI,
	WS_TENKIYA,
	WS_TENKIYA_CHECK,
	WS_ASOBI,
	WS_WEATHER_BALL,
	WS_RECYCLE,
	WS_HOGOSYOKU,
	WS_HUKITOBASHI_OIUCHI,
	WS_YOKODORI_CLIENT_SET,
	WS_KAWARAWARI,

//�A�C�e���V�[�P���X
	WS_BALL_THROW,
	WS_ADD_POKE,
	WS_ZUKAN_CHECK,
	WS_ZUKAN,
	WS_NICKNAME,
	WS_ITEM_HP_KAIFUKU,
	WS_ITEM_CONDITION_RECOVER,
	WS_ITEMEND,
	WS_GETEND,
};

void	SCA_WazaSequence(void)
{
	if(ClientBit==0)
		WazaSequenceTbl[*WazaSeqAdrs]();
}

u8	ServerDefenceClientGet(u16 wazano,u8 range)
{
	u8	dir;
	u8	clientno=0;
	u8	attackrange;

	if(range)
		attackrange=range-1;
	else
		attackrange=WTD[wazano].attackrange;

	switch(attackrange){
		case RANGE_NORMAL:		//�����ȊO��3��
			dir=MineEnemyCheck(AttackClient)^1;
			if((SCC[dir].konoyubitomareflag)&&(PSP[SCC[dir].konoyubitomareno].hp)){
				clientno=SCC[dir].konoyubitomareno;
				break;
			}
			dir=MineEnemyCheck(AttackClient);
			do{
				clientno=pp_rand()%ClientSetMax;
			}while((clientno==AttackClient)||
				   (dir==MineEnemyCheck(clientno))||
			       (NoReshuffleClient&no2bittbl[clientno]));
			if((WTD[wazano].wazatype==ELECTRIC_TYPE)&&
			   (SpecialAbirityCheck(SPEABI_ENEMYHAVE_SPEABI_CNT,
									AttackClient,TOKUSYU_HIRAISIN,0,0))){
				if(PSP[clientno].speabino!=TOKUSYU_HIRAISIN){
					clientno^=2;
					TokusyuNoSet(clientno,PSP[clientno].speabino);
					OSTF[clientno].hiraishinflag=1;
				}
			}
			break;
		case RANGE_DOUBLE:		//�Q�̂ɓ�����
		case RANGE_TRIPLE:		//�����ȊO3�̂ɓ�����
		case RANGE_FIELD:		//��ɂ�����
		case RANGE_NONE:		//�I���Ȃ�
			clientno=ClientNoGet((ClientTypeGet(AttackClient)&1)^1);
			if(NoReshuffleClient&no2bittbl[clientno])
				clientno^=2;
			break;
		case RANGE_RANDOM:		//�����_��
			dir=MineEnemyCheck(AttackClient)^1;
			if((SCC[dir].konoyubitomareflag)&&(PSP[SCC[dir].konoyubitomareno].hp)){
				clientno=SCC[dir].konoyubitomareno;
				break;
			}
			if((FightType&FIGHT_TYPE_2vs2)&&
				(attackrange&RANGE_RANDOM)){
				if(MineEnemyCheck(AttackClient)==SIDE_MINE){
					if(pp_rand()&1)
						clientno=ClientNoGet(BSS_CLIENT_ENEMY);
					else
						clientno=ClientNoGet(BSS_CLIENT_ENEMY2);
				}
				else{
					if(pp_rand()&1)
						clientno=ClientNoGet(BSS_CLIENT_MINE);
					else
						clientno=ClientNoGet(BSS_CLIENT_MINE2);
				}
				if(NoReshuffleClient&no2bittbl[clientno])
					clientno^=2;
			}
			else
				clientno=ClientNoGet((ClientTypeGet(AttackClient)&1)^1);
			break;
		case RANGE_ALWAYS:		//�������܂߂�4��(1��1�̎����I��L��j
		case RANGE_MINE:		//�����I��
			clientno=AttackClient;
			break;
	}
	UserWork[DEFENCE_CLIENT+AttackClient]=clientno;
	return clientno;
}

u8		BadgeCheck(void)
{
	int	i,j;
	u8	level=0;

	if(FightType&FIGHT_TYPE_TUUSHIN)
		return 0;
	if(MineEnemyCheck(AttackClient)==SIDE_ENEMY)
		return 0;

	if((PokeOneSelfCheckAct(PSP[AttackClient].id_no,(u8 *)&PSP[AttackClient].oyaname))==0)
		return 0;

#ifdef PM_DEBUG
	if(DebugFightFlag&DEBUG_FIGHT_NO_BADGE)
		level=10;
	else{
		if(EventFlagCheck(SYS_BATCH08_GET))
			return 0;

		level=10;
		if(EventFlagCheck(SYS_BATCH02_GET))
			level=30;
		if(EventFlagCheck(SYS_BATCH04_GET))
			level=50;
		if(EventFlagCheck(SYS_BATCH06_GET))
			level=70;
	}
#else
	if(EventFlagCheck(SYS_BATCH08_GET))
		return 0;

	level=10;
	if(EventFlagCheck(SYS_BATCH02_GET))
		level=30;
	if(EventFlagCheck(SYS_BATCH04_GET))
		level=50;
	if(EventFlagCheck(SYS_BATCH06_GET))
		level=70;
#endif

	if(PSP[AttackClient].level<=level)
		return 0;

	i=((pp_rand()&0xff)*(PSP[AttackClient].level+level))>>8;

	if(i<level) return 0;

	//�������I�����Ă����Ȃ�A�{���e�[�W�������Ȃ��悤�ɂ���
	if(WazaNo==WAZANO_IKARI)
		PSP[AttackClient].condition2&=CONDITION2_IKARI_OFF;

	if((PSP[AttackClient].condition&CONDITION_NEMURI)&&
	  ((WazaNo==WAZANO_IBIKI)||(WazaNo==WAZANO_NEGOTO))){
		WazaSeqAdrs=(u8 *)&WAZA_OrderIgnoreSleep;	//���ߖ����i�˂ނ�j
		return 1;
	}

	i=((pp_rand()&0xff)*(PSP[AttackClient].level+level))>>8;

	if(i<level){
		i=WaruagakiWazaNoCheck(AttackClient,no2bittbl[WazaPosNo],WWNC_ALL);
		if(i==0x0f){		//�o����Z���Ȃ�
			ServerWork[5]=pp_rand()&3;
			WazaSeqAdrs=(u8 *)&WAZA_Namakeru;		//���ߖ����i�Ȃ܂���j
			return 1;
		}
		do{
			WazaPosNo=WazaPosNoTmp=pp_rand()&3;
		}while(i&no2bittbl[WazaPosNo]);
		PushWazaNo=PSP[AttackClient].waza[WazaPosNo];
		WazaSeqAdrs=(u8 *)&WAZA_OrderIgnore;		//���ߖ���
		DefenceClient=ServerDefenceClientGet(PushWazaNo,0);
		ServerStatusFlag|=SERVER_STATUS_FLAG_BADGE_CHECKED;
		return 2;
	}

	level=PSP[AttackClient].level-level;
	i=pp_rand()&0xff;

	if(((i<level)&&((PSP[AttackClient].condition&CONDITION_BAD)==0))&&
	    (PSP[AttackClient].speabino!=TOKUSYU_YARUKI)&&
	    (PSP[AttackClient].speabino!=TOKUSYU_HUMIN)){
		for(j=0;j<ClientSetMax;j++){
			if(PSP[j].condition2&CONDITION2_SAWAGU)
				break;
		}
		if(j==ClientSetMax){
			WazaSeqAdrs=(u8 *)&WAZA_Hirune;				//���ߖ����i�Ђ�ˁj
			return 1;
		}
	}

	i-=level;

	if(i<level){
		WazaDamage=WazaDamageCalc(
				   &PSP[AttackClient],
				   &PSP[AttackClient],
				   WAZANO_HATAKU,
				   0,
				   40,
				   0,
				   AttackClient,
				   AttackClient);
		DefenceClient=AttackClient;
		WazaSeqAdrs=(u8 *)&WAZA_OwnAttack;			//���ߖ����i�������U���j
		//2�̂ɂ�����Z���o���Ă���Ƃ���
		//DOUBLECHECK�������Ȃ����߂̏���
		ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
		return 2;
	}

	ServerWork[5]=pp_rand()&3;
	WazaSeqAdrs=(u8 *)&WAZA_Namakeru;			//���ߖ����i�Ȃ܂���j
	return 1;
}

void	WS_BADGECHECK(void)
{
	int	i;

	//�U���Œ��ɏ��s�������Ă�����A�����ɃV�[�P���X���I��炷
	if(WinLoseFlag!=0){
		ActionSeqNo=SCA_EndNo;
		return;
	}

	//�U���Œ���Attack�����m���ɂȂ�����
	//WAZA_WazaEnd�ɂƂ΂�
	if((PSP[AttackClient].hp==0)&&
	  ((ServerStatusFlag&SERVER_STATUS_FLAG_DOUBLE_CHECK)==0)){
		ServerStatusFlag|=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK;
		WazaSeqAdrs=(u8 *)&WAZA_WazaEnd;
		return;
	}

	if(PokeConditionCheck()) return;

	if(SpecialAbirityCheck(SPEABI_WAZA_HIT_BEFORE,DefenceClient,0,0,0)) return;

	if((PSP[AttackClient].pp[WazaPosNo]==0)&&
	   (WazaNo!=WAZANO_WARUAGAKI)&&
	  ((ServerStatusFlag&(SERVER_STATUS_FLAG_NOPP_CHECK|SERVER_STATUS_FLAG_DOUBLE_CHECK))==0)&&
	  ((PSP[AttackClient].condition2&CONDITION2_KEEP)==0)){
//		PSP[AttackClient].condition2&=CONDITION2_KEEP_OFF;	//PP���Ȃ��Ȃ����Z�ł��ߌn�͉���
		WazaSeqAdrs=(u8 *)&WAZA_PPNoneAttack;
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		return;
	}

	ServerStatusFlag&=SERVER_STATUS_FLAG_NOPP_CHECK_OFF;

#if 0
	if(ServerStatusFlag&SERVER_STATUS_FLAG_BADGE_CHECKED)
		ServerStatusFlag&=SERVER_STATUS_FLAG_BADGE_CHECKED_OFF;
	else{
		if((i=BadgeCheck())!=0){
			if(i==2)
				ServerStatusFlag|=SERVER_STATUS_FLAG_WAZA_OUT;
			else
				WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
			return;
		}
	}
#endif

	if(((ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT)==0)&&
	   ((PSP[AttackClient].condition2&CONDITION2_KEEP)==0)){
		if((i=BadgeCheck())!=0){
			if(i==2)
				ServerStatusFlag|=SERVER_STATUS_FLAG_WAZA_OUT;
			else
				WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
			return;
		}

	}
	ServerStatusFlag|=SERVER_STATUS_FLAG_WAZA_OUT;

	if((OTF[DefenceClient].magiccortflag)&&
	   (WTD[WazaNo].flag&FLAG_MAGICCORT)){		//�}�W�b�N�R�[�g�`�F�b�N
		//�����v���b�V���[�ɂ��PPDEC���v�Z
		PressurePPDecCheck(AttackClient,DefenceClient,WAZANO_MAZIKKUKOOTO);
		OTF[DefenceClient].magiccortflag=0;
		PushAdrsAct();
		WazaSeqAdrs=(u8 *)&WAZA_MagicCort;
		return;
	};
	for(i=0;i<ClientSetMax;i++){
		if((OTF[ActionClientNo[i]].yokodoriflag)&&
		   (WTD[WazaNo].flag&FLAG_YOKODORI)){		//�悱�ǂ�`�F�b�N
			//�����v���b�V���[�ɂ��PPDEC���v�Z
			PressurePPDecCheck(AttackClient,ActionClientNo[i],WAZANO_YOKODORI);
			OTF[ActionClientNo[i]].yokodoriflag=0;
			UserWork[CLIENT_NO_WORK]=ActionClientNo[i];
			PushAdrsAct();
			WazaSeqAdrs=(u8 *)&WAZA_Yokodori;
			return;
		};
	}
	if(OSTF[DefenceClient].hiraishinflag){
		OSTF[DefenceClient].hiraishinflag=0;
		SpeabiNo=TOKUSYU_HIRAISIN;
		PushAdrsAct();
		WazaSeqAdrs=(u8 *)&WAZA_Hiraishin;
		TokusyuNoSet(DefenceClient,SpeabiNo);
		return;
	}
	if((OTF[DefenceClient].guardflag)&&
	   (WTD[WazaNo].flag&FLAG_MAMORU)){
		if((WazaNo==WAZANO_NOROI)&&
		   (PSP[AttackClient].type1!=GHOST_TYPE)&&
		   (PSP[AttackClient].type2!=GHOST_TYPE)){
			WazaSeqAdrs++;
			return;
		}
		if((TameWazaCheck(WazaNo)==0)||(PSP[AttackClient].condition2&CONDITION2_KEEP)){
//			PSP[AttackClient].condition2&=CONDITION2_KEEP_OFF;
			ServerKeepOff(AttackClient);
			WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
			WazaNoHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
			WazaTypeHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
			ServerWork[6]=MSG_MAMORUNOHIT;
			WazaSeqAdrs++;
			return;
		}
	}

	WazaSeqAdrs++;
}

void	WazaSeqSet(u8 ofs,u16 wazano)
{
	u8	*adr;

	adr=WazaSeqAdrs+ofs;
	if(WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL){
		WazaNoHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
		WazaTypeHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
		adr=(u8 *)(WazaSeqAdrs[1]<< 0|
				   WazaSeqAdrs[2]<< 8|
				   WazaSeqAdrs[3]<<16|
				   WazaSeqAdrs[4]<<24);
	}
	else{
		//�݂��Â�`�F�b�N
		MichidureCheckAct();
		if(SpecialAbirityCheck(SPEABI_WAZA_HIT,DefenceClient,0,0,wazano))
			return;
	}
	WazaSeqAdrs=adr;
}

void	WS_GUARDCHECK()
{
	if((OTF[DefenceClient].guardflag)&&
	   (WTD[WazaNo].flag&FLAG_MAMORU)){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		WazaSeqSet(5,0);
		ServerWork[6]=MSG_MAMORUNOHIT;
	}
	else
		WazaSeqAdrs+=5;
}

u8	GuardCheck(u16 wazano)
{
	u8	ret=0;
	if((OTF[DefenceClient].guardflag)&&
	   (WTD[WazaNo].flag&FLAG_MAMORU)){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		WazaSeqSet(7,wazano);
		ServerWork[6]=MSG_MAMORUNOHIT;
		ret=1;
	}
	return ret;
}

u8	WazaFieldConditionCheck(u16 wazano)
{
	if((WazaKouka[DefenceClient]&WAZAKOUKA_LOCKON)&&
	   (WKC[DefenceClient].lockonclientno==AttackClient)){
		WazaSeqSet(7,wazano);
		return 1;
	}

	if(((ServerStatusFlag&SERVER_STATUS_FLAG_SORAWOTOBUHIT)==0)&&
	    (WazaKouka[DefenceClient]&WAZAKOUKA_SORAWOTOBU)){
			WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
			WazaSeqSet(7,wazano);
			return 1;
	}
	else
		ServerStatusFlag&=SERVER_STATUS_FLAG_SORAWOTOBUHIT_OFF;

	if(((ServerStatusFlag&SERVER_STATUS_FLAG_ANAWOHORUHIT)==0)&&
	    (WazaKouka[DefenceClient]&WAZAKOUKA_ANAWOHORU)){
			WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
			WazaSeqSet(7,wazano);
			return 1;
	}
	else
		ServerStatusFlag&=SERVER_STATUS_FLAG_ANAWOHORUHIT_OFF;

	if(((ServerStatusFlag&SERVER_STATUS_FLAG_DAIBINGUHIT)==0)&&
	    (WazaKouka[DefenceClient]&WAZAKOUKA_DAIBINGU)){
			WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
			WazaSeqSet(7,wazano);
			return 1;
	}
	else
		ServerStatusFlag&=SERVER_STATUS_FLAG_DAIBINGUHIT_OFF;

	if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_NOOTENKI,0,0)==0)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_EAROKKU,0,0)==0)){
		if(FieldCondition&FIELD_CONDITION_AMEHURI){
//			if(WTD[wazano].wazatype==ELECTRIC_TYPE){
			if(WTD[wazano].battleeffect==152){
				WazaSeqSet(7,wazano);
				return 1;
			}
		}
	}

	//�K�������鏈��
	if((WTD[wazano].battleeffect==17)||
	   (WTD[wazano].battleeffect==78)){
		WazaSeqSet(7,wazano);
		return 1;
	}
	return	0;
}

typedef struct{
	u8	kakeru;
	u8	waru;
}HitProbabilityTable;

const	HitProbabilityTable	HPT[]={
	{
	33,100			// 0.33
	},
	{
	36,100			//  0.36
	},
	{
	43,100			//  0.43
	},
	{
	50,100			//  0.50
	},
	{
	60,100			//  0.60
	},
	{
	75,100			//  0.75
	},
	{
	1,1				//  1.00
	},
	{
	133,100			//  1.33
	},
	{
	166,100			//  1.66
	},
	{
	2,1				//  2.00
	},
	{
	233,100			//  2.33
	},
	{
	133,50			//  2.66
	},
	{
	3,1				//  3.00
	},
};

void	WS_HITCHECK(void)
{
	u16		work;
	s8		value;
	u8		eqp;
	u8		atc;
	u16		wazano;
	u8		wazatype;
	u16		hit;

	wazano=WazaSeqAdrs[5]|(WazaSeqAdrs[6]<<8);

	if((wazano==HITCHECK_KIE)||(wazano==HITCHECK_GUARD)){
		if((WazaKouka[DefenceClient]&WAZAKOUKA_LOCKON)&&
		   (wazano==HITCHECK_KIE)&&
		   (WKC[DefenceClient].lockonclientno==AttackClient))
			WazaSeqAdrs+=7;
		else if(WazaKouka[DefenceClient]&WAZAKOUKA_KIE)
			WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
							   WazaSeqAdrs[2]<< 8|
							   WazaSeqAdrs[3]<<16|
							   WazaSeqAdrs[4]<<24);
		else if(GuardCheck(0))		//�܂���A�݂���`�F�b�N
			return;
		else
			WazaSeqAdrs+=7;
		return;
	}

	if(wazano==0)
		wazano=WazaNo;

	if(UserWork[DAMAGE_TYPE_WORK])
		wazatype=UserWork[DAMAGE_TYPE_WORK]&0x3f;
	else
		wazatype=WTD[wazano].wazatype;

	if(GuardCheck(wazano)) return;		//�܂���A�݂���`�F�b�N
	if(WazaFieldConditionCheck(wazano)) return;

	if(PSP[DefenceClient].condition2&CONDITION2_MIYABURU)
		value=6+(PSP[AttackClient].abiritycnt[COND_HIT]-6);
	else
		value=6+(6-PSP[DefenceClient].abiritycnt[COND_AVOID])
			   +(PSP[AttackClient].abiritycnt[COND_HIT]-6);

	if(value<0)
		value=0;

	if(value>12)
		value=12;

	hit=WTD[wazano].hitprobability;
	//����\�͂̂��Ă񂫂́A�`�F�b�N�����Ȃ�
	if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_NOOTENKI,0,0)==0)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_EAROKKU,0,0)==0)){
		//����̂Ƃ��A���݂Ȃ�͖�������50%�ɂ���
		if((FieldCondition&FIELD_CONDITION_HIDERING)&&(WTD[wazano].battleeffect==152))
			hit=50;
	}

	work=hit*HPT[value].kakeru;
	work=work/HPT[value].waru;

	if(PSP[AttackClient].speabino==TOKUSYU_HUKUGAN)
		work=work*130/100;	//����\�͂ӂ�����́A���������R�O���A�b�v

	//����\�͂̂��Ă񂫂́A�`�F�b�N�����Ȃ�
	if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_NOOTENKI,0,0)==0)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_EAROKKU,0,0)==0)){
		if((PSP[DefenceClient].speabino==TOKUSYU_SUNAGAKURE)&&
		   (FieldCondition&FIELD_CONDITION_SUNAARASHIING))
			work=work*80/100;		//����\�͂��Ȃ�����́A���������Q�O���_�E���i���Ȃ��炵�̎��Ɂj
	}

	if((PSP[AttackClient].speabino==TOKUSYU_HARIKIRI)&&
	   (wazatype<HATE_TYPE))
		work=work*80/100;		//����\�͂͂肫��́A���������Q�O���_�E��

	if(PSP[DefenceClient].item==ITEM_NAZONOMI){
//		eqp=Fld.RamSeed.item_equip;
//		atc=Fld.RamSeed.item_power;
		eqp=FRS[DefenceClient].item_equip;
		atc=FRS[DefenceClient].item_power;
	}
	else{
		eqp=ItemEquipGet(PSP[DefenceClient].item);
		atc=ItemAttackGet(PSP[DefenceClient].item);
	}

	ItemClient=DefenceClient;

	//�A�C�e���Ŗ������_�E���̌��ʂ̂�����̂́A���������_�E��
	if(eqp==SOUBI_MEITYUURITUDOWN)
		work=work*(100-atc)/100;

	if((pp_rand()%100)+1>work){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		if((FightType&FIGHT_TYPE_2vs2)&&
		  ((WTD[wazano].attackrange==RANGE_DOUBLE)||
		   (WTD[wazano].attackrange==RANGE_TRIPLE)))
			ServerWork[6]=MSG_DOUBLENOHIT;
		else
			ServerWork[6]=MSG_NOHIT;
		WazaNoHitTypeCheck();
	}
	WazaSeqSet(7,wazano);
}

void	WS_ATTACK_MESSAGE(void)
{
	if(ClientBit==0){
		if((ServerStatusFlag&
		   (SERVER_STATUS_FLAG_NOATTACK_MES|SERVER_STATUS_FLAG_DOUBLE_CHECK))==0){
			FightMsgServerSet(ATTACKMSGNO,AttackClient);
			ServerStatusFlag|=SERVER_STATUS_FLAG_NOATTACK_MES;
		}
		WazaSeqAdrs++;
		ServerWork[7]=0;				//���b�Z�[�W�E�F�C�g�𖳌��ɂ���
	}
}

void	WS_PP_DEC(void)
{
	int	decpp=1;

	if(ClientBit==0){
//����\�̓v���b�V���[��PP���{����
		if(OSTF[AttackClient].nopressureflag==0){
			switch(WTD[WazaNo].attackrange){
				case RANGE_TRIPLE:
					decpp+=SpecialAbirityCheck(SPEABI_HAVE_SPEABI_NOMINE_CNT,
											   AttackClient,TOKUSYU_PURESSYAA,0,0);
					break;
				case RANGE_DOUBLE:
				case RANGE_FIELD:
					decpp+=SpecialAbirityCheck(SPEABI_ENEMYHAVE_SPEABI_CNT,
											   AttackClient,TOKUSYU_PURESSYAA,0,0);
					break;
				default:
					if((AttackClient!=DefenceClient)&&
					   (PSP[DefenceClient].speabino==TOKUSYU_PURESSYAA))
						decpp++;
					break;
			}
		}

		if(((ServerStatusFlag&
			(SERVER_STATUS_FLAG_NOPP_DEC|SERVER_STATUS_FLAG_DOUBLE_CHECK))==0)&&
			(PSP[AttackClient].pp[WazaPosNo]!=0)){
			OTF[AttackClient].ppdecflag=1;
			if(PSP[AttackClient].pp[WazaPosNo]>decpp)
				PSP[AttackClient].pp[WazaPosNo]-=decpp;
			else
				PSP[AttackClient].pp[WazaPosNo]=0;

			if(((PSP[AttackClient].condition2&CONDITION2_HENSHIN)==0)&&
			   ((WKC[AttackClient].monomanebit&no2bittbl[WazaPosNo])==0)){
				ClientNo=AttackClient;
				BSS_POKEPARA_PUT_SET(BSS_SERVER,
								 	 BSS_PARA_PP1+WazaPosNo,
									 BSS_SELMONSNO,
									 1,
			 						 (u8 *)&PSP[AttackClient].pp[WazaPosNo]);
				ClientBitSet(AttackClient);
			}
		}
		ServerStatusFlag&=SERVER_STATUS_FLAG_NOPP_DEC_OFF;
		WazaSeqAdrs++;
	}
}

const	u16	CriticalTable[]={
	16,8,4,3,2
};

void	WS_CRITICAL_CHECK(void)
{
	u16		work;
	u16		itemno;
	u8		eqp;

	itemno=PSP[AttackClient].item;
	if(itemno==ITEM_NAZONOMI)
//		eqp=Fld.RamSeed.item_equip;
		eqp=FRS[AttackClient].item_equip;
	else
		eqp=ItemEquipGet(itemno);

	ItemClient=AttackClient;

	work=(((PSP[AttackClient].condition2&CONDITION2_KIAIDAME)!=0)*2)+
		 (WTD[WazaNo].battleeffect==43)+
		 (WTD[WazaNo].battleeffect==75)+
	     (WTD[WazaNo].battleeffect==200)+
	     (WTD[WazaNo].battleeffect==209)+
		 (eqp==SOUBI_KYUUSYONIATARIYASUI)+
		 (2*((eqp==SOUBI_RAKKIIDATOKYUUSYONIATARIYASUI)&&
			 (PSP[AttackClient].monsno==MONSNO_RAKKII)))+
		 (2*((eqp==SOUBI_KAMONEGIKYUUSYOATARIYASUI)&&
			 (PSP[AttackClient].monsno==MONSNO_KAMONEGI)));
	if(work>4) work=4;

//����\�͂ŃJ�u�g�A�[�}�[������Ƃ��͋}���ɂ�����Ȃ�
	if((PSP[DefenceClient].speabino!=TOKUSYU_KABUTOAAMAA)&&
//����\�͂ŃV�F���A�[�}�[������Ƃ��͋}���ɂ�����Ȃ�
	   (PSP[DefenceClient].speabino!=TOKUSYU_SHERUAAMAA)&&
//�K�C�h�o�g���p
	  ((WazaKouka[AttackClient]&WAZAKOUKA_NOCRITICAL)==0)&&
	  ((FightType&FIGHT_TYPE_GUIDE)==0)&&
	  ((FightType&FIGHT_TYPE_GET_DEMO)==0)){
		if(pp_rand()%CriticalTable[work]==0){
			CriticalValue=2;
		}
		else
			CriticalValue=1;
	}
	else
		CriticalValue=1;

	WazaSeqAdrs++;
}

void	WS_DAMAGE_CALC(void)
{
	WazaDamage=WazaDamageCalc(&PSP[AttackClient],&PSP[DefenceClient],WazaNo,SideCondition[ClientTypeGet(DefenceClient)&1],DamagePowWork,UserWork[DAMAGE_TYPE_WORK],AttackClient,DefenceClient);
//	DamagePowWork=0;
	WazaDamage*=CriticalValue;
	WazaDamage*=UserWork[DAMAGE_VALUE];

	//�[�d���͗������̓_���[�W�Q�{
	if((WazaKouka[AttackClient]&WAZAKOUKA_JUUDEN)&&
	   (WTD[WazaNo].wazatype==ELECTRIC_TYPE))
		WazaDamage*=2;

	//�Ă������̓_���[�W�P�D�T�{
	if(OTF[AttackClient].tedasukeflag)
		WazaDamage=WazaDamage*15/10;

	WazaSeqAdrs++;
}

void	DamageCalcAct(u8 attack,u8 defence)
{
	WazaDamage=WazaDamageCalc(&PSP[attack],&PSP[defence],WazaNo,SideCondition[ClientTypeGet(defence)&1],DamagePowWork,UserWork[DAMAGE_TYPE_WORK],attack,defence);
	DamagePowWork=0;
	WazaDamage*=CriticalValue;
	WazaDamage*=UserWork[DAMAGE_VALUE];

	//�[�d���͗������̓_���[�W�Q�{
	if((WazaKouka[attack]&WAZAKOUKA_JUUDEN)&&
	   (WTD[WazaNo].wazatype==ELECTRIC_TYPE))
		WazaDamage*=2;

	//�Ă������̓_���[�W�P�D�T�{
	if(OTF[attack].tedasukeflag)
		WazaDamage=WazaDamage*15/10;
}

#define	KOUKANAI	0
#define	IMAHITOTSU	5
#define	BATSUGUN	20
void	TypeCheckCalc(u8 work)
{
	WazaDamage=(WazaDamage*work)/10;
	if((WazaDamage==0)&&(work!=0)) WazaDamage=1;
	switch(work){
		case KOUKANAI:
			WazaStatusFlag|=WAZASTATUSFLAG_KOUKANAI;
			WazaStatusFlag&=WAZASTATUSFLAG_IMAHITOTSU_OFF;
			WazaStatusFlag&=WAZASTATUSFLAG_BATSUGUN_OFF;
			break;
		case IMAHITOTSU:
			if((WTD[WazaNo].damage)&&
			  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
				if(WazaStatusFlag&WAZASTATUSFLAG_BATSUGUN)
					WazaStatusFlag&=WAZASTATUSFLAG_BATSUGUN_OFF;
				else
					WazaStatusFlag|=WAZASTATUSFLAG_IMAHITOTSU;
			}
			break;
		case BATSUGUN:
			if((WTD[WazaNo].damage)&&
			  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
				if(WazaStatusFlag&WAZASTATUSFLAG_IMAHITOTSU)
					WazaStatusFlag&=WAZASTATUSFLAG_IMAHITOTSU_OFF;
				else
					WazaStatusFlag|=WAZASTATUSFLAG_BATSUGUN;
			}
			break;
	}
}

void	WS_TYPE_CHECK(void)
{
	int	i;
	u8	wazatype;
	
	i=0;

	//��邠�����́A�������Ȃ̂�TypeCheck���Ȃ�
	if(WazaNo==WAZANO_WARUAGAKI){
		WazaSeqAdrs++;
		return;
	}

	if(UserWork[DAMAGE_TYPE_WORK])
		wazatype=UserWork[DAMAGE_TYPE_WORK]&0x3f;
	else
		wazatype=WTD[WazaNo].wazatype;

	if((PSP[AttackClient].type1==wazatype)||
	   (PSP[AttackClient].type2==wazatype)){
		WazaDamage=WazaDamage*15;
		WazaDamage=WazaDamage/10;
	}

	//������ʂӂ䂤�ɂ͂��߂�^�C�v�̋Z�͓�����Ȃ�
	if((PSP[DefenceClient].speabino==TOKUSYU_HUYUU)&&
	   (wazatype==JIMEN_TYPE)){
		SpeabiNo=TOKUSYU_HUYUU;
		WazaStatusFlag|=(WAZASTATUSFLAG_KOUKANAI|WAZASTATUSFLAG_NOHIT);
		WazaNoHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
		WazaTypeHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
		ServerWork[6]=MSG_HUYUUHIT;
		TokusyuNoSet(DefenceClient,SpeabiNo);
	}
	else{
		while(TypeCheckTable[i]!=0xff){
			if(TypeCheckTable[i]==0xfe){		//�݂�Ԃ�`�F�b�N
				if(PSP[DefenceClient].condition2&CONDITION2_MIYABURU)
					break;
				else{
					i+=3;
					continue;
				}
			}
			if(TypeCheckTable[i]==wazatype){
				if(TypeCheckTable[i+1]==PSP[DefenceClient].type1){
					TypeCheckCalc(TypeCheckTable[i+2]);
				}
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type2)&&
				   (PSP[DefenceClient].type1!=PSP[DefenceClient].type2)){
					TypeCheckCalc(TypeCheckTable[i+2]);
				}
			}
			i+=3;
		}
	}

	//����\�͂ӂ����Ȃ܂���́A���ʂ����Q����������Ȃ�
	if((PSP[DefenceClient].speabino==TOKUSYU_HUSIGINAMAMORI)&&
	   (TameWazaTurnCheck(AttackClient,WazaNo)==2)&&
	 (((WazaStatusFlag&WAZASTATUSFLAG_BATSUGUN)==0)||
	  ((WazaStatusFlag&WAZASTATUSFLAG_SOUSAI)==WAZASTATUSFLAG_SOUSAI))&&
	   (WTD[WazaNo].damage)){
		SpeabiNo=TOKUSYU_HUSIGINAMAMORI;
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		WazaNoHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
		WazaTypeHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
		ServerWork[6]=MSG_HUSIGINAMAMORIHIT;
		TokusyuNoSet(DefenceClient,SpeabiNo);
	}
	if(WazaStatusFlag&WAZASTATUSFLAG_KOUKANAI)
		OTF[AttackClient].koukanaiflag=1;

	WazaSeqAdrs++;
}

//�Z��������Ȃ������Ƃ���TYPE_CHECK
void	WazaNoHitTypeCheck(void)
{
	int	i;
	u8	wazatype;
	u8	flag=0;
	
	i=0;

	//��邠�����́A�������Ȃ̂�TypeCheck���Ȃ�
	if(WazaNo==WAZANO_WARUAGAKI)
		return;

	if(WTD[WazaNo].damage==0)
		return;

	if(UserWork[DAMAGE_TYPE_WORK])
		wazatype=UserWork[DAMAGE_TYPE_WORK]&0x3f;
	else
		wazatype=WTD[WazaNo].wazatype;

	//������ʂӂ䂤�ɂ͂��߂�^�C�v�̋Z�͓�����Ȃ�
	if((PSP[DefenceClient].speabino==TOKUSYU_HUYUU)&&
	   (wazatype==JIMEN_TYPE)){
		SpeabiNo=TOKUSYU_HUYUU;
		ServerWork[6]=MSG_HUYUUHIT;
		TokusyuNoSet(DefenceClient,SpeabiNo);
		return;
	}
	else{
		while(TypeCheckTable[i]!=0xff){
			if(TypeCheckTable[i]==0xfe){		//�݂�Ԃ�`�F�b�N
				if(PSP[DefenceClient].condition2&CONDITION2_MIYABURU)
					break;
				else{
					i+=3;
					continue;
				}
			}
			if(TypeCheckTable[i]==wazatype){
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type1)&&
				   (TypeCheckTable[i+2]==KOUKANAI)){
					WazaStatusFlag|=WAZASTATUSFLAG_KOUKANAI;
					OTF[AttackClient].koukanaiflag=1;
				}
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type2)&&
				   (PSP[DefenceClient].type1!=PSP[DefenceClient].type2)&&
				   (TypeCheckTable[i+2]==KOUKANAI)){
					WazaStatusFlag|=WAZASTATUSFLAG_KOUKANAI;
					OTF[AttackClient].koukanaiflag=1;
				}
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type1)&&
				   (TypeCheckTable[i+2]==BATSUGUN))
					flag|=1;
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type2)&&
				   (PSP[DefenceClient].type1!=PSP[DefenceClient].type2)&&
				   (TypeCheckTable[i+2]==BATSUGUN))
					flag|=1;
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type1)&&
				   (TypeCheckTable[i+2]==IMAHITOTSU))
					flag|=2;
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type2)&&
				   (PSP[DefenceClient].type1!=PSP[DefenceClient].type2)&&
				   (TypeCheckTable[i+2]==IMAHITOTSU))
					flag|=2;
			}
			i+=3;
		}
	}

	//����\�͂ӂ����Ȃ܂���́A���ʂ����Q����������Ȃ�
	if((PSP[DefenceClient].speabino==TOKUSYU_HUSIGINAMAMORI)&&
	   (TameWazaTurnCheck(AttackClient,WazaNo)==2)&&
	  ((flag&2)||((flag&1)==0))&&
	   (WTD[WazaNo].damage)){
		SpeabiNo=TOKUSYU_HUSIGINAMAMORI;
		ServerWork[6]=MSG_HUSIGINAMAMORIHIT;
		TokusyuNoSet(DefenceClient,SpeabiNo);
	}
	return;
}

void	TypeCheckCalc2(u8 work,u16 wazano,u8 *flag)
{
	WazaDamage=(WazaDamage*work)/10;
	if((WazaDamage==0)&&(work!=0)) WazaDamage=1;
	switch(work){
		case KOUKANAI:
			flag[0]|=WAZASTATUSFLAG_KOUKANAI;
			flag[0]&=WAZASTATUSFLAG_IMAHITOTSU_OFF;
			flag[0]&=WAZASTATUSFLAG_BATSUGUN_OFF;
			break;
		case IMAHITOTSU:
			if((WTD[wazano].damage)&&
			  ((flag[0]&WAZASTATUSFLAG_NOCRITICAL)==0)){
				if(flag[0]&WAZASTATUSFLAG_BATSUGUN)
					flag[0]&=WAZASTATUSFLAG_BATSUGUN_OFF;
				else
					flag[0]|=WAZASTATUSFLAG_IMAHITOTSU;
			}
			break;
		case BATSUGUN:
			if((WTD[wazano].damage)&&
			  ((flag[0]&WAZASTATUSFLAG_NOCRITICAL)==0)){
				if(flag[0]&WAZASTATUSFLAG_IMAHITOTSU)
					flag[0]&=WAZASTATUSFLAG_IMAHITOTSU_OFF;
				else
					flag[0]|=WAZASTATUSFLAG_BATSUGUN;
			}
			break;
	}
}

u8	TypeCheckAct(u16 wazano,u8 attack,u8 defence)
{
	int	i=0;
	u8	wazatype;
	u8	wazakoukaflag=0;
	
	//��邠�����́A�������Ȃ̂�TypeCheck���Ȃ�
	if(wazano==WAZANO_WARUAGAKI)
		return wazakoukaflag;

	wazatype=WTD[wazano].wazatype;

	if((PSP[attack].type1==wazatype)||
	   (PSP[attack].type2==wazatype)){
		WazaDamage=WazaDamage*15;
		WazaDamage=WazaDamage/10;
	}


	//������ʂӂ䂤�ɂ͂��߂�^�C�v�̋Z�͓�����Ȃ�
	if((PSP[defence].speabino==TOKUSYU_HUYUU)&&(wazatype==JIMEN_TYPE))
		wazakoukaflag|=(WAZASTATUSFLAG_KOUKANAI|WAZASTATUSFLAG_NOHIT);
	else{
		while(TypeCheckTable[i]!=0xff){
			if(TypeCheckTable[i]==0xfe){		//�݂�Ԃ�`�F�b�N
				if(PSP[defence].condition2&CONDITION2_MIYABURU)
					break;
				else{
					i+=3;
					continue;
				}
			}
			if(TypeCheckTable[i]==wazatype){
				if(TypeCheckTable[i+1]==PSP[defence].type1){
					TypeCheckCalc2(TypeCheckTable[i+2],wazano,&wazakoukaflag);
				}
				if((TypeCheckTable[i+1]==PSP[defence].type2)&&
				   (PSP[DefenceClient].type1!=PSP[defence].type2)){
					TypeCheckCalc2(TypeCheckTable[i+2],wazano,&wazakoukaflag);
				}
			}
			i+=3;
		}
	}

	//����\�͂ӂ����Ȃ܂���́A���ʂ����Q����������Ȃ�
	if((PSP[defence].speabino==TOKUSYU_HUSIGINAMAMORI)&&
	  ((wazakoukaflag&WAZASTATUSFLAG_NOHIT)==0)&&
	   (TameWazaTurnCheck(attack,wazano)==2)&&
	 (((wazakoukaflag&WAZASTATUSFLAG_BATSUGUN)==0)||
	  ((wazakoukaflag&WAZASTATUSFLAG_SOUSAI)==WAZASTATUSFLAG_SOUSAI))&&
	   (WTD[wazano].damage)){
		wazakoukaflag|=WAZASTATUSFLAG_NOHIT;
	}

	return	wazakoukaflag;
}

u8	TypeCheckAct2(u16 wazano,u16 monsno,u8 speabino)
{
	int	i=0;
	u8	wazatype;
	u8	wazakoukaflag=0;
	u8	type1;
	u8	type2;

	type1=PPD[monsno].type1;
	type2=PPD[monsno].type2;
	
	//��邠�����́A�������Ȃ̂�TypeCheck���Ȃ�
	if(wazano==WAZANO_WARUAGAKI)
		return wazakoukaflag;

	wazatype=WTD[wazano].wazatype;

	//������ʂӂ䂤�ɂ͂��߂�^�C�v�̋Z�͓�����Ȃ�
	if((speabino==TOKUSYU_HUYUU)&&(wazatype==JIMEN_TYPE))
		wazakoukaflag|=(WAZASTATUSFLAG_KOUKANAI|WAZASTATUSFLAG_NOHIT);
	else{
		while(TypeCheckTable[i]!=0xff){
			if(TypeCheckTable[i]==0xfe){	//�݂�Ԃ�`�F�b�N
				i+=3;
				continue;
			}
			if(TypeCheckTable[i]==wazatype){
				if(TypeCheckTable[i+1]==type1){
					TypeCheckCalc2(TypeCheckTable[i+2],wazano,&wazakoukaflag);
				}
				if((TypeCheckTable[i+1]==type2)&&
				   (PSP[DefenceClient].type1!=type2)){
					TypeCheckCalc2(TypeCheckTable[i+2],wazano,&wazakoukaflag);
				}
			}
			i+=3;
		}
	}

	//����\�͂ӂ����Ȃ܂���́A���ʂ����Q����������Ȃ�
	if((speabino==TOKUSYU_HUSIGINAMAMORI)&&
	 (((wazakoukaflag&WAZASTATUSFLAG_BATSUGUN)==0)||
	  ((wazakoukaflag&WAZASTATUSFLAG_SOUSAI)==WAZASTATUSFLAG_SOUSAI))&&
	   (WTD[wazano].damage)){
		wazakoukaflag|=WAZASTATUSFLAG_KOUKANAI;
	}

	return	wazakoukaflag;
}

void	DamageLossAct(void)
{
	u16	work;

	work=100-(pp_rand()%16);
	if(WazaDamage!=0){
		WazaDamage*=work;
		WazaDamage/=100;
		if(WazaDamage==0) WazaDamage=1;
	}
}

void	WS_DAMAGE_LOSS(void)
{
	u16		work;
	u8		eqp;
	u8		atc;

	work=100-(pp_rand()%16);
	if(WazaDamage!=0){
		WazaDamage*=work;
		WazaDamage/=100;
		if(WazaDamage==0) WazaDamage=1;
	}

	if(PSP[DefenceClient].item==ITEM_NAZONOMI){
//		eqp=Fld.RamSeed.item_equip;
//		atc=Fld.RamSeed.item_power;
		eqp=FRS[DefenceClient].item_equip;
		atc=FRS[DefenceClient].item_power;
	}
	else{
		eqp=ItemEquipGet(PSP[DefenceClient].item);
		atc=ItemAttackGet(PSP[DefenceClient].item);
	}

	ItemClient=DefenceClient;

	if((eqp==SOUBI_HPITIDETAERU)&&
	  ((pp_rand()%100)<atc)){
		SoubiItemSet(DefenceClient,eqp);
		OSTF[DefenceClient].itemkoraeruflag=1;
	}

	if((PSP[DefenceClient].condition2&CONDITION2_MIGAWARI)==0){	//�g���肪����ꍇ�̓`�F�b�N���Ȃ�
		if((WTD[WazaNo].battleeffect==101)||		//�݂˂���
		   (OTF[DefenceClient].koraeruflag)||		//���炦��
		   //�A�C�e����HP1�őς���̑������ʂ�HP1�őς���
		   (OSTF[DefenceClient].itemkoraeruflag)){
			if(PSP[DefenceClient].hp<=WazaDamage){
				WazaDamage=PSP[DefenceClient].hp-1;
				if(OTF[DefenceClient].koraeruflag)	//���炦��
					WazaStatusFlag|=WAZASTATUSFLAG_KORAETA;
				else if(OSTF[DefenceClient].itemkoraeruflag){
					WazaStatusFlag|=WAZASTATUSFLAG_ITEM_KORAETA;
					ItemNo=PSP[DefenceClient].item;
				}
			}
		}
	}
	WazaSeqAdrs++;
}

//�݂˂����`�F�b�N�����Ȃ�
void	WS_DAMAGE_LOSS_ONLY(void)
{
	u16		work;
	u8		eqp;
	u8		atc;

	work=100-(pp_rand()%16);
	if(WazaDamage!=0){
		WazaDamage*=work;
		WazaDamage/=100;
		if(WazaDamage==0) WazaDamage=1;
	}

	if(PSP[DefenceClient].item==ITEM_NAZONOMI){
//		eqp=Fld.RamSeed.item_equip;
//		atc=Fld.RamSeed.item_power;
		eqp=FRS[DefenceClient].item_equip;
		atc=FRS[DefenceClient].item_power;
	}
	else{
		eqp=ItemEquipGet(PSP[DefenceClient].item);
		atc=ItemAttackGet(PSP[DefenceClient].item);
	}

	ItemClient=DefenceClient;

	if((eqp==SOUBI_HPITIDETAERU)&&
	  ((pp_rand()%100)<atc)){
		SoubiItemSet(DefenceClient,eqp);
		OSTF[DefenceClient].itemkoraeruflag=1;
	}

	if((PSP[DefenceClient].condition2&CONDITION2_MIGAWARI)==0){	//�g���肪����ꍇ�̓`�F�b�N���Ȃ�
		if((OTF[DefenceClient].koraeruflag)||		//���炦��
		   //�A�C�e����HP1�őς���̑������ʂ�HP1�őς���
		   (OSTF[DefenceClient].itemkoraeruflag)){
			if(PSP[DefenceClient].hp<=WazaDamage){
				WazaDamage=PSP[DefenceClient].hp-1;
				if(OTF[DefenceClient].koraeruflag)	//���炦��
					WazaStatusFlag|=WAZASTATUSFLAG_KORAETA;
				else if(OSTF[DefenceClient].itemkoraeruflag){
					WazaStatusFlag|=WAZASTATUSFLAG_ITEM_KORAETA;
					ItemNo=PSP[DefenceClient].item;
				}
			}
		}
	}
	WazaSeqAdrs++;
}

void	WS_WAZA_EFFECT(void)
{
	if(ClientBit==0){
		if((ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF)&&
		   (WazaNo!=WAZANO_HENSIN)&&
		   (WazaNo!=WAZANO_MIGAWARI)){
//			if(WTD[WazaNo].damage)
//				WazaSeqAdrs++;
//			else{
				PushAdrsSet(WazaSeqAdrs+1);
				WazaSeqAdrs=(u8 *)&WAZA_Wait;
//			}
//�Z�G�t�F�N�g���o���Ă��Ȃ��Ă��J�E���^�[�͐i�߂Ă���
			UserWork[WAZA_EFF_CNT]++;
			UserWork[WAZA_EFF_CNT_TMP]++;
		}
		else{
			if(((WTD[WazaNo].attackrange&RANGE_DOUBLE)||
			   (WTD[WazaNo].attackrange&RANGE_TRIPLE)||
			   (WTD[WazaNo].attackrange&RANGE_NONE))&&
			   (UserWork[WAZA_EFF_CNT_TMP])){
				WazaSeqAdrs++;
				return;
			}
			if((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0){
				ClientNo=AttackClient;
				BSS_WAZA_EFFECT_SET(BSS_SERVER,
									WazaNo,
									UserWork[WAZA_EFF_CNT],
									wazapow,
									WazaDamage,
									PSP[AttackClient].friend,
									(u8 *)&WKC[AttackClient]);
				UserWork[WAZA_EFF_CNT]++;
				UserWork[WAZA_EFF_CNT_TMP]++;
				ClientBitSet(AttackClient);
				WazaSeqAdrs++;
			}
			else{
//				RetWazaSeqAdrs=WazaSeqAdrs+1;
				PushAdrsSet(WazaSeqAdrs+1);
				WazaSeqAdrs=(u8 *)&WAZA_Wait;
			}
		}
	}
}

#if 0
void	WS_WAZA_EFFECT(void)
{
	if(ClientBit==0){
		if(OTF[DefenceClient].hiraishinflag){
			OTF[DefenceClient].hiraishinflag=0;
			SpeabiNo=TOKUSYU_HIRAISIN;
			PushAdrsAct();
			WazaSeqAdrs=(u8 *)&WAZA_Hiraishin;
			return;
		}
		if(ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF){
			if(WTD[WazaNo].damage)
				WazaSeqAdrs++;
			else{
				PushAdrsSet(WazaSeqAdrs+1);
				WazaSeqAdrs=(u8 *)&WAZA_Wait;
			}
		}
		else if((ServerStatusFlag&SERVER_STATUS_FLAG_DOUBLE_CHECK)==0){
			if((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0){
				ClientNo=AttackClient;
				BSS_WAZA_EFFECT_SET(BSS_SERVER,
									WazaNo,
									UserWork[WAZA_EFF_CNT],
									wazapow,
									WazaDamage,
									PSP[AttackClient].friend,
									(u8 *)&WKC[AttackClient]);
				UserWork[WAZA_EFF_CNT]++;
				ClientBitSet(AttackClient);
				WazaSeqAdrs++;
			}
			else{
//				RetWazaSeqAdrs=WazaSeqAdrs+1;
				PushAdrsSet(WazaSeqAdrs+1);
				WazaSeqAdrs=(u8 *)&WAZA_Wait;
			}
		}
		else
			WazaSeqAdrs++;
	}
}
#endif

void	WS_WAZA_EFFECT_WAIT(void)
{
	if(ClientBit==0){
		WazaSeqAdrs++;
	}
}

void	WS_HP_DEC(void)
{
	if(ClientBit==0){
		if((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0){
#if 0
			if(WazaSeqAdrs[1]==SIDE_DEFENCE)
				ClientNo=DefenceClient;
			else if(WazaSeqAdrs[1]==SIDE_ATTACK)
				ClientNo=AttackClient;
			else if(WazaSeqAdrs[1]==SIDE_CLIENTNOWORK)
				ClientNo=UserWork[CLIENT_NO_WORK];
			else
				ClientNo=KizetsuClient;
#else
			ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

			if((PSP[ClientNo].condition2&CONDITION2_MIGAWARI)&&
			   (WKC[ClientNo].migawarihp)&&
			  ((ServerStatusFlag&SERVER_STATUS_FLAG_NO_MIGAWARI)==0)){
				//�݂����̏���
				FightMsgServerSet(MIGAWARIHITMSGNO,ClientNo);
			}
			else{
				BSS_HP_GAUGE_SET(BSS_SERVER,WazaDamage);
				ClientBitSet(ClientNo);
				if((MineEnemyCheck(ClientNo)==SIDE_MINE)&&
				   (WazaDamage>0))
					FTW.DamageFlag=1;
			}
		}
		WazaSeqAdrs+=2;
	}
}

void	WS_HP_DEC_WAIT(void)
{
	u8	wazatype;

	if(ClientBit==0){

		if(UserWork[DAMAGE_TYPE_WORK]){
			if(UserWork[DAMAGE_TYPE_WORK]&0x40)		//�߂��߂�p���[�΍�
				wazatype=WTD[WazaNo].wazatype;
			else
				wazatype=UserWork[DAMAGE_TYPE_WORK]&0x3f;
		}
		else
			wazatype=WTD[WazaNo].wazatype;

		if((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0){
#if 0
			if(WazaSeqAdrs[1]==SIDE_DEFENCE)
				ClientNo=DefenceClient;
			else if(WazaSeqAdrs[1]==SIDE_ATTACK)
				ClientNo=AttackClient;
			else if(WazaSeqAdrs[1]==SIDE_CLIENTNOWORK)
				ClientNo=UserWork[CLIENT_NO_WORK];
			else
				ClientNo=KizetsuClient;
#else
			ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
			if((PSP[ClientNo].condition2&CONDITION2_MIGAWARI)&&
			   (WKC[ClientNo].migawarihp)&&
			  ((ServerStatusFlag&SERVER_STATUS_FLAG_NO_MIGAWARI)==0)){
				//�݂����̏���
				if(WKC[ClientNo].migawarihp>=WazaDamage){
					if(OSTF[ClientNo].kaigaradamage==0)
						OSTF[ClientNo].kaigaradamage=WazaDamage;
					WKC[ClientNo].migawarihp-=WazaDamage;
					HitDamage=WazaDamage;
				}
				else{
					if(OSTF[ClientNo].kaigaradamage==0)
						OSTF[ClientNo].kaigaradamage=WKC[ClientNo].migawarihp;
					HitDamage=WKC[ClientNo].migawarihp;
					WKC[ClientNo].migawarihp=0;
				}
				if(WKC[ClientNo].migawarihp==0){
					WazaSeqAdrs+=2;
					PushAdrsAct();
					WazaSeqAdrs=(u8 *)&WAZA_MigawariKizetsu;
					return;
				}
			}
			else{
				ServerStatusFlag&=SERVER_STATUS_FLAG_NO_MIGAWARI_OFF;
				if(WazaDamage<0){
					PSP[ClientNo].hp-=WazaDamage;
					if(PSP[ClientNo].hp>PSP[ClientNo].hpmax) PSP[ClientNo].hp=PSP[ClientNo].hpmax;
				}
				else{
					if(ServerStatusFlag&SERVER_STATUS_FLAG_NOGAMAN)
						ServerStatusFlag&=SERVER_STATUS_FLAG_NOGAMAN_OFF;
					else{
						StoreDamage[ClientNo]+=WazaDamage;
						if(WazaSeqAdrs[1]==SIDE_DEFENCE)
							ClientNoHit[ClientNo]=AttackClient;		//���܂�p
						else
							ClientNoHit[ClientNo]=DefenceClient;	//���܂�p
					}

					if(PSP[ClientNo].hp>WazaDamage){
						PSP[ClientNo].hp-=WazaDamage;
						HitDamage=WazaDamage;
					}
					else{
						HitDamage=PSP[ClientNo].hp;
						PSP[ClientNo].hp=0;
					}

					if((OSTF[ClientNo].kaigaradamage==0)&&
					  ((ServerStatusFlag&SERVER_STATUS_FLAG_NO_ZOKUSEI)==0))
						OSTF[ClientNo].kaigaradamage=HitDamage;

					if((wazatype<HATE_TYPE)&&		//�����U��
					  ((ServerStatusFlag&SERVER_STATUS_FLAG_NO_ZOKUSEI)==0)&&
					   (WazaNo!=WAZANO_ITAMIWAKE)){	//�����݂킯�́A�J�E���^�[���Ȃ�
						OTF[ClientNo].butsurihitdamage=HitDamage;
						OSTF[ClientNo].butsurihitdamage=HitDamage;
						if(WazaSeqAdrs[1]==SIDE_DEFENCE){
							OTF[ClientNo].butsurihitclient=AttackClient;
							OSTF[ClientNo].butsurihitclient=AttackClient;
						}
						else{
							OTF[ClientNo].butsurihitclient=DefenceClient;
							OSTF[ClientNo].butsurihitclient=DefenceClient;
						}
					}
					else if((wazatype>=HATE_TYPE)&&
						   ((ServerStatusFlag&SERVER_STATUS_FLAG_NO_ZOKUSEI)==0)){
						OTF[ClientNo].tokusyuhitdamage=HitDamage;
						OSTF[ClientNo].tokusyuhitdamage=HitDamage;
						if(WazaSeqAdrs[1]==SIDE_DEFENCE){
							OTF[ClientNo].tokusyuhitclient=AttackClient;
							OSTF[ClientNo].tokusyuhitclient=AttackClient;
						}
						else{
							OTF[ClientNo].tokusyuhitclient=DefenceClient;
							OSTF[ClientNo].tokusyuhitclient=DefenceClient;
						}
					}
				}
				ServerStatusFlag&=SERVER_STATUS_FLAG_NO_ZOKUSEI_OFF;
				BSS_POKEPARA_PUT_SET(BSS_SERVER,BSS_PARA_HP,BSS_SELMONSNO,2,
								 (u8 *)&PSP[ClientNo].hp);
				ClientBitSet(ClientNo);
			}
		}
		else{
#if 0
			if(WazaSeqAdrs[1]==SIDE_DEFENCE)
				ClientNo=DefenceClient;
			else if(WazaSeqAdrs[1]==SIDE_ATTACK)
				ClientNo=AttackClient;
			else if(WazaSeqAdrs[1]==SIDE_CLIENTNOWORK)
				ClientNo=UserWork[CLIENT_NO_WORK];
			else
				ClientNo=KizetsuClient;
#else
			ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
			if(OSTF[ClientNo].kaigaradamage==0)
				OSTF[ClientNo].kaigaradamage=0xffff;
		}
		WazaSeqAdrs+=2;
	}
}

void	WS_CRITICAL_MESSAGE(void)
{
	if(ClientBit==0){
		if((CriticalValue==2)&&((WazaStatusFlag&(WAZASTATUSFLAG_NOCRITICAL))==0)){
#if 0
			i=FightMsgCreate((u8 *)&CriticalMsg);
			FightMsgServerSet(i);
#endif
			FightMsgServerSet(CRITICALMSGNO,AttackClient);
			ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
		}
		WazaSeqAdrs++;
	}
}

void	WS_WAZAKOUKA_SE(void)
{
	if(ClientBit==0){
		ClientNo=DefenceClient;
		if((WazaStatusFlag&WAZASTATUSFLAG_NOHIT)==0){
			switch((WazaStatusFlag)&(WAZASTATUSFLAG_WAZAKOUKA)){
				case	WAZASTATUSFLAG_BATSUGUN:		//���ʂ͔��Q
					BSS_SE_PLAY_SET(BSS_SERVER,SE_KOUKA_H);
					ClientBitSet(ClientNo);
					break;
				case	WAZASTATUSFLAG_IMAHITOTSU:		//���ʂ͂��܂ЂƂ�
					BSS_SE_PLAY_SET(BSS_SERVER,SE_KOUKA_L);
					ClientBitSet(ClientNo);
					break;
				case	WAZASTATUSFLAG_KOUKANAI:		//���ʂ��Ȃ�
					break;
				case	WAZASTATUSFLAG_UMAKUKIMARAN:	//���܂����܂��
					break;
				case	WAZASTATUSFLAG_ICHIGEKI:		//�ꌂ�K�E
				case	WAZASTATUSFLAG_KORAETA:			//���炦��
				case	WAZASTATUSFLAG_ITEM_KORAETA:	//�������ʂł��炦��
				default:
					if(WazaStatusFlag&WAZASTATUSFLAG_BATSUGUN){
						BSS_SE_PLAY_SET(BSS_SERVER,SE_KOUKA_H);
						ClientBitSet(ClientNo);
					}
					else if(WazaStatusFlag&WAZASTATUSFLAG_IMAHITOTSU){
						BSS_SE_PLAY_SET(BSS_SERVER,SE_KOUKA_L);
						ClientBitSet(ClientNo);
					}
					else if(((WazaStatusFlag&WAZASTATUSFLAG_KOUKANAI)==0)&&
					   ((WazaStatusFlag&WAZASTATUSFLAG_UMAKUKIMARAN)==0)){
						BSS_SE_PLAY_SET(BSS_SERVER,SE_KOUKA_M);
						ClientBitSet(ClientNo);
					}
					break;
			}
		}
		WazaSeqAdrs++;
	}
}

void	WS_WAZAKOUKA_MESSAGE(void)
{
	int	i=0;

	if(ClientBit==0){
		if(((WazaStatusFlag&WAZASTATUSFLAG_NOHIT)&&
		   ((WazaStatusFlag&WAZASTATUSFLAG_KOUKANAI)==0))||
		   ((WazaStatusFlag&WAZASTATUSFLAG_NOHIT)&&
			(ServerWork[6]>=MSG_HUSIGINAMAMORIHIT))){
//			i=FightMsgCreate((u8 *)&NoHitMsg);
//			i=FightMsgCreate((u8 *)NoHitMsgTbl[ServerWork[6]]);
			i=NoHitMsgTbl[ServerWork[6]];
			ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
		}
		else{
			ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
			switch((WazaStatusFlag)&(WAZASTATUSFLAG_WAZAKOUKA)){
				case	WAZASTATUSFLAG_BATSUGUN:	//���ʂ͔��Q
//					i=FightMsgCreate((u8 *)&BatsugunMsg);
					i=BATSUGUNMSGNO;
					break;
				case	WAZASTATUSFLAG_IMAHITOTSU:	//���ʂ͂��܂ЂƂ�
//					i=FightMsgCreate((u8 *)&ImahitotsuMsg);
					i=IMAHITOTSUMSGNO;
					break;
				case	WAZASTATUSFLAG_KOUKANAI:	//���ʂ��Ȃ�
//					i=FightMsgCreate((u8 *)&KoukanaiMsg);
					i=KOUKANAIMSGNO;
					break;
				case	WAZASTATUSFLAG_ICHIGEKI:	//�ꌂ�K�E
//					i=FightMsgCreate((u8 *)&IchigekiMsg);
					i=ICHIGEKIMSGNO;
					break;
				case	WAZASTATUSFLAG_UMAKUKIMARAN:	//���܂����܂��
//					i=FightMsgCreate((u8 *)&UmakukimaranMsg);
					i=UMAKUKIMARANMSGNO;
					break;
				case	WAZASTATUSFLAG_KORAETA:			//���炦��
//					i=FightMsgCreate((u8 *)&KoraetaMsg);
					i=KORAETAMSGNO;
					break;
				case	WAZASTATUSFLAG_ITEM_KORAETA:	//�������ʂł��炦��
//					i=FightMsgCreate((u8 *)&KoraetaMsg);
					ItemNo=PSP[DefenceClient].item;
					ItemClient=DefenceClient;
					WazaStatusFlag&=
						(WAZASTATUSFLAG_KORAETA_OFF&WAZASTATUSFLAG_ITEM_KORAETA_OFF);
					PushAdrsAct();
					WazaSeqAdrs=(u8 *)&WAZA_ItemKoraeta;
					return;
					break;
				default:
					if(WazaStatusFlag&WAZASTATUSFLAG_KOUKANAI){
//						i=FightMsgCreate((u8 *)&KoukanaiMsg);
						i=KOUKANAIMSGNO;
						break;
					}
					if(WazaStatusFlag&WAZASTATUSFLAG_ICHIGEKI){
						WazaStatusFlag&=WAZASTATUSFLAG_ICHIGEKI_OFF;
						WazaStatusFlag&=WAZASTATUSFLAG_BATSUGUN_OFF;
						WazaStatusFlag&=WAZASTATUSFLAG_IMAHITOTSU_OFF;
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Ichigeki;
						return;
					}
					if(WazaStatusFlag&WAZASTATUSFLAG_KORAETA){
//						i=FightMsgCreate((u8 *)&KoraetaMsg);
//						i=KORAETAMSGNO;
//						break;
						WazaStatusFlag&=
							(WAZASTATUSFLAG_KORAETA_OFF&WAZASTATUSFLAG_ITEM_KORAETA_OFF);
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_Koraeta;
						return;
					}
					if(WazaStatusFlag&WAZASTATUSFLAG_ITEM_KORAETA){
//						i=FightMsgCreate((u8 *)&KoraetaMsg);
//						i=ITEMKORAETAMSGNO;
//						break;
						ItemNo=PSP[DefenceClient].item;
						ItemClient=DefenceClient;
						WazaStatusFlag&=
							(WAZASTATUSFLAG_KORAETA_OFF&WAZASTATUSFLAG_ITEM_KORAETA_OFF);
						PushAdrsAct();
						WazaSeqAdrs=(u8 *)&WAZA_ItemKoraeta;
						return;
					}
					if(WazaStatusFlag&WAZASTATUSFLAG_UMAKUKIMARAN){
//						i=FightMsgCreate((u8 *)&UmakukimaranMsg);
						i=UMAKUKIMARANMSGNO;
						break;
					}
					ServerWork[7]=0;			//���b�Z�[�W�E�F�C�g�𖳌��ɂ���
					break;
			}
		}
		if(i!=0){
			FightMsgServerSet(i,AttackClient);
		}
		WazaSeqAdrs++;
	}
}

void	WS_MESSAGE(void)
{
#if 0
	int	i;
	u8	*adr;

	if(ClientBit==0){
		adr=(u8 *)((WazaSeqAdrs[1]<< 0)|
				   (WazaSeqAdrs[2]<< 8)|
				   (WazaSeqAdrs[3]<<16)|
				   (WazaSeqAdrs[4]<<24));

		i=FightMsgCreate(adr);
		FightMsgServerSet(i);
		WazaSeqAdrs+=5;
		ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
	}
#else
	if(ClientBit==0){
		FightMsgServerSet((WazaSeqAdrs[1]|(WazaSeqAdrs[2]<<8)),AttackClient);
		WazaSeqAdrs+=3;
		ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
	}
#endif
}

void	WS_ALERT_MESSAGE(void)
{
	ClientNo=AttackClient;
	BSS_ALERT_MESSAGE_SET(BSS_SERVER,(WazaSeqAdrs[1]|(WazaSeqAdrs[2]<<8)));
	ClientBitSet(ClientNo);
	WazaSeqAdrs+=3;
	ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
}

void	WS_MESSAGE_WAIT(void)
{
	u16	cnt;
	if(ClientBit==0){
		if(ServerWork[7]==0)		//���b�Z�[�W�E�F�C�g�̗L��
			WazaSeqAdrs+=3;
		else{
			cnt=WazaSeqAdrs[1]|(WazaSeqAdrs[2]<<8);
			MojiWaitCount++;
			if(MojiWaitCount>=cnt){
				MojiWaitCount=0;
				WazaSeqAdrs+=3;
				ServerWork[7]=0;	//���b�Z�[�W�E�F�C�g�𖳌��ɂ���
			}
		}
	}
}

void	WS_MESSAGE_TBL(void)
{
#if 0
	int	i;
	u8	*adr1,*adr2;

	if(ClientBit==0){
		adr1=(u8 *)(((WazaSeqAdrs[1]<< 0)|
			  		 (WazaSeqAdrs[2]<< 8)|
			  		 (WazaSeqAdrs[3]<<16)|
			  		 (WazaSeqAdrs[4]<<24))+ServerWork[5]*4);

		adr2=(u8 *)((adr1[0]<< 0)|
			  		(adr1[1]<< 8)|
			  		(adr1[2]<<16)|
			  		(adr1[3]<<24));

		i=FightMsgCreate(adr2);
		FightMsgServerSet(i);
		WazaSeqAdrs+=5;
		ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
	}
#else
	u16	*adr;

	if(ClientBit==0){
		adr=(u16 *)(((WazaSeqAdrs[1]<< 0)|
		  		     (WazaSeqAdrs[2]<< 8)|
		  		     (WazaSeqAdrs[3]<<16)|
		  		     (WazaSeqAdrs[4]<<24))+ServerWork[5]*2);

		FightMsgServerSet(adr[0],AttackClient);
		WazaSeqAdrs+=5;
		ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
	}
#endif
}

void	WS_ALERT_MESSAGE_TBL(void)
{
	u16	*adr;

	if(ClientBit==0){
		adr=(u16 *)(((WazaSeqAdrs[1]<< 0)|
		  		     (WazaSeqAdrs[2]<< 8)|
		  		     (WazaSeqAdrs[3]<<16)|
		  		     (WazaSeqAdrs[4]<<24))+ServerWork[5]*2);

		ClientNo=AttackClient;
		BSS_ALERT_MESSAGE_SET(BSS_SERVER,adr[0]);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=5;
		ServerWork[7]=1;				//���b�Z�[�W�E�F�C�g��L���ɂ���
	}
}

u8		ActionNoGet(u8 clientno)
{
	int	i;

	for(i=0;i<ClientSetMax;i++){
		if(ActionClientNo[i]==clientno)
			break;
	}
	return i;
}

const u32 TSUIKATbl[]={
	CONDITION_NOCHECK,			//�_�~�[
	CONDITION_NEMURI,			//�˂ނ�
	CONDITION_DOKU,				//��
	CONDITION_YAKEDO,			//�₯��
	CONDITION_KOORI,			//������
	CONDITION_MAHI,				//�܂�
	CONDITION_DOKUDOKU,			//�ǂ��ǂ�
	CONDITION2_KONRAN,			//������
	CONDITION2_HIRUMU,			//�Ђ��
	CONDITION2_NOCHECK,			//�g���C�A�^�b�N�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_SAWAGU,			//���킮
	CONDITION2_NOCHECK,			//�˂��ɂ��΂�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_KEEP,			//���ߌn�̋Z
	CONDITION2_SHIME,			//���ߌn�̋Z
	CONDITION2_NOCHECK,			//�͂˂�����n�̋Z�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�U���A�b�v�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�h��A�b�v�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�f�����A�b�v�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//����U���A�b�v�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//����h��A�b�v�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�������A�b�v�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//��𗦃A�b�v�i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�U���_�E���i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�h��_�E���i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�f�����_�E���i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//����U���_�E���i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//����h��_�E���i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�������_�E���i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//��𗦃_�E���i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_HANDOU,			//�͂�ǂ�
	CONDITION2_NOCHECK,			//������i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_NOCHECK,			//�ǂ�ڂ��i���ۂ̓r�b�g�͂����Ȃ��j
	CONDITION2_KUROIMANAZASHI,	//���낢�܂Ȃ���
	CONDITION2_AKUMU,			//������
	CONDITION2_NOCHECK,			//���񂵂̂�����
	CONDITION2_NOCHECK,			//���������X�s��
	CONDITION2_NOCHECK,			//�܂Ђ��Ȃ���
	CONDITION2_NOCHECK,			//�΂�������
	CONDITION2_NOCHECK,			//���Ă݃^�b�N��
	CONDITION2_NOCHECK,			//�U���̓A�b�v(2�i�K�j
	CONDITION2_NOCHECK,			//�h��̓A�b�v(2�i�K�j
	CONDITION2_NOCHECK,			//�f�����A�b�v(2�i�K�j
	CONDITION2_NOCHECK,			//���U�A�b�v(2�i�K�j
	CONDITION2_NOCHECK,			//���h�A�b�v(2�i�K�j
	CONDITION2_NOCHECK,			//�������A�b�v(2�i�K�j
	CONDITION2_NOCHECK,			//��𗦃A�b�v(2�i�K�j
	CONDITION2_NOCHECK,			//�U���̓_�E��(2�i�K�j
	CONDITION2_NOCHECK,			//�h��̓_�E��(2�i�K�j
	CONDITION2_NOCHECK,			//�f�����_�E��(2�i�K�j
	CONDITION2_NOCHECK,			//���U�_�E��(2�i�K�j
	CONDITION2_NOCHECK,			//���h�_�E��(2�i�K�j
	CONDITION2_NOCHECK,			//�������_�E��(2�i�K�j
	CONDITION2_NOCHECK,			//��𗦃_�E��(2�i�K�j
	CONDITION2_ABARERU,			//���΂��
	CONDITION2_NOCHECK,			//�͂������Ƃ�
	CONDITION2_NOCHECK,			//�����A�b�v�iabiritycnt�n�j
	CONDITION2_NOCHECK,			//�����A�b�v2�i�K�iabiritycnt�n�j
	CONDITION2_NOCHECK,			//�����_�E���iabiritycnt�n�j
	CONDITION2_NOCHECK,			//�����_�E��2�i�K�iabiritycnt�n�j
	CONDITION2_NOCHECK,			//�I�[�o�[�q�[�g
};

const u8 * const TSUIKASeqTbl[]={
	(u8 *)&WAZA_AddNemuri,				//�_�~�[
	(u8 *)&WAZA_AddNemuri,				//�˂ނ�
	(u8 *)&WAZA_AddDoku,				//��
	(u8 *)&WAZA_AddYakedo,				//�₯��
	(u8 *)&WAZA_AddKoori,				//������
	(u8 *)&WAZA_AddMahi,				//�܂�
	(u8 *)&WAZA_AddDokudoku,			//�ǂ��ǂ�
	(u8 *)&WAZA_AddKonran,				//������
	(u8 *)&WAZA_AddNemuri,				//�Ђ��
	(u8 *)&WAZA_AddNemuri,				//�g���C�A�^�b�N
	(u8 *)&WAZA_AddSawagu,				//���킮
	(u8 *)&WAZA_AddNekonikoban,			//�˂��ɂ��΂�
	(u8 *)&WAZA_AddNemuri,				//���ߌn�̋Z
	(u8 *)&WAZA_AddShime,				//���ߌn�̋Z
	(u8 *)&WAZA_AddHanekaeri,			//�͂˂�����n�̋Z
	(u8 *)&WAZA_AddNemuri,				//�U���A�b�v
	(u8 *)&WAZA_AddNemuri,				//�h��A�b�v
	(u8 *)&WAZA_AddNemuri,				//�f�����A�b�v
	(u8 *)&WAZA_AddNemuri,				//���U�A�b�v
	(u8 *)&WAZA_AddNemuri,				//���h�A�b�v
	(u8 *)&WAZA_AddNemuri,				//�������A�b�v
	(u8 *)&WAZA_AddNemuri,				//��𗦃A�b�v
	(u8 *)&WAZA_AddNemuri,				//�U���_�E��
	(u8 *)&WAZA_AddNemuri,				//�h��_�E��
	(u8 *)&WAZA_AddNemuri,				//�f�����_�E��
	(u8 *)&WAZA_AddNemuri,				//���U�_�E��
	(u8 *)&WAZA_AddNemuri,				//���h�_�E��
	(u8 *)&WAZA_AddNemuri,				//�������_�E��
	(u8 *)&WAZA_AddNemuri,				//��𗦃_�E��
	(u8 *)&WAZA_AddNemuri,				//�͂�ǂ�
	(u8 *)&WAZA_AddNemuri,				//������
	(u8 *)&WAZA_AddNemuri,				//�ǂ�ڂ�
	(u8 *)&WAZA_AddNemuri,				//���낢�܂Ȃ���
	(u8 *)&WAZA_AddNemuri,				//������
	(u8 *)&WAZA_AddNemuri,				//���񂵂̂�����
	(u8 *)&WAZA_AddNemuri,				//���������X�s��
	(u8 *)&WAZA_AddNemuri,				//�܂Ђ��Ȃ���
	(u8 *)&WAZA_AddNemuri,				//�΂�������
	(u8 *)&WAZA_AddHanekaeri,			//���Ă݃^�b�N��
};

void	TsuikaAct(u8 flag,u8 flag2)
{
	u8	tsuika=0;
	u16	gold;
	u8	tsuikaflag=0;
	u16	*item=0;
	u8	weatherflag=1;
	u8	dir=0;

	if(ServerWork[3]&TSUIKA_ATTACK){
		TsuikaClient=AttackClient;
		ServerWork[3]&=TSUIKA_ATTACK_OFF;
		tsuikaflag=TSUIKA_ATTACK;
		UserWork[CLIENT_NO_WORK]=DefenceClient;
	}
	else{
		TsuikaClient=DefenceClient;
		UserWork[CLIENT_NO_WORK]=AttackClient;
	}

	//����\�͂��Ղ�͒ǉ����ʂ������Ȃ�
	if((PSP[TsuikaClient].speabino==TOKUSYU_RINPUN)&&
	//��������̒ǉ����ʂ͂��Ղ�̉e�����󂯂Ȃ�
	  ((ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA)==0)&&
	   (flag==TSUIKA_INDIRECT)&&
	   (ServerWork[3]<COND2_SAWAGU)){
		WazaSeqAdrs++;
		return;
	}

	//����҂̂܂��肪����Ƃ��͒ǉ����ʂ������Ȃ�
	if((SideCondition[ClientTypeGet(TsuikaClient)&1]&SIDE_CONDITION_SHINPI)&&
	//��������̒ǉ����ʂ͂���҂̂܂���̉e�����󂯂Ȃ�
	  ((ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA)==0)&&
	   (flag==TSUIKA_INDIRECT)&&
	   (ServerWork[3]<COND2_HIRUMU)){
		WazaSeqAdrs++;
		return;
	}

#if 0
	//����\�̓N���A�{�f�B�͔\�͒ቺ�������Ȃ�
	if((PSP[TsuikaClient].speabino==TOKUSYU_KURIABODEHI)&&
	   (ServerWork[3]>=COND2_POWDOWN)&&
	   (ServerWork[3]<=COND2_AVOIDDOWN)){
		if(flag==TSUIKA_DIRECT){
			WazaSeqAdrs=(u8 *)&WAZA_ClearBody;
			SpeabiNo=TOKUSYU_KURIABODEHI;
		}
		else
			WazaSeqAdrs++;
		return;
	}
#endif

	//�̗͂O�́A�ǉ����ʂȂ��@�l�R�ɂ��΂�A�ǂ�ڂ��͗�O
	if((PSP[TsuikaClient].hp==0)&&
	   (ServerWork[3]!=COND2_NEKONIKOBAN)&&
	   (ServerWork[3]!=COND2_DOROBOU)){
		WazaSeqAdrs++;
		return;
	}

	//�g���肪����ꍇ�͒ǉ����ʂȂ�
	if((PSP[TsuikaClient].condition2&CONDITION2_MIGAWARI)&&(tsuikaflag!=TSUIKA_ATTACK)){
		WazaSeqAdrs++;
		return;
	}

	if(ServerWork[3]<COND2_KONRAN){
		switch(TSUIKATbl[ServerWork[3]]){
			case CONDITION_NEMURI:
				if(PSP[TsuikaClient].speabino!=TOKUSYU_BOUON){
					for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
						if(PSP[ClientNo].condition2&CONDITION2_SAWAGU)
							break;
					}
				}
				else
					ClientNo=ClientSetMax;
				if((PSP[TsuikaClient].condition==0)&&
					(ClientNo==ClientSetMax)&&
					(PSP[TsuikaClient].speabino!=TOKUSYU_YARUKI)&&
					(PSP[TsuikaClient].speabino!=TOKUSYU_HUMIN)){
					//�˂ނ�����A���ߋZ�͉���
					ServerKeepOff(TsuikaClient);
					tsuika=1;
				}
				break;
			case CONDITION_DOKU:
				//�ǉ�����100%�̎��͂��̏����ł�
				//����\�͂߂񂦂��͂ǂ��ɂȂ�Ȃ�
				if((PSP[TsuikaClient].speabino==TOKUSYU_MENEKI)&&
				  ((flag==TSUIKA_DIRECT)||(flag2==TSUIKA_ALWAYS))){
					SpeabiNo=TOKUSYU_MENEKI;
					TokusyuNoSet(TsuikaClient,SpeabiNo);
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Meneki;
					if(ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA){
						ServerWork[5]=1;
						ServerStatusFlag&=SERVER_STATUS_FLAG_SPEABITSUIKA_OFF;
					}
					else{
						ServerWork[5]=0;
					}
					return;
				}
				else if(((PSP[TsuikaClient].type1==POISON_TYPE)||
						 (PSP[TsuikaClient].type2==POISON_TYPE)||
						 (PSP[TsuikaClient].type1==METAL_TYPE)||
						 (PSP[TsuikaClient].type2==METAL_TYPE))&&
					     (ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA)&&
					    ((flag==TSUIKA_DIRECT)||(flag2==TSUIKA_ALWAYS))){
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Meneki;
					ServerWork[5]=2;
					return;
				}
				else if((PSP[TsuikaClient].type1!=POISON_TYPE)&&
						(PSP[TsuikaClient].type2!=POISON_TYPE)&&
						(PSP[TsuikaClient].type1!=METAL_TYPE)&&
						(PSP[TsuikaClient].type2!=METAL_TYPE)&&
						(PSP[TsuikaClient].condition==0)&&
						(PSP[TsuikaClient].speabino!=TOKUSYU_MENEKI))
					tsuika=1;
				break;
			case CONDITION_YAKEDO:
				//�ǉ�����100%�̎��͂��̏����ł�
				//����\�݂͂��̃x�[���͂₯�ǂ��Ȃ�
				if((PSP[TsuikaClient].speabino==TOKUSYU_MIZUNOBEERU)&&
				  ((flag==TSUIKA_DIRECT)||(flag2==TSUIKA_ALWAYS))){
					SpeabiNo=TOKUSYU_MIZUNOBEERU;
					TokusyuNoSet(TsuikaClient,SpeabiNo);
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Mizunobeeru;
					if(ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA){
						ServerWork[5]=1;
						ServerStatusFlag&=SERVER_STATUS_FLAG_SPEABITSUIKA_OFF;
					}
					else{
						ServerWork[5]=0;
					}
					return;
				}
				else if(((PSP[TsuikaClient].type1==FIRE_TYPE)||
						 (PSP[TsuikaClient].type2==FIRE_TYPE))&&
					     (ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA)&&
					    ((flag==TSUIKA_DIRECT)||(flag2==TSUIKA_ALWAYS))){
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Mizunobeeru;
					ServerWork[5]=2;
					return;
				}
				else if((PSP[TsuikaClient].type1!=FIRE_TYPE)&&
						(PSP[TsuikaClient].type2!=FIRE_TYPE)&&
						//����\�݂͂��̃x�[���͂₯�ǂ��Ȃ�
						(PSP[TsuikaClient].speabino!=TOKUSYU_MIZUNOBEERU)&&
						(PSP[TsuikaClient].condition==0))
					tsuika=1;
				break;
			case CONDITION_KOORI:
				if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,
										0,
										TOKUSYU_NOOTENKI,0,0)==0)&&
				   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,
										0,
										TOKUSYU_EAROKKU,0,0)==0)){
				  if(FieldCondition&FIELD_CONDITION_HIDERING)
					  weatherflag=0;
				}
				//����ł�����ǉ�����100%�͂Ȃ��̂ł��̏����ł�
				if((PSP[TsuikaClient].type1!=KOORI_TYPE)&&
				   (PSP[TsuikaClient].type2!=KOORI_TYPE)&&
				   (PSP[TsuikaClient].condition==0)&&
				//�ɂق�΂ꒆ�͂�����Ȃ�
				   (weatherflag)&&
				//����\�̓}�O�}�̂�낢�͂�����Ȃ�
				   (PSP[TsuikaClient].speabino!=TOKUSYU_MAGUMANOYOROI)){
					//����������A���ߋZ�͉���
					ServerKeepOff(TsuikaClient);
					tsuika=1;
				}
				break;
			case CONDITION_MAHI:
				//�ǉ�����100%�̎��͂��̏����ł�
				//����\�͂��イ�Ȃ�͂܂Ђ��Ȃ�
				if((PSP[TsuikaClient].speabino==TOKUSYU_ZYUUNAN)&&
				  ((flag==TSUIKA_DIRECT)||(flag2==TSUIKA_ALWAYS))){
					SpeabiNo=TOKUSYU_ZYUUNAN;
					TokusyuNoSet(TsuikaClient,SpeabiNo);
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Juunan;
					if(ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA){
						ServerWork[5]=1;
						ServerStatusFlag&=SERVER_STATUS_FLAG_SPEABITSUIKA_OFF;
					}
					else{
						ServerWork[5]=0;
					}
					return;
				}
				//����\�͂��イ�Ȃ�͂܂Ђ��Ȃ�
				else if((PSP[TsuikaClient].speabino!=TOKUSYU_ZYUUNAN)&&
						(PSP[TsuikaClient].condition==0))
					tsuika=1;
				break;
			case CONDITION_DOKUDOKU:
				//�ǉ�����100%�̎��͂��̏����ł�
				//����\�͂߂񂦂��͂ǂ��ɂȂ�Ȃ�
				if((PSP[TsuikaClient].speabino==TOKUSYU_MENEKI)&&
				  ((flag==TSUIKA_DIRECT)||(flag2==TSUIKA_ALWAYS))){
					SpeabiNo=TOKUSYU_MENEKI;
					TokusyuNoSet(TsuikaClient,SpeabiNo);
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Meneki;
					if(ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA){
						ServerWork[5]=1;
						ServerStatusFlag&=SERVER_STATUS_FLAG_SPEABITSUIKA_OFF;
					}
					else{
						ServerWork[5]=0;
					}
					return;
				}
				else if(((PSP[TsuikaClient].type1==POISON_TYPE)||
						 (PSP[TsuikaClient].type2==POISON_TYPE)||
						 (PSP[TsuikaClient].type1==METAL_TYPE)||
						 (PSP[TsuikaClient].type2==METAL_TYPE))&&
					     (ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA)&&
					    ((flag==TSUIKA_DIRECT)||(flag2==TSUIKA_ALWAYS))){
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Meneki;
					ServerWork[5]=2;
					return;
				}
				else if(PSP[TsuikaClient].condition==0){
					if((PSP[TsuikaClient].type1!=POISON_TYPE)&&
				   	   (PSP[TsuikaClient].type2!=POISON_TYPE)&&
					   (PSP[TsuikaClient].type1!=METAL_TYPE)&&
				   	   (PSP[TsuikaClient].type2!=METAL_TYPE)){
						if(PSP[TsuikaClient].speabino!=TOKUSYU_MENEKI){
							PSP[TsuikaClient].condition&=CONDITION_DOKUDOKU_CNT_OFF;
							PSP[TsuikaClient].condition&=CONDITION_DOKU_OFF;
							tsuika=1;
						}
					}
					else
						WazaStatusFlag|=WAZASTATUSFLAG_KOUKANAI;
				}
				break;
		}
		if(tsuika==1){
			PushAdrsSet(WazaSeqAdrs+1);
			if(TSUIKATbl[ServerWork[3]]==CONDITION_NEMURI)
				PSP[TsuikaClient].condition|=((pp_rand()&0x03)+2);
			else
				PSP[TsuikaClient].condition|=TSUIKATbl[ServerWork[3]];
			WazaSeqAdrs=(u8 *)TSUIKASeqTbl[ServerWork[3]];
			ClientNo=TsuikaClient;
			BSS_POKEPARA_PUT_SET(BSS_SERVER,
								BSS_PARA_CONDITION,
								BSS_SELMONSNO,
								4,
								(u8 *)&PSP[TsuikaClient].condition);
			ClientBitSet(ClientNo);
			if(ServerStatusFlag&SERVER_STATUS_FLAG_SPEABITSUIKA){
				ServerWork[5]=1;
				ServerStatusFlag&=SERVER_STATUS_FLAG_SPEABITSUIKA_OFF;
			}
			else{
				ServerWork[5]=0;
			}
			if((ServerWork[3]==COND_DOKU)||
			   (ServerWork[3]==COND_DOKUDOKU)||
			   (ServerWork[3]==COND_MAHI)||
			   (ServerWork[3]==COND_YAKEDO)){
				UserWork[TSUIKA_PARA]=ServerWork[3];
				ServerStatusFlag|=SERVER_STATUS_FLAG_TSUIKA;
			}
		}
		else if(tsuika==0)
			WazaSeqAdrs++;
	}
	else{
		if(PSP[TsuikaClient].condition2&TSUIKATbl[ServerWork[3]])
			WazaSeqAdrs++;
		else{
			switch(ServerWork[3]){
				case	COND2_KONRAN:
					if(PSP[TsuikaClient].speabino!=TOKUSYU_MAIPEESU){
						if((PSP[TsuikaClient].condition2&CONDITION2_KONRAN)==0){
//							PSP[TsuikaClient].condition2|=TSUIKATbl[ServerWork[3]];
							PSP[TsuikaClient].condition2|=((pp_rand()%4)+2);
							PushAdrsSet(WazaSeqAdrs+1);
							WazaSeqAdrs=(u8 *)TSUIKASeqTbl[ServerWork[3]];
						}
						else
							WazaSeqAdrs++;
					}
					else
						WazaSeqAdrs++;
					break;
				case	COND2_HIRUMU:
					//����\�͂��������傭�͂Ђ�܂Ȃ�
					if((PSP[TsuikaClient].speabino==TOKUSYU_SEISINRYOKU)&&
					  ((flag==TSUIKA_DIRECT)||(flag2==TSUIKA_ALWAYS))){
						SpeabiNo=TOKUSYU_SEISINRYOKU;
						TokusyuNoSet(TsuikaClient,SpeabiNo);
						WazaSeqAdrs=(u8 *)&WAZA_Seisinryoku;
						return;
					}
					else if((PSP[TsuikaClient].speabino!=TOKUSYU_SEISINRYOKU)&&
//						   ((PSP[TsuikaClient].condition&CONDITION_NEMURI)==0)&&
						    (ActionNoGet(TsuikaClient)>ActionCount))
						PSP[TsuikaClient].condition2|=TSUIKATbl[ServerWork[3]];
					WazaSeqAdrs++;
					break;
				case	COND2_SAWAGU:
					if((PSP[TsuikaClient].condition2&CONDITION2_SAWAGU)==0){
						PSP[TsuikaClient].condition2|=CONDITION2_KEEP;
						WazaNoKeep[TsuikaClient]=WazaNo;
						PSP[TsuikaClient].condition2|=(((pp_rand()&0x3)+0x2)<<SAWAGU_SHIFT);
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)TSUIKASeqTbl[ServerWork[3]];
					}
					else
						WazaSeqAdrs++;
					break;
				case	COND2_NEKONIKOBAN:
					if((ClientTypeGet(AttackClient)&1)==0){
						gold=KobanCounter;
						KobanCounter+=PSP[AttackClient].level*5;
						if(gold>KobanCounter)
							KobanCounter=0xffff;
					}
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)TSUIKASeqTbl[ServerWork[3]];
					break;
				case	COND2_TRIATTACK:
					if(PSP[TsuikaClient].condition==0){
						ServerWork[3]=COND_YAKEDO+(pp_rand()%3);
						TsuikaAct(TSUIKA_INDIRECT,0);
					}
					else
						WazaSeqAdrs++;
					break;
				case	COND2_KEEP:
					PSP[TsuikaClient].condition2|=CONDITION2_KEEP;
					WazaNoKeep[TsuikaClient]=WazaNo;
					OTF[TsuikaClient].tamewazaflag=1;
					WazaSeqAdrs++;
					break;
				case	COND2_SHIME:
					if((PSP[TsuikaClient].condition2&CONDITION2_SHIME)==0){
						PSP[TsuikaClient].condition2|=(((pp_rand()&0x03)+3)<<
														SHIME_SHIFT);
						UserWork[SHIME_WAZANO+TsuikaClient*2]=WazaNo&0xff;
						UserWork[SHIME_WAZANO+TsuikaClient*2+1]=(WazaNo&0xff00)>>8;
						UserWork[SHIME_WAZACLIENT+TsuikaClient]=AttackClient;
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)TSUIKASeqTbl[ServerWork[3]];
						for(ServerWork[5]=0;ServerWork[5]<5;ServerWork[5]++){
							if(ShimeWazaNo[ServerWork[5]]==WazaNo) break;
						}
					}
					else
						WazaSeqAdrs++;
					break;
				case	COND2_HANEKAERI:
					WazaDamage=HitDamage/4;
					if(WazaDamage==0) WazaDamage=1;
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)TSUIKASeqTbl[ServerWork[3]];
					break;
				case	COND2_POWUP:
				case	COND2_DEFUP:
				case	COND2_AGIUP:
				case	COND2_SPEPOWUP:
				case	COND2_SPEDEFUP:
				case	COND2_HITUP:
				case	COND2_AVOIDUP:
					if(CondChgAct(PARA_RANKU1,
								   ServerWork[3]-COND2_POWUP+1,
							   	   TSUIKA_INDIRECT|tsuikaflag,0))
						WazaSeqAdrs++;
					else{
						UserWork[FIGHT_EFFECT_WORK+0]=
							(ServerWork[3]&TSUIKA_ATTACK_OFF&TSUIKA_ALWAYS_OFF);
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)&WAZA_CondUpTsuikaMsg;
					}
					break;
				case	COND2_POWDOWN:
				case	COND2_DEFDOWN:
				case	COND2_AGIDOWN:
				case	COND2_SPEPOWDOWN:
				case	COND2_SPEDEFDOWN:
				case	COND2_HITDOWN:
				case	COND2_AVOIDDOWN:
					if(CondChgAct(PARA_RANKD1,
								  ServerWork[3]-COND2_POWDOWN+1,
							   	  TSUIKA_INDIRECT|tsuikaflag,0))
						WazaSeqAdrs++;
					else{
						UserWork[FIGHT_EFFECT_WORK+0]=
							(ServerWork[3]&TSUIKA_ATTACK_OFF&TSUIKA_ALWAYS_OFF);
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)&WAZA_CondDownTsuikaMsg;
					}
					break;
				case	COND2_POWUP2:
				case	COND2_DEFUP2:
				case	COND2_AGIUP2:
				case	COND2_SPEPOWUP2:
				case	COND2_SPEDEFUP2:
				case	COND2_HITUP2:
				case	COND2_AVOIDUP2:
					if(CondChgAct(PARA_RANKU2,
								   ServerWork[3]-COND2_POWUP2+1,
							   	   TSUIKA_INDIRECT|tsuikaflag,0))
						WazaSeqAdrs++;
					else{
						UserWork[FIGHT_EFFECT_WORK+0]=
							(ServerWork[3]&TSUIKA_ATTACK_OFF&TSUIKA_ALWAYS_OFF);
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)&WAZA_CondUpTsuikaMsg;
					}
					break;
				case	COND2_POWDOWN2:
				case	COND2_DEFDOWN2:
				case	COND2_AGIDOWN2:
				case	COND2_SPEPOWDOWN2:
				case	COND2_SPEDEFDOWN2:
				case	COND2_HITDOWN2:
				case	COND2_AVOIDDOWN2:
					if(CondChgAct(PARA_RANKD2,
								  ServerWork[3]-COND2_POWDOWN2+1,
							   	  TSUIKA_INDIRECT|tsuikaflag,0))
						WazaSeqAdrs++;
					else{
						UserWork[FIGHT_EFFECT_WORK+0]=
							(ServerWork[3]&TSUIKA_ATTACK_OFF&TSUIKA_ALWAYS_OFF);
						UserWork[FIGHT_EFFECT_WORK+1]=0;
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)&WAZA_CondDownTsuikaMsg;
					}
					break;
				case	COND2_HANDOU:
					PSP[TsuikaClient].condition2|=CONDITION2_HANDOU;
					WKC[TsuikaClient].handoucnt=2;
					WazaNoKeep[TsuikaClient]=WazaNo;
					WazaSeqAdrs++;
					break;
				case	COND2_IKARI:
					PSP[AttackClient].condition2|=CONDITION2_IKARI;
					WazaSeqAdrs++;
					break;
				case	COND2_DOROBOU:
					dir=MineEnemyCheck(AttackClient);
					if((MineEnemyCheck(AttackClient)==SIDE_ENEMY)&&
					  ((FightType&FIGHT_TYPE_NO_DOROBOU)==0)&&
					   (FieldTrainerID!=ORUSUBAN_TRAINER)){
						WazaSeqAdrs++;
						break;
					}
					if(((FightType&FIGHT_TYPE_NO_DOROBOU)==0)&&
					    (FieldTrainerID!=ORUSUBAN_TRAINER)&&
					//�͂������Ƃ���Ă���Ƃ��́A�������Ȃ�
					    (FCC.hatakiotosuitem[dir]&no2bittbl[SelMonsNo[AttackClient]])){
						WazaSeqAdrs++;
						break;
					}
					//����\�͂˂񂿂Ⴍ�͂ǂ����𓐂܂�Ȃ�
					if((PSP[DefenceClient].item)&&
					   (PSP[DefenceClient].speabino==TOKUSYU_NENTYAKU)){
							WazaSeqAdrs=(u8 *)&WAZA_Nentyaku;
							SpeabiNo=PSP[DefenceClient].speabino;
							TokusyuNoSet(DefenceClient,SpeabiNo);
					}
					else if((PSP[AttackClient].item==0)&&
							(PSP[DefenceClient].item!=ITEM_NAZONOMI)&&
							(PSP[DefenceClient].item)){
						item=(u16 *)&UserWork[ITEMNO_STOCK+AttackClient*2];
						ItemNo=item[0]=PSP[DefenceClient].item;
						PSP[DefenceClient].item=0;
						ClientNo=AttackClient;
						BSS_POKEPARA_PUT_SET(BSS_SERVER,
											 BSS_PARA_ITEM,
											 BSS_SELMONSNO,
											 2,
		 									 (u8 *)&ItemNo);
						ClientBitSet(AttackClient);
						ClientNo=DefenceClient;
						BSS_POKEPARA_PUT_SET(BSS_SERVER,
											 BSS_PARA_ITEM,
											 BSS_SELMONSNO,
											 2,
		 									 (u8 *)&PSP[DefenceClient].item);
						ClientBitSet(DefenceClient);
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)&WAZA_AddDorobou;
						//�������n�`�}�L�̃��[�N���N���A
						UserWork[KODAWARI_WAZANO+0+DefenceClient*2]=0;
						UserWork[KODAWARI_WAZANO+1+DefenceClient*2]=0;
					}
					else
						WazaSeqAdrs++;
					break;
				case	COND2_KUROIMANAZASHI:
					PSP[DefenceClient].condition2|=CONDITION2_KUROIMANAZASHI;
					WKC[DefenceClient].manazashiclientno=AttackClient;
					WazaSeqAdrs++;
					break;
				case	COND2_AKUMU:
					PSP[DefenceClient].condition2|=CONDITION2_AKUMU;
					WazaSeqAdrs++;
					break;
				case	COND2_GENSHINOCHIKARA:
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Genshinochikara;
					break;
				case	COND2_KOUSOKUSPIN:
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Kousokuspin;
					break;
				case	COND_NOMAHI:
					if(PSP[DefenceClient].condition&CONDITION_MAHI){
						PSP[DefenceClient].condition&=CONDITION_MAHI_OFF;
						ClientNo=DefenceClient;
						BSS_POKEPARA_PUT_SET(BSS_SERVER,
											BSS_PARA_CONDITION,
											BSS_SELMONSNO,
											4,
											(u8 *)&PSP[ClientNo].condition);
						ClientBitSet(ClientNo);
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)&WAZA_NoMahi;
					}
					else
						WazaSeqAdrs++;
					break;
				case	COND2_BAKADIKARA:
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_Bakadikara;
					break;
				case	COND2_HANEKAERI2:
					WazaDamage=HitDamage/3;
					if(WazaDamage==0) WazaDamage=1;
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)TSUIKASeqTbl[ServerWork[3]];
					break;
				case	COND2_ABARERU:
					if((PSP[TsuikaClient].condition2&CONDITION2_ABARERU)==0){
						PSP[TsuikaClient].condition2|=CONDITION2_KEEP;
						WazaNoKeep[TsuikaClient]=WazaNo;
						//���΂��2-3�^�[���Z�b�g
						PSP[TsuikaClient].condition2|=
							(((pp_rand()&1)+2)<<ABARERU_SHIFT);	
					}
					else
						WazaSeqAdrs++;
					break;
				case	COND2_HATAKIOTOSU:
					//����\�͂˂񂿂Ⴍ�͂ǂ����𓐂܂�Ȃ�
					if((PSP[TsuikaClient].speabino==TOKUSYU_NENTYAKU)&&
					   (PSP[TsuikaClient].item!=0)){
						SpeabiNo=TOKUSYU_NENTYAKU;
						WazaSeqAdrs=(u8 *)&WAZA_Nentyaku;
						TokusyuNoSet(TsuikaClient,SpeabiNo);
					}
					else if(PSP[TsuikaClient].item!=0){
						dir=MineEnemyCheck(TsuikaClient);
						ItemNo=PSP[TsuikaClient].item;
						PSP[TsuikaClient].item=0;
						FCC.hatakiotosuitem[dir]|=no2bittbl[SelMonsNo[TsuikaClient]];
						PushAdrsSet(WazaSeqAdrs+1);
						WazaSeqAdrs=(u8 *)&WAZA_Hatakiotosu;
						//�������n�`�}�L�̃��[�N���N���A
						UserWork[KODAWARI_WAZANO+0+TsuikaClient*2]=0;
						UserWork[KODAWARI_WAZANO+1+TsuikaClient*2]=0;
					}
					else
						WazaSeqAdrs++;
					break;
				case	COND2_OVERHEAT:
					PushAdrsSet(WazaSeqAdrs+1);
					WazaSeqAdrs=(u8 *)&WAZA_OverHeat;
					break;
			}
		}
	}
}

void	WS_TSUIKA_CHECK(void)
{
	u16	kakuritsu;

	//����\�͂Ă�̂߂��݂́A�m����{�ɂ���
	if(PSP[AttackClient].speabino==TOKUSYU_TENNOMEGUMI)
		kakuritsu=WTD[WazaNo].addeffect*2;
	else
		kakuritsu=WTD[WazaNo].addeffect;

#ifdef PM_DEBUG
	if((DebugFightFlag&DEBUG_FIGHT_WAZA_DEBUG)&&
	  ((ServerWork[3]&TSUIKA_ALWAYS)==0)&&
	  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
		TsuikaAct(TSUIKA_INDIRECT,0);
	}
	else if((ServerWork[3]&TSUIKA_ALWAYS)&&
		   ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
		ServerWork[3]&=TSUIKA_ALWAYS_OFF;
		TsuikaAct(TSUIKA_INDIRECT,TSUIKA_ALWAYS);
	}
#else
	if((ServerWork[3]&TSUIKA_ALWAYS)&&
	  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
		ServerWork[3]&=TSUIKA_ALWAYS_OFF;
		TsuikaAct(TSUIKA_INDIRECT,TSUIKA_ALWAYS);
	}
#endif
	else if(((pp_rand()%100)<=kakuritsu)&&
			 (ServerWork[3]!=0)&&
			((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
		if(kakuritsu>=100)
			TsuikaAct(TSUIKA_INDIRECT,TSUIKA_ALWAYS);
		else
			TsuikaAct(TSUIKA_INDIRECT,0);
	}
	else
		WazaSeqAdrs++;
	ServerWork[3]=0;
	UserWork[SERVER_WORK3_TEMP]=0;
}

void	WS_TSUIKA_DIRECT_ACT(void)
{
	TsuikaAct(TSUIKA_DIRECT,0);
}

void	WS_TSUIKA_INDIRECT_ACT(void)
{
	TsuikaAct(TSUIKA_INDIRECT,0);
}

void	WS_SAKUJO(void)
{
	
#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	if(ServerWork[3]<COND2_KONRAN)
		PSP[ClientNo].condition&=(TSUIKATbl[ServerWork[3]]^0xffffffff);
	else
		PSP[ClientNo].condition2&=(TSUIKATbl[ServerWork[3]]^0xffffffff);

	ServerWork[3]=0;
	WazaSeqAdrs+=2;
	UserWork[SERVER_WORK3_TEMP]=0;
}

void	WS_KIZETSU_CHECK(void)
{
	u8	*adr;
	u8	clientno;
	u8	no;

	if(WazaSeqAdrs[2]){
//�܂��т��ɂ�邫�����������̃`�F�b�N��p�i���̂Ƃ���j
//�g������ύX����Ƃ��͒���
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
		if(ServerStatusFlag&(no2bittbl[ClientNo]<<28)){
			//�܂��т��̃V�[�P���X��WazaSeqAdrs��Push���Ă���̂�Pop���Ă���
			adr=(u8 *)((WazaSeqAdrs[3]<< 0)|
					   (WazaSeqAdrs[4]<< 8)|
					   (WazaSeqAdrs[5]<<16)|
					   (WazaSeqAdrs[6]<<24));
			PopAdrsAct();
			WazaSeqAdrs=adr;
			//�܂��т��`�F�b�N�𗎂Ƃ�
			SideCondition[MineEnemyCheck(ClientNo)]&=SIDE_CONDITION_MAKIBISHI_CHECK_OFF;
		}
		else
			WazaSeqAdrs+=7;
	}
	else{
		if(WazaSeqAdrs[1]==SIDE_ATTACK){
			ClientNo=AttackClient;
			clientno=DefenceClient;
			adr=(u8 *)&WAZA_KizetsuA;
		}
		else{
			ClientNo=DefenceClient;
			clientno=AttackClient;
			adr=(u8 *)&WAZA_KizetsuD;
		}
		if(((NoReshuffleClient&no2bittbl[ClientNo])==0)&&
			(PSP[ClientNo].hp==0)){
			//�|�����ق��̃I�E�����������[�N��������
			UserWork[OUMU_WAZANO+0+clientno*2]=0;
			UserWork[OUMU_WAZANO+1+clientno*2]=0;
			UserWork[OUMU_HIT_WAZANO+0+clientno*4]=0;
			UserWork[OUMU_HIT_WAZANO+1+clientno*4]=0;
			UserWork[OUMU_HIT_WAZANO+2+clientno*4]=0;
			UserWork[OUMU_HIT_WAZANO+3+clientno*4]=0;
			ServerStatusFlag|=(no2bittbl[ClientNo]<<28);
			PushAdrsSet(WazaSeqAdrs+7);
			WazaSeqAdrs=adr;
			if(MineEnemyCheck(ClientNo)==SIDE_MINE){
				ServerStatusFlag|=SERVER_STATUS_FLAG_MINE_KIZETSU;
				if(FTW.MineKizetsuCount<0xff)	//�|�P�������m���ɂȂ�����
					FTW.MineKizetsuCount++;		//�|�P�������m���ɂȂ�����
				if(PSP[clientno].level>PSP[ClientNo].level){
					if(PSP[clientno].level-PSP[ClientNo].level>=30)
						FriendCalc(&PokeParaMine[SelMonsNo[ClientNo]],FRIEND_LEVEL30_HINSHI);
					else
						FriendCalc(&PokeParaMine[SelMonsNo[ClientNo]],FRIEND_HINSHI);
				}
			}
			else{
				if(FTW.EnemyKizetsuCount<0xff)	//�퓬�œ|�����|�P�����̐�
					FTW.EnemyKizetsuCount++;	//�퓬�œ|�����|�P�����̐�
				FTW.EnemyLastPokeNo=PSP[ClientNo].monsno;
											//����̍Ō�̃|�P�����i���o�[
			}
			if((ServerStatusFlag&SERVER_STATUS_FLAG_MICHIDURE)&&
			   (PSP[AttackClient].hp!=0)){
				PushAdrsSet(WazaSeqAdrs);
				WazaDamage=PSP[clientno].hp;
				WazaSeqAdrs=(u8 *)&WAZA_Michidure;
			}
			if((WazaKouka[DefenceClient]&WAZAKOUKA_ONNEN)&&
				//�݂��Âꂳ��Ȃ��U���ɂ́A����˂�́A�������Ȃ��i�t���O�͋��p�j
			  ((ServerStatusFlag&SERVER_STATUS_FLAG_NO_MICHIDURE)==0)&&
				//�������m�ł͂���˂�́A�������Ȃ�
			   (MineEnemyCheck(AttackClient)!=MineEnemyCheck(DefenceClient))&&
			   //���΂��n�̂Ƃ��ɂ͔������Ȃ�
			   (PSP[AttackClient].hp!=0)&&
			   //�u��邠�����v�ɂ���˂�͔������Ȃ�
			   (WazaNo!=WAZANO_WARUAGAKI)){
				no=UserWork[WAZA_POS_WORK+AttackClient];
				PSP[AttackClient].pp[no]=0;
				PushAdrsSet(WazaSeqAdrs);
				WazaSeqAdrs=(u8 *)&WAZA_Onnen;
				ClientNo=AttackClient;
				BSS_POKEPARA_PUT_SET(BSS_SERVER,
									BSS_PARA_PP1+no,
									BSS_SELMONSNO,
									1,
									(u8 *)&PSP[ClientNo].pp[no]);
				ClientBitSet(ClientNo);
				EvStrBuf0[0]=I_MSG;
				EvStrBuf0[1]=EV_WAZANO;
				EvStrBuf0[2]=PSP[AttackClient].waza[no]&0xff;
				EvStrBuf0[3]=(PSP[AttackClient].waza[no]&0xff00)>>8;
				EvStrBuf0[4]=EOM_;
			}
		}
		else
			WazaSeqAdrs+=7;
	}
}

void	WS_KIZETSU(void)
{
	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else
			ClientNo=DefenceClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
		BSS_POKE_KIZETSU_SET(BSS_SERVER);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=2;
	}
}

void	WS_KIZETSU_WAIT(void)
{
	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else
			ClientNo=DefenceClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
		PSP[ClientNo].condition=0;
		BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 BSS_PARA_CONDITION,
							 BSS_SELMONSNO,
							 4,
							 (u8 *)&PSP[ClientNo].condition);
		ClientBitSet(ClientNo);
		ServerKizetsuWorkInit();
		WazaSeqAdrs+=2;
	}
}

void	WS_CONDITION_CHECK(void)
{
	u8	*adrs;
	u8	clientno;
	u32	condition;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		clientno=AttackClient;
	else
		clientno=DefenceClient;
#else
	clientno=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	condition=((WazaSeqAdrs[2]<< 0)+
			   (WazaSeqAdrs[3]<< 8)+
			   (WazaSeqAdrs[4]<<16)+
			   (WazaSeqAdrs[5]<<24));

	adrs=(u8 *)((WazaSeqAdrs[6]<< 0)+
			    (WazaSeqAdrs[7]<< 8)+
			    (WazaSeqAdrs[8]<<16)+
			    (WazaSeqAdrs[9]<<24));

	if((PSP[clientno].condition&condition)&&
	   (PSP[clientno].hp!=0))
		WazaSeqAdrs=adrs;
	else
		WazaSeqAdrs+=10;
}

void	WS_CONDITION2_CHECK(void)
{
	u8	*adrs;
	u8	clientno;
	u32	condition;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		clientno=AttackClient;
	else
		clientno=DefenceClient;
#else
	clientno=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	condition=((WazaSeqAdrs[2]<< 0)+
			   (WazaSeqAdrs[3]<< 8)+
			   (WazaSeqAdrs[4]<<16)+
			   (WazaSeqAdrs[5]<<24));

	adrs=(u8 *)((WazaSeqAdrs[6]<< 0)+
			    (WazaSeqAdrs[7]<< 8)+
			    (WazaSeqAdrs[8]<<16)+
			    (WazaSeqAdrs[9]<<24));

	if((PSP[clientno].condition2&condition)&&
	   (PSP[clientno].hp!=0))
		WazaSeqAdrs=adrs;
	else
		WazaSeqAdrs+=10;
}

void	WS_SPEABI_CHECK(void)
{
	u8	*adrs;
	u8	clientno;
	u8	speabi;

	speabi=WazaSeqAdrs[2];
	adrs=(u8 *)((WazaSeqAdrs[3]<< 0)+
			    (WazaSeqAdrs[4]<< 8)+
			    (WazaSeqAdrs[5]<<16)+
			    (WazaSeqAdrs[6]<<24));

	if(WazaSeqAdrs[1]==SIDE_MINE_BENCH){
		clientno=SpecialAbirityCheck(SPEABI_MYHAVE_SPEABI,AttackClient,speabi,0,0);
		if(clientno){
			SpeabiNo=speabi;
			WazaSeqAdrs=adrs;
			TokusyuNoSet(clientno-1,SpeabiNo);
			UserWork[SPEABI_CLIENT]=clientno-1;
		}
		else
			WazaSeqAdrs+=7;
	}
	else if(WazaSeqAdrs[1]==SIDE_ENEMY_BENCH){
		clientno=SpecialAbirityCheck(SPEABI_ENEMYHAVE_SPEABI,AttackClient,speabi,0,0);
		if(clientno){
			SpeabiNo=speabi;
			WazaSeqAdrs=adrs;
			TokusyuNoSet(clientno-1,SpeabiNo);
			UserWork[SPEABI_CLIENT]=clientno-1;
		}
		else
			WazaSeqAdrs+=7;
	}
	else{
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			clientno=AttackClient;
		else
			clientno=DefenceClient;
#else
		clientno=SideClientNoGet(WazaSeqAdrs[1]);
#endif
		if(PSP[clientno].speabino==speabi){
			SpeabiNo=PSP[clientno].speabino;
			WazaSeqAdrs=adrs;
			TokusyuNoSet(clientno,SpeabiNo);
			UserWork[SPEABI_CLIENT]=clientno;
		}
		else
			WazaSeqAdrs+=7;
	}
}

void	WS_SIDECONDITION_CHECK(void)
{
	u8	*adrs;
	u8	side;
	u16	cond;

	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		side=ClientTypeGet(AttackClient)&1;
	else
		side=ClientTypeGet(DefenceClient)&1;

	cond=WazaSeqAdrs[2]|(WazaSeqAdrs[3]<<8);
	adrs=(u8 *)((WazaSeqAdrs[4]<< 0)+
			    (WazaSeqAdrs[5]<< 8)+
			    (WazaSeqAdrs[6]<<16)+
			    (WazaSeqAdrs[7]<<24));

	if(SideCondition[side]&cond)
		WazaSeqAdrs=adrs;
	else
		WazaSeqAdrs+=8;
}

void	WS_CONDCHG_CHECK(void)
{
	u8	clientno;
	u8	data;
	u8	ret=0;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		clientno=AttackClient;
	else
		clientno=DefenceClient;
#else
	clientno=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	data=PSP[clientno].abiritycnt[WazaSeqAdrs[3]];

	switch(WazaSeqAdrs[2]){
		case IFEQ:
			if(data==WazaSeqAdrs[4])
				ret++;
			break;
		case IFNE:
			if(data!=WazaSeqAdrs[4])
				ret++;
			break;
		case IFC:
			if(data>WazaSeqAdrs[4])
				ret++;
			break;
		case IFNC:
			if(data<WazaSeqAdrs[4])
				ret++;
			break;
		case IFBIT:
			if(data&WazaSeqAdrs[4])
				ret++;
			break;
		case IFNBIT:
			if((data&WazaSeqAdrs[4])==0)
				ret++;
			break;
	}
	if(ret)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[5]<< 0)+
						   (WazaSeqAdrs[6]<< 8)+
						   (WazaSeqAdrs[7]<<16)+
						   (WazaSeqAdrs[8]<<24));
	else
		WazaSeqAdrs+=9;

}

void	WS_WAZAKOUKA_CHECK(void)
{
	u8	*adrs;
	u32	cond;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	cond=(WazaSeqAdrs[2]<< 0)+
 	     (WazaSeqAdrs[3]<< 8)+
		 (WazaSeqAdrs[4]<<16)+
		 (WazaSeqAdrs[5]<<24);

	adrs=(u8 *)((WazaSeqAdrs[7]<< 0)+
			    (WazaSeqAdrs[8]<< 8)+
			    (WazaSeqAdrs[9]<<16)+
			    (WazaSeqAdrs[10]<<24));

	if(WazaSeqAdrs[6]){
		if(WazaKouka[ClientNo]&cond)
			WazaSeqAdrs+=11;
		else
			WazaSeqAdrs=adrs;
	}
	else{
		if(WazaKouka[ClientNo]&cond)
			WazaSeqAdrs=adrs;
		else
			WazaSeqAdrs+=11;
	}
}

void	WS_CHECK_TYPE(void)
{
	u8	*adrs;
	u8	clientno;
	u8	type;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		clientno=AttackClient;
	else
		clientno=DefenceClient;
#else
	clientno=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	type=WazaSeqAdrs[2];
	adrs=(u8 *)((WazaSeqAdrs[3]<< 0)+
			    (WazaSeqAdrs[4]<< 8)+
			    (WazaSeqAdrs[5]<<16)+
			    (WazaSeqAdrs[6]<<24));

	if((PSP[clientno].type1==type)||(PSP[clientno].type2==type))
		WazaSeqAdrs=adrs;
	else
		WazaSeqAdrs+=7;
}

#if 0
void	WS_GET_EXP_CHECK(void)
{
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[2]<< 0)+
			    (WazaSeqAdrs[3]<< 8)+
			    (WazaSeqAdrs[4]<<16)+
			    (WazaSeqAdrs[5]<<24));

	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;

	if((MineEnemyCheck(ClientNo)!=SIDE_MINE)||
		(FightType&FIGHT_TYPE_TUUSHIN)||(WazaNo==120))
		WazaSeqAdrs=adrs;
	else
		WazaSeqAdrs+=6;
}

void	WS_GET_EXP(void)
{
	u8	*adrs;
	u8	KizetsuClientNo;
	u8	level;


	if(WazaSeqAdrs[1]==SIDE_ATTACK){
		ClientNo=AttackClient;
		KizetsuClientNo=DefenceClient;
	}
	else{
		ClientNo=DefenceClient;
		KizetsuClientNo=AttackClient;
	}

	adrs=(u8 *)((WazaSeqAdrs[2]<< 0)+
			    (WazaSeqAdrs[3]<< 8)+
			    (WazaSeqAdrs[4]<<16)+
			    (WazaSeqAdrs[5]<<24));

	PokeParaMine[SelMonsNo[ClientNo]].PPP.exp+=(PPD[PSP[KizetsuClientNo].monsno].give_exp*
						PSP[KizetsuClientNo].level)/7;

	level=PokeLevelCalc(PSP[ClientNo].monsno,
						PokeParaMine[SelMonsNo[ClientNo]].PPP.exp);

	if(level!=PSP[ClientNo].level){
		PSP[ClientNo].level=level;
		level=0;
	}

	if(level==0)
		WazaSeqAdrs+=6;
	else
		WazaSeqAdrs=adrs;
}

void	WS_LEVEL_UP(void)
{
	u16	wazano;
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[2]<< 0)+
			    (WazaSeqAdrs[3]<< 8)+
			    (WazaSeqAdrs[4]<<16)+
			    (WazaSeqAdrs[5]<<24));

	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;

	PokeParaMine[SelMonsNo[ClientNo]].level=PSP[ClientNo].level;
	wazano=PokeParaMine[SelMonsNo[ClientNo]].hp;
	PokeParaCalc(&PokeParaMine[SelMonsNo[ClientNo]]);
	PokeParaMine[SelMonsNo[ClientNo]].hp+=wazano;

	GaugeTransChrSet(ClientGaugeSt[ClientNo],
				&PokeParaMine[SelMonsNo[ClientNo]], G_ALL);

	wazano=PokeWazaOboeCheck(&PokeParaMine[SelMonsNo[ClientNo]]);

	if(wazano!=0){
		WazaNo=wazano;
		WazaSeqAdrs+=6;
	}
	else
		WazaSeqAdrs=adrs;

	BSS_POKEPARA_GET_SET(BSS_SERVER,BSS_PARA_FIGHT,BSS_SELMONSNO);
	ClientBitSet(ClientNo);
}

void	WS_GET_WAZA(void)
{
	int	i;
	u8	*adrs;

	if(ClientBit==0){
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else
			ClientNo=DefenceClient;

		adrs=(u8 *)&PSP[ClientNo];

//		for(i=0;i<XtOffset(PokemonServerParam *,work);i++)
		for(i=0;i<sizeof(PokemonServerParam);i++)
			adrs[i]=ClientBuffer[ClientNo][i+3];

		WazaSeqAdrs+=2;
	}
}
#else

enum{
	GET_EXP_CHECK=0,
	GET_EXP_VALUE,
	GET_EXP_GAUGE_INIT,
	GET_EXP_GAUGE,
	GET_EXP_GAUGE_WAIT,
	GET_EXP_GAUGE_AFTER,
	GET_EXP_END,
};

void	WS_GET_EXP(void)
{
	int	i,j,k=0,flag;
	u16	*exp;
	u16	itemno;
	u16	totalexp;
	u8	eqp;

	exp=(u16 *)&UserWork[GETEXP_EXPTMP];
	
#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		KizetsuClient=AttackClient;
	else
		KizetsuClient=DefenceClient;
#else
	KizetsuClient=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	flag=FightJoinFlag[(KizetsuClient&2)>>1];

	switch(UserWork[GETEXP_SEQNO]){
		case GET_EXP_CHECK:
			if((MineEnemyCheck(KizetsuClient)!=SIDE_ENEMY)||
			(FightType&FIGHT_TYPE_NO_EXP))
				UserWork[GETEXP_SEQNO]=GET_EXP_END;
			else{
				UserWork[GETEXP_SEQNO]++;
				UserWork[EXP_SELMONS_BIT]|=no2bittbl[SelMonsNo[KizetsuClient]];
			}
			break;
		case GET_EXP_VALUE:
			j=0;
			for(i=0;i<6;i++){
				if((PokeParaGet(&PokeParaMine[i],ID_monsno))&&
				   (PokeParaGet(&PokeParaMine[i],ID_hp))){
					if(flag&no2bittbl[i]) j++;
					itemno=PokeParaGet(&PokeParaMine[i],ID_item);
					if(itemno==ITEM_NAZONOMI){
						eqp=Fld.RamSeed.item_equip;
					}
					else
						eqp=ItemEquipGet(itemno);
					if(eqp==SOUBI_KEIKENTIGET)
						k++;
				}
			}
			totalexp=((PPD[PSP[KizetsuClient].monsno].give_exp*PSP[KizetsuClient].level)/7);
			if(k){
				exp[0]=(totalexp/2)/j;
				if(exp[0]==0)
					exp[0]=1;
				GakushuuExp=(totalexp/2)/k;
				if(GakushuuExp==0)
					GakushuuExp=1;
			}
			else{
				exp[0]=totalexp/j;
				if(exp[0]==0)
					exp[0]=1;
				GakushuuExp=0;
			}

			UserWork[GETEXP_SEQNO]++;
			UserWork[GETEXP_SELMONS]=0;
			UserWork[FIGHT_JOIN_FLAG]=flag;
		case GET_EXP_GAUGE_INIT:
			if(ClientBit==0){
				itemno=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],
								   ID_item);
				if(itemno==ITEM_NAZONOMI)
					eqp=Fld.RamSeed.item_equip;
				else
					eqp=ItemEquipGet(itemno);
				if((eqp!=SOUBI_KEIKENTIGET)&&((UserWork[FIGHT_JOIN_FLAG]&1)==0)){
					UserWork[FIGHT_JOIN_FLAG]=UserWork[FIGHT_JOIN_FLAG]>>1;
					UserWork[GETEXP_SEQNO]=GET_EXP_GAUGE_AFTER;
					WazaDamage=0;
					break;
				}
#if 1
				if(PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_level)==100){
					UserWork[FIGHT_JOIN_FLAG]=UserWork[FIGHT_JOIN_FLAG]>>1;
					UserWork[GETEXP_SEQNO]=GET_EXP_GAUGE_AFTER;
					WazaDamage=0;
					break;
				}
#endif
				//�쐶�|�P�����Ƃ̐퓬�ł�
				//�o���l���炦�鎞�_�ŏ������y
				//�����������ɖ�肠��
				if(((FightType&FIGHT_TYPE_TRAINER)==0)&&
//					(WinLoseFlag==FIGHT_WIN)&&
					(PSP[0].hp)&&
				    (UserWork[JINGLE_FLAG]==0)){
					PinchSeStop();
					MusicPlay(MUS_KACHI2);
				    UserWork[JINGLE_FLAG]++;
				}
#if 0
				if((PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_hp))&&
				   (PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_level)!=100)){
#else
				if(PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_hp)){
#endif
					if(UserWork[FIGHT_JOIN_FLAG]&1)
						WazaDamage=exp[0];
					else
						WazaDamage=0;

					if(eqp==SOUBI_KEIKENTIGET)
						WazaDamage+=GakushuuExp;

					if(eqp==SOUBI_KEIKENTIHUERU)
						WazaDamage=WazaDamage*150/100;

					if(FightType&FIGHT_TYPE_TRAINER)
						WazaDamage=WazaDamage*150/100;

					if(PokeOneSelfCheck(&PokeParaMine[UserWork[GETEXP_SELMONS]])){
						WazaDamage=WazaDamage*150/100;
						i=KEIKENTIGETOOMEMSGNO;
					}
					else
						i=KEIKENTIGETNORMALMSGNO;

					if(FightType&FIGHT_TYPE_2vs2){
						if((SelMonsNo[2]==UserWork[GETEXP_SELMONS])&&
						  ((NoReshuffleClient&no2bittbl[2])==0))
							UserWork[GETEXP_CLIENTNO]=2;
						else if((NoReshuffleClient&no2bittbl[0])==0)
							UserWork[GETEXP_CLIENTNO]=0;
						else
							UserWork[GETEXP_CLIENTNO]=2;
					}
					else
						UserWork[GETEXP_CLIENTNO]=0;

					EvStrBuf0[0]=I_MSG;
					EvStrBuf0[1]=EV_NICKNAME;
					EvStrBuf0[2]=UserWork[GETEXP_CLIENTNO];
					EvStrBuf0[3]=UserWork[GETEXP_SELMONS];
					EvStrBuf0[4]=EV_MSGID;
					EvStrBuf0[5]=i&0xff;
					EvStrBuf0[6]=(i&0xff00)>>8;
					EvStrBuf0[7]=EOM_;

					EvStrBuf1[0]=I_MSG;
					EvStrBuf1[1]=EV_NUM;
					EvStrBuf1[2]=4;
					EvStrBuf1[3]=5;
					EvStrBuf1[4]=(WazaDamage&0x000000ff)>> 0;
					EvStrBuf1[5]=(WazaDamage&0x0000ff00)>> 8;
					EvStrBuf1[6]=(WazaDamage&0x00ff0000)>>16;
					EvStrBuf1[7]=(WazaDamage&0xff000000)>>24;
					EvStrBuf1[8]=EOM_;

//					i=FightMsgCreate((u8 *)&KeikentiGetMsg);
//					FightMsgServerSet(i);
					FightMsgServerSet(KEIKENTIGETMSGNO,UserWork[GETEXP_CLIENTNO]);
					ParaExpGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],PSP[KizetsuClient].monsno);
				}
				UserWork[FIGHT_JOIN_FLAG]=UserWork[FIGHT_JOIN_FLAG]>>1;
				UserWork[GETEXP_SEQNO]++;
			}
			break;
		case GET_EXP_GAUGE:
			if(ClientBit==0){
				ClientBuffer[UserWork[GETEXP_CLIENTNO]][0]=0;//�N���C�A���g����̕Ԃ�l���N���A
				if((PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_hp))&&
				   (PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_level)!=100)){
					PUSH_POKE_PARAM->hpmax=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],
													   ID_hpmax);
					PUSH_POKE_PARAM->pow=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],
													   ID_pow);
					PUSH_POKE_PARAM->def=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],
													   ID_def);
					PUSH_POKE_PARAM->agi=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],
													   ID_agi);
					PUSH_POKE_PARAM->spepow=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],
													   ID_spepow);
					PUSH_POKE_PARAM->spedef=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],
													   ID_spedef);
					ClientNo=UserWork[GETEXP_CLIENTNO];
					BSS_EXP_GAUGE_SET(BSS_SERVER,UserWork[GETEXP_SELMONS],WazaDamage);
					ClientBitSet(ClientNo);
				}
				UserWork[GETEXP_SEQNO]++;
			}
			break;
		case GET_EXP_GAUGE_WAIT:
			if(ClientBit==0){
				ClientNo=UserWork[GETEXP_CLIENTNO];
				if((ClientBuffer[ClientNo][0]==BSS_SELECT_RETURN)&&
				   (ClientBuffer[ClientNo][1]==BSS_SELECT_RETURN_LEVELUP)){
					if((FightType&FIGHT_TYPE_TRAINER)&&
					   (SelMonsNo[ClientNo]==UserWork[GETEXP_SELMONS]))
						PinchSeCheckPlay(&PokeParaMine[SelMonsNo[ClientNo]],ClientNo);
					EvStrBuf0[0]=I_MSG;
					EvStrBuf0[1]=EV_NICKNAME;
					EvStrBuf0[2]=ClientNo;
					EvStrBuf0[3]=UserWork[GETEXP_SELMONS];
					EvStrBuf0[4]=EOM_;
					EvStrBuf1[0]=I_MSG;
					EvStrBuf1[1]=EV_NUM;
					EvStrBuf1[2]=1;
					EvStrBuf1[3]=3;
					EvStrBuf1[4]=PokeParaGet(
								 &PokeParaMine[UserWork[GETEXP_SELMONS]],
								 ID_level);
					EvStrBuf1[5]=EOM_;

					PushAdrsAct();
					LevelUpBit|=no2bittbl[UserWork[GETEXP_SELMONS]];
					WazaSeqAdrs=(u8 *)&WAZA_LevelUp;
					WazaDamage=ClientBuffer[ClientNo][2]|
							  (ClientBuffer[ClientNo][3]<<8);
					FriendCalc(&PokeParaMine[UserWork[GETEXP_SELMONS]],FRIEND_LEVELUP);

					if((SelMonsNo[0]==UserWork[GETEXP_SELMONS])&&
					   (PSP[0].hp)){
						PSP[0].level=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_level);
						PSP[0].hp=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_hp);
						PSP[0].hpmax=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_hpmax);
						PSP[0].pow=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_pow);
						PSP[0].def=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_def);
						PSP[0].agi=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_agi);
						PSP[0].agi=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_agi);
						PSP[0].spepow=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_spepow);
						PSP[0].spedef=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_spedef);
					}
					if((SelMonsNo[2]==UserWork[GETEXP_SELMONS])&&
					   (PSP[2].hp)&&
					   (FightType&FIGHT_TYPE_2vs2)){
						PSP[2].level=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_level);
						PSP[2].hp=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_hp);
						PSP[2].hpmax=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_hpmax);
						PSP[2].pow=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_pow);
						PSP[2].def=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_def);
						PSP[2].agi=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_agi);
						PSP[2].agi=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_agi);
						PSP[2].spepow=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_spepow);
					}
				}
				else
					WazaDamage=0;
				UserWork[GETEXP_SEQNO]=GET_EXP_GAUGE_AFTER;
			}
			break;
		case GET_EXP_GAUGE_AFTER:
			if(WazaDamage!=0)
				UserWork[GETEXP_SEQNO]=GET_EXP_GAUGE;
			else{
				UserWork[GETEXP_SELMONS]++;
				if(UserWork[GETEXP_SELMONS]<6)
					UserWork[GETEXP_SEQNO]=GET_EXP_GAUGE_INIT;
				else{
//					PSP[ClientNo].exp+=exp[0];
					UserWork[GETEXP_SEQNO]=GET_EXP_END;
				}
			}
			break;
		case GET_EXP_END:
			if(ClientBit==0){
				PSP[KizetsuClient].item=0;
				PSP[KizetsuClient].speabino=0;
				WazaSeqAdrs+=2;
			}
			break;
	}
}
#endif

const	u8	bittbl[]={
	0x00,
	0x01,
	0x03,
	0x07,
	0x0f,
	0x1f,
	0x3f,
};

void	WS_ZENMETSU_CHECK(void)
{
	int	i;
	u16	hpwork=0;
	int	SideM,SideE;

	if(ClientBit==0){
		for(i=0;i<6;i++){
			if((PokeParaGet(&PokeParaMine[i],ID_monsno)&&
			   (PokeParaGet(&PokeParaMine[i],ID_tamago_flag)==0)))
				hpwork=hpwork+PokeParaGet(&PokeParaMine[i],ID_hp);
		}
		if(hpwork==0)
			WinLoseFlag|=FIGHT_LOSE;

		hpwork=0;
		for(i=0;i<6;i++){
			if((PokeParaGet(&PokeParaEnemy[i],ID_monsno)&&
			   (PokeParaGet(&PokeParaEnemy[i],ID_tamago_flag)==0)))
				hpwork=hpwork+PokeParaGet(&PokeParaEnemy[i],ID_hp);
		}
		if(hpwork==0)
			WinLoseFlag|=FIGHT_WIN;
		if((WinLoseFlag==0)&&(FightType&FIGHT_TYPE_TUUSHIN)){
			SideM=0;
			for(i=0;i<ClientSetMax;i+=2){
				if((ServerStatusFlag&(0x10000000)<<i)&&
				   (OSTF[i].reshuffleflag==0))
					SideM++;
			}
			SideE=0;
			for(i=1;i<ClientSetMax;i+=2){
				if((ServerStatusFlag&(0x10000000)<<i)&&
				   (OSTF[i].reshuffleflag==0))
					SideE++;
			}
			if(FightType&FIGHT_TYPE_4_2vs2){
				if((SideE+SideM)>1)
					WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
									   (WazaSeqAdrs[2]<< 8)+
									   (WazaSeqAdrs[3]<<16)+
									   (WazaSeqAdrs[4]<<24));
				else
					WazaSeqAdrs+=5;
			}
			else{
				if((SideE)&&(SideM))
					WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
									   (WazaSeqAdrs[2]<< 8)+
									   (WazaSeqAdrs[3]<<16)+
									   (WazaSeqAdrs[4]<<24));
				else
					WazaSeqAdrs+=5;
			}
		}
		else
			WazaSeqAdrs+=5;
	}
}

void	LoopInitAct(void)
{
	WazaStatusFlag=0;
	UserWork[DAMAGE_VALUE]=1;
	CriticalValue=1;	//�N���e�B�J���_���[�W���P�{�ɖ߂�
	ServerWork[3]=0;	//�ǉ����ʃ��[�N���N���A
	ServerWork[6]=0;	//NoHitMsg���[�N���N���A
	ServerStatusFlag&=SERVER_STATUS_FLAG_MICHIDURE_OFF;
	//�ǉ������t���O���N���A
	ServerStatusFlag&=SERVER_STATUS_FLAG_TSUIKA_OFF;
}

void	WS_LOOP_INIT(void)
{
	LoopInitAct();
	WazaSeqAdrs++;
}

void	WS_LOOPCNT(void)
{
	WazaSeqLoopCount=WazaSeqAdrs[1];
	WazaSeqAdrs+=2;
}

void	WS_LOOP(void)
{
	if(--WazaSeqLoopCount==0){
		WazaSeqAdrs+=5;
	}
	else{
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
						   (WazaSeqAdrs[2]<< 8)+
						   (WazaSeqAdrs[3]<<16)+
						   (WazaSeqAdrs[4]<<24));
	}
}

void	WS_JUMP(void)
{
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
			    (WazaSeqAdrs[2]<< 8)+
			    (WazaSeqAdrs[3]<<16)+
			    (WazaSeqAdrs[4]<<24));
	WazaSeqAdrs=adrs;
}

void	WS_IF8(void)
{
	u8	*adrs,*jump;
	u8	work;
	u8	flag;

	flag=WazaSeqAdrs[1];

	adrs=(u8 *)((WazaSeqAdrs[2]<< 0)+
			    (WazaSeqAdrs[3]<< 8)+
			    (WazaSeqAdrs[4]<<16)+
			    (WazaSeqAdrs[5]<<24));
	work=WazaSeqAdrs[6];

	jump=(u8 *)((WazaSeqAdrs[7]<< 0)+
			    (WazaSeqAdrs[8]<< 8)+
			    (WazaSeqAdrs[9]<<16)+
			    (WazaSeqAdrs[10]<<24));

	WazaSeqAdrs+=11;

	switch(flag){
		case FLAG_EQ:
			if(*adrs==work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NE:
			if(*adrs!=work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_C:
			if(*adrs>work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NC:
			if(*adrs<work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_BIT:
			if(*adrs&work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NBIT:
			if((*adrs&work)==0)
				WazaSeqAdrs=jump;
			break;
	}
}

void	WS_IF16(void)
{
	u16	*adrs;
	u8	*jump;
	u16	work;
	u8	flag;

	flag=WazaSeqAdrs[1];

	adrs=(u16 *)((WazaSeqAdrs[2]<< 0)+
			     (WazaSeqAdrs[3]<< 8)+
			     (WazaSeqAdrs[4]<<16)+
			     (WazaSeqAdrs[5]<<24));

	work=WazaSeqAdrs[6]|(WazaSeqAdrs[7]<<8);

	jump=(u8 *)((WazaSeqAdrs[8]<< 0)+
			    (WazaSeqAdrs[9]<< 8)+
			    (WazaSeqAdrs[10]<<16)+
			    (WazaSeqAdrs[11]<<24));

	WazaSeqAdrs+=12;

	switch(flag){
		case FLAG_EQ:
			if(*adrs==work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NE:
			if(*adrs!=work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_C:
			if(*adrs>work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NC:
			if(*adrs<work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_BIT:
			if(*adrs&work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NBIT:
			if((*adrs&work)==0)
				WazaSeqAdrs=jump;
			break;
	}
}

void	WS_IF32(void)
{
	u32	*adrs;
	u8	*jump;
	u32	work;
	u8	flag;

	flag=WazaSeqAdrs[1];

	adrs=(u32 *)((WazaSeqAdrs[2]<< 0)+
			     (WazaSeqAdrs[3]<< 8)+
			     (WazaSeqAdrs[4]<<16)+
			     (WazaSeqAdrs[5]<<24));

	work=WazaSeqAdrs[6]|
		(WazaSeqAdrs[7]<< 8)|
		(WazaSeqAdrs[8]<<16)|
		(WazaSeqAdrs[9]<<24);

	jump=(u8 *)((WazaSeqAdrs[10]<< 0)+
			    (WazaSeqAdrs[11]<< 8)+
			    (WazaSeqAdrs[12]<<16)+
			    (WazaSeqAdrs[13]<<24));

	WazaSeqAdrs+=14;

	switch(flag){
		case FLAG_EQ:
			if(*adrs==work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NE:
			if(*adrs!=work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_C:
			if(*adrs>work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NC:
			if(*adrs<work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_BIT:
			if(*adrs&work)
				WazaSeqAdrs=jump;
			break;
		case FLAG_NBIT:
			if((*adrs&work)==0)
				WazaSeqAdrs=jump;
			break;
	}
}

void	WS_IFWEQ(void)
{
	u8	*adrs1,*adrs2,*jump;
	u8	size,i;

	adrs1=(u8 *)((WazaSeqAdrs[1]<< 0)+
		  	     (WazaSeqAdrs[2]<< 8)+
			     (WazaSeqAdrs[3]<<16)+
			     (WazaSeqAdrs[4]<<24));
	adrs2=(u8 *)((WazaSeqAdrs[5]<< 0)+
		  	     (WazaSeqAdrs[6]<< 8)+
			     (WazaSeqAdrs[7]<<16)+
			     (WazaSeqAdrs[8]<<24));
	size=WazaSeqAdrs[9];
	jump=(u8 *)((WazaSeqAdrs[10]<< 0)+
			    (WazaSeqAdrs[11]<< 8)+
			    (WazaSeqAdrs[12]<<16)+
			    (WazaSeqAdrs[13]<<24));
	for(i=0;i<size;i++){
		if(*adrs1!=*adrs2){
			WazaSeqAdrs+=14;
			break;
		}
		adrs1++;
		adrs2++;
	}
	if(i==size)
		WazaSeqAdrs=jump;
}

void	WS_IFWNE(void)
{
	u8	*adrs1,*adrs2,*jump;
	u8	size,i,j=0;

	adrs1=(u8 *)((WazaSeqAdrs[1]<< 0)+
		  	     (WazaSeqAdrs[2]<< 8)+
			     (WazaSeqAdrs[3]<<16)+
			     (WazaSeqAdrs[4]<<24));
	adrs2=(u8 *)((WazaSeqAdrs[5]<< 0)+
		  	     (WazaSeqAdrs[6]<< 8)+
			     (WazaSeqAdrs[7]<<16)+
			     (WazaSeqAdrs[8]<<24));
	size=WazaSeqAdrs[9];
	jump=(u8 *)((WazaSeqAdrs[10]<< 0)+
			    (WazaSeqAdrs[11]<< 8)+
			    (WazaSeqAdrs[12]<<16)+
			    (WazaSeqAdrs[13]<<24));
	for(i=0;i<size;i++){
		if(*adrs1==*adrs2) j++;
		adrs1++;
		adrs2++;
	}
	if(j!=size)
		WazaSeqAdrs=jump;
	else
		WazaSeqAdrs+=14;
}

void	WS_SETVALUE(void)
{
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
			    (WazaSeqAdrs[2]<< 8)+
			    (WazaSeqAdrs[3]<<16)+
			    (WazaSeqAdrs[4]<<24));

	*adrs=WazaSeqAdrs[5];

	WazaSeqAdrs+=6;
}

void	WS_ADDVALUE(void)
{
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
			    (WazaSeqAdrs[2]<< 8)+
			    (WazaSeqAdrs[3]<<16)+
			    (WazaSeqAdrs[4]<<24));

	adrs[0]+=WazaSeqAdrs[5];

	WazaSeqAdrs+=6;
}

void	WS_SUBVALUE(void)
{
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
			    (WazaSeqAdrs[2]<< 8)+
			    (WazaSeqAdrs[3]<<16)+
			    (WazaSeqAdrs[4]<<24));

	adrs[0]-=WazaSeqAdrs[5];

	WazaSeqAdrs+=6;
}

void	WS_SETWORK(void)
{
	int	i;
	u8	*dest;
	u8	*src;
	u8	size;

	dest=(u8 *)((WazaSeqAdrs[1]<< 0)+
			    (WazaSeqAdrs[2]<< 8)+
			    (WazaSeqAdrs[3]<<16)+
			    (WazaSeqAdrs[4]<<24));
	src=(u8 *)((WazaSeqAdrs[5]<< 0)+
		       (WazaSeqAdrs[6]<< 8)+
		       (WazaSeqAdrs[7]<<16)+
			   (WazaSeqAdrs[8]<<24));
	size=WazaSeqAdrs[9];
	
	for(i=0;i<size;i++)
		dest[i]=src[i];

	WazaSeqAdrs+=10;
}

void	WS_SETWORKOFS(void)
{
	int	i;
	u8	*dest;
	u8	*src;
	u8	*ofs;
	u8	size;

	dest=(u8 *)((WazaSeqAdrs[1]<< 0)+
			    (WazaSeqAdrs[2]<< 8)+
			    (WazaSeqAdrs[3]<<16)+
			    (WazaSeqAdrs[4]<<24));
	src=(u8 *)((WazaSeqAdrs[5]<< 0)+
		       (WazaSeqAdrs[6]<< 8)+
		       (WazaSeqAdrs[7]<<16)+
			   (WazaSeqAdrs[8]<<24));
	ofs=(u8 *)((WazaSeqAdrs[9]<< 0)+
		       (WazaSeqAdrs[10]<< 8)+
		       (WazaSeqAdrs[11]<<16)+
			   (WazaSeqAdrs[12]<<24));
	size=WazaSeqAdrs[13];
	
	for(i=0;i<size;i++)
		dest[i]=src[i+ofs[0]];

	WazaSeqAdrs+=14;
}

void	WS_SET8BIT(void)
{
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
			    (WazaSeqAdrs[2]<< 8)+
			    (WazaSeqAdrs[3]<<16)+
			    (WazaSeqAdrs[4]<<24));

	*adrs|=WazaSeqAdrs[5];

	WazaSeqAdrs+=6;
}

void	WS_SET16BIT(void)
{
	u16	*adrs;

	adrs=(u16 *)((WazaSeqAdrs[1]<< 0)+
			     (WazaSeqAdrs[2]<< 8)+
			     (WazaSeqAdrs[3]<<16)+
			     (WazaSeqAdrs[4]<<24));

	*adrs|=(WazaSeqAdrs[5]|(WazaSeqAdrs[6]<<8));

	WazaSeqAdrs+=7;
}

void	WS_SET32BIT(void)
{
	u32	*adrs;

	adrs=(u32 *)((WazaSeqAdrs[1]<< 0)+
			     (WazaSeqAdrs[2]<< 8)+
			     (WazaSeqAdrs[3]<<16)+
			     (WazaSeqAdrs[4]<<24));

	*adrs|=((WazaSeqAdrs[5]<< 0)+
			(WazaSeqAdrs[6]<< 8)+
			(WazaSeqAdrs[7]<<16)+
			(WazaSeqAdrs[8]<<24));

	WazaSeqAdrs+=9;
}

void	WS_RESET8BIT(void)
{
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[1]<< 0)+
			    (WazaSeqAdrs[2]<< 8)+
			    (WazaSeqAdrs[3]<<16)+
			    (WazaSeqAdrs[4]<<24));

	*adrs&=(WazaSeqAdrs[5]^0xff);

	WazaSeqAdrs+=6;
}

void	WS_RESET16BIT(void)
{
	u16	*adrs;

	adrs=(u16 *)((WazaSeqAdrs[1]<< 0)+
			     (WazaSeqAdrs[2]<< 8)+
			     (WazaSeqAdrs[3]<<16)+
			     (WazaSeqAdrs[4]<<24));

	*adrs&=((WazaSeqAdrs[5]|(WazaSeqAdrs[6]<<8))^0xffff);

	WazaSeqAdrs+=7;
}

void	WS_RESET32BIT(void)
{
	u32	*adrs;

	adrs=(u32 *)((WazaSeqAdrs[1]<< 0)+
			     (WazaSeqAdrs[2]<< 8)+
			     (WazaSeqAdrs[3]<<16)+
			     (WazaSeqAdrs[4]<<24));

	*adrs&=(((WazaSeqAdrs[5]<< 0)+
		 	 (WazaSeqAdrs[6]<< 8)+
			 (WazaSeqAdrs[7]<<16)+
			 (WazaSeqAdrs[8]<<24))^0xffffffff);

	WazaSeqAdrs+=9;
}

void	WS_WAIT(void)
{
	u16	cnt;

	if(ClientBit==0){
		cnt=WazaSeqAdrs[1]|(WazaSeqAdrs[2]<<8);
		MojiWaitCount++;
		if(MojiWaitCount>=cnt){
			MojiWaitCount=0;
			WazaSeqAdrs+=3;
		}
	}
}

void	WS_CLIENT_BIT_WAIT(void)
{
	if(ClientBit==0)
		WazaSeqAdrs++;
}

void	WS_HP_GAUGE_REDRAW(void)
{
	if(WazaSeqAdrs[1]==0) ClientNo=DefenceClient;
	else				 	  ClientNo=AttackClient;
	BSS_HP_GAUGE_SET(BSS_SERVER,WazaDamage);
	ClientBitSet(ClientNo);
	WazaSeqAdrs+=2;
}

void	WS_SEQRET(void)
{
//	WazaSeqAdrs=RetWazaSeqAdrs;
	PopAdrsAct();
}

void	WS_WAZAEND(void)
{
#if 0
	u8	clientno;

//	if(WazaKoukaCheck()) return;
	if((ServerStatusFlag&SERVER_STATUS_FLAG_NO_DOUBLE_CHECK)==0){
		if((FightType&FIGHT_TYPE_2vs2)&&
//		   (WTD[WazaNoTmp].attackrange==RANGE_DOUBLE)&&
		   (OTF[AttackClient].tamewazaflag==0)&&
		   (WTD[WazaNo].attackrange==RANGE_DOUBLE)&&
		  ((ServerStatusFlag&SERVER_STATUS_FLAG_DOUBLE_CHECK)==0)){
//			DefenceClient=ClientNoGet((ClientTypeGet(DefenceClient)&1)^2);
			clientno=ClientNoGet(ClientTypeGet(DefenceClient)^2);
			if(PSP[clientno].hp!=0){
				DefenceClient=clientno;
				ServerStatusFlag|=SERVER_STATUS_FLAG_DOUBLE_CHECK;
				UserWork[WKC_WORK0]=0;
				WazaStatusFlag=0;
//				WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNoTmp].battleeffect];
				WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
				return;
			}
			else
				ServerStatusFlag|=SERVER_STATUS_FLAG_DOUBLE_CHECK;
		}
	}
#endif
//	ServerStatusFlag&=SERVER_STATUS_FLAG_DOUBLE_CHECK_OFF;
//	ServerStatusFlag&=SERVER_STATUS_FLAG_NO_DOUBLE_CHECK_OFF;
	WazaStatusFlag=0;				//�Z�X�e�[�^�X�t���O��������
	ClientNo=0;
	ActionSeqNo=SCA_EndCheckNo;
}

void	WS_SEQEND(void)
{
	ClientNo=0;
	ActionSeqNo=SCA_EndCheckNo;
}

void	WS_SPEABIEND(void)
{
//	WazaSeqAdrs=RetSpeabiSeqAdrs;
	PopAdrsAct();
	if(PUSH_SERVER_ADRS->PushServerAdrsCnt)
		PUSH_SERVER_ADRS->PushServerAdrsCnt--;
	ServerSequence=
		PUSH_SERVER_ADRS->
		RetServerSequence[PUSH_SERVER_ADRS->PushServerAdrsCnt];
}


void	WS_GOSUB(void)
{
	PushAdrsSet(WazaSeqAdrs+5);
	WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
					   WazaSeqAdrs[2]<< 8|
					   WazaSeqAdrs[3]<<16|
					   WazaSeqAdrs[4]<<24);
}

void	WS_MONS_TYPE(void)
{
	u8	clientno;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		clientno=AttackClient;
	else
		clientno=DefenceClient;
#else
	clientno=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	if((WazaSeqAdrs[2]==PSP[clientno].type1)||
	   (WazaSeqAdrs[2]==PSP[clientno].type2)){
		WazaSeqAdrs=(u8 *)(WazaSeqAdrs[3]<< 0|
						   WazaSeqAdrs[4]<< 8|
						   WazaSeqAdrs[5]<<16|
						   WazaSeqAdrs[6]<<24);
	}
	else
		WazaSeqAdrs+=7;
}

void	WS_SPEABI_HAVE_CHECK(void)
{
	if(SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,WazaSeqAdrs[1],0,0))
		WazaSeqAdrs=(u8 *)(WazaSeqAdrs[2]<< 0|
						   WazaSeqAdrs[3]<< 8|
						   WazaSeqAdrs[4]<<16|
						   WazaSeqAdrs[5]<<24);
	else
		WazaSeqAdrs+=6;
}

void	WS_ALERTEND(void)
{
	UserWork[ALERT_END+AttackClient]=1;
}

void	WS_FIGHT_EFFECT(void)
{
	u16	*adrs;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
		ClientNo=DefenceClient;
	else if(WazaSeqAdrs[1]==SIDE_FIELD)
		ClientNo=0;
	else if(WazaSeqAdrs[1]==SIDE_CLIENTNOWORK)
		ClientNo=UserWork[CLIENT_NO_WORK];
	else
		ClientNo=TsuikaClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	adrs=(u16 *)((WazaSeqAdrs[3]<< 0)+
			     (WazaSeqAdrs[4]<< 8)+
			     (WazaSeqAdrs[5]<<16)+
			     (WazaSeqAdrs[6]<<24));

	if((WazaSeqAdrs[2]==FE_ID_STATUS_UPDOWN)||
	   (WazaSeqAdrs[2]==FE_ID_YOKODORI)||
	   (WazaSeqAdrs[2]==FE_ID_MIGAWARI_KIZETU)){
		BSS_FIGHT_EFFECT_SET(BSS_SERVER,WazaSeqAdrs[2],adrs[0]);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=7;
	}
	else if(ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF){
		PushAdrsSet(WazaSeqAdrs+7);
		WazaSeqAdrs=(u8 *)&WAZA_Wait;
	}
	else if((WazaSeqAdrs[2]==FE_ID_AMAGOI)||
			(WazaSeqAdrs[2]==FE_ID_NIHONBARE)||
			(WazaSeqAdrs[2]==FE_ID_SUNAARASI)||
			(WazaSeqAdrs[2]==FE_ID_ARARE)){
		BSS_FIGHT_EFFECT_SET(BSS_SERVER,WazaSeqAdrs[2],adrs[0]);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=7;
	}
	else if(WazaKouka[ClientNo]&WAZAKOUKA_KIE)
		WazaSeqAdrs+=7;
	else{
		BSS_FIGHT_EFFECT_SET(BSS_SERVER,WazaSeqAdrs[2],adrs[0]);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=7;
	}
}

void	WS_FIGHT_EFFECT2(void)
{
	u8	*adrs1;
	u16	*adrs2;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
		ClientNo=DefenceClient;
	else if(WazaSeqAdrs[1]==SIDE_FIELD)
		ClientNo=0;
	else
		ClientNo=TsuikaClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	adrs1=(u8 *)((WazaSeqAdrs[2]<< 0)+
		 	     (WazaSeqAdrs[3]<< 8)+
			     (WazaSeqAdrs[4]<<16)+
			     (WazaSeqAdrs[5]<<24));
	adrs2=(u16 *)((WazaSeqAdrs[6]<< 0)+
		 	      (WazaSeqAdrs[7]<< 8)+
			      (WazaSeqAdrs[8]<<16)+
			      (WazaSeqAdrs[9]<<24));

	if((adrs1[0]==FE_ID_STATUS_UPDOWN)||
	   (adrs1[0]==FE_ID_YOKODORI)||
	   (adrs1[0]==FE_ID_MIGAWARI_KIZETU)){
		BSS_FIGHT_EFFECT_SET(BSS_SERVER,adrs1[0],adrs2[0]);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=10;
	}
	else if(ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF)
		WazaSeqAdrs+=10;
	else if((adrs1[0]==FE_ID_AMAGOI)||
			(adrs1[0]==FE_ID_NIHONBARE)||
			(adrs1[0]==FE_ID_SUNAARASI)||
			(adrs1[0]==FE_ID_ARARE)){
		BSS_FIGHT_EFFECT_SET(BSS_SERVER,adrs1[0],adrs2[0]);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=10;
	}
	else if(WazaKouka[ClientNo]&WAZAKOUKA_KIE)
		WazaSeqAdrs+=10;
	else{
		BSS_FIGHT_EFFECT_SET(BSS_SERVER,adrs1[0],adrs2[0]);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=10;
	}
}

void	WS_COND_CHG_WORK_SET(void)
{
	int	para=0;

	switch(UserWork[COND_CHG_WORK]&0xf0){
		case PARA_RANKU1:
			para=COND2_POWUP;
			break;
		case PARA_RANKU2:
			para=COND2_POWUP2;
			break;
		case PARA_RANKD1:
			para=COND2_POWDOWN;
			break;
		case PARA_RANKD2:
			para=COND2_POWDOWN2;
			break;
	}
	UserWork[FIGHT_EFFECT_WORK+0]=
		para+(UserWork[COND_CHG_WORK]&0x0f)-1;
	UserWork[FIGHT_EFFECT_WORK+1]=0;
	WazaSeqAdrs++;
}

void	WS_COND_CHG_BIT_CHECK(void)
{
	int	i=0;
	u16	para=0;
	u16	paratmp;
	int	cnt=0;
	u8	bit;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
		ClientNo=DefenceClient;
	else
		ClientNo=TsuikaClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	bit=WazaSeqAdrs[2];

	if(WazaSeqAdrs[3]&COND_FLAG_DOWN){
		if(WazaSeqAdrs[3]&COND_FLAG_DOUBLE)
			paratmp=COND2_POWDOWN2-1;
		else
			paratmp=COND2_POWDOWN-1;
		while(bit){
			if(bit&1){
				if(WazaSeqAdrs[3]&COND_FLAG_NO_TOKUSEI){
					if(PSP[ClientNo].abiritycnt[i]>0){
						para=paratmp+i;
						cnt++;
					}
				}
				else{
					//���낢����Ɏ���Ă�������ʂȂ�
					if((SCC[ClientTypeGet(ClientNo)&1].shiroikiricnt==0)&&
					//����\�̓N���A�{�f�B�A���낢���ނ�͔\�͒ቺ�������Ȃ�
					   (PSP[ClientNo].speabino!=TOKUSYU_KURIABODHI)&&
					   (PSP[ClientNo].speabino!=TOKUSYU_SIROIKEMURI)&&
					//����\�͂���ǂ��߂͖�������������Ȃ�
					  ((PSP[ClientNo].speabino!=TOKUSYU_SURUDOIME)||(i!=COND_HIT))&&
					//����\�͂����肫�o�T�~�͍U���͂�������Ȃ�
					  ((PSP[ClientNo].speabino!=TOKUSYU_KAIRIKIBASAMI)||(i!=COND_POW))&&
					   (PSP[ClientNo].abiritycnt[i]>0)){
						para=paratmp+i;
						cnt++;
					}
				}
			}
			bit=bit>>1;
			i++;
		}
		if(cnt>1){
			if(WazaSeqAdrs[3]&COND_FLAG_DOUBLE)
				para=COND2_MULTIDOWN2;
			else
				para=COND2_MULTIDOWN;
		}
	}
	else{
		if(WazaSeqAdrs[3]&COND_FLAG_DOUBLE)
			paratmp=COND2_POWUP2-1;
		else
			paratmp=COND2_POWUP-1;
		while(bit){
			if(bit&1){
				if(PSP[ClientNo].abiritycnt[i]<12){
					para=paratmp+i;
					cnt++;
				}
			}
			bit=bit>>1;
			i++;
		}
		if(cnt>1){
			if(WazaSeqAdrs[3]&COND_FLAG_DOUBLE)
				para=COND2_MULTIUP2;
			else
				para=COND2_MULTIUP;
		}
	}

	if((WazaSeqAdrs[3]&COND_FLAG_MULTI)&&(cnt<2))
		WazaSeqAdrs+=4;
	else if((cnt)&&(UserWork[COND_CHG_MULTI]==0)){
		BSS_FIGHT_EFFECT_SET(BSS_SERVER,FE_ID_STATUS_UPDOWN,para);
		ClientBitSet(ClientNo);
		if((WazaSeqAdrs[3]&COND_FLAG_MULTI)&&(cnt>1))
			UserWork[COND_CHG_MULTI]=1;
		WazaSeqAdrs+=4;
	}
	else
		WazaSeqAdrs+=4;
}

void	WS_WKC_ACT(void)
{
	int	i;
	u8	clientno;
	u8	ret=0;
	u8	wazatype;
	u8	atkeqp;
	u16	*waza;
	u16	*item=0;
	u8	flag;
	u8	param;

	flag=WazaSeqAdrs[1];
	param=WazaSeqAdrs[2];

	if(PSP[AttackClient].item==ITEM_NAZONOMI)
		atkeqp=FRS[AttackClient].item_equip;
	else
		atkeqp=ItemEquipGet(PSP[AttackClient].item);

	waza=(u16 *)&UserWork[KODAWARI_WAZANO+AttackClient*2];

	if(UserWork[DAMAGE_TYPE_WORK])
		wazatype=UserWork[DAMAGE_TYPE_WORK]&0x3f;
	else
		wazatype=WTD[WazaNo].wazatype;

	do{
		switch(UserWork[WKC_WORK0]){
			case WKC_IKARI_CHECK:
				if((PSP[DefenceClient].condition2&CONDITION2_IKARI)&&
				   (PSP[DefenceClient].hp!=0)){	//������`�F�b�N
					if((AttackClient!=DefenceClient)&&
					   (MineEnemyCheck(AttackClient)!=MineEnemyCheck(DefenceClient))&&
					  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
					  ((OTF[DefenceClient].butsurihitdamage)||
					   (OTF[DefenceClient].tokusyuhitdamage))&&
					   (WTD[WazaNo].damage)){
						if(PSP[DefenceClient].abiritycnt[COND_POW]<12){
							PSP[DefenceClient].abiritycnt[COND_POW]++;
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Ikari;
							ret=1;
						}
					}
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_TOKERU_CHECK:
				if((PSP[DefenceClient].condition&CONDITION_KOORI)&&
				   (PSP[DefenceClient].hp!=0)){	//�Ƃ���`�F�b�N
					if((AttackClient!=DefenceClient)&&
					   (OSTF[DefenceClient].tokusyuhitdamage)&&
					  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
						if(wazatype==FIRE_TYPE){
							PSP[DefenceClient].condition&=CONDITION_KOORI_OFF;
							ClientNo=DefenceClient;
							BSS_POKEPARA_PUT_SET(BSS_SERVER,
												BSS_PARA_CONDITION,
												BSS_SELMONSNO,
												4,
												(u8 *)&PSP[DefenceClient].condition);
							ClientBitSet(ClientNo);
							PushAdrsAct();
							WazaSeqAdrs=(u8 *)&WAZA_Melt2;
							ret=1;
						}
					}
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_SPEABI_SINKURO_DEFENCE:
				if(SpecialAbirityCheck(SPEABI_SINKURO_DEFENCE,DefenceClient,0,0,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
			case WKC_SPEABI_CHECK:
				if(SpecialAbirityCheck(SPEABI_WAZA_HIT_AFTER,DefenceClient,0,0,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
			case WKC_SPEABI_CHECK_ALL:
				if(SpecialAbirityCheck(SPEABI_WAZA_HIT_AFTER_ALL,0,0,0,0))
					ret=1;
				else
					UserWork[WKC_WORK0]++;
				break;
			case WKC_SPEABI_SINKURO_ATTACK:
				if(SpecialAbirityCheck(SPEABI_SINKURO_ATTACK,AttackClient,0,0,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
//�ǂ�ڂ������A�C�e���������ƌ���悤�ɂ��邽�߁A
//���Ԃ�ITEM_SET�̌�Ɉړ�
#if 0
			case WKC_SOUBI_CHECK_ALL:
				if(SoubiItemCheck(SOUBI_WAZA_HIT_AFTER_ALL,0,0))
					ret=1;
				else
					UserWork[WKC_WORK0]++;
				break;
			case WKC_SOUBI_CHECK:
				if(SoubiItemCheck(SOUBI_WAZA_HIT_AFTER,0,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
#endif
//�ǂ�ڂ������A�C�e�����������n�`�}�L���Ƃ܂����̂�
//�K��ITEM_SET���͐�Ƀ`�F�b�N
			case WKC_KODAWARI_SET:
				if((ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT)&&
				   (atkeqp==SOUBI_ONAZIWAZAONLY)&&
				   //��邠�����́A�������n�`�}�L�̌��ʂ��󂯂Ȃ�
				   (WazaNoTmp!=WAZANO_WARUAGAKI)&&
				  ((waza[0]==0)||
				   (waza[0]==0xffff))){
					if((WazaNoTmp==WAZANO_BATONTATTI)&&
					   ((WazaStatusFlag&WAZASTATUSFLAG_UMAKUKIMARAN)==0)){
						UserWork[WKC_WORK0]++;
						break;
					}
					waza[0]=WazaNoTmp;
				}
				//�Z�Y��ɂ���āA�������n�`�}�L�̋Z���Ȃ��Ƃ���
				//�������n�`�}�L���[�N�����������鏈��
				for(i=0;i<4;i++){
					if(PSP[AttackClient].waza[i]==waza[0])
						break;
				}
				if(i==4)
					waza[0]=0;
				UserWork[WKC_WORK0]++;
				break;
			case WKC_ITEM_SET:
				for(i=0;i<ClientSetMax;i++){
					item=(u16 *)&UserWork[ITEMNO_STOCK+i*2];
					if(item[0]){
						PSP[i].item=item[0];
						item[0]=0;
					}
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_SOUBI_CHECK_ALL:
				if(SoubiItemCheck(SOUBI_WAZA_HIT_AFTER_ALL,0,0))
					ret=1;
				else
					UserWork[WKC_WORK0]++;
				break;
			case WKC_SOUBI_CHECK:
				if(SoubiItemCheck(SOUBI_WAZA_HIT_AFTER,0,0))
					ret=1;
				UserWork[WKC_WORK0]++;
				break;
			case WKC_VANISH_ATTACK_ON:
				if((WazaKouka[AttackClient]&WAZAKOUKA_KIE)&&
				   (ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF)){
					ClientNo=AttackClient;
					BSS_VANISH_ON_OFF_SET(BSS_SERVER,BSS_VANISH_ON);
					ClientBitSet(ClientNo);
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_VANISH_ATTACK_CHECK:
				if((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)||
				  ((WazaKouka[AttackClient]&WAZAKOUKA_KIE)==0)||
//				   (OTF[AttackClient].mahiflag)||
//				   (OTF[AttackClient].koukanaiflag)||
//				   (OTF[AttackClient].huuinflag)||
//				   (OTF[AttackClient].konranflag)){
				   (WazaOutCheck(AttackClient))){
					ClientNo=AttackClient;
					BSS_VANISH_ON_OFF_SET(BSS_SERVER,BSS_VANISH_OFF);
					ClientBitSet(ClientNo);
					WazaKouka[AttackClient]&=WAZAKOUKA_KIE_OFF;
					OSTF[AttackClient].vanishoffflag=1;
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_VANISH_DEFENCE_CHECK:
				if(OSTF[DefenceClient].vanishoffflag==0){
					if((DefenceClient<ClientSetMax)&&
					  ((WazaKouka[DefenceClient]&WAZAKOUKA_KIE)==0)){
						ClientNo=DefenceClient;
						BSS_VANISH_ON_OFF_SET(BSS_SERVER,BSS_VANISH_OFF);
						ClientBitSet(ClientNo);
						WazaKouka[DefenceClient]&=WAZAKOUKA_KIE_OFF;
					}
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_MIGAWARI_CHECK:
				for(i=0;i<ClientSetMax;i++){
					if(WKC[i].migawarihp==0)
						PSP[i].condition2&=CONDITION2_MIGAWARI_OFF;
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_HIT_FLAG_SET:
				if(ServerStatusFlag&SERVER_STATUS_FLAG_A2D){
					ClientNo=AttackClient;
					AttackClient=DefenceClient;
					DefenceClient=ClientNo;
					ServerStatusFlag&=SERVER_STATUS_FLAG_A2D_OFF;
				}
#if 0
				if((CommandReturn[AttackClient]==BSS_SELECT_RETURN_FIGHT)&&
				  ((NoReshuffleClient&no2bittbl[AttackClient])==0)&&
				  ((UserWork[NO_RESHUFFLE_CLIENT]&no2bittbl[AttackClient])==0)){
#else
				if((WTD[WazaNoTmp].battleeffect!=127)||
				  ((WTD[WazaNoTmp].battleeffect==127)&&
				   (WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)))
					//�X�P�b�`����Z�́A�o�Ă��悤�����s���悤������Ă���
					WazaNoSketch[AttackClient]=WazaNoTmp;
				if(((NoReshuffleClient&no2bittbl[AttackClient])==0)&&
				   ((UserWork[NO_RESHUFFLE_CLIENT]&no2bittbl[AttackClient])==0)&&
				   //�o�g���^�b�`�̎��͒l�����Ȃ�
				    (WTD[WazaNoTmp].battleeffect!=127)){
#endif
					if(ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT){
						WazaNoUse[AttackClient]=WazaNoTmp;
						WazaNoLast[AttackClient]=WazaNo;
					}
					else{
						WazaNoUse[AttackClient]=0xffff;
						WazaNoLast[AttackClient]=0xffff;
					}

					if((ServerStatusFlag&(no2bittbl[DefenceClient]<<28))==0){
//						WazaNoDefenceClient[AttackClient]=DefenceClient;
						WazaNoAttackClient[DefenceClient]=AttackClient;
					}

#if 0
					DefenceClient=UserWork[DEFENCE_CLIENT+AttackClient];
					if(NoReshuffleClient&no2bittbl[DefenceClient])
						DefenceClient=ClientNoGet(ClientTypeGet(DefenceClient)^2);
#endif
					if((ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT)&&
					  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
						if(WazaNoTmp==0xffff)
							WazaNoHit[DefenceClient]=WazaNoTmp;
						else{
							WazaNoHit[DefenceClient]=WazaNo;
							if(UserWork[DAMAGE_TYPE_WORK])
								WazaTypeHit[DefenceClient]=UserWork[DAMAGE_TYPE_WORK]&0x3f;
							else
								WazaTypeHit[DefenceClient]=WTD[WazaNo].wazatype;
						}
					}
					else
						WazaNoHit[DefenceClient]=0xffff;		//�m�[�q�b�g�͂��̃i���o�[
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_OUMU_WAZA_NO:
//				if(((NoReshuffleClient&no2bittbl[AttackClient])==0)&&
//				   ((UserWork[NO_RESHUFFLE_CLIENT]&no2bittbl[AttackClient])==0)&&
//				    (WTD[WazaNoTmp].battleeffect!=127)){
				if(((NoReshuffleClient&no2bittbl[AttackClient])==0)&&
				   ((UserWork[NO_RESHUFFLE_CLIENT]&no2bittbl[AttackClient])==0)){
					if((WTD[WazaNoTmp].flag&FLAG_OUMUGAESI)&&
					   (ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT)&&
					   (AttackClient!=DefenceClient)&&
					  ((ServerStatusFlag&(no2bittbl[DefenceClient]<<28))==0)&&
					  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)){
						UserWork[OUMU_WAZANO+0+DefenceClient*2]=WazaNoTmp&0xff;
						UserWork[OUMU_WAZANO+1+DefenceClient*2]=(WazaNoTmp&0xff00)>>8;
						UserWork[OUMU_HIT_WAZANO+0+
								 DefenceClient*4+
								 ((AttackClient>>1)*2)]=WazaNoTmp&0xff;
						UserWork[OUMU_HIT_WAZANO+1+
								 DefenceClient*4+
								 ((AttackClient>>1)*2)]=(WazaNoTmp&0xff00)>>8;
					}
					else{
#if 0
						if(AttackClient==DefenceClient){
							for(i=0;i<ClientSetMax;i++){
								if(MineEnemyCheck(AttackClient)!=
								   MineEnemyCheck(i)){
									UserWork[OUMU_WAZANO+0+i*2]=0;
									UserWork[OUMU_WAZANO+1+i*2]=0;
									UserWork[OUMU_HIT_WAZANO+0+
											 i*4+
											 ((AttackClient>>1)*2)]=0;
									UserWork[OUMU_HIT_WAZANO+1+
											 i*4+
											 ((AttackClient>>1)*2)]=0;
								}
							}
						}
						else{
							UserWork[OUMU_WAZANO+0+DefenceClient*2]=0;
							UserWork[OUMU_WAZANO+1+DefenceClient*2]=0;
							UserWork[OUMU_HIT_WAZANO+0+
									 DefenceClient*4+
									 ((AttackClient>>1)*2)]=0;
							UserWork[OUMU_HIT_WAZANO+1+
									 DefenceClient*4+
									 ((AttackClient>>1)*2)]=0;
						}
#endif
						if(AttackClient!=DefenceClient){
							UserWork[OUMU_WAZANO+0+DefenceClient*2]=0;
							UserWork[OUMU_WAZANO+1+DefenceClient*2]=0;
							UserWork[OUMU_HIT_WAZANO+0+
									 DefenceClient*4+
									 ((AttackClient>>1)*2)]=0;
							UserWork[OUMU_HIT_WAZANO+1+
									 DefenceClient*4+
									 ((AttackClient>>1)*2)]=0;
						}
					}
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_DOUBLE_ATTACK:
				if((ServerStatusFlag&SERVER_STATUS_FLAG_NO_DOUBLE_CHECK)==0){
					if((FightType&FIGHT_TYPE_2vs2)&&
					   (OTF[AttackClient].tamewazaflag==0)&&
					   (WTD[WazaNo].attackrange==RANGE_DOUBLE)&&
					  ((ServerStatusFlag&SERVER_STATUS_FLAG_DOUBLE_CHECK)==0)){
						clientno=ClientNoGet(ClientTypeGet(DefenceClient)^2);
						if(PSP[clientno].hp!=0){
							DefenceClient=clientno;
							ServerStatusFlag|=SERVER_STATUS_FLAG_DOUBLE_CHECK;
							UserWork[WKC_WORK0]=0;
							LoopInitAct();
#if 0
							WazaStatusFlag=0;
							UserWork[DAMAGE_VALUE]=1;
							CriticalValue=1;	//�N���e�B�J���_���[�W���P�{�ɖ߂�
							ServerWork[6]=0;	//NoHitMsg���[�N���N���A
#endif
//							WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
							PushAdrsSet((u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect]);
							WazaSeqAdrs=(u8 *)&WAZA_BlankMsg;
							return;
						}
						else
							ServerStatusFlag|=SERVER_STATUS_FLAG_DOUBLE_CHECK;
					}
				}
				UserWork[WKC_WORK0]++;
				break;
			case WKC_END:
				break;
		}

		if((flag==WKC_CHECK_ONLY)&&(ret==0))
			UserWork[WKC_WORK0]=WKC_END;
	
		if((flag==WKC_CHECK_KOKOMADE)&&
		   (param==UserWork[WKC_WORK0]))
			UserWork[WKC_WORK0]=WKC_END;

	}while((UserWork[WKC_WORK0]!=WKC_END)&&(ret==0));

	if((UserWork[WKC_WORK0]==WKC_END)&&(ret==0))
		WazaSeqAdrs+=3;

}

void	WS_TYPE_CHECK_NO_HIT(void)
{
	int	i;
	u8	wazatype;
	u8	flag=0;
	
	i=0;

	wazatype=WTD[WazaNo].wazatype;

	//������ʂӂ䂤�ɂ͂��߂�^�C�v�̋Z�͓�����Ȃ�
	if((PSP[DefenceClient].speabino==TOKUSYU_HUYUU)&&
	   (wazatype==JIMEN_TYPE)){
		SpeabiNo=TOKUSYU_HUYUU;
		WazaStatusFlag|=(WAZASTATUSFLAG_KOUKANAI|WAZASTATUSFLAG_NOHIT);
		WazaNoHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
		ServerWork[6]=MSG_HUYUUHIT;
		TokusyuNoSet(DefenceClient,SpeabiNo);
	}
	else{
		while(TypeCheckTable[i]!=0xff){
			if(TypeCheckTable[i]==0xfe){		//�݂�Ԃ�`�F�b�N
				if(PSP[DefenceClient].condition2&CONDITION2_MIYABURU)
					break;
				else{
					i+=3;
					continue;
				}
			}
			if(TypeCheckTable[i]==wazatype){
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type1)&&
				   (TypeCheckTable[i+2]==0)){
					WazaStatusFlag|=WAZASTATUSFLAG_KOUKANAI;
					break;
				}
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type1)&&
				   (TypeCheckTable[i+2]==5)){
					flag|=WAZASTATUSFLAG_IMAHITOTSU;
				}
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type1)&&
				   (TypeCheckTable[i+2]==20)){
					flag|=WAZASTATUSFLAG_BATSUGUN;
				}
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type2)&&
				   (PSP[DefenceClient].type1!=PSP[DefenceClient].type2)&&
				   (TypeCheckTable[i+2]==0)){
					WazaStatusFlag|=WAZASTATUSFLAG_KOUKANAI;
					break;
				}
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type2)&&
				   (PSP[DefenceClient].type1!=PSP[DefenceClient].type2)&&
				   (TypeCheckTable[i+2]==5)){
					flag|=WAZASTATUSFLAG_IMAHITOTSU;
				}
				if((TypeCheckTable[i+1]==PSP[DefenceClient].type2)&&
				   (PSP[DefenceClient].type1!=PSP[DefenceClient].type2)&&
				   (TypeCheckTable[i+2]==20)){
					flag|=WAZASTATUSFLAG_BATSUGUN;
				}
			}
			i+=3;
		}
	}

	//����\�͂ӂ����Ȃ܂���́A���ʂ����Q����������Ȃ�
	if((PSP[DefenceClient].speabino==TOKUSYU_HUSIGINAMAMORI)&&
	  ((flag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
	   (TameWazaTurnCheck(AttackClient,WazaNo)==2)&&
	 (((flag&WAZASTATUSFLAG_BATSUGUN)==0)||
	  ((flag&WAZASTATUSFLAG_SOUSAI)==WAZASTATUSFLAG_SOUSAI))&&
	   (WTD[WazaNo].damage)){
		SpeabiNo=TOKUSYU_HUSIGINAMAMORI;
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		WazaNoHit[DefenceClient]=0;		//���O�Ƀq�b�g�����Z�i���o�[���N���A
		ServerWork[6]=MSG_HUSIGINAMAMORIHIT;
		TokusyuNoSet(DefenceClient,SpeabiNo);
	}
	if(WazaStatusFlag&WAZASTATUSFLAG_KOUKANAI)
		OTF[AttackClient].koukanaiflag=1;

	WazaSeqAdrs++;
}

//����A�N�V����

void	WS_MODOSU_EFFECT(void)
{
	ClientNo=AttackClient;
//	SelMonsNo[ClientNo]=ClientBuffer[ClientNo][1];
//	FightJoinFlagSet(ClientNo);
//	if((ServerStatusFlag&SERVER_STATUS_FLAG_KIZETSU)==0){	//���������ɂ��
															//������
	if((ServerStatusFlag&(no2bittbl[ClientNo]<<28))==0){	//���������ɂ��
															//������
		BSS_POKE_RETURN_SET(BSS_SERVER,0);
		ClientBitSet(ClientNo);
	}
	WazaSeqAdrs++;
}

void	WS_APPEAR_EFFECT_INIT(void)
{
	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else
			ClientNo=KizetsuClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

		SelMonsNo[ClientNo]=UserWork[AFTER_RESHUFFLE_SELMONSNO+ClientNo];
		BSS_POKEPARA_GET_SET(BSS_SERVER,BSS_PARA_FIGHT,no2bittbl[SelMonsNo[ClientNo]]);
		ClientBitSet(ClientNo);

		WazaSeqAdrs+=2;
	}
}

void	WS_APPEAR_EFFECT(void)
{
	int	i;
	u8	*adr;
	PokemonServerParam	PSP2;

	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else
			ClientNo=KizetsuClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

		PSP2=PSP[ClientNo];
		adr=(u8 *)&PSP[ClientNo];
		for(i=0;i<sizeof(PokemonServerParam);i++)
			adr[i]=ClientBuffer[ClientNo][i+4];
		PSP[ClientNo].type1=PPD[PSP[ClientNo].monsno].type1;
		PSP[ClientNo].type2=PPD[PSP[ClientNo].monsno].type2;
		PSP[ClientNo].speabino=
			SpecialAbirityGet(PSP[ClientNo].monsno,PSP[ClientNo].speabi);

		//�͂������Ƃ���Ă���Ƃ��́A�A�C�e�������ĂȂ����Ƃɂ���
		i=MineEnemyCheck(ClientNo);
		if(FCC.hatakiotosuitem[i]&no2bittbl[SelMonsNo[ClientNo]])
			PSP[ClientNo].item=0;

		//�o�g���^�b�`�̏���
		if(WTD[WazaNo].battleeffect==127){
			for(i=0;i<8;i++)
				PSP[ClientNo].abiritycnt[i]=PSP2.abiritycnt[i];
			PSP[ClientNo].condition2=PSP2.condition2;
		}
		ServerReshuffleWorkInit();

		UserWork[CLIENT_NO_WORK]=ClientNo;	//���b�Z�[�W�\���̂��߂̃N���C�A���g�i���o�[���Z�b�g

//		PM_strcpy(EvStrBuf0,PSP[ClientNo].nickname);
		EvStrBuf0[0]=I_MSG;
		EvStrBuf0[1]=EV_NICKNAMEONLY;
		EvStrBuf0[2]=ClientNo;
		EvStrBuf0[3]=SelMonsNo[ClientNo];
		EvStrBuf0[4]=EOM_;

		WazaSeqAdrs+=2;
	}
}

void	WS_APPEAR_EFFECT_WAIT(void)
{
	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else
			ClientNo=KizetsuClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

		if((MineEnemyCheck(ClientNo)==SIDE_ENEMY)&&
		  ((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
		  ((FightType&FIGHT_TYPE_CARD_E)==0)&&
		  ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0))
			//�}�ӂɌ����t���O�Z�b�g
			ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[ClientNo].monsno),Z_SEE_SET);

		//�A�C�e���ŕ��������|�P�������J��o�����Ƃ���Client�����������邽�߂̏���
		NoReshuffleClient&=(no2bittbl[ClientNo]^0xffffffff);

		BSS_POKE_APPEAR_SET(BSS_SERVER,SelMonsNo[ClientNo],WazaSeqAdrs[2]);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=3;
//		RetServerSequence=ServerSequence;
//		if(SoubiItemCheck(SOUBI_POKEAPPEAR,ClientNo)) return;
//		if(SpecialAbirityCheck(SPEABI_POKEAPPEAR,ClientNo,0,0,0)) return;
	}
}

void	WS_RESHUFFLE_CHECK(void)
{
	int i,j;
	u8	no1,no2;
	PokemonParam	*PP;

	ClientNo=SideClientNoGet(WazaSeqAdrs[1]&0x7f);

	if(((WazaSeqAdrs[1]&0x80)==0)&&
	  ((PSP[ClientNo].condition2&(CONDITION2_SHIME|CONDITION2_KUROIMANAZASHI))||
	    (WazaKouka[ClientNo]&WAZAKOUKA_NEWOHARU))){
		WazaSeqAdrs=(u8 *)(WazaSeqAdrs[2]<< 0|
						   WazaSeqAdrs[3]<< 8|
						   WazaSeqAdrs[4]<<16|
						   WazaSeqAdrs[5]<<24);
		return;
	}

	if(FightType&FIGHT_TYPE_4_2vs2){
		if(MineEnemyCheck(ClientNo)==SIDE_ENEMY)	//Enemy
			PP=&PokeParaEnemy[0];
		else								//Mine
			PP=&PokeParaMine[0];
		j=0;
		if(GetTeamPos2(GetTeamPosID(ClientNo))==1)
			j=3;
		for(i=j;i<j+3;i++){
			if((PokeParaGet(&PP[i],ID_monsno)!=0)&&
			   (PokeParaGet(&PP[i],ID_tamago_flag)==0)&&
			   (PokeParaGet(&PP[i],ID_hp)!=0)&&
			   (SelMonsNo[ClientNo]!=i))
				break;
		}
		if(i==j+3)
			WazaSeqAdrs=(u8 *)(WazaSeqAdrs[2]<< 0|
							   WazaSeqAdrs[3]<< 8|
							   WazaSeqAdrs[4]<<16|
							   WazaSeqAdrs[5]<<24);
		else
			WazaSeqAdrs+=6;
	}
	else{
		if(MineEnemyCheck(ClientNo)==SIDE_ENEMY){	//Enemy
			no1=ClientNoGet(BSS_CLIENT_ENEMY);
			if(FightType&FIGHT_TYPE_2vs2)
				no2=ClientNoGet(BSS_CLIENT_ENEMY2);
			else
				no2=no1;
			PP=&PokeParaEnemy[0];
		}		
		else{								//Mine
			no1=ClientNoGet(BSS_CLIENT_MINE);
			if(FightType&FIGHT_TYPE_2vs2)
				no2=ClientNoGet(BSS_CLIENT_MINE2);
			else
			no2=no1;
			PP=&PokeParaMine[0];
		}
		for(i=0;i<6;i++){
			if((PokeParaGet(&PP[i],ID_hp)!=0)&&
			   (PokeParaGet(&PP[i],ID_monsno)!=0)&&
			   (PokeParaGet(&PP[i],ID_tamago_flag)==0)&&
			   (i!=SelMonsNo[no1])&&(i!=SelMonsNo[no2])) break;
		}
		if(i==6)
			WazaSeqAdrs=(u8 *)(WazaSeqAdrs[2]<< 0|
							   WazaSeqAdrs[3]<< 8|
							   WazaSeqAdrs[4]<<16|
							   WazaSeqAdrs[5]<<24);
		else
			WazaSeqAdrs+=6;
	}
}

static void PokeSelectSet(u8 noselno)
{
	UserWork[BEFORE_RESHUFFLE_SELMONSNO+ClientNo]=SelMonsNo[ClientNo];
	BSS_POKE_SELECT_SET(BSS_SERVER,
						PLT_B_USELESS,
						noselno,
						0,
						(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
	ClientBitSet(ClientNo);
}

void	WS_POKE_RESHUFFLE(void)
{
	u8	*adr;
	u8	flag;
	u8	clientno=0;
	u8	waitflag=0;

	adr=(u8 *)(WazaSeqAdrs[2]<< 0|
			   WazaSeqAdrs[3]<< 8|
			   WazaSeqAdrs[4]<<16|
			   WazaSeqAdrs[5]<<24);

	if(WazaSeqAdrs[1]==SIDE_DOUBLE1){
		if((FightType&FIGHT_TYPE_4_2vs2)||
		  ((FightType&FIGHT_TYPE_2vs2)==0)){
			for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
				if(ServerStatusFlag&(no2bittbl[ClientNo]<<28)){
					if(PokeReshuffleCheck(ClientNo,6,6)){
						NoReshuffleClient|=no2bittbl[ClientNo];
						ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
						BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
						ClientBitSet(ClientNo);
					}
					else{
						if(OSTF[ClientNo].reshuffleflag==0){
							PokeSelectSet(6);
							OSTF[ClientNo].reshuffleflag=1;
						}
					}
				}
				else{
					BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
					ClientBitSet(ClientNo);
				}
			}
		}
		else if(FightType&FIGHT_TYPE_2vs2){
			flag=(ServerStatusFlag>>28);
			if(flag&no2bittbl[0]){
				ClientNo=0;
				if(PokeReshuffleCheck(ClientNo,6,6)){
					NoReshuffleClient|=no2bittbl[ClientNo];
					ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
					BSS_GAUGE_OFF_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
				else{
//					PokeSelectSet(6);
					if(OSTF[ClientNo].reshuffleflag==0){
						PokeSelectSet(UserWork[AFTER_RESHUFFLE_SELMONSNO+2]);
						OSTF[ClientNo].reshuffleflag=1;
					}
					else{
						BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
						ClientBitSet(ClientNo);
						waitflag|=1;
					}
				}
			}
			if((flag&no2bittbl[2])&&((flag&no2bittbl[0])==0)){
				ClientNo=2;
				if(PokeReshuffleCheck(ClientNo,6,6)){
					NoReshuffleClient|=no2bittbl[ClientNo];
					ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
					BSS_GAUGE_OFF_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
				else{
//					PokeSelectSet(6);
					if(OSTF[ClientNo].reshuffleflag==0){
						PokeSelectSet(UserWork[AFTER_RESHUFFLE_SELMONSNO+0]);
						OSTF[ClientNo].reshuffleflag=1;
					}
					else if((waitflag&1)==0){
						BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
						ClientBitSet(ClientNo);
					}
				}
			}

			if(flag&no2bittbl[1]){
				ClientNo=1;
				if(PokeReshuffleCheck(ClientNo,6,6)){
					NoReshuffleClient|=no2bittbl[ClientNo];
					ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
					BSS_GAUGE_OFF_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
				else{
//					PokeSelectSet(6);
					if(OSTF[ClientNo].reshuffleflag==0){
						PokeSelectSet(UserWork[AFTER_RESHUFFLE_SELMONSNO+3]);
						OSTF[ClientNo].reshuffleflag=1;
					}
					else{
						BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
						ClientBitSet(ClientNo);
						waitflag|=2;
					}
				}
			}

			if((flag&no2bittbl[3])&&((flag&no2bittbl[1])==0)){
				ClientNo=3;
				if(PokeReshuffleCheck(ClientNo,6,6)){
					NoReshuffleClient|=no2bittbl[ClientNo];
					ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
					BSS_GAUGE_OFF_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
				else{
//					PokeSelectSet(6);
					if(OSTF[ClientNo].reshuffleflag==0){
						PokeSelectSet(UserWork[AFTER_RESHUFFLE_SELMONSNO+1]);
						OSTF[ClientNo].reshuffleflag=1;
					}
					else if((waitflag&2)==0){
						BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
						ClientBitSet(ClientNo);
					}
				}
			}

			if((OSTF[0].reshuffleflag==0)&&
			   (OSTF[2].reshuffleflag==0)&&
			   (flag)){
				if(NoReshuffleClient&no2bittbl[0])
					ClientNo=2;
				else
					ClientNo=0;
				BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
				ClientBitSet(ClientNo);
			}

			if((OSTF[1].reshuffleflag==0)&&
			   (OSTF[3].reshuffleflag==0)&&
			   (flag)){
				if(NoReshuffleClient&no2bittbl[1])
					ClientNo=3;
				else
					ClientNo=1;
				BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
				ClientBitSet(ClientNo);
			}

		}
		WazaSeqAdrs+=6;
	}
	else if(WazaSeqAdrs[1]==SIDE_DOUBLE2){
		if(FightType&FIGHT_TYPE_4_2vs2)
			WazaSeqAdrs+=6;
		else if(FightType&FIGHT_TYPE_2vs2){
			flag=(ServerStatusFlag>>28);
			if((flag&no2bittbl[2])&&(flag&no2bittbl[0])){
				ClientNo=2;
				if(PokeReshuffleCheck(ClientNo,ClientBuffer[0][1],6)){
					NoReshuffleClient|=no2bittbl[ClientNo];
					ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
					BSS_GAUGE_OFF_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
				else{
					if(OSTF[ClientNo].reshuffleflag==0){
						PokeSelectSet(UserWork[AFTER_RESHUFFLE_SELMONSNO+0]);
						OSTF[ClientNo].reshuffleflag=1;
					}
				}
			}
			if((flag&no2bittbl[3])&&(flag&no2bittbl[1])){
				ClientNo=3;
				if(PokeReshuffleCheck(ClientNo,ClientBuffer[1][1],6)){
					NoReshuffleClient|=no2bittbl[ClientNo];
					ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
					BSS_GAUGE_OFF_SET(BSS_SERVER);
					ClientBitSet(ClientNo);
				}
				else{
					if(OSTF[ClientNo].reshuffleflag==0){
						PokeSelectSet(UserWork[AFTER_RESHUFFLE_SELMONSNO+1]);
						OSTF[ClientNo].reshuffleflag=1;
					}
				}
			}
			WazaSeqAdrs+=6;
		}
		else
			WazaSeqAdrs+=6;
		flag=(ServerStatusFlag>>28);
		KizetsuClient=0;
		while(((flag&no2bittbl[KizetsuClient])==0)&&(KizetsuClient<ClientSetMax)){
			KizetsuClient++;
		}
		if(KizetsuClient==ClientSetMax)
			WazaSeqAdrs=adr;
	}
	else{
		if(WazaSeqAdrs[1]&B_CANCEL_OK)
			flag=PLT_NORMAL;
		else
			flag=PLT_B_USELESS;
	
#if 0
		if((WazaSeqAdrs[1]&0x7f)==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else if((WazaSeqAdrs[1]&0x7f)==SIDE_ATTACK)
			ClientNo=AttackClient;
		else
			ClientNo=KizetsuClient;
#else
		clientno=SideClientNoGet(WazaSeqAdrs[1]&0x7f);
#endif
		if(OSTF[clientno].reshuffleflag){
			WazaSeqAdrs+=6;
			return;
		}
	
		if(PokeReshuffleCheck(clientno,6,6)){
			ClientNo=clientno;
			NoReshuffleClient|=no2bittbl[ClientNo];
			ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
//			BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
//			ClientBitSet(ClientNo);
			WazaSeqAdrs=adr;
		}
		else{
			ClientNo=clientno;
			UserWork[BEFORE_RESHUFFLE_SELMONSNO+ClientNo]=SelMonsNo[ClientNo];
			BSS_POKE_SELECT_SET(BSS_SERVER,
								flag,
//								6,
								UserWork[AFTER_RESHUFFLE_SELMONSNO+(ClientNo^2)],
								0,
								(u8 *)&UserWork[SERVER_LIST_ROW+ClientNo*3]);
			ClientBitSet(ClientNo);
//			ServerWork[5]=ClientTypeGet(ClientNo)&1;
			WazaSeqAdrs+=6;
			if(ClientTypeGet(ClientNo)==SIDE_MINE){
				if(FTW.PokeReshuffleCount<0xff)	//�������|�P��������������
					FTW.PokeReshuffleCount++;	//�������|�P��������������
			}
			if(FightType&FIGHT_TYPE_4_2vs2){
				for(ClientNo=0;ClientNo<ClientSetMax;ClientNo++){
					if(ClientNo!=clientno){
						BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
						ClientBitSet(ClientNo);
					}
				}
			}
			else{
				ClientNo=ClientNoGet(ClientTypeGet(clientno)^1);
				if(NoReshuffleClient&no2bittbl[ClientNo])
					ClientNo^=2;
				BSS_TUUSHIN_WAIT_SET(BSS_SERVER,2);
				ClientBitSet(ClientNo);
			}
		}
	}
}

void	WS_POKE_RESHUFFLE_WAIT(void)
{
	int	i;
	if(ClientBit==0){
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
		switch(WazaSeqAdrs[2]){
			case PRW_AFTER_SELMONS_SET:
				for(i=0;i<ClientSetMax;i++){
					if(ClientBuffer[i][0]==BSS_POKE_RESHUFFLE)
						UserWork[AFTER_RESHUFFLE_SELMONSNO+i]=ClientBuffer[i][1];
				}
				break;
			case PRW_LISTROW_SET:
				if((FightType&FIGHT_TYPE_4_2vs2)==0)
					ServerListRowSet(ClientNo);
				break;
			case PRW_NORMAL:
				ServerWork[0]=ClientBuffer[ClientNo][1];
				UserWork[AFTER_RESHUFFLE_SELMONSNO+ClientNo]=ClientBuffer[ClientNo][1];
				if(FightType&FIGHT_TYPE_4_2vs2){
					UserWork[SERVER_LIST_ROW+ClientNo*3]&=0x0f;
					UserWork[SERVER_LIST_ROW+ClientNo*3]|=
						(ClientBuffer[ClientNo][2]&0xf0);
					UserWork[SERVER_LIST_ROW+1+ClientNo*3]=
						ClientBuffer[ClientNo][3];
		
					UserWork[SERVER_LIST_ROW+(ClientNo^2)*3]&=0xf0;
					UserWork[SERVER_LIST_ROW+(ClientNo^2)*3]|=
						((ClientBuffer[ClientNo][2]&0xf0)>>4);
					UserWork[SERVER_LIST_ROW+2+(ClientNo^2)*3]=
						ClientBuffer[ClientNo][3];
				}
				else
					ServerListRowSet(ClientNo);
//����ւ����I�����Ă���Ƃ��̎��ɌJ��o���|�P��������\�����邽�߂̃f�[�^�Z�b�g
				EvStrBuf0[0]=I_MSG;
				EvStrBuf0[1]=EV_MONSNAME;
				EvStrBuf0[2]=(PSP[AttackClient].monsno&0x00ff)>>0;
				EvStrBuf0[3]=(PSP[AttackClient].monsno&0xff00)>>8;
				EvStrBuf0[4]=EOM_;
				EvStrBuf1[0]=I_MSG;
				EvStrBuf1[1]=EV_NICKNAMEONLY;
				EvStrBuf1[2]=ClientNo;
				EvStrBuf1[3]=ClientBuffer[ClientNo][1];
				EvStrBuf1[4]=EOM_;
				break;
		}
		WazaSeqAdrs+=3;
	}
}

void	WS_POKE_RESHUFFLE_END(void)
{
	int	i;
	u16	*hp;
	u8	makibishidamage;
	u8	flag;

#if 0
	if(WazaSeqAdrs[1]==SIDE_DEFENCE)
		ClientNo=DefenceClient;
	else if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=KizetsuClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
	FightJoinFlagSet(ClientNo);
	ServerStatusFlag&=((no2bittbl[ClientNo]<<28)^0xffffffff);
	OSTF[ClientNo].reshuffleflag=0;
	if((SideCondition[MineEnemyCheck(ClientNo)]&SIDE_CONDITION_MAKIBISHI_CHECK)==0){
		if((SideCondition[MineEnemyCheck(ClientNo)]&SIDE_CONDITION_MAKIBISHI)&&
		   (PSP[ClientNo].type1!=HIKOU_TYPE)&&
		   (PSP[ClientNo].type2!=HIKOU_TYPE)&&
		   (PSP[ClientNo].speabino!=TOKUSYU_HUYUU)){
			SideCondition[MineEnemyCheck(ClientNo)]|=SIDE_CONDITION_MAKIBISHI_CHECK;
			makibishidamage=(5-SCC[MineEnemyCheck(ClientNo)].makibishicnt)*2;
			WazaDamage=PSP[ClientNo].hpmax/makibishidamage;
			if(WazaDamage==0)
				WazaDamage=1;
			UserWork[CLIENT_NO_WORK]=ClientNo;
			PushAdrsAct();
//			PushWazaSeqAdrs=WazaSeqAdrs;
			if(WazaSeqAdrs[1]==SIDE_DEFENCE)
				WazaSeqAdrs=(u8 *)&WAZA_MakibishiD;
			else if(WazaSeqAdrs[1]==SIDE_ATTACK)
				WazaSeqAdrs=(u8 *)&WAZA_MakibishiA;
			else
				WazaSeqAdrs=(u8 *)&WAZA_MakibishiK;
			return;
		}
	}

	//�����Ȃ܂����o�Ă����Ƃ��́A���̃^�[���łȂ܂��Ȃ��悤�ɂ���
	if(PSP[ClientNo].speabino==TOKUSYU_NAMAKE)
		WKC[ClientNo].namakebit=1;

	if(SpecialAbirityCheck(SPEABI_POKEAPPEAR,ClientNo,0,0,0)) return;
//	if(SpecialAbirityCheck(SPEABI_IKAKU2,0,0,0,0)) return;
	if(SoubiItemCheck(SOUBI_POKEAPPEAR,ClientNo,0)) return;
//	if(SoubiItemCheck(SOUBI_WAZA_HIT_AFTER_ALL,0,0)) return;

	SideCondition[MineEnemyCheck(ClientNo)]&=SIDE_CONDITION_MAKIBISHI_CHECK_OFF;

	for(i=0;i<ClientSetMax;i++){
		if(ActionClientNo[i]==ClientNo)
			ActionNo[i]=SCA_EndNo;
	}

	for(i=0;i<ClientSetMax;i++){
		hp=(u16 *)&UserWork[MONS_HP_TEMP+(MineEnemyCheck(i)*2)];
		hp[0]=PSP[i].hp;
	}

	if(WazaSeqAdrs[1]==SIDE_DOUBLE1){
		flag=(ServerStatusFlag>>28);
		KizetsuClient++;
		while((((flag&no2bittbl[KizetsuClient])==0)||
				(NoReshuffleClient&no2bittbl[KizetsuClient]))&&
				(KizetsuClient<ClientSetMax)){
			KizetsuClient++;
		}
	}
	WazaSeqAdrs+=2;
}

void	WS_TRAINER_APPEAR(void)
{
	if(WazaSeqAdrs[1]==SIDE_MINE)
		ClientNo=ClientNoGet(BSS_CLIENT_MINE);
	else
		ClientNo=ClientNoGet(BSS_CLIENT_ENEMY);
	BSS_TRAINER_APPEAR_SET(BSS_SERVER);
	ClientBitSet(ClientNo);
	WazaSeqAdrs+=2;
}

void	WS_SE_PLAY(void)
{
//	ClientNo=ClientNoGet(BSS_CLIENT_ENEMY);
	ClientNo=AttackClient;
	BSS_SE_PLAY_SET(BSS_SERVER,WazaSeqAdrs[1]|(WazaSeqAdrs[2]<<8));
	ClientBitSet(ClientNo);
	WazaSeqAdrs+=3;
}

void	WS_ME_PLAY(void)
{
//	ClientNo=ClientNoGet(BSS_CLIENT_ENEMY);
	ClientNo=AttackClient;
	BSS_ME_PLAY_SET(BSS_SERVER,WazaSeqAdrs[1]|(WazaSeqAdrs[2]<<8));
	ClientBitSet(ClientNo);
	WazaSeqAdrs+=3;
}

void	WS_VOICE(void)
{
#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	BSS_VOICE_PLAY_SET(BSS_SERVER);
	ClientBitSet(ClientNo);
	WazaSeqAdrs+=2;
}

void	WS_END_FIGHT(void)
{
	ClientNo=ClientNoGet(BSS_CLIENT_MINE);
	BSS_FIGHT_END_SET(BSS_SERVER,WinLoseFlag);
	ClientBitSet(ClientNo);
	WazaSeqAdrs++;
}

void	WS_POKEDELACTOR(void)
{
#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	BSS_POKE_RETURN_SET(BSS_SERVER,1);
	ClientBitSet(ClientNo);

	WazaSeqAdrs+=2;
}

void	WS_WAZAOBOE_CHECK(void)
{
	u8	*adr1;
	u8	*adr2;
	u16	ret;

	adr1=(u8 *)(WazaSeqAdrs[1]<< 0|
	 		    WazaSeqAdrs[2]<< 8|
			    WazaSeqAdrs[3]<<16|
			    WazaSeqAdrs[4]<<24);
	adr2=(u8 *)(WazaSeqAdrs[5]<< 0|
	 		    WazaSeqAdrs[6]<< 8|
			    WazaSeqAdrs[7]<<16|
			    WazaSeqAdrs[8]<<24);

	ret=PokeWazaOboeCheck(&PokeParaMine[UserWork[GETEXP_SELMONS]],WazaSeqAdrs[9]);
	if(ret==SAME_WAZA_SET)
		while((ret=PokeWazaOboeCheck(&PokeParaMine[UserWork[GETEXP_SELMONS]],0))==SAME_WAZA_SET);
	if(ret==0)
		WazaSeqAdrs=adr2;
	else if(ret==NO_WAZA_SET)
		WazaSeqAdrs+=10;
	else{
		ClientNo=ClientNoGet(BSS_CLIENT_MINE);
		if((SelMonsNo[ClientNo]==UserWork[GETEXP_SELMONS])&&
		  ((PSP[ClientNo].condition2&CONDITION2_HENSHIN)==0))
			PokeWazaSetServer(&PSP[ClientNo],ret);
		if(FightType&FIGHT_TYPE_2vs2){
			ClientNo=ClientNoGet(BSS_CLIENT_MINE2);
			if((SelMonsNo[ClientNo]==UserWork[GETEXP_SELMONS])&&
			  ((PSP[ClientNo].condition2&CONDITION2_HENSHIN)==0))
				PokeWazaSetServer(&PSP[ClientNo],ret);
		}
		WazaSeqAdrs=adr1;
	}
}

enum{
	WAZAWASURE_WINOPEN=0,
	WAZAWASURE_SELECT,
	WAZAWASURE_WAZASELECT_INIT,
	WAZAWASURE_WAZASELECT,
	WAZAWASURE_WAZAAKIRAME,
	WAZAWASURE_ALERT,
};

void	WS_WAZAWASURE_CHECK(void)
{
	u16	wazano;
	u8	pos;

	ClientNo=0;

	switch(UserWork[WW_SEQNO]){
		case WAZAWASURE_WINOPEN:
			WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,0);
			MsgPutSet(&FightMsg,YesNoMsg,CGX_START+0x80,WIN_X1+2,WIN_Y1+1);
			MsgPut(&FightMsg);
			UserWork[WW_SEQNO]++;
			ServerWork[1]=0;
			YesNoCsrOn();
			break;
		case WAZAWASURE_SELECT:
			if((sys.Trg&U_KEY)==U_KEY){
				if(ServerWork[1]){
					SePlay(SE_SELECT);
					YesNoCsrOff();
					ServerWork[1]=0;
					YesNoCsrOn();
				}
			}
			if((sys.Trg&D_KEY)==D_KEY){
				if(ServerWork[1]==0){
					SePlay(SE_SELECT);
					YesNoCsrOff();
					ServerWork[1]=1;
					YesNoCsrOn();
				}
			}
			if((sys.Trg&A_BUTTON)==A_BUTTON){
				SePlay(SE_SELECT);
				if(ServerWork[1])
					UserWork[WW_SEQNO]=WAZAWASURE_WAZAAKIRAME;
				else{
					WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
					PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
					UserWork[WW_SEQNO]++;
				}
				break;
			}
			if((sys.Trg&B_BUTTON)==B_BUTTON){
				SePlay(SE_SELECT);
				UserWork[WW_SEQNO]=WAZAWASURE_WAZAAKIRAME;
				break;
			}
			break;
		case WAZAWASURE_WAZASELECT_INIT:
			if(FadeData.fade_sw == 0){
				PokeNewWazaSet(&PokeParaMine,
							   UserWork[GETEXP_SELMONS],
							   PokeCountMine-1,
							   BattleRecover,
							   OboeWazaNo);
				UserWork[WW_SEQNO]++;
			}
			break;
		case WAZAWASURE_WAZASELECT:
			if((FadeData.fade_sw==0)&&(sys.pMainProc==MainFight)){
				pos=GetNewWazaSelectPos();
				if(pos==4)
					UserWork[WW_SEQNO]=WAZAWASURE_WAZAAKIRAME;
				else{
					wazano=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],ID_waza1+pos);

					if(HidenWazaCheck(wazano)){
						FightMsgServerSet(NOWAZAWASUREMSGNO,ClientNo);
						UserWork[WW_SEQNO]=WAZAWASURE_ALERT;
						return;
					}
					WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
	 		    					   WazaSeqAdrs[2]<< 8|
			    					   WazaSeqAdrs[3]<<16|
			    					   WazaSeqAdrs[4]<<24);
					EvStrBuf1[0]=I_MSG;
					EvStrBuf1[1]=EV_WAZANO;
					EvStrBuf1[2]=wazano&0xff;
					EvStrBuf1[3]=(wazano&0xff00)>>8;
					EvStrBuf1[4]=EOM_;

					PokePPCountClear(&PokeParaMine[UserWork[GETEXP_SELMONS]],pos);
					PokeWazaSetPos(&PokeParaMine[UserWork[GETEXP_SELMONS]],OboeWazaNo,pos);
					if((SelMonsNo[0]==UserWork[GETEXP_SELMONS])&&
					  ((PSP[0].condition2&CONDITION2_HENSHIN)==0)&&
					  ((WKC[0].monomanebit&no2bittbl[pos])==0)){
						PokePPCountClearServer(&PSP[0],pos);
						PokeWazaSetPosServer(&PSP[0],OboeWazaNo,pos);
					}
					if(FightType&FIGHT_TYPE_2vs2){
						if((SelMonsNo[2]==UserWork[GETEXP_SELMONS])&&
						  ((PSP[2].condition2&CONDITION2_HENSHIN)==0)&&
						  ((WKC[2].monomanebit&no2bittbl[pos])==0)){
							PokePPCountClearServer(&PSP[2],pos);
							PokeWazaSetPosServer(&PSP[2],OboeWazaNo,pos);
						}
					}
				}
			}
			break;
		case WAZAWASURE_WAZAAKIRAME:
			WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
			WazaSeqAdrs+=5;
			break;
		case WAZAWASURE_ALERT:
			if(ClientBit==0){
				UserWork[WW_SEQNO]=WAZAWASURE_WAZASELECT_INIT;
			}
			break;
	}
}

enum{
	WAZAAKIRAME_WINOPEN=0,
	WAZAAKIRAME_SELECT,
};

void	WS_WAZAAKIRAME_CHECK(void)
{
	switch(UserWork[WW_SEQNO]){
		case WAZAAKIRAME_WINOPEN:
			WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,0);
			MsgPutSet(&FightMsg,YesNoMsg,CGX_START+0x80,WIN_X1+2,WIN_Y1+1);
			MsgPut(&FightMsg);
			UserWork[WW_SEQNO]++;
			ServerWork[1]=0;
			YesNoCsrOn();
			break;
		case WAZAAKIRAME_SELECT:
			if((sys.Trg&U_KEY)==U_KEY){
				if(ServerWork[1]){
					SePlay(SE_SELECT);
					YesNoCsrOff();
					ServerWork[1]=0;
					YesNoCsrOn();
				}
			}
			if((sys.Trg&D_KEY)==D_KEY){
				if(ServerWork[1]==0){
					SePlay(SE_SELECT);
					YesNoCsrOff();
					ServerWork[1]=1;
					YesNoCsrOn();
				}
			}
			if((sys.Trg&A_BUTTON)==A_BUTTON){
				SePlay(SE_SELECT);
				if(ServerWork[1])
					WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
 		    						   WazaSeqAdrs[2]<< 8|
		    						   WazaSeqAdrs[3]<<16|
		    						   WazaSeqAdrs[4]<<24);
				else
					WazaSeqAdrs+=5;
					WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
				break;
			}
			if((sys.Trg&B_BUTTON)==B_BUTTON){
				SePlay(SE_SELECT);
				WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
 	    						   WazaSeqAdrs[2]<< 8|
	    						   WazaSeqAdrs[3]<<16|
	    						   WazaSeqAdrs[4]<<24);
				WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
				break;
			}
			break;
	}
}
#if 0
void	WS_WAZAOBOE_CHECK(void)
{
	u8	*adr1;
	u8	*adr2;
	u16	ret;

	if(WazaSeqAdrs[1]==0)
		ClientNo=DefenceClient;
	else
		ClientNo=AttackClient;

	adr1=(u8 *)(WazaSeqAdrs[2]<< 0|
	 		    WazaSeqAdrs[3]<< 8|
			    WazaSeqAdrs[4]<<16|
			    WazaSeqAdrs[5]<<24);
	adr2=(u8 *)(WazaSeqAdrs[6]<< 0|
	 		    WazaSeqAdrs[7]<< 8|
			    WazaSeqAdrs[8]<<16|
			    WazaSeqAdrs[9]<<24);

	ret=PokeWazaOboeCheck(&PokeParaMine[UserWork[GETEXP_SELMONS]]);
	if(ret==0)
		WazaSeqAdrs=adr2;
	else if(ret==0xffff)
		WazaSeqAdrs+=10;
	else
		WazaSeqAdrs=adr1;
}

enum{
	WAZAWASURE_WINOPEN=0,
	WAZAWASURE_SELECT,
	WAZAWASURE_WAZASELECT,
	WAZAWASURE_WAZAAKIRAME,
};

void	WS_WAZAWASURE_CHECK(void)
{
	if(WazaSeqAdrs[1]==0)
		ClientNo=DefenceClient;
	else
		ClientNo=AttackClient;

	switch(UserWork[WW_SEQNO]){
		case WAZAWASURE_WINOPEN:
			WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,0);
			MsgPutSet(&FightMsg,YesNoMsg,CGX_START+0x80,WIN_X1+2,WIN_Y1+1);
			MsgPut(&FightMsg);
			UserWork[WW_SEQNO]++;
			ServerWork[1]=0;
			YesNoCsrOn();
			break;
		case WAZAWASURE_SELECT:
			switch(sys.Trg){
				case U_KEY:
				case D_KEY:
					YesNoCsrOff();
					ServerWork[1]^=1;
					YesNoCsrOn();
					break;
				case A_BUTTON:
					if(ServerWork[1])
						UserWork[WW_SEQNO]=WAZAWASURE_WAZAAKIRAME;
					else{
						BSS_WAZA_SELECT_SET(BSS_SERVER,
											0,
											BSS_WAZA_SELECT_WAZAWASURE);
						ClientBitSet(ClientNo);
						WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
						UserWork[WW_SEQNO]++;
					}
					break;
				case B_BUTTON:
					UserWork[WW_SEQNO]=WAZAWASURE_WAZAAKIRAME;
					break;
			}
			break;
		case WAZAWASURE_WAZASELECT:
			if(ClientBit==0){
				if((ClientBuffer[ClientNo][2]|
				   (ClientBuffer[ClientNo][3]<<8))==0xffff)
					UserWork[WW_SEQNO]=WAZAWASURE_WAZAAKIRAME;
				else{
					PM_strcpy(EvStrBuf0,
							  WazaNameData[
							  PokeParaGet(
							  &PokeParaMine[UserWork[GETEXP_SELMONS]],
							  ID_waza1+ClientBuffer[ClientNo][2])
							  ]);
					WazaSeqAdrs=(u8 *)(WazaSeqAdrs[2]<< 0|
	 		    					   WazaSeqAdrs[3]<< 8|
			    					   WazaSeqAdrs[4]<<16|
			    					   WazaSeqAdrs[5]<<24);
					PokeWazaSetPos(&PokeParaMine[UserWork[GETEXP_SELMONS]],
								   WazaNo,
								   ClientBuffer[ClientNo][2]);
				}
			}
			break;
		case WAZAWASURE_WAZAAKIRAME:
			WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
			WazaSeqAdrs+=6;
			break;
	}
}

enum{
	WAZAAKIRAME_WINOPEN=0,
	WAZAAKIRAME_SELECT,
};

void	WS_WAZAAKIRAME_CHECK(void)
{
	if(WazaSeqAdrs[1]==0)
		ClientNo=DefenceClient;
	else
		ClientNo=AttackClient;

	switch(UserWork[WW_SEQNO]){
		case WAZAAKIRAME_WINOPEN:
			WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,0);
			MsgPutSet(&FightMsg,YesNoMsg,CGX_START+0x80,WIN_X1+2,WIN_Y1+1);
			MsgPut(&FightMsg);
			UserWork[WW_SEQNO]++;
			ServerWork[1]=0;
			YesNoCsrOn();
			break;
		case WAZAAKIRAME_SELECT:
			switch(sys.Trg){
				case U_KEY:
				case D_KEY:
					YesNoCsrOff();
					ServerWork[1]^=1;
					YesNoCsrOn();
					break;
				case A_BUTTON:
					if(ServerWork[1])
						WazaSeqAdrs=(u8 *)(WazaSeqAdrs[2]<< 0|
	 		    						   WazaSeqAdrs[3]<< 8|
			    						   WazaSeqAdrs[4]<<16|
			    						   WazaSeqAdrs[5]<<24);
					else
						WazaSeqAdrs+=6;
					WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
					break;
				case B_BUTTON:
					WazaSeqAdrs=(u8 *)(WazaSeqAdrs[2]<< 0|
 		    						   WazaSeqAdrs[3]<< 8|
		    						   WazaSeqAdrs[4]<<16|
		    						   WazaSeqAdrs[5]<<24);
					WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
					break;
			}
			break;
	}
}
#endif

void	WS_POKE_BLINK(void)
{
#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	if((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0){
		if((ServerStatusFlag&SERVER_STATUS_FLAG_NO_MIGAWARI)&&
	       (PSP[ClientNo].condition2&CONDITION2_MIGAWARI)&&
	       (WKC[ClientNo].migawarihp))
			WazaSeqAdrs+=2;
		else{
			BSS_POKEDAMAGE_BLINK_SET(BSS_SERVER);
			ClientBitSet(ClientNo);
			WazaSeqAdrs+=2;
		}
	}
	else
		WazaSeqAdrs+=2;
}

void	WS_OKODUKAI_CALC(void)
{
	int	i=0;
	TrainerTypeNormalData	*TTND;
	TrainerTypeWazaData		*TTWD;
	TrainerTypeItemData		*TTID;
	TrainerTypeMultiData	*TTMD;
	u32	money;
	u8	level=0;

	if(FieldTrainerID==ORUSUBAN_TRAINER)
		money=SBD_WORK->battle.level[0]*20*UserWork[OKANE2BAI];
	else{
		switch(TD[FieldTrainerID].datatype){
			case DATATYPE_NORMAL:
				TTND=(TrainerTypeNormalData *)TD[FieldTrainerID].TPD;
				level=TTND[TD[FieldTrainerID].monscount-1].level;
				break;
			case DATATYPE_WAZA:
				TTWD=(TrainerTypeWazaData *)TD[FieldTrainerID].TPD;
				level=TTWD[TD[FieldTrainerID].monscount-1].level;
				break;
			case DATATYPE_ITEM:
				TTID=(TrainerTypeItemData *)TD[FieldTrainerID].TPD;
				level=TTID[TD[FieldTrainerID].monscount-1].level;
				break;
			case DATATYPE_MULTI:
				TTMD=(TrainerTypeMultiData *)TD[FieldTrainerID].TPD;
				level=TTMD[TD[FieldTrainerID].monscount-1].level;
				break;
		}
		while(TMD[i].trtype!=0xff){
			if(TMD[i].trtype==TD[FieldTrainerID].type)
				break;
			i++;
		}
		TTMD=(TrainerTypeMultiData *)TD[FieldTrainerID].TPD;
		money=level*4*UserWork[OKANE2BAI]*(FightType&FIGHT_TYPE_2vs2?2:1)*TMD[i].money;
	}
	AddGold(&Fld.my_gold,money);
//	PM_NumMsgSet(EvStrBuf0,money,NUM_MODE_LEFT,5);
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_NUM;
	EvStrBuf0[2]=4;
	EvStrBuf0[3]=5;
	EvStrBuf0[4]=(money&0x000000ff)>> 0;
	EvStrBuf0[5]=(money&0x0000ff00)>> 8;
	EvStrBuf0[6]=(money&0x00ff0000)>>16;
	EvStrBuf0[7]=(money&0xff000000)>>24;
	EvStrBuf0[8]=EOM_;

	WazaSeqAdrs++;
}

void	WS_WAZA_GET(void)
{
	int	i;
	PokemonServerParam	*PSP2;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
	
	switch(ServerWork[0]){
		case 0:
			BSS_POKEPARA_GET_SET(BSS_SERVER,BSS_PARA_FIGHT,BSS_SELMONSNO);
			ClientBitSet(ClientNo);
			ServerWork[0]++;
			break;
		case 1:
			if(ClientBit==0){
				PSP2=(PokemonServerParam *)&ClientBuffer[ClientNo][4];
				for(i=0;i<4;i++){
					PSP[ClientNo].waza[i]=PSP2->waza[i];
					PSP[ClientNo].pp[i]=PSP2->pp[i];
				}
				WazaSeqAdrs+=2;
			}
			break;
	}
}

void	WS_SWITCH_A_D(void)
{
	ClientNo=AttackClient;
	AttackClient=DefenceClient;
	DefenceClient=ClientNo;

	if(ServerStatusFlag&SERVER_STATUS_FLAG_A2D)
		ServerStatusFlag&=SERVER_STATUS_FLAG_A2D_OFF;
	else
		ServerStatusFlag|=SERVER_STATUS_FLAG_A2D;

	WazaSeqAdrs++;
}

void	WS_INC_RECORD(void)
{
	if(MineEnemyCheck(AttackClient)==SIDE_MINE)
		IncRecord(WazaSeqAdrs[1]);

	WazaSeqAdrs+=2;
}

void	WS_BALL_GAUGE_SET(void)
{
	int	i;
	BallGaugePara	BGP[6];
	PokemonParam	*PP;
	
	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else
			ClientNo=KizetsuClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
	
		if(MineEnemyCheck(ClientNo)==SIDE_MINE)
			PP=&PokeParaMine[0];
		else
			PP=&PokeParaEnemy[0];
	
		for(i=0;i<6;i++){
			if((PokeParaGet(&PP[i],ID_monsno_egg)==0)||
			   (PokeParaGet(&PP[i],ID_monsno_egg)==MONSNO_TAMAGO)){
				BGP[i].hp=0xffff;
				BGP[i].condition=0;
			}
			else{
				BGP[i].hp=PokeParaGet(&PP[i],ID_hp);
				BGP[i].condition=PokeParaGet(&PP[i],ID_condition);
			}
		}
		BSS_BALL_GAUGE_SET_SET(BSS_SERVER,(u8 *)&BGP,1);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=2;
	}
}

void	WS_BALL_GAUGE_OFF(void)
{
#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
		ClientNo=DefenceClient;
	else
		ClientNo=KizetsuClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	BSS_BALL_GAUGE_OFF_SET(BSS_SERVER);
	ClientBitSet(ClientNo);
	WazaSeqAdrs+=2;
}

void	WS_WAZASEQADRS(void)
{
	if(WazaSeqAdrs[1])
		WazaNo=PushWazaNo;
	else
		WazaNoTmp=WazaNo=PushWazaNo;
	
	WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
}

void	WS_STATUS_EFFECT(void)
{
	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else
			ClientNo=TsuikaClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
	
		if(((WazaKouka[ClientNo]&WAZAKOUKA_KIE)==0)&&
		    (WKC[ClientNo].migawarihp==0)&&
		   ((ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF)==0)){
			BSS_STATUS_EFFECT_SET(BSS_SERVER,0,PSP[ClientNo].condition);
			ClientBitSet(ClientNo);
		}
		WazaSeqAdrs+=2;
	}
}

void	WS_STATUS_EFFECT2(void)
{
	u32	condition;

	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else
			ClientNo=TsuikaClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

		condition=(WazaSeqAdrs[2]<< 0)|
				  (WazaSeqAdrs[3]<< 8)|
				  (WazaSeqAdrs[4]<<16)|
				  (WazaSeqAdrs[5]<<24);
	
		if(((WazaKouka[ClientNo]&WAZAKOUKA_KIE)==0)&&
		    (WKC[ClientNo].migawarihp==0)&&
		   ((ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF)==0)){
			BSS_STATUS_EFFECT_SET(BSS_SERVER,1,PSP[ClientNo].condition2&condition);
			ClientBitSet(ClientNo);
		}
		WazaSeqAdrs+=6;
	}
}

void	WS_STATUS_EFFECT_DIRECT(void)
{
	u32	condition;

	if(ClientBit==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else
			ClientNo=TsuikaClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

		condition=(WazaSeqAdrs[3]<< 0)|
				  (WazaSeqAdrs[4]<< 8)|
				  (WazaSeqAdrs[5]<<16)|
				  (WazaSeqAdrs[6]<<24);
	
		if(((WazaKouka[ClientNo]&WAZAKOUKA_KIE)==0)&&
		    (WKC[ClientNo].migawarihp==0)&&
		   ((ServerStatusFlag&SERVER_STATUS_FLAG_EFF_ANM_OFF)==0)){
			BSS_STATUS_EFFECT_SET(BSS_SERVER,WazaSeqAdrs[2],condition);
			ClientBitSet(ClientNo);
		}
		WazaSeqAdrs+=7;
	}
}

enum{
	YES_NO_WINOPEN,
	YES_NO_SELECT,
};

void	WS_YES_NO_WINDOW(void)
{
	switch(ServerWork[0]){
		case YES_NO_WINOPEN:
			WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,0);
			MsgPutSet(&FightMsg,YesNoMsg,CGX_START+0x80,WIN_X1+2,WIN_Y1+1);
			MsgPut(&FightMsg);
			ServerWork[0]++;
			ServerWork[1]=0;
			YesNoCsrOn();
			break;
		case YES_NO_SELECT:
			if((sys.Trg&U_KEY)==U_KEY){
				if(ServerWork[1]){
					SePlay(SE_SELECT);
					YesNoCsrOff();
					ServerWork[1]=0;
					YesNoCsrOn();
				}
			}
			if((sys.Trg&D_KEY)==D_KEY){
				if(ServerWork[1]==0){
					SePlay(SE_SELECT);
					YesNoCsrOff();
					ServerWork[1]=1;
					YesNoCsrOn();
				}
			}
			if((sys.Trg&B_BUTTON)==B_BUTTON){
				ServerWork[1]=1;
				SePlay(SE_SELECT);
				WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
				WazaSeqAdrs++;
				break;
			}
			if((sys.Trg&A_BUTTON)==A_BUTTON){
				SePlay(SE_SELECT);
				WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,1);
				WazaSeqAdrs++;
				break;
			}
			break;
	}
}

void	WS_SCA_END_SET(void)
{
	int	i;

	for(i=0;i<ClientSetMax;i++)
		ActionNo[i]=SCA_EndNo;

	WazaSeqAdrs++;
}

void	WS_KORAERU_CHECK(void)
{
	u8	eqp;
	u8	atc;

	if(PSP[DefenceClient].item==ITEM_NAZONOMI){
//		eqp=Fld.RamSeed.item_equip;
//		atc=Fld.RamSeed.item_power;
		eqp=FRS[DefenceClient].item_equip;
		atc=FRS[DefenceClient].item_power;
	}
	else{
		eqp=ItemEquipGet(PSP[DefenceClient].item);
		atc=ItemAttackGet(PSP[DefenceClient].item);
	}

	ItemClient=DefenceClient;

	if((eqp==SOUBI_HPITIDETAERU)&&
	  ((pp_rand()%100)<atc)){
		SoubiItemSet(DefenceClient,eqp);
		OSTF[DefenceClient].itemkoraeruflag=1;
	}

	if((PSP[DefenceClient].condition2&CONDITION2_MIGAWARI)==0){//�g���肪����ꍇ�̓`�F�b�N���Ȃ�
		if((WTD[WazaNo].battleeffect==101)||		//�݂˂���
		   (OTF[DefenceClient].koraeruflag)||		//���炦��
		   //�A�C�e����HP1�őς���̑������ʂ�HP1�őς���
		   (OSTF[DefenceClient].itemkoraeruflag)){
			if(PSP[DefenceClient].hp<=WazaDamage){
				WazaDamage=PSP[DefenceClient].hp-1;
				if(OTF[DefenceClient].koraeruflag)	//���炦��
					WazaStatusFlag|=WAZASTATUSFLAG_KORAETA;
				else if(OSTF[DefenceClient].itemkoraeruflag){
					WazaStatusFlag|=WAZASTATUSFLAG_ITEM_KORAETA;
					ItemNo=PSP[DefenceClient].item;
				}
			}
		}
	}
	WazaSeqAdrs++;
}

void	WS_KILLITEM(void)
{
	u16	*itemno;

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
		ClientNo=DefenceClient;
	else
		ClientNo=UserWork[CLIENT_NO_WORK];
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	itemno=(u16 *)&UserWork[RECYCLE_WORK+ClientNo*2];

	*itemno=PSP[ClientNo].item;	//���T�C�N���p��ItemNo��ۑ�
	PSP[ClientNo].item=0;

	BSS_POKEPARA_PUT_SET(BSS_SERVER,
						 BSS_PARA_ITEM,
						 BSS_SELMONSNO,
						 2,
						 (u8 *)&PSP[ClientNo].item);
	ClientBitSet(ClientNo);

	WazaSeqAdrs+=2;
}

void	WS_RESHUFFLE_NICKNAME(void)
{
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_NICKNAMEONLY;
	EvStrBuf0[2]=AttackClient;
	EvStrBuf0[3]=SelMonsNo[AttackClient];
	EvStrBuf0[4]=EOM_;

	WazaSeqAdrs++;
}

enum{
	SW_WINDOW_INIT=0,
	SW_WINDOW_MAIN,
	SW_WINDOW_MAIN2,
	SW_END,
};

#define	SW_WIN_X1	11
#define	SW_WIN_Y1	0
#define	SW_WIN_X2	29
#define	SW_WIN_Y2	7

const u8 SWLevelUpTbl[]={
	ID_hpmax,
	ID_spepow,
	ID_pow,
	ID_spedef,
	ID_def,
	ID_agi,
};

void	WS_STATUS_WINDOW(void)
{
	int	i,flg;
	u8	*msg;
	s16	work1=0;
	s16	work2=0;
	
	switch(UserWork[SW_SEQNO]){
		case SW_WINDOW_INIT:
			WindowOpen(SW_WIN_X1,SW_WIN_Y1,SW_WIN_X2,SW_WIN_Y2,0);
			msg=PM_strcpy(&MsgExpandBuffer[0],LevelUpWindowMsg);
			for(i=0;i<6;i++){
				flg=0;
				msg=PM_strcat(msg,LevelUpWindowMsgTbl[i]);
				work1=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],SWLevelUpTbl[i]);
				switch(i){
					case 0:
						work2=work1-PUSH_POKE_PARAM->hpmax;
						break;
					case 1:
						work2=work1-PUSH_POKE_PARAM->spepow;
						break;
					case 2:
						work2=work1-PUSH_POKE_PARAM->pow;
						break;
					case 3:
						work2=work1-PUSH_POKE_PARAM->spedef;
						break;
					case 4:
						work2=work1-PUSH_POKE_PARAM->def;
						break;
					case 5:
						work2=work1-PUSH_POKE_PARAM->agi;
						break;
				}

				if(work2<0){
					work2*=-1;
					flg++;
				}

				msg[0]=spc_;
				msg[1]=EOM_;
				msg++;

				if(flg)
					msg=PM_strcat(msg,LevelUpWindowMsgMinus);
				else
					msg=PM_strcat(msg,LevelUpWindowMsgPlus);

				msg=PM_NumMsgSet(msg,work2,NUM_MODE_SPACE,2);

				if(i&1){
					msg[0]=CR_;
					msg[1]=EOM_;
					msg++;
				}
				else{
					msg[0]=spc_;
					msg[1]=EOM_;
					msg++;
				}
			}
			MsgPutSet(&FightMsg,
					  MsgExpandBuffer,
					  CGX_START+0x80,
					  SW_WIN_X1+1,
					  SW_WIN_Y1+1);
			MsgPut(&FightMsg);
			UserWork[SW_SEQNO]++;
			break;
		case SW_WINDOW_MAIN:
			if(sys.Trg){
				SePlay(SE_SELECT);
				msg=PM_strcpy(&MsgExpandBuffer[0],LevelUpWindowMsg);
				for(i=0;i<6;i++){
					flg=0;
					msg=PM_strcat(msg,LevelUpWindowMsgTbl[i]);
					work1=PokeParaGet(&PokeParaMine[UserWork[GETEXP_SELMONS]],SWLevelUpTbl[i]);
					msg[0]=spc_;
					msg[1]=EOM_;
					msg++;

					msg=PM_NumMsgSet(msg,work1,NUM_MODE_SPACE,3);
					if(i&1){
						msg[0]=CR_;
						msg[1]=EOM_;
						msg++;
					}
					else{
						msg[0]=spc_;
						msg[1]=EOM_;
						msg++;
					}
				}
				MsgPutSet(&FightMsg,
						  MsgExpandBuffer,
						  CGX_START+0x80,
						  SW_WIN_X1+1,
						  SW_WIN_Y1+1);
				MsgPut(&FightMsg);
				UserWork[SW_SEQNO]++;
			}
			break;
		case SW_WINDOW_MAIN2:
			if(sys.Trg){
				SePlay(SE_SELECT);
				WindowOpen(SW_WIN_X1,SW_WIN_Y1,SW_WIN_X2,SW_WIN_Y2,1);
				UserWork[SW_SEQNO]++;
			}
			break;
		case SW_END:
			WazaSeqAdrs++;
			break;
	}
}

void	WS_EXPFLAG_CLEAR(void)
{
	FightJoinFlagInit();
	WazaSeqAdrs++;
}

void	WS_GET_RESHUFFLE_CLIENT(void)
{
	AttackClient=ClientNoGet(BSS_CLIENT_MINE);
	WazaSeqAdrs++;
}

void	WS_VANISH_OFF(void)
{

#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
		ClientNo=DefenceClient;
	else
		ClientNo=TsuikaClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	BSS_VANISH_ON_OFF_SET(BSS_SERVER,BSS_VANISH_OFF);
	ClientBitSet(ClientNo);

	WazaSeqAdrs+=2;
}

void	WS_TOKUSYUNO_SET(void)
{
#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
		ClientNo=DefenceClient;
	else
		ClientNo=TsuikaClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	TokusyuNoSet(ClientNo,SpeabiNo);

	WazaSeqAdrs++;
}

#if 0
void	WS_SPEABI_HIT_AFTER_CHECK(void)
{
	WazaSeqAdrs++;
	SpecialAbirityCheck(SPEABI_WAZA_HIT_AFTER,DefenceClient,0,0,0);
}
#endif

void	OboeWazaNoSet(void)
{
	EvStrBuf1[0]=I_MSG;
	EvStrBuf1[1]=EV_WAZANO;
	EvStrBuf1[2]=OboeWazaNo&0xff;
	EvStrBuf1[3]=(OboeWazaNo&0xff00)>>8;
	EvStrBuf1[4]=EOM_;
}

void	WS_OBOEWAZANO_SET(void)
{
	OboeWazaNoSet();
	WazaSeqAdrs++;
}

void	WS_ESCAPE_CHECK(void)
{
	if(EscapeCheckAct(KizetsuClient))
		WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
						   WazaSeqAdrs[2]<< 8|
						   WazaSeqAdrs[3]<<16|
						   WazaSeqAdrs[4]<<24);
	else
		WazaSeqAdrs+=5;
}

void	WS_POKE_HPMAX_RATE(void)
{
	u8	clientno;
	int	rate;

	if((FightType&FIGHT_TYPE_2vs2)==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else
			ClientNo=KizetsuClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

		clientno=ClientNo^1;

		rate=PSP[clientno].hp*100/PSP[clientno].hpmax;
		if(rate==0)
			rate=1;
		if((rate>=70)||(PSP[clientno].hp==0))
			UserWork[MESSAGE_OFFSET]=0;
		else if(rate>=40)
			UserWork[MESSAGE_OFFSET]=1;
		else if(rate>=10)
			UserWork[MESSAGE_OFFSET]=2;
		else
			UserWork[MESSAGE_OFFSET]=3;
	}
	WazaSeqAdrs+=2;
}

void	WS_POKE_HPDEC_RATE(void)
{
	u8	clientno;
	int	rate;
	u16	hpmax;

	if((FightType&FIGHT_TYPE_2vs2)==0){
#if 0
		if(WazaSeqAdrs[1]==SIDE_ATTACK)
			ClientNo=AttackClient;
		else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
			ClientNo=DefenceClient;
		else
			ClientNo=KizetsuClient;
#else
		ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

		clientno=ClientNo^1;

		hpmax=UserWork[MONS_HP_TEMP+(MineEnemyCheck(clientno)*2)];
		rate=(hpmax-PSP[clientno].hp)*100/hpmax;
		if(PSP[clientno].hp>=hpmax)
			UserWork[MESSAGE_OFFSET]=0;
		else if(rate<30)
			UserWork[MESSAGE_OFFSET]=1;
		else if(rate<70)
			UserWork[MESSAGE_OFFSET]=2;
		else
			UserWork[MESSAGE_OFFSET]=3;
	}
	WazaSeqAdrs+=2;
}

void	WS_STATUS_RECOVER(void)
{
	TempClientNo=AttackClient;
	StatusRecover2(&PokeParaEnemy[SelMonsNo[AttackClient]],
				   ItemNo,
				   SelMonsNo[AttackClient],
				   0,
				   1);
	WazaSeqAdrs++;
}

void	WS_SUBROUTINE_CALL(void)
{
	int	dir;
	int	i;
	u16	*waza;

	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);

	switch(WazaSeqAdrs[2]){
		case SERVER_KEEP_OFF:
			ServerKeepOff(ClientNo);
			break;
		case MAGICCORT_A_D:
			AttackClient=DefenceClient;
			dir=MineEnemyCheck(AttackClient)^1;
			if((SCC[dir].konoyubitomareflag)&&(PSP[SCC[dir].konoyubitomareno].hp))
				DefenceClient=SCC[dir].konoyubitomareno;
			else
				DefenceClient=ClientNo;
			break;
		case TELEPORT_CHECK:
			ServerWork[0]=EscapeNGCheck();
			break;
		case DEFENCE_CLIENT_GET:
			DefenceClient=ServerDefenceClientGet(WazaNo,0);
			break;
		case CHECK_KIZETSU:
			if(ServerStatusFlag&(no2bittbl[ClientNo]<<28))
				ServerWork[0]=1;
			else
				ServerWork[0]=0;
			break;
		case RESHUFFLE_INIT:
			OSTF[ClientNo].ikakuflag=0;
			OSTF[ClientNo].traceflag=0;
			break;
		case KODAWARI_RESET:
			if((SelMonsNo[0]!=UserWork[GETEXP_SELMONS])&&
			   (SelMonsNo[2]!=UserWork[GETEXP_SELMONS]))
				break;
			if(SelMonsNo[0]==UserWork[GETEXP_SELMONS])
				ClientNo=0;
			else
				ClientNo=2;
			waza=(u16 *)&UserWork[KODAWARI_WAZANO+ClientNo*2];
			for(i=0;i<4;i++){
				if(PSP[ClientNo].waza[i]==waza[0])
					break;
			}
			if(i==4)
				waza[0]=0;
			break;
	}
	WazaSeqAdrs+=3;
}

//�Z�V�[�P���X

const u16 KakuritsuTable[]={
	0xffff/1,
	0xffff/2,
	0xffff/4,
	0xffff/8,
};

void	WS_GUARD_SUCCESS_CHECK(void)
{
	int	flag=1;

	if((WazaNoLast[AttackClient]!=WAZANO_MAMORU)&&
	   (WazaNoLast[AttackClient]!=WAZANO_MIKIRI)&&
	   (WazaNoLast[AttackClient]!=WAZANO_KORAERU))
		WKC[AttackClient].successcnt=0;

	if(ActionCount==ClientSetMax-1)
			flag=0;

	if((KakuritsuTable[WKC[AttackClient].successcnt]>pp_rand())&&(flag)){
		if(WTD[WazaNo].battleeffect==111){
			OTF[AttackClient].guardflag=1;		//�܂���A�݂���
			ServerWork[5]=0;
		}
		if(WTD[WazaNo].battleeffect==116){
			OTF[AttackClient].koraeruflag=1;	//���炦��
			ServerWork[5]=1;
		}
		WKC[AttackClient].successcnt++;
	}
	else{
		WKC[AttackClient].successcnt=0;
		ServerWork[5]=2;
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
	}

	WazaSeqAdrs++;
}

void	WS_JIBAKU_INIT(void)
{
	if(ClientBit==0){
		for(DefenceClient=0;DefenceClient<ClientSetMax;DefenceClient++){
			if(PSP[DefenceClient].speabino==TOKUSYU_SIMERIKE)
				break;
		}
		if(DefenceClient==ClientSetMax){
			ClientNo=AttackClient;
			WazaDamage=PSP[ClientNo].hp;
			BSS_HP_GAUGE_SET(BSS_SERVER,JIBAKU_HP);
			ClientBitSet(ClientNo);
			WazaSeqAdrs++;
	
			for(DefenceClient=0;DefenceClient<ClientSetMax;DefenceClient++){
				if((DefenceClient!=AttackClient)&&
				  ((NoReshuffleClient&no2bittbl[DefenceClient])==0))
								break;
			}
		}
		else{
			SpeabiNo=TOKUSYU_SIMERIKE;
			TokusyuNoSet(DefenceClient,PSP[DefenceClient].speabino);
			WazaSeqAdrs=(u8 *)&WAZA_Shimerike;
		}
	}
}

void	WS_JIBAKU(void)
{
	if(ClientBit==0){
		ClientNo=AttackClient;
		PSP[ClientNo].hp=0;
		BSS_POKEPARA_PUT_SET(BSS_SERVER,BSS_PARA_HP,BSS_SELMONSNO,2,
		(u8 *)&PSP[ClientNo].hp);
		ClientBitSet(ClientNo);
		WazaSeqAdrs++;
	}
}

void	WS_JIBAKU_LOOP(void)
{
	u8	*adr;

	adr=(u8 *)(WazaSeqAdrs[1]<< 0|
			   WazaSeqAdrs[2]<< 8|
			   WazaSeqAdrs[3]<<16|
			   WazaSeqAdrs[4]<<24);

	if(FightType&FIGHT_TYPE_2vs2){
		DefenceClient++;
		while((DefenceClient==AttackClient)||
		  (NoReshuffleClient&no2bittbl[DefenceClient])) DefenceClient++;
		if(DefenceClient>=ClientSetMax) 
			WazaSeqAdrs+=5;
		else
			WazaSeqAdrs=adr;
	}
	else
		WazaSeqAdrs+=5;

#if 0
	if(WazaSeqAdrs!=adr)
		DefenceClient=AttackClient;	//�����̋C��Ōo���l������Ȃ��悤�ɂ���	
#endif
}

void	WS_KAIFUKU(void)
{
	u8	*adr;
	adr=(u8 *)(WazaSeqAdrs[1]<< 0|
			   WazaSeqAdrs[2]<< 8|
			   WazaSeqAdrs[3]<<16|
			   WazaSeqAdrs[4]<<24);

	if(WazaSeqAdrs[5]==SIDE_ATTACK)
		DefenceClient=AttackClient;

	WazaDamage=PSP[DefenceClient].hpmax/2;
	if(WazaDamage==0)
		WazaDamage=1;
	WazaDamage*=-1;
	if(PSP[DefenceClient].hp==PSP[DefenceClient].hpmax)
		WazaSeqAdrs=adr;
	else
		WazaSeqAdrs+=6;
}

void	WS_OUMU_CHECK(void)
{
	u16	wazano;
	u16	wazano1,wazano2;
//	if((WazaNoHit[AttackClient]!=0)&&(WazaNoHit[AttackClient]!=0xffff)){
//		WazaNo=WazaNoTmp=WazaNoHit[AttackClient];
//		WazaNo=WazaNoHit[AttackClient];
	wazano=UserWork[OUMU_WAZANO+0+AttackClient*2]|
		  (UserWork[OUMU_WAZANO+1+AttackClient*2]<<8);
	wazano1=UserWork[OUMU_HIT_WAZANO+0+AttackClient*4]|
		   (UserWork[OUMU_HIT_WAZANO+1+AttackClient*4]<<8);
	wazano2=UserWork[OUMU_HIT_WAZANO+2+AttackClient*4]|
		   (UserWork[OUMU_HIT_WAZANO+3+AttackClient*4]<<8);
	if((wazano!=0)&&
	   (wazano!=0xffff)){
		ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
		WazaNo=wazano;
		DefenceClient=ServerDefenceClientGet(WazaNo,0);
		WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
	}
	else{
		if((wazano1!=0)&&(wazano2!=0)&&
		   (wazano1!=0xffff)&&(wazano2!=0xffff)){
			ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
			if(pp_rand()&1)
				WazaNo=wazano1;
			else
				WazaNo=wazano2;
			DefenceClient=ServerDefenceClientGet(WazaNo,0);
			WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
		}
		else if((wazano1!=0)&&(wazano1!=0xffff)){
			ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
			WazaNo=wazano1;
			DefenceClient=ServerDefenceClientGet(WazaNo,0);
			WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
		}
		else if((wazano2!=0)&&(wazano2!=0xffff)){
			ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
			WazaNo=wazano2;
			DefenceClient=ServerDefenceClientGet(WazaNo,0);
			WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
		}
		else{
			OSTF[AttackClient].nopressureflag=1;
			WazaSeqAdrs++;
		}
	}
}

void	WS_AMAGOI(void)
{
//��J�̎d�l�͂Ȃ�
#if 0
	if(FieldCondition&FIELD_CONDITION_OOAME){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=2;
	}
	else if(FieldCondition&FIELD_CONDITION_AME){
		FieldCondition=FIELD_CONDITION_OOAME;
		ServerWork[5]=1;
		FCC.FCCcnt=AMAGOI_COUNT;
	}
	else{
		FieldCondition=FIELD_CONDITION_AME;
		ServerWork[5]=0;
		FCC.FCCcnt=AMAGOI_COUNT;
	}
	WazaSeqAdrs++;
#else
	if(FieldCondition&FIELD_CONDITION_AMEHURI){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=2;
	}
	else{
		FieldCondition=FIELD_CONDITION_AME;
		ServerWork[5]=0;
		FCC.FCCcnt=AMAGOI_COUNT;
	}
	WazaSeqAdrs++;
#endif
}

void	WS_REFLEC(void)
{
	if(SideCondition[ClientTypeGet(AttackClient)&1]&SIDE_CONDITION_BUTSURIGUARD){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=0;
	}
	else{
		SideCondition[ClientTypeGet(AttackClient)&1]|=SIDE_CONDITION_BUTSURIGUARD;
		SCC[ClientTypeGet(AttackClient)&1].butsuriguardcnt=5;
		if((FightType&FIGHT_TYPE_2vs2)&&
		   (HitCountCheck(HCC_ATTACK)==2))
			ServerWork[5]=2;
		else
			ServerWork[5]=1;
	}
	WazaSeqAdrs++;
}

void	WS_YADORIGI(void)
{
	if((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)||
	   (WazaKouka[DefenceClient]&WAZAKOUKA_YADORARE)){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=1;
		WazaSeqAdrs++;
		return;
	}
	if((PSP[DefenceClient].type1==KUSA_TYPE)||
	   (PSP[DefenceClient].type2==KUSA_TYPE)){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=2;
		WazaSeqAdrs++;
		return;
	}
	WazaKouka[DefenceClient]|=AttackClient;
	WazaKouka[DefenceClient]|=WAZAKOUKA_YADORARE;
	ServerWork[5]=0;
	WazaSeqAdrs++;
}

void	WS_DAMAGE_CHG(void)
{
	switch(WazaSeqAdrs[1]){
		case DAMAGE_NEG:				//�v���X�}�C�i�X���]
			WazaDamage*=-1;
			break;
		case DAMAGE_HALF:				//�_���[�W����
			WazaDamage/=2;
			if(WazaDamage==0)
				WazaDamage=1;
			if((PSP[DefenceClient].hpmax/2)<WazaDamage)
				WazaDamage=PSP[DefenceClient].hpmax/2;
			break;
		case DAMAGE_BAI:				//�_���[�W�{
			WazaDamage*=2;
			break;
	}
	WazaSeqAdrs+=2;
}

void	WS_NEMURU(void)
{
	u8	*adr;
	adr=(u8 *)(WazaSeqAdrs[1]<< 0|
			   WazaSeqAdrs[2]<< 8|
			   WazaSeqAdrs[3]<<16|
			   WazaSeqAdrs[4]<<24);
	ClientNo=DefenceClient=AttackClient;
	WazaDamage=PSP[DefenceClient].hpmax*-1;
	if(PSP[DefenceClient].hp==PSP[DefenceClient].hpmax)
		WazaSeqAdrs=adr;
	else{
		if(PSP[DefenceClient].condition&0xf8)
			ServerWork[5]=1;
		else
			ServerWork[5]=0;
		PSP[DefenceClient].condition=2+1;
		BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 BSS_PARA_CONDITION,
							 BSS_SELMONSNO,
							 4,
							 (u8 *)&PSP[ClientNo].condition);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=5;
	}
}

void	WS_NEKODAMASHI(void)
{
	u8	*adr;
	adr=(u8 *)(WazaSeqAdrs[1]<< 0|
			   WazaSeqAdrs[2]<< 8|
			   WazaSeqAdrs[3]<<16|
			   WazaSeqAdrs[4]<<24);
	if(WKC[AttackClient].nekodamashicnt)
		WazaSeqAdrs+=5;
	else
		WazaSeqAdrs=adr;
}

void	WS_SAWAGU(void)
{
//	DefenceClient=GetDefenceClient(AttackClient);
//	if(ServerStatusFlag&no2bittbl[ClientTypeGet(DefenceClient)])
//	if(NoReshuffleClient&no2bittbl[DefenceClient])
//		DefenceClient^=2;
		WazaSeqAdrs++;
}

#if 0
u8	SawaguCheck()
{
	int	i;

	for(i=0;i<ClientSetMax;i++){
		if((PSP[i].condition2&CONDITION2_SAWAGU)&&
		   (PSP[DefenceClient].speabino!=TOKUSYU_BOUON)){
			UserWork[CLIENT_NO_WORK]=i;
			if(DefenceClient==0xff)
				DefenceClient=i;
			else if(DefenceClient==i)
				ServerWork[5]=0;
			else
				ServerWork[5]=1;
			break;
		}
	}
	if(i==ClientSetMax)
		return 0;
	else
		return 1;

}
#else
u8	SawaguCheck(u8 clientno)
{
	int	i;

	for(i=0;i<ClientSetMax;i++){
		if((PSP[i].condition2&CONDITION2_SAWAGU)&&
		   (PSP[clientno].speabino!=TOKUSYU_BOUON)){
			UserWork[CLIENT_NO_WORK]=i;
			if(DefenceClient==0xff)
				DefenceClient=i;
			else if(DefenceClient==i)
				ServerWork[5]=0;
			else
				ServerWork[5]=1;
			break;
		}
	}
	if(i==ClientSetMax)
		return 0;
	else
		return 1;

}
#endif

void	WS_SAWAGU_CHECK(void)
{
	u8	*adr;
	adr=(u8 *)(WazaSeqAdrs[1]<< 0|
			   WazaSeqAdrs[2]<< 8|
			   WazaSeqAdrs[3]<<16|
			   WazaSeqAdrs[4]<<24);
	if(SawaguCheck(DefenceClient))
		WazaSeqAdrs=adr;
	else{
		if((PSP[DefenceClient].speabino==TOKUSYU_HUMIN)||
		   (PSP[DefenceClient].speabino==TOKUSYU_YARUKI)){
			SpeabiNo=PSP[DefenceClient].speabino;
			ServerWork[5]=2;
			WazaSeqAdrs=adr;
			TokusyuNoSet(DefenceClient,SpeabiNo);
		}
		else
			WazaSeqAdrs+=5;
	}
}

void	WS_TAKUWAERU(void)
{
	if(WKC[AttackClient].takuwaerucnt==3){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=1;
	}
	else{
		WKC[AttackClient].takuwaerucnt++;
//		EvStrBuf0[0]=n0_+WKC[AttackClient].takuwaerucnt;
//		EvStrBuf0[1]=EOM_;
		EvStrBuf0[0]=I_MSG;
		EvStrBuf0[1]=EV_NUM;
		EvStrBuf0[2]=1;
		EvStrBuf0[3]=1;
		EvStrBuf0[4]=WKC[AttackClient].takuwaerucnt;
		EvStrBuf0[5]=EOM_;

		ServerWork[5]=0;
	}
	WazaSeqAdrs++;
}

void	WS_HAKIDASU(void)
{
	u8	*adr;

	adr=(u8 *)(WazaSeqAdrs[1]<< 0|
			   WazaSeqAdrs[2]<< 8|
			   WazaSeqAdrs[3]<<16|
			   WazaSeqAdrs[4]<<24);
	if(WKC[AttackClient].takuwaerucnt==0)
		WazaSeqAdrs=adr;
	else{
		//�܂���ꂽ�Ƃ��̓J�E���^�����N���A����
		if(ServerWork[6]!=MSG_MAMORUNOHIT){
			WazaDamage=
				WazaDamageCalc(&PSP[AttackClient],
							   &PSP[DefenceClient],
							   WazaNo,
							   SideCondition[ClientTypeGet(DefenceClient)&1],
							   0,
							   0,
							   AttackClient,
							   DefenceClient)*
							   WKC[AttackClient].takuwaerucnt;
			//�Z�G�t�F�N�g�p�ɃJ�E���^��ێ����Ă���
			UserWork[WAZA_EFF_CNT]=WKC[AttackClient].takuwaerucnt;
			if(OTF[AttackClient].tedasukeflag)
				WazaDamage=WazaDamage*15/10;
		}
		WKC[AttackClient].takuwaerucnt=0;
		WazaSeqAdrs+=5;
	}
}

void	WS_NOMIKOMU(void)
{
	u8	*adr;

	adr=(u8 *)(WazaSeqAdrs[1]<< 0|
			   WazaSeqAdrs[2]<< 8|
			   WazaSeqAdrs[3]<<16|
			   WazaSeqAdrs[4]<<24);
	if(WKC[AttackClient].takuwaerucnt==0){
		WazaSeqAdrs=adr;
		ServerWork[5]=0;
	}
	else if(PSP[AttackClient].hpmax==PSP[AttackClient].hp){
		WKC[AttackClient].takuwaerucnt=0;
		WazaSeqAdrs=adr;
		DefenceClient=AttackClient;
		ServerWork[5]=1;
	}
	else{
		WazaDamage=
		(PSP[AttackClient].hpmax/(1<<(3-WKC[AttackClient].takuwaerucnt)));
		if(WazaDamage==0)
			WazaDamage=1;
		WazaDamage*=-1;
		//�Z�G�t�F�N�g�p�ɃJ�E���^��ێ����Ă���
		UserWork[WAZA_EFF_CNT]=WKC[AttackClient].takuwaerucnt;
		WKC[AttackClient].takuwaerucnt=0;
		WazaSeqAdrs+=5;
		DefenceClient=AttackClient;
	}
}

void	WS_SUITORI(void)
{
	WazaDamage=-(HitDamage/2);
	if(WazaDamage==0) WazaDamage=-1;

	WazaSeqAdrs++;
}

u8		CondChgAct(s8 value,u8 type,u8 flag,u8 *retadrs)
{
	int	i;
	u8	tokusei=0;
	u8	ikaku=0;

	if(flag&TSUIKA_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;

	flag&=TSUIKA_ATTACK_OFF;

	if(flag&NOCHECK_TOKUSEI)
		tokusei++;

	flag&=NOCHECK_TOKUSEI_OFF;
	
	if(flag&IKAKU_CONDCHG)
		ikaku++;

	flag&=IKAKU_CONDCHG_OFF;

//	PM_strcpy(EvStrBuf0,AbirityMsg[type]);
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_STATUS;
	EvStrBuf0[2]=type;
	EvStrBuf0[3]=EOM_;

	if(value<0){
		//���낢����Ɏ���Ă�������ʂȂ�
		if((SCC[ClientTypeGet(ClientNo)&1].shiroikiricnt!=0)&&
		   (tokusei==0)&&		//�����`�F�b�N����
		   (WazaNo!=WAZANO_NOROI)){		//�̂낢�ɂ́A���������Ȃ�
			if(flag==TSUIKA_DIRECT){
				if(OSTF[ClientNo].clearbodyflag)
					WazaSeqAdrs=retadrs;
				else{
					PushAdrsSet(retadrs);
					UserWork[CLIENT_NO_WORK]=ClientNo;
					WazaSeqAdrs=(u8 *)&WAZA_Shiroikiri;
					OSTF[ClientNo].clearbodyflag=1;
				}
			}
			return 1;
		}
		//�̂낢�A�������ɂ́A���������Ȃ�
		if((WazaNo!=WAZANO_NOROI)&&(ikaku!=1)){
			if(GuardCheck(0)){			//�܂���A�݂���
				WazaSeqAdrs=(u8 *)&WAZA_Umakukimaran;
				return 1;
			}
		}
		//����\�̓N���A�{�f�B�A���낢���ނ�͔\�͒ቺ�������Ȃ�
		if(((PSP[ClientNo].speabino==TOKUSYU_KURIABODHI)||
		    (PSP[ClientNo].speabino==TOKUSYU_SIROIKEMURI))&&
			(tokusei==0)&&		//�����`�F�b�N����
			(WazaNo!=WAZANO_NOROI)){		//�̂낢�ɂ́A���������Ȃ�
			if(flag==TSUIKA_DIRECT){
				if(OSTF[ClientNo].clearbodyflag)
					WazaSeqAdrs=retadrs;
				else{
					PushAdrsSet(retadrs);
					UserWork[CLIENT_NO_WORK]=ClientNo;
					WazaSeqAdrs=(u8 *)&WAZA_ClearBody;
					SpeabiNo=PSP[ClientNo].speabino;
					TokusyuNoSet(ClientNo,SpeabiNo);
					OSTF[ClientNo].clearbodyflag=1;
				}
			}
			return 1;
		}
		//����\�͂���ǂ��߂͖�������������Ȃ�
		if((PSP[ClientNo].speabino==TOKUSYU_SURUDOIME)&&
		   (tokusei==0)&&		//�����`�F�b�N����
		   (type==COND_HIT)){
			if(flag==TSUIKA_DIRECT){
				PushAdrsSet(retadrs);
				UserWork[CLIENT_NO_WORK]=ClientNo;
				WazaSeqAdrs=(u8 *)&WAZA_Surudoime;
				SpeabiNo=PSP[ClientNo].speabino;
				TokusyuNoSet(ClientNo,SpeabiNo);
			}
			return 1;
		}
		//����\�͂����肫�o�T�~�͍U���͂�������Ȃ�
		if((PSP[ClientNo].speabino==TOKUSYU_KAIRIKIBASAMI)&&
		   (tokusei==0)&&		//�����`�F�b�N����
		   (type==COND_POW)){
			if(flag==TSUIKA_DIRECT){
				PushAdrsSet(retadrs);
				UserWork[CLIENT_NO_WORK]=ClientNo;
				WazaSeqAdrs=(u8 *)&WAZA_Kairikibasami;
				SpeabiNo=PSP[ClientNo].speabino;
				TokusyuNoSet(ClientNo,SpeabiNo);
			}
			return 1;
		}
		//����\�͂��Ղ�́A�ǉ����ʂł̔\�͒ቺ�������Ȃ�
		if((PSP[ClientNo].speabino==TOKUSYU_RINPUN)&&
		   (flag==TSUIKA_INDIRECT))
			return 1;

		value=-((value>>4)&7);
#if 0
		EvStrBuf1[0]=EOM_;
		if(value==-2)
			PM_strcpy(EvStrBuf1,GakuttoMsg);
		PM_strcat(EvStrBuf1,SagattaMsg);
#endif
		i=0;
		EvStrBuf1[i++]=I_MSG;
		if(value==-2){
			EvStrBuf1[i++]=EV_MSGID;
			EvStrBuf1[i++]=GAKUTTOMSGNO&0xff;
			EvStrBuf1[i++]=(GAKUTTOMSGNO&0xff00)>>8;
		}
		EvStrBuf1[i++]=EV_MSGID;
		EvStrBuf1[i++]=SAGATTAMSGNO&0xff;
		EvStrBuf1[i++]=(SAGATTAMSGNO&0xff00)>>8;
		EvStrBuf1[i]=EOM_;

		if(PSP[ClientNo].abiritycnt[type]==0)
			ServerWork[5]=2;
		else
			ServerWork[5]=(DefenceClient==ClientNo);
	}
	else{
		value=(value>>4)&7;
#if 0
		EvStrBuf1[0]=EOM_;
		if(value==2)
			PM_strcpy(EvStrBuf1,GuntoMsg);
		PM_strcat(EvStrBuf1,AgattaMsg);
#endif
		i=0;
		EvStrBuf1[i++]=I_MSG;
		if(value==2){
			EvStrBuf1[i++]=EV_MSGID;
			EvStrBuf1[i++]=GUNTOMSGNO&0xff;
			EvStrBuf1[i++]=(GUNTOMSGNO&0xff00)>>8;
		}
		EvStrBuf1[i++]=EV_MSGID;
		EvStrBuf1[i++]=AGATTAMSGNO&0xff;
		EvStrBuf1[i++]=(AGATTAMSGNO&0xff00)>>8;
		EvStrBuf1[i]=EOM_;

		if(PSP[ClientNo].abiritycnt[type]==12)
			ServerWork[5]=2;
		else
			ServerWork[5]=(DefenceClient==ClientNo);
	}

	PSP[ClientNo].abiritycnt[type]+=value;

	if(PSP[ClientNo].abiritycnt[type]<0)
		PSP[ClientNo].abiritycnt[type]=0;

	if(PSP[ClientNo].abiritycnt[type]>12)
		PSP[ClientNo].abiritycnt[type]=12;

	if((ServerWork[5]==2)&&(flag&TSUIKA_DIRECT))
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;

	if((ServerWork[5]==2)&&((flag&TSUIKA_DIRECT)==0))
		return 1;

	return 0;
}

void	WS_CONDCHG(void)
{
	u8	*adrs;

	adrs=(u8 *)((WazaSeqAdrs[2]<< 0)|
				(WazaSeqAdrs[3]<< 8)|
				(WazaSeqAdrs[4]<<16)|
				(WazaSeqAdrs[5]<<24));

	if(CondChgAct(UserWork[COND_CHG_WORK]&0xf0,
				  UserWork[COND_CHG_WORK]&0x0f,
				  WazaSeqAdrs[1],
				  adrs)==0)
		WazaSeqAdrs+=6;
}

void	WS_KUROIKIRI(void)
{
	int	i,j;

	for(i=0;i<ClientSetMax;i++){
		for(j=COND_HP;j<COND_MAX;j++){
			PSP[i].abiritycnt[j]=6;
		}
	}
	WazaSeqAdrs++;
}

void	WS_GAMAN(void)
{
	PSP[AttackClient].condition2|=CONDITION2_KEEP;
	WazaNoKeep[AttackClient]=WazaNo;
	StoreDamage[AttackClient]=0;
	PSP[AttackClient].condition2|=0x200;	//���܂�2�^�[�����Z�b�g�i�d�l�ύX�j
	WazaSeqAdrs++;
}

void	WS_ABARERU(void)
{
	if((PSP[AttackClient].condition2&CONDITION2_ABARERU)==0){
#if 0
		PSP[AttackClient].condition2|=CONDITION2_KEEP;
		WazaNoKeep[AttackClient]=WazaNo;
		//���΂��2-3�^�[���Z�b�g
		PSP[AttackClient].condition2|=(((pp_rand()&1)+2)<<ABARERU_SHIFT);	
#endif
		ServerWork[3]=COND2_ABARERU+TSUIKA_ATTACK;
	}
	WazaSeqAdrs++;
}

void	WS_RENZOKU(void)
{
	if(WazaSeqAdrs[1])
		WazaSeqLoopCount=WazaSeqAdrs[1];
	else{
		if((WazaSeqLoopCount=pp_rand()&3)<2)
			WazaSeqLoopCount+=2;
		else
			WazaSeqLoopCount=(pp_rand()&3)+2;
	}
	WazaSeqAdrs+=2;
}

void	WS_RENZOKU_NUM_INIT(void)
{
#if 0
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_NUM;
	EvStrBuf0[2]=1;
	EvStrBuf0[3]=1;
	EvStrBuf0[4]=0;
	EvStrBuf0[5]=EOM_;
#else
	UserWork[RENZOKU_COUNT+0]=I_MSG;
	UserWork[RENZOKU_COUNT+1]=EV_NUM;
	UserWork[RENZOKU_COUNT+2]=1;
	UserWork[RENZOKU_COUNT+3]=1;
	UserWork[RENZOKU_COUNT+4]=0;
	UserWork[RENZOKU_COUNT+5]=EOM_;
#endif
	WazaSeqAdrs++;
}

u8	HukitobashiCheck(void)
{
	u16	level;

	if(PSP[AttackClient].level>=PSP[DefenceClient].level){
		UserWork[BEFORE_RESHUFFLE_SELMONSNO+DefenceClient]=SelMonsNo[DefenceClient];
		WazaSeqAdrs=(u8 *)&WAZA_Hukitobashi;
		return 1;
	}
	else{
		level=((
				(pp_rand()&0xff)*
				(PSP[AttackClient].level+PSP[DefenceClient].level)
			)>>8)+1;
		if(level>(PSP[DefenceClient].level/4)){
			UserWork[BEFORE_RESHUFFLE_SELMONSNO+DefenceClient]=SelMonsNo[DefenceClient];
			WazaSeqAdrs=(u8 *)&WAZA_Hukitobashi;
			return 1;
		}
		else{
			WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
							   WazaSeqAdrs[2]<< 8|
							   WazaSeqAdrs[3]<<16|
							   WazaSeqAdrs[4]<<24);
			return 0;
		}
	}
}

void	WS_HUKITOBASHI(void)
{
	PokemonParam	*PP;
	u8	cnt,i,j;

	if(FightType&FIGHT_TYPE_TRAINER){
		if(MineEnemyCheck(DefenceClient)==SIDE_MINE)
			PP=&PokeParaMine[0];
		else
			PP=&PokeParaEnemy[0];

		if(FightType&FIGHT_TYPE_4_2vs2){
			cnt=0;
			j=0;
			if(GetTeamPos2(GetTeamPosID(DefenceClient))==1)
				j=3;
			for(i=j;i<j+3;i++){
				if((PokeParaGet(&PP[i],ID_monsno)!=0)&&
				   (PokeParaGet(&PP[i],ID_tamago_flag)==0)&&
				   (PokeParaGet(&PP[i],ID_hp)!=0))
					cnt++;
			}
		}
		else{
			cnt=0;
			for(i=0;i<6;i++){
				if((PokeParaGet(&PP[i],ID_monsno)!=0)&&
				   (PokeParaGet(&PP[i],ID_tamago_flag)==0)&&
				   (PokeParaGet(&PP[i],ID_hp)!=0))
					cnt++;
			}
		}
		if((cnt<=1)&&(((FightType&FIGHT_TYPE_2vs2)==0)||(FightType&FIGHT_TYPE_4_2vs2)))
			WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
							   WazaSeqAdrs[2]<< 8|
							   WazaSeqAdrs[3]<<16|
							   WazaSeqAdrs[4]<<24);
		else if((cnt<=2)&&(FightType&FIGHT_TYPE_2vs2)&&((FightType&FIGHT_TYPE_4_2vs2)==0))
			WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
							   WazaSeqAdrs[2]<< 8|
							   WazaSeqAdrs[3]<<16|
							   WazaSeqAdrs[4]<<24);
		else{
			if(HukitobashiCheck()){
				if(FightType&FIGHT_TYPE_4_2vs2){
					do{
						do{
							j=pp_rand()%3;
							if(GetTeamPos2(GetTeamPosID(DefenceClient))==1)
								i=j+3;
							else
								i=j;
						}while((i==SelMonsNo[DefenceClient])||
							   (i==SelMonsNo[DefenceClient^2]));
					}while((PokeParaGet(&PP[i],ID_monsno)==0)||
						   (PokeParaGet(&PP[i],ID_tamago_flag)==1)||
						   (PokeParaGet(&PP[i],ID_hp)==0));
				}
				else if(FightType&FIGHT_TYPE_2vs2){
					do{
						do{
							i=pp_rand()%6;
						}while((i==SelMonsNo[DefenceClient])||
							   (i==SelMonsNo[DefenceClient^2]));
					}while((PokeParaGet(&PP[i],ID_monsno)==0)||
						   (PokeParaGet(&PP[i],ID_tamago_flag)==1)||
						   (PokeParaGet(&PP[i],ID_hp)==0));
				}
				else{
					do{
						do{
							i=pp_rand()%6;
						}while(i==SelMonsNo[DefenceClient]);
					}while((PokeParaGet(&PP[i],ID_monsno)==0)||
						   (PokeParaGet(&PP[i],ID_tamago_flag)==1)||
						   (PokeParaGet(&PP[i],ID_hp)==0));
				}
//				SelMonsNo[DefenceClient]=i;
				UserWork[AFTER_RESHUFFLE_SELMONSNO+DefenceClient]=i;

#if 0
				if((MineEnemyCheck(DefenceClient)==SIDE_MINE)&&
				   (MultiCheck()==FALSE))
					ServerListRowSet(DefenceClient);
#else
				if(MultiCheck()==FALSE)
					ServerListRowSet(DefenceClient);
#endif
				ListRowExChange(DefenceClient,i,0);
				ListRowExChange(DefenceClient^2,i,1);
			}
		}
	}
	else
		HukitobashiCheck();
}

void	WS_TEXTURE(void)
{
	u8	*adr;
	u8	cnt,i;
	u8	wazatype;

	for(cnt=0;cnt<4;cnt++){
		if(PSP[AttackClient].waza[cnt]==0) break;
	}
	for(i=0;i<cnt;i++){
		wazatype=WTD[PSP[AttackClient].waza[i]].wazatype;
		if(wazatype==HATE_TYPE){
			if((PSP[AttackClient].type1==GHOST_TYPE)||
			   (PSP[AttackClient].type2==GHOST_TYPE))
				wazatype=GHOST_TYPE;
			else
				wazatype=NORMAL_TYPE;
		}
		if((wazatype!=PSP[AttackClient].type1)&&
		   (wazatype!=PSP[AttackClient].type2))
			break;
	}
	if(i==cnt){
		adr=(u8 *)(WazaSeqAdrs[1]<< 0|
				   WazaSeqAdrs[2]<< 8|
				   WazaSeqAdrs[3]<<16|
				   WazaSeqAdrs[4]<<24);
		WazaSeqAdrs=adr;
	}
	else{
		do{
			do{
				i=pp_rand()&3;
			}while(i>=cnt);
			wazatype=WTD[PSP[AttackClient].waza[i]].wazatype;
			if(wazatype==HATE_TYPE){
				if((PSP[AttackClient].type1==GHOST_TYPE)||
				   (PSP[AttackClient].type2==GHOST_TYPE))
					wazatype=GHOST_TYPE;
				else
					wazatype=NORMAL_TYPE;
			}
		}while((wazatype==PSP[AttackClient].type1)||
			   (wazatype==PSP[AttackClient].type2));
		PSP[AttackClient].type1=wazatype;
		PSP[AttackClient].type2=wazatype;
//		PM_strcpy(EvStrBuf0,ZokuseiMsg[wazatype]);
		EvStrBuf0[0]=I_MSG;
		EvStrBuf0[1]=EV_ZOKUSEI;
		EvStrBuf0[2]=wazatype;
		EvStrBuf0[3]=EOM_;

		WazaSeqAdrs+=5;
	}
}

void	WS_KOBAN_CHECK(void)
{
//	u32	gold;

	if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&(KobanCounter!=0)){
#if 0
		gold=Fld.my_gold;
		Fld.my_gold+=KobanCounter;
		if(gold>Fld.my_gold)
			Fld.my_gold=0xffffffff;
#endif
		AddGold(&Fld.my_gold,KobanCounter*UserWork[OKANE2BAI]);
//		PM_NumMsgSet(EvStrBuf0,KobanCounter,NUM_MODE_LEFT,5);
		EvStrBuf0[0]=I_MSG;
		EvStrBuf0[1]=EV_NUM;
		EvStrBuf0[2]=2;
		EvStrBuf0[3]=5;
		EvStrBuf0[4]=(KobanCounter&0x00ff)>>0;
		EvStrBuf0[5]=(KobanCounter&0xff00)>>8;
		EvStrBuf0[6]=EOM_;

//		RetWazaSeqAdrs=WazaSeqAdrs+1;
		PushAdrsSet(WazaSeqAdrs+1);
		WazaSeqAdrs=(u8 *)&WAZA_Nekonikoban;
	}
	else
		WazaSeqAdrs++;
}

void	WS_HIKARINOKABE(void)
{
	if(SideCondition[ClientTypeGet(AttackClient)&1]&SIDE_CONDITION_TOKUSYUGUARD){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=0;
	}
	else{
		SideCondition[ClientTypeGet(AttackClient)&1]|=SIDE_CONDITION_TOKUSYUGUARD;
		SCC[ClientTypeGet(AttackClient)&1].tokusyuguardcnt=5;
		if((FightType&FIGHT_TYPE_2vs2)&&
		   (HitCountCheck(HCC_ATTACK)==2))
			ServerWork[5]=4;
		else
			ServerWork[5]=3;
	}
	WazaSeqAdrs++;
}

void	WS_ICHIGEKI(void)
{
	u16	hit;
	u8	eqp;
	u8	atc;

	if(PSP[DefenceClient].item==ITEM_NAZONOMI){
//		eqp=Fld.RamSeed.item_equip;
//		atc=Fld.RamSeed.item_power;
		eqp=FRS[DefenceClient].item_equip;
		atc=FRS[DefenceClient].item_power;
	}
	else{
		eqp=ItemEquipGet(PSP[DefenceClient].item);
		atc=ItemAttackGet(PSP[DefenceClient].item);
	}

	ItemClient=DefenceClient;

	if((eqp==SOUBI_HPITIDETAERU)&&
	  ((pp_rand()%100)<atc)){
		SoubiItemSet(DefenceClient,eqp);
		OSTF[DefenceClient].itemkoraeruflag=1;
	}

	if(PSP[DefenceClient].speabino==TOKUSYU_GANZYOU){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		SpeabiNo=TOKUSYU_GANZYOU;
		WazaSeqAdrs=(u8 *)&WAZA_Ganzyou;
		TokusyuNoSet(DefenceClient,SpeabiNo);
	}
	else{
		if((WazaKouka[DefenceClient]&WAZAKOUKA_LOCKON)==0){
			hit=WTD[WazaNo].hitprobability+
//				(PSP[AttackClient].level-PSP[DefenceClient].level)*2;
				(PSP[AttackClient].level-PSP[DefenceClient].level);
			if(((pp_rand()%100)+1<hit)&&
				(PSP[AttackClient].level>=PSP[DefenceClient].level))
				hit=1;
			else
				hit=0;
		}
		else{
			if((WKC[DefenceClient].lockonclientno==AttackClient)&&
			   (PSP[AttackClient].level>=PSP[DefenceClient].level))
				hit=1;
			else
				hit=0;
		}
		if(hit){
			if(OTF[DefenceClient].koraeruflag){		//���炦���D��
				WazaDamage=PSP[DefenceClient].hp-1;
				WazaStatusFlag|=WAZASTATUSFLAG_KORAETA;
			}
			else if(OSTF[DefenceClient].itemkoraeruflag){
				WazaDamage=PSP[DefenceClient].hp-1;
				WazaStatusFlag|=WAZASTATUSFLAG_ITEM_KORAETA;
				ItemNo=PSP[DefenceClient].item;
			}
			else{
				WazaDamage=PSP[DefenceClient].hp;
				WazaStatusFlag|=WAZASTATUSFLAG_ICHIGEKI;
			}
			WazaSeqAdrs+=5;
		}
		else{
			WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
			if(PSP[AttackClient].level>=PSP[DefenceClient].level)
				ServerWork[5]=0;
			else
				ServerWork[5]=1;

			WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
							   WazaSeqAdrs[2]<< 8|
							   WazaSeqAdrs[3]<<16|
							   WazaSeqAdrs[4]<<24);
		}
	}
}

void	WS_HPHALF(void)
{
	WazaDamage=PSP[DefenceClient].hp/2;
	if(WazaDamage==0) WazaDamage=1;
	WazaSeqAdrs++;
}

void	WS_SUNAARASHI(void)
{
	if(FieldCondition&FIELD_CONDITION_SUNAARASHIING){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=2;
	}
	else{
		FieldCondition=FIELD_CONDITION_SUNAARASHI;
		ServerWork[5]=3;
		FCC.FCCcnt=SUNAARASHI_COUNT;
	}
	WazaSeqAdrs++;
}

void	WS_WEATHER_KEEP(void)
{
	//����\�͂̂��Ă񂫂́A�V��_���[�W���󂯂Ȃ�
	if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_NOOTENKI,0,0)==0)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_EAROKKU,0,0)==0)){
		if(FieldCondition&FIELD_CONDITION_SUNAARASHIING){
			if((PSP[AttackClient].type1!=IWA_TYPE)&&
			   (PSP[AttackClient].type1!=METAL_TYPE)&&
			   (PSP[AttackClient].type1!=JIMEN_TYPE)&&
			   (PSP[AttackClient].type2!=IWA_TYPE)&&
			   (PSP[AttackClient].type2!=METAL_TYPE)&&
			   (PSP[AttackClient].type2!=JIMEN_TYPE)&&
			   (PSP[AttackClient].speabino!=TOKUSYU_SUNAGAKURE)&&
			  ((WazaKouka[AttackClient]&WAZAKOUKA_ANAWOHORU)==0)&&
			  ((WazaKouka[AttackClient]&WAZAKOUKA_DAIBINGU)==0)){
				WazaDamage=PSP[AttackClient].hpmax/16;
				if(WazaDamage==0)
					WazaDamage=1;
			}
			else
				WazaDamage=0;
		}
		if(FieldCondition&FIELD_CONDITION_ARARE){
			if((PSP[AttackClient].type1!=KOORI_TYPE)&&
			   (PSP[AttackClient].type2!=KOORI_TYPE)&&
			  ((WazaKouka[AttackClient]&WAZAKOUKA_ANAWOHORU)==0)&&
			  ((WazaKouka[AttackClient]&WAZAKOUKA_DAIBINGU)==0)){
				WazaDamage=PSP[AttackClient].hpmax/16;
				if(WazaDamage==0)
					WazaDamage=1;
			}
			else
				WazaDamage=0;
		}
	}
	else
		WazaDamage=0;

	if(NoReshuffleClient&no2bittbl[AttackClient])
		WazaDamage=0;

	WazaSeqAdrs++;
}

void	WS_MEROMERO(void)
{
	PokemonParam	*PPA,*PPD;
	u16	MonsNoA;
	u16	MonsNoD;
	u32	P_RndA;
	u32	P_RndD;

	if(MineEnemyCheck(AttackClient)==SIDE_MINE)
		PPA=&PokeParaMine[SelMonsNo[AttackClient]];
	else
		PPA=&PokeParaEnemy[SelMonsNo[AttackClient]];

	if(MineEnemyCheck(DefenceClient)==SIDE_MINE)
		PPD=&PokeParaMine[SelMonsNo[DefenceClient]];
	else
		PPD=&PokeParaEnemy[SelMonsNo[DefenceClient]];

	MonsNoA=PokeParaGet(PPA,ID_monsno);
	P_RndA=PokeParaGet(PPA,ID_personal_rnd);
	MonsNoD=PokeParaGet(PPD,ID_monsno);
	P_RndD=PokeParaGet(PPD,ID_personal_rnd);

	if(PSP[DefenceClient].speabino==TOKUSYU_DONKAN){
	//����\�͂ǂ񂩂�̓��������ɂȂ�Ȃ�
		WazaSeqAdrs=(u8 *)&WAZA_Donkan;
		SpeabiNo=TOKUSYU_DONKAN;
		TokusyuNoSet(DefenceClient,SpeabiNo);
	}
	else if(((PokeSexGetMonsNo(MonsNoA,P_RndA))==(PokeSexGetMonsNo(MonsNoD,P_RndD)))||
			(WazaKouka[DefenceClient]&WAZAKOUKA_KIE)||
			(PSP[DefenceClient].condition2&CONDITION2_MEROMERO)||
			(PokeSexGetMonsNo(MonsNoA,P_RndA)==MONS_UNKNOWN)||
			(PokeSexGetMonsNo(MonsNoD,P_RndD)==MONS_UNKNOWN))
		WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
						   WazaSeqAdrs[2]<< 8|
						   WazaSeqAdrs[3]<<16|
						   WazaSeqAdrs[4]<<24);
#if 0
	else if(((PokeSexGetMonsNo
			(PSP[AttackClient].monsno,PSP[AttackClient].personal_rnd))==
			(PokeSexGetMonsNo(
			PSP[DefenceClient].monsno,PSP[DefenceClient].personal_rnd)))||
			(WazaKouka[DefenceClient]&WAZAKOUKA_KIE)||
			(PSP[DefenceClient].condition2&CONDITION2_MEROMERO)||
			(PokeSexGetMonsNo(
			PSP[DefenceClient].monsno,PSP[DefenceClient].personal_rnd)==MONS_UNKNOWN)||
			(PokeSexGetMonsNo(
			PSP[AttackClient].monsno,PSP[AttackClient].personal_rnd)==MONS_UNKNOWN)
			)
		WazaSeqAdrs=(u8 *)(WazaSeqAdrs[1]<< 0|
						   WazaSeqAdrs[2]<< 8|
						   WazaSeqAdrs[3]<<16|
						   WazaSeqAdrs[4]<<24);
#endif
	else{
		PSP[DefenceClient].condition2|=(no2bittbl[AttackClient]<<MEROMERO_SHIFT);
		WazaSeqAdrs+=5;
	}
}

void	WS_CONDITION_GAUGE(void)
{
	if(ClientBit==0){
		if(WazaSeqAdrs[1]!=SIDE_ATTACK_DOUBLE){
#if 0
			if(WazaSeqAdrs[1]==SIDE_ATTACK)
				ClientNo=AttackClient;
			else if(WazaSeqAdrs[1]==SIDE_DEFENCE)
				ClientNo=DefenceClient;
			else if(WazaSeqAdrs[1]==SIDE_CLIENTNOWORK)
				ClientNo=UserWork[CLIENT_NO_WORK];
			else
				ClientNo=TsuikaClient;
#else
			ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif
		
			BSS_STATUS_SET_SET(BSS_SERVER,PSP[ClientNo].condition,PSP[ClientNo].condition2);
			ClientBitSet(ClientNo);
			WazaSeqAdrs+=2;
		}
		else{
			ClientNo=AttackClient;
			if((NoReshuffleClient&no2bittbl[ClientNo])==0){
				BSS_STATUS_SET_SET(BSS_SERVER,PSP[ClientNo].condition,PSP[ClientNo].condition2);
				ClientBitSet(ClientNo);
			}
			if(FightType&FIGHT_TYPE_2vs2){
				ClientNo=ClientNoGet(ClientTypeGet(AttackClient)^2);
				if((NoReshuffleClient&no2bittbl[ClientNo])==0){
					BSS_STATUS_SET_SET(BSS_SERVER,PSP[ClientNo].condition,PSP[ClientNo].condition2);
					ClientBitSet(ClientNo);
				}
			}
			WazaSeqAdrs+=2;
		}
	}
}

void	WS_SHIROIKIRI(void)
{
	if(SCC[ClientTypeGet(AttackClient)&1].shiroikiricnt!=0){
		WazaStatusFlag|=WAZASTATUSFLAG_UMAKUKIMARAN;
		ServerWork[5]=1;
	}
	else{
		SCC[ClientTypeGet(AttackClient)&1].shiroikiricnt=5;	//���낢����J�E���^
		SideCondition[ClientTypeGet(AttackClient)&1]|=SIDE_CONDITION_SHIROIKIRI;
		ServerWork[5]=0;
	}
	WazaSeqAdrs++;
}

void	WS_KIAIDAME(void)
{
	if(PSP[AttackClient].condition2&CONDITION2_KIAIDAME){
		WazaStatusFlag|=WAZASTATUSFLAG_UMAKUKIMARAN;
		ServerWork[5]=1;
	}
	else{
		PSP[AttackClient].condition2|=CONDITION2_KIAIDAME;
		ServerWork[5]=0;
	}
	WazaSeqAdrs++;
}

void	WS_HENSHIN(void)
{
	//������ƂԁA���Ȃ��ق�A�݂����A���Ȃ��΂�̎��̏�����Y�ꂸ��
	int	i;
	u8	*adr1,*adr2;
	
	WazaNoTmp=0xffff;		//�I�E���������A�A���R�[�����ł��Ȃ��悤�ɂ���
	WazaSeqAdrs++;
	
	if((PSP[DefenceClient].condition2&CONDITION2_HENSHIN)||
//	   (WKC[DefenceClient].migawarihp!=0)||
//	   (PSP[DefenceClient].monsno==MONSNO_ANNOON)||
//	   (PSP[DefenceClient].monsno==MONSNO_PATTIIRU)||
	   (WazaKouka[DefenceClient]&WAZAKOUKA_KIE)){
		WazaStatusFlag|=WAZASTATUSFLAG_UMAKUKIMARAN;
		ServerWork[5]=1;
		return;
	}

	PSP[AttackClient].condition2|=CONDITION2_HENSHIN;
	WKC[AttackClient].kanashibariwazano=0;
	WKC[AttackClient].kanashibaricnt=0;
	WKC[AttackClient].henshin_rnd=PSP[DefenceClient].personal_rnd;
	WKC[AttackClient].monomanebit=0;

//	PokeNameSet(EvStrBuf0,PSP[DefenceClient].monsno);
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_MONSNAME;
	EvStrBuf0[2]=(PSP[DefenceClient].monsno&0x00ff)>>0;
	EvStrBuf0[3]=(PSP[DefenceClient].monsno&0xff00)>>8;
	EvStrBuf0[4]=EOM_;

	adr1=(u8 *)&PSP[AttackClient];
	adr2=(u8 *)&PSP[DefenceClient];

	for(i=0;i<XtOffset(PokemonServerParam *,work)+1;i++)
		adr1[i]=adr2[i];

	for(i=0;i<4;i++){
		if(WTD[PSP[AttackClient].waza[i]].pp<5)
			PSP[AttackClient].pp[i]=
				WTD[PSP[AttackClient].waza[i]].pp;
		else
			PSP[AttackClient].pp[i]=5;
	}

#if 0
	if((ClientTypeGet(AttackClient)&1)==SIDE_MINE)
		PokemonBObjCellPalSet(PSP[AttackClient].monsno,AttackClient);
	else
		PokemonFObjCellPalSet(PSP[AttackClient].monsno,AttackClient);
#endif

//	ActAnmChg(&ActWork[PokemonStruct[AttackClient]],0);	
	ClientNo=AttackClient;
	BSS_CSRPOS_CLEAR_SET(BSS_SERVER,BSS_CSRPOS_CLEAR_WAZA);
	ClientBitSet(ClientNo);

	ServerWork[5]=0;
}

void	WS_MIGAWARI(void)
{
	u16	hpmax;
	hpmax=PSP[AttackClient].hpmax/4;
	if(hpmax==0)
		hpmax=1;
	if(PSP[AttackClient].hp<=hpmax){
		WazaDamage=0;
		ServerWork[5]=1;
	}
	else{
		WazaDamage=PSP[AttackClient].hpmax/4;
		if(WazaDamage==0)
			WazaDamage=1;
		PSP[AttackClient].condition2|=CONDITION2_MIGAWARI;
		PSP[AttackClient].condition2&=CONDITION2_SHIME_OFF;	//�g�������������A���ߕt����n�͉���
															//�O��Ƃ̐������̂���
		WKC[AttackClient].migawarihp=WazaDamage;
		ServerWork[5]=0;
		ServerStatusFlag|=SERVER_STATUS_FLAG_NO_MIGAWARI;	//�݂���肪�_���[�W���󂯂Ȃ��悤�ɂ���
	}
	WazaSeqAdrs++;
}

const u16 YubihuriNGTbl[]={
	WAZANO_YUBIWOHURU,			// ��т��ӂ�
	WAZANO_WARUAGAKI,			// ��邠����
	WAZANO_SUKETTI,				// �X�P�b�`
	WAZANO_MONOMANE,			// ���̂܂�
	0xfffe,						// ���̂܂˂ł��Ȃ��e�[�u���̃G���h
	WAZANO_KAUNTAA,				// �J�E���^�[
	WAZANO_MIRAAKOOTO,			// �~���[�R�[�g
	WAZANO_MAMORU,				// �܂���
	WAZANO_MIKIRI,				// �݂���
	WAZANO_KORAERU,				// ���炦��
	WAZANO_MITIDURE,			// �݂��Â�
	WAZANO_NEGOTO,				// �˂���
	WAZANO_DOROBOU,				// �ǂ�ڂ�
	WAZANO_KONOYUBITOMARE,		// ���̂�тƂ܂�
	WAZANO_YOKODORI,			// �悱�ǂ�
	WAZANO_TEDASUKE,			// �Ă�����
	WAZANO_HOSIGARU,			// �ق�����
	WAZANO_TORIKKU,				// �g���b�N
	WAZANO_KIAIPANTI,			// �������p���`
	0xffff
};

u8	MonomaneNGCheck(u16	wazano)
{
	int	i;

	i=0;

	while(YubihuriNGTbl[i]!=0xfffe){
		if(YubihuriNGTbl[i]==wazano)
			break;
		i++;
	}
	return(YubihuriNGTbl[i]!=0xfffe);
}

void	WS_MONOMANE(void)
{
	int	i;

	WazaNoTmp=0xffff;		//�I�E���������A�A���R�[�����ł��Ȃ��悤�ɂ���

	if((MonomaneNGCheck(WazaNoUse[DefenceClient]))||
		//�ϐg���̂��̂܂˂͎��s����
	   (PSP[AttackClient].condition2&CONDITION2_HENSHIN)||
	   (WazaNoUse[DefenceClient]==0)||
	   (WazaNoUse[DefenceClient]==0xffff)){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
	else{
		for(i=0;i<4;i++){
			if(PSP[AttackClient].waza[i]==WazaNoUse[DefenceClient])
				break;
		}
		if(i==4){
			PSP[AttackClient].waza[WazaPosNo]=WazaNoUse[DefenceClient];
			if(WTD[WazaNoUse[DefenceClient]].pp<5)
				PSP[AttackClient].pp[WazaPosNo]=WTD[WazaNoUse[DefenceClient]].pp;
			else
				PSP[AttackClient].pp[WazaPosNo]=5;
			EvStrBuf0[0]=I_MSG;
			EvStrBuf0[1]=EV_WAZANO;
			EvStrBuf0[2]=(WazaNoUse[DefenceClient]&0x00ff)>>0;
			EvStrBuf0[3]=(WazaNoUse[DefenceClient]&0xff00)>>8;
			EvStrBuf0[4]=EOM_;
			WKC[AttackClient].monomanebit|=no2bittbl[WazaPosNo];
			WazaSeqAdrs+=5;
		}
		else
			WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
							   (WazaSeqAdrs[2]<< 8)|
							   (WazaSeqAdrs[3]<<16)|
							   (WazaSeqAdrs[4]<<24));
	}
}

void	WS_YUBIWOHURU(void)
{
	int	i;
#ifdef PM_DEBUG
	u16	*wazano;

	wazano=(u16 *)&UserWork[YUBIWOHURU_DEBUG+AttackClient*2];

	if((PSP[AttackClient].waza[1]==0)&&
	   (PSP[AttackClient].waza[2])&&
	   (PSP[AttackClient].waza[3])){
		while(1){
			if(wazano[0]==0)
				wazano[0]=PSP[AttackClient].waza[2];
			WazaNo=wazano[0];
			if(PSP[AttackClient].waza[2]<PSP[AttackClient].waza[3]){
				if(wazano[0]==PSP[AttackClient].waza[3])
					wazano[0]=PSP[AttackClient].waza[2];
				else
					wazano[0]++;
			}
			else{
				if(wazano[0]==WAZANO_MAX)
					wazano[0]=1;
				else if(wazano[0]==PSP[AttackClient].waza[3])
					wazano[0]=PSP[AttackClient].waza[2];
				else
					wazano[0]++;
			}
#if 0
			i=-1;
			do{
				i++;
				if(YubihuriNGTbl[i]==WazaNo) break;
			}while(YubihuriNGTbl[i]!=0xffff);
			if(YubihuriNGTbl[i]!=0xffff) continue;
#endif
			ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
			WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
			ServerStatusFlag|=SERVER_STATUS_FLAG_NOPP_DEC;
			DefenceClient=ServerDefenceClientGet(WazaNo,0);
			return;
		}
	}
	while(1){
		WazaNo=(pp_rand()&0x1ff)+1;
		if(WazaNo>WAZANO_MAX) continue;
		for(i=0;i<4;i++){
			if(PSP[AttackClient].waza[i]==WazaNo) continue;
		}
		i=-1;
		do{
			i++;
			if(YubihuriNGTbl[i]==WazaNo) break;
		}while(YubihuriNGTbl[i]!=0xffff);
		if(YubihuriNGTbl[i]!=0xffff) continue;
		ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
		WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
		DefenceClient=ServerDefenceClientGet(WazaNo,0);
		break;
	}
#else
	while(1){
		WazaNo=(pp_rand()&0x1ff)+1;
		if(WazaNo>WAZANO_MAX) continue;
		for(i=0;i<4;i++){
			if(PSP[AttackClient].waza[i]==WazaNo) continue;
		}
		i=-1;
		do{
			i++;
			if(YubihuriNGTbl[i]==WazaNo) break;
		}while(YubihuriNGTbl[i]!=0xffff);
		if(YubihuriNGTbl[i]!=0xffff) continue;
		ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
		WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
		DefenceClient=ServerDefenceClientGet(WazaNo,0);
		break;
	}
#endif
}

void	WS_CHIKYUUNAGE(void)
{
	WazaDamage=PSP[AttackClient].level;
	WazaSeqAdrs++;
}

void	WS_PSYCHOWAVE(void)
{
	int	i;

	while(1){
		i=pp_rand()&0xf;
		if(i<11) break;
	}
	i*=10;
	WazaDamage=PSP[AttackClient].level*(50+i)/100;
	WazaSeqAdrs++;
}

void	WS_COUNTER(void)
{
	u8	dir1,dir2;

	dir1=MineEnemyCheck(AttackClient);
	dir2=MineEnemyCheck(OTF[AttackClient].butsurihitclient);

	if((OTF[AttackClient].butsurihitdamage!=0)&&
	   (dir1!=dir2)&&
	   (PSP[OTF[AttackClient].butsurihitclient].hp!=0)){
		WazaDamage=OTF[AttackClient].butsurihitdamage*2;
		if((SCC[dir2].konoyubitomareflag)&&(PSP[SCC[dir2].konoyubitomareno].hp))
			DefenceClient=SCC[dir2].konoyubitomareno;
		else
			DefenceClient=OTF[AttackClient].butsurihitclient;
		WazaSeqAdrs+=5;
	}
	else{
		OSTF[AttackClient].nopressureflag=1;
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
}
void	WS_MIRRORCOAT(void)
{
	u8	dir1,dir2;

	dir1=MineEnemyCheck(AttackClient);
	dir2=MineEnemyCheck(OTF[AttackClient].tokusyuhitclient);

	if((OTF[AttackClient].tokusyuhitdamage!=0)&&
	   (dir1!=dir2)&&
	   (PSP[OTF[AttackClient].tokusyuhitclient].hp!=0)){
		WazaDamage=OTF[AttackClient].tokusyuhitdamage*2;
		if((SCC[dir2].konoyubitomareflag)&&(PSP[SCC[dir2].konoyubitomareno].hp))
			DefenceClient=SCC[dir2].konoyubitomareno;
		else
			DefenceClient=OTF[AttackClient].tokusyuhitclient;
		WazaSeqAdrs+=5;
	}
	else{
		OSTF[AttackClient].nopressureflag=1;
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
}

void	WS_KANASHIBARI(void)
{
	int	i;

	for(i=0;i<4;i++){
		if(PSP[DefenceClient].waza[i]==WazaNoUse[DefenceClient]) break;
	}
	if((WKC[DefenceClient].kanashibariwazano==0)&&
	   (i!=4)&&(PSP[DefenceClient].pp[i]!=0)){
//		PM_strcpy(EvStrBuf0,WazaNameData[PSP[DefenceClient].waza[i]]);
		EvStrBuf0[0]=I_MSG;
		EvStrBuf0[1]=EV_WAZANO;
		EvStrBuf0[2]=(PSP[DefenceClient].waza[i]&0x00ff)>>0;
		EvStrBuf0[3]=(PSP[DefenceClient].waza[i]&0xff00)>>8;
		EvStrBuf0[4]=EOM_;

		WKC[DefenceClient].kanashibariwazano=PSP[DefenceClient].waza[i];
		WKC[DefenceClient].kanashibaricnt=(pp_rand()&0x03)+2;
		WKC[DefenceClient].kanashibaricnttmp=WKC[DefenceClient].kanashibaricnt;
//		PSP[DefenceClient].condition2&=CONDITION2_KEEP_OFF;	//���ߋZ�����Ȃ��΂����Ƃ��͂��߂��I�t
		WazaSeqAdrs+=5;
	}
	else{
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
}

void	WS_ENCORE(void)
{
	int	i;

	for(i=0;i<4;i++){
		if(PSP[DefenceClient].waza[i]==WazaNoUse[DefenceClient]) break;
	}

	if((WazaNoUse[DefenceClient]==WAZANO_WARUAGAKI)||
   	   (WazaNoUse[DefenceClient]==WAZANO_ANKOORU)||
   	   (WazaNoUse[DefenceClient]==WAZANO_OUMUGAESI))
		i=4;		//�A���R�[���ł��Ȃ��Z�̎w��

	if((WKC[DefenceClient].encorewazano==0)&&
	   (i!=4)&&(PSP[DefenceClient].pp[i]!=0)){
		WKC[DefenceClient].encorewazano=PSP[DefenceClient].waza[i];
		WKC[DefenceClient].encorewazapos=i;
		WKC[DefenceClient].encorecnt=(pp_rand()&0x03)+3;
		WKC[DefenceClient].encorecnttmp=WKC[DefenceClient].encorecnt;
//		WKC[DefenceClient].encoreclient=WazaNoDefenceClient[DefenceClient];
		WazaSeqAdrs+=5;
	}
	else{
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
}

void	WS_ITAMIWAKE(void)
{
	s32	hitpoint;

	if((PSP[DefenceClient].condition2&CONDITION2_MIGAWARI)==0){//�g���肪����ꍇ�̓`�F�b�N���Ȃ�
		hitpoint=(PSP[AttackClient].hp+PSP[DefenceClient].hp)/2;
	
		WazaDamage=PSP[DefenceClient].hp-hitpoint;
		UserWork[ITAMIWAKE_WORK+0]=(WazaDamage&0x000000ff)>> 0;
		UserWork[ITAMIWAKE_WORK+1]=(WazaDamage&0x0000ff00)>> 8;
		UserWork[ITAMIWAKE_WORK+2]=(WazaDamage&0x00ff0000)>>16;
		UserWork[ITAMIWAKE_WORK+3]=(WazaDamage&0xff000000)>>24;
		WazaDamage=PSP[AttackClient].hp-hitpoint;
		OSTF[DefenceClient].kaigaradamage=0xffff;	//��������̂����ŉ񕜂��Ȃ��悤�ɂ���
		WazaSeqAdrs+=5;
	}
	else{
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
}

void	WS_TEXTURE2(void)
{
	int	i,j;

	if((WazaNoHit[AttackClient]==0)||(WazaNoHit[AttackClient]==0xffff)){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
		return;
	}

	if((TameWazaCheck(WazaNoHit[AttackClient]))&&
	   (OTF[AttackClient].butsurihitdamage==0)&&
	   (OTF[AttackClient].tokusyuhitdamage==0)){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
		return;
	}

	for(i=0;i<1000;i++){
		do{
			j=pp_rand()&0x7f;
		}while(j>TYPE_CHECK_TABLE_MAX);
		j*=3;
		if((TypeCheckTable[j]==WazaTypeHit[AttackClient])&&
		   (TypeCheckTable[j+2]<=5)&&
		   (PSP[AttackClient].type1!=TypeCheckTable[j+1])&&
		   (PSP[AttackClient].type2!=TypeCheckTable[j+1])){
			PSP[AttackClient].type1=TypeCheckTable[j+1];
			PSP[AttackClient].type2=TypeCheckTable[j+1];
//			PM_strcpy(EvStrBuf0,ZokuseiMsg[TypeCheckTable[j+1]]);
			EvStrBuf0[0]=I_MSG;
			EvStrBuf0[1]=EV_ZOKUSEI;
			EvStrBuf0[2]=TypeCheckTable[j+1];
			EvStrBuf0[3]=EOM_;
			WazaSeqAdrs+=5;
			return;
		}
	}

	for(i=0;i<TYPE_CHECK_TABLE_MAX*3;i+=3){
		switch(TypeCheckTable[i]){
			case 0xfe:
			case 0xff:
				break;
			default:
				if((TypeCheckTable[i]==WazaTypeHit[AttackClient])&&
				   (TypeCheckTable[i+2]<=5)&&
				   (PSP[AttackClient].type1!=TypeCheckTable[j+1])&&
				   (PSP[AttackClient].type2!=TypeCheckTable[j+1])){
					PSP[AttackClient].type1=TypeCheckTable[i+1];
					PSP[AttackClient].type2=TypeCheckTable[i+1];
					EvStrBuf0[0]=I_MSG;
					EvStrBuf0[1]=EV_ZOKUSEI;
					EvStrBuf0[2]=TypeCheckTable[i+1];
					EvStrBuf0[3]=EOM_;
					WazaSeqAdrs+=5;
					return;
				}
				break;
		}
	}
	WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
					   (WazaSeqAdrs[2]<< 8)|
					   (WazaSeqAdrs[3]<<16)|
					   (WazaSeqAdrs[4]<<24));
}

void	WS_LOCKON(void)
{
	WazaKouka[DefenceClient]&=WAZAKOUKA_LOCKON_OFF;
	WazaKouka[DefenceClient]|=(LOCKON_COUNT*2);
	WKC[DefenceClient].lockonclientno=AttackClient;
	WazaSeqAdrs++;
}

void	WS_SKETCH(void)
{
	PokemonWazaParam	PWP;
	int	i;

	WazaNoTmp=0xffff;		//�I�E���������A�A���R�[�����ł��Ȃ��悤�ɂ���

	if((PSP[AttackClient].condition2&CONDITION2_HENSHIN)||
	   (WazaNoSketch[DefenceClient]==WAZANO_WARUAGAKI)||
	   (WazaNoSketch[DefenceClient]==0)||
	   (WazaNoSketch[DefenceClient]==0xffff)||
	   (WazaNoSketch[DefenceClient]==WAZANO_SUKETTI)){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
	else{
		for(i=0;i<4;i++){
			if((PSP[AttackClient].waza[i]!=WAZANO_SUKETTI)&&
			   (PSP[AttackClient].waza[i]==WazaNoSketch[DefenceClient]))
				break;
		}
		if(i==4){
			PSP[AttackClient].waza[WazaPosNo]=WazaNoSketch[DefenceClient];
			PSP[AttackClient].pp[WazaPosNo]=WTD[WazaNoSketch[DefenceClient]].pp;
			ClientNo=AttackClient;
			for(i=0;i<4;i++){
				PWP.waza[i]=PSP[AttackClient].waza[i];
				PWP.pp[i]=PSP[AttackClient].pp[i];
			}
			PWP.pp_count=PSP[AttackClient].pp_count;
			BSS_POKEPARA_PUT_SET(BSS_SERVER,
								BSS_PARA_WAZA,
								BSS_SELMONSNO,
								sizeof(PWP),
								(u8 *)&PWP);
			ClientBitSet(ClientNo);
//			PM_strcpy(EvStrBuf0,WazaNameData[WazaNoUse[DefenceClient]]);
			EvStrBuf0[0]=I_MSG;
			EvStrBuf0[1]=EV_WAZANO;
			EvStrBuf0[2]=(WazaNoSketch[DefenceClient]&0x00ff)>>0;
			EvStrBuf0[3]=(WazaNoSketch[DefenceClient]&0xff00)>>8;
			EvStrBuf0[4]=EOM_;
			WazaSeqAdrs+=5;
		}
		else
			WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
							   (WazaSeqAdrs[2]<< 8)|
							   (WazaSeqAdrs[3]<<16)|
							   (WazaSeqAdrs[4]<<24));
	}
}

u8	TameWazaCheck(u16 wazano)
{
	if((WTD[wazano].battleeffect==145)||			//���P�b�g���˂�
	   (WTD[wazano].battleeffect==39)||				//���ߌn
	   (WTD[wazano].battleeffect==75)||				//�S�b�h�o�[�h
	   (WTD[wazano].battleeffect==151)||			//�\�[���[�r�[��
	   (WTD[wazano].battleeffect==155)||			//������
	   (WTD[wazano].battleeffect==26))				//���܂�
		return 1;
	return 0;
}

u8	RendouWazaCheck(u16 wazano)
{
	if((wazano==0)||
	   (wazano==WAZANO_NEGOTO)||
	   (wazano==WAZANO_NEKONOTE)||
	   (wazano==WAZANO_OUMUGAESI)||
	   (wazano==WAZANO_YUBIWOHURU))
		return 1;
	return 0;
}

u8	TameWazaTurnCheck(u8 clientno,u16 wazano)
{
	if((WTD[wazano].battleeffect==151)&&			//�\�[���[�r�[��
	   (FieldCondition&FIELD_CONDITION_HIDERING))
		return 2;

	if((WTD[wazano].battleeffect==145)||			//���P�b�g���˂�
	   (WTD[wazano].battleeffect==39)||				//���ߌn
	   (WTD[wazano].battleeffect==75)||				//�S�b�h�o�[�h
	   (WTD[wazano].battleeffect==151)||			//�\�[���[�r�[��
	   (WTD[wazano].battleeffect==155)||			//������
	   (WTD[wazano].battleeffect==26)){				//���܂�
		if(ServerStatusFlag&SERVER_STATUS_FLAG_TAME)
			return 1;
		else
			return 2;
	}
	return 2;
}

void	WS_NEGOTO(void)
{
	int	i;
	u8	wazabit;

	wazabit=0;

	for(i=0;i<4;i++){
		if((RendouWazaCheck(PSP[AttackClient].waza[i]))||
		   (PSP[AttackClient].waza[i]==WAZANO_KIAIPANTI)||
		   (PSP[AttackClient].waza[i]==WAZANO_SAWAGU)||
		   (TameWazaCheck(PSP[AttackClient].waza[i])))
			wazabit|=no2bittbl[i];
	}

	wazabit=WaruagakiWazaNoCheck(AttackClient,wazabit,(WWNC_PP_NONE^0xff));

	if(wazabit==0x0f){
		WazaSeqAdrs+=5;
		return;
	}

	while(1){
		i=pp_rand()&0x3;
		if((wazabit&no2bittbl[i])==0)
		   break;
	}
//	WazaNo=WazaNoTmp=PSP[AttackClient].waza[i];
	PushWazaNo=PSP[AttackClient].waza[i];		//����݂ł˂��Ƃ���o���Z��PP�����炳�Ȃ��悤�ɂ���
	WazaPosNo=i;
	ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
	DefenceClient=ServerDefenceClientGet(PushWazaNo,0);
//	WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
	WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
					   (WazaSeqAdrs[2]<< 8)|
					   (WazaSeqAdrs[3]<<16)|
					   (WazaSeqAdrs[4]<<24));
}

void	WS_MICHIDURE(void)
{
	PSP[AttackClient].condition2|=CONDITION2_MICHIDURE;
	WazaSeqAdrs++;
}

void	MichidureCheckAct(void)
{
	u8	dir1,dir2;

	dir1=MineEnemyCheck(AttackClient);
	dir2=MineEnemyCheck(DefenceClient);

	if((PSP[DefenceClient].condition2&CONDITION2_MICHIDURE)&&
	   (dir1!=dir2)&&
	  ((ServerStatusFlag&SERVER_STATUS_FLAG_NO_MICHIDURE)==0))
		ServerStatusFlag|=SERVER_STATUS_FLAG_MICHIDURE;
}

void	WS_MICHIDURE_CHECK(void)
{
	//�݂��Â�`�F�b�N
	MichidureCheckAct();
	WazaSeqAdrs++;
}

const u8 JitabataTbl[]=
{
	1,200,
	4,150,
	9,100,
	16,80,
	32,40,
	48,20,
};

void	WS_JITABATA(void)
{
#if 0
	int	pow,pow1,pow2;

//	pow=(18000/48)*(PSP[AttackClient].hp*100)/(PSP[AttackClient].hpmax*100/48);
	pow1=PSP[AttackClient].hpmax*100/48;
	pow2=37500*PSP[AttackClient].hp;
	pow=pow2/pow1;
	pow=pow/100;
	DamagePowWork=200-pow;
	WazaSeqAdrs++;
#endif
	int	pow;
	int	i;
	pow=GetNumDotto(PSP[AttackClient].hp,PSP[AttackClient].hpmax,48);
	for(i=0;i<12;i+=2){
		if(pow<=JitabataTbl[i])
			break;
	}
	DamagePowWork=JitabataTbl[i+1];
	WazaSeqAdrs++;
}

void	WS_URAMI(void)
{
	int	i,j;

	if((WazaNoUse[DefenceClient]!=0)&&
	   (WazaNoUse[DefenceClient]!=0xffff)&&
	  ((WazaKouka[DefenceClient]&WAZAKOUKA_KIE)==0)){
		for(i=0;i<4;i++){
			if(WazaNoUse[DefenceClient]==PSP[DefenceClient].waza[i])
				break;
		}
		if(i==4)
			WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
							   (WazaSeqAdrs[2]<< 8)|
							   (WazaSeqAdrs[3]<<16)|
							   (WazaSeqAdrs[4]<<24));
		else if	(PSP[DefenceClient].pp[i]<2)
			WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
							   (WazaSeqAdrs[2]<< 8)|
							   (WazaSeqAdrs[3]<<16)|
							   (WazaSeqAdrs[4]<<24));
		else{
			j=(pp_rand()&0x03)+2;
			if(PSP[DefenceClient].pp[i]<j)
				j=PSP[DefenceClient].pp[i];
			EvStrBuf0[0]=I_MSG;
			EvStrBuf0[1]=EV_WAZANO;
			EvStrBuf0[2]=(WazaNoUse[DefenceClient]&0x00ff)>>0;
			EvStrBuf0[3]=(WazaNoUse[DefenceClient]&0xff00)>>8;
			EvStrBuf0[4]=EOM_;
			PM_NumMsgSet(EvStrBuf1,j,NUM_MODE_LEFT,1);
			EvStrBuf1[0]=I_MSG;
			EvStrBuf1[1]=EV_NUM;
			EvStrBuf1[2]=1;
			EvStrBuf1[3]=1;
			EvStrBuf1[4]=j;
			EvStrBuf1[5]=EOM_;
			PSP[DefenceClient].pp[i]-=j;
			ClientNo=DefenceClient;
			if(((WKC[ClientNo].monomanebit&no2bittbl[i])==0)&&
			   ((PSP[ClientNo].condition2&CONDITION2_HENSHIN)==0)){
				BSS_POKEPARA_PUT_SET(BSS_SERVER,BSS_PARA_PP1+i,BSS_SELMONSNO,1,
					(u8 *)&PSP[ClientNo].pp[i]);
				ClientBitSet(ClientNo);
			}
			WazaSeqAdrs+=5;
//PP���O�ɂȂ�����A���ߋZ�n�́A���߂��Ȃ����B
			if(PSP[DefenceClient].pp[i]==0)
//				PSP[DefenceClient].condition2&=CONDITION2_KEEP_OFF;
				ServerKeepOff(DefenceClient);
		}
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));

}

void	WS_IYASHINOSUZU(void)
{
	PokemonParam	*PP;
	int	i;
	u16	monsno;
	u8	speabi=0;
	u8	speabino;
	u32	condition=0;
	u8	flag=0;

	if(WazaNo==WAZANO_IYASINOSUZU){
		ServerWork[5]=0;
		if(MineEnemyCheck(AttackClient)==SIDE_MINE)
			PP=&PokeParaMine[0];
		else
			PP=&PokeParaEnemy[0];
		if(PSP[AttackClient].speabino!=TOKUSYU_BOUON)
			PSP[AttackClient].condition=0;
		else{
			TokusyuNoSet(AttackClient,PSP[AttackClient].speabino);
			ServerWork[5]|=1;
		}
		ClientNo=UserWork[CLIENT_NO_WORK]=ClientNoGet(ClientTypeGet(AttackClient)^2);
		if((FightType&FIGHT_TYPE_2vs2)&&
		  ((NoReshuffleClient&no2bittbl[ClientNo])==0)){
			if(PSP[ClientNo].speabino!=TOKUSYU_BOUON)
				PSP[ClientNo].condition=0;
			else{
				TokusyuNoSet(ClientNo,PSP[ClientNo].speabino);
				ServerWork[5]|=2;
			}
		}
		for(i=0;i<6;i++){
			monsno=PokeParaGet(&PP[i],ID_monsno_egg);
			speabi=PokeParaGet(&PP[i],ID_speabi);
			if((monsno!=0)&&
			   (monsno!=MONSNO_TAMAGO)){
				if(SelMonsNo[AttackClient]==i)
					speabino=PSP[AttackClient].speabino;
				else if((FightType&FIGHT_TYPE_2vs2)&&
						(SelMonsNo[ClientNo]==i)&&
						((NoReshuffleClient&no2bittbl[ClientNo])==0))
					speabino=PSP[ClientNo].speabino;
				else
					speabino=SpecialAbirityGet(monsno,speabi);
				if(speabino!=TOKUSYU_BOUON)
					flag|=(1<<i);
			}
		}
	}
	else{
		ServerWork[5]=4;
		flag=0x3f;			 //�S�����Ȃ���
		PSP[AttackClient].condition=0;
		ClientNo=ClientNoGet(ClientTypeGet(AttackClient)^2);
		if((FightType&FIGHT_TYPE_2vs2)&&
		  ((NoReshuffleClient&no2bittbl[ClientNo])==0))
			PSP[ClientNo].condition=0;
	}
		
	ClientNo=AttackClient;
	BSS_POKEPARA_PUT_SET(BSS_SERVER,
						 BSS_PARA_CONDITION,
						 flag,
						 4,
						 (u8 *)&condition);
	ClientBitSet(ClientNo);
	WazaSeqAdrs++;
}

void	WS_NOROI(void)
{
	if((PSP[DefenceClient].condition2&CONDITION2_NOROI)||
//	   (PSP[AttackClient].hp<=(PSP[AttackClient].hpmax/2))||
	   (WazaKouka[DefenceClient]&WAZAKOUKA_KIE))
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		PSP[DefenceClient].condition2|=CONDITION2_NOROI;
		WazaDamage=PSP[AttackClient].hpmax/2;
		if(WazaDamage==0)
			WazaDamage=1;
		WazaSeqAdrs+=5;
	}
}

void	WS_MAKIBISHI(void)
{
	u8	dir;

	dir=MineEnemyCheck(AttackClient)^1;

	if(SCC[dir].makibishicnt==3){
		OSTF[AttackClient].nopressureflag=1;
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
	else{
		SideCondition[dir]|=SIDE_CONDITION_MAKIBISHI;
		SCC[dir].makibishicnt++;
		WazaSeqAdrs+=5;
	}
}

void	WS_MIYABURU(void)
{
	PSP[DefenceClient].condition2|=CONDITION2_MIYABURU;

	WazaSeqAdrs++;
}

void	WS_HOROBINOUTA(void)
{
	int	i,j=0;
	
	for(i=0;i<ClientSetMax;i++){
		if((WazaKouka[i]&WAZAKOUKA_HOROBINOUTA)||
		   (PSP[i].speabino==TOKUSYU_BOUON))
			j++;
		else{
			WazaKouka[i]|=WAZAKOUKA_HOROBINOUTA;
			WKC[i].horobinoutacnt=3;
			WKC[i].horobinoutacnttmp=3;
//			if(i!=AttackClient)
//				PressurePPDecCheck(i,AttackClient,WAZANO_HOROBINOUTA);
		}
	}
	PressurePPDecCheckHorobinouta(AttackClient);
	if(j==ClientSetMax)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else
		WazaSeqAdrs+=5;
}

void	WS_KOROGARU(void)
{
	int	i;

	if(WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL){
//		WKC[AttackClient].korogarucnt=0;
//		PSP[AttackClient].condition2&=CONDITION2_KEEP_OFF;
		ServerKeepOff(AttackClient);
		WazaSeqAdrs=(u8 *)&WAZA_NormalNohitWait;
		return;
	}

//	if(WKC[AttackClient].korogarucnt==0){
	if((PSP[AttackClient].condition2&CONDITION2_KEEP)==0){
		WKC[AttackClient].korogarucnt=5;
		WKC[AttackClient].korogarucnttmp=5;
		PSP[AttackClient].condition2|=CONDITION2_KEEP;
		WazaNoKeep[AttackClient]=WazaNo;
	}

	if(--WKC[AttackClient].korogarucnt==0)
		PSP[AttackClient].condition2&=CONDITION2_KEEP_OFF;

	DamagePowWork=WTD[WazaNo].damage;
	for(i=1;i<(5-WKC[AttackClient].korogarucnt);i++)
		DamagePowWork*=2;
	if(PSP[AttackClient].condition2&CONDITION2_MARUKUNARU)
		DamagePowWork*=2;

	WazaSeqAdrs++;
}

void	WS_IBARU(void)
{
	if((PSP[DefenceClient].condition2&CONDITION2_KONRAN)&&
	   (PSP[DefenceClient].abiritycnt[WazaSeqAdrs[1]]==12))
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[2]<< 0)|
						   (WazaSeqAdrs[3]<< 8)|
						   (WazaSeqAdrs[4]<<16)|
						   (WazaSeqAdrs[5]<<24));
	else
		WazaSeqAdrs+=6;

}

void	WS_RENZOKUGIRI(void)
{
	int	i;

	if(WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL){
		WKC[AttackClient].renzokugiricnt=0;			//��񂼂�����J�E���^
		WazaSeqAdrs=(u8 *)&WAZA_NormalNohitWait;
		return;
	}

	if(WKC[AttackClient].renzokugiricnt!=5)			//��񂼂�����J�E���^
		WKC[AttackClient].renzokugiricnt++;

	DamagePowWork=WTD[WazaNo].damage;
	for(i=1;i<WKC[AttackClient].renzokugiricnt;i++)
		DamagePowWork*=2;


	WazaSeqAdrs++;
}

void	WS_NATSUKI(void)
{
	if(WTD[WazaNo].battleeffect==121)				//���񂪂���
		DamagePowWork=PSP[AttackClient].friend*10/25;
	else											//�������
		DamagePowWork=(255-PSP[AttackClient].friend)*10/25;

	WazaSeqAdrs++;
}

void	WS_PRESENT(void)
{
	int	present;

	present=pp_rand()&0xff;

	if(present<(255*40/100))
		DamagePowWork=40;
	else if(present<(255*70/100))
		DamagePowWork=80;
	else if(present<(255*80/100))
		DamagePowWork=120;
	else{
		WazaDamage=PSP[DefenceClient].hpmax/4;
		if(WazaDamage==0)
			WazaDamage=1;
		WazaDamage*=-1;
	}

	if(present<(255*80/100))
		WazaSeqAdrs=(u8 *)&WAZA_HitCalcOnly;
	else{
		if(PSP[DefenceClient].hpmax==PSP[DefenceClient].hp)
			WazaSeqAdrs=(u8 *)&WAZA_Mantan;
		else
			WazaSeqAdrs=(u8 *)&WAZA_Kaifuku;
	}
}

void	WS_SHINPINOMAMORI(void)
{
	if(SideCondition[ClientTypeGet(AttackClient)&1]&SIDE_CONDITION_SHINPI){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=0;
	}
	else{
		SideCondition[ClientTypeGet(AttackClient)&1]|=SIDE_CONDITION_SHINPI;
		SCC[ClientTypeGet(AttackClient)&1].shinpicnt=5;
		ServerWork[5]=5;
	}
	WazaSeqAdrs++;
}

void	WS_MAGNITUDE(void)
{
	int	magnitude;

	magnitude=pp_rand()%100;

	if(magnitude<5){
		DamagePowWork=10;
		magnitude=4;
	}
	else if(magnitude<15){
		DamagePowWork=30;
		magnitude=5;
	}
	else if(magnitude<35){
		DamagePowWork=50;
		magnitude=6;
	}
	else if(magnitude<65){
		DamagePowWork=70;
		magnitude=7;
	}
	else if(magnitude<85){
		DamagePowWork=90;
		magnitude=8;
	}
	else if(magnitude<95){
		DamagePowWork=110;
		magnitude=9;
	}
	else{
		DamagePowWork=150;
		magnitude=10;
	}

//	PM_NumMsgSet(EvStrBuf0,magnitude,NUM_MODE_LEFT,2);
	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_NUM;
	EvStrBuf0[2]=1;
	EvStrBuf0[3]=2;
	EvStrBuf0[4]=magnitude;
	EvStrBuf0[5]=EOM_;
	for(DefenceClient=0;DefenceClient<ClientSetMax;DefenceClient++){
		if((DefenceClient!=AttackClient)&&
		  ((NoReshuffleClient&no2bittbl[DefenceClient])==0))
						break;
	}
	WazaSeqAdrs++;
}

void	WS_OIUCHI_CHECK(void)
{
	int	i;

	if(WazaSeqLoopCount==1){
		if(MineEnemyCheck(AttackClient)==SIDE_MINE)
			DefenceClient=ClientNoGet(BSS_CLIENT_ENEMY);
		else
			DefenceClient=ClientNoGet(BSS_CLIENT_MINE);
	}
	else{
		if(MineEnemyCheck(AttackClient)==SIDE_MINE)
			DefenceClient=ClientNoGet(BSS_CLIENT_ENEMY2);
		else
			DefenceClient=ClientNoGet(BSS_CLIENT_MINE2);
	}

	if((CommandReturn[DefenceClient]==BSS_SELECT_RETURN_FIGHT)&&
	   (AttackClient==UserWork[DEFENCE_CLIENT+DefenceClient])&&
	  ((PSP[DefenceClient].condition&(CONDITION_NEMURI|CONDITION_KOORI))==0)&&
	   (PSP[AttackClient].hp)&&
	   (WKC[DefenceClient].namakebit==0)&&
	   (SelectWazaNo[DefenceClient]==WAZANO_OIUTI)){
		for(i=0;i<ClientSetMax;i++){
			if(ActionClientNo[i]==DefenceClient)
				ActionNo[i]=SCA_EndCheckNo;
		}
	   	WazaNo=WAZANO_OIUTI;
		WazaSeqAdrs+=5;
		UserWork[WAZA_EFF_CNT]=1;
		ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_NIHONBARE(void)
{
	if(FieldCondition&FIELD_CONDITION_HIDERING){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=2;
	}
	else{
		FieldCondition=FIELD_CONDITION_HARE;
		ServerWork[5]=4;
		FCC.FCCcnt=NIHONBARE_COUNT;
	}
	WazaSeqAdrs++;
}

void	WS_HARADAIKO(void)
{
	u16	hpmax;

	hpmax=PSP[AttackClient].hpmax/2;
	if(hpmax==0)
		hpmax=1;

	if((PSP[AttackClient].abiritycnt[COND_POW]<12)&&
	   (PSP[AttackClient].hp>hpmax)){
		PSP[AttackClient].abiritycnt[COND_POW]=12;
		WazaDamage=PSP[AttackClient].hpmax/2;
		if(WazaDamage==0)
			WazaDamage=1;
		WazaSeqAdrs+=5;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_JIKOANJI(void)
{
#if 0
	int	i,j=0;

	for(i=COND_HP;i<COND_MAX;i++){
		if(PSP[DefenceClient].abiritycnt[i]>PSP[AttackClient].abiritycnt[i]){
			PSP[AttackClient].abiritycnt[i]=PSP[DefenceClient].abiritycnt[i];
			j=1;
		}
	}

	if(j)
		WazaSeqAdrs+=5;
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
#else
	int	i;

	for(i=COND_HP;i<COND_MAX;i++)
		PSP[AttackClient].abiritycnt[i]=PSP[DefenceClient].abiritycnt[i];
	WazaSeqAdrs+=5;

#if 0
	for(i=COND_HP;i<COND_MAX;i++){
		if(PSP[DefenceClient].abiritycnt[i]>6)
			break;
	}
	if(i==COND_MAX)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		for(i=COND_HP;i<COND_MAX;i++)
			PSP[AttackClient].abiritycnt[i]=PSP[DefenceClient].abiritycnt[i];
		WazaSeqAdrs+=5;
	}
#endif
	
#endif
}

void	WS_KOUSOKUSPIN(void)
{
	//���ߌn�̋Z
	if(PSP[AttackClient].condition2&CONDITION2_SHIME){
		PSP[AttackClient].condition2&=CONDITION2_SHIME_OFF;
		DefenceClient=UserWork[SHIME_WAZACLIENT+AttackClient];
		EvStrBuf0[0]=I_MSG;
		EvStrBuf0[1]=EV_WAZANO;
		EvStrBuf0[2]=UserWork[SHIME_WAZANO+AttackClient*2];
		EvStrBuf0[3]=UserWork[SHIME_WAZANO+AttackClient*2+1];
		EvStrBuf0[4]=EOM_;
		PushAdrsAct();
		WazaSeqAdrs=(u8 *)&WAZA_ShimeEscape;
		return;
	}
	//��ǂ肬�̂���
	if(WazaKouka[AttackClient]&WAZAKOUKA_YADORARE){
		WazaKouka[AttackClient]&=WAZAKOUKA_YADORARE_OFF;
		WazaKouka[AttackClient]&=WAZAKOUKA_YADORIGI_OFF;
		PushAdrsAct();
		WazaSeqAdrs=(u8 *)&WAZA_YadorigiEscape;
		return;
	}
	//�܂��т�
	if(SideCondition[MineEnemyCheck(AttackClient)]&SIDE_CONDITION_MAKIBISHI){
		SideCondition[MineEnemyCheck(AttackClient)]&=
			SIDE_CONDITION_MAKIBISHI_OFF;
		SCC[MineEnemyCheck(AttackClient)].makibishicnt=0;
		PushAdrsAct();
		WazaSeqAdrs=(u8 *)&WAZA_MakibishiEscape;
		return;
	}
	WazaSeqAdrs++;
}

void	WS_MARUKUNARU(void)
{
	PSP[AttackClient].condition2|=CONDITION2_MARUKUNARU;
	WazaSeqAdrs++;
}

void	WS_WEATHERKAIFUKU(void)
{
	DefenceClient=AttackClient;
	if(PSP[AttackClient].hp!=PSP[AttackClient].hpmax){
		if((FieldCondition==0)||
		   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_NOOTENKI,0,0))||
		   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_EAROKKU,0,0)))
			WazaDamage=PSP[AttackClient].hpmax/2;
		else if(FieldCondition&FIELD_CONDITION_HIDERING)
			WazaDamage=PSP[AttackClient].hpmax*20/30;
		else
			WazaDamage=PSP[AttackClient].hpmax/4;
		if(WazaDamage==0)
			WazaDamage=1;
		WazaDamage*=-1;
		WazaSeqAdrs+=5;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_MEZAMERUPOWER(void)
{
	u8	pow;
	u8	type;

	pow=((PSP[AttackClient].hp_rnd&2)		>>1)|
	    ((PSP[AttackClient].pow_rnd&2)		>>0)|
	    ((PSP[AttackClient].def_rnd&2)		<<1)|
	    ((PSP[AttackClient].agi_rnd&2)		<<2)|
	    ((PSP[AttackClient].spepow_rnd&2)	<<3)|
	    ((PSP[AttackClient].spedef_rnd&2)	<<4);
	type=((PSP[AttackClient].hp_rnd&1)		>>0)|
	     ((PSP[AttackClient].pow_rnd&1)		<<1)|
	     ((PSP[AttackClient].def_rnd&1)		<<2)|
	     ((PSP[AttackClient].agi_rnd&1)		<<3)|
	     ((PSP[AttackClient].spepow_rnd&1)	<<4)|
	     ((PSP[AttackClient].spedef_rnd&1)	<<5);

	DamagePowWork=pow*40/63+30;
	UserWork[DAMAGE_TYPE_WORK]=(type*15/63)+1;

	if(UserWork[DAMAGE_TYPE_WORK]>=HATE_TYPE)
		UserWork[DAMAGE_TYPE_WORK]++;

	UserWork[DAMAGE_TYPE_WORK]|=0xc0;		//�J�E���^�[�̂ݗL���ɂ���
	WazaSeqAdrs++;
}

void	WS_JISIN(void)
{
	for(DefenceClient=0;DefenceClient<ClientSetMax;DefenceClient++){
		if((DefenceClient!=AttackClient)&&
		  ((NoReshuffleClient&no2bittbl[DefenceClient])==0))
						break;
	}
	WazaSeqAdrs++;
}

void	WS_MIRAIYOCHI(void)
{
#if 0
	SideCondition[ClientTypeGet(DefenceClient)&1]|=SIDE_CONDITION_MIRAIYOCHI;
	SCC[ClientTypeGet(DefenceClient)&1].miraiyochiwazano=WazaNo;
	SCC[ClientTypeGet(DefenceClient)&1].miraiyochiattackclient=AttackClient;
	SCC[ClientTypeGet(DefenceClient)&1].miraiyochidefenceclient=DefenceClient;
	SCC[ClientTypeGet(DefenceClient)&1].miraiyochicnt=3;
	SCC[ClientTypeGet(DefenceClient)&1].miraiyochidamage=
		WazaDamageCalc(&PSP[AttackClient],&PSP[DefenceClient],WazaNo,SideCondition[ClientTypeGet(DefenceClient)&1],0,0,AttackClient,DefenceClient);
	if(OTF[AttackClient].tedasukeflag)
		SCC[ClientTypeGet(DefenceClient)&1].miraiyochidamage=
			SCC[ClientTypeGet(DefenceClient)&1].miraiyochidamage*15/10;

	if(WazaNo==WAZANO_HAMETUNONEGAI)
		ServerWork[5]=1;
	else
		ServerWork[5]=0;
	WazaSeqAdrs++;
#endif
	if(FCC.miraiyochicnt[DefenceClient])
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		FCC.miraiyochiwazano[DefenceClient]=WazaNo;
		FCC.miraiyochiattackclient[DefenceClient]=AttackClient;
		FCC.miraiyochicnt[DefenceClient]=3;
		FCC.miraiyochidamage[DefenceClient]=
		WazaDamageCalc(&PSP[AttackClient],
					   &PSP[DefenceClient],
					   WazaNo,
					   SideCondition[ClientTypeGet(DefenceClient)&1],
					   0,0,AttackClient,DefenceClient);
		if(OTF[AttackClient].tedasukeflag)
			FCC.miraiyochidamage[DefenceClient]=
				FCC.miraiyochidamage[DefenceClient]*15/10;
		if(WazaNo==WAZANO_HAMETUNONEGAI)
			ServerWork[5]=1;
		else
			ServerWork[5]=0;
		WazaSeqAdrs+=5;
	}
}

void	WS_HUKURODATAKI(void)
{
	PokemonParam	*PP;
	int	damage2;
	int	flag;
	
	if(MineEnemyCheck(AttackClient)==SIDE_MINE)
		PP=&PokeParaMine[0];
	else
		PP=&PokeParaEnemy[0];

	if(PSP[DefenceClient].hp==0){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
		return;
	}

	flag=ServerWork[0];

	while(ServerWork[0]<6){
		if((PokeParaGet(&PP[ServerWork[0]],ID_hp)!=0)&&
		   (PokeParaGet(&PP[ServerWork[0]],ID_monsno_egg)!=0)&&
		   (PokeParaGet(&PP[ServerWork[0]],ID_monsno_egg)!=MONSNO_TAMAGO)&&
		   (PokeParaGet(&PP[ServerWork[0]],ID_condition)==0)){
			break;
		}
		ServerWork[0]++;
	}
	if(ServerWork[0]<6){
//		PokeParaGet(&PP[ServerWork[0]],ID_nickname,EvStrBuf0);
		EvStrBuf0[0]=I_MSG;
		EvStrBuf0[1]=EV_NICKNAME;
		EvStrBuf0[2]=AttackClient;
		EvStrBuf0[3]=ServerWork[0];
		EvStrBuf0[4]=EOM_;

		WazaSeqAdrs+=9;
		WazaDamage=PPD[PokeParaGet(&PP[ServerWork[0]],ID_monsno)].basic_pow;
		WazaDamage*=WTD[WazaNo].damage;
		WazaDamage*=(PokeParaGet(&PP[ServerWork[0]],ID_level)*2/5+2);

		damage2=PPD[PSP[DefenceClient].monsno].basic_def;
		WazaDamage/=damage2;
		WazaDamage/=50;
		WazaDamage+=2;
		if(OTF[AttackClient].tedasukeflag)
			WazaDamage=WazaDamage*15/10;

		ServerWork[0]++;
	}
	else{
		if(flag)
			WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
							   (WazaSeqAdrs[2]<< 8)|
							   (WazaSeqAdrs[3]<<16)|
							   (WazaSeqAdrs[4]<<24));
		else
			WazaSeqAdrs=(u8 *)((WazaSeqAdrs[5]<< 0)|
							   (WazaSeqAdrs[6]<< 8)|
							   (WazaSeqAdrs[7]<<16)|
							   (WazaSeqAdrs[8]<<24));
	}
}

void	WS_KIERUTAME(void)
{
	switch(WazaNo){
		case WAZANO_SORAWOTOBU:
		case WAZANO_TOBIHANERU:
			WazaKouka[AttackClient]|=WAZAKOUKA_SORAWOTOBU;
			break;
		case WAZANO_ANAWOHORU:
			WazaKouka[AttackClient]|=WAZAKOUKA_ANAWOHORU;
			break;
		case WAZANO_DAIBINGU:
			WazaKouka[AttackClient]|=WAZAKOUKA_DAIBINGU;
			break;
	}

	WazaSeqAdrs++;
}

void	WS_KIERUTAME_AFTAR(void)
{
	switch(WazaNo){
		case WAZANO_SORAWOTOBU:
		case WAZANO_TOBIHANERU:
			WazaKouka[AttackClient]&=WAZAKOUKA_SORAWOTOBU_OFF;
			break;
		case WAZANO_ANAWOHORU:
			WazaKouka[AttackClient]&=WAZAKOUKA_ANAWOHORU_OFF;
			break;
		case WAZANO_DAIBINGU:
			WazaKouka[AttackClient]&=WAZAKOUKA_DAIBINGU_OFF;
			break;
	}

	WazaSeqAdrs++;
}

void	WS_CHIISAKUNARU(void)
{
	if(ServerStatusFlag&SERVER_STATUS_FLAG_WAZA_OUT)
		WazaKouka[AttackClient]|=WAZAKOUKA_CHIISAKUNARU;
	WazaSeqAdrs++;
}

void	WS_ARARE(void)
{
	if(FieldCondition&FIELD_CONDITION_ARARE){
		WazaStatusFlag|=WAZASTATUSFLAG_NOHIT;
		ServerWork[5]=2;
	}
	else{
		FieldCondition=FIELD_CONDITION_ARARE;
		ServerWork[5]=5;
		FCC.FCCcnt=ARARE_COUNT;
	}
	WazaSeqAdrs++;
}

void	WS_OKIMIYAGE(void)
{
	if((PSP[DefenceClient].abiritycnt[COND_POW]==0)&&
	   (PSP[DefenceClient].abiritycnt[COND_SPEPOW]==0)&&
	   (ServerWork[6]!=MSG_MAMORUNOHIT))
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		ClientNo=AttackClient;
		WazaDamage=PSP[ClientNo].hp;
		BSS_HP_GAUGE_SET(BSS_SERVER,JIBAKU_HP);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=5;
	}
}

void	WS_KONOYUBITOMARE(void)
{
	SCC[MineEnemyCheck(AttackClient)].konoyubitomareflag=1;
	SCC[MineEnemyCheck(AttackClient)].konoyubitomareno=AttackClient;
	WazaSeqAdrs++;
}

void	WS_JUUDEN(void)
{
	WazaKouka[AttackClient]|=WAZAKOUKA_JUUDEN;
	WKC[AttackClient].juudencnt=2;
	WKC[AttackClient].juudencnttmp=2;
	WazaSeqAdrs++;
}

const u16 ShizennochikaraTbl[]={
	WAZANO_SIBIREGONA,
	WAZANO_HAPPAKATTAA,
	WAZANO_ZISIN,
	WAZANO_HAIDOROPONPU,
	WAZANO_NAMINORI,
	WAZANO_BABURUKOUSEN,
	WAZANO_IWANADARE,
	WAZANO_SYADOOBOORU,
	WAZANO_SUPIIDOSUTAA,
	WAZANO_SUPIIDOSUTAA,
};

void	WS_SHIZENNOCHIKARA(void)
{
//	WazaNoTmp=WazaNo=ShizennochikaraTbl[GroundNo];
	ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
	WazaNo=ShizennochikaraTbl[GroundNo];
	DefenceClient=ServerDefenceClientGet(WazaNo,0);
	PushAdrsSet((u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect]);
	WazaSeqAdrs++;
}

void	WS_REFLESH(void)
{
	if(PSP[AttackClient].condition&CONDITION_KARAGENKI){	//�ǂ��A�܂ЁA�₯��
		PSP[AttackClient].condition=0;
		
		WazaSeqAdrs+=5;
		ClientNo=AttackClient;
		BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 BSS_PARA_CONDITION,
							 BSS_SELMONSNO,
							 4,
							 (u8 *)&PSP[ClientNo].condition);
		ClientBitSet(ClientNo);
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_ICHAMON(void)
{
	if(PSP[DefenceClient].condition2&CONDITION2_ICHAMON)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		PSP[DefenceClient].condition2|=CONDITION2_ICHAMON;
		WazaSeqAdrs+=5;
	}
}

void	WS_KIAIPUNCH(void)
{
	if((OTF[AttackClient].butsurihitdamage!=0)||
	   (OTF[AttackClient].tokusyuhitdamage!=0))
		WazaSeqAdrs+=5;
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_CHOUHATSU(void)
{
	if(WKC[DefenceClient].chouhatsucnt==0){
		WKC[DefenceClient].chouhatsucnt=2;
		WKC[DefenceClient].chouhatsucnttmp=2;
		WazaSeqAdrs+=5;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_TEDASUKE(void)
{
	DefenceClient=ClientNoGet(ClientTypeGet(AttackClient)^2);
	if((FightType&FIGHT_TYPE_2vs2)&&
	  ((NoReshuffleClient&no2bittbl[DefenceClient])==0)&&
	   (OTF[AttackClient].tedasukeflag==0)&&
	   (OTF[DefenceClient].tedasukeflag==0)){
		OTF[DefenceClient].tedasukeflag=1;
		WazaSeqAdrs+=5;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_TRICK(void)
{
	u16	itemno;
	u16 *item;
	u8	dir=0;

	if((MineEnemyCheck(AttackClient)==SIDE_ENEMY)&&
	  ((FightType&FIGHT_TYPE_NO_DOROBOU)==0)&&
	   (FieldTrainerID!=ORUSUBAN_TRAINER)){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
		return;
	}
	//�͂������Ƃ���Ă���Ƃ��́A�������Ȃ�
	dir=MineEnemyCheck(AttackClient);
	if(((FightType&FIGHT_TYPE_NO_DOROBOU)==0)&&
	    (FieldTrainerID!=ORUSUBAN_TRAINER)&&
		(FCC.hatakiotosuitem[dir]&no2bittbl[SelMonsNo[AttackClient]])){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
		return;
	}
	if(((PSP[AttackClient].item==0)&&
	    (PSP[DefenceClient].item==0))||
		(PSP[AttackClient].item==ITEM_NAZONOMI)||
		(PSP[DefenceClient].item==ITEM_NAZONOMI))
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		//����\�͂˂񂿂Ⴍ�͂ǂ����𓐂܂�Ȃ�
		if(PSP[DefenceClient].speabino==TOKUSYU_NENTYAKU){
			WazaSeqAdrs=(u8 *)&WAZA_Nentyaku;
			SpeabiNo=PSP[DefenceClient].speabino;
			TokusyuNoSet(DefenceClient,SpeabiNo);
		}
		else{
#if 0
			itemno=PSP[AttackClient].item;
			PSP[AttackClient].item=PSP[DefenceClient].item;
			PSP[DefenceClient].item=itemno;
	
			ClientNo=AttackClient;
			BSS_POKEPARA_PUT_SET(BSS_SERVER,
								BSS_PARA_ITEM,
								BSS_SELMONSNO,
								2,
								(u8 *)&PSP[AttackClient].item);
			ClientBitSet(AttackClient);
#else
			item=(u16 *)&UserWork[ITEMNO_STOCK+AttackClient*2];
			itemno=PSP[AttackClient].item;
			item[0]=PSP[DefenceClient].item;
			PSP[AttackClient].item=0;
			PSP[DefenceClient].item=itemno;
	
			ClientNo=AttackClient;
			BSS_POKEPARA_PUT_SET(BSS_SERVER,
								BSS_PARA_ITEM,
								BSS_SELMONSNO,
								2,
								(u8 *)item);
			ClientBitSet(AttackClient);
#endif
	
			ClientNo=DefenceClient;
			BSS_POKEPARA_PUT_SET(BSS_SERVER,
								BSS_PARA_ITEM,
								BSS_SELMONSNO,
								2,
								(u8 *)&PSP[DefenceClient].item);
			ClientBitSet(DefenceClient);

			//�������n�`�}�L�̃��[�N���N���A
			UserWork[KODAWARI_WAZANO+0+DefenceClient*2]=0;
			UserWork[KODAWARI_WAZANO+1+DefenceClient*2]=0;
			UserWork[KODAWARI_WAZANO+0+AttackClient*2]=0;
			UserWork[KODAWARI_WAZANO+1+AttackClient*2]=0;
	
			WazaSeqAdrs+=5;
			EvStrBuf0[0]=I_MSG;
			EvStrBuf0[1]=EV_ITEMNO;
			EvStrBuf0[2]=item[0]&0xff;
			EvStrBuf0[3]=(item[0]&0xff00)>>8;
			EvStrBuf0[4]=EOM_;
			EvStrBuf1[0]=I_MSG;
			EvStrBuf1[1]=EV_ITEMNO;
			EvStrBuf1[2]=itemno&0xff;
			EvStrBuf1[3]=(itemno&0xff00)>>8;
			EvStrBuf1[4]=EOM_;
			if((itemno)&&(item[0]))
				ServerWork[5]=2;
			else if(item[0])
				ServerWork[5]=0;
			else
				ServerWork[5]=1;
		}
	}
}

void	WS_NARIKIRI(void)
{
	if((PSP[DefenceClient].speabino)&&
		//�����ӂ����Ȃ܂���́A�Ȃ肫��ł��Ȃ�
	   (PSP[DefenceClient].speabino!=TOKUSYU_HUSIGINAMAMORI)){
		PSP[AttackClient].speabino=PSP[DefenceClient].speabino;
		SpeabiNo=PSP[DefenceClient].speabino;
		WazaSeqAdrs+=5;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_NEGAIGOTO(void)
{
#if 0
	u8	dir;

	dir=MineEnemyCheck(AttackClient);

	switch(WazaSeqAdrs[1]){
		case 0:
			if(SideCondition[dir]&SIDE_CONDITION_NEGAIGOTO)
				WazaSeqAdrs=(u8 *)((WazaSeqAdrs[2]<< 0)|
								   (WazaSeqAdrs[3]<< 8)|
								   (WazaSeqAdrs[4]<<16)|
								   (WazaSeqAdrs[5]<<24));
			else{
				SideCondition[dir]|=SIDE_CONDITION_NEGAIGOTO;
				SCC[dir].negaigotocnt=2;
				SCC[dir].negaigotoclientno=AttackClient;
				SCC[dir].negaigotoselmonsno=SelMonsNo[AttackClient];
				WazaSeqAdrs+=6;
			}
			break;
		case 1:
			DefenceClient=SCC[dir].negaigotoclientno;
			EvStrBuf0[0]=I_MSG;
			EvStrBuf0[1]=EV_NICKNAME;
			EvStrBuf0[2]=DefenceClient;
			EvStrBuf0[3]=SCC[dir].negaigotoselmonsno;
			EvStrBuf0[4]=EOM_;

			WazaDamage=PSP[DefenceClient].hpmax/2;
			if(WazaDamage==0)
				WazaDamage=1;
			WazaDamage*=-1;
			if(PSP[DefenceClient].hp==PSP[DefenceClient].hpmax)
				WazaSeqAdrs=(u8 *)((WazaSeqAdrs[2]<< 0)|
								   (WazaSeqAdrs[3]<< 8)|
								   (WazaSeqAdrs[4]<<16)|
								   (WazaSeqAdrs[5]<<24));
			else
				WazaSeqAdrs+=6;
			break;
	}
#else
	switch(WazaSeqAdrs[1]){
		case 0:
			if(FCC.negaigotocnt[AttackClient])
				WazaSeqAdrs=(u8 *)((WazaSeqAdrs[2]<< 0)|
								   (WazaSeqAdrs[3]<< 8)|
								   (WazaSeqAdrs[4]<<16)|
								   (WazaSeqAdrs[5]<<24));
			else{
				FCC.negaigotocnt[AttackClient]=2;
				FCC.negaigotoselmonsno[AttackClient]=SelMonsNo[AttackClient];
				WazaSeqAdrs+=6;
			}
			break;
		case 1:
			EvStrBuf0[0]=I_MSG;
			EvStrBuf0[1]=EV_NICKNAME;
			EvStrBuf0[2]=DefenceClient;
			EvStrBuf0[3]=FCC.negaigotoselmonsno[DefenceClient];
			EvStrBuf0[4]=EOM_;

			WazaDamage=PSP[DefenceClient].hpmax/2;
			if(WazaDamage==0)
				WazaDamage=1;
			WazaDamage*=-1;
			if(PSP[DefenceClient].hp==PSP[DefenceClient].hpmax)
				WazaSeqAdrs=(u8 *)((WazaSeqAdrs[2]<< 0)|
								   (WazaSeqAdrs[3]<< 8)|
								   (WazaSeqAdrs[4]<<16)|
								   (WazaSeqAdrs[5]<<24));
			else
				WazaSeqAdrs+=6;
			break;
	}
#endif
}

void	WS_NEWOHARU(void)
{
	if(WazaKouka[AttackClient]&WAZAKOUKA_NEWOHARU)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		WazaKouka[AttackClient]|=WAZAKOUKA_NEWOHARU;
		WazaSeqAdrs+=5;
	}
}

void	WS_REVENGE(void)
{
	if(
	  ((OTF[AttackClient].butsurihitdamage!=0)&&
	   (OTF[AttackClient].butsurihitclient==DefenceClient))||
	  ((OTF[AttackClient].tokusyuhitdamage!=0)&&
	   (OTF[AttackClient].tokusyuhitclient==DefenceClient)))
		UserWork[DAMAGE_VALUE]=2;
	WazaSeqAdrs++;
}

void	WS_AKUBI(void)
{
	if((WazaKouka[DefenceClient]&WAZAKOUKA_AKUBI)||
//	   (PSP[DefenceClient].speabino==TOKUSYU_YARUKI)||
//	   (PSP[DefenceClient].speabino==TOKUSYU_HUMIN)||
	   (PSP[DefenceClient].condition&CONDITION_BAD))
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		WazaKouka[DefenceClient]|=(AKUBI_COUNT<<1);
		WazaSeqAdrs+=5;
	}
}

void	WS_GAMUSYARA(void)
{
	if(PSP[DefenceClient].hp<=PSP[AttackClient].hp)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		WazaDamage=PSP[DefenceClient].hp-PSP[AttackClient].hp;
		WazaSeqAdrs+=5;
	}
}

void	WS_HUNKA(void)
{
	//2�̂ɓ�����Ƃ��ɁAHP���ω�����ƍČv�Z����Ă��܂��̂ŁA
	//2�x�ڂ̌v�Z�̓p�X����
	if(DamagePowWork==0){
		DamagePowWork=WTD[WazaNo].damage*PSP[AttackClient].hp/PSP[AttackClient].hpmax;
		if(DamagePowWork==0)
			DamagePowWork=1;
	}

	WazaSeqAdrs++;
}

void	WS_SKILLSWAP(void)
{
	u8	speabino;

	if(((PSP[AttackClient].speabino==0)&&
	    (PSP[DefenceClient].speabino==0))||
		//�����ӂ����Ȃ܂���́A�X�L���X���b�v�ł��Ȃ�
	   ((PSP[AttackClient].speabino==TOKUSYU_HUSIGINAMAMORI)||
	    (PSP[DefenceClient].speabino==TOKUSYU_HUSIGINAMAMORI))||
	    (WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL))
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		speabino=PSP[AttackClient].speabino;
		PSP[AttackClient].speabino=PSP[DefenceClient].speabino;
		PSP[DefenceClient].speabino=speabino;
		WazaSeqAdrs+=5;
	}
}

void	WS_HUUIN(void)
{
	int	i,j;
	u8	dir=0;
	u8	clientno;

	if(WazaKouka[AttackClient]&WAZAKOUKA_HUUIN)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
#if 0
		dir=MineEnemyCheck(AttackClient);
		for(clientno=0;clientno<ClientSetMax;clientno++){
			if(dir!=MineEnemyCheck(clientno))
				PressurePPDecCheck(clientno,AttackClient,WAZANO_HUUIN);
		}
#endif
		PressurePPDecCheckHuuin(AttackClient);
		for(clientno=0;clientno<ClientSetMax;clientno++){
			if(dir!=MineEnemyCheck(clientno)){
				for(i=0;i<4;i++){
					for(j=0;j<4;j++){
						if((PSP[AttackClient].waza[i]==PSP[clientno].waza[j])&&
						   (PSP[AttackClient].waza[i]!=0)&&
						   (PSP[clientno].waza[j]!=0))
							break;
					}
					if(j!=4)
						break;
				}
				if(i!=4){
					WazaKouka[AttackClient]|=WAZAKOUKA_HUUIN;
					WazaSeqAdrs+=5;
					break;
				}
			}
		}
		if(clientno==ClientSetMax)
			WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
							   (WazaSeqAdrs[2]<< 8)|
							   (WazaSeqAdrs[3]<<16)|
							   (WazaSeqAdrs[4]<<24));
	}
}

void	WS_ONNEN(void)
{
	if(WazaKouka[AttackClient]&WAZAKOUKA_ONNEN)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		WazaKouka[AttackClient]|=WAZAKOUKA_ONNEN;
		WazaSeqAdrs+=5;
	}
}

const u16 KetaguriTbl[]={
	100,20,
	250,40,
	500,60,
	1000,80,
	2000,100,
	0xffff,0xffff,
};

void	WS_KETAGURI(void)
{
	int	i;

	i=0;
	while(KetaguriTbl[i]!=0xffff){
//		if(KetaguriTbl[i]>ZukanPokeDataGet(PSP[DefenceClient].monsno,
		if(KetaguriTbl[i]>ZukanPokeDataGet(PokeMonsNo2ZukanNoGet(PSP[DefenceClient].monsno),
										   ZDATA_OMOSA))
			break;
		i+=2;
	}

	if(KetaguriTbl[i]!=0xffff)
		DamagePowWork=KetaguriTbl[i+1];
	else
		DamagePowWork=120;

	WazaSeqAdrs++;
}

void	WS_NEKONOTE(void)
{
	int	i,j,k,l=0;
	PokemonParam	*PP;
	u16	*wazabuf;
	u16	wazano=0;

	wazabuf=(u16 *)&UserWork[NEKONOTE_WAZANO];

	if(ClientTypeGet(AttackClient)&1)
		PP=&PokeParaEnemy[0];
	else
		PP=&PokeParaMine[0];

	for(i=0;i<6;i++){
		if(i!=SelMonsNo[AttackClient]){
			if((PokeParaGet(&PP[i],ID_monsno_egg)!=0)&&
			   (PokeParaGet(&PP[i],ID_monsno_egg)!=MONSNO_TAMAGO)){
				for(j=0;j<4;j++){
					k=0;
					wazano=PokeParaGet(&PP[i],ID_waza1+j);
					if((RendouWazaCheck(wazano)==0)){
						while(YubihuriNGTbl[k]!=0xffff){
							if(wazano==YubihuriNGTbl[k])
								break;
							k++;
						}
						if((YubihuriNGTbl[k]==0xffff)&&
						   (wazano!=0)){
							wazabuf[l]=wazano;
							l++;
						}
					}
				}
			}
		}
	}
	if(l){
		ServerStatusFlag&=SERVER_STATUS_FLAG_NOATTACK_MES_OFF;
		PushWazaNo=wazabuf[((pp_rand()&0xff)*l)>>8];
		DefenceClient=ServerDefenceClientGet(PushWazaNo,0);
//		WazaSeqAdrs=(u8 *)WazaEffectTbl[WTD[WazaNo].battleeffect];
		WazaSeqAdrs+=5;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_MAGICCORT(void)
{
	DefenceClient=AttackClient;
	OSTF[AttackClient].nopressureflag=1;
	if(ActionCount==ClientSetMax-1){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
	else{
		OTF[AttackClient].magiccortflag=1;
		WazaSeqAdrs+=5;
	}
}

void	WS_YOKODORI(void)
{
	OSTF[AttackClient].nopressureflag=1;
	if(ActionCount==ClientSetMax-1){
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	}
	else{
		OTF[AttackClient].yokodoriflag=1;
		WazaSeqAdrs+=5;
	}
}

void	WS_IKAKU(void)
{
	u8	dir;

	UserWork[CLIENT_NO_WORK]=UserWork[IKAKU_CLIENTNO];
	dir=MineEnemyCheck(UserWork[CLIENT_NO_WORK]);

	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_SPEABINO;
	EvStrBuf0[2]=PSP[UserWork[CLIENT_NO_WORK]].speabino;
	EvStrBuf0[3]=EOM_;

	while(DefenceClient<ClientSetMax){
		if((MineEnemyCheck(DefenceClient)!=dir)&&
		  ((NoReshuffleClient&no2bittbl[DefenceClient])==0))
			break;
		else
			DefenceClient++;
	}
	if(DefenceClient>=ClientSetMax)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else
		WazaSeqAdrs+=5;
}

void	WS_SIZENKAIHUKU(void)
{
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);

	if(PSP[ClientNo].speabino==TOKUSYU_SIZENKAIHUKU){
		PSP[ClientNo].condition=0;
		BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 BSS_PARA_CONDITION,
							 no2bittbl[UserWork[BEFORE_RESHUFFLE_SELMONSNO+ClientNo]],
							 4,
							 (u8 *)&PSP[ClientNo].condition);
		ClientBitSet(ClientNo);
	}
	WazaSeqAdrs+=2;
}

void	WS_HP_CHECK(void)
{
#if 0
	if(WazaSeqAdrs[1]==SIDE_ATTACK)
		ClientNo=AttackClient;
	else
		ClientNo=DefenceClient;
#else
	ClientNo=SideClientNoGet(WazaSeqAdrs[1]);
#endif

	if(PSP[ClientNo].hp==0)
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[2]<< 0)|
						   (WazaSeqAdrs[3]<< 8)|
						   (WazaSeqAdrs[4]<<16)|
						   (WazaSeqAdrs[5]<<24));
	else
		WazaSeqAdrs+=6;
}

#if 0
void	WS_IKARI_CHECK(void)
{
	if((PSP[DefenceClient].condition2&CONDITION2_IKARI)&&
	   (PSP[DefenceClient].hp!=0)){	//������`�F�b�N
		if((AttackClient!=DefenceClient)&&
		  ((WazaStatusFlag&WAZASTATUSFLAG_NOCRITICAL)==0)&&
		  ((OTF[DefenceClient].butsurihitdamage)||
		   (OTF[DefenceClient].tokusyuhitdamage))&&
		   (WTD[WazaNo].damage)){
			if(PSP[DefenceClient].abiritycnt[COND_POW]<12){
				PSP[DefenceClient].abiritycnt[COND_POW]++;
				PushAdrsSet(WazaSeqAdrs+1);
				WazaSeqAdrs=(u8 *)&WAZA_Ikari2;
				UserWork[WKC_WORK0]=WKC_TOKERU_CHECK;
				return;
			}
		}
	}
	WazaSeqAdrs++;
}
#endif

void	WS_HIMITUNOTIKARA(void)
{
	switch(GroundNo){
		case GROUND_KUSA:
			ServerWork[3]=COND_DOKU;
			break;
		case GROUND_LONGKUSA:
			ServerWork[3]=COND_NEMURI;
			break;
		case GROUND_SUNA:
			ServerWork[3]=COND2_HITDOWN;
			break;
		case GROUND_MIZU:
			ServerWork[3]=COND2_DEFDOWN;
			break;
		case GROUND_UMI:
			ServerWork[3]=COND2_POWDOWN;
			break;
		case GROUND_IKE:
			ServerWork[3]=COND2_AGIDOWN;
			break;
		case GROUND_IWA:
			ServerWork[3]=COND2_KONRAN;
			break;
		case GROUND_DOUKUTSU:
			ServerWork[3]=COND2_HIRUMU;
			break;
		default:
			ServerWork[3]=COND_MAHI;
			break;
	}
	WazaSeqAdrs++;
}

const u16 MonohiroiTbl[]={
	ITEM_IIKIZUGUSURI,		30,
	ITEM_NANDEMONAOSI,		40,
	ITEM_HAIPAABOORU,		50,
	ITEM_HUSIGINAAME,		60,
	ITEM_KAIHUKUNOKUSURI,	70,
	ITEM_GENKINOKAKERA,		80,
	ITEM_KINNOTAMA,			90,
	ITEM_TAURIN,			95,
	ITEM_POINTOAPPU,		99,
	ITEM_OUZYANOSIRUSI,		1,
};

void	WS_MONOHIROI(void)
{
	int	rnd;
	int	i,j;
	u16 monsno;
	u16 itemno;
	u8	speabi;

	for(i=0;i<6;i++){
		monsno=PokeParaGet(&PokeParaMine[i],ID_monsno_egg);
		itemno=PokeParaGet(&PokeParaMine[i],ID_item);
		if(PokeParaGet(&PokeParaMine[i],ID_speabi))
			speabi=PPD[monsno].speabi2;
		else
			speabi=PPD[monsno].speabi1;
		if((speabi==TOKUSYU_MONOHIROI)&&
		   (monsno!=0)&&
		   (monsno!=MONSNO_TAMAGO)&&
		   (itemno==0)&&
		  ((pp_rand()%10)==0)){
			rnd=pp_rand()%100;
			for(j=0;j<9*2;j+=2){
				if(MonohiroiTbl[j+1]>rnd)
				break;
			}
			PokeParaPut(&PokeParaMine[i],
						ID_item,
						(u8 *)&MonohiroiTbl[j]);
		}
	}
	WazaSeqAdrs++;
}

void	WS_TENKIYA(void)
{
	ClientNo=UserWork[CLIENT_NO_WORK];
	if(PSP[ClientNo].condition2&CONDITION2_MIGAWARI)
		UserWork[HENGE_NO]|=BW_MIGAWARI_FLAG;
//	if((WazaKouka[ClientNo]&WAZAKOUKA_KIE)==0){
		BSS_FIGHT_EFFECT_SET(BSS_SERVER,FE_ID_HENGE,UserWork[HENGE_NO]);
		ClientBitSet(ClientNo);
//	}
	WazaSeqAdrs++;
}

void	WS_TENKIYA_CHECK(void)
{
	int	ret;
	WazaSeqAdrs++;

	ret=SpeabiTenkiyaAct(UserWork[CLIENT_NO_WORK]);
	if(ret){
		ServerSpeabiSubroutineSet((u8 *)&WAZA_Tenkiya);
		UserWork[HENGE_NO]=ret-1;
	}
}

void	WS_ASOBI(void)
{
	int	ret=0;

	if(WTD[WazaNo].battleeffect==201){
		if((WazaKouka[AttackClient]&WAZAKOUKA_DOROASOBI)==0){
			WazaKouka[AttackClient]|=WAZAKOUKA_DOROASOBI;
			ServerWork[5]=0;
			ret++;
		}
	}
	else{
		if((WazaKouka[AttackClient]&WAZAKOUKA_MIZUASOBI)==0){
			WazaKouka[AttackClient]|=WAZAKOUKA_MIZUASOBI;
			ServerWork[5]=1;
			ret++;
		}
	}
	if(ret)
		WazaSeqAdrs+=5;
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_WEATHER_BALL(void)
{
	//�����m�[�Ă񂫂́A�v�Z�����Ȃ�
	if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_NOOTENKI,0,0)==0)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI_HP,0,TOKUSYU_EAROKKU,0,0)==0)){
		if(FieldCondition&FIELD_CONDITION_TENKI)
			UserWork[DAMAGE_VALUE]=2;

		if(FieldCondition&FIELD_CONDITION_AMEHURI)
			UserWork[DAMAGE_TYPE_WORK]=WATER_TYPE|0x80;
		else if(FieldCondition&FIELD_CONDITION_SUNAARASHIING)
			UserWork[DAMAGE_TYPE_WORK]=IWA_TYPE|0x80;
		else if(FieldCondition&FIELD_CONDITION_HIDERING)
			UserWork[DAMAGE_TYPE_WORK]=FIRE_TYPE|0x80;
		else if(FieldCondition&FIELD_CONDITION_ARARE)
			UserWork[DAMAGE_TYPE_WORK]=KOORI_TYPE|0x80;
		else
			UserWork[DAMAGE_TYPE_WORK]=NORMAL_TYPE|0x80;
	}
	WazaSeqAdrs++;
}

void	WS_RECYCLE(void)
{
	u16	*itemno;

	ClientNo=AttackClient;

	itemno=(u16 *)&UserWork[RECYCLE_WORK+ClientNo*2];

	if((*itemno)&&
	   (PSP[ClientNo].item==0)){
		ItemNo=*itemno;
		*itemno=0;
		PSP[ClientNo].item=ItemNo;
		BSS_POKEPARA_PUT_SET(BSS_SERVER,
							 BSS_PARA_ITEM,
							 BSS_SELMONSNO,
							 2,
							 (u8 *)&PSP[ClientNo].item);
		ClientBitSet(ClientNo);
		WazaSeqAdrs+=5;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

const u8 HogosyokuTypeTbl[]={
	KUSA_TYPE,		//GROUND_KUSA=0,
	KUSA_TYPE,		//GROUND_LONGKUSA,
	JIMEN_TYPE,		//GROUND_SUNA,
	WATER_TYPE,		//GROUND_MIZU,
	WATER_TYPE,		//GROUND_UMI,
	WATER_TYPE,		//GROUND_IKE,
	IWA_TYPE,		//GROUND_IWA,
	IWA_TYPE,		//GROUND_DOUKUTSU,
	NORMAL_TYPE,		//GROUND_ROOM,
	NORMAL_TYPE,		//GROUND_ALL,
};

void	WS_HOGOSYOKU(void)
{
	if((PSP[AttackClient].type1!=HogosyokuTypeTbl[GroundNo])&&
	   (PSP[AttackClient].type2!=HogosyokuTypeTbl[GroundNo])){
		PSP[AttackClient].type1=HogosyokuTypeTbl[GroundNo];
		PSP[AttackClient].type2=HogosyokuTypeTbl[GroundNo];
		EvStrBuf0[0]=I_MSG;
		EvStrBuf0[1]=EV_ZOKUSEI;
		EvStrBuf0[2]=HogosyokuTypeTbl[GroundNo];
		EvStrBuf0[3]=EOM_;
		WazaSeqAdrs+=5;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_HUKITOBASHI_OIUCHI(void)
{

	ClientNo=ClientNoGet(ClientTypeGet(AttackClient)^2);

	if((FightType&FIGHT_TYPE_2vs2)&&
	  ((NoReshuffleClient&no2bittbl[ClientNo])==0)&&
	   (CommandReturn[ClientNo]==BSS_SELECT_RETURN_FIGHT)&&
	   (SelectWazaNo[ClientNo]==WAZANO_OIUTI)){
		ActionNo[ClientNo]=SCA_EndCheckNo;
	   	WazaNo=WAZANO_OIUTI;
		WazaSeqAdrs+=5;
		UserWork[WAZA_EFF_CNT]=1;
		UserWork[PUSH_ATTACK_CLIENT]=AttackClient;
		AttackClient=ClientNo;
	}
	else
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
}

void	WS_YOKODORI_CLIENT_SET(void)
{
	TsuikaClient=AttackClient;

	if(AttackClient==DefenceClient)
		AttackClient=DefenceClient=UserWork[CLIENT_NO_WORK];
	else
		DefenceClient=UserWork[CLIENT_NO_WORK];

	UserWork[CLIENT_NO_WORK]=TsuikaClient;

	WazaSeqAdrs++;
}

void	WS_KAWARAWARI(void)
{
	u8	dir;

	dir=MineEnemyCheck(AttackClient)^1;
	if(SCC[dir].butsuriguardcnt||SCC[dir].tokusyuguardcnt){
		SideCondition[dir]&=SIDE_CONDITION_BUTSURIGUARD_OFF;
		SideCondition[dir]&=SIDE_CONDITION_TOKUSYUGUARD_OFF;
		SCC[dir].butsuriguardcnt=0;
		SCC[dir].tokusyuguardcnt=0;
		UserWork[WAZA_EFF_CNT]=1;
		UserWork[WAZA_EFF_CNT_TMP]=1;
	}
	else{
		UserWork[WAZA_EFF_CNT]=0;
		UserWork[WAZA_EFF_CNT_TMP]=0;
	}
	WazaSeqAdrs++;
}

//�A�C�e���V�[�P���X

const u8 BallRate[]={
		20,		//�n�C�p�[�{�[��
		15,		//�X�[�p�[�{�[��
		10,		//�����X�^�[�{�[��
		15,		//�T�t�@���{�[��
};

void	WS_BALL_THROW(void)
{
	u8	i;
	u32	hokakuritsu;
	u32	hokakuritsu2;
	u8	rate;
	u8	ballrate=0;

	if(ClientBit!=0) return;

	ClientNo=AttackClient;
	DefenceClient=AttackClient^1;

	if(FightType&FIGHT_TYPE_TRAINER){
		BSS_GET_FAIL_SET(BSS_SERVER,5);
		ClientBitSet(ClientNo);
		WazaSeqAdrs=(u8 *)&ITEM_TrainerMonsGet;
		return;
	}

	if(FightType&FIGHT_TYPE_GET_DEMO){		//�ߊl�f��
		BSS_GET_FAIL_SET(BSS_SERVER,4);
		ClientBitSet(ClientNo);
		WazaSeqAdrs=(u8 *)&ITEM_DemoMonsGetSuccess;
		return;
	}

//�T�t�@���{�[���Ȃ�߂Â����񐔂ŕߊl����ϓ�
	if(ItemNo==ITEM_SAFARIBOORU)
		rate=UserWork[SAFARI_GET_COUNT]*5*255/100;
	else
		rate=PPD[PSP[DefenceClient].monsno].get_rate;

	if(ItemNo>ITEM_SAFARIBOORU){
		switch(ItemNo){
			case ITEM_NETTOBOORU:
				if((PSP[DefenceClient].type1==WATER_TYPE)||
				   (PSP[DefenceClient].type2==WATER_TYPE)||
				   (PSP[DefenceClient].type1==MUSHI_TYPE)||
				   (PSP[DefenceClient].type2==MUSHI_TYPE))
					ballrate=30;
				else
					ballrate=10;
				break;
			case ITEM_DAIBUBOORU:
				if(GetNowMapType()==MAPTYPE_WATER)
					ballrate=35;
				else
					ballrate=10;
				break;
			case ITEM_NESUTOBOORU:
				if(PSP[DefenceClient].level<40){
					ballrate=40-PSP[DefenceClient].level;
					if(ballrate<10)
						ballrate=10;
				}
				else
					ballrate=10;
				break;
			case ITEM_RIPIITOBOORU:
				if(ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[DefenceClient].monsno),Z_GET_CHK))
					ballrate=30;
				else
					ballrate=10;
				break;
			case ITEM_TAIMAABOORU:
				ballrate=10+(1*FTW.FightTurn);
				if(ballrate>40)
					ballrate=40;
				break;
			case ITEM_GOOZYASUBOORU:
			case ITEM_PUREMIABOORU:
				ballrate=10;
				break;
		}
	}
	else
		ballrate=BallRate[ItemNo-2];

//	hokakuritsu=PPD[PSP[DefenceClient].monsno].get_rate/10*BallRate[ItemNo-2]*
//	(PSP[DefenceClient].hpmax*3-PSP[DefenceClient].hp*2)/PSP[DefenceClient].hpmax*3;
	hokakuritsu=((rate*ballrate)/10)*
				(PSP[DefenceClient].hpmax*3-PSP[DefenceClient].hp*2)/(PSP[DefenceClient].hpmax*3);

	if(PSP[DefenceClient].condition&(CONDITION_NEMURI|CONDITION_KOORI))
		hokakuritsu*=2;

	if(PSP[DefenceClient].condition&(CONDITION_DOKU|CONDITION_MAHI|CONDITION_YAKEDO))
		hokakuritsu=hokakuritsu*15/10;

	if(ItemNo!=ITEM_SAFARIBOORU){		//�T�t�@���{�[���̓e���r���Ɋ܂܂Ȃ�
		if(ItemNo==ITEM_MASUTAABOORU)
			FTW.ThrowMasterBallFlag=1;	//�}�X�^�[�{�[���𓊂������̃t���O
		else{
			if(FTW.ThrowBall[ItemNo-ITEM_HAIPAABOORU]<0xff)
				FTW.ThrowBall[ItemNo-ITEM_HAIPAABOORU]++;
		}
									//�퓬���Ƀ{�[���𓊂�����
	}

	if(hokakuritsu>=255){
		//�}�ӂɃQ�b�g�����t���O�Z�b�g
//		ZukanCheck(PSP[DefenceClient].monsno,Z_GET_SET);
//		ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[DefenceClient].monsno),Z_GET_SET);
		BSS_GET_FAIL_SET(BSS_SERVER,4);
		ClientBitSet(ClientNo);
		WazaSeqAdrs=(u8 *)&ITEM_MonsGetSuccess;
		PokeParaPut(&PokeParaEnemy[SelMonsNo[DefenceClient]],ID_get_ball,(u8 *)&ItemNo);
		if(PokeCountMineGet()==6)
			ServerWork[5]=0;
		else
			ServerWork[5]=1;
	}
	else{
		hokakuritsu2=(255<<16)/hokakuritsu;
		hokakuritsu2=Sqrt(hokakuritsu2);
		hokakuritsu=Sqrt(hokakuritsu2);
		hokakuritsu=(RAND_MAX<<4)/hokakuritsu;
		for(i=0;i<4;i++){
			if(pp_rand()>=hokakuritsu) break;
		}
		if(ItemNo==ITEM_MASUTAABOORU)
			i=4;				//�}�X�^�[�{�[��
		BSS_GET_FAIL_SET(BSS_SERVER,i);
		ClientBitSet(ClientNo);
		if(i==4){
			//�}�ӂɃQ�b�g�����t���O�Z�b�g
//			ZukanCheck(PSP[DefenceClient].monsno,Z_GET_SET);
//			ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[DefenceClient].monsno),Z_GET_SET);
			WazaSeqAdrs=(u8 *)&ITEM_MonsGetSuccess;
			PokeParaPut(&PokeParaEnemy[SelMonsNo[DefenceClient]],ID_get_ball,(u8 *)&ItemNo);
			if(PokeCountMineGet()==6)
				ServerWork[5]=0;
			else
				ServerWork[5]=1;
		}
		else{
			ServerWork[5]=i;
			WazaSeqAdrs=(u8 *)&ITEM_MonsGetFail;
		}
	}
}

void	WS_ADD_POKE(void)
{
#if 0
	if((AddPokemon(SelMonsNo[AttackClient^1]))==1){
		i=FightMsgCreate((u8 *)&PasoTensouMsg);
		FightMsgServerSet(i);
	}
#else
	AddPokemon(&PokeParaEnemy[SelMonsNo[AttackClient^1]]);
#endif
	FTW.GetPokeNo=PSP[AttackClient^1].monsno;
							//�߂܂����|�P�����̃|�P�����i���o�[
	PokeParaGet(&PokeParaEnemy[SelMonsNo[AttackClient^1]],ID_nickname,(u8 *)&FTW.GetPokeNickname);
							//�߂܂����|�P�����ɂ����j�b�N�l�[��
	WazaSeqAdrs++;
}

void	WS_ZUKAN_CHECK(void)
{
	if(ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[DefenceClient].monsno),Z_GET_CHK))
		WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
						   (WazaSeqAdrs[2]<< 8)|
						   (WazaSeqAdrs[3]<<16)|
						   (WazaSeqAdrs[4]<<24));
	else{
		ZukanCheck(PokeMonsNo2ZukanNoGet(PSP[DefenceClient].monsno),Z_GET_SET);
		if(PSP[DefenceClient].monsno==MONSNO_ANNOON)
			MyData.Zukan.UnknownRand=PSP[DefenceClient].personal_rnd;
		if(PSP[DefenceClient].monsno==MONSNO_PATTIIRU)
			MyData.Zukan.PachiRand=PSP[DefenceClient].personal_rnd;
		WazaSeqAdrs+=5;
	}
}

enum{
	ZUKAN_INIT=0,
	ZUKAN_WAIT,
	ZUKAN_END_INIT,
	ZUKAN_END,
};

void	WS_ZUKAN(void)
{
	switch(ServerWork[0]){
		case ZUKAN_INIT:
			PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
			ServerWork[0]++;
			break;
		case ZUKAN_WAIT:
			if(FadeData.fade_sw == 0){
				ServerWork[1]=ZukanSampleSet(PokeMonsNo2ZukanNoGet(PSP[DefenceClient].monsno),
																   PSP[DefenceClient].id_no,
																   PSP[DefenceClient].personal_rnd);
				ServerWork[0]++;
			}
			break;
		case ZUKAN_END_INIT:
			if((FadeData.fade_sw == 0)&&
			   (sys.pMainProc==MainFight)&&
			   (TaskTable[ServerWork[1]].occ==0)){
				*(vu16 *)REG_BG3CNT = BG_SCREEN_SIZE_1 | BG_PRIORITY_3
				| 26 << BG_SCREEN_BASE_SHIFT | 2 << BG_CHAR_BASE_SHIFT
				| BG_COLOR_16 | BG_MOS_OFF;
				DecordVram((void *)ground0_pch_ADRS,(void *)BG_VRAM+0x8000);
				DecordVram((void *)ground0_psc_ADRS,(void *)BG_VRAM+26*0x0800);
				DecordPaletteWork((void *)ground0_pcl_ADRS,PA_BG2,0x60);
				ScrX3=0x100;
				PaletteFadeReq(0x0000fffc,0,16,0,0);
				ServerWork[0]++;
			}
			break;
		case ZUKAN_END:
			if(FadeData.fade_sw == 0)
				WazaSeqAdrs++;
			break;
	}
}

enum{
	NICKNAME_WINOPEN,
	NICKNAME_SELECT,
	NICKNAME_INPUT,
	NICKNAME_INPUTWAIT,
	NICKNAME_NOINPUT,
};

void	WindowOpen(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	int	x,y;
	u16	chr=0;

	for(y=y1;y<=y2;y++){
		for(x=x1;x<=x2;x++){
			if(y==y1){
				if(x==x1)
					chr=WIN_CHR_UL;
				else if(x==x2)
					chr=WIN_CHR_UR;
				else
					chr=WIN_CHR_U;
			}
			else if(y==y2){
				if(x==x1)
					chr=WIN_CHR_DL;
				else if(x==x2)
					chr=WIN_CHR_DR;
				else
					chr=WIN_CHR_D;
			}
			else{
				if(x==x1)
					chr=WIN_CHR_ML;
				else if(x==x2)
					chr=WIN_CHR_MR;
				else
					chr=WIN_CHR_M;
			}
			if(mode)
				*(vu16 *)(BG_VRAM+WIN_SCR*0x800+y*0x40+x*2)=0;
			else
				*(vu16 *)(BG_VRAM+WIN_SCR*0x800+y*0x40+x*2)=chr;
		}
	}
}

void	YesNoCsrOn(void)
{
	*(vu16 *)(BG_VRAM+WIN_SCR*0x800+(WIN_X1+1)*2+
			 ((WIN_Y1+1)+ServerWork[1]*2)*0x40)=CSR_CHR_U;
	*(vu16 *)(BG_VRAM+WIN_SCR*0x800+(WIN_X1+1)*2+
			 ((WIN_Y1+2)+ServerWork[1]*2)*0x40)=CSR_CHR_D;
}

void	YesNoCsrOff(void)
{
	*(vu16 *)(BG_VRAM+WIN_SCR*0x800+(WIN_X1+1)*2+
			 ((WIN_Y1+1)+ServerWork[1]*2)*0x40)=SPACE_CHR_B;
	*(vu16 *)(BG_VRAM+WIN_SCR*0x800+(WIN_X1+1)*2+
			 ((WIN_Y1+2)+ServerWork[1]*2)*0x40)=SPACE_CHR_B;
}


void	WS_NICKNAME(void)
{
	switch(ServerWork[0]){
		case NICKNAME_WINOPEN:
			WindowOpen(WIN_X1,WIN_Y1,WIN_X2,WIN_Y2,0);
			MsgPutSet(&FightMsg,YesNoMsg,CGX_START+0x80,WIN_X1+2,WIN_Y1+1);
			MsgPut(&FightMsg);
			ServerWork[0]++;
			ServerWork[1]=0;
			YesNoCsrOn();
			break;
		case NICKNAME_SELECT:
			if((sys.Trg&U_KEY)==U_KEY){
				if(ServerWork[1]){
					SePlay(SE_SELECT);
					YesNoCsrOff();
					ServerWork[1]=0;
					YesNoCsrOn();
				}
			}
			if((sys.Trg&D_KEY)==D_KEY){
				if(ServerWork[1]==0){
					SePlay(SE_SELECT);
					YesNoCsrOff();
					ServerWork[1]=1;
					YesNoCsrOn();
				}
			}
			if((sys.Trg&A_BUTTON)==A_BUTTON){
				SePlay(SE_SELECT);
				if(ServerWork[1])
					ServerWork[0]=NICKNAME_NOINPUT;
				else{
					ServerWork[0]++;
					LightFadeReq(N_to_B);
				}
				break;
			}
			if((sys.Trg&B_BUTTON)==B_BUTTON){
				SePlay(SE_SELECT);
				ServerWork[0]=NICKNAME_NOINPUT;
				break;
			}
			break;
		case NICKNAME_INPUT:
			if(FadeData.fade_sw == 0){
#if 0
				NameIn(NAMEIN_POKE,
					(u8 *)&PokeParaEnemy[SelMonsNo[AttackClient^1]].PPP.nickname,
					PokeParaGet(&PokeParaEnemy[SelMonsNo[AttackClient^1]],ID_monsno),
					PokeSexGet(&PokeParaEnemy[SelMonsNo[AttackClient^1]]),
					MainFight);
#endif
				PokeParaGet(&PokeParaEnemy[SelMonsNo[AttackClient^1]],
							ID_nickname,
							(u8 *)&UserWork[NICKNAME_BUF]);
				NameIn(NAMEIN_POKE,
					(u8 *)&UserWork[NICKNAME_BUF],
					PokeParaGet(&PokeParaEnemy[SelMonsNo[AttackClient^1]],ID_monsno),
					PokeSexGet(&PokeParaEnemy[SelMonsNo[AttackClient^1]]),
					PokeParaGet(&PokeParaEnemy[SelMonsNo[AttackClient^1]],ID_personal_rnd,0),
					MainFight);
				ServerWork[0]++;
			}
			break;
		case NICKNAME_INPUTWAIT:
			if((sys.pMainProc==MainFight)&&
			   (FadeData.fade_sw==0)){
				PokeParaPut(&PokeParaEnemy[SelMonsNo[AttackClient^1]],
							ID_nickname,
							(u8 *)&UserWork[NICKNAME_BUF]);
				WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
								   (WazaSeqAdrs[2]<< 8)|
								   (WazaSeqAdrs[3]<<16)|
								   (WazaSeqAdrs[4]<<24));
			}
			break;
		case NICKNAME_NOINPUT:
			if(PokeCountMineGet()==6)
				WazaSeqAdrs+=5;
			else
				WazaSeqAdrs=(u8 *)((WazaSeqAdrs[1]<< 0)|
								   (WazaSeqAdrs[2]<< 8)|
								   (WazaSeqAdrs[3]<<16)|
								   (WazaSeqAdrs[4]<<24));
			break;
	}
}

void	WS_ITEM_HP_KAIFUKU(void)
{
	PSP[AttackClient].hp-=WazaDamage;
	WazaSeqAdrs++;
}

void	WS_ITEM_CONDITION_RECOVER(void)
{
	PSP[AttackClient].condition=0;
	WazaSeqAdrs++;
}

void	WS_ITEMEND(void)
{
	ActionSeqNo=SCA_EndNo;
}

void	WS_GETEND(void)
{
	ActionSeqNo=SCA_EndNo;
	ActionCount=ClientSetMax;
//	WinLoseFlag=FIGHT_END;
}
