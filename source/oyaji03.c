//=========================================================================================
//									���T�C�N�����₶
//								2002/02/18 by nakahiro
//=========================================================================================
#include "common.h"
#include "print.h"
#include "menutool.h"
#include "task.h"
#include "script.h"
#include "gds_main.h"
#include "goods.h"
#include "oyaji.h"
#include "oyaji03.h"
#include "mus_tool.h"
#include "itemtool.h"
#include "message.h"

#include "syswork.h"		//OBJCHRWORK�Q�Ƃ̂���

#include "../data/gds_sym.h"

//=========================================================================================
//										�V���{��
//=========================================================================================
#define	WIN_X		0		// �E�B���h�E�\��X���W
#define	WIN_Y		1		// �E�B���h�E�\��Y���W
#define	WIN_H		12		// �E�B���h�E��
#define	WIN_VMAX	11		// �E�B���h�E�ő卂��

// �^�X�N���[�N�ԍ�
#define	P_MAX		1		// �O�b�Y��
#define	POS			2		// �J�[�\���ʒu

//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
// ���₶�������Ă�f�t�H���g�O�b�Y�f�[�^
// ���O
static const u8 * const def_name[] = {
	recycle_oyaji_name01,
	recycle_oyaji_name02,
	recycle_oyaji_name03,
	recycle_oyaji_name04,
};
// �O�b�Y
static const u8 def_goods[] = {
	GOODS_YOMAWARUDOORU,
	GOODS_BOORUKUSSYON,
	GOODS_TAIYA,
	GOODS_KAWAIIHANA,
};


//------------------------------------
//		���T�C�N���O�b�Y���\�[�g
//------------------------------------
static void RecycleGoodsSort(void)
{
	u8	i,j;
	u8	no, name[RECYCLE_OYAJI_NAME];
	OYAJIDATA_RECYCLE * dat;

	dat = (OYAJIDATA_RECYCLE *)(Fld.oyaji_work);

	for( i=0; i<RECYCLE_OYAJI_MAX-1; i++ ){
		for( j=i+1; j<RECYCLE_OYAJI_MAX; j++ ){
			if( dat->goods[i] == 0 ){
				no = dat->goods[i];
				dat->goods[i] = dat->goods[j];
				dat->goods[j] = no;

				PM_strcpy( name, &dat->name[RECYCLE_OYAJI_NAME*i] );
				PM_strcpy( &dat->name[RECYCLE_OYAJI_NAME*i], &dat->name[RECYCLE_OYAJI_NAME*j] );
				PM_strcpy( &dat->name[RECYCLE_OYAJI_NAME*j], name );
			}
		}
	}
}

//----------------------------------
//		���T�C�N�����₶������
//----------------------------------
void InitRecycleOyaji(void)
{
	u8	i;
	OYAJIDATA_RECYCLE * dat;

	dat = (OYAJIDATA_RECYCLE *)(Fld.oyaji_work);
	dat->type = OYAJITYPE_RECYCLE;
	dat->flg  = 0;

	for( i=0; i<RECYCLE_OYAJI_MAX; i++ ){
		PM_strcpy( &dat->name[ RECYCLE_OYAJI_NAME * i ],  def_name[i] );
		dat->goods[i] = def_goods[i];
	};

	RecycleGoodsSort();
}

//---------------------------------------
//		�O�b�Y���������t���O������
//---------------------------------------
void ResetRGGiveFlag(void)
{
	OYAJIDATA_RECYCLE * dat;

	dat = (OYAJIDATA_RECYCLE *)(Fld.oyaji_work);
	dat->flg = 0;
}

//----------------------------------------------
//		���T�C�N���{�b�N�X�փO�b�Y������
//----------------------------------------------
void RecycleBoxIn( u8 goods )
{
//	OYAJIDATA_RECYCLE * dat;

//	dat = (OYAJIDATA_RECYCLE *)(Fld.oyaji_work);
//	Fld.EventWork[WK_RECYCLE_GOODS-SVWK_START] = goods;
	SetEventWorkValue( WK_RECYCLE_GOODS, (u16)goods );
}

//--------------------------------
//		�O�b�Y�E�B���h�E�\��
//--------------------------------
static void DrawRecycleGoodsWin( u8 id )
{
	u8	i,j=1,k=0;
	OYAJIDATA_RECYCLE * dat;

	dat = (OYAJIDATA_RECYCLE *)(Fld.oyaji_work);

	for( i=0; i<RECYCLE_OYAJI_MAX; i++ ){
		if( dat->goods[i] != 0 )	j++;
	}

	NMenuWinBoxWrite( WIN_X, WIN_Y, WIN_X+WIN_H, WIN_Y+1+(j<<1) );

	for( i=0; i<RECYCLE_OYAJI_MAX; i++ ){
		if( dat->goods[i] != 0 ){
			if( dat->goods[i] > GOODS_DATA_MAX )
				NMenuMsgWrite( str_hate5, WIN_X+2, WIN_Y+1+(k<<1) );
			else
				NMenuMsgWrite( base_goods[dat->goods[i]].name, WIN_X+2, WIN_Y+1+(k<<1) );
			k++;
		}
	}
	NMenuMsgWrite( str_yameru, WIN_X+2, WIN_Y+1+(k<<1) );
	NMenuVCursorInit( WIN_X+1, WIN_Y+1, j, 0 );
	TaskTable[id].work[P_MAX] = k;
}

//------------------------
//		�O�b�Y�I��
//------------------------
static void GetSelectGoods( u8 id, u8 no )
{
	if( no > GOODS_DATA_MAX )	ScriptParameter0 = 0xffff;
	else						ScriptParameter0 = no;

	NMenuBoxClear( WIN_X, WIN_Y, WIN_X+WIN_H, WIN_Y+WIN_VMAX );
	DelTask(id);
	ContinueFieldScript();
}

static void SelectRecycleGoods( u8 id )
{
	OYAJIDATA_RECYCLE * dat;

	dat = (OYAJIDATA_RECYCLE *)(Fld.oyaji_work);

	if( sys.Trg & U_KEY ){
		SePlay(SE_SELECT);
		NMenuVCursorSet( -1 );
		return;
	}
	if( sys.Trg & D_KEY ){
		SePlay(SE_SELECT);
		NMenuVCursorSet( 1 );
		return;
	}
	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		ScriptParameter1 = NMenuVCursorPosGet();
		if( TaskTable[id].work[P_MAX] == ScriptParameter1 ){
			GetSelectGoods( id, 0 );
		}else{
			PM_strcpy( StrTempBuffer0, &dat->name[RECYCLE_OYAJI_NAME*ScriptParameter1] );
			GetSelectGoods( id, dat->goods[ ScriptParameter1 ] );
		}
		return;
	}
	if( sys.Trg & B_BUTTON ){
		SePlay(SE_SELECT);
		GetSelectGoods( id, 0 );
	}
}


//=========================================================================================
//										�X�N���v�g
//=========================================================================================
//--------------------------------------
//	�O�b�Y�����������`�F�b�N
//--------------------------------------
void TradeOyajiTradeCheck(void)
{
	OYAJIDATA_RECYCLE * dat;

	dat = (OYAJIDATA_RECYCLE *)(Fld.oyaji_work);
	AnswerWork = dat->flg;
}

//--------------------------------------
//	�O�b�Y�������Ă��邩
//--------------------------------------
void TradeOyajiGoodsMaxCheck(void)
{
	u8	i;

	for( i=0; i<8; i++ ){
		if( GoodsMaxCheck2(i) != 0 ){
			AnswerWork = 0;
			return;
		}
	}
	AnswerWork = 1;
}

//--------------------------------------
//	�O�b�Y�̍��ڃ`�F�b�N
//--------------------------------------
void TradeOyajiGoodsGenreCheck(void)
{
	AnswerWork = 0;

	if( base_goods[ ScriptParameter0 ].box == base_goods[ ScriptParameter2 ].box )
		return;

	if( CheckGoodsSpace( base_goods[ ScriptParameter0 ].box ) == -1 ){
		GetGoodsTypeStr( StrTempBuffer1, base_goods[ ScriptParameter0 ].box );
		AnswerWork = 1;
	}
}


//--------------------------------------
// ��������O�b�Y��I��
//--------------------------------------
void TradeOyajiMyGoodsSelect(void)
{
	AddTask( TradeOyajiMyGoodsSelectInit, 0 );
}

// �I��
void TradeOyajiMyGoodsSet( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	DelScrCurActor( MARK_ACT_U_CURSOR );
	DelScrCurActor( MARK_ACT_D_CURSOR );
	DelMarkActorAll( mark_act, 8 );
	DelMarkPal();
	if( GoodsPutCheck() == TRUE ){
		ScriptParameter2 = GoodsType[select_goods_pos];
		PM_strcpy( StrTempBuffer2, base_goods[ ScriptParameter0 ].name );
		PM_strcpy( StrTempBuffer1, base_goods[ ScriptParameter2 ].name );
	}else	ScriptParameter2 = 0xffff;
	DelTask(id);
	ContinueFieldScript();
}
// �L�����Z��
void TradeOyajiMyGoodsCancel( u8 id )
{
	NMenuBoxClear( 0, 0, 29, 19 );
	ScriptParameter2 = 0;
	DelTask(id);
	ContinueFieldScript();
}

//--------------------------------------
//	�O�b�Y������
//--------------------------------------
void TradeOyageGoodsTarade(void)
{
	OYAJIDATA_RECYCLE * dat;

	dat = (OYAJIDATA_RECYCLE *)(Fld.oyaji_work);

	SubGoods( (u8)ScriptParameter2 );
	AddGoods( (u8)ScriptParameter0 );

	PM_strcpy( &dat->name[RECYCLE_OYAJI_NAME*ScriptParameter1], MyData.my_name );
	dat->goods[ ScriptParameter1 ] = (u8)ScriptParameter2;
	RecycleGoodsSort();
	dat->flg = 1;
}

//-----------------------------------------
//	���T�C�N���O�b�Y�I�������ݒ�
//-----------------------------------------
void InitRecycleWinSelect(void)
{
	u8	id;

	id = AddTask( SelectRecycleGoods, 0 );
	DrawRecycleGoodsWin(id);
}
