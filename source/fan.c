//====================================================================
//
//							�t�@�����		02/03/12 nohara							
//
//====================================================================
#include "common.h"
#include "intr.h"
#include "script.h"					//ScriptParameter
#include "calctool.h"				//pp_rand
#include "poketool.h"				//PokeNameData
#include "print.h"					//PM_strcpy
#include "syswork.h"				//EV_FAN_WORK
#include "server.h"					//WinLoseFlag
#include "score.h"					//SCORE_TABLE
#include "fan.h"					//FAN_YOUNGM1
#include "message.h"				//���b�Z�[�W�f�[�^
#include "..\script\saveflag.h"		//FV_FAN_01_C106_R0401
#include "..\script\savework.h"		//WK_SCENE_C106_R0401


/*
typedef struct{
	u8	point:7;
	u8  sw:1;
	u8	char_bit;
	u16 time;
} FAN_DATA;			//4byte
*/


//====================================================================
//							�ϐ�
//====================================================================
//�t�@���|�C���g�l
static const u8 fan_point_data[FAN_POINT_DATA_MAX] = {2,1,2,1};

//�������Ƃ��̃t�@���ɂȂ�₷����(�オ����)
const u8 fan_obj_add[FAN_MAX] = {
	FAN_SCHOOLW1,
	FAN_MIDDLEM2,
	FAN_OJYOU2,
	FAN_BOYA1,
	FAN_BOY2,
	FAN_YOUNGM1,
	FAN_WORKERW1,
	FAN_OLDWOMAN1,
};

//�������Ƃ��̂����₷����(�オ����)
const u8 fan_obj_sub[FAN_MAX] = {
	FAN_SCHOOLW1,
	FAN_YOUNGM1,
	FAN_WORKERW1,
	FAN_BOYA1,
	FAN_OJYOU2,
	FAN_BOY2,
	FAN_OLDWOMAN1,
	FAN_MIDDLEM2,
};


//====================================================================
//						�v���g�^�C�v�錾
//====================================================================
void FanAllInit();
void FanCommBattleTimeSet();
void FanStart();
u8 FanPointCalc(u8);
u8 FanBitOn();
u8 FanBitOff();
void FanBitOnFirst();
u16 FanNoGet();
void FanTimeChk();
u16 ScrFanBitChk();
void ScrFanNameSet();
static void FanScoreNameSet(SCORE_TABLE *,u8,u8);
void FanCommBattleWinLoseChk();
u8 FanStartFlagChk();
void FanStartFlagOn();
u8 ScrFanPointAdd();


//--------------------------------------------------------------------
//							���ׂď�����
//--------------------------------------------------------------------
void FanAllInit()
{
	FLD(FAN_WK) = 0;
	FLD(FAN_TM) = 0;
	return;
}

//--------------------------------------------------------------------
//				���ԏ��������āA�ʐM�ΐ킵�����Ԃ�ۑ�����
//--------------------------------------------------------------------
void FanCommBattleTimeSet()
{
	//add 02/08/24
	//�t�@����悪�X�^�[�g���ĂȂ������牽�����Ȃ�
	if( FanStartFlagChk() == FALSE )	return;

	//add 02/08/24
	//�ʐM�ΐ��������ɌĂ΂��
	//�n�߂Ɍo�ߎ��Ԃ����ăt�@�������炷
	//���ɍ��̃v���C���Ԃ��Z�b�g����
	FanTimeChk();
	FLD(FAN_TM) = MyData.playtime_h;
	return;
}

//--------------------------------------------------------------------
//				�l�V����|�����̂ŁA�t���O���I���ɂ���
//				�ŏ��R�l���t�@���ɂ�
//--------------------------------------------------------------------
void FanStart()
{
	if(  ((FLD(FAN_WK) >> FAN_START_FLAG_BIT) & 0x01) == 0 )
	{
		FanStartFlagOn();						//add 02/08/24
		FanBitOnFirst();
		FLD(FAN_TM) = MyData.playtime_h;		//add 02/08/24

		EventFlagReset( FV_FAN_01_C106_R0401 );
		EventFlagReset( FV_FAN_02_C106_R0401 );
		EventFlagReset( FV_FAN_03_C106_R0401 );
		EventFlagReset( FV_FAN_04_C106_R0401 );

		SetEventWorkValue( WK_SCENE_C106_R0401, 1 );
	}

	return;
}

//--------------------------------------------------------------------
//					�t�@���|�C���g���v�Z����
//
//	����	�R���e�X�g�A�l�V����Ȃ�
//	�߂�l	�t�@���|�C���g
//--------------------------------------------------------------------
u8 FanPointCalc(u8 type)
{
	//�t�@����悪�X�^�[�g���Ă��邩
	//if( FanStartFlagChk() == FALSE )	return FAN_POINT;
	
	//add 02/09/16
	//�t�@�����o�}���Ă����A�j��������܂ł͉������Ȃ�
	if( GetEventWorkValue( WK_SCENE_C106_R0401 ) != 2 ) return FAN_POINT;

	if(  ( FAN_POINT + fan_point_data[type] ) >= FAN_POINT_MAX )
	{
		if( FanNoGet() < FAN_POINT_OBJ_ADD_MAX )
		{
			FanBitOn();	//�t�@���r�b�g�𗧂Ă�
			FLD(FAN_WK) &= (0xff80);		//�t�@���|�C���g�N���A
		}else
		{
			FLD(FAN_WK) &= (0xff80);		//�t�@���|�C���g�N���A
			FLD(FAN_WK) |= FAN_POINT_MAX;	//�t�@���|�C���gMAX�ɂ���
		}
	}else
	{
		FLD(FAN_WK) += (fan_point_data[type] << FAN_POINT_BIT);
	}

	return FAN_POINT;
}

//--------------------------------------------------------------------
//					�t�@���̐l���𑝂₷
//--------------------------------------------------------------------
u8 FanBitOn()
{
	u8 i,fan_no=0;
	u16 rnd;

	for( i=0; i < FAN_MAX ; i++ )
	{
		if( ((FLD(FAN_WK) >> fan_obj_add[i]) & 0x01) == 0 )
		{
			fan_no = i;				//�ۑ�
			rnd=pp_rand();
			if( (rnd % 2) )
			{
				FLD(FAN_WK) |= (1 << fan_obj_add[i]);
				return i;
			}
		}
	}

	//fan_no�ɓ����Ă���r�b�g���I���ɂ���
	FLD(FAN_WK) |= (1 << fan_obj_add[fan_no]);

	return fan_no;
}

//--------------------------------------------------------------------
//					�t�@���̐l�������炷
//					(�O�l�ɂ͂Ȃ�Ȃ�)
//--------------------------------------------------------------------
u8 FanBitOff()
{
	u8 i,fan_no=0;
	u16 rnd;

	if( FanNoGet() == 1 ) return fan_no;					//�t�@������l�̎��͏I��

	for( i=0; i < FAN_MAX ; i++ )
	{
		if( ((FLD(FAN_WK) >> fan_obj_sub[i]) & 0x01) == 1 )
		{
			fan_no = i;				//�ۑ�
			rnd=pp_rand();
			if( (rnd % 2) )
			{
				FLD(FAN_WK) ^= (1 << fan_obj_sub[i]);		//�t�@���̐l�������炷
				return i;
			}
		}
	}

	//fan_no�ɓ����Ă���r�b�g���I�t�ɂ���
	if( ((FLD(FAN_WK) >> fan_obj_sub[fan_no]) & 0x01) == 1 )
		FLD(FAN_WK) ^= (1 << fan_obj_sub[fan_no]);

	return fan_no;
}

//--------------------------------------------------------------------
//					�t�@���̐l�����擾����
//--------------------------------------------------------------------
u16 FanNoGet()
{
	u8 i,count=0;

	for( i=0; i < FAN_MAX ; i++ )
	{
		if( ((FLD(FAN_WK) >> (FAN_BIT+i)) & 0x01) == 1 )	count++;
	}

	return count;
}

//--------------------------------------------------------------------
//�O��ʐM�ΐ킵�����Ԃ���́A�o�ߎ���(�v���C����)�����ăt�@�������炷
//--------------------------------------------------------------------
void FanTimeChk()
{
	u8 count=0;

	//�ő�999����
	if( MyData.playtime_h >= 999 ) return;	//���炳�Ȃ�

	while(1)
	{
		//add 02/08/24
		//���Ԃł̌��炷�����ɂS�l�ȉ��ɂ͖�����ǉ�
		if( FanNoGet() <= FAN_TIME_OBJ_SUB_MAX ) 
		{
			FLD(FAN_TM) = MyData.playtime_h;
			break;
		}

		if( count == FAN_MAX ) break;//���Ԃ��������o���Ă�����̑Ώ�

		if( (MyData.playtime_h - FLD(FAN_TM)) >= FAN_TIME_OVER )
		{
			FanBitOff();
			FLD(FAN_TM) += FAN_TIME_OVER;
			count++;
		}else
		{
			break;
		}
	}

	return;
}

//--------------------------------------------------------------------
//					�����̃t�@�����`�F�b�N
//--------------------------------------------------------------------
u16 ScrFanBitChk()
{
	return ((FLD(FAN_WK) >> ScriptParameter0) & 0x01);
}

//--------------------------------------------------------------------
//					�f�t�H���g�̃t�@�����Z�b�g����
//--------------------------------------------------------------------
void FanBitOnFirst()
{
	FLD(FAN_WK) |= (1 << FAN_YOUNGM1);		//�t�@���̐l���𑝂₷
	FLD(FAN_WK) |= (1 << FAN_SCHOOLW1);		//�t�@���̐l���𑝂₷
	FLD(FAN_WK) |= (1 << FAN_OJYOU2);		//�t�@���̐l���𑝂₷
	return;
}

//�ΐ퐬�ѕ\�Ƀf�[�^���Ȃ��Ƃ��̃��[�_�[�̖��O
enum{
	FAN_MSG_MIKURI = 0,
	FAN_MSG_DAIGO,
	FAN_MSG_TOUKI,
	FAN_MSG_NAGI,
	FAN_MSG_HUYOU,
	FAN_MSG_PURIMU,
};

//--------------------------------------------------------------------
//					�ΐ�\�̉��Ԗڂ̃t�@�������擾
//--------------------------------------------------------------------
void ScrFanNameSet()
{
	u8 no = 0;
	u8 leader = FAN_MSG_MIKURI;

	switch( ScriptParameter0 )
	{
		case FAN_SCHOOLW1:
			break;
		case FAN_MIDDLEM2:
			break;
		case FAN_OJYOU2:
			no = 0;
			leader = FAN_MSG_NAGI;
			break;
		case FAN_BOYA1:
			no = 0;
			leader = FAN_MSG_DAIGO;
			break;
		case FAN_BOY2:
			no = 1;
			leader = FAN_MSG_MIKURI;
			break;
		case FAN_YOUNGM1:
			no = 0;
			leader = FAN_MSG_HUYOU;
			break;
		case FAN_WORKERW1:
			no = 1;
			leader = FAN_MSG_PURIMU;
			break;
		case FAN_OLDWOMAN1:
			break;
	}

	FanScoreNameSet( &Fld.TaisenScoreTable, no, leader );
	return;
}

//--------------------------------------------------------------------
//					�ΐ퐬�ѕ\���疼�O���Z�b�g����
//
//	�ΐ퐬�ѕ\�ɉ����f�[�^�������Ă��Ȃ�������u�~�N���v��\������
//--------------------------------------------------------------------
static void FanScoreNameSet( SCORE_TABLE *tbl, u8 no, u8 leader )
{
	if( tbl->score[no].name[0] == EOM_ )
	{
		switch( leader )
		{
			case FAN_MSG_MIKURI:
				PM_strcpy( StrTempBuffer0, fan_leader_name01 );
				break;
			case FAN_MSG_DAIGO:
				PM_strcpy( StrTempBuffer0, fan_leader_name02 );
				break;
			case FAN_MSG_TOUKI:
				PM_strcpy( StrTempBuffer0, fan_leader_name03 );
				break;
			case FAN_MSG_NAGI:
				PM_strcpy( StrTempBuffer0, fan_leader_name04 );
				break;
			case FAN_MSG_HUYOU:
				PM_strcpy( StrTempBuffer0, fan_leader_name05 );
				break;
			case FAN_MSG_PURIMU:
				PM_strcpy( StrTempBuffer0, fan_leader_name06 );
				break;
			default:
				PM_strcpy( StrTempBuffer0, fan_leader_name01 );
				break;
		}
	}else
	{
		PM_strncpy( StrTempBuffer0, tbl->score[no].name, PERSON_NAME_SIZE );
		StrTempBuffer0[PERSON_NAME_SIZE] = EOM_;
	}

	return;
}

//--------------------------------------------------------------------
//				�ʐM�ΐ��A���s�����ăt�@���̑���������
//--------------------------------------------------------------------
void FanCommBattleWinLoseChk()
{
	//�t�@�����o�}���Ă����A�j��������܂ł͉������Ȃ�
	if( GetEventWorkValue( WK_SCENE_C106_R0401 ) != 2 ) return;

	//add 02/08/24
	//���ԏ��������āA�ʐM�ΐ킵�����Ԃ�ۑ�����
	FanCommBattleTimeSet();

	if( WinLoseFlag == FIGHT_WIN )	FanBitOn();
	else	FanBitOff();

	return;
}

//--------------------------------------------------------------------
//					�t�@����悪�X�^�[�g���Ă��邩
//
//	�߂�l	0	�܂��X�^�[�g���ĂȂ�
//			1	�X�^�[�g���Ă���
//--------------------------------------------------------------------
u8 FanStartFlagChk()
{
	return ((FLD(FAN_WK) >> FAN_START_FLAG_BIT) & 0x01);
}

//--------------------------------------------------------------------
//					�t�@�������X�^�[�g������
//--------------------------------------------------------------------
void FanStartFlagOn()
{
	FLD(FAN_WK) |= FAN_START_FLAG_ON;
}

//--------------------------------------------------------------------
//					�t�@���|�C���g�����Ăяo��
//
//					ScriptParameter0�Ƀ^�C�v������
//--------------------------------------------------------------------
u8 ScrFanPointAdd()
{
	return FanPointCalc((u8)ScriptParameter0);
}


