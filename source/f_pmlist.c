//=========================================================================
//
//	f_pmlist.c
//	�t�B�[���h�ł̃|�P�������X�g�Ăяo���C���^�[�t�F�C�X
//
//	2001.06.28
//=========================================================================
//	2001/07/04	by nakahiro

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
//#include "wazalist.h"
#include "kaiwa.h"
#include "item.h"
#include "itemtool.h"
#include "fight.h"
#include "mail.h"
#include "wazano.def"
#include "fieldmap.h"
#include "mapparam.h"
#include "fld_tool.h"
#include "cubecase.h"
#include "mus_tool.h"
#include "ev_check.h"
#include "map_attr.h"
#include "pokeicon.h"
#include "initgame.h"
#include "my_pc.h"
#include "message.h"
#include "fld_task.h"

#include "../evdata/maparray.h"
#include "tenji.h"

//=========================================================================================
//										�V���{��
//=========================================================================================
#define	MENU_SX			20
#define	MENU_SY			0//4
#define	MENU_EX			29
#define	MENU_EY			19//6 * 2 + 3
#define	TRG_WAIT		7
#define	MENU_MAX		10

// ���j���[�ԍ�
enum {
	MENU_TUYOSAWOMIRU = 0,	// 00:�悳���݂�
	MENU_NARABIKAE,			// 01:�Ȃ�т���
	MENU_MOTIMONO,			// 02:��������
	MENU_MODORU,			// 03:���ǂ�
	MENU_MOTASERU,			// 04:��������
	MENU_AZUKARU,			// 05:��������
	MENU_MERU_GET,			// 06:���[�����Ƃ�
	MENU_MERU,				// 07:���[��
	MENU_MERU_READ,			// 08:���[����ǂ�
	MENU_YAMERU,			// 09:��߂�
};

#define	WAZA_MENU_NO	10		// ���j���[���X�g�Z�J�n�ʒu
#define	WAZA_TBL_MAX	0xff	// �Z�e�[�u���I�[

// �^�X�N���[�N�ԍ�
#define	ITEM_WORK		10
#define WAZA_MENU		11

#define MENU_MSG_LEN	8
#define ITEM_MSG_LEN	6

//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void	FieldMainRecover(void);
extern void	FieldMenuRecover(void);
extern void FieldPokeListRuckInit(void);

extern u8 IaigiriCheck(void);
extern u8 IwakudakiCheck(void);
extern u8 AnawohoruCheck(void);
extern u8 FlashCheck(void);
extern u8 HimitunotikaraCheck(void);
extern u8 KairikiCheck(void);
extern u8 AmaikaoriCheck(void);
extern u8 MilkNomiCheck(void);
extern u8 TeleportCheck(void);

extern void MilkNomiInit( u8 id );

extern void MenuComSet( u8 *,u8 *, u8 );	//���X�g�ǉ�

extern void MailViewInit(MAIL_DATA *,pFunc,u8);	//add matsuda 2001.12.18(��)

extern void ReturnBagInit(void);

extern void FlyingInit(void);

extern u8	ItemTypeCheck( u16 no );	// �A�C�e���̎�ނ𒲂ׂ�

static void MakePokeMenuList(void);		//���j���[���X�g�̍쐬�i���j
static void FldBagPokeListItemSetCheck(u8 id);
static void FldBagPokeListItemMailInput(u8 id);
static void ReturnFldBagMailInput(void);
static void FldBagMailInputFadeWait(u8 id);
static void FldBagMailInputMsgWait(u8 id);
void ReturnFldPokeItemSetMail(void);
static void FieldPokeListMailAdd(u8 id);
static void MailMenu(u8 id);
static void MailMenuTask(u8 id);
static void MailRead(u8 id);
static void MailReadFadeOut(u8 id);
static void MailReadReturn(void);

void FieldPokeListTask(u8);
static void FieldPokeListMenu(u8);
static void FieldPokeListEnd(u8);

static void	Status(u8);			// �悳���݂�
static void	Position(u8);		// �Ȃ�т���
static void	PokeItem(u8);		// ��������
static void	Return(u8);			// ���ǂ�
static void	Return2(u8);
static void	ReturnMenu(u8);		// ��߂�
static void	Extra(u8);			// �Z
static void PokeMailGet(u8);	// ���[�����Ƃ�
static void MineItem(u8);		// ��������
static void PokeItemGet(u8);	// ��������

static void UseExtra(u8);			// �Z���g�p
static void ExtraNoBadge(u8);		// �o�b�W������
void StartExtraInit(void);			// �t�B�[���h�G�t�F�N�g�����ݒ�
static void StartExtra(u8);			// �t�F�[�h�I����҂��ăG�t�F�N�g�X�^�[�g
static void ExtraErrorTask(u8);
static u8 NaminoriCheck(void);		// �Ȃ݂̂�`�F�b�N
static u8 SorawotobuCheck(void);	// ������Ƃԃ`�F�b�N
static u8 DivingCheck(void);		// �_�C�r���O�`�F�b�N
static u8 TakinoboriCheck(void);	// ��o��`�F�b�N

void FieldBagPokeListTask(u8);
static void FieldPokeListReturnBag(u8);
static void FldBagPokeListItemSet(u8);
static void ReturnMailBoxInit(u8);	// ���[���{�b�N�X�֕��A
static void PokeAllDethRcvTask(u8);	// �S���m���񕜗p�^�X�N

static void ReturnPokeListMenu(void);	// �|�P�������X�g�ɕ��A��A���j���[���J��


//=========================================================================================
//										�O���ϐ�
//=========================================================================================
extern u16 ReturnItemNo;					// �o�b�O�őI�������A�C�e���ԍ�
extern u8	UseFldPokeFlg;					// ����/��������/�L���[�u
extern void (*FieldReturnItemUse)(u8);		// �t�B�[���h���A��̔�ѐ�


static const MENU_ITEM	menu[] =
{
	{ pl_str_tuyosa,    Status      },	// 00:�悳���݂�
	{ pl_str_narabikae, Position    },	// 01:�Ȃ�т���
	{ str_motimono,     PokeItem    },	// 02:��������
	{ str_yameru,       Return      },	// 03:���ǂ�
	{ str_motaseru,     MineItem    },	// 04:��������
	{ str_azukaru,      PokeItemGet },	// 05:��������
	{ pl_str_mail_get,  PokeMailGet },	// 06:���[�����Ƃ�
	{ str_mail,         MailMenu    },	// 07:���[��		add by matsuda 2001.12.18(��)
	{ pl_str_mail_read, MailRead    },	// 08:���[����ǂ�	add by matsuda 2001.12.18(��)
	{ str_yameru,       ReturnMenu  },	// 09:��߂�		add by matsuda 2001.12.18(��)

	// �Z�͂������牺�ɒǉ��A����ȊO�͏�֒ǉ��i enum,define�̕ύX�� �j
	// �o�b�W��
	{ WazaNameData[ WAZANO_IAIGIRI ],	 Extra },		// 10:����������
	{ WazaNameData[ WAZANO_HURASSYU ],	 Extra },		// 11:�t���b�V��
	{ WazaNameData[ WAZANO_IWAKUDAKI ],  Extra },		// 12:���킭����
	{ WazaNameData[ WAZANO_KAIRIKI ],	 Extra },		// 13:�����肫
	{ WazaNameData[ WAZANO_NAMINORI ],   Extra },		// 14:�Ȃ݂̂�
	{ WazaNameData[ WAZANO_SORAWOTOBU ], Extra },		// 15:������Ƃ�
	{ WazaNameData[ WAZANO_DAIBINGU ],   Extra },		// 16:�_�C�r���O
	{ WazaNameData[ WAZANO_TAKINOBORI ], Extra },		// 17:�����̂ڂ�

	// ��`�Z�ȊO
	{ WazaNameData[ WAZANO_TEREPOOTO ],      Extra },	// 18:�e���|�[�g
	{ WazaNameData[ WAZANO_ANAWOHORU ],      Extra },	// 19:���Ȃ��ق�
	{ WazaNameData[ WAZANO_HIMITUNOTIKARA ], Extra },	// 20:�Ђ݂̂�����
	{ WazaNameData[ WAZANO_MIRUKUNOMI ],     Extra },	// 21:�~���N�̂�
	{ WazaNameData[ WAZANO_TAMAGOUMI ],      Extra },	// 22:�^�}�S����
	{ WazaNameData[ WAZANO_AMAIKAORI ],      Extra },	// 23:���܂�������
};

static const u16 waza_no_tbl[] = {
	WAZANO_IAIGIRI,			// ����������
	WAZANO_HURASSYU,		// �t���b�V��
	WAZANO_IWAKUDAKI,		// ���킭����
	WAZANO_KAIRIKI,			// �����肫
	WAZANO_NAMINORI,		// �Ȃ݂̂�
	WAZANO_SORAWOTOBU,		// ������Ƃ�
	WAZANO_DAIBINGU,		// �_�C�r���O
	WAZANO_TAKINOBORI,		// �����̂ڂ�

	WAZANO_TEREPOOTO,		// �e���|�[�g
	WAZANO_ANAWOHORU,		// ���Ȃ��ق�
	WAZANO_HIMITUNOTIKARA,	// �Ђ݂̂�����
	WAZANO_MIRUKUNOMI,		// �~���N�̂�
	WAZANO_TAMAGOUMI,		// �^�}�S����
	WAZANO_AMAIKAORI,		// ���܂�������

	WAZA_TBL_MAX			// �I��
};

static u8 menu_pos = 0;				//�J�[�\���ʒu
static u8 menu_max = 0;				//���j���[��
static u8 menu_list[MENU_MAX] = {};	//���j���[���X�g

static const u8 item_list[] = { MENU_MOTASERU, MENU_AZUKARU, MENU_YAMERU };
static const MenuListData Fld_MenuData = { 3, ITEM_MSG_LEN, item_list };

static const u8 mailmenu_list[] = { MENU_MERU_READ, MENU_MERU_GET, MENU_YAMERU };
static const MenuListData Mail_MenuData = { 3, 9, mailmenu_list };

u8	FPokeNo;	//�I�����ꂽ�|�P����

void (*ExtraInit)(void);

typedef struct {
	u8   (*check)(void);	//�`�F�b�N�֐�
	u8	 msg;				//�G���[���b�Z�[�W�ԍ�
}EXTRAFUNC;

static const EXTRAFUNC	ExtraCheck[] =
{
	{ IaigiriCheck,        IAIGIRIERROR_MSG  },	// ����������
	{ FlashCheck,          WAZAERROR_MSG     },	// �t���b�V��
	{ IwakudakiCheck,      WAZAERROR_MSG     },	// ���킭����
	{ KairikiCheck,        WAZAERROR_MSG     },	// �����肫
	{ NaminoriCheck,       NAMINORIERROR_MSG },	// �Ȃ݂̂�
	{ SorawotobuCheck,     WAZAERROR_MSG     },	// ������Ƃ�
	{ DivingCheck,         WAZAERROR_MSG     },	// �_�C�r���O
	{ TakinoboriCheck,     WAZAERROR_MSG     },	// �����̂ڂ�

	{ TeleportCheck,       WAZAERROR_MSG     },	// �e���|�[�g
	{ AnawohoruCheck,      WAZAERROR_MSG     },	// ���Ȃ��ق�
	{ HimitunotikaraCheck, WAZAERROR_MSG     },	// �Ђ݂̂�����
	{ MilkNomiCheck,       TAIRYOKUNAI_MSG   },	// �~���N�̂�
	{ MilkNomiCheck,       TAIRYOKUNAI_MSG   },	// �^�}�S����
	{ AmaikaoriCheck,      WAZAERROR_MSG     },	// ���܂�������
};


//=========================================================================================
//							�t�B�[���h���j���[����̏���
//=========================================================================================
//----------------------
//		�����ݒ�
//----------------------
void FieldMenuPokeListInit(void)
{
	FadeData.trans_stop = 1;
	PokeListCall( FIELD_POKELIST_ID, SINGLE_LIST );
}

//-------------------------------------
//		���j���[���X�g�̍쐬�i���j
//-------------------------------------
static void MakePokeMenuList(void)
{
	u16	i,j;

	menu_max = 0;

	// �^�}�S�̏ꍇ
	if( PokeParaGet( &PokeParaMine[ FPokeNo ], ID_tamago_flag ) != 0 ){
		MenuComSet( menu_list, &menu_max, MENU_TUYOSAWOMIRU );	// "�悳���݂�"
		MenuComSet( menu_list, &menu_max, MENU_NARABIKAE );		// "�Ȃ�т���"
		MenuComSet( menu_list, &menu_max, MENU_MODORU );		// "���ǂ�"
		return;
	}
	
	// �킴�`�F�b�N
	for( i=0; i<4; i++ ){
		j = 0;
		while(1){
			if( waza_no_tbl[j] == WAZA_TBL_MAX )	break;
			if( PokeParaGet( &PokeParaMine[ FPokeNo ], ID_waza1+i ) == waza_no_tbl[j] ){
				MenuComSet( menu_list, &menu_max, WAZA_MENU_NO+j );
				break;
			}
			j++;
		}
	}
	
	MenuComSet( menu_list, &menu_max, MENU_TUYOSAWOMIRU );	// "�悳���݂�"
//	MenuComSet( menu_list, &menu_max, MENU_TUKAERUWAZA );	// "������킴"
	if( PokeParaGet( &PokeParaMine[1], ID_monsno ) != 0 )
		MenuComSet( menu_list, &menu_max, MENU_NARABIKAE );	// "�Ȃ�т���"
	if(MailItemNoCheck(PokeParaGet(&PokeParaMine[FPokeNo], ID_item)))
		MenuComSet(menu_list, &menu_max, MENU_MERU);		// "���[��"
	else
		MenuComSet( menu_list, &menu_max, MENU_MOTIMONO );	// "��������"

	MenuComSet( menu_list, &menu_max, MENU_MODORU );	// "���ǂ�"
}

//-------------------------------------
//		���j���[�E�B���h�E�쐬
//-------------------------------------
void MakeMenuWindow( u8 x, u8 y, u8 w, u8 max, const MENU_ITEM *menu, const u8 *List )
{
//	PokeListWinClear2();
	PokeListWinFixMsgPut2( DOUSIMASUKA_MSG, 1 );
	NMenuWinBoxWrite( x, y, x+w, y+max*2+1 );
	NMenuFreeListWrite( x+2, y+1, max, menu, List );
}

//--------------------------------------------------
//		�t�B�[���h�|�P�������X�g�̃��j���[�쐬
//--------------------------------------------------
static void MakeFldMenuWin( u8 init_pos )
{
	u8	y;

	y = MENU_EY-1-( menu_max << 1 );
	MakeMenuWindow( MENU_SX, y, 9, menu_max ,menu, menu_list );
	NMenuVCursorInit( MENU_SX+1, y+1, menu_max, init_pos );
}

//------------------------------
//		�|�P�����I���^�X�N
//------------------------------
void FieldPokeListTask(u8 id)
{
	if(!(FadeData.fade_sw)){
		switch( PokeCursorSys(id) ){
		case A_BUTTON:
			SePlay(SE_SELECT);
			FPokeNo = GetNowCursorPos(id);
			PokeNickNameSet( &PokeParaMine[FPokeNo], StrTempBuffer0 );
			MakePokeMenuList();
			menu_pos = 0;
			MakeFldMenuWin(0);
			TaskTable[id].TaskAdrs = FieldPokeListMenu;
			break;

		case B_BUTTON:
			SePlay(SE_SELECT);
			PaletteFadeReq(0xffffffff,0,0,16,0);
			TaskTable[id].TaskAdrs = FieldPokeListEnd;
			break;
		}
	}
}

//------------------------------
//		���j���[�I���^�X�N
//------------------------------
static void FieldPokeListMenu(u8 id)
{
	MENUFUNC	func;

	if( FadeData.fade_sw )	return;

	if( ( sys.Repeat & PLUS_KEY_MASK ) == U_KEY ){
		SePlay(SE_SELECT);
		menu_pos = NMenuVCursorSet(-1);
		return;
	}

	if( ( sys.Repeat & PLUS_KEY_MASK ) == D_KEY ){
		SePlay(SE_SELECT);
		menu_pos = NMenuVCursorSet(1);
		return;
	}

	if( sys.Trg & A_BUTTON ){
		SePlay(SE_SELECT);
		func = menu[ menu_list[menu_pos] ].ptr;
		func(id);
		return;
	}

	if( sys.Trg & B_BUTTON )
		Return(id);
}

//---------------------
//		�I������
//---------------------
static void FieldPokeListEnd(u8 id)
{
	if( FadeData.fade_sw == 0 ){
		FPokeNo = 0;
		MainProcChange( FieldMenuRecover );
		DelTask(id);
	}
}

//--------------------------------------------------
//		�|�P�������X�g�ɕ��A��A���j���[���J��
//--------------------------------------------------
/*
static u8 GetReturnMenuPos(void)
{
	u8	i,pos;

	switch( P_LIST->menu_flg ){
	case 1:
		pos = MENU_TUYOSAWOMIRU;
		break;
	case 2:
		pos = MENU_TUKAERUWAZA;
		break;
	case 3:
		pos = MENU_MOTIMONO;
		break;
	case 4:
		pos = MENU_MERU;
		break;
	default:
		return	0;
	}

	for( i=0; i<MENU_MAX; i++ ){
		if( menu_list[i] == pos )
			break;
	}

	return	i;
}
*/

static void ReturnPokeListMenu(void)
{
	PokeNickNameSet( &PokeParaMine[FPokeNo], StrTempBuffer0 );
	MakePokeMenuList();
//	menu_pos = GetReturnMenuPos();
	menu_pos = 0;
	MakeFldMenuWin( menu_pos );
}

//-----------------------------------
//		���j���[�@"�悳���݂�"
//-----------------------------------
static void ReturnFieldStatus2(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, PokeStatusBuf );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			FPokeNo = PokeStatusBuf;
			ReturnPokeListMenu();
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}
static void ReturnFieldStatus(void)
{
	FadeData.trans_stop = 1;

	PokeListCallParamSet( FIELD_POKELIST_ID, DEFAULT_LIST, FieldPokeListMenu, DOUSIMASUKA_MSG );
	MainProcChange( ReturnFieldStatus2 );
}

static void StatusFade( u8 id )
{
	u8 temp;
	
	if(FadeData.fade_sw == 0){
		temp = ActWork[TaskTable[id].work[M_CURSOR] >> 8].work[0];
		DelTask(id);
		P_LIST->menu_flg = 1;
		PokeStatusCall2( PokeParaMine, temp, PokeCountMine-1, ReturnFieldStatus, LIST_ST );
	}
}

static void	Status(u8 id)
{
	PaletteFadeReq(0xffffffff,0,0,16,0);
	TaskTable[id].TaskAdrs = StatusFade;
}

//----------------------------------
//		���j���[�@"�Ȃ�т���"
//----------------------------------
void F_PokeSortSet( u8 id )
{
	ChangeTask( id, PokemonSortInit, FieldPokeListTask );
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
}

static void	Position( u8 id )
{
	//SePlay(SE_SELECT);
	P_CARD->UseTask = PokeCardChangeInit;
	P_LIST->work[7] = DOKONI_IDOU_MSG;
	F_PokeSortSet(id);
}

//---------------------------------
//		���j���[�@"��������"
//---------------------------------
//���j���[�I���^�X�N
static void ItemMenuTask(u8 id)
{
	pTaskFunc	func;

	if(sys.Trg == U_KEY){
		if(menu_pos != 0){
			menu_pos = NMenuVCursorSet(-1);
			SePlay(SE_SELECT);
		}
	}

	if(sys.Trg == D_KEY){
		if(menu_pos != 2){
			menu_pos = NMenuVCursorSet(1);
			SePlay(SE_SELECT);
		}
	}

	if(sys.Trg & A_BUTTON){
		SePlay(SE_SELECT);
		func = ListMenuCall(0,&Fld_MenuData,menu,menu_pos);
		func(id);
		return;
	}

	if(sys.Trg & B_BUTTON){
		PokeListMenuWinClear(0,&Fld_MenuData);
		ReturnMenu(id);
	}
}

static void PokeItem(u8 id)
{
	menu_pos = 0;
	//SePlay(SE_SELECT);
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
	PokeListMenuWindowPut( 0, &Fld_MenuData, menu, 0 );
	PokeListWinFixMsgPut2( MOTIMONO_MSG, 2 );

	TaskTable[id].TaskAdrs = ItemMenuTask;
}

//��������
static void EndPokeListItemAddFade(u8 id)
{
	u8 cursor, mailid;

	if(FadeData.fade_sw)
		return;
	cursor = GetNowCursorPos(id);
	mailid = PokeParaGet(&PokeParaMine[cursor], ID_mailid);
	DelTask(id);
	KaiwaInputBegin(KAIWA_MODE_MAIL, Fld.MailData[mailid].msg, ReturnFldPokeItemSetMail, KAIWA_INTERVIEWER_NOTHING);
}

static void ItemMotasetaMsgWait(u8 id)
{
	if(ListMsgStatus == 1)
		return;
	ItemIconStatusSet(id, GetNowCursorPos(id));
	PokeListWinFixMsgPut2( ERANDE_MSG, 0 );
	TaskTable[id].TaskAdrs = FieldPokeListTask;
}

static void EndPokeListItemAdd(u8 id)
{
	if(MailItemNoCheck(ReturnItemNo) && ListStatus)
	{
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs = EndPokeListItemAddFade;
	}
	else if(0)//ListStatus)
	{
		SetMotasetaMsg( FPokeNo, ReturnItemNo, 0 );
//		PokeListScrMsgSet( msg_poke_item01, 0 );
		TaskTable[id].TaskAdrs = ItemMotasetaMsgWait;
	}
	else
	{
		NMenuBoxClear( 0, 0, 29, 19);
		PokeListWinFixMsgPut2( ERANDE_MSG, 0 );
		TaskTable[id].TaskAdrs = FieldPokeListTask;
	}
}

static void FieldPokeListItemAdd(u8 id)
{
	if(!(FadeData.fade_sw)){
		ListPokeAddItem( id, ReturnItemNo, EndPokeListItemAdd );
	}
}

// �������铹����[���̂Ƃ�
static void SetMailInput( u8 id )
{
	u8 cursor, mailid;

	cursor = GetNowCursorPos(id);
	mailid = PokeParaGet(&PokeParaMine[cursor], ID_mailid);
	DelTask(id);
	KaiwaInputBegin(KAIWA_MODE_MAIL, Fld.MailData[mailid].msg, ReturnFldPokeItemSetMail, KAIWA_INTERVIEWER_NOTHING);
}
static void FieldPokeListItemAdd2( u8 id )
{
	ListPokeAddItem( id, ReturnItemNo, SetMailInput );
}
static void MailAddMain(void)
{
	MainTask();
}

//----------------------------------------------------------
//	�ʏ펞�̃o�b�O��ʂ���̖߂��
//----------------------------------------------------------
// �A�C�e������
static void ReturnFldPokeItemSet2(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, FPokeNo );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}

// �L�����Z��
static void ReturnFldPokeItemSetNo(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, FPokeNo );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			P_LIST->menu_flg = 3;
			ReturnPokeListMenu();
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}

void ReturnFldPokeItemSet(void)
{
	u8	id = 0;
	u8	cursor,pokeitem;
	
	if(MailItemNoCheck(ReturnItemNo) != 0){
		id = AddTask( FieldPokeListItemAdd2, 0 );
		FadeData.trans_stop = 1;
		PokeCursorSet2( id, 0 );
//		PokeCursorSet2( id, 1 );
//		PokeCursorSet2( id, 2 );
		PokeCursorPosSet(id, FPokeNo);
		PokeNowCursorMove2( id, 0 );
		cursor = GetNowCursorPos(id);
		pokeitem = PokeParaGet(&PokeParaMine[cursor], ID_item);

		if(	pokeitem == NON_ITEM ){
			MainProcChange( MailAddMain );
			return;
		}else{
			DelTask(id);
		}
	}

	FadeData.trans_stop = 1;

	if( ReturnItemNo != 0 ){
		PokeListCallParamSet( FIELD_POKELIST_ID, DEFAULT_LIST, FieldPokeListItemAdd, MSG_NOT_PUT );
		MainProcChange( ReturnFldPokeItemSet2 );
	}else{
		PokeListCallParamSet( FIELD_POKELIST_ID, DEFAULT_LIST, FieldPokeListMenu, DOUSIMASUKA_MSG );
		MainProcChange( ReturnFldPokeItemSetNo );
	}
	
/*	
	u8	id = 0;
	u8	cursor,pokeitem;
	
	if(MailItemNoCheck(ReturnItemNo) != 0){
		id = AddTask( FieldPokeListItemAdd2, 0 );
		PokeCursorPosSet(id, FPokeNo);
		PokeNowCursorMove2( id, 0 );
		cursor = GetNowCursorPos(id);
		pokeitem = PokeParaGet(&PokeParaMine[cursor], ID_item);

		if(	pokeitem == NON_ITEM ){
			MainProcChange( MailAddMain );
			return;
		}else{
			DelTask(id);
		}
	}

	FadeData.trans_stop = 1;
	MainProcChange( ReturnFldPokeItemSet2 );
*/

}


//----------------------------------------------------------
//	�o�b�O��ʂŃ_�C���N�g�Ń��[�����͉�ʂɍs�������̖߂��
//----------------------------------------------------------
static void RF_PokeSetMailMsg(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, FPokeNo );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}

void ReturnFldPokeItemSetMail(void)
{
	u16 itemno;
	
	FadeData.trans_stop = 1;
	if(AnswerWork == 0)
	{	//���[���ɂȂɂ����͂�������Ȃ������ꍇ
		itemno = PokeParaGet(&PokeParaMine[FPokeNo], ID_item);
		AddItem(itemno, 1);
		itemno = 0;
		PokeParaPut(&PokeParaMine[FPokeNo], ID_item, (u8*)&itemno);
		PokeDelMail(&PokeParaMine[FPokeNo]);
		PokeListCallParamSet( FIELD_POKELIST_ID, DEFAULT_LIST, FieldPokeListTask, ERANDE_MSG );
	}else
		PokeListCallParamSet( FIELD_POKELIST_ID, DEFAULT_LIST, FieldPokeListMailAdd, MSG_NOT_PUT );

	MainProcChange( RF_PokeSetMailMsg );
}

static void FieldPokeListMailAdd(u8 id)
{
	if(FadeData.fade_sw)
		return;
	SetMotasetaMsg( FPokeNo, ReturnItemNo, 0 );
//	PokeListScrMsgSet( msg_poke_item01, 0 );
	TaskTable[id].TaskAdrs = ItemMotasetaMsgWait;
}

static void ItemFadeWait(u8 id)
{
	if(!(FadeData.fade_sw)){
		MainProcChange( FieldPokeListRuckInit );
		DelTask(id);
	}
}

static void MineItem(u8 id)
{
//	SePlay(SE_SELECT);
	ListBuffer = GetNowCursorPos(id);	//add by matsuda 2001.12.18(��)
	PaletteFadeReq(0xffffffff,0,0,16,0);
	TaskTable[id].TaskAdrs = ItemFadeWait;
}

//��������
static void PokeItemGet2(u8 id)
{
	Return2(id);
}

static void PokeItemGet(u8 id)
{
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
	PokeListWinClear2();
	ListPokeDecItem(id,PokeItemGet2);
}

//-------------------------------------
//		���j���[�@"���[�����Ƃ�"
//-------------------------------------
static void PokeMailGet(u8 id)
{
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
	PokeListWinClear2();
	ListPokeDecMail(id,PokeItemGet2);
}

//----------------------------------------------------------
//	���j���[�@�h���[���h	add by matsuda 2001.12.18(��)
//----------------------------------------------------------
static void MailMenu(u8 id)
{
	menu_pos = 0;
	//SePlay(SE_SELECT);
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
	PokeListMenuWindowPut( 0, &Mail_MenuData, menu, 0 );

	TaskTable[id].TaskAdrs = MailMenuTask;
}

static void MailMenuTask(u8 id)
{
	pTaskFunc	func;

	if(sys.Repeat == U_KEY){
		SePlay(SE_SELECT);
		if(menu_pos == 0)
			menu_pos = NMenuVCursorSet(Mail_MenuData.menu_max-1);
		else
			menu_pos = NMenuVCursorSet(-1);
	}

	if(sys.Repeat == D_KEY){
		SePlay(SE_SELECT);
		if(menu_pos == Mail_MenuData.menu_max-1)
			menu_pos = NMenuVCursorSet(-(Mail_MenuData.menu_max-1));
		else
			menu_pos = NMenuVCursorSet(1);
	}

	if(sys.Trg & A_BUTTON){
		SePlay(SE_SELECT);
		func = ListMenuCall(0,&Mail_MenuData,menu,menu_pos);
		func(id);
		return;
	}

	if(sys.Trg & B_BUTTON){
		PokeListMenuWinClear(0,&Mail_MenuData);
		Return(id);
	}
}

//----------------------------------------------------------
//	���j���[�@�h���[������ށhadd by matsuda 2001.12.18(��)
//----------------------------------------------------------
static void MailRead(u8 id)
{
	PaletteFadeReq(0xffffffff,0,0,16,0);
	TaskTable[id].TaskAdrs = MailReadFadeOut;
}

static void MailReadFadeOut(u8 id)
{
	u8 cursor, mailid;

	if(FadeData.fade_sw)
		return;
	cursor = GetNowCursorPos(id);
	mailid = PokeParaGet(&PokeParaMine[cursor], ID_mailid);
	DelTask(id);
	MailViewInit(&Fld.MailData[mailid], MailReadReturn, 1);
}

//-- ���[���������ʂ���̖߂�� --//
static void MailReadReturn(void)
{
	PokeStatusBuf = FPokeNo;
	P_LIST->menu_flg = 4;
	ReturnFieldStatus();
}

//-------------------------------
//		���j���[�@"���ǂ�"
//-------------------------------
static void Return2(u8 id)
{
	menu_pos = 0;
	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
//	PokeListWinClear2();
	PokeListWinFixMsgPut2( ERANDE_MSG, 0 );
	TaskTable[id].TaskAdrs = FieldPokeListTask;
}

static void	Return(u8 id)
{
	SePlay(SE_SELECT);
	Return2(id);
}

//-------------------------------
//		���j���[�@"��߂�"
//-------------------------------
static void	ReturnMenu(u8 id)
{
	SePlay(SE_SELECT);

	NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
	PokeListWinClear2();
//	PokeListWinFixMsgPut2( DOUSIMASUKA_MSG, 0 );
	MakeFldMenuWin( menu_pos );
	TaskTable[id].TaskAdrs = FieldPokeListMenu;
}

//------------------------------
//		���j���[�@�Z�֘A
//------------------------------
static void	Extra(u8 id)
{
	s16 * wk = TaskTable[id].work;

	wk[WAZA_MENU] = menu_list[menu_pos] - WAZA_MENU_NO;

	// �ʐM��
	if( ItemToolSioCheck() == TRUE ){
		NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
//		PokeListWinClear2();

		// �u�~���N�̂݁v�u�^�}�S���݁v
		if( wk[ WAZA_MENU ] == 11 || wk[ WAZA_MENU ] == 12 )
			PokeListWinFixMsgPut2( WAZAERROR_MSG, 0 );
		else
			PokeListWinFixMsgPut2( ExtraCheck[ wk[WAZA_MENU] ].msg, 0 );

		TaskTable[id].TaskAdrs = ExtraErrorTask;
		return;
	}

	// ��`�Z
	if( wk[ WAZA_MENU ] < 8 ){
		// �o�b�W�̂���Ȃ�
		if( EventFlagCheck( SYS_BATCH01_GET + wk[WAZA_MENU] ) != ON ){
			NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
			PokeListWinClear2();
			PokeListScrMsgSet( msg_poke_no_badge, 1 );
			TaskTable[id].TaskAdrs = ExtraNoBadge;
			return;
		}
	}

	// ���ꂼ��̂킴�`�F�b�N
	if( ExtraCheck[ wk[WAZA_MENU] ].check() == TRUE ){
		menu_pos = 0;
//		ScriptParameter0 = FPokeNo;
		if( wk[WAZA_MENU] != 11 && wk[WAZA_MENU] != 12 ){
			TaskTable[id].TaskAdrs = UseExtra;
			PaletteFadeReq(0xffffffff,0,0,16,0);
		}else	MilkNomiInit(id);
	}else{
		NMenuBoxClear( MENU_SX, MENU_SY, MENU_EX, MENU_EY );
//		PokeListWinClear2();

		// �u�Ȃ݂̂�v�ł��łɏ���Ă���
		if( wk[WAZA_MENU] == 4 && CheckHeroState(MY_MV_BIT_SWIM) != 0 )
			PokeListWinFixMsgPut2( NAMINORIERROR2_MSG, 0 );
		else
			PokeListWinFixMsgPut2( ExtraCheck[ wk[WAZA_MENU] ].msg, 0 );

		TaskTable[id].TaskAdrs = ExtraErrorTask;
	}
}

// �o�b�W������
static void ExtraNoBadge( u8 id )
{
	if( ListMsgStatus != 1 ){
		if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
			NMenuBoxClear( 0, 14, 29, 19);
			Return(id);
		}
	}
}

//�Z���g�p
static void UseExtra( u8 id )
{
	if( FadeData.fade_sw == 0 ){
		if( TaskTable[id].work[WAZA_MENU] != 5 )
		{
			MainProcChange( FieldMainRecover );
		}else
		{
			//�_���̓��A�`�F�b�N
			if( TenjiSorawotobuChk() )
				MainProcChange( FieldMainRecover );
			else
				MainProcChange( FlyingInit );
		}

		DelTask(id);
	}
}

//�t�B�[���h�G�t�F�N�g�����ݒ�
void StartExtraInit(void)
{
//	FieldFadeControl( FIELD_BLACK_IN, 0 );
	FieldFadeBlackInSet();
	AddTask( StartExtra, 8) ;
}

//�t�F�[�h�I����҂��ăG�t�F�N�g�X�^�[�g
static void StartExtra(u8 id)
{
	if( FieldFadeinCheck() == 1 ){
		FeSetWork[0] = PokeParaGet( &PokeParaMine[ FPokeNo ], ID_monsno );
		ExtraInit();
		DelTask(id);
	}
}

//�G���[�I���҂�
static void ExtraErrorTask(u8 id)
{
	if( sys.Trg & A_BUTTON || sys.Trg & B_BUTTON ){
		NMenuBoxClear(1,17,28,18);
		Return(id);
	}
}


//---------------------
//		�Ȃ݂̂�
//---------------------
//�G�t�F�N�g�Z�b�g
static void SetFeNaminori(void)
{
	FeSetWork[0] = FPokeNo;
	FldEffSet( FECODE_HidenPokeSwim );
//	ResetForceEvent();
}
//�g�p�\�`�F�b�N
static u8 NaminoriCheck(void)
{
	if( CheckHeroNaminoriStart() == TRUE && CheckHeroAttrNaminoriStart() == TRUE ){
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeNaminori;
		return	TRUE;
	}
	return	FALSE;
}

//-----------------------
//		������Ƃ�
//-----------------------
//�G�t�F�N�g�Z�b�g
static void SetFeSorawotobu(void)
{
	FeSetWork[0] = FPokeNo;
	FldEffSet( FECODE_HidenPokeSora );
//	ResetForceEvent();
}

//�g�p�\�`�F�b�N
static u8 SorawotobuCheck(void)
{
	//�_���̓��A�`�F�b�N
	if( TenjiSorawotobuChk() )
	{
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeEvSora;
		return	TRUE;
	}

	if( MTYPE_IsEnableFlyCheck( NowMap.type ) == TRUE ){
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeSorawotobu;
		return	TRUE;
	}

	return	FALSE;
}
//�L�����Z���E�|�P�������X�g�ɕ��A
static void ReturnFldPokeSora2(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, FPokeNo );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}
void ReturnFldPokeSora(void)
{
	FadeData.trans_stop = 1;
	PokeListCallParamSet( FIELD_POKELIST_ID, DEFAULT_LIST, FieldPokeListTask, ERANDE_MSG );
	MainProcChange( ReturnFldPokeSora2 );
}

// �|�P�����i���o�[��Ԃ�
u16 GetHidenMonsNo(void)
{
	return	PokeParaGet( &PokeParaMine[ FPokeNo ], ID_monsno );
}


//-----------------------
//		�_�C�r���O
//-----------------------
//�G�t�F�N�g�Z�b�g
static void SetFeDiving(void)
{
	FeSetWork[0] = FPokeNo;
	FldEffSet( FECODE_Diving );
}

// �g�p�\�`�F�b�N
static u8 DivingCheck(void)
{
	FeSetWork[1] = DivingMapCheck();

	if( FeSetWork[1] != 0 ){
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeDiving;
		return	TRUE;
	}
	return	FALSE;
}

//-------------------
//		��o��
//-------------------
// �G�t�F�N�g�Z�b�g
static void SetFeTakinobori(void)
{
	FeSetWork[0] = FPokeNo;
	FldEffSet( FECODE_Takinobori );
}

// �g�p�\�`�F�b�N
static u8 TakinoboriCheck(void)
{
	s16	px,py;

	GetHeroFrontCellPos( &px, &py );

	if( ATR_IsWaterFallCheck( GetCellAttribute( px, py ) ) == TRUE &&
		CheckHeroTakinoboriStart() == TRUE )
	{
		pFieldRecoverFunc = StartExtraInit;
		ExtraInit = SetFeTakinobori;
		return	TRUE;
	}
	return	FALSE;
}

// �f�o�b�O�p
#ifdef	PM_DEBUG
void TestTakinobori(void)
{
	s16	px,py;

	GetHeroFrontCellPos( &px, &py );

	if( ATR_IsWaterFallCheck( GetCellAttribute( px, py ) ) == TRUE &&
		CheckHeroTakinoboriStart() == TRUE )
	{
		SetFeTakinobori();
		return;
	}
	ResetForceEvent();
}
#endif	PM_DEBUG

//--------------------------
//		�e���|�[�g�i���j
//--------------------------
/*
//�G�t�F�N�g�Z�b�g
static void SetFeTeleport(void)
{
	InitEventFlagAtTeleport();		//�e���|�[�g�ɂ��t���O������
	InitHeroLastStatus();
	SetFieldFlyInStart();

	NextMapSetByTelMapID();
//	AddMapChangeWarpTask();

	MainProcChange( PokeListMain );
}
//�g�p�\�`�F�b�N
static u8 TeleportCheck(void)
{
	if( MTYPE_IsEnableFlyCheck( NowMap.type ) == TRUE ){
		SetFeTeleport();
//		pFieldRecoverFunc = StartExtraInit;
//		ExtraInit = SetFeTeleport;
		return	TRUE;
	}

	return	FALSE;
}
*/


//=========================================================================================
//									�o�b�O����̏���
//=========================================================================================
//--------------------------------------------------
//		�Z�}�V���̎���"�����܂���"��\������
//--------------------------------------------------
static void ListCardTukaemasenSet(void)
{
	u8	i;
	u8	machine_no;

	machine_no = ReturnItemNo - ITEM_SKILL_START;

	for( i=0; i<6; i++ ){
		if( PokeParaGet( &PokeParaMine[i], ID_monsno ) != 0 ){
			StHpAreaClear(i);
#ifdef	PM_DEBUG
			if( DebugMainFlag == 0 &&
				( PokeParaGet( &PokeParaMine[i], ID_tamago_flag ) != 0 ||
				PokeWazaMachineCheck( &PokeParaMine[i], machine_no ) == 0 ) )
			{
				ListCardMoji( i, CARD_OBOERARENAI );
			}else{
				if( TemotiNijyuuCheck( &PokeParaMine[i], GetWazaNoItem(ReturnItemNo) ) )
					ListCardMoji( i, CARD_OBOETERU );
				else
					ListCardMoji( i, CARD_OBOERARERU );
			}
#else
			if( PokeParaGet( &PokeParaMine[i], ID_tamago_flag ) != 0 ||
				PokeWazaMachineCheck( &PokeParaMine[i], machine_no ) == 0 )
			{
				ListCardMoji( i, CARD_OBOERARENAI );
			}else{
				if( TemotiNijyuuCheck( &PokeParaMine[i], GetWazaNoItem(ReturnItemNo) ) )
					ListCardMoji( i, CARD_OBOETERU );
				else
					ListCardMoji( i, CARD_OBOERARERU );
			}
#endif	PM_DEBUG
		}
	}
}

//-------------------------------
//		�i���A�C�e���̂Ƃ�
//-------------------------------
static void ListCardEvolusion(void)
{
	u8	i;

	for( i=0; i<6; i++ ){
		if( PokeParaGet( &PokeParaMine[i], ID_monsno ) != 0 &&
			( PokeParaGet( &PokeParaMine[i], ID_tamago_flag ) != 0 ||
			  PokeShinkaCheck( &PokeParaMine[i], ITEM_SHINKA_CHECK, ReturnItemNo ) == 0 ) )
		{
			StHpAreaClear(i);
			ListCardMoji( i, CARD_TUKAENAI );
		}
	}
}

//---------------------
//		�����ݒ�
//---------------------
void FieldBagPokeListInit2(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			if( UseFldPokeFlg == PM_ITEM_USE_MODE ){
				switch( ItemTypeCheck( ReturnItemNo ) ){
				case SKILL_ITEM:
					ListCardTukaemasenSet();
					break;
				case EVO_ITEM:
					ListCardEvolusion();
				}
			}
			if( FPokeNo > 5 || PokeParaGet( &PokeParaMine[FPokeNo], ID_monsno ) == 0 )
				FPokeNo = 0;
			PokeCursorPosSet( P_LIST->task_id, FPokeNo );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}

void FieldBagPokeListInit(void)
{
	FadeData.trans_stop = 1;
	switch( UseFldPokeFlg ){
	case PM_ITEM_USE_MODE:		// ����
		if( ItemTypeCheck( ReturnItemNo ) == SKILL_ITEM )
			PokeListCallParamSet( FIELD_POKELIST_ID, SINGLE_LIST, FieldBagPokeListTask, OBOESASEMASUKA_MSG );
		else
			PokeListCallParamSet( FIELD_POKELIST_ID, SINGLE_LIST, FieldBagPokeListTask, TUKAIMASUKA_MSG );
		break;
	case PM_ITEM_USE_ALLDETH_RCV_MODE:
		PokeListCallParamSet( FIELD_POKELIST_ID, SINGLE_LIST, PokeAllDethRcvTask, MSG_NOT_PUT );
		break;
	case PM_ITEM_PUT_MODE:		// ��������
	case PM_ITEM_MAIL_BOX_MODE:	// ���[���{�b�N�X���烁�[������������
		PokeListCallParamSet( FIELD_POKELIST_ID, SINGLE_LIST, FieldBagPokeListTask, MOTASEMASUKA_MSG );
	}
	MainProcChange( FieldBagPokeListInit2 );
}

//---------------------------------------------------
//		�|�P�����I���^�X�N
//---------------------------------------------------
void FieldBagPokeListTask(u8 id)
{
	if( FadeData.fade_sw != 0 )	return;

	switch(PokeCursorSys(id)){
	case A_BUTTON:
		FPokeNo = GetNowCursorPos(id);
		if( PokeParaGet( &PokeParaMine[ FPokeNo ], ID_tamago_flag ) != 0 ){
			SePlay(SE_HAZURE);
			break;
		}

		PokeListWinClear2();

		// �A�C�e�����g��
		if( UseFldPokeFlg == PM_ITEM_USE_MODE )
			PokeListItemUseFunc( id, ReturnItemNo, FldBagPokeListItemSet );
		// �A�C�e������������
		if( UseFldPokeFlg == PM_ITEM_PUT_MODE ){
			SePlay(SE_SELECT);
			ListPokeAddItem( id, ReturnItemNo, FldBagPokeListItemSetCheck );
		}
		// ���[���{�b�N�X�̃��[������������
		if( UseFldPokeFlg == PM_ITEM_MAIL_BOX_MODE ){
			SePlay(SE_SELECT);
			ListPokeAddMailBox( id, ReturnMailBoxInit );
		}

		break;

	case B_BUTTON:
		FPokeNo = GetNowCursorPos(id);
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);

		if( UseFldPokeFlg == PM_ITEM_USE_MODE ||
			UseFldPokeFlg == PM_ITEM_PUT_MODE )
		{
			TaskTable[id].TaskAdrs = FieldPokeListReturnBag;
		}
		if( UseFldPokeFlg == PM_ITEM_MAIL_BOX_MODE )
			TaskTable[id].TaskAdrs = ReturnMailBoxInit;

		break;
	}
}

//---------------------------------------
//	�S���m���񕜗p�^�X�N
//---------------------------------------
static void PokeAllDethRcvTask( u8 id )
{
	if( FadeData.fade_sw != 0 )	return;

	PokeListItemUseFunc( id, ReturnItemNo, FldBagPokeListItemSet );
}

//------------------------------------------
//		����������̃t�F�[�h�A�E�g�ݒ�
//------------------------------------------
static void FldBagPokeListItemSet(u8 id)
{
	PaletteFadeReq(0xffffffff,0,0,16,0);
	TaskTable[id].TaskAdrs = FieldPokeListReturnBag;
}

//---------------------------
//		�o�b�O�֕��A
//---------------------------
static void FieldPokeListReturnBag(u8 id)
{
	if(FadeData.fade_sw == 0){
		MainProcChange( ReturnBagInit );
		DelTask(id);
	}
}

//---------------------------------
//		���[���{�b�N�X�֕��A
//---------------------------------
static void ReturnMailBoxMain( u8 id )
{
	if( !(FadeData.fade_sw) ){
		MainProcChange( MyPC_MailPokeSetReturn );
		DelTask(id);
	}
}
static void ReturnMailBoxInit( u8 id )
{
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	TaskTable[id].TaskAdrs = ReturnMailBoxMain;
}


//----------------------------------------------------------
//	���������A�C�e����Ұق��Ⴄ���ɂ���ď����𕪂���
//									by matsuda	2001.12.18(��)
//----------------------------------------------------------
static void FldBagPokeListItemSetCheck(u8 id)
{
	if(ListStatus == 2)
	{
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs = FldBagPokeListItemMailInput;
	}
	else
		FldBagPokeListItemSet(id);
}

//----------------------------------------------------------
//	Ұٓ��͌Ăяo��	by matsuda 2001.12.18(��)
//----------------------------------------------------------
static void FldBagPokeListItemMailInput(u8 id)
{
	u8 mailid;
	
	if(FadeData.fade_sw)
		return;
	FPokeNo = GetNowCursorPos(id);
	mailid = PokeParaGet(&PokeParaMine[FPokeNo], ID_mailid);
	DelTask(id);
	KaiwaInputBegin(KAIWA_MODE_MAIL, Fld.MailData[mailid].msg, ReturnFldBagMailInput, KAIWA_INTERVIEWER_NOTHING);
}

//----------------------------------------------------------
//	Ұٓ��͉�ʂ���̖߂��	by matsuda 2001.12.18(��)
//----------------------------------------------------------
static void ReturnFldBagMailInput2(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, FPokeNo );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}

static void ReturnFldBagMailInput(void)
{
	u16 itemno;
	
	FadeData.trans_stop = 1;
	if(AnswerWork == 0)
	{	//���[���ɂȂɂ����͂�������Ȃ������ꍇ
		itemno = PokeParaGet(&PokeParaMine[FPokeNo], ID_item);
		AddItem(itemno, 1);
		itemno = 0;
		PokeParaPut(&PokeParaMine[FPokeNo], ID_item, (u8*)&itemno);
		PokeDelMail(&PokeParaMine[FPokeNo]);
		AddTask(FieldPokeListReturnBag, 5);
	}
	else{
		PokeListCallParamSet( FIELD_POKELIST_ID, SINGLE_LIST, FldBagMailInputFadeWait, MSG_NOT_PUT );
		MainProcChange( ReturnFldBagMailInput2 );
		return;
	}
	FadeData.trans_stop = 0;
	MainProcChange( PokeListMain );
}

static void FldBagMailInputFadeWait(u8 id)
{
	if(FadeData.fade_sw)
		return;
	SetMotasetaMsg( FPokeNo, ReturnItemNo, 1 );
//	PokeListScrMsgSet( msg_poke_item01, 1 );
	TaskTable[id].TaskAdrs = FldBagMailInputMsgWait;
}

static void FldBagMailInputMsgWait(u8 id)
{
	if(ListMsgStatus == 1)
		return;
	FldBagPokeListItemSet(id);
}


//--------------------------------------------------
//		�Z�}�V���g�p���ɋZ��ʂ��烊�X�g�֕��A
//--------------------------------------------------
extern void WazaDelSet2( u8 id );
extern void WazaAkirameSelectSeq2( u8 id );

// �o�b�O��
void NewWazaReturnBag( u8 id )
{
	FldBagPokeListItemSet(id);
}

void ReturnNewWazaSet2(void)
{
	while(1){
		if( PokeListInitMain() == TRUE ){
			PokeCursorPosSet( P_LIST->task_id, PokeStatusBuf );
			PokeNowCursorMove2( P_LIST->task_id, 0 );
			MainProcChange( PokeListMain );
			break;
		}
		if( ItemToolSioCheck() == TRUE )	break;
	}
}

// �����ɖ߂�
void ReturnNewWazaSet(void)
{
	FadeData.trans_stop = 1;

	if( GetNewWazaSelectPos() != 4 ){
		PokeListCallParamSet( FIELD_POKELIST_ID, SINGLE_LIST, WazaDelSet2, MSG_NOT_PUT );
	}else{
		PokeListCallParamSet( FIELD_POKELIST_ID, SINGLE_LIST, WazaAkirameSelectSeq2, MSG_NOT_PUT );
	}

	MainProcChange( ReturnNewWazaSet2 );
}
