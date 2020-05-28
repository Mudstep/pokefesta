/**********************************************************************/
/*  �p�\�R���{�b�N�X������  -�J�[�\��-                                */
/**********************************************************************/
#include "common.h"
#include "intr.h"
#include "decord.h"
#include "actanm.h"
#include "actor.h"
#include "palanm.h"
#include "config.h"
#include "wazano.def"
#include "monsno.def"
#include "p_status.h"
#include "item.h"
#include "message.h"

#include "boxsub.h"
#include "box_cursor.dat"
#include "box2.h"


/**********************************************************************/
/*  �}�N���E�萔                                                      */
/**********************************************************************/
//#define CURSOR_PRI  (


#define TAGMODE_XPOS   (120)
#define TAGMODE_YPOS   (14)
#define TAGMODE_CATCH_YPOS  (8)
#define TAGMODE_XDIFF  (88)

enum CURSORANM {
	CURSORANM_WAIT,
	CURSORANM_STOP,
	CURSORANM_OPEN,
	CURSORANM_CATCH,
};

enum CURSORMODE {
	CURSORMODE_TRAY,
	CURSORMODE_MINE,
	CURSORMODE_MENU,
	CURSORMODE_TAG,
	CURSORMODE_BOXSEL,
};

enum PARAMTYPE{
	PARAMTYPE_MINE,
	PARAMTYPE_TRAY,
	PARAMTYPE_NULL,
};

/**********************************************************************/
/*  �O���[�o���ϐ�                                                    */
/**********************************************************************/
static s8 CursorMode = CURSORMODE_TRAY;
static s8 CursorPos = 0;
static u8 IconCatchFlg = 0;
static u8 CatchPokeBox = 0;
static u8 CatchPokePos = 0;
static u8 CursorQuickFlg = 0;

/**********************************************************************/
/*  �v���g�^�C�v                                                      */
/**********************************************************************/
static void add_cursor_actor(void);
static void get_cursor_pos(u8 mode, u8 pos, s16 *x, s16 *y);
static void set_move_param(u8 nextmode, u8 nextpos);
static void pre_move(u8 nextmode, u8 nextpos);
static void finish_move(void);
static void calc_move_param(void);
static void update_pointpoke_status(void);
static void recover_pointpoke_status(void);
static void set_status_param(void *param_add, u8 type);
static u16 get_point_monsno(void);
static void quickmode_switch(void);

static void take_pokedata(u8 box, u8 pos);
static void put_pokedata(u8 box, u8 pos);
static void swap_pokedata(u8 box, u8 pos);
static void del_pokedata(u8 box, u8 pos);

/*-- ���j���[������ --*/
static void initmenu_tray(void);
static u8 initmenu_poke(void);

/*==============================================
==	������
==============================================*/
void InitBoxCursor(void)
{
	if(Work->mode != BOX_MODE_AZUKERU){
		CursorMode = CURSORMODE_TRAY;
	}else{
		CursorMode = CURSORMODE_MINE;
	}
	CursorPos = 0;
	IconCatchFlg = 0;
	CatchPokeBox = 0;
	CatchPokePos = 0;
	CursorQuickFlg = 0;

	TrayReturnPosInit();

	add_cursor_actor();
	Work->mine_pos_save = 1;

	update_pointpoke_status();
}
/*==============================================
==	���A
==============================================*/
void RecoverBoxCursor(void)
{
	add_cursor_actor();
	recover_pointpoke_status();
	Work->mine_pos_save = 1;

	if(IconCatchFlg){
//		Work->PP = *POKETMP_ADDR;
		RecoverBoxCatchIcon();
	}
}
/*==============================================
==	�J�[�\�����W�v�Z
==	mode   �J�[�\�����샂�[�h
==	pos    �J�[�\���ʒu�ԍ�
==	x, y   �ʒu�f�[�^���󂯎��ϐ��A�h���X
==============================================*/
static void get_cursor_pos(u8 mode, u8 pos, s16 *x, s16 *y)
{
	switch(mode){
	case CURSORMODE_TRAY:
		*x = TRAYICON_XORG + (pos % 6) * ICON_WIDTH;
		*y = (TRAYICON_YORG + (pos / 6) * ICON_HEIGHT) - CURSOR_ICON_DIFF;
		break;

	case CURSORMODE_MINE:
		if(pos == 0){
			*x = TOPICON_XPOS;
			*y = TOPICON_YPOS - CURSOR_ICON_DIFF;
		}else if(pos == MINEBOX_CURSOR_MAXPOS){
			*x = MINEBOX_MODORU_XPOS;
			*y = MINEBOX_MODORU_YPOS - CURSOR_ICON_DIFF;
		}else{
			*x = MINEICON_XORG;
			*y = MINEICON_YORG + (pos-1)*ICON_HEIGHT - CURSOR_ICON_DIFF;
		}
		break;

	case CURSORMODE_MENU:
		*x = 162;//92 + pos * 70;
		*y = 12;
		break;

	case CURSORMODE_TAG:
		*y = (IconCatchFlg)? TAGMODE_CATCH_YPOS : TAGMODE_YPOS;
		*x = TAGMODE_XPOS + pos * TAGMODE_XDIFF;
		break;

	case CURSORMODE_BOXSEL:
		*x = 160;
		*y = 96;
		break;
	}
}
/*==============================================
==	���݃J�[�\�����w���Ă���|�P�����i���o�[��Ԃ�
==	return: �|�P�����i���o�[
==============================================*/
static u16 get_point_monsno(void)
{
	switch(CursorMode){
	case CURSORMODE_MINE:
		return PokeParaGet(PokeParaMine+CursorPos, ID_monsno);

	case CURSORMODE_TRAY:
	{
		u8 current = GetCurrentBox();
		return PokePasoParaGet(&(BoxData.PPPBox[current][CursorPos]), ID_monsno);
	}

	default:
		return NON_POKE;
	}
}
/**********************************************************************/
/*  �J�[�\���ړ�                                                      */
/**********************************************************************/
#define TOP_LIMIT     (-16)
#define BOTTOM_LIMIT  (176)
#define LEFT_LIMIT    (64)
#define RIGHT_LIMIT   (256)

/*==============================================
==	�J�[�\���ړ�����
==	return: �ړ��I�� 0,  �ړ��� 1
==============================================*/
u8 BoxCursorMoveWait(void)
{
	if(Work->cursor_move_sync == 0){ return 0; }


	if(--(Work->cursor_move_sync)){
		Work->cursor_x += Work->cursor_ax;
		Work->cursor_y += Work->cursor_ay;
		Work->cursor_actor->x = Work->cursor_x >> 8;
		Work->cursor_actor->y = Work->cursor_y >> 8;

		if(Work->cursor_actor->x > RIGHT_LIMIT){
			s16 d = Work->cursor_actor->x - RIGHT_LIMIT;
			Work->cursor_actor->x = LEFT_LIMIT + d;
		}
		if(Work->cursor_actor->x < LEFT_LIMIT){
			s16 d = LEFT_LIMIT - Work->cursor_actor->x;
			Work->cursor_actor->x = RIGHT_LIMIT - d;
		}
		if(Work->cursor_actor->y > BOTTOM_LIMIT){
			s16 d = Work->cursor_actor->y - BOTTOM_LIMIT;
			Work->cursor_actor->y = TOP_LIMIT + d;
		}
		if(Work->cursor_actor->y < TOP_LIMIT){
			s16 d = TOP_LIMIT - Work->cursor_actor->y;
			Work->cursor_actor->y = BOTTOM_LIMIT - d;
		}

		if(Work->cursor_vflip)
		{
			if(--(Work->cursor_vflip) == 0)
			{
				Work->cursor_actor->Vflip = !(Work->cursor_actor->Vflip);
			}
		}

		return 1;
	}

	Work->cursor_actor->x = Work->cursor_end_x;
	Work->cursor_actor->y = Work->cursor_end_y;
	finish_move();
	return 1;
}
/*==============================================
==	�J�[�\������p�����[�^�Z�b�g
==	nextmode  �ړ��ヂ�[�h
==	nextpos   �ړ���ʒu�i���o�[
==============================================*/
static void set_move_param(u8 nextmode, u8 nextpos)
{
	s16 x, y;
	get_cursor_pos(nextmode, nextpos, &x, &y);
	Work->cursor_nextmode = nextmode;
	Work->cursor_nextpos = nextpos;
	Work->cursor_end_x = x;
	Work->cursor_end_y = y;
}
/*==============================================
==	�J�[�\���ړ��v�Z
==============================================*/
static void calc_move_param(void)
{
	s32  dx, dy;

	if(Work->cursor_hloop || Work->cursor_vloop){
		Work->cursor_move_sync = 12;
	}else{
		Work->cursor_move_sync = 6;
	}

	if(Work->cursor_vflip)
	{
		Work->cursor_vflip = Work->cursor_move_sync / 2;
	}

	switch(Work->cursor_vloop){
	case -1: dy = -((BOTTOM_LIMIT-Work->cursor_end_y)+(Work->cursor_actor->y-TOP_LIMIT)); break;
	case  1: dy = (Work->cursor_end_y-TOP_LIMIT)+(BOTTOM_LIMIT-Work->cursor_actor->y); break;
	default: dy = Work->cursor_end_y - Work->cursor_actor->y; break;
	}
	switch(Work->cursor_hloop){
	case -1: dx = -((RIGHT_LIMIT-Work->cursor_end_x)+(Work->cursor_actor->x-LEFT_LIMIT)); break;
	case  1: dx = (Work->cursor_end_x-LEFT_LIMIT)+(RIGHT_LIMIT-Work->cursor_actor->x); break;
	default: dx = Work->cursor_end_x - Work->cursor_actor->x; break;
	}

	dy <<= 8;
	dx <<= 8;

	Work->cursor_ax = dx / Work->cursor_move_sync;
	Work->cursor_ay = dy / Work->cursor_move_sync;

	Work->cursor_x = Work->cursor_actor->x << 8;
	Work->cursor_y = Work->cursor_actor->y << 8;

}
/*==============================================
==	�J�[�\������O����
==============================================*/
static void pre_move(u8 nextmode, u8 nextpos)
{
	set_move_param(nextmode, nextpos);
	calc_move_param();

	if(!IconCatchFlg){
		ActAnmChg(Work->cursor_actor, CURSORANM_STOP);
	}

	// �ʃ��[�h����Ă������[�h�ɓ��鎞�A
	// �E�L�[�ł̈ړ�����P�Ԃɂ��Ă���
	if(nextmode == CURSORMODE_MINE &&
	   CursorMode != CURSORMODE_MINE)
	{
		Work->mine_pos_save = 1;
		Work->shadow_actor->banish = 1;
	}

	switch(nextmode){
	case CURSORMODE_MENU:
	case CURSORMODE_TAG:
	case CURSORMODE_MINE:
		Work->shadow_actor->banish = 1;
		Work->shadow_actor->oamData.Priority = 1;
		break;

	case CURSORMODE_TRAY:
	}
}
/*==============================================
==	�J�[�\������㏈��
==============================================*/
static void finish_move(void)
{
	CursorMode = Work->cursor_nextmode;
	CursorPos = Work->cursor_nextpos;

	if(!IconCatchFlg){
		ActAnmChg(Work->cursor_actor, CURSORANM_WAIT);
	}

	update_pointpoke_status();

	switch(CursorMode){
	case CURSORMODE_TAG:
		break;

	case CURSORMODE_MENU:
		BoxArrowMove(1);
		break;

	case CURSORMODE_MINE:
		Work->shadow_actor->pri = ACTPRI_MINESHADOW;
		break;

	case CURSORMODE_TRAY:
		Work->shadow_actor->oamData.Priority = 2;
		Work->shadow_actor->pri = ACTPRI_TRAYSHADOW;
		Work->shadow_actor->banish = 0;
		break;
	}

}
/**********************************************************************/
/*  ��������ݒ�                                                      */
/**********************************************************************/
/*==============================================
==	�Ă������[�h�֕ύX
==============================================*/
void BoxCursorMoveToMine(void)
{
	u8 pos;
	if(IconCatchFlg == 0){
		pos = 0;
	}else{
		pos = PokeCountMineGet();
		if(pos >= 6){ pos = 5; }
	}

	if(Work->cursor_actor->Vflip)
	{
		Work->cursor_vflip = 1;
	}
	
	pre_move(CURSORMODE_MINE, pos);
}
/*==============================================
==	�g���C���[�h�֕ύX
==	pos  �ړ���ʒu
==============================================*/
void BoxCursorMoveToTray(u8 pos)
{
	pre_move(CURSORMODE_TRAY, pos);
}
/**********************************************************************/
/*  �Ă������g���C�ֈړ����̈ʒu�ԍ�                                  */
/**********************************************************************/
static u8 TrayReturnPos = 0;

/*==============================================
==	������������сA�^�O����Ă����{�b�N�X�ɓ���
==	���ɃN���A
==============================================*/
void TrayReturnPosInit(void)
{
	TrayReturnPos = 0;
}
/*==============================================
==	�g���C����u��Ă����v��I�񂾂Ƃ��ɕۑ�
==============================================*/
void TrayReturnPosSave(void)
{
	TrayReturnPos = CursorPos;
}
/*==============================================
==	�Ă����{�b�N�X�𔲂��鎞�ɓǂݏo��
==============================================*/
u8 TrayReturnPosLoad(void)
{
	return TrayReturnPos;
}

/**********************************************************************/
/*  ���C������̖��ߏ���                                              */
/**********************************************************************/
static u8 op_tukamu(void);
static u8 op_oku(void);
static u8 op_irekaeru(void);

static void catch_poke(void);
static void put_poke(void);
static u8 tukamu_down(void);
static u8 tukamu_up(void);

/*==============================================
==	������
==	op   ���߃R�[�h�ienum CURSOR_OP�j
==============================================*/
void BoxCursorOP_Init(u8 op)
{
	static u8 (* const op_func_tbl[])(void) = {
		op_tukamu,
		op_oku,
		op_irekaeru,
	};
	Work->cursor_op = op_func_tbl[op];
	Work->cursor_op_prog = 0;
}
/*==============================================
==	���ߎ��s
==	return: �I�� 0, �p���� 1
==============================================*/
u8 BoxCursorOP_Exe(void)
{
	return (*(Work->cursor_op))();
}

/*==============================================
==	�u���ށv����
==	return : �I�� 0,  �p���� 1
==============================================*/
static u8 op_tukamu(void)
{
	switch(Work->cursor_op_prog){
	case 0:
		if(IconCatchFlg){ return 0; }
		ActAnmChg(Work->cursor_actor, CURSORANM_OPEN);
		Work->cursor_op_prog++;
		break;

	case 1:
		if(!tukamu_down()){
			ActAnmChg(Work->cursor_actor, CURSORANM_CATCH);
			catch_poke();
			Work->cursor_op_prog++;
		}
		break;

	case 2:
		if(!tukamu_up()){
			Work->cursor_op_prog++;
		}
		break;

	case 3:
		return 0;

	}
	return 1;
}
/*==============================================
==	�u�����v����
==	return : �I�� 0,  �p���� 1
==============================================*/
static u8 op_oku(void)
{
	switch(Work->cursor_op_prog){
	case 0:
		if(!tukamu_down()){
			ActAnmChg(Work->cursor_actor, CURSORANM_OPEN);
			put_poke();
			Work->cursor_op_prog++;
		}
		break;

	case 1:
		if(!tukamu_up()){
			ActAnmChg(Work->cursor_actor, CURSORANM_WAIT);
			Work->cursor_op_prog++;
		}
		break;

	case 2:
		return 0;
	}
	return 1;
}
/*==============================================
==	�u���ꂩ����v����
==	return : �I�� 0,  �p���� 1
==============================================*/
static u8 op_irekaeru(void)
{
	switch(Work->cursor_op_prog){
	case 0:
		switch(CursorMode){
		case CURSORMODE_MINE: Work->swap_poke_box = BOX_KAZU; break;
		case CURSORMODE_TRAY: Work->swap_poke_box = GetCurrentBox(); break;
		default: return 0;
		}
		ActAnmChg(Work->cursor_actor, CURSORANM_OPEN);
		BoxIconSwapInit(Work->swap_poke_box, CursorPos);
		Work->cursor_op_prog++;
		break;

	case 1:
		if(!BoxIconSwapWait()){
			ActAnmChg(Work->cursor_actor, CURSORANM_CATCH);
			swap_pokedata(Work->swap_poke_box, CursorPos);
			Work->cursor_op_prog++;
		}
		break;

	case 2:
		return 0;
	}
	return 1;
}
/*==============================================
==	�u���ށv���̃J�[�\���_�E��
==	return : �I�� 0,  �p���� 1
==============================================*/
static u8 tukamu_down(void)
{
	switch(Work->cursor_actor->dy){
	case 0:
		Work->cursor_actor->dy++;
		break;

	case CURSOR_TUKAMU_DIPTH:
		return 0;

	default:
		Work->cursor_actor->dy++;
		break;
	}
	return 1;
}
/*==============================================
==	�u���ށv���̃J�[�\���A�b�v
==	return : �I�� 0,  �p���� 1
==============================================*/
static u8 tukamu_up(void)
{
	switch(Work->cursor_actor->dy){
	case 0:
		return 0;

	default:
		Work->cursor_actor->dy--;
		break;
	}
	return 1;
}
/*==============================================
==	���݈ʒu�̃|�P�����f�[�^���A�u����ł���v
==	�|�P�����Ƃ��ă��[�N�Ɏ擾
==============================================*/
static void catch_poke(void)
{
	switch(CursorMode){
	case CURSORMODE_MINE:
		take_pokedata(BOX_KAZU, CursorPos);
		BoxIconCatch(ICONTYPE_MINE, CursorPos);
		break;

	case CURSORMODE_TRAY:
		{
			u8 current = GetCurrentBox();
			take_pokedata(current, CursorPos);
			BoxIconCatch(ICONTYPE_TRAY, CursorPos);
		}
		break;

	default:
		return;
	}

	IconCatchFlg = 1;
}
/*==============================================
==	�u����ł���v�|�P�������A���݈ʒu��
==	�ۑ�����
==============================================*/
static void put_poke(void)
{
	switch(CursorMode){
	case CURSORMODE_MINE:
		put_pokedata(BOX_KAZU, CursorPos);
		BoxIconPut(BOX_KAZU, CursorPos);
		break;

	case CURSORMODE_TRAY:
		{
			u8 current = GetCurrentBox();
			put_pokedata(current, CursorPos);
			BoxIconPut(current, CursorPos);
		}
		break;

	default:
		return;
	}

	IconCatchFlg = 0;
}
/*==============================================
==	�I�����Ă����߹�ݽð�����������I�ɍX�V
==============================================*/
void BoxUpdateStatusParam(void)
{
	update_pointpoke_status();
}
/**********************************************************************/
/*  �|�P�����f�[�^����                                                */
/**********************************************************************/
/*==============================================
==	�|�P�����f�[�^�����[�N�Ɉړ�������
==	box  �{�b�N�X�ԍ��i==BOX_KAZU �Ȃ�莝���j
==	pos  �{�b�N�X���̈ʒu
==============================================*/
static void take_pokedata(u8 box, u8 pos)
{
//	u16 nonpoke = NON_POKE;

	if(box == BOX_KAZU){
		Work->PP = PokeParaMine[CursorPos];
//		PokeParaPut(PokeParaMine+CursorPos, ID_monsno, (u8*)(&nonpoke));
	}else{
		PokeReplace(&(BoxData.PPPBox[box][pos]), &(Work->PP));
//		PokePasoParaPut(&(BoxData.PPPBox[box][CursorPos]), ID_monsno, (u8*)(&nonpoke));
	}
	del_pokedata(box, pos);

	CatchPokeBox = box;
	CatchPokePos = pos;
}
/*==============================================
==	���[�N�̃|�P�����f�[�^���w��ʒu�Ɉړ�������
==	box  �{�b�N�X�ԍ��i==BOX_KAZU �Ȃ�莝���j
==	pos  �{�b�N�X���̈ʒu
==============================================*/
static void put_pokedata(u8 box, u8 pos)
{
	if(box == BOX_KAZU){
#if 1
		PokeParaMine[pos] = Work->PP;
#else
		if(CatchPokeBox == BOX_KAZU){
			PokeParaMine[pos] = Work->PP;
		}else{
			PokeReplace(&(Work->PP.PPP), &PokeParaMine[pos]);
		}
#endif
	}else{
		// PP���ő�ɉ�
		PokePasoPPMaxSet(&(Work->PP.PPP));
		BoxData.PPPBox[box][pos] = Work->PP.PPP;
	}
}
/*==============================================
==	�{�b�N�X�̃|�P�����f�[�^���폜
==	box  �{�b�N�X�ԍ��i==BOX_KAZU �Ȃ�莝���j
==	pos  �{�b�N�X���̈ʒu
==============================================*/
static void del_pokedata(u8 box, u8 pos)
{
//	u16 nonpoke = NON_POKE;

	if(box == BOX_KAZU){
//		PokeParaPut(PokeParaMine+pos, ID_monsno, (u8*)(&nonpoke));
		PokeParaInit(PokeParaMine + pos);
	}else{
//		PokePasoParaPut(&(BoxData.PPPBox[box][pos]), ID_monsno, (u8*)(&nonpoke));
		PokePasoParaInit(&(BoxData.PPPBox[box][pos]));
	}
}
/*==============================================
==	���[�N�̃|�P�����f�[�^�Ǝw��ʒu�̃|�P�����f�[�^�����ւ���
==	box  �{�b�N�X�ԍ��i==BOX_KAZU �Ȃ�莝���j
==	pos  �{�b�N�X���̈ʒu
==============================================*/
static void swap_pokedata(u8 box, u8 pos)
{
	if(box == BOX_KAZU){
		Work->PPtmp = PokeParaMine[pos];
	}else{
		PokeReplace(&(BoxData.PPPBox[box][pos]), &(Work->PPtmp));
	}
	put_pokedata(box, pos);
	Work->PP = Work->PPtmp;
	set_status_param(&(Work->PP), PARAMTYPE_MINE);
	CatchPokeBox = box;
	CatchPokePos = pos;
}
/*==============================================
==	�J�[�\����Ԃɉ����āA�|�P�������u��������v
==	���̃f�[�^�A�A�C�R���������s��
==	box  ��������̃{�b�N�X�i���o�[
==	return: ����  1,  �{�b�N�X�������ς� 0
==============================================*/
u8 BoxAzukeruInit(u8 box)
{
	s16 pos = GetPasoBoxBlankPos(box);
	if(pos == -1){ return 0; }

	if(IconCatchFlg){
		put_pokedata(box, pos);
		BoxIcon_DelCatch();
		IconCatchFlg = 0;
	}else{
		take_pokedata(BOX_KAZU, CursorPos);
		put_pokedata(box, pos);
		DelMineIcon(CursorPos);
	}

	if(box == GetCurrentBox()){
		BoxIcon_TrayUpdate(pos);
	}

	ActAnmChg(Work->cursor_actor, CURSORANM_STOP);

	return 1;
}
/*==============================================
==	�u��������v��́A�J�[�\����ԕ������X�e�[�^�X�X�V
==============================================*/
void BoxAzukeruFinish(void)
{
	ActAnmChg(Work->cursor_actor, CURSORANM_WAIT);
	update_pointpoke_status();
}
/*==============================================
==	�J�[�\����Ԃɉ����āA�|�P�������u�ɂ����v
==	�A�C�R���A�j�����J�n
==============================================*/
void BoxNigasuAnmInit(void)
{
	u8 icon;

	if(IconCatchFlg){
		icon = ICONTYPE_CATCH;
	}else{
		if(CursorMode == CURSORMODE_MINE){
			icon = ICONTYPE_MINE;
		}else{
			icon = ICONTYPE_TRAY;
		}
//		del_pokedata(box, CursorPos);
	}
	BoxIcon_NigasuInit(icon, CursorPos);

	PM_strcpy(Work->nigashita_pokename, Work->status_pokename);
}
/*==============================================
==	�u�ɂ����v�����҂�
==	�I�� 0,  �p���� 1
==============================================*/
u8 BoxNigasuAnmWait(void)
{
	if(!BoxIcon_NigasuWait()){
		ActAnmChg(Work->cursor_actor, CURSORANM_WAIT);
		return 0;
	}
	return 1;
}
/*==============================================
==	�u�ɂ����v��̃X�e�[�^�X�X�V
==============================================*/
void BoxNigasuFinish(void)
{
	BoxIcon_NigasuFinish();

	if(IconCatchFlg){
		IconCatchFlg = 0;
	}else{
		u8 box;
		if(CursorMode == CURSORMODE_MINE){
			box = BOX_KAZU;
		}else{
			box = GetCurrentBox();
		}
		del_pokedata(box, CursorPos);
	}

	update_pointpoke_status();
}
/*==============================================
==	�ɂ������A�C�R���́u���ǂ�v�A�j���I���㏈��
==============================================*/
void BoxModoruFinish(void)
{
	if(IconCatchFlg){
		ActAnmChg(Work->cursor_actor, CURSORANM_CATCH);
	}
}




/*==============================================
==	�J�[�\�����w���߹�݂́A��`�Z����
==�i�������j
==============================================*/
#define NIGASERU_CHK_UNIT  (5)  // ��x�����������߹�ݐ�
void BoxNigaseruCheckInit(void)
{
	u16 ret;

	if(IconCatchFlg){
		Work->PPtmp = Work->PP;

//		if(CatchPokeBox == BOX_KAZU){
//			Work->PPtmp = Work->PP;
//		}else{
//			PokeReplace(&(Work->PP.PPP), &(Work->PPtmp));
//		}

		Work->nigaseru_target_box = -1;
		Work->nigaseru_target_pos = -1;
	}else{
		if(CursorMode == CURSORMODE_MINE){
			Work->PPtmp = PokeParaMine[CursorPos];
			Work->nigaseru_target_box = BOX_KAZU;
		}else{
			PokeReplace(&(BoxData.PPPBox[BoxData.CurrentBoxNo][CursorPos]), &(Work->PPtmp));
			Work->nigaseru_target_box = BoxData.CurrentBoxNo;
		}
		Work->nigaseru_target_pos = CursorPos;
	}

	Work->nigaseru_naminori_chk = 0;
	Work->nigaseru_diving_chk = 0;

	Work->nigaseru_chk_wazano[0] = WAZANO_NAMINORI;
	Work->nigaseru_chk_wazano[1] = WAZANO_DAIBINGU;
	Work->nigaseru_chk_wazano[2] = WAZANO_MAX + 1;  // �I�[

	ret = PokeParaGet(&(Work->PPtmp), ID_waza_have_check, (u8*)(Work->nigaseru_chk_wazano));
	Work->nigaseru_naminori_chk = ((ret & 1) != 0);
	Work->nigaseru_diving_chk =  ((ret & 2) != 0);

	if(Work->nigaseru_naminori_chk
	|| Work->nigaseru_diving_chk )
	{
		Work->nigaseru_chk_end = 0;
	}
	else
	{
		Work->nigaseru_chk_end = 1;
		Work->nigaseru_chk_result = 1;
	}

	Work->nigaseru_chk_prog = 0;

}
/*==============================================
==	�J�[�\�����w���߹�݂́A��`�Z�����i���s�j
==  return:  ��������:1   �������Ȃ�:0  ���ג�:-1
==============================================*/
s8  BoxNigaseruCheck(void)
{
	u16 i, ret;

	if(Work->nigaseru_chk_end)
	{
		return Work->nigaseru_chk_result;
	}

	switch(Work->nigaseru_chk_prog){
	// �Ă����`�F�b�N
	case 0:
		for(i = 0; i < TEMOTI_POKEMAX; i++)
		{
			if(Work->nigaseru_target_box==BOX_KAZU
			&& Work->nigaseru_target_pos == i){
				continue;
			}
			ret = PokeParaGet(PokeParaMine+i, ID_waza_have_check, (u8*)(Work->nigaseru_chk_wazano));

			if(ret&1){ Work->nigaseru_naminori_chk = 0; }
			if(ret&2){ Work->nigaseru_diving_chk = 0; }
		}

		if(Work->nigaseru_naminori_chk == 0
		&& Work->nigaseru_diving_chk == 0 )
		{
			Work->nigaseru_chk_end = 1;
			Work->nigaseru_chk_result = 1;
			break;
		}

		Work->nigaseru_chk_box = 0;
		Work->nigaseru_chk_pos = 0;
		Work->nigaseru_chk_prog++;
		break;

	// �{�b�N�X�`�F�b�N
	case 1:
		for(i = 0; i < NIGASERU_CHK_UNIT; i++)
		{
			ret = PokePasoParaGet(&(BoxData.PPPBox[Work->nigaseru_chk_box][Work->nigaseru_chk_pos]),  ID_waza_have_check, (u8*)(Work->nigaseru_chk_wazano));
			if(ret){
				if(Work->nigaseru_target_box != Work->nigaseru_chk_box
				|| Work->nigaseru_target_pos != Work->nigaseru_chk_pos)
				{
					if(ret & 1){ Work->nigaseru_naminori_chk = 0; }
					if(ret & 2){ Work->nigaseru_diving_chk = 0; }
				}
			}

			if(++(Work->nigaseru_chk_pos) >= BOX_PM_KAZU)
			{
				Work->nigaseru_chk_pos = 0;
				if(++(Work->nigaseru_chk_box) >= BOX_KAZU)
				{
					Work->nigaseru_chk_end = 1;
					Work->nigaseru_chk_result = 0;
					break;
				}
			}
		}

		if(Work->nigaseru_naminori_chk == 0
		&& Work->nigaseru_diving_chk == 0 )
		{
			Work->nigaseru_chk_end = 1;
			Work->nigaseru_chk_result = 1;
		}
		break;


	}

	return -1;
}
/*==============================================
==	����ł���|�P�����f�[�^�ꎞ�̈�ɃZ�b�g
==============================================*/
void BoxSetTmpPokeData(void)
{
	if(IconCatchFlg){
		*POKETMP_ADDR = Work->PP;

//		if(CatchPokeBox==BOX_KAZU){
//			*POKETMP_ADDR = Work->PP;
//		}else{
//			PokeReplace(&(Work->PP.PPP), POKETMP_ADDR);
//		}
	}
}
/*==============================================
==	�ꎞ�̈�ɃZ�b�g�����|�P�����f�[�^�����ɖ߂�
==============================================*/
void BoxGetTmpPokeData(void)
{
	if(IconCatchFlg){
		if(CatchPokeBox==BOX_KAZU){
			Work->PP = *POKETMP_ADDR;
		}else{
			Work->PP.PPP = POKETMP_ADDR->PPP;
		}
	}
//	}else{
//		if(CursorMode == CURSORMODE_MINE){
//			PokeParaMine[CursorPos] = *POKETMP_ADDR;
//		}else{
//			u8 current = GetCurrentBox();
//			BoxData.PPPBox[current][CursorPos] = POKETMP_ADDR->PPP;
//		}

}
/*==============================================
==	�u�悤�����݂�v�Ŏg��
==  PokeStatusCall2 �ɓn�����������[�N�ɗp�ӂ���
==============================================*/
void BoxYousuParamSet(void)
{
	if(IconCatchFlg){
		BoxSetTmpPokeData();
		Work->yousu_array_addr = (void*)POKETMP_ADDR;
		Work->yousu_array_pos = 0;
		Work->yousu_array_max = 0;
		Work->yousu_type = LIST_ST;
	}else{
		if(CursorMode == CURSORMODE_MINE){
			Work->yousu_array_addr = (void*)(&PokeParaMine[0]);
			Work->yousu_array_pos = CursorPos;
			Work->yousu_array_max = Box_CountMine() - 1;
			Work->yousu_type = LIST_ST;
//			*POKETMP_ADDR = PokeParaMine[CursorPos];
		}else{
			Work->yousu_array_addr = (void*)(&(BoxData.PPPBox[BoxData.CurrentBoxNo][0]));
			Work->yousu_array_pos = CursorPos;
			Work->yousu_array_max = BOX_PM_KAZU - 1;
			Work->yousu_type = BOX_ST;
//			u8 current = GetCurrentBox();
//			PokeReplace(&(BoxData.PPPBox[current][CursorPos]), POKETMP_ADDR);
		}
	}
}
/*==============================================
==	�u�悤�����݂�v�Ŏg�����p�����[�^����
==	�J�[�\���ʒu���̃f�[�^�𕜋A
==============================================*/
void BoxYousuParamRecover(void)
{
	if(IconCatchFlg){
		BoxGetTmpPokeData();
	}else{
		CursorPos = P_SYSTEM->put_no;
	}
}
/*==============================================
==	�Ă����|�P�����f�[�^�̌��������l�߂�
==	return : �������̂������ꏊ�i�Ȃ���� -1�j
==============================================*/
s16 BoxTumeMineData(void)
{
	u16 i, p, monsno;
	s16 nuke = -1;

	for(i = 0, p = 0; i < TEMOTI_POKEMAX; i++){
		monsno = PokeParaGet(PokeParaMine + i, ID_monsno);
		if(monsno != NON_POKE){
			if(i != p){
				PokeParaMine[p] = PokeParaMine[i];
			}
			p++;
		}else{
			if(nuke == -1){ nuke = i; }
		}
	}

	monsno = NON_POKE;

	while(p < TEMOTI_POKEMAX){
		PokeParaInit(PokeParaMine+p);
		p++;
	}

	return nuke;

}
/*==============================================
==	�J�[�\���̎w���Ă���i����ł���j�|�P����
==	�̃}�[�L���O�l���Z�b�g
==	mark  �Z�b�g����}�[�L���O�l
==============================================*/
void BoxSetMarkingParam(u8 mark)
{
	Work->status_mark = mark;
	if(IconCatchFlg){
		PokeParaPut(&(Work->PP), ID_mark, &mark);
	}else{
		if(CursorMode == CURSORMODE_MINE){
			PokeParaPut(PokeParaMine + CursorPos, ID_mark, &mark);
//			PokeParaMine[CursorPos].PPP.mark = mark;
		}
		if(CursorMode == CURSORMODE_TRAY){
			u8 current = GetCurrentBox();
//			BoxData.PPPBox[current][CursorPos].mark = mark;
			PokePasoParaPut(&(BoxData.PPPBox[current][CursorPos]), ID_mark, &mark);
		}
	}
}
/*==============================================
==	�J�[�\���̎w���|�P�������莝���Ō�̂P�̂��H
==	return: 1 ... �Ō�̂P�̂ł�
==	        0 ... �Ō�̂P�̂���Ȃ��ł�
==============================================*/
u8 CheckLastOneMine(void)
{
	if(CursorMode != CURSORMODE_MINE) return 0;
	if(IconCatchFlg) return 0;

//	if(Box_CountMinePoke() > 1) return 0;
//	if(PokeParaGet(PokeParaMine + CursorPos, ID_tamago_flag)) return 0;
	if(Box_CountMinePokeExSub(CursorPos)!=0) return 0;

	return 1;
}
/*==============================================
==	�u���ꂩ����v�����s�ł��邩�`�F�b�N
==	return: 1 ... �ł���
==	        0 ... �ł��Ȃ��i�莝���Ō�̂P�́j
==============================================*/
u8 CheckIrekaeOK(void)
{
	if(IconCatchFlg == 0) return 0;

	if(CursorMode != CURSORMODE_MINE) return 1;

//	if(Box_CountMinePoke() > 1) return 1;
//	if(PokeParaGet(PokeParaMine + CursorPos, ID_tamago_flag)) return 1;
	if(Box_CountMinePokeExSub(CursorPos) > 0) return 1;

	if(Work->status_tamago_flg) return 0;
	if(PokeParaGet(&(Work->PP), ID_hp)==0) return 0;

	return 1;
}

/*==============================================
==	�A�C�R��������ł����Ԃ��`�F�b�N
==	return: ����ł��� 1,  ����ł��Ȃ� 0
==============================================*/
u8 CheckCatchIcon(void)
{
	return IconCatchFlg;
}
/*==============================================
==	���������ɃX�N���[���{�^���𓮍삳����󋵂��ǂ����`�F�b�N
==	return: ���삳����  1,   ���삳���Ȃ�  0
==============================================*/
u8 CheckArrowMoveState(void)
{
	return CursorMode == CURSORMODE_MENU;
}
/*==============================================
==	�J�[�\�����Ƃ���{�^���̏�ɂ��邩�`�F�b�N
==	return: �Ƃ���{�^���̏� 1,  ����ȊO 0
==============================================*/
u8 CheckBoxCursorOnCloseButton(void)
{
	return (CursorMode == CURSORMODE_TAG && CursorPos == 1);
}
/**********************************************************************/
/*  �X�e�[�^�X�\���f�[�^                                              */
/**********************************************************************/
/*==============================================
==	�|�C���g���Ă���|�P�����̃p�����[�^�X�V
==============================================*/
static void update_pointpoke_status(void)
{
	Work->point_poke_update = !IconCatchFlg;
	if(IconCatchFlg){ return; }

	switch(CursorMode){
	case CURSORMODE_MENU:
	case CURSORMODE_TAG:
		set_status_param(NULL, PARAMTYPE_NULL);
		break;

	case CURSORMODE_MINE:
		if(CursorPos < TEMOTI_POKEMAX){
			set_status_param((void*)(&PokeParaMine[CursorPos]), PARAMTYPE_MINE);
		}else{
			set_status_param(NULL, PARAMTYPE_NULL);
		}
		break;

	case CURSORMODE_TRAY:
		{
			u8 current = GetCurrentBox();
			set_status_param((void*)(&BoxData.PPPBox[current][CursorPos]), PARAMTYPE_TRAY);
		}
		break;
	}
}
/*==============================================
==	���A���̃|�P�����p�����[�^�ăZ�b�g
==============================================*/
static void recover_pointpoke_status(void)
{
	if(IconCatchFlg){// || get_point_monsno() != NON_POKE){
		set_status_param((void*)POKETMP_ADDR, PARAMTYPE_MINE);
	}else{
		update_pointpoke_status();
//		RecoverBox
//		set_status_param(NULL, PARAMTYPE_NULL);
	}
}
/*==============================================
==	�|�P�����p�����[�^�p�f�[�^�Z�b�g
==	param_add  �p�����[�^�\���̃A�h���X
==	type       �p�����[�^�^�C�v�ienum PARAM_TYPE�j
==============================================*/
static void set_status_param(void *param_add, u8 type)
{
	u8 sex = 0;
	Work->status_itemno = 0;

	if(type == PARAMTYPE_MINE){
		PokemonParam *param = (PokemonParam*)param_add;

		Work->status_monsno = PokeParaGet(param, ID_monsno_egg);
		if(Work->status_monsno != NON_POKE){
			Work->status_tamago_flg = PokeParaGet(param, ID_tamago_flag);
			PokeParaGet(param, ID_nickname, Work->status_pokename);
			NickNameCutOff(Work->status_pokename);
			Work->status_lv = PokeParaGet(param, ID_level);
			Work->status_mark = PokeParaGet(param, ID_mark);
			Work->status_rnd = PokeParaGet(param, ID_personal_rnd);
			Work->status_pal_addr = (void*)PokePalGet(param);
			sex = PokeSexGet(param);
			Work->status_itemno = PokeParaGet(param, ID_item);
		}
	}else if(type == PARAMTYPE_TRAY){
		PokemonPasoParam *param = (PokemonPasoParam*)param_add;

		Work->status_monsno = PokePasoParaGet(param, ID_monsno_egg);

		if(Work->status_monsno != NON_POKE){
			u32 id;

			id = PokePasoParaGet(param, ID_id_no);
			Work->status_tamago_flg = PokePasoParaGet(param, ID_tamago_flag);
			PokePasoParaGet(param, ID_nickname, Work->status_pokename);
			NickNameCutOff(Work->status_pokename);
			Work->status_lv = PokePasoLevelCalc(param);
			Work->status_mark = PokePasoParaGet(param, ID_mark);
			Work->status_rnd = PokePasoParaGet(param, ID_personal_rnd);
			Work->status_pal_addr = (void*)PokePalGetPara(
				Work->status_monsno, id, Work->status_rnd);
			sex = PokeSexGetMonsNo(Work->status_monsno, Work->status_rnd);
			Work->status_itemno = PokePasoParaGet(param, ID_item);
		}

	}else{
		Work->status_monsno = NON_POKE;
	}


	if(Work->status_monsno == NON_POKE){
		PM_strclear(Work->status_pokename, spc_, 5);
		PM_strclear(Work->status_str[0], spc_, 8);
		PM_strclear(Work->status_str[1], spc_, 8);
		PM_strclear(Work->status_str[2], spc_, 8);
	}else if(Work->status_tamago_flg){

//		PM_strcpy(Work->status_pokename, str_tamago);
		PM_strcpy_clear(Work->status_str[0], Work->status_pokename, spc_, 8);
		PM_strclear(Work->status_str[1], spc_, 8);
		PM_strclear(Work->status_str[2], spc_, 8);

	}else{
		u8 *cp;

		// �j�h�������E���͐��ʕ\�����Ȃ�
		if(Work->status_monsno == MONSNO_NIDORAN_F
		|| Work->status_monsno == MONSNO_NIDORAN_M
		){
			sex = MONS_UNKNOWN;
		}

		cp = PM_strcpy_clear(Work->status_str[0], Work->status_pokename, spc_, 5);
		PM_NumMsgSet(cp, Work->status_lv, NUM_MODE_SPACE, 3);

		cp = Work->status_str[1];
		*cp++ = spc_;
		*cp++ = sura_;
		cp = PM_strcpy_clear(cp, PokeNameData[Work->status_monsno], spc_, 5);
		*cp++ = CTRL_;
		*cp++ = C_PAL_;

		switch(sex){
		case MONS_MALE :   *cp++ = MALE_PAL/16;   *cp++ = osu_; break;
		case MONS_FEMALE : *cp++ = FEMALE_PAL/16; *cp++ = mesu_; break;
		default:           *cp++ = STATUS_PAL/16; *cp++ = spc_; break;
		}
		*cp++ = CTRL_;
		*cp++ = C_PAL_;
		*cp++ = STATUS_PAL/16;
		*cp = EOM_;

		if(Work->status_itemno){
			PM_strcpy_clear(Work->status_str[2], ItemNameGet(Work->status_itemno), spc_, 8);
		}else{
			PM_strclear(Work->status_str[2], spc_, 8);
		}

	}
}
/**********************************************************************/
/*  �L�[����                                                          */
/**********************************************************************/
/*==============================================
==	�g���C���[�h�L�[����
==	return:  �J�[�\������
==============================================*/
static u8 keyread_tray(void)
{
	u8 move;
	s8 nextmode, nextpos;

	do {
		nextmode = CursorMode;
		nextpos = CursorPos;
		Work->cursor_vloop = 0;
		Work->cursor_hloop = 0;
		Work->cursor_vflip = 0;

		if(sys.Repeat&U_KEY){
			move = ACT_CURSOR_MOVE;
			if(CursorPos >= YOKO_KAZU){
				nextpos -= YOKO_KAZU;
			}else{
				nextmode = CURSORMODE_MENU;
				nextpos = 0;
			}
			break;
		}

		if(sys.Repeat&D_KEY){
			move = ACT_CURSOR_MOVE;
			nextpos += YOKO_KAZU;
			if(nextpos >= BOX_PM_KAZU){
				nextmode = CURSORMODE_TAG;
				nextpos -= BOX_PM_KAZU;
				nextpos /= (YOKO_KAZU/2);
				Work->cursor_vloop = 1;
				Work->cursor_vflip = 1;
			}
			break;
		}

		if(sys.Repeat&L_KEY){
			move = ACT_CURSOR_MOVE;
			if(CursorPos % YOKO_KAZU){
				nextpos--;
			}else{
				Work->cursor_hloop = -1;
				nextpos += (YOKO_KAZU-1);
			}
			break;
		}

		if(sys.Repeat&R_KEY){
			move = ACT_CURSOR_MOVE;
			if((CursorPos+1) % YOKO_KAZU){
				nextpos++;
			}else{
				Work->cursor_hloop = 1;
				nextpos -= (YOKO_KAZU-1);
			}
			break;
		}

		if(sys.Trg & START_BUTTON)
		{
			move = ACT_CURSOR_MOVE;
			nextmode = CURSORMODE_MENU;
			nextpos = 0;
			break;
		}

		if(sys.Trg & A_BUTTON){
			if(initmenu_poke()){
				if(!CursorQuickFlg)
				{
					return ACT_POKEMENU;
				}
				else
				{
					switch(BoxMenuCheck(0))
					{
						case MENU_AZUKERU:   return ACT_DIRECT_AZUKERU;
						case MENU_TURETEIKU: return ACT_DIRECT_TURETEIKU;
						case MENU_TUKAMU:    return ACT_DIRECT_TUKAMU;
						case MENU_IREKAERU:  return ACT_DIRECT_IREKAERU;
						case MENU_OKU:       return ACT_DIRECT_OKU;
					}
				}
			}
		}

		if(sys.Trg & B_BUTTON){
			return ACT_ISCONTINUE;
		}

		// LR��Ӱ��
		if(MyData.key_config == CNT_MD_LR)
		{
			if(sys.Cont&L_BUTTON){
				return ACT_SCROLL_PREV;
			}
			if(sys.Cont&R_BUTTON){
				return ACT_SCROLL_NEXT;
			}
		}

		if(sys.Trg & SELECT_BUTTON)
		{
			quickmode_switch();
			return ACT_NOTHING;
		}

		move = ACT_NOTHING;

	}while(0);

	if(move != ACT_NOTHING){
		pre_move(nextmode, nextpos);
	}

	return move;
}
/*==============================================
==	�Ă������[�h�L�[����
==	return:  �J�[�\������
==============================================*/
static u8 keyread_mine(void)
{
	u8 move, modoru_flg;
	s8 nextpos, nextmode;

	do {
		nextmode = CursorMode;
		nextpos = CursorPos;
		Work->cursor_hloop = 0;
		Work->cursor_vloop = 0;
		Work->cursor_vflip = 0;

		modoru_flg = 0;
		move = ACT_NOTHING;

		if(sys.Repeat&U_KEY){
			if(--nextpos < 0){
				nextpos = MINEBOX_CURSOR_MAXPOS;
			}
			if(nextpos != CursorPos){
				move = ACT_CURSOR_MOVE;
			}
			break;
		}

		if(sys.Repeat&D_KEY){
			if(++nextpos > MINEBOX_CURSOR_MAXPOS){
				nextpos = 0;
			}
			if(nextpos != CursorPos){
				move = ACT_CURSOR_MOVE;
			}
			break;
		}

		if(sys.Repeat&L_KEY){
			if(CursorPos){
				move = ACT_CURSOR_MOVE;
				Work->mine_pos_save = CursorPos;
				nextpos = 0;
				break;
			}
		}

		if(sys.Repeat&R_KEY){
			if(CursorPos == 0){
				move = ACT_CURSOR_MOVE;
				nextpos = Work->mine_pos_save;
			}else{
				move = ACT_MINEBOX_OFF;
				nextmode = CURSORMODE_TRAY;
				nextpos = 0;
			}
			break;
		}

		if(sys.Trg & A_BUTTON){
			if(CursorPos == MINEBOX_CURSOR_MAXPOS){
				if(Work->mode == BOX_MODE_AZUKERU){
					return ACT_BOXOFF;
				}
				modoru_flg = 1;
			}else{
				if(initmenu_poke()){
					if(!CursorQuickFlg){
						return ACT_POKEMENU;
					}else{
						switch(BoxMenuCheck(0)){
						case MENU_AZUKERU:   return ACT_DIRECT_AZUKERU;
						case MENU_TURETEIKU: return ACT_DIRECT_TURETEIKU;
						case MENU_TUKAMU:    return ACT_DIRECT_TUKAMU;
						case MENU_IREKAERU:  return ACT_DIRECT_IREKAERU;
						case MENU_OKU:       return ACT_DIRECT_OKU;
						}
					}
				}
			}
		}

		if(sys.Trg & B_BUTTON){
			if(Work->mode == BOX_MODE_AZUKERU){
				return ACT_ISCONTINUE;
			}
			modoru_flg = 1;
		}

		if(modoru_flg){
			move = ACT_MINEBOX_OFF;
			nextmode = CURSORMODE_TRAY;
			nextpos = 0;
			break;
		}

		if(sys.Trg & SELECT_BUTTON)
		{
			quickmode_switch();
			return ACT_NOTHING;
		}

	}while(0);

	if(move != ACT_NOTHING){
		pre_move(nextmode, nextpos);
	}

	return move;
}
/*==============================================
==	���j���[���[�h�L�[����
==	return:  �J�[�\������
==============================================*/
static u8 keyread_menu(void)
{
	u8 move;
	s8 nextpos, nextmode;

	do {
		nextmode = CursorMode;
		nextpos = CursorPos;
		Work->cursor_hloop = 0;
		Work->cursor_vloop = 0;
		Work->cursor_vflip = 0;

		if(sys.Repeat&U_KEY){
			move = ACT_CURSOR_MOVE;
			nextmode = CURSORMODE_TAG;
			nextpos = 0;
			Work->cursor_vflip = 1;
			break;
		}

		if(sys.Repeat&D_KEY){
			move = ACT_CURSOR_MOVE;
			nextmode = CURSORMODE_TRAY;
			nextpos = 2;
			break;
		}

		if(sys.Cont&L_KEY){
			return ACT_SCROLL_PREV;
		}

		if(sys.Cont&R_KEY){
			return ACT_SCROLL_NEXT;
		}
		// LR��Ӱ��
		if(MyData.key_config == CNT_MD_LR)
		{
			if(sys.Cont&L_BUTTON){
				return ACT_SCROLL_PREV;
			}
			if(sys.Cont&R_BUTTON){
				return ACT_SCROLL_NEXT;
			}
		}


		if(sys.Trg & A_BUTTON){
			BoxArrowMove(0);
			initmenu_tray();
			return ACT_TRAYMENU;
		}

		if(sys.Trg & B_BUTTON){
			return ACT_ISCONTINUE;
		}

		if(sys.Trg & SELECT_BUTTON)
		{
			quickmode_switch();
			return ACT_NOTHING;
		}

		move = ACT_NOTHING;

	}while(0);

	if(move != ACT_NOTHING){
		if(nextmode != CURSORMODE_MENU)
		{
			BoxArrowMove(0);
		}
		pre_move(nextmode, nextpos);
	}

	return move;
}
/*==============================================
==	�^�O���[�h�L�[����
==	return:  �J�[�\������
==============================================*/
static u8 keyread_tag(void)
{
	u8 move;
	s8 nextpos, nextmode;

	do {
		nextmode = CursorMode;
		nextpos = CursorPos;
		Work->cursor_hloop = 0;
		Work->cursor_vloop = 0;
		Work->cursor_vflip = 0;


		if(sys.Repeat&U_KEY){
			move = ACT_CURSOR_MOVE;
			nextmode = CURSORMODE_TRAY;
			Work->cursor_vloop = -1;
			if(CursorPos == 0){
				nextpos = 24;
			}else{
				nextpos = 29;
			}
			Work->cursor_vflip = 1;
			break;
		}

		if(sys.Repeat&(D_KEY|START_BUTTON)){
			move = ACT_CURSOR_MOVE;
			nextmode = CURSORMODE_MENU;
			nextpos = 0;
			Work->cursor_vflip = 1;
			break;
		}

		if(sys.Repeat&L_KEY){
			move = ACT_CURSOR_MOVE;
			if(--nextpos < 0){
				Work->cursor_hloop = -1;
				nextpos = 1;
			}
			break;
		}

		if(sys.Repeat&R_KEY){
			move = ACT_CURSOR_MOVE;
			if(++nextpos > 1){
				Work->cursor_hloop = 1;
				nextpos = 0;
			}
			break;
		}

		if(sys.Trg & A_BUTTON){
			return (nextpos==0)? ACT_MINEBOX_ON : ACT_BOXOFF;
			break;
		}

		if(sys.Trg & B_BUTTON){
			return ACT_ISCONTINUE;
		}

		if(sys.Trg & SELECT_BUTTON)
		{
			quickmode_switch();
			return ACT_NOTHING;
		}

		move = ACT_NOTHING;

	}while(0);

	if(move != ACT_NOTHING){
		pre_move(nextmode, nextpos);
	}

	return move;
}
/*==============================================
==	�L�[���胁�C��
==	return:  ���삠�� 1,  ����Ȃ� 0
==============================================*/
u8 BoxCursorKeyRead(void)
{
	static const  struct {
		u8 (*func)(void);
		s8  mode;
	}functbl[] = {
		{ keyread_tray, CURSORMODE_TRAY },
		{ keyread_mine, CURSORMODE_MINE },
		{ keyread_menu, CURSORMODE_MENU },
		{ keyread_tag,  CURSORMODE_TAG  },
		{ NULL, 0 },
	};
	u16 i;

	for(i = 0; functbl[i].func != NULL; i++){
		if(functbl[i].mode == CursorMode){
			u8 ret = (*(functbl[i].func))();
			return ret;
		}
	}
	return 0;
}
/**********************************************************************/
/*  ���j���[����                                                      */
/**********************************************************************/
/*==============================================
==	�g���C���j���[������
==============================================*/
static void initmenu_tray(void)
{
	BoxMenuInit();
	BoxMenuAdd(MENU_BOXCHANGE);
	BoxMenuAdd(MENU_WALLPAPER);
	BoxMenuAdd(MENU_BOXNAME);
	BoxMenuAdd(MENU_YAMERU);
}
#ifdef PM_DEBUG
/*==============================================
==	�f�o�b�O�p�|�P�����Z�b�g���[�h
==============================================*/
static u8 initmenu_debug_pokeset(void)
{
	if(NON_POKE == get_point_monsno()){
		return 0;
	}
	BoxMenuInit();
	BoxMenuAdd(MENU_DEBUG_POKESET);
	BoxMenuAdd(MENU_YOUSUMIRU);

	return 1;
}
#endif
/*==============================================
==	�|�P�������j���[�i�g���C���[�h�j������
==	return:  ���j���[�\���� 1,  �s�� 0
==============================================*/
static u8 initmenu_poke(void)
{
	u16 monsno;

	// �f�o�b�O�p�|�P�����Z�b�g���[�h�̓���
#ifdef PM_DEBUG
	if(BoxDebugMode){
		return initmenu_debug_pokeset();
	}
#endif

	BoxMenuInit();

	// �P�Ԗڂ̃��j���[�͓��샂�[�h�E�J�[�\����ԁE�ʒu�ɂ���ĈقȂ�
	monsno = get_point_monsno();
	switch(Work->mode){
	case BOX_MODE_AZUKERU:
		if(monsno != NON_POKE){
			BoxMenuAdd(MENU_AZUKERU);
		}else{
			return 0;
		}
		break;

	case BOX_MODE_TURETEIKU:
		if(monsno != NON_POKE){
			BoxMenuAdd(MENU_TURETEIKU);
		}else{
			return 0;
		}
		break;

	case BOX_MODE_SEIRI:
		if(IconCatchFlg){
			if(monsno != NON_POKE){
				BoxMenuAdd(MENU_IREKAERU);
			}else{
				BoxMenuAdd(MENU_OKU);
			}
		}else{
			if(monsno != NON_POKE){
				BoxMenuAdd(MENU_TUKAMU);
			}else{
				return 0;
			}
		}
		break;

	default:
		return 0;
	}

	// �Q�Ԗڂ̃��j���[�́g�悤�����݂�h�Œ�
	BoxMenuAdd(MENU_YOUSUMIRU);

	// �g�����肷��h���[�h�Ȃ�A�J�[�\���ʒu�ɉ�����
	// �g��Ă����h�܂��́g��������h��ǉ�
	if(Work->mode == BOX_MODE_SEIRI){
		if(CursorMode == CURSORMODE_TRAY){
			BoxMenuAdd(MENU_TURETEIKU);
		}else{
			BoxMenuAdd(MENU_AZUKERU);
		}
	}

	// �ȉ��A�Œ�
	BoxMenuAdd(MENU_MARKING);
	BoxMenuAdd(MENU_NIGASU);
	BoxMenuAdd(MENU_YAMERU);

	return 1;
}
/**********************************************************************/
/*  ����֐�                                                          */
/**********************************************************************/
/*==============================================
==	�J�[�\���̉e
==============================================*/
static void shadow_actmove(actWork *act)
{
	act->x = Work->cursor_actor->x;
	act->y = Work->cursor_actor->y + CURSOR_SHADOW_DIFF;
}
/**********************************************************************/
/*  �f�o�b�O�p�|�P�����f�[�^�Z�b�g                                    */
/**********************************************************************/
void BoxDebugPokeSet(PokemonPasoParam *ppp)
{
	if(CursorMode == CURSORMODE_MINE){
		*ppp = PokeParaMine[CursorPos].PPP;
	}else{
		u8 current = GetCurrentBox();
		*ppp = BoxData.PPPBox[current][CursorPos];
	}
}
/**********************************************************************/
/*  �A�N�^�[�o�^�E�폜                                                */
/**********************************************************************/
static const actHeader CursorHeader;
static const actHeader ShadowHeader;

/*==============================================
==	�A�N�^�[�o�^
==============================================*/
static void add_cursor_actor(void)
{
	CellData cell[] = {
		{ cursor_Character, sizeof(cursor_Character), CELLID_CURSOR },
		{ shadow_Character, sizeof(shadow_Character), CELLID_SHADOW },
		{ CELL_END, 0, 0 },
	};
	PalData pal[] = {
		{  cursor_Palette, PALID_CURSOR },
		{  quick_cursor_Palette, PALID_QUICK_CURSOR },
		{  OBJPAL_END, 0 },
	};

	u16 no;
	s16 x, y;
	u8 actpri, oampri;

	CellSets(cell);
	ObjPalSets(pal);

	Work->cursor_palno[0] = ObjPalNumGet(PALID_CURSOR);
	Work->cursor_palno[1] = ObjPalNumGet(PALID_QUICK_CURSOR);

	get_cursor_pos(CursorMode, CursorPos, &x, &y);

	no = AddActor(&CursorHeader, x, y, ACTPRI_CURSOR);
	if(no != ACT_MAX){
		Work->cursor_actor = &ActWork[no];
		Work->cursor_actor->oamData.Pltt = Work->cursor_palno[CursorQuickFlg];
		if(IconCatchFlg){
			ActAnmChg(Work->cursor_actor, CURSORANM_CATCH);
		}
	}else{
		Work->cursor_actor = NULL;
	}

	if(CursorMode == CURSORMODE_MINE){
		actpri = ACTPRI_MINESHADOW;
		oampri = 1;
	}else{
		actpri = ACTPRI_TRAYSHADOW;
		oampri = 2;
	}
	no = AddActor(&ShadowHeader, 0, 0, actpri);
	if(no != ACT_MAX){
		Work->shadow_actor = &ActWork[no];
		Work->shadow_actor->oamData.Priority = oampri;
		if(CursorMode != CURSORMODE_TRAY){
			Work->shadow_actor->banish = 1;
		}
	}else{
		Work->shadow_actor = NULL;
	}
}

/*====================================*/
/*  �A�N�^�[�p�f�[�^                  */
/*====================================*/
static const ActOamData CursorOam = {
	0,		//u32 VPos:8;             // �x���W
	0,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	0,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;    // �A�t�B���ϊ��p�����[�^No & H,VFlip
	2,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	1,      //u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
static const ActOamData ShadowOam = {
	0,		//u32 VPos:8;             // �x���W
	0,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	0,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;    // �A�t�B���ϊ��p�����[�^No & H,VFlip
	1,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	2,      //u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
static const actAnm CursorAnm0[] = {
	{  0, 30, 0, 0 },
	{ 16, 30, 0, 0 },
	{ACT_ANMLOOP,0,0,0},
};
static const actAnm CursorAnm1[] = {
	{ 0, 5, 0, 0 },
	{ACT_ANMEND,0,0,0},
};
static const actAnm CursorAnm2[] = {
	{ 32, 5, 0, 0 },
	{ACT_ANMEND,0,0,0},
};
static const actAnm CursorAnm3[] = {
	{ 48, 5, 0, 0 },
	{ACT_ANMEND,0,0,0},
};
static const actAnm * const CursorAnmTbl[] = {
	CursorAnm0,
	CursorAnm1,
	CursorAnm2,
	CursorAnm3,
};
static const actHeader CursorHeader = {
	CELLID_CURSOR,
	PALID_CURSOR,
	&CursorOam,
	CursorAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	DummyActMove,
};
static const actHeader ShadowHeader = {
	CELLID_SHADOW,
	PALID_CURSOR,
	&ShadowOam,
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	shadow_actmove,
};
/**********************************************************************/
/*  �ʏ탂�[�h�|�N�C�b�N���[�h�ؑւ�                                  */
/**********************************************************************/
static void quickmode_switch(void)
{
	CursorQuickFlg = !CursorQuickFlg;
	Work->cursor_actor->oamData.Pltt = Work->cursor_palno[CursorQuickFlg];
}

