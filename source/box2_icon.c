/**********************************************************************/
/*  �p�\�R���{�b�N�X������  -�A�C�R��-                                */
/**********************************************************************/
#include "common.h"
#include "intr.h"
#include "decord.h"
#include "actanm.h"
#include "actor.h"
#include "palanm.h"
#include "calctool.h"

#include "box2.h"

/**********************************************************************/
/*  �萔                                                              */
/**********************************************************************/
/*-- �n�`�l�v���C�I���e�B --*/
#define CATCHICON_PRI   (1)
#define MINEICON_PRI    (1)
#define TRAYICON_PRI    (2)

/*-- --*/
#define TRAYICON_XMIN  (68)
#define TRAYICON_XMAX  (252)

#define CALC_TRAYICON_ACTPRI(x)   (ACTPRI_TRAYICON+(YOKO_KAZU-1-(x)))

/*-- �A�C�R���A�N�^�[�̃��[�N --*/
// 0�Ԃ͉摜������ް�Ɏg�p����i���������s�I�j
#define ICONWK_CHAR_ID     (0)

// ��۰�
enum ICONWK_SCROLL {
ICONWK_SCR_SYNC  = 1,
ICONWK_SCR_SPEED,
ICONWK_SCR_LASTX,
ICONWK_SCR_WAIT,
ICONWK_SCR_WORK,
};

// �莝���󂫋l��
enum ICONWK_TSUME {
ICONWK_TSUME_POS  = 1,
ICONWK_TSUME_X,
ICONWK_TSUME_Y,
ICONWK_TSUME_AX,
ICONWK_TSUME_AY,
ICONWK_TSUME_SYNC,
};



/**********************************************************************/
/*  �O���[�o���ϐ�                                                    */
/**********************************************************************/
extern const PalData PokeIconPalData[];
extern const u8 PokeIconPalette[];
extern const u8 p_icon_1_Character[];
static const actAffAnm * const IconNigasuAffTbl[];

/**********************************************************************/
/*  �v���g�^�C�v                                                      */
/**********************************************************************/
static actWork* add_icon(u16 monsno, u32 personal_rnd, s16 x, s16 y, u8 oampri, u8 actpri);
static void del_icon(actWork *act);

static void trayicon_scrollout_move(actWork *act);
static void trayicon_scrollin_move(actWork *act);
static void catch_icon_move(actWork * act);

/**********************************************************************/
/*  �������E���A                                                      */
/**********************************************************************/
/*==============================================
==	������
==============================================*/
void InitBox2Icon(void)
{
	u16 i;

	PokeIconPaletteAllSet();

	for(i = 0; i < ICON_CELLMAN_MAX; i++){
		Work->icon_cell_cnt[i] = 0;
	}

	for(i = 0; i < ICON_CELLMAN_MAX; i++){
		Work->icon_cell_use[i] = 0;
	}

	for(i = 0; i < TEMOTI_POKEMAX; i++){
		Work->mine_icon[i] = NULL;
	}

	for(i = 0; i < BOX_PM_KAZU; i++){
		Work->tray_icon[i] = NULL;
	}

	Work->catch_icon = NULL;
	Work->scricon_req_cnt = 0;
}
/*==============================================
==	����ł���A�C�R���̕��A
==============================================*/
void RecoverBoxCatchIcon(void)
{
	u32 rnd    = PokeParaGet(&(Work->PP), ID_personal_rnd);
	u16 monsno = PokeParaGet(&(Work->PP), ID_monsno_egg);

	Work->catch_icon = add_icon(monsno, rnd, 0, 0, CATCHICON_PRI, ACTPRI_CATCHICON);
	Work->catch_icon->move = catch_icon_move;
}
/**********************************************************************/
/*  �g���C�A�C�R������                                                */
/**********************************************************************/
/*==============================================
==	�g���C�A�C�R���S�o�^
==	box  �{�b�N�X�i���o�[
==============================================*/
void AddTrayIcon(u8 box)
{
	PokemonPasoParam *ppp;
	u32 rnd;
	u16 x, y, c, no;


	ppp = BoxData.PPPBox[box];
	c = 0;

	for(y = 0; y < TATE_KAZU; y++){
		for(x = 0; x < YOKO_KAZU; x++){
			no = PokePasoParaGet(ppp, ID_monsno_egg);
			if(no != NON_POKE){
				rnd = PokePasoParaGet(ppp, ID_personal_rnd);
				Work->tray_icon[c] = add_icon(no, rnd, TRAYICON_XORG+(x*ICON_WIDTH), TRAYICON_YORG+(y*ICON_HEIGHT),
					TRAYICON_PRI, CALC_TRAYICON_ACTPRI(x));
			}else{
				Work->tray_icon[c] = NULL;
			}
			ppp++;
			c++;
		}
	}
}
/*==============================================
==	�g���C�A�C�R���X�V�i�P�ӏ��j
==	pos   �X�V�̂������ꏊ
==============================================*/
void BoxIcon_TrayUpdate(u8 pos)
{
	u16 current, monsno;

	current = GetCurrentBox();
	monsno = PokePasoParaGet(&(BoxData.PPPBox[current][pos]), ID_monsno_egg);
	if(monsno != NON_POKE){
		u32 rnd;
		s16 x, y;
		x = TRAYICON_XORG + (pos % YOKO_KAZU) * ICON_WIDTH;
		y = TRAYICON_YORG + (pos / YOKO_KAZU) * ICON_HEIGHT;
		rnd = PokePasoParaGet(&(BoxData.PPPBox[current][pos]), ID_personal_rnd);
		Work->tray_icon[pos] = add_icon(monsno, rnd, x, y, TRAYICON_PRI, CALC_TRAYICON_ACTPRI(pos%YOKO_KAZU));
	}
}
/*==============================================
==	���݂̃A�C�R�����X�N���[���A�E�g������
==	s16  speed  �X�N���[�����鑬�x
==============================================*/
void TrayIconScrollOut(s16 speed)
{
	u16 i;
	for(i = 0; i < BOX_PM_KAZU; i++){
		if(Work->tray_icon[i] != NULL){
			Work->tray_icon[i]->work[ICONWK_SCR_SPEED] = speed; // �ړ����x
			Work->tray_icon[i]->work[ICONWK_SCR_WAIT] = 1;  // �ړ��J�n�܂ł̳���
			Work->tray_icon[i]->move = trayicon_scrollout_move;
		}
	}
}
/*==============================================
==	�X�N���[���C������֐�
==============================================*/
static void trayicon_scrollin_move(actWork *act)
{
	// 1:�ړ�sync��  2:�ړ����x  3:�ŏI�ʒu
	
	if(act->work[ICONWK_SCR_SYNC]){
		act->work[ICONWK_SCR_SYNC]--;
		act->x += act->work[ICONWK_SCR_SPEED];
	}else{
		Work->icon_scroll_actcnt--;
		act->x = act->work[ICONWK_SCR_LASTX];
		act->move = DummyActMove;
	}
}
/*==============================================
==	�X�N���[���A�E�g����֐�
==============================================*/
static void trayicon_scrollout_move(actWork *act)
{
	if(act->work[ICONWK_SCR_WAIT]){
		act->work[ICONWK_SCR_WAIT]--;
		return;
	}
	act->x += act->work[ICONWK_SCR_SPEED];
	act->work[ICONWK_SCR_WORK] = act->x + act->dx;
	if(act->work[ICONWK_SCR_WORK] <= TRAYICON_XMIN || act->work[ICONWK_SCR_WORK] >= TRAYICON_XMAX){
		act->move = DummyActMove;
	}
}

/*==============================================
==	�X�N���[�����̃A�C�R�����A�c�P���C������
==	line  (��:0 �`  �E: YOKO_KAZU-1�j
==============================================*/
static void scroll_icon_del(u8 line)
{
	u16 i;
	for(i = 0; i < TATE_KAZU; i++)
	{
		if(Work->tray_icon[line] != NULL){
			del_icon(Work->tray_icon[line]);
			Work->tray_icon[line] = NULL;
		}
		line += YOKO_KAZU;
	}
}
/*==============================================
== �X�N���[�����̃A�C�R�����A�c�P���C���ǉ�
== line   �����C���ڂ��H
== timer  ��sync�Œ�ʒu�ɓ��B���邩�H
== speed  �ړ����x
==
== return:�o�^�������ݱ����̐�
==============================================*/
static u8 scroll_icon_add(u8 line, u16 timer, s16 speed)
{
	u32 rnd;
	u16 i, monsno;
	u16 last_x, xpos, ypos;
	u8  cnt, actpri;

	//  ���݈ʒu�v�Z
	ypos = TRAYICON_YORG;
	last_x = TRAYICON_XORG + (line * ICON_WIDTH);
	xpos = last_x - ((timer+1) * speed);
	actpri = CALC_TRAYICON_ACTPRI(line);

	cnt = 0;
	for(i = 0; i < TATE_KAZU; i++){
		monsno = PokePasoParaGet(&(BoxData.PPPBox[Work->icon_scroll_box][line]), ID_monsno_egg);
		if(monsno != NON_POKE)
		{
			rnd = PokePasoParaGet(&(BoxData.PPPBox[Work->icon_scroll_box][line]), ID_personal_rnd);
			Work->tray_icon[line] = add_icon(monsno, rnd, xpos, ypos, TRAYICON_PRI, actpri);
			if(Work->tray_icon[line] != NULL){
				// 1:�ړ�sync��  2:���x  3:�ŏI�ʒu
				Work->tray_icon[line]->work[ICONWK_SCR_SYNC] = timer;
				Work->tray_icon[line]->work[ICONWK_SCR_SPEED] = speed;
				Work->tray_icon[line]->work[ICONWK_SCR_LASTX] = last_x;
				Work->tray_icon[line]->move = trayicon_scrollin_move;
				cnt++;
			}
		}
		line += YOKO_KAZU;
		ypos += ICON_HEIGHT;
	}

	return cnt;
}
/*==============================================
==	�A�C�R���X�N���[��������
==	box    ���Ԃ��ޯ���փX�N���[�����邩�H
==	vec    �X�N���[�������i�E����:-1  ������:1)
==============================================*/
void TrayIconScrollInit(u8 box, s8 vec)
{
	Work->icon_scroll_seq = 0;
	Work->icon_scroll_box = box;
	Work->icon_scroll_vec = vec;
	Work->icon_scroll_timer = TRAY_SCROLL_SYNC;
	Work->icon_scroll_speed = -(vec * TRAY_SCROLL_SPEED);
	Work->icon_scroll_actcnt = 0;

	if(vec > 0)
	{
		Work->icon_scroll_line = 0;
	}
	else
	{
		Work->icon_scroll_line = (YOKO_KAZU-1);
	}

	Work->icon_scroll_xpos = TRAYICON_XORG + (Work->icon_scroll_line * ICON_WIDTH);

	TrayIconScrollOut(Work->icon_scroll_speed);
}
/*==============================================
==	�A�C�R���X�N���[���I���҂�
==	return  1:�p����  0:�I��
==============================================*/
u8  TrayIconScrollWait(void)
{
	if(Work->icon_scroll_timer){
		Work->icon_scroll_timer--;
	}

	switch(Work->icon_scroll_seq){
	// �X�N���[�������ȏ�i�񂾂�P���C������
	case 0:
		Work->icon_scroll_xpos += Work->icon_scroll_speed;
		if(Work->icon_scroll_xpos <= 64 || Work->icon_scroll_xpos >= 252)
		{
			scroll_icon_del(Work->icon_scroll_line);
			Work->icon_scroll_xpos += (Work->icon_scroll_vec * ICON_WIDTH);
			Work->icon_scroll_seq++;
		}
		break;

	// �����̌�A�P���C���ǉ�
	case 1:
		Work->icon_scroll_xpos += Work->icon_scroll_speed;
		Work->icon_scroll_actcnt += scroll_icon_add(Work->icon_scroll_line, Work->icon_scroll_timer, Work->icon_scroll_speed);

		// �ŏI���C�����H
		if((Work->icon_scroll_vec > 0 && Work->icon_scroll_line == (YOKO_KAZU-1))
		|| (Work->icon_scroll_vec < 0 && Work->icon_scroll_line == 0) )
		{
			Work->icon_scroll_seq++;
		}
		else
		{
			Work->icon_scroll_line += Work->icon_scroll_vec;
			Work->icon_scroll_seq = 0;
		}
		break;

	// �S�A�C�R�����X�N���[������܂ő҂�
	case 2:
		if(Work->icon_scroll_actcnt == 0){
			Work->icon_scroll_timer++;
			return 0;
		}
		break;

	default:
		return 0;
	}

	return 1;
}












/**********************************************************************/
/*  �Ă����A�C�R������                                                */
/**********************************************************************/
/*==============================================
==	�Ă����A�C�R���o�^
==	disp  �ŏ�����\������          1
==	      �ŏ��̓X�N���[���O�̈ʒu  0
==============================================*/
#define MINE_MOVE_LEN  (MINEBOX_SCROLL_SYNC*8)

void AddMineIcon(u8 disp)
{
	u32 rnd;
	u16 i, max, monsno;


	monsno = PokeParaGet(PokeParaMine, ID_monsno_egg);
	rnd = PokeParaGet(PokeParaMine, ID_personal_rnd);
	Work->mine_icon[0] = add_icon(monsno, rnd, TOPICON_XPOS, TOPICON_YPOS, 1, ACTPRI_MINEICON);

	max = 1;
	for(i = 1; i < TEMOTI_POKEMAX; i++){
		monsno = PokeParaGet(PokeParaMine + i, ID_monsno_egg);
		if(monsno != NON_POKE){
			rnd = PokeParaGet(PokeParaMine + i, ID_personal_rnd);
			Work->mine_icon[i] = add_icon(monsno, rnd, MINEICON_XORG, MINEICON_YORG+(i-1)*ICON_HEIGHT, 1, ACTPRI_MINEICON);
			max++;
		}else{
			Work->mine_icon[i] = NULL;
		}
	}

	if(!disp){
		for(i = 0; i < max; i++){
			Work->mine_icon[i]->y -= MINE_MOVE_LEN;
			Work->mine_icon[i]->banish = 1;
		}
	}
}
/*==============================================
==	�Ă����A�C�R���󂫋l�߁i�������j
==============================================*/
static void set_tsume_icon_move(actWork *act, u16 pos);
static void tsume_icon_move(actWork *act);

void BoxIcon_TumeInit(void)
{
	u16 i, p;

	Work->tsume_move_icons = 0;
	for(i = 0, p = 0; i < TEMOTI_POKEMAX; i++){
		if(Work->mine_icon[i] != NULL){
			if(i != p){
				set_tsume_icon_move(Work->mine_icon[i], p);
				Work->mine_icon[i] = NULL;
				Work->tsume_move_icons++;
			}
			p++;
		}
	}
}
/*==============================================
==	�Ă����A�C�R���󂫋l�߁i�I���҂��j
==	return: �I��  0,  �p����  1
==============================================*/
u8 BoxIcon_TumeWait(void)
{
	return Work->tsume_move_icons;
}
/*==============================================
==	�Ă����A�C�R���󂫋l�ߓ���ݒ�
==	act  �A�C�R���A�N�^�[
==	pos  �ړ���ԍ�
==============================================*/
#define TSUME_ICON_SYNC  (8)
static void set_tsume_icon_move(actWork *act, u16 pos)
{
	s16 ex, ey;

	act->work[ICONWK_TSUME_POS] = pos;
	if(pos == 0){
		ex = TOPICON_XPOS;
		ey = TOPICON_YPOS;
	}else{
		ex = MINEICON_XORG;
		ey = MINEICON_YORG + (pos-1)*ICON_HEIGHT;
	}

	act->work[ICONWK_TSUME_X] = act->x << 3;
	act->work[ICONWK_TSUME_Y] = act->y << 3;

	act->work[ICONWK_TSUME_AX] = ((ex << 3) - act->work[ICONWK_TSUME_X]) / TSUME_ICON_SYNC;
	act->work[ICONWK_TSUME_AY] = ((ey << 3) - act->work[ICONWK_TSUME_Y]) / TSUME_ICON_SYNC;

	act->work[ICONWK_TSUME_SYNC] = TSUME_ICON_SYNC;

	act->move = tsume_icon_move;

}

static void tsume_icon_move(actWork *act)
{
	if(act->work[ICONWK_TSUME_SYNC]){
		act->work[ICONWK_TSUME_X] += act->work[ICONWK_TSUME_AX];
		act->work[ICONWK_TSUME_Y] += act->work[ICONWK_TSUME_AY];
		act->x = (act->work[ICONWK_TSUME_X] >> 3);
		act->y = (act->work[ICONWK_TSUME_Y] >> 3);
		act->work[ICONWK_TSUME_SYNC]--;
	}else{
		if(act->work[ICONWK_TSUME_POS] == 0){
			act->x = TOPICON_XPOS;
			act->y = TOPICON_YPOS;
		}else{
			act->x = MINEICON_XORG;
			act->y = MINEICON_YORG + (act->work[ICONWK_TSUME_POS]-1)*ICON_HEIGHT;
		}
		act->move = DummyActMove;
		Work->mine_icon[act->work[ICONWK_TSUME_POS]] = act;
		Work->tsume_move_icons--;
	}
}

/*==============================================
==	����ł���A�C�R�����폜����
==============================================*/
void BoxIcon_DelCatch(void)
{
	if(Work->catch_icon != NULL){
		del_icon(Work->catch_icon);
		Work->catch_icon = NULL;
	}
}
/*==============================================
==	�Ă����A�C�R���㉺�X�N���[��
==	vector  �ړ���
==============================================*/
void ScrollMineIcon(s16 vector)
{
	u16 i;
	s16 y;
	for(i = 0; i < TEMOTI_POKEMAX; i++){
		if(Work->mine_icon[i] != NULL){
			Work->mine_icon[i]->y += vector;
			y = Work->mine_icon[i]->y + Work->mine_icon[i]->dy + Work->mine_icon[i]->cy;
			if(y < -16 || y > 176){
				Work->mine_icon[i]->banish = 1;
			}else{
				Work->mine_icon[i]->banish = 0;
			}
		}
	}
}
/*==============================================
==	�Ă����A�C�R���폜
==============================================*/
void DelMineIcon(u8 pos)
{
	if(Work->mine_icon[pos] != NULL){
		del_icon(Work->mine_icon[pos]);
		Work->mine_icon[pos] = NULL;
	}
}
void DelMineIconAll(void)
{
	u16 i;
	for(i = 0; i < TEMOTI_POKEMAX; i++){
		if(Work->mine_icon[i] != NULL){
			del_icon(Work->mine_icon[i]);
			Work->mine_icon[i] = NULL;
		}
	}
}
/**********************************************************************/
/*  �A�C�R�����u���ށv�u�����v���̏���                              */
/**********************************************************************/
/*==============================================
==	�w��ʒu�̃A�C�R�����u���ށv
==	icon_type  �A�C�R���^�C�v�ienum ICONTYPE�j
==	pos  �{�b�N�X���̈ʒu
==============================================*/
void BoxIconCatch(u8 icon_type, u8 pos)
{
	if(icon_type == ICONTYPE_MINE){
		Work->catch_icon = Work->mine_icon[pos];
		Work->mine_icon[pos] = NULL;
	}else if(icon_type == ICONTYPE_TRAY){
		Work->catch_icon = Work->tray_icon[pos];
		Work->tray_icon[pos] = NULL;
	}else{
		return;
	}
	Work->catch_icon->move = catch_icon_move;
	Work->catch_icon->oamData.Priority = CATCHICON_PRI;
	Work->catch_icon->pri = ACTPRI_CATCHICON;
}
/*==============================================
==	�w��ʒu�ɃA�C�R�����u�����v
==	box  �{�b�N�X�ԍ��i==BOX_KAZU �Ȃ�莝���j
==	pos  �{�b�N�X���̈ʒu
==============================================*/
void BoxIconPut(u8 box, u8 pos)
{
	if(box == BOX_KAZU){
		Work->mine_icon[pos] = Work->catch_icon;
		Work->mine_icon[pos]->oamData.Priority = MINEICON_PRI;
		Work->mine_icon[pos]->pri = ACTPRI_MINEICON;
	}else{
		Work->tray_icon[pos] = Work->catch_icon;
		Work->tray_icon[pos]->oamData.Priority = TRAYICON_PRI;
		Work->tray_icon[pos]->pri = CALC_TRAYICON_ACTPRI(pos%YOKO_KAZU);
	}
	Work->catch_icon->move = DummyActMove;
	Work->catch_icon = NULL;
}
/*==============================================
==	�u���ꂩ����v������
==	box  �{�b�N�X�ԍ��i==BOX_KAZU �Ȃ�莝���j
==	pos  �{�b�N�X���̈ʒu
==============================================*/
void BoxIconSwapInit(u8 box, u8 pos)
{
	if(box == BOX_KAZU){
		Work->swap_icon = &(Work->mine_icon[pos]);
	}else{
		Work->swap_icon = &(Work->tray_icon[pos]);
	}
	Work->catch_icon->move = DummyActMove;
	Work->icon_swap_prog = 0;
}
/*==============================================
==	�u���ꂩ����v���쁕�I���҂�
==	return: �I�� 0,  �p���� 1
==============================================*/
u8 BoxIconSwapWait(void)
{
	if(Work->icon_swap_prog == 16){ return 0; }

	Work->icon_swap_prog++;
	if(Work->icon_swap_prog & 1){
		(*Work->swap_icon)->y--;
		Work->catch_icon->y++;
	}
	(*Work->swap_icon)->dx = (sin_cos_table[Work->icon_swap_prog*8] / 16);
	Work->catch_icon->dx = -(sin_cos_table[Work->icon_swap_prog*8] / 16);

	if(Work->icon_swap_prog == 8){
		Work->catch_icon->oamData.Priority = (*Work->swap_icon)->oamData.Priority;
		Work->catch_icon->pri = (*Work->swap_icon)->pri;
		(*Work->swap_icon)->oamData.Priority = CATCHICON_PRI;
		(*Work->swap_icon)->pri = ACTPRI_CATCHICON;
	}
	if(Work->icon_swap_prog == 16){
		actWork *tmp = Work->catch_icon;
		Work->catch_icon = (*Work->swap_icon);
		(*Work->swap_icon) = tmp;

		Work->catch_icon->move = catch_icon_move;
		(*Work->swap_icon)->move = DummyActMove;
	}

	return 1;
}
/*==============================================
==	�u�ɂ����v�A�j���J�n
==	icon_type  �A�C�R���^�C�v�ienum ICONTYPE�j
==	pos   �A�C�R���ʒu
==============================================*/
void BoxIcon_NigasuInit(u8 icon_type, u8 pos)
{
	switch(icon_type){
	case ICONTYPE_MINE:  Work->nigasu_icon = &(Work->mine_icon[pos]); break;
	case ICONTYPE_TRAY:  Work->nigasu_icon = &(Work->tray_icon[pos]); break;
	case ICONTYPE_CATCH: Work->nigasu_icon = &(Work->catch_icon); break;
	default: return;
	}

	if((*Work->nigasu_icon) != NULL){
		AffineNumSet(*Work->nigasu_icon);
		(*Work->nigasu_icon)->oamData.AffineMode = 1;
		(*Work->nigasu_icon)->aff_tbl = IconNigasuAffTbl;
		ActAffAnmChg(*Work->nigasu_icon, 0);
	}
}
/*==============================================
==	�u�ɂ����v�A�j�������҂�
==	return:  �I�� 0,  �p���� 1
==============================================*/
u8 BoxIcon_NigasuWait(void)
{
	if((*Work->nigasu_icon)==NULL){ return 0; }
	if((*Work->nigasu_icon)->banish){ return 0; }

	if((*Work->nigasu_icon)->affend_sw){
//		AffineWorkNumDel((*Work->nigasu_icon)->oamData.AffineParamNo);
//		DelActor((*Work->nigasu_icon));
//		(*Work->nigasu_icon) = NULL;
		(*Work->nigasu_icon)->banish = 1;
	}
	return 1;
}
/*==============================================
==	�u�ɂ����v�A�j����̃A�C�R�����폜
==============================================*/
void BoxIcon_NigasuFinish(void)
{
	if((*Work->nigasu_icon)!=NULL){
		AffineWorkNumDel((*Work->nigasu_icon)->oamData.AffineParamNo);
		del_icon((*Work->nigasu_icon));
		(*Work->nigasu_icon) = NULL;
	}
}
/*==============================================
==	�u�ɂ����v�A�j����̃A�C�R���Ɂu���ǂ�v�A�j���Z�b�g
==============================================*/
void BoxIcon_ModoruInit(void)
{
	if((*Work->nigasu_icon)!=NULL){
		(*Work->nigasu_icon)->banish = 0;
		ActAffAnmChg((*Work->nigasu_icon), 1);
	}
}
/*==============================================
==	�u���ǂ�v�A�j���I���҂�
==============================================*/
u8 BoxIcon_ModoruWait(void)
{
	if(Work->nigasu_icon==NULL){
		return 0;
	}

	if((*Work->nigasu_icon)->affend_sw){
		Work->nigasu_icon = NULL;
	}
	return 1;
}



/*==============================================
==	���܂�Ă���A�C�R���̓���֐�
==============================================*/
static void catch_icon_move(actWork * act)
{
	act->x = Work->cursor_actor->x;
	act->y = Work->cursor_actor->y + Work->cursor_actor->dy + CURSOR_CATCTHICON_DIFF;
}

/**********************************************************************/
/*                                                                    */
/*  �A�C�R���p�Z���}�l�[�W��                                          */
/*                                                                    */
/**********************************************************************/

/*==============================================
==	�w���߹�����ް�ɑΉ������Z���f�[�^��o�^
==	monsno  �����X�^�[�i���o�[
==	return: �A�N�^�[�L�����i���o
==============================================*/
static u16 icon_cellreg_push(u16 monsno)
{
	u16 i;

	// �������݂��o�^����Ă��邩�H
	for(i = 0; i < ICON_CELLMAN_MAX; i++)
	{
		if(Work->icon_cell_use[i] == monsno)
		{
			break;
		}
	}
	// �󂫂����邩�H
	if(i == ICON_CELLMAN_MAX){
		for(i = 0; i < ICON_CELLMAN_MAX; i++)
		{
			if(Work->icon_cell_use[i] == NON_POKE)
			{
				break;
			}
		}
	}
	if(i != ICON_CELLMAN_MAX)
	{
		u8 *src, *dst;
		u16 charno;

		Work->icon_cell_use[i] = monsno;
		Work->icon_cell_cnt[i]++;

		charno = i * 16;
		src = (u8*)(IconCharAdrs[monsno]);
		dst = (u8*)OBJ_MODE0_VRAM + charno * 32;

		CpuCopy(src, dst, OBJ_SIZE16_32x32, 32);

		return charno;
	}

	return 0xffff;
}

/*==============================================
==	�w���߹�����ް�َ̾Q�Ƃ��P���炷
==	monsno  �����X�^�[�i���o�[
==============================================*/
static void icon_cellreg_pop(u16 monsno)
{
	u16 i;

	for(i = 0; i < ICON_CELLMAN_MAX; i++){
		if(Work->icon_cell_use[i] == monsno){
			if(--(Work->icon_cell_cnt[i]) == 0){
				Work->icon_cell_use[i] = NON_POKE;
			}
			return;
		}
	}
}

/**********************************************************************/
/*  �A�N�^�[�o�^�E�폜                                                */
/**********************************************************************/
static const ActOamData PokeIconOam;

/*==============================================
==	�A�C�R���A�N�^�[�o�^
==	monsno  �����X�^�[�i���o�[
==	personal_rnd   ������
==	x, y    �������W
==	oampri  �n�`�l�v���C�I���e�B
==	actpri  �A�N�^�[�v���C�I���e�B
==	reserve �Z���o�^��VBlank���ɍs���悤�ɂ��邩�H
==
==	return: �A�C�R���A�N�^�[�A�h���X
==============================================*/
static actWork* add_icon(u16 monsno, u32 personal_rnd, s16 x, s16 y, u8 oampri, u8 actpri)
{
	actHeader header = {
		CELLID_ICON_MIN,
		PALID_POKEICON_0,
		&PokeIconOam,
		DummyActAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		DummyActMove,
	};

	u16 actno, charno;

	monsno = GetPokeIconCharID(monsno,personal_rnd);

	// �w�b�_�ҏW
	header.palNo = PALID_POKEICON_0 + IconPalAtr[monsno];

	charno = icon_cellreg_push(monsno);
	if(charno == 0xffff)
	{
		return NULL;
	}

	actno = AddActor(&header, x, y, actpri);
	if(actno == ACT_MAX)
	{
		icon_cellreg_pop(monsno);
		return NULL;
	}

	ActWork[actno].oamData.CharNo = charno;
	ActWork[actno].oamData.Priority = oampri;
	ActWork[actno].work[ICONWK_CHAR_ID] = monsno;

	return &ActWork[actno];
}
/*==============================================
==	�A�C�R���A�N�^�[�폜
==	act  �A�C�R���A�N�^�[
==============================================*/
static void del_icon(actWork *act)
{
	icon_cellreg_pop(act->work[ICONWK_CHAR_ID]);
	DelActor(act);
}

/*==============================================
==	�n�`�l�f�[�^
==============================================*/
static const ActOamData PokeIconOam = {
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
	0,      //u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};

/*==============================================
==	�ɂ������A���ǂ鎞�̊g�k�A�j��
==============================================*/
static const actAffAnm IconNigasuAffAnm[] = {
	{ -0x02, -0x02, 0, 120 },
	{ ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm IconModoruAffAnm[] = {
	{ 0x10, 0x10, 0, ACT_AFSET },
	{ 0x10, 0x10, 0, 15        },
	{ ACT_AFEND,0,0,0 },
};

static const actAffAnm * const IconNigasuAffTbl[] = {
	IconNigasuAffAnm,
	IconModoruAffAnm,
};
