//==================================================================================
//
//
//	8�~8�Œ�t�H���g�ł̃E�B���h�E�E���b�Z�[�W�\�����C�u����
//
//
//==================================================================================
#include "common.h"
#include "print.h"
#include "mus_tool.h"

#define MENUTOOL_H_GLOBAL
#include "menutool.h"

#include "mapbg.h"
#include "window.h"
#include "field.h"
#include "fld_main.h"

TALK_WIN win[4] ={};
TALK_WIN * pWin = NULL;

#define FIX_WIN_START	(SYS_FONT_SIZ+NWIN_CHR_START_NO)
#define LINE_HEIGHT	2

/* �J�[�\������\���� */
typedef struct {
	u8 x,y;
	s8 Pos;
	s8 Start;
	s8 End;
	u8 xsize;				//nohara	02/01/18
	u8 ysize;				//nohara	02/01/18
	u8 StrWidth[7];			//nohara	02/01/18
}CURSOR_CTRL;

static u8 menu_init_seq = 0;

static CURSOR_CTRL	csr;

u16 MenuFontStartNo = 0;	// �����t�H���g�J�n�ʒu
u16 MenuNWinStartNo = 0;	// ���j���[�E�B���h�E�t�H���g�J�n�ʒu
u16 MenuKWinStartNo = 0;	// MSG�E�B���h�E�t�H���g�J�n�ʒu
u16 MenuChrEndNo = 0;		//�@�t�H���g�I���ʒu


//==================================================================================
//
//	�E�B���h�E�E���b�Z�[�W�`��֘A
//
//==================================================================================

//----------------------------------------------------------------------------------
//	����������
//	���t�H���g�A�E�B���h�E�L�����̃Z�b�g
//	��MSG�\���p�ϐ��̏�����
//----------------------------------------------------------------------------------
void NMenuScreenInit(CONTEXT_TYPE type)
{
	NMenuScreenInitEx( type, FIX_WIN_START );
};

void NMenuScreenInitStart(CONTEXT_TYPE type )
{
	NMenuScreenInitStartEx( type, FIX_WIN_START );
}

void NMenuScreenInitStartEx(CONTEXT_TYPE type, u16 chr_no)
{
	menu_init_seq = 0;
	MenuFontStartNo = chr_no;
	pWin = &win[0];
	MsgControlInit( pWin, type );
}

int NMenuScreenInitMain( void )
{
	switch ( menu_init_seq ) {
	case 0:
		menu_init_seq++;break;

	case 1:
		MenuNWinStartNo = PRT_SetSplitFontInit( pWin, MenuFontStartNo );
		menu_init_seq++;break;
	
	case 2:
		if ( PRT_SetSplitFontMain() == FALSE ) break;
		menu_init_seq++;break;

	case 3:
		MenuKWinStartNo = NWinFrameInit( MenuNWinStartNo );
		menu_init_seq++;break;
	
	case 4:
		NWinFrameDataSet( pWin );
		MenuChrEndNo = KWinFrameInit( MenuKWinStartNo );
		return TRUE;
	};
	return FALSE;
}

void NMenuScreenInitEx(CONTEXT_TYPE type, u16 chr_no)
{
	pWin = &win[0];
	MsgControlInit( pWin, type );

	MenuFontStartNo = chr_no;
	MenuNWinStartNo = PrintFontInit( pWin, MenuFontStartNo );
	MenuKWinStartNo = NWinFrameInit( MenuNWinStartNo );
	NWinFrameDataSet( pWin );
	MenuChrEndNo = KWinFrameInit( MenuKWinStartNo );
};

void NMenuScreenInitEx2( CONTEXT_TYPE type, u16 chr_no )
{
	pWin = &win[0];
	MsgControlInit( pWin, type );

	MenuNWinStartNo = chr_no;
	MenuKWinStartNo = NWinFrameInit( MenuNWinStartNo );
	NWinFrameDataSet( pWin );
	MenuFontStartNo = KWinFrameInit( MenuKWinStartNo );
	MenuChrEndNo = PrintFontInit( pWin, MenuFontStartNo );
}


void NMenuWindowCharSet( u8 type )
{
	NWinFrameDataCustomSet( pWin, type );
}

void NMenuWindowReset( void )
{
	NWinFrameDataSet( pWin );
}

//----------------------------------------------------------------------------------
//	�g�p�R���e�L�X�g�̐؂�ւ�
//	���t�H���g�L�����͎��O�ɌĂ΂ꂽNMenuScreenInit�ŃZ�b�g����Ă��邽��
//	�@�R���e�L�X�g�ł̎w�肪����Ă��L������J���[�͉�ʂɂ͔��f����Ȃ�
// <����>
//	type	�R���e�L�X�g�^�C�v�w��
//----------------------------------------------------------------------------------
void NMenuContextChange( CONTEXT_TYPE type )
{
	MsgControlInit( pWin, type );
};

//----------------------------------------------------------------------------------
//�@���b�Z�[�W�̕\��
// <����>
//	msg		���b�Z�[�W�f�[�^�ւ̃|�C���^
//	x,y		�\���ʒu�w��
//----------------------------------------------------------------------------------
void NMenuMsgWrite(const u8 * msg, u8 x, u8 y)
{
	MsgPrint(pWin,msg, MenuFontStartNo, x, y);
};

//----------------------------------------------------------------------------------
//�@��`�̈�̃N���A
// <����>
//	x1,y1,x2,y2		�̈�w��
//----------------------------------------------------------------------------------
void NMenuBoxClear(u8 x1, u8 y1, u8 x2, u8 y2)
{
	FontScrnAreaClear( pWin, x1, y1, x2, y2 );
};

//----------------------------------------------------------------------------------
//	��`�̈���󔒂ŕ`��
// <����>
//	x1,y1,x2,y2		�̈�w��
//----------------------------------------------------------------------------------
void NMenuBoxWrite(u8 x1, u8 y1, u8 x2, u8 y2)
{
	FontScrnEasyBoxFill(pWin,x1,y1,x2,y2);
};

//----------------------------------------------------------------------------------
//	�X�N���[���S�̂��N���A
//----------------------------------------------------------------------------------
void NMenuScreenClear(void)
{
	NMenuBoxClear( 0,0,29,19);
};

//----------------------------------------------------------------------------------
//	��`�̈�ɃE�B���h�E�`��
// <����>
//	x1,y1,x2,y2		�̈�w��
//----------------------------------------------------------------------------------
void NMenuWinBoxWrite(u8 x1, u8 y1, u8 x2, u8 y2)
{
	NWinFrameWrite(pWin,x1,y1,x2,y2);
};

//----------------------------------------------------------------------------------
//	��ʉ����ɃE�B���h�E�ň͂܂ꂽ���b�Z�[�W��\��
// <����>
//	msg		���b�Z�[�W�f�[�^�ւ̃|�C���^
//----------------------------------------------------------------------------------
void NMenuWinMsgWrite(const u8 * msg)
{
	NMenuWinBoxWrite(2,14,28,19);
	NMenuMsgWrite(msg,3,15);
};

//----------------------------------------------------------------------------------
//�@�ꕶ���\��
// <����>
//	code		�\�����镶���R�[�h
//	x,y		�\���ʒu�w��
//----------------------------------------------------------------------------------
void NMenuPutc( u8 code, u8 x, u8 y )
{
	MsgPutc( pWin, code, MenuFontStartNo, x, y);
}

//----------------------------------------------------------------------------------
//	�X�N���[���R�[�h�擾
// <����>
//	x,y		�擾�ʒu�w��
//<�߂�l>
//			�w��ʒu�̃X�N���[���R�[�h
//----------------------------------------------------------------------------------
u16 NMenuGetCode( u8 x, u8 y )
{
	return FontScrnCodeGet(pWin,x,y);
}

//----------------------------------------------------------------------------------
//	��`�`��i�g�̂݁j
//<����>
//	code			�`��Ɏg�p����R�[�h
//	x1,y1,x2,y2		�̈�w��
//----------------------------------------------------------------------------------
void NMenuBoxLineDirect( u16 code, u8 x1, u8 y1, u8 x2, u8 y2 )
{
	FontScrnBoxLineDirect( pWin, code, x1,y1,x2,y2);
}

//==================================================================================
//
//	���胁�b�Z�[�W�֘A
//
//==================================================================================
//----------------------------------------------------------------------------------
//	��b�E�B���h�E�\��
//----------------------------------------------------------------------------------
void NMenuTalkWinWrite( void )
{
	KWinFrameWrite( pWin );
};

//----------------------------------------------------------------------------------
//	���胁�b�Z�[�W�\���Z�b�g
//	<����>
//	msg		���b�Z�[�W�f�[�^�ւ̃|�C���^
//	x,y		�\���ʒu�w��
//----------------------------------------------------------------------------------
void NMenuTalkMsgSet(const u8 * msg, u8 x, u8 y)
{
	TalkMsgSet( pWin, msg, MenuFontStartNo, x, y);
};

//----------------------------------------------------------------------------------
//	�W���ʒu�ւ̑��胁�b�Z�[�W�\���Z�b�g
//	<����>
//	msg		���b�Z�[�W�f�[�^�ւ̃|�C���^
//----------------------------------------------------------------------------------
void NMenuTalkMsgSetEasy( const u8 * msg )
{
	TalkMsgSet( pWin, msg, MenuFontStartNo, MWIN_MSGXPOS, MWIN_MSGYPOS );
};

//----------------------------------------------------------------------------------
//	�������b�Z�[�W�\���Z�b�g(2002/4/30 TETSU)
//	<����>
//	msg		���b�Z�[�W�f�[�^�ւ̃|�C���^
//----------------------------------------------------------------------------------
void NMenuTalkMsgSetContinue( const u8 * msg )
{
	//�Ƃ肠��������
	ContinueMsgSet( pWin, msg );
};

//----------------------------------------------------------------------------------
//	���胁�b�Z�[�W�\��
//	<�߂�l>
//	0			�\����
//	1			�w�蕶����̕\���I��
//----------------------------------------------------------------------------------
u8 NMenuTalkMsgPut( void )
{
	return TalkMsgPut( pWin );
};

//----------------------------------------------------------------------------------
//	���b�Z�[�W�\��(�ꕶ�����ƂɕԂ�j
//	��TalkMsg�n�̐���R�[�h���قƂ�ǎg�p�ł��Ȃ��̂Œ���
//	<�߂�l>
//	0			�\����
//	1			�w�蕶����̕\���I��
//----------------------------------------------------------------------------------
u8 NMenuTalkMsgPutc( void )
{
	return TalkMsgPutc( pWin );
}

//----------------------------------------------------------------------------------
//	�E�B���h�E���b�Z�[�W�̃N���A
//----------------------------------------------------------------------------------
void NMenuTalkMsgClr( void )
{
	TalkMsgClr( pWin );
}


//==================================================================================
//
//	�J�[�\���I���֘A
//
//==================================================================================

//----------------------------------------------------------------------------------
//	�c�����J�[�\���I���̏�����
// <����>
//	x,y		�J�[�\����{�ʒu
//	Max		�J�[�\���I������
//	Pos		�J�[�\�������I���ʒu
// <�߂�l>
//	�J�[�\�������I���ʒu�i�C����j
//----------------------------------------------------------------------------------
u8 NMenuVCursorInit(u8 x, u8 y, u8 Max, u8 Pos)
{
	csr.x = x;
	csr.y = y;
	csr.Start = 0;
	csr.End = Max - 1;
	if(Pos < csr.Start || Pos > csr.End) Pos=csr.Start;
	csr.Pos = Pos;
	NMenuVCursorSet(0);
	return Pos;
};

//----------------------------------------------------------------------------------
//	�c�����J�[�\���̈ړ� & �Z�b�g
// <����>
// mv		�J�[�\���ړ��l
// <�߂�l>
//	�J�[�\���I���ʒu
//----------------------------------------------------------------------------------
//	�c�����J�[�\���̈ړ� & �Z�b�g(���[�v����)
u8 NMenuVCursorSet(s8 mv)
{
	u8	code[]={cursor_,EOM_};

	NMenuBoxWrite( csr.x, csr.y+(csr.Pos * LINE_HEIGHT),
			csr.x, csr.y+1+(csr.Pos * LINE_HEIGHT) );
		
	if( csr.Pos+mv < csr.Start )
		csr.Pos=csr.End;

	else if( csr.Pos+mv > csr.End )
		csr.Pos=csr.Start;

	else
		csr.Pos += mv;

	NMenuMsgWrite(code, csr.x, csr.y+(csr.Pos * LINE_HEIGHT) );

	return csr.Pos;
};

//	�c�����J�[�\���̈ړ� & �Z�b�g(���[�v�Ȃ�)
u8 NMenuVCursorSetNoloop(s8 mv)
{
	u8	code[]={cursor_,EOM_};

	NMenuBoxWrite( csr.x, csr.y+(csr.Pos * LINE_HEIGHT),
			csr.x, csr.y+1+(csr.Pos * LINE_HEIGHT) );
		
	if( csr.Pos+mv < csr.Start )
		csr.Pos=csr.Start;

	else if( csr.Pos+mv > csr.End )
		csr.Pos=csr.End;

	else
		csr.Pos += mv;

	NMenuMsgWrite(code, csr.x, csr.y+(csr.Pos * LINE_HEIGHT) );

	return csr.Pos;
};

//----------------------------------------------------------------------------------
//	�c�����J�[�\���̑I���ʒu���擾
// <�߂�l>
//	�J�[�\���I���ʒu
//----------------------------------------------------------------------------------
u8 NMenuVCursorPosGet(void)
{
	return csr.Pos;
};

//----------------------------------------------------------------------------------
//	�ȒP�ȏc�����J�[�\���̐���
//	��L�[(��ړ��j�A���L�[�i���ړ��j�A�`�{�^���i����j�A�a�{�^���i�L�����Z���j
//
// <�߂�l>
//	CURSOR_CANCEL_VALUE		�L�����Z�����ꂽ
//	CURSOR_DEFAULT_VALUE	�܂��I������Ă��Ȃ�
//	0 �`					���肳�ꂽ�I���ʒu
//----------------------------------------------------------------------------------
//	�ȒP�ȏc�����J�[�\���̐���(���[�v����)
s8 NMenuVCursorControl(void)
{
	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		return csr.Pos;
	};
	if( sys.Trg & B_BUTTON ){
		return CURSOR_CANCEL_VALUE;
	};
	if( sys.Trg & U_KEY ){
		SePlay(SE_SELECT);
		NMenuVCursorSet(-1);
		return CURSOR_DEFAULT_VALUE;
	};
	if( sys.Trg & D_KEY ){
		SePlay(SE_SELECT);
		NMenuVCursorSet(1);
		return CURSOR_DEFAULT_VALUE;
	};

	return CURSOR_DEFAULT_VALUE;
};

//	�ȒP�ȏc�����J�[�\���̐���(���[�v�Ȃ�)
s8 NMenuVCursorControlNoloop(void)
{
	u8	pos_bak;

	pos_bak = csr.Pos;

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		return csr.Pos;
	};
	if( sys.Trg & B_BUTTON ){
		return CURSOR_CANCEL_VALUE;
	};
	if( sys.Trg & U_KEY ){
		if(pos_bak != NMenuVCursorSetNoloop(-1))	SePlay(SE_SELECT);
		return CURSOR_DEFAULT_VALUE;
	};
	if( sys.Trg & D_KEY ){
		if(pos_bak != NMenuVCursorSetNoloop(1))		SePlay(SE_SELECT);
		return CURSOR_DEFAULT_VALUE;
	};
	return CURSOR_DEFAULT_VALUE;
};

//u8 xsize;
//u8 ysize;
//----------------------------------------------------------------------------------
//	�c�������J�[�\���̈ړ� & �Z�b�g			2001/08/28 by nakahiro
// <����>
// m		�J�[�\���ړ��l
// <�߂�l>
//	�J�[�\���I���ʒu
//----------------------------------------------------------------------------------
//
//
u8 NMenuHVCursorSet( s8 m )
{
	u8	code[] = { cursor_,EOM_ };
	u8	v_max;

/*	NMenuBoxWrite( csr.x+( (csr.Pos>>1) * 6), csr.y+( (csr.Pos % 2) * LINE_HEIGHT),
		csr.x+( (csr.Pos>>1) * 6), csr.y+1+( (csr.Pos % 2) * LINE_HEIGHT) );
*/
	v_max = ( csr.End + 1 ) >> 1;

	NMenuBoxWrite( csr.x+csr.Pos/v_max*6, csr.y+csr.Pos%v_max*LINE_HEIGHT,
		csr.x+csr.Pos/v_max*6, csr.y+1+csr.Pos%v_max*LINE_HEIGHT );

	if( csr.Pos+m < csr.Start )
		csr.Pos=csr.End;

	else if( csr.Pos+m > csr.End )
		csr.Pos=csr.Start;

	else
		csr.Pos += m;

//	NMenuMsgWrite(code, csr.x+( (csr.Pos>>1) * 6), csr.y+( (csr.Pos % 2) * LINE_HEIGHT) );
	NMenuMsgWrite(code, csr.x+csr.Pos/v_max*6, csr.y+csr.Pos%v_max*LINE_HEIGHT );

	return csr.Pos;
};

/********************************************/
//�v���g�^�C�v�錾
/********************************************/
u8 NMenuHVCursorSet2(s8 m);
u8 NMenuHVCursorKeyChk();
s8 YesNoHVSelectMain(void);
void NMenuHVWinSet(u8 x, u8 y, u8 Max, const MENU_ITEM * menu, u8 arrange);
u8 NMenuHVCursorInit(u8 x, u8 y, u8 Max, u8 Pos, u8 arrange);
void NMenuHVStrSet(u8 x, u8 y, u8 Max, const MENU_ITEM * menu, u8 arrange);
void HVSelectInit(u8 x, u8 y, u8 Max, u8 Pos, const MENU_ITEM * menu, u8 arrange);

//----------------------------------------------------------------------------------
//	�c�������J�[�\���̈ړ� & �Z�b�g			02/01/17 nohara
// <����>
// m		�J�[�\���ړ��l
// <�߂�l>
//	�J�[�\���I���ʒu
//----------------------------------------------------------------------------------
u8 NMenuHVCursorSet2(s8 m)
{
	u8	code[]={cursor_,EOM_};

	//�J�[�\������
	if( (csr.Pos+m) <= csr.End )
	{
		if( NMenuHVCursorKeyChk() == 1 )	return csr.Pos;
	}else
	{
		return csr.Pos;
	}

	//�ړ��O�̃J�[�\���ʒu�N���A
	if( (csr.End+1) / csr.xsize == 0 )
	{
		NMenuBoxWrite( csr.x+csr.StrWidth[csr.Pos % csr.xsize], 
			csr.y+( ( (csr.Pos / csr.xsize) % csr.ysize) * LINE_HEIGHT),
			csr.x+csr.StrWidth[csr.Pos % csr.xsize],
			csr.y+1+( ( (csr.Pos / csr.xsize) % csr.ysize) * LINE_HEIGHT) );
	}else
	{
		NMenuBoxWrite( csr.x+csr.StrWidth[csr.Pos % csr.xsize], 
			csr.y+( ( (csr.Pos / csr.xsize) % csr.ysize) * LINE_HEIGHT),
			csr.x+csr.StrWidth[csr.Pos % csr.xsize],
			csr.y+1+( (csr.Pos / csr.xsize) * LINE_HEIGHT) );
	}

	//�J�[�\���ړ�
	csr.Pos += m;

	//�J�[�\���\��
	if( (csr.End+1) / csr.xsize == 0 )
	{
		NMenuMsgWrite(code, csr.x+csr.StrWidth[csr.Pos % csr.xsize],
			csr.y+( ( (csr.Pos / csr.xsize   ) % csr.ysize) * LINE_HEIGHT) );
	}else
	{
		NMenuMsgWrite(code, csr.x+csr.StrWidth[csr.Pos % csr.xsize],
			csr.y+( (csr.Pos / csr.xsize   ) * LINE_HEIGHT) );
	}

	return csr.Pos;
};

//----------------------------------------------------------------------------------
//	�c���J�[�\������					02/01/17 nohara
// <�߂�l>
//	0=�ړ��ł���
//	1=�ړ��ł��Ȃ�
//----------------------------------------------------------------------------------
u8 NMenuHVCursorKeyChk()
{
	if( sys.Trg & U_KEY ){
		if( csr.Pos < csr.xsize ) return 1;
	}
	if( sys.Trg & D_KEY ){
		if( csr.Pos >= (csr.End+1 - csr.xsize) ) return 1;
	}
	if( sys.Trg & L_KEY ){
		if( ( (csr.Pos-(csr.Pos % csr.xsize)) % csr.xsize) == 1 || csr.Pos == 0 
				|| (csr.Pos % csr.xsize == 0) ) return 1;
	}
	if( sys.Trg & R_KEY ){
		if( (csr.Pos % csr.xsize) == (csr.xsize - 1) ) return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------------
//	�c�������J�[�\���I���̏�����		02/01/17 nohara
// <����>
//	x,y		�J�[�\����{�ʒu
//	Max		�J�[�\���I������
//	Pos		�J�[�\�������I���ʒu
//	arrange	���ڂ̕��ו�
// <�߂�l>
//	�J�[�\�������I���ʒu�i�C����j
//----------------------------------------------------------------------------------
u8 NMenuHVCursorInit(u8 x, u8 y, u8 Max, u8 Pos, u8 arrange)
{
	//�J�[�\���\���̂ɃZ�b�g
	NMenuVCursorInit(x,y,Max,Pos);
	csr.xsize = arrange;
	csr.ysize = (Max / arrange);
	return Pos;
}

//----------------------------------------------------------------------------------
//�c���E�B���h�E�\��(�E�B���h�E�̂�)	02/01/17	nohara
// <����>
//	x,y		����̍��W
//	Max		�J�[�\���I������
//	menu	������
//	arrange	���ڂ̕��ו�
//----------------------------------------------------------------------------------
void NMenuHVWinSet(u8 x, u8 y, u8 Max, const MENU_ITEM * menu, u8 arrange)
{
	u8 i,j,y2;
	u16 kazu=0;


	//������
	for( i=0; i < 7; i++ )
		csr.StrWidth[i] = 0;


	//�ő啶�����擾
	for( i=0; i < arrange ; i++ )
	{
		for( j=0; (i+j) < Max ; j+=arrange )
		{
			kazu=PM_strlen( menu[i+j].msg );
			if( csr.StrWidth[i+1] < kazu ) 
				csr.StrWidth[i+1]=kazu;
		}
	}


	//�v�Z�����Ȃ��悤�ɕ��𑫂��Ă���
	for( i=1; i < arrange+1 ; i++ )
		csr.StrWidth[i] += csr.StrWidth[i-1]+2;

	csr.StrWidth[arrange] -= 2;	//����

	if(  ( (Max >> 1) >= arrange && (Max & 0x01) == 0 ) 
			|| arrange == 1 || arrange == Max )
	{
		y2=y+( (Max / arrange)*2+1 );
	}else
	{
		y2=y+( (Max / arrange + 1)*2+1 );	//�]�肪�o����P�i����
	}

	//�E�B���h�E�\��
	NMenuWinBoxWrite( x, y, x+csr.StrWidth[arrange]+1+1, y2 );

	return;
}

//----------------------------------------------------------------------------------
//�c���E�B���h�E�\��(������̂�)			02/01/17	nohara
// <����>
//	x,y		����̍��W
//	Max		�J�[�\���I������
//	menu	������
//	arrange	���ڂ̕��ו�
//----------------------------------------------------------------------------------
void NMenuHVStrSet(u8 x, u8 y, u8 Max, const MENU_ITEM * menu, u8 arrange)
{
	u8 i,j,k;
	u16 kazu=0;


	//������
	for( i=0; i < 7; i++ )
		csr.StrWidth[i] = 0;


	//�ő啶�����擾
	for( i=0; i < arrange ; i++ )
	{
		for( j=0; (i+j) < Max ; j+=arrange )
		{
			kazu=PM_strlen( menu[i+j].msg );
			if( csr.StrWidth[i+1] < kazu ) 
				csr.StrWidth[i+1]=kazu;
		}
	}


	//�v�Z�����Ȃ��悤�ɕ��𑫂��Ă���
	for( i=1; i < arrange+1 ; i++ )
		csr.StrWidth[i] += csr.StrWidth[i-1]+2;

	csr.StrWidth[arrange] -= 2;	//����

	//������\��
	for( i=0; i < arrange ; i++ )
	{
		for( k=0, j=0; (i+j) < Max ; j+=arrange, k++ )
		{
			NMenuMsgWrite( menu[i+j].msg,
				x+csr.StrWidth[i % arrange],y+(k*2));
		}
	}

	return;
}

//----------------------------------------------------------------------------------
//�c���E�B���h�E�Z�b�g(�E�B���h�E�\���A�J�[�\���������A������\��)	02/01/17 nohara
// <����>
//	x,y		����̍��W
//	Max		�J�[�\���I������
//	Pos		�J�[�\�������I���ʒu
//	menu	������
//	arrange	���ڂ̕��ו�
//----------------------------------------------------------------------------------
void HVSelectInit(u8 x, u8 y, u8 Max, u8 Pos, const MENU_ITEM * menu, u8 arrange)
{
	NMenuHVWinSet(x,y,Max,menu,arrange);
	NMenuHVCursorInit(x+1,y+1,Max,Pos,arrange);
	NMenuHVStrSet(x+2,y+1,Max,menu,arrange);
}

//----------------------------------------------------------------------------------
//	�c���I���E�B���h�E�̐���			02/01/17 nohara
// <�߂�l>
//	CURSOR_CANCEL_VALUE		�L�����Z�����ꂽ
//	CURSOR_DEFAULT_VALUE	�܂��I������Ă��Ȃ�
//	0 �`					���肳�ꂽ�I���ʒu
//----------------------------------------------------------------------------------
s8 HVSelectMain(void)
{
	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		return NMenuVCursorPosGet();
	};
	if( sys.Trg & B_BUTTON ){
		return CURSOR_CANCEL_VALUE;
	};
	if( sys.Trg & U_KEY ){
		SePlay(SE_SELECT);
		NMenuHVCursorSet2(-csr.xsize);
		return CURSOR_DEFAULT_VALUE;
	};
	if( sys.Trg & D_KEY ){
		SePlay(SE_SELECT);
		NMenuHVCursorSet2(csr.xsize);
		return CURSOR_DEFAULT_VALUE;
	};
	if( sys.Trg & L_KEY ){
		SePlay(SE_SELECT);
		NMenuHVCursorSet2(-1);
		return CURSOR_DEFAULT_VALUE;
	};
	if( sys.Trg & R_KEY ){
		SePlay(SE_SELECT);
		NMenuHVCursorSet2(1);
		return CURSOR_DEFAULT_VALUE;
	};

	return CURSOR_DEFAULT_VALUE;
};

//----------------------------------------------------------------------------------
//	�w�肵���v�f���̕��������擾		02/01/17 nohara
// <�߂�l>
//	������
//----------------------------------------------------------------------------------
u8 NMenuStrWidthGet(u8 no)
{
	return csr.StrWidth[no];
}

//==================================================================================
//
//	���j���[�����֘A
//
//==================================================================================

//----------------------------------------------------------------------------------
//	�Œ胊�X�g�̕\��
// <����>
//	x,y		�\����{�ʒu�w��
//	Max		�\�����ڐ�
//	menu	���ڃf�[�^�ւ̃|�C���^
//----------------------------------------------------------------------------------
void NMenuFixListWrite(u8 x, u8 y, u8 Max, const MENU_ITEM * menu)
{
	u8	i;
	for( i=0;i<Max;i++){
		NMenuMsgWrite( menu[i].msg, x, y+(i*LINE_HEIGHT) );
	}
};

//----------------------------------------------------------------------------------
//	���X�g���獀�ڂ��擾���ĕ\������
// <����>
//	x,y		�\����{�ʒu�w��
//	Max		�\�����ڐ�
//	menu	���ڃf�[�^�ւ̃|�C���^
//	List	�\�����鍀�ڃi���o�[���X�g
//----------------------------------------------------------------------------------
void NMenuFreeListWrite(u8 x, u8 y, u8 Max, const MENU_ITEM * menu, const u8 * List)
{
	u8	i;
	for( i=0;i<Max;i++){
		NMenuMsgWrite( ( menu[List[i]] ).msg, x, y+(i*LINE_HEIGHT) );
	}
};


//==================================================================================
//
//		�͂�/������ �I���E�B���h�E�֌W
//
//==================================================================================

// �͂��E�������E�B���h�E�p�f�[�^

static const u8 yes_msg[] = {ha_,i_,EOM_};
static const u8 no_msg[] = {i_,i_,e_,EOM_};

const MENU_ITEM	yesno_msg[]=
{
	{ yes_msg,	NULL },	//00:�͂�
	{ no_msg,	NULL }	//01:������
};

#define YESNO_SELECT_MAX	NELEMS(yesno_msg)


//----------------------------------------------------------------------------------
//	�͂��E�������I�𕶎��Z�b�g
// <����>
//	x,y		�\���ʒu�w��
//----------------------------------------------------------------------------------
void YesNoSelectStrInit(u8 x, u8 y)
{
	NMenuFixListWrite(x+YESNOMSG_OFFX,y+YESNOMSG_OFFY,YESNO_SELECT_MAX,yesno_msg);
	NMenuVCursorInit(x+YESNOCSR_OFFX,y+YESNOCSR_OFFY,YESNO_SELECT_MAX,0);
}

//----------------------------------------------------------------------------------
//	�͂��E�������I���E�B���h�E�Z�b�g
// <����>
//	x,y		�\���ʒu�w��
//----------------------------------------------------------------------------------
void YesNoSelectInit(u8	x, u8 y)
{
	NMenuWinBoxWrite( x, y, x+YESNOWIN_OFFX, y+YESNOWIN_OFFY );
	YesNoSelectStrInit(x,y);
}

//----------------------------------------------------------------------------------
//	�͂��E�������I���E�B���h�E�̐���
// <�߂�l>
//	CURSOR_CANCEL_VALUE		�L�����Z�����ꂽ
//	CURSOR_DEFAULT_VALUE	�܂��I������Ă��Ȃ�
//	CURSOR_YES_VALUE		�͂���I��
//	CURSOR_NO_VALUE			��������I��
//----------------------------------------------------------------------------------
s8 YesNoSelectMain(void)
{
	return NMenuVCursorControlNoloop();
}










