//**********************************************************************
//		fld_eff.h
//**********************************************************************
#ifndef	FLD_EFF_H
#define	FLD_EFF_H

#undef	EXT
#ifdef 	FLD_EFF_H_PROTO
#define	EXT	/**/
#else
#define	EXT extern
#endif

/*------------------------------------------*/
/*	̨���޴̪�� �Ăяo�� ����				*/
/*------------------------------------------*/
#define	FECODE_Gyoe					0x00				/* 001 �ޮ��				*/
#define	FECODE_IaiGrass				0x01				/* 001 ����������(��)		*/
#define	FECODE_IaiTree				0x02				/* 002 ����������(��)		*/
#define FECODE_Shade				0x03				/* 003 �e					*/
#define FECODE_ShortGrassShake		0x04				/* 004 �Z�����h��			*/
#define FECODE_Ripple				0x05				/* 005 ������				*/
#define FECODE_HidenCutIn			0x06				/* 006 ��`�Z��Ĳ�			*/
#define FECODE_Ash					0x07				/* 007 �D					*/
#define FECODE_PokeSwim				0x08				/* 008 �g����߹��			*/
#define FECODE_HidenPokeSwim		0x09				/* 009 ��`�Z�g���			*/
#define FECODE_Hokori				0x0a				/* 010 ����					*/
#define FECODE_CaveBase				0x0b				/* 011 ��n�쐬�i���A�j		*/
#define FECODE_ShortGrassFall		0x0c				/* 012 �Z������				*/
#define FECODE_FootPrint			0x0d				/* 013 ����					*/
#define FECODE_SplashSea			0x0e				/* 014 ����3				*/
#define FECODE_Splash				0x0f				/* 015 ����1				*/
#define FECODE_SplashWater			0x10				/* 016 ����2				*/
#define FECODE_LongGrassShake		0x11				/* 017 �������h��			*/
#define FECODE_LongGrassFall		0x12				/* 018 ��������				*/
#define FECODE_ShortGrassEncount	0x13				/* 019 �Z�����ݶ���			*/
#define FECODE_LongGrassEncount		0x14				/* 020 �������ݶ���			*/
#define FECODE_SandEncount			0x15				/* 021 ���ݶ���				*/
#define FECODE_WaterEncount			0x16				/* 022 ���ݶ���				*/
#define FECODE_SeedEffect			0x17				/* 023 ��̪��				*/
#define FECODE_FootPrintDeep		0x18				/* 024 ����					*/
#define FECODE_PcRecover			0x19				/* 025 �߹�݉񕜴̪��		*/
#define FECODE_TreeBase				0x1a				/* 026 ��n�쐬�i�؁j		*/
#define FECODE_GrassBase			0x1b				/* 027 ��n�쐬�i���j		*/
#define FECODE_TrainerHideWood		0x1c				/* 028 �ڰŰ�B�ꖪ��		*/
#define FECODE_TrainerHideRock		0x1d				/* 029 �ڰŰ�B�ꖪ��		*/
#define FECODE_PokeSora				0x1e				/* 030 ����Ƃ��߹��		*/
#define FECODE_HidenPokeSora		0x1f				/* 031 ��`�Z�����		*/
#define	FECODE_HidenPokeSoraEnd		0x20				/* 032 ��`�Z����ԏI��	*/
#define FECODE_Hatena				0x21				/* 033 ���ϰ�				*/
#define	FECODE_Shoal				0x22				/* 034 ��					*/
#define FECODE_FootPrintCycle		0x23				/* 035 ����					*/
#define FECODE_TrainerHideSand		0x24				/* 036 �ڰŰ�B�ꖪ��		*/
#define FECODE_Iwakudaki			0x25				/* 037 ��`�Z�E��ӂ�		*/
#define FECODE_Anawohoru			0x26				/* 038 �Z�E�����@��			*/
#define FECODE_DeepSand				0x27				/* 039 �[����				*/
#define FECODE_KairikiSet			0x28				/* 040 ��`�Z�E���̓Z�b�g	*/
#define FECODE_SmallGrass			0x29				/* 041 ��������				*/
#define FECODE_Onsen				0x2a				/* 042 ����					*/
#define FECODE_Takinobori			0x2b				/* 043 ��o��				*/
#define FECODE_Diving				0x2c				/* 044 �޲��ݸ�				*/
#define FECODE_EcActBall			0x2d				/* 045 �ݶ����ް�			*/
#define FECODE_Heart				0x2e				/* 046 ʰ�ϰ�				*/
#define FECODE_Fusen				0x2f				/* 047 ��n�E�����D			*/
#define FECODE_Doro					0x30				/* 048 ��n�E�D�c�q			*/
#define FECODE_GymSand				0x31				/* 049 ��э���				*/
#define FECODE_Gayser				0x32				/* 050 �Ԍ���				*/
#define FECODE_Amaikaori			0x33				/* 051 �Z�E�Â�����			*/
#define	FECODE_SndObjGoods			0x34				/* 052 ��n�E���̒u��		*/
#define FECODE_Awa					0x35				/* 053 �A					*/
#define FECODE_Kirari				0x36				/* 054 �����				*/
#define FECODE_CaveBase2			0x37				/* 055 ��n�쐬�i���A�j		*/
#define FECODE_TreeBase2			0x38				/* 056 ��n�쐬�i�؁j		*/
#define FECODE_GrassBase2			0x39				/* 057 ��n�쐬�i���j		*/
#define	FECODE_IaiGrass2			0x3a				/* 058 ����������(��)		*/
#define FECODE_HidenCutInMine		0x3b				/* 059 ��`�Z��Ĳݎ莝��	*/
#define FECODE_EvSora				0x3c				/* 060 �_���p�u����ԁv	*/
#define FECODE_BasePC				0x3d				/* 061 ��n�̃p�\�R��		*/
#define FECODE_DendouBall			0x3e				/* 062 �a�������ް�			*/
#define FECODE_Teleport				0x3f				/* 063 �Z�E�e���|�[�g		*/

#define FECODE_END					0xff				/* 255 �I������				*/

#ifndef ASM_CPP											/* C ���					*/
//------------------------------------------
//	̨������گ�ID �K��l
//------------------------------------------
#define	FLD_PalID_FE_START		0x1000					// ̨���޴̪�ėp��گ�ID
#define	FLD_PalID_FE_END		0x10ff

#define	FLD_PalID_EVOBJ_START	0x1100					// ̨����OBJ�p��گ�ID
#define	FLD_PalID_EVOBJ_END		0x11ff

#define	FLD_PalID_WEATHER_START	0x1200					// �V��p��گ�ID
#define	FLD_PalID_WEATHER_END	0x12ff

//------------------------------------------
//	̨���޴̪�� ��گ� ID
//------------------------------------------
enum
{
	FE_Grass_PalID = FLD_PalID_FE_START,				// ��
	FE_StandHero_PalID,									// ����˰۰
	FE_StandHeroine_PalID,								// �����۲�
	FE_CaveBase_PalID,									// ��n�쐬(���A)2001/10/22 by nakahiro
	FE_Use0_PalID,										// ̨���޴̪�Đ�p��گ�0
	FE_Use1_PalID,										// ̨���޴̪�Đ�p��گ�1
	FE_StandOdamaki_PalID,								// �������m
	FE_PcRecover_PalID,									// �߹�݉񕜴̪��
	FE_FldBase_PalID,									// ��n�쐬�@2001/12/25 by nakahiro
	FE_EcBall_PalID,									// �ް�
	FE_EcTrainer_PalID,									// �ڰŰ
	FE_FusenGoods_PalID,								// ��n�E�����D
	FE_DoroGoods_PalID,									// ��n�E�D�c�q
	FE_GymSand_PalID,									// ��э���,�Ԍ���
	FE_SndObjGoods_PalID,								// ��n�E���̒u��
	FE_Kirari_PalID,									// �����
	FE_DendouScrn_PalID,								// �a������
};

//------------------------------------------
//	̨���� �� ID �K��l
//------------------------------------------
#define	FLD_CellID_FE_START			0x1000				// ̨���޴̪�ėp��ID
#define	FLD_CellID_FE_END			0x10ff

#define	FLD_CellID_EVOBJ_START		0x1100				// �����OBJ�p��ID
#define	FLD_CellID_EVOBJ_END		0x11ff

#define FLD_CellID_WEATHER_START	0x1200				// �V��p��ID
#define FLD_CellID_WEATHER_END		0x12ff

//------------------------------------------
//	̨���޴̪�� �� ID
//------------------------------------------
enum
{
	FE_DMY_CELLID = FLD_CellID_FE_START,
};

//------------------------------------------
//	��۰���
//------------------------------------------
EXT s32	FeSetWork[8];									// ���ܰ�

//------------------------------------------
//	�֐���`
//------------------------------------------
EXT	u32	FldEffSet(u8);									// �̪�ČĂяo��

EXT void FeDelAct(actWork*);							// �̪�ı����폜
EXT void FeDelActCode(actWork*,u8);						// �����폜,���ލ폜

EXT void FeCodeInit(void);								// �̪�ĺ��ޏ�����
EXT void FeCodeSet(u8);									// �̪�ĺ��޾��
EXT void FeCodeDel(u8);									// ���ލ폜
EXT u8 FeCodeChk(u8);									// �̪�ĺ�������

EXT u8 SetStandTrainerActor(u8,s16,s16,u8,void*);		// �����p�ڰŰ�������
EXT void SetStandTrainerVram(u8,u16,void*);				// �����p�ڰ�VRAM���
EXT u8 StandOdamakiSet(s16,s16,u8);						// ���m����������

EXT u8 FePokeSet(u16,s16,s16,u8);						// �߹��OBJ���
EXT u8 FePokeRareSet(u16,u32,u32,s16,s16,u8);			// �߹��OBJ���
EXT void FePokeDel(actWork*);							// �߹��OBJ�폜

EXT void ColorFadeUp(u16,u8,u8,u8);						// �װ̪��ޱ���
EXT void ColorFadeDown(u16,u8,u8,u8);					// �װ̪����޳�

EXT void SetFieldFlyInStart(void);
EXT void FieldInitFallTask(void);						// ���Ƃ��������ŏ��̏���
EXT void FieldInitWarpTask(void);						// ���[�v��̏���

EXT void SetMapChangeEscalatorTask(u8,u8);				// ���ڰ��ړ�����������
EXT void SetMapChangeGayser(u8);						// �Ԍ���������
EXT void SetMapChangeHidePitFall(u8);					// �B����������
EXT void SetMapChangeAnanuke(void);						// ������������
EXT void SetMapChangeTeleport(void);					// ���߰�������

EXT void FeGayserActMove(actWork*);						// �Ԍ�����
EXT void FeGymSandActMove(actWork*);					// ��э�������

#endif	// ASM_CPP
#undef	EXT
#endif	// FLD_EFF_H

