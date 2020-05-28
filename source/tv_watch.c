//====================================================================
//
//						�s�u���	02/03/28 nohara							
//
//						��������
//
//
//
/*
						���f�[�^�^�C�v��ǉ������Ƃ���
						�ETVBroadCastSelect()�ɒǉ�


*/
//====================================================================
#include "common.h"
#include "intr.h"
#include "menu.h"					//menutool.h
#include "calctool.h"				//pp_rand
#include "script.h"					//ScriptParameter
#include "fld_talk.h"				//FieldMsgSet
#include "poketool.h"				//PokeNameData
#include "server.h"					//FIGHT_WIN
#include "item.h"					//ITEM_MASTERBALL
#include "kaiwa.h"					//KaiwaCopyWord
#include "tv_def.h"					//TV��define�Q
#include "tv_tool.h"				//�֐��Q
#include "tv.h"						//TVCameraWatchFlagReset
#include "letter.h"					//GeneratingEventSet
#include "record.h"					//CNT_TOWER_RENSHOU
#include "b_tower.def"				//BATTLE_TOWER_WIN


//====================================================================
//						  ���b�Z�[�W�f�[�^
//====================================================================
#include "tv_msg.dat"


//====================================================================
//							  �O���ϐ�
//====================================================================
static u8	tv_msgno = 0;


//====================================================================
//							 extern�錾 
//====================================================================
extern u8* CopyMapName(u8 *buf, u16 div, u16 size);			//wmap.c


//====================================================================
//							�v���g�^�C�v�錾
//====================================================================
void TVBroadCastSelect();					//TV����
static void TVBroadCastEnd();				//�����I�����ʏ���
void TVBroadCastInit();						//�����O�̏���������

//�莆�^
static void LetterPokeBroadCastSet();		//�|�P�����A���P�[�g(�J�C�i)
static void LetterRecentlyBroadCastSet();	//�ŋ߂������o����(�J�C�i)
static void LetterClubBroadCastSet();		//��D���N���u(�J�C�i)
static void LetterGymBroadCastSet();		//�ۗ�
static void LetterNameBroadCastSet();		//�������f�t(�J�C�i)
void LetterGeneratingBroadCastSet();		//��ʔ���
static void LetterConBroadCastSet();		//�R���e�X�g
static void LetterTowerBroadCastSet();		//�o�g���^���[

//�閧�^
static void SecBtlBroadCastSet();			//�߂܂����|�P�����̏Љ�
static void SecShopBroadCastSet();			//���������
static void SecGetFailedBroadCastSet();		//�ߊl���s
static void SecFishingBroadCastSet();		//�ނ肵�܂���
static void SecOneDayBroadCastSet();		//����ɕ߂܂�����

//�C���^�r���A�[���J�����}��
void TVCameraBroadCastSet();


//====================================================================
//							�v���O����
//====================================================================

//--------------------------------------------------------------------
//							�s�u����
//
//			ScriptParameter0�̏ꏊ��datatype�����ď����𕪂���
//--------------------------------------------------------------------
void TVBroadCastSelect()
{
	if( Fld.tv_event[ScriptParameter0].watch_flag == TV_WATCH_FLAG_OFF ) return;

	switch( Fld.tv_event[ScriptParameter0].datatype )
	{
		case TV_POKEMON_TYPE1:
			LetterPokeBroadCastSet();
			break;
		case TV_RECENTLY_TYPE1:
			LetterRecentlyBroadCastSet();
			break;
		case TV_CLUB_TYPE1:
			LetterClubBroadCastSet();
			break;
		case TV_GYMLEADER_TYPE1:
			LetterGymBroadCastSet();
			break;
		case TV_GENERATING_TYPE1:
			LetterGeneratingBroadCastSet();
			break;
		case TV_CONTEST_TYPE1:
			LetterConBroadCastSet();
			break;
		case TV_TOWER_TYPE1:
			LetterTowerBroadCastSet();
			break;
		case TV_GETPOKE_TYPE1:
			SecBtlBroadCastSet();
			break;
		case TV_SHOP_TYPE1:
			SecShopBroadCastSet();
			break;
		case TV_NAME_TYPE1:
			LetterNameBroadCastSet();
			break;
		case TV_FAILED_TYPE1:
			SecGetFailedBroadCastSet();
			break;
		case TV_FISHING_TYPE1:
			SecFishingBroadCastSet();
			break;
		case TV_ONEDAY_TYPE1:
			SecOneDayBroadCastSet();
			break;
	};
}

//--------------------------------------------------------------------
//						�莆�^�R���e�X�g����
//
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void LetterConBroadCastSet()
{
	u8 no;
	TV_Con_Work *p;

	p = (TV_Con_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;

	switch( tv_msgno )
	{
		case 0:
			//buf=0 ��l���̖��O
			//buf=1 type
			//buf=2	rank
			PM_strcpy( StrTempBuffer0, p->player_name );
			TVContestTypeGet(1,p->type);
			TVContestRankGet(2,p->rank);

			//�j�b�N�l�[�������Ă��邩
			if( PM_strcmp( (u8 *)PokeNameData[p->pokeno], (u8 *)p->nickname ) == 0 )	
			{
				tv_msgno = 8;		//���ĂȂ�
			}else
			{
				tv_msgno = 1;		//���Ă�
			}

			break;
		case 1:
			//buf=0	�|�P�����̎��
			//buf=1	�j�b�N�l�[��
			//buf=2 type
			PM_strcpy( StrTempBuffer0, PokeNameData[p->pokeno] );
			NickNameCopy( StrTempBuffer1, p->nickname );
			TVContestTypeGet(2,p->type);
			tv_msgno = 2;
			break;
		case 2:
			//buf=0 ��l���̖��O
			PM_strcpy( StrTempBuffer0, p->player_name );

			if( p->zyuni == 0 )	tv_msgno = 3;	//�P��
			else	tv_msgno = 4;				//�Q�ʈȉ�
			break;
		case 3:
			//buf=0 ��l���̖��O
			//buf=1	�ꌾ���̂P
			//buf=2	�R���e�X�g�̏���
			PM_strcpy( StrTempBuffer0, p->player_name );
			KaiwaCopyWord( StrTempBuffer1, p->easytalk_no[0] );
			SetNumSelectBuf( 2, (int)(p->zyuni+1) );
			tv_msgno = 5;
			break;
		case 4:
			//buf=0 ��l���̖��O
			//buf=1	�ꌾ���̂P
			//buf=2	�R���e�X�g�̏���
			PM_strcpy( StrTempBuffer0, p->player_name );
			KaiwaCopyWord( StrTempBuffer1, p->easytalk_no[0] );
			SetNumSelectBuf( 2, (int)(p->zyuni+1) );
			tv_msgno = 5;
			break;
		case 5:
			//buf=0 ��l���̖��O
			//buf=1	type
			//buf=2	�ꌾ���̂Q
			PM_strcpy( StrTempBuffer0, p->player_name );
			TVContestTypeGet(1,p->type);
			KaiwaCopyWord( StrTempBuffer2, p->easytalk_no[1] );

			if( p->waza != 0 )	tv_msgno = 6;
			else	tv_msgno = 7;	//�����ȋZ
			break;
		case 6:
			//buf=0 �|�P�����̎��
			//buf=1 �Ō�Ɏg�����Z
			//buf=2	�ꌾ���̂Q
			PM_strcpy( StrTempBuffer0, PokeNameData[p->pokeno] );
			PM_strcpy( StrTempBuffer1, WazaNameData[p->waza] );
			KaiwaCopyWord( StrTempBuffer2, p->easytalk_no[1] );
			tv_msgno = 7;
			break;
		case 7:
			//buf=0	��l���̖��O
			//buf=1 �|�P�����̎��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );
			TVBroadCastEnd();
			break;
		case 8:
			//buf=0 �|�P�����̎��
			PM_strcpy( StrTempBuffer0, PokeNameData[p->pokeno] );
			tv_msgno = 2;
			break;
	};

	FieldMsgSet(msg_letter_contest_buf[no]);
	return;
}

//--------------------------------------------------------------------
//						�莆�^�o�g���^���[����
//
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void LetterTowerBroadCastSet()
{
	u8 no;
	TV_Tower_Work *p;

	p = (TV_Tower_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;

	switch( tv_msgno )
	{
		case 0:
			//buf=0 ��l���̖��O
			//buf=1	�莝���|�P����
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );

			if(  p->win >= 7 ) tv_msgno = 1;
			else	tv_msgno = 2;
			break;
		case 1:
			//buf=0	���x��
			//buf=1	�A����
			SetNumSelectBuf( 0, (int)p->level );
			SetNumSelectBuf( 1, (int)p->win );

			if( p->win_flag == FIGHT_WIN ) tv_msgno = 3;
			else	tv_msgno = 4;
			break;
		case 2:
			//buf=0 �Ō�̃g���[�i�[�̖��O
			//buf=1	�A����
			PM_strcpy( StrTempBuffer0, p->enemy_name );
			SetNumSelectBuf( 1, (int)(p->win + 1) );

			//�܂񂼂��E�ӂ܂�I��
			if( p->ans == 0 )	tv_msgno = 5;		//�܂񂼂�
			else	tv_msgno = 6;					//�ӂ܂�
			break;
		case 3:
			//buf=0 �Ō�̃g���[�i�[�̖��O
			//buf=1	�Ō�̃|�P����
			PM_strcpy( StrTempBuffer0, p->enemy_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->enemy_pokeno] );

			//�܂񂼂��E�ӂ܂�I��
			if( p->ans == 0 )	tv_msgno = 5;		//�܂񂼂�
			else	tv_msgno = 6;					//�ӂ܂�
			break;
		case 4:
			//buf=0 �Ō�̃g���[�i�[�̖��O
			//buf=1	�Ō�̃|�P����
			PM_strcpy( StrTempBuffer0, p->enemy_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->enemy_pokeno] );

			//�܂񂼂��E�ӂ܂�I��
			if( p->ans == 0 )	tv_msgno = 5;		//�܂񂼂�
			else	tv_msgno = 6;					//�ӂ܂�
			break;
		case 5:
			//buf=0 �Ō�̃g���[�i�[�̖��O
			PM_strcpy( StrTempBuffer0, p->enemy_name );

			tv_msgno = 11;
			break;
		case 6:
			//buf=0 �Ō�̃g���[�i�[�̖��O
			PM_strcpy( StrTempBuffer0, p->enemy_name );

			tv_msgno = 11;
			break;
		case 7:
			tv_msgno = 11;
			break;
		case 8:
			//buf=0 ��l���̖��O
			PM_strcpy( StrTempBuffer0, p->player_name );

			tv_msgno = 11;
			break;
		case 9:
			//buf=0 ��l���̖��O
			PM_strcpy( StrTempBuffer0, p->player_name );

			tv_msgno = 11;
			break;
		case 10:
			//buf=0 ��l���̖��O
			PM_strcpy( StrTempBuffer0, p->player_name );

			tv_msgno = 11;
			break;
		case 11:
			//buf=0 �ȈՕ��̂P���[�h
			KaiwaCopyWord( StrTempBuffer0, p->easytalk_no );

			//�܂񂼂��E�ӂ܂�I��
			if( p->ans == 0 )	tv_msgno = 12;		//�܂񂼂�
			else	tv_msgno = 13;					//�ӂ܂�

			break;
		case 12:
			//buf=0 �ȈՕ��̂P���[�h
			//buf=1 ��l���̖��O
			//buf=2 �Ō�̃g���[�i�[�̖��O
			KaiwaCopyWord( StrTempBuffer0, p->easytalk_no );
			PM_strcpy( StrTempBuffer1, p->player_name );
			PM_strcpy( StrTempBuffer2, p->enemy_name );

			tv_msgno = 14;
			break;
		case 13:
			//buf=0 �ȈՕ��̂P���[�h
			//buf=1 ��l���̖��O
			//buf=2 �Ō�̃g���[�i�[�̖��O
			KaiwaCopyWord( StrTempBuffer0, p->easytalk_no );
			PM_strcpy( StrTempBuffer1, p->player_name );
			PM_strcpy( StrTempBuffer2, p->enemy_name );

			tv_msgno = 14;
			break;
		case 14:
			//buf=0 ��l���̖��O
			//buf=1	�莝���|�P����
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );

			TVBroadCastEnd();
			break;
	};

	FieldMsgSet(msg_letter_tower_buf[no]);
	return;
}

//--------------------------------------------------------------------
//						�閧�^������������
//
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void SecShopBroadCastSet()
{
	u8 no;
	TV_Sec_Shop_Work *p;

	p = (TV_Sec_Shop_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;


	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=1 ���̖��O
			PM_strcpy( StrTempBuffer0, p->player_name );
			CopyMapName( StrTempBuffer1, p->worldpos, 0);

			if( p->num[0] >= 255 )	tv_msgno = 11;
			else	tv_msgno = 1;
			break;
		case 1:
			//buf=0	��l���̖��O
			//buf=1 ���i���P
			//buf=2 ���i���P�̐�
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, ItemNameGet( p->itemno[0] ) );
			SetNumSelectBuf( 2, (int)p->num[0] );

			tv_msgno += (pp_rand() % 4)+1;
			break;
		case 2:
		case 4:
		case 5:
			if( p->itemno[1] != 0 )	tv_msgno = 6;
			else	tv_msgno = 10;
			break;
		case 3:
			//buf=2 ���i���P�̐�(����1�̎��̂�+1���ĕ\������)
			SetNumSelectBuf( 2, (int)(p->num[0]+1) );

			if( p->itemno[1] != 0 )	tv_msgno = 6;
			else	tv_msgno = 10;
			break;
		case 6:
			//buf=1 ���i���Q
			//buf=2 ���i���Q�̐�
			PM_strcpy( StrTempBuffer1, ItemNameGet( p->itemno[1] ) );
			SetNumSelectBuf( 2, (int)p->num[1] );

			if( p->itemno[2] != 0 )	tv_msgno = 7;
			else if( p->bargain_flag == 1 )	tv_msgno = 8;
			else	tv_msgno = 9;
			break;
		case 7:
			//buf=1 ���i���R
			//buf=2 ���i���R�̐�
			PM_strcpy( StrTempBuffer1, ItemNameGet( p->itemno[2] ) );
			SetNumSelectBuf( 2, (int)p->num[2] );

			if( p->bargain_flag == 1 )	tv_msgno = 8;
			else	tv_msgno = 9;
			break;
		case 8:
			if( p->num[0] >= 255 )	tv_msgno = 12;
			else	tv_msgno = 9;
			break;
		case 9:
			//buf=1 ���v���z
			SetTotalMoneySelectBuf( 1, p );

			TVBroadCastEnd();
			break;
		case 10:
			if( p->bargain_flag == 1 )	tv_msgno = 8;
			else	tv_msgno = 9;
			break;
		case 11:
			//buf=0	��l���̖��O
			//buf=1 ���i���P
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, ItemNameGet( p->itemno[0] ) );

			if( p->bargain_flag == 1 )	tv_msgno = 8;
			else	tv_msgno = 12;
			break;
		case 12:
			//buf=0	��l���̖��O
			PM_strcpy( StrTempBuffer0, p->player_name );

			TVBroadCastEnd();
			break;
	};

	FieldMsgSet(msg_sec_shop_buf[no]);
	return;
}

//--------------------------------------------------------------------
//						�閧�^�������f������
//
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void LetterNameBroadCastSet()
{
	u8 no;
	TV_Name_Work *p;

	p = (TV_Name_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;


	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=1	�|�P�����̎��
			//buf=2 �j�b�N�l�[��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );
			NickNameCopy( StrTempBuffer2, p->nickname );

			tv_msgno = ( GetPokeNickNameBit(p) + 1 );
			break;
		case 1:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			//tv_msgno = (pp_rand() % 3)+9;
			if( p->r_q_no[0] == 0 )			tv_msgno = 9;
			else if( p->r_q_no[0] == 1 )	tv_msgno = 10;
			else if( p->r_q_no[0] == 2 )	tv_msgno = 11;
			break;
		case 2:
			//buf=0	��l���̖��O
			PM_strcpy( StrTempBuffer0, p->player_name );

			if( p->r_q_no[0] == 0 )			tv_msgno = 9;
			else if( p->r_q_no[0] == 1 )	tv_msgno = 10;
			else if( p->r_q_no[0] == 2 )	tv_msgno = 11;
			break;
		case 9:
		case 10:
		case 11:
			//buf=0 �j�b�N�l�[��
			//buf=1	�j�b�N�l�[���P������
			//buf=2	�j�b�N�l�[���Q������
			NickNameCopy( StrTempBuffer0, p->nickname );
			SetMojiNoSelectBuf( 1, 0, TV_NAME_NTOP, TV_NAME_NICK, 0, p );
			SetMojiNoSelectBuf( 2, 1, TV_NAME_NTOP, TV_NAME_NICK, 0, p);

			tv_msgno = 12;
			break;
		case 12:
			if( p->r_q_no[1] == 0 )	tv_msgno = 13;
			else	tv_msgno = 15;
			break;
		case 13:
			//buf=0	��l���̖��O
			//buf=1	��l���Q������
			//buf=2	�j�b�N�l�[����납��Q������
			PM_strcpy( StrTempBuffer0, p->player_name );
			SetMojiNoSelectBuf( 1, 0, TV_NAME_TOP, TV_NAME_MY, 0, p );
			SetMojiNoSelectBuf( 2, 0, TV_NAME_LAST, TV_NAME_NICK, 0, p );

			tv_msgno = 14;
			break;
		case 14:
			//buf=1	�j�b�N�l�[���Q������
			//buf=2	��l����납��Q������
			SetMojiNoSelectBuf( 1, 0, TV_NAME_TOP, TV_NAME_NICK, 0, p );
			SetMojiNoSelectBuf( 2, 0, TV_NAME_LAST, TV_NAME_MY, 0, p );

			tv_msgno = 18;
			break;
		case 15:
			//buf=0	�j�b�N�l�[���Q������
			//buf=1	�|�P�����̎��
			//buf=2	�|�P�����̎�ތ�납��Q������
			SetMojiNoSelectBuf( 0, 0, TV_NAME_TOP, TV_NAME_NICK, 0, p );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );
			SetMojiNoSelectBuf( 2, 0, TV_NAME_LAST, TV_NAME_POKE, p->pokeno, p );

			tv_msgno = 16;
			break;
		case 16:
			//buf=0	�|�P�����̎�ނ���Q������
			//buf=2	�j�b�N�l�[����납��Q������
			SetMojiNoSelectBuf( 0, 0, TV_NAME_TOP, TV_NAME_POKE, p->pokeno, p );
			SetMojiNoSelectBuf( 2, 0, TV_NAME_LAST, TV_NAME_NICK, 0, p );

			tv_msgno = 17;
			break;
		case 17:
			//buf=0	�j�b�N�l�[���Q������
			//buf=1	�����_���̃|�P�����̎��
			//buf=2	���̌�납��Q������
			SetMojiNoSelectBuf( 0, 0, TV_NAME_TOP, TV_NAME_NICK, 0, p );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->r_pokeno] );
			SetMojiNoSelectBuf( 2, 0, TV_NAME_LAST, TV_NAME_POKE, p->r_pokeno, p );

			tv_msgno = 18;
			break;
		case 18:
			//buf=0 �j�b�N�l�[��
			NickNameCopy( StrTempBuffer0, p->nickname );

			TVBroadCastEnd();
			break;
	};

	FieldMsgSet(msg_name_buf[no]);
	return;
}

//--------------------------------------------------------------------
//					�閧�^�߂܂����|�P��������
//
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void SecBtlBroadCastSet()
{
	u8 no;
	TV_Sec_Btl_Work *p;

	p = (TV_Sec_Btl_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;


	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=1 �߂܂����|�P�����̎��
			//buf=2 �߂܂����|�P�����̃j�b�N�l�[��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->get_pokeno] );
			NickNameCopy( StrTempBuffer2, p->nickname );

			if( p->ball_type == ITEM_MASUTAABOORU )	tv_msgno = 5;
			else	tv_msgno = 1;
			break;
		case 1:
			tv_msgno = 2;
			break;
		case 2:
			//buf=1	�{�[���̎��
			//buf=2 �{�[���𓊂�����
			PM_strcpy( StrTempBuffer1, ItemNameGet( p->ball_type ) );
			SetNumSelectBuf( 2, (int)p->ball_count );

			if( p->ball_count <= 3 )	tv_msgno = 3;
			else	tv_msgno = 4;
			break;
		case 3:
			//buf=0	��l���̖��O
			//buf=1 �߂܂����|�P�����̎��
			//buf=2 �߂܂����|�P�����̃j�b�N�l�[��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->get_pokeno] );
			NickNameCopy( StrTempBuffer2, p->nickname );

			tv_msgno = 6;
			break;
		case 4:
			tv_msgno = 6;
			break;
		case 5:
			//buf=0	��l���̖��O
			//buf=1 �߂܂����|�P�����̎��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->get_pokeno] );

			tv_msgno = 6;
			break;
		case 6:
			//buf=0	��l���̖��O
			//buf=1 �߂܂����|�P�����̎��
			//buf=2 �߂܂����|�P�����̃j�b�N�l�[��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->get_pokeno] );
			NickNameCopy( StrTempBuffer2, p->nickname );
			
			tv_msgno += (pp_rand() % 4)+1;
			break;
		case 7:
		case 8:
			//buf=0 �߂܂����|�P�����̎��
			//buf=1 �߂܂����|�P�����̃j�b�N�l�[��
			//buf=2	�����_���̃|�P�����̎��(0�Ƃ��Ԃ�Ȃ��悤��)
			PM_strcpy( StrTempBuffer0, PokeNameData[p->get_pokeno] );
			NickNameCopy( StrTempBuffer1, p->nickname );
			SetRndChoicePokeSelectBuf( 2, p->get_pokeno );

			tv_msgno = 11;
			break;
		case 9:
		case 10:
			//buf=0 �߂܂����|�P�����̎��
			//buf=1 �߂܂����|�P�����̃j�b�N�l�[��
			PM_strcpy( StrTempBuffer0, PokeNameData[p->get_pokeno] );
			NickNameCopy( StrTempBuffer1, p->nickname );
			
			tv_msgno = 11;
			break;
		case 11:
			TVBroadCastEnd();
			break;
	};

	FieldMsgSet(msg_sec_btl_buf[no]);
	return;
}

//--------------------------------------------------------------------
//						�閧�^�ߊl���s����
//
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void SecGetFailedBroadCastSet()
{
	u8 no;
	TV_Sec_Get_Failed_Work *p;

	p = (TV_Sec_Get_Failed_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;


	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=1 �|�P�����̎��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );

			tv_msgno = 1;
			break;
		case 1:
			//buf=0	��l���̖��O
			//buf=1 ��l���������n��
			//buf=2 �ߊl���s�����|�P����
			PM_strcpy( StrTempBuffer0, p->player_name );
			CopyMapName( StrTempBuffer1, p->worldpos, 0);
			PM_strcpy( StrTempBuffer2, PokeNameData[p->failed_pokeno] );

			if( p->failed_flag == FIGHT_WIN )	tv_msgno = 3;	//�|����
			else	tv_msgno = 2;						//������ꂽ
			break;
		case 2:
		case 3:
			//buf=0	��l���̖��O
			//buf=1 �{�[���𓊂�����
			PM_strcpy( StrTempBuffer0, p->player_name );
			SetNumSelectBuf( 1, (int)p->ball_count );

			if( (pp_rand() % 3) == 0 )	tv_msgno = 5;		//1 : 3
			else	tv_msgno = 4;
			break;
		case 4:
		case 5:
			//buf=0	��l���̖��O
			PM_strcpy( StrTempBuffer0, p->player_name );
			tv_msgno = 6;
			break;
		case 6:
			TVBroadCastEnd();
			break;
	};

	FieldMsgSet(msg_sec_get_failed_buf[no]);
	return;
}

#define KAIWA_NO	50			//�ȈՉ�b��\������tv_msgno
#define KAIWA_NO2	51			//�ȈՉ�b��\������tv_msgno
//--------------------------------------------------------------------
//					�莆�^�|�P�����A���P�[�g����
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void LetterPokeBroadCastSet()
{
	u8 no;
	u16 rnd;
	TV_Letter_Work *p;

	p = (TV_Letter_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;


	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=1 ���̃|�P�����ɂ��Ă̎莆��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->inside_data] );
	
			tv_msgno = KAIWA_NO;
			break;
		case 1:

			rnd = (pp_rand() % 4)+1;
			if( rnd == 1 )	tv_msgno = 2;
			else	tv_msgno = rnd+2;		//4,5,6
			break;
		case 2:
			tv_msgno = KAIWA_NO2;
			break;
		case 3:
			tv_msgno += (pp_rand() % 3)+1;
			break;
		case 4:
		case 5:
		case 6:
			//buf=2 �ȈՕ��̒��̓K���ȂP���[�h
			GetLetterRndKaiwaWord(p);
			
			tv_msgno = 7;
			break;
		case 7:
			//buf=2 �����_���̓_�� 70-100�̊�
			rnd = (pp_rand() % 31)+70;
			SetNumSelectBuf( 2, (int)rnd );

			TVBroadCastEnd();
			break;
		case KAIWA_NO:
			KaiwaCopySentence(MsgExpandBuffer, p->easytalk, 2, 2);
			FieldMsgSet(MsgExpandBuffer);
			tv_msgno = 1;
			return;
		case KAIWA_NO2:
			KaiwaCopySentence(MsgExpandBuffer, p->easytalk, 2, 2);
			FieldMsgSet(MsgExpandBuffer);
			tv_msgno = 3;
			return;
	};

	//�ȈՉ�b�\�������邽��
	FieldMsgSet(msg_letter_poke_buf[no]);
	return;
}

//--------------------------------------------------------------------
//					�莆�^��������̂ł����ƕ���
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void LetterRecentlyBroadCastSet()
{
	u8 no;
	TV_Letter_Work *p;

	p = (TV_Letter_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;


	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=2 �ȈՕ��̒��̓K���ȂP���[�h
			PM_strcpy( StrTempBuffer0, p->player_name );
			GetLetterRndKaiwaWord(p);
			
			tv_msgno = KAIWA_NO;
			break;
		case 1:
			tv_msgno += (pp_rand() % 3)+1;
			break;
		case 2:
		case 3:
		case 4:
			tv_msgno = 5;
			break;
		case 5:
			TVBroadCastEnd();
			break;
		case KAIWA_NO:
			KaiwaCopySentence(MsgExpandBuffer, p->easytalk, 2, 2);
			FieldMsgSet(MsgExpandBuffer);
			tv_msgno = 1;
			return;
	};

	//�ȈՉ�b�\�������邽��
	FieldMsgSet(msg_letter_recently_buf[no]);
	return;
}

//--------------------------------------------------------------------
//				�莆�^��D���N���u�̃C���^�r���[����
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void LetterClubBroadCastSet()
{
	u8 no;
	TV_Club_Work *p;

	p = (TV_Club_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;
	no = tv_msgno;


	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=1 �߂܂����|�P�����̎��
			//buf=2 �߂܂����|�P�����̃j�b�N�l�[��
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );
			NickNameCopy( StrTempBuffer2, p->nickname );

			tv_msgno = (p->ans+1);
			break;
		case 1:
		case 2:
		case 3:
			//buf=0	��l���̖��O
			//buf=1 �߂܂����|�P�����̎��
			//buf=2 �ȈՉ�b��1���[�h[0]
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );
			KaiwaCopyWord( StrTempBuffer2, p->easytalk_no[0] );
			
			tv_msgno = 4;
			break;
		case 4:
			//buf=0	��l���̖��O
			//buf=2 �ȈՉ�b��1���[�h[1]
			PM_strcpy( StrTempBuffer0, p->player_name );
			KaiwaCopyWord( StrTempBuffer2, p->easytalk_no[1] );
			
			TVBroadCastEnd();
			break;
	};

	FieldMsgSet(msg_letter_club_buf[no]);
	return;
}

//--------------------------------------------------------------------
//				�莆�^�W�����[�_�[���̃C���^�r���[����
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void LetterGymBroadCastSet()
{
	return;
}

//--------------------------------------------------------------------
//						�莆�^��ʔ�������
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
void LetterGeneratingBroadCastSet()
{
	TV_Generating_Work *p;
	p = (TV_Generating_Work *)&Fld.tv_event[ScriptParameter0];

	//buf=0 ���H�ԍ�
	//buf=1	�|�P�����̎��
	CopyMapName( StrTempBuffer0, p->map_id, 0);
	PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );

	TVBroadCastEnd();

	//�f�[�^�͏������Ɏ���Ă���(�ʐM�ő���ɓn������)

	//��ʔ����C�x���g���Z�b�g����
	GeneratingEventSet();	

	FieldMsgSet(msg_letter_generating_buf[tv_msgno]);
	return;
}

//--------------------------------------------------------------------
//					�C���^�r���A�[���J�����}������
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
void TVCameraBroadCastSet()
{
	u8 no;
	AnswerWork = 0;
	no = tv_msgno;

	switch( tv_msgno )
	{
		case 0:
			//buf=0	������ꏊ
			CopyMapName( StrTempBuffer0, Fld.tv_camera.worldpos, 0);

			if( Fld.tv_camera.battle >= 2 ) tv_msgno = 1;
			else tv_msgno = 2;
			break;
		case 1:
			tv_msgno = 2;
			break;
		case 2:
			if( Fld.tv_camera.damage_flag == 0 ) tv_msgno = 4;
			else if( Fld.tv_camera.throw_flag ) tv_msgno = 5;
			else if( Fld.tv_camera.kaihuku_flag ) tv_msgno = 6;
			else if( Fld.tv_camera.kizetsu_flag ) tv_msgno = 7;
			else tv_msgno = 3;
			break;
		case 3:
			//buf=0	�g�����|�P�����̎��
			//buf=1 �Ō�Ɏg�����Z
			//buf=2	�g�����|�P�����̎�ނQ
			PM_strcpy( StrTempBuffer0, PokeNameData[Fld.tv_camera.pokeno[0]] );
			PM_strcpy( StrTempBuffer1, WazaNameData[Fld.tv_camera.waza] );
			PM_strcpy( StrTempBuffer2, PokeNameData[Fld.tv_camera.pokeno[1]] );
			tv_msgno = 8;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			tv_msgno = 8;
			break;
		case 8:
			//buf=0 �ȈՕ���1���[�h
			//buf=1	�g�����|�P�����̎��
			//buf=2	�g�����|�P�����̎�ނQ
			KaiwaCopyWord( StrTempBuffer0, Fld.tv_camera.easytalk_no );
			PM_strcpy( StrTempBuffer1, PokeNameData[Fld.tv_camera.pokeno[0]] );
			PM_strcpy( StrTempBuffer2, PokeNameData[Fld.tv_camera.pokeno[1]] );
			AnswerWork = 1;
			tv_msgno = 0;
			TVCameraWatchFlagReset();
			break;
	};

	FieldMsgSet(camera_msg_buf[no]);
	return;
}

//--------------------------------------------------------------------
//						�閧�^�ނ肵�܂������
//
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void SecFishingBroadCastSet()
{
	u8 no;
	TV_Sec_Fishing_Work *p;

	p = (TV_Sec_Fishing_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;

	if( p->true_count < p->false_count )	tv_msgno = 0;	//���s
	else	tv_msgno = 1;	//����

	no = tv_msgno;


	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=1 �|�P�����̎��
			//buf=2 �ނ��������
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );
			SetNumSelectBuf( 2, (int)p->false_count );

			TVBroadCastEnd();
			break;
		case 1:
			//buf=0	��l���̖��O
			//buf=1 �|�P�����̎��
			//buf=2 �ނ��������
			PM_strcpy( StrTempBuffer0, p->player_name );
			PM_strcpy( StrTempBuffer1, PokeNameData[p->pokeno] );
			SetNumSelectBuf( 2, (int)p->true_count );

			TVBroadCastEnd();
			break;
	};

	FieldMsgSet(msg_sec_fish_buf[no]);
	return;
}

//--------------------------------------------------------------------
//					�閧�^����ɕ߂܂���������
//
//
//	AnswerWork�ɕ����I�����������Ă���		0 = �܂�����
//												1 = �����I��
//--------------------------------------------------------------------
static void SecOneDayBroadCastSet()
{
	u8 no;
	TV_Sec_OneDay_Work *p;

	p = (TV_Sec_OneDay_Work *)&Fld.tv_event[ScriptParameter0];
	AnswerWork = 0;

	no = tv_msgno;

	switch( tv_msgno )
	{
		case 0:
			//buf=0	��l���̖��O
			//buf=1 ����
			//buf=2 �߂܂�����
			PM_strcpy( StrTempBuffer0, p->player_name );
			SetNumSelectBuf( 1, (int)p->walk_count );
			SetNumSelectBuf( 2, (int)p->count );
			tv_msgno = 1;
			break;
		case 1:
			//buf=0 �|�P�����̎��
			PM_strcpy( StrTempBuffer0, PokeNameData[p->pokeno] );
			tv_msgno = 2;
			break;
		case 2:
			//buf=0	��l���̖��O
			//buf=1 ���̖��O
			//buf=2 �߂܂����|�P�����̎��
			PM_strcpy( StrTempBuffer0, p->player_name );
			CopyMapName( StrTempBuffer1, p->worldpos, 0);
			PM_strcpy( StrTempBuffer2, PokeNameData[p->get_pokeno] );
			TVBroadCastEnd();
			break;
	};

	FieldMsgSet(msg_sec_oneday_buf[no]);
	return;
}

//--------------------------------------------------------------------
//						�����I�����ʏ���
//--------------------------------------------------------------------
static void TVBroadCastEnd()
{
	AnswerWork = 1;
	tv_msgno = 0;
	Fld.tv_event[ScriptParameter0].watch_flag = TV_WATCH_FLAG_OFF;
	return;
}

//--------------------------------------------------------------------
//						�����O�̏���������
//--------------------------------------------------------------------
void TVBroadCastInit()
{
	tv_msgno = 0;
	return;
}


