//==============================================================
//
//	�C�x���g�⏕�֘A
//
//
//==============================================================

#include "common.h"

#define EV_SUB_H_GLOBAL
#include "ev_sub.h"

#include "fld_tool.h"
//#include "weather.h"
#include "wea_ctrl.h"
#include "map_attr.h"
#include "task.h"
#include "fieldmap.h"
#include "mapbg.h"
#include "jiki.h"
#include "syswork.h"
#include "script.h"
#include "mus_tool.h"
#include "fld_task.h"
#include "item.h"
#include "ev_time.h"
#include "fld_bgm.h"

//==============================================================
//
//	���W�ɂ��v���O�����Ăяo��
//	���X�N���v�g�ƈႢ�A�����C�x���g���[�h�ɓ���Ȃ�
//
//==============================================================

//-------------------------------------------------------------
//	�V��ω�
//-------------------------------------------------------------
static void set_weather_shine(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_SHINE );
}
static void set_weather_shine2(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_SHINE2 );
}
static void set_weather_rain(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_RAIN );
}
static void set_weather_snow(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_SNOW );
}
static void set_weather_thunder(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_THUNDER );
}
static void set_weather_mist(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_MIST );
}
static void set_weather_mist2(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_MIST2 );
}
static void set_weather_ash(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_ASH );
}
static void set_weather_storm(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_STORM );
}
static void set_weather_cloud(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_CLOUD );
}
static void set_weather_shine3(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_SHINE3 );
}

static void set_weather_srtr(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_SRTR );
}
static void set_weather_ssrs(void)
{
	ChangeFieldWeatherSmooth( ID_WEATHER_SSRS );
}

//-------------------------------------------------------------
//	�v���O�����o�^�e�[�u��
//-------------------------------------------------------------
static const struct{
	u8 id;
	pFunc func;
} func_tbl[] =
{
	{ SHINE_CTRL,		set_weather_shine	},
	{ SHINE2_CTRL,		set_weather_shine2	},
	{ RAIN_CTRL,		set_weather_rain	},
	{ SNOW_CTRL,		set_weather_snow	},
	{ THUNDER_CTRL,		set_weather_thunder	},
	{ MIST_CTRL,		set_weather_mist	},
	{ MIST2_CTRL,		set_weather_mist2	},
	{ ASH_CTRL,			set_weather_ash		},
	{ STORM_CTRL,		set_weather_storm	},
	{ CLOUD_CTRL,		set_weather_cloud	},
	{ SHINE3_CTRL,		set_weather_shine3	},
	{ SRTR_CTRL,		set_weather_srtr	},
	{ SSRS_CTRL,		set_weather_ssrs	},
};

/* �e�[�u���̗v�f�� */
#define	POS_FUNC_MAX	NELEMS(func_tbl)

//-------------------------------------------------------------
//�w�肳�ꂽ�v���O�������Ăяo��
//-------------------------------------------------------------
void EventPosProgramCall( u8 id )
{
	u8 i;

	for( i=0; i < POS_FUNC_MAX ;i++ )
	{
		if( func_tbl[i].id == id )
		{
			(func_tbl[i].func)();
			return;
		}
	}
}

//==============================================================
//
//
//	�}�b�v�ŗL�G�t�F�N�g���䃁�C���V�X�e��
//
//
//==============================================================

static const pTaskFunc effect_task_table[];
static void dummy_effect_task(u8 no);

static void slide_slope_task( u8 no );
static void map_control_task( u8 no );

// �ŗL�G�t�F�N�g��`
static void ash_grass_effect_task(u8 no);
static void hang_bridge_effect_task( u8 no );
static void log_effect_task( u8 no );
static void crash_ice_task( u8 no );
static void break_floor_task( u8 no );

extern void CargoControlTask( u8 no );

extern void BaseGoodsTrap(u8);

//-------------------------------------------------------------
//	�}�b�v�ŗL�G�t�F�N�g�{��
//-------------------------------------------------------------
void field_effect_task( u8 no )
{
	int id = TaskTable[no].work[0];
	(effect_task_table[id])(no);
}

//-------------------------------------------------------------
//	�}�b�v�ŗL�G�t�F�N�g�e�[�u��
//-------------------------------------------------------------
static const pTaskFunc effect_task_table[] =
{
	dummy_effect_task,
	ash_grass_effect_task,
	hang_bridge_effect_task,
	log_effect_task,
	crash_ice_task,
	CargoControlTask,
	BaseGoodsTrap,
	break_floor_task,
};

//-------------------------------------------------------------
//	����p�^�X�N
//-------------------------------------------------------------
enum{
	CTRLWK_SEED_SEQ = 0,
	CTRLWK_VOICE_SEQ,
	CTRLWK_VOICE_WAIT,
};

enum{
	TIME_CYCLE_VALUE = 0x1000,
};

static void update_time_event( s16 * seq )
{
	switch ( *seq ) {
	case 0:
		if (sys.main_timer & TIME_CYCLE_VALUE) {
			UpdateTimeEvent();
			(*seq)++;
		}
		break;

	case 1:
		if ((sys.main_timer & TIME_CYCLE_VALUE) == 0) {
			(*seq)--;
		}
		break;
	}
}

static void map_control_task( u8 no )
{
	s16 * wk = TaskTable[no].work;
	if ( CheckForceEvent() == FALSE ) {
		update_time_event( &(wk[CTRLWK_SEED_SEQ]) );
		FieldVoiceControl( &wk[CTRLWK_VOICE_SEQ], &wk[CTRLWK_VOICE_WAIT] );
	}
}


//-------------------------------------------------------------
//	�}�b�v�ŗL�G�t�F�N�g����̏�����
//-------------------------------------------------------------
void InitMapEffectControl( void )
{
	u8 no;

	if ( CheckTask(field_effect_task) == FALSE ) {
		no = AddTask( field_effect_task, TSK_PRI_EFFECT );
		TaskTable[no].work[0] = MAP_EFFECT_NOTHING;
	}

	if ( CheckTask(slide_slope_task) == FALSE ) {
		AddTask( slide_slope_task,TSK_PRI_EFFECT );
	}

	if ( CheckTask(map_control_task) == FALSE ) {
		AddTask( map_control_task,TSK_PRI_EFFECT );
	}
}

//-------------------------------------------------------------
//	�}�b�v�ŗL�G�t�F�N�g�����ID�ݒ�
//	<����>	id		�}�b�v�ŗL�G�t�F�N�g��ID
//-------------------------------------------------------------
void SetMapEffectControl( u8 id )
{
	u8 no;
	int i;
	s16 * wk;

	no = CheckTaskNo(field_effect_task);
	if ( no == NOT_EXIST_TASK_ID ) return;
	wk = TaskTable[no].work;
	for ( i=0;i<TASK_WORK_MAX;i++ ) {
		wk[i] = 0;
	}

	if ( id >= NELEMS(effect_task_table) ) {
		wk[0] = MAP_EFFECT_NOTHING;
	} else {
		wk[0] = id;
	}
}

//-------------------------------------------------------------
//	�}�b�v�ŗL�G�t�F�N�g�̃}�b�v�؂�ւ��ɂ��X�V
//-------------------------------------------------------------
void UpdateMapEffectControl( void )
{
	u8 no;
	s16 * wk;
	no = CheckTaskNo(field_effect_task);
	if ( no != NOT_EXIST_TASK_ID ) {
		wk = TaskTable[no].work;
	}

	no = CheckTaskNo(map_control_task);
	if ( no != NOT_EXIST_TASK_ID ) {
		// �����V�[�P���X������������
		wk = TaskTable[no].work;
		wk[CTRLWK_VOICE_SEQ] = 0;
		wk[CTRLWK_VOICE_WAIT] = 0;
	}
}

//==============================================================
//	�_�~�[�G�t�F�N�g
//==============================================================
static void dummy_effect_task( u8 no)
{
}


//==============================================================
//
//		�ۑ��n�`
//
//==============================================================
#define	LOGEFFECT_WAIT	(8)
enum{
	LOGWK_SEQ = 1,
	LOGWK_X,
	LOGWK_Y,
	LOGWK_OLDX,
	LOGWK_OLDY,
	LOGWK_COUNT,
};
enum{
	LOGSEQ_INIT,
	LOGSEQ_WATCH,
	LOGSEQ_EFFECT,
};

enum{
	LOG_UP_FLOAT_CELL_ID = 600,
	LOG_UP_HALF_CELL_ID = 601,
	LOG_UP_SINK_CELL_ID = 602,

	LOG_DOWN_FLOAT_CELL_ID = 608,
	LOG_DOWN_HALF_CELL_ID = 609,
	LOG_DOWN_SINK_CELL_ID = 610,

	LOG_LEFT_FLOAT_CELL_ID = 592,
	LOG_LEFT_HALF_CELL_ID = 594,
	LOG_LEFT_SINK_CELL_ID = 596,

	LOG_RIGHT_FLOAT_CELL_ID = 593,
	LOG_RIGHT_HALF_CELL_ID = 595,
	LOG_RIGHT_SINK_CELL_ID = 597,
};

typedef const struct{
	s8 x1,y1;
	s16 cell1;
	s8 x2,y2;
	s16 cell2;
}LOG_CELL_WRITE;

static LOG_CELL_WRITE half_cell_data[] =
{
	{ 0, 0, LOG_UP_HALF_CELL_ID,	0,+1, LOG_DOWN_HALF_CELL_ID, },
	{ 0,-1, LOG_UP_HALF_CELL_ID,	0, 0, LOG_DOWN_HALF_CELL_ID, },
	{ 0, 0, LOG_LEFT_HALF_CELL_ID,	+1, 0, LOG_RIGHT_HALF_CELL_ID, },
	{ -1, 0, LOG_LEFT_HALF_CELL_ID,	0, 0, LOG_RIGHT_HALF_CELL_ID, },
};

static LOG_CELL_WRITE sink_cell_data[] =
{
	{ 0, 0, LOG_UP_SINK_CELL_ID,	0,+1, LOG_DOWN_SINK_CELL_ID, },
	{ 0,-1, LOG_UP_SINK_CELL_ID,	0, 0, LOG_DOWN_SINK_CELL_ID, },
	{ 0, 0, LOG_LEFT_SINK_CELL_ID,	+1, 0, LOG_RIGHT_SINK_CELL_ID, },
	{ -1, 0, LOG_LEFT_SINK_CELL_ID,	0, 0, LOG_RIGHT_SINK_CELL_ID, },
};

static LOG_CELL_WRITE float_cell_data[] =
{
	{ 0, 0, LOG_UP_FLOAT_CELL_ID, 0,+1, LOG_DOWN_FLOAT_CELL_ID, },
	{ 0,-1, LOG_UP_FLOAT_CELL_ID, 0, 0, LOG_DOWN_FLOAT_CELL_ID, },
	{ 0, 0, LOG_LEFT_FLOAT_CELL_ID, +1, 0, LOG_RIGHT_FLOAT_CELL_ID, },
	{ -1, 0, LOG_LEFT_FLOAT_CELL_ID, 0, 0, LOG_RIGHT_FLOAT_CELL_ID, },
};

static LOG_CELL_WRITE * get_log_cell_data( LOG_CELL_WRITE * data, u16 attr )
{
	if ( ATR_IsLogBridgeUpCheck(attr) ) {
		return data;
	}
	else if ( ATR_IsLogBridgeDownCheck(attr) ) {
		return data+1;
	}
	else if ( ATR_IsLogBridgeLeftCheck(attr) ) {
		return data+2;
	}
	else if ( ATR_IsLogBridgeRightCheck(attr) ) {
		return data+3;
	}
	else return NULL;
}

static void write_log_cell( LOG_CELL_WRITE * data, s16 x, s16 y, int flag )
{
	u16 attr = GetCellAttribute( x, y );

	data = get_log_cell_data( data, attr );
	if ( data == NULL ) return;

	SetCell( x+data->x1, y+data->y1, data->cell1 );
	if ( flag ) RewriteSingleBG( x+data->x1, y+data->y1 );
	SetCell( x+data->x2, y+data->y2, data->cell2 );
	if ( flag ) RewriteSingleBG( x+data->x2, y+data->y2 );
}

static void write_half_cell( s16 x, s16 y, int flag )
{
	write_log_cell( half_cell_data, x, y, flag );
}

static void write_sink_cell( s16 x, s16 y, int flag )
{
	write_log_cell( sink_cell_data, x, y, flag );
}

static void write_float_cell( s16 x, s16 y, int flag )
{
	write_log_cell( float_cell_data, x, y, flag );
}


static int check_before_pos_log( s16 nx, s16 ny, s16 ox, s16 oy )
{
	u16 old_attr = GetCellAttribute(ox,oy);

	if ( ATR_IsLogBridgeUpCheck(old_attr) ) {
		if ( ny > oy ) return FALSE;
	}
	else if ( ATR_IsLogBridgeDownCheck(old_attr) ) {
		if ( ny < oy ) return FALSE;
	}
	else if ( ATR_IsLogBridgeLeftCheck(old_attr) ) {
		if ( nx > ox ) return FALSE;
	}
	else if ( ATR_IsLogBridgeRightCheck(old_attr) ) {
		if ( nx < ox ) return FALSE;
	}
	return TRUE;
}

static int check_new_pos_log( s16 nx, s16 ny, s16 ox, s16 oy )
{
	u16 new_attr = GetCellAttribute(nx,ny);

	if ( ATR_IsLogBridgeUpCheck(new_attr) ) {
		if ( ny < oy ) return FALSE;
	}
	else if ( ATR_IsLogBridgeDownCheck(new_attr) ) {
		if ( ny > oy ) return FALSE;
	}
	else if ( ATR_IsLogBridgeLeftCheck(new_attr) ) {
		if ( nx < ox ) return FALSE;
	}
	else if ( ATR_IsLogBridgeRightCheck(new_attr) ) {
		if ( nx > ox ) return FALSE;
	}
	return TRUE;
}

static void log_effect_task( u8 no )
{
	s16 x,y;
	s16 * wk = TaskTable[no].work;

	GetHeroCellPos( &x, &y );

	switch ( wk[LOGWK_SEQ] ) {
	case LOGSEQ_INIT:
		wk[LOGWK_X] = x;
		wk[LOGWK_Y] = y;
		write_sink_cell(x,y, TRUE);
		wk[LOGWK_SEQ] = LOGSEQ_WATCH;
		break;
	
	case LOGSEQ_WATCH:
		if ( x == wk[LOGWK_X] && y == wk[LOGWK_Y] ) return;
		if ( check_before_pos_log(x,y,wk[LOGWK_X],wk[LOGWK_Y]) ) {
			write_half_cell(wk[LOGWK_X],wk[LOGWK_Y], TRUE);
			write_float_cell(wk[LOGWK_X],wk[LOGWK_Y],FALSE);
			wk[LOGWK_OLDX] = wk[LOGWK_X];
			wk[LOGWK_OLDY] = wk[LOGWK_Y];
			wk[LOGWK_SEQ] = LOGSEQ_EFFECT;
			wk[LOGWK_COUNT] = LOGEFFECT_WAIT;
		} else {
			wk[LOGWK_OLDX] = -1;
			wk[LOGWK_OLDY] = -1;
		}
		if ( check_new_pos_log(x,y,wk[LOGWK_X],wk[LOGWK_Y]) ) {
			write_half_cell(x,y, TRUE);
			wk[LOGWK_SEQ] = LOGSEQ_EFFECT;
			wk[LOGWK_COUNT] = LOGEFFECT_WAIT;
		}
		wk[LOGWK_X] = x;
		wk[LOGWK_Y] = y;
		if ( ATR_IsLogBridgeCheck(GetCellAttribute(x,y)) ) SePlay( SE_MIZU );
		break;

	case LOGSEQ_EFFECT:
		wk[LOGWK_COUNT]--;
		if ( wk[LOGWK_COUNT] != 0 ) return;
		write_sink_cell(x,y, TRUE);

		if ( wk[LOGWK_OLDX] != -1 && wk[LOGWK_OLDY] != -1 ) {
			write_float_cell(wk[LOGWK_OLDX],wk[LOGWK_OLDY],TRUE);
		}
		wk[LOGWK_SEQ] = LOGSEQ_WATCH;
		break;
	}
}


//==============================================================
//
//		�苴�n�`
//
//==============================================================
enum{
	HBRIDGEWK_SEQ = 1,
	HBRIDGEWK_X,
	HBRIDGEWK_Y,
	HBRIDGEWK_BACK_X,
	HBRIDGEWK_BACK_Y,
	HBRIDGEWK_WAIT,
};
enum{
	HBRIDGESEQ_INIT,
	HBRIDGESEQ_WATCH,
	HBRIDGESEQ_EFFECT,
};

enum{
	HBRIDGE_WAIT = 16,
};

static void set_down_hbridge_cell( s16 x, s16 y )
{
	if ( GetHeroHeight() % 2 ) return;
	switch ( GetCellCode(x,y) ) {
	case 590: SetCell(x,y,591); break;
	case 598: SetCell(x,y,599); break;
	}
}

static void set_up_hbridge_cell( s16 x, s16 y )
{
	if ( GetHeroHeight() % 2 ) return;
	switch ( GetCellCode(x,y) ) {
	case 591: SetCell(x,y,590);break;
	case 599: SetCell(x,y,598);break;
	}
}


static void hang_bridge_effect_task( u8 no )
{
	s16 x,y;
	s16 nx,ny;
	s16 * wk = TaskTable[no].work;
	u8 now_is_flag, before_is_flag;
	u8 height_flag;

	GetHeroCellPos( &x, &y );

	switch ( wk[HBRIDGEWK_SEQ] ) {
	case HBRIDGESEQ_INIT:
		wk[HBRIDGEWK_X] = x;
		wk[HBRIDGEWK_Y] = y;
		if( ATR_IsHangBridgeCheck( GetCellAttribute(x,y) ) ) {
			set_down_hbridge_cell(x,y);
			RewriteSingleBG(x,y);
		}
		wk[HBRIDGEWK_SEQ] = HBRIDGESEQ_WATCH;
		break;
	
	case HBRIDGESEQ_WATCH:
		nx = wk[HBRIDGEWK_X]; ny = wk[HBRIDGEWK_Y];
		if ( x == nx && y == ny ) return;

		now_is_flag = ATR_IsHangBridgeCheck( GetCellAttribute(x,y) );
		before_is_flag = ATR_IsHangBridgeCheck( GetCellAttribute(nx,ny) );
		height_flag = GetHeroHeight() % 2 == 0 ? TRUE:FALSE;
		if ( height_flag && ( now_is_flag == TRUE || before_is_flag == TRUE ) ) {
			SePlay( SE_HASHI );
		};
		if ( before_is_flag ) {
			set_up_hbridge_cell( nx, ny );
			RewriteSingleBG( nx, ny );
		}
		if ( before_is_flag ) {
			set_down_hbridge_cell(x,y);
			RewriteSingleBG(x,y);
		}
		wk[HBRIDGEWK_BACK_X] = nx;
		wk[HBRIDGEWK_BACK_Y] = ny;
		wk[HBRIDGEWK_X] = x;
		wk[HBRIDGEWK_Y] = y;
		if ( before_is_flag == FALSE ) break;
		wk[HBRIDGEWK_WAIT] = HBRIDGE_WAIT;
		wk[HBRIDGEWK_SEQ] = HBRIDGESEQ_EFFECT;
	//	break;

	case HBRIDGESEQ_EFFECT:
		wk[HBRIDGEWK_WAIT]--;
		nx = wk[HBRIDGEWK_BACK_X]; ny = wk[HBRIDGEWK_BACK_Y];
		switch ( wk[HBRIDGEWK_WAIT] % 7 ) {

		case 0:
			RewriteSingleBG(nx,ny);
			break;
		case 1:
		case 2:
		case 3:
			break;
		case 4:
			set_down_hbridge_cell(nx,ny);
			RewriteSingleBG(nx,ny);
			set_up_hbridge_cell(nx,ny);
			break;
		case 5:
		case 6:
		case 7:
			break;
		}
		if ( wk[HBRIDGEWK_WAIT] == 0 ) {
			wk[HBRIDGEWK_SEQ] = HBRIDGESEQ_WATCH;
		}
	}
}


//==============================================================
//
//		�����X�n�`
//
//==============================================================

enum{
	CELLID_CRACKED_ICE	= 526,	//�Ђъ��ꂽ�X�̃Z��
	CELLID_BROKEN_ICE	= 518,	//�����������X�̃Z��
};

enum {
	ICE_MEM_SX = 3,			//�Ђъ���ʒu�̍ō��[
	ICE_MEM_EX = 13,		//�Ђъ���ʒu�̍ŉE�[
	ICE_MEM_SY = 6,			//�Ђъ���ʒu�̍ŏ�[
	ICE_MEM_EY = 19,		//�Ђъ���ʒu�̍ŉ��[
};

//�Ђъ���ʒu���L������ꏊ�̃e�[�u��
//	�x���W���I�t�Z�b�g�Ƃ��ĎQ�Ƃ���
static const u16 ice_mem_table[] = {
	0,				// 0
	0,				// 1
	0,				// 2
	0,				// 3
	0,				// 4
	0,				// 5
	LOCALWORK1,		// 6
	LOCALWORK2,		// 7
	LOCALWORK3,		// 8
	LOCALWORK4,		// 9
	0,				// 10
	0,				// 11
	LOCALWORK5,		// 12
	LOCALWORK6,		// 13
	LOCALWORK7,		// 14
	0,				// 15
	0,				// 16
	LOCALWORK8,		// 17
	LOCALWORK9,		// 18
	LOCALWORK10,	// 19
	0,				// 20
	0,				// 21
	0,				// 22
	0,				// 23
	0,				// 24
	0,				// 25
};

//-------------------------------------------------------------
//	�Ђъ�����Ǘ�
//-------------------------------------------------------------
//�Ђъ���ۑ�����ꏊ���ǂ����̃`�F�b�N
static int check_ice_mem_exist(s16 x, s16 y)
{
	if (x < ICE_MEM_SX || x > ICE_MEM_EX)
		return FALSE;

	if (y < ICE_MEM_SY || y > ICE_MEM_EY)
		return FALSE;

	if (ice_mem_table[y] == 0)
		return FALSE;

	return TRUE;
}

//�Ђъ���t���O���Z�b�g
static void set_ice_mem_flag(s16 x, s16 y)
{
	u16 * work;

	if (check_ice_mem_exist(x, y) == FALSE)
		return;

	work = GetEventWorkAdrs(ice_mem_table[y]);
	*work |= 1 << (x - ICE_MEM_SX);
}

//�Ђъ���t���O�̃��Z�b�g
static int check_ice_mem_flag(s16 x, s16 y)
{
	u16 value;

	if (check_ice_mem_exist(x, y) == FALSE)
		return FALSE;

	value = GetEventWorkValue(ice_mem_table[y]);
	value &= 1 << (x - ICE_MEM_SX);
	if (value != 0)
		return TRUE;
	else
		return FALSE;
}

//-------------------------------------------------------------
//�Ђъ���t���O���Q�Ƥ��ʓ��̕X������������
//-------------------------------------------------------------
void SetAllCrackedIceCell(void)
{
	int x,y,ex,ey;

	ex = NowMap.Screen->Xsize;
	ey = NowMap.Screen->Ysize;

	for (x=0; x<ex; x++) {
		for (y=0; y<ey; y++) {
			if (check_ice_mem_flag(x,y) == TRUE) {
				SetCell(x+MAP_X_MARGIN, y+MAP_Y_MARGIN, CELLID_CRACKED_ICE);
			}
		}
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
enum{
	//�g�p���[�N�i���o�[��`
	ICEWK_SEQ = 1,		//�V�[�P���X�ێ�
	ICEWK_X,			//���O�̂w���W�ێ�
	ICEWK_Y,			//���O�̂x���W�ێ�
	ICEWK_SETX,			//�Z��������������w���W
	ICEWK_SETY,			//�Z��������������x���W
	ICEWK_COUNT,		//�E�F�C�g�J�E���^
};

enum{
	//����V�[�P���X�i���o�[��`
	ICESEQ_INIT = 0,
	ICESEQ_WATCH,
	ICESEQ_CRACK_EFFECT,
	ICESEQ_TRAP_EFFECT,
};

enum{
	//�Z�����������܂ł̃E�F�C�g
	WAIT_ICE_EFFECT = 4,
};

//-------------------------------------------------------------
//	���l�W���G�t�F�N�g����
//-------------------------------------------------------------
static void crash_ice_task( u8 no )
{
	s16 x,y;
	u16 attr;
	u16 * count;
	s16 * wk = TaskTable[no].work;
	

	switch ( wk[ICEWK_SEQ] ) {

	case ICESEQ_INIT:				/* ���������� */
		GetHeroCellPos( &x, &y );
		wk[ICEWK_X] = x;
		wk[ICEWK_Y] = y;
		wk[ICEWK_SEQ] = ICESEQ_WATCH;
		break;

	case ICESEQ_WATCH:				/* �ʏ�A�Ď����[�h */
		GetHeroCellPos( &x, &y );
		if ( x == wk[ICEWK_X] && y == wk[ICEWK_Y] ) return;
	
		wk[ICEWK_X] = x;
		wk[ICEWK_Y] = y;

		attr = GetCellAttribute(x,y);
		count = GetEventWorkAdrs( WK_ICE_COUNT );
		if( ATR_IsTrapIceCheck( attr ) == TRUE ) {
			*count += 1;
			wk[ICEWK_COUNT] = WAIT_ICE_EFFECT;
			wk[ICEWK_SEQ] = ICESEQ_CRACK_EFFECT;
			wk[ICEWK_SETX] = x;
			wk[ICEWK_SETY] = y;
		}
		else if ( ATR_IsCrackedIceCheck(attr) == TRUE ) {
			*count = 0;		/* �C�x���g�N���L�[���Z�b�g */ 
			wk[ICEWK_COUNT] = WAIT_ICE_EFFECT;
			wk[ICEWK_SEQ] = ICESEQ_TRAP_EFFECT;
			wk[ICEWK_SETX] = x;
			wk[ICEWK_SETY] = y;
		}
		break;

	case ICESEQ_CRACK_EFFECT:			/* �ЂуG�t�F�N�g */
		if ( wk[ICEWK_COUNT] ) {
			wk[ICEWK_COUNT]--;
			return;
		}
		x = wk[ICEWK_SETX];
		y = wk[ICEWK_SETY];
		SePlay( SE_RU_BARI );
		SetCell(x,y,CELLID_CRACKED_ICE);
		RewriteSingleBG(x,y);
		set_ice_mem_flag(x-MAP_X_MARGIN, y-MAP_Y_MARGIN);
		wk[ICEWK_SEQ] = ICESEQ_WATCH;
		return;

	case ICESEQ_TRAP_EFFECT:			/* �����G�t�F�N�g */
		if ( wk[ICEWK_COUNT] ) {
			wk[ICEWK_COUNT]--;
			return;
		}
		x = wk[ICEWK_SETX];
		y = wk[ICEWK_SETY];
		SePlay( SE_RU_GASYAN );
		SetCell(x,y,CELLID_BROKEN_ICE);
		RewriteSingleBG(x,y);
		wk[ICEWK_SEQ] = ICESEQ_WATCH;
		return;
	}
}


//==============================================================
//
//	�D�G�t�F�N�g�֘A
//
//==============================================================


enum {
	//�D�G�t�F�N�g�����ۂɔ���������܂ł̃E�F�C�g
	ASH_EFF_WAIT = 4,

	//�D�܂ɏW�߂���ő吔
	ASH_GATHER_MAX	= 9999,
};

enum{
	WK_ASH_ID = 0,

	WK_ASH_X,			//X���W�ێ��p���[�N
	WK_ASH_Y,			//Y���W�ێ��p���[�N
};

enum{
	//road113�̊D�����Ԃ�����
	CELLID_ASH_COVERD_R113 = 522,
	CELLID_ASH_REMOVED_R113 = 530,

	//cave_d0701�̊D�����Ԃ�����
//	CELLID_ASH_COVERD_R112 =  519,
	CELLID_ASH_REMOVED_R112 = 518,
};

//----------------------------
//	����^�X�N�F�D�G�t�F�N�g
//----------------------------
static void ash_grass_effect_task(u8 no)
{
	s16 x,y;
	s16 * wk = TaskTable[no].work;

	GetHeroCellPos( &x, &y );
	
	if ( x != wk[WK_ASH_X] || y != wk[WK_ASH_Y] ) {
		wk[WK_ASH_X] = x;
		wk[WK_ASH_Y] = y;

		if ( ATR_AshGrassCheck( GetCellAttribute(x,y) ) == FALSE ) return;
		if ( GetCellCode(x,y) == CELLID_ASH_COVERD_R113 ) {
			FeAshMake(x, y, CELLID_ASH_REMOVED_R113, ASH_EFF_WAIT);
		} else {
			FeAshMake(x, y, CELLID_ASH_REMOVED_R112, ASH_EFF_WAIT);
		}
		if ( ItemCheck( ITEM_HAIBUKURO, 1 ) ) {
		//�D�n�`��ʉ߂����񐔂��J�E���g����
			u16 * count = GetEventWorkAdrs( WK_ASH_GATHER_COUNT );
			if ( *count < ASH_GATHER_MAX ) ++ *count;
		}
	}
}


//==============================================================
//
//		�G�t�F�N�g����@���鏰
//
//==============================================================
enum{
	BFWK_ID = 0,
	BFWK_SEQ,
	BFWK_X,
	BFWK_Y,
	BFWK_EF_WAIT1,
	BFWK_EF_X1,
	BFWK_EF_Y1,
	BFWK_EF_WAIT2,
	BFWK_EF_X2,
	BFWK_EF_Y2,
};

enum{
	BREAK_EFFECT_WAIT = 3,
};

enum{
	//�ʏ�̏ꍇ�̉��鏰
	CELLID_DANGER_FLOOR_DUNGEON = 559,
	//�ʏ�̏ꍇ�̉�ꂽ��
	CELLID_BROKEN_FLOOR_DUNGEON = 518,

	//����̂͂���̏ꍇ�̉�ꂽ��
	CELLID_BROKEN_FLOOR_TOWER	= 567,
};

static void set_broken_floor(s16 x, s16 y)
{
	u16 cell;

	if (GetCellCode(x, y) == CELLID_DANGER_FLOOR_DUNGEON)
		cell = CELLID_BROKEN_FLOOR_DUNGEON;
	else
		cell = CELLID_BROKEN_FLOOR_TOWER;

	SetCell(x, y, cell);
	RewriteSingleBG(x, y);
}

static void break_floor_task( u8 no )
{
	s16 x,y;
	u16 attr;
	s16 * wk;
	
	wk = TaskTable[no].work;

	GetHeroCellPos(&x, &y);
	attr = GetCellAttribute(x, y);

	//�G�t�F�N�g����P
	if ( wk[BFWK_EF_WAIT1] != 0 ) {
		wk[BFWK_EF_WAIT1]--;
		if ( wk[BFWK_EF_WAIT1] == 0 ) {
			set_broken_floor(wk[BFWK_EF_X1], wk[BFWK_EF_Y1]);
		}
	}

	//�G�t�F�N�g����Q
	if ( wk[BFWK_EF_WAIT2] != 0 ) {
		wk[BFWK_EF_WAIT2]--;
		if ( wk[BFWK_EF_WAIT2] == 0 ) {
			set_broken_floor(wk[BFWK_EF_X2], wk[BFWK_EF_Y2]);
		}
	}

	if ( ATR_IsBrokenFloorCheck(attr) ) {
		SetEventWorkValue( WK_ICE_COUNT, 0 );
	}

	if ( x == wk[BFWK_X] && y == wk[BFWK_Y] ) return;

	wk[BFWK_X] = x;
	wk[BFWK_Y] = y;
	if ( ATR_IsCrackedFloorCheck(attr) == FALSE ) return;

	if (GetHeroMoveSpeed() != HERO_MOVE_SPEED_MAX) {
		SetEventWorkValue( WK_ICE_COUNT, 0 );
	}

	if ( wk[BFWK_EF_WAIT1] == 0 ) {
		wk[BFWK_EF_WAIT1] = BREAK_EFFECT_WAIT;
		wk[BFWK_EF_X1] = x;
		wk[BFWK_EF_Y1] = y;
	}
	else if ( wk[BFWK_EF_WAIT2] == 0 ) {
		wk[BFWK_EF_WAIT2] = BREAK_EFFECT_WAIT;
		wk[BFWK_EF_X2] = x;
		wk[BFWK_EF_Y2] = y;
	}
	
}

//==============================================================
//
//	���ׂ��̃G�t�F�N�g
//
//	���˂ɂ����Ƃ���Ă���̂łh�c�̗̈��
//	�@���ʗp�Ɏ���Ă����K�v�͂Ȃ�
//==============================================================
enum{
	SLOPEWK_ID = 0,
	SLOPEWK_SEQ,
	SLOPEWK_X0,
	SLOPEWK_Y0,

	SLOPEWK_CNT1,
	SLOPEWK_X1,
	SLOPEWK_Y1,

	SLOPEWK_CNT2,
	SLOPEWK_X2,
	SLOPEWK_Y2,

	SLOPEWK_CNT3,
	SLOPEWK_X3,
	SLOPEWK_Y3,

	SLOPEWK_CNT4,
	SLOPEWK_X4,
	SLOPEWK_Y4,
};

enum{
	CELLID_SLOPE_NORMAL = 232,
	CELLID_SLOPE_ANIME1 = 233,
	CELLID_SLOPE_ANIME2 = 234,
	CELLID_SLOPE_ANIME3 = 235,
};

static const u16 slope_anime_table[] = {
	CELLID_SLOPE_NORMAL,
	CELLID_SLOPE_ANIME3,
	CELLID_SLOPE_ANIME2,
	CELLID_SLOPE_ANIME1,
};

enum{
	//�A�j���P�p�^�[��������̕\���t���[��
	SLOPE_ANM_FRAME = 8,

	//�A�j���e�[�u���̒���
	SLOPE_ANM_TBL_MAX = NELEMS(slope_anime_table),

	//�A�j���S�̂̃t���[����
	SLOPE_ANM_COUNT_MAX = SLOPE_ANM_FRAME * SLOPE_ANM_TBL_MAX,
};


static void control_slide_anime( s16 * count, s16 x, s16 y )
{
	u16 cell;

	(*count) --;

	if (*count == 0) {
		cell = CELLID_SLOPE_NORMAL;
	} else {
		cell = slope_anime_table[*count / SLOPE_ANM_FRAME];
	}

	SetCell(x, y, cell);
	RewriteSingleBG(x, y);
	SetCell(x, y, CELLID_SLOPE_NORMAL);
}

static void slide_slope_task( u8 no )
{
	int i;
	s16 x,y;
	s16 * wk;
	s16 sx,sy;
	u16 mapid;

	wk = TaskTable[no].work;

	GetHeroCellPos( &x, &y );
	mapid = (Fld.MapID.div_id << 8) | Fld.MapID.map_id;

	switch ( wk[SLOPEWK_SEQ] ) {
	case 0:
		wk[SLOPEWK_ID] = mapid;
		wk[SLOPEWK_X0] = x;
		wk[SLOPEWK_Y0] = y;
		wk[SLOPEWK_SEQ] = 1;
		wk[SLOPEWK_CNT1] = 0;
		wk[SLOPEWK_CNT2] = 0;
		wk[SLOPEWK_CNT3] = 0;
		wk[SLOPEWK_CNT4] = 0;
		break;
	case 1:
		if ( wk[SLOPEWK_X0] == x && wk[SLOPEWK_Y0] == y ) break;
		wk[SLOPEWK_X0] = x;
		wk[SLOPEWK_Y0] = y;
		if ( ATR_IsSlipSlopeCheck(GetCellAttribute(x,y)) == FALSE ) break;
		for ( i=SLOPEWK_CNT1; i<=SLOPEWK_CNT4; i += 3 ) {
			if ( wk[i] == 0 ) {
				wk[i] = SLOPE_ANM_COUNT_MAX;
				wk[i+1] = x;
				wk[i+2] = y;
				break;
			}
		}
		
	}

	if (MapChange.chg_flag && mapid != wk[SLOPEWK_ID]) {
		wk[SLOPEWK_ID] = mapid;
		sx = MapChange.diffx;
		sy = MapChange.diffy;
	} else {
		sx = 0;
		sy = 0;
	}

	for ( i=SLOPEWK_CNT1; i<=SLOPEWK_CNT4; i += 3 ) {
		if (wk[i] == 0) continue;
		wk[i+1] = wk[i+1] - sx;
		wk[i+2] = wk[i+2] - sy;
		control_slide_anime( &wk[i], wk[i+1], wk[i+2] );
	}
}




