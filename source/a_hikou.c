//=========================================================================
//	
//	��s�n �Z�G�t�F�N�g
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
#include "palanm.h"
#include "task.h"
#include "bss.h"







//=========================================================================
//
//		����������
//												by matsuda 2002.02.05(��)
//=========================================================================
static void KazeOkosiInit(actWork *xreg);
static void KazeOkosiMain(actWork *xreg);
static void KazeColorManageMain(u8 id);

#define KAZEOKOSI_INITSEC		191
#define KAZEOKOSI_SYNC			70
#define KAZEOKOSI_ADDSEC		((256+128)/KAZEOKOSI_SYNC)
#define KAZEOKOSI_FURIHABA_X	32
#define KAZEOKOSI_FURIHABA_Y	8

const	actHeader ActKazeOkosiHeader={
	WAZACHR_ID_KAZE,
	WAZACHR_ID_KAZE,
	&WazaOamDataNormal[11],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	KazeOkosiInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset
//----------------------------------------------------------
static void KazeOkosiInit(actWork *xreg)
{
	WazaDefaultPosSet1(xreg, EFF_MODE);
	xreg->y += 20;
	xreg->work[1] = KAZEOKOSI_INITSEC;
	xreg->move = KazeOkosiMain;
	xreg->move(xreg);
}

static void KazeOkosiMain(actWork *xreg)
{
	xreg->dx = SinMove(xreg->work[1], KAZEOKOSI_FURIHABA_X);
	xreg->dy = CosMove(xreg->work[1], KAZEOKOSI_FURIHABA_Y);
	xreg->work[1] = 0xff & (xreg->work[1] + KAZEOKOSI_ADDSEC);
	if(xreg->work[0]++ == KAZEOKOSI_SYNC)
		DelWazaEffect(xreg);
}

//----------------------------------------------------------
//	�����̃L�����̃p���b�g��҂��������
//	0:�װ��ݼނ�����Sync�Ԋu	1:�����Del����܂ł̳���
//----------------------------------------------------------
void KazeColorManageTask(u8 id)
{
	TaskTable[id].work[0] = WazaEffWork[1];
	TaskTable[id].work[1] = WazaEffWork[0];
	TaskTable[id].work[2] = ObjPalNumGet(WAZACHR_ID_KAZE);
	TaskTable[id].TaskAdrs = KazeColorManageMain;
}

static void KazeColorManageMain(u8 id)
{
	int i;
	u16 temp;
	u8 palno;
	
	if(TaskTable[id].work[10]++ == TaskTable[id].work[1])
	{
		TaskTable[id].work[10] = 0;
		palno = TaskTable[id].work[2];
		temp = PaletteWorkTrans[16*16+16*palno+1+7];
		for(i = 7; i > 0; i--)
			PaletteWorkTrans[16*16+16*palno+1+i] = PaletteWorkTrans[16*16+16*palno+1+i-1];
		PaletteWorkTrans[16*16+16*palno+1] = temp;
	}
	
	if(--TaskTable[id].work[0] == 0)
		DelTaskEffect(id);
}












//=========================================================================
//
//		�΂��ł���
//												by matsuda 2002.02.06(��)
//=========================================================================
static void TubasadeUtuInit(actWork *xreg);
static void TubasadeUtuMain(actWork *xreg);

#define TUBASAUTU_DEFAULT_AFF	(0xf0)
#define TUBASAUTU_AFFLOOP		24
#define TUBASAUTU_ADDAFF		(TUBASAUTU_DEFAULT_AFF / TUBASAUTU_AFFLOOP)
static const actAffAnm TubasadeUtuAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{ 0x100-TUBASAUTU_DEFAULT_AFF, 0x100, 0, ACT_AFSET },
	{ TUBASAUTU_ADDAFF, 0, 0, TUBASAUTU_AFFLOOP},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const TubasadeUtuAffTbl[] = {
	TubasadeUtuAff0,
};

const	actHeader ActTubasadeUtuHeader={
	WAZACHR_ID_KAZE,
	WAZACHR_ID_KAZE,
	&WazaOamDataAffine[11],
	DummyActAnmTbl,
	0,
	TubasadeUtuAffTbl,
	TubasadeUtuInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset	4:Sync��
//----------------------------------------------------------
static void TubasadeUtuInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];
	xreg->work[0] = WazaEffWork[4];
	xreg->work[1] = xreg->x;
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X) + WazaEffWork[2];
	xreg->work[3] = xreg->y;
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y) + WazaEffWork[3];
	W_GetStraightSyncParam(xreg);
	xreg->move = W_AffEndWait;
	SetWorkSeq(xreg, TubasadeUtuMain);
}

static void TubasadeUtuMain(actWork *xreg)
{
	if(W_ParallelSync(xreg))
		DelWazaEffect(xreg);
}










//=========================================================================
//
//		�G�A���u���X�g
//												by matsuda 2002.02.06(��)
//=========================================================================
static void EaroInit(actWork *xreg);

#define EARO_ANMWAIT	3
static const actAnm EaroAnm0[]={
	{OBJ16_32x16*0, EARO_ANMWAIT, 0, 0},
	{OBJ16_32x16*0, EARO_ANMWAIT, 1, 0},
	{OBJ16_32x16*0, EARO_ANMWAIT, 0, 1},
	{OBJ16_32x16*0, EARO_ANMWAIT, 1, 1},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm *const EaroAnmTbl[]={
		EaroAnm0,
};
const	actHeader ActEaroHeader={
	WAZACHR_ID_EARO,
	WAZACHR_ID_EARO,
	&WazaOamDataNormal[6],
	EaroAnmTbl,
	0,
	DummyActAffTbl,
	EaroInit,
};

//----------------------------------------------------------
//	offset	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//			4:Sync��	5:���WӰ�ޏ��8�ޯ�=�n�_  ����8�ޯ�=�I�_(0:PMӰ��	1:EFFӰ��)
//			6:AnmNo		7:0=ɰ�فA1=2�̑I�����[�h
//	offset	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//			4:Sync��	5:AnmNo		6:0=ɰ�فA1=2�̑I�����[�h
//----------------------------------------------------------
static void EaroInit(actWork *xreg)
{
	if(MineEnemyCheck(AttackNo)){
		WazaEffWork[0] = -WazaEffWork[0];
		WazaEffWork[1] = -WazaEffWork[1];
		WazaEffWork[2] = -WazaEffWork[2];
		WazaEffWork[3] = -WazaEffWork[3];
	}
	if(ContestCheck()){
		WazaEffWork[1] = -WazaEffWork[1];
		WazaEffWork[3] = -WazaEffWork[3];
	}
	xreg->x = GetWazaEffPos(AttackNo, PM_X);
	xreg->y = GetWazaEffPos(AttackNo, PM_Y);
	xreg->x += WazaEffWork[0];
	xreg->y += WazaEffWork[1];
	
	xreg->work[0] = WazaEffWork[4];

	if(WazaEffWork[6] == 0){
		xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
		xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	}
	else
		GetCenterEffPos(DefenceNo, PM_MODE, &xreg->work[2], &xreg->work[4]);
	xreg->work[2] += WazaEffWork[2];
	xreg->work[4] += WazaEffWork[3];
	
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
	
	ActAnmOffsChg(xreg, WazaEffWork[5]);
	//W_StraightMoveInit(xreg);
}

















//=========================================================================
//
//		������Ƃ�
//												by matsuda 2002.02.12(��)
//=========================================================================
static void SeqWazaSoraInit2(actWork *xreg);
static void SeqWazaSoraMain2(actWork *xreg);
static void SeqWazaSoraInit(actWork *xreg);
static void SeqWazaSoraMain(actWork *xreg);

#define SORA_DEFAULT_AFF	0x10
#define SORA_AFFLOOP		((0x100 - SORA_DEFAULT_AFF) / 40)
#define SORA_ADDAFF			((0x100-SORA_DEFAULT_AFF) / SORA_AFFLOOP)
static const actAffAnm SoraAff0[] = {	//1�^�[����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{SORA_DEFAULT_AFF, 0x100, 0, ACT_AFSET },
	{SORA_ADDAFF, 0, 0, SORA_AFFLOOP},
	{0, -0x20, 0, 5},
	{-0x10, 0x20, 0, 10},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const SoraAffTbl[] = {
	SoraAff0,
};

#define SORA_AFFTHETA	40
static const actAffAnm SoraTurn2Aff0[] = {	//2�^�[����	������
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0, SORA_AFFTHETA+10, 1},
	{ACT_AFEND,0,0,0},
};
static const actAffAnm SoraTurn2Aff1[] = {	//2�^�[����	�G��
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0,0, -SORA_AFFTHETA, 1},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const SoraTurn2AffTbl[] = {
	SoraTurn2Aff0,
	SoraTurn2Aff1,
};

const	actHeader ActSorawoTobu0Header={	//1�^�[����
	WAZACHR_ID_SORA2,
	WAZACHR_ID_SORA2,
	&WazaOamDataAffine2[3],
	DummyActAnmTbl,
	0,
	SoraAffTbl,
	SeqWazaSoraInit,
};
const	actHeader ActSorawoTobu1Header={	//2�^�[����
	WAZACHR_ID_SORA2,
	WAZACHR_ID_SORA2,
	&WazaOamDataAffine[3],
	DummyActAnmTbl,
	0,
	SoraTurn2AffTbl,
	SeqWazaSoraInit2,
};

//----------------------------------------------------------
//		1�^�[����
//	0:Xoffset	1:Yoffset	2:���˂܂ł̳���	3:�㏸���x(�Œ菭��)
//----------------------------------------------------------
static void SeqWazaSoraInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[2];
	xreg->work[1] = WazaEffWork[3];
	xreg->move = SeqWazaSoraMain;
	ActWork[W_GetPokeActNo(0)].banish = 1;
}

static void SeqWazaSoraMain(actWork *xreg)
{
	if(xreg->work[0] > 0)
		xreg->work[0]--;
	else
	{
		xreg->work[2] += xreg->work[1];
		xreg->dy -= xreg->work[2] >> 8;
	}
	
	if(xreg->y + xreg->dy < -32)
		DelWazaEffect(xreg);
}

//----------------------------------------------------------
//		2�^�[����
//	0:�G�̏��ɓ��B����܂ł�Sync��
//----------------------------------------------------------
static void SeqWazaSoraInit2(actWork *xreg)
{
	if(MineEnemyCheck(AttackNo))
	{
		xreg->x = 240+32;
		xreg->y = -32;
		ActAffAnmChg(xreg, 1);
	}
	else
	{
		xreg->x = -32;
		xreg->y = -32;
	}
	xreg->work[0] = WazaEffWork[0];
	xreg->work[1] = xreg->x;
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	xreg->work[3] = xreg->y;
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	W_GetStraightSyncParam(xreg);
	xreg->move = SeqWazaSoraMain2;
}

static void SeqWazaSoraMain2(actWork *xreg)
{
	xreg->work[0] = 1;
	W_ParallelSync(xreg);
	
	if(((u16)xreg->work[3] >> 8) > 200)
	{	//dx�͌Œ菭���̈�255�܂ł����l���g���Ȃ��̂ŉ�ʊO�ɐݒ肵��
		//�ꍇ�Ȃǂ͵��ް�۰��X���W���߂����肵�Ă��܂�����
		xreg->x += xreg->dx;
		xreg->dx = 0;
		xreg->work[3] &= 0x00ff;
	}

	if(xreg->x+xreg->dx > 240+32 || xreg->x+xreg->dx < -32 || xreg->y+xreg->dy > 160)
	{
		ActWork[W_GetPokeActNo(0)].banish = 0;
		DelWazaEffect(xreg);
	}
}

//=========================================================================
//
//		297:�t�F�U�[�_���X
//												by iwasawa	2002.07.27(Sad) add
//															2002.07.29(Mon)
//=========================================================================
static void SeqFeatherFallInit(actWork*);
static void SeqFeatherFall(actWork*);
static const actAnm	FeatherAnm[] = {
	{OBJ16_32x32*0,0,0,0},
	{ACT_ANMEND,0,0,0},
	{OBJ16_32x32*1,0,1,0},
	{ACT_ANMEND,0,0,0},
};
static const actAnm* const FeatherAnmTbl[] = {
	&(FeatherAnm[0]),
	&(FeatherAnm[2]),
};
const	actHeader ActFeatherDanceHeader={
	WAZACHR_ID_FEATHERDANCE,
	WAZACHR_ID_FEATHERDANCE,
	&WazaOamDataAffine[2],
	FeatherAnmTbl,
	0,
	DummyActAffTbl,
	SeqFeatherFallInit,
};
//��n��
void SeqWazaEffectEnd(actWork *xreg)
{
	if(xreg->work[0]-- > 0) return;

	if(xreg->oamData.AffineMode & 1){
		AffineWorkNumDel(xreg->oamData.AffineParamNo);
		xreg->oamData.AffineMode = 0;
	}
	DelActor(xreg);
	WazaEffectCount--;
}
//=====================================================================
//	�������ė�����H
//=====================================================================
typedef struct _FeatherFall{
	u16	wait_sw:1;		//0
	u16	width_sw:1;
	u16 pri_sw:1;
	u16 flip_sw:1;
	u16	old_area:4;
	u16	ct:8;

	u16	r;				//1
	
	s16 rv;				//2

	u16	v;				//3
	u16	h;				//4
	u16	s;				//5
	u8	w[2];			//6
	
	u16	pri:1;			//7
	u16 life:15;	
}_fFall;
//=====================================================================
//	������
//	0:����X�I�t�Z�b�g
//	1:����Y�I�t�Z�b�g,
//	2:H:�H�̏����p�x,L:�����p�x,
//	3:�p���x,
//	4:�������x,
//	5:H:�U�蕝�P,L:�U�蕝�O
//	6:�������~�߂�ʒu(EFF_Y����̃I�t�Z�b�g)
//	7:	H:�N���C�A���g(0:Defence,1:Attack),
//		L:�v���C�I���e�B(0:Up,1:Down,2:��)
//=====================================================================
#define FD_PRIDOWN	12
#define FD_WAIT		0
#define ANM_CHG(pAct){				\
	(pAct)->Hflip ^= 1;				\
	(pAct)->anm_no = (pAct)->Hflip;	\
	(pAct)->anm_sw = 1;				\
	(pAct)->anmend_sw = 0;			\
}							
#define FD_PRICHG(pAct,pWork){							\
	if((pWork)->pri_sw){								\
		if(sys.FieldBattleFlag){						\
			if((pWork)->pri == 0){						\
				(pAct)->oamData.Priority -= 1;			\
				(pWork)->pri ^= 1;						\
			}else{										\
				(pAct)->oamData.Priority += 1;			\
				(pWork)->pri ^= 1;						\
			}											\
		}else{											\
			if((pWork)->pri == 0){						\
				(pAct)->pri -= FD_PRIDOWN;				\
				(pWork)->pri ^= 1;						\
			}											\
			else{										\
				(pAct)->pri += FD_PRIDOWN;				\
				(pWork)->pri ^= 1;						\
			}											\
		}												\
	}													\
}
#define FD_WAITSET(pWork){	\
	(pWork)->wait_sw = 1;	\
	(pWork)->ct = FD_WAIT;	\
}
void SeqFeatherFallInit(actWork *xreg)
{
	u8		client = 0,r = 0,affNo = 0;
	s16		sin = 0;
	_fFall*	pWork = (_fFall *)xreg->work;
	
	//�N���C�A���g�����ɂ��邩�A��O�ɂ��邩
	if(WazaEffWork[7]  & 0x0100)	client = AttackNo;
	else							client = DefenceNo;
	
	//�����ʒu�����߂�
	if(!(MineEnemyCheck(client)))	WazaEffWork[0] = -WazaEffWork[0];	//�N���C�A���g����O�ɂ����甽�]
	xreg->x = GetWazaEffPos(client,EFF_X)+WazaEffWork[0];
	
	sin = GetWazaEffPos(client,EFF_Y);
	xreg->y = sin+WazaEffWork[1];
	pWork->h = (u16)(xreg->y) << 8;				//���݂�Y���Œ菭���ŕۑ�
	pWork->life = (u16)(sin+WazaEffWork[6]);	//�������~�߂�ʒu
	
	//���݂̃X�e�[�g��ݒ�
	pWork->pri_sw = 1;
	pWork->r	= (u16)(WazaEffWork[2]&0x00FF);	//���݂̊p�x
	pWork->s	= (u16)WazaEffWork[2] >> 8;		//�H�̌X��
	pWork->rv	=	   WazaEffWork[3];			//�p���x
	pWork->v	= (u16)WazaEffWork[4];			//Y�����̑��x 
	*((u16 *)pWork->w) = (u16)WazaEffWork[5];	//�U�蕝

	//�v���C�I���e�BGET
	if(pWork->r >= 64 && pWork->r < 192){	//�L�����̉�
		if(sys.FieldBattleFlag){
			xreg->oamData.Priority = GetPokeBGPri(client)+1;
		}else{
			xreg->oamData.Priority = GetPokeBGPri(client);
		}

		pWork->pri = 0;
		if((pWork->rv & 0x8000)==0) ANM_CHG(xreg);
	}
	else{	//�L�����̎�O
		xreg->oamData.Priority = GetPokeBGPri(client);

		pWork->pri = 1;
		if((pWork->rv & 0x8000)) ANM_CHG(xreg);
	}

	//���݂̃G���A�i���o�[��ۑ�
	pWork->old_area = pWork->r/64;

	//���݂�dx,dy�����߂�
	xreg->dx = (_Sin(pWork->r)*(s32)pWork->w[0])>>8;

	//���݂̉H�̌X����ݒ�
	affNo = xreg->oamData.AffineParamNo;
	r = ((-xreg->dx>>1) + pWork->s) & 0xFF;
	sin = _Sin(r);
	OBJ_AffineWork[ affNo ].H_DiffX = 
	OBJ_AffineWork[ affNo ].V_DiffY =  _Cos(r);
	OBJ_AffineWork[ affNo ].V_DiffX =  sin;
	OBJ_AffineWork[ affNo ].H_DiffY = -sin;

	xreg->move = SeqFeatherFall;
}
	
//=====================================================================
//	0:�J�E���^	16:wait_sw,15:width_sw,14,pri_sw,13:flip_sw,12-9:old_area,
//				8-1:counter
//	1:�p�x,2:�p���x(MSB:�t���O,���x),3:�������x(16bit�Œ菭��)
//	4:�H�̏����p�x
//	7:���C�t,
//=====================================================================
static void SeqFeatherFall(actWork *xreg)
{
	u8	r = 0,affNo = 0;
	s16	sin = 0;
	_fFall *pWork = (_fFall*)xreg->work;
	
	if(pWork->wait_sw){		//�E�F�C�g���͋A��
		if(pWork->ct-- > 0) return;
		pWork->wait_sw = 0;
		pWork->ct = 0;
		return;
	}

	//�X�e�[�g�ω��̃`�F�b�N
	switch(pWork->r/64){
	case 0:
		if((u8)pWork->old_area == 1){	//Wait&flipFlag On
			pWork->flip_sw = 1;
			FD_WAITSET(pWork);
		}else if((u8)pWork->old_area == 3){	//Wait&WidthChenge
			pWork->width_sw ^= 1;	
			FD_WAITSET(pWork);
		}else if(pWork->flip_sw){	//�د��&Priority����
			ANM_CHG(xreg);
			FD_PRICHG(xreg,pWork);
			pWork->flip_sw = 0;
		}
		pWork->old_area = 0;
		break;
	case 1:
		if((u8)pWork->old_area == 0){	//Wait & FlipFlag On
			pWork->flip_sw = 1;
			FD_WAITSET(pWork);
		}else if((u8)pWork->old_area == 2){	//Wait
			FD_WAITSET(pWork);
		}else if(pWork->flip_sw){	//�د��&Priority����
			ANM_CHG(xreg);
			FD_PRICHG(xreg,pWork);
			pWork->flip_sw = 0;
		}
		pWork->old_area = 1;
		break;
	case 2:
		if((u8)pWork->old_area == 3){	//Wait & FlipFlag on
			pWork->flip_sw = 1;
			FD_WAITSET(pWork);
		}else if((u8)pWork->old_area == 1){	//Wait
			FD_WAITSET(pWork);
		}else if(pWork->flip_sw){	//�د��&Priority����
			ANM_CHG(xreg);
			FD_PRICHG(xreg,pWork);
			pWork->flip_sw = 0;
		}
		pWork->old_area = 2;
		break;
	case 3:
		if((u8)pWork->old_area == 2){	//Wait & FlipFlag on
			pWork->flip_sw = 1;
		}else if((u8)pWork->old_area == 0){	//Wait & width change
			pWork->width_sw ^= 1;
			FD_WAITSET(pWork);
		}else if(pWork->flip_sw){		//�د��&Priority����
			ANM_CHG(xreg);
			FD_PRICHG(xreg,pWork);
			pWork->flip_sw = 0;
		}
		pWork->old_area = 3;
		break;
	}
	//���݂�dx�����߂�
	xreg->dx = ((s32)(pWork->w[pWork->width_sw]) * _Sin(pWork->r)) >> 8;

	//���݂̉H�̌X����ݒ�
	affNo = xreg->oamData.AffineParamNo;
	r = ((-xreg->dx>>1) + pWork->s) & 0xFF;
	sin = _Sin(r);
	OBJ_AffineWork[ affNo ].H_DiffX = 
	OBJ_AffineWork[ affNo ].V_DiffY =  _Cos(r);
	OBJ_AffineWork[ affNo ].V_DiffX =  sin;
	OBJ_AffineWork[ affNo ].H_DiffY = -sin;

	//Y�����̈ړ�����
	pWork->h += pWork->v;
	xreg->y = (s16)(pWork->h >> 8);

	//X�����̈ړ�����
	if(pWork->rv & 0x8000){		//���̊p�x���o��
		pWork->r = (pWork->r-(pWork->rv&0x7FFF)) & 0xFF;	
	}else{
		pWork->r = (pWork->r+(pWork->rv&0x7FFF)) & 0xFF;
	}
	
	if(xreg->y + xreg->dy < pWork->life) return;

	xreg->work[0] = 0;
	xreg->move = SeqWazaEffectEnd;
}

//=========================================================================
//
//		296:�~�X�g�{�[��
//												by iwasawa	2002.07.30(Tue) add
//															2002.07.30(Tue)
//=========================================================================
const u16 mistball_Palette[16]={
	0x464d,0x7e52,0x7f17,0x7f7b,0x7fbd,0x7fdd,0x7fff,0x7fdd,
	0x7fff,0x0000,0x0000,0x0000,0x0000,0x7f59,0x7f39,0x7f9c,
};
//----------------------------------------------------------
//	W_StraightMoveInit�̈������Ƃ�
//	offset	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//	4:Sync��		5:���WӰ��:���8�ޯ�=�n�_  ����8�ޯ�=�I�_(0:PMӰ��	1:EFFӰ��)
//----------------------------------------------------------
static void SeqMistBallInit(actWork *xreg)
{
	//�p���b�g�`�F���W
//	PaletteWorkSet(mistball_Palette,256+xreg->oamData.Pltt*16,0x20);

	//Priority
	xreg->oamData.Priority = GetPokeBGPri(DefenceNo);
	
	xreg->x = GetWazaEffPos(AttackNo,PM_X);
	xreg->y = GetWazaEffPos(AttackNo,PM_Y);
	xreg->move = W_StraightMoveInit;
}
const actHeader MistBall01Header = {
	WAZACHR_ID_MIZU2,
	WAZACHR_ID_MIZU2,
	&WazaOamDataNormal[1],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	SeqMistBallInit,
};
//------------------------------------------------------------
//	�~�X�g�{�[����2�i�K��
//	0:��ʁF�H�̊p�x,���ʁF��яo���p�x,
//	1:�����W(��ʁF�U�蕝,���ʁF��яo�����a),
//	2:(��ʁF�������̑��x�F���ʁF��яo�����x�j
//	3:�������I������W(�I�t�Z�b�gY)
//------------------------------------------------------------
static void SeqMistBallFeather01(actWork *xreg);
#define MB_CNT	0
#define MB_R	1
#define MB_W	2
#define MB_V	3
#define MB_WORK0	4
#define MB_WORK1	5
#define MB_WORK2	6
#define MB_LIFE		7
static void SeqMistBallFeatherInit(actWork *xreg)
{
	s16	sin = 0,r = 0;
	u8	affNo = 0;

	//�K�v�Ȓl��ۑ�
	xreg->work[MB_R]	= WazaEffWork[0];
	xreg->work[MB_W]	= WazaEffWork[1];
	xreg->work[MB_V]	= WazaEffWork[2];

	if(sys.FieldBattleFlag){
		if(ClientType[DefenceNo]&1){
			xreg->work[MB_LIFE]	= GetWazaEffPos(DefenceNo, EFF_Y) + WazaEffWork[3];
		}else{
			xreg->work[MB_LIFE] = GetWazaEffPos(DefenceNo, EFF_Y)+40;
		}
		//Priority
		if(WazaEffWork[4])	xreg->oamData.Priority = GetPokeBGPri(DefenceNo)+1;
		else				xreg->oamData.Priority = GetPokeBGPri(DefenceNo);
	}else{
		xreg->work[MB_LIFE]	= GetWazaEffPos(DefenceNo, EFF_Y) + WazaEffWork[3];
	}

	//��яo���p�x���o���Ă���
	xreg->work[MB_WORK0] =_Sin(xreg->work[MB_R] & 0xFF);
	xreg->work[MB_WORK1] =-(_Cos(xreg->work[MB_R] & 0xFF));
	xreg->work[MB_WORK2] = 0;
	xreg->dx = xreg->dy = 0;

	//�H�̏����p�x�ݒ�
	affNo = xreg->oamData.AffineParamNo;
	xreg->work[MB_R] = ((u16)xreg->work[MB_R]) >> 8;
	r = pp_rand();
	if(r & 0x8000) xreg->work[MB_R] = 255 - xreg->work[MB_R];
	sin = _Sin(xreg->work[MB_R]);
	OBJ_AffineWork[ affNo ].H_DiffX = 
	OBJ_AffineWork[ affNo ].V_DiffY =  _Cos(xreg->work[MB_R]);
	OBJ_AffineWork[ affNo ].V_DiffX =  sin;
	OBJ_AffineWork[ affNo ].H_DiffY = -sin;
	
	//�H�̌����������_���Ō���
	xreg->anm_sw = 1;
	xreg->anmend_sw = 0;
	if(r & 0x0001){
		xreg->anm_no = 1;
		xreg->Hflip = 1;
	}
	xreg->move = SeqMistBallFeather01;
}
static void SeqMistBallFeather01(actWork *xreg)
{
	u16		work[8];
	_fFall*	pWork = (_fFall *)xreg->work;
	
	xreg->work[MB_CNT]++;
	if(xreg->work[MB_CNT] < 5) return;
	
	xreg->dx = ( xreg->work[MB_WORK0] * xreg->work[ MB_WORK2 ] ) >> 8;
	xreg->dy = ( xreg->work[MB_WORK1] * xreg->work[ MB_WORK2 ] ) >> 8;
	
	xreg->work[MB_WORK2] += (xreg->work[MB_V]&0x00FF);
	if(xreg->work[MB_WORK2] < (xreg->work[MB_W]&0x00FF)) return;

	//���݈ʒu�����Z�b�g
	xreg->x += xreg->dx;
	xreg->y += xreg->dy;
	xreg->dx = 0;
	xreg->dy = 0;

	//FeatherFall�ֈ����p���p�����[�^���Z�b�g
	memcpy(work,xreg->work,16);
	memset(xreg->work,0,16);

	pWork->h = (u16)xreg->y << 8;	//���݂�Y���Œ菭���ŕۑ�
	pWork->v = work[MB_V] >> 8;		//�������x 
	pWork->r = 0;					//���݂̊p�x
	pWork->s = work[MB_R];			//�H�̌��݂̊p�x

	if(xreg->anm_no){
		if(pWork->v & 0x0008) pWork->rv = 0x8001;	//�p���x
		else				  pWork->rv = 0x8002;	//�p���x
	}
	else{
		if(pWork->v & 0x0008) pWork->rv = 0x0001;	//�p���x
		else				  pWork->rv = 0x0002;	//�p���x
	}

	pWork->w[0] = (u8)(work[MB_W] >> 8);	//�ӂ蕝
	pWork->w[1] = pWork->w[0]-2;							
	pWork->life = work[MB_LIFE];			//�����I�_

	xreg->move = SeqFeatherFall;
}
const actHeader MistBall02Header = {
	WAZACHR_ID_FEATHERDANCE,
	WAZACHR_ID_FEATHERDANCE,
	&WazaOamDataAffine[2],
	FeatherAnmTbl,
	0,
	DummyActAffTbl,
	SeqMistBallFeatherInit,
};
/*******************************************************************************

//----------------------------------------------------------
//	�萔
//----------------------------------------------------------
#define SORA_ANM_WAIT	4
#define SORA_UP_SP		7
//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
extern const actHeader ActTstHitRHeader;
extern const actHeader ActDenkouHeader;

//----------------------------------------------------------
//	�A�N�^�[�e�[�u��
//----------------------------------------------------------
static const actAnm SoraAnm0[]={
	{OBJ16_32x32 * 0, 2, 0, 0},
	{OBJ16_32x32 * 1, 2, 0, 0},
	{ACT_ANMLOOP, 0, 0, 0},
};
static const actAnm SoraAnm1[]={
	{OBJ16_32x32 * 2, SORA_ANM_WAIT, 0, 0},
	{OBJ16_32x32 * 3, SORA_ANM_WAIT, 0, 0},
	{OBJ16_32x32 * 4, SORA_ANM_WAIT, 0, 0},
	{OBJ16_32x32 * 5, SORA_ANM_WAIT, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm *const SoraAnmTbl[]={
		SoraAnm0,
		SoraAnm1,
};
const	actHeader ActSorawoTobuHeader={
	WAZACHR_ID_SORA,
	WAZACHR_ID_SORA,
	&WazaOamDataNormal[2],
	SoraAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaSoraInit,
};
//-- �����ۂ̕��� --//
static const actAnm SoraSubAnm0[]={
	{OBJ16_32x32*6, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm *const SoraSubAnmTbl[]={
		SoraSubAnm0,
};
const	actHeader ActSorawoTobuSubHeader={
	WAZACHR_ID_SORA,
	WAZACHR_ID_SORA,
	&WazaOamDataNormal[6],
	SoraSubAnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaSoraInit,
};
//-- �����ۂ̂���ɂ����� --//
static const actAnm SoraSub2Anm0[]={
	{OBJ16_32x32*6+OBJ16_32x8*1, 1, 0, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm *const SoraSub2AnmTbl[]={
		SoraSub2Anm0,
};
const	actHeader ActSorawoTobuSub2Header={
	WAZACHR_ID_SORA,
	WAZACHR_ID_SORA,
	&WazaOamDataNormal[5],
	SoraSub2AnmTbl,
	0,
	DummyActAffTbl,
	SeqWazaSoraInit,
};
//----------------------------------------------------------
//	�A�N�^�[�V�[�P���X
//----------------------------------------------------------
//----------------------------------------------------------
//	offset 0:Xoffset	1:Yoffset	2:�߰ԍ�(��Ҕԍ�)
//----------------------------------------------------------
static void SeqWazaSoraInit(actWork *Xreg)
{
	WazaDefaultPosSet2(Xreg, PM_MODE);
	ActAnmChg(Xreg, WazaEffWork[2]);
	ActWork[PokemonStruct[AttackNo]].banish = 1;
	
	Xreg->banish = 1;
	if(WazaEffWork[2] == 1)
		Xreg->banish = 0;
	Xreg->work[0] = SORA_ANM_WAIT * 3;
//	Xreg->move = SeqWazaSoraUp;
	Xreg->move = W_WaitSeq;
	SetWorkSeq(Xreg, SeqWazaSoraUp);
}

static void SeqWazaSoraUp(actWork *Xreg)
{
	Xreg->banish = 0;
	Xreg->dy -= SORA_UP_SP;
	if(Xreg->dy < -100)
	{
		DelWazaEffect(Xreg);
		ActWork[PokemonStruct[AttackNo]].banish = 0;
	}
}

//-------------------------------------------------------------
//		�^�X�N�V�[�P���X
//-------------------------------------------------------------
//----------------------------------------------------------
// offset: �Ȃ�
//----------------------------------------------------------
void TaskSorawoTobu(u8 taskno)
{
	if(1)	//���߂𒲂ׂ�
	{
		AddActorJump(&ActDenkouHeader, GetWazaEffPos(AttackNo, PM_X),
			GetWazaEffPos(AttackNo, PM_Y), 0);
		ActWork[PokemonStruct[AttackNo]].banish = 1;
	}
	else	//2�^�[����
	{
		WazaEffWork[0] = 0;
		WazaEffWork[1] = 0;
		AddActorJump(&ActTstHitRHeader, GetWazaEffPos(AttackNo, PM_X),
			GetWazaEffPos(AttackNo, PM_Y), 0);
	}
	DelTaskEffect(taskno);
}

//----------------------------------------------------------
//	offset:�Ȃ�
//----------------------------------------------------------
void TaskSoraEndChk(u8 taskno)
{
	if(1)	//�������
	{
		ActWork[PokemonStruct[AttackNo]].banish = 1;
	}
	DelTaskEffect(taskno);
}

**************************************************************************/












//=========================================================================
//
//		��]�n�G�t�F�N�g
//												by matsuda 2002.02.19(��)
//=========================================================================
static void KaitenInit(actWork *xreg);
static void KaitenMain(actWork *xreg);

#define KAITEN_ANMWAIT		1
#define KAITEN_ANM_MAX		5	//��]�n���ð��ق̍ő吔 ����ð��ق��������炱�������₷
static const actAnm KaitenAnm0[]={
	{OBJ16_32x16*0, KAITEN_ANMWAIT, 0, 0},
	{OBJ16_32x16*1, KAITEN_ANMWAIT, 0, 0},
	{OBJ16_32x16*2, KAITEN_ANMWAIT, 0, 0},
	{OBJ16_32x16*1, KAITEN_ANMWAIT, 1, 0},
	{OBJ16_32x16*0, KAITEN_ANMWAIT, 1, 0},
	{ACT_ANMEND, 0, 0, 0},
};
static const actAnm *const KaitenAnmTbl[]={
		KaitenAnm0,
};
const	actHeader ActKaitenHeader={
	WAZACHR_ID_KAITEN,
	WAZACHR_ID_KAITEN,
	&WazaOamDataNormal[6],
	KaitenAnmTbl,
	0,
	DummyActAffTbl,
	KaitenInit,
};

//----------------------------------------------------------
//	0:Xoffset	1:Yoffset	2:�U�����A�h�䑤�ǂ���ɏo����(0:�U����  1:�h�䑤)
//	3:Del����܂ł̳���		4:�J�n��ҵ̾��
//----------------------------------------------------------
#define KAITEN_DX	((64 << 8) / (KAITEN_ANMWAIT*5))
static void KaitenInit(actWork *xreg)
{
	if(WazaEffWork[2] == 0)
		WazaDefaultPosSet2(xreg, EFF_MODE);
	else
		WazaDefaultPosSet1(xreg, EFF_MODE);
	
	if((WazaEffWork[2] == 0 && MineEnemyCheck(AttackNo) == SIDE_MINE)
		|| (WazaEffWork[2] == 1 && MineEnemyCheck(DefenceNo) == SIDE_MINE))
		xreg->x += MINE_SIDE_OFFSET_Y;
	
	ActAnmOffsChg(xreg, WazaEffWork[4]);
	xreg->x += -32;
	xreg->work[1] = KAITEN_DX;
	xreg->dx += (xreg->work[1] >> 8) * (WazaEffWork[4] * KAITEN_ANMWAIT);
	xreg->work[0] = WazaEffWork[4] * KAITEN_ANMWAIT;
	xreg->work[7] = WazaEffWork[3];
	xreg->move = KaitenMain;
}

static void KaitenMain(actWork *xreg)
{
	xreg->dx += xreg->work[1] >> 8;
	if(xreg->work[0]++ == KAITEN_ANMWAIT * KAITEN_ANM_MAX)
	{
		xreg->work[0] = 0;
		xreg->dx = 0;
		ActAnmChg(xreg, 0);
	}
	
	if(xreg->work[7]-- == 0)
		DelWazaEffect(xreg);
}










//=========================================================================
//
//		�h���������΂�
//												by matsuda 2002.02.19(��)
//=========================================================================
extern const actHeader ActHitMarkHeader;
extern const actHeader ActHitMarkBlinkHeader;

#define DORIRU_HIT_SEC	32	//���b�㖈��˯�ϰ����o���� ��256������؂�鐔����
#define DORIRU_ADD_SEC	8	//��]���x�@��256������؂�鐔
#define DORIRU_FURIHABA	13
//----------------------------------------------------------
//	offset	�Ȃ�
//----------------------------------------------------------
void DoriruKutibasiTask(u8 id)
{
	if(TaskTable[id].work[0] % DORIRU_HIT_SEC == 0)
	{
		WazaEffectCount++;
		WazaEffWork[0] = SinMove(TaskTable[id].work[0], -DORIRU_FURIHABA);
		WazaEffWork[1] = CosMove(TaskTable[id].work[0], -DORIRU_FURIHABA);
		WazaEffWork[2] = 1;
		WazaEffWork[3] = 3;
//		AddActorJump(&ActHitMarkHeader, GetWazaEffPos(DefenceNo, PM_X),
//			GetWazaEffPos(DefenceNo, PM_Y), 3);
		AddActorJump(&ActHitMarkBlinkHeader, GetWazaEffPos(DefenceNo, PM_X),
			GetWazaEffPos(DefenceNo, PM_Y), 3);
	}
	TaskTable[id].work[0] += DORIRU_ADD_SEC;
	if(TaskTable[id].work[0] > 255)
		DelTaskEffect(id);
}


//=========================================================================
//
//		������
//												by matsuda 2002.07.26(��)
//=========================================================================
static void UkabuInit(actWork *xreg);
static void UkabuInit2(actWork *xreg);

#define UKABU_DEFAULT_AFF	0x10
#define UKABU_AFFLOOP		((0x100 - UKABU_DEFAULT_AFF) / 40)
#define UKABU_ADDAFF			((0x100-UKABU_DEFAULT_AFF) / UKABU_AFFLOOP)
static const actAffAnm UkabuAff0[] = {	//1�^�[����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{UKABU_DEFAULT_AFF, 0x100, 0, ACT_AFSET },
	{UKABU_ADDAFF, 0, 0, UKABU_AFFLOOP},
	{0, -0x20, 0, 5},
	//{-0x10, 0x20, 0, 10},
	{-0x14,0,0,7},
	{-0x14,-0x14,0,5},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const UkabuAffTbl[] = {
	UkabuAff0,
};

const	actHeader UkabuHeader={	//1�^�[����
	WAZACHR_ID_SORA2,
	WAZACHR_ID_SORA2,
	&WazaOamDataAffine2[3],
	DummyActAnmTbl,
	0,
	UkabuAffTbl,
	UkabuInit,
};

static const actAffAnm UkabuAff2[] = {	//2�^�[����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0xa0,0x100,0,ACT_AFSET},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const UkabuAffTbl2[] = {
	UkabuAff2,
};
const	actHeader UkabuHeader2={	//2�^�[����
	WAZACHR_ID_SORA2,
	WAZACHR_ID_SORA2,
	&WazaOamDataAffine2[3],
	DummyActAnmTbl,
	0,
	UkabuAffTbl2,
	UkabuInit2,
};

//----------------------------------------------------------
//		1�^�[����
//	0:Xoffset	1:Yoffset
//----------------------------------------------------------
static void UkabuInit(actWork *xreg)
{
	switch(xreg->work[0]){
		case 0:
			WazaDefaultPosSet2(xreg, PM_MODE);
			ActWork[W_GetPokeActNo(0)].banish = 1;
			xreg->work[0]++;
			break;
		case 1:
			if(xreg->affend_sw)
				DelWazaEffect(xreg);
			break;
	}
}

//----------------------------------------------------------
//	2��ݖ�
//	offset �Ȃ�
//----------------------------------------------------------
#define UKABU_SP	10
static void UkabuInit2(actWork *xreg)
{
	switch(xreg->work[0]){
		case 0:
			xreg->y = GetWazaEffPos(DefenceNo, EFF_Y);
			xreg->dy = -xreg->y - 32;
			xreg->work[0]++;
			break;
		case 1:
			xreg->dy += UKABU_SP;
			if(xreg->dy >= 0)
				xreg->work[0]++;
			break;
		case 2:
			xreg->dy -= UKABU_SP;
			if(xreg->y + xreg->dy < -32){
				ActWork[W_GetPokeActNo(0)].banish = 0;
				DelWazaEffect(xreg);
			}
			break;
	}
}









//=========================================================================
//
//		�_�C�r���O
//												by matsuda 2002.07.29(��)
//=========================================================================
static void DiveInit(actWork *xreg);
static void DiveUp(actWork *xreg);
static void DiveDown(actWork *xreg);

#define DIVE_DEFAULT_AFF	0x10
#define DIVE_AFFLOOP		((0x100 - DIVE_DEFAULT_AFF) / 40)
#define DIVE_ADDAFF			((0x100-DIVE_DEFAULT_AFF) / DIVE_AFFLOOP)
static const actAffAnm DiveAff0[] = {	//1�^�[����
//	{ �g�kx, �g�ky, �p�x, ���� }
	{DIVE_DEFAULT_AFF, 0x100, 0, ACT_AFSET },
	{DIVE_ADDAFF, 0, 0, DIVE_AFFLOOP},
	{0, -0x20, 0, 5},
	{-0x10, 0x20, 0, 10},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const DiveAffTbl[] = {
	DiveAff0,
};

const	actHeader DiveHeader={	//1�^�[����
	WAZACHR_ID_SORA2,
	WAZACHR_ID_SORA2,
	&WazaOamDataAffine2[3],
	DummyActAnmTbl,
	0,
	DiveAffTbl,
	DiveInit,
};

//----------------------------------------------------------
//	1��ݖ�
//	0:Xoffset	1:Yoffset	2:���˂܂ł̳���	3:�㏸���x(�Œ菭��)
//----------------------------------------------------------
static void DiveInit(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	xreg->work[0] = WazaEffWork[2];
	xreg->work[1] = WazaEffWork[3];
	xreg->move = DiveUp;
	ActWork[W_GetPokeActNo(0)].banish = 1;
}

static void DiveUp(actWork *xreg)
{
	if(xreg->work[0] > 0)
		xreg->work[0]--;
	else if(xreg->y + xreg->dy > -32)
	{
		xreg->work[2] += xreg->work[1];
		xreg->dy -= xreg->work[2] >> 8;
	}
	else{
		xreg->banish = 1;
		if(xreg->work[3]++ > 20)
			xreg->move = DiveDown;
	}
}

static void DiveDown(actWork *xreg)
{
	xreg->dy += xreg->work[2] >> 8;
	
	if(xreg->y + xreg->dy > -32)
		xreg->banish = 0;
	if(xreg->dy > 0)
		DelWazaEffect(xreg);
}


//=========================================================================
//	�����Ԃ�
//=========================================================================
static void MizusibukiInit(actWork *xreg);

#define MIZUSIBUKI_AFFLOOP	12
#define MIZUSIBUKI_AFFADD	(0x180 / MIZUSIBUKI_AFFLOOP)
static const actAffAnm MizusibukiAff0[] = {
//	{ �g�kx, �g�ky, �p�x, ���� }
	{0x0100, 0, 0, ACT_AFSET },
	{0, MIZUSIBUKI_AFFADD, 0, MIZUSIBUKI_AFFLOOP},
	{0, -MIZUSIBUKI_AFFADD, 0, MIZUSIBUKI_AFFLOOP - 1},
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const MizusibukiAffTbl[] = {
	MizusibukiAff0,
};
const	actHeader MizusibukiHeader={
	WAZACHR_ID_DIVE,
	WAZACHR_ID_DIVE,
	&WazaOamDataAffine2[3],
	DummyActAnmTbl,
	0,
	//MizusibukiAffTbl,
	DummyActAffTbl,
	MizusibukiInit,
};
//----------------------------------------------------------
//	0: 0=�U�����ɏo��	1=�h�䑤�ɏo��
//----------------------------------------------------------
static void MizusibukiInit(actWork *xreg)
{
	s32 dy;
	s32  height;
	u8 affno;

	switch(xreg->work[0]){
		case 0:
			if(WazaEffWork[0] == 0){
				xreg->x = GetWazaEffPos(AttackNo, EFF_X);
				xreg->y = GetWazaEffPos(AttackNo, EFF_Y);
			}
			else{
				xreg->x = GetWazaEffPos(DefenceNo, EFF_X);
				xreg->y = GetWazaEffPos(DefenceNo, EFF_Y);
			}
			xreg->work[1] = 0x0100 + 0x100;	//0x500;
			KakusyukuSet(xreg, 0, 0x0100, xreg->work[1], 0);
			xreg->work[0]++;
			break;
		case 1:
			if(xreg->work[2] < MIZUSIBUKI_AFFLOOP)
				xreg->work[1] -= 40;//MIZUSIBUKI_AFFADD;
			else
				xreg->work[1] += 40;//MIZUSIBUKI_AFFADD;
			xreg->work[2]++;
			KakusyukuSet(xreg, 0, 0x0100, xreg->work[1], 0);
		
			//-- Y���W�␳ --//
			height = 64;
			affno = xreg->oamData.AffineParamNo;

			dy = (((height-3) << 8) / OBJ_AffineWork[affno].V_DiffY) + 1;
			if(dy > (height*2)){ dy = (height*2); }
			dy = ((height - dy) / 2);

			xreg->dy = dy;
			
//			if(xreg->affend_sw)
			if(xreg->work[2] == MIZUSIBUKI_AFFLOOP*2){
				KakusyukuEnd(xreg);
				DelWazaEffect(xreg);
			}
			break;
	}
}


//=========================================================================
//	���H
//=========================================================================
static void SplashInit(actWork *xreg);
static void SplashMain(actWork *xreg);

const	actHeader SplashHeader={
	WAZACHR_ID_SPLASH,
	WAZACHR_ID_SPLASH,
	&WazaOamDataNormal[0],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	SplashInit,
};

//----------------------------------------------------------
//	0: 0 = �E�ֈړ�	1=���ֈړ�
//	1: 0=�U�����ɏo��	1=�h�䑤�ɏo��
//----------------------------------------------------------
#define SPLASH_DEFAULT_X	0x02e0
#define SPLASH_DEFAULT_Y	0x0380
#define SPLASH_SUB_X	0x0000
#define SPLASH_SUB_Y	0x0020
#define SPLASH_ENDWAIT	30
static void SplashInit(actWork *xreg)
{
	u16 x_offset, y_offset;
	
	x_offset = pp_rand() & 0x01ff;
	y_offset = pp_rand() & 0x007f;	//0x00ff;
	if(x_offset & 1)
		xreg->work[0] = SPLASH_DEFAULT_X + x_offset;
	else
		xreg->work[0] = SPLASH_DEFAULT_X - x_offset;
	if(y_offset & 1)
		xreg->work[1] = SPLASH_DEFAULT_Y + y_offset;
	else
		xreg->work[1] = SPLASH_DEFAULT_Y - y_offset;
	xreg->work[2] = WazaEffWork[0];
	if(WazaEffWork[0])
		xreg->oamData.AffineParamNo = 8;	//Hflip ON
	if(WazaEffWork[1] == 0){
		xreg->x = GetWazaEffPos(AttackNo, EFF_X);
		xreg->y = GetWazaEffPos(AttackNo, EFF_Y) + 32;
	}
	else{
		xreg->x = GetWazaEffPos(DefenceNo, EFF_X);
		xreg->y = GetWazaEffPos(DefenceNo, EFF_Y) + 32;
	}
	xreg->move = SplashMain;
}

static void SplashMain(actWork *xreg)
{
	if(xreg->work[2] == 0){
		xreg->dx += xreg->work[0] >> 8;
		xreg->dy -= xreg->work[1] >> 8;
	}
	else{
		xreg->dx -= xreg->work[0] >> 8;
		xreg->dy -= xreg->work[1] >> 8;
	}
	xreg->work[0] -= SPLASH_SUB_X;
	xreg->work[1] -= SPLASH_SUB_Y;
	if(xreg->work[0] < 0)
		xreg->work[0] = 0;
	
	if(xreg->work[3]++ == SPLASH_ENDWAIT)
		DelWazaEffect(xreg);
}















//=========================================================================
//
//	�S�b�h�o�[�h
//													by taya 2002.08.01
//=========================================================================

//======================================
// �T�[�`���C�g
//======================================
#define GBLIGHT_XADD  (4)
#define GBLIGHT_YADD  (1)
#define GBLIGHT_XSHIFT  (2)
#define GBLIGHT_YSHIFT  (2)
#define GBLIGHT_TENMETSU_WAIT  (8)
#define GBLIGHT_TENMETSU_MAX   (6)
#define GBLIGHT_MOVE_SYNC  (12)  // �����ړ�sync
static void GodBirdLightActMove(actWork *act);

static void GodBirdLightActInit(actWork *act)
{
	act->work[6] = 0;
	act->work[7] = 64;

	act->move = GodBirdLightActMove;
}
static void GodBirdLightActMove(actWork *act)
{
	switch(act->work[0]){
	case 0:
		if(++(act->work[1]) > GBLIGHT_TENMETSU_WAIT)
		{
			act->work[1] = 0;
			act->banish ^= 1;
			if(++(act->work[2]) >= GBLIGHT_TENMETSU_MAX
			&& act->banish == 1)
			
			{
				act->work[0]++;
			}
		}
		break;

	case 1:
		DelWazaEffect(act);
		break;
	}
}
const actHeader GodBirdLightHeader = {
	WAZACHR_ID_JUUDEN2,
	WAZACHR_ID_JUUDEN2,
	&WazaOamDataBldNormal[3],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	GodBirdLightActInit,
};
//======================================
// ����ɃK�[���Ɠ˂����ރA�N�^�[
//======================================
#define GODBIRD_SYNC  (12)
static void GodBirdActMove(actWork *act);

static void GodBirdActInit(actWork *act)
{
	u16 kakudo;
	s16 ex, ey;

	ex = act->x;
	ey = act->y;
	act->x = GetWazaEffPos(AttackNo, PM_X);
	act->y = GetWazaEffPos(AttackNo, PM_Y);

	act->work[4] = act->x << 4;
	act->work[5] = act->y << 4;
	act->work[6] = ((ex-act->x)<<4) / GODBIRD_SYNC;
	act->work[7] = ((ey-act->y)<<4) / GODBIRD_SYNC;

	kakudo = GetKakudo2(ex-act->x, ey-act->y);
	kakudo -= 0x4000;	//���̊G���c�Ȃ̂Ő^���ɂ���̵ׂ̾��
//	kakudo -= 0x2000;	//�G���̂�45�x�������Ă�̂ŕ␳
//	kakudo += 0x10000 / 2;	//�G���𔽓]
	KakusyukuSet(act, 1, 0x0100, 0x0100, kakudo);

	act->move = GodBirdActMove;
}
static void GodBirdActMove(actWork *act)
{
	act->work[4] += act->work[6];
	act->work[5] += act->work[7];

	act->x = act->work[4] >> 4;
	act->y = act->work[5] >> 4;

	if(act->x > 285 || act->x < -45
	|| act->y > 157 || act->y < -45)
	{
		W_AffCheckEndSeq(act);
	}
}

/*
static const actAffAnm GodBirdAff[] = {
	{0x100, 0x100, 0, ACT_AFSET },
	{ACT_AFEND,0,0,0},
};
static const	actAffAnm * const GodBirdAffTbl[] = {
	GodBirdAff,
};
*/
const actHeader GodBirdHeader = {
	WAZACHR_ID_GODBIRD,
	WAZACHR_ID_GODBIRD,
	&WazaOamDataAffine2[3],
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	GodBirdActInit,
};



//======================================
// 2��ݖځA�U���������������
// 0(0:������  1:�I��)
//======================================
void TaskGodBirdAttack(u8 id)
{
	if(WazaEffWork[0] == 0){
		u8 no = W_GetPokeActNo(0);
		ActWork[no].banish = 1;
	}else{
		u8 no = W_GetPokeActNo(0);
		ActWork[no].banish = 0;
	}
	DelTaskEffect(id);
}
