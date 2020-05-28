/***********************************************************************
**	�V�󐧌�^�X�N
**
**
***********************************************************************/
#ifndef __WEATHER_H__
#define __WEATHER_H__

/*========================================
==	�V����
========================================*/
#define WEATHER_OFF      (0)      // �V��G�t�F�N�g�Ȃ�
#define WEATHER_SHINE    (1)      // ����
#define WEATHER_SHINE2   (2)      // ����i�_�Ȃ��j
#define WEATHER_RAIN     (3)      // �J
#define WEATHER_SNOW     (4)      // ��
#define WEATHER_THUNDER  (5)      // �J�~�i��
#define WEATHER_MIST     (6)      // ��
#define WEATHER_ASH      (7)      // �ΎR�D
#define WEATHER_STORM    (8)      // ����
#define WEATHER_MIST2    (9)      // ���Q
#define WEATHER_SEA     (10)      // �C��
#define WEATHER_CLOUD   (11)      // �܂�
#define WEATHER_SHINE3  (12)      // ����i�����������j
#define WEATHER_HEAVYRAIN  (13)   // ��J
#define WEATHER_MARINE  (14)      // �C��
#define WEATHER_MAX     (15)      /* �ő�l */
/* -------------- */

/*========================================
==	�t�F�[�h���[�h
========================================*/
#define FIELD_BLACK_IN  (0)
#define FIELD_BLACK_OUT (1)
#define FIELD_WHITE_IN  (2)
#define FIELD_WHITE_OUT (3)


#ifndef ASM_CPP

// ���ȑO�̎d�l�Ƃ̌݊��̂��߂̃}�N���i�s�v�ɂȂ���������j
#define FIELD_FADE_IN  (FIELD_BLACK_IN)
#define FIELD_FADE_OUT (FIELD_BLACK_OUT)


void WeatherInit(void);
void WeatherChangeSmooth(u8 weather);
void WeatherChangeDirect(u8 weather);
void WeatherChangeDemo(u8 weather);
void FieldFadeControl(u8 fade_mode, u8 wait);
u8  FieldFadeinCheck(void);
void FieldWeather_AdjustObjPal(u8 pal_num);
void FieldWeather_AdjustBGPal(u8 pal_num);
u8 GetWeatherNow(void);
void StopRainSound(void);
u8 CheckWeatherAdjustFill(void);
u8 CheckWeatherChange(void);
void WeatherFadeStop(void);
void WeatherFadeStart(void);

void WeatherFadeSkipStart(u8 palno);
void WeatherFadeSkipEnd(void);

#endif /* #ifndef ASM_CPP */
#endif
