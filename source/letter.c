//====================================================================
//
//						�s�u���	01/11/15 nohara							
//
//						�莆�^�E�閧�^
//
//
//
//
/*
						���莆�^��ǉ������Ƃ���
						�ETVDataSetSelect�ɒǉ�

						���������f����ύX�����Ƃ���
						�E�����_���Ȓl��ۑ����Ă���̂ŁA
						�@���̕����ύX�ɂȂ�����
						�@LetterNameDataSet���ύX����


*/
//====================================================================
#include "common.h"
#include "intr.h"
#include "menu.h"					//menutool.h
#include "calctool.h"				//pp_rand
#include "script.h"					//ScriptParameter
#include "fieldmap.h"				//NowMap
#include "poketool.h"				//PokeNameData
#include "contest.h"				//ConTemotiNo
#include "conscr.h"					//SUPER_RANK
#include "server.h"					//FTW
#include "item.h"					//ITEM_MASTERBALL
#include "tv_def.h"					//TV��define�Q
#include "tv_tool.h"				//�֐��Q
#include "cm.h"						//cm_event_flag
#include "record.h"					//GetRecord
#include "b_tower.h"				//BattleTowerWinGet
#include "monsno.def"				//MONSNO_AMETAMA
#include "wazano.def"				//WAZANO_AMETAMA
#include "sysflag.h"
#include "../evdata/maparray.h"		//FIELD_R102
#include "scr_tool.h"				//GetTemotiBtlPokeNum


//====================================================================
//						  extern
//====================================================================
//fd_shop.c
extern MINEITEM TVItemData[];			//�������������̂�����


//====================================================================
//						�v���g�^�C�v�錾
//====================================================================
void TVData_Update(u16);			//���ɂ�����
void Generating_Update(u16);		//��ʔ����̓��ɂ�����
void GeneratingIncubation_Update(u16);	//��ʔ��������̓��ɂ�����
void TVOneDay_Update(u16);			//����ɕ߂܂������̓��ɂ�����

//�莆�^
void LetterPokemonDataSet();		//�|�P�����A���P�[�g
void LetterRecentlyDataSet();		//�ŋ߂̏o����
void LetterClubDataSet();			//��D���N���u�̃C���^�r���[
void LetterGymDataSet();			//�W�����[�_�[���̃C���^�r���[
void LetterNameDataSet();			//�������f��
void LetterGeneratingDataSet();		//��ʔ����f�[�^�̃����_������
void GeneratingEventSet();
void GeneratingEventClear();		//��ʔ����C�x���g���I������
void TVLetterIDSet(TV_Letter_Work *p);//�莆�^�ɕK�v��ID���Z�b�g
void TVConDataSet();				//�R���e�X�g�D��
void TVConWazaSet(u16);				//�Ō�Ɏg�p�����Z���Z�b�g
void TVConZyuniSet(u8);				//�R���e�X�g�̏��ʂƃ^�C�v���Z�b�g
void TVTowerDataSet();				//�o�g���^���[
u8 TVTowerWinLoseFlagGet();			//�o�g���^���[���s�擾
u8 TVTowerSeqNoGet();				//�o�g���^���[�󋵎擾

//�閧�^
void TVDataSetSelect();				//�f�[�^�Z�b�g
void TVBtlDataSet();				//�߂܂����|�P�����̏Љ�
void TVShopDataSet();				//�����̔��������
void TVGetFailedDataSet();			//�ߊl���s
void TVSecretIDSet(TV_Sec_Shop_Work *p);//�閧�^�ɕK�v��ID���Z�b�g
void TVFishingDataSet();			//�ނ肵�܂���
void TVFishingCount(u8);			//�ނ�̉񐔂��J�E���g
void TVFishingPokeNoSet(u16);		//�ނ����|�P�����i���o�[�Z�b�g
void TVOneDayDataCount();			//����ŕ߂܂��������J�E���g����
void TVOneDayDataSet();				//����ŕ߂܂������Z�b�g

#define GENERATING_POKE_DATA_MAX	5		//��ʔ����f�[�^�ő吔
typedef struct{
	u16 monsno;				//�|�P�����i���o�[
	u16 waza1;				//�o���Ă���Z
	u16 waza2;				//�o���Ă���Z
	u16 waza3;				//�o���Ă���Z
	u16 waza4;				//�o���Ă���Z
	u8 level;				//���x��
	u8 map_id;				//mapID
} GeneratingPokeData;

static const GeneratingPokeData GPD[GENERATING_POKE_DATA_MAX] = {
	{ MONSNO_AMETAMA, WAZANO_AWA,	  WAZANO_DENKOUSEKKA, 0, 0, 3, FIELD_R102 },
	{ MONSNO_AMETAMA, WAZANO_AWA,	  WAZANO_DENKOUSEKKA, 0, 0, 15, FIELD_R114 },	
	{ MONSNO_AMETAMA, WAZANO_AWA,	  WAZANO_DENKOUSEKKA, 0, 0, 15, FIELD_R117 },
	{ MONSNO_AMETAMA, WAZANO_AWA,	  WAZANO_DENKOUSEKKA, 0, 0, 28, FIELD_R120 },
	{ MONSNO_ENEKO,   WAZANO_NAKIGOE, WAZANO_TAIATARI,	  0, 0, 15, FIELD_R116 }
};


//====================================================================
//							�v���O����
//====================================================================

//--------------------------------------------------------------------
//
//							�f�[�^�Z�b�g
//
//			ScriptParameter1 = datatype�����ď����𕪂���
//--------------------------------------------------------------------
void TVDataSetSelect()
{
	switch( ScriptParameter1 )
	{
		case TV_POKEMON_TYPE1:
			LetterPokemonDataSet();
			break;
		case TV_RECENTLY_TYPE1:
			LetterRecentlyDataSet();
			break;
		case TV_CLUB_TYPE1:
			LetterClubDataSet();
			break;
		case TV_GYMLEADER_TYPE1:
			LetterGymDataSet();
			break;
		case TV_CONTEST_TYPE1:
			TVConDataSet();
			break;
		case TV_TOWER_TYPE1:
			TVTowerDataSet();
			break;
	}
}

//--------------------------------------------------------------------
//					�߂܂����|�P�����̏Љ�f�[�^�쐬
//--------------------------------------------------------------------
void TVBtlDataSet()
{
	u8  i;
	u16 type,ball;
	TV_Sec_Btl_Work * p;
	ball = 0;

	//�f�[�^�쐬�̃`�F�b�N��퓬��ɍs��	add 02/08/26
	CMDataSet();						// CM�f�[�^�̃����_������
	LetterGeneratingDataSet();			// ��ʔ����f�[�^�̃����_������

	//�ߊl���s�E����ŕ߂܂������̍쐬�`�F�b�N
	if( FTW.GetPokeNo == 0 )
	{
		//�ߊl���s
		TVGetFailedDataSet();
		return;
	}else
	{
		//����ŕ߂܂��������J�E���g����
		TVOneDayDataCount();
	}

	//�����`�F�b�N
	if( TVRndChk(TV_GETPOKE_START_ON) ) return;

	//�j�b�N�l�[����������
	if( PM_strcmp( (u8 *)PokeNameData[FTW.GetPokeNo],
				(u8 *)FTW.GetPokeNickname ) == 0 )
		return;
	
	tv_akino = TVDataPartnerAkiChk(Fld.tv_event);
	if( tv_akino == -1 )	return;		//�󂫂��Ȃ�

	if( TVSecDataTypeChk(TV_GETPOKE_TYPE1) == 1 ) return;

	//�{�[���𓊂����񐔂��J�E���g����(�T�t�@���]�[��)
	for( i=0; i < FTW_BALL_MAX ;i++ )
	{
		ball += FTW.ThrowBall[i];
	}

	//�{�[���𓊂��Ă��Ȃ�(�T�t�@���]�[��)
	if( ball == 0 && FTW.ThrowMasterBallFlag == 0 ) return;

	ball = 0;		//�N���A
	p = (TV_Sec_Btl_Work *)&Fld.tv_event[tv_akino];

	p->datatype = TV_GETPOKE_TYPE1;
	p->watch_flag = TV_WATCH_FLAG_OFF;

	if( FTW.ThrowMasterBallFlag == 1 )
	{
		ball = 1;
		type = ITEM_MASUTAABOORU;
	}else
	{
		for( i=0; i < FTW_BALL_MAX ;i++ )
		{
			ball += FTW.ThrowBall[i];
		}

		//�P�o�C�g�������e�ʊm�ۂ��Ă��Ȃ��̂�
		if( ball > 0xff ) ball = 0xff;

		type = ItemNo;			//server.h�ɂ���
	}

    p->ball_count =	(u8)ball;
    p->ball_type =	(u8)type;
	
	PM_strcpy(p->player_name, MyData.my_name);// ���O
	PM_strcpy(p->nickname, FTW.GetPokeNickname);//�j�b�N�l�[��

    p->get_pokeno = FTW.GetPokeNo;		// �߂܂����|�P�����̎��
	TVSecretIDSet((TV_Sec_Shop_Work *)p);
}

//--------------------------------------------------------------------
//						�߂܂��������J�E���g
//--------------------------------------------------------------------
void TVOneDayDataCount()
{
	TV_Sec_OneDay_Work * p;
	p = (TV_Sec_OneDay_Work*)&TV_TEMP_WORK;

	if( p->datatype != TV_ONEDAY_TYPE1 )
	{
		TVDataClear(Fld.tv_event,TVDATA_MAX);		// �N���A����
		p->walk_count	= GetRecord(CNT_WALK);		//���݂̕����ۑ�
		p->datatype		= TV_ONEDAY_TYPE1;
	}

    p->count++;										//�߂܂�����
    p->get_pokeno	= FTW.GetPokeNo;				//�߂܂����|�P�����̎��
	
	//�g�p���Ă����|�P�����̎��
    p->pokeno		= FTW.MineLastPokeNo;	

	p->worldpos		= NowMap.WorldPos;

	return;
}

//--------------------------------------------------------------------
//						�Q�b�g���s�s�u�f�[�^�쐬
//--------------------------------------------------------------------
void TVGetFailedDataSet()
{
	u8  i;
	u16 count;
	TV_Sec_Get_Failed_Work * p;

	//�����`�F�b�N
	if( TVRndChk(TV_FAILED_START_ON) ) return;

	//�R��ȏ�{�[���𓊂��Ă��邩
	for( i=0, count=0; i < FTW_BALL_MAX ;i++ )
	{
		count += FTW.ThrowBall[i];
	}

	//�P�o�C�g�������e�ʊm�ۂ��Ă��Ȃ��̂�
	if( count > 0xff ) count = 0xff;

	if( count < TV_FAILED_CREATE_NUM ) return;

	//�G�|�P���������������A�퓬�ɏ�������
	if( WinLoseFlag == FIGHT_ENEMY_ESCAPE || WinLoseFlag == FIGHT_WIN )
	{
		tv_akino = TVDataPartnerAkiChk(Fld.tv_event);
		if( tv_akino == -1 )	return;		//�󂫂��Ȃ�

		if( TVSecDataTypeChk(TV_FAILED_TYPE1) == 1 ) return;

		p = (TV_Sec_Get_Failed_Work *)&Fld.tv_event[tv_akino];

		p->datatype		= TV_FAILED_TYPE1;
		p->watch_flag	= TV_WATCH_FLAG_OFF;

		p->pokeno		= FTW.MineLastPokeNo;	

		p->failed_pokeno= FTW.EnemyLastPokeNo;
		p->ball_count	= (u8)count;

		p->failed_flag	= WinLoseFlag;
		p->worldpos		= NowMap.WorldPos;

		PM_strcpy(p->player_name, MyData.my_name);// ���O

		TVSecretIDSet((TV_Sec_Shop_Work *)p);
	}
}

//--------------------------------------------------------------------
//						�閧�^�ɕK�v��ID���Z�b�g
//--------------------------------------------------------------------
void TVSecretIDSet(TV_Sec_Shop_Work *p)
{
	u32 id;

	id = GetMyDataID();

    p->data_id[0] = (u8)(id & 0xff);				//�s�u�f�[�^����ID
    p->data_id[1] = (u8)( (id >> 8)& 0xff );		//�s�u�f�[�^����ID

    p->before_id[0] = (u8)(id & 0xff);				//�O�̃g���[�i�[ID
    p->before_id[1] = (u8)( (id >> 8 )& 0xff );		//�O�̃g���[�i�[ID

    p->trainer_id[0] = (u8)(id & 0xff);				// �s�u�f�[�^�����ID
    p->trainer_id[1] = (u8)( (id >> 8 )& 0xff );	// �s�u�f�[�^�����ID
}

//-------------------------------------------------------------------
//						�莆�^�ɕK�v��ID���Z�b�g
//--------------------------------------------------------------------
void TVLetterIDSet(TV_Letter_Work *p)
{
	u32 id;

	id = GetMyDataID();

    p->data_id[0] = (u8)(id & 0xff);				//�s�u�f�[�^����ID
    p->data_id[1] = (u8)( (id >> 8)& 0xff );		//�s�u�f�[�^����ID

    p->before_id[0] = (u8)(id & 0xff);				//�O�̃g���[�i�[ID
    p->before_id[1] = (u8)( (id >> 8)& 0xff );		//�O�̃g���[�i�[ID
}

//====================================================================
//						�R���e�X�g��s�u�f�[�^�쐬
//====================================================================
void TVConDataSet()
{
	TV_Con_Work * p;
	TV_Con_Work * from_p;

	from_p = (TV_Con_Work *)&TV_TEMP_WORK;

	//�ۑ��p���[�N�̃f�[�^�^�C�v��������
	if( from_p->datatype != TV_CONTEST_TYPE1 ) return;

	p = (TV_Con_Work *)&Fld.tv_event[tv_akino];

    p->datatype		=	TV_CONTEST_TYPE1;
    p->watch_flag	=	TV_WATCH_FLAG_ON;

	//change 02/09/17
	p->pokeno		=	from_p->pokeno;	

	PM_strcpy(p->player_name, MyData.my_name);// ���O

	//change 02/09/17
	PM_strcpy(p->nickname, from_p->nickname);

	p->type		=		 from_p->type;
    p->rank		=		 from_p->rank;

    p->waza		=		 from_p->waza;			// �Ō�Ɏg�����Z
    p->zyuni	=		 from_p->zyuni;			// ����
    p->type		=		 from_p->type;			// �^�C�v

	TVLetterIDSet((TV_Letter_Work *)p);
}

//====================================================================
//				�R���e�X�g�ōŌ�Ɏg�p�����Z���Z�b�g����
//====================================================================
void TVConWazaSet(u16 waza)
{
	TV_Con_Work * p;
	p = (TV_Con_Work *)&TV_TEMP_WORK;

	//���̃^�C�~���O�Ŏ莆�`�F�b�N��������	add 02/09/09
	//�R���e�X�g��̃C���^�r���A�[���o���`�F�b�N����
	//ScrLetterConTypeChk���s���̂ŁA
	//���ɏ����Ă���TVDataHimeselfAkiChk��
	//���̊֐��ł́A�͂������̂ɁA
	//�C���^�r���A�[�o���`�F�b�N�Œʂ��Ă��܂��̂ŁI
	//(���Ă��铯���f�[�^����������폜����悤�ɂ��ċ󂫂����)
	ScrLetterConTypeChk();

	//�����f�[�^�^�C�v�Ō��Ă��Ȃ���Ԃ��������Ƃ���
	//tv_akino�ɒl�����Ă��Ȃ��̂�TVDatahimselfAkiChk������ add 02/09/09
	tv_akino = TVDataHimselfAkiChk((TV_EVENT *)Fld.tv_event);
	if( tv_akino == -1 )	return;	//�󂫂��Ȃ�

	TVDataClear(Fld.tv_event,TVDATA_MAX);	// �N���A����
    p->waza = waza;					// �Ō�Ɏg�����Z
    p->datatype = TV_CONTEST_TYPE1;
	return;
}

//====================================================================
//					�R���e�X�g�̏��ʂ��Z�b�g����
//					�R���e�X�g�̃^�C�v���Z�b�g����
//====================================================================
void TVConZyuniSet(u8 zyuni)
{
	TV_Con_Work * p;
	p = (TV_Con_Work *)&TV_TEMP_WORK;

	tv_akino = TVDataHimselfAkiChk((TV_EVENT *)Fld.tv_event);
	if( tv_akino == -1 )	return;	//�󂫂��Ȃ�

    p->zyuni = zyuni;				// ����
    p->type = (u8)ContestKind;		// �R���e�X�g�̃^�C�v
    p->rank = (u8)ContestRank;		// �R���e�X�g�̃����N
	p->pokeno =	PokeParaGet( &PokeParaMine[ConTemotiNo], ID_monsno, 0 );//�|�P�����i���o�[	
	PokeParaGet( &PokeParaMine[ConTemotiNo], ID_nickname, p->nickname );//�j�b�N�l�[��	

	return;
}

//====================================================================
//						�o�g���^���[�s�u�f�[�^�쐬
//
//				�󂯓����f�[�^��ScriptParameter0�ɓ���Ă���
//====================================================================
void TVTowerDataSet()
{
	TV_Tower_Work * p;

	p = (TV_Tower_Work *)&Fld.tv_event[tv_akino];

    p->datatype =	TV_TOWER_TYPE1;
    p->watch_flag =	TV_WATCH_FLAG_ON;

	PM_strcpy(p->player_name, MyData.my_name);	// ���O
	PM_strcpy(p->enemy_name, MyData.BattleTowerWork.SBTTVD.TrainerName);// �Ō�ɑΐ킵������̖��O

    p->pokeno = MyData.BattleTowerWork.SBTTVD.MineLastPokeNo;		//�������Ō�ɏo���Ă���pokeno
    p->enemy_pokeno = MyData.BattleTowerWork.SBTTVD.EnemyLastPokeNo;//���肪				pokeno

	//change 02/09/18
	//p->win		= BattleTowerWinGet( MyData.BattleTowerWork.BattleTowerLevel );	//�A����
	p->win		= BattleTowerWinGet( MyData.BattleTowerWork.BattleTowerInterviewLevel );//�A����

	p->win_flag	= 
		MyData.BattleTowerWork.BattleTowerWinLoseFlag;		//���s

	//change 02/09/18
	//if( MyData.BattleTowerWork.BattleTowerLevel == 0 ) p->level = 50;	//���x��50
	if( MyData.BattleTowerWork.BattleTowerInterviewLevel == 0 ) p->level = 50;	//���x��50
	else p->level = 100;												//���x��100

	p->ans		= ScriptParameter0;												//�󂯓����f�[�^

	TVLetterIDSet((TV_Letter_Work *)p);
}

//====================================================================
//						���������s�u�f�[�^�쐬
//====================================================================
void TVShopDataSet()
{
	u8 i;
	TV_Sec_Shop_Work * p;

	//�����`�F�b�N
	if( TVRndChk(TV_SHOP_START_ON) ) return;

	tv_akino = TVDataPartnerAkiChk(Fld.tv_event);
	if( tv_akino == -1 )	return;	//�󂫂��Ȃ�

	if( TVSecDataTypeChk(TV_SHOP_TYPE1) == 1 ) return;

	//���̑������ɕ��ёւ��āA20�ȏ�̂��̂����邩
	TVShopDataSort();
	if( TVItemData[0].no < TV_SHOP_CREATE_NUM ) return;

	p = (TV_Sec_Shop_Work *)&Fld.tv_event[tv_akino];

    p->datatype = TV_SHOP_TYPE1;
	p->watch_flag = TV_WATCH_FLAG_OFF;

    p->worldpos = NowMap.WorldPos;
	
	for( i=0; i < 3; i++ )
	{
		p->itemno[i] = TVItemData[i].item;
		p->num[i] = TVItemData[i].no;
	}

	p->bargain_flag = SALE_FLG;		//������肩

	PM_strcpy(p->player_name, MyData.my_name);// ���O

	TVSecretIDSet((TV_Sec_Shop_Work *)p);
}


//====================================================================
//						�������f���s�u�f�[�^�쐬
//====================================================================
void LetterNameDataSet()
{
	TV_Name_Work * p;

	ScrLetterNameTypeChk();
	if( AnswerWork == 1 )	return;

	//StrTempBuffer0�ɑ�����āA�Q�����ȏォ�`�F�b�N
	//ScriptParameter0�Ƀ��X�g�̉��Ԗڂ�I�񂾂��������Ă���
	PokeParaGet( &PokeParaMine[ScriptParameter0], ID_nickname, StrTempBuffer0 );	
	if( PM_strlen( (const u8 *)MyData.my_name ) < 2 ) return;
	if( PM_strlen( (const u8 *)StrTempBuffer0 ) < 2 ) return;

	p = (TV_Name_Work *)&Fld.tv_event[tv_akino];

    p->datatype = TV_NAME_TYPE1;
	p->watch_flag = TV_WATCH_FLAG_ON;

	p->pokeno	= PokeParaGet( &PokeParaMine[ScriptParameter0], ID_monsno, 0 );	

	//�����_���őI������������ۑ�����
	//�����_���őI�������|�P�����i���o�[��ۑ�����
	p->r_q_no[0]= (pp_rand() % 3);
	p->r_q_no[1]= (pp_rand() % 2);
	p->r_pokeno = RndChoicePoke(p->pokeno);

	PM_strcpy(p->player_name, MyData.my_name);// ���O
	PokeParaGet( &PokeParaMine[ScriptParameter0], ID_nickname, p->nickname );	

	TVLetterIDSet((TV_Letter_Work *)p);
}

//--------------------------------------------------------------------
//					Generating_Event_Work�ɃZ�b�g����
//
//	ScriptParameter0 = �������Ă���v�f��(tv.ev�ő�������)
//--------------------------------------------------------------------
void GeneratingEventSet()
{
	TV_Generating_Work *p;
	p = (TV_Generating_Work *)&Fld.tv_event[ScriptParameter0];

	Fld.tv_encount.pokeno		= p->pokeno;	// �|�P�����̎��
    Fld.tv_encount.map_id		= p->map_id;	// �ǂ��ŏo��������
    Fld.tv_encount.div_id		= p->div_id;	// �ǂ��ŏo��������

    Fld.tv_encount.level		= p->level;		// �ǂ̃��x���ŏo�Ă��邩
    Fld.tv_encount.special		= p->special;	// ����\��
    Fld.tv_encount.itemno		= p->itemno;	// �����Ă���A�C�e��

    Fld.tv_encount.waza[0]		= p->waza[0];	// �����Ă���Z
    Fld.tv_encount.waza[1]		= p->waza[1];	// �����Ă���Z
    Fld.tv_encount.waza[2]		= p->waza[2];	// �����Ă���Z
    Fld.tv_encount.waza[3]		= p->waza[3];	// �����Ă���Z

    Fld.tv_encount.character	= p->character;	// ���i
    Fld.tv_encount.appearance	= p->appearance;// �o��(%)
    Fld.tv_encount.time_count	= GENERATING_TIME;	// ��ʔ������Ă������
}

//====================================================================
//					�莆(�|�P�����A���P�[�g)�쐬
//====================================================================
void LetterPokemonDataSet()
{
	TV_Letter_Work * p;

	p = (TV_Letter_Work *)&Fld.tv_event[tv_akino];

    p->datatype =	TV_POKEMON_TYPE1;
    p->watch_flag =	TV_WATCH_FLAG_ON;

	PM_strcpy(p->player_name, MyData.my_name);	// ���O

	p->inside_data = PokeParaGet( &PokeParaMine[GetTemotiBtlPokeNum()], ID_monsno, 0 );	//�莝���̐擪�|�P����
	TVLetterIDSet((TV_Letter_Work *)p);
}

//====================================================================
//						�莆(�ŋ߂̏o����)�쐬
//====================================================================
void LetterRecentlyDataSet()
{
	TV_Letter_Work * p;

	p = (TV_Letter_Work *)&Fld.tv_event[tv_akino];

    p->datatype =	TV_RECENTLY_TYPE1;
    p->watch_flag =	TV_WATCH_FLAG_ON;

	PM_strcpy(p->player_name, MyData.my_name);// ���O

    p->inside_data = 0;
	TVLetterIDSet((TV_Letter_Work *)p);
}

//====================================================================
//				�莆(��D���N���u�C���^�r���[)�쐬
//====================================================================
void LetterClubDataSet()
{
	TV_Club_Work * p;

	p = (TV_Club_Work *)&Fld.tv_event[tv_akino];

    p->datatype =	TV_CLUB_TYPE1;
    p->watch_flag =	TV_WATCH_FLAG_ON;

	//�Ȃ��x���g�p�I�I
	p->friend = (u8)( PokeParaGet( &PokeParaMine[GetTemotiBtlPokeNum()], ID_friend, 0 ) >> 4 );
    p->ans =		(u8)ScriptParameter3;

	PM_strcpy(p->player_name, MyData.my_name);// ���O
	PokeParaGet( &PokeParaMine[GetTemotiBtlPokeNum()], ID_nickname, p->nickname );	

	p->pokeno = PokeParaGet( &PokeParaMine[GetTemotiBtlPokeNum()], ID_monsno, 0 );
	TVLetterIDSet((TV_Letter_Work *)p);
}

//====================================================================
//				�莆(�W�����[�_�[���̃C���^�r���[)�쐬
//====================================================================
void LetterGymDataSet()
{
	return;
}

//====================================================================
//					�莆(��ʔ����C�x���g)�쐬
//====================================================================
void LetterGeneratingDataSet()
{
	u8 i;
	u16 rnd;
	TV_Generating_Work * p;

	//�Q�[���N���A���Ă��Ȃ�������
	if( EventFlagCheck( SYS_GAME_CLEAR ) == FALSE ) return;

	//����datatype�����݂���ꍇ�͍��Ȃ�
	for( i=0; i < TVDATA_MAX ;i++ )
	{
		if( Fld.tv_event[i].datatype == TV_GENERATING_TYPE1 )
			return;
	}
	
	//��ʔ������쐬���邩�́A�����_�����擾
	if( TVRndChk(TV_GENERATING_START_ON) ) return;
	
	tv_akino = TVDataHimselfAkiChk((TV_EVENT *)Fld.tv_event);
	if( tv_akino == -1 )	return;	//�󂫂��Ȃ�

	rnd = (pp_rand() % GENERATING_POKE_DATA_MAX);

	p = (TV_Generating_Work *)&Fld.tv_event[tv_akino];

    p->datatype =	TV_GENERATING_TYPE1;
    p->watch_flag =	TV_WATCH_FLAG_ON;

    p->level =		GPD[rnd].level;
    p->special =	0;
    p->character =	0;
	p->pokeno =		GPD[rnd].monsno;
    p->itemno =		0;

    p->waza[0] =	GPD[rnd].waza1;
    p->waza[1] =	GPD[rnd].waza2;
    p->waza[2] =	GPD[rnd].waza3;
    p->waza[3] =	GPD[rnd].waza4;

    p->map_id =		GPD[rnd].map_id;
    p->div_id =		FIELD;
    p->rare =		0;
    p->appearance = 50;								//�o����50%
    p->batch =		0;
    p->time_count =	GENERATING_INCUBATION_TIME;		//��������

	TVLetterIDSet((TV_Letter_Work *)p);
}

//--------------------------------------------------------------------
//						��ʔ����C�x���g�N���A
//--------------------------------------------------------------------
void GeneratingEventClear()
{
	Fld.tv_encount.pokeno		= 0;	// �|�P�����̎��
    Fld.tv_encount.map_id		= 0;	// �ǂ��ŏo��������
    Fld.tv_encount.div_id		= 0;	// �ǂ��ŏo��������

    Fld.tv_encount.level		= 0;	// �ǂ̃��x���ŏo�Ă��邩
    Fld.tv_encount.special		= 0;	// ����\��
    Fld.tv_encount.itemno		= 0;	// �����Ă���A�C�e��

    Fld.tv_encount.waza[0]		= 0;	// �����Ă���Z
    Fld.tv_encount.waza[1]		= 0;	// �����Ă���Z
    Fld.tv_encount.waza[2]		= 0;	// �����Ă���Z
    Fld.tv_encount.waza[3]		= 0;	// �����Ă���Z

    Fld.tv_encount.character	= 0;	// ���i
    Fld.tv_encount.appearance	= 0;	// �o��(%)
    Fld.tv_encount.time_count	= 0;	// �C�x���g�I���܂ł̎c�莞��
}

//--------------------------------------------------------------------
//							���ɂ�����
//--------------------------------------------------------------------
void TVData_Update(u16 days)
{
	GeneratingIncubation_Update(days);	//��ʔ���(����)
	Generating_Update(days);			//��ʔ���
	CMData_Update(days);				//CM
	TVOneDay_Update(days);				//����ɕ߂܂�����
	return;
}

//--------------------------------------------------------------------
//					��ʔ��������̓��ɂ�����
//--------------------------------------------------------------------
void GeneratingIncubation_Update(u16 days)
{
	u8 i;
	TV_Generating_Work *p;

	//��ʔ������ĂȂ�������
	if( Fld.tv_encount.pokeno == 0 )
	{
		//�v�f���̈�ԎႢ�����猩�Ă���
		//��ʔ����f�[�^�̐������������炷
		for( i=0; i < TVDATA_MAX ;i++ )
		{
			if( Fld.tv_event[i].datatype == TV_GENERATING_TYPE1
					&& Fld.tv_event[i].watch_flag == TV_WATCH_FLAG_ON )
			{
				p = (TV_Generating_Work *)&Fld.tv_event[i];
				if( p->time_count < days )	p->time_count = 0;
				else	p->time_count -= days;
				return;
			}
		}
	}

	return;
}

//--------------------------------------------------------------------
//						��ʔ����̓��ɂ�����
//--------------------------------------------------------------------
void Generating_Update(u16 days)
{
	if( Fld.tv_encount.time_count <= days )	GeneratingEventClear();
	else	Fld.tv_encount.time_count -= days;
	return;
}

static u16 tv_fish_pokeno = 0;
static u16 tv_fish_count = 0;
#define FISHING_TRUE_BIT		(0)
#define FISHING_FALSE_BIT		(8)
#define TV_FISHING_CREATE_COUNT	(5)		//�ȏ�A���̎��ɋt�ɂȂ�ƍ쐬����
#define TV_FISHING_COUNT_MAX	(0xff)	//�ȏ�A����������쐬����
//--------------------------------------------------------------------
//					�ނ�̘A�������A���s�̐���ۑ�
//--------------------------------------------------------------------
void TVFishingCount(u8 ans)
{
	if( ans )	//����
	{
		//���s���Â��Đ���������
		if( ((tv_fish_count) >> FISHING_FALSE_BIT) >= TV_FISHING_CREATE_COUNT )	TVFishingDataSet();

		tv_fish_count&=0x00ff;

		//�A��������TV_FISHING_COUNT_MAX�ɂȂ��Ă�����
		if( tv_fish_count != TV_FISHING_COUNT_MAX )
		{
			tv_fish_count+=(1 << FISHING_TRUE_BIT);
		}

			
	}else	//���s
	{
		//�������Â��Ď��s������
		if( ((tv_fish_count)&0x00ff) >= TV_FISHING_CREATE_COUNT )	TVFishingDataSet();

		tv_fish_count&=0xff00;

		//�A��������TV_FISHING_COUNT_MAX�ɂȂ��Ă�����
		if( ((tv_fish_count) >> FISHING_FALSE_BIT) != TV_FISHING_COUNT_MAX )
		{
			tv_fish_count+=(1 << FISHING_FALSE_BIT);
		}
	}

	return;
}

//--------------------------------------------------------------------
//						�ނ肵�܂���f�[�^�Z�b�g
//--------------------------------------------------------------------
void TVFishingDataSet()
{
	//u16 * res;
	TV_Sec_Fishing_Work * p;

	tv_akino = TVDataPartnerAkiChk(Fld.tv_event);
	if( tv_akino == -1 )	return;	//�󂫂��Ȃ�

	if( TVSecDataTypeChk(TV_FISHING_TYPE1) == 1 ) return;

	p = (TV_Sec_Fishing_Work *)&Fld.tv_event[tv_akino];

    p->datatype		= TV_FISHING_TYPE1;
	p->watch_flag	= TV_WATCH_FLAG_OFF;

	p->true_count	= (u8)tv_fish_count;
	p->false_count	= (u8)((tv_fish_count) >> FISHING_FALSE_BIT);
	p->pokeno		= tv_fish_pokeno;

	PM_strcpy(p->player_name, MyData.my_name);// ���O

	TVSecretIDSet((TV_Sec_Shop_Work *)p);

	return;
}

//--------------------------------------------------------------------
//						�ނ����|�P�����i���o�[�Z�b�g
//--------------------------------------------------------------------
void TVFishingPokeNoSet(u16 no)
{
	tv_fish_pokeno = no;				//�ۑ�
	return;
}

//--------------------------------------------------------------------
//					����ɕ߂܂������̓��ɂ�����
//--------------------------------------------------------------------
void TVOneDay_Update(u16 days)
{
	TV_Sec_OneDay_Work *p;

	p = (TV_Sec_OneDay_Work*)&TV_TEMP_WORK;

	//�f�[�^�^�C�v��������
	if( p->datatype != TV_ONEDAY_TYPE1 ) return;

	if( p->count < TV_ONEDAY_CREATE_NUM )
	{
		TVDataClear(Fld.tv_event,TVDATA_MAX);		// �N���A����
	}else
	{
		TVOneDayDataSet();
	}

	return;
}

//--------------------------------------------------------------------
//					����ɕ߂܂������̃Z�b�g
//--------------------------------------------------------------------
void TVOneDayDataSet()
{
	TV_Sec_OneDay_Work * p;
	TV_Sec_OneDay_Work * from_p;

	from_p = (TV_Sec_OneDay_Work*)&TV_TEMP_WORK;

	//�����`�F�b�N
	if( TVRndChk(TV_ONEDAY_START_ON) ) return;

	tv_akino = TVDataPartnerAkiChk(Fld.tv_event);
	if( tv_akino == -1 )	return;		//�󂫂��Ȃ�

	if( TVSecDataTypeChk(TV_ONEDAY_TYPE1) == 1 ) return;

	p = (TV_Sec_OneDay_Work *)&Fld.tv_event[tv_akino];

	p->datatype = TV_ONEDAY_TYPE1;
	p->watch_flag = TV_WATCH_FLAG_OFF;

    p->count		= from_p->count;
	p->walk_count	= GetRecord(CNT_WALK) - from_p->walk_count;
    p->get_pokeno	= from_p->get_pokeno;
    p->pokeno		= from_p->pokeno;
	p->worldpos		= from_p->worldpos;

	PM_strcpy(p->player_name, MyData.my_name);// ���O

	TVSecretIDSet((TV_Sec_Shop_Work *)p);

	return;
}


