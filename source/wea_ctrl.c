//=========================================================================
//
//
//	�V��w��֘A
//
//
//=========================================================================

#include "common.h"
#include "weather.h"
#include "map.h"
#include "record.h"

#define __WEA_CTRL_H_GLOBAL__
#include "wea_ctrl.h"

static u8 convert_weather_code( u8 code );
static void count_rain_weather( u8 new, u8 before );

//=========================================================================
//
//	�V�󐧌�
//
//=========================================================================

void SetFieldWeatherValue( int type )
{
	u8 before = Fld.WeatherValue;
	Fld.WeatherValue = convert_weather_code(type);
	count_rain_weather( Fld.WeatherValue, before );
}

u8 GetFieldWeatherValue( void )
{
	return Fld.WeatherValue;
}

void InitFieldWeatherValue( void )
{
	u8 before = Fld.WeatherValue;
	Fld.WeatherValue = convert_weather_code(NowMap.weather);
	count_rain_weather( Fld.WeatherValue, before );
}

void ChangeFieldWeatherSmooth( int type )
{
	SetFieldWeatherValue( type );
	WeatherChangeSmooth( GetFieldWeatherValue() );
}

void ChangeFieldWeatherDirect( int type )
{
	SetFieldWeatherValue( type );
	WeatherChangeDirect( GetFieldWeatherValue() );
}

void UpdateFieldWeatherSmooth( void )
{
	WeatherChangeSmooth( GetFieldWeatherValue() );
}

void UpdateFieldWeatherDirect( void )
{
	WeatherChangeDirect( GetFieldWeatherValue() );
}

//==================================================================================
//
//	�V��w��R�[�h�֘A
//
//==================================================================================
static const u8 srtr_table[] =
{
	WEATHER_SHINE2,
	WEATHER_RAIN,
	WEATHER_THUNDER,
	WEATHER_RAIN,
};
static const u8 ssrs_table[] =
{
	WEATHER_SHINE2,
	WEATHER_SHINE2,
	WEATHER_RAIN,
	WEATHER_SHINE2,
};

//-----------------------------------------------------------------------------
//	�V��w��R�[�h�iwea_ctrl.h�j��V��R�[�h�iweather.h�Œ�`�j�ɕϊ�
//-----------------------------------------------------------------------------
static u8 convert_weather_code( u8 code )
{
	switch ( code ) {
	case ID_WEATHER_OFF:      return WEATHER_OFF;      // �V��G�t�F�N�g�Ȃ�
	case ID_WEATHER_SHINE:    return WEATHER_SHINE;    // ����
	case ID_WEATHER_SHINE2:   return WEATHER_SHINE2;   // ����i�_�Ȃ��j
	case ID_WEATHER_RAIN:     return WEATHER_RAIN;     // �J
	case ID_WEATHER_SNOW:     return WEATHER_SNOW;     // ��
	case ID_WEATHER_THUNDER:  return WEATHER_THUNDER;  // �J�~�i��
	case ID_WEATHER_MIST:     return WEATHER_MIST;     // ��
	case ID_WEATHER_ASH:      return WEATHER_ASH;      // �ΎR�D
	case ID_WEATHER_STORM:    return WEATHER_STORM;    // ����
	case ID_WEATHER_MIST2:    return WEATHER_MIST2;    // ���Q
	case ID_WEATHER_SEA:      return WEATHER_SEA;     // �C��
	case ID_WEATHER_CLOUD:    return WEATHER_CLOUD;   // �܂�
	case ID_WEATHER_SHINE3:   return WEATHER_SHINE3;  // ����i�����������j
	case ID_WEATHER_HEAVYRAIN:return WEATHER_HEAVYRAIN; // ��J  2002/06/28 taya
	case ID_WEATHER_MARINE:   return WEATHER_MARINE;  // �C��


	case ID_WEATHER_SRTR:     return srtr_table[Fld.WeatherCycle];
	case ID_WEATHER_SSRS:     return ssrs_table[Fld.WeatherCycle];
	}
	return WEATHER_OFF;
}

//-----------------------------------------------------------------------------
//	�V������̍X�V
//-----------------------------------------------------------------------------
void WeatherCycle_Update( u16 diff )
{
	u16 value = Fld.WeatherCycle;
	value += diff;
	value %= WEATHER_CYCLE_MAX;
	Fld.WeatherCycle = value;
}

//-----------------------------------------------------------------------------
//	�J���~�����񐔂��J�E���g�i��J�̓J�E���g���Ȃ��j
//-----------------------------------------------------------------------------
static void count_rain_weather( u8 new, u8 before )
{
	if ( new == before ) return;
	if ( new == WEATHER_RAIN || new == WEATHER_THUNDER ) {
		IncRecord( CNT_RAIN );
	}
}

