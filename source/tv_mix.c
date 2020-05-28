//====================================================================
//
//
//					�ʐM���̂s�u�f�[�^�����鏈��	01/12/12 nohara
//
//
/*
						��datatype��ǉ���������
						�ETVDataZukanCheck�ɒǉ�����


*/
//====================================================================
#include "common.h"
#include "intr.h"
#include "pokesio.h"				//GetSioConnectId
#include "tv_def.h"					//TV��define�Q
#include "tv_tool.h"				//�֐��Q
#include "cm.h"						//CMDATA_MAX
#include "zukan.h"					//Z_SEE_CHK
#include "poketool.h"				//PokeMonsNo2ZukanNoGet
#include "ev_flag.h"				//EventFlagCheck
#include "sysflag.h"				//SYS_GAME_CLEAR


//====================================================================
//						�v���g�^�C�v�錾
//====================================================================
void TVDataMixer(void *,u32,u8);
void TVDataMixerMain(TV_EVENT *,TV_EVENT *,TV_EVENT *,TV_EVENT *);
static u8 TVDataCopyChk(TV_EVENT **,TV_EVENT **,u8);
static u8 TVDataLetterTypeCopy(TV_Letter_Work *,TV_Letter_Work *,u8);
static u8 TVDataSecretTypeCopy(TV_Sec_Btl_Work *,TV_Sec_Btl_Work *,u8);
static u8 TVDataTimeTypeCopy(TV_Generating_Work *,TV_Generating_Work *,u8);
static s8 MixTVCopyTypeChk(TV_EVENT *);
static void	TVDataVerCheck(u8);
static void	TVDataZukanCheck();
static void	TVDataZukanSet(u16,u8);
static void	CMDataVerCheck();
static void	TVDataGameClearCheck();
static void	CMDataGameClearCheck();
void TVDataHimselfLetterWatchFlagOff();
static void	TVDataAkiKakuho();

//====================================================================
//							�ϐ�
//====================================================================
static s8 mix_akino;		//�ʐM�ő����Ă����f�[�^�̃R�s�[����ꏊ

#define MIX_TV_DATA_SIZE    (sizeof(Fld.tv_event))            // �ʐM���̃f�[�^�T�C�Y
#define MIX_TV_TMP_BUFFER   (TV_EVENT*)(&(UserWork[0x7000]))  // �ʐM���ɑS�f�[�^���R�s�[���Ă����ꎞ�o�b�t�@


//====================================================================
//							�֐�
//====================================================================

//--------------------------------------------------------------------
//
//						�n�߂ɌĂ΂��
//
//				�����Ă����f�[�^�̏��Ԃ����킹��
//
//--------------------------------------------------------------------
void TVDataMixer( void *data, u32 size, u8 my_id )
{
	TV_EVENT *from_p;
	u8 i;

	for(i = 0; i < 4; i++){
		memcpy((u8*)MIX_TV_TMP_BUFFER+i*MIX_TV_DATA_SIZE, (u8*)data+i*size, MIX_TV_DATA_SIZE);
	}

	from_p = (TV_EVENT*)MIX_TV_TMP_BUFFER;

	//���Ԃ𐮂���
	switch( my_id )
	{
		case 0:
			TVDataMixerMain( (TV_EVENT *)Fld.tv_event,
					(TV_EVENT *)(from_p+TV_WORK_MAX), 
					(TV_EVENT *)(from_p+TV_WORK_MAX*2),
					(TV_EVENT *)(from_p+TV_WORK_MAX*3) );
			break;
		case 1:
			TVDataMixerMain( (TV_EVENT *)from_p,
					(TV_EVENT *)Fld.tv_event,
					(TV_EVENT *)(from_p+TV_WORK_MAX*2),
					(TV_EVENT *)(from_p+TV_WORK_MAX*3) );
			break;
		case 2:
			TVDataMixerMain( (TV_EVENT *)from_p,
					(TV_EVENT *)(from_p+TV_WORK_MAX),
					(TV_EVENT *)Fld.tv_event,
					(TV_EVENT *)(from_p+TV_WORK_MAX*3) );
			break;
		case 3:
			TVDataMixerMain( (TV_EVENT *)from_p,
					(TV_EVENT *)(from_p+TV_WORK_MAX),
					(TV_EVENT *)(from_p+TV_WORK_MAX*2),
					(TV_EVENT *)Fld.tv_event );
			break;
	};

	//�����鏈�����I��������\�[�g��������
	TVDataSort(Fld.tv_event);

	//����f�[�^�̑��������āA����f�[�^�ɂT���̋󂫂����
	TVDataAkiKakuho();
	
	//�V���b�t������
	//TVDataShuffle();

	//TVDataAkiKakuho���I��������\�[�g��������
	TVDataSort(Fld.tv_event);

	//�}�ӂɍڂ��Ă��邩�`�F�b�N
	TVDataZukanCheck();

	//�Q�[���N���A���Ă��邩�̃`�F�b�N
	TVDataGameClearCheck();
}

//--------------------------------------------------------------------
//
//							���C��
//
//--------------------------------------------------------------------
void TVDataMixerMain( TV_EVENT *agb1, TV_EVENT *agb2, TV_EVENT *agb3, TV_EVENT *agb4 )
{
	TV_EVENT **p_list[4];
	u8 i,j,ans;
	static u8 num,end_chk = 0;

	p_list[0] = &agb1;
	p_list[1] = &agb2;
	p_list[2] = &agb3;
	p_list[3] = &agb4;

	//�ʐM�l�����擾
	num = GetConnectSioMenberNum();

	while(1)
	{
		//�l����
		for( j=0; j < num ;j++ )
		{
			if(j == 0)	end_chk = 0;		//�I���J�E���^�[�N���A

			//�n����f�[�^�̏ꏊ�𒲂ׂ�
			mix_akino = MixTVCopyTypeChk(*p_list[j]);
			if( mix_akino == -1 )
			{
				end_chk++;
				if( end_chk == num ) return;
			}else
			{
				for( i=0; i < (num-1) ;i++ )
				{
					tv_akino = TVDataPartnerAkiChk(*p_list[(j+i+1)%num]);
					if( tv_akino != -1 )
					{
						ans = TVDataCopyChk( p_list[(j+i+1)%num], p_list[j], ((j+i+1)%num) );
	
						//�R�s�[�o������break����
						if( ans == 1 ) break;
					}
				}

				//�N���R�s�[���Ȃ������ꍇ�͍폜����
				if( i == (num-1) ) 
				{
					TVDataClear(*p_list[j],(u8)mix_akino);
				}
			}
		}
	}

	return;
}

//--------------------------------------------------------------------
//
//					�ʐM�^�C�v�����ď����𕪂���
//
//--------------------------------------------------------------------
static u8 TVDataCopyChk(TV_EVENT **p, TV_EVENT **pp, u8 id)
{
	u8 ans,type;
	TV_EVENT *to_p;
	TV_EVENT *from_p;
	to_p = *p;
	from_p = *pp;
	ans = 0;


	type = TVDataCopyTypeGet( from_p[mix_akino].datatype );
	switch( type )
	{
		//(�莆�^)�J�b�g���y�[�X�g����
		case CUT_TYPE1:
			ans =TVDataLetterTypeCopy( (TV_Letter_Work *)(to_p)+tv_akino,
					(TV_Letter_Work *)(from_p)+mix_akino, id );
			break;

		//(�閧�^)�J�b�g���y�[�X�g����
		case CUT_TYPE2:
			ans =TVDataSecretTypeCopy( (TV_Sec_Btl_Work *)(to_p)+tv_akino,
					(TV_Sec_Btl_Work *)(from_p)+mix_akino, id );
			break;

		//(���Ԍ^)�J�b�g���y�[�X�g����
		case CUT_TIME_TYPE:
			ans =TVDataTimeTypeCopy( (TV_Generating_Work *)(to_p)+tv_akino,
					(TV_Generating_Work *)(from_p)+mix_akino, id );
			break;

		//(���A�^)��΂ɃR�s�[����
		case RARE_TYPE1:
			//ans = ???;
			break;
	};

	if( ans == 1 )
	{
		TVDataClear((TV_EVENT *)from_p,(u8)mix_akino);
		return 1;
	}

	return 0;
}

//--------------------------------------------------------------------
//							�莆�^
//
//				�R�s�[����K�v�����邩�`�F�b�N����
//				����̃J�Z�b�g��TV�f�[�^���R�s�[����
//
//	�߂�l	0 = �f�[�^���R�s�[���Ȃ�����
//			1 = �f�[�^���R�s�[����
//--------------------------------------------------------------------
static u8 TVDataLetterTypeCopy(TV_Letter_Work *p, TV_Letter_Work *pp, u8 id)
{
	u32	get_id;

	get_id = GetSioTrainerId(id);

	//�O�̃g���[�i�[ID�Ɠ�����
	if( ((get_id)&0xff) == pp->before_id[0] &&
		 (((get_id)>>8)&0xff) == pp->before_id[1] ) return 0;

	//�O�̃g���[�i�[ID�Ƀf�[�^ID���R�s�[���āA
	//�f�[�^ID�ɍ���ROM�̃g���[�i�[ID��������
	pp->before_id[0] = pp->data_id[0];
	pp->before_id[1] = pp->data_id[1];
	pp->data_id[0] = (get_id & 0xff);
	pp->data_id[1] = ((get_id >> 8 )& 0xff);

	*(p)=*(pp);			//�f�[�^���R�s�[����
	p->watch_flag = TV_WATCH_FLAG_ON;	//������Ԃɂ���

	return 1;
}


//--------------------------------------------------------------------
//							�閧�^
//
//				�R�s�[����K�v�����邩�`�F�b�N����
//				����̃J�Z�b�g��TV�f�[�^���R�s�[����
//
//	�߂�l	0 = �f�[�^���R�s�[���Ȃ�����
//			1 = �f�[�^���R�s�[����
//--------------------------------------------------------------------
static u8 TVDataSecretTypeCopy(TV_Sec_Btl_Work *p, TV_Sec_Btl_Work *pp, u8 id)
{
	u32	get_id;

	get_id = GetSioTrainerId(id);

	//�O�̃g���[�i�[ID�Ɠ�����
	if( ((get_id)&0xff) == pp->before_id[0] &&
		 (((get_id)>>8)&0xff) == pp->before_id[1] ) return 0;

	//�����ID��������
	if( ((get_id)&0xff) == pp->trainer_id[0] &&
		 (((get_id)>>8)&0xff) == pp->trainer_id[1] ) return 0;

	//�O�̃g���[�i�[ID�Ƀf�[�^ID���R�s�[���āA
	//�f�[�^ID�ɍ���ROM�̃g���[�i�[ID��������
	pp->before_id[0] = pp->data_id[0];
	pp->before_id[1] = pp->data_id[1];
	pp->data_id[0] = (get_id & 0xff);
	pp->data_id[1] = ((get_id >> 8 )& 0xff);

	*(p)=*(pp);				//�f�[�^���R�s�[����
	p->watch_flag = TV_WATCH_FLAG_ON;	//������Ԃɂ���
	return 1;
}
//--------------------------------------------------------------------
//							���Ԍ^
//
//				�R�s�[����K�v�����邩�`�F�b�N����
//				����̃J�Z�b�g��TV�f�[�^���R�s�[����
//
//	�߂�l	0 = �f�[�^���R�s�[���Ȃ�����
//			1 = �f�[�^���R�s�[����
//--------------------------------------------------------------------
static u8 TVDataTimeTypeCopy(TV_Generating_Work *p, TV_Generating_Work *pp, u8 id)
{
	u32	get_id;

	get_id = GetSioTrainerId(id);

	//�O�̃g���[�i�[ID�Ɠ�����
	if( ((get_id)&0xff) == pp->before_id[0] &&
		 (((get_id)>>8)&0xff) == pp->before_id[1] ) return 0;

	//�O�̃g���[�i�[ID�Ƀf�[�^ID���R�s�[���āA
	//�f�[�^ID�ɍ���ROM�̃g���[�i�[ID��������
	pp->before_id[0] = pp->data_id[0];
	pp->before_id[1] = pp->data_id[1];
	pp->data_id[0] = (u8)((get_id)&0xff);
	pp->data_id[1] = (u8)(((get_id)>>8)&0xff);


	*(p)=*(pp);			//�f�[�^���R�s�[����
	p->watch_flag = TV_WATCH_FLAG_ON;			//������Ԃɂ���
	p->time_count = GENERATING_INCUBATION_TIME;	//�����������Z�b�g����

	return 1;
}

//--------------------------------------------------------------------
//					�n���f�[�^�̏ꏊ��Ԃ�
//
//	�߂�l		������	0�`TVDATA_MAX�̐���
//				�Ȃ�����  -1
//--------------------------------------------------------------------
static s8 MixTVCopyTypeChk(TV_EVENT *p)
{
	u8 i;

	for(i=0; i < TVDATA_MAX ;i++)
	{
		if( (p+i)->watch_flag == TV_WATCH_FLAG_OFF )
		{
			if( TV_TYPE_START <= (p+i)->datatype &&
					(p+i)->datatype < TV_TYPE_END )
				return i;
		}
	}

	return -1;			
}

//--------------------------------------------------------------------
//�}�ӂ����Ă܂����ĂȂ��|�P�����f�[�^��������s�u��������Ԃɂ���
//--------------------------------------------------------------------
static void	TVDataZukanCheck()
{
	u16 i,pokeno;

	//TV�f�[�^
	for(i=0; i < TVDATA_MAX ;i++)
	{
		switch( Fld.tv_event[i].datatype )	
		{
			case TV_NODATA_TYPE1:
			case TV_SHOP_TYPE1:
			case TV_GENERATING_TYPE1:
			case TV_RECENTLY_TYPE1:
				break;
			case TV_POKEMON_TYPE1:
				pokeno = ((TV_Letter_Work*)(&(Fld.tv_event[i])))->inside_data;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_CLUB_TYPE1:
				pokeno = ((TV_Club_Work*)(&(Fld.tv_event[i])))->pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_GYMLEADER_TYPE1:
				pokeno = ((TV_GYM_Work*)(&(Fld.tv_event[i])))->pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_NAME_TYPE1:
				pokeno = ((TV_Name_Work*)(&(Fld.tv_event[i])))->pokeno;
				TVDataZukanSet( pokeno, i );

				pokeno = ((TV_Name_Work*)(&(Fld.tv_event[i])))->r_pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_CONTEST_TYPE1:
				pokeno = ((TV_Con_Work*)(&(Fld.tv_event[i])))->pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_TOWER_TYPE1:
				pokeno = ((TV_Tower_Work*)(&(Fld.tv_event[i])))->pokeno;
				TVDataZukanSet( pokeno, i );

				pokeno = ((TV_Tower_Work*)(&(Fld.tv_event[i])))->enemy_pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_GETPOKE_TYPE1:
				pokeno = ((TV_Sec_Btl_Work*)(&(Fld.tv_event[i])))->get_pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_FAILED_TYPE1:
				pokeno = ((TV_Sec_Get_Failed_Work*)(&(Fld.tv_event[i])))->pokeno;
				TVDataZukanSet( pokeno, i );

				pokeno = ((TV_Sec_Get_Failed_Work*)(&(Fld.tv_event[i])))->failed_pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_FISHING_TYPE1:
				pokeno = ((TV_Sec_Fishing_Work*)(&(Fld.tv_event[i])))->pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			case TV_ONEDAY_TYPE1:
				pokeno = ((TV_Sec_OneDay_Work*)(&(Fld.tv_event[i])))->pokeno;
				TVDataZukanSet( pokeno, i );

				pokeno = ((TV_Sec_OneDay_Work*)(&(Fld.tv_event[i])))->get_pokeno;
				TVDataZukanSet( pokeno, i );
				break;
			default:
				TVDataVerCheck( (u8)i );
				break;
		}
	}

	return;
}

//--------------------------------------------------------------------
//		ROM�ɂȂ��f�[�^�^�C�v�������ꍇ�́A�s�u��������Ԃɂ���
//--------------------------------------------------------------------
static void	TVDataVerCheck(u8 no)
{
	Fld.tv_event[no].watch_flag = TV_WATCH_FLAG_OFF;
	return;
}

//--------------------------------------------------------------------
//			�}�Ӄ`�F�b�N�����āAwatch_flag������Ȃ���Ԃɂ���
//--------------------------------------------------------------------
static void	TVDataZukanSet(u16 pokeno, u8 no)
{
	s8 ans = 0;
	u16 z_pokeno = 0;

	//�|�P�����i���o�[��S���ɂ���
	z_pokeno = PokeMonsNo2ZukanNoGet( pokeno );
	ans = ZukanCheck( z_pokeno, Z_SEE_CHK );
	if( ans == 0 )	Fld.tv_event[no].watch_flag = TV_WATCH_FLAG_OFF;
	return;
}

//--------------------------------------------------------------------
//	�Q�[���N���A���Ă��Ȃ�������watch_flag������Ȃ���Ԃɂ���
//--------------------------------------------------------------------
static void	TVDataGameClearCheck()
{
	u16 i;

	//�Q�[���N���A���Ă�����
	if( EventFlagCheck( SYS_GAME_CLEAR ) == TRUE ) return;

	//TV�f�[�^
	for(i=0; i < TVDATA_MAX ;i++)
	{
		switch( Fld.tv_event[i].datatype )	
		{
			case TV_TOWER_TYPE1:
				Fld.tv_event[i].watch_flag = TV_WATCH_FLAG_OFF;
				break;
			case TV_GENERATING_TYPE1:
				Fld.tv_event[i].watch_flag = TV_WATCH_FLAG_OFF;
				break;
		}
	}

	return;
}

//--------------------------------------------------------------------
//			�����̂s�u�f�[�^�̎����f�[�^�ɂ���莆�^��
//			�S�Č������(�ʐM�o������)�ɂ���
//--------------------------------------------------------------------
void TVDataHimselfLetterWatchFlagOff()
{
	u8 i,type;

	for( i=0; i < TVDATA_HIMSELF_MAX ;i++ )
	{
		type = TVDataCopyTypeGet( Fld.tv_event[i].datatype );

		//�莆�^��������
		if( type == CUT_TYPE1 )	
			Fld.tv_event[i].watch_flag = TV_WATCH_FLAG_OFF;
	}

	return;
}

//--------------------------------------------------------------------
//		����f�[�^�̑��������āA����f�[�^�ɂT���̋󂫂����
//--------------------------------------------------------------------
#define	AKI_KAKUHO_SIZE		TVDATA_HIMSELF_MAX
static void TVDataAkiKakuho()
{
	s8 i,count = 0;

	for( i=TVDATA_HIMSELF_MAX; i < TVDATA_MAX ;i++ )
	{
		if( Fld.tv_event[i].datatype == TV_NODATA_TYPE1 )
			count++;
	}

	for (i=0; i < AKI_KAKUHO_SIZE - count; i++ ) {
		TVDataClear( Fld.tv_event, i+TVDATA_HIMSELF_MAX );
	}

	return;
}


/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


void CMDataMixer(void *,u32,u8);
static u8 CMDataInterViewTypeCopy(TV_CM *,TV_CM *,s8);
void CMDataMixerMain(TV_CM *,TV_CM *,TV_CM *,TV_CM *);
static void CMDataCopyChk(TV_CM **,TV_CM **);
static s8 MixCMCopyTypeChk(TV_CM *,u8);

#define MIX_CM_DATA_SIZE    (sizeof(Fld.tv_cm))            // �ʐM���̃f�[�^�T�C�Y
#define MIX_CM_TMP_BUFFER   (TV_CM*)(&(UserWork[0x7000]))  // �ʐM���ɑS�f�[�^���R�s�[���Ă����ꎞ�o�b�t�@
//--------------------------------------------------------------------
//			�����Ă����f�[�^�Ǝ����̃f�[�^���~�b�N�X����
//--------------------------------------------------------------------
void CMDataMixer( void * data, u32 size, u8 my_id )
{
	TV_CM *from_p;
	u8 i;

	for(i = 0; i < 4; i++){
		memcpy((u8*)MIX_CM_TMP_BUFFER+i*MIX_CM_DATA_SIZE, (u8*)data+i*size, MIX_CM_DATA_SIZE);
	}

	from_p = (TV_CM*)MIX_CM_TMP_BUFFER;

	//���Ԃ𐮂���
	switch( my_id )
	{
		case 0:
			CMDataMixerMain( Fld.tv_cm,
					(TV_CM *)(from_p+CMDATA_MAX), 
					(TV_CM *)(from_p+CMDATA_MAX*2),
					(TV_CM *)(from_p+CMDATA_MAX*3) );
			break;
		case 1:
			CMDataMixerMain( (TV_CM *)from_p,
					Fld.tv_cm,
					(TV_CM *)(from_p+CMDATA_MAX*2),
					(TV_CM *)(from_p+CMDATA_MAX*3) );
			break;
		case 2:
			CMDataMixerMain( (TV_CM *)from_p,
					(TV_CM *)(from_p+CMDATA_MAX),
					Fld.tv_cm,
					(TV_CM *)(from_p+CMDATA_MAX*3) );
			break;
		case 3:
			CMDataMixerMain( (TV_CM *)from_p,
					(TV_CM *)(from_p+CMDATA_MAX),
					(TV_CM *)(from_p+CMDATA_MAX*2),
					Fld.tv_cm );
			break;
	};

	CMDataVerCheck();					//ver�`�F�b�N
	CMDataGameClearCheck();				//�Q�[���N���A���Ă邩�`�F�b�N
}

//--------------------------------------------------------------------
//
//							���C��
//
//--------------------------------------------------------------------
void CMDataMixerMain( TV_CM *agb1, TV_CM *agb2, TV_CM *agb3, TV_CM *agb4 )
{
	TV_CM **p_list[4];
	u8 i,j,k;
	static u8 num;

	p_list[0] = &agb1;
	p_list[1] = &agb2;
	p_list[2] = &agb3;
	p_list[3] = &agb4;

	//�ʐM�l�����擾
	num = GetConnectSioMenberNum();

	for(  k=0; k < CMDATA_MAX ;k++ )
	{
		for( j=0; j < num ;j++ )
		{
			//�n����f�[�^�̏ꏊ�𒲂ׂ�
			mix_akino = MixCMCopyTypeChk(*p_list[j],k);
			if( mix_akino != -1 )
			{
				for( i=0; i < (num-1) ;i++ )
				{
					tv_akino = CMDataAkiChk((TV_CM *)*p_list[(j+i+1)%num]);
					if( tv_akino != -1 )
					{
						CMDataCopyChk( p_list[(j+i+1)%num], p_list[j] );
					}
				}
			}
		}
	}

	return;
}

//--------------------------------------------------------------------
//
//					�ʐM�^�C�v�����ď����𕪂���
//					CM�̓R�s�[�������݂��Ȃ�
//
//--------------------------------------------------------------------
static void CMDataCopyChk(TV_CM **p, TV_CM **pp)
{
	TV_CM *to_p;
	TV_CM *from_p;
	to_p = *p;
	from_p = *pp;

	CMDataInterViewTypeCopy( (TV_CM *)(to_p),(TV_CM *)(from_p)+mix_akino,tv_akino );

	return;
}

//--------------------------------------------------------------------
//						�C���^�r���[�^(CM)
//
//				�R�s�[����K�v�����邩�`�F�b�N����
//				����̃J�Z�b�g��TV�f�[�^���R�s�[����
//
//	�߂�l	0 = �f�[�^���R�s�[���Ȃ�����
//			1 = �f�[�^���R�s�[����
//--------------------------------------------------------------------
static u8 CMDataInterViewTypeCopy(TV_CM *p, TV_CM *pp, s8 no)
{
	u8 i;

	if( pp->datatype == CM_NODATA_TYPE )	return 0;

	//������CM�f�[�^�ɓ���datatype�̂��̂����邩
	for( i=0; i < CMDATA_MAX ;i++ )
	{
		if( p[i].datatype == pp->datatype )	return 0;
	}

	//�f�[�^���R�s�[����
	p[no].datatype = pp->datatype;
	p[no].watch_flag = TV_WATCH_FLAG_ON;	//������Ԃɂ���
	p[no].time_count = pp->time_count;

	return 1;
}

//--------------------------------------------------------------------
//			�w�肵���ꏊ�̃f�[�^��CM_NODATA_TYPE�ł͂Ȃ���
//
//	�߂�l		CM_NODATA_TYPE						-1
//				����ȊO�͓n���ꂽ�ꏊ��Ԃ�		no
//--------------------------------------------------------------------
static s8 MixCMCopyTypeChk(TV_CM *p, u8 no)
{
	if( p[no].datatype == CM_NODATA_TYPE )	return -1;
	else return (s8)no;
}

//--------------------------------------------------------------------
//			ROM�ɂȂ��f�[�^�^�C�v�������ꍇ�́A�b�l���폜����
//--------------------------------------------------------------------
static void	CMDataVerCheck()
{
	u8 i;

	for( i=0; i < CMDATA_MAX ;i++ )
	{
		if( Fld.tv_cm[i].datatype >= CM_DATATYPE_MAX )
			CMDataClear(i);
	}

	CMDataSort();

	return;
}

//--------------------------------------------------------------------
//					�Q�[���N���A���Ă��邩�`�F�b�N
//--------------------------------------------------------------------
static void	CMDataGameClearCheck()
{
	u8 i;

	if( EventFlagCheck( SYS_GAME_CLEAR ) == TRUE ) return;

	for( i=0; i < CMDATA_MAX ;i++ )
	{
		Fld.tv_cm[i].watch_flag = TV_WATCH_FLAG_OFF;
	}

	return;
}


