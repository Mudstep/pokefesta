/*==================================================================*/
/*                  pic_plt.c                                       */
/*             �@�@ �摜�p���b�g�v�Z����                            */
/*                  by tomo                                         */
/*==================================================================*/

/*==================================================================*/
/*                      INCLUDE FILE                                */
/*==================================================================*/
#include "common.h"
#include "intr.h"
#include "print.h"
#include "calctool.h"
#include "menu.h"
#include "palanm.h"
#include "actanm.h"
#include "objdata.h"
#include "decord.h"
#include "pic_eff.h"

/*==================================================================*/
/*==================================================================*/
extern u16 black_white(u16 *rgb);
/*==================================================================*/
/*                      �֐��錾                                    */
/*==================================================================*/
void poke_bit_color_down(BE_ST *est);
//void effect_pal_trans(BE_ST *est);

//void bit16_to_vram(BE_ST *est,u8 x,u8 y);
/*==================================================================*/
/*                      �����֐��錾                                */
/*==================================================================*/
static void pal_set_c16(void);
static void pal_set_bw(void);
static void pal_set_gr1(void);
static void pal_set_gr2(void);

static void bit_set_c256(u8 mode);
static void bit_set_c16(void);
static void bit_set_gr1(void);
static void bit_set_gr2(void);
static void bit_set_bw(void);

static u16 rgb_sub_color2(u16 *rgb);
static u16 rgb_sub_color3(u16 *rgb);
static u16 rgb_sub_gr1(u16 *rgb);
static u16 rgb_sub_gr2(u16 *rgb);

/*==================================================================*/
/*                      define,typedef��                            */
/*==================================================================*/

/*==================================================================*/
/*                       �ϐ�                                       */
/*==================================================================*/
u8 eax,eay;				//�G�t�F�N�g�G���A�̎n�_(X,Y)
u8 eah,eav;				//�G�t�F�N�g�G���A�̑傫��(h,v)
u8 sah,sav;				//�X�N���[���G���A�̑傫��(h,v)

u16 *sa_adrs;				//�X�N���[���G���A�A�h���X
u16 *pal_adrs;				//�p���b�g�A�h���X
u16 pal_ofs;				//�擪����̃I�t�Z�b�g

/*==================================================================*/
/*==================================================================*/
//���F�Ƃ����p�̃p���b�g�𐶐����܂��B
//u8 mode:�@0:255�F,1:223�F,2:15�F,3:32�F�O���[,4:15�F�O���[,5:2�F����
//////////u8 p_flag: 0:���F�{�p���b�g����,1:���F�̂�,2:�p���b�g�����̂�
//
void poke_bit_color_down(BE_ST *est)
{
	pal_ofs = est->pal_p*16;
	pal_adrs = est->pal_adrs + pal_ofs;
	sa_adrs = est->bg_adrs;
	eax = est->eax, eay = est->eay, eah = est->eah, eav = est->eav;
	sah = est->sah, sav = est->sav;

	switch(est->mode){
	case COL_MODE_255:			//255
		bit_set_c256(0);
		break;
	case COL_MODE_223:			//223(16*2���𑼂Ŏg���̂�)
		bit_set_c256(1);
		break;
	case COL_MODE_15:			//15
		pal_set_c16();
		bit_set_c16();
		break;
	case COL_MODE_32G:			//32g
		pal_set_gr2();
		bit_set_gr2();
		break;
	case COL_MODE_15G:			//15g
		pal_set_gr1();
		bit_set_gr1();
		break;
	case COL_MODE_BW:			//b/w
		pal_set_bw();
		bit_set_bw();
		break;
	default:
		break;
	}
}

/*==================================================================*/
/*	����ȍ~�̓p���b�g�v�Z����                                      */
/*==================================================================*/
//15(�k�P�F���݂�16�F)�ŃJ���[�p���b�g�쐬
static void pal_set_c16(void)
{
	*(pal_adrs+0) = RGB_BLACK;		//�����F�N���A
	*(pal_adrs+1) = ((B_MIN<<10) | (G_MIN<<5) | (R_MIN));		//��
	*(pal_adrs+2) = ((B_MAX<<10) | (G_MAX<<5) | (R_MAX));		//��
	*(pal_adrs+3) = ((B_MID<<10) | (G_MID<<5) | (R_MID));		//�D�F

	*(pal_adrs+4) = ((B_MIN<<10) | (G_MIN<<5) | (R_MAX));		//��
	*(pal_adrs+5) = ((B_MIN<<10) | (G_MAX<<5) | (R_MIN));		//��
	*(pal_adrs+6) = ((B_MAX<<10) | (G_MIN<<5) | (R_MIN));		//��

	*(pal_adrs+7) = ((B_MIN<<10) | (G_MAX<<5) | (R_MAX));		//�C�G���[
	*(pal_adrs+8) = ((B_MAX<<10) | (G_MIN<<5) | (R_MAX));		//�s���N
	*(pal_adrs+9) = ((B_MAX<<10) | (G_MAX<<5) | (R_MIN));		//���F

	*(pal_adrs+10)= ((B_MIN<<10) | (G_MID<<5) | (R_MAX));		//
	*(pal_adrs+11)= ((B_MIN<<10) | (G_MAX<<5) | (R_MID));		//
	*(pal_adrs+12)= ((B_MAX<<10) | (G_MID<<5) | (R_MIN));		//

	*(pal_adrs+13)= ((B_MID<<10) | (G_MIN<<5) | (R_MAX));		//
	*(pal_adrs+14)= ((B_MID<<10) | (G_MAX<<5) | (R_MIN));		//
	*(pal_adrs+15)= ((B_MAX<<10) | (G_MIN<<5) | (R_MID));		//
}

/*==================================================================*/
/*==================================================================*/
//������p�p���b�g�쐬
static void pal_set_bw(void)
{
	*(pal_adrs+0) = RGB_BLACK;			//�k�P�F

	*(pal_adrs+1) = RGB_BLACK;			//�ŏ��̐F������
	*(pal_adrs+2) = RGB_WHITE;			//���̐F�𔒂�
}

/*==================================================================*/
/*==================================================================*/
//�P�T�F(�k�P�F���݂łP�U�F)�ŃO���[�X�P�[���p���b�g�쐬
static void pal_set_gr1(void)
{
	u8 i;

	*(pal_adrs+0) = RGB_BLACK;			//�k�P�F

	*(pal_adrs+1) = RGB_BLACK;			//�ŏ��̐F������

	for(i=0;i<14;i++){					//�P�T(ǹ�F���݂�16)�F�ɂ��邽�߁A"R:G:B=2"�͖���
		*(pal_adrs+2+i) = ((((i+2)*2)<<10) | (((i+2)*2)<<5) | (i+2)*2);
	}
}

/*==================================================================*/
/*==================================================================*/
//32�F(�k�P�F���݂�33�F)�ŃO���[�X�P�[���p���b�g�쐬
static void pal_set_gr2(void)
{
	u8 i;

	*(pal_adrs+0) = RGB_BLACK;			//�k�P�F

	for(i=0;i<32;i++){
		*(pal_adrs+1+i) = ((i<<10) | (i<<5) | i);
	}
}

/*==================================================================*/
/*	����ȍ~�͌��F�Z�b�g����                                        */
/*==================================================================*/
//�t���J���[->256�F���F
//mode 0:255�F(�t���Ŏg�p)�A1:223(255-32)�F
static void bit_set_c256(u8 mode)
{
	u8 r,s,t,c_max;
	u16 *adrs;
	u16 pd;

	if(mode==0){
		c_max=255;
	}else{
		c_max=223;
	}
	for(r=0;r<c_max;r++){								//�p���b�g������
		*(pal_adrs + r) = RGB_BLACK;
	}
	*(pal_adrs + c_max) = RGB_GRAY;					//�D�F(�I�[�o�[��)

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) {	//�k�P�F�̂Ƃ�
				*adrs=pal_ofs;
				continue;
			}

			pd = rgb_sub_color2(adrs);		//���F
			//�Y������p���b�g�`�F�b�N�i�Ȃ��Ƃ��͐V�K�Ƀp���b�g�Z�b�g�j
			for(t=1;t<c_max;t++){
				if(*(pal_adrs+t) == RGB_BLACK){	//�󂫂̂Ƃ�
					*(pal_adrs+t) = pd;
					*adrs=t+pal_ofs;
					break;
				}

				if(*(pal_adrs+t) == pd){		//������
					*adrs=t+pal_ofs;
					break;
				}
			}
			if(t==c_max){			//���Ă͂܂�Ȃ��Ƃ�
				*adrs=c_max;
			}
		}
	}
}

/*==================================================================*/
/*==================================================================*/
//�t���J���[->�����Q�F���F
static void bit_set_bw(void)
{
	u8 r,s;
	u16 *adrs;
	u16 pd;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) {	//�k�P�F�̂Ƃ�
				*adrs=pal_ofs;
				continue;
			}

			pd = black_white(adrs);		//���F
			if(pd == RGB_BLACK) {	//��
				*adrs=1+pal_ofs;
			}else{				//��
				*adrs=2+pal_ofs;
			}
		}
	}
}

/*==================================================================*/
/*==================================================================*/
//�t���J���[->15�F�O���[���F
static void bit_set_gr1(void)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) {	//�k�P�F�̂Ƃ�
				*adrs=pal_ofs;
				continue;
			}

			*adrs = rgb_sub_gr1(adrs) + pal_ofs;
		}
	}

}

/*==================================================================*/
/*==================================================================*/
//�t���J���[->32�F�O���[���F(256�p���b�g�p�j
static void bit_set_gr2(void)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) {	//�k�P�F�̂Ƃ�
				*adrs=pal_ofs;
				continue;
			}

			*adrs = rgb_sub_gr2(adrs) + pal_ofs;
		}
	}
}

/*==================================================================*/
/*==================================================================*/
//�t���J���[->16�F���F
static void bit_set_c16(void)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) {	//�k�P�F�̂Ƃ�
				*adrs=pal_ofs;
				continue;
			}

			*adrs = rgb_sub_color3(adrs) + pal_ofs;
		}
	}
}

/*==================================================================*/
/*            �֐����̌v�Z�֐�                                      */
/*==================================================================*/
//���F����(256����99%�̊m���Ŏ��܂�H�j
static u16 rgb_sub_color2(u16 *rgb)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

	if(r & 0x0003) r = (r & 0x001c) + 4;
	if(g & 0x0003) g = (g & 0x001c) + 4;
	if(b & 0x0003) b = (b & 0x001c) + 4;

	if(r <  6) r = 6;
	if(r > 30) r = 30;
	if(g <  6) g = 6;
	if(g > 30) g = 30;
	if(b <  6) b = 6;
	if(b > 30) b = 30;

	return ((b<<10) | (g<<5) | r);
}

/*==================================================================*/
/*==================================================================*/
#define BL_MAX 11		//���Ƃ��Ă̌��E�̖��邳
#define WH_MIN 20		//���Ƃ��Ă̌��E�̈Â�

//���F����(15�F�ȓ��ɕK���[�߂�)
static u16 rgb_sub_color3(u16 *rgb)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

//--------
	if((r <= BL_MAX)&&(g < BL_MAX)&&(b < BL_MAX)) {			//��
		return 1;
	}
	if((r >= WH_MIN)&&(g >= WH_MIN)&&(b >= WH_MIN)) {		//��
		return 2;
	}

//--------
	if((r >= WH_MIN)&&(g >= WH_MIN)) {			//�C�G���[
		if(b >= ((BL_MAX+WH_MIN)/2)){ //�c��̂P�F�����邢�Ƃ��͔���
			return 2;
		}
		return 7;
	}
	if((r >= WH_MIN)&&(b >= WH_MIN)) {			//�}�[���_
		if(g >= ((BL_MAX+WH_MIN)/2)){ //�c��̂P�F�����邢�Ƃ��͔���
			return 2;
		}
		return 8;
	}
	if((g >= WH_MIN)&&(b >= WH_MIN)) {			//�V�A��
		if(r >= ((BL_MAX+WH_MIN)/2)){ //�c��̂P�F�����邢�Ƃ��͔���
			return 2;
		}
		return 9;
	}

//--------
	if(r >= WH_MIN) {			//�Ԃ������Ƃ�
		if((g > BL_MAX)&&(b > BL_MAX)){		//�c��Q�F���������邢�Ƃ�
			if(g < b){		//�}�[���_��
				return 8;
			}else{			//�C�G���[
				return 7;
			}
		}

		if(g > BL_MAX) return 10;
		if(b > BL_MAX) return 13;
		return 4;
	}
	if(g >= WH_MIN) {			//�΂������Ƃ�
		if((r > BL_MAX)&&(b > BL_MAX)){		//�c��Q�F���������邢�Ƃ�
			if(r < b){		//�V�A����
				return 9;
			}else{			//�C�G���[��
				return 7;
			}
		}

		if(r > BL_MAX) return 11;
		if(b > BL_MAX) return 14;
		return 5;
	}
	if(b >= WH_MIN) {			//�������Ƃ�
		if((r > BL_MAX)&&(g > BL_MAX)){		//�c��Q�F���������邢�Ƃ�
			if(r < g){		//�V�A����
				return 9;
			}else{			//�}�[���_��
				return 8;
			}
		}

		if(g > BL_MAX) return 12;
		if(b > BL_MAX) return 15;
		return 6;
	}

	return 3;									//�c��͑S�ĊD�F(�O�̂���)
}

/*==================================================================*/
/*==================================================================*/
//���F����(�O���[15�F�ȓ��ɕK���[�߂�)
static u16 rgb_sub_gr1(u16 *rgb)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

	r = ((r + g + b)/3)&0x001e;		//���Ԓl����

	if(r==0) return 1;				//0�ƂQ�𓯈��(1�p���b�g����Ȃ��̂�)

	return (r/2);
}

/*==================================================================*/
/*==================================================================*/
//���F����(�O���[32�F�ȓ��ɕK���[�߂�)
static u16 rgb_sub_gr2(u16 *rgb)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

	r = (r + g + b)/3;		//���Ԓl����

	return (1+r);
}

/*==================================================================*/
/*==================================================================*/
#if 0
//bit��������p���b�g����(16bit)�ɕϊ����������A
//���[�h�S�`���łu�q�`�l�֓W�J(�������܂ŃG�t�F�N�g�����킩�̎����p�j
//VRAM�ւ̃A�N�Z�X�́A���[�h�S�ł��P�U�r�b�g�A�N�Z�X�Ȃ̂Œ���
//���̂��߁A�e�X�g�łł͂w���W�͋�������ł��B
void bit16_to_vram(BE_ST *est,u8 x,u8 y)
{
	u8 r,s;
	u16 *in_adrs;
	u16 *out_adrs;
	u16 d;

	effect_pal_trans(est);

	for(s=0;s<eav;s++){	//�c�̃Z��
		in_adrs = sa_adrs + (eay+s) * sah + eax;
		out_adrs = (u16 *)((u8 *)(BG_BITMAP0_VRAM + (y+s) * 240 + x));
		for(r=0;r<(eah/2);r++){	//���̃Z��
			d = (*in_adrs) + ((*(in_adrs+1))<<8);
			*out_adrs = d;

			in_adrs+=2;			//4byte��((u16 *) * 2)
			out_adrs++;			//2byte��((u16 *) * 1)
		}
	}
}
#endif

/*==================================================================*/
/*==================================================================*/
#if 0
//�G�t�F�N�g��̃p���b�g���p���b�gRAM�֓]��(BGorOBJ)
void effect_pal_trans(BE_ST *est)
{
	u16 i,len;

	pal_ofs = est->pal_p*16;
	pal_adrs = est->pal_adrs + pal_ofs;

	len = 0;
	switch(est->mode){
	//256�F�^�C�v
	case COL_MODE_255:			//255
		len = 256*2;
		break;
	case COL_MODE_223:			//223
		len = 224*2;
		break;
	case COL_MODE_15:			//32g
		len = 33*2;
		break;
	//16�F�^�C�v
	case COL_MODE_32G:			//15
		len = 16*2;
		break;
	case COL_MODE_15G:			//15g
		len = 16*2;
		break;
	case COL_MODE_BW:			//b/w
		len = 3*2;
		break;
	}

	if((est->out_mode==0)||(est->out_mode==1)){	//BG mode
		for(i=0;i<len;i++){
			*((u16 *)BG_PLTT+pal_ofs+i) = *(pal_adrs+i);
		}
	}else{								//OBJ mode
		for(i=0;i<len;i++){
			*((u16 *)OBJ_PLTT+pal_ofs+i) = *(pal_adrs+i);
		}
	}
}
#endif
/*==================================================================*/
/*==================================================================*/
