//**********************************************************************
//		namein.c
//		��������
//**********************************************************************
#define		NAMEIN_H_PROTO
#include	<agb.h>
#include	"common.h"
#include	"intr.h"
#include	"actor.h"
#include	"actanm.h"
#include	"palanm.h"
#include	"poketool.h"
#include	"evobj.h"
#include	"menu.h"
#include	"task.h"
#include	"calctool.h"
#include	"namein.h"
#include	"pokeicon.h"
#include	"jiki.h"
#include	"mus_tool.h"

#include	"..\objdata\w_name.ach"
#include	"..\objdata\w_name2.ach"
#include	"..\objdata\com_i.ach"

//----------------------------------------------------------------------
//		extern
//----------------------------------------------------------------------
extern u16 KeyRepeatStartWait;

//----------------------------------------------------------------------
//		define
//----------------------------------------------------------------------
#define	FONT_NO			(MenuChrEndNo)					// ���� �������
#define	FONT_DATA		(FONT_NO<<5)					// ���� ��׻���

#define BG_CHNO_MAX		(0x40)							// BG ��א�
#define BG_CHDATA_MAX	(BG_CHNO_MAX<<5)				// BG ��׻���
#define BG_CHNO_0		(FONT_NO)						// BG ������ވʒu
#define BG0_CH_BASE		(BG_VRAM)						// BG0 ����ް�
#define BG1_CH_BASE		(BG_VRAM)						// BG1 ����ް�
#define BG2_CH_BASE		(BG_VRAM)						// BG2 ����ް�
#define BG3_CH_BASE		(BG_VRAM)						// BG3 ����ް�
#define	BG0_MAP_BASE	(BG_VRAM+0xf800)				// BG0 ϯ���ް�
#define	BG1_MAP_BASE 	(BG_VRAM+0x8000)				// BG1 ϯ���ް�
#define	BG2_MAP_BASE	(BG1_MAP_BASE+0x0800)			// BG2 ϯ���ް�
#define	BG3_MAP_BASE	(BG2_MAP_BASE+0x0800)			// BG3 ϯ���ް�
#define BG0_CH_BASE_NO	((BG0_CH_BASE-BG_VRAM)>>14)		// BG0 ����ް�����(no/4000h)
#define BG1_CH_BASE_NO	((BG1_CH_BASE-BG_VRAM)>>14)		// BG1 ����ް�����
#define BG2_CH_BASE_NO	((BG2_CH_BASE-BG_VRAM)>>14)		// BG2 ����ް�����
#define BG3_CH_BASE_NO	((BG3_CH_BASE-BG_VRAM)>>14)		// BG3 ����ް�����
#define BG0_MAP_BASE_NO	((BG0_MAP_BASE-BG_VRAM)>>11)	// BG0 ����ް�����(no/800h)
#define BG1_MAP_BASE_NO	((BG1_MAP_BASE-BG_VRAM)>>11)	// BG0 ����ް�����
#define BG2_MAP_BASE_NO	((BG2_MAP_BASE-BG_VRAM)>>11)	// BG0 ����ް�����
#define BG3_MAP_BASE_NO	((BG3_MAP_BASE-BG_VRAM)>>11)	// BG0 ����ް�����

#define BG_CH_TRANS_BASE (BG0_CH_BASE+FONT_DATA)		// BG ��ד]���̈�

#define BG_VRAMSET(m,x,y,c) m[((y)<<5)+(x)] = c			// BG��׾��
#define	BG_SCRN_DATA(ch,h,v,pal) \
		((ch)|(h<<10)|(v<<11)|(pal<<12))				// ��ذ��ް�����

#define BG_BOARD_HIRAGANA_PALNO	(0x01)					// �ް�ނ�����گ�
#define BG_BOARD_KATAKANA_PALNO	(0x02)					// �ް�޶���گ�
#define BG_BOARD_ABC_PALNO		(0x03)					// �ް�ޱ�̧�ޯ���گ�
#define BG_FONT_SPACE_NO_0		(0x3b)					// ��߰����0
#define BG_FONT_SPACE_NO_1		(0x3c)					// ��߰����1

#define	TRG		(sys.Trg)								// ���ض�
#define	CONT	(sys.Cont)								// �����ƭ
#define REPEAT	(sys.Repeat)							// ����߰�

#define DAKU_		(EOM_)								// ���_����
#define HANDAKU_	(CR_)								// �����_����

#define CURSOR_XPOS_BUTTON	(0x13)						// �������݈ʒu

enum													// ��ID
{
	NAME_CELL_RET = 0,
	NAME_CELL_END,
	NAME_CELL_SEL,
	NAME_CELL_SEL_BACK,
	NAME_CELL_HIRAGANA,
	NAME_CELL_KATAKANA,
	NAME_CELL_ABC,
	NAME_CELL_BOARD_CURSOR0,
	NAME_CELL_BOARD_CURSOR1,
	NAME_CELL_BOARD_CURSOR2,
	NAME_CELL_FONT_CURSOR,
	NAME_CELL_FONT_BAR,
};

enum													// ��گ�ID
{
	PAL_OBJ_0 = 0,
	PAL_OBJ_1,
	PAL_OBJ_2,
	PAL_OBJ_3,
	PAL_OBJ_4,
	PAL_OBJ_5,
	PAL_OBJ_6
};

enum													// �\��OBJ
{
	NAME_OBJ_NON = 0,
	NAME_OBJ_HERO,
	NAME_OBJ_BOX,
	NAME_OBJ_POKE,
};

enum													// ��̫�Ė�
{
	DEF_NAME_OFF = 0,
	DEF_NAME_ON,
};

enum													// ���ʕ\��
{
	SEX_FONT_OFF = 0,
	SEX_FONT_ON,
};

enum													// ����Ӱ��
{
	MODE_KATAKANA = 0,									// ����
	MODE_HIRAGANA,										// �Ђ炪��
	MODE_ABC,											// ��̧�ޯ�
	MODE_MAX,											// �ő�
};

enum													// ����̪���
{
	BUTTON_FADE_SELECT = 0,
	BUTTON_FADE_RET,
	BUTTON_FADE_END,
	BUTTON_FADE_NOTHING,
};

enum													// ��
{
	KEY_OFF = 0,
	KEY_ON,
};

enum													// ����̪���
{
	CURSOR_FADE_OFF = 0,
	CURSOR_FADE_ON,
};

enum													// ������
{
	KEY_NOTHING = 0,
	KEY_U,
	KEY_D,
	KEY_L,
	KEY_R,
	KEY_A,
	KEY_B,
	KEY_R_BT,
	KEY_SELECT,
	KEY_START,
};

enum													// ���َ���
{
	CURSOR_POS_BOARD = 0,
	CURSOR_POS_SELECT,
	CURSOR_POS_RET,
	CURSOR_POS_END
};

enum													// �����ײ��è
{
	TASK_PRI_BOARD_CHANGE = 0,
	TASK_PRI_KEY_CONTORL,
	TASK_PRI_NAMEIN,
	TASK_PRI_BUTTON_FADE,
};

enum													// ���ݽ����
{
	SEQNO_INIT = 0,
	SEQNO_FADE_IN,
	SEQNO_KEY_INPUT,
	SEQNO_LAST_INPUT,
	SEQNO_SET_BOARD_CHANGE,
	SEQNO_BOARD_CHANGE,
	SEQNO_SET_END,
	SEQNO_END_WINDOW_WAIT,
	SEQNO_FADE_OUT,
	SEQNO_END
};

#define FADE_SELECT_COLNO \
(OBJPALNO_TO_PANO(ObjPalNumGet(PAL_OBJ_4))+PA_COL14)	// ̪��޶װ
#define FADE_RET_COLNO \
(OBJPALNO_TO_PANO(ObjPalNumGet(PAL_OBJ_6))+PA_COL12)
#define FADE_END_COLNO \
(OBJPALNO_TO_PANO(ObjPalNumGet(PAL_OBJ_6))+PA_COL14)
#define FADE_BOARD_CURSOR_COLNO \
(OBJPALNO_TO_PANO(ObjPalNumGet(PAL_OBJ_5))+PA_COL1)

//----------------------------------------------------------------------
//		typedef
//----------------------------------------------------------------------
typedef struct
{
	u8 name_def_sw;										// ��̫�Ė��\��
	u8 name_max;										// ���͐��ő�
	u8 icon_obj;										// ����OBJ
	u8 sex_font_sw;										// ���ʕ\��
	u8 font_board_start_mode;							// �����Տ����\��
	const u8 *info_font;								// �ē�����
}NAMEDATA;

typedef struct
{
	u8	seq_no;											// ���ݽ����
	u8	name_type;										// ��������
	s16 namefont_start_x;								// �\���J�n�ʒu
	s16 bg1_y;											// BG1 Y
	s16 bg2_y;											// BG2 Y
	u16	bg1_pri;										// BG1 ��ײ��è
	u16 bg2_pri;										// BG2 ��ײ��è
	u8	bg_board_face;									// BG ������ �\
	u8	bg_board_back;									// BG ������ ��
	u8	board_mode;										// �ް��Ӱ��
	u8	actno_cursor;									// ���ٱ������
	u8	actno_select;									// �ڸ����ݱ������
	u8	name[16];										// ���O�i�[
	const NAMEDATA *namedata;							// ���͏��
	u8	*set_name_buf;									// ���O���͐�
	u16 key_repeart_wait_buf;							// ����߰ĕۑ�
	s16 arg_work0;										// �w��ܰ�0
	s16 arg_work1;										// �w��ܰ�1
	u32	arg_work2;										// �w��ܰ�2
	pFunc rec_proc;										// ���A����
}NAMEIN;

//----------------------------------------------------------------------
//		����
//----------------------------------------------------------------------
static void PushNameInTempWork( u8, u8 *, u16, u16, u32,  pFunc );
static void PopNameInTempWork( void );

static void NameInScrnOFF( void );
static void NameInScrnON( void );
static void NameInBgVramInit( void );
static void NameInOamInit( void );
static void NameInScrnInit( void );
static void NameInWorkInit( void );
static void NameInFontInit( void );
static void NameInProcInit( void );
static void NameInVramSet( void );
static void NameInActorSet( void );
static void NameInProcSet( void );

static void SetNameInTask( void );

static void SetKeyControlTask( void );
static u8 GetKeyControl( void );
static void SetKeyControl( u8 );

static void SetButtonFadeTask( void );
static void SetButtonFade( u8, u8, u8 );

static void SetFontBoardChangeTask( void );
static u8 CheckFontBoardChangeTask( void );

static void AddBoardCursorActor( void );
static void SetBoardCursorPos( s16, s16 );
static void GetBoardCursorPos( s16 *, s16 * );
static void SetBoardCursorPosEnd( void );
static void SetBoardCursorVanish( u8 );
static void SetBoardCursorFade( u8 );
static void SetBoardCursorAnime( void );
static u8 CheckBoardCursorAnime( void );
static u8 GetBoardCursorPosKind( void );
static void BoardCursorActMove( actWork * );

static void AddSelectButtonActor( void );
static void SelectButtonActMove( actWork * );
static void SetSelectButtonChange( void );

static void AddRetEndButtonActor( void );

static void AddNameCursorBarActor( void );
static void NameCursorActMove( actWork * );
static void NameBarActMove( actWork * );

static void AddIconActor( void );

static u8 GetNameInputPos( void );
static u8 GetNameChangePos( void );
static void SetNameReturn( void );
static u8 SetNameInput( void );
static void SetNameInputFugou( void );
static void SetNameBuffer( void );
static void SetNamePokeWindow( void );

static void SetObjVram( void );

static void SetBgVram( void );
static void SetMapBoardFaceBack( void );
static void SetMapBoardBack( void );
static void SetMapBack( void );
static void SetMapBoardFaceSpace( void );
static void SetMapBoardBackSpace( void );

static u8 GetFontBoardCode( s16, s16 );
static void SetFontBoardFaceBack( void );
static void SetFontBoardBack( void );
static void SetFontInfoSex( void );
static void SetFontName( void );

static const NAMEDATA * const NameData[];

static const u8 FontBoardCode[3][4][20];

static const OAMPatSt SelectOamPat[];
static const OAMPatSt SelectFontOamPat[];
static const OAMPatSt RetEndOamPat[];
static const OAMPatSt IconBoxOamPat[];

static const actHeader SelectButtonActHead;
static const actHeader SelectBackActHead;
static const actHeader SelectFontActHead;
static const actHeader RetButtonActHead;
static const actHeader EndButtonActHead;
static const actHeader BoardCursorActHead;
static const actHeader NameCursorActHead;
static const actHeader NameBarActHead;
static const actHeader IconBoxActHead;

static const u16 w_name_Palette[];
static const u16 w_name1_Map[];
static const u16 w_name2_Map[];
static const u16 w_name3_Map[];
static const u16 w_name4_Map[];

static const CellData w_name_CellData[];
static const PalData w_name_PaletteData[];

static NAMEIN * const Namein = ((NAMEIN*)UserWork);

//======================================================================
//		���O����
//======================================================================
static void NameInInit( void );
static void NameInMain( void );
static void NameInVBlank( void );

//------------------------------------------
//	���O����
//	In:		type			; ��������
//			buf				; �����i�[�ޯ̧
//			work0,1,2		; ����ܰ�
//			rec_proc		; ���A����
//------------------------------------------
void NameIn( u8 type, u8 *buf, u16 work0, u16 work1, u32 work2, pFunc rec_proc )
{
	PushNameInTempWork( type, buf, work0, work1, work2, rec_proc );
	MainProcChange( NameInInit );
}

//------------------------------------------
//	���O���� ������
//------------------------------------------
static void NameInInit( void )
{
	switch( sys.InitSeqWork )
	{
		case 0:
			NameInScrnOFF();
			NameInWorkInit();
			sys.InitSeqWork++;
			break;
		case 1:
			NameInBgVramInit();
			sys.InitSeqWork++;
			break;
		case 2:
			NameInOamInit();
			sys.InitSeqWork++;
			break;
		case 3:
			NameInScrnInit();
			sys.InitSeqWork++;
			break;
		case 4:
			NameInFontInit();
			sys.InitSeqWork++;
			break;
		case 5:
			NameInProcInit();
			sys.InitSeqWork++;
			break;
		case 6:
			NameInVramSet();
			sys.InitSeqWork++;
			break;
		case 7:
			NameInActorSet();
			NameInProcSet();
			NameInScrnON();
			MainProcChange( NameInMain );
	}
}

//------------------------------------------
//	Ҳ�
//------------------------------------------
static void NameInMain( void )
{
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

//------------------------------------------
//	V Blank
//------------------------------------------
static void NameInVBlank( void )
{
	actOAMtrans();
	actDMAtrans();
	PaletteFadeTrans();

    *(vu16 *)REG_BG1VOFS = Namein->bg1_y;
	*(vu16 *)REG_BG2VOFS = Namein->bg2_y;
	*(vu16 *)REG_BG1CNT &= 0xfff0;						// ��ײ��è�ر
	*(vu16 *)REG_BG1CNT |= Namein->bg1_pri;
	*(vu16 *)REG_BG2CNT &= 0xfff0;						// ��ײ��è�ر
	*(vu16 *)REG_BG2CNT |= Namein->bg2_pri;
}

//======================================================================
//		������ܰ�
//======================================================================
static void NameInTempWorkTask( u8 );

//------------------------------------------
//	�ް��ޔ�
//------------------------------------------
static void PushNameInTempWork( u8 type, u8 *buf, u16 w0, u16 w1, u32 w2, pFunc rec_p )
{
	u8 no;
	TASK_TABLE *task;

	no = AddTask( NameInTempWorkTask, 0xff );
	task = &TaskTable[no];

	task->work[0] = type;
	task->work[1] = w0;
	task->work[2] = w1;
	task->work[3] = (s16)(w2 >> 16);
	task->work[4] = (s16)(w2 & 0x0000ffff);
	SetAdrs_to_Work( &task->work[5], buf );
	SetAdrs_to_Work( &task->work[7], rec_p );
}

//------------------------------------------
//	�ޔ�ܰ����A
//------------------------------------------
static void PopNameInTempWork( void )
{
	u8 no;
	TASK_TABLE *task;

	no = CheckTaskNo( NameInTempWorkTask );
	task = &TaskTable[no];

	Namein->name_type = (u8)task->work[0];
	Namein->arg_work0 = task->work[1];
	Namein->arg_work1 = task->work[2];
	Namein->arg_work2 = (u32)(((task->work[3] << 16) & 0xffff0000) | (u16)task->work[4]);
	SetWork_to_Seq( &task->work[5], &Namein->set_name_buf );
	SetWork_to_Seq( &task->work[7], &Namein->rec_proc );

	DelTask( no );
}

//------------------------------------------
//	�ޔ�p���
//------------------------------------------
static void NameInTempWorkTask( u8 no )
{
}

//======================================================================
//		����������
//======================================================================
//------------------------------------------
//	��ʏ��� OFF
//------------------------------------------
static void NameInScrnOFF( void )
{
	SetVBlankFunc( NULL );
	SetHBlankFunc( NULL );
	
	*(vu16*)REG_DISPCNT = 0;
}

//------------------------------------------
//	��ʏ��� ON
//------------------------------------------
static void NameInScrnON( void )
{
	SetVBlankFunc( NameInVBlank );
	REG_IE_SET( V_BLANK_INTR_FLAG );					// V ���荞��
	*(vu16*)REG_STAT |= STAT_V_BLANK_IF_ENABLE;			// V Blank���荞�݋����׸�
	
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
static void NameInBgVramInit( void )
{
	DIV_DMACLEAR( 3, 0, BG_VRAM, BG_MODE0_VRAM_SIZE, 16 );
}

//------------------------------------------
//	OAM ������
//------------------------------------------
static void NameInOamInit( void )
{
	DIV_DMACLEAR( 3, 0, OAM,  OAM_SIZE, 16 );
}

//------------------------------------------
//	��ʏ�����
//------------------------------------------
static void NameInScrnInit( void )
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
	
	*(vu16 *)REG_BLDCNT =
		BLD_BG1_2ND | BLD_BG2_2ND | BLD_A_BLEND_MODE;

	*(vu16 *)REG_BLDALPHA = 0x080c;
}

//------------------------------------------
//	ܰ�������
//------------------------------------------
static void NameInWorkInit( void )
{
	PopNameInTempWork();								// �ޔ�ܰ����A

	Namein->seq_no = SEQNO_INIT;
	Namein->bg1_y = 0;
	Namein->bg2_y = 0;
	Namein->bg1_pri = 1;
	Namein->bg2_pri = 2;
	Namein->bg_board_face = 0;
	Namein->bg_board_back = 1;
	Namein->namedata = NameData[Namein->name_type];
	Namein->board_mode = Namein->namedata->font_board_start_mode;
	Namein->namefont_start_x = 0x0e - (Namein->namedata->name_max >> 1);
	Namein->key_repeart_wait_buf = KeyRepeatStartWait;
	memset( Namein->name, EOM_, 16 );

	if( Namein->namedata->name_def_sw )					// ��̫�Ė��\��
	{
		PM_strcpy( Namein->name, Namein->set_name_buf );
	}

	KeyRepeatStartWait = 16;
}

//------------------------------------------
//	̫�ď�����
//------------------------------------------
static void NameInFontInit( void )
{
	PrintScreenInit( NAMEIN_BG0_TYPE );
	NMenuScreenInit( NAMEIN_BG0_TYPE );
}

//------------------------------------------
//	����������
//------------------------------------------
static void NameInProcInit( void )
{
	PaletteAnimeInit();
	InitActor();
	ObjPalManInit();
	InitTask();
}

//------------------------------------------
//	Vram ���
//------------------------------------------
static void NameInVramSet( void )
{
	SetBgVram();
	SetObjVram();
	SetMapBack();
	SetMapBoardFaceBack();
	SetFontBoardFaceBack();
	SetFontInfoSex();
	SetFontName();
}

//------------------------------------------
//	������
//------------------------------------------
static void NameInActorSet( void )
{
	AddBoardCursorActor();
	AddSelectButtonActor();
	AddRetEndButtonActor();
	AddNameCursorBarActor();
	AddIconActor();
}

//------------------------------------------
//	�������
//------------------------------------------
static void NameInProcSet( void )
{
	SetKeyControlTask();
	SetButtonFadeTask();
	SetNameInTask();
}

//======================================================================
//		���O���͏���
//======================================================================
static void NameInTask( u8 );
static u8 NameIn0( TASK_TABLE * );
static u8 NameIn1( TASK_TABLE * );
static u8 NameIn2( TASK_TABLE * );
static u8 NameIn3( TASK_TABLE * );
static u8 NameIn4( TASK_TABLE * );
static u8 NameIn5( TASK_TABLE * );
static u8 NameIn6( TASK_TABLE * );
static u8 NameIn7( TASK_TABLE * );
static u8 NameIn8( TASK_TABLE * );
static u8 NameIn9( TASK_TABLE * );
static u8 NameInKeyProcess( void );
static u8 KeyProcessBoard( u8 );
static u8 KeyProcessSelect( u8 );
static u8 KeyProcessReturn( u8 );
static u8 KeyProcessEnd( u8 );
static u8 KeyProcessPushSelect( void );

//------------------------------------------
//	Ҳݏ���������
//------------------------------------------
static void SetNameInTask( void )
{
	u8 i;
	
	i = AddTask( NameInTask, TASK_PRI_NAMEIN );
	NameInTask( i );
}

//------------------------------------------
//	���O���͏���ð���
//------------------------------------------
static u8 (* const NameInTaskTbl[])( TASK_TABLE * ) =
{
	NameIn0,
	NameIn1,
	NameIn2,
	NameIn3,
	NameIn4,
	NameIn5,
	NameIn6,
	NameIn7,
	NameIn8,
	NameIn9,
};

//------------------------------------------
//	���O�������
//------------------------------------------
static void NameInTask( u8 no )
{
	while( NameInTaskTbl[Namein->seq_no](&TaskTable[no]) );
}

//------------------------------------------
//	���O���� 0 ������
//------------------------------------------
static u8 NameIn0( TASK_TABLE *task )
{
	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
	Namein->seq_no++;
	return( 0 );
}

//------------------------------------------
//	���O���� 1 ̪��޲�
//------------------------------------------
static u8 NameIn1( TASK_TABLE *task )
{
	if( FadeData.fade_sw == 0 )
	{
		SetKeyControl( KEY_ON );
		SetBoardCursorFade( CURSOR_FADE_ON );
		Namein->seq_no++;
	}
	
	return( 0 );
}

//------------------------------------------
//	���O���� 2 ������
//------------------------------------------
static u8 NameIn2( TASK_TABLE *task )
{
	return( NameInKeyProcess() );
}

//------------------------------------------
//	���O���� 3 �Ō�̓���
//------------------------------------------
static u8 NameIn3( TASK_TABLE *task )
{
	if( CheckBoardCursorAnime() )
	{
		SetKeyControl( KEY_ON );
		SetBoardCursorPosEnd();
		Namein->seq_no = SEQNO_KEY_INPUT;
	}

	return( 0 ); 
}

//------------------------------------------
//	���O���� 4 �����Ր؂�ւ����
//------------------------------------------
static u8 NameIn4( TASK_TABLE *task )
{
	SetKeyControl( KEY_OFF );
	SetSelectButtonChange();
	SetFontBoardChangeTask();
	SetBoardCursorVanish( 1 );
	SetButtonFade( BUTTON_FADE_SELECT, 0, 1 );
	SePlay( SE_WIN_OPEN );

	Namein->seq_no = SEQNO_BOARD_CHANGE;
	return( 0 );
}

//------------------------------------------
//	���O���� 5 �����Ր؂�ւ�
//------------------------------------------
static u8 NameIn5( TASK_TABLE *task )
{
	if( CheckFontBoardChangeTask() )
	{
		Namein->seq_no = SEQNO_KEY_INPUT;
		Namein->board_mode++;
		Namein->board_mode %= MODE_MAX;
		SetMapBoardBack();
		SetFontBoardBack();
		SetKeyControl( KEY_ON );
		SetBoardCursorVanish( 0 );
	}
	
	return( 0 );
}

//------------------------------------------
//	���O���� 6 �I�����
//------------------------------------------
static u8 NameIn6( TASK_TABLE *task )
{
	SetNameBuffer();
	SetKeyControl( KEY_OFF );
	SetBoardCursorFade( CURSOR_FADE_OFF );
	SetButtonFade( BUTTON_FADE_NOTHING, 0, 1 );
	KeyRepeatStartWait = Namein->key_repeart_wait_buf;

	if( Namein->name_type == NAMEIN_POKE && PokeCountMineGet() >= 6 )
	{
		SetNamePokeWindow();
		Namein->seq_no = SEQNO_END_WINDOW_WAIT;
		return( 0 );
	}

	Namein->seq_no = SEQNO_FADE_OUT;
	return( 1 );
}

//------------------------------------------
//	���O���� 7 ү����
//------------------------------------------
static u8 NameIn7( TASK_TABLE *task )
{
	if( NMenuTalkMsgPut() )
	{
		Namein->seq_no++;
	}
	
	return( 0 );
}

//------------------------------------------
//	���O���� 8 ̪��ޱ��
//------------------------------------------
static u8 NameIn8( TASK_TABLE *task )
{
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	Namein->seq_no++;
	return( 0 );
}

//------------------------------------------
//	���O���� 9 �I��
//------------------------------------------
static u8 NameIn9( TASK_TABLE *task )
{
	if( FadeData.fade_sw == 0 )
	{
		MainProcChange( Namein->rec_proc );
	}

	return( 0 );
}

//------------------------------------------
//	���O���ͷ�����ð���
//------------------------------------------
static u8 (* const NameInKeyProcTbl[])( u8 ) =
{
	KeyProcessBoard,
	KeyProcessSelect,
	KeyProcessReturn,
	KeyProcessEnd,
};

//------------------------------------------
//	���O���� ������
//------------------------------------------
static u8 NameInKeyProcess( void )
{
	u8 key,mode;

	key = GetKeyControl();
	mode = GetBoardCursorPosKind();

	if( key == KEY_SELECT )
	{
		return( KeyProcessPushSelect() );
	}

	if( key == KEY_B )
	{
		SetNameReturn();
		return( 0 );
	}
	
	if( key == KEY_R_BT )
	{
		SetNameInputFugou();
		return( 0 );
	}

	return( NameInKeyProcTbl[mode](key) );
}

//------------------------------------------
//	���@������
//------------------------------------------
static u8 KeyProcessBoard( u8 btn )
{
	u8 flag;

	SetButtonFade( BUTTON_FADE_NOTHING, 0, 0 );
	
	if( btn == KEY_A )
	{
		flag = SetNameInput();
		SetBoardCursorAnime();

		if( flag )
		{
			SetKeyControl( KEY_OFF );
			Namein->seq_no = SEQNO_LAST_INPUT;
		}
	}

	return( 0 );
}

//------------------------------------------
//	�� �ڸ�
//------------------------------------------
static u8 KeyProcessSelect( u8 btn )
{
	SetButtonFade( BUTTON_FADE_SELECT, 1, 0 );
	
	if( btn == KEY_A )
	{
		return( KeyProcessPushSelect() );
	}

	return( 0 );
}

//------------------------------------------
//	�� �߂�
//------------------------------------------
static u8 KeyProcessReturn( u8 btn )
{
	SetButtonFade( BUTTON_FADE_RET, 1, 0 );
	
	if( btn == KEY_A )
	{
		SetNameReturn();
	}

	return( 0 );
}

//------------------------------------------
//	�� �I��
//------------------------------------------
static u8 KeyProcessEnd( u8 btn )
{
	SetButtonFade( BUTTON_FADE_END, 1, 0 );
	
	if( btn == KEY_A )
	{
		SePlay( SE_SELECT );
		Namein->seq_no = SEQNO_SET_END;
		return( 1 );
	}

	return( 0 );
}

//------------------------------------------
//	�� �ڸ����� ����
//------------------------------------------
static u8 KeyProcessPushSelect( void )
{
	Namein->seq_no = SEQNO_SET_BOARD_CHANGE;
	return( 1 );
}

//======================================================================
//		������
//======================================================================
static void KeyControlTask( u8 no );
static void KeyControl0( TASK_TABLE * );
static void KeyControl1( TASK_TABLE * );
static void KeyControlCursor( TASK_TABLE * );

//------------------------------------------
//	������������
//------------------------------------------
static void SetKeyControlTask( void )
{
	AddTask( KeyControlTask, TASK_PRI_KEY_CONTORL );
}

//------------------------------------------
//	���ݎ擾
//------------------------------------------
static u8 GetKeyControl( void )
{
	u8 i;

	i = CheckTaskNo( KeyControlTask );
	return( (u8)TaskTable[i].work[1] );
}

//------------------------------------------
//	�����쾯�
//------------------------------------------
static void SetKeyControl( u8 sw )
{
	u8 i;

	i = CheckTaskNo( KeyControlTask );
	TaskTable[i].work[0] = sw;
}

//------------------------------------------
//	�����͏���ð���
//------------------------------------------
static void (* const SetKeyControlTaskTbl[])( TASK_TABLE * ) =
{
	KeyControl0,
	KeyControl1,
};

//------------------------------------------
//	�����ͺ��۰����
//------------------------------------------
static void KeyControlTask( u8 no )
{
	SetKeyControlTaskTbl[TaskTable[no].work[0]]( &TaskTable[no] );
}

//------------------------------------------
//	������ 0
//------------------------------------------
static void KeyControl0( TASK_TABLE *task )
{
	task->work[1] = KEY_NOTHING;
}

//------------------------------------------
//	������ 1
//------------------------------------------
static void KeyControl1( TASK_TABLE *task )
{
	task->work[1] = KEY_NOTHING;

	if( TRG & A_BUTTON )
	{
		task->work[1] = KEY_A;
		return;
	}

	if( TRG & B_BUTTON )
	{
		task->work[1] = KEY_B;
		return;
	}

	if( TRG & SELECT_BUTTON )
	{
		task->work[1] = KEY_SELECT;
		return;
	}

	if( TRG & START_BUTTON )
	{
		task->work[1] = KEY_START;
		SetBoardCursorPosEnd();
		return;
	}
	
	if( TRG & R_BUTTON )
	{
		task->work[1] = KEY_R_BT;
		return;
	}

	KeyControlCursor( task );
}

//------------------------------------------
//	������ ����
//------------------------------------------
static void KeyControlCursor( TASK_TABLE *task )
{
	u8 key;
	s16 x,y,old_x;
	s16 xtbl[] = { 0, 0, 0, -1, 1 };
	s16 ytbl[] = { 0, -1, 1, 0, 0 };
	s16 y_bt_pos[] = { 0, 1, 1, 2 };
	s16 y_ret_pos[] = { 0, 0, 3 };

	GetBoardCursorPos( &x, &y );

	key = KEY_NOTHING;
	if( REPEAT & U_KEY ) key = KEY_U;
	if( REPEAT & D_KEY ) key = KEY_D;
	if( REPEAT & L_KEY ) key = KEY_L;
	if( REPEAT & R_KEY ) key = KEY_R;

	old_x = x;
	x += xtbl[key];
	y += ytbl[key];

	if( x < 0 ) x = CURSOR_XPOS_BUTTON;
	if( x > CURSOR_XPOS_BUTTON ) x = 0;

	if( xtbl[key] )
	{
		if( Namein->board_mode != MODE_ABC )
		{
			if( x == (CURSOR_XPOS_BUTTON - 1) )			// ײݔ�΂�
			{
				if( xtbl[key] > 0 ) x++;
				else x--;
			}
		}

		if( x == CURSOR_XPOS_BUTTON )					// ����
		{
			task->work[2] = y;
			y = y_bt_pos[y];
		}
		else if( old_x == CURSOR_XPOS_BUTTON )			// ����
		{
			if( y == 1 ) y = task->work[2];
			else y = y_ret_pos[y];
		}
	}

	if( x == CURSOR_XPOS_BUTTON )
	{
		if( y < 0 ) y = 2;
		if( y > 2 ) y = 0;

		if( y == 0 ) task->work[2] = 1;
		else if( y == 2 ) task->work[2] = 2;
	}
	else
	{
		if( y < 0 ) y = 3;
		if( y > 3 ) y = 0;
	}

	SetBoardCursorPos( x, y );
}

//======================================================================
//		����̪���
//======================================================================
static void ButtonFadeTask( u8 no );
static u16 GetButtonFadeColorNo( u8 );
static void PopButtonFadeColor( u8 );
static void SetButtonFadeWork( TASK_TABLE *, u8, u8 );

//------------------------------------------
//	����̪���������
//------------------------------------------
static void SetButtonFadeTask( void )
{
	u8 i;

	i = AddTask( ButtonFadeTask, TASK_PRI_BUTTON_FADE );
	TaskTable[i].work[0] = BUTTON_FADE_NOTHING;
}

//------------------------------------------
//	����̪��޾��
//------------------------------------------
static void SetButtonFade( u8 kind, u8 loop, u8 force )
{
	u8 i;
	TASK_TABLE *task;

	i = CheckTaskNo( ButtonFadeTask );
	task = &TaskTable[i];

	if( kind == task->work[0] && force == 0 )
	{
		task->work[1] = loop;
		task->work[2] = 1;
		return;
	}

	if( kind == BUTTON_FADE_NOTHING &&
		task->work[1] == 0 && force == 0 )
	{
		return;
	}

	if( task->work[0] != BUTTON_FADE_NOTHING )
	{
		PopButtonFadeColor( task->work[0] );
	}

	SetButtonFadeWork( task, kind, loop );
}

//------------------------------------------
//	����̪������
//------------------------------------------
static void ButtonFadeTask( u8 no )
{
	u16 colno;
	TASK_TABLE *task;
	
	task = &TaskTable[no];

	if( task->work[0] == BUTTON_FADE_NOTHING )
	{
		return;
	}
	
	if( task->work[2] )									// loop
	{
		colno = GetButtonFadeColorNo( task->work[0] );
		ColorFadeUp( colno, task->work[3], task->work[3], task->work[3] );
		
		if( task->work[5] )								// wait
		{
			task->work[5]--;
		}
		
		if( task->work[5] == 0 )
		{
			task->work[5] = 2;
			task->work[3] += task->work[4];				// evy
			
			if( task->work[3] == 16 )
			{
				task->work[4] = -task->work[4];
			}
			else if( task->work[3] == 0 )
			{
				task->work[2] = task->work[1];			// loop
				task->work[4] = -task->work[4];
			}
		}
	}
}

//------------------------------------------
//	����̪��� �װNo
//------------------------------------------
static u16 GetButtonFadeColorNo( u8 kind )
{
	u16 tbl[] = { FADE_SELECT_COLNO, FADE_RET_COLNO, FADE_END_COLNO };

	return( tbl[kind] );
}

//------------------------------------------
//	����̪��� ���̐F�ɖ߂�
//------------------------------------------
static void PopButtonFadeColor( u8 kind )
{
	u16 palno;
	
	palno = GetButtonFadeColorNo( kind );
	PaletteWorkTrans[palno] = PaletteWork[palno];
}

//------------------------------------------
//	����̪���ܰ����
//------------------------------------------
static void SetButtonFadeWork( TASK_TABLE *task, u8 kind, u8 loop )
{
	task->work[0] = kind;
	task->work[1] = loop;
	task->work[2] = 1;
	task->work[3] = 15;
	task->work[4] = 1;
	task->work[5] = 0;
}

//======================================================================
//		�����Ր؂�ւ�
//======================================================================
static void FontBoardChangeTask( u8 );
static u8 FontBoardChange0( TASK_TABLE * );
static u8 FontBoardChange1( TASK_TABLE * );
static u8 FontBoardChange2( TASK_TABLE * );
static u8 FontBoardChange3( TASK_TABLE * );

//------------------------------------------
//	�����Ր؂�ւ�������
//------------------------------------------
static void SetFontBoardChangeTask( void )
{
	u8 i;

	i = AddTask( FontBoardChangeTask, TASK_PRI_BOARD_CHANGE );
	FontBoardChangeTask( i );
}

//------------------------------------------
//	�����Ր؂�ւ�����
//------------------------------------------
static u8 CheckFontBoardChangeTask( void )
{
	if( CheckTaskNo(FontBoardChangeTask) == NOT_EXIST_TASK_ID )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	�����Ր؂�ւ�����ð���
//------------------------------------------
static u8 (* const FontBoardChangeTaskTbl[])( TASK_TABLE * ) =
{
	FontBoardChange0,
	FontBoardChange1,
	FontBoardChange2,
	FontBoardChange3,
};

//------------------------------------------
//	�����Ր؂�ւ�
//------------------------------------------
static void FontBoardChangeTask( u8 no )
{
	while( FontBoardChangeTaskTbl[TaskTable[no].work[0]](&TaskTable[no]) );
}

//------------------------------------------
//	�����Ր؂�ւ� 0
//------------------------------------------
static u8 FontBoardChange0( TASK_TABLE *task )
{
	Namein->bg1_y = 0;
	Namein->bg2_y = 0;

	task->work[0]++;
	return( 0 );
}

//------------------------------------------
//	�����Ր؂�ւ� 1
//------------------------------------------
static u8 FontBoardChange1( TASK_TABLE *task )
{
	u8 dmy;
	s16 *pos[2] = { &Namein->bg2_y, &Namein->bg1_y };

	task->work[1] += 4;
	*pos[Namein->bg_board_face] = SinMove( task->work[1], 40 );
	*pos[Namein->bg_board_back] = SinMove( ((task->work[1]+128)&0xff), 40 );
	
	if( task->work[1] >= 64 )
	{
		dmy = Namein->bg1_pri;
		Namein->bg1_pri = Namein->bg2_pri;
		Namein->bg2_pri = dmy;
		
		task->work[0]++;
	}	
	
	return( 0 );
}

//------------------------------------------
//	�����Ր؂�ւ� 2
//------------------------------------------
static u8 FontBoardChange2( TASK_TABLE *task )
{
	u8 dmy;
	s16 *pos[2] = { &Namein->bg2_y, &Namein->bg1_y };

	task->work[1] += 4;
	*pos[Namein->bg_board_face] = SinMove( task->work[1], 40 );
	*pos[Namein->bg_board_back] = SinMove( ((task->work[1]+128)&0xff), 40 );

	if( task->work[1] >= 128 )
	{
		dmy = Namein->bg_board_face;
		Namein->bg_board_face = Namein->bg_board_back;
		Namein->bg_board_back = dmy;

		task->work[0]++;								// 1sync�҂�
	}

	return( 0 );
}

//------------------------------------------
//	�����Ր؂�ւ� 3
//------------------------------------------
static u8 FontBoardChange3( TASK_TABLE *task )
{
	DelTask( CheckTaskNo(FontBoardChangeTask) );
	return( 0 );
}

//======================================================================
//		�����ն���
//======================================================================
#define CURSOR_X_DEF ((0x03<<3)+4)
#define CURSOR_Y_DEF ((0x0a<<3)+0)

//------------------------------------------
//	�����ն��ٱ������
//------------------------------------------
static void AddBoardCursorActor( void )
{
	Namein->actno_cursor = AddActor( &BoardCursorActHead, 0, 0, 0 );
	ActWork[Namein->actno_cursor].oamData.Priority = 1;
	ActWork[Namein->actno_cursor].oamData.ObjMode = ST_OAM_OBJ_BLEND;
	ActWork[Namein->actno_cursor].work[6] = 1;
	ActWork[Namein->actno_cursor].work[6] = 2;
	SetBoardCursorPos( 0, 0 );
}

//------------------------------------------
//	���ٱ����ʒu���
//------------------------------------------
static void SetBoardCursorPos( s16 x, s16 y )
{
	actWork *act;

	act = &ActWork[Namein->actno_cursor];
	act->x = (x << 3) + CURSOR_X_DEF;
	act->y = (y << 4) + CURSOR_Y_DEF;
	act->work[2] = act->work[0];
	act->work[3] = act->work[1];
	act->work[0] = x;
	act->work[1] = y;
}

//------------------------------------------
//	���ٱ����ʒu�擾
//------------------------------------------
static void GetBoardCursorPos( s16 *x, s16 *y )
{
	actWork *act;
	
	act = &ActWork[Namein->actno_cursor];
	*x = act->work[0];
	*y = act->work[1];
}

//------------------------------------------
//	���ٱ����I���ʒu��
//------------------------------------------
static void SetBoardCursorPosEnd( void )
{
	SetBoardCursorPos( CURSOR_XPOS_BUTTON, 2 );
}

//------------------------------------------
//	���ٱ�����Ƽݸ�
//------------------------------------------
static void SetBoardCursorVanish( u8 sw )
{
	ActWork[Namein->actno_cursor].work[4] &= 0xff00;
	ActWork[Namein->actno_cursor].work[4] |= sw;
	ActAnmChg( &ActWork[Namein->actno_cursor], 0 );
}

//------------------------------------------
//	���ٱ���̪��޾��
//------------------------------------------
static void SetBoardCursorFade( u8 sw )
{
	ActWork[Namein->actno_cursor].work[4] &= 0x00ff;
	ActWork[Namein->actno_cursor].work[4] |= sw << 8;
}

//------------------------------------------
//	���ٱ�����Ҿ��
//------------------------------------------
static void SetBoardCursorAnime( void )
{
	ActAnmChg( &ActWork[Namein->actno_cursor], 1 );
}

//------------------------------------------
//	���ٱ����������
//------------------------------------------
static u8 CheckBoardCursorAnime( void )
{
	return( ActWork[Namein->actno_cursor].anmend_sw );
}

//------------------------------------------
//	���وʒu��ގ擾
//------------------------------------------
static u8 GetBoardCursorPosKind( void )
{
	s16 x,y;
	u8 tbl[] = { CURSOR_POS_SELECT, CURSOR_POS_RET, CURSOR_POS_END };

	GetBoardCursorPos( &x, &y );

	if( x < CURSOR_XPOS_BUTTON )
	{
		return( CURSOR_POS_BOARD );
	}

	return( tbl[y] );
}

//------------------------------------------
//	���ٱ�������
//------------------------------------------
static void BoardCursorActMove( actWork *act )
{
	u8 evy_r,evy_gb;

	if( act->anmend_sw )
	{
		ActAnmChg( act, 0 );
	}

	act->banish = act->work[4] & 0x00ff;

	if( act->work[0] == CURSOR_XPOS_BUTTON )
	{
		act->banish = 1;
	}

	if( act->banish || (act->work[4] & 0xff00) == 0 ||
		act->work[0] != act->work[2] || act->work[1] != act->work[3] )
	{
		act->work[5] = 0;
		act->work[6] = 1;
		act->work[7] = 2;
	}

	act->work[7]--;

	if( act->work[7] == 0 )
	{
		act->work[5] += act->work[6];
		
		if( act->work[5] == 16 || act->work[5] == 0 )
		{
			act->work[6] = -act->work[6];
		}
		
		act->work[7] = 2;
	}
	
	if( (act->work[4] & 0xff00) )
	{
		evy_gb = act->work[5];
		evy_r = act->work[5] >> 1;
		ColorFadeUp( FADE_BOARD_CURSOR_COLNO, evy_r, evy_gb, evy_gb );
	}
}
 
//======================================================================
//		�ڸ�����
//======================================================================
static u8 SelectButton0( actWork * );
static u8 SelectButton1( actWork * );
static u8 SelectButton2( actWork * );
static u8 SelectButton3( actWork * );
static void SetSelectPartsStatus( u8, actWork *, actWork * );

//------------------------------------------
//	�ڸ����ݱ������
//------------------------------------------
static void AddSelectButtonActor( void )
{
	u8 i;
	actWork *act;
	
	i = AddActor( &SelectButtonActHead, (0x19*8)+4, (0x0a*8), 0 );
	Namein->actno_select = i;
	act = &ActWork[i];
	SetActOamPat( act, SelectOamPat );
	
	i = AddActor( &SelectFontActHead, (0x19*8)+4, (0x0a*8)-4, 1 );
	act->work[6] = i;
	SetActOamPat( &ActWork[i], SelectFontOamPat );

	i = AddActor( &SelectBackActHead, (0x19*8)+4, (0x0a*8)-5, 2 );
	ActWork[i].oamData.Priority = 1;
	act->work[7] = i;
}

//------------------------------------------
//	�ڸ����ݱ�Ҿ��
//------------------------------------------
static void SetSelectButtonChange( void )
{
	actWork *act;

	act = &ActWork[Namein->actno_select];
	act->work[0] = 2;
	act->work[1] = Namein->board_mode;
}

//------------------------------------------
//	�ڸ����ݏ���
//------------------------------------------
static u8 (* const SelectButtonActMoveTbl[])( actWork * ) =
{
	SelectButton0,
	SelectButton1,
	SelectButton2,
	SelectButton3
};

//------------------------------------------
//	�ڸ����ݱ���
//------------------------------------------
static void SelectButtonActMove( actWork *act )
{
	while( SelectButtonActMoveTbl[act->work[0]](act) );
}

//------------------------------------------
//	�ڸĔw�i��گ�
//------------------------------------------
static const u16 SelectBackPalette[] =
{
	PAL_OBJ_1,											// ����
	PAL_OBJ_3,											// ABC
	PAL_OBJ_2,											// ��
};

//------------------------------------------
//	�ڸĕ�����
//------------------------------------------
static const u16 SelectFontCell[] =
{
	NAME_CELL_HIRAGANA,
	NAME_CELL_ABC,
	NAME_CELL_KATAKANA,
};

//------------------------------------------
//	�ڸ�0
//------------------------------------------
static u8 SelectButton0( actWork *act )
{
	actWork *font,*back;
	
	font = &ActWork[act->work[6]];
	back = &ActWork[act->work[7]];
	SetSelectPartsStatus( Namein->board_mode, font, back );

	act->work[0]++;
	return( 0 );
}

//------------------------------------------
//	�ڸ�1
//------------------------------------------
static u8 SelectButton1( actWork *act )
{
	return( 0 );
}

//------------------------------------------
//	�ڸ�2
//------------------------------------------
static u8 SelectButton2( actWork *act )
{
	actWork *font,*back;

	font = &ActWork[act->work[6]];
	back = &ActWork[act->work[7]];

	font->dy++;
	
	if( font->dy >= 8 )
	{
		act->work[0]++;
		font->dy = -4;
		font->banish = 1;
		SetSelectPartsStatus( ((u8)act->work[1]+1)%MODE_MAX, font, back );
	}

	return( 0 );
}

//------------------------------------------
//	�ڸ�3
//------------------------------------------
static u8 SelectButton3( actWork *act )
{
	actWork *font;

	font = &ActWork[act->work[6]];

	font->banish = 0;
	font->dy++;
	
	if( font->dy >= 0 )
	{
		font->dy = 0;
		act->work[0] = 1;
	}
	
	return( 0 );
}

//------------------------------------------
//	�ڸ��߰¾��
//------------------------------------------
static void SetSelectPartsStatus( u8 mode, actWork *font, actWork *back )
{
	back->oamData.Pltt = ObjPalNumGet( SelectBackPalette[mode] );
	font->char_num = CellCharNumGet( SelectFontCell[mode] );
	font->oampat_no = mode;
}

//======================================================================
//		�߂�I�����ݱ���
//======================================================================
//------------------------------------------
//	�߂�I�鱸�����
//------------------------------------------
static void AddRetEndButtonActor( void )
{
	u8 i;

	i = AddActor( &RetButtonActHead, (0x19*8)+4, (0x0d*8)+4, 0 );
	SetActOamPat( &ActWork[i], RetEndOamPat );
	i = AddActor( &EndButtonActHead, (0x19*8)+4, (0x10*8)+4, 0 );
	SetActOamPat( &ActWork[i], RetEndOamPat );
};

//======================================================================
//		���O����,�_
//======================================================================
//------------------------------------------
//	���O����,�_�����
//------------------------------------------
static void AddNameCursorBarActor( void )
{
	u8 i,no;
	s16 x;
	
	x = ((Namein->namefont_start_x - 1) << 3) + 4;
	no = AddActor( &NameCursorActHead, x, (5 << 3) + 4 - 4,  0 );
	ActWork[no].oamData.Priority = 3;

	x = (Namein->namefont_start_x << 3) + 4;

	for( i = 0; i < Namein->namedata->name_max; i++, x += 8 )
	{
		no = AddActor( &NameBarActHead, x, (5 << 3) + 4, 0 );
		ActWork[no].oamData.Priority = 3;
		ActWork[no].work[0] = i;
	}
}

//------------------------------------------
//	���O���ٓ���
//------------------------------------------
static void NameCursorActMove( actWork *act )
{
	s16 tbl[] = { 0, -4, -2, -1 };

	if( act->work[0] )
	{
		act->work[0]--;
	}

	if( act->work[0] == 0 )
	{
		act->work[0] = 8;
		act->work[1] = (act->work[1] + 1) & 0x03;
	}

	act->dx = tbl[act->work[1]];
}

//------------------------------------------
//	���O�ް����
//------------------------------------------
static void NameBarActMove( actWork *act )
{
	u8 i;
	s16 tbl[] = { 2, 3, 2, 1 };

	i = GetNameInputPos();
	
	if( i != (u8)act->work[0] )
	{
		act->dy = 0;
		act->work[1] = 0;
		act->work[2] = 0;
	}
	else
	{
		act->dy = tbl[act->work[1]];
		act->work[2]++;

		if( act->work[2] > 8 )
		{
			act->work[1] = (act->work[1] + 1) & 0x03;
			act->work[2] = 0;
		}
	}
}

//======================================================================
//		����OBJ
//======================================================================
static void SetIconNothing( void );
static void SetIconHero( void );
static void SetIconBox( void );
static void SetIconPoke( void );

//------------------------------------------
//	���ݱ������ð���
//------------------------------------------
static void (* const SetIconActorTbl[])( void ) =
{
	SetIconNothing,										// NAME_OBJ_NON
	SetIconHero,										// NAME_OBJ_HERO
	SetIconBox,											// NAME_OBJ_BOX
	SetIconPoke,										// NAME_OBJ_POKE
};
	
//------------------------------------------
//	���ݱ������
//------------------------------------------
static void AddIconActor( void )
{
	SetIconActorTbl[Namein->namedata->icon_obj]();
}

//------------------------------------------
//	���� ����
//------------------------------------------
static void SetIconNothing( void )
{
}

//------------------------------------------
//	���� ��l��
//------------------------------------------
static void SetIconHero( void )
{
	u8 i;
	u8 code;

	code = GetHeroSexObjCode( H_NORMAL, Namein->arg_work0 );
	i = EvObjMakeAddAct( code, DummyActMove, 7 << 3, 3 << 3, 0 );
	ActWork[i].oamData.Priority = 3;
	ActAnmChg( &ActWork[i], WALK_D_1_ST );
}

//------------------------------------------
//	���� �ޯ��
//------------------------------------------
static void SetIconBox( void )
{
	u8 i;
	
	i = AddActor( &IconBoxActHead, (7<<3)-4, (3<<3), 0 );
	SetActOamPat( &ActWork[i], IconBoxOamPat );
	ActWork[i].oamData.Priority = 3;
}

//------------------------------------------
//	���� �߹��
//------------------------------------------
static void SetIconPoke( void )
{
	u8 i;
	
	PokeIconPaletteAllSet();
	
//	i = PokeIconSet( Namein->arg_work0, DummyActMove, (7<<3)-4, (3<<3), 0 );
	i = PokeIconSet2( Namein->arg_work0, DummyActMove, (7<<3)-4, (3<<3), 0, Namein->arg_work2 );
	ActWork[i].oamData.Priority = 3;
}

//======================================================================
//		��������
//======================================================================
static u8 SetNameDakuten( void );
static u8 SetNameHandakuten( void );
static void SetNameInputFont( u8 );
static u8 CheckNameDakuten( u8 );
static u8 CheckNameHandakuten( u8 );
static u8 CheckNameSetDakuten( u8 );
static u8 CheckNameSetHandakuten( u8 );
static void SetNameInputDakuten( u8, u8 );
static void SetNameInputHandakuten( u8, u8 );
static void SetNamePutBackDakuten( u8, u8 );
static void SetNamePutBackHandakuten( u8, u8 );

//------------------------------------------
//	�������͈ʒu
//------------------------------------------
static u8 GetNameInputPos( void )
{
	u8 i;
	
	for( i = 0; i < Namein->namedata->name_max; i++ )
	{
		if( Namein->name[i] == EOM_ )
		{
			return( i );
		}
	}
	
	return( Namein->namedata->name_max - 1 );
}

//------------------------------------------
//	�����ύX�ʒu
//------------------------------------------
static u8 GetNameChangePos( void )
{
	s8 i;
	
	for( i = Namein->namedata->name_max - 1; i > 0; i-- )
	{
		if( Namein->name[i] != EOM_ )
		{
			return( i );
		}
	}
	
	return( 0 );
}

//------------------------------------------
//	�����߂�
//------------------------------------------
static void SetNameReturn( void )
{
	u8 pos;
	u8 mode;

	pos = GetNameChangePos();

	Namein->name[pos] = spc_;							// ���͈ʒu��������
	SetFontName();
	Namein->name[pos] = EOM_;
	
	mode = GetBoardCursorPosKind();
	
	if( mode == CURSOR_POS_BOARD || mode == CURSOR_POS_RET )
	{
		SetButtonFade( BUTTON_FADE_RET, 0, 1 );
	}

	SePlay( SE_BOWA );
}

//------------------------------------------
//	��������
//------------------------------------------
static u8 SetNameInput( void )
{
	u8 font,flag;
	s16 x,y;

	GetBoardCursorPos( &x, &y );
	font = GetFontBoardCode( x, y );
	
	flag = 1;

	if( font == DAKU_ )
	{
		flag = SetNameDakuten();
	}
	else if( font == HANDAKU_ )
	{
		flag = SetNameHandakuten();
	}
	else
	{
		SetNameInputFont( font );
	}

	SetFontName();
	SePlay( SE_SELECT );
	
	if( flag &&	GetNameChangePos() == (Namein->namedata->name_max - 1) )
	{
		return( 1 );									// ���ͱ� & �ő�
	}

	return( 0 );
}

//------------------------------------------
//	R ���݂ɂ�镄������
//------------------------------------------
static void SetNameInputFugou( void )
{
	u8 pos,font;
	
	pos = GetNameChangePos();
	font = Namein->name[pos];
	
	if( CheckNameDakuten(font) )
	{
		if( CheckNameSetHandakuten(font) )				// �����_�\
		{
			SetNameInputHandakuten( font, pos );
		}
		else
		{
			SetNamePutBackDakuten( font, pos );
		}
	}
	else if( CheckNameHandakuten(font) )
	{
		SetNamePutBackHandakuten( font, pos );
	}
	else
	{
		SetNameInputDakuten( font, pos );
	}

	SetFontName();
	SePlay( SE_SELECT );
}

//------------------------------------------
//	���_���
//------------------------------------------
static u8 SetNameDakuten( void )
{
	u8 pos,font;

	pos = GetNameChangePos();
	font = Namein->name[pos];

	if( CheckNameSetDakuten(font) )
	{
		SetNameInputDakuten( font, pos );
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	�����_���
//------------------------------------------
static u8 SetNameHandakuten( void )
{
	u8 pos,font;

	pos = GetNameChangePos();
	font = Namein->name[pos];

	if( CheckNameSetHandakuten(font) )
	{
		SetNameInputHandakuten( font, pos );
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	�������
//------------------------------------------
static void SetNameInputFont( u8 font )
{
	u8 pos;

	pos = GetNameInputPos();
	Namein->name[pos] = font;
}

//------------------------------------------
//	���_����
//------------------------------------------
static u8 CheckNameDakuten( u8 font )
{
	if( (font >= ga_ && font <= go_) ||
		(font >= za_ && font <= zo_) ||
		(font >= da_ && font <= do_) ||
		(font >= ba_ && font <= bo_) ||
		(font >= GA_ && font <= GO_) ||
		(font >= ZA_ && font <= ZO_) ||
		(font >= DA_ && font <= DO_) ||
		(font >= BA_ && font <= BO_) )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	�����_����
//------------------------------------------
static u8 CheckNameHandakuten( u8 font )
{
	if( (font >= pa_ && font <= po_) ||
		(font >= PA_ && font <= PO_) )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	���_�\����
//------------------------------------------
static u8 CheckNameSetDakuten( u8 font )
{
	if( (font >= ka_ && font <= to_) ||
		(font >= ha_ && font <= ho_) ||
		(font >= pa_ && font <= po_) ||
		(font >= KA_ && font <= TO_) ||
		(font >= HA_ && font <= HO_) ||
		(font >= PA_ && font <= PO_) )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	�����_�\����
//------------------------------------------
static u8 CheckNameSetHandakuten( u8 font )
{
	if( (font >= ha_ && font <= ho_) ||
		(font >= ba_ && font <= bo_) ||
		(font >= HA_ && font <= HO_) ||
		(font >= BA_ && font <= BO_) )
	{
		return( 1 );
	}
	
	return( 0 );
}

//------------------------------------------
//	���_���
//------------------------------------------
static void SetNameInputDakuten( u8 font, u8 pos )
{
	if( font >= ka_ && font <= ko_ )		font = (font - ka_) + ga_;
	else if( font >= sa_ && font <= so_ )	font = (font - sa_) + za_;
	else if( font >= ta_ && font <= to_ )	font = (font - ta_) + da_;
	else if( font >= ha_ && font <= ho_ )	font = (font - ha_) + ba_;
	else if( font >= pa_ && font <= po_ )	font = (font - pa_) + ba_;
	else if( font >= KA_ && font <= KO_ )	font = (font - KA_) + GA_;
	else if( font >= SA_ && font <= SO_ )	font = (font - SA_) + ZA_;
	else if( font >= TA_ && font <= TO_ )	font = (font - TA_) + DA_;
	else if( font >= HA_ && font <= HO_ )	font = (font - HA_) + BA_;
	else if( font >= PA_ && font <= PO_ )	font = (font - PA_) + BA_;

	Namein->name[pos] = font;
}

//------------------------------------------
//	�����_���
//------------------------------------------
static void SetNameInputHandakuten( u8 font, u8 pos )
{
	if( font >= ha_ && font <= ho_ ) 		font = (font - ha_) + pa_;
	else if( font >= ba_ && font <= bo_ )	font = (font - ba_) + pa_;
	else if( font >= HA_ && font <= HO_ )	font = (font - HA_) + PA_;
	else if( font >= BA_ && font <= BO_ )	font = (font - BA_) + PA_;

	Namein->name[pos] = font;
}

//------------------------------------------
//	���_�߂�
//------------------------------------------
static void SetNamePutBackDakuten( u8 font, u8 pos )
{
	if( font >= ga_ && font <= go_ )		font = (font - ga_) + ka_;
	else if( font >= za_ && font <= zo_ )	font = (font - za_) + sa_;
	else if( font >= da_ && font <= do_ )	font = (font - da_) + ta_;
	else if( font >= ba_ && font <= bo_ )	font = (font - ba_) + ha_;
	else if( font >= GA_ && font <= GO_ )	font = (font - GA_) + KA_;
	else if( font >= ZA_ && font <= ZO_ )	font = (font - ZA_) + SA_;
	else if( font >= DA_ && font <= DO_ )	font = (font - DA_) + TA_;
	else if( font >= BA_ && font <= BO_ )	font = (font - BA_) + HA_;
	
	Namein->name[pos] = font;
}

//------------------------------------------
//	�����_�߂�
//------------------------------------------
static void SetNamePutBackHandakuten( u8 font, u8 pos )
{
	if( font >= pa_ && font <= po_ )	font = (font - pa_) + ha_;
	else if( font >= PA_ && font <= PO_ )	font = (font - PA_) + HA_;
	
	Namein->name[pos] = font;
}

//------------------------------------------
//	���͕����ޯ̧���
//------------------------------------------
static void SetNameBuffer( void )
{
	u8 i;
	
	for( i = 0; i < Namein->namedata->name_max; i++ )
	{
		if( Namein->name[i] != spc_ && Namein->name[i] != EOM_ )
		{
			PM_strncpy( Namein->set_name_buf, Namein->name, Namein->namedata->name_max + 1 );
			break;
		}
	}
}

//------------------------------------------
//	�߹�ݓ]������޳����
//------------------------------------------
static const u8 namein_poke_win_msg[] = 
{
	I_MSG_, STR_TEMP_BUF0, ha_, spc_, PA_, SO_, KO_, N_, he_, spc_,
	te_, n_, so_, u_, sa_, re_, ta_, gyoe_, CTRL_, C_TRGWAIT_, EOM_
};

//------------------------------------------
//	�߹�ݓ]������޳���
//------------------------------------------
static void SetNamePokeWindow( void )
{
	PM_strcpy( StrTempBuffer0, Namein->set_name_buf );
	PM_MsgExpand( MsgExpandBuffer, namein_poke_win_msg );
	NMenuContextChange( NAMEIN_BG0_TYPE );
	NMenuTalkWinWrite();
	NMenuTalkMsgSetEasy( (const u8 *)MsgExpandBuffer );
}

//======================================================================
//		OBJ
//======================================================================
//------------------------------------------
//	OBJ VRAM ���
//------------------------------------------
static void SetObjVram( void )
{
	CellSets( w_name_CellData );
	ObjPalSets( w_name_PaletteData );
}

//======================================================================
//		BG
//======================================================================
static void SetBgChar( void );
static void SetBgPalette( void );
static void SetMapHiraganaBoard( u16 * );
static void SetMapKatakanaBoard( u16 * );
static void SetMapAbcBoard( u16 * );
static void SetMap240x160( u16 *, u16 * );
static void SetMapBoardSpace( u8, u8 );

//------------------------------------------
//	BG VRAM ���
//------------------------------------------
static void SetBgVram( void )
{
	SetBgChar();
	SetBgPalette();
}

//------------------------------------------
//	BG ��׾��
//------------------------------------------
static void SetBgChar( void )
{
	DIV_DMACOPY( 3, w_name_Character, BG_CH_TRANS_BASE, BG_CHDATA_MAX, 16 );
}

//------------------------------------------
//	BG ��گľ��
//------------------------------------------
static void SetBgPalette( void )
{
	PaletteWorkSet( w_name_Palette, PA_BG0, 0x20*4 );
}

//------------------------------------------
//	�����Օ`�揈��ð���
//------------------------------------------
static void (* const SetMapBoardTbl[3][2])(u16 *) =
{
	{ SetMapKatakanaBoard, SetMapHiraganaBoard },
	{ SetMapHiraganaBoard, SetMapAbcBoard },
	{ SetMapAbcBoard, SetMapKatakanaBoard }
};

//------------------------------------------
//	�����Օ\���`��
//------------------------------------------
static void SetMapBoardFaceBack( void )
{
	u16 *map[2] = { (u16*)BG1_MAP_BASE, (u16*)BG2_MAP_BASE };
	
	SetMapBoardTbl[Namein->board_mode][0]( map[Namein->bg_board_face] );
	SetMapBoardTbl[Namein->board_mode][1]( map[Namein->bg_board_back] );
}

//------------------------------------------
//	�����՗��`��
//------------------------------------------
static void SetMapBoardBack( void )
{
	u16 *map[2] = { (u16*)BG1_MAP_BASE, (u16*)BG2_MAP_BASE };

	SetMapBoardTbl[Namein->board_mode][1]( map[Namein->bg_board_back] );
}

//------------------------------------------
//	�Ђ炪�Ȕ`��
//------------------------------------------
static void SetMapHiraganaBoard( u16 *map )
{
	SetMap240x160( map, (u16*)w_name2_Map );
}

//------------------------------------------
//	���Ŕ`��
//------------------------------------------
static void SetMapKatakanaBoard( u16 *map )
{
	SetMap240x160( map, (u16*)w_name3_Map );
}

//------------------------------------------
//	��̧�ޯĔ`��
//------------------------------------------
static void SetMapAbcBoard( u16 *map )
{
	SetMap240x160( map, (u16*)w_name4_Map );
}

//------------------------------------------
//	�w�i�`��
//------------------------------------------
static void SetMapBack( void )
{
	SetMap240x160( (u16*)BG3_MAP_BASE, (u16*)w_name1_Map );
}

//------------------------------------------
//	240x160ϯ�ߕ`��
//------------------------------------------
static void SetMap240x160( u16 *map, u16 *data )
{
	s16 x,y;
	
	for( y = 0; y < 0x14; y++ )
	{
		for( x = 0; x < 0x1e; BG_VRAMSET(map,x,y,(*data)+BG_CHNO_0), x++, data++ );
	}
}

//------------------------------------------
//	��߰�̫�ľ�� �\
//------------------------------------------
static void SetMapBoardFaceSpace( void )
{
	SetMapBoardSpace( Namein->board_mode, Namein->bg_board_face );
}

//------------------------------------------
//	��߰�̫�ľ�� ��
//------------------------------------------
static void SetMapBoardBackSpace( void )
{
	SetMapBoardSpace( (Namein->board_mode+1) % MODE_MAX, Namein->bg_board_back );
}

//------------------------------------------
//	��߰�̫�ľ��
//------------------------------------------
static void SetMapBoardSpace( u8 mode, u8 faceback )
{
	u16 *map[2] = { (u16*)BG1_MAP_BASE, (u16*)BG2_MAP_BASE };
	u16 pal[2] = { BG_BOARD_KATAKANA_PALNO, BG_BOARD_HIRAGANA_PALNO };

	if( mode != MODE_ABC )
	{
		BG_VRAMSET( map[faceback], 0x14, 0x09,
			BG_SCRN_DATA(BG_FONT_SPACE_NO_0+BG_CHNO_0,0,0,pal[mode]) );
		BG_VRAMSET( map[faceback], 0x14, 0x0a, 
			BG_SCRN_DATA(BG_FONT_SPACE_NO_1+BG_CHNO_0,0,0,pal[mode]) );
	}
}

//======================================================================
//		����
//======================================================================
static void SetMsgFontBoard( u8 );
static void SetFontBoardHiragana( void );
static void SetFontBoardKatakana( void );
static void SetFontBoardAbc( void );
static void SetFontInfoNothing( void );
static void SetFontInfoPoke( void );
static void SetFontSexNothing( void );
static void SetFontSexPoke( void );

//------------------------------------------
//	�������ގ擾
//------------------------------------------
static u8 GetFontBoardCode( s16 x, s16 y )
{
	return( FontBoardCode[Namein->board_mode][y][x] );
}

//------------------------------------------
//	�����Տ���ð���
//------------------------------------------
static void (* const SetFontBoardTbl[3][2])( void ) =
{
	{ SetFontBoardKatakana, SetFontBoardHiragana },
	{ SetFontBoardHiragana, SetFontBoardAbc },
	{ SetFontBoardAbc, SetFontBoardKatakana }
};

//------------------------------------------
//	������÷��ð���
//------------------------------------------
static const u16 SetFontBoardTextTbl[3][2][2] =
{
	{
		{ NAMEIN_KANA_BG1_TYPE, NAMEIN_KANA_BG2_TYPE },
		{ NAMEIN_HIRA_BG1_TYPE, NAMEIN_HIRA_BG2_TYPE }
	},
	{
		{ NAMEIN_HIRA_BG1_TYPE, NAMEIN_HIRA_BG2_TYPE },
		{ NAMEIN_ABC_BG1_TYPE, NAMEIN_ABC_BG2_TYPE }
	},
	{
		{ NAMEIN_ABC_BG1_TYPE, NAMEIN_ABC_BG2_TYPE },
		{ NAMEIN_KANA_BG1_TYPE, NAMEIN_KANA_BG2_TYPE }
	}
};

//------------------------------------------
//	������ �\�����
//------------------------------------------
static void SetFontBoardFaceBack( void )
{
	NMenuContextChange( SetFontBoardTextTbl[Namein->board_mode][0][Namein->bg_board_face] );
	SetFontBoardTbl[Namein->board_mode][0]();
	NMenuContextChange( SetFontBoardTextTbl[Namein->board_mode][1][Namein->bg_board_back] );
	SetFontBoardTbl[Namein->board_mode][1]();
	
	SetMapBoardFaceSpace();
	SetMapBoardBackSpace();
}

//------------------------------------------
//	������ �����
//------------------------------------------
static void SetFontBoardBack( void )
{
	NMenuContextChange( SetFontBoardTextTbl[Namein->board_mode][1][Namein->bg_board_back] );
	SetFontBoardTbl[Namein->board_mode][1]();
	
	SetMapBoardBackSpace();
}

//------------------------------------------
//	�Ђ炪�ȕ����Օ`��
//------------------------------------------
static void SetFontBoardHiragana( void )
{
	SetMsgFontBoard( MODE_HIRAGANA );
}

//------------------------------------------
//	���ŕ����Օ`��
//------------------------------------------
static void SetFontBoardKatakana( void )
{
	SetMsgFontBoard( MODE_KATAKANA );
}

//------------------------------------------
//	��̧�ޯĕ����Օ`��
//------------------------------------------
static void SetFontBoardAbc( void )
{
	SetMsgFontBoard( MODE_ABC );
}

//------------------------------------------
//	�����`��
//------------------------------------------
static void SetMsgFontBoard( u8 mode )
{
	s16 y,ypos;
	
	for( y = 0, ypos = 0x09; y < 4; y++, ypos += 2 )
	{
		NMenuMsgWrite( FontBoardCode[mode][y], 0x03, ypos );
	}
}

//------------------------------------------
//	�ē���������ð���
//------------------------------------------
static void (* const SetFontInfoTbl[])( void ) =
{
	SetFontInfoNothing,									// NAMEIN_HERO
	SetFontInfoNothing,									// NAMEIN_BOX
	SetFontInfoPoke,									// NAMEIN_POKE
	SetFontInfoPoke,									// NAMEIN_POKE_NICK
};

//------------------------------------------
//	���ʕ\��ð���
//------------------------------------------
static void (* const SetFontSexTbl[])( void ) =
{
	SetFontSexNothing,									// SEX_FONT_OFF
	SetFontSexPoke,										// SEX_FONT_ON
};

//------------------------------------------
//	�ē�����,���ʕ\��
//------------------------------------------
static void SetFontInfoSex( void )
{
	NMenuContextChange( NAMEIN_BG3_TYPE );
	SetFontInfoTbl[Namein->name_type]();
	SetFontSexTbl[Namein->namedata->sex_font_sw]();
	NMenuMsgWrite( Namein->namedata->info_font, 0x09, 0x02 );
}

//------------------------------------------
//	�ē��������� ���ɖ���
//------------------------------------------
static void SetFontInfoNothing( void )
{
}

//------------------------------------------
//	�ē��������� �߹��
//------------------------------------------
static void SetFontInfoPoke( void )
{
	PM_strcpy( StrTempBuffer0, PokeNameData[Namein->arg_work0] );
}

//------------------------------------------
//	���ʕ\�� ����
//------------------------------------------
static void SetFontSexNothing( void )
{
}

//------------------------------------------
//	���ʕ\�� �߹��
//------------------------------------------
static void SetFontSexPoke( void )
{
	u8 sex[2] = { osu_, EOM_ };
	
	if( Namein->arg_work1 != MONS_UNKNOWN )
	{
		if( Namein->arg_work1 == MONS_FEMALE )
		{
			sex[0] = mesu_;
		}
		
		NMenuMsgWrite( sex, 0x12, 0x04 );
	}
}

//------------------------------------------
//	���O�\��
//------------------------------------------
static void SetFontName( void )
{
	NMenuContextChange( NAMEIN_BG3_TYPE );
	NMenuMsgWrite( Namein->name, Namein->namefont_start_x, 0x04 );
}

//======================================================================
//		�����ް�
//======================================================================
//------------------------------------------
//	�ē�����
//------------------------------------------
static const u8 NameInfoFontHero[] =					// ��l��
{a_,na_,ta_,spc_,no_,spc_,na_,ma_,e_,ha_,hate_,EOM_};

static const u8 NameInfoFontBox[] =						// �ޯ��
{BO_,TTU_,KU_,SU_,spc_,no_,spc_,na_,ma_,e_,ha_,hate_,EOM_};

static const u8 NameInfoFontPoke[] =					// �߹��
{I_MSG_,STR_TEMP_BUF0,spc_,no_,spc_,NI_,TTU_,KU_,NE_,bou_,MU_,ha_,hate_,EOM_};

//------------------------------------------
//	��l��
//------------------------------------------
static const NAMEDATA NameDataHero =
{
	DEF_NAME_OFF,
	5,
	NAME_OBJ_HERO,
	SEX_FONT_OFF,
	MODE_KATAKANA,
	NameInfoFontHero
};

//------------------------------------------
//	�ޯ��
//------------------------------------------
static const NAMEDATA NameDataBox =
{
	DEF_NAME_OFF,
	8,
	NAME_OBJ_BOX,
	SEX_FONT_OFF,
	MODE_KATAKANA,
	NameInfoFontBox
};

//------------------------------------------
//	�߹��
//------------------------------------------
static const NAMEDATA NameDataPoke =
{
	DEF_NAME_OFF,
	5,
	NAME_OBJ_POKE,
	SEX_FONT_ON,
	MODE_KATAKANA,
	NameInfoFontPoke
};

//------------------------------------------
//	���͈ꗗ
//------------------------------------------
static const NAMEDATA * const NameData[] =
{
	&NameDataHero,										// NAMEIN_HERO
	&NameDataBox,										// NAMEIN_BOX
	&NameDataPoke,										// NAMEIN_POKE
	&NameDataPoke,										// NAMEIN_POKE_NICK
};

//======================================================================
//		������
//======================================================================
//------------------------------------------
//	������
//------------------------------------------
static const u8 FontBoardCode[3][4][20] =
{
 {
  {A_,I_,U_,E_,O_,spc_,NA_,NI_,NU_,NE_,NO_,spc_,YA_,YU_,YO_,gyoe_,hate_,spc_,spc_,EOM_},
  {KA_,KI_,KU_,KE_,KO_,spc_,HA_,HI_,HU_,HE_,HO_,spc_,WA_,WO_,N_,spc_,DAKU_,HANDAKU_,spc_,EOM_},
  {SA_,SI_,SU_,SE_,SO_,spc_,MA_,MI_,MU_,ME_,MO_,spc_,YYA_,YYU_,YYO_,TTU_,bou_,spc_,spc_,EOM_},
  {TA_,TI_,TU_,TE_,TO_,spc_,RA_,RI_,RU_,RE_,RO_,spc_,AA_,II_,UU_,EE_,OO_,spc_,spc_,EOM_}
 },
 {
  {a_,i_,u_,e_,o_,spc_,na_,ni_,nu_,ne_,no_,spc_,ya_,yu_,yo_,gyoe_,hate_,spc_,spc_,EOM_},
  {ka_,ki_,ku_,ke_,ko_,spc_,ha_,hi_,hu_,he_,ho_,spc_,wa_,wo_,n_,spc_,DAKU_,HANDAKU_,spc_,EOM_},
  {sa_,si_,su_,se_,so_,spc_,ma_,mi_,mu_,me_,mo_,spc_,yya_,yyu_,yyo_,ttu_,bou_,spc_,spc_,EOM_},
  {ta_,ti_,tu_,te_,to_,spc_,ra_,ri_,ru_,re_,ro_,spc_,aa_,ii_,uu_,ee_,oo_,spc_,spc_,EOM_}
 },
 {
  {A__,B__,C__,D__,E__,F__,G__,H__,I__,J__,K__,L__,M__,N__,O__,P__,Q__,R__,S__,EOM_},
  {T__,U__,V__,W__,X__,Y__,Z__,spc_,n0_,n1_,n2_,n3_,n4_,n5_,n6_,n7_,n8_,n9_,spc_,EOM_},
  {a__,b__,c__,d__,e__,f__,g__,h__,i__,j__,k__,l__,m__,n__,o__,p__,q__,r__,s__,EOM_},
  {t__,u__,v__,w__,x__,y__,z__,spc_,kten_,nakag_,tenten_,kako2_,kakot2_,kako_,kakot_,sura_,osu_,mesu_,spc_,EOM_}
 }
};

//======================================================================
//		����
//======================================================================
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

static const ActOamData oam16x16 =
{
	0,							// u32 VPos:8
	0,							// u32 AffineMode:2
	0,							// u32 ObjMode:2
	0,							// u32 Mosaic:1
	0,							// u32 ColorMode:1
	0,							// u32 Shape:2
	0,							// u32 HPos:9
	0,							// u32 AffineParamNo:5
	1,							// u32 Size:2
	0,							// u32 CharNo:10
	0,							// u32 Priority:2
	0,							// u32 Pltt:3
	0,							// AffineWork
};

static const ActOamData oam32x16 =
{
	0,							// u32 VPos:8
	0,							// u32 AffineMode:2
	0,							// u32 ObjMode:2
	0,							// u32 Mosaic:1
	0,							// u32 ColorMode:1
	1,							// u32 Shape:2
	0,							// u32 HPos:9
	0,							// u32 AffineParamNo:5
	2,							// u32 Size:2
	0,							// u32 CharNo:10
	0,							// u32 Priority:2
	0,							// u32 Pltt:3
	0,							// AffineWork
};

//------------------------------------------
//	OAM�߯�
//------------------------------------------
static const OAMPatData SelectOamPat0[] =
{ 
	{ -20, -16, 1, 1, 0x00, 1 },
	{  12, -16, 0, 0, 0x04, 1 },
	{ -20,  -8, 1, 1, 0x05, 1 },
	{  12,  -8, 0, 0, 0x09, 1 },
	{ -20,   0, 1, 1, 0x0a, 1 },
	{  12,   0, 0, 0, 0x0e, 1 },
	{ -20,   8, 1, 1, 0x0f, 1 },
	{  12,   8, 0, 0, 0x13, 1 }
};

static const OAMPatData SelectFontOamPat0[] =
{
	{ -12, -4, 0, 0, 0x00, 1 },
	{   4, -4, 0, 0, 0x01, 1 }
};

static const OAMPatData SelectFontOamPat1[] =
{
	{ -12, -4, 1, 0, 0x00, 1 },
	{   4, -4, 0, 0, 0x02, 1 }
};

static const OAMPatData RetEndOamPat0[] =
{
	{ -20, -12, 1, 1, 0x00, 1 },
	{  12, -12, 0, 0, 0x04, 1 },
	{ -20,  -4, 1, 1, 0x05, 1 },
	{  12,  -4, 0, 0, 0x09, 1 },
	{ -20,   4, 1, 1, 0x0a, 1 },
	{  12,   4, 0, 0, 0x0e, 1 },
};

static const OAMPatData IconBoxOamPat0[] =
{
	{ -8, -12, 1, 0, 0x00, 3 },
	{ -8, -4, 1, 0, 0x02, 3 },
	{ -8, 4, 1, 0, 0x04, 3 },
};

static const OAMPatSt SelectOamPat[] =
{
	{ 8, SelectOamPat0 },
};

static const OAMPatSt SelectFontOamPat[] =
{
	{ 2, SelectFontOamPat0 },							// ��	(����)
	{ 2, SelectFontOamPat1 },							// ABC	(��)
	{ 2, SelectFontOamPat0 },							// ����	(ABC)
};

static const OAMPatSt RetEndOamPat[] =
{
	{ 6, RetEndOamPat0 },
};

static const OAMPatSt IconBoxOamPat[] =
{
	{ 3, IconBoxOamPat0 },
};

//------------------------------------------
//	�ٓ]���ް�
//------------------------------------------
static const actAnmData IconBoxCellTrans[] =
{
	{ &com_i_Character[(0x20*6)*0], 0x20*6 },
	{ &com_i_Character[(0x20*6)*1], 0x20*6 },
};

//------------------------------------------
//	����ް�
//------------------------------------------
static const actAnm NameInNonAnm[] =
{
	{ 0, 1, 0, 0 },
	{ ACT_ANMLOOP, 0, 0, 0 },
};

static const actAnm BoardCursorAnm1[] =
{
	{ 0x04, 8, 0, 0 },
	{ 0x08, 8, 0, 0 },
	{ ACT_ANMEND, 0, 0, },
};

static const actAnm IconBoxAnm0[] =
{
	{ 0, 2, 0, 0 },
	{ 1, 2, 0, 0 },
	{ ACT_ANMLOOP, 0, 0, 0 },
};

static const actAnm * const NameInNonAnmTbl[] =
{
	NameInNonAnm,
};

static const actAnm * const BoardCursorAnmTbl[] =
{
	NameInNonAnm,
	BoardCursorAnm1,
};

static const actAnm * const IconBoxAnmTbl[] =
{
	IconBoxAnm0,
};

//------------------------------------------
//	����ͯ��
//------------------------------------------
static const actHeader SelectButtonActHead =
{
	NAME_CELL_SEL,
	PAL_OBJ_4,
	&oam8x8,
	NameInNonAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	SelectButtonActMove,
};

static const actHeader SelectBackActHead =
{
	NAME_CELL_SEL_BACK,
	PAL_OBJ_1,
	&oam32x16,
	NameInNonAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	DummyActMove,
};

static const actHeader SelectFontActHead =
{
	NAME_CELL_HIRAGANA,
	PAL_OBJ_4,
	&oam8x8,
	NameInNonAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	DummyActMove,
};

static const actHeader RetButtonActHead =
{
	NAME_CELL_RET,
	PAL_OBJ_6,
	&oam8x8,
	NameInNonAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	DummyActMove,
};

static const actHeader EndButtonActHead =
{
	NAME_CELL_END,
	PAL_OBJ_6,
	&oam8x8,
	NameInNonAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	DummyActMove,
};

static const actHeader BoardCursorActHead =
{
	NAME_CELL_BOARD_CURSOR0,
	PAL_OBJ_5,
	&oam16x16,
	BoardCursorAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	BoardCursorActMove,
};

static const actHeader NameCursorActHead =
{
	NAME_CELL_FONT_CURSOR,
	PAL_OBJ_3,
	&oam8x8,
	NameInNonAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	NameCursorActMove,
};

static const actHeader NameBarActHead =
{
	NAME_CELL_FONT_BAR,
	PAL_OBJ_3,
	&oam8x8,
	NameInNonAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	NameBarActMove,
};

static const actHeader IconBoxActHead =
{
	ACT_TMODE,
	PAL_OBJ_0,
	&oam8x8,
	IconBoxAnmTbl,
	IconBoxCellTrans,
	DummyActAffTbl,
	DummyActMove,
};

//======================================================================
//		��
//======================================================================
//------------------------------------------
//	���ް�
//------------------------------------------
static const CellData w_name_CellData[] =
{
	{ &w_name2_Character[0x20*0x00], 0x20*0x0f, NAME_CELL_RET },
	{ &w_name2_Character[0x20*0x10], 0x20*0x0f, NAME_CELL_END },
	{ &w_name2_Character[0x20*0x20], 0x20*0x14, NAME_CELL_SEL },
	{ &w_name2_Character[0x20*0x34], 0x20*0x08,	NAME_CELL_SEL_BACK },
	{ &w_name2_Character[0x20*0x3c], 0x20*0x02,	NAME_CELL_HIRAGANA },
	{ &w_name2_Character[0x20*0x3e], 0x20*0x02,	NAME_CELL_KATAKANA },
	{ &w_name2_Character[0x20*0x40], 0x20*0x03,	NAME_CELL_ABC },
	{ &w_name2_Character[0x20*0x43], 0x20*0x04,	NAME_CELL_BOARD_CURSOR0 },
	{ &w_name2_Character[0x20*0x47], 0x20*0x04,	NAME_CELL_BOARD_CURSOR1 },
	{ &w_name2_Character[0x20*0x4b], 0x20*0x04,	NAME_CELL_BOARD_CURSOR2 },
	{ &w_name2_Character[0x20*0x0f], 0x20*0x01,	NAME_CELL_FONT_CURSOR },
	{ &w_name2_Character[0x20*0x1f], 0x20*0x01,	NAME_CELL_FONT_BAR },
	{ CELL_END, 0, 0 }
};

//======================================================================
//		��گ�
//======================================================================
//------------------------------------------
//	��گ��ް�
//------------------------------------------
static const u16 w_name_Palette[96]={
#if 0
	0x0000,0x1ce7,0x2529,0x318c,0x39ce,0x4631,0x18ca,0x14ad,
	0x5f5a,0x0c93,0x0c96,0x10b0,0x7bbe,0x3e54,0x631a,0x7fff,
	
	0x0000,0x7fff,0x1ce7,0x39ce,0x6253,0x6eb6,0x0000,0x7f5c,
	0x4e33,0x0000,0x0000,0x0000,0x0000,0x0000,0x318c,0x76d9,
	
	0x0000,0x7fff,0x1ce7,0x39ce,0x4ead,0x52ee,0x0000,0x6794,
	0x3a90,0x0000,0x0000,0x0000,0x0000,0x0000,0x318c,0x5b52,
	
	0x0000,0x7fff,0x1ce7,0x39ce,0x321a,0x3e7c,0x0000,0x5b3e,
	0x29f5,0x0000,0x0000,0x0000,0x0000,0x0000,0x318c,0x4adf,
	
	0x0000,0x7fbd,0x1ce7,0x39ce,0x6253,0x76d9,0x4a8d,0x5b52,
	0x321a,0x4adf,0x16d9,0x37be,0x318c,0x733b,0x318c,0x76f9,
	
	0x0000,0x043f,0x24fb,0x2916,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x76f9,
#else
	0x0000,0x310a,0x394c,0x318c,0x39ce,0x4631,0x28ec,0x20ce,
	0x5f5a,0x1093,0x0c96,0x18b1,0x7bbe,0x3e54,0x631a,0x7fff,

	0x0000,0x7fff,0x1ce7,0x39ce,0x6253,0x6eb6,0x0000,0x7f5c,
	0x4e33,0x0000,0x0000,0x0000,0x0000,0x0000,0x318c,0x76d9,

	0x0000,0x7fff,0x1ce7,0x39ce,0x4ead,0x52ee,0x0000,0x6794,
	0x3a90,0x0000,0x0000,0x0000,0x0000,0x0000,0x318c,0x5b52,

	0x0000,0x7fff,0x1ce7,0x39ce,0x321a,0x3e7c,0x0000,0x5b3e,
	0x29f5,0x0000,0x0000,0x0000,0x0000,0x0000,0x318c,0x4adf,

	0x0000,0x7fbd,0x1ce7,0x39ce,0x6253,0x76d9,0x4a8d,0x5b52,
	0x321a,0x4adf,0x16d9,0x37be,0x318c,0x733b,0x318c,0x76f9,

	0x0000,0x043f,0x24fb,0x2916,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x76f9,
#endif
};

//------------------------------------------
//	OBJ ��گ� ��گ��Ȱ�ެ
//------------------------------------------
static const PalData w_name_PaletteData[] =
{
	{ &w_name_Palette[0x10*0x00], PAL_OBJ_0 },
	{ &w_name_Palette[0x10*0x01], PAL_OBJ_1 },
	{ &w_name_Palette[0x10*0x02], PAL_OBJ_2 },
	{ &w_name_Palette[0x10*0x03], PAL_OBJ_3 },
	{ &w_name_Palette[0x10*0x04], PAL_OBJ_4 },
	{ &w_name_Palette[0x10*0x05], PAL_OBJ_5 },
	{ &w_name_Palette[0x10*0x04], PAL_OBJ_6 },
	{ OBJPAL_END, 0 }
};

//======================================================================
//		ϯ���ް�
//======================================================================
//------------------------------------------
//	�w�iϯ���ް� 30x20
//------------------------------------------
static const u16 w_name1_Map[600]={
	0x0008,0x0009,0x000a,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 0 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x040a,0x0409,0x0408,
	0x000b,0x000c,0x0001,0x0001,0x0002,0x0003,0x0003,0x0003,    /* 1 */
	0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,
	0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,
	0x0003,0x0402,0x0001,0x0401,0x040c,0x040b,
	0x000d,0x000e,0x0001,0x0001,0x0004,0x0007,0x0007,0x0007,    /* 2 */
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0404,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0004,0x0007,0x0007,0x0007,    /* 3 */
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0404,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0004,0x0007,0x0007,0x0007,    /* 4 */
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0404,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0004,0x0007,0x0007,0x0007,    /* 5 */
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0404,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0005,0x0006,0x0006,0x0006,    /* 6 */
	0x0006,0x0006,0x0006,0x0006,0x0006,0x0006,0x0006,0x0006,
	0x0006,0x0006,0x0006,0x0006,0x0006,0x0006,0x0006,0x0006,
	0x0006,0x0405,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 7 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 8 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 9 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 10 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 11 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 12 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 13 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 14 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 15 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 16 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x000d,0x000e,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 17 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0401,0x040e,0x040d,
	0x0010,0x0011,0x0001,0x0001,0x0015,0x0016,0x0017,0x0018,    /* 18 */
	0x0019,0x001a,0x001b,0x001c,0x001d,0x001e,0x0001,0x0001,
	0x0001,0x0001,0x0020,0x0021,0x0022,0x0023,0x0001,0x0024,
	0x0001,0x0025,0x0001,0x0401,0x0411,0x0410,
	0x0012,0x0013,0x0014,0x0001,0x0001,0x0001,0x0001,0x0001,    /* 19 */
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,
	0x0001,0x0001,0x0001,0x0414,0x0413,0x0412,
};

//------------------------------------------
//	�Ђ炪��ϯ���ް� 30x20
//------------------------------------------
static const u16 w_name2_Map[600]={
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 4 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 5 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 6 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 7 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1030,0x1031,0x1031,0x1031,0x1031,0x1031,    /* 8 */
	0x1031,0x1031,0x1031,0x1031,0x1031,0x1031,0x1031,0x1031,
	0x1031,0x1031,0x1031,0x1031,0x1031,0x1031,0x1032,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1033,0x1034,0x1034,0x1034,0x1034,0x1034,    /* 9 */
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1033,0x1034,0x1034,0x1034,0x1034,0x1034,    /* 10 */
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1033,0x1034,0x1034,0x1034,0x1034,0x1034,    /* 11 */
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,
	0x1034,0x1034,0x1034,0x1039,0x103a,0x1034,0x1035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1033,0x1034,0x1034,0x1034,0x1034,0x1034,    /* 12 */
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1033,0x1034,0x1034,0x1034,0x1034,0x1034,    /* 13 */
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1033,0x1034,0x1034,0x1034,0x1034,0x1034,    /* 14 */
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1033,0x1034,0x1034,0x1034,0x1034,0x1034,    /* 15 */
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,
	0x1034,0x1034,0x1034,0x1034,0x103b,0x1034,0x1035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1033,0x1034,0x1034,0x1034,0x1034,0x1034,    /* 16 */
	0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,0x1034,
	0x1034,0x1034,0x1034,0x1034,0x103c,0x1034,0x1035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x1036,0x1037,0x1037,0x1037,0x1037,0x1037,    /* 17 */
	0x1037,0x1037,0x1037,0x1037,0x1037,0x1037,0x1037,0x1037,
	0x1037,0x1037,0x1037,0x1037,0x1037,0x1037,0x1038,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 18 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 19 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

//------------------------------------------
//	����ϯ���ް� 30x20
//------------------------------------------
static const u16 w_name3_Map[600]={
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 4 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 5 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 6 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 7 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2030,0x2031,0x2031,0x2031,0x2031,0x2031,    /* 8 */
	0x2031,0x2031,0x2031,0x2031,0x2031,0x2031,0x2031,0x2031,
	0x2031,0x2031,0x2031,0x2031,0x2031,0x2031,0x2032,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2033,0x2034,0x2034,0x2034,0x2034,0x2034,    /* 9 */
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2033,0x2034,0x2034,0x2034,0x2034,0x2034,    /* 10 */
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2033,0x2034,0x2034,0x2034,0x2034,0x2034,    /* 11 */
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,
	0x2034,0x2034,0x2034,0x2039,0x203a,0x2034,0x2035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2033,0x2034,0x2034,0x2034,0x2034,0x2034,    /* 12 */
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2033,0x2034,0x2034,0x2034,0x2034,0x2034,    /* 13 */
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2033,0x2034,0x2034,0x2034,0x2034,0x2034,    /* 14 */
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2033,0x2034,0x2034,0x2034,0x2034,0x2034,    /* 15 */
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,
	0x2034,0x2034,0x2034,0x2034,0x203b,0x2034,0x2035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2033,0x2034,0x2034,0x2034,0x2034,0x2034,    /* 16 */
	0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,0x2034,
	0x2034,0x2034,0x2034,0x2034,0x203c,0x2034,0x2035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x2036,0x2037,0x2037,0x2037,0x2037,0x2037,    /* 17 */
	0x2037,0x2037,0x2037,0x2037,0x2037,0x2037,0x2037,0x2037,
	0x2037,0x2037,0x2037,0x2037,0x2037,0x2037,0x2038,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 18 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 19 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

//------------------------------------------
//	��̧�ޯ�ϯ���ް� 30x20
//------------------------------------------
static const u16 w_name4_Map[600]={
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 4 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 5 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 6 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 7 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3030,0x3031,0x3031,0x3031,0x3031,0x3031,    /* 8 */
	0x3031,0x3031,0x3031,0x3031,0x3031,0x3031,0x3031,0x3031,
	0x3031,0x3031,0x3031,0x3031,0x3031,0x3031,0x3032,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3033,0x3034,0x3034,0x3034,0x3034,0x3034,    /* 9 */
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3033,0x3034,0x3034,0x3034,0x3034,0x3034,    /* 10 */
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3033,0x3034,0x3034,0x3034,0x3034,0x3034,    /* 11 */
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3033,0x3034,0x3034,0x3034,0x3034,0x3034,    /* 12 */
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3033,0x3034,0x3034,0x3034,0x3034,0x3034,    /* 13 */
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3033,0x3034,0x3034,0x3034,0x3034,0x3034,    /* 14 */
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3033,0x3034,0x3034,0x3034,0x3034,0x3034,    /* 15 */
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3033,0x3034,0x3034,0x3034,0x3034,0x3034,    /* 16 */
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,
	0x3034,0x3034,0x3034,0x3034,0x3034,0x3034,0x3035,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x3036,0x3037,0x3037,0x3037,0x3037,0x3037,    /* 17 */
	0x3037,0x3037,0x3037,0x3037,0x3037,0x3037,0x3037,0x3037,
	0x3037,0x3037,0x3037,0x3037,0x3037,0x3037,0x3038,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 18 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0182,0x0183,0x0000,0x0000,0x0000,0x0000,0x00e5,
	0x00e6,0x00e7,0x00e8,0x00e9,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,    /* 19 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

