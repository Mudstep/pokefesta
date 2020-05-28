@===================================================================
@
@	�X�N���v�g�p���[�N�e�[�u����`
@
@	_2001_08_24 by tama
@
@===================================================================

@----------------------------------------------------------
@
@	�X�N���v�g���[�N��`�p�̃}�N���錾
@
@----------------------------------------------------------
	.MACRO	INIT_WORK
.IFDEF	EV_WORK_S_GLOBAL
	.ALIGN
.ENDIF
DEF_WORK_COUNT	=	SCWK_START
	.ENDM

	.MACRO	DEF_WORK	workID , symname

.IFDEF	EV_WORK_S_GLOBAL			@ �B��̌��ł����e�[�u���͐������Ȃ�
.WORD	\symname
.ENDIF
\workID			=	DEF_WORK_COUNT
DEF_WORK_COUNT 	=	(DEF_WORK_COUNT + 1)

	.ENDM

.MACRO	EV_WORK_LABEL	adrs
.IFDEF	EV_WORK_S_GLOBAL
	.GLOBAL	\adrs
\adrs:
.ENDIF
.ENDM


	INIT_WORK
	EV_WORK_LABEL		ScriptWorkTable
/*	�X�N���v�g�}�N�����Ŏg�p����ėp���W�X�^ */
	DEF_WORK	RG0,		ScriptRegister0
	DEF_WORK	RG1,		ScriptRegister1
	DEF_WORK	RG2,		ScriptRegister2
	DEF_WORK	RG3,		ScriptRegister3

	DEF_WORK	PARAM0,		ScriptParameter0
	DEF_WORK	PARAM1,		ScriptParameter1
	DEF_WORK	PARAM2,		ScriptParameter2
	DEF_WORK	PARAM3,		ScriptParameter3

	DEF_WORK	TMPWK0,		ScriptTempWork0
	DEF_WORK	TMPWK1,		ScriptTempWork1
	DEF_WORK	TMPWK2,		ScriptTempWork2
	DEF_WORK	TMPWK3,		ScriptTempWork3

	DEF_WORK	RG_SITE,	TalkSiteWork
	DEF_WORK	ANSWORK,	AnswerWork
	DEF_WORK	ITEMNO,		ReturnItemNo
	DEF_WORK	TARGET_OBJ,	TargetObjID
	DEF_WORK	CON_RANK,	ContestRank
	DEF_WORK	CON_TYPE,	ContestKind

	EV_WORK_LABEL		ScriptWorkTableEnd

