//=========================================================================
//
//
//	�C�x���g�t���O�E���[�N����֘A
//
//
//	2001.10.23	GAME FREAK Inc.
//
//	2001.11.07	by tama
//		�X�N���v�g�E�֐��o������A�N�Z�X����郏�[�N�̒�`��script.c����ړ�
//
//=========================================================================


#include "common.h"

#define	EV_FLAG_H_GLOBAL
#include "ev_flag.h"

#include "../script/savework.h"
#include "syswork.h"
#include "sysflag.h"

#include "zukan.h"

//-------------------------------------------------------------------------
//	��`�Ȃ�
//-------------------------------------------------------------------------
enum{
	LOCAL_WORK_SIZE = LOCALWORK15 - LOCALWORK0 + 1,
};

//-------------------------------------------------------------------------
//	�ϐ�
//-------------------------------------------------------------------------
// �X�N���v�g�����ł̏����p
u16 ScriptRegister0 = 0;
u16 ScriptRegister1 = 0;
u16 ScriptRegister2 = 0;
u16 ScriptRegister3 = 0;

// �X�N���v�g�ƃv���O�����̃f�[�^�����p
u16 ScriptParameter0 = 0;
u16 ScriptParameter1 = 0;
u16 ScriptParameter2 = 0;
u16 ScriptParameter3 = 0;

// �X�N���v�g�ł̃e���|����
u16 ScriptTempWork0 = 0;
u16 ScriptTempWork1 = 0;
u16 ScriptTempWork2 = 0;
u16 ScriptTempWork3 = 0;

// �X�N���v�g�ɓ�����Ԃ��ėp���[�N
u16 AnswerWork = 0;

// �b�������Ώۂ̂n�a�i�̂h�c
u16 TargetObjID = 0;

// �b������������ێ����郏�[�N
u16 TalkSiteWork = 0;

static u8 EventCtrlFlag[CTRLFLAG_AREA_MAX] = {};

//=========================================================================
//
//	�������E���Z�b�g����֘A
//
//=========================================================================
extern u16 * const ScriptWorkTable[];
extern u16 * const ScriptWorkTableEnd[];

//-------------------------------------------------------------
//	�C�x���g�t���O�E���[�N�̏�����
//-------------------------------------------------------------
void EventDataInit( void )
{
	memset( Fld.EventFlag, 0x00, sizeof(Fld.EventFlag) );	// �Z�[�u�t���O
	memset( Fld.EventWork, 0x00, sizeof(Fld.EventWork) );	// �Z�[�u���[�N
	memset( EventCtrlFlag, 0x00, sizeof(EventCtrlFlag) );	// ����t���O�i�Z�[�u���Ȃ��j
}

//-------------------------------------------------------------
//	�}�b�v������̃t���O���N���A����
//		���}�b�v�J�ڃ^�C�~���O�ŌĂ΂��
//-------------------------------------------------------------
void LocalEventFlagClear( void )
{
	//���[�J���t���O�̃N���A 8*4 = 32��
	memset( (u8 *)&(Fld.EventFlag[0]), 0x00, 4 );

	//���[�J�����[�N�̃N���A
	memset( (u8 *)&(Fld.EventWork[0]), 0x00,
			sizeof(Fld.EventWork[0]) * LOCAL_WORK_SIZE );

	EventFlagReset( SYS_ENC_UP_ITEM );
	EventFlagReset( SYS_ENC_DOWN_ITEM );
	EventFlagReset( SYS_USE_WAZA_KAIRIKI );

	//OBJ�����^�C�~���O�̐���ɂ���
	//�i�퓬��A��ʕ��A�O�Ƀ|�P����OBJ�����������ꍇ�Ȃǁj
	EventFlagReset( SYS_CTRL_OBJ_DELETE );
}

//-------------------------------------------------------------
//	����o�߂��ƂɃN���A�����t���O���N���A����
//-------------------------------------------------------------
void OneDayFlagClear( void )
{
	u8 * p;
	p = (u8 *)&(Fld.EventFlag[TIMEFLAG_START/8]);
	memset( p, 0x00, TIMEFLAG_AREA_MAX );
}

//-------------------------------------------------------------
//	������̑S�����[�h�A�z�E�G�����[�h�֘A
//-------------------------------------------------------------
#define	ZUKAN_ENABLE_WORD	0x302
#define ZUKAN_ENABLE_BYTE	0xda

//	������
void ZukanSpecialFlagInit( void )
{
	u16 * wk = GetEventWorkAdrs( WK_SPECIAL_ZUKAN );

	MyData.Zukan.ZenkokuZukanFlag = 0;
	*wk = 0;
	EventFlagReset( SYS_SPECIAL_ZUKAN );
}

//	�Z�b�g
void ZukanSpecialFlagSet( void )
{
	u16 * wk = GetEventWorkAdrs( WK_SPECIAL_ZUKAN );

	MyData.Zukan.ZenkokuZukanFlag = ZUKAN_ENABLE_BYTE;
	*wk = ZUKAN_ENABLE_WORD;
	EventFlagSet( SYS_SPECIAL_ZUKAN );
	MyData.Zukan.ListMode = ZLIST_WORLD;
	MyData.Zukan.ListSort = ZSORT_NUM;
	ZukanWorkInit();
}

//	�`�F�b�N
int ZukanSpecialFlagCheck( void )
{
	if ( MyData.Zukan.ZenkokuZukanFlag != ZUKAN_ENABLE_BYTE ) return FALSE;
	if ( GetEventWorkValue( WK_SPECIAL_ZUKAN ) != ZUKAN_ENABLE_WORD ) return FALSE;
	if ( EventFlagCheck( SYS_SPECIAL_ZUKAN ) == FALSE ) return FALSE;
	return TRUE;
}

//-------------------------------------------------------------
//	�O���f�[�^�̎g�p���֘A
//-------------------------------------------------------------
//�N���A
void ExDataSpecialFlagInit( void )
{
	EventFlagReset( SYS_EXDATA_ENABLE );
}

//�Z�b�g
void ExDataSpecialFlagSet( void )
{
	EventFlagSet( SYS_EXDATA_ENABLE );
}

//�`�F�b�N
int ExDataSpecialFlagCheck( void )
{
	return EventFlagCheck( SYS_EXDATA_ENABLE );
}

//-------------------------------------------------------------
//	�Q�[�������Ԃ̍Đݒ苖�֘A
//-------------------------------------------------------------
#define	RTCRESET_ENABLE_WORD	0x0920

void GameTimeSpecialFlagInit(void)
{
	SetEventWorkValue(WK_GAMETIME_RESET, 0);
	EventFlagReset(SYS_GAMETIME_RESET);
}

void GameTimeSpecialFlagSet(void)
{
	SetEventWorkValue(WK_GAMETIME_RESET, RTCRESET_ENABLE_WORD);
	EventFlagSet(SYS_GAMETIME_RESET);
}

int GameTimeSpecialFlagCheck(void)
{
	if (EventFlagCheck(SYS_GAMETIME_RESET) == FALSE)
		return FALSE;

	if (GetEventWorkValue(WK_GAMETIME_RESET) != RTCRESET_ENABLE_WORD)
		return FALSE;

	return TRUE;
}

//=========================================================================
//	�C�x���g���[�N�֘A
//=========================================================================
//-------------------------------------------------------------
//	u16 �T�C�Y�̃��[�N�A�h���X���擾
//	<����>
//		���[�N�h�c
//		0 -			�ʏ�̃Z�[�u���[�N
//		0x8000-		�o�^�����v���O������̕ϐ�
//	<�߂�l>
//		���[�N�̃A�h���X
//-------------------------------------------------------------
u16 * GetEventWorkAdrs(u16 id)
{
	if( id < SVWK_START )return NULL;
	if( id < SCWK_START ) return &Fld.EventWork[id - SVWK_START];
	return ScriptWorkTable[ id - SCWK_START ];
}

//-------------------------------------------------------------
//	�C�x���g���[�N�̒l���擾
//	<����>
//			id		���[�N�h�c
//	<�߂�l>
//			���[�N�̒l
//-------------------------------------------------------------
u16 GetEventWorkValue(u16 id)
{
	u16 * res;
	res = GetEventWorkAdrs(id);
	if( res == NULL )return id;
	return *res;
}

//-------------------------------------------------------------
//	�C�x���g���[�N�ɒl���Z�b�g
//	<����>
//			id		���[�N�h�c
//			value	�Z�b�g����l
//	<�߂�l>
//			TRUE	�Z�b�g�ł���
//			FALSE	�Z�b�g���s
//-------------------------------------------------------------
int SetEventWorkValue( u16 id, u16 value )
{
	u16 * p;
	p = GetEventWorkAdrs( id );
	if ( p == NULL ) return FALSE;
	*p = value;
	return TRUE;
}

//-------------------------------------------------------------
// �X�N���v�g����w�肷��n�a�i�R�[�h���擾
// <����>
// 0 - 7
// <�߂�l>
// �n�a�i�L�����R�[�h
//-------------------------------------------------------------
u8 GetEvDefineObjCode( u8 no )
{
	return (u8)GetEventWorkValue( OBJCHRWORK1 + no );
}


//=========================================================================
//		�C�x���g�t���O�֘A
//=========================================================================
//-------------------------------------------------------------
//	�C�x���g�t���O�̃A�h���X���擾����
//	<����>		�t���O�h�c
//	<�߂�l>	�t���O�A�h���X
//				NULL�̏ꍇ������`
//-------------------------------------------------------------
static u8 * get_flag_adrs( u16 flag )
{
	if ( flag == 0 ) {
		return NULL;
	} else if ( flag < SCFLG_START ) {
		return &(Fld.EventFlag[flag/8]);
	} else {
		return &(EventCtrlFlag[ (flag-SCFLG_START)/8 ]);
	}
}

//-------------------------------------------------------------
//	�C�x���g�t���O���Z�b�g����
//	<����>
//		�t���O�h�c�i../script/saveflag.h���Q��)
//-------------------------------------------------------------
u8 EventFlagSet(u16 flag)
{
	u8 * p = get_flag_adrs( flag );
	if ( p != NULL ) {
		*p |= 1 << ( flag % 8 );
	}
	return 0;
}

//-------------------------------------------------------------
//	�C�x���g�t���O�����Z�b�g����
//	<����>
//		�t���O�h�c�i../script/saveflag.h���Q��)
//-------------------------------------------------------------
u8 EventFlagReset(u16 flag)
{
	u8 * p = get_flag_adrs( flag );
	if ( p != NULL ) {
		*p &= 0xff ^ (1 << ( flag % 8 ) );
	}
	return 0;
}

//-------------------------------------------------------------
//	�C�x���g�t���O���Z�b�g����
//	<����>
//		�t���O�h�c�i../script/saveflag.h���Q��)
//	<�߂�l>
//		ON/OFF
//-------------------------------------------------------------
u8 EventFlagCheck(u16 flag)
{
	u8 * p = get_flag_adrs( flag );
	if ( p != NULL ) {
		if ( *p & ( 1 << ( flag % 8 ) ) ) return ON;
	}
	return OFF;
}

