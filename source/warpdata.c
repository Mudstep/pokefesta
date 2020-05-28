//=========================================================================
//	
//
//	���[�v��ѐ�f�[�^�֘A
//
//
//	2001.01.24	GAME FREAK Inc.
//=========================================================================

#include "common.h"

#include "map.h"
#include "../evdata/maparray.h"

#define __WARPDATA_H_GLOBAL_
#include "warpdata.h"

static const WARP_POS WarpPosTable[];

#include "../data/warptbl.c"

#define WARP_DATA_MAX	NELEMS(WarpPosTable)

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...


//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

//=========================================================================
//	�����h�c�ƃ}�b�v�h�c���烏�[�v�Ƃѐ�w���Ԃ�
//	<����>		div_id	�����h�c
//				map_id	�}�b�v�h�c	
//	<�߂�l>	0�ȊO	��ѐ�w��i���o�[
//				0		�w��Ȃ�
//=========================================================================
int GetWarpNoByMapID( u16 div_id, u16 map_id )
{
	int i;

	for ( i=0; i<WARP_DATA_MAX; i++ ) {
		if ( WarpPosTable[i].div_id == div_id &&
				WarpPosTable[i].map_id == map_id ) {
			return i+1;
		}
	}
	return 0;
}

//=========================================================================
//	�����h�c�ƃ}�b�v�h�c���烏�[�v�Ƃѐ�f�[�^��Ԃ�
//	<����>		div_id	�����h�c
//				map_id	�}�b�v�h�c	
//	<�߂�l>	NULL�ȊO	��ѐ�f�[�^�A�h���X
//				NULL		�w��Ȃ�
//=========================================================================
const WARP_POS * GetWarpPosByMapID( u16 div_id, u16 map_id )
{
	int num;

	num = GetWarpNoByMapID( div_id, map_id );
	if ( num == 0 ) return NULL;

	return &WarpPosTable[num-1];
}

const WARP_POS * GetWarpPosByNo( int number )
{
	if ( number == 0 ) return NULL;
	if ( number > WARP_DATA_MAX ) return NULL;
	return &WarpPosTable[number-1];
}


