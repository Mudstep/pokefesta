#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

//=========================================================================
//
//
//	�Q�[���i�s�p�Z�[�u�f�[�^�̒�`
//
//
//=========================================================================

#include "evwkdef.h"			// �t���O�̈�̒�`
#include "evobjsv.def"			// EventOBJ�\���̒�`
#include "param.h"				// �|�P�����֘A�̒�`
#include "mail.h"				// ���[���֘A�̒�`
#include "popword_data.h"		// ���s��֘A�̒�`
#include "rtc.h"				// �Q�[�������Ԋ֘A�̒�`
#include "cube_def.h"			// �L���[�u�֘A�̒�`




//=========================================================================
//	
//	�f�[�^��`
//
//=========================================================================

//--------------------------------------------------------------
//	�����񐧌�֘A
//--------------------------------------------------------------

#define	POKE_ZUKAN_LEN	52		/* 52 x 8 = 416 �� KAIHATSU_NUM_MAX */

typedef struct {
	u8 ListSort;
	u8 ListMode;
	u8 ZenkokuZukanFlag;
	u8 dummy;
	u32 UnknownRand;
	u32 PachiRand;
	u32 Rand1;
	u8 zukan_get_flag[ POKE_ZUKAN_LEN ];	//�|�P�����߂܂����t���O
	u8 zukan_see_flag[ POKE_ZUKAN_LEN ];	//�|�P�����݂����t���O
} ZUKAN_SV_WORK;



/* ���ʒ�` */
enum{
		MALE,			// ���Ƃ��̂�
		FEMALE,			// ����Ȃ̂�
};


enum{
	SXY_OBJ_MAX	= 64,	// SXY��OBJ�f�[�^�̍ő�o�^��
};

//------------------------------------------------------------------------
// �}�b�v�ʒu
//------------------------------------------------------------------------
typedef struct {
		s8 div_id;
		s8 map_id;
		s8 exit_id;
		s16 x;
		s16 y;
} MapID;

//------------------------------------------------------------------------
// �ǂ����֘A
//------------------------------------------------------------------------
typedef	struct {

	u16	item;	//�A�C�e���ԍ�
	u16	no;		//��

}MINEITEM;

#define MY_PCBOX_ITEM_MAX	50
#define MY_NORMAL_ITEM_MAX	20
#define MY_EXTRA_ITEM_MAX	20
#define MY_BALL_ITEM_MAX	16
#define MY_SKILL_ITEM_MAX	64
#define MY_SEED_ITEM_MAX	46


//------------------------------------------------------------------------
//------------------------------------------------------------------------
#define	EXTRA_EVENT_WORK_MAX	20
#define	EXTRA_EVENT_FLAG_MAX	20

//------------------------------------------------------------------------
//	�����L���O�p�J�E���^�֘A
//------------------------------------------------------------------------
#define SV_RECORD_MAX		50


//------------------------------------------------------------------------
//		�^�l�֘A
//------------------------------------------------------------------------

//������ꂽ���݂̂̏�ԕۑ����[�N��`
typedef struct SEED_BED_ST {
	u8 type;			// �^�l�̎��
	u8 growth:7;		// �����i�K
	u8 hook:1;			// �����}�~�t���O
	u16 time;			//	���̐����܂ł̎���
	u8 fruit_count;		// �^�l�̂ł�����

	u8 seeds:4;			// �^�l���ł�����
	u8 w_ug_bit:1;		//	�����r�b�g�F�A��������
	u8 w_hutaba_bit:1;	//	�����r�b�g�F��̒i�K
	u8 w_miki_bit:1;	//	�����r�b�g�F���̒i�K
	u8 w_hana_bit:1;	//	�����r�b�g�F�Ԃ̒i�K

}SEEDBED;

#define SEEDBED_MAX	128



#define		TAG_MSG_SIZE			44

//���̂݃p�����[�^
typedef struct SEED_PARAM_TAG {
	u8 name[SEED_NAME_SIZE+EOM_SIZE];	//���O
	u8 hardness;						//����
	u16 size;							//�傫��
	u8 f_max,f_min;						//�Ԃ̐��i�ő�A�ŏ��j
	const u8 * tag1;					//�^�O��i
	const u8 * tag2;					//�^�O���i
	u8 speed;							//����x

	u8 karai;				//�h��
	u8 sibui;				//�a��
	u8 amai;				//�Â�
	u8 nigai;				//�ꂳ
	u8 suppai;				//�_���ς�
	u8 umai;				//�|��

}SEED_PARAM;

//RAM���̂݃f�[�^
typedef struct {
	SEED_PARAM	param;
	u8 cgx[0x20*36];
	u16 pal[16];
	u8 tag1[TAG_MSG_SIZE+EOM_SIZE];
	u8 tag2[TAG_MSG_SIZE+EOM_SIZE];
	u8 item_param[18];
	u8 item_equip;
	u8 item_power;
	u32 checksum;
}RAM_SEED;



//------------------------------------------------------------------------
//		�閧��n �֘A
//------------------------------------------------------------------------

typedef struct{
	u32 p_rnd[TEMOTI_POKEMAX];		/* ������ */
	u16 waza[4*TEMOTI_POKEMAX];		/* �킴 */
	u16 mons_no[TEMOTI_POKEMAX];	/* �|�P�����i���o�[ */
	u16 item[TEMOTI_POKEMAX];		/* �ǂ��� */
	u8 level[TEMOTI_POKEMAX];		/* ���x�� */
	u8 exp[TEMOTI_POKEMAX];			/* �w�͒l�̕��� */
}BASE_BATTLE_DATA;					/* ������ԑΐ�̃��[�N��` */


typedef struct {
	u8 pos;								// �閧��n�̏ꏊ

	u8 entry_flag:4;					// �o�^�t���O
	u8 sex:1;							// ����
	u8 fight_flag:1;					// �ΐ�t���O
	u8 flg:2;							// �o�^�p�t���O(0:���o�^ 1:�o�^)

	u8 name[PERSON_NAME_SIZE];			// ������̖��O
	u8 id[4];							// �������ID
	u16 org_count;						// �I���W�i���J�E���g(�^�C���X�^���v�̂悤�Ȃ��́j
	u8 enter_count;						// �K�ꂽ��
	u8 exchange_item;					// �����p�A�C�e���i�͗l�ւ��O�b�Y�̂݁j

	// �͗l�ւ��f�[�^
	u8 furniture_id[16];				// id
	u8 furniture_pos[16];				// ���4�r�b�g:x ����4�r�b�g:y

	BASE_BATTLE_DATA battle;

}SECRET_BASE_DATA;


#define SECRET_BASE_MAX 20

#define	GOODS_DESK_MAX		10		// ����
#define	GOODS_CHAIR_MAX		10		// ����
#define	GOODS_PLANT_MAX		10		// ���A��
#define	GOODS_ETC_MAX		30		// ���炭��
#define	GOODS_MAT_MAX		30		// �}�b�g
#define	GOODS_POSTER_MAX	10		// �|�X�^�[
#define	GOODS_DOLL_MAX		40		// �ʂ������
#define	GOODS_CUSHION_MAX	10		// �N�b�V����

#define	MYROOM_FURNITURE_MAX	12

//------------------------------------------------------------------------
//		 �s�u�C�x���g���[�N
//------------------------------------------------------------------------

typedef struct {
	u8 datatype;				//�f�[�^�^�C�v(0=�f�[�^�Ȃ�)
	u8 watch_flag;
	u8 dmy[34];
}TV_EVENT;

#define	TV_WORK_MAX		25

typedef struct {
	u8 datatype;
	u8 watch_flag;
	u16 time_count;
}TV_CM;

#define	TVCM_WORK_MAX	16

//--------------------------------------------------------------
//						�莆�^(��������)
//					��ʔ����̃C�x���g�f�[�^���[�N
//--------------------------------------------------------------
typedef struct{
    u16 pokeno;				// �|�P�����̎��
    u8  map_id;				// �ǂ��ŏo��������
    u8  div_id;				// �ǂ��ŏo��������

    u8  level;				// �ǂ̃��x���ŏo�Ă��邩
    u8  special;			// ����\��
    u16 itemno;				// �����Ă���A�C�e��

    u16 waza[4];			// �����Ă���Z

    u8  character;			// ���i
    u8  appearance;			// �o��(%)
    u16 time_count;			// �C�x���g�I���܂ł̎c�莞��
}Generating_Event_Work;		// 20byte

//--------------------------------------------------------------
//					�C���^�r���A�[&�J�����}��
//--------------------------------------------------------------
typedef struct{
    u16 pokeno[2];			// �����̃|�P�����̎��

    u16 waza;				// �������Ō�Ɏg�����Z
	u16 easytalk_no;		// �퓬��̈ꌾ

    u8  worldpos;			// ������ꏊ
    u8  battle;				// �������

    u8  damage_flag:1;		// �m�[�_���[�W�t���O
    u8  kizetsu_flag:1;		// �m���t���O
    u8  kaihuku_flag:1;		// �񕜃A�C�e���t���O
    u8  throw_flag:1;		// �����X�^�{�[���t���O
    u8  watch_flag:1;		// �t���O
    u8  work:3;				// �]��

    u8  old_damage_flag:1;	// �m�[�_���[�W�t���O
    u8  old_kizetsu_flag:1;	// �m���t���O
    u8  old_kaihuku_flag:1;	// �񕜃A�C�e���t���O
    u8  old_throw_flag:1;	// �����X�^�{�[���t���O
    u8  old_work:4;			// �]��
}TV_CAMERA;					// 12byte


//------------------------------------------------------------------------
//		�G�`���C�x���g�֘A
//------------------------------------------------------------------------
typedef struct {
	u32 personal_rnd;							//������
	u32 idno;									//�|�P�����h�c
	u16 monsno;									//�|�P�����i���o�[
	u8	pict_type;								//�G�̎��
	u8	nickname[MONS_NAME_SIZE+EOM_SIZE];		//�|�P�����̖��O
	u8	oyaname[PERSON_NAME_SIZE+EOM_SIZE];		//�e�̖��O
}PORTRAIT_DATA;

#define	PORTRAIT_MAX	13


//------------------------------------------------------------------------
//------------------------------------------------------------------------

//--------------------------------------------------------------
//	�g���[�i�[�Đ�	�֘A
//--------------------------------------------------------------
#define	REVENGER_MAX			100

//--------------------------------------------------------------
//	��ĉ��@�֘A
//--------------------------------------------------------------

typedef struct{									//���R�[�h�R�[�i�[�Ή����[���\����
	MAIL_DATA	Mail;								//���[���f�[�^
	u8 oyaname[PERSON_NAME_SIZE+EOM_SIZE];			//���[���̎�����̖��O
	u8 pokename[MONS_NAME_SIZE+EOM_SIZE];			//���[���̂��Ă��|�P�����̖��O
}SIO_MAIL_DATA;

typedef struct{									//��ĉ��\����
	PokemonPasoParam	Pokemon[2];					//�a������|�P����
	SIO_MAIL_DATA		SioMail[2];					//���R�[�h�R�[�i�[�Ή����[���\����
	u32					WalkCount[2];				//�����J�E���g
	u16					Egg;						//�^�}�S(0=���� 0�ȊO=�L��=�����̃^�l
	u8					EggCount;					//�^�}�S�z���J�E���g
}SODATEYA_WORK;


//--------------------------------------------------------------
//	�ʐM�ΐ팋�ʊ֘A
//--------------------------------------------------------------
#define	TAISEN_SCORE_MAX	5

typedef struct {
	u8 name[PERSON_NAME_SIZE];
	u16	id;
	u16 win;
	u16 lose;
	u16 draw;
}TAISEN_SCORE;

typedef struct {
	TAISEN_SCORE score[TAISEN_SCORE_MAX];
	u16 total_win;
	u16 total_lose;
	u16 total_draw;
}SCORE_TABLE;


//--------------------------------------------------------------
//	�؂̎��u�����_�[�֘A
//--------------------------------------------------------------
#define BPM_TYPE_MAX		3

//--------------------------------------------------------------
//	�ǉ����{���֘A
//--------------------------------------------------------------
#define EXTRA_RIBBON_MAX	11		// �ǉ����{���̐�


//--------------------------------------------------------------
//	�o�g���^���[�@�֘A
//--------------------------------------------------------------
typedef struct {
	StockBattleTowerTrainer	SBTT_Mine;	//�X�g�b�N�g���[�i�[����
	StockBattleTowerTrainer	SBTT[5];	//�X�g�b�N�g���[�i�[�i5�l���j
	StockBattleTowerTVData	SBTTVD;		//�o�g���^���[��TV���p�̃X�g�b�N�f�[�^
	StockBattleCardETrainer	SBTT_CardE;	//�J�[�h������̃X�g�b�N�f�[�^

	u8	BattleTowerLevel	:1;			//�o�g���^���[�̃��x��
	u8	BattleTowerSaved	:1;			//�Z�[�u�ς݂��ǂ���
	u8						:6;
	u8	BattleTowerWinLoseFlag;		//�o�g���^���[�ł̐퓬���ʃt���O
	u8	BattleTowerSeqNo[2];
	u16	BattleTowerRound[2];		//�����l�ځH
	u16	BattleTowerStage[2];		//�������ځH
	u16	BattleTowerWinRecord[2];	//�A���L�^
	u8	BattleTowerTrainerNo;		//�o�g���^���[�g���[�i�[�i���o�[
	u8	BattleTowerSelectPoke[3];	//�I�������|�P�����̈ʒu
	u16	BattleTowerItemNo;			//�A���ł��炦��A�C�e���i���o�[
	u8	TrainerNoStock[6];			//�ΐ킵���g���[�i�[�i���o�[�̃X�g�b�N

	u16	BattleTowerKachi;			//�o�g���^���[�ł̏�����
	u16	BattleTowerKachinuki;		//�o�g���^���[�ł̘A����
	u16	BattleTowerRensyouNow[2];	//�o�g���^���[�ł̘A�����i����or�O��j

	u8	BattleTowerInterviewLevel;	//�o�g���^���[�̃��x���i�C���^�[�r���[�p�j
	u8	work[3];					//4�o�C�g���E�␳�i�ꉞ�j

	u8	dummy[2000-40-(sizeof(StockBattleTowerTrainer)*7)-sizeof(StockBattleTowerTVData)];
} BATTLE_TOWER_WORK;

//--------------------------------------------------------------
//	�ړ��|�P�����@�֘A
//--------------------------------------------------------------
typedef struct {
	u8 dummy[28];
} MOVING_POKEMON;

//--------------------------------------------------------------
//	�O���ǉ��X�N���v�g�֘A
//--------------------------------------------------------------
#define	EX_SCR_SIZE		1000
typedef struct {
	u32 checksum;
	u8	work[EX_SCR_SIZE];
} EXTRA_SCRIPT_WORK;

//--------------------------------------------------------------
//	�z�z�A�C�e���֘A
//--------------------------------------------------------------
#define	EX_ITEM_MAX		3
typedef struct {
	u8 type;
	u8 count;
	u16 number;
} EXTRA_ITEM;

typedef struct {
	u32 checksum;
	EXTRA_ITEM item[EX_ITEM_MAX];
} EXTRA_ITEM_WORK;

//=========================================================================
//	
// �v���C���[�V�X�e���f�[�^
//
//=========================================================================

typedef struct PersonalData{
		u8 my_name[PERSON_NAME_SIZE+EOM_SIZE];			//	�����̖��O
		u8 my_sex;					//	�����̐���
		u8 save_flag;				//	�Z�[�u�������ǂ����̃t���O
		u8 ID[4];					//	�h�c�i���o�[

		u16 playtime_h;				//	�v���C���ԁi���j
		u8 playtime_m;				//	�v���C���ԁi���j
		u8 playtime_s;				//	�v���C���ԁi�b�j
		u8 playtime_ms;				//	�v���C���ԁi1/60�b�j

		u8 key_config;				//	�I�v�V�����̃L�[�ݒ�
		u16 msg_speed:3;			//	�l�r�f����̑���
		u16 window_type:5;			//	�V�X�e���E�B���h�E�̎��
		u16 sound_mode:1;			//	�X�e���I�E���m�����؂�ւ�
		u16 btl_type:1;				//	�u�����ʂ��v�u���ꂩ���v
		u16 btl_eff_sw:1;			//	�퓬�G�t�F�N�g�̗L��
		u16 tnavi_zoom:1;			//	�g���i�r�}�b�v���[�h

		ZUKAN_SV_WORK Zukan;

		GAME_TIME dst_time;				//	DST�ݒ�i�C�O�ł̂ݎg�p�j
		GAME_TIME diff_time;			//	RTC�Ƃ̎��ԍ�
		GAME_TIME save_time;			//	�Z�[�u���̃Q�[��������

		BATTLE_TOWER_WORK BattleTowerWork;
}SYS_SAVE_DATA;


//=========================================================================
//
//		�Q�[���i�s�f�[�^�{��
//
//=========================================================================

typedef struct {

		s16 MapXpos;					//���z�}�b�v���̂w�ʒu
		s16 MapYpos;					//���z�}�b�v���̂x�ʒu
		MapID MapID;					//�}�b�vID
		MapID ContinueMapID;			//�O�̃}�b�v�h�c
		MapID SpMapID;					//����ڑ���̃}�b�v�h�c
		MapID TelMapID;					//�e���|�[�g�Ȃǂ̖߂��h�c
		MapID EscMapID;					//���Ȃ��ق�Ȃǂ̖߂��h�c
		u16 MapMusicNo;					//���݂̉��y
		u8 WeatherValue;				//���݂̓V��
		u8 WeatherCycle;				//���݂̓V�����
		u8 Brightness;					//���݂̖��邳
		u16 MapScreenNo;				//���݂̃}�b�v�X�N���[��


		u16 VirtualMapSv[256];		//���z�}�b�v�Z�[�u
		u8 PokeCountTemoti;									//�莝���|�P�����̐�
		PokemonParam PokeParaTemoti[TEMOTI_POKEMAX];		//�莝���|�P�����f�[�^

		u32	my_gold;					//�莝���̂���
		u16	my_coin;					//�莝���̃R�C��

		u16	cnv_btn;	//�֗����݂ɓo�^���ꂽ���єԍ�

		MINEITEM MyPCBoxItem[ MY_PCBOX_ITEM_MAX ];		//�p�\�R���ɗa��������
		MINEITEM MyNormalItem[ MY_NORMAL_ITEM_MAX ];	//�莝���̕��ʂ̓���
		MINEITEM MyExtraItem[ MY_EXTRA_ITEM_MAX];		//�莝���̑�؂ȓ���
		MINEITEM MyBallItem[ MY_BALL_ITEM_MAX];			//�莝�����ݽ���ް�
		MINEITEM MySkillItem[ MY_SKILL_ITEM_MAX];		//�莝���̋Z�}�V��
		MINEITEM MySeedItem[ MY_SEED_ITEM_MAX];			//�莝���̂���

		CUBE MyCube[ MY_CUBE_MAX ];						//�莝���̃L���[�u

		u8 zukan_see_flag2[ POKE_ZUKAN_LEN ];			//�|�P�����݂����t���O���̂Q

		u16 NormalBPMRecord[ BPM_TYPE_MAX ];			//�u�����h�}�V�[���ō����x
		u16 SioBPMRecord[ BPM_TYPE_MAX ];

		u16 RevengeStepCount;							//�g���[�i�[�Đ�
		u8 RevengeFlag[ REVENGER_MAX ];

		EventOBJ EvObj[EV_OBJ_MAX];						//�t�B�[���hOBJ���
		evObjData SvSxyObj[SXY_OBJ_MAX];				//���̃}�b�v��SXY���

		u8 EventFlag[ EVFLAG_AREA_MAX ];				//�C�x���g�t���O
		u16 EventWork[ EVWORK_AREA_MAX ];				//�C�x���g���[�N
		u32	Record[ SV_RECORD_MAX ];					//�����L���O�p�J�E���^

		SEEDBED	SeedBed[ SEEDBED_MAX ];					//�t�B�[���h�̃^�l�̃f�[�^
		SECRET_BASE_DATA SecretBase[ SECRET_BASE_MAX ];	//�閧��n�̃f�[�^

		u8 MyRoomFurnitureID[ MYROOM_FURNITURE_MAX ];	//����̖͗l�ւ����:���
		u8 MyRoomFurniturePos[ MYROOM_FURNITURE_MAX ];	//����̖͗l�ւ����:�ʒu

		u8 FurnitureDesk[ GOODS_DESK_MAX ];				// �͗l�ւ��O�b�Y�F����
		u8 FurnitureChair[ GOODS_CHAIR_MAX ];			// �͗l�ւ��O�b�Y�F����
		u8 FurniturePlant[ GOODS_PLANT_MAX ];			// �͗l�ւ��O�b�Y�F���A��
		u8 FurnitureEtc[ GOODS_ETC_MAX ];				// �͗l�ւ��O�b�Y�F���炭��
		u8 FurnitureMat[ GOODS_MAT_MAX ];				// �͗l�ւ��O�b�Y�F�}�b�g
		u8 FurniturePoster[ GOODS_POSTER_MAX ];			// �͗l�ւ��O�b�Y�F�|�X�^�[
		u8 FurnitureDoll[ GOODS_DOLL_MAX ];				// �͗l�ւ��O�b�Y�F�ʂ������
		u8 FurnitureCushion[ GOODS_CUSHION_MAX ];		// �͗l�ւ��O�b�Y�F�N�b�V����

		TV_EVENT	tv_event[ TV_WORK_MAX ];			//�s�u�C�x���g�̃f�[�^
		TV_CM		tv_cm[ TVCM_WORK_MAX ];				//�s�u�C�x���g(�b�l�j�̃f�[�^
		Generating_Event_Work tv_encount;				//��ʔ����p�̃f�[�^
		TV_CAMERA	tv_camera;							//�s�u�J�����̃f�[�^

		u16 kaiwa_self[6];			//	�ȈՉ�b�f�[�^�F���ȏЉ�
		u16 kaiwa_battle[6];		//	�ȈՉ�b�f�[�^�F�ΐ�J�n
		u16 kaiwa_win[6];			//	�ȈՉ�b�f�[�^�F�ΐ폟��
		u16 kaiwa_lose[6];			//	�ȈՉ�b�f�[�^�F�ΐ핉��

		MAIL_DATA	MailData[MAIL_STOCK_MAX];			//���[��

		u8 kaiwa_word_flag[8];      //  �ȈՉ�b�p�B�����[�h�t���O
		u8 oyaji_work[64];			//	7�l�̃I���W�p���[�N�G���A
		PopularWord PopWord[POPWORD_MAX];
		PORTRAIT_DATA PortraitData[PORTRAIT_MAX];		//����G�f�[�^�p���[�N

		SODATEYA_WORK SodateyaWork;						//��ĉ��p���[�N

		SCORE_TABLE TaisenScoreTable;					//�ʐM�ΐ팋�ʗp���[�N

		u8 ExRibbonNo[EXTRA_RIBBON_MAX];

		u8 ExtraEventWork[ EXTRA_EVENT_WORK_MAX ];
		u8 ExtraEventFlag[ EXTRA_EVENT_FLAG_MAX ];

		MOVING_POKEMON MovingPokemon;

		RAM_SEED RamSeed;

		EXTRA_SCRIPT_WORK	ExtraScript;				//�O���ǉ��X�N���v�g
		EXTRA_ITEM_WORK		ExtraItem;					//�z�z�p�f�[�^

		u8 zukan_see_flag3[ POKE_ZUKAN_LEN ];			//�|�P�����݂����t���O���̂R

}FIELD_SAVE_DATA;



#endif	/*__GAMEDATA_H__*/

