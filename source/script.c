//=========================================================================
//	
//
//	�X�N���v�g���䃁�C������
//
//	2001.11.07 by tama
//		�������ɋL�q����Ă����R�}���h�֐���scrcmd.c�Ɉړ�����
//=========================================================================


#include "common.h"

#include "vm.h"
#include "map.h"
#include "sxytype.h"
#include "ev_flag.h"
#include "syswork.h"

#define SCRIPT_H_GLOBAL
#include "script.h"


//==============================================================
//	�ϐ�
//==============================================================

//	�X�N���v�g��Ԓ�`
enum{
	SCRIPT_ON,
	SCRIPT_WAIT,
	SCRIPT_OFF,
};

//	�X�N���v�g��ԕϐ�
static u8 field_script_status;

//	���z�}�V���p����\����
static VM_MACHINE field_script;
static VM_MACHINE special_script;


//	�����C�x���g����t���O
static u8 ForceEventFlag;


extern const VM_CMD ScriptCommand[];	// �X�N���v�g�R�}���h�֐��e�[�u��
extern const VM_CMD ScriptCommandEnd[];	// �e�[�u���Ō�̃A�h���X

//==================================================================================
//
//	�C�x���g�N���`�F�b�N�֘A
//
//==================================================================================

//--------------------------------------------------------------
//	�����C�x���g�t���O�Z�b�g�i�ق��̃C�x���g�N�����t�b�N�����j
//--------------------------------------------------------------
void SetForceEvent( void )
{
	ForceEventFlag = TRUE;
}

//--------------------------------------------------------------
//	�����C�x���g�t���O����
//--------------------------------------------------------------
void ResetForceEvent( void )
{
	ForceEventFlag = FALSE;
}

//--------------------------------------------------------------
//	�����C�x���g�t���O�`�F�b�N
//	<�߂�l>
//	TRUE	�����C�x���g��
//	FALSE	�����C�x���g�łȂ�
//--------------------------------------------------------------
u8 CheckForceEvent( void )
{
	return ForceEventFlag;
}



//==================================================================================
//
//	�X�N���v�g����֘A
//
//==================================================================================
//--------------------------------------------------------------
//	�X�N���v�g���䏉����
//--------------------------------------------------------------
void InitFieldScript( void )
{
	VM_Init( &field_script, ScriptCommand, ScriptCommandEnd );
	field_script_status = SCRIPT_OFF;
}

//--------------------------------------------------------------
//	�X�N���v�g���䃁�C��
//	<�߂�l>
//	TRUE	�X�N���v�g���s��
//	FALSE	�X�N���v�g���s�I��
//--------------------------------------------------------------
u8 ControlFieldScript( void )
{
	if( field_script_status == SCRIPT_OFF )return FALSE;
	if( field_script_status == SCRIPT_WAIT )return FALSE;
	SetForceEvent();
	if( VM_Control( &field_script ) == FALSE )
	{
		field_script_status = SCRIPT_OFF;
		ResetForceEvent();
		return FALSE;
	};
	return TRUE;
}

//--------------------------------------------------------------
//	�X�N���v�g����X�^�[�g
//	<����>
//	script	�X�N���v�g�f�[�^�A�h���X
//--------------------------------------------------------------
void StartFieldScript(const VM_CODE * script)
{
		VM_Init( &field_script, ScriptCommand, ScriptCommandEnd );
		VM_Start( &field_script, script );
		SetForceEvent();
		field_script_status = SCRIPT_ON;
}

//--------------------------------------------------------------
//	�X�N���v�g�����~
//--------------------------------------------------------------
void StopFieldScript( void )
{
	field_script_status = SCRIPT_WAIT;
}

//--------------------------------------------------------------
//	�X�N���v�g����ĊJ
//	�iInitTask���Ă΂ꂽ�ꍇ�Ȃǂɕ���������)
//--------------------------------------------------------------
void ContinueFieldScript( void )
{
	field_script_status = SCRIPT_ON;
	SetForceEvent();
}

//--------------------------------------------------------------
//	����X�N���v�g���s
//	<����>
//	script	�X�N���v�g�f�[�^�A�h���X
//--------------------------------------------------------------
void DoSpecialScript( const VM_CODE * script )
{
	VM_Init( &special_script, ScriptCommand, ScriptCommandEnd );
	VM_Start( &special_script, script );
	while( VM_Control( &special_script ) == TRUE );
}

//=========================================================================
//
//	�X�N���v�g�w�b�_�����֘A
//
//=========================================================================

//-------------------------------------------------------------
//	�w�b�_�[�f�[�^����w��^�C�v�̂��̂�����
//-------------------------------------------------------------
static const EV_DATA * search_header_data( u8 key )
{
	const EV_DATA * p;
	u32 adr;

	p = NowMap.Event;
	if ( p == NULL ) return NULL;
	while ( TRUE ) {
		if ( *p == 0 ) return NULL;
		if ( *p == key ) {
			p++;
			adr = *p + ( *(p+1)<<8 ) + ( *(p+2)<<16 ) + ( *(p+3)<<24 );
			return (EV_DATA *) adr;
		}
		p += (1 + 4);
	}
}

//-------------------------------------------------------------
//	�w�b�_�[�f�[�^����w��^�C�v�̂��̂������A���s����
//-------------------------------------------------------------
static void do_search_sp_script( u8 key )
{
	const EV_DATA * p;

	p = search_header_data( key );
	if( p == NULL )return;

	DoSpecialScript( p );
}

//-------------------------------------------------------------
//	�w��^�C�v�̃e�[�u���������A�������`�F�b�N����
//	���s�X�N���v�g�̃A�h���X��Ԃ�
//-------------------------------------------------------------
static const VM_CODE * get_enable_script( u8 key )
{
	const EV_DATA * p;
	u32 ret;
	u16 flag1,flag2;

	p = search_header_data( key );
	if ( p == NULL ) return NULL;

	while ( TRUE ) {
		flag1 = *p + ( *(p+1)<<8 );
		if( flag1 == 0 )return NULL;
		p += 2;
		flag2 = *p + ( *(p+1)<<8 );
		p += 2;
		if(  GetEventWorkValue(flag1) ==  GetEventWorkValue(flag2) )
		{
			ret =  *p + ( *(p+1)<<8 ) + ( *(p+2)<<16 ) + ( *(p+3)<<24 ) ;
			return (const VM_CODE *) ret;
		}
		p += 4;
	}
}

//-------------------------------------------------------------
//
//	�}�b�v�`�掞�̓��ꏈ���֘A
//
//-------------------------------------------------------------
void BgControlScriptCheck( void )
{
	do_search_sp_script( HEAD_BG_CHANGE );
}

//-------------------------------------------------------------
//
//	�}�b�v�J�ڎ��̃t���O����
//
//-------------------------------------------------------------
void FlagControlScriptCheck( void )
{
	do_search_sp_script( HEAD_FLAG_CTRL );
}

//-------------------------------------------------------------
//
// �}�b�v���������Ƃ̓��ꏈ���֘A
//
//-------------------------------------------------------------
void InitControlScriptCheck( void )
{
	do_search_sp_script( HEAD_INIT_CTRL );
}

//-------------------------------------------------------------
//
//	�����N����̓��ꏈ��
//	������A�_�C�r���O�̓���ڑ����������邽�߂̂ݎg�p
//
//-------------------------------------------------------------
void LinkControlScriptCheck( void )
{
	do_search_sp_script( HEAD_LINK_CTRL );
}

//-------------------------------------------------------------
//
//	�V�[�������C�x���g�`�F�b�N
//
//-------------------------------------------------------------
u8 SceneEventCheck( void )
{
	const VM_CODE * scr;

	scr = get_enable_script( HEAD_SCENE_CHK );
	if( scr == NULL )return FALSE;

	StartFieldScript( scr );
	return TRUE;
}

//-------------------------------------------------------------
//
//	�}�b�v�J�ڎ���OBJ�֘A����
//
//-------------------------------------------------------------
void ObjControlScriptCheck( void )
{
	const VM_CODE * scr;

	scr = get_enable_script( HEAD_OBJ_CTRL );
	if( scr == NULL )return;

	DoSpecialScript( scr );
}

//=========================================================================
//
//		�O���X�N���v�g�֘A
//
//=========================================================================
	/* �Ƃ肠���������Ă���Ƃ�����g�� */
#define	DATA_STORE_ADRS	(Fld.ExtraScript.work)
enum {
	EX_SCR_SIGN	= 0x33,	//���ł��������Ƃ肠�������ʗp�R�[�h

	EX_SCR_FLGID = 0,
	EX_SCR_DIVID,
	EX_SCR_MAPID,
	EX_SCR_OBJID,
	EX_SCR_MAINDATA,	//�X�N���v�g�f�[�^�J�n�ʒu
};

const VM_CODE * ReserveScript = NULL;

//-------------------------------------------------------------
//	�O���X�N���v�g�`�F�b�N�T���Z�o
//-------------------------------------------------------------
static u32 calc_exscript_checksum( void )
{
	u32 i,sum;
	sum = 0;
	for  (i=0;i<sizeof(DATA_STORE_ADRS); i++ ) {
		sum += DATA_STORE_ADRS[i];
	}
	return sum;
}

//-------------------------------------------------------------
//	�O���X�N���v�g�̃N���A
//-------------------------------------------------------------
void ClearExScript( void )
{
	CpuClear( 0, (u8 *)&(Fld.ExtraScript), sizeof(Fld.ExtraScript), 32);
}

//-------------------------------------------------------------
//	�O���X�N���v�g�̃Z�b�g
//	<����>	
//			src				�O���X�N���v�g�̃A�h���X
//			size			�O���X�N���v�g�̃T�C�Y
//			div_id	map_id	�}�b�v�w��
//			obj_id			���̃}�b�v�ł̂n�a�i�h�c�w��
//-------------------------------------------------------------
int SetExScript( const u8 * src, u16 size, u8 div, u8 map, u8 obj )
{
	u8 * s = Fld.ExtraScript.work;

	ClearExScript();
	if ( size >= EX_SCR_SIZE - EX_SCR_MAINDATA ) return FALSE;

	s[EX_SCR_FLGID] = EX_SCR_SIGN;
	s[EX_SCR_DIVID] = div;
	s[EX_SCR_MAPID] = map;
	s[EX_SCR_OBJID] = obj;

	memcpy( &(s[EX_SCR_MAINDATA]), src, size );

	Fld.ExtraScript.checksum = calc_exscript_checksum();

	return TRUE;
}

//-------------------------------------------------------------
//	�n�a�i�Ɏw�肵���O���X�N���v�g�̃`�F�b�N
//	<����>	obj_id		�`�F�b�N����n�a�i�̂h�c
//			org_script	�ʏ�N������͂��̃X�N���v�g�A�h���X
//	<�߂�l>	�N������X�N���v�g�A�h���X
//-------------------------------------------------------------
const VM_CODE * ExScriptObjCheck( u8 obj_id, const VM_CODE * org_script )
{
	u8 * s = Fld.ExtraScript.work;

	ReserveScript = NULL;

	if ( s[EX_SCR_FLGID] != EX_SCR_SIGN ) return org_script;
	if ( s[EX_SCR_DIVID] != Fld.MapID.div_id ) return org_script;
	if ( s[EX_SCR_MAPID] != Fld.MapID.map_id ) return org_script;
	if ( s[EX_SCR_OBJID] != obj_id ) return org_script;
	if ( calc_exscript_checksum() != Fld.ExtraScript.checksum ) {
		ClearExScript();	//��ꂽ�X�N���v�g�f�[�^���폜
		return org_script;
	}

	ReserveScript = org_script;
	return &(s[EX_SCR_MAINDATA]);
}

