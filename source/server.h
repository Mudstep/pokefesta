#include	"gamedata.h"
#include	"server.def"
#define	SBD_WORK			((SECRET_BASE_DATA *)&UserWork[0x17000])
#define	SAD_WORK			((SpecialAbirityData *)&UserWork[0x17100])
#define	PUSH_ADRS			((PushAdrs *)&UserWork[0x17110])
#define	PUSH_SERVER_ADRS	((PushServerAdrs *)&UserWork[0x17140])
#define	PUSH_POKE_PARAM		((PushPokeParam *)&UserWork[0x17180])

//#define	MATSUDA_USER_WORK_START	0x17800				//���c�N���퓬�Ŏg�p�ł���UserWork�̃X�^�[�g
														//���͐G�����Ⴞ��(���ۂ�client.h�ɒ�`�j

enum{
	SPEABI_POKEAPPEAR=0,
	SPEABI_TURN_CHECK,
	SPEABI_WAZA_HIT_BEFORE,
	SPEABI_WAZA_HIT,
	SPEABI_WAZA_HIT_AFTER,
	SPEABI_WAZA_HIT_AFTER_ALL,
	SPEABI_WAZA_HIT_AFTER_TENKIYA,
	SPEABI_SINKURO_DEFENCE,
	SPEABI_SINKURO_ATTACK,
	SPEABI_IKAKU,
	SPEABI_IKAKU2,
	SPEABI_TRACE,
	SPEABI_ENEMYHAVE_SPEABI,	//���葤�����̓���\�͂������Ă��邩�H
	SPEABI_MYHAVE_SPEABI,		//�����������̓���\�͂������Ă��邩�H
	SPEABI_HAVE_SPEABI,			//�퓬�ɎQ�����Ă���|�P���������̓���\�͂������Ă��邩�H
	SPEABI_HAVE_SPEABI_NOMINE,	//�퓬�ɎQ�����Ă��鎩���ȊO�̃|�P���������̓���\�͂������Ă��邩�H
	SPEABI_ENEMYHAVE_SPEABI_CNT,//���葤�����̓���\�͂������Ă��邩�J�E���g
	SPEABI_MYHAVE_SPEABI_CNT,	//�����������̓���\�͂������Ă��邩�J�E���g
	SPEABI_HAVE_SPEABI_NOMINE_CNT,	//�퓬�ɎQ�����Ă��鎩���ȊO�̃|�P���������̓���\�͂������Ă��邩�H
	SPEABI_HAVE_SPEABI_HP,		//�퓬�ɎQ�����Ă���|�P���������̓���\�͂������Ă��邩�H
								//HP0�ł́A���̓����͔������Ȃ�
};

enum{
	SOUBI_POKEAPPEAR=0,
	SOUBI_TURN_CHECK,
	SOUBI_WAZA_HIT,
	SOUBI_WAZA_HIT_AFTER_ALL,
	SOUBI_WAZA_HIT_AFTER,
};

#ifndef	__SERVER_H_
#define __SERVER_H_

extern	pFunc	ServerSequence;
extern	pFunc	ClientSequence[CLIENT_MAX];
extern	pFunc	PushFirstProc;

extern	u8		ServerBuffer[CLIENT_MAX][512];
extern	u8		ClientBuffer[CLIENT_MAX][512];

extern	u8		ClientNo;
extern	u32		ClientBit;
extern	u8		ClientSetMax;
extern	u8		ClientType[CLIENT_MAX];

extern	u16		SelMonsNo[CLIENT_MAX];

extern	u8		ActionNo[CLIENT_MAX];
extern	u8		ActionClientNo[CLIENT_MAX];
extern	u8		ActionCount;
extern	u8		ActionSeqNo;

extern	PokemonServerParam	PSP[CLIENT_MAX];

extern	u8		PokemonStruct[CLIENT_MAX];

extern	void	ServerDummy(void);
extern	void	ServerInit(void);
extern	void	ServerMain(void);
extern	u8		CriticalValue;
extern	void	ClientBitSet(u8);
extern	void	ClientBitSet2(u8);
extern	u8		SpecialAbirityCheck(u8,u8,u8,u8,u16);
extern	void	FightJoinFlagSet(u8 clientno);

extern	u8		AttackClient;
extern	u8		DefenceClient;
extern	u8		NoReshuffleClient;
extern	u8		TsuikaClient;
extern	u8		ItemClient;

extern	u8		WinLoseFlag;
//extern	int		FightMsgCreate(u8 *);

extern	u8		ServerWork[8];					//�T�[�o�[�p�ėp���[�N

extern	u32		ServerStatusFlag;					//�T�[�o�[�p�ėp���[�N

extern	u16		FieldCondition;
extern	u16		EncountEffectFlag;

extern	s32		WazaDamage;
extern	u16		WazaNo;
extern	u16		WazaNoTmp;
extern	u16		WazaNoUse[CLIENT_MAX];
extern	u16		WazaNoHit[CLIENT_MAX];
extern	u8		WazaNoAttackClient[CLIENT_MAX];
extern	u8		WazaNoDefenceClient[CLIENT_MAX];
extern	u16		ItemNo;
extern	u8		SpeabiNo;

extern	FightTVWork	FTW;
extern	FightRamSeed FRS[CLIENT_MAX];

extern	const	u8	SpecialAbirityMsg[][8];
extern	const	u8	SpecialAbirityInfoMsg[][19];

extern	u32		WazaKouka[CLIENT_MAX];
extern	WazaKoukaCount	WKC[CLIENT_MAX];

extern	u16		SideCondition[2];
extern	SideConditionCount	SCC[2];

extern	u8	AgiCalc(u8,u8,u8);

extern	u8	PokeSelFlag;		//�|�P��������ւ��t���O

extern	u8	WazaStatusFlag;
extern	u8	WazaSeqLoopCount;

extern	u16	DamagePowWork;

extern	u8		CommandReturn[CLIENT_MAX];

extern	void	WindowOpen(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
extern	void	YesNoCsrOn(void);
extern	void	YesNoCsrOff(void);
extern	u8		WaruagakiWazaNoCheck(u8,u8,u8);

extern	void	OboeWazaNoSet(void);

extern	void	DamageCalcAct(u8 attack,u8 defence);
extern	u8		TypeCheckAct(u16 wazano,u8 attack,u8 defence);
extern	u8		TypeCheckAct2(u16 wazano,u16 monsno,u8 speabino);
extern	void	DamageLossAct(void);

#endif

