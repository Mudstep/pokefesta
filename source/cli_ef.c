
//======================================================
//    cli_ef.c                                           
//                                                      
//	�N���C�A���g�p�G�t�F�N�g���[�`��
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
#include "palanm.h"
#include "madefine.h"
#include "cli_num.h"
#include "cli_def.h"
#include "client_t.h"
#include "cli_ef.h"
#include "pm_aff.h"
#include "server.h"
#include "client.h"
#include "wazatool.h"
#include "gauge.h"
#include "task.h"
#include "mus_tool.h"
#include "a_fight.h"


//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
u8 MonsterBallSet(u16 itemNo, u8 rate);
void MonsterBallMove(actWork *actst);
void MonsterBallOpenWait(actWork *actst);
void MonsterBallCloseWait(actWork *actst);
void MonsterBallClose(actWork *actst);
void MonsterBallGetWait(actWork *actst);
void MonsterBallGetMove(actWork *actst);
void MonsterBallGetMiss(actWork *actst);
void MonsterBallGet(actWork *actst);
void GAE_Move(actWork *Xreg);
//void GCE_move(actWork *Xreg);
static void MonsterBallGetGuga(actWork *actst);
static void MonsterBallGetGugaWait(actWork *xreg);
static void MonsterBallGetFailInit(actWork *actst);
static void MonsterBallGetSuccessInit(actWork *actst);
void GAE_Wait(actWork *Xreg);
//static void MonsterBallMoveMain(actWork *Xreg);
//static void MBHikariInitSeq(actWork *xreg);
//static void MBHikariMainSeq(actWork *xreg);
u8 MBHikariSet(u8 x, u8 y, u8 oam_pri, u8 act_pri);
u8 MBHikariColorEffectSet(u8 type, u8, u32);
//static void MBHikariColorEffectInMain(u8 id);
//static void MBHikariColorEffectOutInit(u8 id);
//static void MBHikariColorEffectOutMain(u8 id);
static void MB_PokeMinePut(actWork *xreg);
static void MB_PokeMinePutMain(actWork *xreg);
static void MB_PokeEnemyPut(actWork *xreg);
//static void HikariAnmCheck(actWork *xreg);
void MonsterBallAppearEffSet(u8 pokeact, u8 palno, u8 xpos, u8 ypos, u8,u8,u8,u32);
static void MonsBallAppearWait(actWork *xreg);
static void MonsterBallAppearEnd(actWork *xreg);
static void GaugeSwayMain(actWork *xreg);
static void MonsBallReturnWait(actWork *xreg);
static void MonsterBallReturnClose(actWork *actst);
static void MonsterBallReturnEnd(actWork *actst);
static void AppearWait(actWork *xreg);
static u16 GetBallGet(u8 client_no);

//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern u8 BallItemNoTblNoConv(u16 itemno);


//=========================================================================
//	�v���O����
//=========================================================================

//=========================================================================
//	�ݽ���ްق���o�鉌
//=========================================================================
//----------------------------------------------------------
// �߂�l: ���̪�ĊĎ��p�����̔z��ԍ�(�I�����������Ȃ��ꍇ�͓��ɕK�v�Ȃ�)
//	end_sw = 0:���̪�ďI�����������Ȃ�(�̪�Ă��I������珟���DelActor�����
//           1:�̪�ďI������������(�̪�Ă��I�������move��DummyActMove������̂�
//�@�@�@�@�@�@ ������m�F���Ă��玩���ŊĎ��p������DelActor����
//----------------------------------------------------------
u8 MBSmokeSet(s16 xpos, s16 ypos, u8 end_sw)
{
	u8 actno, no;
	
	#if (MBSMOKE_CELL == 1)
	if(CellCharNumGet(MBSmokeCell.act_num) == 0xffff)
	{
		DecordCellSet((void*)&MBSmokeCell);
		DecordPalSet((void*)&MBSmokePal);
		//CellSet((CellData*)&MBSmokeCell);
		//PalSet((PalData*)&MBSmokePal);
	}
	#endif
	
	actno = AddBanishActor(MBSmokeCheck);
	ActWork[actno].work[1] = end_sw;
	
	no = AddActor(&MBSmokeHeader, xpos-16, ypos-16, MBSMOKE_PRI);		//����
	ActWork[no].work[0] = actno;
	ActWork[actno].work[0]++;
	ActAnm(&ActWork[no]);

	no = AddActor(&MBSmokeHeader, xpos, ypos-16, MBSMOKE_PRI);	//�E��
	ActWork[no].work[0] = actno;
	ActWork[actno].work[0]++;
	ActAnmChg(&ActWork[no], 1);
	ActAnm(&ActWork[no]);

	no = AddActor(&MBSmokeHeader, xpos-16, ypos, MBSMOKE_PRI);	//����
	ActWork[no].work[0] = actno;
	ActWork[actno].work[0]++;
	ActAnmChg(&ActWork[no], 2);
	ActAnm(&ActWork[no]);

	no = AddActor(&MBSmokeHeader, xpos, ypos, MBSMOKE_PRI);//�E��
	ActWork[no].work[0] = actno;
	ActWork[actno].work[0]++;
	ActAnmChg(&ActWork[no], 3);
	ActAnm(&ActWork[no]);
	
//	SePlay(SE_BOWA2);

	return actno;
}

void MBSmokeCheck(actWork *actst)
{
	if(actst->work[0] == 0)
	{
		#if (MBSMOKE_CELL == 1)
		CellDel(MBSmokeCell.act_num);
		ObjPalRegNumDel(MBSmokePal.reg_num);
		#endif

		if(actst->work[1] == 0)
		{
			DelActor(actst);
			return;
		}
		actst->move = DummyActMove;
	}
}

void MBSmokeWait(actWork *actst)
{
	if(actst->anmend_sw == 1)
	{
		ActWork[actst->work[0]].work[0]--;
		DelActor(actst);
	}
}



//=========================================================================
//	�ݽ���ްَg�p�̪��
//=========================================================================
//=========================================================================
//	�f�[�^
//=========================================================================
//----------------------------------------------------------
//	�����X�^�[�{�[��
//----------------------------------------------------------
const CellData MonsterBallCellTbl[] = 
{
	{(void *)b_ani0_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_MonsterBall},
	{(void *)b_ani1_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_SuperBall},
	{(void *)b_ani2_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_SafariBall},
	{(void *)b_ani3_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_HyperBall},
	{(void *)b_ani4_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_MasterBall},
	{(void *)b_ani6_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_NetBall},
	{(void *)b_ani7_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_DiveBall},
	{(void *)b_ani8_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_NestBall},
	{(void *)b_ani9_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_RepeatBall},
	{(void *)b_ania_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_TimerBall},
	{(void *)b_anib_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_GoujyasuBall},
	{(void *)b_anic_pch_ADRS, OBJ_SIZE16_16x16 * 3, CELLID_PremiaBall},
};
//-- cli_ef.h�ɏ�����Ă���enum�ƕ��т𓯂��ɂ��Ă�������!! --//
const PalData MonsterBallPalTbl[]=
{
	{(void *)b_ani0_pcl_ADRS,	CELLID_MonsterBall},
	{(void *)b_ani1_pcl_ADRS,	CELLID_SuperBall},
	{(void *)b_ani2_pcl_ADRS,	CELLID_SafariBall},
	{(void *)b_ani3_pcl_ADRS,	CELLID_HyperBall},
	{(void *)b_ani4_pcl_ADRS,	CELLID_MasterBall},
	{(void *)b_ani6_pcl_ADRS,	CELLID_NetBall},
	{(void *)b_ani7_pcl_ADRS,	CELLID_DiveBall},
	{(void *)b_ani8_pcl_ADRS,	CELLID_NestBall},
	{(void *)b_ani9_pcl_ADRS,	CELLID_RepeatBall},
	{(void *)b_ania_pcl_ADRS,	CELLID_TimerBall},
	{(void *)b_anib_pcl_ADRS,	CELLID_GoujyasuBall},
	{(void *)b_anic_pcl_ADRS,	CELLID_PremiaBall},
};

//=========================================================================
//	�A�N�^�[�e�[�u��
//=========================================================================
//----------------------------------------------------------
//	�����X�^�[�{�[��
//----------------------------------------------------------
const ActOamData MonsterBallOamData = 
{
	0,		//u32 VPos:8;             // �x���W
	3,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	0,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
	1,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	2,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};

static const actAnm monsball_up_anm0[] =
{
	{OBJ16_16x16 * 0, 5, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm monsball_upOpen_anm0[] =
{
	{OBJ16_16x16 * 1, 1, 0, 0},
	{ACT_ANMLOOP, 0,0,0},
};
static const actAnm monsball_down_anm0[] =
{
	{OBJ16_16x16 * 2, 5, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm monsball_downOpen_anm0[] =
{
	{OBJ16_16x16 * 3, 1, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm monsball_anm[] =
{
	{OBJ16_16x16 * 0, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm monsball_open_anm[] =
{
	{OBJ16_16x16 * 1, 5, 0, 0},
	{OBJ16_16x16 * 2, 5, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm monsball_close_anm[] =
{
	{OBJ16_16x16 * 1, 5, 0, 0},
	{OBJ16_16x16 * 0, 5, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm * const MonsterBallAnmTbl[] = 
{
	monsball_anm,
	monsball_open_anm,
	monsball_close_anm,
	//�ȉ��A�g�p���ĂȂ�
	monsball_up_anm0,
	monsball_down_anm0,
	monsball_upOpen_anm0,
	monsball_downOpen_anm0,
};

//--- �g�k ----------
static	const actAffAnm MonsBallActAff[] = 		//���񂾂񏬂���
{
//	{ �g�kx, �g�ky, �p�x, ���� }
//	{-1, -1, 0, 1},
	{0,0,0,1},
	{ACT_AFLOOP, 0, 0, 0 },
};
#define BALL_GUGA_AFF	3	//10
static const actAffAnm MonsBallGugaAff0[] = //�E��]
{
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0, 0, -BALL_GUGA_AFF, 1},
	{ACT_AFLOOP, 0, 0, 0 },
};
static const actAffAnm MonsBallGugaAff1[] = //����]
{
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0, 0, BALL_GUGA_AFF, 1},
	{ACT_AFLOOP, 0, 0, 0 },
};
static const actAffAnm MonsBallGugaAff2[] = //ؾ�ėp
{
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0x100, 0x100, 0, ACT_AFSET},
	{ACT_AFEND, 0, 0, 0 },
};
static const actAffAnm MonsBallKaiten[] = //�o���̪�Ď��̉�]
{
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0, 0, 25, 1},
	{ACT_AFLOOP, 0, 0, 0 },
};
static const actAffAnm * const MonsBallActAffTbl[] = 
{
	MonsBallActAff,
	MonsBallGugaAff0,
	MonsBallGugaAff1,
	MonsBallGugaAff2,
	MonsBallKaiten,
};

//-- cli_ef.h�ɂ���enum�ƕ��т𓯂��ɂ��Ă�������! --//
const actHeader MonsterBallHeader[MONS_BALL_MAX] = 
{
	{
		CELLID_MonsterBall,
		CELLID_MonsterBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_SuperBall,
		CELLID_SuperBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_SafariBall,
		CELLID_SafariBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_HyperBall,
		CELLID_HyperBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_MasterBall,
		CELLID_MasterBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_NetBall,
		CELLID_NetBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_DiveBall,
		CELLID_DiveBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_NestBall,
		CELLID_NestBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_RepeatBall,
		CELLID_RepeatBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_TimerBall,
		CELLID_TimerBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_GoujyasuBall,
		CELLID_GoujyasuBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
	{
		CELLID_PremiaBall,
		CELLID_PremiaBall,
		&MonsterBallOamData,
		MonsterBallAnmTbl,
		0,
		MonsBallActAffTbl,
		MonsterBallMove,
	},
};


//=========================================================================
//	�v���O����
//=========================================================================


//=========================================================================
//	�߹�ݕߊl�̪��
// rate = �ߊl�� or �o������(0xff:�����|�P�������o�����A 0xfe�F�G�|�P�������o����)
//=========================================================================
static void MonsterBallSetMain(u8 id);

u8 MonsterBallSet(u16 itemNo, u8 rate)
{
	u8 id;
	
	//�߹�ݏo���̪�Ă͏I����ball_eff_wait�Ō���B�ޯĴ̪�Ă�ClientEffectWait�Ō���
	ClientEffectWait = 1;
	CliSys[ClientNo].ball_eff_wait = 1;
	
	id = AddTask(MonsterBallSetMain, 5);
	TaskTable[id].work[1] = itemNo;
	TaskTable[id].work[2] = rate;
	TaskTable[id].work[3] = ClientNo;
	return 0;
}

static void MonsterBallSetMain(u8 id)
{
	u8 no;
	u16 itemNo, rate;
	u8 client_no;
	u8 flg = 0;
	u16 ball;
	u8 tblno;
	
	if(TaskTable[id].work[0] == 0){
		TaskTable[id].work[0]++;
		return;
	}
	
	itemNo = TaskTable[id].work[1];
	rate = TaskTable[id].work[2];
	client_no = TaskTable[id].work[3];
	
//	DecordCellSet((CellData*)&MonsterBallCellTbl[BallNo]);
//	DecordPalSet((PalData*)&MonsterBallPalTbl[BallNo]);
	if(MineEnemyCheck(client_no))
		ball = PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_get_ball);
	else
		ball = PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_get_ball);
	tblno = BallItemNoTblNoConv(ball);
	MonsBallCellPalSet(tblno);

	no = AddActor(&MonsterBallHeader[tblno], MONS_BALL_X, MONS_BALL_Y, MONS_BALL_PRI);

#if 1
	//-- work init -------//
	ActWork[no].work[0] = 128;
	ActWork[no].work[1] = 0;
//	ActWork[no].work[6] = DefenceClient;
	ActWork[no].work[7] = rate;
#endif

	switch(rate)
	{
		case 0xff:
			DefenceClient = client_no;
			ActWork[no].x = 24;//16;//32;
			ActWork[no].y = 68;//72;
			ActWork[no].move = MB_PokeMinePut;
			break;
		case 0xfe:
			ActWork[no].x = GetWazaEffPos(client_no, EFF_X);
			ActWork[no].y = GetWazaEffPos(client_no, EFF_Y) + 24;
			DefenceClient = client_no;
			ActWork[no].work[0] = 0;
			ActWork[no].move = MB_PokeEnemyPut;
			break;
		default:
			DefenceClient = ClientNoGet(BSS_CLIENT_ENEMY);
			flg = 1;	//�V�K�ɼ��ݽ����蒼�������� 2002.05.01(��)
	}
	ActWork[no].work[6] = DefenceClient;	//���b��  ���ޯ��p��DefenceClient���P�ɌŒ�

	if(flg == 0)
		DelTask(id);
	else{
		ActWork[no].work[0] = 34;	//38;
		ActWork[no].work[2] = GetWazaEffPos(DefenceClient, EFF_X);
		ActWork[no].work[4] = GetWazaEffPos(DefenceClient, EFF_Y) - 16;
		ActWork[no].work[5] = -40;
		YamanariMoveInit(&ActWork[no]);
		ActWork[no].oamData.AffineParam = id;
		TaskTable[id].work[4] = DefenceClient;
		TaskTable[id].TaskAdrs = DummyTaskSeq;
		SePlay(SE_NAGERU);
	}
}


//=========================================================================
//	�������ݽ
//=========================================================================
#define GET_BAUND_NUM		3
#define GET_FURIHABA_Y		(-40)
#define GET_SEC_SP			3
#define GET_SUB_FURIHABA	(-20)
#define GET_LOOP_NUM		(127 / GET_SEC_SP * GET_BAUND_NUM)
//----------------------------------------------------------
//	
//----------------------------------------------------------
#define MB_FURIHABA_V (80)
//#define MB_FURIHABA_H (TEST_ENEMY_X - MONS_BALL_X + 20)
//#define MB_STOP_Y		(TEST_ENEMY_Y + 8)
void MonsterBallMove(actWork *Xreg)
{
	u8 defence_client;
	u8 taskid, rate;
	u16 ballno, tblno;
	
	if(YamanariMoveMain(Xreg) == 0)
		return;
	
	taskid = Xreg->oamData.AffineParam;
	defence_client = TaskTable[taskid].work[4];
	rate = TaskTable[taskid].work[2];
	
	ActAnmChg(Xreg, 1);
	//ActAffAnmHoldChg(Xreg, 1);
	Xreg->aff_pause = 1;
	Xreg->x += Xreg->dx;
	Xreg->y += Xreg->dy;
	Xreg->dx = 0;
	Xreg->dy = 0;

	Xreg->work[5] = 0;
	
	ballno = GetBallGet(defence_client);
	tblno = BallItemNoTblNoConv(ballno);
	HikariEffectSet(Xreg->x, Xreg->y-5, 1, MONS_BALL_PRI-1, tblno);
	Xreg->work[0] = HikariColorEffectSet(0, defence_client, 14, tblno);

	//-- �����[�`���ƍ�������̂ŕK�v��ܰ��̒l�����āA�����Del���� --//
	Xreg->work[6] = defence_client;
	Xreg->work[7] = rate;
	DelTask(taskid);
	
	Xreg->move = MonsterBallOpenWait;
}
//----------------------------------------------------------
//	
//----------------------------------------------------------
#define MB_OPEN_SP	3	//�ްق̊J��(����)��߰��
void MonsterBallOpenWait(actWork *actst)
{
//	if(TaskTable[actst->work[0]].occ == 0)
		actst->move = MonsterBallCloseWait;
}
//----------------------------------------------------------
//	
//----------------------------------------------------------
#define MB_CLOSE_WAIT 10	//�ްق��J���Ă������܂ł̳���
void MonsterBallCloseWait(actWork *actst)
{
	actst->work[5]++;
	if(actst->work[5] == MB_CLOSE_WAIT)
	{
//		ActAnmChg(actst, 2);
		actst->work[5] = 0;
		actst->move = MonsterBallClose;

		//�ߊl�Ώۂ��߹�ݏk��   �k���̎d���͎b��I ��ŗv�ύX ��
//		ActWork[EnemyStruct].aff_tbl = BallPokeAffTbl;
//		ActWork[EnemyStruct].oamData.AffineMode = 1;
//		AffineNumSet(&ActWork[EnemyStruct]);
		ActAffAnmChg(&ActWork[PokemonStruct[actst->work[6]]], PAF_BALLIN);
		ActAnm(&ActWork[PokemonStruct[actst->work[6]]]);
		ActWork[PokemonStruct[actst->work[6]]].work[1] = 0;
	}
}
//----------------------------------------------------------
//	
//----------------------------------------------------------
void MonsterBallClose(actWork *actst)
{
	if(actst->work[5]++ == 10)
		SePlay(SE_SUIKOMU);
		
//	if(actst->anmend_sw == 1 && ActWork[PokemonStruct[actst->work[6]]].affend_sw)
	if(ActWork[PokemonStruct[actst->work[6]]].affend_sw)
	{
		ActAnmChg(actst, 2);

		ActWork[PokemonStruct[actst->work[6]]].banish = 1;	//��
		actst->work[5] = 0;
		actst->move = MonsterBallGetWait;
	}
	else
	{
		ActWork[PokemonStruct[actst->work[6]]].work[1] += 0x0060;
		ActWork[PokemonStruct[actst->work[6]]].dy = 
			-ActWork[PokemonStruct[actst->work[6]]].work[1] >> 8;
	}
}
//----------------------------------------------------------
//	
//----------------------------------------------------------
#define GET_WAIT_TIME 1
#define BALL_FURIHABA_DEFAULT	32
void MonsterBallGetWait(actWork *actst)
{
	if(actst->anmend_sw == 0)
		return;
		
	actst->work[5]++;
	if(actst->work[5] == GET_WAIT_TIME)
	{
//		actst->aff_pause = 0;
		//ActAffAnmHoldChg(actst, 1);
		actst->work[3] = 0;
		actst->work[4] = BALL_FURIHABA_DEFAULT;	//�U�蕝
		actst->work[5] = 0;
		actst->y += CosMove(0, actst->work[4]);
		actst->dy = -CosMove(0, actst->work[4]);
		actst->move = MonsterBallGetMove;
	}
}
//----------------------------------------------------------
//	
//----------------------------------------------------------
#define BALL_ADD_SEC	4
#define BALL_FURIHABA_SUB	10
void MonsterBallGetMove(actWork *actst)
{
	u8 end_flg = 0;

	switch(actst->work[3] & 0xff){
		case 0:
			actst->dy = -CosMove(actst->work[5], actst->work[4]);
			actst->work[5] += BALL_ADD_SEC + (actst->work[3]>>8);
			if(actst->work[5] >= 64){
				actst->work[4] -= BALL_FURIHABA_SUB;
				actst->work[3]++;
				actst->work[3] += 0x0100;
				if((actst->work[3] >> 8) == 4)	//3)
					end_flg = 1;
				
				switch(actst->work[3] >> 8){
					case 1:
						SePlay(SE_KON);
						break;
					case 2:
						SePlay(SE_KON2);
						break;
					case 3:
						SePlay(SE_KON3);
						break;
					default:
						SePlay(SE_KON4);
				}
			}
			break;
		case 1:
			actst->dy = -CosMove(actst->work[5], actst->work[4]);
			actst->work[5] -= BALL_ADD_SEC + (actst->work[3]>>8);
			if(actst->work[5] <= 0){
				actst->work[5] = 0;
				actst->work[3] &= 0xff00;
			}
			break;
	}
	
	if(end_flg){
		actst->work[3] = 0;
		actst->y += CosMove(64, BALL_FURIHABA_DEFAULT);	//actst->dy;
		actst->dy = 0;
		if(actst->work[7] == 0)	//�O�K������
			actst->move = MonsterBallGetFailInit;
		else
		{
			actst->move = MonsterBallGetGugaWait;
			actst->work[4] = 1;
			actst->work[5] = 0;
		}
	}
}

static void MonsterBallGetGugaWait(actWork *actst)
{
	if(actst->work[3]++ == 30){
		actst->work[3] = 0;
		actst->aff_pause = 1;
		ActAffAnmChg(actst, 1);
		actst->move = MonsterBallGetGuga;
		SePlay(SE_BOWA);
	}
}

#define BALL_GUGA_X		4
#define BALL_GUGA_WAIT	0
#define BALL_GUGA_CENTER_WAIT	30
static void MonsterBallGetGuga(actWork *actst)
{
	switch(actst->work[3] & 0xff)
	{
		case 0:
		case 2:
			actst->dx += actst->work[4];
			actst->work[5] += actst->work[4];
			actst->aff_pause = 0;
			//if(actst->work[5] > 127)
			if(actst->work[5] >= BALL_GUGA_X || actst->work[5] <= -BALL_GUGA_X){
				actst->work[3]++;
				actst->work[5] = 0;
			}
			break;
		case 1:
			if(actst->work[5]++ == BALL_GUGA_WAIT){
				actst->work[5] = 0;
				actst->work[4] = -actst->work[4];
				actst->work[3]++;
				actst->aff_pause = 0;
				if(actst->work[4] < 0)
					ActAffAnmHoldChg(actst, 2);
				else
					ActAffAnmHoldChg(actst, 1);
			}else
				actst->aff_pause = 1;
			break;
		case 3:
			actst->work[3] += 0x0100;	//�O�K�񐔲ݸ����
			
			if((actst->work[3]>>8) == actst->work[7])	//�O�K����
				actst->move = MonsterBallGetFailInit;
			else if(actst->work[7] == SUCCESS_RATE 
					&& (actst->work[3]>>8) == (SUCCESS_RATE-1))
			{
				actst->move = MonsterBallGetSuccessInit;
				actst->aff_pause = 1;
			}
			else{
				actst->work[3]++;
				actst->aff_pause = 1;
			}
//			else
//				ActAffAnmChg(actst, 1);
			break;
		case 4:
		default:
			if(actst->work[5]++ == BALL_GUGA_CENTER_WAIT){
				actst->work[5] = 0;
				actst->work[3] &= 0xff00;
				ActAffAnmChg(actst, 3);		//1��Affine��ؾ�Ă���ׂɕK�v
				if(actst->work[4] < 0)
					ActAffAnmChg(actst, 2);
				else
					ActAffAnmChg(actst, 1);
				SePlay(SE_BOWA);
			}
	}
}

#if 0
static void VoiceWaitPlay(u8 id)
{
	u8 voice_type;
	
	if(TaskTable[id].work[8]++ >= 2){
		voice_type = TaskTable[id].work[2];
		if(voice_type == 0)
			VoicePlay(TaskTable[id].work[0], TaskTable[id].work[1]);
		else if(voice_type == 1){
			if(TaskTable[id].work[10]++ == 0){
				VoicePlayStop();
				return;
			}
			else
				VoicePlay4(TaskTable[id].work[0], TaskTable[id].work[1], 1);
		}
		else{
			if(TaskTable[id].work[10]++ >= 5){
				if(VoiceEndCheck() == 0){
					TaskTable[id].work[9]++;
					if(TaskTable[id].work[9] == 14)
						VoicePlayStop();
					else if(TaskTable[id].work[9] == 15){
						VoicePlay4(TaskTable[id].work[0], TaskTable[id].work[1], 0);
						DelTask(id);
					}
				}
			}
			return;
		}
		DelTask(id);
	}
}
#endif

static void VoiceWaitPlay(u8 id)
{
	u8 voice_type;
	s8 pan;
	u16 monsno;

	voice_type = TaskTable[id].work[2];
	pan = TaskTable[id].work[1];
	monsno = TaskTable[id].work[0];
	
	switch(TaskTable[id].work[15]){
		default:
		case 0:
			if(TaskTable[id].work[8] < 3){
				TaskTable[id].work[8]++;
				break;
			}
			TaskTable[id].work[15] = voice_type+1;
			break;

		case 1:		//ɰ��
			VoicePlay(monsno, pan);
			DelTask(id);
			break;
		
		case 2:		//2vs2SendOut 1�̖�
			VoicePlayStop();
			TaskTable[id].work[10] = 3;
			TaskTable[id].work[15] = 20;
			break;
		case 20:		//2vs2SendOut 1�̖�
			if(TaskTable[id].work[10]){
				TaskTable[id].work[10]--;
				break;
			}
			VoicePlay4(monsno, pan, 1);
			DelTask(id);
			break;
		
		case 3:		//2vs2SendOut 2�̖�
			TaskTable[id].work[10] = 6;
			TaskTable[id].work[15] = 30;
			break;
		case 30:		//2vs2SendOut 2�̖�
			if(TaskTable[id].work[10]){
				TaskTable[id].work[10]--;
				break;
			}
			TaskTable[id].work[15]++;
		case 31:		//2vs2SendOut 2�̖�
			if(VoiceEndCheck() == 0){
				VoicePlayStop();
				TaskTable[id].work[10] = 3;
				TaskTable[id].work[15]++;
			}
			break;
		case 32:		//2vs2SendOut 1�̖�
			if(TaskTable[id].work[10]){
				TaskTable[id].work[10]--;
				break;
			}
			VoicePlay4(monsno, pan, 0);
			DelTask(id);
			break;
		
	}
}

static void MonsterBallGetFailInit(actWork *actst)
{
	u8 client_no;
	PokemonParam *pp;
	s8 pan;
	u16 ballno, tblno;
	u16 monsno;
	u8 voice_type, id;
	
	client_no = actst->work[6];
	
	ActAnmChg(actst, 1);
	ballno = GetBallGet(client_no);
	tblno = BallItemNoTblNoConv(ballno);
	HikariEffectSet(actst->x, actst->y-5, 1, MONS_BALL_PRI-1, tblno);
	actst->work[0] = HikariColorEffectSet(1, actst->work[6], 14, tblno);
	actst->move = MonsterBallGetMiss;
	
	//�ߊl�Ώۂ��߹�� ��ŗv�ύX ��
	if(sys.FieldBattleFlag){
		if(MineEnemyCheck(client_no)){
			pp = &PokeParaEnemy[SelMonsNo[client_no]];
			pan = 25;
		}else{
			pp = &PokeParaMine[SelMonsNo[client_no]];
			pan = -25;
		}
		
		monsno = PokeParaGet(pp, ID_monsno);
		if((client_no == ClientNoGet(BSS_CLIENT_MINE) || client_no == ClientNoGet(BSS_CLIENT_ENEMY)) && FightCheck() && BtlWork->sendout_eff){
			if(FightType&FIGHT_TYPE_4_2vs2){
				if(MusEndCheck())
					m4aMPlayStop(&m4a_mplay000);
			}
			else
				m4aMPlayVolumeControl(&m4a_mplay000,0xffff,128);//1�̖ڂȂ特�ʂ�������
		}
		
		if(FightCheck() == 0 || BtlWork->sendout_eff == 0){
			//VoicePlay(PokeParaGet(pp, ID_monsno), pan);
			voice_type = 0;
		}
		else if(client_no == ClientNoGet(BSS_CLIENT_MINE) || client_no == ClientNoGet(BSS_CLIENT_ENEMY)){
			//VoicePlay3(PokeParaGet(pp, ID_monsno), pan, 1);
			voice_type = 1;
		}
		else{
			//VoicePlay3(PokeParaGet(pp, ID_monsno), pan, 0);
			voice_type = 2;
		}

		id = AddTask(VoiceWaitPlay, 3);
		TaskTable[id].work[0] = monsno;
		TaskTable[id].work[1] = pan;
		TaskTable[id].work[2] = voice_type;
		TaskTable[id].work[15]= 0;		//SeqNo
	}
//	ActWork[PokemonStruct[actst->work[6]]].banish = 0;	GetMiss�ł��悤�ɕύX2002.09.10(��)
	ActAffAnmChg(&ActWork[PokemonStruct[actst->work[6]]], PAF_BALLOUT);
	ActAnm(&ActWork[PokemonStruct[actst->work[6]]]);

	ActWork[PokemonStruct[actst->work[6]]].work[1] = 16 << 8;
}

static void MonsterBallGetSuccessInit(actWork *actst)
{
	actst->anm_pause = 1;
	actst->move = MonsterBallGet;
	actst->work[3] = 0;
	actst->work[4] = 0;
	actst->work[5] = 0;
}

//----------------------------------------------------------
//	���G�̕������C��
//----------------------------------------------------------
void MonsterBallGetMiss(actWork *actst)
{
	u8 aff_flg = 0;
	u8 client_no;
	int i, s;
	
	client_no = actst->work[6];
	ActWork[PokemonStruct[client_no]].banish = 0;
	
	if(actst->anmend_sw == 1)
	{
		actst->banish = 1;
	}
	if(ActWork[PokemonStruct[client_no]].affend_sw == 1)
	{
//		AffineWorkNumDel(ActWork[EnemyStruct].oamData.AffineParamNo);
//		ActWork[EnemyStruct].aff_tbl = DummyActAffTbl;
//		ActWork[EnemyStruct].oamData.AffineMode = 0;
		ActAffAnmChg(&ActWork[PokemonStruct[client_no]], PAF_NORMAL);
		aff_flg++;
	}
	else
	{
		ActWork[PokemonStruct[client_no]].work[1] -= 0x120;
		ActWork[PokemonStruct[client_no]].dy = 
			ActWork[PokemonStruct[client_no]].work[1] >> 8;
	}
	
	if(actst->anmend_sw == 1 && aff_flg)
	{
		ActWork[PokemonStruct[client_no]].dy = 0;
	//	actst->move = DummyActMove;
		ClientEffectWait = 0;
		CliSys[client_no].ball_eff_wait = 0;
		AffineDelActor(actst);	//add 2002.09.20(��) by matsuda
		DelActor(actst);
		for(i = 0, s = 0; i < CLIENT_MAX; i++){
			if(CliSys[i].ball_eff_wait == 0)
				s++;
		}
		if(s == CLIENT_MAX){//Client�S�Ă��ްٴ̪�Ă��I�����Ă���Ȃ�S�����ްق�Del����
			for(i = 0; i < MONS_BALL_MAX; i++)
				MonsBallCellPalDel(i);
		}
	}
}
//----------------------------------------------------------
//	
//----------------------------------------------------------
#define MB_GET_WAIT		40
#define MB_GET_WAIT2	(MB_GET_WAIT+55)
#define MB_GET_SE_WAIT	(MB_GET_WAIT2+220)
void MonsterBallGet(actWork *actst)
{
	u8 client_no;
	
	client_no = actst->work[6];
	actst->work[4]++;

	if(actst->work[4] == MB_GET_WAIT)
	{
//		SePlay(SE_RETURN);
	}
	else if(actst->work[4] == MB_GET_WAIT2)
	{
		//���ClientBit�𗎂Ƃ��Ă��܂� 2002.05.02(��) �]�䕔����Ƌ��c�̏�
		ClientEffectWait = 0;
		m4aMPlayAllStop();
		//JinglePlay(MUS_FANFA5);
		SePlay(MUS_FANFA5);
	}
	else if(actst->work[4] == MB_GET_SE_WAIT)
	{
		//-- �G ��ŏC�� ���b�� --//
		AffineWorkNumDel(ActWork[PokemonStruct[actst->work[6]]].oamData.AffineParamNo);
		DelActor(&ActWork[PokemonStruct[actst->work[6]]]);	//������DelActor���Ă����̂�?
//		actst->move = DummyActMove;
		AllDelActor(actst);
		//ClientEffectWait = 0;
		if(sys.FieldBattleFlag)
			CliSys[client_no].ball_eff_wait = 0;

//		MusicPlay(MUS_KACHI22);
	}
}


//----------------------------------------------------------
//	�����̃|�P�������o�����̃A�N�^�[�V�[�P���X
//----------------------------------------------------------
static void MB_PokeMinePut(actWork *xreg)
{
	xreg->work[0] = 25;	//30;
	xreg->work[2] = GetWazaEffPos(xreg->work[6], PM_X);
	xreg->work[4] = GetWazaEffPos(xreg->work[6], PM_Y) + 24;
	xreg->work[5] = -30;
	xreg->oamData.AffineParam = xreg->work[6];
	YamanariMoveInit(xreg);
	xreg->move = MB_PokeMinePutMain;
}

static void MB_PokeMinePutMain(actWork *xreg)
{
	s16 tempX, tempY, sync;
	u8 onebit, onebit2;
	
	if(((0xff & (xreg->work[7] >> 8)) >= 35) && ((0xff & (xreg->work[7] >> 8)) < 80)){//85)){
//		if((xreg->oamData.AffineParam >> 8) < 20){
//			xreg->oamData.AffineParam += 0x0100;
//			return;
//		}
		if((xreg->oamData.AffineParam & 0xff00) == 0){
			onebit = xreg->work[1] & 1;
			onebit2 = xreg->work[2] & 1;
			tempX = xreg->work[1];
			tempY = xreg->work[2];
			xreg->work[1] = ((xreg->work[1] / 3) & 0xfffe) | onebit;
			xreg->work[2] = ((xreg->work[2] / 3) & 0xfffe) | onebit2;
			ActAffAnmChg(xreg, 4);
		}
		sync = xreg->work[0];
		W_ParallelSync(xreg);
		xreg->work[7] += xreg->work[6] / 3;
		xreg->dy += SinMove(0xff & (xreg->work[7] >> 8), xreg->work[5]);
		xreg->oamData.AffineParam += 0x0100;
		if((xreg->oamData.AffineParam >> 8) % 3 != 0)
			xreg->work[0] = sync;
		else
			xreg->work[0] = sync-1;
		
		if((0xff & (xreg->work[7] >> 8)) >= 80){	//85){
			onebit = xreg->work[1] & 1;
			onebit2 = xreg->work[2] & 1;
			xreg->work[1] = ((xreg->work[1] * 3) & 0xfffe) | onebit;
			xreg->work[2] = ((xreg->work[2] * 3) & 0xfffe) | onebit2;
		}
	}
	else if(YamanariMoveMain(xreg))
//	if(YamanariMoveMain(xreg))	//����else if�ł��R�Ȃ�֘A�g�p!�R�Ȃ�ٰ�ݕς���ꍇ�͒���
	{
		xreg->x += xreg->dx;
		xreg->y += xreg->dy;
		xreg->dx = xreg->dy = 0;
		xreg->work[6] = xreg->oamData.AffineParam & 0x00ff;
		xreg->work[0] = 0;
		if(FightCheck() && BtlWork->sendout_eff && xreg->work[6] == ClientNoGet(BSS_CLIENT_MINE2))
			xreg->move = AppearWait;//SendOut�̎�2�̖ڂ́A1�̖ڂ����Ԋu���󂯂�
		else
			xreg->move = MonsterBallGetFailInit;
		ActAffAnmChg(xreg, 0);
	}
//	else if((xreg->oamData.AffineParam >> 8) == 0 && (xreg->work[7] >> 8) >= 64)
//		xreg->oamData.AffineParam += 0x0100;
}

//----------------------------------------------------------
//	
//----------------------------------------------------------
static void AppearWait(actWork *xreg)
{
	if(xreg->work[0]++ >= 25){
		xreg->work[0] = 0;
		xreg->move = MonsterBallGetFailInit;
	}
}

//----------------------------------------------------------
//	�G�̃|�P�������o�����̃V�[�P���X
//----------------------------------------------------------
static void MB_PokeEnemyPut(actWork *xreg)
{
	if(++xreg->work[0] > 15){
		xreg->work[0] = 0;
		if(FightCheck() && BtlWork->sendout_eff && xreg->work[6] == ClientNoGet(BSS_CLIENT_ENEMY2))
			xreg->move = AppearWait;//SendOut�̎�2�̖ڂ́A1�̖ڂ����Ԋu���󂯂�
		else
			xreg->move = MonsterBallGetFailInit;
	}
}

//----------------------------------------------------------
//	�ݽ���ްق��J�����ɏo���
//----------------------------------------------------------
u8 MBHikariSet(u8 x, u8 y, u8 oam_pri, u8 act_pri)
{
	u8 id;
	
	id = HikariEffectSet(x, y, oam_pri, act_pri, EF_MONSTER_BALL);
	return id;
}

//----------------------------------------------------------
//	�ݽ���ްق��J�������Ɍ��ɂ܂��G�t�F�N�g
// type = �Ώ��߹�݂��ǂ���������̪��ނ����邩(0:���̐F���w��F   1:�w��F�����̐F)
//	haikei_palbit = �ׯ��������w�i����گĂ��w��(PaletteFadeReq��fade_bit�Ɠ���)
//----------------------------------------------------------
#define MB_POKECOLOR	0x7adf
u8 MBHikariColorEffectSet(u8 type, u8 palno, u32 haikei_palbit)
{
	u8 id;
	
	id = HikariColorEffectSet(type, palno, haikei_palbit, EF_MONSTER_BALL);
	return id;
}








//=========================================================================
//	�߹�݁A�ްق���o���̪��
//	����: pokeact = �߹�݂�ActNo	palno = �߹�݂̃p���b�g�ԍ�
//		  xpos = X���W		ypos = Y���W
//		  oam_pri = OAM����ײ��è	act_pri = ������ײ��è
//		  appear_wait = �ްق��J���܂ł̳���
//		  haikei_fadebit = �ׯ��������w�i
//						(�ޯĂŎw��B�w����@��PaletteFadeReq��fade_bit�Ɠ���)
//=========================================================================
void MonsterBallAppearEffSet(u8 pokeact, u8 palno, u8 xpos, u8 ypos,
	 u8 oam_pri, u8 act_pri, u8 appear_wait, u32 haikei_fadebit)
{
	u8 BallNo;
	u8 no;
	
	BallNo = EF_MONSTER_BALL;//(u8)itemNo;

	DecordCellSet((CellData*)&MonsterBallCellTbl[BallNo]);
	DecordPalSet((PalData*)&MonsterBallPalTbl[BallNo]);

	no = AddActor(&MonsterBallHeader[BallNo], xpos, ypos, act_pri);
	ActWork[no].work[0] = pokeact;
	//---------------------
	ActWork[no].work[5] = ActWork[pokeact].x;
	ActWork[no].work[6] = ActWork[pokeact].y;
	ActWork[pokeact].x = xpos;
	ActWork[pokeact].y = ypos;
	//---------------------
	ActWork[no].work[1] = appear_wait;
	ActWork[no].work[2] = palno;
	ActWork[no].work[3] = haikei_fadebit & 0x0000ffff;
	ActWork[no].work[4] = haikei_fadebit >> 16;
	ActWork[no].oamData.Priority = oam_pri;
	ActWork[no].move = MonsBallAppearWait;

	ActWork[pokeact].banish = 1;
//	return no;
}

static void MonsBallAppearWait(actWork *xreg)
{
	u8 pokeact, palno, act_pri;
	u32 haikei_fadebit;
	
	if(xreg->work[1] == 0)
	{
		pokeact = xreg->work[0];
		palno = xreg->work[2];
		haikei_fadebit = (u16)xreg->work[3] | ((u16)xreg->work[4] << 16);
		act_pri = xreg->pri == 0 ? 0:(xreg->pri-1);
		ActAnmChg(xreg, 1);
	//	MBSmokeSet(TEST_ENEMY_X, TEST_ENEMY_Y + 20, 0);
		MBHikariSet(xreg->x, xreg->y-5, xreg->oamData.Priority, act_pri);
		xreg->work[1] = MBHikariColorEffectSet(1, palno, haikei_fadebit);
		xreg->move = MonsterBallAppearEnd;
		//�ߊl�Ώۂ��߹�� ��ŗv�ύX ��
		ActWork[pokeact].banish = 0;
		ActAffAnmChg(&ActWork[pokeact], PAF_BALLOUT);
		ActAnm(&ActWork[pokeact]);

		ActWork[pokeact].work[1] = 16 << 8;
		xreg->work[7] = 0;
	}
	else
		xreg->work[1]--;
}

static void MonsterBallAppearEnd(actWork *xreg)
{
	u8 aff_flg = 0;
	u8 mv_flg = 0;
	u8 pokeact;
	s16 dlen,xlen,ylen;
	
	pokeact = xreg->work[0];
	
	if(xreg->anmend_sw == 1)
		xreg->banish = 1;
	if(ActWork[pokeact].affend_sw == 1)
	{
		ActAffAnmChg(&ActWork[pokeact], PAF_NORMAL);
		aff_flg++;
	}
	xlen = ((xreg->work[5] - xreg->x) * xreg->work[7]/0x80) + xreg->x;
	ylen = ((xreg->work[6] - xreg->y) * xreg->work[7]/0x80) + xreg->y;
	ActWork[pokeact].x = xlen;
	ActWork[pokeact].y = ylen;

	if(xreg->work[7] < 0x80){
		dlen = -((sin_cos_table[(u8)(xreg->work[7])]*0x20/0x100));
		xreg->work[7] += 4;

		ActWork[pokeact].dx = dlen;
		ActWork[pokeact].dy = dlen;
	}else{
		ActWork[pokeact].x	= xreg->work[5];
		ActWork[pokeact].y	= xreg->work[6];
		ActWork[pokeact].dx = 0;
		ActWork[pokeact].dy = 0;
		mv_flg++;
	}
	if(xreg->anmend_sw == 1 && aff_flg && mv_flg)
	{
//		ActWork[PokemonStruct[xreg->work[6]]].dy = 0;
//		xreg->move = DummyActMove;
		AllDelActor(xreg);
	}
}












//=========================================================================
//	�߹�݁A�ްقɈ������޴̪��
//	����: pokeact = �߹�݂�ActNo	palno = �߹�݂̃p���b�g�ԍ�
//		  xpos = X���W		ypos = Y���W
//		  oam_pri = OAM����ײ��è	act_pri = ������ײ��è
//		  appear_wait = �ްق��J���܂ł̳���
//		  haikei_fadebit = �ׯ��������w�i
//						(�ޯĂŎw��B�w����@��PaletteFadeReq��fade_bit�Ɠ���)
//
//	�߂�l: �ްق�ActNo
//=========================================================================
u8 MonsterBallReturnEffSet(u8 pokeact, u8 palno, u8 xpos, u8 ypos,
	 u8 oam_pri, u8 act_pri, u8 appear_wait, u32 haikei_fadebit)
{
	u8 BallNo;
	u8 no;
	
	BallNo = 0;//(u8)itemNo;

	DecordCellSet((CellData*)&MonsterBallCellTbl[BallNo]);
	DecordPalSet((PalData*)&MonsterBallPalTbl[BallNo]);

	no = AddActor(&MonsterBallHeader[BallNo], xpos, ypos, act_pri);
	ActWork[no].work[0] = pokeact;
	ActWork[no].work[1] = appear_wait;
	ActWork[no].work[2] = palno;
	ActWork[no].work[3] = haikei_fadebit & 0x0000ffff;
	ActWork[no].work[4] = haikei_fadebit >> 16;
	ActWork[no].oamData.Priority = oam_pri;
	ActWork[no].move = MonsBallReturnWait;

//	ActWork[pokeact].banish = 1;
	return no;
}

static void MonsBallReturnWait(actWork *xreg)
{
	u8 pokeact, palno, act_pri;
	u32 haikei_fadebit;
	
	if(xreg->work[1] == 0)
	{
		pokeact = xreg->work[0];
		palno = xreg->work[2];
		haikei_fadebit = (u16)xreg->work[3] | ((u16)xreg->work[4] << 16);
		act_pri = xreg->pri == 0 ? 0:(xreg->pri-1);
		ActAnmChg(xreg, 1);
	//	MBSmokeSet(TEST_ENEMY_X, TEST_ENEMY_Y + 20, 0);
		MBHikariSet(xreg->x, xreg->y-5, xreg->oamData.Priority, act_pri);
		xreg->work[1] = MBHikariColorEffectSet(1, palno, haikei_fadebit);
		xreg->move = MonsterBallReturnClose;

		//�|�P����
		ActAffAnmChg(&ActWork[pokeact], PAF_BALLIN);
		ActAnm(&ActWork[pokeact]);
		ActWork[pokeact].work[1] = 0;
	}
	else
		xreg->work[1]--;
}

static void MonsterBallReturnClose(actWork *actst)
{
	u8 pokeact;
	
	if(actst->work[5]++ == 10)
		SePlay(SE_SUIKOMU);
	
	pokeact = actst->work[0];
	
	if(ActWork[pokeact].affend_sw)
	{
		ActAnmChg(actst, 2);

		ActWork[pokeact].banish = 1;	//��
		actst->work[5] = 0;
		actst->move = MonsterBallReturnEnd;
	}
	else
	{
		ActWork[pokeact].work[1] += 0x0060;
		ActWork[pokeact].dy = 
			-ActWork[pokeact].work[1] >> 8;
	}
}

static void MonsterBallReturnEnd(actWork *actst)
{
	if(actst->anmend_sw == 0)
		return;
	actst->move = DummyActMove;
}


//----------------------------------------------------------
//	�ްق�Cell,Pal,������Del����
//----------------------------------------------------------
void MonsterBallCellPalDel(actWork *xreg)
{
	AllDelActor(xreg);
}










//=========================================================================
//	�ް�ޏo���G�t�F�N�g����
//=========================================================================
void GaugeAppearEffect(u8 client_no)
{
	actWork *Xreg = &ActWork[ClientGaugeSt[client_no]];
//	actWork *Xreg2 = &ActWork[Xreg->work[7]];
	
	Xreg->work[0] = GAE_MOVE_SP;
	Xreg->work[1] = 0;
	Xreg->dx = GAE_DX_DY;
	Xreg->dy = 0;
	Xreg->move = GAE_Move;
	/*
	Xreg2->work[0] = 0;
	Xreg2->work[1] = GAE_MOVE_SP;
	Xreg2->dx = 0;
	Xreg2->dy = GAE_DX_DY;
	Xreg2->move = GAE_Move;
	*/
	if(MineEnemyCheck(client_no))
	{
		Xreg->work[0] = -Xreg->work[0];
		Xreg->work[1] = -Xreg->work[1];
		Xreg->dx = -Xreg->dx;
		Xreg->dy = -Xreg->dy;
		/*
		Xreg2->work[0] = -Xreg2->work[0];
		Xreg2->work[1] = -Xreg2->work[1];
		Xreg2->dx = -Xreg2->dx;
		Xreg2->dy = -Xreg2->dy;
		*/
	}
	
	//�߰̓���֐����s(���W�ݒu�x������̂���)
	ActWork[Xreg->work[5]].move(&ActWork[Xreg->work[5]]);
	
	if(ClientTypeGet(client_no) == BSS_CLIENT_MINE2)
		Xreg->move = GAE_Wait;
}
//----------------------------------------------------------
//	�A�N�^�[�V�[�P���X
//----------------------------------------------------------
void GAE_Wait(actWork *Xreg)	//�f�o�b�N�p�Ɉꎞ�I�ɖ����ް�ނ�����ďo�Ă���悤�ɂ���
{
	Xreg->work[1]++;
	if(Xreg->work[1] == 20)
	{
		Xreg->work[1] = 0;
		Xreg->move = GAE_Move;
	}
}

void GAE_Move(actWork *Xreg)
{
	Xreg->dx -= Xreg->work[0];
	Xreg->dy -= Xreg->work[1];
	if(Xreg->dx == 0 && Xreg->dy == 0)
		Xreg->move = DummyActMove;
}




//=========================================================================
//	�_���[�W���󂯂����̹ް�ޗh�炵�G�t�F�N�g
//=========================================================================
void GaugeSwayEffect(u8 client_no)
{
	u8 actno;
	
	actno = AddBanishActor(GaugeSwayMain);
	
	//-- �ް�ޗh�炵 --//
	ActWork[actno].work[0] = 1;
	ActWork[actno].work[1] = ClientGaugeSt[client_no];
	ActWork[actno].move = GaugeSwayMain;
}

static void GaugeSwayMain(actWork *xreg)
{
	u8 gauge;
	
	gauge = xreg->work[1];
	
	ActWork[gauge].dy = xreg->work[0];
	xreg->work[0] = -xreg->work[0];
//	if(	ClientEffectWait == 0){
	if(xreg->work[2]++ == 20){
		ActWork[gauge].dx = 0;
		ActWork[gauge].dy = 0;
		DelActor(xreg);
	}
}









//=========================================================================
//	�����X�^�[�{�[��	2002.07.16(��)  by matsuda
//=========================================================================
//----------------------------------------------------------
//	�w�肵���ݽ���ްق�Cell����گĂ�Ă���(2�d�o�^��������)
//----------------------------------------------------------
void MonsBallCellPalSet(u8 tblno)
{
	u16 charno;
	
	if(CellCharNumGet(MonsterBallCellTbl[tblno].act_num) == 0xffff){
		DecordCellSet((CellData*)&MonsterBallCellTbl[tblno]);
		DecordPalSet((PalData*)&MonsterBallPalTbl[tblno]);
	}
	
	switch(tblno){
		case EF_DIVE_BALL:		//3����ݖڂ��Ǝ��ɂ��鷬ׂ���
		case EF_GOUJYASU_BALL:
		case EF_PREMIA_BALL:
			break;
		default:	//3����ݖڂ����ʷ�ׂ��g�p���鷬ׂ���
			charno = CellCharNumGet(MonsterBallCellTbl[tblno].act_num);
			DecordVram((void*)b_anix_pch_ADRS, (void*)OBJ_MODE0_VRAM+(charno+2*4)*32);
			break;
	}
}

//----------------------------------------------------------
//	�w�肵���ݽ���ްق�Cell����گĂ�Del����
//----------------------------------------------------------
void MonsBallCellPalDel(u8 tblno)
{
	CellDel(MonsterBallCellTbl[tblno].act_num);
	ObjPalRegNumDel(MonsterBallPalTbl[tblno].reg_num);
}






//=========================================================================
//	�w�肳�ꂽ�N���C�A���g�̃|�P������get_ball���擾����
//=========================================================================
static u16 GetBallGet(u8 client_no)
{
	u16 get_ball;
	
	if(MineEnemyCheck(client_no))
		get_ball = PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_get_ball);
	else
		get_ball = PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_get_ball);
	return get_ball;
}

