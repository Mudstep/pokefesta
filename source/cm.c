//====================================================================
//
//						�s�u���	01/11/21 nohara							
//
//						�b�l
//
//====================================================================
#include "common.h"
#include "intr.h"
#include "menu.h"					//print.h
#include "calctool.h"				//pp_rand
#include "script.h"					//ScriptParameter
#include "fld_talk.h"				//FieldMsgSet
#include "tv_def.h"					//TV��define�Q
#include "tv_tool.h"				//TV�̊֐��Q
#include "cm.h"						//CM��define�Q
#include "rtctool.h"				//GameTime
#include "sysflag.h"				//SYS_GAME_CLEAR
#include "..\sxydata\field_c102.sym"//GANBARIYA_01_FIELD_C102
#include "fieldmap.h"				//���̃C���N���[�h�̂���
#include "../evdata/maparray.h"		//C104_FS01


//====================================================================
//						���b�Z�[�W�f�[�^
//====================================================================
#include "cm_msg.dat"


//====================================================================
//						�v���g�^�C�v�錾	
//====================================================================
void CMDataSet();				//�f�[�^�Z�b�g
s8 CMDataAkiChk(TV_CM *);		//CM�f�[�^�̋󂢂Ă���ꏊ��������
void CMDataAllInit();			//CM�f�[�^������
u8 CMTimeCountChk();			//time_count <= CM_BROADCAST_TIME �����邩
void CMDataClear(u8);			//�w�肵���ꏊ��CM�f�[�^���폜
void CMDataSort();				//�󂫂������CM�f�[�^��O�ɂ߂�(�\�[�g)
static u8 CMDataTypeChk(u8);	//����datatype�����݂��邩
u8 CMEventStartChk(u8 type);	//�C�x���g�J�Â��Ă��邩
void CMData_Update(u16);		//���ԃJ�E���g�X�V
u8 CMDataHoldingTypeChk(u8);	//�^�C�v�ʊJ�Ã`�F�b�N


//====================================================================
//							�ϐ�
//====================================================================
//�J�ÑO�̕������b�Z�[�W
const u8 * const cm_msg_buf[]=
{
	NULL,
	msg_cm_01_p01_bargain,
	msg_cm_01_p01_casino,
	msg_cm_01_p01_find,
};

//�J�Ó����̕������b�Z�[�W(�ߑO)
const u8 * const cm_morning_msg_buf[]=
{
	NULL,
	msg_cm_01_p02_bargain,
	msg_cm_01_p02_casino,
	msg_cm_01_p02_find,
};

//�J�Ó����̕������b�Z�[�W(�ߌ�)
const u8 * const cm_night_msg_buf[]=
{
	NULL,
	msg_cm_01_p03_bargain,
	msg_cm_01_p03_casino,
	msg_cm_01_p03_find,
};


//====================================================================
//							�f�[�^�Z�b�g
//
//						CM�͂�����Ăׂ΂悢
//====================================================================
void CMDataSet()
{
	u16 rnd;
	u16 ans;

	//�Q�[���N���A���Ă��邩
	if( EventFlagCheck( SYS_GAME_CLEAR ) == FALSE ) return;

	//�󂫂����邩
	tv_akino = CMDataAkiChk(Fld.tv_cm);
	if( tv_akino == -1 )	return;

	//CM�f�[�^���쐬���邩�́A�����_�����擾
	if( TVRndChk(CM_START_ON) == 1 ) return;

	rnd = pp_rand();

	//datatype�擾
	ans = (rnd % (CM_DATATYPE_MAX-1)+1 );

	//����datatype�����݂��邩
	if( CMDataTypeChk( (u8)ans ) == 1 ) return;

	//datatype��������
    Fld.tv_cm[tv_akino].datatype = (u8)(ans);

	//���ԃJ�E���g�����l���擾
	Fld.tv_cm[tv_akino].time_count	= (CM_TIME_COUNT * TODAY_TIME);

    Fld.tv_cm[tv_akino].watch_flag = TV_WATCH_FLAG_ON;	//������Ԃɂ���
}


//====================================================================
//						�ʐM���ɕK�v�Ȋ֐�
//						�f�[�^�̃R�s�[�Ȃ�
//====================================================================

//--------------------------------------------------------------------
//				CM�f�[�^�̋󂢂Ă���ꏊ��������
//
//	�߂�l		�󂫂�������	0�`CMDATA_MAX�̐���
//				�󂫂��Ȃ�����  -1
//--------------------------------------------------------------------
s8 CMDataAkiChk(TV_CM *p)
{
	s8 i;

	for(i=0; i < CMDATA_MAX ;i++)
	{
		if( p[i].datatype == CM_NODATA_TYPE )	
			return i;
	}

	return -1;	//�󂫂��Ȃ�
}

//--------------------------------------------------------------------
//					   	 CM�f�[�^������
//			 �Q�[���J�n���ɌĂ΂��(TVDataAllInit����Ă΂��)
//--------------------------------------------------------------------
void CMDataAllInit()
{
	u8 i;

	for(i=0; i < CMDATA_MAX ;i++)
	{
		CMDataClear(i);
	}

	return;
}

//--------------------------------------------------------------------
//			   	 �w�肵���ꏊ��CM�f�[�^���폜
//--------------------------------------------------------------------
void CMDataClear(u8 no)
{
	Fld.tv_cm[no].datatype=CM_NODATA_TYPE;			// �f�[�^�Ȃ�
	Fld.tv_cm[no].watch_flag=TV_WATCH_FLAG_OFF;		//����Ȃ���Ԃɂ���
	Fld.tv_cm[no].time_count=0;
}

//--------------------------------------------------------------------
//				�󂫂������CM�f�[�^��O�ɂ߂�(�\�[�g)
//--------------------------------------------------------------------
void CMDataSort()
{
	u8 i,j;

	for(i=0; i < CMDATA_MAX-1 ;i++)
	{
		if( Fld.tv_cm[i].datatype == CM_NODATA_TYPE )	
		{
			for(j=i+1; j < CMDATA_MAX ;j++)
			{
				if( Fld.tv_cm[j].datatype != CM_NODATA_TYPE )	
				{
					Fld.tv_cm[i]=Fld.tv_cm[j];
					CMDataClear(j);
					break;
				}
			}
		}
	}

	return;
}

//--------------------------------------------------------------------
//							CM�f�[�^
//
//				time_count <= CM_BROADCAST_TIME �����邩�`�F�b�N
//
//	�߂�l		0��������		0�`CMDATA_MAX�̒l
//				0���Ȃ�����     TV_NOTHING
//--------------------------------------------------------------------
u8 CMTimeCountChk()
{
	u8 i;
	
	for(i=0; i < CMDATA_MAX ;i++)
	{
		if( Fld.tv_cm[i].datatype != CM_NODATA_TYPE )
		{
			//������Ԃ�
			if( Fld.tv_cm[i].watch_flag == TV_WATCH_FLAG_ON )
			{
				//�����J�n������菭�Ȃ���
				if( Fld.tv_cm[i].time_count <= CM_BROADCAST_TIME )
					return i;	
			}
		}
	}

	return TV_NOTHING;			
}

//--------------------------------------------------------------------
//							�b�l����
//
//	AnswerWork�ɕ����������������Ă���		0 = �������Ȃ�����
//												1 = ��������
//--------------------------------------------------------------------
void CMBroadCastSet()
{
	u8 ans;
	u16 ret;

	ans = CMTimeCountChk();
	if( ans == TV_NOTHING )
	{
		AnswerWork = 0;
		return;
	}

	//������������
	if( Fld.tv_cm[ans].time_count < TODAY_TIME )
	{
		Fld.tv_cm[ans].watch_flag=TV_WATCH_FLAG_HOLDING;	//�J�Ó���

		//�\�����郁�b�Z�[�W��datatype�ɂ���ĕ�����悤�ɂ���
		if( GameTime.hour < CM_BROADCAST_CHANGE_TIME )	
			FieldMsgSet(cm_morning_msg_buf[Fld.tv_cm[ans].datatype]);	//��
		else							
			FieldMsgSet(cm_night_msg_buf[Fld.tv_cm[ans].datatype]);		//��

	}else
	{
		//���Ɖ���
		ret = Fld.tv_cm[ans].time_count / TODAY_TIME;
		PM_NumMsgSet( StrTempBuffer0, (int)ret, NUM_MODE_LEFT, 1);

		//����Ȃ���Ԃɂ���(�����񂾂�)
		Fld.tv_cm[ans].watch_flag = TV_WATCH_FLAG_OFF;

		//�\�����郁�b�Z�[�W��datatype�ɂ���ĕ�����悤�ɂ���
		FieldMsgSet(cm_msg_buf[Fld.tv_cm[ans].datatype]);
	};

	AnswerWork = 1;					//��������
}

//--------------------------------------------------------------------
//					�C�x���g�J�Â��Ă��邩
//
//	�߂�l	1 = �J�Ò�
//			0 = �܂�
//--------------------------------------------------------------------
u8 CMEventStartChk(u8 type)
{
	u8 i;

	if( CM_NODATA_TYPE == type )
	{
		return 0;
	}

	//datatype��n���悤�ɂ���
	for( i=0; i < CMDATA_MAX ;i++ )
	{
		if( Fld.tv_cm[i].datatype == type )
		{
			//�J�Ó�����������
			if( Fld.tv_cm[i].watch_flag == TV_WATCH_FLAG_HOLDING )
			{
				if( CMDataHoldingTypeChk(type) ) return 1;
				else	return 0;
			}else
			{
				return 0;			//�܂�
			}
		}
	}

	return 0;
}

u8 CMDataHoldingTypeChk(u8 type)
{
	switch( type )
	{
		case CM_BARGAIN_TYPE:
			//�J�C�i�ł���΂艮��OBJ�ɘb���|���Ă��邩
			if( Fld.MapID.div_id == FIELD && Fld.MapID.map_id == FIELD_C102 &&
					TargetObjID == GANBARIYA_02_FIELD_C102 ) 
			{
				return TRUE;
			}else
			{
				return FALSE;
			}

			break;
		case CM_FIND_ITEM_TYPE:
			//�f�p�[�g�̉��ォ
			if( Fld.MapID.div_id == C106 && Fld.MapID.map_id == C106_R1106 ) 
			{
				return TRUE;
			}else
			{
				return FALSE;
			}

			break;
	}

	//����ȊO�̃^�C�v�̎��͊J�Â��Ă���
	return TRUE;
}


//--------------------------------------------------------------------
//				����datatype�����݂��邩(�b�l)
//
//	�߂�l		0=���݂��Ȃ�
//				1=���݂���
//--------------------------------------------------------------------
static u8 CMDataTypeChk(u8 type)
{
	u8 i;

	if( type == CM_NODATA_TYPE )	return 1;

	for(i=0; i < CMDATA_MAX ;i++)
	{
		if( Fld.tv_cm[i].datatype == type )
				return 1;	//���݂���
	}

	return 0;	//���݂��Ȃ�
}

//--------------------------------------------------------------------
//				�b�l�f�[�^�̎��ԃJ�E���g��i�߂�
//
//  ���� days   �����������߂邩�H
//--------------------------------------------------------------------
void CMData_Update(u16 days)
{
	u8 i;

	for(i=0; i < CMDATA_MAX ;i++)
	{
		if( Fld.tv_cm[i].datatype != CM_NODATA_TYPE )
		{
			if( Fld.tv_cm[i].time_count < days )
			{
				CMDataClear(i);
			}else
			{
				//����Ȃ���Ԃ�������A������Ԃɂ���
				if( Fld.tv_cm[i].watch_flag == TV_WATCH_FLAG_OFF && 
						EventFlagCheck( SYS_GAME_CLEAR ) == TRUE )
					Fld.tv_cm[i].watch_flag = TV_WATCH_FLAG_ON;

				Fld.tv_cm[i].time_count-=days;
			}
		}
	}

	CMDataSort();
	return;
}


