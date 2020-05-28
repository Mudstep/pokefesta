//========================================================================================
//								�~�j�Q�[���E���[���b�g
//								2002/03/28 by nakahiro
//								2002/05/30 iwasawa's job,a job from nakahiro
//								2002/06/28 renewal
//========================================================================================
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "script.h"
#include "fld_main.h"
#include "menu.h"
#include "task.h"
#include "bag.h"
#include "calctool.h"
#include "fld_task.h"
#include "itemtool.h"
#include "roulette.h"
#include "rlt_obj.h"
#include "coin.h"
#include "calctool.h"
#include "mitool.h"
#include "mus_tool.h"
#include "poketool.h"
#include "record.h"
#include "item.h"
#include "laster.h"
#include "rtctool.h"

#include "../scaddata/minig_r.acl"		//bg�p�p���b�g
#include "../scaddata/rlt_bg01.asc"		//bg�p���k�X�N���[���f�[�^

#include "rlt_com.dat"
#include "roulette.dat"
#include "monsno.def"
//========================================================================================
//										�V���{��
//========================================================================================
// �^�X�N���[�N
#define BET_WORK		0	// �q��������
#define CNT_WORK		1	// �J�E���g
#define ODDS_WORK		2	// �I�b�Y
#define BONUS_WORK		3	// �{�[�i�X�I�b�Y
#define BET_POS_WORK	4	// �q�����ꏊ
#define WIN_WORK		5	// ���s
#define	LOOP_WORK		6	// �Q�[����
#define WAIT_WORK		7	// �E�F�C�g�Ǘ�
#define NPLAY_WORK		8	// �A���v���C��

#define BET_ALL_WORK	9	// ���ݓq���Ă��鑍������
#define BETPOS_OLD_WORK	10	// �ߋ��̃J�[�\���ʒu
#define NWIN_WORK		11	// �A���������̕ۑ�
#define LASTHIT_WORK	12	// �Ō��HIT����Position
#define COIN_WORK		13	// �R�C���̐�
#define	DEBUG_WORK		15	// �f�o�b�O�p

#ifdef PM_DEBUG
	ROULETTE*	gp_rlt;		//�f�o�b�O�p�̕ϐ�
	u8			g_rlt_debug_sw = 0;
#endif //ifdef PM_DEBUG
//========================================================================================
//									�v���g�^�C�v�錾
//========================================================================================

//�Q�[���S�̂�ʂ��ẮA�L�[�擾
static void TS_RouletteKeyIn(u8 id);

//��ԏ��߂ɑ�ɘb���|���Ă���A�Q�[�������ۂɂ͂��߂�܂ł̏���
static void TS_FirstPlayFadeWait(u8 id);	//�t�F�[�h�C����҂�
static void TS_FirstPlaySelectInit(u8);		// �v���C����H
static void TS_FirstPlaySelectMain(u8);		// �͂�/�������Z�b�g
static void TS_NextPlaySelectInit(u8 id);
static void TS_NextPlaySelectMain(u8);

static void TS_PlayYes_1st( u8 id );	// Yes
static void TS_PlayNo_1st( u8 id );		// No

static void TS_PlayYes(u8);				// YES
static void TS_PlayNo(u8);				// NO

//�x�b�g�e�[�u���ɂ����鏈��
static void TS_BetInit(u8);				// �q����
static void TS_BetMain_1st(u8);			// �ǂ��ɓq���邩���߂�(�{�[��������O)
static void TS_BetMain(u8);				// �ǂ��ɓq���邩���߂�(�{�[��������O)
static void TS_BallParamInit(u8);		// �J�����g�{�[���̃p�����[�^��������
static void TS_BallStart(u8);			// �X�^�[�g�A�{�[�������鏈��
static void TS_ResultWait(u8);			// ���ʑ҂��̃E�F�C�g�̏���
static void TS_ResultDemo(u8);			// ���ʂ�\������f�� 
static void TS_ResultSet(u8);			// ���ʂ���ʂɃZ�b�g
static void TS_ReturnBetFunc(u8);		// ���ʂ��J��
static void TS_ReturnBetFunc2(u8);		// ���̃Q�[���ւ̏���
static void TS_GameStateReset(u8);		// �Q�[���̏�Ԃ����Z�b�g
static void RouletteEndInit(u8);		// �I�����ăt�B�[���h�֕��A
static void TS_RouletteEnd(u8);

static void TS_WinSlide_1st(u8 id);		// ��ʂ̃X���C�h����
static void TS_WinSlide_2nd(u8 id);
////////////////////////////////////////////
//�⏕�n

//�^�X�N����֘A
static void TS_WaitNextTask(u8 id);
static void SetWaitNextTask(u8 id,pTaskFunc pAdrs,u16 wait,u16 key);

//�Q�[���֘A
static void OneGameStateClear(u8 id);		//1�Q�[���̏�Ԃ��N���A
static u8 GetOddsPreCell(u8 no);			//�w��Z���̃I�b�Y�����߂�

// �q�b�g�����Z���̃`�F�b�N
static void HitCellClear(void);				// �Z���̃q�b�g��Ԃ��N���A����
static u8 ResultFlgSet(u8 id,u8 hit_cell );	// �q�b�g�����Z���̏�Ԃ�ۑ�
static u8 WinCheck( u8 hit_bitNo, u8 bet );	// �\�z�������������ǂ����`�F�b�N

//���̑�
static void RouletteBGRotate(void);		// ���[���b�g�Չ�]����

//�`��⏕�n
static void RltColorChange(u8 pos);		//�J���[�A�j������
static void DrawWakuWindow(u8);
static void DrawBetWindow(u8);			// �q����ꏊ�E�B���h�E�\��

//���b�Z�[�W�\���⏕
#define MsgWrite(msg){									\
	NMenuWinBoxWrite( 0, 14, 29, 19 );					\
	NMenuMsgWrite((msg), 2, 15);						\
}
#define MsgMiniWrite(msg){								\
	NMenuWinBoxWrite( 0, 14, 18, 19 );					\
	NMenuMsgWrite((msg), 2, 15);						\
}
#define MsgReWrite(msg){								\
	NMenuTalkMsgClr();									\
	NMenuMsgWrite((msg), MWIN_MSGXPOS, MWIN_MSGYPOS);	\
}
#define SetMsgWrite(id,msg,nextadrs){					\
	NMenuWinBoxWrite( 0, 14, 29, 19 );					\
	NMenuMsgWrite((msg), 2, 15);						\
	TaskTable[(id)].TaskAdrs = (nextadrs);				\
}
#define MsgBoxWrite(){									\
	NMenuTalkWinWrite();								\
}

/*--------------------------------------------
	1�Z�b�g�I����̃Q�[���̃X�e�[�g���N���A����
------------------------------------------------*/
#define GameSetClear(id)	{					\
	TaskTable[(id)].work[ LOOP_WORK ] = 0;		\
												\
	OneGameStateClear((id));					\
	HitCellClear();								\
	G_BallParamClear();							\
												\
	DrawBetWindow(0);							\
	DrawRestballControl(6);						\
}

//========================================================================================
//									�O���[�o���ϐ�
//========================================================================================
static const u8	msg_str_bet[] = { I_MSG_,STR_TEMP_BUF0,ma_,i_,EOM_ };

static const SELECT_ITEM sg_yes_no_1st = {TS_PlayYes_1st, TS_PlayNo_1st};
static const SELECT_ITEM sg_yes_no = { TS_PlayYes, TS_PlayNo };	// �͂�/����������

//=========================================================================================
//										���C������
//=========================================================================================
/*--------------------------------------------
	���[���b�g���C���^�X�N

	MainTask
	staitc void TM_RouletteMain(void);
----------------------------------------------*/
static void TM_RouletteMain(void)
{
	MainTask();
	JumpActor();
	SetActor();

	//�J���[�A�j��
	if(MG_RLT->colanm.uc_flg){
		ColorAnimeLum(&MG_RLT->colanm);
	}
}

//=========================================================================================
//										���荞��
//=========================================================================================
static void VInterFunc( void )
{
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();

	RouletteBGRotate();

	//BG1�X�N���[���X�N���[������
	*(vu16 *)REG_BG1HOFS = 512 - MG_RLT->bg1_scrX;

	//�A���t�@�u�����f�B���O�W���ݒ�
	if(MG_RLT->state_flg){
		*(vu16 *)REG_BLDALPHA = MG_RLT->w_alpha;
	}

	if(MG_RLT->bg1_cnt){
		DmaCopy(3,&(BG1_data[0x00E0]),BG_VRAM+0x21C0,0x340,16);	//BG�}�b�v�]��
		MG_RLT->bg1_cnt = 0;
	}
	
	switch(MG_RLT->bg0_cnt){
	case 0:
		return;
	case 1:
		*(vu16 *)REG_BG0CNT = BG_COLOR_16					// 16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_0					// BG�D�揇��
							| 31 << BG_SCREEN_BASE_SHIFT	// �X�N���[���x�[�X�u���b�N
							| 0 << BG_CHAR_BASE_SHIFT ;		// �L�����N�^�x�[�X�u���b�N
		DmaCopy(3,&(BG2_data[0x00E0]),BG_VRAM+0xf9c0,0x340,16);
		MG_RLT->bg0_cnt = 2;
		return;
	case 2:
		DmaCopy(3,&(BG2_data[0x00E0]),BG_VRAM+0xf9c0,0x340,16);
		return;
	case 0xFF:
		*(vu16 *)REG_BG0CNT = BG_COLOR_16					// 16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_0					// BG�D�揇��
							| 31 << BG_SCREEN_BASE_SHIFT	// �X�N���[���x�[�X�u���b�N
							| 2 << BG_CHAR_BASE_SHIFT ;		// �L�����N�^�x�[�X�u���b�N
		DmaClear(3,0x0000,BG_VRAM+0xf9c0,0x340,16);
		MG_RLT->bg0_cnt = 0;
		return;
	}

}

//=========================================================================================
//										������
//=========================================================================================
/*--------------------------------------------------
	���[���b�g�Ŏg�p���鑍���p�����[�^�̏�����

	static void InitRouletteParam(void);
----------------------------------------------------*/
static void InitRouletteParam(void)
{
	u8	ct;
	u16	betPal[3] = {0x2898,0x1A6A,0x2898};
	
	memset(MG_RLT,0,sizeof(ROULETTE));

	//�X�N���v�g�p�����[�^�̈��p��
	MG_RLT->rlt_ver	  =	(ScriptParameter0 & RLT_CHECK_RATE);	//���[�g
	if(ScriptParameter0 & RLT_IF_BONUS)	MG_RLT->rlt_bonus =	1;							//�{�[�i�X�t���O

	//��̃p�����[�^
	MG_RLT->bg_spd	= g_table[MG_RLT->rlt_ver].uc_rSpd;
	MG_RLT->bg_wait	= g_table[MG_RLT->rlt_ver].uc_rWait;
	MG_RLT->uc_rate = g_rltRate[MG_RLT->rlt_bonus][MG_RLT->rlt_ver];
	
	MG_RLT->betAll = 1;									//���ӏ��ɓq�����邩

	//�w�i�F��ύX�i���j
	if(MG_RLT->uc_rate == 1){
		PaletteWork[PA_BG0] = 
		PaletteWork[PA_BG5+PA_COL1] = 
		PaletteWorkTrans[PA_BG0] = 
		PaletteWorkTrans[PA_BG5+PA_COL1] = betPal[0];
	}
	else{
		PaletteWork[PA_BG0] = 
		PaletteWork[PA_BG5+PA_COL1] = 
		PaletteWorkTrans[PA_BG0] = 
		PaletteWorkTrans[PA_BG5+PA_COL1] = betPal[1];
	}

	//�J���[�A�j���[�V�����p�̃p�����[�^��o�^
	ColorAnimeInit(&MG_RLT->colanm);
	for(ct = 0;ct < 13;ct++)
	{
		AddColorAnime(&MG_RLT->colanm,ct,(ColAnmData *)&(g_colanm[ct]));
	}

	//����̃����X�^�[��A��Ă��邩�ǂ�������
	for(ct = 0;ct < 6;ct++)
	{
		//ID_monsno_egg�̓^�}�S�̂Ƃ��̓L�m�R�R�Ƃ��X�o���͕Ԃ��Ȃ��ݒ�
		switch(PokeParaGet(&PokeParaMine[ct],ID_monsno_egg)){
		case MONSNO_KINOKOKO:	//�L�m�R�R
			MG_RLT->mons_flg |= 1;
			break;
		case MONSNO_SUBAME:	//�X�o��
			MG_RLT->mons_flg |= 2;
			break;
		default:
			break;
		}
	}
	GetGameTime();	//����������GET
}

static void TM_RouletteInit(void)
{
	u8	id;

#ifdef PM_DEBUG	
	gp_rlt = (ROULETTE *)&UserWork[RLT_UWORK_PROG];
#endif	//ifdef PM_DEBUG

	switch( sys.InitSeqWork ){
	case 0:
		SetVBlankFunc(NULL);	// �����݊֐��ݒ�(VBLANK)
		LasterInit();			//HDMA������
		InitHVBlankFunc();		// HVBlank������
		ItemToolAGBRegInit();	// ���W�X�^�EVRAM�EOAM�EPALRAM������

		*(vu16 *)REG_BG2CNT = BG_COLOR_256				// 256�F�I��
							| BG_SCREEN_SIZE_1			// �X�N���[���T�C�Y
							| BG_PRIORITY_2				// BG�D�揇��
							| 6 << BG_SCREEN_BASE_SHIFT	// �X�N���[���x�[�X�u���b�N
							| 1 << BG_CHAR_BASE_SHIFT	// �L�����N�^�x�[�X�u���b�N
							| BG_LOOP_OFF;		
		*(vu16 *)REG_BG1CNT = BG_COLOR_16					// 16�F�I��
							| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
							| BG_PRIORITY_1					// BG�D�揇��
							| 4 << BG_SCREEN_BASE_SHIFT		// �X�N���[���x�[�X�u���b�N
							| 0 << BG_CHAR_BASE_SHIFT ;		// �L�����N�^�x�[�X�u���b�N

		//�u�����f�B���O�Ώۂ̐ݒ�
	    *(vu16 *)REG_BLDCNT =  BLD_BG2_2ND | BLD_BD_2ND;
	    *(vu16 *)REG_BLDALPHA = 0x060A;		//�u�����f�B���O�p�����[�^
	
		//�Ώ�BG�Ƀf�[�^���Z�b�g
		LZ77UnCompVram((void *)rlt_bg01_pch_ADRS,(void *)BG_VRAM);
		LZ77UnCompVram((void *)rlt_bg02_pch_ADRS,(void *)(BG_VRAM+0x4000));

		sys.InitSeqWork++;
		break;

	case 1:
		PaletteAnimeInit();			// ��گı�� ������
		InitActor();				// ����������
		InitTask();					// �^�X�N������

		sys.InitSeqWork++;
		break;

	case 2:
		PrintScreenInit(DEFAULT_TYPE);		// ү���ޕ\���̈揉����
		NMenuScreenInit(FIELD_MENU_TYPE);	// �����f�[�^������
		PaletteWorkSet( minig_r_Palette, PA_BG0, 0x1C0);			// 256_1�p���b�g
		
		sys.InitSeqWork++;
		break;
	case 3:
		//�����p�����[�^�[�Z�b�g
		InitRouletteParam();
		
		//������ʂ����
		BGDataInit();						// BG�W�J�p�o�b�t�@������

		//���k�f�[�^��W�J
		LZ77UnCompWram((void *)rlt_bg01_LZ,(void *)BG01_MAP);
		LZ77UnCompVram((void *)rlt_bg02_LZ,(void *)(BG_VRAM+0x3000));
		
		sys.InitSeqWork++;
		break;
	case 4:
		RouletteObjPal(0);			// ���[���b�gOBJ�p���b�g�ݒ�
		AddBallObj();				// �{�[���̃I�u�W�F�N�g�ǉ�
		AddBallHoleObj();			// �z�[��(�Z���^�[�e�[�u���I�u�W�F�N�g)��ǉ�

		AddBg1OBJ();				// Bg1�p�I�u�W�F�N�g�ǉ�
		AddRoulettePokeWinOBJ();	// �|�P�����}�[�NOBJ�ǉ�
		AddSTCellObj();				// �x�b�g�e�[�u���p�{�[���I�u�W�F�N�g��ǉ�
		
		AddRotatePokeOBJ();			// ���[���b�g�Ղ̃|�P����OBJ�ǉ�
		
		sys.InitSeqWork++;
		break;
	case 5:
		JumpActor();
		SetActor();
		DrawCoinControl(Fld.my_coin);	//���ݎ����Ă���R�C���̖���
		DrawRestballControl(6);			//�c��̋ʂ̐�
		DrawOddsControl(0);				//�I�b�Y
		DrawBetWindow(0);				//�x�b�g�e�[�u����\��
		MsgWrite(MSG_PANEL);
		
		ActOX = -60;
		ActOY = 0;
	
		sys.InitSeqWork++;
		break;
	case 6:
		*(vu16 *)REG_DISPCNT = DISP_MODE_1			// BG���[�h
							 | DISP_OBJ_CHAR_1D_MAP	// OBJ1����ϯ��ݸ�
							 | DISP_BG2_ON			// BG2�L��
							 | DISP_BG1_ON			// BG1�L��
							 | DISP_BG0_ON			// BG0�L��
							 | DISP_OBJ_ON
							 ;

		sys.InitSeqWork++;
		break;
	case 7:
		REG_IE_SET( V_BLANK_INTR_FLAG );
		*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;
		SetVBlankFunc(VInterFunc);			// �����݊֐��ݒ�(VBLANK)
		
		//�t�F�[�h�C�����N�G�X�g
		HardFadeReq(0xFF,0,16,0,1);
		
		//�T�u�^�X�N�Z�b�g
		id = MG_RLT->sTask_id[RLT_STASK_MAIN] = AddTask( TS_FirstPlayFadeWait, 0 );
		TaskTable[id].work[ LOOP_WORK ] = RLT_GAME_MAX;	//�Q�[�������Z�b�g
		TaskTable[id].work[ COIN_WORK ] = Fld.my_coin;	//�R�C�����Z�b�g
		
		//���ʃL�[�擾�p�A�T�u�^�X�N�Z�b�g
		MG_RLT->sTask_id[RLT_STASK_KEY] = AddTask(TS_RouletteKeyIn,1);

		//���C���^�X�N�`�F���W
		MainProcChange( TM_RouletteMain );
	}
}

//===========================================
//	�Q�[�����A���ׂĂ�ʂ��ẮA���ʃL�[�擾
//===========================================
static void TS_RouletteKeyIn(u8 id)
{
	s16	sin,cos;

	//��]����
	if(MG_RLT->bg_ct++ == MG_RLT->bg_wait){
		MG_RLT->bg_ct = 0;
		MG_RLT->bg_rotate -= (u16)MG_RLT->bg_spd;
		if( MG_RLT->bg_rotate < 0 )	MG_RLT->bg_rotate = THETA_360_DEF - MG_RLT->bg_spd;
	}

	sin = Sin_360_Get( MG_RLT->bg_rotate );
	cos = Cos_360_Get( MG_RLT->bg_rotate );
	
	//�g�嗦�i�P/�P= 0x1000 �j�ŉ�]
	sin = (s16)(((s32)sin * FF_MAG1000)/0x100);
	MG_RLT->bg_sPa =  
	MG_RLT->bg_sPd = (s16)(((s32)cos * FF_MAG1000)/0x100);
	MG_RLT->bg_sPb =  sin;
	MG_RLT->bg_sPc = -sin;

#ifdef PM_DEBUG	
	if(!g_rlt_debug_sw) return;
	if(!(sys.Trg & START_BUTTON)) return;

	//�X�^�[�g�{�^���������ꂽ��A�Q�[���������I��
	TaskTable[MG_RLT->sTask_id[RLT_STASK_MAIN]].TaskAdrs = TS_PlayNo;
	m4aMPlayStop(&m4a_mplay001);
	m4aMPlayStop(&m4a_mplay002);
	m4aMPlayStop(&m4a_mplay003);
#endif	//PM_DEBUG	
}

//=====================================
//	���[���b�g��ʂɓ����Ă����Ƃ��A
//=====================================
/*---------------------------------------------------
	���[���b�g��ʂɓ����Ă����Ƃ��A�t�F�[�h�C����҂�

	SubTask	
		staitc void TS_FirstPlayFadeWait(u8 id);

	Depend
		itemtool.c
------------------------------------------------------*/
static void TS_FirstPlayFadeWait(u8 id)
{
	if(PaletteAnime()) return;
	
	//�u�����f�B���O�Ώۂ̐ݒ�
	*(vu16 *)REG_BLDCNT |=  BLD_BG2_2ND | BLD_BD_2ND;
	*(vu16 *)REG_BLDALPHA = 0x0808;		//�u�����f�B���O�p�����[�^

	//�t�F�[�h�C�����I�����Ă���
	//����������b�Z�[�W
	GameSetClear(id);
	SetWaitNextTask(id,TS_BetInit,0xFFFF,A_BUTTON|B_BUTTON);
	return;
}

/*-----------------------------------------------------------
	�A���v���C������A���Ȃ���I������^�X�N�Ƃ��ċ@�\����
	�P�v���C���I�������A���̃^�X�N�ɖ߂��Ă���

	SubTask
	static void TS_NextPlaySelectMain(u8 id);

	depend	
		menutool.c
		itemtool.c
---------------------------------------------------------------*/
static void TS_NextPlaySelectMain( u8 id )
{
	//�\���ʒu���w�肵�āAYesOrNo�̃��j���[box��\������
	YesNoSelectInit( YESNO_WINX, YESNO_WINY );	//menutool.c
	MsgWrite(MSG_REPLAY);

	//TS_PlayYes �܂��� TS_PlayNo �ɃT�u�^�X�N�`�F���W
	SelectToolInit( id, &sg_yes_no );
}

// YES
/*----------------------------------------------
	�v���C�����I�񂾂Ƃ��̏����i��ԏ��߁j
	SelectToolInit(id,&sg_yes_no); ���

	SubTask
	static void TS_PlayYes(u8 id)

	Depend
		menutool.c
		itemtool.c
------------------------------------------------*/
static void TS_PlayYes( u8 id )
{
	//YesNo���b�Z�[�WBox������
	NMenuBoxClear( YESNO_WINX, YESNO_WINY, YESNO_WINX+6, YESNO_WINY+5 );

	TaskTable[id].TaskAdrs = TS_BetInit;
	return;
}

// NO
/*--------------------------------------------
	�v���C���I�������N�G�X�g����

	SubTask
	static void TS_PlayNo(u8 id);
----------------------------------------------*/
static void TS_PlayNo( u8 id )
{
	//�L�[�擾�p�̃^�X�N���폜
	DelTask(MG_RLT->sTask_id[RLT_STASK_KEY]);
	RouletteEndInit(id);
}

////////////////////////////////////////////////////////////
//	�x�b�h�G���g���[
//////////////////////////////////////////////////////////////

//------------------
//		�q����
//------------------
/*---------------------------------------
	�q�����ꏊ�ɉ������x�b�h�e�[�u���̕`��
	�x�b�hOBJ�E�I��g�̑I��g�\���E��\���R���g���[��

	FileRocal
	static void DrawBetTable(u8 id,u8 flg)

	u8	id	�Ăяo�����T�u�^�X�N��ID
	u8	flg	0 :CURSOL_POS : ���ݓq���Ă���ꏊ�ƁA�J�[�\���ʒu�̔{����\��
			1 : BET_POS   : �q�����ꏊ�Ƃ��̔{����\��
			
	Depend
		task.h
--------------------------------------------*/
#define CURSOL_POS	0
#define BET_POS		1
/*-------------------------------------------
	�I��g�̕\��
----------------------------------------------*/
static void DrawWakuWindow(u8 pos)
{
	u8	dx = 0,dy = 0;

	switch(pos)
	{
	case 0:
		FillChar16(BG2_data,0x0000,14,7,16,13);
		return;
	case 1:
	case 2:
	case 3:
	case 4:
		dx = 14+pos*3;
		dy = 7;
		FillChar16(BG2_data,0x0000,14,7,16,13);
		SetChar16(BG2_data,WAKU01_MAP,dx,7,3,13);
		return;
	case 5:
	case 10:
	case 15:
		dx = 14;
		dy = 10+((pos-1)/5)*3;
		FillChar16(BG2_data,0x0000,14,7,16,13);
		SetChar16(BG2_data,WAKU02_MAP,dx,dy,16,3);
		return;
	default:
		dx = 14+(pos%5)*3;
		dy = 7+((pos-1)/5)*3;
		FillChar16(BG2_data,0x0000,14,7,16,13);
		SetChar16(BG2_data,WAKU00_MAP,dx,dy,3,3);
		return;
	}
}
static void DrawBetTable( u8 id ,u8 flg)
{
	if(flg == CURSOL_POS){
		//���݂̃J�[�\���ʒu�̏ꏊ�Ɣ{����\��
		DrawOddsControl((u8)TaskTable[id].work[BET_POS_WORK]);
	}
	else{
		//�q�����ꏊ�ƁA���̔{����\��
		DrawOddsControl(MG_RLT->uc_aBet[MG_RLT->nBet]);
	}
	DrawWakuWindow((u8)TaskTable[id].work[BET_POS_WORK]);
}

/*-------------------------------------------
	�x�b�h�E�B���h�E�̕`��A����������

	SubTask
	static void TS_BetInit(u8 id);
----------------------------------------------*/
static void TS_BetMain_1st(u8 id)
{
	//�g�E�B���h�E�̕\���J�n
	MG_RLT->bg0_cnt = 1;
	DrawWakuWindow(TaskTable[id].work[BET_POS_WORK]);

	MG_RLT->bg_wait = 2;
	MG_RLT->bg_ct = 0;
	TaskTable[id].TaskAdrs = TS_BetMain;		//�^�X�N�`�F���W
}
static void TS_BetInit( u8 id )
{
	short i;
	
	//�I���J�[�\�����ŏ��ɂǂ��Ɏ����Ă������H
	if((MG_RLT->rlt_hp & BET_FLG05)){	//1��ڂ��S�����܂��Ă���Ƃ�
		for(i = 11;i < 14;i++){
			if((MG_RLT->rlt_hp & g_rltBet[i].flg) == 0) break;
		}
	}
	else{	//1��ڂɋ󂫂�����Ƃ�
		for(i = 6;i < 10;i++){
			if((MG_RLT->rlt_hp & g_rltBet[i].flg) == 0) break;
		}
	}
	TaskTable[id].work[BET_POS_WORK] = i;

	//1�Q�[���̏�Ԃ��N���A���܂�
	OneGameStateClear(id);

	DrawBetWindow(TaskTable[id].work[BET_POS_WORK]);	//�x�b�h�R�C���E�B���h�E��`�悵�܂�
	DrawOddsControl(TaskTable[id].work[BET_POS_WORK]);	//�I�b�Y�̕\��
	RltColorChange(TaskTable[id].work[BET_POS_WORK]);
	TaskTable[id].work[CNT_WORK] = 0;
	TaskTable[id].TaskAdrs = TS_BetMain_1st;		//�^�X�N�`�F���W
}

/*----------------------------------------------------
	�x�b�h�ɂǂ��ɃR�C���������邩�I�΂��鏈��
	�L�[���͈ˑ�

	FileRocal
	static u8 BetMainSelect(s16 *pos,u8 key);

	io	s16		���݂ǂ̃Z���ɃJ�[�\�������邩���i�[�����ϐ��ւ̃|�C���^
	i	u8		�ǂ̃L�[�������ꂽ���H
				0:	Up
				1:	Down
				2:	Left
				3:	Right

	�߂�l�F���ݖ��g�p
------------------------------------------------------*/
static u8 BetMainSelect( s16 * pos, u8 key )
{
	s8	min=0,now=0,max=0;
	s8	cnt_tbl[] = { -5, 5, -1, 1 };

	now = (s8)*pos;		//���ǂ��ɂ��邩�H

	switch( key ){
	case 0:
	case 1:
		min = *pos % 5;
		max = 15 + min;
		if( min == 0 )	min = 5;
		break;
	case 2:
	case 3:
		min = (*pos / 5) * 5;
		max = min + 4;
		if( min == 0 )	min = 1;
	}

	while(1){		//���łɃq�b�g���Ă���Z���͔�΂�
		*pos += (s16)cnt_tbl[ key ];
		if( *pos < min )	*pos = (s16)max;
		if( *pos > max )	*pos = (s16)min;
		if( *pos == now )	break;
		return TRUE;
	}
	return FALSE;
}

/*-----------------------------------------------------------------
	�ǂ��ɂ����邩�I�ԂƂ��̃L�[���͂��������A
	�e�[�u����`�悵�Ȃ���

	FileLocal
	static void BetMainSelectKeyIn(u8 id);
------------------------------------------------------------------*/
static void BetMainSelectKeyIn(u8 id)
{
	u8	i = 0,ofs = 0,flg = 0;

	if( sys.Trg & U_KEY ){
		flg = 1;
		if( BetMainSelect( &TaskTable[id].work[ BET_POS_WORK ], 0 ) == FALSE ) return;
	}
	if( sys.Trg & D_KEY ){
		flg = 1;
		if( BetMainSelect( &TaskTable[id].work[ BET_POS_WORK ], 1 ) == FALSE ) return;
	}
	if( sys.Trg & L_KEY ){
		flg = 1;
		if( BetMainSelect( &TaskTable[id].work[ BET_POS_WORK ], 2 ) == FALSE ) return;
	}
	if( sys.Trg & R_KEY ){
		flg = 1;
		if( BetMainSelect( &TaskTable[id].work[ BET_POS_WORK ], 3 ) == FALSE ) return;
	}
	if(!flg) return;

	DrawBetWindow(TaskTable[id].work[BET_POS_WORK]);
	DrawBetTable(id,CURSOL_POS);
	TaskTable[id].work[CNT_WORK] = 0;
	SePlay(SE_SELECT);

	//�J���[�A�j���[�V�����`�F���W
	ColorAnimeEnd(&MG_RLT->colanm,0xFFFF);
	MG_RLT->colanm.work[13].live_sw = 
	MG_RLT->colanm.work[14].live_sw = 
	MG_RLT->colanm.work[15].live_sw = 0;
	RltColorChange(TaskTable[id].work[BET_POS_WORK]);

	//�E�B���h�E�I�u�W�F�N�g�`�F���W
	for(i = 0;i < 4;i++)
	{
		ActWork[ MG_RLT->act_id[ WIN_POKE12+i]].oamData.CharNo =
			ActWork[ MG_RLT->act_id[WIN_POKE12+i]].char_num + 
			(u16)(ActWork[ MG_RLT->act_id[WIN_POKE12+i]].anm_tbl)[0][0].pat;		
	}
	if(TaskTable[id].work[BET_POS_WORK] > 0 && 5 >TaskTable[id].work[BET_POS_WORK]){
		//�c�̗񂪖��܂��Ă���Ƃ��́A�ς��Ȃ�
		if(MG_RLT->rlt_hp & g_rltBet[TaskTable[id].work[BET_POS_WORK]].flg) return;
		
		ofs = TaskTable[id].work[BET_POS_WORK]-1;
		ActWork[ MG_RLT->act_id[ WIN_POKE12+ofs]].oamData.CharNo =
			ActWork[ MG_RLT->act_id[WIN_POKE12+ofs]].char_num + 
			(u16)(ActWork[ MG_RLT->act_id[WIN_POKE12+ofs]].anm_tbl)[0][1].pat;		
	}
	return;
}

/*-----------------------------------------------------------------
	�x�b�h�e�[�u����\��������ŁA
	�ǂ��ɂ����邩�I�΂��鏈���ł�

	SubTask
	static void TS_BetMain(u8 id);

	Depend
		rlt_obj.c
		menutool.c
------------------------------------------------------------------*/
static void TS_BetMainEnd(u8 id)
{
	//�g�E�B���h�E����
	MG_RLT->bg0_cnt = 0xFF;

	if(MG_RLT->uc_rate == 1)	MG_RLT->bg_wait = 1;
	else						MG_RLT->bg_wait = 0;
	MG_RLT->bg_ct = 0;
	
	TaskTable[id].work[CNT_WORK] = 32;			//�^�C�}�[�Z�b�g
	TaskTable[id].TaskAdrs = TS_WinSlide_1st;	//�^�X�N�`�F���W
	return;
}
static void TS_BetMainEndInit(u8 id)
{
	//�q�����ꏊ��ۑ�
	MG_RLT->uc_aBet[MG_RLT->nBet] = (u8)TaskTable[id].work[BET_POS_WORK];
	//���I������Ă���ꏊ����A�|���������߂�
	TaskTable[id].work[ODDS_WORK] = GetOddsPreCell(MG_RLT->uc_aBet[MG_RLT->nBet]);
	//���݂̃I�b�Y�̕`��
	DrawOddsControl(MG_RLT->uc_aBet[MG_RLT->nBet]);
	
	TaskTable[id].work[COIN_WORK] -= MG_RLT->uc_rate;	//�莝���R�C�������炷����
	if(TaskTable[id].work[COIN_WORK] < 0) TaskTable[id].work[COIN_WORK] = 0;
	DrawCoinControl(TaskTable[id].work[COIN_WORK]);
	
	TaskTable[id].TaskAdrs = TS_BetMainEnd;	//�^�X�N�`�F���W
	return;
}
static void TS_BetMain( u8 id )
{
	//�L�[����͏������A�K�؂ȃx�b�h�e�[�u���̏�Ԃ�`�悷��
	BetMainSelectKeyIn(id);

	//�I��g�̓_�ŏ���
	switch(TaskTable[id].work[CNT_WORK])
	{
	case 0:
		DrawWakuWindow((u8)TaskTable[id].work[BET_POS_WORK]);
		TaskTable[id].work[CNT_WORK]++;
		break;
	case 30:
		DrawWakuWindow(0);
		TaskTable[id].work[CNT_WORK]++;
		break;
	case 59:
		TaskTable[id].work[CNT_WORK] = 0;
		break;
	default:
		TaskTable[id].work[CNT_WORK]++;
		break;
	}
	
	if(!(sys.Trg & A_BUTTON)) return;
	
	//�I�����ꂽ�Z���ɁA���łɃq�b�g���Ă����ꍇ�́A�I������Ȃ�
	if(MG_RLT->rlt_hp & g_rltBet[TaskTable[id].work[BET_POS_WORK]].flg){
		//SE��炷
		SePlay(SE_BOO);
		return;
	}
	
	//���艹��炷
	m4aSongNumStart(SE_REGI);
	TaskTable[id].TaskAdrs = TS_BetMainEndInit;	//�^�X�N�`�F���W
	return;
}

/*------------------------------------------------
	��ʂ��X���C�h������P
	�X���C�h���I��������A�{�[��������

	static void TS_WinSlide_1st(u8 id)
--------------------------------------------------*/
static void TS_WinSlide_1st(u8 id)
{
	if(TaskTable[id].work[CNT_WORK]-- > 0)
	{
		if(TaskTable[id].work[CNT_WORK] > 2) ActOX += 2;
		MG_RLT->bg1_scrX += 4;

		if(MG_RLT->bg1_scrX == 104){
			ActWork[MG_RLT->act_id[WIN_ODDS]].move = DummyActMove;
		}
		return;
	}

	BanishRoulettePokeControl(1,0xFF);
	BanishSTCellControl(1,0xFF);

	TaskTable[id].TaskAdrs = TS_BallParamInit;	//�^�X�N�`�F���W
	TaskTable[id].work[CNT_WORK] = 0;		//�^�C�}�[�N���A
}
/*-----------------------------------------------------
	�{�[���̏����̃����_������Ԃ�

	u8 get_range(u16 play_num,u16 rand)
-------------------------------------------------------*/
static u8 get_range(u16 play_num,u16 rand)
{
	switch(MG_RLT->mons_flg){
	case 1:
	case 2:
		if(GameTime.hour < 10 && GameTime.hour >= 4 ){
			if(play_num < 12 || (rand&1)) return g_table[MG_RLT->rlt_ver].uc_rBaseH/2;
			return 1;
		}
		if((rand & 3)==0){
			return g_table[MG_RLT->rlt_ver].uc_rBaseH/2;
		}
		return g_table[MG_RLT->rlt_ver].uc_rBaseH;		
	case 3:
		if(GameTime.hour < 11 && GameTime.hour >= 4){
			if(play_num < 6 || (rand&1)) return g_table[MG_RLT->rlt_ver].uc_rBaseH/2;
			return 1;
		}
		if((rand&1) && play_num > 6) return g_table[MG_RLT->rlt_ver].uc_rBaseH/4;
		return g_table[MG_RLT->rlt_ver].uc_rBaseH/2;
	case 0:
	default:
		if(GameTime.hour < 10 && GameTime.hour >= 4){
			if((rand & 3) == 0) return 1;
			return g_table[MG_RLT->rlt_ver].uc_rBaseH/2;
		}
		else if((rand & 3) == 0){
			if(play_num > 12) return g_table[MG_RLT->rlt_ver].uc_rBaseH/2;
			else			  return g_table[MG_RLT->rlt_ver].uc_rBaseH;
		}
		else if(rand & 0x8000){
			if(play_num > 12) return g_table[MG_RLT->rlt_ver].uc_rBaseH;
			else			  return g_table[MG_RLT->rlt_ver].uc_rBase;
		}
		else{
			return g_table[MG_RLT->rlt_ver].uc_rBase*2;
		}
		return 0;
	}
}
/*------------------------------------------------------
	�J�����g�{�[���̏����p�����[�^���Z�b�g

	static void TS_BallParamInit(u8 id);
--------------------------------------------------------*/
static void TS_BallParamInit(u8 id)
{
	u16		t = 0;
	u16		aRot[4] = {0,180,90,270};
	u16		r_seed = pp_rand();
	u16		r_seed100 = r_seed % 100;
	s8		rand,ofs;
	u8		range;
	
	MG_RLT->ball.uc_no = GetPreBallNo(id);		//�J�����g�̋ʂ̃i���o�[���L��
	MG_RLT->ball.uc_flg =
	MG_RLT->ball.uc_cell =
	MG_RLT->ball.uc_area = 0;

	//�����p�����[�^�̌���
	range = get_range(TaskTable[id].work[NPLAY_WORK],r_seed);
	rand =	r_seed%range - (range/2);

	if(GameTime.hour < 13)	ofs = 0;
	else					ofs = 1;
	if(r_seed100 < 80)	ofs = ofs*2;
	else				ofs = (1-ofs)*2;
	
	//�ʂ�stop����܂ł̎��Ԃ�����
	MG_RLT->ball.us_syncMax = g_table[MG_RLT->rlt_ver].w_t0 + rand;

	t = (u16)((float)MG_RLT->ball.us_syncMax / 5);
	MG_RLT->ball.us_syncA0 = t * 3;
	MG_RLT->ball.us_syncA1 = t;
	MG_RLT->ball.us_syncA2 = t;

	//�p���x
	MG_RLT->ball.fr =  (float)aRot[(r_seed&0x0001)+ofs];
	MG_RLT->ball.frv = (float)g_table[MG_RLT->rlt_ver].w_v0;
	MG_RLT->ball.fra = (((MG_RLT->ball.frv/2.0f) - MG_RLT->ball.frv) / (float)MG_RLT->ball.us_syncA0);

	//���a�ɑ΂��鑬�x
	MG_RLT->ball.fw  = RLT_AREA1_OFS;
	MG_RLT->ball.fwa = 0.0f;
	MG_RLT->ball.fwv = -(RLT_AREA1_SIZ / (float)MG_RLT->ball.us_syncA0);
	MG_RLT->ball.fwo = 36;

	TaskTable[id].TaskAdrs = TS_BallStart;	//�^�X�N�`�F���W
}
/*------------------------------------------------------
	�{�[�����X�^�[�g������

	static void TS_BallStart(u8 id);
--------------------------------------------------------*/
static void TS_BallStart(u8 id)
{
	//�{�[�����A�N�e�B�u�ɂȂ����t���O�𗧂Ă�
	MG_RLT->ev_bact = 1;

	//�{�[�����R���g���[���J�n
	GetPreActwork(RLT_BALL0 + MG_RLT->ball.uc_no);
	MG_RLT->pAct->move = BallMoveAct_ST0;
	
	//�Q�[�������v���X
	TaskTable[id].work[LOOP_WORK]++;	//1�Q�[���̃v���C��
	TaskTable[id].work[NPLAY_WORK]++;	//�A���v���C��

	DrawRestballControl(RLT_BALL_MAX - (u8)TaskTable[id].work[LOOP_WORK]);
	
	//�ʂ��]���鉹�J�n
	m4aSongNumStart(SE_TAMAKORO);
	
	TaskTable[id].TaskAdrs = TS_ResultWait;	//�^�X�N�`�F���W
}
/*
//--------------------
//		�Q�[���X�^�[�g
//--------------------
*/
//--------------------------
//		�q������̏���
//--------------------------

/*------------------------------------------------------
	���[���b�g������Ă���Ԃ̏����ł��B
	�ʂ�]�����A���ʂ��o���܂��B

	SubTask
	static void ResulutWait(u8 id);

	Depend
		pm_str.h
-----------------------------------------------------*/
static void TS_ResultWait( u8 id )
{
	//�ʂ��Ƃ܂�܂ő҂���
	if(!(MG_RLT->ball.uc_flg & 0xFF )) return;

	//�C�x���g�I���܂ł͋A��
	if(MG_RLT->ev_on){
		//�C�x���g�I���`�F�b�N
		if(MG_RLT->ev_end){
			MG_RLT->ev_end = 0;	//�t���O���Z�b�g
			MG_RLT->ev_on = 0;
		}
		return;
	}
	if(TaskTable[id].work[CNT_WORK] == 0){
		//���ʂ��`�F�b�N
		TaskTable[id].work[WIN_WORK] = 
			WinCheck(ResultFlgSet(id,MG_RLT->ball.uc_cell),MG_RLT->uc_aBet[MG_RLT->nBet]);

		//�������Ă�����A�J���[�A�j���N��
		if(TaskTable[id].work[WIN_WORK] == RLT_RET_WIN){
			ColorAnimeStart(&MG_RLT->colanm,COLANM_SW12);
		}
	}
	
	// �^�C��������܂ŃE�F�C�g������
	if(TaskTable[id].work[ CNT_WORK ] <= 60)
	{
		if(sys.Trg & A_BUTTON) TaskTable[id].work[CNT_WORK] = 60;
		TaskTable[id].work[ CNT_WORK ]++;
		return;
	}

	DrawBetWindow(MG_RLT->uc_aBet[MG_RLT->nBet]);	//�e�[�u�����ĕ\��
	
	//�{�[����������f���̏���
	BanishRoulettePokeControl(0,(u8)TaskTable[id].work[LASTHIT_WORK]);	
	BanishSTCellControl(0,TaskTable[id].work[LOOP_WORK]-1);	//�{�[�����������Z���ɁA�{�[����\��

	TaskTable[id].work[ CNT_WORK ] = 32;	//�J�E���^�Z�b�g
	TaskTable[id].TaskAdrs = TS_WinSlide_2nd;

	return;
}

/*--------------------------------------------------
	��ʂ��X���C�h������Q
	�{�[�������������ƁA��ʂ����ɖ߂�

	static void TS_WinSlide_2nd(u8 id);
----------------------------------------------------*/
static void TS_WinSlide_2nd(u8 id)
{
	if(TaskTable[id].work[CNT_WORK]-- > 0)
	{
		if(TaskTable[id].work[CNT_WORK] > 2) ActOX -= 2;
		MG_RLT->bg1_scrX -= 4;

		if(MG_RLT->bg1_scrX == 104){
			ActWork[MG_RLT->act_id[WIN_ODDS]].move = Bg1_ScrollActMove;
		}
		return;
	}
	//�I��g�̏���
	BanishWakuControl(TaskTable[id].work[LASTHIT_WORK]);	

	if(TaskTable[id].work[WIN_WORK] == RLT_RET_WIN)
		TaskTable[id].work[ CNT_WORK ] = 121;		//�J�E���^�Z�b�g
	else
		TaskTable[id].work[ CNT_WORK ] = 61;		//�J�E���^�Z�b�g
	TaskTable[id].TaskAdrs = TS_ResultDemo;
}

/*-----------------------------------------------
	���ʂ�\������f��

	static void TS_ResultDemo(u8 id);
--------------------------------------------------*/
static void TS_ResultDemo(u8 id)
{
	//�f����
	if(TaskTable[id].work[CNT_WORK]-- > 1)
	{
		//���������ꏊ�̓_�ŕ\��
		switch(TaskTable[id].work[CNT_WORK]%16)
		{
		case 8:
			BanishRoulettePokeControl(0,0xFF);	
			BanishSTCellControl(0,0xFF);
			break;
		case 0:
			BanishRoulettePokeControl(0,(u8)TaskTable[id].work[LASTHIT_WORK]);	
			BanishSTCellControl(0,TaskTable[id].work[LOOP_WORK]-1);
			break;
		default:
			break;
		}
		return;	
	}
	//�f���I���
	SetWaitNextTask(id,TS_ResultSet,30,0);
}

/*-------------------------------------------
	���ʂ����b�Z�[�W�ŊJ��
----------------------------------------------*/
//SE���Ȃ�I���܂ő҂�
static void TS_ResultSetEnd(u8 id)
{
	switch(TaskTable[id].work[WIN_WORK])
	{
	case RLT_RET_WIN:
	case RLT_BONUS_WIN:
		if(JingleWait()==0)return;
		
		//�A�����������J�E���g
		if(GetRecord(CNT_ROULETTE) < ++TaskTable[id].work[NWIN_WORK]){
			SetRecord(CNT_ROULETTE,TaskTable[id].work[NWIN_WORK]);
		}
		SetWaitNextTask(id,TS_ReturnBetFunc,0xFFFF,A_BUTTON|B_BUTTON);
		break;
	case RLT_RET_ROSE:
	default:
		if(SeEndCheck()) return;
		//�A�������������Z�b�g
		TaskTable[id].work[NWIN_WORK] = 0;
		SetWaitNextTask(id,TS_ReturnBetFunc2,0xFFFF,A_BUTTON|B_BUTTON);
		break;
	}
	return;
}
//���ʃ��b�Z�[�W�̕\��
static void TS_ResultSet(u8 id)
{
	//�����������b�Z�[�W�̕\��
	switch(TaskTable[id].work[WIN_WORK])
	{
	case RLT_RET_WIN:	//����
	case RLT_BONUS_WIN:	//�{�[�i�X���ɏ�����
		if(TaskTable[id].work[ODDS_WORK] == 12){
			JinglePlay(MUS_ME_B_BIG);
			MsgWrite(MSG_WIN2);
		}
		else{
			JinglePlay(MUS_ME_B_SMALL);
			MsgWrite(MSG_WIN);	
		}
		break;
	case RLT_RET_ROSE:	//����
	default:
		m4aSongNumStart(SE_HAZURE);
		MsgWrite(MSG_ROST);
		break;
	}
	TaskTable[id].work[CNT_WORK] = 0;
	TaskTable[id].TaskAdrs = TS_ResultSetEnd;
	return;
}

/*--------------------------------------------
	�R�C���̑���

	SubTask
	static void TS_AddCoin(u8 id);
----------------------------------------------*/
static void TS_AddCoin(u8 id)
{
	//���ʉ���炷
	switch(TaskTable[id].work[WAIT_WORK]){
	case 0:
		TaskTable[id].work[COIN_WORK] += 1;
		m4aSongNumStart(SE_PIN);
		DrawCoinControl(TaskTable[id].work[COIN_WORK]);
		
		if(TaskTable[id].work[COIN_WORK] >= MY_COIN_MAX){
			TaskTable[id].work[CNT_WORK] = MY_COIN_MAX;
			TaskTable[id].work[CNT_WORK] = 0;
			break;
		}
		TaskTable[id].work[CNT_WORK]--;
		TaskTable[id].work[WAIT_WORK]++;
		break;
	case 3:
		m4aSongNumStop(SE_PIN);
		TaskTable[id].work[WAIT_WORK] = 0;
		break;
	default:
		TaskTable[id].work[WAIT_WORK]++;
		break;
	}
	
	if(TaskTable[id].work[CNT_WORK] != 0) return;
	SetWaitNextTask(id,TS_ReturnBetFunc2,0xFFFF,A_BUTTON|B_BUTTON);
}
/*-------------------------------------------
	���b�Z�[�W�\����A���������ɂ��R�C���̑���

	SebTask
	static void TS_ReturnBetFunc(u8 id);

	Depend
		task.h
		coin.c
-----------------------------------------------*/
static void TS_ReturnBetFunc( u8 id )
{
	//�������Ƃ��ɂ́A���������̃R�C���𑫂�
	PM_NumMsgSet(
		StrTempBuffer0,
		MG_RLT->uc_rate*TaskTable[id].work[ODDS_WORK],
		NUM_MODE_LEFT, 2 );
	PM_MsgExpand( MsgExpandBuffer,MSG_GETCOIN);

	//�R�C���m�܂� �f�d�s
	MsgWrite(MsgExpandBuffer);
		
	TaskTable[id].work[CNT_WORK] = 
		MG_RLT->uc_rate*TaskTable[id].work[ODDS_WORK];
	
	TaskTable[id].work[WAIT_WORK] = 0;
	TaskTable[id].TaskAdrs = TS_AddCoin;	//�^�X�N�`�F���W
}

/*----------------------------------------------
	���ʂ��o����AA�EB�{�^�����������܂őҋ@

	SubTask
	static void TS_ReturnBetFunc2(u8 id);

	Depend
		common.h / system.h
		menutool.c
------------------------------------------------*/
static void TS_ReturnBetFunc2( u8 id )
{
	//�J���[�A�j���[�V�����I���
	ColorAnimeEnd(&MG_RLT->colanm,0xFFFF);
	MG_RLT->colanm.work[13].live_sw = 
	MG_RLT->colanm.work[14].live_sw = 
	MG_RLT->colanm.work[15].live_sw = 0;

	//�q�b�g�����|�P�����̃I�u�W�F�N�g��Banish
	ActWork[MG_RLT->act_id[RLT_RPOKE00+g_rltBet[TaskTable[id].work[LASTHIT_WORK]].cID]].banish = 1;

	TaskTable[id].TaskAdrs = TS_NextPlaySelectInit;
	return;
}
static void TS_NextPlaySelectInit(u8 id)
{
	u8 i = 0;

	//���֐i��
	TaskTable[id].work[ BET_POS_WORK ] = 0;	//�������q�����ꏊ���N���A
	MG_RLT->uc_aBet[MG_RLT->nBet] = 0;

	DrawBetWindow(0);		//�x�b�h�ƃe�[�u�������������Ă���
	ActWork[MG_RLT->act_id[RLT_WAKU]].banish = 1;
	
	//�E�B���h�E�I�u�W�F�N�g�`�F���W
	for(i = 0;i < 4;i++)
	{
		ActWork[ MG_RLT->act_id[ WIN_POKE12+i]].oamData.CharNo =
			ActWork[ MG_RLT->act_id[WIN_POKE12+i]].char_num + 
			(u16)(ActWork[ MG_RLT->act_id[WIN_POKE12+i]].anm_tbl)[0][0].pat;		
	}

	//�R�C��������΂�����x�v���C���邩�̃��b�Z�[�W���o���āA�^�X�N�`�F���W
	if(TaskTable[id].work[COIN_WORK] >= MG_RLT->uc_rate){
		
		//6��v���C���Ă����烊�Z�b�g
		if(TaskTable[id].work[LOOP_WORK] == RLT_GAME_MAX)
		{
			//�Q�[�����Z�b�g���b�Z�[�W���o��
			MsgWrite(MSG_GCLEAR);
			
			//�^�X�N�`�F���W
			SetWaitNextTask(id,TS_GameStateReset,0xFFFF,A_BUTTON|B_BUTTON);
			return;
		}
		
		//���Z�b�g���Ȃ��Ƃ�
		if(TaskTable[id].work[COIN_WORK] == MY_COIN_MAX){
			MsgWrite(MSG_FULLCOIN);
			SetWaitNextTask(id,TS_NextPlaySelectMain,0xFFFF,A_BUTTON|B_BUTTON);
		}else{
			TaskTable[id].TaskAdrs = TS_NextPlaySelectMain;
		}
	}
	else{	//�R�C���͂�������Ȃ�/�v���C�����I��
		MsgWrite(MSG_SPEND);
		SetWaitNextTask(id,TS_PlayNo,60,A_BUTTON|B_BUTTON);
	}
	return;
}

/*------------------------------------------------
	1�Z�b�g���̃Q�[���̏�Ԃ����Z�b�g

	SubTask
	static void TS_GameStateReset(u8 id);
---------------------------------------------------*/
static void TS_GameStateReset(u8 id)
{
	u8	i = 0;
	
	//�Q�[���̃X�e�[�g���N���A
	GameSetClear(id);

	//�Ղ̃|�P�����I�u�W�F�N�g�𕜋A
	for(i = 0;i < 12;i++){
		ActWork[MG_RLT->act_id[RLT_RPOKE00+i]].banish = 0;
	}
	
	if(TaskTable[id].work[COIN_WORK] == MY_COIN_MAX){
		MsgWrite(MSG_FULLCOIN);
		SetWaitNextTask(id,TS_NextPlaySelectMain,0xFFFF,A_BUTTON|B_BUTTON);
	}else{
		TaskTable[id].TaskAdrs = TS_NextPlaySelectMain;
	}
	return;
}

//=========================================
//		�I�����ăt�B�[���h�֕��A
//=========================================
/*----------------------------------------
	�Q�[�����I������O�ɁA�t�F�[�h�A�E�g��������

	FileLocal
	static void RouletteEndInit(u8 id);

	u8	id	�Ăяo���T�u�^�X�N�i���o�[

	Depend
		task.h
		palanm.h/palanm.c
------------------------------------------*/
static void RouletteEndInit( u8 id )
{
	//�J���[�A�j��������~
	ColorAnimeEnd(&MG_RLT->colanm,0xFFFF);
	ColorAnimeInit(&MG_RLT->colanm);
	
	//�R�C���̎c�ʂŃt���O�𗧂ĂĂ���
	//�t�B�[���h���A�����Ƃ��̃��b�Z�[�W�ύX�p
	Fld.my_coin = TaskTable[id].work[COIN_WORK];
	if(Fld.my_coin < MG_RLT->uc_rate)	ScriptParameter0 = 1;
	else								ScriptParameter0 = 0;
	
	//�t�F�[�h�A�E�g���N�G�X�g
	HardFadeReq(0xFF,0,0,16,0);
	
	//�^�X�N�`�F���W
	TaskTable[id].TaskAdrs = TS_RouletteEnd;
}

/*-------------------------------------------
	�Q�[�����I�����A�t�B�[���h�փ��C���^�X�N�𕜋A����

	SubTask
	static void TS_RouletteEnd(u8 id);

	Depend
		task.h
		palanm.h
		rlt_obj.h
		fld_main.h
		fld_task.h
------------------------------------------------*/
static void TS_RouletteEnd( u8 id )
{
	if(PaletteAnime()) return;

	SetVBlankFunc(NULL);	// �����݊֐��ݒ�(VBLANK)

	//�g�p�������[�N�G���A������
	memset(MG_RLT,0,sizeof(ROULETTE));
	
	//�A�N�^�[�p�X�N���[���X�N���[���O���[�o���l��������
	ActOX = ActOY = 0;
	
	ItemToolAGBRegInit();		// ���W�X�^�EVRAM�EOAM�EPALRAM������
	
	ObjPalManInit();			// �I�u�W�F�N�g�p���b�g�}�l�[�W��������
	PaletteAnimeInit();			// ��گı�� ������
	InitActor();				// ����������
	BGDataInit();				// BG�W�J�p�o�b�t�@������
	
	//�u�����f�B���O�p�����[�^�����Z�b�g
	*(vu16 *)REG_BLDCNT = 0;
	*(vu16 *)REG_BLDALPHA = 0;
	*(vu16 *)REG_BLDY = 0;
		
	pFieldRecoverFunc = AddScriptFieldInTask;	//fld_main.h / fld_task.h
	MainProcChange( FieldMainRecover );			//���C���^�X�N�`�F���W
	DelTask(id);	//���[���b�g�T�u�^�X�N�폜

#ifdef PM_DEBUG
	//�f�o�b�O�p�̃t���O��؂�
	g_rlt_debug_sw = 0;
#endif	//PM_DEBUG	
	return;
}

//=======================================================
//	���[�J���֐�
//=======================================================

////////////////////////////////////////////////////////
//	�^�X�N����֘A
////////////////////////////////////////////////////////
/*----------------------------------------------
	�E�F�C�g�҂��̃T�u�^�X�N

	static void TS_WaitNextTask(u8 id);
------------------------------------------------*/
static void TS_WaitNextTask(u8 id)
{
	//�E�F�C�g�I�����A�L�����Z���L�[��������Ă����玟
	if(	(MG_RLT->task.us_wait == 0) ||
		(sys.Trg & MG_RLT->task.us_cKey) ){
		TaskTable[id].TaskAdrs = MG_RLT->task.wait_ret_adrs;
	
		//�L�[���Ƃ����ꍇ�ASE��炷
		if(MG_RLT->task.us_cKey != 0){
			SePlay(SE_SELECT);
		}

		//�g�p�����p�����[�^�N���A
		MG_RLT->task.wait_ret_adrs = NULL;
		MG_RLT->task.us_wait = MG_RLT->task.us_cKey = 0;
	}
	//�L�����Z�������܂ŁA�������[�v�łȂ���΁A�E�F�C�g�l���}�C�i�X
	if(MG_RLT->task.us_wait != 0xFFFF) MG_RLT->task.us_wait--;
}

/*----------------------------------------------
	���̃E�F�C�g�������āA���̃^�X�N�ɐ؂�ւ���

	io	u8			���݂̃T�u�^�X�NID
	i	pTaskFunc	�E�F�C�g��ɔ�ԃ^�X�N
	i	u16			�E�F�C�g�l(Sync)--0xFFFF�̏ꍇ�̓L�[���Ƃ�܂Ŗ����ɃE�F�C�g
	i	u16			�L�[�L�����Z���ł���ꍇ�̃L�[�t���O
------------------------------------------------*/
static void SetWaitNextTask(u8 id,pTaskFunc pAdrs,u16 wait,u16 key)
{
	//���݂̃A�h���X��ۑ�
	MG_RLT->task.back_adrs = TaskTable[id].TaskAdrs;

	//���ɔ�ԃA�h���X��ۑ�
	if(pAdrs == NULL) pAdrs = MG_RLT->task.back_adrs;
	MG_RLT->task.wait_ret_adrs = pAdrs;
	
	//�E�F�C�g�̕ۑ�
	MG_RLT->task.us_wait = wait;

	//�L�����Z���L�[�̕ۑ�
	if(wait == 0xFFFF && key == 0){		//�������[�v�w��̂Ƃ��ɁA�L�[�w�肪�Ȃ�������
		MG_RLT->task.us_cKey = 0xFFFF;	//�Ȃ񂩉����ꂽ�甲����
	}
	else{
		MG_RLT->task.us_cKey = key;
	}

	//�^�X�N�`�F���W
	TaskTable[id].TaskAdrs = TS_WaitNextTask;
}

////////////////////////////////////////////////////////
//	�Q�[���̏�Ԃ��N���A����
////////////////////////////////////////////////////////
/*--------------------------------------------
	1�Q�[���̏�Ԃ��N���A
----------------------------------------------*/
static void OneGameStateClear(u8 id)
{
	u8	ct = 0;

	MG_RLT->time_flg = 0;

	//�C�x���g�֘A�̃t���O�N���A
	MG_RLT->ev_bact = 0;		//�{�[���̃A�N�e�B�u����
	MG_RLT->ev_on = 0;			//�X�^�b�N�C�x���g�t���O�N���A
	MG_RLT->ev_end = 0;
	MG_RLT->ev_no = 0;

	//�x�b�g����ꏊ���N���A
	for(ct = 0;ct < RLT_BET_MAX;ct++)
		MG_RLT->uc_aBet[ct] = 0;

	//���ݓq���Ă��閇�����N���A
	MG_RLT->nBet = 0;
	
	//�^�C�}�[�N���A
	TaskTable[id].work[CNT_WORK] = 0;
}

////////////////////////////////////////////////////////
//	�Z���̃q�b�g��ԊǗ�
////////////////////////////////////////////////////////
/*-----------------------------
	�Z���̃q�b�g��Ԃ��N���A����
	
	FileLocal
	static void HitCellClear(void);
--------------------------------*/
static void HitCellClear(void)
{
	u8 i;
		
	MG_RLT->rlt_hp = 0;

	for(i = 0;i < RLT_BET_MAX;i++)
	{
		MG_RLT->hit_cell[i] = 0;
	}
	//�c���C���N���A
	for(i = 0;i < 4;i++)
		MG_RLT->hit_vLine[i] = 0;
	for(i = 0;i < 3;i++)
		MG_RLT->hit_hLine[i] = 0;

	//�e�[�u���p�{�[���\���I�u�W�F�N�g���N���A
	BanishSTCellControl(1,0xFF);
	return;
}

/*---------------------------------------------------
	�q�b�g�����Z���i���o�[����A���ʂ��t���O�ɕۑ�����
	�q�b�g�����Z���̃t���O�i���o�[��Ԃ�

	FileLocal
	static u8 ResultFlgSet(u8 id,u8 hit);

	i	u8	�T�u�^�X�N�̃��C��ID
	i	u8	�q�b�g�����Z���i���o�[(0-11)

	return u8	�q�b�g�����Z���̃t���O�i���o�[

	�r�b�g�Ή��\					// �G���A�i���o�[�Ή��\
		x  | 01 | 02 | 03 | 04		x  | XX | XX | XX | XX
		05 | 06 | 07 | 08 | 09		x  | 01 | 03 | 05 | 07
		10 | 11 | 12 | 13 | 14		x  | 09 | 11 | 13 | 15
		15 | 16 | 17 | 18 | 19		x  | 17 | 19 | 21 | 23
------------------------------------------------------*/
static u8 ResultFlgSet(u8 id, u8 hit )
{
	u32 vflg[4] = {BET_FLG_VP1,BET_FLG_VP2,BET_FLG_VP3,BET_FLG_VP4};
	u32 hflg[3] = {BET_FLG_HR,BET_FLG_HG,BET_FLG_HB};
	u8	i,j;
	
	//�ςȐ��l�������Ă�����͂���
	if(hit >= RLT_CELL_MAX){
		
		return 0;
	}

	//�q�b�g�����Z�������ԂɊo���Ă���
	MG_RLT->hit_cell[TaskTable[id].work[LOOP_WORK]-1] = g_rltCell[hit].uc_bit;

	//�Ō��HIT�����ꏊ��ۑ�
	TaskTable[id].work[LASTHIT_WORK] = (u16)g_rltCell[hit].uc_bit;

	//�S�̊Ǘ��̃t���O�𗧂Ă�
	MG_RLT->rlt_hp |= g_rltCell[hit].ui_flg;

	//�c
	for(j = 0;j < 4;j++){
		if(g_rltCell[hit].ui_flg & vflg[j]) MG_RLT->hit_vLine[j]++;

		//��������̂Ńt���O�𗧂Ă�
		if(MG_RLT->hit_vLine[j] >= 3) MG_RLT->rlt_hp |= vflg[j];
	}
	//��
	for(i = 0;i < 3;i++){
		if(g_rltCell[hit].ui_flg & hflg[i]) MG_RLT->hit_hLine[i]++;

		//��������̂Ńt���O�𗧂Ă�
		if(MG_RLT->hit_hLine[i] >= 4) MG_RLT->rlt_hp |= hflg[i];
	}

	return	g_rltCell[hit].uc_bit;
}

/*-----------------------------------------------
	�����E��������

	FileLocal
	static u8 WinCheck(u8 bitNo, u8 bet);

	i	u8	hit		�q�b�g�����Z���̃t���O�r�b�g�i���o�[
	i	u8	bet		�q���Ă����ꏊ�̃i���o�[(1-19)

	return	u8		0: RLT_RET_ROSE		����
					1: RLT_RET_WIN		����
					2: RLT_BONUS_WIN	�{�[�i�X���̏���
					3: RLT_NO_BET		�q���Ă��Ȃ�

	�t���O�r�b�g�Ή��\				// �G���A�i���o�[�Ή��\
		x  | 01 | 02 | 03 | 04		x  | XX | XX | XX | XX
		05 | 06 | 07 | 08 | 09		x  | 01 | 03 | 05 | 07
		10 | 11 | 12 | 13 | 14		x  | 09 | 11 | 13 | 15
		15 | 16 | 17 | 18 | 19		x  | 17 | 19 | 21 | 23	
-------------------------------------------------*/
static u8 WinCheck( u8 bitNo, u8 bet )
{
	//�ςȐ��l�������Ă�����ⓚ���p�ŊO��(��)
	if(bitNo == 0 || bitNo > 19) return RLT_RET_ROSE;
	
	switch( bet ){
	case 0:	//�q���Ă��Ȃ��Ƃ�
		return RLT_NO_BET;
	case 1:
	case 2:
	case 3:
	case 4:
		//�|�P�����̎�ނɓq�����Ƃ�
		if( bitNo == 5+bet || bitNo == 10+bet || bitNo == 15+bet )
			return	RLT_RET_WIN;//+(u8)MG_RLT->rlt_bonus;
		break;
	case 5:
	case 10:
	case 15:
		//�F�ɓq�����Ƃ�
		if( bitNo >= bet+1 && bitNo <= bet+4 )
			return	RLT_RET_WIN;// + (u8)MG_RLT->rlt_bonus;
		break;
	default:
		//�P�̂ɓq�����Ƃ�
		if( bitNo == bet )	return	RLT_RET_WIN;// + (u8) MG_RLT->rlt_bonus;
	}

	//�O�ꂽ�Ƃ�
	return	RLT_RET_ROSE;
}

//////////////////////////////////////////////////////////////////
//	�`��n�⏕
///////////////////////////////////////////////////////////////////
/*----------------------------------------------------
	�ǂ��ɂ��������ŁA�J���[�A�j���[�V�������N��

	staic void RltColorChange(u8 pos)
	
	u8 pos �q�����ꏊ�̃Z���i���o�[(0-19)
------------------------------------------------------*/
static void RltColorChange(u8 pos)
{
	u8	num = 0,i = 0,no = 0;
	u16	ofs = 0,flg = 0;
	ColAnmData	dat[3];
	
	switch(pos)
	{
	case 5:
	case 10:
	case 15:
		for(i = pos+1;i < pos+5;i++){
			if(MG_RLT->rlt_hp & g_rltBet[i].flg) continue;
			flg |= g_rltBet[i].canm;
		}
		ColorAnimeStart(&MG_RLT->colanm,(flg & (COLANM_SW13^0xFFFF)));
		return;
	default:
		break;
	}
	
	memcpy(&dat,&(g_colanm[13]),sizeof(ColAnmData)*3);
	
	//�I�u�W�F�p�p���b�g�̃J���[��
	if(pos > 0 && pos < 5){
		num = 3;
	}
	else{
		num = 1;
	}
	ofs = (pos / 5) - 1;
	//�J���[�i���o�[
	switch(pos%5)
	{
	case 1:
		ofs = ActWork[MG_RLT->act_id[RLT_RPOKE00]].oamData.Pltt*16;
		break;
	case 2:
		ofs = ActWork[MG_RLT->act_id[RLT_RPOKE01]].oamData.Pltt*16;
		break;
	case 3:
		ofs = ActWork[MG_RLT->act_id[RLT_RPOKE02]].oamData.Pltt*16;
		break;
	case 4:
		ofs = ActWork[MG_RLT->act_id[RLT_RPOKE03]].oamData.Pltt*16;
		break;
	}
	if(num == 1){
		if(MG_RLT->rlt_hp & g_rltBet[pos].flg) return;
		dat[(pos/5)-1].w_colNo += ofs;
		AddColorAnime(&MG_RLT->colanm,13,&(dat[pos/5-1]));
	}
	else{
		for(i = 0;i < 3;i++){
			no = 5*i+pos+5;
			if(MG_RLT->rlt_hp & g_rltBet[no].flg) continue;
			dat[(no/5)-1].w_colNo += ofs;
			AddColorAnime(&MG_RLT->colanm,13+i,&(dat[(no/5)-1]));
			
			if(num == 3) flg = g_rltBet[no].canm;
			num--;
		}

		if(num != 2) flg = 0;
	}
	
	ColorAnimeStart(&MG_RLT->colanm,g_rltBet[pos].canm | flg);
	return;
}

/*------------------------------------
	���݂̃q�b�g��Ԃ��`�F�b�N���ăx�b�h�E�B���h�E��\��

	FileLocal
	static void DrawBetWindow(u8 pos);

	Depend
		rlt_obj.c
		itemtool.h / screen.h / screen.c
------------------------------------*/
static void DrawBetWindow(u8 pos)
{
	vu8		i,j;
	vu16	x,y;
	vu8		dy;
	u8		no,tbl[5];

	//Bg1�̏��������t���O�𗧂ĂĂ���
	MG_RLT->bg1_cnt = 1;

	//�q�b�g���Ă��邩���Ȃ����`�F�b�N���āA�q�b�g���Ă���ꏊ�̃|�P����OBJ�͏���
	BanishRoulettePokeControl(0,0);

	//�܂��͑S�̂̃E�B���h�E��`���Ă��܂�
	SetChar16(BG1_data,BG01_MAP,14,7,16,13);
	
	//���ݑI������Ă���Z�����A���邭�\��
	switch(pos){
	case 0:
		return;
	case 1:
	case 2:
	case 3:
	case 4:
		no = 4;
		for(i = 0;i < no;i++){
			tbl[i] = pos+i*5;
		}
		break;
	case 5:
	case 10:
	case 15:
		no = 5;
		for(i = 0;i < no;i++){
			tbl[i] = pos+i;
		}
		break;
	default:
		no = 1;
		tbl[0] = pos;
		break;
	}
	for(i = 0;i < no;i++)
	{
		//���߂�{�b�N�X�f�[�^�̃L�����N�^�ԍ�
		dy	= g_rltBet[tbl[i]].dy;
		x	= g_rltBet[tbl[i]].posx;
		for( j=0; j<3; j++ ){	//���F�̃{�b�N�X��`��
			y = (g_rltBet[tbl[i]].posy+j)<<5;
			BG1_data[ x+y ]   = BG01_SMAP[(dy+j)*3 + 0];
			BG1_data[ x+y+1 ] = BG01_SMAP[(dy+j)*3 + 1];
			BG1_data[ x+y+2 ] = BG01_SMAP[(dy+j)*3 + 2];
		}
	}
	return;
}

/*-----------------------------------------------
	�w��Z���̃I�b�Y��Ԃ�

	static u8 GetOddsPreCell(u8 no);

	u8	no	�w�肷��e�[�u���Z���i���o�[
--------------------------------------------------*/
static u8 GetOddsPreCell(u8 no)
{
	u8	oddsTbl[5] = {0,3,4,6,12};

	if(no > 19) no = 0;
	
	switch(g_rltBet[no].odds)
	{
	case 3:		//�F�q���̃Z���ꍇ
		no = no /5-1;
		if(MG_RLT->hit_hLine[no] >= 4) return 0;	//�������܂��Ă���
		
		return oddsTbl[MG_RLT->hit_hLine[no]+1];
	case 4:		//��ޓq���̃Z���̏ꍇ
		no = no-1;
		if(MG_RLT->hit_vLine[no] >= 3) return 0;
		return oddsTbl[MG_RLT->hit_vLine[no]+2];
		break;
	case 12:	//�P�̂ɓq�����ꍇ
		if(MG_RLT->rlt_hp & g_rltBet[no].flg) return 0;
		return oddsTbl[4];
	default:
		return 0;
	}
}

//----------------------------------
//		���[���b�g�Չ�]����
//----------------------------------
static void RouletteBGRotate(void)
{
	s32	bg_start_x,bg_start_y;

	*(vu16*)REG_BG2PA = MG_RLT->bg_sPa;
	*(vu16*)REG_BG2PB = MG_RLT->bg_sPb;
	*(vu16*)REG_BG2PC = MG_RLT->bg_sPc;
	*(vu16*)REG_BG2PD = MG_RLT->bg_sPd;

	//BG��]�̎Q�ƊJ�n�_
	bg_start_x = 
		( CENTER_X << 8 )-( MG_RLT->bg_sPa * (CENTER_X+ActOX) )-( MG_RLT->bg_sPb * (CENTER_Y+BG2_DISP_OFSY+ActOY) );
	bg_start_y = 
		( CENTER_Y << 8 )-( MG_RLT->bg_sPc * (CENTER_X+ActOX) )-( MG_RLT->bg_sPd * (CENTER_Y+BG2_DISP_OFSY+ActOY) );

    *(vu16 *)REG_BG2X_L = (u16)(bg_start_x & 0xffff);
    *(vu16 *)REG_BG2X_H = (u16)(u32)( ( bg_start_x & 0x0fff0000 ) >> 16 );
    *(vu16 *)REG_BG2Y_L = (u16)(bg_start_y & 0xffff);
    *(vu16 *)REG_BG2Y_H = (u16)(u32)( ( bg_start_y & 0x0fff0000 ) >> 16 );
	return;
}

//--------------------------------------------------------------
//                     a*b �����߂�
//--------------------------------------------------------------
s16 fix_mul( s16 a, s16 b )
{
    s32 tmp;
    tmp = a;
    tmp *= b;
    tmp /= 0x100;
    return (s16)tmp;
}

//--------------------------------------------------------------
//                     1/b �����߂�
//--------------------------------------------------------------
s16 fix_inverse( s16 b )
{
    s32 tmp;
    tmp = 0x10000;
    tmp /= b;
    return (s16)tmp;
}


//========================================================================================
//										�b����������
//========================================================================================
//////////////////////////////////////////////////////
//	�R�C��������Ă���Ƃ��ɁA�v���C���邩���Ȃ���
//////////////////////////////////////////////////////
/*-----------------------------------------------------------
	����Ƀv���C����A���Ȃ���I������^�X�N�Ƃ��ċ@�\����

	SubTask
	static void TS_FirstPlaySelectMain(u8 id);

	depend	
		menutool.c
		itemtool.c
---------------------------------------------------------------*/
static void TS_FirstPlaySelectMain( u8 id )
{
	//�\���ʒu���w�肵�āAYesOrNo�̃��j���[box��\������
	YesNoSelectInit( YESNO_WINX, YESNO_WINY );	//menutool.c

	//TS_PlayYes �܂��� TS_PlayNo �ɃT�u�^�X�N�`�F���W
	SelectToolInit( id, &sg_yes_no_1st );
}

//////////////////////////////////////////////
// YES
static void TS_SetRoulettePlay( u8 id )
{
	//�t�F�[�h�A�E�g���I���܂ő҂�
	if( !(FadeData.fade_sw) ){
		SetVBlankFunc(NULL);	// �����݊֐��ݒ�(VBLANK)
		MainProcChange( TM_RouletteInit );
		DelTask(id);
	}
}
/*----------------------------------------------
	�v���C�����I�񂾂Ƃ��̏����i��ԏ��߁j
	SelectToolInit(id,&sg_yes_no); ���

	SubTask
	static void TS_PlayYes_1st(u8 id)

	Depend
		menutool.c
		itemtool.c
------------------------------------------------*/
static void TS_PlayYes_1st( u8 id )
{
	//YesNo���b�Z�[�WBox������
	NMenuBoxClear( YESNO_WINX, YESNO_WINY, YESNO_WINX+6, YESNO_WINY+5 );
	NMenuScreenClear();
	
	//�t�F�[�h�A�E�g���N�G�X�g
	PaletteFadeReq(0xFFFFFFFF,0,0,16,0x0000);
	FadeData.wait = FadeData.wait_def;
	PaletteAnime();

	//�^�X�N�`�F���W
	TaskTable[id].TaskAdrs = TS_SetRoulettePlay;
	return;
}

//////////////////////////////////////////////
// NO
/*--------------------------------------------
	�v���C���I�������N�G�X�g����

	SubTask
	static void TS_PlayNo_1st(u8 id);
----------------------------------------------*/
static void TS_PlayNo_1st( u8 id )
{
#ifdef PM_DEBUG	
	//�f�o�b�O�p�t���O����
	g_rlt_debug_sw = 0;
#endif	//PM_DEBUG

	//�����C�x���g���������āA�t�B�[���h�ɖ߂�
	NMenuScreenClear();		//�X�N���[�����b�Z�[�W�N���A
	ResetForceEvent();
	DelTask(id);
}

//////////////////////////////////////////////////////
//�R�C�����Ȃ��ăv���C�ł��Ȃ��Ƃ�
//////////////////////////////////////////////////////
static void TS_FirstPlayNoCoinEnd(u8 id)
{
	TaskTable[id].work[0]++;

	//60sync�o�߂��邩�A�{�^���������ꂽ�烁�b�Z�[�W�������ďI���
	if( ((sys.Trg & (A_BUTTON | B_BUTTON)) != 0) || (TaskTable[id].work[0] > 60))
	{
		ScriptParameter0 = 1;
		NMenuScreenClear();
		ResetForceEvent();
		DelTask(id);
	}
}

//=====================================
//		�v���C����H	����
//=====================================

/*---------------------------------------------------
	�{�[�i�X�^�C�����ǂ�����\������

	TS_FirstPlayIfBonus(u8 id);
-------------------------------------------------------*/
static void TS_FirstPlayIfBonus(u8 id)
{
	u8	rate;
	
	rate = g_rltRate[ScriptParameter0 >> 7][ScriptParameter0 & RLT_CHECK_RATE];	
	PM_NumMsgSet(StrTempBuffer0,rate,NUM_MODE_ZERO,1);
	PM_MsgExpand(MsgExpandBuffer,MSG_START);
	SetMsgWrite( id,MsgExpandBuffer, TS_FirstPlaySelectMain);
}
/*---------------------------------------------------
	�����v���C���ɗV�Ԃ��V�΂Ȃ����̃��b�Z�[�W��\��

	SubTask	
		staitc void TS_FirstPlaySelectInit(u8 id);

	Depend
		itemtool.c
------------------------------------------------------*/
static void TS_FirstPlaySelectInit( u8 id )
{
	u8	rate;
	
	//���b�Z�[�W�\�����I�����悱�̃T�u�^�X�N�̃A�h���X��ύX����
	//���̑�̃��[�g�́H�H�ł��A�����т܂����H

	//�������������Ă���R�C���̖�����\��
	//���l��������ϊ�(�o�b�t�@�A���l�A�ϊ����[�h�A�\������)
	PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[COIN_WORK], NUM_MODE_SPACE, 4 );	//pm_str.c
	//I_MSG_���o�b�t�@�ɓW�J
	PM_MsgExpand( MsgExpandBuffer, msg_str_bet );
	//��`�̈�ɃE�B���h�E�`��
	NMenuWinBoxWrite( 0, 0, 9, 3 );	//menutool.c
	//�w��ʒu�Ƀ��b�Z�[�W�`��
	NMenuMsgWrite( MsgExpandBuffer, 2, 1 );

	//��̃o�[�W�����ɂ���āA�q�������̒P�ʂ����܂�
	//���[�g�ɉ����āA�R�C��������邩����Ȃ����H
	rate = g_rltRate[ScriptParameter0 >> 7][ScriptParameter0 & RLT_CHECK_RATE];	
	
	PM_NumMsgSet(StrTempBuffer0,rate,NUM_MODE_ZERO,1);
	if(TaskTable[id].work[COIN_WORK] >= rate)		//�R�C���������̂ŗV�ׂ܂�
	{
		if((ScriptParameter0 & RLT_IF_BONUS) && (ScriptParameter0 & RLT_CHECK_RATE)){
			MsgWrite(MSG_BONUS);
			SetWaitNextTask( id,TS_FirstPlayIfBonus,0xFFFF,A_BUTTON|B_BUTTON);
		}
		else{
			PM_MsgExpand(MsgExpandBuffer,MSG_START);
			SetMsgWrite( id,MsgExpandBuffer, TS_FirstPlaySelectMain);
		}
	}
	else{	//���[�g�ɑ΂��āA�R�C��������܂���
		PM_MsgExpand(MsgExpandBuffer,MSG_NOCOIN);
		SetMsgWrite( id,MsgExpandBuffer, TS_FirstPlayNoCoinEnd);
		TaskTable[id].work[COIN_WORK] = 0;
		TaskTable[id].work[0] = 0;
	}
}

//////////////////////////////////////////////////
//�f�o�b�O�p�ɃR�C���̖�����I�΂���
#ifdef PM_DEBUG
static const u8 msg_selectcoin[] = {
	KO_,I_,N_,no_,spc_,ka_,zu_,spc_,S__,T__,A__,R__,T__,de_,spc_,ki_,me_,te_,CR_,
	A__,bou_,p__,n1_,spc_,B__,bou_,m__,n1_,spc_,R__,colon_,L__,bou_,batu_,n1_,n0_,EOM_,
};
extern u8 g_stackFlg;
static void TS_DmyRltCoinSet(u8 id)
{
	u8	msg_coin[] = {I_MSG_,STR_TEMP_BUF0,EOM_};
	
	if(sys.Trg & A_BUTTON)	
	{
		TaskTable[id].work[COIN_WORK]++;
		if(TaskTable[id].work[COIN_WORK] == 10000) TaskTable[id].work[COIN_WORK] = 0;

		//���l��������ϊ�(�o�b�t�@�A���l�A�ϊ����[�h�A�\������)
		PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[COIN_WORK], NUM_MODE_SPACE, 4 );	//pm_str.c
		//I_MSG_���o�b�t�@�ɓW�J
		PM_MsgExpand( MsgExpandBuffer, msg_coin );
		
		//�w��ʒu�Ƀ��b�Z�[�W�`��
		NMenuMsgWrite( MsgExpandBuffer, 2, 1 );
		return;
	}
	if(sys.Trg & B_BUTTON)
	{
		TaskTable[id].work[COIN_WORK]--;
		if(TaskTable[id].work[COIN_WORK] == -1) TaskTable[id].work[COIN_WORK] = MY_COIN_MAX;
		
		//���l��������ϊ�(�o�b�t�@�A���l�A�ϊ����[�h�A�\������)
		PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[COIN_WORK], NUM_MODE_SPACE, 4 );	//pm_str.c
		//I_MSG_���o�b�t�@�ɓW�J
		PM_MsgExpand( MsgExpandBuffer, msg_coin );
		
		//�w��ʒu�Ƀ��b�Z�[�W�`��
		NMenuMsgWrite( MsgExpandBuffer, 2, 1 );	
		return;
	}
	if(sys.Trg & R_BUTTON)
	{
		TaskTable[id].work[COIN_WORK] += 10;
		if(TaskTable[id].work[COIN_WORK]  >= 10000) TaskTable[id].work[COIN_WORK] -= MY_COIN_MAX;
		
		//���l��������ϊ�(�o�b�t�@�A���l�A�ϊ����[�h�A�\������)
		PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[COIN_WORK], NUM_MODE_SPACE, 4 );	//pm_str.c
		//I_MSG_���o�b�t�@�ɓW�J
		PM_MsgExpand( MsgExpandBuffer, msg_coin );
	
		//�w��ʒu�Ƀ��b�Z�[�W�`��
		NMenuMsgWrite( MsgExpandBuffer, 2, 1 );	
		return;
	}
	if(sys.Trg & L_BUTTON)
	{
		TaskTable[id].work[COIN_WORK] -= 10;
		if(TaskTable[id].work[COIN_WORK] <= -1) TaskTable[id].work[COIN_WORK] += MY_COIN_MAX;
		
		//���l��������ϊ�(�o�b�t�@�A���l�A�ϊ����[�h�A�\������)
		PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[COIN_WORK], NUM_MODE_SPACE, 4 );	//pm_str.c
		//I_MSG_���o�b�t�@�ɓW�J
		PM_MsgExpand( MsgExpandBuffer, msg_coin );
		
		//�w��ʒu�Ƀ��b�Z�[�W�`��
		NMenuMsgWrite( MsgExpandBuffer, 2, 1 );	
		return;
	}
	
	//����
	if(sys.Trg & START_BUTTON)
	{
		Fld.my_coin = TaskTable[id].work[COIN_WORK];
		TaskTable[id].TaskAdrs = TS_FirstPlaySelectInit;
		
		//���l��������ϊ�(�o�b�t�@�A���l�A�ϊ����[�h�A�\������)
		PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[COIN_WORK], NUM_MODE_SPACE, 4 );	//pm_str.c
		//I_MSG_���o�b�t�@�ɓW�J
		PM_MsgExpand( MsgExpandBuffer, msg_coin );
		
		//�w��ʒu�Ƀ��b�Z�[�W�`��
		NMenuMsgWrite( MsgExpandBuffer, 2, 1 );	

		g_stackFlg = 0;	
		return;
	}
	if(sys.Trg & SELECT_BUTTON)
	{
		Fld.my_coin = TaskTable[id].work[COIN_WORK];
		TaskTable[id].TaskAdrs = TS_FirstPlaySelectInit;
		
		//���l��������ϊ�(�o�b�t�@�A���l�A�ϊ����[�h�A�\������)
		PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[COIN_WORK], NUM_MODE_SPACE, 4 );	//pm_str.c
		//I_MSG_���o�b�t�@�ɓW�J
		PM_MsgExpand( MsgExpandBuffer, msg_coin );
		
		//�w��ʒu�Ƀ��b�Z�[�W�`��
		NMenuMsgWrite( MsgExpandBuffer, 2, 1 );	

		g_stackFlg = 1;	
		return;
	}	
}

static void TS_DmyRltCoinSetInit(u8 id)
{
	TaskTable[id].work[COIN_WORK] = Fld.my_coin;
	
	//1/2�̊m���Ń{�[�i�X
	if(pp_rand()&0x0001) ScriptParameter0 |= RLT_IF_BONUS;

	//���l��������ϊ�(�o�b�t�@�A���l�A�ϊ����[�h�A�\������)
	PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[COIN_WORK], NUM_MODE_SPACE, 4 );	//pm_str.c
	//I_MSG_���o�b�t�@�ɓW�J
	PM_MsgExpand( MsgExpandBuffer, msg_str_bet );
	//��`�̈�ɃE�B���h�E�`��
	NMenuWinBoxWrite( 0, 0, 9, 3 );	//menutool.c
	//�w��ʒu�Ƀ��b�Z�[�W�`��
	NMenuMsgWrite( MsgExpandBuffer, 2, 1 );

	SetMsgWrite(id,msg_selectcoin,TS_DmyRltCoinSet);
	return;
}
#endif	//ifdef PM_DEBUG

void RouletteSet(void)
{
	u8	id;

	SetForceEvent();

#ifdef PM_DEBUG
	g_stackFlg = 0;
	if(g_rlt_debug_sw){
		//�_�~�[���[�`��-�g�p����R�C���̖�����I�΂���
		id = AddTask(TS_DmyRltCoinSetInit,0);
		return;
	}
#endif	//PM_DEBUG	

	//��f�o�b�O��
	id = AddTask(TS_FirstPlaySelectInit,0);
	TaskTable[id].work[COIN_WORK] = Fld.my_coin;		
}
