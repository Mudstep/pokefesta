//====================================================================
//
//							�f�o�b�N	
//
//====================================================================

#ifdef PM_DEBUG

#include "common.h"
#include "intr.h"
#include "menu.h"					//menutool.h
#include "script.h"					//EventFlagReset
#include "tv_def.h"					//TV��define�Q
#include "tv_tool.h"				//TVDataSort
#include "tv.h"						//TVDataAllInit
#include "cm.h"						//CMDATA_MAX
#include "fan.h"					//FanAllInit
#include "sysflag.h"				//SYS_BATCH01_GET
#include "syswork.h"				//EV_FAN_WORK
#include "mus_tool.h"				//SePlay
#include "poketool.h"				//PokeParaGet
#include "..\script\savework.h"		//WK_SCENE_C101_R0201
#include "..\script\saveflag.h"		//
#include "record.h"					//CNT_TOWER_RENSHOU
#include "scr_tool.h"				//GetTemotiBtlPokeNum


//====================================================================
//							extern�錾
//====================================================================
extern void TVSecretIDSet(TV_Sec_Shop_Work *);
extern void TVLetterIDSet(TV_Letter_Work *);
extern u8	(*MenuFunc)();


//====================================================================
//						�v���g�^�C�v�錾
//====================================================================
//d_menu.c����Ă΂��
u8 DebugNoharaTest();
void NoharaTestMain();
static u8	DebugNoharaMenu();				//�f�o�b�N���j���[�I��҂�

//�s�u
static u8	DebugTVInit(void);				//���j���[�\��
static u8	DebugTVMenu(void);				//�I��҂�
void DebugTVTypeChange();					//�閧�^��������Ԃɂ���
static u8	DebugTVWatchFlagOff();			//������Ԃɂ���
static u8	DebugTVWatchFlagOn();			//���ĂȂ���Ԃɂ���
static void DebugTVTypeChangeInit();		//
static u8	DebugTVTypeChangeMain();		//
static void DebugTVCMTypeWin();				//�s�u�A�b�l�̃f�[�^�^�C�v��\������
static void DebugTVCMWatchWin();			//�s�u�A�b�l��������Ԃ���\������
static u8   DebugTVFlagOn();				//�s�u���X�^�[�g
static void DebugTVDataMakeInit();			//�s�u�f�[�^���쐬����(�ʐM�e�X�g�p)
static u8   DebugTVDataMakeMain();			//
static void DebugTVDataMake(u8 no, u8 type);//
static u8	DebugTVCMDataAllClear();		//�s�u�A�b�l��S�ăN���A����
static void DebugCMDataMakeInit();			//�b�l�f�[�^���쐬����(�ʐM�e�X�g�p)
static u8   DebugCMDataMakeMain();			//
static void DebugCMDataMake(u8 no, u8 type);//
static u8   DebugCasinoTable();				//�J�W�m�������擾
static u8   DebugTVCMDataMax();				//�s�u�A�b�l�f�[�^�������ς��ɂ���

//�t�@��
static u8	DebugFanInit();					//���j���[�\��
static u8	DebugFanMenu();					//�I��҂�
static u8   DebugFanFlagOn();				//�t�@�����X�^�[�g
static u8   DebugFanBitOn();				//�t�@���𑝂₷
static u8   DebugFanBitOff();				//�t�@�������炷
static u8	DebugFanBitOnMain();			//�L�[�҂�
static u8	DebugFanPoint();				//�t�@���|�C���g�\��
static u8	DebugFanTimeAdd();				//�v���C���Ԃ𑫂�
static u8	DebugFanPointShiten();			//�l�V���������
static u8	DebugFanPointBase();			//�閧��n�ΐ������
static u8	DebugFanPointContest();			//�R���e�X�g������
static u8	DebugFanPointTower();			//�o�g���^���[������

//���̑�
//static u8	DebugBatchFlagChange();			//�o�b�`���擾���Ă��Ȃ���Ԃɂ���
static u8   DebugSceneChangePapaBattle();	//��������Ɛ킦���Ԃɂ���
static u8	DebugSceneChangePapaBattleAfter();	//��������Ɛ퓬��̏�Ԃɂ���
static u8	DebugSceneChangeRune();			//���l�̃h�A���J����(���[�_�[���o��)
static u8	DebugSceneChangeFune();			//�D�ɏ����Ԃɂ���
static u8	DebugKazanbaiAdd();				//�ΎR�D�𑝂₷(9999)
static u8   DebugDensetuOpen();				//�`���̓��A�J����
static u8   DebugTowerRenshouAdd();			//�A�����{�T�O


//====================================================================
//							���b�Z�[�W
//====================================================================
static const u8 Nohara_TV_Msg[] = { TE_,RE_,BI_,EOM_ };
static const u8 Nohara_Fan_Msg[] = { HU_,AA_,N_,EOM_ };
//static const u8 Nohara_Menu1_Msg[] = { BA_,TTU_,TI_,spc_,O_,HU_,EOM_ };
static const u8 Nohara_Menu2_Msg[] = { PA_,PA_,spc_,BA_,TO_,RU_,O_,N_,EOM_ };
static const u8 Nohara_Menu25_Msg[] = { PA_,PA_,spc_,BA_,TO_,RU_,spc_,go_,EOM_ };
static const u8 Nohara_Menu20_Msg[] = { RU_,NE_,EOM_ };
static const u8 Nohara_Menu21_Msg[] = { HU_,NE_,spc_,no_,re_,ru_,EOM_ };
static const u8 Nohara_Menu22_Msg[] = { ha_,i_,spc_,n9_,n9_,n9_,n9_,EOM_ };
static const u8 Nohara_Menu23_Msg[] = { de_,n_,se_,tu_,HU_,RA_,GU_,O_,N_,EOM_ };
static const u8 Nohara_Menu24_Msg[] = { re_,n_,si_,yyo_,u_,ta_,su_,EOM_ };

static const MENU_ITEM DebugNoharaMenuTbl[] =
{
	{ Nohara_TV_Msg,	DebugTVInit},
	{ Nohara_Fan_Msg,	DebugFanInit},
	//{ Nohara_Menu1_Msg, DebugBatchFlagChange},
	{ Nohara_Menu2_Msg, DebugSceneChangePapaBattle},
	{ Nohara_Menu25_Msg, DebugSceneChangePapaBattleAfter},
	{ Nohara_Menu20_Msg, DebugSceneChangeRune},
	{ Nohara_Menu21_Msg, DebugSceneChangeFune},
	{ Nohara_Menu22_Msg, DebugKazanbaiAdd},
	{ Nohara_Menu23_Msg, DebugDensetuOpen},
	{ Nohara_Menu24_Msg, DebugTowerRenshouAdd},
};
#define D_NOHARA_MENU_MAX	NELEMS(DebugNoharaMenuTbl)	//���j���[�ő吔

//====================================================================
//					�f�o�b�N���j���[����Ă΂��	
//====================================================================
void NoharaTestMain(void)
{
	//sys.pMainProc = InitBoxAzukeru;			
};

u8 DebugNoharaTest(void)
{
	NMenuScreenClear();
	NMenuWinBoxWrite(0, 0, 11, 19);
	NMenuFixListWrite(2, 1, D_NOHARA_MENU_MAX, DebugNoharaMenuTbl);
	NMenuVCursorInit(1, 1, D_NOHARA_MENU_MAX, 0);
	MenuFunc = DebugNoharaMenu;
	return 0;
}

//--------------------------------------------------------------------
//					�f�o�b�N���j���[�I��҂�
//--------------------------------------------------------------------
static u8 DebugNoharaMenu(void)
{
	s8 num;

	num = NMenuVCursorControl();
	switch(num){
	case CURSOR_DEFAULT_VALUE:
		return 0;
	case CURSOR_CANCEL_VALUE:
		MenuEnd();
		return 1;
	default:
		break;
	}

	MenuFunc = DebugNoharaMenuTbl[num].ptr;
	return 0;
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


/********************************************************************/
//							�s�u���
/********************************************************************/
static u8 test_datatype;			//�f�o�b�N�łs�u�f�[�^�I��
static u8 test_no;					//�f�o�b�N�łs�u�f�[�^�v�f���I��
static u8 test_chg;					//�\���ؑ�

static const u8 debug_tv_msg01[]={hi_,mi_,tu_,ga_,ta_,wo_,spc_,su_,gu_,mi_,re_,ru_,yo_,u_,ni_,spc_,spc_,spc_,spc_,hi_,da_,ri_,KI_,bou_,de_,CR_,DE_,bou_,TA_,TA_,I_,PU_,wo_,spc_,he_,n_,ko_,u_,si_,ma_,su_,ka_,hate_,spc_,spc_,ki_,ri_,ka_,e_,EOM_};
static const u8 debug_tv_msg02[]={sa_,ku_,se_,i_,si_,ta_,i_,spc_,TE_,RE_,BI_,DE_,bou_,TA_,wo_,spc_,se_,n_,ta_,ku_,CR_,si_,te_,ku_,da_,sa_,i_,spc_,tu_,u_,si_,n_,TE_,SU_,TO_,yo_,u_,de_,su_,EOM_};
static const u8 debug_tv_msg03[]={ho_,u_,so_,u_,no_,spc_,ha_,ttu_,se_,i_,ri_,tu_,wo_,spc_,SE_,TTU_,TO_,CR_,si_,te_,ku_,da_,sa_,i_,spc_,hate_,bu_,n_,no_,n1_,ni_,spc_,na_,ri_,ma_,su_,EOM_};

static const u8 debug_tv_data01[]={A_,N_,KE_,bou_,TO_,EOM_};
static const u8 debug_tv_data02[]={sa_,i_,ki_,n_,EOM_};
static const u8 debug_tv_data03[]={KU_,RA_,BU_,EOM_};
static const u8 debug_tv_data04[]={ZI_,MU_,EOM_};
static const u8 debug_tv_data05[]={ta_,i_,ri_,yyo_,u_,EOM_};
static const u8 debug_tv_data06[]={se_,i_,me_,i_,EOM_};
static const u8 debug_tv_data07[]={KO_,N_,TE_,SU_,TO_,EOM_};
static const u8 debug_tv_data08[]={si_,yyo_,u_,ka_,i_,EOM_};
static const u8 debug_tv_data09[]={ka_,i_,mo_,no_,EOM_};
static const u8 debug_tv_data10[]={si_,ttu_,pa_,i_,EOM_};
static const u8 debug_tv_data11[]={tu_,ri_,EOM_};
static const u8 debug_tv_data12[]={i_,ti_,ni_,ti_,EOM_};
static const u8 debug_tv_data13[]={TA_,WA_,bou_,EOM_};

//�s�u�f�[�^
static const u8 test_datatype_buf[] = {		//�f�o�b�N�łs�u�ʐM�^�C�v�I��
			TV_POKEMON_TYPE1,
			TV_RECENTLY_TYPE1,
			TV_CLUB_TYPE1,
			//TV_GYMLEADER_TYPE1,
			TV_GENERATING_TYPE1,
			TV_NAME_TYPE1,
			TV_CONTEST_TYPE1,
			TV_GETPOKE_TYPE1,
			TV_SHOP_TYPE1,
			TV_FAILED_TYPE1,
			TV_FISHING_TYPE1,
			TV_ONEDAY_TYPE1,
			TV_TOWER_TYPE1,
};
#define D_DATATYPE_MAX	NELEMS(test_datatype_buf)

static const u8 * const debug_tv_msg_buf[] = {
			debug_tv_data01,
			debug_tv_data02,
			debug_tv_data03,
			//debug_tv_data04,
			debug_tv_data05,
			debug_tv_data06,
			debug_tv_data07,
			debug_tv_data08,
			debug_tv_data09,
			debug_tv_data10,
			debug_tv_data11,
			debug_tv_data12,
			debug_tv_data13,
};

//�b�l�f�[�^
static const u8 debug_cm_data01[]={ya_,su_,u_,ri_,EOM_};
static const u8 debug_cm_data02[]={KA_,ZI_,NO_,EOM_};
static const u8 debug_cm_data03[]={ho_,ri_,da_,si_,EOM_};

static const u8 test_cm_datatype_buf[] = {
			CM_BARGAIN_TYPE,
			CM_CASINO_TYPE,
			CM_FIND_ITEM_TYPE,
};
#define D_CM_DATATYPE_MAX	NELEMS(test_cm_datatype_buf)

static const u8 * const debug_cm_msg_buf[] = {
			debug_cm_data01,
			debug_cm_data02,
			debug_cm_data03,
};

//���j���[
static const u8 Nohara_Menu0_Msg[] = { hi_,mi_,tu_,ga_,ta_,EOM_ };
static const u8 Nohara_Menu3_Msg[] = { SU_,TA_,bou_,TO_,EOM_ };
static const u8 Nohara_Menu4_Msg[] = { T__,V__,sa_,ku_,se_,i_,EOM_ };
static const u8 Nohara_TV_Menu3_Msg[] = { O_,bou_,RU_,KU_,RI_,A_,EOM_ };
static const u8 Nohara_TV_Menu5_Msg[] = { su_,be_,te_,mi_,ta_,EOM_ };
static const u8 Nohara_CM_Menu1_Msg[] = { C__,M__,sa_,ku_,se_,i_,EOM_ };
static const u8 Nohara_TV_Menu6_Msg[] = { su_,be_,te_,mi_,te_,na_,i_,EOM_ };
static const u8 Nohara_TV_Menu7_Msg[] = { a_,ta_,ri_,TE_,bou_,BU_,RU_,EOM_ };
static const u8 Nohara_TV_Menu8_Msg[] = { i_,ttu_,pa_,i_,ni_,su_,ru_,EOM_ };

static const MENU_ITEM DebugTVMenuTbl[] =
{
	{ Nohara_Menu0_Msg, DebugTVTypeChangeInit},
	{ Nohara_Menu3_Msg, DebugTVFlagOn},
	{ Nohara_Menu4_Msg, DebugTVDataMakeInit},
	{ Nohara_TV_Menu3_Msg, DebugTVCMDataAllClear},
	{ Nohara_TV_Menu5_Msg, DebugTVWatchFlagOff},
	{ Nohara_CM_Menu1_Msg, DebugCMDataMakeInit},
	{ Nohara_TV_Menu6_Msg, DebugTVWatchFlagOn},
	{ Nohara_TV_Menu7_Msg, DebugCasinoTable},
	{ Nohara_TV_Menu8_Msg, DebugTVCMDataMax},
};
#define D_TV_MENU_MAX	NELEMS(DebugTVMenuTbl)

//--------------------------------------------------------------------
//							���j���[�\��
//--------------------------------------------------------------------
u8 DebugTVInit(void)
{
	test_chg = 0;
	NMenuScreenClear();
	NMenuWinBoxWrite(0, 0, 10, 19);
	NMenuFixListWrite(2, 1, D_TV_MENU_MAX, DebugTVMenuTbl);
	NMenuVCursorInit(1, 1, D_TV_MENU_MAX, 0);
	MenuFunc = DebugTVMenu;
	return 0;
}

//--------------------------------------------------------------------
//						�f�o�b�N���j���[�I��҂�
//--------------------------------------------------------------------
static u8 DebugTVMenu(void)
{
	s8 num;

	num = NMenuVCursorControl();
	switch(num){
	case CURSOR_DEFAULT_VALUE:
		return 0;
	case CURSOR_CANCEL_VALUE:
		MenuEnd();
		return 1;
	default:
		break;
	}

	MenuFunc = DebugTVMenuTbl[num].ptr;
	return 0;
}

//====================================================================
//							�f�o�b�N�s�u
//
//						datatype�̕\��
//						�閧�^�������悤�ɕύX����
//====================================================================
static void DebugTVTypeChangeInit()
{
	//�s�u�A�b�l�̃^�C�v�\��
	DebugTVCMTypeWin();

	NMenuWinMsgWrite(debug_tv_msg01);
	YesNoSelectInit(3,3);

	MenuFunc = DebugTVTypeChangeMain;
}

//--------------------------------------------------------------------
//						�閧�^��������Ԃɂ���
//--------------------------------------------------------------------
void DebugTVTypeChange()
{
	u8 i;

	for(i=0; i < TVDATA_MAX ;i++)
	{
		if( SEC_TYPE_START <= Fld.tv_event[i].datatype &&
				Fld.tv_event[i].datatype < SEC_TYPE_MAX )
		//if( Fld.tv_event[i].datatype != TV_NODATA_TYPE1 )
		{
			if( Fld.tv_event[i].watch_flag == TV_WATCH_FLAG_OFF )
				Fld.tv_event[i].watch_flag = TV_WATCH_FLAG_ON;
		}
	}
}

//--------------------------------------------------------------------
//					�s�u�f�[�^��S�Č�����Ԃɂ���
//--------------------------------------------------------------------
u8 DebugTVWatchFlagOff()
{
	u8 i;

	for(i=0; i < TVDATA_MAX ;i++)
	{
		Fld.tv_event[i].watch_flag = TV_WATCH_FLAG_OFF;
	}

	MenuEnd();
	return 1;
}

//--------------------------------------------------------------------
//					�s�u�f�[�^��S�Č��ĂȂ���Ԃɂ���
//--------------------------------------------------------------------
u8 DebugTVWatchFlagOn()
{
	u8 i;

	for(i=0; i < TVDATA_MAX ;i++)
	{
		if( Fld.tv_event[i].datatype != TV_NODATA_TYPE1 )
			Fld.tv_event[i].watch_flag = TV_WATCH_FLAG_ON;
	}

	MenuEnd();
	return 1;
}

const u8 atari_table_pos[12][12] ={
	{12,4},
	{12,8},
	{12,10},
	{14,6},
	{14,8},
	{14,10},
	{16,4},
	{16,6},
	{16,10},
	{18,4},
	{18,6},
	{18,10},
};

//--------------------------------------------------------------------
//					�X���b�g�̓������e�[�u���\��
//--------------------------------------------------------------------
u8 DebugCasinoTable()
{
	u8 i;

	for( i=0; i < 12 ;i++ )
	{
		ScriptParameter0 = i;
		PM_NumMsgSet( StrTempBuffer0, ScrSlotAtariGet(), NUM_MODE_LEFT, 1);
		NMenuMsgWrite( (const u8 *)StrTempBuffer0 , atari_table_pos[i][0], atari_table_pos[i][1]);
	}

	ScriptParameter0 = 0;		//�N���A
	MenuFunc = DebugFanBitOnMain;
	return 0;
}

//--------------------------------------------------------------------
//						�f�o�b�N�s�u�̃��C��
//--------------------------------------------------------------------
static u8 DebugTVTypeChangeMain()
{
	//�Ƃ肠���������ɑ}��
	//�\���ؑ�
	if( sys.Trg & L_KEY )
	{
		NMenuBoxClear( 10, 0, 29, 13 );
		test_chg ^= 1; 
		if( test_chg )	DebugTVCMWatchWin();	//datatype
		else	DebugTVCMTypeWin();			//watch_flag

		return 0;
	}

	switch( YesNoSelectMain() )
	{
	case CURSOR_DEFAULT_VALUE:
		return 0;
	case CURSOR_NO_VALUE:
	case CURSOR_CANCEL_VALUE:
		break;
	case CURSOR_YES_VALUE:
		DebugTVTypeChange();
		break;
	};

	MenuEnd();
	return 1;
}

//--------------------------------------------------------------------
//						�s�u�A�b�l�f�[�^��\������
//--------------------------------------------------------------------
static void DebugTVCMTypeWin()
{
	u8 i;

	//TV�f�[�^
	for( i=0; i < TVDATA_HIMSELF_MAX ;i++ )
	{
		PM_NumMsgSet( StrTempBuffer0, (int)Fld.tv_event[i].datatype,
				NUM_MODE_LEFT, 2);
		NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+(i*2), 0);
	}

	for( i=TVDATA_HIMSELF_MAX; i < TVDATA_MAX ;i++ )
	{
		PM_NumMsgSet( StrTempBuffer0, (int)Fld.tv_event[i].datatype,
				NUM_MODE_LEFT, 2);
		if( i < 15 )
			NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+((i-5)*2), 3);
		else
			NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+((i-15)*2), 6);
	}

	//CM�f�[�^
	for( i=0; i < CMDATA_MAX ;i++ )
	{
		PM_NumMsgSet( StrTempBuffer0, (int)Fld.tv_cm[i].datatype,
				NUM_MODE_LEFT, 2);
		if( i < 8 )
			NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+(i*2), 9);
		else
			NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+((i-8)*2), 12);
	}

	return;
}

//--------------------------------------------------------------------
//					�s�u�A�b�l��������Ԃ���\������
//--------------------------------------------------------------------
static void DebugTVCMWatchWin()
{
	u8 i;

	//TV�f�[�^
	for( i=0; i < TVDATA_HIMSELF_MAX ;i++ )
	{
		PM_NumMsgSet( StrTempBuffer0, (int)Fld.tv_event[i].watch_flag,
				NUM_MODE_LEFT, 2);
		NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+(i*2), 0);
	}

	for( i=TVDATA_HIMSELF_MAX; i < TVDATA_MAX ;i++ )
	{
		PM_NumMsgSet( StrTempBuffer0, (int)Fld.tv_event[i].watch_flag,
				NUM_MODE_LEFT, 2);
		if( i < 15 )
			NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+((i-5)*2), 3);
		else
			NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+((i-15)*2), 6);
	}

	//CM�f�[�^
	for( i=0; i < CMDATA_MAX ;i++ )
	{
		PM_NumMsgSet( StrTempBuffer0, (int)Fld.tv_cm[i].watch_flag,
				NUM_MODE_LEFT, 2);
		if( i < 8 )
			NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+(i*2), 9);
		else
			NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 10+((i-8)*2), 12);
	}

	return;
}


//====================================================================
//						TV�������悤�ɂ���
//====================================================================
static u8 DebugTVFlagOn()
{
	EventFlagSet( SYS_TV_START );
	EventFlagSet( SYS_C103_ARRIVE );
	MenuEnd();
	return 1;
}

//====================================================================
//							�f�o�b�N�s�u
//
//				datatype��I���TV�f�[�^���쐬����
//				ID�A���O�̂�ROM���甽�f����
//				�c��̃f�[�^�͂P�����Ă���
//====================================================================
static void DebugTVDataMakeInit()
{
	test_datatype = 0;
	NMenuWinMsgWrite(debug_tv_msg02);

	NMenuBoxWrite( 13,6,22,8 );
	NMenuMsgWrite(debug_tv_msg_buf[0], 14, 7);
	NMenuBoxWrite( 22,1,24,2 );
	PM_NumMsgSet( StrTempBuffer0, 0, NUM_MODE_LEFT, 2);
	NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 23, 1);

	MenuFunc = DebugTVDataMakeMain;
}

//--------------------------------------------------------------------
//					�f�o�b�N�s�u�f�[�^�쐬�̃��C��
//--------------------------------------------------------------------
static u8 DebugTVDataMakeMain()
{
	u8 change = 0;

	//�v�f����ύX
	if( sys.Trg & U_KEY ){
		test_no++;
		if( test_no == TVDATA_MAX ) test_no = 0;
		//m4aSongNumStart(SE_SELECT);
		SePlay(SE_SELECT);
		change = 1;
	};

	if( sys.Trg & D_KEY ){
		if( test_no == 0 ) test_no = TVDATA_MAX;
		test_no--;
		//m4aSongNumStart(SE_SELECT);
		SePlay(SE_SELECT);
		change = 1;
	};

	//�f�[�^�^�C�v��ύX
	if( sys.Trg & R_KEY ){
		test_datatype++;
		if( test_datatype == D_DATATYPE_MAX ) test_datatype = 0;
		//m4aSongNumStart(SE_SELECT);
		SePlay(SE_SELECT);
		change = 1;
	};

	if( sys.Trg & L_KEY ){
		if( test_datatype == 0 ) test_datatype = D_DATATYPE_MAX;
		test_datatype--;
		//m4aSongNumStart(SE_SELECT);
		SePlay(SE_SELECT);
		change = 1;
	};

	//������ύX
	if(change){
		NMenuBoxWrite( 13,6,22,8 );
		NMenuMsgWrite(debug_tv_msg_buf[test_datatype], 14, 7);

		NMenuBoxWrite( 22,1,24,2 );
		PM_NumMsgSet( StrTempBuffer0, (int)test_no, NUM_MODE_LEFT, 2);
		NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 23, 1);
	}

	//�쐬
	if( sys.Trg & A_BUTTON ){
		//m4aSongNumStart(SE_PIN);
		SePlay(SE_PIN);
		DebugTVDataMake(test_no,test_datatype_buf[test_datatype]);
	};

	//�I��
	if( sys.Trg & START_BUTTON || sys.Trg & B_BUTTON ){
		TVDataSort(Fld.tv_event);
		MenuEnd();
		return 1;
	}

	return 0;
}

//--------------------------------------------------------------------
//						�f�o�b�N�s�u�f�[�^�쐬
//--------------------------------------------------------------------
static void DebugTVDataMake(u8 no, u8 type)
{
	u8 i,ans,ret;

	ret = GetTemotiBtlPokeNum();

	Fld.tv_event[no].datatype = type;
	Fld.tv_event[no].watch_flag = TV_WATCH_FLAG_ON;

	for( i=0; i < 34 ;i++ )	
		Fld.tv_event[no].dmy[i] = 1;	

	ans = TVDataCopyTypeGet( type );

	//ID�Z�b�g
	switch( ans )
	{
		case CUT_TYPE1:
		case CUT_TIME_TYPE:
			TVLetterIDSet((TV_Letter_Work *)&Fld.tv_event[no]);
			break;
		case CUT_TYPE2:
			TVSecretIDSet((TV_Sec_Shop_Work *)&Fld.tv_event[no]);
			break;
	}

	//���O�ƃj�b�N�l�[���ƃ|�P�����i���o�[�ȂǃZ�b�g
	switch( type )
	{
		case TV_POKEMON_TYPE1:
		case TV_RECENTLY_TYPE1:
			 ((TV_Letter_Work*)(&(Fld.tv_event[no])))->inside_data = 1;
			PM_strcpy( ((TV_Letter_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			break;
		case TV_CLUB_TYPE1:
			((TV_Club_Work*)(&(Fld.tv_event[no])))->pokeno = 1;
			PM_strcpy( ((TV_Club_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			PokeParaGet( &PokeParaMine[ret], ID_nickname, 
					((TV_Club_Work*)(&(Fld.tv_event[no])))->nickname );
			break;
		case TV_GYMLEADER_TYPE1:
			break;
		case TV_NAME_TYPE1:
			((TV_Name_Work*)(&(Fld.tv_event[no])))->pokeno = 1;
			((TV_Name_Work*)(&(Fld.tv_event[no])))->r_pokeno = 1;
			PM_strcpy( ((TV_Name_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			PokeParaGet( &PokeParaMine[ret], ID_nickname, 
					((TV_Name_Work*)(&(Fld.tv_event[no])))->nickname );
			break;
		case TV_CONTEST_TYPE1:
			((TV_Con_Work*)(&(Fld.tv_event[no])))->pokeno = 1;
			PM_strcpy( ((TV_Con_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			PokeParaGet( &PokeParaMine[ret], ID_nickname, 
					((TV_Con_Work*)(&(Fld.tv_event[no])))->nickname );
			break;
		case TV_TOWER_TYPE1:
			((TV_Tower_Work*)(&(Fld.tv_event[no])))->pokeno = 1;
			((TV_Tower_Work*)(&(Fld.tv_event[no])))->enemy_pokeno = 1;
			PM_strcpy( ((TV_Tower_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			PM_strcpy( ((TV_Tower_Work*)(&(Fld.tv_event[no])))->enemy_name, MyData.my_name );
			break;
		case TV_GENERATING_TYPE1:
			((TV_Generating_Work*)(&(Fld.tv_event[no])))->pokeno = 1;
			((TV_Generating_Work*)(&(Fld.tv_event[no])))->time_count = GENERATING_INCUBATION_TIME;
			break;
		case TV_GETPOKE_TYPE1:
			((TV_Sec_Btl_Work*)(&(Fld.tv_event[no])))->get_pokeno = 1;
			PM_strcpy( ((TV_Sec_Btl_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			PokeParaGet( &PokeParaMine[ret], ID_nickname, 
					((TV_Sec_Btl_Work*)(&(Fld.tv_event[no])))->nickname );
			break;
		case TV_SHOP_TYPE1:
			PM_strcpy( ((TV_Sec_Shop_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			break;
		case TV_FAILED_TYPE1:
			((TV_Sec_Get_Failed_Work*)(&(Fld.tv_event[no])))->pokeno = 1;
			((TV_Sec_Get_Failed_Work*)(&(Fld.tv_event[no])))->failed_pokeno = 1;
			PM_strcpy( ((TV_Sec_Get_Failed_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			break;
		case TV_FISHING_TYPE1:
			((TV_Sec_Fishing_Work*)(&(Fld.tv_event[no])))->pokeno = 1;
			PM_strcpy( ((TV_Sec_Fishing_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			break;
		case TV_ONEDAY_TYPE1:
			((TV_Sec_OneDay_Work*)(&(Fld.tv_event[no])))->get_pokeno = 1;
			((TV_Sec_OneDay_Work*)(&(Fld.tv_event[no])))->pokeno = 1;
			PM_strcpy( ((TV_Sec_OneDay_Work*)(&(Fld.tv_event[no])))->player_name, MyData.my_name );
			break;
	}

	return;
}

//====================================================================
//						TVCM�f�[�^��S�č폜����
//====================================================================
static u8 DebugTVCMDataAllClear()
{
	TVDataAllInit();
	MenuEnd();
	return 1;
}

//====================================================================
//							�f�o�b�N�b�l
//
//				datatype��I���CM�f�[�^���쐬����
//====================================================================
static void DebugCMDataMakeInit()
{
	test_datatype = 0;
	NMenuWinMsgWrite(debug_tv_msg02);

	NMenuBoxWrite( 13,6,22,8 );
	NMenuMsgWrite(debug_cm_msg_buf[0], 14, 7);
	NMenuBoxWrite( 22,1,24,2 );
	PM_NumMsgSet( StrTempBuffer0, 0, NUM_MODE_LEFT, 2);
	NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 23, 1);

	MenuFunc = DebugCMDataMakeMain;
}

//--------------------------------------------------------------------
//					�f�o�b�N�b�l�f�[�^�쐬�̃��C��
//--------------------------------------------------------------------
static u8 DebugCMDataMakeMain()
{
	u8 change = 0;

	//�v�f����ύX
	if( sys.Trg & U_KEY ){
		test_no++;
		if( test_no == CMDATA_MAX ) test_no = 0;
		SePlay(SE_SELECT);
		change = 1;
	};

	if( sys.Trg & D_KEY ){
		if( test_no == 0 ) test_no = CMDATA_MAX;
		test_no--;
		SePlay(SE_SELECT);
		change = 1;
	};

	//�f�[�^�^�C�v��ύX
	if( sys.Trg & R_KEY ){
		test_datatype++;
		if( test_datatype == D_CM_DATATYPE_MAX ) test_datatype = 0;
		SePlay(SE_SELECT);
		change = 1;
	};

	if( sys.Trg & L_KEY ){
		if( test_datatype == 0 ) test_datatype = D_CM_DATATYPE_MAX;
		test_datatype--;
		SePlay(SE_SELECT);
		change = 1;
	};

	//������ύX
	if(change){
		NMenuBoxWrite( 13,6,22,8 );
		NMenuMsgWrite(debug_cm_msg_buf[test_datatype], 14, 7);

		NMenuBoxWrite( 22,1,24,2 );
		PM_NumMsgSet( StrTempBuffer0, (int)test_no, NUM_MODE_LEFT, 2);
		NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 23, 1);
	}

	//�쐬
	if( sys.Trg & A_BUTTON ){
		SePlay(SE_PIN);
		DebugCMDataMake(test_no,test_cm_datatype_buf[test_datatype]);
	};

	//�I��
	if( sys.Trg & START_BUTTON || sys.Trg & B_BUTTON ){
		CMDataSort();
		MenuEnd();
		return 1;
	}

	return 0;
}

//--------------------------------------------------------------------
//						�f�o�b�N�b�l�f�[�^�쐬
//--------------------------------------------------------------------
static void DebugCMDataMake(u8 no, u8 type)
{
	Fld.tv_cm[no].datatype   = type;
	Fld.tv_cm[no].watch_flag = TV_WATCH_FLAG_ON;
	Fld.tv_cm[no].time_count = (CM_TIME_COUNT * TODAY_TIME);
	return;
}

//--------------------------------------------------------------------
//					�f�o�b�N�s�u�E�b�l�f�[�^�}�b�N�X
//--------------------------------------------------------------------
static u8 DebugTVCMDataMax()
{
	u8 i,j;

	for( j=0,i=0; i < TVDATA_MAX ;i++ )
	{
		if( Fld.tv_event[i].datatype == TV_NODATA_TYPE1 )
		{
			if( j == D_DATATYPE_MAX ) j = 0;
			DebugTVDataMake(i,test_datatype_buf[j]);
			Fld.tv_event[i].watch_flag = TV_WATCH_FLAG_OFF;
			j++;
		}
	}

	for( j=0,i=0; i < CMDATA_MAX ;i++ )
	{
		if( Fld.tv_cm[i].datatype == CM_NODATA_TYPE )
		{
			if( j == D_CM_DATATYPE_MAX ) j = 0;
			DebugCMDataMake(i,test_cm_datatype_buf[j]);
			j++;
		}
	}

	MenuEnd();
	return 1;
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


/********************************************************************/
//							�t�@�����
/********************************************************************/
//�t�@���̖��O
static const u8 debug_fan_name01[]={n1_,spc_,SU_,KU_,RU_,EOM_};
static const u8 debug_fan_name02[]={n2_,spc_,MI_,DO_,RU_,EOM_};
static const u8 debug_fan_name03[]={n3_,spc_,O_,ZI_,YO_,EOM_};
static const u8 debug_fan_name04[]={n4_,spc_,BO_,bou_,YA_,EOM_};
static const u8 debug_fan_name05[]={n5_,spc_,BO_,bou_,I_,EOM_};
static const u8 debug_fan_name06[]={n6_,spc_,YA_,N_,GU_,EOM_};
static const u8 debug_fan_name07[]={n7_,spc_,WO_,bou_,KA_,EOM_};
static const u8 debug_fan_name08[]={n8_,spc_,O_,RU_,DO_,EOM_};

static const u8 * const debug_fan_msg_buf[] = {
			debug_fan_name01,
			debug_fan_name02,
			debug_fan_name03,
			debug_fan_name04,
			debug_fan_name05,
			debug_fan_name06,
			debug_fan_name07,
			debug_fan_name08,
};

//���j���[
static const u8 Nohara_Menu5_Msg[] = { SU_,TA_,bou_,TO_,EOM_ };
static const u8 Nohara_Menu6_Msg[] = { hu_,ya_,su_,EOM_ };
static const u8 Nohara_Menu7_Msg[] = { he_,ra_,su_,EOM_ };
static const u8 Nohara_Menu8_Msg[] = { PO_,I_,N_,TO_,EOM_ };
static const u8 Nohara_Menu9_Msg[] = { PU_,RE_,I_,ZI_,KA_,N_,spc_,n6_,EOM_ };
static const u8 Nohara_Menu10_Msg[] = { P__,si_,te_,n_,no_,u_,EOM_ };
static const u8 Nohara_Menu11_Msg[] = { P__,hi_,mi_,tu_,ki_,ti_,EOM_ };
static const u8 Nohara_Menu12_Msg[] = { P__,KO_,N_,TE_,SU_,TO_,EOM_ };
static const u8 Nohara_Menu13_Msg[] = { P__,BA_,TO_,RU_,TA_,WA_,bou_,EOM_ };

static const MENU_ITEM DebugFanMenuTbl[] =
{
	{ Nohara_Menu5_Msg, DebugFanFlagOn},
	{ Nohara_Menu6_Msg, DebugFanBitOn},
	{ Nohara_Menu7_Msg, DebugFanBitOff},
	{ Nohara_Menu8_Msg, DebugFanPoint},
	{ Nohara_Menu9_Msg, DebugFanTimeAdd},
	{ Nohara_Menu10_Msg, DebugFanPointShiten},
	{ Nohara_Menu11_Msg, DebugFanPointBase},
	{ Nohara_Menu12_Msg, DebugFanPointContest},
	{ Nohara_Menu13_Msg, DebugFanPointTower},
};
#define D_FAN_MENU_MAX	NELEMS(DebugFanMenuTbl)

//--------------------------------------------------------------------
//							���j���[�\��
//--------------------------------------------------------------------
u8 DebugFanInit(void)
{
	NMenuScreenClear();
	NMenuWinBoxWrite(0, 0, 10, 19);
	NMenuFixListWrite(2, 1, D_FAN_MENU_MAX, DebugFanMenuTbl);
	NMenuVCursorInit(1, 1, D_FAN_MENU_MAX, 0);
	MenuFunc = DebugFanMenu;
	return 0;
}

//--------------------------------------------------------------------
//						�f�o�b�N���j���[�I��҂�
//--------------------------------------------------------------------
static u8 DebugFanMenu(void)
{
	s8 num;

	num = NMenuVCursorControl();
	switch(num){
	case CURSOR_DEFAULT_VALUE:
		return 0;
	case CURSOR_CANCEL_VALUE:
		MenuEnd();
		return 1;
	default:
		break;
	}

	MenuFunc = DebugFanMenuTbl[num].ptr;
	return 0;
}

//====================================================================
//							�t�@�����X�^�[�g
//====================================================================
static u8 DebugFanFlagOn()
{
	FanAllInit();
	FanStart();
	MenuEnd();
	return 1;
}

//====================================================================
//						�t�@��	�ʐM�ΐ폟��
//					�t�@�����X�^�[�g���ĂȂ��Ă��o����
//====================================================================
static u8 DebugFanBitOn()
{
	u8 ans;

	ans = FanBitOn();
	NMenuMsgWrite(debug_fan_msg_buf[ (fan_obj_add[ans] - FAN_BIT )], 14, 7);
	MenuFunc = DebugFanBitOnMain;
	return 0;
}

//====================================================================
//						�t�@��	�ʐM�ΐ�s�k
//					�t�@�����X�^�[�g���ĂȂ��Ă��o����
//====================================================================
static u8 DebugFanBitOff()
{
	u8 ans;

	ans = FanBitOff();
	NMenuMsgWrite(debug_fan_msg_buf[ (fan_obj_sub[ans] - FAN_BIT )], 14, 7);
	MenuFunc = DebugFanBitOnMain;
	return 0;
}

//====================================================================
//					�������A�������t�@���̖��O��\��
//====================================================================
static u8 DebugFanBitOnMain()
{
	if( sys.Trg & A_BUTTON ){			//�I��
		MenuEnd();
		return 1;
	};

	return 0;
}

//====================================================================
//						�t�@��	�|�C���g�\��
//====================================================================
static u8 DebugFanPoint()
{
	PM_NumMsgSet( StrTempBuffer0, (int)FAN_POINT, NUM_MODE_LEFT, 2);
	NMenuMsgWrite( (const u8 *)StrTempBuffer0 , 16, 7);
	MenuFunc = DebugFanBitOnMain;
	return 0;
}

//====================================================================
//						�t�@��	�v���C���ԍX�V�{�U
//====================================================================
static u8 DebugFanTimeAdd()
{
	MyData.playtime_h+=6;
	MenuEnd();
	return 1;
}

//====================================================================
//						�t�@���|�C���g	�l�V����
//====================================================================
static u8 DebugFanPointShiten()
{
	FanPointCalc( FAN_SHITEN_TYPE );
	MenuEnd();
	return 1;
}

//====================================================================
//						�t�@���|�C���g	�閧��n
//====================================================================
static u8 DebugFanPointBase()
{
	FanPointCalc( FAN_BASE_TYPE );
	MenuEnd();
	return 1;
}

//====================================================================
//						�t�@���|�C���g	�R���e�X�g
//====================================================================
static u8 DebugFanPointContest()
{
	FanPointCalc( FAN_CONTEST_TYPE );
	MenuEnd();
	return 1;
}

//====================================================================
//						�t�@���|�C���g	�o�g���^���[
//====================================================================
static u8 DebugFanPointTower()
{
	FanPointCalc( FAN_BATTLE_TOWER_TYPE );
	MenuEnd();
	return 1;
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


/********************************************************************/
//							���̑�
/********************************************************************/

/*
//====================================================================
//				���ׂẴo�b�`���擾���Ă��Ȃ���Ԃɂ���
//====================================================================
static u8 DebugBatchFlagChange()
{
	EventFlagReset( SYS_BATCH01_GET );
	EventFlagReset( SYS_BATCH02_GET );
	EventFlagReset( SYS_BATCH03_GET );
	EventFlagReset( SYS_BATCH04_GET );
	EventFlagReset( SYS_BATCH05_GET );
	EventFlagReset( SYS_BATCH06_GET );
	EventFlagReset( SYS_BATCH07_GET );
	EventFlagReset( SYS_BATCH08_GET );
	MenuEnd();
	return 1;
}
*/

//====================================================================
//					��������Ɛ킦���Ԃɂ���
//====================================================================
static u8 DebugSceneChangePapaBattle()
{
	//�o�b�`���S�����Ă���
	SetEventWorkValue( WK_SCENE_C101_R0201, 6 );
	MenuEnd();
	return 1;
}

//====================================================================
//					��������Ɛ������̏�Ԃɂ���
//====================================================================
static u8 DebugSceneChangePapaBattleAfter()
{
	SetEventWorkValue( WK_SCENE_C101_R0201, 7 );
	MenuEnd();
	return 1;
}

//====================================================================
//						���l�̃h�A���J����(���[�_�[���o��)
//====================================================================
static u8 DebugSceneChangeRune()
{
	//���̃h�A���J����
	EventFlagSet( FE_POKE1_01_CAVE_D1111 );
	EventFlagSet( FE_POKE1_01_CAVE_D1206 );

	//���[�_�[���o��
	EventFlagReset(FV_LEADER08_01_C108_R0101);

	MenuEnd();
	return 1;
}

//====================================================================
//						�D�ɏ����Ԃɂ���
//====================================================================
static u8 DebugSceneChangeFune()
{
	EventFlagReset( FV_HAGI_01_R104_R0101 );
	SetEventWorkValue( WK_SCENE_R104_R0101, 1 );
	MenuEnd();
	return 1;
}

//====================================================================
//						�ΎR�D�𑝂₷(9999����)
//====================================================================
static u8 DebugKazanbaiAdd()
{
	SetEventWorkValue( WK_ASH_GATHER_COUNT, 9999 );
	MenuEnd();
	return 1;
}

//====================================================================
//						�`���̓��A���J����
//====================================================================
static u8 DebugDensetuOpen()
{
	EventFlagSet( FE_DENSETU_OPEN );
	MenuEnd();
	return 1;
}

//====================================================================
//					�o�g���^���[�A�������T�O�����₷
//====================================================================
static u8 DebugTowerRenshouAdd()
{
//	u32 ret;
#if 0
	ret = GetRecord( CNT_TOWER_RENSHOU );
	ret+=50;
	SetRecord( CNT_TOWER_RENSHOU, ret );
	MenuEnd();
#endif

	if( MyData.BattleTowerWork.BattleTowerKachinuki < 50 )
		MyData.BattleTowerWork.BattleTowerKachinuki = 50;	//50�����Z�b�g
	else if( MyData.BattleTowerWork.BattleTowerKachinuki < 100 )
		MyData.BattleTowerWork.BattleTowerKachinuki = 100;
	else if( MyData.BattleTowerWork.BattleTowerKachinuki < 1000 )
		MyData.BattleTowerWork.BattleTowerKachinuki = 1000;
	else if( MyData.BattleTowerWork.BattleTowerKachinuki < 5000 )
		MyData.BattleTowerWork.BattleTowerKachinuki = 9990;
	else if( MyData.BattleTowerWork.BattleTowerKachinuki < 9990 )
		MyData.BattleTowerWork.BattleTowerKachinuki = 9999;

	MenuEnd();
	return 1;
}

#endif

