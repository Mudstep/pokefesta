#ifndef __RTCTOOL_H__
#define __RTCTOOL_H__

//=========================================================================
//	
//
//
//
//=========================================================================

#undef GLOBAL
#ifdef __RTCTOOL_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

/* RTC_GetStatus �p�̒�` */

#define	RTCST_NORMAL				0x0000	/*	���� */

#define	RTCST_ERR_ACCESS_LOCK		0x0001	/*	�A�N�Z�X���b�N */
#define	RTCST_INITIALIZE			0x0002	/*	������ */

#define	RTCST_ERR_NO_BACKUP			0x0010	/*	�o�b�N�A�b�v�ُ�(���邢�͏���N���� */
#define	RTCST_ERR_LOW_BATTERY		0x0020	/*	�d���ቺ */
#define	RTCST_ERR_ILLEGAL_YEAR		0x0040	/*	�N�f�[�^�ُ̈� */
#define	RTCST_ERR_ILLEGAL_MONTH		0x0080	/*	���f�[�^�ُ̈� */
#define	RTCST_ERR_ILLEGAL_DAY		0x0100	/*	���f�[�^�ُ̈� */
#define	RTCST_ERR_ILLEGAL_HOUR		0x0200	/*	���f�[�^�ُ̈� */
#define	RTCST_ERR_ILLEGAL_MINUTE	0x0400	/*	���f�[�^�ُ̈� */
#define	RTCST_ERR_ILLEGAL_SECOND	0x0800	/*	�b�f�[�^�ُ̈� */

#define	RTCST_ERR_MASK				0x0ff0	/*	��L�G���[�̂����ꂩ */


//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

//�Q�[�������Ԋi�[�p�ϐ�
extern GAME_TIME GameTime;

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

//--------------------------------------------------------------
//	RTC���`�F�b�N�A�ُ킪����ꍇ�͏�����
//--------------------------------------------------------------
GLOBAL void RTC_Init( void );

//--------------------------------------------------------------
//	RTC�̏�Ԃ�Ԃ�
//--------------------------------------------------------------
GLOBAL u16 RTC_GetStatus( void );

//--------------------------------------------------------------
//	RTC�̃��Z�b�g�Ăяo��
//--------------------------------------------------------------
GLOBAL void RTC_Reset(void);

GLOBAL void SetAllGameTime( int day, int hour, int min, int sec );
GLOBAL void SetGameTime( int hour, int min );
GLOBAL void GetGameTime( void );

//--------------------------------------------------------------
//	GAME_TIME�\���̂̍������Z�o
//--------------------------------------------------------------
GLOBAL void CalcGameTimeDiff( GAME_TIME *ans,
		const GAME_TIME *old_time, const GAME_TIME *new_time );

//--------------------------------------------------------------
//	���ݎ��Ԃ�b�P�ʂŎ擾
//	�������̏������Ɏg������
//--------------------------------------------------------------
GLOBAL u32 GetRealTimeMinute(void);

#ifdef	PM_DEBUG

/* �f�o�b�O�p�����񐶐� */
GLOBAL void RTC_DebugMakeTimeString( u8 * buf );
GLOBAL void RTC_DebugMakeDateString( u8 * buf );
GLOBAL void RTC_DebugMakeDayString( u8 * buf );
GLOBAL void RTC_DebugMakeStatString(u8 * buf);
GLOBAL void MakeTimeString( u8 * buf, int hour, int min, int sec );
GLOBAL void MakeDateString( u8 * buf, int year, int month, int day );

#endif	PM_DEBUG

#undef GLOBAL
#endif	/*__RTCTOOL_H__*/

