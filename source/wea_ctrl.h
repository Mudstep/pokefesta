#ifndef __WEA_CTRL_H__
#define __WEA_CTRL_H__

//=========================================================================
//	
//	�V��w��֘A
//
//=========================================================================

#undef GLOBAL
#ifdef __WEA_CTRL_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

#define ID_WEATHER_OFF      (0)      // �V��G�t�F�N�g�Ȃ�
#define ID_WEATHER_SHINE    (1)      // ����
#define ID_WEATHER_SHINE2   (2)      // ����i�_�Ȃ��j
#define ID_WEATHER_RAIN     (3)      // �J
#define ID_WEATHER_SNOW     (4)      // ��
#define ID_WEATHER_THUNDER  (5)      // �J�~�i��
#define ID_WEATHER_MIST     (6)      // ��
#define ID_WEATHER_ASH      (7)      // �ΎR�D
#define ID_WEATHER_STORM    (8)      // ����
#define ID_WEATHER_MIST2    (9)      // ���Q
#define ID_WEATHER_SEA     (10)      // �C��
#define ID_WEATHER_CLOUD   (11)      // �܂�
#define ID_WEATHER_SHINE3  (12)      // ����i�����������j
#define ID_WEATHER_HEAVYRAIN (13)    // ��J
#define ID_WEATHER_MARINE  (14)      // �C��

#define	ID_WEATHER_SRTR		(20)	// ����A�J�A���A�J�@�œ����ω�
#define	ID_WEATHER_SSRS		(21)	// ����A����A�J�A����A�œ����ω�


//�V�����
#define WEATHER_CYCLE_MAX	4

#ifndef	ASM_CPP
//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

GLOBAL void InitFieldWeatherValue( void );
GLOBAL void SetFieldWeatherValue( int type );
GLOBAL u8 GetFieldWeatherValue( void );
GLOBAL void ChangeFieldWeatherSmooth( int type );
GLOBAL void ChangeFieldWeatherDirect( int type );
GLOBAL void UpdateFieldWeatherSmooth( void );
GLOBAL void UpdateFieldWeatherDirect( void );

//-----------------------------------------------------------------------------
//	�V������̍X�V
//-----------------------------------------------------------------------------
GLOBAL void WeatherCycle_Update( u16 diff );

#endif /* ASM_CPP */


#undef GLOBAL
#endif	/*__WEA_CTRL_H__*/

