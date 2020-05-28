/**********************************************************************/
/*  �}�b�v�f�[�^�]��                                                  */
/**********************************************************************/
#include "common.h"
#include "maptrans.h"

/**********************************************************************/
/*  �O���[�o���ϐ�                                                    */
/**********************************************************************/
static MAP_TRANS_WORK *wp = NULL;

/**********************************************************************/
/*  �v���g�^�C�v                                                      */
/**********************************************************************/
static void trans_func(MAP_TRANS_PARAM *param);
static void clear_func(MAP_TRANS_PARAM *param);

/*=============================================
==	������
==	work   ���[�N�G���A�̐擪�A�h���X
==	param  �p�����[�^�G���A�̐擪�A�h���X
==	max    �p�����[�^�z��̐�
=============================================*/
void MapTransInit(MAP_TRANS_WORK *work, MAP_TRANS_PARAM *param, u8 max)
{
	wp = work;
	wp->param = param;
	wp->max = max;
	wp->cnt = 0;
}
/*=============================================
==	�}�b�v�f�[�^�]�����s�iV Blank�j
=============================================*/
void MapTrans(void)
{
	if(wp->cnt){
		MAP_TRANS_PARAM *param;
		u16 i;
		for(i = 0; i < wp->cnt; i++){
			param = &(wp->param[i]);
			(*(param->func))(param);
		}
		wp->cnt = 0;
	}
}
/*==============================================
==	�}�b�v�f�[�^�]�����N�G�X�g
==
==	dst            �]����}�b�v�f�[�^�̈�̐擪�A�h���X
==  dx,dy          �]����Z�����W�i0�`29�j
==  src            �]�����}�b�v�f�[�^�A�h���X
==  sx, sy         �]�����Z�����W�i0�`29�j
==  width, height  �]����`�T�C�Y�i0�`29�j
==  src_xcell      �]�����}�b�v�f�[�^�̉��Z����
==
==  return:  �ʏ�1, ���s0
==
==============================================*/
u8 MapTransReq(u16 *dst, u16 dx, u16 dy, const u16 *src, u16 sx, u16 sy, u16 width, u16 height, u16 src_xcell)
{
	MAP_TRANS_PARAM *param;

	if(wp->cnt >= wp->max){ return 0; }

	param = &(wp->param[wp->cnt++]);
	param->size = width * sizeof(u16);
	param->dst = dst + (dy * 32 + dx);
	param->src = src + (sy * src_xcell + sx);
	param->height = height;
	param->xcell = src_xcell;
	param->func = trans_func;

	return 1;
}
/*==============================================
==	�]�����s�֐�
==	param  �]���p�����[�^�\����
==============================================*/
static void trans_func(MAP_TRANS_PARAM *param)
{
	u16 i;
	for(i = 0; i < param->height; i++){
		CpuCopy(param->src, param->dst, param->size, 16);
		param->dst += 32;
		param->src += param->xcell;
	}
}

/*==============================================
==	�}�b�v�f�[�^�[���N���A���N�G�X�g
==
==	dst            �]����}�b�v�f�[�^�̈�̐擪�A�h���X
==  dx,dy          �]����Z�����W�i0�`29�j
==  width, height  �]����`�T�C�Y�i0�`29�j
==
==  return:  �ʏ�1, ���s0
==
==============================================*/
u8 MapClearReq(u16 *dst, u16 dx, u16 dy, u16 width, u16 height)
{
	MAP_TRANS_PARAM *param;

	if(wp->cnt >= wp->max){ return 0; }

	param = &(wp->param[wp->cnt++]);
	param->size = width * sizeof(u16);
	param->dst = dst + (dy * 32 + dx);
	param->height = height;
	param->func = clear_func;

	return 1;
}
/*==============================================
==	�[���N���A���s�֐�
==	param  �]���p�����[�^�\����
==============================================*/
static void clear_func(MAP_TRANS_PARAM *param)
{
	u16 i;
	for(i = 0; i < param->height; i++){
		DIV_DMACLEAR(3, 0, param->dst, param->size, 16);
		param->dst += 32;
	}
}
