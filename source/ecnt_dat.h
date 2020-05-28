#ifndef	ECNT_DAT_H
#define ECNT_DAT_H

#include "common.h"
#include "poketool.h"
#include "map_attr.h"
#include "fieldmap.h"
#include "map.h"

#include "monsno.def"				// �|�P�����ԍ�
#include "../evdata/maparray.h"

#define	ENCOUNT_DATA_MAX	0xff

typedef struct {
	u8	MinLv;		// �Œ�Lv
	u8	MaxLv;		// �ō�Lv
	u16	pokeNo;		// �|�P����No
}ENCOUNTPOKE;

typedef struct {
	u8	percent;				// �o����
	const ENCOUNTPOKE * data;	// �|�P�����f�[�^
}ENCOUNTDATA;

typedef struct {
	u8	div_id;		// divID
	u8	map_id;		// mapID
	const ENCOUNTDATA * ground;		// ���n�f�[�^
	const ENCOUNTDATA * water;		// ����f�[�^
	const ENCOUNTDATA * rock;		// ��ӂ��f�[�^
	const ENCOUNTDATA * fish;		// �ނ�f�[�^
}MAPENCOUNT;

// r119 Special Encount
extern u16	SP_ENCOUNT_R119ProgramSym( u8 flg );
extern u16 SpFishingRand(void);
extern void SpFishingSrand( u16 seed );

//================================================================
//	�G���J�E���g����
//================================================================
enum {
	ENCOUNT_ON = 0,
	ENCOUNT_OFF,
};

extern u8 encount_flag;		// �G���J�E���g�t���O	0 = ENCOUNT, 1 = NOT ENCOUNT

extern void EncountFlagSet( u8 flg );


//-------------------------------------------------------------
// �쐫�|�P�����G���J�E���g�`�F�b�N
//	<����>		attr	���݂̃A�g���r���[�g
//				old		���O�̃A�g���r���[�g
//	<�߂�l>	TRUE	�G���J�E���g�C�x���g����
//				FALSE	��������
//-------------------------------------------------------------
extern u8 FieldEncount( u16 attr, u16 old );

//-------------------------------------------------------------
// �X�v���[�����`�F�b�N
//	<�߂�l>	TRUE	�X�v���[���ʐ؂�C�x���g����
//-------------------------------------------------------------
extern u8 SprayCheck(void);

//-------------------------------------------------------------
//	�|�P�����i���o�[�擾�i�����p�j
//	<����>	����G���J�E���g���ǂ�����Ԃ����߂̃t���O�A�h���X
//	<�߂�l>	�|�P�����i���o�[�i�O�̎��̓f�[�^�Ȃ��j
//-------------------------------------------------------------
extern u16 GetEncountVoice(u8 * water_flag);

//-------------------------------------------------------------
//	�|�P�����i���o�[�擾�i�����p�j
//	�����G���J�E���g��p
//	<�߂�l>	�|�P�����i���o�[�i�O�̎��̓f�[�^�Ȃ��j
//-------------------------------------------------------------
extern u16 GetWaterEncountVoice(void);


#endif	// ECNT_DAT_H
