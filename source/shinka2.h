#include	"shinka.def"

//���[�t�B���O�f�[�^�\���̐錾
typedef struct{
//	u8	LineCntB[64];				//�i���O�f�[�^�̍����ɂ�郉�C����
//	u8	SeqNoLeftB[64][32];			//�i���O���C�������̃h�b�g�̃V�[�P���X�i���o�[
//	u8	SeqNoRightB[64][32];		//�i���O���C���E���̃h�b�g�̃V�[�P���X�i���o�[
//	u8	NowLeftPosB[64][32];		//�i���O�ړ����̃��C���̍����f�[�^
//	u8	NowRightPosB[64][32];		//�i���O�ړ����̃��C���̉E���f�[�^
	u8	LineCntA[64];				//�i����f�[�^�̍����ɂ�郉�C����
	u8	SeqNoLeftA[64][32];			//�i���ド�C�������̃h�b�g�̃V�[�P���X�i���o�[
	u8	SeqNoRightA[64][32];		//�i���ド�C���E���̃h�b�g�̃V�[�P���X�i���o�[
	u8	NowLeftPosA[64][32];		//�i����ړ����̃��C���̍����f�[�^
	u8	NowRightPosA[64][32];		//�i����ړ����̃��C���̉E���f�[�^
	u8	EndLeftPosA[64][32];		//�i����ړ��I�����C���̍����f�[�^
	u8	EndRightPosA[64][32];		//�i����ړ��I�����C���̉E���f�[�^
	u16	LeftMoveV[64][32];
	u16	RightMoveV[64][32];
	u16	LeftMoveVTmp[64][32];
	u16	RightMoveVTmp[64][32];
	u32	MorphAdrs[64][32];			//�A�h���X���[�N
	u8	Wait;
}MORPH_DATA2;

#define	MD2		((MORPH_DATA2 *)&UserWork[0x14800])

#ifndef		__SHINKA2_H__
#define		__SHINKA2_H__
extern	void	InitShinka2(PokemonParam *,u16);
#endif
