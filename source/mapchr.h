
#ifndef __MAPCHR_H__
#define __MAPCHR_H__

//=========================================================================
//	
//	�}�b�v�L�����E�p���b�g�f�[�^�֘A
//
//=========================================================================

#include	"mapscrn.h"


#undef GLOBAL
#ifdef __MAPCHR_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

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
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

GLOBAL void TransMapCharData(const MapScreenData * scrn);
GLOBAL void TransMapPaletteData(const MapScreenData * scrn);

GLOBAL void TransMapChar2ndData(const MapScreenData * scrn);
GLOBAL void TransMapChar1stData(const MapScreenData * scrn);
GLOBAL void TransMapPalette1stData(const MapScreenData * scrn);
GLOBAL void TransMapPalette2ndData(const MapScreenData * scrn);


#undef GLOBAL
#endif	/*__MAPCHR_H__*/

