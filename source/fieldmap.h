#ifndef FIELDMAP_H
#define FIELDMAP_H

//=========================================================================
//
//
//	���z�t�B�[���h�}�b�v�����֘A
//
//
//=========================================================================

#include "map.h"

#undef GLOBAL
#ifdef FIELDMAP_H_GLOBAL
#define GLOBAL	/* */
#else
#define GLOBAL	extern
#endif


//=========================================================================
//	��`
//=========================================================================


typedef struct {
		int Xsize;
		int Ysize;
		u16 * Data;
}VMAP_STATUS;

typedef struct {
		u8	chg_flag:1;		//TRUE--> Map Change, FALSE--> Same Map...
		int diffx;
		int diffy;
}MapChg;


//=========================================================================
//	�ϐ�
//=========================================================================

extern VMAP_STATUS VMap;				// ���z�}�b�v�f�[�^
extern MapChg MapChange;


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
GLOBAL void MakeNowVirtualMap( void );
GLOBAL void MakeContinueVirtualMap( void );

GLOBAL int CheckMapMove( int site );
GLOBAL int CheckMapLimit(int nextXpos, int nextYpos);
GLOBAL int MoveVMapPosition(int dx, int dy);
GLOBAL const MLink * GetLinkMap(s16 x, s16 y);
const MapTotalData * GetNextLinkMapData(const MLink * Link);

GLOBAL void SetMapCenterPos( u16 x, u16 y );
GLOBAL void GetMapCenterPos(u16* x, u16* y);
GLOBAL void SetNowMapPos( u16 x, u16 y );
GLOBAL void GetNowMapPos(u16* x, u16* y);

//GLOBAL int GetCell(int map_x, int map_y);					//�Z���f�[�^�S�̂��擾����
GLOBAL int GetCellAttribute(int px,int py);					//�Z���A�g���r���[�g���擾����
GLOBAL int GetCellCode(int px, int py);						//�Z���f�[�^���擾
GLOBAL u8 GetCellHeight(int px, int py);					//�Z���̍������擾
GLOBAL u8 GetCellHitCheck(int px, int py);					//�Z���̐i���ۂ��擾
GLOBAL void SetCell(int map_x, int map_y ,u16 cell);		// �w��ʒu�ɃZ�����Z�b�g
GLOBAL void SetCellDirect(int map_x, int map_y ,u16 cell);	// 2002/07/22 by nakahiro
GLOBAL u8 GetCellLayer(int px, int py);						//�Z���`���BG�K�w���擾

GLOBAL u16 GetMapCellAttributeData(u16 code);				//�Z���R�[�h���Z���A�g���r���[�g

GLOBAL void BackupVirtualMap( void );
GLOBAL void RecoverVirtualMap( void );

#undef GLOBAL
#endif /*FIELDMAP_H*/
