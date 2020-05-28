
#ifndef __POKELIST_H__
#define __POKELIST_H__


#include "task.h"
#include "param.h"
#include "menu.h"

//----------------------------------------------------------
//	�Ƃ肠����	��
//----------------------------------------------------------
#define STOCK_POKE_MAX		TEMOTI_POKEMAX	//�莝���|�P�����ő吔

//----------------------------------------------------------
//	�t�H���gBG�ݒ�
//----------------------------------------------------------
#define LIST8x8_CGX_BLK		1

//----------------------------------------------------------
//	���j���[�֘A�萔
//----------------------------------------------------------
#define MENU_END_X		(240/8)
#define MENU_END_Y		(160/8)

//----------------------------------------------------------
//	�\���̐錾
//----------------------------------------------------------
typedef struct{
	u8 menu_max;
	u8 x_len;				//X�����ւ�ү���ޒ�
	const u8 *menu_list;
}MenuListData;

//----------------------------------------------------------
//	�O���ϐ��錾
//----------------------------------------------------------
extern u8 ListStatus;	//�߹��ؽĂ��ǂ̏������s���ďI���������Ԃ�
extern u8 ListBuffer;	//�߹��ؽĂ̖߂�l������
extern u8 ListMsgStatus;	//0:ү���ޕ\�����ł͂Ȃ��@�@1:ү���ޕ\����

// �A�C�e���g�p���� 2002/02/18 by nakahiro
// id = Ҳ������ID  itemno = ���єԍ�  ReturnAdrs = �߂��̱��ڽ
extern void (*PokeListItemUseFunc)( u8 id, u16 itemno, pTaskFunc ReturnAdrs );

//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------

//----------------------------------------------------------
//	�V�K
//----------------------------------------------------------
extern void PokeListGaugeSetAll(void);
extern void PokeNamePutAll(void);
extern void PokeLvPutAll(void);
extern void PokeHpPutAll(void);
extern void PokeMaxHpPutAll(void);
extern u8	PokeListBgSet(u8);
extern u8	PokeCursorSet2(u8,u8);
extern u16	PokeCursorSys(u8 taskno);
extern u16	PokeCursorSys2(u8 taskno);
extern void PokeNowCursorMove2(u8 taskno, s8 move);
extern void PokeNowCursorMove3( u8 taskno, s8 move );
extern void PokeSelectPosJumpKettei( u8 taskno );
extern void PokemonSortInit(u8 taskno);
extern void PokeListWinFixMsgPut2( u8 msg_no, u8 flg );		// 2002/04/10 by nakahiro
extern void PokeListWinClear2(void);		// 2002/04/19 by nakahiro
extern void PokeCursorPosSet(u8 taskno, u8 pos);
extern u8	GetNowCursorPos(u8 taskno);
extern u8	PokeListScrMsgSet(const u8 msg[], u8);
extern void I_MSG_Deployment(u8 id, u32 type);
extern void ItemIconStatusSet(u8 id, u8 pokepos);
extern void ListPokeAddItem(u8 id, u16 itemno, pTaskFunc returnAdrs);
extern void ListPokeAddMailBox(u8 id, pTaskFunc returnAdrs);
extern void ListPokeDecItem(u8 id, pTaskFunc returnAdrs);
extern u8	PokeAddItem(PokemonParam *pp, u16 itemno);
extern void ListPokeDecMail(u8 id, pTaskFunc returnAdrs);
extern void PokeListMenuWinClear(u8 menu_type, const MenuListData *pmd);
extern void PokeListMenuWindowPut(u8, const MenuListData *, const MENU_ITEM *, u8);
extern pTaskFunc ListMenuCall(u8, const MenuListData *pmd, const MENU_ITEM *mi, u8 pos);
extern void ListPokeWazaSet(u8 id, u16 wazano, pTaskFunc returnAdrs);
extern void CubeItemUse(u8 id, u16 itemno, pTaskFunc returnAdrs);			// 02/01/09 by nakahiro
extern void ListPokeNamePut(u8 pos, PokemonParam *pp);
extern void ListPokeLvPut(u8 pos, PokemonParam *pp);
extern void ListPokeHpPut(u8 pos, PokemonParam *pp);
extern void ListPokeMaxHpPut(u8 pos, PokemonParam *pp);
extern void ListGaugeScreenSet(u8 pos, PokemonParam *pp);
extern void PataChrActorSet(u8 taskno, u8 , PokemonParam *pp);

extern void ListPokeNamePutParam( u8 pos, u8 type, u8 * name );
extern void ListPokeLvPutParam( u8 pos, u8 type, u8 lv );
extern void ListPokeHpPutParam( u8 pos, u8 type, u16 hp );
extern void ListPokeMaxHpPutParam( u8 pos, u8 type, u16 mhp );
extern void ListGaugeScreenSetParam( u8 pos, u8 type, u16 hp, u16 mhp );
extern void PataChrActorSetParam( u8 taskno, u8 pos, u8 type, MultiPokeListData * mpld );

extern void ItemIconActSetParam( u8 id, u8 pos, u16 item );
extern void PokeConditionPutCardMoji( u8 pos, u8 type, u8 condition );

extern void ListPokeNamePut2( u8 pos, u8 type, PokemonParam * pp );
extern void ListPokeLvPut2( u8 pos, u8 type, PokemonParam * pp);
extern void ListPokeHpPut2( u8 pos, u8 type, PokemonParam * pp );
extern void ListPokeMaxHpPut2( u8 pos, u8 type, PokemonParam * pp );
extern void ListGaugeScreenSet2( u8 pos, u8 type, PokemonParam * pp );
extern void PataChrActorSet2( u8 taskno, u8 pos, u8 type, PokemonParam * pp );

extern void ItemIconActorSet(u8 id);
extern void ItmeIconCellPalSet(void);
extern void SwapPokemon(PokemonParam *pPoke1, PokemonParam *pPoke2);
extern void ListPokeItemUse(u8 id, u16 itemno, pTaskFunc returnAdrs);
extern u16	GetWazaNoItem(u16 itemno);
extern u8	TemotiNijyuuCheck( PokemonParam * pp, u16 wazano );
extern void PPRecoverInit( u8 id, u16 itemno, pTaskFunc returnAdrs );		// 02/02/14 by nakahiro
extern void WazaPointUpInit( u8 id, u16 itemno, pTaskFunc returnAdrs );		// 02/02/15 by nakahiro
extern void PokeListLvUpInit( u8 id, u16 itemno, pTaskFunc returnAdrs );	// 02/02/20 by nakahiro
extern void PokeListEvolutionInit( u8 id, u16 item, pTaskFunc returnAdrs );	// 02/04/24 by nakahiro
extern void SetMotasetaMsg( u8 poke, u16 item, u8 clear );
extern u8	DelWazaHidenCheck( u16 waza );

extern u8	SetStatusRecover( u8 cur, u16 item, u8 menu );		// 02/02/21 by nakahiro
extern void PataChrActorSetAll(u8 taskno);

extern u8	PokeListBGCardInit2(u8);
extern void ScrollInPokeListBGCardInit(void);

extern void ListCardMojiAll(u8);
extern void ListCardMoji( u8, u8 );
extern void ListCardSexPut(void);
extern void DrawBG2KETTEI(u8);
extern void DrawBG2MODORU(u8);

extern void StHpAreaClear( u8 pos );

extern u8	MultiCheck(void);

extern void ScrollInPokeListCardMove( u8 id, u16 mons1, u16 mons2 );
extern void SetPokeIconScroll( u8 id, u16 monsno, u8 no );
extern void ScrollInActXPosPlus( u8 id, u8 pos );

extern void PokeCardChangeInit( u8 id );

extern void PokeListSexPut( u8 pos, u8 type, PokemonParam * pp );
extern void PokeListSexPutParam( u16 mons, u8 sex, u8 type, u8 pos, u8 * name );

//----------------------------------------------------------
//	�|�P�����̃j�b�N�l�[���Z�b�g
//	�߂�l�@EOM_�A�h���X
//----------------------------------------------------------
extern u8 * PokeNickNameSet( PokemonParam * pp, u8 * str );


//----------------------------------------------------------
//	�萔
//----------------------------------------------------------
//ListStatus�ɓ����߹��ؽĂ̖߂�l
enum{
	P_NORMAL,	//���ɉ�������Ȃ�����
	P_CHANGE,	//�߹�ݓ���ւ����s����(�퓬)
};

#define LISTGAUGE_TRANS_ADDR	( (void*)CHAR_BASE1_ADDR + 32 * 256 )
#define LISTGAUGE_CHARSUU		12
#define LISTGAUGE_TOPCHRNO		256

#define	LIST_MOJI_TRANS_ADDR	( LISTGAUGE_TRANS_ADDR + LISTGAUGE_CHARSUU * 0x20 )
#define	LIST_MOJI_TOPCHARNO		( LISTGAUGE_TOPCHRNO + LISTGAUGE_CHARSUU )
#define	LIST_MOJI_CHARSUU		0x46

#define	LIST_CONDITION_TRANS_ADDR	( LIST_MOJI_TRANS_ADDR + LIST_MOJI_CHARSUU * 0x20 )
#define	LIST_CONDITION_TOPCHARNO	( LIST_MOJI_TOPCHARNO + LIST_MOJI_CHARSUU )
#define	LIST_CONDITION_CHARSUU		0x1b


#define POKELIST_FONT_START	SYS_FONT_SIZ	//̫�ķ�ד]���J�n�ʒu

#define POKEPATA_PRI		5
#define ITEMICON_PRI		4	//�����ׂ�����ײ��è������

// OBJ����
#define POKE_CURSOR_POS_X1	8
#define POKE_CURSOR_POS_Y1	52
#define POKE_CURSOR_POS_X2	92
#define POKE_CURSOR_POS_Y2	12
#define POKE_CURSOR_INTERVAL_OBJ	24

//����
#define CURSOR_PRI1			7				//Ҳݶ���
#define CURSOR_PRI2			(CURSOR_PRI1+1)	//��޶���
#define SUB_CURSOR_DX		(-2)
#define SUB_CURSOR_DY		4


//-- ���ѱ��݂������ׂ���Ƃ����̾�� --//
#define ITEMICON_OFFSET_X	(4)
#define ITEMICON_OFFSET_Y	(10)


// �X�N���[�����b�Z�[�W�E�B���h�E���W
#define	SCR_MSG_WINSX	3
#define	SCR_MSG_WINSY	14
#define	SCR_MSG_WINEX	26
#define	SCR_MSG_WINEY	19



//=========================================================================
//	��ʉ��̃E�B���h�E���A��^���b�Z�[�W�� �i���o�[
//=========================================================================
enum{
	ERANDE_MSG,
	DOKONI_IDOU_MSG,
	OSIEMASUKA_MSG,
	TUKAIMASUKA_MSG,
	MOTASEMASUKA_MSG,
	DOUSIMASUKA_MSG,		//2001/07/17 by nakahiro
	IAIGIRIERROR_MSG,		//2001/08/01 by nakahiro
	NAMINORIERROR_MSG,		//2001/10/02 by nakahiro
	NAMINORIERROR2_MSG,
	WAZAERROR_MSG,			//2002/02/08 by nakahiro
	WAZARECOVER_MSG,		//2002/02/14 by nakahiro
	WAZAPOINTUP_MSG,		//2002/02/15 by nakahiro
	KUUHAKU_MSG,			//��
	MOTIMONO_MSG,

	TAISEN_INAI_MSG,
	AZUKERU_POKE_MSG,

	TAIRYOKUNAI_MSG,

	POKE_3_INAI_MSG,
	ONAZI_POKE_MSG,
	ONAJI_ITEM_MSG,

	OBOESASEMASUKA_MSG,

	MSG_NOT_PUT = 0xff,		// �\�����Ȃ�
};


//=========================================================================
//	�J�[�h�̏�ɕ\�����镶���w��
//=========================================================================
#define	CARD_MSG_LEN		7						// ������̒���
#define	CARD_MSG_CHAR		( CARD_MSG_LEN * 2 )	// ������̃L������

#define	CARD_TUKAENAI		0
#define	CARD_SANKASINAI		( CARD_MSG_LEN * 2 )
#define	CARD_ITIBANME		( CARD_MSG_LEN * 4 )
#define	CARD_NIBANME		( CARD_MSG_LEN * 6 )
#define	CARD_SANBANME		( CARD_MSG_LEN * 8 )
#define	CARD_HUMEI			( CARD_MSG_LEN * 10 )
#define	CARD_OSU			( CARD_MSG_LEN * 12 )
#define	CARD_MESU			( CARD_MSG_LEN * 14 )
#define CARD_DERAREMASU		( CARD_MSG_LEN * 16 )
#define	CARD_DERAREMASEN	( CARD_MSG_LEN * 18 )
#define	CARD_OBOERARERU		( CARD_MSG_LEN * 20 )
#define	CARD_OBOERARENAI	( CARD_MSG_LEN * 22 )
#define	CARD_OBOETERU		( CARD_MSG_LEN * 24 )


//=========================================================================
//	�E�B���h�E�X�e�[�^�X �i���o�[
//=========================================================================
enum{	//LS = ؽĽð��
	LS_BATTLE,
	LS_HINSI,
	LS_EGG,
};

//=========================================================================
//	�|�P�������X�g�^�C�v�@���퓬�p
//=========================================================================
enum{
	PLT_NORMAL,					//�m�[�}��
	PLT_B_USELESS,				//B�L�����Z���֎~
	PLT_NOT_CHANGE,				//����ւ��֎~
	PLT_ITEM,					//���юg�p
	PLT_NOT_CHANGE_TOKUSEI,		//�����ɂ�����ւ��֎~
};

//=========================================================================
//	�^�X�N�A���[�N�ԍ�����U��
//=========================================================================
//--- ���C�� -----
#define M_PATA_ACTNO0_1		0	//�����ׂ̱��No(���8�ޯ�0�ԖځA����8�ޯ�1�Ԗ�)
#define M_PATA_ACTNO2_3		1	//�����ׂ̱��No(���8�ޯ�2�ԖځA����8�ޯ�3�Ԗ�)
#define M_PATA_ACTNO4_5		2
#define M_CURSOR			3	//���8�ޯĶ��ق�actno ����8�ޯĖ���
#define M_MENUTYPE			4	//�ƭ�ؽĂ�����
#define M_MENUTYPE_TEMP		5	//�ƭ�ؽĂ����ߑޔ�ܰ�

//--- �\�[�g -----
#define S_BACK_TASKNO		0	//Ҳ���������No�ۑ�
#define S_CURSOR1		1	//1�߂̶��ق�actno
#define S_CURSOR2		2	//2�߂̶��ق�actno
// �� NES_SORT�̪�Ď��̂ݕK�v
#define S_ST_END_Y0		3	//��ʳ���޳0����Y���W ���ʴ���Y���W
#define S_ST_END_Y1		4	//��ʳ���޳1����Y���W ���ʴ���Y���W
#define S_WIN0_OFFSET	5	//
#define S_WIN1_OFFSET	6	

//-- �߹�݂ɱ��т���������(�Zϼ݂Ƌ��p)�Ƃ������ --//
#define I_TEMP0			10
#define I_TEMP1			11
#define I_TEMP2			12
#define I_TEMP3			13
#define I_TEMP4			14
#define I_TEMP5			15

//=========================================================================
//	�A�N�^�[�A���[�N�ԍ�����U��
//=========================================================================
//--- ���ق̱��� ---
#define C_POS			0	//���̶݂��وʒu
#define C_OLDPOS		1	//����8�ޯ�:���ق������ɂ��鎞�͍Ō�ɉE���ɂ�����
							//���ق̏ꏊ�A���ق��E���ɂ��鎞�͍Ō�ɍ����ɂ��������ق̏ꏊ

//=========================================================================
//	�������֘A
//=========================================================================
// ������ID
enum {
	FIELD_POKELIST_ID = 0,	// �t�B�[���h
	BATTLE_POKELIST_ID,		// �o�g��
	CONTEST_POKELIST_ID,	// �R���e�X�g
	NAME_POKELIST_ID,		// �������f
	MULTI_POKELIST_ID,		// �}���`�o�g���Q���I��
	SCROLL_POKELIST_ID,		// �}���`�o�g���O
	GROW_POKELIST_ID,		// ��ĉ�
	MANIA_POKELIST_ID,		// �Z�����}�j�A
};

// �|�P�������X�g���C�A�E�g�w��
enum {
	SINGLE_LIST = 0,		// 1vs1 ���C�A�E�g
	DOUBLE_LIST,			// 2vs2 ���C�A�E�g

	DEFAULT_LIST = 0xff,	// ���C�A�E�g�ύX�Ȃ�
};

// �|�P�������X�g�Ăяo��
extern void PokeListCallParamSet( u8, u8, void *, u8 );	// �������p�����[�^�Z�b�g
extern void PokeListCallEx( u8, u8, void *, u8 );		// �^�X�N�A���b�Z�[�W�w��
extern void PokeListCall( u8, u8 );						// �m�[�}��

extern u8	PokeListInitMain(void);		// ���������C������
extern void PokeListMain(void);			// ���C������

extern void PokeListEnd_ContinueScript(u8);	// �|�P�������X�g�I�� -> �t�B�[���h���A -> �X�N���v�g���A


//=========================================================================
//	�J�[�\���ʒu
//=========================================================================
enum {
	PL_POS_POKE0 = 0,
	PL_POS_POKE1,
	PL_POS_POKE2,
	PL_POS_POKE3,
	PL_POS_POKE4,
	PL_POS_POKE5,
	PL_POS_KETTEI,		// ����
	PL_POS_MODORU,		// ���ǂ�
};

//=========================================================================
//	�A�C�e�����ʃ`�F�b�N
//=========================================================================
extern u8 ItemRecoverCheck( u16 item );

enum {
	ITEM_TYPE_BTL_ST_UP = 0,	// �퓬�p�X�e�[�^�X�A�b�v�n
	ITEM_TYPE_LV_UP,			// LvUp�n
	ITEM_TYPE_HP_RCV,			// HP��
	ITEM_TYPE_DOKU_RCV,			// �ŉ�
	ITEM_TYPE_NEMURI_RCV,		// �����
	ITEM_TYPE_YAKEDO_RCV,		// �Ώ���
	ITEM_TYPE_KOORI_RCV,		// �X��
	ITEM_TYPE_MAHI_RCV,			// ��჉�
	ITEM_TYPE_KONRAN_RCV,		// ������
	ITEM_TYPE_MEROMERO_RCV,		// ����������
	ITEM_TYPE_ALLDETH_RCV,		// �S���m����
	ITEM_TYPE_ALL_ST_RCV,		// �X�e�[�^�X�ُ�S��
	ITEM_TYPE_ATC_UP,			// �h�[�s���O�A�C�e�� �U���w�͒lUP
	ITEM_TYPE_HP_UP,			// �h�[�s���O�A�C�e�� HP�w�͒lUP
	ITEM_TYPE_SPA_UP,			// �h�[�s���O�A�C�e�� ���U�w�͒lUP
	ITEM_TYPE_SPD_UP,			// �h�[�s���O�A�C�e�� ���h�w�͒lUP
	ITEM_TYPE_AGL_UP,			// �h�[�s���O�A�C�e�� �f�����w�͒lUP
	ITEM_TYPE_DEF_UP,			// �h�[�s���O�A�C�e�� �h��w�͒lUP
	ITEM_TYPE_EVO,				// �i���A�C�e��
	ITEM_TYPE_PP_UP,			// PPUp
	ITEM_TYPE_PP_3UP,			// PP3Up
	ITEM_TYPE_PP_RCV,			// PP��

	ITEM_TYPE_ETC,				// ���̑�
};


//=========================================================================
//	�|�P�������X�g�@���[�N�i���[�U�[���[�N�g�p�j
//=========================================================================
// ���C�������p
typedef struct {
	PokemonParam	TempParam[6];	// ListRow�p�W�J�ꏊ�i�ėp�j100h x 6 = 600h

	// �����ݒ�p
	u8	init_id;			// ������ID
	u8	init_msg;			// �������b�Z�[�W
	void (*MainTask)(u8);	// ���C���^�X�N
	u8	task_id;			// ���C���^�X�N��ID

	// �����p
	u8	card_flg;	// �J�[�h�]���t���O
	u8	menu_flg;	// ���������Ƀ��j���[���J�����i�t�B�[���h�ȊO�̓J�[�\���ʒu�j
					// ���t�B�[���h�ł̔ԍ�
					// 0 = �J���Ȃ�
					// 1 = �悳���݂�
					// 2 = ������킴
					// 3 = ��������
					// 4 = ���[��

	u8	btl_tower;	// �o�g���^���[�p�̃t���O

	s16	work[16];	// �ėp���[�N
}POKELIST;

// �A�C�e���g�p�p
typedef struct {
	PokemonParam * pp;		// �J�[�\���ʒu�̃|�P�����f�[�^

	u8	main_id;			// ���C���^�X�N��ID
	u8	cursor;				// �I�����ꂽ�|�P�����̈ʒu
	u16	item;				// �A�C�e���ԍ�
	u16	new_waza;			// �V�K�Ɋo����Z

	s32	gauge_work;			// HP�Q�[�W�p���[�N

	pTaskFunc	returnAdrs;	// �߂��^�X�N�A�h���X
	pTaskFunc	SubReturn;	// HP�񕜂Ȃǂ̖߂��^�X�N�A�h���X
}POKE_ITEM_USE;

// �����̃|�P�����I��p
typedef struct {
	u8	main_id;	// ���C���^�X�N��ID
	u8	cur[2];		// �J�[�\���̃A�N�^�[�ԍ�
	u8	act[2];		// �A�C�R���̃A�N�^�[�ԍ�
	u8	move[2];	// �ړ����Ă���ꏊ
	s16	m_cnt[2];	// �ړ��J�E���^�[

	void (*UseTask)(u8);	// �|�P�����؂�ւ����̃^�X�N��ۑ�

}POKE_CARD;


#define P_LIST	( (POKELIST *)&UserWork[ 0x1b000 ] )
#define P_ITEM	( (POKE_ITEM_USE *)&UserWork[ 0x1c000 ] )
#define	P_CARD	( (POKE_CARD *)&UserWork[ 0x1000 ] )


#endif //__POKELIST_H__
