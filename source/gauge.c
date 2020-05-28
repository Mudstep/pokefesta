//----------------------------------------------------------
//
//	���C�t�o�[�v�Z��\��				by matsuda
//
//----------------------------------------------------------
#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "madefine.h"
#include "ef_tool.h"
#include "calctool.h"
#include "gauge.h"

#include "cli_def.h"
#include "decord.h"
#include "cli_num.h"
#include "client_t.h"
#include "print.h"
#include "poketool.h"
#include "client.h"
#include "bss.h"
#include "fight.h"
#include "zukan.h"
#include "server.h"
#include "d_matsu.h"
#include "task.h"

#include "wazatool.h"		//add by soga 2001.09.26
#include "trainer.h"
#include "param.h"
#include "ev_fight.h"
#include "p_status.h"
#include "fightmsg.h"
#include "safari.h"
#include "palanm.h"
#include "monsno.def"
#include "mus_tool.h"

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
//s16 GaugeBG(GaugeDataBG *gdata, u16 *HP_Work, u16 *TransPos, u16 *pl_buf);
s16 GaugeBG(GaugeDataBG *gdata, s32 *HP_Work, u16 *TransPos, u16 *pl_buf);
//static void GaugeProcSub(u16 *HP_Work);
//void PutGaugeBG(s16 MaxHP, s16 AfterHP, s16 resultHP, u16 *TransPos, u16 TopChr);
//void PutGaugeBG(GaugeDataBG *gdata, s16 AfterHP, s16 resultHP, u16 *TransPos, u8 Bunsi);
static void PutGaugeBG(GaugeDataBG *gdata, s32 *HP_Work, u16 *TransPos);
static u8 GaugeColChk(s16 MaxHP, s16 resultHP, u16 *pl_buf);
//void PutHPNumber(s16 MaxHP, s16 resultHP, u16 *TransPos);
//void PutNumber(s16 number, u16 *TransPos, u8 yose);

//static void PutGaugeOBJ(GaugeDataOBJ *gdata, GaugeDataOBJ2 *gdata2, u8 Bunsi, u8);
//static s8 PutGaugeProc(GaugeDataOBJ *gdata, GaugeDataOBJ2 *gdata2, u8,u8 *, u8);
//static void GaugeProc(GaugeDataOBJ *gdata, GaugeDataOBJ2 *gdata2, s8 Direction);
//void PutGaugeOBJ(GaugeDataOBJ *gdata, u8 req);
void PutGaugeOBJ(u8 , u8);

//static u8 PutGaugeProc(s16 MaxHP, s16 NowHP, s16 beHP, s32 *HP_Work,
//	 u8 *gauge_chr0, u8 GaugeMax);
//static s16 GaugeProc(s16 MaxHP, s16 NowHP, s16 beHP, s32 *HP_Work, u8 GaugeMax, u16);
static u8 PutGaugeProc(s32 MaxHP, s32 NowHP, s32 beHP, s32 *HP_Work,
	 u8 *gauge_chr, u8 GaugeMax);
static s32 GaugeProc(s32 MaxHP, s32 NowHP, s32 beHP, s32 *HP_Work, u8 GaugeMax, u16 add_dec);

//static void PutGaugePokeList(GaugeDataOBJ *gdata, GaugeDataOBJ2 *gdata2, u8 Bunsi);
//static void ArrowTipSeq(actWork *Xreg);
static void GaugePartsSeq(actWork *Xreg);
static void GaugeSubSeq(actWork *Xreg);
u8 *GetGaugePartsAdrs(u8 parts_id);
//s16 GaugeOBJ(u8 actno, u8 req_data, u16 *pl_buf);
s32 GaugeOBJ(u8 client_no, u8 actno, u8 req_data, u16 *pl_buf);
//static u8 DottoOffsetCalc(s16 nowHP, s16 beHP, s16 MaxHP, u8 GaugeMax);
static u8 DottoOffsetCalc(s32 nowHP, s32 beHP, s32 MaxHP, u8 GaugeMax);
void TemotiBallOff(u8 id);
static void TemotiGaugeSeq(actWork *xreg);
static void TemotiBallSeq(actWork *xreg);
static u8 GetCondID(u8 cond_id, u8 client_no);
void GaugeHPTrans2vs2(u8 actno, s16 hp, u8 flg);
void GaugeStatusTrans(u8 actno);
static void TemotiBallOffMain(u8 id);
static void TemotiBallOutSeq(actWork *xreg);
static void TemotiGaugeOutSeq(actWork *xreg);
static void TemotiBallOffSyokai(u8 id);
static void TemotiBallOffSyokaiEnd(u8 id);
static void TemotiBallNormalSeq(actWork *xreg);

extern	TALK_WIN	FightMsg;
//extern	u8	FightMsgBuf[64];
//extern	const	u8	PokeNameData[][6];
extern int BGNo;	//���b��
extern const u32 GrowTbl[][101];

//----------------------------------------------------------
//	�T�u���[�`��
//----------------------------------------------------------

//----------------------------------------------------------
//	�萔��`
//----------------------------------------------------------
#define DEBUG_DAKUTEN	0//8		//���b��  8��ו��i�߂邱�ƂŖ��O�̕����̉��ɉB��
#define GAUGE_SOKUDO_TYPE	1	//�ް�ނ̓�������(0:�������Ă����A1:��Ɉ�葬�x)

//-------------------- �ް�ނ�į�߷�����ް ----------------------------------
#define G_NUM_TOPCHR	20		//����į�߷��(0,1,2,3,4,5,6,7,8,9 �̏��ŕ���ł邱��)
#define G_SPACE_CHR		30		//��߰�

//----------------------- �G --------------------------------
#define GAUGE_DOTTO		8	//�ް��1���8�ޯ�  ��8�ޯČŒ�
#define GAUGE_MAX		6//8	//�ް�޷�׸���� ���[�̊G�̕���(2��ו�)����
#define GAUGE_PARSENT	(GAUGE_DOTTO * GAUGE_MAX)
#define GAUGE_DOTTOMAX	(GAUGE_DOTTO * GAUGE_MAX)

//--------------------- ���� -------------------------
#define DEFAULT_DOWN_PARAM	7//12		//�ŏ��̉����� (MAX:15) ���ް�ނ̐i�޽�߰��
#define KAISOU				DEFAULT_DOWN_PARAM	//���i�K��������
					//��(DEFAULT_DOWN_PARAM�ȊO�̒l���w��ł��邪�A���������̕����Ȃ߂炩)
#define DOWN_BUNSI_PARAM	1	//1�x�ɉ����Ă���������
#define DEFAULT_BUNBO		255	//�������傫���قǹް�ނ����߂��܂�������(MAX:255)
#define FAST_END			120	//���q�����̐����ɒB����ƽ�߰�ނ������Ă���

#define NEXT_BUNSI1			(DEFAULT_BUNBO - FAST_END)	//���񂩂玟�Ɉڂ�܂ł̎���
#define BUNSI_HABA			(FAST_END / KAISOU)	//��߰�ނ������Ă����Ƃ��̕��q��

//----------------------- �F --------------------------------
#define COLOR_POS		159	//�ް�ނŎg�p���Ă�����گĈʒu (0�`255)
#define NORMAL_PARSENT	50	// �߰��ð�ނŋL�� 45�Ə����Ă����45%�ȏゾ��NORMAL���
#define PINCH_PARSENT	25
#define DANGER_PARSENT	0

#define NORMAL_COLOR	0x7c00	//HP��NORMAL_PARSENT�ȏ�̎��̶װ�ް�
#define PINCH_COLOR		0x03e0
#define DANGER_COLOR	0x001f

#define ARROW_NEXT_WAIT		40
#define ARROW_NEXT_WAIT2	(ARROW_NEXT_WAIT + 20)

//----------------------------------------------------------
//	�N�̹ް���߰�
//----------------------------------------------------------
#define MINE_PARTS_1VS1		0
#define MINE_PARTS_2VS2		1
#define ENEMY_PARTS			2

//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
//#include "..\scaddata\gauge5b.ach"
#include "gauge.dat"



//==================================================================
//	�Q�[�W�̐F���������ĕύX
// ���� : 	gdata->MaxHP:�ő�HP	AfterHP:��Ұ�ތ�(HP�̕ύX���󂯂���)��HP
// �߂�l : ��گ�No(0�`15)
//==================================================================
static u8 GaugeColChk(s16 MaxHP, s16 resultHP, u16 *pl_buf)
{
	s16 HP_power;
	u16 color;
	
	HP_power = 100 * (resultHP) / MaxHP;	//resultHP�����L����HP�̊���
	
	if(NORMAL_PARSENT < HP_power)
	{
		color = NORMAL_COLOR;
	}
	else if(PINCH_PARSENT < HP_power)
	{
		color = PINCH_COLOR;
	}
	else
	{
		color = DANGER_COLOR;
	}
	
	//SoftFade(pl_buf, 0x0000, COLOR_POS, 1, 16, color);
	return (COLOR_POS >> 4);
}


//==================================================================
//	�����\��
// ���� : number : �\�����鐔��(BUFF_MAX���܂�)
//		  *TransPos : �\������ꏊ(��ذ��ް����)
//		  gdata->G_NUM_TOPCHR {0,1,2,3,4,5,6,7,8,9}  �����̷���ް������鷬׸��Ȱєԍ�
//		  SpcChr : ��߰��̷�׸��Ȱєԍ�
//		  yose : 0 = �E�l�ߕ\��   1 = ���l�ߕ\��
// �߂�l : �Ȃ�
//==================================================================
#define BUFF_MAX	4	//�ő包
#define SCREEN_X_SIZE	256	//��ذ�X����
#define SCREEN_X	(SCREEN_X_SIZE / 8)	//1���C����̽�ذ��ް���

#if 1	//==========================================================
//�t�H���g�T�C�Y8x16�p
void PutNumber(s16 number, u16 *TransPos, u8 yose)
{
	s8 count;	//for����ϲŽ�̒l���g�p����̂�singed�ł�邱��
	s8 pos;
	u8 buff[BUFF_MAX];
	
	for(count = 0; count < BUFF_MAX; count++)
	{
		buff[count] = 0;
	}
	count = BUFF_MAX-1;
	
	while(1)
	{
		if(number > 0)
		{
			buff[count] = number % 10;
			number /= 10;
			count--;
		}
		else
		{
			for(; count > -1; count--)
			{
				buff[count] = 0xff;	//��а�ް�
			}
			if(buff[BUFF_MAX-1] == 0xff)
			{
				buff[BUFF_MAX-1] = 0;	//�S����а�ް��������ꍇ��0��\��������
			}
			break;
		}
	}
	
	if(yose == 0)
	{				//�E�l��
		for(count = 0, pos = 0; count < BUFF_MAX; count++)
		{
			if(buff[pos] == 0xff)
			{
				TransPos[pos] &= 0xfc00;
				TransPos[pos] |= G_SPACE_CHR;

				TransPos[count + SCREEN_X] &= 0xfc00;
				TransPos[count + SCREEN_X] |= G_SPACE_CHR;
			}
			else
			{
				TransPos[pos] &= 0xfc00;
				TransPos[pos] |= (G_NUM_TOPCHR + buff[pos]);

				TransPos[count + SCREEN_X] &= 0xfc00;
				TransPos[count + SCREEN_X] |= (G_NUM_TOPCHR + buff[count] + 0x20);
			}
			pos++;
		}
	}
	else
	{				//���l��
		for(count = 0; count < BUFF_MAX; count++)
		{
			if(buff[count] == 0xff)
			{
				TransPos[count] &= 0xfc00;
				TransPos[count] |= G_SPACE_CHR;

				TransPos[count + SCREEN_X] &= 0xfc00;
				TransPos[count + SCREEN_X] |= G_SPACE_CHR;
			}
			else
			{
				TransPos[count] &= 0xfc00;
				TransPos[count] |= (G_NUM_TOPCHR + buff[count]);

				TransPos[count + SCREEN_X] &= 0xfc00;
				TransPos[count + SCREEN_X] |= (G_NUM_TOPCHR + buff[count] + 0x20);
			}
		}
	}
}

#else	//============================================================
// �t�H���g�T�C�Y8x8�p
void PutNumber(s16 number, u16 *TransPos, u8 yose)
{
	s8 count;	//for����ϲŽ�̒l���g�p����̂�singed�ł�邱��
	s8 pos;
	u8 buff[BUFF_MAX];
	
	for(count = 0; count < BUFF_MAX; count++)
	{
		buff[count] = 0;
	}
	count = BUFF_MAX-1;
	
	while(1)
	{
		if(number > 0)
		{
			buff[count] = number % 10;
			number /= 10;
			count--;
		}
		else
		{
			for(; count > -1; count--)
			{
				buff[count] = 0xff;	//��а�ް�
			}
			if(buff[BUFF_MAX-1] == 0xff)
			{
				buff[BUFF_MAX-1] = 0;	//�S����а�ް��������ꍇ��0��\��������
			}
			break;
		}
	}
	
	if(yose == 0)
	{				//�E�l��
		for(count = 0, pos = 0; count < BUFF_MAX; count++)
		{
			if(buff[pos] == 0xff)
			{
				TransPos[pos] &= 0xfc00;
				TransPos[pos] |= G_SPACE_CHR;
			}
			else
			{
				TransPos[pos] &= 0xfc00;
				TransPos[pos] |= (G_NUM_TOPCHR + buff[pos]);
			}
			pos++;
		}
	}
	else
	{				//���l��
		for(count = 0; count < BUFF_MAX; count++)
		{
			if(buff[count] == 0xff)
			{
				TransPos[count] &= 0xfc00;
				TransPos[count] |= G_SPACE_CHR;
			}
			else
			{
				TransPos[count] &= 0xfc00;
				TransPos[count] |= (G_NUM_TOPCHR + buff[count]);
			}
		}
	}
}
#endif	//========================================================

void PutHPNumber(s16 MaxHP, s16 resultHP, u16 *TransPos)
{
	TransPos[4] = G_SPACE_CHR;
	PutNumber(resultHP, TransPos, 0);
	PutNumber(MaxHP, TransPos + 5, 1);
}


























//=========================================================================
//	�ȉ� OBJ�p
//=========================================================================
//==================================================================
// �ް�ނ̷�ׂ� [,0,1,2,3,4,5,6,7,8,], �̏��ɕ���œo�^����Ă邱��
//==================================================================


//=========================================================================
//	�o�g���Q�[�W
// ����1��ܰ� : user �� ����2��actno �� ���ъ֌W
// ����2��ܰ� : �ް�ޗp�ް�
//=========================================================================
//----------------------------------------------------------
//	�ް�ނŎg�p���鱸��(OBJ)���܂Ƃ߂�AddActor����
//
// ����: user = �N�̹ް�ނ�(0:����1�A1:����2�A2:�G1�A3:�G2)
// �߂�l:1�Ԗڂ̱����z��ԍ�(2�Ԗڂ̱����z��ԍ���1�Ԗڂ̱�����work7�ɓ����Ă�)
//		  work[5]�ɂ��߰�actno������
//        work[6]�ɂ�user,work[7]�ɂ͹ް�ނ̶��ق�actno������(��ٰ�݂�)
//----------------------------------------------------------
u8 GaugeActorSet(u8 client_no)
{
	u8 gauge, gauge2, parts;
	u8 parts_offs = 0;
	
	if(FightCheck() == 0)
	{
	#if GAUGE_CHANGE
		gauge = AddActor(&GaugeMineHeader[0], 240, 160, GAUGE_ACT_PRI);
		if(MineEnemyCheck(client_no) == SIDE_MINE)
			SetActOamPat(&ActWork[gauge], &GaugePatMine[0]);
		else
			SetActOamPat(&ActWork[gauge], &GaugePatEnemy[0]);
	#else
		if(MineEnemyCheck(client_no) == SIDE_MINE)
		{
			gauge = AddActor(&GaugeMineHeader[0], 240, 160, GAUGE_ACT_PRI);
			gauge2 = AddActorBottom(&GaugeMineHeader[0], 240, 160, GAUGE_ACT_PRI);
			ActWork[gauge].oamData.Shape = 0;
			ActWork[gauge2].oamData.Shape = 0;
			ActWork[gauge2].oamData.CharNo += 64;
			parts_offs = MINE_PARTS_1VS1;
		}
		else
		{
			gauge = AddActor(&GaugeEnemyHeader[0], 240, 160, GAUGE_ACT_PRI);
			gauge2 = AddActorBottom(&GaugeEnemyHeader[0], 240, 160, GAUGE_ACT_PRI);
			ActWork[gauge2].oamData.CharNo += 32;
			parts_offs = ENEMY_PARTS;
		}
		ActWork[gauge].oamData.AffineParam = gauge2;	//�E���̱�����ActNo
		ActWork[gauge2].work[5] = gauge;	//�{�̂�ActNo
		ActWork[gauge2].move = GaugeSubSeq;
	#endif
	}
	else
	{
		if(MineEnemyCheck(client_no) == SIDE_MINE)
		{
			gauge = AddActor(&GaugeMineHeader[ClientTypeGet(client_no) >> 1],
				 240, 160, GAUGE_ACT_PRI);
		#if GAUGE_CHANGE
			SetActOamPat(&ActWork[gauge], &GaugePatMine[1]);
		#else
			gauge2 = AddActorBottom(&GaugeMineHeader[ClientTypeGet(client_no) >> 1],
				 240, 160, GAUGE_ACT_PRI);
			ActWork[gauge].oamData.AffineParam = gauge2;
			ActWork[gauge2].work[5] = gauge;	//�{�̂�ActNo
			ActWork[gauge2].oamData.CharNo += 32;
			ActWork[gauge2].move = GaugeSubSeq;
			parts_offs = MINE_PARTS_2VS2;
		#endif
		}
		else
		{
			gauge = AddActor(&GaugeEnemyHeader[ClientTypeGet(client_no) >> 1], 
				240, 160, GAUGE_ACT_PRI);
		#if GAUGE_CHANGE
			SetActOamPat(&ActWork[gauge], &GaugePatEnemy[1]);
		#else
			gauge2 = AddActorBottom(&GaugeEnemyHeader[ClientTypeGet(client_no) >> 1], 
				240, 160, GAUGE_ACT_PRI);
			ActWork[gauge].oamData.AffineParam = gauge2;
			ActWork[gauge2].work[5] = gauge;	//�{�̂�ActNo
			ActWork[gauge2].oamData.CharNo += 32;
			ActWork[gauge2].move = GaugeSubSeq;
			parts_offs = ENEMY_PARTS;
		#endif
		}
	}
	parts = AddActorBottom(&GaugePartsHeader[ClientType[client_no]], 140, 60, HP_GAUGE_ACT_PRI);
	SetActOamPat(&ActWork[parts], &GaugePatParts[MineEnemyCheck(client_no)]);
	ActWork[parts].oampat_sw = OAMPAT_PRIUNIT;	//ʰ����ײ��è�S���� 2002.07.04(��)
	ActWork[parts].oamData.Priority = GAUGE_OAM_PRI;
//	DIV_DMACOPY(3, GetGaugePartsAdrs(GP_HP), (ActWork[parts].oamData.CharNo << 5)
//		+ OBJ_MODE0_VRAM, 0x40, 32);
	CpuCopy(GetGaugePartsAdrs(GP_HP), (ActWork[parts].oamData.CharNo << 5)
		+ OBJ_MODE0_VRAM, 0x40, 32);
	
	ActWork[gauge].work[5] = parts;		//�߰�ActNo
	ActWork[gauge].work[6] = client_no;	//���L��
//	ActWork[gauge].work[7] = 0;	 2��2�̎��A���ݕ\�����Ă���̂͹ް�ނ��A���l��
	ActWork[gauge].banish = 1;
	ActWork[gauge2].banish = 1;
	
	ActWork[parts].work[5] = gauge;		//�{�̂�ActNo
	ActWork[parts].work[6] = parts_offs;
	ActWork[parts].banish = 1;
	return gauge;
}

//add by soga 2002.06.01
u8 GaugeActorSetSafari(void)
{
	u8 gauge, gauge2;
	
	gauge = AddActor(&GaugeSafariHeader, 240, 160, GAUGE_ACT_PRI);
	gauge2 = AddActorBottom(&GaugeSafariHeader, 240, 160, GAUGE_ACT_PRI);
	ActWork[gauge].oamData.Shape = 0;
	ActWork[gauge2].oamData.Shape = 0;
	ActWork[gauge2].oamData.CharNo += 64;
	ActWork[gauge].oamData.AffineParam = gauge2;	//�E���̱�����ActNo
	ActWork[gauge2].work[5] = gauge;	//�{�̂�ActNo
	ActWork[gauge2].move = GaugeSubSeq;

	return gauge;
}

//----------------------------------------------------------
//	�Q�[�W�p�[�c�̷�ׂ̱��ڽ�����o��
//----------------------------------------------------------
u8 *GetGaugePartsAdrs(u8 parts_id)
{
	u16 charno;
	
	charno = parts_id * 0x20;
//	return (u8*)&gauge5b_Character[charno];
	return (u8*)(gauge5b_pch_ADRS+charno);
}


//----------------------------------------------------------
//	�Q�[�W�p�[�c�̃V�[�P���X
//----------------------------------------------------------
static void GaugePartsSeq(actWork *Xreg)
{
	u8 hontai;
	
	hontai = Xreg->work[5];
	switch(Xreg->work[6])
	{
		case MINE_PARTS_1VS1:
			Xreg->x = ActWork[hontai].x + 16;	//24;
			Xreg->y = ActWork[hontai].y + 0;	//16;
			break;
		case MINE_PARTS_2VS2:
			Xreg->x = ActWork[hontai].x + 16;	//24;
			Xreg->y = ActWork[hontai].y + 0;	//16;
			break;
		case ENEMY_PARTS:
		default:
			Xreg->x = ActWork[hontai].x + 8;	//24;
			Xreg->y = ActWork[hontai].y + 0;	//16;
	}
	Xreg->dx = ActWork[hontai].dx;
	Xreg->dy = ActWork[hontai].dy;
}

//----------------------------------------------------------
//	�{�̉E���̃V�[�P���X
//----------------------------------------------------------
static void GaugeSubSeq(actWork *Xreg)
{
	u8 hontai;
	
	hontai = Xreg->work[5];
	Xreg->x = ActWork[hontai].x +64;
	Xreg->y = ActWork[hontai].y;
	Xreg->dx = ActWork[hontai].dx;
	Xreg->dy = ActWork[hontai].dy;
}



//----------------------------------------------------------
//						���ް��OBJ��p
//	�ް�ނ�OBJ�ŏo���ꍇ��GaugeData�������Ƃ��ēn�����肵�Ȃ���
//  ���̊֐����Ă�Ńf�[�^��Ă���
//
// ����:client_no
//	   :actno = �Q�[�W��ActNo
//     :beHP = ��������l
//	   :�ް�ނ�������į�߂̈ʒu���牽��׸�����ꂽ�ʒu���炠�邩
//     :req_data = �����ް���(0:HP 1:Exp)
//----------------------------------------------------------
void GaugeDataSet(u8 client_no, u8 actno, s32 MaxHP, s32 NowHP, s32 beHP)
{
	gwork[client_no].actno = actno;
	gwork[client_no].MaxHP = MaxHP;
	gwork[client_no].NowHP = NowHP;
	gwork[client_no].beHP = beHP;
#if GAUGE_SOKUDO_TYPE
	gwork[client_no].HP_Work = HP_WORK_INIT_VALUE;
#else
	gwork[client_no].HP_Work = 0;		//HP_Work������
#endif
}

//----------------------------------------------------------
//	�ް�ޕ\��OFF
// actno = �Ď��p�����̔z��ԍ�
//----------------------------------------------------------
void GaugeOFF(u8 actno)
{
	ActWork[actno].banish = 1;
	ActWork[ActWork[actno].work[5]].banish = 1;
#if GAUGE_CHANGE == 0
	ActWork[ActWork[actno].oamData.AffineParam].banish = 1;
#endif
}
//----------------------------------------------------------
//	�ް�ޕ\��ON
// actno = �Ď��p�����̔z��ԍ�
//----------------------------------------------------------
void GaugeON(u8 actno)
{
	ActWork[actno].banish = 0;
	ActWork[ActWork[actno].work[5]].banish = 0;
#if GAUGE_CHANGE == 0
	ActWork[ActWork[actno].oamData.AffineParam].banish = 0;
#endif
}
//----------------------------------------------------------
//	�ް�ޕ\���ʒu�ݒ�
//----------------------------------------------------------
void GaugePosSet(u8 actno, s16 xpos, s16 ypos)
{
	ActWork[actno].x = xpos;
	ActWork[actno].y = ypos;
//	ActWork[ActWork[actno].work[7]].x = xpos;// + 64;	// + xxx = �ޯĐ�
//	ActWork[ActWork[actno].work[7]].y = ypos+32-8;
}
//----------------------------------------------------------
//	�ް�ނŎg�p��������(OBJ)���܂Ƃ߂�DelActor����
//----------------------------------------------------------
void GaugeActorDel(u8 actno)
{
#if GAUGE_CHANGE == 0
	DelActor(&ActWork[ActWork[actno].oamData.AffineParam]);
#endif
	DelActor(&ActWork[ActWork[actno].work[5]]);
	DelActor(&ActWork[actno]);
}
//----------------------------------------------------------
//	�ް�ނ���ײ��è�ύX
// pri = 0(��O)  1(��)  2(���])
//----------------------------------------------------------
void GaugePriSet(u8 actno, u8 pri)
{
#if 0	//************************************
	u8 gauge_pri;
	
	switch(pri)
	{
		case 0:	//��O
			gauge_pri = BTL_GAUGE_PRI1;
			break;
		case 1:	//��
			gauge_pri = BTL_GAUGE_PRI2;
			break;
		case 2:	//���]
		default:
			if(ActWork[actno].pri == BTL_GAUGE_PRI1)
				gauge_pri = BTL_GAUGE_PRI2;
			else
				gauge_pri = BTL_GAUGE_PRI1;
			break;
	}
	ActWork[actno].pri = gauge_pri;
//	ActWork[ActWork[actno].work[7]].pri = gauge_pri;
#endif	//*********************************
}

//=========================================================================
//	�퓬�Q�[�W�̃v���C�I���e�B�𑀍삷��	2002.07.06(�y) by matsuda
//=========================================================================
void GaugeHardPriSet(u8 hard_pri)
{
	u8 gauge, gauge2, parts;
	int i;
	
	for(i = 0; i < ClientSetMax; i++)
	{	//�ް�ނ̏ꍇ�͑������C�₵�ĂĂ�Add�͂���Ă���̂�2�̖������͂��ĂȂ�
		gauge = ClientGaugeSt[i];		//�{�́@����
		gauge2 = ActWork[gauge].oamData.AffineParam;	//�{�́@�E��
		parts = ActWork[gauge].work[5];			//�p�[�c

		ActWork[gauge].oamData.Priority = hard_pri;
		ActWork[gauge2].oamData.Priority = hard_pri;
		ActWork[parts].oamData.Priority = hard_pri;
	}
}

//----------------------------------------------------------
//	�ް�ނ̏������W���
//----------------------------------------------------------
void GaugeDefaultPosSet(u8 client_no)
{
	u8 xpos = 0, ypos = 0;
	
//	if(FightType == FIGHT_TYPE_1vs1)
	if(FightCheck() == 0)
	{
		if(MineEnemyCheck(client_no))
		{
			xpos = CAI_GAUGE_X;
			ypos = CAI_GAUGE_Y;
		}
		else
		{
			xpos = C_GAUGE_X;
			ypos = C_GAUGE_Y;
		}
	}
	else
	{
		switch(ClientTypeGet(client_no))
		{
			case BSS_CLIENT_MINE:
				xpos = C_GAUGE_X1;
				ypos = C_GAUGE_Y1;
				break;
			case BSS_CLIENT_MINE2:
				xpos = C_GAUGE_X2;
				ypos = C_GAUGE_Y2;
				break;
			case BSS_CLIENT_ENEMY:
				xpos = CAI_GAUGE_X1;
				ypos = CAI_GAUGE_Y1;
				break;
			case BSS_CLIENT_ENEMY2:
				xpos = CAI_GAUGE_X2;
				ypos = CAI_GAUGE_Y2;
				break;
		}
	}
	GaugePosSet(ClientGaugeSt[client_no], xpos, ypos);
}



void GaugeLevelTrans(u8 actno, u8 level)
{
	u8 buff[30] = {CTRL_,C_FCOL_,1,CTRL_,C_BCOL_,0x2};
	u8 i, t;
	u8 pos = 0;
	u8 level_temp;
	u8 trans_num;
	
	level_temp = level;
	t = 0;

#if GAUGE_CHANGE
	if(FightCheck() == 0)
	{
		if(MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE)
			pos = MINE_LV_POS;
		else
			pos = ENEMY_LV_POS;
	}
	else
		pos = LVPOS_2VS2;
#else
	if(FightCheck() == 0)
	{
		if(MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE)
			pos = MINE_LV_POS;
		else
			pos = ENEMY_LV_POS;
	}else{
		if(MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE)
			pos = MINE_LV_POS2;
		else
			pos = ENEMY_LV_POS;
	}
#endif

	if(level == 100)
	{
		PM_NumMsgSet(&buff[6], level, NUM_MODE_LEFT, 3);
		trans_num = 3;
	}
	else
	{
//		DmaTransReq((void*)GetGaugePartsAdrs(GP_CORON),	//�h�F�h�]��
//			((ActWork[actno].oamData.CharNo + pos) << 5) + OBJ_MODE0_VRAM, 0x20);
//		DIV_DMACOPY(3, (void*)GetGaugePartsAdrs(GP_CORON),	//�h�F�h�]��
//			((ActWork[actno].oamData.CharNo + pos) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
		CpuCopy((void*)GetGaugePartsAdrs(GP_CORON),	//�h�F�h�]��
			((ActWork[actno].oamData.CharNo + pos) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
		PM_NumMsgSet(&buff[6], level, NUM_MODE_LEFT, 2);
		if(level < 10)
		{
			buff[7] = spc_;	buff[8] = EOM_;
		}
		trans_num = 2;
		pos++;
	}

	MsgCGXDataSet((void*)GAUGE_DECORD_ADR, buff);
	for(i = 0; i < trans_num; i++)
//		DIV_DMACOPY(3, (void*)GAUGE_DECORD_ADR + i * 0x40 + 0x20,
//			((ActWork[actno].oamData.CharNo + pos+i) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
		CpuCopy((void*)GAUGE_DECORD_ADR + i * 0x40 + 0x20,
			((ActWork[actno].oamData.CharNo + pos+i) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
/*---------------------------
	for(i = 0; i < 3; i++)
	{
		if(1)//MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE)
		{
			DmaTransReq((void*)(GAUGE_DECORD_ADR + ((buff[i] + NUM_CHR_OFFSET+1+10) << 5)),
				((ActWork[actno].oamData.CharNo + pos + t) << 5)
					 + OBJ_MODE0_VRAM,
						0x20);
		}
		else	//�G��
		{
			if(i == 0)
			{
				DmaTransReq((void*)(GAUGE_DECORD_ADR + ((buff[i] + NUM_CHR_OFFSET+1+10) << 5)),
					((ActWork[actno].oamData.CharNo + 64+15 + t) << 5)
						 + OBJ_MODE0_VRAM,
							0x20);
			}
			else
			{
				DmaTransReq((void*)(GAUGE_DECORD_ADR + ((buff[i] + NUM_CHR_OFFSET+1+10) << 5)),
					((ActWork[actno].oamData.CharNo + pos + t) << 5)
						 + OBJ_MODE0_VRAM,
							0x20);
			}
		}
		t++;
	}
---------------------------*/
}

#define HPKETA_MAX	3	//�ő�\������
//	DecordWram((void*)tesnum_pch_ADRS, (void*)GAUGE_DECORD_ADR);
//----------------------------------------------------------
//	�]����ׂ�ܰ��тւ̓]���͂�����ĂԑO�ɂ���Ă�������
// flg = 0:����HP  1:�ő�HP
//----------------------------------------------------------
void GaugeHPTrans(u8 actno, s16 hp, u8 flg)
{
	u8 buff[20] = {CTRL_,C_FCOL_,1,CTRL_,C_BCOL_,0x2};
	u8 i;//, j = 0;
	u8 pos;
	u8 client_no;
	
	client_no = ActWork[actno].work[6];
	if(FightCheck() == 1 || MineEnemyCheck(client_no) == SIDE_ENEMY){
		GaugeHPTrans2vs2(actno, hp, flg);
		return;
	}

	if(MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE)
	{	//����
		if(flg == 0)
			pos = MINE_HP_POS;
		else
			pos = MINE_HPMAX_POS;
	}
	else{	//�G
		if(flg == 0)
			pos = ENEMY_HP_POS;
		else
			pos = ENEMY_HPMAX_POS;
	}
	
	PM_NumMsgSet(&buff[6], hp, NUM_MODE_SPACE, 3);
	MsgCGXDataSet((void*)GAUGE_DECORD_ADR, buff);
	for(i = 0; i < HPKETA_MAX; i++)
		CpuCopy(GAUGE_DECORD_ADR+0x20 + i*0x40, 
			((ActWork[actno].oamData.CharNo + pos + i) << 5) 
			+ OBJ_MODE0_VRAM, 0x20, 32);
}

//----------------------------------------------------------
//	2��2�̎���HP�\��(�Q�[�W��������HP��\������)
// flg = 0:����HP  1:�ő�HP
//----------------------------------------------------------
static const u8 sura_eom_msg[] = {sura_,EOM_};
void GaugeHPTrans2vs2(u8 actno, s16 hp, u8 flg)
{
	u8 buff[20] = {CTRL_,C_FCOL_,1,CTRL_,C_BCOL_,0};	//xf};
	u8 i;//, j = 0;
	u8 pos;
	u8 parts;
	u8 *msg;
	u8 client_no;
	
//	if(FightCheck() == 0)
//		return;
	
	client_no = ActWork[actno].work[6];
	
	if(Climem[client_no].gauge_mode == 0)
		return;
		
	if(flg == 0)
		pos = 0;
	else
		pos = 4;

	parts = ActWork[actno].work[5];
	msg = PM_NumMsgSet(&buff[6], hp, NUM_MODE_SPACE, 3);
	if(flg == 0){
//		msg[0] = sura_;
//		msg[1] = EOM_;
		PM_strcpy(msg, sura_eom_msg);
	}
	MsgCGXDataSet((void*)GAUGE_DECORD_ADR, buff);
	for(i = pos; i < pos + HPKETA_MAX; i++){
		if(i < 3)
			CpuCopy(GAUGE_DECORD_ADR+0x20 + (i-pos)*0x40, 
				((ActWork[parts].oamData.CharNo + 1 + i) << 5) + OBJ_MODE0_VRAM,
				 0x20, 32);
		else
			CpuCopy(GAUGE_DECORD_ADR+0x20 + (i-pos)*0x40, 
				((ActWork[parts].oamData.CharNo + 4 + i - 3) << 5)
				 + OBJ_MODE0_VRAM, 0x20, 32);
	}
	
	if(flg == 0){
		CpuCopy(GAUGE_DECORD_ADR+0x20 + HPKETA_MAX*0x40,
			((ActWork[parts].oamData.CharNo + 4) << 5) + OBJ_MODE0_VRAM,0x20,32);

		//[HP]��[H]�̕��������l�\���̂܂܉�ʐ؂�ւ�������Ǝc���ďo�Ă���̂�
		//�����ŏ����Ă��� add2002.09.18(��)
		CpuClear(0, OBJ_MODE0_VRAM + ActWork[parts].oamData.CharNo*32, 32, 32);
	}
	else if(MineEnemyCheck(client_no) == SIDE_MINE)
	{	//�G���̕\�������ޯ����݂̂Ȃ̂ł��Ȃ�
		CpuCopy(GetGaugePartsAdrs(GP_HPNUMCLOSE),
			(ActWork[actno].oamData.CharNo+0x34) * 32 + OBJ_MODE0_VRAM, 0x20, 32);
	}
}

extern const u8 * const SeikakuMsgTbl[];

void	GaugeSafariDebugTrans(u8 actno,PokemonParam *PP)
{
	int	i;
	u8 	j;
	u16	k;
	u8	*adr1;
	u32	adr2;
	u8	dakuten;
	u8	chr;
	u8	buff[20] = {CTRL_,C_FCOL_,1,CTRL_,C_BCOL_,0x02};
	u8	parts;
	
	adr1 = (void*)(GAUGE_DECORD_ADR + ((NAME_CHR_OFFSET << 5)
				+ 32 * 6 * 2 * ClientTypeGet(ActWork[actno].work[6])));

	k=5;
	chr=PokeSeikakuGet(PP);

	PM_strcpy(&buff[6],SeikakuMsgTbl[chr]);
	
	MsgCGXDataSet(adr1,buff);	//ү�����ް��W�J

	for(i = 6, j = 0; j < k; i++)
	{
		if((buff[i] >= ga_ && buff[i] <= bo_)
			|| (buff[i] >= GA_ && buff[i] <= BO_))
		{	//���_���̳���޳��ׂ𑗂�
			dakuten = GP_DAKUTEN;
		}
		else if((buff[i] >= pa_ && buff[i] <= po_)
			|| (buff[i] >= PA_ && buff[i] <= PO_))
		{	//�����_���̳���޳��ׂ𑗂�
			dakuten = GP_HANDAKUTEN;
		}
		else
		{	//���_�Ȃ��̳���޳��ׂ𑗂�
			dakuten = GP_TENNASI;
		}
		CpuCopy(GetGaugePartsAdrs(dakuten), &adr1[j * 0x40], 0x20, 32);
		j++;
	}

	for(i = 1; i < k+1; i++)
	{
		//���_
		adr2=((ActWork[actno].oamData.CharNo+(i%8)+(0x40*(i/8))) << 5)
			 + OBJ_MODE0_VRAM ;
		CpuCopy(adr1,adr2,0x20,32);
		adr1+=0x20;
		
		//�{��
		adr2=((ActWork[actno].oamData.CharNo+8+(i%8)+(0x40*(i/8))) << 5) + OBJ_MODE0_VRAM;
		CpuCopy(adr1,adr2,0x20,32);
		adr1+=0x20;
	}

	parts = ActWork[actno].work[5];
	PM_NumMsgSet(&buff[6],UserWork[SAFARI_GET_COUNT],NUM_MODE_SPACE,2);
	PM_NumMsgSet(&buff[9],UserWork[SAFARI_ESCAPE_COUNT],NUM_MODE_SPACE,2);
	buff[5]=0;
	buff[8]=sura_;

	MsgCGXDataSet((void*)GAUGE_DECORD_ADR, buff);

	for(i = 0; i < 5 ; i++){
		if(i < 2)
			CpuCopy(GAUGE_DECORD_ADR+0x20 + i*0x40, 
				((ActWork[parts].oamData.CharNo + 2 + i) << 5) + OBJ_MODE0_VRAM,
				 0x20, 32);
		else
			CpuCopy(GAUGE_DECORD_ADR+0x20 + i*0x40, 
				((ActWork[parts].oamData.CharNo + 8 + i - 2) << 5)
				 + OBJ_MODE0_VRAM, 0x20, 32);
	}
}

//----------------------------------------------------------
//	2��2�̎��A������ĂԂ��т�HP�ƃQ�[�W�����ւ��ĕ\������
//----------------------------------------------------------
void HPNumGaugeChangePut(void)
{
	int i;
	u8 change_flg;
	u8 parts;
	
	for(i = 0; i < ClientSetMax; i++)
	{
		//�o���̪�Ē��ȂǁA�����ް�ނ�ܰ����g�p����̂Əd�Ȃ�Ɠ��삪���������Ȃ�̂�
		if(ActWork[ClientGaugeSt[i]].move != DummyActMove)
			continue;
	
	#ifdef PM_DEBUG
		if(DebugMainFlag == 0)
	#endif
		{
			if(MineEnemyCheck(i) == SIDE_ENEMY)	//ɰ����ڲ���͓G��HP�͏o���Ȃ�
				continue;
		}
	
		if(FightCheck() == 0 && MineEnemyCheck(i) == SIDE_MINE)
			continue;
		
		Climem[i].gauge_mode ^= 1;
		change_flg = Climem[i].gauge_mode;
		
		if(MineEnemyCheck(i) == 0){
			if((FightCheck() == 0)||
			   (FightType&FIGHT_TYPE_SAFARI))
				continue;
				
			if(change_flg == 1){
				parts = ActWork[ClientGaugeSt[i]].work[5];
				CpuClear(0, OBJ_MODE0_VRAM + ActWork[parts].oamData.CharNo*32, 
						8*32, 32);
				GaugeHPTrans2vs2(ClientGaugeSt[i],
					PokeParaGet(&PokeParaMine[SelMonsNo[i]], ID_hp), 0);
				GaugeHPTrans2vs2(ClientGaugeSt[i],
					PokeParaGet(&PokeParaMine[SelMonsNo[i]], ID_hpmax), 1);
			}else{
				GaugeStatusTrans(ClientGaugeSt[i]);
				GaugeTransChrSet(ClientGaugeSt[i], 
					&PokeParaMine[SelMonsNo[i]], G_HPGAUGE);
				CpuCopy(GetGaugePartsAdrs(GP_HPBARCLOSE),
					(ActWork[ClientGaugeSt[i]].oamData.CharNo+0x34) * 32 
					+ OBJ_MODE0_VRAM, 0x20, 32);
			}
		}else{
			if(change_flg == 1){
				if(FightType&FIGHT_TYPE_SAFARI)
					GaugeSafariDebugTrans(ClientGaugeSt[i],&PokeParaEnemy[SelMonsNo[i]]);
				else{
					parts = ActWork[ClientGaugeSt[i]].work[5];
					CpuClear(0, OBJ_MODE0_VRAM + ActWork[parts].oamData.CharNo*32, 
							8*32, 32);
					GaugeHPTrans2vs2(ClientGaugeSt[i],
						PokeParaGet(&PokeParaEnemy[SelMonsNo[i]], ID_hp), 0);
					GaugeHPTrans2vs2(ClientGaugeSt[i],
						PokeParaGet(&PokeParaEnemy[SelMonsNo[i]], ID_hpmax), 1);
				}
			}else{
				GaugeStatusTrans(ClientGaugeSt[i]);
				GaugeTransChrSet(ClientGaugeSt[i], 
					&PokeParaEnemy[SelMonsNo[i]], G_HPGAUGE);
				if(FightType&FIGHT_TYPE_SAFARI)
					GaugeTransChrSet(ClientGaugeSt[i], 
						&PokeParaEnemy[SelMonsNo[i]], G_NAME);
			}
		}
		ActWork[ClientGaugeSt[i]].work[7] ^= 1;
	}
}


//----------------------------------------------------------
// �莝���߹�ݐ����ްٱ��݂ŕ\��
// ����:zahyou = 0:�ް�ނ̒�ʒu�ɏo��	1:ClientNo�ɂ���Ĺް�ނ̍��W��ς���
//		syokai = 1:�ŏ����ڰŰ���ײ�޲݂��ė������ɏo�鎞
//				 0:�|�P��������ւ���
// �߂�l�F�{�[����\�����Ă���Q�[�W��ActNo
//----------------------------------------------------------
#define TEMOTI_BALL_STWAIT	7	//�ްٷ�ׂ������ɓ����Ă���Ԋu

u8 TemotiBallPut(u8 client_no, BallGaugePara *bgd, u8 zahyou, u8 syokai)
{
	s8 kazu;
	int i, s;	//, parts, getid;
	u8 mine_enemy;
	u8 id, actno, ball[6];
	s16 gauge_x, gauge_y;
	s16 gauge_dx, gauge_add;
	
//	if(FightCheck() == 1 || (FightType & FIGHT_TYPE_TRAINER) == 0)
//		return;

	if(zahyou == 0 || ClientTypeGet(client_no) != BSS_CLIENT_ENEMY2){
		if(MineEnemyCheck(client_no) == SIDE_MINE){
			mine_enemy = 0;
			gauge_x = MINE_TEMOTI_GAUGE_X;
			gauge_y = MINE_TEMOTI_GAUGE_Y;
			gauge_dx = TEMOTI_GAUGE_SP * TEMOTI_GAUEG_MOVE_SYNC;
			gauge_add = -TEMOTI_GAUGE_SP;
		}else{
			mine_enemy = 1;
			if(zahyou == 0 || FightCheck() == 0){
				gauge_x = TEMOTI_ENEMY2_X;
				gauge_y = TEMOTI_ENEMY2_Y;
			}
			else{
				gauge_x = ENEMY_TEMOTI_GAUGE_X;
				gauge_y = ENEMY_TEMOTI_GAUGE_Y;
			}
			gauge_dx = -(TEMOTI_GAUGE_SP * TEMOTI_GAUEG_MOVE_SYNC);
			gauge_add = TEMOTI_GAUGE_SP;
		}
	}else{//ENEMY2�݂̂�遦�]�䕔���񂪌�����(�����ڰŰ���߹�ݓ���ւ��̎��Ɏg����)
		mine_enemy = 1;
		gauge_x = TEMOTI_ENEMY2_X;
		gauge_y = TEMOTI_ENEMY2_Y;
		gauge_dx = -(TEMOTI_GAUGE_SP * TEMOTI_GAUEG_MOVE_SYNC);
		gauge_add = TEMOTI_GAUGE_SP;
	}
	
	for(i = 0, kazu = 0; i < TEMOTI_POKEMAX; i++)
	{
		if(bgd[i].hp == 0xffff)
			continue;
		kazu++;
	}
	
	DecordCellSet((void*)&TemotiGaugeCell[mine_enemy]);
	CellSet((CellData*)&TemotiBallCell[mine_enemy]);
	ObjPalSet((PalData*)&TemotiGaugePal[mine_enemy]);
	ObjPalSet((PalData*)&TemotiBallPal[mine_enemy]);
	actno = AddActor(&TemotiGaugeHeader[mine_enemy], gauge_x,gauge_y,TEMOTI_GAUGE_PRI);
	SetActOamPat(&ActWork[actno], &TemotiGaugePatTbl[0]);//mine_enemy]);
	ActWork[actno].dx = gauge_dx;
	ActWork[actno].work[0] = gauge_add;
	if(mine_enemy){
		ActWork[actno].x -= 96;
		ActWork[actno].oamData.AffineParamNo = 8;	//Hflip
	}else
		ActWork[actno].x += 96;
	
	for(i = 0; i < TEMOTI_POKEMAX; i++){
		ball[i] = AddActorBottom(&TemotiBallHeader[mine_enemy],
//			240+8, 160+8, TEMOTI_BALL_PRI);
			gauge_x, gauge_y - 4, TEMOTI_BALL_PRI);
		if(syokai == 0)
			ActWork[ball[i]].move = TemotiBallNormalSeq;
		if(mine_enemy == 0){
			ActWork[ball[i]].dx = 0;
			ActWork[ball[i]].dy = 0;
		}
//		ActWork[ball[i]].dx += 10*i;	//8*i;
		ActWork[ball[i]].work[0] = actno;
		if(mine_enemy == 0){
			ActWork[ball[i]].x += 10*i + 24;
			ActWork[ball[i]].work[1] = TEMOTI_BALL_STWAIT * i + 10;
			ActWork[ball[i]].dx = 120;
		}
		else{
			ActWork[ball[i]].x -= 10*(TEMOTI_POKEMAX-1-i) + 24;
			ActWork[ball[i]].work[1] = TEMOTI_BALL_STWAIT * (TEMOTI_POKEMAX - i) + 10;
			ActWork[ball[i]].dx = -120;
		}
		ActWork[ball[i]].work[2] = mine_enemy;
	}
	
	if(MineEnemyCheck(client_no) == SIDE_MINE){
		//������kazu�ł͂Ȃ��Abgd�Ŏ��悤�ɂ��������������ǁA
		//����o�O���|���̂ŁA�Ƃ肠�����͂���� 02/05/02 by matsuda
		for(i = 0; i < TEMOTI_POKEMAX; i++){
			if(FightType&FIGHT_TYPE_4_2vs2){
				if(bgd[i].hp == 0xffff){
					ActWork[ball[i]].oamData.CharNo++;	//BallOFF�̕\��
					ActWork[ball[i]].work[7] = 1;		//BallOFF�̈�
				}
				else if(bgd[i].hp == 0)
					ActWork[ball[i]].oamData.CharNo += 3;	//�C��
				else if(bgd[i].condition != 0)
					ActWork[ball[i]].oamData.CharNo += 2;	//�ð���ُ�
			}
			else{
				if(i >= kazu){
					ActWork[ball[i]].oamData.CharNo++;	//BallOFF�̕\��
					ActWork[ball[i]].work[7] = 1;	//BallOFF�̈�
				}
				else if(bgd[i].hp == 0)
					ActWork[ball[i]].oamData.CharNo += 3;	//�C��
				else if(bgd[i].condition != 0)
					ActWork[ball[i]].oamData.CharNo += 2;	//�ð���ُ�
			}
		}
	}else{
		s = TEMOTI_POKEMAX - 1;
		for(i = 0; i < TEMOTI_POKEMAX; i++){
			if(FightType&FIGHT_TYPE_4_2vs2){
				if(bgd[i].hp == 0xffff){
					ActWork[ball[s]].oamData.CharNo++;	//BallOFF�̕\��
					ActWork[ball[s]].work[7] = 1;	//BallOFF�̈�
				}
				else if(bgd[i].hp == 0)
					ActWork[ball[s]].oamData.CharNo += 3;	//�C��
				else if(bgd[i].condition != 0)
					ActWork[ball[s]].oamData.CharNo += 2;	//�ð���ُ�
				s--;
			}
			else{
				if(i >= kazu){
					ActWork[ball[s]].oamData.CharNo++;	//BallOFF�̕\��
					ActWork[ball[s]].work[7] = 1;	//BallOFF�̈�
				}
				else if(bgd[i].hp == 0)
					ActWork[ball[s]].oamData.CharNo += 3;	//�C��
				else if(bgd[i].condition != 0)
					ActWork[ball[s]].oamData.CharNo += 2;	//�ð���ُ�
				s--;
			}
		}
	}
	
	id = AddTask(DummyTaskSeq, 5);
	TaskTable[id].work[0] = client_no;
	TaskTable[id].work[1] = actno;
//	TaskTable[id].work[2] = actno[1];
	for(i = 0; i < TEMOTI_POKEMAX; i++)
		TaskTable[id].work[3+i] = ball[i];
	TaskTable[id].work[10] = syokai;
//	SetWork_Adrs(&TaskTable[id].work[14], retadrs);

	SePlayPan(SE_TB_START, 0);
	return id;
}

void TemotiBallOff(u8 id)
{
	u8 ball[TEMOTI_POKEMAX];
	int i;
	u8 actno;
	u8 syokai;
	u8 client_no;
	
	syokai = TaskTable[id].work[10];
	actno = TaskTable[id].work[1];
	client_no = TaskTable[id].work[0];
	
	for(i = 0; i < TEMOTI_POKEMAX; i++)
		ball[i] = TaskTable[id].work[3+i];
	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL;
	*(vu16*)REG_BLDALPHA = 16;
	TaskTable[id].work[15] = 16;
	for(i = 0; i < TEMOTI_POKEMAX; i++)
		ActWork[ball[i]].oamData.ObjMode = 1;
	ActWork[actno].oamData.ObjMode = 1;
	
	if(syokai){
		for(i = 0; i < TEMOTI_POKEMAX; i++){
			if(MineEnemyCheck(client_no)){
				ActWork[ball[TEMOTI_POKEMAX-1-i]].work[1] = TEMOTI_BALL_STWAIT * i;
				ActWork[ball[TEMOTI_POKEMAX-1-i]].work[3] = 0;
				ActWork[ball[TEMOTI_POKEMAX-1-i]].work[4] = 0;
				ActWork[ball[TEMOTI_POKEMAX-1-i]].move = TemotiBallOutSeq;
			}
			else{
				ActWork[ball[i]].work[1] = TEMOTI_BALL_STWAIT * i;
				ActWork[ball[i]].work[3] = 0;
				ActWork[ball[i]].work[4] = 0;
				ActWork[ball[i]].move = TemotiBallOutSeq;
			}
		}
		ActWork[actno].work[0] /= 2;
		ActWork[actno].work[1] = 0;	// add 2002.09.13(��)
		ActWork[actno].move = TemotiGaugeOutSeq;
		SetActOamPat(&ActWork[actno], &TemotiGaugePatTbl[1]);
		TaskTable[id].TaskAdrs = TemotiBallOffSyokai;
	}
	else{
		TaskTable[id].TaskAdrs = TemotiBallOffMain;
	}
}

//-- ���񎞂̏�����G�t�F�N�g(���ɽײ�ނ��Ă���) --//
static void TemotiBallOffSyokai(u8 id)
{
	u8 actno;
	
	actno = TaskTable[id].work[1];

	if(TaskTable[id].work[11]++ % 2 == 0){
		TaskTable[id].work[15]--;
		if(TaskTable[id].work[15] >= 0){
			*(vu16*)REG_BLDALPHA = ((16-TaskTable[id].work[15]) << 8) | TaskTable[id].work[15];
		}
	}
	
	if(TaskTable[id].work[15] == 0)
		TaskTable[id].TaskAdrs = TemotiBallOffSyokaiEnd;
}

static void TemotiBallOffSyokaiEnd(u8 id)
{
	u8 client_no;
	u8 actno;
	u8 ball[TEMOTI_POKEMAX];
	int i;

	TaskTable[id].work[15]--;
	if(TaskTable[id].work[15] == -1){
		client_no = TaskTable[id].work[0];
		actno = TaskTable[id].work[1];
	//	actno2 = TaskTable[id].work[2];
		for(i = 0; i < TEMOTI_POKEMAX; i++)
			ball[i] = TaskTable[id].work[3+i];

		AllDelActor(&ActWork[actno]);
	//	DelActor(&ActWork[actno2]);
		AllDelActor(&ActWork[ball[0]]);
		for(i = 1; i < TEMOTI_POKEMAX; i++)
			DelActor(&ActWork[ball[i]]);
	//	SetAdrs_Work(&TaskTable[id].work[14], ClientSequence[client_no]);
	}
	else if(TaskTable[id].work[15] == -3){	//�������S��Del����ĂȂ��̂ŗ]�v��1SyncWait
		*(vu16*)REG_BLDCNT = 0;
		*(vu16*)REG_BLDALPHA = 0;
		DelTask(id);
	}
}

//-- ���̏�Ŕ������ŏ����Ă��� --//
static void TemotiBallOffMain(u8 id)
{
	u8 client_no;
	u8 actno;
	u8 ball[TEMOTI_POKEMAX];
	int i;
	
	TaskTable[id].work[15]--;
	if(TaskTable[id].work[15] >= 0){
		*(vu16*)REG_BLDALPHA = ((16-TaskTable[id].work[15]) << 8) | TaskTable[id].work[15];
		return;
	}
	
	if(TaskTable[id].work[15] == -1){
		client_no = TaskTable[id].work[0];
		actno = TaskTable[id].work[1];
	//	actno2 = TaskTable[id].work[2];
		for(i = 0; i < TEMOTI_POKEMAX; i++)
			ball[i] = TaskTable[id].work[3+i];

		AllDelActor(&ActWork[actno]);
	//	DelActor(&ActWork[actno2]);
		AllDelActor(&ActWork[ball[0]]);
		for(i = 1; i < TEMOTI_POKEMAX; i++)
			DelActor(&ActWork[ball[i]]);
	//	SetAdrs_Work(&TaskTable[id].work[14], ClientSequence[client_no]);
	}
	else if(TaskTable[id].work[15] == -3){	//�������S��Del����ĂȂ��̂ŗ]�v��1SyncWait
		*(vu16*)REG_BLDCNT = 0;
		*(vu16*)REG_BLDALPHA = 0;
		DelTask(id);
	}
}

static void TemotiGaugeSeq(actWork *xreg)
{
	if(xreg->dx != 0)
		xreg->dx += xreg->work[0];
}

//-- ���񎞂̎莝���Q�[�W�������Ă��� --//
#define TEMOTI_GAUGE_OUTSP	0x20	//0x18
static void TemotiGaugeOutSeq(actWork *xreg)
{
//	xreg->dx += xreg->work[0];
	xreg->work[1] += TEMOTI_GAUGE_OUTSP;
	if(xreg->work[0] > 0)
		xreg->dx += xreg->work[1] >> 4;
	else
		xreg->dx -= xreg->work[1] >> 4;
	xreg->work[1] &= 0x000f;
}

#define TEMOTI_BALLIN_SP	0x0038		//�ްق��݂��Ă��鑬�x(����4�r�b�g����)
static void TemotiBallSeq(actWork *xreg)
{
	u8 mine_enemy;
	u16 dx;
	s8 pan;
	
	if(xreg->work[1] > 0){
		xreg->work[1]--;
		return;
	}
	
	mine_enemy = xreg->work[2];
	dx = xreg->work[3];
	dx += TEMOTI_BALLIN_SP;
	xreg->work[3] = dx & 0xfff0;
	if(mine_enemy){
		xreg->dx += dx >> 4;
		if(xreg->dx > 0)
			xreg->dx = 0;
	}
	else{
		xreg->dx -= dx >> 4;
		if(xreg->dx < 0)
			xreg->dx = 0;
	}
	
	if(xreg->dx == 0){
		if(mine_enemy)
			pan = WAZA_SE_L;
		else
			pan = WAZA_SE_R;
		if(xreg->work[7])
			SePlayPanCh2(SE_TB_KARA, pan);	//BallOFF
		else
			SePlayPanCh1(SE_TB_KON, pan);
		xreg->move = DummyActMove;
	}
}

static void TemotiBallOutSeq(actWork *xreg)
{
	u8 mine_enemy;
	u16 dx;
	
	if(xreg->work[1] > 0){
		xreg->work[1]--;
		return;
	}
	
	mine_enemy = xreg->work[2];
	dx = xreg->work[3];
	dx += TEMOTI_BALLIN_SP;
	xreg->work[3] = dx & 0xfff0;
	if(mine_enemy){
		xreg->dx += dx >> 4;
	}
	else{
		xreg->dx -= dx >> 4;
	}
	
	if(xreg->dx+xreg->x > 248 || xreg->dx+xreg->x < -8){
		xreg->banish = 1;
		xreg->move = DummyActMove;
	}
}

//-- ����ȊO�̎��̓��� --//
static void TemotiBallNormalSeq(actWork *xreg)
{
	u8 hontai;
	
	hontai = xreg->work[0];
	xreg->dx = ActWork[hontai].dx;
	xreg->dy = ActWork[hontai].dy;
}

















//----------------------------------------------------------
//	���O�]��
//----------------------------------------------------------
#define SEIGYO_LEN	6	//�ް�ނ̖��O�Ŏg�p���Ă��鐧�亰�ނ̕�����
#define SEIGYO_TOPLEN	3	//���O�����̐��亰��(�Y�A���L���̐��亰�ނ͊܂܂Ȃ�)
static const u8 GaugeFontBaseColor_msg[] = {CTRL_,C_BCOL_,0x2,EOM_};
void	GaugePokeNameTrans(u8 actno,PokemonParam *PP)
{
	int	i;
	u8 j;
	u16	k;
	u8	*adr1;
	u32	adr2;
	u8 dakuten;
	u8 offset = 0;
	u8 sex;
	u8	nickname[MONS_NAME_SIZE+EOM_SIZE];
	u16 monsno;
	
	PM_strcpy(FightMsgBuf, GaugeFontBaseColor_msg);
//	adr1=(u8 *)PM_strcpy(&FightMsgBuf[2],PokeNameData[PP->PPP.monsno]);
	PokeParaGet(PP,ID_nickname,(u8 *)&nickname);
	NickNameCutOff(nickname);
	adr1=(u8 *)PM_strcpy(&FightMsgBuf[3],nickname);

//	*adr1++ = FCOL_;
//	*adr1++ = 0xd;//COL_LBLUE;
	*adr1++ = CTRL_;
	*adr1++ = C_FCOL_;
//	*adr1++ = 0xd;
	sex = PokeSexGet(PP);
	monsno = PokeParaGet(PP, ID_monsno);
	if(monsno == MONSNO_NIDORAN_F || monsno == MONSNO_NIDORAN_M){
		if(PM_strcmp(nickname, PokeNameData[monsno]) == 0)
			sex = 100;	//Ư�Ȱт���̫�Ė��ƈꏏ�Ȃ琫�ʂ�\�����Ȃ�
	}
	
	switch(sex){
		case	MONS_MALE:
			*adr1++ = 0xb;	//̫�ĐF
			*adr1=osu_;
			adr1++;
			*adr1=EOM_;
			break;
		case	MONS_FEMALE:
			*adr1++ = 0xa;	//̫�ĐF
			*adr1=mesu_;
			adr1++;
			*adr1=EOM_;
			break;
		default:
			*adr1++ = 0xb;	//̫�ĐF
			*adr1 = EOM_;
			break;
	}

//	adr1 = (void*)(GAUGE_DECORD_ADR + ((NAME_CHR_OFFSET << 5)
//				+ 32 * 6 * 2 * ActWork[actno].work[6]));
//				changed by soga 2001.09.26
	adr1 = (void*)(GAUGE_DECORD_ADR + ((NAME_CHR_OFFSET << 5)
				+ 32 * 6 * 2 * ClientTypeGet(ActWork[actno].work[6])));

	k = PM_strlen(FightMsgBuf);
	i = k - SEIGYO_LEN;	//����R�[�h��1�̕����Ƃ��Đ������Ă��܂��̂ŁA���̕�����
	j = 0;
	for( ; i < 6; i++, j++)	//6�����ɒB���ĂȂ��Ȃ���ͽ�߰��Ŗ��߂�
	{
		FightMsgBuf[k+j] = spc_;
		FightMsgBuf[k+j+1] = EOM_;
	}
	k=PM_strlen(FightMsgBuf);
	k -= SEIGYO_LEN;//����R�[�h��1�̕����Ƃ��Đ������Ă��܂��̂ŁA���̕�����
	
	MsgCGXDataSet(adr1,(u8 *)&FightMsgBuf);	//ү�����ް��W�J
	for(i = SEIGYO_TOPLEN, j = 0; j < k; i++)
	{
		if((FightMsgBuf[i] >= ga_ && FightMsgBuf[i] <= bo_)
			|| (FightMsgBuf[i] >= GA_ && FightMsgBuf[i] <= BO_))
		{	//���_���̳���޳��ׂ𑗂�
			dakuten = GP_DAKUTEN;
		}
		else if((FightMsgBuf[i] >= pa_ && FightMsgBuf[i] <= po_)
			|| (FightMsgBuf[i] >= PA_ && FightMsgBuf[i] <= PO_))
		{	//�����_���̳���޳��ׂ𑗂�
			dakuten = GP_HANDAKUTEN;
		}
		else
		{	//���_�Ȃ��̳���޳��ׂ𑗂�
			dakuten = GP_TENNASI;
		}
		CpuCopy(GetGaugePartsAdrs(dakuten), &adr1[j * 0x40], 0x20, 32);
		j++;
	}
	
/*-------------------------
	if(MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE)
	{
		for(i=0;i<k;i++)
		{
			//�{��
			if(i < 5)	//�c��́���ϰ������Ȃ̂ő��_�̏����͂��Ȃ�
			{
				//���_
				adr2=((ActWork[actno].oamData.CharNo+3+i+DEBUG_DAKUTEN) << 5)
					 + OBJ_MODE0_VRAM ;
				//DmaTransReq(adr1,adr2,0x20);	//change 2001.07.25(��)
				CpuCopy(adr1, adr2, 0x20, 32);
				adr1+=0x20;
				adr2=((ActWork[actno].oamData.CharNo+11+i) << 5) + OBJ_MODE0_VRAM ;
			}
			else
			{
				//���_
				adr2=((ActWork[actno].oamData.CharNo+32+(i-5)+DEBUG_DAKUTEN) << 5)
					 + OBJ_MODE0_VRAM ;
				//DmaTransReq(adr1,adr2,0x20);	//change 2001.07.25(��)
				CpuCopy(adr1, adr2, 0x20, 32);
				adr1+=0x20;
				adr2=((ActWork[actno].oamData.CharNo+32+8+(i-5)) << 5) + OBJ_MODE0_VRAM ;
			}
			//DmaTransReq(adr1,adr2,0x20);	//change 2001.07.25(��)
			CpuCopy(adr1, adr2, 0x20, 32);
			adr1+=0x20;
		}
	}
	else
	{
	-------------------------------*/
	if(MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE && FightCheck() == 0)
	{
		for(i = 0; i < k; i++)
		{
			//���_
			adr2=((ActWork[actno].oamData.CharNo+2+i+offset+DEBUG_DAKUTEN) << 5)
				 + OBJ_MODE0_VRAM ;
			//DmaTransReq(adr1,adr2,0x20);	//change 2001.07.25(��)
			CpuCopy(adr1,adr2,0x20,32);
			adr1+=0x20;
			
			//�{��
			adr2=((ActWork[actno].oamData.CharNo+10+i+offset) << 5) + OBJ_MODE0_VRAM;
			//DmaTransReq(adr1,adr2,0x20);	//change 2001.07.25(��)
			CpuCopy(adr1,adr2,0x20,32);
			adr1+=0x20;
		}
	}
	else
	{
		if(MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE)
			offset = 1;
			
		for(i = 0; i < k; i++)
		{
			//���_
			adr2=((ActWork[actno].oamData.CharNo+1+i+offset+DEBUG_DAKUTEN) << 5)
				 + OBJ_MODE0_VRAM ;
			//DmaTransReq(adr1,adr2,0x20);	//change 2001.07.25(��)
			CpuCopy(adr1,adr2,0x20,32);
			adr1+=0x20;
			
			//�{��
			adr2=((ActWork[actno].oamData.CharNo+9+i+offset) << 5) + OBJ_MODE0_VRAM;
			//DmaTransReq(adr1,adr2,0x20);	//change 2001.07.25(��)
			CpuCopy(adr1,adr2,0x20,32);
			adr1+=0x20;
		}
	}
}


//----------------------------------------------------------
//	�|�P�����Q�b�g�ς݃}�[�N�\��
//	put_mode = 0:elase 1:write
//----------------------------------------------------------
void GaugeGetBallTrans(u8 actno, u8 put_mode)
{
#ifndef FESTA_VER_ROM
	u8 client_no;
	u16 zukanno;
	u8 parts;
	
//�ߊl�f���ł́A�}�[�N�͂���Ȃ� add by soga 2002.06.27
	if(FightType&FIGHT_TYPE_GET_DEMO)
		return;
	if(FightType & FIGHT_TYPE_TRAINER)	//�ڰŰ��ł�����Ȃ� 2002.08.09(��)
		return;
		
	client_no = ActWork[actno].work[6];
	if(MineEnemyCheck(client_no) == SIDE_MINE)
		return;
	zukanno = PokeMonsNo2ZukanNoGet(
		PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_monsno));
	if(ZukanCheck(zukanno, Z_GET_CHK) == 0)
		return;

	parts = ActWork[actno].work[5];
	if(put_mode){
		CpuCopy((void*)GetGaugePartsAdrs(GP_GETBALL),
			((ActWork[parts].oamData.CharNo + 8) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
	}
	else{
		CpuClear(0, 
			((ActWork[parts].oamData.CharNo + 8) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
	}
#endif
}

//----------------------------------------------------------
//	��Ԉُ� �\��
//----------------------------------------------------------
//������ �ŁA��ჁA����A�X�A�₯�� ����گ�
static const u16 StPalData[] = {0x6198,0x0ef7,0x4694,0x72d1,0x29dc};

void GaugeStatusTrans(u8 actno)
{
	u8 client_no;
	u32 cond;
	u8 *parts;
	s16 offset;
	u8 parts_act;
	int i;
	u16 trans_pal;
	u8 st_pal = 0;
	
//	if(FightCheck() == 1)
//		return;

	client_no = ActWork[actno].work[6];
	parts_act = ActWork[actno].work[5];
#if GAUGE_CHANGE
	if(MineEnemyCheck(client_no) == SIDE_MINE)
	{
		cond = PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_condition);
		if(FightCheck() == 1)
			offset = 0xb;
		else
			offset = 0;
	}
	else
	{
		cond = PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_condition);
		if(FightCheck() == 1)
			offset = 0xb;
		else
			offset = -0x9;	//0x40;
	}
#else
	if(MineEnemyCheck(client_no) == SIDE_MINE){
		cond = PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_condition);
		if(FightCheck() == 0)
			offset = 0x1a;
		else
			offset = 0x12;
	}else{
		cond = PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_condition);
		offset = 0x11;
	}
#endif

	if(cond & CONDITION_NEMURI){
		parts = GetGaugePartsAdrs(GetCondID(GP_NEMURI, client_no));
		st_pal = 2;
	}
	else if(cond & CONDITION_DOKU || cond & CONDITION_DOKUDOKU){
		parts = GetGaugePartsAdrs(GetCondID(GP_DOKU, client_no));
		st_pal = 0;
	}
	else if(cond & CONDITION_YAKEDO){
		parts = GetGaugePartsAdrs(GetCondID(GP_YAKEDO, client_no));
		st_pal = 4;
	}
	else if(cond & CONDITION_KOORI){
		parts = GetGaugePartsAdrs(GetCondID(GP_KOORI, client_no));
		st_pal = 3;
	}
	else if(cond & CONDITION_MAHI){
		parts = GetGaugePartsAdrs(GetCondID(GP_MAHI, client_no));
		st_pal = 1;
	}
	else
	{	//��Ԉُ�Ȃ�
		
	#if GAUGE_CHANGE	//��Ԉُ�̕\���ꏊ���ς�����̂�
		if(FightCheck() == 0)
		{
			parts = GetGaugePartsAdrs(GP_KUUHAKU);
///			DIV_DMACOPY(3, (void*)parts, 
//				((ActWork[actno].oamData.CharNo + 0x1a+offset) << 5)
//				 + OBJ_MODE0_VRAM, 0x20, 32);
//			DIV_DMACOPY(3, (void*)parts, 
//				((ActWork[actno].oamData.CharNo + 0x1b+offset) << 5)
//				 + OBJ_MODE0_VRAM, 0x20, 32);
//			DIV_DMACOPY(3, (void*)parts, 
//				((ActWork[actno].oamData.CharNo + 0x1c+offset) << 5)
//				 + OBJ_MODE0_VRAM, 0x20, 32);
			CpuCopy((void*)parts, 
				((ActWork[actno].oamData.CharNo + 0x1a+offset) << 5)
				 + OBJ_MODE0_VRAM, 0x20, 32);
			CpuCopy((void*)parts, 
				((ActWork[actno].oamData.CharNo + 0x1b+offset) << 5)
				 + OBJ_MODE0_VRAM, 0x20, 32);
			CpuCopy((void*)parts, 
				((ActWork[actno].oamData.CharNo + 0x1c+offset) << 5)
				 + OBJ_MODE0_VRAM, 0x20, 32);
		}
		else{
			parts = GetGaugePartsAdrs(GP_LV);
//			DIV_DMACOPY(3, (void*)parts, 
//				((ActWork[actno].oamData.CharNo + 0xf) << 5)
//				 + OBJ_MODE0_VRAM, 0x20, 32);
			CpuCopy((void*)parts, 
				((ActWork[actno].oamData.CharNo + 0xf) << 5)
				 + OBJ_MODE0_VRAM, 0x20, 32);
			if(MineEnemyCheck(client_no) == SIDE_MINE)
				GaugeLevelTrans(actno,
					PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_level));
			else
				GaugeLevelTrans(actno,
					PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_level));
		}
	#else
		//-- �X�e�[�^�X���ر --//
		parts = GetGaugePartsAdrs(GP_KUUHAKU);
		for(i = 0; i < 3; i++){
//			DIV_DMACOPY(3, (void*)parts, 
//				((ActWork[actno].oamData.CharNo + offset+i) << 5)
//						 + OBJ_MODE0_VRAM, 0x20, 32);
			CpuCopy((void*)parts, 
				((ActWork[actno].oamData.CharNo + offset+i) << 5)
						 + OBJ_MODE0_VRAM, 0x20, 32);
		}
		if(Climem[client_no].gauge_mode == 0){
			//-- HP�̕\���𕜋A --//
//			DIV_DMACOPY(3, GetGaugePartsAdrs(GP_HP), 
//				(ActWork[parts_act].oamData.CharNo << 5) + OBJ_MODE0_VRAM, 0x40, 32);
			CpuCopy(GetGaugePartsAdrs(GP_HP), 
				(ActWork[parts_act].oamData.CharNo << 5) + OBJ_MODE0_VRAM, 0x40, 32);
		}
		
		GaugeGetBallTrans(actno, 1);
	#endif
		return;
	}

#if GAUGE_CHANGE
//	DIV_DMACOPY(3, (void*)parts,
//		((ActWork[actno].oamData.CharNo + 0x1a+offset) << 5) + OBJ_MODE0_VRAM, 0x60,32);
	CpuCopy((void*)parts,
		((ActWork[actno].oamData.CharNo + 0x1a+offset) << 5) + OBJ_MODE0_VRAM, 0x60,32);
	if(FightCheck())
//		DIV_DMACOPY(3, GetGaugePartsAdrs(GP_KUUHAKU),
//			((ActWork[actno].oamData.CharNo + 0xf) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
		CpuCopy(GetGaugePartsAdrs(GP_KUUHAKU),
			((ActWork[actno].oamData.CharNo + 0xf) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
#else
	trans_pal = ActWork[actno].oamData.Pltt * 16 + 0xc + client_no;
//	DIV_DMACOPY(3, (void*)parts, 
//		((ActWork[actno].oamData.CharNo + offset) << 5) + OBJ_MODE0_VRAM, 0x60, 32);
	PaletteWorkClear(StPalData[st_pal], trans_pal+16*16, 2);
	CpuCopy((void*)&PaletteWork[trans_pal+16*16], OBJ_PLTT+trans_pal*2, 2,16);
	CpuCopy((void*)parts, 
		((ActWork[actno].oamData.CharNo + offset) << 5) + OBJ_MODE0_VRAM, 0x60, 32);
	
	if(FightCheck() == 1 || MineEnemyCheck(client_no) == SIDE_ENEMY){
		if(Climem[client_no].gauge_mode == 0){
			//-- HP�̕\�������� --//
//			DIV_DMACOPY(3, GetGaugePartsAdrs(GP_SPACE), 
//				(ActWork[parts_act].oamData.CharNo << 5) + OBJ_MODE0_VRAM, 0x20, 32);
//			DIV_DMACOPY(3, GetGaugePartsAdrs(GP_HPHEAD), 
//				((ActWork[parts_act].oamData.CharNo +1)<< 5)+OBJ_MODE0_VRAM, 0x20, 32);
			CpuCopy(GetGaugePartsAdrs(GP_SPACE), 
				(ActWork[parts_act].oamData.CharNo << 5) + OBJ_MODE0_VRAM, 0x20, 32);
			CpuCopy(GetGaugePartsAdrs(GP_HPHEAD), 
				((ActWork[parts_act].oamData.CharNo +1)<< 5)+OBJ_MODE0_VRAM, 0x20, 32);
		}
	}
	GaugeGetBallTrans(actno, 0);
#endif
}

//----------------------------------------------------------
//	��Ԉُ�̃p�[�c�擾�̂��߂�ID���擾
//----------------------------------------------------------
static u8 GetCondID(u8 cond_id, u8 client_no)
{
	u8 new_id = cond_id;
	
	switch(cond_id){
		case GP_DOKU:
			if(client_no == 0)
				new_id = GP_DOKU;
			else if(client_no == 1)
				new_id = GP_DOKU1;
			else if(client_no == 2)
				new_id = GP_DOKU2;
			else
				new_id = GP_DOKU3;
			break;
		case GP_MAHI:
			if(client_no == 0)
				new_id = GP_MAHI;
			else if(client_no == 1)
				new_id = GP_MAHI1;
			else if(client_no == 2)
				new_id = GP_MAHI2;
			else
				new_id = GP_MAHI3;
			break;
		case GP_NEMURI:
			if(client_no == 0)
				new_id = GP_NEMURI;
			else if(client_no == 1)
				new_id = GP_NEMURI1;
			else if(client_no == 2)
				new_id = GP_NEMURI2;
			else
				new_id = GP_NEMURI3;
			break;
		case GP_KOORI:
			if(client_no == 0)
				new_id = GP_KOORI;
			else if(client_no == 1)
				new_id = GP_KOORI1;
			else if(client_no == 2)
				new_id = GP_KOORI2;
			else
				new_id = GP_KOORI3;
			break;
		case GP_YAKEDO:
			if(client_no == 0)
				new_id = GP_YAKEDO;
			else if(client_no == 1)
				new_id = GP_YAKEDO1;
			else if(client_no == 2)
				new_id = GP_YAKEDO2;
			else
				new_id = GP_YAKEDO3;
			break;
	}
	return new_id;
}

//----------------------------------------------------------
//	�T�t�@���{�[���]��
//----------------------------------------------------------

void	GaugeSafariBallTrans(u8 actno)
{
	int	i;
	u8 j;
	u16	k;
	u8	*adr1;
	u32	adr2;
	u8 dakuten;
	
	adr1 = (void*)(GAUGE_DECORD_ADR + ((NAME_CHR_OFFSET << 5)
				+ 32 * 6 * 2 * ClientTypeGet(ActWork[actno].work[6])));

//	k=PM_strlen(SafariBallMsg);
	k=7;
	
	MsgCGXDataSet(adr1,(u8 *)&SafariBallMsg);	//ү�����ް��W�J
	for(i = SEIGYO_TOPLEN, j = 0; j < k; i++)
	{
		if((SafariBallMsg[i] >= ga_ && SafariBallMsg[i] <= bo_)
			|| (SafariBallMsg[i] >= GA_ && SafariBallMsg[i] <= BO_))
		{	//���_���̳���޳��ׂ𑗂�
			dakuten = GP_DAKUTEN;
		}
		else if((SafariBallMsg[i] >= pa_ && SafariBallMsg[i] <= po_)
			|| (SafariBallMsg[i] >= PA_ && SafariBallMsg[i] <= PO_))
		{	//�����_���̳���޳��ׂ𑗂�
			dakuten = GP_HANDAKUTEN;
		}
		else
		{	//���_�Ȃ��̳���޳��ׂ𑗂�
			dakuten = GP_TENNASI;
		}
		CpuCopy(GetGaugePartsAdrs(dakuten), &adr1[j * 0x40], 0x20, 32);
		j++;
	}

	for(i = 3; i < k+3; i++)
	{
		//���_
		adr2=((ActWork[actno].oamData.CharNo+(i%8)+(0x40*(i/8))) << 5)
			 + OBJ_MODE0_VRAM ;
		CpuCopy(adr1,adr2,0x20,32);
		adr1+=0x20;
		
		//�{��
		adr2=((ActWork[actno].oamData.CharNo+8+(i%8)+(0x40*(i/8))) << 5) + OBJ_MODE0_VRAM;
		CpuCopy(adr1,adr2,0x20,32);
		adr1+=0x20;
	}
}

void	GaugeNokoriBallTrans(u8 actno)
{
	int	i;
	u16	k;
	u8	*adr1;
	u32	adr2;

	adr1=PM_strcpy(FightMsgBuf,NokoriMsg);
	adr1=PM_NumMsgSet(adr1,SafariBallCount,NUM_MODE_SPACE,2);
	PM_strcat(adr1,NankoMsg);
	
	adr1 = (void*)(GAUGE_DECORD_ADR + ((NAME_CHR_OFFSET << 5)
				+ 32 * 6 * 2 * ClientTypeGet(ActWork[actno].work[6])));

	k=7;
	
	MsgCGXDataSet(adr1,(u8 *)&FightMsgBuf);	//ү�����ް��W�J
	adr1+=0x20;
	for(i = 4; i < k+4; i++)
	{
		//�{��
		adr2=((ActWork[actno].oamData.CharNo+0x18+(i%8)+(0x40*(i/8))) << 5) + OBJ_MODE0_VRAM;
		CpuCopy(adr1,adr2,0x20,32);
		adr1+=0x40;
	}
}

//==================================================================
//	�ް�ނ̓]����ׂ�Ă���
// ���� : actno = �ް�ނ�actno
//        *PokePara = �\���Ώۂ��߹�݂����Ұ����߲��
//        mode = ����\�����邩(gauge.h��enum�Q��)
// �߂�l : �Ȃ�
//==================================================================
void GaugeTransChrSet(u8 actno, PokemonParam *PokePara, u8 mode)
{
	u16 pokeno;
	u8 pokelevel;
	//u16 nowEXP, nextEXP;
	s32 nowEXP, nextEXP;
//	u8 parts;
	u8 client_no;
	
	client_no = ActWork[actno].work[6];
	if(mode == G_ALL){	// add 2002.07.24(��)
		if(FightCheck() == 0 && MineEnemyCheck(client_no) == SIDE_MINE)
			;
		else{
			//���x���A�b�v���Ɉ�u[HP]�̕����������Ă��܂��̂ō폜
//			parts = ActWork[actno].work[5];
//			CpuClear(0, OBJ_MODE0_VRAM + ActWork[parts].oamData.CharNo*32, 8*32, 32);
		}
	}

	if(MineEnemyCheck(ActWork[actno].work[6]) == SIDE_MINE)	//user
	{
		if(mode == G_LV || mode == G_ALL)		//LV
			GaugeLevelTrans(actno, PokeParaGet(PokePara,ID_level));
	#if 1
		if(mode == G_HP || mode == G_ALL)		//HP(����)
			GaugeHPTrans(actno, PokeParaGet(PokePara,ID_hp), 0);
		if(mode == G_MAXHP || mode == G_ALL)	//MAX HP(����)
			GaugeHPTrans(actno, PokeParaGet(PokePara,ID_hpmax), 1);
	#endif
		if(mode == G_HPGAUGE || mode == G_ALL)	//HP�ް��
		{
			BattleGaugeTransWram(0);
			GaugeDataSet(client_no, actno, PokeParaGet(PokePara,ID_hpmax), PokeParaGet(PokePara,ID_hp), 0);
			GaugeOBJ(client_no, actno, 0, 0);
		//	GaugeOFF(actno);
		}
		if(FightCheck() == 0 && (mode == G_EXPGAUGE || mode == G_ALL))	//EXP�ް��
		{
			BattleGaugeTransWram(3);
			pokeno = PokeParaGet(PokePara,ID_monsno);
			pokelevel = PokeParaGet(PokePara,ID_level);
			nowEXP = PokeParaGet(PokePara,ID_exp) - GrowTbl[PPD[pokeno].grow][pokelevel];
			nextEXP = GrowTbl[PPD[pokeno].grow][pokelevel + 1] 
						- GrowTbl[PPD[pokeno].grow][pokelevel];
			GaugeDataSet(client_no, actno, nextEXP, nowEXP, 0);
			GaugeOBJ(client_no, actno, 1, 0);
		}
		if(mode == G_NAME || mode == G_ALL)				//NAME
			GaugePokeNameTrans(actno, PokePara);
		if(mode == G_STATUS || mode == G_ALL)			//��Ԉُ�
			GaugeStatusTrans(actno);
		if(mode == G_SAFARI)							//�T�t�@���{�[�����b�Z�[�W
			GaugeSafariBallTrans(actno);
		if(mode == G_SAFARI || mode == G_SAFARIBALL )	//�T�t�@���{�[���c���
			GaugeNokoriBallTrans(actno);
	}
	else
	{
		if(mode == G_LV || mode == G_ALL)		//LV
			GaugeLevelTrans(actno, PokeParaGet(PokePara,ID_level));
	#ifdef PM_DEBUG
		if(DebugMainFlag == 1){	//�f�o�b�O�p�ɓG��HP�������Ō�����悤�ɂ���
			if(mode == G_HP || mode == G_ALL)		//HP(����)
				GaugeHPTrans(actno, PokeParaGet(PokePara,ID_hp), 0);
			if(mode == G_MAXHP || mode == G_ALL)	//MAX HP(����)
				GaugeHPTrans(actno, PokeParaGet(PokePara,ID_hpmax), 1);
		}
	#endif
		if(mode == G_HPGAUGE || mode == G_ALL)	//HP�ް��
		{
			BattleGaugeTransWram(0);
			GaugeDataSet(client_no, actno, PokeParaGet(PokePara,ID_hpmax), PokeParaGet(PokePara,ID_hp), 0);
			GaugeOBJ(client_no, actno, 0, 0);
		//	GaugeOFF(actno);
		}
		if(mode == G_NAME || mode == G_ALL)		//NAME
			GaugePokeNameTrans(actno, PokePara);
	/*	//�G�̹ޯčς��ްقͽð���ُ�ŌĂ΂�邩�炱���ł͂���Ȃ�����
		if(mode == G_GETCHKBALL || mode == G_ALL)	//�ޯčς݂����ް�
			GaugeGetBallTrans(actno, 1);
	*/
		if(mode == G_STATUS || mode == G_ALL)	//��Ԉُ�
			GaugeStatusTrans(actno);
	}
}























//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if GAUGE_SOKUDO_TYPE

//	����Ɉ�葬�x�̃o�[�W����

//=========================================================================
//	�o�g����ʗp�Q�[�W
// �����炩���ߓ]������ް�޷�ׂ�ܰ��тɓW�J���Ă�������!!
// req_data = 0:HP�ް��  1:Exp�ް��
//=========================================================================
#define BATTLE_GAUGE_HPMAX	6
#define BATTLE_GAUGE_EXPMAX	8
#define BATTLE_GAUGE_MAX	BATTLE_GAUGE_EXPMAX		//���ق̹ް�ނň�ԑ傫���ް��

s32 GaugeOBJ(u8 client_no, u8 actno, u8 req_data, u16 *pl_buf)
{
	s32 temp;
	u16 dotto_offset;
	
	if(req_data == 0)
		temp = GaugeProc(gwork[client_no].MaxHP, gwork[client_no].NowHP, gwork[client_no].beHP,
					 &gwork[client_no].HP_Work, BATTLE_GAUGE_HPMAX, 1);
	else{	//EXP
		//EXP�ް�ނ͌v�Z���l���傫���̂ŁA�ޯĂ̵̾�Ă��牽Sync�ŏI��点�邩�����߂�
		dotto_offset = DottoOffsetCalc(gwork[client_no].NowHP, gwork[client_no].beHP, gwork[client_no].MaxHP,
				BATTLE_GAUGE_EXPMAX);
		if(dotto_offset == 0)
			dotto_offset = 1;
		dotto_offset = abs(gwork[client_no].beHP / dotto_offset);
		temp = GaugeProc(gwork[client_no].MaxHP, gwork[client_no].NowHP, gwork[client_no].beHP,
				&gwork[client_no].HP_Work, BATTLE_GAUGE_EXPMAX, dotto_offset);
	}
	
	if(req_data == 1 || (req_data == 0 && Climem[client_no].gauge_mode == 0))
		PutGaugeOBJ(client_no, req_data);
	
	if(temp == -1)
		gwork[client_no].HP_Work = 0;
	return temp;
}


//==================================================================
//		�Q�[�W�\��
//
// ���� : 
// �߂�l : �Ȃ�
//==================================================================
void PutGaugeOBJ(u8 client_no, u8 req)
{
	u8 i;
	u8 gauge_chr[BATTLE_GAUGE_MAX];
	u8 parts, parts_num;
	u8 put_dot;
	u8 level;
	
	if(1)//MineEnemyCheck(ActWork[gwork[client_no].actno].work[6]) == SIDE_MINE)	//user
	{
		switch(req)
		{
			case 0:	//HP
				put_dot = PutGaugeProc(gwork[client_no].MaxHP, gwork[client_no].NowHP, gwork[client_no].beHP,
					 &gwork[client_no].HP_Work, gauge_chr, BATTLE_GAUGE_HPMAX);
				
				//���������̌v�Z����ς�����GetGaugeDottoColor���ύX���邱��!!
				if(put_dot > GAUGE_DOTTOMAX / 2)	//2002.07.11(��) by matsuda
					parts_num = GP_HPBAR;
				else if(put_dot > 9)
					parts_num = GP_HPBAR_YELLOW;
				else
					parts_num = GP_HPBAR_RED;
				for(i = 0; i < BATTLE_GAUGE_HPMAX; i++)
				{
					parts = ActWork[gwork[client_no].actno].work[5];
					if(i < 2)
					{
						CpuCopy((void*)(GetGaugePartsAdrs(parts_num) 
							+ (gauge_chr[i] << 5)),
							((ActWork[parts].oamData.CharNo 
							+ 2 + i) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
					}else
					{
						CpuCopy((void*)(GetGaugePartsAdrs(parts_num)
							 + (gauge_chr[i] << 5)),
							 ((ActWork[parts].oamData.CharNo 
							 + 4 + i-2) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
					}
				}
				break;
			case 1:	//Exp
				PutGaugeProc(gwork[client_no].MaxHP, gwork[client_no].NowHP, gwork[client_no].beHP, &gwork[client_no].HP_Work,
					 gauge_chr, BATTLE_GAUGE_EXPMAX);
				level = PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_level);
				if(level == 100){
					for(i = 0; i < BATTLE_GAUGE_EXPMAX; i++)
						gauge_chr[i] = 0;
				}
				for(i = 0; i < BATTLE_GAUGE_EXPMAX; i++)
				{
					if(i < 4){
/*						DIV_DMACOPY(3, 
							(void*)(GetGaugePartsAdrs(GP_EXPBAR) + (gauge_chr[i] << 5)),
							((ActWork[gwork[client_no].actno].oamData.CharNo +0x24 + i) << 5)
							+ OBJ_MODE0_VRAM, 0x20, 32);
*/
						CpuCopy(
							(void*)(GetGaugePartsAdrs(GP_EXPBAR) + (gauge_chr[i] << 5)),
							((ActWork[gwork[client_no].actno].oamData.CharNo +0x24 + i) << 5)
							+ OBJ_MODE0_VRAM, 0x20, 32);
					}else{
/*						DIV_DMACOPY(3, 
							(void*)(GetGaugePartsAdrs(GP_EXPBAR) + (gauge_chr[i] << 5)),
							((ActWork[gwork[client_no].actno].oamData.CharNo +0x60 + i-4) << 5)
							+ OBJ_MODE0_VRAM, 0x20, 32);
*/
						CpuCopy(
							(void*)(GetGaugePartsAdrs(GP_EXPBAR) + (gauge_chr[i] << 5)),
							((ActWork[gwork[client_no].actno].oamData.CharNo +0x60 + i-4) << 5)
							+ OBJ_MODE0_VRAM, 0x20, 32);
					}
				}
				break;
		}
	}
	else
	{
		PutGaugeProc(gwork[client_no].MaxHP, gwork[client_no].NowHP, gwork[client_no].beHP, &gwork[client_no].HP_Work,
				 gauge_chr, BATTLE_GAUGE_HPMAX);

		for(i = 0; i < BATTLE_GAUGE_HPMAX; i++)
		{
			if(i < 4)
			{
				CpuCopy(
					(void*)(GAUGE_DECORD_ADR + ((gauge_chr[i]+HP_CHR_OFFSET) << 5)),
					((ActWork[gwork[client_no].actno].oamData.CharNo 
					+ ENEMY_HPGAUGE_POS1 + i) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
			}
			else
			{
				CpuCopy(
					(void*)(GAUGE_DECORD_ADR + ((gauge_chr[i]+HP_CHR_OFFSET) << 5)),
					((ActWork[gwork[client_no].actno].oamData.CharNo 
					+ ENEMY_HPGAUGE_POS2 + (i-4)) << 5) + OBJ_MODE0_VRAM, 0x20, 32);
			}
		}
	}
}


//==================================================================
//			�Q�[�W�v�Z		��BG,OBJ����
//	HP_Work : MaxHP��GaugeMax*8�ȏ�Ȃ�A����̲ݸ���āA�޸���Ă���Ă�l��
//	�����Ă�BGaugeMax*8�����̎��͉���8�ޯď����ŏ��8�ޯĂɲݸ���āA
//	�޸���Ă���Ă�l������B
//	add_dec : �ް�ނ������Z���Ă����l GaugeDotto��GaugeMax�ȉ��̏ꍇ�͖�������
//
//	�߂�l: -1 = �I��	����ȊO�F�v�Z�r���̐���(��ʂɏo�����l)
//==================================================================
static s32 GaugeProc(s32 MaxHP, s32 NowHP, s32 beHP, s32 *HP_Work, u8 GaugeMax, u16 add_dec)
{
//	s16 endHP;
	s32 endHP;	//2002.09.14(�y)
	s32 ret;
	u8 GaugeMaxDot;
	s32 add_hp;
	
	GaugeMaxDot = GaugeMax * GAUGE_DOTTO;
	
	//-- HP_Work��NowHP������̎��͏���Ȃ̂�ܰ��������� --//
//	if(*HP_Work == NowHP){
//		if(MaxHP < GaugeMaxDot)
//			*HP_Work <<= 8;
//	}
	if(*HP_Work == HP_WORK_INIT_VALUE){
		if(MaxHP < GaugeMaxDot)
			*HP_Work = NowHP << 8;
		else
			*HP_Work = NowHP;
	}

	//-- �I������ --//
	endHP = NowHP - beHP;
	if(endHP < 0)
		endHP = 0;
	else if(endHP > MaxHP)
		endHP = MaxHP;
	
	if(MaxHP < GaugeMaxDot){			//����������������
		if(endHP == (*HP_Work >> 8) && (*HP_Work & 0xff) == 0)
			return (-1);
	}
	else{
		if(endHP == *HP_Work)
			return (-1);
	}
	//-- �I�������I�� --//
	
	if(MaxHP < GaugeMaxDot){
		add_hp = MaxHP * 0x100 / GaugeMaxDot;
		if(beHP < 0){	//�ް�މ�
			*HP_Work += add_hp;
			ret = *HP_Work >> 8;
			if(ret >= endHP){	//�����������Ă���̂ŵ��ް�۰����
				*HP_Work = endHP << 8;
				ret = endHP;
			}
		}
		else{					//�ް�ތ���
			*HP_Work -= add_hp;
			ret = *HP_Work >> 8;
			if((*HP_Work & 0xff) > 0)	//��������������Ȃ班���J��グ�ŕ\��
				ret++;
			if(ret <= endHP){	//�����������Ă���̂ŵ��ް�۰����
				*HP_Work = endHP << 8;
				ret = endHP;
			}
		}
	}
	else{
		if(beHP < 0){		//�ް�މ�
			*HP_Work += add_dec;
			if(*HP_Work > endHP)
				*HP_Work = endHP;
		}
		else{				//�ް�ތ���
			*HP_Work -= add_dec;
			if(*HP_Work < endHP)
				*HP_Work = endHP;
		}
		ret = *HP_Work;
	}
	return ret;
}


//=========================================================================
//	�ް�ޕ\���v�Z	��BG,OBJ����
//
//  *gauge_chr�ɕ\������ް�ނ̒l�������ĕԂ�
//    0:����ۂ̹ް��  1:1�ޯĂ̹ް�� 2:2�ޯĂ̹ް�� ��� 8:8�ޯ�(����)�̹ް��
//
//	�߂�l:�\������h�b�g��
//=========================================================================
static u8 PutGaugeProc(s32 MaxHP, s32 NowHP, s32 beHP, s32 *HP_Work,
	 u8 *gauge_chr, u8 GaugeMax)
{
	u8 i;
	u8 GaugeMaxDot;
	u8 put_dot, dot_ret;		//�\�������ޯĐ�
	s32 AfterHP;
	
	AfterHP = NowHP - beHP;
	if(AfterHP < 0)
		AfterHP = 0;
	else if(AfterHP > MaxHP)
		AfterHP = MaxHP;

	GaugeMaxDot = GAUGE_DOTTO * GaugeMax;
	for(i = 0; i < GaugeMax; i++)	//�󔒹ް�ނŏ�����
		gauge_chr[i] = 0;

	//���������̃h�b�g�擾�̌v�Z����ύX������GetNumDotto�̌v�Z�����ύX���邱��!!
	if(MaxHP < GaugeMaxDot)	// by 2002.07.11(��) matsuda
		put_dot = (*HP_Work * GaugeMaxDot / MaxHP) >> 8;
	else
		put_dot = *HP_Work * GaugeMaxDot / MaxHP;
	dot_ret = put_dot;
	//�ޯČv�Z�ł�0�ł��A���ۂ�HP��1�ȏ゠��Ȃ�1�ޯĂ̹ް�ނ�\������
	if(put_dot == 0 && AfterHP > 0){
		gauge_chr[0] = 1;
		dot_ret = 1;
	}else{
		for(i = 0; i < GaugeMax; i++){
			if(put_dot >= GAUGE_DOTTO){
				gauge_chr[i] = 8;
				put_dot -= GAUGE_DOTTO;
			}
			else{
				gauge_chr[i] = put_dot;
				break;	//�ޯĂ��s�����̂Ŕ�����
			}
		}
	}
	return dot_ret;
}




//==================================================================
//			�Q�[�W�v�Z��\��(BG�p)
//
//    �� HP_Work:ܰ��Ƃ��Ďg�p �������HP_WORK_INIT_VALUE�����Ă�������
//
// ���� : MaxHP:�ő�HP
//		  NowHP:����HP(��Ұ�ނ��󂯂�O)
//		  beHP:�󂯂���Ұ��(�񕜂���ꍇ��ϲŽ�œ����)
//        *HP_Work : �r���o�߂�ۑ�������ׂ�ܰ��@�������0�����Ă�������!!
//		  *TransPos:�ް�ނ�\�������齸ذ��ް��̍��W
//		  TopChr:�ް�ނ̷�ׂ�į�߱��ڽ
//		  *pl_buf:��گ��ޯ̧
// �߂�l : -1:�I��		0�ȏ�̐��l:�ް�ނƈꏏ�ɕ\�������鐔�l�̒l
//==================================================================
s16 GaugeBG(GaugeDataBG *gdata, s32 *HP_Work, u16 *TransPos, u16 *pl_buf)
{
	s16 temp;
	u16 resultHP;

	temp = GaugeProc(gdata->MaxHP, gdata->NowHP, gdata->beHP, HP_Work, GAUGE_MAX, 1);
	PutGaugeBG(gdata, HP_Work, TransPos);
	
	if(gdata->MaxHP < GAUGE_DOTTO * GAUGE_MAX)
		resultHP = *HP_Work >> 8;
	else
		resultHP = *HP_Work;
	GaugeColChk(gdata->MaxHP, resultHP, pl_buf);
	
//	if(temp == -1)
//		*HP_Work = 0;
	return temp;
}

//==================================================================
//		�Q�[�W�\��(��ذ��ް���ύX����)
//
// ���� : GAUGE_TOPCHR:�ް�ނ̍ŏ��̷�ׂ��o�^����Ă��鷬����ް
// �߂�l : �Ȃ�
//==================================================================
static void PutGaugeBG(GaugeDataBG *gdata, s32 *HP_Work, u16 *TransPos)
{
	u8 gauge_chr[GAUGE_MAX];
	u16 DummyScrn[GAUGE_MAX];
	u8 i;

	PutGaugeProc(gdata->MaxHP, gdata->NowHP, gdata->beHP, HP_Work, gauge_chr, GAUGE_MAX);
	for(i = 0; i < GAUGE_MAX; i++)
		DummyScrn[i] = (gdata->color << 12) | (gdata->CharNo + gauge_chr[i]);
//	DIV_DMACOPY(3, DummyScrn, TransPos, GAUGE_MAX*2, 16);
	CpuCopy(DummyScrn, TransPos, GAUGE_MAX*2, 16);
}

//=========================================================================
//	�ް�ތv�Z�O�ƌv�Z��̍ŏI�I���ޯĂ̍��������߂�
//=========================================================================
static u8 DottoOffsetCalc(s32 nowHP, s32 beHP, s32 MaxHP, u8 GaugeMax)
{
	s8 now_dotto, end_dotto;
	u8 GaugeMaxDot;
	s32 endHP;
	GaugeMaxDot = GaugeMax * GAUGE_DOTTO;

	endHP = nowHP - beHP;
	if(endHP < 0)
		endHP = 0;
	else if(endHP > MaxHP)
		endHP = MaxHP;
	now_dotto = nowHP * GaugeMaxDot / MaxHP;
	end_dotto = endHP * GaugeMaxDot / MaxHP;
	return (abs(now_dotto - end_dotto));
}

//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
#else	//���ŏ������A���񂾂񌸑����Ă����o�[�W����
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================


/* �F�ς��悤�ɒʏ�̺��Ăł��������Ƃ�

//==================================================================
//			�Q�[�W�v�Z��\��(OBJ�p)
//
//    �� HP_Work:ܰ��Ƃ��Ďg�p ��0����ܰ��̏��������s���� �����0�����Ă�������
//
// ���� : MaxHP:�ő�HP
//		  NowHP:����HP(��Ұ�ނ��󂯂�O)
//		  beHP:�󂯂���Ұ��(�񕜂���ꍇ��ϲŽ�œ����)
//        *HP_Work : ܰ��̱��ڽ(�ް�ނ̓r���o�߂��ް�������)
//								���0�`3�ޯ� �������A 4�`7 �׸ށA 8�`15 ���q �Ŏg�p����
//		  *TransPos:�ް�ނ�\�������齸ذ��ް��̍��W
//		  TopChr:�ް�ނ̷�ׂ�į�߱��ڽ
//		  *pl_buf:��گ��ޯ̧
// �߂�l : -1:�I��		0�ȏ�̐��l:�ް�ނƈꏏ�ɕ\�������鐔�l�̒l
//==================================================================

//=========================================================================
//	�o�g����ʗp�Q�[�W
// �����炩���ߓ]������ް�޷�ׂ�ܰ��тɓW�J���Ă�������!!
// req_data = 0:HP�ް��  1:Exp�ް��
//=========================================================================
#define BATTLE_GAUGE_HPMAX	6
#define BATTLE_GAUGE_EXPMAX	8
#define BATTLE_GAUGE_MAX	BATTLE_GAUGE_EXPMAX		//���ق̹ް�ނň�ԑ傫���ް��
s16 GaugeOBJ(u8 actno, u8 req_data, u16 *pl_buf)
{
	GaugeDataOBJ *gdata;
	GaugeDataOBJ2 gdata2;
	
//	gdata = (GaugeDataOBJ *)&ActWork[ActWork[actno].work[7]].work[0];
	gdata = (GaugeDataOBJ *)&ActWork[actno].work[0];
	
	if(((gdata->HP_Work) & 0x00ff) == DEFAULT_BUNBO)	//�I������
	{	//�������������Ă���̂͑S�Ă̐�����߂�l�Ƃ��ĕԂ���1�T�����Ă���
		gdata->HP_Work = 0;
		return -1;	//�S�Ă̹ް�ނ��I�����I�����
	}
	
	if(gdata->beHP < 0)	//��	�ް�ޑ��₷
	{
		GaugeProc(gdata, &gdata2, 1);
		PutGaugeOBJ(gdata, &gdata2, ((gdata->HP_Work) & 0x00ff), req_data);
	}
	else	//��Ұ��  �ް�ތ��炷
	{
		GaugeProc(gdata, &gdata2, -1);
		PutGaugeOBJ(gdata, &gdata2, DEFAULT_BUNBO - ((gdata->HP_Work) & 0x00ff), req_data);
	}
	
//	GaugeColChk(gdata->MaxHP, resultHP, pl_buf);	���܂Ͷװ�ύX���Ȃ�
	
	if(gdata->beHP == 0)
	{	//beHP��0�Ȃ�΍��̹ް�ނ�\�����邾���Ȃ̂ō����ٰ�߂ŏI���ɂ���
		gdata->HP_Work = ((gdata->HP_Work) & 0xff00) | DEFAULT_BUNBO;
	}
	return gdata2.resultHP;
}

//==================================================================
//		�Q�[�W�\��
//
// ���� : 
// �߂�l : �Ȃ�
//==================================================================
static void PutGaugeOBJ(GaugeDataOBJ *gdata, GaugeDataOBJ2 *gdata2, u8 Bunsi, u8 req)
{
	u8 i;
	u8 gauge_chr[BATTLE_GAUGE_MAX];
	u8 parts;

//	i = PutGaugeProc(gdata, gdata2, Bunsi, gauge_chr, GaugeMax);
//	if(i == 1)
//	{
//		return;
//	}
	
#if 0
	if((ActWork[gdata->actno].work[6] & 1) == 0)	//user
#else	//changed by soga 2001.09.26
	if(1)//MineEnemyCheck(ActWork[gdata->actno].work[6]) == SIDE_MINE)	//user
#endif
	{
		switch(req)
		{
			case 0:	//HP
				i = PutGaugeProc(gdata, gdata2, Bunsi, gauge_chr, BATTLE_GAUGE_HPMAX);
				if(i == 1)
				{
					return;
				}
				
				for(i = 0; i < BATTLE_GAUGE_HPMAX; i++)
				{
					parts = ActWork[gdata->actno].work[5];
					if(i < 2)
					{
						DmaTransReq(
							(void*)(GetGaugePartsAdrs(GP_HPBAR) + (gauge_chr[i] << 5)),
							((ActWork[parts].oamData.CharNo 
							+ 2 + i) << 5) + OBJ_MODE0_VRAM, 0x20);
					}else
					{
						DmaTransReq(
							(void*)(GetGaugePartsAdrs(GP_HPBAR) + (gauge_chr[i] << 5)),
							((ActWork[parts].oamData.CharNo 
							+ 8 + i-2) << 5) + OBJ_MODE0_VRAM, 0x20);
					}
				}
				break;
			case 1:	//Exp
				i = PutGaugeProc(gdata, gdata2, Bunsi, gauge_chr, BATTLE_GAUGE_EXPMAX);

				for(i = 0; i < BATTLE_GAUGE_EXPMAX; i++)
				{
					if(i == 0 && gauge_chr[0] == 1)	//EXP��HP�ް�ނ���1/2�̂���
						gauge_chr[0] = 2;	//���ۂ̹ް�ޒl��0�łȂ��Ȃ�Œ�1��؂͕\��
					gauge_chr[i] /= 2;
					DmaTransReq(
						(void*)(GetGaugePartsAdrs(GP_EXPBAR) + (gauge_chr[i] << 5)),
						((ActWork[gdata->actno].oamData.CharNo +0x33 + i) << 5)
						+ OBJ_MODE0_VRAM, 0x20);
				}
				break;
		}
	}
	else
	{
		i = PutGaugeProc(gdata, gdata2, Bunsi, gauge_chr, BATTLE_GAUGE_HPMAX);
		if(i == 1)
		{
			return;
		}

		for(i = 0; i < BATTLE_GAUGE_HPMAX; i++)
		{
			if(i < 4)
			{
				DmaTransReq(
					(void*)(GAUGE_DECORD_ADR + ((gauge_chr[i]+HP_CHR_OFFSET) << 5)),
						((ActWork[gdata->actno].oamData.CharNo 
							+ ENEMY_HPGAUGE_POS1 + i) << 5) + OBJ_MODE0_VRAM, 0x20);
			}
			else
			{
				DmaTransReq(
					(void*)(GAUGE_DECORD_ADR + ((gauge_chr[i]+HP_CHR_OFFSET) << 5)),
						((ActWork[gdata->actno].oamData.CharNo 
							+ ENEMY_HPGAUGE_POS2 + (i-4)) << 5) + OBJ_MODE0_VRAM, 0x20);
			}
		}
	}
}





//=========================================================================
//	�ް�ޕ\���v�Z
//   ��GaugeProc�͉������������Ұ���̉����Z���ꂽ�l�����߂�
//     PutGaugeProc��GaugeProc���狁�ߏo���ꂽ�l��ް�ނƂ����G�ɕ\�����߂̌v�Z
//
//  *gauge_chr�ɕ\������ް�ނ̒l�������ĕԂ�
//    0:����ۂ̹ް��  1:1�ޯĂ̹ް�� 2:2�ޯĂ̹ް�� ��� 8:8�ޯ�(����)�̹ް��
//=========================================================================
static s8 PutGaugeProc(GaugeDataOBJ *gdata, GaugeDataOBJ2 *gdata2, u8 Bunsi, 
								u8 *gauge_chr, u8 GaugeMax)
{
	u8 i;
	u8 gauge_power, HP_power, Now_power, temp;
	u8 GaugeParsent;
	
	GaugeParsent = GAUGE_DOTTO * GaugeMax;
	
	for(i = 0; i < GaugeMax; i++)	//�󔒹ް�ނŏ�����
	{
		gauge_chr[i] = 0;
	}

#if 0	//EXP�ް�ނ�0���瑝���Ă����ꍇ�Ȃǂɂ�����1�Ɏ���܂ł̓r���o�߂��o�Ȃ��̂�
	if(gdata2->resultHP == 0)
	{
		return 1;	//�����I��
	}
#endif
	
	if(gdata->NowHP > gdata2->AfterHP)
	{	//���ް�ނ������Ȃ����������L����HP�̊���
		Now_power = PERCENT_CONV(gdata2->AfterHP, gdata->MaxHP, 100);
//		Now_power = 100 * gdata2->AfterHP / gdata->MaxHP;
		HP_power = PERCENT_CONV(gdata->NowHP, gdata->MaxHP, 100);
//		HP_power = 100 * gdata->NowHP / gdata->MaxHP;
		HP_power -= Now_power;				//�ް�ނ��ϓ����镔�������L����HP�̊���
	}
	else
	{
		//�ް�ނ������Ȃ����������L����HP�̊���
		Now_power = PERCENT_CONV(gdata->NowHP, gdata->MaxHP, 100);
		HP_power = PERCENT_CONV(gdata2->AfterHP, gdata->MaxHP, 100);
	//	Now_power= 100 * gdata->NowHP / gdata->MaxHP;//�ް�ނ������Ȃ����������L����HP�̊���
	//	HP_power = 100 * gdata2->AfterHP / gdata->MaxHP;
		HP_power -= Now_power;				//�ް�ނ��ϓ����镔�������L����HP�̊���
	}
	//DEFAULT_BUNBO����Bunsi��100%����Bunsi�ɕς���
	temp = PERCENT_CONV(Bunsi, DEFAULT_BUNBO, 100);
	//Bunsi����ް�ނ��\������镔���܂ł�%�����߂�
	HP_power = PERCENT_CONV(temp, 100, HP_power);
//	temp = 100 * Bunsi / DEFAULT_BUNBO;	//DEFAULT_BUNBO����Bunsi��100%����Bunsi�ɕς���
//	HP_power = HP_power * temp / 100;	//Bunsi����ް�ނ��\������镔���܂ł�%�����߂�
	HP_power += Now_power;	//�\�������߰��ð��

	
	for(i = 1; i <= GaugeMax; i++)
	{
		gauge_power = 100 * i / GaugeMax;	//�ް�޶��Ă܂ł����L����HP�̊�����
		
		if(HP_power < gauge_power)
		{
			//i��א��܂łɗ]���Ă��܂����������߂�(1��ׂ܂�܂�𖄂߂��Ȃ��[��)
			HP_power -= 100 * (i - 1) / GaugeMax;	//��%�]���Ă�̂�
			HP_power = GaugeParsent * HP_power / 100;
			
			if(i == 1 && HP_power == 0 && gdata2->AfterHP > 0)
			{	//�߰��ð�ނł�0�ł��A���ۂ�HP��1�ȏ゠��Ȃ�1�ޯĂ̹ް�ނ�\������
				gauge_chr[i-1] = 1;
			}
			else
			{	//�ʏ�
				gauge_chr[i-1] = HP_power;
			}
			break;
		}
		else
		{
			gauge_chr[i-1] = 8;
		}
	}
	
	return 0;	//����I��
}



//=========================================================================
//	BG,OBJ���ʌv�Z���[�`��
//=========================================================================

//==================================================================
//			�Q�[�W�v�Z
//	Direction:�ް�ނ𓮂��������̌���(1 �� -1)  �� 0�֎~!!
//==================================================================
static void GaugeProc(GaugeDataOBJ *gdata, GaugeDataOBJ2 *gdata2, s8 Direction)
{
	GaugeProcSub(&gdata->HP_Work);
	
	if(Direction > 0)	//�ް�މ�
	{
		if(gdata->NowHP + (-gdata->beHP) > gdata->MaxHP)
		{
			gdata->beHP = -(gdata->MaxHP - gdata->NowHP);
		}
		gdata2->AfterHP = gdata->NowHP + (-gdata->beHP);
		
		gdata2->resultHP = gdata->NowHP + (-gdata->beHP) 
								* (gdata->HP_Work&0x00ff) / DEFAULT_BUNBO;
		if(gdata2->resultHP > gdata->MaxHP)
		{
			gdata2->resultHP = gdata->MaxHP;
		}
	}
	else				//�ް�ތ��炷
	{
		if(gdata->NowHP - gdata->beHP < 0)
		{
			gdata->beHP = gdata->NowHP;
		}
		gdata2->AfterHP = gdata->NowHP - gdata->beHP;

		gdata2->resultHP = gdata->NowHP - gdata->beHP
								* (gdata->HP_Work&0x00ff) / DEFAULT_BUNBO;
		if(gdata2->resultHP < gdata2->AfterHP)
		{
			gdata2->resultHP = gdata2->AfterHP;
		}
	}
}
//=========================================================================
//		�ް�ނ̉���(�グ)���擾
//=========================================================================
static void GaugeProcSub(u16 *HP_Work)
{
	u8 param, flg, bunsi;
	
	if(*HP_Work == 0)	//ܰ�������
	{
		*HP_Work = DEFAULT_DOWN_PARAM << 12;//8;	//�ŏ��̉��������
	}

	bunsi = (*HP_Work) & 0x00ff;
	flg = (((*HP_Work) & 0x0f00) >> 8);
	param = (*HP_Work) >> 12;	//������
	
	if(bunsi > NEXT_BUNSI1)
	{
		if(flg == 0)	//�׸�����
		{
			param -= DOWN_BUNSI_PARAM;
			flg++;
		}
		else if(param > 1 && (bunsi >= (NEXT_BUNSI1 + BUNSI_HABA * flg)))
		{
			if(param - DOWN_BUNSI_PARAM < 1)
			{
				param = 1;
			}
			else
			{
				param -= DOWN_BUNSI_PARAM;
				flg++;
			}
		}
	}
	
	flg &= 0xf;
	bunsi &= 0xff;
	param &= 0xf;
	
	if(bunsi + param > DEFAULT_BUNBO)
	{
		bunsi = DEFAULT_BUNBO;
	}
	else
	{
		bunsi += param;
	}
	
	*HP_Work = ((param << 12) | (flg << 8) | bunsi);
}



//==================================================================
//			�Q�[�W�v�Z��\��(BG�p)
//
//    �� HP_Work:ܰ��Ƃ��Ďg�p ��0����ܰ��̏��������s���� �����0�����Ă�������
//
// ���� : MaxHP:�ő�HP
//		  NowHP:����HP(��Ұ�ނ��󂯂�O)
//		  beHP:�󂯂���Ұ��(�񕜂���ꍇ��ϲŽ�œ����)
//        *HP_Work : ܰ��̱��ڽ(�ް�ނ̓r���o�߂��ް�������)
//								���0�`3�ޯ� �������A 4�`7 �׸ށA 8�`15 ���q �Ŏg�p����
//		  *TransPos:�ް�ނ�\�������齸ذ��ް��̍��W
//		  TopChr:�ް�ނ̷�ׂ�į�߱��ڽ
//		  *pl_buf:��گ��ޯ̧
// �߂�l : -1:�I��		0�ȏ�̐��l:�ް�ނƈꏏ�ɕ\�������鐔�l�̒l
//==================================================================
//s16 GaugeBG(s16 MaxHP, s16 NowHP, s16 beHP, u16 *HP_Work, 
//										u16 *TransPos, u16 TopChr, u16 *pl_buf)
s16 GaugeBG(GaugeDataBG *gdata, u16 *HP_Work, u16 *TransPos, u16 *pl_buf)
{
	s16 AfterHP, resultHP;
	u8 temp;

	
	if(((*HP_Work) & 0x00ff) == DEFAULT_BUNBO)	//�I������
	{	//�������������Ă���̂͑S�Ă̐�����߂�l�Ƃ��ĕԂ���1�T�����Ă���
		return -1;	//�S�Ă̹ް�ނ��I�����I�����
	}
	
	
	if(gdata->beHP < 0)	//��	�ް�ޑ��₷
	{
		if(gdata->NowHP + (-gdata->beHP) > gdata->MaxHP)
						// -gdata->beHP = -(-gdata->beHP) = ���̐��l��gdata->beHP
		{
			gdata->beHP = -(gdata->MaxHP - gdata->NowHP);
		}
		AfterHP = gdata->NowHP + (-gdata->beHP);
		
//fff		GaugeProcSub(HP_Work);				//���������ٰ�߂ŕ\������ް��
		resultHP = gdata->NowHP + (-gdata->beHP) * (*HP_Work&0x00ff) / DEFAULT_BUNBO;
		if(resultHP > gdata->MaxHP)
		{
			resultHP = gdata->MaxHP;
		}
		
		PutGaugeBG(gdata, AfterHP, resultHP, TransPos, ((*HP_Work) & 0x00ff));
	}
	else	//��Ұ��  �ް�ތ��炷
	{
		if(gdata->NowHP - gdata->beHP < 0)
		{
			gdata->beHP = gdata->NowHP;
		}
		AfterHP = gdata->NowHP - gdata->beHP;	//��Ұ�ތ��HP
				
//fff		GaugeProcSub(HP_Work);				//���������ٰ�߂ŕ\������ް��
		resultHP = gdata->NowHP - gdata->beHP * (*HP_Work&0x00ff) / DEFAULT_BUNBO;
		if(resultHP < AfterHP)
		{
			resultHP = AfterHP;
		}
		
		temp = DEFAULT_BUNBO - ((*HP_Work) & 0x00ff);
		PutGaugeBG(gdata, AfterHP, resultHP, TransPos, temp);
	}
	
//	temp = DEFAULT_BUNBO - ((*HP_Work) & 0x00ff);
//	PutGaugeBG(gdata->MaxHP, AfterHP, resultHP, TransPos, GAUGE_TOPCHR, gdata->NowHP, temp);
	GaugeColChk(gdata->MaxHP, resultHP, pl_buf);
	
	if(gdata->beHP == 0)
	{	//beHP��0�Ȃ�΍��̹ް�ނ�\�����邾���Ȃ̂ō����ٰ�߂ŏI���ɂ���
		*HP_Work = ((*HP_Work) & 0xff00) | DEFAULT_BUNBO;
	}
	return resultHP;
}


//==================================================================
//		�Q�[�W�\��(��ذ��ް���ύX����)
//
// ���� : GAUGE_TOPCHR:�ް�ނ̍ŏ��̷�ׂ��o�^����Ă��鷬����ް
// �߂�l : �Ȃ�
//==================================================================
void PutGaugeBG(GaugeDataBG *gdata, s16 AfterHP, s16 resultHP, u16 *TransPos, u8 Bunsi)
{
	u8 gauge_count;
	u8 i;
	u8 gauge_power, HP_power, Now_power, temp;
//	u8 put_work;
	u16 DummyScrn[GAUGE_MAX];
	vu8 dma_wait;
	
	gauge_count = 0;
	
	for(i = 0; i < GAUGE_MAX; i++)	//�ް�ޕ�������x�󔒹ް�ނŏ�����
	{
//		TransPos[i] = (gdata->color << 12) | gdata->CharNo;
		DummyScrn[i] = (gdata->color << 12) | gdata->CharNo;
	}

//	TransPos[0] &= 0xfc00;			//�ް�ނ̒[�̊G�̕���
//	TransPos[0] |= GAUGE_TOPCHR;
//	TransPos[GAUGE_MAX+1] &= 0xfc00;
//	TransPos[GAUGE_MAX+1] |= GAUGE_TOPCHR + 10;	//���� <= �Ȃ̂�
	
	if(resultHP == 0)
	{
		DIV_DMACOPY(3, DummyScrn, TransPos, GAUGE_MAX*2, 16);
		dma_wait = 0;	dma_wait = 0;
		return;
	}
	
//	HP_power = 100 * (resultHP) / gdata->MaxHP;	//resultHP�����L����HP�̊���
	if(gdata->NowHP > AfterHP)
	{
		Now_power = 100 * AfterHP / gdata->MaxHP;	//�ް�ނ������Ȃ����������L����HP�̊���
		HP_power = 100 * gdata->NowHP / gdata->MaxHP;
		HP_power -= Now_power;				//�ް�ނ��ϓ����镔�������L����HP�̊���
	}
	else
	{
		Now_power = 100 * gdata->NowHP / gdata->MaxHP;	//�ް�ނ������Ȃ����������L����HP�̊���
		HP_power = 100 * AfterHP / gdata->MaxHP;
		HP_power -= Now_power;				//�ް�ނ��ϓ����镔�������L����HP�̊���
	}
	temp = 100 * Bunsi / DEFAULT_BUNBO;	//DEFAULT_BUNBO����Bunsi��100%����Bunsi�ɕς���
	HP_power = HP_power * temp / 100;	//Bunsi����ް�ނ��\������镔���܂ł�%�����߂�
	HP_power += Now_power;	//�\�������߰��ð��

	
	for(gauge_count = 1; gauge_count <= GAUGE_MAX; gauge_count++)
	{
		gauge_power = 100 * gauge_count / GAUGE_MAX;	//�ް�޶��Ă܂ł����L����HP�̊�����
		
		if(HP_power < gauge_power)
		{
			//gauge_count��א��܂łɗ]���Ă��܂����������߂�(1��ׂ܂�܂�𖄂߂��Ȃ��[��)
			HP_power -= 100 * (gauge_count - 1) / GAUGE_MAX;	//��%�]���Ă�̂�
			HP_power = GAUGE_PARSENT * HP_power / 100;
			
			if(gauge_count == 1 && HP_power == 0 && AfterHP > 0)
			{	//�߰��ð�ނł�0�ł��A���ۂ�HP��1�ȏ゠��Ȃ�1�ޯĂ̹ް�ނ�\������
				//TransPos[0] &= 0xfc00;
				//TransPos[0] |= gdata->CharNo + 1;
				DummyScrn[0] &= 0xfc00;
				DummyScrn[0] |= gdata->CharNo + 1;
			}
			else
			{	//�ʏ�
				//TransPos[gauge_count-1] &= 0xfc00;//-1=gauge_count��1����n�܂��Ă��邽��
				//TransPos[gauge_count-1] |= (gdata->CharNo + HP_power);	//put_work);
				DummyScrn[gauge_count-1] &= 0xfc00;//-1=gauge_count��1����n�܂��Ă��邽��
				DummyScrn[gauge_count-1] |= (gdata->CharNo + HP_power);	//put_work);
			}
			break;
		}
		else
		{
			//TransPos[gauge_count-1] &= 0xfc00;
			//TransPos[gauge_count-1] |= gdata->CharNo + 8;
			DummyScrn[gauge_count-1] &= 0xfc00;
			DummyScrn[gauge_count-1] |= gdata->CharNo + 8;
		}
	}
	DIV_DMACOPY(3, DummyScrn, TransPos, GAUGE_MAX*2, 16);
	dma_wait = 0;	dma_wait = 0;
}

*/

#endif


//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================



//----------------------------------------------------------
//	����HP�ƍő�HP�A�ް�ނ̍ő��ޯĐ���n���āA����HP���Ɖ��ޯĂȂ̂���Ԃ�
//----------------------------------------------------------
u8 GetNumDotto(s16 NowHP, s16 MaxHP, u8 GaugeMaxDot)
{
	u8 put_dot;
	
	put_dot = NowHP * GaugeMaxDot / MaxHP;
	if(put_dot == 0 && NowHP > 0)	//�ޯČv�Z�ł�0�ł����ۂ�HP��1�ȏ�Ȃ�1�ޯĂɂ���
		put_dot = 1;
	return put_dot;
}

//----------------------------------------------------------
//	HP�ް�ނ̶װ���擾����
//	�߂�l: 0=�ޯĂȂ�	1=��	2=��	3=��	4=HP�����^��
//----------------------------------------------------------
u8 GetGaugeDottoColor(s16 NowHP, s16 MaxHP)
{
	u8 put_dot;
	u8 ret;
	
	if(NowHP == MaxHP)
		return 4;
	
	put_dot = GetNumDotto(NowHP, MaxHP, GAUGE_DOTTOMAX);
	if(put_dot > GAUGE_DOTTOMAX / 2)
		ret = 3;	//��
	else if(put_dot > 9)
		ret = 2;	//��
	else if(put_dot > 0)
		ret = 1;	//��
	else
		ret = 0;	//HP��0�̎�
	
	return ret;
}

