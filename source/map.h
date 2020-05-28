#ifndef __MAP_H__
#define __MAP_H__

//=========================================================================
//
//
//	�}�b�v�֘A��`
//
//	
//=========================================================================

#include "evobj.h"
#include "sxy.h"
#include "mapscrn.h"

#undef GLOBAL
#ifdef MAP_H_GLOBAL
#define GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif


//=========================================================================
//	��`
//=========================================================================

//------------------------------------------------------------
//------------------------------------------------------------
#define CELL_SIZE	16

// ���z��ʊ֘A��`
#define	X_MARGIN		7
#define	Y_MARGIN		5
#define	VIEW_UP_MARGIN	2

#define SCR_CELL_X	(15)
#define SCR_CELL_Y	(12)
#define	SCR_BG_CELL_X	(SCR_CELL_X)
#define SCR_BG_CELL_Y	(14)

#define CENTER_X_MARGIN	(X_MARGIN)
#define CENTER_Y_MARGIN	(Y_MARGIN+VIEW_UP_MARGIN)

#define MAP_X_MARGIN	(CENTER_X_MARGIN)
#define MAP_Y_MARGIN	(CENTER_Y_MARGIN)

#define MAP_LEFT_MARGIN		MAP_X_MARGIN
#define MAP_RIGHT_MARGIN	8
#define MAP_UP_MARGIN		MAP_Y_MARGIN
#define MAP_DOWN_MARGIN		7


#define MAP_OUT_MARGIN		2

//------------------------------------------------------------
//	�t�B�[���h�}�b�v�ڑ��p��`
//------------------------------------------------------------
enum{
	MAPSITE_NOTHING,       	//000
	MAPSITE_DOWN,          	//001
	MAPSITE_UP,            	//002
	MAPSITE_LEFT,          	//003
	MAPSITE_RIGHT,         	//004
	SITE_BELOW,			/* ��������� */
	SITE_ABOVE,			/* �����ԕ��� */
};

//------------------------------------------------------------
// �C�x���g�f�[�^(���j
//------------------------------------------------------------
typedef u8 EV_DATA;

//------------------------------------------------------------
// �t�B�[���h�}�b�v�ڑ��f�[�^
//------------------------------------------------------------
typedef struct{
		u8 direction;
		int PosDiff;
		u8 div_id;
		u8 map_id;
}MLink;

typedef struct{
		int count;
		const MLink * Links;
}MapLinkData;



//------------------------------------------------------------
//	�}�b�v�����f�[�^�\���̒�`
//------------------------------------------------------------
typedef struct MapTotal{
		const MapScreenData * Screen;		// �}�b�v�X�N���[���f�[�^�ւ̃|�C���^
		const SXY_DATA * Sxy;				// �r�w�x�f�[�^�ւ̃|�C���^
		const EV_DATA * Event;				// �C�x���g�f�[�^�ւ̃|�C���^
		const MapLinkData * Link;			// �}�b�v�����N���ւ̃|�C���^
		u16		BGMNo;						// �a�f�l�̃i���o�[
		u16		screen_id;					// �}�b�v�X�N���[���f�[�^��ID
		u8		WorldPos;					// ���[���h�}�b�v��̈ʒu���
		u8		lighting;					// ���邳�w��
		u8		weather;					// �V��w��
		u8		type;						// ����
		u8		wx,wy;						// ���[���h�}�b�v��ł̈ʒu�w��
		u8		disp_sw;					// �n���\���̃I���E�I�t
		u8		battle_bg;					// �퓬�w�i�̎w��
}MapTotalData;

typedef MapTotalData MTD;



//Scroll ����p�A�b��
typedef struct {
	int dx;
	int dy;
}Vector;


#define NOT_MOVE_VALUE (-1)
#include "scroll.ndw"


//=========================================================================
//	�ϐ�
//=========================================================================

extern const Vector SiteVector[];

/* ���݂̃}�b�v�����f�[�^�\���� */
extern MapTotalData NowMap;

#undef GLOBAL
#endif /*__MAP_H__*/
