/*==================================================================*/
/*                  pic_eff.c                                       */
/*             �@�@ �摜�G�t�F�N�g�v�Z����                          */
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

#include "e_rand.h"

#include "pic_eff.h"

/*==================================================================*/
/*                      �֐��錾                                    */
/*==================================================================*/
void poke_bit_effect(BE_ST *est);

/*==================================================================*/
/*                      �����֐��錾                                */
/*==================================================================*/
//static void effect_tenten(void);
//static void effect_pasuteru(void);
static void effect_pasuteru_poke(void);
//static void effect_focus_h(void);
//static void effect_focus_v(void);
static void effect_gray_scale(void);
//static void effect_gray_scale2(void);
//static void effect_fuchidori_h(void);
static void effect_fuchidori_v(void);
//static void effect_fuchidori_v_v2(void);
static void effect_rinkaku(void);
static void effect_rinkaku_color(u8 pat);
static void effect_rinkaku_plus(void);
static void effect_hanten(void);
static void effect_awai_poke(void);
//static void effect_awai(void);
static void effect_fuchidori2_h(void);
static void effect_fuchidori2_v(void);
static void effect_gray_color_up(u8 uv);
static void effect_gray_white_down(u8 uv);

//static void aburae_noise(u16 no);
static void aburae_noise_poke(u16 no);
u16 black_white(u16 *rgb);
static u16 color_white(u16 *rgb,u8 pat) ;
static u16 black_fuchi(u16 *rgb,u16 *rgb2);
static u16 black2color(u8 pat);
//static u16 rgb_comp3(u16 *rgb1,u16 *rgb2,u16 *rgb3);
static u16 rgb_gray_scale(u16 *rgb);
//static u16 rgb_gray_scale2(u16 *rgb);
static u16 rgb_meian_change(u16 *rgb);
static u16 rgb_fuchi2_x(u16 *rgb1,u16 *rgb2);
static u16 rgb_fuchi3(u16 *rgb1,u16 *rgb2,u16 *rgb3);
static u16 rgb_fuchi3_v2(u16 *rgb1,u16 *rgb2,u16 *rgb3);

/*==================================================================*/
/*                define,typedef��                                  */
/*==================================================================*/
typedef struct{
	u8 x;
	u8 y;
	u16 sub_col;		//���G��������l
}CN;		//���G���m�C�Y�i�[�p���[�N

/*==================================================================*/
/*                �ϐ�                                              */
/*==================================================================*/
u8 eax,eay;				//�G�t�F�N�g�G���A�̎n�_(X,Y)
u8 eah,eav;				//�G�t�F�N�g�G���A�̑傫��(h,v)
u8 sah,sav;				//�X�N���[���G���A�̑傫��(h,v)

u16 *sa_adrs;			//�X�N���[���G���A�A�h���X

u8 eff_pat;				//�G�t�F�N�g�p�^�[��(0:�ʏ�A1-4:�p�^�[��1-4)
/*==================================================================*/
/*==================================================================*/
//bitmap�摜���G�t�F�N�g������
//��15bit(32768�F)�Abit15���P�̂Ƃ��̓k�P�B
//u8 eff_no:�G�t�F�N�g�ԍ�(1-34�B���Ԃ���)
//u8 ex,ey:�G�t�F�N�g���镔���̎n�_
//u8 eh,ev:�G�t�F�N�g���镔����X,Y�̑傫��
//u8 sh,sv:�i�[���Ă���G���A�S�̂�X,Y�̑傫��

void poke_bit_effect(BE_ST *est)
{
	sa_adrs = est->bg_adrs;
	eff_pat = est->pat;
	eax = est->eax, eay = est->eay, eah = est->eah, eav = est->eav;
	sah = est->sah, sav = est->sav;

	switch(est->eff_no){
//	case EFF_KOUSHI:			//�i�q
//		effect_tenten();
//		break;
	case EFF_PASUTERU_POKE:			//�p�X�e����(�|�P�����摜��p�Łj
		effect_pasuteru_poke();
		break;
//	case EFF_FOCUS_H:			//�t�H�[�J�X�i�����C���j
//		effect_focus_h();
//		break;
//	case EFF_FOCUS_V:			//�t�H�[�J�X�i�c���C���j
//		effect_focus_v();
//		break;
//	case EFF_FOCUS:				//�t�H�[�J�X�i�c���j
//		effect_focus_h();
//		effect_focus_v();
//		break;
//	case EFF_GRAY15:			//�O���[�X�P�[���P�T�F
//		effect_gray_scale2();
//		break;
//	case EFF_FUCHI_H:			//�ӂ��ǂ苭���i�����C���j
//		effect_fuchidori_h();
//		break;
	case EFF_FUCHI_V:			//�ӂ��ǂ苭���i�c���C���j
		effect_fuchidori_v();
		break;
	case EFF_RINKAKU:			//�֊s��肾��
		effect_rinkaku_plus();
		effect_rinkaku_color(eff_pat);
		break;
	case EFF_SHIRUETTO:			//�V���G�b�g(����)
		effect_rinkaku_plus();
		effect_hanten();
		effect_rinkaku();
		effect_hanten();
		break;
	case EFF_KYOUCYOU_BW:		//���������摜(����)
		effect_rinkaku_plus();
		effect_fuchidori2_h();
		effect_fuchidori2_h();
		effect_fuchidori2_v();
		effect_rinkaku();
		break;
	case EFF_AWAI_POKE:				//�W�F���i�����j
		effect_awai_poke();
		break;
	case EFF_RINKAKU_KYOUCYO:	//�֊s�̂݋���
		effect_rinkaku_plus();
		break;
	case EFF_HANTEN:			//���Ô��]
		effect_hanten();
		break;
	case EFF_FUCHI2_H:			//��������Łi���j
		effect_fuchidori2_h();
		break;
	case EFF_FUCHI2_V:			//��������Łi�c�j
		effect_fuchidori2_v();
		break;
	case EFF_GRAY32:			//�O���[�X�P�[���i32�F�j
		effect_gray_scale();
		effect_gray_color_up(GRAY_COLOR_UP);
		break;
//	case EFF_PASUTERU:			//�p�X�e����
//		effect_pasuteru();
//		break;
	case EFF_SUMIE:				//�n�G���i�����j
		effect_rinkaku_plus();
		effect_fuchidori2_h();
		effect_fuchidori2_v();
		effect_rinkaku();
		effect_fuchidori_v();
		effect_fuchidori_v();
		effect_gray_color_up(SUMIE_COLOR_UP);
		effect_gray_white_down(SUMIE_WHITE_DOWN);
		break;
//	case EFF_AWAI:				//�W�F���i�����j
//		effect_awai();
//		break;
	default:
		break;
	}
}

/*==================================================================*/
/*                 �e�G�t�F�N�g�v�Z����                             */
/*==================================================================*/
//�O���[���̃J���[�̓x�������グ��
//���O���[�ȊO�̃f�[�^�ł̓���͕ۏ؂��܂���B
static void effect_gray_color_up(u8 uv)
{
	u8 r,s,rgb;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;

			rgb = *adrs & 0x001f;
			rgb+=uv;
			if(rgb > 0x1f) rgb=0x1f;

			*adrs = ((rgb<<10) | (rgb<<5) | rgb);
		}
	}
}

/*==================================================================*/
/*==================================================================*/
//�O���[���̔��ߕӂ̐F���D�F��
//���O���[�ȊO�̃f�[�^�ł̓���͕ۏ؂��܂���B
static void effect_gray_white_down(u8 uv)
{
	u8 r,s,rgb;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;

			rgb = *adrs & 0x001f;

			if(rgb > (0x1f-uv)) rgb = 0x1f - (uv/2);

			*adrs = ((rgb<<10) | (rgb<<5) | rgb);
		}
	}
}
#if 0			//����͎g�p���Ȃ��̂� 0
/*==================================================================*/
/*==================================================================*/
//�G�t�F�N�g	//�_�X�H�G�t�F�N�g
static void effect_tenten(void)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;
			if(!(((s%2)==0)&&((r%2)==0))){
				*(adrs) = RGB_WHITE;
			}
		}
	}
}
#endif

#if 0			//����͎g�p���Ȃ��̂� 0
/*==================================================================*/
/*==================================================================*/
//���G�i�p�X�e���j��
static void effect_pasuteru(void)
{
	u32 i;

	for(i=0;i<(eah*50);i++) aburae_noise(i);
}
#endif

/*==================================================================*/
/*==================================================================*/
//���G�i�p�X�e���j��(�|�P����64*64�h�b�g��p�����Łj
static void effect_pasuteru_poke(void)
{
	u32 i;

	for(i=0;i<(PA_H*50);i++) aburae_noise_poke(i);
}

#if 0			//����͎g�p���Ȃ��̂� 0
/*==================================================================*/
/*==================================================================*/
//�t�H�[�J�X��1��
static void effect_focus_h(void)
{
	u8 r,s;
	u16 rgb_tmp1;
	u16 *adrs;

	for(s=0;s<eav;s++){
		adrs = sa_adrs + (eay+s) * sah + eax;
		//���[
		rgb_tmp1 = *adrs;
		//����
		for(r=1,adrs++;r<(eah-1);r++,adrs++){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_comp3(&rgb_tmp1,adrs,(adrs+1));
			rgb_tmp1 = *adrs;
		}
		//�E�[
		//���ݓ��ɏ����Ȃ�
	}
}

/*==================================================================*/
/*==================================================================*/
//�t�H�[�J�X�c1��
static void effect_focus_v(void)
{
	u8 r,s;
	u16 rgb_tmp1;
	u16 *adrs;

	for(r=0;r<eah;r++){
		adrs = sa_adrs + eay * sah + (eax+r);
		//��[
		rgb_tmp1 = *adrs;	
		//����
		for(s=1,adrs+=sah;s<(eav-1);s++,adrs+=sah){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_comp3(&rgb_tmp1,adrs,(adrs+sah));
			rgb_tmp1 = *adrs;
		}
		//���[
		//���ݓ��ɏ����Ȃ�
	}
}
#endif

/*==================================================================*/
/*==================================================================*/
//��ʑS�̂��O���C�X�P�[����(32�F�g�p�j
static void effect_gray_scale(void)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;

			*adrs = rgb_gray_scale(adrs);
		}
	}
}

#if 0			//����͎g�p���Ȃ��̂� 0
/*==================================================================*/
/*==================================================================*/
//��ʑS�̂��O���C�X�P�[����(15�F�g�p�j
static void effect_gray_scale2(void)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;
			*adrs = rgb_gray_scale2(adrs);
		}
	}
}
#endif

#if 0			//����͎g�p���Ȃ��̂� 0
/*==================================================================*/
/*==================================================================*/
//����艡�̂�
static void effect_fuchidori_h(void)
{
	u8 r,s;
	u16 rgb_tmp1;
	u16 *adrs;

	for(s=0;s<eav;s++){		//������
		adrs = sa_adrs + (eay+s) * sah + eax;
		//���[
		rgb_tmp1 = *adrs;	
		//����
		for(r=1,adrs++;r<(eah-1);r++,adrs++){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_fuchi3(&rgb_tmp1,adrs,(adrs+1));
			rgb_tmp1 = *adrs;
		}
		//�E�[
		//���ݓ��ɏ����Ȃ�
	}
}
#endif


/*==================================================================*/
/*==================================================================*/
//�����c�̂�
static void effect_fuchidori_v(void)
{
	u8 r,s;
	u16 rgb_tmp1;
	u16 *adrs;
	
	for(r=0;r<eah;r++){
		adrs = sa_adrs + eay * sah + (eax+r);
		//��[
		rgb_tmp1 = *adrs;	
		//����
		for(s=1,adrs+=sah;s<(eav-1);s++,adrs+=sah){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_fuchi3(&rgb_tmp1,adrs,(adrs+sah));
			rgb_tmp1 = *adrs;
		}
		//���[
	}
}

/*==================================================================*/
/*==================================================================*/
#if 0			//����͎g�p���Ȃ��̂� 0
//�����c�̂�(����Łj
static void effect_fuchidori_v_v2(void)
{
	u8 r,s;
	u16 rgb_tmp1;
	u16 *adrs;
	
	for(r=0;r<eah;r++){
		adrs = sa_adrs + eay * sah + (eax+r);
		//��[
		rgb_tmp1 = *adrs;	
		//����
		for(s=1,adrs+=sah;s<(eav-1);s++,adrs+=sah){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_fuchi3_v2(&rgb_tmp1,adrs,(adrs+sah));
			rgb_tmp1 = *adrs;
		}
		//���[
	}
}
#endif
/*==================================================================*/
/*==================================================================*/
//�֊s�̂ݎ��o��(�J���[��)
static void effect_rinkaku_color(u8 pat)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;
			*adrs = color_white(adrs,pat);
		}
	}
}

/*==================================================================*/
/*==================================================================*/
//�֊s�̂ݎ��o��
static void effect_rinkaku(void)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;
			*adrs = black_white(adrs);
		}
	}
}

/*==================================================================*/
/*==================================================================*/
//�O�֊s�⋭(�P�̂ł͎g�p�����j
static void effect_rinkaku_plus(void)
{
	u8 r,s;
	u16 *adrs;

//������
	for(s=0;s<eav;s++){
		adrs = sa_adrs + (eay+s) * sah + eax;
		//���[
		*adrs = black_fuchi(adrs,(adrs+1));
		//����
		for(r=1,adrs++;r<(eah-1);r++,adrs++){
			*adrs = black_fuchi(adrs,(adrs+1));
			*adrs = black_fuchi(adrs,(adrs-1));
		}
		//�E�[
		*adrs = black_fuchi(adrs,(adrs-1));
	}

//�c����
	for(r=0;r<eah;r++){
		adrs = sa_adrs + eay * sah + (eax+r);

		*adrs = black_fuchi(adrs,(adrs+sah));
		for(s=1,adrs+=sah;s<(eav-1);s++,adrs+=sah){
			*adrs = black_fuchi(adrs,(adrs+sah));
			*adrs = black_fuchi(adrs,(adrs-sah));
		}
		*adrs = black_fuchi(adrs,(adrs-sah));
	}
}

/*==================================================================*/
/*==================================================================*/
//RGB�̊e���Â𔽓]
static void effect_hanten(void)
{
	u8 r,s;
	u16 *adrs;

	for(s=0;s<eav;s++){	//�c�̃Z��
		adrs = sa_adrs + (eay+s) * sah + eax;
		for(r=0;r<eah;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;
			*adrs = rgb_meian_change(adrs);
		}
	}
}

/*==================================================================*/
/*==================================================================*/
//�W��(�|�P�����摜64*64dot��p)
static void effect_awai_poke(void)
{
	u8 r,s;
	u16 *adrs;
	u16 rgb_tmp1;

//���]
	adrs = sa_adrs;
	for(s=0;s<64;s++){
		for(r=0;r<64;r++,adrs++){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_meian_change(adrs);
		}
	}

//�����c�̂�
	for(r=0;r<64;r++){
		adrs = sa_adrs + r;
		//��[
		rgb_tmp1 = *adrs;	
		*adrs = 0x8000;
		//����
		for(s=1,adrs+=64;s<(64-1);s++,adrs+=64){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_fuchi3_v2(&rgb_tmp1,adrs,(adrs+64));
			rgb_tmp1 = *adrs;
		}
		//���[
		*adrs = 0x8000;

		adrs = sa_adrs + r;
		//��[
		rgb_tmp1 = *adrs;	
		*adrs = 0x8000;
		//����
		for(s=1,adrs+=64;s<(64-1);s++,adrs+=64){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_fuchi3_v2(&rgb_tmp1,adrs,(adrs+64));
			rgb_tmp1 = *adrs;
		}
		//���[
		*adrs = 0x8000;
	}

//���]
	adrs = sa_adrs;
	for(s=0;s<64;s++){	//�c�̃Z��
		for(r=0;r<64;r++,adrs++){	//���̃Z��
			if((*adrs)&0x8000) continue;
			*adrs = rgb_meian_change(adrs);
		}
	}
}

/*==================================================================*/
/*==================================================================*/

#if 0			//����͎g�p���Ȃ��̂� 0

//�W��
static void effect_awai(void)
{
	effect_hanten();
	effect_fuchidori_v_v2();
	effect_fuchidori_v_v2();
	effect_hanten();
}

#endif

/*==================================================================*/
/*==================================================================*/
//���ꉏ��艡�̂�
static void effect_fuchidori2_h(void)
{
	u8 r,s;
	u16 rgb_tmp1;
	u16 *adrs;

	for(s=0;s<eav;s++){		//������
		adrs = sa_adrs + (eay+s) * sah + eax;
		//���[
		rgb_tmp1 = *adrs;	
		//����
		for(r=1,adrs++;r<(eah-1);r++,adrs++){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_fuchi2_x(&rgb_tmp1,adrs);
			rgb_tmp1 = *adrs;
		}
		//�E�[
	}
}

/*==================================================================*/
/*==================================================================*/
//���ꉏ���c�̂�
static void effect_fuchidori2_v(void)
{
	u8 r,s;
	u16 rgb_tmp1;
	u16 *adrs;

	for(r=0;r<eah;r++){
		adrs = sa_adrs + eay * sah + (eax+r);

		//��[
		rgb_tmp1 = *adrs;	
		//����
		for(s=1,adrs+=sah;s<(eav-1);s++,adrs+=sah){
			if((*adrs)&0x8000) continue;
			*adrs = rgb_fuchi2_x(&rgb_tmp1,adrs);
			rgb_tmp1 = *adrs;
		}
		//���[
	}
}

/*==================================================================*/
/*            �G�t�F�N�g�֐����̌v�Z�֐�                            */
/*==================================================================*/
#if 0			//����͎g�p���Ȃ��̂� 0
//���G���m�C�Y
static void aburae_noise(u16 no)
{
	u16 *adrs;
	u16 rr,gg,bb;
	u8 p,v;
	u8 i;
	CN d[6];

	d[0].x = rand_tbl_poke[no][0]%eah;	//�ʒuX
	d[0].y = rand_tbl_poke[no][1]%eav;	//�ʒuY

	d[0].sub_col = ((rand_tbl_poke[no][2])>>3)&0x07;	//����
	p = ((rand_tbl_poke[no][2])>>1)&0x03;			//�m�C�Y�p�^�[��
	v = ((rand_tbl_poke[no][2]))&0x01;			//"+"(0),"-"(1)�̕�����

	for(i=1;i<d[0].sub_col;i++){
		if(v==0){	//"+"
			d[i].x = d[0].x - i;
			d[i].y = d[0].y + i;
		}else{		//"-"
			d[i].x = d[0].x + 1;		//��"1"�́A"i"�̊ԈႢ�B
			d[i].y = d[0].y - 1;		//�ł����̕����Y��Ȃ̂ō̗p
		}
		//��ʊO�I�[�o�[�`�F�b�N
		if((d[i].x >= eah)||(d[i].y >= eav)) {	//�I�[�o�[���A
			d[0].sub_col = i - 1;
			break;
		}

		d[i].sub_col = d[0].sub_col - i;
	}

	for(i=0;i<d[0].sub_col;i++){
		adrs = sa_adrs + (d[i].y+eay)*sah + (d[i].x+eax);
		if((*adrs)&0x8000) continue;

		rr = *adrs & 0x001f;
		gg = ((*adrs)>>5) & 0x001f;
		bb = ((*adrs)>>10) & 0x001f;

		switch(p){
		case 0:
		case 1:
			switch((((rand_tbl_poke[no][2])>>3)&0x07)%3){
			case 0:
				if(rr >= d[i].sub_col){
					rr -= d[i].sub_col;
				}else{
					rr = 0;
				}
				break;
			case 1:
				if(gg >= d[i].sub_col){
					gg -= d[i].sub_col;
				}else{
					gg = 0;
				}
				break;
			case 2:
				if(bb >= d[i].sub_col){
					bb -= d[i].sub_col;
				}else{
					bb = 0;
				}
				break;
			}
			break;
		case 2:
		case 3:
			rr += d[i].sub_col;
			gg += d[i].sub_col;
			bb += d[i].sub_col;
			if(rr > 0x001f) rr = 0x001f;
			if(gg > 0x001f) gg = 0x001f;
			if(bb > 0x001f) bb = 0x001f;
			break;
		}

		*adrs = ((bb)<<10) | ((gg)<<5) | rr;
	}
}
#endif

/*==================================================================*/
/*==================================================================*/
//���G���m�C�Y(�|�P�����摜(64*64dot)��p������)
static void aburae_noise_poke(u16 no)
{
	u16 *adrs;
	u16 rr,gg,bb;
	u8 p,v;
	u8 i;
	CN d[6];

	d[0].x = rand_tbl_poke[no][0];	//�ʒuX
	d[0].y = rand_tbl_poke[no][1];	//�ʒuY
	d[0].sub_col = ((rand_tbl_poke[no][2])>>3)&0x07;	//����
	p = ((rand_tbl_poke[no][2])>>1)&0x03;			//�m�C�Y�p�^�[��
	v = ((rand_tbl_poke[no][2]))&0x01;			//"+"(0),"-"(1)�̕�����

	for(i=1;i<d[0].sub_col;i++){
		if(v==0){	//"+"
			d[i].x = d[0].x - i;
			d[i].y = d[0].y + i;
		}else{		//"-"
			d[i].x = d[0].x + 1;		//��"1"�́A"i"�̊ԈႢ�B
			d[i].y = d[0].y - 1;		//�ł����̕����Y��Ȃ̂ō̗p
		}
		//��ʊO�I�[�o�[�`�F�b�N
		if((d[i].x >= 64)||(d[i].y >= 64)) {	//�I�[�o�[���A
			d[0].sub_col = i - 1;
			break;
		}

		d[i].sub_col = d[0].sub_col - i;
	}


	for(i=0;i<d[0].sub_col;i++){
		adrs = sa_adrs + (d[i].y)*PA_H + (d[i].x);
		if((*adrs)&0x8000) continue;

		rr = *adrs & 0x001f;
		gg = ((*adrs)>>5) & 0x001f;
		bb = ((*adrs)>>10) & 0x001f;

		switch(p){
		case 0:
		case 1:
			switch((((rand_tbl_poke[no][2])>>3)&0x07)%3){
			case 0:
				if(rr >= d[i].sub_col){
					rr -= d[i].sub_col;
				}else{
					rr = 0;
				}
				break;
			case 1:
				if(gg >= d[i].sub_col){
					gg -= d[i].sub_col;
				}else{
					gg = 0;
				}
				break;
			case 2:
				if(bb >= d[i].sub_col){
					bb -= d[i].sub_col;
				}else{
					bb = 0;
				}
				break;
			}
			break;
		case 2:
		case 3:
			rr += d[i].sub_col;
			gg += d[i].sub_col;
			bb += d[i].sub_col;
			if(rr > 0x001f) rr = 0x001f;
			if(gg > 0x001f) gg = 0x001f;
			if(bb > 0x001f) bb = 0x001f;
			break;
		}

		*adrs = ((bb)<<10) | ((gg)<<5) | rr;
	}
}

#if 0			//����͎g�p���Ȃ��̂� 0
/*==================================================================*/
/*==================================================================*/
//�J���[�������i1/3,1/3,1/3)
static u16 rgb_comp3(u16 *rgb1,u16 *rgb2,u16 *rgb3)
{
	u16 r,g,b,r2,g2,b2,r3,g3,b3;

	r = *rgb1 & 0x001f;
	g = ((*rgb1)>>5) & 0x001f;
	b = ((*rgb1)>>10) & 0x001f;

	r2 = *rgb2 & 0x001f;
	g2 = ((*rgb2)>>5) & 0x001f;
	b2 = ((*rgb2)>>10) & 0x001f;

	r3 = *rgb3 & 0x001f;
	g3 = ((*rgb3)>>5) & 0x001f;
	b3 = ((*rgb3)>>10) & 0x001f;

	r = (r3 + r2 + r) / 3;
	g = (g3 + g2 + g) / 3;
	b = (b3 + b2 + b) / 3;

	return ((b<<10) | (g<<5) | r);
}
#endif

/*==================================================================*/
/*==================================================================*/
//RGB���O���C�X�P�[���ɕϊ��i32�F�Łj
static u16 rgb_gray_scale(u16 *rgb)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

	r = (r + g + b)/3;

	return ((r<<10) | (r<<5) | r);
}

#if 0			//����͎g�p���Ȃ��̂� 0
/*==================================================================*/
/*==================================================================*/
//RGB���O���C�X�P�[���ɕϊ��i�P�T�F�Łj
u16 rgb_gray_scale2(u16 *rgb)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

	r = ((r + g + b)/3)&0x001e;

	if(r == 0x0002) r = 0x0000;		//�قڍ��Ȃ̂ō��ł܂Ƃ߂�

	return ((r<<10) | (r<<5) | r);
}
#endif

/*==================================================================*/
/*==================================================================*/
//�����ϊ�(�̃J���[�ŁB�����J���[�ցj
static u16 color_white(u16 *rgb,u8 pat)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

	if((r < 0x0011)&&(g < 0x0011)&&(b < 0x0011)) {	//��
		return (black2color(pat));
	}

	return RGB_WHITE;
}
/*==================================================================*/
/*==================================================================*/
#define TANCOL	23
#define FUKUCOL	21
//�����_���ȐF��Ԃ��B
//��pat�ɑ΂��ď�ɓ���
//���ŏ��ɂU�p�^�[���ɑ�܂��ɕ���
//  ���ɍׂ����F�ւ����s���Ă���B
static u16 black2color(u8 pat)
{
	u8 pp;
	u16 r,g,b;

	r = g = b = 0;
	pp = pat/6%3;

	switch(pat%6){
	case 0:
		r = FUKUCOL - pp; g = FUKUCOL - pp; b = 0x0000;
		break;
	case 1:
		r = 0x0000; g = FUKUCOL - pp; b = FUKUCOL - pp;
		break;
	case 2:
		r = FUKUCOL - pp; g = 0x0000; b = FUKUCOL - pp;
		break;
	case 3:
 		r = 0x0000; g = 0x0000; b = TANCOL - pp;
		break;
	case 4:
		r = TANCOL - pp; g = 0x0000; b = 0x0000;
		break;
	case 5:
		r = 0x0000; g = TANCOL - pp; b = 0x0000;
		break;
	}

	return ((r<<10) | (g<<5) | b);
}

/*==================================================================*/
/*==================================================================*/
//�����ϊ�
u16 black_white(u16 *rgb)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

	if((r < 0x0011)&&(g < 0x0011)&&(b < 0x0011)) {	//��
		return RGB_BLACK;
	}

	return RGB_WHITE;
}

/*==================================================================*/
/*==================================================================*/
//�����ϊ�2
static u16 black_fuchi(u16 *rgb,u16 *rgb2)
{
	if(*rgb == 0x0000) return 0x0000;	//���Ȃ̂ŋ����̕K�v���Ȃ��̂�

	if((*rgb)&0x8000) return 0x8000;	//�����F�Ȃ̂Ŗ���

	if((*rgb2)&0x8000) return 0x0000;	//�������F���ŗׂ������̂Ƃ�������

	return (*rgb);
}

/*==================================================================*/
/*==================================================================*/
//���Â𔽓]
static u16 rgb_meian_change(u16 *rgb)
{
	u16 r,g,b;

	r = *rgb & 0x001f;
	g = ((*rgb)>>5) & 0x001f;
	b = ((*rgb)>>10) & 0x001f;

	r = 0x001f - r;
	g = 0x001f - g;
	b = 0x001f - b;

	return ((b<<10) | (g<<5) | r);
}

/*==================================================================*/
/*==================================================================*/
//���ڂ̋���(����Łj
static u16 rgb_fuchi2_x(u16 *rgb1,u16 *rgb2)
{
	u8 c;
	u16 r,g,b,ss;
	u16 d[2][3],s[1][3];

	if(*rgb1 == *rgb2) return *rgb2;  //���ڂ������Ƃ��͖���

	//RGB�֕������Ċi�[
	d[0][0] = *rgb1 & 0x001f;
	d[0][1] = ((*rgb1)>>5) & 0x001f;
	d[0][2] = ((*rgb1)>>10) & 0x001f;

	d[1][0] = *rgb2 & 0x001f;
	d[1][1] = ((*rgb2)>>5) & 0x001f;
	d[1][2] = ((*rgb2)>>10) & 0x001f;

	//���ɋ߂��Ƃ��̓L�����Z��
	if((d[0][0]>0x19)&&(d[0][1]>0x19)&&(d[0][2]>0x19)) return *rgb2;
	if((d[1][0]>0x19)&&(d[1][1]>0x19)&&(d[1][2]>0x19)) return *rgb2;

	//RGB���́A���E�h�b�g�Ƃ̍���(�傫����)���v�Z
	for(c=0;c<3;c++){
		if(d[0][c] > d[1][c]){
			s[0][c] = d[0][c] - d[1][c];
		}else{
			s[0][c] = d[1][c] - d[0][c];		
		}
	}

	//RGB�̒��ň�ԑ傫���l���擾
	if(s[0][0] >= s[0][1]){
		if(s[0][0] >= s[0][2]){
			ss = s[0][0];
		}else{
			if(s[0][1] >= s[0][2]){
				ss = s[0][1];
			}else{
				ss = s[0][2];
			}
		}
	}else{
		if(s[0][1] >= s[0][2]){
			ss = s[0][1];
		}else{
			if(s[0][2] >= s[0][0]){
				ss = s[0][2];
			}else{
				ss = s[0][0];
			}
		}
	}

	r = d[1][0] * (31 - (ss)/2) / 31;
	g = d[1][1] * (31 - (ss)/2) / 31;
	b = d[1][2] * (31 - (ss)/2) / 31;

	return ((b<<10) | (g<<5) | r);
}

/*==================================================================*/
/*==================================================================*/
//���ڂ̋���(����Łj
static u16 rgb_fuchi3(u16 *rgb1,u16 *rgb2,u16 *rgb3)
{
	u16 l,m,rr, ls,rs,ss, r,g,b, n;

	if(((*rgb1)==(*rgb2))&&((*rgb3)==(*rgb2))) return *rgb2;
										//���[�ƈႢ���Ȃ�(�܂���������)

	r = *rgb2 & 0x001f;
	g = ((*rgb2)>>5) & 0x001f;
	b = ((*rgb2)>>10) & 0x001f;

	l = ((*rgb1 & 0x001f) + (((*rgb1)>>5) & 0x001f) + (((*rgb1)>>10) & 0x001f))/3;
	m = (r + g + b)/3;
	rr =((*rgb3 & 0x001f) + (((*rgb3)>>5) & 0x001f) + (((*rgb3)>>10) & 0x001f))/3;

	if((l==m)&&(rr==m)) return *rgb2;		//���[�ƈႢ���Ȃ�(���邳�I��)

	if(l > m){			//���h�b�g�Ƃ̍���
		ls = l - m;
	}else{
		ls = m - l;
	}

	if(rr > m){			//�E�h�b�g�Ƃ̍���
		rs = rr - m;
	}else{
		rs = m - rr;
	}

	if(ls >= rs){	//�����傫��(or����)
		ss = ls;
	}else{
		ss = rs;
	}

	n = 31 - ((ss)>>1);
	r = r * n / 31;
	g = g * n / 31;
	b = b * n / 31;

	return ((b<<10) | (g<<5) | r);
}

/*==================================================================*/
/*==================================================================*/
//���ڂ̋���(����Łj �W���J���[�p�Ɍ��ʂQ�{
static u16 rgb_fuchi3_v2(u16 *rgb1,u16 *rgb2,u16 *rgb3)
{
	u16 l,m,rr, ls,rs,ss, r,g,b, n;

	if(((*rgb1)==(*rgb2))&&((*rgb3)==(*rgb2))) return *rgb2;
										//���[�ƈႢ���Ȃ�(�܂���������)

	r = *rgb2 & 0x001f;
	g = ((*rgb2)>>5) & 0x001f;
	b = ((*rgb2)>>10) & 0x001f;

	l = ((*rgb1 & 0x001f) + (((*rgb1)>>5) & 0x001f) + (((*rgb1)>>10) & 0x001f))/3;
	m = (r + g + b)/3;
	rr =((*rgb3 & 0x001f) + (((*rgb3)>>5) & 0x001f) + (((*rgb3)>>10) & 0x001f))/3;

	if((l==m)&&(rr==m)) return *rgb2;		//���[�ƈႢ���Ȃ�(���邳�I��)

	if(l > m){			//���h�b�g�Ƃ̍���
		ls = l - m;
	}else{
		ls = m - l;
	}

	if(rr > m){			//�E�h�b�g�Ƃ̍���
		rs = rr - m;
	}else{
		rs = m - rr;
	}

	if(ls >= rs){	//�����傫��(or����)
		ss = ls;
	}else{
		ss = rs;
	}

	n = 31 - ss;
	r = r * n / 31;
	g = g * n / 31;
	b = b * n / 31;

	return ((b<<10) | (g<<5) | r);
}

/*==================================================================*/
/*==================================================================*/
