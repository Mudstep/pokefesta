@===================================================================
@
@	�X�N���v�g�R�}���h�e�[�u����`
@
@	_2001_08_24 by tama
@
@===================================================================


@----------------------------------------------------------
@
@	�X�N���v�g�R�}���h��`�p�̃}�N���錾
@
@----------------------------------------------------------
	.MACRO	INIT_CMD
.IFDEF	EV_CMD_S_GLOBAL
	.ALIGN
.ENDIF
DEF_CMD_COUNT	=	0
	.ENDM

	.MACRO	DEF_CMD	symname , funcname

.IFDEF	EV_CMD_S_GLOBAL			@ �B��̌��ł����e�[�u���͐������Ȃ�
.WORD	\funcname
.ENDIF
\symname	=	DEF_CMD_COUNT
DEF_CMD_COUNT 	=	(DEF_CMD_COUNT + 1)

	.ENDM

	.MACRO	DEF_CMD_LABEL	label
.IFDEF	EV_CMD_S_GLOBAL
	.GLOBAL	\label
\label:
.ENDIF
	.ENDM

@----------------------------------------------------------
@	�X�N���v�g�R�}���h��`�e�[�u��
@----------------------------------------------------------
@	.GLOBAL	ScriptCommand		@ �֐��e�[�u���V���{��
@	.GLOBAL	ScriptCommandEnd	@ �R�}���h�̍Ō������

	INIT_CMD

	DEF_CMD_LABEL	ScriptCommand

	DEF_CMD	EVCMD_NOP			,EvCmdNop
	DEF_CMD	EVCMD_DUMMY			,EvCmdDummy
	DEF_CMD	EVCMD_END			,EvCmdEnd

	DEF_CMD	EVCMD_RET			,EvCmdRet
	DEF_CMD	EVCMD_GLOBAL_CALL	,EvCmdGlobalCall
	DEF_CMD	EVCMD_GLOBAL_JUMP	,EvCmdGlobalJump
	DEF_CMD	EVCMD_IFJUMP		,EvCmdIfJump
	DEF_CMD	EVCMD_IFCALL		,EvCmdIfCall

	DEF_CMD	EVCMD_JUMPID		,EvCmdJumpID
	DEF_CMD	EVCMD_CALLID		,EvCmdCallID
	DEF_CMD	EVCMD_IFJUMPID		,EvCmdIfJumpID
	DEF_CMD	EVCMD_IFCALLID		,EvCmdIfCallID
	DEF_CMD	EVCMD_CHG_SCRIPT	,EvCmdChangeScript
	DEF_CMD	EVCMD_CLR_EXSCRIPT	,EvCmdClearExScript
	DEF_CMD	EVCMD_SET_EXERROR	,EvCmdSetExDataError

	DEF_CMD EVCMD_LD_REG_WDATA	,EvCmdLoadRegWData
	DEF_CMD	EVCMD_LD_REG_VAL	,EvCmdLoadRegValue
	DEF_CMD	EVCMD_LD_ADR_VAL	,EvCmdLoadAdrsValue
	DEF_CMD	EVCMD_LD_REG_ADR	,EvCmdLoadRegAdrs
	DEF_CMD	EVCMD_LD_ADR_REG	,EvCmdLoadAdrsReg
	DEF_CMD	EVCMD_LD_REG_REG	,EvCmdLoadRegReg
	DEF_CMD	EVCMD_LD_ADR_ADR	,EvCmdLoadAdrsAdrs
	DEF_CMD	EVCMD_LD_WK_VAL		,EvCmdLoadWkValue
	DEF_CMD	EVCMD_WK_ADD		,EvCmdWkAdd			@01/10/15 nohara
	DEF_CMD	EVCMD_WK_SUB		,EvCmdWkSub			@02/07/17 nohara
	DEF_CMD	EVCMD_LD_WK_WK		,EvCmdLoadWkWk
	DEF_CMD	EVCMD_LD_WK_WKVAL	,EvCmdLoadWkWkValue	@01/10/19 nohara

	DEF_CMD	EVCMD_CP_REG_REG	,EvCmdCmpRegReg
	DEF_CMD	EVCMD_CP_REG_VAL	,EvCmdCmpRegValue
	DEF_CMD	EVCMD_CP_REG_ADR	,EvCmdCmpRegAdrs
	DEF_CMD	EVCMD_CP_ADR_REG	,EvCmdCmpAdrsReg
	DEF_CMD	EVCMD_CP_ADR_VAL	,EvCmdCmpAdrsValue
	DEF_CMD	EVCMD_CP_ADR_ADR	,EvCmdCmpAdrsAdrs
	DEF_CMD	EVCMD_CP_WK_VAL		,EvCmdCmpWkValue
	DEF_CMD	EVCMD_CP_WK_WK		,EvCmdCmpWkWk

	DEF_CMD	EVCMD_SUBROUTINE	,EvCmdSubroutine				
	DEF_CMD	EVCMD_SETWAITSUB	,EvCmdSetWaitRoutine 
	DEF_CMD	EVCMD_PROGRAM		,EvCmdProgram
	DEF_CMD	EVCMD_FUNCCALL		,EvCmdFunctionCall
	DEF_CMD	EVCMD_STOPSCRIPT	,EvCmdStopScript
	DEF_CMD	EVCMD_TIMEWAIT		,EvCmdTimeWait

	DEF_CMD	EVCMD_FLAGSET		,EvCmdFlagSet
	DEF_CMD	EVCMD_FLAGRESET		,EvCmdFlagReset
	DEF_CMD	EVCMD_FLAGCHECK		,EvCmdFlagCheck

	DEF_CMD	EVCMD_SET_TIME		,EvCmdSetGameTime
	DEF_CMD	EVCMD_UPDATE_TIME	,EvCmdUpdateGameTime
	DEF_CMD	EVCMD_GET_TIME		,EvCmdGetGameTime
	DEF_CMD	EVCMD_SET_SE		,EvCmdSetSE
	DEF_CMD	EVCMD_SE_WAIT		,EvCmdSEWait
	DEF_CMD	EVCMD_SET_JINGLE	,EvCmdSetJingle
	DEF_CMD	EVCMD_JINGLE_WAIT	,EvCmdJingleWait
	DEF_CMD	EVCMD_BGM_SET		,EvCmdBGMSet
	DEF_CMD	EVCMD_BGM_SP_SET	,EvCmdBGMSpecialSet
	DEF_CMD	EVCMD_BGM_NOW_SET	,EvCmdBGMNowSet
	DEF_CMD	EVCMD_BGM_FADE_CHG	,EvCmdBGMFadeChg
	DEF_CMD	EVCMD_BGM_FADEOUT	,EvCmdBGMFadeOut
	DEF_CMD	EVCMD_BGM_FADEIN	,EvCmdBGMFadeIn
	DEF_CMD	EVCMD_MAP_CHANGE	,EvCmdMapChange
	DEF_CMD	EVCMD_MAP_CHG_SENOT	,EvCmdMapChangeSeNot
	DEF_CMD	EVCMD_DOORMAP_CHANGE,EvCmdDoorMapChange
	DEF_CMD EVCMD_MAP_CHG_FALL	,EvCmdMapChangeFall
	DEF_CMD EVCMD_MAP_CHG_WARP	,EvCmdMapChangeWarp
	DEF_CMD	EVCMD_SET_MAP_CHG	,EvCmdSetMapChangePos
	DEF_CMD EVCMD_SETSPMAPID	,EvCmdSetSPMapID
	DEF_CMD EVCMD_SETDIVINGID	,EvCmdSetDivingMapID
	DEF_CMD EVCMD_SETPITFALLID	,EvCmdSetPitfallMapID
	DEF_CMD	EVCMD_GET_MAPPOS	,EvCmdGetMapPosition
	DEF_CMD	EVCMD_GETTEMOTINUM	,EvCmdGetTemotiNum

	DEF_CMD	EVCMD_ADDITEM		,EvCmdAddItem
	DEF_CMD	EVCMD_SUBITEM		,EvCmdSubItem
	DEF_CMD	EVCMD_ADDITEMCHK	,EvCmdAddItemChk
	DEF_CMD	EVCMD_CHECKITEM		,EvCmdCheckItem
	DEF_CMD	EVCMD_GETPOCKETNO	,EvCmdGetPocketNo
	DEF_CMD	EVCMD_ADDBOXITEM	,EvCmdAddPCBoxItem
	DEF_CMD	EVCMD_CHECKBOXITEM	,EvCmdCheckPCBoxItem

	DEF_CMD	EVCMD_ADDGOODS		,EvCmdAddGoods
	DEF_CMD	EVCMD_SUBGOODS		,EvCmdSubGoods
	DEF_CMD	EVCMD_CHECKGOODS	,EvCmdCheckGoods
	DEF_CMD	EVCMD_ADDGOODSCHK	,EvCmdAddGoodsChk

	DEF_CMD	EVCMD_OBJANIME		,EvCmdObjAnime
	DEF_CMD	EVCMD_OBJANIMESP	,EvCmdObjAnimeSP
	DEF_CMD	EVCMD_OBJANIMEWAIT	,EvCmdObjAnimeWait
	DEF_CMD	EVCMD_OBJANIMEWAITSP,EvCmdObjAnimeWaitSP
	DEF_CMD	EVCMD_OBJDEL		,EvCmdObjDel
	DEF_CMD	EVCMD_OBJDELSP		,EvCmdObjDelSP
	DEF_CMD	EVCMD_OBJAPPEAR		,EvCmdObjAppear
	DEF_CMD	EVCMD_OBJAPPEARSP	,EvCmdObjAppearSP
	DEF_CMD	EVCMD_OBJ_POSCHG	,EvCmdObjPosChg
	DEF_CMD	EVCMD_OBJ_VISIBLE	,EvCmdObjSetVisible
	DEF_CMD	EVCMD_OBJ_INVISIBLE	,EvCmdObjSetInvisible
	DEF_CMD	EVCMD_OBJ_TO_HERO	,EvCmdObjSiteToHero
	DEF_CMD	EVCMD_OBJSITECHG	,EvCmdObjSiteChg

	DEF_CMD	EVCMD_TRAINERSET	,EvCmdTrainerDataSet
	DEF_CMD	EVCMD_TRAINERFIGHT	,EvCmdTrainerFightStart
	DEF_CMD	EVCMD_TRAINERAFTER	,EvCmdTrainerAfterScript
	DEF_CMD	EVCMD_TRN_END_SCRIPT,EvCmdTrainerBattleEndScript
	DEF_CMD	EVCMD_TRN_FLAG_CHK	,EvCmdTrainerFlagCheck
	DEF_CMD	EVCMD_TRN_FLAG_SET	,EvCmdTrainerFlagSet
	DEF_CMD	EVCMD_TRN_FLAG_RESET,EvCmdTrainerFlagReset

	DEF_CMD	EVCMD_SXYPOS_CHG	,EvCmdSxyPosChange
	DEF_CMD	EVCMD_SXYPOS_OWRITE	,EvCmdSxyPosOverWrite
	DEF_CMD	EVCMD_SXYSITE_CHG	,EvCmdSxySiteChange
	DEF_CMD	EVCMD_TALKWAIT		,EvCmdTalkWait
	DEF_CMD	EVCMD_TALKMSG		,EvCmdTalkMsg
	DEF_CMD	EVCMD_TALKCLOSE		,EvCmdTalkWinClose
	DEF_CMD	EVCMD_TALKSTART		,EvCmdTalkStart
	DEF_CMD	EVCMD_TALKOBJSTART	,EvCmdTalkObjStart
	DEF_CMD	EVCMD_TALKEND		,EvCmdTalkEnd
	DEF_CMD	EVCMD_TALKOBJEND	,EvCmdTalkObjEnd

	DEF_CMD	EVCMD_ABKEY_WAIT	,EvCmdABKeyWait
	DEF_CMD	EVCMD_YESNOWIN		,EvCmdYesNoWin
	DEF_CMD	EVCMD_SELECTWIN		,EvCmdSelectWin
	DEF_CMD	EVCMD_SELECTWINCSR	,EvCmdSelectWinCsr
	DEF_CMD	EVCMD_HVWIN			,EvCmdHVWin
	DEF_CMD	EVCMD_WINWRITE		,EvCmdWinWrite
	DEF_CMD	EVCMD_WINCLEAR		,EvCmdWinClear
	DEF_CMD	EVCMD_MENUWRITE		,EvCmdSelectMenu
	DEF_CMD	EVCMD_POKECGWRITE	,EvCmdPokeCGWinOpen
	DEF_CMD	EVCMD_POKECGERASE	,EvCmdPokeCGWinErase
	DEF_CMD	EVCMD_PUTPORTRAIT	,EvCmdPutPortrait
	DEF_CMD	EVCMD_TENJIMSGPUT	,EvCmdTenjiMsgPut

	DEF_CMD	EVCMD_ADDPOKE		,EvCmdAddPokemon
	DEF_CMD	EVCMD_ADDTAMAGO		,EvCmdAddTamago			// matsuda 2002.08.13(��)
	DEF_CMD	EVCMD_PM_WAZA_CHANGE,EvCmdPokeWazaChange
	DEF_CMD EVCMD_PM_WAZA_CHECK ,EvCmdPokeWazaCheck		// 2002/01/23 by nakahiro
	DEF_CMD	EVCMD_GETMONSNAME	,EvCmdGetMonsName
	DEF_CMD	EVCMD_GETTEMOTIMONSNAME,EvCmdGetTemotiMonsName
	DEF_CMD	EVCMD_GETNICKNAME	,EvCmdGetNickName
	DEF_CMD	EVCMD_GETITEMNAME	,EvCmdGetItemName
	DEF_CMD	EVCMD_GETGOODSNAME	,EvCmdGetGoodsName
	DEF_CMD	EVCMD_GETWAZANAME	,EvCmdGetWazaName
	DEF_CMD	EVCMD_GETNUMMSG		,EvCmdGetNumMsg
	DEF_CMD	EVCMD_SETBUF		,EvCmdSetBuf
	DEF_CMD	EVCMD_COPYBUF		,EvCmdCopyBuf

	DEF_CMD	EVCMD_SHOPSTART		,EvCmdShopStart
	DEF_CMD	EVCMD_GD_SHOPSTART	,EvCmdGoodsShopStart
	DEF_CMD	EVCMD_FL_SHOPSTART	,EvCmdFlowerShopStart
	DEF_CMD	EVCMD_SLOTMACHINE	,EvCmdSlotMachine
	DEF_CMD	EVCMD_SET_SEED		,EvCmdSetSeed

	DEF_CMD	EVCMD_CONPOKELIST	,EvCmdConPokeList
	DEF_CMD	EVCMD_CONNIJICALL	,EvCmdConNijiCall
	DEF_CMD	EVCMD_CONRESULTSCR	,EvCmdConResultScr
	DEF_CMD	EVCMD_CONTUUSININIT	,EvCmdConTuusinInit

	DEF_CMD	EVCMD_GETRND		,EvCmdGetRand
	DEF_CMD	EVCMD_ADDGOLD		,EvCmdAddGold
	DEF_CMD	EVCMD_SUBGOLD		,EvCmdSubGold
	DEF_CMD	EVCMD_COMPGOLD		,EvCmdCompGold
	DEF_CMD	EVCMD_GOLDWINWRITE	,EvCmdGoldWinWrite
	DEF_CMD	EVCMD_GOLDWINDEL	,EvCmdGoldWinDel
	DEF_CMD	EVCMD_GOLDWRITE		,EvCmdGoldWrite

	DEF_CMD	EVCMD_CMEVENTSTARTCHK,EvCmdCMEventStartChk

	DEF_CMD	EVCMD_FIELDFADE		,EvCmdFieldFade
	DEF_CMD	EVCMD_FIELDFADEWAIT	,EvCmdFieldFadeWait
	DEF_CMD	EVCMD_CHG_BRIGHT	,EvCmdChangeBrightValue
	DEF_CMD	EVCMD_BRIGHT_EFF	,EvCmdChangeBrightCircle

	DEF_CMD	EVCMD_SYSTALKMSG	,EvCmdSysTalkMsg

	DEF_CMD	EVCMD_SETFLDEFF		,EvCmdSetFldEff
	DEF_CMD	EVCMD_SETFEWORK		,EvCmdSetFEWork		// 2002/01/24 by nakahiro
	DEF_CMD	EVCMD_FLDEFFWAIT	,EvCmdFldEffWait

	DEF_CMD	EVCMD_SETWARPID		,EvCmdSetWarpID

	DEF_CMD	EVCMD_GETMYSEX		,EvCmdGetMySex
	DEF_CMD	EVCMD_VOICEPLAY		,EvCmdVoicePlay

	DEF_CMD	EVCMD_SETCELL		,EvCmdSetCell

	DEF_CMD	EVCMD_INIT_WEATHER	,EvCmdInitWeather
	DEF_CMD	EVCMD_SET_WEATHER	,EvCmdSetWeather
	DEF_CMD	EVCMD_UPDATE_WEATHER,EvCmdUpdateWeather
	DEF_CMD EVCMD_SET_MAPEFFECT	,EvCmdSetMapEffect
	DEF_CMD	EVCMD_CHG_MAPSCREEN	,EvCmdChangeMapScreen

	DEF_CMD	EVCMD_OBJPRICHG		,EvCmdObjPriChg
	DEF_CMD	EVCMD_OBJPRIOFF		,EvCmdObjPriOff

	DEF_CMD	EVCMD_OBJDMYACTADD	,EvCmdObjDmyActAdd
	DEF_CMD	EVCMD_OBJDMYSITECHG	,EvCmdObjDmySiteChg

	DEF_CMD	EVCMD_OPENDOORANIME	,EvCmdOpenDoorAnime
	DEF_CMD	EVCMD_CLOSEDOORANIME,EvCmdCloseDoorAnime
	DEF_CMD	EVCMD_DOORANIMEWAIT	,EvCmdDoorAnimeWait
	DEF_CMD	EVCMD_OPENDOORSET	,EvCmdOpenDoorSet
	DEF_CMD	EVCMD_CLOSEDOORSET	,EvCmdCloseDoorSet

	DEF_CMD	EVCMD_ELEVATORSET	,EvCmdElevatorSet
	DEF_CMD	EVCMD_ELEVATORSTART	,EvCmdElevatorStart

	DEF_CMD	EVCMD_CHECKCOIN		,EvCmdCheckCoin		@ 2002/07/03 by nakahiro
	DEF_CMD	EVCMD_ADDCOIN		,EvCmdAddCoin
	DEF_CMD	EVCMD_SUBCOIN		,EvCmdSubCoin

	DEF_CMD	EVCMD_SET_WILDFIGHT	,EvCmdWildFightSet
	DEF_CMD	EVCMD_START_WILDFIGHT, EvCmdWildFightStart

	DEF_CMD	EVCMD_SET_RELADRS	,EvCmdSetRelativeAdrs
	DEF_CMD	EVCMD_JUMPR			,EvCmdJumpRelative
	DEF_CMD	EVCMD_CALLR			,EvCmdCallRelative
	DEF_CMD	EVCMD_IFJUMPR		,EvCmdIfJumpRelative
	DEF_CMD	EVCMD_IFCALLR		,EvCmdIfCallRelative
	DEF_CMD	EVCMD_TALKMSG_REL	,EvCmdTalkMsgRelative
	DEF_CMD EVCMD_EXPANDMSG_REL	,EvCmdExpandMsgRelative
	DEF_CMD	EVCMD_COPYBUF_REL	,EvCmdCopyBufRelative

	DEF_CMD	EVCMD_COINWINWRITE  ,EvCmdCoinWinWrite
	DEF_CMD	EVCMD_COINWINDEL	,EvCmdCoinWinDel
	DEF_CMD	EVCMD_COINWRITE		,EvCmdCoinWrite
	DEF_CMD	EVCMD_INC_RECORD	,EvCmdIncRecord

	DEF_CMD EVCMD_SET_ESCMAPID	,EvCmdSetEscapeMapID

	DEF_CMD	EVCMD_VOICE_WAIT	,EvCmdVoicePlayWait

	DEF_CMD_LABEL		ScriptCommandEnd

	DEF_CMD	EVCMD_NOTHING		,EvCmdNop




@===================================================================
@
@===================================================================
@----------------------------------------------------------
@	�C�x���g���x���̐錾
@----------------------------------------------------------
.MACRO	EVENT_LABEL	label
	.GLOBAL	\label
\label:
.ENDM

@----------------------------------------------------------
@	�X�N���v�g�J�n�錾
@----------------------------------------------------------
@---------------------------------------------
@---------------------------------------------
@�X�N���v�g�w�b�_�f�[�^�L�q
.MACRO	EVENT_DATA_START	label,flag
	EVENT_LABEL	\label
	.short	\flag
.ENDM

@�X�N���v�g�w�b�_�f�[�^�I��
.MACRO	EVENT_DATA_END
	.byte	HEAD_NO_WORK
.ENDM

@---------------------------------------------
@	����X�N���v�g�L�q�F�a�f��������
@	�����z�}�b�v�\�z�̃^�C�~���O�ŌĂ΂�Ă���
@---------------------------------------------
.MACRO	BG_CHANGE_LABEL	script
	.byte	HEAD_BG_CHANGE
	.word	\script
.ENDM

@---------------------------------------------
@	����X�N���v�g�L�q�F�V�[���X�N���v�g�N��
@	���펞�������Ď����Ă���B
@---------------------------------------------
.MACRO	SP_SCRIPT_LABEL	adr
	.byte	HEAD_SCENE_CHK
	.word	\adr
.ENDM

@�V�[���X�N���v�g�N�������L�q
@ wk == num �̏ꍇ�Alabel�̃X�N���v�g���N������
.MACRO	SP_SCRIPT_DATA	wk,num,label
	.short	\wk
	.short	\num
	.word	\label
.ENDM

@�V�[���X�N���v�g�L�q�I��
.MACRO	SP_SCRIPT_END
	.short	0
.ENDM

@---------------------------------------------
@	����X�N���v�g�L�q�F�t���O����
@	���}�b�v�J�ڃ^�C�~���O�ň�x�����Ă΂��
@---------------------------------------------
.MACRO	FLAG_CHANGE_LABEL	script
	.byte	HEAD_FLAG_CTRL
	.word	\script
.ENDM

@---------------------------------------------
@	����X�N���v�g�L�q�F�t���O����
@	���t�B�[���h���������ɖ���Ă΂��
@---------------------------------------------
.MACRO	INIT_CTRL_LABEL	script
	.byte	HEAD_INIT_CTRL
	.word	\script
.ENDM

@---------------------------------------------
@	����X�N���v�g�L�q�F�����N����
@	����A�_�C�r���O�ڑ��̓��ꐧ��݂̂Ŏg�p
@---------------------------------------------
.MACRO	LINK_CTRL_LABEL	script
	.byte	HEAD_LINK_CTRL
	.word	\script
.ENDM

@---------------------------------------------
@	����X�N���v�g�L�q�F�n�a�i����
@	���}�b�v�J�ڃ^�C�~���O�ň�x�����Ă΂��
@---------------------------------------------
.MACRO	OBJ_CTRL_LABEL	adr
	.byte	HEAD_OBJ_CTRL
	.word	\adr
.ENDM

@�n�a�i����X�N���v�g�N�������L�q
@ wk == num �̏ꍇ�Alabel�̃X�N���v�g���N������
.MACRO	OBJ_CTRL_DATA	wk,num,label
	.short	\wk
	.short	\num
	.word	\label
.ENDM

@�n�a�i����X�N���v�g�L�q�I��
.MACRO	OBJ_CTRL_END
	.short	0
.ENDM


@----------------------------------------------------------
@	�X�N���v�g�p���x���錾
@----------------------------------------------------------
.MACRO	SCRIPT_LABEL	label
	EVENT_LABEL \label
.ENDM

@----------------------------------------------------------
@	�B���A�C�e���̃t���O�錾
@----------------------------------------------------------
.MACRO	HIDE_ITEM_DATA	flag,item,num
	.short	\flag
	.short	\item,\num
.ENDM

@---------------------------------------------
@
@---------------------------------------------
@===================================================================
@
@	�X�N���v�g�R�}���h�̒�`
@
@===================================================================

@----------------------------------------------------------
@ ��{�R�}���h
@----------------------------------------------------------
.MACRO	_NOP
	.byte	EVCMD_NOP
.ENDM

.MACRO	_DUMMY
	.byte	EVCMD_DUMMY
.ENDM

.MACRO	_END
	.byte	EVCMD_END
.ENDM

@----------------------------------------------------------
@ ��{����R�}���h
@----------------------------------------------------------
.MACRO	_RET
	.byte	EVCMD_RET
.ENDM

.MACRO	_GLOBAL_CALL	ADDRESS
	.byte	EVCMD_GLOBAL_CALL
	.word	\ADDRESS
.ENDM

.MACRO	_GLOBAL_JUMP	ADDRESS
	.byte	EVCMD_GLOBAL_JUMP
	.word	\ADDRESS
.ENDM

@ �X�N���v�g�Ăяo��
.MACRO	_CALL	ADDRESS
	_GLOBAL_CALL	\ADDRESS
.ENDM

@ �W�����v
.MACRO	_JUMP	ADDRESS
	_GLOBAL_JUMP	\ADDRESS
.ENDM

@---------------------------------------------
@	�O���w�肵���X�N���v�g�ɕ���
@---------------------------------------------
.MACRO	_CHANGE_SCRIPT
	.byte	EVCMD_CHG_SCRIPT
.ENDM

@---------------------------------------------
@	�O���X�N���v�g�̃N���A���I��
@---------------------------------------------
.MACRO	_CLEAR_EX_SCRIPT
	.byte	EVCMD_CLR_EXSCRIPT
.ENDM
@---------------------------------------------
@	�O���X�N���v�g�̃G���[�R�[�h�Z�b�g
@---------------------------------------------
.MACRO	_SET_EXDATA_ERROR	error
	.byte	EVCMD_SET_EXERROR
	.byte	\error
.ENDM

@----------------------------------------------------------
@
@	�X�N���v�g����v���O�������ĂԃR�}���h
@
@----------------------------------------------------------
@---------------------------------------------
@ ID �ɂ��o�^�v���O�����̌Ăяo��
@---------------------------------------------
.MACRO	_PROGRAM	id
	.byte	EVCMD_PROGRAM
	.short	\id
.ENDM

@---------------------------------------------
@ ID �ɂ��o�^�v���O�����̌Ăяo��
@ ��wk�Ŏw�肵�����[�N��u8�̖߂�l������
@---------------------------------------------
.MACRO	_FUNC_CALL	wk,id
	.byte	EVCMD_FUNCCALL
	.short	\wk,\id
.ENDM

@---------------------------------------------
@�X�N���v�g�̒�~
@���ʏ�̓^�X�N�Ȃǂɐ����n�����߂Ɏg�p����
@---------------------------------------------
.MACRO	_STOP_SCRIPT
	.byte	EVCMD_STOPSCRIPT
.ENDM

@----------------------------------------------------------
@
@		���[�N�̑���
@
@----------------------------------------------------------
@ ���[�N�ɒl����
.MACRO	_LDVAL	wk,val
	.byte	EVCMD_LD_WK_VAL
	.short	\wk
	.short	\val
.ENDM

@ ���[�N�ɒl�𑫂�
.MACRO	_ADDWK	wk,add
	.byte	EVCMD_WK_ADD
	.short	\wk
	.short	\add
.ENDM

@ ���[�N����l������
.MACRO	_SUBWK	wk,sub
	.byte	EVCMD_WK_SUB
	.short	\wk
	.short	\sub
.ENDM

@ ���[�N�Ƀ��[�N�̒l����
.MACRO	_LDWK	wk1,wk2
	.byte	EVCMD_LD_WK_WK
	.short	\wk1
	.short	\wk2
.ENDM

@ ���[�N�ɒl�����[�N�̒l����
.MACRO	_LDWKVAL	wk1,wk2
	.byte	EVCMD_LD_WK_WKVAL
	.short	\wk1
	.short	\wk2
.ENDM

@ ���[�N�ƒl�̔�r
.MACRO	_CMPVAL	wk,val
	.byte	EVCMD_CP_WK_VAL
	.short	\wk
	.short	\val
.ENDM

@ ���[�N�ƃ��[�N�̔�r
.MACRO	_CMPWK	wk1,wk2
	.byte	EVCMD_CP_WK_WK
	.short	\wk1
	.short	\wk2
.ENDM

@ �����̎擾(0�`�w�肵���l-1�܂ł̗������擾����AnswerWork�ɓ����)
.MACRO	_GET_RND	limit
	.byte	EVCMD_GETRND
	.short	\limit					@short�ł悢�H
.ENDM


@----------------------------------------------------------
@ �����t������R�}���h
@----------------------------------------------------------
@ �����ɂ���ĕ���
.MACRO	_IF_JUMP	cond,adr
	.byte	EVCMD_IFJUMP
	.byte	\cond
	.word	\adr
.ENDM

@ �����ɂ���ĕ���(CALL)							02/03/05	nohara	
.MACRO	_IF_CALL	cond,adr
	.byte	EVCMD_IFCALL
	.byte	\cond
	.word	\adr
.ENDM

@ ���[�N�ƒl���r�A�����ǂ���̏ꍇ����
.MACRO	_IFVAL_JUMP	wk,cond,val,adr
	_CMPVAL	\wk,\val
	.byte	EVCMD_IFJUMP
	.byte	\cond
	.word	\adr
.ENDM

@ ���[�N�ƒl���r�A�����ǂ���̏ꍇ����(CALL)		02/03/04	nohara
.MACRO	_IFVAL_CALL	wk,cond,val,adr
	_CMPVAL	\wk,\val
	.byte	EVCMD_IFCALL
	.byte	\cond
	.word	\adr
.ENDM

@ ���[�N�ƃ��[�N���r�A�����ǂ���̏ꍇ����
.MACRO	_IFWK_JUMP	wk1,cond,wk2,adr
	_CMPWK	\wk1,\wk2
	.byte	EVCMD_IFJUMP
	.byte	\cond
	.word	\adr
.ENDM

@ ���[�N�ƃ��[�N���r�A�����ǂ���̏ꍇ����(CALL)	02/03/04	nohara
.MACRO	_IFWK_CALL	wk1,cond,wk2,adr
	_CMPWK	\wk1,\wk2
	.byte	EVCMD_IFCALL
	.byte	\cond
	.word	\adr
.ENDM

@ ANSWORK��0�̏ꍇ�W�����v
.MACRO	_IF_ZERO_JUMP	adr
	_CMPVAL	ANSWORK,0
	.byte	EVCMD_IFJUMP
	.byte	EQ
	.word	\adr
.ENDM

@----------------------------------------------------------
@	�h�c�ɂ�鋤�ʃX�N���v�g�ւ̕���
@----------------------------------------------------------
.MACRO	_JUMPID		id
	.byte	EVCMD_JUMPID
	.byte	\id
.ENDM

.MACRO	_CALLID		id
	.byte	EVCMD_CALLID
	.byte	\id
.ENDM

.MACRO	_IFJUMPID	cond,id
	.byte	EVCMD_IFJUMPID
	.byte	\cond
	.byte	\id
.ENDM

.MACRO	_IFCALLID	cond,id
	.byte	EVCMD_IFCALLID
	.byte	\cond
	.byte	\id
.ENDM

.MACRO	_IFVAL_JUMPID	wk,cond,val,id
	_CMPVAL	\wk,\val
	_IFJUMPID	\cond,\id
.ENDM

.MACRO	_IFVAL_CALLID	wk,cond,val,id
	_CMPVAL	\wk,\val
	_IFCALLID	\cond,\id
.ENDM

.MACRO	_IFWK_JUMPID	wk,cond,val,id
	_CMPWK	\wk,\val
	_IFJUMPID	\cond,\id
.ENDM

.MACRO	_IFWK_CALLID	wk,cond,val,id
	_CMPWK	\wk,\val
	_IFCALLID	\cond,\id
.ENDM

@----------------------------------------------------------
@	switch �` case �\��
@----------------------------------------------------------
.MACRO	_SWITCH	wk
	_LDWK	RG0,\wk
.ENDM

.MACRO	_CASEJUMP	val,adr
	_CMPVAL	RG0,\val
	_IF_JUMP	EQ,\adr
.ENDM

@�X�N���v�g�E�B���h�E�\������B�L�����Z��
@ev_win.c��define ANS_CANCEL 127������(ANSWORK�ɓ���)
.MACRO	_CASECANCEL	adr
	_CMPVAL	RG0,127
	_IF_JUMP	EQ,\adr
.ENDM


@----------------------------------------------------------
@ �t���O�֘A
@----------------------------------------------------------
@ �t���O�Z�b�g
.MACRO	_FLAG_SET	flag
	.byte	EVCMD_FLAGSET
	.short	\flag
.ENDM

@ �t���O���Z�b�g
.MACRO	_FLAG_RESET	flag
	.byte	EVCMD_FLAGRESET
	.short	\flag
.ENDM

@ �t���O��ON�̂Ƃ�����
.MACRO	_IF_FLAGON_JUMP	flag,adr
	.byte	EVCMD_FLAGCHECK
	.short	\flag
	_IF_JUMP FLGON ,\adr
.ENDM

@ �t���O��ON�̂Ƃ�����(CALL)				02/03/05	nohara
.MACRO	_IF_FLAGON_CALL	flag,adr
	.byte	EVCMD_FLAGCHECK
	.short	\flag
	_IF_CALL FLGON ,\adr
.ENDM

@ �t���O��OFF�̂Ƃ�����
.MACRO	_IF_FLAGOFF_JUMP	flag,adr
	.byte	EVCMD_FLAGCHECK
	.short	\flag
	_IF_JUMP FLGOFF ,\adr
.ENDM

@ �t���O��OFF�̂Ƃ�����(CALL)				02/03/05	nohara
.MACRO	_IF_FLAGOFF_CALL	flag,adr
	.byte	EVCMD_FLAGCHECK
	.short	\flag
	_IF_CALL FLGOFF ,\adr
.ENDM

@

@----------------------------------------------------------
@	�V�󐧌�֘A
@----------------------------------------------------------
@---------------------------------------------
@	�V��R�[�h�̏�����
@---------------------------------------------
.MACRO	_INIT_WEATHER
	.byte	EVCMD_INIT_WEATHER
.ENDM

@---------------------------------------------
@	�V��R�[�h�̃Z�b�g
@---------------------------------------------
.MACRO	_SET_WEATHER	weather_id
	.byte	EVCMD_SET_WEATHER
	.short	\weather_id
.ENDM

@---------------------------------------------
@	�V��R�[�h�̔��f
@---------------------------------------------
.MACRO	_UPDATE_WEATHER
	.byte	EVCMD_UPDATE_WEATHER
.ENDM

@----------------------------------------------------------
@----------------------------------------------------------
@---------------------------------------------
@	�}�b�v�ŗL�̃G�t�F�N�g��ݒ�
@---------------------------------------------
.MACRO	_SET_MAP_EFFECT	id
	.byte	EVCMD_SET_MAPEFFECT
	.byte	\id
.ENDM

@---------------------------------------------
@	�}�b�v�X�N���[���̐؂�ւ�
@	��FLAG_CHANGE_LABEL�̃^�C�~���O�ŌĂԂ���
@---------------------------------------------
.MACRO	_CHANGE_MAP_SCREEN	screen_id
	.byte	EVCMD_CHG_MAPSCREEN
	.short	\screen_id
.ENDM

@----------------------------------------------------------
@	���Ԑ���֘A
@----------------------------------------------------------
@---------------------------------------------
@	�Q�[�������Ԃ̃Z�b�g
@---------------------------------------------
.MACRO	_SET_GAME_TIME	hour,min
	.byte	EVCMD_SET_TIME
	.short	\hour
	.short	\min
.ENDM

@---------------------------------------------
@	�Q�[�������ԁ��֘A�t���O�X�V
@---------------------------------------------
.MACRO	_UPDATE_GAME_TIME
	.byte	EVCMD_UPDATE_TIME
.ENDM

@---------------------------------------------
@	�Q�[�������Ԃ��擾����(RG0,RG1,RG2�ɑ������)
@---------------------------------------------
.MACRO	_GET_GAME_TIME
	.byte	EVCMD_GET_TIME
.ENDM


@----------------------------------------------------------
@	���y�E�r�d�֘A
@----------------------------------------------------------
.MACRO	_SE	se_no
	.byte	EVCMD_SET_SE
	.short	\se_no
.ENDM

.MACRO	_JINGLE	mus_no
	.byte	EVCMD_SET_JINGLE
	.short	\mus_no
.ENDM

.MACRO	_JINGLE_WAIT
	.byte	EVCMD_JINGLE_WAIT
.ENDM

.MACRO	_BGM_SET	bgm_no, flag=0
	.byte	EVCMD_BGM_SET
	.short	\bgm_no
	.byte	\flag
.ENDM

.MACRO	_BGM_SPECIAL_SET	bgm_no
	.byte	EVCMD_BGM_SP_SET
	.short	\bgm_no
.ENDM

.MACRO	_BGM_SPECIAL_CLR	bgm_no
	.byte	EVCMD_BGM_SP_SET
	.short	0
.ENDM

.MACRO	_BGM_NOW_SET
	.byte	EVCMD_BGM_NOW_SET
.ENDM

.MACRO	_BGM_FADE_CHG		bgm_no
	.byte	EVCMD_BGM_FADE_CHG
	.short	\bgm_no
.ENDM

.MACRO	_BGM_FADEOUT	sec=0
	.byte	EVCMD_BGM_FADEOUT
	.byte	\sec
.ENDM

.MACRO	_BGM_FADEIN		sec=0
	.byte	EVCMD_BGM_FADEIN
	.byte	\sec
.ENDM

@----------------------------------------------------------
@	�E�F�C�g�i�P�^�U�O�P�ʁj
@----------------------------------------------------------
.MACRO	_TIME_WAIT	cnt
	.byte	EVCMD_TIMEWAIT
	.short	\cnt
.ENDM

@----------------------------------------------------------
@�@����֘A
@----------------------------------------------------------
.MACRO	_ADD_ITEM	item,num
	.byte	EVCMD_ADDITEM
	.short	\item
	.short	\num
.ENDM

.MACRO	_SUB_ITEM	item,num
	.byte	EVCMD_SUBITEM
	.short	\item
	.short	\num
.ENDM

@�A�C�e���������邩�̃`�F�b�N
.MACRO	_ADD_ITEM_CHK	item,num
	.byte	EVCMD_ADDITEMCHK
	.short	\item
	.short	\num
.ENDM

@�A�C�e���������Ă��邩�̃`�F�b�N(�o�b�N)
.MACRO	_CHECK_ITEM	item,num
	.byte	EVCMD_CHECKITEM
	.short	\item
	.short	\num
.ENDM

@ �|�P�b�g�̔ԍ����擾
.MACRO	_GET_POCKET_NO	item
	.byte	EVCMD_GETPOCKETNO
	.short	\item
.ENDM

@ �A�C�e���擾���b�Z�[�W
@.MACRO	_ADD_ITEM_MSG
@	_CALL	ev_itemget_p01
@.ENDM

@ �A�C�e���擾���s���b�Z�[�W

@ �A�C�e���擾���ʃC�x���g
@ _LDWKVAL�Œl�n���Ă����[�N�n���Ă������悤�ɂ��Ă���
.MACRO	_ITEM_EVENT	no,num
	_LDWKVAL	RG0,\no
	_LDWKVAL	RG1,\num
	_CALLID	SCRID_ITEM_EVENT
@	_CALL	ev_item_event
.ENDM

@�t�B�[���h��̃A�C�e���擾����
.MACRO	_FLD_ITEM_EVENT	no,num
	_LDWKVAL	RG0,\no
	_LDWKVAL	RG1,\num
	_CALLID	SCRID_FLD_ITEM_EVENT
@	_CALL		ev_fld_item_event
.ENDM

@---------------------------------------------
//	�p�\�R���ɂǂ���������
@---------------------------------------------
.MACRO	_ADD_BOX_ITEM	item,num
	.byte	EVCMD_ADDBOXITEM
	.short	\item
	.short	\num
.ENDM

@---------------------------------------------
//	�p�\�R���̂ǂ����`�F�b�N
@---------------------------------------------
.MACRO	_CHECK_BOX_ITEM	item,num
	.byte	EVCMD_CHECKBOXITEM
	.short	\item
	.short	\num
.ENDM

@----------------------------------------------------------
@�@�O�b�Y�֘A
@----------------------------------------------------------
.MACRO	_ADD_GOODS	goods
	.byte	EVCMD_ADDGOODS
	.short	\goods
.ENDM

.MACRO	_SUB_GOODS	goods
	.byte	EVCMD_SUBGOODS
	.short	\goods
.ENDM

@�O�b�Y���������邩�̃`�F�b�N
.MACRO	_ADD_GOODS_CHK	goods
	.byte	EVCMD_ADDGOODSCHK
	.short	\goods
.ENDM

@�O�b�Y�������Ă��邩�̃`�F�b�N
.MACRO	_CHECK_GOODS	goods
	.byte	EVCMD_CHECKGOODS
	.short	\goods
.ENDM

@ �O�b�Y�擾���ʃC�x���g
@ _LDWKVAL�Œl�n���Ă����[�N�n���Ă������悤�ɂ��Ă���
.MACRO	_GOODS_EVENT	wk0
	_LDWKVAL	RG0,\wk0
	_CALLID	SCRID_GOODS_EVENT
.ENDM

@----------------------------------------------------------
@	�n�a�i�A�j������֘A
@----------------------------------------------------------
@ SP�Ɠ��ɂ��Ă���R�}���h�̓}�b�v�Ԃ���l�����܂�����
@ ������}�b�v�ł͂Ȃ��Ƃ����OBJ�̑�������鎞�Ɏg�p����

@ OBJ�A�j���Z�b�g
@	id = OBJ�w��
@	adrs = �A�j���f�[�^�̃��x��
.MACRO	_SET_OBJ_ANIME	id,adrs
	.byte	EVCMD_OBJANIME
	@.byte	\id
	.short	\id
	.word	\adrs
.ENDM

@ OBJ�A�j���Z�b�g(div,mapID���w�肷��)
@	id = OBJ�w��
@	adrs = �A�j���f�[�^�̃��x��
.MACRO	_SP_SET_OBJ_ANIME	id,adrs,div,map
	.byte	EVCMD_OBJANIMESP
	.short	\id
	.word	\adrs
	.byte	\div,\map
.ENDM

@ OBJ�A�j���I���҂�
@	id = OBJ�w��,�w��Ȃ��̏ꍇ��
@			�Ō�ɃA�j�����Z�b�g����OBJ
.MACRO	_OBJ_ANIME_WAIT	id = 0
	.byte	EVCMD_OBJANIMEWAIT
	@.byte	\id
	.short	\id
.ENDM

@ OBJ�A�j���I���҂�(div,mapID���w�肷��)
@	id = OBJ�w��,�w��Ȃ��̏ꍇ��
@			�Ō�ɃA�j�����Z�b�g����OBJ
.MACRO	_SP_OBJ_ANIME_WAIT	id,div,map
	.byte	EVCMD_OBJANIMEWAITSP
	.short	\id
	.byte	\div,\map
.ENDM

@ OBJ����
.MACRO	_OBJ_DELETE	id
	.byte	EVCMD_OBJDEL
	.short	\id
.ENDM

@ OBJ����(div,mapID���w�肷��)
.MACRO	_SP_OBJ_DELETE	id,div,map
	.byte	EVCMD_OBJDELSP
	.short	\id
	.byte	\div,\map
.ENDM

@ OBJ�o��
.MACRO	_OBJ_APPEAR	id
	.byte	EVCMD_OBJAPPEAR
	@.byte	\id
	.short	\id
.ENDM

@ OBJ�o��(div,mapID���w�肷��)
.MACRO	_SP_OBJ_APPEAR	id,div,map
	.byte	EVCMD_OBJAPPEARSP
	.short	\id
	.byte	\div,\map
.ENDM

@ OBJ�\���ʒu�ύX
.MACRO	_OBJ_POS_CHANGE	id,x,y
	.byte	EVCMD_OBJ_POSCHG
	.short \id,\x,\y
.ENDM

@ �\���t���O�̐���(������悤�ɂ���j
.MACRO	_OBJ_VISIBLE	id,div,map
	.byte	EVCMD_OBJ_VISIBLE
	.short	\id
	.byte	\div,\map
.ENDM

@ �\���t���O�̐���(�����Ȃ��悤�ɂ���j
.MACRO	_OBJ_INVISIBLE	id,div,map
	.byte	EVCMD_OBJ_INVISIBLE
	.short	\id
	.byte	\div,\map
.ENDM

@SXY�f�[�^�̈ʒu�����݂̈ʒu�ŏ㏑������
.MACRO	_SXY_POS_OVERWRITE		id
	.byte	EVCMD_SXYPOS_OWRITE
	.short	\id
.ENDM

@SXY�f�[�^�̈ʒu��������������
.MACRO	_SXY_POS_CHANGE	id,x,y
	.byte	EVCMD_SXYPOS_CHG
	.short	\id,\x,\y
.ENDM

@ OBJ�̌�����ύX
.MACRO	_OBJ_SITE_CHG	id,site
	.byte	EVCMD_OBJSITECHG
	.short	\id
	.byte	\site
.ENDM

@SXY�f�[�^�̈ړ�����������������
.MACRO	_SXY_SITE_CHANGE	id,site
	.byte	EVCMD_SXYSITE_CHG
	@.short	\id,\site
	.short	\id
	.byte	\site
.ENDM

@----------------------------------------------------------
@	�n�a�i�A�j������֘A
@----------------------------------------------------------
@
@ OBJ�̃v���C�I���e�B�ύX
.MACRO	_OBJ_PRI_CHG	id,div,map,pri
	.byte	EVCMD_OBJPRICHG
	.short	\id
	.byte	\div,\map,\pri
.ENDM

@ OBJ�̃v���C�I���e�B��������ʒu�ɏC������(�Œ�t���O���N���A����)
.MACRO	_OBJ_PRI_OFF	id,div,map
	.byte	EVCMD_OBJPRIOFF
	.short	\id
	.byte	\div,\map
.ENDM

@----------------------------------------------------------
@	�t�B�[���h�n�a�i�_�~�[�A�N�^�[�ǉ�
@----------------------------------------------------------
@
@ �ǉ�
.MACRO	_DMY_OBJ_ADD	code,id,x,y,h,site
	.byte	EVCMD_OBJDMYACTADD
	.byte	\code,\id
	.short	\x,\y
	.byte	\h,\site
.ENDM

@ �����ύX
.MACRO	_DMY_OBJ_SITE_CHG	id,site
	.byte	EVCMD_OBJDMYSITECHG
	.byte	\id,\site
.ENDM

@----------------------------------------------------------
@	 ��b����֘A
@----------------------------------------------------------

@---------------------------------------------
@��b�J�n�i���@����I���҂��A���ׂĂ̂n�a�i�̐Î~�Z�b�g�j
@---------------------------------------------
.MACRO	_TALK_START
	.byte	EVCMD_TALKSTART
.ENDM

@---------------------------------------------
@��b�I���i��b�E�B���h�E�����A���ׂĂ̂n�a�i�̐Î~�����j
@---------------------------------------------
.MACRO	_TALK_END
	.byte	EVCMD_TALKEND
.ENDM

@---------------------------------------------
@�n�a�i�b�������p��b�J�n
@---------------------------------------------
.MACRO	_TALK_OBJ_START
	.byte	EVCMD_TALKOBJSTART
	.byte	EVCMD_OBJ_TO_HERO
.ENDM

@---------------------------------------------
@�n�a�i�b�������p��b�I��
@---------------------------------------------
.MACRO	_TALK_OBJ_END
	.byte	EVCMD_TALKOBJEND
.ENDM

@---------------------------------------------
@���C�����؂�ւ�������ƁA�X�N���v�g���A���ɌĂ�
@���@����I���҂��A���ׂĂ̂n�a�i�̐Î~�Z�b�g���Z�b�g����
@�K�v�ȏ�����ǉ����Ă���
@---------------------------------------------
.MACRO	_TALK_OBJ_RECOVER
	_TALK_OBJ_START
.ENDM

.MACRO	_TALK_RECOVER
	_TALK_START
.ENDM

@---------------------------------------------
@ ���b�Z�[�W�\������т���
@---------------------------------------------
.MACRO	_TALKMSG	ADDRESS
	.byte	EVCMD_TALKMSG
	.word	\ADDRESS
.ENDM

@---------------------------------------------
@ ���b�Z�[�W����֎~�̃��b�Z�[�W�\���Z�b�g
@---------------------------------------------
.MACRO	_SYSTALKMSG	ADDRESS
	.byte	EVCMD_SYSTALKMSG
	.word	\ADDRESS
.ENDM

@---------------------------------------------
@ ���b�Z�[�W�\���I���҂����Z�b�g
@---------------------------------------------
.MACRO	_TALK_WAIT
	.byte	EVCMD_TALKWAIT
.ENDM

@---------------------------------------------
@ ���b�Z�[�W�E�B���h�E�����
@---------------------------------------------
.MACRO	_TALK_CLOSE
	.byte	EVCMD_TALKCLOSE
.ENDM

@---------------------------------------------
@ �ȈՃ��b�Z�[�W�Ăяo���n�a�i�p
@ ��l���̌����ɍ��킹��
@---------------------------------------------
.MACRO	_EASY_OBJ_MSG	msg
	_LD_REG_WDATA	0,\msg
	_CALLID	SCRID_EASYOBJMSGSCRIPT
@	_CALL	EasyObjMessageScript
.ENDM

@---------------------------------------------
@ �ȈՃ��b�Z�[�W�Ăяo���a�f�p
@ �a�f�E�b���|������n�܂�Ȃ�����(�����C�x���g�Ȃ�)
@---------------------------------------------
.MACRO	_EASY_MSG	msg
	_LD_REG_WDATA	0,\msg
	_CALLID	SCRID_EASYBGMSGSCRIPT
@	_CALL	EasyBgMessageScript
.ENDM

@---------------------------------------------
@ �E�B���h�E��������Ȃ��Ƃ��p(_TALKMSG,_TALK_WAIT,_AB_KEYWAIT)
@---------------------------------------------
.MACRO	_TALK_KEYWAIT	msg
	_LD_REG_WDATA	0,\msg
	_CALLID	SCRID_MSGWAITKEYSCRIPT
@	_CALL	MessageWaitKeyScript
.ENDM

@---------------------------------------------
@ AB�L�[�҂�
@---------------------------------------------
.MACRO	_AB_KEYWAIT
	.byte	EVCMD_ABKEY_WAIT
.ENDM

@---------------------------------------------
@�@�u�͂��E�������v�E�B���h�E
@---------------------------------------------
.MACRO	_YES_NO_WINDOW	xpos,ypos
	.byte	EVCMD_YESNOWIN
	.byte	\xpos,\ypos
.ENDM

@---------------------------------------------
@ �u�͂��E�������v�m�[�}���E�B���h�E
@---------------------------------------------
.MACRO	_YES_NO_NORMAL_WIN
	@_YES_NO_WINDOW	3,3			@����
	_YES_NO_WINDOW	20,8		@�E��
.ENDM

@---------------------------------------------
@���b�Z�[�W�\�����āA�u�͂��E�������v��\������
@---------------------------------------------
.MACRO	_TALK_YES_NO	msg
	_LD_REG_WDATA	0,\msg
	_CALLID	SCRID_MSGYESNOSCRIPT
@	_CALL	MessageYesNoScript
.ENDM

@---------------------------------------------
@�@�X�N���v�g�E�B���h�E
@---------------------------------------------
@.MACRO	_SELECT_WINDOW	xpos,ypos,xpos2,ypos2,mojino,mojimax
@	.byte	EVCMD_SELECTWIN
@	.byte	\xpos,\ypos,\xpos2,\ypos2,\mojino,\mojimax
@.ENDM

@---------------------------------------------
@�@�X�N���v�g�E�B���h�E
@ check = B�L�����Z����L���ɂ��邩,�w��Ȃ�(0)�̏ꍇ�͗L��
@---------------------------------------------
.MACRO	_SELECT_WINDOW	xpos,ypos,mojino,check = 0
	.byte	EVCMD_SELECTWIN
	.byte	\xpos,\ypos,\mojino
	.byte	\check
.ENDM

@---------------------------------------------
@�@�X�N���v�g�E�B���h�E
@ check = B�L�����Z����L���ɂ��邩,�w��Ȃ�(0)�̏ꍇ�͗L��
@ �J�[�\���̏����ʒu���w��o����
@---------------------------------------------
.MACRO	_SELECT_WINDOW_CSR	xpos,ypos,mojino,csr,check=0
	.byte	EVCMD_SELECTWINCSR
	.byte	\xpos,\ypos,\mojino,\csr
	.byte	\check
.ENDM

@---------------------------------------------
@�@�c���E�B���h�E
@ check = B�L�����Z����L���ɂ��邩,�w��Ȃ�(0)�̏ꍇ�͗L��
@---------------------------------------------
.MACRO	_HV_WINDOW	xpos,ypos,mojino,arrange,check = 0
	.byte	EVCMD_HVWIN
	.byte	\xpos,\ypos,\mojino,\arrange
	.byte	\check
.ENDM

//020717 tomo �R�C���E�C���h�E�n�ǉ�
@---------------------------------------------
@ �����R�C���E�B���h�E�\��
@---------------------------------------------
.MACRO	_COIN_WIN_WRITE	x,y
	.byte	EVCMD_COINWINWRITE
	.byte	\x,\y
.ENDM

@---------------------------------------------
@ �����R�C���E�B���h�E�폜
@---------------------------------------------
.MACRO	_COIN_WIN_DEL	x,y
	.byte	EVCMD_COINWINDEL
	.byte	\x,\y
.ENDM

@---------------------------------------------
@ �����R�C���E�B���h�E�X�V
@---------------------------------------------
.MACRO	_COIN_WRITE	x,y
	.byte	EVCMD_COINWRITE
	.byte	\x,\y
.ENDM

@---------------------------------------------
@�@�E�B���h�E(�̂�)�\��
@---------------------------------------------
.MACRO	_WINDOW_WRITE	xpos,ypos,xpos2,ypos2
	.byte	EVCMD_WINWRITE
	.byte	\xpos,\ypos,\xpos2,\ypos2
.ENDM

@---------------------------------------------
@�@�w��͈͂��N���A
@---------------------------------------------
.MACRO	_WINDOW_CLEAR	xpos,ypos,xpos2,ypos2
	.byte	EVCMD_WINCLEAR
	.byte	\xpos,\ypos,\xpos2,\ypos2
.ENDM

@---------------------------------------------
@---------------------------------------------
.MACRO	_SCREEN_CLEAR
	_WINDOW_CLEAR	0,0,29,19
.ENDM

@---------------------------------------------
@�@���j���[(�̂�)�\��
@---------------------------------------------
.MACRO	_MENU_WRITE	xpos,ypos,mojino,check = 0
	.byte	EVCMD_MENUWRITE
	.byte	\xpos,\ypos,\mojino
	.byte	\check
.ENDM

@---------------------------------------------
@�@�|�P����CG�\���E�B���h�E�\��
@---------------------------------------------
.MACRO	_POKE_CG_WRITE	no,xpos,ypos
	.byte	EVCMD_POKECGWRITE
	.short	\no
	.byte	\xpos,\ypos
.ENDM

@---------------------------------------------
@�@�|�P����CG�\���E�B���h�E����
@---------------------------------------------
.MACRO	_POKE_CG_ERASE
	.byte	EVCMD_POKECGERASE
.ENDM


@---------------------------------------------
@	����G�\��
@	number == ����G�f�[�^�i���o�[�A1�`13
@---------------------------------------------
.MACRO	_PUT_PORTRAIT	number
//	.byte	EVCMD_PUTPORTRAIT
//	.byte	\number
//	_TIME_WAIT		1
//	_AB_KEYWAIT
//	_SCREEN_CLEAR

	_FLD_FADE_BLACKOUT
	.byte	EVCMD_PUTPORTRAIT
	.byte	\number
.ENDM

@---------------------------------------------
@		�_�����b�Z�[�W�\��
@---------------------------------------------
.MACRO	_PUT_TENJI	adrs
	.byte	EVCMD_TENJIMSGPUT
	.word	\adrs
.ENDM

.MACRO	_EASY_PUT_TENJI	adrs
	_TALK_START
	_PUT_TENJI	\adrs
	_AB_KEYWAIT
	_SCREEN_CLEAR
	_TALK_END
.ENDM

@----------------------------------------------------------
@
@	�|�P�����֘A
@
@----------------------------------------------------------
@---------------------------------------------
@�@�|�P�������莝���ɉ�����
@---------------------------------------------
.MACRO	_ADD_POKEMON	monsno,lv,itemno,adrs1,adrs2,name_sw
	.byte	EVCMD_ADDPOKE
	.short	\monsno
	.byte	\lv
	.short	\itemno
	.word	\adrs1,\adrs2
	.byte	\name_sw
.ENDM

@---------------------------------------------
@�@�^�}�S���莝���ɉ�����
@---------------------------------------------
.MACRO	_ADD_TAMAGO		monsno
	.byte	EVCMD_ADDTAMAGO
	.short	\monsno
.ENDM

@---------------------------------------------
@ �莝���|�P�����̋Z��u��������
@---------------------------------------------
.MACRO	_WAZA_CHANGE pos,waza_pos,wazano
	.byte	EVCMD_PM_WAZA_CHANGE
	.byte	\pos
	.byte	\waza_pos
	.short	\wazano
.ENDM

@---------------------------------------------
@ �w�肳�ꂽ�Z���o���Ă���莝���|�P�����𒲂ׂ�	2002/01/23 by nakahiro
@---------------------------------------------
.MACRO	_WAZA_CHECK	wazano
	.byte	EVCMD_PM_WAZA_CHECK
	.short	\wazano
.ENDM

@----------------------------------------------------------
@
@	�����񑀍�֘A
@
@----------------------------------------------------------
@---------------------------------------------
@�@������擾(�|�P����)
@---------------------------------------------
.MACRO	_GET_MONSNAME	buf,monsno
	.byte	EVCMD_GETMONSNAME
	.byte	\buf
	.short	\monsno
.ENDM

@---------------------------------------------
@�@������擾(�莝���̐擪�̃|�P����)
@---------------------------------------------
.MACRO	_GET_TEMOTI_MONSNAME	buf
	.byte	EVCMD_GETTEMOTIMONSNAME
	.byte	\buf
.ENDM

@---------------------------------------------
@�@������擾(�|�P�����j�b�N�l�[��)
@---------------------------------------------
.MACRO	_GET_NICKNAME	buf,no
	.byte	EVCMD_GETNICKNAME
	.byte	\buf
	.short	\no
.ENDM

@---------------------------------------------
@�@������擾(�A�C�e��)
@---------------------------------------------
.MACRO	_GET_ITEMNAME	buf,itemno
	.byte	EVCMD_GETITEMNAME
	.byte	\buf
	.short	\itemno
.ENDM

@---------------------------------------------
@�@������擾(�O�b�Y)
@---------------------------------------------
.MACRO	_GET_GOODSNAME	buf,goodsno
	.byte	EVCMD_GETGOODSNAME
	.byte	\buf
	.short	\goodsno
.ENDM

@---------------------------------------------
@�@������擾(�Z)
@---------------------------------------------
.MACRO	_GET_WAZANAME	buf,waza
	.byte	EVCMD_GETWAZANAME
	.byte	\buf
	.short	\waza
.ENDM

@---------------------------------------------
@�@������擾(����)
@---------------------------------------------
.MACRO	_GET_NUM_MSG	buf,num
	.byte	EVCMD_GETNUMMSG
	.byte	\buf
	.short	\num
.ENDM

@---------------------------------------------
@�@������擾
@---------------------------------------------
.MACRO	_SET_BUF	buf,no
	.byte	EVCMD_SETBUF
	.byte	\buf
	.short	\no
.ENDM

@---------------------------------------------
@	�w�蕶������o�b�t�@�ɃR�s�[
@---------------------------------------------
.MACRO	_COPY_BUF	buf,str
	.byte	EVCMD_COPYBUF
	.byte	\buf
	.word	\str
.ENDM

@----------------------------------------------------------
@	�쐶�|�P������֘A
@----------------------------------------------------------

@---------------------------------------------
@	�쐶�|�P�����f�[�^�Z�b�g
@---------------------------------------------
.MACRO	_WILD_FIGHT_SET	pokeno,level,itemno
	.byte	EVCMD_SET_WILDFIGHT
	.short	\pokeno
	.byte	\level
	.short	\itemno
.ENDM

@---------------------------------------------
@	�쐶�|�P�����@�G���J�E���g��X�^�[�g
@---------------------------------------------
.MACRO	_WILD_FIGHT_START
	.byte	EVCMD_START_WILDFIGHT
.ENDM


@----------------------------------------------------------
@	�g���[�i�[�����֘A
@----------------------------------------------------------
@---------------------------------------------
@	�g���[�i�[��̌Ăяo��
@---------------------------------------------
.MACRO	_TRAINER_FIGHT
	.byte	EVCMD_TRAINERFIGHT
.ENDM

@---------------------------------------------
@	�g���[�i�[��I����̃X�N���v�g�Ăяo��
@---------------------------------------------
.MACRO	_TRAINER_AFTER
	.byte	EVCMD_TRAINERAFTER
.ENDM

@---------------------------------------------
@	�g���[�i�[��I����̃X�N���v�g�Ăяo��
@---------------------------------------------
.MACRO	_TRAINER_BATTLE_END_SCRIPT
	.byte	EVCMD_TRN_END_SCRIPT
.ENDM

@---------------------------------------------
@	�g���[�i�[�p��b�J�n
@�������_TALK_OBJ_START�Ƃقړ�������
@�@����̎d�l�ύX�Ȃǂ��l�����ĕʃR�}���h��
@�@�p�ӂ��Ă���
@---------------------------------------------
.MACRO	_TRAINER_OBJ_START
	.byte	EVCMD_TALKOBJSTART
.ENDM

@---------------------------------------------
@
@---------------------------------------------
.MACRO	_TRAINER_EVENT_FIGHT	id,msg
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_FORCE
	.short	\id,0
	.word	\msg
.ENDM

@---------------------------------------------
@	�g���[�i�[���̊ȈՂl�r�f�\��
@---------------------------------------------
.MACRO	_TRAINER_EASY_MSG	msg
	_LD_REG_WDATA	0,\msg
	_CALLID	SCRID_TRAINEREASYMSG
@	_CALL	TrainerEasyMsgScript
.ENDM


@	�g���[�i�[�f�[�^�̋L�q

@---------------------------------------------
@�ʏ�̃g���[�i�[�f�[�^�J�n
@---------------------------------------------
.MACRO	_TRAINER_DATA_START
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_NORMAL
.ENDM

@---------------------------------------------
@�C�x���g����݂̃g���[�i�[�f�[�^�J�n
@---------------------------------------------
.MACRO	_EVTRAINER_DATA_START
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_EVENT
.ENDM

@---------------------------------------------
@�W�����[�_�[�̃g���[�i�[�f�[�^�J�n
@---------------------------------------------
.MACRO	_LEADER_DATA_START
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_LEADER
.ENDM

@---------------------------------------------
@2vs2�ΐ�̃g���[�i�[�f�[�^��`
@---------------------------------------------
.MACRO	_TRAINER2vs2_DATA_START
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_2VS2
.ENDM

@---------------------------------------------
@�C�x���g����݂�2vs2�ΐ�̃g���[�i�[�f�[�^��`
@---------------------------------------------
.MACRO	_EVTRAINER2vs2_DATA_START
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_EVENT2VS2
.ENDM

@---------------------------------------------
@2vs2�ΐ�W�����[�_�[�̃g���[�i�[�f�[�^��`
@---------------------------------------------
.MACRO	_LEADER2vs2_DATA_START
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_LEADER2VS2
.ENDM

@---------------------------------------------
@�Đ�^�C�v�̃g���[�i�[�f�[�^��`
@---------------------------------------------
.MACRO	_REVENGE_TRAINER_DATA_START
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_REVENGE
.ENDM

@---------------------------------------------
@2vs2�Đ�^�C�v�̃g���[�i�[�f�[�^��`
@---------------------------------------------
.MACRO	_REVENGE2vs2_TRAINER_DATA_START
	.byte	EVCMD_TRAINERSET
	.byte	TR_TYPE_REVENGE2VS2
.ENDM


@�g���[�i�[ID�i+�g���[�i�[OBJ�j�̒�`
.MACRO	_TRAINER_DATA1	id,obj=0
	.short	\id,\obj
.ENDM

@�퓬�J�n�OMSG
.MACRO	_TRAINER_DATA2	before_msg
	.word	\before_msg
.ENDM

@�퓬�I������MSG�i�퓬��ʓ��ŕ\���j
.MACRO	_TRAINER_DATA3	after_msg
	.word	\after_msg
.ENDM

@�퓬���Ȃ��ꍇ��MSG�i2vs2�ΐ��p�A���͕s�v�j
@		���邢��
@�C�x���g�g���[�i�[�̑ΐ��X�N���v�g�w��
.MACRO	_TRAINER_DATA4	nofight_msg
	.word	\nofight_msg
.ENDM

@2vs2�ΐ�ŃC�x���g�g���[�i�[�̏ꍇ�̑ΐ��X�N���v�g�w��
.MACRO	_TRAINER_DATA5	after_script
	.word	\after_script
.ENDM

@�f�[�^�u���b�N�I���i����_�~�[�j
.MACRO	_TRAINER_DATA_END
.ENDM


@---------------------------------------------
@ �g���[�i�[�t���O�֘A
@---------------------------------------------
@ �t���O�Z�b�g
.MACRO	_TR_FLAG_SET	flag
	.byte	EVCMD_TRN_FLAG_SET
	.short	\flag
.ENDM

@ �t���O���Z�b�g
.MACRO	_TR_FLAG_RESET	flag
	.byte	EVCMD_TRN_FLAG_RESET
	.short	\flag
.ENDM

@ �t���O��ON�̂Ƃ�����
.MACRO	_IF_TR_FLAGON_JUMP	flag,adr
	.byte	EVCMD_TRN_FLAG_CHK
	.short	\flag
	_IF_JUMP FLGON ,\adr
.ENDM

@ �t���O��ON�̂Ƃ�����(CALL)				02/03/05	nohara
.MACRO	_IF_TR_FLAGON_CALL	flag,adr
	.byte	EVCMD_TRN_FLAG_CHK
	.short	\flag
	_IF_CALL FLGON ,\adr
.ENDM

@ �t���O��OFF�̂Ƃ�����
.MACRO	_IF_TR_FLAGOFF_JUMP	flag,adr
	.byte	EVCMD_TRN_FLAG_CHK
	.short	\flag
	_IF_JUMP FLGOFF ,\adr
.ENDM

@ �t���O��OFF�̂Ƃ�����(CALL)				02/03/05	nohara
.MACRO	_IF_TR_FLAGOFF_CALL	flag,adr
	.byte	EVCMD_TRN_FLAG_CHK
	.short	\flag
	_IF_CALL FLGOFF ,\adr
.ENDM

@----------------------------------------------------------
@ �v���O�����𒼐ڑ��삷�閽��(�ʏ�͎g��Ȃ��j
@----------------------------------------------------------
@---------------------------------------------
@ �����҂��֐����Z�b�g
@---------------------------------------------
.MACRO	_SET_WAIT_FUNC	function
	.byte	EVCMD_SETWAITSUB
	.word	\function
.ENDM

@---------------------------------------------
@ �v���O�������ڌďo��
@---------------------------------------------
.MACRO	_FUNC_DIRECT_CALL	adrs
	.byte	EVCMD_SUBROUTINE
	.word	\adrs
.ENDM


@----------------------------------------------------------
@ Load Command
@----------------------------------------------------------
.MACRO	_LD_REG_WDATA	r,wdata
	.byte	EVCMD_LD_REG_WDATA
	.byte	\r
	.word	\wdata
.ENDM

.MACRO	_LD_REG_REG	r1,r2
	.byte	EVCMD_LD_REG_REG
	.byte	\r1,\r2
.ENDM

.MACRO	_LD_REG_VAL	r,val
	.byte	EVCMD_LD_REG_VAL
	.byte	\r
	.byte	\val
.ENDM

.MACRO	_LD_REG_ADR	reg,ADDRESS
	.byte	EVCMD_LD_REG_ADR
	.byte	\reg
	.word	\ADDRESS
.ENDM

.MACRO	_LD_ADR_REG	adrs,reg
	.byte	EVCMD_LD_ADR_REG
	.byte	\reg
	.word	\adrs
.ENDM

.MACRO	_LD_ADR_VAL	adrs,val
	.byte	EVCMD_LD_ADR_VAL
	.word	\adrs
	.byte	\val
.ENDM

.MACRO	_LD_ADR_ADR	adr1,adr2
	.byte	EVCMD_LD_ADR_ADR
	.word	\adr1
	.word	\adr2
.ENDM

@----------------------------------------------------------
@ compare command
@----------------------------------------------------------
.MACRO	_CP_REG_REG	r1,r2
	.byte	EVCMD_CP_REG_REG
	.byte	\r1,\r2
.ENDM

.MACRO	_CP_REG_VAL	r1,val
	.byte	EVCMD_CP_REG_VAL
	.byte	\r1,\val
.ENDM

.MACRO	_CP_REG_ADR	r1,adrs
	.byte	EVCMD_CP_REG_ADR
	.byte	\r1
	.word	\adrs
.ENDM

.MACRO	_CP_ADR_REG	adrs,r1
	.byte	EVCMD_CP_ADR_REG
	.word	\adrs
	.byte	\r1
.ENDM

.MACRO	_CP_ADR_VAL	adrs,val
	.byte	EVCMD_CP_ADR_VAL
	.word	\adrs
	.byte	\val
.ENDM

.MACRO	_CP_ADR_ADR	adr1,adr2
	.byte	EVCMD_CP_ADR_ADR
	.word	\adr1
	.word	\adr2
.ENDM

@----------------------------------------------------------
@	�t�F�[�h�R�}���h
@----------------------------------------------------------
@ 
.MACRO	_FLD_FADE_BLACKIN
	.byte	EVCMD_FIELDFADE
	.byte	0
.ENDM

.MACRO	_FLD_FADE_BLACKOUT
	.byte	EVCMD_FIELDFADE
	.byte	1
.ENDM

.MACRO	_FLD_FADE_WHITEIN
	.byte	EVCMD_FIELDFADE
	.byte	2
.ENDM

.MACRO	_FLD_FADE_WHITEOUT
	.byte	EVCMD_FIELDFADE
	.byte	3
.ENDM

.MACRO	_FLD_FADE_WAIT	type,wait
	.byte	EVCMD_FIELDFADEWAIT
	.byte	\type,\wait
.ENDM

@---------------------------------------------
@	�t�B�[���h���E�̐���
@---------------------------------------------
.MACRO	_FLD_BRIGHT_CHANGE	val
	.byte	EVCMD_CHG_BRIGHT
	.short	\val
.ENDM

.MACRO	_FLD_BRIGHT_EFFECT	end
	.byte	EVCMD_BRIGHT_EFF
	.byte	\end
.ENDM

@----------------------------------------------------------
@	�}�b�v�J��
@----------------------------------------------------------
@---------------------------------------------
@	�ꏊ���w�肵�ă}�b�v�ړ�
@ ���̃R�}���h�̌��_STOP_SCRIPT�������悤�ɂ���(���@�̓�����Ƃ߂�)
@---------------------------------------------
.MACRO	_MAP_CHANGE	div,map,x,y
	.byte	EVCMD_MAP_CHANGE
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM

@---------------------------------------------
@	�ꏊ���w�肵�ă}�b�v�ړ�(SE�Ȃ�)
@ ���̃R�}���h�̌��_STOP_SCRIPT�������悤�ɂ���(���@�̓�����Ƃ߂�)
@---------------------------------------------
.MACRO	_MAP_CHG_SENOT	div,map,x,y
	.byte	EVCMD_MAP_CHG_SENOT
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM

@---------------------------------------------
@	(�h�A�ɓ�����)�ꏊ���w�肵�ă}�b�v�ړ�
@ ���̃R�}���h�̌��_STOP_SCRIPT�������悤�ɂ���(���@�̓�����Ƃ߂�)
@---------------------------------------------
.MACRO	_DOOR_MAP_CHANGE	div,map,x,y
	.byte	EVCMD_DOORMAP_CHANGE
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM

@---------------------------------------------
@	�}�b�v�ړ��̏ꏊ���w��
@�����̃R�}���h�����ł̓}�b�v�J�ڂ͂��Ȃ�
@---------------------------------------------
.MACRO	_SET_MAP_CHANGE_POS	div,map,x,y
	.byte	EVCMD_SET_MAP_CHG
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM


@---------------------------------------------
@	�ꏊ���w�肵�ă}�b�v�ړ�
@	�����������W�͌��݈ʒu�����̂܂܈����p��
@---------------------------------------------
.MACRO	_MAP_CHANGE_FALL	div,map
	.byte	EVCMD_MAP_CHG_FALL
	.byte	\div,\map
.ENDM

@---------------------------------------------
@	�ꏊ���w�肵�ă}�b�v�ړ�
@	SE_WARPIN,SE_WARPOUT�������Ă���
@---------------------------------------------
.MACRO	_MAP_CHANGE_WARP	div,map,x,y
	.byte	EVCMD_MAP_CHG_WARP
	.byte	\div,\map,NO_EXIT_ID
	@.byte	\x,\y
	.short	\x,\y
.ENDM

@---------------------------------------------
@	�ʐM�����ւ̃}�b�v�J��
@---------------------------------------------
.MACRO	_COMM_MAP_CHANGE	div,map,x,y
	.byte	EVCMD_MAP_CHANGE
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM

@---------------------------------------------
@	�ʐM�R���e�X�g��p�̃}�b�v�J��
@---------------------------------------------
.MACRO	_MAP_CHANGE_CONTEST	div,map,x,y
	_SET_MAP_CHANGE_POS	\div,\map,\x,\y
	_PROGRAM	CONTEST_MAP_CHANGE
	_STOP_SCRIPT
.ENDM

@---------------------------------------------
@	����ڑ��Z�b�g
@---------------------------------------------
.MACRO	_SET_SP_MAP_ID	div,map,x,y
	.byte	EVCMD_SETSPMAPID
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM

@---------------------------------------------
@	�_�C�r���O�ڑ���Z�b�g
@---------------------------------------------
.MACRO	_SET_DIVING_MAP_ID	div,map,x,y
	.byte	EVCMD_SETDIVINGID
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM

@---------------------------------------------
@	���Ƃ����ڑ���Z�b�g
@---------------------------------------------
.MACRO	_SET_PITFALL_MAP_ID	div,map,x,y
	.byte	EVCMD_SETPITFALLID
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM

@---------------------------------------------
@	���Ȃʂ��ڑ���Z�b�g
@---------------------------------------------
.MACRO	_SET_ESCAPE_MAP_ID	div,map,x,y
	.byte	EVCMD_SET_ESCMAPID
	.byte	\div,\map,NO_EXIT_ID
	.short	\x,\y
.ENDM

@---------------------------------------------
@	���[�v�̖߂����Z�b�g����(�퓬�Ŕs�k�������Ȃ�)
@---------------------------------------------
@ 
.MACRO	_SET_WARP_ID	id
	.byte	EVCMD_SETWARPID
	.short	\id
.ENDM

@---------------------------------------------
@	���݂̃}�b�v�ʒu�̎擾
@---------------------------------------------
.MACRO	_GET_MAP_POS	x,y
	.byte	EVCMD_GET_MAPPOS
	.short	\x,\y
.ENDM

@----------------------------------------------------------
@	�w��ʒu�ɃZ�����Z�b�g
@----------------------------------------------------------
@ 
.MACRO	_SET_CELL		x,y,code,chk
	.byte	EVCMD_SETCELL
	.short	\x,\y,\code,\chk
.ENDM

@---------------------------------------------
@	�莝���̃|�P�����̐l�����擾
@---------------------------------------------
.MACRO	_GET_TEMOTI_NUM
	.byte	EVCMD_GETTEMOTINUM
.ENDM


@----------------------------------------------------------
@	�����֘A
@----------------------------------------------------------
@ 
@---------------------------------------------
@ ������������
@ code = �������s���ꏊ,�w��Ȃ��̏ꍇ�͎莝��
@---------------------------------------------
.MACRO	_ADD_GOLD	gold,code = 0
	.byte	EVCMD_ADDGOLD
	.word	\gold
	.byte	\code
.ENDM

@---------------------------------------------
@ ����������
@ code = �������s���ꏊ,�w��Ȃ��̏ꍇ�͎莝��
@---------------------------------------------
.MACRO	_SUB_GOLD	gold,code = 0
	.byte	EVCMD_SUBGOLD
	.word	\gold
	.byte	\code
.ENDM

@---------------------------------------------
@ ���z�𒲂ׂ�
@ code = �������s���ꏊ,�w��Ȃ��̏ꍇ�͎莝��
@---------------------------------------------
.MACRO	_COMP_GOLD	gold,code = 0
	.byte	EVCMD_COMPGOLD
	.word	\gold
	.byte	\code
.ENDM

@---------------------------------------------
@ �������E�B���h�E�\��
@ code = �������s���ꏊ,�w��Ȃ��̏ꍇ�͎莝��
@---------------------------------------------
.MACRO	_GOLD_WIN_WRITE	x,y,code = 0
	.byte	EVCMD_GOLDWINWRITE
	.byte	\x,\y,\code
.ENDM

@---------------------------------------------
@ �������E�B���h�E�폜
@---------------------------------------------
.MACRO	_GOLD_WIN_DEL	x,y
	.byte	EVCMD_GOLDWINDEL
	.byte	\x,\y
.ENDM

@---------------------------------------------
@ �������E�B���h�E�X�V
@ code = �������s���ꏊ,�w��Ȃ��̏ꍇ�͎莝��
@---------------------------------------------
.MACRO	_GOLD_WRITE	x,y,code = 0
	.byte	EVCMD_GOLDWRITE
	.byte	\x,\y,\code
.ENDM

@----------------------------------------------------------
@	�V���b�v�֘A
@----------------------------------------------------------
@---------------------------------------------
@�@�V���b�v�Ăяo��
@---------------------------------------------
// �t�����h���B�V���b�v
.MACRO	_SHOP_START	menu
	.byte	EVCMD_SHOPSTART
	.word	\menu
	_TALK_KEYWAIT	msg_shop_p02
	_TALK_OBJ_END
	_END
.ENDM

// �O�b�Y�V���b�v
.MACRO	_GOODS_SHOP_START	menu
	.byte	EVCMD_GD_SHOPSTART
	.word	\menu
	_TALK_KEYWAIT	msg_shop_p02
	_TALK_OBJ_END
	_END
.ENDM

// �t�����[�V���b�v
.MACRO	_FLOWER_SHOP_START	menu
	.byte	EVCMD_FL_SHOPSTART
	.word	\menu
	_TALK_KEYWAIT	msg_shop_p02
	_TALK_OBJ_END
	_END
.ENDM

@---------------------------------------------
@	�V���b�v���j���[�L�q�p��`
@---------------------------------------------
.MACRO	_SHOP_MENU_START	menu
	.ALIGN
\menu:
.ENDM

.MACRO	_SHOP_MENU	item
	.short	\item
.ENDM
.MACRO	_SHOP_MENU_END
	.short	0
.ENDM

@----------------------------------------------------------
@	�J�W�m�֘A
@----------------------------------------------------------
@---------------------------------------------
@	�X���b�g�}�V�[��
@---------------------------------------------
.MACRO	_SLOT_MACHINE	type
	.byte	EVCMD_SLOTMACHINE
	.short	\type
.ENDM

@----------------------------------------------------------
@		���̂݊֘A
@----------------------------------------------------------
.MACRO	_SET_SEED	bed_no,type,growth
	.byte	EVCMD_SET_SEED
	.byte	\bed_no,\type,\growth
.ENDM

@----------------------------------------------------------
@	�s�u���
@----------------------------------------------------------
@ 
@ �J�Â��Ă���CM�f�[�^�����邩
.MACRO	_CM_EVENT_START_CHK	type
	.byte	EVCMD_CMEVENTSTARTCHK
	.short	\type
.ENDM

@----------------------------------------------------------
@	�R���e�X�g
@----------------------------------------------------------
@ 
@�@�R���e�X�g�̃|�P�����I��(�X�e�[�^�X���)�Ăяo��
.MACRO	_CON_POKELIST
	.byte	EVCMD_CONPOKELIST
.ENDM

@�@�񎟐R�����Ăяo��
.MACRO	_CON_NIJI_CALL
	.byte	EVCMD_CONNIJICALL
.ENDM

@�@���ʔ��\��ʂ��Ăяo��
.MACRO	_CON_RESULTSCR
	.byte	EVCMD_CONRESULTSCR
.ENDM

@�@�R���e�X�g�ʐM
.MACRO	_CON_TUUSIN_INIT
	.byte	EVCMD_CONTUUSININIT
.ENDM

@----------------------------------------------------------
@	�t�B�[���h�G�t�F�N�g
@----------------------------------------------------------
@ 
@�@�Ăяo��
.MACRO	_SET_FLD_EFF		no
	.byte	EVCMD_SETFLDEFF
	.short	\no
.ENDM

@�@���[�N�Z�b�g		2002/01/24 by nakahiro
.MACRO	_SET_FE_WORK		no,param
	.byte	EVCMD_SETFEWORK
	.byte	\no
	.short	\param
.ENDM

@  �G�t�F�N�g�I���҂�(code = �G�t�F�N�g�R�[�h)
.MACRO	_FLD_EFF_WAIT	code
	.byte	EVCMD_FLDEFFWAIT
	.short	\code
.ENDM


@----------------------------------------------------------
@	���ʂ��擾����
@----------------------------------------------------------
@ 
.MACRO	_GET_MY_SEX
	.byte	EVCMD_GETMYSEX
.ENDM

@----------------------------------------------------------
@	�|�P�����̋�������炷
@----------------------------------------------------------
@ 
.MACRO	_VOICE_PLAY		no,ptn
	_SE_WAIT
	.byte	EVCMD_VOICEPLAY
	.short	\no,\ptn
.ENDM

.MACRO	_VOICE_WAIT
	.byte	EVCMD_VOICE_WAIT
.ENDM

@----------------------------------------------------------
@	�h�A�A�j���Ăяo��
@----------------------------------------------------------
@ 
@�h�A�A�j���[�V����(������)
.MACRO	_OPEN_DOOR_ANIME		x,y
	.byte	EVCMD_OPENDOORANIME
	.short	\x,\y
.ENDM

@�h�A�A�j���[�V����(���߂�)
.MACRO	_CLOSE_DOOR_ANIME		x,y
	.byte	EVCMD_CLOSEDOORANIME
	.short	\x,\y
.ENDM

@�h�A�A�j���[�V�����I���҂�
.MACRO	_DOOR_ANIME_WAIT
	.byte	EVCMD_DOORANIMEWAIT
.ENDM

@�h�A�̃Z�����Z�b�g����(�����Ă����Ԃɂ���)
.MACRO	_OPEN_DOOR_SET	x,y
	.byte	EVCMD_OPENDOORSET
	.short	\x,\y
.ENDM

@�h�A�̃Z�����Z�b�g����(���܂��Ă����Ԃɂ���)
.MACRO	_CLOSE_DOOR_SET	x,y
	.byte	EVCMD_CLOSEDOORSET
	.short	\x,\y
.ENDM

@----------------------------------------------------------
@	�G���x�[�^�֘A
@----------------------------------------------------------

@---------------------------------------------
@	�G���x�[�^�L�q�p��`
@---------------------------------------------
.MACRO	_ELEVATOR_MENU	msg,div,map,exit
	.byte	EVCMD_ELEVATORSET
	.byte	\msg
	.short	\div,\map,\exit
.ENDM

.MACRO	_ELEVATOR_MENU_START
	_TALKMSG	msg_elevator_p01
	_TALK_WAIT
	.byte	EVCMD_ELEVATORSTART
.ENDM


@----------------------------------------------------------
@	�R�C���֘A	2002/07/03 by nakahiro
@----------------------------------------------------------
@ �莝���R�C���̖������`�F�b�N
.MACRO	_CHECK_COIN	wk
	.byte	EVCMD_CHECKCOIN
	.short	\wk
.ENDM

@ �莝���R�C����������
.MACRO	_ADD_COIN	coin
	.byte	EVCMD_ADDCOIN
	.short	\coin
.ENDM

@ �莝���R�C�������炷
.MACRO	_SUB_COIN	coin
	.byte	EVCMD_SUBCOIN
	.short	\coin
.ENDM



@===================================================================
@
@
@	���΃A�h���X���p����
@	
@	���z�z�p�X�N���v�g�̃A�h���X���s��Ȃ̂�
@	�@�f�[�^�E�X�N���v�g����Ȃǂ��ׂđ��΃A�h���X�\�L����B
@
@===================================================================

@----------------------------------------------------------
@	���򖽗ߊ֘A
@----------------------------------------------------------

@---------------------------------------------
@	���΃A�h���X�g�p�錾
@---------------------------------------------
.MACRO		_INIT_RELATIVE_ADRS	adrs
	.GLOBAL	\adrs
\adrs:
	.byte	EVCMD_SET_RELADRS
	.word	\adrs
.ENDM

@---------------------------------------------
@	���΃A�h���X�W�����v
@---------------------------------------------
.MACRO		_JUMPR	adrs
	.byte	EVCMD_JUMPR
	.word	\adrs
.ENDM

@---------------------------------------------
@	���΃A�h���X�R�[��
@---------------------------------------------
.MACRO		_CALLR	adrs
	.byte	EVCMD_CALLR
	.word	\adrs
.ENDM

@---------------------------------------------
@	�����t�����΃A�h���X�W�����v
@---------------------------------------------
.MACRO		_IF_JUMPR	cond,adrs
	.byte	EVCMD_IFJUMPR
	.byte	\cond
	.word	\adrs
.ENDM

@---------------------------------------------
@	�����t�����΃A�h���X�R�[��
@---------------------------------------------
.MACRO		_IF_CALLR	cond,adrs
	.byte	EVCMD_IFCALLR
	.byte	\cond
	.word	\adrs
.ENDM

@ �t���O��ON�̂Ƃ�����
.MACRO	_IF_FLAGON_JUMPR	flag,adr
	.byte	EVCMD_FLAGCHECK
	.short	\flag
	_IF_JUMPR FLGON ,\adr
.ENDM

@ �t���O��ON�̂Ƃ�����(CALL)				02/03/05	nohara
.MACRO	_IF_FLAGON_CALLR	flag,adr
	.byte	EVCMD_FLAGCHECK
	.short	\flag
	_IF_CALLR FLGON ,\adr
.ENDM

@ �t���O��OFF�̂Ƃ�����
.MACRO	_IF_FLAGOFF_JUMPR	flag,adr
	.byte	EVCMD_FLAGCHECK
	.short	\flag
	_IF_JUMPR FLGOFF ,\adr
.ENDM

@ �t���O��OFF�̂Ƃ�����(CALL)				02/03/05	nohara
.MACRO	_IF_FLAGOFF_CALLR	flag,adr
	.byte	EVCMD_FLAGCHECK
	.short	\flag
	_IF_CALLR FLGOFF ,\adr
.ENDM

@���[�N�ƒl���r�A�����ǂ���̏ꍇ����
.MACRO	_IFVAL_JUMPR	wk,cond,val,adr
	_CMPVAL	\wk,\val
	.byte	EVCMD_IFJUMPR
	.byte	\cond
	.word	\adr
.ENDM

@���[�N�ƒl���r�A�����ǂ���̏ꍇ����
.MACRO	_IFVAL_CALLR	wk,cond,val,adr
	_CMPVAL	\wk,\val
	.byte	EVCMD_IFCALLR
	.byte	\cond
	.word	\adr
.ENDM

@ ���[�N�ƃ��[�N���r�A�����ǂ���̏ꍇ����
.MACRO	_IFWK_JUMPR	wk1,cond,wk2,adr
	_CMPWK	\wk1,\wk2
	.byte	EVCMD_IFJUMPR
	.byte	\cond
	.word	\adr
.ENDM

@ ���[�N�ƃ��[�N���r�A�����ǂ���̏ꍇ����
.MACRO	_IFWK_CALLR	wk1,cond,wk2,adr
	_CMPWK	\wk1,\wk2
	.byte	EVCMD_IFCALLR
	.byte	\cond
	.word	\adr
.ENDM

@----------------------------------------------------------
@----------------------------------------------------------
@---------------------------------------------
@	��b�l�r�f�\���i���΃A�h���X�w��j
@---------------------------------------------
.MACRO	_TALKMSG_REL	ADDRESS
	.byte	EVCMD_TALKMSG_REL
	.word	\ADDRESS
.ENDM

@---------------------------------------------
@	�w�胁�b�Z�[�W�̓W�J�������Ȃ�(���΃A�h���X�w��j
@---------------------------------------------
.MACRO	_EXPAND_MSG_REL	ADDRESS
	.byte	EVCMD_EXPANDMSG_REL
	.word	\ADDRESS
.ENDM

@---------------------------------------------
@	�w�蕶������o�b�t�@�ɃR�s�[(���΃A�h���X�w��j
@---------------------------------------------
.MACRO	_COPY_BUF_REL	buf_no,	ADDRESS
	.byte	EVCMD_COPYBUF_REL
	.byte	\buf_no
	.word	\ADDRESS
.ENDM

@----------------------------------------------------------
@	�ł񂹂֘A
@		�ł񂹂p�L�^�̃J�E���g�A�b�v
@----------------------------------------------------------
.MACRO	_INC_RECORD	record_id
	.byte	EVCMD_INC_RECORD
	.byte	\record_id
.ENDM

@----------------------------------------------------------
@	���y�E�r�d�֘A
@	�r�d�I���҂�	
@----------------------------------------------------------
.MACRO	_SE_WAIT
	.byte	EVCMD_SE_WAIT
.ENDM

