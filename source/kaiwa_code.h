/***********************************************************************
**	�ȈՉ�b�i�J�e�S�����j
**
**	Masao Taya
**
***********************************************************************/
#ifndef _KAIWA_WORD_H_
#define _KAIWA_WORD_H_

/*============================================*/
/*	�P��J�e�S��                              */
/*============================================*/
enum KAIWA_WORD_CATEGORY {
	KWC_POKEMON,    // �|�P����      00
	KWC_TRAINER,    // �g���[�i�[    01
	KWC_STATUS,     // �X�e�[�^�X    02
	KWC_BATTLE,     // �o�g��        03
	KWC_AISATU,     // ��������      04
	KWC_HITO,       // �Ђ�          05
	KWC_KOE,        // ����          06
	KWC_KAIWA,      // ������        07
	KWC_MUSUBI,     // �ނ���        08
	KWC_KIMOTI,     // ������        09
	KWC_JOUTAI,     // ���傤����    0a
	KWC_KOUDOU,     // �����ǂ�      0b
	KWC_SEIKATU,    // ��������      0c
	KWC_SYUMI,      // �����        0d
	KWC_JIKAN,      // ������        0e
	KWC_AREKORE,    // ���ꂱ��      0f
	KWC_KURIKAESI,  // ���肩����    10
	KWC_EVENT,      // �C�x���g      11
	KWC_WAZA1,      // �킴�P        12
	KWC_WAZA2,      // �킴�Q        13
	KWC_NOW,        // �i�E�����Ƃ�  14
	KWC_POKEMON2,   // �|�P�����Q    15

	KWC_MAX,
};
#define KWC_BEGIN   (KWC_POKEMON)
#define KWC_BORDER  (KWC_EVENT)     // �B���J�e�S���̊J�n�ʒu
#define KWC_HIDDEN_CNT  (KWC_MAX-KWC_BORDER)

/*============================================*/
/*	�}�N���E�萔                              */
/*============================================*/
#define CODE_TO_CATEGORY(code)  (((code)>>9)&0x7f)
#define CODE_TO_WORD(code)      ((code)&0x1ff)
#define MAKE_CODE(c,w)          ((((c)&0x7f)<<9)|((w)&0x1ff))

#endif /* #ifndef _KAIWA_WORD_H_ */

