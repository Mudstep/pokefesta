//**********************************************************************
//		mystatus.h
//**********************************************************************
#ifndef	MYSTATUS_H
#define	MYSTATUS_H

#undef	EXT
#ifdef 	MYSTATUS_H_PROTO
#define	EXT	/**/
#else
#define	EXT extern
#endif

//----------------------------------------------------------------------
//		define typedef
//----------------------------------------------------------------------
typedef struct												// �ڰŰ����
{
	u8	sex;												// ����
	u8	star;												// ��(��������)
	u8	zukan_get_flag;										// �}�ӹޯ��׸�
	u8	zukan_complete_flag;								// �}�ӊ����׸�
	u8	contest_art_flag;									// ��ý�,���p�ٲ���ĸر�׸�
	u16	clear_time_h;										// ����ر���(��)
	u16	clear_time_m;										// ����ر���(��)
	u16	clear_time_s;										// ����ر���(�b)
	u16	zukan_num;											// �}��
	u16	id;													// ID
	u16	play_time_h;										// ��ڲ����(��)
	u16 play_time_m;										// ��ڲ����(��)
	u16	cable_fight_win;									// ����ّΐ폟����
	u16	cable_fight_lose;									// ����ّΐ핉����
	u16	battle_tower_win;									// �����ܰ������
	u16	battle_tower_cons_win;								// �����ܰ�A����
	u16	cable_contest_win;									// ����ٺ�ýėD����
	u16	make_cube;											// ����ލ쐬��
	u16 cable_poke_exe;										// �߹�ݹ���ٌ�����
	u32 gold;												// ����
	u16	kaiwa[4];											// �ȈՉ�b
	u8	name[PERSON_NAME_SIZE+EOM_SIZE];					// ���O(11BYTE)
}TRCARD;

#define TRCARD_SIZE	(sizeof(TRCARD))

enum														// ��������
{
	TRCARD_NORMAL = 0,										// ɰ��
	TRCARD_BRONZE,											// ���ݽ�
	TRCARD_COPPER,											// ����-
	TRCARD_SILVER,											// ���ް
	TRCARD_GOLD,											// �ް���
};

#define TRCARD_CABLE_F_WIN_MAX		(9999)					// ����ّΐ폟���ő�
#define TRCARD_CABLE_F_LOSE_MAX		(9999)					// ����ّΐ핉���ő�
#define TRCARD_POKE_EXE_MAX			(65535)					// �߹�݌����ő�

//----------------------------------------------------------------------
//		��۰���
//----------------------------------------------------------------------
EXT TRCARD MultiTrainerCard[];								// �ʐM�p�ڰŰ����ܰ�

//----------------------------------------------------------------------
//		�֐�
//----------------------------------------------------------------------
EXT void SetTrainerCardPlayer(pFunc);						// ��l���ڰŰ����
EXT void SetTrainerCardFriend(u8,pFunc);					// �ʐM�ڰŰ����
EXT void SetTrainerCardWorkPlayer(TRCARD*);					// ��ڲ԰�ڰŰ����ܰ����
EXT u8 GetMultiTrainerCardStar(u8);							// �ʐM�ڰŰ�������َ擾

EXT void FontHeroNameWrite(s16,s16);						// ��l�����O�`��
EXT void FontHeroSexWrite(s16,s16);							// ��l�����ʕ`��
EXT void FontPokeZukanNumWrite(s16,s16);					// �߹�ݐ}�Ӑ��`��
EXT void FontOpPokeZukanNumWrite(s16,s16);					// �߹�ݐ}�Ӑ��`��(����)
EXT void FontPlayTimeWrite(s16,s16);						// ��ڲ ���ԕ`��
EXT void FontOpPlayTimeWrite(s16,s16);						// ��ڲ���ԕ`��(����)
EXT void FontBatchWrite(s16,s16);							// �ޯ����`��

EXT void MyStatusWindow(s16,s16);							// �ð������޳�`��
EXT void MyStatusWindowClear(s16,s16);						// �ð������޳����

#ifdef PM_DEBUG
EXT void SetTrainerCardPlayerDebug(pFunc);					// ���ޯ�޽ð��
EXT void SetTrainerCardFriendDebug(pFunc);					// ���ޯ�޽ð��
EXT void InitTrainerCardFriendDebug( void );				// ���ޯ�޽ð��������
#endif

#undef EXT
#endif // MYSTATUS_H
