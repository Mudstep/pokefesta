/***********************************************************************
**	�V�󐧌�^�X�N  ���L�w�b�_
**
***********************************************************************/
#ifndef _WEA_COM_H_
#define _WEA_COM_H_

/*========================================================*/
/*  �w�b�_�C���N���[�h                                    */
/*========================================================*/
#include "common.h"
#include "task.h"
#include "palanm.h"
#include "map.h"
#include "menu.h"
#include "calctool.h"
#include "fld_main.h"
#include "weather.h"
#include "weather_dat.h"
#include "MaDefine.h"
#include "mus_tool.h"
#include "menutool.h"    // �f�o�b�O�p

/*========================================================*/
/*  �萔                                                  */
/*========================================================*/
/* �p���b�g�h�c */
enum WEATHER_PAL_ID {
	WEATHER_PALID_COMMON = FLD_PalID_WEATHER_START,
	WEATHER_PALID_EX,
};
/* �Z���h�c */
enum WEATHER_CELL_ID {
	WEATHER_CELLID_CLOUD = FLD_CellID_WEATHER_START,
	WEATHER_CELLID_MIST,
	WEATHER_CELLID_ASH,
	WEATHER_CELLID_MIST2,
	WEATHER_CELLID_STORM,
	WEATHER_CELLID_AWA,
	WEATHER_CELLID_RAIN,
};
/* ������ײ��è */
enum
{
	WEATHER_ACT_PRI0 = EVOBJ_HI_ACTPRI - 4,
	WEATHER_ACT_PRI1,
	WEATHER_ACT_PRI2,
	WEATHER_ACT_PRI3,
};
/* �e��n�a�i�ő吔 */
#define CLOUD_OBJ_MAX      (3)
#define RAIN_OBJ_MAX       (24)
#define SNOW_OBJ_MAX       (16)
#define ASH_OBJ_MAX        (20)
#define MIST_OBJ_MAX       (20)
#define MIST2_OBJ_MAX      (20)
#define STORM_OBJ_MAX      (20)
#define STORM2_OBJ_MAX     (5)

/* �܂�p�p���b�g�t�F�[�h */
#define CLOUD_DIPTH_MAX      (3)
#define CLOUD_CHANGE_WAIT    (20)
#define RAIN_ADD_WAIT   (8)
#define RAIN_DEL_WAIT   (12)
#define SNOW_ADD_WAIT   (36)
#define SNOW_DEL_WAIT   (8)

/* �Ǝ��p���b�g�t�F�[�h�� start, end �l */
#define  FADE_EX_MAX  (33)
#define  FADE_MAX     (32)
#define  FADE_MIN     (0)
#define  FADE_BASE    (16)

#define  FADE_CLOUD_MAX    (3)
#define  FADE_THUNDER_MAX  (19)
#define  FADE_LV_MAX       (19)

enum FADE_MODE{
	FADEMODE_WEATHER,  // �ܓV̪���
	FADEMODE_IN,       // ̨���޲ݗp
	FADEMODE_OUT,      // ̨���ޱ�ėp
	FADEMODE_NULL,     // �������Ȃ�
};


/* �J�̑��x */
enum RAIN_SPEED {
	RAIN_SPEED_NORMAL,
	RAIN_SPEED_FAST,
};

#define RAIN_DISP_RAIN      (10)  // �ő�\���n�a�i���i���߁j
#define RAIN_DISP_THUNDER   (16)  // �ő�\���n�a�i���i���݂Ȃ�j
#define RAIN_DISP_HEAVYRAIN (RAIN_OBJ_MAX)  // �ő�\���n�a�i���i�������߁j


#define IS_RAINY_WEATHER(w)  (((w)==WEATHER_RAIN)||((w)==WEATHER_THUNDER)||((w)==WEATHER_HEAVYRAIN))
#define VERYSHINE_LV_MAX  (6)

/* PaletteFadeReq ���̃r�b�g */
#define FIELD_FADE_BIT_MASK  0x3fff

enum WEATHER_PAL_STATE {
	WEATHER_PAL_NORMAL,
	WEATHER_PAL_CLOUD,
	WEATHER_PAL_THUNDER,
	WEATHER_PAL_SHINE,
	WEATHER_PAL_BLACKOUT,
	WEATHER_PAL_WHITEOUT,
};

#define  MISTFADEIN_PALMAX  (6)   // ��̪��޲ݕ␳�Ώ���گĖ{���i�ő�j


#define PLAY_RAIN_SOUND  (1)

/*========================================================*/
/*  ���[�N�̈�                                            */
/*========================================================*/
typedef struct {
	// �A�N�^�[�|�C���^
	actWork *rain_actor[RAIN_OBJ_MAX];
	actWork *snow_act[SNOW_OBJ_MAX];
	actWork *mist_actor[MIST_OBJ_MAX];
	actWork *ash_actor[ASH_OBJ_MAX];
	actWork *mist2_actor[MIST2_OBJ_MAX];
	actWork *storm_actor[STORM_OBJ_MAX];
	actWork *storm2_actor[STORM2_OBJ_MAX];
	actWork *cloud_act[CLOUD_OBJ_MAX];

	// �t�F�[�h�p�e�[�u��
	u8 bg_fade_tbl[FADE_LV_MAX][32];
	u8 obj_fade_tbl[FADE_LV_MAX][32];
	s8 now_evy;
	s8 next_evy;
	u8 evy_wait;
	u8 evy_timer;

	u16 fadein_col;
	u8  fade_mode;
	u8  fadein_step;
	u8  fadereq_flg;
	u8  task_id;
	u8  fade_fill_flg;
	u8  fade_fill_cnt;

	u16  prog;      // �V��i���x
	u16  quit_prog;
	u8   status;      // ���݂̓V��
	u8   next_status; // ���̓V��
	u8   direct_end;
	u8   smooth_end;  // ���X�ɕύX�����H

//	u8 off_fadereq_flg;

	u8  expal_num;
	u8  compal_num;

	// �J
	u16 rain_disp_timer;  // �J�I�u�W�F�����^�C�}�[
	u8  rain_disp_cnt;    // �\���J�I�u�W�F���̌��ݒl
	u8  rain_disp_max;    // �\���J�I�u�W�F���̐ݒ�l
	u8  rain_obj_cnt;     // �A�N�^�[�o�^����Ă���J�I�u�W�F��
	u8  rain_disp_wait;   // �J�I�u�W�F�����Ԋu
	u8  rain_speed;
	u8  rain_sound_type;  // 
//	u8  rain_dispmax_reserve;

	// �_�i���ʂɉf�鱸���j
	u8 cloud_obj_flg;   // �����o�^�ς݂��H

	// ��
	u16 snow_timer;       // ��I�u�W�F�����^�C�}�[
	u16 snow_comeback_timer;
	u8  snow_obj_cnt;     // ��I�u�W�F���̌��ݒl
	u8  snow_obj_max;     // ��I�u�W�F���̐ݒ�l
//	u8  snow_change_wait; // ��I�u�W�F�����Ԋu

	// �J�~�i��
	u16 thunder_timer;
	u16 thunder_se_timer;
	u8  thunder_stop;
	u8  thunder_type;
	u8  thunder_cnt1;
	u8  thunder_se_flg;

	// ��
	u16 mist_xline;
	u16 mist_timer, mist_scr;
	u8  mist_fadein_palno[MISTFADEIN_PALMAX];
	u8  mist_fadein_palcnt;
	u8  mist_actor_flg;

	// �ΎR�D
	u16 ash_xline;
	u16 ash_obj_cnt;
	u8  ash_obj_flg;

	// ����
	u32 storm_xofs, storm_yofs;
	u16 storm_obj_max;
	u16 storm_xline, storm_yline, storm_cnt, storm_cnt_sub;
	u8  storm_obj_flg;
	u8  storm2_obj_flg;

	// ���Q
	u16 mist2_xline, mist2_yline;
	u16 mist2_xcnt, mist2_ycnt, mist2_xofs, mist2_yofs;
	u8 mist2_obj_flg;

	// �����Ă��i�A�A�N�^�[�j
	u16  awa_add_cnt;
	u16  awa_interval_cnt;
	u16  awa_pos_cnt;
	u16  awa_act_cnt;
	u8   awa_actor_flg;

	// ���������W�X�^����
	u16 eva, evb, end_eva, end_evb;
	u8  blend_cnt;
	u8  blend_timer;
	u8  blend_wait;

	// �͂�R�i���Ƃ�j
	s16  vs_level;
	s16  vs_before_level;
	s16  vs_cnt;
	s16  vs_vec;
	u16  vs_bld_prog;
	u16  vs_bldy;
	u16  vs_bld_timer;
	u16  vs_winin_save;
	u8   s3_tbl_make_end;
	s8   s3_r;
	s8   s3_g;

}WEATHER_WORK;

extern WEATHER_WORK* const wp;

#endif /* #ifndef _WEA_COM_H_ */

/*========================================================*/
/*  �֐��v���g�^�C�v                                      */
/*========================================================*/
// ����
void WeatherFadeNow(s8 evy);
void WeatherFadeReq(s8 begin, s8 end, u8 wait);

void WeatherBlendInit(u8 eva, u8 evb);
void WeatherBlendReq(u8 eva, u8 evb, u8 wait);
u8 WeatherBlendProg(void);

void WeatherS3TblMake_Begin(void);
u8 WeatherS3TblMake(void);

void WeatherEXPalReg(const u16 *pal_data);
void PlayRainSound(u16 se);

// �͂�
void InitShine(void);
void ShineProc(void);
void DirectShine(void);
u8 QuitShine(void);

void InitShine2(void);
void Shine2Proc(void);
void DirectShine2(void);
u8 QuitShine2(void);

void FadeOutCloudObj(void);
void DelCloudObj(void);

// ����
void InitRain(void);
void DirectRain(void);
void RainProc(void);
u8   QuitRain(void);
u8 RainActorAdd(void);
void RainCellSet(void);
u8 AdjustRainActor(void);
void DelRainObj(void);

// ���݂Ȃ�
void InitThunder(void);
void ThunderProc(void);
void DirectThunder(void);
u8 QuitThunder(void);

void InitHeavyRain(void);
void DirectHeavyRain(void);

// �䂫
void InitSnow(void);
void SnowProc(void);
void DirectSnow(void);
u8 QuitSnow(void);

// ������΂�
void InitAsh(void);
void DirectAsh(void);
void AshProc(void);
u8 QuitAsh(void);


// ����
void InitMist(void);
void DirectMist(void);
void MistProc(void);
u8 QuitMist(void);

void FadeOutMistObj(void);
void DelMistObj(void);

// ����Q
void InitMist2(void);
void DirectMist2(void);
void Mist2Proc(void);
u8 QuitMist2(void);


// ���Ȃ��炵
void InitStorm(void);
void DirectStorm(void);
void StormProc(void);
u8 QuitStorm(void);

// ������
void InitCloud(void);
void DirectCloud(void);
void CloudProc(void);
u8 QuitCloud(void);

void InitShine3(void);
void Shine3Proc(void);
void DirectShine3(void);
u8 QuitShine3(void);

// �����Ă�
void InitMarine(void);
void MarineProc(void);
void DirectMarine(void);
u8 QuitMarine(void);


