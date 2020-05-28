//**********************************************************************
//		mystatus.c
//		��l���ð�����
//**********************************************************************
#define		MYSTATUS_H_PROTO
#include	"common.h"
#include	"intr.h"
#include	"actor.h"
#include	"actanm.h"
#include	"palanm.h"
#include	"menu.h"
#include	"task.h"
#include	"fld_eff.h"
#include	"mystatus.h"
#include	"zukan.h"
#include	"pm_str.h"
#include	"fld_eff.h"
#include	"sysflag.h"
#include	"ev_flag.h"
#include	"pm_str.h"
#include	"laster.h"
#include	"calctool.h"
#include	"kaiwa.h"
#include	"multisio.h"
#include	"calctool.h"
#include	"record.h"
#include	"mus_tool.h"
#include	"conscr.h"
#include	"map.h"
#include	"score.h"

//----------------------------------------------------------------------
//		���ޯ��
//----------------------------------------------------------------------
#define BUTTON_ACTOR_ON 0								// ON = ���ݱ����N��

//----------------------------------------------------------------------
//		�O���Q��
//----------------------------------------------------------------------
extern u8 * ExCopyMapName( u8 *, u16 );

//----------------------------------------------------------------------
//		��۰���ܰ�
//----------------------------------------------------------------------
TRCARD MultiTrainerCard[MULTI_SIO_PLAYERS_MAX] =		// �ʐM�p�ڰŰ����ܰ�
{
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		{0,0,0,0},
		{0,0,0,0,0,0,0,0}
	},
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		{0,0,0,0},
		{0,0,0,0,0,0,0,0}
	},
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		{0,0,0,0},
		{0,0,0,0,0,0,0,0}
	},
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		{0,0,0,0},
		{0,0,0,0,0,0,0,0}
	}
};

//----------------------------------------------------------------------
//		�ݸٰ��
//----------------------------------------------------------------------
#include	"..\objdata\status.ach"
#include	"..\objdata\l_badge.ach"

#if BUTTON_ACTOR_ON
#include	"..\objdata\st_menu.ach"
#endif

//----------------------------------------------------------------------
//		define
//----------------------------------------------------------------------
#define BG_C_CHNO_MAX	(0xa4)							// BG ���޷�א�
#define BG_C_CHDATA_MAX (BG_C_CHNO_MAX<<5)				// BG ���޷�׻���
#define BG_B_CHNO_MAX	(0x20)							// BG �ޯ�޷�א�
#define BG_B_CHDATA_MAX (BG_B_CHNO_MAX<<5)				// BG �ޯ�޷�׻���
#define BG_CHNO_MAX		(BG_C_CHNO_MAX+BG_B_CHNO_MAX)	// BG ����,�ޯ�޷�׻���
#define BG_CHDATA_MAX (BG_C_CHDATA_MAX+BG_B_CHDATA_MAX) // BG ����,�ޯ�޷�׻���
#define BG_CHNO_0		(0x00)							// BG ������ވʒu
#define BG0_CH_BASE		(BG_VRAM+0xc000)				// BG0 ����ް�
#define BG1_CH_BASE		(BG_VRAM)						// BG1 ����ް�
#define BG2_CH_BASE		(BG1_CH_BASE)					// BG2 ����ް�
#define BG3_CH_BASE		(BG1_CH_BASE)					// BG3 ����ް�
#define BG0_MAP_BASE	(BG_VRAM+0xb800)				// BG0 ϯ���ް�
#define BG1_MAP_BASE	(BG_VRAM+0x4000)				// BG1 ϯ���ް�
#define BG2_MAP_BASE	(BG1_MAP_BASE+0x800)			// BG2 ϯ���ް�
#define BG3_MAP_BASE	(BG2_MAP_BASE+0x800)			// BG3 ϯ���ް�
#define BG0_CH_BASE_NO	((BG0_CH_BASE-BG_VRAM)>>14)		// BG0 ����ް�����(no/4000h)
#define BG1_CH_BASE_NO	((BG1_CH_BASE-BG_VRAM)>>14)		// BG1 ����ް�����
#define BG2_CH_BASE_NO	((BG2_CH_BASE-BG_VRAM)>>14)		// BG2 ����ް�����
#define BG3_CH_BASE_NO	((BG3_CH_BASE-BG_VRAM)>>14)		// BG3 ����ް�����
#define BG0_MAP_BASE_NO	((BG0_MAP_BASE-BG_VRAM)>>11)	// BG0 ����ް�����(no/800h)
#define BG1_MAP_BASE_NO	((BG1_MAP_BASE-BG_VRAM)>>11)	// BG0 ����ް�����
#define BG2_MAP_BASE_NO	((BG2_MAP_BASE-BG_VRAM)>>11)	// BG0 ����ް�����
#define BG3_MAP_BASE_NO	((BG3_MAP_BASE-BG_VRAM)>>11)	// BG0 ����ް�����

#define BG_BADGE_CH_BASE (BG1_CH_BASE+BG_C_CHDATA_MAX)	// BG �ޯ�޷���ް�
#define BG_BADGE_CHNO_0  (BG_CHNO_0+BG_C_CHNO_MAX)		// BG �ޯ�޷�����ވʒu
#define BG_BADGE_PALNO   (3)							// BG �ޯ����گ�����

#define BG_HERO_CHNO_MAX	(0x40)						// BG ˰۰��א�
#define BG_HERO_CHDATA_MAX	(BG_HERO_CHNO_MAX<<5)		// BG ˰۰��׻���
#define BG_HERO_CH_BASE		(BG1_CH_BASE+BG_CHDATA_MAX)	// BG ˰۰����ް�
#define BG_HERO_CHNO_0		(BG_CHNO_0+BG_CHNO_MAX)		// BG ˰۰������ވʒu
#define BG_HERO_PALNO		(0x05)						// BG ˰۰��گ�����

#define BG_STAR_CHNO		(0x8f)						// ���������
#define BG_STAR_PALNO		(0x04)						// ����گ�

#define FONT_BLACK_PALNO	(PA_BG15)					// ��������گ�
#define FONT_RED_PALNO		(PA_BG14)					// �ԕ�����گ�

#define BG_CARD_BASE_NO		(0x01)						// �����ް��������
#define BG_CARD_BASE_PALNO	(0x00)						// �����ް���گ�����

#define BG_FONT_HITO_NO_0	(0x8c)						// "�l"
#define BG_FONT_HITO_NO_1	(0x9c)
#define BG_FONT_HITO_PALNO	(0x00)
#define BG_FONT_KAI_NO_0	(0x8d)						// "��" 
#define BG_FONT_KAI_NO_1	(0x9d)
#define BG_FONT_KAI_PALNO	(0x00)

#define BG_VRAMSET(m,x,y,c) m[((y)<<5)+(x)] = c			// BG��׾��
#define	BG_SCRN_DATA(ch,h,v,pal) \
		((ch)|(h<<10)|(v<<11)|(pal<<12))				// ��ذ��ް�����

#define	TRG		(sys.Trg)								// ���ض�
#define	CONT	(sys.Cont)								// �����ƭ

#define	CELL_MENU 0										// ��ID
#define PAL_MENU 0										// ��گ�ID

#define BG_POS_Y_OFFSET		(-0x04)						// Y�̾��

enum
{
	CARD_PLAYER = 0,									// ��ڲ԰����
	CARD_FRIEND,										// �ʐM����
};

enum													// ���ݽ����
{
	SEQNO_INIT,
	SEQNO_FADE_IN,
	SEQNO_KEY_WAIT,
	SEQNO_SET_CARD_TURN,
	SEQNO_CARD_TURN_WAIT,
	SEQNO_SET_END,
	SEQNO_END,
};

//----------------------------------------------------------------------
//		typedef
//----------------------------------------------------------------------
typedef struct
{
	u8	seq_no;											// ���ݽ����
	u8	my_friend;										// ����
	u8	star;											// ��
	u8	face_back;										// ���ޕ\0��1
	u8	laster_buf_forward;								// ���ݸ�ޯ̧�׸�
	u8	playtime_s;										// �b
	u8	playtime_ms;									// �b���
	u8	sw_zukan;										// �}���׸�
	u8	sw_dendow;										// �a���׸�
	u8	sw_cable;										// ������׸�
	u8	sw_battletower;									// �����ܰ�׸�
	u8	sw_contest;										// ��ý��׸�
	u8	sw_cube;										// ������׸�
	u8	sw_pokeexchange;								// �����׸�
	u8	sw_badge[8];									// �ޯ���׸�
	actWork *act_abt;									// A ����
	actWork *act_bbt;									// B ����
	u8	fnt_kaiwa[4][8];								// �ȈՉ�b
	pFunc rec_proc;										// ���A����
	TRCARD card;										// �����ް�
}MYST;

//----------------------------------------------------------------------
//		����
//----------------------------------------------------------------------
static void PushTempWorkPlayer( pFunc );
static void PushTempWorkFriend( u8, pFunc );
static void PopTempWork( void );

static void MystScrnOFF( void );
static void MystScrnON( void );
static void MystBgVramInit( void );
static void MystOamInit( void );
static void MystScrnInit( void );
static void MystWorkInit( void );
static void MystProcInit( void );
static void MystFontInitStart( void );
static void MystVramSet( void );
static void MystDecordDataSet( void );
static void MystActorSet( void );
static void MystProcSet( void );

static void SetStatusTask( void );
static void SetPlayTimeTask( void );
static void DelPlayTimeTask( void );
static void SetCardTurnTask( void );
static u8 CheckCardTurnTask( void );

static void SetScreenCard( void );

//----
#if BUTTON_ACTOR_ON
//----
static void SetButtonActor( void );
static void SetButtonAnime( void );
static void A_ButtonActMove( actWork * );
static void B_ButtonActMove( actWork * );
static void SetObjVram( void );
//----
#endif
//----

static void SetBgOffset( void );
static void SetBgVram( void );
static void SetBgCardPalette( void );
static void SetBgDecordData( void );
static void SetMapCardFace( void );
static void SetMapCardBack( void );
static void SetMapStatusBack( void );
static void SetMapHero( void );
static void SetMapStar( void );
static void SetMapBadge( void );
static void ClearMapBg1( void );
static void SetMapBgFontHito( s16, s16 );
static void SetMapBgFontKai( s16, s16 );
static void ClearMapBgFontZukan( void );

static void SetFontCardFace( void );
static void SetFontCardBack( void );
static void SetFontID( void );
static void SetFontOkozukai( void );
static void SetFontZukan( void );
static void SetFontPlayTime( void );
static void SetFontKaiwa( void );
static void SetFontNameTrainerCard( void );
static void SetFontTextDendow( void );
static void SetFontDataDendow( void );
static void SetFontTextCableFight( void );
static void SetFontDataCableFight( void );
static void SetFontTextBattleTower( void );
static void SetFontDataBattleTower( void );
static void SetFontTextContext( void );
static void SetFontDataContext( void );
static void SetFontTextCube( void );
static void SetFontDataCube( void );
static void SetFontTextPokeExchange( void );
static void SetFontDataPokeExchange( void );

static u16 GetMyStatusZukanGetTotal( void );

static const u16 * const StatusBgPaletteTbl[];
static const u16 StatusBgPaletteBackFemale[];
static const u16 l_badge_Palette[];
static const u16 star_Palette[];
static const u16 FontRedPalette[];
static const u16 st_menu_Palette[];
static const CellData status_CellData[];
static const PalData status_PaletteData[];
static const OAMPatSt status_Oampat[];
static const actHeader a_button_ActHead;
static const actHeader b_button_ActHead;
static const u16 BadgeCharMap[8][4];
static const u16 status1_Map[];
static const u16 status2_Map[];
static const u16 status3_Map[];
static const u16 statusbg_Map[];

static MYST * const Myst = ((MYST*)UserWork);

#ifdef PM_DEBUG
static u8 MyStatusDebug;
#endif

//======================================================================
//		�ð�����
//======================================================================
static void MyStatusInit( void );
static void MyStatusMain( void );
static void MyStatusVBlank( void );

//------------------------------------------
//	��l���ڰŰ����
//	In:		rec_proc	; ���A����
//------------------------------------------
void SetTrainerCardPlayer( pFunc rec_proc )
{
#ifdef PM_DEBUG
	MyStatusDebug = 0;
#endif
	PushTempWorkPlayer( rec_proc );
	MainProcChange( MyStatusInit );
}

//------------------------------------------
//	�ʐM�p�ڰŰ����
//	In:		no			; �\�������ڰŰ����ܰ�����
//			rec_proc	; ���A����
//------------------------------------------
void SetTrainerCardFriend( u8 no, pFunc rec_proc )
{
#ifdef PM_DEBUG
	MyStatusDebug = 0;
#endif
	PushTempWorkFriend( no, rec_proc );
	MainProcChange( MyStatusInit );
}

//----
#ifdef PM_DEBUG
//----
//------------------------------------------
//	ý��ް�
//------------------------------------------
static const TRCARD test_card =
{
	1,														// ����
	TRCARD_GOLD,											// ��
	1,														// �}�ӹޯ��׸�
	1,														// �}�Ӻ���ذ��׸�
	1,														// ��ý�,���p�ٲ���ĸر�׸�
	999,													// ����ر���(��)
	99,														// ����ر���(��)
	99,														// ����ر���(�b)
	KAIHATSU_NUM_MAX,										// �}��
	12345,													// ID
	99,														// ��ڲ����(��)
	99,														// ��ڲ����(��)
	9999,													// ����ّΐ폟����
	9999,													// ����ّΐ핉����
	9999,													// �����ܰ������
	9999,													// �����ܰ�A����
	999,													// ����ٺ�ýėD����
	65535,													// ����ލ쐬��
	65535,													// �߹�ݹ���ٌ�����
	99999,													// ����
	{ 1, 2, 3, 4 },											// �ȈՉ�b
	{ te_, su_, to_, EOM_ }									// ���O(11BYTE)
};

//------------------------------------------
//	��l���ڰŰ���� ���ޯ�ޗp
//------------------------------------------
void SetTrainerCardPlayerDebug( pFunc rec_proc )
{
	MyStatusDebug = 1;
	PushTempWorkPlayer( rec_proc );
	MainProcChange( MyStatusInit );
}

//------------------------------------------
//	�ʐM�p�ڰŰ���� ���ޯ�ޗp
//------------------------------------------
void SetTrainerCardFriendDebug( pFunc rec_proc )
{
	MultiTrainerCard[0] = test_card;
	MyStatusDebug = 1;
	PushTempWorkFriend( 0, rec_proc );
	MainProcChange( MyStatusInit );
}

void InitTrainerCardFriendDebug( void )
{
	int i;
	for ( i=0;i<4;i++ ) {
		MultiTrainerCard[i] = test_card;
	}
}
//----
#endif
//----

//------------------------------------------
//	�ð����� ������
//------------------------------------------
static void MyStatusInit( void )
{
	switch( sys.InitSeqWork )
	{
		case 0:
			MystScrnOFF();
			MystWorkInit();
			sys.InitSeqWork++;
			break;
		case 1:
			MystBgVramInit();
			sys.InitSeqWork++;
			break;
		case 2:
			MystOamInit();
			sys.InitSeqWork++;
			break;
		case 3:
			MystScrnInit();
			MystProcInit();
			sys.InitSeqWork++;
			break;
		case 4:
			MystFontInitStart();
			sys.InitSeqWork++;
		case 5:
			if( NMenuScreenInitMain() )
				sys.InitSeqWork++;
			break;
		case 6:
			MystDecordDataSet();
			sys.InitSeqWork++;
			break;
		case 7:
			MystVramSet();
			sys.InitSeqWork++;
			break;
		case 8:
			MystActorSet();
			MystProcSet();
			MystScrnON();
			MainProcChange( MyStatusMain );
	}
}

//------------------------------------------
//	Ҳ�
//------------------------------------------
static void MyStatusMain( void )
{
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

//------------------------------------------
//	V Blank
//------------------------------------------
static void MyStatusVBlank( void )
{
	actOAMtrans();
	actDMAtrans();
	PaletteFadeTrans();

	if( ++Myst->playtime_ms >= 60 )
	{
		Myst->playtime_ms = 0;
		Myst->playtime_s ^= 1;
	}

	if( Myst->laster_buf_forward )
	{
		DmaCopy( 3, LasterBuffer[0], LasterBuffer[1], 160*2, 16 );
	}
}

//======================================================================
//		ܰ�
//======================================================================
static void MystTempWorkTask( u8 );
static u32 GetCardRecord( u8, u32 );
static u8 CheckTrainerCardStarData( TRCARD * );

//------------------------------------------
//	�ް��ޔ� ��ڲ԰
//------------------------------------------
static void PushTempWorkPlayer( pFunc rec_proc )
{
	u8 i;
	TASK_TABLE *task;

	i = AddTask( MystTempWorkTask, 0xff );
	task = &TaskTable[i];
	
	task->work[0] = CARD_PLAYER;
	SetAdrs_to_Work( &task->work[2], rec_proc );
}

//------------------------------------------
//	�ް��ޔ� �ʐM�p
//------------------------------------------
static void PushTempWorkFriend( u8 no, pFunc rec_proc )
{
	u8 i;
	TASK_TABLE *task;
	
	i = AddTask( MystTempWorkTask, 0xff );
	task = &TaskTable[i];
	
	task->work[0] = CARD_FRIEND;
	task->work[1] = no;
	SetAdrs_to_Work( &task->work[2], rec_proc );
}

//------------------------------------------
//	�ޔ�ܰ����A
//------------------------------------------
static void PopTempWork( void )
{
	u8 i;
	TASK_TABLE *task;

	i = CheckTaskNo( MystTempWorkTask );
	task = &TaskTable[i];
	
	Myst->my_friend = (u8)task->work[0];
	SetWork_to_Seq( &task->work[2], &Myst->rec_proc );
	
	if( Myst->my_friend )
	{
		Myst->card = MultiTrainerCard[task->work[1]];
	}
	else
	{
		SetTrainerCardWorkPlayer( &Myst->card );
	}
}

//------------------------------------------
//	�ޔ�p���
//------------------------------------------
static void MystTempWorkTask( u8 no )
{
}

//------------------------------------------
//	�ڰŰ�����ް����
//	In:		card		; ��Ă���ܰ�
//------------------------------------------
void SetTrainerCardWorkPlayer( TRCARD *card )
{
	u8 i;
	u32 dat;

	card->sex = MyData.my_sex;
	card->play_time_h = MyData.playtime_h;
	card->play_time_m = MyData.playtime_m;

	dat = GetRecord( SV_TOTAL_TIME );
	if( GetRecord(CNT_DENDOU) == 0 ) dat = 0;
	card->clear_time_h = (dat >> 16) & 0x0000ffff;
	card->clear_time_m = (dat >> 8) & 0x000000ff;
	card->clear_time_s = dat & 0xff;

	card->zukan_get_flag = EventFlagCheck( SYS_ZUKAN_GET );
	card->zukan_complete_flag = ZukanHyousyouCheckHouen();
	card->zukan_num = GetMyStatusZukanGetTotal();
	card->id = (MyData.ID[1] << 8) | MyData.ID[0];

	card->cable_fight_win = GetCardRecord( CNT_KACHI, 9999 );
	card->cable_fight_lose = GetCardRecord( CNT_MAKE, 9999 );
	card->cable_contest_win = GetCardRecord( CNT_COMM_CONTEST_WIN, 999 );
	card->make_cube = GetCardRecord( CNT_COMM_CUBE_MAKE, 65535 );
	card->cable_poke_exe = GetCardRecord( CNT_COMM_TRADE, 65535 );

#if 0
	card->battle_tower_win = GetCardRecord( CNT_TOWER_KACHI, 9999 );
	card->battle_tower_cons_win = GetCardRecord( CNT_TOWER_RENSHOU, 9999 );
#elif 0
	dat = GetBattleTowerRenshou( 0 );
	card->battle_tower_cons_win = GetBattleTowerRenshou( 1 );
	if( dat > card->battle_tower_cons_win ) card->battle_tower_cons_win = dat;
	if( card->battle_tower_cons_win > 9999 ) card->battle_tower_cons_win = 9999;
#else
	card->battle_tower_win = MyData.BattleTowerWork.BattleTowerKachi;
	card->battle_tower_cons_win = MyData.BattleTowerWork.BattleTowerKachinuki;
	if( card->battle_tower_win > 9999 ) card->battle_tower_win = 9999;
	if( card->battle_tower_cons_win > 9999 ) card->battle_tower_cons_win = 9999;
#endif

	dat = 0;
	if( ArtMuseumCountCheck() >= MUSEUM_PICTURE_MAX ) dat = 1;
	card->contest_art_flag = dat;

	card->gold = Fld.my_gold;

	for( i = 0; i < 4; card->kaiwa[i] = Fld.kaiwa_self[i], i++ );
	for( i = 0; i < (PERSON_NAME_SIZE+EOM_SIZE);
						card->name[i] = MyData.my_name[i], i++ );
	
	card->star = CheckTrainerCardStarData( card );
}

//------------------------------------------
//	�ʐM�ڰŰ���� �����擾
//	In:		no			; �ڰŰ����ܰ�����
//------------------------------------------
u8 GetMultiTrainerCardStar( u8 no )
{
	return( MultiTrainerCard[no].star );
}

//------------------------------------------
//	ں����ް��擾
//------------------------------------------
static u32 GetCardRecord( u8 sym, u32 max )
{
	u32 dat;
	
	dat = GetRecord( sym );
	if( dat > max ) dat = max;

	return( dat );
}

//------------------------------------------
//	�ڰŰ���ސ�����
//------------------------------------------
static u8 CheckTrainerCardStarData( TRCARD *card )
{
	u8 star;

	star = 0;

	if( card->clear_time_h ||							// �a����������
		card->clear_time_m ||
		card->clear_time_s )
	{
		star++;
	}

	if( card->zukan_complete_flag )						// �}�Ӻ���ذ�����
	{
		star++;
	}

	if( card->battle_tower_cons_win >= 50 )				// �����ܰ50�l��������
	{
		star++;
	}

	if( card->contest_art_flag )						// ��ý�,���p�ٲ���ĸر
	{
		star++;
	}
	
	return( star );
}

//======================================================================
//		����������
//======================================================================
static void MystFlagSet( void );

//------------------------------------------
//	��ʏ��� OFF
//------------------------------------------
static void MystScrnOFF( void )
{
	SetVBlankFunc( NULL );
	SetHBlankFunc( NULL );
	
	*(vu16*)REG_DISPCNT = 0;
}

//------------------------------------------
//	��ʏ��� ON
//------------------------------------------
static void MystScrnON( void )
{
	SetVBlankFunc( MyStatusVBlank );

	REG_IE_SET( V_BLANK_INTR_FLAG|H_BLANK_INTR_FLAG );	// H V ���荞��

	*(vu16*)REG_STAT |=
					STAT_V_BLANK_IF_ENABLE |			// V Blank���荞�݋����׸�ON
					STAT_H_BLANK_IF_ENABLE;				// H Blank���荞�݋����׸�ON

	*(vu16 *)REG_DISPCNT =
					DISP_MODE_0	|						// Ӱ�� 0
					DISP_BG0_ON	|						// BG ON
					DISP_BG1_ON |
					DISP_BG2_ON	|
					DISP_BG3_ON |
					DISP_OBJ_CHAR_1D_MAP |				// ���1����ϯ��ݸ�
					DISP_OBJ_ON;						// OBJ ON
}

//------------------------------------------
// BG VRAM ������
//------------------------------------------
static void MystBgVramInit( void )
{
	DIV_DMACLEAR( 3, 0, BG_VRAM, BG_MODE0_VRAM_SIZE, 16 );
}
	
//------------------------------------------
//	OAM ������
//------------------------------------------
static void MystOamInit( void )
{
	DIV_DMACLEAR( 3, 0, OAM,  OAM_SIZE, 16 );
}

//------------------------------------------
//	��ʏ�����
//------------------------------------------
static void MystScrnInit( void )
{
	*(vu16 *)REG_BG0CNT = 0;							// BG ���۰ٸر
	*(vu16 *)REG_BG1CNT = 0;
	*(vu16 *)REG_BG2CNT = 0;
	*(vu16 *)REG_BG3CNT = 0;

	*(vu16 *)REG_BG0HOFS = 0;							// ���W�ر
	*(vu16 *)REG_BG0VOFS = 0; 
	*(vu16 *)REG_BG1HOFS = 0;
	*(vu16 *)REG_BG1VOFS = 0; 
	*(vu16 *)REG_BG2HOFS = 0;
	*(vu16 *)REG_BG2VOFS = 0;
	*(vu16 *)REG_BG3HOFS = 0;
	*(vu16 *)REG_BG3VOFS = 0;

	*(vu16 *)REG_BG0CNT =								// BG 0
		(BG0_MAP_BASE_NO << BG_SCREEN_BASE_SHIFT)	|	// ��ذ��ް�
		(BG0_CH_BASE_NO << BG_CHAR_BASE_SHIFT)		| 	// ����ް�
								BG_COLOR_16 		|	// 16
								BG_SCREEN_SIZE_0	|	// 256x256
								BG_PRIORITY_0;			// ��ײ��è

	*(vu16 *)REG_BG1CNT =								// BG 1
		(BG1_MAP_BASE_NO << BG_SCREEN_BASE_SHIFT)	|	// ��ذ��ް�
		(BG1_CH_BASE_NO << BG_CHAR_BASE_SHIFT)		| 	// ����ް�
								BG_COLOR_16 		|	// 16
								BG_SCREEN_SIZE_0	|	// 256x256
								BG_PRIORITY_1;			// ��ײ��è

	*(vu16 *)REG_BG2CNT =								// BG 2
		(BG2_MAP_BASE_NO << BG_SCREEN_BASE_SHIFT)	|	// ��ذ��ް�
		(BG2_CH_BASE_NO << BG_CHAR_BASE_SHIFT)		| 	// ����ް�
								BG_COLOR_16 		|	// 16
								BG_SCREEN_SIZE_0	|	// 256x256
								BG_PRIORITY_2;			// ��ײ��è

	*(vu16 *)REG_BG3CNT =								// BG 3
		(BG3_MAP_BASE_NO << BG_SCREEN_BASE_SHIFT)	|	// ��ذ��ް�
		(BG3_CH_BASE_NO << BG_CHAR_BASE_SHIFT)		| 	// ����ް�
								BG_COLOR_16 		|	// 16
								BG_SCREEN_SIZE_0	|	// 256x256
								BG_PRIORITY_3;			// ��ײ��è
}

//------------------------------------------
//	ܰ�������
//------------------------------------------
static void MystWorkInit( void )
{
	u8 i;

	PopTempWork();										// ܰ����A

	Myst->seq_no = SEQNO_INIT;
	Myst->face_back = 0;
	Myst->laster_buf_forward = 0;
	Myst->star = Myst->card.star;
	Myst->playtime_s = 0;
	Myst->playtime_ms = 0;

	for( i = 0; i < 4; i++ )							// ��b����
	{
		KaiwaCopyWord( Myst->fnt_kaiwa[i], Myst->card.kaiwa[i] );
	}
	
	MystFlagSet();
}

//------------------------------------------
//	�׸޾��
//------------------------------------------
static void MystFlagSet( void )
{
	u32	i,badge;

	Myst->sw_zukan = 0;									// �׸޸ر
	Myst->sw_dendow = 0;
	Myst->sw_cable = 0;
	Myst->sw_battletower = 0;
	Myst->sw_contest = 0;
	Myst->sw_cube = 0;
	Myst->sw_pokeexchange = 0;
	memset( Myst->sw_badge, 0, 8 );						// �ޯ�޸ر

	if( Myst->card.zukan_get_flag )						// �}�ӏ�������
	{
		Myst->sw_zukan++;
	}

	if(	Myst->card.clear_time_h ||						// �ر�������
		Myst->card.clear_time_m ||
		Myst->card.clear_time_s )
	{
		Myst->sw_dendow++;
	}

	if(	Myst->card.cable_fight_win ||					// �ʐM�ΐ�����
		Myst->card.cable_fight_lose )
	{
		Myst->sw_cable++;
	}
	
	if(	Myst->card.battle_tower_win ||					// �����ܰ����
		Myst->card.battle_tower_cons_win )
	{
		Myst->sw_battletower++;
	}
	
	if( Myst->card.cable_contest_win )					// ��ý�����
	{
		Myst->sw_contest++;
	}
	
	if( Myst->card.make_cube )							// ���������
	{
		Myst->sw_cube++;
	}

	if( Myst->card.cable_poke_exe )						// �߹�݌�������
	{
		Myst->sw_pokeexchange++;
	}

	if( Myst->my_friend == CARD_PLAYER )				// ��ڲ԰���� & �ޯ������
	{
		for( i = 0, badge = SYS_BATCH01_GET; badge <= SYS_BATCH08_GET; badge++, i++ )
		{
			if( EventFlagCheck(badge) ) Myst->sw_badge[i]++;
		}
	}

//----
#ifdef PM_DEBUG
//----
	if( MyStatusDebug )
	{
		Myst->sw_dendow = 1;
		Myst->sw_cable = 1;
		Myst->sw_battletower = 1;
		Myst->sw_contest = 1;
		Myst->sw_cube = 1;
		Myst->sw_pokeexchange = 1;
		memset( Myst->sw_badge, 1, 8 );
	}
//----
#endif
//----
}

//------------------------------------------
//	����������
//------------------------------------------
static void MystProcInit( void )
{
	PaletteAnimeInit();
	InitActor();
	ObjPalManInit();
	InitTask();
}

//------------------------------------------
//	̫�ď�����
//------------------------------------------
static void MystFontInitStart( void )
{
	PrintScreenInit( MYSTATUS0_TYPE );
	NMenuScreenInitStart( MYSTATUS0_TYPE );
}

//------------------------------------------
//	Vram ���
//------------------------------------------
static void MystVramSet( void )
{
	SetBgOffset();
	SetBgVram();										// BG VRAM ���

#if BUTTON_ACTOR_ON
	SetObjVram();										// OBJ VRAM ��� 
#endif

	SetMapStatusBack();									// �w�i���
	SetScreenCard();									// ���޾��
}

//------------------------------------------
//	���k�ް����
//------------------------------------------
static void MystDecordDataSet( void )
{
	SetBgDecordData();
}

//------------------------------------------
//	������
//------------------------------------------
static void MystActorSet( void )
{
#if BUTTON_ACTOR_ON
	SetButtonActor();
#endif
}

//------------------------------------------
//	�������
//------------------------------------------
static void MystProcSet( void )
{
	SetStatusTask();									// �ð������
}

//======================================================================
//		�ð������
//======================================================================
static void StatusTask( u8 );
static u8 Status0( TASK_TABLE * );
static u8 Status1( TASK_TABLE * );
static u8 Status2( TASK_TABLE * );
static u8 Status3( TASK_TABLE * );
static u8 Status4( TASK_TABLE * );
static u8 Status5( TASK_TABLE * );
static u8 Status6( TASK_TABLE * );

//------------------------------------------
//	Ҳݏ���������
//------------------------------------------
static void SetStatusTask( void )
{
	u8 i;
	
	i = AddTask( StatusTask, 0 );
	StatusTask( i );
}

//------------------------------------------
//	�ð������ð���
//------------------------------------------
static u8 (* const StatusTaskTbl[])( TASK_TABLE * ) =
{
	Status0,
	Status1,
	Status2,
	Status3,
	Status4,
	Status5,
	Status6,
};

//------------------------------------------
//	�ð�����
//------------------------------------------
static void StatusTask( u8 no )
{
	while( StatusTaskTbl[Myst->seq_no](&TaskTable[no]) );
}

//------------------------------------------
//	�ð�� 0	������
//------------------------------------------
static u8 Status0( TASK_TABLE *task )
{
	Myst->playtime_s = MyData.playtime_s & 0x01;
	Myst->playtime_ms = MyData.playtime_ms;
	SetPlayTimeTask();									// ��ڲ����������
	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );			// ̪��޲�
	Myst->seq_no++;
	return( 0 );
}

//------------------------------------------
//	�ð�� 1 ̪��޲�
//------------------------------------------
static u8 Status1( TASK_TABLE *task )
{
	if( FadeData.fade_sw == 0 )
	{
		Myst->seq_no++;
	}
	
	return( 0 );
}

//------------------------------------------
//	�ð�� 2 ������
//------------------------------------------
static u8 Status2( TASK_TABLE *task )
{
	if( TRG & B_BUTTON )
	{
		Myst->seq_no = SEQNO_SET_END;
		return( 1 );
	}

	if( TRG & A_BUTTON )
	{
		if( Myst->face_back )
		{
			Myst->seq_no = SEQNO_SET_END;
		}
		else
		{
			Myst->face_back ^= 1;						// �\��
			Myst->seq_no = SEQNO_SET_CARD_TURN;
		}

		return( 1 );
	}

//----
#ifdef PM_DEBUG
//----
	if( MyStatusDebug && TRG & R_BUTTON )
	{
		Myst->star++;
		Myst->star %= 5;
		SetBgCardPalette();
		if( Myst->face_back == 0 ) SetMapStar();
	}
//----
#endif
//----

	return( 0 );
}

//------------------------------------------
//	�ð�� 3 ���ތ���
//------------------------------------------
static u8 Status3( TASK_TABLE *task )
{
	SetCardTurnTask();

#if BUTTON_ACTOR_ON
	SetButtonAnime();
#endif

	SePlay( SE_CARD );
	Myst->seq_no++;
	return( 0 );
}

//------------------------------------------
//	�ð�� 4 ���ތ���҂�
//------------------------------------------
static u8 Status4( TASK_TABLE *task )
{
	if( CheckCardTurnTask() )
	{
		Myst->seq_no = SEQNO_KEY_WAIT;
	}

	return( 0 );
}

//------------------------------------------
//	�ð�� 5 �I�����
//------------------------------------------
static u8 Status5( TASK_TABLE *task )
{
	DelPlayTimeTask();
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	Myst->seq_no++;
	return( 0 );
}

//------------------------------------------
//	�ð�� 6 �I��
//------------------------------------------
static u8 Status6( TASK_TABLE *task )
{
	if( FadeData.fade_sw == 0 )
	{
		MainProcChange( Myst->rec_proc );
	}
	
	return( 0 );
}

//======================================================================
//		��ڲ����
//======================================================================
static void PlayTimeTask( u8 );

//------------------------------------------
//	��ڲ����������
//------------------------------------------
static void SetPlayTimeTask( void )
{
	AddTask( PlayTimeTask, 0 );
	NMenuContextChange( MYSTATUS0_TYPE );
}

//------------------------------------------
//	��ڲ��������폜
//------------------------------------------
static void DelPlayTimeTask( void )
{
	u8 i;
	
	i = CheckTaskNo( PlayTimeTask );
	
	if( i != NOT_EXIST_TASK_ID )
	{
		DelTask( i );
	}
}

//------------------------------------------
//	��ڲ�������
//------------------------------------------
static void PlayTimeTask( u8 no )
{
	TASK_TABLE *task;
	u8 fnt[2][2] = { {colon_,EOM_}, {spc_,EOM_} };

	task = &TaskTable[no];

	SetFontPlayTime();

	if( Myst->playtime_s != task->work[1] )				// ���ԍX�V
	{
		task->work[1] = Myst->playtime_s;
		task->work[0] ^= 1;
	}

	NMenuMsgWrite( fnt[task->work[0]], 0x0d, 0x0c );
}

//======================================================================
//		���ޕԂ�
//======================================================================
static void CardTurnTask( u8 );
static u8 CardTurn0( TASK_TABLE * );
static u8 CardTurn1( TASK_TABLE * );
static u8 CardTurn2( TASK_TABLE * );
static u8 CardTurn3( TASK_TABLE * );
static u8 CardTurn4( TASK_TABLE * );
static void CardTurnHBlank( void );

//------------------------------------------
//	���ޕԂ�������
//------------------------------------------
static void SetCardTurnTask( void )
{
	u8 i;

	i = AddTask( CardTurnTask, 0 );
	CardTurnTask( i );
}

//------------------------------------------
//	���ޕԂ�����
//------------------------------------------
static u8 CheckCardTurnTask( void )
{
	if( CheckTaskNo(CardTurnTask) == NOT_EXIST_TASK_ID )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	���ޕԂ�����ð���
//------------------------------------------
static u8 (* const CardTurnTaskTbl[])(TASK_TABLE*) =
{
	CardTurn0,
	CardTurn1,
	CardTurn2,
	CardTurn3,
	CardTurn4,
};

//------------------------------------------
//	���ޕԂ�
//------------------------------------------
static void CardTurnTask( u8 no )
{
	while( CardTurnTaskTbl[TaskTable[no].work[0]](&TaskTable[no]) );
}

//------------------------------------------
//	���ޕԂ� 0
//------------------------------------------
static u8 CardTurn0( TASK_TABLE *task )
{
	u32 i;

	Myst->laster_buf_forward = 0;
	LasterWorkInit();

	for( i = 0; i < 160; LasterBuffer[1][i] = BG_POS_Y_OFFSET, i++ );

	SetHBlankFunc( CardTurnHBlank );
	Myst->laster_buf_forward = 1;

	task->work[0]++;
	return( 0 );
}

//------------------------------------------
//	���ޕԂ� 1
//------------------------------------------
static u8 CardTurn1( TASK_TABLE *task )
{
	s16 i;
	u32 y0,y1,magn,view,view0,view1,z_magn;
	u32 view_max;

	Myst->laster_buf_forward = 0;

	task->work[1] += 3;

	if( task->work[1] >= 80 )
	{
		task->work[1] = 79;
	}

	y0 = task->work[1];
	y1 = 160 - task->work[1];
	magn = y1 - y0;

	view0 = (-y0 << 16);
	view1 = (160 << 16) / magn;
	view1 -= 0x00010000;
	view_max = view0 + (view1 * magn);
	z_magn = (view1 / magn);
	view1 *= 2;

	for( i = 0; i < y0; LasterBuffer[0][i] = (u16)-i + BG_POS_Y_OFFSET, i++ );

	for( ; i < (s16)y1; i++ )
	{
		view = view0 >> 16;
		view0 += view1;
		view1 -= z_magn;
		LasterBuffer[0][i] = view + BG_POS_Y_OFFSET;
	}

	for( view = view_max >> 16; i < 160; LasterBuffer[0][i] = view + BG_POS_Y_OFFSET, i++ );

	Myst->laster_buf_forward = 1;

	if( task->work[1] >= 75 )
	{
		task->work[0]++;
	}

	return( 0 );
}

//------------------------------------------
//	���ޕԂ� 2
//------------------------------------------
static u8 CardTurn2( TASK_TABLE *task )
{
	DelPlayTimeTask();
	SetScreenCard();									// ���޾��
	
	if( Myst->face_back == 0 )
	{
		SetPlayTimeTask();
	}
	
	task->work[0]++;
	return( 1 );
}

//------------------------------------------
//	���ޕԂ� 3
//------------------------------------------
static u8 CardTurn3( TASK_TABLE *task )
{
	s16 i;
	u32 y0,y1,magn,view,view0,view1,z_magn;
	u32 view_max;

	Myst->laster_buf_forward = 0;

	task->work[1] -= 3;

	if( task->work[1] <= 0 )
	{
		task->work[1] = 0;
	}

	y0 = task->work[1];
	y1 = 160 - task->work[1];
	magn = y1 - y0;

	view0 = (-y0 << 16);
	view1 = (160 << 16) / magn;
	view1 -= 0x00010000;
	view_max = view0 + (view1 * magn);
	z_magn = (view1 / magn);
	view1 /= 2;

	for( i = 0; i < y0; LasterBuffer[0][i] = (u16)-i + BG_POS_Y_OFFSET, i++ );

	for( ; i < (s16)y1; i++ )
	{
		view = view0 >> 16;
		view0 += view1;
		view1 += z_magn;
		LasterBuffer[0][i] = view + BG_POS_Y_OFFSET;
	}
	
	for( view = view_max >> 16; i < 160; LasterBuffer[0][i] = view + BG_POS_Y_OFFSET, i++ );

	Myst->laster_buf_forward = 1;

	if( task->work[1] <= 0 )
	{
		task->work[0]++;
	}

	return( 0 );
}

//------------------------------------------
//	���ޕԂ� 4
//------------------------------------------
static u8 CardTurn4( TASK_TABLE *task )
{
	Myst->laster_buf_forward = 0;
	SetHBlankFunc( NULL );
	SetBgOffset();
	DelTask( CheckTaskNo(CardTurnTask) );
	
	return( 0 );
}

//------------------------------------------
//	���ޕԂ� H ���ݸ
//------------------------------------------
static void CardTurnHBlank( void )
{
	u16 buf;

	buf = LasterBuffer[1][(*(vu16*)REG_VCOUNT)&0xff];

	*(vu16*)REG_BG0VOFS = buf;
	*(vu16*)REG_BG1VOFS = buf;
	*(vu16*)REG_BG2VOFS = buf;
}

//======================================================================
//		���
//======================================================================
static void SetScreenCardFace( void );
static void SetScreenCardBack( void );

//------------------------------------------
//	���޾��
//------------------------------------------
static void SetScreenCard( void )
{
	if( Myst->face_back )								// ��
	{
		SetScreenCardBack();
	}
	else
	{
		SetScreenCardFace();
	}
}

//------------------------------------------
//	���ޕ\���
//------------------------------------------
static void SetScreenCardFace( void )
{
	NMenuScreenClear();
	ClearMapBg1();
	SetMapCardFace();
	SetMapHero();
	SetMapStar();
	SetMapBadge();
	SetFontCardFace();
}

//------------------------------------------
//	���ޗ����
//------------------------------------------
static void SetScreenCardBack( void )
{
	NMenuScreenClear();
	ClearMapBg1();
	SetMapCardBack();
	SetFontCardBack();
}

//======================================================================
//		����
//======================================================================
//----
#if BUTTON_ACTOR_ON
//----
//------------------------------------------
//	���ݱ������
//------------------------------------------
static void SetButtonActor( void )
{
	u8 i;
	
	i = AddActor( &a_button_ActHead, (0x15*8)+4, (0x13*8)-2, 0 );
	Myst->act_abt = &ActWork[i];
	SetActOamPat( Myst->act_abt, status_Oampat );
	Myst->act_abt->oampat_no = 1;

	i = AddActor( &b_button_ActHead, (0x1b*8)+4, (0x13*8)-2, 0 );
	Myst->act_bbt = &ActWork[i];
	SetActOamPat( Myst->act_bbt, status_Oampat );
	Myst->act_bbt->oampat_no = 2;
}

//------------------------------------------
//	���ݱ�Ҿ��
//------------------------------------------
static void SetButtonAnime( void )
{
	Myst->act_abt->work[0] = 1;
	Myst->act_bbt->work[0] = 1;
}

//------------------------------------------
//	���ݱ������ʓ���
//------------------------------------------
static u8 ButtonActMove( actWork *act )
{
	if( act->work[0] == 1 )
	{
		act->dy++;

		if( act->dy >= 18 )
		{
			act->work[0]++;
			return( 1 );
		}
	}
	else if( act->work[0] == 2 )
	{
		act->dy--;
	
		if( act->dy == 0 )
		{
			act->work[0] = 0;
		}
	}
	
	return( 0 );
}

//------------------------------------------
//	A ���ݱ�������
//------------------------------------------
static void A_ButtonActMove( actWork *act )
{
	if( ButtonActMove(act) )
	{
		act->oampat_no++;
		act->oampat_no &= 1;
	}
}

//------------------------------------------
//	B ���ݱ�������
//------------------------------------------
static void B_ButtonActMove( actWork *act )
{
	ButtonActMove( act );
}
//----
#endif
//----

//======================================================================
//		OBJ
//======================================================================
//----
#if BUTTON_ACTOR_ON
//----
//------------------------------------------
//	OBJ VRAM ���
//------------------------------------------
static void SetObjVram( void )
{
	CellSets( status_CellData );
	ObjPalSets( status_PaletteData );
}
//----
#endif
//----

//======================================================================
//		BG
//======================================================================
static void SetMap240x160( u16 *map, u16 *data );

//------------------------------------------
//	BG �̾�č��W���
//------------------------------------------
static void SetBgOffset( void )
{
	*(vu16 *)REG_BG0VOFS = BG_POS_Y_OFFSET; 
	*(vu16 *)REG_BG1HOFS = 0;
	*(vu16 *)REG_BG1VOFS = BG_POS_Y_OFFSET; 
	*(vu16 *)REG_BG2HOFS = 0;
	*(vu16 *)REG_BG2VOFS = BG_POS_Y_OFFSET;
}

//------------------------------------------
//	��ڲ԰ BG VRAM ���
//------------------------------------------
static void SetBgVram( void )
{
	SetBgCardPalette();
	PaletteWorkSet( FontRedPalette, FONT_RED_PALNO, 0x20 );
	DIV_DMACOPY( 3, status_Character, BG1_CH_BASE, BG_C_CHDATA_MAX, 16 );
	DIV_DMACOPY( 3, l_badge_Character, BG_BADGE_CH_BASE, BG_B_CHDATA_MAX, 16 );
}

//------------------------------------------
//	������گľ��
//------------------------------------------
static void SetBgCardPalette( void )
{
	PaletteWorkSet( StatusBgPaletteTbl[Myst->star], PA_BG0, 0x60 );
	PaletteWorkSet( l_badge_Palette, PA_BG3, 0x20 );
	PaletteWorkSet( star_Palette, PA_BG4, 0x20 );

	if( Myst->card.sex )
	{
		PaletteWorkSet( StatusBgPaletteBackFemale, PA_BG1, 0x20 );
	}
}

//------------------------------------------
//	BG ���k�ް����
//------------------------------------------
static void SetBgDecordData( void )
{
	SetStandTrainerVram( Myst->card.sex, PA_BG5, (void*)BG_HERO_CH_BASE );
}

//------------------------------------------
//	�\���ޕ`��
//------------------------------------------
static void SetMapCardFace( void )
{
	u16 *tbl[2] = { (u16*)status1_Map, (u16*)status3_Map };

	SetMap240x160( (u16*)BG2_MAP_BASE, tbl[Myst->my_friend] );
}

//------------------------------------------
//	�����ޕ`��
//------------------------------------------
static void SetMapCardBack( void )
{
	SetMap240x160( (u16*)BG2_MAP_BASE, (u16*)status2_Map );
}

//------------------------------------------
//	�w�i�`��
//------------------------------------------
static void SetMapStatusBack( void )
{
	SetMap240x160( (u16*)BG3_MAP_BASE, (u16*)statusbg_Map );
}

//------------------------------------------
//	240x160ϯ�ߕ`��
//------------------------------------------
static void SetMap240x160( u16 *map, u16 *data )
{
	s16 x,y;
	
	for( y = 0; y < 0x14; y++ )
	{
		for( x = 0; x < 0x1e; BG_VRAMSET(map,x,y,(*data)), x++, data++ );
	}
}

//------------------------------------------
//	��l���`��
//------------------------------------------
static void SetMapHero( void )
{
	s16 x,y;
	u16 ch;
	u16 *map;

	ch = BG_HERO_CHNO_0;
	map = (u16*)BG1_MAP_BASE;

	for( y = 0x05; y < 0x0d; y++ )
	{
		for( x = 0x13; x < 0x1b; x++, ch++ )
		{
			BG_VRAMSET( map, x, y, BG_SCRN_DATA(ch,0,0,BG_HERO_PALNO) );
		}
	}
}

//------------------------------------------
//	�����
//------------------------------------------
static void SetMapStar( void )
{
	s16 x0,x1;
	u16 *map;
	
	map = (u16*)BG1_MAP_BASE;
	
	for( x0 = 0x0f, x1 = 0x0f+Myst->star; x0 < x1; x0++ )
	{
		BG_VRAMSET( map, x0, 0x06, BG_SCRN_DATA(BG_STAR_CHNO,0,0,BG_STAR_PALNO) );
	}

	for( ; x0 < (0x0f+4); x0++ )
	{
		BG_VRAMSET( map, x0, 0x06, BG_SCRN_DATA(0,0,0,0) );
	}
}

//------------------------------------------
//	�ޯ�޾��
//------------------------------------------
static void SetMapBadge( void )
{
	s16 x,i;
	u16 *map;
	
	if( Myst->my_friend == CARD_PLAYER )
	{
		map = (u16*)BG1_MAP_BASE;

		for( i = 0, x = 0x04; i < 8; i++, x += 3 )
		{
			if( Myst->sw_badge[i] )
			{
				BG_VRAMSET( map, x, 0x0f,
					BG_SCRN_DATA(BadgeCharMap[i][0],0,0,BG_BADGE_PALNO) );
				BG_VRAMSET( map, x+1, 0x0f,
					BG_SCRN_DATA(BadgeCharMap[i][1],0,0,BG_BADGE_PALNO) );
				BG_VRAMSET( map, x, 0x10,
					BG_SCRN_DATA(BadgeCharMap[i][2],0,0,BG_BADGE_PALNO) );
				BG_VRAMSET( map, x+1, 0x10,
					BG_SCRN_DATA(BadgeCharMap[i][3],0,0,BG_BADGE_PALNO) );
			}
		}
	}
}

//------------------------------------------
//	BG 1 �ر
//------------------------------------------
static void ClearMapBg1( void )
{
	s16 i;
	u16 *map;
	
	for( i = 0, map = (u16*)BG1_MAP_BASE; i < 0x400; *map = 0, i++, map++ );
}

//------------------------------------------
//	BG "�l" ���
//------------------------------------------
static void SetMapBgFontHito( s16 x, s16 y )
{
	u16 *map;

	map = (u16*)BG1_MAP_BASE;
	BG_VRAMSET( map, x, y, BG_SCRN_DATA(BG_FONT_HITO_NO_0,0,0,BG_FONT_HITO_PALNO) );
	BG_VRAMSET( map, x, y+1, BG_SCRN_DATA(BG_FONT_HITO_NO_1,0,0,BG_FONT_HITO_PALNO) );
}

//------------------------------------------
//	BG "��" ���
//------------------------------------------
static void SetMapBgFontKai( s16 x, s16 y )
{
	u16 *map;

	map = (u16*)BG1_MAP_BASE;
	BG_VRAMSET( map, x, y, BG_SCRN_DATA(BG_FONT_KAI_NO_0,0,0,BG_FONT_HITO_PALNO) );
	BG_VRAMSET( map, x, y+1, BG_SCRN_DATA(BG_FONT_KAI_NO_1,0,0,BG_FONT_HITO_PALNO) );
}

//------------------------------------------
//	BG "�߹�݂�����" ����
//------------------------------------------
static void ClearMapBgFontZukan( void )
{
	u16 x;
	u16 *map;

	map = (u16*)BG2_MAP_BASE;

	for( x = 0x03; x < 0x11; x++ )
	{
		BG_VRAMSET( map, x, 0x0a, BG_SCRN_DATA(BG_CARD_BASE_NO,0,0,BG_CARD_BASE_PALNO) );
		BG_VRAMSET( map, x, 0x0b, BG_SCRN_DATA(BG_CARD_BASE_NO,0,0,BG_CARD_BASE_PALNO) );
	}
}

//======================================================================
//		����
//======================================================================
//------------------------------------------
//	���ޕ\����
//------------------------------------------
static void SetFontCardFace( void )
{
	NMenuContextChange( MYSTATUS0_TYPE );
	NMenuMsgWrite( Myst->card.name, 0x07, 0x05 );
	SetFontID();
	SetFontOkozukai();
	SetFontZukan();
	SetFontKaiwa();
}

//------------------------------------------
//	���ޗ�����
//------------------------------------------
static void SetFontCardBack( void )
{
	NMenuContextChange( MYSTATUS0_TYPE );
	SetFontNameTrainerCard();
	SetFontTextDendow();
	SetFontTextCableFight();
	SetFontTextBattleTower();
	SetFontTextContext();
	SetFontTextCube();
	SetFontTextPokeExchange();

	NMenuContextChange( MYSTATUS1_TYPE );
	SetFontDataDendow();
	SetFontDataCableFight();
	SetFontDataBattleTower();
	SetFontDataContext();
	SetFontDataCube();
	SetFontDataPokeExchange();
}

//------------------------------------------
//	ID
//------------------------------------------
static void SetFontID( void )
{
	u8 fnt[8];

	PM_NumMsgSet( fnt, Myst->card.id, NUM_MODE_ZERO, 5 );
	NMenuMsgWrite( (const u8 *)fnt, 0x14, 0x02 );
}

//------------------------------------------
//	����������
//------------------------------------------
static void SetFontOkozukai( void )
{
	u8 fnt[16];
	u16 pos;

	PM_NumMsgSet( fnt, Myst->card.gold, NUM_MODE_LEFT, 6 );
	pos = PM_strlen( fnt ) - 1;							// EOM ����
	NMenuMsgWrite( (const u8 *)fnt, 0x0e - pos, 0x08 );
}

//------------------------------------------
//	�}��
//------------------------------------------
static void SetFontZukan( void )
{
	u8	fnt[16];
	u16	num;
	
//----
#ifdef PM_DEBUG
//----
	if( MyStatusDebug == 0 && Myst->sw_zukan == 0 )
	{
		ClearMapBgFontZukan();
		return;	
	}
//----
#else
//----
	if( Myst->sw_zukan == 0 )
	{
		ClearMapBgFontZukan();
		return;	
	}
//----
#endif
//----

	PM_NumMsgSet( fnt, Myst->card.zukan_num, NUM_MODE_LEFT, 3 );
	num = PM_strlen( fnt ) - 1;
	NMenuMsgWrite( (const u8 *)fnt, 0x0e - num, 0x0a );
}

//------------------------------------------
//	��ڲ����
//------------------------------------------
static void SetFontPlayTime( void )
{
	u16 h,m;
	u8 fnt[16];
	
	h = MyData.playtime_h;
	m = MyData.playtime_m;

	if( Myst->my_friend )								// �ʐM
	{
		h = Myst->card.play_time_h;
		m = Myst->card.play_time_m;
	}

	PM_NumMsgSet( fnt, h, NUM_MODE_SPACE, 3 );			// �ς�
	NMenuMsgWrite( fnt, 0x0a, 0x0c );
	PM_NumMsgSet( fnt, m, NUM_MODE_ZERO, 2 );
	NMenuMsgWrite( fnt, 0x0e, 0x0c );
}

//------------------------------------------
//	��b
//------------------------------------------
static void SetFontKaiwa( void )
{
	s16 x,y,i;

	if( Myst->my_friend )								// �ʐM
	{
		for( i = 0, x = 0x02, y = 0x0e; y < 0x12; y += 2, x = 0x02 )
		{
			NMenuMsgWrite( Myst->fnt_kaiwa[i], x, y );
			x += PM_strlen( Myst->fnt_kaiwa[i] ) + 1;	// EOM + ��߰�
			i++;
			NMenuMsgWrite( Myst->fnt_kaiwa[i], x, y );
			i++;
		}
	}
}

//------------------------------------------
//	�������ڰŰ����
//------------------------------------------
static void SetFontNameTrainerCard( void )
{
	u16 i;
	u8 fnt[] = { no_,TO_,RE_,bou_,NA_,bou_,KA_,bou_,DO_,EOM_ };

	i = PM_strlen( (const u8*)Myst->card.name );		// EOM
	NMenuMsgWrite( fnt, 0x13, 0x02 );
	NMenuMsgWrite( Myst->card.name, 0x13 - i, 0x02 );
}

//------------------------------------------
//	���߂Ă̓a������
//------------------------------------------
static void SetFontTextDendow( void )
{
	u8 fnt[] = { ha_,zi_,me_,te_,no_,spc_,de_,n_,do_,u_,i_,ri_,EOM_ };
	
	if( Myst->sw_dendow )
	{
		NMenuMsgWrite( fnt, 0x03, 0x05 );
	}
}

//------------------------------------------
//	���߂Ă̓a������
//------------------------------------------
static void SetFontDataDendow( void )
{
	u16 h,m,s;
	u8 fnt[4];
	u8 colon[] = { colon_, EOM_ };

	if( Myst->sw_dendow )
	{
		h = Myst->card.clear_time_h;
		m = Myst->card.clear_time_m;
		s = Myst->card.clear_time_s;
		PM_NumMsgSet( fnt, h, NUM_MODE_SPACE, 3 );
		NMenuMsgWrite( fnt, 0x10, 0x05 );
		PM_NumMsgSet( fnt, m, NUM_MODE_ZERO, 2 );
		NMenuMsgWrite( fnt, 0x14, 0x05 );
		PM_NumMsgSet( fnt, s, NUM_MODE_ZERO, 2 );
		NMenuMsgWrite( fnt, 0x17, 0x05 );
		NMenuMsgWrite( colon, 0x13, 0x05 );
		NMenuMsgWrite( colon, 0x16, 0x05 );
	}
}

//------------------------------------------
//	����ّΐ�
//------------------------------------------
static void SetFontTextCableFight( void )
{
	u8 fnt[] = { KE_,bou_,BU_,RU_,ta_,i_,se_,n_,spc_,
				ka_,ti_,spc_,spc_,spc_,spc_,spc_,spc_,ma_,ke_,EOM_ };
	
	if( Myst->sw_cable )
	{
		NMenuMsgWrite( fnt, 0x03, 0x07 );
		SetMapBgFontKai( 0x12, 0x07 );
		SetMapBgFontKai( 0x1a, 0x07 );
	}
}

//------------------------------------------
//	����ّΐ�
//------------------------------------------
static void SetFontDataCableFight( void )
{
	u16 kati,make;
	u8 fnt[8];

	if( Myst->sw_cable )
	{
		kati = Myst->card.cable_fight_win;
		make = Myst->card.cable_fight_lose;
		PM_NumMsgSet( fnt, kati, NUM_MODE_SPACE, 4 );
		NMenuMsgWrite( fnt, 0x0e, 0x07 );
		PM_NumMsgSet( fnt, make, NUM_MODE_SPACE, 4 );
		NMenuMsgWrite( fnt, 0x16, 0x07 );
	}
}

//------------------------------------------
//	�����ܰ�ΐ�
//------------------------------------------
static void SetFontTextBattleTower( void )
{
	u8 fnt[] = { BA_,TO_,RU_,TA_,WA_,bou_,spc_,
				ka_,ti_,spc_,spc_,spc_,spc_,spc_,spc_,ka_,ti_,nu_,ki_,EOM_ };
	
	if( Myst->sw_battletower )
	{
		NMenuMsgWrite( fnt, 0x03, 0x0f );
		SetMapBgFontKai( 0x10, 0x0f );
		SetMapBgFontHito( 0x1a, 0x0f );
	}
}

//------------------------------------------
//	�����ܰ�ΐ�
//------------------------------------------
static void SetFontDataBattleTower( void )
{
	u8 fnt[8];
	u16 kati,katinuki;
	
	if( Myst->sw_battletower )
	{
		kati = Myst->card.battle_tower_win;
		katinuki = Myst->card.battle_tower_cons_win;
		PM_NumMsgSet( fnt, kati, NUM_MODE_SPACE, 4 );
		NMenuMsgWrite( fnt, 0x0c, 0x0f );
		PM_NumMsgSet( fnt, katinuki, NUM_MODE_SPACE, 4 );
		NMenuMsgWrite( fnt, 0x16, 0x0f );
	}
}

//------------------------------------------
//	��ý�
//------------------------------------------
static void SetFontTextContext( void )
{
	u8 fnt[] = { to_,mo_,da_,ti_,to_,KO_,N_,TE_,SU_,TO_,spc_,
				yu_,u_,si_,yyo_,u_,EOM_ };
	
	if( Myst->sw_contest )
	{
		NMenuMsgWrite( fnt, 0x03, 0x0d );
		SetMapBgFontKai( 0x17, 0x0d );
	}
}

//------------------------------------------
//	��ý�
//------------------------------------------
static void SetFontDataContext( void )
{
	u8 fnt[8];
	u16 i;

	if( Myst->sw_contest )
	{
		i = Myst->card.cable_contest_win;
		PM_NumMsgSet( fnt, i, NUM_MODE_SPACE, 3 );
		NMenuMsgWrite( fnt, 0x14, 0x0d );
	}
}

//------------------------------------------
//	�����
//------------------------------------------
static void SetFontTextCube( void )
{
	u8 fnt[] = { to_,mo_,da_,ti_,to_,spc_,tu_,ku_,ttu_,ta_,spc_,PO_,RO_,TTU_,KU_,
				spc_,spc_,spc_,spc_,spc_,spc_,ko_,EOM_ };

	if( Myst->sw_cube )
	{
		NMenuMsgWrite( fnt, 0x03, 0x0b );
	}
}

//------------------------------------------
//	�����
//------------------------------------------
static void SetFontDataCube( void )
{
	u8 fnt[8];
	u16 i;

	if( Myst->sw_cube )
	{
		i = Myst->card.make_cube;
		PM_NumMsgSet( fnt, i, NUM_MODE_SPACE, 5 );
		NMenuMsgWrite( fnt, 0x13, 0x0b );
	}
}

//------------------------------------------
//	�߹�݌���
//------------------------------------------
static void SetFontTextPokeExchange( void )
{
	u8 fnt[] = { PO_,KE_,MO_,N_,wo_,spc_,ko_,u_,ka_,n_,EOM_ };

	if( Myst->sw_pokeexchange )
	{
		NMenuMsgWrite( fnt, 0x03, 0x09 );
		SetMapBgFontKai( 0x13, 0x09 );
	}
}

//------------------------------------------
//	�߹�݌���
//------------------------------------------
static void SetFontDataPokeExchange( void )
{
	u8 fnt[8];
	u16 i;

	if( Myst->sw_pokeexchange )
	{
		i = Myst->card.cable_poke_exe;
		PM_NumMsgSet( fnt, i, NUM_MODE_SPACE, 5 );
		NMenuMsgWrite( fnt, 0x0e, 0x09 );
	}
}

//======================================================================
//		�ð������
//======================================================================
//------------------------------------------
//	��l�����O
//	In:		x,y				; �\�����W
//------------------------------------------
void FontHeroNameWrite( s16 x, s16 y )
{
	NMenuMsgWrite( (const u8 *)MyData.my_name, x, y );
}

//------------------------------------------
//	���ʕ`��
//	In:		x,y				; �\�����W
//------------------------------------------
void FontHeroSexWrite( s16 x, s16 y )
{
	u8 osu[] = { o_, to_, ko_, EOM_ };
	u8 mesu[] = { o_, n_, na_, EOM_ };
	u8 *fnt;
	
	fnt = osu;
	
	if( MyData.my_sex == FEMALE )
	{
		fnt = mesu;
	}
	
	NMenuMsgWrite( (const u8 *)fnt, x, y );
}

//------------------------------------------
//	�߹�ݐ}�ӂ̐�
//	In:		x,y				; �\�����W
//------------------------------------------
void FontPokeZukanNumWrite( s16 x, s16 y )
{
	u8	fnt[8];
	u8	*p;
	u16	num;

	num = 0;

	num = GetMyStatusZukanGetTotal();
	p = PM_NumMsgSet( fnt, num, NUM_MODE_LEFT, 3 );
	p[0] = hi_;
	p[1] = ki_;
	p[2] = EOM_;

	NMenuMsgWrite( (const u8 *)fnt, x, y );
}

//------------------------------------------
//	�߹�ݐ}�ӂ̐�
//	In:		x,y				; �\�����W(���)
//------------------------------------------
void FontPokeZukanNumBackWrite( s16 x, s16 y )
{
	u8	fnt[8];
	u8	*p;
	u16	num;

	num = 0;

	num = GetMyStatusZukanGetTotal();
	p = PM_NumMsgSet( fnt, num, NUM_MODE_LEFT, 3 );
	p[0] = hi_;
	p[1] = ki_;
	p[2] = EOM_;

	num = PM_strlen(fnt) - 1;							// EOM
	NMenuMsgWrite( (const u8 *)fnt, x - num, y );
}

//------------------------------------------
//	�߹�ݐ}�ӂ̐�(�Â�����)
//	In:		x,y				; �\�����W
//------------------------------------------
void FontOpPokeZukanNumWrite( s16 x, s16 y )
{
	u8 fnt[8];
	u8 *p;
	u16 num;

	num = 0;

	num = GetMyStatusZukanGetTotal();
	p = PM_NumMsgSet( fnt, num, NUM_MODE_LEFT, 3 );
	num = PM_strlen( fnt );
	
	if( num < 3 ) x++;									// 3����߰�����

	p[0] = hi_;
	p[1] = ki_;
	p[2] = EOM_;

	NMenuMsgWrite( (const u8 *)fnt, x, y );
}

//------------------------------------------
//	��ڲ����
//	In:		x,y				; �\�����W
//------------------------------------------
void FontPlayTimeWrite( s16 x, s16 y )
{
	u8 coron[] = { colon_, EOM_ };
	u8 fnt[16];
	
	PM_NumMsgSet( fnt, MyData.playtime_h, NUM_MODE_SPACE, 3 );
	NMenuMsgWrite( fnt, x, y );
	NMenuMsgWrite( coron, x + 3, y );
	PM_NumMsgSet( fnt, MyData.playtime_m, NUM_MODE_ZERO, 2 );
	NMenuMsgWrite( fnt, x + 4, y );
}

//------------------------------------------
//	��ڲ����(�Â�����)
//	In:		x,y				; �\�����W
//------------------------------------------
void FontOpPlayTimeWrite( s16 x, s16 y )
{
	u8 coron[] = { colon_, EOM_ };
	u8 fnt[16];
	u16 num;

	PM_NumMsgSet( fnt, MyData.playtime_h, NUM_MODE_LEFT, 3 );
	num = PM_strlen( fnt );

	if( num < 3 ) x++;									// 3����߰�����

	NMenuMsgWrite( fnt, x, y );

	NMenuMsgWrite( coron, x + num, y );
	PM_NumMsgSet( fnt, MyData.playtime_m, NUM_MODE_ZERO, 2 );
	NMenuMsgWrite( fnt, x + num + 1, y );
}

//------------------------------------------
//	�ޯ�ސ�
//	In:		x,y			; �\�����W
//------------------------------------------
void FontBatchWrite( s16 x, s16 y )
{
	u32 i,count;
	u8 fnt[2] = { EOM_, EOM_ };
	
	for( i = SYS_BATCH01_GET, count = 0; i <= SYS_BATCH08_GET; i++ )
	{
		if( EventFlagCheck(i) )
		{
			count++;
		}
	}
	
	fnt[0] = n0_ + count;
	
	NMenuMsgWrite( fnt, x, y );
}

//------------------------------------------
//	�}�Ӑ�
//------------------------------------------
static u16 GetMyStatusZukanGetTotal( void )
{
	if( ZukanSpecialFlagCheck() )						// �S���}��
	{
		return( ZukanTotal(Z_GET_CHK) );
	}

	return( ZukanHouenTotal(Z_GET_CHK) );
}

//======================================================================
//		�Ȉսð������޳
//======================================================================
static u8 CheckMyStatusWindowSize( void );
static void MyStWinMyName( s16, s16 );
static void MystWinMapName( s16, s16 );
static void MyStWinBatch( s16, s16 );
static void MyStWinPoke( s16, s16 );
static void MyStWinPlayTime( s16, s16 );

//------------------------------------------
//	�Ȉսð������޳�\��
//	In:		x,y				; �\�����W(�ْP��)
//------------------------------------------
void MyStatusWindow( s16 x, s16 y )
{
	u16 size;

	size = 12;
	if( CheckMyStatusWindowSize() ) size++;

	if( EventFlagCheck(SYS_ZUKAN_GET) )
	{
		NMenuWinBoxWrite( x, y, x + size, y + 11 );
		MystWinMapName( ++x, ++y );
		MyStWinMyName( x, y + 2 );
		MyStWinBatch( x, y + 4 );
		MyStWinPoke( x, y + 6 );
		MyStWinPlayTime( x, y + 8 );
	}
	else
	{
		NMenuWinBoxWrite( x, y, x + size, y + 9 );
		MystWinMapName( ++x, ++y );
		MyStWinMyName( x, y + 2 );
		MyStWinBatch( x, y + 4 );
		MyStWinPlayTime( x, y + 6 );
	}
}

//------------------------------------------
//	�Ȉսð������޳����
//	In:		x,y				; �\�����W(�ْP��)
//------------------------------------------
void MyStatusWindowClear( s16 x, s16 y )
{
	u16 size;

	size = 12;
	if( CheckMyStatusWindowSize() ) size++;

	if( EventFlagCheck(SYS_ZUKAN_GET) )
	{
		NMenuBoxClear( x, y, x + size, y + 11 );
	}
	else
	{
		NMenuBoxClear( x, y, x + size, y + 9 );
	}
}

static u8 CheckMyStatusWindowSize( void )
{
	if( GetMyStatusZukanGetTotal() >= 10 ||
		PM_strlen(MyData.my_name) > 3 || MyData.playtime_h >= 10 )
	{
		return( 1 );
	}

	return( 0 );
}

static void MyStWinMyName( s16 x, s16 y )
{
	u8 fnt[] = {si_,yyu_,zi_,n_,ko_,u_,EOM_};
	
	NMenuMsgWrite( fnt, x, y );
	FontHeroNameWrite( x + 7, y );
}

static void MystWinMapName( s16 x, s16 y )
{
	u8 fnt[32];

	ExCopyMapName( fnt, NowMap.WorldPos );
	NMenuMsgWrite( fnt, x, y );
}

static void MyStWinBatch( s16 x, s16 y )
{
	u8 fnt[] = {mo_,ttu_,te_,i_,ru_,BA_,TTU_,ZI_,EOM_};
	u8 ko[] = {ko_,EOM_};

	NMenuMsgWrite( fnt, x, y );
	FontBatchWrite( x + 9, y );
	NMenuMsgWrite( ko, x+10, y );
}

static void MyStWinPoke( s16 x, s16 y )
{
	u8 fnt[] = {PO_,KE_,MO_,N_,zu_,ka_,n_,EOM_};
	
	NMenuMsgWrite( fnt, x, y );
	FontOpPokeZukanNumWrite( x + 7, y );
}

static void MyStWinPlayTime( s16 x, s16 y )
{
	u8 fnt[] = {PU_,RE_,I_,zi_,ka_,n_,EOM_};
	
	NMenuMsgWrite( fnt, x, y );
	FontOpPlayTimeWrite( x + 6, y );
}

//======================================================================
//		��گ��ް�
//======================================================================
//------------------------------------------
//	ɰ����گ�
//------------------------------------------
static const u16 status_n_Palette[48]={	
	0x3991,0x7fff,0x6ffc,0x5294,0x3def,0x398c,0x20e5,0x45a7,
	0x6228,0x4f96,0x43b2,0x3b70,0x26aa,0x7eed,0x6739,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x6acd,0x51e5,0x0000,0x0000,
	0x398c,0x7fff,0x779c,0x5294,0x3def,0x7fff,0x779c,0x5294,
	0x3def,0x7fff,0x779c,0x5294,0x3def,0x7eed,0x6739,0x0000,
};

//------------------------------------------
//	���ݽ���گ�
//------------------------------------------
static const u16 status_b_Palette[48]={
	0x3991,0x7bfd,0x73ba,0x5294,0x3def,0x398c,0x20e5,0x45a7,
	0x6228,0x6757,0x52d2,0x3e4d,0x2dc8,0x7eed,0x6739,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x6acd,0x51e5,0x0000,0x0000,
	0x398c,0x7fff,0x779c,0x5294,0x3def,0x7fff,0x779c,0x5294,
	0x3def,0x7fff,0x779c,0x5294,0x3def,0x7eed,0x6739,0x0000,
};

//------------------------------------------
//	����-��گ�	
//------------------------------------------
static const u16 status_c_Palette[48]={
	0x3991,0x73df,0x6b9e,0x2e79,0x2216,0x398c,0x20e5,0x45a7,
	0x6228,0x46fc,0x3a79,0x2a16,0x15b3,0x7eed,0x6739,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x6acd,0x51e5,0x0000,0x0000,
	0x398c,0x7fff,0x779c,0x5294,0x3def,0x7fff,0x779c,0x5294,
	0x3def,0x7fff,0x779c,0x5294,0x3def,0x7eed,0x6739,0x0000,
};

//------------------------------------------
//	���ް��گ�
//------------------------------------------
static const u16 status_s_Palette[48]={
	0x3991,0x7fde,0x779c,0x62f7,0x4e73,0x15b3,0x20e5,0x45a7,
	0x6228,0x779c,0x62f7,0x4e73,0x3def,0x7eed,0x6739,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x6acd,0x51e5,0x0000,0x0000,
	0x398c,0x7fff,0x779c,0x5294,0x3def,0x7fff,0x779c,0x5294,
	0x3def,0x7fff,0x779c,0x5294,0x3def,0x7eed,0x6739,0x0000,
};

//------------------------------------------
//	�ް�����گ�
//------------------------------------------
static const u16 status_g_Palette[48]={
	0x3991,0x6fde,0x57be,0x5294,0x3def,0x398c,0x20e5,0x45a7,
	0x6228,0x479e,0x1b5c,0x1f1a,0x1dd3,0x7eed,0x6739,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x6acd,0x51e5,0x0000,0x0000,
	0x398c,0x7fff,0x779c,0x5294,0x3def,0x7fff,0x779c,0x5294,
	0x3def,0x7fff,0x779c,0x5294,0x3def,0x7eed,0x6739,0x0000,
};

//------------------------------------------
//	BG ��گ�ð���
//------------------------------------------
static const u16 * const StatusBgPaletteTbl[] =
{
	status_n_Palette,
	status_b_Palette,
	status_c_Palette,
	status_s_Palette,
	status_g_Palette,
};

//------------------------------------------
//	BG ���w�i��گ�
//------------------------------------------
static const u16 StatusBgPaletteBackFemale[] =
{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x45FC,0x38D4,0x0000,0x0000,
};

//------------------------------------------
//	�ޯ����گ�
//------------------------------------------
static const u16 l_badge_Palette[16]={
	0x3991,0x7fff,0x6b5a,0x5ad6,0x3def,0x1e3f,0x04fb,0x7eed,
	0x6228,0x335f,0x1edd,0x61f8,0x4d32,0x7eed,0x6739,0x0000,
};

//------------------------------------------
//	����گ�
//------------------------------------------
static const u16 star_Palette[16] = {
	0x3991,0x031e,0x0299,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

//------------------------------------------
//	�ԕ�����گ�
//------------------------------------------
static const u16 FontRedPalette[16] = {
	0x0000,0x001f,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x7c1f,0x001f,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

//------------------------------------------
//	�ƭ���گ�
//------------------------------------------
#if BUTTON_ACTOR_ON
static const u16 st_menu_Palette[16]={
	0x6b7f,0x7fff,0x73bd,0x7f57,0x6e4e,0x1b1e,0x65aa,0x7f1a,
	0x7f9d,0x5aff,0x6b7f,0x0000,0x0000,0x0000,0x65aa,0x0000,
};
#endif

//======================================================================
//		�����ް�
//======================================================================
//----
#if BUTTON_ACTOR_ON
//----
//------------------------------------------
//	OAM
//------------------------------------------
static const ActOamData oam8x8 =
{
	0,							// u32 VPos:8
	0,							// u32 AffineMode:2
	0,							// u32 ObjMode:2
	0,							// u32 Mosaic:1
	0,							// u32 ColorMode:1
	0,							// u32 Shape:2
	0,							// u32 HPos:9
	0,							// u32 AffineParamNo:5
	0,							// u32 Size:2
	0,							// u32 CharNo:10
	0,							// u32 Priority:2
	0,							// u32 Pltt:3
	0,							// AffineWork
};

//------------------------------------------
//	���ް�
//------------------------------------------
static const CellData status_CellData[] =
{
	{ st_menu_Character, 0x20*0x18, CELL_MENU },
	{ CELL_END, 0, 0 }
};

//------------------------------------------
//	��گ��ް�
//------------------------------------------
static const PalData status_PaletteData[] =
{
	{ st_menu_Palette, PAL_MENU },
	{ OBJPAL_END, 0 },
};

//------------------------------------------
//	�߯�
//------------------------------------------
static const OAMPatData status_Pat0[] =
{
	{ -20, -4, 0, 1, 0x10, 0 },							// A
	{ -4, -4, 2, 0, 0x00, 0 },							// ��
	{ 4, -4, 2, 0, 0x02, 0 },							// ��
	{ 12, -4, 2, 0, 0x04, 0 },							// ��
};

static const OAMPatData status_Pat1[] =
{
	{ -20, -4, 0, 1, 0x10, 0 },							// A
	{ -4, -4, 2, 0, 0x06, 0 },							// ��
	{ 4, -4, 2, 0, 0x08, 0 },							// ��
};

static const OAMPatData status_Pat2[] =
{
	{ -20, -4, 0, 1, 0x14, 0 },							// B
	{ -4, -4, 2, 0, 0x0a, 0 },							// ��
	{ 4, -4, 2, 0, 0x0c, 0 },							// ��
	{ 12, -4, 2, 0, 0x0e, 0 },							// ��
};

static const OAMPatSt status_Oampat[] =
{
	{ 4, status_Pat0 },
	{ 3, status_Pat1 },
	{ 4, status_Pat2 },
};

//------------------------------------------
//	���
//------------------------------------------
static const actAnm status_anm0[] =
{
	{ 0, 1, 0, 0 },
	{ ACT_ANMEND, 0, 0, 0 },
};

static const actAnm * const status_AnmTbl[] =
{
	status_anm0,
};

//------------------------------------------
//	����ͯ��
//------------------------------------------
static const actHeader a_button_ActHead =
{
	CELL_MENU,
	PAL_MENU,
	&oam8x8,
	status_AnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	A_ButtonActMove
};

static const actHeader b_button_ActHead =
{
	CELL_MENU,
	PAL_MENU,
	&oam8x8,
	status_AnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	B_ButtonActMove
};
//----
#endif
//----

//======================================================================
//		ϯ���ް�
//======================================================================
//------------------------------------------
//	�ޯ�޷��ϯ��
//------------------------------------------
static const u16 BadgeCharMap[8][4] =
{
	{
		BG_BADGE_CHNO_0+0x00,
		BG_BADGE_CHNO_0+0x01,
		BG_BADGE_CHNO_0+0x10,
		BG_BADGE_CHNO_0+0x11,
	},
	{
		BG_BADGE_CHNO_0+0x02,
		BG_BADGE_CHNO_0+0x03,
		BG_BADGE_CHNO_0+0x12,
		BG_BADGE_CHNO_0+0x13,
	},
	{
		BG_BADGE_CHNO_0+0x04,
		BG_BADGE_CHNO_0+0x05,
		BG_BADGE_CHNO_0+0x14,
		BG_BADGE_CHNO_0+0x15,
	},
	{
		BG_BADGE_CHNO_0+0x06,
		BG_BADGE_CHNO_0+0x07,
		BG_BADGE_CHNO_0+0x16,
		BG_BADGE_CHNO_0+0x17,
	},
	{
		BG_BADGE_CHNO_0+0x08,
		BG_BADGE_CHNO_0+0x09,
		BG_BADGE_CHNO_0+0x18,
		BG_BADGE_CHNO_0+0x19,
	},
	{
		BG_BADGE_CHNO_0+0x0a,
		BG_BADGE_CHNO_0+0x0b,
		BG_BADGE_CHNO_0+0x1a,
		BG_BADGE_CHNO_0+0x1b,
	},
	{
		BG_BADGE_CHNO_0+0x0c,
		BG_BADGE_CHNO_0+0x0d,
		BG_BADGE_CHNO_0+0x1c,
		BG_BADGE_CHNO_0+0x1d,
	},
	{
		BG_BADGE_CHNO_0+0x0e,
		BG_BADGE_CHNO_0+0x0f,
		BG_BADGE_CHNO_0+0x1e,
		BG_BADGE_CHNO_0+0x1f,
	}
};

//------------------------------------------
//	�ð����ʕ\ϯ���ް� 30x20
//------------------------------------------
static const u16 status1_Map[600]={
	0x0000,0x0039,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,    /* 0 */
	0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,
	0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,
	0x080c,0x080c,0x080c,0x080c,0x0439,0x0000,
	0x0038,0x0036,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,    /* 1 */
	0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,
	0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,
	0x0037,0x0037,0x0037,0x0037,0x0436,0x0438,
	0x0002,0x0014,0x048b,0x0080,0x0081,0x0082,0x0083,0x0084,    /* 2 */
	0x0085,0x0086,0x0087,0x0088,0x0089,0x008a,0x008b,0x0004,
	0x0004,0x001a,0x001b,0x0024,0x0004,0x0004,0x0004,0x0004,
	0x0004,0x0004,0x0004,0x0004,0x0414,0x0402,
	0x0002,0x0014,0x049b,0x0090,0x0091,0x0092,0x0093,0x0094,    /* 3 */
	0x0095,0x0096,0x0097,0x0098,0x0099,0x009a,0x009b,0x0004,
	0x0004,0x002a,0x002b,0x0034,0x0004,0x0004,0x0004,0x0004,
	0x0004,0x0004,0x0004,0x0004,0x0414,0x0402,
	0x0002,0x000d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,    /* 4 */
	0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,
	0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,
	0x002d,0x002d,0x002d,0x002d,0x040d,0x0402,
	0x0002,0x0013,0x0025,0x0020,0x0021,0x0022,0x0001,0x0001,    /* 5 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x000e,0x000f,0x040f,
	0x040e,0x0001,0x0001,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0035,0x0030,0x0031,0x0032,0x0001,0x0001,    /* 6 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x001c,0x001d,0x0004,0x0004,0x0004,
	0x0004,0x041d,0x041c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0001,0x0011,0x0011,0x0011,0x0011,0x0011,    /* 7 */
	0x0011,0x0011,0x0011,0x0011,0x0011,0x0011,0x0011,0x0001,
	0x0001,0x0001,0x0001,0x002c,0x0004,0x002e,0x002f,0x042f,
	0x042e,0x0004,0x042c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0025,0x0040,0x0041,0x0042,0x0043,0x0044,    /* 8 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0023,
	0x0001,0x0001,0x0001,0x003c,0x0004,0x003e,0x003f,0x043f,
	0x043e,0x0004,0x043c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0035,0x0050,0x0051,0x0052,0x0053,0x0054,    /* 9 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0033,
	0x0001,0x0001,0x0001,0x084c,0x084c,0x084e,0x0004,0x0004,
	0x0c4e,0x084c,0x0c4c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0025,0x0060,0x0061,0x0062,0x0063,0x0064,    /* 10 */
	0x0065,0x0066,0x0001,0x0001,0x0001,0x0001,0x0001,0x0045,
	0x0046,0x0001,0x0001,0x083c,0x0004,0x083e,0x083f,0x0c3f,
	0x0c3e,0x0004,0x0c3c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0035,0x0070,0x0071,0x0072,0x0073,0x0074,    /* 11 */
	0x0075,0x0076,0x0001,0x0001,0x0001,0x0001,0x0001,0x0055,
	0x0056,0x0001,0x0001,0x082c,0x0004,0x082e,0x082f,0x0c2f,
	0x0c2e,0x0004,0x0c2c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0025,0x0047,0x0048,0x0067,0x0068,0x001e,    /* 12 */
	0x001f,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x081c,0x081d,0x0004,0x0004,0x0004,
	0x0004,0x0c1d,0x0c1c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0035,0x0057,0x0058,0x0077,0x0078,0x003a,    /* 13 */
	0x003b,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x080e,0x080f,0x0c0f,
	0x0c0e,0x0001,0x0001,0x0001,0x0413,0x0402,
	0x0002,0x0014,0x0004,0x0006,0x0007,0x0008,0x0009,0x000a,    /* 14 */
	0x000b,0x0004,0x0004,0x0004,0x0004,0x0004,0x0004,0x0004,
	0x0004,0x0004,0x0004,0x0004,0x0004,0x0004,0x0004,0x0004,
	0x0004,0x0004,0x0004,0x0004,0x0414,0x0402,
	0x0002,0x0014,0x0004,0x0004,0x0415,0x0015,0x0004,0x0415,    /* 15 */
	0x0015,0x0004,0x0415,0x0015,0x0004,0x0415,0x0015,0x0004,
	0x0415,0x0015,0x0004,0x0415,0x0015,0x0004,0x0415,0x0015,
	0x0004,0x0415,0x0015,0x0004,0x0414,0x0402,
	0x0002,0x0014,0x0004,0x0004,0x0c15,0x0016,0x0004,0x0c15,    /* 16 */
	0x0017,0x0004,0x0c15,0x0018,0x0004,0x0c15,0x0019,0x0004,
	0x0c15,0x0026,0x0004,0x0c15,0x0027,0x0004,0x0c15,0x0028,
	0x0004,0x0c15,0x0029,0x0004,0x0414,0x0402,
	0x0838,0x0836,0x0837,0x0837,0x0837,0x0837,0x0837,0x0837,    /* 17 */
	0x0837,0x0837,0x0837,0x0837,0x0837,0x0837,0x0837,0x0837,
	0x0837,0x0837,0x0837,0x0837,0x0837,0x0837,0x0837,0x0837,
	0x0837,0x0837,0x0837,0x0837,0x0c36,0x0c38,
	0x0000,0x0839,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,    /* 18 */
	0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,
	0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,
	0x000c,0x000c,0x000c,0x000c,0x0c39,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 19 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

//------------------------------------------
//	�ð����ʗ�ϯ���ް�
//------------------------------------------
static const u16 status2_Map[600]={
	0x0000,0x0039,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,    /* 0 */
	0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,
	0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,
	0x080c,0x080c,0x080c,0x080c,0x0439,0x0000,
	0x0038,0x005c,0x005b,0x005b,0x005b,0x005b,0x005b,0x005b,    /* 1 */
	0x005b,0x005b,0x005b,0x005b,0x005b,0x005b,0x005b,0x005b,
	0x005b,0x005b,0x005b,0x005b,0x005b,0x005b,0x005b,0x005b,
	0x005b,0x005b,0x005b,0x005b,0x045c,0x0438,
	0x0002,0x0059,0x085a,0x085a,0x085a,0x085a,0x085a,0x085a,    /* 2 */
	0x085a,0x085a,0x085a,0x085a,0x085a,0x085a,0x085a,0x085a,
	0x085a,0x085a,0x085a,0x085a,0x085a,0x085a,0x085a,0x085a,
	0x085a,0x085a,0x085a,0x085a,0x0459,0x0402,
	0x0002,0x0059,0x0069,0x0069,0x0069,0x0069,0x0069,0x0069,    /* 3 */
	0x0069,0x0069,0x0069,0x0069,0x0069,0x0069,0x0069,0x0069,
	0x0069,0x0069,0x0069,0x0069,0x0069,0x0069,0x0069,0x0069,
	0x0069,0x0069,0x0069,0x0069,0x0459,0x0402,
	0x0002,0x007c,0x007d,0x007d,0x007d,0x007d,0x007d,0x007d,    /* 4 */
	0x007d,0x007d,0x007d,0x007d,0x007d,0x007d,0x007d,0x007d,
	0x007d,0x007d,0x007d,0x007d,0x007d,0x007d,0x007d,0x007d,
	0x007d,0x007d,0x007d,0x007d,0x047c,0x0402,
	0x0002,0x007b,0x004f,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 5 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x047b,0x0402,
	0x0002,0x007b,0x004d,0x003d,0x003d,0x003d,0x003d,0x003d,    /* 6 */
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x047b,0x0402,
	0x0002,0x007b,0x004f,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 7 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x047b,0x0402,
	0x0002,0x007b,0x004d,0x003d,0x003d,0x003d,0x003d,0x003d,    /* 8 */
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x047b,0x0402,
	0x0002,0x007b,0x004f,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 9 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x047b,0x0402,
	0x0002,0x007b,0x004d,0x003d,0x003d,0x003d,0x003d,0x003d,    /* 10 */
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x047b,0x0402,
	0x0002,0x007b,0x004f,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 11 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x047b,0x0402,
	0x0002,0x007b,0x004d,0x003d,0x003d,0x003d,0x003d,0x003d,    /* 12 */
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x047b,0x0402,
	0x0002,0x007b,0x004f,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 13 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x047b,0x0402,
	0x0002,0x007b,0x004d,0x003d,0x003d,0x003d,0x003d,0x003d,    /* 14 */
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x047b,0x0402,
	0x0002,0x007b,0x004f,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 15 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x047b,0x0402,
	0x0002,0x007b,0x004d,0x003d,0x003d,0x003d,0x003d,0x003d,    /* 16 */
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,0x003d,
	0x003d,0x003d,0x003d,0x003d,0x047b,0x0402,
	0x0838,0x0079,0x007a,0x007a,0x007a,0x007a,0x007a,0x007a,    /* 17 */
	0x007a,0x007a,0x007a,0x007a,0x007a,0x007a,0x007a,0x007a,
	0x007a,0x007a,0x007a,0x007a,0x007a,0x007a,0x007a,0x007a,
	0x007a,0x007a,0x007a,0x007a,0x0479,0x0c38,
	0x0000,0x0839,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,    /* 18 */
	0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,
	0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,
	0x000c,0x000c,0x000c,0x000c,0x0c39,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 19 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

//------------------------------------------
//	�ð����ʕ\�ʐMϯ���ް�
//------------------------------------------
static const u16 status3_Map[600]={
	0x0000,0x0039,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,    /* 0 */
	0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,
	0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,0x080c,
	0x080c,0x080c,0x080c,0x080c,0x0439,0x0000,
	0x0038,0x0036,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,    /* 1 */
	0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,
	0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,0x0037,
	0x0037,0x0037,0x0037,0x0037,0x0436,0x0438,
	0x0002,0x0014,0x048b,0x0080,0x0081,0x0082,0x0083,0x0084,    /* 2 */
	0x0085,0x0086,0x0087,0x0088,0x0089,0x008a,0x008b,0x0004,
	0x0004,0x001a,0x001b,0x0024,0x0004,0x0004,0x0004,0x0004,
	0x0004,0x0004,0x0004,0x0004,0x0414,0x0402,
	0x0002,0x0014,0x049b,0x0090,0x0091,0x0092,0x0093,0x0094,    /* 3 */
	0x0095,0x0096,0x0097,0x0098,0x0099,0x009a,0x009b,0x0004,
	0x0004,0x002a,0x002b,0x0034,0x0004,0x0004,0x0004,0x0004,
	0x0004,0x0004,0x0004,0x0004,0x0414,0x0402,
	0x0002,0x000d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,    /* 4 */
	0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,
	0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,0x002d,
	0x002d,0x002d,0x002d,0x002d,0x040d,0x0402,
	0x0002,0x0013,0x0025,0x0020,0x0021,0x0022,0x0001,0x0001,    /* 5 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x000e,0x000f,0x040f,
	0x040e,0x0001,0x0001,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0035,0x0030,0x0031,0x0032,0x0001,0x0001,    /* 6 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x001c,0x001d,0x0004,0x0004,0x0004,
	0x0004,0x041d,0x041c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0001,0x0011,0x0011,0x0011,0x0011,0x0011,    /* 7 */
	0x0011,0x0011,0x0011,0x0011,0x0011,0x0011,0x0011,0x0001,
	0x0001,0x0001,0x0001,0x002c,0x0004,0x002e,0x002f,0x042f,
	0x042e,0x0004,0x042c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0025,0x0040,0x0041,0x0042,0x0043,0x0044,    /* 8 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0023,
	0x0001,0x0001,0x0001,0x003c,0x0004,0x003e,0x003f,0x043f,
	0x043e,0x0004,0x043c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0035,0x0050,0x0051,0x0052,0x0053,0x0054,    /* 9 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0033,
	0x0001,0x0001,0x0001,0x084c,0x084c,0x084e,0x0004,0x0004,
	0x0c4e,0x084c,0x0c4c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0025,0x0060,0x0061,0x0062,0x0063,0x0064,    /* 10 */
	0x0065,0x0066,0x0001,0x0001,0x0001,0x0001,0x0001,0x0045,
	0x0046,0x0001,0x0001,0x083c,0x0004,0x083e,0x083f,0x0c3f,
	0x0c3e,0x0004,0x0c3c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0035,0x0070,0x0071,0x0072,0x0073,0x0074,    /* 11 */
	0x0075,0x0076,0x0001,0x0001,0x0001,0x0001,0x0001,0x0055,
	0x0056,0x0001,0x0001,0x082c,0x0004,0x082e,0x082f,0x0c2f,
	0x0c2e,0x0004,0x0c2c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0025,0x0047,0x0048,0x0067,0x0068,0x001e,    /* 12 */
	0x001f,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x081c,0x081d,0x0004,0x0004,0x0004,
	0x0004,0x0c1d,0x0c1c,0x0001,0x0413,0x0402,
	0x0002,0x0013,0x0035,0x0057,0x0058,0x0077,0x0078,0x003a,    /* 13 */
	0x003b,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x080e,0x080f,0x0c0f,
	0x0c0e,0x0001,0x0001,0x0001,0x0413,0x0402,
	0x0002,0x0014,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,    /* 14 */
	0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,
	0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,
	0x0404,0x0404,0x0404,0x0004,0x0414,0x0402,
	0x0002,0x0014,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,    /* 15 */
	0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,
	0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,
	0x0404,0x0404,0x0404,0x0004,0x0414,0x0402,
	0x0002,0x0014,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,    /* 16 */
	0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,
	0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,
	0x0404,0x0404,0x0c04,0x0004,0x0414,0x0402,
	0x0838,0x0849,0x084a,0x084a,0x084a,0x084a,0x084a,0x084a,    /* 17 */
	0x084a,0x084a,0x084a,0x084a,0x084a,0x084a,0x084a,0x084a,
	0x084a,0x084a,0x084a,0x084a,0x084a,0x084a,0x084a,0x084a,
	0x084a,0x084a,0x084a,0x084a,0x0c49,0x0c38,
	0x0000,0x0839,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,    /* 18 */
	0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,
	0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,0x000c,
	0x000c,0x000c,0x000c,0x000c,0x0c39,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 19 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

//------------------------------------------
//	�ð����ʔw�iϯ���ް�
//------------------------------------------
const u16 statusbg_Map[600]={
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 0 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 1 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 2 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 3 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 4 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 5 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 6 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 7 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 8 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 9 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 10 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 11 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 12 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 13 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 14 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 15 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 16 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 17 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 18 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,    /* 19 */
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
	0x1012,0x1012,0x1012,0x1012,0x1012,0x1012,
};

