
#ifndef	__SOFTFADE_H__
#define	__SOFTFADE_H__


//;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	�t�F�[�h�֌W�}�N��
//;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//----------------------------------------------------------
// �w�肳�ꂽ��گĂ�VRAM��̱��ڽ����ԁA�΁A�̐F�����o��
//	col_adr = ���o����گĂ�VRAM��̱��ڽ
//----------------------------------------------------------
#define RED_DATA(color) 	((u8)(color & 31))
#define GREEN_DATA(color)	((u8)((color & 0x3E0) >> 5))
#define BLUE_DATA(color)	((u8)((color & 0x7C00) >> 10))

//----------------------------------------------------------
// col_dest = �Z�b�g����ꏊ(u16)
// col = �Z�b�g�����ް�
//----------------------------------------------------------
#define RED_SET(col_dest, col)		\
{									\
	col_dest &= 0x7FE0;				\
	col_dest |= (u16)col;			\
}

#define GREEN_SET(col_dest, col)	\
{									\
	col_dest &= 0x7C1F;				\
	col_dest |= ((u16)col << 5);	\
}

#define BLUE_SET(col_dest, col)		\
{									\
	col_dest &= 0x03FF;				\
	col_dest |= ((u16)col << 10);	\
}

//----------------------------------------------------------
//		�t�F�[�h�v�Z
// before_color = ���̐F
// after_color = �ύX��̐F
// evy = �W��(�ύX�̓x�����B0�`16)
//----------------------------------------------------------
#define FADE_CHENJI(before_color, after_color, evy)			\
	(before_color+((after_color - before_color) * evy >> 4))
	//(before_color+(after_color - before_color) * evy/16)



#endif	//__SOFTFADE_H__

