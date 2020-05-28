#include	"fightmsg.def"

//�퓬���b�Z�[�W�p�̓���R�[�h

#define	I_MSG			I_MSG_		//�퓬�ł����ŋ@�\���鉼�̋@�\

enum{
	EV_STR_BUF0=0,		//EvStrBuf0�̒��g���Q��
	EV_STR_BUF1,		//EvStrBuf1�̒��g���Q��
	MY_MONS,			//�������̃|�P�����̖��O���Q��
	ENEMY_MONS,			//���葤�̃|�P�����̖��O���Q��
	MY_MONS2,			//�������̃|�P�����̖��O���Q��(2vs2)
	ENEMY_MONS2,		//���葤�̃|�P�����̖��O���Q��(2vs2)
	MY_TMONS,			//�������̃|�P�����̖��O���Q��
	ENEMY_TMONS,			//���葤�̃|�P�����̖��O���Q��
	MY_TMONS2,			//�������̃|�P�����̖��O���Q��(2vs2)
	ENEMY_TMONS2,		//���葤�̃|�P�����̖��O���Q��(2vs2)
	SIDE_MONS,			//AttackClient���݂Ď��������肩�𔻒f���āA
						//�����瑤�̃|�P�����̖��O���Q��
	SIDE_MONS2,			//AttackClient���݂Ď��������肩�𔻒f���āA
						//�����瑤�̃|�P�����̖��O���Q��(2vs2)
	ATTACK_MONS,		//AttackClient�̃|�P�����̖��O���Q��
	DEFENCE_MONS,		//DefenceClient�̃|�P�����̖��O���Q��
	TSUIKA_MONS,		//TsuikaClient�̃|�P�����̖��O���Q��
	CLIENT_MONS,		//ClientNo�̃|�P�����̖��O���Q��
	CLIENTNOWORK,		//UserWork[CLIENT_NO_WORK]�̃|�P�����̖��O���Q��
	WAZA_NAME,			//WazaNo����Z�̖��O���Q��
	WAZA_NAME_TMP,		//WazaNoTmp����Z�̖��O���Q��
	ITEM_NAME,			//ItemNo����A�C�e���̖��O���Q��
	SPEABI_NAME,		//SpeabiNo�������\�̖͂��O���Q��
	SPEABI_NAMEA,		//SpeabiNo�������\�̖͂��O���Q��(AttackClient)
	SPEABI_NAMED,		//SpeabiNo�������\�̖͂��O���Q��(DefenceClient)
	SPEABI_NAMEC,		//SpeabiNo�������\�̖͂��O���Q��(CLIENT_NO_WORK)
	SPEABI_NAMET,		//SpeabiNo�������\�̖͂��O���Q��(TsuikaClient)
	TRAINER_TYPE,		//FieldTrainerID����g���[�i�[�^�C�v���Q��
	TRAINER_NAME,		//FieldTrainerID����g���[�i�[�����Q��
	TRAINER_MYNAME,		//�ʐM�g���[�i�[�����Q��
	TRAINER_MYNAME2,	//�ʐM�g���[�i�[�����Q��
	TRAINER_ENENAME,	//�ʐM�g���[�i�[�����Q��
	TRAINER_ENENAME2,	//�ʐM�g���[�i�[�����Q��
	TRAINER_CLIENTNO,	//�ʐM�g���[�i�[�����Q��
	MY_NAME,			//�����̖��O���Q��
	TRAINER_LOSE,		//FieldTrainerID����g���[�i�[�̕����䎌���Q��
	CLIENT_SELMONS_WORK,	//UserWork[CLIENT_NO_WORK]
							//UserWork[SEL_MONS_WORK]����|�P�����̖��O���Q��	
	PASO_NAME,			//���ꂩ�̃p�\�R�����}���~�̃p�\�R��
	SIDE_ATTACK_NAMENO,	//�����āA�݂���
	SIDE_DEFENCE_NAMENO,	//�����āA�݂���
	SIDE_ATTACK_NAMEHA,	//�����āA�݂���
	SIDE_DEFENCE_NAMEHA,	//�����āA�݂���
	SIDE_ATTACK_NAMEWO,	//�����āA�݂���
	SIDE_DEFENCE_NAMEWO,	//�����āA�݂���
};

//�퓬���b�Z�[�W�p�̓���R�[�h(EvStrBuf�ɐݒ肳���R�[�h)
//EvStrBuf[0]�ɂ�I_MSG�������Ă���

enum{
	EV_MSGID=0,			//����2�o�C�g�����b�Z�[�WID�Ɖ��߂���
	EV_NUM,				//���l�f�[�^�𕶎��ɕϊ�����
						//+1:EV_NUM
						//+2:�f�[�^�̃o�C�g��
						//+3:���l�̌���
						//+4-:�f�[�^�̃o�C�g�����̐��l�f�[�^
	EV_WAZANO,			//����2�o�C�g���Z�i���o�[�Ɖ��߂���
	EV_ZOKUSEI,			//����1�o�C�g�𑮐��i���o�[�Ɖ��߂���
	EV_NICKNAME,		//�|�P�����̃j�b�N�l�[���ɕϊ�����
						//+1:EV_NICKNAME
						//+2:ClientNo
						//+3:SelMonsNo
	EV_STATUS,			//����1�o�C�g���X�e�[�^�X�i���o�[�Ɖ��߂���
	EV_MONSNAME,		//����2�o�C�g�������X�^�[�i���o�[�Ɖ��߂���
						//�����X�^�[�̖��O�ɕϊ�����iNICKNAME�ł͂Ȃ��j
	EV_NICKNAMEONLY,	//�|�P�����̃j�b�N�l�[���ɕϊ�����(�u�Ă��́v�����Ȃ��j
						//+1:EV_NICKNAMEONLY
						//+2:ClientNo
						//+3:SelMonsNo
	EV_TASTE,			//����1�o�C�g��Taste�i���o�[�Ɖ��߂���
	EV_SPEABINO,		//����1�o�C�g������i���o�[�Ɖ��߂���
	EV_ITEMNO,			//����2�o�C�g���A�C�e���i���o�[�Ɖ��߂���
};

//����Ȕ���n
#if 0
	DousuruMsg,
	YesNoMsg,
	EnemyMsg,
	TaikiMsg,
#endif

#ifndef	__FIGHTMSG_H_
#define	__FIGHTMSG_H_

typedef struct{
	u16	WazaNo;
	u16	WazaNoTmp;

	u16	ItemNo;
	u8	SpeabiNo;
	u8	ClientNoWork;			//UserWork[CLIENT_NO_WORK]

	u8	SelMonsWork;			//UserWork[SEL_MONS_WORK]
	u8	MessageOffset;			//UserWork[MESSAGE_OFFSET]
	u8	ItemClient;
	u8	WazaType;

	u8	Speabi[4];

	u8	EvStrBuf0[16];
	u8	EvStrBuf1[16];
}FightMsgStruct;

extern	void	FightMsgID(u16);
extern	int		FightMsgCreate(u8 *);
extern	int		FightMsgCreateAct(u8 *,u8 *);
extern	void	WazaMsgCreate1(u8 *);
extern	void	WazaMsgCreate2(u8 *);

extern	const u16 NoHitMsgTbl[];
extern	const u16 ShimeWazaNo[];

extern	const u8 * const FightMsgTbl[];

//���[�J���C�Y�̂��߂ɓ����������b�Z�[�W�Q
extern	const u8 ShinkaBeforeMsg[];
extern	const u8 ShinkaAfterMsg[];
extern	const u8 ShinkaCancelMsg[];
extern	const u8 DousuruMsg[];
extern	const u8 SafariDousuruMsg[];
extern	const u8 DemoDousuruMsg[];
extern	const u8 TaikiMsg[];
extern	const u8 CommandMsg[];
extern	const u8 SafariCommandMsg[];
extern	const u8 WazaInfoMsg[];
extern	const u8 ColBlueMsg[];
extern	const u8 WasureMsg[];
extern	const u8 YesNoMsg[];
extern	const u8 WazaSwapMsg[];
extern	const u8 SafariBallMsg[];
extern	const u8 NokoriMsg[];
extern	const u8 NankoMsg[];
extern	const u8 NemuriMsg[];
extern	const u8 DokuMsg[];
extern	const u8 YakedoMsg[];
extern	const u8 MahiMsg[];
extern	const u8 KooriMsg[];
extern	const u8 KonranMsg[];
extern	const u8 MeromeroMsg[];
extern	const u8 SpaceMsg[];
extern	const u8 ScrollWaitMsg[];
extern	const u8 CRMsg[];
extern	const u8 HaMsg[];
extern	const u8 HaScrollWaitMsg[];
extern	const u8 LevelUpWindowMsg[];
extern	const u8 LevelUpWindowMsgArrow[];
extern	const u8 LevelUpWindowMsgPlus[];
extern	const u8 LevelUpWindowMsgMinus[];
extern	const u8 * const LevelUpWindowMsgTbl[];
extern	const u8 DarekanoMsg[];
extern	const u8 MayuminoMsg[];
extern	const u8 HuseiNaTamagoMsg[];
extern	const u8 KeikentiGetNormalMsg[];
extern	const u8 KeikentiGetOomeMsg[];
extern	const u8 TokuseiNoReshuffleMsg[];
extern	const u8 MitsuruNameMsg[];
extern	const u8 FightWinMsg[];
extern	const u8 FightLoseMsg[];
extern	const u8 FightDrawMsg[];

#endif	__FIGHTMSG_H_

