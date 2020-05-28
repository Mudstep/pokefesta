
//=========================================================================
//
//		�p���b�g�A�j���E�t�F�[�h�A�E�g�A�C��
//											01/04/20 by matsuda
//=========================================================================

#include "common.h"
#include "palanm.h"
#include "softfade.h"
#include "ef_tool.h"
#include "decord.h"


//----------------------------------------------------------
//	�v���g�^�C�v�錾	�֐����ް�
//----------------------------------------------------------
u8 AddPalAnm(const PalAnmData *pAnmDat, u16 pal_no);
u8 PaletteAnime(void);
static void PaletteAnimeEndCheck(PalAnmWorkData *paw);
void DelPalAnm(u16 id_no);
void PalAnmInitWork(u8 pal_anm_no);
void PaletteAnimeInit(void);
void PalAnmPause(u16 id_no);
void PalAnmMove(u16 id_no);
u8 PalAnmGetWorkNumber(u16 id_no);
void PaletteFadeTrans( void );
void PlttCopy(void);
void PaletteAnimeBeforeAfter(u8 trans_type, u32 *PalAnmBit);
static void PaletteAnimeSub(PalAnmWorkData *paw, u32*);
u8 ColorAddSub(void);
static void FadeCheck(PalAnmWorkData *paw, u32 *PalAnmBit);
void FadeWorkInit(void);
u8 LightColorAddSub(void);
static u8 HardColorCalc(void);
static void HardColorRegisterSet(void);
static u8 FadeEndWaitCheck(void);
void LightFadeReqAdj(u8 fade_type, u8 fade_value);
static void LightFadeCommon(u8 fade_type);

//----------------------------------------------------------
//	�O���[�o���ϐ�
//----------------------------------------------------------
u16 PaletteWork[0x200] = {};			//�ؼ��ف@�@BG&OBJ��گ��ޯ�����
u16 PaletteWorkTrans[0x200] = {};		//�]���p
PalAnmWorkData PalAnmWork[PAL_ANM_MAX] = {};	//�p���b�g�A�j���pܰ�
PlttFadeData FadeData = {};				//̪��ޱ�āA�ݗp�\����
u32 PalAnmFadeBit = 0;	//��گı�ҁA̪��޲݁A��Ă�������گ� �e�ޯĂ���گ�No�ɑΉ�

//�f�o�b�O 2002.09.12 TETSU
u16 PaletteWorkTmp[0x200] = {};			//�W�J�e���|�������[�N
//�f�o�b�O
//
//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
const PalAnmData DmmyPalAnmDat = 
{
	65535,			//id_no
	0, //*PalData		�]������װ��گı��ڽ
	PA_BEFORE,      //type			BEFORE:�װ�����Z�Ώہ@�@AFTER:�װ�����Z��Ώ�
	PA_BG0+PA_COL0, //pal_no		0�`511
	0,              //TransValue	col_no���� �����܂ł���گĂ�ΏۂƂ��邩
	0,              //TransWaitDef	������گı�҂Ɉڂ�܂ł̳���
	0,              //TransValueLoopDef	����]�����J��Ԃ���
	PA_END,         //PalAnmSeq		��ҏI����̏���
	0,             //LoopWaitDef
};




//=========================================================================
//	�p���b�g���[�N�Ɉ��k���ꂽ�p���b�g�f�[�^���Z�b�g
// ����:*SrcAdrs = ���k����Ă����ް����ڽ
// 		pal_no = �W�J����ꏊ  PA_BG0+PA_COL5  PA_OBJ3+PA_COL0 ...��
//		size = �޲Ļ���
//=========================================================================
void DecordPaletteWork(void *SrcAdrs, u16 pal_no, u16 size)
{
#if 0
	DecordWram(SrcAdrs, &PaletteWork[pal_no]);
	CpuCopy(&PaletteWork[pal_no], &PaletteWorkTrans[pal_no], size, 16);
#else
	DecordWram(SrcAdrs, &PaletteWorkTmp[0x0000]);
	CpuCopy(&PaletteWorkTmp[0x0000], &PaletteWork[pal_no], size, 16);
	CpuCopy(&PaletteWorkTmp[0x0000], &PaletteWorkTrans[pal_no], size, 16);
#endif
}
//=========================================================================
//	�p���b�g���[�N�ɔ񈳏k�̃p���b�g�f�[�^���Z�b�g
// ����:*SrcAdrs = ���k����Ă����ް����ڽ
// 		pal_no = �W�J����ꏊ  PA_BG0+PA_COL5  PA_OBJ3+PA_COL0 ...��
//		size = �޲Ļ���
//=========================================================================
void PaletteWorkSet(const u16 *pal_data, u16 pal_no, u16 size)
{
	CpuCopy(pal_data, &PaletteWork[pal_no], size, 16);
	CpuCopy(pal_data, &PaletteWorkTrans[pal_no],size, 16);
}

//=========================================================================
//	�p���b�g���[�N���w��f�[�^�ŃN���A����
// ����: clear_data = ���̃f�[�^�ŗ̈�𖄂߂܂��B
//		pal_no = �ر�����ꏊ  PA_BG0+PA_COL5  PA_OBJ3+PA_COL0 ...��
//		size = �޲Ļ���
//=========================================================================
void PaletteWorkClear(u16 clear_data, u16 pal_no, u16 size)
{
	CpuClear(clear_data, &PaletteWork[pal_no], size, 16);
	CpuClear(clear_data, &PaletteWorkTrans[pal_no], size, 16);
}

/*----------------------------------------------------------*/
/*	V�u�����N���̏���										*/
/*	(V�u�����N���ɏ�ɃR�[�����Ă��������j					*/
/*----------------------------------------------------------*/
void PaletteFadeTrans( void )
{
	if(FadeData.trans_stop == 1)
		return;
	
	DIV_DMACOPY(3, PaletteWorkTrans, PLTT, PLTT_SIZE, 16);
	PalAnmFadeBit = 0;
	if(FadeData.type == 2 && FadeData.fade_sw)
		HardColorRegisterSet();
}

//=========================================================================
//	�p���b�g�A�j��		  ��Ҳ�ٰ�ߓ��ŌĂ�ł�������
// �߂�l: 0=̪���ظ��Ă��������ĂȂ��A����ظ��Ă��I������
//         1=̪��ގ��s��
//         2=̪���ظ��Ă͂������Ă��邪���Ē��Ȃ̂�(̪��ނ�)�����͂��Ȃ�����
//      0xff=�O���ظ����ް����܂��]������Ă��Ȃ�
//=========================================================================
u8 PaletteAnime(void)
{
	u8 temp;
	u32 PalAnmBit = 0;
	
	if(PalAnmFadeBit != 0)
		return 0xff;
	
	if(FadeData.type == 0)
	{
//		PaletteAnimeBeforeAfter(PA_BEFORE, &PalAnmBit);
		temp = ColorAddSub();
//		PaletteAnimeBeforeAfter(PA_AFTER, &PalAnmBit);
	}
	else if(FadeData.type == 1)
	{
//		PaletteAnimeBeforeAfter(PA_BEFORE, &PalAnmBit);
		temp = LightColorAddSub();
//		PaletteAnimeBeforeAfter(PA_AFTER, &PalAnmBit);
	}
	else
	{
//		PaletteAnimeBeforeAfter(PA_BEFORE, &PalAnmBit);
		temp = HardColorCalc();
//		PaletteAnimeBeforeAfter(PA_AFTER, &PalAnmBit);
	}
	
	PalAnmFadeBit = PalAnmBit | FadeData.fade_bit;
	return temp;
}
//----------------------------------------------------------
//	��گı��ܰ�&̪���ظ��ėpܰ��S������
//----------------------------------------------------------
void PaletteAnimeInit(void)
{
	u8 i;
	
	for(i = 0; i < PAL_ANM_MAX; i++)
	{
		PalAnmInitWork(i);
	}
	FadeWorkInit();
}
//----------------------------------------------------------
//	PLTT_VRAM���ް���S��PaletteWork�ɺ�߰
//----------------------------------------------------------
void PlttCopy(void)
{
	u16 i;
	u16 *dest;
	
	dest = (u16 *)PLTT;
	
	for(i = 0; i < 0x200; i++)
	{
		PaletteWork[i] = dest[i];
		PaletteWorkTrans[i] = dest[i];
	}
}

//----------------------------------------------------------
//	�p���b�g�����Z���N�G�X�g  ��ظ��Ă��d�Ȃ����ꍇ�͐揟���ł�
//
// fade_bit : �p���b�g���w��i�e�r�b�g���p���b�g�ԍ��ɑΉ�
//			  �r�b�g0 = BG��0�p���b�g�A Bit16 = OBJ��0�p���b�g)
// wait : �l��傫������Ƃ������̪��ޱ�āA�݂��܂��B
//		  �l������������Ƒ���̪��ޱ�āA�݂��܂��B
// start_evy : �ŏ��̐F�̔Z�x���w�肵�܂�(0�`16)
// 			   0(���̐F)�`2,3..(���ԐF)�`16(�w�肵���F)
// end_evy : ̪��ތv�Z��̍ŏI�I�ȐF�̔Z�x���w�肵�܂��B�����̒l�́��Ɠ����ł�
// next_rgb : �ύX��̐F���w��
//
// �߂�l: 1=����I��  0=�װ
//----------------------------------------------------------
u8 PaletteFadeReq(u32 fade_bit, s8 wait, u8 start_evy, u8 end_evy, u16 next_rgb)
{
	u8 temp;
	
	if(FadeData.fade_sw == 1)
		return 0;
	
	FadeData.fade_value = 2;
	if(wait < 0){	//wait��ϲŽ�̎���value��傫������̪��ނ𑁂�����
		FadeData.fade_value += abs(wait);
		wait = 0;
	}
	
	FadeData.fade_bit = fade_bit;
	FadeData.wait = wait;			//�����ظ��Ă������u�ԂɎ��s
	FadeData.wait_def = wait;
	FadeData.now_evy = start_evy;
	FadeData.end_evy = end_evy;
	FadeData.next_rgb = next_rgb;
	FadeData.fade_sw = 1;
	FadeData.type = 0;
	if(start_evy < end_evy)
		FadeData.direction = 0;
	else
		FadeData.direction = 1;
	
	PaletteAnime();		//add 2001.06.26(��) ظ��Ă��Ă����ɔ��f������悤�ύX
	
	temp = FadeData.trans_stop;
	FadeData.trans_stop = 0;

	//PaletteFadeTrans();	//add 2001.06.26(��)
	CpuCopy(PaletteWorkTrans, PLTT, PLTT_SIZE, 32);
	PalAnmFadeBit = 0;
	if(FadeData.type == 2 && FadeData.fade_sw)
		HardColorRegisterSet();
	
	FadeData.trans_stop = temp;
	
	return 1;
}

u8 PaletteFadeReq2(u32 fade_bit, s8 wait, u8 start_evy, u8 end_evy, u16 next_rgb)
{
	u8 temp;
	
	PlttCopy();
	temp = PaletteFadeReq(fade_bit, wait, start_evy, end_evy, next_rgb);
	return temp;
}

//=========================================================================
//	�p���b�g�A�j�� ���C��
//=========================================================================
void PaletteAnimeBeforeAfter(u8 trans_type, u32 *PalAnmBit)
{
	u8 i;
	PalAnmWorkData *paw;
	
	for(i = 0; i < PAL_ANM_MAX; i++)
	{
		paw = &PalAnmWork[i];

		if(paw->pal_sw == 0 || paw->PalAnmDat->type != trans_type)
			continue;
		
		if(paw->TransValueLoop == paw->PalAnmDat->TransValueLoopDef)
		{
			PaletteAnimeEndCheck(paw);
			if(paw->pal_sw == 0)	//����EndCheck��Del���ꂽ���̂���
				continue;
		}

		if(paw->TransWait == 0)
		{
			PaletteAnimeSub(paw, PalAnmBit);
		}
		else
		{
			paw->TransWait--;
		}
		
		FadeCheck(paw, PalAnmBit);
	}
}

//----------------------------------------------------------
//	�p���b�g�A�j�� ���Ă���������ܰ��֓]��
//----------------------------------------------------------
static void PaletteAnimeSub(PalAnmWorkData *paw, u32 *PalAnmBit)
{
	u8 trans_value = 0;
	u16 now_pal;
	
	now_pal = paw->TransValueLoop * paw->PalAnmDat->TransValue;
	
	if(paw->PalAnmDat->type == PA_BEFORE)
	{
		while(trans_value < paw->PalAnmDat->TransValue)
		{
			PaletteWork[paw->pal_no]
						= paw->PalAnmDat->PalData[now_pal];
			PaletteWorkTrans[paw->pal_no]
						= paw->PalAnmDat->PalData[now_pal];
			trans_value++;
			paw->pal_no++;
			now_pal++;
		}
	}
	else	//PA_AFTER
	{
		while(trans_value < paw->PalAnmDat->TransValue)
		{
			PaletteWorkTrans[paw->pal_no]
						= paw->PalAnmDat->PalData[now_pal];
			trans_value++;
			paw->pal_no++;
			now_pal++;
		}
	}
	
	paw->pal_no = paw->pal_no_def;
	paw->TransWait = paw->PalAnmDat->TransWaitDef;
	
	//if(paw->TransValueLoop < paw->PalAnmDat->TransValueLoopDef)
		paw->TransValueLoop++;
	//else
	if(paw->TransValueLoop >= paw->PalAnmDat->TransValueLoopDef)
	{
		if(paw->LoopWait)
			paw->LoopWait--;
		paw->TransValueLoop = 0;
	}
	
	*PalAnmBit |= GET_PALNO_BIT(paw->pal_no_def);
//	BIT_SET(*PalAnmBit, paw->pal_no_def >> 4);	//�ύX������گĂ��ޯĂ��
}

//----------------------------------------------------------
//	̪��ނ���گı�҂������ɓ����Ƃ��̉����Z�␳
//----------------------------------------------------------
static void FadeCheck(PalAnmWorkData *paw, u32 *PalAnmBit)
{
	u8 i;
	u16 now_pal;
	
	if(FadeData.fade_sw == 0 
		|| ((GET_PALNO_BIT(paw->pal_no_def) & FadeData.fade_bit) == 0))
	{
		return;
	}
	
	if(paw->PalAnmDat->type == PA_BEFORE)
	{
		if(FadeData.wait == FadeData.wait_def)
			return;		//̪��ނŏ���Ɍ��F�����̂�OK
		
		//̪��ނ����ĂŎ��s����Ȃ��̂ł����ł��
		SoftFade(paw->pal_no_def, paw->PalAnmDat->TransValue,
				FadeData.now_evy, FadeData.next_rgb);
	}
	else	//PA_AFTER
	{
		if(FadeData.wait > 0)
			return;		//̪��ނ����s����Ȃ��̂Ō��F����Ȃ�
		
		if(paw->TransWait == paw->PalAnmDat->TransWaitDef)
			return;		//�ް����]�����ꂽ�̂�OK
		
		//̪��ނŌ��F����Ă��܂��Ă���̂œ]�����Ȃ���
		now_pal = paw->TransValueLoop * paw->PalAnmDat->TransValue;
		for(i = 0; i < paw->PalAnmDat->TransValue; i++)
		{
			PaletteWorkTrans[paw->pal_no_def+i] = paw->PalAnmDat->PalData[now_pal+i];
			//now_pal++;
		}
	}
}

//----------------------------------------------------------
//	�S�Ă�ٰ�ߏ������I�������̏���
//----------------------------------------------------------
static void PaletteAnimeEndCheck(PalAnmWorkData *paw)
{
	if(paw->LoopWait == 0)
	{
		switch(paw->PalAnmDat->PalAnmSeq)
		{
			case PA_LOOP:
				paw->TransValueLoop = 0;
				paw->TransWait = paw->PalAnmDat->TransWaitDef;
				paw->LoopWait = paw->PalAnmDat->LoopWaitDef;
				paw->pal_no = paw->pal_no_def;
				break;
			case PA_OUFUKU:
			case PA_END:
				DelPalAnm(paw->PalAnmDat->id_no);
				break;
			default:
		}
	}
	else
	{
		paw->LoopWait--;
	}
}
//----------------------------------------------------------
//	�w�肵��ID�̃p���b�g�A�j�����폜
//----------------------------------------------------------
void DelPalAnm(u16 id_no)
{
	u8 i;
	
	if((i = PalAnmGetWorkNumber(id_no)) == PAL_ANM_MAX)
		return;		//error
	PalAnmInitWork(i);
}
//----------------------------------------------------------
//	�p���b�g�A�j���P��������
//----------------------------------------------------------
void PalAnmInitWork(u8 pal_anm_no)
{
	PalAnmWork[pal_anm_no].PalAnmDat = (PalAnmData *)&DmmyPalAnmDat;
	PalAnmWork[pal_anm_no].pal_sw = 0;
	PalAnmWork[pal_anm_no].pal_no_def = 0;
	PalAnmWork[pal_anm_no].pal_no = 0;
	PalAnmWork[pal_anm_no].TransValueLoop = 0;
	PalAnmWork[pal_anm_no].pause = 0;
	PalAnmWork[pal_anm_no].TransWait = 0;
	PalAnmWork[pal_anm_no].LoopWait = 0;
}
//----------------------------------------------------------
//	̪���ظ���ܰ�������
//----------------------------------------------------------
void FadeWorkInit(void)
{
	FadeData.fade_bit = 0;
	FadeData.wait_def = 0;
	FadeData.wait = 0;
	FadeData.now_evy = 0;
	FadeData.end_evy = 0;
	FadeData.next_rgb = 0;
	FadeData.fade_sw = 0;
	FadeData.direction = 0;
	FadeData.trans_stop = 0;
	FadeData.reset_flg = 0;
	FadeData.hard_end = 0;
	FadeData.fade_end = 0;
	FadeData.end_wait = 0;
	FadeData.bg_obj = 0;
	FadeData.fade_value = 2;
}
//----------------------------------------------------------
//	�w�肵��ID�̃p���b�g�A�j�����~�߂�
// �o�^����Ă���PalAnmWork�̔z��ԍ����������Ă���Ȃ璼�ڂ���������������
//----------------------------------------------------------
void PalAnmPause(u16 id_no)
{
	u8 i;
	
	if((i = PalAnmGetWorkNumber(id_no)) == PAL_ANM_MAX)
		return;		//error
	PalAnmWork[i].pause = 1;
}
//----------------------------------------------------------
//	�w�肵��ID��Pause�Ŏ~�߂Ă�����گı�҂𓮂���
// �o�^����Ă���PalAnmWork�̔z��ԍ����������Ă���Ȃ璼�ڂ���������������
//----------------------------------------------------------
void PalAnmMove(u16 id_no)
{
	u8 i;
	
	if((i = PalAnmGetWorkNumber(id_no)) == PAL_ANM_MAX)
		return;		//error
	
	PalAnmWork[i].pause = 0;
}
//----------------------------------------------------------
//	ID����PalAnmWork�̔z��ԍ����o��
//----------------------------------------------------------
u8 PalAnmGetWorkNumber(u16 id_no)
{
	u8 i;
	
	for(i = 0; i < PAL_ANM_MAX; i++)
	{
		if(PalAnmWork[i].PalAnmDat->id_no == id_no)
			return i;
	}
	return PAL_ANM_MAX;	//error
}


//----------------------------------------------------------
//	�\�t�g�t�F�[�h�C���A�A�E�g(��گı��ٰ�ݗp)
// �߂�l: 0=̪���ظ��Ă��������ĂȂ��A����ظ��Ă��I������
//         1=̪��ގ��s��
//         2=̪���ظ��Ă͂������Ă��邪���Ē��Ȃ̂ŏ����͂��Ȃ�����
//----------------------------------------------------------
#include "softfade.h"
u8 ColorAddSub(void)
{
	u16 offset2;
	u16 next_rgb;
	u32 fade_bit;
	s8  evy;
	
	if(FadeData.fade_sw == 0)
		return 0;
	if(FadeEndWaitCheck())
		return FadeData.fade_sw;
	
	if(FadeData.bg_obj == 0){
		if(FadeData.wait < FadeData.wait_def)
		{
			FadeData.wait++;
			return 2;
		}
		else
			FadeData.wait = 0;
	}

	offset2 = 0;
	next_rgb = FadeData.next_rgb;

	if(FadeData.bg_obj == 0)
		fade_bit = FadeData.fade_bit & 0xffff;
	else{
		fade_bit = FadeData.fade_bit >> 16;
		offset2 = 16*16;
	}
	while(fade_bit)
	{
		if(fade_bit & 1)
			SoftFade(offset2, 16, FadeData.now_evy, FadeData.next_rgb);
		fade_bit >>= 1;
		offset2 += 16;
	}
	FadeData.bg_obj ^= 1;
	if(FadeData.bg_obj)
		return FadeData.fade_sw;

	if(FadeData.now_evy == FadeData.end_evy)
	{
		FadeData.fade_bit = 0;
		//FadeData.fade_sw = 0;
		FadeData.fade_end = 1;
	}
	else if(FadeData.direction == 0)
	{
		evy = FadeData.now_evy;	//31�𒴂���Ƶ��ް�۰���N�����̂�
		evy += FadeData.fade_value;
		if(evy > FadeData.end_evy)
			evy = FadeData.end_evy;
		FadeData.now_evy = evy;
	}
	else
	{
		evy = FadeData.now_evy;	//ϲŽ����邽��
		evy -= FadeData.fade_value;
		if(evy < FadeData.end_evy)
			evy = FadeData.end_evy;
		FadeData.now_evy = evy;
	}
	return FadeData.fade_sw;
}







//=========================================================================
//	�J���[���]						2001.06.26(��)
// ����:�@bit = �J���[���]����Ώ���گ�   0bit:BGPalette0   16bit:ObjPalette0
//=========================================================================
void ColorReversal(u32 bit)
{
	u16 i = 0;
	u8 j;
	
	while(bit)
	{
		if(bit & 1)
		{
			for(j = 0; j < 16; j++)
				PaletteWorkTrans[i + j] ^= 0xffff;
		}
		bit >>= 1;
		i += 16;
	}
}

//=========================================================================
//	�J���[���Z						2001.06.29
// ����:�@bit = �J���[���Z����Ώ���گ�   0bit:BGPalette0   16bit:ObjPalette0
//=========================================================================
void ColorRandomPlus(u32 bit,s8 red,s8 green, s8 blue)
{
	u16 i = 0;
	u8 j;
	PlttData *p;
	
	while(bit)
	{
		if(bit & 1)
		{
			for(j = 0; j < 16; j++){
				p = (PlttData*)&(PaletteWorkTrans[i + j]);
				p->Red+=red;
				p->Green+=green;
				p->Blue+=blue;
			}
		}
		bit >>= 1;
		i += 16;
	}
}

//=========================================================================
//	�J���[���Z�̌�n��					2001.06.29
// ����:�@bit = �߂��Ώ���گ�   0bit:BGPalette0   16bit:ObjPalette0
//=========================================================================
void ColorRandomPlusEnd(u32 bit)
{
	u16 i = 0;
	u8 j;
	
	while(bit)
	{
		if(bit & 1)
		{
			for(j = 0; j < 16; j++)
				PaletteWorkTrans[i + j] = PaletteWork[i+j];
		}
		bit >>= 1;
		i += 16;
	}
}


//=========================================================================
//	���Ɣ��݂̂ɗp�r�����ڂ��������t�F�[�h�֐�
// fade_type = �ǂ�̪��ނ��s����
//			   W_to_N:����ɰ��  N_to_W:ɰ�ف���  B_to_N:����ɰ��  N_to_B:ɰ�ف���
//=========================================================================
void LightFadeReq(u8 fade_type)
{
	FadeData.fade_value = 2;
	LightFadeCommon(fade_type);
}

//----------------------------------------------------------
//	LightFadeReq, LightFadeReqSync���ʏ���
//----------------------------------------------------------
static void LightFadeCommon(u8 fade_type)
{
	FadeData.now_evy = 31;
	FadeData.wait_def = fade_type;
	FadeData.fade_sw = 1;
	FadeData.type = 1;
	
	if(fade_type == B_to_N)
		CpuArrayClear(0, PaletteWorkTrans, 16)
	if(fade_type == W_to_N)
		CpuArrayClear(0x7fff, PaletteWorkTrans, 16);
	PaletteAnime();		//ظ��Ă��Ă����ɔ��f������悤�ύX
}

//----------------------------------------------------------
//	�\�t�g�t�F�[�h�C���A�A�E�g(��گı��ٰ�ݗp)
// �߂�l: 0=̪���ظ��Ă��������ĂȂ��A����ظ��Ă��I������
//         1=̪��ގ��s��
//----------------------------------------------------------

u8 LightColorAddSub(void)
{
	u16 i;
	u16 start_pal, end_pal;
	s8 red,green,blue,t_red,t_green,t_blue;
	
	if(FadeData.fade_sw == 0)
		return 0;
	if(FadeEndWaitCheck())
		return FadeData.fade_sw;

	if(FadeData.bg_obj){
		start_pal = 0x100;
		end_pal = 0x200;
	}else{
		start_pal = 0;
		end_pal = 0x100;
	}

	switch(FadeData.wait_def)
	{
		case W_to_N:
			for(i = start_pal; i < end_pal; i++)
			{
				red = ((PlttData*)&PaletteWork[i])->Red;
				green = ((PlttData*)&PaletteWork[i])->Green;
				blue = ((PlttData*)&PaletteWork[i])->Blue;
				t_red = ((PlttData*)&PaletteWorkTrans[i])->Red-2;
				t_green = ((PlttData*)&PaletteWorkTrans[i])->Green-2;
				t_blue = ((PlttData*)&PaletteWorkTrans[i])->Blue-2;
				if(t_red < red)
					t_red = red;
				if(t_green < green)
					t_green = green;
				if(t_blue < blue)
					t_blue = blue;
				PaletteWorkTrans[i] = t_red | (t_green<<5) | (t_blue<<10);
			}
			break;
		case N_to_W:
			for(i = start_pal; i < end_pal; i++)
			{
				t_red = ((PlttData*)&PaletteWorkTrans[i])->Red+2;
				t_green = ((PlttData*)&PaletteWorkTrans[i])->Green+2;
				t_blue = ((PlttData*)&PaletteWorkTrans[i])->Blue+2;
				if(t_red > 31)
					t_red = 31;
				if(t_green > 31)
					t_green = 31;
				if(t_blue > 31)
					t_blue = 31;
				PaletteWorkTrans[i] = t_red | (t_green<<5) | (t_blue<<10);
			}
			break;
		case B_to_N:
			for(i = start_pal; i < end_pal; i++)
			{
				red = ((PlttData*)&PaletteWork[i])->Red;
				green = ((PlttData*)&PaletteWork[i])->Green;
				blue = ((PlttData*)&PaletteWork[i])->Blue;
				t_red = ((PlttData*)&PaletteWorkTrans[i])->Red+2;
				t_green = ((PlttData*)&PaletteWorkTrans[i])->Green+2;
				t_blue = ((PlttData*)&PaletteWorkTrans[i])->Blue+2;
				if(t_red > red)
					t_red = red;
				if(t_green > green)
					t_green = green;
				if(t_blue > blue)
					t_blue = blue;
				PaletteWorkTrans[i] = t_red | (t_green<<5) | (t_blue<<10);
			}
			break;
		case N_to_B:
			for(i = start_pal; i < end_pal; i++)
			{
				t_red = ((PlttData*)&PaletteWorkTrans[i])->Red-2;
				t_green = ((PlttData*)&PaletteWorkTrans[i])->Green-2;
				t_blue = ((PlttData*)&PaletteWorkTrans[i])->Blue-2;
				if(t_red < 0)
					t_red = 0;
				if(t_green < 0)
					t_green = 0;
				if(t_blue < 0)
					t_blue = 0;
				PaletteWorkTrans[i] = t_red | (t_green<<5) | (t_blue<<10);
			}
			break;
	}
	
	FadeData.bg_obj ^= 1;
	if(FadeData.bg_obj)
		return FadeData.fade_sw;

	if(FadeData.now_evy - FadeData.fade_value < 0)
		FadeData.now_evy = 0;
	else
		FadeData.now_evy -= FadeData.fade_value;
	if(FadeData.now_evy == 0)
	{
		switch(FadeData.wait_def)
		{	//�����؂�̂Ẳe���Ŋ��S�ɂ��̐F�ɂ͂Ȃ�Ȃ��̂�
			case W_to_N:
			case B_to_N:
				CpuArrayCopy(PaletteWork, PaletteWorkTrans, 32);
				break;
			case N_to_W:
				CpuArrayClear(0xffffffff, PaletteWorkTrans, 32);
				break;
			case N_to_B:
				CpuArrayClear(0, PaletteWorkTrans, 32);
				break;
		}
		FadeData.type = 0;
		//FadeData.fade_sw = 0;
		FadeData.fade_end = 1;
	}

	return FadeData.fade_sw;
}








//----------------------------------------------------------
//	�n�[�h�ɂ��p���b�g�����Z���N�G�X�g
//
// fade_bit : �����Z�̑Ώۂɂ���BG,OBJ���w��(BLDCNTڼ޽��̑�1�Ώ��߸�قɐݒ肷���ޯ�)
//				6,7bit:���x�ύX���߁A�޳��ޯ�(10:UP	 11:DOWN)
//				5bit:BD(�ޯ���׳���), 4bit:OBJ, 3:BG3, 2:BG2, 1:BG1, 0:BG0
//
// wait : �l��傫������Ƃ������̪��ޱ�āA�݂��܂� (MAX:63)
// start_evy : �ŏ��̐F�̔Z�x���w�肵�܂�(0�`16)
// 			   0(���̐F)�`2,3..(���ԐF)�`16(���A�������͔�)
// end_evy : �ŏI�I�ȐF�̔Z�x���w�肵�܂��B�����̒l�́��Ɠ����ł�
// reset_flg : ̪��ޏI������ڼ޽���̏����������邩(0:���Ȃ�  1:����)
//----------------------------------------------------------
void HardFadeReq(u8 fade_bit, u8 wait, u8 start_evy, u8 end_evy, u8 reset_flg)
{
	FadeData.fade_bit = fade_bit;
	FadeData.wait = wait;			//�����ظ��Ă������u�ԂɎ��s
	FadeData.wait_def = wait;
	FadeData.now_evy = start_evy;
	FadeData.end_evy = end_evy;
	FadeData.fade_sw = 1;
	FadeData.type = 2;				//RegisterFade
	FadeData.reset_flg = reset_flg;
	FadeData.hard_end = 0;
	if(start_evy < end_evy)
		FadeData.direction = 0;
	else
		FadeData.direction = 1;
}

//----------------------------------------------------------
//	���W�X�^�[�t�F�[�h�C���A�A�E�g(��گı��ٰ�ݗp)
// �߂�l: 0=̪���ظ��Ă��������ĂȂ��A����ظ��Ă��I������
//         1=̪��ގ��s��
//         2=̪���ظ��Ă͂������Ă��邪���Ē��Ȃ̂ŏ����͂��Ȃ�����
//----------------------------------------------------------
static u8 HardColorCalc(void)
{
	s8  evy;
	
	if(FadeData.fade_sw == 0)
		return 0;

	if(FadeData.wait < FadeData.wait_def)
	{
		FadeData.wait++;
		return 2;
	}
	else
		FadeData.wait = 0;

	if(FadeData.direction == 0){
		FadeData.now_evy++;
		if(FadeData.now_evy > FadeData.end_evy){
			FadeData.hard_end++;
			FadeData.now_evy--;
		}
	}
	else
	{
		evy = FadeData.now_evy;	//ϲŽ����邽��
		FadeData.now_evy--;
		evy--;
		if(evy < FadeData.end_evy){
			FadeData.hard_end++;
			FadeData.now_evy++;
		}
	}
	
	if(FadeData.hard_end){
		//FadeData.fade_sw = 0;		HardColorRegisterSet�֐��̒��ŗ��Ƃ��̂�
		//FadeData.fade_bit = 0;
		if(FadeData.reset_flg){
			FadeData.fade_bit = 0;
			FadeData.now_evy = 0;
		}
		FadeData.reset_flg = 0;
	}
	return FadeData.fade_sw;
}

static void HardColorRegisterSet(void)
{
	*(vu16 *)REG_BLDCNT = FadeData.fade_bit;
	*(vu16 *)REG_BLDY = FadeData.now_evy;
	if(FadeData.hard_end){
		FadeData.hard_end = 0;
		FadeData.type = 0;
		FadeData.fade_bit = 0;
		FadeData.now_evy = 0;
		FadeData.fade_sw = 0;
	}
}





//=========================================================================
//	̪��ޏI����A���Sync���Ă�����
//	�߂�l: 0=�܂�̪��ޏI�����ĂȂ�		1=���Ē�
//=========================================================================
static u8 FadeEndWaitCheck(void)
{
	if(FadeData.fade_end){
		if(FadeData.end_wait == FADE_END_WAIT){
			FadeData.fade_sw = 0;
			FadeData.fade_end = 0;
			FadeData.end_wait = 0;
		}else
			FadeData.end_wait++;
		return 1;
	}
	return 0;
}




//=========================================================================
//	�w��r�b�g�̃J���[�p���b�g���Ă΂ꂽ�u�ԂɎw��F�A�w��Z�x�ɕύX����
//
// fade_bit : �p���b�g���w��i�e�r�b�g���p���b�g�ԍ��ɑΉ�
//			  �r�b�g0 = BG��0�p���b�g�A Bit16 = OBJ��0�p���b�g)
// start_evy : �ύX��̐F�̔Z�x���w�肵�܂�(0�`16)
// 			   0(���̐F)�`2,3..(���ԐF)�`16(�w�肵���F)
// next_rgb : �ύX��̐F���w��
//=========================================================================
void ColorConceChange(u32 fade_bit, u8 evy, u16 next_rgb)
{
	u16 offset = 0;
	
	while(fade_bit){
		if(fade_bit & 1)
			SoftFade(offset, 16, evy, next_rgb);
		fade_bit >>= 1;
		offset += 16;
	}
}



//=========================================================================
//	�w��r�b�g�̃J���[�p���b�g���Ă΂ꂽ�u�ԂɎw��F�A�w��Z�x�ɕύX����
// ���w�肳��Ă��Ȃ��ޯĂ���گĂ͌��̐F�ɖ߂�܂��B
// fade_bit : �p���b�g���w��i�e�r�b�g���p���b�g�ԍ��ɑΉ�
//			  �r�b�g0 = BG��0�p���b�g�A Bit16 = OBJ��0�p���b�g)
// start_evy : �ύX��̐F�̔Z�x���w�肵�܂�(0�`16)
// 			   0(���̐F)�`2,3..(���ԐF)�`16(�w�肵���F)
// next_rgb : �ύX��̐F���w��
//=========================================================================
void ColorConceChange2(u32 fade_bit, u8 evy, u16 next_rgb)
{
	DIV_DMACOPY(3, PaletteWork, PaletteWorkTrans, 0x400, 32);
	ColorConceChange(fade_bit, evy, next_rgb);
}

