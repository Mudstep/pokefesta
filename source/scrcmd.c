//=========================================================================
//
//	�X�N���v�g�R�}���h�p�֐�
//
//		2001.03.28	by tama
//		2001.11.07	by tama	�X�N���v�g�p�u�l�̃R�A������vm.c�Ɉړ�
//							�R�}���h������scrcmd.c �Ƃ��ēƗ�
//
//=========================================================================
#include "common.h"

#include "vm.h"


#include "vmstat.h"

#include "ev_flag.h"
#include "script.h"
#include "fld_task.h"
#include "fld_talk.h"
#include "fieldmap.h"				//SetCell
#include "sxytype.h"
#include "item.h"
#include "evobj.h"
#include "evobjmv.h"
#include "ev_win.h"
#include "ev_sub.h"
#include "poketool.h"
#include "calctool.h"
#include "gold.h"
#include "weather.h"
#include "wea_ctrl.h"
#include "palanm.h"
#include "tv_def.h"
#include "tv_tool.h"
#include "cm.h"
#include "conscr.h"
#include "fld_tool.h"
#include "fld_bgm.h"
#include "ev_fight.h"
#include "mus_tool.h"
#include "print.h"
#include "ev_check.h"
#include "jiki.h"
#include "menu.h"
#include "zukan.h"
#include "rtctool.h"
#include "ev_time.h"
#include "slot.h"
#include "seed.h"
#include "fld_main.h"
#include "coin.h"
#include "pokescr.h"
#include "goods.h"
#include "gds_main.h"
#include "ex_load.h"
#include "record.h"
#include "contest.h"
#include "scr_tool.h"
#include "fld_eff.h"


//=========================================================================
//
//
//
//=========================================================================

#define VMGetWork(core)			GetEventWorkAdrs( VMGetU16(core) )

#define VMGetWorkValue(core)	GetEventWorkValue( VMGetU16(core) )

//�@���W�X�^��r�̌��ʒ�`
enum{
		MINUS_RESULT,		// ��r���ʂ��}�C�i�X
		EQUAL_RESULT,		// ��r���ʂ��C�R�[��
		PLUS_RESULT,		// ��r���ʂ��v���X
};

//	��������p�e�[�u��
static const u8 ConditionTable[6][3] =
{
			/*	MINUS	EQUAL	PLUS */
/* LT */	{	TRUE,	FALSE,	FALSE,	},
/* EQ */	{	FALSE,	TRUE,	FALSE,	},
/* GT */	{	FALSE,	FALSE,	TRUE,	},
/* LE */	{	TRUE,	TRUE,	FALSE	},
/* GE */	{	FALSE,	TRUE,	TRUE,	},
/* NE */	{	TRUE,	FALSE,	TRUE,	},
};

// �o�^�v���O�����e�[�u���ւ̎Q��
typedef u16 (* pMultiFunc)();

extern const pMultiFunc ScriptCallTable[];
extern const pMultiFunc ScriptCallTableEnd[];


//�h�c�ɂ�鋤�ʃX�N���v�g�Ăяo���̂��߂̒�`
extern const u32 CommonScriptTableStart[];
extern const u32 CommonScriptTableEnd[];

//-------------------------------------------------------------------------
// �X�N���v�g�p�֐�
//-------------------------------------------------------------------------

u8 EvCmdNop(VM_MACHINE * core);						/* EVCMD_NOP			*/
u8 EvCmdEnd(VM_MACHINE * core);						/* EVCMD_END			*/
u8 EvCmdRet(VM_MACHINE * core);						/* EVCMD_RET			*/
u8 EvCmdGlobalCall(VM_MACHINE * core);				/* EVCMD_CALL			*/
u8 EvCmdGlobalJump(VM_MACHINE * core);				/* EVCMD_JUMP			*/
u8 EvCmdIfJump(VM_MACHINE * core);					/* EVCMD_IFJUMP			*/
u8 EvCmdIfCall(VM_MACHINE * core);					/* EVCMD_IFCALL			*/

u8 EvCmdLoadRegWData(VM_MACHINE * core);			/* EVCMD_LD_REG_WDATA */
u8 EvCmdLoadRegAdrs(VM_MACHINE * core);				/* EVCMD_LD_REG_ADR	*/
u8 EvCmdLoadAdrsValue(VM_MACHINE * core);			/* EVCMD_LD_ADR_VAL	*/
u8 EvCmdLoadRegValue(VM_MACHINE * core);			/* EVCMD_LD_REG_VAL		*/
u8 EvCmdLoadAdrsReg(VM_MACHINE * core);				/* EVCMD_LD_ADR_REG	*/
u8 EvCmdLoadAdrsAdrs(VM_MACHINE * core);			/* EVCMD_LD_ADR_ADR	*/
u8 EvCmdLoadRegReg(VM_MACHINE * core);				/* EVCMD_LD_REG_REG */
u8 EvCmdLoadWkValue(VM_MACHINE * core);				/* EVCMD_LD_WK_VAL */
u8 EvCmdLoadWkWk(VM_MACHINE * core);				/* EVCMD_LD_WK_WK */
u8 EvCmdLoadWkWkValue(VM_MACHINE * core);			/* EVCMD_LD_WK_WKVAL */

u8 EvCmdCmpRegReg(VM_MACHINE * core);				/* EVCMD_CMP_REG		*/
u8 EvCmdCmpRegValue(VM_MACHINE * core);				/* EVCMD_CP_REG_VAL */
u8 EvCmdCmpRegAdrs(VM_MACHINE * core);				/* EVCMD_CP_REG_ADR	*/
u8 EvCmdCmpAdrsReg(VM_MACHINE * core);				/* EVCMD_CP_ADR_REG	*/
u8 EvCmdCmpAdrsValue(VM_MACHINE * core);			/* EVCMD_CP_ADR_VAL	*/
u8 EvCmdCmpAdrsAdrs(VM_MACHINE * core);				/* EVCMD_CP_ADR_ADR	*/
u8 EvCmdCmpWkValue(VM_MACHINE * core);				/* EVCMD_CP_WK_VAL */
u8 EvCmdCmpWkWk(VM_MACHINE * core);					/* EVCMD_CP_WK_WK */

u8 EvCmdSubroutine(VM_MACHINE * core);				/* EVCMD_SUBROUTINE */
u8 EvCmdTalkMsg(VM_MACHINE * core);					/* EVCMD_TALKMSG */
u8 EvCmdTalkWait(VM_MACHINE * core);				/* EVCMD_TALKWAIT */
u8 EvCmdTalkWinClose(VM_MACHINE * core);			/* EVCMD_TALKCLOSE */
u8 EvCmdABKeyWait(VM_MACHINE * core);				/* EVCMD_ABKEY_WAIT */
u8 EvCmdYesNoWin(VM_MACHINE * core);				/* EVCMD_YESNOWIN */
u8 EvCmdNothing(VM_MACHINE * core);
u8 EvCmdSetWaitRoutine(VM_MACHINE * core);			/* EVCMD_SETWAITROUTINE	*/

u8 EvCmdProgram(VM_MACHINE * core);					/* EVCMD_PROGRAM */

u8 EvCmdFlagSet(VM_MACHINE * core);					/* EVCMD_FLAGSET */
u8 EvCmdFlagReset(VM_MACHINE * core);				/* EVCMD_FLAGRESET */
u8 EvCmdFlagCheck(VM_MACHINE * core);				/* EVCMD_FLAGCHECK */

u8 EvCmdAddItem(VM_MACHINE * core);					/* EVCMD_ADDITEM */
u8 EvCmdSubItem(VM_MACHINE * core);					/* EVCMD_SUBITEM */
u8 EvCmdCheckItem(VM_MACHINE * core);				/* EVCMD_CHECKITEM */
u8 EvCmdGetPocketNo(VM_MACHINE * core);				/* EVCMD_GETPOCKETNO */

u8 EvCmdAddGoods(VM_MACHINE * core);					/* EVCMD_ADDGOODS */
u8 EvCmdSubGoods(VM_MACHINE * core);					/* EVCMD_SUBGOODS */

u8 EvCmdObjAnime(VM_MACHINE * core);				/* EVCMD_OBJANIME */
u8 EvCmdObjAnimeWait(VM_MACHINE * core);			/* EVCMD_OBJANIMEWAIT */
u8 EvCmdObjDel(VM_MACHINE * core);					/* EVCMD_OBJDEL */
u8 EvCmdObjAppear(VM_MACHINE * core);				/* EVCMD_OBJAPPEAR */

u8 EvCmdSelectWin(VM_MACHINE * core);				/* EVCMD_SELECTWIN */
u8 EvCmdWinWrite(VM_MACHINE * core);				/* EVCMD_WINWRITE */
u8 EvCmdWinClear(VM_MACHINE * core);				/* EVCMD_WINCLEAR */
u8 EvCmdSelectMenu(VM_MACHINE * core);				/* EVCMD_MENUWIN */

u8 EvCmdCheckGoods(VM_MACHINE * core);				/* EVCMD_CHECKGOODS */

u8 EvCmdCoinWinWrite(VM_MACHINE * core);			/* EVCMD_COINWINWRITE */
u8 EvCmdCoinWinDel(VM_MACHINE * core);				/* EVCMD_COINWINDEL */
u8 EvCmdCoinWrite(VM_MACHINE * core);				/* EVCMD_COINWRITE */


#define	GET_REL_ADRS(adrs)	( (adrs) - script_adrs_diff )

u32 script_adrs_diff = 0;

//=========================================================================
//
//	��{�V�X�e������
//
//=========================================================================

//-------------------------------------------------------------------------
//	�m�n�o���߁i�Ȃɂ����Ȃ��j
//-------------------------------------------------------------------------
u8 EvCmdNop(VM_MACHINE * core)						/* EVCMD_NOP			*/
{
	/* Do Nothing !! */
	return 0;
}

//-------------------------------------------------------------------------
//	�������Ȃ��i�f�o�b�K�ň����|���邽�߂̖��߁j
//-------------------------------------------------------------------------
u8 EvCmdDummy( VM_MACHINE * core )
{
	return 0;
}

//-------------------------------------------------------------------------
//	�X�N���v�g�̏I��
//-------------------------------------------------------------------------
u8 EvCmdEnd(VM_MACHINE * core)						/* EVCMD_END			*/
{
	VM_End(core);
	return 0;
}

//-------------------------------------------------------------------------
//	�X�N���v�g�E�F�C�g�֐���ݒ�
//-------------------------------------------------------------------------
u8 EvCmdSetWaitRoutine(VM_MACHINE * core)			/* EVCMD_SETWAITROUTINE	*/
{
	VM_SetWait( core, (void *)VMGetU32(core) );
	return 1;
}

//-------------------------------------------------------------------------
//	�v���O�����̌Ăяo��
//	��ev_call.s�̃e�[�u���ɒ�`����Ă�����̂��Ăяo��
//-------------------------------------------------------------------------
u8 EvCmdProgram(VM_MACHINE * core)					/* EVCMD_PROGRAM */
{
	u16 id;
	id = VMGetU16(core);
	(ScriptCallTable[id])();
	return 0;
}

//-------------------------------------------------------------------------
//	�v���O�����̌Ăяo��(�߂�l����j
//	��ev_call.s�̃e�[�u���ɒ�`����Ă�����̂��Ăяo��
//	���w�胏�[�N�ɖ߂�l��������
//-------------------------------------------------------------------------
u8 EvCmdFunctionCall( VM_MACHINE * core )
{
	u16 * work;
	work = VMGetWork(core);
	*work = ( ScriptCallTable[ VMGetU16(core) ] )();

	return 0;
}

//-------------------------------------------------------------------------
//	�v���O�����̌Ăяo��
//-------------------------------------------------------------------------
u8 EvCmdSubroutine(VM_MACHINE * core)				/* EVCMD_SUBROUTINE */
{
	pFunc func = (pFunc)VMGetU32(core);
	(func)();
	return 0;
}

//-------------------------------------------------------------------------
//	�X�N���v�g���s�̒�~
//	����~��AContinueFieldScript���Ă΂���
//	�@��~����������X�N���v�g�͍ĊJ�����
//-------------------------------------------------------------------------
u8 EvCmdStopScript(VM_MACHINE * core)			/* EVCMD_STOPSCRIPT */
{
	StopFieldScript();
	return 1;
}


//=========================================================================
//
//	���򖽗�
//
//=========================================================================

//-------------------------------------------------------------------------
//	�X�N���v�g�W�����v
//-------------------------------------------------------------------------
u8 EvCmdGlobalJump(VM_MACHINE * core)						/* EVCMD_JUMP			*/
{
	VM_CODE * adrs;
	adrs = (VM_CODE *)VMGetU32(core);
	VMJump( core, adrs );
	return 0;
}

//-------------------------------------------------------------------------
//	�X�N���v�g���^�[��
//-------------------------------------------------------------------------
u8 EvCmdRet(VM_MACHINE * core)						/* EVCMD_RET			*/
{
	VMRet( core );
	return 0;
}

//-------------------------------------------------------------------------
//	�X�N���v�g�R�[��
//-------------------------------------------------------------------------
u8 EvCmdGlobalCall(VM_MACHINE * core)						/* EVCMD_CALL			*/
{
	VM_CODE * adrs ;

	adrs = (VM_CODE *)VMGetU32(core);
	VMCall( core, adrs );
	return 0;
}

//-------------------------------------------------------------------------
//	�X�N���v�g�����W�����v
//-------------------------------------------------------------------------
u8 EvCmdIfJump(VM_MACHINE * core)					/* EVCMD_IFJUMP			*/
{
	u8 r;
	VM_CODE * adrs;

	r = VMGetU8(core);
	adrs = (VM_CODE *)VMGetU32(core);
	if( ConditionTable[r][core->cmp_flag] == TRUE ) VMJump( core, adrs );
	return 0;
}

//-------------------------------------------------------------------------
//	�X�N���v�g�����R�[��
//-------------------------------------------------------------------------
u8 EvCmdIfCall(VM_MACHINE * core)					/* EVCMD_IFCALL			*/
{
	u8 r;
	VM_CODE * adrs ;

	r = VMGetU8(core);
	adrs = (VM_CODE *)VMGetU32(core);

	if( ConditionTable[r][core->cmp_flag] == TRUE ) VMCall( core, adrs );
	return 0;
}

//-------------------------------------------------------------------------
//	���΃W�����v�ʒu���擾
//-------------------------------------------------------------------------

u8 EvCmdSetRelativeAdrs( VM_MACHINE * core )
{
	u32 new_adrs,old_adrs;
	new_adrs = (u32)core->PC - 1;	//���݂̃X�N���v�g�J�n�ʒu
	old_adrs = VMGetU32(core);		//�r���h���̃X�N���v�g�J�n�ʒu
	script_adrs_diff = old_adrs - new_adrs;
	return 0;
}

//-------------------------------------------------------------------------
//	���΃W�����v
//-------------------------------------------------------------------------
u8 EvCmdJumpRelative( VM_MACHINE * core )
{
	VM_CODE * adrs;
	adrs = (VM_CODE *) GET_REL_ADRS(VMGetU32(core));
	VMJump( core, adrs );
	return 0;
}

//-------------------------------------------------------------------------
//	���΃R�[��
//-------------------------------------------------------------------------
u8 EvCmdCallRelative( VM_MACHINE * core )
{
	VM_CODE * adrs ;
	adrs = (VM_CODE *)GET_REL_ADRS(VMGetU32(core));
	VMCall( core, adrs );
	return 0;
}

//-------------------------------------------------------------------------
//	�����t���΃W�����v
//-------------------------------------------------------------------------
u8 EvCmdIfJumpRelative(VM_MACHINE * core)
{
	u8 r;
	VM_CODE * adrs;

	r = VMGetU8(core);
	adrs = (VM_CODE *)GET_REL_ADRS(VMGetU32(core));
	if ( ConditionTable[r][core->cmp_flag] == TRUE ) {
		VMJump( core, adrs );
	}
	return 0;
}

//-------------------------------------------------------------------------
//	�����t���΃R�[��
//-------------------------------------------------------------------------
u8 EvCmdIfCallRelative(VM_MACHINE * core)
{
	u8 r;
	VM_CODE * adrs ;

	r = VMGetU8(core);
	adrs = (VM_CODE *)GET_REL_ADRS(VMGetU32(core));
	if ( ConditionTable[r][core->cmp_flag] == TRUE ) {
		VMCall( core, adrs );
	}
	return 0;
}

//-------------------------------------------------------------------------
//	�h�c�ɂ�鋤�ʃX�N���v�g�W�����v
//-------------------------------------------------------------------------
u8 EvCmdJumpID( VM_MACHINE * core )
{
	u8 id = VMGetU8(core);
	if ( CommonScriptTableStart+id < CommonScriptTableEnd ) {
		VMJump( core, (VM_CODE *)(CommonScriptTableStart[id]) );
	}
	return 0;
}

//-------------------------------------------------------------------------
//	�h�c�ɂ�鋤�ʃX�N���v�g�R�[��
//-------------------------------------------------------------------------
u8 EvCmdCallID( VM_MACHINE * core )
{
	u8 id = VMGetU8(core);
	if ( CommonScriptTableStart+id < CommonScriptTableEnd ) {
		VMCall( core, (VM_CODE *)(CommonScriptTableStart[id]) );
	}
	return 0;
}

//-------------------------------------------------------------------------
//	�h�c�ɂ�鋤�ʃX�N���v�g�W�����v�i�����t���j
//-------------------------------------------------------------------------
u8 EvCmdIfJumpID( VM_MACHINE * core )
{
	u8 r;
	u8 id;
	r = VMGetU8(core);
	id = VMGetU8(core);
	if ( ConditionTable[r][core->cmp_flag] == TRUE ) {
		if ( CommonScriptTableStart+id < CommonScriptTableEnd ) {
			VMJump( core, (VM_CODE *)(CommonScriptTableStart[id]) );
		}
	}
	return 0;
}
//-------------------------------------------------------------------------
//	�h�c�ɂ�鋤�ʃX�N���v�g�R�[���i�����t���j
//-------------------------------------------------------------------------
u8 EvCmdIfCallID( VM_MACHINE * core )
{
	u8 r;
	u8 id;
	r = VMGetU8(core);
	id = VMGetU8(core);
	if ( ConditionTable[r][core->cmp_flag] == TRUE ) {
		if ( CommonScriptTableStart+id < CommonScriptTableEnd ) {
			VMCall( core, (VM_CODE *)(CommonScriptTableStart[id]) );
		}
	}
	return 0;
}

//-------------------------------------------------------------------------
//	�O���w�肵���A�h���X�ɃX�N���v�g�𕪊�
//-------------------------------------------------------------------------
u8 EvCmdChangeScript( VM_MACHINE * core )
{
	VMJump( core, (VM_CODE *)ReserveScript );
	return 0;
}

//-------------------------------------------------------------------------
//	�O���X�N���v�g�̃N���A���X�N���v�g�I��
//-------------------------------------------------------------------------
u8 EvCmdClearExScript( VM_MACHINE * core )
{
	ClearExScript();
	VM_End(core);
	return 1;
}

//-------------------------------------------------------------------------
//	�O���X�N���v�g�G���[�ʒm
//-------------------------------------------------------------------------
u8 EvCmdSetExDataError( VM_MACHINE * core )
{
	SetExtraDataError( VMGetU8(core) );
	return 0;
}


//=========================================================================
//
//	�f�[�^���[�h�E�X�g�A�֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadRegWData(VM_MACHINE * core)				/* EVCMD_LD_REG_WDATA */
{
		u32 wdata;
		u8 r;
		r = VMGetU8(core);
		wdata= VMGetU32(core);
		core->reg[r] = wdata;
		return 0;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadRegAdrs(VM_MACHINE * core)				/* EVCMD_LD_REG_ADR	*/
{
		VM_CODE * adrs;
		u8 r;
		r = VMGetU8(core);
		adrs = (VM_CODE *)VMGetU32(core);
		core->reg[r] = *adrs;
		return 0;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadAdrsValue(VM_MACHINE * core)				/* EVCMD_LD_ADR_VAL	*/
{
		u8 r;
		VM_CODE * adrs;
		r = VMGetU8(core);
		adrs = (VM_CODE *)VMGetU32(core);
		*adrs = r;
		return 0;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadRegValue(VM_MACHINE * core)					/* EVCMD_LD_REG_VAL		*/
{
		u8 r;
		r = VMGetU8(core);
		core->reg[r] = VMGetU8(core);
		return 0;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadAdrsReg(VM_MACHINE * core)				/* EVCMD_LD_ADR_REG	*/
{
		u8 r;
		VM_CODE * adrs;
		r = VMGetU8(core);
		adrs = (VM_CODE *)VMGetU32(core);
		*adrs = core->reg[r];
		return 0;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadRegReg(VM_MACHINE * core)				/* EVCMD_LD_REG_REG */
{
		u8 r1,r2;
		r1 = VMGetU8(core);
		r2 = VMGetU8(core);
		core->reg[r1] = core->reg[r2];
		return 0;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadAdrsAdrs(VM_MACHINE * core)				/* EVCMD_LD_ADR_ADR	*/
{
		VM_CODE * adr1;
		VM_CODE * adr2;
		adr1 = (VM_CODE *)VMGetU32(core);
		adr2 = (VM_CODE *)VMGetU32(core);
		*adr1 = * adr2;
		return 0;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadWkValue(VM_MACHINE * core)				// EVCMD_LD_WK_VAL
{
	u16 * work;
	work = VMGetWork(core);
	*work = VMGetU16(core);
	return 0;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdLoadWkWk(VM_MACHINE * core)					// EVCMD_LD_WK_WK 
{
	u16 * wk1;
	u16 * wk2;
	wk1 = VMGetWork(core);
	wk2 = VMGetWork(core);
	*wk1 = *wk2;
	return 0;
}

//-------------------------------------------------------------------------
//LDVAL�̑������l��VMGetWorkValue�Ŏ��悤�ɕύX��������	01/10/19 nohara
//-------------------------------------------------------------------------
u8 EvCmdLoadWkWkValue(VM_MACHINE * core)			// EVCMD_LD_WK_WKVAL
{
	u16 * work;
	work = VMGetWork(core);
	*work = VMGetWorkValue(core);
	return 0;
}


//=========================================================================
//
//	��r����
//
//=========================================================================

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpMain(u16 r1, u16 r2)
{
		if( r1 < r2 )
		{
				return MINUS_RESULT;
		}
		else if( r1 == r2)
		{
				return EQUAL_RESULT;
		}
		else
		{
				return PLUS_RESULT;
		};
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpRegReg(VM_MACHINE * core)				/* EVCMD_CMP_REG		*/
{
		u8 r1,r2;
		r1 = core->reg[VMGetU8(core)];
		r2 = core->reg[VMGetU8(core)];
		core->cmp_flag = EvCmdCmpMain(r1,r2);
		return 0;
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpRegValue(VM_MACHINE * core)				/* EVCMD_CP_REG_VAL */
{
		u8 r1,r2;
		r1 = core->reg[VMGetU8(core)];
		r2 = VMGetU8(core);
		core->cmp_flag = EvCmdCmpMain(r1,r2);
		return 0;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpRegAdrs(VM_MACHINE * core)				/* EVCMD_CP_REG_ADR	*/
{
		u8 r1,r2;
		r1 = core->reg[VMGetU8(core)];
		r2 = *(VM_CODE *)VMGetU32(core);
		core->cmp_flag = EvCmdCmpMain(r1,r2);
		return 0;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpAdrsReg(VM_MACHINE * core)				/* EVCMD_CP_ADR_REG	*/
{
		u8 r1,r2;
		r1 = *(VM_CODE *)VMGetU32(core);
		r2 = core->reg[ VMGetU8(core) ];
		core->cmp_flag = EvCmdCmpMain(r1,r2);
		return 0;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpAdrsValue(VM_MACHINE * core)				/* EVCMD_CP_ADR_VAL	*/
{
		u8 r1,r2;
		r1 = *(VM_CODE *)VMGetU32(core);
		r2 = VMGetU8(core);
		core->cmp_flag = EvCmdCmpMain(r1,r2);
		return 0;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpAdrsAdrs(VM_MACHINE * core)				/* EVCMD_CP_ADR_ADR	*/
{
		u8 r1,r2;
		r1 = *(VM_CODE *)VMGetU32(core);
		r2 = *(VM_CODE *)VMGetU32(core);
		core->cmp_flag = EvCmdCmpMain(r1,r2);
		return 0;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpWkValue(VM_MACHINE * core)				/* EVCMD_LD_WK_VAL */
{
	u16 r1,r2;
	u16 * work;
	work = VMGetWork(core);
	r1 = *work;
	r2 = VMGetU16(core);
	core->cmp_flag = EvCmdCmpMain(r1,r2);
	return 0;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdCmpWkWk(VM_MACHINE * core)				/* EVCMD_LD_WK_WK */
{
	u16 * wk1;
	u16 * wk2;
	wk1 = VMGetWork(core);
	wk2 = VMGetWork(core);
	core->cmp_flag = EvCmdCmpMain(*wk1,*wk2);
	return 0;
}


//=========================================================================
//
//	���Z�E���l�֘A����
//
//=========================================================================

//-------------------------------------------------------------------------
//	���[�N�ɑ���	01/10/15 nohara
//-------------------------------------------------------------------------
u8 EvCmdWkAdd(VM_MACHINE * core)				/* EVCMD_ADDWK */
{
	u16 * work;
	work = VMGetWork(core);
	*work += VMGetU16(core);
	return 0;
}

//-------------------------------------------------------------------------
//	���[�N�������	02/07/17 nohara
//-------------------------------------------------------------------------
u8 EvCmdWkSub(VM_MACHINE * core)				/* EVCMD_SUBWK */
{
	u16 * work;
	work = VMGetWork(core);
	*work -= VMGetWorkValue(core);
	return 0;
}

//-------------------------------------------------------------------------
//	�����̎擾	add 01/10/03 nohara
//-------------------------------------------------------------------------
u8 EvCmdGetRand(VM_MACHINE * core)		/* EVCMD_GETRND */
{
	u16 limit;

	//0x4000�ȏ�̒l���K�v�ȂƂ���VMGetU16�ɕύX����
	limit = VMGetWorkValue(core);

	AnswerWork = (pp_rand() % limit);
	return 0;
}


//=========================================================================
//
//	�A�C�e���֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//	�A�C�e����������
//-------------------------------------------------------------------------
u8 EvCmdAddItem(VM_MACHINE * core)					/* EVCMD_ADDITEM */
{
	u16 item,amount;

	item = VMGetWorkValue(core);
	amount = VMGetWorkValue(core);

	AnswerWork = (u16)AddItem(item,(u8)amount);
	return 0;
};

//-------------------------------------------------------------------------
//	�A�C�e�������炷
//-------------------------------------------------------------------------
u8 EvCmdSubItem(VM_MACHINE * core)					/* EVCMD_SUBITEM */
{
	u16 item,amount;

	item = VMGetWorkValue(core);
	amount = VMGetWorkValue(core);

	AnswerWork = (u16)SubItem(item,(u8)amount);
	return 0;
};

//-------------------------------------------------------------------------
//	�A�C�e�����������邩�̃`�F�b�N
//-------------------------------------------------------------------------
u8 EvCmdAddItemChk(VM_MACHINE * core)				/* EVCMD_ADDITEMCHK */
{
	u16 item,amount;

	item = VMGetWorkValue(core);
	amount = VMGetWorkValue(core);

	AnswerWork = (u16)AddItemCheck(item,(u8)amount);
	return 0;
};

//-------------------------------------------------------------------------
//	�o�b�O�̂ǂ����`�F�b�N
//-------------------------------------------------------------------------
u8 EvCmdCheckItem(VM_MACHINE * core)				/* EVCMD_CHECKITEM */
{
	u16 item,amount;
	item = VMGetWorkValue(core);
	amount = VMGetWorkValue(core);
	AnswerWork = (u16)ItemCheck(item,(u8)amount);
	return 0;
};

//-------------------------------------------------------------------------
//	�|�P�b�g�i���o�[�擾
//-------------------------------------------------------------------------
u8 EvCmdGetPocketNo(VM_MACHINE * core)				/* EVCMD_GETPOCKETNO */
{
	u16 item;
	item = VMGetWorkValue(core);
	AnswerWork = (u16)GetPocketNo(item);
	return 0;
};

//-------------------------------------------------------------------------
//	�p�\�R���ɂǂ���������
//-------------------------------------------------------------------------
u8 EvCmdAddPCBoxItem( VM_MACHINE * core )
{
	u16 item,num;
	item = VMGetWorkValue(core);
	num = VMGetWorkValue(core);
	AnswerWork = AddPCBoxItem(item,num);
	return 0;
}

//-------------------------------------------------------------------------
//	�p�\�R���̂ǂ����`�F�b�N
//-------------------------------------------------------------------------
u8 EvCmdCheckPCBoxItem( VM_MACHINE * core )
{
	u16 item,num;
	item = VMGetWorkValue(core);
	num = VMGetWorkValue(core);
	AnswerWork = PCBoxItemCheck(item,num);
	return 0;
}


//=========================================================================
//
//	�O�b�Y�֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//	�O�b�Y��������
//-------------------------------------------------------------------------
u8 EvCmdAddGoods(VM_MACHINE * core)					/* EVCMD_ADDITEM */
{
	u16 goods;

	goods = VMGetWorkValue(core);

	AnswerWork = (u16)AddGoods(goods);
	return 0;
};

//-------------------------------------------------------------------------
//	�O�b�Y�����炷
//-------------------------------------------------------------------------
u8 EvCmdSubGoods(VM_MACHINE * core)					/* EVCMD_SUBITEM */
{
	u16 goods;

	goods = VMGetWorkValue(core);

	AnswerWork = (u16)SubGoods(goods);
	return 0;
};

//-------------------------------------------------------------------------
//	�O�b�Y���������邩�̃`�F�b�N
//-------------------------------------------------------------------------
u8 EvCmdAddGoodsChk(VM_MACHINE * core)				/* EVCMD_ADDITEMCHK */
{
	u16 goods;

	goods = VMGetWorkValue(core);

	AnswerWork = (u16)AddGoodsCheck(goods);
	return 0;
};

//-------------------------------------------------------------------------
//	�o�b�O�̃O�b�Y�`�F�b�N
//-------------------------------------------------------------------------
u8 EvCmdCheckGoods(VM_MACHINE * core)					/* EVCMD_CHECKGOODS */
{
	u16 goods;

	goods = VMGetWorkValue(core);

	AnswerWork = (u16)CheckGoodsMine(goods);
	return 0;
};


//=========================================================================
//
//	�C�x���g�t���O�֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//	�t���O�̃Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdFlagSet(VM_MACHINE * core)					/* EVCMD_FLAGSET */
{
	u16 flag;
	flag = VMGetU16(core);
	EventFlagSet( flag );
	return 0;
};

//-------------------------------------------------------------------------
//	�t���O�̃��Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdFlagReset(VM_MACHINE * core)				/* EVCMD_FLAGRESET */
{
	u16 flag;
	flag = VMGetU16(core);
	EventFlagReset( flag );
	return 0;
};

//-------------------------------------------------------------------------
//	�t���O�̃`�F�b�N
//-------------------------------------------------------------------------
u8 EvCmdFlagCheck(VM_MACHINE * core)				/* EVCMD_FLAGCHECK */
{
	u16 flag;
	flag = VMGetU16(core);
	core->cmp_flag = EventFlagCheck( flag );
	return 0;
};

//-------------------------------------------------------------------------
//	�J�E���^�̃C���N�������g
//-------------------------------------------------------------------------
u8 EvCmdIncRecord( VM_MACHINE * core )
{
	IncRecord( VMGetU8(core) );
	return 0;
}


//=========================================================================
//
//	�t�B�[���h����֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
u8 EvCmdChangeBrightCircle( VM_MACHINE * core )
{
	u8 end;
	end = VMGetU8(core);
	StartFieldLasterChange( end );
	StopFieldScript();
	return 1;
}

//-------------------------------------------------------------------------
//	���邳�̕ύX
//-------------------------------------------------------------------------
u8 EvCmdChangeBrightValue( VM_MACHINE * core )
{
	SetBright( VMGetWorkValue(core) ); 
	return 0;
}

//-------------------------------------------------------------------------
//	�t�F�[�h�̐���
//-------------------------------------------------------------------------
/* �t�F�[�h�I���Ď� */
static u8 EvWaitFadeEnd(void)
{
	if( FadeData.fade_sw == 0 )return TRUE;
	return FALSE;
}

u8 EvCmdFieldFade( VM_MACHINE * core )
{
	u8 type;
	type = VMGetU8(core);
	FieldFadeControl( type , 0 );
	VM_SetWait( core, EvWaitFadeEnd );
	return 1;
}

//�t�F�[�h���x�w���
u8 EvCmdFieldFadeWait( VM_MACHINE * core )
{
	u8 type,wait;
	type = VMGetU8(core);
	wait = VMGetU8(core);
	FieldFadeControl( type , wait );
	VM_SetWait( core, EvWaitFadeEnd );
	return 1;
}

//-------------------------------------------------------------------------
//	�E�F�C�g����
//-------------------------------------------------------------------------
static u16 ScriptWaitTime = 0;

static u8 EvWaitTime(void)
{
	ScriptWaitTime--;
	if( ScriptWaitTime == 0 )return TRUE;
	return FALSE;
}

u8 EvCmdTimeWait( VM_MACHINE * core )			/* EVCMD_TIMEWAIT */
{
	ScriptWaitTime = VMGetU16(core);
	VM_SetWait( core, EvWaitTime );
	return 1;
}

//-------------------------------------------------------------------------
//	�Q�[�������Ԃ̃Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdSetGameTime( VM_MACHINE * core )
{
	u8 hour,min;
	hour = VMGetWorkValue(core);
	min = VMGetWorkValue(core);
	SetGameTime( hour, min );
	return 0;
}

//-------------------------------------------------------------------------
//	�Q�[�������Ԃ̍X�V
//	���Q�[�������ԃ`�F�b�N���t���O�X�V���Ăяo��
//-------------------------------------------------------------------------
u8 EvCmdUpdateGameTime( VM_MACHINE * core )
{
	UpdateTimeEvent();
	return 0;
}

//-------------------------------------------------------------------------
//	�Q�[�������Ԃ̎擾(RG0,RG1,RG2�ɑ������)
//-------------------------------------------------------------------------
u8 EvCmdGetGameTime( VM_MACHINE * core )
{
	GetGameTime();
	ScriptRegister0 = (u16)GameTime.hour;
	ScriptRegister1 = (u16)GameTime.min;
	ScriptRegister2 = (u16)GameTime.sec;
	return 0;
}

//-------------------------------------------------------------------------
//	�V��R�[�h�̃Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdSetWeather( VM_MACHINE * core )
{
	u16 wea;
	wea = VMGetWorkValue(core);
	SetFieldWeatherValue(wea);
	return 0;
}

//-------------------------------------------------------------------------
//	�V��R�[�h�̏�����
//-------------------------------------------------------------------------
u8 EvCmdInitWeather( VM_MACHINE * core )
{
	InitFieldWeatherValue();
	return 0;
}

//-------------------------------------------------------------------------
//	�V��R�[�h�̔��f
//-------------------------------------------------------------------------
u8 EvCmdUpdateWeather( VM_MACHINE * core )
{
	UpdateFieldWeatherSmooth();
	return 0;
}

//-------------------------------------------------------------------------
//	�}�b�v�ŗL�̃G�t�F�N�g�̐ݒ�
//-------------------------------------------------------------------------
u8 EvCmdSetMapEffect( VM_MACHINE * core )
{
	SetMapEffectControl( VMGetU8(core) );
	return 0;
}

//-------------------------------------------------------------------------
//	�}�b�v�X�N���[���̐؂�ւ�
//	��FLAG_CHANGE_LABEL�̃��x���ő��삷�邱��
//-------------------------------------------------------------------------
u8 EvCmdChangeMapScreen( VM_MACHINE * core )
{
	u16 id = VMGetWorkValue(core);
	ChangeNowMapScreen( id );
	return 0;
}


//=========================================================================
//
//	�}�b�v�J��
//
//=========================================================================

//-------------------------------------------------------------------------
//	�}�b�v�J��
//-------------------------------------------------------------------------
u8 EvCmdMapChange( VM_MACHINE * core )			/* EVCMD_MAP_CHANGE */
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	NextMapSetDirect( div, map, exit, (u8)x, (u8)y );
	AddMapChangeTask();
	InitHeroLastStatus();
	return 1;
}

//-------------------------------------------------------------------------
//	�}�b�v�J��(SE�Ȃ�)
//-------------------------------------------------------------------------
u8 EvCmdMapChangeSeNot( VM_MACHINE * core )			/* EVCMD_MAP_CHG_SENOT */
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	NextMapSetDirect( div, map, exit, (u8)x, (u8)y );
	AddMapChangeSeNotTask();
	InitHeroLastStatus();
	return 1;
}

//-------------------------------------------------------------------------
//	�h�A�ɓ����ă}�b�v�J��
//-------------------------------------------------------------------------
u8 EvCmdDoorMapChange( VM_MACHINE * core )			/* EVCMD_DOORMAP_CHANGE */
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	NextMapSetDirect( div, map, exit, (u8)x, (u8)y );
	AddDoorMapChangeTask();
	InitHeroLastStatus();
	return 1;
}

//-------------------------------------------------------------------------
//	�����ɂ��}�b�v�J��
//-------------------------------------------------------------------------
u8 EvCmdMapChangeFall( VM_MACHINE * core )
{
	u8  div,map;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	GetHeroCellPos( &x, &y );

	if ( div == 0xff && map == 0xff ) {
		NextMapSetByPitfall( x - MAP_X_MARGIN, y - MAP_Y_MARGIN );
	} else {
		NextMapSetDirect( div, map, NO_EXIT_ID,
				x - MAP_X_MARGIN, y - MAP_Y_MARGIN );
	}
	AddMapChangeFallTask();
	InitHeroLastStatus();

	return 1;
}

//-------------------------------------------------------------------------
//	���[�v�ɂ��}�b�v�J��
//-------------------------------------------------------------------------
u8 EvCmdMapChangeWarp( VM_MACHINE * core )
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	NextMapSetDirect( div, map, exit, (u8)x, (u8)y );
	AddMapChangeWarpTask();
	InitHeroLastStatus();
	return 1;
}

//-------------------------------------------------------------------------
//	�}�b�v�J�ڐ���Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdSetMapChangePos( VM_MACHINE * core )
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	NextMapSetDirect( div, map, exit, (u8)x, (u8)y );
	return 0;
}

//-------------------------------------------------------------------------
//	����ڑ��Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdSetSPMapID( VM_MACHINE * core )
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	SetSpecialMapDirect( 0, div, map, exit, (u8)x, (u8)y );
	return 0;
}

//-------------------------------------------------------------------------
//	�_�C�r���O�ڑ��Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdSetDivingMapID( VM_MACHINE * core )
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	SetDivingMapDirect( div, map, exit, (u8)x, (u8)y );
	return 0;
}

//-------------------------------------------------------------------------
//	���Ƃ����ڑ��Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdSetPitfallMapID( VM_MACHINE * core )
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	SetPitfallMapDirect( div, map, exit, (u8)x, (u8)y );
	return 0;
}

//-------------------------------------------------------------------------
//	�������J�ڐ�̃Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdSetEscapeMapID( VM_MACHINE * core )
{
	u8  div,map,exit;
	u16 x,y;

	div = VMGetU8(core);
	map = VMGetU8(core);
	exit = VMGetU8(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	EscapeMapSetDirect( div, map, exit, (u8)x, (u8)y );
	return 0;
}

//-------------------------------------------------------------------------
//	���݂̃}�b�v�ʒu�̎擾
//-------------------------------------------------------------------------
u8 EvCmdGetMapPosition( VM_MACHINE * core )
{
	u16 *x, *y;
	x = VMGetWork( core );
	y = VMGetWork( core );
	*x = Fld.MapXpos;
	*y = Fld.MapYpos;
	return 0;
}

//-------------------------------------------------------------------------
//	�莝���̃|�P�����̐����擾
//-------------------------------------------------------------------------
u8 EvCmdGetTemotiNum( VM_MACHINE * core )
{
	AnswerWork = (u8)PokeCountMineGet();
	return 0;
}


//=========================================================================
//
//	���y�E�r�d�֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//	�r�d��炷
//-------------------------------------------------------------------------
u8 EvCmdSetSE(VM_MACHINE * core )
{
	SePlay( VMGetU16(core) );
	return 0;
}

//-------------------------------------------------------------------------
//	�r�d�̏I���҂�
//-------------------------------------------------------------------------
static u8 EvWaitSE(void)
{
	if( SeEndCheck() == 0 ) return TRUE;
	return FALSE;
};

u8 EvCmdSEWait(VM_MACHINE * core )
{
	VM_SetWait( core, EvWaitSE );
	return 1;
}

//-------------------------------------------------------------------------
//	�W���O���̃Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdSetJingle(VM_MACHINE * core )
{
	JinglePlay( VMGetU16(core) );
	return 0;
}

//-------------------------------------------------------------------------
//	�W���O���̏I���҂�
//-------------------------------------------------------------------------
static u8 EvWaitJingle(void)
{
	return JingleWait();
};

u8 EvCmdJingleWait(VM_MACHINE * core )
{
	VM_SetWait( core, EvWaitJingle );
	return 1;
}

//-------------------------------------------------------------------------
//	�a�f�l��ύX����
//-------------------------------------------------------------------------
u8 EvCmdBGMSet( VM_MACHINE * core )
{
	u16 music;
	u8 flag;
	music = VMGetU16(core);
	flag = VMGetU8(core);
	if ( flag == TRUE ) {
		MapBGM_SetSpecial( music );
	}
	BGM_PlaySet( music );
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
u8 EvCmdBGMSpecialSet( VM_MACHINE * core )
{
	u16 music;
	music = VMGetU16(core);
	MapBGM_SetSpecial( music );
	return 0;
}

//-------------------------------------------------------------------------
//	�a�f�l
//-------------------------------------------------------------------------
//�a�f�l�t�F�[�h�����̂a�f�l�֕ύX
//�������Ȃ̏ꍇ�@�\���Ȃ�
u8 EvCmdBGMNowSet( VM_MACHINE * core )
{
	MapBGM_FadeNowSet();
	return 0;
}

//�a�f�l�t�F�[�h���w��a�f�l�ɕύX
//�����̃}�b�v�ƍ��̃}�b�v��BGM�������ꍇ�@�\���Ȃ�
u8 EvCmdBGMFadeChg( VM_MACHINE * core )
{
	u16 music;
	music = VMGetU16(core);

	MapBGM_FadeChange(music);
	return 0;
}

//-------------------------------------------------------------------------
//	�a�f�l���t�F�[�h�A�E�g�҂�
//-------------------------------------------------------------------------
u8 EvCmdBGMFadeOut( VM_MACHINE * core )
{
	u8 sec;
	sec = VMGetU8(core);
	if ( sec ) {
		MusicFadeOutPause( BGMFADE_1SEC * sec );
	}else
	{
		MusicFadeOutPause( BGMFADE_1SEC );
	}

	VM_SetWait( core, MusicFadeOutPauseWait );
	return 1;
}

//-------------------------------------------------------------------------
//	�a�f�l���t�F�[�h�C���i�t�F�[�h�A�E�g�������̂��ĊJ����j
//-------------------------------------------------------------------------
u8	EvCmdBGMFadeIn( VM_MACHINE * core )
{
	u8 sec;
	sec = VMGetU8(core);
	if ( sec != 0 ) {
		MusicFadeInContinue( BGMFADE_1SEC * sec );
	}else
	{
		MusicFadeInContinue( BGMFADE_1SEC );
	}

	return 0;
}


//=========================================================================
//
//	�n�a�i�֘A
//
//=========================================================================
static u16 ScriptAnimeObjNumber = 0;
static u16 ScriptAnimeDivID = 0;
static u16 ScriptAnimeMapID = 0;

//-------------------------------------------------------------------------
//	�n�a�i�A�j���̃Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdObjAnime(VM_MACHINE * core)				/* EVCMD_OBJANIME */
{
	u16 obj_id;
	ACMD_CODE *adrs;
	
	obj_id = VMGetWorkValue(core);
	adrs = (ACMD_CODE *)VMGetU32(core);
	AnmCmdSet( (u8)obj_id, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id, adrs );
	ScriptAnimeObjNumber = obj_id;
	return 0;
};

//div,mapID���w�肷��
u8 EvCmdObjAnimeSP(VM_MACHINE * core)				/* EVCMD_OBJANIMESP */
{
	u8  div_id,map_id;
	u16 obj_id;
	ACMD_CODE *adrs;
	
	obj_id = VMGetWorkValue(core);
	adrs = (ACMD_CODE *)VMGetU32(core);
	div_id = VMGetU8(core);
	map_id = VMGetU8(core);
	AnmCmdSet( (u8)obj_id, map_id, div_id, adrs );
	ScriptAnimeObjNumber = obj_id;
	return 0;
};

//-------------------------------------------------------------------------
//	�n�a�i�A�j���I���҂�
//-------------------------------------------------------------------------
static u8 EvWaitAnimeEnd(void)
{
	/* �߂�l ==1:����I���@==0:���쒆 */
	return AnmCmdEnd(ScriptAnimeObjNumber,ScriptAnimeMapID,ScriptAnimeDivID);
};

u8 EvCmdObjAnimeWait(VM_MACHINE * core)			/* EVCMD_OBJANIMEWAIT */
{
	u16 val = VMGetWorkValue(core);
	if( val != 0 )ScriptAnimeObjNumber = val;
	ScriptAnimeDivID = Fld.MapID.div_id;
	ScriptAnimeMapID = Fld.MapID.map_id;

	VM_SetWait( core, EvWaitAnimeEnd );
	return 1;
};

//div,mapID���w�肷��
u8 EvCmdObjAnimeWaitSP(VM_MACHINE * core)			/* EVCMD_OBJANIMEWAITSP */
{
	u8  div_id,map_id;
	u16 val = VMGetWorkValue(core);
	if( val != 0 )ScriptAnimeObjNumber = val;

	div_id = VMGetU8(core);
	map_id = VMGetU8(core);
	ScriptAnimeDivID = div_id;
	ScriptAnimeMapID = map_id;

	VM_SetWait( core, EvWaitAnimeEnd );
	return 1;
};

//-------------------------------------------------------------------------
//	�n�a�i�̏���
//-------------------------------------------------------------------------
u8 EvCmdObjDel(VM_MACHINE * core)					/* EVCMD_OBJDEL */
{
	u16 obj_id;

	obj_id = VMGetWorkValue(core);
	EvObjDelId( (u8)obj_id, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id );
	return 0;
};

//div,mapID���w�肷��
u8 EvCmdObjDelSP(VM_MACHINE * core)					/* EVCMD_OBJDELSP */
{
	u16 obj_id;
	u8  div_id,map_id;

	obj_id = VMGetWorkValue(core);
	div_id = VMGetU8(core);
	map_id = VMGetU8(core);

	EvObjDelId( (u8)obj_id, map_id, div_id );
	return 0;
};

//-------------------------------------------------------------------------
//	�n�a�i�̕\��
//-------------------------------------------------------------------------
u8 EvCmdObjAppear(VM_MACHINE * core)				/* EVCMD_OBJAPPEAR */
{
	u16 obj_id;

	obj_id = VMGetWorkValue(core);
	EvObjAddId( (u8)obj_id, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id );
	return 0;
};

//div,mapID���w�肷��
u8 EvCmdObjAppearSP(VM_MACHINE * core)				/* EVCMD_OBJAPPEARSP */
{
	u16 obj_id;
	u8  div_id,map_id;

	obj_id = VMGetWorkValue(core);
	div_id = VMGetU8(core);
	map_id = VMGetU8(core);
	EvObjAddId( (u8)obj_id, map_id, div_id );
	return 0;
};

//-------------------------------------------------------------------------
//	�n�a�i�\���ʒu�̕ύX
//-------------------------------------------------------------------------
u8 EvCmdObjPosChg( VM_MACHINE * core )			/* EVCMD_OBJ_POSCHG */
{
	u16 id,x,y;

	id = VMGetWorkValue(core);
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);
	EvObjPosAllChgId( (u8)id, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id, (s16)x, (s16)y );
	return 0;
}

//-------------------------------------------------------------------------
//	SXY�f�[�^�̍��W����ύX
//-------------------------------------------------------------------------
u8 EvCmdSxyPosChange( VM_MACHINE * core )	
{
	u16 id,px,py;
	id = VMGetWorkValue(core);
	px = VMGetWorkValue(core);
	py = VMGetWorkValue(core);
	SxyObjDataPosChange( id, px, py );
	return 0;
}

//-------------------------------------------------------------------------
//	���݂�OBJ�ʒu�ɂ��킹��SXY�f�[�^�̍��W�����X�V
//-------------------------------------------------------------------------
u8 EvCmdSxyPosOverWrite( VM_MACHINE * core )
{
	u16 id;

	id = VMGetWorkValue(core);
	EvObjSxyIdPosChange( (u8)id, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id );
	return 0;
}

//-------------------------------------------------------------------------
//	�n�a�i�̌�����E�����Ȃ��X�C�b�`�؂�ւ�
//-------------------------------------------------------------------------
u8 EvCmdObjSetVisible( VM_MACHINE * core )
{
	u16 id;
	u8	map,div;
	id = VMGetWorkValue(core);
	div = VMGetU8(core);
	map = VMGetU8(core);
	EvObjBanishSet( (u8)id, map, div, OFF );
	return 0;
}

u8 EvCmdObjSetInvisible( VM_MACHINE * core )
{
	u16 id;
	u8	map,div;
	id = VMGetWorkValue(core);
	div = VMGetU8(core);
	map = VMGetU8(core);
	EvObjBanishSet( (u8)id, map, div, ON );
	return 0;
}

//-------------------------------------------------------------------------
//	�n�a�i�v���C�I���e�B�̕ύX
//-------------------------------------------------------------------------
u8 EvCmdObjPriChg( VM_MACHINE * core )			/* EVCMD_OBJPRICHG */
{
	u16 id;
	u8	div,map,pri;

	id = VMGetWorkValue(core);
	div = VMGetU8(core);
	map = VMGetU8(core);
	pri = VMGetU8(core);

	EvObjPriChg( (u8)id, map, div, EVOBJ_ACTPRI0 + pri );
	return 0;
}

//-------------------------------------------------------------------------
//	�n�a�i�v���C�I���e�B��������ʒu�ɏC������
//-------------------------------------------------------------------------
u8 EvCmdObjPriOff( VM_MACHINE * core )			/* EVCMD_OBJPRIOFF */
{
	u16 id;
	u8	div,map;

	id = VMGetWorkValue(core);
	div = VMGetU8(core);
	map = VMGetU8(core);

	EvObjPriChgOff( (u8)id, map, div );
	return 0;
}

//--------------------------------------------------------------------
//	�b���|�����n�a�i�Ɏ��@�̕�����������
//--------------------------------------------------------------------
u8 EvCmdObjSiteToHero( VM_MACHINE * core )
{
	if (EvObj[TargetObjNo].sw == 1) {
		AcmdCodeTurnSiteSet( &EvObj[TargetObjNo], GetHeroSite() );
	}
	return 0;
}

//-------------------------------------------------------------------------
//	�n�a�i�̌�����ύX
//-------------------------------------------------------------------------
u8 EvCmdObjSiteChg(VM_MACHINE * core)				/* EVCMD_OBJSITECHG */
{
	u16 obj_id;
	u8 site;

	obj_id = VMGetWorkValue(core);
	site = VMGetU8(core);
	EvObjChgSiteId(obj_id, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id, site);
	return 0;
};

//-------------------------------------------------------------------------
//	SXY�f�[�^�̈ړ������ύX
//-------------------------------------------------------------------------
u8 EvCmdSxySiteChange( VM_MACHINE * core )	
{
	u16 id;
	u8	site;
	id = VMGetWorkValue(core);
	site = VMGetU8(core);
	SxyObjDataSiteChange( id, site );
	return 0;
}

//-------------------------------------------------------------------------
//	�t�B�[���hOBJ�_�~�[�A�N�g�ǉ�
//-------------------------------------------------------------------------
u8 EvCmdObjDmyActAdd( VM_MACHINE * core )	
{
	u8 code,id,h,site;
	u16	x,y;

	code	= VMGetU8(core);
	id		= VMGetU8(core);
	x		= VMGetWorkValue(core);
	y		= VMGetWorkValue(core);
	h		= VMGetU8(core);
	site	= VMGetU8(core);

	EvObjDmyAddAct( code, id, (s16)x, (s16)y, h, site );
	return 0;
}

//-------------------------------------------------------------------------
//	�t�B�[���hOBJ�_�~�[�A�N�g�����ύX
//-------------------------------------------------------------------------
u8 EvCmdObjDmySiteChg( VM_MACHINE * core )	
{
	u16 id;
	u8	site;

	id		= VMGetU8(core);
	site	= VMGetU8(core);
	
	EvObjDmyActSiteChg( id, site );
	return 0;
}


//=========================================================================
//
//	���b�Z�[�W�E�E�B���h�E�֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//	��b�X�^�[�g
//-------------------------------------------------------------------------
u8 EvCmdTalkStart( VM_MACHINE * core )
{
	if ( IsFieldCommMode() ) return 0;
	AddTalkStartTask();
	VM_SetWait( core, EvWaitTalkStart );
	return 1;
}

//-------------------------------------------------------------------------
//	�n�a�i�Ƃ̉�b�X�^�[�g
//-------------------------------------------------------------------------
u8 EvCmdTalkObjStart( VM_MACHINE * core )
{
	if ( IsFieldCommMode() ) return 0;

	if (EvObj[TargetObjNo].sw == 1) {
		AddTalkObjStartTask();
		VM_SetWait( core, EvWaitTalkObjStart );
	} else {
		AddTalkStartTask();
		VM_SetWait( core, EvWaitTalkStart );
	}

	return 1;
}

//-------------------------------------------------------------------------
//	��b�I��
//-------------------------------------------------------------------------
u8 EvCmdTalkEnd( VM_MACHINE * core )
{
	FieldMsgClear();

	AcmdEnd(EvObjSpWorkAdrs(EV_OBJ_SPID));			// ���@�����A�j������
	AnmCmdFree();
	EvObjPauseClrAll();
	return 0;
}

//-------------------------------------------------------------------------
//	�n�a�i�Ƃ̉�b�I��
//-------------------------------------------------------------------------
u8 EvCmdTalkObjEnd( VM_MACHINE * core )
{
	FieldMsgClear();

	if (EvObj[TargetObjNo].sw == 1) AcmdEnd(&EvObj[TargetObjNo]);
	AcmdEnd(EvObjSpWorkAdrs(EV_OBJ_SPID));			// ���@�����A�j������
	AnmCmdFree();
	EvObjPauseClrAll();
	return 0;
}

//-------------------------------------------------------------------------
//	��b�l�r�f�Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdTalkMsg(VM_MACHINE * core)					/* EVCMD_TALKMSG		*/
{
	VM_CODE * msg = (VM_CODE *)VMGetU32(core);
	if( (u32)msg == 0 )msg = (VM_CODE *)(core->reg[0]);
	FieldMsgSet(msg);
	return 0;
};

//-------------------------------------------------------------------------
//	��b�l�r�f�Z�b�g�i�l�r�f�̃L�[����Ȃ��j
//-------------------------------------------------------------------------
u8 EvCmdSysTalkMsg(VM_MACHINE * core)			/* EVCMD_SYSTALKMSG		*/
{
	VM_CODE * msg = (VM_CODE *)VMGetU32(core);
	if( (u32)msg == 0 )msg = (VM_CODE *)(core->reg[0]);
	FieldMsgSetNoSkip(msg);
	return 0;
}

//-------------------------------------------------------------------------
//	��b�l�r�f�\���I���҂�
//-------------------------------------------------------------------------
u8 EvCmdTalkWait(VM_MACHINE * core)					/* EVCMD_TALKWAIT	*/
{
	VM_SetWait( core, FieldMsgWait );
	return 1;
};

//-------------------------------------------------------------------------
//	��b�E�B���h�E�����
//-------------------------------------------------------------------------
u8 EvCmdTalkWinClose(VM_MACHINE * core)				/* EVCMD_TALKCLOSE */
{
	FieldMsgClear();
	return 0;
};

//-------------------------------------------------------------------------
//	�L�[�E�F�C�g
//-------------------------------------------------------------------------
static u8 EvWaitABKey(void)
{
	if(sys.Trg & A_BUTTON )return TRUE;
	if(sys.Trg & B_BUTTON )return TRUE;
	return FALSE;
};

u8 EvCmdABKeyWait(VM_MACHINE * core)
{
	VM_SetWait( core, EvWaitABKey );
	return 1;
};

//-------------------------------------------------------------------------
//	�u�͂��E�������v�E�B���h�E
//-------------------------------------------------------------------------
u8 EvCmdYesNoWin(VM_MACHINE * core)				/* EVCMD_YESNOWIN */
{
	u8 x,y;

	x = VMGetU8(core);
	y = VMGetU8(core);
	if( AddScriptYesNoTask(x,y) == TRUE )
	{
		StopFieldScript();
		return 1;
	};
	return 0;
};

//-------------------------------------------------------------------------
//	�X�N���v�g�@�E�B���h�E	add 01/09/26 nohara
//-------------------------------------------------------------------------
u8 EvCmdSelectWin(VM_MACHINE * core)		/* EVCMD_SELECTWIN */
{
	u8 x,y,no,check;

	x  = VMGetU8(core);						//8bit(1byte)�f�[�^�擾
	y  = VMGetU8(core);
	no = VMGetU8(core);
	check = VMGetU8(core);					//B�L�����Z���L�肩

	if( AddScriptEvWinTask(x,y,no,check) == TRUE )
	{
		StopFieldScript();
		return 1;
	};
	return 0;
};

//�J�[�\�������ʒu�w��
u8 EvCmdSelectWinCsr(VM_MACHINE * core)		/* EVCMD_SELECTWINCSR */
{
	u8 x,y,no,check,csr;

	x  = VMGetU8(core);						//8bit(1byte)�f�[�^�擾
	y  = VMGetU8(core);
	no = VMGetU8(core);
	csr = VMGetU8(core);
	check = VMGetU8(core);					//B�L�����Z���L�肩

	if( AddScriptEvWinCsrTask(x,y,no,check,csr) == TRUE )
	{
		StopFieldScript();
		return 1;
	};
	return 0;
};

//-------------------------------------------------------------------------
//	�E�B���h�E(�̂�)�\��
//-------------------------------------------------------------------------
u8 EvCmdWinWrite(VM_MACHINE * core)				/* EVCMD_WINWRITE */
{
	u8 x,y,x2,y2;

	x   = VMGetU8(core);						//8bit(1byte)�f�[�^�擾
	y   = VMGetU8(core);
	x2  = VMGetU8(core);						
	y2  = VMGetU8(core);

	NMenuWinBoxWrite( x, y, x2, y2 );
	return 0;
};

//-------------------------------------------------------------------------
//	�c���X�N���v�g�@�E�B���h�E	add 02/01/17 nohara
//-------------------------------------------------------------------------
u8 EvCmdHVWin(VM_MACHINE * core)		/* EVCMD_HVWIN */
{
	u8 x,y,no,arrange,check;

	x  = VMGetU8(core);						//8bit(1byte)�f�[�^�擾
	y  = VMGetU8(core);
	no = VMGetU8(core);
	arrange = VMGetU8(core);				//���ו�
	check = VMGetU8(core);					//B�L�����Z���L�肩

	if( AddScriptEvHVWinTask(x,y,no,check,arrange) == TRUE )
	{
		//VM_SetWait( core, EvWaitYesNoEnd );
		StopFieldScript();
		return 1;
	};
	return 0;
};

//-------------------------------------------------------------------------
//			�@				�w��͈͂��N���A
//-------------------------------------------------------------------------
u8 EvCmdWinClear(VM_MACHINE * core)				/* EVCMD_WINCLEAR */
{
	u8 x,y,x2,y2;

	x   = VMGetU8(core);						//8bit(1byte)�f�[�^�擾
	y   = VMGetU8(core);
	x2  = VMGetU8(core);						
	y2  = VMGetU8(core);

	NMenuBoxClear( x, y, x2, y2);
	return 0;
};

//-------------------------------------------------------------------------
//					�X�N���v�g�@���j���[(�E�B���h�E�Ȃ�)
//-------------------------------------------------------------------------
u8 EvCmdSelectMenu(VM_MACHINE * core)		/* EVCMD_MENUWRITE */
{
	u8 x,y,no,check;

	x  = VMGetU8(core);						//8bit(1byte)�f�[�^�擾
	y  = VMGetU8(core);
	no = VMGetU8(core);
	check = VMGetU8(core);					//B�L�����Z���L�肩

	if( AddScriptEvMenuTask(x,y,no,check) == TRUE )
	{
		StopFieldScript();
		return 1;
	};
	return 0;
};

//-------------------------------------------------------------------------
//	�|�P�����b�f�\���E�B���h�E
//-------------------------------------------------------------------------
u8 EvCmdPokeCGWinOpen(VM_MACHINE * core)		/* EVCMD_POKECGWINOPEN */
{
	u16 no;
	u8	x,y;

	no = VMGetWorkValue(core);
	x = VMGetU8(core);
	y = VMGetU8(core);
	FieldPokeCGWinOpen(no,x,y);
	return 0;
};

u8 EvCmdPokeCGWinErase(VM_MACHINE * core)		/* EVCMD_POKECGWINERASE */
{
	void * res;
	res = FieldPokeCGWinClose();
	if( res == NULL )return 0;
	VM_SetWait( core, res );
	return 1;
};

//-------------------------------------------------------------------------
//	����G�\����т���
//	���f�o�b�O�@�\�𗬗p�����b��ŁB
//-------------------------------------------------------------------------
extern void PutPortrait( int number );
extern void PortraitPictureSet( int number );
u8 EvCmdPutPortrait( VM_MACHINE * core )
{
	u8 number;
	number = VMGetU8(core);
	if ( number != 0 ) PortraitPictureSet( number );
	PortraitPicturePut();

	StopFieldScript();
	return 1;
//	PutPortrait( VMGetU8(core) );
//	return 0;
}

//-------------------------------------------------------------------------
//		�_�����b�Z�[�W�\��
//-------------------------------------------------------------------------
u8 EvCmdTenjiMsgPut( VM_MACHINE * core )
{
	u8 x1,y1,x2,y2,mx,my;
	const u8 * adrs;

	adrs = (const u8 *)VMGetU32(core);
	x1 = adrs[0]; y1 = adrs[1]; x2 = adrs[2]; y2 = adrs[3];
	mx = adrs[4]; my = adrs[5];
	PM_TenjiExpand( MsgExpandBuffer, adrs+6 );
	NMenuWinBoxWrite(x1,y1,x2,y2);
	NMenuMsgWrite( MsgExpandBuffer, mx, my );
	return 0;
}

//-------------------------------------------------------------------------
//	��b�l�r�f�Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdTalkMsgRelative( VM_MACHINE * core )
{
	u32 msg;
	msg = GET_REL_ADRS( VMGetU32(core) );
//	if( (u32)msg == 0 )msg = (VM_CODE *)(core->reg[0]);
	FieldMsgSet( (const u8 *)msg );
	return 0;
}


//=========================================================================
//
//	������擾
//
//=========================================================================

static u8 * const buf_list[] =
{
	StrTempBuffer0,
	StrTempBuffer1,
	StrTempBuffer2,
};

//-------------------------------------------------------------------------
//	�|�P�������擾
//-------------------------------------------------------------------------
u8 EvCmdGetMonsName(VM_MACHINE * core)				/* EVCMD_GETMONSNAME */
{
	u8 buf;
	u16 mons;

	buf = VMGetU8(core);
	mons = VMGetWorkValue(core);
	PM_strcpy( buf_list[buf], PokeNameData[mons] );
	return 0;
};

//-------------------------------------------------------------------------
//	�^�}�S�𔲂������擪�̃|�P�����̖��O�擾
//-------------------------------------------------------------------------
u8 EvCmdGetTemotiMonsName(VM_MACHINE * core)			/* EVCMD_GETTEMOTIMONSNAME */
{
	u8 buf;

	buf = VMGetU8(core);
	PM_strcpy( buf_list[buf], 
			PokeNameData[PokeParaGet(&PokeParaMine[GetTemotiBtlPokeNum()],ID_monsno,0)] );
	return 0;
};

//-------------------------------------------------------------------------
//	�j�b�N�l�[���擾
//-------------------------------------------------------------------------
u8 EvCmdGetNickName(VM_MACHINE * core)				/* EVCMD_GETNICKNAME */
{
	u8 buf;
	u16 no;

	buf = VMGetU8(core);
	no = VMGetWorkValue(core);
	PokeParaGet( &PokeParaMine[no], ID_nickname, buf_list[buf] );
	NickNameCutOff( buf_list[buf] );
	return 0;
};

//-------------------------------------------------------------------------
//	�A�C�e�����擾
//-------------------------------------------------------------------------
u8 EvCmdGetItemName(VM_MACHINE * core)				/* EVCMD_GETITEMNAME */
{
	u8 buf;
	u16 item;

	buf = VMGetU8(core);
	item = VMGetWorkValue(core);
	GetItemName( item, buf_list[buf] );				//�����؂̎����擾�p
	return 0;
};

//-------------------------------------------------------------------------
//	�O�b�Y���擾
//-------------------------------------------------------------------------
u8 EvCmdGetGoodsName(VM_MACHINE * core)				/* EVCMD_GETGOODSNAME */
{
	u8 buf;
	u16 goods;

	buf = VMGetU8(core);
	goods = VMGetWorkValue(core);

	PM_strcpy(buf_list[buf], base_goods[goods].name );

	return 0;
};

//-------------------------------------------------------------------------
//	�Z���擾
//-------------------------------------------------------------------------
u8 EvCmdGetWazaName(VM_MACHINE * core)				/* EVCMD_GETWAZANAME */
{
	u8 buf;
	u16 waza;

	buf = VMGetU8(core);
	waza = VMGetWorkValue(core);
	PM_strcpy( buf_list[buf], &WazaNameData[waza][0] );
	return 0;
};

//-------------------------------------------------------------------------
//	�����擾
//-------------------------------------------------------------------------
u8 EvCmdGetNumMsg(VM_MACHINE * core)			/* EVCMD_GETNUMMSG */
{
	u8 buf,keta = 1;
	u16 num;

	buf = VMGetU8(core);
	num = VMGetWorkValue(core);
	keta = GetNumKeta(num);

	//���l�ߌŒ�ɂ��Ă���
	PM_NumMsgSet( buf_list[buf], (int)num, NUM_MODE_LEFT, keta);

	return 0;
};

//-------------------------------------------------------------------------
//	ev_win�ɂ��镶������擾����
//-------------------------------------------------------------------------
u8 EvCmdSetBuf(VM_MACHINE * core)				/* EVCMD_SETBUF */
{
	u8 buf;
	u16 no;

	buf = VMGetU8(core);
	no = VMGetWorkValue(core);
	PM_strcpy( buf_list[buf], msg_data_buf[no] );
	return 0;
};

//-------------------------------------------------------------------------
//	�w�蕶������o�b�t�@�ɃR�s�[
//-------------------------------------------------------------------------
u8 EvCmdCopyBuf( VM_MACHINE * core )
{
	u8 buf_no;
	const u8 * str;

	buf_no = VMGetU8(core);
	str = (const u8 *)VMGetU32(core);
	PM_strcpy( buf_list[buf_no], str );

	return 0;
}

//-------------------------------------------------------------------------
//	�w�胁�b�Z�[�W�̓W�J�������Ȃ�(���΃A�h���X�w��j
//-------------------------------------------------------------------------
u8 EvCmdExpandMsgRelative( VM_MACHINE * core )
{
	const u8 * msg;
	msg = (const u8 *)GET_REL_ADRS( VMGetU32(core) );
	PM_MsgExpand( MsgExpandBuffer, msg );
	return 0;
}

//-------------------------------------------------------------------------
//	�w�蕶������o�b�t�@�ɃR�s�[(���΃A�h���X�w��j
//-------------------------------------------------------------------------
u8 EvCmdCopyBufRelative( VM_MACHINE * core )
{
	u8 buf_no;
	const u8 * str;

	buf_no = VMGetU8(core);
	str = (const u8 *)GET_REL_ADRS( VMGetU32(core) );
	PM_strcpy( buf_list[buf_no], str );

	return 0;
}


//=========================================================================
//
//	�|�P�����֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//	�|�P�������莝���ɉ�����	add by matsuda 2001.09.28(��)
//-------------------------------------------------------------------------
u8 EvCmdAddPokemon(VM_MACHINE * core)
{
	u16 monsno, itemno;
	u8 lv;
	VM_CODE * pokename_adrs;
	u32 oyaname_adrs;
	u8 name_sw;
	
	monsno = VMGetWorkValue(core);
	lv = VMGetU8(core);
	itemno = VMGetWorkValue(core);
	pokename_adrs = (VM_CODE *)VMGetU32(core);
	
	oyaname_adrs = VMGetU32(core);
	name_sw = VMGetU8(core);
	
	AnswerWork = (u16)TemotiPokeAdd(monsno, lv, itemno, pokename_adrs, (void*)oyaname_adrs, name_sw);
	return 0;
}

//-------------------------------------------------------------------------
//	�^�}�S���莝���ɉ�����		add by matsuda 2002.08.13(��)
//-------------------------------------------------------------------------
u8 EvCmdAddTamago(VM_MACHINE * core)
{
	u16 monsno;
	
	monsno = VMGetWorkValue(core);
	AnswerWork = (u16)TemotiTamagoAdd(monsno);
	return 0;
}

//-------------------------------------------------------------------------
//	�莝���|�P�����̋Z��u��������
//-------------------------------------------------------------------------
u8 EvCmdPokeWazaChange( VM_MACHINE * core )
{
	u8 pos,waza_pos;
	u16 wazano;

	pos = VMGetU8(core);
	waza_pos = VMGetU8(core);
	wazano = VMGetU16(core);
	TemotiPokeWazaChange( pos, wazano, waza_pos );

	return 0;
}

//-------------------------------------------------------------------------
//	�w�肳�ꂽ�Z���o���Ă���莝���|�P�����𒲂ׂ�	2002/01/23 by nakahiro
//-------------------------------------------------------------------------
extern	u8 TemotiNijyuuCheck(PokemonParam *pp, u16 wazano);

u8 EvCmdPokeWazaCheck( VM_MACHINE * core )
{
	u8	i;
	u16	waza;
	u16 monsno;

	waza = VMGetU16(core);
	AnswerWork = TEMOTI_POKEMAX;		// �o���Ă���|�P���������Ȃ�

	for( i=0; i<TEMOTI_POKEMAX; i++ ){
		monsno = PokeParaGet( &PokeParaMine[i], ID_monsno, 0 );
		if(	monsno == 0 )	break;
		if( PokeParaGet( &PokeParaMine[i], ID_tamago_flag ) == 0 &&
			TemotiNijyuuCheck( &PokeParaMine[i], waza ) == 1 )
		{
			AnswerWork = i;
			ScriptParameter0 = monsno;
			break;
		}
	}

	return	0;
}


//=========================================================================
//
//	�����֘A
//
//=========================================================================

//-------------------------------------------------------------------------
//	������������	add 01/10/03 nohara
//-------------------------------------------------------------------------
u8 EvCmdAddGold(VM_MACHINE * core)		/* EVCMD_ADDGOLD */
{
	u32 gold;
	u8 code;

	gold = VMGetU32(core);
	code = VMGetU8(core);

	//�ő�̋��z��������A���z�ɕύX�Ȃ��ł悢�H
	if( code == 0 )						//�莝��
	{
		AddGold( &Fld.my_gold, gold );
	}else								//�a���Ă��邨��
	{
		//AddGold( ???, gold );
	}
		
	return 0;
};

//-------------------------------------------------------------------------
//	����������	add 01/10/03 nohara
//-------------------------------------------------------------------------
u8 EvCmdSubGold(VM_MACHINE * core)		/* EVCMD_SUBGOLD */
{
	u32 gold;
	u8 code;

	gold = VMGetU32(core);
	code = VMGetU8(core);

	if( code == 0 )						//�莝��
	{
		SubGold( &Fld.my_gold, gold );
	}else								//�a���Ă��邨��
	{
		//SubGold( ???, gold );
	}
		
	return 0;
};

//-------------------------------------------------------------------------
//	���z�𒲂ׂ�	add 01/10/03 nohara
//-------------------------------------------------------------------------
u8 EvCmdCompGold(VM_MACHINE * core)		/* EVCMD_COMPGOLD */
{
	u32 gold;
	u8 code;

	gold = VMGetU32(core);
	code = VMGetU8(core);

	if( code == 0 )						//�莝��
	{
		AnswerWork=CheckGold( Fld.my_gold, gold );
	}else								//�a���Ă��邨��
	{
		//AnswerWork=CheckGold( ???, gold );
	}
		
	return 0;
};

//-------------------------------------------------------------------------
//	�������E�B���h�E�\��	add 02/06/26 nohara
//-------------------------------------------------------------------------
u8 EvCmdGoldWinWrite(VM_MACHINE * core)		/* EVCMD_GOLDWINWRITE */
{
	u8 x,y,code;

	x = VMGetU8(core);
	y = VMGetU8(core);
	code = VMGetU8(core);

	if( code == 0 )						//�莝��
	{
		DrawGoldWin( Fld.my_gold, x, y );
	}else								//�a���Ă��邨��
	{
		//DrawGoldWin( ???, x, y );
	}
		
	return 0;
};

//-------------------------------------------------------------------------
//	�������E�B���h�E�폜	add 02/06/26 nohara
//-------------------------------------------------------------------------
u8 EvCmdGoldWinDel(VM_MACHINE * core)		/* EVCMD_GOLDWINDEL */
{
	u8 x,y;

	x = VMGetU8(core);
	y = VMGetU8(core);

	DelOkozukaiWin( x, y );
	return 0;
};

//-------------------------------------------------------------------------
//	�������ĕ\��	add 02/06/26 nohara
//-------------------------------------------------------------------------
u8 EvCmdGoldWrite(VM_MACHINE * core)		/* EVCMD_GOLDWRITE */
{
	u8 x,y,code;

	x = VMGetU8(core);
	y = VMGetU8(core);
	code = VMGetU8(core);

	if( code == 0 )						//�莝��
	{
		DrawWinOkozukai( Fld.my_gold, x, y );
	}else								//�a���Ă��邨��
	{
		//DrawWinOkozukai( ???, x, y );
	}
		
	return 0;
};


//-------------------------------------------------------------------------
//	�����R�C���E�B���h�E�\��	add 02/07/17 tomo
//-------------------------------------------------------------------------
u8 EvCmdCoinWinWrite(VM_MACHINE * core)			/* EVCMD_COINWINWRITE */
{
	u8 x,y;

	x = VMGetU8(core);
	y = VMGetU8(core);

	DrawCoinWin( Fld.my_coin, x, y );

	return 0;
}

//-------------------------------------------------------------------------
//	�����R�C���E�B���h�E�폜	add 02/07/17 tomo
//-------------------------------------------------------------------------
u8 EvCmdCoinWinDel(VM_MACHINE * core)				/* EVCMD_COINWINDEL */
{
	u8 x,y;

	x = VMGetU8(core);
	y = VMGetU8(core);

	DelCoinWin( x, y );

	return 0;
}

//-------------------------------------------------------------------------
//	�����R�C���ĕ\��	add 02/07/17 tomo
//-------------------------------------------------------------------------
u8 EvCmdCoinWrite(VM_MACHINE * core)				/* EVCMD_COINWRITE */
{
	u8 x,y;

	x = VMGetU8(core);
	y = VMGetU8(core);

	DrawWinCoin( Fld.my_coin, x, y );

	return 0;
}


//=========================================================================
//
//	�퓬�֘A
//
//=========================================================================
//-------------------------------------------------------------------------
//	�g���[�i�[��̐ݒ�
//-------------------------------------------------------------------------
u8 EvCmdTrainerDataSet( VM_MACHINE * core )
{
	core->PC = (VM_CODE *) SetTrainerDataByAdrs( core->PC );
	return 0;
}

//-------------------------------------------------------------------------
//	�g���[�i�[��J�n
//-------------------------------------------------------------------------
u8 EvCmdTrainerFightStart( VM_MACHINE * core )
{
	FieldTrainerFightStart();
	return 1;
}

//-------------------------------------------------------------------------
//	�g���[�i�[��ς݂̏ꍇ�̃X�N���v�g�֕���
//-------------------------------------------------------------------------
u8 EvCmdTrainerAfterScript( VM_MACHINE * core )
{
	core->PC = GetTrainerAfterScript();
	return 0;
}

//-------------------------------------------------------------------------
//	�g���[�i�[��I������̃X�N���v�g�֕���
//-------------------------------------------------------------------------
u8 EvCmdTrainerBattleEndScript( VM_MACHINE * core )
{
	core->PC = GetTrainerBattleEndScript();
	return 0;
}

//-------------------------------------------------------------------------
//	�g���[�i�[ID����t���O���`�F�b�N
//-------------------------------------------------------------------------
u8 EvCmdTrainerFlagCheck( VM_MACHINE * core )
{
	core->cmp_flag = CheckTrainerFlagByID( VMGetWorkValue(core) );
	return 0;
}

//-------------------------------------------------------------------------
//	�g���[�i�[ID����t���O���Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdTrainerFlagSet( VM_MACHINE * core )
{
	SetTrainerFlagByID( VMGetWorkValue(core) );
	return 0;
}

//-------------------------------------------------------------------------
//	�g���[�i�[ID����t���O�����Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdTrainerFlagReset( VM_MACHINE * core )
{
	ResetTrainerFlagByID( VMGetWorkValue(core) );
	return 0;
}

//-------------------------------------------------------------------------
//	�쐫�|�P�����̃f�[�^���Z�b�g
//-------------------------------------------------------------------------
u8 EvCmdWildFightSet( VM_MACHINE * core )
{
	u16 pokeno,itemno;
	u8 level;
	pokeno = VMGetU16(core);
	level = VMGetU8(core);
	itemno = VMGetU16(core);
	ScrEncountPokeSet( pokeno, level, itemno );
	return 0;
}

//-------------------------------------------------------------------------
//	�쐶�|�P�����Ƃ̃G���J�E���g��J�n
//-------------------------------------------------------------------------
extern void StartScriptEncountEvent(void);

u8 EvCmdWildFightStart( VM_MACHINE * core )
{
	StartScriptEncountEvent();
	StopFieldScript();
	return 1;
}


//=========================================================================
//
//	�ʂ̃C�x���g�֘A�̃X�N���v�g�R�}���h
//
//=========================================================================

//-------------------------------------------------------------------------
//
//	���X�̌Ăяo��
//
//-------------------------------------------------------------------------
extern void FdShopSet( u32 adrs );
extern void GoodsShopSet( u32 adrs );
extern void FlowerShopSet( u32 adrs );

// �t�����h���B�V���b�v
u8 EvCmdShopStart( VM_MACHINE * core )
{
	FdShopSet( VMGetU32(core) );
	StopFieldScript();
	return 1;
}

// �O�b�Y�V���b�v
u8 EvCmdGoodsShopStart( VM_MACHINE * core )
{
	GoodsShopSet( VMGetU32(core) );
	StopFieldScript();
	return 1;
}

// �t�����[�V���b�v
u8 EvCmdFlowerShopStart( VM_MACHINE * core )
{
	FlowerShopSet( VMGetU32(core) );
	StopFieldScript();
	return 1;
}

//-------------------------------------------------------------------------
//
//	�X���b�g�}�V�[���̌Ăяo��
//
//-------------------------------------------------------------------------
extern void FieldScriptRecover(void);

u8 EvCmdSlotMachine( VM_MACHINE * core )
{
	Slot( VMGetWorkValue(core), FieldScriptRecover );
	StopFieldScript();
	return 1;
}

//-------------------------------------------------------------------------
//
//	���݂̂�A���鏈��
//
//-------------------------------------------------------------------------
#define SET_SEED_USER_ID	0
u8 EvCmdSetSeed( VM_MACHINE * core )
{
	u8 bed_no,type,growth;
	bed_no = VMGetU8(core);
	type = VMGetU8(core);
	growth = VMGetU8(core);

	if( type == SET_SEED_USER_ID )
	{
		//�^�C�v�����߂鏈��������
		//GetSeedType();
		SeedBedSet( bed_no, type, growth, FALSE );
	}else
	{
		SeedBedSet( bed_no, type, growth, FALSE );
	}
	return 0;
}

//-------------------------------------------------------------------------
//	CM�f�[�^�J�Ã`�F�b�N
//-------------------------------------------------------------------------
u8 EvCmdCMEventStartChk(VM_MACHINE * core)					
{
	u16 type;

	type = VMGetWorkValue(core);
	AnswerWork = (u16)CMEventStartChk((u8)type);
	return 0;
}


//=========================================================================
//
//
//	�R���e�X�g�֘A�ǉ�	01/10/16 nohara
//
//
//=========================================================================

extern void ContestPokeListInit(void);
//-------------------------------------------------------------------------
//			�R���e�X�g�̃|�P������I��(�X�e�[�^�X���)���Ă�	
//-------------------------------------------------------------------------
u8 EvCmdConPokeList( VM_MACHINE * core )
{
	ContestPokeListInit();
	StopFieldScript();
	return 1;
};

//-------------------------------------------------------------------------
//				�@�񎟐R�����Ăяo��
//-------------------------------------------------------------------------
u8 EvCmdConNijiCall( VM_MACHINE * core )
{
	ContestNijiAppeal();
	StopFieldScript();
	return 1;
}

//-------------------------------------------------------------------------
//					���ʔ��\��ʂ��Ăяo��	
//-------------------------------------------------------------------------
u8 EvCmdConResultScr( VM_MACHINE * core )
{
	ContestResultScr();
	StopFieldScript();
	return 1;
}

//-------------------------------------------------------------------------
//					�R���e�X�g�ʐM
//-------------------------------------------------------------------------
u8 EvCmdConTuusinInit( VM_MACHINE * core )
{
	ContestTuusinInit(ContestKind);
	StopFieldScript();
	return 1;
}

static u16 ScriptFldEffCodeNumber = 0;
//-------------------------------------------------------------------------
//	�t�B�[���h�G�t�F�N�g���Ă�			02/01/24 nohara
//-------------------------------------------------------------------------
u8 EvCmdSetFldEff( VM_MACHINE * core )
{
	u16 no;
	no = VMGetWorkValue(core);
	ScriptFldEffCodeNumber = no;
	FldEffSet( (u8)no );
	return 0;
};

//-------------------------------------------------------------------------
//	�t�B�[���h�G�t�F�N�g�̃��[�N�ɒl���Z�b�g�@2002/01/24 by nakahiro
//-------------------------------------------------------------------------
u8 EvCmdSetFEWork( VM_MACHINE * core )
{
	u8	no;
	u16	param;

	no = VMGetU8(core);
	param = VMGetWorkValue(core);
	FeSetWork[no] = (s16)param;
	return 0;
}

//-------------------------------------------------------------------------
//	�t�B�[���h�G�t�F�N�g�I���҂�		02/01/25 nohara
//-------------------------------------------------------------------------
static u8 EvWaitFldEffEnd(void)
{
	/* �߂�l ==1:����I���@==0:���쒆 */
	if( FeCodeChk((u8)ScriptFldEffCodeNumber) == 0 ) return 1;	//�I��
	else	return 0;
};

u8 EvCmdFldEffWait(VM_MACHINE * core)	/* EVCMD_FLDEFFWAIT */
{
	u16 code = VMGetWorkValue(core);
	ScriptFldEffCodeNumber = code;
	VM_SetWait( core, EvWaitFldEffEnd );
	return 1;
};

//-------------------------------------------------------------------------
//	���[�v����Z�b�g����			02/01/25 nohara
//-------------------------------------------------------------------------
u8 EvCmdSetWarpID( VM_MACHINE * core )
{
	u16 no;
	no = VMGetWorkValue(core);
	TeleportMapSetByWarpID( (u8)no );
	return 0;
};

//-------------------------------------------------------------------------
//	��l���̐��ʂ��擾����			02/02/06 nohara
//-------------------------------------------------------------------------
u8 EvCmdGetMySex( VM_MACHINE * core )
{
	AnswerWork = (u16)MyData.my_sex;
	return 0;
};

//-------------------------------------------------------------------------
//	�|�P�����̋�������炷		02/07/12 nohara
//-------------------------------------------------------------------------
u8 EvCmdVoicePlay( VM_MACHINE * core )
{
	u16 no,ptn;
	no = VMGetWorkValue(core);
	ptn = VMGetWorkValue(core);
	VoicePlayEvent( no, (u8)ptn );
	return 0;
};

u8 EvCmdVoicePlayWait( VM_MACHINE * core )
{
	VM_SetWait( core, VoicePlayWait );
	return 1;
}

//-------------------------------------------------------------------------
//	�w��ʒu�ɃZ�����Z�b�g			02/02/15 nohara
//-------------------------------------------------------------------------
u8 EvCmdSetCell( VM_MACHINE * core )
{
	u16 x,y,code,hitchk;
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);
	code = VMGetWorkValue(core);
	hitchk = VMGetWorkValue(core);

	x += MAP_X_MARGIN;
	y += MAP_Y_MARGIN;
	if( hitchk == 0 )	SetCell( x, y , code );
	else	SetCell( x, y , code | CELL_HITCHECK_MASK );

	return 0;
};

//-------------------------------------------------------------------------
//	�h�A�A�j���Ăяo��				02/06/01 nohara
//-------------------------------------------------------------------------
extern void AddCloseDoorAnimeTask( int, int );
extern void AddOpenDoorAnimeTask( int, int );
extern void CloseDoorCellSet( int, int );
extern void OpenDoorCellSet( int, int );
extern u16 GetDoorSENumber(int px, int py);

//�A�j���[�V�����L��(������)
u8 EvCmdOpenDoorAnime( VM_MACHINE * core )
{
	u16 x,y;
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	x += MAP_X_MARGIN;
	y += MAP_Y_MARGIN;
	
	SePlay( GetDoorSENumber(x,y) );
	AddOpenDoorAnimeTask( (int)x, (int)y );		//������
	return 0;
}

//�A�j���[�V�����L��(���߂�)
u8 EvCmdCloseDoorAnime( VM_MACHINE * core )
{
	u16 x,y;
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	x += MAP_X_MARGIN;
	y += MAP_Y_MARGIN;
	
	AddCloseDoorAnimeTask( (int)x, (int)y );	//���߂�
	return 0;
}

//�h�A�A�j���[�V�����I���҂�
extern u8 CheckDoorAnime();
static u8 EvWaitDoorAnimeEnd(void)
{
	/* �߂�l ==1:����I���@==0:���쒆 */
	if ( CheckDoorAnime() == FALSE ) return TRUE;
	else return FALSE;
}

u8 EvCmdDoorAnimeWait( VM_MACHINE * core )
{
	VM_SetWait( core, EvWaitDoorAnimeEnd );
	return 1;
}

//�h�A�̃Z�����Z�b�g����(�����Ă���)
u8 EvCmdOpenDoorSet( VM_MACHINE * core )
{
	u16 x,y;
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	x += MAP_X_MARGIN;
	y += MAP_Y_MARGIN;
	
	OpenDoorCellSet( (int)x, (int)y );			//������
	return 0;
}

//�h�A�̃Z�����Z�b�g����(���܂��Ă���)
u8 EvCmdCloseDoorSet( VM_MACHINE * core )
{
	u16 x,y;
	x = VMGetWorkValue(core);
	y = VMGetWorkValue(core);

	x += MAP_X_MARGIN;
	y += MAP_Y_MARGIN;
	
	CloseDoorCellSet( (int)x, (int)y );			//���߂�
	return 0;
}

//-------------------------------------------------------------------------
//
//	�G���x�[�^�̌Ăяo��
//
//-------------------------------------------------------------------------
extern void ElevatorSet(u8,u8,u8,u8);
extern void ElevatorStart();

u8 EvCmdElevatorSet( VM_MACHINE * core )
{
	u8  floor;
	u16 div,map,exit;
	floor = VMGetU8(core);
	div = VMGetWorkValue(core);
	map = VMGetWorkValue(core);
	exit = VMGetWorkValue(core);

	ElevatorSet(floor,(u8)div,(u8)map,(u8)exit);
	return 0;
}

u8 EvCmdElevatorStart( VM_MACHINE * core )
{
	ElevatorStart();
	StopFieldScript();
	return 1;
}

//-------------------------------------------------------------------------
//
//	�R�C���֘A		2002/07/03 by nakahiro
//
//-------------------------------------------------------------------------
// �莝���R�C���̖������擾
u8 EvCmdCheckCoin( VM_MACHINE * core )
{
	u16 * work;
	
	work  = VMGetWork(core);
	*work = G_CheckCoin();
	return	0;
}

// �莝���R�C���̖�����������
u8 EvCmdAddCoin( VM_MACHINE * core )
{
	u16	coin;

	coin = VMGetWorkValue(core);
	if( G_AddCoin( coin ) == TRUE )	AnswerWork = 0;
	else							AnswerWork = 1;

	return	0;
}

// �莝���R�C���̖��������炷
u8 EvCmdSubCoin( VM_MACHINE * core )
{
	u16	coin;

	coin = VMGetWorkValue(core);
	if( G_SubCoin( coin ) == TRUE )	AnswerWork = 0;
	else							AnswerWork = 1;

	return	0;
}


