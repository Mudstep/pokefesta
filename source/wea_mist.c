/***********************************************************************
**
**	�V��G�t�F�N�g�^�X�N����i����j
**
***********************************************************************/
#include "wea_com.h"

/*==============================================
==	�֐��v���g�^�C�v
==============================================*/
static void add_mist_obj(void);
static void del_mist_obj(void);

//==============================================
//	������
//==============================================
void InitMist(void)
{
	wp->prog = 0;
	wp->direct_end = 0;

	wp->next_evy = 0;
	wp->evy_wait = CLOUD_CHANGE_WAIT;


	if(wp->mist_actor_flg == 0){
		wp->mist_timer = 0;
		wp->mist_scr = 0;
		wp->mist_xline = 0;
		WeatherBlendInit(0x00,0x10);
	}
}
//==============================================
//	�_�C���N�g������
//==============================================
void DirectMist(void)
{
	InitMist();
	while(wp->direct_end==0){
		MistProc();
	}
}
//==============================================
//	�i�s
//==============================================
void MistProc(void)
{
	wp->mist_xline = (ActOX - wp->mist_scr) & 255;
	if(++(wp->mist_timer) > 3){
		wp->mist_timer = 0;
		wp->mist_scr++;
	}

	switch(wp->prog){
	case 0:
		add_mist_obj();
		// ����
		if(wp->status == WEATHER_MIST){
			WeatherBlendReq(0x0c, 0x08, 3);
		// �C��
		}else{
			WeatherBlendReq(0x04, 0x10, 0);
		}
		wp->prog++;
		break;

	case 1:
		if(WeatherBlendProg()){
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
u8 QuitMist(void)
{
	wp->mist_xline = (ActOX - wp->mist_scr) & 255;
	if(++(wp->mist_timer) > 3){
		wp->mist_timer = 0;
		wp->mist_scr++;
	}

	switch(wp->quit_prog){
	case 0:
		WeatherBlendReq(0x00, 0x10, 3);
		wp->quit_prog++;
		break;

	case 1:
		if(WeatherBlendProg()){
			wp->quit_prog++;
		}
		break;

	case 2:
		del_mist_obj();
		wp->quit_prog++;
		break;

	default:
		return 0;
	}
	return 1;
}
/*======================================================================
==	���A�N�^�[�֘A�f�[�^
======================================================================*/
static void MistActMove(actWork *act);

/* --- �n�`�l�f�[�^ --- */
static const ActOamData MistObjOam =
{
	0,										// u32 VPos:8
	0,										// u32 AffineMode:2
	1,										// u32 ObjMode:2
	0,										// u32 Mosaic:1
	0,										// u32 ColorMode:1
	0,										// u32 Shape:2

	0,										// u32 HPos:9
	0,										// u32 AffineParamNo:5
	3,										// u32 Size:2

	0,										// u32 CharNo:10
	2,										// u32 Priority:2
	0,										// u32 Pltt:3
	0,										// AffineWork
};


/* --- �A�j���e�[�u�� --- */
static const actAnm MistObjAnm[]=
{
	{0,16,0,0},
	{ACT_ANMEND,0,0,0},

	{32,16,0,0},
	{ACT_ANMEND,0,0,0},

	{64,16,0,0},
	{ACT_ANMEND,0,0,0},

	{96,16,0,0},
	{ACT_ANMEND,0,0,0},

	{128,16,0,0},
	{ACT_ANMEND,0,0,0},

	{160,16,0,0},
	{ACT_ANMEND,0,0,0},
};
/* --- �A�j���p�^�[�� --- */
static const actAnm * const MistObjAnmTbl[] =
{
	MistObjAnm,
	MistObjAnm+2,
	MistObjAnm+4,
	MistObjAnm+6,
	MistObjAnm+8,
	MistObjAnm+10,
};
/* --- �g�k�p�^�[�� --- */
static const actAffAnm MistAffAnm[] = {
	{ 0x0200, 0x0200, 0, ACT_AFSET },
	{ ACT_AFEND, 0, 0, 0   },
};
static const actAffAnm * const MistAffAnmTbl[] = {
	MistAffAnm,
};
/* --- �A�N�g�w�b�_ --- */
static const actHeader MistObj =
{
	WEATHER_CELLID_MIST,		// cell id
	WEATHER_PALID_COMMON,// pal id
	&MistObjOam,		// oam
	MistObjAnmTbl,		// anm table
	ACT_NTRANS,			// trans table
	MistAffAnmTbl,		// affine table
	MistActMove,		// move
};
/* --- ���[�N��` --- */
enum MIST_WORK {
	MISTWK_XID,
};
/***********************************************************************
**	���A�N�^�[����֐�
***********************************************************************/
static void MistActMove(actWork *act)
{
	act->dy = ActOY & 0xff;

	act->x = (wp->mist_xline+32) + (act->work[MISTWK_XID] * 64);
	if(act->x > 271){
		act->x = (wp->mist_xline+480)-((4-act->work[MISTWK_XID])*64);
		act->x &= 511;
	}
}
/***********************************************************************
**	���A�N�^�[�o�^�E�t�F�[�h�C��
***********************************************************************/
static void add_mist_obj(void)
{
	if(wp->mist_actor_flg == 0){
		CellData cell = { weather_mist_Character, sizeof(weather_mist_Character), WEATHER_CELLID_MIST };
		actWork *act;
		u16 act_no;
		u16 i;

		CellSet(&cell);
		for(i = 0; i < MIST_OBJ_MAX; i++){
			act_no = AddActorBottom(&MistObj, 0, 0, 255);
			if(act_no != ACT_MAX)
			{
				act = &ActWork[act_no];
				act->work[MISTWK_XID] = i % 5;
				act->x = 32 + (i % 5) * 64;
				act->y = 32 + (i / 5) * 64;
				wp->mist_actor[i] = act;
			}
			else
			{
				wp->mist_actor[i] = NULL;
			}
		}
		wp->mist_actor_flg = 1;
	}
}
/***********************************************************************
**	���A�N�^�[����
***********************************************************************/
static void del_mist_obj(void)
{
	if(wp->mist_actor_flg){
		u16 i;
		for(i = 0; i < MIST_OBJ_MAX; i++){
			if(wp->mist_actor[i] != NULL){
				DelActor(wp->mist_actor[i]);
			}
		}
		CellDel(WEATHER_CELLID_MIST);
		wp->mist_actor_flg = 0;
	}
}
