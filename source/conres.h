#ifndef __CONRES_H__
#define __CONRES_H__

#include "contest.h"


//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define CRES_SPACE		0//382		//���ʔ��\��ʂł̸ر�p��߰��̷�׈ʒu
#define BALL_PUT_WAIT	35		//�{�[�������o���Ă����Ԋu
#define RANKING_PUT_WAIT	20	//�ްق�\�����I����Ă����ݷݸނ��o���܂ł̳���
#define POKEWIN_PUT_WAIT	30	//�D�������߹�݂Ƴ���޳ү���ނ��o��܂ł̳���

//-- ү���ޓ]����׈ʒu --//
#define CRES_MSG_START	0x200//0x100
#define CRES_NAME_LEN	(5*2)
#define CRES_OYA_LEN	(6*2)	//�ׯ���t���Ȃ̂�6
#define CRES_NAME		CRES_MSG_START
#define CRES_OYA		(CRES_NAME + CRES_NAME_LEN*BREEDER_MAX)
#define CRES_WINMSG		(CRES_OYA + CRES_OYA_LEN*BREEDER_MAX)

//=========================================================================
//	�V�K
//=========================================================================
//----------------------------------------------------------
//	���b�Z�[�WVRAM
//----------------------------------------------------------
//-- ���b�Z�[�WCGX�� --//
#define CRES_POKENAME_LEN		(MONS_NAME_SIZE*2)
#define CRES_BRDNAME_LEN		((PERSON_NAME_SIZE+1)*2)	//+1 = �ׯ���̕�

//-- ���b�Z�[�WVRAM�W�J�ʒu --//
#define CRES_MSGCGX_START	(512+256)
#define CRES_CLEAR_MSG		CRES_MSGCGX_START
#define CRES_POKE_NAMECGX	(CRES_CLEAR_MSG+2)	//+2 = �ر�p��߰��̕�
#define CRES_BRD_NAMECGX	(CRES_POKE_NAMECGX+CRES_POKENAME_LEN*BREEDER_MAX)

//�|�P�����A�C�R���W�J��̐擪�L�����i���o�[
#define CRES_POKEICON_TOPCHRNO	512

//----------------------------------------------------------
//	���W
//----------------------------------------------------------
#define CRES_STAR_X0	19	//�ŏ�����ذ�ް�̐��̷�א擪���W
#define CRES_STAR_Y0	5
#define CRES_HEART_X0	CRES_STAR_X0	//�ŏ�����ذ�ް��ʰĂ̷�א擪���W
#define CRES_HEART_Y0	(CRES_STAR_Y0+1)

//----------------------------------------------------------
//	�X�N���[���f�[�^�萔
//----------------------------------------------------------
#define CRES_HEART_SPACE	0x60a2		//ʰċ󔒷��
#define CRES_HEART_FIX		0x60a3		//ʰ�̪��ނ��Ȃ����
#define CRES_HEART_FADE		0x60a4		//ʰ�̪��ނ��鷬�

#define CRES_HEARTBL_SPACE	CRES_HEART_SPACE		//��ʰċ󔒷��
#define CRES_HEARTBL_FIX	0x60a5		//��ʰ�̪��ނ��Ȃ����
#define CRES_HEARTBL_FADE	0x60a6		//��ʰ�̪��ނ��鷬�

#define CRES_STAR_SPACE		0x60b2		//���󔒷��
#define CRES_STAR_FIX		0x60b3		//��̪��ނ��Ȃ��L����
#define CRES_STAR_FADE		0x60b4		//��̪��ނ��鷬�

//----------------------------------------------------------
//	BG���W�X�^�ݒ�
//----------------------------------------------------------
//�L�����x�[�XNo
#define CRES_HAIKEI_CHARBASE_NO	0	//�w�i�p����ް�No
#define CRES_EFF_CHARBASE_NO	CRES_HAIKEI_CHARBASE_NO	//�̪�ėp����ް�No
#define CRES_FONT_CHARBASE_NO	CRES_HAIKEI_CHARBASE_NO	//̫�āE�߹�ݱ��݁E�ް�ޗp����ް�No
//�X�N���[��No
#define CRES_EFF_SCRN_NO		30
#define CRES_HAIKEI_SCRN_NO		26
#define CRES_FONT_SCRN_NO		24
#define CRES_WIN_SCRN_NO		28
//�L�����x�[�X�A�h���X
#define CRES_EFF_VRAM			(BG_VRAM+0x4000*CRES_EFF_CHARBASE_NO)
#define CRES_HAIKEI_VRAM		(BG_VRAM+0x4000*CRES_HAIKEI_CHARBASE_NO)
#define CRES_FONT_VRAM			(BG_VRAM+0x4000*CRES_FONT_CHARBASE_NO+0x4000)
//�X�N���[���A�h���X
#define CRES_EFF_SCRN			(BG_VRAM+0x800*CRES_EFF_SCRN_NO)
#define CRES_HAIKEI_SCRN		(BG_VRAM+0x800*CRES_HAIKEI_SCRN_NO)
#define CRES_FONT_SCRN			(BG_VRAM+0x800*CRES_FONT_SCRN_NO)
#define CRES_WIN_SCRN			(BG_VRAM+0x800*CRES_WIN_SCRN_NO)

//----------------------------------------------------------
//	
//----------------------------------------------------------
#define	CRES_POKEICON_BGPALNO	10//�߹�ݱ��݂Ŏg�p����BG��گĂ̊J�nNo(��������4�{�g�p)

//----------------------------------------------------------
//	UserWork���ڽ����U���`
//----------------------------------------------------------
#define CRES_USERWORK_START		0x18000
#define CRES_SYSTEM_ADRS		CRES_USERWORK_START
#define CRES_GRAPH_ADRS			(CRES_SYSTEM_ADRS + sizeof(CresSys))
#define CRES_MSG_DECORD_USER	(CRES_GRAPH_ADRS + sizeof(ConGraph)*BREEDER_MAX)//ү���ވꎞ�W�J�̈�

//-- �}�N�� --//
#define cres	((CresSys*)&UserWork[CRES_SYSTEM_ADRS])
#define cgraph	((ConGraph*)&UserWork[CRES_GRAPH_ADRS])
#define CRES_MSG_DECORD_ADR		(&UserWork[CRES_MSG_DECORD_USER])

//----------------------------------------------------------
//	���b�Z�[�W�E�B���h�E
//----------------------------------------------------------
#define MSGWINOBJ_DEFAULT_X		(240+32)	//��ʊO X���W
#define MSGWINOBJ_DEFAULT_Y		144			//�ʏ�ү���ގ���Y���W
#define MSGWINOBJ_SIOWAIT_Y		80			//�u�����񂽂������イ�v����Y���W
#define MSGWINOBJ_INOUT_SP		0x0440		//�ײ�޲݁A��āA�W�����x
enum{	//���ݏ󋵂������萔
	WO_TAIKI_OUT = 0,	//��ʊO�ҋ@
	WO_SLIDEIN,			//�ײ�޲ݒ�
	WO_TAIKI_IN,		//��ʓ��őҋ@
	WO_SLIDEOUT,		//�ײ�ޱ�Ē�
};
#define NEXT_MSG_WAIT			20			//����ү���ނ𗬂��܂ł̳���

//----------------------------------------------------------
//	���ƃn�[�g
//----------------------------------------------------------
#define CRES_STAR_MAX	10
#define CRES_HEART_MAX	10

//----------------------------------------------------------
//	���C���^�X�N�̃��[�N����U��
//----------------------------------------------------------
enum{
	MAIN_SEQ=0,	//�e�V�[�P���X�̒��Ŏg�p�B�g�p�����ꍇ���̼��ݽ�Ɉڂ�O�ɕK��0������
	MAIN_TEMP,
	MAIN_WORK0,
	MAIN_WORK1,
	MAIN_WORK2,
	MAIN_WORK3,
	MAIN_WORK4,
	MAIN_WORK5,
	MAIN_WORK6,
	MAIN_WORK7,
};

//----------------------------------------------------------
//	CELL ID
//----------------------------------------------------------
enum{
	CELLID_HyoukaBall0 = 3000,
	CELLID_HyoukaBall1,
	CELLID_HyoukaBall2,
	CELLID_HyoukaBall3,
	CELLID_HyoukaBall4,
	CELLID_HyoukaBall5,
	CELLID_HyoukaBall6,
	CELLID_HyoukaBall7,
	//-- ����(����) --//
	CELLID_JyunniNum,

	//-- �V�K 2002.06.24(��)--//
	CELLID_MSGWIN0,
	CELLID_MSGWIN1,
	CELLID_MSGWIN2,
	CELLID_MSGWIN3,
	CELLID_MSGWIN4,
	CELLID_MSGWIN5,
	CELLID_MSGWIN6,
	CELLID_MSGWIN7,
	
	CELLID_KAMIFUBUKI,
};

//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern void ContestResultInit(void);


//----------------------------------------------------------
//	�\���̒�`
//----------------------------------------------------------
//-- ���э\���� --//
typedef struct{
	u8 win_act;		//����޳OBJ�̐e��ActNo
	u8 siowin_act;	//�ʐMү���ޗp�̳���޳OBJ�̐e��ActNo
	u8 main_id;		//���C���^�X�N��ID
	u8 champ_winpal_task;	//����ߵݳ���޳����گı�Ґ���^�X�N
	
	u8 win_status;	//ү���޳���޳�̌��ݏ� ��enum�Q��
	u8 jyunni_put;	//�e��ذ�ް�̏��ʂ�\��������ݸ���Ă��Ă���
	u8 poke_appear;	//�߹�݂̏� 1:�ײ�޲ݏI�� 2:�ײ�ޱ�ďI��
	u8 kamifubuki_num;	//Add����Ă��鎆�ӂԂ������̐�
	u8 poke_act;	//�߹�݂�ActNo
	u8 kamifubuki_end;	//1����Ă��ꂽ�玆�ӂԂ�����߂�
	u8 gauge_fade;	//0=���AʰĂ�̪��ނ��f�̏�Ԃ̂Ƃ�  1=̪��ނ��������Ă���
	
	s16 meter[BREEDER_MAX];	//���݂�Ұ���h�b�g��
	u8 meter_eff;	// 1�ȏ��Ұ�����쒆	0�͉��������ĂȂ�
}CresSys;

//-- Ұ������\���� --//
typedef struct{
	s32 itiji;		//�ꎟ�R���|�C���g�����L����p�[�Z���e�[�W
	s32 niji;		//�񎟐R���|�C���g�����L����p�[�Z���e�[�W
	u32 itiji_dotto;	//�ꎟ�R�����̏��L���Ă���h�b�g��
	u32 niji_dotto;		//�񎟐R�����̏��L���Ă���h�b�g��
	u8	niji_mainasu;	//�񎞐R���|�C���gϲŽ�׸�(1����Ă���Ă����2���R���߲�Ă�ϲŽ)
	u8 itiji_starnum;	//�\������鐯(�ꎟ�R������)�̐�
	u8 niji_heartnum;	//�\�������ʰ�(�񎟐R������)�̐�
}ConGraph;


#endif

