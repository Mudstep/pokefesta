#ifndef __MAPSCRN_H__
#define __MAPSCRN_H__

//=========================================================================
//
//	MAPSCRN.H
//
//	�}�b�v�L�����A�Z���A�X�N���[���̌^��`
//	
//=========================================================================

#undef GLOBAL
#ifdef MAPSCRN_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================


//------------------------------------------------------------
// �}�b�v�X�N���[���f�[�^�̓���
//  4�r�b�g�F�������
//  2�r�b�g�F�q�b�g�`�F�b�N
// 10�r�b�g�F�Z���i���o�[
//------------------------------------------------------------
#define CELL_HEIGHT_MASK	0xf000
#define	CELL_HITCHECK_MASK	0x0c00
#define CELL_CODE_MASK		0x03ff

#define	CELL_HEIGHT_SHIFT	12
#define CELL_HITCHECK_SHIFT	10

// �}�b�v�̍�����`
#define MAP_HEIGHT_FREE			0x00
#define MAP_HEIGHT_WATER		0x01
#define MAP_HEIGHT_GROUND		0x04
#define MAP_HEIGHT_ROOF			0x07
#define MAP_HEIGHT_HOLD			0x0f

//�͈͊O�Z���̒�`
#define OUTRANGE_CELL	(0xffff&CELL_CODE_MASK)


//------------------------------------------------------------
// �Z���A�g���r���[�g�f�[�^�̓���
// 8�r�b�g�F�ʏ�Z���A�g���r���[�g
// 4�r�b�g�F���g�p
// 4�r�b�g�FBG���f�t���O�i==1�ŉ�2�w�ɔ��f����j
// �c��F���g�p
//------------------------------------------------------------
#define CELLATTR_DATA_MASK	0x00ff
#define CELLATTR_LAYER_MASK	0xf000
#define CELLATTR_LAYER_SHIFT 12


enum{
	MAP_CGX_NORMAL = 0,		/* �񈳏k�}�b�v�f�[�^ */
	MAP_CGX_COMPRESS,		/* ���k�}�b�v�f�[�^ */

	MAP_PLTT_NORMAL = 0,	/* �񈳏k�p���b�g�f�[�^ */
	MAP_PLTT_NORMALSUB,		/* �񈳏k�p���b�g�f�[�^(��풓�j */
	MAP_PLTT_COMPRESS,		/* ���k�p���b�g�f�[�^ */
};

//------------------------------------------------------------
//	�}�b�v�L�����f�[�^
//------------------------------------------------------------
typedef struct {
	u8 char_type;						// CGX�f�[�^�`��
	u8 pltt_type;						// PLTT�f�[�^�`��
	const u8 * CharData;				// CGX�f�[�^�A�h���X
	const u16 * ColorData;				// �p���b�g�f�[�^�A�h���X
	const u16 * CellCharData;			// �Z�����\������L�����̃e�[�u��
	const u16 * CellAttrData;			// �Z���ɑΉ�����A�g���r���[�g�̃e�[�u��
	void (*BGanime)(void);				// BG�A�j���������֐��ւ̃|�C���^
}MapCharData;

//------------------------------------------------------------
//	�}�b�v�X�N���[���f�[�^
//------------------------------------------------------------
typedef struct {
	int Xsize;							// �X�N���[���̂w�T�C�Y
	int Ysize;							// �X�N���[���̂x�T�C�Y
	const u16 * OutRangeCell;			// �͈͊O�Z�����\������f�[�^
	const u16 * ScreenData;				// �X�N���[�����\������Z���f�[�^
	const MapCharData * MainCharData;	// �g�p�풓�L�����f�[�^�̎w��
	const MapCharData * SubCharData;	// �g�p��풓�L�����f�[�^�̎w��
}MapScreenData;



#undef GLOBAL
#endif	/*__MAPSCRN_H__*/

