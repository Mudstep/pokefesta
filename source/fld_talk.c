//=========================================================================
//
//	�t�B�[���h�ł̃��b�Z�[�W�E�B���h�E����
//
//	2001.03.28 by tama
//
//=========================================================================

#include "common.h"
#include "print.h"
#include "window.h"

#include "field.h"

#define FLD_TALK_H_GLOBAL
#include "fld_talk.h"

#include "evobj.h"
#include "evobjmv.h"
#include "jiki.h"
#include "task.h"

#include "palanm.h"
#include "screen.h"
#include "menutool.h"
#include "ev_check.h"
#include "script.h"


static u8 field_msg_status;

static TALK_WIN	fld_win = {};


static void field_msg_set(const u8 * msg_adrs );
static void field_kaiwa_msg_set( void );

//=========================================================================
//	���b�Z�[�W�E�B���h�E�֘A
//=========================================================================

//-------------------------------------------------------------------------
//	����������
//-------------------------------------------------------------------------
void FieldMsgInit(void)
{
	field_msg_status = FLDMSG_STAT_OFF;
	KWinFrameInit( MenuKWinStartNo );
	MsgControlInit( &fld_win, FIELD_MENU_TYPE );
};

//-------------------------------------------------------------------------
//	���b�Z�[�W�\���̐���
//	���@���C�����[�v����^�X�N�Ƃ��ď�ɌĂ΂��
//-------------------------------------------------------------------------
static void field_msg_task(u8 no)
{
	TASK_TABLE * my = &TaskTable[no];

	switch(my->work[0]){

	case 0:
		KWinFrameChrSet( &fld_win );
		my->work[0]++;
		break;

	case 1:
		KWinFrameScrnWrite( &fld_win );
		my->work[0]++;
		break;

	case 2:
		switch( field_msg_status ){
		case FLDMSG_STAT_NORMAL:
			if( TalkMsgPut( &fld_win ) == 0 ) return;
			break;

		case FLDMSG_STAT_NOSKIP:
			if( NoSkipTalkMsgPut( &fld_win ) == 0 ) return;
			break;
		}
		field_msg_status = FLDMSG_STAT_OFF;
		DelTask(no);
	};
}

static void add_field_msg_task( void )
{
	AddTask( field_msg_task, 80 );
}

static void del_field_msg_task( void )
{
	u8 no;
	no = CheckTaskNo( field_msg_task );
	if ( no == NOT_EXIST_TASK_ID ) return;
	DelTask(no);
}

//-------------------------------------------------------------------------
//�@���b�Z�[�W�\���̃Z�b�g
//-------------------------------------------------------------------------
u8 FieldMsgSet( const u8 * msg_adrs )
{
	if(field_msg_status != FLDMSG_STAT_OFF) return FALSE;

	field_msg_set( msg_adrs );
	field_msg_status = FLDMSG_STAT_NORMAL;
	return TRUE;
}

//-------------------------------------------------------------------------
//�@���b�Z�[�W�\���̃Z�b�g�iMSG����Ȃ��j
//-------------------------------------------------------------------------
u8 FieldMsgSetNoSkip( const u8 * msg_adrs )
{
	if(field_msg_status != FLDMSG_STAT_OFF) return FALSE;

	field_msg_status = FLDMSG_STAT_NOSKIP;
	field_msg_set( msg_adrs );
	return TRUE;
}

//-------------------------------------------------------------------------
//�@���b�Z�[�W�\���̋����Z�b�g�i�f�o�b�O�p�j
//-------------------------------------------------------------------------
u8 FieldMsgSetForce( const u8 * msg_adrs )
{
	field_msg_status = FLDMSG_STAT_NOSKIP;
	field_msg_set( msg_adrs );
	return TRUE;	/* �߂�l�ɈӖ��͂Ȃ��B�Ƃ肠���� */
}

//-------------------------------------------------------------------------
//	�ȈՉ�b�f�[�^�\�����Z�b�g
//-------------------------------------------------------------------------
u8 FieldKaiwaMsgSet( void )
{
	if(field_msg_status != FLDMSG_STAT_OFF) return FALSE;

	field_msg_status = FLDMSG_STAT_NORMAL;
	field_kaiwa_msg_set();
	return TRUE;
}

//-------------------------------------------------------------------------
//<����>
//	msg_adrs	���b�Z�[�W�f�[�^�A�h���X
//-------------------------------------------------------------------------
static void field_msg_set(const u8 * msg_adrs )
{
	PM_MsgExpand(MsgExpandBuffer, msg_adrs);
	TalkMsgSet( &fld_win, MsgExpandBuffer, MenuFontStartNo, MWIN_MSGXPOS, MWIN_MSGYPOS );
	add_field_msg_task();
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
static void field_kaiwa_msg_set( void )
{
	// MsgExpandBuffer �ɊȈՉ�b���e��W�J
	TalkMsgSet( &fld_win, MsgExpandBuffer, MenuFontStartNo, MWIN_MSGXPOS, MWIN_MSGYPOS );
	add_field_msg_task();
}

//-------------------------------------------------------------------------
//	�t�B�[���h���b�Z�[�W�E�B���h�E���N���A
//-------------------------------------------------------------------------
void FieldMsgClear( void )
{
	del_field_msg_task();
	KWinFrameClear( &fld_win );
	field_msg_status = FLDMSG_STAT_OFF;
}

//-------------------------------------------------------------------------
// ���b�Z�[�W�̏�Ԃ�Ԃ�
//-------------------------------------------------------------------------
u8 FieldMsgStatusCheck(void)
{
		return field_msg_status;
};

//-------------------------------------------------------------------------
//	���b�Z�[�W�̏I���҂�
//	<�߂�l>
//	TRUE	MSG�\���I��
//	FALSE	MSG�\����
//-------------------------------------------------------------------------
u8 FieldMsgWait(void)
{
		if(field_msg_status == FLDMSG_STAT_OFF)return TRUE;
		return FALSE;
};

//-------------------------------------------------------------------------
//	�t�B�[���h���b�Z�[�W���L�����Z��
//	���E�B���h�E��������
//-------------------------------------------------------------------------
void FieldMsgCancel(void)
{
	del_field_msg_task();
	KWinFrameScrnWrite( &fld_win );
	field_msg_status = FLDMSG_STAT_OFF;
}

//=========================================================================
//	�t�B�[���h�n�a�i����֘A
//=========================================================================

u8 EvWaitHeroMoveEnd(void)
{
		if(MyMoveState.mv_state == EV_M_ON)return FALSE;
		return TRUE;
};

//-------------------------------------------------------------------------
//	��b�J�n�������F���@�̓���I���҂�����
//-------------------------------------------------------------------------
static void talk_start_task(u8 no)
{
	TASK_TABLE * my;
	my = &TaskTable[no];

	if( EvWaitHeroMoveEnd() == FALSE ) return;

	HeroAcmdForceStop();
	DelTask(no);
}

u8 EvWaitTalkStart( void )
{
	if( CheckTask( talk_start_task ) )return FALSE;
	SetHeroMoveStateInit();		//���@��ԁi�_�j�G���Ƃ��c�j�̃N���A
	return TRUE;
}

void AddTalkStartTask( void )
{
	EvObjPauseAll();
	AddTask( talk_start_task, 80 );
}


//-------------------------------------------------------------------------
//	��b�J�n�������F�b���������Ώۂ̂n�a�i�Ǝ��@�̓���I���҂�����
//-------------------------------------------------------------------------
enum{
	HERO_STOP_FLAG,
	TARGET_STOP_FLAG,
};

static void talk_obj_start_task(u8 no)
{
	TASK_TABLE * my = &TaskTable[no];

	if( my->work[HERO_STOP_FLAG] == 0 && EvWaitHeroMoveEnd() == TRUE ){
		HeroAcmdForceStop();				//���@������I�����Ă���΃|�[�Y
		my->work[HERO_STOP_FLAG] = 1;
	}

	if( my->work[TARGET_STOP_FLAG] == 0 && EvObj[TargetObjNo].move_sw == 0 ){
		EvObjPause( &EvObj[TargetObjNo] );		//�Ώۂ��b�������\�ȏ�ԂȂ�|�[�Y
		my->work[TARGET_STOP_FLAG] = 1;
	};

	if( my->work[HERO_STOP_FLAG] == 0 || my->work[TARGET_STOP_FLAG] == 0 ) return;

	DelTask(no);
}

u8 EvWaitTalkObjStart( void )
{
	if( CheckTask( talk_obj_start_task ) )return FALSE;
	SetHeroMoveStateInit();		//���@��ԁi�_�j�G���Ƃ��c�j�̃N���A
	return TRUE;
}

void AddTalkObjStartTask( void )
{
	u8 no;

	EvObjPauseAlmost( TargetObjNo );			//���@�ƑΏۈȊO���|�[�Y
	no = AddTask( talk_obj_start_task, 80 );
	if( EvObj[TargetObjNo].move_sw == 0 ){		//�Ώۂ��b�������\�ȏ�ԂȂ�|�[�Y
		EvObjPause( &EvObj[TargetObjNo] );
		TaskTable[no].work[TARGET_STOP_FLAG] = 1;
	}
}

//-------------------------------------------------------------------------
//	��b�I��
//-------------------------------------------------------------------------
void TalkEnd(void)
{
	AcmdEnd(EvObjSpWorkAdrs(EV_OBJ_SPID));			// ���@�����A�j������
	AnmCmdFree();
	EvObjPauseClrAll();
}

void TalkObjEnd(void)
{
	if (EvObj[TargetObjNo].sw == 1) AcmdEnd(&EvObj[TargetObjNo]);
	AcmdEnd(EvObjSpWorkAdrs(EV_OBJ_SPID));			// ���@�����A�j������
	AnmCmdFree();
	EvObjPauseClrAll();
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void SetTalkObjSiteToHero( void )
{
	AcmdCodeTurnSiteSet( &EvObj[TargetObjNo], TalkSiteWork );
}

void ResetTalkObjSite( void )
{
	AcmdFree(&EvObj[TargetObjNo]);
}
