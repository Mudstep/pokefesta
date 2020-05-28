//=========================================================================
//
//	pm_print.c
//
//	�|�P�����`�f�a�p���b�Z�[�W�\���V�X�e��
//
//=========================================================================


#include "common.h"
#include "palanm.h"
#include "screen.h"
#include "fight.h"
#include "mus_tool.h"
#include "contest.h"

#define PRINT_H_GLOBAL
#include "print.h"

#include "fcontext.h"

#include "fnt_tenji.h"

//-------------------------------------------------------------------------
//							��`
//-------------------------------------------------------------------------

/* 8x16�p�V���{����8x8�p�V���{���ϊ��p�̍\���� */
typedef struct {
	u8 chr1st;
	u8 chr2nd;
}Conv8x8;


//	�����\�����C���J�ڏ�Ԓ�`
enum{
		MSG_END,			/* �\���I��(�������) */
		MSG_START,			/* �\���J�n */
		MSG_WRITE,			/* MSG�\�� */
		MSG_WAIT,			/* �E�F�C�g */
		MSG_TIMEWAIT,		/* ��莞�ԃE�F�C�g */
		MSG_KEYWAIT,		/* �L�[���͑҂� */
		MSG_CR,				/* ���s */
		MSG_INDIRECT,		/* �Ԑڕ�����Q�� */

		MSG_NWAIT,			/* �L�[�҂���MSG�E�B���h�E�N���A�ŕ\���ĊJ */
		MSG_SWAIT,			/* �L�[�҂�����s�X�N���[���ŕ\���ĊJ */
		MSG_SEWAIT,			/* SE�̖�I����҂� */
};

//StringPrint ����̖߂�l��`
enum{
	STRPRT_END = 0,		//�I��
	STRPRT_PUT,			//�\��
	STRPRT_CTRL,		//����
};

//����MSG�̐��䃂�[�h�imsg_skip_mode)
enum{
	MSGSKIP_NORMAL = 0,		//�ʏ퓮��
	MSGSKIP_BATTLE,			//�ʐM���̓L�[���Ƃ�Ȃ�
	MSGSKIP_FORCE,			//�L�[����֎~
	MSGSKIP_CONTEST,		//�R���e�X�g���A�ʐM���̓L�[�����Ȃ�
};

#define LFONT_CHR_USE	2						//�ʏ�t�H���g����߂�L������
#define FDSIZE			(0x08)					//̫���ް�����(�W�J�O)
#define FSIZE			(CHRSIZE*LFONT_CHR_USE)	//̫���ް�����(�W�J��)

/* �E�B���h�E�l�r�f�֘A��` */
#define	WIN_SPC_SZ		0						//�E�B���h�E�Ŏg�p����󔒃L����
#define WIN_CSR_CHR_POS	1						//�E�B���h�E�Ŏg�p���鑗��L����
#define	WIN_SYS_FONT_SIZE	2				//�E�B���h�E�Ŏg�p����V�X�e���L�����T�C�Y
#define	WIN_LINE1_CHRSTART		( WIN_SYS_FONT_SIZE * LFONT_CHR_USE )
#define	WIN_LINE2_CHRSTART(n)	( WIN_LINE1_CHRSTART + n * LFONT_CHR_USE )

#define Y_CGX_FLAG		(0x0002)	//���s��(CGX���ڽ�w��)�׸�

#define	FIXSCRNFONTUSE	2

#define AB_BUTTON	(A_BUTTON | B_BUTTON )
#define COMM_BATTLE_WAIT	(60*1)


#define FIX_SKIPCURSOR	0xfe		/* �Œ�t�H���g���[�h���̑���J�[�\���ʒu */
									/* ���̐����̂��߃t�H���g�͍ő�ł�0xfd��ނ܂� */




//-------------------------------------------------------------------------
//
//						�f�[�^
//
//-------------------------------------------------------------------------

//-----------------------------------------------------------------
//-----------------------------------------------------------------
/* ����������p�@TALK_WIN�\���̒�` */
static const TALK_WIN TalkWinInitializer =  {
	NULL,				//CONST_FONT_CONTEXT_P  context;
	MSG_END,			//u16		status;		// �ð��
	0,					//u16		work;		//
	0,					//u8		sw;			// �����׸�
	0,					//u8		wait;		// ����

	0,					//u8		mode;		// �����\�����[�h
	0,					//u8		font;		// �g�p�t�H���g
	0,					//u8		col_f;  	// �F�ԍ�(����)
	0,					//u8		col_b;		// �F�ԍ�(�w�i)
	0,					//u8		col_s;		// �F�ԍ�(�e)
	0,					//u8		palette;	// �p���b�g

	NULL,				//const u8 *text;		// ÷�� �ް����ڽ
	0,					//u16		text_p;		// ÷�� �̾��

	0,					//u16		cgx_n;		// CGX �ʒu
	0,					//u16		cgx_np;		// CGX Offset

	0,					//u8		scr_x;		// ��ذ�X �ʒu
	0,					//u8		scr_xp;		// ��ذ�X Offset
	0,					//u8		scr_y;		// ��ذ�Y �ʒu
	0,					//u8		scr_yp;		// ��ذ�Y Offset

	NULL,				//void *	cgx_adrs;
};

/* 8x16�p�V���{����8x8�p�V���{���ϊ��e�[�u�� */
#include "conv8x8.dat"

#include "..\data\tenji_tbl.dat"

//-----------------------------------------------------------------
//			�\���p�f�[�^
//-----------------------------------------------------------------
/* �󔒃L�����p�f�[�^��` */
static const u8 PM_Print_Blank[8] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/* �����\���p�p���b�g�f�[�^ */

static const u16 PM_Print_Palette[16] = {
0x0000,		//�����F
0x2529,		//��
0x001F,		//��
0x03E0,		//��
0x7C00,		//��
0x03FF,		//���F
0x7FE0,		//���F
0x7C1F,		//��
0x675a,0x0000,0x0000,0x779c,0x7fff,0x7f33,0x72ef,0x7fff,
};

/* �t�H���gCGX�f�[�^ */
#include "../scaddata/font8x16.dat"		//8x16�t�H���g�f�[�^�ifntequ.h �̒�`�ƈ�Έ�Ή��j
#include "../scaddata/font8x8.dat"		//8x8�t�H���g�f�[�^�idef8x8.h �̒�`�ƈ�Έ�Ή��j
#include "../scaddata/font8x8s.ach"		//8x8�e�t�t�H���g�f�[�^
#include "../scaddata/font8x16s.ach"	//8x16�e�t�t�H���g�f�[�^
#include "../scaddata/tenji.dat"		//�_���L�����N�^

/* MSG����J�[�\��CGX�f�[�^ */
#include "../scaddata/skip_cur.ach"
#define CHAR_SKIPCURSOR	(u32)skip_cur_Character
#define	BLINK_PAT	(sizeof(skip_cur_Character)/(CHRSIZE*2) )
//#define BLINK_PAT	4			//�@����J�[�\���A�j���̃p�^�[����
#define BLINK_WAIT	6			//�@����J�[�\���A�j���̃E�F�C�g



//-------------------------------------------------------------------------
//
//						���[�J���ϐ�
//
//-------------------------------------------------------------------------

//-- �����\���X�N���[������p�ϐ� --
static const FONT_CONTEXT *	NowContext;

/* �b�f�w�Z�b�g�p����\���� */
static TALK_WIN win_work ={};

/* ���b�Z�[�W���萧��p�t���O */
static u8 msg_skip_mode;

/* ��bMSG�̕\�����w�� */
static u8 talk_msg_width;

//-------------------------------------------------------------------------
//
//						���[�J���֐��v���g�^�C�v
//					
//-------------------------------------------------------------------------
static void print_bg_init(const FONT_CONTEXT * context);
static void print_pal_init(const FONT_CONTEXT * context);
static void print_reg_init(const FONT_CONTEXT * context);

static u16 Print8x16FontInit(const FONT_CONTEXT * context, u16 cgx_num);
static u16 Print8x8FontInit(const FONT_CONTEXT * context, u16 cgx_num);
static u16 Print8x8sFontInit(const FONT_CONTEXT * context, u16 cgx_num);
static u16 PrintTenjiFontInit(const FONT_CONTEXT * context, u16 cgx_num);
static void PrintSplitFontSet(const FONT_CONTEXT * context, u16 cgx_num, u8 code );

static u16 PrintFixScrnInit( TALK_WIN * tw ,u16 cgx_num);

static void MsgParamSetMain( TALK_WIN* tw, const u8* msg_adrs, u16 cgx_num, u8 xpos, u8 ypos);

static u8 StringPrint(TALK_WIN * tw);

static void (* const font_print_func[])(TALK_WIN *,int);

static void transcgx_font_print(TALK_WIN * tw, int code);
static void trans_font_print(TALK_WIN * tw, int code);
static void fix_font_print(TALK_WIN * tw, int code);
static void fixscrn_font_print(TALK_WIN * tw, int code );

static void FontCGXSet(int code, u32 cgx_adrs, u8 font, u8 col_f, u8 col_b, u8 col_s);
static void FontSCRSet(TALK_WIN * tw, u16 chip1, u16 chip2);
static u32 GetFontSCRAddr(TALK_WIN * tw);
static void GetFontAdrs( u8 font, u16 code, u32 * adr1, u32 * adr2 );

static void TalkMsgCR(TALK_WIN * tw);
static void	FontSpread(u32 FromAddress, u32 ToAddress, u8 FColorF, u8 FColorB);
static void	FontSpread2(u32 src, u32 dst, u8 ColorF, u8 ColorS, u8 ColorB);

static u32 FixScrn_CGXAdrs(TALK_WIN * tw);
static void FixScrn_FontSCRSet(TALK_WIN * tw);
static u32 FixScrnCGXoffset(TALK_WIN * tw, u8 x, u8 y );

/* MSG�f�[�^�擾�p�}�N�� */
#define GETCODE(tw)	( (tw)->text[(tw)->text_p++] )
#define PASSCODE(tw) ( (tw)->text_p++ )

/* �t�H���g�L�����̃f�[�^�A�h���X�擾�p�}�N�� */

#define FONT8x16DATAADRS(code)	( (u32)(Font8x16Data) + (u32)(code) * FDSIZE * 2 )

#define FONT8x8DATAADRS(code)	( (u32)(Font8x8Data) + (u32)(code) * 8 )

// ���L�̂悤�ȍ\���Ŕz�񂵂Ă���f�[�^���Ƃ肾�����ߌ��ň��������������ɂȂ��Ă���
// ��(��j0x0000	��(��j0x0010	��(��)0x0020 ....
// ��(���j0x0200	��(��) 0x0210	��(��)0x0220 ....
#define	FONT8x16sDATAADRS(code)	( (u32)(font8x16s_Character) \
		+ (u32)( (code & 0xfff0) * CHRSIZE * LFONT_CHR_USE + (code & 0xf)*CHRSIZE ) )

#define	FONT8x8sDATAADRS(code)	( (u32)(font8x8s_Character) + (u32)(code) * CHRSIZE )

#define	FONTtenjiDATAADRS(code)	( (u32)(tenji_Character) + (u32)(code) * 8 )


/* �t�H���g�L�����̃Z�b�g�A�h���X�擾�p�}�N�� */
#define LASTCGXNUMBER(tw)		( (tw)->cgx_n + (tw)->cgx_np )
#define LASTCGXADRS(tw)			( (tw)->context->CGXadrs + LASTCGXNUMBER(tw) * CHRSIZE ) 

/* �Œ�X�N���[���̃L�����R�[�h�v�Z�}�N�� */
#define FIXSCRNCODE(tw,x,y)	((tw)->cgx_n + FIXSCRNFONTUSE \
		+ (tw)->scr_x + (tw)->scr_xp + (x) \
		+ ( (tw)->scr_y + (tw)->scr_yp + (y) ) * VX_SIZE )


/* �g�p�p���b�g�擾�p�}�N�� */
#define PAL_VAL(tw)		( (tw)->palette << BG_SC_PLTT_SHIFT )

static u16 FontBlankCode(TALK_WIN * tw);

static u8 TalkMsgForceSkipCheck( TALK_WIN * tw );
static void TalkMsgWaitInit(TALK_WIN * tw);
static void TalkMsgWaitEnd(TALK_WIN * tw);
static void TalkMsgWaitMain(TALK_WIN * tw);
static u8 TalkMsgWait( TALK_WIN * tw );

static u8 TalkMsgSpeedGet( TALK_WIN * tw );


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//	���\�[�X�������֘A
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//=================================================================
//
//	���������[�`��
//
//	���p���b�g�A�j���Ɉˑ����Ă��邽��PaletteAnimeInit��肠�Ƃ�
//	�@�ĂԕK�v������
//	�������ŃX�N���[�����䃌�W�X�^��ݒ肵�Ă���
//=================================================================

//-----------------------------------------------------------------
//-----------------------------------------------------------------
void PrintScreenInit(CONTEXT_TYPE type)
{
	NowContext = GetFontContext(type);

	print_reg_init(NowContext);
	print_bg_init(NowContext);
	print_pal_init(NowContext);
}

//-----------------------------------------------------------------
//	VRAM�̏������i�󔒃L�����Z�b�g���X�N���[���N���A�j
//-----------------------------------------------------------------
void PrintBgInit( CONTEXT_TYPE type )
{
	print_bg_init( GetFontContext(type) );
}

//-----------------------------------------------------------------
//	�f�t�H���g�p���b�g�̃Z�b�g
//-----------------------------------------------------------------
void PrintPaletteInit( CONTEXT_TYPE type )
{
	print_pal_init( GetFontContext(type) );
}

//-----------------------------------------------------------------
//	�X�N���[�����W�X�^�̏�����
//-----------------------------------------------------------------
void PrintRegisterInit( CONTEXT_TYPE type )
{
	print_reg_init( GetFontContext(type) );
}


//-----------------------------------------------------------------
// �X�N���[�����������ʃ��[�`��
//-----------------------------------------------------------------
static void print_bg_init(const FONT_CONTEXT * context)
{
	CpuFastClear( 0, context->CGXadrs, CHRSIZE );	//�󔒃L�����Z�b�g
	CpuFastClear( 0, context->SCRadrs, 32*32*2 );	//�X�N���[���N���A
}

static void print_pal_init(const FONT_CONTEXT * context)
{
	PaletteWorkSet(PM_Print_Palette,(context->BasePalette)*16,32);
}

static void print_reg_init(const FONT_CONTEXT * context)
{
	*(RegBGHofs[context->BGNum]) = 0;
	*(RegBGVofs[context->BGNum]) = 0;
	*(RegBGctrl[context->BGNum]) =
			BG_COLOR_16											// 16�F�I��
			| BG_SCREEN_SIZE_0									// ��ذݻ���(256�~256)
			| ( context->BGpriority << BG_PRIORITY_SHIFT )		// BG�D�揇�ʎw��
			| ( context->SCRBlockNum << BG_SCREEN_BASE_SHIFT)	// ��ذ��ް���ۯ� 
			| ( context->CGXBlockNum << BG_CHAR_BASE_SHIFT)		// ��׸���ް���ۯ�
			;
}

//=================================================================
//	�풓�t�H���g����֘A
//=================================================================

//-----------------------------------------------------------------
//-----------------------------------------------------------------
u16 PrintFontInit(TALK_WIN * tw, u16 cgx_num)
{
	tw->cgx_n = cgx_num;

	switch ( tw->context->UseMode ) {
	case TRANSCGXMODE:
	case TRANSFONTMODE:
		return 0;
	case FIXSCRNMODE:
		return PrintFixScrnInit(tw ,cgx_num);
	case FIXFONTMODE:
		break;
	}

	/* FIXFONTMODE �̏ꍇ */
	switch ( tw->context->BaseFontType ) {

	case FONT8x16TYPE:
	case FONT8x16sTYPE:
		return Print8x16FontInit( tw->context, cgx_num );

	case FONT8x8TYPE:
	case FONT8x8RAWTYPE:
		return Print8x8FontInit( tw->context, cgx_num );

	case FONT8x8sTYPE:
	case FONT8x8sRAWTYPE:
		return Print8x8sFontInit( tw->context, cgx_num );

	case FONTTENJITYPE:
		return PrintTenjiFontInit( tw->context, cgx_num );
	}
	return 0;
}


//-----------------------------------------------------------------
//
//	�t�H���g�f�[�^�̕����]���Z�b�g
//
//-----------------------------------------------------------------

static const FONT_CONTEXT * fc;
static u16 cgx_num_sv;
static u16 count_sv;

int PRT_SetSplitFontInit( TALK_WIN * tw, u16 cgx_num )
{
	count_sv = 0;
	fc = tw->context;
	cgx_num_sv = cgx_num;

	tw->cgx_n = cgx_num;

	switch ( tw->context->UseMode ) {
	case TRANSCGXMODE:
	case TRANSFONTMODE:
		return 0;
	case FIXSCRNMODE:
		return PrintFixScrnInit(tw ,cgx_num);
	case FIXFONTMODE:
		break;
	}


	switch ( tw->context->BaseFontType ) {
	case FONT8x16TYPE:
	case FONT8x16sTYPE:
		return 256 * LFONT_CHR_USE;
	case FONT8x8TYPE:
	case FONT8x8RAWTYPE:
	case FONT8x8sTYPE:
	case FONT8x8sRAWTYPE:
		return 256;
	}
	return 0;
}

int PRT_SetSplitFontMain(void)
{
	int i;
	switch ( fc->UseMode ) {
	/* ���L�̃t�H���g�^�C�v�͏������݂̂œ]���s�v */
	case TRANSCGXMODE:
	case TRANSFONTMODE:
	case FIXSCRNMODE:
		return TRUE;
	}

	for ( i=count_sv; i<count_sv+16; i++ ) {
		PrintSplitFontSet(fc,cgx_num_sv,i);
	}
	count_sv += 16;
	if ( count_sv == 256 ) {
		return TRUE;		/* �]���I�� */
	} else {
		return FALSE;		/* �]���� */
	}
}


static void PrintSplitFontSet(const FONT_CONTEXT * context, u16 cgx_num, u8 code )
{
	u32 cgx;

	switch ( context->BaseFontType ) {

	case FONT8x16TYPE:
	case FONT8x16sTYPE:
		cgx = context->CGXadrs + (cgx_num * CHRSIZE) + (code * FSIZE);
		FontCGXSet(code, cgx, context->BaseFontType,
				context->BaseFontColor,
				context->BaseBGColor,
				context->BaseShadowColor);
		break;

	case FONT8x8TYPE:
	case FONT8x8RAWTYPE:
		cgx = context->CGXadrs + (code+cgx_num) * CHRSIZE ;
		FontSpread( FONT8x8DATAADRS(code),cgx,
				context->BaseFontColor,
				context->BaseBGColor);
		break;

	case FONT8x8sTYPE:
	case FONT8x8sRAWTYPE:
		cgx = context->CGXadrs + (code+cgx_num) * CHRSIZE ;
		FontSpread2( FONT8x8sDATAADRS(code), cgx,
				context->BaseFontColor,
				context->BaseShadowColor,
				context->BaseBGColor);
		break;
	}
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static u16 Print8x16FontInit(const FONT_CONTEXT * context, u16 cgx_num)
{
	int i;
	u32 cgx;

	for ( i=0; i<256; i++ ) {
		cgx = context->CGXadrs + (cgx_num * CHRSIZE) + (i * FSIZE);
		FontCGXSet(i, cgx, context->BaseFontType,
				context->BaseFontColor, context->BaseBGColor, context->BaseShadowColor);
	}
	return i * LFONT_CHR_USE;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
static u16 Print8x8FontInit(const FONT_CONTEXT * context, u16 cgx_num)
{
	int i;
	u32 cgx;
	for ( i=0; i<256; i++ ) {
		cgx = context->CGXadrs + (i+cgx_num) * CHRSIZE ;
		FontSpread( FONT8x8DATAADRS(i),cgx,
				context->BaseFontColor, context->BaseBGColor);
	}
	return i;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static u16 Print8x8sFontInit(const FONT_CONTEXT * context, u16 cgx_num)
{
	int i;
	u32 cgx;
	for ( i=0; i<256; i++ ) {
		cgx = context->CGXadrs + (i+cgx_num) * CHRSIZE ;
		FontSpread2( FONT8x8sDATAADRS(i), cgx,
				context->BaseFontColor,
				context->BaseShadowColor,
				context->BaseBGColor);
	}
	return i;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static u16 PrintTenjiFontInit(const FONT_CONTEXT * context, u16 cgx_num)
{
	int i;
	u32 cgx;
	for ( i=0; i<256; i++ ) {
		cgx = context->CGXadrs + (i+cgx_num) * CHRSIZE ;
		FontSpread( FONTtenjiDATAADRS(i),cgx,
				context->BaseFontColor, context->BaseBGColor);
	}
	return i;
}

//-----------------------------------------------------------------
//	�Œ�X�N���[�����[�h�̃L�����G���A������
//-----------------------------------------------------------------
static u16 PrintFixScrnInit( TALK_WIN * tw ,u16 cgx_num)
{
	tw->cgx_n = cgx_num;
	CpuFastClear( 0, tw->context->CGXadrs+tw->cgx_n * CHRSIZE, CHRSIZE );	//�󔒃L�����Z�b�g
	FontSpread( (u32)(&PM_Print_Blank), tw->context->CGXadrs + (tw->cgx_n+1)*CHRSIZE,
			tw->context->BaseFontColor, tw->context->BaseBGColor );
	return tw->cgx_n + FIXSCRNFONTUSE + VX_SIZE * VY_SIZE;
}


//=================================================================
//
//	Vsync�ł̓]�����[�`��
//	������_�~�[�Ŏg�p���Ă��Ȃ�
//
//=================================================================
void PrintScreenVsync(void)
{
		// dummy routine
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=================================================================
//	MSG �o�͐ݒ�
//=================================================================

//-----------------------------------------------------------------
// MsgControlInit
// TALK_WIN�\���̂̏���������
// <����>
//		tw		MSG����\����
//		n		MSG���[�h�̃^�C�v
//-----------------------------------------------------------------
void MsgControlInit(TALK_WIN * tw, CONTEXT_TYPE n)
{
	* tw = TalkWinInitializer;
	tw->context = GetFontContext(n);

	tw->mode	= tw->context->UseMode;
	tw->font	= tw->context->BaseFontType;
	tw->col_f	= tw->context->BaseFontColor;  	// �F�ԍ�(����)
	tw->col_b	= tw->context->BaseBGColor;		// �F�ԍ�(�w�i)
	tw->col_s	= tw->context->BaseShadowColor; // �F�ԍ�(�e)
	tw->palette = tw->context->BasePalette;		// �p���b�g
}


//-----------------------------------------------------------------
// TALK_WIN�\���̂ւ̃p�����[�^�ݒ�
//-----------------------------------------------------------------
static void MsgParamSetMain( TALK_WIN* tw, const u8* msg_adrs, u16 cgx_num, u8 xpos, u8 ypos)
{
	tw->status	= MSG_START;			// �ð��
	tw->work	= 0;
	tw->sw		= 0;					// �����׸�
	tw->mode	= tw->context->UseMode;
	tw->font	= tw->context->BaseFontType;
	tw->col_f	= tw->context->BaseFontColor;  	// �F�ԍ�(����)
	tw->col_b	= tw->context->BaseBGColor;		// �F�ԍ�(�w�i)
	tw->col_s	= tw->context->BaseShadowColor; // �F�ԍ�(�e)
	tw->palette = tw->context->BasePalette;		// �p���b�g

	tw->text	= (u8 *)msg_adrs;		// ÷�� �ް����ڽ
	tw->text_p	= 0;					// ÷�� �̾��

	tw->cgx_n	= cgx_num;		// CGX �ʒu
	tw->cgx_np	= 0;			// CGX Offset

	tw->scr_x	= xpos;			// ������ذ�X �ʒu
	tw->scr_xp	= 0;			// ������ذ�X Offset
	tw->scr_y	= ypos;			// ������ذ�Y �ʒu
	tw->scr_yp	= 0;			// ������ذ�Y Offset

	tw->wait	= 0;			// ����
}


//=================================================================
// MsgCGXDataSet
// �w��CGX�f�[�^�̃Z�b�g�݂̂������Ȃ�
//
//	adrs		�f�[�^��z�u����ʒu�ւ̃|�C���^
//	msg			���b�Z�[�W�f�[�^�ւ̃|�C���^
//=================================================================
void MsgCGXDataSet(void * adrs, const u8 * msg)
{
	win_work.context = GetFontContext( CGX_ONLY_TYPE );
	MsgParamSetMain( &win_work, msg, 0, 0, 0 );
	win_work.mode = TRANSCGXMODE;
	win_work.cgx_adrs = adrs;
	MsgPut( &win_work );
}


//=================================================================
//	MsgPutSet
//	������\���Z�b�g���[�`��
//
//	tw			�g�p���郁�b�Z�[�W�\����
//	msg_adrs	���b�Z�[�W�f�[�^�ւ̃|�C���^
//	cgx_num		�g�p�L�����J�n�ʒu�I�t�Z�b�g
//	xpos		�\���w�ʒu
//	ypos		�\���x�ʒu(8�~16�T�C�Y�t�H���g�̉�����j
//=================================================================
void MsgPutSet(TALK_WIN* tw, const u8* msg_adrs, u16 cgx_num, u8 xpos, u8 ypos)
{
	MsgParamSetMain(tw, msg_adrs, cgx_num, xpos, ypos);
}


//=================================================================
//	�E�B���h�E���b�Z�[�W�Z�b�g���[�`��
//
// <����>
//	tw			�g�p���郁�b�Z�[�W�\����
//	msg_adrs	���b�Z�[�W�f�[�^�ւ̃|�C���^
//	cgx_num		�g�p�L�����J�n�ʒu�I�t�Z�b�g
//	xpos,ypos	�\���ʒu
//=================================================================

void TalkMsgSet(TALK_WIN* tw, const u8* msg_adrs, u16 cgx_num, u8 xpos, u8 ypos)
{
	/* ����̃g���K�[������܂ŉ������ςȂ��ő��肪�ł��Ȃ��悤�ɂ��� */
	sys.TrgFlagMask = A_BUTTON | B_BUTTON;
	sys.TrgFlag = 0;

	msg_skip_mode = MSGSKIP_NORMAL;
	talk_msg_width = WIN_LINE_SZ;

	MsgParamSetMain(tw, msg_adrs, cgx_num, xpos, ypos);

	if ( tw->mode == TRANSFONTMODE ) {
		FontSpread( (u32)(&PM_Print_Blank), LASTCGXADRS(tw), tw->col_b, tw->col_b );
		FontSpread( FONT8x8DATAADRS(d_cursor_8x8),
				LASTCGXADRS(tw) + CHRSIZE, tw->col_f, tw->col_b );
		tw->cgx_np = WIN_LINE1_CHRSTART;
	}
}



//=================================================================
//	MsgPut
//	MSG�o�̓��[�`���i�w�蕶�����S�ĕ\�����Ă���Ԃ��Ă���j
//
//	<���̓p�����[�^>
//	tw			�g�p���郁�b�Z�[�W�\����
//=================================================================
extern const u8 * GetIndirectString( int i );

//���݂̕�������Z�[�u���ĐV�K������\��
static u8 recursive_msg_put( TALK_WIN * tw , const u8 * next_text )
{
	const u8 * sv_text;
	u16 sv_text_p;
	u8 result;

	sv_text = tw->text;
	sv_text_p = tw->text_p;
	tw->text = next_text;
	tw->text_p = 0;
	tw->status = MSG_WRITE;

	result = MsgPut( tw );

	tw->text = sv_text;
	tw->text_p = sv_text_p;
	tw->status = MSG_WRITE;

	return result;
}

u8 MsgPut(TALK_WIN * tw)
{
	while ( tw->status != MSG_END ) {
		if ( tw->status == MSG_CR ) {
			tw->scr_xp = 0;
			tw->scr_yp += 2;	//YײݕύX����
			tw->status = MSG_WRITE;

		} else if( tw->status == MSG_INDIRECT ) {
			recursive_msg_put( tw, GetIndirectString(GETCODE(tw)) );
		}

		StringPrint(tw);
	}
	return 1;
}

//=================================================================
//	ContinueMsgSet(2002/4/30 TETSU)
//
//	<���̓p�����[�^>
//	tw			�g�p���郁�b�Z�[�W�\����
//=================================================================
void ContinueMsgSet(TALK_WIN* tw, const u8* msg_adrs)
{
	tw->status	= MSG_WRITE;
	tw->text	= msg_adrs;		// ÷�� �ް����ڽ
	tw->text_p	= 0;			// ÷�� �̾��
	tw->work	= 0;
	tw->sw		= 0;			// �����׸�
	tw->wait	= 0;			// ����
}

//=================================================================
//	������\�����[�`���i�ꕶ���\�����邲�ƂɕԂ��Ă���j
//	<����>
//	tw		�g�p���郁�b�Z�[�W�\����
//	<�߂�l>
//	0		�\����
//	1		�I��
//=================================================================
u8 TalkMsgPutc( TALK_WIN * tw )
{
	while ( tw->status != MSG_END ) {
		if (tw->status == MSG_CR) {
			tw->scr_xp = 0;
			tw->scr_yp += 2;	//YײݕύX����
			tw->status = MSG_WRITE;
		}
		if ( StringPrint(tw) == STRPRT_PUT ) return 0;
	}
	return 1;
}

//=================================================================
//
//	�E�B���h�E���b�Z�[�W���C��
//
//=================================================================

//-----------------------------------------------------------------
//	�E�B���h�E���b�Z�[�W���䃁�C��
//-----------------------------------------------------------------
static u8 talk_msg_main(TALK_WIN * tw)
{
	switch ( tw->status ) {

	case MSG_END:			/*	MSG�I��	*/
		return 1;

	case MSG_KEYWAIT:		/*	�{�^���҂�	*/
		if ( TalkMsgForceSkipCheck(tw) == TRUE ) {
			tw->wait --;
			if( tw->wait == 0 ) tw->status = MSG_WRITE;
		} else if ( sys.Trg & AB_BUTTON ) {
			SePlay( SE_SELECT );
			tw->status = MSG_WRITE ;
		}
		return 0;

	case MSG_WAIT:			/* �E�F�C�g	*/
		if ( TalkMsgForceSkipCheck(tw) != TRUE ) {
			if ( (sys.Cont & AB_BUTTON) && sys.TrgFlag == 1 ) {
				//�{�^�������ő�����
				tw->wait = 0;
				tw->status = MSG_WRITE;
				break;
			}
		}
		if (tw->wait != 0) { tw->wait --;}
		if (tw->wait != 0) return 0;
		tw->status = MSG_WRITE;
		break;

	case MSG_NWAIT:			/* �{�^���҂����N���A */
		if ( TalkMsgWait(tw) ) {
			TalkMsgClr( tw );
			tw->status = MSG_WRITE;
		}
		return 0;

	case MSG_SWAIT:			/* �{�^���҂������s�X�N���[�� */
		if ( TalkMsgWait(tw) ) {
			TalkMsgCR( tw );
			tw->status = MSG_WRITE;
		}
		return 0;

	case MSG_TIMEWAIT:
		if ( tw->wait != 0 ) tw->wait --;
		if ( tw->wait != 0 ) return 0;
		tw->status = MSG_WRITE;
		break;

	case MSG_INDIRECT:
		PASSCODE(tw);
		tw->status = MSG_WRITE;
		break;

	case MSG_CR:			/* ���s	*/
		TalkMsgCR(tw);
		tw->status = MSG_WRITE;
		return 0;

	case MSG_START:			/*	MSG�o�͊J�n	*/
		TalkMsgClr(tw);
	case MSG_WRITE:			/*	MSG�o�͒�	*/
		break;

	case MSG_SEWAIT:		/* SE�̖�I����҂� */
		if ( SeEndCheck() ) return 0;
		tw->status = MSG_WRITE;
		break;

	default:				/*	����`��Ԃ͋����I��	*/
		tw->status = MSG_END;
		return 1;
	}

	StringPrint(tw);

	switch ( tw->status ) {
	case MSG_CR:
	case MSG_SEWAIT:
	case MSG_TIMEWAIT:
		return 0;

	case MSG_KEYWAIT:
	case MSG_NWAIT:
	case MSG_SWAIT:
		if ( TalkMsgForceSkipCheck(tw) == TRUE ) tw->wait = COMM_BATTLE_WAIT;
		return 0;
	case MSG_END:
		return 1;
	}

	tw->status = MSG_WAIT;
	tw->wait = TalkMsgSpeedGet( tw );
	return 0;
}

//-----------------------------------------------------------------
//	�E�B���h�E���b�Z�[�W���䃋�[�`��
//
//	��1�����\��or1�E�F�C�g���ɕԂ��Ă���B
//
// <����>
//	tw			�g�p���郁�b�Z�[�W�\����
//
// <�߂�l>
//	0			�\����
//	1			�w�蕶����̕\���I��
//-----------------------------------------------------------------
u8 TalkMsgPut( TALK_WIN * tw )
{
	msg_skip_mode = MSGSKIP_NORMAL;
	return talk_msg_main( tw );
}

//-----------------------------------------------------------------
//	�o�g���p�E�B���h�E���b�Z�[�W���䃋�[�`��
//	���ʐM�ΐ펞�ɕ������肪�ł��Ȃ��Ȃ�ȊO��TalkMsgPut�Ɠ����@�\
//-----------------------------------------------------------------
u8 BattleTalkMsgPut( TALK_WIN * tw)
{
	u8 res;

	msg_skip_mode = MSGSKIP_BATTLE;
	talk_msg_width = 26;
	res = talk_msg_main( tw );
	talk_msg_width = WIN_LINE_SZ;
	msg_skip_mode = MSGSKIP_NORMAL;
	return res;
}

//-----------------------------------------------------------------
//	�ʐM�p�E�B���h�E���b�Z�[�W���䃋�[�`��
//	����ɕ������肪�֎~�����
//-----------------------------------------------------------------
u8 NoSkipTalkMsgPut( TALK_WIN * tw)
{
	u8 res;

	msg_skip_mode = MSGSKIP_FORCE;
	res = talk_msg_main( tw );
	msg_skip_mode = MSGSKIP_NORMAL;
	return res;
}

//-----------------------------------------------------------------
//	�R���e�X�g�p�E�B���h�E���b�Z�[�W���䃋�[�`��
//-----------------------------------------------------------------
u8 ContestTalkMsgPut( TALK_WIN * tw )
{
	u8 res;

	msg_skip_mode = MSGSKIP_CONTEST;
	talk_msg_width = 16;	//15;	2002.08.19(��) by matsuda
	res = talk_msg_main( tw );
	talk_msg_width = WIN_LINE_SZ;
	return res;
}

//=================================================================
//	MsgPrint
//	������Z�b�g���\�����[�`��
//	�������I�ɂ�MsgPutSet��MsgPut�̌Ăяo���܂ōs���Ă���
//
//	<���̓p�����[�^>
//	tw			�g�p���郁�b�Z�[�W�\����
//	msg_adrs	���b�Z�[�W�f�[�^�ւ̃|�C���^
//	cgx_num		�g�p�L�����J�n�ʒu�I�t�Z�b�g
//	xpos		�\���w�ʒu
//	ypos		�\���x�ʒu(8�~16�T�C�Y�t�H���g�̉�����j
//=================================================================
u8 MsgPrint( TALK_WIN* tw, const u8* msg_adrs, u16 cgx_num, u8 xpos, u8 ypos)
{
	MsgPutSet(tw, msg_adrs, cgx_num, xpos, ypos);
	return MsgPut(tw);
}

//=================================================================
//	�ꕶ���\���p���[�`��
//	<���̓p�����[�^>
//	tw			�g�p���郁�b�Z�[�W�\����
//	code		�\�����镶���R�[�h
//	cgx_num		�g�p�L�����J�n�ʒu�I�t�Z�b�g
//	xpos		�\���w�ʒu
//	ypos		�\���x�ʒu(8�~16�T�C�Y�t�H���g�̉�����j
//=================================================================
u8 MsgPutc( TALK_WIN* tw, u8 code, u16 cgx_num, u8 xpos, u8 ypos )
{
	u8 dmy_msg[2];
	dmy_msg[0] = code;
	dmy_msg[1] = EOM_;

	MsgParamSetMain( tw, dmy_msg, cgx_num, xpos, ypos );
	return StringPrint(tw);
}


//=================================================================
//
//	�����񏈗����C�����[�`��
//
//	�������������Ƃɏ�ʂ̃��[�`���ɖ߂�
//	�@��ʃ��[�`����StringPrint�ł�tw->status�̕ύX��
//	�@��萧���ς���
//
//=================================================================

//-----------------------------------------------------------------
//	����R�[�h����
//-----------------------------------------------------------------
static u8 CtrlPrint( TALK_WIN * tw )
{
	u16	musno=0;

	switch ( GETCODE(tw) ) {

	case C_FCOL_:
		tw->col_f = GETCODE(tw);
		return STRPRT_CTRL;

	case C_BCOL_:
		tw->col_b = GETCODE(tw);
		return STRPRT_CTRL;

	case C_SCOL_:
		tw->col_s = GETCODE(tw);
		return STRPRT_CTRL;

	case C_ACOL_:
		tw->col_f = GETCODE(tw);
		tw->col_b = GETCODE(tw);
		tw->col_s = GETCODE(tw);
		return STRPRT_CTRL;

	case C_PAL_:
		tw->palette = GETCODE(tw);
		return STRPRT_CTRL;

	case C_FCHG_:
		tw->font = GETCODE(tw);
		return STRPRT_CTRL;

	case C_FNML_:
		tw->font = tw->context->BaseFontType;
		return STRPRT_CTRL;

	case C_WAIT_:
		tw->status = MSG_TIMEWAIT;
		tw->wait = GETCODE(tw);
		return STRPRT_CTRL;

	case C_TRGWAIT_:
		tw->status = MSG_KEYWAIT;
		return STRPRT_CTRL;

//add by soga 2002.06.12
	case C_SEWAIT_:
		tw->status = MSG_SEWAIT;
		return STRPRT_CTRL;
	case C_MUSPLAY_:
		musno = GETCODE(tw);
		musno |= (GETCODE(tw)<<8);
		MusicPlay(musno);
		return STRPRT_CTRL;
//add by soga 2002.09.29
	case C_SEPLAY_:
		musno = GETCODE(tw);
		musno |= (GETCODE(tw)<<8);
		SePlay(musno);
		return STRPRT_CTRL;

	case C_EXFONT_:
		font_print_func[tw->mode](tw,GETCODE(tw));
		return STRPRT_PUT;

	case C_XCHG_:
		tw->scr_xp += GETCODE(tw);
		return STRPRT_PUT;

	case C_YCHG_:
		tw->scr_yp += GETCODE(tw);
		return STRPRT_PUT;

	case C_CLR_:
		TalkMsgClr(tw);
		return STRPRT_CTRL;

	default:
		return STRPRT_CTRL;
	}
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static u8 StringPrint(TALK_WIN * tw)
{
	int code;

	code = GETCODE(tw);

	switch ( code ) {
		case EOM_ :
			tw->status= MSG_END;
			return STRPRT_END;

		case I_MSG_ :
			tw->status = MSG_INDIRECT;
			return STRPRT_CTRL;

		case CR_ :
			tw->status = MSG_CR;
			return STRPRT_CTRL;

		case NORMAL_WAIT_:
			TalkMsgWaitInit(tw);
			tw->status = MSG_NWAIT;
			return STRPRT_CTRL;

		case SCROLL_WAIT_:
			TalkMsgWaitInit(tw);
			tw->status = MSG_SWAIT;
			return STRPRT_CTRL;

		case CTRL_:
			return CtrlPrint(tw);

		default:
			font_print_func[tw->mode](tw,code);
			return STRPRT_PUT;
	}

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//	�����`��֘A
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//=================================================================
//
//	�����\�����C�����[�`��
//
//=================================================================

static void (* const font_print_func[])(TALK_WIN *,int)  =
{
	trans_font_print,			//TRANSFONTMODE
	fix_font_print,				//FIXFONTMODE
	transcgx_font_print,		//TRANSCGXMODE
	fixscrn_font_print,			//FIXSCRNMODE
};


//-----------------------------------------------------------------
//	�L�����f�[�^�]���݂̂̏���
//-----------------------------------------------------------------
static void transcgx_font_print(TALK_WIN * tw, int code)
{
	FontCGXSet(code,(u32)tw->cgx_adrs,tw->font,tw->col_f,tw->col_b,tw->col_s);
	tw->cgx_adrs += CHRSIZE*LFONT_CHR_USE;
}

//-----------------------------------------------------------------
//	�Œ�X�N���[���A�L�����]���ŕ`��
//-----------------------------------------------------------------
static void FixScrnFontCGXSet(int code, u32 cgx_adrs,
		u8 font, u8 col_f, u8 col_b, u8 col_s )
{
	u32 data1,data2;

	GetFontAdrs( font, code, &data1, &data2 );

	switch( font )
	{
	case FONT8x16TYPE:
	case FONT8x8TYPE:
	case FONT8x8RAWTYPE:
	case FONTTENJITYPE:
		FontSpread( data1, cgx_adrs,						col_f, col_b );
		FontSpread( data2, cgx_adrs + CHRSIZE * VX_SIZE,	col_f, col_b );
		break;

	case FONT8x16sTYPE:
	case FONT8x8sTYPE:
	case FONT8x8sRAWTYPE:
		FontSpread2( data1, cgx_adrs,						col_f, col_s, col_b );
		FontSpread2( data2, cgx_adrs + CHRSIZE * VX_SIZE,	col_f, col_s, col_b );
		break;
	}
}

static void FixScrnPrintMain(TALK_WIN * tw, int code)
{
	vu32 cgx_adrs;

	cgx_adrs = FixScrn_CGXAdrs(tw);
	FixScrnFontCGXSet( code ,cgx_adrs, tw->font, tw->col_f, tw->col_b, tw->col_s );
	FixScrn_FontSCRSet(tw);
}


static void fixscrn_font_print(TALK_WIN * tw, int code )
{
	FixScrnPrintMain(tw,code);
	tw->scr_xp++;
}

static void FixScrn_FontSCRSet(TALK_WIN * tw)
{
	u16 data1,data2;

	data1 = FIXSCRNCODE( tw, 0, 0 );
	data2 = FIXSCRNCODE( tw, 0, 1 );
	FontSCRSet( tw, data1, data2 );
}

static u32 FixScrn_CGXAdrs(TALK_WIN * tw)
{
	u16 x,y;

	x = tw->scr_x + tw->scr_xp;
	y = tw->scr_y + tw->scr_yp;

	return FixScrnCGXoffset( tw, x, y );
}

static u32 FixScrnCGXoffset(TALK_WIN * tw, u8 x, u8 y )
{
	return tw->context->CGXadrs + CHRSIZE * (tw->cgx_n + FIXSCRNFONTUSE + x + y * VX_SIZE );
}

//-----------------------------------------------------------------
//	�]���t�H���g�ŕ`��
//-----------------------------------------------------------------
static void TransFontPrintMain(TALK_WIN * tw, int code)
{
	u32 cgx_adrs;
	u16 chip;

	cgx_adrs = LASTCGXADRS(tw);
	FontCGXSet(code,cgx_adrs,tw->font,tw->col_f,tw->col_b,tw->col_s);

    chip = LASTCGXNUMBER(tw);
	FontSCRSet(tw, chip, chip+1);
}

static void trans_font_print(TALK_WIN * tw, int code)
{
	TransFontPrintMain(tw,code);
	tw->cgx_np += LFONT_CHR_USE;
	tw->scr_xp++;
}

//-----------------------------------------------------------------
//	�풓�t�H���g�ŕ`��
//-----------------------------------------------------------------
static void fix_8x16font_print( TALK_WIN * tw, int code )
{
	u16 chip1,chip2;

	chip1 = tw->cgx_n + code * LFONT_CHR_USE;
	chip2 = chip1 + 1;
	FontSCRSet(tw, chip1, chip2);
}

static void fix_8x8font_print( TALK_WIN * tw, int code )
{
	u16 chip1,chip2;

	chip1 = tw->cgx_n + ConvTo8x8Table[code].chr1st;
	chip2 = tw->cgx_n + ConvTo8x8Table[code].chr2nd;
	FontSCRSet(tw, chip1, chip2);
}

static void fix_8x8rawfont_print( TALK_WIN * tw, int code )
{
	u16 chip1,chip2;

	chip1 = tw->cgx_n + spc_8x8;
	chip2 = tw->cgx_n + code;
	FontSCRSet(tw, chip1, chip2);
}

static void fix_tenjifont_print( TALK_WIN * tw, int code )
{
	u16 chip1,chip2;

	chip1 = tw->cgx_n + BrailleConvTable[code].chr1st;
	chip2 = tw->cgx_n + BrailleConvTable[code].chr2nd;
	FontSCRSet(tw, chip1, chip2);
}

static void (* const fix_font_print_func[])( TALK_WIN * tw, int code ) =
{
	fix_8x16font_print,			//FONT8x16TYPE
	fix_8x8font_print,			//FONT8x8TYPE
	fix_8x8rawfont_print,		//FONT8x8RAWTYPE

	fix_8x16font_print,			//FONT8x16sTYPE
	fix_8x8font_print,			//FONT8x8sTYPE
	fix_8x8rawfont_print,		//FONT8x8sRAWTYPE
	fix_tenjifont_print,		//FONTTENJITYPE
};


static void fix_font_print(TALK_WIN * tw, int code)
{
	fix_font_print_func[tw->font](tw,code);
	tw->scr_xp++;
}

//=================================================================
//
//	�����t�H���g�������[�`��
//
//=================================================================
static void FontCGXSet(int code, u32 cgx_adrs, u8 font, u8 col_f, u8 col_b, u8 col_s)
{
	u32 data1,data2;

	GetFontAdrs( font, code, &data1, &data2 );

	switch ( font ) {

	case FONT8x16TYPE:
	case FONT8x8TYPE:
	case FONT8x8RAWTYPE:
	case FONTTENJITYPE:
		FontSpread( data1, cgx_adrs,			col_f, col_b );
		FontSpread( data2, cgx_adrs + CHRSIZE,	col_f, col_b );
		break;

	case FONT8x16sTYPE:
	case FONT8x8sTYPE:
	case FONT8x8sRAWTYPE:
		FontSpread2( data1, cgx_adrs,			col_f, col_s, col_b );
		FontSpread2( data2, cgx_adrs + CHRSIZE,	col_f, col_s, col_b );
		break;
	}

}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static void FontSCRSet(TALK_WIN *tw, u16 chip1, u16 chip2)
{
	u32 set_adrs;

	set_adrs = GetFontSCRAddr(tw);

   	*(u16 *)(set_adrs)			= PAL_VAL(tw) | chip1;
   	*(u16 *)(set_adrs+2*SCRX_SZ)= PAL_VAL(tw) | chip2;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static u32 GetFontSCRAddr(TALK_WIN * tw)
{
	u8 scx,scy;

	scx = tw->scr_x + tw->scr_xp;
	scy	= tw->scr_y + tw->scr_yp;
	return (u32)( tw->context->SCRadrs+2*(SCRX_SZ*scy + scx) ) ;
}

//-----------------------------------------------------------------
//	�g�p�p���b�g�̒l���擾����
//-----------------------------------------------------------------
u16	FontPalValue(TALK_WIN * tw)
{
	return PAL_VAL(tw);
}

//-----------------------------------------------------------------
//	�t�H���g�A�h���X�擾
//-----------------------------------------------------------------
static void GetFontAdrs( u8 font, u16 code, u32 * adr1, u32 * adr2 )
{
	switch( font )
	{
	case FONT8x16TYPE:
		*adr1 = FONT8x16DATAADRS( code );
		*adr2 = *adr1 + FDSIZE;
		break;

	case FONT8x8TYPE:
		*adr1 = FONT8x8DATAADRS( ConvTo8x8Table[code].chr1st );
		*adr2 = FONT8x8DATAADRS( ConvTo8x8Table[code].chr2nd );
		break;

	case FONT8x8RAWTYPE:
		*adr1 = FONT8x8DATAADRS( spc_8x8 );
		*adr2 = FONT8x8DATAADRS( code );
		break;

	case FONT8x16sTYPE:
		*adr1 = FONT8x16sDATAADRS( code );
		*adr2 = *adr1 + CHRSIZE * 16;
		break;

	case FONT8x8sTYPE:
		*adr1 = FONT8x8sDATAADRS( ConvTo8x8Table[code].chr1st );
		*adr2 = FONT8x8sDATAADRS( ConvTo8x8Table[code].chr2nd );
		break;

	case FONT8x8sRAWTYPE:
		*adr1 = FONT8x8sDATAADRS( spc_8x8 );
		*adr2 = FONT8x8sDATAADRS( code );
		break;

	case FONTTENJITYPE:
		*adr1 = FONTtenjiDATAADRS( BrailleConvTable[code].chr1st );
		*adr2 = FONTtenjiDATAADRS( BrailleConvTable[code].chr2nd );
		break;

	default:
		return;
	}
}


/************************************************************************/
/*			FontSpread													*/
/*                                                                      */
/*  ���e    �F�P�����i8*8dot)�̷�ׂ�W�J&�]��							*/
/*  IN        	u32 FromAddress	:�]�������ڽ							*/
/*				u32 ToAddress	:�]������ڽ							*/
/*            	u8  FColorF		:�����F(0�`15)							*/
/*            	u8  FColorB		:�w�i�F(0�`15)							*/
/************************************************************************/
static void FontSpread(u32 FromAddress, u32 ToAddress, u8 FColorF, u8 FColorB)
{
    u8  i,j;                    // for���p
    u8  Read;                   // �ǂݍ��ݗp
    u32 Write;                  // �������ݗp

    for (i=0;i<8;i++) {                                 // �c8�s
        Write = 0x00000000;                             // ���������ް��ر
        Read  = *(u8 *)(FromAddress + i);               // 8bit�ǂݍ���     
        for (j=0;j<8;j++) {                             // ��8��
            if (Read & 0x80)
			{
					Write |= (FColorF<<(4*j)); // 1bit��4bit�ɓW�J(16�F)����
			}else{
					Write |= (FColorB<<(4*j)); // 1bit��4bit�ɓW�J(16�F)�w�i
			};
            Read <<= 1;
        }
        *(u32 *)(ToAddress + i*4) = Write;              // 32bit��������
    }
}


static u8 conv[CHRSIZE];

static void FontSpread2(u32 src, u32 dst, u8 ColorF, u8 ColorS, u8 ColorB )
{
	int i;
	u8 Read;
	u8 write;

	for ( i=0; i<CHRSIZE; i++ ) {

		Read = ((u8 *)src)[i] & 0xf0;
		if ( Read == 0x0 ) {
			write = ColorB << 4;
		}
		else if ( Read == 0xf0 ) {
			write = ColorF << 4;
		}
		else if ( Read == 0xe0 ) {
			write = ColorS << 4;
		}
		else {
			write = Read & 0xf0;
		}

		Read = ((u8 *)src)[i] & 0x0f;
		if ( Read == 0x0 ) {
			write |= ColorB;
		}
		else if ( Read == 0xf ) {
			write |= ColorF;
		}
		else if (Read == 0xe ) {
			write |= ColorS;
		}
		else {
			write |= Read;
		}
		conv[i] = write;
	}

	CpuCopy(conv,dst,CHRSIZE,32);
}





//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//	��b�E�B���h�E����֘A�T�u���[�`��
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=================================================================
//	��b�E�B���h�E:MSG���葬�x�擾
//=================================================================
static const u8 msg_speed[] = {
	SLOW_MSG_SPEED,
	NORMAL_MSG_SPEED,
	FAST_MSG_SPEED
};

static u8 TalkMsgSpeedGet( TALK_WIN * tw )
{
	if( TalkMsgForceSkipCheck( tw ) ) return NORMAL_MSG_SPEED;
	return msg_speed[ MyData.msg_speed ];
}


//=================================================================
//
//	��b�E�B���h�E:���s����
//	
//	1�s�ڂ̏ꍇ��2�s�ڂ̐擪�ɕ\���ʒu���ς��
//	2�s�ڂ̏ꍇ��2�s�ڂ̓��e��1�s�ڂɈړ����A2�s�ڂ��N���A����
//
//=================================================================

//-----------------------------------------------------------------
//	���s�L�[�̖����`�F�b�N
//-----------------------------------------------------------------
static u8 TalkMsgForceSkipCheck( TALK_WIN * tw )
{
	if( msg_skip_mode == MSGSKIP_FORCE )return TRUE;
	if( msg_skip_mode == MSGSKIP_NORMAL )return FALSE;

	if( msg_skip_mode == MSGSKIP_CONTEST ) {
		if( ContestTuusin ) return TRUE;
		return FALSE;
	}

	if( msg_skip_mode == MSGSKIP_BATTLE ) {
		if( FightType & FIGHT_TYPE_TUUSHIN ) return TRUE;
		return FALSE;
	}

	return FALSE;
}

static void TalkMsgCR_TransFont(TALK_WIN * tw);
static void TalkMsgCR_FixFont(TALK_WIN * tw);
static void TalkMsgCR_FixScrn(TALK_WIN * tw);
static void LineMove2( TALK_WIN * tw, u8 width );
static void LineMove( TALK_WIN * tw, u16 width );
//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
static void TalkMsgCR(TALK_WIN * tw)
{
	switch ( tw->mode ) {

	case TRANSFONTMODE:
		TalkMsgCR_TransFont(tw);
		break;
	case FIXFONTMODE:
		TalkMsgCR_FixFont(tw);
		break;
	case TRANSCGXMODE:
		/* ���肦�Ȃ� */
		break;
	case FIXSCRNMODE:
		TalkMsgCR_FixScrn(tw);
		break;
	}
}

//-----------------------------------------------------------------
//	���s�̐���
//	���@�P�s�ڂ̏ꍇ�͂Q�s�ڂ��珑���n�߂�悤�ɂ���
//	�@�@�Q�s�ڂ̏ꍇ�͎g�p���Ă���L�����̈���t���b�v���A
//		�Q�s�ڂ��P�s�ڂɈړ�����
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
static void TalkMsgCR_TransFont(TALK_WIN * tw)
{
	//1�s�ڂ�?
	if (tw->scr_yp == 0) {
		tw->cgx_np = WIN_LINE2_CHRSTART(talk_msg_width);
		tw->scr_xp = 0;
		tw->scr_yp += 2;
	} else {
		if ( tw->sw & Y_CGX_FLAG ) {
			tw->cgx_np = WIN_LINE2_CHRSTART(talk_msg_width);
		} else {
			tw->cgx_np = WIN_LINE1_CHRSTART;
		}

		tw->sw ^= Y_CGX_FLAG;

		tw->scr_xp = 0;
		LineMove( tw, talk_msg_width );
	}
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static void TalkMsgCR_FixFont(TALK_WIN * tw)
{		
	//1�s�ڂ�?
	if ( tw->scr_yp == 0 ) {
		tw->scr_xp = 0;
		tw->scr_yp += 2;
	} else {
		tw->sw ^= Y_CGX_FLAG;
		tw->scr_xp = 0;
		LineMove( tw, talk_msg_width );
	}
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
static void TalkMsgCR_FixScrn(TALK_WIN * tw)
{
	//1�s�ڂ�?
	if ( tw->scr_yp == 0 ) {
		tw->scr_xp = 0;
		tw->scr_yp += 2;
	} else {
		tw->sw ^= Y_CGX_FLAG;
		tw->scr_xp = 0;
		LineMove2( tw, talk_msg_width );
	}
}


//-----------------------------------------------------------------
//	�Œ�t�H���g���[�h�A�]���t�H���g���[�h�p
//-----------------------------------------------------------------
static void LineMove( TALK_WIN * tw, u16 width )
{
	int i;
	u16 * scrn;
	u16 set_code;

	set_code = FontPalValue(tw) | FontBlankCode(tw);
	scrn = (u16 *) ( GetFontSCRAddr(tw) );
	scrn -= SCRX_SZ*2;			// line1��i���|�C���g����

	CpuCopy( &scrn[SCRX_SZ*2], &scrn[SCRX_SZ*0], width*2, 16);	//line2��i��line1��i�֓]��
	CpuCopy( &scrn[SCRX_SZ*3], &scrn[SCRX_SZ*1], width*2, 16);	//line2��i��line1��i�֓]��

	for ( i=0; i<width; i++ ) {
		scrn[ i + SCRX_SZ*2 ] = set_code;
		scrn[ i + SCRX_SZ*3 ] = set_code;
   	}
}

//-----------------------------------------------------------------
// �Œ�X�N���[�����[�h�p
//-----------------------------------------------------------------
static void LineMove2( TALK_WIN * tw, u8 width )
{
	u32 up_adr,dw_adr,base_adr;

	dw_adr = FixScrn_CGXAdrs(tw);						//line2��i
	up_adr = dw_adr - 2 * VX_SIZE * CHRSIZE;			//line1��i
	CpuCopy( dw_adr, up_adr, (width * CHRSIZE), 32);

	dw_adr = FixScrn_CGXAdrs(tw) + VX_SIZE * CHRSIZE;	//line2���i
	up_adr = dw_adr - 2 * VX_SIZE * CHRSIZE;			//line1���i
	CpuCopy( dw_adr, up_adr, (width * CHRSIZE), 32);

	FixScrnPrintMain( tw, spc_ );						//�ЂƂ����󔒂�\���A���Ƃ̓R�s�[����
	base_adr = FixScrn_CGXAdrs(tw);
	CpuCopy( base_adr, base_adr+CHRSIZE, CHRSIZE*(width-1), 32 );
	CpuCopy( base_adr, base_adr+CHRSIZE*VX_SIZE, CHRSIZE*width, 32 );
}

//=================================================================
//
//	��b�E�B���h�E:��b�E�B���h�E�̃N���A
//
//=================================================================
//-----------------------------------------------------------------
//-----------------------------------------------------------------
static void TalkMsgClrMain( TALK_WIN * tw, u8 width );
static void TalkMsgClr_FixScrn( TALK_WIN * tw, u8 width );
static void FixScrnCodeSetRect( TALK_WIN * tw, u8 x1, u8 y1, u8 x2, u8 y2 );
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void TalkMsgClr(TALK_WIN * tw)
{
	switch ( tw->mode ) {

	case TRANSFONTMODE: 
		TalkMsgClrMain(tw, talk_msg_width);
		tw->cgx_np = WIN_LINE1_CHRSTART;
		break;

	case FIXFONTMODE:
		TalkMsgClrMain(tw, talk_msg_width);
		break;

	case FIXSCRNMODE:
		TalkMsgClr_FixScrn(tw, talk_msg_width);
		break;

	case TRANSCGXMODE:
		/* ���肦�Ȃ� */
		break;
	}
}

//-----------------------------------------------------------------
//	�E�B���h�E���b�Z�[�W�̃N���A
//	���󔒕����ŃE�B���h�E���b�Z�[�W�\���̈���N���A����
//-----------------------------------------------------------------
static void TalkMsgClrMain( TALK_WIN * tw, u8 width )
{
	u16 * scr;
	u16 code;
	u8 x,y;

	tw->scr_xp = 0;
	tw->scr_yp = 0;
	tw->sw = 0;

	scr = (u16 *) ( GetFontSCRAddr(tw) );
	code = FontPalValue(tw) | FontBlankCode(tw);

	for ( y=0; y<4; y++ ) {
		for ( x=0; x<width ;x++ ) {
			scr[SCRX_SZ * y + x] = code;
		}
	}
}

//-----------------------------------------------------------------
//	�Œ�X�N���[�����[�h�p
//	���󔒃L�������Z�b�g����
//-----------------------------------------------------------------
static void TalkMsgClr_FixScrn( TALK_WIN * tw, u8 width )
{
	u32 cgx;

	tw->scr_xp = 0;
	tw->scr_yp = 0;
	tw->sw = 0;

	FixScrnPrintMain( tw, spc_ );
	cgx = FixScrn_CGXAdrs(tw);
	CpuCopy( cgx, cgx+CHRSIZE, CHRSIZE*(width-1), 32 );
	CpuCopy( cgx, cgx+CHRSIZE*VX_SIZE*1, CHRSIZE*width, 32 );
	CpuCopy( cgx, cgx+CHRSIZE*VX_SIZE*2, CHRSIZE*width, 32 );
	CpuCopy( cgx, cgx+CHRSIZE*VX_SIZE*3, CHRSIZE*width, 32 );

	FixScrnCodeSetRect( tw, tw->scr_x, tw->scr_y, tw->scr_x + width, tw->scr_y + 4 );
}

static void FixScrnCodeSetRect( TALK_WIN * tw, u8 x1, u8 y1, u8 x2, u8 y2 )
{
	u8 x,y;
	u16 pal;
	u16 base_code;
	u16 * base_scrn;

	pal = FontPalValue(tw);
	base_scrn = (u16 *)( tw->context->SCRadrs + 2 * ( y1 * SCRX_SZ + x1 ) );
	base_code = tw->cgx_n + FIXSCRNFONTUSE + x1 + y1 * VX_SIZE;

	for ( y=0 ; y < y2-y1 ; y++ ) {
		for ( x=0 ; x<x2-x1 ; x++ ) {
			base_scrn[ y * SCRX_SZ + x ] = ( base_code + x + VX_SIZE * y ) | pal;
		}
	}
}



//=================================================================
//
//�@��b�E�B���h�E:�l�r�f����v�����v�g����
//
//=================================================================

//-----------------------------------------------------------------
//	����}�[�N�A�j��
//-----------------------------------------------------------------
static void TalkMsgBlinkOn(TALK_WIN * tw)
{
	u8 pat;
	u32 cgx_adrs, dat_adrs;
	u16 chip;

	if (TalkMsgForceSkipCheck(tw) == TRUE)
		return;

	pat = ( tw->work >> 8 ) & 0x0f;
	dat_adrs = CHAR_SKIPCURSOR + pat * CHRSIZE * 2;

	switch ( tw->mode ) {

	case TRANSFONTMODE:
		cgx_adrs = LASTCGXADRS(tw);
		FontSpread2( dat_adrs, cgx_adrs, 
				tw->col_f, tw->col_s, tw->col_b );
		FontSpread2( dat_adrs + CHRSIZE, cgx_adrs + CHRSIZE, 
				tw->col_f, tw->col_s, tw->col_b );

		chip = LASTCGXNUMBER(tw);
		FontSCRSet(tw, chip, chip+1);
		break;

	case FIXFONTMODE:
		switch ( tw->font ) {

		case FONT8x16TYPE:
		case FONT8x16sTYPE:
			chip = tw->cgx_n + FIX_SKIPCURSOR * LFONT_CHR_USE;
			break;

		case FONT8x8RAWTYPE:
		case FONT8x8sRAWTYPE:
		case FONT8x8TYPE:
		case FONT8x8sTYPE:
		case FONTTENJITYPE:
		default:
			chip = tw->cgx_n + FIX_SKIPCURSOR;
			break;
		}
		cgx_adrs = tw->context->CGXadrs + chip * CHRSIZE;
		FontSpread2( dat_adrs, cgx_adrs, 
				tw->col_f, tw->col_s, tw->col_b );
		FontSpread2( dat_adrs + CHRSIZE, cgx_adrs + CHRSIZE, 
				tw->col_f, tw->col_s, tw->col_b );
		FontSCRSet(tw, chip, chip+1);
		break;

	case TRANSCGXMODE:
		/* ���肦�Ȃ� */
		break;
	
	case FIXSCRNMODE:
		cgx_adrs = FixScrn_CGXAdrs(tw);
		FontSpread2( dat_adrs, cgx_adrs, 
				tw->col_f, tw->col_s, tw->col_b );
		FontSpread2( dat_adrs + CHRSIZE, cgx_adrs + CHRSIZE * VX_SIZE, 
				tw->col_f, tw->col_s, tw->col_b );
		FixScrn_FontSCRSet(tw);
		break;
	}
}

//-----------------------------------------------------------------
//	����}�[�N����
//-----------------------------------------------------------------
static void TalkMsgBlinkOff(TALK_WIN * tw)
{
	if (TalkMsgForceSkipCheck(tw) == TRUE)
		return;

	switch ( tw->mode ) {

	case TRANSFONTMODE:
		TransFontPrintMain(tw,spc_);
		break;
	case FIXFONTMODE:
		fix_font_print_func[tw->font](tw,spc_);
		break;
	case TRANSCGXMODE:
		/* ���肦�Ȃ� */
		break;
	case FIXSCRNMODE:
		FixScrnPrintMain(tw,spc_);
		break;
	}
}


//-----------------------------------------------------------------
//	����}�[�N������
//-----------------------------------------------------------------
static void TalkMsgWaitInit(TALK_WIN * tw)
{
	tw->work = 0;
	TalkMsgBlinkOn(tw);
}

//-----------------------------------------------------------------
//	����}�[�N�I��
//-----------------------------------------------------------------
static void TalkMsgWaitEnd(TALK_WIN * tw)
{
	tw->work = 0;
	TalkMsgBlinkOff(tw);
}

//-----------------------------------------------------------------
//	����}�[�N����
//-----------------------------------------------------------------
static void TalkMsgWaitMain(TALK_WIN * tw)
{
	u8 wait,pat;

	wait = tw->work & 0x0f;
	pat = (tw->work >> 8 ) & 0x0f;

	wait ++;
	if ( wait == BLINK_WAIT ) {
		wait = 0;
		pat ++;
		if( pat >= BLINK_PAT ) pat = 0;
		tw->work = wait | (pat << 8 );
		TalkMsgBlinkOn(tw);
	}
	tw->work = wait | (pat << 8 );
}

//-----------------------------------------------------------------
//	�L�[�҂�������}�[�N����
//-----------------------------------------------------------------
static u8 TalkMsgWait( TALK_WIN * tw )
{
	if ( TalkMsgForceSkipCheck(tw) ) {
		tw->wait --;
		if ( tw->wait == 0 ) {
			TalkMsgWaitEnd(tw);
			return TRUE;
		} else {
			TalkMsgWaitMain(tw);
			return FALSE;
		}

	} else {

		if ( sys.Trg & AB_BUTTON ) {
			SePlay( SE_SELECT );
			TalkMsgWaitEnd(tw);
			return TRUE;
		} else {
			TalkMsgWaitMain(tw);
			return FALSE;
		}
	}
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=================================================================
//
//	�t�H���g�\���a�f��ł̏����E�h��Ԃ�����
//
//=================================================================
//-----------------------------------------------------------------
//	�w��ʒu�̃X�N���[���f�[�^���擾����
//	<����>
//	tw			�g�p���郁�b�Z�[�W�\����
//	x,y			���W
//	<�߂�l>
//	�w��ʒu�̃X�N���[���f�[�^�̒l
//-----------------------------------------------------------------
u16 FontScrnCodeGet( TALK_WIN * tw, u8 x, u8 y )
{
	u16 * pos = (u16 *)(tw->context->SCRadrs);
	return pos[x + y * SCRX_SZ];
}

//-----------------------------------------------------------------
//	��`�`��i�g�̂݁j
//<����>
//	code			�`��Ɏg�p����R�[�h�i���p���b�gNo���K�v�j
//	x1,y1,x2,y2		�̈�w��
//	
//-----------------------------------------------------------------
void FontScrnBoxLineDirect( TALK_WIN* tw, u16 code, u8 x1, u8 y1, u8 x2, u8 y2 )
{
	u8 x,y;
	u16 * pos = (u16 *)(tw->context->SCRadrs);

	for ( y=y1; y<=y2 ;y++ ) {
		if ( y == y1 || y == y2 ) {
			for( x=x1; x<=x2; x++) pos[x + y * SCRX_SZ] = code;
		} else {
			pos[x1 + y * SCRX_SZ] = code;
			pos[x2 + y * SCRX_SZ] = code;
		}
	}
}
//-----------------------------------------------------------------
//	��`�`��i�g�̂݁j
//<����>
//	code			�`��Ɏg�p����R�[�h
//	x1,y1,x2,y2		�̈�w��
//-----------------------------------------------------------------
void FontScrnBoxLine( TALK_WIN* tw, u16 code, u8 x1, u8 y1, u8 x2, u8 y2 )
{
	code |= PAL_VAL(tw);
	FontScrnBoxLineDirect(tw,code,x1,y1,x2,y2);
}

//-----------------------------------------------------------------
// �w�肵���X�N���[���G���A���w��R�[�h�œh��Ԃ�
//
//	<���̓p�����[�^>
//	tw			�g�p���郁�b�Z�[�W�\����
//	code		�h��Ԃ��X�N���[���R�[�h�i���p���b�gNo���K�v�j
//	x1,y1		������W
//	x2,y2		�E�����W
//-----------------------------------------------------------------
void FontScrnBoxFillDirect( TALK_WIN * tw, u16 code, u8 x1, u8 y1, u8 x2, u8 y2 )
{
	u8 x,y;
	u16 * pos = (u16 *)(tw->context->SCRadrs);

	for( x=x1; x<=x2; x++) {
		for ( y=y1; y<=y2 ;y++ ) {
			pos[x + y * SCRX_SZ] = code;
		}
	}
}

//-----------------------------------------------------------------
// FontScrnBoxFill
// �w�肵���X�N���[���G���A���w��R�[�h�œh��Ԃ�
//
//	<���̓p�����[�^>
//	tw			�g�p���郁�b�Z�[�W�\����
//	code		�h��Ԃ��X�N���[���R�[�h
//	x1,y1		������W
//	x2,y2		�E�����W
//-----------------------------------------------------------------
void FontScrnBoxFill( TALK_WIN * tw ,u16 code, u8 x1, u8 y1, u8 x2, u8 y2)
{
	code |= PAL_VAL(tw);
	FontScrnBoxFillDirect(tw,code,x1,y1,x2,y2);
}

//-----------------------------------------------------------------
//	�w��G���A�̃N���A
//-----------------------------------------------------------------
void FontScrnAreaClear(TALK_WIN * tw, u8 x1, u8 y1, u8 x2, u8 y2)
{
	FontScrnBoxFill(tw, CLR_CHR_NO, x1, y1, x2, y2);
}

//-----------------------------------------------------------------
// FontScrnEasyBoxFill
// �w��G���A�h��Ԃ��ȈՔ�
//
// �����[�h�ɂ�蓮�삪������
// �Œ�t�H���g���[�h:		�h��Ԃ��R�[�h�͋󔒃L�������g�p����
// �]���t�H���g���[�h:		�h��Ԃ��R�[�h�͎����I�ɍŏ��ɓo�^�����L�������g�p����
//
//	<���̓p�����[�^>
//	tw				�g�p���郁�b�Z�[�W�\����
//	x1,y1,x2,y2		��`�͈͎w��
//-----------------------------------------------------------------
void FontScrnEasyBoxFill( TALK_WIN * tw, u8 x1, u8 y1, u8 x2, u8 y2)
{
	u16 code;

	code = FontBlankCode(tw);
	FontScrnBoxFill(tw,code, x1, y1, x2, y2);
}

//-----------------------------------------------------------------
//	�󔒃R�[�h���擾����
// �����[�h�ɂ�蓮�삪������
// �Œ�t�H���g���[�h:		�o�^�ς݋󔒃L����
// �]���t�H���g���[�h:		�����I�ɍŏ��ɓo�^�����L����
//-----------------------------------------------------------------
static u16 FontBlankCode(TALK_WIN * tw)
{
	switch ( tw->mode ) {
	case TRANSFONTMODE:
			return tw->cgx_n;

	case FIXFONTMODE:
			switch( tw->font )
			{
			case FONT8x16TYPE:
			case FONT8x16sTYPE:
				return tw->cgx_n + spc_;
			case FONT8x8TYPE:
			case FONT8x8RAWTYPE:
			case FONT8x8sTYPE:
			case FONT8x8sRAWTYPE:
				return tw->cgx_n + spc_8x8;
			case FONTTENJITYPE:
				return tw->cgx_n + TENJI0000_;
			};
			break;

	case TRANSCGXMODE:
			return 0;

	case FIXSCRNMODE:
			return tw->cgx_n + 1;

	default:
			return 0;
	}
	return 0;		/*�O�̂���*/
}

//=================================================================
//=================================================================
