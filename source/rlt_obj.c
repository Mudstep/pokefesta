//========================================================================================
//								�~�j�Q�[���E���[���b�g��OBJ
//									2002/03/28 by nakahiro
//========================================================================================
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "task.h"
#include "MaDefine.h"
#include "calctool.h"
#include "mus_tool.h"
#include "roulette.h"
#include "rlt_obj.h"
#include "monsno.def"

#include "../scaddata/rlt_obj.acl"
#include "../scaddata/rlt_ball.ach"
#include "../scaddata/rlt_ev00.ach"
#include "../scaddata/rlt_shadow.ach"
//========================================================================================
//										�V���{��
//========================================================================================

//========================================================================================
//									�v���g�^�C�v�錾
//========================================================================================
static u8 AddRotateObj( const actHeader *, u8, u16 * );	// ��]OBJ�Z�b�g

static void RotatePokeActMove( actWork * act );	// ���[���b�g�Ղ̃|�P������]����
static void RotateCtblActMove( actWork * act );	// �Z���^�[�e�[�u���̉�]

//�A�N�^�[�V�[�P���X
static void BallMoveAct_END(actWork *act);		//�{�[�����Ƃ܂����Ƃ�
static void BallEv0_ST00(actWork *act);			//�{�[�����X�^�b�N�����Ƃ�
void Bg1_ScrollActMove1(actWork *act);

//�C�x���g�p
static void StackEventInit(actWork* act);
static void AddEventCharObj00(actWork *act);
static void AddEventCharObj01(actWork *act);
static void Ev0_MoveAct00(actWork *act);
static void Ev1_MoveAct00(actWork *act);

//========================================================================================
//									�O���[�o���ϐ�
//========================================================================================
#include "rlt_com.dat"
#include "rlt_obj.dat"

extern RLT_TABLE_PARAM	g_table[];	//��̃p�����[�^
//========================================================================================
//									�SOBJ���ʏ���
//========================================================================================
//--------------------------------------
//		���[���b�gOBJ�p���b�g�ݒ�
//--------------------------------------
void RouletteObjPal( u8 flg )
{
	if( flg == 0 ){	//�o�^
		ObjPalManInit();
		ObjPalSets( (PalData *)&rlt_obj_Pal );

		//�]���A�N�^�[�p�f�[�^�����[�N�ɓW�J
		LZ77UnCompWram((void *)rlt_event_LZ	,(void *)OBJ_EVPOKE_ADRS);
		LZ77UnCompWram((void *)rlt_ball_LZ	,(void *)OBJ_BALL_ADRS);
		LZ77UnCompWram((void *)rlt_shadow_LZ,(void *)OBJ_SHADOW_ADRS);
	}
	else{	//�폜
		ObjPalManInit();
	}
}


//========================================================================================
//						��]OBJ���ʏ����i���[���b�g�Ղ̃|�P�����j
//========================================================================================
//-------------------------
//		��]OBJ�Z�b�g
//-------------------------
static u8 AddRotateObj( const actHeader * header, u8 width, u16 * angle )
{
	u8	id = 0;
	u8	pri = header->oamData_p->VPos;

	//�I�u�W�F�N�g���V�X�e���ɒǉ�
	//OAM�f�[�^��VPosition��OBJ�v���C�I���e�B�ɓK�p���Ă���
	id = AddActor( header, CENTER_X_OBJ, CENTER_Y_OBJ,pri);
	ActWork[id].work[ ACT_ANGLE ] = *angle;		//���b�g�̏����l���Z�b�g
	ActWork[id].work[ ACT_WIDTH ] = width;		//�g�����X�̏����l���Z�b�g
	ActWork[id].scr = 1;
	ActWork[id].anm_pause = 1;
	ActWork[id].aff_pause = 1;
	*angle += ANGLE_SPACE;		//rot�ɗׂ�OBJ�Ƃ̊p�x���𑫂�

	//Rot��360�𒴂����烊�Z�b�g
	if( *angle >= THETA_360_DEF )	*angle -= THETA_360_DEF;

	return id;
}


//========================================================================================
//								�q���e�[�u���̃|�P����
//========================================================================================
//--------------------------------
//		�|�P�����}�[�N�Z�b�g
//--------------------------------
/*
	�� | �� | �� | �~
	0  | 09 | 06 | 03
	04 | 01 | 10 | 07
	08 | 05 | 02 | 10
	�̏��ŕ\������
*/
void AddRoulettePokeWinOBJ(void)
{
	u8	i,j;
	u8	dy;
	u8	id = 0;
	CellData	cell;
	
	//�Z���Z�b�g1
	LZ77UnCompWram(rlt_wPokeCell.ch_data,(void *)UserWork);
	cell.ch_data = (void *)UserWork;
	cell.size	 = rlt_wPokeCell.size;
	cell.act_num = rlt_wPokeCell.act_num;
	CellSet(&cell);

	//�Z���Z�b�g�Q
	LZ77UnCompWram(rlt_wPoke2Cell.ch_data,(void *)UserWork);
	cell.ch_data = (void *)UserWork;
	cell.size	 = rlt_wPoke2Cell.size;
	cell.act_num = rlt_wPoke2Cell.act_num;
	CellSet(&cell);
	
	//�q���e�[�u���̃|�P����OBJ��ActorID��ۑ����Ă���
	for( i=0; i<3; i++ ){
		dy = i*24;
		for( j=0; j<4; j++ ){
			id = MG_RLT->act_id[ WIN_POKE00+i*4+j ] =
				AddActor( &normal2_poke_Header[j], 148+j*24,92 + dy, PRI_WINPOKE );

			ActWork[id].anm_pause = 1;
			dy += 24;
			if(dy >= 24*3) dy = 0;
		}
	}
	//�����
	for(i = 0;i < 4;i++){
		id = MG_RLT->act_id[ WIN_POKE12+i] = 
			AddActor( &normal_poke_Header[i],148+24*i,70,PRI_WINPOKE);
		ActWork[id].anm_pause = 1;
	}
	//�c���
	for(i = 0;i < 3;i++){
		id = MG_RLT->act_id[ WIN_POKE16+i] = 
			AddActor( &normal_poke_Header[4+i],126,92+24*i,PRI_WINPOKE);
		ActWork[id].anm_pause = 1;
	}
}


//--------------------------------
//		�|�P�����}�[�N�폜
//--------------------------------
void DelRoulettePokeWinOBJ(void)
{
	u8	i;

	for( i=0; i<12; i++ )
		DelActor( &ActWork[ MG_RLT->act_id[ WIN_POKE00+i ] ] );
}

//----------------------------------
//		�|�P�����}�[�N�\���ؑ�
//----------------------------------
/*
	u8	flg ==  1	���ׂĂ�����
				0	�q�b�g���Ă���|�P������������
	u8	no		�Ō�Ƀq�b�g�����Z���̃r�b�g�i���o�[
*/
void BanishRoulettePokeControl( u8 flg ,u8 no)
{
	u8	i;

	switch(flg)
	{
	case 1:
		//���ׂẴE�B���h�E�|�P�����I�u�W�F�N�g������
		for(i = 0;i < WIN_OBJ_NUM;i++)
			ActWork[MG_RLT->act_id[WIN_POKE00+i]].banish = 1;
		return;		
	case 0:	//�q�b�g���Ă���|�P������������
		for( i=0; i< RLT_CELL_MAX; i++ ){
			if( ( MG_RLT->rlt_hp & g_rltCell[i].ui_flg) != 0)
			{
				if(g_rltCell[i].uc_bit != no)
					ActWork[ MG_RLT->act_id[ WIN_POKE00+i ] ].banish = 1;
				else
					ActWork[ MG_RLT->act_id[ WIN_POKE00+i ] ].banish = 0;
			}
			else	//�q�b�g���Ă��Ȃ��Ƃ�
			{
				ActWork[ MG_RLT->act_id[ WIN_POKE00+i ] ].banish = 0;
			}
		}
		//�c���\���̃|�P������ON
		for(;i< WIN_OBJ_NUM;i++)
			ActWork[ MG_RLT->act_id[ WIN_POKE00+i]].banish = 0;
		break;	
	}
}

//========================================================================================
//								�{�[�����������Z���̕\��
//========================================================================================
//--------------------------------
//		�x�b�g�e�[�u���p�{�[���Z�b�g
//--------------------------------
/*----------------------------------------
	�{�[�����������Z���̕\���p�A�{�[���I�u�W�F�N�g�쐬

	void AddSTCellObj(void);
-----------------------------------------*/
void AddSTCellObj(void)
{
	u8	i = 0;
	
	//�x�b�g�e�[�u���p�{�[���I�u�W�F�N�g��ActorID��ۑ����Ă���
	for(i = 0;i < RLT_BET_MAX;i++)
	{
		MG_RLT->act_id[ RLT_STCELL00 + i] = 
				AddActor( &rlt_ball_Header,92+24,20,PRI_STCELL);

		ActWork[MG_RLT->act_id[ RLT_STCELL00 + i]].banish = 1;
		ActWork[MG_RLT->act_id[ RLT_STCELL00 + i]].work[0] = 1;
		ActWork[MG_RLT->act_id[ RLT_STCELL00 + i]].move = Bg1_ScrollActMove;

		//�\���v���C�I���e�B�ύX
		ActWork[MG_RLT->act_id[ RLT_STCELL00 + i]].oamData.Priority = 1;
		
		//�A�j���[�V������8�p�^�[����
		ActAnmChg(&(ActWork[MG_RLT->act_id[RLT_STCELL00 + i]]),8);
	}
	return;
}

//--------------------------------------------------
//		�x�b�g�e�[�u���p�{�[���I�u�W�F�N�g�\���ؑ�
//--------------------------------------------------
/*	
	u8	flg ==  0	����
				1	���łɗ������ꏊ�ɕ\��
				2	�Ō�ɗ������ꏊ�ȊO��\��
*/
void BanishSTCellControl( u8 flg,u8 no)
{
	u8	i = 0;
	
	//�����Ƃ�
	if(flg){
		for(i = 0;i < RLT_BET_MAX;i++){
			ActWork[MG_RLT->act_id[RLT_STCELL00+i]].banish = 1;
		}
		return;
	}

	//�\������Ƃ�
	for(i = 0;i < RLT_BET_MAX;i++)
	{
		if(MG_RLT->hit_cell[i] == 0 || i == no){
			ActWork[MG_RLT->act_id[RLT_STCELL00+i]].banish = 1;
			continue;
		}
		
		ActWork[MG_RLT->act_id[RLT_STCELL00+i]].banish = 0;
		ActWork[MG_RLT->act_id[RLT_STCELL00+i]].x = 
			(g_rltBet[MG_RLT->hit_cell[i]].posx + 1)*8 + 4;
		ActWork[MG_RLT->act_id[RLT_STCELL00+i]].y = 
			(g_rltBet[MG_RLT->hit_cell[i]].posy + 1)*8 + 3;
	}
}

void BanishWakuControl(u8 no)
{
	if(no == 0){
		ActWork[MG_RLT->act_id[RLT_WAKU]].banish = 1;
		return;
	}
	
	//�\��
	ActWork[MG_RLT->act_id[RLT_WAKU]].banish = 0;
	ActWork[MG_RLT->act_id[RLT_WAKU]].x = (g_rltBet[no].posx+2)*8;
	ActWork[MG_RLT->act_id[RLT_WAKU]].y = (g_rltBet[no].posy+2)*8;
}
//=======================================================================================
//							���[���b�g�Ղ̃|�P����
//=======================================================================================
//----------------------------------------
//		���[���b�g�Ղ̃|�P�����Z�b�g
//----------------------------------------
void AddRotatePokeOBJ(void)
{
	u8	i,j,id = 0;
	u16	angle;

	//�Z���Z�b�g
	CellData	cell;

	//���k�f�[�^��W�J
#if 0
	for(i = 0;i < 4;i ++){
		LZ77UnCompWram(rlt_rPokeCell[i].ch_data,(void*)UserWork);
		cell.ch_data = (void *)UserWork;
		cell.size	 = rlt_rPokeCell[i].size;
		cell.act_num = rlt_rPokeCell[i].act_num;
		CellSet(&cell);
	}
#else
	LZ77UnCompWram(rlt_rPokeCell.ch_data,(void*)UserWork);
	cell.ch_data = (void *)UserWork;
	cell.size	 = rlt_rPokeCell.size;
	cell.act_num = rlt_rPokeCell.act_num;
	CellSet(&cell);
#endif	
	
	angle = INIT_ANGLE;	//�A���O���͏����p�x�Ɨׂ�OBJ�Ƃ̊p�x��
	if(angle >= THETA_360_DEF)	angle -= THETA_360_DEF;

	//POKE_WIDTH = �|�P�����̃��[���b�g�ł���̂��ӂ�����
	for( i=0; i<3; i++ ){
		for( j=0; j<4; j++ )
		{
			id = MG_RLT->act_id[RLT_RPOKE00 + i*4+j] = 
				AddRotateObj( &rotate_poke_Header[i*4+j], POKE_WIDTH, &angle );
			ActWork[id].anm_pause = 1;
			ActWork[id].aff_pause = 1;
		}
	}
}

//--------------------
//		���[���b�g�Ճ|�P����OBJ�̉�]����
//--------------------
static void RotatePokeActMove( actWork * act )
{
	s16	sin,cos;
	s16	angle;
	u8	affNo;

	//���[���b�g�Ղ�Rot�ƃA�N�^�[OBJ��Rot�̃I�t�Z�b�g�𑫂����킹�Ă���
	angle = MG_RLT->bg_rotate + act->work[ ACT_ANGLE ];
	if(	angle >= THETA_360_DEF )	angle -= THETA_360_DEF;
	
	//�Ղ̏�̉�]�^��
	sin = Sin_360_Get( (u16)angle );
	cos = Cos_360_Get( (u16)angle );
	//��]�̒��S����̃I�t�Z�b�g������Ă���
	act->dx = (  sin * act->work[ ACT_WIDTH ] ) >> 12;
	act->dy = ( -cos * act->work[ ACT_WIDTH ] ) >> 12;

	//�I�u�W�F�N�g�̉�]����
	affNo = act->oamData.AffineParamNo;
	sin = (s16)(((s32)sin * FF_MAG1000)/0x100);
	OBJ_AffineWork[ affNo ].H_DiffX = 
	OBJ_AffineWork[ affNo ].V_DiffY = (s16)(((s32)cos * FF_MAG1000)/0x100);
	OBJ_AffineWork[ affNo ].V_DiffX =  sin;
	OBJ_AffineWork[ affNo ].H_DiffY = -sin;
}


//========================================================================================
//								BG1_Window�\��OBJ
//========================================================================================
void AddBg1OBJ(void)
{
	u8	i;

	//�Z���o�^
	CellData	cell;
	for(i = 0;i < 5;i++){
		LZ77UnCompWram(rlt_bgoCell[i].ch_data,UserWork);
		cell.ch_data = UserWork;
		cell.size	 = rlt_bgoCell[i].size;
		cell.act_num = rlt_bgoCell[i].act_num;
		CellSet(&cell);
	}
	
	//�N���W�b�g�E�B���h�E
	MG_RLT->act_id[WIN_COIN] = AddActor(&(rlt_coin_Header[0]),208,16,PRI_WIN);
	ActWork[MG_RLT->act_id[WIN_COIN]].anm_pause = 1;
	
	//�N���W�b�g�E�B���h�E�̐���
	for(i = 0;i < 4;i++){
		MG_RLT->act_id[WIN_COIN00+i] = 
			AddActor( &(rlt_coin_Header[1]),196+i*8,24,PRI_NUM);
		ActWork[ MG_RLT->act_id[WIN_COIN00 +i] ].banish = 1;
		ActWork[ MG_RLT->act_id[WIN_COIN00 +i] ].anm_pause = 1;
	}
	
	//�I�b�Y�\��
	MG_RLT->act_id[WIN_ODDS] = AddActor(&rlt_odds_Header,120,68,PRI_WIN);
	ActWork[MG_RLT->act_id[WIN_ODDS]].anm_pause = 1;

	//�c��̋ʕ\��
	for(i = 0;i < 3;i++){
		MG_RLT->act_id[WIN_BALL00+i] = 
			AddActor( &rlt_restball_Header,192+i*16,36,PRI_WIN);
		ActWork[ MG_RLT->act_id[WIN_BALL00 + i] ].banish = 1;
		ActWork[ MG_RLT->act_id[WIN_BALL00 + i] ].anm_pause = 1;
	}

	//�g
	MG_RLT->act_id[RLT_WAKU] = AddActor(&rlt_waku_Header,152,96,PRI_WAKU);
	ActWork[MG_RLT->act_id[RLT_WAKU]].oamData.Priority = 1;
	ActWork[MG_RLT->act_id[RLT_WAKU]].anm_pause = 1;
	ActWork[MG_RLT->act_id[RLT_WAKU]].banish = 1;
}

//�R�C���̕\���R���g���[��
void DrawCoinControl(u16 coin)
{
	u16	p=1000;
	u8	i,no,flg=0;

//	coin = Fld.my_coin;

	for( i=0; i<4; i++ ){
		no = coin / p;
		//��\��
		ActWork[ MG_RLT->act_id[WIN_COIN00+i] ].banish = 1;
		if( no != 0 || flg != 0 || i == 3 ){
			ActWork[ MG_RLT->act_id[WIN_COIN00+i]].banish = 0;
			ActWork[ MG_RLT->act_id[WIN_COIN00+i]].oamData.CharNo =
				ActWork[ MG_RLT->act_id[WIN_COIN00+i]].char_num + 
				(u16)(ActWork[ MG_RLT->act_id[WIN_COIN00+i]].anm_tbl)[0][no].pat;
			flg = 1;
		}
		coin %= p;
		p /= 10;
	}
}
////////////////////////////////////////////////////
//�I�b�Y�̕\���R���g���[��
/*-----------------------------------------------
	�w��Z���̃I�b�Y��Ԃ�

	static u8 GetOddsNoPreCell(u8 no);

	u8	no	�w�肷��e�[�u���Z���i���o�[
--------------------------------------------------*/
static u8 GetOddsNoPreCell(u8 no)
{
	u8	oddsTbl[5] = {0,1,2,3,4};

	if(no > 19) no = 0;
	
	switch(g_rltBet[no].odds)
	{
	case 3:		//�F�q���̃Z���ꍇ
		no = no /5-1;
		if(MG_RLT->hit_hLine[no] >= 4) return 0;
		
		return oddsTbl[MG_RLT->hit_hLine[no]+1];
	case 4:		//��ޓq���̃Z���̏ꍇ
		no = no-1;
		if(MG_RLT->hit_vLine[no] >= 3) return 0;
		return oddsTbl[MG_RLT->hit_vLine[no]+2];
		break;
	case 12:	//�P�̂ɓq�����ꍇ
		if(MG_RLT->rlt_hp & g_rltBet[no].flg) return 0;
		return oddsTbl[4];
	default:
		return 0;
	}
}
//�����F���ݑI������Ă���Z���i���o�[
void DrawOddsControl(u8 no)
{
	actWork *act = &(ActWork[MG_RLT->act_id[WIN_ODDS]]);
	//���݂̃I�b�Y�̃A�j���[�V�����i���o�[�����߂�
	act->anm_offs = GetOddsNoPreCell(no);
	act->oamData.CharNo = act->char_num + 
		(u16)(act->anm_tbl)[0][act->anm_offs].pat;
}

//////////////////////////////////////////////////////
//�c��̋ʕ\��
//�����F�c��̋ʐ�
void DrawRestballControl(u8 num)
{
	u8	i = 0,ofs = 0;
	
	if(MG_RLT->uc_rate == 1){
		ofs = 2;
	}
	switch(num)
	{
	case 6:
		for(i = 0;i < 3;i++){
			ActWork[ MG_RLT->act_id[ WIN_BALL00+i] ].banish = 0;
			ActWork[ MG_RLT->act_id[ WIN_BALL00+i]].oamData.CharNo =
				ActWork[ MG_RLT->act_id[WIN_BALL00+i]].char_num + 
				(u16)(ActWork[ MG_RLT->act_id[WIN_BALL00+i]].anm_tbl)[0][0].pat;
		}
		return;
	case 5:
		ActWork[ MG_RLT->act_id[ WIN_BALL02]].oamData.CharNo =
			ActWork[ MG_RLT->act_id[WIN_BALL02]].char_num + 
			(u16)(ActWork[ MG_RLT->act_id[WIN_BALL02]].anm_tbl)[0][1+ofs].pat;
		return;
	case 4:
		ActWork[ MG_RLT->act_id[ WIN_BALL02]].oamData.CharNo =
			ActWork[ MG_RLT->act_id[WIN_BALL02]].char_num + 
			(u16)(ActWork[ MG_RLT->act_id[WIN_BALL02]].anm_tbl)[0][2+ofs].pat;
		return;
	case 3:
		ActWork[ MG_RLT->act_id[ WIN_BALL01]].oamData.CharNo =
			ActWork[ MG_RLT->act_id[WIN_BALL01]].char_num + 
			(u16)(ActWork[ MG_RLT->act_id[WIN_BALL01]].anm_tbl)[0][1+ofs].pat;
		return;
	case 2:
		ActWork[ MG_RLT->act_id[ WIN_BALL01]].oamData.CharNo =
			ActWork[ MG_RLT->act_id[WIN_BALL01]].char_num + 
			(u16)(ActWork[ MG_RLT->act_id[WIN_BALL01]].anm_tbl)[0][2+ofs].pat;
		return;
	case 1:
		ActWork[ MG_RLT->act_id[ WIN_BALL00]].oamData.CharNo =
			ActWork[ MG_RLT->act_id[WIN_BALL00]].char_num + 
			(u16)(ActWork[ MG_RLT->act_id[WIN_BALL00]].anm_tbl)[0][1+ofs].pat;
		return;
	case 0:
	default:
		for(i = 0;i < 3;i++){
			ActWork[ MG_RLT->act_id[ WIN_BALL00+i]].oamData.CharNo =
				ActWork[ MG_RLT->act_id[WIN_BALL00+i]].char_num + 
				(u16)(ActWork[ MG_RLT->act_id[WIN_BALL00+i]].anm_tbl)[0][2+ofs].pat;
		}
		return;
	}
}

/////////////////////////////////////////////////
//�X�N���[���X�N���[������

//BG1�̃X�N���[���Ɉˑ�
void Bg1_ScrollActMove(actWork *act)
{
	act->dx = MG_RLT->bg1_scrX;
}

//========================================================================================
//										�ʂ�����ꏊ
//========================================================================================
//---------------------
//		AddActor��Z���^�[�e�[�u��
//---------------------
void AddBallHoleObj(void)
{
	u8 id;
	CellData	cell;
	
	//�Z���^�[�e�[�u�� 
	LZ77UnCompWram(rlt_ctblCell.ch_data,(void *)UserWork);
	cell.ch_data = (void *)UserWork;
	cell.size	 = rlt_ctblCell.size;
	cell.act_num = rlt_ctblCell.act_num;
	CellSet(&cell);

	id = AddActor(&rlt_ctbl_Header,CENTER_X_OBJ,CENTER_Y_OBJ,PRI_CTBL);
	
	ActWork[id].work[ ACT_ANGLE ] = MG_RLT->bg_rotate;		//���b�g�̏����l���Z�b�g
	ActWork[id].work[ ACT_WIDTH ] = 0;		//�g�����X�̏����l���Z�b�g
	ActWork[id].anm_pause = 1;
	ActWork[id].aff_pause = 1;
	ActWork[id].scr = 1;

	return;
}

/*-------------------------------------------
	�Z���^�[�e�[�u��OBJ�̉�]����

	FileLocal
	static void RotateCtblActMove(actWork *act);
---------------------------------------------*/
static void RotateCtblActMove(actWork* act)
{
	u8	affNo;

	//�I�u�W�F�N�g�̉�]����
	affNo = act->oamData.AffineParamNo;

	OBJ_AffineWork[ affNo ].H_DiffX =
	OBJ_AffineWork[ affNo ].V_DiffY = MG_RLT->bg_sPa;
	OBJ_AffineWork[ affNo ].V_DiffX = MG_RLT->bg_sPb;
	OBJ_AffineWork[ affNo ].H_DiffY = MG_RLT->bg_sPc;
	return;
}

//========================================================================================
//										��
//========================================================================================
//------------------------
//		AddActor-�{�[��
//------------------------
void AddBallObj(void)
{
	u8	i;

	for(i = 0;i < RLT_BALL_MAX;i++)
	{
		MG_RLT->act_id[RLT_BALL0 + i] =		//ID�ۑ� 
			AddActor(&rlt_ball_Header,RLT_BALL_X,RLT_BALL_Y,PRI_BALL+(RLT_BALL_MAX-1-i));
		if(MG_RLT->act_id[RLT_BALL0 + i] != ACT_MAX)
		{
			ActWork[MG_RLT->act_id[RLT_BALL0 + i]].banish = 1;	//���͔�\��
			ActWork[MG_RLT->act_id[RLT_BALL0 + i]].scr = 1;		//�X�N���[���X�N���[���ˑ�
		}
	}
}

/*------------------------------------------------
	�{�[��6�̃p�����[�^�[���N���A����
	
	global
	void G_BallParamClear(void);
--------------------------------------------------*/
void G_BallParamClear(void)
{
	u8 i,no,ct;

	no = MG_RLT->act_id[RLT_BALL0];
	for(i = 0;i < RLT_BALL_MAX;i++)
	{
		ActWork[no].banish = 1;		//��\��
		ActWork[no].move = DummyActMove;		//�����܂���
		ActAnmChg(&(ActWork[no]),0);			//�A�j���`�F���W
		
		//���[�N�̏�����
		for(ct = 0;ct < 8;ct++)
			ActWork[no].work[ct] = 0;
	
		no++;
	}
	return;
}

//////////////////////////////////////////////////
//	�{�[���̓���֐�
/////////////////////////////////////////////////
#define BGROT_BIG	(0x0001)
#define BGROT_MIN	(0x0000)

#define GetBallAngle(angle,act){									\
	(angle) = (act)->work[BALL_R]+BALL_OFS_R;						\
	if((angle) < 0) (angle) += THETA_360_DEF;						\
	else if((angle) >= THETA_360_DEF) (angle) -= THETA_360_DEF;		\
}

/*----------------------------------------------
	���݂̃{�[����ROT�ƁA�Ղ�ROT�Ƃ̍���Ԃ�
	�A�N�^�[���[�N�ɂ������ɕԂ�

	s16	Get_B2B_RotDiff(actWork *act);
-------------------------------------------------*/
s16	Get_B2B_RotDiff(actWork *act)
{
	if(MG_RLT->bg_rotate > act->work[BALL_R])
	{
		act->work[BGROT_OFS] = 
			THETA_360_DEF - MG_RLT->bg_rotate + act->work[BALL_R] ;
		
		if(act->work[BGROT_OFS] >= THETA_360_DEF)
			act->work[BGROT_OFS] -= THETA_360_DEF;
	}
	else{
		act->work[BGROT_OFS] = act->work[BALL_R]-MG_RLT->bg_rotate;
	}
	
	return	act->work[BGROT_OFS];
}

/*-----------------------------------------------
	���݂̃{�[����ROT�ƁA�Ղ�ROT����A
	���݃{�[��������Z���i���o�[��Ԃ�

	u8	GetBallExistCell(actWork *act);
-------------------------------------------------*/
u8	GetBallExistCell(actWork *act)
{
	s16 rot;
	
	rot = Get_B2B_RotDiff(act);	//BG�ƃ{�[���Ƃ�ROT�̃I�t�Z�b�g�����߂�
	MG_RLT->ball.uc_cell = (u8)((float)rot/30.0f);

	return MG_RLT->ball.uc_cell;
}

/*-----------------------------------------------
	�Ղƃ{�[���̓����ɕK�v�ȁASync�I�t�Z�b�g�����߂�

	s16	Get_B2B_SyncOfs(actWork *act);
-------------------------------------------------*/
s16 Get_B2B_SyncOfs(actWork *act)
{
	s16	ofs;

	//�ՂƂ�ROT������A�I�t�Z�b�g�����߂�
	ofs = (Get_B2B_RotDiff(act)%30);	//0-29

	if(ofs == 14){
		act->work[BALL_TIME] = 0;
		return 0;		//�҂�����Z���̐^��
	}
	
	if(ofs < 14){	//0����13�̎��́A14�܂ł��炷
		ofs = 14 - ofs;
	}
	else{	//15�ȏ�
		ofs = 29 - ofs + 14;	//29-16,16-29,
	}
	act->work[BALL_TIME] = ofs;
	return ofs;
}

/*-----------------------------------------------
	�{�[���̔��a��K�p�����~�^��

	FileLocal
	static void RotateBallMove(actWork *act)
--------------------------------------------------*/
static void RotateBallMove(actWork* act)
{
	s16	sin,cos;
	
	//�Ղ̏�̉�]�^��
	MG_RLT->ball.frv += MG_RLT->ball.fra;
	MG_RLT->ball.fr += MG_RLT->ball.frv;

	if( MG_RLT->ball.fr >= (float)THETA_360_DEF )
		MG_RLT->ball.fr -= (float)THETA_360_DEF;
	else if(MG_RLT->ball.fr < 0.0f)
		MG_RLT->ball.fr += (float)THETA_360_DEF;
	
	act->work[BALL_R] = (s16)(MG_RLT->ball.fr);

	//���a�̌v�Z
	MG_RLT->ball.fwv += MG_RLT->ball.fwa;
	MG_RLT->ball.fw += MG_RLT->ball.fwv;
	act->work[BALL_W] = (s16)MG_RLT->ball.fw;

	sin = Sin_360_Get( (u16)act->work[BALL_R] );
	cos = Cos_360_Get( (u16)act->work[BALL_R] );

	//��]�̒��S����̃I�t�Z�b�g������Ă���
	act->dx = (  sin * act->work[ BALL_W ] ) >> 12;
	act->dy = ( -cos * act->work[ BALL_W ] ) >> 12;

	if(SeEndCheck()){
		m4aMPlayPanpotControl( &m4a_mplay001, 0xffff, act->dx);
		m4aMPlayPanpotControl( &m4a_mplay002, 0xffff, act->dx);
	}
	return;
}

//�{�[�������S�ɂƂ܂�Ƃ��ɌĂ�
#define BallMoveEnd(act)											\
{																	\
	MG_RLT->ball.uc_flg = 0xFF;										\
	MG_RLT->ev_bact = 0;											\
	ActAnmChg((act),3+(act)->anm_offs);								\
	GetBallExistCell((act));										\
	(act)->work[BALL_W] = 24+6;										\
	Get_B2B_RotDiff((act));											\
	(act)->work[BGROT_OFS] = ((act)->work[BGROT_OFS]/30)*30+15;		\
	(act)->move = BallMoveAct_END;									\
	m4aSongNumStartOrChange(SE_HASHI);								\
}

////////////////////////////////////////////////////////////
///	�{�[���� �A�N�^�[�V�[�P���X
/////////////////////////////////////////////////////////////
/*------------------------------------------------
	�{�[���̓���(�����A����)
	�ՂƓ������ĉ�]�^��

	static void BallMoveAct_END(actWork *act);
--------------------------------------------------*/
void BallMoveAct_END(actWork *act)
{
	s16	sin,cos;
	
	//�ՂƓ������Ẳ�]�^��
	act->work[BALL_R] = MG_RLT->bg_rotate + act->work[BGROT_OFS];
	if(act->work[BALL_R] >= 360)	act->work[BALL_R] -= 360;
	
	sin = Sin_360_Get( (u16)act->work[BALL_R] );
	cos = Cos_360_Get( (u16)act->work[BALL_R] );

	//��]�̒��S����̃I�t�Z�b�g������Ă���
	act->dx = (  sin * act->work[ BALL_W ] ) >> 12;
	act->dy = (( -cos * act->work[ BALL_W ] ) >> 12)+ActOY;

	return;
}

/*--------------------------------------------------
	�X�^�b�N�C�x���g00_1

	static void BallEv0_ST01(actWork *act);
----------------------------------------------------*/
static void BallEv0_ST01(actWork *act)
{
	RotateBallMove(act);

	act->work[BALL_TIME]++;
	
	//��ʊO�\���h�~
	if(act->work[BALL_W] > 80 || act->work[BALL_W] < -132){
		act->banish = 1;
	}	
	else
		act->banish = 0;
	
	//�X�^�[�g����ɏI���t���O�����̂�h�~
	if(act->work[BALL_TIME]  < 30) return;
	
	if(act->work[BALL_FLG] == 0)
	{
		if(MG_RLT->ball.fw <= (MG_RLT->ball.fwo-2)){
			BallMoveEnd(act);
			MG_RLT->ball.fwa = MG_RLT->ball.fwv = 0;
			MG_RLT->ball.frv = -1.0f;
		}
	}
	else{
		if(MG_RLT->ball.fw >= (MG_RLT->ball.fwo-2)){
			BallMoveEnd(act);
			MG_RLT->ball.fwa = MG_RLT->ball.fwv = 0;
			MG_RLT->ball.frv = -1.0f;
		}
	}
}
/*------------------------------------------------
	�X�^�b�N�C�x���g00_0

	static void BallEv0_ST00(actWork *act);
--------------------------------------------------*/
static void BallEv0_ST00(actWork *act)
{
	float v0 = 0,t0 = 0,t = 0;
	
	RotateBallMove(act);

	switch(act->work[BALL_R])
	{
	case 0:
		if(act->work[BALL_FLG] == 1) return;
		
		t0 = (float)act->work[BALL_WORK];	//���Z����ɗ��Ƃ����H
		t = t0*g_table[MG_RLT->rlt_ver].uc_rBase+
			(g_table[MG_RLT->rlt_ver].uc_rBaseH-1);
		v0 = (t0/g_table[MG_RLT->rlt_ver].ev[0].prm2);
		break;
	case 180:
		if(act->work[BALL_FLG] == 0) return;

		t0 = (float)(act->work[BALL_WORK]);	//���Z����ɗ��Ƃ����H
		t = t0*g_table[MG_RLT->rlt_ver].uc_rBase+
			(g_table[MG_RLT->rlt_ver].uc_rBaseH-1);
		v0 = -(t0/g_table[MG_RLT->rlt_ver].ev[0].prm2);
		break;
	default:
		return;
	}
	MG_RLT->ball.fwo = MG_RLT->ball.fw;
	MG_RLT->ball.fwv = v0;
	MG_RLT->ball.fwa = -( (2*v0/t ) + (2 / (t*t)));
	MG_RLT->ball.frv = 0.0f;
	act->anm_pause = 0;
	act->anm_no = 0;
	act->anm_sw = 1;
	act->anmend_sw = 0;
	act->move = BallEv0_ST01;
	act->work[BALL_TIME] = 0;
	return;
}

/*------------------------------------------------
	�X�^�b�N�C�x���g01_2

	�X�o���Ɏ����グ����,
	�����グ��ꂽ���ƁA�^�C��������܂őҋ@
	
	static void BallEv1_ST02(actWork *act);
--------------------------------------------------*/
static void BallEv1_ST02(actWork *act)
{
	//�����^��
	act->dy = (short)(0.05f*act->work[BALL_TIME]*act->work[BALL_TIME])-45;
	act->work[BALL_TIME]++;

	if(act->work[BALL_TIME] < 30 ) return;
	if(act->dy < 0) return;

	//�I�t�Z�b�gY��0�ɂȂ�����
	BallMoveEnd(act);

	//�C�x���g�I���t���O�𗧂Ă�
	MG_RLT->ev_end = 1;
}

/*------------------------------------------------
	�X�^�b�N�C�x���g01_1

	�X�o���Ɏ����グ����,
	�����グ��ꂽ���ƁA�^�C��������܂őҋ@
	
	static void BallEv1_ST01(actWork *act);
--------------------------------------------------*/
static void BallEv1_ST01(actWork *act)
{
	if(act->work[BALL_TIME]++ < 45)
	{
		act->dy--;

		if(act->work[BALL_TIME] != 45) return;
		if(ActWork[MG_RLT->act_id[EV_POKE]].anm_offs == 1) act->dy++;

		return;
	}

	//�����グ��ꂽ���ƁA�^�C��������܂ŏ㉺
	if(act->work[BALL_TIME] < act->work[BALL_WORK]){
		if( ActWork[MG_RLT->act_id[EV_POKE]].anm_wait != 0) return;
		
		if(ActWork[MG_RLT->act_id[EV_POKE]].anm_offs == 1)	act->dy++;
		else												act->dy--;

		return;
	}

	//�^�C���A�b�v������A�����^��
	act->anm_pause = 0;
	act->anm_no = 1;
	act->anm_sw = 1;
	act->anmend_sw = 0;
	act->work[BALL_TIME] = 0;
	act->move = BallEv1_ST02;

	m4aSongNumStart(SE_NAGERU);
}

/*------------------------------------------------
	�X�^�b�N�C�x���g01_0

	static void BallEv1_ST00(actWork *act);
--------------------------------------------------*/
static void BallEv1_ST00(actWork *act)
{
	RotateBallMove(act);

	switch(act->work[BALL_R])
	{
	case 90:
		if(act->work[BALL_FLG] == 1) return;
		act->move = BallEv1_ST01;
		act->work[BALL_TIME] = 0;
		break;
	case 270:
		if(act->work[BALL_FLG] == 0) return;
		act->move = BallEv1_ST01;
		act->work[BALL_TIME] = 0;
		break;
	default:
		return;
	}
}
/*------------------------------------------------
	�{�[���̓���
	�X�^�b�N�C�x���g�ғ�

	static void BallMoveAct_EvInit(actWork *act)
--------------------------------------------------*/
static void BallMoveAct_EvInit(actWork *act)
{
	//�{�[���̓���
	RotateBallMove(act);
	
	switch(MG_RLT->ev_no)
	{
	default:		
	case 0:		//�C�x���g�i���o�[�O
		AddEventCharObj00(act);
		act->move = BallEv0_ST00;
		break;
	case 1:
		AddEventCharObj01(act);
		act->move = BallEv1_ST00;
		break;
	}
}
/*------------------------------------------------
	�{�[���̓���(�X�e�[�g5)
	�����悤�Ƃ����Ƃ���ɂ��łɃ{�[���������Ƃ��̉������

	static void BallMoveAct_ST5(actWork *act);
--------------------------------------------------*/
static void BallMoveAct_ST5(actWork *act)
{
	//�{�[���̓���
	RotateBallMove(act);

	if(--act->work[BALL_TIME] == 15)
	{
		//���x���]
		MG_RLT->ball.fwv *= -1;
	}
		
	if(act->work[BALL_TIME] == 0)	//�^�C�}�[��0�ɂȂ������_�ŏI��
	{
		if(act->work[BALL_FLG] == 0){	//�X�^�b�N���Ȃ�
			//�{�[�����Ƃ߂�
			BallMoveEnd(act);
		}
		else{							//�X�^�b�N����
			act->anm_pause = 1;
			m4aSongNumStart(SE_KON);
			StackEventInit(act);		//�C�x���g0������
		}
	}
	return;
}

/*------------------------------------------------
	�{�[���̓���(�X�e�[�g4)
	�ՂƓ������邽�߂́A�{�[���̈ʒu�␳�^�C��
	�␳�����񂾂Ƃ���ŃW���b�W����������

	static void BallMoveAct_ST4(actWork *act);
--------------------------------------------------*/
#ifdef PM_DEBUG
u8	g_stackFlg = 0;
static void BallMoveAct_ST4_Dmy(actWork *act)
{
	s16	rand;
	u8	no;
	
	//�Տ�̉�]�^��
	RotateBallMove( act );
	
	//�^�C�}�[���Z�b�g
	act->work[BALL_TIME] = 0;
	
	//�W���b�W
	no = GetBallExistCell(act);
	
	//������Z�����󂫂łȂ������Ƃ�
	m4aSongNumStart(SE_KON);
	rand = pp_rand()&0x0001;

	if(rand)	//�{�[���i�s������
	{
		//�p���x�X�V
		MG_RLT->ball.frv = 0.0f;
		
		//���̃Z���i���o�[�����߂�
		no = (MG_RLT->ball.uc_cell+1)%RLT_CELL_MAX;

		//�X�^�b�N�����ꍇ�̃G���A�i���o�[���o���Ă���
		MG_RLT->ball.uc_area = (MG_RLT->ball.uc_cell+1)%RLT_CELL_MAX;
	}
	else	//�i�s�������Α�
	{
		//�p���x�X�V
		MG_RLT->ball.frv = g_table[MG_RLT->rlt_ver].f_invV*2;

		//���̃Z���i���o�[�����߂�
		no = (MG_RLT->ball.uc_cell + (RLT_CELL_MAX-1))%RLT_CELL_MAX;

		//�X�^�b�N�����ꍇ�̃G���A�i���o�[���o���Ă���
		MG_RLT->ball.uc_area = MG_RLT->ball.uc_cell;
	}

	act->work[BALL_FLG] = 1;		//�X�^�b�N
	act->work[BALL_TIME] = (short)g_table[MG_RLT->rlt_ver].uc_rBaseH;

	//���a�ɑ΂��鑬�x�̍X�V
	MG_RLT->ball.fwv = 0.085f;
	
	act->move = BallMoveAct_ST5;
	act->work[BALL_STAT] = 5;
	return;

}
#endif	//ifdef PM_DEBUG
static void BallMoveAct_ST4(actWork *act)
{
	s16	rand;
	u8	no;
	
	//�Տ�̉�]�^��
	RotateBallMove( act );
	
	//�^�C�}�[���Z�b�g
	act->work[BALL_TIME] = 0;
	
	//�W���b�W
	//������Z�����󂫂Ȃ�΁A���̂܂ܗ�����
	no = GetBallExistCell(act);
	if((g_rltCell[MG_RLT->ball.uc_cell].ui_flg & MG_RLT->rlt_hp) == 0)
	{
		BallMoveEnd(act);
		return;
	}
	//������Z�����󂫂łȂ������Ƃ�
	m4aSongNumStart(SE_KON);
	rand = pp_rand()&0x0001;

	if(rand)	//�{�[���i�s������
	{
		//�p���x�X�V
		MG_RLT->ball.frv = 0.0f;
		
		//���̃Z���i���o�[�����߂�
		no = (MG_RLT->ball.uc_cell+1)%RLT_CELL_MAX;

		//�X�^�b�N�����ꍇ�̃G���A�i���o�[���o���Ă���
		MG_RLT->ball.uc_area = (MG_RLT->ball.uc_cell+1)%RLT_CELL_MAX;
	}
	else	//�i�s�������Α�
	{
		//�p���x�X�V
		MG_RLT->ball.frv = g_table[MG_RLT->rlt_ver].f_invV*2;

		//���̃Z���i���o�[�����߂�
		no = (MG_RLT->ball.uc_cell + (RLT_CELL_MAX-1))%RLT_CELL_MAX;

		//�X�^�b�N�����ꍇ�̃G���A�i���o�[���o���Ă���
		MG_RLT->ball.uc_area = MG_RLT->ball.uc_cell;
	}

	//���̃Z�����󂢂Ă��邩�ǂ����H
	if(g_rltCell[no].ui_flg & MG_RLT->rlt_hp){	//�󂢂Ă��Ȃ�
		act->work[BALL_FLG] = 1;		//�X�^�b�N
		act->work[BALL_TIME] = (short)g_table[MG_RLT->rlt_ver].uc_rBaseH;
	}
	else{	 //�󂢂Ă���
		act->work[BALL_FLG] = 0;		//�X�^�b�N���Ȃ�
		if(MG_RLT->rlt_ver){
			act->work[BALL_TIME] = (short)g_table[MG_RLT->rlt_ver].uc_rBase;
		}
		else{
			act->work[BALL_TIME] = (short)g_table[MG_RLT->rlt_ver].uc_rBaseH;
			if(rand){
				MG_RLT->ball.frv = 0.5f;
			}else{
				MG_RLT->ball.frv = -1.5f;
			}
		}
	}

	//���a�ɑ΂��鑬�x�̍X�V
	MG_RLT->ball.fwv = 0.085f;
	
	act->move = BallMoveAct_ST5;
	act->work[BALL_STAT] = 5;
	return;
}

/*------------------------------------------------
	�{�[���̓���(�X�e�[�g3)
	�W���b�W�̔��a�ɂȂ�܂ŁA�~�^��

	static void BallMoveAct_ST3(actWork *act);
--------------------------------------------------*/
static void BallMoveAct_ST3(actWork *act)
{
	//�Տ�̉�]�^��
	RotateBallMove( act );

	if(MG_RLT->ball.frv > 0.5f) return;
	
	//���x��0.5�ȉ��ɂȂ�����W���b�W
	GetBallExistCell(act);		//���݂���Z�������߂�
	
	if(Get_B2B_SyncOfs(act) == 0){	//�Z���҂�����ɂ���̂ŕ␳�Ȃ�
		MG_RLT->ball.fra = 0.0f;
		MG_RLT->ball.frv -= 
		(float)g_table[MG_RLT->rlt_ver].uc_rSpd/(g_table[MG_RLT->rlt_ver].uc_rWait+1);
		act->work[BALL_STAT] = 4;

#ifdef PM_DEBUG
		if(g_stackFlg)	act->move = BallMoveAct_ST4_Dmy;
		else			act->move = BallMoveAct_ST4;
#else
		act->move = BallMoveAct_ST4;
#endif		//PM_DEBUG
		return;
	}

	//���x���}�C�i�X�ɂȂ�̂�h�~
	if(MG_RLT->ball.fra != 0.0f){
		if(MG_RLT->ball.frv < 0.0f){
			MG_RLT->ball.fra = 0.0f;
			MG_RLT->ball.frv = 0.0f;
			MG_RLT->ball.fwv /= 1.2;
		}
	}
	return;
}

/*------------------------------------------------
	�{�[���̓���(�X�e�[�g2)
	�~�^��

	static void BallMoveAct_ST2(actWork *act);
--------------------------------------------------*/
static void BallMoveAct_ST2(actWork *act)
{
	//�Ղ̏�̉�]�^��
	RotateBallMove( act);

	//���a�̃G���A��AREA3�ɓ˓������玟�̃X�e�[�g��
	if(MG_RLT->ball.fw > (float)RLT_AREA3_OFS) return;

	//���a�ɑ΂��鑬�x���X�V
	MG_RLT->ball.fwv = -(RLT_AREA3_SIZ/(float)MG_RLT->ball.us_syncA2);
	//�����x���X�V
	MG_RLT->ball.fra = -(MG_RLT->ball.frv/(float)MG_RLT->ball.us_syncA2);

	act->anm_no = 2;
	act->anm_sw = 1;
	act->anmend_sw = 0;
	
	act->work[BALL_STAT] = 3;
	act->move = BallMoveAct_ST3;
	return;
}

/*------------------------------------------------
	�{�[���̓���(�X�e�[�g1)
	�~�^��

	static void BallMoveAct_ST1(actWork *act);
--------------------------------------------------*/
static void BallMoveAct_ST1(actWork *act)
{
	//�Ղ̏�̉�]�^��
	RotateBallMove( act);

	//���a�̃G���A��AREA2�ɓ˓������玟�̃X�e�[�g��
	if(MG_RLT->ball.fw > (float)RLT_AREA2_OFS) return;
	
	m4aSongNumStartOrChange(SE_TAMAKORO_E);
	
	//���a�ɑ΂��鑬�x���X�V
	MG_RLT->ball.fwv = -(RLT_AREA2_SIZ/(float)MG_RLT->ball.us_syncA1);
	//�����x���X�V
	MG_RLT->ball.fra = (1.0f-MG_RLT->ball.frv)/(float)MG_RLT->ball.us_syncA1;

	act->anm_no = 1;
	act->anm_sw = 1;
	act->anmend_sw = 0;

	act->work[BALL_STAT] = 2;
	act->move = BallMoveAct_ST2;
	return;
}

/*-----------------------------------------------
	�{�[���̓���(�X�e�[�g0)
	�������E�\���J�n
	
	void BallMoveAct_ST0(actWork *act);
-------------------------------------------------*/
void BallMoveAct_ST0(actWork *act)
{
	//�����ʒu�ݒ�
	act->work[BALL_STAT] = 1;
	act->work[BALL_TIME] = 0;

	RotateBallMove(act);

	act->banish = 0;				//�\���J�n
	act->move = BallMoveAct_ST1;
}

///////////////////////////////////////////////////////////////
//	�C�x���g�L�����N�^
///////////////////////////////////////////////////////////////
/*----------------------------------------------------
	�X�^�b�N�C�x���g0�p�̃L�����N�^��ǉ�

	void AddEventCharObj00(actWork *ballAct);
	
	i	actWork*	�J�����g�{�[���̃A�N�^�[�ւ̃|�C���^
------------------------------------------------------*/
void AddEventCharObj00(actWork*	ballAct)
{
	u8	i = 0;
	s16	no = 0,ptn;

	//�{�[���̉e�̈ʒu
	s16	pos[2][2] = {{CENTER_X_OBJ,CENTER_Y_OBJ-36},
					 {CENTER_X_OBJ,CENTER_Y_OBJ+32}};
	
	//�C�x���g�p�^�[���̎擾
	no = ballAct->work[BALL_WORK]-2;
	if(MG_RLT->rlt_ver) ptn = no;
	else				ptn = no + 2;
	
	//�C�x���g�p�T�|�[�g�|�P�����̃I�u�W�F�N�g
	MG_RLT->act_id[EV_POKE] = 
	AddActor(&(rlt_evPoke_Header[0]),36,-12,PRI_EPOKE);
	
	//�{�[���̉e
	MG_RLT->act_id[EV_SHADOW0] =
	AddActor(&(rlt_shadow_Header[0]),pos[ballAct->work[BALL_FLG]][0],
									 pos[ballAct->work[BALL_FLG]][1],PRI_ESHADOW2);
	
	//�L�����N�^�[�̉e
	MG_RLT->act_id[EV_SHADOW1] = 
	AddActor(&(rlt_shadow_Header[1]),36,140,PRI_ESHADOW);
	ActWork[MG_RLT->act_id[EV_SHADOW1]].oamData.ObjMode = 1;	//������	

	//���ʂ̃p�����[�^������
	for(i = 0;i< 3;i++)
	{
		ActWork[MG_RLT->act_id[EV_POKE + i]].scr = 0;
		ActWork[MG_RLT->act_id[EV_POKE + i]].banish = 1;
		ActWork[MG_RLT->act_id[EV_POKE + i]].anm_pause = 1;
		ActWork[MG_RLT->act_id[EV_POKE + i]].aff_pause = 1;

		//�݂��̃A�N�^�[ID�i���o�[��ۑ�
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_PMID] = MG_RLT->act_id[EV_POKE];
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_SBID] = MG_RLT->act_id[EV_SHADOW0];
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_SCID] = MG_RLT->act_id[EV_SHADOW1];
	
		//�C�x���g�p�^�[���ۑ�
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_PTN] = no;	

		//�C�x���g�̉ғ����Ԃ�ۑ�
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_TIME] = 
			ballAct->work[BALL_WORK]*g_table[MG_RLT->rlt_ver].uc_rBase+
			(g_table[MG_RLT->rlt_ver].uc_rBaseH-1);
	}
	ActWork[MG_RLT->act_id[EV_SHADOW0]].scr = 1;

	//�J�����g�{�[���̃A�N�^�[�|�C���^���AMG_RLT->pAct�ɕۑ����Ă���
	MG_RLT->pAct = ballAct;
	return;
}
/*----------------------------------------------------
	�X�^�b�N�C�x���g1�p�̃L�����N�^��ǉ�

	void AddEventCharObj01(actWork *ballAct);
	
	i	actWork*	�J�����g�{�[���̃A�N�^�[�ւ̃|�C���^
------------------------------------------------------*/
void AddEventCharObj01(actWork*	ballAct)
{
	u8	i = 0;
	s16	no = 0;

	//�L�����N�^�Ɖe�̏����ʒu
	s16	pos[2][2] = {{256,CENTER_Y_OBJ+4},
					 {-16,CENTER_Y_OBJ+4}};
	
	//�C�x���g�p�^�[���̎擾
	no = ballAct->work[BALL_WORK] - 2;
	
	//�C�x���g�p�T�|�[�g�|�P�����̃I�u�W�F�N�g
	MG_RLT->act_id[EV_POKE] = 
	AddActor(&(rlt_evPoke_Header[1]),pos[ballAct->work[BALL_FLG]][0],
									 pos[ballAct->work[BALL_FLG]][1],PRI_EPOKE);
	ActAnmChg(&(ActWork[MG_RLT->act_id[EV_POKE]]),ballAct->work[BALL_FLG]);
	
	//�L�����N�^�[�̉e
	MG_RLT->act_id[EV_SHADOW0] = 
	AddActor(&(rlt_shadow_Header[2]),pos[ballAct->work[BALL_FLG]][0],
									 pos[ballAct->work[BALL_FLG]][1],PRI_ESHADOW);
	ActWork[MG_RLT->act_id[EV_SHADOW0]].aff_pause = 1;
	ActWork[MG_RLT->act_id[EV_SHADOW0]].anm_pause = 1;

	//�ғ����Ԃ�ۑ�
	ballAct->work[BALL_WORK] = 
		45+
		no*g_table[MG_RLT->rlt_ver].uc_rBase+
		g_table[MG_RLT->rlt_ver].ev[1].prm0;

	//���ʂ̃p�����[�^������
	for(i = 0;i< 2;i++)
	{
		//�݂��̃A�N�^�[ID�i���o�[��ۑ�
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_PMID] = MG_RLT->act_id[EV_POKE];
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_SBID] = MG_RLT->act_id[EV_SHADOW0];
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_SCID] = MG_RLT->act_id[EV_SHADOW0];
	
		//�C�x���g�p�^�[���ۑ�
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_PTN] = no;	

		//�C�x���g�̉ғ����Ԃ�ۑ�
		ActWork[MG_RLT->act_id[EV_POKE + i]].work[EV_TIME] = 
			ballAct->work[BALL_WORK]-45;
	}
	
	//�J�����g�{�[���̃A�N�^�[�|�C���^���AMG_RLT->pAct�ɕۑ����Ă���
	MG_RLT->pAct = ballAct;

	return;
}

//�X�^�b�N�C�x���g�������ɕK�v�Ȓl������������
/*--------------------------------------------------
	�X�^�b�N�C�x���g�̏�����
	
	static void StackEventInit(actWork* act);
	
	actWork*	�J�����g�{�[���̃A�N�^�[�A�h���X
----------------------------------------------------*/
static void  StackEventInit(actWork* act)
{
	u8	i = 0,no = 0,ofs = 0,nFree = 0,entry = 5,fix = 0;
	u8	free[10] = {0,0,0,0,0,0,0,0,0,0};
	u16 rot = 0,rand = pp_rand();
	
	//�X�^�b�N�̃t���O�𗧂Ă�
	MG_RLT->ball.uc_flg = 0x01;
	MG_RLT->ev_on = 1;
	MG_RLT->ev_end = 0;

	//�X�^�b�N�����Ƃ��������Z���i���o�[���Z�b�g
	MG_RLT->ball.uc_cell = 0xFF;
	
	MG_RLT->ball.fr = (float)(act->work[BALL_R]);
	
	//�{�[���̑��x��ݒ肷��
	MG_RLT->ball.fwv = 0.0f;	
	MG_RLT->ball.frv = g_table[MG_RLT->rlt_ver].f_invV;

	//�t���O�ݒ�
	rot = 33 + MG_RLT->rlt_ver*30 + (1-MG_RLT->ev_no)*15;
	for(i = 0;i < 4;i++){
		if(	(rot < act->work[BALL_R]) && (act->work[BALL_R]) <= rot+90){
			act->work[BALL_FLG] = i/2;
			MG_RLT->ev_no = i%2;
			break;
		}
		if(i == 3){
			act->work[BALL_FLG] = 1;
			MG_RLT->ev_no = 1;
			break;
		}
		rot += 90;
	}

	//�����Đ�
	if(MG_RLT->ev_no){
		if(act->work[BALL_FLG])
			VoicePlay(MONSNO_SUBAME,-63);
		else
			VoicePlay(MONSNO_SUBAME,63);
	}else{
		VoicePlay(MONSNO_KINOKOKO,-63);
	}
	//�ǂ��ɗ��Ƃ������߂�	
	//������Z�� + (MG_RLT->ev_no*2)+1 �̃Z������U�Z����܂łŁA�󂢂Ă���Z����T��
	ofs = 2;
	no = (MG_RLT->ball.uc_area + ofs) % RLT_CELL_MAX;
	
	//�G���g���[��������
	if(MG_RLT->ev_no == 1 && MG_RLT->rlt_ver == 1){
		entry += ofs+4;
	}else{
		entry += ofs;
	}
	
	for(i = ofs;i < entry;i++)
	{
		//�󂢂Ă��Ȃ�
		if( (MG_RLT->rlt_hp & g_rltCell[no].ui_flg)){
			no = (no + 1) % RLT_CELL_MAX;
			continue;
		}

		//�󂢂Ă���
		free[nFree++] = i;

		//�󂢂Ă���ꏊ�Ɠq���Ă���ꏊ����v������ۑ�
		if(	(!fix) && (g_rltCell[no].ui_flg & g_rltBet[MG_RLT->uc_aBet[MG_RLT->nBet]].bet_flg)){
			fix = i;
		}
		no = (no + 1) % RLT_CELL_MAX;
	}

	//�󂢂Ă���Z�����̒����烉���_���őI��
	if((MG_RLT->ev_no+1) & MG_RLT->mons_flg){
		if(fix && ((rand&0x00FF) < 192)){
			act->work[BALL_WORK] = fix;
		}
		else{
			act->work[BALL_WORK] = free[rand % nFree];
		}
	}
	else{
		act->work[BALL_WORK] = free[rand % nFree];
//		act->work[BALL_WORK] = free[nFree-1];
	}

	//���ɔ�ԃ^�X�N��ݒ�
	act->move = BallMoveAct_EvInit;
	return;
}

///////////////////////////////////////////////////
//	�C�x���g00�p ����֐�
///////////////////////////////////////////////////
static const u16 g_alpha[10] = {
	0x0907,0x0808,0x0709,0x060A,0x050B,0x040C,0x030D,0x020E,0x010F,0x0010,
};
/*-------------------------------------------------------
	�C�x���g00_2	Charater & shadow of char

	�L�����N�^�ƃL�����N�^�̉e���A��ʂ��������܂�

	static void Ev0_MoveAct02(actWork *act);
---------------------------------------------------------*/
static void Ev0_MoveAct02(actWork *act)
{	
	//�^�C���A�b�v����܂ł͋A��
	if( act->work[EV_CNT]++ < act->work[EV_TIME]){
		return;
	}
	
	act->x -= 2;

	//��ʂ����������
	if(act->x < -16){
		if(!MG_RLT->ev_end) MG_RLT->ev_end = 1;		//�C�x���g�I���t���O�𗧂Ă�
		
		//�A�N�^�[�폜
		DelActor(act);

		MG_RLT->state_flg = 0;
		MG_RLT->w_alpha = g_alpha[0];
	}
}

/*---------------------------------------------------
	�C�x���g00_3		shadow of ball
	
	�{�[���̉e�ƃX�N���[���X�N���[������

	static void Ev0_MoveAct03(actWork *act);
-----------------------------------------------------*/
static void Ev0_MoveAct03(actWork *act)
{
	s16	rand[3][4] = {{-1,0,1,0},{-2,0,2,0},{-3,0,3,0}};
	
	//�^�C���A�b�v����܂ŁA�X�N���[���X�N���[��
	if( act->work[EV_CNT]++ < act->work[EV_TIME]){

//		if(MyData.playtime_ms%2 == 0){
		if(act->work[EV_CNT]&0x0001){
			ActOY = rand[act->work[EV_PTN]/2][act->work[EV_WORK]];
			act->work[EV_WORK] = (act->work[EV_WORK]+1)%4;
		}
		act->banish ^= 1;	//blink
		return;
	}

	//�X�N���[���X�N���[�����Z�b�g
	ActOY = 0;

	//�L�����N�^�[�̃p�^�[���A�j���J�n
	ActWork[MG_RLT->act_id[EV_POKE]].anm_pause = 0;
		
	//�A�N�^�[�폜
	DelActor(act);
}

/*-----------------------------------------------
	�C�x���g00_1	All	

	�L�����N�^�̗����^��

	static void Ev0_MoveAct01(actWork *act);
-------------------------------------------------*/
static void Ev0_MoveAct01(actWork *act)
{
	act->work[EV_CNT]++;
	act->dy = (short)(0.039f*act->work[EV_CNT]*act->work[EV_CNT]);

	MG_RLT->w_alpha = g_alpha[(MG_RLT->state_flg-1)/2];
	if(MG_RLT->state_flg < 19) MG_RLT->state_flg++;

	if(act->work[EV_CNT] > 60){
		act->work[EV_CNT] = 0;			//�^�C�}�[���Z�b�g

		//�L�����N�^
		act->move = Ev0_MoveAct02;		//����ύX

		//�L�����N�^�̉e
		ActWork[act->work[EV_SCID]].move = Ev0_MoveAct02;
		ActWork[act->work[EV_SCID]].work[EV_CNT] = -2;

		//�{�[���̉e�A�\���J�n
		ActWork[act->work[EV_SBID]].banish = 0;
		ActWork[act->work[EV_SBID]].move = Ev0_MoveAct03;

		m4aSongNumStart(SE_W070);
	}
}
/*----------------------------------------------
	�C�x���g00_0	ALL

	�{�[�����K��ʒu�ɗ���܂őҋ@

	static void Ev0_MoveAct00(actWork *act);
-------------------------------------------------*/
static void Ev0_MoveAct00(actWork *act)
{
	if(act->work[EV_WORK] == 0){
		if(MG_RLT->pAct->work[BALL_FLG] == 0){
			if(	MG_RLT->pAct->work[BALL_R] != 
				g_table[MG_RLT->rlt_ver].ev[0].prm0){
				return;
			}
		}
		else{
			if(	MG_RLT->pAct->work[BALL_R] != 
				180+g_table[MG_RLT->rlt_ver].ev[0].prm0) return;
		}

		//�{�[�����K��ʒu�ɂ���
		//�L�����N�^�̉e�̕\�����͂��߂�
		act->banish = 0;
		act->work[EV_WORK]++;
		m4aSongNumStart(SE_RU_HYUU);
		MG_RLT->state_flg = 1;
		MG_RLT->w_alpha = g_alpha[0];
	}else{
		MG_RLT->w_alpha = g_alpha[(MG_RLT->state_flg-1)/2];
		if(MG_RLT->state_flg < 19) MG_RLT->state_flg++;

		if(MG_RLT->pAct->work[BALL_FLG] == 0){
			if(	MG_RLT->pAct->work[BALL_R] != 
				g_table[MG_RLT->rlt_ver].ev[0].prm1){
				return;
			}
		}
		else{
			if(	MG_RLT->pAct->work[BALL_R] != 
				180+g_table[MG_RLT->rlt_ver].ev[0].prm1) return;
		}
		//�{�[�����K��ʒu�ɂ���
		ActWork[act->work[EV_PMID]].move = Ev0_MoveAct01;	//�L�����N�^����X�^�[�g
		ActWork[act->work[EV_PMID]].banish = 0;
		act->move = DummyActMove;
		act->work[EV_WORK] = 0;

//		VoicePlay(MONSNO_KINOKOKO,-63);
	}
	return;
}

///////////////////////////////////////////////////
//	�C�x���g01�p ����֐�
///////////////////////////////////////////////////
static void Ev1_MoveAct05(actWork *act){
	act->banish ^= 1;
}
/*-------------------------------------------------------
	�C�x���g01	Character

	�L�����N�^���{�[��������ď㏸
	��ʊO�ɏ�����܂�

	static void Ev1_MoveAct04(actWork *act);
 --------------------------------------------------------*/
static void Ev1_MoveAct04(actWork *act)
{
	if(act->y > -16){
		act->y -= 1;
		return;
	}

	act->move = DummyActMove;
	act->banish = 1;
	act->anm_pause = 1;

	m4aSongNumStop(SE_BASABASA);
	DelActor(act);
	AffineWorkNumDel(ActWork[MG_RLT->act_id[EV_SHADOW0]].oamData.AffineParamNo);
	DelActor(&(ActWork[ MG_RLT->act_id[EV_SHADOW0] ]));
	return;
}

/*-------------------------------------------------------
	�C�x���g01	Character

	�L�����N�^���{�[���������グ�ď㏸�A
	�^�C��������܂őҋ@

	static void Ev1_MoveAct03(actWork *act);
 --------------------------------------------------------*/
static void Ev1_MoveAct03(actWork *act)
{
	//�㏸��
	if(act->work[EV_CNT] >= 0){
		act->work[EV_CNT]--;
		act->y--;

		if(act->work[EV_CNT] == 0){
			if(act->anm_offs == 1) act->dy++;
		}
		return;
	}
	//�ҋ@��
	if(act->work[EV_TIME] >= 0)
	{
		act->work[EV_TIME]--;

		if(act->anm_wait != 0) return;
		
		if(act->anm_offs == 1)	act->dy++;
		else					act->dy--;

		return;
	}

	//�ҋ@�I���E�㏸���ĉ�ʊO��
	m4aSongNumStart(SE_RU_HYUU);
	ActAnmChg(act,4+MG_RLT->pAct->work[BALL_FLG]);
	act->move = Ev1_MoveAct04;

	//�e��Affine�A�j���[�V�����J�n
	ActWork[act->work[EV_SCID]].aff_pause = 0;
}

/*-------------------------------------------------------
	�C�x���g01	Character

	�L�����N�^���A��ʂɓ˓��B�{�[���ƐڐG����܂�

	static void Ev1_MoveAct02(actWork *act);
---------------------------------------------------------*/
static void Ev1_MoveAct02(actWork *act)
{
	s8	sign[2] = {-1,1};
	s8	pos[8][2] = {{2,0},{2,0},{2,-1},{2,-1},{2,-1},{2,-1},{2,-2},{2,-2}};
	s8	pan = 0;

	if(act->work[EV_CNT]-- >= 8){
		act->x += sign[MG_RLT->pAct->work[BALL_FLG]]*2;
		if(SeEndCheck()){
			pan = -((CENTER_X-act->x)/2);
			m4aMPlayPanpotControl(&m4a_mplay001,0xFFFF,pan);
			m4aMPlayPanpotControl(&m4a_mplay002,0xFFFF,pan);
		}
		return;
	}
	if(act->work[EV_CNT] >= 0){
		act->x += sign[MG_RLT->pAct->work[BALL_FLG]] * pos[7-act->work[EV_CNT]][0];
		act->y += pos[7-act->work[EV_CNT]][1];
		return;
	}

	//�J�E���^�A�b�v
	m4aSongNumStartOrChange(SE_BASABASA);
	if(MG_RLT->pAct->work[BALL_FLG] == 0) VoicePlay(MONSNO_SUBAME,63);
	else								  VoicePlay(MONSNO_SUBAME,-63);
	ActAnmChg(act,2+MG_RLT->pAct->work[BALL_FLG]);	//�x���H�΂����ɃA�j���`�F���W
	act->work[EV_CNT] = 45;
	act->move = Ev1_MoveAct03;
}
/*-------------------------------------------------------
	�C�x���g01	shadow of character

	�L�����N�^�̉e���A��ʂɓ˓��B��~����܂�

	static void Ev1_MoveAct01(actWork *act);
---------------------------------------------------------*/
static void Ev1_MoveAct01(actWork *act)
{
	s8	sign[2] = {-1,1};

	if(act->work[EV_CNT]-- >= 0){
		act->x += sign[MG_RLT->pAct->work[BALL_FLG]]*2;
		ActWork[act->work[EV_SCID]].banish ^= 1;	//blink
		return;
	}
	//�J�E���^�A�b�v
	act->move = Ev1_MoveAct05;
}

/*-------------------------------------------------------
	�C�x���g01	Charater

	�{�[�����K��̈ʒu�ɗ���܂őҋ@

	static void Ev1_MoveAct00(actWork *act);
---------------------------------------------------------*/
static void Ev1_MoveAct00(actWork *act)
{
	if(MG_RLT->pAct->work[BALL_FLG] == 0){
		if(	MG_RLT->pAct->work[BALL_R] != 
			90+g_table[MG_RLT->rlt_ver].ev[1].prm1){
			return;
		}
		ActWork[act->work[EV_SCID]].work[EV_CNT] = 52;
		ActWork[act->work[EV_PMID]].work[EV_CNT] = 52;
	}
	else{
		if(	MG_RLT->pAct->work[BALL_R] != 
			270+g_table[MG_RLT->rlt_ver].ev[1].prm2){
			return;
		}
		ActWork[act->work[EV_SCID]].work[EV_CNT] = 46;
		ActWork[act->work[EV_PMID]].work[EV_CNT] = 46;
	}

	//�{�[�����K��̈ʒu�܂ł�����
	ActWork[act->work[EV_SCID]].move = Ev1_MoveAct01;
	ActWork[act->work[EV_PMID]].move = Ev1_MoveAct02;
	m4aSongNumStart(SE_RU_HYUU);
}

