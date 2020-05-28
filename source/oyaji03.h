//=========================================================================================
//									���T�C�N�����₶
//								2002/02/18 by nakahiro
//=========================================================================================
#ifndef __OYAJI03_H__
#define __OYAJI03_H__


#define	RECYCLE_OYAJI_MAX	4
#define	RECYCLE_OYAJI_NAME	11		// ���O�̃T�C�Y��7+1�����ǂƂ肠�����E�E�E

typedef struct {
	u8	type;
	u8	goods[ RECYCLE_OYAJI_MAX ];
	u8	name[ RECYCLE_OYAJI_NAME * RECYCLE_OYAJI_MAX ];
	u8	flg;
}OYAJIDATA_RECYCLE;

extern void RecycleBoxIn(u8);			// ���T�C�N���{�b�N�X�փO�b�Y������
extern void InitRecycleWinSelect(void);	// ���T�C�N���O�b�Y�I��E�ݒ�
extern void ResetRGGiveFlag(void);		// �O�b�Y���������t���O������

extern void TradeOyajiMyGoodsSet( u8 id );		// ��������O�b�Y��I��
extern void TradeOyajiMyGoodsCancel( u8 id );	// ��������O�b�Y��I�΂Ȃ�����


#endif //__OYAJI03_H__
