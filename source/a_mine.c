//----------------------------------------------------------
//	�����|�P��������	by	soga
//----------------------------------------------------------

#include "common.h"
#include "actor.h"
#include "actanm.h"
#include "MaDefine.h"
#include "ef_tool.h"
#include "calctool.h"

#include "gauge.h"
#include "client.h"
#include "bss.h"
#include "server.h"
#include "a_mine.h"
#include "cli_def.h"


//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------

void SeqMinePokeAppearInit(actWork *);
void SeqMinePokeAppearMain(actWork *);
void SeqMinePokeWait(actWork *);
void SeqMinePokeWaitInit(actWork *actst);
static void SelectMoveMain(actWork *xreg);

//actHeader MineHeader; 


//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
const ActOamData MineOamData = {
	0,		//u32 VPos:8;             // �x���W
	1,		//u32 AffineMode:2;       // �A�t�B�����[�h
	0,		//u32 ObjMode:2;          // �n�a�i���[�h
	OFF,	//u32 Mosaic:1;           // ���U�C�N
	0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
	0,		//u32 Shape:2;            // �n�a�i�`��

	0,		//u32 HPos:9;             // �w���W
	0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
	3,		//u32 Size:2;             // �n�a�i�T�C�Y

	0,		//u32 CharNo:10;          // �L�����N�^�m��
	POKE_OAM_PRI,		//u32 Priority:2;         // �\���D��
	2,		//u32 Pltt:4;             // �p���b�g�m��
	0,
};

//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/
const actAnm AnmMineAppear[] =
{
	{0, 5, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};

const actAnm * const MineAnmTbl[] = 
{
	AnmMineAppear,
};

const actAffAnm act_mine_aff_anm[] = {

//	{ �g�kx, �g�ky, �p�x, ���� }
	{ -16, 0, 0, 4 },					// ���ɐL�т�
	{ 0, 0, 0, 60 },
	{ACT_AFLOOP,1,0,0},					// ٰ�� , �߂��
};

const	actAffAnm * const act_mine_aff_tbl[] = {
	act_mine_aff_anm,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

void SeqMinePokeAppearInit(actWork *actst)
{
	//actst->dx=200;
	actst->move=SeqMinePokeAppearMain;
}
void SeqMinePokeAppearMain(actWork *actst)
{
	if(EncountEffectFlag&ENCOUNT_EFFECT_FLAG_OBJSTOP) return;
	actst->dx-=ENCOUNT_SPEED;
	if(actst->dx==0){
		actst->move=SeqMinePokeWait;
		actst->work[1]=0;
	}
}
void SeqMinePokeWaitInit(actWork *actst)
{
	//GaugeON(ClientGaugeSt[actst->work[0]]);
//	GaugeON(ClientExpGaugeSt[actst->work[0]]);

//	NumberON(ClientHPNumSt[actst->work[0]]);
//	NumberON(ClientHPMAXNumSt[actst->work[0]]);
//	NumberON(ClientLVNumSt[actst->work[0]]);
	
	actst->move = SeqMinePokeWait;
}
void SeqMinePokeWait(actWork *actst)
{
	int	i;

	i=0;
	i=actst->work[1];

//	actst->work[1]+=4;
//	actst->dy=sin_cos_table[actst->work[1] & 0xff] / 0x80;
}


//-- ���ٰ�� --------
void SeqMinePokeMove(actWork *actst)
{
	if(EncountEffectFlag&ENCOUNT_EFFECT_FLAG_OBJSTOP) return;
	actst->dx += actst->work[1];
	actst->dy += actst->work[2];
}



//=========================================================================
//	�I�𒆂̂���炷�铮��
//=========================================================================
//----------------------------------------------------------
//	����炷�铮����Ă���
// poke_gauge = �h�炷�Ώ۱���(MOTION_POKE:�߹�݁@MOTION_GAUGE:�ް��)
//----------------------------------------------------------
//void SelectMotionSet(u8 actno, s8 addsec, s8 furihaba)
void SelectMotionSet(u8 client_no, u8 poke_gauge, s8 addsec, s8 furihaba)
{
	u8 no;
	u8 taisyou;
	
	if(poke_gauge){
		if(CliSys[client_no].gauge_motion_set)
			return;
	}else{
		if(CliSys[client_no].poke_motion_set)
			return;
	}
	
	//������Ǝ��s���ɉ�ʐ؂�ւ������������TaskInit�������炸��
	//���̂܂܎��s����Â��Ă��܂��̂ű����ōs��
	no = AddBanishActor(SelectMoveMain);
	if(poke_gauge == MOTION_GAUGE){
		taisyou = ClientGaugeSt[client_no];
		CliSys[client_no].gauge_motion_no = no;
		CliSys[client_no].gauge_motion_set = 1;
		//�ް�ނ��߹�݂œ������������Ȃ��悤�ɂ���
		ActWork[no].work[0] = 256-64*2;
	}else{
		taisyou = PokemonStruct[client_no];
		CliSys[client_no].poke_motion_no = no;
		CliSys[client_no].poke_motion_set = 1;
		ActWork[no].work[0] = 256-64;
	}
//	ActWork[no].work[0] = 256-64;
	ActWork[no].work[1] = addsec;
	ActWork[no].work[2] = furihaba;
	ActWork[no].work[3] = taisyou;
	ActWork[no].work[4] = poke_gauge;
	ActWork[taisyou].dx = 0;
	ActWork[taisyou].dy = 0;
}

//----------------------------------------------------------
//	����炵�Ă�̂��~�߂�
// poke_gauge = �h�炷�Ώ۱���(0:�߹�݁@1:�ް��)
//----------------------------------------------------------
void SelectMotionStop(u8 client_no, u8 poke_gauge)
{
	u8 taisyou;
	
	if(poke_gauge == MOTION_GAUGE){
		if(CliSys[client_no].gauge_motion_set == 0)
			return;
		taisyou = ActWork[CliSys[client_no].gauge_motion_no].work[3];
		DelActor(&ActWork[CliSys[client_no].gauge_motion_no]);
		CliSys[client_no].gauge_motion_set = 0;
	}else{
		if(CliSys[client_no].poke_motion_set == 0)
			return;
		taisyou = ActWork[CliSys[client_no].poke_motion_no].work[3];
		DelActor(&ActWork[CliSys[client_no].poke_motion_no]);
		CliSys[client_no].poke_motion_set = 0;
	}
	ActWork[taisyou].dx = 0;
	ActWork[taisyou].dy = 0;
}

static void SelectMoveMain(actWork *xreg)
{
	u8 taisyou;
	
	taisyou = xreg->work[3];
	if(xreg->work[4] == MOTION_GAUGE)
		ActWork[taisyou].dy = SinMove(xreg->work[0], xreg->work[2]) + xreg->work[2];
	else
		ActWork[taisyou].dy = SinMove(xreg->work[0], xreg->work[2]) + xreg->work[2];
	xreg->work[0] = 0xff & (xreg->work[0] + xreg->work[1]);
}


