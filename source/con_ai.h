
@======================================================
@	
@======================================================

	.text
	.code 32

@======================================================
@	extern�錾
@======================================================

@======================================================
@	AI�̓���V�[�P���X����macro
@======================================================

	.MACRO	INIT_CMD
DEF_CMD_COUNT	=	0
	.ENDM

	.MACRO	DEF_CMD	symname
\symname	=	DEF_CMD_COUNT
DEF_CMD_COUNT 	=	(DEF_CMD_COUNT + 1)
	.ENDM

	INIT_CMD
	DEF_CMD	AI_C_INCDEC
	DEF_CMD	AI_C_LD_TURN
	DEF_CMD	AI_C_IF_TURN_UNDER
	DEF_CMD	AI_C_IF_TURN_OVER
	DEF_CMD	AI_C_IF_TURN_EQUAL
	DEF_CMD	AI_C_IFN_TURN_EQUAL
	DEF_CMD	AI_C_LD_TENSYON
	DEF_CMD	AI_C_IF_TENSYON_UNDER
	DEF_CMD	AI_C_IF_TENSYON_OVER
	DEF_CMD	AI_C_IF_TENSYON_EQUAL
	DEF_CMD	AI_C_IFN_TENSYON_EQUAL
	DEF_CMD	AI_C_LD_ORDER
	DEF_CMD	AI_C_IF_ORDER_UNDER
	DEF_CMD	AI_C_IF_ORDER_OVER
	DEF_CMD	AI_C_IF_ORDER_EQUAL
	DEF_CMD	AI_C_IFN_ORDER_EQUAL
	DEF_CMD	AI_C_LD_ORDER
	DEF_CMD	AI_C_IF_BONUS_UNDER
	DEF_CMD	AI_C_IF_BONUS_OVER
	DEF_CMD	AI_C_IF_BONUS_EQUAL
	DEF_CMD	AI_C_IFN_BONUS_EQUAL
	DEF_CMD	AI_C_LD_AP
	DEF_CMD	AI_C_IF_AP_UNDER
	DEF_CMD	AI_C_IF_AP_OVER
	DEF_CMD	AI_C_IF_AP_EQUAL
	DEF_CMD	AI_C_IFN_AP_EQUAL
	DEF_CMD	AI_C_LD_BP
	DEF_CMD	AI_C_IF_BP_UNDER
	DEF_CMD	AI_C_IF_BP_OVER
	DEF_CMD	AI_C_IF_BP_EQUAL
	DEF_CMD	AI_C_IFN_BP_EQUAL
	DEF_CMD	AI_C_LD_CONTYPE
	DEF_CMD	AI_C_IF_CONTYPE_EQUAL
	DEF_CMD	AI_C_IFN_CONTYPE_EQUAL
	DEF_CMD	AI_C_LD_WAZATENSYON
	DEF_CMD	AI_C_IF_WAZATENSYON_UNDER
	DEF_CMD	AI_C_IF_WAZATENSYON_OVER
	DEF_CMD	AI_C_IF_WAZATENSYON_EQUAL
	DEF_CMD	AI_C_IFN_WAZATENSYON_EQUAL
	DEF_CMD	AI_C_LD_APNO
	DEF_CMD	AI_C_IF_APNO_EQUAL
	DEF_CMD	AI_C_IFN_APNO_EQUAL
	DEF_CMD	AI_C_LD_WAZACLASS
	DEF_CMD	AI_C_IF_WAZACLASS_EQUAL
	DEF_CMD	AI_C_IFN_WAZACLASS_EQUAL
	DEF_CMD	AI_C_LD_APTOP
	DEF_CMD	AI_C_IF_APTOP_EQUAL
	DEF_CMD	AI_C_LD_BOUGAITOP
	DEF_CMD	AI_C_IF_BOUGAITOP_EQUAL
	DEF_CMD	AI_C_LD_WAZAAP
	DEF_CMD	AI_C_IF_WAZAAP_UNDER
	DEF_CMD	AI_C_IF_WAZAAP_OVER
	DEF_CMD	AI_C_IF_WAZAAP_EQUAL
	DEF_CMD	AI_C_IFN_WAZAAP_EQUAL
	DEF_CMD	AI_C_LD_BOUGAI
	DEF_CMD	AI_C_IF_BOUGAI_UNDER
	DEF_CMD	AI_C_IF_BOUGAI_OVER
	DEF_CMD	AI_C_IF_BOUGAI_EQUAL
	DEF_CMD	AI_C_IFN_BOUGAI_EQUAL
	DEF_CMD	AI_C_LD_CONTI
	DEF_CMD	AI_C_IF_CONTI_UNDER
	DEF_CMD	AI_C_IF_CONTI_OVER
	DEF_CMD	AI_C_IF_CONTI_EQUAL
	DEF_CMD	AI_C_IFN_CONTI_EQUAL
	DEF_CMD	AI_C_LD_COMBO1
	DEF_CMD	AI_C_IF_COMBO1_EQUAL
	DEF_CMD	AI_C_IFN_COMBO1_EQUAL
	DEF_CMD	AI_C_LD_COMBO2
	DEF_CMD	AI_C_IF_COMBO2_EQUAL
	DEF_CMD	AI_C_IFN_COMBO2_EQUAL
	DEF_CMD	AI_C_LD_COMBOOCCUR
	DEF_CMD	AI_C_IF_COMBOOCCUR_EQUAL
	DEF_CMD	AI_C_IFN_COMBOOCCUR_EQUAL
	DEF_CMD	AI_C_LD_RIVALBONUS
	DEF_CMD	AI_C_IF_RIVALBONUS_UNDER
	DEF_CMD	AI_C_IF_RIVALBONUS_OVER
	DEF_CMD	AI_C_IF_RIVALBONUS_EQUAL
	DEF_CMD	AI_C_IFN_RIVALBONUS_EQUAL
	DEF_CMD	AI_C_LD_RIVALCOMBO1
	DEF_CMD	AI_C_IF_RIVALCOMBO1_UNDER
	DEF_CMD	AI_C_IF_RIVALCOMBO1_OVER
	DEF_CMD	AI_C_IF_RIVALCOMBO1_EQUAL
	DEF_CMD	AI_C_IFN_RIVALCOMBO1_EQUAL
	DEF_CMD	AI_C_LD_STATUS
	DEF_CMD	AI_C_IF_STATUS_EQUAL
	DEF_CMD	AI_C_IFN_STATUS_EQUAL
	DEF_CMD	AI_C_LD_COMBOCONTI
	DEF_CMD	AI_C_IF_COMBOCONTI_EQUAL
	DEF_CMD	AI_C_IFN_COMBOCONTI_EQUAL
	DEF_CMD	AI_C_LD_APCOMP
	DEF_CMD	AI_C_IF_APCOMP_UNDER
	DEF_CMD	AI_C_IF_APCOMP_OVER
	DEF_CMD	AI_C_IF_APCOMP_EQUAL
	DEF_CMD	AI_C_IFN_APCOMP_EQUAL
	DEF_CMD	AI_C_LD_BPCOMP
	DEF_CMD	AI_C_IF_BPCOMP_UNDER
	DEF_CMD	AI_C_IF_BPCOMP_OVER
	DEF_CMD	AI_C_IF_BPCOMP_EQUAL
	DEF_CMD	AI_C_IFN_BPCOMP_EQUAL
	DEF_CMD	AI_C_LD_APNOHISTORY
	DEF_CMD	AI_C_IF_APNOHISTORY_UNDER
	DEF_CMD	AI_C_IF_APNOHISTORY_OVER
	DEF_CMD	AI_C_IF_APNOHISTORY_EQUAL
	DEF_CMD	AI_C_IFN_APNOHISTORY_EQUAL
	DEF_CMD	AI_C_LD_TENSYONHISTORY
	DEF_CMD	AI_C_IF_TENSYONHISTORY_UNDER
	DEF_CMD	AI_C_IF_TENSYONHISTORY_OVER
	DEF_CMD	AI_C_IF_TENSYONHISTORY_EQUAL
	DEF_CMD	AI_C_IFN_TENSYONHISTORY_EQUAL
	DEF_CMD	AI_C_LD_CLASSHISTORY
	DEF_CMD	AI_C_IF_CLASSHISTORY_EQUAL
	DEF_CMD	AI_C_IFN_CLASSHISTORY_EQUAL
	DEF_CMD	AI_C_LDWK_RESURT
	DEF_CMD	AI_C_LDWK_NUM
	DEF_CMD	AI_C_ADDSUBLDWK_NUM
	DEF_CMD	AI_C_ADDSUBLDWK_WK
	DEF_CMD	AI_C_LDWK_WK
	DEF_CMD	AI_C_IF_WKCOMPNUM_UNDER
	DEF_CMD	AI_C_IF_WKCOMPNUM_OVER
	DEF_CMD	AI_C_IF_WKCOMPNUM_EQUAL
	DEF_CMD	AI_C_IFN_WKCOMPNUM_EQUAL
	DEF_CMD	AI_C_IF_WKCOMPWK_UNDER
	DEF_CMD	AI_C_IF_WKCOMPWK_OVER
	DEF_CMD	AI_C_IF_WKCOMPWK_EQUAL
	DEF_CMD	AI_C_IFN_WKCOMPWK_EQUAL
	DEF_CMD	AI_C_IF_RANDCOMPNUM_UNDER
	DEF_CMD	AI_C_IF_RANDCOMPNUM_OVER
	DEF_CMD	AI_C_JUMP
	DEF_CMD	AI_C_CALL
	DEF_CMD	AI_C_AIEND
	DEF_CMD	AI_C_LD_ALLWAZA_TENSYONCHECK
	DEF_CMD AI_C_IF_ALLWAZA_TENSYONCHECK_EQUAL
	DEF_CMD AI_C_IFN_ALLWAZA_TENSYONCHECK_EQUAL
	DEF_CMD	AI_C_LD_WAZANOCHECK
	DEF_CMD	AI_C_IF_WAZANOCHECK_EQUAL
	DEF_CMD	AI_C_IFN_WAZANOCHECK_EQUAL


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�ɑ΂��ă|�C���g�����Z
@
@	value:�����Z����l(s8)
@
@------------------------------------------------------------

	.macro		C_INCDEC				value
	.byte		AI_C_INCDEC
	.byte		\value
	.endm


@------------------------------------------------------------
@
@	���݂̃^�[�����ŕ���
@
@	value:��r����l(u8:0�`4)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_TURN
	.byte		AI_C_LD_TURN
	.endm

	.macro		C_IF_TURN_UNDER		value,adrs
	.byte		AI_C_IF_TURN_UNDER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_TURN_OVER		value,adrs
	.byte		AI_C_IF_TURN_OVER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_TURN_EQUAL		value,adrs
	.byte		AI_C_IF_TURN_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_TURN_EQUAL	value,adrs
	.byte		AI_C_IFN_TURN_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	���݂̏�̃e���V�����ŕ���
@
@	value:��r����l(s8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_TENSYON
	.byte		AI_C_LD_TENSYON
	.endm

	.macro		C_IF_TENSYON_UNDER		value,adrs
	.byte		AI_C_IF_TENSYON_UNDER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_TENSYON_OVER		value,adrs
	.byte		AI_C_IF_TENSYON_OVER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_TENSYON_EQUAL		value,adrs
	.byte		AI_C_IF_TENSYON_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_TENSYON_EQUAL		value,adrs
	.byte		AI_C_IFN_TENSYON_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�����̃A�s�[�����鏇�Ԃŕ���
@
@	value:��r����l(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_ORDER
	.byte		AI_C_LD_ORDER
	.endm

	.macro		C_IF_ORDER_UNDER		value,adrs
	.byte		AI_C_IF_ORDER_UNDER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_ORDER_OVER		value,adrs
	.byte		AI_C_IF_ORDER_OVER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_ORDER_EQUAL		value,adrs
	.byte		AI_C_IF_ORDER_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_ORDER_EQUAL		value,adrs
	.byte		AI_C_IFN_ORDER_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�����̃{�[�i�X��Ԃŕ���
@
@	value:��r����l(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_BONUS
	.byte		AI_C_LD_BONUS
	.endm

	.macro		C_IF_BONUS_UNDER		value,adrs
	.byte		AI_C_IF_BONUS_UNDER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_BONUS_OVER		value,adrs
	.byte		AI_C_IF_BONUS_OVER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_BONUS_EQUAL		value,adrs
	.byte		AI_C_IF_BONUS_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_BONUS_EQUAL		value,adrs
	.byte		AI_C_IFN_BONUS_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	������AP�l���擾���ĕ���
@
@	value:��r����l(s16)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_AP
	.byte		AI_C_LD_AP
	.endm

	.macro		C_IF_AP_UNDER		value,adrs
	.byte		AI_C_IF_AP_UNDER
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IF_AP_OVER		value,adrs
	.byte		AI_C_IF_AP_OVER
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IF_AP_EQUAL		value,adrs
	.byte		AI_C_IF_AP_EQUAL
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IFN_AP_EQUAL		value,adrs
	.byte		AI_C_IFN_AP_EQUAL
	.short		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	������BP�l�ŕ���
@
@	value:��r����l(s16)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_BP
	.byte		AI_C_LD_BP
	.endm

	.macro		C_IF_BP_UNDER		value,adrs
	.byte		AI_C_IF_BP_UNDER
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IF_BP_OVER		value,adrs
	.byte		AI_C_IF_BP_OVER
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IF_BP_EQUAL		value,adrs
	.byte		AI_C_IF_BP_EQUAL
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IFN_BP_EQUAL		value,adrs
	.byte		AI_C_IFN_BP_EQUAL
	.short		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�o�ꂵ�Ă���R���e�X�g�^�C�v�ŕ���
@
@	value:��r�����ý�����(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_CONTYPE
	.byte		AI_C_LD_CONTYPE
	.endm

	.macro		C_IF_CONTYPE_EQUAL		value,adrs
	.byte		AI_C_IF_CONTYPE_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_CONTYPE_EQUAL		value,adrs
	.byte		AI_C_IFN_CONTYPE_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z���o�������ɕω�����e���V�����l�ŕ���
@
@	value:��r�����ý�����(s8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_WAZATENSYON
	.byte		AI_C_LD_WAZATENSYON
	.endm

	.macro		C_IF_WAZATENSYON_UNDER		value,adrs
	.byte		AI_C_IF_WAZATENSYON_UNDER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_WAZATENSYON_OVER		value,adrs
	.byte		AI_C_IF_WAZATENSYON_OVER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_WAZATENSYON_EQUAL		value,adrs
	.byte		AI_C_IF_WAZATENSYON_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_WAZATENSYON_EQUAL		value,adrs
	.byte		AI_C_IFN_WAZATENSYON_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�̃A�s�[��No���擾���ĕ���
@
@	value:��r����l(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_APNO
	.byte		AI_C_LD_APNO
	.endm

	.macro		C_IF_APNO_EQUAL		value,adrs
	.byte		AI_C_IF_APNO_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_APNO_EQUAL		value,adrs
	.byte		AI_C_IFN_APNO_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�̃A�s�[�����ނ��擾���ĕ���
@
@	value:��r����l(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_WAZACLASS
	.byte		AI_C_LD_WAZACLASS
	.endm

	.macro		C_IF_WAZACLASS_EQUAL		value,adrs
	.byte		AI_C_IF_WAZACLASS_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_WAZACLASS_EQUAL		value,adrs
	.byte		AI_C_IFN_WAZACLASS_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�������Ă���Z�̒��ň��AP�l���������ǂ����`�F�b�N����
@
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_APTOP
	.byte		AI_C_LD_APTOP
	.endm

	.macro		C_IF_APTOP_EQUAL		adrs
	.byte		AI_C_IF_APTOP_EQUAL
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�������Ă���Z�̒��ň�ԖW�Q�l���������ǂ����`�F�b�N����
@
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_BOUGAITOP
	.byte		AI_C_LD_BOUGAITOP
	.endm

	.macro		C_IF_BOUGAITOP_EQUAL		value,adrs
	.byte		AI_C_IF_BOUGAITOP_EQUAL
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�̃A�s�[���l���擾���ĕ���
@
@	value:��r����l(s8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_WAZAAP
	.byte		AI_C_LD_WAZAAP
	.endm

	.macro		C_IF_WAZAAP_UNDER		value,adrs
	.byte		AI_C_IF_WAZAAP_UNDER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_WAZAAP_OVER		value,adrs
	.byte		AI_C_IF_WAZAAP_OVER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_WAZAAP_EQUAL		value,adrs
	.byte		AI_C_IF_WAZAAP_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_WAZAAP_EQUAL		value,adrs
	.byte		AI_C_IFN_WAZAAP_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�̖W�Q�l���擾���ĕ���
@
@	value:��r����l(s8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_BOUGAI
	.byte		AI_C_LD_BOUGAI
	.endm

	.macro		C_IF_BOUGAI_UNDER		value,adrs
	.byte		AI_C_IF_BOUGAI_UNDER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_BOUGAI_OVER		value,adrs
	.byte		AI_C_IF_BOUGAI_OVER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_BOUGAI_EQUAL		value,adrs
	.byte		AI_C_IF_BOUGAI_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_BOUGAI_EQUAL		value,adrs
	.byte		AI_C_IFN_BOUGAI_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�̘A���g�p�񐔂��擾���ĕ���
@
@	value:��r����l(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_CONTI
	.byte		AI_C_LD_CONTI
	.endm

	.macro		C_IF_CONTI_UNDER		value,adrs
	.byte		AI_C_IF_CONTI_UNDER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_CONTI_OVER		value,adrs
	.byte		AI_C_IF_CONTI_OVER
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_CONTI_EQUAL		value,adrs
	.byte		AI_C_IF_CONTI_EQUAL
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_CONTI_EQUAL		value,adrs
	.byte		AI_C_IFN_CONTI_EQUAL
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�Ƒ��̎����Z���r���ĺ��ޏo���邩�ǂ����������镪��1
@
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_COMBO1
	.byte		AI_C_LD_COMBO1
	.endm

	.macro		C_IF_COMBO1_EQUAL		adrs
	.byte		AI_C_IF_COMBO1_EQUAL
	.long		\adrs
	.endm

	.macro		C_IFN_COMBO1_EQUAL		adrs
	.byte		AI_C_IFN_COMBO1_EQUAL
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�Ƒ��̎����Z���r���ĺ��ޏo���邩�ǂ����������镪��2
@
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_COMBO2
	.byte		AI_C_LD_COMBO2
	.endm

	.macro		C_IF_COMBO2_EQUAL		adrs
	.byte		AI_C_IF_COMBO2_EQUAL
	.long		\adrs
	.endm

	.macro		C_IFN_COMBO2_EQUAL		adrs
	.byte		AI_C_IFN_COMBO2_EQUAL
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�[�`���Ă���Z�ź��ނ��������邩�ŕ���
@
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_COMBOOCCUR
	.byte		AI_C_LD_COMBOOCCUR
	.endm

	.macro		C_IF_COMBOOCCUR_EQUAL		adrs
	.byte		AI_C_IF_COMBOOCCUR_EQUAL
	.long		\adrs
	.endm

	.macro		C_IFN_COMBOOCCUR_EQUAL		adrs
	.byte		AI_C_IFN_COMBOOCCUR_EQUAL
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̃{�[�i�X��Ԃ��`�F�b�N
@
@	appealno:�A�s�[���ԍ�(u8)
@	value:��r����l(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_RIVALBONUS
	.byte		AI_C_LD_RIVALBONUS
	.byte		\appealno
	.endm

	.macro		C_IF_RIVALBONUS_UNDER		appealno,value,adrs
	.byte		AI_C_IF_RIVALBONUS_UNDER
	.byte		\appealno
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_RIVALBONUS_OVER		appealno,value,adrs
	.byte		AI_C_IF_RIVALBONUS_OVER
	.byte		\appealno
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_RIVALBONUS_EQUAL		appealno,value,adrs
	.byte		AI_C_IF_RIVALBONUS_EQUAL
	.byte		\appealno
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_RIVALBONUS_EQUAL		appealno,value,adrs
	.byte		AI_C_IFN_RIVALBONUS_EQUAL
	.byte		\appealno
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̃R���{1��Ԃ��`�F�b�N
@
@	appealno:�A�s�[���ԍ�(u8)
@	value:��r����l(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_RIVALCOMBO1
	.byte		AI_C_LD_RIVALCOMBO1
	.byte		\appealno
	.endm

	.macro		C_IF_RIVALCOMBO1_UNDER		appealno,value,adrs
	.byte		AI_C_IF_RIVALCOMBO1_UNDER
	.byte		\appealno
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_RIVALCOMBO1_OVER		appealno,value,adrs
	.byte		AI_C_IF_RIVALCOMBO1_OVER
	.byte		\appealno
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IF_RIVALCOMBO1_EQUAL		appealno,value,adrs
	.byte		AI_C_IF_RIVALCOMBO1_EQUAL
	.byte		\appealno
	.byte		\value
	.long		\adrs
	.endm

	.macro		C_IFN_RIVALCOMBO1_EQUAL		appealno,value,adrs
	.byte		AI_C_IFN_RIVALCOMBO1_EQUAL
	.byte		\appealno
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̏�Ԃ��`�F�b�N
@	�Q���ł����ԁ��P	�Q���ł��Ȃ����(�x��)���O
@
@	appealno:�A�s�[���ԍ�(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_STATUS
	.byte		AI_C_LD_STATUS
	.byte		\appealno
	.endm

	@@-- �Q���ł���Ȃ�W�����v --@@
	.macro		C_IF_STATUS_EQUAL		appealno,adrs
	.byte		AI_C_IF_STATUS_EQUAL
	.byte		\appealno
	.long		\adrs
	.endm

	@@-- �Q���ł��Ȃ��Ȃ�W�����v --@@
	.macro		C_IFN_STATUS_EQUAL		appealno,adrs
	.byte		AI_C_IFN_STATUS_EQUAL
	.byte		\appealno
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̃R���{�����`�F�b�N
@
@	appealno:�A�s�[���ԍ�(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_COMBOCONTI
	.byte		AI_C_LD_COMBOCONTI
	.byte		\appealno
	.endm

	@@-- �����������Ƃ����� --@@
	.macro		C_IF_COMBOCONTI_EQUAL		appealno,adrs
	.byte		AI_C_IF_COMBOCONTI_EQUAL
	.byte		\appealno
	.long		\adrs
	.endm

	@@-- �����������Ƃ��Ȃ� --@@
	.macro		C_IFN_COMBOCONTI_EQUAL		appealno,adrs
	.byte		AI_C_IFN_COMBOCONTI_EQUAL
	.byte		\appealno
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̌��݂�AP�l�Ǝ�����AP�l���r����
@
@	appealno:�A�s�[���ԍ�(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	@@-- ������AP�l�̕����傫����΃}�C�i�X�Ō��ʂ����� --@@
	.macro		C_LD_APCOMP
	.byte		AI_C_LD_APCOMP
	.byte		\appealno
	.endm

	@@-- �����̕����傫����΃W�����v --@@
	.macro		C_IF_APCOMP_UNDER		appealno,adrs
	.byte		AI_C_IF_APCOMP_UNDER
	.byte		\appealno
	.long		\adrs
	.endm

	@@-- �����̕����Ⴏ��΃W�����v --@@
	.macro		C_IF_APCOMP_OVER		appealno,adrs
	.byte		AI_C_IF_APCOMP_OVER
	.byte		\appealno
	.long		\adrs
	.endm

	.macro		C_IF_APCOMP_EQUAL		appealno,adrs
	.byte		AI_C_IF_APCOMP_EQUAL
	.byte		\appealno
	.long		\adrs
	.endm

	.macro		C_IFN_APCOMP_EQUAL		appealno,adrs
	.byte		AI_C_IFN_APCOMP_EQUAL
	.byte		\appealno
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̌��݂�BP�l�Ǝ�����BP�l���r����
@
@	appealno:�A�s�[���ԍ�(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	@@-- ������AP�l�̕����傫����΃}�C�i�X�Ō��ʂ����� --@@
	.macro		C_LD_BPCOMP
	.byte		AI_C_LD_BPCOMP
	.endm

	@@-- �����̕����傫����΃W�����v --@@
	.macro		C_IF_BPCOMP_UNDER		appealno,adrs
	.byte		AI_C_IF_BPCOMP_UNDER
	.byte		\appealno
	.long		\adrs
	.endm

	@@-- �����̕����Ⴏ��΃W�����v --@@
	.macro		C_IF_BPCOMP_OVER		appealno,adrs
	.byte		AI_C_IF_BPCOMP_OVER
	.byte		\appealno
	.long		\adrs
	.endm

	.macro		C_IF_BPCOMP_EQUAL		appealno,adrs
	.byte		AI_C_IF_BPCOMP_EQUAL
	.byte		\appealno
	.long		\adrs
	.endm

	.macro		C_IFN_BPCOMP_EQUAL		appealno,adrs
	.byte		AI_C_IFN_BPCOMP_EQUAL
	.byte		\appealno
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̑O�̃^�[���܂ł̋Z����A�A�s�[��No���擾���ĕ���
@
@	appealno:�A�s�[���ԍ�(u8)
@	turn:���^�[���ڂ̃f�[�^�����邩(u8)
@	apno:��r����APNo(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_APNOHISTORY			appealno,turn
	.byte		AI_C_LD_APNOHISTORY
	.byte		\appealno
	.byte		\turn
	.endm

	.macro		C_IF_APNOHISTORY_UNDER		appealno,turn,apno,adrs
	.byte		AI_C_IF_APNOHISTORY_UNDER
	.byte		\appealno
	.byte		\turn
	.byte		\apno
	.long		\adrs
	.endm

	.macro		C_IF_APNOHISTORY_OVER		appealno,turn,apno,adrs
	.byte		AI_C_IF_APNOHISTORY_OVER
	.byte		\appealno
	.byte		\turn
	.byte		\apno
	.long		\adrs
	.endm

	.macro		C_IF_APNOHISTORY_EQUAL		appealno,turn,apno,adrs
	.byte		AI_C_IF_APNOHISTORY_EQUAL
	.byte		\appealno
	.byte		\turn
	.byte		\apno
	.long		\adrs
	.endm

	.macro		C_IFN_APNOHISTORY_EQUAL		appealno,turn,apno,adrs
	.byte		AI_C_IFN_APNOHISTORY_EQUAL
	.byte		\appealno
	.byte		\turn
	.byte		\apno
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̑O�̃^�[���܂ł̋Z����A��߰����߂ŕω������ݼ�ݒl���擾���ĕ���
@
@	appealno:�A�s�[���ԍ�(u8)
@	turn:���^�[���ڂ̃f�[�^�����邩(u8)
@	tensyon:��r����APNo(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_TENSYONHISTORY			appealno,turn
	.byte		AI_C_LD_TENSYONHISTORY
	.byte		\appealno
	.byte		\turn
	.endm

	.macro		C_IF_TENSYONHISTORY_UNDER		appealno,turn,tensyon,adrs
	.byte		AI_C_IF_TENSYONHISTORY_UNDER
	.byte		\appealno
	.byte		\turn
	.byte		\tensyon
	.long		\adrs
	.endm

	.macro		C_IF_TENSYONHISTORY_OVER		appealno,turn,tensyon,adrs
	.byte		AI_C_IF_TENSYONHISTORY_OVER
	.byte		\appealno
	.byte		\turn
	.byte		\tensyon
	.long		\adrs
	.endm

	.macro		C_IF_TENSYONHISTORY_EQUAL		appealno,turn,tensyon,adrs
	.byte		AI_C_IF_TENSYONHISTORY_EQUAL
	.byte		\appealno
	.byte		\turn
	.byte		\tensyon
	.long		\adrs
	.endm

	.macro		C_IFN_TENSYONHISTORY_EQUAL		appealno,turn,tensyon,adrs
	.byte		AI_C_IFN_TENSYONHISTORY_EQUAL
	.byte		\appealno
	.byte		\turn
	.byte		\tensyon
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�e�|�P�����̑O�̃^�[���܂ł̋Z����A�A�s�[�����ނ��擾���ĕ���
@
@	appealno:�A�s�[���ԍ�(u8)
@	turn:���^�[���ڂ̃f�[�^�����邩(u8)
@	class:��r����APNo(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_CLASSHISTORY			appealno,turn
	.byte		AI_C_LD_CLASSHISTORY
	.byte		\appealno
	.byte		\turn
	.endm

	.macro		C_IF_CLASSHISTORY_UNDER		appealno,turn,class,adrs
	.byte		AI_C_IF_CLASSHISTORY_UNDER
	.byte		\appealno
	.byte		\turn
	.byte		\class
	.long		\adrs
	.endm

	.macro		C_IF_CLASSHISTORY_OVER		appealno,turn,class,adrs
	.byte		AI_C_IF_CLASSHISTORY_OVER
	.byte		\appealno
	.byte		\turn
	.byte		\class
	.long		\adrs
	.endm

	.macro		C_IF_CLASSHISTORY_EQUAL		appealno,turn,class,adrs
	.byte		AI_C_IF_CLASSHISTORY_EQUAL
	.byte		\appealno
	.byte		\turn
	.byte		\class
	.long		\adrs
	.endm

	.macro		C_IFN_CLASSHISTORY_EQUAL		appealno,turn,class,adrs
	.byte		AI_C_IFN_CLASSHISTORY_EQUAL
	.byte		\appealno
	.byte		\turn
	.byte		\class
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	���ʂ����[�N�ɑ������
@
@	work:�����̃��[�N
@
@------------------------------------------------------------

	.macro		C_LDWK_RESULT		work
	.byte		AI_C_LDWK_RESULT
	.byte		\work
	.endm


@------------------------------------------------------------
@
@	���[�N�ɒl��������
@
@	work:�����̃��[�N
@	value:�������l(s16)
@
@------------------------------------------------------------

	.macro		C_LDWK_NUM		work,value
	.byte		AI_C_LDWK_NUM
	.byte		\work
	.short		\value
	.endm


@------------------------------------------------------------
@
@	���[�N�ɒl�������Z����
@
@	work:�����Z��̃��[�N
@	value:�����Z����l(s16)
@
@------------------------------------------------------------

	.macro		C_ADDSUBLDWK_NUM		work,value
	.byte		AI_C_ADDSUBLDWK_NUM
	.byte		\work
	.short		\value
	.endm


@------------------------------------------------------------
@
@	���[�NA�ƃ��[�NB�������Z���ă��[�NA�ɑ������
@
@	workA:���[�NA
@	workB:���[�NB
@
@------------------------------------------------------------

	.macro		C_ADDSUBLDWK_WK		work,work
	.byte		AI_C_ADDSUBLDWK_WK
	.byte		\workA
	.byte		\workB
	.endm


@------------------------------------------------------------
@
@	���[�NA�Ƀ��[�NB��������
@
@	workA:���[�NA
@	workB:���[�NB
@
@------------------------------------------------------------

	.macro		C_LDWK_WK		workA,workB
	.byte		AI_C_LDWK_WK
	.byte		\workA
	.byte		\workB
	.endm


@------------------------------------------------------------
@
@	���[�N�ƒl���r���ĕ���
@
@	work:��r���郏�[�N
@	value:��r����l(s16)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_IF_WKCOMPNUM_UNDER		work,value,adrs
	.byte		AI_C_IF_WKCOMPNUM_UNDER
	.byte		\work
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IF_WKCOMPNUM_OVER			work,value,adrs
	.byte		AI_C_IF_WKCOMPNUM_OVER
	.byte		\work
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IF_WKCOMPNUM_EQUAL		work,value,adrs
	.byte		AI_C_IF_WKCOMPNUM_EQUAL
	.byte		\work
	.short		\value
	.long		\adrs
	.endm

	.macro		C_IFN_WKCOMPNUM_EQUAL		work,value,adrs
	.byte		AI_C_IFN_WKCOMPNUM_EQUAL
	.byte		\work
	.short		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	���[�N�ƃ��[�N���r���ĕ���
@
@	workA:��r���郏�[�N
@	workB:��r���郏�[�N
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_IF_WKCOMPWK_UNDER		workA,workB,adrs
	.byte		AI_C_IF_WKCOMPWK_UNDER
	.byte		\workA
	.byte		\workB
	.long		\adrs
	.endm

	.macro		C_IF_WKCOMPWK_OVER			workA,workB,adrs
	.byte		AI_C_IF_WKCOMPWK_OVER
	.byte		\workA
	.byte		\workB
	.long		\adrs
	.endm

	.macro		C_IF_WKCOMPWK_EQUAL		workA,workB,adrs
	.byte		AI_C_IF_WKCOMPWK_EQUAL
	.byte		\workA
	.byte		\workB
	.long		\adrs
	.endm

	.macro		C_IFN_WKCOMPWK_EQUAL		workA,workB,adrs
	.byte		AI_C_IFN_WKCOMPWK_EQUAL
	.byte		\workA
	.byte		\workB
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�����_���l���擾���ĕ���
@	0�`255�͈̔͂Ń����_���l���擾���āA�l�Ɣ�r������
@
@	value:��r����l(u8)
@	adrs:��ѐ�
@
@------------------------------------------------------------

	@@-- �����_���l�̕����Ⴏ��Ε��� --@@
	.macro		C_IF_RANDCOMPNUM_UNDER		value,adrs
	.byte		AI_C_IF_RANDCOMPNUM_UNDER
	.byte		\value
	.long		\adrs
	.endm

	@@-- �����_���l�̕���������Ε��� --@@
	.macro		C_IF_RANDCOMPNUM_OVER		value,adrs
	.byte		AI_C_IF_RANDCOMPNUM_OVER
	.byte		\value
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�������ŃW�����v����
@
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_JUMP			adrs
	.byte		AI_C_JUMP
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�T�u���[�`���Ăяo��
@
@	adrs:�Ăяo����
@
@------------------------------------------------------------

	.macro		C_CALL			adrs
	.byte		AI_C_CALL
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�I���R�[�h & �T�u���[�`���I�[�R�[�h
@
@------------------------------------------------------------

	.macro		C_AIEND
	.byte		AI_C_AIEND
	.endm


@------------------------------------------------------------
@
@	�莝���̋Z�̒����ݼ�݂��グ��Z�����邩���ׂĕ���
@
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_ALLWAZA_TENSYONCHECK
	.byte		AI_C_LD_ALLWAZA_TENSYONCHECK
	.endm

	@@-- �莝���̋Z���ݼ�݂�������Z������Ȃ番�� --@@
	.macro		C_IF_ALLWAZA_TENSYONCHECK_EQUAL		adrs
	.byte		AI_C_IF_ALLWAZA_TENSYONCHECK_EQUAL
	.long		\adrs
	.endm

	@@-- �莝���̋Z���ݼ�݂�������Z���Ȃ��Ȃ番�� --@@
	.macro		C_IFN_ALLWAZA_TENSYONCHECK_EQUAL		adrs
	.byte		AI_C_IFN_ALLWAZA_TENSYONCHECK_EQUAL
	.long		\adrs
	.endm


@------------------------------------------------------------
@
@	�ZNo��n���Ď莝���̋Z�̒��ɂ��邩�ǂ����ŕ���
@
@	wazano:�T�[�`����ZNo
@	adrs:��ѐ�
@
@------------------------------------------------------------

	.macro		C_LD_WAZANOCHECK			wazano
	.byte		AI_C_LD_WAZANOCHECK
	.short		\wazano
	.endm

	@@-- �莝���̋Z�Ɏw�肵���Z������Ȃ番�� --@@
	.macro		C_IF_WAZANOCHECK_EQUAL		wazano,adrs
	.byte		AI_C_IF_WAZANOCHECK_EQUAL
	.short		\wazano
	.long		\adrs
	.endm

	@@-- �莝���̋Z�Ɏw�肵���Z���Ȃ��Ȃ番�� --@@
	.macro		C_IFN_WAZANOCHECK_EQUAL		wazano,adrs
	.byte		AI_C_IFN_WAZANOCHECK_EQUAL
	.short		\wazano
	.long		\adrs
	.endm

