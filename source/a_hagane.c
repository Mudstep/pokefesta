//=========================================================================
//	
//	�|�n �Z�G�t�F�N�g
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

#include "fight.h"
#include "decord.h"
#include "palanm.h"
#include "task.h"
#include "cli_def.h"
#include "poketool.h"
#include "ef_tool.h"
#include "contest.h"
#include "bss.h"




//=========================================================================
//
//		�u���^���N���[�v�A�u���肳���v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void SeqWazaMetaruInit(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------
#define	ANM_WAIT_H	4

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
static const actAnm MetaruAnm0[]={
		{OBJ16_32x32*0,ANM_WAIT_H,0,0},
		{OBJ16_32x32*1,ANM_WAIT_H,0,0},
		{OBJ16_32x32*2,ANM_WAIT_H,0,0},
		{OBJ16_32x32*3,ANM_WAIT_H,0,0},
		{OBJ16_32x32*4,ANM_WAIT_H,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm MetaruAnm1[]={
		{OBJ16_32x32*0,ANM_WAIT_H,1,0},
		{OBJ16_32x32*1,ANM_WAIT_H,1,0},
		{OBJ16_32x32*2,ANM_WAIT_H,1,0},
		{OBJ16_32x32*3,ANM_WAIT_H,1,0},
		{OBJ16_32x32*4,ANM_WAIT_H,1,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm *const MetaruAnmTbl[]={
		MetaruAnm0,
		MetaruAnm1,
};
const	actHeader ActMetaruKuroHeader={
	WAZACHR_ID_KIRISAKU,
	WAZACHR_ID_KIRISAKU,
	&WazaOamDataNormal[2],
	MetaruAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaMetaruInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:X�̾��  1:Y�̾�� 
// 		  2: 0=�E�������  1=���������
//-------------------------------------------------------------
static void SeqWazaMetaruInit(actWork *Xreg)
{
//	EffXpositionSet(Xreg, WazaEffWork[0]);
	Xreg->x += WazaEffWork[0];
	Xreg->y += WazaEffWork[1];
	ActAnmChg(Xreg, WazaEffWork[2]);
	
	Xreg->move = W_AnmEndWait;
	SetWorkSeq(Xreg, DelWazaEffect);
}











//=========================================================================
//
//		���^���n���ʃG�t�F�N�g
//												by matsuda 2002.03.19(��)
//=========================================================================
static void MetaruEffectMain(u8 id);
void MetaruEffectTask(u8 id);

//----------------------------------------------------------
//	offset 0: 0 = ��ٴ̪�Ă��I������������İ݂ɂȂ��Ă���װ�����ɖ߂�
//			�@1 = �̪�Ă��I�����Ă��߹�݂���İ݂̂܂�
//		   1:0=�ʏ�ʂ���İ݉��A1=�w�肵���F�ɂ���
//		   2:�w�肷��װ�BWazaEffWork[1]��1�̎��̂ݗL��
//----------------------------------------------------------
void MetaruEffectTask(u8 id)
{
	u8 pokeact, winact;
	u16 palno;
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
	DecordVram((void*)metaru_psc_ADRS, wazabg.ScrnAdrs);
	DecordVram((void*)metaru_pch_ADRS, wazabg.CharAdrs);
	DecordPaletteWork((void*)metaru_pcl_ADRS, wazabg.PlttNo*16, 0x20);
	ScrX1 = -ActWork[pokeact].x + 32 + 64;
	ScrY1 = -ActWork[pokeact].y + 32;

	//-- �|�P���������m�g�[���� --//
	palno = ActWork[pokeact].oamData.Pltt + 16;
	if(WazaEffWork[1] == 0)
		W_ColorMonotone(palno, 0);
	else
		SoftFade(palno*16, 16, 11, WazaEffWork[2]);
	
	TaskTable[id].work[0] = winact;
	TaskTable[id].work[1] = WazaEffWork[0];
	TaskTable[id].work[2] = WazaEffWork[1];
	TaskTable[id].work[3] = WazaEffWork[2];
	TaskTable[id].work[6] = pri_flg;
	TaskTable[id].TaskAdrs = MetaruEffectMain;
}

static void MetaruEffectMain(u8 id)
{
	u16 palno;
	u8 pokeact;
	WazaBgData wazabg;
	
	TaskTable[id].work[10] += 4;
	ScrX1 -= 4;
	if(TaskTable[id].work[10] == 128){
		TaskTable[id].work[10] = 0;
		ScrX1 += 128;
		if(++TaskTable[id].work[11] == 2){
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
			if(TaskTable[id].work[1] == 0)
				W_ColorMonotone(palno, 1);
		//	AffineDelActor(&ActWork[TaskTable[id].work[0]]);
			DelActor(&ActWork[TaskTable[id].work[0]]);
			GetWazaEffBgAdrs(&wazabg);
			DIV_DMACLEAR(3, 0, wazabg.ScrnAdrs, 0x800, 32);
			if(TaskTable[id].work[6] == 1){
				ActWork[PokemonStruct[AttackNo^2]].oamData.Priority++;
			}
			DelTaskEffect(id);
		}
	}
}








//=========================================================================
//
//		�|�P���������m�g�[���ɂ���E�߂�
//												by matsuda 2002.05.10(��)
//=========================================================================
//----------------------------------------------------------
//	offset	0: 0=�U���� 1=�h�䑤 2=�U�������� 3=�h�䑤����
//				�������I��p�����Ұ��ǉ�
//			    4=Mine1 5=Mine2 6=Enemy1 7=Enemy2
//			1: 0=���m�g�[���ɂ��� 1=���ɖ߂�
//----------------------------------------------------------
void PokeMonotoneTask(u8 id)
{
	u16 palno;
	u8 pokeact;
	u8 client_no, client_type = 0, type_flg = 0;
	
	switch(WazaEffWork[0]){
		case 0:
		case 1:
		case 2:
		case 3:
			pokeact = W_GetPokeActNo(WazaEffWork[0]);
			break;
		case 4:
			client_type = BSS_CLIENT_MINE;
			type_flg = 1;
			break;
		case 5:
			client_type = BSS_CLIENT_MINE2;
			type_flg = 1;
			break;
		case 6:
			client_type = BSS_CLIENT_ENEMY;
			type_flg = 1;
			break;
		case 7:
			client_type = BSS_CLIENT_ENEMY2;
			type_flg = 1;
			break;
		default:
			pokeact = 0xff;
	}
	
	if(type_flg){
		client_no = ClientNoGet(client_type);
		if(PokeBG2taimeCheck(client_no))
			pokeact = PokemonStruct[client_no];
		else
			pokeact = 0xff;
	}
	
	if(pokeact != 0xff){
		palno = ActWork[pokeact].oamData.Pltt + 16;
		W_ColorMonotone(palno, WazaEffWork[1]);
	}
	DelTaskEffect(id);
}



//=========================================================================
//
//		�͂߂̂˂���
//												by mori 2002.08.01
//=========================================================================

//----------------------------------------------------------
// �^�[���`�F�b�N�^�X�N
//----------------------------------------------------------
void TaskHametunoNegaiCheck(u8 no)
{
	
	if(WazaCounter == 0 || WazaCounter == 1) 	WazaEffWork[7] = 0;
	if(WazaCounter == 2 )							WazaEffWork[7] = 1;
	DelTaskEffect(no);

}