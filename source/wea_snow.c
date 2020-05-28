/***********************************************************************
**
**	�V��G�t�F�N�g�^�X�N����i�䂫�j
**
***********************************************************************/
#include "wea_com.h"


/*==============================================
==	�萔
==============================================*/
/* ��A�N�^�[�̃��[�N��` */
enum SNOWOBJ_WORK {
	SNOWWK_YPOS,  // 0: �x���W�i�Œ菬���_���j
	SNOWWK_YACC,  // 1: �x�����ړ��ʁi�Œ菬���_���j
	SNOWWK_XACC,  // 2: �w�����ړ��ʁi�����j
	SNOWWK_XTIMER,// 3: �w�����ړ��p�^�C�}�[
	SNOWWK_ID,    // 4: �h�c
	SNOWWK_AGE,   // 5: �N��iVsync�j
	SNOWWK_LIFE,  // 6: ����
	SNOWWK_YACC_SAVE,
};

/*==============================================
==	�O���[�o���ϐ�
==============================================*/
static const actHeader SnowObj;

/*==============================================
==	�֐��v���g�^�C�v
==============================================*/
static u8 adjust_snow_obj(void);
static u8 add_single_snow_obj(void);
static u8 del_single_snow_obj(void);
static void reset_snow_act_param(actWork *act);
static void snow_act_move(actWork * act);

//==============================================
//	������
//==============================================
void InitSnow(void)
{
	wp->prog = 0;
	wp->direct_end = 0;

	wp->next_evy = FADE_CLOUD_MAX;
	wp->evy_wait = CLOUD_CHANGE_WAIT;
	wp->snow_obj_max = SNOW_OBJ_MAX;
	wp->snow_timer = 0;
}
//==============================================
//	�_�C���N�g������
//==============================================
void DirectSnow(void)
{
	u16 i;

	InitSnow();
	while(wp->direct_end == 0){
		SnowProc();
		for(i = 0; i < wp->snow_obj_cnt; i++)
		{
			snow_act_move(wp->snow_act[i]);
		}
	}
}
//==============================================
//	�i�s
//==============================================
void SnowProc(void)
{
	switch(wp->prog){
	case 0:
		if(!adjust_snow_obj()){
			wp->direct_end = 1;
			wp->prog++;
		}
		break;

	default:
		return;
	}
}
//==============================================
//	�I��
//==============================================
u8 QuitSnow(void)
{
	switch(wp->quit_prog){
	case 0:
		wp->snow_obj_max = 0;
		wp->snow_timer = 0;
		wp->quit_prog++;

	case 1:
		if(!adjust_snow_obj()){
			wp->quit_prog++;
			return 0;
		}
		break;

	default:
		return 0;
	}
	return 1;
}

/***********************************************************************
**	��I�u�W�F���̒���  0:�I��  1:�p����
***********************************************************************/
static u8 adjust_snow_obj(void)
{
	if(wp->snow_obj_cnt == wp->snow_obj_max){ return 0; }

	if(++(wp->snow_timer) > SNOW_ADD_WAIT){
		wp->snow_timer = 0;
		if(wp->snow_obj_cnt < wp->snow_obj_max){
			add_single_snow_obj();
		}else{
			del_single_snow_obj();
		}
	}

	return wp->snow_obj_cnt != wp->snow_obj_max;
}
/***********************************************************************
**	��I�u�W�F�P�ǉ�
**
**	����
**	�߂�l	����...TRUE / ���s...FALSE
**
***********************************************************************/
static u8 add_single_snow_obj(void)
{
	u16  act_no;

	act_no = AddActorBottom(&SnowObj, 0, 0 ,WEATHER_ACT_PRI0 );
	if(act_no == ACT_MAX){
		return FALSE;
	}

	ActWork[act_no].work[SNOWWK_ID] = wp->snow_obj_cnt;
	reset_snow_act_param(&ActWork[act_no]);
	ActWork[act_no].scr = 1;

	wp->snow_act[wp->snow_obj_cnt++] = &ActWork[act_no];
	return TRUE;
}
/***********************************************************************
**	��I�u�W�F�P����
**
**	����	-
**	�߂�l	����...TRUE / ���s...FALSE
**
***********************************************************************/
static u8 del_single_snow_obj(void)
{
	if(wp->snow_obj_cnt){
		DelActor(wp->snow_act[(--wp->snow_obj_cnt)]);
		return TRUE;
	}
	return FALSE;
}
/***********************************************************************
**	��A�N�^�[�̃p�����[�^���ăZ�b�g����
**
**	����    act  �A�N�^�[�\���̂ւ̃|�C���^
**
**	�߂�l	-
**
***********************************************************************/
static void reset_snow_act_param(actWork *act)
{
	u16 val;
	u16 x;

//	x = (act->work[SNOWWK_ID] & 3) * 60 + (pp_rand() % 60);
	x = ((act->work[SNOWWK_ID] * 5) & 7) * 30 + (pp_rand() % 30);
	act->y = -3 - (ActOY + act->cy);
	act->x = x - (ActOX + act->cx);
	act->work[SNOWWK_YPOS] = act->y * 128;
	act->dx = 0;

	val = pp_rand();

	act->work[SNOWWK_YACC] = 64 + (5 * (val & 3));
	act->work[SNOWWK_YACC_SAVE] = act->work[SNOWWK_YACC];
	ActAnmChg(act, ((val & 1) == 0));
	act->work[SNOWWK_XTIMER] = 0;
	act->work[SNOWWK_XACC] = (((val & 3)==0)? 2 : 1);
	act->work[SNOWWK_LIFE] = 210 + (val & 31);
	act->work[SNOWWK_AGE] = 0;
}
/***********************************************************************
**	��A�N�^�[����֐�
***********************************************************************/
// �E�F�C�g
static void SnowActMoveWait(actWork *act)
{
	if(wp->snow_comeback_timer > 18){
		act->banish = 0;
		act->move = snow_act_move;
		act->y = 250 - (ActOY + act->cy);
		act->work[SNOWWK_YPOS] = act->y * 128;
		wp->snow_comeback_timer = 0;
	}
}
// ����
static void snow_act_move(actWork * act)
{
	s16 x;
	u16 y;

	/* �~�� */
	act->work[SNOWWK_YPOS] += act->work[SNOWWK_YACC];
	act->y = (act->work[SNOWWK_YPOS] >> 7);

	/* �h�� */
	act->work[SNOWWK_XTIMER] += act->work[SNOWWK_XACC];
	act->work[SNOWWK_XTIMER] &= 0xff;
	act->dx = sin_cos_table[act->work[SNOWWK_XTIMER]] / 64;

	/* ���X�N���[���ŏ������Ƃ� */
	x = (act->x + act->cx + ActOX) & 0x1ff;
	if(x & 0x100){ x |= 0xff00; }
	if(x < -3){
		act->x = 242 - (ActOX + act->cx);
	}else if(x > 242){
		act->x = -3 - (ActOX + act->cx);
	}

	/* �c�X�N���[���ŏ������Ƃ� */
	y = (act->y + act->cy + ActOY) & 0xff;
	if(y > 163 && y < 171){

		act->y = 250 - (ActOY + act->cy);
		act->work[SNOWWK_YPOS] = act->y * 128;
		act->work[SNOWWK_AGE] = 0;
		act->work[SNOWWK_LIFE] = 220;

//		act->banish = 1;
//		act->move = SnowActMoveWait;

	}else if(y > 242 && y < 250){

		act->y = 163;// - (ActOY + act->cy);
		act->work[SNOWWK_YPOS] = act->y * 128;
		act->work[SNOWWK_AGE] = 0;
		act->work[SNOWWK_LIFE] = 220;
		act->banish = 1;
		act->move = SnowActMoveWait;
	}

#if 1
	/* �����ŏ������Ƃ� */
	act->work[SNOWWK_AGE]++;
	if(act->work[SNOWWK_AGE]==act->work[SNOWWK_LIFE]){
		reset_snow_act_param(act);
		act->y = 250;//+= (ActOY + act->cy);
		act->banish = 1;
		act->move = SnowActMoveWait;
	}
#endif
}
/*======================================================================
==	��
======================================================================*/
/* --- �A�N�^�[�f�[�^ --- */
static const ActOamData SnowObjOam8x8 =
{
	0,										// u32 VPos:8
	0,										// u32 AffineMode:2
	0,										// u32 ObjMode:2
	0,										// u32 Mosaic:1
	0,										// u32 ColorMode:1
	0,										// u32 Shape:2
	0,										// u32 HPos:9
	0,										// u32 AffineParamNo:5
	0,										// u32 Size:2
	0,										// u32 CharNo:10
	1,										// u32 Priority:2
	0,										// u32 Pltt:3
	0,										// AffineWork
};
static const actAnmData snow_font_celltrans[] =
{
	{ (const u8 *)&weather_snow_Character,       0x20*1},
	{ (const u8 *)&weather_snow_Character[0x20], 0x20*1},
};
static const actAnm SnowObjAnm[]=				// ��
{
	{0,16,0,0},
	{ACT_ANMEND,0,0,0},
	{1,16,0,0},
	{ACT_ANMEND,0,0,0},
};
static const actAnm * const SnowObjAnmTbl[] =
{
	SnowObjAnm,
	SnowObjAnm + 2,
};
/* --- �A�N�g�w�b�_ --- */
static const actHeader SnowObj =
{
	ACT_TMODE,				// cell id
	WEATHER_PALID_COMMON,	// pal id
	&SnowObjOam8x8,			// oam
	SnowObjAnmTbl,			// anm table
	snow_font_celltrans,	// trans table
	DummyActAffTbl,			// affine table
	snow_act_move,			// move
};
















#if 0
/***********************************************************************
**	��I�u�W�F�S����
**
**	�����E�߂�l	-
**
***********************************************************************/
void DelAllSnowObj(void)
{
	if(wp->snow_obj_cnt){
		u8 i;
		for(i=0; i < wp->snow_obj_cnt; i++)
		{
			DelActor(wp->snow_act[i]);
		}
		wp->snow_obj_cnt = 0;
	}
}
#endif
