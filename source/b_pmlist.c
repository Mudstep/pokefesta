//=========================================================================
//
//	b_pmlist.c
//	�퓬��ʂł̃|�P�������X�g�Ăяo���C���^�[�t�F�C�X
//
//	2001.06.28
//=========================================================================

#include "common.h"
#include "print.h"
#include "pokelist.h"
#include "task.h"
#include "palanm.h"
#include "actor.h"
#include "actanm.h"
#include "intr.h"

#include "poketool.h"
#include "cli_def.h"
#include "client.h"
#include "p_status.h"
#include "p_status.h"
#include "b_return.h"
#include "menu.h"
#include "b_pmlist.h"
#include "bss.h"
#include "server.h"
#include "mail.h"
#include "wazatool.h"
#include "pokeicon.h"
#include "window.h"
#include "mus_tool.h"
#include "itemtool.h"
#include "message.h"

#include "fight.h"
#include "field.h"


//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
void BattlePokeListMain(u8 id);
static void BattlePokeListEndWait(u8 id);
static void PokeStatusMenu(u8 id);
static void ChangePokeMenu(u8 id);
static void CancelMenu(u8 id);
static void PokeListMenuMain(u8 id);
static void BattlePokeListEnd(u8 id);
static void BPL_ItemReturn(u8 id);
static void BPL_ItemBagReturn(u8 id);
static void ReturnBattleStatusMain(void);
void ListRowInitCommon(u8 *listbuf, u8 side);
void ListRowInitCommonClient(u8 *listbuf, u8 side, u8 client_no);


//=========================================================================
//	�O���֐�
//=========================================================================
extern void BtlRuckInit(void);

//=========================================================================
//	�O���[�o���ϐ�
//=========================================================================
u8 ListRow[3] = {};		//�߹�݂̕��я�(�퓬�ɏo�Ă��߹�݂��O�ɏo��̂ŕ��т�ێ�����
u8 PokeListType = 0;

extern u16	ReturnItemNo;	//2002/03/18 by nakahiro

//=========================================================================
//	�f�[�^
//=========================================================================
#define MENU_MSG_LEN	9		//�ƭ�ү���ޒ�
#define HINSI_MSG_LEN	9

static const MENU_ITEM	BattleListMenu[] =
{
	{ pl_str_tuyosa,	PokeStatusMenu },	// 00:�悳���݂�
	{ str_yameru,	    CancelMenu     },	// 01:���ǂ�
	{ pl_str_irekaeru,	ChangePokeMenu },	// 02:���ꂩ����
	{ pl_str_sentou,	ChangePokeMenu },	// 03:�퓬�ɂ���
};

static const u8 NormalMenu[]  = { 2, 0, 1 };
static const u8 HinsiMenu[]   = { 3, 0, 1 };
static const u8 EggMenu[]     = { 0, 1 };

static const MenuListData BPL_MenuData[] = 
{
	{ 3, MENU_MSG_LEN,  NormalMenu  },		// 00:�ʏ�
	{ 3, HINSI_MSG_LEN, HinsiMenu   },		// 01:�m��
	{ 2, MENU_MSG_LEN,  EggMenu     },		// 02:�^�}�S
};


//=========================================================================
//
//	�|�P�������X�g�\��
//
//=========================================================================

//----------------------------------------------------------
//	�o�g�����߹��ؽĂŎg�p����ܰ��̏�����
//----------------------------------------------------------
void BattleListWorkInit(void)
{
	ListRowInitCommon(ListRow, GetTeamPos());
}

//----------------------------------------------------------
//	�T�[�o�[�ŕێ�����ListRow��ܰ�������������
// ����: side = 0:�O�q	1:��q
//----------------------------------------------------------
void ServerListRowInit(u8 client_no, u8 side)
{
	ListRowInitCommonClient(&UserWork[SERVER_LIST_ROW+client_no*3], side,client_no);
}

void ListRowInitCommon(u8 *listbuf, u8 side)
{
	int i,j;
	u8	tbl[6];
	
	if(MultiCheck() == TRUE){
		if(side != 0){
			listbuf[0] = ( 3 << 4 );
			listbuf[1] = ( 4 << 4 ) | 5;
			listbuf[2] = ( 1 << 4 ) | 2;
		}else{
			listbuf[0] = 3;
			listbuf[1] = ( 1 << 4 ) | 2;
			listbuf[2] = ( 4 << 4 ) | 5;
		}
	}else{

		if( FightCheck() == 0 ){
			j = 1;
			tbl[0] = SelMonsNo[ ClientNoGet( BSS_CLIENT_MINE ) ];

			for( i=0; i<6; i++ ){
				if( i != tbl[0] ){
					tbl[j] = i;
					j++;
				}
			}
		}else{
			j = 2;
			tbl[0] = SelMonsNo[ ClientNoGet( BSS_CLIENT_MINE ) ];
			tbl[1] = SelMonsNo[ ClientNoGet( BSS_CLIENT_MINE2 ) ];
			for( i=0; i<6; i++ ){
				if( i != tbl[0] && i != tbl[1] ){
					tbl[j] = i;
					j++;
				}
			}
		}

		for( i=0; i<3; i++ ){
			listbuf[i] = ( tbl[i<<1] << 4 ) | ( tbl[(i<<1)+1] );
		}
	}
}

void ListRowInitCommonClient(u8 *listbuf, u8 side,u8 clientno)
{
	int i,j;
	u8	tbl[6];
	u8	no1,no2;

	if(MineEnemyCheck(clientno)==SIDE_MINE){
		no1=ClientNoGet(BSS_CLIENT_MINE);
		no2=ClientNoGet(BSS_CLIENT_MINE2);
	}
	else{
		no1=ClientNoGet(BSS_CLIENT_ENEMY);
		no2=ClientNoGet(BSS_CLIENT_ENEMY2);
	}
	
	if(MultiCheck() == TRUE){
		if(side != 0){
			listbuf[0] = ( 3 << 4 );
			listbuf[1] = ( 4 << 4 ) | 5;
			listbuf[2] = ( 1 << 4 ) | 2;
		}else{
			listbuf[0] = 3;
			listbuf[1] = ( 1 << 4 ) | 2;
			listbuf[2] = ( 4 << 4 ) | 5;
		}
	}else{

		if( FightCheck() == 0 ){
			j = 1;
			tbl[0] = SelMonsNo[ no1 ];

			for( i=0; i<6; i++ ){
				if( i != tbl[0] ){
					tbl[j] = i;
					j++;
				}
			}
		}else{
			j = 2;
			tbl[0] = SelMonsNo[ no1 ];
			tbl[1] = SelMonsNo[ no2 ];
			for( i=0; i<6; i++ ){
				if( i != tbl[0] && i != tbl[1] ){
					tbl[j] = i;
					j++;
				}
			}
		}

		for( i=0; i<3; i++ ){
			listbuf[i] = ( tbl[i<<1] << 4 ) | ( tbl[(i<<1)+1] );
		}
	}
}

//----------------------------------------------------------
//	�u�ӂ��Ƃ΂��v����ListRow�̒l���ύX����鎞�ɃT�[�o�[����Ă΂��
//	�}���`�v���C�ʐM���݂̂̏���	2002.05.18(�y) by matsuda
//----------------------------------------------------------
void ListRowExChange(u8 client_no, u8 selmons_no, u8 side)
{
	int i, j = 0;
	u8 buff[TEMOTI_POKEMAX];
	u8 *listrow;
	u8 top, temp = 0;
	
	if(MultiCheck() == FALSE)
		return;
	
	listrow = &UserWork[SERVER_LIST_ROW+client_no*3];
	
	for(i = 0, j = 0; i < TEMOTI_POKEMAX/2; i++){
		buff[j++] = listrow[i] >> 4;
		buff[j++] = listrow[i] & 0x0f;
	}
	
	top = buff[side];
	for(i = 0; i < TEMOTI_POKEMAX; i++){
		if(buff[i] == selmons_no){
			temp = buff[i];
			buff[i] = top;
			break;
		}
	}
	if(i == TEMOTI_POKEMAX)
		return;
	buff[side] = temp;

	listrow[0] = (buff[0] << 4) | buff[1];
	listrow[1] = (buff[2] << 4) | buff[3];
	listrow[2] = (buff[4] << 4) | buff[5];
}

//----------------------------------------------------------
//	���وʒu(�\���ʒu)����APokeParaMine�̉��Ԗڂ��߹�݂����邩�擾����
//----------------------------------------------------------
u8 GetListRow(u8 pos)
{
	u8 kisuu = 0;
	u8 ret;
	
	if(pos & 1)
		kisuu = 1;
	pos >>= 1;
	if(kisuu)
		ret = ListRow[pos] & 0x0f;
	else
		ret = ListRow[pos] >> 4;
	return ret;
}

//----------------------------------------------------------
//	���وʒu(�\���ʒu)�ɁAPokeParaMine�̉��Ԗڂ��߹�݂����邩�Z�b�g����
//----------------------------------------------------------
static void SetListRow(u8 pos, u8 new_pos)
{
	u8 kisuu = 0;
	
	if(pos & 1)
		kisuu = 1;
	pos >>= 1;
	if(kisuu)
		ListRow[pos] = (ListRow[pos] & 0xf0) | new_pos;
	else
		ListRow[pos] = (ListRow[pos] & 0x0f) | (new_pos<<4);
}

//----------------------------------------------------------
//	2�¶��وʒu(�\���ʒu)���w�肵�A���g�����ւ���
//----------------------------------------------------------
//static void ChangeListRow(u8 pos1, u8 pos2)
void ChangeListRow(u8 pos1, u8 pos2)
{
	u8 data1, data2;
	
	data1 = GetListRow(pos1);
	data2 = GetListRow(pos2);
	SetListRow(pos1, data2);
	SetListRow(pos2, data1);
}

//----------------------------------------------------------
//	PokeParaMine�̔ԍ������āA�����߹�݂��ǂ̈ʒu�ɂ��邩��Ԃ�
//----------------------------------------------------------
u8 SearchListRow(u8 pokepara_pos)
{
	u8 i, j;
	
	for(i = 0, j = 0; i < 3; i++)
	{
		if((ListRow[i] >> 4) == pokepara_pos)
			return j;
		j++;
		if((ListRow[i] & 0x0f) == pokepara_pos)
			return j;
		j++;
	}
	return 0;
}

//----------------------------------------------------------
//	ListRow�̕��я������Ұ������ёւ���
//----------------------------------------------------------
static void ListRowPokeParaSort(void)
{
	u8	i,poke;
	
	memcpy( P_LIST->TempParam, PokeParaMine, sizeof(PokemonParam) * 6 );
	for( i=0; i<6; i++ ){
		poke = SearchListRow(i);
		memcpy( &PokeParaMine[poke], &P_LIST->TempParam[i], sizeof(PokemonParam) );
	}
}

//----------------------------------------------------------
//	���̕��тɖ߂�
//----------------------------------------------------------
static void ListRowPokeParaSortBack(void)
{
	PokemonParam TempParam[STOCK_POKE_MAX];
	u8 i, poke;
	
	memcpy(TempParam, PokeParaMine, sizeof(PokemonParam) * STOCK_POKE_MAX);

	for( i=0; i<TEMOTI_POKEMAX; i++ )
	{
		poke = GetListRow(i);
		memcpy(&PokeParaMine[poke], &TempParam[i], sizeof(PokemonParam));
	}
}

//---------------------------------------------------------------
//	����ȋZ�i�ق���Ȃǁj�œ���ւ�����Ƃ���ListRow���Đݒ�
//---------------------------------------------------------------
void RemakeListRowNext(void)
{
	u8	no,zero;
	u8	i;

	for( i=1; i<6; i++ ){
		no = GetListRow(i);
		if( PokeParaGet( &PokeParaMine[ no ], ID_monsno ) != 0 &&
			PokeParaGet( &PokeParaMine[ no ], ID_hp ) != 0 )
		{
			zero = GetListRow(0);
			ChangeListRow( 0, i );
			SwapPokemon( &PokeParaMine[zero], &PokeParaMine[no] );
			break;
		}
	}
}


//=========================================================================
//	
//=========================================================================

//-------------------------------------------------------------------------
//	�|�P�������X�g��ʌĂяo��
//-------------------------------------------------------------------------
void BattlePokeListInit( u8 list_type )
{
	FadeData.trans_stop = 1;
	PokeListType = list_type;

	PushBattleWork();	//2001.07.13(��)
	ListRowPokeParaSort();	//���Ұ������ёւ� add 2001.11.06(��)
	PokeListCall( BATTLE_POKELIST_ID, DEFAULT_LIST );
}

//-------------------------------------------------------------------------
//	�o�b�O��ʂ���̃|�P�������X�g��ʌĂяo��
//-------------------------------------------------------------------------
void BagBattlePokeListInit(void)
{
	BattlePokeListInit(PLT_ITEM);
};

//-------------------------
//		�����`�揈��
//-------------------------
u8 BtlListInit(void)
{
	switch( P_LIST->work[0] ){
	case 0:
		if( MultiCheck() == TRUE ){
			if( P_LIST->work[1] != TEMOTI_POKEMAX ){
				PataChrActorSet( P_LIST->task_id, P_LIST->work[1], 
								 &PokeParaMine[ P_LIST->work[1] ] );
				P_LIST->work[1]++;
			}else{
				P_LIST->work[1] = 0;
				P_LIST->work[0]++;
			}
		}else{
			if( P_LIST->work[1] < TEMOTI_POKEMAX ){
				PataChrActorSet( P_LIST->task_id, P_LIST->work[1],
								 &PokeParaMine[ P_LIST->work[1] ] );
				P_LIST->work[1]++;
			}else{
				P_LIST->work[1] = 0;
				P_LIST->work[0]++;
			}
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
		PokeHpPutAll();
		P_LIST->work[0]++;
		break;

	case 7:
		PokeMaxHpPutAll();
		P_LIST->work[0]++;
		break;

	case 8:
		PokeListGaugeSetAll();
		P_LIST->work[0]++;
		break;

	case 9:
		if( PokeListBGCardInit2( P_LIST->work[1] ) == 1 ){
			P_LIST->work[1] = 0;
			P_LIST->work[0]++;
		}else	P_LIST->work[1]++;
		break;

	case 10:
		if( PokeListType == PLT_ITEM ){
			if( ItemRecoverCheck( ReturnItemNo ) == ITEM_TYPE_ALLDETH_RCV )
				P_LIST->init_msg = MSG_NOT_PUT;
			else	P_LIST->init_msg = TUKAIMASUKA_MSG;
		}
		return	TRUE;
	}

	return	FALSE;
}

//---------------------------
//		���j���[�쐬
//---------------------------
static void MenuListCheck( u8 id, u8 pos )
{
	if( PokeParaGet( &PokeParaMine[ pos ], ID_tamago_flag ) == 0 ){
		if( PokeListType == PLT_B_USELESS ){
			TaskTable[ P_LIST->task_id ].work[M_MENUTYPE]      = LS_HINSI;
			TaskTable[ P_LIST->task_id ].work[M_MENUTYPE_TEMP] = LS_HINSI; 
		}else{
			TaskTable[ P_LIST->task_id ].work[M_MENUTYPE]      = LS_BATTLE;
			TaskTable[ P_LIST->task_id ].work[M_MENUTYPE_TEMP] = LS_BATTLE;
		}
	}else{
		TaskTable[ P_LIST->task_id ].work[M_MENUTYPE]      = LS_EGG;
		TaskTable[ P_LIST->task_id ].work[M_MENUTYPE_TEMP] = LS_EGG;
	}
	PokeListMenuWindowPut(
		TaskTable[id].work[M_MENUTYPE], BPL_MenuData, BattleListMenu, 0 );
}

//=========================================================================
//	�|�P�������X�g  �^�X�N���C��
//=========================================================================
void BattlePokeListMain(u8 id)
{
	if( FadeData.fade_sw )	return;

	if( PokeListType == PLT_ITEM &&
		ItemRecoverCheck( ReturnItemNo ) == ITEM_TYPE_ALLDETH_RCV )
	{
		PokeListItemUseFunc( id, ReturnItemNo, BPL_ItemReturn );
		return;
	}

	switch( PokeCursorSys(id) ){
	case A_BUTTON:
		if( PokeListType == PLT_ITEM ){
			if( PokeParaGet( &PokeParaMine[ GetNowCursorPos(id) ], ID_tamago_flag ) != 0 ){
				SePlay(SE_HAZURE);
				break;
			}
			PokeListWinClear2();
			PokeListItemUseFunc( id, ReturnItemNo, BPL_ItemReturn );	// 2002/03/18 by nakahiro
		}else{
			SePlay(SE_SELECT);
			PokeNickNameSet( &PokeParaMine[ GetNowCursorPos(id) ], StrTempBuffer0 );
//			PokeListMenuWindowPut( TaskTable[id].work[M_MENUTYPE], 
//				BPL_MenuData, BattleListMenu, 0 );
			MenuListCheck( id, GetNowCursorPos(id) );
			ChangeTask(id, PokeListMenuMain, BattlePokeListMain);
		}
		break;
	case B_BUTTON:
		if( PokeListType == PLT_B_USELESS ){
			SePlay( SE_HAZURE );
			break;
		}

		SePlay( SE_SELECT );
		if( PokeListType == PLT_ITEM ){
			ListStatus = 0;
			TaskTable[id].TaskAdrs = BPL_ItemReturn;
		}else{
			ListStatus = P_NORMAL;
			TaskTable[id].TaskAdrs = BattlePokeListEnd;
		}
		break;
	}
}

//----------------------------------------------------------
//	�߹��ؽďI��
//----------------------------------------------------------
static void BattlePokeListEnd(u8 id)
{
	PaletteFadeReq(0xffffffff, 0, 0, 16, 0);
	//LightFadeReq(N_to_B);
	//HardFadeReq(0xff, 0, 0, 16, 0);
	TaskTable[id].TaskAdrs = BattlePokeListEndWait;
}

//----------------------------------------------------------
//	�߹��ؽďI���A̪��ނ��I���܂őҋ@
//----------------------------------------------------------
static void BattlePokeListEndWait(u8 id)
{
	if(FadeData.fade_sw == 0)
	{
		ListRowPokeParaSortBack();
		DelTask(id);
		MainProcChange( ClientPokeListReturn );
	}
}

//----------------------------------------------------------
//	���юg�p���̏I���߂��
//----------------------------------------------------------
static void BPL_ItemReturn(u8 id)
{
	if(ListStatus)
		BattlePokeListEnd(id);
	else
	{	//���юg���Ȃ������B�o�b�O��ʂɖ߂�
//		LightFadeReq(N_to_B);
		PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
		TaskTable[id].TaskAdrs = BPL_ItemBagReturn;
	}
}

//-- ���т��g���Ȃ������̂Ńo�b�O��ʂɖ߂� --//
static void BPL_ItemBagReturn(u8 id)
{
	if( FadeData.fade_sw )	return;

	ListRowPokeParaSortBack();
	DelTask(id);
	BtlRuckInit();
}


//=========================================================================
//	�߹�ݽð����ʂ���߂��Ă����Ƃ���sys.pMainProc�ɾ�Ă����֐�
//=========================================================================
static void ReturnBattleStatus(void)
{
	FadeData.trans_stop = 1;
	PokeListCallParamSet( BATTLE_POKELIST_ID, DEFAULT_LIST, BattlePokeListMain, DOUSIMASUKA_MSG );
	MainProcChange( ReturnBattleStatusMain );
}

static void ReturnBattleStatusMain(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, PokeStatusBuf );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			PokeNickNameSet( &PokeParaMine[ PokeStatusBuf ], StrTempBuffer0 );
//			PokeListMenuWindowPut( 0, BPL_MenuData, BattleListMenu, P_LIST->menu_flg );
//			PokeListMenuWindowPut( 0, BPL_MenuData, BattleListMenu, 0 );
			MenuListCheck( P_LIST->task_id, PokeStatusBuf );
			ChangeTask( P_LIST->task_id, PokeListMenuMain, BattlePokeListMain );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}


//=========================================================================
//	���j���[���C�����[�v
//=========================================================================
static void PokeListMenuMain(u8 id)
{
	pTaskFunc ptask;
	
	if( FadeData.fade_sw )	return;

	if( sys.Repeat & U_KEY ){
		SePlay(SE_SELECT);
		NMenuVCursorSet(-1);
		return;
	}

	if( sys.Repeat & D_KEY ){
		SePlay(SE_SELECT);
		NMenuVCursorSet(1);
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		ptask = ListMenuCall(
						TaskTable[id].work[M_MENUTYPE],
						BPL_MenuData, BattleListMenu, NMenuVCursorPosGet() );
		ptask(id);
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay(SE_SELECT);
		CancelMenu(id);
	}
}



static void TalkEndCancelSeq(u8 id)
{
	if(ListMsgStatus == 0)
		CancelMenu(id);
}


//=========================================================================
//	���j���[�֐�
//=========================================================================
//----------------------------------------------------------
//	�悳���݂�
//----------------------------------------------------------
static void FadeWaitStatusCall(u8 id)
{
	u8 temp;

	temp = GetNowCursorPos(id);

	if(FadeData.fade_sw == 0)
	{
		DelTask(id);
		P_LIST->menu_flg = 1;
		PokeStatusCall2( PokeParaMine, temp, PokeCountMine-1, ReturnBattleStatus, BTL_WAZA );
	}
}
static void PokeStatusMenu(u8 id)
{
	u8 temp;

	temp = GetNowCursorPos(id);
	TaskTable[id].TaskAdrs = FadeWaitStatusCall;
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
//	HardFadeReq(0xff, 0, 0, 16, 0);
}


//----------------------------------------------------------
//	���ꂩ����
//----------------------------------------------------------
static void ChangePokeMenu(u8 id)
{
	u8 i=0;
	u8 cursor_pos, client_no;
	u8 sel_pos;
	
	PokeListMenuWinClear(TaskTable[id].work[M_MENUTYPE], BPL_MenuData);
	cursor_pos = GetNowCursorPos(id);

	if( MultiCheck() == TRUE ){
		if( cursor_pos == 1 || cursor_pos == 4 || cursor_pos == 5 ){
			PokeListWinClear2();
//			PokeParaGet( &PokeParaMine[cursor_pos], ID_oyaname, StrTempBuffer0 );
			PM_strcpy( StrTempBuffer0, TeamTrainerNameGet() );
			PM_MsgExpand( MsgExpandBuffer, msg_poke_chg_error04 );
			PokeListScrMsgSet( MsgExpandBuffer, 0 );
			TaskTable[id].TaskAdrs = TalkEndCancelSeq;
			return;
		}
	}

/*
�D��x
�m�����u�o�Ă��܂��v���^�}�S���I���ς݁�����
*/
	// �m��
	if( PokeParaGet( &PokeParaMine[cursor_pos], ID_hp ) == 0 ){
		PokeListWinClear2();
		PokeNickNameSet( &PokeParaMine[ cursor_pos ], StrTempBuffer0 );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_chg_error03 );
		PokeListScrMsgSet( MsgExpandBuffer, 0 );
		TaskTable[id].TaskAdrs = TalkEndCancelSeq;
		return;
	}

	//�u�o�Ă��܂��v
	for( i=0; i<ClientSetMax; i++ ){
		if( MineEnemyCheck(i) == SIDE_MINE ){
			if( GetListRow(cursor_pos) == SelMonsNo[i] ){
				PokeListWinClear2();
				PokeNickNameSet( &PokeParaMine[ cursor_pos ], StrTempBuffer0 );
				PM_MsgExpand( MsgExpandBuffer, msg_poke_chg_error01 );
				PokeListScrMsgSet(MsgExpandBuffer, 0);
				TaskTable[id].TaskAdrs = TalkEndCancelSeq;
				return;
			}
		}
	}

	// �^�}�S
	if( PokeParaGet( &PokeParaMine[cursor_pos], ID_tamago_flag ) != 0 ){
		PokeListWinClear2();
		PM_MsgExpand( MsgExpandBuffer, msg_poke_chg_error05 );
		PokeListScrMsgSet( MsgExpandBuffer, 0 );
		TaskTable[id].TaskAdrs = TalkEndCancelSeq;
		return;
	}

	// �I���ς�
	if( GetListRow(cursor_pos) == UserWork[NO_SEL_MONS] ){
		PokeListWinClear2();
		PokeNickNameSet( &PokeParaMine[ cursor_pos ], StrTempBuffer0 );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_chg_error02 );
		PokeListScrMsgSet( MsgExpandBuffer, 0 );
		TaskTable[id].TaskAdrs = TalkEndCancelSeq;
		return;
	}

	// ����
	if( PokeListType == PLT_NOT_CHANGE_TOKUSEI ){
		PokeListWinClear2();
		PokeNoReshuffleMsgGet();
		PokeListScrMsgSet( MsgExpandBuffer, 0 );
		TaskTable[id].TaskAdrs = TalkEndCancelSeq;
		return;
	}

//	if(TaskTable[id].work[M_LIST_TYPE] == PLT_NOT_CHANGE)
	if( PokeListType == PLT_NOT_CHANGE )
	{
		client_no = TempClientNo;
		PokeListWinClear2();
		PokeNickNameSet(
			&PokeParaMine[SearchListRow( SelMonsNo[client_no] )],
			StrTempBuffer0 );
		PM_MsgExpand( MsgExpandBuffer, msg_poke_chg_error00 );
		PokeListScrMsgSet(MsgExpandBuffer, 0);
		TaskTable[id].TaskAdrs = TalkEndCancelSeq;
		return;
	}
	
//	PokeListMenuWinClear(TaskTable[id].work[M_MENUTYPE], BPL_MenuData);
	ListBuffer = GetListRow(cursor_pos);
	ListStatus = P_CHANGE;
	sel_pos = SearchListRow( SelMonsNo[ TempClientNo ] );
	ChangeListRow(sel_pos, cursor_pos);
	SwapPokemon(&PokeParaMine[sel_pos], &PokeParaMine[cursor_pos]);
	TaskTable[id].TaskAdrs = BattlePokeListEnd;
}


//----------------------------------------------------------
//	�L�����Z��
//----------------------------------------------------------
static void CancelMenu(u8 id)
{
	PokeListMenuWinClear(TaskTable[id].work[M_MENUTYPE], BPL_MenuData);
	TaskTable[id].work[M_MENUTYPE] = TaskTable[id].work[M_MENUTYPE_TEMP];

//	PokeListWinClear2();
	PokeListWinFixMsgPut2( ERANDE_MSG, 0 );
	ChangeBackTask(id);
}
