//====================================================================
//
//						�s�u���	01/10/05 nohara							
//
//====================================================================
#include "common.h"
#include "intr.h"
#include "task.h"
#include "menu.h"					//menutool.h
#include "calctool.h"				//pp_rand
#include "script.h"					//ScriptParameter
#include "sysflag.h"				//SYS_TV_START
#include "fieldmap.h"				//NowMap
#include "server.h"					//FTW
#include "tv_def.h"					//define�Q
#include "tv_tool.h"				//�֐��Q
#include "kaiwa.h"					//KaiwaCopyWord
#include "cm.h"						//CMDataAllInit
#include "tv.h"						//��`
#include "ev_time.h"				//UpdateTimeEvent
#include "mapbg.h"
#include "..\evdata\maparray.h"		//C106
#include "..\sxydata\field_r111.sym"//INTERVW1_01_FIELD_R111
#include "..\sxydata\field_r118.sym"//INTERVW1_01_FIELD_R118
#include "..\sxydata\field_r120.sym"//INTERVW1_01_FIELD_R120
#include "record.h"					//CNT_INTERVIEW
#include "..\script\saveflag.h"		//FH_00


//====================================================================
//						  �ϐ�
//====================================================================
s8 tv_akino;				//�s�u�f�[�^�̋󂢂Ă�ꏊ


//====================================================================
//						�v���g�^�C�v�錾
//====================================================================
void TVDataAllInit();						//TV�f�[�^������(�Q�[���J�n���ɌĂ΂��)
u8 ScrTVTypeGet();							//�f�[�^�^�C�v���擾
u8 TVDataWatchFlagOnChk();					//�����o����TV�f�[�^�����邩
void BroadCastFlagChk();					//TV��CM�ǂ����������邩chk
u8 GeneratingTVCellChk();					//��ʔ����̃Z�����������`�F�b�N
u8 ScrGeneratingBroadCastChk();				//��ʔ������܂����悤�Ƃ��Ă����Ƃ�
u8 TVOnceFlagChk();							//TV��������x�݂����̃`�F�b�N

//�C���^�r���A�[���J�����}��
void TVCameraAllInit();
void TVCameraBattleDataSet();
void TVCameraDataSet();
void TVCameraWatchFlagReset();
u8 TVCameraBattleGet();
u8 TVCameraBroadCastChk();
u8 TVCameraKaiwaSet();
u8 TVCameraOldFlagChk();
void TVCameraObjIDGet();


//====================================================================
//							�v���O����
//====================================================================

//--------------------------------------------------------------------
//					   	 TV�f�[�^������
//					 �Q�[���J�n���ɌĂ΂��
//--------------------------------------------------------------------
void TVDataAllInit()
{
	u8 i,j;

	//TV�f�[�^(TVDATA_MAX����Ԍ��̊m�ۗ̈���܂܂Ȃ��̂� <= �ɂ���)
	for(i=0; i <= TVDATA_MAX ;i++)
	{
		Fld.tv_event[i].datatype=TV_NODATA_TYPE1;		// 0=�f�[�^�Ȃ� 
		Fld.tv_event[i].watch_flag=TV_WATCH_FLAG_OFF;	// 0=TV����Ȃ�

		for(j=0; j < 34 ;j++)
		{
			Fld.tv_event[i].dmy[j]=0;					// 0=�f�[�^�Ȃ� 
		}
	}

	//CM�f�[�^
	CMDataAllInit();
	return;
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


//====================================================================
//							�����֘A
//====================================================================

//--------------------------------------------------------------------
//				watch_flag��������Ԃ̏ꏊ�̗v�f����Ԃ�
//				TV_GENERATINE_TYPE1�̂ݎ��Ԃ����Ă���
//
//	�߂�l		0��������		0�`TVDATA_MAX�̒l
//				0���Ȃ�����		TV_NOTHING
//--------------------------------------------------------------------
u8 TVDataWatchFlagOnChk()
{
	u8 i;
	TV_Generating_Work *p;
	u8 ans,hozon;

	//�f�[�^�������ĂȂ��ꏊ��Ԃ�
	for( i=TVDATA_HIMSELF_MAX; i < TVDATA_MAX ;i++ )
	{
		if( Fld.tv_event[i].datatype == TV_NODATA_TYPE1 ) break;
	}

	ans = (pp_rand() % i);					//0-4�ȏ�(����f�[�^�������ĂȂ����܂ł͈̔�)
	hozon = ans;

	while(1)
	{
		//CUT_TIME_TYPE�ł͂Ȃ�������
		if( TVDataCopyTypeGet( Fld.tv_event[ans].datatype ) != CUT_TIME_TYPE )
		{
			if( Fld.tv_event[ans].watch_flag == TV_WATCH_FLAG_ON ) return ans;
		}else
		{
			p = (TV_Generating_Work *)&Fld.tv_event[ans];
			if( p->time_count == 0 && p->watch_flag == TV_WATCH_FLAG_ON )
				return ans;
		}

		if( ans == 0 )	ans = (TVDATA_MAX - 1);
		else	ans--;

		//�s�u�f�[�^�����������I��
		if( ans == hozon )	
		{
			break;
		}
	};

	return TV_NOTHING;			
}

//--------------------------------------------------------------------
//TVDataWatchFlagOnChk�Ƒ�ʔ������Ă��鎞�̃`�F�b�N����������ɂ�������
//--------------------------------------------------------------------
u8 TVDataWatchFlagOnChkGeneratingChk();
u8 TVDataWatchFlagOnChkGeneratingChk()
{
	u8 ret;
	ret = TVDataWatchFlagOnChk();

	//�����o����f�[�^���Ȃ�����
	if( ret == TV_NOTHING ) return TV_NOTHING;

	//��ʔ������Ă��鎞�ɁA��ʔ�����������悤�Ƃ��Ă�����
	if( Fld.tv_encount.pokeno != 0 && Fld.tv_event[ret].datatype == TV_GENERATING_TYPE1 )
	{
		//��������v�f����ύX����
		return GeneratingTVCellChk();
	}

	return ret;
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


//====================================================================
//							BG�A�j���֘A
//====================================================================

//--------------------------------------------------------------------
//						TV�̃Z����ύX����
//--------------------------------------------------------------------
#define TV_CELL_ATTR	0x86
#define TVON_CELL_CODE	3
#define TVOFF_CELL_CODE	2

void ChangeTVCell(int,int);
void ReturnTVCell();
static void SetTVCellCode(int,int,u16);

//--------------------------------------------------------------------
//				�}�b�v�؂�ւ�����TV�̃Z��������������
//--------------------------------------------------------------------
void ChangeTVCell( int xsize, int ysize )
{
	u8 ans;

	//�s�u������Ȃ����Ă���
	EventFlagSet( SYS_TV_WATCH );
	
	//���Ԃ̌o�߂��`�F�b�N(�s�u��悪�X�^�[�g���ĂȂ��Ă��`�F�b�N����)
	//fld_tool.c�ł��̊֐����Ă΂��O��UpdateTimeEvent()���Ă΂�Ă���
	//UpdateTimeEvent();

	//��O------------------------------------------------------------
	//����TV������邩	
	ans = TVMyHomeChk();
	if( ans == TRUE )
	{
		SetTVCellCode( xsize, ysize, TVON_CELL_CODE );
		return;
	}else if( ans == 2 )
	{
		//����ł��łɃt���O�����Ă�����s�u�����点�Ȃ�
		return;
	}
	
	//�~�i���̖��h 
	if( Fld.MapID.div_id == C106 && Fld.MapID.map_id == C106_R0101 )
	{
		SetTVCellCode( xsize, ysize, TVON_CELL_CODE );
		return;
	}
	//-------------------------------------------------------------------

	//�s�u��悪�X�^�[�g���Ă��邩
	if( EventFlagCheck(SYS_TV_START) == 0 ) return;

	//������Ԃ̂s�u�����邩
	//(�s�u�̃`�F�b�N�ɑ�ʔ����̃`�F�b�N�������ɍs���悤�ɂ���)
	//if( (TVDataWatchFlagOnChk() == TV_NOTHING) 
	if( (TVDataWatchFlagOnChkGeneratingChk() == TV_NOTHING)		//add 02/09/10
			&& (CMTimeCountChk() == TV_NOTHING) 
				&& (TVCameraBroadCastChk() == TV_WATCH_FLAG_OFF) ) return;

	//�s�u�������悤�ɂ���
	EventFlagReset( SYS_TV_WATCH );

	SetTVCellCode( xsize, ysize, TVON_CELL_CODE );

	return;
}

//--------------------------------------------------------------------
//						����TV�Z���R�[�h�ɂ���
//--------------------------------------------------------------------
static void SetTVCellCode( int x, int y, u16 code )
{
	int i,j;

	//���̃}�b�v����TV�̃Z��(Atr86h)��T��
	for( i=0; i < y ;i++ )
	{
		for( j=0; j < x ;j++ )
		{
			if( GetCellAttribute(j,i) == TV_CELL_ATTR )//�w��ʒu�̾ٱ���ޭ�Ď擾
			{
				SetCell(j,i,code | CELL_HITCHECK_MASK);
			}
		}
	}

	return;
}

//--------------------------------------------------------------------
//					TV������������TV�̃Z�������ɖ߂�
//				(�����̒���TV�͈�������݂��Ȃ����̂Ƃ���)
//--------------------------------------------------------------------
void ReturnTVCell()
{
	SetTVCellCode( VMap.Xsize, VMap.Ysize, TVOFF_CELL_CODE );
	MakeMapBG();							// ��аVram �W�J
	return;
}

//--------------------------------------------------------------------
//			ScriptParameter0�̏ꏊ��TV�f�[�^�^�C�v���擾����
//--------------------------------------------------------------------
u8 ScrTVTypeGet()
{
	return Fld.tv_event[ScriptParameter0].datatype;
}

//--------------------------------------------------------------------
//	��ʔ������Ă��āA������Ԃ̂s�u����ʔ����̂s�u�f�[�^����
//	�Ȃ������ꍇ�ɂs�u�̃Z���������������Ȃ��悤�ɂ���
//
//	�߂�l	������Ԃ̂s�u�����ɂ���		�v�f��
//			������Ԃ̂s�u����ʔ����̂�	TV_NOTHING
//--------------------------------------------------------------------
u8 GeneratingTVCellChk()
{
	u8 i;

	for( i=0; i < TVDATA_MAX ;i++ )
	{
		if( Fld.tv_event[i].datatype != TV_NODATA_TYPE1 )
		{
			//��ʔ����ȊO�Ō�����Ԃ̂s�u�����邩
			if( Fld.tv_event[i].datatype != TV_GENERATING_TYPE1
				&& Fld.tv_event[i].watch_flag == TV_WATCH_FLAG_ON )
				return i;
		}
	}

	//������Ԃ̂s�u����ʔ����f�[�^�����Ȃ�
	return TV_NOTHING;
}

//--------------------------------------------------------------------
//		��ʔ�����������悤�Ƃ��āA���łɔ������Ă�����
//		�v�f����ύX����A�������͕������Ȃ�
//
//	�߂�l	������Ԃ̂s�u�����ɂ���		�v�f��
//			������Ԃ̂s�u����ʔ����̂�	TV_NOTHING
//--------------------------------------------------------------------
u8 ScrGeneratingBroadCastChk()
{
	TV_Generating_Work *p;
	p = (TV_Generating_Work *)&Fld.tv_event[ScriptParameter0];

	//��ʔ�����������悤�Ƃ��Ă�����
	if( p->datatype == TV_GENERATING_TYPE1 )
	{
		//�������Ă���
		if( Fld.tv_encount.pokeno != 0 )
		{
			//��������v�f����ύX����
			return GeneratingTVCellChk();
		}
	}

	return (u8)ScriptParameter0;
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


//====================================================================
//					�C���^�r���A�[���J�����}��
//====================================================================

//--------------------------------------------------------------------
//							������
//--------------------------------------------------------------------
void TVCameraAllInit()
{
    Fld.tv_camera.pokeno[0]			= 0;	// �����̃|�P�����̎��
    Fld.tv_camera.pokeno[1]			= 0;	// �����̃|�P�����̎��

    Fld.tv_camera.waza				= 0;	// �������Ō�Ɏg�����Z
	//Fld.tv_camera.easytalk_no		= 0;	// �퓬��̈ꌾ
	Fld.tv_camera.easytalk_no		= KAIWA_CODE_BLANK;	// �퓬��̈ꌾ

    Fld.tv_camera.damage_flag		= 0;	// �m�[�_���[�W�t���O
    Fld.tv_camera.kizetsu_flag		= 0;	// �m���t���O
    Fld.tv_camera.kaihuku_flag		= 0;	// �񕜃A�C�e���t���O
    Fld.tv_camera.throw_flag		= 0;	// �����X�^�{�[���t���O
    Fld.tv_camera.watch_flag		= 0;	// �t���O
    Fld.tv_camera.work				= 0;	// �]��

    Fld.tv_camera.old_damage_flag	= 0;	// �m�[�_���[�W�t���O
    Fld.tv_camera.old_kizetsu_flag	= 0;	// �m���t���O
    Fld.tv_camera.old_kaihuku_flag	= 0;	// �񕜃A�C�e���t���O
    Fld.tv_camera.old_throw_flag	= 0;	// �����X�^�{�[���t���O
    Fld.tv_camera.old_work			= 0;	// �]��

    Fld.tv_camera.worldpos			= 0;	// ������ꏊ
    Fld.tv_camera.battle			= 0;	// �������

	return;
}

//--------------------------------------------------------------------
//					�f�[�^�Z�b�g(�퓬��ɃZ�b�g)
//--------------------------------------------------------------------
void TVCameraBattleDataSet()
{
	u8 i;

    Fld.tv_camera.pokeno[0]		= FTW.MineLastPokeNo;
    Fld.tv_camera.pokeno[1]		= FTW.MineLastPokeNo2;
    Fld.tv_camera.waza			= FTW.MineLastWazaNo;
    if( Fld.tv_camera.battle != 0xff ) Fld.tv_camera.battle++;

    Fld.tv_camera.damage_flag	= FTW.DamageFlag;
	if( FTW.MineKizetsuCount > 0 )	Fld.tv_camera.kizetsu_flag = 1;
	else	Fld.tv_camera.kizetsu_flag = 0;

	if( FTW.ItemUseKaifukuCount > 0 )	Fld.tv_camera.kaihuku_flag = 1;
	else Fld.tv_camera.kaihuku_flag = 0;

	//�{�[���𓊂����J�E���g������ĂȂ��悤�Ȃ̂ł��̏����͓��ɈӖ����Ȃ�
	if( FTW.ThrowMasterBallFlag == 0 )
	{
		for( i=0; i < FTW_BALL_MAX ;i++ )
		{
			if( FTW.ThrowBall[i] > 0 )
			{
				Fld.tv_camera.throw_flag = 1;
				break;
			}
		}
	}else
	{
		Fld.tv_camera.throw_flag = 1;
	}

	//�f�[�^������Ƃ��ɐ퓬�f�[�^�����V�K�ɂȂ��Ăs�u������Ă��܂��̂�
	TVCameraWatchFlagReset();

	//���������Z�������Ȃ��Ő퓬�I��������(�����݂̂ŁA�񕜂��Â���Ȃ�)
	//���łɃC���^�r���[���󂯂���Ԃɂ���
    if( Fld.tv_camera.waza == 0 )	EventFlagSet( FH_00 );

	return;
}

//--------------------------------------------------------------------
//					�f�[�^�Z�b�g(�b���|���ŃZ�b�g)
//--------------------------------------------------------------------
void TVCameraDataSet()
{
    Fld.tv_camera.old_damage_flag	= Fld.tv_camera.damage_flag;
    Fld.tv_camera.old_kizetsu_flag	= Fld.tv_camera.kizetsu_flag;
    Fld.tv_camera.old_kaihuku_flag	= Fld.tv_camera.kaihuku_flag;
    Fld.tv_camera.old_throw_flag	= Fld.tv_camera.throw_flag;

    Fld.tv_camera.watch_flag	= TV_WATCH_FLAG_ON;	//������Ԃɂ���
    Fld.tv_camera.worldpos		= NowMap.WorldPos;

	//�C���^�r���[�̉񐔂��C���N�������g
	IncRecord( CNT_INTERVIEW );

	return;
}

//--------------------------------------------------------------------
//						watch_flag�����낷
//--------------------------------------------------------------------
void TVCameraWatchFlagReset()
{
    Fld.tv_camera.watch_flag = TV_WATCH_FLAG_OFF;
	return;
}

//--------------------------------------------------------------------
//						�퓬�񐔂��擾����
//						6�ȏ��6-8��Ԃ��悤�ɂ���
//--------------------------------------------------------------------
u8 TVCameraBattleGet()
{
	if( Fld.tv_camera.battle >= 6 )
		return ((Fld.tv_camera.battle % 3)+6);
	else
		return Fld.tv_camera.battle;
}

//--------------------------------------------------------------------
//						��������f�[�^�����邩
//--------------------------------------------------------------------
u8 TVCameraBroadCastChk()
{
	return Fld.tv_camera.watch_flag;
}

//--------------------------------------------------------------------
//						�O��̊ȈՉ�b��������
//--------------------------------------------------------------------
u8 TVCameraKaiwaSet()
{
	//�f�[�^�Ȃ�
	//if( Fld.tv_camera.easytalk_no == 0 ) return 0;
	if( Fld.tv_camera.easytalk_no == KAIWA_CODE_BLANK ) return 0;

	KaiwaCopyWord( StrTempBuffer0, Fld.tv_camera.easytalk_no );
	//Fld.tv_camera.easytalk_no = 0;
	Fld.tv_camera.easytalk_no = KAIWA_CODE_BLANK;
	return 1;
}

//--------------------------------------------------------------------
//					�O��̐퓬�̏󋵂��`�F�b�N����
//--------------------------------------------------------------------
u8 TVCameraOldFlagChk()
{
	if( Fld.tv_camera.old_damage_flag == 0 ) return 1;
	else if( Fld.tv_camera.old_throw_flag ) return 2;
	else if( Fld.tv_camera.old_kaihuku_flag ) return 3;
	else if( Fld.tv_camera.old_kizetsu_flag ) return 4;

	return 0;
}

//--------------------------------------------------------------------
//					�C���^�r���A�[��OBJ�w����Q�b�g����
//
//	�߂�l OBJ�w��
//		   �G���[�� = EV_OBJ_SPID
//--------------------------------------------------------------------
void TVCameraObjIDGet()
{
	switch( TVCameraBattleGet() )
	{
		case 1:
			ScriptParameter0 = INTERVW1_01_FIELD_R111;
			ScriptParameter1 = CAMERA1_01_FIELD_R111;
			break;
		case 2:
			ScriptParameter0 = INTERVW1_01_FIELD_R118;
			ScriptParameter1 = CAMERA1_01_FIELD_R118;
			break;
		case 3:
			ScriptParameter0 = INTERVW1_01_FIELD_R120;
			ScriptParameter1 = CAMERA1_01_FIELD_R120;
			break;
		case 4:
			ScriptParameter0 = INTERVW1_02_FIELD_R111;
			ScriptParameter1 = CAMERA1_02_FIELD_R111;
			break;
		case 5:
			ScriptParameter0 = INTERVW1_02_FIELD_R118;
			ScriptParameter1 = CAMERA1_02_FIELD_R118;
			break;
		case 6:
			ScriptParameter0 = INTERVW1_02_FIELD_R120;
			ScriptParameter1 = CAMERA1_02_FIELD_R120;
			break;
		case 7:
			ScriptParameter0 = INTERVW1_03_FIELD_R111;
			ScriptParameter1 = CAMERA1_03_FIELD_R111;
			break;
		case 8:
			ScriptParameter0 = INTERVW1_03_FIELD_R118;
			ScriptParameter1 = CAMERA1_03_FIELD_R118;
			break;
	};

	return;
}

