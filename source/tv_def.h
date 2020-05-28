//====================================================================
//						�s�u��拤�ʒ�`
//
//				�X�N���v�g����Ă΂��֐���extern�錾�Ȃ�
//
//
//
/*
			��PERSON_NAME_SIZE�Ȃǂ̃T�C�Y���ύX�ɂȂ�������
				�E�莆�A�閧�Ō��܂��������o�ɂȂ��Ă���
				�@�A�h���X�������̂ō����ł��߂�悤�ɂ���

*/
//
//====================================================================
#ifndef __TV_DEF_H__
#define __TV_DEF_H__


#define TV_NOTHING			0xff	//�����R�[�h

#define TV_WATCH_FLAG_OFF		0	//����Ȃ����(�������)
#define TV_WATCH_FLAG_ON		1	//�������(�܂����ĂȂ����)
#define TV_WATCH_FLAG_HOLDING	2	//�J�Ó���(�b�l�Ŏg�p�C�x���g��)

#define	TV_NODATA_TYPE1			0					//�f�[�^�Ȃ�
#define TV_TYPE_START			1					//�f�[�^�^�C�v��`�̃X�^�[�g
#define LETTER_TYPE_START	(TV_TYPE_START)			//�莆�^�̃X�^�[�g
#define LETTER_TYPE_MAX		(LETTER_TYPE_START+20)	//�莆�^�̐�
#define SEC_TYPE_START		(LETTER_TYPE_MAX)		//�閧�^�̃X�^�[�g
#define SEC_TYPE_MAX		(SEC_TYPE_START+20)		//�閧�^�̐�
#define TIME_TYPE_START		(SEC_TYPE_MAX)			//���Ԍ^�̃X�^�[�g
#define TIME_TYPE_MAX		(TIME_TYPE_START+20)	//���Ԍ^�̐�
#define TV_TYPE_END			(TIME_TYPE_MAX)			//�f�[�^�^�C�v��`�̏I���

//�莆�^
#define	TV_POKEMON_TYPE1	(LETTER_TYPE_START+0)		//�|�P�����A���P�[�g
#define	TV_RECENTLY_TYPE1	(LETTER_TYPE_START+1)		//�ŋ߂̂ł�����
#define	TV_CLUB_TYPE1		(LETTER_TYPE_START+2)		//��D���N���u�̃C���^�r���[
#define	TV_GYMLEADER_TYPE1	(LETTER_TYPE_START+3)		//GYMLeader��̌�̃C���^�r���[
#define	TV_NAME_TYPE1		(LETTER_TYPE_START+4)		//�������f��
#define	TV_CONTEST_TYPE1	(LETTER_TYPE_START+5)		//�R���e�X�g
#define	TV_TOWER_TYPE1		(LETTER_TYPE_START+6)		//�o�g���^���[
//�閧�^
#define	TV_GETPOKE_TYPE1	(SEC_TYPE_START+0)			//�퓬
#define	TV_SHOP_TYPE1		(SEC_TYPE_START+1)			//�V���b�v
#define	TV_FAILED_TYPE1		(SEC_TYPE_START+2)			//�ߊl���s
#define	TV_FISHING_TYPE1	(SEC_TYPE_START+3)			//�ނ肵�܂���
#define	TV_ONEDAY_TYPE1		(SEC_TYPE_START+4)			//����ŕ߂܂�����
//���Ԍ^
#define	TV_GENERATING_TYPE1	(TIME_TYPE_START+0)			//��ʔ���

#ifndef ASM_CPP		/* �A�Z���u���̏ꍇ�݂̂��̃V���{������`����� */


//�ʐM�^�C�v��`
enum{
	NOCOPY_TYPE1 = 0,				//�ʐM���Ȃ��^
	COPY_TYPE1,						//�R�s�[�^
	CUT_TYPE1,						//�؂���^(�莆�^)
	CUT_TYPE2,						//�؂���^(�閧�^)
	CUT_TIME_TYPE,					//�؂���^(���Ԍ^)
	RARE_TYPE1,						//���A(��΂ɃR�s�[�����)
};


#define TVDATA_HIMSELF_MAX	5		//TV�f�[�^�̊m�ې�(����)
#define TVDATA_PARTNER_MAX	19//20		//TV�f�[�^�̊m�ې�(����)
#define TVDATA_MAX	(TVDATA_HIMSELF_MAX+TVDATA_PARTNER_MAX)//TV�f�[�^�̊m�ې�

#define GENERATING_INCUBATION_TIME		1		//��������

#define TV_TEMP_WORK		(Fld.tv_event[TVDATA_MAX])	//TV�̕ۑ��p���[�N
#define TV_SHOP_CREATE_NUM	20		//������������鐔
									
//200����1�̊m��(65535/200 = 327)
#define TV_GENERATING_START_ON	327		//��ʔ�������������l(�ȉ��Ŕ���)
#define TV_GETPOKE_START_ON		65535	//�߂܂����|�P����
#define TV_CONTEST_START_ON		65535	//�R���e�X�g����
//3����1�̊m��(65535/3 = 21845)
#define TV_SHOP_START_ON		21845	//���������
#define TV_FAILED_START_ON		65535	//�ߊl���s
#define TV_ONEDAY_START_ON		65535	//����ɕ߂܂�����

#define GENERATING_TIME			2		//��ʔ������Ă������
#define TV_ONEDAY_CREATE_NUM	20		//����ɕ߂܂������쐬���鐔
#define TV_FAILED_CREATE_NUM	3		//�ߊl���s���쐬����{�[���𓊂�����

//(�閧�^)�������f��
enum{
	TV_NAME_NTOP = 0,
	TV_NAME_NLAST,
	TV_NAME_TOP,
	TV_NAME_LAST,
};
enum{
	TV_NAME_MY = 0,
	TV_NAME_NICK,
	TV_NAME_POKE,
};


//====================================================================
//						�s�u���\����
//====================================================================

//--------------------------------------------------------------
//							�莆�^
//
//				�|�P�����A���P�[�g�E��������̂ł�����
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u16 inside_data;	    // �����f�[�^(�s�J�`���E�ɂ��ē�)

	u16  easytalk[6];		// �ȈՕ���

    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
    u8  work[8];			// �]��


	//�������牺�͎莆�^����
    u8  data_id[2];			// TV�f�[�^����ID
	u8  before_id[2];		// �O�̃g���[�i�[��ID
}TV_Letter_Work;			// 36byte


//--------------------------------------------------------------
//							�莆�^
//
//				��D���N���u�̃C���^�r���[�̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u16 pokeno;				// �擪�ɂ���|�P�����̎��

    u8  friend:4;		    // �Ȃ��x(���4�r�b�g��ۑ�����)
	u8  ans:4;				// ����ɑ΂��Ă̓���

    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
	u8  work2[3];			// �]��
    u8  nickname[MONS_NAME_SIZE+EOM_SIZE];		// �j�b�N�l�[��

	u8  work;				// �]��

    u16 easytalk_no[2];		// �ȈՉ�b�ꌾ

	//�������牺�͎莆�^����
    u8  data_id[2];			// TV�f�[�^����ID
	u8  before_id[2];		// �O�̃g���[�i�[��ID
}TV_Club_Work;				// 36byte


//--------------------------------------------------------------
//							�莆�^
//
//					�W�����[�_�[���̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u16 easytalk_no;		// �ȈՉ�b�ꌾ

	u8  bad;				// �m���ɂȂ�����
	u8  turn;				// �^�[����

    u16 pokeno;				// �W�����[�_�[��|�����|�P����
    u16 waza;				// �Ō�Ɏg�����Z

	u8  worldpos;			// �W�����[�_�[�̃V�e�B��

    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
	u8  work2[3];			// �]��
    u8  ans;				// ����ɑ΂��Ă̓���
	u8  work[9];			// �]��

	//�������牺�͎莆�^����
    u8  data_id[2];			// TV�f�[�^����ID
	u8  before_id[2];		// �O�̃g���[�i�[��ID
}TV_GYM_Work;				// 36byte


//--------------------------------------------------------------
//							�莆�^
//
//						�������f���̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u16 pokeno;		        // �|�P�����̎��

    u8  nickname[MONS_NAME_SIZE+EOM_SIZE];		// �j�b�N�l�[��
    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
    u8  work2[3];			// �]��

	u8  r_q_no[2];			// �����_���Ȏ���i���o�[��ۑ�
	u16	r_pokeno;			// �����_���ȃ|�P�����i���o�[��ۑ�

    u8  work[2];			// �]��

	//�������牺�͎莆�^����
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
}TV_Name_Work;				// 36byte


//--------------------------------------------------------------
//							�莆�^
//
//						�R���e�X�g�̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u16 pokeno;				// �|�P�����̎��

    u16 easytalk_no[2];		// �ȈՉ�b�ꌾ

    u8  nickname[MONS_NAME_SIZE+EOM_SIZE];		// �j�b�N�l�[��

    u8  type:3;				// �R���e�X�g�^�C�v
    u8  rank:2;				// �R���e�X�g�����N
    u8  zyuni:2;			// ����
    u8  work:1;				// �]��

    u16 waza;				// �Ō�Ɏg�����Z

    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
	u8	work2[2];			//

	//�������牺�͎莆�^����
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
}TV_Con_Work;				// 36byte


//--------------------------------------------------------------
//							�莆�^
//
//				�o�g���^���[��̃C���^�r���[�̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];		// �v���C���[�̖��O
    u16 pokeno;				// �|�P�����̎��

    u8  enemy_name[PERSON_NAME_SIZE+EOM_SIZE];		// ����̖��O
    u16 enemy_pokeno;		// �Ō�ɑΐ킵���|�P�����̎��

    u16 win;				// �A����
    u16 easytalk_no;		// �ȈՉ�b�ꌾ

    u8  level;				// ���x��
    u8  ans;				// �󂯓����f�[�^
    u8  win_flag;			// ���������t���O

	u8	work[3];			// �]��

	//�������牺�͎莆�^����
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
}TV_Tower_Work;				// 36byte


//--------------------------------------------------------------
//						�莆�^(��������)
//
//						��ʔ����̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u8  special;			// ����\��
    u8  character;			// ���i

    u16 waza[4];			// �����Ă���Z

    u16 pokeno;				// �|�P�����̎��
    u16 itemno;				// �����Ă���A�C�e��

    u8  map_id;				// �ǂ��ŏo��������
    u8  div_id;				// �ǂ��ŏo��������
    u8  rare;				// ���A�x
    u8  appearance;			// �o��(%)

    u8  level;				// �ǂ̃��x���ŏo�Ă��邩
    u8  batch;				// �o�b�`��
    u16 time_count;			// ��������

    u8  work[8];			// �]��

	//�������牺�͎莆�^����
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
}TV_Generating_Work;		// 36byte


//--------------------------------------------------------------
//							�閧�^
//
//					�߂܂����|�P�����̏Љ�̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

	u8  work[2];			// �]��

    u8  nickname[MONS_NAME_SIZE+EOM_SIZE];		// �j�b�N�l�[��
    u8  ball_type;			// �������{�[���̎��
    u16 get_pokeno;			// �߂܂����|�P�����̎��

    u8  ball_count;			// �{�[���𓊂�����


	//�������牺�͔閧�^����
    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
	u8	work3[3];			//
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
    u8  trainer_id[2];		// TV�f�[�^�����ID(�g���[�i�[ID)
}TV_Sec_Btl_Work;			// 36byte


//--------------------------------------------------------------
//							�閧�^
//
//					�����̔��������̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u8  bargain_flag;		// �������̎���
    u8  work[3];			// �]��

    u16 itemno[3];			// �A�C�e���̎��
    u16 num[3];				// �������A�C�e���̐�

    u8  worldpos;			// �ǂ���

	//�������牺�͔閧�^����
    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
	u8	work3[3];			//
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
    u8  trainer_id[2];		// TV�f�[�^�����ID
}TV_Sec_Shop_Work;			// 36byte


//--------------------------------------------------------------
//							�閧�^
//
//						�ߊl���s�̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u8  work[10];			// �]��

    u16 pokeno;				// �|�P�����̎��
    u16 failed_pokeno;		// ������(�|����)�|�P�����̎��

    u8  ball_count;			// �{�[���𓊂�����
    u8  failed_flag;		// ���������|�������t���O
	u8	worldpos;			// ���ԓ��H��

	//�������牺�͔閧�^����
    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
	u8	work3[3];			//
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
    u8  trainer_id[2];		// TV�f�[�^�����ID(�g���[�i�[ID)
}TV_Sec_Get_Failed_Work;	// 36byte


//--------------------------------------------------------------
//							�閧�^
//
//						�ނ肵�܂���̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

	u8	true_count;			// �A��������
	u8	false_count;		// �A�����s��

    u16 pokeno;				// �ނ����|�P�����̎��

    u8  work[13];			// �]��

	//�������牺�͔閧�^����
    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
	u8	work3[3];			//
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
    u8  trainer_id[2];		// TV�f�[�^�����ID(�g���[�i�[ID)
}TV_Sec_Fishing_Work;		// 36byte

//--------------------------------------------------------------
//							�閧�^
//
//					����ŕ߂܂������̃��[�N
//--------------------------------------------------------------
typedef struct{
    u8  datatype;			// �f�[�^�^�C�v(0=�f�[�^�Ȃ�) 
	u8	watch_flag;			// TV������邩

    u16 count;				// �߂܂�����

    u16 get_pokeno;			// �Ō�ɕ߂܂����|�P�����̎��
    u16 walk_count;			// ���̓�����������

    u16 pokeno;				// �Ō�Ɏg�p���Ă����|�P�����̎��
    u8  worldpos;			// �Ō�ɕ߂܂����ꏊ

    u8  work[8];			// �]��

	//�������牺�͔閧�^����
    u8  player_name[PERSON_NAME_SIZE+EOM_SIZE];    // �v���C���[�̖��O
	u8	work3[3];			//
    u8  data_id[2];			// TV�f�[�^����ID
    u8  before_id[2];		// �O�̃g���[�i�[ID
    u8  trainer_id[2];		// TV�f�[�^�����ID(�g���[�i�[ID)
}TV_Sec_OneDay_Work;		// 36byte


//--------------------------------------------------------------------
//					�X�N���v�g����Ă΂��
//--------------------------------------------------------------------
//tv.c
extern void InterViewSet();			//�C���^�r���[��т���


//--------------------------------------------------------------------
//				
//--------------------------------------------------------------------
extern s8 tv_akino;					//�s�u�f�[�^�̋󂢂Ă�ꏊ
extern pFunc InterViewEndFunc;		//�I�����ɌĂ΂�郋�[�`�����i�[


#endif	/*ASM_CPP*/
#endif	/*__TV_DEF_H__*/

