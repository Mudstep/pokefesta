/***********************************************************************
**	�ȈՉ�b���́i�O���j
**
**	masao taya
***********************************************************************/
#ifndef _KAIWA_H_
#define _KAIWA_H_

#include "kaiwa_mode.h"



/*=======================================================*/
/* ���͉�ʏ������֐�                                    */
/*                                                       */
/*   mode  ... ���̓��[�h�ikaiwa_mode.h�j                */
/*   addr  ... ��b�R�[�h�i�[�A�h���X                    */
/*   ret_func  ... ��ʕ��A�֐�                          */
/*   interviewer ... �C���^�r���A�[�^�C�v�ikaiwa_mode.h�j*/
/*                                                       */
/*   �� addr �Ɋi�[����Ă���f�[�^��������Ԃɔ��f����  */
/*                                                       */
/*=======================================================*/
void KaiwaInputBegin(u8 mode, u16 *addr, pFunc ret_func, u8 interviewer);


/*=======================================================*/
/* ��b�R�[�h��������ϊ��֐�                            */
/*=======================================================*/
u8 *KaiwaCopyWord(u8 *buf, u16 code);
u8 *KaiwaCopySentence(u8 *buf, u16 *code, u16 x, u16 y);
u16 KaiwaWordLenGet(u16 code);
u8 KaiwaLineOverCheck(const u16 *code, u8 rows, u8 cols, u16 max);

/*=======================================================*/
/* �s�u�p���[�N�������֐�                                */
/*=======================================================*/
void KaiwaTVWorkInit(u8 mode, u16 num);
void KaiwaWorkInit(u16 *work, u16 size);

/*=======================================================*/
/* �i�E�C���ƂΊ֘A                                      */
/*=======================================================*/
u8 NowWordFlagCheck(u8 word_no);
void NowWordFlagSet(u8 word_no);
u16 NowWordGetUnknown(void);
u8 KnownNowWordCnt(void);
u16 NowWordGetKnown(void);

/*=======================================================*/
/* �w��J�e�S�������烉���_���ɒP��P�I��              */
/*=======================================================*/
u16 KaiwaGetRandomWord(u16 category); 
u16 KaiwaGetRandomKnownWord(u16 category);

/*=======================================================*/
/* �w��J�e�S�����̗L���ȒP�ꐔ��Ԃ�                    */
/*=======================================================*/
u16 KaiwaEnableWordCnt(u8 category);

#endif /* #ifndef _KAIWA_H_ */
