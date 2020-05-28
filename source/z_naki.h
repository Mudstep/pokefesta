
#ifndef _Z_NAKI_H_
#define _Z_NAKI_H_

// �g�`�p
typedef struct {
	u16		ChrAddr;		// �L����(BG_VRAM)�A�h���X
	u8		ScrBlock;		// �X�N���[���E�u���b�N�ԍ�
	u8		PalNo;			// �g�p����p���b�g�ԍ�
	u8		YOffs;			// �x�|�W�V����(�L�����P�ʂŎw��)
	u8		XSize;			// �\�����钷��(�L�����P�ʂŎw��)
}NakigoeHakei;

// ���[�^�[�p
typedef struct {
	u16		ChrAddr;		// �L����(BG_VRAM)�A�h���X
	u8		ScrBlock;		// �X�N���[���E�u���b�N�ԍ�
	u8		PalNo;			// �g�p����p���b�g�ԍ�
	u8		XPos;			// �\���ʒu�w(�L�����P�ʂŎw��)
	u8		YPos;			// �\���ʒu�x(�L�����P�ʂŎw��)
}NakigoeMeter;

typedef NakigoeHakei	HakeiSet;
typedef	NakigoeMeter	MeterSet;

extern	u8		HakeiInit( HakeiSet* hs,u8 bg_num );
extern	void	HakeiMain( u8 bg_num );
extern	void	HakeiStart( u16 pokemon_no );
extern	u8		VuMeterInit( MeterSet* ms,u8 bg_num );
extern	void	VuMeterEnd( void );

extern	u8 ZNakiInitSeq;

#endif

