//=========================================================================
//	
//	���̑��n �Z�G�t�F�N�g	���c�[���Ƃ��ėp�I�ȓ��������Ƃ��E�E�E���ȁH
//
//=========================================================================
#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "madefine.h"
#include "wazatool.h"
#include "waza_dat.h"
#include "waza_eff.h"
#include "server.h"
#include "calctool.h"

#include "bss.h"
#include "task.h"
#include "palanm.h"
#include "client.h"
#include "fight.h"
#include "ef_tool.h"
#include "poketool.h"
#include "decord.h"
#include "cli_def.h"
#include "contest.h"
#include "mus_tool.h"




//=========================================================================
//
//		�u�Ђ悱�v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void SeqWazaHiyokoInit(actWork *Xreg);
static void SeqWazaHiyokoMain(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define HIYOKO_FURIHABA_DX	30
#define HIYOKO_FURIHABA_DY	10
#define HIYOKO_ANM			8

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
//----------------------------------------------------------
//	�s����
//----------------------------------------------------------
static const actAnm HiyokoAnm1[]={
	{OBJ16_16x16 * 0, HIYOKO_ANM, 0, 0},
	{OBJ16_16x16 * 1, HIYOKO_ANM, 0, 0},
	{OBJ16_16x16 * 0, HIYOKO_ANM, 1, 0},
	{OBJ16_16x16 * 2, HIYOKO_ANM, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm HiyokoAnm2[]={
	{OBJ16_16x16 * 0, HIYOKO_ANM, 1, 0},
	{OBJ16_16x16 * 1, HIYOKO_ANM, 0, 0},
	{OBJ16_16x16 * 0, HIYOKO_ANM, 0, 0},
	{OBJ16_16x16 * 2, HIYOKO_ANM, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},};

static const actAnm *const HiyokoAnmTbl[]={
	HiyokoAnm1,
	HiyokoAnm2,
};

const	actHeader ActHiyokoHeader={
	WAZACHR_ID_HIYOKO,
	WAZACHR_ID_HIYOKO,
	&WazaOamDataNormal[1],
	HiyokoAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaHiyokoInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------
//=========================================================================
//	�ֺ
//=========================================================================
//-------------------------------------------------------------
// offset 0:Xoffset 1:Yoffset 2:�boffset 3:��]��߰�� 4:ٰ�߉�
//-------------------------------------------------------------
static void SeqWazaHiyokoInit(actWork *Xreg)
{
	Xreg->x += WazaEffWork[0];
	Xreg->y += WazaEffWork[1];
	Xreg->work[0] = WazaEffWork[2];	//�b
	if(MineEnemyCheck(AttackNo))
	{
		Xreg->work[1] = -WazaEffWork[3];
		Xreg->work[4] = 1;
	}
	else
	{
		Xreg->work[1] = WazaEffWork[3];
		Xreg->work[4] = 0;
		ActAnmChg(Xreg, 1);
	}
	Xreg->work[3] = WazaEffWork[4];
	Xreg->move=SeqWazaHiyokoMain;
	SeqWazaHiyokoMain(Xreg);
}

static void SeqWazaHiyokoMain(actWork *Xreg)
{
	Xreg->dx = CosMove(Xreg->work[0], HIYOKO_FURIHABA_DX);
	Xreg->dy = SinMove(Xreg->work[0], HIYOKO_FURIHABA_DY);
	
	if(Xreg->work[0] >= 0 && Xreg->work[0] < 128)
		Xreg->oamData.Priority = 1;
	else
		Xreg->oamData.Priority = 3;
	Xreg->work[0] = 0xff & (Xreg->work[0] + Xreg->work[1]);
	
	Xreg->work[2]++;
	if(Xreg->work[2] == Xreg->work[3])
		DelWazaEffect(Xreg);
}





//=========================================================================
//
//		�u�J���[�����Z�n�v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void WazaSeqColorWait(actWork *Xreg);
static void WazaSeqColorInit(actWork *Xreg);
static u32 WazaSeqColorInitSub(s16);
static void WazaSeqFlashWait(actWork *Xreg);
static void WazaSeqFlashEnd(actWork *Xreg);
static void WazaSeqColorFlashInit(actWork *Xreg);

static void SeqWazaKiraFlashInit(actWork *Xreg);

void WazaTaskColorFade(u8 taskno);
static void WazaTaskColorFadeWait(u8 taskno);
static void WazaTaskColorFadeMain(u8 taskno, u8 start_evy, u8 end_evy);

static void WazaTaskFlashEnd(u8 id);
static void WazaTaskFlashWait(u8 id);

static void WazaChrFadeRepeatMain(u8 taskno, u8 start_evy, u8 end_evy);
static void WazaChrColorFadeRepeatWait(u8 taskno);


//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define FLASH_KIRA_WAIT		3

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
const	actHeader ActFadeHeader={	//̪���
	0,
	0,
	&DummyActOam,
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	WazaSeqColorInit,
};
const	actHeader ActFlashHeader={	//�ׯ��
	0,
	0,
	&DummyActOam,
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	WazaSeqColorFlashInit,
};


static const actAnm FlashKiraAnm[]={
	{OBJ16_32x32 * 0, FLASH_KIRA_WAIT, 0, 0},
	{OBJ16_32x32 * 1, FLASH_KIRA_WAIT, 0, 0},
	{OBJ16_32x32 * 2, FLASH_KIRA_WAIT, 0, 0},
	{OBJ16_32x32 * 3, FLASH_KIRA_WAIT, 0, 0},
	{OBJ16_32x32 * 4, FLASH_KIRA_WAIT, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm *const FlashKiraAnmTbl[]={
		FlashKiraAnm,
};
const	actHeader ActKiraFlashHeader={
	WAZACHR_ID_L_HIKARI,
	WAZACHR_ID_L_HIKARI,
	&WazaOamDataNormal[2],
	FlashKiraAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaKiraFlashInit,
};


//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------
//----------------------------------------------------------
//	̪���ظ���
// offset 0:̪��ޑΏ� 0bit=�w�i(�y�n�܂�) 1bit=�U�����߹�� 2bit=�h�䑤�߹��
//					  3bit=�U�����߹�ݑ���  4bit=�h�䑤�߹�ݑ���
//					  5bit=�̪��BG1  6bit=�̪��BG2
// 1..:���PaletteFadeReq��fade_bit�ȊO�̈��������X�Ɏw��(wait�ȍ~)
//	u8 wait, u8 start_evy, u8 end_evy, u16 next_rgb
//----------------------------------------------------------
static void WazaSeqColorInit(actWork *Xreg)
{
	u32 fade_bit = 0;
	
	fade_bit = WazaSeqColorInitSub(WazaEffWork[0]);
	PaletteFadeReq(fade_bit, WazaEffWork[1],
				WazaEffWork[2], WazaEffWork[3], WazaEffWork[4]);

	Xreg->banish = 1;
	Xreg->move = WazaSeqColorWait;
}

static u32 WazaSeqColorInitSub(s16 fade_flg)
{
	u32 fade_bit = 0;

	u8 haikei,atpoke,depoke,atpoke2,depoke2,eff1,eff2;
	
	haikei = fade_flg&1;
	atpoke = (fade_flg>>1)&1;
	depoke = (fade_flg>>2)&1;
	atpoke2 = (fade_flg>>3)&1;
	depoke2 = (fade_flg>>4)&1;
	eff1 = (fade_flg>>5)&1;
	eff2 = (fade_flg>>6)&1;
	fade_bit = GetBattleFadeBit(haikei,atpoke,depoke,atpoke2,depoke2,eff1,eff2);

	return fade_bit;
}

//----------------------------------------------------------
//	̪��ނ��I������܂�WAIT
//----------------------------------------------------------
static void WazaSeqColorWait(actWork *Xreg)
{
	if(FadeData.fade_sw == 0)
		DelWazaEffect(Xreg);
}

//=========================================================================
//	
//=========================================================================
//----------------------------------------------------------
//	�ׯ��ظ���(2�F�̶װ�ƔZ�x���w�肵�āA����2�F������������)
// offset 0:̪��ޑΏ� 0bit=�w�i 1bit=�U�����߹�� 2bit=�U���Ώ��߹��
//					  3bit=�U�����߹�ݑ���  4bit=�h�䑤�߹�ݑ���
//					  5bit=�̪��BG1  6bit=�̪��BG2
// 1:wait(�ׯ������Ԋu ��MAX255)  2:�J��Ԃ���
// 3:color1 4:color1��evy 5:color2 6:color2��evy
//----------------------------------------------------------
static void WazaSeqColorFlashInit(actWork *Xreg)
{
	u32 fade_bit = 0;
	
	Xreg->work[0] = WazaEffWork[1];
	Xreg->work[1] = WazaEffWork[1];		//BackUp  ���8�ޯĂ��׸ނŎg�p
	Xreg->work[2] = WazaEffWork[2];
	Xreg->work[3] = WazaEffWork[3];
	Xreg->work[4] = WazaEffWork[4];
	Xreg->work[5] = WazaEffWork[5];
	Xreg->work[6] = WazaEffWork[6];
	Xreg->work[7] = WazaEffWork[0];
	
	fade_bit = WazaSeqColorInitSub(Xreg->work[7]);
//	PaletteFadeReq(fade_bit, 0,	Xreg->work[4], Xreg->work[4], Xreg->work[3]);
	ColorConceChange(fade_bit, WazaEffWork[4], WazaEffWork[3]);

	Xreg->banish = 1;
	Xreg->move = WazaSeqFlashWait;
}

//----------------------------------------------------------
//	ٰ�ߏ���
//----------------------------------------------------------
static void WazaSeqFlashWait(actWork *Xreg)
{
	u32 fade_bit = 0;
	
	if(Xreg->work[0] > 0)
		Xreg->work[0]--;
	else if(FadeData.fade_sw)
		return;
	else
	{
		if(Xreg->work[2] == 0)
		{
			Xreg->move = WazaSeqFlashEnd;
			return;
		}
		
		fade_bit = WazaSeqColorInitSub(Xreg->work[7]);
		if(Xreg->work[1] & 0x100)
//			PaletteFadeReq(fade_bit, 0,	Xreg->work[4], Xreg->work[4], Xreg->work[3]);
			ColorConceChange(fade_bit, Xreg->work[4], Xreg->work[3]);
		else
//			PaletteFadeReq(fade_bit, 0,	Xreg->work[6], Xreg->work[6], Xreg->work[5]);
			ColorConceChange(fade_bit, Xreg->work[6], Xreg->work[5]);
	
		Xreg->work[1] ^= 0x100;
		Xreg->work[0] = Xreg->work[1] & 0xff;
		Xreg->work[2]--;
	}
}

//----------------------------------------------------------
//	�F�����ɖ߂��ďI��
//----------------------------------------------------------
static void WazaSeqFlashEnd(actWork *Xreg)
{
	u32 fade_bit = 0;
	
	if(FadeData.fade_sw)
		return;
	fade_bit = WazaSeqColorInitSub(Xreg->work[7]);
//	PaletteFadeReq(fade_bit, 0, 0, 0, 0);
	ColorConceChange(fade_bit, 0, 0);
	DelWazaEffect(Xreg);
}




//=========================================================================
//	�ׯ�����ɏo�鐯
// offset 0:Xoffset 1:Yoffset
//=========================================================================
static void SeqWazaKiraFlashInit(actWork *Xreg)
{
	Xreg->x += WazaEffWork[0];
	Xreg->y += WazaEffWork[1];
	
	Xreg->work[0] = 0;
	Xreg->work[1] = 10;
	Xreg->work[2] = 8;			//�p�x������
	Xreg->work[3] = 40;			//ٰ�߉�
	Xreg->work[4] = 0x0070;
	Xreg->work[5] = 0;
	SetWorkSeq(Xreg, W_AffCheckEndSeq);
	Xreg->move = PMObjKaiten2;
	PMObjKaiten2(Xreg);	//���̂܂܂���1�u�����Ă��܂��̂ŁA�����ŏ���̺��
}






//=========================================================================
//	�^�X�N�V�[�P���X
//=========================================================================
//----------------------------------------------------------
//̪���ظ��ČJ��Ԃ�(�װ��2�̔Z�x���w�肵�āA����2�̔Z�x�̊Ԃ�̪��ނ��J��Ԃ�)
//  ���ŏ���̪��ނ�start_evy��������0�ɂȂ�܂��B
//    �Ō��̪��ނ�end_evy��������0�ɂȂ�܂��B
// offset 0:̪��ޑΏ� 0bit=�w�i 1bit=�U�����߹�� 2bit=�U���Ώ��߹��
//					  3bit=�U�����߹�ݑ���  4bit=�h�䑤�߹�ݑ���
//					  5bit=�̪��BG1  6bit=�̪��BG2
// 1:wait
// 2:�J��Ԃ���
// 3:start_evy
// 4:end_evy
// 5:color1(RGB�w��)
//----------------------------------------------------------
void WazaTaskColorFade(u8 taskno)
{
	TaskTable[taskno].work[0] = WazaEffWork[0];
	TaskTable[taskno].work[1] = WazaEffWork[1];
	TaskTable[taskno].work[2] = WazaEffWork[2];
	TaskTable[taskno].work[3] = WazaEffWork[3];
	TaskTable[taskno].work[4] = WazaEffWork[4];
	TaskTable[taskno].work[5] = WazaEffWork[5];

	TaskTable[taskno].work[8] = 0;	//�׸ނŎg�p
	WazaTaskColorFadeMain(taskno, 0, TaskTable[taskno].work[4]);//�ŏ���start_evy��0
	TaskTable[taskno].TaskAdrs = WazaTaskColorFadeWait;
}

static void WazaTaskColorFadeMain(u8 taskno, u8 start_evy, u8 end_evy)
{
	u32 fade_bit = 0;

	fade_bit = WazaSeqColorInitSub(TaskTable[taskno].work[0]);
	PaletteFadeReq(fade_bit, TaskTable[taskno].work[1],	
				start_evy, end_evy, TaskTable[taskno].work[5]);
	
	TaskTable[taskno].work[2]--;
	TaskTable[taskno].work[8] ^= 1;
}

static void WazaTaskColorFadeWait(u8 taskno)
{
	u8 start_evy, end_evy;
	
	if(FadeData.fade_sw == 0)
	{
		if(TaskTable[taskno].work[2] > 0)
		{
			if(TaskTable[taskno].work[8] == 0)
			{
				start_evy = TaskTable[taskno].work[3];
				end_evy = TaskTable[taskno].work[4];
			}
			else
			{
				start_evy = TaskTable[taskno].work[4];
				end_evy = TaskTable[taskno].work[3];
			}
			if(TaskTable[taskno].work[2] == 1)
				end_evy = 0;	//���̐F�ɖ߂��ďI�����邽��
			WazaTaskColorFadeMain(taskno, start_evy, end_evy);
		}
		else
			DelTaskEffect(taskno);
	}
}








//=========================================================================
//	�^�X�N�V�[�P���X
//=========================================================================
//----------------------------------------------------------
//̪���ظ��ČJ��Ԃ�(�װ��2�̔Z�x���w�肵�āA����2�̔Z�x�̊Ԃ�̪��ނ��J��Ԃ�)
// WAZACHR_ID���w�肵�āA������گĂɑ΂��ăt�F�[�h���s��
// 0:WAZACHR_ID
// 1:wait
// 2:�J��Ԃ���
// 3:start_evy
// 4:end_evy
// 5:color1(RGB�w��)
//----------------------------------------------------------
void WazaChrColorFadeRepeat(u8 taskno)
{
	TaskTable[taskno].work[0] = WazaEffWork[0];
	TaskTable[taskno].work[1] = WazaEffWork[1];
	TaskTable[taskno].work[2] = WazaEffWork[2];
	TaskTable[taskno].work[3] = WazaEffWork[3];
	TaskTable[taskno].work[4] = WazaEffWork[4];
	TaskTable[taskno].work[5] = WazaEffWork[5];

	TaskTable[taskno].work[8] = 0;	//�׸ނŎg�p
	WazaChrFadeRepeatMain(taskno, 0, TaskTable[taskno].work[4]);//�ŏ���start_evy��0
	TaskTable[taskno].TaskAdrs = WazaChrColorFadeRepeatWait;
}

static void WazaChrFadeRepeatMain(u8 taskno, u8 start_evy, u8 end_evy)
{
	u32 fade_bit = 0;
	u8 pal_no;
	
	pal_no = ObjPalNumGet(TaskTable[taskno].work[0]);
	fade_bit = 1 << (16 + pal_no);
	PaletteFadeReq(fade_bit, TaskTable[taskno].work[1],	
				start_evy, end_evy, TaskTable[taskno].work[5]);
	
	TaskTable[taskno].work[2]--;
	TaskTable[taskno].work[8] ^= 1;
}

static void WazaChrColorFadeRepeatWait(u8 taskno)
{
	u8 start_evy, end_evy;
	
	if(FadeData.fade_sw == 0)
	{
		if(TaskTable[taskno].work[2] > 0)
		{
			if(TaskTable[taskno].work[8] == 0)
			{
				start_evy = TaskTable[taskno].work[3];
				end_evy = TaskTable[taskno].work[4];
			}
			else
			{
				start_evy = TaskTable[taskno].work[4];
				end_evy = TaskTable[taskno].work[3];
			}
			if(TaskTable[taskno].work[2] == 1)
				end_evy = 0;	//���̐F�ɖ߂��ďI�����邽��
			WazaChrFadeRepeatMain(taskno, start_evy, end_evy);
		}
		else
			DelTaskEffect(taskno);
	}
}






//=========================================================================
//	�^�X�N
//=========================================================================
//----------------------------------------------------------
//	�ׯ��ظ���2(2�F�̶װ�ƔZ�x���w�肵�āA����2�F������������)
// WAZACHR_ID���w�肵�āA������گĂɑ΂��ăt���b�V����������
// offset 0:WAZACHR_ID
// 1:wait(�ׯ������Ԋu ��MAX255)  2:�J��Ԃ���
// 3:color1 4:color1��evy 5:color2 6:color2��evy
//----------------------------------------------------------
void WazaTaskWazaChrFlash(u8 id)
{
	u8 pal_no;
	u32 fade_bit = 0;
	
	TaskTable[id].work[0] = WazaEffWork[1];
	TaskTable[id].work[1] = WazaEffWork[1];		//BackUp  ���8�ޯĂ��׸ނŎg�p
	TaskTable[id].work[2] = WazaEffWork[2];
	TaskTable[id].work[3] = WazaEffWork[3];
	TaskTable[id].work[4] = WazaEffWork[4];
	TaskTable[id].work[5] = WazaEffWork[5];
	TaskTable[id].work[6] = WazaEffWork[6];
	TaskTable[id].work[7] = WazaEffWork[0];
	
	pal_no = ObjPalNumGet(WazaEffWork[0]);
	fade_bit = 1 << (16 + pal_no);
	PaletteFadeReq(fade_bit, 0,	WazaEffWork[4], WazaEffWork[4], WazaEffWork[3]);
	TaskTable[id].TaskAdrs = WazaTaskFlashWait;
}

//----------------------------------------------------------
//	ٰ�ߏ���
//----------------------------------------------------------
static void WazaTaskFlashWait(u8 id)
{
	u8 pal_no;
	u32 fade_bit = 0;
	
	if(TaskTable[id].work[0] > 0)
		TaskTable[id].work[0]--;
	else if(FadeData.fade_sw)
		return;
	else
	{
		if(TaskTable[id].work[2] == 0)
		{
			TaskTable[id].TaskAdrs = WazaTaskFlashEnd;
			return;
		}
		
		pal_no = ObjPalNumGet(TaskTable[id].work[7]);
		fade_bit = 1 << (16 + pal_no);
		if(TaskTable[id].work[1] & 0x100)
			PaletteFadeReq(fade_bit, 0,	TaskTable[id].work[4],
					 TaskTable[id].work[4], TaskTable[id].work[3]);
		else
			PaletteFadeReq(fade_bit, 0,	TaskTable[id].work[6],
					 TaskTable[id].work[6], TaskTable[id].work[5]);
	
		TaskTable[id].work[1] ^= 0x100;
		TaskTable[id].work[0] = TaskTable[id].work[1] & 0xff;
		TaskTable[id].work[2]--;
	}
}

//----------------------------------------------------------
//	�F�����ɖ߂��ďI��
//----------------------------------------------------------
static void WazaTaskFlashEnd(u8 id)
{
	u8 pal_no;
	u32 fade_bit = 0;
	
	if(FadeData.fade_sw)
		return;
	pal_no = ObjPalNumGet(TaskTable[id].work[7]);
	fade_bit = 1 << (16 + pal_no);
	PaletteFadeReq(fade_bit, 0, 0, 0, 0);
	DelTaskEffect(id);
}







//=========================================================================
//	�װ���]
//=========================================================================
//----------------------------------------------------------
//    1 = ���]   0 = ��Ώ�
// offset 0:���8�ޯ�=�w�i		   ����8�ޯ�=�ް��
//		  1:���8�ޯ�=�U�����߹��1 ����8�ޯ�=�U�����߹��2
//		  2:���8�ޯ�=�h�䑤�߹��1 ����8�ޯ�=�h�䑤�߹��2
//		�@3:���8�ޯ�=�U�����ް��1 ����8�ޯ�=�U�����ް��2
//		�@4:���8�ޯ�=�h�䑤�ް��1 ����8�ޯ�=�h�䑤�ް��2
//----------------------------------------------------------
void WazaTaskColorReversal(u8 id)
{
	u32 palbit = 0;
	u8 a_client, d_client;	//attack, defence
//	u8 temp;
	
	a_client = AttackNo;
	d_client = DefenceNo;
	
	if(WazaEffWork[0] & 0x0100)
		palbit |= GetBattleFadeBit(1,0,0,0,0,0,0);

	if(WazaEffWork[1] & 0x0100)
		palbit |= (1 << a_client) << 16;
	
	//if(WazaEffWork[1] & 1)	//�U�����߹��2
	
	if(WazaEffWork[2] & 0x0100)
		palbit |= (1 << d_client) << 16;
	
	//if(WazaEffWork[2] & 1)	//�h�䑤�߹��2
	
	ColorReversal(palbit);
	DelTaskEffect(id);
}

//=========================================================================
//	�װ���Z		31(5bit)�ȉ���؂�̂Ă��܂܉��Z�������܂�
//=========================================================================
//----------------------------------------------------------
//    1 = ���Z   0 = ��Ώ�
// offset 0:���8�ޯ�=�w�i		   ����8�ޯ�=�ް��
//		  1:���8�ޯ�=�U�����߹��1 ����8�ޯ�=�U�����߹��2
//		  2:���8�ޯ�=�h�䑤�߹��1 ����8�ޯ�=�h�䑤�߹��2
//		�@3:���8�ޯ�=�U�����ް��1 ����8�ޯ�=�U�����ް��2
//		�@4:���8�ޯ�=�h�䑤�ް��1 ����8�ޯ�=�h�䑤�ް��2
//		  5:��گ� RED   ���Z���l
//		  5:��گ� GREEN ���Z���l
//		  5:��گ� BLUE  ���Z���l
//----------------------------------------------------------
void WazaTaskColorRandomPlus(u8 id)
{
	u32 palbit = 0;
	u8 a_client, d_client;	//attack, defence
	u8 temp;
	
	if(TaskTable[id].work[0]==0){
		TaskTable[id].work[2] = WazaEffWork[0];	//�����Z�Ώ�bit�w�� BG
		TaskTable[id].work[3] = WazaEffWork[1];	//�����Z�Ώ�bit�w�� MYOBJ
		TaskTable[id].work[4] = WazaEffWork[2];	//�����Z�Ώ�bit�w�� ENEMYOBJ
		TaskTable[id].work[1] = WazaEffWork[3]; //�E�F�C�g��
		TaskTable[id].work[5] = WazaEffWork[4];	//RED���Z
		TaskTable[id].work[6] = WazaEffWork[5];	//GREEN���Z
		TaskTable[id].work[7] = WazaEffWork[6];	//BLUE���Z
	}
	TaskTable[id].work[0]++;
	a_client = AttackNo;
	d_client = DefenceNo;
	
	if(TaskTable[id].work[2] & 0x0100)
		palbit |= 0xffff;
	
	if(TaskTable[id].work[2] & 1)
	{
		temp = ObjPalNumGet(ActWork[ClientGaugeSt[a_client]].actHead->palNo);
		palbit |= (1 << temp) << 16;
	}

	if(TaskTable[id].work[3] & 0x0100)
		palbit |= (1 << a_client) << 16;
	
	
	if(TaskTable[id].work[4] & 0x0100)
		palbit |= (1 << d_client) << 16;
	

	ColorRandomPlus(palbit,TaskTable[id].work[5],TaskTable[id].work[6],TaskTable[id].work[7]);

	if(TaskTable[id].work[0]==TaskTable[id].work[1]){
		ColorRandomPlusEnd(palbit);
		DelTaskEffect(id);
	}
}











//=========================================================================
//
//		�u��ʗh�炵�G�t�F�N�g�v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void DispQuakeInit( actWork * Xreg );
static void DispQuakeMain(actWork *Xreg);
static void PokeQuakeInit(void);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
enum{
	DISP_H,
	DISP_V,
	POKE_H,
	POKE_V,
};

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
const	actHeader ActQuakeHeader={	//�h�炵
	0,
	0,
	&DummyActOam,
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	DispQuakeInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------
//----------------------------------------------------------
//	offset 0:�U�蕝 1:wait(��ʂ�h�炷�Ԋu) 2:�I���܂ł�wait
//         3:�Ώ� 0=�w�i���h�炵 1=�w�i�c�h�炵 2=�߹�݉� 3=�߹�ݏc
//		   4:�Ώ�2(�Ώۂ��߹�݂̎������K�v) 0=�U���� 1=�U���Ώ� 2=����
//----------------------------------------------------------
static void DispQuakeInit( actWork * Xreg )
{
	Xreg->banish = 1;
	Xreg->work[0] = -WazaEffWork[0];
	Xreg->work[1] = WazaEffWork[1];
	Xreg->work[2] = WazaEffWork[1];		//BackUp
	Xreg->work[3] = WazaEffWork[2];

	switch(WazaEffWork[3])
	{
		case DISP_H:		//��ʉ��h�炵
			SetWorkSeq(Xreg, &ScrX3);
			break;
		case DISP_V:		//��ʏc�h�炵
			SetWorkSeq(Xreg, &ScrY3);
			break;
		case POKE_H:		//�߹�݉��h�炵
			SetWorkSeq(Xreg, &ActOX);
			break;
		case POKE_V:		//�߹�ݏc�h�炵
		default:
			SetWorkSeq(Xreg, &ActOY);
			break;
	}
	Xreg->work[4] = *(u32*)((void*)(Xreg->work[6] | (Xreg->work[7] << 16)));//BackUp
	Xreg->work[5] = WazaEffWork[3];
	
	if(WazaEffWork[3] == POKE_H || WazaEffWork[3] == POKE_V)
		PokeQuakeInit();
	
	Xreg->move = DispQuakeMain;
//	DispQuakeMain(Xreg);	//�Ă΂ꂽ�̂Ɠ����ɂ��ׁA����̂������ŌĂ�
}

//----------------------------------------------------------
//	���C��
//----------------------------------------------------------
static void DispQuakeMain(actWork *Xreg)
{
	u8 i;
	
	if(Xreg->work[3] > 0)
	{
		Xreg->work[3]--;

		if(Xreg->work[1] > 0)
			Xreg->work[1]--;
		else
		{
			Xreg->work[1] = Xreg->work[2];
			*(u32*)((void*)(Xreg->work[6] | (Xreg->work[7] << 16))) += Xreg->work[0];
			Xreg->work[0] = -Xreg->work[0];
		}
	}
	else
	{
		*(u32*)((void*)(Xreg->work[6] | (Xreg->work[7] << 16))) = Xreg->work[4];
		if(Xreg->work[5] == POKE_H || Xreg->work[5] == POKE_V)
		{
			for(i = 0; i < ClientSetMax; i++)
				ActWork[PokemonStruct[i]].scr = 0;
		}
		DelWazaEffect(Xreg);
	}
}

//----------------------------------------------------------
//	�h�炵�Ώۂ��߹�݂̎������̏�����
//----------------------------------------------------------
static void PokeQuakeInit(void)
{
	ActWork[PokemonStruct[AttackNo]].scr = 0;
	ActWork[PokemonStruct[DefenceNo]].scr = 0;
	
	if(WazaEffWork[4] == 2)
	{
		ActWork[PokemonStruct[AttackNo]].scr = 1;
		ActWork[PokemonStruct[DefenceNo]].scr = 1;
		return;
	}
	
	if(WazaEffWork[4] == 0)
		ActWork[PokemonStruct[AttackNo]].scr = 1;
	else
		ActWork[PokemonStruct[DefenceNo]].scr = 1;
}





//=========================================================================
//
//		�w�i�h�炵�G�t�F�N�g
//												by matsuda 2002.07.17(��)
//=========================================================================
static void TaskHaikeiQuakeMain(u8 id);

//-------------------------------------------------------------
// offset 0:X�U�蕝(���a)  1:Y�U�蕝(���a)  2:��  3:wait
//-------------------------------------------------------------
void TaskHaikeiQuake(u8 id)
{
	TaskTable[id].work[0] = WazaEffWork[0];
	TaskTable[id].work[1] = WazaEffWork[1];
	TaskTable[id].work[2] = WazaEffWork[2];
	TaskTable[id].work[3] = WazaEffWork[3];
	TaskTable[id].work[8] = WazaEffWork[3];	//�l�ێ�
	ScrX3 = WazaEffWork[0];
	ScrY3 = WazaEffWork[1];
	TaskTable[id].TaskAdrs = TaskHaikeiQuakeMain;
	TaskTable[id].TaskAdrs(id);
}

static void TaskHaikeiQuakeMain(u8 id)
{
	if(TaskTable[id].work[3] == 0)
	{
		if(ScrX3 == TaskTable[id].work[0])
			ScrX3 = -TaskTable[id].work[0];
		else
			ScrX3 = TaskTable[id].work[0];
		if(ScrY3 == -TaskTable[id].work[1])
			ScrY3 = 0;
		else
			ScrY3 = -TaskTable[id].work[1];
		TaskTable[id].work[3] = TaskTable[id].work[8];
		TaskTable[id].work[2]--;

		if(TaskTable[id].work[2] == 0)
		{
			ScrX3 = 0;
			ScrY3 = 0;
			DelTaskEffect(id);
		}
	}
	else
		TaskTable[id].work[3]--;
}






//=========================================================================
//
//		�q�b�g�}�[�N
//												by matsuda 2002.01.08(��)
//=========================================================================
static void HitMarkInitSeq(actWork *xreg);
static void HitMarkYflipInitSeq(actWork *xreg);
static void RandomHitMarkInitSeq(actWork *xreg);
static void SearchHitMarkInitSeq(actWork *xreg);
static void XHitMarkInitSeq(actWork *xreg);
static void HitMarkBlinkInit(actWork *xreg);
static void HitMarkBlinkMain(actWork *xreg);


#define HITMARK_AFF		200
#define HITMARK_AFLOOP	4
#define HITMARK_AFSP	(HITMARK_AFF / HITMARK_AFLOOP)
#define HITMARK_WAIT	8

static const actAffAnm HitMarkAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
//	{0x100 - HITMARK_AFF, 0x100 - HITMARK_AFF, 0, ACT_AFSET},
//	{ HITMARK_AFSP, HITMARK_AFSP, 0, HITMARK_AFLOOP},
//	{0, 0, 0, 7},
	{0,0,0,HITMARK_WAIT},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm HitMarkAff1[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0xd8,0xd8,0,ACT_AFSET},
	{0,0,0,HITMARK_WAIT},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm HitMarkAff2[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0xb0,0xb0,0,ACT_AFSET},
	{0,0,0,HITMARK_WAIT},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm HitMarkAff3[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0x80,0x80,0,ACT_AFSET},
	{0,0,0,HITMARK_WAIT},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const HitMarkAffTbl[] = {
	HitMarkAff0,
	HitMarkAff1,
	HitMarkAff2,
	HitMarkAff3,
};
const actHeader ActHitMarkHeader = {	//ɰ��˯�ϰ�
	WAZACHR_ID_HITMARK,
	WAZACHR_ID_HITMARK,
	&WazaOamDataBldAffine[2],
	DummyActAnmTbl,
	0,
	HitMarkAffTbl,
	HitMarkInitSeq,
};
const actHeader ActHitMarkYflipHeader = {	//ɰ��˯�ϰ�(�G������Y���W�̵̾�Ă𔽓])
	WAZACHR_ID_HITMARK,
	WAZACHR_ID_HITMARK,
	&WazaOamDataBldAffine[2],
	DummyActAnmTbl,
	0,
	HitMarkAffTbl,
	HitMarkYflipInitSeq,
};
const actHeader ActMizuHitHeader = {	//���n˯�ϰ�
	WAZACHR_ID_MIZUHIT,
	WAZACHR_ID_MIZUHIT,
	&WazaOamDataBldAffine[2],
	DummyActAnmTbl,
	0,
	HitMarkAffTbl,
	HitMarkInitSeq,
};
const actHeader ActRandomHitMarkHeader = {	// �\���ʒu�����˯�ϰ�
	WAZACHR_ID_HITMARK,
	WAZACHR_ID_HITMARK,
	&WazaOamDataBldAffine[2],
	DummyActAnmTbl,
	0,
	HitMarkAffTbl,
	RandomHitMarkInitSeq,
};
const actHeader ActSearchHitMarkHeader = {	// �Ώےǐ�˯�ϰ�
	WAZACHR_ID_HITMARK,
	WAZACHR_ID_HITMARK,
	&WazaOamDataBldAffine[2],
	DummyActAnmTbl,
	0,
	HitMarkAffTbl,
	SearchHitMarkInitSeq,
};
const actHeader ActXHitMarkHeader = {	// �w�^�q�b�g�}�[�N
	WAZACHR_ID_XHITMARK,
	WAZACHR_ID_XHITMARK,
	&WazaOamDataBldNormal[2],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	XHitMarkInitSeq,
};
const actHeader ActHitMarkBlinkHeader = {	//ɰ��˯�ϰ�(�񔼓���,�_��Ver)
	WAZACHR_ID_HITMARK,
	WAZACHR_ID_HITMARK,
	&WazaOamDataAffine[2],
	DummyActAnmTbl,
	0,
	HitMarkAffTbl,
	HitMarkBlinkInit,
};

//----------------------------------------------------------
// offset 0:Xoffset  1:Yoffset  2: 0=�U�����ɏo��  1=�h�䑤�ɏo��
//		  3:˯�ϰ��̑傫���w��(�� 0�`3 ��)
//----------------------------------------------------------
static void HitMarkInitSeq(actWork *xreg)
{
	ActAffAnmChg(xreg, WazaEffWork[3]);
	if(WazaEffWork[2] == 0)
		WazaDefaultPosSet2(xreg, PM_MODE);
	else
		WazaDefaultPosSet1(xreg, PM_MODE);
	xreg->move = W_AffEndWait;
	SetWorkSeq(xreg, DelWazaEffect);
}
//----------------------------------------------------------
// offset 0:Xoffset  1:Yoffset  2: 0=�U�����ɏo��  1=�h�䑤�ɏo��
//		  3:˯�ϰ��̑傫���w��(�� 0�`3 ��),4:Wait
//		  
//	*AffineAnim�I����A�w�肵��Sync�E�F�C�g
//												02.08.27_add_iwasawa
//----------------------------------------------------------
extern void SeqWazaEffectEnd(actWork *xreg);
static void HitMarkWaitInitSeq(actWork *xreg)
{
	ActAffAnmChg(xreg, WazaEffWork[3]);
	if(WazaEffWork[2] == 0)
		WazaDefaultPosSet2(xreg, PM_MODE);
	else
		WazaDefaultPosSet1(xreg, PM_MODE);

	//Wait��ۑ�
	xreg->work[0] = WazaEffWork[4];
	xreg->move = W_AffEndWait;
	SetWorkSeq(xreg, SeqWazaEffectEnd);
}
const actHeader ActHitMarkWaitHeader = {	//ɰ��˯�ϰ�,Wait�t��
	WAZACHR_ID_HITMARK,
	WAZACHR_ID_HITMARK,
	&WazaOamDataBldAffine[2],
	DummyActAnmTbl,
	0,
	HitMarkAffTbl,
	HitMarkWaitInitSeq,
};

//----------------------------------------------------------
// offset 0:Xoffset  1:Yoffset  2: 0=�U�����ɏo��  1=�h�䑤�ɏo��
//		  3:˯�ϰ��̑傫���w��(�� 0�`3 ��)
//----------------------------------------------------------
static void HitMarkYflipInitSeq(actWork *xreg)
{
	if(MineEnemyCheck(AttackNo) && ContestCheck()==0)
		WazaEffWork[1] = -WazaEffWork[1];
	HitMarkInitSeq(xreg);
}

//----------------------------------------------------------
// offset 0:(0:�U����  1:�h�䑤)  
//		  1:˯�ϰ��̑傫���w��(�� 0�`3 ��  -1:�����)
//----------------------------------------------------------
static void RandomHitMarkInitSeq(actWork *xreg)
{
	if(WazaEffWork[1] == -1){
		WazaEffWork[1] = pp_rand() & 3;
	}
	ActAffAnmChg(xreg, WazaEffWork[1]);

	if(WazaEffWork[0] == 0)
		WazaDefaultPosSet2(xreg, EFF_MODE);
	else
		WazaDefaultPosSet1(xreg, EFF_MODE);

	xreg->dx += ((pp_rand() % 48) - 24);
	xreg->dy += ((pp_rand() % 24) - 12);

//	SetWorkSeq(xreg, DelWazaEffect);
	SetWorkSeq(xreg, W_AffCheckEndSeq);
	xreg->move = W_AffEndWait;
}
//----------------------------------------------------------
// offset 0:(0:�U����  1:�h�䑤)  
//        1:xofs  2:yofs
//		  3:˯�ϰ��̑傫���w��(�� 0�`3 ��  -1:�����)
//----------------------------------------------------------
static void SearchHitMarkInitSeq(actWork *act)
{
	act->work[0] = W_GetPokeActNo(WazaEffWork[0]);

	act->x = ActWork[act->work[0]].x + ActWork[act->work[0]].dx;
	act->y = ActWork[act->work[0]].y + ActWork[act->work[0]].dy;
	act->dx = WazaEffWork[1];
	act->dy = WazaEffWork[2];

	ActAffAnmChg(act,WazaEffWork[3]);
	SetWorkSeq(act, W_AffCheckEndSeq);
	act->move = W_AffEndWait;
}
//----------------------------------------------------------
// offset 0:Xoffset  1:Yoffset  2: 0=�U�����ɏo��  1=�h�䑤�ɏo��
//		  3:�\��sync��
//----------------------------------------------------------
static void XHitMarkInitSeq(actWork *xreg)
{
	if(WazaEffWork[2] == 0)
		WazaDefaultPosSet2(xreg, PM_MODE);
	else
		WazaDefaultPosSet1(xreg, PM_MODE);

	xreg->work[0] = WazaEffWork[3];
	SetWorkSeq(xreg,DelWazaEffect);
	xreg->move = W_WaitSeq;
}

//----------------------------------------------------------
// offset 0:Xoffset  1:Yoffset  2: 0=�U�����ɏo��  1=�h�䑤�ɏo��
//		  3:˯�ϰ��̑傫���w��(�� 0�`3 ��)
//----------------------------------------------------------
static void HitMarkBlinkInit(actWork *xreg)
{
	ActAffAnmChg(xreg, WazaEffWork[3]);
	if(WazaEffWork[2] == 0)
		WazaDefaultPosSet2(xreg, PM_MODE);
	else
		WazaDefaultPosSet1(xreg, PM_MODE);
	xreg->move = HitMarkBlinkMain;
}

static void HitMarkBlinkMain(actWork *xreg)
{
	xreg->banish ^= 1;
//	if(xreg->affend_sw)
	if(xreg->work[0]++ > HITMARK_WAIT + HITMARK_WAIT/2)
		DelWazaEffect(xreg);
}





//=========================================================================
//
//		New �p���b�g�t�F�[�h
//												by matsuda 2002.01.18(��)
//=========================================================================
static void ColorFadeMainSeq(u8 id);
static void ColorFadeCommonSeq(u8 id, u32 fade_bit);

//----------------------------------------------------------
//	̪���ظ���
// offset 0:̪��ޑΏ� 0bit=�w�i(�y�n�܂�) 1bit=�U�����߹�� 2bit=�h�䑤�߹��
//					  3bit=�U�����߹�ݑ���  4bit=�h�䑤�߹�ݑ���
//					  5bit=�̪��BG1  6bit=�̪��BG2
//					  ���������I���ōU���A�h�䑤�Ƃ��Ď��Ȃ����̂��߂ɒǉ���
//					  7bit=Mine1  8bit=Mine2	9bit=Enemy1	10bit=Enemy2
// 1:wait
// 2:start_evy
// 3:end_evy
// 4:color1(RGB�w��)
//----------------------------------------------------------
void ColorFadeSeq(u8 id)
{
	u32 fade_bit = 0;
	u32 poke_bit = 0;
	u8 mine0=0,mine1=0,enemy0=0,enemy1=0;
	
	fade_bit = WazaSeqColorInitSub(WazaEffWork[0]);
	
	//-- add 2002.09.03(��) --//
	if((WazaEffWork[0] >> 7) & 1)
		mine0 = 1;
	if((WazaEffWork[0] >> 8) & 1)
		mine1 = 1;
	if((WazaEffWork[0] >> 9) & 1)
		enemy0 = 1;
	if((WazaEffWork[0] >> 10) & 1)
		enemy1 = 1;
	poke_bit = GetPokeFadeBit(mine0, mine1, enemy0, enemy1);
	fade_bit |= poke_bit;
	//-- �����܂� --//
	
	ColorFadeCommonSeq(id, fade_bit);
}

//----------------------------------------------------------
//	�ΏۈȊO�S�Ă�̪���ظ���
// offset 0:̪��ޏ��O�Ώ� 
//(0:�U���|�P  1:�h��|�P  2:�U���߹&�w�i  3:�h���߹&�w�i�j
//(4:�U���߹���h���߹  5:�ΏۂȂ�)
//(5:�U���|�P����  6:�h��|�P�����j
// 1:wait
// 2:start_evy
// 3:end_evy
// 4:color1(RGB�w��)
//----------------------------------------------------------
void ExceptColorFadeSeq(u8 id)
{
	u32 fade_bit;
	u8  client[2];
	u8  i;

	client[1] = 0xff;  // �����ȸײ������ް
	fade_bit = WazaSeqColorInitSub(1);  // �w�i
	switch(WazaEffWork[0]){
	case 2:
		fade_bit = 0;
	case 0:
		client[0] = AttackNo;
		break;

	case 3:
		fade_bit = 0;
	case 1:
		client[0] = DefenceNo;
		break;

	case 4:
		client[0]=AttackNo;
		client[1]=DefenceNo;
		break;

	case 5:
		client[0]=0xff;
		break;

	case 6:
		fade_bit = 0;
		client[0] = AttackNo^2;
		break;

	case 7:
		fade_bit = 0;
		client[0] = DefenceNo^2;
	}

	for(i = 0; i < 4; i++){
		if(i != client[0] && i != client[1] && PokeBG2taimeCheck(i)){
			fade_bit |= (0x00010000 << GetClientPokePalNo(i));
		}
	}
	
	ColorFadeCommonSeq(id, fade_bit);
}
//----------------------------------------------------------
//	�n�`�Ɠ����悤�ȐF�ɂȂ�悤��̪���ظ��āi�ق����傭�j
// offset 0:̪��ޑΏ� 0bit=�w�i(�y�n�܂�) 1bit=�U�����߹�� 2bit=�h�䑤�߹��
//					  3bit=�U�����߹�ݑ���  4bit=�h�䑤�߹�ݑ���
//					  5bit=�̪��BG1  6bit=�̪��BG2
// 1:wait
// 2:start_evy
// 3:end_evy
//----------------------------------------------------------
void FieldColorFadeSeq(u8 id)
{
	u32 fade_bit = 0;

	fade_bit = WazaSeqColorInitSub(WazaEffWork[0]);

	switch(GroundNo){
	case GROUND_KUSA:     WazaEffWork[4] = 0x0b0c; break;
	case GROUND_LONGKUSA: WazaEffWork[4] = 0x09e0; break;
	case GROUND_SUNA:     WazaEffWork[4] = 0x2f1e; break;
	case GROUND_MIZU:     WazaEffWork[4] = 0x4800; break;
	case GROUND_UMI:      WazaEffWork[4] = 0x7ecb; break;
	case GROUND_IKE:      WazaEffWork[4] = 0x7ecb; break;
	case GROUND_IWA:      WazaEffWork[4] = 0x2a16; break;
	case GROUND_DOUKUTSU: WazaEffWork[4] = 0x0d2e; break;
	case GROUND_ROOM:     WazaEffWork[4] = 0x7fff; break;
	case GROUND_ALL:      WazaEffWork[4] = 0x7fff; break;

	}

	ColorFadeCommonSeq(id, fade_bit);
}



//----------------------------------------------------------
//	WAZACHR_ID���w�肵��̪���ظ���
// offset 0: �t�F�[�h������WAZACHR_ID
// 1:wait
// 2:start_evy
// 3:end_evy
// 4:color1(RGB�w��)
//----------------------------------------------------------
void WazaChrColorFade(u8 id)
{
	u8 pal_no;
	u32 fade_bit;
	
	pal_no = ObjPalNumGet(WazaEffWork[0]);
	fade_bit = 1 << (16 + pal_no);
	ColorFadeCommonSeq(id, fade_bit);
}

static void ColorFadeCommonSeq(u8 id, u32 fade_bit)
{
	TaskTable[id].work[0] = fade_bit & 0xffff;
	TaskTable[id].work[1] = fade_bit >> 16;
	TaskTable[id].work[2] = WazaEffWork[1];
	TaskTable[id].work[3] = WazaEffWork[2];
	TaskTable[id].work[4] = WazaEffWork[3];
	TaskTable[id].work[5] = WazaEffWork[4];
	TaskTable[id].work[10] = WazaEffWork[2];
	TaskTable[id].TaskAdrs = ColorFadeMainSeq;
	TaskTable[id].TaskAdrs(id);
}

//�Ƃ肠����9������v�Z�pܰ��Ƃ��Ďg�p���āA���̑O��ܰ��́��̂Ŏg�p�ɂ��Ă�
static void ColorFadeMainSeq(u8 id)
{
	u32 fade_bit;
	u16 src = 0;
	
	if(TaskTable[id].work[9] == TaskTable[id].work[2])
	{
		TaskTable[id].work[9] = 0;
		fade_bit = TaskTable[id].work[0] | (TaskTable[id].work[1] << 16);
		while(fade_bit)
		{
			if(fade_bit & 1)
				SoftFade(src, 16, TaskTable[id].work[10], TaskTable[id].work[5]);
			src += 16;
			fade_bit >>= 1;
		}
		if(TaskTable[id].work[10] < TaskTable[id].work[4])
			TaskTable[id].work[10]++;
		else if(TaskTable[id].work[10] > TaskTable[id].work[4])
			TaskTable[id].work[10]--;
		else
			DelTaskEffect(id);
	}
	else
		TaskTable[id].work[9]++;
}







//=========================================================================
//
//		�n�[�h�t�F�[�h���N�G�X�g
//												by matsuda 2002.06.08(�y)
//=========================================================================
static void WazaHardFadeTaskWait(u8 id);

//----------------------------------------------------------
//	HardFadeReq�֐��̈��������̂܂܎w��
//
// 0,fade_bit : �����Z�̑Ώۂɂ���BG,OBJ���w��(BLDCNTڼ޽��̑�1�Ώ��߸�قɐݒ肷���ޯ�)
//				6,7bit:���x�ύX���߁A�޳��ޯ�(10:UP	 11:DOWN)
//				5bit:BD(�ޯ���׳���), 4bit:OBJ, 3:BG3, 2:BG2, 1:BG1, 0:BG0
//
// 1,wait : �l��傫������Ƃ������̪��ޱ�āA�݂��܂� (MAX:63)
// 2,start_evy : �ŏ��̐F�̔Z�x���w�肵�܂�(0�`16)
// 			   0(���̐F)�`2,3..(���ԐF)�`16(���A�������͔�)
// 3,end_evy : �ŏI�I�ȐF�̔Z�x���w�肵�܂��B�����̒l�́��Ɠ����ł�
// 4,reset_flg : ̪��ޏI������ڼ޽���̏����������邩(0:���Ȃ�  1:����)
//----------------------------------------------------------
void WazaHardFadeTask(u8 id)
{
	HardFadeReq(WazaEffWork[0],WazaEffWork[1],WazaEffWork[2],
			WazaEffWork[3],WazaEffWork[4]);
	TaskTable[id].TaskAdrs = WazaHardFadeTaskWait;
}

static void WazaHardFadeTaskWait(u8 id)
{
	if(FadeData.fade_sw == 0)
		DelTaskEffect(id);
}








//=========================================================================
//	����I�Ƀ|�P�����c����\������^�X�N
//
//																	TAYA
//=========================================================================
static void TaskPokeZanzouMain(u8 id);
static void zanzou_actmove(actWork *act);

//-------------------------------------------------------------
//	������
//  0:0=�U����  1=�h�䑤
//	1:�E�F�C�gsync��  2:�c���̎���sync��  3:�\����
//-------------------------------------------------------------
void TaskPokeZanzou(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	my->work[0] = WazaEffWork[0];  // 0:�U����or�h�䑤
	my->work[1] = 0;               // 1:����
	my->work[2] = WazaEffWork[1];  // 2:wait
	my->work[3] = WazaEffWork[2];  // 3:�c���̎���
	my->work[4] = WazaEffWork[3];  // 4:�ő�\����
	my->work[5] = 0;               // 5:���݂̕\����
	my->TaskAdrs = TaskPokeZanzouMain;
}

static void TaskPokeZanzouMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	if(my->work[4]){
		if(my->work[1]){
			my->work[1]--;
		}else{
			my->work[6] = W_PokeZanzouMake(my->work[0]);
			// �c�������Ɏ���sync���A���ܰ��̶����ʒu�ȂǏ����Z�b�g
			if(my->work[6] >= 0){
				ActWork[my->work[6]].oamData.Priority = (my->work[0])? 1 : 2;
				ActWork[my->work[6]].work[0] = my->work[3];
				ActWork[my->work[6]].work[1] = id;
				ActWork[my->work[6]].work[2] = 5;
				ActWork[my->work[6]].move = zanzou_actmove;
				my->work[5]++;
			}
			my->work[4]--;
			my->work[1] = my->work[2];
		}
	}else{
		if(my->work[5] == 0){
			DelTaskEffect(id);
		}
	}

}
static void zanzou_actmove(actWork *act)
{
	if(act->work[0]){
		act->work[0]--;
	}else{
		TaskTable[act->work[1]].work[act->work[2]]--;
		W_PokeZanzouDel(act);
	}
}












//=========================================================================
//
//		�̂낢�m�[�}���G�t�F�N�g
//												by matsuda 2002.05.21(��)
//=========================================================================
static void StUpEffectMain(u8 id);

//----------------------------------------------------------
//	offset �Ȃ�
//----------------------------------------------------------
const u16 st_up_color = 0x7fff;

void StUpEffectTask(u8 id)
{
	u8 pokeact, winact;
	u16 monsno;
	WazaBgData wazabg;
	u8 pri_flg = 0;
	
	Win0H = 0;
	Win0V = 0;
	*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
	*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | 0x3d;
	*(vu16*)REG_DISPCNT |= DISP_OBJWIN_ON;
	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL | BLD_BG1_1ST;
	*(vu16*)REG_BLDALPHA = (0x0c << 8) | 0x8;

	(*(vBgCnt *)REG_BG1CNT).Priority = 0;
	(*(vBgCnt *)REG_BG1CNT).Size = 0;
	(*(vBgCnt *)REG_BG1CNT).Loop = 1;
	if(ContestCheck() == 0)//�̪�Ă̊Ԃ�����׸��ް���ύX(��ذ��ް��̊֌W��)
		(*(vBgCnt *)REG_BG1CNT).CharBasep = 1;
//��ײ��è���Ⴂ�߹�݂��Ώۂ̏ꍇ�A�ׂ��߹�݂ɴ̪�Ă����Ԃ�Ȃ��悤����ײ��è���グ��
	if(FightCheck() && ContestCheck()==0)
	{
		if(ClientTypeGet(AttackNo) == BSS_CLIENT_ENEMY2
			|| ClientTypeGet(AttackNo) == BSS_CLIENT_MINE)
		{
			if(PokeBG2taimeCheck(AttackNo^2) == 1){
				ActWork[PokemonStruct[AttackNo^2]].oamData.Priority--;
				(*(vBgCnt *)REG_BG1CNT).Priority = 1;
				pri_flg = 1;
			}
		}
	}

	if(ContestCheck())
		monsno = conpara->monsno;
	else if(MineEnemyCheck(AttackNo))
		monsno = PokeParaGet(&PokeParaEnemy[SelMonsNo[AttackNo]], ID_monsno);
	else
		monsno = PokeParaGet(&PokeParaMine[SelMonsNo[AttackNo]], ID_monsno);
	
	//-- WinObj --//
	pokeact = W_GetPokeActNo(0);
	winact = PokeWinObjMake(AttackNo, pokeact, monsno);
	
	//-- ���G�t�F�N�g --//
	GetWazaEffBgAdrs(&wazabg);
	DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x1000, 32);
	DecordVram((void*)st_up_psc_ADRS, wazabg.ScrnAdrs);
	if(ContestCheck())
		ScrnPalRewrite(wazabg.PlttNo, (u32)wazabg.ScrnAdrs, 0, 0);
	DecordVram((void*)st_up_pch_ADRS, wazabg.CharAdrs);
	//DecordPaletteWork((void*)metaru_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	PaletteWorkSet(&st_up_color, wazabg.PlttNo*16+1, 2);
	ScrX1 = -ActWork[pokeact].x + 32;
//	ScrY1 = -ActWork[pokeact].y + 32-64;
	ScrY1 = -ActWork[pokeact].y + 32;

	TaskTable[id].work[0] = winact;
	TaskTable[id].work[6] = pri_flg;
	TaskTable[id].TaskAdrs = StUpEffectMain;
}

static void StUpEffectMain(u8 id)
{
	u16 palno;
	u8 pokeact;
	WazaBgData wazabg;
	
	TaskTable[id].work[10] += 4;
//	ScrY1 += 4;
	ScrY1 -= 4;
	if(TaskTable[id].work[10] == 64){
		TaskTable[id].work[10] = 0;
//		ScrY1 -= 64;
		ScrY1 += 64;
		if(++TaskTable[id].work[11] == 4){
			PokeBGDropClear(0);
			Win0H = 0;
			Win0V = 0;
			*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
			*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | WIN_ALL_ON;
			if(ContestCheck() == 0)
				(*(vBgCnt *)REG_BG1CNT).CharBasep = 0;
			*(vu16*)REG_DISPCNT ^= DISP_OBJWIN_ON;
			*(vu16*)REG_BLDCNT = 0;
			*(vu16*)REG_BLDALPHA = 0;
			pokeact = W_GetPokeActNo(0);
			palno = ActWork[pokeact].oamData.Pltt + 16;
			//AffineDelActor(&ActWork[TaskTable[id].work[0]]);
			DelActor(&ActWork[TaskTable[id].work[0]]);
			GetWazaEffBgAdrs(&wazabg);
			DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x800, 32);
			if(TaskTable[id].work[6] == 1){
				ActWork[PokemonStruct[AttackNo^2]].oamData.Priority++;
			}
			(*(vBgCnt *)REG_BG1CNT).Loop = 0;
			ScrY1 = 0;
			DelTaskEffect(id);
		}
	}
}







//=========================================================================
//
//		�X�e�[�^�X�㏸�E���~�G�t�F�N�g
//												by matsuda 2002.05.21(��)
//=========================================================================
static void StatusEffectMain(u8 id);
static void StatusEffectInit(u8 id);
//----------------------------------------------------------
//	offset 0:�㏸�E���~�׸�(0=�㏸�A1=���~)
//		   1:�ǂ̽ð����(0:�U�� 1:�h�� 2:���� 3:���΂₳ 
//						 4:��� 5:���U 6:���h 0xff:����)
//		   2:0=�U�����ɏo��	  1=�h�䑤�ɏo��
//		   3:1=�����ɂ��o��	0=�����ɂ͏o���Ȃ�
//		   4:0=1�i�K	1=2�i�K
//----------------------------------------------------------
#define STATUS_EFF_ADD_Y	3
#define STATUS_EFF_BLDWAIT	1
void StatusEffectTask(u8 id)
{	//1Sync�Ԃ��󂯂�
	u8 i;
	for(i = 0; i < 8; i++)
		TaskTable[id].work[i] = WazaEffWork[i];
	TaskTable[id].TaskAdrs = StatusEffectInit;
}

static void StatusEffectInit(u8 id)
{
	u8 pokeact, winact;
	u8 pokeact2=0, winact2=0;
	u16 monsno;
	WazaBgData wazabg;
	u8 client_no;
	u8 aikata = 0;
	s16 work[8];
	int i;
	u8 pri_flg = 0;
	
	for(i = 0; i < 8; i++){
		work[i] = TaskTable[id].work[i];
		TaskTable[id].work[i] = 0;
	}
	
	if(work[2] == 0)
		client_no = AttackNo;
	else
		client_no = DefenceNo;
	aikata = client_no^2;
	
	if(ContestCheck() || (work[3] && PokeBG2taimeCheck(aikata)==0))
		work[3] = 0;
	
	Win0H = 0;
	Win0V = 0;
	*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
	*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | 0x3d;
	*(vu16*)REG_DISPCNT |= DISP_OBJWIN_ON;
	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL | BLD_BG1_1ST;
	*(vu16*)REG_BLDALPHA = 16 << 8;

	(*(vBgCnt *)REG_BG1CNT).Priority = 0;
	(*(vBgCnt *)REG_BG1CNT).Size = 0;
	(*(vBgCnt *)REG_BG1CNT).Loop = 1;
	if(ContestCheck() == 0)//�̪�Ă̊Ԃ�����׸��ް���ύX(��ذ��ް��̊֌W��)
		(*(vBgCnt *)REG_BG1CNT).CharBasep = 1;
//��ײ��è���Ⴂ�߹�݂��Ώۂ̏ꍇ�A�ׂ��߹�݂ɴ̪�Ă����Ԃ�Ȃ��悤����ײ��è���グ��
	if(FightCheck() && work[3] == 0)
	{
		if(ClientTypeGet(client_no) == BSS_CLIENT_ENEMY2
			|| ClientTypeGet(client_no) == BSS_CLIENT_MINE)
		{
			if(PokeBG2taimeCheck(aikata) == 1){
				ActWork[PokemonStruct[aikata]].oamData.Priority--;
				(*(vBgCnt *)REG_BG1CNT).Priority = 1;
				pri_flg = 1;
			}
		}
	}
	
	if(ContestCheck()){
		monsno = conpara->monsno;
	}
	else{
		if(MineEnemyCheck(client_no))
			monsno = PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_monsno);
		else
			monsno = PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_monsno);
	}

	//-- WinObj --//
	pokeact = PokemonStruct[client_no];		//W_GetPokeActNo(0);
	winact = PokeWinObjMake(client_no, pokeact, monsno);
	if(work[3]){
		pokeact2 = PokemonStruct[aikata];		//W_GetPokeActNo(0);
		winact2 = PokeWinObjMake(aikata, pokeact2, monsno);
	}
	
	//-- ���G�t�F�N�g --//
	GetWazaEffBgAdrs(&wazabg);
	//DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x1000, 32);
	//DecordVram((void*)s_p_psc_ADRS, wazabg.ScrnAdrs);
	if(work[0] == 0)
		DecordVram((void*)s_p_up_psc_ADRS, wazabg.ScrnAdrs);
	else
		DecordVram((void*)s_p_down_psc_ADRS, wazabg.ScrnAdrs);
	if(ContestCheck())
		ScrnPalRewrite(wazabg.PlttNo, (u32)wazabg.ScrnAdrs, 0, 0);
	DecordVram((void*)s_p_pch_ADRS, wazabg.CharAdrs);
	if(work[1] == 0)
		DecordPaletteWork((void*)s_pkoug_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	else if(work[1] == 1)
		DecordPaletteWork((void*)s_pboug_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	else if(work[1] == 2)
		DecordPaletteWork((void*)s_pmei_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	else if(work[1] == 3)
		DecordPaletteWork((void*)s_psuba_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	else if(work[1] == 4)
		DecordPaletteWork((void*)s_pkai_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	else if(work[1] == 5)
		DecordPaletteWork((void*)s_ptkou_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	else if(work[1] == 6)
		DecordPaletteWork((void*)s_ptbou_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	else
		DecordPaletteWork((void*)s_pmult_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	
//	ScrX1 = -ActWork[pokeact].x + 32;
//	ScrY1 = -ActWork[pokeact].y + 32-64;
	ScrX1 = 0;
	ScrY1 = 0;
	if(work[0] == 1){
		ScrX1 += 64;
		TaskTable[id].work[1] = -STATUS_EFF_ADD_Y;
	}
	else
		TaskTable[id].work[1] = STATUS_EFF_ADD_Y;
	
	if(work[4] == 0){
		TaskTable[id].work[4] = 10;//7;	//1�i�K����BLDMAX
		TaskTable[id].work[5] = 20;	//wait
	}
	else{
		TaskTable[id].work[4] = 13;//11;	//2�i�K����BLDMAX
		TaskTable[id].work[5] = 30;
	}
	
	TaskTable[id].work[0] = winact;
	TaskTable[id].work[2] = work[3];
	TaskTable[id].work[3] = winact2;
	TaskTable[id].work[6] = pri_flg;
	TaskTable[id].work[7] = PokemonStruct[aikata];
	TaskTable[id].TaskAdrs = StatusEffectMain;
	
	if(work[0] == 0)
		SePlayPan(SE_W287, GetSePanAttack(WAZA_SE_L));
	else
		SePlayPan(SE_W287B, GetSePanAttack(WAZA_SE_L));
}

static void StatusEffectMain(u8 id)
{
	ScrY1 += TaskTable[id].work[1];
	switch(TaskTable[id].work[15]){
		case 0:
			if(TaskTable[id].work[11]++ < STATUS_EFF_BLDWAIT)
				break;
			TaskTable[id].work[11] = 0;
			TaskTable[id].work[12]++;
			*(vu16*)REG_BLDALPHA = ((16-TaskTable[id].work[12])<<8) | TaskTable[id].work[12];
			if(TaskTable[id].work[12] == TaskTable[id].work[4])
				TaskTable[id].work[15]++;
			break;
		case 1:
			TaskTable[id].work[10]++;
			if(TaskTable[id].work[10] == TaskTable[id].work[5])
				TaskTable[id].work[15]++;
			break;
		case 2:
			if(TaskTable[id].work[11]++ < STATUS_EFF_BLDWAIT)
				break;
			TaskTable[id].work[11] = 0;
			TaskTable[id].work[12]--;
			*(vu16*)REG_BLDALPHA = ((16-TaskTable[id].work[12])<<8) | TaskTable[id].work[12];
			if(TaskTable[id].work[12] == 0){
				PokeBGDropClear(0);
				Win0H = 0;
				Win0V = 0;
				*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
				*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | WIN_ALL_ON;
				if(ContestCheck() == 0)
					(*(vBgCnt *)REG_BG1CNT).CharBasep = 0;
				*(vu16*)REG_DISPCNT ^= DISP_OBJWIN_ON;
				*(vu16*)REG_BLDCNT = 0;
				*(vu16*)REG_BLDALPHA = 0;
				//AffineDelActor(&ActWork[TaskTable[id].work[0]]);
				DelActor(&ActWork[TaskTable[id].work[0]]);
				if(TaskTable[id].work[2]){
					//AffineDelActor(&ActWork[TaskTable[id].work[3]]);
					DelActor(&ActWork[TaskTable[id].work[3]]);
				}
				if(TaskTable[id].work[6] == 1){
					ActWork[TaskTable[id].work[7]].oamData.Priority++;
				}
			//	GetWazaEffBgAdrs(&wazabg);
			//	DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x800, 32);
				(*(vBgCnt *)REG_BG1CNT).Loop = 0;
				DelTaskEffect(id);
			}
			break;
	}
}








//=========================================================================
//	�t���b�V��
//
//																	TAYA
//=========================================================================

static void TaskFlashMain(u8 id);
static void flash_sub(u32 fade_bit, u16 col);

void TaskFlash(u8 id)
{
	u32 fade_bit;

	// �߹�݂�S�č���
	fade_bit = GetPokeFadeBit(1,1,1,1);
	flash_sub(fade_bit, 0x0000);
	TaskTable[id].work[14] = (fade_bit >> 16);

	// �w�i��S�Ĕ����i̪����ޯĂ�ۑ��j
	fade_bit = GetBattleFadeBit(1,0,0,0,0,0,0);
	fade_bit &= 0xffff;
	flash_sub(fade_bit, 0xffff);
	TaskTable[id].work[15] = fade_bit;

	TaskTable[id].work[0] = 0;
	TaskTable[id].work[1] = 0;

	TaskTable[id].TaskAdrs = TaskFlashMain;
}

static void TaskFlashMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[0]){
	case 0:
		my->work[1]++;
		if(my->work[1] > 6){
			my->work[1] = 0;
			my->work[2] = 16;
			my->work[0]++;
		}
		break;

	case 1:
		if(++(my->work[1]) > 1)
		{
			u16 i;

			my->work[1] = 0;
			my->work[2]--;
			for(i = 0; i < 16; i++)
			{
				if(my->work[15] & (1<<i))
				{
					SoftFade(i*16, 16, my->work[2], 0xffff);
				}
				if(my->work[14] & (1<<i))
				{
					SoftFade(256+i*16, 16, my->work[2], 0x0000);
				}
			}
			if(my->work[2] == 0)
			{
				my->work[0]++;
			}
		}
		break;

	case 2:
		DelTaskEffect(id);
		break;
	}
}

static void flash_sub(u32 fade_bit, u16 col)
{
	u16 i, pal, p;

	for(i = 0; i < 32; i++){
		if(fade_bit & 1){
			pal = i * 16;
			for(p = pal; p < (pal+16); p++){
				PaletteWorkTrans[p] = col;
			}
		}
		fade_bit >>= 1;
	}
}



//----------------------------------------------------------
//	�����ȊO̪���ظ���
//
//	�����ȊO�̃|�P�������t�F�[�h�����܂��B
//
// 0:wait
// 1:start_evy
// 2:end_evy
// 3:color1(RGB�w��)
//----------------------------------------------------------
void TaskOtherPokemonFade(u8 id)
{
	u32 fade_bit,i;

	for(fade_bit=0,i=0;i<4;i++)
		if(AttackNo!=i)
			fade_bit |= 1<<(i+16);
	for(i=5;i>0;i--) WazaEffWork[i] = WazaEffWork[i-1];
	ColorFadeCommonSeq(id, fade_bit);
}





//=========================================================================
//
//		�w�i�G�̃X�N���[������^�X�N
//
//	��ɑ����̪�Ă�WAIT_FLAG���g����悤�� ���̃^�X�N���̂͂�����Del����܂��B
//�@�ς��ɂ����ŕʂ������Add����āA��������Ž�۰ِ�����s���܂��B
//	WazaEffWork[7]�ɏI�����ނ������Ă���Ƃ��������Del�����̂ŁA
//	�Y�ꂸ�ɏI�����ނ�����悤�ɂ��Ă��������B
//												by matsuda 2002.06.12(��)
//=========================================================================
static void HaikeiMapScrMain(u8 id);

//----------------------------------------------------------
//	offset 0:X������۰ّ��x(����8�r�b�g����)	1:Y������۰ّ��x(����8�r�b�g����)
//		   2:�G�����Ž�۰ٕ����̔��]���s��(0:�s��Ȃ��@1:�s��)
//		�@ 3:�I������(WazaEffWork[7]�ɂ��̏I�����ނ������Ă�����I�����܂�)
//----------------------------------------------------------
void HaikeiMapScrTask(u8 id)
{
	u8 task;
	
	HaikeiBGLoopSizeSet(0);	//�w�i��BG�ɑ΂��āA��荞���׸ށA���ނ�Ă���
	
	task = AddTask(HaikeiMapScrMain, 5);
	if(WazaEffWork[2] && MineEnemyCheck(AttackNo)){
		WazaEffWork[0] = -WazaEffWork[0];
		WazaEffWork[1] = -WazaEffWork[1];
	}
	TaskTable[task].work[1] = WazaEffWork[0];
	TaskTable[task].work[2] = WazaEffWork[1];
	TaskTable[task].work[3] = WazaEffWork[3];
	TaskTable[task].work[0]++;
	DelTaskEffect(id);
}

static void HaikeiMapScrMain(u8 id)
{
	TaskTable[id].work[10] += TaskTable[id].work[1];
	TaskTable[id].work[11] += TaskTable[id].work[2];
	ScrX3 += TaskTable[id].work[10]>>8;
	ScrY3 += TaskTable[id].work[11]>>8;
	TaskTable[id].work[10] &= 0x00ff;	//�����������c���ĸر
	TaskTable[id].work[11] &= 0x00ff;
	
	if(WazaEffWork[7] == TaskTable[id].work[3]){
		ScrX3 = 0;
		ScrY3 = 0;
		HaikeiBGLoopSizeSet(1);
		DelTask(id);	//west.s��ADD_TASK���߂�Add���ꂽ����ł͂Ȃ��̂ŕ��ʂ�Del
	}
}


//=========================================================================
//	MineEnemyCheck(AttackNo)�̌��ʂ̒l��WazaEffWork[7]�ɓ����
//=========================================================================
void MineEnemyChkTask(u8 id)
{
	WazaEffWork[7] = MineEnemyCheck(AttackNo);
	DelTaskEffect(id);
}
//=========================================================================
//	MineEnemyCheck(DefenceNo)�̌��ʂ̒l��WazaEffWork[7]�ɓ����
//=========================================================================
void MineEnemyChkTask2(u8 id)
{
	WazaEffWork[7] = MineEnemyCheck(DefenceNo);
	DelTaskEffect(id);
}
//=========================================================================
//	�����ւ̍U�����H WazaEffWork[7]�֓����B 0:�G��  1:������
//=========================================================================
void TaskPartyAttackChk(u8 id)
{
	WazaEffWork[7] = ((AttackNo^2)==DefenceNo);
	DelTaskEffect(id);
}
//==========================================================================
//  �U���߹�݈ȊO���߹�ݱ���banish�׸ނ𑀍삷��
//  0:banish�t���O�ɃZ�b�g����l
//==========================================================================
void W_PokeBanishCtrl(u8 id)
{
	u16 i;

	for(i = 0; i < 4; i++){
		if(i != AttackNo && PokeBG2taimeCheck(i)){
			ActWork[PokemonStruct[i]].banish = WazaEffWork[0];
		}
	}
	DelTaskEffect(id);
}













//=========================================================================
//
//		����ʂ��G�t�F�N�g���ʃ��[�`��
//												by matsuda 2002.07.29(��)
//=========================================================================
static void KurinukiEffectMain(u8 id);
//----------------------------------------------------------
//	id = �^�X�N��ID
//	x_sp = X���x	y_sp = Y���x	�Œ菬��
//	put_client = �Ώۂ̃N���C�A���gNo
//	partner = 1=�����ɂ��o��	0=�����ɂ͏o���Ȃ�
//	bld_max = BLD_MAX	bld_wait = BLD_WAIT		max_wait = BLD��Max�ɓ��B���Ă�Ԃ̳���
//	chr_adrs = �L�����̱��ڽ	scrn_adrs=��ذ��ް��̱��ڽ	palette_adrs=��گĂ̱��ڽ
//----------------------------------------------------------
void KurinukiEffectSet(u8 id, s16 x_sp, s16 y_sp, u8 put_client, u8 partner, u8 bld_max, u8 bld_wait, u8 max_wait, void *chr_adrs, void *scrn_adrs, void *palette_adrs)
{
	u8 pokeact, winact;
	u8 pokeact2=0, winact2=0;
	u16 monsno;
	WazaBgData wazabg;
	u8 aikata = 0;
	
	aikata = put_client^2;
	
	if(ContestCheck() || (partner && PokeBG2taimeCheck(aikata)==0))
		partner = 0;
	
	Win0H = 0;
	Win0V = 0;
	*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
	*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | 0x3d;
	*(vu16*)REG_DISPCNT |= DISP_OBJWIN_ON;
	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL | BLD_BG1_1ST;
	*(vu16*)REG_BLDALPHA = 16 << 8;

	(*(vBgCnt *)REG_BG1CNT).Priority = 0;
	(*(vBgCnt *)REG_BG1CNT).Size = 0;
	(*(vBgCnt *)REG_BG1CNT).Loop = 1;
	if(ContestCheck() == 0)//�̪�Ă̊Ԃ�����׸��ް���ύX(��ذ��ް��̊֌W��)
		(*(vBgCnt *)REG_BG1CNT).CharBasep = 1;

	if(ContestCheck()){
		monsno = conpara->monsno;
	}
	else{
		if(MineEnemyCheck(put_client))
			monsno = PokeParaGet(&PokeParaEnemy[SelMonsNo[put_client]], ID_monsno);
		else
			monsno = PokeParaGet(&PokeParaMine[SelMonsNo[put_client]], ID_monsno);
	}

	//-- WinObj --//
	pokeact = PokemonStruct[put_client];		//W_GetPokeActNo(0);
	winact = PokeWinObjMake(put_client, pokeact, monsno);
	if(partner){
		pokeact2 = PokemonStruct[aikata];		//W_GetPokeActNo(0);
		winact2 = PokeWinObjMake(aikata, pokeact2, monsno);
	}
	
	//-- ���G�t�F�N�g --//
	GetWazaEffBgAdrs(&wazabg);
	//DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x1000, 32);
	DecordVram(scrn_adrs, wazabg.ScrnAdrs);
	if(ContestCheck())
		ScrnPalRewrite(wazabg.PlttNo, (u32)wazabg.ScrnAdrs, 0, 0);
		
	DecordVram(chr_adrs, wazabg.CharAdrs);
	DecordPaletteWork(palette_adrs, wazabg.PlttNo*16, 0x20);
	
//	ScrX1 = -ActWork[pokeact].x + 32;
//	ScrY1 = -ActWork[pokeact].y + 32-64;
	ScrX1 = 0;
	ScrY1 = 0;
	TaskTable[id].work[1] = y_sp;
	TaskTable[id].work[4] = bld_max;
	TaskTable[id].work[5] = max_wait;
	TaskTable[id].work[6] = bld_wait;
	
	TaskTable[id].work[0] = winact;
	TaskTable[id].work[2] = partner;
	TaskTable[id].work[3] = winact2;
	TaskTable[id].TaskAdrs = KurinukiEffectMain;
}

static void KurinukiEffectMain(u8 id)
{
	TaskTable[id].work[13] += abs(TaskTable[id].work[1]);
	if(TaskTable[id].work[1] < 0)
		ScrY1 -= TaskTable[id].work[13] >> 8;
	else
		ScrY1 += TaskTable[id].work[13] >> 8;
	TaskTable[id].work[13] &= 0x00ff;
	
	switch(TaskTable[id].work[15]){
		case 0:
			if(TaskTable[id].work[11]++ < TaskTable[id].work[6])
				break;
			TaskTable[id].work[11] = 0;
			TaskTable[id].work[12]++;
			*(vu16*)REG_BLDALPHA = ((16-TaskTable[id].work[12])<<8) | TaskTable[id].work[12];
			if(TaskTable[id].work[12] == TaskTable[id].work[4])
				TaskTable[id].work[15]++;
			break;
		case 1:
			TaskTable[id].work[10]++;
			if(TaskTable[id].work[10] == TaskTable[id].work[5])
				TaskTable[id].work[15]++;
			break;
		case 2:
			if(TaskTable[id].work[11]++ < TaskTable[id].work[6])
				break;
			TaskTable[id].work[11] = 0;
			TaskTable[id].work[12]--;
			*(vu16*)REG_BLDALPHA = ((16-TaskTable[id].work[12])<<8) | TaskTable[id].work[12];
			if(TaskTable[id].work[12] == 0){
				PokeBGDropClear(0);
				Win0H = 0;
				Win0V = 0;
				*(vu16*)REG_WININ = (WIN_ALL_ON << 8) | WIN_ALL_ON;
				*(vu16*)REG_WINOUT = (WIN_ALL_ON << 8) | WIN_ALL_ON;
				if(ContestCheck() == 0)
					(*(vBgCnt *)REG_BG1CNT).CharBasep = 0;
				*(vu16*)REG_DISPCNT ^= DISP_OBJWIN_ON;
				*(vu16*)REG_BLDCNT = 0;
				*(vu16*)REG_BLDALPHA = 0;
				//AffineDelActor(&ActWork[TaskTable[id].work[0]]);
				DelActor(&ActWork[TaskTable[id].work[0]]);
				if(TaskTable[id].work[2]){
				//	AffineDelActor(&ActWork[TaskTable[id].work[3]]);
					DelActor(&ActWork[TaskTable[id].work[3]]);
				}
			//	GetWazaEffBgAdrs(&wazabg);
			//	DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x800, 32);
				(*(vBgCnt *)REG_BG1CNT).Loop = 0;
				DelTaskEffect(id);
			}
			break;
	}
}











//=========================================================================
//
//		���݂̒n�`No��WazaEffWork[0]�ɃZ�b�g����
//												by matsuda 2002.08.01(��)
//=========================================================================
void WorkSetGroundNo(u8 id)
{
	WazaEffWork[0] = GroundNo;
	DelTaskEffect(id);
}



//=========================================================================
//
//	�Z�G�t�F�N�g���̃p���b�g��UserWork�ɑޔ�����
//
//	WazaEffWork[0]: 0=�w�i	1=�U�����|�P����	2:�h�䑤�|�P����
//	WazaEffWork[1]: �p���b�g16�F�P�ʂ�UserWork�̉��{�ڂɑ��邩
//=========================================================================
void TaskWazaEffectPalPush(u8 id)
{
	u32 work,flag;
	flag = 0;
	if(WazaEffWork[0]==0){
		work = GetBattleFadeBit(1, 0, 0, 0, 0, 0, 0);		//�w�i�̃p���b�g�i���o�[���擾
		while((work&1)==0){
			work  = work>>1;
			flag++; 
		}
	}else if(WazaEffWork[0]==1){
		flag = AttackNo+16;
	}else if(WazaEffWork[0]==2){
		flag = DefenceNo+16;
	}
	memcpy(&UserWork[WAZAEFF_USERWORK+WazaEffWork[1]*32],&PaletteWork[flag*16],32);
	DelTaskEffect(id);
}

//=========================================================================
//
//	�Z�G�t�F�N�g���̃p���b�g��UserWork�ɑޔ�����
//
//	WazaEffWork[0]: 0=�w�i	1=�U�����|�P����	2:�h�䑤�|�P����
//	WazaEffWork[1]: �p���b�g16�F�P�ʂ�UserWork�̉��{�ڂ���߂���
//=========================================================================
void TaskWazaEffectPalPop(u8 id)
{
	u32 work,flag;
	flag = 0;
	if(WazaEffWork[0]==0){
		work = GetBattleFadeBit(1, 0, 0, 0, 0, 0, 0);		//�w�i�̃p���b�g�i���o�[���擾
		while((work&1)==0){
			work  = work>>1;
			flag++; 
		}
	}else if(WazaEffWork[0]==1){
		flag = AttackNo+16;
	}else if(WazaEffWork[0]==2){
		flag = DefenceNo+16;
	}
	memcpy(&PaletteWork[flag*16],&UserWork[WAZAEFF_USERWORK+WazaEffWork[1]*32],32);
	DelTaskEffect(id);
}

//=========================================================================
//
//	�Z�G�t�F�N�g���̃p���b�g��PaletteWorkTrans����PaletteWork�ɃR�s�[����
//
//	WazaEffWork[0]: 0=�w�i	1=�U�����|�P����	2:�h�䑤�|�P����
//
//=========================================================================
void TaskWazaEffectPalMoveTrans(u8 id)
{
	u32 work,flag;
	flag = 0;
	if(WazaEffWork[0]==0){
		work = GetBattleFadeBit(1, 0, 0, 0, 0, 0, 0);		//�w�i�̃p���b�g�i���o�[���擾
		while((work&1)==0){
			work  = work>>1;
			flag++; 
		}
	}else if(WazaEffWork[0]==1){
		flag = AttackNo+16;
	}else if(WazaEffWork[0]==2){
		flag = DefenceNo+16;
	}
	memcpy(&PaletteWork[flag*16],&PaletteWorkTrans[flag*16],32);
	DelTaskEffect(id);
}




//=========================================================================
//
//		�R���e�X�g��������WazaEffWork[7]��1��Ă���
//												by matsuda 2002.08.28(��)
//=========================================================================
void ContestChkTask(u8 id)
{
	if(ContestCheck())
		WazaEffWork[7] = 1;
	else
		WazaEffWork[7] = 0;
	DelTaskEffect(id);
}



//=========================================================================
//
//		�����p��AttackNo,DefenceNo���
//												by matsuda 2002.08.30(��)
//=========================================================================
void YokodoriClientSet(u8 id)
{
	AttackNo = DefenceClient;
	DefenceNo = TsuikaClient;
	DelTaskEffect(id);
}

//----------------------------------------------------------
//	�悱�ǂ�F�p�[�e�B�A�^�b�N�`�F�b�N
//	�p�[�e�B�A�^�b�N����WazaEffWork[7]��1������B�ʏ펞��0
//----------------------------------------------------------
void YokodoriPartyAttackChk(u8 id)
{
	if(MineEnemyCheck(AttackNo) == MineEnemyCheck(DefenceNo))
		WazaEffWork[7] = 1;
	else
		WazaEffWork[7] = 0;
	DelTaskEffect(id);
}


//=========================================================================
//
//		DefenceNo��DefenceClient������(FightEffect�Ŏg�p)
//												by matsuda 2002.08.30(��)
//=========================================================================
void DefenceNoClientSet(u8 id)
{
	DefenceNo = DefenceClient;
	DelTaskEffect(id);
}


//=========================================================================
//
//		�D�_�A�~������p�̒ǉ����ʂŔ�������ܴ̪�ėp��Attack�ADefence���
//												by matsuda 2002.08.30(��)
//=========================================================================
void AttackDefenceNoSet(u8 id)
{
	AttackNo = AttackClient;
	DefenceNo = TsuikaClient;
	DelTaskEffect(id);
}




//=========================================================================
//	Climem�̃o�j�b�V�������ꎞ�I�ɋ����o�j�b�V���ɂ�����(���̒l�͑ޔ�)
//	WazaEffWork[7]��0x1000�������Ă����猳�ɖ߂�
//=========================================================================
static void AttackClimemWait(u8 id);

void AttackClimemBanish(u8 id)
{
	if(ContestCheck()){
		DelTaskEffect(id);
		return;
	}
	TaskTable[id].work[0] = Climem[AttackNo].act_banish;
	Climem[AttackNo].act_banish = 1;
	TaskTable[id].TaskAdrs = AttackClimemWait;
	WazaEffectCount--;	//WAIT_FLAG��������悤�ɶ������Dec
}

static void AttackClimemWait(u8 id)
{
	if(WazaEffWork[7] == 0x1000){
		Climem[AttackNo].act_banish = TaskTable[id].work[0];
		DelTask(id);	//WazaEffectCount�͊���Dec���Ă���̂ŕ��ʂ����Del
	}
}

