#ifndef __CONSCR_H__
#define __CONSCR_H__

//----------------------------------------------------------
//	�R���e�X�g�����N
//----------------------------------------------------------
#define	NORMAL_RANK	0
#define	SUPER_RANK	1
#define	HYPER_RANK	2
#define	MASTER_RANK	3

//----------------------------------------------------------
//	�R���e�X�g�^�C�v
//----------------------------------------------------------
#define	STYLE_CONTEST		0	//�����߹�݂̊i�D�悳
#define	BEAUTIFUL_CONTEST	1	//�����߹�݂̔�����
#define	CUTE_CONTEST		2	//�����߹�݂̉���
#define	CLEVER_CONTEST		3	//�����߹�݂̌���
#define	STRONG_CONTEST		4	//�����߹�݂̂����܂���

//----------------------------------------------------------
//	���p�قɏ����邽�߂̕]���l�̍Œ�_
//----------------------------------------------------------
#define ARTMUSEUM_POINT		800	//������]���l��������Δ��p�قɏ�����

//----------------------------------------------------------
//	�G�̏����閇��
//----------------------------------------------------------
#define MUSEUM_PICTURE_MAX			5	//���p�قɏ�����G�̍ő吔
#define CONTEST_PICTURE_MAX			8	//�R���e�X�g���̊G�̍��v��


#ifndef ASM_CPP		/* �A�Z���u���̏ꍇ�݂̂��̃V���{������`����� */

//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------

extern void ContestSankaCheck(void);//u8 type, u8 rank PokemonParam *pp);
extern void GetBreederName();
extern void GetBreederNickName();
extern void GetPrimaryRank();
extern void GetChampBreederName();
extern void GetChampPokeName();
extern void ContestNijiAppeal(void);
extern void PrimaryExaminationCalc(u8 type);
extern void GetMonsNo();
extern void ContestResultScr(void);
extern void GetChampEntryNumber(void);
extern void GetEntryNo(void);
extern void ContestTuusinInit(u8 list_no);
extern void SetBreederObj();
extern void GetChampObjID(void);
extern void SetBrdName(void);
extern void ArtMuseumPortraitCheck(void);
extern void ArtMuseumPortraitSet(void);
extern void ArtMuseumCondCheck(void);
extern u8 ArtMuseumCountCheck(void);
extern void GetBPBreederData(void);
extern void PortraitPicturePut(void);
extern void ContestSioObjChrSet(void);
extern void GetBPNum(void);
extern void RibbonSet(void);
extern u8 RibbonCheck(void);
extern u8 CheckDebugContestFlag(void);
extern u8 BromideRibbonCheckSet(void);
extern u8 ContestServerVisionCheck(u8 version[]);
extern void ContestPokeCGWinOpen(void);
extern void ContestPokeCGWinClose( void );
extern void ContestMusSet(void);
extern void GetContRnd(void);


#endif	/*ASM_CPP*/

#endif	/*__CONSCR_H__*/

