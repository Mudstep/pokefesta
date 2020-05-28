//<mitool.h>
/////////////////////////////////////////////////////////////////
//		
//		���݁A���V���g�p���Ă���ꕔ�̊֐����W�߂����̂ł�
//
//								02.05.31	By MiyukiIwasawa
/////////////////////////////////////////////////////////////////

#ifndef _M_I_TOOL_H_
#define _M_I_TOOL_H_

////////////////////////////////////////////
//	�ˑ����Ă��郉�C�u����
////////////////////////////////////////////
#include "common.h"
#include "decord.h"
#include "madefine.h"
#include "actor.h"
#include "palanm.h"

////////////////////////////////////////////
//	�ˑ����Ă��郊�e����
///////////////////////////////////////////
//PaletteLumAnime
#define COLANM_MAX		(16)
#define COLANM_CMAX		(31)
#define COLANM_CMIN		(0)

#define COLANM_SW00		(0x0001)
#define COLANM_SW01		(0x0002)
#define COLANM_SW02		(0x0004)
#define COLANM_SW03		(0x0008)
#define COLANM_SW04		(0x0010)
#define COLANM_SW05		(0x0020)
#define COLANM_SW06		(0x0040)
#define COLANM_SW07		(0x0080)
#define COLANM_SW08		(0x0100)
#define COLANM_SW09		(0x0200)
#define COLANM_SW10		(0x0400)
#define COLANM_SW11		(0x0800)
#define COLANM_SW12		(0x1000)
#define COLANM_SW13		(0x2000)
#define COLANM_SW14		(0x4000)
#define COLANM_SW15		(0x8000)

typedef struct{
	u16	r:5;
	u16	g:5;
	u16	b:5;
	u16	d:1;
}_rgb;
typedef struct{
	u16 col;		//�J���[
	u16	w_colNo;	//���Ԃ̃J���[����
	u8	uc_num;		//�����H
	
	u8	uc_wait;	//�E�F�C�g�H
	u8	uc_loop;	//���񃋁[�v�H

	s8	nStep:5;	//���i�K�H
	s8	type:2;		//�^�C�v	0:1��,1:loop,2:up down
	s8	dir:1;		//���x���グ��F�O�A������F�P
}ColAnmData;
typedef struct{
	u8		st_flg:7;	//���݂̃X�e�[�g
	u8		live_sw:1;	//�L���P�A�����O
	
	u8		uc_wCt;		//�E�F�C�g�J�E���^
	u8		uc_dCt;		//�����J�E���^
	s8		dx;			//����
	
	ColAnmData	dat;	//�f�[�^
}ColAnmWork;
typedef struct{
	u8			uc_flg;			//�t���O
	u16			w_actsw;		//���[�N�̂ǂ����ғ������邩�̃X�C�b�`
	ColAnmWork	work[COLANM_MAX];
}ColorAnime;

////////////////////////////////////////////
//	BG�}�b�v�̐���
////////////////////////////////////////////
//BG�X�N���[��
typedef struct{
	u8	uc_x,uc_y;
}_scroll;

//�e�L�X�g256��256�}�b�v
typedef struct{
	u16	usa[32][32];
}_tMap;

//��]�g��256*256�}�b�v
typedef struct{
	u8	ca[32][32];
}_rMap;


//BgAct
//===========================
#define BGACT_NORMAL	(0)
#define BGACT_COMP		(1)
#define BGACT_CELL_MAX	(32)
#define BGACT_OBJ_MAX	(32)
#define BGACT_PAL_MAX	(16)
#define BGACT_CHAR_MAX	(1024)

////////////////////////////////////////////////////////////
//	BG�R���g���[���֌W�̍\����
////////////////////////////////////////////////////////////
//BG�p�̃Z���f�[�^
typedef struct{
	void*		pCharData;		//�L�����N�^�f�[�^�ւ̃A�h���X
	u8			uc_xSiz;		//�L�����N�^�T�C�YX
	u8			uc_ySiz;		//�L�����N�^�T�C�YY
	u8			uc_ID;			//0�ȊO���w�肷�邱��
	u16			us_ofs;			//�L�����N�^�i���o�[�I�t�Z�b�g(0���w��)
}_bgCell;

//BG�p�̃p���b�g�f�[�^
typedef struct{
	void*		pPalData;		//16�F�p���b�g�f�[�^�z��ւ̃|�C���^
	u8			uc_ID;			//0�ȊO���w�肷�邱��
	u8			uc_palNo;		//�p���b�g�i���o�[(0���w��)
}_bgPal;

typedef struct{		//_bgObj�̃Z�b�g�p�\����
	u16		*pMap;			//�X�N���[���f�[�^�ւ̃|�C���^
	u8		uc_cellID;		//�g�p����_bgCell��ID
	u8		uc_palID;		//�g�p����_bgPal��ID
	u8		uc_ID;			//�o�^�p��ID(0�ȊO)
}_bgObjData;

typedef struct{
	u8		uc_ID;		//_bgObj��ID
	u8		uc_palNo;	//BG�p���b�g�i���o�[
	u16		us_ofs;		//BG�L�����N�^�I�t�Z�b�g�i���o�[
	u16*	pMap;		//�X�N���[���f�[�^�ւ̃|�C���^
}_bgObj;

typedef struct
{
	u8			uc_nPal;			//�o�^���ꂽBG�p���b�g�̐�
	u8			uc_nCell;			//�o�^���ꂽBG�Z���̐�
	u16			us_nChar;			//�Ō���̃L�����N�^�i���o�[
	
	u16*		pCharBaseAddr;
	_bgPal		aPal[BGACT_PAL_MAX];	//BG_Pltt �f�[�^
	_bgCell		aCell[BGACT_CELL_MAX];	//BG_Cell Data
}_bgAct;

//===============================================================
//	�O���[�o���֐��Q
//===============================================================

//	�J���[�A�j���[�V�����֌W
//====================================
extern void ColorAnimeInit(ColorAnime*);
extern u8	AddColorAnime(ColorAnime*,u8 no,ColAnmData*);
extern u8	DelColorAnime(ColorAnime*,u8 no);
extern void ColorAnimeStart(ColorAnime*,u16 sw);
extern void ColorAnimeEnd(ColorAnime*,u16 sw);
extern void ColorAnimeLum(ColorAnime*);

//	BG�X�N���[���R���g���[���֌W
//====================================
extern void FillChar16	(u16* ps,u16 srcChar,u8 x,u8 y,u8 nx,u8 ny);
extern void SetChar16	(u16* ps,const u16* srcChar,u8 x,u8 y,u8 nx,u8 ny);
extern void FillChar16R	(_tMap* pMap,u16 srcChar,u8 x,u8 y,u8 nx,u8 ny);
extern void SetChar16R	(_tMap* pMap,const u16* srcChar,u8 x,u8 y,u8 nx,u8 ny);

//	BG�A�N�^�[�֌W
//=======================================
extern void	BgActInit(_bgAct* pAct);
extern u8	BgCellSet(_bgAct* pAct,_bgCell *pCell);
extern u8	BgPalSet(_bgAct* pAct,_bgPal *pPal);
extern u8	BgPalSetComp(_bgAct* pAct,_bgPal *pPal);

#endif //_M_I_TOOL_H_

