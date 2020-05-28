//=========================================================================
//
//
//	�Q�[���X�^�[�g�ݒ�
//
//
//=========================================================================


#include "common.h"
#include "print.h"
#include "item.h"
#include "fld_tool.h"
#include "calctool.h"
#include "playtime.h"
#include "poketool.h"
#include "ev_flag.h"
#include "seed.h"
#include "zukan.h"
#include "script.h"
#include "sysflag.h"
#include "score.h"			//InitTaisenScore
#include "goods.h"			// 2002/06/21 by nakahiro
#include "my_pc.h"			// 2002/06/28 by nakahiro
#include "mv_poke.h"		// 2002/06/28 by nakahiro
#include "pokelot.h"		// 2002/07/04  taya
#include "pokescr.h"
#include "sxytype.h"
#include "../evdata/maparray.h"
#include "monsno.def"
#include "scr_tool.h"
#include "ev_time.h"
#include "cubecase.h"
#include "svld_ini.h"
#include "svld_sys.h"
#include "rtctool.h"

#define	__INITGAME_H_GLOBAL__
#include "initgame.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------





extern const VM_CODE ev_flag_on[];
extern const VM_CODE ev_flag_game_over_set[];	//
extern const VM_CODE ev_hagi_fune_reset[];		//�n�M�̑D�̈ʒu�𒼂�

extern void GetPocketItem(void);	//�f�o�b�O�p�ɃA�C�e������������
extern void SetDummyCube(void);		// �莝���Ƀ_�~�[�L���[�u�������� 02/02/26 by nakahiro
extern void InitPocketItem(void);	// �o�b�O�̏�����
extern void BagCountInit(void);		// �o�b�O�̃J�E���^������
extern void InitBoxData(void);      // �{�b�N�X�f�[�^������  2001/10/04 taya
extern void InitKaiwaData(void);    // �{�b�N�X�_�~�[�|�P�����f�[�^�Z�b�g 2001/11/29 taya

extern void TVDataAllInit(void);	// �s�u�f�[�^�̏�����
extern void TVCameraAllInit(void);	// �C���^�r���A�[���J�����}���̏�����
extern void InitAllBase(void);			// �閧��n�f�[�^�̏�����

extern void DebugMenuFlagSet(void);	//�f�o�b�O�p���j���[�t���O�Z�b�g

extern void ContestDataInit(void);	//�R���e�X�g�f�[�^������ by matsuda 2001.10.26(��)

extern void RecordInit(void);		//�Q�[�����L�^�J�E���^������

extern void OyajiInit(void);		// ���₶�f�[�^������  taya 2001/12/18
extern void PopWord_Init(void);		// ���s�ꏉ����

extern u16 NowWordGetUnknown(void);	//

extern u8 TemotiPokeAdd(u16 monsno, u8 lv, u16, u8 *, u8 *, u8);
extern void InitPortraitData(void);	//����G�ް�������

extern void TaneOyajiInit(void);	// �^�l�������₶������
extern void FanAllInit(void);		// �t�@����揉����

u8	FirstSaveFlag = 0;			//�ŏ�����Q�[�����͂��߂����ǂ����̃t���O

//#ifdef	PM_DEBUG
u8	DebugMainFlag = 0;	
//#endif

#ifdef	FESTA_VER_ROM
u8	FestaModeFlag = 0;
#endif

//=========================================================================
//	�f�[�^�������T�u���[�`��
//=========================================================================

//-----------------------------------------------------------------
//	ID(32�r�b�g���j���Z�b�g����
//	<����>
//		id_value	ID�i���o�[(u32�̒l)
//		set_pos		�i�[�ꏊ�̃A�h���X(u8�̔z��)
//-----------------------------------------------------------------
void ID_Set( u32 id_value, u8 * set_pos )
{
	set_pos[0] = id_value & 0xff;
	set_pos[1] = (id_value >> 8 ) & 0xff;
	set_pos[2] = (id_value >> 16 ) & 0xff;
	set_pos[3] = (id_value >> 24 ) & 0xff;
}

//-----------------------------------------------------------------
//	ID(u8�~4�j���R�s�[����
//	<����>		to		�R�s�[��A�h���X
//				from	�R�s�[���A�h���X
//-----------------------------------------------------------------
void ID_Copy( u8 * to, u8 * from )
{
	int i;
	for ( i=0;i<4;i++ ) {
		to[i] = from[i];
	}
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static void id_init( void )
{

	u32 id;
	id = ( pp_rand() << 16 ) | pp_rand();
	ID_Set( id, MyData.ID );
}

static void config_init( void )
{
	MyData.msg_speed = 1;	
	MyData.window_type = 0;
	MyData.sound_mode = 0;
	MyData.btl_type = 0;
	MyData.btl_eff_sw = 0;
	MyData.tnavi_zoom = 0;
}

static void zukan_init( void )
{
	ZukanDebugFlag = 0;
	memset( (MyData.Zukan.zukan_get_flag), 0x00, POKE_ZUKAN_LEN );
	memset( (MyData.Zukan.zukan_see_flag), 0x00, POKE_ZUKAN_LEN );
}


static const PORTRAIT_DATA portrait_dmy = {
	0,			//u32 personal_rnd;		//������
	0,			//u32 idno;				//�|�P�����h�c
	0,			//u16 monsno;				//�|�P�����i���o�[
	0,			//u8	pict_type;			//�G�̎��
	{EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
	{EOM_,},	//u8	oyaname[11];		//�e�̖��O
};

static void portrait_init( void )
{
	int i;

	InitPortraitData();
	for ( i=8; i<PORTRAIT_MAX; i++ ) {
		Fld.PortraitData[i] = portrait_dmy;
	}
}

static void BattleTowerWorkInit( void )
{
	CpuClear( 0, &(MyData.BattleTowerWork), sizeof(BATTLE_TOWER_WORK), 32 );
}

#ifdef	PM_DEBUG
//-------------------------------------------------------------
// �}�b�v�ؑ֋@�\�idebug�p�j
//-------------------------------------------------------------
static const s8 DebugMap[][2] = 
{
	{ SP,			SP_TRUCK	},
	{ T102,			T102_PC02	},
	{ FIELD,		FIELD_T101	},
	{ T101,			T101_R0102	},
};

void DebugSetNextMap(void)
{
	u8 no;
	for ( no=0; no<NELEMS(DebugMap); no++ ) {
		if ( DebugMap[no][0] == Fld.MapID.div_id 
				&& DebugMap[no][1] == Fld.MapID.map_id ) break;
	}
	no++;
	if ( no >= NELEMS(DebugMap) ) no = 0;
	NextMapSetDirect( DebugMap[no][0], DebugMap[no][1], NO_EXIT_ID, -1, -1 );
}
#endif

//-------------------------------------------------------------
//	�Q�[���J�n���̃}�b�v�ݒ�
//-------------------------------------------------------------
void InitStartMap(void)
{
	NextMapSetDirect(SP, SP_TRUCK, NO_EXIT_ID, -1, -1);
	SetNextWorldPosition();
}

//=========================================================================
//
//	�v���C���[�V�X�e���Z�[�u�f�[�^�G���A�̃N���A
//
//=========================================================================
void SysSaveDataInit( void )
{
	CpuClear( 0x00, &MyData, sizeof(MyData), 16 );
	config_init();
}

//=========================================================================
//
//	�Q�[���i�s�p���[�N�G���A�̏�����
//	���Z�[�u����Ȃ����[�N�̏�����
//
//=========================================================================
void InitGameWork( void )
{
	FirstSaveFlag = 0;			//�ŏ�����Q�[�����͂��߂����ǂ����̃t���O

#ifdef	PM_DEBUG
	DebugMainFlag = 0;			//�f�o�b�O���[�hOFF(Default)
#endif

#ifdef	FESTA_VER_ROM
	FestaModeFlag = 0;			//�t�F�X�^���[�hOFF
#endif	

	ZukanWorkInit();
	PokeParaMineInit();			//add soga 2001.07.27
	PokeParaEnemyInit();		//add soga 2001.07.27

	BagCountInit();				// 2002/09/06 by nakahiro
}


//=========================================================================
//
//	�ʏ�Q�[���J�n�f�[�^��ݒ�
//
//=========================================================================

void InitGame( void )
{

	//�Z�[�u�f�[�^�����݂����A�Q�[�����J�n�����ꍇ�͂q�s�b���N���A����
	if (SaveDataCheckInfo == SVLD_RET_NULL ||
			SaveDataCheckInfo == SVLD_RET_DESTROY) {
		RTC_Reset();
	}
//	��Z�[�u���[�N�̃N���A

	FirstSaveFlag = 1;			//�ŏ�����Q�[�����͂��߂����ǂ����̃t���O

	PokeParaMineInit();	
	PokeParaEnemyInit();

//	MyData�̃N���A
	ZukanFirstInit();
	BattleTowerWorkInit();

//	Fld�̃N���A
	memset( (void *)&Fld, 0x00, sizeof(Fld) );
	MailWorkInit();

	/* �v���C���[�V�X�e���f�[�^�@*/

	MyData.save_flag = 0;
	id_init();							//�h�c������
	PlayTimeInit();
//	config_init();						//�R���t�B�O���ڏ�����
	zukan_init();						//������t���O������

	/* �Q�[���i�s�f�[�^ */
	EventDataInit();					//�Q�[���i�s�t���O
	TVDataAllInit();					//TV�C�x���g
	TVCameraAllInit();					//�C���^�r���A�[���J�����}��
	InitAllBase();						//�閧��n
	SeedBedInit();						//�^�l
	Fld.my_gold = 3000;					//����
	ContestDataInit();					//�R���e�X�g
	RecordInit();						//�Q�[�����L�^�J�E���g
	portrait_init();
	InitTaisenScore();					//�ΐ퐬�уN���A
	KinokokoSizeOyajiRecordInit();				//���������I���W�L�^������
	NamazoSizeOyajiRecordInit();				//���������I���W�L�^������
	
	/* �|�P�����f�[�^ */
	PokeCountMine = 0;					//�莝���|�P�����Ȃ�
	PokeParaMineInit();					//�莝���|�P�����N���A
	InitBoxData();						//�|�P�����a����{�b�N�X�N���A

	MvPokeDataInit();					// �ړ��|�P�����f�[�^�N���A
	MvPokeMoveDataInit();

	/* �ǂ����f�[�^ */
	Fld.cnv_btn = 0;					//�֗��{�^���o�^�N���A
	InitPocketItem();					//�o�b�O���N���A
	MyPC_DefItemSet();					//����PC�ɃA�C�e�����Z�b�g

	/* �|���b�N�f�[�^ */
	AllCubeParamInit();     // �莝���|���b�N�f�[�^�N���A

	/* �O�b�Y�f�[�^ */
	GoodsAllInit();						// �莝���O�b�Y�N���A

	/* �ȈՉ�b�f�[�^ 2001/11/29 Taya */
	InitKaiwaData();

	/* ���₶�f�[�^ */
	OyajiInit();

	/* ���s��f�[�^ */
	PopWord_Init();

	/* �t�@�����f�[�^ */
	FanAllInit();

	/* �|�P���������f�[�^ */
	InitPokeLot();

	InitStartMap();						//�Q�[���J�n�ʒu�Z�b�g
	DoSpecialScript( ev_flag_on );		// ����X�N���v�g�ŃC�x���g�t���O��ݒ�

}



//=========================================================================
//
//	�f�o�b�O���[�h�ł̃Q�[���J�n��ݒ�
//
//=========================================================================

#ifdef	PM_DEBUG
static const u8 str_foreign_name[] = {
	K__,R__,Ouml_,T__,E__,EOM_
//	M__,I__,N__,I__,K__,R__,Ouml_,T__,E__,EOM_
//	T__,Y__,P__,H__,L__,O__,S__,I__,O__,N__,EOM_
};

extern void SetDummyBoxData(void);  // �{�b�N�X�_�~�[�|�P�����f�[�^�Z�b�g 2001/11/13 taya

void DebugInitGame( int type )
{
	DebugMainFlag = 1;			//�f�o�b�O���[�hOFF

	InitGame();

	/* �v���C���[�V�X�e���f�[�^�@*/
	Fld.my_gold = 999999;							//����

	/* �Q�[���i�s�f�[�^ */
	EventFlagSet( SYS_POKEMON_GET );
	EventFlagSet( SYS_ZUKAN_GET );
	EventFlagSet( SYS_POKEGEAR_GET );
	EventFlagSet( SYS_B_DASH );				//B�{�^���_�b�V������

	/* �|�P�����f�[�^ */
	TemotiPokeAdd(MONSNO_KIMORI, 99,0,NULL,NULL,0 );		// �h���L�[Lv99
	if ( type == DEBUG_START_FOREIGNER ) {
		PokeParaPut(&PokeParaMine[0],ID_nickname, str_foreign_name );
	}

	SetDummyBoxData();  // �{�b�N�X�Ƀ_�~�[�f�[�^���Z�b�g 2001/11/13 taya

	/* �ǂ����f�[�^ */
	GetPocketItem();		// �A�C�e������������
	SetDummyCube();			// �莝���Ƀ_�~�[�L���[�u�������� 02/02/26 by nakahiro

	/* ���₶�f�[�^ */
	EventFlagSet(SYS_NOW_OYAJI_MEET);	// �i�E�C���₶�ɉ����
	NowWordGetUnknown();				// �P�������t���o����

	DebugMenuFlagSet();

	InitTimeEvent();				//���ԃC�x���g�N��
}
#endif	PM_DEBUG

//=========================================================================
//
//	�|�P�����t�F�X�^�p���[�h�ŃQ�[�����J�n
//
//=========================================================================

#ifdef	FESTA_VER_ROM

static const u8 str_festa_male[] = { HI_,RO_,EOM_ };
static const u8 str_festa_female[] = { A_,KI_,EOM_ };

void FestaInitGame( void )
{
	FestaModeFlag = 1;
	InitGame();

												// �����_���Ōl������
	if ( pp_rand() & 0x80 ) {
		PM_strcpy( MyData.my_name, str_festa_male );
		MyData.my_sex = MALE;
	} else {
		PM_strcpy( MyData.my_name, str_festa_female );
		MyData.my_sex = FEMALE;
	}

	AddItem( ITEM_KIZUGUSURI, 20 );				//�莝���̂���������~�Q�O
	AddItem( ITEM_PIIPIIEIDO, 10 );
	AddItem( ITEM_GENKINOKAKERA, 5 );

	TemotiPokeAdd( MONSNO_SAMEHADAA,12,0,NULL,NULL,0 );	//�莝���P�F�T���n�_�[
	TemotiPokeAdd( MONSNO_KAKUREON,14,0,NULL,NULL,0 );	//�莝���Q�F�J�N���I��

	NextMapSetDirect( TEST, TEST_NEWTOWN, -1, 17, 35 );
	SetNextWorldPosition();
}
#endif


//=========================================================================
//
//	�Q�[���I�[�o�[���̃p�����[�^�ݒ�
//
//=========================================================================
extern void PokeAllKaifuku(void);

void RestartGame( void )
{
	DoSpecialScript( ev_flag_game_over_set );

	Fld.my_gold /= 2;
	PokeAllKaifuku();
	InitEventFlagAtGameOver();

	NextMapSetByTelMapID();
	SetNextWorldPosition();
}


//=========================================================================
//
//
//
//=========================================================================

//-----------------------------------------------------------------
//	������ƂԎ��̃t���O���Z�b�g
//-----------------------------------------------------------------
void InitEventFlagAtFlying( void )
{
	InitHeroLastStatus();
	EventFlagReset( SYS_CYCLING_ROAD );
	EventFlagReset( SYS_CRUISE_MODE );
	EventFlagReset( SYS_SAFARI_MODE );
	EventFlagReset( SYS_USE_WAZA_KAIRIKI );
	EventFlagReset( SYS_USE_WAZA_FLASH );
}

//-----------------------------------------------------------------
//	�e���|�[�g���̃t���O���Z�b�g
//-----------------------------------------------------------------
void InitEventFlagAtTeleport( void )
{
	InitHeroLastStatus();
	EventFlagReset( SYS_CYCLING_ROAD );
	EventFlagReset( SYS_CRUISE_MODE );
	EventFlagReset( SYS_SAFARI_MODE );
	EventFlagReset( SYS_USE_WAZA_KAIRIKI );
	EventFlagReset( SYS_USE_WAZA_FLASH );
	DoSpecialScript( ev_hagi_fune_reset );	//�n�M�̑D�̈ʒu�𒼂�
}

//-----------------------------------------------------------------
//	���Ȃʂ����̃t���O���Z�b�g
//-----------------------------------------------------------------
void InitEventFlagAtEscape( void )
{
	InitHeroLastStatus();
	EventFlagReset( SYS_CYCLING_ROAD );
	EventFlagReset( SYS_CRUISE_MODE );
	EventFlagReset( SYS_SAFARI_MODE );
	EventFlagReset( SYS_USE_WAZA_KAIRIKI );
	EventFlagReset( SYS_USE_WAZA_FLASH );
}

//-----------------------------------------------------------------
//	�Q�[���I�[�o�[���̃t���O���Z�b�g
//-----------------------------------------------------------------
void InitEventFlagAtGameOver( void )
{
	InitHeroLastStatus();
	EventFlagReset( SYS_CYCLING_ROAD );
	EventFlagReset( SYS_CRUISE_MODE );
	EventFlagReset( SYS_SAFARI_MODE );
	EventFlagReset( SYS_USE_WAZA_KAIRIKI );
	EventFlagReset( SYS_USE_WAZA_FLASH );
}

//-----------------------------------------------------------------
//	�R���e�B�j���[���̃t���O���Z�b�g
//-----------------------------------------------------------------
void InitEventFlagAtContnue( void )
{
//	EventFlagReset( SYS_CYCLING_ROAD );
//	EventFlagReset( SYS_CRUISE_MODE );
	EventFlagReset( SYS_SAFARI_MODE );
//	EventFlagReset( SYS_USE_WAZA_KAIRIKI );
//	EventFlagReset( SYS_USE_WAZA_FLASH );
	InitFieldVoice();
	CyclingRoadEventInit();
	MvPokeHeroPosSet();
	MvPokeMoveRnd();
}

