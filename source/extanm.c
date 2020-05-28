//=========================================================================
//
//		�A�N�^�[�O�A�j���֐�
//											2001.07.04(��)	by matsuda
//=========================================================================

#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "madefine.h"
#include "extanm.h"

//=========================================================================
//	�萔
//=========================================================================


//=========================================================================
//	�f�[�^
//=========================================================================
//----------------------------------------------------------
//	oam�̌`��ƻ��ނ���G���޲Ļ��ނ��o��
//   CharByteSizeTbl[shape][size]
//----------------------------------------------------------
static const u16 CharByteSizeTbl[3][4] = 
{
	//shape 0
	{
		OBJ_SIZE16_8x8,		//size 0
		OBJ_SIZE16_16x16,	//size 1
		OBJ_SIZE16_32x32,	//size 2
		OBJ_SIZE16_64x64,	//size 3
	},
	//shape 1
	{
		OBJ_SIZE16_16x8,	//size 0
		OBJ_SIZE16_32x8,	//size 1
		OBJ_SIZE16_32x16,	//size 2
		OBJ_SIZE16_64x32,	//size 3
	},
	//shape 2
	{
		OBJ_SIZE16_8x16,	//size 0
		OBJ_SIZE16_8x32,	//size 1
		OBJ_SIZE16_16x32,	//size 2
		OBJ_SIZE16_32x64,	//size 3
	},
};


//=========================================================================
//	�v���O����
//=========================================================================
/*
	u8 		anm_no;							// �������
	u8 		anm_offs;						// ������޵̾��

	u8 		anm_wait:6;						// ��ҳ���
	u8		anm_pause:1;					// ����߰��
	u8		aff_pause:1;					// �̨ݱ���߰��
	u8		anm_count;						// ���ٰ��
	u16		char_num;						// �̾�ķ������

*/
//----------------------------------------------------------
//	�A�j�����s	�A�j��������ꍇ�͓���֐��̒��ŌĂё����Ă�������
//�@�߂�l: 0=wait��	1=1����݂ɱ�҂��؂�ւ����  2=2����ݱ�҂ɐ؂�ւ���� 3=����
//----------------------------------------------------------
//ExteriorAnm
extern u8 pokeic2_Character[];
u8 ExtAnm(actWork *Xreg)
{
	u16 offset_size;
	u8 *data_adr;
	u8 change_flg = 0;
	
	if(Xreg->anm_wait == 0)
	{
		switch(Xreg->anm_tbl[Xreg->anm_no][Xreg->anm_offs].pat)
		{
			case ACT_ANMEND:
				break;
				
			case ACT_ANMLOOP:
				Xreg->anm_offs = 0;
				break;
			
			default:
				offset_size = CharByteSizeTbl[Xreg->oamData.Shape][Xreg->oamData.Size];
				data_adr = ((u8*)Xreg->anm_trans) + 
					offset_size * Xreg->anm_tbl[Xreg->anm_no][Xreg->anm_offs].pat;
				DmaTransReq(data_adr, (Xreg->oamData.CharNo << 5) + OBJ_MODE0_VRAM, offset_size);
			
				Xreg->anm_wait = Xreg->anm_tbl[Xreg->anm_no][Xreg->anm_offs].wait;
				Xreg->anm_offs++;
				change_flg = Xreg->anm_offs;
		}
	}
	else
		Xreg->anm_wait--;
	
	return change_flg;
}


//----------------------------------------------------------
//	ExtAnm���g�p����ꍇ�́A���̊֐���AddActor����
//----------------------------------------------------------
u8 ExtAddActor(ExtActHeader *eah, s16 x, s16 y, u8 pri)
{
	u8 actno;
	actAnmData TempTransTbl ={		//AddActor�������ɗ̈���m�ۂ��邽��
		0, 	CharByteSizeTbl[eah->oamData->Shape][eah->oamData->Size]//�ꎞ�I��ð��ق��쐬
	};
	actHeader acth = 	//�߹�ݱ��ݗp��ͯ�ް��RAM��Ɍ`��
	{
		ACT_TMODE,
		eah->pal_id,
		eah->oamData,
		eah->anm_tbl,
		&TempTransTbl,
		eah->aff_tbl,
		eah->move,
	};
	actno = AddActor(&acth, x, y, pri);
	
//	ActWork[actno].feature_flg = 1;		//�ؼ��ٱ��ٰ�݂��g�p�����
	ActWork[actno].anm_pause = 1;
	ActWork[actno].anm_sw = 0;			//��ҕύX�׸�OFF
	ActWork[actno].anm_trans = (void*)eah->DataAdrs;
//	ActWork[actno].char_num = ActWork[actno].oamData.CharNo;

	return actno;
}

//----------------------------------------------------------
//	ExtAnm���g�p���Ă��鱸����Del����
//----------------------------------------------------------
void ExtDelActor(actWork *Xreg)
{
	actAnmData TempTransTbl ={		//AddActor�������Ɋm�ۂ����̈���J�����邽��
		0, 	CharByteSizeTbl[Xreg->oamData.Shape][Xreg->oamData.Size]//�ꎞ�I��ð��ق��쐬
	};
	
	Xreg->anm_trans = &TempTransTbl;
	DelActor(Xreg);
}

//----------------------------------------------------------
//	�A�j���e�[�u����ύX
//----------------------------------------------------------
void ExtAnmChg(actWork *xreg, u8 anmNo)
{
	xreg->anm_no = anmNo;
	xreg->anm_wait = 0;
	xreg->anm_offs = 0;
}

