/* �V���b�v�Ŏg�p���郏�[�N���܂Ƃ߂����� */
typedef struct{
	pFunc EndFunc;		// �I�����ɌĂ΂�郋�[�`�����i�[
	u16 const * item;	// ���蕨�f�[�^
	u8	max;			// �i��
	u8	pos;
	u8	pos_max;
	u8	scr;
	u8	type;			// �V���b�v�^�C�v
	u8	buy_max;		// ������ő吔


}SHOP_WORK;

enum {
	FRIEND_SHOP = 0,	// �t�����h���B�V���b�v
	GOODS_SHOP,			// �O�b�Y�V���b�v
	FLOWER_SHOP,		// �t�����[�V���b�v
};

extern void ShopRecycleEnd(u8);
extern void ShopNoGoods(u8);
