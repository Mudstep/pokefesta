//========================================================================================
//									�����̃p�\�R������
//								�@2002/06/25 by nakahiro
//========================================================================================
#include "common.h"
#include "menu.h"
#include "task.h"
#include "script.h"
#include "actanm.h"
#include "palanm.h"
#include "mus_tool.h"
#include "item.h"
#include "pocketno.h"
#include "poketool.h"
#include "pokelist.h"
#include "gds_main.h"
#include "goods.h"
#include "itemtool.h"
#include "fld_main.h"
#include "my_pc.h"
#include "f_pmlist.h"
#include "message.h"
#include "weather.h"
#include "fld_task.h"


//========================================================================================
//										�V���{��
//========================================================================================
#define	MY_ROOM_MENU_MAX	4			// �����̕�������ڑ������Ƃ��̃��j���[��
#define	GLOBAL_PC_MENU_MAX	3			// �����p�[������ڑ������Ƃ��̃��j���[��

#define	MENU_WINX	0					// ���j���[���W
#define	MENU_WINY	0
#define	MENU_WINEX	( MENU_WINX + 10 )
#define	MENU_WINEY	( MENU_WINY + 9 )

#define	ITEM_MENU_WINX		0			//�u�ǂ����@��������v���j���[���W
#define	ITEM_MENU_WINY		0
#define	ITEM_MENU_WINEX		( ITEM_MENU_WINX + 11 )
#define	ITEM_MENU_WINEY		( ITEM_MENU_WINY + 9 )

#define	ITEM_WINX		14				// ����X�g���W
#define	ITEM_WINY		0
#define	ITEM_WINEX		( ITEM_WINX + 15 )
#define	ITEM_WINEY		( ITEM_WINY + 19 )

#define	ITEM_INFO_WINX		0			// �������
#define	ITEM_INFO_WINY		12
#define	ITEM_INFO_WINEX		( ITEM_INFO_WINX + 13 )
#define	ITEM_INFO_WINEY		( ITEM_INFO_WINY + 7 )

#define	ITEM_TYPE_WINX		0			// ���j���[����
#define	ITEM_TYPE_WINY		0
#define	ITEM_TYPE_WINEX		( ITEM_TYPE_WINX + 10 )
#define	ITEM_TYPE_WINEY		( ITEM_TYPE_WINY + 3 )

#define	ITEM_NUM_WINX		6			// ���I��
#define	ITEM_NUM_WINY		8
#define	ITEM_NUM_WINEX		( ITEM_NUM_WINX + 7 )
#define	ITEM_NUM_WINEY		( ITEM_NUM_WINY + 3 )

#define	ITEM_YESNO_WINX		7			// �͂� / ������
#define	ITEM_YESNO_WINY		6
#define	ITEM_YESNO_WINEX	( ITEM_YESNO_WINX + 6 )
#define	ITEM_YESNO_WINEY	( ITEM_YESNO_WINY + 5 )

#define	ITEM_SCR_CUR_X		( ( ITEM_WINX + ( ITEM_WINEX-ITEM_WINX ) / 2 ) * 8 + 8 )
#define	ITEM_SCR_CUR_UY		( ITEM_WINY * 8 + 8 )
#define	ITEM_SCR_CUR_DY		( ITEM_WINEY * 8 )

#define	DUMMY_CURSOR_PAL	13

#define	MAIL_MENU_WINX		0			//�u���[���{�b�N�X�v���j���[���W
#define	MAIL_MENU_WINY		0
#define	MAIL_MENU_WINEX		( MAIL_MENU_WINX + 12 )
#define	MAIL_MENU_WINEY		( MAIL_MENU_WINY + 9 )

#define	MAIL_BOX_WINX		0			// ���j���[����
#define	MAIL_BOX_WINY		0
#define	MAIL_BOX_WINEX		( MAIL_BOX_WINX + 8 )
#define	MAIL_BOX_WINEY		( MAIL_BOX_WINY + 3 )

#define	MAIL_LIST_WINX		20			// ���o�l���X�g
#define	MAIL_LIST_WINY		0
#define	MAIL_LIST_WINEX		( MAIL_LIST_WINX + 9 )
#define	MAIL_LIST_WINEY		( MAIL_LIST_WINY + 19 )

#define	MAIL_SCR_CUR_X		( (MAIL_LIST_WINX+(MAIL_LIST_WINEX-MAIL_LIST_WINX)/2)*8+8 )
#define	MAIL_SCR_CUR_UY		( MAIL_LIST_WINY * 8 + 8 )
#define	MAIL_SCR_CUR_DY		( MAIL_LIST_WINEY * 8 )

#define	MAIL_YESNO_WINX		20
#define	MAIL_YESNO_WINY		8
#define	MAIL_YESNO_WINEX	( MAIL_YESNO_WINX + 6 )
#define	MAIL_YESNO_WINEY	( MAIL_YESNO_WINY + 5 )



// �^�X�N���[�N
#define	WK_ITEM_POS		0
#define	WK_ITEM_SCR		1
#define	WK_ITEM_MAX		2
#define	WK_ITEM_NO		3
#define	WK_ITEM_POSMAX	4
#define	WK_ITEM_GET		5
#define	WK_ITEM_TYPE	6
#define	WK_ITEM_CNGPOS	8
#define	WK_ITEM_CNGFLG	9


// ���ԍ�
#define	MSG_MAENIMODORU		0xffff	// �܂��ɂ��ǂ�
#define	MSG_IKUTU_HIKIDASU	0xfffe	// �����@�Ђ������H
#define	MSG_HIKIDASITA		0xfffd	// �Ђ������܂���
#define	MSG_IKUTU_SUTERU	0xfffc	// �����@���Ă�H
#define	MSG_SUTETA			0xfffb	// ���Ă܂���
#define	MSG_HIKIDASENAI		0xfffa	// �Ђ������Ȃ�
#define	MSG_SUTERARENAI		0xfff9	// ���Ă��Ȃ�
#define	MSG_SUTEMASUKA		0xfff8	// ���Ă܂����H
#define	MSG_IREKAE			0xfff7	// ���ꂩ����H

//========================================================================================
//									�v���g�^�C�v�錾
//========================================================================================
extern void MyPC_BagInit(void);				// �o�b�O�Ăяo��
extern void Draw_X_Num( u16 no, NUM_MODE_TYPE type, u8 x, u8 y, u8 keta );
extern void DrawNumbersItemName( u16 no, const u8 * name, u8 x, u8 y );
extern void MailViewInit( MAIL_DATA *, pFunc, u8 );
extern void FieldBagPokeListInit(void);

static void MyPC_MenuInit( u8 id );			// ���j���[�����ݒ�
static void MyPC_MenuMain( u8 id );			// ���j���[�I��

static void MyPC_ItemMenuInit( u8 id );		// �ǂ����@��������
static void MyPC_MailBoxInit( u8 id );		// ���[���{�b�N�X
static void MyPC_RearrangeInit( u8 id );	// ���悤����
static void MyPC_End( u8 id );				// ���������@����

// �ǂ����@��������
static void MyPC_ItemMenuPut( u8 no );				// ���j���[�\��
static void MyPC_ItemMenuMsgPut( const u8 * str );	// ���j���[���\��
static void MyPC_ItemMenuMain( u8 id );				// ���j���[�I��
static void MyPC_ItemPut( u8 id );					// �ǂ������@��������
static void MyPC_ItemGet( u8 id );					// �ǂ������@�Ђ�����
static void MyPC_ItemDel( u8 id );					// �ǂ������@���Ă�
static void MyPC_ItemEnd( u8 id );					// ���ǂ�

// �ǂ����@�Ђ����� / ���Ă�
static void MyPC_ItemListPut( u8 id );
static void MyPC_ItemInfoPut( u16 item );
static void MyPC_ItemListReWrite( u8 id );
static void MyPC_ItemListWinPut( u8 id, u8 type );
static void MyPC_ItemSelectMain( u8 id );			// ����I��
static void MyPC_ItemSelectEnd( u8 id );			// ����I���I���@����j���[�ɖ߂�
static void MyPC_ItemPosMaxGet( u8 id );			// �ő區�ڐ��擾
static void MyPC_SetItemNoInit( u8 id );			// ������I��
static void MyPC_SetItemNoMain( u8 id );
static void MyPC_ItemSelectReturn( u8 id );			// ����I���ɖ߂�
static void MyPC_ItemGetCheck( u8 id );				// �����o���邩
static void MyPC_ItemDelCheck( u8 id );				// �̂Ă��邩
static void MyPC_ItemDelYes( u8 id );				// �̂Ă�H�@�͂�
static void MyPC_ItemDelNo( u8 id );				// �̂Ă�H�@������
static void MyPC_ItemSubEnd( u8 id );				// ����
static void MyPC_ItemSubEnd2( u8 id );
static void MyPC_ItemChange( u8 id, u8 flg );		// ����ւ���
static void CC_PaletteMake(void);					// �p���b�g�쐬

// ���[���{�b�N�X
static u8	MyPC_MailBoxMaxCheck(void);			// ���[���̐����`�F�b�N
static void MyPC_MailBoxSortZERO(void);			// ���[���{�b�N�X�̃X�y�[�X���l�߂�
static void MyPC_MailListPut( u8 id );			// ���o�l���X�g�\��
static void MyPC_MailListWinPut( u8 id );
static void MyPC_MailBoxEnd( u8 id );			// ���[���{�b�N�X�I��
static void MyPC_MailListMain( u8 id );			// ���[�����X�g�I��
static void MyPC_MailMenuMsgSet( u8 id );		// ���[�����X�g���I�����ꂽ
static void MyPC_MailBoxDispClear(void);
static void MyPC_MailMenuInit( u8 id );			// ���j���[�\��
static void MyPC_MailMenuMain( u8 id );			// ���j���[�I��
static void MyPC_MailRead( u8 id );				// �Ȃ��悤���@���
static void MyPC_MailReadFadeOut( u8 id );		// �t�F�[�h�A�E�g�҂�
static void MyPC_MailReadReturn(void);			// ���[����ʂ���̖߂��
static void MyPC_MailBagSet( u8 id );			// �o�b�O�Ɂ@���ǂ�
static void MyPC_MailBagSetYesNoInit( u8 id );	// �͂��E�������@�Z�b�g
static void MyPC_MailDelYes( u8 id );			// �߂��H�@�͂�
static void MyPC_MailDelNo( u8 id );			// �߂��H�@������
static void MyPC_MailPokeSet( u8 id );			// �|�P�����Ɂ@��������
static void MyPC_PokeMineZero( u8 id );			// �|�P���������Ȃ�
static void MyPC_MailEnd( u8 id );				// ��߂�
static void MyPC_MailEndMain( u8 id );


//========================================================================================
//									�O���[�o���ϐ�
//========================================================================================
extern u8	UseFldPokeFlg;

static const u8 * const msg_item_menu[] = {
	msg_my_pc_item01,
	msg_my_pc_item00,
	msg_my_pc_item02,
	msg_menu_return,
};

static const MENU_ITEM pc_menu[] =
{
	{ str_my_pc_item,  MyPC_ItemMenuInit },		// 00:�ǂ����@��������
	{ str_my_pc_mail,  MyPC_MailBoxInit },		// 01:���[���{�b�N�X
	{ str_my_pc_rearg, MyPC_RearrangeInit },	// 02:���悤����
	{ str_pc_kiru,     MyPC_End },				// 03:���������@����
};

static const u8 my_room_menu[]   = { 0, 1, 2, 3 };
static const u8 global_pc_menu[] = { 0, 1, 3 };

static u8 * menu_list = NULL;
static u8 menu_max;

static const MENU_ITEM item_menu[] =
{
	{ str_my_pc_hikidasu, MyPC_ItemGet },		// 00:�ǂ������@�Ђ�����
	{ str_my_pc_azukeru,  MyPC_ItemPut },		// 01:�ǂ������@��������
	{ str_my_pc_suteru,   MyPC_ItemDel },		// 02:�ǂ������@���Ă�
	{ str_modoru,         MyPC_ItemEnd },		// 03:���ǂ�
};

static const SELECT_ITEM	ItemDelYesNoFunc = { MyPC_ItemDelYes, MyPC_ItemDelNo };
static const SELECT_ITEM	MailDelYesNoFunc = { MyPC_MailDelYes, MyPC_MailDelNo };

// PC���̃f�t�H���g�A�C�e��
static const u16 def_pc_item[][2] =
{
	{ ITEM_KIZUGUSURI, 1 },
	{ 0, 0 },
};

static const MENU_ITEM mail_menu[] =
{
	{ str_my_pc_yomu,     MyPC_MailRead },		// 00:�Ȃ��悤���@���
	{ str_my_pc_modosu,   MyPC_MailBagSet },	// 01:�o�b�O�Ɂ@���ǂ�
	{ str_my_pc_motaseru, MyPC_MailPokeSet },	// 02:�|�P�����Ɂ@��������
	{ str_yameru,		   MyPC_MailEnd },		// 03:��߂�
};



//-----------------------------------------
//	�f�t�H���g�A�C�e���Z�b�g
//-----------------------------------------
void MyPC_DefItemSet(void)
{
	u8	i = 0;

	ItemDataInit( Fld.MyPCBoxItem, MY_PCBOX_ITEM_MAX );

	while(1){
		if( def_pc_item[i][0] == 0 || def_pc_item[i][1] == 0 )
			break;
		if( AddPCBoxItem( def_pc_item[i][0], def_pc_item[i][1] ) != TRUE )
			break;
		i++;
	}
}

//-----------------------------------------
//	�����̕�������Ăяo��
//-----------------------------------------
void MyPC_MyRoomCall(void)
{
	menu_list = (u8 *)my_room_menu;
	menu_max  = MY_ROOM_MENU_MAX;
	SetTalkMsg( AddTask( DummyTaskSeq, 0 ), msg_menu_nanisuru, MyPC_MenuInit, 0 );
}

//-----------------------------------------
//	�����[������Ăяo��
//-----------------------------------------
void MyPC_GlobalPCCall(void)
{
	menu_list = (u8 *)global_pc_menu;
	menu_max  = GLOBAL_PC_MENU_MAX;
	SetTalkMsg( AddTask( DummyTaskSeq, 0 ), msg_menu_nanisuru, MyPC_MenuInit, 0 );
}

//-----------------------------------------
//	���j���[�����ݒ�
//-----------------------------------------
static void MyPC_MenuInit( u8 id )
{
	NMenuWinBoxWrite( MENU_WINX, MENU_WINY, MENU_WINEX, MENU_WINY+1+(menu_max << 1) );
	NMenuFreeListWrite( MENU_WINX+2, MENU_WINY+1, menu_max, pc_menu, menu_list );
	NMenuVCursorInit( MENU_WINX+1, MENU_WINY+1, menu_max, 0 );
	TaskTable[id].TaskAdrs = MyPC_MenuMain;
}

//-----------------------------------------
//	���j���[�I��
//-----------------------------------------
static void MyPC_MenuMain( u8 id )
{
	MENUFUNC	func;

	if( sys.Repeat & U_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet(-1);
		return;
	}

	if( sys.Repeat & D_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet(1);
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		func = pc_menu[ menu_list[ NMenuVCursorPosGet() ] ].ptr;
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		func = pc_menu[ menu_list[ menu_max-1 ] ].ptr;
		func(id);
	}
}

//-----------------------------------------
//	���j���[�I���֖߂�
//-----------------------------------------
void MyPC_ReturnMenu( u8 id )
{
	SetTalkMsg( id, msg_menu_nanisuru, MyPC_MenuInit, 0 );
}


//-----------------------------------------
//	�ǂ����@��������
//-----------------------------------------
static void MyPC_ItemMenuInit( u8 id )
{
	MyPC_ItemMenuPut(0);
	TaskTable[id].TaskAdrs = MyPC_ItemMenuMain;
}

//-----------------------------------------
//	���[���{�b�N�X
//-----------------------------------------
static void MyPC_MailBoxInit( u8 id )
{
	NMenuBoxClear( MENU_WINX, MENU_WINY, MENU_WINEX, MENU_WINEY );

	MYPC_SYS->max = MyPC_MailBoxMaxCheck();	// ���[���̐����`�F�b�N
	if( MYPC_SYS->max == 0 ){
		SetTalkMsg( id, msg_my_pc_no_mail, MyPC_ReturnMenu, 0 );
	}else{
		MYPC_SYS->pos = 0;
		MYPC_SYS->scr = 0;
		MyPC_MailBoxSortZERO();
		MyPC_ItemPosMaxGet(id);
		MyPC_MailListWinPut(id);
		TaskTable[id].TaskAdrs = MyPC_MailListMain;
	}
}

//-----------------------------------------
//	���悤����
//-----------------------------------------
static void MyPC_RearrangeInit( u8 id )
{
	NMenuBoxClear( MENU_WINX, MENU_WINY, MENU_WINEX, MENU_WINEY );
	MyRoomRearrangeInit(id);
}

//-----------------------------------------
//	���������@����
//-----------------------------------------
extern const VM_CODE my_sex_boy_11_t101_r0102[];
extern const VM_CODE my_sex_girl_11_t101_r0202[];

static void MyPC_End( u8 id )
{
	if( menu_max == MY_ROOM_MENU_MAX ){
		NMenuBoxClear( 0, 0, 29, 19 );
		if( MyData.my_sex == MALE )	StartFieldScript( my_sex_boy_11_t101_r0102 );
		else						StartFieldScript( my_sex_girl_11_t101_r0202 );
	}else{
		NMenuBoxClear( MENU_WINX, MENU_WINY, MENU_WINEX, MENU_WINEY );
		ContinueFieldScript();
	}
	DelTask(id);
}



//========================================================================================
//									�ǂ����@��������
//========================================================================================
//-----------------------------------------
//	���j���[�\��
//-----------------------------------------
static void MyPC_ItemMenuPut( u8 no )
{
	NMenuBoxClear( MENU_WINX, MENU_WINY, MENU_WINEX, MENU_WINEY );
	NMenuWinBoxWrite( ITEM_MENU_WINX, ITEM_MENU_WINY, ITEM_MENU_WINEX, ITEM_MENU_WINEY );
	NMenuFixListWrite( ITEM_MENU_WINX+2, ITEM_MENU_WINY+1, 4, item_menu );
	NMenuVCursorInit( ITEM_MENU_WINX+1, ITEM_MENU_WINY+1, 4, no );
	MyPC_ItemMenuMsgPut( msg_item_menu[ no ] );
}

//-----------------------------------------
//	���j���[���\��
//-----------------------------------------
static void MyPC_ItemMenuMsgPut( const u8 * str )
{
	NMenuBoxWrite( 4, 15, 25, 18 );
	NMenuMsgWrite( str, 4, 15 );
}

//-----------------------------------------
//	���j���[�I��
//-----------------------------------------
static void MyPC_ItemMenuMain( u8 id )
{
	MENUFUNC	func;

	if( sys.Repeat & U_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet(-1);
		MyPC_ItemMenuMsgPut( msg_item_menu[ NMenuVCursorPosGet() ] );
		return;
	}

	if( sys.Repeat & D_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet(1);
		MyPC_ItemMenuMsgPut( msg_item_menu[ NMenuVCursorPosGet() ] );
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		func = item_menu[ NMenuVCursorPosGet() ].ptr;
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		func = item_menu[3].ptr;
		func(id);
	}
}

//-----------------------------------------
//	�ǂ������@��������
//-----------------------------------------
static void MyPC_ItemPutFade( u8 id )
{
	if( !( FadeData.fade_sw ) ){
		MyPC_BagInit();	// �o�b�O�Ăяo��
		DelTask(id);
	}
}
static void MyPC_ItemPut( u8 id )
{
	TaskTable[id].TaskAdrs = MyPC_ItemPutFade;
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
//	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}

// �o�b�O���畜�A
static void MyPC_ItemPutReturnTask( u8 id )
{
//	if( !( FadeData.fade_sw ) )
	if( FieldFadeinCheck() == 1 )
		TaskTable[id].TaskAdrs = MyPC_ItemMenuMain;
}
void MyPC_ItemPutReturn(void)
{
	NMenuTalkWinWrite();
	MyPC_ItemMenuPut(1);
	AddTask( MyPC_ItemPutReturnTask, 0 );
	FieldFadeBlackInSet();
//	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
}

//-----------------------------------------
//	�ǂ������@�Ђ�����
//-----------------------------------------
static void MyPC_ItemGet( u8 id )
{
	s16 * wk = TaskTable[id].work;

	NMenuBoxClear( ITEM_MENU_WINX, ITEM_MENU_WINY, ITEM_MENU_WINEX, ITEM_MENU_WINEY );

	wk[WK_ITEM_MAX] = (s16)CheckPCBoxItem();

	if( wk[WK_ITEM_MAX] != 0 ){
		NMenuBoxClear( 0, 14, 29, 19 );
		wk[WK_ITEM_TYPE] = 0;
		wk[WK_ITEM_POS]  = 0;
		wk[WK_ITEM_SCR]  = 0;
		MyPC_ItemPosMaxGet(id);
		MyPC_ItemListWinPut( id, 0 );
		TaskTable[id].TaskAdrs = MyPC_ItemSelectMain;
	}else{
		SetTalkMsg( id, msg_my_pc_no_item, MyPC_ItemMenuInit, 0 );
	}
}

//-----------------------------------------
//	�ǂ������@���Ă�
//-----------------------------------------
static void MyPC_ItemDel( u8 id )
{
	s16 * wk = TaskTable[id].work;

	NMenuBoxClear( ITEM_MENU_WINX, ITEM_MENU_WINY, ITEM_MENU_WINEX, ITEM_MENU_WINEY );

	wk[WK_ITEM_MAX] = (s16)CheckPCBoxItem();

	if( wk[WK_ITEM_MAX] != 0 ){
		NMenuBoxClear( 0, 14, 29, 19 );
		wk[WK_ITEM_TYPE] = 2;
		wk[WK_ITEM_POS]  = 0;
		wk[WK_ITEM_SCR]  = 0;
		MyPC_ItemPosMaxGet(id);
		MyPC_ItemListWinPut( id, 2 );
		TaskTable[id].TaskAdrs = MyPC_ItemSelectMain;
	}else{
		SetTalkMsg( id, msg_my_pc_no_item, MyPC_ItemMenuInit, 0 );
	}
}

//-----------------------------------------
//	���ǂ�
//-----------------------------------------
static void MyPC_ItemEnd( u8 id )
{
	NMenuBoxClear( ITEM_MENU_WINX, ITEM_MENU_WINY, ITEM_MENU_WINEX, ITEM_MENU_WINEY );
	MyPC_ReturnMenu(id);
}

//-----------------------------------------
//	�ő區�ڐ��擾
//-----------------------------------------
static void MyPC_ItemPosMaxGet( u8 id )
{
	s16 * wk = TaskTable[id].work;

	if( wk[WK_ITEM_MAX] >= 8 )	wk[WK_ITEM_POSMAX] = 8;
	else						wk[WK_ITEM_POSMAX] = wk[WK_ITEM_MAX]+1;

	if( MYPC_SYS->max >= 8 )	MYPC_SYS->pos_max = 8;
	else						MYPC_SYS->pos_max = MYPC_SYS->max+1;
}

//-----------------------------------------
//	����I���@�Ђ����� / ���Ă�
//-----------------------------------------
static void MyPC_ItemSelectMain( u8 id )
{
	s16 * wk  = TaskTable[id].work;
	s16   pos = 0;

	if( sys.Repeat & U_KEY ){
		if( wk[WK_ITEM_POS] != 0 ){
			SePlay( SE_SELECT );
			wk[WK_ITEM_POS] = (s16)NMenuVCursorSet(-1);
			pos = wk[WK_ITEM_SCR] + wk[WK_ITEM_POS];

			if( wk[WK_ITEM_CNGFLG] == 0 ){
				if( pos == wk[WK_ITEM_MAX] )	MyPC_ItemInfoPut( MSG_MAENIMODORU );
				else	MyPC_ItemInfoPut( Fld.MyPCBoxItem[pos].item );
			}

			if( wk[WK_ITEM_CNGFLG] != 0 &&
				wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] >= 0 &&
				wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] < 8 &&
				wk[WK_ITEM_CNGPOS] != pos )
			{
				pos = ( wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] ) << 1;
				PalChangeCursorPut( ITEM_WINX+1, ITEM_WINY+2+pos, DUMMY_CURSOR_PAL );
			}

		}else{
			if( wk[WK_ITEM_SCR] != 0 ){
				SePlay( SE_SELECT );
				wk[WK_ITEM_SCR]--;
				MyPC_ItemListReWrite(id);
				if( wk[WK_ITEM_CNGFLG] != 0 )	NMenuVCursorSet(0);
			}
		}
		return;
	}

	if( sys.Repeat & D_KEY ){
		if( wk[WK_ITEM_POS] != wk[WK_ITEM_POSMAX]-1 ){
			SePlay( SE_SELECT );
			wk[WK_ITEM_POS] = (s16)NMenuVCursorSet(1);
			pos = wk[WK_ITEM_SCR] + wk[WK_ITEM_POS];

			if( wk[WK_ITEM_CNGFLG] == 0 ){
				if( pos == wk[WK_ITEM_MAX] )	MyPC_ItemInfoPut( MSG_MAENIMODORU );
				else	MyPC_ItemInfoPut( Fld.MyPCBoxItem[pos].item );
			}

			if( wk[WK_ITEM_CNGFLG] != 0 &&
				wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] >= 0 &&
				wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] < 8 &&
				wk[WK_ITEM_CNGPOS] != pos )
			{
				pos = ( wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] ) << 1;
				PalChangeCursorPut( ITEM_WINX+1, ITEM_WINY+2+pos, DUMMY_CURSOR_PAL );
			}

		}else{
			if( wk[WK_ITEM_SCR]+wk[WK_ITEM_POS] != wk[WK_ITEM_MAX] ){
				SePlay( SE_SELECT );
				wk[WK_ITEM_SCR]++;
				MyPC_ItemListReWrite(id);
				if( wk[WK_ITEM_CNGFLG] != 0 )	NMenuVCursorSet(0);
			}
		}
		return;
	}

	if( sys.Trg & SELECT_BUTTON ){
		if( wk[WK_ITEM_CNGFLG] == 0 ){
			if( wk[WK_ITEM_POS]+wk[WK_ITEM_SCR] != wk[WK_ITEM_MAX] ){
				SePlay( SE_SELECT );
				wk[WK_ITEM_CNGFLG] = 1;
				wk[WK_ITEM_CNGPOS] = wk[WK_ITEM_SCR] + wk[WK_ITEM_POS];
				MyPC_ItemInfoPut( MSG_IREKAE );
			}
		}else{
			SePlay( SE_SELECT );
			MyPC_ItemChange( id, 0 );
		}
		return;
	}


	if( sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		if( wk[WK_ITEM_CNGFLG] == 0 ){
			if( wk[WK_ITEM_SCR] + wk[WK_ITEM_POS] == wk[WK_ITEM_MAX] )
				MyPC_ItemSelectEnd(id);
			else	MyPC_SetItemNoInit(id);
		}else{
			MyPC_ItemChange( id, 0 );
		}
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		if( wk[WK_ITEM_CNGFLG] == 0 )
			MyPC_ItemSelectEnd(id);
		else{
			MyPC_ItemChange( id, 1 );
		}
	}
}

//-----------------------------------------
//	����I���I���@����j���[�ɖ߂�
//-----------------------------------------
static void MyPC_ItemSelectEnd( u8 id )
{
	DelMarkPal();
	DelScrCurActor( MARK_ACT_U_CURSOR );
	DelScrCurActor( MARK_ACT_D_CURSOR );
	NMenuBoxClear( 0, 0, 29, 19 );
	NMenuTalkWinWrite();
	MyPC_ItemMenuPut( (u8)TaskTable[id].work[WK_ITEM_TYPE] );
	TaskTable[id].TaskAdrs = MyPC_ItemMenuMain;
}

//-----------------------------------------
//	������I���@�Ђ����� / ���Ă�
//-----------------------------------------
static void MyPC_SetItemNoInit( u8 id )
{
	s16 * wk = TaskTable[id].work;
	u8    pos = (u8)( wk[WK_ITEM_POS] + wk[WK_ITEM_SCR] );

	ScrCurActAnmStopInit( MARK_ACT_U_CURSOR );
	ScrCurActAnmStopInit( MARK_ACT_D_CURSOR );

	if( wk[WK_ITEM_TYPE] == 0 ){
		if( Fld.MyPCBoxItem[pos].no == 1 ){
			wk[WK_ITEM_NO] = 1;
			MyPC_ItemGetCheck(id);
			return;
		}
		MyPC_ItemInfoPut( MSG_IKUTU_HIKIDASU );
	}else{
		if( Fld.MyPCBoxItem[pos].no == 1 ){
			wk[WK_ITEM_NO] = 1;
			MyPC_ItemDelCheck(id);
			return;
		}
		MyPC_ItemInfoPut( MSG_IKUTU_SUTERU );
	}

	wk[WK_ITEM_NO] = 1;
	NMenuWinBoxWrite( ITEM_NUM_WINX, ITEM_NUM_WINY, ITEM_NUM_WINEX, ITEM_NUM_WINEY );
	Draw_X_Num( wk[WK_ITEM_NO], NUM_MODE_ZERO, ITEM_NUM_WINX+2, ITEM_NUM_WINY+1, 3 );

	TaskTable[id].TaskAdrs = MyPC_SetItemNoMain;
}

static void MyPC_SetItemNoMain( u8 id )
{
	s16 * wk  = TaskTable[id].work;
	u8    pos = (u8)( wk[WK_ITEM_POS] + wk[WK_ITEM_SCR] );

	if( sys.Repeat & U_KEY ){
		if( wk[WK_ITEM_NO] != Fld.MyPCBoxItem[pos].no )
			wk[WK_ITEM_NO]++;
		else	wk[WK_ITEM_NO] = 1;
		Draw_X_Num( wk[WK_ITEM_NO], NUM_MODE_ZERO, ITEM_NUM_WINX+2, ITEM_NUM_WINY+1, 3 );
		return;
	}
	if( sys.Repeat & D_KEY ){
		if( wk[WK_ITEM_NO] != 1 )	wk[WK_ITEM_NO]--;
		else	wk[WK_ITEM_NO] = Fld.MyPCBoxItem[pos].no;
		Draw_X_Num( wk[WK_ITEM_NO], NUM_MODE_ZERO, ITEM_NUM_WINX+2, ITEM_NUM_WINY+1, 3 );
		return;
	}
	if( sys.Repeat & L_KEY ){
		wk[WK_ITEM_NO] -= 10;
		if( wk[WK_ITEM_NO] < 1 )	wk[WK_ITEM_NO] = 1;
		Draw_X_Num( wk[WK_ITEM_NO], NUM_MODE_ZERO, ITEM_NUM_WINX+2, ITEM_NUM_WINY+1, 3 );
		return;
	}
	if( sys.Repeat & R_KEY ){
		wk[WK_ITEM_NO] += 10;
		if( wk[WK_ITEM_NO] > Fld.MyPCBoxItem[pos].no )
			wk[WK_ITEM_NO] = Fld.MyPCBoxItem[pos].no;
		Draw_X_Num( wk[WK_ITEM_NO], NUM_MODE_ZERO, ITEM_NUM_WINX+2, ITEM_NUM_WINY+1, 3 );
		return;
	}


	if( sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		NMenuBoxClear( ITEM_NUM_WINX, ITEM_YESNO_WINY, ITEM_NUM_WINEX, ITEM_YESNO_WINEY );
		if( wk[WK_ITEM_TYPE] == 0 )	MyPC_ItemGetCheck(id);
		else						MyPC_ItemDelCheck(id);
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		NMenuBoxClear( ITEM_NUM_WINX, ITEM_YESNO_WINY, ITEM_NUM_WINEX, ITEM_YESNO_WINEY );
		ScrCurActAnmPlay( MARK_ACT_U_CURSOR );
		ScrCurActAnmPlay( MARK_ACT_D_CURSOR );
		MyPC_ItemInfoPut( Fld.MyPCBoxItem[wk[WK_ITEM_SCR]+wk[WK_ITEM_POS]].item );
		TaskTable[id].TaskAdrs = MyPC_ItemSelectMain;
	}
}

//-----------------------------------------
//	�����o���邩
//-----------------------------------------
static void MyPC_ItemGetCheck( u8 id )
{
	s16 * wk  = TaskTable[id].work;
	u8    pos = (u8)( wk[WK_ITEM_POS] + wk[WK_ITEM_SCR] );

	if( AddItem( Fld.MyPCBoxItem[pos].item, wk[WK_ITEM_NO] ) == TRUE ){
		GetItemName( Fld.MyPCBoxItem[pos].item, StrTempBuffer0 );
		PM_NumMsgSet( StrTempBuffer1, wk[WK_ITEM_NO], NUM_MODE_LEFT, 3 );
		MyPC_ItemInfoPut( MSG_HIKIDASITA );
		TaskTable[id].TaskAdrs = MyPC_ItemSubEnd;
	}else{
		wk[WK_ITEM_NO] = 0;
		MyPC_ItemInfoPut( MSG_HIKIDASENAI );
		TaskTable[id].TaskAdrs = MyPC_ItemSubEnd2;
	}
}

//-----------------------------------------
//	�̂Ă��邩
//-----------------------------------------
static void MyPC_ItemDelCheck( u8 id )
{
	s16 * wk  = TaskTable[id].work;
	u8    pos = (u8)( wk[WK_ITEM_POS] + wk[WK_ITEM_SCR] );

	if( ItemEventGet( Fld.MyPCBoxItem[pos].item ) == 0 ){
		GetItemName( Fld.MyPCBoxItem[pos].item, StrTempBuffer0 );
		PM_NumMsgSet( StrTempBuffer1, wk[WK_ITEM_NO], NUM_MODE_LEFT, 3 );
		MyPC_ItemInfoPut( MSG_SUTEMASUKA );
		YesNoSelectInit( ITEM_YESNO_WINX, ITEM_YESNO_WINY );
		SelectToolInit( id, &ItemDelYesNoFunc );
	}else{
		wk[WK_ITEM_NO] = 0;
		MyPC_ItemInfoPut( MSG_SUTERARENAI );
		TaskTable[id].TaskAdrs = MyPC_ItemSubEnd;
	}
}

// �̂Ă�H�@�͂�
static void MyPC_ItemDelYes( u8 id )
{
	NMenuBoxClear( ITEM_NUM_WINX, ITEM_YESNO_WINY, ITEM_NUM_WINEX, ITEM_YESNO_WINEY );
	MyPC_ItemInfoPut( MSG_SUTETA );
	TaskTable[id].TaskAdrs = MyPC_ItemSubEnd;
}

// �̂Ă�H�@������
static void MyPC_ItemDelNo( u8 id )
{
	s16 * wk  = TaskTable[id].work;

	NMenuBoxClear( ITEM_NUM_WINX, ITEM_YESNO_WINY, ITEM_NUM_WINEX, ITEM_YESNO_WINEY );
	NMenuVCursorInit( ITEM_WINX+1, ITEM_WINY+2, wk[WK_ITEM_POSMAX], wk[WK_ITEM_POS] );
	ScrCurActAnmPlay( MARK_ACT_U_CURSOR );
	ScrCurActAnmPlay( MARK_ACT_D_CURSOR );
	MyPC_ItemInfoPut( Fld.MyPCBoxItem[wk[WK_ITEM_SCR]+wk[WK_ITEM_POS]].item );
	TaskTable[id].TaskAdrs = MyPC_ItemSelectMain;
}


//-----------------------------------------
//	����
//-----------------------------------------
static void MyPC_ItemSubEnd( u8 id )
{
	s16 * wk  = TaskTable[id].work;
	s16   max = 0;

	if( sys.Trg & A_BUTTON || sys.Trg == B_BUTTON ){
		SubPCBoxItem( (u8)( wk[WK_ITEM_POS]+wk[WK_ITEM_SCR] ), wk[WK_ITEM_NO] );
		max = wk[WK_ITEM_MAX];
		wk[WK_ITEM_MAX] = (s16)CheckPCBoxItem();
		if( max != wk[WK_ITEM_MAX] ){
			if( max < wk[WK_ITEM_POSMAX]+wk[WK_ITEM_SCR] && wk[WK_ITEM_SCR] != 0 )
				wk[WK_ITEM_SCR]--;
		}
		MyPC_ItemPosMaxGet(id);
		MyPC_ItemSelectReturn(id);
		NMenuVCursorInit( ITEM_WINX+1, ITEM_WINY+2, wk[WK_ITEM_POSMAX], wk[WK_ITEM_POS] );
	}
}

// �����o���Ȃ�
static void MyPC_ItemSubEnd2( u8 id )
{
	s16 * wk  = TaskTable[id].work;

	if( sys.Trg & A_BUTTON || sys.Trg == B_BUTTON ){
		MyPC_ItemInfoPut( Fld.MyPCBoxItem[  wk[WK_ITEM_SCR] + wk[WK_ITEM_POS] ].item );
		ScrCurActAnmPlay( MARK_ACT_U_CURSOR );
		ScrCurActAnmPlay( MARK_ACT_D_CURSOR );
		TaskTable[id].TaskAdrs = MyPC_ItemSelectMain;
	}
}

//-----------------------------------------
//	����I���ɖ߂�
//-----------------------------------------
static void MyPC_ItemSelectReturn( u8 id )
{
	NMenuBoxClear( ITEM_NUM_WINX, ITEM_YESNO_WINY, ITEM_NUM_WINEX, ITEM_YESNO_WINEY );
	ScrCurActAnmPlay( MARK_ACT_U_CURSOR );
	ScrCurActAnmPlay( MARK_ACT_D_CURSOR );
	MyPC_ItemListReWrite(id);
	TaskTable[id].TaskAdrs = MyPC_ItemSelectMain;
}

//-----------------------------------------
//	����ւ���
//-----------------------------------------
static void MyPC_ItemChange( u8 id, u8 flg )
{
	s16 * wk  = TaskTable[id].work;
	u8	  pos = (u8)(wk[WK_ITEM_SCR] + wk[WK_ITEM_POS]);

	MINEITEM	copy;

	wk[WK_ITEM_CNGFLG] = 0;

	if( (u8)wk[WK_ITEM_MAX] > pos &&
		(u8)wk[WK_ITEM_CNGPOS] != pos &&
		flg == 0 )
	{
		copy = Fld.MyPCBoxItem[ wk[WK_ITEM_CNGPOS] ];
		Fld.MyPCBoxItem[ wk[WK_ITEM_CNGPOS] ] = Fld.MyPCBoxItem[ pos ];
		Fld.MyPCBoxItem[ pos ] = copy;
		MyPC_ItemListReWrite(id);
	}else{
		if( pos == wk[WK_ITEM_MAX] )	MyPC_ItemInfoPut( MSG_MAENIMODORU );
		else	MyPC_ItemInfoPut( Fld.MyPCBoxItem[ pos ].item );

		if( wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] >= 0 &&
			wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] < 8 &&
			wk[WK_ITEM_CNGPOS] != pos )
		{
			pos = ( wk[WK_ITEM_CNGPOS] - wk[WK_ITEM_SCR] ) << 1;
			NMenuBoxWrite( ITEM_WINX+1, ITEM_WINY+2+pos, ITEM_WINX+1, ITEM_WINY+3+pos );
		}
	}
}


//-----------------------------------------
//	����X�g�\��
//-----------------------------------------
// �ǂ��� / �{�[�� / �؂̎��@���
static void MyPC_ItemListNormal( MINEITEM * data, u8 y )
{
	GetItemName( data->item, StrTempBuffer0 );
	NMenuMsgWrite( StrTempBuffer0, ITEM_WINX+2, y );
	Draw_X_Num( data->no, NUM_MODE_SPACE, ITEM_WINX+11, y, 3 );
}

// �������Ȃ��́@���
static void MyPC_ItemListEvent( MINEITEM * data, u8 y )
{
	NMenuMsgWrite( ItemNameGet( data->item ), ITEM_WINX+2, y );
}

// �킴�}�V���@���
static void MyPC_ItemListWaza( MINEITEM * data, u8 y )
{
	NMenuMsgWrite( ItemNameGet( data->item ), ITEM_WINX+2, y );
	if( data->item <= HIDEN_START_NO )
		Draw_X_Num( data->no, NUM_MODE_SPACE, ITEM_WINX+11, y, 3 );
}

// ����\��
static void MyPC_ItemListPut( u8 id )
{
	u16   i, y = 0;
	s16 * wk = TaskTable[id].work;

	for( i=wk[WK_ITEM_SCR]; i<wk[WK_ITEM_SCR]+wk[WK_ITEM_POSMAX]; i++ ){
		y = ( i - wk[WK_ITEM_SCR] ) << 1;

		if( ( i - wk[WK_ITEM_SCR] ) != wk[WK_ITEM_POS] )
			NMenuBoxWrite( ITEM_WINX+1, ITEM_WINY+2+y, ITEM_WINEX-1, ITEM_WINY+3+y );
		else	NMenuBoxWrite( ITEM_WINX+2, ITEM_WINY+2+y, ITEM_WINEX-1, ITEM_WINY+3+y );

		if( i == wk[WK_ITEM_MAX] ){
			NMenuMsgWrite( str_yameru, ITEM_WINX+2, ITEM_WINY+2+y );
			break;
		}

		if( wk[WK_ITEM_CNGFLG] != 0 && i == wk[WK_ITEM_CNGPOS] )
			PalChangeCursorPut( ITEM_WINX+1, ITEM_WINY+2+y, DUMMY_CURSOR_PAL );


		switch( GetPocketNo( Fld.MyPCBoxItem[i].item ) ){
		case NORMAL_POCKET:
		case BALL_POCKET:
		case SEED_POCKET:
			MyPC_ItemListNormal( &Fld.MyPCBoxItem[i], ITEM_WINY+2+y );
			break;
		case IMPORTANT_POCKET:
			MyPC_ItemListEvent( &Fld.MyPCBoxItem[i], ITEM_WINY+2+y );
			break;
		case SKILL_POCKET:
			MyPC_ItemListWaza( &Fld.MyPCBoxItem[i], ITEM_WINY+2+y );
		}
	}

	if( i - wk[WK_ITEM_SCR] != 8 ){
		NMenuBoxWrite( ITEM_WINX+2, ITEM_WINY+4+y, ITEM_WINEX-1, ITEM_WINEY-1 );
	}

	if( wk[WK_ITEM_SCR] != 0 ){
		AddScrCurActor( MARK_ACT_U_CURSOR, ITEM_SCR_CUR_X, ITEM_SCR_CUR_UY );
	}else
		DelScrCurActor( MARK_ACT_U_CURSOR );

	if( wk[WK_ITEM_SCR] + wk[WK_ITEM_POSMAX] <= wk[WK_ITEM_MAX] ){
		AddScrCurActor( MARK_ACT_D_CURSOR, ITEM_SCR_CUR_X, ITEM_SCR_CUR_DY );
	}else
		DelScrCurActor( MARK_ACT_D_CURSOR );
}

// ���\��
static void MyPC_ItemInfoPut( u16 item )
{
	NMenuBoxWrite( ITEM_INFO_WINX+1, ITEM_INFO_WINY+1, ITEM_INFO_WINEX-1, ITEM_INFO_WINEY-1 );

	switch( item ){
	case MSG_MAENIMODORU:		// �܂��ɂ��ǂ�
		NMenuMsgWrite( msg_menu_return, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;
	case MSG_IKUTU_HIKIDASU:	// �����@�Ђ������H
		NMenuMsgWrite( msg_my_pc_get01, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;
	case MSG_HIKIDASITA:		// �Ђ������܂���
		NMenuMsgWrite( msg_my_pc_get02, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;
	case MSG_IKUTU_SUTERU:		// �����@���Ă�H
		NMenuMsgWrite( msg_item_del01, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;
	case MSG_SUTETA:			// ���Ă܂���
		NMenuMsgWrite( msg_item_del02, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;
	case MSG_HIKIDASENAI:		// �Ђ������Ȃ�
		NMenuMsgWrite( msg_my_pc_no_get, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;
	case MSG_SUTERARENAI:		// ���Ă��Ȃ�
		NMenuMsgWrite( msg_item_no_del, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;
	case MSG_SUTEMASUKA:		// ���Ă܂����H
		NMenuMsgWrite( msg_item_del03, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;
	case MSG_IREKAE:			// �ǂ�Ɓ@���ꂩ���܂����H
		NMenuMsgWrite( msg_item_move, ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
		break;

	default:	// ������
		NMenuMsgWrite( ItemMessageGet(item), ITEM_INFO_WINX+1, ITEM_INFO_WINY+1 );
	}
}

// ����E���@��������
static void MyPC_ItemListReWrite( u8 id )
{
	s16 * wk  = TaskTable[id].work;
	s16   pos = wk[WK_ITEM_SCR] + wk[WK_ITEM_POS];

	MyPC_ItemListPut(id);

	if( wk[WK_ITEM_CNGFLG] == 0 ){
		if( pos == wk[WK_ITEM_MAX] )	MyPC_ItemInfoPut( MSG_MAENIMODORU );
		else	MyPC_ItemInfoPut( Fld.MyPCBoxItem[ pos ].item );
	}
}

// �I����ʕ\��
static void MyPC_ItemListWinPut( u8 id, u8 type )
{
	s16 * wk = TaskTable[id].work;

	ScrActInit();
	AddMarkPal();
	CC_PaletteMake();
	NMenuWinBoxWrite( ITEM_WINX, ITEM_WINY, ITEM_WINEX, ITEM_WINEY );
	NMenuWinBoxWrite( ITEM_INFO_WINX, ITEM_INFO_WINY, ITEM_INFO_WINEX, ITEM_INFO_WINEY );
	NMenuWinBoxWrite( ITEM_TYPE_WINX, ITEM_TYPE_WINY, ITEM_TYPE_WINEX, ITEM_TYPE_WINEY );
	MyPC_ItemInfoPut( Fld.MyPCBoxItem[0].item );
	NMenuMsgWrite( item_menu[type].msg, ITEM_TYPE_WINX+1, ITEM_TYPE_WINY+1 );
	MyPC_ItemListPut(id);
	NMenuVCursorInit( ITEM_WINX+1, ITEM_WINY+2, wk[WK_ITEM_POSMAX], wk[WK_ITEM_POS] );
}

//-----------------------------------------
//	�p���b�g�쐬
//-----------------------------------------
static void CC_PaletteMake(void)
{
	u16	pal[] = { 0x5294, 0x6b5a, 0x7fff };

	PaletteWorkSet( &pal[2], DUMMY_CURSOR_PAL * 16 + COL_WHITE, 2 );	// BG
	PaletteWorkSet( &pal[1], DUMMY_CURSOR_PAL * 16 + COL_BLACK, 2 );	// ����
	PaletteWorkSet( &pal[0], DUMMY_CURSOR_PAL * 16 + COL_DGRAY, 2 );	// �e
}



//========================================================================================
//									���[���{�b�N�X
//========================================================================================
//-----------------------------------------
//	���[���̐����`�F�b�N
//-----------------------------------------
static u8 MyPC_MailBoxMaxCheck(void)
{
	u8	i, max = 0;

	for( i=MAIL_TEMOTI_STOCK; i<MAIL_STOCK_MAX; i++ ){
		if( Fld.MailData[i].design_no != NON_ITEM )
			max++;
	}
	return	max;
}

//-----------------------------------------
//	���[���{�b�N�X�̃X�y�[�X���l�߂�
//-----------------------------------------
static void MyPC_MailBoxSortZERO(void)
{
	MAIL_DATA	copy;
	u8	i,j;

	for( i=MAIL_TEMOTI_STOCK; i<MAIL_STOCK_MAX-1; i++ ){
		for( j=i+1; j<MAIL_STOCK_MAX; j++ ){
			if( Fld.MailData[i].design_no == NON_ITEM ){
				copy = Fld.MailData[i];
				Fld.MailData[i] = Fld.MailData[j];
				Fld.MailData[j] = copy;
			}
		}
	}
}

//-----------------------------------------
//	���o�l���X�g�\��
//-----------------------------------------
static void MyPC_MailListPut( u8 id )
{
	u16	i, y = 0;

	for( i=MYPC_SYS->scr; i<MYPC_SYS->scr+MYPC_SYS->pos_max; i++ ){
		y = ( i - MYPC_SYS->scr ) << 1;
		NMenuBoxWrite( MAIL_LIST_WINX+2, MAIL_LIST_WINY+2+y, MAIL_LIST_WINEX-1, MAIL_LIST_WINY+3+y );
		if( i == MYPC_SYS->max ){
			NMenuMsgWrite( str_yameru, MAIL_LIST_WINX+2, MAIL_LIST_WINY+2+y );
			break;
		}
		NMenuMsgWrite(
			Fld.MailData[ MAIL_TEMOTI_STOCK+i ].writer_name,
			MAIL_LIST_WINX+2, MAIL_LIST_WINY+2+y );
	}

	if( i - MYPC_SYS->scr != 8 ){
		NMenuBoxWrite( MAIL_LIST_WINX+2, MAIL_LIST_WINY+4+y, MAIL_LIST_WINEX-1, MAIL_LIST_WINEY-1 );
	}

	if( MYPC_SYS->scr != 0 )
		AddScrCurActor( MARK_ACT_U_CURSOR, MAIL_SCR_CUR_X, MAIL_SCR_CUR_UY );
	else
		DelScrCurActor( MARK_ACT_U_CURSOR );

	if( MYPC_SYS->scr + MYPC_SYS->pos_max <= MYPC_SYS->max )
		AddScrCurActor( MARK_ACT_D_CURSOR, MAIL_SCR_CUR_X, MAIL_SCR_CUR_DY );
	else
		DelScrCurActor( MARK_ACT_D_CURSOR );
}

static void MyPC_MailListWinPut( u8 id )
{
	ScrActInit();
	AddMarkPal();
	NMenuBoxClear( 0, 0, 29, 19 );
	NMenuWinBoxWrite( MAIL_BOX_WINX, MAIL_BOX_WINY, MAIL_BOX_WINEX, MAIL_BOX_WINEY );
	NMenuMsgWrite( str_my_pc_mail, MAIL_BOX_WINX+1, MAIL_BOX_WINY+1 );
	NMenuWinBoxWrite( MAIL_LIST_WINX, MAIL_LIST_WINY, MAIL_LIST_WINEX, MAIL_LIST_WINEY );
	MyPC_MailListPut(id);
	NMenuVCursorInit( MAIL_LIST_WINX+1, MAIL_LIST_WINY+2, MYPC_SYS->pos_max, MYPC_SYS->pos );
}

//-----------------------------------------
//	���[�����X�g�I��
//-----------------------------------------
static void MyPC_MailListMain( u8 id )
{
	if( FadeData.fade_sw != 0 )	return;

	if( sys.Repeat & U_KEY ){
		if( MYPC_SYS->pos != 0 ){
			SePlay( SE_SELECT );
			MYPC_SYS->pos = NMenuVCursorSet(-1);
		}else{
			if( MYPC_SYS->scr != 0 ){
				SePlay( SE_SELECT );
				MYPC_SYS->scr--;
				MyPC_MailListPut(id);
			}
		}
		return;
	}

	if( sys.Repeat & D_KEY ){
		if( MYPC_SYS->pos != MYPC_SYS->pos_max-1 ){
			SePlay( SE_SELECT );
			MYPC_SYS->pos = NMenuVCursorSet(1);
		}else{
			if( MYPC_SYS->scr+MYPC_SYS->pos != MYPC_SYS->max ){
				SePlay( SE_SELECT );
				MYPC_SYS->scr++;
				MyPC_MailListPut(id);
			}
		}
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		if( MYPC_SYS->scr + MYPC_SYS->pos == MYPC_SYS->max )
			MyPC_MailBoxEnd(id);
		else{
			MyPC_MailBoxDispClear();
			TaskTable[id].TaskAdrs = MyPC_MailMenuMsgSet;
		}
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		MyPC_MailBoxEnd(id);
	}
}

//-----------------------------------------
//	���[�����X�g���I�����ꂽ
//-----------------------------------------
static void MyPC_MailBoxDispClear(void)
{
	DelMarkPal();
	DelScrCurActor( MARK_ACT_U_CURSOR );
	DelScrCurActor( MARK_ACT_D_CURSOR );
}

static void MyPC_MailMenuMsgSet( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	PM_strcpy(
		StrTempBuffer0,
		Fld.MailData[ MAIL_TEMOTI_STOCK + MYPC_SYS->scr + MYPC_SYS->pos ].writer_name );
	PM_MsgExpand( MsgExpandBuffer, msg_my_pc_mail_menu );
	SetTalkMsg( id, MsgExpandBuffer, MyPC_MailMenuInit, 0 );
}


//-----------------------------------------
//	���[���{�b�N�X�I��
//-----------------------------------------
static void MyPC_MailBoxEnd2( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	MyPC_ReturnMenu(id);
}
static void MyPC_MailBoxEnd( u8 id )
{
	MyPC_MailBoxDispClear();
	TaskTable[id].TaskAdrs = MyPC_MailBoxEnd2;
}


//-----------------------------------------
//	���j���[�\��
//-----------------------------------------
static void MyPC_MailMenuInit( u8 id )
{
	NMenuWinBoxWrite( MAIL_MENU_WINX, MAIL_MENU_WINY, MAIL_MENU_WINEX, MAIL_MENU_WINEY );
	NMenuFixListWrite( MAIL_MENU_WINX+2, MAIL_MENU_WINY+1, 4, mail_menu );
	NMenuVCursorInit( MAIL_MENU_WINX+1, MAIL_MENU_WINY+1, 4, 0 );
	TaskTable[id].TaskAdrs = MyPC_MailMenuMain;
}

//-----------------------------------------
//	���j���[�I��
//-----------------------------------------
static void MyPC_MailMenuMain( u8 id )
{
	MENUFUNC	func;

	if( sys.Repeat & U_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet(-1);
		return;
	}

	if( sys.Repeat & D_KEY ){
		SePlay( SE_SELECT );
		NMenuVCursorSet(1);
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay( SE_SELECT );
		func = mail_menu[ NMenuVCursorPosGet() ].ptr;
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		MyPC_MailEnd(id);
	}
}

//-----------------------------------------
//	�Ȃ��悤���@���
//-----------------------------------------
static void MyPC_MailRead( u8 id )
{
//	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
	TaskTable[id].TaskAdrs = MyPC_MailReadFadeOut;
}
// �t�F�[�h�A�E�g�҂�
static void MyPC_MailReadFadeOut( u8 id )
{
	if( FadeData.fade_sw )	return;

	MailViewInit(
		&Fld.MailData[ MAIL_TEMOTI_STOCK + MYPC_SYS->scr + MYPC_SYS->pos ],
		MyPC_MailReadReturn, 1 );
	DelTask(id);
}
// ���[����ʂ���̖߂��
static void MyPC_MailReadReturn3( u8 id )
{
	if( FieldFadeinCheck() == 1 )
		TaskTable[id].TaskAdrs = MyPC_MailListMain;
}
static void MyPC_MailReadReturn2(void)
{
	u8	id;

//	id = AddTask( MyPC_MailListMain, 0 );
	id = AddTask( MyPC_MailReadReturn3, 0 );
	MyPC_MailListWinPut(id);
//	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
	FieldFadeBlackInSet();
}
static void MyPC_MailReadReturn(void)
{
	pFieldRecoverFunc = MyPC_MailReadReturn2;
	MainProcChange( FieldMainRecover );
}

//-----------------------------------------
//	�o�b�O�Ɂ@���ǂ�
//-----------------------------------------
static void MyPC_MailBagSet( u8 id )
{
	SetTalkMsg( id, msg_my_pc_del_mail_check, MyPC_MailBagSetYesNoInit, 0 );
}

// �͂��E�������@�Z�b�g
static void MyPC_MailBagSetYesNoInit( u8 id )
{
	YesNoSelectInit( MAIL_YESNO_WINX, MAIL_YESNO_WINY );
	SelectToolInit( id, &MailDelYesNoFunc );
}

// �߂��H�@�͂�
static void MyPC_MailDelYes( u8 id )
{
	MAIL_DATA * md = &Fld.MailData[ MAIL_TEMOTI_STOCK + MYPC_SYS->scr + MYPC_SYS->pos ];

	NMenuBoxClear( MAIL_YESNO_WINX, MAIL_YESNO_WINY, MAIL_YESNO_WINEX, MAIL_YESNO_WINEY );

	if( AddItem( md->design_no, 1 ) == FALSE ){
		SetTalkMsg( id, msg_my_pc_bag_max, MyPC_MailEndMain, 0 );
	}else{
		SetTalkMsg( id, msg_my_pc_del_mail, MyPC_MailEndMain, 0 );
		MailDataInit( md );
		MyPC_MailBoxSortZERO();
		MYPC_SYS->max--;
		if( MYPC_SYS->max < MYPC_SYS->pos_max + MYPC_SYS->scr && MYPC_SYS->scr != 0 )
			MYPC_SYS->scr--;
		MyPC_ItemPosMaxGet(id);
	}
}

// �߂��H�@������
static void MyPC_MailDelNo( u8 id )
{
	NMenuBoxClear( MAIL_YESNO_WINX, MAIL_YESNO_WINY, MAIL_YESNO_WINEX, MAIL_YESNO_WINEY );
	MyPC_MailEndMain(id);
}

//-----------------------------------------
//	�|�P�����Ɂ@��������
//-----------------------------------------
static void MyPC_MailPokeSetFadeOut( u8 id );

static void MyPC_MailPokeSet( u8 id )
{
	if( PokeCountMineGet() == 0 ){
		MyPC_PokeMineZero( id );	// �|�P���������Ȃ�
		return;
	}

//	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	FieldFadeControl( FIELD_BLACK_OUT, 0 );
	TaskTable[id].TaskAdrs = MyPC_MailPokeSetFadeOut;
}
// �t�F�[�h�A�E�g�҂�
static void MyPC_MailPokeSetFadeOut( u8 id )
{
	if( FadeData.fade_sw )	return;

	MainProcChange( FieldBagPokeListInit );
	UseFldPokeFlg = PM_ITEM_MAIL_BOX_MODE;
	DelTask(id);
}

// �|�P�������X�g����̖߂��
static void MyPC_MailPokeSetReturn2(void)
{
	u8	id;
	u8	max;

//	id = AddTask( MyPC_MailListMain, 0 );
	id = AddTask( MyPC_MailReadReturn3, 0 );

	max = MYPC_SYS->max;
	MYPC_SYS->max = MyPC_MailBoxMaxCheck();
	MyPC_MailBoxSortZERO();
	if( max != MYPC_SYS->max &&
		MYPC_SYS->max < MYPC_SYS->pos_max + MYPC_SYS->scr && MYPC_SYS->scr != 0 )
	{
		MYPC_SYS->scr--;
	}
	MyPC_ItemPosMaxGet(id);

	MyPC_MailListWinPut(id);
	FieldFadeBlackInSet();
//	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
}
void MyPC_MailPokeSetReturn(void)
{
	pFieldRecoverFunc = MyPC_MailPokeSetReturn2;
	MainProcChange( FieldMainRecover );
}

// �|�P���������Ȃ�
static void MyPC_PokeMineZero( u8 id )
{
	SetTalkMsg( id, msg_poke_mine_zero, MyPC_MailEndMain, 0 );
}


//-----------------------------------------
//	��߂�
//-----------------------------------------
static void MyPC_MailEnd( u8 id )
{
	NMenuBoxClear( MAIL_MENU_WINX, MAIL_MENU_WINY, MAIL_MENU_WINEX, MAIL_MENU_WINEY );
	MyPC_MailEndMain(id);
}

static void MyPC_MailEndMain( u8 id )
{
	MyPC_MailListWinPut(id);
	TaskTable[id].TaskAdrs = MyPC_MailListMain;
}
