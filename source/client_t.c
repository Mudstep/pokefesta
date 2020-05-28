
//======================================================
//    client_t.c                                           
//                                                      
//    Copyright (C) 2001 GAME FREAK inc.
//======================================================

#include "common.h"
#include "bss.h"		//add
#include "ef_tool.h"
#include "calctool.h"
#include "poketool.h"		//add
#include "decord.h"		//add
#include "objdata.h"		//add
#include "fight.h"		//add

#include "actor.h"
#include "actanm.h"
#include "madefine.h"
#include "cli_num.h"
#include "cli_def.h"
#include "cli_ef.h"
#include "gauge.h"
#include "intr.h"
#include "print.h"
#include "wazatool.h"
#include "client.h"
#include "server.h"
#include "b_pmlist.h"
#include "mus_tool.h"
#include "waza_eff.h"
#include "wazano.def"
#include "monsno.def"
#include "contest.h"


//=========================================================================
//	�O���ϐ�
//=========================================================================
extern int BGNo;
extern	TALK_WIN	FightMsg;

//=========================================================================
//	�O���֐�
//=========================================================================
extern void	MainFight(void);
extern void	fight_v_blank_intr(void);

//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
void ClientSystemWorkInit(void);
void ClientSystemWorkInitAll(void);
void PokeShadowSeq(actWork *xreg);
void PokeShadowBanishSeq(actWork *xreg);
void PokeShadowAllDataSet(void);

//=========================================================================
//	�f�[�^
//=========================================================================
//----------------------------------------------------------
//	��(�ݽ���ްق���o�Ă���Ƃ���)
//----------------------------------------------------------
const CellData MBSmokeCell =
{
	(void *)smokechr_pch_ADRS, OBJ_SIZE16_8x8 * 12, CELLID_MBSmoke
};
const PalData MBSmokePal = 
{
	(void *)smokechr_pcl_ADRS, CELLID_MBSmoke
};


//=========================================================================
//	�A�N�^�[�e�[�u��
//=========================================================================
//----------------------------------------------------------
//	��(�ݽ���ްق���o�Ă���Ƃ���)
//----------------------------------------------------------
const ActOamData MBSmokeOamData = 
{
	0,		//u32 VPos:8;             // �x���W
	0,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	0,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
	1,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	1,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};

static const actAnm mbsmoke_anm0[] =			//����
{//��׸���ް����ް  ����  Hflip  Vflip
	{OBJ16_8x8 * 0, MBSMOKE_ANMWAIT, 0, 0},
	{OBJ16_8x8 * 4, MBSMOKE_ANMWAIT, 0, 0},
	{OBJ16_8x8 * 8, MBSMOKE_ANMWAIT, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm mbsmoke_anm1[] =			//�E��
{//��׸���ް����ް  ����  Hflip  Vflip
	{OBJ16_8x8 * 0, MBSMOKE_ANMWAIT, 1, 0},
	{OBJ16_8x8 * 4, MBSMOKE_ANMWAIT, 1, 0},
	{OBJ16_8x8 * 8, MBSMOKE_ANMWAIT, 1, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm mbsmoke_anm2[] =			//����
{//��׸���ް����ް  ����  Hflip  Vflip
	{OBJ16_8x8 * 0, MBSMOKE_ANMWAIT, 0, 1},
	{OBJ16_8x8 * 4, MBSMOKE_ANMWAIT, 0, 1},
	{OBJ16_8x8 * 8, MBSMOKE_ANMWAIT, 0, 1},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm mbsmoke_anm3[] =			//�E��
{//��׸���ް����ް  ����  Hflip  Vflip
	{OBJ16_8x8 * 0, MBSMOKE_ANMWAIT, 1, 1},
	{OBJ16_8x8 * 4, MBSMOKE_ANMWAIT, 1, 1},
	{OBJ16_8x8 * 8, MBSMOKE_ANMWAIT, 1, 1},
	{ACT_ANMEND, 0, 0, 0},
};

static const actAnm * const MBSmokeAnmTbl[] = 
{
	mbsmoke_anm0,
	mbsmoke_anm1,
	mbsmoke_anm2,
	mbsmoke_anm3,
};

const actHeader MBSmokeHeader = 
{
	CELLID_MBSmoke,
	CELLID_MBSmoke,
	&MBSmokeOamData,
	MBSmokeAnmTbl,
	0,
	DummyActAffTbl,	//MBSmokeAffTbl,
	MBSmokeWait,
};

//----------------------------------------------------------
//	�ްق���o���肷��Ƃ����߹�݊g�k
//----------------------------------------------------------
/*const	actAffAnm * const BallPokeAffTbl[] = 
{
	BallOutAffAnm,
	BallInAffAnm,
};*/


//----------------------------------------------------------
//	�|�P�����̉e
//----------------------------------------------------------
const CellData PokeShadowCell={
	(void *)kage_pch_ADRS,OBJ_SIZE16_8x32*1,	CELLID_POKESHADOW,
};
const ActOamData PokeShadowOamData = 
{
	0,		//u32 VPos:8;             // �x���W
	0,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	1,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
	1,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	3,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
const actHeader PokeShadowHeader = 
{
	CELLID_POKESHADOW,
	CELLID_BattleGaugeMine1,	//����A��گĂ�HP�ް�ނƈꏏ
	&PokeShadowOamData,
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	PokeShadowBanishSeq,
};

//=========================================================================
//	�v���O����
//=========================================================================


//=========================================================================
//	�߹�� �Z���o�^&��گēo�^
// ClientNo�ɂ����BG��گ�0�`3�ɕ�����
//=========================================================================

//const u32 DecordAdrs[]={0x2008000,0x2010000,0x2018000,0x2020000};
//const u32 DecordAdrs[]={0x2008000,0x200c000,0x2010000,0x2014000};
//const u32 DecordAdrs[]={0x2008000,0x200b000,0x200e000,0x2011000};
const u32 DecordAdrs[]={0x2008000,0x200a000,0x200c000,0x200e000};

void PokemonFObjCellPalSet(PokemonParam *pp, u8 client_no)
{
	u8 client_type;
	u16 pokeno;
	u32 personal_rnd;
	u32 rnd;
	u16 obj_palpos;
	u32 poke_id;
	void *paladrs;
	
	personal_rnd = PokeParaGet(pp, ID_personal_rnd);
	if(Climem[client_no].hensin_monsno == 0){
		pokeno = PokeParaGet(pp, ID_monsno);
		rnd	   = personal_rnd;
	}
	else{
		pokeno = Climem[client_no].hensin_monsno;
		rnd	   = HenshinRnd[client_no];
	}
	poke_id = PokeParaGet(pp, ID_id_no);
	
	client_type = ClientTypeGet(client_no);	//add by matsuda 2001.09.26(��)
#if 1
	//�ւ񂵂�p�̌��������g��
	DecordPokeCellMakeBuchi(&PM2FObjDataCell[pokeno], pm2fpara[pokeno].size,
				pm2fpara[pokeno].patcnt, (void*)0x2000000,
				(void*)DecordAdrs[client_type], pokeno, rnd);
#else
	DecordPokeCellMakeBuchi(&PM2FObjDataCell[pokeno], pm2fpara[pokeno].size,
				pm2fpara[pokeno].patcnt, (void*)0x2000000,
				(void*)DecordAdrs[client_type], pokeno, personal_rnd);
#endif
//	DecordPalSet(&PM2NObjDataPal[pokeno]);
	obj_palpos = PA_OBJ0 + 0x10 * client_no;
	if(Climem[client_no].hensin_monsno == 0){
		paladrs = (void*)PokePalGet(pp);
	}else{
		paladrs = (void*)PokePalGetPara(pokeno,poke_id,personal_rnd);
	}
	
	DecordWram(paladrs, (void*)&UserWork[0]);
	PaletteWorkSet((void *)&UserWork[0], obj_palpos, 0x20);
	PaletteWorkSet((void *)&UserWork[0], PA_BG8 + 0x10 * client_no, 0x20);
	//���V�C�|�P�����͓���ȃZ�b�g������
	if(pokeno==MONSNO_POWARUN){
		obj_palpos = PA_OBJ0 + 0x10 * client_no;
//		DecordWram((void*)powawan_pcl_ADRS, (void*)&UserWork[0]);
		DecordWram(paladrs, (void*)&UserWork[POWARUN_PALETTE]);
		PaletteWorkSet((void *)&UserWork[POWARUN_PALETTE+0x20*BattlePokeAnmNo[client_no]],obj_palpos,0x20);
	}
	if(Climem[client_no].hensin_monsno != 0){
		SoftFade(obj_palpos, 16, HENSIN_EVY, 0x7fff);
		CpuCopy(&PaletteWorkTrans[obj_palpos], &PaletteWork[obj_palpos], 0x20, 32);
	}
}
void PokemonBObjCellPalSet(PokemonParam *pp, u8 client_no)
{
	u8 client_type;
	u16 pokeno;
	u32 personal_rnd;
	u32	rnd;
	u16 obj_palpos;
	u32 poke_id;
	void *paladrs;
	
	personal_rnd = PokeParaGet(pp, ID_personal_rnd);
	if(Climem[client_no].hensin_monsno == 0){
		pokeno = PokeParaGet(pp, ID_monsno);
		rnd	   = personal_rnd;
	}
	else{
		pokeno = Climem[client_no].hensin_monsno;
		rnd    = HenshinRnd[client_no];
	}
	poke_id = PokeParaGet(pp, ID_id_no);
	
	client_type = ClientTypeGet(client_no);	//add by matsuda 2001.09.26(��)
#if 1
	//�ւ񂵂�p�̌��������g��
	DecordPokeCellMakeBuchi(&PM2BObjDataCell[pokeno], pm2bpara[pokeno].size,
				pm2bpara[pokeno].patcnt, (void*)0x2000000,
				(void*)DecordAdrs[client_type], pokeno,rnd);
#else
	DecordPokeCellMakeBuchi(&PM2BObjDataCell[pokeno], pm2bpara[pokeno].size,
				pm2bpara[pokeno].patcnt, (void*)0x2000000,
				(void*)DecordAdrs[client_type], pokeno,personal_rnd);
#endif
//	DecordPalSet(&PM2NObjDataPal[pokeno]);
	//											��2�޲��ް��Ȃ̂�
	obj_palpos = PA_OBJ0 + 0x10 * client_no;
	if(Climem[client_no].hensin_monsno == 0){
		paladrs = (void*)PokePalGet(pp);
	}else{
		paladrs = (void*)PokePalGetPara(pokeno,poke_id,personal_rnd);
	}
	
	DecordWram(paladrs, (void*)&UserWork[0]);
	PaletteWorkSet((void *)&UserWork[0], obj_palpos, 0x20);
	PaletteWorkSet((void *)&UserWork[0], PA_BG8 + 0x10 * client_no, 0x20);
	//���V�C�|�P�����͓���ȃZ�b�g������
	if(pokeno==MONSNO_POWARUN){
		obj_palpos = PA_OBJ0 + 0x10 * client_no;
//		DecordWram((void*)powawan_pcl_ADRS, (void*)&UserWork[0]);
		DecordWram(paladrs, (void*)&UserWork[POWARUN_PALETTE]);
		PaletteWorkSet((void *)&UserWork[POWARUN_PALETTE+0x20*BattlePokeAnmNo[client_no]],obj_palpos,0x20);
	}
	if(Climem[client_no].hensin_monsno != 0){
		SoftFade(obj_palpos, 16, HENSIN_EVY, 0x7fff);
		CpuCopy(&PaletteWorkTrans[obj_palpos], &PaletteWork[obj_palpos], 0x20, 32);
	}
}

//=========================================================================
//	�߹�� �فA��گ�Del
//=========================================================================
void PokemonFObjCellPalDel(u16 pokeno)
{
	//�]���^������CellDel�K�v�Ȃ��APalSet�g���ĂȂ�����PalDel�K�v�Ȃ�
//	CellDel(PM2FObjDataCell[pokeno].act_num);
//	ObjPalRegNumDel(PM2NObjDataPal[pokeno].reg_num);
}
void PokemonBObjCellPalDel(u16 pokeno)
{
	//�]���^������CellDel�K�v�Ȃ��APalSet�g���ĂȂ�����PalDel�K�v�Ȃ�
//	CellDel(PM2BObjDataCell[pokeno].act_num);
//	ObjPalRegNumDel(PM2NObjDataPal[pokeno].reg_num);
}

//=========================================================================
//	�ڰŰ �Z���o�^&��گēo�^
//=========================================================================
void TrainerFObjCellPalSet(u16 trainer_no, u8 client_no)
{
	u8 client_type;
	CellData ArcCell;

	client_type = ClientTypeGet(client_no);	//add by matsuda 2001.09.26(��)
	DecordPokeCellMake((CellData*)&TRFObjDataCell[trainer_no],trfpara[trainer_no].size,
				trfpara[trainer_no].patcnt, (void*)0x2000000,(void*)DecordAdrs[client_type],0);
	ArcCell.ch_data=(void*)DecordAdrs[client_type];
	ArcCell.size=TRFObjDataCell[trainer_no].size;
	ArcCell.act_num=TRFObjDataCell[trainer_no].act_num;
	DecordCellSet((CellData*)&ArcCell);
//	DecordCellSet((CellData*)&TRFObjDataCell[trainer_no]);
	DecordPalSet((void*)&TRFObjDataPal[trainer_no]);
}

void TrainerBObjCellPalSet(u16 trainer_no, u8 client_no)
{
	u8 client_type;
	client_type = ClientTypeGet(client_no);	//add by matsuda 2001.09.26(��)
	DecordPokeCellMake((CellData*)&TRBObjDataCell[trainer_no],trbpara[trainer_no].size,
				trbpara[trainer_no].patcnt, (void*)0x2000000,(void*)DecordAdrs[client_type],0);
//	DecordPalSet((PalData*)&TRBObjDataPal[trainer_no]);
	DecordPaletteWork((void*)TRBObjDataPal[trainer_no].pal_data,
		PA_OBJ0 + 0x10 * client_no, 0x20);
}
//=========================================================================
//	�ڰŰ �فA��گ�DEL
//=========================================================================
void TrainerBObjCellPalDel(u16 trainer_no)
{
	//�]���^������CellDel�K�v�Ȃ��APalSet�g���ĂȂ�����PalDel�K�v�Ȃ�
	//ObjPalRegNumDel(TRBObjDataPal[trainer_no].reg_num);
}
void TrainerFObjCellPalDel(u16 trainer_no)
{
	//Cell,PalSet����悤�ɂȂ����̂ł�����Del����
	ObjPalRegNumDel(TRFObjDataPal[trainer_no].reg_num);
	CellDel(TRFObjDataCell[trainer_no].act_num);
}

//=========================================================================
//	�ް�ޖ{�̷�׾��
//=========================================================================
void BattleGaugeCellPalSet(void)
{
	u8 i, parts_max;
	
	ObjPalSet((PalData*)&BattleGaugeDataPal[0]);
	ObjPalSet((PalData*)&BattleGaugeDataPal[1]);
	if(FightCheck() == 0)
	{
	#if GAUGE_CHANGE
		DecordCellSet((void*)&BattleGaugeDataCell_1vs1);
	#else
		DecordCellSet((void*)&MineGaugeCell_1vs1);
		DecordCellSet((void*)&EnemyGaugeCell_1vs1);
	#endif
		parts_max = 2;
	}
	else
	{
	#if GAUGE_CHANGE
		DecordCellSet((void*)&BattleGaugeDataCellMine2[0]);
		DecordCellSet((void*)&BattleGaugeDataCellMine2[1]);
		DecordCellSet((void*)&BattleGaugeDataCellEnemy2[0]);
		DecordCellSet((void*)&BattleGaugeDataCellEnemy2[1]);
	#else
		DecordCellSet((void*)&MineGaugeCell_2vs2[0]);
		DecordCellSet((void*)&MineGaugeCell_2vs2[1]);
		DecordCellSet((void*)&EnemyGaugeCell_2vs2[0]);
		DecordCellSet((void*)&EnemyGaugeCell_2vs2[1]);
	#endif
		parts_max = 4;
	}
	for(i = 0; i < parts_max; i++)
		DecordCellSet((void*)&BattleGaugePartsCell[ClientType[i]]);
//	DecordCellSet((void*)&BattleGaugeCursorCell);
}

//----------------------------------------------------------
//	BattleGaugeCellPalSet�̕����o�[�W����
//	�߂�l��1���Ԃ��Ă���܂�trans_no��0���珇�X�ɑ傫�����������Ė�Sync�Ă�
//----------------------------------------------------------
u8 BattleGaugeCellPalDivSet(u8 trans_no)
{
	u8 ret = 0;
	
	if(trans_no == 0)
		;
	else if(trans_no == 1){
		ObjPalSet((PalData*)&BattleGaugeDataPal[0]);
		ObjPalSet((PalData*)&BattleGaugeDataPal[1]);
	}
	else{
		if(FightCheck() == 0)
		{
			if(trans_no == 2){
//changed by soga 2002.06.01
				if(FightType&FIGHT_TYPE_SAFARI)
					DecordCellSet((void*)&MineGaugeCell_Safari);
				else
					DecordCellSet((void*)&MineGaugeCell_1vs1);
			}
			else if(trans_no == 3)
				DecordCellSet((void*)&EnemyGaugeCell_1vs1);
			else if(trans_no == 4)
				DecordCellSet((void*)&BattleGaugePartsCell[ClientType[0]]);
			else if(trans_no == 5)
				DecordCellSet((void*)&BattleGaugePartsCell[ClientType[1]]);
			else{
				ret = 1;
			}
		}
		else
		{
			if(trans_no == 2)
				DecordCellSet((void*)&MineGaugeCell_2vs2[0]);
			else if(trans_no == 3)
				DecordCellSet((void*)&MineGaugeCell_2vs2[1]);
			else if(trans_no == 4)
				DecordCellSet((void*)&EnemyGaugeCell_2vs2[0]);
			else if(trans_no == 5)
				DecordCellSet((void*)&EnemyGaugeCell_2vs2[1]);
			else if(trans_no == 6)
				DecordCellSet((void*)&BattleGaugePartsCell[ClientType[0]]);
			else if(trans_no == 7)
				DecordCellSet((void*)&BattleGaugePartsCell[ClientType[1]]);
			else if(trans_no == 8)
				DecordCellSet((void*)&BattleGaugePartsCell[ClientType[2]]);
			else if(trans_no == 9)
				DecordCellSet((void*)&BattleGaugePartsCell[ClientType[3]]);
			else{
				ret = 1;
			}
		}
	}
	return ret;
}

//=========================================================================
//	�ް�ޓ]�����ܰ��т֓W�J(�퓬��ʗp)
// flg = 0:HP�ް��&����  1:EXP�ް��
//now!! 0:HP  1:����(LV)  2:����(HP)  3:EXP
//=========================================================================
void BattleGaugeTransWram(u8 flg)
{
	#if GAUGE_DEBUG == 0
	DecordWram((void*)bg_life1_pch_ADRS, (void*)GAUGE_DECORD_ADR);
	#elif 0
	switch(BGNo)
	{
		case 0:
		case 2:
			DecordWram((void*)bg_life1_pch_ADRS, (void*)GAUGE_DECORD_ADR);
			break;
		default:
			DecordWram((void*)bg_life2_pch_ADRS, (void*)GAUGE_DECORD_ADR);
	}
	#else
//	DecordWram((void*)bg_life2_pch_ADRS, (void*)GAUGE_DECORD_ADR);
	DecordWram((void*)btl_gaug_pch_ADRS, (void*)GAUGE_DECORD_ADR);
	#endif
}






//=========================================================================
//	ClientInit�̏����𕪊��]���p�Ɉ�̊֐��ɂ܂Ƃ߂�����
//	counter = ��Sync���Ƃɲݸ���Ă���Ă����܂��B�����Ŏg�p
//	counter2 = �����������Ŏg�p
//		�����ǂ���̶�����������0�N���A������Ԃœn���Ă�������
//
//	�߂�l: 0=�܂�Init��	1:Init�I��
//=========================================================================
enum{
	DIV_0_0_0 = 0,
	DIV_0_0,
	DIV_0_1,
	DIV_0,
	DIV_1,
	DIV_2,
	DIV_3,
	DIV_4,
};
u8 ClientDivInit(u8 *counter, u8 *counter2)
{
	u8 ret = 0;

	switch(*counter){
		case DIV_0_0_0:
			ClientSystemWorkInitAll();
			(*counter)++;
			break;
		case DIV_0_0:
			if(BattleGaugeCellPalDivSet(*counter2)){
				*counter2 = 0;
				(*counter)++;
			}else
				(*counter2)++;
			break;
		case DIV_0_1:
			(*counter)++;
			break;
		case DIV_0:
			//�ް�ޖ{��
//changed by soga 2002.06.01
			if((FightType&FIGHT_TYPE_SAFARI)&&(*counter2==0))
				ClientGaugeSt[*counter2] = GaugeActorSetSafari();
			else
				ClientGaugeSt[*counter2] = GaugeActorSet(*counter2);
			if(++(*counter2) == ClientSetMax){
				*counter2 = 0;
				(*counter)++;
			}
			break;
		case DIV_1:
			GaugeDefaultPosSet(*counter2);
			if(ClientType[*counter2] == BSS_CLIENT_MINE 
					|| ClientType[*counter2] == BSS_CLIENT_ENEMY)
				GaugePriSet(ClientGaugeSt[*counter2], 0);
			else
				GaugePriSet(ClientGaugeSt[*counter2], 1);
			if(++(*counter2) == ClientSetMax){
				*counter2 = 0;
				(*counter)++;
			}
			break;
		case DIV_2:
			//�ް�ޓ]����׾��
			if(MineEnemyCheck(*counter2) == 0){
//changed by soga 2002.06.01
				if((FightType&FIGHT_TYPE_SAFARI)==0)
					GaugeTransChrSet(ClientGaugeSt[*counter2],
								&PokeParaMine[SelMonsNo[*counter2]], G_ALL);
			}
			else
				GaugeTransChrSet(ClientGaugeSt[*counter2],
							&PokeParaEnemy[SelMonsNo[*counter2]], G_ALL);
			GaugeOFF(ClientGaugeSt[*counter2]);
			if(++(*counter2) == ClientSetMax){
				*counter2 = 0;
				(*counter)++;
			}
			break;
//		default:
		case DIV_3:
			//�|�P�����̉e���
			PokeShadowAllDataSet();
			//�퓬�p��ؽĉ�ʂŎg�p����ܰ��̏�����
			BattleListWorkInit();
			ret = 1;
			break;
	}
	return ret;
}


//=========================================================================
//	�ײ��ĂŎg�p����UserWork��ɐ؂��鼽�э\���̂̏�����
//=========================================================================
void ClientSystemWorkInit(void)
{
	memset(CliSys, 0, CLIENT_SYSTEM_SIZEOF);
	memset(BtlWork, 0, BTLWORKDATA_SIZEOF);
}

void ClientSystemWorkInitAll(void)
{
	ClientSystemWorkInit();
	memset(Climem, 0, CLIENT_MEMORY_SIZEOF);
}


//=========================================================================
//	�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
//=========================================================================
void PokeBanishMemory(void)
{
	int i;
	
	//�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
	for(i = 0; i < ClientSetMax; i++)
		Climem[i].act_banish = ActWork[PokemonStruct[i]].banish;
}

void PokeClientBanishMemory(u8 client_no)
{
	//�̪�Ăő��삳�ꂽ��Ư���׸ނ���ʐ؂�ւ����ׁ̈A�L������
	Climem[client_no].act_banish = ActWork[PokemonStruct[client_no]].banish;
}



//=========================================================================
//	�ϐg�A�ω����̃L���������ւ�
//
// my_client�̃L������target_client�̃L�����ɕύX���܂��B
// flg = 0:�ϐg(�Z�́u�ւ񂵂�v)	1:�ω�(���V�C�߹�݂̎p�ύX�Ȃ�)
//=========================================================================

extern u8 PokeEnemyPosY[2][2];

void HensinChrChange(u8 my_client, u8 target_client, u8 flg)
{
	u8 client_type;
	u16 monsno;
	u32 personal_rnd, id_no;
	u16 obj_palpos;
	void *paladrs;

	if(flg){	//�ւ�
		ActAnmChg(&ActWork[PokemonStruct[my_client]],BtlWork->work[0]);
		obj_palpos = PA_OBJ0 + 0x10 * my_client;
//		DecordWram((void*)powawan_pcl_ADRS, (void*)&UserWork[0]);
//		PaletteWorkSet((void *)&UserWork[0x20*BtlWork->work[0]],obj_palpos,0x20);
		PaletteWorkSet((void *)&UserWork[POWARUN_PALETTE+0x20*BtlWork->work[0]],obj_palpos,0x20);
		BattlePokeAnmNo[my_client]=BtlWork->work[0];
		if(Climem[my_client].hensin_monsno != 0){
			SoftFade(obj_palpos, 16, HENSIN_EVY, 0x7fff);
			CpuCopy(&PaletteWorkTrans[obj_palpos], &PaletteWork[obj_palpos], 0x20, 32);
		}
		ActWork[PokemonStruct[my_client]].y = GetPokemonYPos(my_client);
	}
	else{		//�ւ񂵂�
		if(ContestCheck()){	//�R���e�X�g
			client_type = CONT_POKEDECORD_NO;
			monsno = conpara->hensin_monsno;
			personal_rnd = conpara->personal_rnd;
			id_no = conpara->id_no;
			DecordPokeCellMakeBuchi(&PM2BObjDataCell[monsno], pm2bpara[monsno].size,
				pm2bpara[monsno].patcnt, (void*)0x2000000,
				(void*)DecordAdrs[client_type], monsno, conpara->hensin_personal_rnd);
			
		}
		else{				//�퓬
			client_type = ClientTypeGet(my_client);
			if(MineEnemyCheck(target_client) == SIDE_ENEMY)
				monsno=PokeParaGet(&PokeParaEnemy[SelMonsNo[target_client]], ID_monsno);
			else
				monsno= PokeParaGet(&PokeParaMine[SelMonsNo[target_client]], ID_monsno);

			if(MineEnemyCheck(my_client) == SIDE_MINE){
				//���߰��ٗ����͎����̂�K�p
				personal_rnd = PokeParaGet(&PokeParaMine[SelMonsNo[my_client]],
								 ID_personal_rnd);
				id_no = PokeParaGet(&PokeParaMine[SelMonsNo[my_client]], ID_id_no);
	//			DecordPokeCellMake(&PM2BObjDataCell[monsno], pm2bpara[monsno].size,
	//					pm2bpara[monsno].patcnt, (void*)0x2000000,
	//					(void*)DecordAdrs[client_type], monsno);
#if 1
				//�ւ񂵂�́A��p�̌����������������Ă���悤�ɂ���
				DecordPokeCellMakeBuchi(&PM2BObjDataCell[monsno], pm2bpara[monsno].size,
						pm2bpara[monsno].patcnt, (void*)0x2000000,
						(void*)DecordAdrs[client_type], monsno, HenshinRnd[my_client]);
#else
				DecordPokeCellMakeBuchi(&PM2BObjDataCell[monsno], pm2bpara[monsno].size,
						pm2bpara[monsno].patcnt, (void*)0x2000000,
						(void*)DecordAdrs[client_type], monsno, personal_rnd);
#endif
			}
			else{
	//			monsno= PokeParaGet(&PokeParaMine[SelMonsNo[target_client]], ID_monsno);
				//���߰��ٗ����͎����̂�K�p
				personal_rnd = PokeParaGet(&PokeParaEnemy[SelMonsNo[my_client]],
								 ID_personal_rnd);
				id_no = PokeParaGet(&PokeParaEnemy[SelMonsNo[my_client]], ID_id_no);
#if 1
				//�ւ񂵂�́A��p�̌����������������Ă���悤�ɂ���
				DecordPokeCellMakeBuchi(&PM2FObjDataCell[monsno], pm2fpara[monsno].size,
						pm2fpara[monsno].patcnt, (void*)0x2000000,
						(void*)DecordAdrs[client_type], monsno, HenshinRnd[my_client]);
#else
				DecordPokeCellMakeBuchi(&PM2FObjDataCell[monsno], pm2fpara[monsno].size,
						pm2fpara[monsno].patcnt, (void*)0x2000000,
						(void*)DecordAdrs[client_type], monsno, personal_rnd);
#endif
			}
		}
		
		//��Ұ��݂��Ȃ��̂Ŏ����]������Ȃ�����A�����ŃR�s�[�����G��Vram�ɓ]��
		DIV_DMACOPY(3, DecordAdrs[client_type], 
			OBJ_MODE0_VRAM + ActWork[PokemonStruct[my_client]].oamData.CharNo*32,0x800,32);

		obj_palpos = PA_OBJ0 + 0x10 * my_client;
		paladrs = (void*)PokePalGetPara(monsno, id_no, personal_rnd);
		DecordWram(paladrs, (void*)&UserWork[0]);
		PaletteWorkSet((void *)&UserWork[0], obj_palpos, 0x20);
		//���V�C�|�P�����͓���ȃZ�b�g������
		if(monsno==MONSNO_POWARUN){
//			DecordWram((void*)powawan_pcl_ADRS, (void*)&UserWork[0]);
			DecordWram(paladrs, (void*)&UserWork[POWARUN_PALETTE]);
			PaletteWorkSet((void *)&UserWork[POWARUN_PALETTE+0x20*BattlePokeAnmNo[target_client]],obj_palpos,0x20);
		}
		SoftFade(obj_palpos, 16, HENSIN_EVY, 0x7fff);
		CpuCopy(&PaletteWorkTrans[obj_palpos], &PaletteWork[obj_palpos], 0x20, 32);
	
		if(ContestCheck() == 0){
			Climem[my_client].hensin_monsno = monsno;
			BattlePokeAnmNo[my_client] = BattlePokeAnmNo[target_client];
		}
		ActWork[PokemonStruct[my_client]].y = GetPokemonYPos(my_client);
		ActAnmChg(&ActWork[PokemonStruct[my_client]],BattlePokeAnmNo[my_client]);
	}
}


//=========================================================================
//	�g���莞�̃L���������ւ�
//
//	decord_flg = 0:�߹�̷݂�ׂ�g����̷�ׂɍ����ւ�
//				 1:�g����̷�ׂ��߹�̷݂�ׂɖ߂�
//=========================================================================
void MigawariChrChange(u8 client_no, u8 decord_flg)
{
	u8 client_type;
	int i;
	u16 obj_palpos;
	
//	Climem[my_client].migawari = 1;
	
	if(decord_flg == 0){
		if(ContestCheck())
			client_type = CONT_POKEDECORD_NO;
		else
			client_type = ClientTypeGet(client_no);
			
		if(ContestCheck())
			DecordVram((void*)migawari_b_pch_ADRS, (void*)DecordAdrs[client_type]);
		else if(MineEnemyCheck(client_no))
			DecordVram((void*)migawari_f_pch_ADRS, (void*)DecordAdrs[client_type]);
		else
			DecordVram((void*)migawari_b_pch_ADRS, (void*)DecordAdrs[client_type]);
		for(i = 1; i < 4; i++){
			DIV_DMACOPY(3, DecordAdrs[client_type], 
				DecordAdrs[client_type]+0x800*i, 0x800, 32);
		}
		obj_palpos = PA_OBJ0 + 0x10 * client_no;
		DecordPaletteWork((void*)s_migawari_pcl_ADRS, obj_palpos, 0x20);
		//��������Ă���񂾂����H 2002.08.15(��) matsuda
	//	DecordPaletteWork((void*)s_migawari_pcl_ADRS, PA_BG8 + 0x10 * client_no, 0x20);
	}
	else if(ContestCheck() == 0){
		if(MineEnemyCheck(client_no))
			PokemonFObjCellPalSet(&PokeParaEnemy[SelMonsNo[client_no]], client_no);
		else
			PokemonBObjCellPalSet(&PokeParaMine[SelMonsNo[client_no]], client_no);
	}
}

//----------------------------------------------------------
//	�g����̷�ׂɍ����ւ��āA�߹�݂�Y���W�̵̾�Ă��g����̷�חp�ɕύX����
//	decord_flg = 0:�߹�̷݂�ׂ�g����̷�ׂɍ����ւ�
//				 1:�g����̷�ׂ��߹�̷݂�ׂɖ߂�
//	poke = �߹�݂�ActNo
//----------------------------------------------------------
void MigawariChange(u8 client_no, u8 decord_flg, u8 poke)
{
	MigawariChrChange(client_no, decord_flg);
	ActAnmChg(&ActWork[poke], BattlePokeAnmNo[client_no]);//Vram�ɓ]�����邽�߂ɱ��
#if 1
	if(decord_flg == 0)
		ActWork[poke].y = GetMigawariYPos(client_no);
	else
		ActWork[poke].y = GetPokemonYPos(client_no);
#else
	ActWork[poke].y = GetPokemonYPos(client_no);
#endif
}

//----------------------------------------------------------
//	�ZNo���u�݂����v���`�F�b�N���āA�u�݂����v�Ȃ�ΐg�����׸ނ�Ă���
//----------------------------------------------------------
void MigawariFlgCheckSet(u8 client_no, u16 wazano)
{
	if(wazano == WAZANO_MIGAWARI)
		Climem[client_no].migawari = 1;
}

//----------------------------------------------------------
//	�g�����׸ނ����Z�b�g����
//----------------------------------------------------------
void MigawariFlgReset(u8 client_no)
{
	Climem[client_no].migawari = 0;
}


//=========================================================================
//	���݂�HP�̶װ���������������Ȃ�x������炷
//	�����łȂ��Ȃ�x������炳�Ȃ�(�I�t����)
//=========================================================================
void PinchSeCheckPlay(PokemonParam *pp, u8 client_no)
{
	u16 hp, hpmax;
	u8 color;
	
	hp = PokeParaGet(pp, ID_hp);
	hpmax = PokeParaGet(pp, ID_hpmax);
	color = GetGaugeDottoColor(hp, hpmax);
	
	if(color == 1){
		if(Climem[client_no].pinch_se == 0){
			if(Climem[client_no ^2].pinch_se == 0)
				SePlay(SE_HINSI);
			Climem[client_no].pinch_se = 1;
		}
	}
	else{
		Climem[client_no].pinch_se = 0;
		if(FightCheck() == 0)
			m4aSongNumStop(SE_HINSI);
		else if(FightCheck() && Climem[client_no^2].pinch_se == 0)
			m4aSongNumStop(SE_HINSI);
	}
}

//----------------------------------------------------------
//	�����I�ɉ����s���`���̌x�������~�߂�
//----------------------------------------------------------
void PinchSeStop(void)
{
	u8 client_no;
	
	client_no = ClientNoGet(BSS_CLIENT_MINE);
	Climem[client_no].pinch_se = 0;
	if(FightCheck())
		Climem[client_no^2].pinch_se = 0;
	m4aSongNumStop(SE_HINSI);
}

//----------------------------------------------------------
//	�|�P�����̃Q�[�W�̃J���[���擾
//----------------------------------------------------------
u8 PokeGaugeColorGet(PokemonParam *pp)
{
	u8 color;
	s16 hp, hpmax;
	
	hp = PokeParaGet(pp, ID_hp);
	hpmax = PokeParaGet(pp, ID_hpmax);
	color = GetGaugeDottoColor(hp, hpmax);
	return color;
}

//----------------------------------------------------------
//	�ް�ނ̶װ���������ăs���`���̌x�������~�߂�
//		��ܰ�������������͂��Ȃ��B��{�I�ɂ̓|�P�������X�g�p
//----------------------------------------------------------
void PinchSeCheckStop(void)
{
	u8 client1, client2;
//	u8 color;
//	u8 stop1, stop2;
	u8 pos1, pos2;

	if(sys.FieldBattleFlag == 0)
		return;
	
	client1 = ClientNoGet(BSS_CLIENT_MINE);
	client2 = ClientNoGet(BSS_CLIENT_MINE2);
	pos1 = SearchListRow(SelMonsNo[client1]);
	pos2 = SearchListRow(SelMonsNo[client2]);
	
	//if(ClientPokeExistCheck(client1))
	if(PokeParaGet(&PokeParaMine[pos1], ID_hp) > 0)
		PinchSeCheckPlay(&PokeParaMine[pos1], client1);
	if(FightCheck() && PokeParaGet(&PokeParaMine[pos2], ID_hp) > 0)
		PinchSeCheckPlay(&PokeParaMine[pos2], client2);
}




//=========================================================================
//	�S�Ẵ|�P������Affine�׸ނ�OFF����B�̨��Ȱ�ެ�����J������킯�ł͂Ȃ��B
//		���Z�G�t�F�N�g�p	�g�k�����߹�݂�KakusyukuInit�����̂ŁA������
//							Affine��ON�ɂȂ�B����ȊO�̂�OBJ�ݼ޵��ް�΍�Ƃ���
//							OFF�ɂ��Ă���
// 	����: 0=�g�kOFF	1=�g�kON
//=========================================================================
void PokeAffineON_OFF(u8 on_off)
{
	int i;
	
	for(i = 0; i < ClientSetMax; i++)
	{
//		if(PokeBG2taimeCheck(i) == 0)	2002.09.23(��) Exist�ɕύX
		if(ClientPokeExistCheck(i) == 0)
			continue;
		ActWork[PokemonStruct[i]].oamData.AffineMode = on_off;
		
		//�ێ����Ă�̨�ð���No�ɂ���Ă�H,V�د�߂�ON�ɂȂ��Ă��܂��̂�
		if(on_off == 0){
			CliSys[i].poke_aff_no = ActWork[PokemonStruct[i]].oamData.AffineParamNo;
			ActWork[PokemonStruct[i]].oamData.AffineParamNo = 0;
		}
		else{
			ActWork[PokemonStruct[i]].oamData.AffineParamNo = CliSys[i].poke_aff_no;
		}
	}
}


//=========================================================================
//	�|�P�����̉e�ACell,Pal,Actor��S�ľ��
//=========================================================================
void PokeShadowAllDataSet(void)
{
	u8 client_no;

	DecordCellSet((void*)&PokeShadowCell);	//����A��گĂ�HP�ް�ނƈꏏ
	client_no = ClientNoGet(BSS_CLIENT_ENEMY);
	CliSys[client_no].shadow_act = AddActor(&PokeShadowHeader, 
		GetWazaEffPos(client_no,EFF_X),
		GetWazaEffPos(client_no,EFF_Y)+32,
		200);
	ActWork[CliSys[client_no].shadow_act].work[0] = client_no;
	if(FightCheck()){
		client_no = ClientNoGet(BSS_CLIENT_ENEMY2);
		CliSys[client_no].shadow_act = AddActor(&PokeShadowHeader, 
			GetWazaEffPos(client_no,EFF_X),
			GetWazaEffPos(client_no,EFF_Y)+32,
			200);
		ActWork[CliSys[client_no].shadow_act].work[0] = client_no;
	}
}

//=========================================================================
//	�|�P�����̉e�A�N�^�[�V�[�P���X
//	work[0]�ɑΏ��߹�݂�client_no�������Ă�
//=========================================================================
void PokeShadowSeq(actWork *xreg)
{
	actWork *pokeAct;
	u8 client_no;
	u8 banish = 0;
	
	client_no = xreg->work[0];
	pokeAct = &ActWork[PokemonStruct[client_no]];
	if(pokeAct->act_sw == 0 || ClientPokeExistCheck(client_no) == 0){
		xreg->move = PokeShadowBanishSeq;
		return;
	}
	if(WazaEffectFlag || pokeAct->banish)
		banish = 1;	//�̪�Ē��A�Ώ��߹�݂������Ă��鎞�͏���
	else if(Climem[client_no].hensin_monsno != 0){
		if(PokeYOfsTbl[Climem[client_no].hensin_monsno] == 0)
			banish = 1;
	}
	if(Climem[client_no].migawari)
		banish = 1;
	
	xreg->x = pokeAct->x;
	xreg->dx = pokeAct->dx;
	
	xreg->banish = banish;
}

void PokeShadowBanishSeq(actWork *xreg)
{
	xreg->banish = 1;
}

//----------------------------------------------------------
//	�|�P�����̉e�V�[�P���X���N������
//----------------------------------------------------------
void PokeShadowSeqSet(u8 client_no, u16 monsno)
{
	if(MineEnemyCheck(client_no) == SIDE_MINE)	//add 2002.09.28(�y)
		return;
	
	if(Climem[client_no].hensin_monsno != 0)	//add 2002.09.28(�y)
		monsno = Climem[client_no].hensin_monsno;
	
	if(PokeYOfsTbl[monsno] != 0)
		ActWork[CliSys[client_no].shadow_act].move = PokeShadowSeq;
	else
		ActWork[CliSys[client_no].shadow_act].move = PokeShadowBanishSeq;
}

//----------------------------------------------------------
//	�|�P�����̉e�V�[�P���X����Ư������ɂ���
//----------------------------------------------------------
void PokeShadowBanishSeqSet(u8 client_no)
{
	ActWork[CliSys[client_no].shadow_act].move = PokeShadowBanishSeq;
}




//=========================================================================
//
//		�E�B���h�E�̋󔒕��������Ŗ��߂�
//												by matsuda 2002.08.31(�y)
//=========================================================================
#define WIN_MASK_DOTTO	0xf
#define MASK_UP			(WIN_MASK_DOTTO<<4)
#define MASK_DOWN		(WIN_MASK_DOTTO)
void BattleWinPaint(void)
{
	u16 *vram;
	int i, s;
	
	vram = (u16*)(BG_VRAM + 0x12*32);	//win�L�����]���ʒu
	
	for(i = 0; i < 9; i++){	//�E�B���h�E�L����9�L����
		for(s = 0; s < 32/2; s++){
			if(((*vram) & 0xf000) == 0)
				(*vram) |= 0xf000;//MASK_UP;
			if(((*vram) & 0x0f00) == 0)
				(*vram) |= 0x0f00;//MASK_DOWN;
			if(((*vram) & 0x00f0) == 0)
				(*vram) |= 0x00f0;
			if(((*vram) & 0x000f) == 0)
				(*vram) |= 0x000f;
			vram++;
		}
	}
}




//=========================================================================
//
//		�|�P��������ւ����ɃN���A���ׂ��t���O���N���A����
//												by matsuda 2002.09.05(��)
//=========================================================================
//----------------------------------------------------------
//	�����Fbaton_flg = �o�g���^�b�`�t���O
//----------------------------------------------------------
void ClientPokeSysFlagClear(u8 client_no, u8 baton_flg)
{
	Climem[client_no].hensin_monsno = 0;	//�ϐg�׸޸ر
	BattlePokeAnmNo[client_no]=0;		//���V�C�|�P�����p�̃A�j���[�V�����i���o�[������
	if(baton_flg == 0)
		MigawariFlgReset(client_no);			//�g�����׸޸ر
}

