//=========================== �|�P�b�g�ԍ� ===============================
#include "pocketno.h"	// �����Ɉړ�

//============================== �߂�� ==================================
#define FIELD_POKELIST		1
#define	FIELD_MAIN			2
#define	FIELD_CUBECASE		3
#define	FIELD_BAG			4

#define	BATTLE_POKELIST		1
#define	BATTLE_MAIN			2

//==================== �t�B�[���h�v���O�����A�h���X ======================
extern void CheckFieldCycle(u8);			// ���]�Ԃ��g�p�ł��邩
extern void CheckFieldFishing(u8);			// �ނ�Ƃ��g�p�ł��邩
extern void SetPokeSkillInit(u8);			// �|�P�����ɋZ���o�������鏉���ݒ�
extern void CheckSeedUse(u8);				// �^�l���g�p�ł��邩
extern void SetUseMsg(u8);					// �A�C�e���g�p���b�Z�[�W�\��
extern void CheckZyouroUse(u8);				// ���傤�낪�g�p�\���`�F�b�N
extern void FieldBagMailUseInit(u8);		// ���[�����g�p
extern void FldPokeRecoverInit(u8);			// �񕜃A�C�e��
extern void FldPokePPRecoverInit(u8);		// PP�񕜃A�C�e��
extern void FldPokeWazaPointUpInit(u8);		// �|�C���g�A�b�v
extern void FldPokeListLvUptUpInit(u8);		// �ӂ����ȃA��
extern void FldUseSpray(u8);				// �X�v���[
extern void FldAnanukeInit(u8);				// ���Ȃʂ��̂Ђ�
extern void FldItemCubeCase(u8);			// �L���[�u�P�[�X
extern void FldUseEvolutionItem(u8);		// �i���A�C�e��
extern void FldItemDowsingMachine(u8);		// �_�E�W���O�}�V��
extern void FldItemCoinCase(u8);			// �R�C���P�[�X
extern void FldUseEncountVidro(u8);			// �r�[�h��
extern void FldItemUseRAMKinomi(u8);		// �i�]�̎��i RAM�؂̎� �j
extern void FldPokeAllDethRecoverInit(u8);	// �S���m���񕜃A�C�e��

extern void DummyItemUseFunc(u8);			// �g�p�ł��Ȃ��A�C�e���i���j

#define	ITEMPG_FLD_CYCLE		CheckFieldCycle
#define	ITEMPG_FLD_FISHING		CheckFieldFishing
#define ITEMPG_FLD_SKILL		SetPokeSkillInit
#define ITEMPG_FLD_SEED			CheckSeedUse
#define ITEMPG_FLD_USEMSG		SetUseMsg
#define ITEMPG_FLD_ZYOURO		CheckZyouroUse
#define ITEMPG_FLD_MAIL			FieldBagMailUseInit
#define ITEMPG_FLD_RECOVERY		FldPokeRecoverInit
#define ITEMPG_FLD_PP_RECOVERY	FldPokePPRecoverInit
#define ITEMPG_FLD_W_POINT_UP	FldPokeWazaPointUpInit
#define	ITEMPG_FLD_LIST_LV_UP	FldPokeListLvUptUpInit
#define	ITEMPG_FLD_SPRAY		FldUseSpray
#define	ITEMPG_FLD_ESCAPE		FldAnanukeInit
#define	ITEMPG_FLD_CUBECASE		FldItemCubeCase
#define	ITEMPG_FLD_EVOLUTION	FldUseEvolutionItem
#define	ITEMPG_FLD_DOWSING		FldItemDowsingMachine
#define	ITEMPG_FLD_COINCASE		FldItemCoinCase
#define	ITEMPG_FLD_VIDRO		FldUseEncountVidro
#define ITEMPG_FLD_RAM_KINOMI	FldItemUseRAMKinomi
#define	ITEMPG_FLD_ALLDETH_RCV	FldPokeAllDethRecoverInit

#define	ITEMPG_FLD_DUMMY		DummyItemUseFunc

//====================== �o�g���v���O�����A�h���X ========================
extern void BtlMonsterBallUse(u8);		// �����X�^�[�{�[���i�퓬�j
extern void BtlStRecoverItem(u8);		// �񕜃A�C�e���i�퓬�j
extern void BtlPPRecoverItem(u8);		// PP�񕜃A�C�e���i�퓬�j
extern void BtlBagUseItem(u8);			// �퓬�p�X�e�[�^�X�A�b�v�A�C�e��
extern void BtlItemUseNigaiKinomi(u8);	// �ɂ������̂݁i�퓬�j
extern void BtlEscapeItem(u8);			// �s�b�s�l�`�i�����j�i�퓬�j
extern void BtlItemUseRAMKinomi(u8);	// �i�]�̎��i RAM�؂̎� �j
extern void BtlAllDethRecoverItem(u8);	// �S���m���񕜃A�C�e���i�퓬�j

#define	ITEMPG_BTL_BALL			BtlMonsterBallUse
#define	ITEMPG_BTL_RECOVERY		BtlStRecoverItem
#define	ITEMPG_BTL_PP_RECOVERY	BtlPPRecoverItem
#define	ITEMPG_BTL_STATUS_UP	BtlBagUseItem
#define	ITEMPG_BTL_KONRAN_RCV	BtlItemUseNigaiKinomi
#define	ITEMPG_BTL_ESCAPE		BtlEscapeItem
#define ITEMPG_BTL_RAM_KINOMI	BtlItemUseRAMKinomi
#define	ITEMPG_BTL_ALLDETH_RCV	BtlAllDethRecoverItem
