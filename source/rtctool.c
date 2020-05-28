//=========================================================================
//	
//
//	RTC(Real Time Clock)����֘A
//
//
//=========================================================================

#include "common.h"

#include "../library/AgbRtc.h"

#include "print.h"

#define __RTCTOOL_H_GLOBAL__
#include "rtctool.h"

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...


//=========================================================================
//	�ϐ�
//=========================================================================

//RTC�̏��
static u16 RtcStatus;

//�Q�[�������Ԋi�[�p�ϐ�
GAME_TIME GameTime;

//RTC�f�[�^�󂯓n���p�e���|����
static RtcDataOrg rtc_val;

//�N�����̏��
static u8 FirstRtcStatus;

static u16 ime_bak;

static const RtcDataOrg rtc_initializer = {
	0,			//u8	year;
	1,			//u8	month;
	1,			//u8	day;
	0,			//u8	week;
	0,			//u8	hour;
	0,			//u8	minute;
	0,			//u8	second;
	0,			//u8	stat;
	0,			//u8	alarm_hour;
	0,			//u8	alarm_minute;
};

enum{
	ERR_HEX_VALUE = 0xff,
};


static u8 is_leap_year( u8 year );
static u16 get_total_days( u8 year, u8 month, u8 day );

static u16 check_rtc_value( RtcDataOrg * rtc );
static void read_rtc_info( RtcDataOrg * rtc );
static void read_clock(  RtcDataOrg * rtc );

//=========================================================================
//	�֐�
//=========================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//--------------------------------------------------------------
//	���荞�݋֎~
//--------------------------------------------------------------
static void ENTER_CS()
{
	ime_bak = *(vu16 *)REG_IME;
	*(vu16 *)REG_IME = 0;
}

//--------------------------------------------------------------
//	���荞�݋֎~����
//--------------------------------------------------------------
static void EXIT_CS()
{
	*(vu16 *)REG_IME = ime_bak;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
//--------------------------------------------------------------
//	BCD���l���ʏ퐔�l�ɕϊ�
//--------------------------------------------------------------
static int BCD2DEC( u8 value )
{
	if ( value >= 0xa0 || (value & 0x0f) >= 0x0a ) return ERR_HEX_VALUE;
	return ( value>>4 & 0x0f ) * 10 + ( value & 0xf );
}

//-------------------------------------------------------------------------
//	��v�Z�֘A
//���o�ߓ����̌v�Z��99/12/31����ɂ��Ă���
//-------------------------------------------------------------------------
static const int m_days[] = {
//	1  2  3  4  5  6  7  8  9 10 11 12
	31,28,31,30,31,30,31,31,30,31,30,31
};

//--------------------------------------------------------------
// ���邤�N����
//--------------------------------------------------------------
static u8 is_leap_year( u8 year )
{
	if ( ( year % 4 == 0 && year % 100 != 0 ) || year % 400 == 0 )
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------
//	�o�ߓ����̌v�Z
//--------------------------------------------------------------
static u16 get_total_days( u8 year, u8 month, u8 day )
{
	u16 total;
	int n;

	total = 0;

	for ( n=year-1; n>0; n-- ) {
		total += 365;
		if ( is_leap_year( n ) == TRUE ) total ++;
	}
	
	for ( n=0; n<month-1; n++ ) {
		total += m_days[n];
	}

	if ( month > 2 && is_leap_year( year ) == TRUE ) {
		total += 1;
	}

	total += day;

	return total;
}

//--------------------------------------------------------------
//	�o�ߓ����̌v�Z�iRTC�\���̂���Z�o�j
//--------------------------------------------------------------
static u16 get_total_days_RTC( RtcDataOrg * r )
{
	return get_total_days( BCD2DEC(r->year), BCD2DEC(r->month), BCD2DEC(r->day) );
}



//=========================================================================
//
//
//	RTC�A�N�Z�X�֘A
//
//
//=========================================================================

//--------------------------------------------------------------
//	RTC���`�F�b�N�A�ُ킪����ꍇ�͏�����
//--------------------------------------------------------------
void RTC_Init( void )
{
	RtcStatus = 0;

	ENTER_CS();
	RtcIoEnable();
	FirstRtcStatus = RtcPowerOnCheck();
	EXIT_CS();

	if ( (FirstRtcStatus & 0xf) == 0 ) {
		//�A�N�Z�X���b�N��
		RtcStatus = RTCST_ERR_ACCESS_LOCK;
		return;
	}
	if ( FirstRtcStatus & 0xf0 ) {
		//�����������������Ȃ���
		RtcStatus = RTCST_INITIALIZE;
	} else {
		//�������������s��Ȃ�����
		RtcStatus = RTCST_NORMAL;
	}

	read_rtc_info( &rtc_val );
	RtcStatus = check_rtc_value( &rtc_val );
}

//--------------------------------------------------------------
//	RTC�̏�Ԃ�Ԃ�
//--------------------------------------------------------------
u16 RTC_GetStatus( void )
{
	return RtcStatus;
}

//--------------------------------------------------------------
//	���Ԃ̂�݂Ƃ�
//	��RTC�G���[���ɂ͊�{���Ԃ�Ԃ�
//--------------------------------------------------------------
static void read_clock( RtcDataOrg * rtc )
{
	if ( RtcStatus & RTCST_ERR_MASK ) {
		*rtc = rtc_initializer;
	} else {
		read_rtc_info( rtc );
	}
}

//--------------------------------------------------------------
//	���ԃf�[�^�̎擾
//--------------------------------------------------------------
static void read_rtc_date(RtcDataOrg * rtc)
{
	ENTER_CS();
	RtcReadDate( rtc );
	EXIT_CS();
}

static void read_rtc_stat(RtcDataOrg * rtc)
{
	ENTER_CS();
	RtcReadStat( rtc );
	EXIT_CS();
}

static void read_rtc_info( RtcDataOrg * rtc )
{
	read_rtc_stat(rtc);
	read_rtc_date(rtc);
}

//--------------------------------------------------------------
//	���ԃf�[�^�̌��`�F�b�N
//--------------------------------------------------------------
static u16 check_rtc_value( RtcDataOrg * rtc )
{
	u16 stat;
	int year,month,value;

	stat = RTCST_NORMAL;

	// POWER�t���O��ON�F�d���ቺ
	if ( rtc->stat & RTC_POWER_FLAG )
		stat |= RTCST_ERR_LOW_BATTERY;

	//BACKUP�t���O��OFF�F�ŏ��̓d������or���v�ُ�
	if ( (rtc->stat & RTC_BKUP_FLAG) == 0 )
		stat |= RTCST_ERR_NO_BACKUP;

	year = BCD2DEC(rtc->year);
	if ( year == ERR_HEX_VALUE )
		stat |= RTCST_ERR_ILLEGAL_YEAR;

	month = BCD2DEC(rtc->month);
	if ( month == ERR_HEX_VALUE || month == 0 || month > 12 )
		stat |= RTCST_ERR_ILLEGAL_MONTH;

	value = BCD2DEC( rtc->day );
	if ( value == ERR_HEX_VALUE )
		stat |= RTCST_ERR_ILLEGAL_DAY;
	if ( month == 2 ) {
		if ( value > is_leap_year(year) + m_days[2-1] )
			stat |= RTCST_ERR_ILLEGAL_DAY;
	} else {
		if ( value > m_days[month-1] )
			stat |= RTCST_ERR_ILLEGAL_DAY;
	}

	value = BCD2DEC( rtc->hour );
	if ( value == ERR_HEX_VALUE || value > 24 )
		stat |= RTCST_ERR_ILLEGAL_HOUR;

	value = BCD2DEC( rtc->minute );
	if ( value == ERR_HEX_VALUE || value > 60 )
		stat |= RTCST_ERR_ILLEGAL_MINUTE;

	value = BCD2DEC( rtc->second );
	if ( value == ERR_HEX_VALUE || value > 60 )
		stat |= RTCST_ERR_ILLEGAL_SECOND;

	return stat;
}

//--------------------------------------------------------------
//	RTC�̃��Z�b�g�Ăяo��
//--------------------------------------------------------------
void RTC_Reset(void)
{
	ENTER_CS();
	RtcReset();
	EXIT_CS();
}

//=========================================================================
//
//		�����񐶐��֘A
//
//=========================================================================
//--------------------------------------------------------------
//	���ԕ�������擾����
//--------------------------------------------------------------
void MakeTimeString( u8 * buf, int hour, int min, int sec )
{
	buf = PM_NumMsgSet( buf, hour, NUM_MODE_ZERO, 2);
	*buf = colon_; buf++;
	buf = PM_NumMsgSet( buf, min, NUM_MODE_ZERO, 2);
	*buf = colon_; buf++;
	buf = PM_NumMsgSet( buf, sec, NUM_MODE_ZERO, 2);
	*buf = EOM_;
}

void MakeTimeStringBCD( u8 * buf, int hour, int min, int sec )
{
	buf = PM_HexNumMsgSet( buf, hour, NUM_MODE_ZERO, 2);
	*buf = colon_; buf++;
	buf = PM_HexNumMsgSet( buf, min, NUM_MODE_ZERO, 2);
	*buf = colon_; buf++;
	buf = PM_HexNumMsgSet( buf, sec, NUM_MODE_ZERO, 2);
	*buf = EOM_;
}

void RTC_DebugMakeTimeString( u8 * buf )
{
	MakeTimeStringBCD( buf, rtc_val.hour, rtc_val.minute, rtc_val.second );
}

//--------------------------------------------------------------
//	���t��������擾����
//--------------------------------------------------------------
void MakeDateString( u8 * buf, int year, int month, int day )
{
	buf = PM_NumMsgSet( buf, year, NUM_MODE_ZERO, 4);
	*buf = bou_; buf++;
	buf = PM_NumMsgSet( buf, month, NUM_MODE_ZERO, 2);
	*buf = bou_; buf++;
	buf = PM_NumMsgSet( buf, day, NUM_MODE_ZERO, 2);
	*buf = EOM_;
}

void MakeDateStringBCD( u8 * buf, int year, int month, int day )
{
	buf = PM_HexNumMsgSet( buf, year, NUM_MODE_ZERO, 4);
	*buf = bou_; buf++;
	buf = PM_HexNumMsgSet( buf, month, NUM_MODE_ZERO, 2);
	*buf = bou_; buf++;
	buf = PM_HexNumMsgSet( buf, day, NUM_MODE_ZERO, 2);
	*buf = EOM_;
}

#ifdef	PM_DEBUG
void RTC_DebugMakeDateString( u8 * buf )
{
	MakeDateStringBCD( buf, rtc_val.year, rtc_val.month, rtc_val.day );
}

void RTC_DebugMakeDayString( u8 * buf )
{
	u16 days = get_total_days_RTC( &rtc_val );
	PM_NumMsgSet( buf, days, NUM_MODE_SPACE, 4 );
}

void RTC_DebugMakeStatString(u8 * buf)
{
	PM_HexNumMsgSet(buf, rtc_val.stat, NUM_MODE_ZERO, 2);
}
#endif


//=========================================================================
//
//
//	���ԑ���p�c�[���֘A
//
//
//=========================================================================

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//--------------------------------------------------------------
//	���ԍ����̎Z�o���[�`��
//	ans �� rtc - diff
//--------------------------------------------------------------
static void calc_time_offset( RtcDataOrg * rtc, GAME_TIME * ans, GAME_TIME * diff )
{
	u16 days;

	days = get_total_days_RTC( rtc );

	ans->sec = BCD2DEC(rtc->second) - diff->sec;
	ans->min = BCD2DEC(rtc->minute) - diff->min;
	ans->hour = BCD2DEC(rtc->hour) - diff->hour;
	ans->day = days - diff->day;

	if ( ans->sec < 0 ) {
		ans->sec += 60;
		ans->min --;
	}
	if ( ans->min < 0 ) {
		ans->min += 60;
		ans->hour --;
	}
	if ( ans->hour < 0 ) {
		ans->hour += 24;
		ans->day --;
	}
}


//--------------------------------------------------------------
//	�Q�[�������Ԃ̎Z�o
//--------------------------------------------------------------
void GetGameTime( void )
{
	read_clock( &rtc_val );
	calc_time_offset( &rtc_val, &GameTime, &MyData.diff_time );
}

//--------------------------------------------------------------
//	�Q�[�������ԗp�Ɏ����ԂƐݒ肵�������ԂƂ̍��������߂�
//--------------------------------------------------------------
void SetGameTime( int hour, int min )
{
	SetAllGameTime( 0, hour, min, 0 );
}

//--------------------------------------------------------------
//	���t�`�b�܂ŁA���ׂĂ̍������Z�b�g����i�f�o�b�O�p�j
//--------------------------------------------------------------
void SetAllGameTime( int day, int hour, int min, int sec )
{
	GameTime.day = day;
	GameTime.hour = hour;
	GameTime.min = min;
	GameTime.sec = sec;
	read_clock( &rtc_val );
	calc_time_offset( &rtc_val, &MyData.diff_time, &GameTime );
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//--------------------------------------------------------------
//	GAME_TIME�\���̂̍������Z�o
//--------------------------------------------------------------
void CalcGameTimeDiff( GAME_TIME *ans, const GAME_TIME *old_time, const GAME_TIME *new_time )
{
	ans->sec = new_time->sec - old_time->sec;
	ans->min = new_time->min - old_time->min;
	ans->hour = new_time->hour - old_time->hour;
	ans->day = new_time->day - old_time->day;

	if ( ans->sec < 0 ) {
		ans->sec += 60;
		ans->min --;
	}
	if ( ans->min < 0 ) {
		ans->min += 60;
		ans->hour --;
	}
	if ( ans->hour < 0 ) {
		ans->hour += 24;
		ans->day --;
	}
}

//--------------------------------------------------------------
//	���ݎ��Ԃ�b�P�ʂŎ擾
//	�������̏������Ɏg������
//--------------------------------------------------------------
u32 GetRealTimeMinute(void)
{
	u32 total;
	u32 days;

	read_clock(&rtc_val);
	days = get_total_days_RTC(&rtc_val);
	total = days * 24 * 60 + rtc_val.hour * 60 + rtc_val.minute;

	return total;
}

#ifdef	PM_DEBUG
void TestRtcRead(void)
{
	read_rtc_info(&rtc_val);
}
#endif	PM_DEBUG

