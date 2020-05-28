//========================================================================================
//		�}���`�o�g���Q���|�P�����I�� / �I���|�P�����m�F / ��ĉ��|�P�������X�g����
//								2002/05/1 by nakahiro
//========================================================================================
#include "common.h"
#include "print.h"
#include "pokelist.h"
#include "task.h"
#include "palanm.h"
#include "actanm.h"
#include "intr.h"
#include "menu.h"
#include "f_pmlist.h"
#include "poketool.h"
#include "weather.h"
#include "p_status.h"
#include "jiki.h"
#include "script.h"
#include "sysflag.h"
#include "fld_main.h"
#include "map.h"
#include "fight.h"
#include "fieldmap.h"
#include "mapparam.h"
#include "fld_tool.h"
#include "mus_tool.h"
#include "pokeicon.h"
#include "itemtool.h"
#include "message.h"
#include "fld_task.h"

#include "b_tower.h"		//add by soga 2002.09.10

//=========================================================================================
//										�V���{��
//=========================================================================================
#define	MENU_SX			20
#define	MENU_SY			10
#define	MENU_EX			29
#define	MENU_EY			19

//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
static void	Status(u8);			// �悳���݂�
static void	Return(u8);			// ���ǂ�
static void MemberIn(u8);		// ���񂩂���
static void MemberOut(u8);		// ���񂩂��Ȃ�

static void SelectPokeInit(void);	// �Q��������
static void MemberOver(u8);			// �Q�����I�[�o�[
static u8 BtlTowerSelectCheck( PokemonParam * pp );	// �o�g���^���[�̑I���`�F�b�N

static void MultiListMenuTask(u8);	// ���j���[�I���^�X�N
static void SelectPokeListEnd(u8);	// �I������
static void Kettei(u8);				// �����Ă�

static void PokeMineIconPut(u8);
static void PokeMineParamPut(void);
static void PokeMineItemIconPut( u8 id );
static void ScrollInPokeListTask2(u8);
static void SetScrollPokeParam(u8);
static void ScrollInKeyWaitTask(u8);

static void GrowListMenuTask(u8 id);	// ���j���[�I���^�X�N
static void GPL_AzukeruMenu( u8 id );	// ��������
static void GPL_StatusMenu( u8 id );	// �悳���݂�
static void GPL_CancelMenu( u8 id );	// ���ǂ�

// �|�P�������X�g�I�� -> �t�B�[���h���A -> �X�N���v�g���A
static void PokeListEndMain(u8);
static void FadeInWaitSeq(void);
static void FadeInEndCheck(u8);


//=========================================================================================
//										�O���ϐ�
//=========================================================================================
// �}���`�o�g���Q���|�P�����I�� -------------------------------------
static const MENU_ITEM	menu[] =
{
	{ pl_str_tuyosa, Status    },	// 00:�悳���݂�
	{ pl_str_sanka,  MemberIn  },	// 01:���񂩂���
	{ pl_str_sinai,  MemberOut },	// 02:���񂩂��Ȃ�
	{ str_modoru,    Return    },	// 03:���ǂ�
};

static const u8 sanka_list[] = { 1, 0, 3 };
static const u8 sinai_list[] = { 2, 0, 3 };
static const u8 egg_list[]   = { 0, 3 };

static const MenuListData MPL_MenuData[] =
{
	{ 3, 9, sanka_list },	// ���񂩂���
	{ 3, 9, sinai_list },	// ���񂩂��Ȃ�
	{ 2, 9, egg_list }		// �^�}�S
};

u8	SelectPoke[3] = {};		// �Q�����ɔԍ����o����


// ��ĉ� -----------------------------------------------------------
static const MENU_ITEM	GrowListMenu[] =
{
	{ str_azukeru,    GPL_AzukeruMenu },	// 00:��������
	{ pl_str_tuyosa,  GPL_StatusMenu  },	// 01:�悳���݂�
	{ str_modoru,     GPL_CancelMenu  },	// 02:���ǂ�
};

static const u8 menu_tbl[]     = { 0, 1, 2 };
static const u8 egg_menu_tbl[] = { 1, 2 };

static const MenuListData GPL_MenuData[] =
{
	{ 3, 9, menu_tbl },			// �m�[�}��
	{ 2, 9, egg_menu_tbl }		// �^�}�S
};


//=========================================================================================
//						�}���`�o�g���ɎQ������|�P������I��
//=========================================================================================
//----------------------
//		�����ݒ�
//----------------------
// �}���`�o�g���p
void MultiBtlSelectPokeInit(void)
{
	SelectPokeInit();
	P_LIST->btl_tower = 0;
	PokeListCall( MULTI_POKELIST_ID, SINGLE_LIST );
}

// �o�g���^���[�p
void BtlTowerSelectPokeInit(void)
{
	SelectPokeInit();
	P_LIST->btl_tower = 1;
	PokeListCall( MULTI_POKELIST_ID, SINGLE_LIST );
}

// �Q��������
static void SelectPokeInit(void)
{
	u8	i;

	for( i=0; i<3; i++ )
		SelectPoke[i] = 0;
}

//-------------------------
//		�����`�揈��
//-------------------------
u8 MultiListInit(void)
{
	u8	i=0,j=0;

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
		for( i=0; i<PokeCountMine; i++ ){
			for( j=0; j<3; j++ ){
				if( SelectPoke[j] == i+1 ){
					ListCardMoji( i, CARD_ITIBANME + j * CARD_MSG_CHAR );
					break;
				}
			}

			if( j == 3 ){
				if( BtlTowerSelectCheck( &PokeParaMine[i] ) == TRUE )
					ListCardMoji( i, CARD_SANKASINAI );
				else	ListCardMoji( i, CARD_DERAREMASEN );
			}
		}
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

//----------------------------------------------
//	�o�g���^���[�̑I���`�F�b�N
//----------------------------------------------
static u8 BtlTowerSelectCheck( PokemonParam * pp )
{
	int	i=0;	//add by soga 2002.09.10
	u16	work;

	if( PokeParaGet( pp, ID_tamago_flag ) != 0 )	return	FALSE;

	if( P_LIST->btl_tower == 0 ){
		if( PokeParaGet( pp, ID_hp ) == 0 )	return	FALSE;
		return	TRUE;
	}

	// Lv50�̕�����Lv51�ȏ�̂Ƃ�
	if( MyData.BattleTowerWork.BattleTowerLevel == 0 &&
		PokeParaGet( pp, ID_level ) >= 51 )
	{
		return	FALSE;
	}

	work = PokeParaGet( pp, ID_monsno );

//changed by soga 2002.09.10
#if 0
	if( work == MONSNO_MYUU || work == MONSNO_MYUUTUU || work == MONSNO_SEREBHI ||
		work == MONSNO_RUGIA || work == MONSNO_HOUOU ||
		work == MONSNO_KAIOOGA || work == MONSNO_GURAADON || work == MONSNO_REKKUUZA ||
		work == MONSNO_ERIIMU || work == MONSNO_YUNIIJE )
	{
		return	FALSE;
	}

	return	TRUE;
#else
	while(BattleTowerNoChallenge[i]!=0xffff){
		if(BattleTowerNoChallenge[i]==work)
			return	FALSE;
		i++;
	}
	return	TRUE;
#endif
}

//----------------------------------------------
//	�o�g���^���[�̎Q���󋵃`�F�b�N
//----------------------------------------------
static u8 BtlTowerAddPokeCheck(void)
{
	u8	i,j;

	if( P_LIST->btl_tower == 0 )	return	0xff;

	// �R�C���Ȃ�
	if( SelectPoke[2] == 0 )	return	POKE_3_INAI_MSG;

	for( i=0; i<2; i++ ){
		P_LIST->work[15] = PokeParaGet( &PokeParaMine[ SelectPoke[i]-1 ], ID_monsno );
		P_LIST->work[14] = PokeParaGet( &PokeParaMine[ SelectPoke[i]-1 ], ID_item );
		for( j=i+1; j<3; j++ ){
			// �����|�P����������
			if( P_LIST->work[15] == PokeParaGet( &PokeParaMine[ SelectPoke[j]-1 ], ID_monsno ) )
				return	ONAZI_POKE_MSG;
			// ��������������Ă���
			if( P_LIST->work[14] != 0 &&
				P_LIST->work[14] == PokeParaGet( &PokeParaMine[ SelectPoke[j]-1 ], ID_item ) )
			{
				return	ONAJI_ITEM_MSG;
			}
		}
	}

	return	0xff;
}

//----------------------------------
//		�Q������Q�����𒲂ׂ�
//----------------------------------
static u8 CheckBtlMember( u8 pos )
{
	u8	i;

	for( i=0; i<3; i++ ){
		if( SelectPoke[i] == pos )
			return	TRUE;
	}
	return	FALSE;
}

//--------------------------
//		���j���[�\��
//--------------------------
static void DrawMenuWin( u8 id )
{
//	PokeListWinClear2();
	PokeListWinFixMsgPut2( DOUSIMASUKA_MSG, 1 );

	if( BtlTowerSelectCheck( &PokeParaMine[ FPokeNo ] ) == TRUE ){
		if( CheckBtlMember(FPokeNo+1) == TRUE ){
			TaskTable[id].work[ M_MENUTYPE ] = 1;
			PokeListMenuWindowPut( 1, MPL_MenuData, menu, 0 );
		}else{
			TaskTable[id].work[ M_MENUTYPE ] = 0;
			PokeListMenuWindowPut( 0, MPL_MenuData, menu, 0 );
		}
	}else{
		TaskTable[id].work[ M_MENUTYPE ] = 2;
		PokeListMenuWindowPut( 2, MPL_MenuData, menu, 0 );
	}
}

//------------------------------
//		�|�P�����I���^�X�N
//------------------------------
void MultiPokeSelectTask(u8 id)
{
	if( !(FadeData.fade_sw) ){
		switch( PokeCursorSys2(id) ){
		case A_BUTTON:
			SePlay(SE_SELECT);
			FPokeNo = GetNowCursorPos(id);
			if( FPokeNo != 6 ){
				PokeNickNameSet( &PokeParaMine[FPokeNo], StrTempBuffer0 );
				DrawMenuWin(id);
				TaskTable[id].TaskAdrs = MultiListMenuTask;
			}else{
				TaskTable[id].TaskAdrs = Kettei;
			}
			break;

		case B_BUTTON:
			SePlay(SE_SELECT);
			SelectPokeInit();
			PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
			TaskTable[id].TaskAdrs = SelectPokeListEnd;
			break;
		}
	}
}

//------------------------------
//		���j���[�I���^�X�N
//------------------------------
static void MultiListMenuTask(u8 id)
{
	pTaskFunc	func;

	if( FadeData.fade_sw )	return;

	if( sys.Repeat & U_KEY ){
		if( NMenuVCursorPosGet() != 0 ){
			SePlay(SE_SELECT);
			NMenuVCursorSet(-1);
		}
		return;
	}

	if( sys.Repeat & D_KEY ){
		if( NMenuVCursorPosGet() != 3 ){
			SePlay(SE_SELECT);
			NMenuVCursorSet(1);
		}
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		func = ListMenuCall(
					(u8)TaskTable[id].work[M_MENUTYPE], MPL_MenuData,
					menu, NMenuVCursorPosGet() );
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON )
		Return(id);
}

//---------------------
//		�I������
//---------------------
static void SelectPokeListEnd( u8 id )
{
	if( FadeData.fade_sw == 0 ){
		MainProcChange( sys.pReturnProc );
		DelTask(id);
	}
}

//---------------------
//		�����Ă�
//---------------------
static void Kettei2( u8 id )
{
	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON )
		Return(id);
}

static void Kettei( u8 id )
{
	u8	msg;

	msg = BtlTowerAddPokeCheck();

	if( msg != 0xff ){
		PokeListWinFixMsgPut2( msg, 0 );
		TaskTable[id].TaskAdrs = Kettei2;
		return;
	}

	if( SelectPoke[0] != 0 ){
		PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
		TaskTable[id].TaskAdrs = SelectPokeListEnd;
	}else{
		SePlay( SE_HAZURE );
//		PokeListWinClear2();
		PokeListWinFixMsgPut2( TAISEN_INAI_MSG, 0 );
		TaskTable[id].TaskAdrs = Kettei2;
	}
}


//-----------------------------------
//		���j���[�@"�悳���݂�"
//-----------------------------------
// �X�e�[�^�X��ʂ��畜�A
static void ReturnSelectStatus2(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, PokeStatusBuf );
			PokeNowCursorMove3( P_LIST->task_id, 0 );
			PokeNickNameSet( &PokeParaMine[PokeStatusBuf], StrTempBuffer0 );
			FPokeNo = PokeStatusBuf;
			DrawMenuWin( P_LIST->task_id );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}
static void ReturnSelectStatus(void)
{
	FadeData.trans_stop = 1;

	PokeListCallParamSet( MULTI_POKELIST_ID, DEFAULT_LIST, MultiListMenuTask, DOUSIMASUKA_MSG );
	MainProcChange( ReturnSelectStatus2 );
}

// �X�e�[�^�X��ʂ�
static void StatusFade( u8 id )
{
	u8 temp;
	
	if(FadeData.fade_sw == 0){
		temp = ActWork[TaskTable[id].work[M_CURSOR] >> 8].work[0];
		DelTask(id);
		P_LIST->menu_flg = 1;
		PokeStatusCall2( PokeParaMine, temp, PokeCountMine-1, ReturnSelectStatus, LIST_ST );
	}
}

static void	Status(u8 id)
{
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	TaskTable[id].TaskAdrs = StatusFade;
}


//-----------------------------------
//		���j���[�@"���񂩂���"
//-----------------------------------
static void MemberIn( u8 id )
{
	u8	i;

	for( i=0; i<3; i++ ){
		if( SelectPoke[i] == 0 ){
			SelectPoke[i] = FPokeNo + 1;
			ListCardMoji( FPokeNo, CARD_ITIBANME + i * CARD_MSG_CHAR );
			if( i == 2 )	PokeSelectPosJumpKettei( id );
			Return(id);
			return;
		}
	}

	SePlay( SE_HAZURE );
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
	PokeListWinClear2();
	PokeListScrMsgSet( msg_poke_in_over, 1 );
	TaskTable[id].TaskAdrs = MemberOver;
}

// �Q�����I�[�o�[
static void MemberOver( u8 id )
{
	if( ListMsgStatus != 1 ){
		if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
			NMenuBoxClear( 0, 14, 29, 19);
			Return(id);
		}
	}
}


//-----------------------------------
//		���j���[�@"���񂩂��Ȃ�"
//-----------------------------------
static void MemberOut( u8 id )
{
	u8	i;

	for( i=0; i<3; i++ ){
		if( SelectPoke[i] == FPokeNo + 1 ){
			SelectPoke[i] = 0;
			switch( i ){
			case 0:
				SelectPoke[0] = SelectPoke[1];
				SelectPoke[1] = SelectPoke[2];
				SelectPoke[2] = 0;
				break;
			case 1:
				SelectPoke[1] = SelectPoke[2];
				SelectPoke[2] = 0;
			}
			break;
		}
	}

	ListCardMoji( FPokeNo, CARD_SANKASINAI );
	if( SelectPoke[0] != 0 )
		ListCardMoji( SelectPoke[0]-1, CARD_ITIBANME );
	if( SelectPoke[1] != 0 )
		ListCardMoji( SelectPoke[1]-1, CARD_NIBANME );
	Return(id);
}


//-------------------------------
//		���j���[�@"���ǂ�"
//-------------------------------
static void Return2(u8 id)
{
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
//	PokeListWinClear2();
	PokeListWinFixMsgPut2( ERANDE_MSG, 0 );
	TaskTable[id].TaskAdrs = MultiPokeSelectTask;
}

static void	Return(u8 id)
{
	SePlay(SE_SELECT);
	Return2(id);
}



//=======================================================================================
//						�}���`�o�g���O�̃J�[�h�X�N���[���\��
//=======================================================================================

//-------------------------------------------------------------------------------
#ifdef	PM_DEBUG

void DebugMultiPokeParaSet(void)
{
	u8	i;

	memset( (MultiPokeListData *)MPLD, 0, sizeof(MultiPokeListData) * 3 );

	for( i=0; i<3; i++ ){
		MPLD[i].monsno = PokeParaGet( &PokeParaMine[3+i], ID_monsno_egg );
		if( MPLD[i].monsno != 0 ){
			MPLD[i].level        = PokeParaGet( &PokeParaMine[3+i], ID_level );
			MPLD[i].hp           = PokeParaGet( &PokeParaMine[3+i], ID_hp );
			MPLD[i].hpmax        = PokeParaGet( &PokeParaMine[3+i], ID_hpmax );
			MPLD[i].condition    = PokeParaGet( &PokeParaMine[3+i], ID_condition );
			MPLD[i].itemno       = PokeParaGet( &PokeParaMine[3+i], ID_item );
			MPLD[i].personal_rnd = PokeParaGet( &PokeParaMine[3+i], ID_personal_rnd );
			MPLD[i].sex          = PokeSexGet( &PokeParaMine[3+i] );
			PokeParaGet( &PokeParaMine[3+i], ID_nickname, MPLD[i].nickname );
		}
	}
}

#endif	PM_DEBUG

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

//------------------------
//		�����`�揈��
//------------------------
u8	ScrollInListInit(void)
{
	switch( P_LIST->work[0] ){
	case 0:
		PokeMineIconPut( P_LIST->task_id );
		P_LIST->work[0]++;
		break;
	case 1:
		ItmeIconCellPalSet();
		P_LIST->work[0]++;
		break;
	case 2:
		PokeMineItemIconPut( P_LIST->task_id );
		P_LIST->work[0]++;
		break;
	case 3:
		PokeMineParamPut();
		P_LIST->work[0]++;
		break;
	case 4:
		ScrollInPokeListBGCardInit();
		return	TRUE;
	}
	return	FALSE;
}


//---------------------------------------------
//		�����̃|�P�����̃A�C�R����\��
//---------------------------------------------
static void PokeMineIconPut( u8 id )
{
	u8 i;

	for( i=0; i<3; i++ ){

		if( PokeParaGet( &PokeParaMine[i], ID_monsno ) != 0 )
			PataChrActorSet2( id, i, 3, &PokeParaMine[i] );

		if( MPLD[i].monsno != 0 ){
			PataChrActorSetParam( id, i+3, 3, &MPLD[i] );
			ScrollInActXPosPlus( id, i+3 );
		}
	}
}

static void PokeMineItemIconPut( u8 id )
{
	u8 i;

	for( i=0; i<3; i++ ){

		if( PokeParaGet( &PokeParaMine[i], ID_monsno ) != 0 ){
			ItemIconActSetParam( id, i, PokeParaGet( &PokeParaMine[i], ID_item ) );
		}

		if( MPLD[i].monsno != 0 ){
			ItemIconActSetParam( id, i+3, MPLD[i].itemno );
		}
	}
}

//---------------------------------------------
//		�����̃|�P�����̃p�����[�^��\��
//---------------------------------------------
static void PokeMineParamPut(void)
{
	u8 i, con;
	
	for( i=0; i<3; i++){
		if( PokeParaGet( &PokeParaMine[i], ID_monsno ) != 0 ){
			ListPokeMaxHpPut2( i, 3, &PokeParaMine[i] );
			ListPokeHpPut2( i, 3, &PokeParaMine[i] );

			con = GetPokeCondition( &PokeParaMine[i] );
			if( con != ST_NORMAL && con != ST_POKERUS )
				PokeConditionPutCardMoji( i, 3, con-1 );
			else
				ListPokeLvPut2( i, 3, &PokeParaMine[i] );

			PokeListSexPut( i, 3, &PokeParaMine[i] );
			ListPokeNamePut2( i, 3, &PokeParaMine[i] );
			ListGaugeScreenSet2( i, 3, &PokeParaMine[i] );
		}
	}
}

//---------------------------
//		�X�N���[������
//---------------------------
void ScrollInPokeListTask( u8 id )
{
	if(FadeData.fade_sw == 0){
		TaskTable[id].work[0] = 30;
		SetPokeIconScroll( id, MPLD[0].monsno, 0 );
		SetPokeIconScroll( id, MPLD[1].monsno, 1 );
		SetPokeIconScroll( id, MPLD[2].monsno, 2 );
		TaskTable[id].TaskAdrs = ScrollInPokeListTask2;
		P_LIST->card_flg = 1;
	}

}
static void ScrollInPokeListTask2( u8 id )
{
	ScrollInPokeListCardMove( id, MPLD[1].monsno, MPLD[2].monsno );

	if( TaskTable[id].work[0] == 0 ){
		TaskTable[id].TaskAdrs = SetScrollPokeParam;
		P_LIST->card_flg = 2;
		SePlay( SE_W231 );
	}
}

//---------------------------
//		�p�����[�^�\��
//---------------------------
static void SetScrollPokeParam( u8 id )
{
	u8	i;
	u8	con;

	for( i=0; i<3; i++){
		if( MPLD[i].monsno != 0 ){
			ListPokeMaxHpPutParam( i+3, 3, MPLD[i].hpmax );
			ListPokeHpPutParam( i+3, 3, MPLD[i].hp );

			if( MPLD[i].hp == 0 )	con = ST_HINSI;
			else	con = GetPokeConditionParam( MPLD[i].condition );
			if( con != ST_NORMAL )
				PokeConditionPutCardMoji( i+3, 3, con-1 );
			else	ListPokeLvPutParam( i+3, 3, MPLD[i].level );

			PokeListSexPutParam( MPLD[i].monsno, MPLD[i].sex, 3, i+3, MPLD[i].nickname );

			PM_strcpy( StrTempBuffer0, MPLD[i].nickname );
			NickNameCutOff( StrTempBuffer0 );

			ListPokeNamePutParam( i+3, 3, StrTempBuffer0 );
			ListGaugeScreenSetParam( i+3, 3, MPLD[i].hp, MPLD[i].hpmax );
		}
	}

	TaskTable[id].TaskAdrs = ScrollInKeyWaitTask;
	TaskTable[id].work[0] = 0;
}

//-------------------------
//		�L�[���͑҂�
//-------------------------
static void ScrollInKeyWaitTask( u8 id )
{
	TaskTable[id].work[0]++;

	if( TaskTable[id].work[0] != 256 )	return;

	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	TaskTable[id].TaskAdrs = SelectPokeListEnd;
}


//========================================================================================
//								��ĉ��|�P�������X�g
//========================================================================================
//-------------------------
//		�����`�揈��
//-------------------------
u8 GrowListInit(void)
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
		ListCardSexPut();
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

//-------------------------
//		���j���[�쐬
//-------------------------
static void GPL_MakeMenu( u8 id )
{
	// �^�}�S�ȊO
	if( PokeParaGet( &PokeParaMine[ FPokeNo ], ID_tamago_flag ) == 0 ){
		TaskTable[id].work[ M_MENUTYPE ] = 0;
		PokeListMenuWindowPut( 0, GPL_MenuData, GrowListMenu, 0 );
	// �^�}�S
	}else{
		TaskTable[id].work[ M_MENUTYPE ] = 1;
		PokeListMenuWindowPut( 1, GPL_MenuData, GrowListMenu, 0 );
	}
}

//------------------------
//		�|�P�����I��
//------------------------
void GrowPokeSelectTask( u8 id )
{
	if( !(FadeData.fade_sw) ){
		switch( PokeCursorSys(id) ){
		case A_BUTTON:
			SePlay(SE_SELECT);
			FPokeNo = GetNowCursorPos(id);
			PokeNickNameSet( &PokeParaMine[FPokeNo], StrTempBuffer0 );
			GPL_MakeMenu(id);
//			PokeListMenuWindowPut( 0, &GPL_MenuData, GrowListMenu, 0 );
			TaskTable[id].TaskAdrs = GrowListMenuTask;
			break;

		case B_BUTTON:
			SePlay(SE_SELECT);
			FPokeNo = 0xff;
			ScriptParameter0 = FPokeNo;
			PokeListEnd_ContinueScript(id);
			break;
		}
	}
}

//------------------------------
//		���j���[�I���^�X�N
//------------------------------
static void GrowListMenuTask(u8 id)
{
	pTaskFunc	func;

	if( FadeData.fade_sw )	return;

	if( sys.Repeat & U_KEY ){
		if( NMenuVCursorPosGet() != 0 ){
			SePlay(SE_SELECT);
			NMenuVCursorSet(-1);
		}
		return;
	}

	if( sys.Repeat & D_KEY ){
		if( NMenuVCursorPosGet() != 3 ){
			SePlay(SE_SELECT);
			NMenuVCursorSet(1);
		}
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		func = ListMenuCall(
					(u8)TaskTable[id].work[M_MENUTYPE], GPL_MenuData,
					GrowListMenu, NMenuVCursorPosGet() );
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON )
		GPL_CancelMenu(id);
}

//---------------------------------
//		���j���[ "��������"
//---------------------------------
static void GPL_AzukeruMenu( u8 id )
{
	ScriptParameter0 = FPokeNo;
	PokeListEnd_ContinueScript(id);
}

//-------------------------------------
//		���j���[ "�悳���݂�"
//-------------------------------------
// �X�e�[�^�X��ʂ��畜�A
static void ReturnGrowStatus2(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, PokeStatusBuf );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			PokeNickNameSet( &PokeParaMine[PokeStatusBuf], StrTempBuffer0 );
			FPokeNo = PokeStatusBuf;
			GPL_MakeMenu( P_LIST->task_id );
//			PokeListMenuWindowPut( 0, &GPL_MenuData, GrowListMenu, P_LIST->menu_flg-1 );
//			PokeListMenuWindowPut( 0, &GPL_MenuData, GrowListMenu, 0 );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}
static void ReturnGrowStatus(void)
{
	FadeData.trans_stop = 1;

	PokeListCallParamSet( GROW_POKELIST_ID, DEFAULT_LIST, GrowListMenuTask, DOUSIMASUKA_MSG );
	MainProcChange( ReturnGrowStatus2 );
}

static void GPL_StatusFade( u8 id )
{
	u8	temp;

	if(FadeData.fade_sw == 0){
		temp = ActWork[TaskTable[id].work[M_CURSOR] >> 8].work[0];
		DelTask(id);
		P_LIST->menu_flg = 1;
		PokeStatusCall2( PokeParaMine, temp, PokeCountMine-1, ReturnGrowStatus, LIST_ST );
	}
}

static void GPL_StatusMenu( u8 id )
{
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	TaskTable[id].TaskAdrs = GPL_StatusFade;
}

//---------------------------------
//		���j���[ "���ǂ�"
//---------------------------------
static void GPL_CancelMenu( u8 id )
{
	SePlay(SE_SELECT);
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
//	PokeListWinClear2();
	PokeListWinFixMsgPut2( AZUKERU_POKE_MSG, 0 );
	TaskTable[id].TaskAdrs = GrowPokeSelectTask;
}


//--------------------------------------------------------------------
//		�|�P�������X�g�I�� -> �t�B�[���h���A -> �X�N���v�g���A
//--------------------------------------------------------------------
// �t�F�[�h�A�E�g
void PokeListEnd_ContinueScript( u8 id )
{
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	TaskTable[id].TaskAdrs = PokeListEndMain;
}

static void PokeListEndMain( u8 id )
{
	if( !(FadeData.fade_sw) ){
		pFieldRecoverFunc = FadeInWaitSeq;
		MainProcChange( FieldMainRecover );
		DelTask(id);
	}
}

// �t�F�[�h�C��
static void FadeInWaitSeq(void)
{
//	PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
	FieldFadeBlackInSet();
	AddTask( FadeInEndCheck, 10 );
}

static void FadeInEndCheck( u8 id )
{
//	if( !(FadeData.fade_sw) ){
	if( FieldFadeinCheck() == 1 ){
		DelTask(id);
		ResetForceEvent();
		ContinueFieldScript();
	}
}
