//=========================================================================
//	Define�����`
//=========================================================================

#ifndef __CLI_DEF_H__
#define __CLI_DEF_H__


//----------------------------------------------------------
//	�߹�݂̍��W(1vs1��2vs2)
//----------------------------------------------------------
//-- ���� --//
#define C_POKE_X		72//80					//1vs1
#define C_POKE_Y		(80)
#define C_POKE1X		32					//2vs2
#define C_POKE1Y		C_POKE_Y
#define C_POKE2X		90
#define C_POKE2Y		(C_POKE1Y+8)
//-- �G --//
#define CAI_POKE_X		176					//1vs1
#define CAI_POKE_Y		((32-8)+16)
#define CAI_POKE1X		(208-8)			//2vs2
#define CAI_POKE1Y		CAI_POKE_Y
#define CAI_POKE2X		(144+8)
#define CAI_POKE2Y		(CAI_POKE_Y-8)

#if 0	// 2002.06.19(��)

#define C_POKE1PRI		5					//������ײ��è
#define C_POKE2PRI		C_POKE1PRI			//������ײ��è
#define CAI_POKE1PRI	C_POKE1PRI
#define CAI_POKE2PRI	C_POKE1PRI

#else	//++++++++++++++++++++++

#define C_POKE1PRI		30		//����1		����(���)��ײ��è
#define C_POKE2PRI		20		//����2
#define CAI_POKE1PRI	40		//�G1
#define CAI_POKE2PRI	50		//�G2

#endif

#define POKE_OAM_PRI	2	//�߹�ݱ�����OAM����ײ��è(ʰ��)

//----------------------------------------------------------
//	�ڰŰ
//----------------------------------------------------------
#define C_TRN_X			80			//�ڰŰ��X���W
#define C_TRN_Y			80//(C_POKE_Y)
#define C_TRN_PRI		C_POKE1PRI

//#define TRAINER_OUT_SP	(-4)
#define TRAINER_RETURN_SYNCNUM	50	//60			//����
#define ENEMYTRAINER_RETURN_SYNCNUM		35	//�G

//----------------------------------------------------------
//	�߹�݂���Ұ�ނ��󂯂��Ƃ��̓_��
//----------------------------------------------------------
#define BLINK_INTERVAL		4	//�_�ł���Ԋu
#define DAMAGE_BLINK_WAIT	(BLINK_INTERVAL*8)	//�_�Ŵ̪�Ă̑�����

//----------------------------------------------------------
//	�ϐg������گĂ̐F�𗎂�����EVY�l
//----------------------------------------------------------
#define HENSIN_EVY		6

//----------------------------------------------------------
//	�I�𒆂̃|�P�����A�Q�[�W������炷�铮��
//----------------------------------------------------------
#define GAUGE_MOTION_ADDSEC		7
#define GAUGE_MOTION_FURIHABA	1
#define POKE_MOTION_ADDSEC		7	//5
#define POKE_MOTION_FURIHABA	1	//2
#define GAUGE_ENEMY_MOTION_ADDSEC		15	//�G�̃Q�[�W
#define GAUGE_ENEMY_MOTION_FURIHABA	1


//----------------------------------------------------------
//	�ݽ���ްق���o�鉌
//----------------------------------------------------------
#define MBSMOKE_PRI		2					//������ײ��è
#define MBSMOKE_ANMWAIT	4
#define MBSMOKE_CELL	1					//0:�풓  1:�g�p���邽�т�CellSet&PalSet



//----------------------------------------------------------
//	�ްق���o�Ă��鉌�̊g�k�ް�
//----------------------------------------------------------
#define MBS_AFF_SP		11
#define MBS_AFF_LOOP	18
#define MBS_START_AFF	(0x70)	//�ŏ��̊g�k�l

//----------------------------------------------------------
//	�߹�݋C��
//----------------------------------------------------------
#define C_POKE_KIZETU_SP	5			//�߹�݂����ɗ����Ă�����߰��

//----------------------------------------------------------
//	�ݽ���ްَg�p�̪��
//----------------------------------------------------------
#define MONS_BALL_X		C_POKE1X	//�����ʒu
#define MONS_BALL_Y		C_POKE1Y
#define MONS_BALL_PRI	(C_POKE1PRI -1)	//������ײ��è
#define SUCCESS_RATE	4			//�ޯĐ������[�g

//----------------------------------------------------------
//	�ް��
//----------------------------------------------------------
// �ް�ޏ����ʒu
//-- 1vs1 --//
#define C_GAUGE_X			158//152
#define C_GAUGE_Y			88//86//70
#define CAI_GAUGE_X			44//48//24
#define CAI_GAUGE_Y			30//32//20
//-- 2vs2 --//
#define GAUGE_OFFSET_X		12	//�G�A�����̹ް�ނ̵̾�Ă𓝈�ɂ��邽��
#define GAUGE_OFFSET_Y		25

#if 0	//2002.08.14(��)

#define C_GAUGE_X2			(240-96+27)
#define C_GAUGE_Y2			102//86
#define C_GAUGE_X1			(C_GAUGE_X2 - GAUGE_OFFSET_X)
#define C_GAUGE_Y1			(C_GAUGE_Y2 - GAUGE_OFFSET_Y)

#elif 0	//2002.08.16(��)

#define C_GAUGE_X2			(240-96+27)
#define C_GAUGE_Y1			102//86
#define C_GAUGE_X1			(C_GAUGE_X2 - GAUGE_OFFSET_X)
#define C_GAUGE_Y2			(C_GAUGE_Y1 - GAUGE_OFFSET_Y)

#else

#define C_GAUGE_X2			(240-96+27)
#define C_GAUGE_Y2			102//86
#define C_GAUGE_X1			(C_GAUGE_X2 - GAUGE_OFFSET_X)
#define C_GAUGE_Y1			(C_GAUGE_Y2 - GAUGE_OFFSET_Y)

#endif

#if 0	//

#define CAI_GAUGE_X2		32//24
#define CAI_GAUGE_Y2		19//0
#define CAI_GAUGE_X1		(CAI_GAUGE_X2+GAUGE_OFFSET_X)//40	//�G��
#define CAI_GAUGE_Y1		(CAI_GAUGE_Y2+GAUGE_OFFSET_Y)

#elif 0	// 2002.07.26(��) �G�̃Q�[�W�̈ʒu��ύX

#define CAI_GAUGE_X2		32//24
#define CAI_GAUGE_Y1		(CAI_GAUGE_Y2+GAUGE_OFFSET_Y)
#define CAI_GAUGE_X1		(CAI_GAUGE_X2+GAUGE_OFFSET_X)//40	//�G��
#define CAI_GAUGE_Y2		19//0

#else	//2002.08.14(��)

#define CAI_GAUGE_X2		32//24
#define CAI_GAUGE_Y2		(CAI_GAUGE_Y1+GAUGE_OFFSET_Y)
#define CAI_GAUGE_X1		(CAI_GAUGE_X2+GAUGE_OFFSET_X)//40	//�G��
#define CAI_GAUGE_Y1		19//0

#endif

//-- �ް�� ������ײ��è --//
#define GAUGE_ACT_PRI		1					//�ް�ޖ{��
#define HP_GAUGE_ACT_PRI	(GAUGE_ACT_PRI-1)	//HPҰ��
#define GAUGE_ARROW_PRI		(GAUGE_ACT_PRI+1)	//���
//-- �ް�� �n�[�h�v���C�I���e�B --//
#define GAUGE_OAM_PRI		1

//-- �莝���|�P�����\���Q�[�W --//
#define TEMOTI_GAUGE_Y		68//60
#define TEMOTI_GAUGE_PRI	10
#define TEMOTI_BALL_PRI		(TEMOTI_GAUGE_PRI-1)
#define TEMOTI_BALLPUT_WAIT	180	//�Q�[�W�������܂ł̳���
#define MINE_TEMOTI_GAUGE_X	0x88//154	//�Q�[�W�\�����W
#define MINE_TEMOTI_GAUGE_Y	96
#define TEMOTI_GAUGE_SP		5
#define TEMOTI_GAUEG_MOVE_SYNC	20
#define ENEMY_TEMOTI_GAUGE_X	0x68//45
#define ENEMY_TEMOTI_GAUGE_Y	16//12	//40
#define TEMOTI_ENEMY2_X		0x68//45
#define TEMOTI_ENEMY2_Y		40	//12

//���ٹް�ޓ]����דW�J����ڽ
#define GAUGE_DECORD_ADR	0x2000000
//�]����ו��сAį�߷�ׂ���̍���
#define HP_CHR_OFFSET		0
#define EXP_CHR_OFFSET		0
#define NUM_CHR_OFFSET		18
//#define LV_CHR_OFFSET		40	//add 2001.08.08(��)
#define NAME_CHR_OFFSET		41//39
//��ד]���ʒu
#define MINE_LV_POS			0x49//36
#define MINE_LV_POS2		0x29	//2vs2
#define MINE_HP_POS			0x1d	//0x1c	//29
#define MINE_HPMAX_POS		0x59	//0x2c	//(32+25)
#define MINE_GAUGE_HPPOS1	22
#define MINE_GAUGE_HPPOS2	(32+16)
#define MINE_GAUGE_EXPPOS1	35		//����
#define MINE_GAUGE_EXPPOS2	27		//�E��
#define ENEMY_HPGAUGE_POS1	(64+20)
#define ENEMY_HPGAUGE_POS2	(64+48)
#define ENEMY_LV_POS		0x28//(64+36)
#define ENEMY_HP_POS		0x14	//(MINE_HP_POS + 64)
#define ENEMY_HPMAX_POS		0x30	//(MINE_HPMAX_POS + 64)
#define LVPOS_2VS2			MINE_LV_POS


//----------------------------------------------------------
//  ������Define��`
//----------------------------------------------------------
#define DECORD_WORK_ADR		(0x02010000)	//DecordWram�œW�J����Ƃ���RAM���ڽ
//#define ACTOR_PUSH_ADR		0x2003000		//������Push�APop���鎞�̱��ڽ
#define ACTOR_PUSH_ADR	(0x2028000 - ALL_ACTWORK_SIZEOF -1)//������Push�APop���鎞�̱��ڽ

//----------------------------------------------------------
//	�f�o�b�N�׸�
//----------------------------------------------------------
#define GAUGE_DEBUG			1	// 1 = ���ޯ�Ӱ��

//----------------------------------------------------------
//	ýėp
//----------------------------------------------------------
#define TEST_ENEMY_X		(176)
#define TEST_ENEMY_Y		32


//----------------------------------------------------------
//	����޾ڸĉ�ʶ��وʒu
//----------------------------------------------------------
#define FIGHT_CSRPOS		0
#define ITEM_CSRPOS			1
#define POKE_CSRPOS			2
#define ESCAPE_CSRPOS		3

//�T�t�@���]�[���p
#define BALL_CSRPOS			0
#define CUBE_CSRPOS			1
#define APPROACH_CSRPOS		2


//----------------------------------------------------------
//	FightEffect���s�O��SE�I���҂�����ѱ�Ķ���-
//----------------------------------------------------------
#define EFF_SETIME_OUT_COUNT	30


#endif	//__CLI_DEF_H__
