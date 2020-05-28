//====================================================================
//
//						�_�����A�֘A		02/07/23 nohara							
//
//====================================================================
#include "common.h"
#include "intr.h"
#include "poketool.h"				//PokeParaMine
#include "fieldmap.h"				//SetCell
#include "fld_tool.h"				//MAP_X_MARGIN
#include "../evdata/maparray.h"		//�ڑ��p�}�b�vID��`
#include "monsno.def"				//MONSNO_TAIKONGU
#include "mapbg.h"					//MakeMapBG
#include "script.h"					//EventFlagSet
#include "sysflag.h"				//SYS_TENJI_ANAWOHORU
#include "task.h"					//AddTask
#include "ev_check.h"				//EV_POS�Q�Ƃ̂���
#include "fe_extra.h"				//RETURN_ADR_H
#include "mus_tool.h"				//SePlay
#include "fld_talk.h"				//TalkEnd
#include "print.h"
#include "menutool.h"				//NMenuScreenClear
#include "config.h"
#include "f_pmlist.h"


//====================================================================
//
//						�v���g�^�C�v�錾
//
//====================================================================
u8 TenjiAnawohoruChk();
void TenjiAnawohoruSet();
u8 TenjiPokeRetuChk();
u8 TenjiKairikiChk();
void TenjiKairikiSet();
u8 TenjiSorawotobuChk();
void TenjiSorawotobuSet();
static void SetEvSoraEff(void);
void AddTenjiMoveTask();
void AddTenjiMove2Task();
void TenjiMoveTask(u8 id);


//====================================================================
//							��`
//====================================================================
//�u���Ȃ��ق�v
#define TENJI_ANAWOHORU_X	10			//X���W
#define TENJI_ANAWOHORU_X2	9			//X���W
#define TENJI_ANAWOHORU_X3	11			//X���W
#define TENJI_ANAWOHORU_Y	3			//Y���W
#define TENJI_ANAWOHORU_CELL_X1	(9+MAP_X_MARGIN)	//�Z����������X���W
#define TENJI_ANAWOHORU_CELL_Y1	(1+MAP_Y_MARGIN)	//�Z����������Y���W

//�u�����肫�v
#define TENJI_KAIRIKI_X		10			//X���W
#define TENJI_KAIRIKI_X2	9			//X���W
#define TENJI_KAIRIKI_X3	11			//X���W
#define TENJI_KAIRIKI_Y		23			//Y���W
#define TENJI_KAIRIKI_CELL_X1	(7+MAP_X_MARGIN)	//�Z����������X���W
#define TENJI_KAIRIKI_CELL_Y1	(19+MAP_Y_MARGIN)	//�Z����������Y���W

//�u������Ƃԁv
#define TENJI_SORAWOTOBU_X		8		//X���W
#define TENJI_SORAWOTOBU_Y		25		//Y���W
#define TENJI_SORAWOTOBU_CELL_X1	(7+MAP_X_MARGIN) //�Z����������X���W
#define TENJI_SORAWOTOBU_CELL_Y1	(19+MAP_Y_MARGIN)//�Z����������Y���W

//��ʗh�炵
//#define TENJI_SHAKE_X		(4)			//X��
#define TENJI_SHAKE_Y		(2)			//Y��
#define TENJI_SHAKE_WAIT	(5)			//�h�炷�Ԋu
#define TENJI_SHAKE_NUM		(50)		//�h�炷��

//�h�A���J���Ƃ��̗h��
#define TENJI_SHAKE_Y2		(3)			//Y��
#define TENJI_SHAKE_NUM2	(2)			//�h�炷��


//====================================================================
//
//					�u���Ȃ��ق�v�`�F�b�N
//
//	�߂�l TRUE  = ������
//		   FALSE = �������Ȃ�
//====================================================================
u8 TenjiAnawohoruChk()
{
	if ( EventFlagCheck(SYS_TENJI_ANAWOHORU) ) return FALSE;

	if( Fld.MapID.div_id == CAVE && Fld.MapID.map_id == CAVE_D2003 )
	{
		//�R�ӏ�����̂Ń`�F�b�N���R���Ă���
		if( Fld.MapXpos == TENJI_ANAWOHORU_X && Fld.MapYpos == TENJI_ANAWOHORU_Y )
			return TRUE;

		if( Fld.MapXpos == TENJI_ANAWOHORU_X2 && Fld.MapYpos == TENJI_ANAWOHORU_Y )
			return TRUE;

		if( Fld.MapXpos == TENJI_ANAWOHORU_X3 && Fld.MapYpos == TENJI_ANAWOHORU_Y )
			return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------
//					�u���Ȃ��ق�v�Z�b�g
//--------------------------------------------------------------------
void TenjiAnawohoruSet()
{
	//�J�b�g�C���̌�ɂ����ɂ���
		
	//�ǂ��󂷃G�t�F�N�g
	SetCell( TENJI_ANAWOHORU_CELL_X1,  TENJI_ANAWOHORU_CELL_Y1 , 554 );
	SetCell( TENJI_ANAWOHORU_CELL_X1+1,TENJI_ANAWOHORU_CELL_Y1 , 555 );
	SetCell( TENJI_ANAWOHORU_CELL_X1+2,TENJI_ANAWOHORU_CELL_Y1 , 556 );
	SetCell( TENJI_ANAWOHORU_CELL_X1,  TENJI_ANAWOHORU_CELL_Y1+1, 562 | CELL_HITCHECK_MASK  );
	SetCell( TENJI_ANAWOHORU_CELL_X1+1,TENJI_ANAWOHORU_CELL_Y1+1, 563 );
	SetCell( TENJI_ANAWOHORU_CELL_X1+2,TENJI_ANAWOHORU_CELL_Y1+1, 564 | CELL_HITCHECK_MASK  );
	MakeMapBG();
	
	SePlay( SE_BAN );
	EventFlagSet( SYS_TENJI_ANAWOHORU );
	ResetForceEvent();
	return;
}


//====================================================================
//
//					�|�P�����̕��т��`�F�b�N����
//
//	�߂�l TRUE  = ������
//		   FALSE = �������Ȃ�
//====================================================================
u8 TenjiPokeRetuChk()
{
	//�擪���W�[�����X��
	if( PokeParaGet( &PokeParaMine[0], ID_monsno_egg, 0 ) == MONSNO_ZIIRANSU )
	{
		//��Ԍ�낪�z�G���I�[��
		PokeCountMineGet();

		if( PokeParaGet( &PokeParaMine[(PokeCountMine-1)], ID_monsno_egg, 0 ) == MONSNO_HOERUOO )
			return TRUE;
	}

	return FALSE;
}


//====================================================================
//
//					�u�����肫�v�`�F�b�N
//
//	�߂�l TRUE  = ������
//		   FALSE = �������Ȃ�
//====================================================================
u8 TenjiKairikiChk()
{
	if ( EventFlagCheck(SYS_TENJI_KAIRIKI) ) return FALSE;

	if( Fld.MapID.div_id == CAVE && Fld.MapID.map_id == CAVE_D0401 )
	{
		//�R�ӏ�����̂Ń`�F�b�N���R���Ă���
		if( Fld.MapXpos == TENJI_KAIRIKI_X && Fld.MapYpos == TENJI_KAIRIKI_Y )
			return TRUE;

		if( Fld.MapXpos == TENJI_KAIRIKI_X2 && Fld.MapYpos == TENJI_KAIRIKI_Y )
			return TRUE;

		if( Fld.MapXpos == TENJI_KAIRIKI_X3 && Fld.MapYpos == TENJI_KAIRIKI_Y )
			return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------
//					�u�����肫�v�Z�b�g
//--------------------------------------------------------------------
void TenjiKairikiSet()
{
	//�J�b�g�C���̌�ɂ����ɂ���
	FeCodeDel( FECODE_KairikiSet );
	
	//�ǂ��󂷃G�t�F�N�g
	SetCell( TENJI_KAIRIKI_CELL_X1,  TENJI_KAIRIKI_CELL_Y1 , 554 );
	SetCell( TENJI_KAIRIKI_CELL_X1+1,TENJI_KAIRIKI_CELL_Y1 , 555 );
	SetCell( TENJI_KAIRIKI_CELL_X1+2,TENJI_KAIRIKI_CELL_Y1 , 556 );
	SetCell( TENJI_KAIRIKI_CELL_X1,  TENJI_KAIRIKI_CELL_Y1+1, 562 | CELL_HITCHECK_MASK  );
	SetCell( TENJI_KAIRIKI_CELL_X1+1,TENJI_KAIRIKI_CELL_Y1+1, 563 );
	SetCell( TENJI_KAIRIKI_CELL_X1+2,TENJI_KAIRIKI_CELL_Y1+1, 564 | CELL_HITCHECK_MASK  );
	MakeMapBG();
	
	SePlay( SE_BAN );
	EventFlagSet( SYS_TENJI_KAIRIKI );
	ResetForceEvent();
	return;
}


//====================================================================
//
//					�u������Ƃԁv�`�F�b�N
//
//	�߂�l TRUE  = ������
//		   FALSE = �������Ȃ�
//====================================================================
u8 TenjiSorawotobuChk()
{
	if ( EventFlagCheck(SYS_TENJI_SORAWOTOBU) ) return FALSE;

	if( Fld.MapID.div_id == CAVE && Fld.MapID.map_id == CAVE_D1901 )
	{
		if( Fld.MapXpos == TENJI_SORAWOTOBU_X && Fld.MapYpos == TENJI_SORAWOTOBU_Y )
			return TRUE;
	}

	return FALSE;
}

//-----------------------------------
//	�G�t�F�N�g�Z�b�g
//-----------------------------------
void SetFeEvSora(void)
{
	FeSetWork[0] = FPokeNo;
	FldEffSet( FECODE_EvSora );
}

u32 FeEvSora(void)
{
	u8	id;

	id = EffectTaskSet();

	TaskTable[id].work[ RETURN_ADR_H ] = GET_ADR_H(	SetEvSoraEff );
	TaskTable[id].work[ RETURN_ADR_L ] = GET_ADR_L(	SetEvSoraEff );

	return	0;
}

//-----------------------------------
//	�G�t�F�N�g����
//-----------------------------------
static void SetEvSoraEff(void)
{
	FeCodeDel( FECODE_EvSora );
	TenjiSorawotobuSet();
}

//--------------------------------------------------------------------
//					�u������Ƃԁv�Z�b�g
//--------------------------------------------------------------------
void TenjiSorawotobuSet()
{
	//�J�b�g�C���̌�ɂ����ɂ���
		
	//�ǂ��󂷃G�t�F�N�g
	SetCell( TENJI_SORAWOTOBU_CELL_X1,  TENJI_SORAWOTOBU_CELL_Y1 , 554 );
	SetCell( TENJI_SORAWOTOBU_CELL_X1+1,TENJI_SORAWOTOBU_CELL_Y1 , 555 );
	SetCell( TENJI_SORAWOTOBU_CELL_X1+2,TENJI_SORAWOTOBU_CELL_Y1 , 556 );
	SetCell( TENJI_SORAWOTOBU_CELL_X1,  TENJI_SORAWOTOBU_CELL_Y1+1, 562 | CELL_HITCHECK_MASK  );
	SetCell( TENJI_SORAWOTOBU_CELL_X1+1,TENJI_SORAWOTOBU_CELL_Y1+1, 563 );
	SetCell( TENJI_SORAWOTOBU_CELL_X1+2,TENJI_SORAWOTOBU_CELL_Y1+1, 564 | CELL_HITCHECK_MASK  );
	MakeMapBG();
	
	SePlay( SE_BAN );
	EventFlagSet( SYS_TENJI_SORAWOTOBU );
	ResetForceEvent();
	return;
}


//====================================================================
//
//					2���ԃL�[���͂Ȃ��𔻒肷��
//
//====================================================================
extern const VM_CODE bg_chg_02_cave_d1801[];
static void tenji_wait_task( u8 no );
static int tenji_key_check(void);

enum{
	TENJI_WAIT_VALUE = 2 * 60 * 60,		//2����
	CLOSE_WAIT_VALUE = 30,
};

//--------------------------------------------------------------------
//	�^�X�N�ɂ���ăL�[��Ԃ��Ď�
//--------------------------------------------------------------------
void TenjiWaitEventStart( void )
{
	if ( EventFlagCheck(SYS_TENJI_WAIT) ) return;
	AddTask( tenji_wait_task, 80 );
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------

enum{
	SEQ_INIT,
	SEQ_OPEN_WAIT,
	SEQ_CLOSE_WAIT,
	SEQ_START_WAIT,
	SEQ_START_EVENT,
	SEQ_END,
};

static void tenji_wait_task(u8 no)
{
	s16 * twk;

	twk = TaskTable[no].work;
	
	switch (twk[0]) {
	case SEQ_INIT:
		twk[1] = TENJI_WAIT_VALUE;
		twk[0] = SEQ_OPEN_WAIT;
		break;

	case SEQ_OPEN_WAIT:
		if (tenji_key_check()) {
			NMenuScreenClear();
			SePlay(SE_SELECT);
			twk[0] = SEQ_CLOSE_WAIT;
			break;
		}

		if (--twk[1] == 0) {
			NMenuScreenClear();
			twk[0] = SEQ_START_WAIT;
			twk[1] = CLOSE_WAIT_VALUE;
		}
		break;

	case SEQ_CLOSE_WAIT:
		if (tenji_key_check()) {
			TalkEnd();
			DelTask(no);
			ResetForceEvent();
			return;
		}
		if (--twk[1] == 0) {
			twk[0] = SEQ_START_EVENT;
		}
		break;

	case SEQ_START_WAIT:
		if (--twk[1] == 0)
			twk[0] = SEQ_START_EVENT;
		break;

	case SEQ_START_EVENT:
		TalkEnd();
		StartFieldScript(bg_chg_02_cave_d1801);
		DelTask(no);
		return;
	}
}

static int tenji_key_check(void)
{
	u16 mask;

	mask = PLUS_KEY_MASK | A_BUTTON | B_BUTTON | START_BUTTON | SELECT_BUTTON;

	if (MyData.key_config == CNT_MD_LR)
		mask |= L_BUTTON | R_BUTTON;

	if (MyData.key_config == CNT_MD_KATATE)
		mask |= L_BUTTON;

	if (sys.Trg & mask)
		return TRUE;
	else
		return FALSE;
}

//====================================================================
//							��ʗh�炵
//====================================================================

//--------------------------------------------------------------------
//							�Ăяo��
//--------------------------------------------------------------------
void AddTenjiMoveTask()
{
	u8 no;
	no = AddTask( TenjiMoveTask, 9 );
	TaskTable[no].work[1] = 0;					//�E�F�C�g�J�E���^
	TaskTable[no].work[2] = 0;					//
	TaskTable[no].work[4] = TENJI_SHAKE_Y;		//��ꕝ(Y)
	TaskTable[no].work[5] = TENJI_SHAKE_WAIT;	//�E�F�C�g����
	TaskTable[no].work[6] = TENJI_SHAKE_NUM;	//��
	SetMapViewFunc( NULL );
}

//--------------------------------------------------------------------
//							�Ăяo���Q
//
//				�_�����A�h�A���J���Ƃ��̗h�ꏈ��
//--------------------------------------------------------------------
void AddTenjiMove2Task()
{
	u8 no;
	no = AddTask( TenjiMoveTask, 9 );
	TaskTable[no].work[1] = 0;					//�E�F�C�g�J�E���^
	TaskTable[no].work[2] = 0;					//
	TaskTable[no].work[4] = TENJI_SHAKE_Y2;		//��ꕝ(Y)
	TaskTable[no].work[5] = TENJI_SHAKE_WAIT;	//�E�F�C�g����
	TaskTable[no].work[6] = TENJI_SHAKE_NUM2;	//��
	SetMapViewFunc( NULL );
}

//--------------------------------------------------------------------
//							�h�炵����
//--------------------------------------------------------------------
void TenjiMoveTask(u8 id)
{
	TASK_TABLE * task_p;

	task_p = &TaskTable[id];
	
	task_p->work[1]++;

	if( (task_p->work[1] % task_p->work[5]) == 0 )
	{
		task_p->work[1] = 0;
		task_p->work[2]++;

		task_p->work[4] = -task_p->work[4];
		SetMapViewOffset( 0, task_p->work[4] );
		if( task_p->work[2] == task_p->work[6] )
		{
			DelTask(id);
			ContinueFieldScript();
			MapViewInit();
		}
	}

	return;
}


