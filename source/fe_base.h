//========================================================================================
//									�閧��n�G�t�F�N�g
//									2002/06/04 by nakahiro
//========================================================================================
#ifndef __FE_BASE_H__
#define __FE_BASE_H__

// �Z���R�[�h
#define	GOODS_DUMMY			522		// ��
#define	GOODS_DUMMY2		514		// ��
#define	GOODS_FUSEN1		824		// �Ԑ����D
#define	GOODS_FUSEN1_B		827		// �Ԑ����D�E��
#define	GOODS_FUSEN2		828		// �����D
#define	GOODS_FUSEN2_B		831		// �����D�E��
#define	GOODS_FUSEN3		832		// �������D
#define	GOODS_FUSEN3_B		835		// �������D�E��
#define	GOODS_DORO			552		// �D�c�q
#define	GOODS_DORO_B		555		// �D�c�q�E��
#define	GOODS_SAND_WALL		646		// ��


// �v���g�^�C�v�錾
extern void SetBreakGoodsEffect( s16 cell, s16 px, s16 py );	// �����D�E�D�c�q
extern void BaseBreakDoor( s16 x, s16 y );						// �˂��j��h�A
extern void BaseSoundMat( s16 code );							// ���}�b�g
extern void BaseKiraKiraMat(void);								// ���炫��}�b�g


#endif	/* __FE_BASE_H__ */
