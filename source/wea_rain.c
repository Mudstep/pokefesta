/***********************************************************************
**
**	�V��G�t�F�N�g�^�X�N����i���߁j
**
***********************************************************************/
#include "wea_com.h"

/*==============================================
==	�萔
==============================================*/

/*==============================================
==	�O���[�o���ϐ�
==============================================*/
static const actHeader RainObj;
/*==============================================
==	�O���[�o���ϐ�
==============================================*/
static const struct RainPos{
	u16 px;
	u16 py;
}RainObjPos[] = {
	{   0,   0 },
	{   0, 160 },
	{   0,  64 },
	{ 144, 224 },
	{ 144, 128 },
	{  32,  32 },
	{  32, 192 },
	{  32,  96 },

	{  72, 128 },
	{  72,  32 },
	{  72, 192 },
	{ 216,  96 },
	{ 216,   0 },
	{ 104, 160 },
	{ 104,  64 },
	{ 104, 224 },

	{ 144,   0 },
	{ 144, 160 },
	{ 144,  64 },
	{  32, 224 },
	{  32, 128 },
	{  72,  32 },
	{  72, 192 },
	{  48,  96 },


};

/*==============================================
==	�v���g�^�C�v
==============================================*/
static void rain_act_move(actWork * act);

//==============================================
//	������
//==============================================
void InitRain(void)
{
	wp->prog = 0;
	wp->direct_end = 0;
	wp->rain_disp_timer = 0;
	wp->rain_disp_wait = RAIN_ADD_WAIT;

	wp->rain_speed = RAIN_SPEED_NORMAL;
	wp->rain_disp_max = RAIN_DISP_RAIN;

	wp->next_evy = FADE_CLOUD_MAX;
	wp->evy_wait = CLOUD_CHANGE_WAIT;

#if PLAY_RAIN_SOUND
	PlayRainSound(SE_T_KOAME);
#endif
}
//==============================================
//	�_�C���N�g������
//==============================================
void DirectRain(void)
{
	InitRain();

	while(wp->direct_end == 0)
	{
		RainProc();
	}
}
//==============================================
//	�i�s
//==============================================
void RainProc(void)
{
	switch(wp->prog){
	case 0:
		RainCellSet();
		wp->prog++;
		break;

	case 1:
		if(!RainActorAdd()){
			wp->prog++;
		}
		break;

	case 2:
		if(!AdjustRainActor()){
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
u8 QuitRain(void)
{
	switch(wp->quit_prog){
	case 0:
		if(IS_RAINY_WEATHER(wp->next_status))
		{
			wp->quit_prog = 0xff;
			return 0;
		}
		wp->rain_disp_max = 0;
		wp->quit_prog++;
	case 1:
		if(!AdjustRainActor()){
			DelRainObj();
			wp->quit_prog++;
			return 0;
		}
		break;

	default:
		return 0;
	}
	return 1;
}
/*----------------------------------------------------------------------*/
/*  �J�A�N�^�[�֘A�f�[�^                                                */
/*----------------------------------------------------------------------*/
static const ActOamData RainObjOam16x32 =
{
	0,										// u32 VPos:8
	0,										// u32 AffineMode:2
	0,										// u32 ObjMode:2
	0,										// u32 Mosaic:1
	0,										// u32 ColorMode:1
	2,										// u32 Shape:2
	0,										// u32 HPos:9
	0,										// u32 AffineParamNo:5
	2,										// u32 Size:2
	0,										// u32 CharNo:10
	1,										// u32 Priority:2
	OTHERS_OBJ_PALNO_0,						// u32 Pltt:3
	0,										// AffineWork
};

static const actAnm RainObjAnm0[]=				// �ʏ�
{
	{OBJ16_16x32*0,16,0,0},
	{ACT_ANMLOOP,0,0,0},
};
static const actAnm RainObjAnm1[]=				// �͂���
{
	{OBJ16_16x32*1,3,0,0},
	{OBJ16_16x32*4,2,0,0},
	{OBJ16_16x32*5,2,0,0},
	{ACT_ANMEND,0,0,0},
};
static const actAnm RainObjAnm2[]=				// �͂���
{
	{OBJ16_16x32*1,3,0,0},
	{OBJ16_16x32*2,3,0,0},
	{OBJ16_16x32*3,4,0,0},
	{ACT_ANMEND,0,0,0},
};

#define RAIN_ANM_WAIT       (3+2+2)  // ���İ�� wait
#define RAIN_ANM_WAIT_FAST  (3+3+4)  // ���İ�� wait

static const actAnm * const RainObjAnmTbl[] =
{
	RainObjAnm0,
	RainObjAnm1,
	RainObjAnm2,
};
static const actHeader RainObj =
{
	WEATHER_CELLID_RAIN,	// cell id
	WEATHER_PALID_COMMON,	// pal id
	&RainObjOam16x32,		// oam
	RainObjAnmTbl,			// anm table
	0,						// trans table
	DummyActAffTbl,			// affine table
	rain_act_move,			// move
};
/***********************************************************************
**	�J�A�N�^�[����֐�
***********************************************************************/
// �ړ����x
#define RAIN_AX       (-0x68)
#define RAIN_AY       (0xD0)

#define RAIN_FAST_AX  (-0xa0)
#define RAIN_FAST_AY  (0x140)

#define RAIN_MOVE_SYNC      (18)
#define RAIN_MOVE_SYNC_FAST (12)

#define RAIN_ROOT_INI        (361)  // �ʒu���o������悤�ɁA���`���N�`���Ȑ���
#define RAIN_NEXT_ROOT_STEP  (145)  // �ʒu���o������悤�ɁA���`���N�`���Ȑ���

static const s16 RainSpdTbl[][2] = {
	{ RAIN_AX, RAIN_AY },
	{ RAIN_FAST_AX, RAIN_FAST_AY },
};

static const u16 RainMoveSync[][2] = {
	{ RAIN_MOVE_SYNC, RAIN_ANM_WAIT },
	{ RAIN_MOVE_SYNC_FAST, RAIN_ANM_WAIT_FAST },
};

// ���[�N��`
enum RAIN_WORK {
	RAINWK_COUNTER,
	RAINWK_ROOTID,
	RAINWK_XPOS,
	RAINWK_YPOS,
	RAINWK_ANMFLG,
	RAINWK_DISPFLG,
	RAINWK_MOVEFUNC,
};

static void rain_set_nextpos(actWork *act)
{
	u32 seed;
	u16  sync;

	if(act->work[RAINWK_ROOTID] == 0)
	{
		act->work[RAINWK_ROOTID] = RAIN_ROOT_INI;
	}
	seed = act->work[RAINWK_ROOTID] * 1103515245L + 12345;
	act->work[RAINWK_ROOTID] = (s16)(((seed / 65536L) % 32768U)%600);

	sync = RainMoveSync[wp->rain_speed][0];

	act->work[RAINWK_XPOS] = (act->work[RAINWK_ROOTID] % 30) * 8;
	act->work[RAINWK_YPOS] = (act->work[RAINWK_ROOTID] / 30) * 8;

	act->work[RAINWK_XPOS] <<= 4;
	act->work[RAINWK_YPOS] <<= 4;

	act->work[RAINWK_XPOS] -= (RainSpdTbl[wp->rain_speed][0] * sync);
	act->work[RAINWK_YPOS] -= (RainSpdTbl[wp->rain_speed][1] * sync);

	ActAnmChg(act, 0);
	act->work[RAINWK_ANMFLG] = 0;
	act->scr = 0;

	act->work[RAINWK_COUNTER] = sync;
}

static void rain_act_move(actWork * act)
{
	if(act->work[RAINWK_ANMFLG] == 0){
		act->work[RAINWK_XPOS] += RainSpdTbl[wp->rain_speed][0];
		act->work[RAINWK_YPOS] += RainSpdTbl[wp->rain_speed][1];
		act->x = act->work[RAINWK_XPOS] >> 4;
		act->y = act->work[RAINWK_YPOS] >> 4;


		if(act->work[RAINWK_DISPFLG])
		{
			if(act->x < -8 || act->x > 248 || act->y < -16 || act->y > 176){
				act->banish = 1;
			}else{
				act->banish = 0;
			}
		}else
		{
			act->banish = 1;
		}

		act->work[RAINWK_COUNTER]--;
		if(act->work[RAINWK_COUNTER] == 0)
		{
			ActAnmChg(act, wp->rain_speed+1);
			act->work[RAINWK_ANMFLG] = 1;
			act->x -= ActOX;
			act->y -= ActOY;
			act->scr = 1;
		}

	}else{
		if(act->anmend_sw)
		{
			act->banish = 1;
			rain_set_nextpos(act);
		}
	}
}
static void rain_actmove_countwait(actWork *act)
{
	if(act->work[RAINWK_COUNTER] == 0){
		rain_set_nextpos(act);
		act->move = rain_act_move;
	}else{
		act->work[RAINWK_COUNTER]--;
	}
}

//==============================================================
//  ���ۂ̓���֐����w��񐔂����ʂ����̂Ɠ�����Ԃɂ���
//==============================================================
static void rain_actmove_roop(actWork * act, u16 cnt)
{
	u16 move_wait, anm_wait;
	u16 move_roop, rem_sync;

	move_wait = RainMoveSync[wp->rain_speed][0];
	anm_wait = RainMoveSync[wp->rain_speed][1];

	move_roop = cnt / (move_wait+anm_wait);
	rem_sync = cnt % (move_wait+anm_wait);

	while(move_roop--)
	{
		rain_set_nextpos(act);
	}

	if(rem_sync < move_wait){
		while(rem_sync--){
			rain_act_move(act);
		}
		act->work[RAINWK_MOVEFUNC] = 0;
	}else{
		act->work[RAINWK_COUNTER] = rem_sync -= move_wait;
		act->banish = 1;
		act->work[RAINWK_MOVEFUNC] = 1;

//		act->move = rain_actmove_countwait;
		
	}

}

//==================================================
//  �J�پ��
//==================================================
void RainCellSet(void)
{
	static const CellData cell = {
		weather_rain_Character, sizeof(weather_rain_Character), WEATHER_CELLID_RAIN
	};

	CellSet(&cell);
}
//==============================================================
//  �J�A�N�^�[�o�^
//  return: 0 ... �ő�܂œo�^�ς�  1 ... ����ȊO
//==============================================================
u8 RainActorAdd(void)
{
	u8 no, c;

	if(wp->rain_obj_cnt == RAIN_OBJ_MAX){
		return 0;
	}

	c = wp->rain_obj_cnt;
	no = AddActorBottom(&RainObj, RainObjPos[c].px, RainObjPos[c].py ,WEATHER_ACT_PRI0);
	if(no != ACT_MAX){
		ActWork[no].work[RAINWK_DISPFLG] = 0;

		ActWork[no].work[RAINWK_ROOTID] = c * RAIN_NEXT_ROOT_STEP;
		while(ActWork[no].work[RAINWK_ROOTID] >= 600)
		{
			ActWork[no].work[RAINWK_ROOTID] -= 600;
		}
		rain_set_nextpos(&ActWork[no]);
		rain_actmove_roop(&ActWork[no], c*9);

		ActWork[no].banish = 1;
		wp->rain_actor[c] = &ActWork[no];

	}else{
		wp->rain_actor[c] = NULL;
	}

	if(++(wp->rain_obj_cnt) == RAIN_OBJ_MAX)
	{
		u16 i;
		for(i = 0; i < RAIN_OBJ_MAX; i++){
			if(wp->rain_actor[i] != NULL){
				if(wp->rain_actor[i]->work[RAINWK_MOVEFUNC] == 0){
					wp->rain_actor[i]->move = rain_act_move;
				}else{
					wp->rain_actor[i]->move = rain_actmove_countwait;
				}
			}
		}
		return 0;
	}

	return 1;
}
//==================================================
//  �J�����\�����̒���  0:�I��  1:������
//==================================================
u8 AdjustRainActor(void)
{
	if(wp->rain_disp_cnt == wp->rain_disp_max){ return 0; }

	if(++(wp->rain_disp_timer) > wp->rain_disp_wait){
		wp->rain_disp_timer = 0;
		if(wp->rain_disp_cnt < wp->rain_disp_max){
			wp->rain_actor[wp->rain_disp_cnt++]->work[RAINWK_DISPFLG] = 1;
		}else{
			wp->rain_disp_cnt--;
			wp->rain_actor[wp->rain_disp_cnt]->work[RAINWK_DISPFLG] = 0;
			wp->rain_actor[wp->rain_disp_cnt]->banish = 1;
		}
	}
	return 1;
}
//==================================================
//  �J�����S�폜
//==================================================
void DelRainObj(void)
{
	u16 i;
	for(i=0 ; i < wp->rain_obj_cnt; i++)
	{
		if(wp->rain_actor[i] != NULL){
			DelActor(wp->rain_actor[i]);
		}
	}
	wp->rain_obj_cnt = 0;
	CellDel(WEATHER_CELLID_RAIN);
}
