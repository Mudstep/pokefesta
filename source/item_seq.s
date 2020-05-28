
@======================================================
@	item.s
@======================================================

	.text
	.code 32

	.include	"waza_seq.def"
@	.include	"itemmsg.def"

#include	"record.h"
#include	"fightmsg.def"
#include	"server.def"
#include	"..\data\itemsym.h"
#include	"..\snd_file\mus_def.h"
#include	"client.def"

@======================================================
@	define�錾
@======================================================

MOJI_WAIT	=	0x040
MOJI_WAIT2	=	0x030

@======================================================
@	global�錾
@======================================================

	.global		ItemBallTbl
	.global		ItemNormalTbl
	.global		ItemFightTbl
	.global		SpecialActionTbl
	.global		ITEM_MonsGetSuccess
	.global		ITEM_MonsGetFail
	.global		ITEM_DemoMonsGetSuccess
	.global		ITEM_TrainerMonsGet

@======================================================
@	extern�錾
@======================================================

	.extern		SafariBallCount

ItemBallTbl:
	.long	ITEM_MonsterBall		@000 �_�~�[
	.long	ITEM_MonsterBall		@001 �}�X�^�[�{�[��
	.long	ITEM_MonsterBall		@002 �n�C�p�[�{�[��
	.long	ITEM_MonsterBall		@003 �X�[�p�[�{�[��
	.long	ITEM_MonsterBall		@004 �����X�^�[�{�[��
	.long	ITEM_SafariBall			@005 �T�t�@���{�[��
	.long	ITEM_MonsterBall		@006
	.long	ITEM_MonsterBall		@007
	.long	ITEM_MonsterBall		@008
	.long	ITEM_MonsterBall		@009
	.long	ITEM_MonsterBall		@010
	.long	ITEM_MonsterBall		@011
	.long	ITEM_MonsterBall		@012

ItemNormalTbl:
	.long	ITEM_Normal				@�������g�����A�C�e��
									@�|�P�������X�g��ŏ����͏I����Ă���̂ŁAITEMEND����
	.long	ITEM_Kaifukunokusuri
	.long	ITEM_HPKaifuku
	.long	ITEM_ConditionKaifuku
	.long	ITEM_AbirityUp
	.long	ITEM_EffectGuard

ItemFightTbl:
	.long	ITEM_PippiNingyou

//����ȃV�[�P���X�i�{���̓A�C�e���Ƃ͖��֌W�j
SpecialActionTbl:
	.long	SPEACT_Safari
	.long	SPEACT_Near
	.long	SPEACT_CubeThrow
	.long	SPEACT_GetDemo

@===================================================
@	001 �����X�^�[�{�[��
@===================================================

ITEM_MonsterBall:
	IF16			FLAG_BIT,FightType,FIGHT_TYPE_GET_DEMO,ITEM_DemoMonsterBall	@�ߊl�f���͕ʃ��b�Z�[�W
	MESSAGE			ITEMUSEMSGNO
	BALL_THROW
ITEM_DemoMonsterBall:
	MESSAGE			DEMOITEMUSEMSGNO
	BALL_THROW

ITEM_SafariBall:
	MESSAGE			ITEMUSEMSGNO
	CONDITION_GAUGE	SIDE_ATTACK			@�Q�[�W���̃{�[������REDRAW
	BALL_THROW

ITEM_MonsGetSuccess:
	IF16			FLAG_EQ,ItemNo,ITEM_SAFARIBOORU,ITEM_NoCountUp
	INC_RECORD		CNT_CATCH_POKE		@�|�P�������Q�b�g�����񐔂�CountUp
ITEM_NoCountUp:
	MESSAGE			GETSUCCESSMSGNO
	ZUKAN_CHECK		ITEM_NoZukanSave
	MESSAGE			ZUKANSAVEMSGNO
	CLIENT_BIT_WAIT
	SETVALUE		ServerWork,0		@�V�[�P���X�i���o�[�Ƃ��Ďg�p�i�������j
	ZUKAN
ITEM_NoZukanSave:
	MESSAGE			NICKNAMEMSGNO
	CLIENT_BIT_WAIT
	SETVALUE		ServerWork,0		@�V�[�P���X�i���o�[�Ƃ��Ďg�p�i�������j
	NICKNAME		ITEM_NickName
	MESSAGE			PASOTENSOUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
ITEM_NickName:
	ADD_POKE
	SETVALUE		WinLoseFlag,FIGHT_ENEMY_GET
	GETEND

ITEM_DemoMonsGetSuccess:
	MESSAGE			DEMOGETSUCCESSMSGNO
	SETVALUE		WinLoseFlag,FIGHT_ENEMY_GET
	GETEND

ITEM_MonsGetFail:
	MESSAGE_TBL		GetFailMsgTbl
	MESSAGE_WAIT	MOJI_WAIT
	@�T�t�@���]�[���̎��ɁA�{�[���̃J�E���g���`�F�b�N����
	IF8				FLAG_NBIT,FightType,FIGHT_TYPE_SAFARI,ITEM_MonsGetFailRet
	IF8				FLAG_NE,SafariBallCount,0,ITEM_MonsGetFailRet
	MESSAGE			SAFARIZONEFINISHMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SETVALUE		WinLoseFlag,FIGHT_SAFARIBALL_NONE
ITEM_MonsGetFailRet:
@	SEQEND
	ITEMEND

ITEM_TrainerMonsGet:
	MESSAGE_WAIT	MOJI_WAIT
	MESSAGE			TRAINERMONSMSG1NO
	MESSAGE_WAIT	MOJI_WAIT
	MESSAGE			TRAINERMONSMSG2NO
	MESSAGE_WAIT	MOJI_WAIT
@	SEQEND
	ITEMEND

@===================================================
@	001 �m�[�}���A�C�e��
@===================================================

ITEM_Normal	:
@	SEQEND
	SETVALUE		UserWork+WKC_WORK0,WKC_OUMU_WAZA_NO
	WKC_ACT			WKC_CHECK_ONLY,0
@	ITEMEND
	WAZAEND

ITEM_Kaifukunokusuri:
ITEM_HPKaifuku:
	WAIT			MOJI_WAIT2
	SE_PLAY			SE_KAIFUKU
	MESSAGE			TRAINERITEMUSEMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	STATUS_RECOVER
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT		ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
@	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_KAIFUKU,0
	HP_DEC			SIDE_ATTACK
	HP_DEC_WAIT		SIDE_ATTACK
	MESSAGE			ITEMHPKAIFUKUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_ATTACK
	SETVALUE		UserWork+WKC_WORK0,WKC_OUMU_WAZA_NO
	WKC_ACT			WKC_CHECK_ONLY,0
	ITEMEND

ITEM_ConditionKaifuku:
	WAIT			MOJI_WAIT2
	SE_PLAY			SE_KAIFUKU
	MESSAGE			TRAINERITEMUSEMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	STATUS_RECOVER
@	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_KAIFUKU,0
	MESSAGE_TBL		ItemRecoverTbl
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_ATTACK
	SETVALUE		UserWork+WKC_WORK0,WKC_OUMU_WAZA_NO
	WKC_ACT			WKC_CHECK_ONLY,0
	ITEMEND

ITEM_AbirityUp:
	WAIT			MOJI_WAIT2
	SE_PLAY			SE_KAIFUKU
	MESSAGE			TRAINERITEMUSEMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	STATUS_RECOVER
@	IF8				FLAG_NE,ServerWork+5,4,ITEM_AbirityUpNoEffect
@	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
@ITEM_AbirityUpNoEffect:
	MESSAGE_TBL		CondUpMsgTbl
	MESSAGE_WAIT	MOJI_WAIT
	SETVALUE		UserWork+WKC_WORK0,WKC_OUMU_WAZA_NO
	WKC_ACT			WKC_CHECK_ONLY,0
	ITEMEND

ITEM_EffectGuard:
	WAIT			MOJI_WAIT2
	SE_PLAY			SE_KAIFUKU
	MESSAGE			TRAINERITEMUSEMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	STATUS_RECOVER
	MESSAGE_TBL		ShiroikiriMsgTbl
	MESSAGE_WAIT	MOJI_WAIT
	SETVALUE		UserWork+WKC_WORK0,WKC_OUMU_WAZA_NO
	WKC_ACT			WKC_CHECK_ONLY,0
	ITEMEND

@===================================================
@	001 �퓬�Ō��ʂ𔭊�����A�C�e��
@===================================================

ITEM_PippiNingyou:
@	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_PIPPININGYOU,0
	SE_PLAY			SE_NIGERU
	SETVALUE		WinLoseFlag,FIGHT_ESCAPE
	GETEND

@===================================================
@	����A�N�V����
@===================================================

@===================================================
@	000 �T�t�@�����L�̃A�N�V����
@===================================================

SPEACT_Safari:
	MESSAGE			YOUSUUKAGAUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SEQEND

@===================================================
@	001 �����Â�
@===================================================

SPEACT_Near:
	MESSAGE_TBL		NearMsgTbl
	MESSAGE_WAIT	MOJI_WAIT
	SEQEND

@===================================================
@	002 �L���[�u�𓊂���
@===================================================

SPEACT_CubeThrow:
	MESSAGE			CUBETHROWMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_POROC,0
	MESSAGE_TBL		CubeEatMsgTbl
	MESSAGE_WAIT	MOJI_WAIT
	SEQEND

@===================================================
@	003 �ߊl�f��
@===================================================

SPEACT_GetDemo:
	MESSAGE				IREKAEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	MODOSU_EFFECT
	CLIENT_BIT_WAIT
	TRAINER_APPEAR		SIDE_MINE
	CLIENT_BIT_WAIT
	MESSAGE				MITSURUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

