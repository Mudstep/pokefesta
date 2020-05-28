//**********************************************************************
//		mul_play.h	
//**********************************************************************
#ifndef MUL_PLAY_H
#define MUL_PLAY_H

#undef	EXT
#ifdef 	EVOBJMV_H_PROTO
#define	EXT	/**/
#else
#define	EXT extern
#endif

//======================================================================
//		define
//======================================================================
#define	COMM_PLAYER_MAX 	4				// �ʐM�p���@�ő吔

//------------------------------------------
//	�ʐM�p���@���캰��
//------------------------------------------
enum
{
	HCMD_NOTHING,							// ���얳��
	HCMD_UP,								// �����
	HCMD_DOWN,								// ������
	HCMD_LEFT,								// ������
	HCMD_RIGHT,								// �E����
	HCMD_EVENT,								// �����
	HCMD_WAIT,								// ����ĊJ�n�҂�
	HCMD_UP_SITE,							// ������U�����
	HCMD_DOWN_SITE,							// �������U�����
	HCMD_LEFT_SITE,							// �������U�����
	HCMD_RIGHT_SITE,						// �E�����U�����

	HCMD_END,								// ����ȏ�̏ꍇ�̓G���[����
};

//======================================================================
//		typedef
//======================================================================
//------------------------------------------
//	�ʐM���@�p���
//------------------------------------------
typedef struct
{
	u8 sw;									// ����
	u8 my_no;								// ܰ�����
	u8 evobjno;								// �����OBJ����
	u8 mv_status;							// ������
}PLAYER_STATUS;

#define	PLAYER_STATUS_SIZE 		(sizeof(PLAYER_STATUS))
#define PLAYER_STATUS_SIZE_MAX	(PLAYER_STATUS_SIZE*COMM_PLAYER_MAX)

//------------------------------------------
//	�ʐM���@�p	�����OBJ
//------------------------------------------
typedef struct
{
	u32	sw:1;								// ����
	u32 sex:1;								// ����
	u32 debug:1;							// ���ޯ��p
	u32 :29;								// �ޯė]��

	u8	actno;								// ��������
	u8	obj_code;							// ��׺���
	u8	mv_code;							// ���캰��
	u8	ev_type;							// ���������

	u8	objID;								// OBJ ID
	u8	mapID;								// MAP ID
	u8	divID;								// DIV ID
	u8	height:4;							// ����
	u8	height_pri:4;						// ������ײ��è

	s16 dx;									// �ޯč��W
	s16 dy;
	s16	gx;									// ��۰��ٌ����W
	s16	gy;
	s16	old_gx;								// ��۰��ىߋ����W
	s16	old_gy;

	u8	site;								// �����i�[
	u8	mv_site;							// �ړ�����
	u8	dmy1;								// ��а
	u8	dmy2;								// ��а

	u8	acmd;								// ��Ұ��ݺ��ފi�[
	u8	ev_work;							// �����ܰ�
	u8	attr;								// ϯ�߱���ޭ��
	u8	old_attr;							// ϯ�߱���ޭ��
	
	u8	dmy3;								// ��а
	u8	work0;								// ܰ�0
	u8	work1;								// ܰ�1
	u8	:8;
}EV_PLAYER;

//======================================================================
//		��۰���
//======================================================================
EXT PLAYER_STATUS Player[COMM_PLAYER_MAX];

//======================================================================
//		�֐���������
//======================================================================
EXT void InitPlayer(PLAYER_STATUS*);		// ���@��񏉊���
EXT void InitAllPlayer(void);				// ���@���S������
EXT void SetPlayer(u8,s16,s16,u8);			// ���@���
EXT void SetPlayerSite(u8,u8);				// ���@�������
EXT void DelPlayer(u8);						// ���@�폜(��)
EXT void UpdatePlayer(u8,u8);				// ���@����
EXT void SetPlayerActor(u8);				// ���@�������
EXT u8 GetPlayerActorNumber(u8);			// ���@�������ގ擾
EXT void GetPlayerPos(u8,s16*,s16*);		// ���@���W�擾
EXT u8 GetPlayerSite(u8);					// ���@�����擾
EXT u8 GetPlayerHeight(u8);					// ���@�����擾
EXT s16 GetPlayerMoveAmount(u8);			// ���@�ړ��ʎ擾

EXT u8 CheckTalkPlayer(s16, s16);			//��ڲ԰�ւ̘b����������

#undef EXT
#endif // MUL_PLAY_H
