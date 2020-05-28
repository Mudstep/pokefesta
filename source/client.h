
#ifndef __CLIENT_H__
#define __CLIENT_H__


#define ARROW_DEL	0	//����Ȃ��Ȃ����Q�[�W�̖��A�N�^�[���폜


//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern	void	ClientDummy(void);
extern	void	ClientInit(void);
extern	void	ClientPokeListReturn(void);
extern	void	BtlItemReturn(void);
extern	void	TrainerDelEnd(actWork *xreg);
extern	void	ClientCommandWait(void);
extern	void	CommandCsrOn(u8,u8);
extern	void	CommandCsrOff(u8);
extern	void	VoiceCheckTask(u8 id);
extern	void	ClientWazaSelect(void);
extern	u8		ClientWazaSelectShinka(void);
extern	void	ClientBSSWazaSelect(void);
extern	void	ClientBSSWazaSelectAct(void);
extern	void	ClientBSSPokeparaGet(void);
extern	void	ClientBSSPokeparaGet2(void);
extern	void	ClientBSSPokeparaPut(void);

//----------------------------------------------------------
//	�O���ϐ��錾
//----------------------------------------------------------
extern u8 ClientStruct[];	//���������Ď��p
extern u8 ClientGaugeSt[];	//�ް�ޱ����Ď��p
extern u8 GaugeArrowActNo;
extern u8 TempClientNo;
extern	u8	CommandCsrPos[];
extern	u8	WazaCsrPos[];
extern u8 ClientEffectWait;
extern u8 TemotiGauge[];
extern u32	HenshinRnd[];	//�ւ񂵂�p�̌�����

//----------------------------------------------------------
//	�萔
//----------------------------------------------------------
#define	CGX_START	0x080
#define	CGX_START2	0x180
#define	CGX_START3	0x280
#define	CSR_SCR		24

#define	CSR_CHR_U	0x0001
#define	CSR_CHR_D	0x0002
#define	SPACE_CHR_D	0x000a
#define	SPACE_CHR_B	0x1016

#define	WIN_CHR_UL	0x1022
#define	WIN_CHR_U	0x1023
#define	WIN_CHR_UR	0x1024
#define	WIN_CHR_ML	0x1025
#define	WIN_CHR_M	0x1026
#define	WIN_CHR_MR	0x1027
#define	WIN_CHR_DL	0x1028
#define	WIN_CHR_D	0x1029
#define	WIN_CHR_DR	0x102a

//-- �퓬��ʳ���޳��ү���ޕ\�����W --//
#define CL_WINMSG_X	2
#define CL_WINMSG_Y	15

//-- �莝���{�[�������\������Ă��鎞�� --//
#define TEMOTI_BALL_WAIT	92

//-- �G���̏���̎莝���Q�[�W���o���^�C�~���O�������x�点�鳪�� --//
#define ENEMY_TEMOTI_INIT_WAIT	2

#include	"client.def"


//-- ���̑��G�t�F�N�g(EtcEffSeqTbl)�p��ID --//
enum{
	FE_ID_LVUP,			//���x���A�b�v���̃G�t�F�N�g
	FE_ID_POKERETURN_MINE,	//���@���|�P�����������߂�G�t�F�N�g
	FE_ID_POKERETURN_ENEMY,	//�G���|�P�����������߂�G�t�F�N�g
	FE_ID_POKEGET,		//�߂܂��G�t�F�N�g
	FE_ID_POKEGET_SAFARI,	//�߂܂��G�t�F�N�g(�T�t�@���p)
	FE_ID_MIGAWARI_OUT,	//�u�݂����v�L������ײ�ޱ��
	FE_ID_MIGAWARI_IN,	//�u�݂����v�L������ײ�޲�
};


/*----------------------------------------------------------
	�߹�݂̱���ܰ��g�p��
�풓
	work[0] = �����̸ײ������ް
�ȉ������ؗ̈�
	work[1] = ��Ұ�ނ̓_�Ŷ�����Ƃ��Ďg�p
	work[2] = monsno
�����g�p
-----------------------------------------------------------*/







//----------------------------------------------------------
//	�ײ��ļ��э\���̐錾
//----------------------------------------------------------

//��ʐ؂�ւ�����ێ�����f�[�^�͂����ɒǉ����Ă���
typedef struct{
	u8 act_banish:1;		//�ײ��Ă��߹�݂���Ư����Ԃ��ǂ�����ێ�
	u8 pinch_se:1;			//1=�������̌x���������Ă�
	u8 migawari:1;			//�u�݂����v���s���׸�(1:���s���@0:���ĂȂ�)
	u8 migawari_out:1;		//�u�݂����v���������߂Ă��邩(1:�������ߒ��A0:��ɏo�Ă�)
	u8 gauge_mode:1;		//HP�ް�ނ̕\������(0:�ް�ŕ\���@1:���l�ŕ\��)
	u8 			:3;
	u16 hensin_monsno;		//�ϐg���������߹�ݔԍ���ێ�(0�̏ꍇ�͕ϐg���ĂȂ�)
}ClientMemory;

//��ʐ؂�ւ��ŏ����Ă������悤�ȏ��͂��̍\���̂ɒǉ����Ă���
typedef struct{
	u8 temoti_gauge_flg:1;	//1:�莝���ް�ޏo���Ă�		0:�o���ĂȂ�
	u8 gauge_motion_set:1;	//1:�I�𒆂̂���炷�铮�����s��	0:���ĂȂ�
	u8 poke_motion_set:1;
	u8 ball_eff_wait:1;		//1:�ްق���o���̪�Ď��s��	0:���ĂȂ�
	u8 status_eff:1;		//1:�ð���ُ�̪�Ĕ�����
	u8 fight_eff:1;			//1:�퓬�̪�Ĕ�����
	u8 etc_eff:1;			//1:���̑��̪�Ĕ�����
	u8 rare_eff:1;			//1:���A�o��G�t�F�N�g���s��
	
	u8 rare_eff_end:1;		//1:���A�o��G�t�F�N�g�I��
	u8 temoti_wait:5;		//���񎞂̎莝���Q�[�W���o���܂ł̳���
	u8		:2;
	
	u8 gauge_motion_no;		//�I�𒆂̂���炷�铮�������s���铧��������No
	u8 poke_motion_no;
	u8 WazaEffSeqNo;
	u8 wait_work;			//wait�̶����p��ܰ�
	u8 poke_aff_no;			//�e�߹�݂��擾���Ă���AffineNo ���Z�̪�Ăňꎞ�I��0�ر����̂ł����ɑޔ�������
	u8 shadow_act;			//�߹�݂̉e��ActNo
	u8 setime_wait;			//SE�I���҂�����ѱ�Ķ���
	u8 send_out_wait;		//SendOut�I�����̳���
}ClientSystem;

//�e�ײ��Ė��ɂ͎����Ȃ����[�N ����ʐ؂�ւ��ŏ�����
typedef struct{
	u16 work[4];			//FightEffectSet�œn������
	u8 get_rate;			//�߹�ݹޯĴ̪�ĂŎg�p(�ްق̸޶މ�)
	u8 sendout_eff:1;		//SendOut�G�t�F�N�g��
	u8 get_banish:1;		//�߹�ݹޯĴ̪�đO�ɑΏۂ��߹�݂���Ư������ۑ�
	u8			:6;
	
	u8 hikari_count;		//�ݽ���ްُo���̌���ׂ���Ă��ꂽ�񐔂���
	s16 ball_temp;			//�ޯĴ̪�č�Ɨp������ܰ�
}BtlWorkData;

//�e�ײ��ẴQ�[�W�̌v�Z�p���[�N�Ƃ��Ďg�p
typedef struct{
	u8	actno;			//�Q�[�W�̱����̔z��ԍ�
	s32 MaxHP;			//�ő�HP(�ް��ϯ���l)
	s32 NowHP;			//���݂�HP(��Ұ�ނ��󂯂�O)
	s32 beHP;			//�󂯂���Ұ��(�񕜂���ꍇ��ϲŽ�œ����) �ϓ�������l
	//s16 HP_Work;		//��Ɨpܰ�
	s32 HP_Work;		//2002.09.14(�y)
}GaugeData;

#define CLIENT_MEMORY_SIZEOF	(sizeof(ClientMemory)*CLIENT_MAX)
#define CLIENT_SYSTEM_SIZEOF	(sizeof(ClientSystem)*CLIENT_MAX)
#define BTLWORKDATA_SIZEOF		(sizeof(BtlWorkData))
#define GAUGE_DATA_SIZEOF		(sizeof(GaugeData)*CLIENT_MAX)

#define	MATSUDA_USER_WORK_START	0x17800	//���c�N���퓬�Ŏg�p�ł���UserWork�̃X�^�[�g
#define CLI_MEMORY_USERWORK		MATSUDA_USER_WORK_START
#define CLI_SYS_USERWORK		(CLI_MEMORY_USERWORK+CLIENT_MEMORY_SIZEOF)
#define BTLWORKDATA_USERWORK	(CLI_SYS_USERWORK+CLIENT_SYSTEM_SIZEOF)
#define GAUGE_DATA_USERWORK		(BTLWORKDATA_USERWORK+BTLWORKDATA_SIZEOF)
//#define CLI_DMMY				(GAUGE_DATA_USERWORK+BTLWORKDATA_SIZEOF)


//----------------------------------------------------------
//	�N���C�A���g�̼��э\���̂ɱ������鎞�Ɏg�p����}�N��
// 0x18000�����O�ɒ�`����Ă���̂ŉ�ʐ؂�ւ�����ێ�����Ă�f�[�^
//----------------------------------------------------------
#define Climem	((ClientMemory*)&UserWork[CLI_MEMORY_USERWORK])
#define CliSys	((ClientSystem*)&UserWork[CLI_SYS_USERWORK])
#define BtlWork	((BtlWorkData*)&UserWork[BTLWORKDATA_USERWORK])
#define gwork	((GaugeData*)&UserWork[GAUGE_DATA_USERWORK])


// �w�i�؂�ւ��Ž�ذ��ް���W�J���Ă���̂ź�ýĂł�1000h�m��
#define CONTEST_WAZAEFF_USERWORK	0x14800	//��ýĂ̋Z�̪�ėp��UserWork
// �퓬�ͽ�ذ��ް��̈ꎞ�W�J�͂��Ă��Ȃ��̂�800h��OK
#define BATTLE_WAZAEFF_USERWORK	0x18000	//�퓬�̋Z�̪�ėp��UserWork
//�Z�̪�ĂŎg�p�ł���UserWork
#define WAZAEFF_USERWORK		\
			(ContestCheck() ? CONTEST_WAZAEFF_USERWORK : BATTLE_WAZAEFF_USERWORK)
#define WAZAEFF_USERWORK_SIZE		0x0800




#endif //__CLIENT_H__

