//=========================================================================
//
//
//	�t�B�[���h�G���J�E���g
//
//
//=========================================================================

#include "common.h"
#include "field.h"
#include "task.h"
#include "print.h"
#include "poketool.h"
#include "script.h"
#include "fld_main.h"
#include "fld_task.h"
#include "fieldmap.h"
#include "palanm.h"
#include "server.h"
#include "evobj.h"
#include "jiki.h"
#include "fld_tool.h"
#include "mus_tool.h"
#include "mapparam.h"
#include "fight.h"
#include "map_attr.h"
#include "weather.h"
#include "laster.h"
#include "pokescr.h"
#include "record.h"
#include "syswork.h"
#include "ec_eff.h"
#include "ev_fight.h"		//IsGameOverCheck
#include "ev_check.h"

#include "trainer.h"
#include "tr_enum.h"
#include "monsno.def"
#include "safari.h"			//SafariEncountEnd	IsFieldSafariMode
#include "../evdata/maparray.h"
#include "wea_ctrl.h"
#include "trainer.def"

#define	__ENCOUNT_H_GLOBAL__
#include "encount.h"




static void encount_task_set(u8 enc_type, u16 music);
static void EncountFightEnd( void );
static void ScrEncountFightEnd(void);

static void start_normal_encount( void );
static void start_safari_encount( void );

static u8 get_wild_effect( void );
static u8 get_trainer_effect( void );

//=========================================================================
//
//	�G���J�E���g����^�X�N
//
//=========================================================================

extern int CheckFieldPoisonEffect( void );

//------------------------------------------
//	�G���J�E���g�G�t�F�N�g���퓬��
//------------------------------------------
static void encount_task( u8 no )
{
	s16 * twk = TaskTable[no].work;

	switch ( twk[0] ) {

	case 0:
		//�ł̃G�t�F�N�g������ꍇ�͏I����҂�
		if ( CheckFieldPoisonEffect() == FALSE ) {
			SetEcEffect( twk[1] );
			twk[0] ++;
		}
		break;

	case 1:
		if ( CheckEcEffect() == TRUE ) {
			MainProcChange( InitFight );
			InitEncountStep();				//�G���J�E���g�̕�������������
			PoisonStepInit();				//�Ń_���[�W����̕���������
			DelTask( no );
		}
		break;
	}
}

static void encount_task_set(u8 enc_type, u16 music)
{
	u8 no;
	no = AddTask(encount_task,1);
	TaskTable[no].work[1] = enc_type;
	FightBGMSet2(music);
}

//=========================================================================
//
//	�쐫�|�P�����G���J�E���g�`�F�b�N�`�퓬�J�n
//
//=========================================================================

//------------------------------------------
//	�ʏ�G���J�E���g�J�n
//------------------------------------------
void StartWildEncountEvent( void )
{
	if ( IsFieldSafariMode() ) {
		start_safari_encount();
	} else {
		start_normal_encount();
	}
}

//------------------------------------------
//	�쐶�|�P�����G���J�E���g�J�n
//------------------------------------------
static void start_normal_encount( void )
{
	SetForceEvent();
	EvObjPauseAll();
	SetHeroMoveStateInit();
	sys.pReturnProc = EncountFightEnd;
	FightType = FIGHT_TYPE_1vs1;
	encount_task_set(get_wild_effect(), 0);
	IncRecord( CNT_BATTLE );
	IncRecord( CNT_WILD_BATTLE );
}

//------------------------------------------
//	�ړ��|�P�����G���J�E���g�J�n
//------------------------------------------
void StartMvPokeEncountEvent( void )
{
	SetForceEvent();
	EvObjPauseAll();
	SetHeroMoveStateInit();
	sys.pReturnProc = EncountFightEnd;
	FightType = FIGHT_TYPE_1vs1 | FIGHT_TYPE_MV_POKE;
	encount_task_set(get_wild_effect(), 0);
	IncRecord( CNT_BATTLE );
	IncRecord( CNT_WILD_BATTLE );
}

//------------------------------------------
//	�T�t�@���G���J�E���g�J�n
//------------------------------------------
static void start_safari_encount( void )
{
	SetForceEvent();
	EvObjPauseAll();
	SetHeroMoveStateInit();
	sys.pReturnProc = SafariEncountEnd;		//���A�惁�C���V�[�P���X
	FightType = FIGHT_TYPE_1vs1 | FIGHT_TYPE_SAFARI;
	encount_task_set(get_wild_effect(), 0);
//	IncRecord( CNT_BATTLE );		�T�t�@���̓J�E���g���Ȃ���
//	IncRecord( CNT_WILD_BATTLE );
}

//=========================================================================
//	�g���[�i�[�G���J�E���g�J�n
//=========================================================================
void StartTrainerEncountEvent( void )
{
	encount_task_set(get_trainer_effect(), 0);
	IncRecord( CNT_BATTLE );
	IncRecord( CNT_TRAINER_BATTLE );
}


//=========================================================================
//	�ߊl�f���J�n
//	���X�N���v�g����v���O�����Ăяo���ŋN��
//=========================================================================
void StartCaptureDemoEncountEvent( void )
{
	//�񃌃A�J���[�A���ʁ��ŌŒ肷�邽�ߐ�p�̃|�P�����Z�b�g�֐����Ă�
	PokeParaSetDemo( &PokeParaEnemy[0], MONSNO_RARUTOSU, 5 );

	SetForceEvent();
	sys.pReturnProc = FieldScriptRecover;		//���A�惁�C���V�[�P���X
	FightType = FIGHT_TYPE_1vs1 | FIGHT_TYPE_GET_DEMO;
	encount_task_set(EC_BGSHIFT, 0);
}

//=========================================================================
//	�X�N���v�g����̖쐶�|�P������Ăяo��
//=========================================================================
void StartScriptEncountEvent( void )
{
	SetForceEvent();
	sys.pReturnProc = ScrEncountFightEnd;
	FightType = FIGHT_TYPE_1vs1;
	encount_task_set(get_wild_effect(), 0);
	IncRecord( CNT_BATTLE );
	IncRecord( CNT_WILD_BATTLE );
}

//=========================================================================
//	���ʂȃ|�P�����Ƃ̐퓬�Ăяo���i���e�B�A�X�E���e�B�I�X�j
//	���G���J�E���g�l�r�f���Ⴄ
//=========================================================================
void StartKokoroPokeEncountEvent( void )
{
	SetForceEvent();
	sys.pReturnProc = ScrEncountFightEnd;
	FightType = FIGHT_TYPE_1vs1 | FIGHT_TYPE_SPECIAL;
	encount_task_set(get_wild_effect(), 0);
	IncRecord( CNT_BATTLE );
	IncRecord( CNT_WILD_BATTLE );
}

//=========================================================================
//	���ʂȃ|�P�����Ƃ̐퓬�Ăяo���i�^�C�N�[���j
//	���G���J�E���g�l�r�f���a�f�l���Ⴄ
//	���G���J�E���g�G�t�F�N�g�w�肠��
//=========================================================================
void StartSpPokeEncountEvent( void )
{
	SetForceEvent();
	sys.pReturnProc = ScrEncountFightEnd;
	FightType = FIGHT_TYPE_1vs1 | FIGHT_TYPE_SPECIAL;
	encount_task_set(EC_MOSAIC, MUS_BATTLE34);
	IncRecord( CNT_BATTLE );
	IncRecord( CNT_WILD_BATTLE );
}

//=========================================================================
//	�Ñ�|�P�����Ƃ̐퓬�Ăяo��
//	���G���J�E���g�l�r�f���퓬�w�i���a�f�l���Ⴄ
//	���G���J�E���g�G�t�F�N�g�w�肠��
//=========================================================================
void StartKodaiPokeEncountEvent( void )
{
	SetForceEvent();
	sys.pReturnProc = ScrEncountFightEnd;
	FightType = FIGHT_TYPE_1vs1 | FIGHT_TYPE_HAIKEI_SET | FIGHT_TYPE_SPECIAL;

	if (CasetteVersion == VERSION_RUBY)
		encount_task_set(EC_SCREENCUT, MUS_BATTLE34);
	else
		encount_task_set(EC_SWELL, MUS_BATTLE34);

	IncRecord( CNT_BATTLE );
	IncRecord( CNT_WILD_BATTLE );
}

//=========================================================================
//	�_���|�P�����Ƃ̐퓬�Ăяo��
//	���G���J�E���g�l�r�f���a�f�l���Ⴄ
//	���G���J�E���g�G�t�F�N�g�w�肠��
//=========================================================================
void StartTenjiPokeEncountEvent( void )
{
	SetForceEvent();
	sys.pReturnProc = ScrEncountFightEnd;
	FightType = FIGHT_TYPE_1vs1 | FIGHT_TYPE_TENJI | FIGHT_TYPE_SPECIAL;
	encount_task_set(EC_BLACKCOATING, MUS_BATTLE36);
	IncRecord( CNT_BATTLE );
	IncRecord( CNT_WILD_BATTLE );
}

//=========================================================================
//
//	�퓬��̕��A����
//
//=========================================================================

//------------------------------------------
//�쐶�G���J�E���g��̔��菈��
//------------------------------------------
static void EncountFightEnd(void)
{
	CpuClear(0,BG_PLTT,16*16*2,16);
	actOAMinit(0,128);

	if ( IsGameOverCheck(WinLoseFlag) == TRUE ) {
		MainProcChange( GameOverRestart);

	}else{
		MainProcChange( FieldMainRecover);
		pFieldRecoverFunc = AddFieldEncountEndFadeInTask;//OBJ�|�[�Y������
	};
}

//------------------------------------------
//	�X�N���v�g�ł̃|�P�����ΐ픻�菈��
//------------------------------------------
static void ScrEncountFightEnd(void)
{
	CpuClear(0,BG_PLTT,16*16*2,16);
	actOAMinit(0,128);

	if ( IsGameOverCheck(WinLoseFlag) == TRUE ) {
		MainProcChange( GameOverRestart );

	}else{
		MainProcChange( FieldScriptRecover );
	}
}



//=========================================================================
//	�퓬���̒n�`�^�C�v���擾����
//
//	<�߂�l>	�n�`�^�C�v�ifight.h�Œ�`)
//=========================================================================

u8 GetFightGroundType( void )
{
	s16 mx, my;
	u16 attr;

	GetHeroCellPos( &mx, &my );
	attr = GetCellAttribute( mx, my );
	if( ATR_ShortGrassCheck(attr) )return GROUND_KUSA;
	if( ATR_LongGrassCheck(attr) )return GROUND_LONGKUSA;
	if( ATR_IsSandCheck(attr) )return GROUND_SUNA;

	switch( NowMap.type ){
	case MAPTYPE_TOWN:			/* �� */
	case MAPTYPE_CITY:			/* �X */
	case MAPTYPE_ROAD:			/* ���H */
		break;

	case MAPTYPE_CAVE:			/* ���A */
		if (ATR_InDoorEncountCheck(attr)) return GROUND_ROOM;
		if (ATR_IsWaterCheck(attr)) return GROUND_IKE;
		return GROUND_DOUKUTSU;

	case MAPTYPE_BASE:			/* �閧��n */
		return GROUND_ROOM;

	case MAPTYPE_ROOM:			/* ���� */
		return GROUND_ROOM;

	case MAPTYPE_WATER:			/*  �����@*/
		return GROUND_MIZU;

	case MAPTYPE_SEA:			/* �C */
		if( ATR_IsWaterCheck(attr) ) return GROUND_UMI;
		return GROUND_ALL;

	case MAPTYPE_ICE:			/* �X */
		break;
	}

	if (ATR_IsSeaCheck(attr) ) return GROUND_UMI;
	if (ATR_IsWaterCheck(attr) ) return GROUND_IKE;
	if (ATR_IsRockCheck(attr) ) return GROUND_IWA;

	//�ȉ��͓��ꏈ��
	if (CheckHeroState(MY_MV_BIT_SWIM)) {
		if (ATR_GetBridgeCode(attr)) return GROUND_IKE;
		if (ATR_BridgeCheck(attr) == TRUE) return GROUND_UMI;
	}
	if (Fld.MapID.div_id == FIELD && Fld.MapID.map_id == FIELD_R113)
		return GROUND_SUNA;

	if (GetFieldWeatherValue() == WEATHER_STORM)
		return GROUND_SUNA;

	return GROUND_ALL;
}




//=========================================================================
//
//		�G���J�E���g�G�t�F�N�g�̎�ނ�����
//
//=========================================================================
enum{
	ENCPOS_FIELD = 0,
	ENCPOS_DUNGEON,
	ENCPOS_WIPE,
	ENCPOS_WATER,

	ENCLEVEL_LOW	= 0,
	ENCLEVEL_HIGH	= 1,
};

//------------------------------------------
//	�쐶�G���J�E���g�p�e�[�u��
//------------------------------------------
static const u8 wild_encount_table[4][2] =
{
	{		//FIELD
	EC_BGSHIFT,												// BG���		9
	EC_WHITELINE,											// �ܲ�ײ�		10
	},
	{		//DUNGEON
	EC_LINEWINDOW,											// ���v���		5
	EC_BLACKCOATING,										// ��ׯ���èݸ�	12
	},
	{		//DUNGEON(WIPE)
	EC_MOSAIC,												// ӻ޲�		11
	EC_BLACKCOATING,										// ��ׯ���èݸ�	12
	},
	{		//WATER
	EC_CURTAIN,												// ����			7
	EC_SWELL,												// �g���˂�		6
	},
};

//------------------------------------------
//	�g���[�i�[�G���J�E���g�p�e�[�u��
//------------------------------------------
static const u8 trainer_encount_table[4][2] =
{
	{		//FIELD
	EC_SPINBALL,											// ��]�ް�		4
	EC_SCREENCUT,											// ��ʐ؂�		13
	},
	{		//DUNGEON
	EC_VSIN,												// V��ݶ���		2
	EC_BALL,												// �ݽ���ް�	3
	},
	{		//DUNGEON(WIPE)
	EC_MOSAIC,												// ӻ޲�		11
	EC_BLACKCOATING,										// ��ׯ���èݸ�	12
	},
	{		//WATER
	EC_HSIN,												// H��ݶ���		1
	EC_SWELL,												// �g���˂�		6
	}
};

//------------------------------------------
//	�n�`�𔻕�
//------------------------------------------
static u8 get_encount_pos_type( void )
{
	s16 mx, my;
	u16 attr;

	GetHeroCellPos( &mx, &my );
	attr = GetCellAttribute( mx, my );

	if (GetBright() != 0) return ENCPOS_WIPE;

	if( ATR_IsWaterCheck(attr) ) return ENCPOS_WATER;

	switch( NowMap.type ){
	case MAPTYPE_CAVE:			/* ���A */
		return ENCPOS_DUNGEON;

	case MAPTYPE_WATER:			/* ���� */
		return ENCPOS_WATER;
	}

	return ENCPOS_FIELD;
}

//------------------------------------------
//	�v���C���[�莝���|�P�����̃��x���擾
//------------------------------------------
static u8 get_player_mons_level( u8 count )
{
	int i;
	u32 ret;
	u8 level;

	level = 0;
	for ( i=0; i<TEMOTI_POKEMAX; i++ ) {
		ret = PokeParaGet(&PokeParaMine[i],ID_monsno_egg);
		if ( ret == MONSNO_TAMAGO || ret == 0 ) continue;
		if ( PokeParaGet(&PokeParaMine[i],ID_hp) == 0 ) continue;
		level += PokeParaGet(&PokeParaMine[i],ID_level);
		count --;
		if ( count == 0 ) break;
	}
	return level;
}

//------------------------------------------
//	�g���[�i�[�莝���|�P�����̃��x���擾
//------------------------------------------
static u8 get_trainer_mons_level(u16 trainer_id, u8 count)
{
	const TrainerTypeNormalData * TTND;
	const TrainerTypeWazaData * TTWD;
	const TrainerTypeItemData * TTID;
	const TrainerTypeMultiData * TTMD;
	u8 level, i;
	u8 num;

	num = TD[trainer_id].monscount;
	if (num < count)
		count = num;

	level = 0;

	switch (TD[trainer_id].datatype) {
	case DATATYPE_NORMAL:
		TTND = (const TrainerTypeNormalData*)TD[trainer_id].TPD;
		for (i=0; i<count; i++)
			level += TTND[i].level;
		break;

	case DATATYPE_WAZA:
		TTWD = (const TrainerTypeWazaData*)TD[trainer_id].TPD;
		for (i=0; i<count; i++)
			level += TTWD[i].level;
		break;

	case DATATYPE_ITEM:
		TTID = (const TrainerTypeItemData*)TD[trainer_id].TPD;
		for (i=0; i<count; i++)
			level += TTID[i].level;
		break;

	case DATATYPE_MULTI:
		TTMD = (const TrainerTypeMultiData*)TD[trainer_id].TPD;
		for (i=0; i<count; i++)
			level += TTMD[i].level;
		break;
	}

	return level;
}

//------------------------------------------
//�쐶�G���J�E���g�G�t�F�N�g�̎�ނ��擾
//------------------------------------------
static u8 get_wild_effect( void )
{
	u8 enemy_level,my_level;
	u8 pos;

	pos = get_encount_pos_type();
	enemy_level = PokeParaGet(&PokeParaEnemy[0],ID_level);
	my_level = get_player_mons_level(1);

	if ( enemy_level < my_level ) {
		return wild_encount_table[pos][ENCLEVEL_LOW];
	} else {
		return wild_encount_table[pos][ENCLEVEL_HIGH];
	}
}

//------------------------------------------
//�g���[�i�[�G���J�E���g�̎�ނ��擾
//------------------------------------------
static u8 get_trainer_effect( void )
{
	u8 enemy_level,my_level;
	u8 pos,num;

//	if ( GetBright() != 0 ) return EC_MOSAIC;

	/* ����ȃg���[�i�[�֘A */
	if ( FieldTrainerID == ORUSUBAN_TRAINER ) {
		return EC_CUTIN;
	}
	else if ( TD[FieldTrainerID].type == TRTYPE_SITENNOU ) {
		/*�@�l�V���̏ꍇ */
		if (FieldTrainerID == BIG4M_01) return EC_PMLG_KAGETSU;
		if (FieldTrainerID == BIG4W_02) return EC_PMLG_FUYOU;
		if (FieldTrainerID == BIG4W_03) return EC_PMLG_PURIMU;
		if (FieldTrainerID == BIG4M_04) return EC_PMLG_GENJI;
		return EC_CUTIN;
	}
	else if ( TD[FieldTrainerID].type == TRTYPE_TYANPION ) {
		/* �`�����s�I���̏ꍇ */
		return EC_PMLG_CHAMP;
	}

	if ( TD[FieldTrainerID].fighttype == FIGHT_TYPE_2vs2 ) {
		num = 2;
	} else {
		num = 1;
	}

	pos = get_encount_pos_type();

	enemy_level = get_trainer_mons_level(FieldTrainerID, num);
	my_level = get_player_mons_level(num);

	if ( enemy_level < my_level ) {
		return trainer_encount_table[pos][ENCLEVEL_LOW];
	} else {
		return trainer_encount_table[pos][ENCLEVEL_HIGH];
	}
}

//------------------------------------------
//�G���J�E���g�G�t�F�N�g�^�C�v�擾
//���o�g���^���[�ȂǓ���g���[�i�[�p
//<�߂�l>	�G�t�F�N�g�w��iec_eff.h�Œ�`�j
//------------------------------------------
u8 GetSpTrainerEncountEffect(void)
{
	u8 enemy_level, my_level;

	enemy_level = PokeParaGet(&PokeParaEnemy[0], ID_level);
	my_level = get_player_mons_level(1);
	if (enemy_level < my_level) {
		return EC_SPINBALL;
	} else {
		return EC_BALL;
	}
}

//=========================================================================
//
//		�ŏ��̐퓬�@�C�x���g
//
//=========================================================================

extern u16	FirstPokemonNumGet(u16	index);
extern void OpPokeSelEvInit( void );


static void FirstBattleInit( void );
static void FirstBattleStart( void );
static void FirstBattleEnd( void );

/* �ŏ��̂R�̂̃|�P�����I���֑J�� */
void PokeSelEventStart( void )
{
	MainProcChange( OpPokeSelEvInit);
	sys.pReturnProc = FirstBattleInit;
}

/* �|�P�����I����A�G���J�E���g�G�t�F�N�g�� */
static void FirstBattleInit( void )
{
	u16 * save_work = GetEventWorkAdrs( WK_FIRST_POKE );
	*save_work = AnswerWork;
	TemotiPokeAdd( FirstPokemonNumGet(AnswerWork), 5, 0, NULL, NULL, 0 );
	InitTask();
//	MusicPlay(MUS_SUIKUN);
	FightBGMSet();
	MainProcChange( FirstBattleStart);
//	SetEcEffect( EC_MOSAIC );
	SetEcEffect1stBattle( EC_MOSAIC );
}

/* �G���J�E���g�҂����퓬�� */
static void FirstBattleStart( void )
{
	PaletteAnime();
	MainTask();
	if ( CheckEcEffect() == TRUE ) {
		FightType = FIGHT_TYPE_1vs1 | FIGHT_TYPE_GUIDE;
		sys.pReturnProc = FirstBattleEnd;			//���A�惁�C���V�[�P���X
		MainProcChange(InitFight);					//�퓬�V�[�P���X�ɑJ�ځI
		InitEncountStep();				//�G���J�E���g�̕�������������
		PoisonStepInit();				//�Ń_���[�W����̕���������
		IncRecord( CNT_BATTLE );
		IncRecord( CNT_WILD_BATTLE );
	}
}

/* �퓬���畜�A�A�X�N���v�g�ɐ����߂� */
static void FirstBattleEnd( void )
{
	MapBGM_ClearSpecial();
	MainProcChange( FieldScriptRecover);
}



