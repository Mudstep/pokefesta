//=========================================================================
//
//
//	saveload.c
//
//	2001.05.02	by tama
//
//	2001.11.21	by tama
//		SRAM�̔p�~�A�Q�[���i�s�f�[�^�ƃ{�b�N�X�̃Z�N�^�؂藣���Ȃ�
//=========================================================================

#include "agb.h"
#include "common.h"
#include "intr.h"
#include "../library/AgbFlash1M.h"


//#include "map.h"
//#include "fieldmap.h"
//#include "field.h"
//#include "pasocom.h"
//#include "rtctool.h"
#include "poketool.h"		//PokeParaMine
#include "fld_tool.h"		//SetContinueMapBySpID
#include "evobj.h"			//EvObj

#define	__SAVELOAD_H_GLOBAL__
#include "saveload.h"


//=========================================================================
//		��`
//=========================================================================

typedef struct {
	MINEITEM MyNormalItem[ MY_NORMAL_ITEM_MAX ];	//�莝���̕��ʂ̓���
	MINEITEM MyExtraItem[ MY_EXTRA_ITEM_MAX];		//�莝���̑�؂ȓ���
	MINEITEM MyBallItem[ MY_BALL_ITEM_MAX];			//�莝�����ݽ���ް�
	MINEITEM MySkillItem[ MY_SKILL_ITEM_MAX];		//�莝���̋Z�}�V��
	MINEITEM MySeedItem[ MY_SEED_ITEM_MAX];			//�莝���̂���
	MAIL_DATA MailData[MAIL_STOCK_MAX];
}SAVE_ITEM;

//=========================================================================
//	�Z�[�u�̈�
//=========================================================================

SYS_SAVE_DATA MyData = {};		// �V�X�e���f�[�^

FIELD_SAVE_DATA Fld = {};		// �Q�[���i�s�f�[�^


SAVE_ITEM SaveItem = {};		// �莝���ǂ����ޔ����[�N 


//=========================================================================
//	�ϐ�
//=========================================================================

SAVE_MEMORY_TYPE SaveMemType;

static const u32 SizeOfMyData = sizeof(MyData);
static const u32 SizeOfFld = sizeof(Fld);

static const u32 SizeOfSeedParam = sizeof(SEED_PARAM);
static const u32 SizeOfRamSeed = sizeof(RAM_SEED);
static const u32 SizeOfMail = sizeof(MAIL_DATA);
static const u32 SizeOfPokemonParam = sizeof(PokemonParam);

static const u32 SizeOfSaveItem = sizeof(SaveItem);




//=========================================================================
//
//	�O���[�o���֐�
//
//=========================================================================
//-------------------------------------------------------------------------
//�@�Z�[�u���[�h�V�X�e���̏�����
//-------------------------------------------------------------------------
void SaveLoadSystemInit(void)
{
	if ( IdentifyFlash_1M() == 0 ) {
		SaveMemType = FLASH_1M_TYPE;
		FlashIntrInit();

	} else {
		SaveMemType = NO_MEMORY_TYPE;			// �F�؎��s
	};
}


//=========================================================================
//
//	�Z�[�u��ԃt���O�̃`�F�b�N
//
//=========================================================================
//-------------------------------------------------------------------------
//	����ȃR���e�B�j���[��Ԃ��ǂ����̃`�F�b�N
//	<�߂�l>	1	����A�w��}�b�v�ɑJ�ڂ��ĊJ�n
//				0	�ʏ�
//-------------------------------------------------------------------------
int CheckContinueStatus( void )
{
	return MyData.save_flag;
}

//-------------------------------------------------------------------------
//	����R���e�B�j���[��ԃN���A
//-------------------------------------------------------------------------
void InitContinueStatus( void )
{
	MyData.save_flag = 0;
}

//-------------------------------------------------------------------------
//	����R���e�B�j���[��ԃZ�b�g
//-------------------------------------------------------------------------
void SetContinueStatus( void )
{
	MyData.save_flag = 1;
}

//-------------------------------------------------------------------------
//	�ʐM���[�h�Z�[�u�t���O�Z�b�g
//-------------------------------------------------------------------------
void SetCommSaveFlag( void )
{
	SetContinueMapBySpID(0);
	MyData.save_flag = 1;
}

void ClearCommSaveFlag( void )
{
	MyData.save_flag = 0;
}


//=========================================================================
//
//	�Q�[���f�[�^�ƃe���|�������[�N�Ƃ̃R�s�[����
//
//=========================================================================

//-------------------------------------------------------------------------
//	�莝���|�P�����f�[�^�̑ޔ��E���A
//-------------------------------------------------------------------------
void BackUpPokeParaMine( void )
{
	int i;
	Fld.PokeCountTemoti = PokeCountMine;
	for( i=0; i<TEMOTI_POKEMAX; i++ ) Fld.PokeParaTemoti[i] = PokeParaMine[i];
}

void RecoverPokeParaMine( void )
{
	int i;
	PokeCountMine = Fld.PokeCountTemoti;
	for( i=0; i<TEMOTI_POKEMAX; i++ )PokeParaMine[i] = Fld.PokeParaTemoti[i];
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void BackUpEvObj( void )
{
	int i;
	for ( i=0; i<EV_OBJ_MAX; i++ ) {
		Fld.EvObj[i] = EvObj[i];
	}
}

void RecoverEvObj( void )
{
	int i;
	for ( i=0; i<EV_OBJ_MAX; i++ ) {
		EvObj[i] = Fld.EvObj[i];
	}
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void BackUpGameData( void )
{
	BackUpPokeParaMine();
	BackUpEvObj();
}

void RecoverGameData( void )
{
	RecoverPokeParaMine();
	RecoverEvObj();
}


//-------------------------------------------------------------------------
//	�莝���̂ǂ�����ޔ�
//-------------------------------------------------------------------------
void BackUpTemotiItem(void)
{
	int i;

	for (i=0; i<MY_NORMAL_ITEM_MAX; i++)
		SaveItem.MyNormalItem[i] = Fld.MyNormalItem[i];

	for (i=0; i<MY_EXTRA_ITEM_MAX; i++)
		SaveItem.MyExtraItem[i] = Fld.MyExtraItem[i];

	for (i=0; i<MY_BALL_ITEM_MAX; i++)
		SaveItem.MyBallItem[i] = Fld.MyBallItem[i];

	for (i=0; i<MY_SKILL_ITEM_MAX; i++)
		SaveItem.MySkillItem[i] = Fld.MySkillItem[i];

	for (i=0; i<MY_SEED_ITEM_MAX; i++)
		SaveItem.MySeedItem[i] = Fld.MySeedItem[i];

	for (i=0; i<MAIL_STOCK_MAX; i++)
		SaveItem.MailData[i] = Fld.MailData[i];
}

//-------------------------------------------------------------------------
//	�莝���̂ǂ����𕜋A
//-------------------------------------------------------------------------
void RecoverTemotiItem(void)
{
	int i;

	for (i=0; i<MY_NORMAL_ITEM_MAX; i++)
		Fld.MyNormalItem[i] = SaveItem.MyNormalItem[i];

	for (i=0; i<MY_EXTRA_ITEM_MAX; i++)
		Fld.MyExtraItem[i] = SaveItem.MyExtraItem[i];

	for (i=0; i<MY_BALL_ITEM_MAX; i++)
		Fld.MyBallItem[i] = SaveItem.MyBallItem[i];

	for (i=0; i<MY_SKILL_ITEM_MAX; i++)
		Fld.MySkillItem[i] = SaveItem.MySkillItem[i];

	for (i=0; i<MY_SEED_ITEM_MAX; i++)
		Fld.MySeedItem[i] = SaveItem.MySeedItem[i];

	for (i=0; i<MAIL_STOCK_MAX; i++)
		Fld.MailData[i] = SaveItem.MailData[i];
}

