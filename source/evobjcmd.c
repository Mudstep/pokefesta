//**********************************************************************
//		evobjcmd.c
//		̨���� OBJ �����
//**********************************************************************
#define		EVOBJCMD_H_PROTO

#include	<agb.h>
#include 	"common.h"
#include	"calctool.h"
#include	"map.h"
#include	"evobj.h"
#include	"jiki.h"
#include	"task.h"

//----------------------------------------------------------------------
//		define
//----------------------------------------------------------------------
#define	ACMD_OBJ_MAX	16
#define	NoBIT(no)	((u16)no2bittbl[no])

//----------------------------------------------------------------------
//		۰��
//----------------------------------------------------------------------
static ACMD_CODE *AnmCodeAdrs[ACMD_OBJ_MAX] = {};

//----------------------------------------------------------------------
//		����
//----------------------------------------------------------------------
static void AnmCmdTask( u8 );
static void AnmCmdTaskAdd( u8 );
static u8 CmdTaskNo( void );
static u8 CmdSet( u8, u8, ACMD_CODE * );
static u8 CmdWorkNoSeek( u8, u8 );
static void CmdWorkNoGet( u8, u8, u8 * );
static void CmdWorkNoSet( u8, u8, u8 );
static void CmdWorkBitClr( u8, u8 );
static void CmdWorkBitSet( u8, u8 );
static u8 CmdWorkBitChk( u8, u8 );
static void CodeAdrsSet( u8, ACMD_CODE * );
static ACMD_CODE * CodeAdrsGet( u8 );
static void CmdWorkSet( u8, u8, u8, ACMD_CODE * );
static void CmdObjPauseClr( u8 );

//======================================================================
//		̨���� OBJ ��Ұ��ݺ����
//======================================================================
//------------------------------------------
//	����޾��
//	In:		objID			; ��� OBJ ID
//			mapID			; MAP ID
//			divID			; DIV ID
//			code			; ����޺���
//	Out:	u8				; ON = ��ĕs��
//------------------------------------------
u8 AnmCmdSet( u8 objID, u8 mapID, u8 divID, ACMD_CODE *code )
{
	u8 evno;

	if( EvObjIdPosGet(objID,mapID,divID,&evno) )
	{
		return( 1 );
	}

	if( CheckTask(AnmCmdTask) == 0 )
	{
		AnmCmdTaskAdd( 50 );							// ��ײ��è��
	}

	return( CmdSet(CmdTaskNo(),evno,code) );
}

//------------------------------------------
//	����ޏI������
//	In:		objID			; ��� OBJ ID
//			mapID			; MAP ID
//			divID			; DIV ID
//	Out:	u8				; ON = �I��
//------------------------------------------
u8 AnmCmdEnd( u8 objID, u8 mapID, u8 divID )
{
	u8	taskno,workno,evno;
	
	if( EvObjIdPosGet(objID,mapID,divID,&evno) )
	{
		return( 1 );
	}

	taskno = CmdTaskNo();
	workno = CmdWorkNoSeek( taskno, evno );
	
	if( workno == ACMD_OBJ_MAX )
	{
		return( 1 );
	}
	
	return( CmdWorkBitChk(taskno,workno) );
}

//------------------------------------------
//	����ފJ��
//	Info:	��Ұ��� OBJ �߰�� ����
//------------------------------------------
void AnmCmdFree( void )
{
	u8 no;

	no = CmdTaskNo();

	if( no != NOT_EXIST_TASK_ID )
	{
		CmdObjPauseClr( no );
		DelTask( no );
	}
}

//======================================================================
//		���ܰ�
//======================================================================
//------------------------------------------
//	����ǉ�
//------------------------------------------
static void AnmCmdTaskAdd( u8 pri )
{
	u8 i,no;

	no = AddTask( AnmCmdTask, pri );
	
	for( i = 1; i < TASK_WORK_MAX; i++ )
	{
		TaskTable[no].work[i] = 0xffff;
	}
}

//------------------------------------------
//	�������
//------------------------------------------
static u8 CmdTaskNo( void )
{
	return( CheckTaskNo(AnmCmdTask) );
}

//------------------------------------------
//	����޾��
//------------------------------------------
static u8 CmdSet( u8 taskno, u8 evno, ACMD_CODE *code )
{
	u8	workno;

	workno = CmdWorkNoSeek( taskno, evno );

	if( workno != ACMD_OBJ_MAX )
	{
		if( CmdWorkBitChk(taskno,workno) )
		{
			CmdWorkSet( taskno, workno, evno, code );
			return( 0 );
		}
		else
		{
			return( 1 );							// ��Ғ�
		}
	}

	workno = CmdWorkNoSeek( taskno, 0xff );

	if( workno == ACMD_OBJ_MAX )
	{
		return( 1 );								// �󂫖���
	}

	CmdWorkSet( taskno, workno, evno, code );
	
	return( 0 );
}

//------------------------------------------
//	�����ܰ����
//------------------------------------------
static u8 CmdWorkNoSeek( u8 taskno, u8 cmp )
{
	u8 i;
	u8 *adrs;

	adrs = (u8 *)&TaskTable[taskno].work[1];
	
	for( i = 0; i < ACMD_OBJ_MAX; i++, adrs++ )
	{
		if( (*adrs) == cmp )
		{
			return( i );
		}
	}

	return( ACMD_OBJ_MAX );
}

//------------------------------------------
//	�����ܰ����ڽ���
//------------------------------------------
static void CmdWorkNoAdrs( u8 taskno, u8 workno, u8 **adrs )
{
	u8 i;
	
	*adrs = (u8 *)&TaskTable[taskno].work[1];
	for( i = 0; i < workno; i++, (*adrs)++ );
}

//------------------------------------------
//	�����ܰ����
//------------------------------------------
static void CmdWorkNoSet( u8 taskno, u8 workno, u8 evno )
{
	u8 *adrs;
	
	CmdWorkNoAdrs( taskno, workno, &adrs );
	*adrs = evno;
}

//------------------------------------------
//	�����ܰ��ޯ�
//------------------------------------------
static void CmdWorkNoGet( u8 taskno, u8 workno, u8 *evno )
{
	u8 *adrs;
	
	CmdWorkNoAdrs( taskno, workno, &adrs );
	*evno = (*adrs);
}

//------------------------------------------
//	�I���ޯĸر
//------------------------------------------
static void CmdWorkBitClr( u8 taskno, u8 workno )
{
	u16	mask;
	
	mask = ~(NoBIT(workno));
	(u16)TaskTable[taskno].work[0] &= mask;
}

//------------------------------------------
//	�I���ޯľ��
//------------------------------------------
static void CmdWorkBitSet( u8 taskno, u8 workno )
{
	(u16)TaskTable[taskno].work[0] |= NoBIT(workno);
}

//------------------------------------------
//	�I���ޯ�����
//------------------------------------------
static u8 CmdWorkBitChk( u8 taskno, u8 workno )
{
	if( (u16)TaskTable[taskno].work[0] & NoBIT(workno) )
	{
		return( 1 );									// 0 or 1 �ŕԂ�(�����Ă̓s��)
	}
	
	return( 0 );
}

//------------------------------------------
//	���ޱ��ڽ���
//------------------------------------------
static void CodeAdrsSet( u8 no, ACMD_CODE *code )
{
	AnmCodeAdrs[no] = (u8 *)code;
}

//------------------------------------------
//	���ޱ��ڽ�擾
//------------------------------------------
static ACMD_CODE * CodeAdrsGet( u8 no )
{
	return( AnmCodeAdrs[no] );
}

//------------------------------------------
//	�����ܰ����
//------------------------------------------
static void CmdWorkSet( u8 taskno, u8 workno, u8 evno, ACMD_CODE *code )
{
	CmdWorkBitClr( taskno, workno );
	CodeAdrsSet( workno, code );
	CmdWorkNoSet( taskno, workno, evno );
}

//------------------------------------------
//	����޾�� OBJ �߰�޸ر
//------------------------------------------
static void CmdObjPauseClr( u8 taskno )
{
	u8 i;
	u8 *adrs;
	
	adrs = (u8 *)&TaskTable[taskno].work[1];

	for( i = 0; i < ACMD_OBJ_MAX; i++, adrs++ )
	{
		if( (*adrs) != 0xff )
		{
			EvObjPauseClr( &EvObj[(*adrs)] );
		}
	}
}

//======================================================================
//		����ޏ���Ҳ�
//======================================================================
static void AnmCmdProcess( u8, u8, u8, ACMD_CODE * );

//------------------------------------------
//	Ҳ����
//------------------------------------------
static void AnmCmdTask( u8 taskno )
{
	u8 workno,evno;

	for( workno = 0; workno < ACMD_OBJ_MAX; workno++ )
	{
		CmdWorkNoGet( taskno, workno, &evno );
		
		if( evno != 0xff )
		{
			AnmCmdProcess( taskno, workno, evno, CodeAdrsGet(workno) );
		}
	}
}

//------------------------------------------
//	����ޏ���
//------------------------------------------
static void AnmCmdProcess( u8 taskno, u8 workno, u8 evno, ACMD_CODE *code )
{
	u8	ac;

	if( AcmdActionChk(&EvObj[evno]) && AcmdEnd(&EvObj[evno]) == 0 )
	{
		return;
	}

	ac = (u8)(*code);
	
	if( ac == ACMD_END )
	{
		CmdWorkBitSet( taskno, workno );
		EvObjPause( &EvObj[evno] );
		return;
	}
	
	if( AcmdSet(&EvObj[evno],ac) )
	{
		return;
	}

	code++;

	CodeAdrsSet( workno, code );
}

