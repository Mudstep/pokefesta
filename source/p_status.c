//=========================================================================
//
//	
//	�|�P�����X�e�[�^�X�\��
//
//									2001.07.10(��) by matsuda
//=========================================================================
#include "common.h"
#include "print.h"
#include "task.h"
#include "palanm.h"
#include "actor.h"
#include "actanm.h"
#include "intr.h"
#include "madefine.h"
#include "fld_sio.h"

#include "poketool.h"
#include "decord.h"
#include "objdata.h"
#include "p_status.h"
#include "b_pmlist.h"
#include "wazatool.h"
#include "pokelist.h"

#include "fight.h"
#include "window.h"
#include "server.h"
#include "client_t.h"
#include "waza_tbl.h"
#include "item.h"
#include "client.h"
#include "waza_eff.h"
#include "mus_tool.h"
#include "contest.h"
#include "gauge.h"
#include "itemtool.h"
#include "script.h"
#include "marking.h"
#include "bss.h"
#include "cli_ef.h"
#include "message.h"
#include "mappos.h"
#include "pokesio.h"
#include "laster.h"
#include "boxsub.h"

#include "monsno.def"
#include "wazainfo.dat"


//=========================================================================
//	�萔��`
//=========================================================================
#define	SCR_BASE_P2		(  8 << BG_SCREEN_BASE_SHIFT )	// �y�[�W2�̓W�J�ꏊ
#define	SCR_BASE_P3		( 10 << BG_SCREEN_BASE_SHIFT )	// �y�[�W3�̓W�J�ꏊ
#define	SCR_BASE_P4		( 12 << BG_SCREEN_BASE_SHIFT )	// �y�[�W4�̓W�J�ꏊ

#define	BG1_PRI_CNG			( (*(vu16 *)REG_BG1CNT) & 0xfffc )	// BG1 Priority Init
#define	BG2_PRI_CNG			( (*(vu16 *)REG_BG2CNT) & 0xfffc )	// BG2 Priority Init
#define	BG1_SCRN_CNG		( (*(vu16 *)REG_BG1CNT) & 0xe0ff )	// BG1 Screen Adrs Init
#define	BG2_SCRN_CNG		( (*(vu16 *)REG_BG2CNT) & 0xe0ff )	// BG2 Screen Adrs Init
#define	BG1_PRI_SCRN_CNG	( (*(vu16 *)REG_BG1CNT) & 0xe0fc )	// BG1 Pri Scrn Adrs Init
#define	BG2_PRI_SCRN_CNG	( (*(vu16 *)REG_BG2CNT) & 0xe0fc )	// BG2 Pri Scrn Adrs Init


//========================================================================================
//								�v���g�^�C�v�錾
//========================================================================================
extern void VoicePlay( u16 no, s8 pan );
extern u8	BallItemNoTblNoConv( u16 itemno );
extern u32	GetMyDataID(void);

static void PokemonStatusCallMain(void);	// �����ݒ胁�C��
static void PokeStatusRegisterInit(void);
static u8	PokeStatusDecordSet(void);
static u8	St_PokemonSet2( PokemonParam *, u8 * );
static u8	St_PokeActSet( PokemonParam * );
static void SeqStPokeInit( actWork * Xreg );
static void NextPokeStatus( u8 id );
static void NowPokeParamDataSet( PokemonParam * pp );
static u16	PokeWazaNoGet( PokemonParam * pp, u8 wazapos );
static u16	PokeWazaPPGet( PokemonParam * pp, u8 pppos );

static void StPageBGInit(void);			// �����y�[�WBG�Z�b�g

static void PagePosBGSet(void);			// ���݂̃y�[�W
static void ConditionWin( s8 );			// ��Ԉُ�E�B���h�E
static void WazaParamWin( s8, u8 );		// �Z���ʃE�B���h�E
static void ContestParamWin( s8, u8 );	// �R���e�X�g���ʃE�B���h�E
static void FifthWazaWin( u8 );			// �T�ڂ̋Z�E�B���h�E

static void PokerusMark( PokemonParam * pp );	// �|�P���X�����ς݃}�[�N

static void StatusPaletteMake(void);	// �p���b�g�쐬

// �L�[�`�F�b�N
static void PokeStatusMainTask( u8 );		// �X�e�[�^�X��ʃ��C���^�X�N
static void PokeWazaMainTask( u8 );			// �Z��ʃ��C���^�X�N�i����ւ��\�j
static void NewWazaInitTask( u8 id );		// �Z�Y�ꏉ���ݒ�
static void NewWazaMainTask( u8 id );		// �Z�Y�ꃁ�C���^�X�N
static u8	HidenCheck( u8 id );			// ��`�Z���ǂ����`�F�b�N
static void HidenErrorInit( u8 id, u8 pos );// �Z�Y��E��`�Z�G���[
static void WazaChangeTask( u8 );			// �Z����ւ�
static void PageChange( u8, s8 );			// �y�[�W�؂�ւ�
static void PokeChange( u8, s8 );			// �|�P�����؂�ւ�
static void StatusEnd( u8 );				// �I������
static void PokeStatusEnd( u8 );
static void WazaSelect( u8, s8, u8 *);		// �Z�I��
static void WazaChange( u8, s8 );			// �Z����ւ�
static void WazaSelectInit(u8);
static void ReturnPageChenge(u8);			// �Z�I�����I�����ăy�[�W�I���ɖ߂�


static void ST_ActInit(void);				// �A�N�^�[������
static void ST_ActDel(u8);					// �A�N�^�[����
static void ST_ActBanish( u8 no, u8 flg );	// �\���ؑ�

// �����A�N�^�[
static void ST_TYPE_ActMove( u8 id, u8 x, u8 y, u8 no );
static void ST_TYPE_ActSet2(void);

// �J�[�\���A�N�^�[
static void Add_WAZA_Cursor( u8 no );
static void Del_WAZA_Cursor( u8 no );
static void Waza_CursorActMove( actWork * act );
static void WAZA_CursorAnmChange( u8 flg );
static void Waza_CursorbanishClear( u8 flg );

// ��Ԉُ�A�C�R��
static void Add_ST_IzyoIconAct(void);

static void GetMonsBallActSet( PokemonParam * pp );

static void P_StSpMojiPut( u8, u8, u8 );			// �ǉ������\��

// �p�����[�^�\�����C������
static u8 P_StExChange2(void);
static void P_StChange01( PokemonParam * pp );	// ����������1
static void P_StChange02( PokemonParam * pp );	// ����������2
static void P_StChange03( PokemonParam * pp );	// �g����Z

// �p�����[�^�\��
static void P_StRightClear(void);	// ��ʉE�N���A
static void P_StRight01Clear(void);	// ��ʉE�N���A
static void P_StRight02Clear(void);	// ��ʉE�N���A
static void P_StWazaClear(void);	// ��ʉE�N���A

static void P_StRight01All( PokemonParam * );
static void P_StRight02All( PokemonParam * );

static void P_StLeftParam( PokemonParam * );			// "����������"��ʍ��i�ω��j
static void P_StRight01Def(void);						// "�������݂�"��ʉE�P�i�f�t�H���g�j
static void P_StRight01Param( PokemonParam * );			// "�������݂�"��ʉE�P�i�ω��j
static void P_StRight02Def(void);						// "�������݂�"��ʉE�Q�i�f�t�H���g�j
static void P_StRight02Param( PokemonParam * );			// "�������݂�"��ʉE�Q�i�ω��j
static void P_WazaLeftDef(u8);							// "�g����Z"��ʍ��i�f�t�H���g�j
static void P_StWazaPut1( PokemonParam * );				// "�g����Z"��ʉE
static void WazaInfoPut( PokemonParam *, u8 * );		// �Z�̌��ʂ�\��
static void SetNormalWazaInfo(u16);						// �Z������\���i�m�[�}���j
static void SetContestWazaInfo(u16);					// �Z������\���i�R���e�X�g�j
static void SetNormalWazaPoint(u16);					// �퓬�̋Z�̈З́E������\��
static void SetContestWazaPoint(u16);					// �R���e�X�g�̋Z�̌��ʃ|�C���g��\��
static void P_StTamagoMemoPut( PokemonParam *, u8, u8 );// �^�}�S�̃�����\��
static void P_StMemoPut( PokemonParam *, u8, u8 );		// �g���[�i�[�����̓��e��\��
static void PutPokeName( PokemonParam * );				// �|�P������ / ����
static void P_StSexPut( PokemonParam *, u8, u8 );		// ���ʕ\��
static void P_StRibbonPut( PokemonParam * );			// ���{�����\��
static void P_StItemNamePut( u16, u8, u8 );				// �莝���A�C�e���\��
static void P_StNextExpPut( PokemonParam *, u8, u8 );	// ���̃��x���܂łɕK�v�Ȍo���l��\��
static void PokeStFuncInfoMsgPut(void);					// �������b�Z�[�W�\��

static void P_StFifthWazaPut( PokemonParam * );

static void AddBoxMarkAct( PokemonParam * pp );
static void AddBoxMarkActInit( PokemonParam * pp );


//========================================================================================
//									�O���[�o���ϐ�
//========================================================================================
extern const u8	pp_count_mask[];
extern const u32 GrowTbl[][101];
extern const u32 DecordAdrs[];
extern SIOTRAINER_INFO SioTrainerInfo[4];

#include "p_status.dat"

u8 PokeStatusBuf = 0;	//�ð����ʂ�����Ƃ��ɍŌ�ɉ��Ԗڂ����Ă�����������

static void (* PCReturnTask)(u8);

// �֐��e�[�u��
static void (* const P_StDataRightDef[])(void) = {
	P_StRight01Def,
	P_StRight02Def,
};
static void (* const P_StDataParam[])( PokemonParam * pp ) = {
	P_StChange01,
	P_StChange02,
	P_StChange03,
	P_StChange03,
};
static void (* const P_StDataParam2[])( PokemonParam * pp ) = {
	P_StRight01All,
	P_StRight02All,
	P_StWazaPut1,
	P_StWazaPut1,
};



//=======================================================================================
//-------------------------
//		���C�����[�v
//-------------------------
static void PokeStatusMainProc(void)
{
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

//-------------------
//		VBlank
//-------------------
static void PokeStatusVblank(void)
{
	*(vu16 *)REG_BG1HOFS = ScrX1;
	*(vu16 *)REG_BG1VOFS = ScrY1;
	*(vu16 *)REG_BG2HOFS = ScrX2;
	*(vu16 *)REG_BG2VOFS = ScrY2;
	*(vu16 *)REG_BG3HOFS = ScrX3;
	*(vu16 *)REG_BG3VOFS = ScrY3;

	actOAMtrans();
	actDMAtrans();
	PaletteFadeTrans();
}


//========================================================================================
//										�����ݒ�
//========================================================================================
//-------------------------------------------------------------------------------------
//							�|�P�����X�e�[�^�X��ʌĂяo��
//
//	���� : * ppArray    = �|�P�p���擪�A�h���X ( PokemonParam or PokemonPasoParam )
//			 put_no     = �\������ԍ�
//			 poke_max   = �|�P������
//			 ReturnAdrs = �X�e�[�^�X��ʂ���̖߂�� ( sys.pMainProc )
//			 type       = �Ă΂ꂽ�ꏊ
//
//-------------------------------------------------------------------------------------
void PokeStatusCall2( void * ppArray, u8 put_no, u8 poke_max, pFunc ReturnAdrs, u8 type )
{
	FadeData.trans_stop = 1;

	P_SYSTEM->ppArray    = ppArray;
	P_SYSTEM->type       = type;
	P_SYSTEM->put_no     = put_no;
	P_SYSTEM->poke_max   = poke_max;
	P_SYSTEM->ReturnAdrs = ReturnAdrs;
	P_SYSTEM->div_count  = 0;
	P_SYSTEM->waza_pos   = 4;
	P_SYSTEM->new_waza   = 0;
	P_SYSTEM->bg_flg     = 0;
	P_SYSTEM->waza_lock  = 0;

	if( type >= BOX_ST )	P_SYSTEM->adrs_type = PSP_TYPE;
	else					P_SYSTEM->adrs_type = PP_TYPE;

	switch( type ){
	case LIST_ST:
	case BOX_ST:
		P_SYSTEM->page_init   = 0;

#ifdef FESTA_VER_ROM
		P_SYSTEM->page_max = 2;
#else
		P_SYSTEM->page_max = 3;
#endif

		P_SYSTEM->page_loop   = 0;
		P_SYSTEM->change_lock = 0;
		P_SYSTEM->comment1    = PAGE_1_TITLE;
		P_SYSTEM->comment2    = YAMERU_COM;
		break;

	case BTL_WAZA:
		P_SYSTEM->page_init   = 0;

#ifdef FESTA_VER_ROM
		P_SYSTEM->page_max = 2;
#else
		P_SYSTEM->page_max = 3;
#endif

		P_SYSTEM->page_loop   = 0;
		P_SYSTEM->change_lock = 0;
		P_SYSTEM->comment1    = PAGE_1_TITLE;
		P_SYSTEM->comment2    = YAMERU_COM;
		P_SYSTEM->waza_lock   = 1;
		break;

	case NEW_WAZA:
		P_SYSTEM->page_init   = 2;

#ifdef FESTA_VER_ROM
		P_SYSTEM->page_max = 2;
#else
		P_SYSTEM->page_max = 3;
#endif

		P_SYSTEM->page_loop   = 1;
		P_SYSTEM->change_lock = 1;
		P_SYSTEM->comment1    = PAGE_3_TITLE;
		P_SYSTEM->comment2    = 0;
		P_SYSTEM->waza_pos    = 0;
		break;

	case LIST_WAZA:
	case BOX_WAZA:
		P_SYSTEM->page_init   = 2;

#ifdef FESTA_VER_ROM
		P_SYSTEM->page_max = 2;
#else
		P_SYSTEM->page_max = 3;
#endif

		P_SYSTEM->page_loop   = 1;
		P_SYSTEM->change_lock = 1;
	}

	P_SYSTEM->page = P_SYSTEM->page_init;

	MainProcChange( PokemonStatusCallMain );
}

// �Z�Y��
void PokeNewWazaSet( void * ppArray, u8 put_no, u8 poke_max, pFunc ReturnAdrs, u16 waza )
{
	PokeStatusCall2( ppArray, put_no, poke_max, ReturnAdrs, NEW_WAZA );
	P_SYSTEM->new_waza = waza;
}

// ���C���^�X�N�Z�b�g
static void PokeStatusAddMainTask(void)
{
	switch( P_SYSTEM->type ){
	case LIST_ST:
	case BOX_ST:
		P_SYSTEM->id = AddTask( PokeStatusMainTask, 0 );
		break;

	case BTL_WAZA:
		P_SYSTEM->id = AddTask( PokeStatusMainTask, 0 );
		break;

	case NEW_WAZA:
	case DEL_WAZA:
		P_SYSTEM->id = AddTask( NewWazaInitTask, 0 );
		break;

	case LIST_WAZA:
	case BOX_WAZA:
		P_SYSTEM->id = AddTask( PokeWazaMainTask, 0 );
	}
}

static u8 PokemonStatusInit(void)
{
	switch(sys.InitSeqWork){
	case 0:
		SetVBlankFunc( NULL );
		InitActor();
		sys.InitSeqWork++;
		break;
	case 1:
		LasterInit();
		sys.InitSeqWork++;
		break;
	case 2:
		ObjPalManInit();
		sys.InitSeqWork++;
		break;
	case 3:
		DIV_DMACLEAR( 3, 0, BG_VRAM, BG_MODE0_VRAM_SIZE, 32 );
		sys.InitSeqWork++;
		break;
	case 4:
		PokeStatusRegisterInit();
		sys.InitSeqWork++;
		break;
	case 5:
		PrintScreenInit( POKESTATUS_TYPE );
		sys.InitSeqWork++;
		break;
	case 6:
		NMenuScreenInitStart( POKESTATUS_TYPE );
		sys.InitSeqWork++;
		break;
	case 7:
		if ( NMenuScreenInitMain() == FALSE )	break;
		sys.InitSeqWork++;
		break;

	case 8:
		PokeStatusAddMainTask();
		sys.InitSeqWork++;
		break;

	case 9:
		DIV_DMACOPY( 3, statussub_Character, BG_VRAM+0xc000, 0x140, 16 );	// BG�L�����]��
		DIV_DMACOPY( 3, keyinfo_Character, BG_VRAM+0xc140, 0x100, 16 );		// BG�L�����]��
		P_SYSTEM->div_count = 0;
		sys.InitSeqWork++;
		break;
	case 10:
		if( PokeStatusDecordSet() == FALSE )	break;
		P_SYSTEM->div_count = 0;
		sys.InitSeqWork++;
		break;
	case 11:
		ST_ActInit();
		sys.InitSeqWork++;
		break;
	case 12:
		NowPokeParamDataSet( &P_SYSTEM->pp );
		if( GetPokeCondition( &P_SYSTEM->pp ) == ST_NORMAL )	ConditionWin(0);
		else													ConditionWin(10);
		PokerusMark( &P_SYSTEM->pp );
		sys.InitSeqWork++;
		break;
	case 13:
		ST_TYPE_ActSet2();
		AddBoxMarkAct( &P_SYSTEM->pp );
		sys.InitSeqWork++;
		break;
	case 14:
		GetMonsBallActSet( &P_SYSTEM->pp );
		P_SYSTEM->div_count = 0;
		sys.InitSeqWork++;
		break;
	case 15:
		P_SYSTEM->pokeact = St_PokemonSet2( &P_SYSTEM->pp, &P_SYSTEM->div_count );
		if( P_SYSTEM->pokeact == 0xff )	break;
		P_SYSTEM->div_count = 0;
		sys.InitSeqWork++;
		break;
	case 16:
		StatusPaletteMake();
		PagePosBGSet();
		sys.InitSeqWork++;
		break;
	case 17:
		if( P_SYSTEM->page < 2 )
			P_StDataRightDef[ P_SYSTEM->page ]();
		sys.InitSeqWork++;
		break;
	case 18:
		P_StLeftParam( &P_SYSTEM->pp );
		sys.InitSeqWork++;
		break;
	case 19:	// �e�y�[�W�̃p�����[�^
		P_StDataParam2[ P_SYSTEM->page ]( &P_SYSTEM->pp );
		sys.InitSeqWork++;
		break;
	case 20:
		if( PokeParaGet( &P_SYSTEM->pp, ID_tamago_flag ) != 0 )
			ScrX3 = 256;
		else	ScrX3 = 0;
		sys.InitSeqWork++;
		break;
	case 21:
		StPageBGInit();			// �����y�[�WBG�Z�b�g
		if( P_SYSTEM->waza_pos != 0 ){
			WazaParamWin( 0, 0 );
			ContestParamWin( 0, 0 );
		}else{
			WazaParamWin( 9, 0 );
			ContestParamWin( 9, 0 );
		}
		PokeStFuncInfoMsgPut();
		sys.InitSeqWork++;
		break;
	case 22:
		if( FieldRecvFifoClearWait() == TRUE )	break;
		sys.InitSeqWork++;
		break;
	default:
		SetVBlankFunc(PokeStatusVblank);
		HardFadeReq(0xff, 0, 16, 0, 1);
		MainProcChange( PokeStatusMainProc );
		FadeData.trans_stop = 0;
		return	TRUE;
	}

	return	FALSE;
}

static void PokemonStatusCallMain(void)
{
	while(1){
		if( PokemonStatusInit() == TRUE )
			break;
		if( ItemToolSioCheck() == TRUE )	break;
	}
}

//------------------------------
//		���W�X�^�����ݒ�
//------------------------------
static void PokeStatusRegisterInit(void)
{
	// 8x16�t�H���g
	*(vu16 *)REG_BG0CNT = BG_SCREEN_SIZE_0				// 256x256
						| BG_PRIORITY_0					// Priority 0
						| 30 << BG_SCREEN_BASE_SHIFT	// 0x0f000 ( 0x0800 )
						| 2 << BG_CHAR_BASE_SHIFT		// 0x08000 ( 0x4000 )
						| BG_COLOR_16					// 16 Color
						| BG_MOS_OFF;					// Mosaic OFF
	// �y�[�W1
	*(vu16 *)REG_BG1CNT = BG_SCREEN_SIZE_1				// 512x256
						| BG_PRIORITY_1					// Priority 1
						| 8 << BG_SCREEN_BASE_SHIFT 	// 0x04000 ( 0x1000 )
						| 0 << BG_CHAR_BASE_SHIFT		// 0x00000 ( 0x2000 )
						| BG_COLOR_16					// 16 Color
						| BG_MOS_OFF;					// Mosaic OFF
	// �y�[�W2
	*(vu16 *)REG_BG2CNT = BG_SCREEN_SIZE_1				// 512x256
						| BG_PRIORITY_2					// Priority 2
						| 10 << BG_SCREEN_BASE_SHIFT	// 0x05000 ( 0x1000 )
						| 0 << BG_CHAR_BASE_SHIFT		// 0x00000 ( 0x2000 )
						| BG_COLOR_16					// 16 Color
						| BG_MOS_OFF;					// Mosaic OFF
	// �ŉ��w�y�[�W
	*(vu16 *)REG_BG3CNT = BG_SCREEN_SIZE_1				// 512x256
						| BG_PRIORITY_3					// Priority 3
						| 28 << BG_SCREEN_BASE_SHIFT	// 0x0e000 ( 0x1000 )
						| 0 << BG_CHAR_BASE_SHIFT		// 0x08000 ( 0x2000 )
						| BG_COLOR_16					// 16 Color
						| BG_MOS_OFF;					// Mosaic OFF
	ScrX1 = 0;
	ScrY1 = 0;
	ScrX2 = 0;
	ScrY2 = 0;
	ScrX3 = 0;
	ScrY3 = 0;
	*(vu16 *)REG_BG0HOFS = 0;
	*(vu16 *)REG_BG0VOFS = 0;
	*(vu16 *)REG_BG1HOFS = 0;
	*(vu16 *)REG_BG1VOFS = 0;
	*(vu16 *)REG_BG2HOFS = 0;
	*(vu16 *)REG_BG2VOFS = 0;
	*(vu16 *)REG_BG3HOFS = 0;
	*(vu16 *)REG_BG3VOFS = 0;

	*(vu16 *)REG_BLDCNT = 0;

	// �\���ݒ�
	*(vu16 *)REG_DISPCNT = DISP_MODE_0				// BG Mode 0 ( Text BG x 4 )
						 | DISP_OBJ_CHAR_1D_MAP		// OBJ 1D VRAM Map Format
						 | DISP_OBJ_ON				// OBJ ON
						 | DISP_BG0_ON				// BG0 ON
						 | DISP_BG1_ON				// BG1 ON
						 | DISP_BG2_ON				// BG2 ON
						 | DISP_BG3_ON;				// BG3 ON
}

//------------------------------
//		�O���t�B�b�N�Z�b�g
//------------------------------
static u8 PokeStatusDecordSet(void)
{
	switch( P_SYSTEM->div_count ){
	case 0:
		DecordVram( (void*)p_st2_bg_pch_ADRS, (void*)CHAR_BASE0_ADDR );
		break;
	case 1:
		DecordVram( (void*)p_st2_s1_psc_ADRS, (void*)SCREEN_BASE28_ADDR );
		break;
	case 2:
		DecordVram( (void*)p_st2_s5_psc_ADRS, (void*)SCREEN_BASE29_ADDR );
		break;
	case 3:
		DecordVram( (void*)p_st2_s2_psc_ADRS, (void*)SCREEN_BASE9_ADDR );
		break;
	case 4:
		DecordVram( (void*)p_st2_s3_psc_ADRS, (void*)SCREEN_BASE11_ADDR );
		break;
	case 5:
		DecordVram( (void*)p_st2_s4_psc_ADRS, (void*)SCREEN_BASE13_ADDR );
		break;
	case 6:
		DecordPaletteWork( (void*)p_st2_bg_pcl_ADRS, PA_BG0+PA_COL0, 0xa0 );
		break;
	case 7:
		DecordCellSet( (CellData*)&ST_TYPE_32x16_Cell );
		break;
	case 8:
		DecordCellSet( (CellData*)&ST_CURSOR_Cell );
		break;
	case 9:
		DecordCellSet( (CellData*)&ST_IZYO_Cell );
		break;
	case 10:
		DecordPalSet( (PalData*)&ST_IZYO_Pal );
		break;
	case 11:
		DecordPalSet( (PalData*)&ST_CURSOR_Pal );
		break;
	case 12:
		DecordPaletteWork( (void*)st_type_pcl_ADRS, PA_OBJ13, 0x60 );
		P_SYSTEM->div_count = 0;
		return	TRUE;
	}
	P_SYSTEM->div_count += 1;

	return	FALSE;
}

//------------------------
//		�p���b�g�쐬
//------------------------
static void StatusPaletteMake(void)
{
	PaletteWorkSet( &st_moji_pal[0], F_PAL * 16 + COL_LGRAY, 2 );		// �ǉ������p�@�e
	PaletteWorkSet( &st_moji_pal[1], RED_PAL * 16 + COL_BLACK, 2 );		// �����@��
	PaletteWorkSet( &st_moji_pal[2], RED_PAL * 16 + COL_DGRAY, 2 );		// �����@�ԁi�e�j
	PaletteWorkSet( &st_moji_pal[3], WHITE_PAL * 16 + COL_BLACK, 2 );	// �����@��
	PaletteWorkSet( &st_moji_pal[0], WHITE_PAL * 16 + COL_DGRAY, 2 );	// �����@���i�e�j

	PaletteWorkSet( &st_moji_pal[4], BLUE_PAL * 16 + COL_BLACK, 2 );	// �����@��
	PaletteWorkSet( &st_moji_pal[5], BLUE_PAL * 16 + COL_DGRAY, 2 );	// �����@�i�e�j
	PaletteWorkSet( &st_moji_pal[6], RED_PAL2 * 16 + COL_BLACK, 2 );	// �����@��2
	PaletteWorkSet( &st_moji_pal[7], RED_PAL2 * 16 + COL_DGRAY, 2 );	// �����@��2�i�e�j

	PaletteWorkSet( &st_moji_pal[10], M_OYA_PAL * 16 + COL_BLACK, 2 );	// �����@�j�e
	PaletteWorkSet( &st_moji_pal[11], M_OYA_PAL * 16 + COL_DGRAY, 2 );	// �����@�j�e�i�e�j
	PaletteWorkSet( &st_moji_pal[8], F_OYA_PAL * 16 + COL_BLACK, 2 );	// �����@���e
	PaletteWorkSet( &st_moji_pal[9], F_OYA_PAL * 16 + COL_DGRAY, 2 );	// �����@���e�i�e�j

	PaletteWorkSet( &st_moji_pal[13], RC_POKE_PAL*16+COL_BLACK, 2 );	// ���A�J���[�p
	PaletteWorkSet( &st_moji_pal[14], RC_POKE_PAL*16+COL_DGRAY, 2 );	// ���A�J���[�p�i�e�j

	PaletteWorkSet( &st_moji_pal[13], RC_POKE_PAL*16+COL_WHITE, 2 );	// ���A�J���[�p
	PaletteWorkSet( &st_moji_pal[14], RC_POKE_PAL*16+COL_LGRAY, 2 );	// ���A�J���[�p�i�e�j

	PaletteWorkSet( &st_moji_pal[12], F_PAL * 16 + 0xe, 2 );		// �{�^���p�@�e
}

//------------------------------------
//		�X�e�[�^�X��ʏI������
//------------------------------------
// �Z�b�g
static void StatusEnd( u8 id )
{
	SePlay( SE_SELECT );
//	LightFadeReq( N_to_B );
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
	TaskTable[id].TaskAdrs = PokeStatusEnd;
}
// ���C��
static void PokeStatusEnd(u8 id)
{
	if( FieldRecvFifoClearWait() == TRUE )	return;

	if( FadeData.fade_sw == 0 ){
		PokeStatusBuf = P_SYSTEM->put_no;
		InitActor();
		ObjPalManInit();
		VoicePlayStop();
		m4aMPlayVolumeControl( &m4a_mplay000, 0xffff, 256 );
		MainProcChange( P_SYSTEM->ReturnAdrs );
		DelTask(id);
	}
}


//========================================================================================
//									�L�[�`�F�b�N
//========================================================================================
//---------------------------------------
//		�X�e�[�^�X��ʃ��C���^�X�N
//---------------------------------------
static void PokeStatusMainTask( u8 id )
{
	if( FadeData.fade_sw != 0 )	return;

	if( sys.Trg & U_KEY ){
		PokeChange( id, -1 );
		return;
	}
	if( sys.Trg & D_KEY ){
		PokeChange( id, 1 );
		return;
	}
	if( ( sys.Trg & L_KEY ) || LR_TrgCheck() == LRM_L ){
		PageChange( id, -1 );
		return;
	}
	if( ( sys.Trg & R_KEY ) || LR_TrgCheck() == LRM_R ){
		PageChange( id, 1 );
		return;
	}

	if( sys.Trg & A_BUTTON ){
		if( P_SYSTEM->page >= 2 )	WazaSelectInit(id);
		if( P_SYSTEM->page == 0 )	StatusEnd( id );
		return;
	}

	if( sys.Trg & B_BUTTON )
		StatusEnd( id );
}

//-----------------------------------
//		�Z�Y�ꃁ�C���^�X�N
//-----------------------------------
static void NewWazaMainTask( u8 id )
{
	if( FadeData.fade_sw != 0 )	return;

	if( sys.Trg & U_KEY ){
		TaskTable[id].work[0] = 4;
		WazaSelect( id, -1, &P_SYSTEM->waza_pos );
		return;
	}
	if( sys.Trg & D_KEY ){
		TaskTable[id].work[0] = 4;
		WazaSelect( id, 1, &P_SYSTEM->waza_pos );
		return;
	}
	if( ( sys.Trg & L_KEY ) || LR_TrgCheck() == LRM_L ){
		if( P_SYSTEM->page == 3 && 
			!( P_SYSTEM->waza_pos == 4 && P_SYSTEM->new_waza == 0 ) )
		{
			NMenuBoxClear( 0, ST_NAME02_Y+2, ST_NAME02_X+4, ST_NAME02_Y+6 );
		}
		PageChange( id, -1 );
		return;
	}
	if( ( sys.Trg & R_KEY ) || LR_TrgCheck() == LRM_R ){
		if( P_SYSTEM->page == P_SYSTEM->page_max )	return;
		if( P_SYSTEM->page == 2 && 
			!( P_SYSTEM->waza_pos == 4 && P_SYSTEM->new_waza == 0 ) )
		{
			NMenuBoxClear( 0, ST_NAME02_Y+2, ST_NAME02_X+4, ST_NAME02_Y+6 );
		}
		PageChange( id, 1 );
		return;
	}

	if( sys.Trg & A_BUTTON ){
		if( HidenCheck(id) == TRUE || P_SYSTEM->waza_pos == 4 ){
			P_SYSTEM->waza_pos2 = P_SYSTEM->waza_pos;
			ScriptParameter1 = (u16)P_SYSTEM->waza_pos2;
			StatusEnd( id );
		}else{
			SePlay( SE_HAZURE );
			HidenErrorInit( id, P_SYSTEM->waza_pos );
		}
		return;
	}
	if( sys.Trg & B_BUTTON ){
		P_SYSTEM->waza_pos2 = 4;
		ScriptParameter1 = (u16)P_SYSTEM->waza_pos2;
		StatusEnd( id );
	}
}

//--------------------------------------------
//		�Z��ʃ��C���^�X�N�i����ւ��\�j
//--------------------------------------------
static u8 WazaIrekae2ndCheck( PokemonParam * pp );

static void PokeWazaMainTask( u8 id )
{
	if( FadeData.fade_sw != 0 )	return;

	if( sys.Trg & U_KEY ){
		TaskTable[id].work[0] = 4;
		WazaSelect( id, -1, &P_SYSTEM->waza_pos );
		return;
	}
	if( sys.Trg & D_KEY ){
		TaskTable[id].work[0] = 4;
		WazaSelect( id, 1, &P_SYSTEM->waza_pos );
		return;
	}

	if( sys.Trg & A_BUTTON ){
		if( P_SYSTEM->waza_pos != 4 && P_SYSTEM->waza_lock == 0 ){
			if( WazaIrekae2ndCheck( &P_SYSTEM->pp ) == FALSE ){
				SePlay( SE_HAZURE );
				return;
			}
			SePlay( SE_SELECT );
			P_SYSTEM->waza_pos2 = P_SYSTEM->waza_pos;
			WAZA_CursorAnmChange(1);
			Add_WAZA_Cursor( WAZA_CURSOR_DL );
			TaskTable[id].TaskAdrs = WazaChangeTask;
		}else{
			SePlay( SE_SELECT );
			ReturnPageChenge(id);
		}
		return;
	}

	if( sys.Trg & B_BUTTON ){
		SePlay( SE_SELECT );
		ReturnPageChenge(id);
	}
}

// �Z���P�̂Ƃ��͓���ւ��s��
static u8 WazaIrekae2ndCheck( PokemonParam * pp )
{
	u8	i;

	for( i=1; i<4; i++ ){
		if( PokeWazaNoGet( pp, i ) != 0 )
			return	TRUE;
	}
	return	FALSE;
}


//-------------------------------
//		�Z����ւ��̃^�X�N
//-------------------------------
static void WazaChangeTask( u8 id )
{
	if( sys.Trg & U_KEY ){
		TaskTable[id].work[0] = 3;
		WazaSelect( id, -1, &P_SYSTEM->waza_pos2 );
		return;
	}
	if( sys.Trg & D_KEY ){
		TaskTable[id].work[0] = 3;
		WazaSelect( id, 1, &P_SYSTEM->waza_pos2 );
		return;
	}

	if( sys.Trg & A_BUTTON ){
		WazaChange( id, 1 );
		return;
	}

	if( sys.Trg & B_BUTTON ){
		WazaChange( id, -1 );
	}
}

//----------------------
//		�Z����ւ�
//----------------------
// PokemonParam
static void WazaChangePokePara(void)
{
	PokemonParam * pp;
	u8	pos1,pos2;
	u16	no1,no2;
	u8	pp1,pp2;
	u8	cnt,d_cnt1,d_cnt2;

	pp = P_SYSTEM->ppArray;
	pp = &pp[ P_SYSTEM->put_no ];

	pos1 = P_SYSTEM->waza_pos;
	pos2 = P_SYSTEM->waza_pos2;

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

// PokemonPasoParam
static void WazaChangePokePaso(void)
{
	PokemonPasoParam * psp;
	u8	pos1,pos2;
	u16	no1,no2;
	u8	pp1,pp2;
	u8	cnt,d_cnt1,d_cnt2;

	psp = P_SYSTEM->ppArray;
	psp = &psp[ P_SYSTEM->put_no ];

	pos1 = P_SYSTEM->waza_pos;
	pos2 = P_SYSTEM->waza_pos2;

	no1 = PokePasoParaGet( psp, ID_waza1 + pos1 );
	no2 = PokePasoParaGet( psp, ID_waza1 + pos2 );
	pp1 = PokePasoParaGet( psp, ID_pp1 + pos1 );
	pp2 = PokePasoParaGet( psp, ID_pp1 + pos2 );
	cnt = PokePasoParaGet( psp, ID_pp_count );
		
	d_cnt1 = ( cnt & pp_count_mask[ pos1 ] ) >> ( pos1 * 2 );
	d_cnt2 = ( cnt & pp_count_mask[ pos2 ] ) >> ( pos2 * 2 );
	cnt &= ~pp_count_mask[ pos1 ];
	cnt &= ~pp_count_mask[ pos2 ];
	cnt |= ( d_cnt1 << ( pos2 * 2 ) ) + ( d_cnt2 << ( pos1 * 2 ) );
		
	PokePasoParaPut( psp, ID_waza1 + pos1, (u8 *)&no2 );
	PokePasoParaPut( psp, ID_waza1 + pos2, (u8 *)&no1 );
	PokePasoParaPut( psp, ID_pp1 + pos1, &pp2 );
	PokePasoParaPut( psp, ID_pp1 + pos2, &pp1 );
	PokePasoParaPut( psp, ID_pp_count, &cnt );
}

// ���C��
static void WazaChange2( u8 id )
{
	if( P_StExChange2() == FALSE ) return;
	P_SYSTEM->div_count = 0;
	WazaInfoPut( &P_SYSTEM->pp, &P_SYSTEM->waza_pos );
	TaskTable[id].TaskAdrs = PokeWazaMainTask;
}

static void WazaChange( u8 id, s8 flg )
{
	SePlay( SE_SELECT );
	Del_WAZA_Cursor( WAZA_CURSOR_DL );
	WAZA_CursorAnmChange(0);

	// A�{�^��
	if( flg == 1 ){
		if( P_SYSTEM->waza_pos != P_SYSTEM->waza_pos2 ){

			if( P_SYSTEM->adrs_type == PP_TYPE )	WazaChangePokePara();	// ���X�g
			else									WazaChangePokePaso();	// �{�b�N�X

			P_SYSTEM->waza_pos = P_SYSTEM->waza_pos2;
			NowPokeParamDataSet( &P_SYSTEM->pp );
			P_SYSTEM->div_count = 1;
			TaskTable[id].TaskAdrs = WazaChange2;
			return;
		}
	// B�{�^��
	}else{
		WazaInfoPut( &P_SYSTEM->pp, &P_SYSTEM->waza_pos );
	}

	TaskTable[id].TaskAdrs = PokeWazaMainTask;
}

//-------------------------
//		���̋Z���擾
//-------------------------
static void WazaSelect( u8 id, s8 next, u8 * waza_pos )
{
	s8	now_pos,max,i;

	SePlay(SE_SELECT);

	now_pos = *waza_pos;
	max = (s8)TaskTable[id].work[0];

	for( i=0; i<4; i++ ){
		now_pos += next;

		if( now_pos > max ){
			now_pos = 0;
		}else{
			if( now_pos < 0 ){
				now_pos = max;
			}
		}

		if( PokeWazaNoGet( &P_SYSTEM->pp, now_pos ) != 0 || now_pos == 4 )
			break;
	}

	if( ( *waza_pos == 4 && now_pos != 4 && P_SYSTEM->new_waza == 0 ) ||
		TaskTable[id].work[13] == 1 )
	{
		WazaParamWin( 3, now_pos );
		ContestParamWin( 3, now_pos );
		TaskTable[id].work[13] = 0;
	}
	if( *waza_pos != 4 && now_pos == 4 && P_SYSTEM->new_waza == 0 ){
		WazaParamWin( -3, now_pos );
		ContestParamWin( -3, now_pos );
	}

	*waza_pos = now_pos;

	if( waza_pos == &P_SYSTEM->waza_pos )	Waza_CursorbanishClear(0);
	else									Waza_CursorbanishClear(1);

	WazaInfoPut( &P_SYSTEM->pp, waza_pos );
}


static void WazaSelectInit( u8 id )
{
	SePlay( SE_SELECT );
	P_SYSTEM->waza_pos = 0;
	WazaParamWin( 3, 0 );
	ContestParamWin( 3, 0 );

	if( P_SYSTEM->waza_lock == 0 ){
		P_SYSTEM->comment2 = IREKAE_COM;
		PokeStFuncInfoMsgPut();
	}

	FifthWazaWin(0);
	P_StFifthWazaPut( &P_SYSTEM->pp );
	Add_WAZA_Cursor( WAZA_CURSOR_L );
	TaskTable[id].TaskAdrs = PokeWazaMainTask;
}

//---------------------------------------------
//		�Z�I�����I�����ăy�[�W�I���ɖ߂�
//---------------------------------------------
static void ReturnPageChenge( u8 id )
{
	if( P_SYSTEM->waza_pos != 4 ){
		WazaParamWin( -3, 0 );			// ���ʃE�B���h�E��߂�
		ContestParamWin( -3, 0 );
	}
	Del_WAZA_Cursor( WAZA_CURSOR_L );
	FifthWazaWin(1);			// �Z�E�B���h�E��߂�
	NMenuBoxClear( ST_WAZA_X, ST_WAZA_Y+8, ST_WAZA_X+13, ST_WAZA_Y+9 );
	NMenuBoxClear( WAZA_INFO_X, WAZA_INFO_Y, WAZA_INFO_X+17, WAZA_INFO_Y+3 );

	P_SYSTEM->comment2 = KIRIKAE_COM;
	PokeStFuncInfoMsgPut();

	TaskTable[id].TaskAdrs = PokeStatusMainTask;
}


//----------------------------
//		�Z�Y�ꏉ���ݒ�
//----------------------------
static void NewWazaInitTask( u8 id )
{
	if( TaskTable[id].work[0] == 5 ){
		P_SYSTEM->waza_pos = 0;
		if( P_SYSTEM->new_waza != 0 ){
			WazaParamWin( 9, P_SYSTEM->waza_pos );
			ContestParamWin( 9, P_SYSTEM->waza_pos );
		}
		FifthWazaWin(0);
		P_StFifthWazaPut( &P_SYSTEM->pp );
		Add_WAZA_Cursor( WAZA_CURSOR_L );
		TaskTable[id].work[0] = 0;
		TaskTable[id].TaskAdrs = NewWazaMainTask;
	}else	TaskTable[id].work[0]++;
}


//========================================================================================
//									�؂�ւ�����
//========================================================================================
//------------------------------
//		�����y�[�WBG�Z�b�g
//------------------------------
static void StPageBGInit(void)
{
	if( P_SYSTEM->page != 0 ){
		PagePosBGSet();
		ScrX1 = 256;
		if( P_SYSTEM->page == 1 )
			*(vu16 *)REG_BG1CNT = BG1_SCRN_CNG + SCR_BASE_P2;
		if( P_SYSTEM->page == 2 )
			*(vu16 *)REG_BG1CNT = BG1_SCRN_CNG + SCR_BASE_P3;
		if( P_SYSTEM->page == 3 )
			*(vu16 *)REG_BG1CNT = BG1_SCRN_CNG + SCR_BASE_P4;
	}
}

//---------------------------
//		�y�[�W�؂�ւ�
//---------------------------
#define	PAGE_SCR_SPD	32

#define	WK_SEQ	0

static void BGPageSub( u8 id )
{
	s16	* wk = TaskTable[id].work;

	switch( wk[ WK_SEQ ] ){
	// ���̃y�[�W��D��x�̒ႢBG�ɏ���
	case 0:
		if( P_SYSTEM->bg_flg == 0 ){
			if( P_SYSTEM->page != 0)	ScrX2 = 256;
			if( P_SYSTEM->page == 1 )
				*(vu16 *)REG_BG2CNT = BG2_SCRN_CNG + SCR_BASE_P2;
			if( P_SYSTEM->page == 2 )
				*(vu16 *)REG_BG2CNT = BG2_SCRN_CNG + SCR_BASE_P3;
		}else{
			if( P_SYSTEM->page != 0)	ScrX1 = 256;
			if( P_SYSTEM->page == 1 )
				*(vu16 *)REG_BG1CNT = BG1_SCRN_CNG + SCR_BASE_P2;
			if( P_SYSTEM->page == 2 )
				*(vu16 *)REG_BG1CNT = BG1_SCRN_CNG + SCR_BASE_P3;
		}
		wk[ WK_SEQ ]++;
		break;
	// �D��x�̍���BG���X�N���[��
	case 1:
		if( P_SYSTEM->bg_flg == 0 ){
			ScrX1 -= PAGE_SCR_SPD;
			if( ScrX1 == 0 ){
				*(vu16 *)REG_BG1CNT = BG1_PRI_CNG + BG_PRIORITY_2;
				*(vu16 *)REG_BG2CNT = BG2_PRI_CNG + BG_PRIORITY_1;
				wk[ WK_SEQ ]++;
			}
		}else{
			ScrX2 -= PAGE_SCR_SPD;
			if( ScrX2 == 0 ){
				*(vu16 *)REG_BG1CNT = BG1_PRI_CNG + BG_PRIORITY_1;
				*(vu16 *)REG_BG2CNT = BG2_PRI_CNG + BG_PRIORITY_2;
				wk[ WK_SEQ ]++;
			}
		}
		break;

	case 2:
		P_SYSTEM->comment1 = P_SYSTEM->page+1;

		if( P_SYSTEM->type == NEW_WAZA || P_SYSTEM->type == DEL_WAZA ){
			P_SYSTEM->comment2 = 0;
			P_StFifthWazaPut( &P_SYSTEM->pp );
			WazaInfoPut( &P_SYSTEM->pp, &P_SYSTEM->waza_pos );
			P_WazaLeftDef( P_SYSTEM->waza_pos );
		}else{
			if( P_SYSTEM->page >= 2 &&
				( P_SYSTEM->waza_lock == 0 || P_SYSTEM->type == BTL_WAZA ) )
			{
				P_SYSTEM->comment2 = KIRIKAE_COM;
			}else{
				if( P_SYSTEM->page == 0 )	P_SYSTEM->comment2 = YAMERU_COM;
				else						P_SYSTEM->comment2 = 0;
			}
		}
		wk[ WK_SEQ ]++;
		break;

	case 3:
		PagePosBGSet();
		PokeStFuncInfoMsgPut();
		wk[ WK_SEQ ]++;
		break;

	case 4:
		P_StDataParam2[ P_SYSTEM->page ]( &P_SYSTEM->pp );
		P_SYSTEM->bg_flg ^= 1;
		wk[ WK_SEQ ]++;
		break;

	case 5:
//		if( FieldRecvFifoClearWait() == TRUE )	break;
		TaskTable[id].TaskAdrs = PCReturnTask;
	}
}

static void BGPageAdd( u8 id )
{
	s16	* wk = TaskTable[id].work;

	switch( wk[ WK_SEQ ] ){
	//	�D��x�̒ႢBG����ʊO�փX�N���[��
	case 0:
		if( P_SYSTEM->bg_flg == 0 )	ScrX2 = 0;
		else						ScrX1 = 0;
		wk[ WK_SEQ ]++;
		break;
	//	BG������������
	case 1:
		if( P_SYSTEM->bg_flg == 0 ){
			if( P_SYSTEM->page == 1 )
				*(vu16 *)REG_BG2CNT = BG2_PRI_SCRN_CNG + SCR_BASE_P2 + BG_PRIORITY_1;
			if( P_SYSTEM->page == 2 )
				*(vu16 *)REG_BG2CNT = BG2_PRI_SCRN_CNG + SCR_BASE_P3 + BG_PRIORITY_1;
			if( P_SYSTEM->page == 3 )
				*(vu16 *)REG_BG2CNT = BG2_PRI_SCRN_CNG + SCR_BASE_P4 + BG_PRIORITY_1;
			*(vu16 *)REG_BG1CNT = BG1_PRI_CNG + BG_PRIORITY_2;

		}else{
			if( P_SYSTEM->page == 1 )
				*(vu16 *)REG_BG1CNT = BG1_PRI_SCRN_CNG + SCR_BASE_P2 + BG_PRIORITY_1;
			if( P_SYSTEM->page == 2 )
				*(vu16 *)REG_BG1CNT = BG1_PRI_SCRN_CNG + SCR_BASE_P3 + BG_PRIORITY_1;
			if( P_SYSTEM->page == 3 )
				*(vu16 *)REG_BG1CNT = BG1_PRI_SCRN_CNG + SCR_BASE_P4 + BG_PRIORITY_1;
			*(vu16 *)REG_BG2CNT = BG2_PRI_CNG + BG_PRIORITY_2;
		}
		wk[ WK_SEQ ]++;
		break;
	//	BG���X�N���[���C��
	case 2:
		if( P_SYSTEM->bg_flg == 0 ){
			ScrX2 += PAGE_SCR_SPD;
			if( ScrX2 == 256 )	wk[ WK_SEQ ]++;
		}else{
			ScrX1 += PAGE_SCR_SPD;
			if( ScrX1 == 256 )	wk[ WK_SEQ ]++;
		}
		break;

	case 3:
		P_SYSTEM->comment1 = P_SYSTEM->page+1;

		if( P_SYSTEM->type == NEW_WAZA || P_SYSTEM->type == DEL_WAZA ){
			P_SYSTEM->comment2 = 0;
			P_StFifthWazaPut( &P_SYSTEM->pp );
			WazaInfoPut( &P_SYSTEM->pp, &P_SYSTEM->waza_pos );
			P_WazaLeftDef( P_SYSTEM->waza_pos );
		}else{
			if( P_SYSTEM->page >= 2 &&
				( P_SYSTEM->waza_lock == 0 || P_SYSTEM->type == BTL_WAZA ) )
			{
				P_SYSTEM->comment2 = KIRIKAE_COM;
			}else{
				if( P_SYSTEM->page == 0 )	P_SYSTEM->comment2 = YAMERU_COM;
				else						P_SYSTEM->comment2 = 0;
			}
		}
		wk[ WK_SEQ ]++;
		break;

	case 4:
		PagePosBGSet();
		PokeStFuncInfoMsgPut();
		wk[ WK_SEQ ]++;
		break;

	case 5:
		P_StDataParam2[ P_SYSTEM->page ]( &P_SYSTEM->pp );
		P_SYSTEM->bg_flg ^= 1;
		wk[ WK_SEQ ]++;
		break;

	case 6:
		if( FieldRecvFifoClearWait() == TRUE )	break;
		TaskTable[id].TaskAdrs = PCReturnTask;
	}
}

static void BGPageSet( u8 id, s8 move )
{
	P_SYSTEM->page += move;
	PCReturnTask = TaskTable[id].TaskAdrs;

	P_StRightClear();

	TaskTable[id].work[WK_SEQ] = 0;

	if( move == -1 )	TaskTable[id].TaskAdrs = BGPageSub;
	else{
		TaskTable[id].TaskAdrs = BGPageAdd;
		TaskTable[id].TaskAdrs(id);
	}
}

static void PageChange( u8 id, s8 move )
{
	// �^�}�S�͐؂�ւ��֎~
	if( PokeParaGet( &P_SYSTEM->pp, ID_tamago_flag ) != 0 )
		return;

	if( move == -1 && P_SYSTEM->page == P_SYSTEM->page_init )	return;
	if( move ==  1 && P_SYSTEM->page == P_SYSTEM->page_max  )	return;

	SePlay( SE_SELECT );
	BGPageSet( id, move );
}

//-----------------------------
//		�|�P�����؂�ւ�
//-----------------------------
static s8 NextPokeGet( s8 move );
static s8 NextPokeGetMulti( s8 move );

static void PokeChange( u8 id, s8 move )
{
	s8	next;

	if( P_SYSTEM->adrs_type == PSP_TYPE ){
		if( P_SYSTEM->page != 0 ){
			if( move == 1 )	move = NEXT_OFS;
			else			move = PREV_OFS;
		}else{
			if( move == 1 )	move = NEXT_OFS_TAMAGO;
			else			move = PREV_OFS_TAMAGO;
		}
		next = BoxNextOffsetSearch(
				(PokemonPasoParam *)P_SYSTEM->ppArray,
				P_SYSTEM->put_no, P_SYSTEM->poke_max, (u8)move );
	}else{
		if( ItemToolSioCheck() == TRUE && MultiCheck() == TRUE )
			next = NextPokeGetMulti( move );
		else
			next = NextPokeGet( move );
	}

	if( next == -1 )	return;

	SePlay( SE_SELECT );

	if( GetPokeCondition( &P_SYSTEM->pp ) != ST_NORMAL )
		ConditionWin(-2);

	P_SYSTEM->put_no = next;
	P_SYSTEM->PokeTask = TaskTable[id].TaskAdrs;
	TaskTable[id].TaskAdrs = NextPokeStatus;
}

//-----------------------------------------------------
//	���̃|�P�������擾
//-----------------------------------------------------
// �ʏ�
static s8 NextPokeGet( s8 move )
{
	s8	cnt = 0;

	if( P_SYSTEM->page == 0 ){
		if( move == -1 && P_SYSTEM->put_no == 0 )	return	-1;
		if( move == 1 && P_SYSTEM->put_no >= P_SYSTEM->poke_max )	return -1;
		return	P_SYSTEM->put_no + move;
	}

	while(1){
		cnt += move;
		if( P_SYSTEM->put_no + cnt < 0 ||
			P_SYSTEM->put_no + cnt > P_SYSTEM->poke_max )
		{
			return	-1;
		}

		if( PokeParaGet( &PokeParaMine[ P_SYSTEM->put_no + cnt ], ID_tamago_flag ) == 0 )
			return	P_SYSTEM->put_no + cnt;
	}

	return	-1;
}

// �}���`�o�g���p ---------------------------------------
static const u8 multi_poke_tbl[] = { 0, 2, 3, 1, 4, 5 };

static u8 MultiNextPokeCheck( PokemonParam * pp )
{
	if( PokeParaGet( pp, ID_monsno ) != 0 &&
		!( P_SYSTEM->page == 0 && PokeParaGet( pp, ID_tamago_flag ) != 0 ) )
	{
		return	TRUE;
	}
	return	FALSE;
}

static s8 NextPokeMultiPlus( u8 tbl_no )
{
	while(1){
		tbl_no++;
		if( tbl_no == 6 )	return	-1;

		if( MultiNextPokeCheck( &PokeParaMine[ multi_poke_tbl[tbl_no] ] ) == TRUE )
			break;
	}
	return	(s8)( multi_poke_tbl[tbl_no] );
}

static s8 NextPokeMultiMinus( u8 tbl_no )
{
	while(1){
		if( tbl_no == 0 )	return	-1;
		tbl_no--;

		if( MultiNextPokeCheck( &PokeParaMine[ multi_poke_tbl[tbl_no] ] ) == TRUE )
			break;
	}
	return	(s8)( multi_poke_tbl[tbl_no] );
}

// ���C��
static s8 NextPokeGetMulti( s8 move )
{
	u8	tbl_no = 0;
	u8	i;

	// ���݂̈ʒu�`�F�b�N
	for( i=0; i<TEMOTI_POKEMAX; i++ ){
		if( multi_poke_tbl[i] == P_SYSTEM->put_no ){
			tbl_no = i;
			break;
		}
	}

	// ���E�ʒu
	if( ( move == -1 && tbl_no == 0 ) || ( move == 1 && tbl_no == 5 ) )
		return	-1;

	if( move == 1 )		return	NextPokeMultiPlus( tbl_no );
	return	NextPokeMultiMinus( tbl_no );
}

//------------------------------------
//		���̃|�P�����ɐ؂�ւ���
//------------------------------------
static void NextPokeStatus( u8 id )
{
	switch( sys.InitSeqWork ){
	case 0:
		VoicePlayStop();
		sys.InitSeqWork++;
		break;

	case 1:
		AllDelActor( &ActWork[ P_SYSTEM->pokeact ] );
		sys.InitSeqWork++;
		break;

	case 2:
		AllDelActor( &ActWork[ P_SYSTEM->ballact ] );
		sys.InitSeqWork++;
		break;

	case 3:
		P_SYSTEM->div_count = 0;
		P_SYSTEM->waza_pos = 0;
		sys.InitSeqWork++;
		break;

	case 4:
		NowPokeParamDataSet( &P_SYSTEM->pp );
		if( GetPokeCondition( &P_SYSTEM->pp ) != ST_NORMAL )
			ConditionWin(2);
		PokerusMark( &P_SYSTEM->pp );
		sys.InitSeqWork++;
		break;

	case 5:
		P_SYSTEM->pokeact = St_PokemonSet2( &P_SYSTEM->pp, &P_SYSTEM->div_count );
		if( P_SYSTEM->pokeact == 0xff )	break;
		P_SYSTEM->div_count = 0;
		if( PokeParaGet( &P_SYSTEM->pp, ID_tamago_flag ) != 0 )	ScrX3 = 256;
		else													ScrX3 = 0;
		sys.InitSeqWork++;
		break;

	case 6:
		AddBoxMarkActInit( &P_SYSTEM->pp );
		sys.InitSeqWork++;
		break;

	case 7:
		GetMonsBallActSet( &P_SYSTEM->pp );
		sys.InitSeqWork++;
		break;

	case 8:
		if( P_StExChange2() == FALSE ) return;
		P_SYSTEM->div_count = 0;
		sys.InitSeqWork++;
		break;

	default:
		if( FieldRecvFifoClearWait() == TRUE )	break;
		sys.InitSeqWork = 0;
		TaskTable[id].TaskAdrs = P_SYSTEM->PokeTask;
	}
}

static u8 P_StExChange2(void)
{
	switch( P_SYSTEM->div_count ){
	case 0:
		P_StLeftParam( &P_SYSTEM->pp );
		P_SYSTEM->div_count += 1;
		break;

	default:	// �e�y�[�W�̃p�����[�^
		P_StDataParam[ P_SYSTEM->page ]( &P_SYSTEM->pp );
		return	TRUE;
	}

	return	FALSE;
}

//-------------------------------------
//		�y�[�W���̃p�����[�^�\��
//-------------------------------------
// ����������1
static void P_StChange01( PokemonParam * pp )
{
	P_StRight01Clear();
	P_StRight01Param(pp);
}

// ����������2
static void P_StChange02( PokemonParam * pp )
{
	P_StRight02Clear();
	P_StRight02Param(pp);
}

// �g����Z
static void P_StChange03( PokemonParam * pp )
{
	P_StWazaClear();
	P_StWazaPut1(pp);
}


//========================================================================================
//										�c�[����
//========================================================================================
//-------------------------------------------
//		�I�𒆂̃|�P�����̃f�[�^���擾
//-------------------------------------------
static void NowPokeParamDataSet( PokemonParam * pp )
{
	PokemonParam * ppArray;
	PokemonPasoParam * pspArray;

	if( P_SYSTEM->adrs_type == PP_TYPE ){
		ppArray = P_SYSTEM->ppArray;
		*pp = ppArray[ P_SYSTEM->put_no ];
	}else{
		pspArray = P_SYSTEM->ppArray;
		PokeReplace( &pspArray[ P_SYSTEM->put_no ], pp );
	}
}

//-----------------------------------
//			�|�P�����Z�b�g
//	�߂�l : ������ = 0xff
//			 �I��   = Actor Number
//-----------------------------------
static u8 St_PokemonSet2( PokemonParam * pp, u8 * flg )
{
	PalData * pal;
	u16 monsno;
	u32	rnd;
	u32	id;

	switch( *flg ){
	case 0:
		monsno = PokeParaGet( pp, ID_monsno_egg );
		rnd    = PokeParaGet( pp, ID_personal_rnd );
		DecordPokeCellMakeBuchi(
				&PM2FObjDataCell[monsno], pm2fpara[monsno].size, pm2fpara[monsno].patcnt,
				(void*)0x2000000, (void*)DecordAdrs[BSS_CLIENT_ENEMY], monsno, rnd );
		break;
	case 1:
		monsno = PokeParaGet( pp, ID_monsno_egg );
		rnd    = PokeParaGet( pp, ID_personal_rnd );
		id     = PokeParaGet( pp, ID_id_no );
		pal    = (PalData *)PokePalDataGetPara( monsno, id, rnd );
		DecordPalSet( pal );
		PokeActHeaderMake( pal->reg_num, 1 );
		break;
	default:
		return	St_PokeActSet(pp);
	}

	*flg += 1;

	return	0xff;
}

//----------------------
//		�Z�ԍ��擾
//----------------------
static u16 PokeWazaNoGet( PokemonParam * pp, u8 pos )
{
	u16 ret;
	
	switch( pos ){
	case 0:
		ret = PokeParaGet( pp, ID_waza1 );
		break;
	case 1:
		ret = PokeParaGet( pp, ID_waza2 );
		break;
	case 2:
		ret = PokeParaGet( pp, ID_waza3 );
		break;
	default:
		ret = PokeParaGet( pp, ID_waza4 );
	}
	return ret;
}

//-------------------
//		PP�擾
//-------------------
static u16 PokeWazaPPGet( PokemonParam * pp, u8 pos )
{
	u16 ret;
	
	switch( pos ){
	case 0:
		ret = PokeParaGet( pp, ID_pp1 );
		break;
	case 1:
		ret = PokeParaGet( pp, ID_pp2 );
		break;
	case 2:
		ret = PokeParaGet( pp, ID_pp3 );
		break;
	default:
		ret = PokeParaGet( pp, ID_pp4 );
	}
	return ret;
}

//---------------------------
//		 ��`�Z���ǂ���
//---------------------------
static u8 HidenCheck( u8 id )
{
	PokemonParam	pp;
	u16	waza;

	NowPokeParamDataSet( &pp );

	waza = PokeWazaNoGet( &pp, P_SYSTEM->waza_pos );

	if( DelWazaHidenCheck( waza ) == TRUE && P_SYSTEM->type != DEL_WAZA )
		return	FALSE;
	return	TRUE;
}

//--------------------------------------------
//	�Z�Y��E��`�Z�G���[
//--------------------------------------------
static void HidenErrorMain( u8 id )
{
	s16 * wk = TaskTable[id].work;

	if( wk[14] < 4 ){
		wk[14]++;
		return;
	}

	if( sys.Trg & U_KEY ){
		TaskTable[id].TaskAdrs = NewWazaMainTask;
		wk[0]  = 4;
		wk[13] = 1;
		P_SYSTEM->waza_pos = (u8)wk[15];
		WazaSelect( id, -1, &P_SYSTEM->waza_pos );
		return;
	}
	if( sys.Trg & D_KEY ){
		TaskTable[id].TaskAdrs = NewWazaMainTask;
		wk[0]  = 4;
		wk[13] = 1;
		P_SYSTEM->waza_pos = (u8)wk[15];
		WazaSelect( id, 1, &P_SYSTEM->waza_pos );
		return;
	}
	if( ( sys.Trg & L_KEY ) || LR_TrgCheck() == LRM_L ){
		if( P_SYSTEM->page == 2 )	return;
		if( P_SYSTEM->page == 3 && 
			!( P_SYSTEM->waza_pos == 4 && P_SYSTEM->new_waza == 0 ) )
		{
			NMenuBoxClear( 0, ST_NAME02_Y+2, ST_NAME02_X+4, ST_NAME02_Y+6 );
		}
		TaskTable[id].TaskAdrs = NewWazaMainTask;
		PageChange( id, -1 );
		WazaParamWin( 1, (u8)wk[15] );
		ContestParamWin( 1, (u8)wk[15] );
		return;
	}
	if( ( sys.Trg & R_KEY ) || LR_TrgCheck() == LRM_R ){
		if( P_SYSTEM->page == P_SYSTEM->page_max )	return;
		if( P_SYSTEM->page == 2 && 
			!( P_SYSTEM->waza_pos == 4 && P_SYSTEM->new_waza == 0 ) )
		{
			NMenuBoxClear( 0, ST_NAME02_Y+2, ST_NAME02_X+4, ST_NAME02_Y+6 );
		}
		TaskTable[id].TaskAdrs = NewWazaMainTask;
		PageChange( id, 1 );
		WazaParamWin( 1, (u8)wk[15] );
		ContestParamWin( 1, (u8)wk[15] );
		return;
	}

	if( ( sys.Trg & A_BUTTON ) || ( sys.Trg & B_BUTTON ) ){
		WazaParamWin( 3, (u8)wk[15] );
		ContestParamWin( 3, (u8)wk[15] );
		TaskTable[id].TaskAdrs = NewWazaMainTask;
	}
}

static void HidenErrorInit( u8 id, u8 pos )
{
	s16 * wk = TaskTable[id].work;

	wk[14] = 0;
	wk[15] = pos;

	WazaParamWin( -3, 4 );
	ContestParamWin( -3, 4 );

	NMenuBoxClear( WAZA_INFO_X, WAZA_INFO_Y, WAZA_INFO_X+17, WAZA_INFO_Y+3 );
	NMenuMsgWrite( msg_hiden_del_error, WAZA_INFO_X, WAZA_INFO_Y );

	TaskTable[id].TaskAdrs = HidenErrorMain;
}

//----------------------------------------
//		�Q�ڂɑI�������Z�ԍ���Ԃ�
//----------------------------------------
u8	GetNewWazaSelectPos(void)
{
	return	P_SYSTEM->waza_pos2;
}


//========================================================================================
//										�\���֌W
//========================================================================================
//------------------------
//		�ǉ������\��
//------------------------
static void P_StSpMojiPutPal( u8 no, u8 pal, u8 x, u8 y )
{
	u32   trans_pos;
	u16 * ptrans;

	trans_pos = SCREEN_BASE30_ADDR + ( x << 1 ) + ( y << 6 );
	ptrans    = (void *)( trans_pos );

	ptrans[0]  = ( pal << 12 ) + 512 + ( no << 1 );
	ptrans[32] = ( pal << 12 ) + 512 + ( no << 1 ) + 1;
}

static void P_StSpMojiPut( u8 no, u8 x, u8 y )
{
	P_StSpMojiPutPal( no, DEFO_PAL, x, y );
}

//--------------------------------------------------
//	���A�J���[�|�P�����̔w�i�̐F��ύX
//--------------------------------------------------
static u8 PokeBGColorRareChange( PokemonParam * pp )
{
	if( PokeRareGet( pp ) == 0 ){
		PaletteWorkSet( &st_rare_bg_pal[0], 4, 2 );
		return	0;
	}

	PaletteWorkSet( &st_rare_bg_pal[1], 4, 2 );
	return	1;
}

//--------------------------------
//		"����������"��ʍ�
//--------------------------------
static void P_StLeftParam( PokemonParam * pp )
{
	u16	monsno;
	u8	level;
	u8	rare_flg;

	if( PokeParaGet( pp, ID_tamago_flag ) != 0 ){
		NMenuBoxClear( ST_MONSNO_X, ST_MONSNO_Y, ST_MONSNO_X+3, ST_MONSNO_Y+1 );
		NMenuBoxClear( ST_LV_X, ST_LV_Y, ST_LV_X+3, ST_LV_Y+1 );
		NMenuBoxClear( ST_NAME02_X-2, ST_NAME02_Y, ST_NAME02_X+7, ST_NAME02_Y+3 );
		PokeNickNameSet( pp, StrTempBuffer0 );
		PalChangeMojiPut( StrTempBuffer0, WHITE_PAL, ST_LV_X, ST_LV_Y );
		PaletteWorkSet( &st_rare_bg_pal[0], 4, 2 );
		return;
	}

	monsno = PokemonNumberGet( PokeParaGet( pp, ID_monsno ) );
	level  = PokeParaGet( pp, ID_level );

	// �w�i�ύX
	rare_flg = PokeBGColorRareChange( pp );

	// �}��No
	if( monsno != 0xffff ){
		if( rare_flg == 0 ){
			P_StSpMojiPut( SP_MOJI_NO, ST_MONSNO_X, ST_MONSNO_Y );
			PalChangeNumPut( monsno, WHITE_PAL, 3, NUM_MODE_ZERO, ST_MONSNO_X+1, ST_MONSNO_Y );
		}else{
			P_StSpMojiPutPal( SP_MOJI_NO, RC_POKE_PAL, ST_MONSNO_X, ST_MONSNO_Y );
			PalChangeNumPut( monsno, RC_POKE_PAL, 3, NUM_MODE_ZERO, ST_MONSNO_X+1, ST_MONSNO_Y );
		}
	}else	NMenuBoxClear( ST_MONSNO_X, ST_MONSNO_Y, ST_MONSNO_X+3, ST_MONSNO_Y+1 );

	// Lv
	P_StSpMojiPut( SP_MOJI_LV_W, ST_LV_X, ST_LV_Y );		// Lv
	NMenuBoxClear( ST_LV_X+1, ST_LV_Y, ST_LV_X+3, ST_LV_Y+1 );
	PalChangeNumPut( level, WHITE_PAL, 3, NUM_MODE_LEFT, ST_LV_X+1, ST_LV_Y );

	// �j�b�N�l�[��
	PokeNickNameSet( pp, StrTempBuffer0 );
	NMenuBoxClear( ST_NAME02_X, ST_NAME02_Y, ST_NAME02_X+4, ST_NAME02_Y+3 );
	if( level < 10 )
		PalChangeMojiPut( StrTempBuffer0, WHITE_PAL, ST_NAME02_X-1, ST_NAME02_Y );
	else
		PalChangeMojiPut( StrTempBuffer0, WHITE_PAL, ST_NAME02_X, ST_NAME02_Y );

	// �|�P������ / ����
	PutPokeName( pp );
}

//------------------------
//		��ʉE�N���A
//------------------------
static void P_StRightClear(void)
{
	u8	i;

	for( i=0; i<WAZA_CURSOR_DR; i++ )
		ST_ActBanish( i, 1 );

	NMenuBoxClear( 11, 4, 28, 18 );
}


//-----------------------
//		�y�[�W1�E
//-----------------------
static void P_StRight01Def(void)
{
	PalChangeMojiPut( str_pst_Oya, WHITE_PAL, ST_OYA_X, ST_OYA_Y );	// ����/
	NMenuMsgWrite( str_pst_Type, ST_TYPE_X, ST_TYPE_Y );			// �^�C�v/
	P_StSpMojiPut( SP_MOJI_ID, ST_ID_X, ST_ID_Y );			// ID
	P_StSpMojiPut( SP_MOJI_NO, ST_ID_X+1, ST_ID_Y );		// No
}

static void P_StRight01Param( PokemonParam * pp )
{
	u32	id_no;
	u16	mons_no;
	u8	msg;
	u8	i;
	u16	natsuki;

	for( i=0; i<POKE_CURSOR_U; i++ )
		ST_ActBanish( i, 1 );

	NMenuBoxClear( ST_OYANAME_X, ST_OYANAME_Y, ST_OYANAME_X+6, ST_OYANAME_Y+1 );
	NMenuBoxClear( ST_SP_X, ST_SP_Y, ST_SP_X+17, ST_SP_Y+3 );

	// �^�}�S�̂Ƃ�
	if( PokeParaGet( pp, ID_tamago_flag ) != 0 ){
		PalChangeMojiPut( str_hate5, WHITE_PAL, ST_OYANAME_X, ST_OYANAME_Y );
		PalChangeMojiPut( str_hate5, WHITE_PAL, ST_ID_X+2, ST_ID_Y );

		ST_TYPE_ActMove( 9, (ST_TYPE_X+4)<<3, ST_TYPE_Y<<3, 0 );

		natsuki = PokeParaGet( pp, ID_friend );
		if( natsuki <= 5 )	NMenuMsgWrite( msg_pst_egg03, ST_SP_X, ST_SP_Y );
		else	if( natsuki <= 10 )	NMenuMsgWrite( msg_pst_egg02, ST_SP_X, ST_SP_Y );
		else	if( natsuki <= 40 )	NMenuMsgWrite( msg_pst_egg01, ST_SP_X, ST_SP_Y );
		else	NMenuMsgWrite( msg_pst_egg00, ST_SP_X, ST_SP_Y );

		P_StTamagoMemoPut( pp, ST_MEMO_X, ST_MEMO_Y );
		return;
	}

	// �p�[�\�i��
	// �e
	PokeParaGet( pp, ID_oyaname, StrTempBuffer0 );
	if( PokeParaGet( pp, ID_oyasex ) == 0 )
		PalChangeMojiPut( StrTempBuffer0, M_OYA_PAL, ST_OYANAME_X, ST_OYANAME_Y );
	else
		PalChangeMojiPut( StrTempBuffer0, F_OYA_PAL, ST_OYANAME_X, ST_OYANAME_Y );
	// ID
	id_no = PokeParaGet( pp, ID_id_no ) & 0xffff;
	PalChangeNumPut( id_no, WHITE_PAL, 5, NUM_MODE_ZERO, ST_ID_X+2, ST_ID_Y );

	// �^�C�v
	mons_no = PokeParaGet( pp, ID_monsno );
	ST_TYPE_ActMove( PPD[ mons_no ].type1, (ST_TYPE_X+4)<<3, ST_TYPE_Y<<3, 0 );
	if( PPD[ mons_no ].type1 != PPD[ mons_no ].type2 )
		ST_TYPE_ActMove( PPD[ mons_no ].type2, (ST_TYPE_X+9)<<3, ST_TYPE_Y<<3, 1 );

	// ����
	msg = SpecialAbirityGet( PokeParaGet( pp, ID_monsno ), PokeParaGet( pp, ID_speabi ) );
	PalChangeMojiPut( SpecialAbirityMsg[msg], WHITE_PAL, ST_SP_X, ST_SP_Y );
	NMenuMsgWrite( SpecialAbirityInfoMsg[msg], ST_SP_X, ST_SP_Y+2 );

	// �g���[�i�[����
	P_StMemoPut( pp, ST_MEMO_X, ST_MEMO_Y );
}

static void P_StRight01All( PokemonParam * pp )
{
	P_StRight01Def();
	P_StRight01Param(pp);
}

static void P_StRight01Clear(void)
{
	NMenuBoxClear( ST_OYANAME_X, ST_OYANAME_Y, ST_OYANAME_X+4, ST_OYANAME_Y+1 );
	NMenuBoxClear( ST_ID_X+3, ST_ID_Y, ST_ID_X+8, ST_ID_Y+1 );
	NMenuBoxClear( ST_SP_X, ST_SP_Y, ST_SP_X+17, ST_SP_Y+3 );
	NMenuBoxClear( ST_MEMO_X, ST_MEMO_Y, ST_MEMO_X+17, ST_MEMO_Y+3 );
}


//------------------------
//		�y�[�W2�E
//------------------------
static void P_StRight02Def(void)
{
	// �o���l
	PalChangeMojiPut( str_pst_NowExp, WHITE_PAL, ST_EXP_X, ST_EXP_Y );
	PalChangeMojiPut( str_pst_LvUp, WHITE_PAL, ST_EXP_X, ST_EXP_Y+2 );
	NMenuMsgWrite( str_pst_Ato, ST_EXP_X+10, ST_EXP_Y+2 );
	// �\�͕���
	PalChangeMojiPut( str_pst_HP,       WHITE_PAL, PARAM_GX+1,  PARAM_GY );
	PalChangeMojiPut( str_pst_tokukou,  WHITE_PAL, PARAM_GX+11, PARAM_GY );
	PalChangeMojiPut( str_pst_kougeki,  WHITE_PAL, PARAM_GX,    PARAM_GY+2 );
	PalChangeMojiPut( str_pst_tokubou,  WHITE_PAL, PARAM_GX+11, PARAM_GY+2 );
	PalChangeMojiPut( str_pst_bougyo,   WHITE_PAL, PARAM_GX,    PARAM_GY+4 );
	PalChangeMojiPut( str_pst_subayasa, WHITE_PAL, PARAM_GX+11, PARAM_GY+4 );
}

static void P_StRight02Param( PokemonParam * pp )
{
	u8	i;

	for( i=0; i<POKE_CURSOR_U; i++ )
		ST_ActBanish( i, 1 );

	// ������
	P_StItemNamePut( PokeParaGet( pp, ID_item ), ST_ITEM_X, ST_ITEM_Y );
	// ���{��
	P_StRibbonPut( pp );
	// �o���l
	PalChangeNumPut( PokeParaGet( pp, ID_exp ), 0xff, 7, NUM_MODE_SPACE, ST_EXP_X+11, ST_EXP_Y );
	P_StNextExpPut( pp, ST_EXP_X+12, ST_EXP_Y+2 );

	// �\�͒l
	PalChangeNumPut( PokeParaGet( pp, ID_hp ),     0xff, 3, NUM_MODE_SPACE, PARAM_GX+4, PARAM_GY );
	PalChangeNumPut( PokeParaGet( pp, ID_hpmax ),  0xff, 3, NUM_MODE_SPACE, PARAM_GX+8, PARAM_GY );
	NMenuMsgWrite( str_surassyu, PARAM_GX+7, PARAM_GY );
	PalChangeNumPut( PokeParaGet( pp, ID_spepow ), 0xff, 3, NUM_MODE_SPACE, PARAM_GX+15, PARAM_GY );
	PalChangeNumPut( PokeParaGet( pp, ID_pow ),    0xff, 3, NUM_MODE_SPACE, PARAM_GX+4,  PARAM_GY+2 );
	PalChangeNumPut( PokeParaGet( pp, ID_spedef ), 0xff, 3, NUM_MODE_SPACE, PARAM_GX+15, PARAM_GY+2 );
	PalChangeNumPut( PokeParaGet( pp, ID_def ),    0xff, 3, NUM_MODE_SPACE, PARAM_GX+4,  PARAM_GY+4 );
	PalChangeNumPut( PokeParaGet( pp, ID_agi ),    0xff, 3, NUM_MODE_SPACE, PARAM_GX+15, PARAM_GY+4 );
}

static void P_StRight02All( PokemonParam * pp )
{
	P_StRight02Def();
	P_StRight02Param(pp);
}

static void P_StRight02Clear(void)
{
	NMenuBoxClear( ST_ITEM_X, ST_ITEM_Y, ST_ITEM_X+7, ST_ITEM_Y+1 );
	NMenuBoxClear( PARAM_GX+4, PARAM_GY, PARAM_GX+10, PARAM_GY+5 );
	NMenuBoxClear( PARAM_GX+15, PARAM_GY, PARAM_GX+17, PARAM_GY+5 );
	NMenuBoxClear( ST_EXP_X+11, ST_EXP_Y, ST_EXP_X+17, ST_EXP_Y+1 );
	NMenuBoxClear( ST_EXP_X+12, ST_EXP_Y+2, ST_EXP_X+17, ST_EXP_Y+3 );
}


//-----------------------------
//		"�g����Z"��ʍ�
//-----------------------------
static void P_WazaLeftDef( u8 waza_pos )
{
	if( P_SYSTEM->new_waza == 0 && waza_pos == 4 )	return;

	if( P_SYSTEM->page == 2 ){
		PalChangeMojiPut( str_pst_Iryoku,  WHITE_PAL, WAZA_PARAM_GX+1, WAZA_PARAM_GY );
		PalChangeMojiPut( str_pst_Meityuu, WHITE_PAL, WAZA_PARAM_GX,   WAZA_PARAM_GY+2 );
	}else{
		PalChangeMojiPut( str_pst_Appeal, WHITE_PAL, WAZA_PARAM_GX, WAZA_PARAM_GY );
		PalChangeMojiPut( str_pst_Bougai, WHITE_PAL, WAZA_PARAM_GX, WAZA_PARAM_GY+2 );
	}
}

//-----------------------------
//		"�g����Z"��ʉE
//-----------------------------
static void P_StWazaPut1( PokemonParam * pp )
{
	u16	waza_no;
	u16	waza_pp;
	u8	i;

	for( i=0; i<4; i++ ){

		waza_no = PokeWazaNoGet( pp, i );
		waza_pp = PokeWazaPPGet( pp, i );

		if( waza_no == 0 ){
			PalChangeMojiPut( str_bou1, WHITE_PAL, ST_WAZA_X , ST_WAZA_Y + ST_WAZA_INTERVAL*i );
			NMenuMsgWrite( str_bou2, ST_WAZA_X + 9, ST_WAZA_Y + ST_WAZA_INTERVAL*i );
			ST_ActBanish( ST_TYPE_1 + i, 1 );	// �\���ؑ�
		}else{
			PalChangeMojiPut( WazaNameData[ waza_no ], WHITE_PAL, ST_WAZA_X , ST_WAZA_Y + ST_WAZA_INTERVAL*i );
			P_StSpMojiPut( SP_MOJI_PP, ST_WAZA_X + 8, ST_WAZA_Y + ST_WAZA_INTERVAL*i );
			PalChangeNumPut( waza_pp, 0xff, 2, NUM_MODE_SPACE, ST_WAZA_X + 9, ST_WAZA_Y + ST_WAZA_INTERVAL*i );
			NMenuMsgWrite( str_surassyu, ST_WAZA_X + 11, ST_WAZA_Y + ST_WAZA_INTERVAL*i );
			PalChangeNumPut(
					PPMaxGet( waza_no, PokeParaGet( pp, ID_pp_count ), i ),
					0xff, 2, NUM_MODE_SPACE,
					ST_WAZA_X +12, ST_WAZA_Y + ST_WAZA_INTERVAL*i );

			if( P_SYSTEM->page == 2 )
				ST_TYPE_ActMove( WTD[ waza_no ].wazatype,
								((ST_WAZA_X-4)<<3)-1, (ST_WAZA_Y+(i<<1))<<3, i );
			else
				ST_TYPE_ActMove( ConWazaTbl[ waza_no ].con_type + 18,
								((ST_WAZA_X-4)<<3)-1, (ST_WAZA_Y+(i<<1))<<3, i );
		}
	}
}

static void P_StFifthWazaPut( PokemonParam * pp )
{
	u16	waza_no;

	if( P_SYSTEM->new_waza == 0 ){
		PalChangeMojiPut( str_modoru, WHITE_PAL, ST_WAZA_X, ST_WAZA_Y + ST_WAZA_INTERVAL * 4 );
	}else{
		waza_no = P_SYSTEM->new_waza;
		if( P_SYSTEM->page == 2 )
			PalChangeMojiPut( WazaNameData[ waza_no ], F_OYA_PAL, ST_WAZA_X, ST_WAZA_Y + ST_WAZA_INTERVAL * 4 );
		else
			PalChangeMojiPut( WazaNameData[ waza_no ], M_OYA_PAL, ST_WAZA_X, ST_WAZA_Y + ST_WAZA_INTERVAL * 4 );
		P_StSpMojiPut( SP_MOJI_PP, ST_WAZA_X + 8, ST_WAZA_Y + ST_WAZA_INTERVAL * 4 );
		PalChangeNumPut( WTD[ waza_no ].pp, 0xff, 2, NUM_MODE_SPACE, ST_WAZA_X + 9, ST_WAZA_Y + ST_WAZA_INTERVAL * 4 );
		NMenuMsgWrite( str_surassyu, ST_WAZA_X + 11, ST_WAZA_Y + ST_WAZA_INTERVAL * 4 );
		PalChangeNumPut(
				WTD[ waza_no ].pp, 0xff, 2, NUM_MODE_SPACE,
				ST_WAZA_X +12, ST_WAZA_Y + ST_WAZA_INTERVAL * 4 );

		if( P_SYSTEM->page == 2 )
			ST_TYPE_ActMove( WTD[ waza_no ].wazatype,
							((ST_WAZA_X-4)<<3)-1, (ST_WAZA_Y+(4<<1))<<3, 4 );
		else
			ST_TYPE_ActMove( ConWazaTbl[ waza_no ].con_type + 18,
							((ST_WAZA_X-4)<<3)-1, (ST_WAZA_Y+(4<<1))<<3, 4 );
	}
}

static void P_StWazaClear(void)
{
	u8	i;

	for( i=0; i<4; i++ )
		NMenuBoxClear( ST_WAZA_X, ST_WAZA_Y+(i<<1), ST_WAZA_X+13, ST_WAZA_Y+(i<<1)+1 );
}


//-------------------------
//		�Z�̌��ʂ�\��
//-------------------------
static void WazaInfoPut( PokemonParam * pp, u8 * waza_pos )
{
	u16	waza;

	if( *waza_pos != 4 )	waza = PokeWazaNoGet( pp, *waza_pos );
	else{
		if( P_SYSTEM->new_waza == 0 )	waza = 0xffff;
		else							waza = P_SYSTEM->new_waza;
	}

	NMenuBoxClear( WAZA_INFO_X, WAZA_INFO_Y, WAZA_INFO_X+17, WAZA_INFO_Y+3 );

	if( P_SYSTEM->page == 2 ){
		SetNormalWazaInfo( waza );
		SetNormalWazaPoint( waza );
		SetContestWazaPoint( 0xffff );
	}else{
		SetContestWazaInfo( waza );
		SetContestWazaPoint( waza );
	}
}

//-------------------------
//		�Z������\��
//-------------------------
// �m�[�}��
static void SetNormalWazaInfo( u16 waza )
{
	if( waza != 0xffff )
		NMenuMsgWrite( WazaInfo[waza-1], WAZA_INFO_X, WAZA_INFO_Y );
}

// �R���e�X�g
static void SetContestWazaInfo( u16 waza )
{
	if( waza != 0xffff )
		NMenuMsgWrite( WazaSetumeiMsgTbl[ConWazaTbl[waza].apno], WAZA_INFO_X, WAZA_INFO_Y );
}

//---------------------------------------
//		�퓬�̋Z�̈З́E������\��
//---------------------------------------
static void SetNormalWazaPoint( u16 waza )
{
	if( waza != 0xffff ){
		if( WTD[ waza ].damage <= 1 )
			NMenuMsgWrite( str_bou3, WAZA_PARAM_GX+5, WAZA_PARAM_GY );
		else{
			PalChangeNumPut(
				WTD[waza].damage, 0xff, 3,
				NUM_MODE_SPACE, WAZA_PARAM_GX+5, WAZA_PARAM_GY );
		}
		if( WTD[ waza ].hitprobability == 0 )
			NMenuMsgWrite( str_bou3, WAZA_PARAM_GX+5, WAZA_PARAM_GY+2 );
		else{
			PalChangeNumPut(
				WTD[ waza ].hitprobability, 0xff, 3,
				NUM_MODE_SPACE, WAZA_PARAM_GX+5, WAZA_PARAM_GY+2 );
		}
	}
}

//-----------------------------------------------
//		�R���e�X�g�̋Z�̌��ʃ|�C���g��\��
//-----------------------------------------------
static void SetContestWazaPoint( u16 waza )
{
	u8	p,i;
	u16	no;

	u32   trans_pos;
	u16 * ptrans;

	trans_pos = SCREEN_BASE13_ADDR;
	ptrans    = (void *)( trans_pos );

	// �A�s�[���|�C���g
	if( waza != 0xffff ){
		p = WazaKoukaTbl[ ConWazaTbl[ waza ].apno ].ap;
		if( p != 0xff )	p /= 10;

		for( i=0; i<8; i++ ){
			if( p != 0xff && i < p )	no = 0x103a;
			else						no = 0x1039;

			ptrans[ 486 + ( (i>>2)<<5 ) + ( i & 3 ) ] = no;
		}
	}


	// �W�Q�|�C���g
	if( waza != 0xffff ){
		p = WazaKoukaTbl[ ConWazaTbl[ waza ].apno ].bougai;
		if( p != 0xff )	p /= 10;

		for( i=0; i<8; i++ ){
			if( p != 0xff && i < p )	no = 0x103c;
			else						no = 0x103d;

			ptrans[ 550 + ( (i>>2)<<5 ) + ( i & 3 ) ] = no;
		}
	}
}


//-----------------------------------------
//		�g���[�i�[�����̓��e��\��
//-----------------------------------------
extern u8 * StatusMapNameCopy( u8 *buf, u16 wpos );

// �e�m�F
static u8 P_StMemoOyaCheck( PokemonParam * pp )
{
	u8	i, tr;
	u32	ID;

	if( P_SYSTEM->ppArray == PokeParaEnemy ){
		tr = GetSioConnectId() ^ 1;
		ID = SioTrainerInfo[tr].Id & 0xffff;
		PM_strcpy( StrTempBuffer0, SioTrainerInfo[tr].name );
	}else{
		ID = GetMyDataID() & 0xffff;
		PM_strcpy( StrTempBuffer0, MyData.my_name );
	}

	if( ID != ( PokeParaGet( pp, ID_id_no ) & 0xffff ) )
		return	FALSE;

	PokeParaGet( pp, ID_oyaname, StrTempBuffer1 );

	for( i=0; i<STR_TEMP_BUF_LEN; i++ ){
		if( StrTempBuffer0[i] == EOM_ && StrTempBuffer1[i] == EOM_ )
			return	TRUE;
		if( StrTempBuffer0[i] != StrTempBuffer1[i] )
			return	FALSE;
	}

	return	FALSE;
}

// �^�}�S�̏ꍇ
static void P_StTamagoMemoPut( PokemonParam * pp, u8 x, u8 y )
{
	u8	work;

	work = PokeParaGet( pp, ID_get_cassette );		// �ߊl�J�Z�b�g

	// ���̕s��
	if( !( work == VERSION_SAPPHIRE || work == VERSION_RUBY || work == VERSION_EMERALD ) )
	{
		NMenuMsgWrite( msg_tr_memo_egg02, x, y );
		return;
	}

	work = PokeParaGet( pp, ID_get_place );		// �ߊl�ꏊ

	// �C�x���g�œ���
	if( work == POS_EVENTHAIHU ){
		NMenuMsgWrite( msg_tr_memo_egg01, x, y );
		return;
	}

	// �����œ���
	if( P_StMemoOyaCheck(pp) == FALSE ){
		NMenuMsgWrite( msg_tr_memo_egg02, x, y );
		return;
	}

	// ����΂����񂩂����
	if( work == POS_EVENT_EGG ){
		NMenuMsgWrite( msg_tr_memo_egg03, x, y );
		return;
	}

	// ��ĉ��œ���
	NMenuMsgWrite( msg_tr_memo_egg00, x, y );
}

// �ߊl���x���\��
static void P_StMoemoGetLvPut( PokemonParam * pp, u8 eom_pos1, u8 eom_pos2, u8 x, u8 y )
{
	u8	get_lv;

	eom_pos1 = eom_pos1+eom_pos2-1;
	P_StSpMojiPut( SP_MOJI_LV_B, x + eom_pos1, y );

	get_lv = PokeParaGet( pp, ID_get_level );

	// Get Lv.0 = egg = Lv.5
	if( get_lv == 0 )	get_lv = 5;

	PalChangeNumPut( (u32)get_lv, RED_PAL, 3, NUM_MODE_LEFT, x + eom_pos1+1, y );
	if( get_lv < 10 )	NMenuMsgWrite( no_toki, x + eom_pos1 + 2, y );
	else{
		if( get_lv < 100 )	NMenuMsgWrite( no_toki, x + eom_pos1 + 3, y );
		else				NMenuMsgWrite( no_toki, x + eom_pos1 + 4, y );
	}
}

// ���C��
static void P_StMemoPut( PokemonParam * pp, u8 x, u8 y )
{
	u8	msg_no;
	u8	work;
	u8	eom_pos1 = 0;
	u8	eom_pos2 = 0;

	// ���i�\��
	msg_no = PokeSeikakuGet(pp);
	PalChangeMojiPut( SeikakuMsgTbl[msg_no], RED_PAL, x, y );

	while(1){
		if( MsgExpandBuffer[eom_pos1] == EOM_ )
			break;
		eom_pos1++;
	}
	eom_pos1 -= 6;
	if( !( msg_no == 5 || msg_no == 21 ) ){
		NMenuMsgWrite( seikaku_plus_msg00, x + eom_pos1, y );
		eom_pos1 += 1;
	}
	NMenuMsgWrite( seikaku_plus_msg01, x + eom_pos1, y );
	while(1){
		if( seikaku_plus_msg01[eom_pos2] == EOM_ )
			break;
		eom_pos2++;
	}

	// �e������
	if( P_StMemoOyaCheck(pp) == TRUE ){

		work = PokeParaGet( pp, ID_get_place );

		// �^�}�S����z����
		if( PokeParaGet( pp, ID_get_level ) == 0 ){

			eom_pos1 = eom_pos1+eom_pos2-1;
			P_StSpMojiPut( SP_MOJI_LV_B, x + eom_pos1, y );
			PalChangeNumPut( 5, RED_PAL, 3, NUM_MODE_LEFT, x + eom_pos1+1, y );
			StatusMapNameCopy( StrTempBuffer0, work );
			NMenuMsgWrite( no_toki, x + eom_pos1 + 2, y );

			NMenuMsgWrite( de_kaetta, x, y + 2 );
			return;
		}

		// �悭�킩���ꏊ
		if( work >= MAPPOS_END ){
			NMenuMsgWrite( keburukoukann, x, y + 2 );
			return;
		}

		// ���ʂ̏ꏊ
		P_StMoemoGetLvPut( pp, eom_pos1, eom_pos2, x, y );
#ifdef FESTA_VER_ROM
		PM_strcpy( StrTempBuffer0, str_festa_memo );
#else
		StatusMapNameCopy( StrTempBuffer0, work );
#endif
		NMenuMsgWrite( de_deatta, x, y + 2 );
		return;
	}

	work = PokeParaGet( pp, ID_get_cassette );		// �ߊl�J�Z�b�g

	// ���̕s��
	if( !( work == VERSION_SAPPHIRE || work == VERSION_RUBY || work == VERSION_EMERALD ) )
	{
		NMenuMsgWrite( keburukoukann, x, y + 2 );
		return;
	}

	work = PokeParaGet( pp, ID_get_place );		// �ߊl�ꏊ

	// �C�x���g�œ���
	if( work == POS_EVENTHAIHU ){
		P_StMoemoGetLvPut( pp, eom_pos1, eom_pos2, x, y );
		NMenuMsgWrite( unmeiteki, x, y + 2 );
		return;
	}

	// �Q�[���������œ���
	if( work == POS_CABLEKOUKAN ){
		NMenuMsgWrite( keburukoukann, x, y + 2 );
		return;
	}

	// �Q�[���O�����œ���
	// �悭�킩���ꏊ
	if( work >= MAPPOS_END ){
		NMenuMsgWrite( keburukoukann, x, y + 2 );
		return;
	}

	// ���ʂ̏ꏊ
	P_StMoemoGetLvPut( pp, eom_pos1, eom_pos2, x, y );
	StatusMapNameCopy( StrTempBuffer0, work );
	NMenuMsgWrite( de_deattayouda, x, y + 2 );
}

//------------------------------
//		�|�P������ / ����
//------------------------------
static void PutPokeName( PokemonParam * pp )
{
	u8	len;
	u16	monsno;

	monsno = PokeParaGet( pp, ID_monsno );

	PalChangeMojiPut( PokeNameData[monsno], WHITE_PAL, ST_NAME02_X-1, ST_NAME02_Y+2 );
	len = PM_strlen( PokeNameData[monsno] );
	P_StSexPut( pp, ST_NAME02_X-1+len, ST_NAME02_Y+2 );
}

//--------------------
//		���ʕ\��
//--------------------
static void P_StSexPut( PokemonParam * pp, u8 x, u8 y )
{
	u16	mons;

	mons = PokeParaGet( pp, ID_monsno_egg );
	if( mons == MONSNO_NIDORAN_M || mons == MONSNO_NIDORAN_F )
		return;

	switch( PokeSexGet( pp ) ){
	case MONS_MALE:
		PalChangeMojiPut( str_osu, BLUE_PAL, x, y );
		break;
	case MONS_FEMALE:
		PalChangeMojiPut( str_mesu, RED_PAL2, x, y );
		break;
	default:
		NMenuBoxClear( x, y, x, y+1 );
	}
}

//-------------------------
//		���{�����\��
//-------------------------
static u8 P_StRibbonGet( PokemonParam * pp )
{
	u8	p;

	p =  PokeParaGet( pp, ID_stylemedal );		// �������悳���_��
	p += PokeParaGet( pp, ID_beautifulmedal );	// �����������_��
	p += PokeParaGet( pp, ID_cutemedal );		// ���킢�����_��
	p += PokeParaGet( pp, ID_clevermedal );		// �����������_��
	p += PokeParaGet( pp, ID_strongmedal );		// �����܂������_��
	p += PokeParaGet( pp, ID_champ_ribbon );	// �`�����v���{���i�a������j
	p += PokeParaGet( pp, ID_winning_ribbon );	// �E�B�j���O���{���i�o�g���^���[LV50���������j
	p += PokeParaGet( pp, ID_victory_ribbon );	// �r�N�g���[���{���i�o�g���^���[LV100���������j
	p += PokeParaGet( pp, ID_bromide_ribbon );	// �u���}�C�h���{���i���p�قɎ���G������ꂽ�j
	p += PokeParaGet( pp, ID_ganba_ribbon );	// ����΃��{���i�w�͒l����ȏ�j
	p += PokeParaGet( pp, ID_marine_ribbon );	// �}�������{���i�C�x���g�œ���j
	p += PokeParaGet( pp, ID_land_ribbon );		// �����h���{���i�C�x���g�œ���j
	p += PokeParaGet( pp, ID_sky_ribbon );		// �X�J�C���{���i�C�x���g�œ���j
	p += PokeParaGet( pp, ID_country_ribbon );	// �J���g���[���{���i�C�x���g�œ���j
	p += PokeParaGet( pp, ID_national_ribbon );	// �i�V���i�����{���i�C�x���g�œ���j
	p += PokeParaGet( pp, ID_earth_ribbon );	// �A�[�X���{���i�C�x���g�œ���j
	p += PokeParaGet( pp, ID_world_ribbon );	// ���[���h���{���i�C�x���g�œ���j

	return	p;
}

static void P_StRibbonPut( PokemonParam * pp )
{
	u8	p;

	p = P_StRibbonGet( pp );

	NMenuBoxClear( ST_RIBBON_X, ST_RIBBON_Y, ST_RIBBON_X+7, ST_RIBBON_Y+2 );

	if( p == 0 ){
		NMenuMsgWrite( str_nasi, ST_RIBBON_X, ST_RIBBON_Y );
		return;
	}

	PM_strcpy( StrTempBuffer0, str_pst_NowRibon );

	if( p >= 10 )	StrTempBuffer0[5] += ( p / 10 );
	else			StrTempBuffer0[5] = spc_;
	StrTempBuffer0[6] += ( p % 10 );

	NMenuMsgWrite( StrTempBuffer0, ST_RIBBON_X, ST_RIBBON_Y );
}

//--------------------------------
//		�莝���A�C�e���\��
//--------------------------------
static void P_StItemNamePut( u16 item, u8 x, u8 y )
{
	if(item == 0)	NMenuMsgWrite( str_nasi, x, y );
	else{
		GetItemName( item, StrTempBuffer0 );
		NMenuMsgWrite( StrTempBuffer0, x, y );
	}
}

//------------------------------------------------
//		���̃��x���܂łɕK�v�Ȍo���l��\��
//------------------------------------------------
static void P_StNextExpPut( PokemonParam * pp, u8 x, u8 y )
{
	u32	nextEXP = 0;
	u32	exp;
	u8	level;
	u16	monsno;
	s64	dot = 0;
	u16	no;
	u8	i;

	u32	max_exp;
	u32	now_exp;

	u32   trans_pos;
	u16 * ptrans;
	
	exp    = PokeParaGet( pp, ID_exp );
	level  = PokeParaGet( pp, ID_level );
	monsno = PokeParaGet( pp, ID_monsno );

	if( level < 100 ){
		nextEXP = GrowTbl[ PPD[monsno].grow ][ level+1 ] - exp;
		max_exp = GrowTbl[PPD[monsno].grow][level+1]-GrowTbl[PPD[monsno].grow][level];
		now_exp = exp - GrowTbl[PPD[monsno].grow][level];

		dot = now_exp * 64 / max_exp;

		if( dot == 0 && now_exp != 0 )	dot = 1;
	}
	PalChangeNumPut( nextEXP, 0xff, 6, NUM_MODE_SPACE, x, y );

	trans_pos = SCREEN_BASE9_ADDR + ( 0x12 << 6 ) + ( 0x15 << 1 );
	ptrans    = (void *)( trans_pos );

	for( i=0; i<8; i++ ){
		if( dot >= 8 )	no = 0x206a;
		else			no = 0x2062 + ( dot % 8 );
		ptrans[i] = no;
		dot -= 8;
		if( dot < 0 )	dot = 0;
	}
}

//-------------------------------
//		�������b�Z�[�W�\��
//-------------------------------
static void PokeStFuncInfoMsgPut(void)
{

	NMenuBoxClear( 0, 0, 29, 1 );
	if( P_SYSTEM->comment1 != 0 )
		PalChangeMojiPut( st_comment[ P_SYSTEM->comment1 ], WHITE_PAL, ST_COMMENT_LX, ST_COMMENT_Y );
	if( P_SYSTEM->comment2 != 0 ){
		P_StSpMojiPut( SP_A_BUTTON_L, ST_COMMENT_RX, ST_COMMENT_Y );
		P_StSpMojiPut( SP_A_BUTTON_R, ST_COMMENT_RX+1, ST_COMMENT_Y );
		NMenuMsgWrite( st_comment[ P_SYSTEM->comment2 ], ST_COMMENT_RX+2, ST_COMMENT_Y );
	}
}


//========================================================================================
//										BG��������
//========================================================================================
//------------------------------------------
//	�|�P���X�����ς݃}�[�N
//------------------------------------------
static void PokerusMark( PokemonParam * pp )
{
	u32   trans_pos;
	u32   tr_pos_egg;
	u16 * ptrans;
	u16 * pt_egg;

	trans_pos = SCREEN_BASE28_ADDR + ( 0xf << 6 ) + ( 0x3 << 1 );
	ptrans    = (void *)( trans_pos );

	tr_pos_egg = SCREEN_BASE29_ADDR + ( 0xf << 6 ) + ( 0x3 << 1 );
	pt_egg     = (void *)( tr_pos_egg );

	if( PokerusCheck( pp, 0 ) == 0 && PokerusedCheck( pp, 0 ) != 0 ){
		ptrans[0] = 0x2c;
		pt_egg[0] = 0x2c;
	}else{
		ptrans[0] = 0x81a;
		pt_egg[0] = 0x81a;
	}
}

//-------------------------
//		���݂̃y�[�W
//-------------------------
#define	PAGE_OLD	0x4046
#define	PAGE_NOW	0x4041
#define	PAGE_NOW2	0x404b
#define	PAGE_NEXT	0x4043
#define	PAGE_NEXT2	0x4048
#define	PAGE_NO_L	0x4040
#define	PAGE_NO_R	0x404a

static void PagePosBGSet(void)
{
	u8	i;

	u16	dummy[8];

	for( i=0; i<4; i++ ){
		if( i < P_SYSTEM->page_init ){
			dummy[ i << 1 ]         = PAGE_NO_L;
			dummy[ ( i << 1 ) + 1 ] = PAGE_NO_L;
		}else{
			if( i > P_SYSTEM->page_max ){
				dummy[ i << 1 ]         = PAGE_NO_R;
				dummy[ ( i << 1 ) + 1 ] = PAGE_NO_R;
			}else{
				if( i < P_SYSTEM->page ){
					dummy[ i << 1 ]         = PAGE_OLD;
					dummy[ ( i << 1 ) + 1 ] = PAGE_OLD + 1;
				}
				if( i == P_SYSTEM->page ){
					if( i != P_SYSTEM->page_max ){
						dummy[ i << 1 ]         = PAGE_NOW;
						dummy[ ( i << 1 ) + 1 ] = PAGE_NOW + 1;
					}else{
						dummy[ i << 1 ]         = PAGE_NOW2;
						dummy[ ( i << 1 ) + 1 ] = PAGE_NOW2 + 1;
					}
				}
				if( i > P_SYSTEM->page ){
					if( i != P_SYSTEM->page_max ){
						dummy[ i << 1 ]         = PAGE_NEXT;
						dummy[ ( i << 1 ) + 1 ] = PAGE_NEXT + 1;
					}else{
						dummy[ i << 1 ]         = PAGE_NEXT2;
						dummy[ ( i << 1 ) + 1 ] = PAGE_NEXT2 + 1;
					}
				}
			}
		}
	}

	DIV_DMACOPY( 3, dummy, BG_VRAM + 0xe000 + ( 0xb << 1 ), 16, 16 );

	for( i=0; i<8; i++ )
		dummy[i] += 0x10;

	DIV_DMACOPY( 3, dummy, BG_VRAM + 0xe000 + ( 0x2b << 1 ), 16, 16 );
}

//------------------------------
//		��Ԉُ�E�B���h�E
//------------------------------
static void ConditionWinTask( u8 id )
{
	u8	i;

	s16 * wk = TaskTable[id].work;

	wk[1] += wk[0];

	if( wk[1] != 0 ){
		DIV_DMACOPY( 3, &CnoditionWinScreen[10-wk[1]], BG_VRAM + 0xe440, wk[1] << 1, 16 );
		DIV_DMACOPY( 3, &CnoditionWinScreen[20-wk[1]], BG_VRAM + 0xe480, wk[1] << 1, 16 );

		DIV_DMACOPY( 3, &CnoditionWinScreen[10-wk[1]], BG_VRAM + 0xec40, wk[1] << 1, 16 );
		DIV_DMACOPY( 3, &CnoditionWinScreen[20-wk[1]], BG_VRAM + 0xec80, wk[1] << 1, 16 );
	}
	
	for( i=(u8)wk[1]; i<10; i++ ){
		DIV_DMACOPY( 3, &wk[2], BG_VRAM + 0xe440 + ( i << 1 ), 2, 16 );
		DIV_DMACOPY( 3, &wk[2], BG_VRAM + 0xe480 + ( i << 1 ), 2, 16 );

		DIV_DMACOPY( 3, &wk[2], BG_VRAM + 0xec40 + ( i << 1 ), 2, 16 );
		DIV_DMACOPY( 3, &wk[2], BG_VRAM + 0xec80 + ( i << 1 ), 2, 16 );
	}

	if( wk[0] == 0 || wk[1] <= 0 || wk[1] >= 10 ){
		if( wk[1] >= 10 )
			PalChangeMojiPut( str_pst_Zyoutai, WHITE_PAL, ST_ZYOUTAI_X, ST_ZYOUTAI_Y );
		Add_ST_IzyoIconAct();
		DelTask(id);
	}
}

static void ConditionWin( s8 spd )
{
	u8	id;

	NMenuBoxClear( ST_ZYOUTAI_X, ST_ZYOUTAI_Y, ST_ZYOUTAI_X+4, ST_ZYOUTAI_Y+1 );
	ST_ActDel( ST_IZYOU );

	id = AddTask( ConditionWinTask, 0 );
	TaskTable[id].work[0] = (s16)spd;
	if( spd < 0 )	TaskTable[id].work[1] = 10;
	else			TaskTable[id].work[1] = 0;
	TaskTable[id].work[2] = 1;
}

//----------------------------
//		�Z���ʃE�B���h�E
//----------------------------
static void WazaParamWinTask( u8 id )
{
	u8	i,j;

	s16 * wk = TaskTable[id].work;

	wk[1] += wk[0];

	if( wk[1] != 0 ){
		for( i=0; i<7; i++ )
			DIV_DMACOPY(
				3, &WazaParamWinScreen[i*9],
				BG_VRAM+0x5b42+0x40*i+((9-wk[1]) << 1), wk[1] << 1, 16 );
	}
	
	for( i=0; i<9-wk[1]; i++ ){
		for( j=0; j<7; j++ )
			DIV_DMACOPY( 3, &wk[2], BG_VRAM+0x5b42+0x40*j+(i << 1), 2, 16 );
	}

	if( wk[0] == 0 || wk[1] <= 0 ){
		if( P_SYSTEM->page == 2 ){
			PutPokeName( &P_SYSTEM->pp );
			if( GetPokeCondition( &P_SYSTEM->pp ) != ST_NORMAL )
				PalChangeMojiPut( str_pst_Zyoutai, WHITE_PAL, ST_ZYOUTAI_X, ST_ZYOUTAI_Y );
		}
		DelTask(id);
	}

	if( wk[1] >= 9 ){
		if( P_SYSTEM->page == 2 ){
			NMenuBoxClear( 0, ST_NAME02_Y+2, ST_NAME02_X+1, ST_NAME02_Y+6 );
			P_WazaLeftDef( (u8 )wk[3] );
		}
		DelTask(id);
	}
}

static void WazaParamWin( s8 spd, u8 pos )
{
	u8	id;

	if( P_SYSTEM->page == 2 )
		NMenuBoxClear( 0, ST_NAME02_Y+2, ST_NAME02_X+4, ST_NAME02_Y+6 );

	id = CheckTaskNo( WazaParamWinTask );
	if( id == NOT_EXIST_TASK_ID )
		id = AddTask( WazaParamWinTask, 0 );
	TaskTable[id].work[0] = (s16)spd;
	if( spd < 0 )	TaskTable[id].work[1] = 9;
	else			TaskTable[id].work[1] = 0;
	TaskTable[id].work[2] = 0;
	TaskTable[id].work[3] = (s16)pos;
}


//------------------------------------
//		�R���e�X�g���ʃE�B���h�E
//------------------------------------
static void ContestParamWinTak( u8 id )
{
	u8	i,j;

	s16 * wk = TaskTable[id].work;

	wk[1] += wk[0];

	if( wk[1] != 0 ){
		for( i=0; i<7; i++ )
			DIV_DMACOPY(
				3, &ContestParamWinScreen[i*9],
				BG_VRAM+0x6b42+0x40*i+((9-wk[1]) << 1), wk[1] << 1, 16 );
	}
	
	for( i=0; i<9-wk[1]; i++ ){
		for( j=0; j<7; j++ )
			DIV_DMACOPY( 3, &wk[2], BG_VRAM+0x6b42+0x40*j+(i << 1), 2, 16 );
	}

	if( wk[0] == 0 || wk[1] <= 0 ){
		if( P_SYSTEM->page == 3 ){
			PutPokeName( &P_SYSTEM->pp );
			if( GetPokeCondition( &P_SYSTEM->pp ) != ST_NORMAL )
				PalChangeMojiPut( str_pst_Zyoutai, WHITE_PAL, ST_ZYOUTAI_X, ST_ZYOUTAI_Y );
		}
		DelTask(id);
	}

	if( wk[1] >= 9 ){
		if( P_SYSTEM->page == 3 ){
			NMenuBoxClear( 0, ST_NAME02_Y+2, ST_NAME02_X+1, ST_NAME02_Y+6 );
			P_WazaLeftDef( (u8 )wk[3] );
		}
		WazaInfoPut( &P_SYSTEM->pp, (u8 *)&wk[3] );
		DelTask(id);
	}
}

static void ContestParamWin( s8 spd, u8 pos )
{
	u8	id;

	if( P_SYSTEM->page == 3 )
		NMenuBoxClear( 0, ST_NAME02_Y+2, ST_NAME02_X+4, ST_NAME02_Y+6 );

	id = CheckTaskNo( ContestParamWinTak );
	if( id == NOT_EXIST_TASK_ID )
		id = AddTask( ContestParamWinTak, 0 );
	TaskTable[id].work[0] = (s16)spd;
	if( spd < 0 )	TaskTable[id].work[1] = 9;
	else			TaskTable[id].work[1] = 0;
	TaskTable[id].work[2] = 0;
	TaskTable[id].work[3] = (s16)pos;
}

//---------------------------------
//		�T�ڂ̋Z�E�B���h�E
//---------------------------------
static void FifthWazaWin( u8 flg )
{
	u8	i;

	u32   trans_pos;
	u16 * ptrans;

	// �퓬�Z���
	trans_pos = SCREEN_BASE13_ADDR + ( 0xb << 6 ) + ( 0xa << 1 );
	ptrans    = (void *)( trans_pos );

	if( flg == 0 ){
		for( i=0; i<20; i++ ){
			ptrans[i]    = 0x1000 + FifthWazaWinScreen[i];
			ptrans[32+i] = 0x1000 + FifthWazaWinScreen[i];
			ptrans[64+i] = 0x1000 + FifthWazaWinScreen[20+i];
		}
	}else{
		for( i=0; i<20; i++ ){
			ptrans[i]    = 0x1000 + FifthWazaWinScreen[20+i];
			ptrans[32+i] = 0x1000 + FifthWazaWinScreen[40+i];
			ptrans[64+i] = 0x1000 + FifthWazaWinScreen[40+i];
		}
	}

	// �R���e�X�g�Z���
	trans_pos = SCREEN_BASE11_ADDR + ( 0xb << 6 ) + ( 0xa << 1 );
	ptrans    = (void *)( trans_pos );

	if( flg == 0 ){
		for( i=0; i<20; i++ ){
			ptrans[i]    = 0x3000 + FifthWazaWinScreen[i];
			ptrans[32+i] = 0x3000 + FifthWazaWinScreen[i];
			ptrans[64+i] = 0x3000 + FifthWazaWinScreen[20+i];
		}
	}else{
		for( i=0; i<20; i++ ){
			ptrans[i]    = 0x3000 + FifthWazaWinScreen[20+i];
			ptrans[32+i] = 0x3000 + FifthWazaWinScreen[40+i];
			ptrans[64+i] = 0x3000 + FifthWazaWinScreen[40+i];
		}
	}
}


//========================================================================================
//									�|�P�����A�N�^�[
//========================================================================================
// �Z�b�g
static u8 St_PokeActSet( PokemonParam *pp )
{
	u8	act;
	u16	monsno;

	monsno = PokeParaGet( pp, ID_monsno_egg );

	act = AddActor( &PokeActHeader, ST_POKE_X,
					ST_POKE_Y,
					ST_POKE_PRI );

	AffineDelActor( &ActWork[ act ] );
	ActWork[ act ].work[0] = monsno;
	ActWork[ act ].move = SeqStPokeInit;

	if( PokeReverseCheck( monsno ) == 0 )	ActWork[ act ].Hflip = 1;
	else									ActWork[ act ].Hflip = 0;

	return	act;
}

// ����
static void SeqStPokeInit( actWork * Xreg )
{
	if( FadeData.fade_sw != 0 )	return;

	Xreg->move = DummyActMove;

	if( PokeParaGet( &P_SYSTEM->pp, ID_tamago_flag ) == 0 )
		VoicePlay( Xreg->work[0], 0 );	//�߹�݂̖���
}


//========================================================================================
//						�����E�J�[�\���E��Ԉُ�Ȃǂ̃A�N�^�[
//========================================================================================
// ������
static void ST_ActInit(void)
{
	u8	i;

	for( i=0; i<ST_ACT_END; i++ )
		P_ST_ACT[i] = 0xff;
}
// ����
static void ST_ActDel( u8 no )
{
	if( P_ST_ACT[no] != 0xff ){
		DelActor( &ActWork[ P_ST_ACT[no] ] );
		P_ST_ACT[no] = 0xff;
	}
}
// �\���ؑ�
static void ST_ActBanish( u8 no, u8 flg )
{
	ActWork[ P_ST_ACT[no] ].banish = flg;
}


//------------------------
//		�����A�N�^�[
//------------------------
static void ST_TYPE_ActSet2(void)
{
	u8	i;

	for( i=0; i<POKE_CURSOR_U; i++ ){
		if( P_ST_ACT[ ST_TYPE_1+i ] == 0xff )
			P_ST_ACT[ ST_TYPE_1+i ] = AddActor( &ST_TYPE_32x16_Header, 0, 0, 2 );
		ST_ActBanish( ST_TYPE_1+i, 1 );
	}
}

static void ST_TYPE_ActMove( u8 id, u8 x, u8 y, u8 no )
{
	ActAnmChg( &ActWork[ P_ST_ACT[no] ], id );
	ActWork[ P_ST_ACT[no] ].oamData.Pltt = type_pal[id];
	ActWork[ P_ST_ACT[no] ].x = x+16;
	ActWork[ P_ST_ACT[no] ].y = y+8;
	ST_ActBanish( no, 0 );
}

//--------------------------
//		�Z�I���J�[�\��
//--------------------------
static void Add_WAZA_Cursor( u8 no )
{
	u8	i;
	u8	pri = 0;

	if( P_SYSTEM->page < 2 )	return;

	if( no == WAZA_CURSOR_L )	pri = 1;

	for( i=0; i<10; i++ ){
		P_ST_ACT[ no+i ] = AddActor( &ST_CURSOR_Header,
									 ( ( ST_WAZA_CURSOR_X+( i << 1 ) ) << 3 ) + 8,
									 ( ST_WAZA_CURSOR_Y << 3 ) + 8, pri );
		if( i == 0 )	ActAnmChg( &ActWork[ P_ST_ACT[ no+i ] ], 4 );
		else	if( i == 9 )	ActAnmChg( &ActWork[ P_ST_ACT[ no+i ] ], 5 );
		else	ActAnmChg( &ActWork[ P_ST_ACT[ no+i ] ], 6 );

		ActWork[ P_ST_ACT[ no+i ] ].move = Waza_CursorActMove;
		ActWork[ P_ST_ACT[ no+i ] ].work[0] = no;
		ActWork[ P_ST_ACT[ no+i ] ].work[1] = 0;
	}
}
static void Del_WAZA_Cursor( u8 no )
{
	u8	i;

	for( i=0; i<10; i++ ){
		ST_ActDel( no + i );
	}
}

static void WAZA_CursorAnmChange( u8 flg )
{
	u8	i;

	flg = flg * 3;

	ActAnmChg( &ActWork[ P_ST_ACT[ WAZA_CURSOR_L ] ], 4 + flg );
	for( i=0; i<8; i++ )
		ActAnmChg( &ActWork[ P_ST_ACT[ WAZA_CURSOR_N1 + i ] ], 6 + flg );
	ActAnmChg( &ActWork[ P_ST_ACT[ WAZA_CURSOR_R ] ], 5 + flg );
}

static void Waza_CursorActMove( actWork * act )
{
	if( act->anm_no >= 4 && act->anm_no <= 6 ){
		act->work[1] = ( ( act->work[1] + 1 ) & 0x1f );
		if( act->work[1] > 24 ){
			act->banish = 1;
		}else{
			act->banish = 0;
		}
	}else{
		act->work[1] = 0;
		act->banish = 0;
	}

	if( act->work[0] == WAZA_CURSOR_L )	act->dy = P_SYSTEM->waza_pos << 4;
	else								act->dy = P_SYSTEM->waza_pos2 << 4;
}

// �J�[�\���_�ŃN���A
static void Waza_CursorbanishClear( u8 flg )
{
	u8	i;

	flg *= 10;
	
	for( i=flg; i<flg+10; i++ ){
		ActWork[ P_ST_ACT[ WAZA_CURSOR_L + i ] ].work[1] = 0;
		ActWork[ P_ST_ACT[ WAZA_CURSOR_L + i ] ].banish  = 0;
	}
}


//----------------------------
//		��Ԉُ�A�C�R��
//----------------------------
u8	GetPokeConditionParam( u32 con )
{
	// �ǂ�
	if( ( con & CONDITION_DOKU ) != 0 || ( con & CONDITION_DOKUDOKU ) != 0 )
		return	ST_POISON;
	// �܂�
	if( ( con & CONDITION_MAHI ) != 0 )
		return	ST_PARALYSIS;
	// �˂ނ�
	if( ( con & CONDITION_NEMURI ) != 0 )
		return	ST_SLEEP;
	// ������
	if( ( con & CONDITION_KOORI ) != 0 )
		return	ST_ICE;
	// �₯��
	if( ( con & CONDITION_YAKEDO ) != 0 )
		return	ST_BURN;

	return	ST_NORMAL;
}

u8	GetPokeCondition( PokemonParam * pp )
{
	u8	param;

	// �m��
	if( PokeParaGet( pp, ID_hp ) == 0 )	return	ST_HINSI;

	param = GetPokeConditionParam( PokeParaGet( pp, ID_condition ) );
	if( param != ST_NORMAL )	return	param;
	
	// �|�P���X
	if( PokerusCheck( pp, 0 ) != 0 )
		return	ST_POKERUS;

	return	ST_NORMAL;
}

static void Add_ST_IzyoIconAct(void)
{
	PokemonParam	pp;
	u8	anm;
	u32	cond;

	NowPokeParamDataSet( &pp );

	cond = GetPokeCondition( &pp );

	if( cond != ST_NORMAL ){
		anm = cond - 1;
	}else{
		ST_ActDel( ST_IZYOU );
		return;
	}

	if( P_ST_ACT[ ST_IZYOU ] == 0xff )
		P_ST_ACT[ ST_IZYOU ] = AddActor( &ST_IZYO_Header, ST_CONDITION_X, ST_CONDITION_Y, 0 );
	ActAnmChg( &ActWork[ P_ST_ACT[ ST_IZYOU ] ], anm );
}

//-------------------------------------------
//	�{�b�N�X�}�[�N�A�N�^�[
//-------------------------------------------
static actWork * BoxMarkAct = NULL;

static void AddBoxMarkAct( PokemonParam * pp )
{
#ifdef FESTA_VER_ROM
#else
	BoxMarkAct = MarkingStateAnmObjAdd( ST_BOXMARK_ID, ST_BOXMARK_ID, box_mark_Palette );

	if( BoxMarkAct != NULL ){
		ActAnmChg( BoxMarkAct, PokeParaGet( pp, ID_mark ) );
		BoxMarkAct->x = ST_MARK_X;
		BoxMarkAct->y = ST_MARK_Y;
	}
#endif
}

static void AddBoxMarkActInit( PokemonParam * pp )
{
#ifdef FESTA_VER_ROM
#else
	DelActor( BoxMarkAct );
	AddBoxMarkAct( pp );
#endif
}

//-------------------------------------------
//	�߂܂����{�[��OBJ
//-------------------------------------------
static void GetMonsBallActSet( PokemonParam * pp )
{
	u16	ball;
	u8	tbl;

	ball = PokeParaGet( pp, ID_get_ball );
	tbl  = BallItemNoTblNoConv( ball );
	MonsBallCellPalSet( tbl );

	P_SYSTEM->ballact = AddActor(
		&MonsterBallHeader[tbl], P_ST_MONS_BALL_X, P_ST_MONS_BALL_Y,  0 );
	ActWork[ P_SYSTEM->ballact ].move = DummyActMove;
	ActWork[ P_SYSTEM->ballact ].oamData.Priority = 3;
}
