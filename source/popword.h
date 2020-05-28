/***********************************************************************
**	���s��f�[�^�i�֐��錾�Ȃǁj
***********************************************************************/
#ifndef _POPWORD_H_
#define _POPWORD_H_

#include "popword_data.h"

/*==============================================
== �萔
==============================================*/
// �\�[�g�^�C�v�iPopWord_Sort() �Ŏg�p�j
enum POPWORD_SORT_TYPE {
	POPSORT_NOWPOWER,
	POPSORT_MAXPOWER,
	POPSORT_MIXER,
};

/*==============================================
== �O���֐��錾
==============================================*/
void PopWord_Init(void);
void PopWord_Update(u16 days);
void PopWord_Sort(PopularWord *word, u16 max, u8 type);
u8   PopWord_Add(u16 *word);
void PopWord_SetBuffer(void);
void PopWordMixer(void *adrs, u32 size, u8 id);

#endif /* #ifndef _POPWORD_H_ */
