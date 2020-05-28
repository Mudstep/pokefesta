@*******************************************************************
@		fld_eff.s
@*******************************************************************
	
	.text
	.code 32

	.include	"fld_eff.def"

	.global		FldEffSeqTbl

@===================================================================
@		̨���޴̪�ļ��ݽð���
@===================================================================
FldEffSeqTbl:
	.long	FldEff_Gyoe							@ 00 �т�����ϰ�
	.long	FldEff_IaiGrass						@ 01 ����������(��)
	.long	FldEff_IaiTree						@ 02 ����������(��)
	.long	FldEff_Shade						@ 03 �e
	.long	FldEff_ShortGrassShake				@ 04 ��1
	.long	FldEff_Ripple						@ 05 ������
	.long	FldEff_HidenCutIn					@ 06 ��`�Z��Ĳ�
	.long	FldEff_Ash							@ 07 �D
	.long	FldEff_PokeSwim						@ 08 �g����߹��
	.long	FldEff_HidenPokeSwim				@ 09 ��`�Z�g���
	.long	FldEff_Hokori						@ 10 ����
	.long	FldEff_CaveBase						@ 11 ��n�쐬�i���A) 2001/10/22 by nakahiro
	.long	FldEff_ShortGrassFall				@ 12 ��3
	.long	FldEff_FootPrint					@ 13 ����1
	.long	FldEff_SplashSea					@ 14 ����3
	.long	FldEff_Splash						@ 15 ����1
	.long	FldEff_SplashWater					@ 16 ����2
	.long	FldEff_LongGrassShake				@ 17 ��4
	.long	FldEff_LongGrassFall				@ 18 ��6
	.long	FldEff_ShortGrassEncount			@ 19 ��2(�ݶ���)
	.long	FldEff_LongGrassEncount				@ 20 ��5(�ݶ���)
	.long	FldEff_SandEncount					@ 21 ��3(�ݶ���)
	.long	FldEff_WaterEncount					@ 22 ��3(�ݶ���)
	.long	FldEff_SeedEffect					@ 23 ��̪��
	.long	FldEff_FootPrintDeep				@ 24 ����2
	.long	FldEff_PcRecover					@ 25 �߹�ݴ̪��
	.long	FldEff_TreeBase						@ 26 ��n�쐬�i��) 2001/12/25 by nakahiro
	.long	FldEff_GrassBase					@ 27 ��n�쐬�i��) 2001/12/25 by nakahiro
	.long	FldEff_TrainerHideWood				@ 28 �ڰŰ�B�ꖪ��
	.long	FldEff_TrainerHideRock				@ 29 �ڰŰ�B�ꖪ��
	.long	FldEff_PokeSora						@ 30 ����Ƃ��߹��
	.long	FldEff_HidenPokeSora				@ 31 ��`�Z�����
	.long	FldEff_HidenPokeSoraEnd				@ 32 ��`�Z����ԏI��
	.long	FldEff_Hatena						@ 33 ���ϰ�
	.long	FldEff_Shoal						@ 34 ��
	.long	FldEff_FootPrintCycle				@ 35 ����3
	.long	FldEff_TrainerHideSand				@ 36 �ڰŰ�B�ꖪ��
	.long	FldEff_Iwakudaki					@ 37 ��ӂ�
	.long	FldEff_Anawohoru					@ 38 �����@��
	.long	FldEff_DeepSand						@ 39 �[����
	.long	FldEff_KairikiSet					@ 40 ���̓Z�b�g
	.long	FldEff_SmallGrass					@ 41 ��������
	.long	FldEff_Onsen						@ 42 ����
	.long	FldEff_Takinobir					@ 43 ��o��
	.long	FldEff_Diving						@ 44 �޲��ݸ�
	.long	FldEff_EcSpinBall					@ 45 ��]�ް�
	.long	FldEff_Heart						@ 46 ʰ�ϰ�
	.long	FldEff_FusenGoods					@ 47 ��n�E�����D
	.long	FldEff_DoroGoods					@ 48 ��n�E�D�c�q
	.long	FldEff_GymSand						@ 49 ��э���
	.long	FldEff_Gayser						@ 50 �Ԍ���
	.long	FldEff_Amaikaori					@ 51 �Z�E�Â�����
	.long	FldEff_SndObjGoods					@ 52 ��n�E���̒u��
	.long	FldEff_Awa							@ 53 �A
	.long	FldEff_Kirari						@ 54 �����
	.long	FldEff_CaveBase2					@ 55 ��n�쐬�i���A)
	.long	FldEff_TreeBase2					@ 56 ��n�쐬�i��)
	.long	FldEff_GrassBase2					@ 57 ��n�쐬�i��)
	.long	FldEff_IaiGrass2					@ 58 ����������(��)
	.long	FldEff_HidenCutInMine				@ 59 ��`�Z��Ĳݎ莝��
	.long	FldEff_EvSora						@ 60 �_���p�u����ԁv
	.long	FldEff_BasePC						@ 61 ��n�̃p�\�R��
	.long	FldEff_DendouBall					@ 62 �a�������ް�
	.long	FldEff_Teleport						@ 63 �e���|�[�g

@===================================================================
@		�̪�ľ���ް�
@===================================================================
@---------------------------------------
@	�т�����ϰ�
@---------------------------------------
FldEff_Gyoe:
	SUB_SET			FeGyoeSet
	SET_END

@---------------------------------------
@	����������(���E�J�b�g�C��)
@---------------------------------------
FldEff_IaiGrass:
	SUB_SET			FeGrassCut
	SET_END

@---------------------------------------
@	����������(��)
@---------------------------------------
FldEff_IaiTree:
	SUB_SET			FeTreeCut
	SET_END

@---------------------------------------
@	�e
@---------------------------------------
FldEff_Shade:
	SUB_SET			FeShadeSet
	SET_END

@---------------------------------------
@	�Z�����h��
@---------------------------------------
FldEff_ShortGrassShake:
	PAL_SUB_SET		FeUsePal1,FeShortGrassShakeSet
	SET_END

@---------------------------------------
@	���g��
@---------------------------------------
FldEff_Ripple:
	PAL_SUB_SET		FeUsePal1,FeRippleSet
	SET_END

@---------------------------------------
@	��`�Z������Ĳ�
@---------------------------------------
FldEff_HidenCutIn:
	SUB_SET			FeHidenCutIn
	SET_END

@---------------------------------------
@	�D
@---------------------------------------
FldEff_Ash:
	PAL_SUB_SET		FeUsePal1,FeAshSet
	SET_END

@---------------------------------------
@	�g����߹��
@---------------------------------------
FldEff_PokeSwim:
	SUB_SET			FePokeSwimSet
	SET_END

@---------------------------------------
@	��`�Z �g���
@---------------------------------------
FldEff_HidenPokeSwim:
	SUB_SET			FeHidenPokeSwim
	SET_END

@---------------------------------------
@	����
@---------------------------------------
FldEff_Hokori:
	PAL_SUB_SET		FeUsePal0,FeHokoriSet
	SET_END

@---------------------------------------
@	��n�쐬�i���A�E�J�b�g�C��)
@---------------------------------------
FldEff_CaveBase:
	SUB_SET			FeCaveBase
	SET_END

@---------------------------------------
@	�Z������
@---------------------------------------
FldEff_ShortGrassFall:
	PAL_SUB_SET		FeUsePal1,FeShortGrassFallSet
	SET_END

@---------------------------------------
@	����1
@---------------------------------------
FldEff_FootPrint:
	PAL_SUB_SET		FeUsePal0,FeFootPrintSet
	SET_END

@---------------------------------------
@	����3
@---------------------------------------
FldEff_SplashSea:
	PAL_SUB_SET		FeUsePal0,FeSplashSeaSet
	SET_END

@---------------------------------------
@	����1
@---------------------------------------
FldEff_Splash:
	PAL_SUB_SET		FeUsePal0,FeSplashSet
	SET_END

@---------------------------------------
@	����2
@---------------------------------------
FldEff_SplashWater:
	PAL_SUB_SET		FeUsePal0,FeSplashWaterSet
	SET_END

@---------------------------------------
@	�������h��
@---------------------------------------
FldEff_LongGrassShake:
	PAL_SUB_SET		FeUsePal1,FeLongGrassShakeSet
	SET_END

@---------------------------------------
@	��������
@---------------------------------------
FldEff_LongGrassFall:
	PAL_SUB_SET		FeUsePal1,FeLongGrassFallSet
	SET_END

@---------------------------------------
@	��2
@---------------------------------------
FldEff_ShortGrassEncount:
	PAL_SUB_SET		FeUsePal1,FeShortGrassEncountSet
	SET_END

@---------------------------------------
@	��5
@---------------------------------------
FldEff_LongGrassEncount:
	PAL_SUB_SET		FeUsePal1,FeLongGrassEncountSet
	SET_END

@---------------------------------------
@	��3
@---------------------------------------
FldEff_SandEncount:
	PAL_SUB_SET		FeUsePal0,FeSandEncountSet
	SET_END

@---------------------------------------
@	��3
@---------------------------------------
FldEff_WaterEncount:
	PAL_SUB_SET		FeUsePal0,FeWaterEncountSet
	SET_END

@---------------------------------------
@	��̪��
@---------------------------------------
FldEff_SeedEffect:
	SUB_SET			FeSeedEffectSet
	SET_END

@---------------------------------------
@	����1
@---------------------------------------
FldEff_FootPrintDeep:
	PAL_SUB_SET		FeUsePal0,FeFootPrintDeepSet
	SET_END

@---------------------------------------
@	�߹�݉񕜴̪��
@---------------------------------------
FldEff_PcRecover:
	PAL_SET			FePcRecoverPal
	PAL_SUB_SET		FeUsePal0,FePcRecoverSet
	SET_END

@---------------------------------------
@	��n�쐬�i�؁E�J�b�g�C��)
@---------------------------------------
FldEff_TreeBase:
	SUB_SET			FeTreeBase
	SET_END

@---------------------------------------
@	��n�쐬�i���E�J�b�g�C��)
@---------------------------------------
FldEff_GrassBase:
	SUB_SET			FeGrassBase
	SET_END

@---------------------------------------
@	�ڰŰ�B�ꖪ ��
@---------------------------------------
FldEff_TrainerHideWood:
	SUB_SET			FeTrainerHideWoodSet
	SET_END

@---------------------------------------
@	�ڰŰ�B�ꖪ ��
@---------------------------------------
FldEff_TrainerHideRock:
	SUB_SET			FeTrainerHideRockSet
	SET_END

@---------------------------------------
@	����Ƃ��߹��
@---------------------------------------
FldEff_PokeSora:
	SUB_SET			FePokeSoraSet
	SET_END

@---------------------------------------
@	��`�Z�����
@---------------------------------------
FldEff_HidenPokeSora:
	SUB_SET			FeHidenPokeSora
	SET_END

@---------------------------------------
@	��`�Z����ԏI��
@---------------------------------------
FldEff_HidenPokeSoraEnd:
	SUB_SET			FeHidenPokeSoraEnd
	SET_END

@---------------------------------------
@	���ϰ�
@---------------------------------------
FldEff_Hatena:
	SUB_SET			FeHatenaSet
	SET_END

@---------------------------------------
@	��
@---------------------------------------
FldEff_Shoal:
	PAL_SUB_SET		FeUsePal0,FeShoalSet
	SET_END

@---------------------------------------
@	����3
@---------------------------------------
FldEff_FootPrintCycle:
	PAL_SUB_SET		FeUsePal0,FeFootPrintCycleSet
	SET_END

@---------------------------------------
@	�ڰŰ�B�ꖪ ��
@---------------------------------------
FldEff_TrainerHideSand:
	SUB_SET			FeTrainerHideSandSet
	SET_END

@---------------------------------------
@	��`�Z�E��ӂ�
@---------------------------------------
FldEff_Iwakudaki:
	SUB_SET			FeIwakudaki
	SET_END

@---------------------------------------
@	��`�Z�E���̓Z�b�g
@---------------------------------------
FldEff_KairikiSet:
	SUB_SET			FeKairikiSet
	SET_END

@---------------------------------------
@	�Z�E�����@��
@---------------------------------------
FldEff_Anawohoru:
	SUB_SET			FeAnawohoru
	SET_END

@---------------------------------------
@	�[����
@---------------------------------------
FldEff_DeepSand:
	PAL_SUB_SET		FeUsePal0,FeDeepSandSet
	SET_END

@---------------------------------------
@	��������
@---------------------------------------
FldEff_SmallGrass:
	PAL_SUB_SET		FeUsePal1,FeSmallGrassSet
	SET_END

@---------------------------------------
@	����
@---------------------------------------
FldEff_Onsen:
	PAL_SUB_SET		FeUsePal1,FeOnsenSet
	SET_END

@---------------------------------------
@	�����̂ڂ�
@---------------------------------------
FldEff_Takinobir:
	SUB_SET			FeTakinoboriSet
	SET_END

@---------------------------------------
@	�޲��ݸ�
@---------------------------------------
FldEff_Diving:
	SUB_SET			FeDivingSet
	SET_END

@---------------------------------------
@	��]�ް�
@---------------------------------------
FldEff_EcSpinBall:
	PAL_NW_SET		EcBallPalData,
	SUB_SET			FeEcSpinBallSet
	SET_END

@---------------------------------------
@	ʰ�ϰ�
@---------------------------------------
FldEff_Heart:
	PAL_SUB_SET		FeUsePal0,FeHeartSet
	SET_END

@---------------------------------------
@	��n�E�����D
@---------------------------------------
FldEff_FusenGoods:
	SUB_SET			FeFusenGoods
	SET_END

@---------------------------------------
@	��n�E�D�c�q
@---------------------------------------
FldEff_DoroGoods:
	SUB_SET			FeDoroGoods
	SET_END

@---------------------------------------
@	��э���
@---------------------------------------
FldEff_GymSand:
	PAL_SUB_SET		GymSandPal,FeGymSandSet
	SET_END

@---------------------------------------
@	�Ԍ���
@---------------------------------------
FldEff_Gayser:
	PAL_SUB_SET		GymSandPal,FeGayserSet
	SET_END

@---------------------------------------
@	�Z�E�Â�����
@---------------------------------------
FldEff_Amaikaori:
	SUB_SET			FeAmaikaori
	SET_END

@---------------------------------------
@	��n�E���̒u��
@---------------------------------------
FldEff_SndObjGoods:
	PAL_SUB_SET		SndObjPal,FeSndObjGoods
	SET_END

@---------------------------------------
@	�A
@---------------------------------------
FldEff_Awa:
	PAL_SUB_SET		FeUsePal0,FeAwaSet
	SET_END

@---------------------------------------
@	�����
@---------------------------------------
FldEff_Kirari:
	PAL_SUB_SET		KirariPal,FeKirariSet
	SET_END

@---------------------------------------
@	��n�쐬�i���A)
@---------------------------------------
FldEff_CaveBase2:
	PAL_SUB_SET		CaveBasePal,FeCaveBase2
	SET_END

@---------------------------------------
@	��n�쐬�i��)
@---------------------------------------
FldEff_TreeBase2:
	PAL_SUB_SET		FldBasePal,FeTreeBase2
	SET_END

@---------------------------------------
@	��n�쐬�i��)
@---------------------------------------
FldEff_GrassBase2:
	PAL_SUB_SET		FldBasePal,FeGrassBase2
	SET_END

@---------------------------------------
@	����������(��)
@---------------------------------------
FldEff_IaiGrass2:
	PAL_SUB_SET		GrassPal,FeGrassCut2
	SET_END

@---------------------------------------
@	��`�Z��Ĳݎ莝��
@---------------------------------------
FldEff_HidenCutInMine:
	SUB_SET			FeHidenCutInMine
	SET_END

@---------------------------------------
@	�_���p�u����ԁv
@---------------------------------------
FldEff_EvSora:
	SUB_SET		FeEvSora
	SET_END

@---------------------------------------
@	��n�̃p�\�R��
@---------------------------------------
FldEff_BasePC:
	SUB_SET			FeBasePC
	SET_END

@---------------------------------------
@	�a�������ްٴ̪��
@---------------------------------------
FldEff_DendouBall:
	PAL_SET			FePcRecoverPal
	PAL_SUB_SET		FeDendouScrnPal,FeDendouBallSet
	SET_END

@---------------------------------------
@	�Z�E�e���|�[�g
@---------------------------------------
FldEff_Teleport:
	SUB_SET		FeTeleport
	SET_END
