//=========================================================================================
//									�閧��n�E���T�C�N��
//						base_gds.c �𕪊�	2002/02/07 by nakahiro
//=========================================================================================
#include "common.h"
#include "task.h"
#include "menu.h"
#include "itemtool.h"
#include "fd_shop.h"
#include "gds_main.h"
#include "goods.h"
#include "oyaji03.h"
#include "message.h"

//=========================================================================================
//									�V���{����`
//=========================================================================================

//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
static void RecycleCheck(u8);	// ���T�C�N�����邩
static void RecycleYes(u8);		// YES

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
// �͂��E����������
static const SELECT_ITEM YesNoSelectFunc = { RecycleYes, ReturnGoodsSelectWin };


//=========================================================================================
//									���T�C�N������
//=========================================================================================
//--------------------
//		�����ݒ�
//--------------------
// ��n��
void RecycleInit( u8 id )
{
	if( GoodsMaxCheckAll() == 0 ){
		SetTalkMsg( id, msg_no_goods00, NoGoods2, 0 );
		return;
	}

	TaskTable[id].work[ SCENE_NO ] = 1;
	type_pos = 0;
	GoodsTypeSelectInit(id);
}

// �O�b�Y�V���b�v
void RecycleInit2( u8 id )
{
	if( GoodsMaxCheckAll() == 0 ){
		SetTalkMsg( id, msg_no_goods00, NoGoods2, 0 );
		return;
	}

	TaskTable[id].work[ SCENE_NO ] = 2;
	type_pos = 0;
	GoodsTypeSelectInit(id);
}

//--------------------------------
//		�O�b�Y��I�������Ƃ�
//--------------------------------
void GoodsSelectRecycle( u8 id )
{
	DelGoodsWindow();
	DelGoodsTypeInfoWin();

	if( GoodsPutCheck() == TRUE ){
		PM_strcpy( StrTempBuffer0, base_goods[ GoodsType[select_goods_pos] ].name );
		PM_MsgExpand( MsgExpandBuffer, msg_goods_rec01 );
		SetTalkMsg( id, MsgExpandBuffer, RecycleCheck, 0 );
	}else{
		SetTalkMsg( id, msg_goods_rec00, SelectGoodsPutNot, 0 );
	}
}

//----------------------------
//		���T�C�N�����邩
//----------------------------
static void RecycleCheck( u8 id )
{
	YesNoSelectInit( YESNO_WINX, YESNO_WINY );
	SelectToolInit( id, &YesNoSelectFunc );
}

// YES
static void RecycleYes( u8 id )
{
	NMenuBoxClear( YESNO_WINX, YESNO_WINY, YESNO_WINX+6, YESNO_WINY+6 );
	RecycleBoxIn( GoodsType[ select_goods_pos ] );
	GoodsType[ select_goods_pos ] = 0;
	DelGoodsParamSet(id);
	SetTalkMsg( id, msg_goods_rec02, SelectGoodsPutNot, 0 );
}
