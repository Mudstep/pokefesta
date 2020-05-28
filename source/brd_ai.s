@----------------------------------------------------
@
@	��ذ�ްAI
@						2002.05.24(��) by matsuda
@----------------------------------------------------

	.text
	.code 32

	.include	"con_ai.h"

@======================================================
@	define�錾
@======================================================

@======================================================
@	global�錾
@======================================================

	.global		BreederAITbl

#include	"wazano.def"
#include	"con_ai.def"
#include	"agbmons.def"

@AI�V�[�P���X�e�[�u���i�Ƃ肠����32�e�[�u���j
BreederAITbl:
	.long		ConExpAISeq				@01�G�L�X�p�[�gAI
	.long		ConComboAISeq			@02�R���{	
	.long		ConPtAISeq				@03�y�i���e�B
	.long		TensyonAISeq			@04�G�L�T�C�g 
	.long		ConNomverAISeq			@05����
	.long		ConWazaProAISeq			@06�Z��p	
	.long		ConBougaiAISeq			@07�W�Q
	.long		ConDummyAISeq			@08
	.long		ConDummyAISeq			@09
	.long		ConDummyAISeq			@10
	.long		ConDummyAISeq			@11
	.long		ConDummyAISeq			@12
	.long		ConDummyAISeq			@13
	.long		ConDummyAISeq			@14
	.long		ConDummyAISeq			@15
	.long		ConDummyAISeq			@16
	.long		ConDummyAISeq			@17
	.long		ConDummyAISeq			@18
	.long		ConDummyAISeq			@19
	.long		ConDummyAISeq			@20
	.long		ConDummyAISeq			@21
	.long		ConDummyAISeq			@22
	.long		ConDummyAISeq			@23
	.long		ConDummyAISeq			@24
	.long		ConDummyAISeq			@25
	.long		ConDummyAISeq			@26
	.long		ConDummyAISeq			@27
	.long		ConDummyAISeq			@28	
	.long		ConDummyAISeq			@29	
	.long		ConDummyAISeq			@30	
	.long		ConDummyAISeq			@31	
	.long		ConDummyAISeq			@32	

@========================================================
@	�e�X�gAI
@========================================================

TestAISeq:
	C_IFN_TURN_EQUAL		0,TestNext		//�ŏ�����݂łȂ��Ȃ炱���̏����͔�΂�
	C_IFN_APNO_EQUAL		38,TestNext		//�{�[�i�X�Z�łȂ��Ȃ珈�����΂�
	C_INCDEC				10				//�{�[�i�X�Z�ɑ΂��ĉ��Z
TestNext:
	C_CALL					BougaiCheckSeq
	C_AIEND									//�I��

@��������߰ق��鏇�Ԃŏ�̕��̏ꍇ�͖W�Q�Z��D�悵�Ďg�p����
BougaiCheckSeq:
	C_IF_ORDER_OVER			1,BougaiCheckEnd	//2,3�Ԗڂ̎��͂��̼��ݽ�͏I��
	C_IFN_WAZACLASS_EQUAL	DIST1_CLASS,BougaiCheckEnd	//�W�Q�Z�łȂ��Ȃ�I��
	C_IFN_WAZACLASS_EQUAL	DIST2_CLASS,BougaiCheckEnd
	C_INCDEC				10				//�W�Q�Z�ɑ΂��ĉ��Z
BougaiCheckEnd:
	C_AIEND


@========================================================
@	�T�d�hAI:���X�N�̏��Ȃ������ȍs��������
@========================================================
ConsintyouAISeq:
	C_IF_TURN_EQUAL		0,ConsintyouAISeq_01		
	C_IF_TURN_EQUAL		1,ConsintyouAISeq_02		
	C_IF_TURN_EQUAL		2,ConsintyouAISeq_03		
	C_IF_TURN_EQUAL		3,ConsintyouAISeq_04		
	C_IF_TURN_EQUAL		4,ConsintyouAISeq_05		
	C_AIEND

ConsintyouAISeq_01://�P�^�[����
	C_IFN_ORDER_EQUAL	0,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	1,ConsintyouAISeq_01_02	
	C_IFN_ORDER_EQUAL	2,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	3,ConsintyouAISeq_01_01
	C_AIEND

ConsintyouAISeq_01_01://�P�Ԗ�
	C_IF_WAZACLASS_EQUAL	BOUGYO_CLASS,ConsintyouAISeq_INCDEC1
	C_AIEND

ConsintyouAISeq_01_02://�Q�Ԗ�
	C_IF_WAZACLASS_EQUAL	BOUGYO_CLASS,ConsintyouAISeq_INCDEC1
	C_AIEND

ConsintyouAISeq_01_03://�R�Ԗ�
	C_IF_WAZACLASS_EQUAL	BOUGYO_CLASS,ConsintyouAISeq_INCDEC1
	C_AIEND




ConsintyouAISeq_02://�Q�^�[����
	C_IFN_ORDER_EQUAL	0,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	1,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	2,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	3,ConsintyouAISeq_01_01
	C_AIEND

ConsintyouAISeq_03://�R�^�[����
	C_IFN_ORDER_EQUAL	0,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	1,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	2,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	3,ConsintyouAISeq_01_01
	C_AIEND

ConsintyouAISeq_04://�S�^�[����
	C_IFN_ORDER_EQUAL	0,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	1,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	2,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	3,ConsintyouAISeq_01_01
	C_AIEND

ConsintyouAISeq_05://�T�^�[����
	C_IFN_ORDER_EQUAL	0,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	1,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	2,ConsintyouAISeq_01_01	
	C_IFN_ORDER_EQUAL	3,ConsintyouAISeq_01_01
	C_AIEND

ConsintyouAISeq_INCDEC1:
	C_INCDEC	10	
	C_AIEND

ConsintyouAISeq_end:
	C_AIEND


@========================================================
@	��Ԃ`�o�l���������̂�I��AI
@========================================================
ConHiapAISeq:
	C_IF_APTOP_EQUAL	ConHiapAISeq_01
	C_AIEND
	
ConHiapAISeq_01:
	C_INCDEC	10	//��ԍ������̂ɑ΂��ĉ��Z
	C_AIEND

@========================================================
@	�y�i���e�B�����Ȃ�AI
@========================================================

ConPtAISeq:
	C_IF_APNO_EQUAL		3,ConPtAISeq_05	//�A���g�p�n�j�̋Z
	C_IF_CONTI_EQUAL	1,ConPtAISeq_01
	C_IF_CONTI_EQUAL	2,ConPtAISeq_02
	C_IF_CONTI_EQUAL	3,ConPtAISeq_03
	C_IF_CONTI_EQUAL	4,ConPtAISeq_04
	C_AIEND

ConPtAISeq_01:
	C_INCDEC	-5
	C_AIEND
	
ConPtAISeq_02:
	C_INCDEC	-15
	C_AIEND

ConPtAISeq_03:
	C_INCDEC	-20
	C_AIEND

ConPtAISeq_04:
	C_INCDEC	-25
	C_AIEND

ConPtAISeq_05:
	C_AIEND


@========================================================
@	�e���V�����D��AI
@========================================================
TensyonAISeq:
	C_IF_WAZATENSYON_UNDER	0,TensyonAISeq_01	//������Z
	C_IF_WAZATENSYON_EQUAL	0,TensyonAISeq_02	//����
	C_IF_WAZATENSYON_EQUAL	1,TensyonAISeq_03	//�オ��Z
	C_AIEND
	
TensyonAISeq_01:
	C_IF_TENSYON_EQUAL	4,TensyonAISeq_01_01	//�e���V�����S�Ȃ�
	C_IF_TENSYON_EQUAL	3,TensyonAISeq_01_03	//�e���V�����R�Ȃ�:0713
	C_IF_ALLWAZA_TENSYONCHECK_EQUAL	TensyonAISeq_end
	C_INCDEC	+15//0908
	C_AIEND

TensyonAISeq_01_01:
	C_IFN_ORDER_EQUAL	0,TensyonAISeq_01_02	//1�ԂłȂ�
	C_IF_RANDCOMPNUM_UNDER	51,TensyonAISeq_end	//80���̊m���Œe���i���j�F0908
	C_INCDEC	+20
	C_AIEND

TensyonAISeq_01_02:
	C_IF_RANDCOMPNUM_UNDER	127,TensyonAISeq_end	//50���̊m���Œe���i���j
	C_INCDEC	-10
	C_AIEND

TensyonAISeq_01_03:
	C_IFN_ORDER_EQUAL	0,TensyonAISeq_01_05	//1�ԂłȂ�:0713
	C_IF_TURN_EQUAL	4,TensyonAISeq_01_04	//�T�^�[���ڂŃe���V�����R�łP�ԁF0726
	C_IF_RANDCOMPNUM_UNDER	51,TensyonAISeq_end	//80���̊m���Œe���i���j:0726
	C_INCDEC	10
	C_AIEND

TensyonAISeq_01_04://�������e���V�������΂Ɏ��Ȃ���Ԏ��B
	C_INCDEC	15
	C_AIEND

TensyonAISeq_01_05:
	C_IF_RANDCOMPNUM_UNDER	127,TensyonAISeq_end	//50���̊m���Œe���i���j:0801
	C_INCDEC	10
	C_AIEND

TensyonAISeq_02://���ɂȂ�
	C_IF_RANDCOMPNUM_UNDER	127,TensyonAISeq_end	//50���̊m���Œe���i���j:0801
	C_INCDEC	10
	C_AIEND

TensyonAISeq_03:	
	C_IF_CONTI_OVER	0,TensyonAISeq_03_02	//�A���g�p�����͂����F0726
	C_IFN_ORDER_EQUAL	0,TensyonAISeq_03_01	//1�ԂłȂ�
	C_IFN_TENSYON_EQUAL	4,TensyonAISeq_03_01	//�e���V�����S�łȂ�
	C_INCDEC	+30		//�e���V�����S�łP�ԖڂȂ�E�E�E
	C_AIEND
	
TensyonAISeq_03_01:
	C_IF_RANDCOMPNUM_UNDER	100,TensyonAISeq_end	//60���̊m���Œe���i���j
	C_INCDEC	+10
	C_AIEND

TensyonAISeq_03_02:
	C_IFN_APNO_EQUAL	3,TensyonAISeq_end	//�y�i���e�B�ŃG�L�T�C�g���Ȃ��F0726
	C_IFN_ORDER_EQUAL	0,TensyonAISeq_03_01	//1�ԂłȂ�
	C_IFN_TENSYON_EQUAL	4,TensyonAISeq_03_01	//�e���V�����S�łȂ�
	C_INCDEC	+30		//�e���V�����S�łP�ԖڂȂ�E�E�E
	C_AIEND


TensyonAISeq_end:
	C_AIEND


@========================================================
@	�R���{�D��AI
@========================================================
ConComboAISeq:
	C_IF_COMBOOCCUR_EQUAL	ConComboAISeq_03	//���̋Z�ŃR���{����
	C_CALL		ConComboAISeq_01
	C_CALL		ConComboAISeq_02
	C_AIEND

ConComboAISeq_01:
	C_IFN_CONTI_EQUAL	0,ConComboAISeq_end		//�A���g�p
	C_IFN_COMBO1_EQUAL		ConComboAISeq_end	//�R���{�P�łȂ�
	C_IF_ORDER_EQUAL	0,ConComboAISeq_04_01
	C_IF_ORDER_EQUAL	1,ConComboAISeq_04_02
	C_IF_ORDER_EQUAL	2,ConComboAISeq_04_03
	C_IF_ORDER_EQUAL	3,ConComboAISeq_04_04
	C_AIEND
	
ConComboAISeq_02:
	C_IFN_COMBO2_EQUAL		ConComboAISeq_end	//�R���{�Q�łȂ�
	C_INCDEC	-10
	C_AIEND

ConComboAISeq_03:
	C_INCDEC	+25
	C_AIEND


ConComboAISeq_04_01://�P�Ԗ�
	C_IF_TURN_EQUAL	4,ConComboAISeq_04_05	//�T�^�[���ڂȂ�͂���
	C_IF_RANDCOMPNUM_UNDER	150,ConComboAISeq_end	//60���̊m���ł͂����i���j
	C_INCDEC	+10
	C_AIEND

ConComboAISeq_04_02://�Q�Ԗ�
	C_IF_TURN_EQUAL	4,ConComboAISeq_04_05	//�T�^�[���ڂȂ�͂���
	C_IF_RANDCOMPNUM_UNDER	125,ConComboAISeq_end	//50���̊m���ł͂����i���j
	C_INCDEC	+10
	C_AIEND

ConComboAISeq_04_03://�R�Ԗ�
	C_IF_TURN_EQUAL	4,ConComboAISeq_04_05	//�T�^�[���ڂȂ�͂���
	C_IF_RANDCOMPNUM_UNDER	50,ConComboAISeq_end	//19���̊m���ł͂����i���j
	C_INCDEC	+10
	C_AIEND

ConComboAISeq_04_04://�S�Ԗ�
	C_IF_TURN_EQUAL	4,ConComboAISeq_04_05	//�T�^�[���ڂȂ�͂���
	C_INCDEC	+10
	C_AIEND

ConComboAISeq_04_05://�T�^�[���ڂ͂�肸�炢
	C_IF_RANDCOMPNUM_UNDER	125,ConComboAISeq_end	//50���̊m��
	C_INCDEC	-15
	C_AIEND


ConComboAISeq_end:
	C_AIEND
	

@========================================================
@	�Z��pAI(��{�̓v���X)
@========================================================
ConWazaProAISeq:
	C_IF_APNO_EQUAL	39,ConWazaProAISeq_01	//�{�[�i�X�~4	
	C_IF_APNO_EQUAL	40,ConWazaProAISeq_02	//���P�Ԃ̋Z
	C_IF_APNO_EQUAL	41,ConWazaProAISeq_03	//���S�Ԃ̋Z
	C_IF_APNO_EQUAL	3,ConWazaProAISeq_04	//�y�i���e�B�O	
	//C_IF_APNO_EQUAL	2,ConWazaProAISeq_05	//���̃^�[���ȍ~�x��	
	C_IF_APNO_EQUAL	38,ConWazaProAISeq_06	//�{�[�i�X+1	
	C_IF_APNO_EQUAL	47,ConWazaProAISeq_07	//�e���V������~	
	C_IF_APNO_EQUAL	31,ConWazaProAISeq_08	//�S��/2+10��������	
	C_IF_APNO_EQUAL	32,ConWazaProAISeq_09	//�O�̐l+10������	
	C_IF_APNO_EQUAL	46,ConWazaProAISeq_10	//�G�L�T�C�g�ɂ���ĕ]���l���ς��	
	C_IF_APNO_EQUAL	27,ConWazaProAISeq_11	//�{�[�i�X������	
	C_IF_APNO_EQUAL	16,ConWazaProAISeq_12	//�R���{�W�Q	
	C_IF_APNO_EQUAL	17,ConWazaProAISeq_12	//�R���{����	
	C_IF_APNO_EQUAL	26,ConWazaProAISeq_13	//�S���ْ�	
	C_IF_APNO_EQUAL	18,ConWazaProAISeq_14	//���P��x��	
	C_AIEND

ConWazaProAISeq_01://0802
	C_IF_BONUS_EQUAL	3,ConWazaProAISeq_01_01
	C_IF_BONUS_EQUAL	2,ConWazaProAISeq_01_02
	C_IF_BONUS_EQUAL	1,ConWazaProAISeq_01_03
	C_IF_BONUS_EQUAL	0,ConWazaProAISeq_01_04
	
	C_AIEND


ConWazaProAISeq_01_01:
	C_INCDEC	+20
	C_AIEND

ConWazaProAISeq_01_02:
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end	//50���̊m���Œe���i���j
	C_INCDEC	+15
	C_AIEND
	
ConWazaProAISeq_01_03:
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end	//50���̊m���Œe���i���j
	C_INCDEC	+5
	C_AIEND

ConWazaProAISeq_01_04:
	C_INCDEC	-20
	C_AIEND

ConWazaProAISeq_02:
	C_IFN_WAZANOCHECK_EQUAL	29,ConWazaProAISeq_end	//29�����ĂȂ��H
	C_IF_RANDCOMPNUM_UNDER	50,ConWazaProAISeq_end	//25���̊m���Œe���i���j
	C_INCDEC	+20
	C_AIEND

ConWazaProAISeq_03:
	C_IFN_WAZANOCHECK_EQUAL	30,ConWazaProAISeq_end	//30�����ĂȂ��H
	C_IF_RANDCOMPNUM_UNDER	50,ConWazaProAISeq_end	//25���̊m���Œe���i���j
	C_INCDEC	+20
	C_AIEND

ConWazaProAISeq_04:
	C_IFN_ORDER_EQUAL	3,ConWazaProAISeq_end		//�S�ԖڂȂ�
	C_IF_RANDCOMPNUM_UNDER	50,ConWazaProAISeq_end	//25���̊m���Œe���i���j
	C_INCDEC	+15
	C_AIEND

ConWazaProAISeq_05:
	C_IF_TURN_EQUAL	4,ConWazaProAISeq_05_01	//�T�^�[���ڂȂ�
	C_IF_RANDCOMPNUM_UNDER	220,ConWazaProAISeq_05_02//10���̊m���Œe���i���j
	C_INCDEC	+10
	C_AIEND

ConWazaProAISeq_05_01:
	C_IF_RANDCOMPNUM_UNDER	20,ConWazaProAISeq_end	//25���̊m���Œe���i���j
	C_INCDEC	+15
	C_AIEND

ConWazaProAISeq_05_02:
	C_INCDEC	-20
	C_AIEND


ConWazaProAISeq_06:
	C_IFN_WAZANOCHECK_EQUAL	39,ConWazaProAISeq_06_02//39�����ĂȂ��H
	C_IF_BONUS_EQUAL	3,ConWazaProAISeq_06_01
	C_IF_RANDCOMPNUM_UNDER	50,ConWazaProAISeq_end	//25���̊m���Œe���i���j
	C_INCDEC	+15
	C_AIEND

ConWazaProAISeq_06_01:
	C_INCDEC	-10
	C_AIEND

ConWazaProAISeq_06_02://0806
	C_IF_TURN_EQUAL	4,ConWazaProAISeq_06_04	//�T�^�[���ڂȂ�
	C_IF_TURN_EQUAL	0,ConWazaProAISeq_06_03	//1�^�[���ڂȂ�
	C_IF_CONTI_EQUAL	1,ConWazaProAISeq_end	//�A���g�p�Ȃ�͂���
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end	//50���̊m���Œe���i���j
	C_INCDEC	10
	C_AIEND

ConWazaProAISeq_06_03://0806
	C_IF_RANDCOMPNUM_UNDER	100,ConWazaProAISeq_end	//38���̊m���Œe���i���j
	C_INCDEC	10
	C_AIEND

ConWazaProAISeq_06_04://0815
	C_INCDEC	-10
	C_AIEND


ConWazaProAISeq_07://�e���V������~:0728
	C_IF_CONTI_EQUAL	1,ConWazaProAISeq_end//�A���g�p�̂Ƃ�
	C_IF_ORDER_EQUAL	0,ConWazaProAISeq_07_01//0801
	C_IF_ORDER_EQUAL	1,ConWazaProAISeq_07_01//0801
	C_IFN_TURN_EQUAL	4,ConWazaProAISeq_end//�T�^�[����
	C_IF_ALLWAZA_TENSYONCHECK_EQUAL	ConWazaProAISeq_end//�e���V����������Z�����ĂȂ�
	C_IF_TENSYON_UNDER	1,ConWazaProAISeq_end//�e���V����max�ɂȂ�\���Ȃ�
	C_INCDEC	+10
	C_AIEND

ConWazaProAISeq_07_01:
	C_IF_RANDCOMPNUM_UNDER	127,ConWazaProAISeq_end	//50���̊m���Œe���i���j
	C_INCDEC	+10
	C_AIEND

ConWazaProAISeq_08:
	C_IF_ORDER_EQUAL	1,ConWazaProAISeq_08_01
	C_IF_ORDER_EQUAL	2,ConWazaProAISeq_08_02
	C_IF_ORDER_EQUAL	3,ConWazaProAISeq_08_03
	C_AIEND


ConWazaProAISeq_08_01:
	C_INCDEC	+5
	C_AIEND

ConWazaProAISeq_08_02:
	C_INCDEC	+15
	C_AIEND

ConWazaProAISeq_08_03:
	C_INCDEC	+20
	C_AIEND


ConWazaProAISeq_09:
	C_IF_ORDER_EQUAL	0,ConWazaProAISeq_09_01//0826
	C_IF_ORDER_EQUAL	1,ConWazaProAISeq_09_02//0826
	C_IF_ORDER_EQUAL	2,ConWazaProAISeq_09_03//0826
	C_IF_ORDER_EQUAL	3,ConWazaProAISeq_09_04//0826
	C_AIEND


ConWazaProAISeq_09_01:
	C_INCDEC	-10
	C_AIEND

ConWazaProAISeq_09_02:
	C_IFN_STATUS_EQUAL	0,ConWazaProAISeq_end
	C_INCDEC	+5
	C_AIEND

ConWazaProAISeq_09_03:
	C_IFN_STATUS_EQUAL	0,ConWazaProAISeq_09_03_01
	C_INCDEC	+5
	C_JUMP	ConWazaProAISeq_09_03_01
	C_AIEND

ConWazaProAISeq_09_03_01:
	C_IFN_STATUS_EQUAL	1,ConWazaProAISeq_end
	C_INCDEC	+5
	C_AIEND


ConWazaProAISeq_09_04:
	C_IFN_STATUS_EQUAL	0,ConWazaProAISeq_09_04_01
	C_INCDEC	+5
	C_JUMP	ConWazaProAISeq_09_04_01
	C_AIEND

ConWazaProAISeq_09_04_01:
	C_IFN_STATUS_EQUAL	1,ConWazaProAISeq_09_04_02
	C_INCDEC	+5
	C_JUMP	ConWazaProAISeq_09_04_02
	C_AIEND
ConWazaProAISeq_09_04_02:
	C_IFN_STATUS_EQUAL	2,ConWazaProAISeq_end
	C_INCDEC	+5
	C_AIEND

ConWazaProAISeq_10://0826
	C_IF_ORDER_EQUAL	0,ConWazaProAISeq_10_01//�P�Ԗ�
	C_IF_ORDER_OVER		0,ConWazaProAISeq_10_02//�P�Ԗڈȍ~�Ȃ�
	C_AIEND

ConWazaProAISeq_10_01:
	C_IFN_TURN_EQUAL	0,ConWazaProAISeq_10_01_01//�P�^�[���ڂȂ�
	C_IF_TENSYON_EQUAL	4,ConWazaProAISeq_10_01_02	//�e���V�����S�Ȃ�
	C_IF_TENSYON_EQUAL	3,ConWazaProAISeq_10_01_03	//�e���V�����R�Ȃ�
	C_AIEND

ConWazaProAISeq_10_01_01:
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	-15
	C_AIEND

ConWazaProAISeq_10_01_02:
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+20
	C_AIEND

ConWazaProAISeq_10_01_03:
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+15
	C_AIEND

ConWazaProAISeq_10_02:
	C_IF_RANDCOMPNUM_UNDER	178,ConWazaProAISeq_end//30%
	C_INCDEC	+10
	C_AIEND



ConWazaProAISeq_11://0828:�O�Ƀ{�[�i�X��Ԃ�����Ƃ�
	C_IF_ORDER_EQUAL	0,ConWazaProAISeq_end//�P�Ԗ�
	C_JUMP	ConWazaProAISeq_11_01
	C_AIEND

ConWazaProAISeq_11_01://0828
	C_IFN_STATUS_EQUAL	0,ConWazaProAISeq_11_01_04	//�x�݂̏ꍇ
	C_IF_RIVALBONUS_EQUAL	0,0,ConWazaProAISeq_11_01_04	//��0
	C_IF_RIVALBONUS_EQUAL	0,1,ConWazaProAISeq_11_01_01	//���P
	C_IF_RIVALBONUS_EQUAL	0,2,ConWazaProAISeq_11_01_02	//���Q
	C_IF_RIVALBONUS_EQUAL	0,3,ConWazaProAISeq_11_01_03	//���R
	C_AIEND


ConWazaProAISeq_11_01_01://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_11_02//50%
	C_INCDEC	+5
	C_IF_ORDER_OVER	1,ConWazaProAISeq_11_02
	C_AIEND

ConWazaProAISeq_11_01_02://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_11_02//50%
	C_INCDEC	+10
	C_IF_ORDER_OVER	1,ConWazaProAISeq_11_02
	C_AIEND

ConWazaProAISeq_11_01_03://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_11_02//50%
	C_INCDEC	+15
	C_IF_ORDER_OVER	1,ConWazaProAISeq_11_02
	C_AIEND

ConWazaProAISeq_11_01_04://0828
	C_IF_ORDER_OVER	1,ConWazaProAISeq_11_02
	C_AIEND


ConWazaProAISeq_11_02://0828
	C_IFN_STATUS_EQUAL	1,ConWazaProAISeq_11_02_04//�x�݂̏ꍇ
	C_IF_RIVALBONUS_EQUAL	1,0,ConWazaProAISeq_11_02_04	//��0
	C_IF_RIVALBONUS_EQUAL	1,1,ConWazaProAISeq_11_02_01	//���P
	C_IF_RIVALBONUS_EQUAL	1,2,ConWazaProAISeq_11_02_02	//���Q
	C_IF_RIVALBONUS_EQUAL	1,3,ConWazaProAISeq_11_02_03	//���R
	C_AIEND


ConWazaProAISeq_11_02_01://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_11_03//50%
	C_INCDEC	+5
	C_IF_ORDER_OVER	2,ConWazaProAISeq_11_03
	C_AIEND

ConWazaProAISeq_11_02_02://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_11_03//50%
	C_INCDEC	+10
	C_IF_ORDER_OVER	2,ConWazaProAISeq_11_03
	C_AIEND

ConWazaProAISeq_11_02_03://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_11_03//50%
	C_INCDEC	+15
	C_IF_ORDER_OVER	2,ConWazaProAISeq_11_03
	C_AIEND

ConWazaProAISeq_11_02_04://0828
	C_IF_ORDER_OVER	2,ConWazaProAISeq_11_03
	C_AIEND

ConWazaProAISeq_11_03://0828
	C_IFN_STATUS_EQUAL	2,ConWazaProAISeq_11_03_04//�x�݂̏ꍇ
	C_IF_RIVALBONUS_EQUAL	2,0,ConWazaProAISeq_11_03_04	//��0
	C_IF_RIVALBONUS_EQUAL	2,1,ConWazaProAISeq_11_03_01	//���P
	C_IF_RIVALBONUS_EQUAL	2,2,ConWazaProAISeq_11_03_02	//���Q
	C_IF_RIVALBONUS_EQUAL	2,3,ConWazaProAISeq_11_03_03	//���R
	C_AIEND


ConWazaProAISeq_11_03_01://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+5
	C_AIEND

ConWazaProAISeq_11_03_02://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+10
	C_AIEND

ConWazaProAISeq_11_03_03://0828
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+15
	C_AIEND

ConWazaProAISeq_11_03_04://0828
	C_AIEND

//�R���{���������R���{���Ԃ�����
ConWazaProAISeq_12://0829
	C_IF_ORDER_EQUAL	0,ConWazaProAISeq_end//�P�Ԗ�
	C_JUMP	ConWazaProAISeq_12_01
	C_AIEND

ConWazaProAISeq_12_01://0829
	C_IFN_STATUS_EQUAL	0,ConWazaProAISeq_12_02	//�x�݂̏ꍇ
	C_IF_RIVALCOMBO1_EQUAL	0,1,ConWazaProAISeq_12_02//�R���{�P���
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_12_02//50%
	C_INCDEC	+2
	C_IFN_COMBOCONTI_EQUAL	0,ConWazaProAISeq_12_02//�R���{�����������ƂȂ�
	C_INCDEC	+8
	C_AIEND


ConWazaProAISeq_12_02://0828
	C_IF_ORDER_EQUAL	1,ConWazaProAISeq_end//�Q�Ԗ�
	C_IFN_STATUS_EQUAL	1,ConWazaProAISeq_12_03	//�x�݂̏ꍇ
	C_IF_RIVALCOMBO1_EQUAL	1,1,ConWazaProAISeq_12_03//�R���{�P���
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_12_03//50%
	C_INCDEC	+2
	C_IFN_COMBOCONTI_EQUAL	1,ConWazaProAISeq_12_03//�R���{�����������ƂȂ�
	C_INCDEC	+8
	C_AIEND

ConWazaProAISeq_12_03://0828
	C_IF_ORDER_EQUAL	2,ConWazaProAISeq_end//�R�Ԗ�
	C_IFN_STATUS_EQUAL	2,ConWazaProAISeq_end	//�x�݂̏ꍇ
	C_IF_RIVALCOMBO1_EQUAL	2,1,ConWazaProAISeq_end//�R���{�P���
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+2
	C_IFN_COMBOCONTI_EQUAL	2,ConWazaProAISeq_end//�R���{�����������ƂȂ�
	C_INCDEC	+8
	C_AIEND

//�ْ�������
ConWazaProAISeq_13://0829
	C_IF_ORDER_EQUAL	3,ConWazaProAISeq_end//�S�Ԗ�
	C_JUMP	ConWazaProAISeq_13_01
	C_AIEND

ConWazaProAISeq_13_01://0829
	C_IFN_STATUS_EQUAL	3,ConWazaProAISeq_13_02	//�x�݂̏ꍇ
	C_IF_RIVALCOMBO1_EQUAL	3,0,ConWazaProAISeq_13_02//�R���{�P��ԂłȂ�
	C_INCDEC	+5
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_12_02//50%
	C_INCDEC	+5
	C_AIEND

ConWazaProAISeq_13_02://0829
	C_IF_ORDER_EQUAL	2,ConWazaProAISeq_end//�R�Ԗ�
	C_IFN_STATUS_EQUAL	2,ConWazaProAISeq_13_03	//�x�݂̏ꍇ
	C_IF_RIVALCOMBO1_EQUAL	2,0,ConWazaProAISeq_13_03//�R���{�P��ԂłȂ�
	C_INCDEC	+5
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_12_03//50%
	C_INCDEC	+5
	C_AIEND

ConWazaProAISeq_13_03://0829
	C_IF_ORDER_EQUAL	1,ConWazaProAISeq_end//�Q�Ԗ�
	C_IFN_STATUS_EQUAL	1,ConWazaProAISeq_end	//�x�݂̏ꍇ
	C_IF_RIVALCOMBO1_EQUAL	1,0,ConWazaProAISeq_end//�R���{�P��ԂłȂ�
	C_INCDEC	+5
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+5
	C_AIEND


ConWazaProAISeq_14://0901
	C_IF_TURN_EQUAL		4,ConWazaProAISeq_14_01_01//�T�^�[����
	C_JUMP	ConWazaProAISeq_14_01_02
	C_AIEND

ConWazaProAISeq_14_01_01://0901
	C_INCDEC	+5
	C_JUMP	ConWazaProAISeq_14_01_02
	C_AIEND

ConWazaProAISeq_14_01_02:
	C_IF_ORDER_EQUAL	0,ConWazaProAISeq_14_01//�P�Ԗ�
	C_IF_ORDER_EQUAL	1,ConWazaProAISeq_14_02//�Q�Ԗ�
	C_IF_ORDER_EQUAL	2,ConWazaProAISeq_14_03//�R�Ԗ�
	C_IF_ORDER_EQUAL	3,ConWazaProAISeq_14_04//�S�Ԗ�
	C_AIEND

ConWazaProAISeq_14_01://0901
	C_INCDEC	-15
	C_AIEND

ConWazaProAISeq_14_02://0901
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	-10
	C_AIEND

ConWazaProAISeq_14_03://0901
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+5
	C_AIEND

ConWazaProAISeq_14_04://0901
	C_IF_RANDCOMPNUM_UNDER	125,ConWazaProAISeq_end//50%
	C_INCDEC	+15
	C_AIEND

ConWazaProAISeq_end:
	C_AIEND

	
@========================================================
@	�W�Q��p��AI
@========================================================
ConBougaiAISeq:
	C_IF_CONTYPE_EQUAL	DIST1_CLASS,ConBougaiAISeq_01	
	C_IF_CONTYPE_EQUAL	DIST2_CLASS,ConBougaiAISeq_01	
	C_IF_CONTYPE_EQUAL	INDIRECT_CLASS,ConBougaiAISeq_01	
	C_AIEND

ConBougaiAISeq_01:
	C_IF_RANDCOMPNUM_UNDER	125,ConBougaiAISeq_end//50%
	C_INCDEC	+10
	C_AIEND

ConBougaiAISeq_end:
	C_AIEND



@========================================================
@	�G�L�X�p�[�gAI�i���n���ȓ���𐧌�j
@========================================================
ConExpAISeq:
	C_IF_APNO_EQUAL	8,ConExpAISeq_01	//�O��l�W�Q-20*	
	C_IF_APNO_EQUAL	10,ConExpAISeq_01	//�O��l�W�Q-40	
	C_IF_APNO_EQUAL	12,ConExpAISeq_01	//�O��l�W�Q-60	
	C_IF_APNO_EQUAL	14,ConExpAISeq_01	//�O��l�W�Q-����*
	C_IF_APNO_EQUAL	32,ConExpAISeq_01	//�O��l=����
	C_IF_APNO_EQUAL	35,ConExpAISeq_01	//�O��l�Ɠ����ꍇ
	C_IF_APNO_EQUAL	36,ConExpAISeq_01	//�O��l�ƈႤ�ꍇ
	C_IF_APNO_EQUAL	37,ConExpAISeq_01	//�O��l�ŕω�
	C_IF_APNO_EQUAL	9,ConExpAISeq_02	//�O�S���W�Q-20*
	C_IF_APNO_EQUAL	11,ConExpAISeq_02	//�O�S���W�Q-40
	C_IF_APNO_EQUAL	13,ConExpAISeq_02	//�O�S���W�Q-60
	C_IF_APNO_EQUAL	15,ConExpAISeq_02	//�O�S���W�Q-����*
	C_IF_APNO_EQUAL	17,ConExpAISeq_02	//�O�S���R���{-40
	C_IF_APNO_EQUAL	16,ConExpAISeq_02	//�O�S���R���{����*
	C_IF_APNO_EQUAL	18,ConExpAISeq_02	//�O�S��-40�ŋx��
	C_IF_APNO_EQUAL	19,ConExpAISeq_02	//�����^�C�v��-40
	C_IF_APNO_EQUAL	45,ConExpAISeq_02	//�O�S��-�]������
	C_IF_APNO_EQUAL	20,ConExpAISeq_02	//�������悳-40*
	C_IF_APNO_EQUAL	21,ConExpAISeq_02	//��������-40*
	C_IF_APNO_EQUAL	22,ConExpAISeq_02	//���킢��-40*
	C_IF_APNO_EQUAL	23,ConExpAISeq_02	//��������-40*
	C_IF_APNO_EQUAL	24,ConExpAISeq_02	//�����܂���-40*
	C_IF_APNO_EQUAL	28,ConExpAISeq_02	//�O�S��-10or-40(�{�[�i�X)*
	C_IF_APNO_EQUAL	27,ConExpAISeq_02	//�{�[�i�X������:0826
	C_IF_APNO_EQUAL	31,ConExpAISeq_02	//�O�S��/2������
	C_IF_APNO_EQUAL	25,ConExpAISeq_03	//����l�ْ�*
	C_IF_APNO_EQUAL	26,ConExpAISeq_04	//���S����K���ْ�
	C_IF_APNO_EQUAL	47,ConExpAISeq_04	//�e���V�����������Ȃ�
	C_IF_APNO_EQUAL	38,ConExpAISeq_05	//�{�[�i�X+10
	C_IF_APNO_EQUAL	4,ConExpAISeq_06	//�h��P��
	C_IF_APNO_EQUAL	5,ConExpAISeq_06	//���S�h��
	C_IF_APNO_EQUAL	6,ConExpAISeq_06	//20�ȉ����󂯂Ȃ�
	C_IF_APNO_EQUAL	2,ConExpAISeq_07	//����
	C_AIEND

ConExpAISeq_01://�O��l�ɑΏۃ|�P���������Ȃ��ꍇ
	C_IF_ORDER_EQUAL	0,ConExpAISeq_01_01
	C_IF_ORDER_EQUAL	1,ConExpAISeq_01_02
	C_IF_ORDER_EQUAL	2,ConExpAISeq_01_03
	C_IF_ORDER_EQUAL	3,ConExpAISeq_01_04
	C_AIEND

ConExpAISeq_01_01:
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_01_02:
	C_IF_STATUS_EQUAL	0,ConExpAISeq_end
	C_INCDEC	-10	
	C_AIEND

ConExpAISeq_01_03:
	C_IF_STATUS_EQUAL	1,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_01_04:
	C_IF_STATUS_EQUAL	2,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND


ConExpAISeq_02://�O�ɑΏۃ|�P���������Ȃ��ꍇ:0801
	C_IF_ORDER_EQUAL	0,ConExpAISeq_02_01
	C_IF_ORDER_EQUAL	1,ConExpAISeq_02_02
	C_IF_ORDER_EQUAL	2,ConExpAISeq_02_03
	C_IF_ORDER_EQUAL	3,ConExpAISeq_02_04
	C_AIEND

ConExpAISeq_02_01:
	C_INCDEC	-20
	C_AIEND

ConExpAISeq_02_02:
	C_IF_STATUS_EQUAL	0,ConExpAISeq_end
	C_INCDEC	-15
	C_AIEND

ConExpAISeq_02_03:
	C_IF_STATUS_EQUAL	0,ConExpAISeq_end
	C_IF_STATUS_EQUAL	1,ConExpAISeq_end
	C_INCDEC	-15
	C_AIEND

ConExpAISeq_02_04:
	C_IF_STATUS_EQUAL	0,ConExpAISeq_end
	C_IF_STATUS_EQUAL	1,ConExpAISeq_end
	C_IF_STATUS_EQUAL	2,ConExpAISeq_end
	C_INCDEC	-15
	C_AIEND

ConExpAISeq_03://����l�ɑΏۃ|�P���������Ȃ��ꍇ
	C_IF_ORDER_EQUAL	0,ConExpAISeq_03_01
	C_IF_ORDER_EQUAL	1,ConExpAISeq_03_02
	C_IF_ORDER_EQUAL	2,ConExpAISeq_03_03
	C_INCDEC	-10	//�S�Ԗ�
	C_AIEND

ConExpAISeq_03_01:
	C_IF_STATUS_EQUAL	1,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_03_02:
	C_IF_STATUS_EQUAL	2,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_03_03:
	C_IF_STATUS_EQUAL	3,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND


ConExpAISeq_04://���ɑΏۃ|�P���������Ȃ��ꍇ
	C_IF_ORDER_EQUAL	0,ConExpAISeq_04_01
	C_IF_ORDER_EQUAL	1,ConExpAISeq_04_02
	C_IF_ORDER_EQUAL	2,ConExpAISeq_04_03
	C_INCDEC	-10	//�S�Ԗ�
	C_AIEND

ConExpAISeq_04_01:
	C_IF_STATUS_EQUAL	1,ConExpAISeq_end
	C_IF_STATUS_EQUAL	2,ConExpAISeq_end
	C_IF_STATUS_EQUAL	3,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_04_02:
	C_IF_STATUS_EQUAL	2,ConExpAISeq_end
	C_IF_STATUS_EQUAL	3,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_04_03:
	C_IF_STATUS_EQUAL	3,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_05://�{�[�i�X��+3�ɂȂ��Ă���H
	C_IF_BONUS_UNDER	3,ConExpAISeq_end
	C_INCDEC	-20
	C_AIEND

ConExpAISeq_06://���ɑΏۃ|�P���������Ȃ��ꍇ�̖h��Z
	C_IF_ORDER_EQUAL	0,ConExpAISeq_06_01
	C_IF_ORDER_EQUAL	1,ConExpAISeq_06_02
	C_IF_ORDER_EQUAL	2,ConExpAISeq_06_03
	C_INCDEC	-10	//�S�Ԗ�
	C_AIEND

ConExpAISeq_06_01:
	C_IF_STATUS_EQUAL	1,ConExpAISeq_end
	C_IF_STATUS_EQUAL	2,ConExpAISeq_end
	C_IF_STATUS_EQUAL	3,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_06_02:
	C_IF_STATUS_EQUAL	2,ConExpAISeq_end
	C_IF_STATUS_EQUAL	3,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_06_03:
	C_IF_STATUS_EQUAL	3,ConExpAISeq_end
	C_INCDEC	-10
	C_AIEND

ConExpAISeq_07:
	C_IF_TURN_EQUAL		0,ConExpAISeq_07_01
	C_IF_TURN_EQUAL		1,ConExpAISeq_07_02
	C_IF_TURN_EQUAL		2,ConExpAISeq_07_03
	C_IF_TURN_EQUAL		3,ConExpAISeq_07_04
	C_IF_TURN_EQUAL		4,ConExpAISeq_07_05
	C_AIEND

ConExpAISeq_07_01:
	C_IF_RANDCOMPNUM_UNDER	20,ConExpAISeq_end		//7���̊m���Œe���i���j
	C_INCDEC	-15
	C_AIEND

ConExpAISeq_07_02:
	C_IF_RANDCOMPNUM_UNDER	40,ConExpAISeq_end		//15���̊m���Œe���i���j
	C_INCDEC	-15
	C_AIEND

ConExpAISeq_07_03:
	C_IF_RANDCOMPNUM_UNDER	60,ConExpAISeq_end		//23���̊m���Œe���i���j
	C_INCDEC	-15
	C_AIEND

ConExpAISeq_07_04:
	C_IF_RANDCOMPNUM_UNDER	80,ConExpAISeq_end		//31���̊m���Œe���i���j
	C_INCDEC	-15
	C_AIEND

ConExpAISeq_07_05:
	C_IF_RANDCOMPNUM_UNDER	20,ConExpAISeq_end		//8���̊m���Œe���i���j
	C_INCDEC	+20
	C_AIEND

ConExpAISeq_end://�I��
	C_AIEND

@========================================================
@	�A�s�[�����鏇�ԂŗL���s������������AI
@========================================================
ConNomverAISeq:
	C_IF_ORDER_EQUAL	0,ConNomverAISeq_01	
	C_IF_ORDER_EQUAL	1,ConNomverAISeq_02	
	C_IF_ORDER_EQUAL	2,ConNomverAISeq_03	
	C_IF_ORDER_EQUAL	3,ConNomverAISeq_04
	C_AIEND

ConNomverAISeq_01:
	C_IF_APNO_EQUAL		29,ConNomverAISeq_01_01	//�P�ʂȂ�R�{	
	C_IF_APNO_EQUAL		33,ConNomverAISeq_01_02	//�P�ʂȂ�O
	C_IF_WAZACLASS_EQUAL	BOUGYO_CLASS,ConNomverAISeq_01_03//�h��Z�n�H:0807
	C_AIEND

ConNomverAISeq_01_01:
	C_INCDEC	+15
	C_AIEND

ConNomverAISeq_01_02:
	C_INCDEC	-15
	C_AIEND

ConNomverAISeq_01_03:
	C_IF_RANDCOMPNUM_UNDER	100,ConExpAISeq_end		//40���̊m���Œe���i���j
	C_INCDEC	10
	C_AIEND


ConNomverAISeq_02:
	C_IF_APNO_EQUAL	33,ConNomverAISeq_02_01	//�Q�ʂȂ�20
	C_IF_WAZACLASS_EQUAL	BOUGYO_CLASS,ConNomverAISeq_02_02//�h��Z�n�H:0807
	C_AIEND

ConNomverAISeq_02_01:
	C_INCDEC	-5
	C_AIEND

ConNomverAISeq_02_02:
	C_IF_RANDCOMPNUM_UNDER	125,ConExpAISeq_end		//50���̊m���Œe���i���j
	C_INCDEC	10
	C_AIEND



ConNomverAISeq_03:
	C_IF_APNO_EQUAL	33,ConNomverAISeq_03_01	//�R�ʂȂ�40
	C_IF_APNO_EQUAL	31,ConNomverAISeq_03_01	//���܂ł̍��v�l�̔���
	C_IF_APNO_EQUAL	1,ConNomverAISeq_03_01	//�W�Q�Q�{�󂯂�
	C_AIEND

ConNomverAISeq_03_01:
	C_INCDEC	+5
	C_AIEND


ConNomverAISeq_04:
	C_IF_APNO_EQUAL	33,ConNomverAISeq_04_01	//�S�ʂȂ�60
	C_IF_APNO_EQUAL	30,ConNomverAISeq_04_01	//�S�ʂȂ�60
	C_IF_APNO_EQUAL	31,ConNomverAISeq_04_01	//���܂ł̍��v�l�̔���
	C_IF_APNO_EQUAL	1,ConNomverAISeq_04_01	//�W�Q�Q�{�󂯂�
	C_IF_APNO_EQUAL	18,ConNomverAISeq_04_04	//���P��x�݁F0908
	C_IF_WAZACLASS_EQUAL	BOUGYO_CLASS,ConNomverAISeq_04_02//�h��Z�n�H
	C_IF_WAZACLASS_EQUAL	DIST2_CLASS,ConNomverAISeq_04_03//�S�̖W�Q�n�H�F0807
	C_AIEND

ConNomverAISeq_04_01:
	C_INCDEC	+15
	C_AIEND

ConNomverAISeq_04_02:
	C_INCDEC	-10
	C_AIEND

ConNomverAISeq_04_03:
	C_IF_RANDCOMPNUM_UNDER	125,ConExpAISeq_end		//50���̊m���Œe���i���j
	C_INCDEC	10
	C_AIEND

ConNomverAISeq_04_04:
	C_INCDEC	5
	C_AIEND


@========================================================
@	�_�~�[AI
@========================================================
ConDummyAISeq:
	C_AIEND

