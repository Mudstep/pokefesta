//=========================================================================
//								�������֘A
//									2001/09/27 by nakahiro
//=========================================================================
#include "common.h"
#include "MaDefine.h"
#include "menu.h"
#include "actor.h"
#include "actanm.h"
#include "ev_flag.h"
#include "gold.h"
#include "decord.h"
#include "message.h"

//-------------------------------------------------------------------------
//								�������`�F�b�N
//	����  �Fmy    = �`�F�b�N�ꏊ
//			check = �`�F�b�N���z
//	�߂�l�FTRUE  = ����Ă���
//			FALSE = ����Ȃ�
//-------------------------------------------------------------------------
u8 CheckGold( u32 my, u32 check )
{
	if( my >= check )	return	TRUE;
	return	FALSE;
}

//-------------------------------------------------------------------------
//								�������𑝂₷
//	�����F*my = ���₷�ꏊ
//		  add = ���₷���z
//-------------------------------------------------------------------------
void AddGold( u32 *my, u32 add )
{
	if( *my > (u32)( *my + add ) ){
		*my = GOLD_MAX;
		return;
	}

	*my += add;
	if( *my > GOLD_MAX )	*my = GOLD_MAX;
}

//-------------------------------------------------------------------------
//								�����������炷
//	�����F*my = ���炷�ꏊ
//		  sub = ���炷���z
//-------------------------------------------------------------------------
void SubGold( u32 *my, u32 sub )
{
	if( *my < sub )	*my = 0;
	else			*my -= sub;
}

//-------------------------------------------------------------------------
//								���z�\��
//	�����Fmy   = ���z
//		  keta = �\������
//		  x,y  = �E�B���h�E������W
//-------------------------------------------------------------------------
void DrawGold( u32 my, u8 keta, u8 x, u8 y )
{
	PM_NumMsgSet( StrTempBuffer0, my, NUM_MODE_SPACE, keta );
	NMenuMsgWrite( str_yen, x, y );
}

//--------------------------------
//		"����������"�A�N�^�[
//--------------------------------
#define	CELLID_OKOZUKAI		10018
#define	PALID_OKOZUKAI		10018

static u8	act_no = 0;

static const ActOamData gold_oam =
{
	0,		//V�߼޼��
	0,		//�g��k��Ӱ�ސݒ�
	0,		//OBJӰ�ސݒ�(NORMAL)
	0,		//ӻ޲��ݒ�(OFF)
	0,		//�װӰ�ސݒ�(16�F)
	1,		//OBJ�`��ݒ�(�����`)
	0,		//H�߼޼��
	0,		//�g��k�����Ұ����ް�ݒ�
	2,		//OBJ�T�C�Y�ݒ�	16x16
	0,		//��׸����
	0,		//�\����ײ��è��ݒ�(��3�D��)
	0,		//��گ����ް
	0,		//AffineParam
};
static const actAnm gold_anm[] =
{
	{ OBJ16_32x16 * 0, 0, 0, 0 },
	{ ACT_ANMEND, 0, 0, 0},
};
static const actAnm * const GoldAnmTbl[] = 
{
	gold_anm,
};

static const actHeader ActGoldHeader =
{
	CELLID_OKOZUKAI,
	PALID_OKOZUKAI,
	&gold_oam,
	GoldAnmTbl,
	0,
	DummyActAffTbl,
	DummyActMove,
};

static const CellData OkozukaiCell = {
	(void *)gold_pch_ADRS, OBJ_SIZE16_32x16, CELLID_OKOZUKAI
};

static const PalData OkozukaiPal = { (void *)shopwin_pcl_ADRS, PALID_OKOZUKAI };




//-------------------------------------------------------------------------
//						�E�B���h�E���̏������\��
//	�����Fmy  = ������
//		  x,y = �E�B���h�E������W
//-------------------------------------------------------------------------
void DrawWinOkozukai( u32 my, u8 x, u8 y )
{
	DrawGold( my, GOLD_KETA_MAX, x+6, y+1 );
}

//-------------------------------------------------------------------------
//							�������E�B���h�E�\��
//	�����Fmy  = ������
//		  x,y = �E�B���h�E������W
//-------------------------------------------------------------------------
void DrawGoldWin( u32 my, u8 x, u8 y )
{
	NMenuWinBoxWrite( x, y, x+13, y+3 );
	DrawWinOkozukai( my, x, y );

	DecordCellSet( (CellData *)&OkozukaiCell );
	DecordPalSet( (PalData *)&OkozukaiPal );

	act_no = AddActor( &ActGoldHeader, x*8+19, y*8+11, 0 );
}

//-------------------------------------------------------------------------
//							�������E�B���h�E�폜
//	�����Fx,y = �E�B���h�E������W
//-------------------------------------------------------------------------
void DelOkozukaiWin( u8 x, u8 y )
{
	AllDelActor( &ActWork[act_no] );
	ObjPalRegNumDel( PALID_OKOZUKAI );

	NMenuBoxClear( x, y, x+13, y+3 );
}

// =================================================================
// CheckMyGold
// �T�v  : �����̎莝���̂���������Ă��邩(�X�N���v�g�p)
// ����  : �����i��������r������z��ScriptParameter1�ɓ����Ă���j
// �߂�l: u8 TRUE=����Ă�	FALSE=����ĂȂ�
// =================================================================
u8 CheckMyGold(void)
{
	return CheckGold(Fld.my_gold,ScriptParameter1);
}

// =================================================================
// SubMyGold
// �T�v  : �����̎莝���̂���������z������(�X�N���v�g�p)
// ����  : �����i�����������������z��ScriptParameter1�ɓ����Ă���j
// �߂�l: none
// =================================================================
void SubMyGold(void)
{
	SubGold( &Fld.my_gold, ScriptParameter1 );
}

