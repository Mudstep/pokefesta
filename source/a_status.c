//=========================================================================
//	
//	�X�e�[�^�X�ُ�G�t�F�N�g
//													by matsuda
//=========================================================================
#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "madefine.h"

#include "wazatool.h"
#include "waza_dat.h"
#include "server.h"
#include "bss.h"
#include "waza_eff.h"
#include "fight.h"
#include "palanm.h"
#include "client.h"

#include "decord.h"
#include "calctool.h"
#include "poketool.h"
#include "task.h"
#include "cli_def.h"
#include "ef_tool.h"
#include "a_status.h"


//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void StatusChangeEffectMain(u8 id);
static void StatusHikariSeq(actWork *Xreg);
static void StatusHikariPutWaitSeq(actWork *Xreg);
static void KooriStatusBldInit(u8 id);
static void KooriStatusBldWait(u8 id);
static void KooriStatusBldEnd(u8 id);
static void KooriStatusEnd(u8 id);

//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
extern const CellData WazaEffDataCell[];
extern const PalData WazaEffDataPal[];

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define ST_EFF_PRI		4	//�ð���ُ�̪�Ă̱�����ײ��è
//�ǂ�
#define DOKU_FURIHABA	5
#define DOKU_SEC		3
#define DOKU_DY_SP		0x90
#define DOKU_DY_MAX		(-32)
#define DOKU_WAIT		40
//�˂ނ�
#define NEMURI_FURIHABA	5
#define NEMURI_SEC		3
#define NEMURI_DY_SP	0x90
#define NEMURI_DY_MAX	(-32)
#define NEMURI_X_OFFSET	25
#define NEMURI_WAIT		42
//�܂�
#define MAHI_NEXT_WAIT	4	//�܂Ђ̱��(�p�x)��ς���܂ł̳���
#define MAHI_NEXT_COUNT	2	//�����҂������玟�̏ꏊ�Ɉڂ邩
#define MAHI_WAIT		34	//�������Ȃ��ł��鎞��
#define MAHI_DX0		(-10)
#define MAHI_DY0		(-10)
#define MAHI_DX1		(25)
#define MAHI_DY1		(25)
#define MAHI_DX2		(-10)
#define MAHI_DY2		25
#define MAHI_DX3		25
#define MAHI_DY3		(-10)
//�₯��
#define YAKEDO_WAIT		37

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
//������
static const OAMPatData KooriStatusPatData[] =
{
	{-16, -16, 0, 3, 0, 2},	//�������S�����Xoffs,Yoffs, Shape,Size,CharNoOffs, pri
	{-16, 48, 1, 3, 64, 2},
	{48, -16, 2, 3, 96, 2},
	{48, 48, 0, 2, 128, 2},
};
static const OAMPatSt KooriStatusPat = {4, KooriStatusPatData};

const actHeader ActKooriHeader={
	WAZACHR_ID_KOORI,
	WAZACHR_ID_KOORI,
	&WazaOamDataBldNormal[3],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
//	SeqKooriStatusBldInit,
	DummyActMove,			// 2002.07.17(��)
};








//=========================================================================
//
//		�\�̓A�b�v�E�_�E�� �G�t�F�N�g
//												by matsuda 2002.01.09(��)
//=========================================================================
#define ST_HIKARI_MAX	5
#define ST_HIKARI_SP	0x0100	//�Œ菭��
#define ST_HIKARI_BYOUSP	8
#define ST_HIKARI_LOOP	((52 << 8) / ST_HIKARI_SP)	//((64 << 8) / ST_HIKARI_SP)
#define ST_HIKARI_FURIHABA_X	32
#define ST_HIKARI_FURIHABA_Y	8
#define ST_HIKARI_PUTWAIT	21
#define ST_COLOR_LOOP	2
#define ST_COLOR_WAIT	2
const actHeader StatusHikariHeader={
	WAZACHR_ID_KOUKA,
	WAZACHR_ID_KOUKA,
	&WazaOamDataNormal[0],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	StatusHikariPutWaitSeq,
};
//----------------------------------------------------------
//	up_down = 1:�\�̓A�b�v	0:�\�̓_�E��
//----------------------------------------------------------
u8 StatusChangeEffect(u8 client_no, u8 up_down)
{
	u8 i, pokeact, actno, id;
	
	pokeact = PokemonStruct[client_no];
	id = AddTask(StatusChangeEffectMain, 10);
	
	DecordCellSet((CellData*)&WazaEffDataCell[WAZACHR_ID_KOUKA - WAZACHR_ID_FAST_NUMBER]);
	DecordPalSet((PalData*)&WazaEffDataPal[WAZACHR_ID_KOUKA - WAZACHR_ID_FAST_NUMBER]);

	TaskTable[id].work[0] = client_no;	//�p���b�g�ԍ�
	if(up_down)
	{	//�\�̓A�b�v
		TaskTable[id].work[1] = 0x1f;	//�ω���̐F
		for(i = 0; i < ST_HIKARI_MAX * 2; i++)
		{
			actno = AddActor(&StatusHikariHeader, ActWork[pokeact].x, 
				ActWork[pokeact].y+32, 0);
			ActWork[actno].work[0] = i * (255/ST_HIKARI_MAX);	//�b
			ActWork[actno].work[1] = -ST_HIKARI_SP;				//���x
			ActWork[actno].banish = 1;
			if(i >= ST_HIKARI_MAX)
				ActWork[actno].work[6] = ST_HIKARI_PUTWAIT;	//�x��ďo����
		}
	}
	else
	{	//�\�̓_�E��
		TaskTable[id].work[1] = 0x7c00;
		for(i = 0; i < ST_HIKARI_MAX * 2; i++)
		{
			actno = AddActor(&StatusHikariHeader, ActWork[pokeact].x, 
				ActWork[pokeact].y-32, 0);
			ActWork[actno].work[0] = i * (255/ST_HIKARI_MAX);
			ActWork[actno].work[1] = ST_HIKARI_SP;
			ActWork[actno].banish = 1;
			if(i >= ST_HIKARI_MAX)
				ActWork[actno].work[6] = ST_HIKARI_PUTWAIT;	//�x��ďo����
		}
	}
	ActWork[actno].work[7] = 1;	//�Ō�ɓo�^���ꂽ����(�e�Ƃ���)�Ɉ�����Ƃ�
	
	return id;
}

static void StatusChangeEffectMain(u8 id)
{
	if(TaskTable[id].work[2] == ST_COLOR_WAIT)
	{
		TaskTable[id].work[2] = 0;
		
		SoftFade(16*16 + TaskTable[id].work[0]*16,16,
			TaskTable[id].work[4], TaskTable[id].work[1]);
		if(TaskTable[id].work[5] == 0)
		{
			if(++TaskTable[id].work[4] > 8)
			{
				//TaskTable[id].work[4]--;
				TaskTable[id].work[5] ^= 1;
			}
		}
		else
		{
			if(--TaskTable[id].work[4] < 0)
			{
				TaskTable[id].work[4]++;
				TaskTable[id].work[5] ^= 1;
				if(++TaskTable[id].work[3] == ST_COLOR_LOOP)
					DelTask(id);
			}
		}
	}
	else
		TaskTable[id].work[2]++;
}

static void StatusHikariPutWaitSeq(actWork *Xreg)
{
	if(Xreg->work[6] == 0)
	{
		Xreg->banish = 0;
		Xreg->move = StatusHikariSeq;
		Xreg->move(Xreg);
	}
	else
		Xreg->work[6]--;
}

static void StatusHikariSeq(actWork *Xreg)
{
	Xreg->dx = CosMove(Xreg->work[0], ST_HIKARI_FURIHABA_X);
	Xreg->dy = SinMove(Xreg->work[0], ST_HIKARI_FURIHABA_Y);
	Xreg->work[0] < 128 ? (Xreg->pri = C_POKE1PRI-1):(Xreg->pri = C_POKE1PRI+1);

	Xreg->work[0] = 0xff & (Xreg->work[0] + ST_HIKARI_BYOUSP);
	Xreg->work[5] += Xreg->work[1];
	Xreg->dy += Xreg->work[5] >> 8;
	
	if(++Xreg->work[2] == ST_HIKARI_LOOP)
	{
		if(Xreg->work[7])	//�Ō�ɓo�^���ꂽ�����Ȃ�Έꏏ�ɃZ���A��گĂ��J������
			AllDelActor(Xreg);
		else
			DelActor(Xreg);
	}
}









//=========================================================================
//
//		�X�G�t�F�N�g	west.s����Ă΂��
//												by matsuda 2002.07.17(��)
//=========================================================================
//----------------------------------------------------------
//	�u������v
//----------------------------------------------------------
void KooriStatusEffectTask(u8 id)
{
	s16 xpos, ypos;
	u8 actno;
	
	xpos = GetWazaEffPos(DefenceNo, PM_X) - 32;
	ypos = GetWazaEffPos(DefenceNo, PM_Y) - 36;//32;
	if(ContestCheck())
		xpos -= 6;	//�R���ɔ��Ȃ��悤��

	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL;
	*(vu16*)REG_BLDALPHA = 0x0 | (16<<8);

	actno = AddActor(&ActKooriHeader, xpos, ypos, ST_EFF_PRI);
//	ActWork[actno].work[0] = client_no;
	SetActOamPat(&ActWork[actno], &KooriStatusPat);
	
	TaskTable[id].work[15] = actno;
	TaskTable[id].TaskAdrs = KooriStatusBldInit;
}

#define KOORI_PALANM_WAIT	13//25
#define KOORI_PALANM_CHG	3
#define KOORI_BLD			9	//(9*2)
#define KOORI_PALANM_ENDWAIT	(37)
//----------------------------------------------------------
//	�������Ńt�F�[�h�C��������
//----------------------------------------------------------
static void KooriStatusBldInit(u8 id)
{
	u8 ev;
	
	if(TaskTable[id].work[1]++ == KOORI_BLD)
	{
		TaskTable[id].TaskAdrs = KooriStatusBldWait;
		TaskTable[id].work[1] = 0;
	}
	else
	{
		ev = TaskTable[id].work[1];// / 2;
		*(vu16*)REG_BLDALPHA = ev | ((16-ev)<<8);
	}
}

static void KooriStatusBldWait(u8 id)
{
	u8 palno;
	u16 color;
	
	palno = ObjPalNumGet(WAZACHR_ID_KOORI);
	if(TaskTable[id].work[1]++ > KOORI_PALANM_WAIT)
	{
		if(++TaskTable[id].work[2] == KOORI_PALANM_CHG)
		{
			color = PaletteWorkTrans[16*16+16*palno+0xd];
			PaletteWorkTrans[16*16+16*palno+0xd] = PaletteWorkTrans[16*16+16*palno+0xe];
			PaletteWorkTrans[16*16+16*palno+0xe] = PaletteWorkTrans[16*16+16*palno+0xf];
			PaletteWorkTrans[16*16+16*palno+0xf] = color;
			TaskTable[id].work[2] = 0;
			if(++TaskTable[id].work[3] == 3)
			{
				TaskTable[id].work[3] = 0;
				TaskTable[id].work[1] = 0;
				if(++TaskTable[id].work[4] == 2)
				{
					TaskTable[id].work[1] = KOORI_BLD;
					TaskTable[id].TaskAdrs = KooriStatusBldEnd;
				}
			}
		}
	}
}

static void KooriStatusBldEnd(u8 id)
{
	u8 ev;
	
	if(TaskTable[id].work[1]-- == 0)
	{
		TaskTable[id].TaskAdrs = KooriStatusEnd;
		TaskTable[id].work[1] = 0;
	}
	else
	{
		ev = TaskTable[id].work[1];// / 2;
		*(vu16*)REG_BLDALPHA = ev | ((16-ev)<<8);
	}
}

static void KooriStatusEnd(u8 id)
{
	u8 client_no;
	u8 actno;
	
	client_no = DefenceNo;
	TaskTable[id].work[1]++;
	if(TaskTable[id].work[1] == KOORI_PALANM_ENDWAIT)
	{
		actno = TaskTable[id].work[15];
		AffineDelActor(&ActWork[actno]);
		DelActor(&ActWork[actno]);
	}
	else if(TaskTable[id].work[1] == KOORI_PALANM_ENDWAIT+2){
		*(vu16*)REG_BLDCNT = 0;
		*(vu16*)REG_BLDALPHA = 0;
		DelTaskEffect(id);
		//CliSys[client_no].status_eff = 0;
	}
}










//=========================================================================
//
//		�X�e�[�^�X�㏸�E���~�G�t�F�N�g	west.s����Ă΂��
//												by matsuda 2002.07.18(��)
//=========================================================================
enum{
	KOUGEKI = 0,
	BOUGYO,
	MEITYUU,
	SPEED,
	KAIHI,
	TOKUKOU,
	TOKUBOU,
	FUKUGOU = 0xff,
};
enum{
	UP,
	DOWN,
};

extern void StatusEffectTask(u8 id);

void FeStatusEffectTask(u8 id)
{
	u8 updown, type, dankai = 0;
//	u8 target;
	
	switch(BtlWork->work[0]){
		case COND2_POWUP:				//�U���̓A�b�v
			updown = UP;	type = KOUGEKI;
			break;
		case COND2_DEFUP:				//�h��̓A�b�v
			updown = UP;	type = BOUGYO;
			break;
		case COND2_AGIUP:				//�f�����A�b�v
			updown = UP;	type = SPEED;
			break;
		case COND2_SPEPOWUP:			//���U�A�b�v
			updown = UP;	type = TOKUKOU;
			break;
		case COND2_SPEDEFUP:			//���h�A�b�v
			updown = UP;	type = TOKUBOU;
			break;
		case COND2_HITUP:				//�������A�b�v
			updown = UP;	type = MEITYUU;
			break;
		case COND2_AVOIDUP:				//��𗦃A�b�v
			updown = UP;	type = KAIHI;
			break;
		
		case COND2_POWDOWN:				//�U���̓_�E��
			updown = DOWN;	type = KOUGEKI;
			break;
		case COND2_DEFDOWN:				//�h��̓_�E��
			updown = DOWN;	type = BOUGYO;
			break;
		case COND2_AGIDOWN:				//�f�����_�E��
			updown = DOWN;	type = SPEED;
			break;
		case COND2_SPEPOWDOWN:			//���U�_�E��
			updown = DOWN;	type = TOKUKOU;
			break;
		case COND2_SPEDEFDOWN:			//���h�_�E��
			updown = DOWN;	type = TOKUBOU;
			break;
		case COND2_HITDOWN:				//�������_�E��
			updown = DOWN;	type = MEITYUU;
			break;
		case COND2_AVOIDDOWN:			//��𗦃_�E��
			updown = DOWN;	type = KAIHI;
			break;
			
		case COND2_POWUP2:				//�U���̓A�b�v(2�i�K)
			updown = UP;	type = KOUGEKI;	dankai = 1;
			break;
		case COND2_DEFUP2:				//�h��̓A�b�v(2�i�K)
			updown = UP;	type = BOUGYO;	dankai = 1;
			break;
		case COND2_AGIUP2:				//�f�����A�b�v(2�i�K)
			updown = UP;	type = SPEED;	dankai = 1;
			break;
		case COND2_SPEPOWUP2:			//���U�A�b�v(2�i�K)
			updown = UP;	type = TOKUKOU;	dankai = 1;
			break;
		case COND2_SPEDEFUP2:			//���h�A�b�v(2�i�K)
			updown = UP;	type = TOKUBOU;	dankai = 1;
			break;
		case COND2_HITUP2:				//�������A�b�v(2�i�K)
			updown = UP;	type = MEITYUU;	dankai = 1;
			break;
		case COND2_AVOIDUP2:				//��𗦃A�b�v(2�i�K)
			updown = UP;	type = KAIHI;	dankai = 1;
			break;

		case COND2_POWDOWN2:				//�U���̓_�E��
			updown = DOWN;	type = KOUGEKI;	dankai = 1;
			break;
		case COND2_DEFDOWN2:				//�h��̓_�E��
			updown = DOWN;	type = BOUGYO;	dankai = 1;
			break;
		case COND2_AGIDOWN2:				//�f�����_�E��
			updown = DOWN;	type = SPEED;	dankai = 1;
			break;
		case COND2_SPEPOWDOWN2:			//���U�_�E��
			updown = DOWN;	type = TOKUKOU;	dankai = 1;
			break;
		case COND2_SPEDEFDOWN2:			//���h�_�E��
			updown = DOWN;	type = TOKUBOU;	dankai = 1;
			break;
		case COND2_HITDOWN2:				//�������_�E��
			updown = DOWN;	type = MEITYUU;	dankai = 1;
			break;
		case COND2_AVOIDDOWN2:			//��𗦃_�E��
			updown = DOWN;	type = KAIHI;	dankai = 1;
			break;
		
		//����
//		case COND2_GENSHINOCHIKARA:
//			updown = UP;	type = FUKUGOU;	dankai = 0;
//			break;
		case COND2_MULTIUP:
			updown = UP;	type = FUKUGOU;	dankai = 0;
			break;
		case COND2_MULTIUP2:
			updown = UP;	type = FUKUGOU;	dankai = 1;
			break;
		case COND2_MULTIDOWN:
			updown = DOWN;	type = FUKUGOU;	dankai = 0;
			break;
		case COND2_MULTIDOWN2:
			updown = DOWN;	type = FUKUGOU;	dankai = 1;
			break;
		default:
			DelTaskEffect(id);
			return;
	}

	WazaEffWork[0] = updown;
	WazaEffWork[1] = type;
	WazaEffWork[2] = 0;
	WazaEffWork[3] = 0;
	WazaEffWork[4] = dankai;
	TaskTable[id].TaskAdrs = StatusEffectTask;
	TaskTable[id].TaskAdrs(id);
}







//=========================================================================
//
//		���ŋZ�G�t�F�N�g�̒ǉ����ʃG�t�F�N�g�Ɠ������̂����Ă�
//												by matsuda 2002.05.30(��)
//=========================================================================
extern u32 StatusEffSeqTbl[];

static void StatusEffectEndCheck(u8 id);

void SetStatusEffect(u8 client_no, u8 condition_no)
{
	u8 id;
	
	AttackNo = client_no;
	DefenceNo = client_no;
	
	WazaEffectCommonSet(StatusEffSeqTbl, condition_no, 0);
	id = AddTask(StatusEffectEndCheck, 10);
	TaskTable[id].work[0] = client_no;
}

static void StatusEffectEndCheck(u8 id)
{
	WazaEffectSequence();
	if(WazaEffectFlag == 0){
		CliSys[TaskTable[id].work[0]].status_eff = 0;
		DelTask(id);
	}
}

