/***********************************************************************
**
**	�V��G�t�F�N�g�^�X�N����i�͂�j
**
***********************************************************************/
#include "wea_com.h"
#include "script.h"
#include "evobjmv.h"

/*==============================================
==	�O���[�o���ϐ�
==============================================*/
static const actHeader CloudObj;

/*==============================================
==	�֐��v���g�^�C�v
==============================================*/
static void set_cloud_actor(void);
static void del_cloud_actor(void);

/***********************************************************************
**	�_����(WEATHER_SHINE) 
***********************************************************************/
//-------------------------------------
// ������
//-------------------------------------
void InitShine(void)
{
	wp->next_evy = 0;
	wp->evy_wait = CLOUD_CHANGE_WAIT;

	wp->direct_end = 0;
	wp->prog = 0;

	if(wp->cloud_obj_flg == 0){
		WeatherBlendInit(0x00, 0x10);
	}
}
//-------------------------------------
// �_�C���N�g������
//-------------------------------------
void DirectShine(void)
{
	InitShine();
	while(wp->direct_end == 0){
		ShineProc();
	}
}
//-------------------------------------
// �i�s
//-------------------------------------
void ShineProc(void)
{
	switch(wp->prog){
	case 0:
		set_cloud_actor();
		wp->prog++;
		break;

	case 1:
		WeatherBlendReq(0x0c, 0x08, 1);
		wp->prog++;
		break;

	case 2:
		if(WeatherBlendProg()){
			wp->direct_end = 1;
			wp->prog++;
		}
		break;

	default:
		return;
	}
}
//-------------------------------------
// �I��
//-------------------------------------
u8 QuitShine(void)
{
	switch(wp->quit_prog){
	case 0:
		WeatherBlendReq(0x00,0x10,1);
		wp->quit_prog++;
		break;

	case 1:
		if(WeatherBlendProg()){
			del_cloud_actor();
			wp->quit_prog++;
		}
		break;

	default:
		return 0;
	}
	return 1;
}

/***********************************************************************
**	�_�Ȃ�(WEATHER_SHINE2) 
***********************************************************************/
//-------------------------------------
// ������
//-------------------------------------
void InitShine2(void)
{
	wp->next_evy = 0;
	wp->evy_wait = CLOUD_CHANGE_WAIT;
}
//-------------------------------------
// �_�C���N�g������
//-------------------------------------
void DirectShine2(void)
{
	InitShine2();
}
//-------------------------------------
// �i�s
//-------------------------------------
void Shine2Proc(void)
{
	// �Ȃɂ����Ȃ�
}
//-------------------------------------
// �I��
//-------------------------------------
u8 QuitShine2(void)
{
	return 0;  // �����I���
}




/*====================================================================*/
/*                                                                    */
/*  �_��������                                                        */
/*                                                                    */
/*====================================================================*/
//------------------------------------------
// �����o�^
//------------------------------------------
static void set_cloud_actor(void)
{
	static const struct {
		s16 x, y;
	}pos_tbl[] = {
		{   0,  66 },
		{   5,  73 },
		{  10,  78 },
	};

	static const CellData cell = { weather_cloud_Character, sizeof(weather_cloud_Character), WEATHER_CELLID_CLOUD };

	u16 act_no, i;

	if(wp->cloud_obj_flg == 1){ return; }

	CellSet(&cell);
	WeatherEXPalReg(weather_cloud_Palette);

	for(i = 0; i < CLOUD_OBJ_MAX; i++){
		act_no = AddActor(&CloudObj, 0, 0, 255);
		if(act_no != ACT_MAX){
			wp->cloud_act[i] = &ActWork[act_no];

			MapCellSizePosDiff(MAP_LEFT_MARGIN + pos_tbl[i].x, MAP_UP_MARGIN + pos_tbl[i].y,
				&(wp->cloud_act[i]->x),  &(wp->cloud_act[i]->y));

			ActWork[act_no].scr = 1;

		}else{
			wp->cloud_act[i] = NULL;
		}
	}

	wp->cloud_obj_flg = 1;
}
//------------------------------------------
// �����폜
//------------------------------------------
static void del_cloud_actor(void)
{
	if(wp->cloud_obj_flg){
		u16 i;
		for(i = 0; i < CLOUD_OBJ_MAX; i++){
			if(wp->cloud_act[i] != NULL){
				DelActor(wp->cloud_act[i]);
			}
		}
		CellDel(WEATHER_CELLID_CLOUD);
		wp->cloud_obj_flg = 0;
	}
}
//------------------------------------------
// ��������֐�
//------------------------------------------
static void cloud_act_move(actWork *act)
{
	act->work[0]++;
	act->work[0] &= 1;
	if(act->work[0]){
		act->x -= 1;
	}
}
/*====================================================*/
/* �_�A�N�^�[�p�f�[�^                                 */
/*====================================================*/
static const ActOamData CloudObjOam =
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
	3,										// u32 Priority:2
	0,										// u32 Pltt:3
	0,										// AffineWork
};
static const actAnm CloudObjAnm[]=
{
	{0,16,0,0},
	{ACT_ANMEND,0,0,0},
};
static const actAnm * const CloudObjAnmTbl[] =
{
	CloudObjAnm,
};
static const actHeader CloudObj =
{
	WEATHER_CELLID_CLOUD,	// cell id
	WEATHER_PALID_EX,		// pal id
	&CloudObjOam,			// oam
	CloudObjAnmTbl,			// anm table
	ACT_NTRANS,				// trans table
	DummyActAffTbl,			// affine table
	cloud_act_move,			// move
};






/*====================================================================*/
/*                                                                    */
/*  ���Ƃ�̪�ď���(WEATHER_SHINE3)                                   */
/*                                                                    */
/*====================================================================*/
extern void WeatherVeryShineInit(void);
extern void WeatherVeryShineProc(void);

//-------------------------------------
// ������
//-------------------------------------
void InitShine3(void)
{
	wp->prog = 0;
	wp->direct_end = 0;
	wp->next_evy = 0;
	wp->evy_wait = 0;
}
//-------------------------------------
// �_�C���N�g������
//-------------------------------------
void DirectShine3(void)
{
	InitShine3();

	while(wp->direct_end == 0){
		Shine3Proc();
	}
}
//-------------------------------------
// �i�s
//-------------------------------------
void Shine3Proc(void)
{
	switch(wp->prog){
	case 0:
		if(wp->fade_mode != FADEMODE_WEATHER){
			wp->prog++;
		}
		break;

	case 1:
		WeatherS3TblMake_Begin();
		wp->prog++;
		break;

	case 2:
		if(!WeatherS3TblMake()){
			wp->prog++;
		}
		break;

	case 3:
		WeatherVeryShineInit();
		wp->prog++;
		break;

	case 4:
		WeatherVeryShineProc();
		if(wp->vs_level == VERYSHINE_LV_MAX){
			wp->direct_end = 1;
			wp->prog++;
		}
		break;

	default:
		WeatherVeryShineProc();
		break;
	}
}
//-------------------------------------
// �I��
//-------------------------------------
u8 QuitShine3(void)
{
	return 0;
}

/*============================================================*/
/*                                                            */
/*  ���߂āu�͂�R�v�ɂȂ鎞�́A�������̪�āi�P�xڼ޽�����j  */
/*                                                            */
/*============================================================*/
#define VS_BLD_PROG_STOP   (0xff)


enum S3WORK {
	SWK_PROG,
	SWK_BLDY,
	SWK_BLD_TIMER,
	SWK_WININ_SAVE,
};

static void TaskS3Flash(u8 id);

//===================================================
// �����Ă���Ă΂�āA�̪�ėp�����o�^����
//===================================================
void HideriFlashEffect(void)
{
	AddTask(TaskS3Flash,80);
}

//===================================================
// �������̪��Ҳ����
//===================================================
static void TaskS3Flash(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[SWK_PROG]){
	case 0:
		my->work[SWK_BLDY] = 0;
		my->work[SWK_BLD_TIMER] = 0;
		my->work[SWK_WININ_SAVE] = *(vu16*)REG_WININ;
		*(vu16*)REG_WININ = (WIN_ALL_ON<<8)|WIN_ALL_ON;
		*(vu16*)REG_BLDCNT = BLD_BG1_1ST|BLD_BG2_1ST|BLD_BG3_1ST|BLD_OBJ_1ST| BLD_UP_MODE;
		*(vu16*)REG_BLDY = 0;
		my->work[SWK_PROG]++;
//		break;

	case 1:
		my->work[SWK_BLDY] += 3;
		if(my->work[SWK_BLDY]>16){
			my->work[SWK_BLDY] = 16;
		}
		*(vu16*)REG_BLDY = my->work[SWK_BLDY];
		if(my->work[SWK_BLDY] >= 16)
		{
			my->work[SWK_PROG]++;
		}
		break;

	case 2:
		if(++(my->work[SWK_BLD_TIMER])>9)
		{
			my->work[SWK_BLD_TIMER] = 0;
			my->work[SWK_BLDY]--;
			if(my->work[SWK_BLDY] <= 0)
			{
				my->work[SWK_BLDY] = 0;
				my->work[SWK_PROG]++;
			}
			*(vu16*)REG_BLDY = my->work[SWK_BLDY];
		}
		break;

	case 3:
		*(vu16*)REG_BLDCNT = 0;
		*(vu16*)REG_BLDY = 0;
		*(vu16*)REG_WININ = my->work[SWK_WININ_SAVE];
		my->work[SWK_PROG]++;
		break;

	case 4:
		ContinueFieldScript();
		DelTask(id);
		break;
	}
}

