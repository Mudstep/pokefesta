//=========================================================================
//
//
//	�t�B�[���h�a�f�l����֘A
//
//	2001.12.27	GAME FREAK Inc.
//
//=========================================================================

#include "common.h"
#include "task.h"

#define __FLD_BGM_H_GLOBAL__
#include "fld_bgm.h"

#include "fieldmap.h"
#include "mapparam.h"
#include "mus_tool.h"
#include "fld_tool.h"
#include "map_attr.h"
#include "script.h"
#include "calctool.h"
#include "sysflag.h"
#include "syswork.h"
#include "weather.h"
#include "wea_ctrl.h"
#include "../evdata/maparray.h"
#include "ev_sub.h"
#include "jiki.h"
#include "ecnt_dat.h"
#include "scr_tool.h"
#include "../script/savework.h"


#if	(PM_VERSION == VERSION_SAPPHIRE)
//�T�t�@�C�A�o�[�W�����̂Ƃ��A��J��BGM
#define	SP_MUS_WEATHER		MUS_OOAME
#elif	(PM_VERSION == VERSION_RUBY)
//���r�[�o�[�W�����̂Ƃ��A���Ƃ��BGM
#define	SP_MUS_WEATHER		MUS_HIDERI
#endif

enum{
	//�V�C�������苒����BGM
	SP_MUS_OCCUPATION	=	MUS_TOZAN,

	//R118�A���ݑ���BGM
	SP_MUS_R118_LEFT	=	MUS_DOORO_X1,

	//R118�A�E�ݑ���BGM
	SP_MUS_R118_RIGHT	=	MUS_GRANROAD,
};

//==============================================================
//
//	�T�E���h�Ăяo��
//
//==============================================================

//��J�C�x���g�̃}�b�v���H
static u16 is_rain_event_map( MapID * id )
{
	if ( EventFlagCheck( SYS_WEATHER_CTRL ) == FALSE ) return FALSE;

	if ( id->div_id != FIELD ) return FALSE;

	switch ( id->map_id ) {
	case FIELD_C106:
	case FIELD_C107:
	case FIELD_C108:
	case FIELD_C109:
	case FIELD_R124:
	case FIELD_R125:
	case FIELD_R126:
	case FIELD_R127:
	case FIELD_R128:
		return TRUE;
	}
	return FALSE;
}

static u16 is_occupation_event_map(MapID * id)
{
	//�苒�C�x���g�I����͒ʏ��BGM
	if (GetEventWorkValue(WK_SCENE_R119_TENKI) != 0)
		return FALSE;

	if (id->div_id != R119)
		return FALSE;

	if (id->map_id == R119_R0101 || id->map_id == R119_R0102)
		return TRUE;

	return FALSE;
}

static u16 get_map_bgm( MapID * id )
{
	const MapTotalData * p;

	if ( is_rain_event_map(id) == TRUE ) {
		return SP_MUS_WEATHER;
	}
	else if (is_occupation_event_map(id) == TRUE) {
		return SP_MUS_OCCUPATION;
	} else {
		p = GetTotalMapData( id->div_id, id->map_id );
		return p->BGMNo;
	}
}


static u16 get_nowmap_bgm( void )
{
	u16 bgm;

	if (Fld.MapID.div_id == FIELD && Fld.MapID.map_id == FIELD_R111 &&
		GetFieldWeatherValue() == WEATHER_STORM) {
		return MUS_ASHROAD;
	}

	bgm = get_map_bgm( &Fld.MapID );
	if ( bgm != SP_MUS_ROAD118 ) return bgm;

	/* ��̍��E�ǂ���ɂ��邩�𔻒f����BGM��ύX���Ă��� */
	if ( Fld.MapXpos < 24 ) {
		return SP_MUS_R118_LEFT;
	} else {
		return SP_MUS_R118_RIGHT;
	}
}

static u16 get_nextmap_bgm( void )
{
	u16 bgm;
	bgm = get_map_bgm( &NextMapID );
	if ( bgm != SP_MUS_ROAD118 ) return bgm;

	/* ���݂̃}�b�v���L���Z�c�̏ꍇ�̂݁A���ݑ���BGM��Ԃ��Ă��� */
	if (Fld.MapID.div_id == FIELD && Fld.MapID.map_id == FIELD_C103) {
		return SP_MUS_R118_LEFT;
	} else {
		return SP_MUS_R118_RIGHT;
	}
}

//-------------------------------------------------------------
//	�t�B�[���h�a�f�l������
//-------------------------------------------------------------
void MapBGM_Init(void)
{
	BGM_Init();
}

//-------------------------------------------------------------
//	�t�B�[���h�a�f�l�J�n
//	���}�b�v�E�t���O�Ȃǂ𔻒f���ēK�؂Ȃa�f�l�����t����
//	�����݂̂a�f�l�����t���悤�Ƃ��Ă���a�f�l�Ɠ����ꍇ��
//	�@�������Ȃ�
//-------------------------------------------------------------
void MapBGM_Start(void)
{
	u16 now;
	now = get_nowmap_bgm();

	if ( now != SP_MUS_WEATHER ) {
		if ( Fld.MapMusicNo != 0 ) {
			now = Fld.MapMusicNo;
		}
		else if ( GetNowMapType() == MAPTYPE_WATER ) {
			now = MUS_DEEPDEEP;
		}
		else if (CheckHeroState(MY_MV_BIT_SWIM)) {
			now = MUS_NAMINORI;
		}
	}
	if( now == BGM_GetNowNo() ) return;

	BGM_PlaySet( now );
}

//-------------------------------------------------------------
//	�a�f�l�w��Z�b�g
//	���}�b�v������̂a�f�l�w�肪�Z�b�g�����
//	�@���]�Ԃa�f�l�̐���ȂǂɎg�p
//-------------------------------------------------------------
void MapBGM_SetSpecial( u16 music )
{
	Fld.MapMusicNo = music;
}

//-------------------------------------------------------------
//	�a�f�l�w��N���A
//	���}�b�v������̂a�f�l�w�肪�N���A�����
//	�@���]�Ԃa�f�l�̐���ȂǂɎg�p
//-------------------------------------------------------------
void MapBGM_ClearSpecial( void )
{
	Fld.MapMusicNo = 0;
}

//==============================================================
//
//	BGM�t�F�[�h����֘A
//
//==============================================================

//-------------------------------------------------------------
//	�a�f�l�t�F�[�h�����̂a�f�l�֕ύX
//	���t�B�[���h��ł̗אڃ}�b�v�ړ��ŌĂ΂��
//	�����̃}�b�v�ƍ��̃}�b�v��BGM�������ꍇ�@�\���Ȃ�
//-------------------------------------------------------------
void MapBGM_FadeNextSet(void)
{
	u16 next,now;

	if (EventFlagCheck(CTRL_BGM_CHANGE) == TRUE)
		return;

	next = get_nextmap_bgm();
	now = BGM_GetNowNo();
	if (next != SP_MUS_WEATHER) {
		if (now == MUS_DEEPDEEP)
			return;
		if (now == MUS_NAMINORI)
			return;
		if (CheckHeroState(MY_MV_BIT_SWIM)) {
			next = MUS_NAMINORI;
		}
	}

	if (next == now) return;

	if (CheckHeroState(MY_MV_BIT_CYCLE_AND)) {
		//���]�Ԃɏ���Ă���ꍇ�̓t�F�[�h�A�E�g���t�F�[�h�C��
		BGM_FadeNextFadeSet( next, BGMFADE_1SEC, BGMFADE_1SEC );
	} else {
		//�ʏ펞�̓t�F�[�h�A�E�g���J�n
		BGM_FadeNextSet( next, BGMFADE_2SEC );
	}
}

//-------------------------------------------------------------
//	�a�f�l�t�F�[�h�����̃}�b�v�̂a�f�l�֕ύX
//	�������Ȃ̏ꍇ�͋@�\���Ȃ�
//-------------------------------------------------------------
void MapBGM_FadeNowSet(void)
{
	u16 now;
	now = BGM_GetNowNo();
	if ( now == get_nowmap_bgm() ) return;

	BGM_FadeNextSet( get_nowmap_bgm(), BGMFADE_2SEC );
}

//-------------------------------------------------------------
//	�a�f�l�t�F�[�h���w��a�f�l�ɕύX
//	�������Ȃ̏ꍇ�͋@�\���Ȃ�
//	<����>	�a�f�l�̔ԍ�
//-------------------------------------------------------------
void MapBGM_FadeChange( u16 music )
{
	u16 now;
	now = BGM_GetNowNo();
	if( now == music ) return;
	if( now == SP_MUS_WEATHER ) return;	//�C�x���g���͋Ȃ��ς��Ȃ�

	BGM_FadeNextSet( music, BGMFADE_2SEC );
}

//-------------------------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g
//	�����̃}�b�v�ƍ��̃}�b�v��BGM�������ꍇ�@�\���Ȃ�
//-------------------------------------------------------------
//	���A�A�t�B�[���h�ւ̑J�ڂ�2�b
//	�����A�|�P�Z���ւ̑J�ڂ�1�b

static u8 get_bgm_fade_len( void )
{
	u8 type;

	type = (GetNextMapData())->type;
	if( MTYPE_IsIndoorCheck(type) == TRUE )
		return BGMFADE_HALF_SEC;
	else
		return BGMFADE_1SEC;
}

void MapBGM_FadeOutSet( void )
{
	u16 next;
	next = get_nextmap_bgm();

	if(EventFlagCheck(CTRL_BGM_CHANGE) == TRUE)return;

	if( next == BGM_GetNowNo() ) return;

	BGM_FadeOutSet( get_bgm_fade_len() );
}

//-------------------------------------------------------------
//	�a�f�l�t�F�[�h�҂�
//	<�߂�l>	TRUE	�I��
//				FALSE	�t�F�[�h��
//-------------------------------------------------------------
u8 MapBGM_FadeWait( void )
{
	return BGM_FadeOutCheck();
}

//-------------------------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g���I���Z�b�g
//-------------------------------------------------------------
void MapBGM_FadeOutEndSet(void)
{
	BGM_FadeOutSet(BGMFADE_1SEC);
}

//==============================================================
//
//	�t�B�[���h��������
//
//==============================================================
extern void VoicePlay2(u16 no, s8 pan, s8 vol, u8 pri);
extern u16 GetEncountVoice( u8 * water_flag );

#define	ONE_SECOND	60

// �����̃|�P�����i���o�[
static u16 FieldVoiceNo = 0;

// �G���J�E���g�f�[�^�����������ǂ����̃t���O
// (�������̏ꍇTRUE���������Ă���j
static u8 FieldVoiceType = 0;


static const int VOICE_FIRST_WAIT_MIN = 20 * ONE_SECOND;
static const int VOICE_FIRST_WAIT_MAX = 60 * ONE_SECOND;

static const int VOICE_WAIT_MIN = 20 * ONE_SECOND;
static const int VOICE_WAIT_MAX = 40 * ONE_SECOND;

static const int VOICE_VOLUME_MIN = 50;
static const int VOICE_VOLUME_MAX = 80;

static const int VOICE_PAN_MIN = -44;
static const int VOICE_PAN_MAX = 44;

enum{
	VWK_SEQ = 0,
	VWK_WAIT,
};

enum{
	VSEQ_INIT = 0,
	VSEQ_FIRST_SET,
	VSEQ_SET,
	VSEQ_WAIT,
	VSEQ_SLEEP,
};


//-------------------------------------------------------------
//	�����Z�b�g
//-------------------------------------------------------------
static void field_voice_play( void )
{
	s16 x,y;
	s8 pan,vol;
	GetHeroCellPos( &x, &y );
	if ( FieldVoiceType == TRUE &&
			ATR_IsWaterCheck(GetCellAttribute(x,y)) == FALSE ) {
		//���̃G���J�E���g�����Ȃ��n�`�ő��������łȂ��ꍇ�͖��Ȃ�
		return;
	}
	pan = VOICE_PAN_MIN + pp_rand() % (VOICE_PAN_MAX-VOICE_PAN_MIN);
	vol = VOICE_VOLUME_MIN + pp_rand() % (VOICE_VOLUME_MAX-VOICE_VOLUME_MIN);
	VoicePlay2(FieldVoiceNo,pan,vol,1);
}

//-------------------------------------------------------------
//	�t�B�[���h��������{��
//	<����>	seq		����V�[�P���X���[�N
//			wait	�E�F�C�g���[�N
//-------------------------------------------------------------
void FieldVoiceControl( s16 * seq, s16 * wait )
{
	switch ( *seq ) {

	case VSEQ_INIT:
		if ( FieldVoiceNo == 0 ) {
			*seq = VSEQ_SLEEP;
		} else {
			*seq = VSEQ_FIRST_SET;
		}
		break;

	case VSEQ_FIRST_SET:
		*wait = VOICE_FIRST_WAIT_MIN +
			pp_rand()%(VOICE_FIRST_WAIT_MAX-VOICE_FIRST_WAIT_MIN);
		*seq = VSEQ_WAIT;
		break;

	case VSEQ_SET:
		*wait = VOICE_WAIT_MIN + pp_rand()%(VOICE_WAIT_MAX-VOICE_WAIT_MIN);
		*seq = VSEQ_WAIT;
		break;

	case VSEQ_WAIT:
		(*wait) --;
		if ( *wait == 0 ) {
			field_voice_play();
			*seq = VSEQ_SET;
		}
		break;

	case VSEQ_SLEEP:
		break;
	}
}

//-------------------------------------------------------------
//	�����f�[�^�̏�����
//-------------------------------------------------------------
void InitFieldVoice(void)
{
	//�}�{���V�����o�����Ă���Ƃ��ȊO�͐��G���J�E���g�݂̂��Q�Ƃ���
	if (Fld.MapID.div_id == FIELD &&
			Fld.MapID.map_id == FIELD_R130 &&
			Maboroshi_Check() == FALSE)
	{
		FieldVoiceType = TRUE;					//����G���J�E���g�Ȃ̂�
		FieldVoiceNo = GetWaterEncountVoice();
	} else {
		FieldVoiceNo = GetEncountVoice( &FieldVoiceType );
	}
}

