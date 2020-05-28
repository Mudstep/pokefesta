//====================================================================
//
//						�s�u���	01/11/29 nohara							
//
//						�֐��Q
//
/*	
					���f�[�^�ǉ������Ƃ���
					�ELetterTypeSelectChk()�ɒǉ�����


*/
//====================================================================
#include "common.h"
#include "intr.h"
#include "calctool.h"				//pp_rand
#include "script.h"					//ScriptParameter
#include "poketool.h"				//PokeNameData
#include "conscr.h"					//NORMAL_RANK
#include "ev_win.h"					//NORMAL
#include "item.h"					//ItemData
#include "kaiwa.h"					//KaiwaCopyWord
#include "kaiwa_com.h"				//KAIWA_CODE_BLANK
#include "tv_def.h"					//TV��define�Q
#include "letter.h"					//LetterNameDataSet
#include "zukan.h"					//Z_SEE_CHK
#include "sysflag.h"				//SYS_TV_HOME
#include "syswork.h"				//LOCALWORK3
#include "fld_tool.h"				//�����C���N���[�h���鎞�ɕK�v
#include "../evdata/maparray.h"		//�ڑ��p�}�b�vID��`
#include "message.h"				//���b�Z�[�W�t�@�C��
#include "fld_main.h"
#include "pm_str.h"
#include "scr_tool.h"				//GetTemotiBtlPokeNum
#include "..\sxydata\park_battle01.sym"	//INTERVW1_01_PARK_BATTLE01
#include "..\script\savework.h"		//WK_SCENE_BATTLE_TOWER


//====================================================================
//							extern�錾
//====================================================================
//fd_shop.c
extern MINEITEM TVItemData[];		//�������������̂�����


//====================================================================
//						�v���g�^�C�v�錾
//====================================================================
void TVContestRankGet(u8,u8);		//�R���e�X�g�����N���Z�b�g
void TVContestTypeGet(u8,u8);		//�R���e�X�g�^�C�v���Z�b�g
void ScrTVContestTypeGet();			//�X�N���v�g����Z�b�g����
void SetNumSelectBuf(u8,int);		//�n���ꂽ�l���w�肵��buf�ɃZ�b�g
void SetTotalMoneySelectBuf(u8,TV_Sec_Shop_Work*);//�n���ꂽ�s�u�f�[�^�̍��v���z�Z�b�g
u8 TVSecDataTypeChk(u8);			//�������쐬��������datatype�����݂��邩(�閧�^�̂�)
void TVShopDataSort();				//�������������f�[�^�𐔂��������Ƀ\�[�g����
void LetterTypeSelectChk();			//�莆�^�̃f�[�^�쐬	�����`�F�b�N
void TVLetterDataTypeChk(u8);		//(�莆�^�̂�)
void ScrInterViewTypeChk();			//�C���^�r���[�`�F�b�N
void ScrLetterPokeTypeChk();		//�|�P�����A���P�[�g�`�F�b�N
void ScrLetterRecentlyTypeChk();	//��������̂ł����ƃ`�F�b�N
void ScrLetterClubTypeChk();		//��D���N���u�C���^�r���[�`�F�b�N
void ScrLetterGymTypeChk();			//�W�����[�_�[���̃C���^�r���[�`�F�b�N
void ScrLetterNameTypeChk();		//�������f���`�F�b�N
void ScrLetterConTypeChk();			//�R���e�X�g�`�F�b�N
void ScrLetterTowerTypeChk();		//�o�g���^���[�`�F�b�N
void TVDataClear(TV_EVENT *,u8);	//�w�肵���ꏊ��TV�f�[�^���폜
void TVDataSort(TV_EVENT *);		//�󂫂������TV�f�[�^��O�ɂ߂�
u16 SetRndChoicePokeSelectBuf(u8,u16);//�|�P�����̖��O���w�肵��buf�ɃZ�b�g
u16 RndChoicePoke(u16);				//�w�肵���|�P�����i���o�[�ȊO���擾
void ScrTVDataHimselfAkiChk();		//�����f�[�^�̋󂢂Ă�ꏊ�����邩	
s8 TVDataHimselfAkiChk(TV_EVENT *);	//����TV�f�[�^�̋󂢂Ă���ꏊ��������
s8 TVDataPartnerAkiChk(TV_EVENT *);	//����TV�f�[�^�̋󂢂Ă���ꏊ��������
u8 TopPokeNickNameChk(u8);			//�|�P�����Ƀj�b�N�l�[�����t���Ă��邩
u8 ScrTopPokeNickNameChk();			//�擪�|�P�����Ƀj�b�N�l�[�����t���Ă��邩
u8 TVRndChk(u16);					//�����擾�Ŕ������邩
void GetLetterRndKaiwaWord(TV_Letter_Work*);//�ȈՉ�b���͂��烉���_����1���[�h���擾
u8 GetPokeNickNameBit(TV_Name_Work*);//�閧�^�������f���R�r�b�g�擾
void SetMojiNoSelectBuf(u8,u8,u8,u16,u16,TV_Name_Work*);//�w�肵���������ڂ�������
u16 GetNumKeta(int);				//�����擾
void ScrTrainerIDCmp();				//�g���[�i�[ID��r
u8 TVDataCopyTypeGet(u8);			//�ʐM�^�C�v�擾
u8 ScrTVDataTypeChk();				//�w�肵���s�u�f�[�^�����łɂ��邩
u32 GetMyDataID();					//ID���擾����
u8 TVMyHomeChk();					//����s�u������邩�`�F�b�N
void ScrNickNameChangeRecover();	//NameIn��̕��A����
void TVBattleTowerInterViewClear();	//�o�g���^���[�̃C���^�r���A�[�폜�A�V�[���N���A


//====================================================================
//								�ϐ�
//====================================================================
//scrcmd.c�ɂ�����̂Ɠ��e����
u8 * const tempbuf_list[] =
{
	StrTempBuffer0,
	StrTempBuffer1,
	StrTempBuffer2,
};


//====================================================================
//
//								�֐�
//
//====================================================================

//--------------------------------------------------------------------
//						�R���e�X�g�����N���Z�b�g
//--------------------------------------------------------------------
void TVContestRankGet(u8 buf, u8 rank)
{
	switch( rank )
	{
		case NORMAL_RANK:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[NORMAL] );
			break;
		case SUPER_RANK:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[SUPER] );
			break;
		case HYPER_RANK:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[HYPER] );
			break;
		case MASTER_RANK:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[MASTER] );
			break;
	};
}

//--------------------------------------------------------------------
//						�R���e�X�g�^�C�v���Z�b�g
//--------------------------------------------------------------------
void TVContestTypeGet(u8 buf, u8 type)
{
	switch( type )
	{
		case STYLE_CONTEST:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[KAKKOYOSA] );
			break;
		case BEAUTIFUL_CONTEST:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[UTSUKUSHISA] );
			break;
		case CUTE_CONTEST:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[KAWAISA] );
			break;
		case CLEVER_CONTEST:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[KASHIKOSA] );
			break;
		case STRONG_CONTEST:
			PM_strcpy( tempbuf_list[buf], msg_data_buf[TAKUMASHISA] );
			break;
	};
}

//--------------------------------------------------------------------
//					�R���e�X�g�^�C�v���Z�b�g(�X�N���v�g����)
//--------------------------------------------------------------------
void ScrTVContestTypeGet()
{
	TV_Con_Work *p;
	p = (TV_Con_Work *)&Fld.tv_event[ScriptParameter0];
	TVContestTypeGet(1, p->type);
}

//--------------------------------------------------------------------
//					�n���ꂽ�l���w�肵��buf�ɃZ�b�g����
//--------------------------------------------------------------------
void SetNumSelectBuf(u8 buf, int num)
{
	u16 keta = 0;
	keta = GetNumKeta(num);
	PM_NumMsgSet( tempbuf_list[buf], num, NUM_MODE_LEFT, keta);
}

//--------------------------------------------------------------------
//					�n���ꂽ�l�̌������擾����
//--------------------------------------------------------------------
u16 GetNumKeta(int num)
{
	if( num / 10 == 0 )	return 1;
	else if( num / 100 == 0 ) return 2;
	else if( num / 1000 == 0 ) return 3;
	else if( num / 10000 == 0 ) return 4;
	else if( num / 100000 == 0 ) return 5;
	else if( num / 1000000 == 0 ) return 6;
	else if( num / 10000000 == 0 ) return 7;
	else if( num / 100000000 == 0 ) return 8;

	return 1;	//���Ă͂܂�Ȃ�������P����Ԃ�
}

//--------------------------------------------------------------------
//	�n���ꂽ�s�u�f�[�^�̏ꏊ�̍��v���z���w�肵��buf�ɃZ�b�g����
//--------------------------------------------------------------------
void SetTotalMoneySelectBuf(u8 buf, TV_Sec_Shop_Work * p)
{
	u8 i;
	int max;
	max=0;

	for( i=0; i < 3 ;i++)
	{
		if( p->itemno[i] != 0 )
			max += (int)( ItemPriceGet( p->itemno[i] ) * p->num[i] );
	}

	//�������t���O������
	if( p->bargain_flag == 1 )
		SetNumSelectBuf( buf, (max >> 1) );
	else	
		SetNumSelectBuf( buf, max );
}
			
//--------------------------------------------------------------------
//			�������쐬��������datatype�����݂��邩(�閧�^�̂�)
//
//	�߂�l		0=���݂��Ȃ�
//				1=���݂���
//--------------------------------------------------------------------
u8 TVSecDataTypeChk(u8 type)
{
	u8 i;
	u32 id;
	TV_Sec_Shop_Work *p;
	p = (TV_Sec_Shop_Work *)&Fld.tv_event[0];

	//ID���擾����
	id = GetMyDataID();

	for(i=TVDATA_HIMSELF_MAX; i < TVDATA_MAX ;i++)
	{
		if( (p+i)->datatype == type )
		{
			if( ((id)&0xff) == (p+i)->trainer_id[0] &&
				(((id)>>8)&0xff) == (p+i)->trainer_id[1] )
			{
				return 1;	//�����쐬�f�[�^�����݂���
			}
		}
	}

	return 0;	//�����쐬�f�[�^�����݂��Ȃ�
}

//--------------------------------------------------------------------
//				�������������f�[�^�𐔂��������Ƀ\�[�g����
//--------------------------------------------------------------------
void TVShopDataSort()
{
	u16 item,num;
	u8  i,j;

	for( i=0; i < 2; i++ )
	{
		for( j=i+1; j < 3; j++ )
		{
			if( TVItemData[i].no < TVItemData[j].no )
			{
				item = TVItemData[i].item;
				num = TVItemData[i].no;

				TVItemData[i].item = TVItemData[j].item;
				TVItemData[i].no = TVItemData[j].no;

				TVItemData[j].item = item;
				TVItemData[j].no = num;
			}
		}
	}
}

//--------------------------------------------------------------------
//							(�莆�^�̂�)
//			����datatype���݂��Ȃ�������		0
//			����datatype���݂�����				1
//			����datatype(����Ȃ����)���݂�����폜���ă\�[�g����	0
//
//	AnswerWork�ɑ��	0=�莆����
//						1=�莆���Ȃ�
//--------------------------------------------------------------------
void TVLetterDataTypeChk(u8 type)
{
	u8 i;

	for(i=0; i < TVDATA_HIMSELF_MAX ;i++)
	{
		//�����莆�f�[�^�����݂���
		if( Fld.tv_event[i].datatype == type )
		{
			if(	Fld.tv_event[i].watch_flag == TV_WATCH_FLAG_ON )
			{
				//���Ă��Ȃ��f�[�^����������Ȃ�
				AnswerWork = 1;
				return;
			}else
			{
				//���Ă���f�[�^��������폜���Ă����ɍ��悤�ɂ���
				TVDataClear(Fld.tv_event,i);
				TVDataSort(Fld.tv_event);
				ScrTVDataHimselfAkiChk();
				return;
			}
		}
	}

	//�����莆�f�[�^�����݂��Ȃ��āA�󂫂����邩���ׂ�
	ScrTVDataHimselfAkiChk();

	return;
}

//--------------------------------------------------------------------
//				�莆�^�̃f�[�^�쐬	�����`�F�b�N
//
//			ScriptParameter1 = datatype�����ď����𕪂���
//--------------------------------------------------------------------
void LetterTypeSelectChk()
{
	AnswerWork = 0;

	switch( ScriptParameter1 )
	{
		case TV_POKEMON_TYPE1:
			ScrLetterPokeTypeChk();
			break;
		case TV_RECENTLY_TYPE1:
			ScrLetterRecentlyTypeChk();
			break;
		case TV_CLUB_TYPE1:
			ScrLetterClubTypeChk();
			break;
		case TV_GYMLEADER_TYPE1:
			ScrLetterGymTypeChk();
			break;
		case TV_NAME_TYPE1:
			ScrLetterNameTypeChk();
			break;
		case TV_CONTEST_TYPE1:
			ScrLetterConTypeChk();
			break;
		case TV_TOWER_TYPE1:
			ScrLetterTowerTypeChk();
			break;
	};
}

//--------------------------------------------------------------------
//		�X�N���v�g����Ă΂��(�莆�|�P�����A���P�[�g�`�F�b�N)
//
//			TVLetterDataTypeChk(TV_POKEMON_TYPE1);
//			ScrTVDataHimselfAkiChk();
//--------------------------------------------------------------------
void ScrLetterPokeTypeChk()
{
	TV_Letter_Work *p;

	TVLetterDataTypeChk(TV_POKEMON_TYPE1);
	if( AnswerWork == 0 ) 
	{
		PM_strcpy( StrTempBuffer0, 
				PokeNameData[PokeParaGet(&PokeParaMine[GetTemotiBtlPokeNum()],ID_monsno,0)] );

		//change 02/09/19
		//KaiwaTVWorkInit(KAIWA_MODE_HAGAKI,tv_akino);
		p = (TV_Letter_Work *)&Fld.tv_event[tv_akino];
		KaiwaWorkInit( (u16 *)&p->easytalk[0], 6 );
	}
}

//--------------------------------------------------------------------
//		�X�N���v�g����Ă΂��(�莆��������̂ł����ƃ`�F�b�N)
//
//			TVLetterDataTypeChk(TV_RECENTLY_TYPE1);
//			ScrTVDataHimselfAkiChk();
//--------------------------------------------------------------------
void ScrLetterRecentlyTypeChk()
{
	TV_Letter_Work *p;

	TVLetterDataTypeChk(TV_RECENTLY_TYPE1);
	if( AnswerWork == 0 ) 
	{
		//change 02/09/19
		//KaiwaTVWorkInit(KAIWA_MODE_HAGAKI,tv_akino);
		p = (TV_Letter_Work *)&Fld.tv_event[tv_akino];
		KaiwaWorkInit( (u16 *)&p->easytalk[0], 6 );
	}
}

//--------------------------------------------------------------------
//	�X�N���v�g����Ă΂��(�莆��D���N���u�C���^�r���[�`�F�b�N)
//
//			TVLetterDataTypeChk(TV_CLUB_TYPE1);
//			ScrTVDataHimselfAkiChk();
//--------------------------------------------------------------------
void ScrLetterClubTypeChk()
{
	TV_Club_Work *p;

	TVLetterDataTypeChk(TV_CLUB_TYPE1);

	if( AnswerWork == 0 )
	{
		PM_strcpy( StrTempBuffer0, 
				PokeNameData[PokeParaGet(&PokeParaMine[GetTemotiBtlPokeNum()],ID_monsno,0)] );
		PokeParaGet( &PokeParaMine[GetTemotiBtlPokeNum()], ID_nickname, StrTempBuffer1 );
		NickNameCutOff( StrTempBuffer1 );

		//change 02/09/19
		//KaiwaTVWorkInit(KAIWA_MODE_INTERVIEW,tv_akino);
		p = (TV_Club_Work *)&Fld.tv_event[tv_akino];
		KaiwaWorkInit( (u16 *)&p->easytalk_no[0], 2 );
	}
}

//--------------------------------------------------------------------
//	�X�N���v�g����Ă΂��(�莆�W�����[�_�[���̃C���^�r���[�`�F�b�N)
//--------------------------------------------------------------------
void ScrLetterGymTypeChk()
{
	AnswerWork = 1;
	return;
}

//--------------------------------------------------------------------
//			�X�N���v�g����Ă΂��(�莆�������f��)
//
//			TVLetterDataTypeChk(TV_NAME_TYPE1);
//			ScrTVDataHimselfAkiChk();
//--------------------------------------------------------------------
void ScrLetterNameTypeChk()
{
	TVLetterDataTypeChk(TV_NAME_TYPE1);
}

//--------------------------------------------------------------------
//			�X�N���v�g����Ă΂��(�莆�R���e�X�g)
//
//			TVLetterDataTypeChk(TV_CONTEST_TYPE1);
//			ScrTVDataHimselfAkiChk();
//--------------------------------------------------------------------
void ScrLetterConTypeChk()
{
	TV_Con_Work *p;

	TVLetterDataTypeChk(TV_CONTEST_TYPE1);

	if( AnswerWork == 0 ) 
	{
		//change 02/09/19
		//KaiwaTVWorkInit(KAIWA_MODE_HAGAKI,tv_akino);
		p = (TV_Con_Work *)&Fld.tv_event[tv_akino];
		KaiwaWorkInit( (u16 *)&p->easytalk_no[0], 2 );
	}
}

//--------------------------------------------------------------------
//			�X�N���v�g����Ă΂��(�莆�o�g���^���[)
//
//			TVLetterDataTypeChk(TV_TOWER_TYPE1);
//			ScrTVDataHimselfAkiChk();
//--------------------------------------------------------------------
void ScrLetterTowerTypeChk()
{
	TV_Tower_Work *p;

	TVLetterDataTypeChk(TV_TOWER_TYPE1);

	if( AnswerWork == 0 ) 
	{
		//change 02/09/19
		//KaiwaTVWorkInit(KAIWA_MODE_HAGAKI,tv_akino);
		p = (TV_Tower_Work *)&Fld.tv_event[tv_akino];
		KaiwaWorkInit( (u16 *)&p->easytalk_no, 1 );
	}
}

//--------------------------------------------------------------------
//		�w�肵���莝���|�P�����Ƀj�b�N�l�[�����t���Ă��邩
//
//	�߂�l	0=�t���ĂȂ�
//			1=�t���Ă�
//--------------------------------------------------------------------
u8 TopPokeNickNameChk(u8 no)
{
	//StrTempBuffer0�Ɋi�[����
	PokeParaGet( &PokeParaMine[no], ID_nickname, StrTempBuffer0 );

	if( PM_strcmp( (u8 *)PokeNameData[PokeParaGet( &PokeParaMine[no], ID_monsno, 0 )],
				StrTempBuffer0 ) == 0 )
	{
		return 0;
	}

	return 1;
}

//--------------------------------------------------------------------
//			�莝���擪�|�P�����Ƀj�b�N�l�[�����t���Ă��邩
//
//	�߂�l	0=�t���ĂȂ�
//			1=�t���Ă�
//--------------------------------------------------------------------
u8 ScrTopPokeNickNameChk()
{
	return TopPokeNickNameChk( GetTemotiBtlPokeNum() );
}

//--------------------------------------------------------------------
//					�w�肵���ꏊ��TV�f�[�^���폜
//--------------------------------------------------------------------
void TVDataClear(TV_EVENT *p, u8 no)
{
	u8		i;

	p[no].datatype		= TV_NODATA_TYPE1;
	p[no].watch_flag	= TV_WATCH_FLAG_OFF;

	for( i=0; i < 34 ;i++ )
		p[no].dmy[i] = 0;

	return;
}

//--------------------------------------------------------------------
//				�󂫂������TV�f�[�^��O�ɂ߂�(�\�[�g)
//	TV�������Ƃ��Ɉ�ԏ�̃f�[�^�ȊO��time_count=0�ɂȂ��Ă���Ƃ�����
//	�X�N���v�g��TV(BG)�����Ƃ��ɕ��������炱����ĂԂ悤�ɂ���
//--------------------------------------------------------------------
void TVDataSort(TV_EVENT *p)
{
	u8 i,j;

	//����TV�f�[�^
	for(i=0; i < TVDATA_HIMSELF_MAX-1 ;i++)
	{
		if( p[i].datatype == TV_NODATA_TYPE1 )	
		{
			for(j=i+1; j < TVDATA_HIMSELF_MAX ;j++)
			{
				if( p[j].datatype != TV_NODATA_TYPE1 )	
				{
					p[i]=p[j];
					TVDataClear(p,j);
					break;
				}
			}
		}
	}

	//����TV�f�[�^
	//���߂�for���ŁATVDATA_MAX-1�����Ă��Ȃ��̂ŁA
	//�v�f��23�̃f�[�^���󂢂Ă����炻���ɂ߂悤�Ƃ��邪
	//����for����TV_DATA_MAX��菬����������ŁA
	//23+1(i+1)��TV_DATA_MAX�Ɠ������Ȃ�̂ő��v�I(�����ɓ���Ȃ�)
	for(i=TVDATA_HIMSELF_MAX; i < TVDATA_MAX ;i++)
	{
		if( p[i].datatype == TV_NODATA_TYPE1 )	
		{
			for(j=i+1; j < TVDATA_MAX ;j++)
			{
				if( p[j].datatype != TV_NODATA_TYPE1 )	
				{
					p[i]=p[j];
					TVDataClear(Fld.tv_event,j);
					break;
				}
			}
		}
	}

	return;
}

//--------------------------------------------------------------------
//					�w�肵���|�P�����i���o�[�ȊO��
//	  �����_���Ŏ擾�����|�P�����̖��O���w�肵��buf�ɃZ�b�g����
//--------------------------------------------------------------------
u16 SetRndChoicePokeSelectBuf(u8 buf, u16 pokeno)
{
	u16 ans;
	ans = RndChoicePoke(pokeno);
	PM_strcpy(tempbuf_list[buf], PokeNameData[ans] );
	return ans;
}

//--------------------------------------------------------------------
//					�w�肵���|�P�����i���o�[�ȊO��
//					�����_���ȃ|�P�����i���o�[���擾����
//--------------------------------------------------------------------
u16 RndChoicePoke(u16 pokeno)
{
	u16 ans,hozon,z_pokeno=0;

	ans=(pp_rand() % KAIHATSU_NUM_MAX)+1;		//1-411
	hozon = ans;							//


	while(1)
	{
		//�S���|�P�����i���o�[
		z_pokeno = PokeMonsNo2ZukanNoGet( ans );

		if( ZukanCheck( z_pokeno, Z_SEE_CHK ) == 1 )
		{
			if( ans != pokeno )	break;
		}

		if( ans == 1 )	ans = KAIHATSU_NUM_MAX;
		else	ans--;

		//�|�P�����i���o�[�����������I��
		if( ans == hozon )	
		{
			ans = pokeno;	//��΂Ƀ|�`�G�i�����Ă���̂ł����ɂ��邱�Ƃ͂Ȃ�
			break;
		}
	};

	return ans;
}

//--------------------------------------------------------------------
//					�����f�[�^�ɋ󂫂����邩�`�F�b�N
//
//	tv_akino�ɑ��
//	(ScriptParameter2��tv_akino�������Ă���)
//
//	AnswerWork�Ɍ��ʂ�����	0=����
//							1=�Ȃ�
//--------------------------------------------------------------------
void ScrTVDataHimselfAkiChk()
{	
	tv_akino = TVDataHimselfAkiChk((TV_EVENT *)Fld.tv_event);
	ScriptParameter2 = tv_akino;		//�ȈՉ�b�p
	if( tv_akino == -1 )	
	{
		AnswerWork = 1;
		return;
	}

	AnswerWork = 0;
	return;
}

//--------------------------------------------------------------------
//				����TV�f�[�^�̋󂢂Ă���ꏊ��������
//
//	�߂�l		�󂫂�������	0�`TVDATA_HIMSELF_MAX�̐���
//				�󂫂��Ȃ�����  -1
//--------------------------------------------------------------------
s8 TVDataHimselfAkiChk(TV_EVENT *p)
{
	u8 i;

	for(i=0; i < TVDATA_HIMSELF_MAX ;i++)
	{
		if( (p+i)->datatype == TV_NODATA_TYPE1 )	
			return i;
	}

	return -1;	//�󂫂��Ȃ�
}

//--------------------------------------------------------------------
//				����TV�f�[�^�̋󂢂Ă���ꏊ��������
//
//	�߂�l		�󂫂�������	TVDATA_HIMSELF_MAX�`TVDATA_MAX�̐���
//				�󂫂��Ȃ�����  -1
//--------------------------------------------------------------------
s8 TVDataPartnerAkiChk(TV_EVENT *p)
{
	s8 i;

	for(i=TVDATA_HIMSELF_MAX; i < TVDATA_MAX ;i++)
	{
		if( (p+i)->datatype == TV_NODATA_TYPE1 )	
			return i;
	}

	return -1;	//�󂫂��Ȃ�
}

//--------------------------------------------------------------------
//					�������擾���ēn���ꂽ�l�Ƃ̔�r
//
//	�߂�l		��������	0
//				�������Ȃ�	1
//--------------------------------------------------------------------
u8 TVRndChk(u16 num)
{
	if( pp_rand() <= num )	return 0;
	else	return 1;
}

//--------------------------------------------------------------------
//			�ȈՉ�b���͂̒����烉���_����1���[�h���擾����
//--------------------------------------------------------------------
void GetLetterRndKaiwaWord(TV_Letter_Work * p)
{
	u8 rnd;
	rnd = ( pp_rand() % 6 );

	while(1)
	{
		if( rnd == 6 )	rnd = 0;
		if( p->easytalk[rnd] != KAIWA_CODE_BLANK )	break;
		rnd++;
	}

	KaiwaCopyWord( StrTempBuffer2, p->easytalk[rnd] );
	return;
}

#define MOJI_BUF_MAX (MONS_NAME_SIZE+EOM_SIZE)
//--------------------------------------------------------------------
//					�|�P�����̃j�b�N�l�[���f�[�^��
//					���v�f�[�^�̉��ʂR�r�b�g��Ԃ�
//--------------------------------------------------------------------
u8 GetPokeNickNameBit(TV_Name_Work *p)
{
	u16 max = 0;
	u8 i = 0;

	while( i < MOJI_BUF_MAX )
	{
		if( p->nickname[i]  == EOM_ )
		{
			break;
		}else
		{
			max+=p->nickname[i];	
		}

		i++;
	}

	return (max & 0x7);
}

//--------------------------------------------------------------------
//				�w�肵��buf�Ɏw�肵���������ڂ�������
//				
//	buf		StrTempBuffer�̉��Ԗڂ�
//	moijno	�������ڂ�
//	sw		0=������	1=��납��	2=������mojino��	3=��납��mojino��
//	work1	0=��l���̖��O	1=�j�b�N�l�[��	2=�|�P�����̎��
//	work2	�|�P�����i���o�[(work1=2�̎��̂ݕK�v)
//
//--------------------------------------------------------------------
void SetMojiNoSelectBuf(u8 buf, u8 mojino, u8 sw, u16 work1, u16 work2, TV_Name_Work * p )
{
	u8 setbuf[3],i;
	u16 max = 0;

	for( i=0; i < 3 ;i++ )
		setbuf[i] = EOM_;

	if( work1 == TV_NAME_MY )
	{
		max = PM_strlen( (const u8 *)p->player_name );

		if( sw == TV_NAME_NTOP )
		{
			setbuf[0] = p->player_name[mojino];
		}else if( sw == TV_NAME_NLAST )
		{
			setbuf[0] = p->player_name[max - mojino];
		}else if( sw == TV_NAME_TOP )
		{
			setbuf[0] = p->player_name[mojino];
			setbuf[1] = p->player_name[mojino+1];
		}else
		{
			setbuf[0] = p->player_name[max - (mojino+2)];
			setbuf[1] = p->player_name[max - (mojino+1)];
		}
	}else if( work1 == TV_NAME_NICK )
	{
		max = PM_strlen( (const u8 *)p->nickname );

		if( sw == TV_NAME_NTOP )
		{
			setbuf[0] = p->nickname[mojino];
		}else if( sw == TV_NAME_NLAST )
		{
			setbuf[0] = p->nickname[max - mojino];
		}else if( sw == TV_NAME_TOP )
		{
			setbuf[0] = p->nickname[mojino];
			setbuf[1] = p->nickname[mojino+1];
		}else
		{
			setbuf[0] = p->nickname[max - (mojino+2)];
			setbuf[1] = p->nickname[max - (mojino+1)];
		}
	}else
	{
		max = PM_strlen( (const u8 *)PokeNameData[work2] );

		if( sw == TV_NAME_NTOP )
		{
			setbuf[0] = PokeNameData[work2][mojino];
		}else if( sw == TV_NAME_NLAST )
		{
			setbuf[0] = PokeNameData[work2][max - mojino];
		}else if( sw == TV_NAME_TOP )
		{
			setbuf[0] = PokeNameData[work2][mojino];
			setbuf[1] = PokeNameData[work2][mojino+1];
		}else
		{
			setbuf[0] = PokeNameData[work2][max - (mojino+2)];
			setbuf[1] = PokeNameData[work2][max - (mojino+1)];
		}
	}

	PM_strcpy( tempbuf_list[buf], setbuf );
}

//--------------------------------------------------------------------
//					�w�肵���s�u�f�[�^�����łɂ��邩	
//					
//							���g�p�I�I�I
//--------------------------------------------------------------------
u8 ScrTVDataTypeChk()
{
	u8 i;
	
	for( i=0; i < TVDATA_HIMSELF_MAX ;i++ )
	{
		if( Fld.tv_event[i].datatype == ScriptParameter0 )
			return TRUE;
	}

	return FALSE;
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


//====================================================================
//
//							�������f��
//
//====================================================================
u8 ScrNickNameChangeChk();
void ScrNickNameChange();
void ScrSetMyPokeNick();

//--------------------------------------------------------------------
//						�j�b�N�l�[����ύX������
//
//	�߂�l	0=���ĂȂ�
//			1=����
//--------------------------------------------------------------------
u8 ScrNickNameChangeChk()
{
	//StrTempBuffer0�Ɋi�[����
	PokeParaGet( &PokeParaMine[ScriptParameter0], ID_nickname, StrTempBuffer0 );

	if( PM_strcmp( (u8 *)StrTempBuffer2, StrTempBuffer0 ) == 0 )
	{
		return 0;
	}

	//TV�f�[�^�쐬
	LetterNameDataSet();
	return 1;
}

//--------------------------------------------------------------------
//							NameIn�Ăяo��
//
//					StrTempBuffer2�ɑ�����Ă���
//--------------------------------------------------------------------
void ScrNickNameChange()
{
	//�ύX����O�̃j�b�N�l�[����StrTempBuffer2�Ɋi�[����
	PokeParaGet( &PokeParaMine[ScriptParameter0], ID_nickname, StrTempBuffer2 );

	//StrTempBuffer1�Ɋi�[����
	PokeParaGet( &PokeParaMine[ScriptParameter0], ID_nickname, StrTempBuffer1 );

	NameIn( NAMEIN_POKE_NICK,
			StrTempBuffer1,
			PokeParaGet( &PokeParaMine[ScriptParameter0], ID_monsno, 0 ),
			PokeSexGet(&PokeParaMine[ScriptParameter0]),
			PokeParaGet( &PokeParaMine[ScriptParameter0], ID_personal_rnd, 0 ),
			ScrNickNameChangeRecover);
}

//--------------------------------------------------------------------
//							�j�b�N�l�[���Z�b�g
//--------------------------------------------------------------------
void ScrNickNameChangeRecover()
{
	PokeParaPut( &PokeParaMine[ScriptParameter0], ID_nickname, StrTempBuffer1 );
	FieldScriptRecover();
}

//--------------------------------------------------------------------
//						buf�Ƀj�b�N�l�[���Z�b�g
//--------------------------------------------------------------------
void ScrSetMyPokeNick()
{
	PokeParaGet( &PokeParaMine[ScriptParameter0], ID_nickname, StrTempBuffer0 );
	NickNameCutOff( StrTempBuffer0 );
}

//--------------------------------------------------------------------
//						�g���[�i�[ID��r
//
//	����	AnswerWork = 0
//	�Ⴄ	AnswerWork = 1
//--------------------------------------------------------------------
void ScrTrainerIDCmp()
{
	u32 id;

	id = GetMyDataID();

	if( id == PokeParaGet( &PokeParaMine[ScriptParameter0], ID_id_no, 0) )
		AnswerWork = 0;
	else
		AnswerWork = 1;

}

//--------------------------------------------------------------------
//						�ʐM�^�C�v�擾
//
//	����	�f�[�^�^�C�v
//	�߂�l	�ʐM�^�C�v
//--------------------------------------------------------------------
u8 TVDataCopyTypeGet(u8 type)
{
	if( type == TV_NODATA_TYPE1 ) return NOCOPY_TYPE1;
	if( LETTER_TYPE_START <= type && type < LETTER_TYPE_MAX ) return CUT_TYPE1;
	if( SEC_TYPE_START <= type && type < SEC_TYPE_MAX ) return CUT_TYPE2;
	if( TIME_TYPE_START <= type && type < TIME_TYPE_MAX ) return CUT_TIME_TYPE;

	return NOCOPY_TYPE1;
}

//--------------------------------------------------------------------
//							ID���擾����	
//
//				id[0]|id[1]<<8|id[2]|<<16|id[3]<<24
//--------------------------------------------------------------------
u32 GetMyDataID()
{
	return ( MyData.ID[3] << 24 | MyData.ID[2] << 16 | MyData.ID[1] << 8 | MyData.ID[0] );
}

//--------------------------------------------------------------------
//							����̂s�u�`�F�b�N
//
//	�߂�l	0 = ����s�u������Ȃ�
//			1 = ����s�u�������
//			2 = ����s�u�����点�Ȃ�
//--------------------------------------------------------------------
u8 TVMyHomeChk()
{
	//�~�V���^�E������Ȃ�������
	if( Fld.MapID.div_id != T101 )	return FALSE;

	//�����Ȃ�������
	if( MyData.my_sex == MALE )	//�j
	{
		if( Fld.MapID.map_id != T101_R0101 )
			return FALSE;
	}else
	{
		if(	Fld.MapID.map_id != T101_R0201 )
			return FALSE;
	}

	//�ړ��|�P�����j���[�X�𗬂�Ă�����
	if( EventFlagCheck( SYS_MOVE_POKE_NEWS ) == TRUE )	return TRUE;

	//����s�u�t���O�������Ă�����
	if( EventFlagCheck( SYS_TV_HOME ) == TRUE )	
	{
		//�s�u������Ȃ��Łu�s�A�m�̏�ŁE�E�E�v
		return 2;
	}

	//SYS_TV_HOME�������Ă��Ȃ���Ԃłs�u�������t���[�Ō��邱�Ƃ͂Ȃ��I�I
	//�����ɂ���̂͋����C�x���g��(�����s�u�ɏo�Ă���)�s�u�����点�鎞
	//�����C�x���g�I�����SYS_TV_HOME�����̂ł��̏����ɂ͓���Ȃ�
	return TRUE;
}

//--------------------------------------------------------------------
//				TV�������Ȃ����́u�p�p�}�}�v�Z�b�g
//
//				LOCALWORK3�g�p��(�l���ύX���Ă��悢)
//				= 0�܂������_�����߂Ă��Ȃ�
//				= 1�ɂȂ��Ă�����}�}
//				= 2�ɂȂ��Ă�����p�p
//				= �ȊO % 2 + 1��1,2�����߂�
//--------------------------------------------------------------------
void TVNoDataMsgSet()
{
	u16 ans;

	//�~�V���^�E����������
	if( Fld.MapID.div_id == T101 )
	{
		//���������u�}�}�v�ɂ���
		if( MyData.my_sex == MALE )	//�j
		{
			if( Fld.MapID.map_id == T101_R0101 )
			{
				PM_strcpy( StrTempBuffer0, str_mama );
				SetEventWorkValue(LOCALWORK3,1);
			}
		}else
		{
			if( Fld.MapID.map_id == T101_R0201 )
			{
				PM_strcpy( StrTempBuffer0, str_mama );
				SetEventWorkValue(LOCALWORK3,1);
			}
		}
	}


	//�O��̃����_���������p��(�����ɂł͂Ȃ�)
	if( GetEventWorkValue(LOCALWORK3) == 1 )				//�}�}
	{
		PM_strcpy( StrTempBuffer0, str_mama );
		return;

	}else if( GetEventWorkValue(LOCALWORK3) == 2 )			//�p�p
	{
		PM_strcpy( StrTempBuffer0, str_papa );
		return;

	}else if( GetEventWorkValue(LOCALWORK3) > 2 )			//�ȊO
	{
		if( (GetEventWorkValue(LOCALWORK3) % 2 + 1) == 1 )
			PM_strcpy( StrTempBuffer0, str_mama );			//�}�}
		else
			PM_strcpy( StrTempBuffer0, str_papa );			//�p�p

		return;
	}


	//�����_���Łu�p�p�}�}�v���Z�b�g����
	ans = (pp_rand() % 2);

	if( ans )	
	{
		PM_strcpy( StrTempBuffer0, str_mama );		//�}�}
		SetEventWorkValue(LOCALWORK3,1);
	}else
	{
		PM_strcpy( StrTempBuffer0, str_papa );			//�p�p
		SetEventWorkValue(LOCALWORK3,2);
	}

	return;
}

//--------------------------------------------------------------------
//	�o�g���^���[�ŕs���ȏI�����������ɃC���^�r���A�[���폜����	
//	�V�[�����O�ɃN���A����
//--------------------------------------------------------------------
void TVBattleTowerInterViewClear();
void TVBattleTowerInterViewClear()
{
	SetEventWorkValue( WK_SCENE_BATTLE_TOWER, 0 );
	EvObjDelId( INTERVW1_01_PARK_BATTLE01, (u8)Fld.MapID.map_id, (u8)Fld.MapID.div_id );
	return;
}


