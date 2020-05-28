//=========================================================================
//	
//
//	�X�N���v�g�E�C�x���g����̐퓬�Ăяo������
//
//
//	2001.10.22	GAME FREAK Inc.
//=========================================================================

#include "common.h"
#include "print.h"
#include "field.h"
#include "server.h"
#include "fight.h"
#include "fld_main.h"
#include "fld_task.h"
#include "fld_talk.h"
#include "task.h"
#include "poketool.h"
#include "ev_flag.h"
#include "script.h"
#include "fld_tool.h"
#include "sxytype.h"
#include "mus_tool.h"
#include "calctool.h"
#include "message.h"

#include "map.h"
#include "../evdata/maparray.h"
#include "../data/trainer.def"
#include "sysflag.h"


#define EV_FIGHT_H_GLOBAL
#include "ev_fight.h"

extern void StartTrainerEncountEvent( void );
extern const VM_CODE TrainerTalkScript[];
extern const VM_CODE TrainerMoveScript[];
extern const VM_CODE TrainerEventScript[];
extern const VM_CODE Trainer2vs2Script[];
extern const VM_CODE TrainerRevengeScript[];
extern const VM_CODE Trainer2vs2RevengeScript[];

extern const VM_CODE test_bg_script[];
extern u8 TargetObjNo;


//=========================================================================
//	�ϐ�
//=========================================================================

static u16 trainer_type = 0;
u16 FieldTrainerID = 0;
static u16 trainer_obj = 0;
static const u8 * before_msg = NULL;
static const u8 * lose_msg = NULL;
static const u8 * win_msg = NULL;
static const u8 * no_fight_msg = NULL;
static const VM_CODE * after_script = NULL;
static const VM_CODE * battle_end_script = NULL;

//=========================================================================
//	��`
//=========================================================================

#define GET_TRN_FLAG(id)	( TRFLG_START+(id) )


enum{
	GET_1_BYTES,
	GET_2_BYTES,
	GET_4_BYTES,
	DMY_1_BYTES,
	DMY_2_BYTES,
	DMY_4_BYTES,
	GET_END_ADRS,
};

typedef struct {
	void * val_adrs;
	u8 val_type;
}TR_DATA_TYPE;


#define	TRDATA_CODE_SIZE	1

// �g���[�i�[�f�[�^�\��
// ��ev_cmd.s�̃g���[�i�[�f�[�^�L�q�Ƃ��ꂪ�Ȃ��悤�ɁI�I

/* �ʏ�g���[�i�[ */
#define TRDATA_TYPE		(0)				/* u8  �g���[�i�[�̎�� */
#define	TRDATA_ID		(1+0)			/* u16 �퓬ID�w�� */
#define	TRDATA_OBJ		(1+2)			/* u16 */
#define	TRDATA_BEFORE	(1+4)			/* u32 �ΐ�O�t�B�[���h�ł�MSG */
#define	TRDATA_LOSE		(1+8)			/* u32 �ΐ��̐퓬��ʂł�MSG */
#define TRDATA_END		(1+12)

/* �C�x���g�g���[�i�[�f�[�^ */
#define TRDATA_EV_TYPE		(0)				/* u8  �g���[�i�[�̎�� */
#define	TRDATA_EV_ID		(1+0)			/* u16 �퓬ID�w�� */
#define	TRDATA_EV_OBJ		(1+2)			/* u16 */
#define	TRDATA_EV_BEFORE	(1+4)			/* u32 �ΐ�O�t�B�[���h�ł�MSG */
#define	TRDATA_EV_LOSE		(1+8)			/* u32 �ΐ��̐퓬��ʂł�MSG */
#define TRDATA_EV_SCRIPT	(1+12)
#define TRDATA_EV_END		(1+16)

/* 2vs2�ΐ�g���[�i�[ */
#define TRDATA_2_TYPE		(0)				/* u8  �g���[�i�[�̎�� */
#define	TRDATA_2_ID			(1+0)			/* u16 �퓬ID�w�� */
#define	TRDATA_2_OBJ		(1+2)			/* u16 */
#define	TRDATA_2_BEFORE		(1+4)			/* u32 �ΐ�O�t�B�[���h�ł�MSG */
#define	TRDATA_2_LOSE		(1+8)			/* u32 �ΐ��̐퓬��ʂł�MSG */
#define TRDATA_2_NOFIGHT	(1+12)			/* u32 �ΐ킵�Ȃ��ꍇ��MSG */
#define TRDATA_2_END		(1+16)

/* �����g���[�i�[��f�[�^ */
#define TRDATA_F_TYPE	(0)				/* u8  �g���[�i�[�̎�� */
#define	TRDATA_F_ID		(1+0)			/* u16 �퓬ID�w�� */
#define	TRDATA_F_OBJ	(1+2)			/* u16 */
#define	TRDATA_F_MSG	(1+4)
#define	TRDATA_F_END	(1+8)


static const TR_DATA_TYPE normal_trainer_type[] =
{
	{ &trainer_type,		GET_1_BYTES },
	{ &FieldTrainerID,		GET_2_BYTES },
	{ &trainer_obj,			GET_2_BYTES },
	{ &before_msg,			GET_4_BYTES },
	{ &lose_msg,			GET_4_BYTES },
	{ &win_msg,				DMY_4_BYTES },
	{ &no_fight_msg,		DMY_4_BYTES },
	{ &battle_end_script,	DMY_4_BYTES },
	{ &after_script,		GET_END_ADRS },
};

static const TR_DATA_TYPE event_trainer_type[] =
{
	{ &trainer_type,		GET_1_BYTES },
	{ &FieldTrainerID,		GET_2_BYTES },
	{ &trainer_obj,			GET_2_BYTES },
	{ &before_msg,			GET_4_BYTES },
	{ &lose_msg,			GET_4_BYTES },
	{ &win_msg,				DMY_4_BYTES },
	{ &no_fight_msg,		DMY_4_BYTES },
	{ &battle_end_script,	GET_4_BYTES },
	{ &after_script,		GET_END_ADRS },
};

static const TR_DATA_TYPE normal2vs2_trainer_type[] =
{
	{ &trainer_type,		GET_1_BYTES },
	{ &FieldTrainerID,		GET_2_BYTES },
	{ &trainer_obj,			GET_2_BYTES },
	{ &before_msg,			GET_4_BYTES },
	{ &lose_msg,			GET_4_BYTES },
	{ &win_msg,				DMY_4_BYTES },
	{ &no_fight_msg,		GET_4_BYTES },
	{ &battle_end_script,	DMY_4_BYTES },
	{ &after_script,		GET_END_ADRS },
};

static const TR_DATA_TYPE force_trainer_type[] =
{
	{ &trainer_type,		GET_1_BYTES },
	{ &FieldTrainerID,		GET_2_BYTES },
	{ &trainer_obj,			GET_2_BYTES },
	{ &before_msg,			DMY_4_BYTES },
	{ &lose_msg,			GET_4_BYTES },
	{ &win_msg,				DMY_4_BYTES },
	{ &no_fight_msg,		DMY_4_BYTES },
	{ &battle_end_script,	DMY_4_BYTES },
	{ &after_script,		GET_END_ADRS },
};

static const TR_DATA_TYPE event2vs2_trainer_type[] =
{
	{ &trainer_type,		GET_1_BYTES },
	{ &FieldTrainerID,		GET_2_BYTES },
	{ &trainer_obj,			GET_2_BYTES },
	{ &before_msg,			GET_4_BYTES },
	{ &lose_msg,			GET_4_BYTES },
	{ &win_msg,				DMY_4_BYTES },
	{ &no_fight_msg,		GET_4_BYTES },
	{ &battle_end_script,	GET_4_BYTES },
	{ &after_script,		GET_END_ADRS },
};
//=========================================================================
//	�֐�
//=========================================================================

static u32 get_u32_value( const VM_CODE * adrs )
{
	return (u32)*adrs | (u32)*(adrs+1)<<8 |
		(u32)*(adrs+2)<<16 | (u32)*(adrs+3)<<24;
}

static u16 get_u16_value( const VM_CODE * adrs )
{
	return (u16)*adrs | (u16)*(adrs+1)<<8;
}

static u8 get_u8_value( const VM_CODE * adrs )
{
	return (u8)*adrs;
}


static u16 get_now_trn_flag( void )
{
	return GET_TRN_FLAG( FieldTrainerID );
}


//=========================================================================
//
//=========================================================================
//--------------------------------------------------------------
//	�Q�[���I�[�o�[�ɂȂ邩�ǂ����̔���
//	<����>		�퓬���ʁiWinLoseFlag)
//	<�߂�l>	TRUE	�Q�[���I�[�o�[
//				FALSE	�v���C�p��
//	���퓬���ʂ̔��ʂ��s������WinLoseFlag�Ŕ��ʂł���悤�A
//	�@������WinLoseFlag���Ƃ�悤�ɂȂ��Ă���B
//--------------------------------------------------------------
int IsGameOverCheck( int value )
{
	switch ( value ) {
	case FIGHT_WIN:				return FALSE;
	case FIGHT_LOSE:			return TRUE;
	case FIGHT_DRAW:			return TRUE;
	case FIGHT_ESCAPE:			return FALSE;
	case FIGHT_END:				return FALSE;
	case FIGHT_ENEMY_ESCAPE:	return FALSE;
	case FIGHT_ENEMY_GET:		return FALSE;

	default:					return FALSE;
	}
}

//=========================================================================
//
//
//
//=========================================================================
static void init_trainer_data( void )
{
	trainer_type = 0;
	FieldTrainerID = 0;
	trainer_obj = 0;
	before_msg = NULL;
	lose_msg = NULL;
	win_msg = NULL;
	no_fight_msg = NULL;
	after_script = NULL;
	battle_end_script = NULL;
}

static void set_trainer_data( const TR_DATA_TYPE * type, const VM_CODE * adrs )
{
	for( ;;type++ ) {
		switch ( type->val_type ) {
		case GET_1_BYTES:
			*(u8*)type->val_adrs = get_u8_value(adrs);
			adrs ++;
			break;
		case GET_2_BYTES:
			*(u16*)type->val_adrs = get_u16_value(adrs);
			adrs += 2;
			break;
		case GET_4_BYTES:
			*(u32*)type->val_adrs = get_u32_value(adrs);
			adrs += 4;
			break;
		case DMY_1_BYTES:
			*(u8*)type->val_adrs = 0;
			break;
		case DMY_2_BYTES:
			*(u16*)type->val_adrs = 0;
			break;
		case DMY_4_BYTES:
			*(u32*)type->val_adrs = 0;
			break;
		case GET_END_ADRS:
			*(u32*)type->val_adrs = (u32)adrs;
			return;
		}
	}
}

static void set_trainer_obj_id( void )
{
	if (trainer_obj != 0 ) {
		TargetObjID = trainer_obj;
		TargetObjNo = EvObjIdSeek( TargetObjID, Fld.MapID.map_id, Fld.MapID.div_id );
	}
}

//--------------------------------------------------------------
//	�t�B�[���h�g���[�i�[�̃p�����[�^���Z�b�g
//	<����>
//	adr		�g���[�i�[�f�[�^�擪�A�h���X
//	<�߂�l>
//			�g���[�i�[�b�������X�N���v�g�A�h���X
//--------------------------------------------------------------
u32 SetTrainerDataByAdrs( const VM_CODE * adr )
{ 
	init_trainer_data();

	trainer_type	= get_u8_value(adr+TRDATA_TYPE);

	switch ( trainer_type ) {

	case TR_TYPE_FORCE:
		set_trainer_data( force_trainer_type, adr);
		return (u32)TrainerEventScript;

	case TR_TYPE_2VS2:
		set_trainer_data( normal2vs2_trainer_type, adr);
		set_trainer_obj_id();
		return (u32)Trainer2vs2Script;
	
	case TR_TYPE_EVENT:
	case TR_TYPE_LEADER:
		set_trainer_data( event_trainer_type, adr);
		set_trainer_obj_id();
		return (u32)TrainerTalkScript;

	case TR_TYPE_EVENT2VS2:
	case TR_TYPE_LEADER2VS2:
		set_trainer_data( event2vs2_trainer_type, adr);
		set_trainer_obj_id();
		return (u32)Trainer2vs2Script;

	case TR_TYPE_REVENGE2VS2:
		set_trainer_data( normal2vs2_trainer_type, adr);
		set_trainer_obj_id();
		FieldTrainerID = GetRevengerTrainerID( FieldTrainerID );
		return (u32)Trainer2vs2RevengeScript;

	case TR_TYPE_REVENGE:
		set_trainer_data( normal_trainer_type, adr);
		set_trainer_obj_id();
		FieldTrainerID = GetRevengerTrainerID( FieldTrainerID );
		return (u32)TrainerRevengeScript;

	default:
		set_trainer_data( normal_trainer_type, adr);
		set_trainer_obj_id();
		return (u32)TrainerTalkScript;
	}
}

//--------------------------------------------------------------
//
//	�����q�b�g����̏����ݒ�
//
//--------------------------------------------------------------
void SetTrainerMoveScript( u8 evno, const VM_CODE * label )
{
	TargetObjNo = evno;
	TargetObjID = EvObjId( evno );
	SetTrainerDataByAdrs( label + TRDATA_CODE_SIZE );
	StartFieldScript( TrainerMoveScript );
	SetForceEvent();
}

//--------------------------------------------------------------
//	�g���[�i�[�ΐ�t���O�̃`�F�b�N
//--------------------------------------------------------------
int CheckTrainerFlagByAdrs( const VM_CODE * label )
{
	u16 id;
	id = get_u16_value( label+TRDATA_CODE_SIZE+TRDATA_ID );
	return EventFlagCheck( GET_TRN_FLAG( id ) );
}

//--------------------------------------------------------------
//	�g���[�i�[�̓���R�[�h��Î~��Ԃɂ���
//	���X�N���v�g�����΂��
//--------------------------------------------------------------
void StopTrainerObjMove(void)
{
	EventOBJ * ev;

	ev = &EvObj[TargetObjNo];
	EvObjMvCodeChg( ev, EvObjMvCodeStopSite(ev->site) );
}


//=========================================================================
//
//	�X�N���v�g����Ă΂��֐�
//
//=========================================================================
//--------------------------------------------------------------
//	�g���[�i�[�̎�ރR�[�h��Ԃ�
//--------------------------------------------------------------
u8 GetTrainerType( void )
{
	return trainer_type;
}


//--------------------------------------------------------------
//	�g���[�i�[��t���O�֘A
//	�����ݑΏۂƂȂ��Ă���g���[�i�[�ɂ��ČĂяo��
//--------------------------------------------------------------
u8 CheckNowTrainerFlag( void )
{
	return EventFlagCheck( get_now_trn_flag() );
}

void SetNowTrainerFlag( void )
{
	EventFlagSet( get_now_trn_flag() );
}

void ResetNowTrainerFlag( void )
{
	EventFlagSet( get_now_trn_flag() );
}


//--------------------------------------------------------------
//	�g���[�i�[��t���O�֘A
//	���g���[�i�[ID���L�[�ɂ��ČĂяo��
//--------------------------------------------------------------
u8 CheckTrainerFlagByID( u16 id )
{
	return EventFlagCheck( GET_TRN_FLAG(id) );
}

void SetTrainerFlagByID( u16 id )
{
	EventFlagSet( GET_TRN_FLAG(id) );
}

void ResetTrainerFlagByID( u16 id )
{
	EventFlagReset( GET_TRN_FLAG(id) );
}


//--------------------------------------------------------------
//	�t�B�[���h�g���[�i�[��J�n
//	���X�N���v�g�R�}���h
//--------------------------------------------------------------
void FieldTrainerFightStart( void )
{
	FightType = FIGHT_TYPE_TRAINER;
	sys.pReturnProc = FieldTrainerFightEnd;
	StartTrainerEncountEvent();
	StopFieldScript();
}



//--------------------------------------------------------------
//	�t�B�[���h�g���[�i�[��I��
//	���X�N���v�g�R�}���h
//--------------------------------------------------------------
void FieldTrainerFightEnd( void )
{

	if( FieldTrainerID == ORUSUBAN_TRAINER ) {
		MainProcChange( FieldScriptRecover );

	} else if ( IsGameOverCheck(WinLoseFlag) == TRUE ) {
		MainProcChange( GameOverRestart );

	} else {
		MainProcChange( FieldScriptRecover );
		SetNowTrainerFlag();
	}
}

static void FieldRevengeTrainerFightEnd( void )
{

	if( FieldTrainerID == ORUSUBAN_TRAINER ) {
		MainProcChange( FieldScriptRecover );

	} else if ( IsGameOverCheck(WinLoseFlag) == TRUE ) {
		MainProcChange( GameOverRestart );

	} else {
		MainProcChange( FieldScriptRecover );
		SetNowTrainerFlag();
		ResetNowRevengerFlag();
	}
}

void FieldRevengeTrainerFightStart( void )
{
	FightType = FIGHT_TYPE_TRAINER;
	sys.pReturnProc = FieldRevengeTrainerFightEnd;
	StartTrainerEncountEvent();
	StopFieldScript();
}


//--------------------------------------------------------------
//	�g���[�i�[�ΐ�O�̂l�r�f������
//	���X�N���v�g����Ă΂��v���O����
//--------------------------------------------------------------
void PutTrainerBeforeFightMsg( void )
{
	FieldMsgSet(GetTrainerBeforeFightMsg());
}

//--------------------------------------------------------------
//	�g���[�i�[��ς݂̏ꍇ�̃X�N���v�g��Ԃ�
//	���X�N���v�g�R�}���h
//--------------------------------------------------------------
const VM_CODE * GetTrainerAfterScript( void )
{
	if( after_script == 0 )return test_bg_script;
	return after_script;
}

//--------------------------------------------------------------
//	�g���[�i�[��I����̃X�N���v�g��Ԃ�
//	���X�N���v�g�R�}���h
//--------------------------------------------------------------
const VM_CODE * GetTrainerBattleEndScript( void )
{
	if( battle_end_script == 0 )return test_bg_script;
	return battle_end_script;
}


//--------------------------------------------------------------
//	���Ȃ��ꍇ��MSG��\������
//	���X�N���v�g����Ă΂��v���O����
//--------------------------------------------------------------
void PutTrainerNotFightMsg( void )
{
	FieldMsgSet(GetTrainerNotFightMsg());
}

//--------------------------------------------------------------
//	�g���[�i�[�G���J�E���gBGM�Z�b�g
//	���X�N���v�g����Ă΂��v���O����
//--------------------------------------------------------------
void SetTrainerBGM( void )
{
	u16 bgm;

	//���[�_�[��̓G���J�E���g�a�f�l�ɐ؂�ւ��Ȃ�
	if (trainer_type == TR_TYPE_LEADER || trainer_type == TR_TYPE_LEADER2VS2)
		return;

	switch ( GetTrainerSex(FieldTrainerID) ) {

	case EYE_BOY:			bgm = MUS_BOYEYE; break;
	case EYE_GIRL:			bgm = MUS_GIRLEYE; break;
	case EYE_LITTLE_GIRL:	bgm = MUS_SYOUJOEYE; break;
	case EYE_AYASII:		bgm = MUS_AYASII; break;
	case EYE_HAGESII:		bgm = MUS_HAGESHII; break;
	case EYE_KAKKOII:		bgm = MUS_KAKKOII; break;
	case EYE_AQUADAN:		bgm = MUS_AQA_0; break;
	case EYE_MAGUMADAN:		bgm = MUS_MGM0;	break;
	case EYE_SWIMMER:		bgm = MUS_SWIMEYE; break;
	case EYE_FUTAGO:		bgm = MUS_HUTAGO; break;
	case EYE_SITENNOU:		bgm = MUS_SITENNOU; break;
	case EYE_DEBU:			bgm = MUS_YAMA_EYE; break;
	case EYE_INTERVIEWER:	bgm = MUS_INTER_V; break;
	case EYE_GENTLEMAN:		bgm = MUS_TEST; break;

	default:
		bgm = MUS_AYASII; break;	/*�Ƃ肠����,�t�H���[����*/
	}

	BGM_PlaySet( bgm );
}

//=========================================================================
//
//	���b�Z�[�W�f�[�^�A�N�Z�X
//
//=========================================================================
//--------------------------------------------------------------
//	MSG�|�C���^��NULL�̏ꍇ�̓_�~�[MSG�ւ̃|�C���^��Ԃ�
//--------------------------------------------------------------
static const u8 * null_msg_check(const u8 * msg)
{
	if (msg == NULL)
		return str_dummy;
	else
		return msg;
}

//--------------------------------------------------------------
//
//	�g���[�i�[�ΐ�O�̃��b�Z�[�W�f�[�^��Ԃ�
//
//--------------------------------------------------------------
const u8 * GetTrainerBeforeFightMsg( void )
{
	return null_msg_check(before_msg);
}

//--------------------------------------------------------------
//
//	�g���[�i�[���������Ƃ��̃��b�Z�[�W�f�[�^��Ԃ�
//
//--------------------------------------------------------------
extern const u8 * GetOrusubanLoseMsg(void);	// 2002/07/24 by nakahiro

const u8 * GetTrainerLoseMessage( void )
{
	const u8 * msg;

	if ( FieldTrainerID == ORUSUBAN_TRAINER ) {
		msg = GetOrusubanLoseMsg();
	} else {
		msg = lose_msg;
	}
	PM_MsgExpand( MsgExpandBuffer, null_msg_check(msg) );
	return MsgExpandBuffer;
}

//--------------------------------------------------------------
//
//	�g���[�i�[���������Ƃ��̃��b�Z�[�W�f�[�^��Ԃ�
//
//--------------------------------------------------------------
const u8 * GetTrainerWinMessage( void )
{
	return null_msg_check(win_msg);
}

//--------------------------------------------------------------
//
//	���Ȃ��ꍇ�̃��b�Z�[�W�f�[�^��Ԃ�
//	��2vs2�ΐ�p
//
//--------------------------------------------------------------
const u8 * GetTrainerNotFightMsg( void )
{
	return null_msg_check(no_fight_msg);
}






//=========================================================================
//	
//
//			�Đ�g���[�i�[�t���O�֘A
//
//
//=========================================================================


#define	NO_REVENGE_DATA			REVENGE_TRAINER_END		/* �Đ�f�[�^���Ȃ��ꍇ�̎w�� */

#include "../data/revenge.h"
#include "../data/revenge.dat"




enum{
	NOT_EXIST_OFS	= -1,		//�Đ�f�[�^�����݂��Ȃ��ꍇ
};

//=========================================================================
//
//
//
//=========================================================================

//-------------------------------------------------------------------------
/* �g���[�i�[ID����Đ�f�[�^�ւ̃I�t�Z�b�g���擾 */
//-------------------------------------------------------------------------
static int get_rvg_offset_by_trainer_id( const REVENGE_DATA * const rvg, u16 id )
{
	int i;

	for ( i=0; i<REVENGE_DATA_MAX; i++ ) {
		if ( rvg[i].id[0] == id ) return i;
	}
	return NOT_EXIST_OFS;
}

//-------------------------------------------------------------------------
/* �g���[�i�[ID(�Đ��pID�܂�)����Đ�f�[�^�ւ̃I�t�Z�b�g���擾 */
//-------------------------------------------------------------------------
static int get_rvg_offset_by_revenger_id( const REVENGE_DATA * const rvg, u16 id )
{
	int i,j;

	for ( i=0; i<REVENGE_DATA_MAX; i++ ) {
		for ( j=0; j<REVENGE_LEVEL_MAX; j++ ) {
			if ( rvg[i].id[j] == NO_REVENGE_DATA ) break;
			if ( rvg[i].id[j] == id ) return i;
		}
	}
	return NOT_EXIST_OFS;
}

//-------------------------------------------------------------------------
/* �w��}�b�v�ł̍Đ�g���[�i�[�̃����_���Z�b�g */
//-------------------------------------------------------------------------
static int set_random_revenger( const REVENGE_DATA * const rvg, u16 div_id, u16 map_id )
{
	int i,level;
	int check_flag;

	check_flag = FALSE;

	for ( i=0; i<REVENGE_DATA_MAX; i++ ) {
		/* �ꏊ�`�F�b�N */
		if ( rvg[i].div_id != div_id || rvg[i].map_id != map_id ) continue;

		/* ���łɃZ�b�g���Ă��邩�ǂ����`�F�b�N */
		if ( Fld.RevengeFlag[i] != 0 ) {
			check_flag = TRUE;
			continue;
		}

		/* ��������Ƃ����邩�ǂ����`�F�b�N */
		if ( CheckTrainerFlagByID( rvg[i].id[0] ) != TRUE ) continue;

		/* �R�O���̊m���`�F�b�N */
		if ( pp_rand() % 100 > 30 ) continue;

		for ( level = 1; level < REVENGE_LEVEL_MAX; level++ ) {
			if ( rvg[i].id[level] == NO_REVENGE_DATA ) break;
			if ( CheckTrainerFlagByID(rvg[i].id[level]) == FALSE ) break;
		}
		Fld.RevengeFlag[i] = level;
		check_flag = TRUE;
	}
	return check_flag;
}


//-------------------------------------------------------------------------
/*	�w��}�b�v�ɍĐ�\��Ԃ̃g���[�i�[�����邩�ǂ����̃`�F�b�N */
//-------------------------------------------------------------------------
static int check_active_revenger_in_map( const REVENGE_DATA * const rvg, u16 div_id, u16 map_id )
{
	int i;

	for ( i=0; i<REVENGE_DATA_MAX; i++ ) {
		if ( rvg[i].div_id != div_id || rvg[i].map_id != map_id ) continue;
		if ( Fld.RevengeFlag[i] != 0 ) return TRUE;
	}
	return FALSE;
}

//-------------------------------------------------------------------------
/* �w��}�b�v�ɍĐ�g���[�i�[�����݂��邩�ǂ����̃`�F�b�N */
//-------------------------------------------------------------------------
static int check_revenger_exist_in_map( const REVENGE_DATA * const rvg, u16 div_id, u16 map_id )
{
	int i;

	for ( i=0; i<REVENGE_DATA_MAX; i++ ) {
		if ( rvg[i].div_id == div_id && rvg[i].map_id == map_id ) return TRUE;
	}
	return FALSE;
}

//-------------------------------------------------------------------------
/* �w��ID�̃g���[�i�[���Đ�\��Ԃ��ǂ����`�F�b�N */
//-------------------------------------------------------------------------
static u8 check_revenger_by_trainer_id( const REVENGE_DATA * const rvg, u16 id )
{
	int ofs;
	ofs = get_rvg_offset_by_trainer_id(rvg,id);
	if ( ofs == NOT_EXIST_OFS ) return FALSE;
	if ( ofs >= REVENGER_MAX ) return FALSE;

	if ( Fld.RevengeFlag[ofs] == 0 ) return FALSE;
	return TRUE;
}

//-------------------------------------------------------------------------
/* �w��ID�i�Đ��pID�܂ށj�̃g���[�i�[���Đ�\��Ԃ��ǂ����̃`�F�b�N */
//-------------------------------------------------------------------------
static u8 check_revenger_by_revenger_id( const REVENGE_DATA * const rvg, u16 id )
{
	int ofs;
	ofs = get_rvg_offset_by_revenger_id(rvg,id);
	if ( ofs == NOT_EXIST_OFS ) return FALSE;
	if ( ofs >= REVENGER_MAX ) return FALSE;

	if ( Fld.RevengeFlag[ofs] == 0 ) return FALSE;
	return TRUE;
}

//-------------------------------------------------------------------------
/* �w��Đ�g���[�i�[�̍Đ�p�g���[�i�[ID���擾���� */
//-------------------------------------------------------------------------
static u16 get_revenger_id_by_trainer_id( const REVENGE_DATA * const rvg, u16 id )
{
	const REVENGE_DATA * p;
	int level;
	int ofs;

	ofs = get_rvg_offset_by_trainer_id(rvg, id);
	if ( ofs == NOT_EXIST_OFS ) return 0;
	p = rvg + ofs;
	for (level=1; level<REVENGE_LEVEL_MAX; level++) {
		if (p->id[level] == NO_REVENGE_DATA) {
			//�Đ�ID���Ȃ������O��ID��Ԃ�
			return p->id[level-1];
		}
		if (CheckTrainerFlagByID(p->id[level]) == FALSE) {
			//����Ă��Ȃ�ID������ID��Ԃ�
			return p->id[level];
		}
	}
	return p->id[REVENGE_LEVEL_MAX-1];	//�ő僌�x��
}

//-------------------------------------------------------------------------
/* �Đ�t���O�̃N���A */
//-------------------------------------------------------------------------
static void reset_revenger_flag_by_revenger_id( const REVENGE_DATA * const rvg, u16 id )
{
	int ofs;
	ofs = get_rvg_offset_by_revenger_id(rvg,id);
	if ( ofs == NOT_EXIST_OFS ) return;
	Fld.RevengeFlag[ofs] = 0;
}


//-------------------------------------------------------------------------
/* �Đ���������Ƃ�����g���[�i�[���ǂ����̃`�F�b�N */
//-------------------------------------------------------------------------
static int check_revenged_by_trainer_id( const REVENGE_DATA * const rvg, u16 id )
{
	int ofs;

	ofs = get_rvg_offset_by_trainer_id(rvg,id);
	if ( ofs == NOT_EXIST_OFS )
		return FALSE;

	//�e�[�u���̐擪����Q�Ԗڂ�ID == �Đ��p�̃g���[�i�[ID�Ȃ̂ŁA
	//�g���[�i�[�t���O���`�F�b�N����FALSE�̏ꍇ�͂܂��Đ���������Ƃ��Ȃ�
	if ( CheckTrainerFlagByID(rvg[ofs].id[1]) == FALSE ) {
		return FALSE;
	} else {
		return TRUE;
	}
}


//=========================================================================
//
//
//
//=========================================================================
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

static const u16 badge_flags[] =
{
	SYS_BATCH01_GET,
	SYS_BATCH02_GET,
	SYS_BATCH03_GET,
	SYS_BATCH04_GET,
	SYS_BATCH05_GET,
	SYS_BATCH06_GET,
	SYS_BATCH07_GET,
	SYS_BATCH08_GET,
};

static int check_badge_count( void )
{
	int i,count;
	count = 0;
	for ( i=0; i<NELEMS(badge_flags); i++ ) {
		if ( EventFlagCheck(badge_flags[i]) == TRUE ) {
			count ++;
			if ( count >= 5 ) return TRUE;
		}
	}
	return FALSE;
}

//-------------------------------------------------------------------------
//	�Đ�`�F�b�N�p�����J�E���g
//-------------------------------------------------------------------------
void RevengeStepCountUp( void )
{
	if ( check_badge_count() == FALSE ) return;

	if ( Fld.RevengeStepCount >= REVENGE_STEP_MAX ) {
		Fld.RevengeStepCount = REVENGE_STEP_MAX;
	} else {
		Fld.RevengeStepCount++;
	}
}

static int check_revenge_step_count( void )
{
	if ( check_badge_count() == FALSE ) return FALSE;
	if ( Fld.RevengeStepCount < REVENGE_STEP_MAX ) return FALSE;
	return TRUE;
}

//-------------------------------------------------------------------------
//	�����_���ōĐ�g���[�i�[���Đ��ԂɃZ�b�g����
//	<����>
//		div_id,map_id	�}�b�v�̎w��
//-------------------------------------------------------------------------
void SetRandomRevenger( u16 div_id, u16 map_id )
{
	if ( check_revenge_step_count() == FALSE ) return;
	if ( set_random_revenger( RevengeDataTbl, div_id, map_id ) == TRUE ) {
		Fld.RevengeStepCount = 0;
	}
	return;
}


//-------------------------------------------------------------------------
//	�Đ��Ԃ̃g���[�i�[�����邩�ǂ����̃`�F�b�N
//	<����>
//		div_id,map_id		�}�b�v�̎w��
//	<�߂�l>
//		TRUE		���݂���
//		FALSE		���݂��Ȃ�
//-------------------------------------------------------------------------
int CheckRevengerByMapID( u16 div_id, u16 map_id )
{
	return check_active_revenger_in_map( RevengeDataTbl, div_id, map_id );
}

int DebugCheckRevengeDataByMapID( u16 div_id, u16 map_id )
{
	return check_revenger_exist_in_map( RevengeDataTbl, div_id, map_id );
}

//-------------------------------------------------------------------------
//	�Đ�g���[�i�[�̑ΐ�g���[�i�[ID���擾����
//	<����>
//		�Đ�g���[�i�[�̃g���[�i�[ID
//	<�߂�l>
//		�Đ킷��ۂ̃g���[�i�[ID
//-------------------------------------------------------------------------
u16 GetRevengerTrainerID( u16 trainer_id )
{
	return get_revenger_id_by_trainer_id( RevengeDataTbl, trainer_id );
}

//-------------------------------------------------------------------------
//	�Đ�f�[�^�֕��򂷂邩�ǂ����̃`�F�b�N
//	���L�̂ǂ��炩�̏ꍇ�ɁATRUE��Ԃ�
//	�E�Đ�t���O���I��
//	�E�Đ�t���O���I�t�����Đ���������Ƃ�����
//-------------------------------------------------------------------------
u8 IsRevengerCheck( void )
{
	if ( check_revenger_by_trainer_id( RevengeDataTbl, FieldTrainerID ) ) {
		return TRUE;
	}
	return check_revenged_by_trainer_id( RevengeDataTbl, FieldTrainerID );
}

//-------------------------------------------------------------------------
//	�Đ�t���O�̃`�F�b�N
//-------------------------------------------------------------------------
u8 CheckRevengerFlag( void )
{
	return check_revenger_by_revenger_id( RevengeDataTbl, FieldTrainerID );
}

//-------------------------------------------------------------------------
//	�Đ�t���O�̃N���A
//-------------------------------------------------------------------------
void ResetNowRevengerFlag( void )
{
	reset_revenger_flag_by_revenger_id( RevengeDataTbl, FieldTrainerID );
	SetNowTrainerFlag();
}


