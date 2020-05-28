//=========================================================================================
//				�R���e�X�g / �������f / �Z�����}�j�A/ �Z�Y�ꂶ������ �|�P�������X�g
//									2002/01/10 by nakahiro
//=========================================================================================
#include "common.h"
#include "print.h"
#include "pokelist.h"
#include "p_status.h"
#include "task.h"
#include "palanm.h"
#include "actanm.h"
#include "intr.h"
#include "menu.h"
#include "poketool.h"
#include "script.h"
#include "fld_main.h"
#include "fld_task.h"
#include "fight.h"
#include "mus_tool.h"
#include "contest.h"
#include "con_tool.h"

//========================================================================================
//									�v���g�^�C�v�錾
//========================================================================================
static void InitFadeWait(u8);
static void ContestCardMoji(void);

//----------------------------------
//		�|�P�������X�g�Ăяo��
//----------------------------------
// �R���e�X�g
void ContestPokeListInit(void)
{
	u8	id;

	SetForceEvent();
	id = AddTask( InitFadeWait, 10 );
	TaskTable[id].work[0] = CONTEST_POKELIST_ID;
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}

// �������f
void NamePokeListInit(void)
{
	u8	id;

	SetForceEvent();
	id = AddTask( InitFadeWait, 10 );
	TaskTable[id].work[0] = NAME_POKELIST_ID;
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}

// �Z�����}�j�A
void ManiaPokeListInit(void)
{
	u8	id;

	SetForceEvent();
	id = AddTask( InitFadeWait, 10 );
	TaskTable[id].work[0] = MANIA_POKELIST_ID;
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}

static void InitFadeWait( u8 id )
{
	if( !(FadeData.fade_sw) ){
		FadeData.trans_stop = 1;
		PokeListCall( (u8)TaskTable[id].work[0], SINGLE_LIST );
		DelTask(id);
	}
}

//-------------------------
//		�����`�揈��
//-------------------------
u8 ContestListInit(void)
{
	switch( P_LIST->work[0] ){
	case 0:
		if( P_LIST->work[1] < PokeCountMine ){
			PataChrActorSet( P_LIST->task_id,
							 P_LIST->work[1],
							 &PokeParaMine[ P_LIST->work[1] ] );
			P_LIST->work[1]++;
		}else{
			P_LIST->work[1] = 0;
			P_LIST->work[0]++;
		}
		break;

	case 1:
		ItmeIconCellPalSet();
		P_LIST->work[0]++;
		break;

	case 2:
		ItemIconActorSet( P_LIST->task_id );
		P_LIST->work[0]++;
		break;

	case 3:
		if( PokeCursorSet2( P_LIST->task_id, P_LIST->work[1] ) == 1 ){
			P_LIST->work[1] = 0;
			P_LIST->work[0]++;
		}else	P_LIST->work[1]++;
		break;

	case 4:
		PokeLvPutAll();
		P_LIST->work[0]++;
		break;

	case 5:
		PokeNamePutAll();
		P_LIST->work[0]++;
		break;

	case 6:
		ContestCardMoji();
		P_LIST->work[0]++;
		break;

	case 7:
		if( PokeListBGCardInit2( P_LIST->work[1] ) == 1 ){
			P_LIST->work[1] = 0;
			P_LIST->work[0] = 0;
			return	TRUE;
		}else	P_LIST->work[1]++;
	}

	return	FALSE;
}

//-----------------------------------
//		�R���e�X�g�Q���`�F�b�N
//-----------------------------------
static void ContestCardMoji(void)
{
	u8	i;

	for( i=0; i<PokeCountMine; i++ ){
		switch( ContestParticipationCheck( &PokeParaMine[i] ) ){
		case 0:
		case 3:
		case 4:
			ListCardMoji( i, CARD_DERAREMASEN );
			break;
		case 1:
		case 2:
			ListCardMoji( i, CARD_DERAREMASU );
		}
	}
}

//-------------------------
//		���C���^�X�N
//-------------------------
void CN_PokeListTask( u8 id )
{
	if( FadeData.fade_sw )	return;

	switch( PokeCursorSys(id) ){
	case A_BUTTON:
		SePlay(SE_SELECT);
		ConTemotiNo = GetNowCursorPos(id);
		ScriptParameter0 = ConTemotiNo;
		PokeListEnd_ContinueScript(id);
		break;

	case B_BUTTON:
		SePlay(SE_SELECT);
		ConTemotiNo = 0xff;
		ScriptParameter0 = ConTemotiNo;
		PokeListEnd_ContinueScript(id);
	}
}


//========================================================================================
//								�Z�����}�j�A�̏���
//========================================================================================
static void ListCardOboePut(void);

//-------------------------
//		�����`�揈��
//-------------------------
u8 ManiaListInit(void)
{
	switch( P_LIST->work[0] ){
	case 0:
		if( P_LIST->work[1] < PokeCountMine ){
			PataChrActorSet( P_LIST->task_id,
							 P_LIST->work[1],
							 &PokeParaMine[ P_LIST->work[1] ] );
			P_LIST->work[1]++;
		}else{
			P_LIST->work[1] = 0;
			P_LIST->work[0]++;
		}
		break;

	case 1:
		ItmeIconCellPalSet();
		P_LIST->work[0]++;
		break;

	case 2:
		ItemIconActorSet( P_LIST->task_id );
		P_LIST->work[0]++;
		break;

	case 3:
		if( PokeCursorSet2( P_LIST->task_id, P_LIST->work[1] ) == 1 ){
			P_LIST->work[1] = 0;
			P_LIST->work[0]++;
		}else	P_LIST->work[1]++;
		break;

	case 4:
		PokeLvPutAll();
		P_LIST->work[0]++;
		break;

	case 5:
		PokeNamePutAll();
		P_LIST->work[0]++;
		break;

	case 6:
		ListCardOboePut();
		P_LIST->work[0]++;
		break;

	case 7:
		if( PokeListBGCardInit2( P_LIST->work[1] ) == 1 ){
			P_LIST->work[1] = 0;
			P_LIST->work[0] = 0;
			return	TRUE;
		}else	P_LIST->work[1]++;
	}

	return	FALSE;
}

// �u�o������v�u�o�����Ȃ��v�\��
static void ListCardOboePut(void)
{
	u8	i;

	for( i=0; i<PokeCountMine; i++ ){
		if( PokeOboeruWazaCheck( &PokeParaMine[i] ) == 0 )
			ListCardMoji( i, CARD_OBOERARENAI );
		else
			ListCardMoji( i, CARD_OBOERARERU );
	}
}

//-------------------------
//		���C���^�X�N
//-------------------------
void ManiaPokeListTask( u8 id )
{
	if( FadeData.fade_sw )	return;

	switch( PokeCursorSys(id) ){
	case A_BUTTON:
		SePlay(SE_SELECT);
		ScriptParameter0 = GetNowCursorPos(id);
		ScriptParameter1 = (u16)PokeOboeruWazaCheck( &PokeParaMine[ ScriptParameter0 ] );
		PokeListEnd_ContinueScript(id);
		break;

	case B_BUTTON:
		SePlay(SE_SELECT);
		ScriptParameter0 = 0xff;
		PokeListEnd_ContinueScript(id);
	}
}


//========================================================================================
//								�Z�Y�ꂶ������̏���
//========================================================================================
extern const u8	pp_count_mask[];

//-------------------------------------
//		�X�e�[�^�X��ʂ��Ăяo��
//-------------------------------------
void WazaWasureCall(void)
{
	PokeNewWazaSet( PokeParaMine, (u8)ScriptParameter0,
					PokeCountMine-1, FieldMainRecover, 0 );
	P_SYSTEM->type = DEL_WAZA;
	pFieldRecoverFunc = AddScriptFieldInTask;
}

//----------------------------
//		�Z�̌��𒲂ׂ�
//----------------------------
void WazaWasureCheck(void)
{
	u8	i;

	AnswerWork = 0;

	for( i=0; i<4; i++ ){
		if( PokeParaGet( &PokeParaMine[ScriptParameter0], ID_waza1 + i ) != 0 )
			AnswerWork++;
	}
}

//------------------------------------------
//	�j�b�N�l�[���� StrTempBuffer0 �ɃZ�b�g
//	�Y���Z���� StrTempBuffer1 �ɃZ�b�g
//------------------------------------------
void WazaWasureStrSet(void)
{
	PokemonParam * pp;
	u16	waza;

	pp = &PokeParaMine[ ScriptParameter0 ];
	waza = PokeParaGet( pp, ID_waza1 + ScriptParameter1 );

	PokeNickNameSet( pp, StrTempBuffer0 );
	PM_strcpy( StrTempBuffer1, WazaNameData[waza] );
}

//------------------------------------------
//	�Z��Y���
//------------------------------------------
static void WazaChange( PokemonParam * pp, u8 pos1, u8 pos2 )
{
	u16	no1,no2;
	u8	pp1,pp2;
	u8	cnt,d_cnt1,d_cnt2;

	no1 = PokeParaGet( pp, ID_waza1 + pos1 );
	no2 = PokeParaGet( pp, ID_waza1 + pos2 );
	pp1 = PokeParaGet( pp, ID_pp1 + pos1 );
	pp2 = PokeParaGet( pp, ID_pp1 + pos2 );
	cnt = PokeParaGet( pp, ID_pp_count );
		
	d_cnt1 = ( cnt & pp_count_mask[ pos1 ] ) >> ( pos1 * 2 );
	d_cnt2 = ( cnt & pp_count_mask[ pos2 ] ) >> ( pos2 * 2 );
	cnt &= ~pp_count_mask[ pos1 ];
	cnt &= ~pp_count_mask[ pos2 ];
	cnt |= ( d_cnt1 << ( pos2 * 2 ) ) + ( d_cnt2 << ( pos1 * 2 ) );
		
	PokeParaPut( pp, ID_waza1 + pos1, (u8 *)&no2 );
	PokeParaPut( pp, ID_waza1 + pos2, (u8 *)&no1 );
	PokeParaPut( pp, ID_pp1 + pos1, &pp2 );
	PokeParaPut( pp, ID_pp1 + pos2, &pp1 );
	PokeParaPut( pp, ID_pp_count, &cnt );
}

void WazaWasureMain(void)
{
	u16	i;
//	u16	waza1, waza2;

	PokeWazaSetPos( &PokeParaMine[ ScriptParameter0 ], 0, ScriptParameter1 );
	PokePPCountClear( &PokeParaMine[ ScriptParameter0 ], ScriptParameter1 );

	for( i=ScriptParameter1; i<3; i++ ){
		WazaChange( &PokeParaMine[ ScriptParameter0 ], i, i+1 );
//		waza1 = PokeParaGet( &PokeParaMine[ ScriptParameter0 ], ID_waza1 + i );
//		waza2 = PokeParaGet( &PokeParaMine[ ScriptParameter0 ], ID_waza1 + i+1 );
//		PokeWazaSetPos( &PokeParaMine[ ScriptParameter0 ], waza2, i );
//		PokeWazaSetPos( &PokeParaMine[ ScriptParameter0 ], waza1, i+1 );
	}
}

//------------------------------------------
//	�^�}�S���ǂ���
//------------------------------------------
void WazaWasureTamagoCheck(void)
{
	PokemonParam * pp;

	pp = &PokeParaMine[ ScriptParameter0 ];

	AnswerWork = 0;

	if( PokeParaGet( pp, ID_tamago_flag ) != 0 )
		AnswerWork = 1;
}
