//=========================================================================
//
//		�Z�G�t�F�N�g�c�[��
//														
//=========================================================================

#include "common.h"
#include "ef_tool.h"
#include "calctool.h"
#include "actor.h"
#include "actanm.h"

#include "server.h"	
#include "waza_eff.h"
#include "cli_def.h"
#include "wazatool.h"

#include "fight.h"
#include "bss.h"
#include "contest.h"
#include "objdata.h"
#include "poketool.h"
#include "palanm.h"
#include "cli_num.h"
#include "madefine.h"
#include "decord.h"
#include "extanm.h"

#include "task.h"
#include "monsno.def"
#include "client.h"
#include "pm2fenum.h"
#include "pm2benum.h"

//=========================================================================
//	�v���g�^�C�v�錾
//=========================================================================
void SetWorkSeq(actWork *Xreg, void *SeqAdr);
void SetMoveSeq(actWork *Xreg);
void W_ParallelMoveSeq(actWork *Xreg);
void W_GetStraightMoveSp(actWork *Xreg);

void W_GetStraightMoveLoop(actWork *Xreg);
void W_ParallelMove2Seq(actWork *Xreg);

u8	MineEnemyCheck(u8 client_no);		//Add by soga 2001.09.25
u8	ClientTypeGet(u8 client_no);		//Add by soga 2001.09.26
u8	ClientNoGet(u8 client_type);		//Add by soga 2001.09.26

void W_GetStraightSyncParam(actWork *Xreg);
void W_StraightSyncMove(actWork *Xreg);
u8 W_ParallelSync(actWork *Xreg);
void W_ParallelSyncMove(actWork *Xreg);
static void W_YamanariSubSeq(actWork *xreg);
void W_GetStraightSyncParam4(actWork *Xreg);
void W_StraightSyncMove4(actWork *Xreg);
u8 W_ParallelSync4(actWork *Xreg);
void W_ParallelSyncMove4(actWork *Xreg);
void W_GetStraightSpParam4(actWork *xreg);
void W_StraightSpMove4(actWork *Xreg);
void PokePriAllInit(void);
u8 GetPokeSoftPri(u8 client_no);
u8 GetWazaEffPos2(u8 client_no, u8 x_y);
u8 GetWazaEffPokeYPos(u8 client_no, u16 monsno, u8);
u8 RatioXFlipCheck(void);

//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
static const	actHeader WazaPokeHeader[];
static const CellData WazaPokeCell[];

//----------------------------------------------------------
//	�O���ް�
//----------------------------------------------------------
extern const actAffAnm * const PmAffAnmTblF2[];



//=========================================================================
//	�Z�G�t�F�N�g�p �n�_�E�I�_���W�f�[�^
//=========================================================================
const WazaPosData WazaEffPos[][4] = 
{
	//FightType	1vs1
	{
		//ClientNo
		{C_POKE_X, C_POKE_Y},
		{CAI_POKE_X, CAI_POKE_Y},
		{CON_DEFENCE_X,CON_DEFENCE_Y},
		{CON_ATTACK_X,	CON_ATTACK_Y},//1��1�ł͎g���Ȃ��̂ź�ýėp�̍��W��u���Ă���
	},
	//FightType 2vs2
	{
		{C_POKE1X,		C_POKE1Y},
		{CAI_POKE1X,	CAI_POKE1Y},
		{C_POKE2X,		C_POKE2Y},
		{CAI_POKE2X,	CAI_POKE2Y},
	},
};

//=========================================================================
//	�v���O����
//=========================================================================

//----------------------------------------------------------
//	�G�t�F�N�g�n�_�A�I�_���W�擾
// x_y = PM_X or PM_X : X���W  	PM_Y or PM_Y : Y���W
// x_y = POKE_*:�߹�̵݂̾�č��݂ł̍��W���擾�B
//		 ���X���߹�݂̑傫���ɂ���č��W���ς��A�G�̑傫��������Ă����
//		   ���̊G�ɂ������߹�݂̒��S�̍��W������B
//		 EFF_*:�߹�̵݂̾�Ă͊֌W�Ȃ��ŏ�ɋ��ʂ̒��S���W���擾����
//			�ǂ��߹�݂ɂ����Ă����ʂ̍��W���K�v�Ȏ��ȂǂɎg�p����
//----------------------------------------------------------

#if 0
const u8 PowawaEYPos[]={
	20,11,12,11
};
#endif
const pmPara PowawaParaTbl[]={
	{0x44,17},
	{0x66, 9},
	{0x46, 9},
	{0x86, 8},
};

const u8 PowawaYOfsTbl[]={
//	16,6,6,5
	13,14,13,13
};

const u8 PowawaMYPos[]={
	0,0,0,0
};

u8 GetWazaEffPos(u8 client_no, u8 x_y)
{
//	s16 offsetY;
	u8 ret;
	u16 monsno;
	
	if(ContestCheck() && x_y == PM_Y && client_no == CONT_ENEMY_CLIENT)
		x_y = EFF_Y;
	
	switch(x_y)
	{
		case PM_X:
		case EFF_X:
			ret = WazaEffPos[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(client_no)].x;
			break;
		case EFF_Y:
			ret = WazaEffPos[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(client_no)].y;
			break;
		case PM_Y:
		case PM_DEFAULT_Y:
		default:
			if(ContestCheck())
			{
				if(conpara->hensin_flg)
					monsno = conpara->hensin_monsno;
				else
					monsno = conpara->monsno;
			}
			else if(MineEnemyCheck(client_no))
			{
				if(Climem[client_no].hensin_monsno == 0)
					monsno = PokeParaGet(
							&PokeParaEnemy[SelMonsNo[client_no]], ID_monsno);
//					monsno = WazaEffMonsNo[client_no];
				else
					monsno = Climem[client_no].hensin_monsno;
			}
			else
			{
				if(Climem[client_no].hensin_monsno == 0)
					monsno = PokeParaGet(
							&PokeParaMine[SelMonsNo[client_no]], ID_monsno);
//					monsno = WazaEffMonsNo[client_no];
				else
					monsno = Climem[client_no].hensin_monsno;
			}
			if(x_y == PM_Y)
				ret = GetWazaEffPokeYPos(client_no, monsno, 1);
			else
				ret = GetWazaEffPokeYPos(client_no, monsno, 0);
	}
	return ret;
}

//----------------------------------------------------------
//	monsno����|�P������PM_Y���W���擾
//	offset_flg = 0:���@���߹�݂ւ̵̾�ĂȂ��@1:�̾�Ă���
//----------------------------------------------------------

static	u8	PokeYPosGet(u8 client_no,u16 monsno)
{
	u16	annon_rnd;
	u32	rnd;
	u8	ypos=0;

	if(MineEnemyCheck(client_no) == SIDE_MINE || ContestCheck()){
		if(monsno==MONSNO_ANNOON){
			if(ContestCheck()){
				if(conpara->hensin_flg)
					rnd = conpara->hensin_personal_rnd;
				else
					rnd = conpara->personal_rnd;
			}
			else{
				if(Climem[client_no].hensin_monsno==0)
					rnd=PokeParaGet(&PokeParaMine[SelMonsNo[client_no]],ID_personal_rnd);
				else
					rnd=HenshinRnd[client_no];
			}
			annon_rnd=(((rnd&0x03000000)>>18)|((rnd&0x030000)>>12)|((rnd&0x0300)>>6)|(rnd&0x03))%28;
			if(annon_rnd==0)
				annon_rnd=monsno;
			else
				annon_rnd+=(CELLID_pm3b_unb-1);
			ypos=pm2bpara[annon_rnd].patcnt;
		}
		else if(monsno == MONSNO_POWARUN)
			ypos=PowawaMYPos[BattlePokeAnmNo[client_no]];
		else if(monsno>MONSNO_END)
			ypos=pm2bpara[0].patcnt;
		else
			ypos=pm2bpara[monsno].patcnt;
	}
	else{
		if(monsno==MONSNO_ANNOON){
			if(Climem[client_no].hensin_monsno==0)
				rnd=PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_personal_rnd);
			else
				rnd=HenshinRnd[client_no];
			annon_rnd=(((rnd&0x03000000)>>18)|((rnd&0x030000)>>12)|((rnd&0x0300)>>6)|(rnd&0x03))%28;
			if(annon_rnd==0)
				annon_rnd=monsno;
			else
				annon_rnd+=(CELLID_pm3f_unb-1);
			ypos=pm2fpara[annon_rnd].patcnt;
		}
		else if(monsno == MONSNO_POWARUN)
//			ypos=PowawaEYPos[BattlePokeAnmNo[client_no]];
			ypos=PowawaParaTbl[BattlePokeAnmNo[client_no]].patcnt;
		else if(monsno>MONSNO_END)
			ypos=pm2fpara[0].patcnt;
		else
			ypos=pm2fpara[monsno].patcnt;
	}
	return ypos;
}

static	u8	PokeYOfsGet(u8 client_no,u16 monsno)
{
//	u16	annon_rnd;
//	u32	rnd;
	u8	yofs=0;

	if(MineEnemyCheck(client_no)==SIDE_ENEMY && ContestCheck() == 0){
#if 0
		if(monsno==MONSNO_ANNOON){
			rnd=PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]],ID_personal_rnd);
			annon_rnd=(((rnd&0x03000000)>>18)|((rnd&0x030000)>>12)|((rnd&0x0300)>>6)|(rnd&0x03))%28;
			if(annon_rnd==0)
				annon_rnd=monsno;
			else
				annon_rnd+=(CELLID_pm3f_unb-1);
			yofs=PokeYOfsTbl[annon_rnd];
		}
		else if(monsno==MONSNO_POWARUN)
			yofs=PowawaYOfsTbl[BattlePokeAnmNo[client_no]];
		else
			yofs=PokeYOfsTbl[monsno];
#else
		if(monsno==MONSNO_POWARUN)
			yofs=PowawaYOfsTbl[BattlePokeAnmNo[client_no]];
		else if(monsno>MONSNO_END)
			yofs=PokeYOfsTbl[0];
		else
			yofs=PokeYOfsTbl[monsno];
#endif
	}
	return yofs;
}

u8 GetWazaEffPokeYPos(u8 client_no, u16 monsno, u8 offset_flg)
{
	u8 ret;
	s16 offsetY;
	
	if(MineEnemyCheck(client_no) == SIDE_MINE || ContestCheck())
	{
#if 0
		if(monsno == MONSNO_POWARUN)
			offsetY = PowawaMYPos[BattlePokeAnmNo[client_no]];
		else
			offsetY = pm2bpara[monsno].patcnt;
#endif
		offsetY=PokeYPosGet(client_no,monsno);
	}
	else
	{
#if 0
		if(monsno == MONSNO_POWARUN){
			offsetY = PowawaEYPos[BattlePokeAnmNo[client_no]];
		//��ɕ����Ă���n�̃|�P�����̕␳ by soga 2002.08.17
			offsetY -= PowawaYOfsTbl[BattlePokeAnmNo[client_no]];
		}
		else{
			offsetY = pm2fpara[monsno].patcnt;
		//��ɕ����Ă���n�̃|�P�����̕␳ by soga 2002.08.17
			offsetY -= PokeYOfsTbl[monsno];
		}
#else
		offsetY=PokeYPosGet(client_no,monsno);
		offsetY-=PokeYOfsGet(client_no,monsno);
#endif
	}
	ret = WazaEffPos[FightType&FIGHT_TYPE_2vs2][ClientTypeGet(client_no)].y+offsetY;
	
	if(offset_flg){
		if(MineEnemyCheck(client_no) == SIDE_MINE)
			ret += MINE_SIDE_OFFSET_Y;

		#if PM_Y_HOSEI
		if(ret > 112-8)
			ret = 112-8;
		#endif
	}
	
	return ret;
}

//----------------------------------------------------------
//	WazaEffMonsNo��monsno�������Ɠ����Ă���ꍇ�̂ݗL���̍��W�擾�֐�
//	��{�I�ɂ͋Z�̪�Ē��͕��C�B�|�P������AddActor�̎��Ȃǂ̋Z�̪�ĂƂ͊֌W�Ȃ�
//	���ł�WazaEffMonsNo�ɒl�������Ɠ����ĂȂ��̂ŁAGetWazaEffPos�̕����g�p����B
//----------------------------------------------------------
u8 GetWazaEffPos2(u8 client_no, u8 x_y)
{
	u8 ret;
	u16 monsno;
	
	if(x_y == PM_Y || x_y == PM_DEFAULT_Y){
		if(ContestCheck()){
			if(conpara->hensin_flg)
				monsno = conpara->hensin_monsno;
			else
				monsno = conpara->monsno;
		}
		else if(Climem[client_no].hensin_monsno == 0)
			monsno = WazaEffMonsNo[client_no];
		else
			monsno = Climem[client_no].hensin_monsno;
		
		if(x_y == PM_Y)
			ret = GetWazaEffPokeYPos(client_no, monsno, 1);
		else
			ret = GetWazaEffPokeYPos(client_no, monsno, 0);
	}
	else{
		ret = GetWazaEffPos(client_no, x_y);
	}
	return ret;
}

//----------------------------------------------------------
//	�|�P������Y���W���擾
//	GetWazaEffPos�ł͎��@����MINE_SIDE_OFFSET_Y���������������邯�ǁA
//	���̊֐��ł͍��͔������Ȃ��B(�����Ƀ|�P��������Y���W���擾����)
//----------------------------------------------------------
u8 GetPokemonYPos(u8 client_no)
{
	s16 ypos;
	
//	ypos = GetWazaEffPos(client_no, PM_Y);
	ypos = GetWazaEffPos(client_no, PM_DEFAULT_Y);
//	if(MineEnemyCheck(client_no) == SIDE_MINE)
//		ypos -= MINE_SIDE_OFFSET_Y;
	return ypos;
}

//----------------------------------------------------------
//	�g����L������Y���W���擾
//----------------------------------------------------------
u8 GetMigawariYPos(u8 client_no)
{
	s16 ypos;
	
	if(MineEnemyCheck(client_no))
		ypos = GetWazaEffPos(client_no, EFF_Y) + MIGAWARI_F_OFFSET_Y;
	else
		ypos = GetWazaEffPos(client_no, EFF_Y) + MIGAWARI_B_OFFSET_Y;
	return ypos;

}

//----------------------------------------------------------
//	���ł�|�P�����̏�����ւ̃I�t�Z�b�g���l������EFF_Y�̍��W���擾����
//----------------------------------------------------------
u8 GetPokeShadowYPos(u8 client_no)
{
	u8 ypos;
	u16 monsno;
	
	ypos = GetWazaEffPos(client_no, EFF_Y);
	if(ContestCheck())
	{
		return ypos;	//�R���e�X�g�ɉe�͂Ȃ��̂�
	}
	else if(MineEnemyCheck(client_no))
	{
		if(Climem[client_no].hensin_monsno == 0)
			monsno = PokeParaGet(
					&PokeParaEnemy[SelMonsNo[client_no]], ID_monsno);
		else
			monsno = Climem[client_no].hensin_monsno;
	}
	else
	{
		if(Climem[client_no].hensin_monsno == 0)
			monsno = PokeParaGet(
					&PokeParaMine[SelMonsNo[client_no]], ID_monsno);
		else
			monsno = Climem[client_no].hensin_monsno;
	}
	
	if(MineEnemyCheck(client_no))
//		ypos -= PokeYOfsTbl[monsno];
		ypos -= PokeYOfsGet(client_no,monsno);
	return ypos;
}

//----------------------------------------------------------
//	�߹�݂̱������ް�擾
// 0:�U�������߹��   1:�h�䑤���߹��	2:�U�����߹�ݑ���	3:�h�䑤�߹�ݑ���
//	�������߹�݂��w�肵���ہA1vs1��2vs2�ő������C��ł��Ȃ��ꍇ�Ȃǂ�
//	  0xff��װ���ނƂ��ĕԂ��܂�
//----------------------------------------------------------
u8 W_GetPokeActNo(u8 which)
{
	u8 actno;

	if(which == 0){
		//if(PokeBG2taimeCheck(AttackNo) == 0)
		if(ClientPokeExistCheck(AttackNo) == 0)
			actno = 0xff;
		else
			actno = PokemonStruct[AttackNo];
	}
	else if(which == 1){
		//if(PokeBG2taimeCheck(DefenceNo) == 0)
		if(ClientPokeExistCheck(DefenceNo) == 0)
			actno = 0xff;
		else
			actno = PokemonStruct[DefenceNo];
	}
	else if(which == 2){
		if(PokeBG2taimeCheck(AttackNo^2) == 0)
			actno = 0xff;
		else
			actno = PokemonStruct[AttackNo^2];
	}
	else{
		if(PokeBG2taimeCheck(DefenceNo^2) == 0)
			actno = 0xff;
		else
			actno = PokemonStruct[DefenceNo^2];
	}
	return actno;
}

//----------------------------------------------------------
//	������work[6],[7]��SeqAdr���	by matsuda
//----------------------------------------------------------
void SetWorkSeq(actWork *Xreg, void *SeqAdr)
{
	Xreg->work[6] = (s16)((u32)SeqAdr);
	Xreg->work[7] = (s16)((u32)SeqAdr >> 16);
}
//----------------------------------------------------------
//	SetWazaSeqAdr��work[6],[7]�ɾ�Ă������ڽ��move�ɾ��	by matsuda
//----------------------------------------------------------
void SetMoveSeq(actWork *Xreg)
{
	Xreg->move = (void*)((0xffff & Xreg->work[6]) | (Xreg->work[7] << 16));
}

//--------------------------------------------------------------
//         OBJ�~��`�� 
//
// 	work[0]=�p�x
//	work[1]=�U�蕝
//	work[2]=�p�x������
//	work[3]=���[�v��
//--------------------------------------------------------------
void PMObjKaiten( actWork * xreg )
{
	if( xreg->work[3] ) 
	{
		//�~��`��
		xreg->dx = SinMove(xreg->work[0], xreg->work[1]);
		xreg->dy = CosMove(xreg->work[0], xreg->work[1]);

		//�p�x�ω�
		xreg->work[0]+=xreg->work[2];

		if( xreg->work[0] > 255 )
			xreg->work[0]-=256;
		else if( xreg->work[0] < 0 )
			xreg->work[0]+=256;

		xreg->work[3]--;
	}else
	{
		SetMoveSeq(xreg);
	}
}
//--------------------------------------------------------------
//         OBJ�~��`��2
//
// 	work[0]=�p�x
//	work[1]=�U�蕝
//	work[2]=�p�x������
//	work[3]=���[�v��
//	work[4]=�U�蕝������(����8�ޯď���)
//	work[5]=�v�Z�Ŏg�p(0�ŏ��������Ă�������!)
//--------------------------------------------------------------
void PMObjKaiten2( actWork * xreg )
{
	if( xreg->work[3] ) 
	{
		//�~��`��
		xreg->dx = SinMove(xreg->work[0], xreg->work[1] + (xreg->work[5] >> 8));
		xreg->dy = CosMove(xreg->work[0], xreg->work[1] + (xreg->work[5] >> 8));

		//�p�x�ω�
		xreg->work[0]+=xreg->work[2];
		xreg->work[5] += xreg->work[4];

		if( xreg->work[0] > 255 )
			xreg->work[0]-=256;
		else if( xreg->work[0] < 0 )
			xreg->work[0]+=256;

		xreg->work[3]--;
	}else
	{
		SetMoveSeq(xreg);
	}
}
//--------------------------------------------------------------
//         OBJ�~��`��3
//
// 	work[0]=�p�x1			dx
//	work[1]=�U�蕝			����
//	work[2]=�p�x������1
//	work[3]=���[�v��
// 	work[4]=�p�x2			dy
//	work[5]=�p�x������2
//--------------------------------------------------------------
void PMObjKaiten3( actWork * xreg )
{
	if( xreg->work[3] ) 
	{
		//�~��`��
		xreg->dx = SinMove(xreg->work[0], xreg->work[1]);
		xreg->dy = CosMove(xreg->work[4], xreg->work[1]);

		//�p�x�ω�
		xreg->work[0]+=xreg->work[2];
		xreg->work[4]+=xreg->work[5];

		if( xreg->work[0] > 255 )
			xreg->work[0]-=256;
		else if( xreg->work[0] < 0 )
			xreg->work[0]+=256;

		if( xreg->work[4] > 255 )
			xreg->work[4]-=256;
		else if( xreg->work[4] < 0 )
			xreg->work[4]+=256;

		xreg->work[3]--;
	}else
	{
		SetMoveSeq(xreg);
	}
}
//--------------------------------------------------------------
//         OBJ�~��`��4
//
// 	work[0]=�p�x		
//	work[1]=�U�蕝1		dx	
//	work[2]=�p�x������	
//	work[3]=���[�v��
// 	work[4]=�U�蕝2		dy
//--------------------------------------------------------------
void PMObjKaiten4( actWork * xreg )
{
	if( xreg->work[3] ) 
	{
		//�~��`��
		xreg->dx = SinMove(xreg->work[0], xreg->work[1]);
		xreg->dy = CosMove(xreg->work[0], xreg->work[4]);

		//�p�x�ω�
		xreg->work[0]+=xreg->work[2];

		if( xreg->work[0] > 255 )
			xreg->work[0]-=256;
		else if( xreg->work[0] < 0 )
			xreg->work[0]+=256;

		xreg->work[3]--;
	}else
	{
		SetMoveSeq(xreg);
	}
}

//----------------------------------------------------------
//	work[0]��0�ɂȂ�܂ų���	by matsuda
//----------------------------------------------------------
void W_WaitSeq(actWork *Xreg)
{
	if(Xreg->work[0] > 0)
		Xreg->work[0]--;
	else
	{
		SetMoveSeq(Xreg);
	}
}

//----------------------------------------------------------
//		�����o�[�W����
//	�n�_�A�I�_�A�����ړ�		by matsuda
// work0:���ړ����x work1:�n�_X work2:�I�_X work3:�n�_Y work4:�I�_Y
// ���c�̈ړ����x�͉�
//----------------------------------------------------------
void W_StraightMove(actWork *Xreg)
{
	W_GetStraightMoveSp(Xreg);
	Xreg->move = W_ParallelMoveSeq;
	W_ParallelMoveSeq(Xreg);	//���Ă�u�����Ɏ��s����悤�ɏ���̺��
}

//----------------------------------------------------------
//		�����o�[�W����
//	�n�_�A�I�_�̈ړ��ʂ�X�����ւ̑��x����ɏo��
//
// work0:���ړ����x work1:�n�_X work2:�I�_X work3:�n�_Y work4:�I�_Y
// �߂�l:
//	work[0]:��Sync�����ďI�_�ɓ��B�����邩  work[1],[2]:��ٰ�߂��Ƃɑ����l
//----------------------------------------------------------
void W_GetStraightMoveSp(actWork *Xreg)
{
	s16 speed_x;
	
	if(Xreg->work[1] > Xreg->work[2])
		Xreg->work[0] = -Xreg->work[0];
	speed_x = Xreg->work[0];
	
	Xreg->work[0] = abs((Xreg->work[2] - Xreg->work[1]) / speed_x);
	Xreg->work[2] = (Xreg->work[4] - Xreg->work[3]) / Xreg->work[0];
	Xreg->work[1] = speed_x;
}

//----------------------------------------------------------
//		�����o�[�W����
// Sync�񐔕�dx,dy�ɒl�𑫂��Â���	by matsuda
//	work[0]:��Sync�����ďI�_�ɓ��B�����邩  work[1],[2]:��ٰ�߂��Ƃɑ����l([1]=X, [2]=Y)
//----------------------------------------------------------
void W_ParallelMoveSeq(actWork *Xreg)
{
	if(Xreg->work[0] > 0)
	{
		Xreg->work[0]--;
		Xreg->dx += Xreg->work[1];
		Xreg->dy += Xreg->work[2];
	}
	else
	{
		SetMoveSeq(Xreg);
	}
}
//----------------------------------------------------------
//		�����o�[�W����
// Sync�񐔕�dx,dy�ɒl�𑫂��Â���(�Œ菭��Ver)	by matsuda
//	work[0]:��Sync�����ďI�_�ɓ��B�����邩
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//  work[3],[4]:���[�N�Ŏg�p(0�ŏ��������Ă�������!)
//----------------------------------------------------------
void W_ParallelMove2Seq(actWork *Xreg)
{
	if(Xreg->work[0] > 0)
	{
		Xreg->work[0]--;
		Xreg->work[3] += Xreg->work[1];
		Xreg->work[4] += Xreg->work[2];
		Xreg->dx = Xreg->work[3] >> 8;
		Xreg->dy = Xreg->work[4] >> 8;
	}
	else
	{
		SetMoveSeq(Xreg);
	}
}

//----------------------------------------------------------
//		�����o�[�W����
// Sync�񐔕�dx,dy�ɒl�𑫂��Â���(�Œ菭��Ver)	by matsuda
//	work[0]:��Sync�����ďI�_�ɓ��B�����邩
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//  work[3],[4]:���[�N�Ŏg�p(0�ŏ��������Ă�������!)
//  �|�P�����A�C�R���p�̃V�[�P���X
//----------------------------------------------------------
void PokeIconMoveSeq(actWork *Xreg)
{
	if(Xreg->work[0] > 0)
	{
		Xreg->work[0]--;
		Xreg->work[3] += Xreg->work[1];
		Xreg->work[4] += Xreg->work[2];
		Xreg->dx = Xreg->work[3] >> 8;
		Xreg->dy = Xreg->work[4] >> 8;
	}
	else
	{
		SetMoveSeq(Xreg);
	}
	
	ExtAnm(Xreg);
}

//----------------------------------------------------------
//	������Dx,Dy���܂߂����ݒn���瑊��(�I�_)�ֈړ�����
// work[0]:�ړ����x
//----------------------------------------------------------
void W_AttackThrows(actWork *Xreg)
{
//	Xreg->work[0] = THROWS_SP;
	Xreg->work[1] = Xreg->x + Xreg->dx;
	Xreg->work[3] = Xreg->y + Xreg->dy;
	Xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X);
	Xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y);
	Xreg->move = W_StraightMove;
}

//----------------------------------------------------------
// ���߹�ݗp
// Sync�񐔕�dx,dy�ɒl�𑫂��Â���	by matsuda
//	work[0]:��Sync�����ďI�_�ɓ��B�����邩  work[1],[2]:��ٰ�߂��Ƃɑ����l([1]=X, [2]=Y)
//  work[3]:�Ώ��߹�݂̱������ް
//----------------------------------------------------------
void W_PokeParallelMoveSeq(actWork *Xreg)
{
	if(Xreg->work[0] > 0)
	{
		Xreg->work[0]--;
		ActWork[Xreg->work[3]].dx += Xreg->work[1];
		ActWork[Xreg->work[3]].dy += Xreg->work[2];
	}
	else
	{
		SetMoveSeq(Xreg);
	}
}
//----------------------------------------------------------
// ���߹�ݗp
// Sync�񐔕�dx,dy�ɒl�𑫂��Â���(�Œ菭��Ver)	by matsuda
//	work[0]:��Sync�����ďI�_�ɓ��B�����邩
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//  work[3],[4]:���[�N�Ŏg�p(0�ŏ��������Ă�������!)
//  work[5]:�Ώ��߹�݂̱������ް
//----------------------------------------------------------
void W_PokeParallelMoveSeq2(actWork *Xreg)
{
	if(Xreg->work[0] > 0)
	{
		Xreg->work[0]--;
		Xreg->work[3] += Xreg->work[1];
		Xreg->work[4] += Xreg->work[2];
		ActWork[Xreg->work[5]].dx = Xreg->work[3] >> 8;
		ActWork[Xreg->work[5]].dy = Xreg->work[4] >> 8;
	}
	else
	{
		SetMoveSeq(Xreg);
	}
}

//----------------------------------------------------------
//  ���[�v�񐔕�dx,dy�ɑ����A�\����on,off
//  0:LOOP 
//  1:speedX(����8�ޯď����_�ȉ�) 2:�v�Z�p
//  3:speedY(����8�ޯď����_�ȉ�) 4:�v�Z�p
//	5:�\����on,off�̊Ԋu(0�ŕω��Ȃ�)
//----------------------------------------------------------
void W_ParallelMoveSeq2(actWork *Xreg)
{
	if(Xreg->work[0] > 0)
	{
		Xreg->work[0]--;
		Xreg->dx = Xreg->work[2] >> 8;
		Xreg->work[2] += Xreg->work[1];

		Xreg->dy = Xreg->work[4] >> 8;
		Xreg->work[4] += Xreg->work[3];

		if( Xreg->work[0] % Xreg->work[5] == 0 && Xreg->work[5] )
			//Xreg->oamData.AffineMode^=2;
			Xreg->banish^=1;
	}
	else
	{
		SetMoveSeq(Xreg);
	}
}

//----------------------------------------------------------
//  AffineMode���P���R��������AffineWorkNumDel���Ă���
//	DelWazaEffect����
//----------------------------------------------------------
void W_AffCheckEndSeq(actWork *Xreg)
{
	AffineDelActor(Xreg);
	DelWazaEffect(Xreg);
}

//----------------------------------------------------------
//	Dx,Dy���܂߂�����(�I�_)���玩��(�n�_)�ֈړ�����
// work[0]:�ړ����x
//----------------------------------------------------------
void W_AttackThrows2(actWork *Xreg)
{
//	Xreg->work[0] = THROWS_SP;
	Xreg->work[1] = Xreg->x + Xreg->dx;
	Xreg->work[3] = Xreg->y + Xreg->dy;
	Xreg->work[2] = GetWazaEffPos(AttackNo, PM_X);
	Xreg->work[4] = GetWazaEffPos(AttackNo, PM_Y);
	Xreg->move = W_StraightMove;
}

//----------------------------------------------------------
//	DelPalAnm���Ă���W_AffCheckEndSeq��
//	work[5]��id������
//----------------------------------------------------------
void W_DelPalEndSeq(actWork *Xreg)
{
	DelPalAnm((u16)Xreg->work[5]);

	W_AffCheckEndSeq(Xreg);
}

//----------------------------------------------------------
//	affend_sw��1�ɂȂ�܂őҋ@
//----------------------------------------------------------
void W_AffEndWait(actWork *Xreg)
{
	if(Xreg->affend_sw == 1)
		SetMoveSeq(Xreg);
}
//----------------------------------------------------------
//	anmend_sw��1�ɂȂ�܂őҋ@
//----------------------------------------------------------
void W_AnmEndWait(actWork *Xreg)
{
	if(Xreg->anmend_sw == 1)
		SetMoveSeq(Xreg);
}

//----------------------------------------------------------
//	BLDCNT��BLDALPHA��0�N���A����DelWazaEffect���s��
//----------------------------------------------------------
void W_BldClearEnd(actWork *xreg)
{
	*(vu16*)REG_BLDCNT = 0;
	*(vu16*)REG_BLDALPHA = 0;
	DelWazaEffect(xreg);
}

//----------------------------------------------------------
//	BLDCNT��BLDALPHA��0�N���A����DelTaskEffect���s��
//----------------------------------------------------------
void W_BldClearTaskEnd(u8 id)
{
	*(vu16*)REG_BLDCNT = 0;
	*(vu16*)REG_BLDALPHA = 0;
	DelTaskEffect(id);
}

//----------------------------------------------------------
//	��̫�Ă�DefenceNo�̍��W����Ă���Ă���̂�AttackNo�̍��W�ɕς���
//										�����ʂ̊֐��Ƃ��Ďg�p
//----------------------------------------------------------
void EffPositionReset(actWork *Xreg)
{
	Xreg->x = GetWazaEffPos(AttackNo, PM_X);
	Xreg->y = GetWazaEffPos(AttackNo, PM_Y);
}
//----------------------------------------------------------
//	X���W�ւ̵̾�Ă�AttackNo�ɂ���Ĕ��]
//----------------------------------------------------------
void EffXpositionSet(actWork *Xreg, s16 offset_x)
{
#if X_OFFS_REVERSAL_CHECKTYPE	//2002.08.05(��)
	if(MineEnemyCheck(AttackNo))
		Xreg->x -= offset_x;
	else
		Xreg->x += offset_x;
#else	//AttackNo��DefenceNo�����E���Ɉʒu���Ă���Ȃ�ε̾�Ĕ��]
	u8 at_x, de_x;
	
	at_x = GetWazaEffPos(AttackNo, EFF_X);
	de_x = GetWazaEffPos(DefenceNo, EFF_X);
	if(at_x > de_x)
		Xreg->x -= offset_x;
	else if(at_x < de_x)
		Xreg->x += offset_x;
	else{	//���W������
		if(MineEnemyCheck(AttackNo))
			Xreg->x -= offset_x;
		else
			Xreg->x += offset_x;
	}
#endif
}



//----------------------------------------------------------
// ���R�[�����Ďg�p
//
// �S���[�N�g�p : �w�肪�Ȃ����菉�����̕K�v�Ȃ�
//	work 0:��Sync�����ďI�_�ɓ��B�����邩   2:�I�_X�@4:�I�_Y
//  5:�R�Ȃ�̐U�蕝
//----------------------------------------------------------
void YamanariMoveInit(actWork *Xreg)
{
	Xreg->work[1] = Xreg->x;
	Xreg->work[3] = Xreg->y;
//	W_GetStraightMoveLoop(Xreg);
	W_GetStraightSyncParam(Xreg);
	
	Xreg->work[6] = (128 << 8) / Xreg->work[0];
	Xreg->work[7] = 0;
}

//----------------------------------------------------------
//	��SetWorkSeq�����g�p���Ȃ��ŁA����R�[�����Ďg�p
//
// work: YamanariMoveInit���g�p���Ă���Ή����w�肷��K�v�͂Ȃ��ł�
// �߂�l: 0:�܂�����������   1:�I��
//----------------------------------------------------------
u8 YamanariMoveMain(actWork *Xreg)
{
	if(W_ParallelSync(Xreg))
		return 1;

	Xreg->work[7] += Xreg->work[6];
	Xreg->dy += SinMove(0xff & (Xreg->work[7] >> 8), Xreg->work[5]);
	return 0;
}

//----------------------------------------------------------
//	Dx,Dy��X,Y�ɑ�����Dx,Dy��0�N���A
//----------------------------------------------------------
void DxDyAddClear(actWork *xreg)
{
	xreg->x += xreg->dx;
	xreg->y += xreg->dy;
	xreg->dx = 0;
	xreg->dy = 0;
}

//----------------------------------------------------------
//	��̫�č��W��� ����1�i���܂�ɂ��������߂��ĂԌ��������̂ō��������)
// offset 0:Xoffset  1:Yoffset
//	����:���WӰ��(EFF_MODE or PM_MODE)
//----------------------------------------------------------
//-- ���̂܂ܖh�䑤�̍��W --//
void WazaDefaultPosSet1(actWork *Xreg, u8 zahyou_mode)
{
	if(zahyou_mode == EFF_MODE)
	{
		Xreg->x = GetWazaEffPos2(DefenceNo, EFF_X);
		Xreg->y = GetWazaEffPos2(DefenceNo, EFF_Y);
	}
	EffXpositionSet(Xreg, WazaEffWork[0]);
	Xreg->y += WazaEffWork[1];
}
//-- �U�����̍��W�ɐ؂�ւ��Ă���̾�Ă𑫂� --//
void WazaDefaultPosSet2(actWork *Xreg, u8 zahyou_mode)
{
	if(zahyou_mode == EFF_MODE)
	{
		Xreg->x = GetWazaEffPos2(AttackNo, EFF_X);
		Xreg->y = GetWazaEffPos2(AttackNo, EFF_Y);
	}
	else
	{
		Xreg->x = GetWazaEffPos2(AttackNo, PM_X);
		Xreg->y = GetWazaEffPos2(AttackNo, PM_Y);
	}
	EffXpositionSet(Xreg, WazaEffWork[0]);
	Xreg->y += WazaEffWork[1];
}

//----------------------------------------------------------
//		�|�P�����̈ʒu���G���������������f����
//----------------------------------------------------------

u8	MineEnemyCheck(u8 client_no)		//Add by soga 2001.09.25
{
	return	ClientType[client_no]&1;
}

u8	ClientTypeGet(u8 client_no)		//Add by soga 2001.09.26
{
	return	ClientType[client_no];
}

u8	ClientNoGet(u8 client_type)		//Add by soga 2001.09.26
{
	u8	client_no;
	
	for(client_no=0;client_no<ClientSetMax;client_no++){
		if(ClientType[client_no]==client_type) break;
	}
	return client_no;
}

//----------------------------------------------------------
//	�w�肵���ײ��Ă��߹�݂������Ă��邩�A���Ȃ���(���݂��Ă邩�A���ĂȂ���)
//  ����������
//	�߂�l:0:�����ĂȂ�	1:�����Ă�
//----------------------------------------------------------
u8 ClientPokeExistCheck(u8 client_no)
{
	if(ContestCheck()){
		if(AttackNo == client_no || DefenceNo == client_no)
			return 1;
		return 0;
	}
	
#if 1
	if(ClientType[client_no] == 0xff)
		return 0;
	
	if(MineEnemyCheck(client_no))
	{
		if(PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_hp) == 0)
			return 0;
		else
			return 1;
	}
	else
	{
		if(PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_hp) == 0)
			return 0;
		else
			return 1;
	}
#else
	return ((NoReshuffleClient&no2bittbl[client_no])==0);
#endif
}

//----------------------------------------------------------
//	���݂̐퓬��1��1��2��2����������
// �߂�l�F0 = 1��1   1 = 2��2
//----------------------------------------------------------
u8 FightCheck(void)
{
	return (FightType & FIGHT_TYPE_2vs2);
}

//----------------------------------------------------------
//	�퓬�A��ýāABG���g�p����ꍇ�̷�ױ��ڽ�A��ذݱ��ڽ���擾����
//----------------------------------------------------------
void GetWazaEffBgAdrs(WazaBgData *EffData)
{
	if(ContestCheck()){
		EffData->CharAdrs = (void*)(BG_VRAM + CONWAZA_BG_CHARBASE * 0x4000);
		EffData->ScrnAdrs = (void*)(BG_VRAM + 0x800 * CONWAZA_BG_SCRNBASE);
		EffData->PlttNo = CONWAZA_BG_PAL;
	}else{
		EffData->CharAdrs = (void*)(BG_VRAM + BTL_BG_CHARADRS1);
		EffData->ScrnAdrs = (void*)(BG_VRAM + 0x800 * BTL_BG_SCRNBASE1);
		EffData->PlttNo = BTL_BG_PAL1;
	}
}

void GetWazaEffBgAdrs2(WazaBgData *EffData, u8 client_no)
{
	if(ContestCheck()){
		EffData->CharAdrs = (void*)(BG_VRAM + CONWAZA_BG_CHARBASE * 0x4000);
		EffData->ScrnAdrs = (void*)(BG_VRAM + 0x800 * CONWAZA_BG_SCRNBASE);
		EffData->PlttNo = CONWAZA_BG_PAL;
	}else{
		if(GetPokeBGNo(AttackNo) == 1){
			EffData->CharAdrs = (void*)(BG_VRAM + BTL_BG_CHARADRS1);
			EffData->ScrnAdrs = (void*)(BG_VRAM + 0x800 * BTL_BG_SCRNBASE1);
			EffData->PlttNo = BTL_BG_PAL1;
		}
		else{
			EffData->CharAdrs = (void*)(BG_VRAM + BTL_BG_CHARADRS2);
			EffData->ScrnAdrs = (void*)(BG_VRAM + 0x800 * BTL_BG_SCRNBASE2);
			EffData->PlttNo = BTL_BG_PAL2;
		}
	}
}

//----------------------------------------------------------
//	�퓬�A�R���e�X�g�̔w�i�̃p���b�gNo���擾����
//----------------------------------------------------------
u8 GetHaikeiPalNo(void)
{
	if(ContestCheck())
		return 1;
	else
		return 2;
}

//----------------------------------------------------------
//	�w�i��BG�ɑ΂��āA��荞���׸ށA���ނ�0�ɕύX���ľ�āAؾ�Ă���
//	flg: 0=���	1:ؾ��(�����ݒ�ɖ߂�)
//----------------------------------------------------------
void HaikeiBGLoopSizeSet(u8 flg)
{
	if(flg == 0){
		(*(vBgCnt *)REG_BG3CNT).Size = 0;
		(*(vBgCnt *)REG_BG3CNT).Loop = 1;
	}
	else{
		if(ContestCheck()){
			(*(vBgCnt *)REG_BG3CNT).Size = CONTEST_HAIKEI_SIZE;
			(*(vBgCnt *)REG_BG3CNT).Loop = CONTEST_HAIKEI_LOOP;
		}
		else{
			(*(vBgCnt *)REG_BG3CNT).Size = BTL_BG_HAIKEI_SIZE;
			(*(vBgCnt *)REG_BG3CNT).Loop = BTL_BG_HAIKEI_LOOP;
		}
	}
}

//----------------------------------------------------------
//		�����o�[�W����	�܂��g�p���Ă���ٰ�݂�����̂Ŏc���Ă�
//	�n�_�A�I�_�A�����ړ�		by matsuda
// work0:��Sync�����ďI�_�ɓ��B�����邩 work2:�I�_X work4:�I�_Y
//----------------------------------------------------------
void W_StraightMove2(actWork *Xreg)
{
	Xreg->work[1] = Xreg->x;
	Xreg->work[3] = Xreg->y;
	W_GetStraightMoveLoop(Xreg);
	Xreg->move = PokeIconMoveSeq;
	Xreg->move(Xreg);
}

//----------------------------------------------------------
//		�����o�[�W����	�܂��g�p���Ă���ٰ�݂�����̂Ŏc���Ă�
//	�n�_�A�I�_�̈ړ��ʂ�Sync�񐔂���ɏo��
//
// work0:��Sync�����ďI�_�ɓ��B�����邩 work1:�n�_X work2:�I�_X work3:�n�_Y work4:�I�_Y
// �߂�l:
//	work[0]:Sync��  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//	work[3],[4]��0�N���A
//----------------------------------------------------------
void W_GetStraightMoveLoop(actWork *Xreg)
{
	s16 OffSetX, OffSetY;
	
	OffSetX = ((Xreg->work[2] - Xreg->work[1])) << 8;
	OffSetY = ((Xreg->work[4] - Xreg->work[3])) << 8;
	
	Xreg->work[1] = OffSetX / Xreg->work[0];
	Xreg->work[2] = OffSetY / Xreg->work[0];
	Xreg->work[3] = Xreg->work[4] = 0;
}

//----------------------------------------------------------
//		���V�o�[�W����
//	�n�_�A�I�_�̈ړ��ʂ�Sync�񐔂���ɏo��
//
// work0:��Sync�����ďI�_�ɓ��B�����邩 work1:�n�_X work2:�I�_X work3:�n�_Y work4:�I�_Y
// �߂�l:
//	work[0]:Sync��  
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//				���̏ꍇ�͍ŉ����ޯĂ�0�A���̏ꍇ�͍ŉ����ޯĂ�1�ɂȂ�
//				(�����̕�����1�㉺�������炢�ł͂قƂ�Ǖς��Ȃ��̂��׸ނƂ��Ă݂�)
//	work[3],[4]��0�N���A
//----------------------------------------------------------
void W_GetStraightSyncParam(actWork *Xreg)
{
	s32 OffSetX, OffSetY;
	u16 add_x, add_y;
	u8 mainasuX, mainasuY;
	
	OffSetX = Xreg->work[2] - Xreg->work[1];
	OffSetY = Xreg->work[4] - Xreg->work[3];
	OffSetX < 0 ? (mainasuX = 1) : (mainasuX = 0);
	OffSetY < 0 ? (mainasuY = 1) : (mainasuY = 0);
	
	add_x = abs(OffSetX) << 8;
	add_y = abs(OffSetY) << 8;
	add_x /= Xreg->work[0];
	add_y /= Xreg->work[0];
	if(mainasuX)
		add_x |= 1;
	else
		add_x &= 0xfffe;
	if(mainasuY)
		add_y |= 1;
	else
		add_y &= 0xfffe;
	
	Xreg->work[1] = add_x;
	Xreg->work[2] = add_y;
	Xreg->work[3] = Xreg->work[4] = 0;
}

//----------------------------------------------------------
//		���V�o�[�W����
//	�n�_�A�I�_�A�����ړ�		by matsuda
// work0:��Sync�����ďI�_�ɓ��B�����邩 work2:�I�_X work4:�I�_Y
//----------------------------------------------------------
void W_StraightSyncMove(actWork *Xreg)
{
	Xreg->work[1] = Xreg->x;
	Xreg->work[3] = Xreg->y;
	W_GetStraightSyncParam(Xreg);
	Xreg->move = W_ParallelSyncMove;
	Xreg->move(Xreg);
}

//----------------------------------------------------------
//		���R�[�����Ďg�p
//	�߂�l: 0 = �܂����쒆		1 = �I��
//		���V�o�[�W����
// Sync�񐔕�dx,dy�ɒl�𑫂��Â���(�Œ菭��)
//	work[0]:�J��Ԃ�Sync��
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//				�ŉ����ޯĂ�0�̏ꍇ�͐��Ƃ��āA1�̏ꍇ�͕��Ƃ��Ă݂�
//				(�ڂ�����W_GetStraightSyncParam�̺��ĎQ��)
//  work[3],[4]:���[�N�Ŏg�p(0�ŏ��������Ă�������!)
//----------------------------------------------------------
u8 W_ParallelSync(actWork *Xreg)
{
	u16 addX, addY;
	u16 dx, dy;
	
	if(Xreg->work[0] == 0)
		return 1;
	
	addX = Xreg->work[1];
	addY = Xreg->work[2];
	dx = Xreg->work[3];
	dy = Xreg->work[4];
	
	dx += addX;
	dy += addY;
	if(addX & 1)
		Xreg->dx = -(dx >> 8);
	else
		Xreg->dx = dx >> 8;
	if(addY & 1)
		Xreg->dy = -(dy >> 8);
	else
		Xreg->dy = dy >> 8;
	
	Xreg->work[3] = dx;
	Xreg->work[4] = dy;
	Xreg->work[0]--;
	return 0;
}

//----------------------------------------------------------
//
//----------------------------------------------------------
void W_ParallelSyncMove(actWork *Xreg)
{
	if(W_ParallelSync(Xreg))
		SetMoveSeq(Xreg);
}

//----------------------------------------------------------
//		���V�o�[�W����	2002.05.10(��)
//	�n�_�A�I�_�̈ړ��ʂ�X�����ւ̑��x����ɏo��
//
// work0:���ړ����x(�Œ菭��:���̂�) work1:�n�_X work2:�I�_X work3:�n�_Y work4:�I�_Y
//
// �߂�l:
//	work[0]:Sync��
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//				���̏ꍇ�͍ŉ����ޯĂ�0�A���̏ꍇ�͍ŉ����ޯĂ�1�ɂȂ�
//				(�����̕�����1�㉺�������炢�ł͂قƂ�Ǖς��Ȃ��̂��׸ނƂ��Ă݂�)
//	work[3],[4]��0�N���A
//----------------------------------------------------------
void W_GetStraightSpParam(actWork *xreg)
{
	s32 OffSetX;
	
	OffSetX = abs(xreg->work[2] - xreg->work[1]) << 8;
	xreg->work[0] = OffSetX / xreg->work[0];
	W_GetStraightSyncParam(xreg);
}

//----------------------------------------------------------
//		���V�o�[�W����
//	�n�_�A�I�_�AX�����ւ̑��x����ɒ����ړ�		by matsuda
// work0:���ړ����x(�Œ菭��:���̂�) work2:�I�_X work4:�I�_Y
//----------------------------------------------------------
void W_StraightSpMove(actWork *Xreg)
{
	Xreg->work[1] = Xreg->x;
	Xreg->work[3] = Xreg->y;
	W_GetStraightSpParam(Xreg);
	Xreg->move = W_ParallelSyncMove;
	Xreg->move(Xreg);
}








//=========================================================================
//
//		����4�r�b�g������Version
//												by matsuda 2002.06.04(��)
//=========================================================================
//----------------------------------------------------------
//		������4�r�b�g����Version
//	�n�_�A�I�_�̈ړ��ʂ�Sync�񐔂���ɏo��
//
// work0:��Sync�����ďI�_�ɓ��B�����邩 work1:�n�_X work2:�I�_X work3:�n�_Y work4:�I�_Y
// �߂�l:
//	work[0]:Sync��  
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//				���̏ꍇ�͍ŉ����ޯĂ�0�A���̏ꍇ�͍ŉ����ޯĂ�1�ɂȂ�
//				(�����̕�����1�㉺�������炢�ł͂قƂ�Ǖς��Ȃ��̂��׸ނƂ��Ă݂�)
//	work[3],[4]��0�N���A
//----------------------------------------------------------
void W_GetStraightSyncParam4(actWork *Xreg)
{
	s32 OffSetX, OffSetY;
	u16 add_x, add_y;
	u8 mainasuX, mainasuY;
	
	OffSetX = Xreg->work[2] - Xreg->work[1];
	OffSetY = Xreg->work[4] - Xreg->work[3];
	OffSetX < 0 ? (mainasuX = 1) : (mainasuX = 0);
	OffSetY < 0 ? (mainasuY = 1) : (mainasuY = 0);
	
	add_x = abs(OffSetX) << 4;
	add_y = abs(OffSetY) << 4;
	add_x /= Xreg->work[0];
	add_y /= Xreg->work[0];
	if(mainasuX)
		add_x |= 1;
	else
		add_x &= 0xfffe;
	if(mainasuY)
		add_y |= 1;
	else
		add_y &= 0xfffe;
	
	Xreg->work[1] = add_x;
	Xreg->work[2] = add_y;
	Xreg->work[3] = Xreg->work[4] = 0;
}

//----------------------------------------------------------
//		������4�r�b�g����Version
//	�n�_�A�I�_�A�����ړ�		by matsuda
// work0:��Sync�����ďI�_�ɓ��B�����邩 work2:�I�_X work4:�I�_Y
//----------------------------------------------------------
void W_StraightSyncMove4(actWork *Xreg)
{
	Xreg->work[1] = Xreg->x;
	Xreg->work[3] = Xreg->y;
	W_GetStraightSyncParam4(Xreg);
	Xreg->move = W_ParallelSyncMove4;
	Xreg->move(Xreg);
}

//----------------------------------------------------------
//		������4�r�b�g����Version
//		���R�[�����Ďg�p
//	�߂�l: 0 = �܂����쒆		1 = �I��
//		���V�o�[�W����
// Sync�񐔕�dx,dy�ɒl�𑫂��Â���(�Œ菭��)
//	work[0]:�J��Ԃ�Sync��
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//				�ŉ����ޯĂ�0�̏ꍇ�͐��Ƃ��āA1�̏ꍇ�͕��Ƃ��Ă݂�
//				(�ڂ�����W_GetStraightSyncParam�̺��ĎQ��)
//  work[3],[4]:���[�N�Ŏg�p(0�ŏ��������Ă�������!)
//----------------------------------------------------------
u8 W_ParallelSync4(actWork *Xreg)
{
	u16 addX, addY;
	u16 dx, dy;
	
	if(Xreg->work[0] == 0)
		return 1;
	
	addX = Xreg->work[1];
	addY = Xreg->work[2];
	dx = Xreg->work[3];
	dy = Xreg->work[4];
	
	dx += addX;
	dy += addY;
	if(addX & 1)
		Xreg->dx = -(dx >> 4);
	else
		Xreg->dx = dx >> 4;
	if(addY & 1)
		Xreg->dy = -(dy >> 4);
	else
		Xreg->dy = dy >> 4;
	
	Xreg->work[3] = dx;
	Xreg->work[4] = dy;
	Xreg->work[0]--;
	return 0;
}

//----------------------------------------------------------
//		������4�r�b�g����Version
//----------------------------------------------------------
void W_ParallelSyncMove4(actWork *Xreg)
{
	if(W_ParallelSync4(Xreg))
		SetMoveSeq(Xreg);
}

//----------------------------------------------------------
//		������4�r�b�g����Version
//	�n�_�A�I�_�̈ړ��ʂ�X�����ւ̑��x����ɏo��
//
// work0:���ړ����x(����4�r�b�g����:���̂�) 
//	work1:�n�_X work2:�I�_X work3:�n�_Y work4:�I�_Y
//
// �߂�l:
//	work[0]:Sync��
//  work[1],[2]:��ٰ�߂��Ƃɑ����l������8�ޯď���([1]=X, [2]=Y)
//				���̏ꍇ�͍ŉ����ޯĂ�0�A���̏ꍇ�͍ŉ����ޯĂ�1�ɂȂ�
//				(�����̕�����1�㉺�������炢�ł͂قƂ�Ǖς��Ȃ��̂��׸ނƂ��Ă݂�)
//	work[3],[4]��0�N���A
//----------------------------------------------------------
void W_GetStraightSpParam4(actWork *xreg)
{
	s32 OffSetX;
	
	OffSetX = abs(xreg->work[2] - xreg->work[1]) << 4;
	xreg->work[0] = OffSetX / xreg->work[0];
	W_GetStraightSyncParam4(xreg);
}

//----------------------------------------------------------
//		������4�r�b�g����Version
//	�n�_�A�I�_�AX�����ւ̑��x����ɒ����ړ�		by matsuda
// work0:���ړ����x(�Œ菭��:���̂�) work2:�I�_X work4:�I�_Y
//----------------------------------------------------------
void W_StraightSpMove4(actWork *Xreg)
{
	Xreg->work[1] = Xreg->x;
	Xreg->work[3] = Xreg->y;
	W_GetStraightSpParam4(Xreg);
	Xreg->move = W_ParallelSyncMove4;
	Xreg->move(Xreg);
}












//=========================================================================
//
//		�|�P�����g�k�̒l���
//	����ýĂł͌����𔽓]�����Ȃ���΂����Ȃ��̂ŁA������OBJ_AffineWork�ɾ�Ă���̂�
//	�����ɺ�ýĂ̏ꍇ�͔��]������B
//	�g�k�v�Z��OBJ_AffineWork�ɑ΂��Ă̾�Ă��s��
//	
//	���݂̊g�k�l�Ȃǂ̌v�Z�͊O�ł��K�v������
//												by matsuda 2002.02.18(��)
//=========================================================================
//----------------------------------------------------------
//	����: pokeact = �|�P������ActNo
// 		  RatioX,RatioY = �g�kX,Y	Theta = ��]�p�x
//----------------------------------------------------------
void PokeKakusyukuSet(u8 pokeact, s16 RatioX, s16 RatioY, u16 Theta)
{
	ObjAffineSrcData src;
	ObjAffineDestData dest;
	u8 affNo;
	
	src.RatioX = RatioX;	//0x0100;
	src.RatioY = RatioY;	//0x0100;
	src.Theta = Theta;		//0;
	
//	if(ContestCheck())
	if(RatioXFlipCheck())
		src.RatioX = -src.RatioX;
	
	affNo = ActWork[pokeact].oamData.AffineParamNo;
	ObjAffineSet(&src, &dest, 1, 2);
	OBJ_AffineWork[affNo].H_DiffX = dest.H_DiffX;
	OBJ_AffineWork[affNo].V_DiffX = dest.V_DiffX;
	OBJ_AffineWork[affNo].H_DiffY = dest.H_DiffY;
	OBJ_AffineWork[affNo].V_DiffY = dest.V_DiffY;
}

u8 RatioXFlipCheck(void)
{
	if(ContestCheck() && ActWork[W_GetPokeActNo(0)].work[2] != MONSNO_ANNOON)
		return 1;
	return 0;
}

//----------------------------------------------------------
//	�߹�݂̊g�k�����O��Init�֐�
//	����: pokeact = �߹�݂�ActNo
//		  bld_flg = ������ON,OFF�׸�(1:ON  0:OFF)
//----------------------------------------------------------
void PokeKakusyukuInit(u8 pokeact, u8 bld_flg)
{
	u8 client_no;
	
	client_no = ActWork[pokeact].work[0];
	if(ContestCheck() == 0 && PokeBG2taimeCheck(client_no) == 0)	//2002.09.21(�y)
		;	//ۯ��݁�������Ƃԁ����߂��� �ŉ�ʂɂ��Ȃ��߹�݂���Ư���׸ނ�OFF���Ȃ��悤������
	else
		ActWork[pokeact].banish = 0;	//BG���Ƃ�����Ư����ԂɂȂ��Ă��߹�݂��o��
//	ActWork[pokeact].banish = 0;	//BG���Ƃ�����Ư����ԂɂȂ��Ă��߹�݂��o��
	ActWork[pokeact].oamData.ObjMode = bld_flg;	//�������׸�ON
	ActWork[pokeact].aff_pause = 1;		//���ĂĂ����Ȃ���Affine��҂��D�悳��Ă��܂�
	if(ContestCheck() == 0 && 
		ActWork[pokeact].oamData.AffineMode == 0)	//ETC�̪�ĂȂǂł͑ޔ������ĂȂ��̂�
	{
		ActWork[pokeact].oamData.AffineParamNo = CliSys[client_no].poke_aff_no;
	}
	ActWork[pokeact].oamData.AffineMode = 3;	//�߹�݂̔{�p�׸�ON
	//-- �{�p�׸ނ𗧂Ă��̂Œ��S���W��␳ --//
	actCenterCalc(&ActWork[pokeact], ActWork[pokeact].oamData.Shape,
			ActWork[pokeact].oamData.Size, ActWork[pokeact].oamData.AffineMode );
}

//----------------------------------------------------------
//	�߹�݂̊g�k���I������Ƃ��̌Ă�End�֐�
//	����:	pokeact = �߹�݂�ActNo
//----------------------------------------------------------
void PokeKakusyukuEnd(u8 pokeact)
{
	//-- �ꉞ��̫�Ă̒l��� --//
	PokeKakusyukuSet(pokeact, 0x0100, 0x0100,  0);
	ActWork[pokeact].oamData.AffineMode = 1;	//�{�p�׸�OFF
	ActWork[pokeact].oamData.ObjMode = 0;	//�������׸�OFF
	ActWork[pokeact].aff_pause = 0;
	//-- �{�p�׸ނ����ɖ߂����̂Œ��S���W��␳ --//
	actCenterCalc(&ActWork[pokeact], ActWork[pokeact].oamData.Shape,
		ActWork[pokeact].oamData.Size, ActWork[pokeact].oamData.AffineMode );
}
//-------------------------------------------------------------
//	��]�����|�P�����A�N�^�[�̐؂�ڂ������Ȃ��悤�ɁAdy �l��␳����
//	pokeact  �|�P�����A�N�^�[�i���o
//-------------------------------------------------------------
void W_AdjustKaitenYPos(u8 pokeact)
{
	u8 affno;
	s16 H_DiffY;
	u32 dy;

	affno = ActWork[pokeact].oamData.AffineParamNo;
	H_DiffY = OBJ_AffineWork[affno].H_DiffY;
	if(H_DiffY < 0){
		H_DiffY *= -1;
	}
	dy = (H_DiffY * 32) >> 8;
	ActWork[pokeact].dy = dy;
}


//=========================================================================
//	����: xreg = �̨��ް���Ă��鱸��
//		  hosei = �{�pӰ�ނɂ��␳�̍Čv�Z(0:���Ȃ�  1:����)
// 		  RatioX,RatioY = �g�kX,Y	Theta = ��]�p�x
//=========================================================================
void KakusyukuSet(actWork *xreg, u8 hosei, s16 RatioX, s16 RatioY, u16 Theta)
{
	ObjAffineSrcData src;
	ObjAffineDestData dest;
	u8 affNo;

	if((xreg->oamData.AffineMode & 1) == 0)
		return;
	
	xreg->aff_pause = 1;	//���ĂĂ����Ȃ���Affine��҂��D�悳��Ă��܂�
	if(hosei){
		//-- �{�p�׸� ���S���W��␳ --//
		actCenterCalc(xreg, xreg->oamData.Shape,
			xreg->oamData.Size, xreg->oamData.AffineMode );
	}

	src.RatioX = RatioX;	//0x0100;
	src.RatioY = RatioY;	//0x0100;
	src.Theta = Theta;		//0;
	
//	if(ContestCheck())
	if(RatioXFlipCheck())
		src.RatioX = -src.RatioX;
	
	affNo = xreg->oamData.AffineParamNo;
	ObjAffineSet(&src, &dest, 1, 2);
	OBJ_AffineWork[affNo].H_DiffX = dest.H_DiffX;
	OBJ_AffineWork[affNo].V_DiffX = dest.V_DiffX;
	OBJ_AffineWork[affNo].H_DiffY = dest.H_DiffY;
	OBJ_AffineWork[affNo].V_DiffY = dest.V_DiffY;
}

//----------------------------------------------------------
//	�߹�݂̊g�k���I������Ƃ��̌Ă�End�֐�
//	����:	pokeact = �߹�݂�ActNo
//----------------------------------------------------------
void KakusyukuEnd(actWork *xreg)
{
	//-- �ꉞ��̫�Ă̒l��� --//
	KakusyukuSet(xreg, 1, 0x0100, 0x0100,  0);
	xreg->aff_pause = 0;
	//-- �{�p�׸ނ����ɖ߂����̂Œ��S���W��␳ --//
	actCenterCalc(xreg, xreg->oamData.Shape,
		xreg->oamData.Size, xreg->oamData.AffineMode );
}


//=========================================================================
//	�p�x���擾����
//
//	�߂�l�F�p�x 0�`0xffff(���̂܂�AffineParameter�ɓ���l)
//=========================================================================
u16 GetKakudo(s16 x, s16 y)
{
	u16 kakudo;
	
	kakudo = ArcTan2(x,y);
	return kakudo;
}

u16 GetKakudo2(s16 x, s16 y)
{
	u16 kakudo;
	
	kakudo = GetKakudo(x,y);
	return (0x10000 - kakudo);
}

//=========================================================================
//	�J���[�����m�g�[���F�ɕω�
//	����:palno = ���m�g�[���ɂ���J���[�p���b�g(0�`32  ��0�`15:BG�A 16�`31:OBJ)
//		flg = 0:���m�g�[���ɂ���	1:���m�g�[���ɂȂ��Ă����̂����ɖ߂�
//=========================================================================
void W_ColorMonotone(u16 palno, u8 flg)
{
	int i;
	u16 color;
	
	palno *= 16;
	
	if(flg == 0){
		for(i = 0; i < 16; i++){
			color = ((PlttData*)&PaletteWork[palno+i])->Red 
				+ ((PlttData*)&PaletteWork[palno+i])->Green 
				+ ((PlttData*)&PaletteWork[palno+i])->Blue;
			color /= 3;
			((PlttData*)&PaletteWorkTrans[palno+i])->Red = color;
			((PlttData*)&PaletteWorkTrans[palno+i])->Green = color;
			((PlttData*)&PaletteWorkTrans[palno+i])->Blue = color;
		}
	}else
		CpuCopy(&PaletteWork[palno], &PaletteWorkTrans[palno], 0x20, 32);
}


//=========================================================================
//	PaletteFade�Ŏg�p����fade_bit�̓��A�w�i�A�߹�݁A�̪�ėpBG���ޯĂ��擾����
//		�e������ 0=�擾���Ȃ�	1=�擾����
// ������	haikei=�w�i(�y�n�܂�)	atpoke=�U�����߹�� depoke=�h�䑤�߹��
//			atpoke2=�U�����߹�ݑ���  depoke2=�h�䑤�߹�ݑ���
//			eff1=�̪��BG1  eff2=�̪��BG2
//=========================================================================
u32 GetBattleFadeBit(u8 haikei, u8 atpoke, u8 depoke, u8 atpoke2, u8 depoke2, u8 eff1, u8 eff2)
{
	u32 fade_bit = 0;
	u32 pal_no;
	
	if(haikei){
		if(ContestCheck() == 0)
			fade_bit = 0xe;
		else
			fade_bit |= 1 << GetHaikeiPalNo();
	}
	if(atpoke)	//�U�����߹�݂�̪���
	{
		pal_no = AttackNo;
		fade_bit |= 1 << (pal_no + 16);
	}
	if(depoke)	//�h�䑤�߹�݂�̪���
	{
		pal_no = DefenceNo;
		fade_bit |= 1 << (pal_no + 16);
	}
	if(atpoke2 && PokeBG2taimeCheck(AttackNo^2))	//�U�����߹�ݑ���
	{
		pal_no = AttackNo ^ 2;
		fade_bit |= 1 << (pal_no + 16);
	}
	if(depoke2 && PokeBG2taimeCheck(DefenceNo^2))	//�h�䑤�߹�ݑ���
	{
		pal_no = DefenceNo ^ 2;
		fade_bit |= 1 << (pal_no + 16);
	}
	
	if(eff1){
		if(ContestCheck() == 0)
			fade_bit |= 1 << BTL_BG_PAL1;
		else
			fade_bit |= 1 << CONWAZA_BG_PAL;
	}
	if(eff2 && ContestCheck() == 0)
		fade_bit |= 1 << BTL_BG_PAL2;	//0x200;
	
	return fade_bit;
}

//=========================================================================
//	PaletteFade�Ŏg�p����fade_bit�̓��A�߹�݂̓G�A�����A�S�Ă��ޯĂ��擾����
//		�e������ 0=�擾���Ȃ�	1=�擾����
// ������	mine0=���@��1�̖�	mine1=���@��2�̖�
//			enemy0=�G��1�̖�	enemy1=�G��2�̖�
//=========================================================================
u32 GetPokeFadeBit(u8 mine0, u8 mine1, u8 enemy0, u8 enemy1)
{
	u32 fade_bit = 0;
	u32 pal_no;
	
	if(ContestCheck()){
		if(mine0){
			pal_no = CONT_MINE_CLIENT;
			fade_bit |= 1 << (pal_no + 16);
		}
		return fade_bit;
	}
		
	if(mine0 && PokeBG2taimeCheck(ClientNoGet(BSS_CLIENT_MINE))){
		pal_no = ClientNoGet(BSS_CLIENT_MINE);
		fade_bit |= 1 << (pal_no + 16);
	}
	if(mine1 && PokeBG2taimeCheck(ClientNoGet(BSS_CLIENT_MINE2))){
		pal_no = ClientNoGet(BSS_CLIENT_MINE2);
		fade_bit |= 1 << (pal_no + 16);
	}
	if(enemy0 && PokeBG2taimeCheck(ClientNoGet(BSS_CLIENT_ENEMY))){
		pal_no = ClientNoGet(BSS_CLIENT_ENEMY);
		fade_bit |= 1 << (pal_no + 16);
	}
	if(enemy1 && PokeBG2taimeCheck(ClientNoGet(BSS_CLIENT_ENEMY2))){
		pal_no = ClientNoGet(BSS_CLIENT_ENEMY2);
		fade_bit |= 1 << (pal_no + 16);
	}
	return fade_bit;
}

//=========================================================================
//	ClientNo����|�P�����̃p���b�gNo���擾
//=========================================================================
u8 GetClientPokePalNo(u8 client_no)
{
	return client_no;
}

//=========================================================================
//	ClientType(BSS_CLIENT_MINE,BSS_CLIENT_MINE2��)����|�P�����̃p���b�gNo���擾
//=========================================================================
u8 GetTypePokePalNo(u8 client_type)
{
	return (ClientNoGet(client_type));
}










//=========================================================================
//
//		����Init�V�[�P���X
//	�������̍ŏ���Init�֐��Ƃ��Ďg�p(WazaEffWork�̒l���g�p���Ă��邽��)
//=========================================================================
//----------------------------------------------------------
//		��Init�֐�
//	���̏�ű�Ұ��݁A���͊g�k���I������̂�҂��āA���̌�Del����
// offset 0:Xoffset  1:Yoffset  2: 0=�U�����ɏo��  1=�h�䑤�ɏo��
//		  3:���WӰ��(0:PMӰ��	1:EFFӰ��)
//----------------------------------------------------------
void W_AnmAffWaitDel(actWork *xreg)
{
	u8 mode;
	
	if(xreg->work[0] == 0)	//flag
	{
		if(WazaEffWork[3] == 0)
			mode = PM_MODE;
		else
			mode = EFF_MODE;
		
		if(WazaEffWork[2] == 0)
			WazaDefaultPosSet2(xreg, mode);
		else
			WazaDefaultPosSet1(xreg, mode);
		
		xreg->work[0]++;	//����flag on
	}
	else
	{
		if(xreg->anmend_sw == 1 || xreg->affend_sw == 1)
			W_AffCheckEndSeq(xreg);
	}
}

//----------------------------------------------------------
//		��Init�֐�
//	��������G�Ɍ������ċ����΂���Del����
//	offset	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//	4:Sync��		5:���WӰ��:���8�ޯ�=�n�_  ����8�ޯ�=�I�_(0:PMӰ��	1:EFFӰ��)
//----------------------------------------------------------
void W_StraightMoveInit(actWork *xreg)
{
	u8 st_mode, end_mode;
	
	if((WazaEffWork[5] & 0xff00) == 0)
		st_mode = PM_MODE;
	else
		st_mode = EFF_MODE;
	if((WazaEffWork[5] & 0x00ff) == 0)
		end_mode = PM_Y;
	else
		end_mode = EFF_Y;
	
	WazaDefaultPosSet2(xreg, st_mode);
	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];
	xreg->work[0] = WazaEffWork[4];
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X) + WazaEffWork[2];
	xreg->work[4] = GetWazaEffPos(DefenceNo, end_mode) + WazaEffWork[3];
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}

//----------------------------------------------------------
//		��Init�֐�
//	��������G�ɂ�܂Ȃ�Ŕ��ł���
//	offset	0:Xoffset	1:Yoffset	2:�I�_Xoffset	3:�I�_Yoffset	4:Sync��
//			5:Y�U�蕝
//----------------------------------------------------------
void W_YamanariInitSeq(actWork *xreg)
{
	WazaDefaultPosSet2(xreg, PM_MODE);
	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];
	xreg->work[0] = WazaEffWork[4];
	xreg->work[2] = GetWazaEffPos(DefenceNo, PM_X) + WazaEffWork[2];
	xreg->work[4] = GetWazaEffPos(DefenceNo, PM_Y) + WazaEffWork[3];
	xreg->work[5] = WazaEffWork[5];
	YamanariMoveInit(xreg);
	xreg->move = W_YamanariSubSeq;
}

static void W_YamanariSubSeq(actWork *xreg)
{
	if(YamanariMoveMain(xreg))
		DelWazaEffect(xreg);
}

//----------------------------------------------------------
//		��Init�֐�
//	�w�肵���|�P�����̵̾�ĊԂŒ����ړ�����
//	0:�n�_Xoffset	1:�n�_Yoffset	2:�I�_Xoffset	3:�I�_Yoffset
//	4:Sync��	5:�U����(0)���h�䑤(1)��	6:���WӰ��(0:PMӰ��	1:EFFӰ��)
//----------------------------------------------------------
void W_OffsetSpaceMove(actWork *xreg)
{
	u8 client;
	u8 mode, mode_y;
	
	if(WazaEffWork[6] == 0)
	{
		mode = PM_MODE;
		mode_y = PM_Y;
	}
	else
	{
		mode = EFF_MODE;
		mode_y = EFF_Y;
	}
	
	if(WazaEffWork[5] == 0)
	{
		WazaDefaultPosSet2(xreg, mode);
		client = AttackNo;
	}
	else
	{
		WazaDefaultPosSet1(xreg, mode);
		client = DefenceNo;
	}
	
	if(MineEnemyCheck(AttackNo))
		WazaEffWork[2] = -WazaEffWork[2];
	
	WazaDefaultPosSet1(xreg, mode);
	xreg->work[0] = WazaEffWork[4];
	xreg->work[2] = GetWazaEffPos(client, PM_X) + WazaEffWork[2];
	xreg->work[4] = GetWazaEffPos(client, mode_y) + WazaEffWork[3];
	xreg->move = W_StraightSyncMove;
	SetWorkSeq(xreg, DelWazaEffect);
}











//======================================================================
//	�|�P�����̎c���A�N�^�[�쐬
//	which  0:�U����  1:�h�䑤
//	return:  �������ް �i�쐬�ł��Ȃ���� -1�j
//																taya
//======================================================================
s16 W_PokeZanzouMake(u8 which)
{
	u8 actno;
	u16 i;

	actno = W_GetPokeActNo(which);
	if(actno != 0xff)
	{
		for(i = 0; i < ACT_MAX; i++){
			if(ActWork[i].act_sw == 0){
				ActWork[i] = ActWork[actno];
				ActWork[i].oamData.ObjMode = 1;
				ActWork[i].banish = 0;
				return i;
			}
		}
	}

	return -1;
}
//======================================================================
//	�|�P�����̎c���A�N�^�[�폜
//	actor	�������ڽ
//======================================================================
void W_PokeZanzouDel(actWork *actor)
{
	actor->t_mode = 1;  // �{�̂̃L�����̈���J�������Ȃ�
	DelActor(actor);
}




//=========================================================================
//
//	BLDALPHA ���W�X�^����i�K�� BLDALPHA_RESET���g�p���ďI�����邱�Ɓj
//	0:eva �����l  1:evb �����l  2: eva �ŏI�l 3: evb �ŏI�l  4:wait
//
//	wait�҂���Aeva, evb �����݂ɍX�V
//
//=========================================================================
static void TaskBldAlphaMain(u8 id);

void TaskBldAlphaCtrl(u8 id)
{
	s16 eva_diff, evb_diff;

	eva_diff = evb_diff = 0;

	if(WazaEffWork[2] > WazaEffWork[0]){
		eva_diff = 1;
	}
	if(WazaEffWork[2] < WazaEffWork[0]){
		eva_diff = -1;
	}
	if(WazaEffWork[3] > WazaEffWork[1]){
		evb_diff = 1;
	}
	if(WazaEffWork[3] < WazaEffWork[1]){
		evb_diff = -1;
	}

	TaskTable[id].work[0] = 0;              // 0:����
	TaskTable[id].work[1] = WazaEffWork[4]; // 1:wait
	TaskTable[id].work[2] = 0;              // 2:�ǂ�����X�V���邩�H
	TaskTable[id].work[3] = WazaEffWork[0]; // 3:eva
	TaskTable[id].work[4] = WazaEffWork[1]; // 4:evb
	TaskTable[id].work[5] = eva_diff;       // 5:eva������
	TaskTable[id].work[6] = evb_diff;       // 6:evb������
	TaskTable[id].work[7] = WazaEffWork[2]; // 7:�ŏIeva�l
	TaskTable[id].work[8] = WazaEffWork[3]; // 8:�ŏIevb�l

//	*(vu16*)REG_BLDCNT = BLD_A_BLEND_MODE | BLD_2ND_ALL;
	*(vu16*)REG_BLDALPHA = (WazaEffWork[1] << 8)| WazaEffWork[0];

	TaskTable[id].TaskAdrs = TaskBldAlphaMain;
}
static void TaskBldAlphaMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	if(++(my->work[0]) > my->work[1]){
		my->work[0] = 0;
		my->work[2]++;
		if(my->work[2] & 1){
			if(my->work[3] != my->work[7]){
				my->work[3] += my->work[5];
			}
		}else{
			if(my->work[4] != my->work[8]){
				my->work[4] += my->work[6];
			}
		}
		*(vu16*)REG_BLDALPHA = (my->work[4] << 8)| my->work[3];

		if(my->work[3] == my->work[7] && my->work[4] == my->work[8]){
			DelTaskEffect(id);
		}
	}
}


//=========================================================================
//	�|�P�����p���b�g�t�F�[�h�^�X�N
//
//	0:(0:�U����  1:�h�䑤  2:�U������  3:�h�䑊���j 
//	1:̪��޶װ  2:�ő�evy�l  3:wait  4:��
//
//=========================================================================
static void TaskPokeFadeMain(u8 id);
static void FadeCtrlCommon(TASK_TABLE *task);

void TaskPokeFadeCtrl(u8 id)
{
	u16 actno;

	actno = W_GetPokeActNo(WazaEffWork[0]);
	if(actno == 0xff){
		DelTaskEffect(id);
		return;
	}

	TaskTable[id].work[0] = 256 + (ActWork[actno].oamData.Pltt * 16) + 1;  // 0:��گ����ް

	FadeCtrlCommon(&TaskTable[id]);
}
static void FadeCtrlCommon(TASK_TABLE *task)
{
	task->work[1] = WazaEffWork[1];  // 1:̪��޶װ
	task->work[2] = 0;               // 2:evy�l
	task->work[3] = WazaEffWork[2];  // 3:�ő�evy�l
	task->work[4] = 0;               // 4:����
	task->work[5] = WazaEffWork[3];  // 5:wait
	task->work[6] = 0;               // 6:Ӱ��
	task->work[7] = WazaEffWork[4];  // 7:��

	task->TaskAdrs = TaskPokeFadeMain;
}
static void TaskPokeFadeMain(u8 id)
{
	TASK_TABLE *my = &TaskTable[id];

	my->work[4]++;
	if(my->work[4] >= my->work[5]){
		my->work[4] = 0;
		// ����Ӱ��
		if(my->work[6] == 0){
			my->work[2]++;
			SoftFade(my->work[0], 15, my->work[2], my->work[1]);
			if(my->work[2] == my->work[3]){
				my->work[6] = 1;
			}
		// ����Ӱ��
		}else{
			my->work[2]--;
			SoftFade(my->work[0], 15, my->work[2], my->work[1]);
			if(my->work[2] == 0){
				my->work[7]--;
				if(my->work[7]){
					my->work[4] = 0;
					my->work[6] = 0;
				}else{
					DelTaskEffect(id);
				}
			}
		}
	}
}
//=========================================================================
//	�L�����w��p���b�g�t�F�[�h�^�X�N
//
//	0:��گ�ID
//	1:̪��޶װ  2:�ő�evy�l  3:wait  4:��
//
//=========================================================================
void TaskCharFadeCtrl(u8 id)
{
	u16 pal_no;

	pal_no = ObjPalNumGet(WazaEffWork[0]);
	if(pal_no == 0xff){
		DelTaskEffect(id);
		return;
	}

	TaskTable[id].work[0] = 256 + (pal_no * 16) + 1;  // 0:��گ����ް

	FadeCtrlCommon(&TaskTable[id]);
}



/**********************************************************************/
/*  �|�P�����A�N�^�[�g�k�A�j��                                        */
/*                                                                    */
/*  �^�X�N����R�[�����邱�ƂŁA�|�P�����̊g�k�A�j�����s���܂��B      */
/*  �^�X�N��work���A7�`15�܂Ŏg���܂��B                               */
/*                                                                    */
/*  work[15]�Ƀ|�P�����̱������ް������̂ŎQ�Ƃł��܂��B             */
/*                                                                    */
/*  SetPokeKakusyukuAnm �ŕK�v�ȃp�����[�^���Z�b�g��A                */
/*  PokeKakusyukuAnm ���O��Ԃ��܂ŌĂ�ł��������B                   */
/*                                                                    */
/*  SetPokeKakusyukuAnm�Ŏw�肵���g�k�A�j���e�[�u���� ACTAF_END ��    */
/*  �Ȃ��ƁA���܂Ōo���Ă��O��Ԃ��Ȃ��̂ŁA�Ăяo�����ŐӔC������  */
/*  �� PokeKakusyukuEnd ���Ă�ł��������B                            */
/*                                                                    */
/**********************************************************************/
enum {
	PKA_ANM_NO = 7,
	PKA_ANM_TIMER,
	PKA_cLOOP_CNT,
	PKA_XRATIO,
	PKA_YRATIO,
	PKA_THETA,
	PKA_ANMADRS_H,
	PKA_ANMADRS_L,
	PKA_POKE_ACTNO,
};

//==============================================
//	�g�k�A�j��������
//	task	�^�X�N�e�[�u���A�h���X
//	actno	�|�P�����A�N�^�[�i���o
//	anm		�g�k�A�j���e�[�u���̃A�h���X
//==============================================
void SetPokeKakusyukuAnm(TASK_TABLE *task, u8 actno, const actAffAnm *anm)
{
	task->work[PKA_ANM_NO] = 0;
	task->work[PKA_ANM_TIMER] = 0;
	task->work[PKA_cLOOP_CNT] = 0;
	task->work[PKA_POKE_ACTNO] = actno;

	task->work[PKA_XRATIO] =  0x100;
	task->work[PKA_YRATIO] = 0x100;
	task->work[PKA_THETA] = 0;

	Divide32(&(task->work[PKA_ANMADRS_H]), &(task->work[PKA_ANMADRS_L]), (u32)anm);

	PokeKakusyukuInit(actno, 0);
}

u8 PokeKakusyukuAnm(TASK_TABLE *task)
{
	static const actAffAnm *anm = NULL;

	anm = (const actAffAnm*)Unite32(task->work[PKA_ANMADRS_H], task->work[PKA_ANMADRS_L]);
	anm += task->work[PKA_ANM_NO];

	switch(anm->rx){
	default:
		if(anm->wait == ACT_AFSET)
		{
			task->work[PKA_XRATIO] = anm->rx;
			task->work[PKA_YRATIO] = anm->ry;
			task->work[PKA_THETA] = anm->theta;
			task->work[PKA_ANM_NO]++;
			anm++;
		}

		task->work[PKA_XRATIO] += anm->rx;
		task->work[PKA_YRATIO] += anm->ry;
		task->work[PKA_THETA] += anm->theta;
		PokeKakusyukuSet(task->work[PKA_POKE_ACTNO], task->work[PKA_XRATIO], task->work[PKA_YRATIO], task->work[PKA_THETA]);
		PokeKakusyukuYHosei(task->work[PKA_POKE_ACTNO]);
		if(++(task->work[PKA_ANM_TIMER]) >= anm->wait)
		{
			task->work[PKA_ANM_TIMER] = 0;
			task->work[PKA_ANM_NO]++;
		}
		break;

	case ACT_AFLOOP:
		task->work[PKA_ANM_NO] = anm->ry;
		break;

	case ACT_AFcLOOP:
		if(anm->ry){

			if(task->work[PKA_cLOOP_CNT] == 0){
				task->work[PKA_cLOOP_CNT] = anm->ry;
			}else{
				task->work[PKA_cLOOP_CNT]--;
				if(task->work[PKA_cLOOP_CNT] == 0){
					task->work[PKA_ANM_NO]++;
					break;
				}
			}
			while(task->work[PKA_ANM_NO]){
				task->work[PKA_ANM_NO]--;
				anm--;
				if(anm->rx == ACT_AFcLOOP){
					task->work[PKA_ANM_NO]++;
					break;
				}
			}

		}else{

			task->work[PKA_ANM_NO]++;

		}
		break;

	case ACT_AFEND:
		// �I�������͌Ăяo�����ōs��
		ActWork[task->work[PKA_POKE_ACTNO]].dy = 0;
		PokeKakusyukuEnd(task->work[PKA_POKE_ACTNO]);
		return 0;
	}

	return 1;
}
//==============================================
//	�Ă����|�P�����̐؂�ڂ������Ȃ��悤��
//	dy �l��␳
//==============================================
static u16 get_patcnt(u8 actno);
void PokeKakusyukuYHosei(u8 pokeact)
{
	s32 dy;
	s32  height;
	u8 affno;

	height = 64 - (get_patcnt(pokeact) * 2);
	affno = ActWork[pokeact].oamData.AffineParamNo;

	dy = (height << 8) / OBJ_AffineWork[affno].V_DiffY;
	if(dy > 128){ dy = 128; }

	ActWork[pokeact].dy = (height-dy)/2;
}
void ZanzouKakusyukuYHosei(u8 zanzou_actno, u8 poke_actno)
{
	s32 dy;
	s32  height;
	u8 affno;

	height = 64 - (get_patcnt(poke_actno) * 2);
	affno = ActWork[zanzou_actno].oamData.AffineParamNo;

	dy = (height << 8) / OBJ_AffineWork[affno].V_DiffY;
	if(dy > 128){ dy = 128; }

	ActWork[zanzou_actno].dy = (height-dy)/2;

}

// �߹�ݱ�����patcnt�l�i�摜�؂�ڂ̂x���W�j
static u16 get_patcnt(u8 actno)
{
	u16 i, monsno;
	u8 client_no;
	
	client_no = ActWork[actno].work[0];
	for(i = 0; i < CLIENT_MAX; i++)
	{
		if(PokemonStruct[i] == actno)
		{
			if(ContestCheck()){
				monsno = conpara->monsno;
				return pm2bpara[monsno].patcnt;
			}else if(MineEnemyCheck(i) == SIDE_MINE){
				if(Climem[client_no].hensin_monsno == 0)
					monsno = PokeParaGet(&PokeParaMine[SelMonsNo[i]], ID_monsno);
					//monsno = WazaEffMonsNo[i];
				else
					monsno = Climem[client_no].hensin_monsno;
				return pm2bpara[monsno].patcnt;
			}else{
				if(Climem[client_no].hensin_monsno == 0)
					monsno = PokeParaGet(&PokeParaEnemy[SelMonsNo[i]], ID_monsno);
					//monsno = WazaEffMonsNo[i];
				else
					monsno = Climem[client_no].hensin_monsno;
				return pm2fpara[monsno].patcnt;
			}
		}
	}
	return 64; // �����܂ŗ��邱�Ƃ͂��蓾�Ȃ����ǁA�ꉞ�B
}

//==============================================================
//	32bit�l�𕪊����āA16bit�l x 2 �Ƃ��ĕۑ�
//	dst1, dst2   �ۑ���ϐ��A�h���X
//	val          �ۑ�����l
//==============================================================
void Divide32(s16 *dst1, s16 *dst2, s32 val)
{
	*dst1 = val & 0xffff;
	*dst2 = (val >> 16) & 0xffff;
}
//==============================================================
//	Divide32�ŕ����ۑ�����32bit�l�����o��
//	src1, src2   �����ۑ������ϐ�
//==============================================================
s32 Unite32(s16 src1, s16 src2)
{
	return (s32)((src1 & 0xffff) | (src2 << 16));
}

/**********************************************************************/
/*  �|�P�����A�N�^�[�g�k�A�j���Q                                      */
/*                                                                    */
/*  �^�X�N����R�[�����邱�ƂŁA�|�P�����̊g�k�A�j�����s���܂��B      */
/*  �����g�k���E�ŏI�g�k���E�K�v�R�[�������w�肷�邱�ƂŁA            */
/*  �R�[�����Ɋg�k�����������܂��B                                    */
/*                                                                    */
/*  �^�X�N��work���A8�`15�܂Ŏg���܂��B                               */
/*  �߹�݂̱������ނ� work[15] �ɓ���̂ŁA�Q�Ƃł��܂��B             */
/*                                                                    */
/*  SetPokeKakusyukuStep �ŕK�v�ȃp�����[�^���Z�b�g��A               */
/*  PokeKakusyukuStep ��0��Ԃ��܂ŌĂ�ł��������B                   */
/*                                                                    */
/*  �g�k����ύX�����܂ܘA���g�p���邱�Ƃ�O��ɍ���Ă���̂ŁA      */
/*  PokeKakusyukuInit, PokeKakusyukuEnd ������ŌĂт܂���B          */
/*  �����̊֐��͌Ăяo�����ŐӔC�������ČĂ�ł��������B            */
/*                                                                    */
/**********************************************************************/
enum {
	PK2_CNT = 8,
	PK2_XRATIO,
	PK2_YRATIO,
	PK2_ADD_X,
	PK2_ADD_Y,
	PK2_END_X,
	PK2_END_Y,
	PK2_ACTNO,
};

void SetPokeKakusyukuStep(TASK_TABLE *task, u8 actno, s16 sx, s16 sy, s16 ex, s16 ey, u16 sync)
{
	task->work[PK2_CNT] = sync;
	task->work[PK2_ACTNO] = actno;
	task->work[PK2_XRATIO] = sx;
	task->work[PK2_YRATIO] = sy;
	task->work[PK2_END_X] = ex;
	task->work[PK2_END_Y] = ey;
	task->work[PK2_ADD_X] = (ex - sx) / sync;
	task->work[PK2_ADD_Y] = (ey - sy) / sync;
}

u8 PokeKakusyukuStep(TASK_TABLE *task)
{
	if(task->work[PK2_CNT] == 0){
		return 0;
	}

	task->work[PK2_CNT]--;
	if(task->work[PK2_CNT]){
		task->work[PK2_XRATIO] += task->work[PK2_ADD_X];
		task->work[PK2_YRATIO] += task->work[PK2_ADD_Y];
	}else{
		task->work[PK2_XRATIO] = task->work[PK2_END_X];
		task->work[PK2_YRATIO] = task->work[PK2_END_Y];
	}

	PokeKakusyukuSet(task->work[PK2_ACTNO], task->work[PK2_XRATIO], task->work[PK2_YRATIO], 0);

	if(task->work[PK2_CNT]){
		PokeKakusyukuYHosei(task->work[PK2_ACTNO]);
	}else{
		ActWork[task->work[PK2_ACTNO]].dy = 0;
	}

	return task->work[PK2_CNT];
}




//=========================================================================
//	�Ȃ��x��WazaEffWork[7]�̒l�ɃZ�b�g����
// �Ȃ��x��
//	0�`30�Ȃ� 0���Z�b�g
//	31�`100�Ȃ� 1���Z�b�g
//	101�`200�Ȃ� 2���Z�b�g
//	201�ȏ�Ȃ� 3���Z�b�g
//=========================================================================
void NatukidoWorkSet(u8 id)
{
	u8 ret;
	
	if(WazaEffNatukido < 31)
		ret = 0;
	else if(WazaEffNatukido < 101)
		ret = 1;
	else if(WazaEffNatukido < 201)
		ret = 2;
	else
		ret = 3;
	WazaEffWork[7] = ret;
	DelTaskEffect(id);
}


//=========================================================================
//	�S�Ẵ|�P�����̃v���C�I���e�B��ݒ肷��
//=========================================================================
void PokePriAllSet(u8 pri)
{
	if(PokeBG2taimeCheck(DefenceNo))
		ActWork[PokemonStruct[DefenceNo]].oamData.Priority = pri;
	if(PokeBG2taimeCheck(AttackNo))
		ActWork[PokemonStruct[AttackNo]].oamData.Priority = pri;
	if(PokeBG2taimeCheck(DefenceNo^2))
		ActWork[PokemonStruct[DefenceNo^2]].oamData.Priority = pri;
	if(PokeBG2taimeCheck(AttackNo^2))
		ActWork[PokemonStruct[AttackNo^2]].oamData.Priority = pri;
}


//=========================================================================
//	�S�Ẵ|�P�����̿�Ă�ʰ�ނ̃v���C�I���e�B������������
//=========================================================================
void PokePriAllInit(void)
{
	int i;
	
	for(i = 0; i < ClientSetMax; i++)
	{
		if(PokeBG2taimeCheck(i) == 0)
			continue;
		ActWork[PokemonStruct[i]].pri = GetPokeSoftPri(i);
		ActWork[PokemonStruct[i]].oamData.Priority = POKE_OAM_PRI;
	}
}

//----------------------------------------------------------
//	�w�肵���ײ��Ă��߹�݂���̫�Ă̱���(���)��ײ��è�����o��
//----------------------------------------------------------
u8 GetPokeSoftPri(u8 client_no)
{
	u8 client_type;
	u8 pri;
	
	if(ContestCheck()){
		if(client_no == CONT_MINE_CLIENT)
			return C_POKE1PRI;
		else
			return CAI_POKE1PRI;
	}
	
	client_type = ClientTypeGet(client_no);
	if(client_type == BSS_CLIENT_MINE)
		pri = C_POKE1PRI;
	else if(client_type == BSS_CLIENT_MINE2)
		pri = C_POKE2PRI;
	else if(client_type == BSS_CLIENT_ENEMY)
		pri = CAI_POKE1PRI;
	else
		pri = CAI_POKE2PRI;
	return pri;
}

//----------------------------------------------------------
//	�w�肵���ײ��Ă�BG�����̎���BG��ײ��è�����o��
//----------------------------------------------------------
u8 GetPokeBGPri(u8 client_no)
{
	u8 client_type;
	u8 pri;
	
	client_type = ClientTypeGet(client_no);
	if(ContestCheck())
		pri = 2;
	else{
		if(client_type == BSS_CLIENT_MINE || client_type == BSS_CLIENT_ENEMY2)
			pri = (*(vBgCnt *)REG_BG2CNT).Priority;
		else
			pri = (*(vBgCnt *)REG_BG1CNT).Priority;
	}
	return pri;
}

//----------------------------------------------------------
//	�w�肵���ײ��Ă�BG�����̎���BGNo�����o��
// �߂�l: BG1 = 1, BG2 = 2���߂�l�Ƃ��Ė߂�
//----------------------------------------------------------
u8 GetPokeBGNo(u8 client_no)
{
	u8 client_type;
	
	if(ContestCheck())
		return 1;
	
	client_type = ClientTypeGet(client_no);
	if(client_type == BSS_CLIENT_MINE || client_type == BSS_CLIENT_ENEMY2)
		return 2;
	else
		return 1;
}

//=========================================================================
//	�w��No���߹�݂̱����𐶐�����
//	muki = 0:���ʌ���	1:������
//	cell_type = �g�p����ActHeader��ð���(0�`1)�����ԍ���cell_type�͈ꏏ�ɂ�Add�o���Ȃ�
//=========================================================================
#define WAZA_EFF_POKE_TRANSADRS		0x2000000	//0x2004000
u8 WazaEffPokeActSet(u16 monsno, u8 muki, u8 cell_type, s16 x, s16 y, u8 pri, u32 personal_rnd, u32 id_no)
{
	u8 poke;
	u16 cell_no, pal_no;
	
	cell_no = CellSet(&WazaPokeCell[cell_type]);
	pal_no = ObjPalNumSet(WazaPokeHeader[cell_type].palNo);
	if(muki == 0){
		DecordPaletteWork((void*)PokePalGetPara(monsno,id_no,personal_rnd),
				16*16+16*pal_no, 0x20);
		DecordPokeCellMakeRndFB(&PM2FObjDataCell[monsno], pm2fpara[monsno].size,
				pm2fpara[monsno].patcnt, (void*)0x2000000,
				(void*)WAZA_EFF_POKE_TRANSADRS, monsno, personal_rnd, POKE_FRONT);
	}
	else{
		DecordPaletteWork((void*)PokePalGetPara(monsno,id_no,personal_rnd),
				16*16+16*pal_no, 0x20);
		DecordPokeCellMakeRndFB(&PM2BObjDataCell[monsno], pm2bpara[monsno].size,
				pm2bpara[monsno].patcnt, (void*)0x2000000,
				(void*)WAZA_EFF_POKE_TRANSADRS, monsno, personal_rnd, POKE_BACK);
	}
	DIV_DMACOPY(3, WAZA_EFF_POKE_TRANSADRS, OBJ_MODE0_VRAM+cell_no*32, 0x800, 32);

	if(muki == 0)
		poke = AddActor(&WazaPokeHeader[cell_type], x, y+pm2fpara[monsno].patcnt, pri);
	else
		poke = AddActor(&WazaPokeHeader[cell_type], x, y+pm2bpara[monsno].patcnt, pri);
	
	if(ContestCheck()){
		ActWork[poke].aff_tbl = PmAffAnmTblF2;
		ActAffAnmChg(&ActWork[poke], 0);
	}
	
	return poke;
}

//----------------------------------------------------------
//	WazaEffPokeActSet��Add�����߹�݂�Del����
//----------------------------------------------------------
void WazaEffPokeActDel(actWork *xreg)
{
	AllDelActor(xreg);
}

static const	actHeader WazaPokeHeader[]={
	{
		CELLID_WAZAPOKE0,
		CELLID_WAZAPOKE0,
		&WazaOamDataAffine[3],
		DummyActAnmTbl,
		0,
		DummyActAffTbl,
		DummyActMove,
	},
	{
		CELLID_WAZAPOKE1,
		CELLID_WAZAPOKE1,
		&WazaOamDataAffine[3],
		DummyActAnmTbl,
		0,
		DummyActAffTbl,
		DummyActMove,
	},
};
static const CellData WazaPokeCell[] = {	//�̈�m�ۂ��邽�߂�����Cell��`
	{space64_Character, OBJ_SIZE16_64x64, CELLID_WAZAPOKE0},
	{space64_Character, OBJ_SIZE16_64x64, CELLID_WAZAPOKE1},
};





/*==============================================================
==  �|�P�����摜�̕��E�����Ȃǂ𓾂�
==  client_no   �ײ������ް
==	mode
==			POKESIZE_HEIGHT    ����
==			POKESIZE_WIDTH     ��
==			POKESIZE_TOP       ���_
==			POKESIZE_BOTTOM    ���
==			POKESIZE_LEFT      ���[
==			POKESIZE_RIGHT     �E�[
==			POKESIZE_REAL_BOTTOM  �덷�Ȃ����
==
==	�� ���ޯĒ��x�̌덷������܂��B��̖̂ڈ��Ƃ��Ďg���Ă��������B
==	   
==
==============================================================*/
s16 W_GetPokeSize(u8 client_no, u8 mode)
{
	u16 monsno;
	const pmPara *para;
	u16	annon_rnd;
	u32	rnd;

	if(ContestCheck()){
		if(conpara->hensin_flg){
			monsno = conpara->hensin_monsno;
			rnd = conpara->hensin_personal_rnd;
		}
		else{
			monsno = conpara->monsno;
			rnd	   = conpara->personal_rnd;
		}
		if(monsno==MONSNO_ANNOON){
			annon_rnd=(((rnd&0x03000000)>>18)|((rnd&0x030000)>>12)|((rnd&0x0300)>>6)|(rnd&0x03))%28;
			if(annon_rnd==0)
				annon_rnd=monsno;
			else
				annon_rnd+=(CELLID_pm3b_unb-1);
			para = pm2bpara+annon_rnd;
		}
		else if(monsno==MONSNO_POWARUN)
			para = PowawaParaTbl+BattlePokeAnmNo[client_no];
		else if(monsno>MONSNO_END)
			para = pm2bpara;
		else
			para = pm2bpara+monsno;
	}
	else if(MineEnemyCheck(client_no) == SIDE_MINE){
		if(Climem[client_no].hensin_monsno == 0){
			monsno = PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_monsno);
			rnd	   = PokeParaGet(&PokeParaMine[SelMonsNo[client_no]], ID_personal_rnd);
		}
		else{
			monsno = Climem[client_no].hensin_monsno;
			rnd    = HenshinRnd[client_no];
		}
		if(monsno==MONSNO_ANNOON){
			annon_rnd=(((rnd&0x03000000)>>18)|((rnd&0x030000)>>12)|((rnd&0x0300)>>6)|(rnd&0x03))%28;
			if(annon_rnd==0)
				annon_rnd=monsno;
			else
				annon_rnd+=(CELLID_pm3b_unb-1);
			para = pm2bpara+annon_rnd;
		}
		else if(monsno>MONSNO_END)
			para = pm2bpara;
		else
			para = pm2bpara+monsno;
	}else{
		if(Climem[client_no].hensin_monsno == 0){
			monsno = PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_monsno);
			rnd	   = PokeParaGet(&PokeParaEnemy[SelMonsNo[client_no]], ID_personal_rnd);
		}
		else{
			monsno = Climem[client_no].hensin_monsno;
			rnd	   = HenshinRnd[client_no];
		}
		if(monsno==MONSNO_ANNOON){
			annon_rnd=(((rnd&0x03000000)>>18)|((rnd&0x030000)>>12)|((rnd&0x0300)>>6)|(rnd&0x03))%28;
			if(annon_rnd==0)
				annon_rnd=monsno;
			else
				annon_rnd+=(CELLID_pm3f_unb-1);
			para = pm2fpara+annon_rnd;
		}
		else if(monsno==MONSNO_POWARUN)
			para = PowawaParaTbl+BattlePokeAnmNo[client_no];
		else if(monsno>MONSNO_END)
			para = pm2fpara;
		else
			para = pm2fpara+monsno;
	}

	switch(mode){
	case POKESIZE_HEIGHT:
		return (para->size & 0x0f) * 8;

	case POKESIZE_WIDTH:
		return ((para->size >> 4) & 0x0f) * 8;

	case POKESIZE_LEFT:
		return GetWazaEffPos(client_no,PM_X) - (((para->size >> 4) & 0x0f) * 4);

	case POKESIZE_RIGHT:
		return GetWazaEffPos(client_no,PM_X) + (((para->size >> 4) & 0x0f) * 4);

	case POKESIZE_TOP:
		return GetWazaEffPos(client_no,PM_Y) - ((para->size & 0x0f) * 4);

	case POKESIZE_BOTTOM:
		return GetWazaEffPos(client_no,PM_Y) + ((para->size & 0x0f) * 4);

	case POKESIZE_REAL_BOTTOM:
		return GetWazaEffPos(client_no,EFF_Y) + 31 - para->patcnt;
	}

	return 0;

}








//=========================================================================
//
//	1��1�̎��͑Ώۂ̃|�P�����̒��S���W�A2��2�̎��͑Ώۂ�2�΂̃|�P�����̊Ԃ�
//	���S���W���擾����
//												by matsuda 2002.07.19(��)
//=========================================================================
//----------------------------------------------------------
//	mode:EFF_MODE or PM_MODE	xpos,ypos:���ʑ����ܰ�
//----------------------------------------------------------
void GetCenterEffPos(u8 client_no, u8 mode, s16 *xpos, s16 *ypos)
{
	u8 mode_x, mode_y;
	s16 x1,x2,y1,y2;
	
	if(mode == EFF_MODE){
		mode_x = EFF_X;
		mode_y = EFF_Y;
	}
	else{
		mode_x = PM_X;
		mode_y = PM_Y;
	}
	
	x1 = GetWazaEffPos(client_no, mode_x);
	y1 = GetWazaEffPos(client_no, mode_y);
	if(FightCheck() && ContestCheck() == 0){
		x2 = GetWazaEffPos(client_no^2, mode_x);
		y2 = GetWazaEffPos(client_no^2, mode_y);
	}
	else{
		x2 = x1;
		y2 = y1;
	}

	*xpos = (x1 + x2) / 2;
	*ypos = (y1 + y2) / 2;
}





//=========================================================================
//
//		�|�P�����̃E�B���h�EOBJ�𐶐�
//												by matsuda 2002.07.25(��)
//=========================================================================
u8 PokeWinObjMake(u8 client_no, u8 pokeact, u16 monsno)
{
	u8 winact;

	winact = AddBanishActor(DummyActMove);
	ActWork[winact] = ActWork[pokeact];
	ActWork[winact].t_mode = 1;	//����޳OBJ��Del�������ɖ{�̷̂�ׂ��J�����Ȃ��悤��
	ActWork[winact].oamData.Priority = 0;
	ActWork[winact].oamData.ObjMode = 2;	//OBJ Win
	ActWork[winact].oamData.CharNo = ActWork[pokeact].oamData.CharNo;
	ActWork[winact].move = DummyActMove;
//	ActAnmChg(&ActWork[winact], BattlePokeAnmNo[client_no]);

	return winact;
}

