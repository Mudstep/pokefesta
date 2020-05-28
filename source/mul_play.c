//**********************************************************************
//		mul_play.c
//		�ʐM���̕�����ڲ԰����
//**********************************************************************
#define 	MUL_PLAY_H_PROTO

#include 	"common.h"
#include 	"fieldmap.h"
#include	"evobj.h"
#include	"mul_play.h"
#include	"jiki.h"

//----------------------------------------------------------------------
//		define
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//		��۰���
//----------------------------------------------------------------------
PLAYER_STATUS Player[COMM_PLAYER_MAX] = {};

//======================================================================
//		�ʐM�p��ڲ԰
//======================================================================
static void set_player_pos( EV_PLAYER *, s16, s16 );

//------------------------------------------
//	���@��񏉊���
//------------------------------------------
void InitPlayer( PLAYER_STATUS *play )
{
	memset( (u8*)play, 0, PLAYER_STATUS_SIZE );
}

//------------------------------------------
//	���@���S������
//------------------------------------------
void InitAllPlayer( void )
{
	memset( (u8*)Player, 0, PLAYER_STATUS_SIZE_MAX );
}

//------------------------------------------
//	�����OBJ ܰ��ر
//------------------------------------------
static void InitEvPlayer( EV_PLAYER *ev )
{
	memset( (u8*)ev, 0, EVOBJ_SIZE );
}

//------------------------------------------
//	���@����
//	In:		no			; Playerܰ�����
//			x,y			; ϯ��x,y
//			sex			; ����
//------------------------------------------
void SetPlayer( u8 no, s16 x, s16 y, u8 sex )
{
	u8 i;
	EV_PLAYER *ev;
	PLAYER_STATUS *play;

	i = EvObjEmptyWorkSeek();

	play = &Player[no];
	ev = (EV_PLAYER*)&EvObj[i];

	InitPlayer( play );
	InitEvPlayer( ev );
	
	play->sw = 1;
	play->my_no = no;
	play->evobjno = i;
	play->mv_status = EV_M_OFF;

	ev->sw = 1;
	ev->sex = sex;
	ev->mv_site = SITE_U;
	ev->actno = ACT_MAX;

	set_player_pos( ev, x, y );
}

//------------------------------------------
//	���W���
//------------------------------------------
static void set_player_pos( EV_PLAYER *ev, s16 x, s16 y )
{
	ev->gx = x;
	ev->gy = y;
	ev->old_gx = x;
	ev->old_gy = y;

	MapCellSizePosDiff( x, y, &ev->dx, &ev->dy );
	ev->dx += 8;
	
	EvObjHeight( (EventOBJ*)ev );
}

//------------------------------------------
//	���@�������
//	In:		no			; ���@ܰ�����
//			site		; ����
//------------------------------------------
void SetPlayerSite( u8 no, u8 site )
{
	if( Player[no].sw )
	{
		((EV_PLAYER*)EvObj)[Player[no].evobjno].mv_site = site;
	}
}

//------------------------------------------
//	���@�폜(��)
//	In:		no			; ���@ܰ�����
//------------------------------------------
void DelPlayer( u8 no )
{
	EV_PLAYER *ev;
	PLAYER_STATUS *play;

	play = &Player[no];
	ev = (EV_PLAYER*)&EvObj[play->evobjno];

	if( ev->actno != ACT_MAX )
	{
		DelActor( &ActWork[ev->actno] );
	}
	
	play->sw = 0;
	ev->sw = 0;
}

//------------------------------------------
//	��ڲ԰�̱���ܰ����ގ擾
//	In:		num			; ���@ܰ�����
//	Out:	u8			; ���ܰ�����
//------------------------------------------
u8 GetPlayerActorNumber( u8 num )
{
	return( ((EV_PLAYER*)EvObj)[Player[num].evobjno].actno );
}

//------------------------------------------
//	��ڲ԰�̍��W�擾
//	In:		num			; ���@ܰ�����
//			x,y			; ���W�i�[
//------------------------------------------
void GetPlayerPos( u8 num, s16 *x, s16 *y )
{
	EV_PLAYER *ev;
	
	ev = (EV_PLAYER*)&EvObj[Player[num].evobjno];

	*x = ev->gx;
	*y = ev->gy;
}

//------------------------------------------
//	��ڲ԰�̕����擾
//	In:		num			; ���@ܰ�����
//	Out:	u8			; ���@����
//------------------------------------------
u8 GetPlayerSite( u8 num )
{
	return( ((EV_PLAYER*)EvObj)[Player[num].evobjno].mv_site );
}

//------------------------------------------
//	��ڲ԰�̍����擾
//	In:		num			; ���@ܰ�����
//	Out:	u8			; ���@����(0~15)
//------------------------------------------
u8 GetPlayerHeight( u8 num )
{
	return( ((EV_PLAYER*)EvObj)[Player[num].evobjno].height );
}

//------------------------------------------
//	��ڲ԰���ޯĒP�ʂ̈ړ��ʂ��擾
//	In:		num			; ���@ܰ�����
//	Out:	s16			; ���݂̈ړ���(0~16)
//------------------------------------------
s16 GetPlayerMoveAmount( u8 num )
{
	s16 ret;

	ret = 16 - (s8)((EV_PLAYER*)EvObj)[Player[num].evobjno].work0;
	return( ret );
}

//------------------------------------------
//	��ڲ԰�ւ̘b����������
//	In:		x,y		; �b���|��ϯ�ߍ��W�ʒu
//	Out:	u8		; ��ڲ԰ܰ�����
//					; COM_PLAYER_MAX = ��ڲ԰�͂��Ȃ�
//------------------------------------------
u8 CheckTalkPlayer( s16 x, s16 y )
{
	u8 i;
	EV_PLAYER *ev;
	PLAYER_STATUS * play;

	for( i = 0; i < COMM_PLAYER_MAX; i++ )
	{
		play = &Player[i];

		if( play->sw &&
			(play->mv_status == EV_M_OFF || play->mv_status == EV_M_END) )
		{
			ev = &((EV_PLAYER *)EvObj)[play->evobjno];

			if( ev->gx == x && ev->gy == y )
			{
				return( i );
			}

		}
	}

	return( COMM_PLAYER_MAX );
}

//======================================================================
//		��ڲ԰����
//======================================================================
static u8 check_player_status_off( PLAYER_STATUS *, EV_PLAYER *, u8 );
static u8 check_player_status_on( PLAYER_STATUS *, EV_PLAYER *, u8 );
static u8 check_player_status_end( PLAYER_STATUS *, EV_PLAYER *, u8 );
static u8 set_move_player_stop( PLAYER_STATUS *, EV_PLAYER *, u8 );
static u8 set_move_player_walk( PLAYER_STATUS *, EV_PLAYER *, u8 );
static u8 set_move_player_site( PLAYER_STATUS *, EV_PLAYER *, u8 );
static void move_player_stop( PLAYER_STATUS *, EV_PLAYER * );
static void move_player_walk( PLAYER_STATUS *, EV_PLAYER * );
static u8 cmd_site( u8, u8 );
static u8 check_player_move_pos(u8,u8,s16,s16);

//------------------------------------------
//	����ð���
//------------------------------------------
static u8 (* const CheckPlayerStatus[])( PLAYER_STATUS *, EV_PLAYER *, u8 ) =
{
	check_player_status_off,
	check_player_status_on,
	check_player_status_end,
};

//------------------------------------------
//	���ð���
//------------------------------------------
static u8 (* const SetMovePlayer[])( PLAYER_STATUS *, EV_PLAYER *, u8 ) =
{
	set_move_player_stop,
	set_move_player_walk,
	set_move_player_walk,
	set_move_player_walk,
	set_move_player_walk,
	set_move_player_stop,
	set_move_player_stop,
	set_move_player_site,
	set_move_player_site,
	set_move_player_site,
	set_move_player_site,
};

//------------------------------------------
//	����ð���
//------------------------------------------
static void (* const MovePlayer[])( PLAYER_STATUS *, EV_PLAYER * ) =
{
	move_player_stop,
	move_player_walk,
};

enum
{
	MOVE_PLAYER_STOP = 0,
	MOVE_PLAYER_WALK,
};

//------------------------------------------
//	��ڲ԰����
//	In:		num			; ���@���ܰ�����
//			cmd			; ��������
//------------------------------------------
void UpdatePlayer( u8 num, u8 cmd )
{
	EV_PLAYER *ev;
	PLAYER_STATUS *play;

	play = &Player[num];
	ev = (EV_PLAYER*)&EvObj[play->evobjno];

	if( play->sw == 0 )
	{
		return;
	}
	
	if( cmd >= HCMD_END )								// ���ޯ��p
	{
		ev->debug = 1;
		return;
	}

	MovePlayer[CheckPlayerStatus[play->mv_status](play,ev,cmd)]( play, ev );
}

//------------------------------------------
//	�����ð�� OFF
//------------------------------------------
static u8 check_player_status_off( PLAYER_STATUS *play, EV_PLAYER *ev, u8 cmd )
{
	return( SetMovePlayer[cmd](play,ev,cmd) );
}

//------------------------------------------
//	�����ð�� ON
//------------------------------------------
static u8 check_player_status_on( PLAYER_STATUS *play, EV_PLAYER *ev, u8 cmd )
{
	return( MOVE_PLAYER_WALK );
}

//------------------------------------------
//	�����ð�� END
//------------------------------------------
static u8 check_player_status_end( PLAYER_STATUS *play, EV_PLAYER *ev, u8 cmd )
{
	return( SetMovePlayer[cmd](play,ev,cmd) );
}

//------------------------------------------
//	��� ��~
//------------------------------------------
static u8 set_move_player_stop( PLAYER_STATUS *play, EV_PLAYER *ev, u8 cmd )
{
	return( MOVE_PLAYER_STOP );
}

//------------------------------------------
//	��� �ړ�
//------------------------------------------
static u8 set_move_player_walk( PLAYER_STATUS *play, EV_PLAYER *ev, u8 cmd )
{
	s16 x,y;

	ev->mv_site = cmd_site( cmd, ev->mv_site );

	EvObjSitePosMoveWork( (EventOBJ*)ev, ev->mv_site, &x, &y );

	if( check_player_move_pos(play->evobjno,ev->mv_site,x,y) )
	{
		return( MOVE_PLAYER_STOP );
	}
	
	ev->work0 = 16;
	
	EvObjPosChg( (EventOBJ*)ev, x, y );
	EvObjHeight( (EventOBJ*)ev );

	return( MOVE_PLAYER_WALK );
}

//------------------------------------------
//	��� �U�����
//------------------------------------------
static u8 set_move_player_site( PLAYER_STATUS *play, EV_PLAYER *ev, u8 cmd )
{
	ev->mv_site = cmd_site( cmd, ev->mv_site );
	
	return( MOVE_PLAYER_STOP );
}

//------------------------------------------
//	��~
//------------------------------------------
static void move_player_stop( PLAYER_STATUS *play, EV_PLAYER *ev )
{
	play->mv_status = EV_M_OFF;
}

//------------------------------------------
//	�ړ�
//------------------------------------------
static void move_player_walk( PLAYER_STATUS *play, EV_PLAYER *ev )
{
	ev->work0--;
	play->mv_status = EV_M_ON;

	SitePosMove( ev->mv_site, &ev->dx, &ev->dy );

	if( ev->work0 == 0 )
	{
		EvObjPosRenew( (EventOBJ*)ev );
		play->mv_status = EV_M_END;
	}
}

//------------------------------------------
//	����� -> ����
//------------------------------------------
static u8 cmd_site( u8 cmd, u8 defsite )
{
	switch( cmd )
	{
		case HCMD_UP:
		case HCMD_UP_SITE:
			return( SITE_U );
		case HCMD_DOWN:
		case HCMD_DOWN_SITE:
			return( SITE_D );
		case HCMD_LEFT:
		case HCMD_LEFT_SITE:
			return( SITE_L );
		case HCMD_RIGHT:
		case HCMD_RIGHT_SITE:
			return( SITE_R );
	}
	
	return( defsite );
}

//------------------------------------------
//	�ړ�������
//------------------------------------------
static u8 check_player_move_pos( u8 evno, u8 site, s16 mx, s16 my )
{
	u8 i;

	for( i = 0; i < EV_OBJ_MAX; i++ )
	{
		if( i != evno &&
			((EvObj[i].gx == mx && EvObj[i].gy == my) ||
			(EvObj[i].old_gx == mx && EvObj[i].old_gy == my)) )	
		{
			return( 1 );
		}
	}
	
	return( GetCellHitCheck(mx,my) );
}

//======================================================================
//		��ڲ԰��������
//======================================================================
static void PlayerActorMove( actWork * );

//------------------------------------------
//	�����o�^
//	In:		num			; ���@ܰ�����
//------------------------------------------
void SetPlayerActor( u8 num )
{
	actWork *act;
	EV_PLAYER *ev;
	PLAYER_STATUS *play;

	play = &Player[num];
	ev = (EV_PLAYER*)&EvObj[play->evobjno];

	if( play->sw == 0 )
	{
		return;
	}

	ev->actno = EvObjMakeAddAct(
		GetHeroSexObjCode(H_NORMAL,ev->sex), PlayerActorMove, 0, 0, 0 );

	act = &ActWork[ev->actno];
	act->scr = 1;
	act->work[0] = num;
	
	ev->debug = 0;
}

//------------------------------------------
//	��������
//------------------------------------------
static void PlayerActorMove( actWork *act )
{
	EV_PLAYER *ev;
	PLAYER_STATUS *play;

	play = &Player[act->work[0]];
	ev = (EV_PLAYER*)&EvObj[play->evobjno];

	act->x = ev->dx;
	act->y = ev->dy;

	EvActPriControl( ev->height_pri, act, 1 );
	act->oamData.Priority = EvObjHeightBgPri( ev->height_pri );

	if( play->mv_status == EV_M_OFF )
	{
		ActAnmChg( act, EvActAnmNoStop(ev->mv_site) );
	}
	else
	{
		ActAnmChkChg( act, EvActAnmNoWalk1(ev->mv_site) );
	}

	EvActScrollVanish( act, 0 );

	if( ev->debug )
	{
		act->banish = (act->work[7] & 0x04) >> 2;
		act->work[7]++;
	}
}

