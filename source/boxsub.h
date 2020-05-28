#ifndef  _BOXSUB_H_
#define  _BOXSUB_H_

/* �t�H���g�`��p�^�[�� */
enum DRAWFONT_TYPE {
DRAWTYPE_16TRANS,
DRAWTYPE_16NOTRANS,
DRAWTYPE_8TRANS,
DRAWTYPE_8NOTRANS,
};

void NtCellSet(CellData *cell);
void BoxDrawFont(const u8 *str, u8 *dst, u8 type, u16 line_size, u8 bgcol, u8 *work_buf);
void BoxDrawFont_32x16(const u8 *str, u8 *dst, u8 type, u8 bg_col, u8 *work);
void FieldBoxMenuRecover(void);
u8 InitBoxCallMenu(void);
u8 GetPasoBoxPokeCount(u8 box);
s16 GetPasoBoxBlankPos(u8 box);
u8 Box_CountMinePoke(void);   // �߹�݂�������
u8 Box_CountMine(void);       // ���܂��ඳ��

u8 Box_CountMinePokeEx(void);
u8 Box_CountMinePokeExSub(u8 no);


void BoxTransMap(u16 *dest, u16 dx, u16 dy, const u16 *src, u16 sx, u16 sy, u16 width, u16 height, u16 src_xblocks);
void BoxClearMap(u16 *addr, u16 x, u16 y, u16 width, u16 height);


//======================================================================
// �̾�Ļ���i�ð����ʂŁA�����߹�̵݂̾�Ă�T���j
//======================================================================
enum NEXT_OFS_TYPE {
	NEXT_OFS,
	PREV_OFS,
	NEXT_OFS_TAMAGO,
	PREV_OFS_TAMAGO,
};
s16  BoxNextOffsetSearch(PokemonPasoParam *array, u8 now_pos, u8 max, u8 type);





#endif /* #ifndef  _BOXSUB_H_ */
