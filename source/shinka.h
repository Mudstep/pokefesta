#include	"shinka.def"

//���[�t�B���O�f�[�^�\���̐錾
typedef struct{
	u8	BeforeMonsActor;
	u8	AfterMonsActor;
	u8	ShinkaTaskNo;
	u8	Wait;
	u8	BeforeLineCnt[64];			//�i���O�̃f�[�^�̍����ɂ�郉�C����
	u8	AfterLineCnt[64];			//�i����̃f�[�^�̍����ɂ�郉�C����
	u8	MoveLineCnt[64];			//�ړ����郉�C���J�E���^�[
	u8	SeqNoLeft[64][32];			//���C�������̃h�b�g�̃V�[�P���X�i���o�[
	u8	SeqNoRight[64][32];			//���C���E���̃h�b�g�̃V�[�P���X�i���o�[
	u8	BeforeLeftPos[64][32];		//�i���O�̃f�[�^�̃��C���̍����i���o�[
	u8	BeforeRightPos[64][32];		//�i���O�̃f�[�^�̃��C���̉E���i���o�[
	u8	AfterLeftPos[64][32];		//�i����̃f�[�^�̃��C���̍����i���o�[
	u8	AfterRightPos[64][32];		//�i����̃f�[�^�̃��C���̉E���i���o�[
	u8	NowLeftPos[64][32];			//�ړ����̃��C���̍����f�[�^
	u8	NowRightPos[64][32];		//�ړ����̃��C���̉E���f�[�^
	u32	MorphAdrs[64][32];			//�A�h���X���[�N
	u16	NowLeftV[64][32];			//
	u16	NowRightV[64][32];			//
	u16	NowLeftVTmp[64][32];		//
	u16	NowRightVTmp[64][32];		//
	u8	TotalWait;
}MORPH_DATA;
//size=0xa061byte
//
#define	MD		((MORPH_DATA *)&UserWork[0x14800])

#ifndef		__SHINKA_H__
#define		__SHINKA_H__
extern	pFunc	ShinkaReturnProc;
extern	void	InitShinka(PokemonParam *,u16,u8,u8);
extern	void	InitShinkaAct(PokemonParam *,u16,u8,u8);
extern	void	InitShinkaTuushin(PokemonParam *,u16,u8,u8);
#endif
