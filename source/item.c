//=========================================================================
//							�A�C�e���֘A
//									2001/09/06 by nakahiro
//=========================================================================
#include "common.h"
#include "fntequ.h"
#include "menu.h"

#include "bag.h"
#include "item.h"
#include "seed.h"
#include "message.h"

#include "../data/itemsym.h"

#include "item_def.h"			//�A�C�e���f�t�@�C��
#include "../data/itemname.h"	//�A�C�e����
#include "../data/itemeqp.h"	//��������
#include "../data/iteminfo.h"	//�A�C�e������
#include "../data/itemdata.h"	//�A�C�e���f�[�^

extern POCKET	pocket[];		//�|�P�b�g�f�[�^
extern u8		ItemPoint;		//�A�C�e���̏ꏊ

//-------------------------------------------------------------------------
//							�A�C�e�����擾
//	���� : u16  item = �A�C�e��No
//		 : u8 * str  = �A�C�e�����i�[�ꏊ
//-------------------------------------------------------------------------
void GetItemName( u16 item, u8 * str )
{
	const SEED_PARAM * p;

	if( item == ITEM_NAZONOMI ){
		p = GetSeedParam( item - SEED_START_NO );
		PM_strcpy( str, p->name );
		PM_strcat( str, str_XXnomi );
	}else{
		PM_strcpy( str, ItemNameGet( item ) );
	}
}

//-------------------------------------------------------------------------
//						�|�P�b�g�̋󂫂𒲂ׂ�
//�@�����@�Fp    = �|�P�b�g�̔ԍ�
//�@�߂�l�F���� = �󂫈ʒu
//          �Ȃ� = -1
//-------------------------------------------------------------------------
s8	PocketSpace(u8 p)
{
	u8	i;

	for( i=0; i<pocket[ p ].max; i++){
		if(	pocket[ p ].item[i].item == 0 )
			return	i;
	}

	return	-1;
}

//-------------------------------------------------------------------------
//						�|�P�b�g�ɉ������邩�𒲂ׂ�
//�@�����@�Fp    = �|�P�b�g�̔ԍ�
//�@�߂�l�F���� = TRUE
//          �Ȃ� = FALSE
//-------------------------------------------------------------------------
u8	PocketCheck( u8 p )
{
	u8	i;

	for( i=0; i<pocket[ p-1 ].max; i++){
		if(	pocket[ p-1 ].item[i].item != 0 )
			return	TRUE;
	}

	return	FALSE;
}

//-------------------------------------------------------------------------
//							�A�C�e���`�F�b�N
//�@�����@�Fitem = �A�C�e���ԍ�
//          no   = �`�F�b�N�����
//�@�߂�l�F���� = TRUE
//          �Ȃ� = FALSE
//-------------------------------------------------------------------------
u8	ItemCheck( u16 item, u16 no )
{
	u8	i;
	u8	pocketNo;

	if( ItemPocketGet( item ) == 0 )	return FALSE;

	pocketNo = ItemPocketGet( item ) - 1;

	for( i=0; i<pocket[ pocketNo ].max; i++ ){
		if(	pocket[ pocketNo ].item[i].item == item ){
			
			if( pocket[ pocketNo ].item[i].no >= no	)
				return	TRUE;

			no -= pocket[ pocketNo ].item[i].no;

			if( no == 0 )	return	TRUE;
		}
	}

	return	FALSE;


/*
	u8	i;
	u8	pocketNo;

	if( ItemPocketGet( item ) == 0 )	return FALSE;

	pocketNo = ItemPocketGet( item ) - 1;

	for( i=0; i<pocket[ pocketNo ].max; i++){
		if(	pocket[ pocketNo ].item[i].item == item
			&& pocket[ pocketNo ].item[i].no >= no	)
		{
			return	TRUE;
		}
	}

	return	FALSE;
*/

}

//-------------------------------------------------------------------------
//					�A�C�e�����������邩�`�F�b�N
//�@�����@�Fitem  = �A�C�e���ԍ�
//          add   = �������
//�@�߂�l�FTRUE  = ����
//          FALSE = ���s
//-------------------------------------------------------------------------
u8	AddItemCheck( u16 item, u16 add )
{
	u8	i;
	u8	pocketNo;
	u16	max;

	if( ItemPocketGet( item ) == 0 )	return FALSE;

	pocketNo = ItemPocketGet( item ) - 1;

	if( pocketNo != SEED_POCKET-1 )	max = 99;
	else							max = 999;

	// �����A�C�e�������邩
	for( i=0; i<pocket[ pocketNo ].max; i++ ){

		if(	pocket[ pocketNo ].item[i].item == item ){

			// ���������ɏ�������max�𒴂��Ȃ���
			if( pocket[ pocketNo ].item[i].no + add <= max )	return	TRUE;

			// max�𒴂��鎞
			else{
				// �Z�E�؂̎��|�P�b�g��max�ȏ㎝�ĂȂ�
				if( pocketNo == SKILL_POCKET-1 || pocketNo == SEED_POCKET-1 )
					return	FALSE;

				add = add - ( max - pocket[ pocketNo ].item[i].no );
				if( add == 0 )	break;
			}
		}
	}

	// �󂫂ɓ����
	if( add != 0 ){
		for( i=0; i<pocket[ pocketNo ].max; i++ ){
			if(	pocket[ pocketNo ].item[i].item == 0 ){
				if( add > max )	add = add - max;
				else{
					add = 0;
					break;
				}
			}
		}
		if( add != 0 )	return	FALSE;
	}

	return	TRUE;
}


//-------------------------------------------------------------------------
//							�A�C�e����������
//�@�����@�Fitem  = �A�C�e���ԍ�
//          add   = �������
//�@�߂�l�FTRUE  = ����
//          FALSE = ���s
//-------------------------------------------------------------------------
u8	AddItem( u16 item, u16 add )
{
	MINEITEM	dummy[ MY_SKILL_ITEM_MAX ];		// �ł��傫���|�P�b�g
	u8	i;
	u8	pocketNo;
	u16	max;

	if( ItemPocketGet( item ) == 0 )	return FALSE;

	pocketNo = ItemPocketGet( item ) - 1;

	memcpy( dummy, pocket[pocketNo].item, sizeof(MINEITEM) * pocket[pocketNo].max );

	if( pocketNo != SEED_POCKET-1 )	max = 99;
	else							max = 999;

	// �����A�C�e�������邩
	for( i=0; i<pocket[ pocketNo ].max; i++ ){

		if(	dummy[i].item == item ){

			// ���������ɏ�������max�𒴂��Ȃ���
			if( dummy[i].no + add <= max )
			{
//				pocket[ pocketNo ].item[i].no += add;
				dummy[i].no += add;
				memcpy( pocket[pocketNo].item, dummy, sizeof(MINEITEM) * pocket[pocketNo].max );
				return	TRUE;

			// max�𒴂��鎞
			}else{
				// �Z�E�؂̎��|�P�b�g��max�ȏ㎝�ĂȂ�
				if( pocketNo == SKILL_POCKET-1 || pocketNo == SEED_POCKET-1 )
					return	FALSE;

				add = add - ( max - dummy[i].no );
				dummy[i].no = max;
				if( add == 0 )	break;
			}
		}
	}

	// �󂫂ɓ����
	if( add != 0 ){
		for( i=0; i<pocket[ pocketNo ].max; i++ ){
			if(	dummy[i].item == 0 ){
				dummy[i].item = item;
				if( add > max ){
					add = add - max;
					dummy[i].no = max;
				}else{
					dummy[i].no = add;
					add = 0;
					break;
				}
			}
		}
		if( add != 0 )	return	FALSE;
	}

	memcpy( pocket[pocketNo].item, dummy, sizeof(MINEITEM) * pocket[pocketNo].max );

	return	TRUE;
}

//-------------------------------------------------------------------------
//							�A�C�e�������炷
//�@����  �Fitem  = �A�C�e���ԍ�
//          sub   = ���炷��
//�@�߂�l�FTRUE  = ����
//          FALSE = ���s
//-------------------------------------------------------------------------
u8	SubItem( u16 item, u16 sub )
{
	u8	i;
	u8	pocketNo;
	u16	max = 0;

	if( ItemPocketGet( item ) == 0 )	return FALSE;
	if( item == 0 )	return FALSE;

	pocketNo = ItemPocketGet( item ) - 1;

	// �w��A�C�e���̑����𒲂ׂ�
	for( i=0; i<pocket[ pocketNo ].max; i++){
		if(	pocket[ pocketNo ].item[i].item == item )
			max += pocket[ pocketNo ].item[i].no;
	}

	if( max < sub )	return	FALSE;
	
	if( pocket[ pocketNo ].max > ItemPoint ){
		if(	pocket[ pocketNo ].item[ ItemPoint ].item == item ){

			if( pocket[ pocketNo ].item[ ItemPoint ].no >= sub ){
				pocket[ pocketNo ].item[ ItemPoint ].no -= sub;
				sub = 0;
			}else{
				sub -= pocket[ pocketNo ].item[ ItemPoint ].no;
				pocket[ pocketNo ].item[ ItemPoint ].no = 0;
			}

			if( pocket[ pocketNo ].item[ ItemPoint ].no == 0 )
				pocket[ pocketNo ].item[ ItemPoint ].item = 0;

			if( sub == 0 )	return	TRUE;
		}
	}

	for( i=0; i<pocket[ pocketNo ].max; i++){
		if(	pocket[ pocketNo ].item[i].item == item ){

			if( pocket[ pocketNo ].item[i].no >= sub ){
				pocket[ pocketNo ].item[i].no -= sub;
				sub = 0;
			}else{
				sub -= pocket[ pocketNo ].item[i].no;
				pocket[ pocketNo ].item[i].no = 0;
			}

			if( pocket[ pocketNo ].item[i].no == 0 )
				pocket[ pocketNo ].item[i].item = 0;

			if( sub == 0 )	break;
		}
	}

	return	TRUE;


/*
	u8	i;
	u8	pocketNo;

	if( ItemPocketGet( item ) == 0 )	return FALSE;
	if( item == 0 )	return FALSE;

	pocketNo = ItemPocketGet( item ) - 1;

	if( pocket[ pocketNo ].max > ItemPoint ){
		if(	pocket[ pocketNo ].item[ ItemPoint ].item == item &&
			pocket[ pocketNo ].item[ ItemPoint ].no >= sub )
		{
			pocket[ pocketNo ].item[ ItemPoint ].no -= sub;
			if( pocket[ pocketNo ].item[ ItemPoint ].no==0 )
				pocket[ pocketNo ].item[ ItemPoint ].item = 0;
			return	TRUE;
		}
	}

	for( i=0 ; i<pocket[ pocketNo ].max ; i++){
		if(	pocket[ pocketNo ].item[i].item == item
			&& pocket[ pocketNo ].item[i].no >= sub	)
		{
			pocket[ pocketNo ].item[i].no -= sub;
			if( pocket[ pocketNo ].item[i].no==0 )
				pocket[ pocketNo ].item[i].item = 0;
			return	TRUE;
		}
	}

	return	FALSE;
*/

}

//-------------------------------------------------------------------------
//					�A�C�e���ԍ�����|�P�b�g�ԍ����擾
//�@����  �Fitem  = �A�C�e���ԍ�
//�@�߂�l�F�|�P�b�g�ԍ�
//-------------------------------------------------------------------------
u8	GetPocketNo( u16 item )
{
	return	ItemPocketGet( item );
}

//-------------------------------------------------------------------------
//						�A�C�e���f�[�^��������
//	�����@�F* data = �A�C�e���f�[�^�|�C���^
//			max    = ��������
//-------------------------------------------------------------------------
void ItemDataInit( MINEITEM * data, u8 max )
{
	u16	i;

	for( i=0; i<max; i++ ){
		data[i].item = 0;
		data[i].no = 0;
	}
}

//-------------------------------------------------------------------------
//						�p�\�R���̋󂫂𒲂ׂ�
//�@�߂�l�F���� = �󂫈ʒu
//          �Ȃ� = -1
//-------------------------------------------------------------------------
s8 CheckPCBoxItemSpace(void)
{
	s8	i;

	for( i=0; i<MY_PCBOX_ITEM_MAX; i++){
		if(	Fld.MyPCBoxItem[i].item == 0 )
			return	i;
	}

	return	-1;
}

//-------------------------------------------------------------------------
//						�p�\�R���ɉ������邩���ׂ�
//�@�߂�l�F��ސ�
//-------------------------------------------------------------------------
u8 CheckPCBoxItem(void)
{
	u8	i, p = 0;

	for( i=0; i<MY_PCBOX_ITEM_MAX; i++){
		if(	Fld.MyPCBoxItem[i].item != 0 )
			p++;
	}

	return	p;
}

//-------------------------------------------------------------------------
//						�p�\�R���̃A�C�e���`�F�b�N
//�@�����@�Fitem = �A�C�e���ԍ�
//          no   = �`�F�b�N�����
//�@�߂�l�F���� = TRUE
//          �Ȃ� = FALSE
//-------------------------------------------------------------------------
u8 PCBoxItemCheck( u16 item, u16 no )
{
	u8	i;

	for( i=0; i<MY_PCBOX_ITEM_MAX; i++){
		if(	Fld.MyPCBoxItem[i].item == item &&
			Fld.MyPCBoxItem[i].no >= no	)
		{
			return	TRUE;
		}
	}

	return	FALSE;
}

//-------------------------------------------------------------------------
//						�p�\�R���ɃA�C�e����������
//�@�����@�Fitem  = �A�C�e���ԍ�
//          add   = �������
//�@�߂�l�FTRUE  = ����
//          FALSE = ���s
//-------------------------------------------------------------------------
u8 AddPCBoxItem( u16 item, u16 add )
{
	MINEITEM	dummy[ MY_PCBOX_ITEM_MAX ];
	u8	i;
	s8	no;

	memcpy( dummy, Fld.MyPCBoxItem, sizeof(MINEITEM) * MY_PCBOX_ITEM_MAX );

	// �����A�C�e�������邩
	for( i=0; i<MY_PCBOX_ITEM_MAX; i++ ){

		if(	dummy[i].item == item ){

			// ���������ɏ�������max�𒴂��Ȃ���
			if( dummy[i].no + add <= 999 )
			{
//				Fld.MyPCBoxItem[i].no += add;
				dummy[i].no += add;
				memcpy( Fld.MyPCBoxItem, dummy, sizeof(MINEITEM) * MY_PCBOX_ITEM_MAX );
				return	TRUE;

			// max�𒴂��鎞
			}else{
				add = add - ( 999 - dummy[i].no );
				dummy[i].no = 999;
				if( add == 0 )	break;
			}
		}
	}

	// �󂫂ɓ����
	if( add != 0 ){
		no = CheckPCBoxItemSpace();
		if( no == -1 )	return	FALSE;
		dummy[no].item = item;
		dummy[no].no   = add;
	}

	memcpy( Fld.MyPCBoxItem, dummy, sizeof(MINEITEM) * MY_PCBOX_ITEM_MAX );

	return	TRUE;
}

//-------------------------------------------------------------------------
//						�p�\�R���̃A�C�e�������炷
//�@����  �Fpos   = �ʒu
//          sub   = ���炷��
//-------------------------------------------------------------------------
void SubPCBoxItem( u8 pos, u16 sub )
{
	Fld.MyPCBoxItem[pos].no -= sub;

	if( Fld.MyPCBoxItem[pos].no == 0 ){
		Fld.MyPCBoxItem[pos].item = 0;
		PCBoxItemSortZERO();
	}
}

//-------------------------------------------------------------------------
//						�p�\�R���̃X�y�[�X���l�߂�
//	���� : p = �|�P�b�g�f�[�^
//-------------------------------------------------------------------------
void PCBoxItemSortZERO(void)
{
	MINEITEM	copy;
	u16	i,j;

	for( i=0; i<MY_PCBOX_ITEM_MAX-1; i++ ){
		for( j=i+1; j<MY_PCBOX_ITEM_MAX; j++ ){
			if( Fld.MyPCBoxItem[i].item == 0 ){
				copy = Fld.MyPCBoxItem[i];
				Fld.MyPCBoxItem[i] = Fld.MyPCBoxItem[j];
				Fld.MyPCBoxItem[j] = copy;
			}
		}
	}
}




//-------------------------------------------------------------------------
//							�֗��{�^���̌���		//nohara 02/01/16
//-------------------------------------------------------------------------
void ChangeCnvbtn();
void ChangeCnvbtn()
{
	switch( Fld.cnv_btn )
	{
		case ITEM_MAHHAZITENSYA:
			Fld.cnv_btn = ITEM_DAATOZITENSYA;
			break;
		case ITEM_DAATOZITENSYA:
			Fld.cnv_btn = ITEM_MAHHAZITENSYA;
			break;

		//�ǉ����Ă���
	}
}




//=======================================================================================
//								�A�C�e���f�[�^�擾�֐�
//=======================================================================================

//------------------------------------------------------------------------------
//	���r�[ / �T�t�@�C�A �Ő������A�C�e�����ǂ����`�F�b�N
//------------------------------------------------------------------------------
static u16 ItemNumberCheck( u16 item )
{
	if( item > ITEM_DATA_MAX )	return	0;
	return	item;
}

//------------------------------------------------------------------------------
//	�A�C�e�����擾						ItemData[ item ].name[ ITEMNAME_LEN ];
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �A�C�e�����̃|�C���^
//------------------------------------------------------------------------------
u8 * ItemNameGet( u16 item )
{
	return	(u8 *)ItemData[ ItemNumberCheck( item ) ].name;
}

//------------------------------------------------------------------------------
//	�A�C�e���V���{���擾					ItemData[ item ].symbol;
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �A�C�e���V���{��
//------------------------------------------------------------------------------
u16 ItemSymbolGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].symbol;
}

//------------------------------------------------------------------------------
//	�A�C�e���̒l�i�擾						ItemData[ item ].price;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �l�i
//------------------------------------------------------------------------------
u16 ItemPriceGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].price;
}

//------------------------------------------------------------------------------
//	�A�C�e���������ʎ擾					ItemData[ item ].eqp;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �������ʔԍ�
//------------------------------------------------------------------------------
u8 ItemEquipGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].eqp;
}

//------------------------------------------------------------------------------
//	�A�C�e���З͎擾						ItemData[ item ].atc;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �З�
//------------------------------------------------------------------------------
u8 ItemAttackGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].atc;
}

//------------------------------------------------------------------------------
//	�A�C�e���������b�Z�[�W�擾				ItemData[ item ].msg;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �����̃|�C���^
//------------------------------------------------------------------------------
u8 * ItemMessageGet( u16 item )
{
	return	(u8 *)ItemData[ ItemNumberCheck( item ) ].msg;
}

//------------------------------------------------------------------------------
//	�A�C�e���d�v�t���O�擾					ItemData[ item ].imp;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �d�v�t���O
//------------------------------------------------------------------------------
u8 ItemEventGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].imp;
}

//------------------------------------------------------------------------------
//	�A�C�e���֗��{�^���o�^�t���O�擾		ItemData[ item ].cnv_btn;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �֗��{�^���o�^�t���O
//------------------------------------------------------------------------------
u8 ItemCnvButtonGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].cnv_btn;
}

//------------------------------------------------------------------------------
//	�A�C�e���ۑ��|�P�b�g�ԍ��擾			ItemData[ item ].pocket;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �ۑ��|�P�b�g�ԍ�
//------------------------------------------------------------------------------
u8 ItemPocketGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].pocket;
}

//------------------------------------------------------------------------------
//	�A�C�e���t�B�[���h�g�p��ʎ擾			ItemData[ item ].fld;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �t�B�[���h�g�p��ʔԍ�
//------------------------------------------------------------------------------
u8 ItemFldSysGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].fld;
}

//------------------------------------------------------------------------------
//	�A�C�e���g�p�t�B�[���h�v���O�����擾	ItemData[ item ].fld_func
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �t�B�[���h�v���O�����A�h���X
//------------------------------------------------------------------------------
void_func_u8 ItemFldFuncGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].fld_func;
}

//------------------------------------------------------------------------------
//	�A�C�e���o�g���g�p��ʎ擾				ItemData[ item ].btl;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �o�g���g�p��ʔԍ�
//------------------------------------------------------------------------------
u8 ItemBtlSysGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].btl;
}

//------------------------------------------------------------------------------
//	�A�C�e���g�p�o�g���v���O�����擾		ItemData[ item ].btl_func
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : �o�g���v���O�����A�h���X
//------------------------------------------------------------------------------
void_func_u8 ItemBtlFuncGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].btl_func;
}

//------------------------------------------------------------------------------
//	�A�C�e�����[�N�擾						ItemData[ item ].work;
//
//	����   : u16 item = �A�C�e���ԍ�
//	�߂�l : ���[�N���̔ԍ�
//------------------------------------------------------------------------------
u8 ItemWorkGet( u16 item )
{
	return	ItemData[ ItemNumberCheck( item ) ].work;
}
