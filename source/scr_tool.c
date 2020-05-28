//====================================================================
//
//						�X�N���v�g�֘A		02/02/27 nohara							
//
//====================================================================
#include "common.h"
#include "intr.h"
#include "script.h"					//ScriptParameter
#include "pokesio.h"				//GetSioConnectID
#include "calctool.h"				//pp_rand
#include "poketool.h"				//PokeNameData
#include "print.h"					//PM_strcpy
#include "jiki.h"					//GetHeroSexObjCode
#include "fieldmap.h"				//NowMap
#include "fld_talk.h"				//FieldMsgStatusCheck
#include "fld_main.h"				//FieldScriptRecover
#include "menu.h"					//menutool.h
#include "task.h"					//AddTask
#include "itemtool.h"				//AddScrCurActor	2002/07/01 by nakahiro
#include "mapbg.h"					//MakeMapBG
#include "mus_tool.h"				//SePlay
#include "server.h"					//WinLoseFlag
#include "fld_tool.h"				//NextMapSetDirect
#include "../evdata/maparray.h"		//�ڑ��p�}�b�vID��`
#include "sxytype.h"				//NO_EXIT_ID
#include "syswork.h"
#include "sysflag.h"
#include "mapparam.h"
#include "scr_tool.h"				//MSG_1F
#include "..\script\savework.h"		//WK_SCENE_CYCLE_ROAD
#include "zukan.h"					//ZukanTotal
#include "rtctool.h"				//GameTime
#include "..\msgdata\hyouka.h"		//hyouka.mes
#include "tv_def.h"					//TV��define�Q
#include "tv_tool.h"				//GetNumKeta
#include "..\script\saveflag.h"		//�B���A�C�e�����擾�p
#include "..\sxydata\hideitem.h"	//�B���A�C�e�����擾�p
#include "fld_task.h"				//AddScriptFieldInTask
#include "ev_win.h"					//AddScriptPasoMenu
#include "palanm.h"					//FadeData
#include "record.h"
#include "../data/warpsym.h"
#include "saveload.h"
#include "cubecase.h"				//GetLikeCubeName
#include "b_tower.h"				//BattleCardENameGet
#include "weather.h"				//CheckWeatherChange
#include "wea_ctrl.h"				//ID_WEATHER_SRTR
#include "fight.h"				
#include "monsno.def"				//MONSNO_ZIGUZAGUMA
#include "wazano.def"				//WAZANO_TAIATARI
#include "pasocom.h"				//BoxSpacePokemonCheck
#include "message.h"
#include "..\sxydata\park_battle01.sym"//WORKERM1_01_PARK_BATTLE01
#include "cm.h"						//CMEventStartChk
#include "pokescr.h"				//PokeAllKaifuku
#include "evobj.def"				//MY_MV_BIT_CYCLE_S


//====================================================================
//
//
//	�Q�[���N���A���a������
//
//
//====================================================================
extern void DendouInit( void );
extern u8 GameClearFlag;

u8 ScrGameClear( void )
{
	int i,flag;

	/* �|�P�����̃X�e�[�^�X���� */
	PokeAllKaifuku();

	if (EventFlagCheck(SYS_GAME_CLEAR) == TRUE) {
		//�Q��ڈȍ~�A�G���f�B���O������\
		GameClearFlag = 1;
	} else {
		//����A�G���f�B���O������ł��Ȃ�
		GameClearFlag = 0;
		EventFlagSet( SYS_GAME_CLEAR );
	}

	/* ����N���A���̃v���C���Ԃ��L�^ */
	if ( GetRecord( SV_TOTAL_TIME ) == 0 ) {
		u32 clear_time;
		clear_time = ((u32)MyData.playtime_h << 16)
			| (MyData.playtime_m << 8 )
			| (MyData.playtime_s );
		SetRecord( SV_TOTAL_TIME, clear_time );
	}

	/* �N���A��̏o���ʒu���� */
	SetContinueStatus();
	if ( MyData.my_sex == MALE ) {
		SetContinueMapByWarpID( WP_BOY_START );
	} else {
		SetContinueMapByWarpID( WP_GIRL_START );
	}

	/* �N���A���{�������� */
	flag = FALSE;
	for (i=0;i<TEMOTI_POKEMAX;i++) {
		PokemonParam * pp = &PokeParaMine[i];
		if ( PokeParaGet(pp,ID_poke_exist_flag) != 0 
				&& PokeParaGet(pp,ID_egg_exist_flag) == 0
				&& PokeParaGet(pp,ID_champ_ribbon) == 0 )
		{
			u8 set_value[1] = {1};
			PokeParaPut(pp, ID_champ_ribbon, set_value );
			flag = TRUE;
		}
	}
	if ( flag == TRUE ) {
		IncRecord( CNT_RIBBON );			//���{������������񐔂��J�E���g
		EventFlagSet( SYS_RIBBON_GET );		//���{������������t���O���Z�b�g
	}

	MainProcChange( DendouInit );
	return 0;
}


//====================================================================
//
//	�Q�[���I�[�o�[����
//
//====================================================================
u8 GameOver( void )
{
	MainProcChange( GameOverRestart );
	return 0;
}


//====================================================================
//
//	�}�{���V�W�}�̏o���`�F�b�N
//
//====================================================================
u32 get_maboroshi_value( void )
{
	u16 rnd_h,rnd_l;
	rnd_h = GetEventWorkValue( WK_MABOROSI_RND_H );
	rnd_l = GetEventWorkValue( WK_MABOROSI_RND_L );
	return (rnd_h<<16) | rnd_l;
}

void set_maboroshi_value( u32 value )
{
	SetEventWorkValue( WK_MABOROSI_RND_H, (value>>16)&0xffff );
	SetEventWorkValue( WK_MABOROSI_RND_L, value&0xffff );
}

//-------------------------------------------------------------------------
//	�}�{���V�W�}�̔�������������
//-------------------------------------------------------------------------
void MaboroshiCycle_Init( void )
{
	u32 value;
	value = (pp_rand() << 16) | pp_rand();
	set_maboroshi_value( value );
}

//-------------------------------------------------------------------------
//	�}�{���V�W�}�̔��������X�V
//-------------------------------------------------------------------------
void MaboroshiCycle_Update( u16 diff )
{
	u32 value;
	value = get_maboroshi_value();
	while ( diff > 0 ) {
		value = value * 1103515245L + 12345;
		diff --;
	}
	set_maboroshi_value( value );
}

//--------------------------------------------------------------------
//	�X�N���v�g����A�}�b�v�J�ڎ��̂݌Ă΂��
//	�}�{���V�W�}�̏o�������J�E���^���O�ŁA���ړ����Ă����}�b�v��
//	�t�B�[���h�}�b�v�̏ꍇ�̂�TRUE��Ԃ�
//--------------------------------------------------------------------
u8 Maboroshi_Check( void )
{
	int i;
	u16 rnd;

	rnd = (get_maboroshi_value() >> 16) & 0xffff;
	for ( i=0; i<TEMOTI_POKEMAX; i++ ) {
		if ( PokeParaGet(&PokeParaMine[i], ID_monsno) != 0
				&& (PokeParaGet(&PokeParaMine[i], ID_personal_rnd) & 0xffff) == rnd ) {
			return TRUE;
		}
	}
	return FALSE;
}


//====================================================================
//
//	�󐣂̓��A�̖�����������
//
//====================================================================
static const u8 asase_table[24] =
{
	1,1,1,			// 00:00 - 02:59 ����
	0,0,0,0,0,0,	// 03:00 - 08:59 ����
	1,1,1,1,1,1,	// 09:00 - 14:59 ����
	0,0,0,0,0,0,	// 15:00 - 20:59 ����
	1,1,1,			// 21:00 - 23:59 ����
};

//--------------------------------------------------------------------
//	�󐣂̓��A�̖��������ϓ��`�F�b�N
//--------------------------------------------------------------------
void AsaseTideCheck( void )
{
	/* �t�B�[���h����ړ����Ă����ꍇ�̂݃`�F�b�N */
	if ( MTYPE_IsFieldCheck(GetBeforeMapType()) == FALSE ) return;

	GetGameTime();
	if ( asase_table[GameTime.hour] ) {
		EventFlagSet( SYS_ASASE_TIDE );		//����
	} else {
		EventFlagReset( SYS_ASASE_TIDE );	//����
	}
}


//====================================================================
//
//	�V��ω��I���҂�
//
//====================================================================
static void weather_change_wait_task( u8 no )
{
	if ( CheckWeatherChange() ) {
		ContinueFieldScript();
		DelTask(no);
	}
}

void ScrWeatherChangeWait( void )
{
	AddTask( weather_change_wait_task, 80 );
}


//====================================================================
//
//						�}�ӕ]���V�X�e��
//
//====================================================================
void ZukanHyouka_Init(void);
void ZukanHyouka_Update(u16);

//--------------------------------------------------------------------
//							������
//--------------------------------------------------------------------
void ZukanHyouka_Init( void )
{
	u16 * count = GetEventWorkAdrs( WK_HYOUKA_DAYS );
	*count = 0;
}

//--------------------------------------------------------------------
//							�����X�V
//--------------------------------------------------------------------
void ZukanHyouka_Update( u16 diff )
{
	u16 * count = GetEventWorkAdrs( WK_HYOUKA_DAYS );
	*count += diff;
	*count %= 7;
}

//--------------------------------------------------------------------
//					���������E�߂܂������擾
//
//	ScriptParameter0�Ƀz�E�G���}��(0)���S���}��(1)
//	�ǂ���̒l���擾���邩�����Ă���
//
//  ScriptParameter1�Ɍ�����������������
//  ScriptParameter2�ɕ߂܂���������������
//
//	�߂�l	���݃z�E�G���}�ӂ��S���}�ӂ��H
//--------------------------------------------------------------------
u16 ScrZukanNumGet()
{
	if( ScriptParameter0 == 0 )
	{
		//�z�E�G���}��
		ScriptParameter1 = ZukanHouenTotal( Z_SEE_CHK );
		ScriptParameter2 = ZukanHouenTotal( Z_GET_CHK );
	}else
	{
		//�S���}��
		ScriptParameter1 = ZukanTotal( Z_SEE_CHK );
		ScriptParameter2 = ZukanTotal( Z_GET_CHK );
	}

	return (u16)ZukanSpecialFlagCheck();
}

//--------------------------------------------------------------------
//					�]�����b�Z�[�W�̃|�C���^�擾
//--------------------------------------------------------------------
const u8 * const GetZukanHyoukaMsg( u16 num )
{
	if( num <= 9 )	return msg_odamaki_01_p03_zukan;
	if( num <= 19 )	return msg_odamaki_01_p04_zukan;
	if( num <= 29 )	return msg_odamaki_01_p05_zukan;
	if( num <= 39 )	return msg_odamaki_01_p06_zukan;
	if( num <= 49 )	return msg_odamaki_01_p07_zukan;
	if( num <= 59 )	return msg_odamaki_01_p08_zukan;
	if( num <= 69 )	return msg_odamaki_01_p09_zukan;
	if( num <= 79 )	return msg_odamaki_01_p10_zukan;
	if( num <= 89 )	return msg_odamaki_01_p11_zukan;
	if( num <= 99 )	return msg_odamaki_01_p12_zukan;
	if( num <= 109 )	return msg_odamaki_01_p13_zukan;
	if( num <= 119 )	return msg_odamaki_01_p14_zukan;
	if( num <= 129 )	return msg_odamaki_01_p15_zukan;
	if( num <= 139 )	return msg_odamaki_01_p16_zukan;
	if( num <= 149 )	return msg_odamaki_01_p17_zukan;
	if( num <= 159 )	return msg_odamaki_01_p18_zukan;
	if( num <= 169 )	return msg_odamaki_01_p19_zukan;
	if( num <= 179 )	return msg_odamaki_01_p21_zukan;
	if( num <= 189 )	return msg_odamaki_01_p22_zukan;
	if( num <= 199 )	return msg_odamaki_01_p23_zukan;

	//�B���|�P�������܂܂�Ă��邩�̃`�F�b�N
	if( num == 200 )
	{
		//�|�P�����i���o�[��S���ɂ���
		if( ZukanCheck( PokeMonsNo2ZukanNoGet(MONSNO_ZIRAATI), Z_GET_CHK ) ||
				ZukanCheck( PokeMonsNo2ZukanNoGet(MONSNO_DEOKISISU), Z_GET_CHK ) )
		{
			//200�C�ɉB���|�P�������܂܂��̂Ŋ������ĂȂ��I
			return msg_odamaki_01_p23_zukan;
		}else
		{
			return msg_odamaki_01_p24_zukan;
		}
	}

	//�B���|�P�������܂܂�Ă��邩�̃`�F�b�N
	if( num == 201 )
	{
		//�|�P�����i���o�[��S���ɂ���
		if( ZukanCheck( PokeMonsNo2ZukanNoGet(MONSNO_ZIRAATI), Z_GET_CHK ) &&
				ZukanCheck( PokeMonsNo2ZukanNoGet(MONSNO_DEOKISISU), Z_GET_CHK ) )
		{
			//�c��̈�C���B���|�P�����ł͂Ȃ��̂Ŋ������ĂȂ��I
			return msg_odamaki_01_p23_zukan;
		}else
		{
			return msg_odamaki_01_p24_zukan;
		}
	}

	//�B���|�P�������܂ߊ������Ă���I
	if( num == 202 )	return msg_odamaki_01_p24_zukan;

	//����ȊO
	return msg_odamaki_01_p03_zukan;
}

//--------------------------------------------------------------------
//					�]�����b�Z�[�W�\��
//--------------------------------------------------------------------
void SetZukanHyoukaMsg()
{
	FieldMsgSet( GetZukanHyoukaMsg(ScriptParameter0) );
	return;
}

//====================================================================
//				�ł�ǂ�����̌Ăяo�������E���A����
//====================================================================
void ScriptDendouRecover();				//dendou.c����Ă΂��
void ScriptDendouRecover2();
extern void	DendouLookInit(void);
static void script_dendou_fade_watch_task(u8);

//�Ăяo������
void ScrDendouLookInit(void)
{
	MainProcChange(DendouLookInit);
	SetForceEvent();
}

//���A����
void ScriptDendouRecover()
{
	MainProcChange( FieldMainRecover );
	pFieldRecoverFunc = ScriptDendouRecover2;
	return;
}

void ScriptDendouRecover2()
{
	SetForceEvent();
	MapBGM_Start();

	//FieldFadeControl(FIELD_BLACK_IN, 0);
	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );

	//�p�\�R�����j���[�\��
	AddScriptPasoMenu();
	PasoMenuWriteInit2();

	AddTask( script_dendou_fade_watch_task, TSK_PRI_CONTROL );
	return;
}

//-------------------------------------------------------------
//	�X�N���v�g���쒆�ւ̕��A����(�t�F�[�h�҂�)
//-------------------------------------------------------------
static void script_dendou_fade_watch_task( u8 my_no )
{
	if( FadeData.fade_sw == 0 ){
		DelTask(my_no);
	};
}


//====================================================================
//				�\����̌Ăяo�������E���A����
//====================================================================
extern void	HyousyouEvInit(void);

//�Ăяo������
void ScrHyousyouInit(void)
{
	MainProcChange(HyousyouEvInit);
	SetForceEvent();
}


//====================================================================
//				���v���(���Ԋm�F�̂�)�̌Ăяo������
//====================================================================
extern void	TokeiLookInit(void);

//�Ăяo������
void ScrTokeiLookInit(void)
{
	sys.pReturnProc = FieldMainRecover;
	MainProcChange(TokeiLookInit);
	SetForceEvent();
}


//====================================================================
//
//		�T�C�N�����O���[�h�ł̃~�j�Q�[��
//
//====================================================================
void CyclingRoadEventStart( void );
u8 CyclingRoadGateChk( void );
void CyclingRoadGoal( void );
void CyclingRoadHitTimeSet(u32,u8);

u8 bicycle_collision_flag = 0;
u8 bicycle_collision_count = 0;
u32 bicycle_start_time = 0;

//--------------------------------------------------------------------
//	�p�����[�^������
//
//�@�אڃ}�b�v�ړ����E�}�b�v�J�ڎ��E�R���e�B�j���[���ɌĂ΂��
//--------------------------------------------------------------------
void CyclingRoadEventInit( void )
{
	bicycle_collision_flag = 0;
	bicycle_collision_count = 0;
	bicycle_start_time = 0;
}

//--------------------------------------------------------------------
//	�T�C�N�����O���[�h�C�x���g�v���J�n
//--------------------------------------------------------------------
void CyclingRoadEventStart( void )
{
	bicycle_collision_flag = 1;
	bicycle_collision_count = 0;
	bicycle_start_time = sys.main_timer;
}

//--------------------------------------------------------------------
//	�Q�[�g�`�F�b�N
//
//	�߂�l	0 = ���]�Ԃɏ���Ă��Ȃ�
//			1 = �A�N�V�����^�C�v�ɏ���Ă���
//			2 = �X�s�[�h�^�C�v�ɏ���Ă���
//--------------------------------------------------------------------
u8 CyclingRoadGateChk( void )
{
	// ���]��S�ER�̏��
	if( CheckHeroState(MY_MV_BIT_CYCLE_S) )
	{
		//���������A�N�V����
		return 1;
	}else if( CheckHeroState(MY_MV_BIT_CYCLE_R) )
	{
		//���������X�s�[�h
		return 2;
	}

	return 0;
}

//--------------------------------------------------------------------
//	�S�[��
//	StrTempBuffer0	"�`����" or "�X�X�����@�����傤�I"
//	StrTempBuffer1	"�`�т傤" or "�P�Ղ�@�����傤!"
//	AnswerWork		���_�i�O�`�P�O�j
//--------------------------------------------------------------------
void set_cycling_score( u32 time, u8 hit )
{
	u8 score;

	//�Փˉ񐔂��Z�b�g����
	if ( hit < BICYCLE_COLLISION_MAX ) {
		PM_NumMsgSet( StrTempBuffer0, hit, NUM_MODE_LEFT, 2 );
		PM_strcat( StrTempBuffer0, str_kai );
	} else {
		PM_strcpy( StrTempBuffer0, str_countover );
	}
	
	//�^�C�����Z�b�g����
	if ( time < 60 * 60 ) {
		PM_NumMsgSet( StrTempBuffer1, time/60, NUM_MODE_SPACE, 2 );
		StrTempBuffer1[2] = ten_;
		PM_NumMsgSet( StrTempBuffer1+3, (time%60)*100/60, NUM_MODE_ZERO, 2 );
		PM_strcat( StrTempBuffer1, str_byou );
	} else {
		PM_strcpy( StrTempBuffer1, str_timeover );
	}

	//���_�v�Z
	score = 0;
	if ( hit == 0 ) {
		score += 5;
	} else if ( hit <= 3 ) {
		score += 4;
	} else if ( hit <= 9 ) {
		score += 3;
	} else if ( hit <= 19 ) {
		score += 2;
	} else if ( hit <= 99 ) {
		score += 1;
	}

	if ( time / 60 <= 10 ) {
		score += 5;
	} else if ( time / 60 <= 15 ) {
		score += 4;
	} else if ( time / 60 <= 20 ) {
		score += 3;
	} else if ( time / 60 <= 40 ) {
		score += 2;
	} else if ( time / 60 <= 59 ) {
		score += 1;
	}

	AnswerWork = score;
}


#if 0

// �ǂ������̃t�@�C���ŉ��L�̕ϐ����`����B
// �f�o�b�K��ł��������������ƃf�o�b�O���y�`

extern const u32 debug_cycling_time;
extern const u8 debug_cycling_hit;

void DebugCyclingRoadGoal( void )
{
	set_cycling_score( debug_cycling_time, debug_cycling_hit );
	PM_NumMsgSet( StrTempBuffer2, AnswerWork, NUM_MODE_LEFT, 2 );
}

#endif

void CyclingRoadGoal( void )
{
	u32 diff_time;

	diff_time = sys.main_timer - bicycle_start_time;
	set_cycling_score( diff_time, bicycle_collision_count );

	//�L�^��ۑ�����
	CyclingRoadHitTimeSet( diff_time, bicycle_collision_count );
}

//�L�^��ۑ�����
void CyclingRoadHitTimeSet( u32 time, u8 hit )
{
	u16 ret,ret2;
	u32 hozon_time;

	ret  = GetEventWorkValue( WK_CYCLE_ROAD_TIME1 );
	ret2 = GetEventWorkValue( WK_CYCLE_ROAD_TIME2 );
	hozon_time = (ret & 0xffff) + ((ret2 & 0xffff) << 16);

	//hozon_time = 0��������L�^�Ȃ��Ƃ������ƂɂȂ�I
	if( hozon_time > time || hozon_time == 0 )
	{
		SetEventWorkValue( WK_CYCLE_ROAD_TIME1, (time & 0xffff) );
		SetEventWorkValue( WK_CYCLE_ROAD_TIME2, ((time >> 16) & 0xffff) );
		SetEventWorkValue( WK_CYCLE_ROAD_HIT, hit );
	};

	return;
}

//--------------------------------------------------------------------
//				�T�C�N�����O���[�h�̋L�^���Z�b�g����
//
//	�߂�l	FALSE = �L�^�Ȃ�
//			TRUE  = �L�^����
//--------------------------------------------------------------------
u8 CyclingRoadRecordSet( void )
{
	u16 ret,ret2;
	u32 hozon_time;

	ret  = GetEventWorkValue( WK_CYCLE_ROAD_TIME1 );
	ret2 = GetEventWorkValue( WK_CYCLE_ROAD_TIME2 );
	hozon_time = (ret & 0xffff) + ((ret2 & 0xffff) << 16);

	if( hozon_time == 0 )	return FALSE;		//�L�^�Ȃ�

	ret  = GetEventWorkValue( WK_CYCLE_ROAD_HIT );
	set_cycling_score( hozon_time, (u8)ret );

	return TRUE;
}

//--------------------------------------------------------------------
//	�O�̃}�b�v���Q�[�g(��)�łȂ�������V�[����߂�
//--------------------------------------------------------------------
void CyclingRoadBeforeMapChk( void )
{
	if( BeforeMapID.div_id == R110 &&
			BeforeMapID.map_id == R110_R0301 )	return;

	//�T�C�N�����O���[�h�v�����Ƀ��Z�b�g������
	if( GetEventWorkValue( WK_SCENE_CYCLE_ROAD ) == 2 || 
			GetEventWorkValue( WK_SCENE_CYCLE_ROAD ) == 3 )
	{
		SetEventWorkValue( WK_SCENE_CYCLE_ROAD, 0 );
		MapBGM_SetSpecial( 0 );		//BGM�����]�Ԃł͂Ȃ�����
	}

	return;
}


//====================================================================
//
//	�A�N�A���D���̕����J�E���g�֘A
//
//====================================================================

//--------------------------------------------------------------------
//	�A�N�A�����̕����J�E���g�J�n
//--------------------------------------------------------------------
void ShipStepCountStart( void )
{
	u16 * count;

	EventFlagSet( SYS_CRUISE_MODE );
	count = GetEventWorkAdrs( WK_CRUISE_COUNT );
	*count = 0;
}

//--------------------------------------------------------------------
//	�A�N�A�����̕����J�E���g�I��
//--------------------------------------------------------------------
void ShipStepCountEnd( void )
{
	EventFlagReset( SYS_CRUISE_MODE );
}

//--------------------------------------------------------------------
//	�A�N�A�����̕����J�E���g�A�b�v
//--------------------------------------------------------------------
int ShipStepCountCheck( u16 value )
{
	u16 * count;

	if ( EventFlagCheck( SYS_CRUISE_MODE ) == FALSE ) return FALSE;

	count = GetEventWorkAdrs( WK_CRUISE_COUNT );
	*count += value;
	if ( *count >= SHIPMOVE_MAX ) return TRUE;

	return FALSE;
}

//--------------------------------------------------------------------
//	�����D�̒n�}��̈ʒu�Z�o
//	<����>	�f�����̈ʒu��Ԃ����߂̃��[�N
//			�i�߂�l��SHIPPOS_DEMO�ȊO�̎��ɂ͒l�͓���Ȃ��j
//			div_id,map_id		�}�b�v�w��ID
//			xpos,ypos			���W
//	<�߂�l>
//			�w�b�_��MAPPOS_�`���Q��
//--------------------------------------------------------------------
u8 GetShipWorldPos(u8 * div_id, u8 * map_id, s16 * xpos, s16 * ypos )
{
	u16 * count = GetEventWorkAdrs( WK_CRUISE_COUNT );
	u16 * scene = GetEventWorkAdrs( WK_SCENE_SP_SHIP01 );

	switch ( *scene ) {
	case SHIPSCENE_TAKEOFF_KAINA:		//	1	�J�C�i����A�N�A����D����
	case SHIPSCENE_ARRIVE_KAINA:		//	8	�J�C�i������
		return SHIPPOS_KAINA_STOP;

	case SHIPSCENE_FREE_KAINA:			//	3	�f���I����A�t���[���
	case SHIPSCENE_END_DEMO_RIGHT:		//	9	�f���I���i�J�C�i���L�i�M�j
		return SHIPPOS_R131;

	case SHIPSCENE_ARRIVE_MINAMO:		//	4	�~�i��������
	case SHIPSCENE_TAKEOFF_MINAMO:		//	5	�~�i������A�N�A����D����
		return SHIPPOS_MINAMO_STOP;

	case SHIPSCENE_FREE_MINAMO:			//	6	�t���[���
	case SHIPSCENE_END_DEMO_LEFT:		//	10	�f���I���i�L�i�M���J�C�i�j
		return SHIPPOS_R124;

	case SHIPSCENE_DEMO_RIGHT:			//	2	�f�����i�J�C�i���L�i�M�j
		if ( *count < SHIPMOVE_R134 ) {
			*map_id = FIELD_R134;
			*xpos = 19 + *count;
		}
		else if ( *count < SHIPMOVE_R134+SHIPMOVE_R133 ) {
			*map_id = FIELD_R133;
			*xpos = *count - SHIPMOVE_R134;
		}
		else {
			*map_id = FIELD_R132;
			*xpos = *count - SHIPMOVE_R134 - SHIPMOVE_R133;
		}
		break;

	case SHIPSCENE_DEMO_LEFT:			//	7	�f�����i�L�i�M���J�C�i�j

		if ( *count <= SHIPMOVE_R132 ) {
			*map_id = FIELD_R132;
			*xpos = SHIPMOVE_R132 - *count;
		}
		else if ( *count <= SHIPMOVE_R132+SHIPMOVE_R133 ) {
			*map_id = FIELD_R133;
			*xpos = SHIPMOVE_R132 + SHIPMOVE_R133 - *count;
		}
		else {
			*map_id = FIELD_R134;
			*xpos = 19 + SHIPMOVE_MAX - *count;
		}
		break;
	}

	*div_id = FIELD;
	*ypos = 20;

	return SHIPPOS_DEMO;
}


//====================================================================
//
//						���̂݃u�����_�[
//
//====================================================================
extern SIOTRAINER_INFO SioTrainerInfo[4];
extern u8 * const tempbuf_list[];
u8 SetBlendCommName();
void SetBlendCommOBJ();

//--------------------------------------------------------------------
//				�ʐM�����Ď擾�������O���Z�b�g����
//
//	�߂�l	�ʐM�l��
//--------------------------------------------------------------------
u8 SetBlendCommName()
{
	u8 my_id,i,num,j=0;

	my_id = GetSioReliableId();			//������ID
	num   = GetSioReliableNumber();	//�ʐM�l��

	for( i=0; i < num ;i++ )
	{
		if( my_id != i )
		{
			PM_strcpy( tempbuf_list[j], 
				SioTrainerInfo[i].name );

			j++;	//tempbuf_list�̗v�f�������炷
		}

	}

	return num;
}

//--------------------------------------------------------------------
//			�ʐM�l�����^�l�u�����_�[�̎����OBJ��z�u����
//--------------------------------------------------------------------
void SetBlendCommOBJ()
{
	u8 i,code,my_id,site,count=0;
	s16 xpos=0,ypos=0;
	u8 site_buf[4] = { MV_UP, MV_LEFT, MV_DOWN, MV_RIGHT };
	s8 hosei[4][2] = {
		{0,1},		
		{1,0},		
		{0,-1},		
		{-1,0},		
	};

	my_id = GetSioReliableId();			//������ID
	site  = GetHeroSite();


	switch( site )
	{
		case SITE_L:
			count = 2;
			xpos = Fld.MapXpos-1;
			ypos = Fld.MapYpos;
			break;
		case SITE_U:
			count = 1;
			xpos = Fld.MapXpos;
			ypos = Fld.MapYpos-1;
			break;
		case SITE_R:
			count = 0;
			xpos = Fld.MapXpos+1;
			ypos = Fld.MapYpos;
			break;
		case SITE_D:
			count = 3;
			xpos = Fld.MapXpos;
			ypos = Fld.MapYpos+1;
			break;
	}

	for( i=0; i < ScriptParameter0 ;i++ )
	{
		if( my_id != i )
		{
			code = GetHeroSexObjCode( H_NORMAL, SioTrainerInfo[i].sex );
			EvObjMakeSxyAdd( code,site_buf[count],(0xf0-i),
					xpos+hosei[count][0]+7,
					ypos+hosei[count][1]+7,0 );

			count++;
			if( count == 4 ) count = 0;
		}
	}

	return;
}


//====================================================================
//
//							�W���g���b�v
//
//====================================================================

//--------------------------------------------------------------------
//			�L���Z�c�W���̃X�C�b�`�̃Z���R�[�h��ύX����
//--------------------------------------------------------------------
#define GYM03_START_X	(7)				//�T�[�`����X�^�[�g�̂w���W
#define GYM03_END_X		(16)			//�T�[�`����I���̂w���W
#define GYM03_START_Y	(12)			//�T�[�`����X�^�[�g�̂x���W
#define GYM03_END_Y		(24)			//�T�[�`����I���̂x���W

#define GYM03_SWITCH_MAX 3				//�L���Z�c�W���̃X�C�b�`�̐�
#define SWITCH_OFF_CELL_CODE	517
#define SWITCH_ON_CELL_CODE		518

typedef struct{
		u8	x;
		u8	y;
}Cell_Pos;

//X����	7	Y���� 7	�����Ă���
const Cell_Pos gym03_switch[GYM03_SWITCH_MAX] = {
	{7,16},{15,18},{11,22},
};

void ScrGYM03SwitchChangeCell();
void ScrGYM03SwitchChangeCell()
{
	u8 i;

	for( i=0; i < GYM03_SWITCH_MAX ;i++ )
	{
		if( i == ScriptParameter0 ) //����ł���
			SetCell((int)gym03_switch[i].x, (int)gym03_switch[i].y, SWITCH_ON_CELL_CODE);
		else						//����łȂ�
			SetCell((int)gym03_switch[i].x, (int)gym03_switch[i].y, SWITCH_OFF_CELL_CODE);
	}

	return;
}

//--------------------------------------------------------------------
//				�L���Z�c�W���̃h�A�̃Z���R�[�h��ύX����
//--------------------------------------------------------------------
//open = �󂢂Ă�	close = ���܂��Ă���
//U = ��̃Z��		D = ���̃Z��
//L = ���̔�		R = �E�̔�

//���̃Z���R�[�h
#define DOOR_FLOOR_CELL				(538)

//�܂��Ă���h�A��(�h�A�P)
#define	DOOR1_CLOSE_U_L_CELL		(544)
#define	DOOR1_CLOSE_U_R_CELL		(545)
#define	DOOR1_CLOSE_D_L_CELL		(552)
#define	DOOR1_CLOSE_D_R_CELL		(553)

//�܂��Ă���h�A��(�h�A�Q)
#define	DOOR2_CLOSE_U_L_CELL		(546)
#define	DOOR2_CLOSE_U_R_CELL		(547)
#define	DOOR2_CLOSE_D_L_CELL		(554)
#define	DOOR2_CLOSE_D_R_CELL		(555)

//�J���Ă���h�A��(�h�A�P)
#define	DOOR1_OPEN_U_L_CELL			(560)
#define	DOOR1_OPEN_U_R_CELL			(561)
#define	DOOR1_OPEN_D_L_CELL			(568)
#define	DOOR1_OPEN_D_R_CELL			(569)

//�J���Ă���h�A��(�h�A�Q)
#define	DOOR2_OPEN_U_L_CELL			(562)
#define	DOOR2_OPEN_U_R_CELL			(563)
#define	DOOR2_OPEN_D_L_CELL			(570)
#define	DOOR2_OPEN_D_R_CELL			(571)

//�܂��Ă���h�A�c(�h�A�R)
#define	DOOR3_CLOSE_U_CELL			(576)
#define	DOOR3_CLOSE_D_CELL			(584)

//�܂��Ă���h�A�c(�h�A�S)
#define	DOOR4_CLOSE_U_CELL			(577)
#define	DOOR4_CLOSE_D_CELL			(585)

//�J���Ă���h�A�c(�h�A�R)
#define	DOOR3_OPEN_U_CELL			(578)
#define	DOOR3_OPEN_D_CELL			(DOOR_FLOOR_CELL)

//�J���Ă���h�A�c(�h�A�S)
#define	DOOR4_OPEN_U_CELL			(579)
#define	DOOR4_OPEN_D_CELL			(DOOR_FLOOR_CELL)

//�d�C�}�V���̏㑤(�����蔻��Ȃ�)
#define DOOR5_CELL					(593)

//�d�C�}�V���̏㑤(�����蔻�肠��)
#define DOOR6_CELL					(592)

void ScrGYM03DoorChangeCell();
void ScrGYM03DoorChangeCell()
{
	int i,j;

	//for( i=0; i < VMap.Ysize ;i++ )
	for( i=GYM03_START_Y; i < GYM03_END_Y ;i++ )
	{
		//for( j=0; j < VMap.Xsize ;j++ )
		for( j=GYM03_START_X; j < GYM03_END_X ;j++ )
		{
			switch( GetCellCode(j,i) )
			{
				//�P--------------------------------------------------------------
				//���h�A���J����
				case DOOR1_CLOSE_U_L_CELL:
					SetCell(j,i,DOOR1_OPEN_U_L_CELL);
					break;
				case DOOR1_CLOSE_U_R_CELL:
					SetCell(j,i,DOOR1_OPEN_U_R_CELL);
					break;
				case DOOR1_CLOSE_D_L_CELL:
					SetCell(j,i,DOOR1_OPEN_D_L_CELL);
					break;
				case DOOR1_CLOSE_D_R_CELL:
					SetCell(j,i,DOOR1_OPEN_D_R_CELL);
					break;

				//���h�A��߂�
				case DOOR1_OPEN_U_L_CELL:
					SetCell(j,i,DOOR1_CLOSE_U_L_CELL);
					break;
				case DOOR1_OPEN_U_R_CELL:
					SetCell(j,i,DOOR1_CLOSE_U_R_CELL);
					break;
				case DOOR1_OPEN_D_L_CELL:
					SetCell(j,i,DOOR1_CLOSE_D_L_CELL | CELL_HITCHECK_MASK);
					break;
				case DOOR1_OPEN_D_R_CELL:
					SetCell(j,i,DOOR1_CLOSE_D_R_CELL | CELL_HITCHECK_MASK);
					break;

				//�Q--------------------------------------------------------------
				//���h�A���J����
				case DOOR2_CLOSE_U_L_CELL:
					SetCell(j,i,DOOR2_OPEN_U_L_CELL);
					break;
				case DOOR2_CLOSE_U_R_CELL:
					SetCell(j,i,DOOR2_OPEN_U_R_CELL);
					break;
				case DOOR2_CLOSE_D_L_CELL:
					SetCell(j,i,DOOR2_OPEN_D_L_CELL);
					break;
				case DOOR2_CLOSE_D_R_CELL:
					SetCell(j,i,DOOR2_OPEN_D_R_CELL);
					break;

				//���h�A��߂�
				case DOOR2_OPEN_U_L_CELL:
					SetCell(j,i,DOOR2_CLOSE_U_L_CELL);
					break;
				case DOOR2_OPEN_U_R_CELL:
					SetCell(j,i,DOOR2_CLOSE_U_R_CELL);
					break;
				case DOOR2_OPEN_D_L_CELL:
					SetCell(j,i,DOOR2_CLOSE_D_L_CELL | CELL_HITCHECK_MASK);
					break;
				case DOOR2_OPEN_D_R_CELL:
					SetCell(j,i,DOOR2_CLOSE_D_R_CELL | CELL_HITCHECK_MASK);
					break;

				//�R�E�S----------------------------------------------------------
				//�c�h�A���J����
				case DOOR3_CLOSE_U_CELL:
					SetCell(j,i,DOOR3_OPEN_U_CELL | CELL_HITCHECK_MASK);
					break;
				case DOOR3_CLOSE_D_CELL:
					SetCell(j,i,DOOR3_OPEN_D_CELL);
					break;
				case DOOR4_CLOSE_U_CELL:
					SetCell(j,i,DOOR4_OPEN_U_CELL | CELL_HITCHECK_MASK);
					break;
				case DOOR4_CLOSE_D_CELL:
					SetCell(j,i,DOOR4_OPEN_D_CELL);
					break;

				//�c�h�A��߂�
				case DOOR3_OPEN_U_CELL:
					SetCell(j,i,DOOR3_CLOSE_U_CELL | CELL_HITCHECK_MASK);
					break;
				case DOOR3_OPEN_D_CELL:
				//case DOOR4_OPEN_D_CELL:
					//���������Z���Ȃ̂�
					if( GetCellCode( j, (i-1) ) == DOOR3_CLOSE_U_CELL )
					{
						SetCell(j,i,DOOR3_CLOSE_D_CELL | CELL_HITCHECK_MASK);
					}else
					{
						SetCell(j,i,DOOR4_CLOSE_D_CELL | CELL_HITCHECK_MASK);
					}
					break;
				case DOOR4_OPEN_U_CELL:
					SetCell(j,i,DOOR4_CLOSE_U_CELL | CELL_HITCHECK_MASK);
					break;

				//�T�E�U----------------------------------------------------------
				//�d�C�}�V���̏㑤�̃Z����������
				case DOOR5_CELL:
					SetCell(j,i,DOOR6_CELL | CELL_HITCHECK_MASK);
					break;
				case DOOR6_CELL:
					SetCell(j,i,DOOR5_CELL);
					break;
			}
		}
	}

	return;
}

//--------------------------------------------------------------------
//						�W�����[�_�[���̏���
//						�������ׂĊJ����
//						�X�C�b�`��S��ON�ɂ���
//--------------------------------------------------------------------
void ScrGYM03AllChangeCell();
void ScrGYM03AllChangeCell()
{
	int i,j;

	for( i=0; i < GYM03_SWITCH_MAX ;i++ )
		SetCell((int)gym03_switch[i].x, (int)gym03_switch[i].y, SWITCH_ON_CELL_CODE);


	//for( i=0; i < VMap.Ysize ;i++ )
	for( i=GYM03_START_Y; i < GYM03_END_Y ;i++ )
	{
		//for( j=0; j < VMap.Xsize ;j++ )
		for( j=GYM03_START_X; j < GYM03_END_X ;j++ )
		{
			switch( GetCellCode(j,i) )
			{
				//�P--------------------------------------------------------------
				//���h�A���J����
				case DOOR1_CLOSE_U_L_CELL:
					SetCell(j,i,DOOR1_OPEN_U_L_CELL);
					break;
				case DOOR1_CLOSE_U_R_CELL:
					SetCell(j,i,DOOR1_OPEN_U_R_CELL);
					break;
				case DOOR1_CLOSE_D_L_CELL:
					SetCell(j,i,DOOR1_OPEN_D_L_CELL);
					break;
				case DOOR1_CLOSE_D_R_CELL:
					SetCell(j,i,DOOR1_OPEN_D_R_CELL);
					break;

				//�Q--------------------------------------------------------------
				//���h�A���J����
				case DOOR2_CLOSE_U_L_CELL:
					SetCell(j,i,DOOR2_OPEN_U_L_CELL);
					break;
				case DOOR2_CLOSE_U_R_CELL:
					SetCell(j,i,DOOR2_OPEN_U_R_CELL);
					break;
				case DOOR2_CLOSE_D_L_CELL:
					SetCell(j,i,DOOR2_OPEN_D_L_CELL);
					break;
				case DOOR2_CLOSE_D_R_CELL:
					SetCell(j,i,DOOR2_OPEN_D_R_CELL);
					break;

				//�R�E�S----------------------------------------------------------
				//�c�h�A���J����
				case DOOR3_CLOSE_U_CELL:
					SetCell(j,i,DOOR3_OPEN_U_CELL | CELL_HITCHECK_MASK);
					break;
				case DOOR3_CLOSE_D_CELL:
					SetCell(j,i,DOOR3_OPEN_D_CELL);
					break;
				case DOOR4_CLOSE_U_CELL:
					SetCell(j,i,DOOR4_OPEN_U_CELL | CELL_HITCHECK_MASK);
					break;
				case DOOR4_CLOSE_D_CELL:
					SetCell(j,i,DOOR4_OPEN_D_CELL);
					break;

				//�U----------------------------------------------------------
				//�d�C�}�V���̏㑤�̃Z����������
				case DOOR6_CELL:
					SetCell(j,i,DOOR5_CELL);
					break;
			}
		}
	}

	return;
}


//====================================================================
//
//						�g�E�J�W���̃h�A�A�j��
//
//====================================================================
#define GYM05_DOOR_ANIME_MAX	(5)				//�A�j���̐�
#define CELL_CODE_DIFF			(8)				//�Z���i���o�[����
#define GYM05_DOOR01_CELL		(536)			//���߂ɃA�j������Z���i���o�[
static const u8 gym05_door_wait[]={0,1,1,1,1};	//�E�F�C�g����
static const u16 gym05_door_cell_code[GYM05_DOOR_ANIME_MAX]={GYM05_DOOR01_CELL,537,538,539,540};
static u8 gym05_counter = 0;					//�J�E���^�[
static u8 gym05_waitno = 0;						//���̃E�F�C�g�i���o�[

void ScrGYM05DoorAnimeWait();
void GYM05DoorAnimeTask(u8);
static void GYM05DoorAnimePosSelect(u8,u16);
void ScrGYM05DoorChangeCell();

//--------------------------------------------------------------------
//						�h�A�A�j��(�E�F�C�g�L��)
//--------------------------------------------------------------------
void ScrGYM05DoorAnimeWait()
{
	gym05_counter = 0;
	gym05_waitno = 0;
	SePlay(SE_KI_GASYAN);
	AddTask( GYM05DoorAnimeTask, 8 );
	return;
}

void GYM05DoorAnimeTask(u8 id)
{
	if( gym05_door_wait[gym05_waitno] == gym05_counter )
	{
		GYM05DoorAnimePosSelect( (u8)ScriptParameter0, gym05_door_cell_code[gym05_waitno] );

		gym05_counter = 0;
		gym05_waitno++;

		if( gym05_waitno == GYM05_DOOR_ANIME_MAX )
		{
			DelTask(id);
			ContinueFieldScript();
		}

	}else
	{
		gym05_counter++;
	}

	return; 
}

//--------------------------------------------------------------------
//							�Z��������������
//
//	����	no		=	�ǂ̃h�A��	
//			code	=	����������Z���R�[�h
//
//--------------------------------------------------------------------
static void GYM05DoorAnimePosSelect( u8 no, u16 code )
{
	u8 i,max = 0;
	u16 x[4];
	u16 y[4];

	switch( no )
	{
		case 1:		//����؂��̕���
			max = 2;
			x[0] = 1;
			x[1] = 7;
			y[0] = 104;
			y[1] = 104;
			break;
		case 2:		//ELITEM_01(����)
			max = 2;
			x[0] = 1;
			x[1] = 7;
			y[0] = 78;
			y[1] = 78;
			break;
		case 3:		//ELITEW_01(�Ԑ�)
			max = 2;
			x[0] = 1;
			x[1] = 7;
			y[0] = 91;
			y[1] = 91;
			break;
		case 4:		//ELITM_02(��)
			max = 1;
			x[0] = 7;
			y[0] = 39;
			break;
		case 5:		//ELITW_02(�X�s�[�h)
			max = 2;
			x[0] = 1;
			x[1] = 7;
			y[0] = 52;
			y[1] = 52;
			break;
		case 6:		//ELITM_03(�g�o)
			max = 1;
			x[0] = 1;
			y[0] = 65;
			break;
		case 7:		//ELITW_03(�\��)
			max = 1;
			x[0] = 7;
			y[0] = 13;
			break;
		case 8:		//ELITM_04(�Z)
			max = 1;
			x[0] = 1;
			y[0] = 26;
			break;
	}

	for( i=0; i < max ;i++ )
	{
		SetCell( x[i]+MAP_X_MARGIN, y[i]+MAP_Y_MARGIN ,
				code | CELL_HITCHECK_MASK );
		SetCell( x[i]+MAP_X_MARGIN, (y[i] + 1)+MAP_Y_MARGIN ,
				(code + CELL_CODE_DIFF) | CELL_HITCHECK_MASK );
	}

	MakeMapBG();
	return;
}

//--------------------------------------------------------------------
//					�Q�[�����[�h���E���[�_�[�|��������
//					�h�A�̃Z�����E�F�C�g�Ȃ��ŏ���������
//--------------------------------------------------------------------
void ScrGYM05DoorChangeCell()
{
	GYM05DoorAnimePosSelect( (u8)ScriptParameter0, gym05_door_cell_code[GYM05_DOOR_ANIME_MAX-1] );
	return;
}


//====================================================================
//
//							���̑�
//
//====================================================================
void ScrMsgExpandBufferSet();
void ScrGetMyPos();
u8 ScrGetID();
void ScrSetName01();
u8 ScrGetWinLoseFlag();
u8 GetNatukidoLevel(void);

//--------------------------------------------------------------------
//							�X�N���v�g����
//					_PROGRAM	EXPANDBUF_MSG�ŌĂ�
//--------------------------------------------------------------------
void ScrMsgExpandBufferSet()
{
	FieldMsgSet(MsgExpandBuffer);
}


//--------------------------------------------------------------------
//						���@�̈ʒu���擾����
//						ScriptParameter0,1�g�p
//--------------------------------------------------------------------
void ScrGetMyPos()
{
	ScriptParameter0 = (u16)Fld.MapXpos;
	ScriptParameter1 = (u16)Fld.MapYpos;
}


//--------------------------------------------------------------------
//						ID���擾����(���͉��ꌅ)
//--------------------------------------------------------------------
u8 ScrGetID()
{
	u16 id;
	id = (MyData.ID[1] << 8 | MyData.ID[0] );
	return ( id % 10 );  
}


//--------------------------------------------------------------------
//						���o�����A���Z�����Z�b�g
//--------------------------------------------------------------------
void ScrSetName01()
{
	if( MyData.my_sex == MALE )	//�j
	{
		PM_strcpy( StrTempBuffer0, str_onityan );
	}else
	{
		PM_strcpy( StrTempBuffer0, str_onetyan );
	}

	return;
}


//--------------------------------------------------------------------
//						�ނ����A�ނ��߃Z�b�g
//--------------------------------------------------------------------
void ScrSetName02()
{
	if( MyData.my_sex == MALE )	//�j
	{
		PM_strcpy( StrTempBuffer0, str_musume );
	}else
	{
		PM_strcpy( StrTempBuffer0, str_musuko );
	}

	return;
}


//--------------------------------------------------------------------
//						�퓬��Ԃ��擾����
//--------------------------------------------------------------------
u8 ScrGetWinLoseFlag()
{
	return WinLoseFlag;  
}


//--------------------------------------------------------------------
//					���[�v�E�F�C�̈ړ�����Z�b�g����
//--------------------------------------------------------------------
void ScrSetRopewayMapID( void )
{
	if( ScriptParameter0 )
		NextMapSetDirect( R112, R112_R0101, NO_EXIT_ID, 6, 4 );
	else
		NextMapSetDirect( R112, R112_R0102, NO_EXIT_ID, 6, 4 );

	return;
}


//--------------------------------------------------------------------
// ���[�N�ɓ����Ă���l���t���O�i���o�[�Ƃ��ēn���ăt���O���Z�b�g����
//--------------------------------------------------------------------
void ScrWKFlagSet( void );
void ScrWKFlagSet( void )
{
	EventFlagSet(ScriptParameter0);
	return;
}


#define SPACE_CENTER_COUNT_MAX	(9999)		//�J�E���g�ő�l
//--------------------------------------------------------------------
//				�Q�[�����߂Ă��牽�T�Ԃ����������擾����
//--------------------------------------------------------------------
u16 ScrGetWeekFromGameStart();
u16 ScrGetWeekFromGameStart()
{
	u16 week;
	week = (GameTime.day / 7);
	if( week > SPACE_CENTER_COUNT_MAX )	
		return SPACE_CENTER_COUNT_MAX;
	else 
		return week;
}


//--------------------------------------------------------------------
//						�Ȃ��x�̃��x����Ԃ�
//
//						t106_r0301
//						t107_r0201
//						c102_r0401�Ŏg�p��
//--------------------------------------------------------------------
u8 GetNatukidoLevel(void)
{
	u8 ret;
	ret = GetTemotiBtlPokeNum();

	if(PokeParaGet(&PokeParaMine[ret], ID_friend ) == 255 ) return 6;
	if(PokeParaGet(&PokeParaMine[ret], ID_friend ) >= 200 ) return 5;
	if(PokeParaGet(&PokeParaMine[ret], ID_friend ) >= 150 ) return 4;
	if(PokeParaGet(&PokeParaMine[ret], ID_friend ) >= 100 ) return 3;
	if(PokeParaGet(&PokeParaMine[ret], ID_friend ) >=  50 ) return 2;
	if(PokeParaGet(&PokeParaMine[ret], ID_friend ) >=   1 ) return 1;
	return 0;
}


//--------------------------------------------------------------------
//						�^�E���}�b�v�Ăяo��
//--------------------------------------------------------------------
extern void InfoMapInit(pFunc);
static void scr_info_map_start( void )
{
	InfoMapInit(FieldScriptRecover);
}
void ScrInfoMapInit(void)
{
	MainProcChange( scr_info_map_start );
//	InfoMapInit(FieldScriptRecover);
}


//====================================================================
//						�p�\�R���̃Z���A�j��
//
//					ScriptParameter0�Ƀ^�C�v������
//
//====================================================================
#define PASO_CELL_CODE01	5		//�����Ă���p�\�R��(�|�P�Z��)
#define PASO_CELL_CODE02	4		//�����Ă��Ȃ��p�\�R��(�|�P�Z��)
#define PASO_CELL_CODE03	639		//�����Ă���p�\�R��(���� �j)
#define PASO_CELL_CODE04	602		//�����Ă��Ȃ��p�\�R��(���� �j)
#define PASO_CELL_CODE05	638		//�����Ă���p�\�R��(���� ��)
#define PASO_CELL_CODE06	601		//�����Ă��Ȃ��p�\�R��(���� ��)
#define PASO_ANIME_WAIT		6		//�A�j���E�F�C�g
#define PASO_ANIME_NUM_MAX	5		//�A�j�������(��ɂ���)

//taskwork
enum{
	PASO_ANM_SEQ_WK = 0,			//�V�[�P���X�i���o�[
	PASO_ANM_TASKNO_WK,				//taskno�ۑ�
	PASO_ANM_NUM_WK,				//�p�\�R���A�j����
	PASO_ANM_WAIT_WK,				//�E�F�C�g�J�E���^�[
	PASO_ANM_CELL_WK,				//�Z���i���o�[����
};

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void PasoAnimeTaskAdd();			//�p�\�R��������
void PasoAnimeTaskDel();			//�p�\�R��������
void PasoAnimeTask(u8);
void PasoAnimeSeq(TASK_TABLE *);
void PasoAnimeSeqDel();
void PasoAnimeCellSet(s16,s8,s8);	//�Z�����Z�b�g����

//--------------------------------------------------------------------
//						�p�\�R���A�j���^�X�N�ǉ�
//--------------------------------------------------------------------
void PasoAnimeTaskAdd()
{
	u8 no;

	//���łɓo�^����Ă��邩
	if( CheckTask( PasoAnimeTask ) == TRUE ) return;

	no = AddTask( PasoAnimeTask, 8 );
	TaskTable[no].work[PASO_ANM_SEQ_WK] = 0;		//�V�[�P���X�i���o�[
	TaskTable[no].work[PASO_ANM_TASKNO_WK] = no;	//taskno�ۑ�
	TaskTable[no].work[PASO_ANM_NUM_WK] = 0;		//�p�\�R���A�j����
	TaskTable[no].work[PASO_ANM_WAIT_WK] = 0;		//�E�F�C�g�J�E���^�[
	TaskTable[no].work[PASO_ANM_CELL_WK] = 0;		//�Z���i���o�[����
	return;
}

//--------------------------------------------------------------------
//						�p�\�R���A�j���^�X�N
//--------------------------------------------------------------------
void PasoAnimeTask(u8 id)
{
	TASK_TABLE * task_p;

	task_p = &TaskTable[id];
	switch( task_p->work[PASO_ANM_SEQ_WK] )
	{
		case 0:
			PasoAnimeSeq(task_p);
			break;
		case 1:
			break;
	};
}

void PasoAnimeSeq( TASK_TABLE *task_p )
{
	u8 site;
	s8 hosei_x=0,hosei_y=0;

	//�E�F�C�g�J�E���^�[�`�F�b�N
	if( task_p->work[PASO_ANM_WAIT_WK] == PASO_ANIME_WAIT )
	{
		task_p->work[PASO_ANM_WAIT_WK] = 0;		//�N���A

		site = GetHeroSite();

		switch( site )
		{
			case SITE_U:
				hosei_x = 0;
				hosei_y = -1;
				break;
			case SITE_L:
				hosei_x = -1;
				hosei_y = -1;
				break;
			case SITE_R:
				hosei_x = 1;
				hosei_y = -1;
				break;
		};

		//�p�\�R���Z���Z�b�g(�|�P�Z���E����j�A���)
		PasoAnimeCellSet( task_p->work[PASO_ANM_CELL_WK], hosei_x, hosei_y );
		MakeMapBG();

		//�Z�����ʂ�ύX
		task_p->work[PASO_ANM_CELL_WK]^=1;

		//�p�\�R���A�j����
		task_p->work[PASO_ANM_NUM_WK]++;
		if( task_p->work[PASO_ANM_NUM_WK] == PASO_ANIME_NUM_MAX )
			DelTask(task_p->work[PASO_ANM_TASKNO_WK]);
	}

	//�E�F�C�g�J�E���^�[
	task_p->work[PASO_ANM_WAIT_WK]++;

	return; 
}

//--------------------------------------------------------------------
//				�p�\�R���Z���Z�b�g(�|�P�Z���E����j�A���)
//--------------------------------------------------------------------
void PasoAnimeCellSet(s16 no, s8 x, s8 y)
{
	u16 cell_code = 0;

	//�����Ă���E�����Ă��Ȃ��Z���R�[�h�A�j��
	if( no )
	{
		if( ScriptParameter0 == PASO_POKEMON_CENTER )	
		{
			cell_code = PASO_CELL_CODE02;
		}else if( ScriptParameter0 == PASO_MY_HOME_BOY )
		{
			cell_code = PASO_CELL_CODE04;
		}else if( ScriptParameter0 == PASO_MY_HOME_GIRL )
		{
			cell_code = PASO_CELL_CODE06;
		};
	}else
	{
		if( ScriptParameter0 == PASO_POKEMON_CENTER )	
		{
			cell_code = PASO_CELL_CODE01;
		}else if( ScriptParameter0 == PASO_MY_HOME_BOY )
		{
			cell_code = PASO_CELL_CODE03;
		}else if( ScriptParameter0 == PASO_MY_HOME_GIRL )
		{
			cell_code = PASO_CELL_CODE05;
		}
	}

	SetCell( Fld.MapXpos+x+MAP_X_MARGIN, Fld.MapYpos+y+MAP_Y_MARGIN ,
		cell_code | CELL_HITCHECK_MASK );

	return;
}

//--------------------------------------------------------------------
//						�p�\�R���A�j���^�X�N�폜
//--------------------------------------------------------------------
void PasoAnimeTaskDel()
{
	PasoAnimeSeqDel();
	return;
}

void PasoAnimeSeqDel()
{
	u8 site;
	s8 hosei_x=0,hosei_y=0;
	u16 cell_code=0;

	site = GetHeroSite();

	switch( site )
	{
		case SITE_U:
			hosei_x = 0;
			hosei_y = -1;
			break;
		case SITE_L:
			hosei_x = -1;
			hosei_y = -1;
			break;
		case SITE_R:
			hosei_x = 1;
			hosei_y = -1;
			break;
	};

	if( ScriptParameter0 == PASO_POKEMON_CENTER )	
	{
		cell_code = PASO_CELL_CODE02;
	}else if( ScriptParameter0 == PASO_MY_HOME_BOY )
	{
		cell_code = PASO_CELL_CODE04;
	}else if( ScriptParameter0 == PASO_MY_HOME_GIRL )
	{
		cell_code = PASO_CELL_CODE06;
	};

	//���̃p�\�R���̃Z���ɖ߂�
	SetCell( Fld.MapXpos+hosei_x+MAP_X_MARGIN, Fld.MapYpos+hosei_y+MAP_Y_MARGIN ,
		cell_code | CELL_HITCHECK_MASK );

	MakeMapBG();

	return;
}


//====================================================================
//						�|�P���������̃Z���A�j��
//====================================================================
#define NOTE_CELL_CODE01_U	600		//�����Ă���m�[�g(��)
#define NOTE_CELL_CODE01_D	608		//					(��)
#define NOTE_CELL_CODE02_U	669		//�����Ă��Ȃ��m�[�g(��)
#define NOTE_CELL_CODE02_D	677		//					  (��)
#define NOTE_ANIME_WAIT		6		//�A�j���E�F�C�g
#define NOTE_ANIME_NUM_MAX	5		//�A�j�������(��ɂ���)

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void NoteAnimeTaskAdd();			//�m�[�g������
void NoteAnimeTask(u8);
void NoteAnimeSeq(TASK_TABLE *);
void NoteAnimeSeqDel();

//--------------------------------------------------------------------
//						�m�[�g�A�j���^�X�N�ǉ�
//--------------------------------------------------------------------
void NoteAnimeTaskAdd()
{
	u8 no;

	//���łɓo�^����Ă��邩
	if( CheckTask( NoteAnimeTask ) == TRUE ) return;

	no = AddTask( NoteAnimeTask, 8 );
	TaskTable[no].work[PASO_ANM_SEQ_WK] = 0;		//�V�[�P���X�i���o�[
	TaskTable[no].work[PASO_ANM_TASKNO_WK] = no;	//taskno�ۑ�
	TaskTable[no].work[PASO_ANM_NUM_WK] = 0;		//�m�[�g�A�j����
	TaskTable[no].work[PASO_ANM_WAIT_WK] = 0;		//�E�F�C�g�J�E���^�[
	TaskTable[no].work[PASO_ANM_CELL_WK] = 0;		//�Z���i���o�[����
	return;
}

//--------------------------------------------------------------------
//						�m�[�g�A�j���^�X�N
//--------------------------------------------------------------------
void NoteAnimeTask(u8 id)
{
	TASK_TABLE * task_p;

	task_p = &TaskTable[id];

	//�V�[�P���X�i���o�[�ɂ���ď����𕪂���
	if( task_p->work[PASO_ANM_SEQ_WK] == 0 )
		NoteAnimeSeq(task_p);
}

void NoteAnimeSeq( TASK_TABLE *task_p )
{
	//�E�F�C�g�J�E���^�[�`�F�b�N
	if( task_p->work[PASO_ANM_WAIT_WK] == PASO_ANIME_WAIT )
	{
		task_p->work[PASO_ANM_WAIT_WK] = 0;		//�N���A

		//�����Ă���E�����Ă��Ȃ��Z���R�[�h�A�j��
		if( task_p->work[PASO_ANM_CELL_WK] )
		{
			SetCell( 11+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,
				NOTE_CELL_CODE02_U | CELL_HITCHECK_MASK );
			SetCell( 11+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,
				NOTE_CELL_CODE02_D | CELL_HITCHECK_MASK );
		}else
		{
			SetCell( 11+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,
				NOTE_CELL_CODE01_U | CELL_HITCHECK_MASK );
			SetCell( 11+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,
				NOTE_CELL_CODE01_D | CELL_HITCHECK_MASK );
		}

		MakeMapBG();

		//�Z�����ʂ�ύX
		task_p->work[PASO_ANM_CELL_WK]^=1;

		//�m�[�g�A�j����
		task_p->work[PASO_ANM_NUM_WK]++;
		if( task_p->work[PASO_ANM_NUM_WK] == PASO_ANIME_NUM_MAX )
			DelTask(task_p->work[PASO_ANM_TASKNO_WK]);
	}

	//�E�F�C�g�J�E���^�[
	task_p->work[PASO_ANM_WAIT_WK]++;

	return; 
}

//--------------------------------------------------------------------
//					�m�[�g�Z�������̏�Ԃɖ߂�
//--------------------------------------------------------------------
void NoteAnimeSeqDel()
{
	//���̃m�[�g�̃Z���ɖ߂�
	SetCell( 11+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,
		NOTE_CELL_CODE02_U | CELL_HITCHECK_MASK );
	SetCell( 11+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,
		NOTE_CELL_CODE02_D | CELL_HITCHECK_MASK );

	MakeMapBG();

	return;
}


//====================================================================
//						�G���x�[�^�[��т���
//====================================================================

//====================================================================
//							extern�錾	
//====================================================================
//extern void AddScrArrowAct(u16,u16,u8);		// �X�N���[�����A�N�^�[�\��
//extern void DelScrArrowAct(u8);				// �X�N���[�����A�N�^�[�\��
//extern void MarkActNoInit();				// �}�[�N�A�N�^�[�ԍ�������
//extern void MarkPalInit(void);				// �}�[�N�p���b�g������

//====================================================================
//								��`
//====================================================================
#define ELE_WORK_MAX	20		//�G���x�[�^�[���[�N�ő吔
#define ELEVATOR_MENU_MAX	(5)	//�t���A�\���ő吔

//�t���A�E�B���h�E���W
#define ELEVATOR_MENUWINX	(0)
#define ELEVATOR_MENUWINY	(0)
#define ELEVATOR_MENUWINX2	(ELEVATOR_MENUWINX+8)
#define ELEVATOR_MENUWINY2	(ELEVATOR_MENUWINY+(ELEVATOR_MENU_MAX*2)+1)

//���݂̃t���A�E�B���h�E���W
#define GENZAI_MENUWINX	(20)
#define GENZAI_MENUWINY	(0)
#define GENZAI_MENUWINX2	(GENZAI_MENUWINX+9)
#define GENZAI_MENUWINY2	(GENZAI_MENUWINY+5)

//�G���x�[�^�o�����W
#define ELEVATOR_XPOS	2
#define ELEVATOR_YPOS	1

//��ꕝ
#define ELEVATOR_SHAKE_Y		1		//�������̂��
#define ELEVATOR_SHAKE_Y2		1		//�J�n���̂��
#define ELEVATOR_SHAKE_NUM		23//13//3
#define ELEVATOR_SHAKE_NUM2		1//12
#define ELEVATOR_SHAKE_NUM3		3
#define ELEVATOR_SHAKE_WAIT		9		//�������̂��E�F�C�g
#define ELEVATOR_SHAKE_WAIT2	3		//�J�n���̂��E�F�C�g

//�G���x�[�^�\����
typedef struct{
	u8  floor;					// �t���A�f�[�^
	u8	div;
	u8	map;
	u8	exit;					// NO_EXIT_ID�ō��W�Ŏw�肵�Ă���̂ł���Ȃ�
}ELEVATOR_WORK;

//====================================================================
//								�ϐ�
//====================================================================
static ELEVATOR_WORK	ele[ELE_WORK_MAX];
static u8 floor_max = 0;		//�t���A�̐�
static u8 floor_pos = 0;		//���݂̃J�[�\���ʒu
static u8 act_check = 0;		//�A�N�^�[�����łɏo���Ă��邩

//====================================================================
//							���b�Z�[�W�f�[�^	
//====================================================================
static const u8 * const FloorData[] = { 
	msg1F, msg2F, msg3F, msg4F, msg5F, msg6F, msg7F, msg8F, msg9F, msg10F, msg11F, 
	msgB1F, msgB2F, msgB3F, msgB4F, msgTOP };

//====================================================================
//							�v���g�^�C�v�錾
//====================================================================
//FLAG_CHANGE_LABEL
void ElevatorNowFloorSet();				//���݂̊K�w�Z�b�g

//�K�w�f�[�^�Z�b�g
void ElevatorSet(u8,u8,u8,u8);			//�K�w�Z�b�g

//�G���x�[�^����
void ElevatorStart();					//�G���x�[�^�[�����X�^�[�g
static void ElevatorFloorSet();			//�E�B���h�E�쐬�Etask�ǉ��Ȃ�
void ElevatorTask(u8);
static void ElevatorDel(u8);			//�I������
static u8 ElevatorFloorWrite(u8,u8);	//�t���A���X�g�\��
void AddElevatorMoveTask();				//��ʗh�炵�����ǉ�
void ElevatorMoveTask(u8);				//��ʗh�炵����
static void ElevatorCsrSeq(u8,u8);		//�J�[�\������
static void AddArrowU();				//�㕔�J�[�\��
static void AddArrowD();				//�����J�[�\��
static void DelArrowUD();				//�J�[�\���폜
void ElevatorNowFloorWrite();			//���݂̃t���A�\��

static void TowerAnimeTaskAdd();			//�G���x�[�^�Z���A�j��
void TowerAnimeTask(u8);
void TowerAnimeSeq(TASK_TABLE *);

//--------------------------------------------------------------------
//							���݂̊K�w�Z�b�g
//
//					FLAG_CHANGE_LABEL�̃^�C�~���O�ŌĂ�
//
/*
					�~�i���ȊO�̃G���x�[�^�������ł�����
					ScriptParameter1�ɑ������case������ǉ�����
*/
//--------------------------------------------------------------------
void ElevatorNowFloorSet()
{
	u16 no;

	switch( Fld.SpMapID.map_id )
	{
		case C106_R1101:
			no = MSG_1F;
			break;
		case C106_R1102:
			no = MSG_2F;
			break;
		case C106_R1103:
			no = MSG_3F;
			break;
		case C106_R1104:
			no = MSG_4F;
			break;
		case C106_R1105:
			no = MSG_5F;
			break;
		case C106_R1106:
			no = MSG_TOP;
			break;

		//�ǉ�����
		//case

		default:
			no = MSG_1F;
	};

	SetEventWorkValue( WK_ELEVATOR_NO, no );
	return;
}

//--------------------------------------------------------------------
//							�K�w�Z�b�g
//--------------------------------------------------------------------
void ElevatorSet(u8 floor, u8 div, u8 map, u8 exit)
{
	u8 i; 

	//�P��ڂ̂ݏ���������
	if( ScriptParameter0 == 0 )
	{
		for( i=0; i < ELE_WORK_MAX ;i++ )
		{
			ele[i].floor = MSG_DMY;
		}
	}

	ele[ScriptParameter0].floor = floor;	//�t���A�f�[�^
	ele[ScriptParameter0].div	= div;		//divID
	ele[ScriptParameter0].map	= map;		//mapID
	ele[ScriptParameter0].exit = exit;		//exitID
	ScriptParameter0++;						//�v�f���C���N�������g
};

//--------------------------------------------------------------------
//						�G���x�[�^�[�X�^�[�g
//--------------------------------------------------------------------
void ElevatorStart()
{
	u8 i=0;

	floor_max = 0;
	floor_pos = 0;

	//END����
	ElevatorSet( MSG_DMY, 0, 0, 0 );

	//�t���A�̐�
	while(1)
	{
		if( ele[i].floor == MSG_DMY )	break;

		floor_max++;
		i++;
	}

	ElevatorFloorSet();
}

//--------------------------------------------------------------------
//						�E�B���h�E�Etask�̒ǉ��Ȃ�							
//--------------------------------------------------------------------
static void ElevatorFloorSet()
{
	u8 i;

	SetForceEvent();

	//�E�B���h�E�쐬
	if( ELEVATOR_MENU_MAX < floor_max )
	{
		NMenuWinBoxWrite( ELEVATOR_MENUWINX, ELEVATOR_MENUWINY, 
				ELEVATOR_MENUWINX2, ELEVATOR_MENUWINY2 );
		NMenuVCursorInit( ELEVATOR_MENUWINX+1, ELEVATOR_MENUWINY+1, ELEVATOR_MENU_MAX, 0);

		//�X�N���[���J�[�\���\��
		act_check = 0;
		//MarkActNoInit();
//		MarkPalInit();
		ScrActInit();
		AddMarkPal();
		AddArrowD();
	}else
	{
		NMenuWinBoxWrite( ELEVATOR_MENUWINX, ELEVATOR_MENUWINY, 
				ELEVATOR_MENUWINX2, ELEVATOR_MENUWINY+1+(floor_max*2) );
		NMenuVCursorInit( ELEVATOR_MENUWINX+1, ELEVATOR_MENUWINY+1, floor_max, 0);
	}

	//���X�g�쐬
	for( i=0 ; i < ELEVATOR_MENU_MAX ; i++ )
	{
		if( ele[i].floor == MSG_DMY )	break;

		NMenuMsgWrite( FloorData[ ele[i].floor ], 
				ELEVATOR_MENUWINX+2, ELEVATOR_MENUWINY+1+(i*2) );
	}

	//�E��Ɍ��݂̃t���A�\��
	ElevatorNowFloorWrite();

	AddTask( ElevatorTask, 8 );
}

//--------------------------------------------------------------------
//						�E��Ɍ��݂̃t���A�\��
//--------------------------------------------------------------------
void ElevatorNowFloorWrite()
{
	NMenuWinBoxWrite( GENZAI_MENUWINX, GENZAI_MENUWINY, 
				GENZAI_MENUWINX2, GENZAI_MENUWINY2 );
	NMenuMsgWrite( msg_genzai, 
				GENZAI_MENUWINX+1, GENZAI_MENUWINY+1 );
	NMenuMsgWrite( FloorData[ ScriptParameter1 ], 
				GENZAI_MENUWINX+3, GENZAI_MENUWINY+3 );

	return;
}

//--------------------------------------------------------------------
//
//					AnswerWork = TRUE = �t���A��I������
//							   = FALSE = �L�����Z������
//							
//--------------------------------------------------------------------
void ElevatorTask(u8 id)
{
	u8 csr_pos=0;
	//u8 no;

	if(sys.Trg == U_KEY)
	{
		if( floor_pos != 0 )
		{
			floor_pos--;
			csr_pos = NMenuVCursorPosGet();
			NMenuVCursorSetNoloop(-1);
			ElevatorFloorWrite(csr_pos, U_KEY);
		}
	}

	if(sys.Trg == D_KEY)
	{
		if( floor_pos != (floor_max-1) )
		{
			floor_pos++;
			csr_pos = NMenuVCursorPosGet();
			NMenuVCursorSetNoloop(1);
			ElevatorFloorWrite(csr_pos, D_KEY);
		}
	}
	
	if( sys.Trg & A_BUTTON )
	{
		SetSpecialMapDirect( 0, ele[floor_pos].div, ele[floor_pos].map, 
				//ele[floor_pos].exit, ELEVATOR_XPOS, ELEVATOR_YPOS );
				NO_EXIT_ID, ELEVATOR_XPOS, ELEVATOR_YPOS );

		//�I�񂾃t���A��������
		if( ScriptParameter1 == floor_pos )
		{
			AnswerWork = FALSE;
			SePlay(SE_SELECT);
			NMenuBoxClear(0,0,29,12);
			ElevatorDel(id);
		}else
		{
			AnswerWork = TRUE;

			//������x�G���x�[�^�[�K�[���ɘb�����Ƃ���
			//���݂̃t���A���X�V�����悤�ɂ���
			ScriptParameter1 = floor_pos;

			//��ʗh�炷�G�t�F�N�g
			AddElevatorMoveTask();

			//�󂯕t���̌�����ς���
			EvObjChgSiteId(TargetObjID, Fld.MapID.map_id, Fld.MapID.div_id, SITE_D);

			DelArrowUD();
			NMenuScreenClear();
			DelTask(id);
		}

		return;
	}

	if( sys.Trg & B_BUTTON ){
		AnswerWork = FALSE;
		SePlay(SE_SELECT);
		DelArrowUD();
		NMenuBoxClear(0,0,29,12);
		ElevatorDel(id);
	}
}

//--------------------------------------------------------------------
//							�t���A���X�g�\��						
//
//	�߂�l	FALSE = ���������ĂȂ�
//			TRUE  = ����������
//--------------------------------------------------------------------
static u8 ElevatorFloorWrite(u8 csr_pos, u8 key)
{
	u8 i,j,sw=FALSE,start=0;

	//���X�g�̐����`�F�b�N
	if( floor_max < ELEVATOR_MENU_MAX )	return FALSE;

	//��L�[�X�N���[��
	if( key == U_KEY )
	{
		if( csr_pos == 0 )
		{
			start = floor_pos;
			sw = TRUE;
		}
	
	//���L�[�X�N���[��
	}else if( key == D_KEY )
	{
		if( csr_pos == (ELEVATOR_MENU_MAX-1) )
		{
			//floor_pos����`��
			start = (floor_pos-csr_pos);
			sw = TRUE;
		}
	}

	//�t���A���X�g��������
	if( sw )
	{
		//�J�[�\������
		ElevatorCsrSeq(start,ELEVATOR_MENU_MAX);

		//�󔒂ŕ`��
		NMenuBoxWrite( ELEVATOR_MENUWINX+2, ELEVATOR_MENUWINY+1, 
			ELEVATOR_MENUWINX2-1, ELEVATOR_MENUWINY2-1 );

		for( i=start,j=0 ; j < ELEVATOR_MENU_MAX ; i++,j++ )
		{
			if( ele[i].floor == MSG_DMY )	break;
	
			//���X�g�`��
			NMenuMsgWrite( FloorData[ ele[i].floor ], 
				ELEVATOR_MENUWINX+2, ELEVATOR_MENUWINY+1+(j*2) );
		}
	}

	return sw;
}

//--------------------------------------------------------------------
//							�J�[�\������
//--------------------------------------------------------------------
static void ElevatorCsrSeq(u8 start, u8 max)
{
	//��Ԏn�߂�\�����Ă�����㕔�J�[�\�����폜����
	if( start == 0 )
	{
		act_check ^= 2;
		DelScrCurActor( MARK_ACT_U_CURSOR );
//		DelScrArrowAct(0);
	}

	//��Ԏn�߂�\�����Ă��Ȃ�������㕔�J�[�\����ǉ�����
	if( start > 0 )
	{
		AddArrowU();
	}

	//�Ō�܂ŕ\�����Ă��Ȃ������牺���J�[�\����ǉ�����
	if( (start+max) < floor_max )
	{
		AddArrowD();
		return;

	//�Ō�܂ŕ\�����Ă����牺���J�[�\�����폜����
	}else if( (start+max) == floor_max )
	{
		act_check ^= 1;
		DelScrCurActor( MARK_ACT_D_CURSOR );
//		DelScrArrowAct(1);
		return;
	}

	return;
}

//--------------------------------------------------------------------
//					�G���x�[�^�[��ʗh�炵�Ăяo��
//--------------------------------------------------------------------
void AddElevatorMoveTask()
{
	u8 no;
	no = AddTask( ElevatorMoveTask, 9 );
	TaskTable[no].work[0] = ELEVATOR_SHAKE_Y;	//��ꕝ(�I��)
	TaskTable[no].work[1] = 0;					//�E�F�C�g�J�E���^
	TaskTable[no].work[2] = 0;					//
	TaskTable[no].work[3] = 0;					//�J�n�E�҂��E�I��
	TaskTable[no].work[4] = ELEVATOR_SHAKE_Y2;	//��ꕝ(�J�n)
	TaskTable[no].work[5] = ELEVATOR_SHAKE_WAIT2;//�E�F�C�g����
	SetMapViewFunc( NULL );

	TowerAnimeTaskAdd();		//�Z���A�j��
	SePlay(SE_ELEBETA);
}

//--------------------------------------------------------------------
//					�G���x�[�^�[��ʗh�炵����
//--------------------------------------------------------------------
void ElevatorMoveTask(u8 id)
{
	TASK_TABLE * task_p;

	task_p = &TaskTable[id];
	
	task_p->work[1]++;

	if( (task_p->work[1] % task_p->work[5]) == 0 )
	{
		task_p->work[1] = 0;
		task_p->work[2]++;

		switch( task_p->work[3] )
		{
			case 0:
				task_p->work[4] = -task_p->work[4];
				SetMapViewOffset( 0, task_p->work[4] );
				if( task_p->work[2] == ELEVATOR_SHAKE_NUM )
				{
					SePlay(SE_PINPON);	
					ElevatorDel(id);
					MapViewInit();
				}
				break;
				break;
		}
	}

	return;
}

//--------------------------------------------------------------------
//							�I������
//--------------------------------------------------------------------
void ElevatorDel(u8 id)
{
	DelTask(id);
	ContinueFieldScript();
	return;
}

//--------------------------------------------------------------------
//							�㕔�J�[�\���\��
//--------------------------------------------------------------------
static void AddArrowU()
{
	//���łɕ\������Ă���
	if( (act_check >> 1) == 1 )	return;

	act_check |= 2;
	AddScrCurActor( MARK_ACT_U_CURSOR, ELEVATOR_MENUWINX+36, ELEVATOR_MENUWINY+8 );
//	AddScrArrowAct( ELEVATOR_MENUWINX+36, ELEVATOR_MENUWINY+8, 0);
	return;
}

//--------------------------------------------------------------------
//							�����J�[�\���\��
//--------------------------------------------------------------------
static void AddArrowD()
{
	//���łɕ\������Ă���
	if( (act_check & 1) == 1 )	return;

	act_check |= 1;
	AddScrCurActor( MARK_ACT_D_CURSOR, ELEVATOR_MENUWINX+36, ELEVATOR_MENUWINY+(8*9) );
//	AddScrArrowAct( ELEVATOR_MENUWINX+36, ELEVATOR_MENUWINY+(8*9), 1);
	return;
}


//====================================================================
//					�o�g���^���[�G���x�[�^�̃Z���A�j��
//====================================================================
#define TOWER_CELL_CODE01A	616		//
#define TOWER_CELL_CODE01B	619		//					
#define TOWER_CELL_CODE02A	617		//
#define TOWER_CELL_CODE02B	620		//
#define TOWER_CELL_CODE03A	618		//
#define TOWER_CELL_CODE03B	621		//
#define TOWER_CELL_CODE04A	624		//
#define TOWER_CELL_CODE04B	627		//					
#define TOWER_CELL_CODE05A	625		//
#define TOWER_CELL_CODE05B	628		//
#define TOWER_CELL_CODE06A	626		//
#define TOWER_CELL_CODE06B	629		//
#define TOWER_CELL_CODE07A	632		//
#define TOWER_CELL_CODE07B	635		//					
#define TOWER_CELL_CODE08A	633		//
#define TOWER_CELL_CODE08B	636		//
#define TOWER_CELL_CODE09A	634		//
#define TOWER_CELL_CODE09B	637		//
#define TOWER_ANIME_WAIT	8		//�A�j���E�F�C�g
#define TOWER_ANIME_NUM_MAX	8		//�A�j�������(�����ɂ���)

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//						�A�j���^�X�N�ǉ�
//--------------------------------------------------------------------
static void TowerAnimeTaskAdd()
{
	u8 no;

	//���łɓo�^����Ă��邩
	if( CheckTask( TowerAnimeTask ) == TRUE ) return;

	no = AddTask( TowerAnimeTask, 8 );
	TaskTable[no].work[PASO_ANM_SEQ_WK] = 0;		//�V�[�P���X�i���o�[
	TaskTable[no].work[PASO_ANM_TASKNO_WK] = no;	//taskno�ۑ�
	TaskTable[no].work[PASO_ANM_NUM_WK] = 0;		//�m�[�g�A�j����
	TaskTable[no].work[PASO_ANM_WAIT_WK] = 0;		//�E�F�C�g�J�E���^�[
	TaskTable[no].work[PASO_ANM_CELL_WK] = 0;		//�Z���i���o�[����
	return;
}

//--------------------------------------------------------------------
//							�A�j���^�X�N
//--------------------------------------------------------------------
void TowerAnimeTask(u8 id)
{
	TASK_TABLE * task_p;
	task_p = &TaskTable[id];
	TowerAnimeSeq(task_p);
}

void TowerAnimeSeq( TASK_TABLE *task_p )
{
	//�E�F�C�g�J�E���^�[�`�F�b�N
	if( task_p->work[PASO_ANM_WAIT_WK] == TOWER_ANIME_WAIT )
	{
		task_p->work[PASO_ANM_WAIT_WK] = 0;		//�N���A

		if( task_p->work[PASO_ANM_CELL_WK] )
		{
			SetCell( 0+MAP_X_MARGIN, 0+MAP_Y_MARGIN ,TOWER_CELL_CODE01A | CELL_HITCHECK_MASK );
			SetCell( 1+MAP_X_MARGIN, 0+MAP_Y_MARGIN ,TOWER_CELL_CODE02A | CELL_HITCHECK_MASK );
			SetCell( 2+MAP_X_MARGIN, 0+MAP_Y_MARGIN ,TOWER_CELL_CODE02A | CELL_HITCHECK_MASK );
			SetCell( 3+MAP_X_MARGIN, 0+MAP_Y_MARGIN ,TOWER_CELL_CODE03A | CELL_HITCHECK_MASK );
			SetCell( 0+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,TOWER_CELL_CODE04A | CELL_HITCHECK_MASK );
			SetCell( 1+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,TOWER_CELL_CODE05A | CELL_HITCHECK_MASK );
			SetCell( 2+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,TOWER_CELL_CODE05A | CELL_HITCHECK_MASK );
			SetCell( 3+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,TOWER_CELL_CODE06A | CELL_HITCHECK_MASK );
			SetCell( 0+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,TOWER_CELL_CODE07A | CELL_HITCHECK_MASK );
			SetCell( 1+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,TOWER_CELL_CODE08A | CELL_HITCHECK_MASK );
			SetCell( 2+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,TOWER_CELL_CODE08A | CELL_HITCHECK_MASK );
			SetCell( 3+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,TOWER_CELL_CODE09A | CELL_HITCHECK_MASK );
		}else
		{
			SetCell( 0+MAP_X_MARGIN, 0+MAP_Y_MARGIN ,TOWER_CELL_CODE01B | CELL_HITCHECK_MASK );
			SetCell( 1+MAP_X_MARGIN, 0+MAP_Y_MARGIN ,TOWER_CELL_CODE02B | CELL_HITCHECK_MASK );
			SetCell( 2+MAP_X_MARGIN, 0+MAP_Y_MARGIN ,TOWER_CELL_CODE02B | CELL_HITCHECK_MASK );
			SetCell( 3+MAP_X_MARGIN, 0+MAP_Y_MARGIN ,TOWER_CELL_CODE03B | CELL_HITCHECK_MASK );
			SetCell( 0+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,TOWER_CELL_CODE04B | CELL_HITCHECK_MASK );
			SetCell( 1+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,TOWER_CELL_CODE05B | CELL_HITCHECK_MASK );
			SetCell( 2+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,TOWER_CELL_CODE05B | CELL_HITCHECK_MASK );
			SetCell( 3+MAP_X_MARGIN, 1+MAP_Y_MARGIN ,TOWER_CELL_CODE06B | CELL_HITCHECK_MASK );
			SetCell( 0+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,TOWER_CELL_CODE07B | CELL_HITCHECK_MASK );
			SetCell( 1+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,TOWER_CELL_CODE08B | CELL_HITCHECK_MASK );
			SetCell( 2+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,TOWER_CELL_CODE08B | CELL_HITCHECK_MASK );
			SetCell( 3+MAP_X_MARGIN, 2+MAP_Y_MARGIN ,TOWER_CELL_CODE09B | CELL_HITCHECK_MASK );
		}

		MakeMapBG();

		//�Z�����ʂ�ύX
		task_p->work[PASO_ANM_CELL_WK]^=1;

		//�A�j����
		task_p->work[PASO_ANM_NUM_WK]++;
		if( task_p->work[PASO_ANM_NUM_WK] == TOWER_ANIME_NUM_MAX )
			DelTask(task_p->work[PASO_ANM_TASKNO_WK]);
	}

	//�E�F�C�g�J�E���^�[
	task_p->work[PASO_ANM_WAIT_WK]++;

	return; 
}


//--------------------------------------------------------------------
//							�J�[�\���폜
//--------------------------------------------------------------------
static void DelArrowUD()
{
	if( (act_check & 1) == 1 )
//		DelScrArrowAct(1);
		DelScrCurActor( MARK_ACT_D_CURSOR );
	if( (act_check >> 1) == 1 )
//		DelScrArrowAct(0);
		DelScrCurActor( MARK_ACT_U_CURSOR );
	DelMarkPal();
	return;
}

//--------------------------------------------------------------------
//						�B���A�C�e�������t���O�I��(�������ԁH)
//--------------------------------------------------------------------
//�Ƃ肠�����A�J���N���剤�p
void KarakuriHideItemOff(void)
{
	ScriptParameter0 = 0xffff & ( (u32)kinnotama_1 >> 16 );
	ScriptParameter0 += FH_FLAG_START;
	EventFlagSet(ScriptParameter0);
	return;
}

//--------------------------------------------------------------------
//						�B���A�C�e�������t���O�I�t(���O�̏�ԁH)
//--------------------------------------------------------------------
//�Ƃ肠�����A�J���N���剤�p
void KarakuriHideItemOn(void)
{
	ScriptParameter0 = 0xffff & ( (u32)kinnotama_1 >> 16 );
	ScriptParameter0 += FH_FLAG_START;
	EventFlagReset(ScriptParameter0);
	return;
}

//--------------------------------------------------------------------
//					���������N���u�p�������悳�`�F�b�N
//--------------------------------------------------------------------
//0:�`199,1:200�`
u8 DaisukiClubStyleCheck(void)
{
	if(PokeParaGet(&PokeParaMine[GetTemotiBtlPokeNum()], ID_style ) < 200 ) return 0;
	return 1;
}

//--------------------------------------------------------------------
//					���������N���u�p���������`�F�b�N
//--------------------------------------------------------------------
//0:�`199,1:200�`
u8 DaisukiClubBeautifulCheck(void)
{
	if(PokeParaGet(&PokeParaMine[GetTemotiBtlPokeNum()], ID_beautiful ) < 200 ) return 0;
	return 1;
}

//--------------------------------------------------------------------
//					���������N���u�p���킢���`�F�b�N
//--------------------------------------------------------------------
//0:�`199,1:200�`
u8 DaisukiClubCuteCheck(void)
{
	if(PokeParaGet(&PokeParaMine[GetTemotiBtlPokeNum()], ID_cute ) < 200 ) return 0;
	return 1;
}

//--------------------------------------------------------------------
//					���������N���u�p���������`�F�b�N
//--------------------------------------------------------------------
//0:�`199,1:200�`
u8 DaisukiClubCleverCheck(void)
{
	if(PokeParaGet(&PokeParaMine[GetTemotiBtlPokeNum()], ID_clever ) < 200 ) return 0;
	return 1;
}

//--------------------------------------------------------------------
//					���������N���u�p�����܂����`�F�b�N
//--------------------------------------------------------------------
//0:�`199,1:200�`
u8 DaisukiClubStrongCheck(void)
{
	if(PokeParaGet(&PokeParaMine[GetTemotiBtlPokeNum()], ID_clever ) < 200 ) return 0;
	return 1;
}

//--------------------------------------------------------------------
//					�莝���ɂ����^�C�v�����邩�`�F�b�N(�^�}�S�͖����j
//					AnswerWork = TRUE  :�@����
//					AnswerWork = FALSE :�@���Ȃ�
//--------------------------------------------------------------------
void TypeKusaAllCheck(void)
{
	u8	i;
	u16 no;
	PokemonParam *pp;

	for (i=0;i<TEMOTI_POKEMAX;i++) {
		pp = &PokeParaMine[i];
		if (( PokeParaGet(pp,ID_poke_exist_flag) != 0 )&&( PokeParaGet( pp, ID_tamago_flag ) == 0 )){
			no = PokeParaGet( pp, ID_monsno );
			if( PPD[no].type1 == KUSA_TYPE || PPD[no].type2 == KUSA_TYPE ) {
				AnswerWork = TRUE;
				return;
			}
		}
	}
	AnswerWork = FALSE;
}




//--------------------------------------------------------------------
//							�ΎR�D���j���[
//--------------------------------------------------------------------
#define KAZANBAI_MENU_MAX	5		//�\�����鐔
#define KAZANBAI_MENU_MAX2	8		//���j���[�̐�

//�t���A�E�B���h�E���W
#define KAZANBAI_MENUWINX	(0)
#define KAZANBAI_MENUWINY	(0)
#define KAZANBAI_MENUWINX2	(KAZANBAI_MENUWINX+10)
#define KAZANBAI_MENUWINY2	(KAZANBAI_MENUWINY+(KAZANBAI_MENU_MAX*2)+1)

static const u8 * const KazanbaiMenuData[] = { 
	msg_aoibi, msg_kiirobi, msg_akaibi, msg_shiroibi, 
	msg_kuroibi, msg_kireinaisu, msg_kireinatukue, str_yameru }; 

void AddKazanbaiMenuTask();
void KazanbaiMenuTask(u8);
static u8 KazanbaiMenuWrite(u8,u8);
static void KazanbaiCsrSeq(u8,u8);
static void AddKazanbaiArrowU();
static void AddKazanbaiArrowD();


void AddKazanbaiMenuTask()
{
	u8 i;

	SetForceEvent();

	//�E�B���h�E�쐬
	NMenuWinBoxWrite( KAZANBAI_MENUWINX, KAZANBAI_MENUWINY, 
			KAZANBAI_MENUWINX2, KAZANBAI_MENUWINY2 );
	NMenuVCursorInit( KAZANBAI_MENUWINX+1, KAZANBAI_MENUWINY+1, KAZANBAI_MENU_MAX, 0);

	//�X�N���[���J�[�\���\��
	act_check = 0;
	//MarkActNoInit();
	//MarkPalInit();
	ScrActInit();
	AddMarkPal();
	AddKazanbaiArrowD();

	//���X�g�쐬
	for( i=0 ; i < KAZANBAI_MENU_MAX ; i++ )
	{
		NMenuMsgWrite( KazanbaiMenuData[i], 
				KAZANBAI_MENUWINX+2, KAZANBAI_MENUWINY+1+(i*2) );
	}

	floor_pos = 0;
	floor_max = KAZANBAI_MENU_MAX2;

	AddTask( KazanbaiMenuTask, 8 );
}

//--------------------------------------------------------------------
//
//					AnswerWork = �I�񂾗v�f�� = ���肵�� 
//							   = 127		  = �L�����Z������
//							
//--------------------------------------------------------------------
void KazanbaiMenuTask(u8 id)
{
	u8 csr_pos=0;

	if(sys.Trg == U_KEY)
	{
		if( floor_pos != 0 )
		{
			floor_pos--;
			csr_pos = NMenuVCursorPosGet();
			NMenuVCursorSetNoloop(-1);
			KazanbaiMenuWrite(csr_pos, U_KEY);
		}
	}

	if(sys.Trg == D_KEY)
	{
		if( floor_pos != (floor_max-1) )
		{
			floor_pos++;
			csr_pos = NMenuVCursorPosGet();
			NMenuVCursorSetNoloop(1);
			KazanbaiMenuWrite(csr_pos, D_KEY);
		}
	}
	
	if( sys.Trg & A_BUTTON )
	{
		AnswerWork = floor_pos;
		SePlay(SE_SELECT);
		DelArrowUD();
		NMenuBoxClear(0,0,29,12);
		ElevatorDel(id);
	}

	if( sys.Trg & B_BUTTON ){
		AnswerWork = 127;
		SePlay(SE_SELECT);
		DelArrowUD();
		NMenuBoxClear(0,0,29,12);
		ElevatorDel(id);
	}
}

//--------------------------------------------------------------------
//							�t���A���X�g�\��						
//
//	�߂�l	FALSE = ���������ĂȂ�
//			TRUE  = ����������
//--------------------------------------------------------------------
static u8 KazanbaiMenuWrite(u8 csr_pos, u8 key)
{
	u8 i,j,sw=FALSE,start=0;

	//���X�g�̐����`�F�b�N
	if( floor_max < KAZANBAI_MENU_MAX )	return FALSE;

	//��L�[�X�N���[��
	if( key == U_KEY )
	{
		if( csr_pos == 0 )
		{
			start = floor_pos;
			sw = TRUE;
		}
	
	//���L�[�X�N���[��
	}else if( key == D_KEY )
	{
		if( csr_pos == (KAZANBAI_MENU_MAX-1) )
		{
			//floor_pos����`��
			start = (floor_pos-csr_pos);
			sw = TRUE;
		}
	}

	//�t���A���X�g��������
	if( sw )
	{
		//�J�[�\������
		KazanbaiCsrSeq(start,KAZANBAI_MENU_MAX);

		//�󔒂ŕ`��
		NMenuBoxWrite( KAZANBAI_MENUWINX+2, KAZANBAI_MENUWINY+1, 
			KAZANBAI_MENUWINX2-1, KAZANBAI_MENUWINY2-1 );

		for( i=start,j=0 ; j < KAZANBAI_MENU_MAX ; i++,j++ )
		{
			//���X�g�`��
			NMenuMsgWrite( KazanbaiMenuData[i], 
				KAZANBAI_MENUWINX+2, KAZANBAI_MENUWINY+1+(j*2) );
		}
	}

	return sw;
}
//--------------------------------------------------------------------
//							�㕔�J�[�\���\��(�Ƃ肠����)
//--------------------------------------------------------------------
static void AddKazanbaiArrowU()
{
	//���łɕ\������Ă���
	if( (act_check >> 1) == 1 )	return;

	act_check |= 2;
	AddScrCurActor( MARK_ACT_U_CURSOR, ELEVATOR_MENUWINX+44, ELEVATOR_MENUWINY+8 );
	return;
}

//--------------------------------------------------------------------
//							�����J�[�\���\��(�Ƃ肠����)
//--------------------------------------------------------------------
static void AddKazanbaiArrowD()
{
	//���łɕ\������Ă���
	if( (act_check & 1) == 1 )	return;

	act_check |= 1;
	AddScrCurActor( MARK_ACT_D_CURSOR, ELEVATOR_MENUWINX+44, ELEVATOR_MENUWINY+(8*11) );
	return;
}

//--------------------------------------------------------------------
//							�J�[�\������
//--------------------------------------------------------------------
static void KazanbaiCsrSeq(u8 start, u8 max)
{
	//��Ԏn�߂�\�����Ă�����㕔�J�[�\�����폜����
	if( start == 0 )
	{
		act_check ^= 2;
		DelScrCurActor( MARK_ACT_U_CURSOR );
//		DelScrArrowAct(0);
	}

	//��Ԏn�߂�\�����Ă��Ȃ�������㕔�J�[�\����ǉ�����
	if( start > 0 )
	{
		AddKazanbaiArrowU();
	}

	//�Ō�܂ŕ\�����Ă��Ȃ������牺���J�[�\����ǉ�����
	if( (start+max) < floor_max )
	{
		AddKazanbaiArrowD();
		return;

	//�Ō�܂ŕ\�����Ă����牺���J�[�\�����폜����
	}else if( (start+max) == floor_max )
	{
		act_check ^= 1;
		DelScrCurActor( MARK_ACT_D_CURSOR );
//		DelScrArrowAct(1);
		return;
	}

	return;
}


//====================================================================
//						�}�b�v�X�N���[��
//====================================================================
void ScrMapScroolSet()
{
	u8 no;

	no = EvObjMakeSxyAdd( BOY1, MV_DOWN, DMY_OBJ_ID,
					Fld.MapXpos+MAP_X_MARGIN,Fld.MapYpos+MAP_Y_MARGIN,3 );

	EvObjNoBanishOn( no );		//�o�j�b�V���I��
	FldScrollActChg( EvObjActNo(no) );		//�X�N���[���A�N�^�[�ύX
	return;
}

void ScrMapScroolReset()
{
	FldScrollActChg( GetHeroActorNumber() );		//�X�N���[���A�N�^�[�ύX
	EvObjDelId( (u8)DMY_OBJ_ID, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id );
	return;
}	


//====================================================================
//	�|�P�����̐��i�ɂ���čD�݂̃|���b�N�����邱�Ƃ������Ă����
//====================================================================
u8 ScrGetLikeCube()
{
	u8 ans,seikaku;
	seikaku = PokeSeikakuGet( &PokeParaMine[GetTemotiBtlPokeNum()] );
	ans = GetLikeCubeName( seikaku, StrTempBuffer0 );
	return ans;
}


extern u8* CopyMapName(u8 *buf, u16 div, u16 size);			//wmap.c
//====================================================================
//				�����̔閧��n������}�b�v���擾
//====================================================================
void ScrGetMyBaseMapNo()
{
	CopyMapName( StrTempBuffer0, GetEventWorkValue(WK_BASE_MAPNO), 0);
	return;
}


//====================================================================
//					�o�g���^���[�̘A�����擾
//====================================================================
u16 BattleTowerRenshouGet();
u16 BattleTowerRenshouGet()
{
	return (u16)GetRecord(CNT_TOWER_RENSHOU);
}

//====================================================================
//					�J�[�he�g���[�i�[���O�擾
//====================================================================
void ScrBattleCardENameGet();
void ScrBattleCardENameGet()
{
	BattleCardENameGet( StrTempBuffer0 );
	return;
}

#define SLOT_TABLE_MAX	(12)
static const u8 slot_table_num[SLOT_TABLE_MAX] =	{ 12,2,4,5,1,8,7,11,3,10,9,6 };
static const u8 slot_atari_table[SLOT_TABLE_MAX] =	{ 0,1,1,2,2,2,3,3,3,4,4,5 };//�ʏ�p
static const u8 slot_atari_table2[SLOT_TABLE_MAX] =	{ 3,3,3,3,3,3,4,4,4,4,5,5 };//�J�W�m�f�[�p
//====================================================================
//						�X���b�g�̓������擾
//
//					ScriptParameter0�ɑ�̒l����
//====================================================================
u16 ScrSlotAtariGet();
u16 ScrSlotAtariGet()
{
	u32 ret = 0;
	ret = Fld.PopWord[0].now_power + Fld.PopWord[0].random + slot_table_num[ScriptParameter0];

	//�J�W�m�f�[���J�Â��Ă��邩
	if( CMEventStartChk( CM_CASINO_TYPE ) )
	{
		//�J�W�m�f�[
		return (u16)slot_atari_table2[ret % SLOT_TABLE_MAX];
	}else
	{
		//�ʏ�
		return (u16)slot_atari_table[ret % SLOT_TABLE_MAX];
	}
}

//====================================================================
//					���ʑD�̂P�����̃J�M�擾�`�F�b�N
//
//====================================================================
u8	Key1Check(void)
{
	ScriptParameter0 = 0xffff & ( (u32)sp1gousitunokagi_31 >> 16 );
	ScriptParameter0 += FH_FLAG_START;
	if ( EventFlagCheck(ScriptParameter0) == FALSE ) return FALSE;

	return TRUE;
}

//====================================================================
//					���ʑD�̂Q�����̃J�M�擾�`�F�b�N
//
//====================================================================
u8	Key2Check(void)
{
	ScriptParameter0 = 0xffff & ( (u32)sp2gousitunokagi_32 >> 16 );
	ScriptParameter0 += FH_FLAG_START;
	if ( EventFlagCheck(ScriptParameter0) == FALSE ) return FALSE;


	return TRUE;
}

//====================================================================
//					���ʑD�̂S�����̃J�M�擾�`�F�b�N
//
//====================================================================
u8	Key4Check(void)
{
	ScriptParameter0 = 0xffff & ( (u32)sp4gousitunokagi_33 >> 16 );
	ScriptParameter0 += FH_FLAG_START;
	if ( EventFlagCheck(ScriptParameter0) == FALSE ) return FALSE;

	return TRUE;
}

//====================================================================
//					���ʑD�̂U�����̃J�M�擾�`�F�b�N
//
//====================================================================
u8	Key6Check(void)
{
	ScriptParameter0 = 0xffff & ( (u32)sp6gousitunokagi_34 >> 16 );
	ScriptParameter0 += FH_FLAG_START;
	if ( EventFlagCheck(ScriptParameter0) == FALSE ) return FALSE;

	return TRUE;
}

//====================================================================
//							����΃��{���֘A
//====================================================================
u8	ChkGanbaRibbon(void);
void SetGanbaRibbon(void);
u8 ChkGanbaRibbonParaExp(void);

//--------------------------------------------------------------------
//				�莝���̐擪�̃|�P�������u����΃��{���v��
//					�����Ă��邩�̃`�F�b�N
//--------------------------------------------------------------------
u8 ChkGanbaRibbon(void)
{
	return PokeParaGet( &PokeParaMine[GetTemotiBtlPokeNum()], ID_ganba_ribbon, 0 );
}

//--------------------------------------------------------------------
//			�莝���̐擪�̃|�P�����Ɂu����΃��{���v������
//--------------------------------------------------------------------
void SetGanbaRibbon(void)
{
	u8 ribbon;

	//�L�^�J�E���g�A�b�v
	IncRecord( CNT_RIBBON );

	//���{�������
	EventFlagSet( SYS_RIBBON_GET );
	
	ribbon = 1;
	PokeParaPut( &PokeParaMine[GetTemotiBtlPokeNum()], ID_ganba_ribbon, &ribbon );
	return;
}

//--------------------------------------------------------------------
//				�莝���̐擪�̃|�P�����̓w�͒l�̍��v���擾����
//				�}�b�N�X�����`�F�b�N����
//
//	�߂�l	TRUE  = MAX
//			FALSE = MAX�ł͂Ȃ�
//--------------------------------------------------------------------
u8 ChkGanbaRibbonParaExp(void)
{
	u16 ret;
	ret = ParaExpTotalGet( &PokeParaMine[GetTemotiBtlPokeNum()] );

	if( ret >= PARA_EXP_TOTAL_MAX )	return TRUE;
	else return FALSE;
}

//--------------------------------------------------------------------
//			�J�i�V�_�g���l���Ŋ�ӂ����g�p���Ă��邩�`�F�b�N
//
// 	WK_SCENE_CAVE_D0201 = 4 = �����̊�ӂ���
//	WK_SCENE_CAVE_D0201 = 5 = �㑤�̊�ӂ���
//
//	�߂�l	TRUE  = ����
//			FALSE = �������Ȃ�
//--------------------------------------------------------------------
u8 ChkKanasidaIwa(void);
u8 ChkKanasidaIwa(void)
{
	//���łɃJ�i�V�_�g���l���J�ʂ��Ă�����
	if( EventFlagCheck( FE_KANASIDA_GO ) ) return FALSE;

	if( Fld.MapID.div_id == CAVE && Fld.MapID.map_id == CAVE_D0201 )
	{
		if( EventFlagCheck( FV_IWA_01_CAVE_D0201 ) )
		{
			SetEventWorkValue( WK_SCENE_CAVE_D0201,4 );
			return TRUE;
		}

		if( EventFlagCheck( FV_IWA_02_CAVE_D0201 ) ) 
		{
			SetEventWorkValue( WK_SCENE_CAVE_D0201,5 );
			return TRUE;
		}
	}
	
	return FALSE;
}


//====================================================================
//
//							�󐣂̓��A
//
//====================================================================
void Asase_Update(u16);

//--------------------------------------------------------------------
//							�����X�V
//
//		����ȏ�o�߂��Ă�����A�C�e�������ł���悤�ɂ���
//--------------------------------------------------------------------
void Asase_Update( u16 diff )
{
	//����ȏ�o�߂����I
	EventFlagSet( SYS_ASASE_ITEM );
	return;
}


//--------------------------------------------------------------------
//		�����~�c���ɑ݂��|�P���������莝���̐擪�ɃZ�b�g����	
//		�Ăяo���O�Ɏ莝���|�P������ޔ������Ă���
//--------------------------------------------------------------------
void ScrMitsuruPokeSet()
{
	u16 ans;

	PokeParaSet(&PokeParaMine[0], MONSNO_ZIGUZAGUMA, 7, 
			POW_RND,RND_NO_SET, 0, ID_NO_SET, 0);

	ans = 1;
	PokeParaPut( &PokeParaMine[0], ID_speabi, (u8 *)&ans );

	ans = WAZANO_TAIATARI;
	PokeParaPut( &PokeParaMine[0], ID_waza1,  (u8 *)&ans );
	ans = 0;
	PokeParaPut( &PokeParaMine[0], ID_waza2,  (u8 *)&ans );
	PokeParaPut( &PokeParaMine[0], ID_waza3,  (u8 *)&ans );
	PokeParaPut( &PokeParaMine[0], ID_waza4,  (u8 *)&ans );

	return;
}

//--------------------------------------------------------------------
//		���߂Ď�ɓ��ꂽ�|�P�������莝���ɂ��邩�`�F�b�N
//
//	�߂�l	TRUE  = ����
//			FALSE = ���Ȃ�
//--------------------------------------------------------------------
extern u16	FirstPokemonNumGet(u16	index);
u16 ScrFirstPokeNumChk();
u16 ScrFirstPokeNumChk()
{
	u8 i,count;
	u16 ret = 0;
	ret = FirstPokemonNumGet( GetEventWorkValue( WK_FIRST_POKE ) );
	count = PokeCountMineGet();

	for( i=0; i < count ;i++ )
	{
		if( PokeParaGet( &PokeParaMine[i], ID_monsno_egg, 0 ) == ret )
			return TRUE;
	}

	return FALSE;
}



//--------------------------------------------------------------------
//		�{�b�N�X�ɋ󂫂����邩
//
//	�߂�l	TRUE  = ����
//			FALSE = �Ȃ�
//--------------------------------------------------------------------
u8	BoxSpacePokemonCheck(void)
{
	u16 i,j;

	for(i=0;i<BOX_KAZU;i++){
		for(j=0;j<BOX_PM_KAZU;j++){
			if(PokePasoParaGet(&BoxData.PPPBox[i][j],ID_monsno,0)==0){
				return TRUE;
			}
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------
//�莝���̃|�P�����̒��Ƀ|�P���X�Ɋ������Ă���|�P���������邩�̃`�F�b�N
//
//	�߂�l	TRUE  = ����
//			FALSE = �Ȃ�
//--------------------------------------------------------------------
u8	ScrTemotiPokerusCheck(void)
{
	//00111111 = 0x3f = �U�̕��`�F�b�N
	if( PokerusCheck( &PokeParaMine[0], 0x3f ) == 0 )	return FALSE;
	else	return TRUE;
}


//====================================================================
//							��ʗh�炵
//====================================================================
void AddKodaiMoveTask();
void KodaiMoveTask(u8 id);
void KodaiDel(u8 id);

#define KODAI_SHAKE_NUM 8				//�h�炷��
#define KODAI_SHAKE_WAIT 5				//�h�炷�Ԋu
//--------------------------------------------------------------------
//							�Ăяo��
//--------------------------------------------------------------------
void AddKodaiMoveTask()
{
	u8 no;
	no = AddTask( KodaiMoveTask, 9 );
	TaskTable[no].work[0] = ScriptParameter1;	//��ꕝY
	TaskTable[no].work[1] = 0;					//�E�F�C�g�J�E���^
	TaskTable[no].work[2] = 0;					//
	TaskTable[no].work[3] = 0;					//�J�n�E�҂��E�I��
	TaskTable[no].work[4] = ScriptParameter0;	//��ꕝX
	TaskTable[no].work[5] = KODAI_SHAKE_WAIT;	//�h�炷�Ԋu
	SetMapViewFunc( NULL );
	SePlay(SE_W070);
}

//--------------------------------------------------------------------
//							�h�炵����
//--------------------------------------------------------------------
void KodaiMoveTask(u8 id)
{
	TASK_TABLE * task_p;

	task_p = &TaskTable[id];
	
	task_p->work[1]++;

	if( (task_p->work[1] % task_p->work[5]) == 0 )
	{
		task_p->work[1] = 0;
		task_p->work[2]++;

		switch( task_p->work[3] )
		{
			case 0:
				task_p->work[0] = -task_p->work[0];
				task_p->work[4] = -task_p->work[4];
				SetMapViewOffset( task_p->work[0], task_p->work[4] );
				if( task_p->work[2] == KODAI_SHAKE_NUM )
				{
					KodaiDel(id);
					MapViewInit();
				}
				break;
		}
	}

	return;
}

//--------------------------------------------------------------------
//							�I������
//--------------------------------------------------------------------
void KodaiDel(u8 id)
{
	DelTask(id);
	ContinueFieldScript();
	return;
}

//--------------------------------------------------------------------
//					�B���́u���낢���K�l�v���肵�Ă��邩	
//--------------------------------------------------------------------
#define HIDE_KUROIMEGANE_NO		96		//FH_FLAG_START�̃I�t�Z�b�g
u16 ChkHideKuroiMegane();
u16 ChkHideKuroiMegane()
{
	return EventFlagCheck( FH_FLAG_START + HIDE_KUROIMEGANE_NO );
}

//--------------------------------------------------------------------
//						���[�h�P�P�X�̓V�󑀍�
//--------------------------------------------------------------------
void SetWeatherR119();
void SetWeatherR119()
{
	/* �t�B�[���h����ړ����Ă����ꍇ�͉������Ȃ� */
	if ( MTYPE_IsFieldCheck(GetBeforeMapType()) == TRUE ) return;

	//�����ω��V��R�[�h�Z�b�g
	SetFieldWeatherValue( ID_WEATHER_SRTR );
	return;
}

//--------------------------------------------------------------------
//						���[�h�P�Q�R�̓V�󑀍�
//--------------------------------------------------------------------
void SetWeatherR123();
void SetWeatherR123()
{
	/* �t�B�[���h����ړ����Ă����ꍇ�͉������Ȃ� */
	if ( MTYPE_IsFieldCheck(GetBeforeMapType()) == TRUE ) return;

	//�����ω��V��R�[�h�Z�b�g
	SetFieldWeatherValue( ID_WEATHER_SSRS );
	return;
}


//--------------------------------------------------------------------
//				�莝���̐擪����^�}�S�ł͂Ȃ��ꏊ��Ԃ�
//--------------------------------------------------------------------
u8 GetTemotiBtlPokeNum();
u8 GetTemotiBtlPokeNum()
{
	u8 i,count;
	count = PokeCountMineGet();

	for( i=0; i < count ;i++ )
	{
		if( PokeParaGet( &PokeParaMine[i], ID_monsno_egg, 0 ) != MONSNO_TAMAGO &&
				PokeParaGet( &PokeParaMine[i], ID_monsno_egg, 0 ) != 0 )
			return i;
	}

	return 0;
}


//--------------------------------------------------------------------
//			���X�g����I�񂾃i���o�[�̃|�P�����i���o�[��Ԃ�
//--------------------------------------------------------------------
u16 GetListNoPokeNo();
u16 GetListNoPokeNo()
{
	return PokeParaGet( &PokeParaMine[ScriptParameter0], ID_monsno_egg, 0 );
}


//--------------------------------------------------------------------
//					�o�g���^���[�̃I�[�i�[�ǉ�����
//--------------------------------------------------------------------
void AddTowerOwner();
void AddTowerOwner()
{
	EvObjWorkAdd( WORKERM1_01_PARK_BATTLE01 );
	return;
}


//--------------------------------------------------------------------
//		�A�C�e������肵�Ă���P�T�Ԉȏソ���������擾����
//
//	�߂�l	0	  = ��T�Ԉȏソ���Ă���
//			1 - 6 = �㉽���ň�T�Ԃ���
//--------------------------------------------------------------------
u16 ScrGetOneWeekFromWazaGet();
u16 ScrGetOneWeekFromWazaGet()
{
	u16 ret;
	ret = GetEventWorkValue( WK_SCENE_T107_R0201 );

	//��T�Ԉȏソ���Ă��邩
	if( (GameTime.day - ret) >= 7 )
	{
		return 0;
	}

	//�}�C�i�X�̒l���o���Ƃ�
	if( GameTime.day < 0 )
	{
		//��΂ɂ��肦�Ȃ���
		return 8;
	}

	//�㉽������Ԃ�
	return ( 7 - (GameTime.day - ret) );
}

//--------------------------------------------------------------------
//				�A�C�e������肵�����ɂ����Z�b�g����
//--------------------------------------------------------------------
u16 ScrSetWazaGetGameDays();
u16 ScrSetWazaGetGameDays()
{
	SetEventWorkValue( WK_SCENE_T107_R0201, GameTime.day );
	return GameTime.day;
}

//--------------------------------------------------------------------
//							�e����r
//							
//	�߂�l	0=�e������
//			1=�e���Ⴄ
//--------------------------------------------------------------------
u8 ScrOyaNameCmp(u8);
u8 ScrOyaNameCmp(u8 no)
{
	//StrTempBuffer0�Ɋi�[����
	PokeParaGet( &PokeParaMine[ScriptParameter0], ID_oyaname, StrTempBuffer0 );

	if( PM_strcmp( (u8 *)&MyData.my_name[0], StrTempBuffer0 ) == 0 )
	{
		return 0;
	}

	return 1;
}

//--------------------------------------------------------------------
//					������i���o�[�̕�����擾
//--------------------------------------------------------------------
void ScrPokeLotNumGet();
void ScrPokeLotNumGet()
{
	if( AnswerWork >= 10000 )
	{
		//99999-10000
		SetNumSelectBuf( 0, (int)AnswerWork );
	}else if( AnswerWork >= 1000 )
	{
		//9999-1000
		StrTempBuffer0[0] = n0_;
		PM_NumMsgSet( &StrTempBuffer0[1], (int)AnswerWork, 
				NUM_MODE_LEFT, GetNumKeta((int)AnswerWork) );
	}else if( AnswerWork >= 100 )
	{
		//999-100
		StrTempBuffer0[0] = n0_;
		StrTempBuffer0[1] = n0_;
		PM_NumMsgSet( &StrTempBuffer0[2], (int)AnswerWork, 
				NUM_MODE_LEFT, GetNumKeta((int)AnswerWork) );
	}else if( AnswerWork >= 10 )
	{
		//99-10
		StrTempBuffer0[0] = n0_;
		StrTempBuffer0[1] = n0_;
		StrTempBuffer0[2] = n0_;
		PM_NumMsgSet( &StrTempBuffer0[3], (int)AnswerWork, 
				NUM_MODE_LEFT, GetNumKeta((int)AnswerWork) );
	}else
	{
		//9-0
		StrTempBuffer0[0] = n0_;
		StrTempBuffer0[1] = n0_;
		StrTempBuffer0[2] = n0_;
		StrTempBuffer0[3] = n0_;
		PM_NumMsgSet( &StrTempBuffer0[4], (int)AnswerWork, 
				NUM_MODE_LEFT, GetNumKeta((int)AnswerWork) );
	}

	return;
}


