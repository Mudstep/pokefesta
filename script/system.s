//=========================================================================
//
//	�V�X�e���p�X�N���v�g�t�@�C��
//
//=========================================================================

//-------------------------------------------------------------------------
//	�ȈՃ��b�Z�[�W�\���V�X�e��
//	<����>	���W�X�^�O�Ƀ��b�Z�[�W�A�h���X�������Ă���
//-------------------------------------------------------------------------
EasyObjMessageScript:
	_TALK_OBJ_START
	_TALKMSG	0x00
	_TALK_WAIT
	_AB_KEYWAIT
	_TALK_OBJ_END
	_RET

EasyBgMessageScript:
	_TALK_START
	_TALKMSG	0x00
	_TALK_WAIT
	_AB_KEYWAIT
	_TALK_END
	_RET

MessageWaitKeyScript:
	_TALKMSG	0x00
	_TALK_WAIT
	_AB_KEYWAIT
	_RET

MessageYesNoScript:
	_TALKMSG	0x00
	_TALK_WAIT
	_YES_NO_NORMAL_WIN
	_RET

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
PickItemScript:
	_RET

//-------------------------------------------------------------------------
//	�Z�[�u�̗���
//	<�߂�l>	ANSWORK
//	== TRUE		�Z�[�u����
//	== FALSE	�L�����Z���E�Z�[�u���s
//-------------------------------------------------------------------------
SCRIPT_LABEL	FieldSaveScript
	_PROGRAM	SAVE_INTERFACE
	_STOP_SCRIPT

	_RET


//=========================================================================
//
//		�g���[�i�[����X�N���v�g
//
//=========================================================================

//-------------------------------------------------------------------------
//�����q�b�g�ɂ�铮��J�n
//-------------------------------------------------------------------------
SCRIPT_LABEL	TrainerMoveScript

	_TRAINER_OBJ_START
	_PROGRAM			TR_BGM_SET
	_PROGRAM			TR_MOVE_CTRL
	_STOP_SCRIPT
	_JUMP				TrainerBattleScript


//-------------------------------------------------------------------------
// �g���[�i�[�ւ̘b���|��
//-------------------------------------------------------------------------
SCRIPT_LABEL	TrainerTalkScript

	_TALK_OBJ_START
	_SET_OBJ_ANIME		TARGET_OBJ,_trn_anm_appear
	_OBJ_ANIME_WAIT
	_FUNC_CALL			ANSWORK,TR_FLAG_CHECK
	_IFVAL_JUMP			ANSWORK,NE,0,_trn_after_fight

	_PROGRAM			TR_BGM_SET
	_PROGRAM			TR_STOP_OBJMOVE		@����R�[�h��Î~��Ԃɂ���
	_JUMP				TrainerBattleScript

//	���łɐ퓬�����ꍇ
_trn_after_fight:
	_TRAINER_AFTER
	

//-------------------------------------------------------------------------
//	2vs2�ΐ��p�g���[�i�[
//-------------------------------------------------------------------------
SCRIPT_LABEL	Trainer2vs2Script
	_TALK_OBJ_START
	_CALL				TrainerAppearScript
	_FUNC_CALL			ANSWORK,TR_FLAG_CHECK
	_IFVAL_JUMP			ANSWORK,NE,0,_trn_2vs_after_fight

	_PROGRAM			POKE2VS2_CHK
	_IFVAL_JUMP			ANSWORK,NE,0,_trn_2vs2_no_fight

	_PROGRAM			TR_BGM_SET
	_PROGRAM			TR_STOP_OBJMOVE		@����R�[�h��Î~��Ԃɂ���
	_JUMP				TrainerBattleScript

_trn_2vs2_no_fight:
	_PROGRAM			TR_NOFIGHT_MSG
	_TALK_WAIT
	_AB_KEYWAIT
	_TALK_OBJ_END
	_END

_trn_2vs_after_fight:
	_TRAINER_AFTER


//-------------------------------------------------------------------------
//	�C�x���g�g���[�i�[
//-------------------------------------------------------------------------
SCRIPT_LABEL	TrainerEventScript
	_SET_OBJ_ANIME		TARGET_OBJ,_trn_anm_appear
	_OBJ_ANIME_WAIT
	_PROGRAM			TR_BGM_SET

	_TRAINER_FIGHT

	_TRAINER_AFTER

//-------------------------------------------------------------------------
//	�Đ�g���[�i�[
//-------------------------------------------------------------------------
SCRIPT_LABEL	TrainerRevengeScript
	_CALL				TrainerAppearScript

	_FUNC_CALL			ANSWORK,TR_REV_FLAG_CHK
	_IFVAL_JUMP			ANSWORK,EQ,FALSE,_trn_after_revenge_fight

	_PROGRAM			TR_BGM_SET
	_PROGRAM			TR_STOP_OBJMOVE		@����R�[�h��Î~��Ԃɂ���
	_PROGRAM			TR_BEFORE_MSG
	_TALK_WAIT
	_AB_KEYWAIT
	_PROGRAM			TR_REV_FIGHT
	_STOP_SCRIPT

	/* �ΐ�ɏ������ꍇ�A�ȉ������s����� */
	_TALK_END
	_END

_trn_after_revenge_fight:
	_TRAINER_AFTER

//-------------------------------------------------------------------------
//	2vs2�Đ�g���[�i�[
//-------------------------------------------------------------------------
SCRIPT_LABEL	Trainer2vs2RevengeScript
	_FUNC_CALL			ANSWORK,TR_REV_FLAG_CHK
	_IFVAL_JUMP			ANSWORK,EQ,FALSE,_trn_after_2vs2revenge_fight

	_PROGRAM			POKE2VS2_CHK
	_IFVAL_JUMP			ANSWORK,NE,0,_trn_2vs2revenge_no_fight

	_PROGRAM			TR_BGM_SET
	_PROGRAM			TR_STOP_OBJMOVE		@����R�[�h��Î~��Ԃɂ���
	_PROGRAM			TR_BEFORE_MSG
	_TALK_WAIT
	_AB_KEYWAIT
	_PROGRAM			TR_REV_FIGHT
	_STOP_SCRIPT

	/* �ΐ�ɏ������ꍇ�A�ȉ������s����� */
	_TALK_END
	_END

_trn_after_2vs2revenge_fight:
	_TRAINER_AFTER

_trn_2vs2revenge_no_fight:
	_PROGRAM			TR_NOFIGHT_MSG
	_TALK_WAIT
	_AB_KEYWAIT
	_TALK_OBJ_END
	_END


//-------------------------------------------------------------------------
//	�B�ꖪ����
//-------------------------------------------------------------------------
TrainerAppearScript:
	_SET_OBJ_ANIME		TARGET_OBJ,_trn_anm_appear
	_OBJ_ANIME_WAIT
	_RET

//	�B���ԉ����A�j��
_trn_anm_appear:
	.byte	AC_HIDE_PULLOFF
	.byte	ACMD_END

//-------------------------------------------------------------------------
//	�g���[�i�[�ΐ폈��
//-------------------------------------------------------------------------
TrainerBattleScript:
	_PROGRAM			TR_BEFORE_MSG
	_TALK_WAIT
	_AB_KEYWAIT
	_TRAINER_FIGHT
	/* �ΐ�ɏ������ꍇ�A�ȉ������s����� */

	_FUNC_CALL			ANSWORK,TR_TYPE_GET
	_IFVAL_JUMP			ANSWORK,EQ,TR_TYPE_NORMAL,_normal_after_fight
	_IFVAL_JUMP			ANSWORK,EQ,TR_TYPE_EVENT,_event_after_fight
	_IFVAL_JUMP			ANSWORK,EQ,TR_TYPE_LEADER,_event_after_fight
	_IFVAL_JUMP			ANSWORK,EQ,TR_TYPE_EVENT2VS2,_event_after_fight
	_IFVAL_JUMP			ANSWORK,EQ,TR_TYPE_LEADER2VS2,_event_after_fight

_normal_after_fight:
	_TALK_END
	_END

_event_after_fight:
	_TRAINER_BATTLE_END_SCRIPT


//-------------------------------------------------------------------------
// �g���[�i�[�b���|���p
//-------------------------------------------------------------------------
SCRIPT_LABEL	TrainerEasyMsgScript
	_TALKMSG	0x00
	_TALK_WAIT
	_AB_KEYWAIT
	_TALK_OBJ_END
	_RET


