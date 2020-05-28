//=========================================================================
//
//		�o�g����ʕ��A�v���O����
//														by matsuda
//=========================================================================


#include "common.h"
#include "bss.h"		//add
#include "ef_tool.h"
#include "calctool.h"
#include "poketool.h"		//add
#include "decord.h"		//add
#include "objdata.h"		//add
#include "fight.h"		//add

#include "client2.h"
#include "client_t.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "cli_def.h"
#include "madefine.h"
#include "gauge.h"
#include "a_mine.h"
#include "cli_ef.h"
#include "pokelist.h"
#include "print.h"
#include "waza_eff.h"
#include "task.h"
#include "b_return.h"
#include "server.h"

#include "intr.h"
#include "client.h"
#include "clientai.h"
#include "p_status.h"
#include "waza_eff.h"
#include "wazatool.h"
#include "laster.h"
#include "trbenum.h"


//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
//static void Add_and_Copy(u8 *backno, const actHeader *phead, actWork *BackUpActor);
static void BattleActorInit(u8);
static void BattleRegisterInit(void);
void BattleRecover(void);
static u8 BattleCellInit(u8);
//static void SetPushWork(u8 id, u8 actno);
//static void SetPopWork(u8 id, u8 actno);
static void BattleRecoverMain(void);
static void BattleGaugeActorSet(u8 client_no);

//=========================================================================
//	�O���֐�
//=========================================================================
extern void MainFight(void);

//=========================================================================
//	�O���ް�
//=========================================================================
extern const u8 CommandMsg[];
extern const u8 PokeMinePosX[2][2];
extern const u8 PokeEnemyPosX[2][2];
extern const u8 PokeMinePosY[2][2];
extern const u8 PokeEnemyPosY[2][2];

//=========================================================================
//	�O���[�o���ϐ�
//=========================================================================
extern	TALK_WIN	FightMsg;
u8 PushBattleID[CLIENT_MAX] = {};		//��ʕ��A�p���ް����ۑ����Ă��������ID
					//0:�����߹��1  1:�����߹��2  2:�G�߹��1�@3:�G�߹��2�@���b��


//=========================================================================
//	��ʕ��A�p�Ƀo�g�����[�N�ޔ�
//=========================================================================
void PushBattleWork(void)
{
}

//=========================================================================
//		�o�g����ʕ��A����
//
// ���֐����Ăԏ��Ԃɒ���!!  ����ClientNo���ǂ�������������Ă��邩
//
//=========================================================================
void BattleRecover(void)
{
	FadeData.trans_stop = 1;

	SetHBlankFunc(NULL);
	SetVBlankFunc(NULL);
	*(vu16*)REG_MOSAIC = 0;

	UserWork[0x20000 - 1] = 0;
	UserWork[0x20000 - 2] = 0;
	MainProcChange( BattleRecoverMain );
}

enum{
	BR_0,
	BR_0_1,
	BR_1,
	BR_2,
	BR_2_1,
	BR_3,
	BR_3_1,
	BR_4,
	BR_5,
	BR_6,
	BR_7,
	BR_8,
	BR_9,
	BR_10,
	BR_11,
	BR_12,
	BR_13,
	BR_14,
	BR_15,
	BR_16,
};

static void BattleRecoverMain(void)
{
	u8 client_no;
	
	switch(UserWork[0x20000 - 1]){
		case BR_0:
			LasterWorkInit();

		//	*(vu16 *)REG_DISPCNT = DISP_MODE_0 | DISP_OBJ_CHAR_1D_MAP;
		//	*(vu16 *)REG_DISPCNT = DISP_MODE_1 | DISP_OBJ_CHAR_1D_MAP;

			PrintScreenInit(FIGHT_TYPE);
			PaletteAnimeInit();
			MsgControlInit(&FightMsg,FIGHT_TYPE);	//change 2002.07.16 by soga
			ScrX0=0;
			ScrY0=0;
			ScrX1=0;
			ScrY1=0;
			ScrX2=0;
			ScrY2=0;
			ScrX3=0;
			ScrY3=0;
			break;
		
		case BR_0_1:
		//	DIV_DMACLEAR(3,0,VRAM,(VRAM_END-VRAM),32);
		//	CpuClear(0, VRAM, (VRAM_END-VRAM), 32);
			CpuFastClear(0, VRAM, VRAM_END-VRAM);
			break;
		case BR_1:
			if(FightBGSet(UserWork[0x20000-2]))
				UserWork[0x20000-2] = 0;
			else{
				UserWork[0x20000-2]++;
				UserWork[0x20000-1]--;
			}
			break;
			
		case BR_2:
			InitActor();
			break;
		case BR_2_1:
			ObjPalManInit();
			ObjPalManStart = 4;
			break;
			
		case BR_3:
			//add 2002.05.01(��) ���̂���ClientDivInit���g�p����悤�ɕύX���邱��!!
			//DivInit�ɕς����炻�̒���ClientSystemWorkInitAll��
			//��ʕ��A����ClientSystemWorkInit�ɂȂ�悤�ɑΉ����邱�ƁI
			ClientSystemWorkInit();
//			MsgControlInit(&FightMsg,FIGHT_TYPE);	//change 2001.07.12(��) by matsuda
			//MsgInit(&FightMsg);
			break;
		case BR_3_1:
			//BattleGaugeCellPalSet();
			if(BattleGaugeCellPalDivSet(UserWork[0x20000-2]))
				UserWork[0x20000-2] = 0;
			else{
				UserWork[0x20000-2]++;
				UserWork[0x20000-1]--;
			}
			break;
		case BR_4:
			if(BattleCellInit(0) == 0)
				UserWork[0x20000-1]--;
			break;
		case BR_5:
			if(BattleCellInit(1) == 0)
				UserWork[0x20000-1]--;
			break;
		case BR_6:
			if(BattleCellInit(2) == 0)
				UserWork[0x20000-1]--;
			break;
		case BR_7:
			if(BattleCellInit(3) == 0)
				UserWork[0x20000-1]--;
			break;
			
		case BR_8:
			//InitActWork();
			BattleActorInit(0);
			break;
		case BR_9:
			BattleActorInit(1);
			break;
		case BR_10:
			BattleActorInit(2);
			break;
		case BR_11:
			BattleActorInit(3);
			break;
		
		case BR_12:
			BattleGaugeActorSet(0);
			break;
		case BR_13:
			BattleGaugeActorSet(1);
			break;
		case BR_14:
			BattleGaugeActorSet(2);
			break;
		case BR_15:
			BattleGaugeActorSet(3);
			break;
		
		case BR_16:
		#if 0	//2vs2�̎��A1�̖ڂ�2�̖ڂŹް�ނ̐F��ς���K�v���Ȃ��Ȃ����̂�
			if(FightType & FIGHT_TYPE_2vs2)
			{
				col =ObjPalNumGet(ActWork[ClientGaugeSt[BSS_CLIENT_MINE2]].actHead->palNo);
				SoftFade(256 + (col << 4), 16, 7, 0);
				PaletteWorkSet(&PaletteWorkTrans[256 + (col << 4)], 256+(col<<4), 0x20);
			}
		#endif
			//�|�P�����̉e���
			PokeShadowAllDataSet();
			client_no = ClientNoGet(BSS_CLIENT_ENEMY);
			PokeShadowSeqSet(client_no, 
				PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_monsno));
			if(FightCheck()){
				client_no = ClientNoGet(BSS_CLIENT_ENEMY2);
				PokeShadowSeqSet(client_no, 
					PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_monsno));
			}
			
			CommandCsrOn(CommandCsrPos[TempClientNo],0);
//			MsgPutSet(&FightMsg, CommandMsg, CGX_START+0x10,  19, 35);
//			MsgPut(&FightMsg);
			break;
			
		default:
			SetHBlankFunc(fight_h_blank_intr);
			SetVBlankFunc(fight_v_blank_intr);

		//	*(vu16 *)REG_DISPCNT = DISP_MODE_0 | DISP_OBJ_CHAR_1D_MAP | DISP_BG0_ON
		//		| DISP_BG1_ON | DISP_BG2_ON | DISP_BG3_ON | DISP_OBJ_ON;
		//	*(vu16 *)REG_DISPCNT |= DISP_BG0_ON | DISP_BG1_ON | DISP_BG2_ON | DISP_OBJ_ON;

			BattleRegisterInit();
//			if((FightType&FIGHT_TYPE_TUUSHIN)==0)
				//PaletteFadeReq(0xffffffff, 0, 16, 0, 0);
				//LightFadeReq(B_to_N);
				HardFadeReq(0xff, 0, 16, 0, 1);
			FadeData.trans_stop = 0;

			//sys.pMainProc=MainFight;
			MainProcChange(MainFight);
	}
	UserWork[0x20000 - 1]++;
};

//----------------------------------------------------------
//	ڼ޽�������
//----------------------------------------------------------
static void BattleRegisterInit(void)
{
	FightREGSet();
	(*(vBgCnt *)REG_BG1CNT).CharBasep = 0;
	(*(vBgCnt *)REG_BG2CNT).CharBasep = 0;
}

//----------------------------------------------------------
//	Cell�ēo�^
//	�����]���ő���BUserWork�̒l�Œ��ŏ���ɂ�����̂ŁA��Ԃ܂��ɂ��Ƃ��̂�
//	�g�p����UserWork�̗̈��0�N���A�B�߂�l��1���߂�܂Ŗ�Sync�ĂԁB
//----------------------------------------------------------
static u8 BattleCellInit(u8 client_no)
{
	u8 ret;
	
	if(client_no >= ClientSetMax)
		return 1;
	
	if(MineEnemyCheck(client_no)){
		if(Climem[client_no].migawari == 0)
			PokemonFObjCellPalSet(&PokeParaEnemy[SelMonsNo[client_no]], client_no);
		else
			MigawariChrChange(client_no, 0);
		ret = 1;
	}else{
		if((FightType&FIGHT_TYPE_SAFARI)&&(client_no==0))
			TrainerBObjCellPalSet(MyData.my_sex, client_no);
		else if((FightType&FIGHT_TYPE_GET_DEMO)&&(client_no==0))
			TrainerBObjCellPalSet(CELLID_tr3b_019, client_no);
		else{
			if(Climem[client_no].migawari == 0)
				PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[client_no]], client_no);
			else
				MigawariChrChange(client_no, 0);
		}
		ret = 1;
	}
	
	if(ret){
		UserWork[0x20000-2] = 0;
		return 1;
	}else{
		UserWork[0x20000-2]++;
		return 0;
	}
}
//----------------------------------------------------------
//	�A�N�^�[�ēo�^
//�@�@(���߂�AddActor����̂�Vram�g�p�̈��Cell�֌W���̍Đݒ�����邽��)
//----------------------------------------------------------
static void BattleActorInit(u8 client_no)
{
	u8 y;
	
	if(client_no >= ClientSetMax)
		return;
	
	if(Climem[client_no].migawari)
		y = GetMigawariYPos(client_no);
	else
		y = GetPokemonYPos(client_no);

	if(MineEnemyCheck(client_no))
	{
		if(PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_hp) == 0)
			return;
		PokeActHeaderMake(
		PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_monsno), ClientTypeGet(client_no));

#if 0
		PokemonStruct[client_no] = AddActor(&PokeActHeader,
			PokeEnemyPosX[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(client_no) >> 1], PokeEnemyPosY[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(client_no) >> 1] +
//				4*(8-pm2fpara[
//				PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_monsno)].size),
				pm2fpara[PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_monsno)].patcnt,
				C_POKE1PRI);
#else
		PokemonStruct[client_no]=
			AddActor(&PokeActHeader,
					 GetWazaEffPos(client_no,PM_X),
					 y,
					 //C_POKE1PRI);
					 GetPokeSoftPri(client_no));
#endif
		ActWork[PokemonStruct[client_no]].oamData.Pltt = client_no;
		ActWork[PokemonStruct[client_no]].move = DummyActMove;
		ActWork[PokemonStruct[client_no]].work[0] = client_no;
		ActWork[PokemonStruct[client_no]].work[2] =
			PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_monsno);
		ActAnmChg(&ActWork[PokemonStruct[client_no]],BattlePokeAnmNo[client_no]);
	}
	else
	{
		if((FightType&FIGHT_TYPE_SAFARI)&&(client_no==0)){
//			PokeActHeaderMake(MyData.my_sex+500,ClientTypeGet(client_no));
			TrainerActHeaderMake(MyData.my_sex,ClientTypeGet(client_no));
			PokemonStruct[client_no] = AddActor(&PokeActHeader,
									C_TRN_X,
									C_TRN_Y+4*(8-trbpara[MyData.my_sex].size),
									//C_TRN_PRI);	//�ڰŰ
									GetPokeSoftPri(client_no));

			ActWork[PokemonStruct[client_no]].oamData.Pltt = client_no;
			ActWork[PokemonStruct[client_no]].move = DummyActMove;
			ActWork[PokemonStruct[client_no]].work[0] = client_no;
		}
		else if((FightType&FIGHT_TYPE_GET_DEMO)&&(client_no==0)){
//			PokeActHeaderMake(CELLID_tr3b_019+500,ClientTypeGet(client_no));
			TrainerActHeaderMake(CELLID_tr3b_019,ClientTypeGet(client_no));
			PokemonStruct[client_no] = AddActor(&PokeActHeader,
									C_TRN_X,
									C_TRN_Y+4*(8-trbpara[CELLID_tr3b_019].size),
									//C_TRN_PRI);	//�ڰŰ
									GetPokeSoftPri(client_no));

			ActWork[PokemonStruct[client_no]].oamData.Pltt = client_no;
			ActWork[PokemonStruct[client_no]].move = DummyActMove;
			ActWork[PokemonStruct[client_no]].work[0] = client_no;
		}
		else{
			if(PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_hp) == 0)
				return;
			PokeActHeaderMake(
			PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_monsno), ClientTypeGet(client_no));
#if 0
			PokemonStruct[client_no] = AddActor(&PokeActHeader,
					PokeMinePosX[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(client_no) >> 1], PokeMinePosY[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(client_no) >> 1] + 
//				4*(8-pm2bpara[
//				PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_monsno)].size),
					pm2bpara[PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_monsno)].patcnt,
						C_POKE1PRI);
#else
			PokemonStruct[client_no]=
				AddActor(&PokeActHeader,
						 GetWazaEffPos(client_no,PM_X),
						 y,
						 //C_POKE1PRI);
						 GetPokeSoftPri(client_no));
#endif
			ActWork[PokemonStruct[client_no]].oamData.Pltt = client_no;
			ActWork[PokemonStruct[client_no]].move = DummyActMove;
			ActWork[PokemonStruct[client_no]].work[0] = client_no;
			ActWork[PokemonStruct[client_no]].work[2] =
			PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_monsno);
			ActAnmChg(&ActWork[PokemonStruct[client_no]],BattlePokeAnmNo[client_no]);
		}
	}
	
	//��ʐ؂�ւ��O����Ư������Ă��߹�݂͉�ʕ��A���������p���������
	ActWork[PokemonStruct[client_no]].banish = Climem[client_no].act_banish;
}

static void BattleGaugeActorSet(u8 client_no)
{
	u8 gauge_actno;
	
	if(client_no >= ClientSetMax)
		return;

	if((FightType&FIGHT_TYPE_SAFARI)&&(client_no==0))
		gauge_actno = GaugeActorSetSafari();
	else if((FightType&FIGHT_TYPE_GET_DEMO)&&(client_no==0))
		return;
	else
		gauge_actno = GaugeActorSet(client_no);

	ClientGaugeSt[client_no] = gauge_actno;
	GaugeDefaultPosSet(client_no);
	GaugeON(gauge_actno);
	if(MineEnemyCheck(client_no))
		GaugeTransChrSet(ClientGaugeSt[client_no],
				&PokeParaEnemy[SelMonsNo[client_no]], G_ALL);
	else{
		if(FightType&FIGHT_TYPE_SAFARI)
			GaugeTransChrSet(ClientGaugeSt[client_no],
					&PokeParaMine[SelMonsNo[client_no]], G_SAFARI);
		else
			GaugeTransChrSet(ClientGaugeSt[client_no],
					&PokeParaMine[SelMonsNo[client_no]], G_ALL);
	}

	if(ClientTypeGet(client_no) == BSS_CLIENT_ENEMY2
				 || ClientTypeGet(client_no) == BSS_CLIENT_MINE2)	//���b��
		GaugePriSet(ClientGaugeSt[client_no], 1);
	else
		GaugePriSet(ClientGaugeSt[client_no], 0);

	if(MineEnemyCheck(client_no))
	{
		if(PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_hp) == 0)
			GaugeOFF(gauge_actno);
	}
	else
	{
		if((FightType&FIGHT_TYPE_SAFARI)==0){
			if(PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_hp) == 0)
				GaugeOFF(gauge_actno);
		}
	}
}



/*---------------------

//----------------------------------------------------------
//	AddActor�����Ă����ޯ������ް��ŏ㏑������
// ����: *backno = �ޯ����ߌ���actno
//       *phead = AddActor���鱸�ͯ�ް
//       *BackUpActor = �ޯ����߱����̐擪���ڽ
//----------------------------------------------------------
static void Add_and_Copy(u8 *backno, const actHeader *phead, actWork *BackUpActor)
{
	u8 actno;
	u16 offset_char_num = 0;
	u16 temp_CharNo;
	u8 temp_AffineParamNo;
	u8 temp_Pltt;
	
	if(BackUpActor[*backno].act_sw == 1)
	{
		actno = AddActor(phead, 0,0,0);
		
		//���I���ް���ޔ�
		if(BackUpActor[*backno].char_num > 0)
		{
			offset_char_num = 
				BackUpActor[*backno].oamData.CharNo - BackUpActor[*backno].char_num;
		}
		temp_CharNo = ActWork[actno].oamData.CharNo;
		temp_AffineParamNo = ActWork[actno].oamData.AffineParamNo;
		temp_Pltt = ActWork[actno].oamData.Pltt;
		ActAnm(&ActWork[actno]);	//����ް��������̂���
		memcpy(&ActWork[actno], &BackUpActor[*backno], sizeof(actWork));
		//���I���ް���Ă��Ȃ���
		if(offset_char_num > 0)
			ActWork[actno].char_num = temp_CharNo + offset_char_num;
		ActWork[actno].oamData.CharNo = temp_CharNo;
		ActWork[actno].oamData.AffineParamNo = temp_AffineParamNo;
		ActWork[actno].oamData.Pltt = temp_Pltt;
		
		*backno = actno;	//�ŐV��actno�ɍX�V
	}
}

-------------------------------*/

