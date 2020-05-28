//=========================================================================
//
//		�|�P�����R���e�X�g�p�c�[��
//											by matsuda 2001.09.20(��)
//=========================================================================

#include "common.h"
#include "poketool.h"
#include "con_tool.h"
#include "contest.h"
#include "menu.h"
#include "print.h"
#include "pokeicon.h"
#include "cli_def.h"
#include "objdata.h"
#include "client_t.h"
#include "madefine.h"
#include "decord.h"
#include "palanm.h"
#include "con_waza.h"
#include "actanm.h"
#include "fight.h"
#include "calctool.h"
#include "conscr.h"
#include "evobjdat.h"
#include "task.h"
#include "ef_tool.h"
#include "memtool.h"
#include "mus_tool.h"
#include "laster.h"
#include "item.h"
#include "bss.h"
#include "wazatool.h"
#include "waza_eff.h"
#include "consio.h"
#include "monsno.def"
#include "server.h"
#include "wazano.def"


//----------------------------------------------------------
//	���b�Z�[�W�f�[�^
//----------------------------------------------------------
#include "..\msgdata\conmsg.h"


//----------------------------------------------------------
//	�O���ϐ�
//----------------------------------------------------------
extern	TALK_WIN	FightMsg;
extern	TALK_WIN	win[4];
//extern	u8	FightMsgBuf[64];
extern unsigned long r_next;	//���b��@�f�o�b�N�p

//----------------------------------------------------------
//	�O���f�[�^
//----------------------------------------------------------
extern	const u32 DecordAdrs[];
extern const actAffAnm * const PmAffAnmTblF2[];
extern const actAffAnm * const PmAffAnmTblB2[];
extern const actAffAnm * const PmAffAnmTblB[];
extern const actAffAnm * const PmAffAnmTblF[];
extern	const	WazaTblDat	WTD[];

//----------------------------------------------------------
//	�O���֐�
//----------------------------------------------------------
extern void	ConWazaAIInit(u8);
extern u8		ConWazaAIMain(void);

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
void PutBreederName(u8 pos, u8);
void PutPokeName(u8 pos, u8);
void PutPokeNameIt(u8 pos);
void PutBreederNameIt(u8 pos);
static s8 GetMeterNum(s16 ap);
static void AppealMeterPutSeq(u8 id);
static void JyunniIconEffectInitSeq(actWork *Xreg);
static void JyunniIcon_BtoS_Init(actWork *Xreg);
static void JyunniIcon_StoB_Init(actWork *Xreg);
static void JyunniIcon_StoB(actWork *Xreg);
static void EyeColorFadeStopMain(u8 id);
static void EyeColorFadeSeq(u8 id);
static void MeterColorFadeCheck(u8 brd_pos);
static void MeterColorFadeSeq(u8 id);
void MeterColorFlagClear(u8 brd_pos);
void MeterColorClearAll(void);
static void MeterDefaultColorSet(u8 brd_pos);
static void AppealKoukaColorSeq(u8 id);
static void BrdWinAffEndSeq(actWork *Xreg);
static void BrdWinRegClearEnd(actWork *Xreg);
void UsedWorkNameClear(u8 brd);
void DebugAPUpdatingCheckPut(void);
static u16 GetBaseMeterScrn(u8 brd);
u8 PortraitDataSpaceSearch(u8 tiiki_no, u8);
static void AppealHeartPutSeq(u8 id);
static void HeartIconMoveSeq(actWork *xreg);
static void PlayerInfoCopySub(u32 ScrnBaseAdr);
void ConWazaBrdSort(void);
//static u16 GetNextSortScrn(u8 brd_pos);
//static u16 GetNextSortNumScrn(u8 brd_pos);
//static void VoltageIconSeq(actWork *xreg);
//static void VoltageIconStopSeq(actWork *xreg);
u8 TensyonMaxPalAnmSet(void);
static void TensyonMaxPalAnmMain(u8 id);
static void KankyakuAnmTask(u8 id);
static void VoltageGaugeSlidPutTask(u8 id);
static void VoltageGaugeSlidOutTask(u8 id);
static void VoltageGaugeSlidInTask(u8 id);
static void KankyakuColorFadeTask(u8 id);
static u32 GetNextSortNumAdrs(u8 brd_pos);
static void HeartIconAffEffCheckSeq(u8 id);
void InfoRewriteData(u8 id);
void SetEnemyBanishActor(void);
void EnemyBanishActorParamInit(void);
u16 GetContWazaNo(u16 wazano);
u16 GetContMonsNo(u16 monsno);
static void GetContestJyunni(void);
static u8 EvalSortConditionsCheck(int brd1, int brd2, int data[][4]);
u8 ContestPokeFlipCheck(u16 monsno);

//----------------------------------------------------------
//	���ʱ���
//----------------------------------------------------------
#define JYUNNI_ICON_AFFSP	15
#define JYUNNI_ICON_AFFLOOP	15
const CellData JyunniIconCell={
	(void *)contrank_pch_ADRS,OBJ_SIZE16_16x16*4, CELLID_JYUNNIICON0
};
const PalData JyunniIconPal[]={
	{(void *)contrank_pcl_ADRS, CELLID_JYUNNIICON0},
	{(void *)contrank_pcl_ADRS, CELLID_JYUNNIICON1},
	{(void *)contrank_pcl_ADRS, CELLID_JYUNNIICON2},
	{(void *)contrank_pcl_ADRS, CELLID_JYUNNIICON3},
};
static const actAnm JyunniIconAnm0[]={
	{OBJ16_16x16*0,1,0,0},			
	{ACT_ANMEND,0,0,0}
};
static const actAnm JyunniIconAnm1[]={
	{OBJ16_16x16*1,1,0,0},			
	{ACT_ANMEND,0,0,0}
};
static const actAnm JyunniIconAnm2[]={
	{OBJ16_16x16*2,1,0,0},			
	{ACT_ANMEND,0,0,0}
};
static const actAnm JyunniIconAnm3[]={
	{OBJ16_16x16*3,1,0,0},			
	{ACT_ANMEND,0,0,0}
};
static const actAnm *const JyunniIconAnmTbl[]={
	JyunniIconAnm0,
	JyunniIconAnm1,
	JyunniIconAnm2,
	JyunniIconAnm3,
};
static const actAffAnm JyunniIconAffAnm0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0, 0, 0, 1 },						// ����
	{ACT_AFEND,0,0,0},					// ٰ�� , �߂��
};
static const actAffAnm JyunniIconAffAnm1[] = {	//�ʏ��Ԃ��珬����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ -JYUNNI_ICON_AFFSP, -JYUNNI_ICON_AFFSP, 0, JYUNNI_ICON_AFFLOOP},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm JyunniIconAffAnm2[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0x100-JYUNNI_ICON_AFFSP*JYUNNI_ICON_AFFLOOP,
		 0x100-JYUNNI_ICON_AFFSP*JYUNNI_ICON_AFFLOOP, 0, ACT_AFSET },
	{JYUNNI_ICON_AFFSP, JYUNNI_ICON_AFFSP, 0, JYUNNI_ICON_AFFLOOP},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const JyunniIconAffTbl[] = {
	JyunniIconAffAnm0,
	JyunniIconAffAnm1,
	JyunniIconAffAnm2,
0};
static const ActOamData JyunniIconOamData = {
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
	0,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
const actHeader JyunniIconHeader[]={
	{
		CELLID_JYUNNIICON0,
		CELLID_JYUNNIICON0,
		&JyunniIconOamData,
		JyunniIconAnmTbl,
		0,
		JyunniIconAffTbl,
		DummyActMove,
	},
	{
		CELLID_JYUNNIICON0,
		CELLID_JYUNNIICON1,
		&JyunniIconOamData,
		JyunniIconAnmTbl,
		0,
		JyunniIconAffTbl,
		DummyActMove,
	},
	{
		CELLID_JYUNNIICON0,
		CELLID_JYUNNIICON2,
		&JyunniIconOamData,
		JyunniIconAnmTbl,
		0,
		JyunniIconAffTbl,
		DummyActMove,
	},
	{
		CELLID_JYUNNIICON0,
		CELLID_JYUNNIICON3,
		&JyunniIconOamData,
		JyunniIconAnmTbl,
		0,
		JyunniIconAffTbl,
		DummyActMove,
	},
};

//----------------------------------------------------------
//	BG�̏����޳�@OBJ���Ƃ�
//----------------------------------------------------------
enum{
	CELLID_BRD_WIN0 = 33000,
	CELLID_BRD_WIN1,
	CELLID_BRD_WIN2,
	CELLID_BRD_WIN3,
};
static const CellData BrdWinCell[] = {
	{(void*)space64_pch_ADRS, OBJ_SIZE16_64x64*2, CELLID_BRD_WIN0},
	{(void*)space64_pch_ADRS, OBJ_SIZE16_64x64*2, CELLID_BRD_WIN1},
	{(void*)space64_pch_ADRS, OBJ_SIZE16_64x64*2, CELLID_BRD_WIN2},
	{(void*)space64_pch_ADRS, OBJ_SIZE16_64x64*2, CELLID_BRD_WIN3},
};
static const PalData BrdWinPal[] = {
	{(void*)&UserWork[CONT_PALETTE_ADRS + 16*5*2], CELLID_BRD_WIN0},
	{(void*)&UserWork[CONT_PALETTE_ADRS + 16*6*2], CELLID_BRD_WIN1},
	{(void*)&UserWork[CONT_PALETTE_ADRS + 16*7*2], CELLID_BRD_WIN2},
	{(void*)&UserWork[CONT_PALETTE_ADRS + 16*8*2], CELLID_BRD_WIN3},
};
static const ActOamData BrdWinOamData = {
	0,		//u32 VPos:8;             // �x���W
	3,		//u32 AffineMode:2;       // �A�t�B�����[�h
	1,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	0,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
	3,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	0,		//u32 Priority:2;         // �\���D��
	0,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};
static const actAffAnm BrdWinAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0x100, 0x100, 0, ACT_AFSET },
	{ACT_AFEND,0,0,0},					// ٰ�� , �߂��
};
#define WIN_AFF		3
#define WIN_AFF_LOOP	15
static const actAffAnm BrdWinAff1[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ WIN_AFF,	WIN_AFF,	0, WIN_AFF_LOOP},
	{ -WIN_AFF, -WIN_AFF,	0, WIN_AFF_LOOP},
	{ WIN_AFF, 	WIN_AFF, 	0, WIN_AFF_LOOP},
	{ -WIN_AFF, -WIN_AFF, 	0, WIN_AFF_LOOP},
	{ACT_AFEND,0,0,0},					// ٰ�� , �߂��
};
static const	actAffAnm * const BrdWinAffTbl[] = {
	BrdWinAff0,
	BrdWinAff1,
};
static const actHeader BrdWinActHeader[]={
	{
		CELLID_BRD_WIN0,
		CELLID_BRD_WIN0,
		&BrdWinOamData,
		DummyActAnmTbl,
		0,
		BrdWinAffTbl,
		DummyActMove,
	},
	{
		CELLID_BRD_WIN1,
		CELLID_BRD_WIN1,
		&BrdWinOamData,
		DummyActAnmTbl,
		0,
		BrdWinAffTbl,
		DummyActMove,
	},
	{
		CELLID_BRD_WIN2,
		CELLID_BRD_WIN2,
		&BrdWinOamData,
		DummyActAnmTbl,
		0,
		BrdWinAffTbl,
		DummyActMove,
	},
	{
		CELLID_BRD_WIN3,
		CELLID_BRD_WIN3,
		&BrdWinOamData,
		DummyActAnmTbl,
		0,
		BrdWinAffTbl,
		DummyActMove,
	},
};


//=========================================================================
//	��������ذ�ްNo���擾
//=========================================================================
void GetMyBreederNo(void)
{
	if((ContestTuusin & C_TUUSIN_FLG) == 0)
		MyBreederNo = 3;
}

//=========================================================================
//	�ʐM�ΐ펞�A�������e�����ׂ�
//	1=�e�A0=�q
//=========================================================================
u8 ContestSioOyaCheck(void)
{
	if(MyBreederNo == ContestSioOya)
		return 1;
	return 0;
}

//=========================================================================
//	��ذ�ްܰ��Ɏ�������ذ�ް�ް���Ă���
// temotino = �莝���̉��Ԗڂ��߹�݂��ް���Ă��邩
//=========================================================================
void SetMyBreederData(u8 temotino)
{
	u16 itemno;
	u8 buff[20];
	s16 style, beautiful, cute, clever, strong;
	
	PM_strcpy(buff, MyData.my_name);
#if	( PM_LANG== LANG_JAPAN )
	buff[5] = EOM_;		//���[�J���C�Y�Ή��̂���5�����Ő؂�
#endif
	PM_strcpy(BreederParam[MyBreederNo].name, buff);
	if(MyData.my_sex == MALE)
		BreederParam[MyBreederNo].pattern = CTHERO;	//HERO;
	else
		BreederParam[MyBreederNo].pattern = CTHINE;	//HEROINE;
	BreederParam[MyBreederNo].ai = 0;
	BreederParam[MyBreederNo].msg[0] = 0;//EOM_;
	BreederParam[MyBreederNo].monsno = PokeParaGet(&PokeParaMine[temotino], ID_monsno);
	PokeParaGet(&PokeParaMine[temotino], ID_nickname, buff);
	NickNameCutOff(buff);
	PM_strcpy(BreederParam[MyBreederNo].nickname, buff);
	BreederParam[MyBreederNo].style = PokeParaGet(&PokeParaMine[temotino], ID_style);
	BreederParam[MyBreederNo].beautiful
			 = PokeParaGet(&PokeParaMine[temotino], ID_beautiful);
	BreederParam[MyBreederNo].cute = PokeParaGet(&PokeParaMine[temotino], ID_cute);
	BreederParam[MyBreederNo].clever = PokeParaGet(&PokeParaMine[temotino], ID_clever);
	BreederParam[MyBreederNo].strong = PokeParaGet(&PokeParaMine[temotino], ID_strong);
	BreederParam[MyBreederNo].fur = PokeParaGet(&PokeParaMine[temotino], ID_fur);
	BreederParam[MyBreederNo].waza[0] = PokeParaGet(&PokeParaMine[temotino], ID_waza1);
	BreederParam[MyBreederNo].waza[1] = PokeParaGet(&PokeParaMine[temotino], ID_waza2);
	BreederParam[MyBreederNo].waza[2] = PokeParaGet(&PokeParaMine[temotino], ID_waza3);
	BreederParam[MyBreederNo].waza[3] = PokeParaGet(&PokeParaMine[temotino], ID_waza4);
	BreederParam[MyBreederNo].personal_rnd = PokeParaGet(&PokeParaMine[temotino], ID_personal_rnd);
	BreederParam[MyBreederNo].id_no = PokeParaGet(&PokeParaMine[temotino], ID_id_no);
	
	itemno = PokeParaGet(&PokeParaMine[temotino], ID_item);

	style = BreederParam[MyBreederNo].style;
	beautiful = BreederParam[MyBreederNo].beautiful;
	cute = BreederParam[MyBreederNo].cute;
	clever = BreederParam[MyBreederNo].clever;
	strong = BreederParam[MyBreederNo].strong;
	if(itemno == ITEM_AKAIBANDANA)
		style += 20;
	else if(itemno == ITEM_AOIBANDANA)
		beautiful += 20;
	else if(itemno == ITEM_PINKUNOBANDANA)
		cute += 20;
	else if(itemno == ITEM_MIDORINOBANDANA)
		clever += 20;
	else if(itemno == ITEM_KIIRONOBANDANA)
		strong += 20;
	
	if(style > 255)
		style = 255;
	if(beautiful > 255)
		beautiful = 255;
	if(cute > 255)
		cute = 255;
	if(clever > 255)
		clever = 255;
	if(strong > 255)
		strong = 255;
	BreederParam[MyBreederNo].style = style;
	BreederParam[MyBreederNo].beautiful = beautiful;
	BreederParam[MyBreederNo].cute = cute;
	BreederParam[MyBreederNo].clever = clever;
	BreederParam[MyBreederNo].strong = strong;
}

//=========================================================================
//	�R���e�X�g�ɎQ������u���[�_�[�̃f�[�^���
// �����Ftype = �Q�������ýĂ̎��  rank = ��ýĂ��ݸ
//=========================================================================
void SetBreederData(u8 type, u8 rank)
{
	int i, t;
	u8 putbrd[BREEDER_AI_DATA_MAX+1];
	u8 count = 0, put;
	u16 no;
	
	GetMyBreederNo();
	for(i = 0; i < BREEDER_AI_DATA_MAX; i++){
		put = 0;
		
		if(rank != bd_tbl[i].rank)
			continue;
		
		if(type == STYLE_CONTEST && bd_tbl[i].style_appear)
			put++;
		else if(type == BEAUTIFUL_CONTEST && bd_tbl[i].beautiful_appear)
			put++;
		else if(type == CUTE_CONTEST && bd_tbl[i].cute_appear)
			put++;
		else if(type == CLEVER_CONTEST && bd_tbl[i].clever_appear)
			put++;
		else if(type == STRONG_CONTEST && bd_tbl[i].strong_appear)
			put++;
		else
			continue;
		
		putbrd[count++] = i;
	}
	putbrd[count] = 0xff;	//�ԕ�
	
	for(i = 0; i < BREEDER_MAX-1; i++){
		no = pp_rand() % count;
		BreederParam[i] = bd_tbl[putbrd[no]];
		for(t = no; putbrd[t] != 0xff; t++)
			putbrd[t] = putbrd[t+1];
		count--;
	}
	SetMyBreederData(ConTemotiNo);
}


//=========================================================================
//	��ýĎQ������
//	�߂�l�F 1 = �Q���ł���	0 = �Q���ł��Ȃ�	2 = ���ɗD���ς�
//			 3 = �^�}�S		4 = �m��
//=========================================================================
u8 ContestParticipationCheck(PokemonParam *pp)
{
	u8 kekka = 0;
	u8 medal = 0;
	
	// �^�}�S�͎Q���s��		2002/05/31 by nakahiro
	if( PokeParaGet( pp, ID_tamago_flag ) != 0 )
		return	3;
	if(PokeParaGet(pp, ID_hp) == 0)
		return 4;	//�C���Ԃ��Q���s��
		
	switch(ContestKind)
	{
		case STYLE_CONTEST:				//�����߹�݂̊i�D�悳
			medal = PokeParaGet(pp, ID_stylemedal);
			break;
		case BEAUTIFUL_CONTEST:			//�����߹�݂̔�����
			medal = PokeParaGet(pp, ID_beautifulmedal);
			break;
		case CUTE_CONTEST:				//�����߹�݂̉���
			medal = PokeParaGet(pp, ID_cutemedal);
			break;
		case CLEVER_CONTEST:				//�����߹�݂̌���
			medal = PokeParaGet(pp, ID_clevermedal);
			break;
		case STRONG_CONTEST:				//�����߹�݂̂����܂���
			medal = PokeParaGet(pp, ID_strongmedal);
			break;
		default:
			//kekka = 0;
			return 0;
	}
	
	if(medal > ContestRank)
		kekka = 2;
	else if(medal >= ContestRank)
		kekka = 1;
	else
		kekka = 0;

	return kekka;
}


//=========================================================================
//	��ʉE�̃v���C���[����\��
//=========================================================================
void PutPlayerInfo(void)
{
	u8 i;
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		FontScrnBoxFill(&FightMsg, MSG_CLEAR_NO, C_PokeNamePos[BrdSort[i]][0],
				C_PokeNamePos[BrdSort[i]][1], C_BreederPos[BrdSort[i]][0]+5, 
					C_BreederPos[BrdSort[i]][1]+1);
		PutBreederNameIt(i);
		PutPokeNameIt(i);
	}
}

//----------------------------------------------------------
//	�t�H���g���ʐݒ�
//----------------------------------------------------------
static const u8 FontCommon_msg[] = {CTRL_,C_BCOL_,COL_NUKE,CTRL_,C_FCOL_,EOM_};
static u8 * FontCommonSet(const u8 *msg_adr, u8 font_col)
{
	u8 *msg_end;

	msg_end = PM_strcpy(FightMsgBuf, FontCommon_msg);
	msg_end[0] = font_col;
	msg_end++;
	msg_end = PM_strcpy(msg_end, msg_adr);
	return msg_end;
}
//----------------------------------------------------------
//	�u���[�_�[���\��
//----------------------------------------------------------
void PutBreederNameIt(u8 pos)
{
	u8 font_col;
	
/*	if(pos == MyBreederNo)
		font_col = COL_WHITE;
	else
		font_col = COL_BLACK;
*/
	font_col = BLD_PAL + pos;
	PutBreederName(pos, font_col);
}

void PutBreederName(u8 pos, u8 font_col)
{
	u8 msg[30];
	
	msg[0] = sura_;
	PM_strcpy(&msg[1], BreederParam[pos].name);
	FontCommonSet(msg, font_col);
	MsgPrint(&FightMsg, FightMsgBuf, CON_TRAINER + CON_TRAINER_LEN* BrdSort[pos],
			C_BreederPos[BrdSort[pos]][0] - 1, C_BreederPos[BrdSort[pos]][1]);
}
//----------------------------------------------------------
//	�߹�݂�Ư�Ȱѕ\��
//----------------------------------------------------------
void PutPokeNameIt(u8 pos)
{
	u8 font_col;
	
/*
	if(pos == MyBreederNo)
		font_col = COL_WHITE;
	else
		font_col = COL_BLACK;
*/
	font_col = BLD_PAL+pos;
	PutPokeName(pos, font_col);
}

void PutPokeName(u8 pos, u8 font_col)
{
	u8 *msg;
	u8 name_len;
	u8 i;
	
//	NMenuMsgWrite(BreederParam[pos].nickname, C_PokeNamePos[pos][0], C_PokeNamePos[pos][1]);
	name_len = PM_strlen(BreederParam[pos].nickname);
	name_len = 5 - name_len;
	msg = FontCommonSet(BreederParam[pos].nickname, font_col);
	for(i = 0 ; name_len > i; i++)
		msg[i] = spc_;
	msg[name_len] = EOM_;	//sura_;
//	msg[name_len+1] = EOM_;
	MsgPrint(&FightMsg, FightMsgBuf, CON_POKENAME + CON_POKE_LEN * BrdSort[pos], 
			C_PokeNamePos[BrdSort[pos]][0], C_PokeNamePos[BrdSort[pos]][1]);
}




//=========================================================================
//	�u���[�_�[�|�C���g���擾����
//=========================================================================
u16 GetBP(u8 brd, u8 MainContest)
{
	u8 main_data, sub1, sub2;
	u16 ret;
	
	switch(MainContest)
	{
		case STYLE_CONTEST:
			main_data = BreederParam[brd].style;
			sub1 = BreederParam[brd].strong;
			sub2 = BreederParam[brd].beautiful;
			break;
		case BEAUTIFUL_CONTEST:
			main_data = BreederParam[brd].beautiful;
			sub1 = BreederParam[brd].style;
			sub2 = BreederParam[brd].cute;
			break;
		case CUTE_CONTEST:
			main_data = BreederParam[brd].cute;
			sub1 = BreederParam[brd].beautiful;
			sub2 = BreederParam[brd].clever;
			break;
		case CLEVER_CONTEST:
			main_data = BreederParam[brd].clever;
			sub1 = BreederParam[brd].cute;
			sub2 = BreederParam[brd].strong;
			break;
		case STRONG_CONTEST:
		default:
			main_data = BreederParam[brd].strong;
			sub1 = BreederParam[brd].clever;
			sub2 = BreederParam[brd].style;
			break;
	}
//	ret = main_data + (double)sub1 / 2 + (double)sub2 / 2 + (double)BreederParam[brd].fur / 2;
	ret = main_data + ((sub1 + sub2 + BreederParam[brd].fur) / 2);
	return ret;
}

void GetBPAll(u8 MainContest)
{
	u8 i;
	
	for(i = 0; i < BREEDER_MAX; i++)
		ContBP[i] = GetBP(i, MainContest);
}


//=========================================================================
//	�R�����̱���(��گāA�O���t�B�b�N�ް��܂�)���Z�b�g����
//=========================================================================
u8 ContestJudgeActorSet(void)
{
	u8 actno;

	DecordCellSet((CellData*)&JudgeCell);
	DecordPaletteWork((void*)shinsain_pcl_ADRS, PA_OBJ0 + 0x10 * 1, 0x20);
	actno = AddActor(&JudgeHeader, CON_JUDGE_X, CON_JUDGE_Y, C_POKE1PRI);

	ActWork[actno].oamData.Pltt = 1;
	ActWork[actno].move = DummyActMove;
	
//	ActWork[actno].aff_tbl = PmAffAnmTblF2;
//	ActAffAnmChg(&ActWork[actno], 0);
	
	return actno;
}

//=========================================================================
//	�R���̂ӂ������A�C�R���A�N�^�[�̾��
//=========================================================================
u8 JudgeFukidasiActorSet(void)
{
	u8 actno;
	
	DecordCellSet((CellData*)&FukidasiCell);
	DecordPalSet((void*)&FukidasiPal);
	actno = AddActor(&FukidasiHeader, CON_FUKIDASI_X, CON_FUKIDASI_Y, C_POKE1PRI-1);
	ActWork[actno].banish = 1;
	//���ð��ق͎g�p���Ȃ��̂�į�߂̷��No��ێ����Ă���
	ActWork[actno].work[0] = ActWork[actno].oamData.CharNo;
	return actno;
}

//=========================================================================
//	�|�P�����̃A�N�^�[(��گāA���̨���ް��܂�)���Z�b�g����
//		�����ʌ���
//=========================================================================
u8 ContestPokeActorSet(void)
{
	u8 actno;
	u16 monsno;
	
	monsno = BreederParam[MyBreederNo].monsno;
	DecordPokeCellMake(&PM2FObjDataCell[monsno], pm2fpara[monsno].size,
				pm2fpara[monsno].patcnt, (void*)0x2000000,(void*)DecordAdrs[1],monsno);
	DecordPaletteWork((void*)PM2NObjDataPal[monsno].pal_data,
		PA_OBJ0 + 0x10 * 1, 0x20);
	PokeActHeaderMake(BreederParam[MyBreederNo].monsno, 1);
	actno = AddActor(&PokeActHeader, CON_POKE_X, CON_POKE_Y + 
		4*(8-pm2fpara[BreederParam[MyBreederNo].monsno].size),
					C_POKE1PRI);	//�߹��
	ActWork[actno].oamData.Pltt = 1;
	ActWork[actno].move = DummyActMove;
	
	ActWork[actno].aff_tbl = PmAffAnmTblF2;
	ActAffAnmChg(&ActWork[actno], 0);
	
	return actno;
}
//=========================================================================
//	�|�P�����̃A�N�^�[(��گāA���̨���ް��܂�)���Z�b�g����
//		���w�����
//=========================================================================
u8 ContestPokeActorSet2(u16 monsno, u32 id_no, u32 personal_rnd)
{
	u8 actno;
	u32 palAdrs;
	
	monsno = GetContMonsNo(monsno);
	
	DecordPokeCellMakeBuchi(&PM2BObjDataCell[monsno], pm2bpara[monsno].size,
				pm2bpara[monsno].patcnt, (void*)0x2000000,(void*)DecordAdrs[CONT_POKEDECORD_NO],monsno, personal_rnd);

	//�Z�̪�Ă��߹�݂���گ�No�̎擾��AttackNo�Ŏ���Ă�̂ł���ɍ��킹��
	palAdrs = PokePalGetPara(monsno, id_no, personal_rnd);
	DecordPaletteWork((void*)palAdrs, PA_OBJ0 + 0x10 * CONT_MINE_CLIENT, 0x20);
	PokeActHeaderMake(monsno, CONT_POKEDECORD_NO);
//	actno = AddActor(&PokeActHeader, CON_POKE_X, CON_POKE_Y + 
//		pm2bpara[monsno].patcnt, C_POKE1PRI);	//�߹��
	actno = AddActor(&PokeActHeader, CON_POKE_X, 
		GetWazaEffPokeYPos(CONT_MINE_CLIENT, monsno, 0), C_POKE1PRI);	//�߹��
//	ActWork[actno].oamData.Pltt = 0;
	ActWork[actno].oamData.Pltt = 2;	// 2002.06.06(��) AttackClient�̈ʒu�ɍ��킹��
	ActWork[actno].oamData.Priority = POKE_OAM_PRI;
	ActWork[actno].pri = GetPokeSoftPri(CONT_MINE_CLIENT);
	ActWork[actno].move = DummyActMove;
	ActWork[actno].work[0] = ActWork[actno].oamData.Pltt;	//�퓬�Ɠ����悤��ClientNo
	ActWork[actno].work[2] = monsno;	//�퓬�Ɠ����悤��MonsNo�����Ă���
	
	if(ContestPokeFlipCheck(monsno))
		ActWork[actno].aff_tbl = PmAffAnmTblF2;
	else
		ActWork[actno].aff_tbl = PmAffAnmTblF;	//���]�֎~�|�P����
	ActAffAnmChg(&ActWork[actno], 0);
	
	return actno;
}

//-- ��ýĂŔ��]�����Ȃ��L���������� --//
//----------------------------------------------------------
//	��ýĂŔ��]�����Ȃ��L����������
// �߂�l: 1=���]OK	0=���]�֎~
//----------------------------------------------------------
u8 ContestPokeFlipCheck(u16 monsno)
{
	if(monsno == MONSNO_ANNOON)
		return 0;
	return 1;
}


//=========================================================================
//	1���ڂɏ�����Ă���v���C���[����2���ڂ̽�ذ݂ɺ�߰����
//=========================================================================
void PlayerInfoCopy(void)
{
	PlayerInfoCopySub(SCREEN_BASE24_ADDR);	//�t�H���g�����̃R�s�[
	PlayerInfoCopySub(SCREEN_BASE28_ADDR);	//Window�����̃R�s�[
}

static void PlayerInfoCopySub(u32 ScrnBaseAdr)
{
//	vu8 dma_wait;

	//�㔼���܂ź�߰�@�@1���ڂƂ��Ԃ镔�����Ȃ��̂�
	DIV_DMACOPY(3, ScrnBaseAdr, ScrnBaseAdr + (256/8)*(160/8)*2,
		 (256/8)*(80/8)*2, 16);
}


//=========================================================================
//	�Z�^�C�v�̃A�C�R���̷�����ް���擾����
//=========================================================================
static u16 GetWazaKubunIconCharNo(u16 wazano, u8 brd_pos)
{
	u8 kubun;
	u16 scrn_data;
	
//	kubun = conwaza_tbl[wazano].kubun;
	kubun = WazaKoukaTbl[ConWazaTbl[wazano].apno].class;
	
	switch(kubun)
	{
		case NORMAL_KUBUN:			//�m�[�}��
		case COMBO2_KUBUN:			//�R���{2
		case BOUGYO_KUBUN:			//�h��
			scrn_data = 0x9082;
			break;
		case DIST1_KUBUN:				//�W�Q
		case DIST2_KUBUN:
			scrn_data = 0x9088;
			break;
		case SPECIAL_KUBUN:			//����
		case COMBO1_KUBUN:			//�R���{1
		default:
			scrn_data = 0x9086;
			break;
	}
	scrn_data += 0x9000 + brd_pos*0x1000;
	return scrn_data;
}

//=========================================================================
//	�Z�̌��ʺ��ĕ\��
//=========================================================================
static const u8 sura_msg[] = {sura_,EOM_,};

void PutWazaComment(u16 wazano)
{
	u16 icon_scrn;
	int i;
	u16 end_heart;
//	u16 offset_char;
	u8 con_type;
	
	//-- �Z�̑�����\��(�����������A���킢����) --//
	FontScrnBoxFill(&FightMsg, MSG_CLEAR_NO,
		CON_TYPE_X, CON_TYPE_Y, CON_TYPE_X+5, CON_TYPE_Y+3);
//	MsgPrint(&FightMsg, ConTypeMsgTbl[ConWazaTbl[wazano].con_type],
//			CON_TYPETEXT, CON_TYPE_X, CON_TYPE_Y);
	con_type = ConWazaTbl[wazano].con_type;
	if(con_type == STYLE_TYPE)
		icon_scrn = STYLE_MSG_SCRN;
	else if(con_type == BEAUTIFUL_TYPE)
		icon_scrn = BEAUTIFUL_MSG_SCRN;
	else if(con_type == CUTE_TYPE)
		icon_scrn = CUTE_MSG_SCRN;
	else if(con_type == CLEVER_TYPE)
		icon_scrn = CLEVER_MSG_SCRN;
	else
		icon_scrn = STRONG_MSG_SCRN;
	for(i = 0; i < 5; i++){
		*(vu16*)(SCREEN_BASE24_ADDR + (0x20*0x14 + 0x20*11+0xb+i)*2)
				= icon_scrn;
		*(vu16*)(SCREEN_BASE24_ADDR + (0x20*0x14 + 0x20*11+0xb+i+0x20)*2)
				= icon_scrn+0x10;
		icon_scrn++;
	}
	
	//-- ʰı��� --//
	//AP
	if(WazaKoukaTbl[ConWazaTbl[wazano].apno].ap == 0xff)	//���������
		end_heart = 0;
	else
		end_heart = WazaKoukaTbl[ConWazaTbl[wazano].apno].ap / METER_ONE_AP;// 5;
	if(end_heart > HEART_ICON_MAX)
		end_heart = HEART_ICON_MAX;
	for(i = 0; i < HEART_ICON_MAX; i++){
		if(i < end_heart)
			*(vu16 *)(SCREEN_BASE24_ADDR
				+ (0x20*0x14 + 0x20*11 + 0x15 + i)*2) = HEART_RED_SCRN0;
		else
			*(vu16 *)(SCREEN_BASE24_ADDR
				+ (0x20*0x14 + 0x20*11 + 0x15 + i)*2) = HEART_REDSPACE_SCRN;
	}
	//�W�Q�߲��
	if(WazaKoukaTbl[ConWazaTbl[wazano].apno].bougai == 0xff)	//���������
		end_heart = 0;
	else
		end_heart = WazaKoukaTbl[ConWazaTbl[wazano].apno].bougai / METER_ONE_AP;// 5;
	if(end_heart > HEART_ICON_MAX)
		end_heart = HEART_ICON_MAX;
	for(i = 0; i < HEART_ICON_MAX; i++){
		if(i < end_heart)
			*(vu16 *)(SCREEN_BASE24_ADDR
				+ (0x20*0x14 + 0x20*12 + 0x15 + i)*2) = HEART_BLACK_SCRN0;
		else
			*(vu16 *)(SCREEN_BASE24_ADDR
				+ (0x20*0x14 + 0x20*12 + 0x15 + i)*2) = HEART_BLACKSPACE_SCRN;
	}
	
//	if(conwaza_tbl[wazano].combo != 0)
//		PM_strcpy(StrTempBuffer0, ComboIMSG_Tbl[conwaza_tbl[wazano].combo]);
	MsgPrint(&FightMsg, WazaSetumeiMsgTbl[ConWazaTbl[wazano].apno],
			CON_WAZAEXPLA, CON_RANKU_X, CON_RANKU_Y);

	MsgPrint(&FightMsg, sura_msg, CON_WAZASURA, 16,20+11);
}

void WazaCommentClear(void)
{
	FontScrnBoxFill(&FightMsg, MSG_CLEAR_NO,
		CON_RANKU_X, CON_RANKU_Y, CON_RANKU_X+17, CON_RANKU_Y+5);
}

//=========================================================================
//	�Z�̋敪���݂��e��ذ�ް�̳���޳�ɕ\��
//=========================================================================
void WazaKubunIconPut(u16 wazano, u8 brd_pos)
{
	u16 icon_scrn;
	u8 ypos;
	
	ypos = 2 + (BrdSort[brd_pos] * 5);
	if(YasumiTurnCheck(brd_pos) == 0 && wazano != 0)
	{
		icon_scrn = GetWazaKubunIconCharNo(wazano, brd_pos);
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x14)*2) = icon_scrn;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x15)*2) = icon_scrn+1;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x14)*2) = icon_scrn+0x10;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x15)*2) = icon_scrn+0x11;
	}
	else
	{
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x14)*2) = 0;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x15)*2) = 0;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x14)*2) = 0;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x15)*2) = 0;
	}
}

void WazaKubunIconPutAll(void)
{
	u8 i;
	for(i = 0; i < BREEDER_MAX; i++)
		WazaKubunIconPut(apsys[i].use_waza, i);
}

//=========================================================================
//	�{�[�i�X�A�C�R���̽�ذ݃f�[�^���擾����
//=========================================================================
static u16 GetBonusIconScrn(u8 brd_pos)
{
	u16 scrn_data;
	scrn_data = 0x2034;
	return scrn_data;
}

//=========================================================================
//	�{�[�i�X���݂��e��ذ�ް�̳���޳�ɕ\��
//	�ްŽ�A�C�R���ɂ�t�����A���͏��������� 1�A
//	���ɃA�C�R���ɑ΂��ĉ������Ȃ��������� 0
//	se_flg = 1:SE��点��	0:�炳�Ȃ�
//=========================================================================
u8 BonusIconPut(u8 brd_pos, u8 se_flg)
{
	u16 icon_scrn;
	u8 ypos;
	int i;
	
	if(apsys[brd_pos].bonus_flg == 0)
		return 0;
		
	ypos = 2 + (BrdSort[brd_pos] * 5);
	if(apsys[brd_pos].bonus_flg == 1){			//�ްŽ���t����
		icon_scrn = GetBonusIconScrn(brd_pos);
		for(i = 0; apsys[brd_pos].bonus / 10 > i; i++)
			*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+i) + 0x13)*2) = icon_scrn;
		if(se_flg){
			SePlay(SE_EXPMAX);	//SE_RU_GASHIN);
			apsys[brd_pos].bonus_flg = 0;
		}
	}else{										//�ްŽ��������
		icon_scrn = MSG_CLEAR_NO;
		for(i = 3;apsys[brd_pos].bonus / 10 < i; i--)
			*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+i-1) + 0x13)*2) = icon_scrn;
		if(se_flg){
			SePlay(SE_FU_ZAKU2);
			apsys[brd_pos].bonus_flg = 0;
		}
	}
	return 1;
}

//=========================================================================
//	�S�Ă���ذ�ް���ްŽ���݂��ĕ`�悷��
//=========================================================================
void BonusIconResetAll(void)
{
	u16 icon_scrn;
	u8 ypos;
	int i, t;
	
	for(t = 0; t < BREEDER_MAX; t++){
		ypos = 2 + (BrdSort[t] * 5);
		icon_scrn = GetBonusIconScrn(t);
		for(i = 0; i < apsys[t].bonus / 10; i++)
			*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+i) + 0x13)*2) = icon_scrn;
		icon_scrn = MSG_CLEAR_NO;
		for( ; i < 3; i++)
			*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+i) + 0x13)*2) = icon_scrn;
	}
}


//=========================================================================
//	�Z���ʂŔ�������A�C�R���̷�����ް���擾����
//=========================================================================
static u16 GetWazaKoukaIconScrn(u8 brd_pos, u8 icon_type)
{
	u16 scrn_data = 0;
	
	switch(icon_type)
	{
		case SHIELD_TYPE:			//�V�[���h
			scrn_data = 0x0080;
			break;
		case KINTYOU_TYPE:			//�ْ�
			scrn_data = 0x0084;
			break;
		case YASUMI_TYPE:			//�x��
			scrn_data = 0x0086;
			break;
		case BOUGAI_TARGET_TYPE:	//�W�Q�̃^�[�Q�b�g�ɂ��ꂽ
			scrn_data = 0x0088;
			break;
		case BOUGAI2BAI_TYPE:		//�W�Q2�{
			scrn_data = 0x0082;
			break;
	}
	scrn_data += 0x9000;
	return scrn_data;
}

//=========================================================================
//	�Z���ʂŔ�������A�C�R���̕\��(�h��Ƃ��A�ْ��Ƃ�)
//	�߂�l�F1=���݂�`����	0=���݂���������
//=========================================================================
u8 WazaKoukaIconPut(u8 brd)
{
	u8 put_mode = 1;
	u16 icon_scrn = 0;
	u8 ypos;
	
	ypos = 2 + (BrdSort[brd] * 5);
	
	if(apsys[brd].Ashield_flg || apsys[brd].invalid10 || 
		apsys[brd].shield_flg || apsys[brd].invalid)
	{	//�V�[���h�A�C�R���\��
		icon_scrn = GetWazaKoukaIconScrn(brd, SHIELD_TYPE);
	}
	else if(apsys[brd].miss_flg)	//�ْ��A�C�R��
		icon_scrn = GetWazaKoukaIconScrn(brd, KINTYOU_TYPE);
	else if(apsys[brd].yasumi_turn || apsys[brd].endAppeal_flg)	//�x�݃A�C�R��
		icon_scrn = GetWazaKoukaIconScrn(brd, YASUMI_TYPE);
	else
		put_mode = 0;
	
	if(put_mode)
	{
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x14)*2) = icon_scrn;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x15)*2) = icon_scrn+1;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x14)*2) = icon_scrn+0x10;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x15)*2) = icon_scrn+0x11;
	}
	else
	{	//�A�C�R������
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x14)*2) = 0;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x15)*2) = 0;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x14)*2) = 0;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x15)*2) = 0;
	}
	
	return put_mode;
}

void WazaKoukaIconPutAll(void)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++)
		WazaKoukaIconPut(i);
}

//=========================================================================
//	Talk���b�Z�[�W�\��������ر�
//=========================================================================
void ContestTalkMsgClear(void)
{
	FontScrnBoxFill(&FightMsg, MSG_CLEAR_NO,
		CON_TALKMSG_X, CON_TALKMSG_Y, CON_TALKMSG_X+15, CON_TALKMSG_Y+3);
}


//=========================================================================
//	�J��o�����Z���ް���擾
//=========================================================================
u16 UseWorkGet(u8 pos)
{
	u8 wazapos;
	u16 wazano;
/*	u16 rand_temp;
	
	rand_temp = r_next;
	pp_srand(sys.main_timer);
	r_next += pos;
	*/
	
	if(YasumiTurnCheck(pos))
		wazano = 0;
	else if(pos != MyBreederNo){
//		wazano = BreederParam[pos].waza[pp_rand() % 4];	//�Z������тŹޯ�
		ConWazaAIInit(pos);
		wazapos = ConWazaAIMain();
		wazano = BreederParam[pos].waza[wazapos];
	
	}else
		wazano = BreederParam[pos].waza[consys->cursor];
	
	//r_next = rand_temp;	//�^�l�����ɖ߂�
	
	return wazano;
}

void UseWorkGetAll(void)
{
	u8 i;
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		apsys[i].use_waza = UseWorkGet(i);
	}
}

//=========================================================================
//	�g�p����Z����\��
// deep = �F�̔Z��  0:���ʂ̐F   1:�����F
//=========================================================================
void UsedWorkNamePut(u8 brd, u8 deep)
{
	u8 font_col;
	
	if(deep == 0)
	//	brd == MyBreederNo ? (font_col = COL_WHITE) : (font_col = COL_BLACK);
		font_col = BLD_PAL + brd;
	else
	//	brd == MyBreederNo ? (font_col = CON_GRAYPAL) : (font_col = CON_GRAYPAL);
		font_col = CON_GRAYPAL;
	
	//if(YasumiTurnCheck(brd))
	if(apsys[brd].use_waza == 0)
		FontCommonSet(yasumi_msg, font_col);
	else
		FontCommonSet(WazaNameData[apsys[brd].use_waza], font_col);
	
//		MsgPrint(&FightMsg, transparent_msg, CON_WAZA + CON_WAZANAME_LEN * brd,
//				C_RankingPos[brd][0], C_RankingPos[brd][1]);	//�����F�Ÿر
	UsedWorkNameClear(brd);
	MsgPrint(&FightMsg, FightMsgBuf, CON_WAZA + CON_WAZANAME_LEN * brd,
			C_RankingPos[brd][0], C_RankingPos[brd][1]);	//�Z����������
}

void UsedWorkNamePutAll(u8 deep)
{
	u8 i;
	for(i = 0; i < BREEDER_MAX; i++)
		UsedWorkNamePut(i, deep);
}

//----------------------------------------------------------
//	�Z����������Ă��镔�����N���A�[����
//----------------------------------------------------------
void UsedWorkNameClear(u8 brd)
{
	FontScrnBoxFill(&FightMsg, MSG_CLEAR_NO, C_RankingPos[brd][0], C_RankingPos[brd][1],
		C_RankingPos[brd][0]+7, C_RankingPos[brd][1]+1);
}

void UsedWorkNameClearAll(void)
{
	u8 i;
	for(i = 0; i < BREEDER_MAX; i++)
		UsedWorkNameClear(i);
}


//=========================================================================
//	��߰���т̏��ʌ��菈��
//=========================================================================
void TotalAPCalc(void)
{
	u8 i, j;
	s16 ap[BREEDER_MAX];
	s16 temp;
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		apsys[i].total_ap += apsys[i].ap;
		ap[i] = apsys[i].total_ap;
	}

	for(i = 0; i < BREEDER_MAX - 1; i++)	//���
	{
		for(j = BREEDER_MAX - 1; j > i; j--)
		{
			if(ap[j-1] < ap[j])
			{
				temp = ap[j];
				ap[j] = ap[j-1];
				ap[j-1] = temp;
			}
		}
	}
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		for(j = 0; j < BREEDER_MAX; j++)
		{
			if(apsys[i].total_ap == ap[j])
			{
				apsys[i].ranking = j;
				break;
			}
		}
	}

	BreederDataSort(1);
	ConWazaBrdSort();
}

//=========================================================================
//	�^�[���I�����̌���MsgID���擾����
//=========================================================================
void GetTurnKekkaMsgID(void)
{
	int i;
	u8 kekka_msg;
	
	for(i = 0; i < BREEDER_MAX; i++){
	//	if(YasumiTurnCheck(MyBreederNo))
		if(apsys[i].use_waza == 0)
			kekka_msg = SITENAI_MSG;
		else if(apsys[i].ap < 1)
			kekka_msg = ZENZEN_MSG;
		else if(apsys[i].ap < 30)
			kekka_msg = ANMARI_MSG;
		else if(apsys[i].ap < 60)
			kekka_msg = MAAMAA_MSG;
		else if(apsys[i].ap < 80)
			kekka_msg = KANARI_MSG;
		else
			kekka_msg = MONOSUGOKU_MSG;
		apsys[i].kekka_msgid = kekka_msg;
	}
}

//=========================================================================
//	�Z�̓�����ʂ𔭓��ł��邩�`�F�b�N
//   & �Z���o���Ƃ��ɔ���������������
//=========================================================================
u8 WazaCalcFlagCheck(u8 brd)
{
	if(apsys[brd].yasumi_turn || apsys[brd].endAppeal_flg)
		return 0;
	/*
	if(apsys[brd].AtoD_turn)
	{
		if(conwaza_tbl[apsys[brd].use_waza].class > S_CLASS)//A_CLASS)
		{
			apsys[brd].ap += apsys[brd].found_ap;
			apsys[brd].found_ap += apsys[brd].found_ap;
			AppealPutMsgSet(1);
		}
	}*/
	return 1;
}


//=========================================================================
//	�Z�t���O�̃`�F�b�N�ƃN���A
//=========================================================================
void ConWazaFlagCheckClear(void)
{
	u8 i;
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		apsys[i].ap = 0;
		apsys[i].found_ap = 0;
//		apsys[i].add_ap = 0;
//		apsys[i].sub_ap = 0;
		apsys[i].shield_flg = 0;
//		apsys[i].reflect_flg = 0;
//		apsys[i].surru_flg = 0;
//		apsys[i].surru_no = i;
//		apsys[i].SpeUseless_flg = 0;
//		apsys[i].conti_count;	����è�v�Z�ŏ���
//		apsys[i].endAppeal_flg	�������痧�����ςȂ�
//		apsys[i].eachTime_flg	�������痧�����ςȂ�
		if(apsys[i].yasumi_turn > 0)
			apsys[i].yasumi_turn--;
//		if(apsys[i].AtoD_turn > 0)
//			apsys[i].AtoD_turn--;
//		if(apsys[i].combo_turn > 0)
//			apsys[i].combo_turn--;
//		apsys[i].bougai_count	�Z���ʂ̒��ŏ���
//		apsys[i].bougai_flg = 0;
		
		apsys[i].bougai = 0;
		apsys[i].invalid10 = 0;
		apsys[i].invalid = 0;
		apsys[i].Ashield_flg = 0;
		apsys[i].bougai2bai = 0;
		apsys[i].penalty0 = 0;
		apsys[i].miss_flg = 0;
		apsys[i].msgid = 0xff;
		apsys[i].msgid2nd = 0xff;
		apsys[i].bonus_flg = 0;
		apsys[i].combo1_invalid = apsys[i].penalty;
		apsys[i].penalty = 0;
		apsys[i].now_nextsort = 0;
		apsys[i].bonus3bai = 0;
		if(apsys[i].next_yasumi_turn){
			apsys[i].yasumi_turn = 1;
			apsys[i].next_yasumi_turn = 0;
		}
		if(apsys[i].next_endAppeal_flg){
			apsys[i].endAppeal_flg = 1;
			apsys[i].next_endAppeal_flg = 0;
		}
		apsys[i].tensyon_up = 0;
	}

	for(i = 0; i < BREEDER_MAX; i++)
	{
		apsys[i].use_wazaold = apsys[i].use_waza;
		consys->WazaHistory[consys->turn][i] = apsys[i].use_waza;
		consys->TensyonHistory[consys->turn][i] = TensyonCheck(apsys[i].use_waza);
		apsys[i].use_waza = 0;
	}
	ccw->tensyon_stop = 0;
	
	/*
	i = MyBreederNo;
	if(apsys[i].yasumi_turn || apsys[i].endAppeal_flg)
		consys->fusanka = 1;
	else
		consys->fusanka = 0;
	*/
}

//=========================================================================
//	������݋x�݂��`�F�b�N
// �߂�l: 1 = �x��		0 = �x�݂���Ȃ�
//=========================================================================
u8 YasumiTurnCheck(u8 brd)
{
	if(apsys[brd].yasumi_turn || apsys[brd].endAppeal_flg)
		return 1;
	return 0;
}

//=========================================================================
//	�Z���󂯂�(�o����)���ɕ\��������ү���ނ����ް���
// �߂�l�F 0 = �G���[(��Ă���ꏊ���Ȃ�����)   1 = ����I��
//=========================================================================
u8 ConWazaMsgSet(u8 brd_no, u8 msg_no)
{
	u8 i;
	
	if(brd_no != MyBreederNo)
		return 1;
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		if(msg_no == ITUMOYORI_MSG)
		{
			consys->itumoyori_flg = 1;
			return 1;
		}
		else if(msg_no == ITUMOYORI_MUCH_MSG)
		{
			consys->itumoyori_much = 1;
			return 1;
		}
		else if(consys->waza_msg[i] == msg_no)
			return 1;	//���ɓ���ү���ނ��o�^�ς�
		if(consys->waza_msg[i] == 0xff)
		{
			consys->waza_msg[i] = msg_no;
			return 1;
		}
	}
	return 0;
}


//=========================================================================
//	�]���l�擾
//=========================================================================
void GetEvaluationValue(u8 brd)
{
	NijiPoint[brd] = GetNijisinsaPoint(brd);
	ContEvaluation[brd] = ContBP[brd] + NijiPoint[brd];
}

void GetEvaluationValueAll(void)
{
	u8 i;
	
	for(i = 0; i < BREEDER_MAX; i++)
		GetEvaluationValue(i);
	GetContestJyunni();
}

//----------------------------------------------------------
//	2���R���|�C���g���擾
//----------------------------------------------------------
s16 GetNijisinsaPoint(u8 brd)
{
	//return ContAP[brd];
	return (ContAP[brd]*2);
}

//=========================================================================
//	�e��ذ�ް�̍ŏI�I�ȏ��ʂ����o��
//=========================================================================
static void GetContestJyunni(void)
{
	u16 random[BREEDER_MAX];
	int i,t, s;
	int data[BREEDER_MAX][4];	//0:Evaluation 1:BP 2:random 3:BreederNo
	int temp[4];
	
	memset(random, 0, sizeof(random));
	for(i = 0; i < BREEDER_MAX; i++){	//�e��ذ�ް������ђl���擾
		random[i] = pp_rand();
		for(t = 0; t < i; t++){
			if(random[i] == random[t]){
				i--;	//����ђl�����Ԃ����� �����������т������Ȃ���
				break;
			}
		}
	}
	
	for(i = 0; i < BREEDER_MAX; i++){
		data[i][0] = ContEvaluation[i];
		data[i][1] = ContBP[i];
		data[i][2] = random[i];
		data[i][3] = i;
	}

	for(i = 0; i < BREEDER_MAX - 1; i++){
		for(s = BREEDER_MAX - 1; s > i; s--){
			if(EvalSortConditionsCheck(s-1, s, data)){
				temp[0] = data[s-1][0];
				temp[1] = data[s-1][1];
				temp[2] = data[s-1][2];
				temp[3] = data[s-1][3];
				data[s-1][0] = data[s][0];
				data[s-1][1] = data[s][1];
				data[s-1][2] = data[s][2];
				data[s-1][3] = data[s][3];
				data[s][0] = temp[0];
				data[s][1] = temp[1];
				data[s][2] = temp[2];
				data[s][3] = temp[3];
			}
		}
	}

	for(i = 0; i < BREEDER_MAX; i++)
		ContestJyunni[data[i][3]] = i;
}

static u8 EvalSortConditionsCheck(int brd1, int brd2, int data[][4])
{
	u8 ret;
	
	if(data[brd1][0] < data[brd2][0])	//Evaluation
		ret = 1;
	else if(data[brd1][0] > data[brd2][0])	//Evaluation
		ret = 0;
	//���̐��ContEvaluation�������l�̏ꍇ
	else if(data[brd1][1] < data[brd2][1])	//BP
		ret = 1;
	else if(data[brd1][1] > data[brd2][1])	//BP
		ret = 0;
	//���̐��ContEvaluation��ContBP�������l�̏ꍇ
	else if(data[brd1][2] < data[brd2][2])	//random
		ret = 1;
	else
		ret = 0;
	
	return ret;
}


//=========================================================================
//	�u������@���������イ�v��ү���ނ��o��
//=========================================================================
void TuusintyuuMsgPut(void)
{
	ScrY0 = 0;
	ScrY2 = 0;
	
	ContestTalkMsgClear();
//	TalkMsgSet(&FightMsg, FightMsgBuf, CON_MSG, CON_TALKMSG_X, CON_TALKMSG_Y);
	MsgPrint(&win[0], tuusintyuu_msg, CON_MSG, CON_TALKMSG_X, CON_TALKMSG_Y);
}

//=========================================================================
//	�A�s�[�����[�^�[���o���V�[�P���X(���)
// now_ap = �����Z�O��AP	add_ap = �����Z����l(�����ꍇ��ϲŽ��)
// col = Ұ���̶װ�w��		brd_pos = �ǂ���ذ�ް�ɑ΂��čs���̂�
// �߂�l�FҰ���𓮂��������ID
//=========================================================================
u8 AppealMeterTaskSet(s16 now_ap, s16 add_ap, u8 col, u8 brd_pos)
{
	u8 id;
	s8 add_meter;
	u8 now_meter;
	u16 base_scrn;
	
	brdsys[brd_pos].meter_eff = 1;
	
	id = AddTask(AppealMeterPutSeq, 20);
	now_meter = GetMeterNum(now_ap);
	add_meter = GetMeterNum(now_ap+add_ap) - now_meter;
	base_scrn = GetBaseMeterScrn(brd_pos);
	TaskTable[id].work[0] = now_meter;
	TaskTable[id].work[1] = add_meter;
	TaskTable[id].work[2] = base_scrn + col;
	TaskTable[id].work[3] = brd_pos;
	
	if(add_ap < 0)	//ϲŽ�Ȃ�Ұ�����~���I������O�ɐ�ɓ_�ł����������Ă��܂�
		MeterColorFadeCheck(brd_pos);

	return id;
}

static void AppealMeterPutSeq(u8 id)
{
	u8 brd_pos;
	u8 pos, i;
	
	brd_pos = TaskTable[id].work[3];

	if(TaskTable[id].work[1] == 0)
	{
		MeterColorFadeCheck(brd_pos);
		//TaskTable[id].TaskAdrs = DummyTaskSeq;
		DelTask(id);
		brdsys[brd_pos].meter_eff = 0;
		return;
	}
	if(++TaskTable[id].work[10] < 30)	//14)
		return;
	
	TaskTable[id].work[10] = 0;
	if(TaskTable[id].work[1] < 0)
	{
		pos = TaskTable[id].work[0]--;
		TaskTable[id].work[1]++;
		SePlay(SE_BOO);
	}
	else
	{
		pos = ++TaskTable[id].work[0];
		TaskTable[id].work[1]--;
		SePlay(SE_PIN);
	}
	
	if((u16)TaskTable[id].work[2] != 0xffff)
	{
		DmaTransReq((u8*)&TaskTable[id].work[2],
			SCREEN_BASE24_ADDR + (32*4 + 19 + pos + (32*5)*brd_pos)*2, 2);
	}
	else
	{
		for(i = 0; i < BREEDER_MAX-1; i++)
		{
			if(TaskTable[id].work[4+i] < 0)
			{
				DmaTransReq((u8*)&TaskTable[id].work[7+i],
					SCREEN_BASE24_ADDR + (32*4 + 19 + pos + (32*5)*brd_pos)*2, 2);
				TaskTable[id].work[4+i]++;
				break;
			}
		}
	}
}

//----------------------------------------------------------
//	�A�s�[�����[�^�[��ر�����
//----------------------------------------------------------
void AppealMeterClear(void)
{
	u8 i;

	for(i = 0; i < BREEDER_MAX; i++){
		DIV_DMACLEAR(3, HEART_CLEAR_SCRN, SCREEN_BASE24_ADDR+(32*2+0x16+(32*5)*i)*2, 16, 16);
		DIV_DMACLEAR(3, HEART_CLEAR_SCRN, SCREEN_BASE24_ADDR+(32*3+0x16+(32*5)*i)*2, 16, 16);
	}
}

//----------------------------------------------------------
//	Ұ����S�Ĕ��Ŗ��߂�
//----------------------------------------------------------
void AppealMeterWhiteSet(s16 ap, u8 brd)
{
	u8 meter, i;
	u16 base_scrn;
	u16 trans_scrn[9];
	
	meter = GetMeterNum(ap);
	if(brd == 0)
		base_scrn = BASE_METER_SCRN0;
	else if(brd == 1)
		base_scrn = BASE_METER_SCRN1;
	else if(brd == 2)
		base_scrn = BASE_METER_SCRN2;
	else
		base_scrn = BASE_METER_SCRN3;
	base_scrn += METER_WHITE;
	
	for(i = 0; i < 9; i++)
	{
		if(i < meter)
			trans_scrn[i] = base_scrn;
		else
			trans_scrn[i] = MSG_CLEAR_NO;
	}
	DIV_DMACOPY(3, (u8*)trans_scrn,	SCREEN_BASE24_ADDR + (32*4+20+(32*5)*brd)*2, 18, 16);
}

//----------------------------------------------------------
//	��ذ�ްNo�����ް��̽�ذ��ް������o��
//----------------------------------------------------------
static u16 GetBaseMeterScrn(u8 brd)
{
	u16 base_scrn;
	
	if(brd == 0)
		base_scrn = BASE_HEART_SCRN0;
	else if(brd == 1)
		base_scrn = BASE_HEART_SCRN1;
	else if(brd == 2)
		base_scrn = BASE_HEART_SCRN2;
	else
		base_scrn = BASE_HEART_SCRN3;
	
	return base_scrn + 1;	//��ɔZ���̂�\������悤�ɕύX
}

//----------------------------------------------------------
//	AP�l���牽��Ұ����������Ԃ�
//----------------------------------------------------------
static s8 GetMeterNum(s16 ap)
{
	s8 ret;
	
	ret = ap / METER_ONE_AP;	//5;
	if(ret > 16)
		ret = 16;
	else if(ret < -16)
		ret = -16;

//	return abs(ret);
	return ret;
}

//=========================================================================
//	�A�s�[���n�[�g���o���V�[�P���X(���)
// now_ap = �����Z�O��AP	add_ap = �����Z����l(�����ꍇ��ϲŽ��)
// brd_pos = �ǂ���ذ�ް�ɑ΂��čs���̂�
// �߂�l�FҰ���𓮂��������ID
//=========================================================================
u8 AppealHeartTaskSet(s16 now_ap, s16 add_ap, u8 brd_pos)
{
	u8 id;
	s8 add_meter;
	s8 now_meter;
	u16 base_scrn;
	
	brdsys[brd_pos].meter_eff = 1;
	
	id = AddTask(AppealHeartPutSeq, 20);
	now_meter = GetMeterNum(now_ap);
	add_meter = GetMeterNum(now_ap+add_ap) - now_meter;
	base_scrn = GetBaseMeterScrn(brd_pos);

	TaskTable[id].work[0] = abs(now_meter);
	TaskTable[id].work[1] = add_meter;
	if(now_meter > 0 || (now_meter == 0 && add_meter > 0))
		TaskTable[id].work[2] = 1;
	else
		TaskTable[id].work[2] = -1;
	TaskTable[id].work[3] = brd_pos;

//	TaskTable[id].work[2] = base_scrn;	//  + col;
	
	if(add_ap < 0)	//ϲŽ�Ȃ�Ұ�����~���I������O�ɐ�ɓ_�ł����������Ă��܂�
		MeterColorFadeCheck(brd_pos);

	return id;
}

//----------------------------------------------------------
//	�n�[�g���[�^�[�������Ă���
//----------------------------------------------------------
static void AppealHeartPutSeq(u8 id)
{
	u8 brd_pos;
	u8 pos;
	s16 now, addsub;
	u16 scrn;
	s8 meter_pos;
	u8 sepos;
	
	brd_pos = TaskTable[id].work[3];
	now = TaskTable[id].work[0];
	addsub = TaskTable[id].work[1];
	
	if(++TaskTable[id].work[10] < METER_WAIT)	//14)
		return;
	TaskTable[id].work[10] = 0;

	if(TaskTable[id].work[1] == 0)
	{
		MeterColorFadeCheck(brd_pos);
		//TaskTable[id].TaskAdrs = DummyTaskSeq;
		DelTask(id);
		brdsys[brd_pos].meter_eff = 0;
		return;
	}

	if(now == 0){
		if(addsub < 0){
			scrn = GetBaseMeterScrn(brd_pos) + 2;
//			if(now+addsub < -9)
//				scrn++;	//�Z���n�[�g�̐F��\��������
			meter_pos = -1;
			TaskTable[id].work[1]++;
		}else{
			scrn = GetBaseMeterScrn(brd_pos);
//			if(now+addsub > 9)
//				scrn++;	//�Z���n�[�g�̐F��\��������
			meter_pos = 1;
			TaskTable[id].work[1]--;
		}
		pos = TaskTable[id].work[0]++;
	}else if(TaskTable[id].work[2] < 0){		//(now < 0){
		meter_pos = -1;
		if(addsub < 0){
			pos = TaskTable[id].work[0]++;
			TaskTable[id].work[1]++;

			scrn = GetBaseMeterScrn(brd_pos) + 2;
//			if(now+addsub < -8)
//				scrn++;	//�Z���n�[�g�̐F��\��������
		}else{
			pos = --TaskTable[id].work[0];
			scrn = HEART_CLEAR_SCRN;
			TaskTable[id].work[1]--;
		}
	}else{
		meter_pos = 1;
		if(addsub < 0){
			pos = --TaskTable[id].work[0];
			scrn = HEART_CLEAR_SCRN;
			TaskTable[id].work[1]++;
		}else{
			pos = TaskTable[id].work[0]++;
			TaskTable[id].work[1]--;

			scrn = GetBaseMeterScrn(brd_pos);
//			if(now+addsub > 8)
//				scrn++;	//�Z���n�[�g�̐F��\��������
		}
	}
	
	sepos = pos;
	if(pos > 7){
		//pos -= 8;
//		if(scrn != HEART_CLEAR_SCRN)
//			scrn++;		//�Z���F�̂�\��
		pos += -8 + 0x20;	//���i�Ɉړ�
	}
	CpuClear(scrn, SCREEN_BASE24_ADDR + 
			(32*2 + 0x16 + pos + (32*5)*BrdSort[brd_pos])*2, 2, 16);

	if(addsub > 0){
		//SePlay(SE_PIN);
		SePlay(SE_C_GAJI);
		m4aMPlayImmInit(&m4a_mplay001);
		m4aMPlayPitchControl(&m4a_mplay001, 0xffff, 256*sepos);
	}
	else{
		SePlay(SE_BOO);
	}
	
	if(pos == 0 && scrn == HEART_CLEAR_SCRN)
		TaskTable[id].work[2] = -TaskTable[id].work[2];
}


//=========================================================================
//	�n�[�g�̃A�C�R���Ō��݂̃g�[�^��AP�l������
//=========================================================================
//----------------------------------------------------------
//	�n�[�g�A�C�R���̃A�N�^�[��Cell&PalSet
//----------------------------------------------------------
void HeartIconAllSet(void)
{
	int i;
	
	CellSet((CellData *)&HeartIconCell);
//	ObjPalSet((PalData*)&HeartIconPal);
	for(i = 0; i < BREEDER_MAX; i++)
		brdsys[i].heart_actno = AddActor(&HeartIconHeader, HEARTICON_X, 
			HeartIconYPos[BrdSort[i]], 1);
}

//----------------------------------------------------------
//	�n�[�g�A�C�R�������݂�total_ap�̈ʒu�܂œ�����
//		���쒆��heart_move�t���O������	���삪�I��������0�ɂȂ�
//----------------------------------------------------------
void SetHeartIconMove(u8 brd)
{
	u8 actno;
	s16 dotto;
	
	brdsys[brd].heart_move = 1;
	actno = brdsys[brd].heart_actno;
	dotto = apsys[brd].total_ap / METER_ONE_AP * 2;	//5;

	if(dotto > 56)
		dotto = 56;
	else if(dotto < 0)
		dotto = 0;

	ActWork[actno].banish = 0;
	ActWork[actno].work[0] = brd;
	ActWork[actno].work[1] = dotto;
	if(dotto > ActWork[actno].dx)
		ActWork[actno].work[2] = 1;
	else
		ActWork[actno].work[2] = -1;
	ActWork[actno].move = HeartIconMoveSeq;
}

//----------------------------------------------------------
//	�S�Ă���ذ�ް�̃n�[�g�A�C�R���ɓ��얽�߂��o��
//----------------------------------------------------------
void SetHeartIconMoveAll(void)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++)
		SetHeartIconMove(i);
}

//----------------------------------------------------------
//	�S�Ă���ذ�ް�̃n�[�g�A�C�R���̓��삪�I�����Ă��邩���ׂ�
//	�߂�l: 0=�܂����쒆	1=�S�Ẵn�[�g�A�C�R��������I�����Ă���
//----------------------------------------------------------
u8 CheckHeartIconMoveAll(void)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++){
		if(brdsys[i].heart_move)
			break;
	}
	if(i == BREEDER_MAX)
		return 1;
	return 0;
}

//----------------------------------------------------------
//	�n�[�g�A�C�R���̓���V�[�P���X(�g�[�^��AP�l�܂œ���)
//----------------------------------------------------------
static void HeartIconMoveSeq(actWork *xreg)
{
	if(xreg->dx == xreg->work[1]){
		brdsys[xreg->work[0]].heart_move = 0;
		xreg->move = DummyActMove;
	}else
		xreg->dx += xreg->work[2];
}

//----------------------------------------------------------
//	ʰı��݂�Y���W���ăZ�b�g
//----------------------------------------------------------
void HeartIconYposSet(void)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++)
		ActWork[brdsys[i].heart_actno].y = HeartIconYPos[BrdSort[i]];
}

//----------------------------------------------------------
//	��ʉ�2��ʰı��݂���ʊO�ɑޔ���������߂�����
// ����: 0 = ��ʓ��ɖ߂�	1 = ��ʊO�ɑޔ�������
//----------------------------------------------------------
void HearIconEva(u8 evac)
{
	int i;
	
	for(i = 0; i < BREEDER_MAX; i++){
		if(BrdSort[i] > 1){
			if(evac == 0)
				ActWork[brdsys[i].heart_actno].x = HEARTICON_X;	//��ʓ���
			else
				ActWork[brdsys[i].heart_actno].x = 256;	//��ʊO��
		}
	}
}

//=========================================================================
//	�u���H�΂�v�̃A�C�R����Cell&Pal&AddActor
//=========================================================================
void SortIconAllSet(void)
{
	int i;
	
	ObjPalSet((PalData*)&SortIconPal);
	for(i = 0; i < BREEDER_MAX; i++){
		DecordCellSet((CellData *)&SortIconCell[i]);
		brdsys[i].sorticon_actno = AddActor(&SortIconHeader[i], SORTICON_X, 
			SortIconYPos[BrdSort[i]], 0);
		SetActOamPat(&ActWork[brdsys[i].sorticon_actno], &SortIconPat);
		ActWork[brdsys[i].sorticon_actno].banish = 1;
	}
}

//=========================================================================
//	�{���e�[�W�A�C�R���A�N�^�[�̾��
//=========================================================================
#define VOLTAGE_GAUGE_X		30//27
#define VOLTAGE_GAUGE_Y		44	//16
#define VOLTAGE_ICON_X		(VOLTAGE_GAUGE_X - 16)
#define VOLTAGE_ICON_Y		VOLTAGE_GAUGE_Y
void VoltageIconActorSet(void)
{
//	u8 actno;
	u8 gauge1;	//, gauge2;
	
	//-- ���̃Q�[�W���� --//
	DecordCellSet((CellData*)&VoltageGaugeCell);
//	DecordPalSet((PalData*)&VoltageGaugePal);
	ObjPalSet((PalData*)&VoltageGaugePal);
	gauge1 = AddActor(&VoltageGaugeHeader, VOLTAGE_GAUGE_X, VOLTAGE_GAUGE_Y, 1);
//	gauge2 = AddActor(&VoltageGaugeSubHeader, 
//		VOLTAGE_GAUGE_X+32+4, VOLTAGE_GAUGE_Y,1);	//�Q�[�W�E�[����
//	ActWork[gauge2].oamData.CharNo += 32;
	ActWork[gauge1].banish = 1;
//	ActWork[gauge2].banish = 1;
	consys->volgauge_act = gauge1;
//	consys->volgaugesub_act = gauge2;
}

//----------------------------------------------------------
//	���ð�ޱ��݂��ݼ�݂ɂ���ı�҂�ύX����
//	0=����	1�ȏ�=�Ί�	-1�ȉ�=������
//----------------------------------------------------------
void VoltageIconAnmSet(s8 tensyon)
{
}


//=========================================================================
//	���ʱ��݂�پ��&��پ��&AddActor����(banish����̫�Ă�ON�ɂȂ�)
//=========================================================================
void JyunniIconAllDataSet(void)
{
	u8 i, actno;
	
	DecordCellSet((CellData*)&JyunniIconCell);
	for(i = 0; i < BREEDER_MAX; i++)
		DecordPalSet((PalData*)&JyunniIconPal[i]);
	for(i = 0; i < BREEDER_MAX; i++)
	{
		actno = AddActor(&JyunniIconHeader[i], 
			C_JyunniIconPos[i][0], C_JyunniIconPos[i][1], 5);
		ActWork[actno].banish = 1;
		ActWork[actno].work[0] = i;
		consys->jyunni_icon[i] = actno;
	}
}

//=========================================================================
//	���ʂ�\��(�ύX)����̪�Ă��o��
//=========================================================================
void JyunniIconPut(void)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++)
		ActWork[consys->jyunni_icon[i]].move = JyunniIconEffectInitSeq;
}

//----------------------------------------------------------
//	�̪�ďI������
// �߂�l: 0 = �܂��̪�Ē�     1 = �̪�ďI��
//----------------------------------------------------------
u8 JyunniIconEffectEndCheck(void)
{
	int i;
	for(i = 0; i < BREEDER_MAX; i++)
	{
		if(ActWork[consys->jyunni_icon[i]].move != DummyActMove)
			break;
	}
	if(i == BREEDER_MAX)
		return 1;
	return 0;
}

//=========================================================================
//	���ʱ��݂̃A�N�^�[�V�[�P���X
//=========================================================================
static void JyunniIconEffectInitSeq(actWork *Xreg)
{
	Xreg->oamData.AffineMode = 1;
	AffineNumSet(Xreg);
	if(Xreg->banish == 1)
		Xreg->move = JyunniIcon_StoB_Init;
	else
	{
		ActAffAnmChg(Xreg, 1);
		Xreg->move = JyunniIcon_BtoS_Init;
//		SePlay(SE_JYUNI);
	}
}

//-- �ʏ��Ԃ��珬���� --//
static void JyunniIcon_BtoS_Init(actWork *Xreg)
{
	if(Xreg->affend_sw == 0)
		return;
		
	Xreg->banish = 1;
	Xreg->move = JyunniIcon_StoB_Init;
}

//-- ��������Ԃ���ʏ��Ԃ� --//
static void JyunniIcon_StoB_Init(actWork *Xreg)
{
	Xreg->banish = 0;
	ActAnmChg(Xreg, apsys[Xreg->work[0]].ranking);
	ActAffAnmChg(Xreg, 2);
	Xreg->move = JyunniIcon_StoB;
	SePlay(SE_JYUNI);
}

static void JyunniIcon_StoB(actWork *Xreg)
{
	if(Xreg->affend_sw == 0)
		return;
	
	AffineDelActor(Xreg);
	Xreg->oamData.AffineMode = 0;
	Xreg->move = DummyActMove;
}


//=========================================================================
//	����1���s���̖ڱ��� �װ���ŏ���
//=========================================================================
//----------------------------------------------------------
//	�����ݒ�
//----------------------------------------------------------
void EyeColorTaskSet(void)
{
	u8 id, i;
	
	id = AddTask(EyeColorFadeSeq, 30);
	consys->eye_color = id;
	for(i = 0; i < BREEDER_MAX; i++)
		TaskTable[id].work[i*4] = 0xff;
}

//----------------------------------------------------------
//	̪��ފJ�nظ��Ċ֐�
//----------------------------------------------------------
void EyeColorFadeStart(u8 brd_pos)
{
	TaskTable[consys->eye_color].work[brd_pos*4] = 0;
	TaskTable[consys->eye_color].work[brd_pos*4+1] = 0;
}

//----------------------------------------------------------
//	̪��ޒ�~ظ��Ċ֐�
//----------------------------------------------------------
void EyeColorFadeStop(u8 brd_pos)
{
	u8 id;
	
	id = AddTask(EyeColorFadeStopMain, 31);
	TaskTable[id].work[0] = brd_pos;
}

static void EyeColorFadeStopMain(u8 id)
{
	u8 brd_pos;
	
	brd_pos = TaskTable[id].work[0];
	//̪��ނ�evy��0�ɂȂ��������A���X̪��ނ���~��ԂȂ�į�߂������Del
	if(TaskTable[consys->eye_color].work[brd_pos*4] == 0 ||
		TaskTable[consys->eye_color].work[brd_pos*4] == 0xff)
	{
		TaskTable[consys->eye_color].work[brd_pos*4] = 0xff;
		TaskTable[consys->eye_color].work[brd_pos*4+1] = 0;
		SoftFade(16*(5+consys->nowturn_sort[brd_pos]) + 6, 2, 0, 0x4bff);
		DelTask(id);
	}
}

//----------------------------------------------------------
//	���C���V�[�P���X
//----------------------------------------------------------
static void EyeColorFadeSeq(u8 id)
{
	u8 base, i;
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		base = i * 4;
		if(TaskTable[id].work[base] != 0xff)
		{
			if(TaskTable[id].work[base+1] == 0)
				TaskTable[id].work[base]++;
			else
				TaskTable[id].work[base]--;
			if(TaskTable[id].work[base] == 16 || TaskTable[id].work[base] == 0)
				TaskTable[id].work[base+1] ^= 1;
			SoftFade(16*(5+consys->nowturn_sort[i]) + 6, 2, TaskTable[id].work[base], 0x4bff);
		}
	}
}

//=========================================================================
//	��߰�Ұ��Max����Ұ���_�ŏ���
//=========================================================================
//----------------------------------------------------------
//	�����Z�b�g
//----------------------------------------------------------
void MeterColorTaskSet(void)
{
	u8 id, i;
	
	id = AddTask(MeterColorFadeSeq, 30);
	consys->meter_color = id;
	for(i = 0; i < BREEDER_MAX; i++)
		MeterColorFlagClear(i);
}

//----------------------------------------------------------
//	�ް�ނ̓_���׸ނ�ر����
//----------------------------------------------------------
void MeterColorFlagClear(u8 brd_pos)
{
	TaskTable[consys->meter_color].work[brd_pos*4] = 0xff;
	TaskTable[consys->meter_color].work[brd_pos*4+1] = 0;
}

void MeterColorClearAll(void)
{
	u8 i;
	for(i = 0; i < BREEDER_MAX; i++)
		MeterDefaultColorSet(i);
}

static void MeterDefaultColorSet(u8 brd_pos)
{
	MeterColorFlagClear(brd_pos);
	DIV_DMACOPY(3, &PaletteWork[16*(5+brd_pos) + 0xa],
		&PaletteWorkTrans[16*(5+brd_pos) + 0xa], 2,16);
	DIV_DMACOPY(3, &PaletteWork[16*(5+brd_pos) + 0xc+brd_pos],
		&PaletteWorkTrans[16*(5+brd_pos) + 0xc+brd_pos], 2,16);
}

//----------------------------------------------------------
//	Ұ���_�� �J�n�A�I����������
//----------------------------------------------------------
static void MeterColorFadeCheck(u8 brd_pos)
{
#if 1	// 2002.06.28(��)
	return;
#else
	u8 id, base;
	
	id = consys->meter_color;
	base = brd_pos*4;
	if(apsys[brd_pos].ap > 80 && TaskTable[id].work[base] == 0xff)
	{
		TaskTable[id].work[base] = 0;
		TaskTable[id].work[base+1] = 0;
	}
	else if(apsys[brd_pos].ap <= 80 && TaskTable[id].work[base] != 0xff)
	{
		MeterDefaultColorSet(brd_pos);
	}
#endif
}

//----------------------------------------------------------
//	�_�ŏ��� ���C���V�[�P���X
//----------------------------------------------------------
static void MeterColorFadeSeq(u8 id)
{
	u8 i, base;
	
	for(i = 0; i < BREEDER_MAX; i++)
	{
		base = i*4;
		if(TaskTable[id].work[base] != 0xff)
		{
			if(++TaskTable[id].work[base+2] < 3)
				continue;
			TaskTable[id].work[base+2] = 0;

			if(TaskTable[id].work[base+1] == 0)
				TaskTable[id].work[base]++;
			else
				TaskTable[id].work[base]--;
			if(TaskTable[id].work[base] == 16 || TaskTable[id].work[base] == 0)
				TaskTable[id].work[base+1] ^= 1;
			SoftFade(16*(5+i) + 0xa, 1, TaskTable[id].work[base], 0x4bff);
			SoftFade(16*(5+i) + 0xc+i, 1, TaskTable[id].work[base], 0x4bff);
		}
	}
}

//=========================================================================
//	��ٌ߰��ʂ�AP���������A�������Ŷװ�_�ł�����
// *color_flg = �e��ذ�ް�ɑ΂��āA�ǂ̶װ�_�ŏ��������邩
//				0:�������Ȃ�	1:���邭�_��	2:�Â��_��
//=========================================================================
u8 AppealKoukaColorTaskSet(u8 *color_flg)
{
	u8 id, i;
	u8 base;
	
	id = AddTask(AppealKoukaColorSeq, 10);
	for(i = 0; i < BREEDER_MAX; i++)
	{
		base = i*4;
		TaskTable[id].work[base] = color_flg[i];
		if(color_flg[i] != 0)
			brdsys[i].win_eff = 1;
	}
	return id;
}

static const u16 WinBlackDicColor[] = {
	0x0073, 0x19e6, 0x0253, 0x4cc4
};
static const u16 WinWhiteDicColor[] = {
	0x001f, 0x03e0, 0x3ff, 0x7c00
};
static void AppealKoukaColorSeq(u8 id)
{
	u8 base, evy, counter;
	u8 i, direction, temp;
	u8 loop;
	
	for(loop = 0; loop < BREEDER_MAX; loop++)
	{
		i = BrdSort[loop];
		
		base = i*4;
		direction = TaskTable[id].work[base];
		if(direction == 0)
			continue;
		counter = TaskTable[id].work[base+1];
		evy = TaskTable[id].work[base+2];
		temp = TaskTable[id].work[base+3];
		if(direction == 1)
		{
			if(++temp == 1)
			{
				temp = 0;
				SoftFade(16*(5+i) + 1, 3, evy, 0x7fff);//WinWhiteDicColor[i]);
				if(evy == 0 && counter == 4)
					TaskTable[id].work[base] = 0;
				else
				{
					evy += 2;
					if(evy >= 14)
					{
						evy = 0;
						counter++;
					}
				}
			}
		}
		else if(direction == 2 || direction == 4)
		{
			if(++temp == 3)
			{
				temp = 0;
				SoftFade(16*(5+i) + 1, 3, evy, WinBlackDicColor[i]);
				if(evy == 0 && counter == 2)
					TaskTable[id].work[base] = 0;
				else
				{
					if(++evy == 14)
					{
						evy = 0;
						counter++;
						if(direction == 4 && counter == 1)
						{
							SoftFade(16*(9+i)+2, 1, 4, 0);
							SoftFade(16*(9+i)+5, 1, 4, 0);
						}
					}
				}
			}
		}
		else if(direction == 3)	//1��x�݂̏���
		{
			if(++temp == 12)
			{
				temp = 0;
				SoftFade(16*(5+i) + 1, 3, evy , 0);
				if(++evy == 5)
				{
					DIV_DMACOPY(3, PaletteWorkTrans[16*(5+i)+1], PaletteWork[16*(5+i)+1],
						6, 16);
					TaskTable[id].work[base] = 0;
				}
			}
		}
		TaskTable[id].work[base+1] = counter;
		TaskTable[id].work[base+2] = evy;
		TaskTable[id].work[base+3] = temp;
	}
	for(i = 0; i < BREEDER_MAX; i++)
	{
		if(TaskTable[id].work[i*4] != 0)
			break;
	}
	if(i == BREEDER_MAX)
	{
		for(i = 0; i < BREEDER_MAX; i++)
			brdsys[i].win_eff = 0;
		DelTask(id);
//		TaskTable[id].TaskAdrs = DummyTaskSeq;
	}
}

//=========================================================================
//	�u�t�V�M�_�l�́@�A�s�[���������I�v��ү���ނ��o��
// put_flg = 0:ү���ނ͏o���Ȃ�		1:ү���ނ��o��
//=========================================================================
void AppealPutMsgSet(u8 put_flg)
{
	if(put_flg)
	{
	//	TalkMsgSet(&FightMsg, wazaput_msg, CON_MSG, CON_TALKMSG_X, CON_TALKMSG_Y);
		//MsgPrint(&FightMsg, appealkouka_msg, CON_MSG, CON_TALKMSG_X, CON_TALKMSG_Y);
		consys->appeal_msg = 1;
	}
//	else	�Z���ʑO��WazaCalcFlagCheck�ŌĂ΂�āA���̌���ʂŌĂ΂�鎖������̂�
//		consys->appeal_msg = 0;	������0�;�Ă��Ȃ�
}

//=========================================================================
//	���ޏ�Ԃ��������ē_�ł���������Z�b�g�����肷��
//=========================================================================
void EyeColorStatusCheckSet(u8 brd)
{
//	if(ConWazaTbl[apsys[brd].use_waza].combo1 != 0)
	if(apsys[brd].combo1_flg)
		EyeColorFadeStart(brd);
	else
		EyeColorFadeStop(brd);
}


//=========================================================================
//	�e��ذ�ް�̳���޳��OBJ�ɗ��Ƃ�����
//�@�߂�l: actno
//=========================================================================
u8 BreederWindowObjSet(u8 brd)
{
	u8 actno, actno2;
	u8 xpos, ypos;
	vu8 dma_wait;
	
	xpos = 184;	//0xa8;
	ypos = 0x20 + BrdSort[brd] * 40;
	DecordCellSet((CellData*)&BrdWinCell[brd]);
	ObjPalSet((PalData*)&BrdWinPal[brd]);
	actno = AddActor(&BrdWinActHeader[brd], xpos, ypos, C_POKE1PRI-1);
	actno2 = AddActor(&BrdWinActHeader[brd], xpos+64, ypos, C_POKE1PRI-1);
	ActWork[actno2].oamData.CharNo += 64;
	BG_to_OBJ(0, 3, (void*)CHAR_BASE0_ADDR, 
			(void*)(SCREEN_BASE28_ADDR + 0x13*2 + 0x20*5*BrdSort[brd]*2),
			(u8*)((ActWork[actno].oamData.CharNo * 32) + OBJ_MODE0_VRAM));
	BG_to_OBJ(0, 3, (void*)CHAR_BASE0_ADDR, 
			(void*)(SCREEN_BASE28_ADDR + (0x13+8)*2 + 0x20*5*BrdSort[brd]*2),
			(u8*)((ActWork[actno2].oamData.CharNo * 32) + OBJ_MODE0_VRAM));
	DIV_DMACLEAR(3, 0, (ActWork[actno].oamData.CharNo+8*5) * 32 + OBJ_MODE0_VRAM, 3*8*32, 32);
	dma_wait = 0;	dma_wait = 0;
	DIV_DMACLEAR(3, 0, (ActWork[actno2].oamData.CharNo+8*5) * 32 + OBJ_MODE0_VRAM, 3*8*32, 32);
	ActWork[actno].work[0] = actno2;
	ActWork[actno2].work[0] = actno;
	ActWork[actno].work[1] = brd;
	ActWork[actno2].work[1] = brd;
	return actno;
}

//----------------------------------------------------------
//	����޳��OBJ�ɗ��Ƃ����ɾ�Ă����A�فA��گāA�����S�Ă�DEL����
// actno = actno 0,1 �ǂ���ł�����(�فA��گċ��ɓ������̂��g�p���Ă���̂�)
//----------------------------------------------------------
void BrdWinObjDel(u8 actno)
{
	u8 actno2;
	
	actno2 = ActWork[actno].work[0];
	AffineDelActor(&ActWork[actno2]);
	DelActor(&ActWork[actno2]);
	AllDelActor(&ActWork[actno]);
}

void BrdWinAlphaRegSet(void)
{
	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL;
	*(vu16*)REG_BLDALPHA = 7 | (9<<8);	//5 | (11<<8);
}
void BrdWinAlphaRegReset(void)
{
	*(vu16*)REG_BLDCNT = 0;
	*(vu16*)REG_BLDALPHA = 0;
}

//----------------------------------------------------------
//	se_flg = 0:�Z���o���Ƃ���SE		1:�A�s�[�����ʂ̎���SE
//----------------------------------------------------------
void BrdWinWazaPutEffSet(u8 actno, u8 se_flg)
{
	u8 actno2;
	
	BrdWinAlphaRegSet();

	brdsys[ActWork[actno].work[1]].win_eff = 1;
	actno2 = ActWork[actno].work[0];
	ActAffAnmChg(&ActWork[actno], 1);
	ActAffAnmChg(&ActWork[actno2], 1);
	ActWork[actno].move = BrdWinAffEndSeq;
	ActWork[actno2].move = DummyActMove;
	if(se_flg == 0)
		SePlay(SE_C_PIKON);
	else
		SePlay(SE_PC_LOGIN);
}

static void BrdWinAffEndSeq(actWork *Xreg)
{
	u8 actno2;
	
	if(Xreg->affend_sw == 0)
		return;
	
	actno2 = Xreg->work[0];
	if(ActWork[actno2].affend_sw)
	{
		//������ڼ޽������̂��Ă��܂��ƈ�u�������łȂ��̂������Ă��܂��̂�
		Xreg->banish = 1;
		ActWork[actno2].banish = 1;
		Xreg->move = BrdWinRegClearEnd;
	}
}

static void BrdWinRegClearEnd(actWork *Xreg)
{
	brdsys[Xreg->work[1]].win_eff = 0;
//	BrdWinObjDel(ActWork[actno2].work[0]);
	BrdWinObjDel(Xreg->work[0]);//�����ǂ����ł�OK
	BrdWinAlphaRegReset();
}


//=========================================================================
//	�Z���o����ذ�ް�̳���޳�𖾂邭����(�Ɩ��𗎂Ƃ��O�̐F�ɖ߂�)
// houkou : 0=����޳���邭     1=����޳�Â�(�Ɩ��𗎂Ƃ������̐F)
//=========================================================================
void WazaPutBrdWinLight(u8 brd, u8 houkou)
{
	brdsys[brd].win_eff = 0;
}



//=========================================================================
//	�e�̖��O�ƌ��݂�İ��AP��؂�ւ��ĕ\������
//			���f�o�b�O�p
//=========================================================================
void DebugAPPut(void)
{
	u8 i;
	
	UserWork[DEBUG_AP] ^= 1;

	if(UserWork[DEBUG_AP] == 0)
	{
		for(i = 0; i < BREEDER_MAX; i++)
			FontScrnBoxFill(&FightMsg, MSG_CLEAR_NO, C_PokeNamePos[i][0],
				C_PokeNamePos[i][1], C_BreederPos[i][0]+5, C_BreederPos[i][1]+1);
		PutPlayerInfo();
		PlayerInfoCopy();
	}
	else
		DebugAPUpdatingCheckPut();
}

//----------------------------------------------------------
//	AP�\���׸ނ���������AP�̒l���X�V
//----------------------------------------------------------
void DebugAPUpdatingCheckPut(void)
{
	u8 i;
	u8 buff[8];
	u8 count = 0;
	s16 total_ap, ap;

	if(UserWork[DEBUG_AP] == 0)
		return;

	for(i = 0; i < BREEDER_MAX; i++)
		FontScrnBoxFill(&FightMsg, MSG_CLEAR_NO, C_PokeNamePos[i][0],
			C_PokeNamePos[i][1], C_BreederPos[i][0]+5, C_BreederPos[i][1]+1);
		
	//İ��AP
	for(i = 0; i < BREEDER_MAX; i++)
	{
		total_ap = apsys[i].total_ap;
		if(total_ap < 0)
		{
			total_ap = abs(total_ap);
			buff[0] = bou_;
			count++;
		}
		PM_NumMsgSet(&buff[count], total_ap, NUM_MODE_LEFT, 4);
		MsgPrint(&FightMsg, buff, CON_TRAINER + CON_TRAINER_LEN*BrdSort[i],
			C_BreederPos[BrdSort[i]][0], C_BreederPos[BrdSort[i]][1]);
		count = 0;
	}
	//AP
	for(i = 0; i < BREEDER_MAX; i++)
	{
		ap = apsys[i].ap;
		if(ap < 0)
		{
			ap = abs(ap);
			buff[0] = bou_;
			count++;
		}
		PM_NumMsgSet(&buff[count], ap, NUM_MODE_LEFT, 4);
		MsgPrint(&FightMsg, buff, CON_POKENAME + CON_POKE_LEN * BrdSort[i], 
			C_PokeNamePos[BrdSort[i]][0], C_PokeNamePos[BrdSort[i]][1]);
		count = 0;
	}
	PlayerInfoCopy();
}

void sort(int a[], int n)
{
	int i,j,t;

	for(i = 0; i < n - 1; i++){
		for(j = n - 1; j > i; j--){
			if(a[j-1] > a[j]){
				t = a[j];
				a[j] = a[j-1];
				a[j-1] = t;
			}
		}
	}
}

//=========================================================================
//	���݂�İ��AP�l�A1���R���̏��ʂ���ɂ�����ذ�ް�ް��̕��ёւ����s��
//	�����Fsort_type = 0:1���R���̏��ʂ���ɕ��ёւ�	1:İ��AP����ɕ��ёւ�
//=========================================================================
void BreederDataSort(u8 sort_type)
{
//	s16 now;
//	u8 no,douritu;
	int i,s,t;
	u8 rank;
	u8 tempSort[BREEDER_MAX];
	u16 random[BREEDER_MAX];
	u8 temp;
	
	memset(random, 0, sizeof(random));
	for(i = 0; i < BREEDER_MAX; i++){	//�e��ذ�ް������ђl���擾
		random[i] = pp_rand();
		for(t = 0; t < i; t++){
			if(random[i] == random[t]){
				i--;	//����ђl�����Ԃ����� �����������т������Ȃ���
				break;
			}
		}
	}
	
	if(sort_type == 0){
		/*-------------------------------------------------
		BP����Ƃ�����Ă̏ꍇ�́A�ʐM��t�Ȃ�2���R����ʈȊO�̏ꏊ�����
		�Ă΂��\��������̂�UserWork�̈�ւ̃A�N�Z�X�͂��Ȃ����ƁI�I
		---------------------------------------------------*/
		
		//BP�l������������ذ�ްNo����ёւ�
		for(i = 0; i < BREEDER_MAX; i++){
			BrdSort[i] = i;
			for(s = 0; s < i; s++){
				//if(ContBP[BrdSort[s]] <= ContBP[i]){
				if(ContBP[BrdSort[s]] < ContBP[i] || 
					(ContBP[BrdSort[s]] == ContBP[i] && random[BrdSort[s]] < random[i]))
				{
					for(t = i; t > s; t--)
						BrdSort[t] = BrdSort[t - 1];
					BrdSort[s] = i;
					//BrdSort[i] = s;
					break;
				}
			}
			if(s == i)
				BrdSort[s] = i;
				//BrdSort[i] = s;
		}
		memcpy(tempSort, BrdSort, sizeof(tempSort));
		for(i = 0; i < BREEDER_MAX; i++)
			BrdSort[tempSort[i]] = i;
	}
	else
	{//İ��AP�l���������ɕ��ёւ�
		
		//�ݷݸޏ��ɕ��ёւ�
		memset(tempSort, 0xff, sizeof(tempSort));
		for(i = 0; i < BREEDER_MAX; i++){	//�����N�����ް����
			rank = apsys[i].ranking;
			for( ; ; rank++){
				if(tempSort[rank] == 0xff){
					tempSort[rank] = i;
					BrdSort[i] = rank;
					break;
				}
			}
		}
		//�ݷݸނ������̂��̂�����ђl�����Ŀ�Ă��Ȃ���
		for(i = 0; i < BREEDER_MAX-1; i++){
			for(s = BREEDER_MAX-1; s > i; s--){
				if(apsys[s-1].ranking == apsys[s].ranking && 
					BrdSort[s-1] < BrdSort[s] && random[s-1] < random[s])
				{
					temp = BrdSort[s];
					BrdSort[s] = BrdSort[s-1];
					BrdSort[s-1] = temp;
				}
			}
		}
	}
}



//=========================================================================
//	��ʉE�̃v���C���[����ύX���ꂽ���я��ɏ�������(����BG����޳��)
//=========================================================================
#define WIN_Y_LEN	5	//����޳�̏c�̷�א�
void ReWritePlayerInfo(void)
{
	int i;	//, t;
	
	for(i = 0; i < BREEDER_MAX; i++){
		PaletteWorkSet((void*)&UserWork[CONT_PALETTE_ADRS + 16*(5+i)*2], 
			16*(5+BrdSort[i]), 0x20);
	}
	PutPlayerInfo();
}


//=========================================================================
//	�Z�v�Z
//=========================================================================
void ContestWazaCalc(u8 nowbrd)	//, ContestCalcWork *pccw)
{
	u16 wazano;
	u8 apno;
	int i;
	u8 combo;
	u8 combo_level;
	u8 rnd;
	
	apsys[nowbrd].ap = 0;
	apsys[nowbrd].found_ap = 0;
//	memset(pccw, 0, sizeof(ContestCalcWork));
	if(WazaCalcFlagCheck(nowbrd) == 0){
//		pccw->yasumi = 1;
		return;
	}
	
	wazano = apsys[nowbrd].use_waza;
	apno = ConWazaTbl[wazano].apno;

	apsys[nowbrd].con_type = ConWazaTbl[wazano].con_type;
	
	if(apsys[nowbrd].use_waza == apsys[nowbrd].use_wazaold && apsys[nowbrd].use_waza != 0){	//�y�i���e�B����
		apsys[nowbrd].penalty = 1;
		apsys[nowbrd].conti_count++;
		combo = 0;
	}else{
//		if(apsys[nowbrd].combo1_invalid == 0)
//			combo = ComboCheck(apsys[nowbrd].use_wazaold, apsys[nowbrd].use_waza);
//		else
			combo = 0;
		apsys[nowbrd].conti_count = 0;
	}
	combo++;	//0�I���W���Ȃ̂Ŋ�{AP�ɑ΂��Ċ|���Z����̂�1�𑫂��Ă���
	apsys[nowbrd].found_ap = WazaKoukaTbl[apno].ap * combo;
	apsys[nowbrd].ap = apsys[nowbrd].found_ap;// + apsys[nowbrd].bonus;
	
	CONWAZA->bougai = WazaKoukaTbl[apno].bougai;
	CONWAZA->bougai_calc = CONWAZA->bougai;
	CONWAZA->nowbrd = nowbrd;

	for(i = 0; i < BREEDER_MAX; i++){
		apsys[i].bougai = 0;
		CONWAZA->nowwaza_target[i] = 0;
	}

	if(apsys[nowbrd].combo1_flg)
	{	//����1���������Ăĺ��ނ��������Ȃ��Ȃ炱����combo1�׸ނ𗎂�
		//(���̋Z�G�t�F�N�g���o���̂Ɠ����ɖڂ̓_�ł���������)
		if(ComboCheck(apsys[nowbrd].use_wazaold, apsys[nowbrd].use_waza) == 0)
			apsys[nowbrd].combo1_flg = 0;
	}
	
	ConWazaSeqTbl2[apno]();		//�Z���ʌv�Z
								//���Z���ʔ�����ɕK�v�Ȍv�Z�͂����ȍ~�ɏ����Ă���
	//�W�Q�Ȃǂ��I����Ă��瑫���悤�ɕύX	2002.08.24(�y)
	if(apsys[nowbrd].bonus_flg == 1)	//������݂Ŕ��������ްŽ�͉��Z���Ȃ�
		apsys[nowbrd].ap += apsys[nowbrd].bonus - BONUS_POINT;
	else{
		if(apsys[nowbrd].bonus3bai)
			apsys[nowbrd].ap += apsys[nowbrd].bonus * 3;
		else
			apsys[nowbrd].ap += apsys[nowbrd].bonus;
	}

	//----------------------------------------------------------
	//	�ǉ��v�Z�� 2002.08.08(��)
	//----------------------------------------------------------
	//-- �R���{�v�Z --//
	apsys[nowbrd].combo_level = 0;
	apsys[nowbrd].combo_hassei = 0;
	if(Combo1TermCheck(nowbrd)){
		combo_level = ComboCheck(apsys[nowbrd].use_wazaold, apsys[nowbrd].use_waza);
		if(combo_level && apsys[nowbrd].combo1_flg){	//���ސ���
			apsys[nowbrd].combo_level = combo_level;
			apsys[nowbrd].combo_hassei = 1;
			apsys[nowbrd].combo1_flg = 0;
			apsys[nowbrd].comboAP = apsys[nowbrd].found_ap * apsys[nowbrd].combo_level;
			apsys[nowbrd].combo_conti_flg = 1;
		}
		else if(ConWazaTbl[apsys[nowbrd].use_waza].combo1){	//����1����
			apsys[nowbrd].combo1_flg = 1;
			apsys[nowbrd].combo_hassei = 1;
		}
		else{											//������������
			apsys[nowbrd].combo1_flg = 0;
		}
	}
	
	//-- �y�i���e�B�v�Z --//
	if(apsys[nowbrd].penalty){
		apsys[nowbrd].penaltyAP = (apsys[nowbrd].conti_count + 1) * 10;
	}
	
	//-- �ْ����̏��� --//
	if(apsys[nowbrd].miss_flg){
		apsys[nowbrd].combo1_flg = 0;
		apsys[nowbrd].ap = 0;
		apsys[nowbrd].found_ap = 0;
	}
	
	//-- �e���V�����v�Z --//
	ccw->tensyon_updown = TensyonCheck(apsys[nowbrd].use_waza);
	if(apsys[nowbrd].tensyon_up)
		ccw->tensyon_updown = TENSYON_GAUGE_UP;
	if(ccw->tensyon_updown > 0){
		if(consys->tensyon + ccw->tensyon_updown >= TENSYON_MAX)
			ccw->tensyonAdd = TENSYON_MAXPOINT;
		else
			ccw->tensyonAdd = TENSYON_POINT;
	}
	else
		ccw->tensyonAdd = 0;

	//-- �u�ւ񂵂�v�u�Ȃ肫��v���g�p�������ɉ��Ԗڂ��߹�݂ɉ����邩 --//
	rnd = pp_rand() % (BREEDER_MAX-1);
	for(i = 0; i < BREEDER_MAX; i++){
		if(i == nowbrd)
			continue;
		if(rnd == 0)
			break;
		else
			rnd--;
	}
	apsys[nowbrd].hensin_monsno = i;
}

//=========================================================================
//	�Z�̃A�s�[�����ʂŔ���������Ԃ̃��b�Z�[�W�ԍ����Z�b�g����
//	���b�Z�[�W���o����ذ�ް
//=========================================================================
void AppealKoukaMsgSet(u8 brdno, u8 msgid)
{
	apsys[brdno].msgid = msgid;
}

void AppealKoukaMsgSet2nd(u8 brdno, u8 msgid)
{
	apsys[brdno].msgid2nd = msgid;
}

//----------------------------------------------------------
//	�W�Q���󂯂����̃��b�Z�[�W���Z�b�g
//		���W�Q�_���[�W�̈З͂ɂ���ă��b�Z�[�W���ς�����
//	brdno = �W�Q���ޯĂ���ذ�ްNo
//----------------------------------------------------------
void BougaiMsgSet(u8 brdno, u8 damage)
{
	if(damage >= 60)
		AppealKoukaMsgSet(brdno, BOUGAI60_MSG);
	else if(damage >= 40)
		AppealKoukaMsgSet(brdno, BOUGAI40_MSG);
	else if(damage >= 30)
		AppealKoukaMsgSet(brdno, BOUGAI30_MSG);
	else if(damage >= 20)
		AppealKoukaMsgSet(brdno, BOUGAI20_MSG);
	else if(damage >= 10)
		AppealKoukaMsgSet(brdno, BOUGAI10_MSG);
}

//=========================================================================
//	�Z���o�������̃A�s�[�����ʃ��b�Z�[�W�̃Z�b�g
//=========================================================================
void AppealMsgSet(u8 brd, u8 msgid)
{
	PM_strcpy(StrTempBuffer0, BreederParam[brd].nickname);
	PM_strcpy(StrTempBuffer1, WazaNameData[apsys[brd].use_waza]);
	if(ConWazaTbl[apsys[CONWAZA->nowbrd].use_waza].con_type == STYLE_CONTEST)
		PM_strcpy(StrTempBuffer2, mojimoji_msg);
	else if(ConWazaTbl[apsys[CONWAZA->nowbrd].use_waza].con_type==BEAUTIFUL_CONTEST)
		PM_strcpy(StrTempBuffer2, dokidoki_msg);
	else if(ConWazaTbl[apsys[CONWAZA->nowbrd].use_waza].con_type == CUTE_CONTEST)
		PM_strcpy(StrTempBuffer2, deredere_msg);
	else if(ConWazaTbl[apsys[CONWAZA->nowbrd].use_waza].con_type == CLEVER_CONTEST)
		PM_strcpy(StrTempBuffer2, orooro_msg);
	else
		PM_strcpy(StrTempBuffer2, bikubiku_msg);
	PM_MsgExpand(MsgExpandBuffer, AppealMsgTbl[msgid]);

	ContestTalkMsgClear();
	TalkMsgSet(&win[0], MsgExpandBuffer, CON_MSG, CON_TALKMSG_X, CON_TALKMSG_Y);
}

//=========================================================================
//	�ْ���ԂɂȂ������ɋْ��׸ނ̾�āA���̑��������̏���
//=========================================================================
void KintyouFlgSet(u8 brd)
{
	apsys[brd].miss_flg = 1;
	apsys[brd].use_waza = 0;
//	apsys[brd].penalty = 1;		//������݂�combo1_invalid�׸ނ�Ă��邽��
}

//=========================================================================
//	�Z���o�����Ԃ̿�Ă��s��
//=========================================================================
void ConWazaBrdSort(void)
{
	int i, s;
	u8 sort[BREEDER_MAX];
	u8 flg[BREEDER_MAX];
	u8 temp = 0;
	
	for(i = 0; i < BREEDER_MAX; i++){
		sort[i] = BrdSort[i];
		flg[i] = 0;
	}
	
	for(i = 0; i < BREEDER_MAX; i++){
		for(s = 0; s < BREEDER_MAX; s++){
			if(apsys[s].next_sort == i){	//�Z���ʂŏ��Ԃ����܂��Ă�̂�T��
				sort[s] = i;
				flg[s] = 1;		//������ذ�ް���ް��̓Z�b�g���ꂽ
				break;
			}
		}
		if(s == BREEDER_MAX){	//�Z���ʂŌ��܂��Ă��ް��͂Ȃ�����
			//�܂���Ă���Ă��Ȃ��ް��ň�Ԑ������Ⴂ�̂�T��
			for(s = 0; s < BREEDER_MAX; s++){
				if(flg[s] == 0 && apsys[s].next_sort == 0xff){
					temp = s;
					s++;
					break;
				}
			}
			for( ; s < BREEDER_MAX; s++){
				if(flg[s] == 0 && apsys[s].next_sort == 0xff){
					if(BrdSort[temp] > BrdSort[s])
						temp = s;
				}
			}
			sort[temp] = i;
			flg[temp] = 1;
		}
	}
	
	for(i = 0; i < BREEDER_MAX; i++){
		CONWAZA->Sort[i] = sort[i];
		//����
		apsys[i].next_sort = 0xff;
		apsys[i].nextsort_flg = 0;
		
		//�Z���ʂŏ��Ԃ��ς�����糨��޳�̕\���ʒu������ɍ��킹��悤�ɂ���
		BrdSort[i] = sort[i];
	}
}












//=========================================================================
//
//		�R���̂ӂ������A�C�R���V�[�P���X
//												by matsuda 2002.04.18(��)
//=========================================================================
static void JudgeFukidasiSeq(actWork *xreg)
{
	if(xreg->work[1]++ < FUKIDASI_PUT_TIME)
		return;
	xreg->work[1] = 0;
	xreg->banish = 1;
	xreg->move = DummyActMove;
	consys->fukidasi_eff = 0;
}

//----------------------------------------------------------
//	�ӂ������\���Ăяo��
//----------------------------------------------------------
void JudgeFukidasiSet(u8 fukidasi_type)
{
	u8 actno;
	
	actno = consys->fukidasi_act;
	switch(fukidasi_type){
		case PENALTY_TYPE:
		case TENSYONDOWN_TYPE:
			ActWork[actno].oamData.CharNo = ActWork[actno].work[0];
			SePlay(SE_HAZURE);
			break;
		case COMBO1_TYPE:
			ActWork[actno].oamData.CharNo = ActWork[actno].work[0]+4*1;
			SePlay(SE_SEIKAI);
			break;
		case COMBO2_TYPE:
			ActWork[actno].oamData.CharNo = ActWork[actno].work[0]+4*2;
			SePlay(SE_SEIKAI);
			break;
		case TENSYONUP_TYPE:
			ActWork[actno].oamData.CharNo = ActWork[actno].work[0]+4*3;
			SePlay(SE_TK_WARPIN);
			break;
		case NEXTSORT_FAST_TYPE:
			ActWork[actno].oamData.CharNo = ActWork[actno].work[0]+4*3;
			SePlay(SE_TK_WARPIN);
			break;
		case NEXTSORT_LAST_TYPE:
			ActWork[actno].oamData.CharNo = ActWork[actno].work[0]+4*4;
			SePlay(SE_TK_WARPIN);
			break;
		case BONUS_TYPE:
			ActWork[actno].oamData.CharNo = ActWork[actno].work[0]+4*6;
			SePlay(SE_W215);
			break;
		case NEXTSORT_QUES_TYPE:
		default:
			ActWork[actno].oamData.CharNo = ActWork[actno].work[0]+4*5;
			SePlay(SE_TK_WARPIN);
			break;
	}
	ActWork[actno].work[1] = 0;
	ActWork[actno].banish = 0;
	ActWork[actno].move = JudgeFukidasiSeq;
	consys->fukidasi_eff = 1;
}



//=========================================================================
//	�e���V�������l�\��
//=========================================================================
#define TENSYON_METER_MAX		5
#define TENSYON_METER_SCRN		0x3031
#define TENSYON_METER_CLR_SCRN	0x3030	//�ر�p��ذ��ް�
void TensyonNumPut(void)
{
	int i;
//	u16 scrn;
	u8 *adrs;
	
	for(i = 0; i < TENSYON_METER_MAX; i++){
		/*
		if(i < consys->tensyon)
			scrn = TENSYON_METER_SCRN;
		else
			scrn = TENSYON_METER_CLR_SCRN;
		*(vu16*)(SCREEN_BASE24_ADDR + (0x20*2 + 0x3 + i)*2) = scrn;
		*/
		if(i < consys->tensyon)
			adrs = (void*)excite2_pch_ADRS + 0x20*2;	//CHAR_BASE0_ADDR + 0x31*32;
		else
			adrs = (void*)excite2_pch_ADRS + 0x20*0;	//CHAR_BASE0_ADDR + 0x30*32;
		CpuCopy(adrs, OBJ_MODE0_VRAM + 
			((ActWork[consys->volgauge_act].oamData.CharNo+0x11+i) * 32), 32, 32);
		CpuCopy(adrs+32, OBJ_MODE0_VRAM + 
			((ActWork[consys->volgauge_act].oamData.CharNo+0x19+i) * 32), 32, 32);
		if(consys->tensyon >= TENSYON_MAX)
			TensyonMaxPalAnmSet();
	}
}

//-- �u�e���V�����F�v��ү���ޕ\�� --//
void TensyonMsgPut(void)
{
	u8 msgbuf[20];
	
	PM_strcpy(msgbuf, Tensyon_msg);
	MsgPrint(&FightMsg, msgbuf, CON_TENSYON_MSG, 0,0);
}

//----------------------------------------------------------
//	�Z�̃^�C�v���R���e�X�g�̃^�C�v�Ƃ����Ă��邩����
//----------------------------------------------------------
static const s8 ConTypeCheckTbl[][5] = {
//�����������A�������A���킢���A�����A�����܂���
	{TENSYON_GAUGE_UP,0,-1,-1,0},	//����������
	{0,TENSYON_GAUGE_UP,0,-1,-1,},	//������
	{-1,0,TENSYON_GAUGE_UP,0,-1},	//���킢��
	{-1,-1,0,TENSYON_GAUGE_UP,0},	//����
	{0,-1,-1,0,TENSYON_GAUGE_UP},	//�����܂���
};
s8 TensyonCheck(u16 wazano)
{
	return ConTypeCheckTbl[ContestKind][ConWazaTbl[wazano].con_type];
}

//----------------------------------------------------------
//	�e���V����Max������گı�Ґ���^�X�N���
//----------------------------------------------------------
u8 TensyonMaxPalAnmSet(void)
{
	u8 id;
	
	id = AddTask(TensyonMaxPalAnmMain, 10);
	TaskTable[id].work[1] = 1;
	TaskTable[id].work[2] = ObjPalNumGet(VOLTAGEGAUGE_ID);
	return id;
}

#define TENSYON_PALANM_WAIT		0
static void TensyonMaxPalAnmMain(u8 id)
{
//	u8 add;
	
	if(TaskTable[id].work[0]++ == TENSYON_PALANM_WAIT){
		TaskTable[id].work[0] = 0;
		if(TaskTable[id].work[3] == 0)
			TaskTable[id].work[4]++;
		else
			TaskTable[id].work[4]--;
		SoftFade(16*16+16*TaskTable[id].work[2]+8, 1, TaskTable[id].work[4], 0x7fff);
		if(TaskTable[id].work[4] == 0 || TaskTable[id].work[4] == 16){
			TaskTable[id].work[3] ^= 1;
		//	if(TaskTable[id].work[4] == 0 && consys->tensyon < TENSYON_METER_MAX)
			if(consys->tensyon < TENSYON_METER_MAX){
				SoftFade(16*16+16*TaskTable[id].work[2]+8, 1, 0, 0x1f);
				DelTask(id);
			}
		}
	}
}

//----------------------------------------------------------
//	�{���e�[�W�Q�[�W���X���C�h�C��
//----------------------------------------------------------
#define VOLTAGE_GAUGE_OUT_DX	(-(VOLTAGE_GAUGE_X+40))	//���ð�޹ް�މ�ʊO���W
#define VOLTAGE_GAUGE_ADD_SLIDE_X	0x0680	//�ײ�ނ��Ă��鑬�x
void VoltageGaugeSlidIn(void)
{
	AddTask(VoltageGaugeSlidInTask, 10);
	ActWork[consys->volgauge_act].dx = VOLTAGE_GAUGE_OUT_DX;
//	ActWork[consys->voltage_act].dx = VOLTAGE_GAUGE_OUT_DX;
//	ActWork[consys->volgaugesub_act].dx = VOLTAGE_GAUGE_OUT_DX;
	ActWork[consys->volgauge_act].banish = 0;
//	ActWork[consys->voltage_act].banish = 0;
//	ActWork[consys->volgaugesub_act].banish = 0;
	consys->voltage_slide_eff = 1;
}

static void VoltageGaugeSlidInTask(u8 id)
{
//	actWork *kao, *gauge, *gauge_sub;
	actWork *gauge;
	
//	kao = &ActWork[consys->voltage_act];	//��A�C�R��
	gauge = &ActWork[consys->volgauge_act];	//�Q�[�W����
//	gauge_sub = &ActWork[consys->volgaugesub_act];	//�ް�މE�[
	TaskTable[id].work[10] += VOLTAGE_GAUGE_ADD_SLIDE_X;
	gauge->dx += TaskTable[id].work[10] >> 8;
	TaskTable[id].work[10] &= 0x00ff;
	if(gauge->dx > 0)
		gauge->dx = 0;
//	gauge->dx = kao->dx;
//	gauge_sub->dx = gauge->dx;
	if(gauge->dx == 0){
		consys->voltage_slide_eff = 0;
		DelTask(id);
	}
}

//----------------------------------------------------------
//	�{���e�[�W�Q�[�W���X���C�h�A�E�g
//----------------------------------------------------------
void VoltageGaugeSlidOut(void)
{
	if(ActWork[consys->volgauge_act].banish == 1){
		consys->voltage_slide_eff = 0;
		return;
	}
	
	AddTask(VoltageGaugeSlidOutTask, 10);
	ActWork[consys->volgauge_act].dx = 0;
//	ActWork[consys->voltage_act].dx = 0;
//	ActWork[consys->volgaugesub_act].dx = 0;
	consys->voltage_slide_eff = 1;
}

static void VoltageGaugeSlidOutTask(u8 id)
{
//	actWork *kao, *gauge, *gauge_sub;
	actWork *gauge;
	
//	kao = &ActWork[consys->voltage_act];	//��A�C�R��
	gauge = &ActWork[consys->volgauge_act];	//�Q�[�W����
//	gauge_sub = &ActWork[consys->volgaugesub_act];	//�ް�މE�[
	TaskTable[id].work[10] += VOLTAGE_GAUGE_ADD_SLIDE_X;
	gauge->dx -= TaskTable[id].work[10] >> 8;
	TaskTable[id].work[10] &= 0x00ff;
	if(gauge->dx < VOLTAGE_GAUGE_OUT_DX)
		gauge->dx = VOLTAGE_GAUGE_OUT_DX;
//	gauge->dx = kao->dx;
//	gauge_sub->dx = gauge->dx;
	if(gauge->dx == VOLTAGE_GAUGE_OUT_DX){
//		kao->banish = 1;
		gauge->banish = 1;
//		gauge_sub->banish = 1;
		consys->voltage_slide_eff = 0;
		DelTask(id);
	}
}

//----------------------------------------------------------
//	���ð�޹ް�ނ��A�ײ�޲݁�wait���ް�ޑ�����SE�A�Ƃ�����
// ����:flg = �ݼ�ݏ㏸����1�A���~����-1
//----------------------------------------------------------
#define VOLTAGE_SE_WAIT		20
void VoltageGaugeSlidPut(s8 flg)
{
	u8 id;
	
	id = AddTask(VoltageGaugeSlidPutTask, 5);
	TaskTable[id].work[0] = flg;
	consys->voltage_eff = 1;
}

static void VoltageGaugeSlidPutTask(u8 id)
{
	switch(TaskTable[id].work[10])
	{
		case 0:
			VoltageGaugeSlidIn();
			TaskTable[id].work[10]++;
			break;
		case 1:
			if(consys->voltage_slide_eff == 0){
				VoltageIconAnmSet(TaskTable[id].work[0]);
				TaskTable[id].work[10]++;
			}
			break;
		case 2:
			if(TaskTable[id].work[11]++ > VOLTAGE_SE_WAIT){
				TaskTable[id].work[11] = 0;
				TensyonNumPut();
				consys->voltage_eff = 0;
				DelTask(id);
			}
			break;
	}
}

//----------------------------------------------------------
//	�{���e�[�W�Q�[�W��\��������(�ײ�޴̪�ĂȂ�)
//----------------------------------------------------------
void VoltageGaugePut(void)
{
	ActWork[consys->volgauge_act].dx = 0;
//	ActWork[consys->voltage_act].dx = 0;
//	ActWork[consys->volgaugesub_act].dx = 0;
	ActWork[consys->volgauge_act].banish = 0;
//	ActWork[consys->voltage_act].banish = 0;
//	ActWork[consys->volgaugesub_act].banish = 0;
}

//----------------------------------------------------------
//	�{���e�[�W�Q�[�W������(�ײ�޴̪�ĂȂ�)
//----------------------------------------------------------
void VoltageGaugeBanish(void)
{
	ActWork[consys->volgauge_act].banish = 1;
//	ActWork[consys->voltage_act].banish = 1;
//	ActWork[consys->volgaugesub_act].banish = 1;
}

//=========================================================================
//	�ϋq��2����ݱ�҂��J�n
//=========================================================================
#define KANKYAKU_ANM_WAIT	7
#define KANKYAKU_ANM_LOOP	9	//��҂������������邩
void KankyakuAnmSet(void)
{
	AddTask(KankyakuAnmTask, 15);
	consys->kankyaku_anm = 1;
}

static void KankyakuAnmTask(u8 id)
{
	if(TaskTable[id].work[10]++ < KANKYAKU_ANM_WAIT)
		return;
	
	TaskTable[id].work[10] = 0;
	if(TaskTable[id].work[11] == 0){
		DIV_DMACOPY(3, &UserWork[CONT_KANKYAKU_ADRS + 0x1000], 
			CHAR_BASE0_ADDR+0x2000, 0x1000, 32);
	}
	else{
		DIV_DMACOPY(3, &UserWork[CONT_KANKYAKU_ADRS], 
			CHAR_BASE0_ADDR+0x2000, 0x1000, 32);
		TaskTable[id].work[12]++;
	}
	TaskTable[id].work[11] ^= 1;
	if(TaskTable[id].work[12] == KANKYAKU_ANM_LOOP){
		consys->kankyaku_anm = 0;
		DelTask(id);
	}
}

//----------------------------------------------------------
//	�ϋq�̐���オ��(���艺����)���̶װ̪��ނ��s�Ȃ�
// flg = 1:����オ��	-1:���艺����
// muki = 1:���̐F����̪��ތ�̐F�ɂ��Ă���	-1:���̐F�ɖ߂��Ă���
//----------------------------------------------------------
#define KANKYAKU_COLOR_WAIT		0
#define KANKYAKU_COLOR_EVY		3
void KankyakuColorFadeSet(s8 flg, s8 muki)
{
	u16 color;
	u8 evy, end_evy;
	u8 id;
	
	id = AddTask(KankyakuColorFadeTask, 10);
	if(flg > 0){
		color = TENSYON_UP_COLOR;
		if(muki > 0){
			evy = 0;
			end_evy = KANKYAKU_COLOR_EVY*consys->tensyon;
		}
		else{
			evy = KANKYAKU_COLOR_EVY*consys->tensyon;
			end_evy = 0;
		}
	}
	else{
		color = TENSYON_DOWN_COLOR;
		if(muki > 0){
			evy = 0;
			end_evy = 12;
		}
		else{
			evy = 12;
			end_evy = 0;
		}
	}
	
	TaskTable[id].work[0] = color;
	TaskTable[id].work[1] = evy;
	TaskTable[id].work[2] = muki;
	TaskTable[id].work[3] = end_evy;
	
	consys->kankyaku_color_eff = 0;
}

static void KankyakuColorFadeTask(u8 id)
{
	if(TaskTable[id].work[10]++ < KANKYAKU_COLOR_WAIT)
		return;
	TaskTable[id].work[10] = 0;
	
	if(TaskTable[id].work[2] > 0)
		TaskTable[id].work[1]++;
	else
		TaskTable[id].work[1]--;
	
	SoftFade(16*1+1, 1, TaskTable[id].work[1], TaskTable[id].work[0]);
	SoftFade(16*1+0xa, 1, TaskTable[id].work[1], TaskTable[id].work[0]);

	if(TaskTable[id].work[1] == TaskTable[id].work[3]){
		DelTask(id);
		consys->kankyaku_color_eff = 0;
	}
}


//=========================================================================
//	�u���H�΂�v�̕\��
// flg = 0:�S���̕\����ر����	1:�u���H�΂�v�̕\��
//=========================================================================
void NextSortPrint(u8 flg)
{
	int t;

	 for(t = 0; t < BREEDER_MAX; t++){
		if(apsys[t].nextsort_flg && flg){
			CpuCopy(GetNextSortNumAdrs(t), (ActWork[brdsys[t].sorticon_actno].oamData.CharNo+2)*32 + OBJ_MODE0_VRAM, 0x20, 32);
			ActWork[brdsys[t].sorticon_actno].y = SortIconYPos[BrdSort[t]];
			ActWork[brdsys[t].sorticon_actno].banish = 0;
		}
		else
			ActWork[brdsys[t].sorticon_actno].banish = 1;
	}
}

//----------------------------------------------------------
//	�u���@�΂�v�̐����̕����̱��ڽ���擾����
//----------------------------------------------------------
static u32 GetNextSortNumAdrs(u8 brd_pos)
{
	u32 adrs;
	
	if(apsys[brd_pos].nextsort_flg == 1)
		adrs = tugi_num_pch_ADRS + apsys[brd_pos].next_sort * 32;
	else
		adrs = tugi_num_pch_ADRS + 4*32;
	return adrs;
}

//=========================================================================
//	�W�Q�A�s�[�������ޯĂɂȂ����߹�݂ɃA�C�R����\��������
//=========================================================================
void BougaiReceiveIconPut(void)
{
	u16 icon_scrn;
	u8 ypos;
	int brd;
	
	for(brd = 0; brd < BREEDER_MAX; brd++){
		if(CONWAZA->nowwaza_target[brd] == 0 || YasumiTurnCheck(brd))
			continue;
		
		ypos = 2 + (BrdSort[brd] * 5);
		icon_scrn = GetWazaKoukaIconScrn(brd, BOUGAI_TARGET_TYPE);
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x14)*2) = icon_scrn;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*ypos + 0x15)*2) = icon_scrn+1;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x14)*2) = icon_scrn+0x10;
		*(vu16 *)(SCREEN_BASE24_ADDR + (0x20*(ypos+1) + 0x15)*2) = icon_scrn+0x11;
		SePlay(SE_C_PASI);
	}
}


//=========================================================================
//	�R���{1���L���ȏ�Ԃ��`�F�b�N
//	�߂�l: 1=�L��	0=����
//=========================================================================
u8 Combo1TermCheck(u8 brd)
{
	//����è���O����݂����A�������Ă�Ȃ���ނ͖���
//	if(apsys[brd].penalty || apsys[brd].combo1_invalid || apsys[brd].miss_flg)
	if(apsys[brd].penalty || apsys[brd].miss_flg)
		return 0;	//����
	return 1;	//�L��
}


//=========================================================================
//
//		�v���C���[�����X�V���鎞�ɃE�B���h�E���B���G�t�F�N�g
//												by matsuda 2002.06.06(��)
//=========================================================================
#define RIGHT_IN	6
#define DOWN_IN		CURTAIN_SP	//7
#define LASTER_IN	6
#define FLASH_WAIT	10

static void DownMoveEffectIn(u8 id);
static void DownMoveEffectOut(u8 id);
static void FlashWait(u8 id);
static void FlashEndWait(u8 id);

//----------------------------------------------------------
//	�}�X�N�G�t�F�N�g�̃f�[�^�A���W�X�^��Ă���
//----------------------------------------------------------
void InfoMaskEffectDataSet(void)
{
	int i;
	
	(*(vBgCnt *)REG_BG1CNT).Priority = 0;
	(*(vBgCnt *)REG_BG1CNT).Size = 1;
	(*(vBgCnt *)REG_BG1CNT).Loop = 0;
	ScrX1 = 240;	//��ʊO���W�Ɏ����Ă�
	ScrY1 = 160;
	*(vu16 *)REG_BG1HOFS = ScrX1;
	*(vu16 *)REG_BG1VOFS = ScrY1;
//��ذݻ��ނ�2��ʕ������ǁA���ۂ̽�ذ��ް���2��ʕ��Ȃ��̂őS�Ă��܂�0�N���A���Ă���
	DIV_DMACLEAR(3, 0, BG_VRAM + 0x800*CONWAZA_BG_SCRNBASE, 0x1000, 32);
	DecordVram((void*)con_bg0_psc_ADRS, (void*)(BG_VRAM+0x800*CONWAZA_BG_SCRNBASE));
	(*(vBgCnt *)REG_BG1CNT).CharBasep = 0;
	for(i = 0; i < BREEDER_MAX; i++){
		ActWork[brdsys[i].heart_actno].oamData.Priority = 1;
		ActWork[brdsys[i].sorticon_actno].oamData.Priority = 1;
	}
	(*(vBgCnt *)REG_BG2CNT).Priority = 1;
	(*(vBgCnt *)REG_BG0CNT).Priority = 1;
	(*(vBgCnt *)REG_BG1CNT).Size = 2;
}

//----------------------------------------------------------
//	�}�X�N�G�t�F�N�g�ŕύX����Vram�A���W�X�^��ر���Č��ɖ߂�
//	��BG0,2����ײ��è�͕�Sync�ɊO���Œ������ƁI�I
//----------------------------------------------------------
void InfoMaskEffectDataClear(void)
{
	int i;
	
	DIV_DMACLEAR(3, 0, BG_VRAM+0x4000*CONWAZA_BG_CHARBASE, 0x2000, 32);
	DIV_DMACLEAR(3, 0, BG_VRAM+0x800*CONWAZA_BG_SCRNBASE, 0x1000, 32);
	ScrX1 = 0;
	ScrY1 = 0;
	(*(vBgCnt *)REG_BG1CNT).Priority = 1;
	(*(vBgCnt *)REG_BG1CNT).Size = 0;
	(*(vBgCnt *)REG_BG1CNT).Loop = 0;
	(*(vBgCnt *)REG_BG1CNT).CharBasep = CONWAZA_BG_CHARBASE;

	for(i = 0; i < BREEDER_MAX; i++){
		ActWork[brdsys[i].heart_actno].oamData.Priority = 0;
		ActWork[brdsys[i].sorticon_actno].oamData.Priority = 0;
	}
}

//----------------------------------------------------------
//	
//----------------------------------------------------------
void DownMoveEffect(u8 id)
{
	ScrX1 = 0;
	ScrY1 = 160;
	
	SePlayPan(SE_C_MAKU_D, 0);
	TaskTable[id].TaskAdrs = DownMoveEffectIn;
}

static void DownMoveEffectIn(u8 id)
{
	ScrY1 -= DOWN_IN;
	if((s16)ScrY1 < 0)
		ScrY1 = 0;
	if(ScrY1 == 0){
		TaskTable[id].work[0] = 0;
		TaskTable[id].work[1] = 0;
		TaskTable[id].work[2] = 0;
		//TaskTable[id].TaskAdrs = FlashWait;
		TaskTable[id].TaskAdrs = InfoRewriteData;
		//InfoRewriteData();
	}
}

//=========================================================================
//	���^�[���Ɍ����ăv���C���[���X�V
//=========================================================================
enum{
	REWRITEDATA_INIT,
	CONWAZAFLAG_CLEAR_INIT,
	CONWAZAFLAG_CLEAR_WAIT,
	CONWAZAFLAG_CLEAR_END,
};
void InfoRewriteData(u8 id)
{
	int i;
	u8 taskno;
	
	switch(TaskTable[id].work[0]){
		case REWRITEDATA_INIT:
			for(i = 0; i < BREEDER_MAX; i++)
				consys->nowturn_sort[i] = BrdSort[i];
			AppealMeterClear();
			MeterColorClearAll();
			BonusIconResetAll();
			ReWritePlayerInfo();
			NextSortPrint(1);
			HeartIconYposSet();
			TaskTable[id].work[0] = CONWAZAFLAG_CLEAR_INIT;
			break;
		case CONWAZAFLAG_CLEAR_INIT:
			if(ContestTuusin & C_TUUSIN_FLG){
				consys->sio_wazacalc = 1;
				if(ContestSioOyaCheck()){
					//����ݸر�����K�v�������׸ނ�ر�����
					ConWazaFlagCheckClear();	//��݂�ݸ���Ă���O�ɌĂԂ���!
				}
				taskno = AddTask(ContWazaCalcDataSioGet, 0);
				ChangeTask(taskno, ContWazaCalcDataSioGet, ContWazaCalcDatatWait);
				TuusintyuuMsgPut();
				TaskTable[id].work[0] = CONWAZAFLAG_CLEAR_WAIT;
			}
			else{
				//����ݸر�����K�v�������׸ނ�ر�����
				ConWazaFlagCheckClear();	//��݂�ݸ���Ă���O�ɌĂԂ���!
				TaskTable[id].work[0] = CONWAZAFLAG_CLEAR_END;
			}
			break;
		case CONWAZAFLAG_CLEAR_WAIT:
			if(consys->sio_wazacalc == 0){
				TaskTable[id].work[0] = CONWAZAFLAG_CLEAR_END;
			}
			break;
		case CONWAZAFLAG_CLEAR_END:
			WazaKoukaIconPutAll();
			PlayerInfoCopy();
			TaskTable[id].work[0] = 0;
			TaskTable[id].TaskAdrs = FlashWait;
			break;
	}
}


static void DownMoveEffectOut(u8 id)
{
	ScrY1 += DOWN_IN;
	if((s16)ScrY1 > 160)
		TaskTable[id].TaskAdrs = InfoMaskEffectEnd;
}


//----------------------------------------------------------
//	
//----------------------------------------------------------
static void FlashWait(u8 id)
{
	if(TaskTable[id].work[2] < FLASH_WAIT){
		TaskTable[id].work[2]++;
		return;
	}

//	�҂����B�t���b�V���͂��Ȃ��@2002.07.10(��)
//	SoftFade(CONWAZA_BG_PAL*16, 16, TaskTable[id].work[0], 0x7fff);
	if(TaskTable[id].work[1] == 0){
		if(TaskTable[id].work[0] == 16)
			TaskTable[id].work[1]++;
		else
			TaskTable[id].work[0]++;
	}
	else{
		if(TaskTable[id].work[0] == 0){
			TaskTable[id].work[1] = 0;
			TaskTable[id].work[2] = 0;
			TaskTable[id].TaskAdrs = FlashEndWait;
		}
		else
			TaskTable[id].work[0]--;
	}
}

static void FlashEndWait(u8 id)
{
	if(TaskTable[id].work[2] < FLASH_WAIT)
		TaskTable[id].work[2]++;
	else
	{
		TaskTable[id].work[2] = 0;
		SePlayPan(SE_C_MAKU_U, 0);
		TaskTable[id].TaskAdrs = DownMoveEffectOut;
	}
}



//=========================================================================
//	�n�[�g�A�C�R���̨̱ݴ̪�Ă��
//		aff_type = client.h��enum����`���Ă���܂�
//=========================================================================
void HeartIconAffEffSet(u8 aff_type)
{
	int i;
	u8 id;
	
	//�A�t�B���f�[�^���擾
	for(i = 0; i < BREEDER_MAX; i++){
		ActWork[brdsys[i].heart_actno].oamData.AffineParamNo
			= AffineWorkNumGet();
		ActWork[brdsys[i].heart_actno].oamData.AffineMode = 1;
		ActAffAnmChg(&ActWork[brdsys[i].heart_actno], aff_type);
		
		if(aff_type == HEART_AFF_BIG){
			ActAnm(&ActWork[brdsys[i].heart_actno]);
			ActWork[brdsys[i].heart_actno].banish = 0;
		}
	}
	id = AddTask(HeartIconAffEffCheckSeq, 5);
	TaskTable[id].work[0] = aff_type;
	consys->heart_aff_eff = 1;	//�̨ݴ̪�Ē��׸޾��
}

//----------------------------------------------------------
//	�n�[�g�A�C�R���̨̱ݱ�҂��I������̂��Ď�
//----------------------------------------------------------
static void HeartIconAffEffCheckSeq(u8 id)
{
	int i;
	u8 aff_type;
	
	if(ActWork[brdsys[0].heart_actno].affend_sw == 0)
		return;
	
	aff_type = TaskTable[id].work[0];
	if(aff_type == HEART_AFF_SMALL){
		for(i = 0; i < BREEDER_MAX; i++)
			ActWork[brdsys[i].heart_actno].banish = 1;
	}
	
	//�̨��Ȱ�ެ��J��
	for(i = 0; i < BREEDER_MAX; i++)
		AffineDelActor(&ActWork[brdsys[i].heart_actno]);

	consys->heart_aff_eff = 0;
	DelTask(id);
}

//=========================================================================
//	���ް�ޮ݂��l����WAZANO_MAX�ȏ�̋ZNo���������́u�͂����v�̋ZNo��Ԃ�
//=========================================================================
u16 GetContWazaNo(u16 wazano)
{
	if(wazano > WAZANO_MAX)
		wazano = 1;	//���m�̋Z�͑S�āu�͂����v
	return wazano;
}

//=========================================================================
//	���ް�ޮ݂��l����MONSNO_END�ȏ�̒l���������́u?�v��MonsNo��Ԃ�
//=========================================================================
u16 GetContMonsNo(u16 monsno)
{
	if(monsno >= MONSNO_END)
		monsno = 0;
	return monsno;
}

//=========================================================================
//	�Z�G�t�F�N�g�Ŏg�p����V�X�e�����[�N��Ă���
//		���Z�̪�Ă��o���O�ɍs�Ȃ�
//=========================================================================
void ContestWazaEffectWorkSet(u8 brd)
{
	int i;
	u16 wazano;
	u16 monsno;
//	u16 rnd;
	
	wazano = GetContWazaNo(apsys[brd].use_waza);
	monsno = GetContMonsNo(BreederParam[brd].monsno);
	
	memset(conpara, 0, sizeof(ConWazaEffParam));
	WazaEffectWorkInit();
	for(i = 0; i < CLIENT_MAX; i++){
		BattlePokeAnmNo[i] = 0;
	}

	//-- �Z���Ƃɂ�鶳��-�̐ݒ� --//
	switch(wazano){
		case WAZANO_NOROI:	//�̂낢
			if(PPD[monsno].type1 == GHOST_TYPE || PPD[monsno].type2 == GHOST_TYPE)
				WazaCounter = 0;
			else
				WazaCounter = 1;
			break;
		case WAZANO_HENSIN:
		case WAZANO_NARIKIRI:
			i = apsys[brd].hensin_monsno;
			conpara->hensin_monsno = GetContMonsNo(BreederParam[i].monsno);
			conpara->hensin_personal_rnd = BreederParam[i].personal_rnd;
			conpara->hensin_flg = 1;
			break;
		case WAZANO_ONGAESI:
			WazaEffNatukido = 255;	//�З�Max��Ԃ̂��o��
			break;
		case WAZANO_YATUATARI:
			WazaEffNatukido = 0;	//�З�Max��Ԃ̂��o��
			break;
		case WAZANO_SOORAABIIMU:	//2��ݘA��
		case WAZANO_KAMAITATI:
		case WAZANO_ROKETTOZUTUKI:
		case WAZANO_GODDOBAADO:
			if(consys->eff_loop == 0){
				consys->eff_loop = 2;
				WazaCounter = 0;
			}
			else
				WazaCounter = 1;
			break;
	}
	
	//-- �������������Ұ�������� --//
	EnemyBanishActorParamInit();
}

//=========================================================================
//	�Z�G�t�F�N�g�Ŏg�p����V�X�e�����[�N���N���A����
//		���Z�̪�Ă��I��������ɍs�Ȃ�
//=========================================================================
void ContestWazaEffectWorkClear(u8 brd)
{
	memset(conpara, 0, sizeof(ConWazaEffParam));	//�ϐg�׸ނ�ر���邽��
	if(consys->eff_loop)
		consys->eff_loop--;
}

//=========================================================================
//	�Z�̪�ĂŕK�v���߹�݃f�[�^��Ă���
//=========================================================================
void ConWazaEffParaSet(u8 brd)
{
	conpara->brd = brd;
	conpara->monsno = GetContMonsNo(BreederParam[brd].monsno);
	conpara->personal_rnd = BreederParam[brd].personal_rnd;
	conpara->id_no = BreederParam[brd].id_no;
}

//=========================================================================
//	�Z�G�t�F�N�g�œG���̖��������铧��������o�^
//=========================================================================
void SetEnemyBanishActor(void)
{
	u8 poke;
	
	poke = AddBanishActor(DummyActMove);
	PokemonStruct[CONT_ENEMY_CLIENT] = poke;
	AffineNumSet(&ActWork[PokemonStruct[DefenceClient]]);	//���������ɂ�Affine�Ȱ�ެ���Ă��Ă����Ȃ��ƕʂ̖h�䑤�߹�݈ȊO�̏��Ɋg�k���������Ă��܂�
	EnemyBanishActorParamInit();
}

//=========================================================================
//	�Z�G�t�F�N�g�œG���̖��������铧�������̃p�����[�^��o�^
//=========================================================================
void EnemyBanishActorParamInit(void)
{
	actWork *xreg;
	
	xreg = &ActWork[PokemonStruct[CONT_ENEMY_CLIENT]];
	xreg->dx = 0;
	xreg->dy = 0;
	xreg->x = GetWazaEffPos(CONT_ENEMY_CLIENT, EFF_X);
	xreg->y = GetWazaEffPos(CONT_ENEMY_CLIENT, EFF_Y);
	xreg->banish = 1;
}

//=========================================================================
//	�ZNo���ݼނɂ���āADefenceClient�̒l��ݒ肷��(AttackCleint�͎����Œ�)
//=========================================================================
void SetRangeClient(u16 wazano)
{
	u8 range;
	
	range = WTD[wazano].attackrange;
	switch(range){
		case RANGE_MINE:
		case RANGE_ALWAYS:
			DefenceClient = CONT_MINE_CLIENT;
			break;
		case RANGE_NORMAL:
		case RANGE_NONE:
		case RANGE_RANDOM:
		case RANGE_DOUBLE:
		case RANGE_TRIPLE:
		default:
			DefenceClient = CONT_ENEMY_CLIENT;
			break;
	}
}







//=========================================================================
//	����G�ް����
//	tiiki_no = ��ýĉ��̏ꏊ(���p���ް���Ă���ꍇ��0xff, 
//			   	�ꎞ�I��UserWork�ɔ��p���ް���Ă���ꍇ��0xfe)
//	�߂�l: 1 = ��Ċ���		0 = ��Ăł��Ȃ�����
//=========================================================================
u8 PortraitDataSet(u8 tiiki_no)
{
	u8 no;
	int brd;
	u8 pict_rnd;
	
	pict_rnd = pp_rand() % 3;	//����т̎�̋��L������Ȃ��悤�ɔ���O�ɂ���Ă���
	
	for(brd = 0; brd < BREEDER_MAX-1; brd++)
	{
		if(ContestJyunni[brd] == 0)
			break;
	}
	if(tiiki_no == 0xff && brd != MyBreederNo)
		return 0;	//�������D�����ĂȂ��̂Ŕ��p�قɂ͏���Ȃ�
	
	switch(ContestKind)
	{
		case STYLE_CONTEST:
			pict_rnd += 0;
			break;
		case BEAUTIFUL_CONTEST:
			pict_rnd += 3;
			break;
		case CUTE_CONTEST:
			pict_rnd += 6;
			break;
		case CLEVER_CONTEST:
			pict_rnd += 9;
			break;
		case STRONG_CONTEST:
			pict_rnd += 12;
			break;
	}

	if(tiiki_no != 0xfe){
		no = PortraitDataSpaceSearch(tiiki_no, 1);
		Fld.PortraitData[no].personal_rnd = BreederParam[brd].personal_rnd;
		Fld.PortraitData[no].monsno = BreederParam[brd].monsno;
		Fld.PortraitData[no].idno = BreederParam[brd].id_no;
		PM_strcpy(Fld.PortraitData[no].nickname, BreederParam[brd].nickname);
		PM_strcpy(Fld.PortraitData[no].oyaname, BreederParam[brd].name);
		if(tiiki_no != 0xff)
			Fld.PortraitData[no].pict_type = ContestKind;
		else
			Fld.PortraitData[no].pict_type = pict_rnd;
	}
	else{	//UserWork�ɔ��p���ް����`��
		PORTRAIT_TEMP->personal_rnd = BreederParam[brd].personal_rnd;
		PORTRAIT_TEMP->idno = BreederParam[brd].id_no;
		PORTRAIT_TEMP->monsno = BreederParam[brd].monsno;
		PM_strcpy(PORTRAIT_TEMP->nickname, BreederParam[brd].nickname);
		PM_strcpy(PORTRAIT_TEMP->oyaname, BreederParam[brd].name);
		PORTRAIT_TEMP->pict_type = pict_rnd;
	}
	return 1;
}

//----------------------------------------------------------
//	��ýĉ��̏ꏊ���羯Ă��鎗��G�ް��̔z��̓Y�������擾����
//	tiiki_no = ��ýĉ��̏ꏊ(���p���ް���Ă���ꍇ��0xff)
//	slide_flg = �ް��̽ײ�ނ��s�������׸�(0:�s��Ȃ�	1:�s��)
//----------------------------------------------------------
u8 PortraitDataSpaceSearch(u8 tiiki_no, u8 slide_flg)
{
	int i;
	u8 ret = 0;
	
	switch(tiiki_no)				//�R���e�X�g���̏ꏊ
	{
		case 0:						//�n�W�c�Q
		case 1:						//�V�_�P
			ret = tiiki_no;
			break;
		case 2:						//�J�C�i
			if(slide_flg){
				for(i = 4; i > 2; i--)
					memcpy(&Fld.PortraitData[i], &Fld.PortraitData[i-1], 
						sizeof(PORTRAIT_DATA));
			}
			ret = 2;
			break;
		case 3:						//�~�i��
			if(slide_flg){
				for(i = 7; i > 5; i--)
					memcpy(&Fld.PortraitData[i], &Fld.PortraitData[i-1], 
						sizeof(PORTRAIT_DATA));
			}
			ret = 5;
			break;
		default:					//���p��
			switch(ContestKind)
			{
				case STYLE_CONTEST:
					ret = 8;
					break;
				case BEAUTIFUL_CONTEST:
					ret = 9;
					break;
				case CUTE_CONTEST:
					ret = 10;
					break;
				case CLEVER_CONTEST:
					ret = 11;
					break;
				case STRONG_CONTEST:
				default:
					ret = 12;
					break;
			}
	}
	return ret;
}



//----------------------------------------------------------
//	����G�ް�����̫�Ēl��ݒ�
//----------------------------------------------------------
static const PORTRAIT_DATA portrait_init_dmy[] = {
	{//0
		0,			//u32 personal_rnd;		//������
		0xffff,			//u32 idno;				//�|�P�����h�c�i���o�[
		MONSNO_SUBAME,			//u16 monsno;				//�|�P�����i���o�[
		3,			//u8	pict_type;			//�G�̎��
		{SU_,WA_,MI_,bou_,EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
		{KI_,YYO_,U_,I_,TI_,EOM_,},	//u8	oyaname[11];		//�e�̖��O
	},
	{//1
		0,			//u32 personal_rnd;		//������
		0xffff,			//u32 idno;				//�|�P�����h�c�i���o�[
		MONSNO_KINOGASSA,			//u16 monsno;				//�|�P�����i���o�[
		1,			//u8	pict_type;			//�G�̎��
		{PI_,NO_,PI_,EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
		{YA_,TTU_,KO_,EOM_,},	//u8	oyaname[11];		//�e�̖��O
	},
	{//2
		0,			//u32 personal_rnd;		//������
		0xffff,			//u32 idno;				//�|�P�����h�c�i���o�[
		MONSNO_PERIPPAA,			//u16 monsno;				//�|�P�����i���o�[
		0,			//u8	pict_type;			//�G�̎��
		{KA_,bou_,GO_,EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
		{MO_,N_,DO_,EOM_,},	//u8	oyaname[11];		//�e�̖��O
	},
	{//3
		0,			//u32 personal_rnd;		//������
		0xffff,			//u32 idno;				//�|�P�����h�c�i���o�[
		MONSNO_PERIPPAA,			//u16 monsno;				//�|�P�����i���o�[
		0,			//u8	pict_type;			//�G�̎��
		{KA_,bou_,GO_,EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
		{MO_,N_,DO_,EOM_,},	//u8	oyaname[11];		//�e�̖��O
	},
	{//4
		0,			//u32 personal_rnd;		//������
		0xffff,			//u32 idno;				//�|�P�����h�c�i���o�[
		MONSNO_PERIPPAA,			//u16 monsno;				//�|�P�����i���o�[
		0,			//u8	pict_type;			//�G�̎��
		{KA_,bou_,GO_,EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
		{MO_,N_,DO_,EOM_,},	//u8	oyaname[11];		//�e�̖��O
	},
	{//5
		0,			//u32 personal_rnd;		//������
		0xffff,			//u32 idno;				//�|�P�����h�c�i���o�[
		MONSNO_ENEKORORO,			//u16 monsno;				//�|�P�����i���o�[
		3,			//u8	pict_type;			//�G�̎��
		{E_,NE_,sa_,n_,EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
		{TO_,ME_,KI_,TI_,EOM_,},	//u8	oyaname[11];		//�e�̖��O
	},
	{//6
		0,			//u32 personal_rnd;		//������
		0xffff,			//u32 idno;				//�|�P�����h�c�i���o�[
		MONSNO_GOKURIN,			//u16 monsno;				//�|�P�����i���o�[
		2,			//u8	pict_type;			//�G�̎��
		{PA_,PI_,I_,EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
		{YU_,ME_,MI_,EOM_,},	//u8	oyaname[11];		//�e�̖��O
	},
	{//7
		0,			//u32 personal_rnd;		//������
		0xffff,			//u32 idno;				//�|�P�����h�c�i���o�[
		MONSNO_DOGOOMU,			//u16 monsno;				//�|�P�����i���o�[
		4,			//u8	pict_type;			//�G�̎��
		{DO_,RA_,U_,DO_,EOM_,},	//u8	nickname[11];		//�|�P�����̖��O
		{YU_,RA_,TO_,EOM_,},	//u8	oyaname[11];		//�e�̖��O
	},
};

void InitPortraitData(void)
{
	int i;
	
	for(i = 0; i < 8; i++)
		Fld.PortraitData[i] = portrait_init_dmy[i];
}


