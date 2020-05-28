#ifndef __WARPDATA_H__
#define __WARPDATA_H__

//=========================================================================
//	
//
//	���[�v��ѐ�f�[�^�֘A
//
//	2001.01.24	GAME FREAK Inc.
//=========================================================================

#undef GLOBAL
#ifdef __WARPDATA_H_GLOBAL_
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================

// ���[�v�Ƃѐ�f�[�^�\����
typedef struct{
	s8	div_id;
	s8	map_id;
	s16	x;
	s16 y;
}WARP_POS;


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);
GLOBAL int GetWarpNoByMapID( u16 div_id, u16 map_id );
GLOBAL const WARP_POS * GetWarpPosByMapID( u16 div_id, u16 map_id );
GLOBAL const WARP_POS * GetWarpPosByNo( int number );

#undef GLOBAL
#endif	/*__WARPDATA_H__*/

