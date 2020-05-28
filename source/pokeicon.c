//=========================================================================
//
//		�|�P�����A�C�R��
//														by matsuda
//=========================================================================

#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "pokeicon.h"
#include "extanm.h"
#include "madefine.h"
#include "decord.h"
#include "monsno.def"
#include "pm2fenum.h"

//=========================================================================
//	�萔
//=========================================================================
#define ICON_PAL_MAX	6

//���ޯ��p
#define NEW_POKE		1		//0:�����ς����݂������A 1:ýčX�V�p

#if (NEW_POKE == 0)
	#define POKE_GRA_MAX 40		//�p�ӂ���Ă���|�P�����̊G
#else
	#define POKE_GRA_MAX 15
#endif

#define HATENA_ICON_NO	260		//�u�H�v���݂�ð���No

//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
void PokeIconDmmyMove(actWork *Xreg);
//static u8 PokeGraAruNasiCheck(u16 pokeno);
u32 GetPokeIconCharAdrs(u16 pokeno, u32 rnd);
u16 GetAnnoonNo(u32 personal_rnd);
void PokeIconPaletteDel(u16 chrid);
void PokeIconPaletteSetCharID(u16 chrid);
u8 PokeIconSetCharID(u16 chrid, pActFunc move, s16 x, s16 y, u8 pri);

//=========================================================================
//	�f�[�^
//=========================================================================
//-- New 2002.04.25(��) --//
#include "..\scaddata\p_icon.acl"

#include "pokeicon.dat"

const PalData PokeIconPalData[] = 
{
/*
#if (NEW_POKE == 0)
	{&pokeicon_Palette[16*0], PALID_POKEICON_0},
	{&pokeicon_Palette[16*1], PALID_POKEICON_1},
	{&pokeicon_Palette[16*2], PALID_POKEICON_2},
	{&pokeicon_Palette[16*3], PALID_POKEICON_3},
	{&pokeicon_Palette[16*4], PALID_POKEICON_4},
	{&pokeicon_Palette[16*5], PALID_POKEICON_5},
#else
	{&p_icon_1_Palette[16*0], PALID_POKEICON_0},
	{&p_icon_1_Palette[16*1], PALID_POKEICON_1},
	{&p_icon_1_Palette[16*2], PALID_POKEICON_2},
	{&p_icon_1_Palette[16*3], PALID_POKEICON_3},
	{&p_icon_1_Palette[16*4], PALID_POKEICON_4},
	{&p_icon_1_Palette[16*5], PALID_POKEICON_5},
#endif
*/
	{&p_icon_Palette[16*0],	PALID_POKEICON_0},
	{&p_icon_Palette[16*1],	PALID_POKEICON_1},
	{&p_icon_Palette[16*2],	PALID_POKEICON_2},
	{&p_icon_Palette[16*3],	PALID_POKEICON_3},
	{&p_icon_Palette[16*4],	PALID_POKEICON_4},
	{&p_icon_Palette[16*5],	PALID_POKEICON_5},
};

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
static const ActOamData PokeIconOamDat = {
	0,		//u32 VPos:8;             // �x���W
	0,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	0,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
	2,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	1,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
static const actAnm PokeIcon_anm0[] =	//HP���^��
{
	{0, 6, 0, 0},		//�������݁A ���āAHflip�AVflip
	{1, 6, 0, 0},		//�������݁A ���āAHflip�AVflip
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm PokeIcon_anm1[] =	//�ιް��
{
	{0, 8, 0, 0},		//�������݁A ���āAHflip�AVflip
	{1, 8, 0, 0},		//�������݁A ���āAHflip�AVflip
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm PokeIcon_anm2[] =	//���F�ް��
{
	{0, 14, 0, 0},		//�������݁A ���āAHflip�AVflip
	{1, 14, 0, 0},		//�������݁A ���āAHflip�AVflip
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm PokeIcon_anm3[] =	//�ԐF�ް��
{
	{0, 22, 0, 0},		//�������݁A ���āAHflip�AVflip
	{1, 22, 0, 0},		//�������݁A ���āAHflip�AVflip
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm PokeIcon_anm4[] =	//�C��
{
	{0, 29, 0, 0},		//�������݁A ���āAHflip�AVflip
	{0, 29, 0, 0},		//�������݁A ���āAHflip�AVflip
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm * const PokeIconAnmTbl[] = 
{
	PokeIcon_anm0,
	PokeIcon_anm1,
	PokeIcon_anm2,
	PokeIcon_anm3,
	PokeIcon_anm4,
};

//add nohara 01/07/26
static const actAffAnm PokeIcon_aff_anm0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0, 0, 0, 10 },					// �������Ȃ�
	{ACT_AFEND,0,0,0},					// ٰ�� , �߂��
};
static const actAffAnm PokeIcon_aff_anm1[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	//{ -1, -1, 0, 255 },				// 
	{ -2, -2, 0, 122 },					// �������Ȃ�
	{ACT_AFEND,0,0,0},					// ٰ�� , �߂��
};
static const	actAffAnm * const PokeIcon_aff_tbl[] = {
	PokeIcon_aff_anm0,
	PokeIcon_aff_anm1,
};

//=========================================================================
//	�v���O����
//=========================================================================

//----------------------------------------------------------
//	�|�P�����̃A�C�R�����Z�b�g����
// ����: pokeno = ��Ă����߹�݂̊J�����ް
// �߂�l: ��Ă������݂�actno
//----------------------------------------------------------
u8 PokeIconSet(u16 pokeno, pActFunc move, s16 x, s16 y, u8 pri)
{
	u8 actno;
	ExtActHeader acth = 
	{
		(ActOamData*)&PokeIconOamDat,
	/*
	#if (NEW_POKE == 0)
		(u8 *)&pokeic2_Character[
			(pokeno > POKE_GRA_MAX ? (0) : (pokeno-1)) * OBJ_SIZE16_32x32],
	#else
		(u8 *)&p_icon_1_Character[(pokeno-1) * OBJ_SIZE16_32x32],
	#endif
	*/
		(u8*)IconCharAdrs[pokeno],
		PokeIconAnmTbl,
		//DummyActAffTbl,
		PokeIcon_aff_tbl,	//add nohara 01/07/26
		move,
//		PokeIconPalData[
//			(pokeno > POKE_GRA_MAX ? (1) : (PokeIconPalette[pokeno-1]))].reg_num,
		PALID_POKEICON_0 + IconPalAtr[pokeno],
	};
	
	//-- ���ޯ��p --//
//	if(PokeGraAruNasiCheck(pokeno) == 0)
//		acth.DataAdrs = (void*)quesicon_Character;
	
	actno = ExtAddActor(&acth, x, y, pri);
	ExtAnm(&ActWork[actno]);		//����̱�ҊG�]��
	return actno;
}

#if 0
//-- ���ޯ��p���[�`�� --//
static const u16 AruPokemon[] = 
{1,2,3,6,9,12,15, 0xffff}; 
static u8 PokeGraAruNasiCheck(u16 pokeno)
{
	u16 i;
	
	for(i = 0; ; i++)
	{
		if(AruPokemon[i] == pokeno)
			return 1;
		if(AruPokemon[i] == 0xffff)
			return 0;
	}
}
#endif

//----------------------------------------------------------
//	�|�P�����̃A�C�R�����Z�b�g����(�A���m�[���Ή��ް�ޮ�)
// ����: pokeno = ��Ă����߹�݂̊J�����ް
// �߂�l: ��Ă������݂�actno
//----------------------------------------------------------
u8 PokeIconSet2(u16 pokeno, pActFunc move, s16 x, s16 y, u8 pri, u32 rnd)
{
	u8 actno;
	ExtActHeader acth = 
	{
		(ActOamData*)&PokeIconOamDat,
		(u8*)GetPokeIconCharAdrs(pokeno,rnd),
		PokeIconAnmTbl,
		PokeIcon_aff_tbl,	//add nohara 01/07/26
		move,
		PALID_POKEICON_0 + IconPalAtr[pokeno],
	};

	if(pokeno > MONSNO_END)	//���m��MonsNo����
		acth.pal_id = PALID_POKEICON_0 + 0;	//0�Ԃ���گĂ��w��
	
	actno = ExtAddActor(&acth, x, y, pri);
	ExtAnm(&ActWork[actno]);		//����̱�ҊG�]��
	return actno;
}

//----------------------------------------------------------
//	�|�P�����̃A�C�R�����Z�b�g����(�A���m�[���Ή��ް�ޮ�)
//	�߹��No�ł͂Ȃ��A�A�C�R���̷��ID���w�肵�ľ�Ă���
// ����: chrid = ��Ă����߹�ݱ��̷݂��ID
// �߂�l: ��Ă������݂�actno
//----------------------------------------------------------
u8 PokeIconSetCharID(u16 chrid, pActFunc move, s16 x, s16 y, u8 pri)
{
	u8 actno;
	ExtActHeader acth = 
	{
		(ActOamData*)&PokeIconOamDat,
		(u8*)IconCharAdrs[chrid],
		PokeIconAnmTbl,
		PokeIcon_aff_tbl,	//add nohara 01/07/26
		move,
		PALID_POKEICON_0 + IconPalAtr[chrid],
	};

	actno = ExtAddActor(&acth, x, y, pri);
	ExtAnm(&ActWork[actno]);		//����̱�ҊG�]��
	return actno;
}

//----------------------------------------------------------
//	�|�P�����A�C�R���̷��ð��ق̓Y�������擾
//----------------------------------------------------------
u16 GetPokeIconCharID(u16 pokeno, u32 rnd)
{
	u16	annon_rnd, adrs_no;
	
	if(pokeno==MONSNO_ANNOON){
		annon_rnd = GetAnnoonNo(rnd);
		if(annon_rnd==0)
			annon_rnd+=CELLID_pm3f_una;
		else
			annon_rnd+=(CELLID_pm3f_unb-1);
		adrs_no = annon_rnd;
	}
	else{
		if(pokeno > MONSNO_END)	//���m��MonsNo����
			adrs_no = HATENA_ICON_NO;	//�u�H�v�̃L����
		else
			adrs_no = pokeno;
	}
	return adrs_no;
}

//-- �A���m�[���̃A���t�@�x�b�g�ԍ����擾 --//
u16 GetAnnoonNo(u32 personal_rnd)
{
	u16 annon_rnd;
	
	annon_rnd=(((personal_rnd&0x03000000)>>18)|((personal_rnd&0x030000)>>12)|((personal_rnd&0x0300)>>6)|(personal_rnd&0x03)) % ANNOON_IDMAX;
	return annon_rnd;
}

//----------------------------------------------------------
//	���[����hold_monsno�𱲺̷݂��ID�ɕϊ�����(��ɰݏ�������)
//----------------------------------------------------------
u16 MailHoldMonsNo_IconCharIDConv(u16 hold_monsno)
{
	u16 monsno, annon_id;
	u16 chrid;
	
	monsno = GetMailHoldMonsNo(hold_monsno, &annon_id);
	if(monsno == MONSNO_ANNOON){
		if(annon_id == 0)
			annon_id += CELLID_pm3f_una;
		else
			annon_id += (CELLID_pm3f_unb-1);
		chrid = annon_id;
	}
	else{
		//chrid = hold_monsno;
		chrid = GetPokeIconCharID(hold_monsno, 0);	//��ɰ݂ł͂Ȃ��̂Ō������͕s�v
	}
	return chrid;
}

//----------------------------------------------------------
//	�|�P�������݂̃L�������ڽ���擾
//----------------------------------------------------------
u32 GetPokeIconCharAdrs(u16 pokeno, u32 rnd)
{
	u16 chrid;
	
	chrid = GetPokeIconCharID(pokeno, rnd);
	return IconCharAdrs[chrid];
}

//----------------------------------------------------------
//	�|�P�����̃A�C�R���̃A�N�^�[��Del����
//----------------------------------------------------------
void PokeIconDel(actWork *Xreg)
{
	ExtDelActor(Xreg);
}

//----------------------------------------------------------
//	�߹�ݱ��݂Ŏg�p������گĂ�S�ľ��
//----------------------------------------------------------
void PokeIconPaletteAllSet(void)
{
	u8 i;
	
	for(i = 0; i < ICON_PAL_MAX; i++)
		ObjPalSet((PalData*)&PokeIconPalData[i]);
}

//----------------------------------------------------------
//	�w���߹�݂̱��݂���گĂ��
// pokeno = �J�����ް
//----------------------------------------------------------
void PokeIconPaletteSet(u16 pokeno)
{
	u8 tblno;

	if(pokeno > MONSNO_END)	//���m��MonsNo����
		pokeno = HATENA_ICON_NO;	//�u�H�v�̃L����
	
	tblno = IconPalAtr[pokeno];
	if(ObjPalNumGet(PokeIconPalData[tblno].reg_num) == 0xff)	//��d�o�^����
		ObjPalSet((PalData*)&PokeIconPalData[tblno]);
}

//----------------------------------------------------------
//	�w���߹�݂̱��݂���گĂ��
// chrid = ���̷݂��ID
//----------------------------------------------------------
void PokeIconPaletteSetCharID(u16 chrid)
{
	u8 tblno;
	
	tblno = IconPalAtr[chrid];
	if(ObjPalNumGet(PokeIconPalData[tblno].reg_num) == 0xff)	//��d�o�^����
		ObjPalSet((PalData*)&PokeIconPalData[tblno]);
}

//----------------------------------------------------------
//	�߹�ݱ��݂Ŏg�p������گĂ�S��Del
//----------------------------------------------------------
void PokeIconPaletteAllDel(void)
{
	u8 i;
	
	for(i = 0; i < ICON_PAL_MAX; i++)
		ObjPalRegNumDel(PokeIconPalData[i].reg_num);
}

//----------------------------------------------------------
//	�w���߹�݂̱��݂���گĂ�Del����
// pokeno = �J�����ް
//----------------------------------------------------------
void PokeIconPaletteDel(u16 pokeno)
{
	u8 tblno;

	if(pokeno > MONSNO_END)	//���m��MonsNo����
		pokeno = HATENA_ICON_NO;	//�u�H�v�̃L����

	tblno = IconPalAtr[pokeno];
	ObjPalRegNumDel(PokeIconPalData[tblno].reg_num);
}

//----------------------------------------------------------
//	�w���߹�݂̱��݂���گĂ�Del����
// chrid = ���̷݂��ID
//----------------------------------------------------------
void PokeIconPaletteDelCharID(u16 chrid)
{
	u8 tblno;
	tblno = IconPalAtr[chrid];
	ObjPalRegNumDel(PokeIconPalData[tblno].reg_num);
}

//----------------------------------------------------------
//	�߹�ݱ��ݗp��аMove�֐�
//----------------------------------------------------------
void PokeIconDmmyMove(actWork *Xreg)
{
	ExtAnm(Xreg);
}


