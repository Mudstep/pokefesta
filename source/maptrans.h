/**********************************************************************/
/*  �}�b�v�f�[�^�]��                                                  */
/**********************************************************************/
#ifndef _TRANS_MAP_H_
#define _TRANS_MAP_H_

#include <agbtypes.h>

/*============================================*/
/*  �]�����s���ɕK�v�ȃp�����[�^              */
/*============================================*/
typedef struct tagMAP_TRANS_PARAM{
	const u16 *src;
	u16 *dst;
	u16 size;
	u16 xcell;
	u16 height;
	void (*func)(struct tagMAP_TRANS_PARAM*);
}MAP_TRANS_PARAM;

/*============================================*/
/*  �]������p���[�N                          */
/*============================================*/
typedef struct {
	MAP_TRANS_PARAM  *param;
	u8 cnt;
	u8 max;
}MAP_TRANS_WORK;

/*============================================*/
/*  �֐��v���g�^�C�v                          */
/*============================================*/
void MapTransInit(MAP_TRANS_WORK *work, MAP_TRANS_PARAM *param, u8 max);
u8 MapTransReq(u16 *dst, u16 dx, u16 dy, const u16 *src, u16 sx, u16 sy, u16 width, u16 height, u16 src_xcell);
u8 MapClearReq(u16 *dst, u16 dx, u16 dy, u16 width, u16 height);
void MapTrans(void);


#endif