/***********************************************************************
**	�ȈՉ�b�i�a�f����F�w�b�_�j
**
**	Masao Taya
**
***********************************************************************/
#ifndef _KAIWA_BG_H_
#define _KAIWA_BG_H_

/*==============================================
==	�\����
==============================================*/
typedef struct {
	u16 left, right, top, bottom;
	s16 al, ar, at, ab;
}TRECT;

typedef struct {
	u16 left, top;
	s16  al, at;
	const u16 *addr;
	u16 xblocks;
}SRC_ORG;

/*==============================================
==	�v���g�^�C�v
==============================================*/
void InitTransRect(TRECT *rect, u16 left, u16 top, u16 right, u16 bottom, u16 al, u16 at, u16 ar, u16 ab);
void ProgTransRect(TRECT *rect);
void InitSrcOrg(SRC_ORG *src, u16 left, u16 top, u16 al, u16 at, const u16* addr, u16 xblocks);
void ProgSrcOrg(SRC_ORG *src);
void RectTransMap(u16 *dest, TRECT *area, SRC_ORG* src);
void RectClearMap(u16 *dest, TRECT *area);


#endif /* #ifndef _KAIWA_BG_H_ */

