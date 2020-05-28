/********************************************************************/
/*          multi_sio.h                                             */
/*            �}���`�ʐM�T���v���v���O���� �w�b�_�t�@�C��           */
/*                                            T.Nakano              */
/*          Copyright (C) 1999-2002 NINTENDO Co.,Ltd.  Ver.20020927 */
/********************************************************************/

#include <Agb.h>

#ifndef _MULTI_SIO_H
#define _MULTI_SIO_H
#endif //_MULTI_SIO_H


/*==================================================================*/
//				�e��@�\��ON/OFF
/*==================================================================*/
#define SUM_CHECK_FUNC_ON			//�T���`�F�b�N�@�\ON�i��M�f�[�^�̃`�F�b�N�T���𒲂ׂ�j
#define CHECK_SUM_NUMBER	0		//�`�F�b�N�T���𒲂ׂ�ʐM�f�[�^�i0�`(MULTI_SIO_RECV_MAX-1)�j
#define BUFFER_COMP_FUNC_ON		//FIFO�o�b�t�@���k�@�\�ł��B
															//�f�[�^���R�}���h���M����ꍇ�́A�o�b�t�@���k�@�\���g�p�ł��܂�


/*=========================================================*/
/*                  ��`                                   */
/*=========================================================*/
//�}���`�v���C�ʐM�֌W
#define MULTI_SIO_SEND_MAX			8					//1�t���[�����ł̗L���f�[�^���M��
#define MULTI_SIO_RECV_MAX			MULTI_SIO_SEND_MAX	//1�t���[�����ł̗L���f�[�^��M��

#define MULTI_SIO_BUF_SIZE			50					//FIFO�o�b�t�@�̃T�C�Y
#define MULTI_SIO_SEND_BUF_SIZE		MULTI_SIO_BUF_SIZE	//���M��FIFO�o�b�t�@�̃T�C�Y
#define MULTI_SIO_RECV_BUF_SIZE		MULTI_SIO_BUF_SIZE	//��M��FIFO�o�b�t�@�̃T�C�Y

#define MULTI_SIO_PLAYERS_MAX		4			// �ő�v���C���[��

#define MULTI_SIO_TIMER_ENABLE	0x0040		//�^�C�}�[�N��
#define MULTI_SIO_TIMER_START		0x0080		//�^�C�}�[���荞�ݗv������

#define MULTI_SIO_TIMER_NO			3					// �^�C�}�[�m��
#define MULTI_SIO_TIMER_INTR_FLAG	(TIMER0_INTR_FLAG << MULTI_SIO_TIMER_NO)	// �^�C�}�[���荞�݃t���O
#define REG_MULTI_SIO_TIMER			(REG_TM0CNT + MULTI_SIO_TIMER_NO * 4)
#define REG_MULTI_SIO_TIMER_L		REG_MULTI_SIO_TIMER		// �^�C�}�[���W�X�^
#define REG_MULTI_SIO_TIMER_H		(REG_MULTI_SIO_TIMER + 2)

#define MULTI_SIO_TIMER_PRESCALER		TMR_PRESCALER_64CK	//�v���X�P�[���[�ݒ�

/*�v���X�P�[���[�ݒ�64CK���̃^�C�}�[�Ԋu�\*/
//#define MULTI_SIO_TIMER_COUNT_TMP	(79)	// �^�C�}�[�J�E���g�b��l(�ݒ�l0.3ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(105)	// �^�C�}�[�J�E���g�b��l(�ݒ�l0.4ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(131)	// �^�C�}�[�J�E���g�b��l(�ݒ�l0.5ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(157)	// �^�C�}�[�J�E���g�b��l(�ݒ�l0.6ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(178)	// �^�C�}�[�J�E���g�b��l(�ݒ�l0.7ms�j
#define MULTI_SIO_TIMER_COUNT_TMP	(196)	// �^�C�}�[�J�E���g�b��l(�ݒ�l0.75ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(210)	// �^�C�}�[�J�E���g�b��l(�ݒ�l0.8ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(236)	// �^�C�}�[�J�E���g�b��l(�ݒ�l0.9ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(262)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.0ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(288)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.1ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(314)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.2ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(341)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.3ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(367)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.4ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(393)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.5ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(419)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.6ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(445)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.7ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(472)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.8ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(498)	// �^�C�}�[�J�E���g�b��l(�ݒ�l1.9ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(524)	// �^�C�}�[�J�E���g�b��l(�ݒ�l2.0ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(655)	// �^�C�}�[�J�E���g�b��l(�ݒ�l2.5ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(798)	// �^�C�}�[�J�E���g�b��l(�ݒ�l3.0ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(917)	// �^�C�}�[�J�E���g�b��l(�ݒ�l3.5ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(1048)	// �^�C�}�[�J�E���g�b��l(�ݒ�l4.0ms�j
//#define MULTI_SIO_TIMER_COUNT_TMP	(1180)	// �^�C�}�[�J�E���g�b��l(�ݒ�l4.5ms�j


#define MULTI_SIO_TIMER_COUNT_MAX	0xffff		// �^�C�}�[�J�E���g�ő�l
#define MULTI_SIO_TIMER_COUNT		(MULTI_SIO_TIMER_COUNT_MAX - MULTI_SIO_TIMER_COUNT_TMP)

#if	MULTI_SIO_TIMER_COUNT_TMP > MULTI_SIO_TIMER_COUNT_MAX
	#error	MULTI_SIO_TIMER_COUNT is too long.
#endif


//��\���ʐM�f�[�^//
#define FORBIDDEN_DATA			0xffff	//�֎~�f�[�^(�ύX�s�j
#define NOT_CONNECTED_DATA		0xffff	//���ڑ�(�ύX�s�j
#define EMPTY_DATA				0xefff	//����f�[�^
//#define INVALID_DATA			0xdfff	//�����f�[�^
#define INVALID_DATA			0		//�����f�[�^
#define SIGNAL_DATA				0x8fff	//�ėp�V�O�i���f�[�^
//#define RESTART_DATA			0x3fff	//�ʐM�ĊJ�f�[�^
//#define END_DATA				0x5fff	//�G���h�f�[�^


//��ڑ��f�[�^��
//CONNECT_CHECK_DATA�Ƀ\�t�g�ŗL�̔F�؃R�[�h��ݒ肵�Ă��������B
//16�r�b�g�̂�������2�r�b�g�̓o�[�W�����R�[�h�ƂȂ��Ă��܂��B
//�ŏ��̃o�[�W�����͍ŉ��ʃr�b�g��0�Ƃ��A�ʐM�����̃o�[�W�����������邲�Ƃ�
//1�A2�ƕύX���Ă��������B�ڑ��F�؂œ����Q�[���Ɣ��f����̂�
//��ԍŏ��̃o�[�W��������3�o�[�W�����܂łƂȂ��Ă��܂��B
//�ڑ��`�F�b�N�f�[�^
#define CONNECT_CHECK_DATA		0xB9A0	//�ڑ��`�F�b�N�f�[�^	2002/10/07�ύX
#define VERSION_CHECK_MASK		0x0003


/*=========================================================*/
/*                  �\����                                 */
/*=========================================================*/
//���M��FIFO�o�b�t�@�\����
typedef struct
{
	u16 Item[MULTI_SIO_SEND_MAX]		//�o�b�t�@�̒��g
			[MULTI_SIO_BUF_SIZE];
	u8  Head;							//�f�[�^�̐擪
	u8  Size;							//�o�b�t�@�ɓ����Ă���f�[�^�̌�

} SendFifoBuf;

//��M��FIFO�o�b�t�@�\����
typedef struct
{
	u16 Item[MULTI_SIO_PLAYERS_MAX]
			[MULTI_SIO_RECV_MAX]
			[MULTI_SIO_BUF_SIZE];	//�o�b�t�@�̒��g
	u8  Head;							//�f�[�^�̐擪
	u8  Size;							//�o�b�t�@�ɓ����Ă���f�[�^�̌�

} RecvFifoBuf;


// �}���`�v���C�ʐM���[�N�G���A�\����
typedef struct
{
	u8  Type;				//�ڑ��֌W�i�e���q���j
	u8  State;				//�ʐM�֐��̏��
	u8	ID;					//�ڑ�ID
	u8	PlayersNum;			//�ڑ��v���C���[��

	u16 CheckData[4];		//�ڑ��`�F�b�N�f�[�^

//���������Ƀ����o��ǉ����Ȃ��ł��������I�I

	u8	SkipFrameFlag;		//�t���[���X�L�b�v�t���O
	s8	SioIntrCheckFlag;	//�ʐM���荞�݃`�F�b�N�t���O
//	s8	SioStopFlag;		//�ʐM��~�t���O�i�ʐM��~���Ɋ��荞�݂��֎~����ꍇ�Ɏg�p�j
	u8	Signal;				//�ėp�V�O�i��
	u8	MultiSioVersion;	//�ʐM�v���O�����̃o�[�W����

	u8	HardError;			//�n�[�h�G���[�t���O
	u8	CheckSumError;		//�`�F�b�N�T���G���[�t���O
	u8	OverFlowError;		//�o�b�t�@�̃I�[�o�[�t���[�t���O
	u8	SioIntervalError;	//�^�C�}�[�Ԋu�ݒ�l�̃G���[�t���O

	u16	CheckSum;			//�`�F�b�N�T��

	u8	SendCounter;		//�f�[�^���M��
	u8	RecvCounter;		//�f�[�^��M��

	SendFifoBuf SendBuf;	//���MFIFO�o�b�t�@
	RecvFifoBuf RecvBuf;	//��MFIFO�o�b�t�@

} MultiSioArea;


/*=========================================================*/
/*                  �ϐ�                                   */
/*=========================================================*/
extern	u16 ImeBak;				//IME���W�X�^�̃o�b�N�A�b�v
extern	MultiSioArea M_s;		//�}���`�v���C�ʐM���[�N�G���A�\����

extern u8	Send_Buf_Size;
extern u8	Recv_Buf_Size;


/*==================================================================*/
/*==================================================================*/
/*                  �}���`�v���C�ʐM                                */
/*==================================================================*/
/*==================================================================*/

/*------------------------------------------------------------------*/
/*                       �ʐM�I��                                   */
/*------------------------------------------------------------------*/
extern	void Multi_SioStop(void);

//�ʐM���荞�݁A�^�C�}�[���荞�݂��֎~���A�ʐM���[�h�����甲���܂��B


/*------------------------------------------------------------------*/
/*             �}���`�v���C�ʐM������                               */
/*------------------------------------------------------------------*/
extern	void Multi_SioInit(void);

//�ʐM���[�h�A���荞�݋�����ݒ肵�A���ϐ������������܂��B


/*------------------------------------------------------------------*/
/*                     �ʐM�Ή��Q�[��������                         */
/*------------------------------------------------------------------*/
extern	void Multi_SioGameInit(void);

//�ڑ��F�����ɓd���I���A�I�t�����Ƃ��Ȃǂ�
//�s����ȋ�������菜���܂��B
//���C�����[�v�ɓ���O�ɌĂяo���Ă��������B

/*------------------------------------------------------------------*/
/*             �}���`�v���C�ʐM���C��                               */
/*------------------------------------------------------------------*/
extern	u32 Multi_SioMain(u8 *Signal_ptr, u16 *SendData_ptr, u16 (*RecvData_ptr)[MULTI_SIO_PLAYERS_MAX]);

//�e�ڑ��A�q�ڑ��𔻕ʁA�^�C�}�[�̐ݒ�A�^�C�}�[���荞�݂�
//���ȂǁA�ʐM�ɂ����鏉���ݒ���s���܂��B
//�ʐM���́A���MFIFO�o�b�t�@�ւ̃f�[�^�̒~�ρA��M�o�b�t�@�����
//�f�[�^�̎��o�����s���܂��B���C���v���O�����̐擪�ŌĂяo���Ă��������B

//�߂�l�ɂ���uROM�o�[�W�����v�́A�ڑ�����Ă���ROM�̂�����
//�ł��Â��o�[�W�����i���o�[��Ԃ��܂��B�܂�Ver1��Ver0��ROM��
//�ʐM�ɂĎg�p���ꂢ��ꍇ�A�߂�l��Ver0�����邱�Ƃ������u1�v��
//�Ԃ��܂��B���߂�l�́u1����4�v�ł��B�u0�v���Ԃ��Ă���ꍇ��
//�ڑ��F�؂ł��Ă��Ȃ����Ƃ������܂��B

#define MULTI_SIO_INTR_TIME_OVER	10

//����
//�Eu8  *Signal_ptr			�V�O�i���f�[�^�|�C���^
//���C���v���O��������̃V�O�i���ŁA�ʐM�֐��̃V�[�P���X��i�߂܂��B
#define SIGNAL_OFF				0	//�V�O�i��OFF
#define SIGNAL_DECISION		1	//����{�^��
#define SIGNAL_CANCEL			2	//�L�����Z���{�^��

//�Eu16 *SendData_ptr	���[�U���M�o�b�t�@�|�C���^
//FileName[MULTI_SIO_SEND_MAX]�̌`��
//�w�肵�Ă��������B�i�gFileName�h�͕ύX�j
//��*SendData_ptr�Ŏw�肳��郆�[�U�o�b�t�@�́A���C���v���O������
// ���������Ɂu�ʐM�Ɏg�p���Ȃ��l�v�ŏ��������Ă����Ă��������B


//�Eu16 (*RecvData_ptr)[MULTI_SIO_PLAYERS_MAX]	���[�U��M�o�b�t�@�|�C���^
//FileName[MULTI_SIO_RECV_MAX][MULTI_SIO_PLAYERS_MAX]�̌`��
//�w�肵�Ă��������B�i�gFileName�h�͕ύX�j


//�߂�l
#define MULTI_SIO_ID_MASK						0x00000003	//ID�}�X�N
#define MULTI_SIO_PLAYERS_NUM_MASK	0x0000001c	//�ڑ��v���[���[���}�X�N
#define MULTI_SIO_VERSION_MASK			0x00000e00	//ROM�o�[�W�����}�X�N�i4�o�[�W�����܂Łj

#define MULTI_SIO_ID_SHIFT						0
#define	MULTI_SIO_PLAYERS_NUM_SHIFT		2
#define MULTI_SIO_VERSION_SHIFT				9

#define MULTI_SIO_PARENT								0x00000020	//�e�ڑ�
#define MULTI_SIO_MAIN_SEQ							0x00000040	//�ʐM���C����
#define MULTI_SIO_FRAME_SKIP						0x00000100	//�t���[���X�L�b�v
#define MULTI_SIO_HARD_ERROR						0x00001000	//�n�[�h�G���[
#define MULTI_SIO_CHECKSUM_ERROR				0x00002000	//�`�F�b�N�T���G���[
#define MULTI_SIO_SEND_OVER_FLOW_ERROR	0x00004000	//���M��FIFO�I�[�o�[�t���[�G���[
#define MULTI_SIO_RECV_OVER_FLOW_ERROR	0x00008000	//��M��FIFO�I�[�o�[�t���[�G���[
#define MULTI_SIO_INTERVAL_ERROR				0x00010000	//�^�C�}�[�Ԋu�ݒ�l�G���[
#define MULTI_SIO_ID_OVER_ERROR					0x00020000	//�ڑ�ID�I�[�o�[�G���[
#define MULTI_SIO_INTR_STOP_ERROR				0x00040000	//SIO���荞�ݒ�~�G���[

//�ʐM�֐��̏��(M_s.State�̒l�j
enum
{ MULTI_SIO_STOP, MULTI_SIO_INIT, MULTI_SIO_CONNECT_CHECK,
 MULTI_SIO_CONNECT_DECISION, MULTI_SIO_MAIN };


/*==================================================================*/
/*             �}���`�v���C�ʐM�T�u���[�`��                         */
/*==================================================================*/

//�}���`�v���C�ʐM�T�u���[�`���ɂ��ẮA���ׂ�MultiSioMain����
//�Ăяo����Ă��邽�߁A�ʂɌĂяo���K�v�͂���܂���B

/*------------------------------------------------------------------*/
/*                         �ڑ��F��                                 */
/*------------------------------------------------------------------*/
extern	void ConnectCheck(void);

//�e�@�A�q�@�ڑ��𔻕ʂ�����A�ڑ��l�����o������
//�ڑ��󋵂��`�F�b�N���܂��B

#define MULTI_SIO_ID0		0
#define MULTI_SIO_ID1		1
#define MULTI_SIO_ID2		2
#define MULTI_SIO_ID3		3


/*------------------------------------------------------------------*/
/*                         �^�C�}�[������                             */
/*------------------------------------------------------------------*/
extern	void TimerInit(void);

//�e�@�̂݃^�C�}�[���荞�݂������A�^�C�}�[���ԂȂǂ�ݒ肵�܂��B


/*------------------------------------------------------------------*/
/*             ���MFIFO�o�b�t�@�ɑ��M�f�[�^��~��                   */
/*------------------------------------------------------------------*/
extern	void PutSendData(u16 *SendData_ptr);

//���MFIFO�o�b�t�@�ւ̃f�[�^�̒~�ς��s���܂��B
//�}���`�v���C�ʐM���C���ŌĂяo���Ă��܂��B
//���[�U�[���M�o�b�t�@����f�[�^���󂯎��A���M��FIFO�o�b�t�@��
//�f�[�^��~�ς����� ���[�U�[�o�b�t�@���uINVALID_DATA�v�ŏ��������܂��B


//����
//�Eu16 *SendData_ptr	���[�U���M�o�b�t�@�|�C���^


/*------------------------------------------------------------------*/
/*             ��MFIFO�o�b�t�@�����M�f�[�^���擾                 */
/*------------------------------------------------------------------*/
extern	void GetRecvData(u16 (*RecvData_ptr)[MULTI_SIO_PLAYERS_MAX]);

//��MFIFO�o�b�t�@�ւ����M�f�[�^�̎��o�����s���܂��B
//�}���`�v���C�ʐM���C���ŌĂяo���Ă��܂��B
//��M��FIFO�o�b�t�@�Ƀf�[�^���~�ς���Ă��Ȃ��Ƃ���
//���[�U�[��M�o�b�t�@�ɁuINVALID_DATA�v��n���܂��B


//����
//�Eu16 (*RecvData_ptr)[MULTI_SIO_PLAYERS_MAX]	���[�U��M�o�b�t�@�|�C���^


/*==================================================================*/
/*==================================================================*/
/*                      ���荞�݊֐�                               */
/*==================================================================*/
/*==================================================================*/

/*------------------------------------------------------------------*/
/*                      V�u�����N���荞�݊֐�                       */
/*------------------------------------------------------------------*/
extern	void MltSioVblkIntr(void);

//�ʐM���J�n���܂��B
//V�u�����N�̐擪�ŌĂяo���Ă��������B


/*------------------------------------------------------------------*/
/*                      �^�C�}�[���荞�݊֐�                           */
/*------------------------------------------------------------------*/
extern	void Multi_SioTimerIntr(void);

//�ݒ莞�Ԃ��o�߂�����A�^�C�}�[���~�߁A�^�C�}�[���Đݒ肵
//�ʐM���J�n���܂��B


/*------------------------------------------------------------------*/
/*                      �ʐM���荞�݊֐�                           */
/*------------------------------------------------------------------*/
extern	void Multi_SioIntr(void);

//��M�f�[�^��FIFO�o�b�t�@�ւ̒~�ρA���M�f�[�^�̃Z�b�g�A
//�^�C�}�[�̐�����s���܂��B



/*==================================================================*/
/*                  ���荞�݊֐��T�u���[�`��                       */
/*==================================================================*/

//���荞�݊֐��T�u���[�`���͏�L�̊��荞�݊֐����ŌĂяo����܂��B
//�ʂɌĂяo���K�v�͂���܂���B

/*------------------------------------------------------------------*/
/*                      �ʐM�J�n                                   */
/*-----------------------------------------------------------------*/
extern	void Multi_SioStart(void);

//�ʐM���J�n���܂��B

/*---------------------------------------------------*/
/*         �ڑ��`�F�b�N                          */
/*---------------------------------------------------*/
extern	u8 MultiSioLinkCheck(void);

//�ڑ��`�F�b�N�f�[�^������肵�A�ڑ���Ԃ��`�F�b�N���܂��B

/*---------------------------------------------------*/
/*         ��M�f�[�^�̏���                          */
/*---------------------------------------------------*/
extern	void SetRecvData(void);

//��M�f�[�^���������܂��B
//�e�t���[�� 1��ڂ̒ʐM�̓v���O�����̎��s�ɕs�K�v�ȃf�[�^������肷��
//�_�~�[�ʐM�ł��邽�߁A��M�����f�[�^�͎�MFIFO�o�b�t�@�ɂ͒~�ς��܂���B

//�_�~�[�ʐM�Ŏ�M�����f�[�^�́A�O�t���[���̑���M�f�[�^��
//�u�`�F�b�N�T���f�[�^�v�Łi�T���`�F�b�N�@�\ON�̂Ƃ��j�A
//���@�̃`�F�b�N�T���f�[�^����M�������@�̃`�F�b�N�T���f�[�^�����؂�
//�l���قȂ�΃`�F�b�N�T���G���[�t���O�𗧂Ă܂��B


/*---------------------------------------------------*/
//          ���M�f�[�^�Z�b�g                         //
/*---------------------------------------------------*/
extern	void SetSendData(void);

//���M�f�[�^���Z�b�g���܂��B
//�ݒ�񐔂̒ʐM�I����A�Ō�̑��M�f�[�^�Z�b�g�^�C�~���O����
//���̃t���[�����Ŏ�M��������M�f�[�^�̃T���`�F�b�N�f�[�^��
//�Z�b�g���i�T���`�F�b�N�@�\ON�̂Ƃ��j�A���t���[����
//�_�~�[�ʐM���ɂĐ��������Ƃ�Ă��邩�����؂��܂��B


/*---------------------------------------------------*/
//                �^�C�}�[���X�g�b�v                 //
/*---------------------------------------------------*/
extern	void StopTimer(void);

//�^�C�}�[���Ƃ߁A�^�C�}�[���Ԃ��Đݒ肵�܂��B

/*---------------------------------------------------*/
//               �^�C�}�[�̃Z�b�g                    //
/*---------------------------------------------------*/
extern	void SetTimer(void);

//�e�@�ŁA�ݒ�񐔂̒ʐM���������Ă��Ȃ��ꍇ�̓^�C�}�[���X�^�[�g���܂��B
//�ݒ�񐔂̒ʐM���������Ă���ꍇ�́A�e�@�A�q�@�Ƃ��ɑ��M�񐔁A
//��M�񐔂�0�ɂ��ǂ��܂��B


/*==================================================================*/
/*==================================================================*/
/*                  FIFO�o�b�t�@                                    */
/*==================================================================*/
/*==================================================================*/

/*------------------------------------------------------------------*/
/*                      �o�b�t�@�̏�����                          */
/*------------------------------------------------------------------*/
extern	void RecvBufInit(void);

//��M�o�b�t�@�����������܂��B
//FIFO�o�b�t�@�̒��g�ɂ͋�i�J���j�f�[�^�uEMPTY_DATA�v�����܂��B

extern	void SendBufInit(void);

//���M�o�b�t�@�����������܂��B
//FIFO�o�b�t�@�̒��g�ɂ͋�i�J���j�f�[�^�uEMPTY_DATA�v�����܂��B

