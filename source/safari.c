//=========================================================================
//
//
//		�T�t�@�����[�h
//
//	
//=========================================================================

#include "common.h"
#include "script.h"
#include "ev_check.h"
#include "server.h"
#include "fld_main.h"
#include "jiki.h"
#include "pm_str.h"
#include "cubecase.h"
#include "fld_task.h"
#include "fld_main.h"
#include "fld_tool.h"
#include "sysflag.h"
#include "record.h"

#define	__SAFARI_H_GLOBAL__
#include "safari.h"


extern const VM_CODE ev_safari_cancel_check[];
extern const VM_CODE ev_safari_end_by_step[];
extern const VM_CODE ev_safari_end_by_ball[];
extern const VM_CODE ev_safari_end_common[];
extern const VM_CODE ev_safari_end_core[];

//=========================================================================
//	��`
//=========================================================================


//=========================================================================
//	�ϐ�
//=========================================================================

//�T�t�@���{�[���̐�
u8 SafariBallCount = 0;

//�T�t�@���ł̕����J�E���^
u16 SafariStepCount = 0;

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

static void SafariCubeTableWalk(void);	// �L���[�u�u����̕����J�E���^

//-------------------------------------------------------------
//	�T�t�@�����[�h���ǂ����̃`�F�b�N
//-------------------------------------------------------------
int IsFieldSafariMode( void )
{
	return EventFlagCheck( SYS_SAFARI_MODE );
}

void SetFieldSafariMode( void )
{
	EventFlagSet( SYS_SAFARI_MODE );
}

void ClearFieldSafariMode( void )
{
	EventFlagReset( SYS_SAFARI_MODE );
}


//-------------------------------------------------------------------------
//	�T�t�@�����[�h�J�n
//-------------------------------------------------------------------------
void SafariModeStart( void )
{
	IncRecord( CNT_SAFARI );
	SetFieldSafariMode();
	SafariCubeTableInitAll();
	SafariBallCount = SAFARI_BALL_MAX;
	SafariStepCount = SAFARI_STEP_MAX;
}

//-------------------------------------------------------------------------
//	�T�t�@�����[�h�I��
//-------------------------------------------------------------------------
void SafariModeEnd( void )
{
	ClearFieldSafariMode();
	SafariCubeTableInitAll();
	SafariBallCount = 0;
	SafariStepCount = 0;
}


//-------------------------------------------------------------------------
//	�T�t�@���̕����`�F�b�N
//-------------------------------------------------------------------------
u8 SafariStepCheck( void )
{
	if ( IsFieldSafariMode() == FALSE ) return FALSE;

	SafariCubeTableWalk();		// �L���[�u�u����̕����J�E���^

	SafariStepCount --;
	if ( SafariStepCount == 0 ) {
		StartFieldScript( ev_safari_end_by_step );
		return TRUE;
	}
	return FALSE;
}

//-------------------------------------------------------------------------
//	�T�t�@�����[�h����߂邩�ǂ����������˂�X�N���v�g���Ăяo��
//-------------------------------------------------------------------------
void SafariCancelAskStart( void )
{
	StartFieldScript( ev_safari_cancel_check );
}

//-------------------------------------------------------------------------
//	�T�t�@���ł̃G���J�E���g��I����̏���
//-------------------------------------------------------------------------
void SafariEncountEnd( void )
{
	if ( SafariBallCount != 0 ) {
		MainProcChange( FieldMainRecover );
	}
	else if ( WinLoseFlag == FIGHT_SAFARIBALL_NONE ) {
		DoSpecialScript( ev_safari_end_core );
		SetNextWorldPosition();
		pFieldRecoverFunc = AddFieldEncountEndFadeInTask;//OBJ�|�[�Y������
		MainProcChange( FieldMainInit );
	}
	else if ( WinLoseFlag == FIGHT_ENEMY_GET ) {
		StartFieldScript( ev_safari_end_by_ball );
		StopFieldScript();
		MainProcChange( FieldScriptRecover );
	}
}


//=========================================================================
//	�L���[�u�u����
//=========================================================================
#define	CUBE_TABLE_MAX	10

typedef struct {
	s16	px;			// X���W
	s16	py;			// Y���W
	s8	map_id;		// �}�b�vID
	u8	cnt;		// �����J�E���^
	CUBE	cube;	// �L���[�u
//	u8	type;		// �L���[�u�̐F
//	s8	param[5];	// ��
}CUBE_TABLE;

static CUBE_TABLE	put_cube[ CUBE_TABLE_MAX ] = {};

//-------------------------------------------------------------------------
//	�L���[�u�u����̃f�[�^��������
//-------------------------------------------------------------------------
void SafariCubeTableInit( u8 no )
{
	memset( &put_cube[no], 0, sizeof(CUBE_TABLE) );
}
// �S��
void SafariCubeTableInitAll(void)
{
	memset( put_cube, 0, sizeof(CUBE_TABLE)*CUBE_TABLE_MAX );
}

//-------------------------------------------------------------------------
//	�L���[�u�u����ɃL���[�u���u����Ă��邩
//	�u����Ă���   : AnswerWork = i
//	�u����Ă��Ȃ� : AnswerWork = 0xffff
//-------------------------------------------------------------------------
// �ڂ̑O
void SafariCubeTableCheck(void)
{
	u16	i;
	s16	x,y;

	GetHeroFrontCellPos( &x, &y );

	for( i=0; i<CUBE_TABLE_MAX; i++ ){
		if( Fld.MapID.map_id == put_cube[i].map_id &&
			put_cube[i].px == x && put_cube[i].py == y )
		{
			AnswerWork = i;
			PM_strcpy( StrTempBuffer0, cube_name[ put_cube[i].cube.type ] );
			return;
		}
	}
	AnswerWork = 0xffff;
}
// 5���ȓ�
void SafariCubeTableCheck2(void)
{
	u16	i;
	s16	x,y;

	GetHeroCellPos( &x, &y );

	for( i=0; i<CUBE_TABLE_MAX; i++ ){
		if( Fld.MapID.map_id == put_cube[i].map_id ){
			x = x - put_cube[i].px;
			y = y - put_cube[i].py;
			if( x < 0 )	x *= (-1);
			if( y < 0 )	y *= (-1);
			if( x + y <= 5 ){
				AnswerWork = i;
				return;
			}
		}
	}
	AnswerWork = 0xffff;
}

//-------------------------------------------------------------------------
//	�L���[�u�u����̃L���[�u�̃p�����[�^���擾
//	�߂�l : �u����Ă���   = �L���[�u�f�[�^�̃A�h���X
//		   : �u����Ă��Ȃ� = NULL
//-------------------------------------------------------------------------
// �ڂ̑O
CUBE * SafariCubeTableGet(void)
{
	SafariCubeTableCheck();

	if( AnswerWork == 0xffff )	return	NULL;

	return	&put_cube[AnswerWork].cube;
}
// 5���ȓ�
CUBE * SafariCubeTableGet2(void)
{
	SafariCubeTableCheck2();

	if( AnswerWork == 0xffff )	return	NULL;

	return	&put_cube[AnswerWork].cube;
}

//-------------------------------------------------------------------------
//	�L���[�u�u����ɃL���[�u���Z�b�g
//	���� : no = �莝���L���[�u�̔ԍ�
//-------------------------------------------------------------------------
void SafariCubeTableSet( u8 no )
{
	u8	i;
	s16	x,y;

	for( i=0; i<CUBE_TABLE_MAX; i++ ){
		if( put_cube[i].map_id == 0 &&
			put_cube[i].px == 0 && put_cube[i].py == 0 )
		{
			GetHeroFrontCellPos( &x, &y );
			put_cube[i].map_id = Fld.MapID.map_id;
			put_cube[i].cube   = Fld.MyCube[no];
			put_cube[i].cnt    = 100;
			put_cube[i].px     = x;
			put_cube[i].py     = y;
			return;
		}
	}
}

//-------------------------------------------------------------------------
//	�L���[�u�u����̕����J�E���^
//-------------------------------------------------------------------------
static void SafariCubeTableWalk(void)
{
	u8	i;

	for( i=0; i<CUBE_TABLE_MAX; i++ ){
		if( put_cube[i].cnt != 0 ){
			put_cube[i].cnt--;
			if( put_cube[i].cnt == 0 )
				SafariCubeTableInit(i);
		}
	}
}

//-------------------------------------------------------------------------
//	�f�o�b�O�p�L���[�u���擾
//	<�߂�l>	FALSE		�L���[�u�����݂��Ȃ�
//				TRUE		�L���[�u����
//	StrTempBuffer0 = ���O�AStrTempBuffer1 = �c�����
//-------------------------------------------------------------------------
int DebugSafariCubeInfo( void )
{
	SafariCubeTableCheck();
	if ( AnswerWork == 0xffff ) {
		return FALSE;
	}
	PM_NumMsgSet( StrTempBuffer1, put_cube[AnswerWork].cnt, NUM_MODE_ZERO, 3 );
	return TRUE;
}

