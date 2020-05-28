//========================================================================================
//									�莝���O�b�Y�֘A
//									2001/06/18 by nakahiro
//========================================================================================
#include "common.h"
#include "gds_main.h"
#include "goods.h"

#include "../data/gds_sym.h"

const GOODS_BOX goods_box[] =
{
	{ Fld.FurnitureDesk,    GOODS_DESK_MAX    },	// ����
	{ Fld.FurnitureChair,   GOODS_CHAIR_MAX   },	// ����
	{ Fld.FurniturePlant,   GOODS_PLANT_MAX   },	// ���A��
	{ Fld.FurnitureEtc,     GOODS_ETC_MAX     },	// ���炭��
	{ Fld.FurnitureMat,     GOODS_MAT_MAX     },	// �}�b�g
	{ Fld.FurniturePoster,  GOODS_POSTER_MAX  },	// �|�X�^�[
	{ Fld.FurnitureDoll,    GOODS_DOLL_MAX    },	// �ʂ������
	{ Fld.FurnitureCushion, GOODS_CUSHION_MAX },	// �N�b�V����
};


//--------------------------------
//		�ꏊ���Ƃɏ�����
//	����   : u8 id = �O�b�Y�̎��
//--------------------------------
void GoodsBoxInit( u8 id )
{
	u8	i;

	for( i=0; i<goods_box[id].max; i++ )
		goods_box[id].box[i] = 0;
}

//--------------------------------
//			������
//--------------------------------
void GoodsAllInit(void)
{
	u8	i;

	for( i=0; i<=GOODS_CUSHION_FOLDER; i++ )
		GoodsBoxInit(i);
}

//--------------------------------
//			�󂫂𒲂ׂ�
//	����   : u8 id = �O�b�Y�̎��
//	�߂�l : s8 i  = �ꏊ
//			 -1    = �󂫖���
//--------------------------------
s8 CheckGoodsSpace( u8 id )
{
	s8	i;

	for( i=0; i<(s8)goods_box[id].max; i++ ){
		if( goods_box[id].box[i] == 0 )	return	i;
	}
	return	-1;
}

//------------------------------------
//		�w��O�b�Y�������Ă��邩
//	����   : u8 id = �O�b�Y�ԍ�
//	�߂�l : TRUE  = ����
//			 FALSE = �Ȃ�
//------------------------------------
u8 CheckGoodsMine( u8 id )
{
	u8	i;
	u8	box = base_goods[id].box;

	for( i=0; i<goods_box[ box ].max; i++ ){
		if( goods_box[ box ].box[i] == id )
			return	TRUE;
	}
	return	FALSE;
}


//----------------------------------
//			�莝���ɉ�����
//	����   : u8 no = �O�b�Y�ԍ�
//	�߂�l : TRUE  = ����
//			 FALSE = ���s
//----------------------------------
u8 AddGoods( u8 no )
{
	s8	pos = 0;

	if( no == 0 )	return	FALSE;

	pos = CheckGoodsSpace( base_goods[no].box );
	if( pos == -1 )	return	FALSE;

	goods_box[ base_goods[no].box ].box[ pos ] = no;
	return	TRUE;
}

//----------------------------------
//			�莝���ɉ������邩�`�F�b�N
//			��02/07/14�ǉ��i@Tomomichi Ohta@)
//	����   : u8 no = �O�b�Y�ԍ�
//	�߂�l : TRUE  = ����
//			 FALSE = ���s
//----------------------------------
u8 AddGoodsCheck( u8 no )
{
	s8	pos = 0;

	if( no == 0 )	return	FALSE;

	pos = CheckGoodsSpace( base_goods[no].box );
	if( pos == -1 )	return	FALSE;

	return	TRUE;
}

//----------------------------------
//			�莝�����猸�炷
//	����   : u8 no = �O�b�Y�ԍ�
//	�߂�l : TRUE  = ����
//			 FALSE = ���s
//----------------------------------
s8 SubGoods( u8 no )
{
	u8	i = 0;

	if( no == 0 )	return	FALSE;

	for( i=0; i<goods_box[ base_goods[no].box ].max; i++ ){
		if( goods_box[ base_goods[no].box ].box[i] == no ){
			goods_box[ base_goods[no].box ].box[i] = 0;
			GoodsSort2( base_goods[no].box );
			return	TRUE;
		}
	}
	return	FALSE;
}

//---------------------------------
//			�O�b�Y�\�[�g
//	���� : u8 id = �O�b�Y�̎��
//---------------------------------
void GoodsSort2( u8 id )
{
	u8	i,j;
	u8	copy;

	for( i=0; i<goods_box[id].max; i++ ){
		for( j=i+1; j<goods_box[id].max; j++ ){
			if( goods_box[id].box[j] != 0 &&
				( goods_box[id].box[i] == 0 || goods_box[id].box[i] > goods_box[id].box[j] ) )
			{
				copy = goods_box[id].box[i];
				goods_box[id].box[i] = goods_box[id].box[j];
				goods_box[id].box[j] = copy;
			}
		}
	}
}

//------------------------------------
//			�O�b�Y���`�F�b�N
//	����   : u8 id = �O�b�Y�̎��
//	�߂�l : ���̎�ނ̎莝���O�b�Y��
//-------------------------------------
u8 GoodsMaxCheck2( u8 id )
{
	u8	i, max = 0;

	for( i=0; i<goods_box[id].max; i++ ){
		if( goods_box[id].box[i] != 0 )		max++;
	}
	return	max;
}

//------------------------------------
//	�@�莝���O�b�Y�̑������`�F�b�N
//	�߂�l : �莝���O�b�Y��
//------------------------------------
u8 GoodsMaxCheckAll(void)
{
	u8	i, cnt = 0;

	for( i=GOODS_DESK_FOLDER; i<=GOODS_CUSHION_FOLDER; i++ )
		cnt += GoodsMaxCheck2(i);

	return	cnt;
}



//========================================================================================

//----------------------------------
//		�f�o�b�O�p�O�b�Y�Z�b�g
//----------------------------------
#ifdef	PM_DEBUG
void DebugGoodsSet2(void)
{
	u8	i;

	for( i=0; i<=GOODS_DATA_MAX; i++ )
		AddGoods(i);
}
#endif	PM_DEBUG
