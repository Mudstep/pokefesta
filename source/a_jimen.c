//=========================================================================
//	
//	�n�ʌn �Z�G�t�F�N�g
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
#include "laster.h"
#include "client_t.h"





//=========================================================================
//
//		�z�l�u�[������
//												by matsuda 2002.02.04(��)
//=========================================================================
static void HoneBoomeranInit(actWork *xreg);
static void HoneBoomeranMain(actWork *xreg);
static void HoneBoomeranReturn(actWork *xreg);

#define HONE_BOOMERAN_ADDSEC	15

static const actAffAnm HoneBoomeranAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0, 0, HONE_BOOMERAN_ADDSEC, 1},
	{ACT_AFLOOP,0,0,0},
};
static const actAffAnm HoneBoomeranAff1[] = {	//�z�l����ڂ��p
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0, 0, 20, 1},
	{ACT_AFLOOP,0,0,0},
};
static const	actAffAnm * const HoneBoomeranAffTbl[] = {
	HoneBoomeranAff0,
	HoneBoomeranAff1,
};
const actHeader ActHoneBoomeranHeader = 
{
	WAZACHR_ID_HONE,
	WAZACHR_ID_HONE,
	&WazaOamDataAffine[2],
	DummyActAnmTbl,
	ACT_NTRANS,
	HoneBoomeranAffTbl,
	HoneBoomeranInit,
};

//----------------------------------------------------------
//	offset �Ȃ�
//----------------------------------------------------------
#define HONE_BOOMERAN_SYNC	20
#define HONE_BOOMERAN_DY	(-40)

static void HoneBoomeranInit(actWork *xreg)
{
	xreg->x = GetWazaEffPos(AttackNo, PM_X);
	xreg->y = GetWazaEffPos(AttackNo, PM_Y);
	xreg->work[0] = HONE_BOOMERAN_SYNC;
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	xreg->work[5] = HONE_BOOMERAN_DY;
	YamanariMoveInit(xreg);
	xreg->move = HoneBoomeranMain;
}

static void HoneBoomeranMain(actWork *xreg)
{
	if(YamanariMoveMain(xreg))
	{
		xreg->x += xreg->dx;
		xreg->y += xreg->dy;
		xreg->dx = xreg->dy = 0;
		xreg->work[0] = HONE_BOOMERAN_SYNC;
		xreg->work[2] = GetWazaEffPos(AttackNo, PM_X);
		xreg->work[4] = GetWazaEffPos(AttackNo, PM_Y);
		xreg->work[5] = -HONE_BOOMERAN_DY;
		YamanariMoveInit(xreg);
		xreg->move = HoneBoomeranReturn;
	}
}

static void HoneBoomeranReturn(actWork *xreg)
{
	if(YamanariMoveMain(xreg))
		DelWazaEffect(xreg);
}










//=========================================================================
//
//		�z�l����ڂ�
//												by matsuda 2002.02.05(��)
//=========================================================================
static void HoneKonbouInit(actWork *xreg);

const actHeader ActHoneKonbouHeader = 
{
	WAZACHR_ID_HONE,
	WAZACHR_ID_HONE,
	&WazaOamDataAffine[2],
	DummyActAnmTbl,
	ACT_NTRANS,
	&HoneBoomeranAffTbl[1],
	HoneKonbouInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset	4:Sync��
//----------------------------------------------------------
static void HoneKonbouInit(actWork *xreg)
{
	WazaDefaultPosSet1(xreg, PM_MODE);
	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];
	xreg->work[0] = WazaEffWork[4];
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X) + WazaEffWork[2];
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y) + WazaEffWork[3];
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}















//=========================================================================
//
//		�u���Ȃ����v
//														by matsuda
//=========================================================================
//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
static void SunaInit(actWork *Xreg);

//----------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
const actHeader ActSunaKakeHeader = 
{
	WAZACHR_ID_SUNA,
	WAZACHR_ID_SUNA,
	&WazaOamDataNormal[0],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	SunaInit,
};

static const actAnm dorokake_anm0[] =
{
	{OBJ16_8x8 * 1, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm * const DorokakeAnmTbl[] = 
{
	dorokake_anm0,
};
const actHeader ActDoroKakeHeader = //�u�ǂ납���v�Ŏg�p
{
	WAZACHR_ID_SUNA,
	WAZACHR_ID_SUNA,
	&WazaOamDataNormal[1],
	DorokakeAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	SunaInit,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

//-------------------------------------------------------------
// offset 0:X�̾��  1:Y�̾��	2:���B�܂ł�ٰ�߉�
//		  3:�I�_X�̾��(�I�_�Ɍ����Ă͓G�A�����ŵ̾�Ă̔��]���s��Ȃ�)
//		  4:�I�_Y�̾��
//-------------------------------------------------------------
static void SunaInit(actWork *Xreg)
{
	u8 end_x, end_y;
	s16 offs_x, offs_y;
	
	WazaDefaultPosSet2(Xreg, PM_MODE);
//	if(MineEnemyCheck(AttackNo))
//		WazaEffWork[3] = -WazaEffWork[3];
		
	end_x = GetWazaEffPos2(DefenceNo, PM_X);
	end_y = GetWazaEffPos2(DefenceNo, PM_Y);
	
	offs_x = pp_rand() % 32;
	offs_y = pp_rand() % 32;
	if(offs_x > 16)
		offs_x = -(offs_x - 16);
	if(offs_y > 16)
		offs_y = -(offs_y - 16);
		
	Xreg->work[0] = WazaEffWork[2];
	Xreg->work[2] = end_x + offs_x;//WazaEffWork[3];
	Xreg->work[4] = end_y + offs_y;//WazaEffWork[4];
	Xreg->move = W_StraightSyncMove;
	SetWorkSeq(Xreg, W_AffCheckEndSeq);
}











//=========================================================================
//
//		�ǂ날����
//														taya 2002.07.25
//=========================================================================
static void DoroAsobiAttackMove(actWork *act);
static void DoroAsobiDefenceMove(actWork *act);

//================================================-
//  0:(0:�U���������яo��  1:�ォ�痎���Ă���)
//  1:XOFS  2:YOFS
//================================================-
static void DoroAsobiActInit(actWork *act)
{
	act->oamData.CharNo += 1;
	if(WazaEffWork[0]==0){
		act->x = GetWazaEffPos(AttackNo, PM_X) + WazaEffWork[1];
		act->y = GetWazaEffPos(AttackNo, PM_Y) + WazaEffWork[2];
		act->work[0] = (WazaEffWork[1] > 0)? 1 : -1;
		act->move = DoroAsobiAttackMove;
	}else{
#if 0
		act->x = GetWazaEffPos(AttackNo, PM_X) + WazaEffWork[1] + WazaEffWork[1];
		act->y = W_GetPokeSize(AttackNo, POKESIZE_REAL_BOTTOM) + WazaEffWork[2];
		if(MineEnemyCheck(AttackNo)==SIDE_MINE){
			act->dy = -120;
		}else{
			act->dy = -60;
		}
#else
		act->x = WazaEffWork[1];
		act->y = WazaEffWork[2];
		act->dy = -(act->y);
#endif
		act->move = DoroAsobiDefenceMove;
	}
}

//================================
// �U��������
//================================
static void DoroAsobiAttackMove(actWork *act)
{
	if(++(act->work[1]) > 1){
		act->work[1] = 0;
		act->x += act->work[0];
	}
	act->y -= 4;

	if(act->y < -4){
		DelWazaEffect(act);
	}
}
//================================
// �h�䑤����
//================================
static void DoroAsobiDefenceMove(actWork *act)
{
	switch(act->work[0]){
	case 0:
		act->dy += 4;
		if(act->dy >= 0){
			act->dy = 0;
			act->work[0]++;
		}
		break;

	case 1:
		if(++(act->work[1]) >= 1){
			act->work[1] = 0;
			act->banish ^= 1;
			if(++(act->work[2]) == 10){
				DelWazaEffect(act);
			}
		}
		break;
	}
}


const actHeader DoroAsobiHeader = 
{
	WAZACHR_ID_SUNA,
	WAZACHR_ID_SUNA,
	&WazaOamDataNormal[1],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	DoroAsobiActInit,
};













//=========================================================================
//	�u���Ȃ��ق�v
//
//															by taya
//=========================================================================
static void anawohoru_setlaster(u8 bgno, s16 top, s16 bottom);

//----------------------------------------------
//	1��ݖ�
//  0:(0:���s  1:�I������)
//----------------------------------------------
static void TaskAnaWoHoruMain(u8 id);
static void TaskAnaWoHoruEnd(u8 id);

void TaskAnaWoHoru(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	if(WazaEffWork[0]==0){
		my->TaskAdrs = TaskAnaWoHoruMain;
	}else{
		my->TaskAdrs = TaskAnaWoHoruEnd;
	}
	(my->TaskAdrs)(id);
}
static void TaskAnaWoHoruMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[0]){
	case 0:
		my->work[10] = W_GetPokeActNo(0);      // 10:�߹�ݱ������ް
//		my->work[9] = GetPokeShadowYPos(AttackNo) - 32; 
		my->work[11] = GetPokeBGNo(AttackNo);  // 13:BGNo
		if(my->work[11] == 1){
			my->work[12] = ScrX1;
			my->work[13] = ScrY1;
		}else{
			my->work[12] = ScrX2;
			my->work[13] = ScrY2;
		}
		my->work[14] = GetPokeShadowYPos(AttackNo) - 32; // 14:�߹���
		my->work[15] = my->work[14] + 64;   // 15:�߹����
		if(my->work[14] < 0){
			my->work[14]=0;
		}
		ActWork[my->work[10]].banish =1;
		my->work[0]++;
		break;

	case 1:
		anawohoru_setlaster(my->work[11], my->work[14], my->work[15]);
		my->work[0]++;
		break;

	case 2:
		my->work[2] = (my->work[2] + 6) & 127;
		if(++(my->work[4]) > 2){
			my->work[4] = 0;
			my->work[3]++;
		}
		my->work[5] = my->work[3] + (sin_cos_table[my->work[2]] >> 4);

		if(my->work[11] == 1){
			ScrY1 = my->work[13] - my->work[5];
		}else{
			ScrY2 = my->work[13] - my->work[5];
		}
		if(my->work[5] >= 64){
			my->work[5] = (120 - my->work[14]);
			// ׽����~���Ă������Ȃ��悤�ɂ���
			if(my->work[11] == 1){
				ScrY1 = my->work[13] - my->work[5];
			}else{
				ScrY2 = my->work[13] - my->work[5];
			}
			ActWork[my->work[10]].dx = 272 - ActWork[my->work[10]].x;  // POKEBG_RESET �ŏo�Ă��Ȃ��悤��
			my->work[0]++;
		}
		break;

	case 3:
		LasterSys.TransType = LASTER_END;
		my->work[0]++;
		break;

	case 4:
		DelTaskEffect(id);
		ActWork[my->work[10]].banish =1;
		break;
	}
}
static void TaskAnaWoHoruEnd(u8 id)
{
	u8 no = W_GetPokeActNo(0);
	ActWork[no].banish = 1;
	ActWork[no].dx = 0;
	ActWork[no].dy = 0;

	if(GetPokeBGNo(AttackNo)==1)
	{
		ScrY1 = 0;
	}
	else
	{
		ScrY2 = 0;
	}

	DelTaskEffect(id);
}


//----------------------------------------------
//	2��ݖ�
//  work[0] : 0:������  1:���s
//----------------------------------------------
static void TaskAnaWoHoru2ndInit(u8 id);
static void TaskAnaWoHoru2ndRun(u8 id);

void TaskAnaWoHoru2nd(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	if(WazaEffWork[0] == 0)
	{
		my->TaskAdrs = TaskAnaWoHoru2ndInit;
	}
	else
	{
		my->TaskAdrs = TaskAnaWoHoru2ndRun;
	}

	(my->TaskAdrs)(id);
}
static void TaskAnaWoHoru2ndInit(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[0]){
	case 0:
		my->work[10] = W_GetPokeActNo(0);
		ActWork[my->work[10]].banish = 0;
		ActWork[my->work[10]].dx = 0;
		ActWork[my->work[10]].dy = 160 - (ActWork[my->work[10]].y);  // �����Ȃ��Ƃ���
//		PokeClientBanishMemory(AttackNo);
		my->work[0]++;
		break;

	case 1:
		DelTaskEffect(id);
		break;

	}
}
static void TaskAnaWoHoru2ndRun(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[0]){
	case 0:
		my->work[10] = W_GetPokeActNo(0);
		my->work[11] = GetPokeBGNo(AttackNo);  // 11:BGNo
		if(my->work[11] == 1){
			my->work[12] = ScrX1;
		}else{
			my->work[12] = ScrX2;
		}
		my->work[14] = GetPokeShadowYPos(AttackNo) - 32; // 14:�߹���
		my->work[15] = my->work[14] + 64;   // 15:�߹����
		my->work[0]++;
		break;

	case 1:
//		anawohoru_setlaster(my->work[11],my->work[14],my->work[15]);
		anawohoru_setlaster(my->work[11],0,my->work[15]);
		my->work[0]++;
		break;

	case 2:
		ActWork[my->work[10]].dy = 96;
		my->work[0]++;
		break;

	case 3:
		ActWork[my->work[10]].dy -= 8;
		if(ActWork[my->work[10]].dy==0)
		{
			LasterSys.TransType = LASTER_END;
			my->work[0]++;
		}
		break;

	case 4:
		DelTaskEffect(id);
		break;

	}
}
//=============================
// ���Ȃ��ق�p׽�����
//�i�߹�݂̉�����BG�\���Ȃ��j
//=============================
static void anawohoru_setlaster(u8 bgno, s16 top, s16 bottom)
{
	LasterData LD;
	s16 i, ScrX;

	if(bgno == 1){
		ScrX = ScrX1;
		LD.DestAdrs = (void*)REG_BG1HOFS;
	}else{
		ScrX = ScrX2;
		LD.DestAdrs = (void*)REG_BG2HOFS;
	}

	if(top < 0){
		top = 0;
	}

	for(i = top; i < bottom; i++){
		LasterBuffer[0][i] = ScrX;
		LasterBuffer[1][i] = ScrX;
	}
	while(i < 160)
	{
		LasterBuffer[0][i] = ScrX + 240;
		LasterBuffer[1][i] = ScrX + 240;
		i++;
	}

	LD.DMAControl = H_DMA_ENABLE16;
	LD.TransType = LASTER_NORMAL;
	LD.LoopCount = 0;

	LasterDmaSet(LD);
}


//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
#define ANAWOHORU_SUNA_SYNC  (18)

static void AnaWoHoruActInit(actWork *act);
static void AnaWoHoruActMove(actWork *act);

const actHeader ActAnaWoHoruHeader = 
{
	WAZACHR_ID_SUNA,
	WAZACHR_ID_SUNA,
	&WazaOamDataNormal[0],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	AnaWoHoruActInit,
};
//----------------------------------------------------------
//	������
//	0:�i0:�U����  1:�h�䑤�j  1:�i0:�E��  1:�����j
//	2: ���򋗗�   3:�c�򋗗�  4:��܂Ȃ�̂ӂ蕝
//	5:�ړ�sync��
//----------------------------------------------------------
static void AnaWoHoruActInit(actWork *act)
{
	u16 client;
	s16 xofs;

	client = (WazaEffWork[0] == 0)? AttackNo : DefenceNo;
	xofs = 24;
	if(WazaEffWork[1] == 1){
		xofs *= -1;
		WazaEffWork[2] *= -1;
	}
	act->x = GetWazaEffPos(client, PM_X) + xofs;
	act->y = GetPokeShadowYPos(client) + 30;

	act->work[0] = WazaEffWork[5];
	act->work[2] = act->x + WazaEffWork[2];
	act->work[4] = act->y + WazaEffWork[3];
	act->work[5] = WazaEffWork[4];
	YamanariMoveInit(act);
	act->move = AnaWoHoruActMove;
}
static void AnaWoHoruActMove(actWork *act)
{
	if(YamanariMoveMain(act)){
		DelWazaEffect(act);
	}
}
//=========================================
// �؂�ډB��
// 0:(0:�U����  1:�h�䑤) 
// 1:(0:����  1:�E��)
// 2:sync
//=========================================
static void KakushiActInit(actWork *act)
{
	u8 client = (WazaEffWork[0]==0)? AttackNo : DefenceNo;

	act->x = GetWazaEffPos(client, EFF_X)-16 + (WazaEffWork[1]*32);
	act->y = GetPokeShadowYPos(client)+32;

	act->oamData.CharNo += (WazaEffWork[1]*8);

	SetWorkSeq(act,DelWazaEffect);
	act->work[0] = WazaEffWork[2];
	act->move = W_WaitSeq;

}
const actHeader KakushiHeader = 
{
	WAZACHR_ID_KAKUSHI,
	WAZACHR_ID_KAKUSHI,
	&WazaOamDataNormal[6],
	DummyActAnmTbl,
	ACT_NTRANS,
	DummyActAffTbl,
	KakushiActInit,
};










//=========================================================================
//
//		�u������v�u�}�O�j�`���[�h�v�u�����v
//															by taya
//=========================================================================
#define JISIN_YURE_COUNT   (50)  // �ő�h�ꕝ���̗h���
#define JISIN_MAX_WAIT     (1)
#define JISIN_FADE_WAIT    (1)

static void TaskQuakeJimen(u8 id);
static void TaskQuakePoke(u8 id);


//============================================
// 0:�h��Ώ�(0:�U���߹ 1:�h���߹ 2:�U������ 3:�h�䑊�� 4:�S�߹ 5:�n��)
// 1:�h�ꕝ (0�Ȃ�З͂ɉ����ĕω�)
// 2:����h���H�i�ő�h�ꕝ���̉񐔂̂݁j
//============================================
void TaskQuake(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	my->work[15] = 3 + ((WazaEffWork[1])? WazaEffWork[1] : (WazaEffPower / 10));
	my->work[14] = my->work[15];
	my->work[3] = WazaEffWork[2];

	switch(WazaEffWork[0]){
	// �n��
	case 5:
		my->work[13] = ScrX3;
		my->TaskAdrs = TaskQuakeJimen;
		break;

	// �S�߹��
	case 4:
		{
			u16 i;
			my->work[13] = 0;
			for(i = 0; i < 4; i++){
				if(PokeBG2taimeCheck(i)){
					my->work[9+my->work[13]] = PokemonStruct[i];
					my->work[13]++;
				}
			}
			my->TaskAdrs = TaskQuakePoke;
		}
		break;

	// �w���߹��1��
	default:
		my->work[9] = W_GetPokeActNo(WazaEffWork[0]);
		if(my->work[9] == 0xff){
			DelTaskEffect(id);
			return;
		}
		my->work[13] = 1;
		my->TaskAdrs = TaskQuakePoke;
		break;
	}
}
//===================================
// �n�ʗh�炵
//===================================
static void TaskQuakeJimen(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[0]){
	// �ő�h�ꕝ�ł��΂炭�h��
	case 0:
		if(++(my->work[1]) > JISIN_MAX_WAIT){
			my->work[1] = 0;
			if((my->work[2] & 1) == 0){
				ScrX3 = my->work[13] + my->work[15];
			}else{
				ScrX3 = my->work[13] - my->work[15];
			}
			if(++(my->work[2]) == my->work[3]){
				my->work[2] = 0;
				my->work[14]--;
				my->work[0]++;
			}
		}
		break;

	// ���X�ɂ����܂�
	case 1:
		if(++(my->work[1]) > JISIN_FADE_WAIT){
			my->work[1] = 0;
			if((my->work[2] & 1) == 0){
				ScrX3 = my->work[13] + my->work[14];
			}else{
				ScrX3 = my->work[13] - my->work[14];
			}
			if(++(my->work[2]) == 4){
				my->work[2] = 0;
				if(--(my->work[14]) == 0){
					my->work[0]++;
				}
			}
		}
		break;

	// �I��
	case 2:
		ScrX3 = my->work[13];
		DelTaskEffect(id);
		break;
	}
}
//===================================
// �߹�ݗh�炵
//===================================
static void quake_poke(TASK_TABLE *my);
static void TaskQuakePoke(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	switch(my->work[0]){
	// �ő�h�ꕝ�ł��΂炭�h��
	case 0:
		if(++(my->work[1]) > JISIN_MAX_WAIT){
			my->work[1] = 0;
			quake_poke(my);
			if(++(my->work[2]) == my->work[3]){
				my->work[2] = 0;
				my->work[14]--;
				my->work[0]++;
			}
		}
		break;

	// ���X�ɂ����܂�
	case 1:
		if(++(my->work[1]) > JISIN_FADE_WAIT){
			my->work[1] = 0;
			quake_poke(my);
			if(++(my->work[2]) == 4){
				my->work[2] = 0;
				if(--(my->work[14]) == 0){
					my->work[0]++;
				}
			}
		}
		break;

	// �I��
	case 2:
		{
			u16 i;
			for(i = 0; i < my->work[13]; i++){
				ActWork[my->work[9+i]].dx = 0;
			}
			DelTaskEffect(id);
		}
		break;
	}
}
static void quake_poke(TASK_TABLE *my)
{
	u16 i;
	s16 yure;

	if((my->work[2] & 1) == 0){
		yure = (my->work[14] / 2) + (my->work[14] & 1);
	}else{
		yure = -(my->work[14] / 2);
	}

	for(i = 0; i < my->work[13]; i++)
	{
		ActWork[my->work[9+i]].dx = yure;
	}
}


//=======================================================
//  ϸ������ނ��ׯ�������邩�H����
//=======================================================
void TaskMagnitudeCheck(u8 id)
{
	// 100�ȏ���ׯ��
	WazaEffWork[15] = (WazaEffPower >= 100);
	DelTaskEffect(id);
}

//=======================================================
//  �n����w�i��Ώۃ|�P�����̐^���ɍ��킹��
//  0:(0:�U���߹ 1:�h���߹ 2:�U������ 3:�h�䑊���j
//  1:�^�X�N�v���C�I���e�B
//  2:�I�����ށiWazaEffWork[7]�ɂ��̒l�������Ă�����I���j
//=======================================================
static void TaskZiwareHaikeiPosSetMain(u8 id);
void TaskZiwareHaikeiPosSet(u8 id)
{
	TASK_TABLE *my;
	u8 client, task_id;

	client = (WazaEffWork[0]&1)? DefenceNo : AttackNo;
	if(WazaEffWork[0]>1){ client ^= 2; }

	task_id = AddTask(TaskZiwareHaikeiPosSetMain, WazaEffWork[1]);
	my = &TaskTable[task_id];

	my->work[1] = (32 - GetWazaEffPos(client,PM_X)) & 0x1ff;
	my->work[2] = (64 - GetWazaEffPos(client,PM_Y)) & 0xff;

	ScrX3 = my->work[1];
	ScrY3 = my->work[2];

	my->work[3] = WazaEffWork[2];

	DelTaskEffect(id);
}
static void TaskZiwareHaikeiPosSetMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];
	if(WazaEffWork[7]==my->work[3]){
		ScrX3 = 0;
		ScrY3 = 0;
		DelTask(id);
	}else{
		ScrX3 = my->work[1];
		ScrY3 = my->work[2];
	}

}
