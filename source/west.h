
@======================================================
@	west.h
@======================================================

	.text
	.code 32

@======================================================
@	�Z�G�t�F�N�g�̓���V�[�P���X����macro
@======================================================

	.MACRO	INIT_CMD
DEF_CMD_COUNT	=	0
	.ENDM

	.MACRO	DEF_CMD	symname
\symname	=	DEF_CMD_COUNT
DEF_CMD_COUNT 	=	(DEF_CMD_COUNT + 1)
	.ENDM

	INIT_CMD
	DEF_CMD	PRI0
	DEF_CMD	PRI1
	DEF_CMD	PRI2
	DEF_CMD	PRI3
	DEF_CMD	PRI4
	DEF_CMD	PRI5
	DEF_CMD	PRI6
	DEF_CMD	PRI7
	DEF_CMD	PRI8
	DEF_CMD	PRI9
	DEF_CMD	PRI10
	DEF_CMD	PRI11
	DEF_CMD	PRI12
	DEF_CMD	PRI13
	DEF_CMD	PRI14

	INIT_CMD
	DEF_CMD	WEST_CHAR_SET
	DEF_CMD	WEST_CHAR_DEL
	DEF_CMD	WEST_ADD_ACTOR
	DEF_CMD	WEST_ADD_TASK
	DEF_CMD	WEST_WAIT
	DEF_CMD	WEST_WAIT_FLAG
	DEF_CMD	WEST_LOOP_LABEL
	DEF_CMD	WEST_LOOP
	DEF_CMD	WEST_SEQEND
	DEF_CMD	WEST_SE
	DEF_CMD	WEST_POKEBG
	DEF_CMD	WEST_POKEBG_RESET
	DEF_CMD	WEST_BLDALPHA_SET
	DEF_CMD	WEST_BLDALPHA_RESET
	DEF_CMD	WEST_SEQ_CALL
	DEF_CMD	WEST_END_CALL
	DEF_CMD	WEST_WORK_SET
	DEF_CMD	WEST_TURN_CHK
	DEF_CMD	WEST_TURN_JP
	DEF_CMD	WEST_SEQ_JP
	DEF_CMD	WEST_HAIKEI_CHG
	DEF_CMD	WEST_HAIKEI_RECOVER
	DEF_CMD	WEST_HAIKEI_HALF_WAIT
	DEF_CMD	WEST_HAIKEI_CHG_WAIT
	DEF_CMD	WEST_HAIKEI_SET
	DEF_CMD	WEST_SEPLAY_PAN
	DEF_CMD	WEST_SEPAN
	DEF_CMD	WEST_SEPAN_FLOW
	DEF_CMD	WEST_SE_REPEAT
	DEF_CMD	WEST_SE_WAITPLAY
	DEF_CMD	WEST_BLDCNT_SET
	DEF_CMD	WEST_SE_TASK
	DEF_CMD WEST_SEWAIT_FLAG
	DEF_CMD	WEST_WORKCHK_JP
	DEF_CMD	WEST_POKEBG_DROP
	DEF_CMD	WEST_POKEBG_DROP_RESET
	DEF_CMD	WEST_CONTEST_CHK_JP
	DEF_CMD	WEST_HAIKEI_CHKCHG
	DEF_CMD	WEST_SEPAN_FLOWFIX
	DEF_CMD	WEST_SEPAN_FLOW_AF
	DEF_CMD	WEST_BGPRI_GAPSET
	DEF_CMD	WEST_BGPRI_GAPSET2
	DEF_CMD	WEST_BGPRI_GAPSET3
	DEF_CMD	WEST_POKE_BANISH_ON
	DEF_CMD	WEST_POKE_BANISH_OFF
	DEF_CMD	WEST_PARTY_ATTACK_BGOFF
	DEF_CMD	WEST_PARTY_ATTACK_BGEND
	DEF_CMD	WEST_SE_STOP
	

@�L�����Z�b�g�}�N��
	.macro	CHAR_SET	chrID
	.byte	WEST_CHAR_SET			@���߃R�[�h�i���o�[
	.short	\chrID
	.endm

@�L�������Z�b�g�}�N��
	.macro	CHAR_DEL	chrID
	.byte	WEST_CHAR_DEL			@���߃R�[�h�i���o�[
	.short	\chrID
	.endm
	
@AddActor�}�N��
	.macro	ADD_ACTOR	adrs,pri,cnt,p0,p1,p2,p3,p4,p5,p6,p7
	.byte	WEST_ADD_ACTOR			@���߃R�[�h�i���o�[
	.long	\adrs
	.byte	\pri
	.byte	\cnt
	.short	\p0
.if	\cnt>1
	.short	\p1
.endif
.if	\cnt>2
	.short	\p2
.endif
.if	\cnt>3
	.short	\p3
.endif
.if	\cnt>4
	.short	\p4
.endif
.if	\cnt>5
	.short	\p5
.endif
.if	\cnt>6
	.short	\p6
.endif
.if	\cnt>7
	.short	\p7
.endif
	.endm

@AddTask�}�N��
	.macro	ADD_TASK	adrs,pri,cnt,p0,p1,p2,p3,p4,p5,p6,p7
	.byte	WEST_ADD_TASK			@���߃R�[�h�i���o�[
	.long	\adrs
	.byte	\pri
	.byte	\cnt
	.short	\p0
.if	\cnt>1
	.short	\p1
.endif
.if	\cnt>2
	.short	\p2
.endif
.if	\cnt>3
	.short	\p3
.endif
.if	\cnt>4
	.short	\p4
.endif
.if	\cnt>5
	.short	\p5
.endif
.if	\cnt>6
	.short	\p6
.endif
.if	\cnt>7
	.short	\p7
.endif
	.endm

@Wait�}�N��(�J�E���^)
	.macro	WAIT		time
	.byte	WEST_WAIT				@���߃R�[�h�i���o�[
	.byte	\time
	.endm

@Wait�}�N��(�t���O)
	.macro	WAIT_FLAG
	.byte	WEST_WAIT_FLAG			@���߃R�[�h�i���o�[
	.endm

@LoopLabelSet�}�N��
	.macro	LOOP_LABEL
	.byte	WEST_LOOP_LABEL			@���߃R�[�h�i���o�[
	.endm

@Loop�}�N��
	.macro	LOOP
	.byte	WEST_LOOP				@���߃R�[�h�i���o�[
	.endm

@SeqEnd�}�N��
	.macro	SEQEND
	.byte	WEST_SEQEND				@���߃R�[�h�i���o�[
	.endm

@SE�}�N��
	.macro	SE		seID
	.byte	WEST_SE					@���߃R�[�h�i���o�[
	.short	\seID
	.endm

@POKEBG�}�N��
	.macro	POKEBG	flag
	.byte	WEST_POKEBG
	.byte	\flag
	.endm

@POKEBG_RESET�}�N��
	.macro	POKEBG_RESET	flag
	.byte	WEST_POKEBG_RESET
	.byte	\flag
	.endm

@BLDALPHA_SET�}�N��
	.macro	BLDALPHA_SET	eva,evb
	.byte	WEST_BLDALPHA_SET
	.byte	\eva
	.byte	\evb
	.endm

@BLDALPHA_RESET�}�N��
	.macro	BLDALPHA_RESET
	.byte	WEST_BLDALPHA_RESET
	.endm

@SEQ_CALL�}�N��
	.macro	SEQ_CALL	adrs
	.byte	WEST_SEQ_CALL
	.long	\adrs
	.endm

@END_CALL�}�N��
	.macro	END_CALL
	.byte	WEST_END_CALL
	.endm

@WORK_SET�}�N��
	.macro	WORK_SET	soeji,work
	.byte	WEST_WORK_SET
	.byte	\soeji
	.short	\work
	.endm

@TURN_CHK�}�N��
	.macro	TURN_CHK	adrs1,adrs2
	.byte	WEST_TURN_CHK
	.long	\adrs1
	.long	\adrs2
	.endm

@TURN_JP�}�N��
	.macro	TURN_JP		turn,adrs
	.byte	WEST_TURN_JP			@���߃R�[�h�i���o�[
	.byte	\turn					@�w��^�[��
	.long	\adrs
	.endm

@SEQ_JP�}�N��
	.macro	SEQ_JP	adrs
	.byte	WEST_SEQ_JP
	.long	\adrs
	.endm

@HAIKEI_CHG�}�N��
	.macro	HAIKEI_CHG	map_id
	.byte	WEST_HAIKEI_CHG
	.byte	\map_id
	.endm

@HAIKEI_RECOVER�}�N��
	.macro	HAIKEI_RECOVER
	.byte	WEST_HAIKEI_RECOVER
	.endm

@HAIKEI_HALF_WAIT�}�N��
	.macro	HAIKEI_HALF_WAIT
	.byte	WEST_HAIKEI_HALF_WAIT
	.endm

@HAIKEI_CHG_WAIT�}�N��
	.macro	HAIKEI_CHG_WAIT
	.byte	WEST_HAIKEI_CHG_WAIT
	.endm

@WEST_HAIKEI_SET�}�N��
	.macro	HAIKEI_SET	map_id
	.byte	WEST_HAIKEI_SET
	.byte	\map_id
	.endm

@SEPLAY_PAN�}�N��
	.macro	SEPLAY_PAN	seID,pan
	.byte	WEST_SEPLAY_PAN					@���߃R�[�h�i���o�[
	.short	\seID
	.byte	\pan
	.endm

@SEPAN�}�N��
	.macro	SEPAN	pan
	.byte	WEST_SEPAN					@���߃R�[�h�i���o�[
	.byte	\pan
	.endm

@SEPAN_FLOW�}�N��
	.macro	SEPAN_FLOW	seID,start_pan,end_pan,add_pan,wait
	.byte	WEST_SEPAN_FLOW					@���߃R�[�h�i���o�[
	.short	\seID
	.byte	\start_pan
	.byte	\end_pan
	.byte	\add_pan
	.byte	\wait
	.endm

@SE_REPEAT�}�N��
	.macro	SE_REPEAT	seID,pan,wait,repeat
	.byte	WEST_SE_REPEAT					@���߃R�[�h�i���o�[
	.short	\seID
	.byte	\pan
	.byte	\wait
	.byte	\repeat
	.endm

@SE_WAITPLAY�}�N��
	.macro	SE_WAITPLAY	seID,pan,wait
	.byte	WEST_SE_WAITPLAY					@���߃R�[�h�i���o�[
	.short	\seID
	.byte	\pan
	.byte	\wait
	.endm

@BLDCNT_SET�}�N��
	.macro	BLDCNT_SET	work
	.byte	WEST_BLDCNT_SET
	.short	\work
	.endm

@SE�p��AddTask�}�N��
	.macro	SE_TASK		adrs,cnt,p0,p1,p2,p3,p4,p5,p6,p7
	.byte	WEST_SE_TASK			@���߃R�[�h�i���o�[
	.long	\adrs
	.byte	\cnt
	.short	\p0
.if	\cnt>1
	.short	\p1
.endif
.if	\cnt>2
	.short	\p2
.endif
.if	\cnt>3
	.short	\p3
.endif
.if	\cnt>4
	.short	\p4
.endif
.if	\cnt>5
	.short	\p5
.endif
.if	\cnt>6
	.short	\p6
.endif
.if	\cnt>7
	.short	\p7
.endif
	.endm

@SE_WAIT�}�N��(�t���O)
	.macro	SE_WAIT_FLAG
	.byte	WEST_SEWAIT_FLAG			@���߃R�[�h�i���o�[
	.endm

@WORKCHK_JP�}�N��
	.macro	WORKCHK_JP		soeji,num,adrs
	.byte	WEST_WORKCHK_JP			@���߃R�[�h�i���o�[
	.byte	\soeji
	.short	\num					@��r����l
	.long	\adrs
	.endm

@POKEBG_DROP�}�N��
	.macro	POKEBG_DROP			flag
	.byte	WEST_POKEBG_DROP
	.byte	\flag
	.endm

@POKEBG_DROP_RESET�}�N��
	.macro	POKEBG_DROP_RESET	flag
	.byte	WEST_POKEBG_DROP_RESET
	.byte	\flag
	.endm

@CONTEST_CHK_JP�}�N��
	.macro	CONTEST_CHK_JP	adrs
	.byte	WEST_CONTEST_CHK_JP
	.long	\adrs
	.endm

@HAIKEI_CHKCHG�}�N��
	.macro	HAIKEI_CHKCHG	mine_haikei,enemy_haikei,contest_haikei
	.byte	WEST_HAIKEI_CHKCHG
	.byte	\mine_haikei
	.byte	\enemy_haikei
	.byte	\contest_haikei
	.endm

@SEPAN_FLOWFIX�}�N��
	.macro	SEPAN_FLOWFIX	seID,start_pan,end_pan,add_pan,wait
	.byte	WEST_SEPAN_FLOWFIX					@���߃R�[�h�i���o�[
	.short	\seID
	.byte	\start_pan
	.byte	\end_pan
	.byte	\add_pan
	.byte	\wait
	.endm

@SEPAN_FLOW_AF�}�N��
	.macro	SEPAN_FLOW_AF	seID,start_pan,end_pan,add_pan,wait
	.byte	WEST_SEPAN_FLOW_AF					@���߃R�[�h�i���o�[
	.short	\seID
	.byte	\start_pan
	.byte	\end_pan
	.byte	\add_pan
	.byte	\wait
	.endm

@BGPRI_GAPSET�}�N��
	.macro	BGPRI_GAPSET		which
	.byte	WEST_BGPRI_GAPSET
	.byte	\which
	.endm

@BGPRI_GAPSET2�}�N��
	.macro	BGPRI_GAPSET2
	.byte	WEST_BGPRI_GAPSET2
	.endm

@BGPRI_GAPSET3�}�N��
	.macro	BGPRI_GAPSET3		which
	.byte	WEST_BGPRI_GAPSET3
	.byte	\which
	.endm

@POKE_BANISH_ON�}�N��
	.macro	POKE_BANISH_ON		client
	.byte	WEST_POKE_BANISH_ON
	.byte	\client
	.endm

@POKE_BANISH_OFF�}�N��
	.macro	POKE_BANISH_OFF		client
	.byte	WEST_POKE_BANISH_OFF
	.byte	\client
	.endm

@PARTY_ATTACK_BGOFF�}�N��
	.macro	PARTY_ATTACK_BGOFF		which
	.byte	WEST_PARTY_ATTACK_BGOFF
	.byte	\which
	.endm

@PARTY_ATTACK_BGEND�}�N��
	.macro	PARTY_ATTACK_BGEND		which
	.byte	WEST_PARTY_ATTACK_BGEND
	.byte	\which
	.endm

@SE_STOP�}�N��
	.macro	SE_STOP
	.byte	WEST_SE_STOP			@���߃R�[�h�i���o�[
	.endm

