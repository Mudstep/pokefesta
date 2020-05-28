#ifndef	__FIGHT_EF_H_
#define	__FIGHT_EF_H_

extern	void	EncountEffectSet(u8);

//==========================================================
//
//	MonsOBJ2BG	�|�P����OBJ�L������BG�ɓW�J����
//
// in : posx	�\������BGX�ʒu�i�Z���P�ʂŎw��@0�`31�j
// 		posy	�\������BGY�ʒu�i�Z���P�ʂŎw��@0�`31�j
// 		chr		�W�J����Ă���DecordAdrs�̈����i0�`3�j
// 		pal		�\������L������BG�p���b�g�i���o�[�i0�`15�j
// 		chradrs		�W�J��L����VRAM�A�h���X�i0x0000�`0xffff�j
// 		scrbase	�W�J��X�N���[���x�[�X�u���b�N�i0�`31�j
// 		chrabase	BG�̃L�����x�[�X�u���b�N(0�`3)
//==========================================================

extern void	MonsOBJ2BG(u8 posx,u8 posy,u8 chr,u8 pal,u16 chradrs,u8 scrbase, u8 chrabase);

//==========================================================
//
//	MonsOBJ2BG2	�|�P����OBJ�L������BG�ɓW�J����
//
// in : posx	�\������BGX�ʒu�i�Z���P�ʂŎw��@0�`31�j
// 		posy	�\������BGY�ʒu�i�Z���P�ʂŎw��@0�`31�j
// 		chr		�W�J����Ă���DecordAdrs�̈����i0�`3�j
// 		pat		�A�j���[�V�����̉����ڂ��i0�`?�j
// 		pal		�\������L������BG�p���b�g�i���o�[�i0�`15�j
// 		chradrs		�W�J��L����VRAM�A�h���X�i0x0000�`0xffff�j
// 		scrbase	�W�J��X�N���[���x�[�X�u���b�N�i0�`31�j
// 		chrabase	BG�̃L�����x�[�X�u���b�N(0�`3)
//==========================================================

extern void	MonsOBJ2BG2(u8 posx,u8 posy,u8 chr,u8 pat,u8 pal,u16 chradrs,u8 scrbase, u8 chrabase);

//==========================================================
//
//	BattleMonsOBJ2BG	�|�P����OBJ�L������BG�ɓW�J����(�퓬��p�j
//
// in : posx	�\������BGX�ʒu�i�Z���P�ʂŎw��@0�`31�j
// 		posy	�\������BGY�ʒu�i�Z���P�ʂŎw��@0�`31�j
// 		clientno	�W�J����Ă���DecordAdrs�̈����i0�`3�jClientNo�Ɠ���
// 		pal		�\������L������BG�p���b�g�i���o�[�i0�`15�j
// 
//==========================================================

extern	void	BattleMonsOBJ2BG(u8 posx,u8 posy,u8 clientno,u8 pal);

#define	BATTLE_CHR_BASE	1
#define	BATTLE_SCR_BASE	30

//==========================================================
//	TaskTable��work�̃C���f�b�N�X
//==========================================================

enum{
	EFF_SEQ=0,
	EFF_EFFNO,
	EFF_WORK0,
	EFF_WORK1,
	EFF_WORK2,
	EFF_WORK3,
	EFF_WORK4,
};

#endif	__FIGHT_EF_H_
