/**********************************************************************/
/*  �p�\�R���{�b�N�X������  -�g���C-                                  */
/**********************************************************************/
#include "common.h"
#include "intr.h"
#include "decord.h"
#include "actanm.h"
#include "actor.h"
#include "palanm.h"
#include "boxsub.h"

#include "box_tray.dat"
#include "box2.h"

/**********************************************************************/
/*  �v���g�^�C�v                                                      */
/**********************************************************************/
static void trans_wallpaper(u8 box, s8 vector);
static s8 calc_scroll_vector(u8 box_no);
static void init_boxname_actor(u8 box);
static void init_arrow_actor(void);

static void init_boxname_scroll(u8 box, s8 vector);
static void init_arrow_scroll(s8 vector);
static void finish_boxname_scroll(void);
static void finish_arrow_scroll(void);
static void boxname_update_pal(void);

/**********************************************************************/
/*  �萔                                                              */
/**********************************************************************/
#define TRAY_MAP_BLOCK   (26)
#define TRAY_CHAR_BLOCK  (2)
#define TRAY_PAL_SIZE    (TRAY_PAL_MAX*0x20)
#define TRAY_MAP_ADDR    MAP_ADDR(TRAY_MAP_BLOCK)
#define TRAY_CHAR_ADDR   CHAR_ADDR(TRAY_CHAR_BLOCK)
#define TRAY_CHAR_MAX    (256)
#define TRAY_CHAR_ADDR_UNIT  (TRAY_CHAR_MAX*32)
#define TRAY_BG_FADE_MASK  (0x0000003f << TRAY_PAL_NUM)

/*==============================================
==	������
==	current_box  �J�����g�{�b�N�X�i���o
==============================================*/
void InitBoxTray(u8 current_box)
{
	Work->tray_trans_pos = 0;
	Work->tray_h = 0;

	DmaClear(3, 0, TRAY_MAP_ADDR, 0x1000, 32);

	trans_wallpaper(current_box, 0);
	init_boxname_actor(current_box);
	init_arrow_actor();
	AddTrayIcon(current_box);

	*(vu16 *)REG_BG2CNT = BG_SCREEN_SIZE_1 | BG_PRIORITY_2
	| TRAY_MAP_BLOCK << BG_SCREEN_BASE_SHIFT
	| TRAY_CHAR_BLOCK << BG_CHAR_BASE_SHIFT
	| BG_COLOR_16 ;
}
/**********************************************************************/
/*  �X�N���[��                                                        */
/**********************************************************************/
/*==============================================
==	�X�N���[��������
==	box_no   �X�N���[����{�b�N�X�i���o
==============================================*/
void BoxTray_InitScroll(u8 box_no)
{
	s8 vector;

	vector = calc_scroll_vector(box_no);
	trans_wallpaper(box_no, vector);
	Work->tray_scroll_speed = vector * TRAY_SCROLL_SPEED;
	Work->tray_scroll_cnt = TRAY_SCROLL_SYNC;

	Work->tray_iconadd_box = box_no;

	Work->tray_iconadd_line = (vector>0)?  0 : (YOKO_KAZU-1);
	Work->tray_iconadd_lineadd = vector;
	Work->tray_iconadd_xorg = (vector>0)? 264 : 56;

	Work->tray_iconadd_x = (vector>0)?  0 : (YOKO_KAZU-1);
	Work->tray_iconadd_xadd = vector;
	Work->tray_iconadd_y = 0;

	Work->tray_iconadd_timer = 2;//TRAY_NEXT_APPEAR_SYNC;

//	TrayIconScrollOut(-(Work->tray_scroll_speed));
//	TrayIconScrollOut(Work->tray_h);
	TrayIconScrollInit(box_no, vector);

	init_boxname_scroll(box_no, vector);
	init_arrow_scroll(vector);
}
/*==============================================
==	�X�N���[���I���҂�
==	return :  �I��  0,  �p���� 1
==============================================*/
u8 BoxTray_WaitScroll(void)
{
	u8 ret = TrayIconScrollWait();

	if(Work->tray_scroll_cnt){
		Work->tray_h += Work->tray_scroll_speed;
		Work->tray_h &= 0x1ff;

		if(--(Work->tray_scroll_cnt) == 0){
			finish_boxname_scroll();
			finish_arrow_scroll();
		}
		return 1;
	}
	return ret;
}
/*==============================================
==	�X�N���[�������v�Z
==	box_no   �X�N���[����{�b�N�X�i���o
==	return :  �E���� -1,  ������ 1
==============================================*/
static s8 calc_scroll_vector(u8 box_no)
{
	u8 current = GetCurrentBox();
	u8 cnt = 0;

	while(current != box_no){
		if(++current >= BOX_KAZU){
			current = 0;
		}
		cnt++;
	}

	return (cnt < (BOX_KAZU/2))? 1 : -1;
}
/**********************************************************************/
/*  �ǎ��ύX                                                          */
/**********************************************************************/
/*==============================================
==	������
==	wp_no   �ǎ��i���o�[
==============================================*/
void BoxTray_WallPaperChangeInit(u8 wp_no)
{
	u8 current = GetCurrentBox();

	BoxData.BGNum[current] = wp_no;
	Work->wp_prog = 0;
}
/*==============================================
==	�I���҂�
==	return: �I�� 0,  �p���� 1
==============================================*/
u8 BoxTray_WallPaperChangeWait(void)
{
	switch(Work->wp_prog){
	case 0:
		PaletteFadeReq(Work->tray_fade_mask, 1, 0, 16, 0xffff);
		Work->wp_prog++;
		break;

	case 1:
		if(!PaletteAnime()){
			trans_wallpaper(GetCurrentBox(), 0);
			boxname_update_pal();
			PaletteFadeReq(Work->tray_fade_mask, 1, 16, 0, 0xffff);
			Work->wp_prog++;
		}
		break;

	case 2:
		if(!PaletteAnime()){
			Work->wp_prog++;
		}
		break;

	case 3:
		return 0;
	}
	return 1;
}
/**********************************************************************/
/*  �ǎ��f�[�^�]��                                                    */
/**********************************************************************/
static void trans_tray_map(u16 *dst_top, const u16 *src, s8 vector, u8 pos);
static void clear_tray_bottom(u16 *dst);

/*==============================================
==	�ǎ��f�[�^�]�����C��
==	box     �{�b�N�X�i���o�[
==	vector  ������  �F-1
==	        ���݈ʒu�F 0
==	        �E����  �F 1
==============================================*/
static void trans_wallpaper(u8 box, s8 vector)
{
	const WPDAT *wpdat;

	if(vector){
		Work->tray_trans_pos = !(Work->tray_trans_pos);
		clear_tray_bottom((u16*)TRAY_MAP_ADDR);
	}

	wpdat = WPDatTbl + BoxData.BGNum[box];

	PaletteWorkSet(wpdat->pal_data, TRAY_PAL+Work->tray_trans_pos*TRAY_PAL_MAX*16, TRAY_PAL_SIZE);

	LZ77UnCompWram(wpdat->map_data, Work->wpbg_work);
	trans_tray_map((u16*)TRAY_MAP_ADDR, Work->wpbg_work, vector, Work->tray_trans_pos);

	LZ77UnCompVram(wpdat->char_data, (void*)(TRAY_CHAR_ADDR+(0x2000*Work->tray_trans_pos)));
}
/*==============================================
==	�ǎ��}�b�v�f�[�^�]��
==	dst_top   �]����}�b�v�u���b�N�̐擪�A�h���X
==	src       �]�����}�b�v�f�[�^�̐擪�A�h���X
==	vector    ������  �F-1
==	          ���݈ʒu�F 0
==	          �E����  �F 1
==  pos       �]����G���A�i0-1�j
==============================================*/
static void trans_tray_map(u16 *dst_top, const u16 *src, s8 vector, u8 pos)
{
	u16 char_add, char_num, pal_add, pal_num, x, y;
	s16 block_x, width, rest_width, clear_x;
	u16 *dst, *rest_dst, *clear_dst;

	char_add = pos * TRAY_CHAR_MAX;
	pal_add = (TRAY_PAL_NUM + pos * TRAY_PAL_MAX) << 12;

	block_x = (10 + (Work->tray_h / 8) + (TRAY_XCHARS * vector)) & 63;

	if(block_x < 13){
		width = 20;
		rest_width = 0;
		dst = dst_top + ((32*2) + block_x);
		rest_dst = NULL;
	}else if(block_x < 32){
		width = 32 - block_x;
		rest_width = 20 - width;
		dst = dst_top + ((32*2) + block_x);
		rest_dst = dst_top + 1024 + (32*2);
	}else if(block_x < 45){
		width = 20;
		rest_width = 0;
		dst = dst_top + 1024 + (32*2) + (block_x-32);
		rest_dst = NULL;
	}else{
		width = 64 - block_x;
		rest_width = 20 - width;
		dst = dst_top + 1024 + (32*2) + (block_x-32);
		rest_dst = dst_top + (32*2);
	}

	for(y = 0; y < 18; y++){

		for(x = 0; x < width; x++){
			char_num = (*src & 0x0fff) + char_add;
			pal_num =  (*src & 0xf000) + pal_add;
			dst[x] = pal_num | char_num;
			src++;
		}

		for(x = 0; x < rest_width; x++){
			char_num = (*src & 0x0fff) + char_add;
			pal_num =  (*src & 0xf000) + pal_add;
			rest_dst[x] = pal_num | char_num;
			src++;
		}

		dst += 32;
		rest_dst += 32;
	}

	if(vector){
		clear_x = ((vector > 0)? (block_x+TRAY_BODY_CHARS) : (block_x-TRAY_MARGIN_CHARS)) & 63;

		if(clear_x < 32){
			clear_dst = dst_top + (32*2) + clear_x;
		}else{
			clear_dst = dst_top + 1024 + (32*2) + (clear_x-32);
		}

		for(x = 0; x < TRAY_MARGIN_CHARS; x++){
			for(y = 0; y < 18; y++){
				*clear_dst = 0x00;
				clear_dst += 32;
			}
			clear_x = (clear_x + 1) & 63;
			if(clear_x < 32){
				clear_dst = dst_top + (32*2) + clear_x;
			}else{
				clear_dst = dst_top + 1024 + (32*2) + (clear_x-32);
			}
		}
	}

}
/*==============================================
==	�X�N���[���J�n�O�ɁA�ǎ��̍ŉ��Z���P���C�����[���N���A
==�i�L�����f�[�^�����������ƁA�X�e�[�^�X�E�B���h�E�������猩���Ă��܂����߁j
==
==	dst       �]����}�b�v�u���b�N�̐擪�A�h���X
==============================================*/
static void clear_tray_bottom(u16 *dst)
{
	s16 block_x;
	u16 x;


	block_x = (10 + (Work->tray_h / 8) + TRAY_BODY_CHARS) & 63;

	if(block_x < 32){
		dst += ((32*19) + block_x);
	}else{
		dst += (1024 + (32*19) + (block_x-32));
	}

	for(x = 0; x < (64-TRAY_BODY_CHARS); x++){
		*dst++ = 0x00;
		block_x = (block_x+1) & 63;
		if(block_x == 0){
			dst -= (1024+32);
		}
		if(block_x == 32){
			dst += (1024-32);
		}
	}
}
/**********************************************************************/
/*  �{�b�N�X���A�N�^�[                                                */
/**********************************************************************/
#define BOXNAME_CX  (164)
#define BOXNAME_Y   (28)

static const actHeader BoxNameHeader;
static s16 calc_boxname_xpos(u16 len);
static void boxname_scrollin_move(actWork *act);
static void boxname_scrollout_move(actWork *act);

/*==============================================
==	������
==	box     �J�����g�{�b�N�X�i���o�[
==============================================*/
static void init_boxname_actor(u8 box)
{
	CellData Cell = {
		(const u8*)(Work->fontobj_cell),
		0x200,
		CELLID_BOXNAME,
	};
	PalData  Pal[] = {
		{ (const u16*)(Work->boxname_pal), PALID_BOXNAME },
		{ (const u16*)(Work->boxname_pal), PALID_NEXTBOXNAME },
		{ OBJPAL_END, 0 },
	};

	u16 bg_no, pal_no, act_no, i;
	s16 x;

	bg_no = BoxData.BGNum[box];
	Work->boxname_pal[14] = BoxNamePalData[bg_no][0];
	Work->boxname_pal[15] = BoxNamePalData[bg_no][1];

	ObjPalSets(Pal);

	Work->tray_fade_mask = TRAY_BG_FADE_MASK;

	pal_no = ObjPalNumGet(PALID_BOXNAME);
	Work->boxname_palno = PA_OBJ0 + pal_no * 16 + 14;
	Work->tray_fade_mask |= (0x00010000 << pal_no);

	pal_no = ObjPalNumGet(PALID_NEXTBOXNAME);
	Work->nextboxname_palno = PA_OBJ0 + pal_no * 16 + 14;
	Work->tray_fade_mask |= (0x00010000 << pal_no);

	PM_strcpy_clear(Work->boxname_buf, BoxData.BoxName[box], spc_, 8);
	BoxDrawFont_32x16(Work->boxname_buf, Work->fontobj_cell, DRAWTYPE_16TRANS, 0, Work->fontobj_work);
	CellSet(&Cell);

	x = calc_boxname_xpos(PM_strlen(BoxData.BoxName[box]));
	for(i = 0; i < 2; i++){
		act_no = AddActor(&BoxNameHeader, x+i*32, BOXNAME_Y, ACTPRI_BOXNAME);
		Work->boxname_actor[i] = &ActWork[act_no];
		ActAnmChg(Work->boxname_actor[i], i);
	}

	Work->boxname_toggle = 0;
}

/*==============================================
==	�X�N���[����̃{�b�N�X���A�N�^�[����������
==	box     �X�N���[����{�b�N�X�i���o�[
==	vector  �X�N���[�������i-1 or 1�j
==============================================*/
static void init_boxname_scroll(u8 box, s8 vector)
{
	CellData Cell = {
		(const u8*)(Work->fontobj_cell),
		0x200,
		CELLID_BOXNAME
	};
	actHeader header = BoxNameHeader;
	u16 bg_no, act_no, pal_no, i;
	s16 end_x, start_x;

	// �Ă΂�邽�т� 0,1,0,1,... �̒l���J��Ԃ�
	Work->boxname_toggle = !(Work->boxname_toggle);
	if(Work->boxname_toggle == 0){
		Cell.act_num = CELLID_BOXNAME;
		pal_no = Work->boxname_palno;
	}else{
		Cell.act_num = CELLID_NEXTBOXNAME;
		pal_no = Work->boxname_palno;
		header.cellNo = CELLID_NEXTBOXNAME;
		header.palNo = PALID_NEXTBOXNAME;
	}

	PM_strcpy_clear(Work->boxname_buf, BoxData.BoxName[box], spc_, 8);
	BoxDrawFont_32x16(Work->boxname_buf, Work->fontobj_cell, DRAWTYPE_16TRANS, 0, Work->fontobj_work);
	CellSet(&Cell);

	bg_no = BoxData.BGNum[box];
	PaletteWorkSet(BoxNamePalData[bg_no], pal_no, 4);

	end_x = calc_boxname_xpos(PM_strlen(BoxData.BoxName[box]));
	start_x = end_x + (vector * TRAY_SCROLL_WIDTH);
	for(i = 0; i < 2; i++){
		act_no = AddActor(&header, start_x+i*32, BOXNAME_Y, ACTPRI_BOXNAME);
		Work->nextboxname_actor[i] = &ActWork[act_no];
		Work->nextboxname_actor[i]->work[0] = (-vector * TRAY_SCROLL_SPEED);
		Work->nextboxname_actor[i]->work[1] = end_x + i*32;
		Work->nextboxname_actor[i]->work[2] = 1;
		Work->nextboxname_actor[i]->move = boxname_scrollin_move;
		ActAnmChg(Work->nextboxname_actor[i], i);

		Work->boxname_actor[i]->work[0] = (-vector * TRAY_SCROLL_SPEED);
		Work->boxname_actor[i]->work[1] = 1;
		Work->boxname_actor[i]->move = boxname_scrollout_move;
	}
}
/*==============================================
==	�X�N���[����̃{�b�N�X���A�N�^�[�I������
==============================================*/
static void finish_boxname_scroll(void)
{
	if(Work->boxname_toggle == 0){
		CellDel(CELLID_NEXTBOXNAME);
	}else{
		CellDel(CELLID_BOXNAME);
	}

	Work->boxname_actor[0] = Work->nextboxname_actor[0];
	Work->boxname_actor[1] = Work->nextboxname_actor[1];

}
/*==============================================
==	�{�b�N�X���̃X�N���[���C������֐�
==============================================*/
static void boxname_scrollin_move(actWork *act)
{
	if(act->work[2]){
		act->work[2]--;
		return;
	}
	act->x += act->work[0];
	if(act->x == act->work[1]){
		act->move = DummyActMove;
	}
}
/*==============================================
==	�{�b�N�X���̃X�N���[���A�E�g����֐�
==============================================*/
static void boxname_scrollout_move(actWork *act)
{
	if(act->work[1]){
		act->work[1]--;
		return;
	}

	act->x += act->work[0];
	act->work[2] = act->x + act->dx;
	if(act->work[2] < 64 || act->work[2] > 256){
		DelActor(act);
	}
}
/*==============================================
==	�J�����g�̃{�b�N�X���p���b�g�X�V
==============================================*/
static void boxname_update_pal(void)
{
	u8 box, bg;

	box = GetCurrentBox();
	bg = BoxData.BGNum[box];

	if(Work->boxname_toggle == 0){
		CpuCopy(BoxNamePalData[bg], &PaletteWork[Work->boxname_palno], 4, 16);
	}else{
		CpuCopy(BoxNamePalData[bg], &PaletteWork[Work->nextboxname_palno], 4, 16);
	}
}
/*==============================================
==	�{�b�N�X���̕���������A�N�^�[�̂w���W�v�Z
==	len  �{�b�N�X���̕�����
==============================================*/
static s16 calc_boxname_xpos(u16 len)
{
	return BOXNAME_CX + 12 - (len * 4);
}
/**********************************************************************/
/*  �X�N���[���{�^���A�N�^�[                                          */
/**********************************************************************/
#define ARROW_X      (92)
#define ARROW_Y      (28)
#define ARROW_XDIFF  (136)
#define ARROW_L_LIMIT (72)
#define ARROW_R_LIMIT (248)
#define ARROW_1ST_APPEAR_SYNC  (TRAY_NEXT_APPEAR_SYNC)
#define ARROW_2ND_APPEAR_SYNC  (TRAY_NEXT_APPEAR_SYNC + ((18*8)/TRAY_SCROLL_SPEED))


enum ARROW_WORK {
	ARWK_MODE,
	ARWK_COUNT1,
	ARWK_COUNT2,
	ARWK_AX,
	ARWK_STOPFLG,
};

enum ARROW_MODE {
	ARMODE_WAIT,
	ARMODE_MOVE,
	ARMODE_SCROLL_WAIT,
	ARMODE_SCROLL,
	ARMODE_SCROLL_AFTER,
};

/*==============================================
==	�X�N���[���{�^���A�N�^�[������
==============================================*/
static const actHeader ArrowActHeader;
static void init_arrow_actor(void)
{
	static const CellData cell = { arrow_Character, sizeof(arrow_Character), CELLID_ARROW };
	static const PalData pal =   { arrow_Palette, PALID_ARROW };
	u16 i, no;

	CellSet(&cell);
	ObjPalSet(&pal);

	for(i = 0; i < 2; i++){
		no = AddActor(&ArrowActHeader, ARROW_X+i*ARROW_XDIFF, ARROW_Y, ACTPRI_ARROW);
		if(no != ACT_MAX){
			ActAnmChg(&ActWork[no], i);
			ActWork[no].work[ARWK_AX] = (i==0)? -1 : 1;
			Work->arrow_actor[i] = &ActWork[no];
		}
	}

	if(CheckArrowMoveState())
	{
		BoxArrowMove(1);
	}
}
/*==============================================
==	�X�N���[��������
==	vector  �X�N���[�������i�E -1,  �� 1�j
==============================================*/
static void init_arrow_scroll(s8 vector)
{
	u16 i;
	for(i = 0; i < 2; i++){
		Work->arrow_actor[i]->dx = 0;
		Work->arrow_actor[i]->work[ARWK_MODE] = ARMODE_SCROLL_WAIT;
	}

	if(vector < 0){
		// COUNT1:�ʒu�������܂ł̳���
		Work->arrow_actor[0]->work[ARWK_COUNT1] = ARROW_2ND_APPEAR_SYNC;
		Work->arrow_actor[1]->work[ARWK_COUNT1] = ARROW_1ST_APPEAR_SYNC;

		// COUNT2:�ʒu���������̂w���W
		Work->arrow_actor[0]->work[ARWK_COUNT2] = ARROW_L_LIMIT;
		Work->arrow_actor[1]->work[ARWK_COUNT2] = ARROW_L_LIMIT;

	}else{
		Work->arrow_actor[0]->work[ARWK_COUNT1] = ARROW_1ST_APPEAR_SYNC;
		Work->arrow_actor[1]->work[ARWK_COUNT1] = ARROW_2ND_APPEAR_SYNC;

		Work->arrow_actor[0]->work[ARWK_COUNT2] = ARROW_R_LIMIT;
		Work->arrow_actor[1]->work[ARWK_COUNT2] = ARROW_R_LIMIT;
	}

	Work->arrow_actor[0]->work[7] = 0;
	Work->arrow_actor[1]->work[7] = 1;

}
/*==============================================
==	�X�N���[���I��
==============================================*/
static void finish_arrow_scroll(void)
{
	u16 i;
	for(i = 0; i < 2; i++){
		Work->arrow_actor[i]->x = ARROW_X+i*ARROW_XDIFF;
		Work->arrow_actor[i]->dx = 0;
		Work->arrow_actor[i]->banish = 0;
	}
	BoxArrowMove(1);
}
/*==============================================
==	�X�N���[���{�^�����h��J�n�E��~
==============================================*/
void BoxArrowMove(u8 on)
{
	u16 i;
	if(on){
		for(i = 0; i < 2; i++){
			Work->arrow_actor[i]->work[ARWK_MODE] = ARMODE_MOVE;
			Work->arrow_actor[i]->work[ARWK_COUNT1] = 0;
			Work->arrow_actor[i]->work[ARWK_COUNT2] = 0;
			Work->arrow_actor[i]->work[ARWK_STOPFLG] = 0;
		}
	}else{
		for(i = 0; i < 2; i++){
			Work->arrow_actor[i]->work[ARWK_MODE] = ARMODE_WAIT;
		}
	}
}
/*==============================================
==	����֐�
==============================================*/
static void arrow_actmove(actWork *act)
{
	switch(act->work[ARWK_MODE]){
	case ARMODE_WAIT:
		act->dx = 0;
//		if(CheckArrowMoveState()){
//			act->work[ARWK_MODE] = ARMODE_MOVE;
//			act->work[ARWK_COUNT1] = 0;
//			act->work[ARWK_COUNT2] = 0;
//		}
		break;

	case ARMODE_MOVE:
		if(++(act->work[ARWK_COUNT1]) > 3){
			act->work[ARWK_COUNT1] = 0;
			act->dx += act->work[ARWK_AX];
			if(++(act->work[ARWK_COUNT2]) > 5){
				act->work[ARWK_COUNT2] = 0;
				act->dx = 0;
			}
		}
//		if(!CheckArrowMoveState()){
//			act->work[ARWK_MODE] = ARMODE_WAIT;
//			act->work[ARWK_COUNT1] = 0;
//			act->work[ARWK_COUNT2] = 0;
//		}
		break;

	case ARMODE_SCROLL_WAIT:
		act->work[ARWK_MODE] = ARMODE_SCROLL;
		break;

	case ARMODE_SCROLL:
		act->x -= Work->tray_scroll_speed;
		if(act->x <= ARROW_L_LIMIT || act->x >= ARROW_R_LIMIT){
			act->banish = 1;
		}
		act->work[ARWK_COUNT1]--;
		if(act->work[ARWK_COUNT1] == 0){
			act->x = act->work[ARWK_COUNT2];
			act->banish = 0;
			act->work[ARWK_MODE] = ARMODE_SCROLL_AFTER;
		}
		break;

	case ARMODE_SCROLL_AFTER:
		act->x -= Work->tray_scroll_speed;
		break;
	}
}
/**********************************************************************/
/*  �A�N�^�[�f�[�^                                                    */
/**********************************************************************/
/*==============================================
==	�{�b�N�X��
==============================================*/
static const ActOamData BoxNameOam = {
	0,		//u32 VPos:8;             // �x���W
	0,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	1,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;    // �A�t�B���ϊ��p�����[�^No & H,VFlip
	2,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	2,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
static const actAnm BoxNameAnm[] = {
	{ 0, 5, 0, 0, }, { ACT_ANMEND, 0, 0, 0 },
	{ 8, 5, 0, 0, }, { ACT_ANMEND, 0, 0, 0 },
};
static const actAnm * const BoxNameAnmTbl[] = {
	BoxNameAnm + 0,
	BoxNameAnm + 2,
};
static const actHeader BoxNameHeader = {
	CELLID_BOXNAME,
	PALID_BOXNAME,
	&BoxNameOam,
	BoxNameAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	DummyActMove,
};
/*==============================================
==	�X�N���[���{�^��
==============================================*/
static const ActOamData ArrowOam = {
	0,		//u32 VPos:8;             // �x���W
	0,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	2,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;    // �A�t�B���ϊ��p�����[�^No & H,VFlip
	0,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	2,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
static const actAnm ArrowAnm[] = {
	{ 0, 5, 0, 0, }, { ACT_ANMEND, 0, 0, 0 },
	{ 2, 5, 0, 0, }, { ACT_ANMEND, 0, 0, 0 },
};
static const actAnm * const ArrowAnmTbl[] = {
	ArrowAnm + 0,
	ArrowAnm + 2,
};

static const actHeader ArrowActHeader = {
	CELLID_ARROW,
	PALID_ARROW,
	&ArrowOam,
	ArrowAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	arrow_actmove,
};
/*==============================================
==	�g���C�p�X�N���[���{�^���Ɠ������̂��w��ʒu�ɕ\������
==
==	x       �����{�^���w���W
==	y       �����{�^���x���W
==	vector  �����i0:������  1:�E�����j
==	oampri  �n�`�l�v���C�I���e�B
==	actpri  �A�N�^�[�v���C�I���e�B
==
==	return: �A�N�^�[�ւ̃|�C���^
==
==============================================*/
actWork* MakeBoxArrowActor(u16 x, u16 y, u8 vector, u8 oampri, u8 actpri)
{
	u16 no;

	no = AddActor(&ArrowActHeader, x, y, actpri);
	if(no == ACT_MAX){
		return NULL;
	}

	vector &= 1;
	ActAnmChg(&ActWork[no], vector);
	ActWork[no].oamData.Priority = oampri;
	ActWork[no].move = DummyActMove;

	return &ActWork[no];
}
