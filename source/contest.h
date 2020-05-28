#ifndef __CONTEST_H__
#define __CONTEST_H__


#include "con_ai.def"



//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define CONTEST_VERSION	100		//��ýĂ̻��ް���ް�ޮ�

#define BREEDER_MAX		4
#define END_TURN		5
#define BREEDER_AI_DATA_MAX	60	//COM�����삷����ذ�ް�ް��̐�

#define CONT_MINE_CLIENT	2	//���@���̸ײ���No
#define CONT_ENEMY_CLIENT	3	//�G��(��������)�̸ײ���No

#define CONT_POKEDECORD_NO	0	//�߹�݂̊G��DecordAdrs�ւ̓W�J�ꏊ

//-- �X����ذ�ް�̍�����گ�No --//
#define BLD_PAL			0xa	// BLD_PAL+��ذ�ްNo �ł�����ذ�ް����گ�
//-- �O����݂ɏo�����Z����\�����鎞����گ�No --//
#define CON_GRAYPAL		(BLD_PAL + BREEDER_MAX)

#define METER_WAIT		15	//30	//��߰�Ұ�����������Ă����ۂ�wait

//-- Cell, Pal�pID --//
enum{
	FUKIDASI_ID = 44000,
	VOLTAGEICON_ID,
	VOLTAGEGAUGE_ID,
};

//-- �v���[���[�����B���� --//
#define CURTAIN_SP		7	//����������(�オ��)���̑��x

//-- �e���V���� --//
#define TENSYON_MAX		5	//�e���V�����ő�l
#define TENSYON_POINT	10	//�Z�����߂���v��������AP�ɉ��Z�����l
#define TENSYON_MAXPOINT	60	//�e���V�������ő�l�ɒB��������AP�ɉ��Z�����l

//-- �{�[�i�X --//
#define BONUS_POINT		10
#define BONUS_MAX		30

//-- �Ɩ���evy --//
#define LIGHT_EVY		2	//3	//5

//-- �Z�G�t�F�N�g�p�̒�` --//
#define CONTEST_RIGHTEND	152		//�w�i�ʂ̉E�[���W

//-- �ϋq�̐���オ��(���艺����)���̃J���[ --//
#define KANKYAKU_EVY		10
#define TENSYON_DOWN_COLOR	0x0000
#define TENSYON_UP_COLOR	0x237e


//-- AI�p�̒萔 --//
//AI_STATUSFLAG�錾
#define	CONAI_STATUSFLAG_END	0x01			//AI�V�[�P���X�̏I��
#define	CONAI_STATUSFLAG_END_OFF	(0x01^0xff)	//AI�V�[�P���X�̏I��
//-- +++++++++ --//

//-- �e���V���� --//
#define TENSYON_GAUGE_UP	1	//��ý����߂ƋZ���߂���v�������ɏオ��ް�ނ��߲��


//----------------------------------------------------------
//	UserWork�̊���U��A�h���X
//----------------------------------------------------------
#if 0 //--------- ���ۂ̒�`��client.h�ł��Ă� --------------
// �w�i�؂�ւ��Ž�ذ��ް���W�J���Ă���̂ź�ýĂł�1000h�m��
#define CONTEST_WAZAEFF_USERWORK	0x14800	//��ýĂ̋Z�̪�ėp��UserWork
#endif //-------------------------------------------------------
//�ϋq��2����ݱ�җp��UserWork�ɷ���ް���W�J���Ă���
#define CONT_KANKYAKU_ADRS	0x15800	//(0x17800 - 0x2000)
/*�� 0x17800�`0x18000�͐퓬�Ÿײ��Ă̼��їpܰ��Ƃ��Ďg�p���Ă���B
	 �Z�̪�Ă̒��ł͂������Ă�\��������̂ŁA0x17800�`0x18000�̊Ԃ�
	 �����Ă���*/
#define DEBUG_AP			0x18000	//�f�o�b�O�p�׸�
#define CONT_PALETTE_ADRS	(DEBUG_AP + 4)	//BG����̫����گĂ�UserWork�Ɏ���Ă���
#define CONT_TURN_PLTT	(CONT_PALETTE_ADRS + 0x200)	//����ݍŏ�����گĂ̏�Ԃ�����Ă���
#define CONT_TURNFADE_PLTT	(CONT_TURN_PLTT + 0x400)//����݈Â�������Ԃ���گĂ�ۑ�
#define CONT_BGWIN_SCRN_TEMP	(CONT_TURNFADE_PLTT + 0x400)//BG����޳�̽�ذ݈ꎞ�ޔ�
#define CONT_SYSTEM_ADRS	(CONT_BGWIN_SCRN_TEMP + 1024*2)
#define CONT_APSYSTEM_ADRS	(CONT_SYSTEM_ADRS + sizeof(ContestSys))
#define CONT_WAZAKOUKA_ADRS	(CONT_APSYSTEM_ADRS + sizeof(ContestAPSys)*BREEDER_MAX)
#define CONT_AI_ADRS		(CONT_WAZAKOUKA_ADRS + sizeof(WazaKoukaWork))
#define CONT_CALCWORK_ADRS	(CONT_AI_ADRS + sizeof(ConAIWorkTbl))
#define CONT_BRDSYS_ADRS	(CONT_CALCWORK_ADRS + sizeof(ContestCalcWork)*BREEDER_MAX)
#define CONT_WAZAPARA_ADRS	(CONT_BRDSYS_ADRS + sizeof(BrdSysData)*BREEDER_MAX)

//----------------------------------------------------------
//	���э\���̂ɱ������鎞�Ɏg�p����}�N��
//----------------------------------------------------------
#define consys	((ContestSys*)&UserWork[CONT_SYSTEM_ADRS])
#define apsys	((ContestAPSys*)&UserWork[CONT_APSYSTEM_ADRS])
#define CONWAZA	((WazaKoukaWork*)&UserWork[CONT_WAZAKOUKA_ADRS])
#define CONAI	((ConAIWorkTbl*)&UserWork[CONT_AI_ADRS])	//AI�p���э\����
#define ccw		((ContestCalcWork*)&UserWork[CONT_CALCWORK_ADRS])
#define brdsys	((BrdSysData*)&UserWork[CONT_BRDSYS_ADRS])
#define conpara	((ConWazaEffParam*)&UserWork[CONT_WAZAPARA_ADRS])

//----------------------------------------------------------
//	�X�N���[���f�[�^
//----------------------------------------------------------
#define COMBO_MARK_SCRN		0x201d
#define WIN_SPC_SCRN		0x000c



//-- �Z�G�t�F�N�g�p��Vram�A�h���X --//
#define CONWAZA_BG_CHARBASE	2
#define CONWAZA_BG_SCRNBASE	30
#define CONWAZA_BG_PAL		0xe
//-- BG3(�w�i)��BG���ނƉ�荞���׸ނ̃f�t�H���g�l --//
#define CONTEST_HAIKEI_SIZE	0
#define CONTEST_HAIKEI_LOOP	1


//-- �R�������W --//
#define CON_JUDGE_X		112//44
#define CON_JUDGE_Y		36
//-- �R���̂ӂ������A�C�R�����W --//
#define CON_FUKIDASI_X	96
#define CON_FUKIDASI_Y	10

// �ײ�ނ��Ă����߹�݂̍��W
#define CON_POKE_X		112	//120//200//(240+32)	//��ʊO
#define CON_POKE_Y		80

//-- �Z���������W --//
#define CON_RANKU_X		0xb
#define CON_RANKU_Y		(20+15)		// 20 = ���̽�ذ݂܂ł̍���
#define CON_WAZACOM_X	11
#define CON_WAZACOM_Y	(20+15)
#define CON_TYPE_X		0xb
#define CON_TYPE_Y		(20+11)

//-- ��ýĒ��ɗ���ү���ނ̍��W --//
#define CON_TALKMSG_X	1
#define CON_TALKMSG_Y	15

//-- �Z�̍��W(˯Ă���ꏊ) --//
#define CON_ATTACK_X	(CON_POKE_X)
#define CON_ATTACK_Y	CON_POKE_Y
#define CON_DEFENCE_X	48	//24	//122
#define CON_DEFENCE_Y	40	//70

//-- �n�[�g�A�C�R���̏������W --//
#define HEARTICON_X		(8*22+4)

//-- �\�[�g�A�C�R���̏������W --//
#define SORTICON_X		(8*0x19+4)

//-- �G���ײ�ނ��ďo�Ă���X�s�[�h --//
//#define CONTEST_ENEMY_SLIDE_SP	(-3)
//#define CONTEST_ENEMY_SLIDE_WAIT	60

//-- �Z���o���߹�݂��ײ�ނ��Ă��鎞�Ɏg�p����萔�l --//
#define CONTEST_SLIDEIN_SP		(-2)//(-3)	//�ײ�޲݂��Ă��鎞�̽�߰��
#define CONTEST_SLIDEOUT_SP		(-6)	//�ײ�ޱ�Ă��鎞�̽�߰��
#define CONTEST_SLIDE_INIT_DX	(-(CONTEST_SLIDEIN_SP * 60))//20))	//����DX
#define CONTEST_SLIDE_WAIT		30		//�ײ�޲݂��Ă��āA�Z�̪�Ă��o���܂ł̳���


//-- ү���޳���޳��ر����邽�߂̋󔒷�ׂ�BGVRAM��ɾ�Ă���ʒu --//
#define MSG_CLEAR_NO	0
#define MSG_START_NO	0x200	//0x100

//-- ү���ފ֘A --//
#define FONT_BYTE		2		//1�������޲Ļ���
//-- ү����̫�� �ް��� --//
#define CON_POKE_LEN			(10*FONT_BYTE)	//۰�ײ�ޗp��10����(EOM����)
#define CON_TRAINER_LEN			(11*FONT_BYTE)	//۰�ײ�ޗp��10����(EOM����) + '/'
#define CON_WAZANAME_LEN		(7*FONT_BYTE)	//�Z���̒���
#define CON_JYUNNI_MSG_LEN		(4*FONT_BYTE)	//�u����񂢁v��ү����
#define CON_NOW_JYUNNI_LEN		(3*FONT_BYTE)	//���݂̏���(��߰� + ���� +�u���v)
//4�̕��̍��v�̒���
#define CON_POKE_ALLLEN			(CON_POKE_LEN*BREEDER_MAX)
#define CON_TRAINER_ALLLEN		(CON_TRAINER_LEN*BREEDER_MAX)
#define CON_WAZA_ALLLEN			(CON_WAZANAME_LEN*BREEDER_MAX)
#define CON_JYUNNI_MSG_ALLLEN	(CON_JYUNNI_MSG_LEN*BREEDER_MAX)
#define CON_NOW_JYUNNI_ALLLEN	(CON_NOW_JYUNNI_LEN*BREEDER_MAX)
#define CON_WAZALIST_ALLLEN		(CON_WAZANAME_LEN*4)	//�ő�Z4��
//-- ү����̫�ē]��CGX.No --//
//��ýĉ�ʒ��Œ�
#define CONTEST_START_FONT	MSG_START_NO
#define CON_POKENAME		CONTEST_START_FONT
#define CON_TRAINER			(CON_POKENAME+CON_POKE_ALLLEN)
#define CON_TENSYON_MSG		(CON_TRAINER+CON_TRAINER_ALLLEN)
#define CON_TENSYON_NUM		(CON_TENSYON_MSG+6*2)

//��ʒ��ł͓����ɏo�邱�Ƃ͂Ȃ��̂ű��ڽ�����Ԃ�
//���Ԃ���ڽ�̐擪
#define CON_OVERLAP_MSG		(CON_TENSYON_NUM+2*2)

#define CON_JYUNNI_MSG		CON_OVERLAP_MSG
#define CON_NOW_JYUNNI		(CON_JYUNNI_MSG+CON_JYUNNI_MSG_ALLLEN)
#define CON_WAZA			CON_OVERLAP_MSG
										//�����ʂ��Z���̕��������̂�
#define CON_MSG				(CON_WAZA+CON_WAZA_ALLLEN)	//�Z���o���Ă��鎞�ɗ����ү����
#define CON_WAZALIST		(CON_WAZA+CON_WAZA_ALLLEN)	//�Z�I����
//#define CON_ICON_SRA		(CON_WAZALIST+CON_WAZALIST_ALLLEN)	//�׽�����߱��݂̊Ԃ̽ׯ��
//#define CON_WAZAEXPLA		(CON_ICON_SRA+1*2)	//�Z:������
#define CON_TYPETEXT		(CON_WAZALIST+CON_WAZALIST_ALLLEN)	//�Z�̑���
#define CON_WAZASURA		(CON_TYPETEXT+5*2)
#define CON_WAZAEXPLA		(CON_WAZASURA+1*2)

#define CON_TUUSIN_TAIKI_MSG	CON_OVERLAP_MSG


//----------------------------------------------------------
//	���ޯ�ޗp�萔����۸���ٰ�݂Ȃǂ̈ꎞFix�Ȃ�
//----------------------------------------------------------
#define CONTEST_DEBUG	0	// �ʐM���ޯ�ޗp���b�Z�[�W�\��

//----------------------------------------------------------
//	�Z���󂯂����̃��b�Z�[�W
//----------------------------------------------------------
enum{
	SIKASIJYAMA_MSG,
	HOKANOPOKE_MSG,
	SIKASISIPPAI_MSG,
	ITUMOYORI_MSG,
	ITUMOYORI_MUCH_MSG,		//������݈ȍ~������ү���ނ��o��
	MEWOSORASITA_MSG,
	KOREMADENO_MSG,
	KOREIJYOU_MSG,
	KOWAIKURAI_MSG,
	MEDATTEIRUPOKE_MSG,
	KITAIWOKOMETA_MSG,
	KANARIUMAKU_MSG,
	FUTUUYORI_MSG,
};

//----------------------------------------------------------
//	���̃^�[���ǂꂾ���ڗ����������ʂ��o�����b�Z�[�W
//----------------------------------------------------------
enum{
	ZENZEN_MSG,
	ANMARI_MSG,
	MAAMAA_MSG,
	KANARI_MSG,
	MONOSUGOKU_MSG,
	SITENAI_MSG,
};

//----------------------------------------------------------
//	�Z�f�[�^�p
//----------------------------------------------------------
// con_ai.def�ɏ�����Ă���u�Z�̃A�s�[�����ށv�Ɠ��ꂷ�邱��
enum{		//�敪
	NORMAL_KUBUN,			//�m�[�}��
	BOUGYO_KUBUN,			//�h��
	DIST1_KUBUN,			//�W�Q1
	DIST2_KUBUN,			//�W�Q2
	INDIRECT_KUBUN,			//�ԐږW�Q
	SPECIAL_KUBUN,			//����
	TURNCONT_KUBUN,			//���Ԑ���

	//-- ��������Ȃ��Ȃ������� --//
	COMBO1_KUBUN,			//�R���{1
	COMBO2_KUBUN,			//�R���{2
};
enum{		//�N���X
	S_CLASS,
	A_CLASS,
	B_CLASS,
	C_CLASS,
	D_CLASS,
};
enum{		//�Z����
	STYLE_TYPE,
	BEAUTIFUL_TYPE,
	CUTE_TYPE,
	CLEVER_TYPE,
	STRONG_TYPE,
};

//----------------------------------------------------------
//	HeartIconAffEffSet�֐��Ŏw�肷��AffineType
//----------------------------------------------------------
enum{
	HEART_AFF_NORMAL = 0,	//�ʏ�̑傫���ɂ���
	HEART_AFF_SMALL,		//�ʏ킩�珬�������Ă���
	HEART_AFF_BIG,			//��������Ԃ���傫�����Ă���
};

//----------------------------------------------------------
//	�R���̂ӂ������A�C�R���̃^�C�v
//----------------------------------------------------------
enum{
	PENALTY_TYPE,
	TENSYONDOWN_TYPE,
	COMBO1_TYPE,
	COMBO2_TYPE,
	TENSYONUP_TYPE,
	NEXTSORT_FAST_TYPE,
	NEXTSORT_LAST_TYPE,
	NEXTSORT_QUES_TYPE,
	BONUS_TYPE,
};

#define FUKIDASI_PUT_TIME	85	//�ӂ�������\�����Ă鎞��


//----------------------------------------------------------
//	�ʐM�׸�
//----------------------------------------------------------
#define C_TUUSIN_FLG	1

//----------------------------------------------------------
//	�^�錾
//----------------------------------------------------------
typedef void (*pConFunc)(u8);


//----------------------------------------------------------
//	�\���̐錾
//----------------------------------------------------------

//-- ��ýėp�����ް� --//
typedef struct{
//	u8 BreederNo;	//��������ذ�ްNo(�ʐM����Ƃ��Ɏ��������Ԃ������悤�ɂ���)
	u8 cursor;	//���وʒu���
	u8 turn;	//��ݶ���
//	u8 fusanka:1;	//������ݎQ���ł��Ȃ��Ƃ���1�����

	u8 waza_msg[BREEDER_MAX];	//�Z���o����(�󂯂�)���ɏo��ү���ނ����ް
	u8 itumoyori_flg:1;		//����݂Ɂu�������E�E�E�v��ү���ނ��o��
	u8 itumoyori_much:1;	//����݈ȍ~�����Ɓu�������E�E�E�v��ү���ނ��o��
	u8 appeal_msg:1;		//1:�u��ٌ߰���!�v��ү���ނ��o���B 0:�o���Ȃ�
	u8 spotlight_eff:1;		//��߯�ײĴ̪�Ď��s���׸�(1:���s��)
	u8 fukidasi_eff:1;		//�R���̂ӂ������̪�Ď��s���׸�(1:���s��)
	u8 voltage_eff:1;		//���ð�޴̪�Ď��s���׸�(1:���s��)
	u8 voltage_slide_eff:1;	//���ð�޹ް�޽ײ�޴̪�Ď��s���׸�(1:���s��)
	u8 kankyaku_anm:1;		//�ϋq��Ҏ��s���׸�(1:���s��)
	
	u8 kankyaku_color_eff:1;//�ϋq�̐���オ��(���艺����)���̶װ̪��޴̪�Ď��s���׸�
//	u8 tensyon_stop:1;		//�׸ނ������Ă��鎞���ݼ�݂��㏸�A���~���Ȃ�
//	u8 tensyon_stop_brd:3;	//tensyon_stop�׸ނ𗧂Ă���ذ�ް��No
	u8 heart_aff_eff:1;		//ʰı��݂��̨ݴ̪�Ē��׸�(1:���s��)
	u8 sio_wazacalc:1;		//�Z�v�Z�f�[�^�ʐM�擾�҂�
	u8					:1;
	
	u8 id;		//Ҳݼ��ݽ�Ƃ��ē����Ă��������ID
	u8 jyunni_icon[BREEDER_MAX];	//���ʱ��݂�actno
	u8 eye_color;	//���ގ��̖ڂ̖��ŊǗ����No
	u8 meter_color;	//Ұ��MAX���̶װ�_�ŊǗ����No
	
	s8 kouka_dameji;	//���Z���o�����߹�݂��甭������A����ذ�ް�ɑ΂��Ă���Ұ���߲��
	u8 wazaput_count;	//���C���߹�݂��Z���o���������Ă��Ă�(����ݸر�����)
	u8 wazabrd;			//���A�Z���o���Ă����߹�݂���ذ�ްNo
	
	u8 fukidasi_act;	//�R���̂ӂ������A�C�R����ActNo
	s8 tensyon;			//���݂̃e���V�����߲��
//	u8 next_sort[BREEDER_MAX];	//������݉��Ԗڂɱ�߰ق��邩
	u8 nowturn_sort[BREEDER_MAX];	//���݂���݂ɱ�߰ق��鏇��(��{�I�ɂ�BrdSort�Ɠ����B										//����BrdSort�ͱ�ُ߰I�����ɍX�V����邪�A�����
									//��ݏI�����ɍX�V�����

	unsigned long r_next;	//����т̎�A�ޔ��G���A

	u16	WazaHistory[END_TURN][BREEDER_MAX];	//�e��ذ�ް���o�����Z�̗���
	s8	TensyonHistory[END_TURN][BREEDER_MAX];	//�e��ذ�ް���㉺�������ݼ�݂̗���

//	u8 voltage_act;		//���ð�ޱ��݂�ActNo
	u8 volgauge_act;	//���ð�޹ް�ނ�ActNo
//	u8 volgaugesub_act;	//���ð�޹ް�ނ̉E�[��ActNo
	
//	u16 UseWaza[BREEDER_MAX];	//�J��o���Z�����ް
//	u16 UseWazaOld[BREEDER_MAX];	//�O����݂ɌJ��o�����Z
//	u8 BreederRanking[BREEDER_MAX];	//�e��ذ�ް�̌��݂̏���
	
//	s16 ap[BREEDER_MAX];		//�e��ذ�ް���o�����Z�̊�{AP
//	s16 total_ap[BREEDER_MAX];	//���܂ł̗݌vAP
	u8 init_counter;	//InitContest���ɏ�����������Ƃ��Ďg�p
	u8 eff_loop;		//�Z�̪�ČJ��Ԃ������
}ContestSys;

//-- �e��ذ�ް���Ɏ����׸ޓ��̏W���� --//
typedef struct{
	//-- ��apsys --//
	u8 heart_actno;			//�n�[�g�A�C�R����ActNo
	u8 sorticon_actno;		//���Ԑ��䱲�݂�ActNo
	u8 heart_move:1;		//�n�[�g�A�C�R�������쒆��(0:�񓮍�	1:���쒆)
	u8 win_eff:1;			//�e��ذ�ް�̳���޳�ɑ΂��Ă̴̪�Ď��s���׸�(1:���s��)
	u8 meter_eff:1;			//�e��ذ�ް��Ұ���ɑ΂��Ă̴̪�Ď��s���׸�(1:���s��)
}BrdSysData;

//-- �Z���ʁAAP�v�Z�Ɏg�p�����׸ޗ� --//
typedef struct{
	s16	found_ap;		//��{AP
	s16 ap;				//���݂���݂�AP�l
	s16 total_ap;		//���܂ł̗݌vAP
//	s16 add_ap;			//���݂̃^�[���ő���AP�l
//	s16 sub_ap;			//���݂̃^�[���ň���AP�l
	
	u16 use_waza;		//�J��o���Z�����ް
	u16 use_wazaold;	//�O����݂ɌJ��o�����Z
	u8	con_type;		//�J��o���Z�̃^�C�v(�����������A������)
	
//	u8 ballact[2];			//���ʔ��\��ʂŎg�p�����ްق�actno
	
	u8 ranking:2;			//���݂̎����̏���
//	u8 reflect_flg:1;		//�W�Q��߰ق𒵂˕Ԃ�
//	u8 surru_flg:1;			//�W�Q��߰ق�ʂ���ذ�ް�ɽٰ����
//	u8 surru_no:2;			//���ő���Ɏ󂯂Ă��炤��ذ�ްNo������
//	u8 SpeUseless_flg:1;	//�׸ނ������Ă���ƋZ�̓�����ʂ��������Ȃ�
	u8 				:2;
	
	u8 conti_count:3;		//�A�����ē����Z���o�����Ƃ��̶���
	u8 endAppeal_flg:1;		//������߰قł��Ȃ�
//	u8 eachTime_flg:1;		//�����AP��+5����
//	u8 kouka_flg:1;			//��ٌ߰��ʂ��󂯂���
	u8 miss_flg:1;			//��َ߰��s�t���O
	
	// turn = ������݂ɸر�������׸�(��Ă��ꂽ��݂�-1�����̂�2����̫�Ăž�Ă����׸�)
	u8 yasumi_turn:2;		//1��x��
//	u8 AtoD_turn:2;			//�����A�`D�׽�̋Z�ű�߰ق���Ƃ��̋Z�̊�{��߰ق� *2 ������
//	u8 combo_turn:2;			//����1�̋Z���o�����Ƃ��ɾ��
	
//	u8 bougai_count:7;		//�W�Q���󂯂���	���������׸ނ͂��ꂼ��Ɨ���������
//	u8 bougai_flg:1;		//�W�Q���ꂽ�痧�B������݂ɂ͸ر�����
	
//	u8 waza_pri;			//���ԖڂɋZ���J��o����(�o���Z�̗D�揇�ʂɂ���Č��܂�)
	s8 bonus;				//��߰��ްŽ(����ݱ�߰��߲�Ăɉ��Z)
	
	u8 bougai;				//�� �o�Ă���Z�̌��ʂŎ󂯂��W�Q�߲��
							//(���̋Z���o�鎞�ɂ�0�N���A�����)
	
	u8 invalid;				//�W�Q�ϐ� ���̒l�����W�Q�_���[�W���猸�炷

	u8 invalid10:1;			//�W�Q���󂯂��� �_���[�W��10�ɂ���
	u8 Ashield_flg:1;		//�W�Q ��Ζh��
	u8 bougai2bai:1;		//�W�Q�����Ƒ啝��AP�l��������B�W�Q�l���W�Q�l�~�Q
	u8 penalty0:1;			//�A�����ďo���Ă��y�i���e�B���󂯂Ȃ�
	u8 bonus_flg:2;			//������݂��ްŽ���t�����A������(1:�t�����A2:������)
	u8 nextsort_flg:2;		//�u���@�΂�v�̕\��(0:�\���Ȃ� 1:�@�ɔԍ�  2:�@�ɁH)
	
	u8 now_nextsort:2;		//���������o�����Z�ŏ��Ԑ��䂪����������
							//(0:�������ĂȂ��@1:1�Ԗځ@2:4�Ԗځ@3:�H�Ԗ�)
	u8 next_yasumi_turn:1;	//������݂��I��������x�݃t���O�𗧂Ă�
	u8 next_endAppeal_flg:1;	//������݂��I��������i�v�x���׸ނ𗧂Ă�
	u8 tensyon_up:1;		//�׸ނ������Ă��鎞�͕K���ݼ�݂��㏸����
	u8 bonus3bai:1;			//�ްŽ��3�{�ɂ���
	u8				:2;
	
	u8 shield_flg;			//�ϋv�l���������h��
	
	u8 msgid;				//�Z���ʎ��̕\�����郁�b�Z�[�W
	u8 msgid2nd;			//
	
	u8 penalty:1;			//�����������è����������
	u8 before_turn_penalty:1;	//�O����݂�����è���������Ă�
	u8 combo1_invalid:1;	//�O����݂ɏo�����Z�̺���1���ʖ���
	u8 combo_conti_flg:1;	//���ނ��o�������Ƃ����邩(0:�o���ĂȂ�	1:�o�������Ƃ�����)
	u8 combo1_flg:1;		// 1:���ݺ���1�������@0:����1��ԂłȂ�
	u8 combo1_del:1;		//�Z���ʂɂ���ĺ���1�����������ꂽ����1����Ă����
	u8 combo_hassei:1;		//1=����1�A���ͺ��ނ������B�@0=���ނ̌��ʂ͉����������ĂȂ�
	u8 		:1;
	u8 combo_level;			//���ސ������̺������فB0�̎��ͺ��ޕs����
	
	u8 comboAP;
	u8 penaltyAP;
	u8 next_sort;	//������݉��Ԗڂɱ�߰ق��邩
	u8 kekka_msgid;			//��ݏI�����̱�ٌ߰��ʂ�MsgID
	
	u8 hensin_monsno;	//[�ւ񂵂�][�Ȃ肫��]���������ɉ��Ԗڂ���ذ�ް���߹�݂ɉ����邩
}ContestAPSys;

//-- �e�݂̂��l��ύX�o����V�X�e�����[�N �q�͎Q�Ƃ̂�--//
typedef struct{
//	u8 nowturn_sort[BREEDER_MAX];	//���݂���݂ɱ�߰ق��鏇��(��{�I�ɂ�BrdSort�Ɠ����B										//����BrdSort�ͱ�ُ߰I�����ɍX�V����邪�A�����
									//��ݏI�����ɍX�V�����
	//s8 tensyon;					//���݂��ݼ���߲��
	s8 tensyon_updown;			//�o�����Z�ɂ���ăe���V�����̕ω�����l
	u8 tensyon_stop:1;			//�׸ނ������Ă��鎞���ݼ�݂��㏸�A���~���Ȃ�
	u8 tensyon_stop_brd:3;		//tensyon_stop�׸ނ𗧂Ă���ذ�ް��No
	u8				:4;
	s8 tensyonAdd;
}ContestCalcWork;

//-- �q�� --//


//-- �u���[�_�[ --//
typedef struct{
	u16 monsno;				//�����߹��
	u8 nickname[MONS_NAME_SIZE+EOM_SIZE];		//�����߹�݃j�b�N�l�[��
	u8 name[PERSON_NAME_SIZE+EOM_SIZE];			//�u���[�_�[��	11
	
	u8 pattern;				//�u���[�_�[����(OBJ�R�[�h)
	//u8 ai;					//AI�p�^�[��
	u32 ai;					//AI�p�^�[��
	u8 rank:2;				//�o�ꂷ��ꏊ(0:ɰ�فA1:���߰�A2:ʲ�߰�A3:Ͻ��)
	u8 style_appear:1;		//�������悳��ýĂɓo�ꂷ�邩(0:�o�ꂵ�Ȃ� 1:�o�ꂷ��)
	u8 beautiful_appear:1;	//��������ýĂɓo�ꂷ�邩(0:�o�ꂵ�Ȃ� 1:�o�ꂷ��)
	u8 cute_appear:1;		//������ýĂɓo�ꂷ�邩(0:�o�ꂵ�Ȃ� 1:�o�ꂷ��)
	u8 clever_appear:1;		//������ýĂɓo�ꂷ�邩(0:�o�ꂵ�Ȃ� 1:�o�ꂷ��)
	u8 strong_appear:1;		//痂�����ýĂɓo�ꂷ�邩(0:�o�ꂵ�Ȃ� 1:�o�ꂷ��)
	u8				:1;
	//u8 dmmy;				4�o�C�g���E�̾��
	
	u16 waza[4];			//�����߹�݂̋Z

	u8 style;				//�����߹�݂̊i�D�悳
	u8 beautiful;			//�����߹�݂̔�����
	u8 cute;				//�����߹�݂̉���
	u8 clever;				//�����߹�݂̌���
	
	u8 strong;				//�����߹�݂̂����܂���
	u8 fur;					//�����߹�݂̂��Â�
	//u8 dmmy[2];			//4�޲ċ��E�̾��
	
	u8 msg[12];				//���b�Z�[�W(�ȈՉ�b)
	u32	personal_rnd;		//������
	u32 id_no;				//�|�P������ID
}BreederData;


#if 0	//�R�����f�[�^�͂Ȃ��Ȃ��� 2002.08.05(��)
//-- �R�����f�[�^ --//
typedef struct{
	u8 type;				//�D��������
	u8 form;				//�D���Ȍ`
	u8 waza;				//�D���ȋZ
	
	u8 dmmy;				//4�޲ċ��E�̾��
}JudgeData;
#endif

//-- �Z�ް��i�[������ --//
typedef struct{
	u8 Sort[BREEDER_MAX];	//���ذNoX����ذ�ް�͉��Ԗڂ̈ʒu�ɂ��邩
	s16 bougai;				//�W�Q�l
	s16 bougai_calc;			//�v�Z��̖W�Q�l
	u8 target[BREEDER_MAX+1];	//�I�[��0xff
	u8 nowwaza_target[BREEDER_MAX];	//���o�����Z�̖W�Q���ޯĂɂȂ�����ذ�ްNo�̏���1������
	u8 nowbrd;
}WazaKoukaWork;

//-- �V�E�Z�ް� --//
typedef struct{
	u8 apno;				//�Z����No
	
	u8 con_type	:3;			//����(�����������A���킢���A�����܂�����)
	u8			:5;
	
	u8 combo1;			//����1
	u8 combo2_0;			//����2
	
	u8 combo2_1;			//����2
	u8 combo2_2;			//����2
	u8 combo2_3;			//����2
	
	u8 dmmy[1];
}ConWazaData2;

//-- �V�E�Z�����ް� --//
typedef struct{
	u8 class;				//����
	u8 ap;					//�]���l
	u8 bougai;				//�W�Q�l
	u8 dmmy;				//4�o�C�g���E�I�t�Z�b�g
}WazaKoukaData;

//-- �Z�̪�Ẵ��[�`���ŕK�v�ȃf�[�^ --//
typedef struct{
	u16 monsno;
	u16 hensin_monsno;
	u8 hensin_flg:1;
	u8 			:7;
	u8 brd;
	u32	personal_rnd;		//������
	u32 id_no;				//�|�P������ID
	u32 hensin_personal_rnd;	//�ϐg��̌�����(��ɰ݁A�߯��ق̂�)
}ConWazaEffParam;

//=========================================================================
//	AI�p�̍\���̐錾
//=========================================================================

typedef struct{
	u8	AI_SEQNO;		//AIٰ�݂�Ҳ݂̼��ݽNo
	u16	AI_WAZANO;		//���݂̋ZNo
	u8	AI_WAZAPOS;
	u8	AI_WAZAPOINT[4];	//AI�ł̕]���l ��{100��AI�ɂ���ĉ����Z����Ă���
	u8	AI_STATUSFLAG;		//���т̏���׸�(����I�����ނ݂̂Ɏg�p)
	u32	AI_CALC_WORK;		//AI�̻��ٰ�݂̖߂�l������
	u8	AI_THINK_NO;		//
	u32	AI_THINK_BIT;		//
	s16 RetWork;			//�R�}���h�̌��ʂ�����
	s16	work[2];			//������ܰ��B�����đ��Ŏg�p
	u8	*PushAdrsBuf[8];	//���ٰ�ݺ�َ��̽���
	u8	PushAdrsCnt;		//��������Ă��鐔����
	u8  AI_Brd;
}ConAIWorkTbl;

//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern void InitContest(void);
extern void InfoMaskEffectEnd(u8 id);
extern void ContestHaikeiRecover(void);
extern void ContWazaCalcDatatWait(u8 id);


//----------------------------------------------------------
//	�O���[�o���ϐ�
//----------------------------------------------------------
extern BreederData BreederParam[BREEDER_MAX];	//��ذ�ް�ް�
/*
extern u8 BreederNo;		//��������ذ�ްNo(�ʐM����Ƃ��Ɏ��������Ԃ������悤�ɂ���)
extern u16 ConWaza[BREEDER_MAX];	//�J��o���Z�����ް
extern u8 ConCursor;	//���وʒu���
extern u8 BreederRanking[BREEDER_MAX];	//�e��ذ�ް�̌��݂̏���
*/
//extern ContestSys *consys;
//extern ContestAPSys *apsys;
//extern JudgeData Judge;
extern s16 ContEvaluation[BREEDER_MAX];
extern s16 ContBP[BREEDER_MAX];
//extern u8 conkind;
extern u8 ContestJyunni[BREEDER_MAX];
extern u8 ConTemotiNo;
extern u8 MyBreederNo;
extern u8 ContestTuusin;
extern u8 ContestSioOya;
extern u16 ContestKind;
extern u16 ContestRank;
extern u8 BrdSort[BREEDER_MAX];
extern unsigned long temp_rand;
extern s16 ContAP[BREEDER_MAX];
extern s16 NijiPoint[BREEDER_MAX];

//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
extern const BreederData bd_tbl[];
extern const u8 C_PokeNamePos[][2];
extern const u8 C_BreederPos[][2];
extern const u8 C_RankingPos[][2];
extern const u8 C_RankingNoPos[][2];
extern const u8 C_IconPos[][2];
extern const u8 C_JyunniIconPos[][2];
extern const u8 C_WazaList[4][2];
extern const u8 * const ConWazaDamejiMsgTbl[];
extern const u8 * const TurnKekkaMsgTbl[];
extern const u8 *const WazaCommentMsgTbl[];
extern const u8 *const ComboIMSG_Tbl[];
extern const CellData HeartIconCell;
extern const PalData HeartIconPal;
extern const actHeader HeartIconHeader;
extern const actHeader JudgeHeader;
extern const CellData JudgeCell;
extern const u8 HeartIconYPos[];
extern const u8 *const WazaSetumeiMsgTbl[];
extern const ConWazaData2 ConWazaTbl[];
extern const WazaKoukaData WazaKoukaTbl[];
extern const u8 *const ConTypeMsgTbl[];
extern const u8 ComboLevelTbl[];
extern const CellData FukidasiCell;
extern const PalData FukidasiPal;
extern const	actHeader FukidasiHeader;
extern const CellData VoltageIconCell;
extern const actHeader VoltageIconHeader;
extern const CellData VoltageGaugeCell;
extern const PalData VoltageGaugePal;
extern const actHeader VoltageGaugeHeader;
extern const actHeader VoltageGaugeSubHeader;
extern const CellData SortIconCell[];
extern const actHeader SortIconHeader[];
extern const u8 SortIconYPos[];
extern const OAMPatSt SortIconPat;
extern const PalData SortIconPal;














//=========================================================================
//
//		�R���e�X�gAI�p�̒�`��
//												by matsuda 2002.05.25(�y)
//=========================================================================

#endif	//__CONTEST_H__

