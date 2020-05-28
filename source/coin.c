//========================================================================================
//										�R�C��
//								2002/03/29 by nakahiro
//								2002/06/05 by Iwasawa
//								2002/07/17 by tomo(win&etc)
//========================================================================================
#include "common.h"
#include "menu.h"
#include "ev_flag.h"
#include "coin.h"
#include "message.h"


//---------------------------------------
//		�E�B���h�E���̏����R�C���\��
//	�����Fmy  = �����R�C��
//		  x,y = �E�B���h�E������W
//---------------------------------------
void DrawWinCoin( u32 my, u8 x, u8 y )
{
	DrawCoin( my, COIN_KETA_MAX, x+2, y+1 );
}

//---------------------------------------
//		�����R�C���E�B���h�E�\��
//	�����Fmy  = �����R�C��
//		  x,y = �E�B���h�E������W
//---------------------------------------
void DrawCoinWin( u32 my, u8 x, u8 y )
{
	NMenuWinBoxWrite( x, y, x+9, y+3 );
	DrawWinCoin( my, x, y );
}

//---------------------------------------
//		�����R�C���E�B���h�E�폜
//	�����Fx,y = �E�B���h�E������W
//---------------------------------------
void DelCoinWin( u8 x, u8 y )
{
	NMenuBoxClear( x, y, x+9, y+3 );
}

//---------------------------------------
//				�R�C���\��
//	�����Fmy   = �R�C����
//		  keta = �\������
//		  x,y  = �E�B���h�E������W
//---------------------------------------
void DrawCoin( u32 my, u8 keta, u8 x, u8 y )
{
	PM_NumMsgSet( StrTempBuffer0, my, NUM_MODE_SPACE, keta );
	NMenuMsgWrite( str_coin_mai, x, y );
}

//---------------------------------------
//			�莝�������`�F�b�N
//	�߂�l : �莝���R�C����
//---------------------------------------
u16 G_CheckCoin(void)
{
	return	Fld.my_coin;
}

//---------------------------------------
//			�莝���ɉ�����
//	����   : u16 coin = ���ׂ閇��
//	�߂�l : TRUE = �����@FALSE = ���s
//---------------------------------------
u8 G_AddCoin( u16 coin )
{
	if( G_CheckCoin() >= MY_COIN_MAX )	return	FALSE;

	if( Fld.my_coin > (u16)( Fld.my_coin + coin ) ){
		Fld.my_coin = MY_COIN_MAX;
		return	TRUE;
	}

	Fld.my_coin += coin;
	if( Fld.my_coin > MY_COIN_MAX )	Fld.my_coin = MY_COIN_MAX;
	return	TRUE;
}

//---------------------------------------
//			�莝�������炷
//	����   : u16 coin = ���ׂ閇��
//	�߂�l : TRUE = �����@FALSE = ���s
//---------------------------------------
u8 G_SubCoin( u16 coin )
{
	if( G_CheckCoin() >= coin ){
		Fld.my_coin -= coin;
		return	TRUE;
	}
	return	FALSE;
}
