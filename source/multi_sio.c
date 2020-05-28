/********************************************************************/
/*          multi_sio.c                                             */
/*            �}���`�ʐM�T���v���v���O����                          */
/*                                            T.Nakano              */
/*          Copyright (C) 1999-2002 NINTENDO Co.,Ltd.  Ver.20020927 */
/********************************************************************/

#include <Agb.h>
#include "multisio.h"

//#define H_BLANK_INTR_ON
//#define DEBUG_USERWORK


/*
#ifdef DEBUG_USERWORK
extern u8 UserWork[];
extern u16 RecvBuf[3][4];
#endif
*/

//#define DEBUG

/*------------------------------------------------------------------*/
/*                       ��`                                       */
/*------------------------------------------------------------------*/
//#define REG_RECV_DATA(x)		(REG_SIOMLT_RECV + (x * 0x02))
#define MULTI_SIO_SEND_BUF		(M_s.SendBuf)
#define MULTI_SIO_RECV_BUF		(M_s.RecvBuf)


/*------------------------------------------------------------------*/
/*                       �O���[�o���ϐ�                             */
/*------------------------------------------------------------------*/
u16	ImeBak;				//IME���W�X�^�̃o�b�N�A�b�v
MultiSioArea M_s;		//�}���`�v���C�ʐM���[�N�G���A�\����

static u8	SioIntrCount;			//SIO���荞�݊Ď��J�E���^
static u8	CheckSendBufFlag;	//�o�b�t�@�T�C�Y�m�F�t���O
static u16	CheckSendData;		//���M�f�[�^�̘_���a�`�F�b�N
static u16	CheckRecvData;		//�S��M�f�[�^�̘_���a�`�F�b�N

static u8	SioStartFlag;			//�ʐM�J�n�t���O
static u8	PlayersNumBak;


/*
u16	debugworkflag;
u16	debugworkcnt;
*/

u8	Send_Buf_Size;		//���MFIFO�o�b�t�@�̂��܂�i�x���j
u8	Recv_Buf_Size;		//��MFIFO�o�b�t�@�̂��܂�i�x���j


#ifdef DEBUG
//�������Ԍv���p
static u16	TmrBuf[256];	//����^�C�}�[�l
static u8	count;			//�J�E���g

#define TIMER0_SET	{								\
	*(vu16 *)REG_TM0CNT_L = 0;				\
	*(vu16 *)REG_TM0CNT_H = 0x0080;		\
}

#define TIMER_STOP	{		\
	TmrBuf[count++]= *(vu16 *)REG_TM0CNT_L;		\
	*(vu16 *)REG_TM0CNT_L = 0;								\
	*(vu16 *)REG_TM0CNT_H = 0;								\
}

#endif //DEBUG

/*------------------------------------------------------------------*/
/*                       �ʐM�I��                                   */
/*------------------------------------------------------------------*/
void Multi_SioStop(void)
{
	ImeBak = *(vu16 *)REG_IME;							//IME�̑ޔ�
	*(vu16 *)REG_IME = 0;								// IME ���Z�b�g

	*(vu16 *)REG_IE &= ~(SIO_INTR_FLAG					//SIO���荞�ݕs����
    				| MULTI_SIO_TIMER_INTR_FLAG);		//�^�C�}�[���荞�ݕs����

	*(vu16 *)REG_IME = ImeBak;							// IME�̕��A

	*(vu16 *)REG_SIOCNT = 0;							//SIO��~
	*(vu16 *)REG_MULTI_SIO_TIMER_H = 0;					//�^�C�}�[���~

    *(vu16 *)REG_IF = (SIO_INTR_FLAG					//���O��SIO���荞�ݗv�����N���A
    				| MULTI_SIO_TIMER_INTR_FLAG);		//���O�̃^�C�}�[���荞�ݗv�����N���A

//	DmaClear(3, 0, &M_s, sizeof(M_s), 32);
	CpuClear( 0, &M_s, sizeof(M_s), 32);
}


/*------------------------------------------------------------------*/
/*             �}���`�v���C�ʐM������                               */
/*------------------------------------------------------------------*/
void Multi_SioInit(void)
{
	ImeBak =  *(vu16 *)REG_IME;							//IME�̑ޔ�
	*(vu16 *)REG_IME = 0;								// IME ���Z�b�g

	*(vu16 *)REG_IE &= ~(SIO_INTR_FLAG					//SIO���荞�ݕs����
    					| MULTI_SIO_TIMER_INTR_FLAG);	//�^�C�}�[���荞�ݕs����

	*(vu16 *)REG_IME = ImeBak;							// IME�̕��A

	//�ʐM���[�h�ݒ�
	*(vu16 *)REG_RCNT = R_SIO_MASTER_MODE;
	*(vu16 *)REG_SIOCNT = SIO_MULTI_MODE;	//�ʐM���[�h���}���`�v���C���[�h�ɐݒ�

	*(vu16 *)REG_SIOCNT |=(SIO_IF_ENABLE	//�ʐM���荞�ݗv������
    				 	| SIO_115200_BPS);	//�{�[���[�g�ݒ�

	//���荞�݋���
	ImeBak = *(vu16 *)REG_IME;				//IME�̑ޔ�

	*(vu16 *)REG_IME = 0;					// IME ���Z�b�g
	*(vu16 *)REG_IE |= SIO_INTR_FLAG;		//SIO���荞�݋���

	*(vu16 *)REG_IME = ImeBak;				// IME�̕��A

	*(vu16 *)REG_SIOMLT_SEND = 0;			//���M���W�X�^�������i�{�^���A�łɂ��듮��ւ̑΍�j

//	DmaClear(3, 0, &M_s, sizeof(M_s), 32);
	CpuClear( 0, &M_s, sizeof(M_s), 32);

//	SendBufInit();	//���M�o�b�t�@�̏������iFIFO�o�b�t�@���u0�v�ŏ���������ꍇ�͎��s����K�v�Ȃ��j
//	RecvBufInit();	//��M�o�b�t�@�̏������iFIFO�o�b�t�@���u0�v�ŏ���������ꍇ�͎��s����K�v�Ȃ��j

	//���ϐ��̏�����
	SioIntrCount = 0;
	CheckSendData = 0;
	CheckRecvData = 0;
	SioStartFlag = 0;
	PlayersNumBak = 0;


	Send_Buf_Size = 0;
	Recv_Buf_Size = 0;
}


/*------------------------------------------------------------------*/
/*                     �ʐM�Ή��Q�[��������                         */
/*------------------------------------------------------------------*/
void Multi_SioGameInit(void)
{
	Multi_SioInit();		//�ʐM������
	Multi_SioStop();		//�ʐM�X�g�b�v
}


/*------------------------------------------------------------------*/
/*                     �}���`�v���C�ʐM���C��                       */
/*------------------------------------------------------------------*/
u32 Multi_SioMain(u8 *Signal_ptr, u16 *SendData_ptr, u16 (*RecvData_ptr)[MULTI_SIO_PLAYERS_MAX])
{
	switch(M_s.State){
		case MULTI_SIO_STOP:		//*****�ʐM�X�g�b�v*****//

			Multi_SioStop();
			M_s.State = MULTI_SIO_INIT;

			break;

		case MULTI_SIO_INIT:		//*****�ʐM������*****//

			if(*Signal_ptr == SIGNAL_DECISION){
				Multi_SioInit();
				M_s.State = MULTI_SIO_CONNECT_CHECK;
			}

			break;

		case MULTI_SIO_CONNECT_CHECK:		//*****�ڑ��F��*****//

			switch(*Signal_ptr){
				case SIGNAL_DECISION:
					if((M_s.Type == SIO_MULTI_PARENT) && (M_s.PlayersNum >= 2))
						M_s.Signal = SIGNAL_DECISION;	//�e�@�Ȃ�V�O�i���Z�b�g
					break;

				case SIGNAL_CANCEL:
					M_s.State = MULTI_SIO_STOP;
					*(vu16 *)REG_SIOMLT_SEND = 0;		//���M���W�X�^������
					break;

				default:	ConnectCheck();		//�ڑ��`�F�b�N
					break;
			}

			break;

		case MULTI_SIO_CONNECT_DECISION:		//*****�ڑ�����*****//

			TimerInit();		//�^�C�}�[�ݒ�
			M_s.State = MULTI_SIO_MAIN;

		case MULTI_SIO_MAIN:				//*****�ʐM���C��*****//

			PutSendData(SendData_ptr);		//���M�f�[�^�𑗐MFIFO�o�b�t�@�ɃZ�b�g
			GetRecvData(RecvData_ptr);		//��M��FIFO�o�b�t�@�����M�f�[�^�����o��

/*
#ifdef DEBUG_USERWORK
				if(RecvBuf[0][0]==0xBBBB)
					debugworkflag=1;
				if(debugworkflag)
					debugworkcnt++;
#endif
*/

			break;
	}

	*Signal_ptr = SIGNAL_OFF;	//�V�O�i��OFF

	return		M_s.ID											//�ڑ�ID
			|	M_s.PlayersNum << 2								//�ڑ��v���C���[��
			|	(M_s.Type == SIO_MULTI_PARENT) << 5				//�ڑ��^�C�v
			|	(M_s.State == MULTI_SIO_MAIN) << 6				//�ʐM���C�������ۂ�
			|	M_s.SkipFrameFlag << 8							//�t���[���X�L�b�v
			|	M_s.MultiSioVersion << 9							//ROM�o�[�W����
			|	M_s.HardError << 12								//�n�[�h�G���[
			|	M_s.CheckSumError << 13							//�`�F�b�N�T���G���[
			|	M_s.OverFlowError << 14							//�I�[�o�[�t���[�G���[�A1�F��M���A2�F���M��
			|	(M_s.SioIntervalError==1) << 16						//�^�C�}�[�ݒ�l�G���[�i�e�@�j
			|	(M_s.ID >= MULTI_SIO_PLAYERS_MAX) << 17			//�ڑ�ID�I�[�o�[�G���[
			|	(M_s.SioIntervalError==2)<<18;		//SIO���荞�ݒ�~�G���[�i�q�@�j

}


/*------------------------------------------------------------------*/
/*                        �R�l�N�^�`�F�b�N                          */
/*------------------------------------------------------------------*/
void ConnectCheck(void)
{
	SioMultiCnt SioCntBak = *(SioMultiCnt *)REG_SIOCNT;

	if(!SioCntBak.SI && SioCntBak.SD && !M_s.ID)
			M_s.Type = SIO_MULTI_PARENT;	//�e�ڑ�
	else
			M_s.Type = SIO_MULTI_CHILD;	//�q�ڑ�
}


/*------------------------------------------------------------------*/
/*                         �^�C�}�[������                             */
/*------------------------------------------------------------------*/
void TimerInit(void)
{
	if(M_s.Type){		//�e�ڑ�
		*(vu16 *)REG_MULTI_SIO_TIMER_L = MULTI_SIO_TIMER_COUNT;		//�^�C�}�[�l�̐ݒ�
		*(vu16 *)REG_MULTI_SIO_TIMER_H = (MULTI_SIO_TIMER_PRESCALER	//�v���X�P�[���l�̐ݒ�	
										| TMR_IF_ENABLE) >> 16;		//�^�C�}�[���荞�ݗv������

		ImeBak =  *(vu16 *)REG_IME;									//IME�̑ޔ�
		*(vu16 *)REG_IME = 0;										// IME ���Z�b�g

		*(vu16 *)REG_IE |= MULTI_SIO_TIMER_INTR_FLAG;				//�^�C�}�[���荞�݋���

		*(vu16 *)REG_IME = ImeBak;									//IME �Z�b�g
	}
}


/*------------------------------------------------------------------*/
//            ���MFIFO�o�b�t�@�ɑ��M�f�[�^��~��                   //
/*------------------------------------------------------------------*/
void PutSendData(u16 *SendData_ptr)
{
	u8	i;
	u8	tail;		//FIFO�o�b�t�@�̂�����
//	u16	IeBak;		//���荞�݋��t���O�̃o�b�N�A�b�v

	ImeBak = *(vu16 *)REG_IME;				//IME�̑ޔ�
	*(vu16 *)REG_IME = 0;					// IME ���Z�b�g

	#ifdef H_BLANK_INTR_ON
	IeBak = *(vu16 *)REG_IE;				//IE�̃o�b�N�A�b�v
	*(vu16 *)REG_IE = H_BLANK_INTR_FLAG;	//H���荞�݂̂݋���;
	*(vu16 *)REG_IME = 1;					//���荞�݋��iH���荞�݂̂݁j
	#endif //H_BLANK_INTR_ON

	if(MULTI_SIO_SEND_BUF.Size < MULTI_SIO_SEND_BUF_SIZE){		//���MFIFO�o�b�t�@�����t�łȂ���΃f�[�^�~��
		//FIFO�o�b�t�@�̂����ۂ��v�Z
		tail = MULTI_SIO_SEND_BUF.Head + MULTI_SIO_SEND_BUF.Size;
		if(tail>= MULTI_SIO_SEND_BUF_SIZE)
			tail -= MULTI_SIO_SEND_BUF_SIZE;

		//FIFO�o�b�t�@�ɒ~��
		for(i = 0; i < MULTI_SIO_SEND_MAX; i ++){
			#ifdef BUFFER_COMP_FUNC_ON
			CheckSendData |= *SendData_ptr;							//���M�f�[�^�̘_���a�`�F�b�N
			#endif //BUFFER_COMP_FUNC_ON

			//********���MFIFO�o�b�t�@�ɒ~��********//
			MULTI_SIO_SEND_BUF.Item[i][tail] = *SendData_ptr;

			*SendData_ptr = INVALID_DATA;							//���[�U�[���M�o�b�t�@��������
			SendData_ptr ++;
		}

	}else	M_s.OverFlowError = 1;		//���MFIFO�o�b�t�@�̃I�[�o�[�t���[���o


	#ifdef BUFFER_COMP_FUNC_ON
	if(CheckSendData != 0)
	#endif //BUFFER_COMP_FUNC_ON
	{
		MULTI_SIO_SEND_BUF.Size ++;		//�T�C�Y��1���₷
		CheckSendData = 0;				//������
	}

	#ifdef H_BLANK_INTR_ON
	*(vu16 *)REG_IME = 0;			// IME ���Z�b�g
	*(vu16 *)REG_IE = IeBak;		//IE�𕜋A
	#endif //H_BLANK_INTR_ON

	*(vu16 *)REG_IME = ImeBak;		// IME�̕��A

	Send_Buf_Size = MULTI_SIO_SEND_BUF.Size;		//���M�f�[�^�̒x��������
}


/*------------------------------------------------------------------*/
//           ��MFIFO�o�b�t�@�����M�f�[�^�����o��
/*------------------------------------------------------------------*/
void GetRecvData(u16 (*RecvData_ptr)[MULTI_SIO_PLAYERS_MAX])
{
	u8	i, j;
//	u16	IeBak;		//���荞�݋��t���O�̃o�b�N�A�b�v

	ImeBak = *(vu16 *)REG_IME;				//IME�̑ޔ�
	*(vu16 *)REG_IME = 0;					// IME ���Z�b�g

	#ifdef H_BLANK_INTR_ON
	IeBak = *(vu16 *)REG_IE;				//IE�̃o�b�N�A�b�v
	*(vu16 *)REG_IE = H_BLANK_INTR_FLAG;	//H���荞�݂̂݋���;
	*(vu16 *)REG_IME = 1;					//���荞�݋��iH���荞�݂̂݁j
	#endif //H_BLANK_INTR_ON

	if(MULTI_SIO_RECV_BUF.Size == 0){
		for(i = 0; i < MULTI_SIO_RECV_MAX; i++){
			for(j = 0; j < M_s.PlayersNum; j++)
				RecvData_ptr[i][j] = INVALID_DATA;		//���[�U�[��M�o�b�t�@��������
		}

		M_s.SkipFrameFlag = 1;		//�t���[���X�L�b�v�t���O�𗧂Ă�

	}else{
		//FIFO�o�b�t�@����f�[�^�����o���A���[�U�[�o�b�t�@�֎󂯓n��
		for(i = 0; i < MULTI_SIO_RECV_MAX; i++){
			for(j = 0; j < M_s.PlayersNum; j ++){
				RecvData_ptr[i][j] = MULTI_SIO_RECV_BUF.Item[j][i][MULTI_SIO_RECV_BUF.Head];	//��M�f�[�^�̎��o��
//				MULTI_SIO_RECV_BUF.Item[j][i][MULTI_SIO_RECV_BUF.Head] = EMPTY_DATA;			//�o�b�t�@���J���f�[�^�ŏ�����
			}
		}

		MULTI_SIO_RECV_BUF.Size --;		//�T�C�Y���f�N�������g
		MULTI_SIO_RECV_BUF.Head ++;		//�w�b�h���ړ�

		if(MULTI_SIO_RECV_BUF.Head >= MULTI_SIO_RECV_BUF_SIZE)
			MULTI_SIO_RECV_BUF.Head = 0;

		M_s.SkipFrameFlag = 0;		//�t���[���X�L�b�v�t���O�����낷
	}

	#ifdef H_BLANK_INTR_ON
	*(vu16 *)REG_IME = 0;			//IME ���Z�b�g
	*(vu16 *)REG_IE = IeBak;		//IE�𕜋A
	#endif //H_BLANK_INTR_ON

	*(vu16 *)REG_IME = ImeBak;		// IME�̕��A
}


/*==================================================================*/
/*==================================================================*/
/*                      ���荞�݊֐�                               */
/*==================================================================*/
/*==================================================================*/

/*------------------------------------------------------------------*/
/*                 �ʐM�pV�u�����N���荞�݊֐�                     */
/*------------------------------------------------------------------*/
void MltSioVblkIntr(void)
{
	if(M_s.Type){			//�e���̏���
		switch(M_s.State){
			case MULTI_SIO_MAIN:
				if(M_s.SioIntrCheckFlag <= MULTI_SIO_SEND_MAX){	//1�t���[�����ɐݒ�񐔂̒ʐM���I�����Ă��邩�H
					if(M_s.HardError != 1){
						M_s.SioIntervalError = 1;	//�ʐM�Ԋu�ݒ�l�G���[
						break;
					}

				}else{
#if 1
					if(M_s.SioIntervalError == 1)	break;		//�C���^�[�o���G���[�������Ă���΁A�ʐM���J�n���Ȃ�
#endif
					M_s.SioIntrCheckFlag = 0;		//���荞�݃`�F�b�N�t���O������
				}

			case MULTI_SIO_CONNECT_CHECK:
				Multi_SioStart();				//********�ʐM�J�n********//
				break;

			default:	break;
		}

	}else{	//�q�@���̏���
		if(M_s.State == MULTI_SIO_MAIN || M_s.State == MULTI_SIO_CONNECT_CHECK){
			if(++SioIntrCount > MULTI_SIO_INTR_TIME_OVER){	//�ʐM���荞�݂�������Ȃ��Ȃ����Ƃ��̏���
				if(M_s.State == MULTI_SIO_MAIN)
					M_s.SioIntervalError = 2;				//�ʐM���C�����Ȃ�ʐM�X�g�b�v�G���[
				if(M_s.State == MULTI_SIO_CONNECT_CHECK){
					M_s.PlayersNum = 0;	//�ڑ��`�F�b�N���Ȃ�ڑ��l����0�ɏ�����
					M_s.MultiSioVersion = 0;
				}
			}
		}
	}
}


/*------------------------------------------------------------------*/
/*                      �^�C�}�[���荞�݊֐�                        */
/*------------------------------------------------------------------*/
void Multi_SioTimerIntr(void)
{
	StopTimer();		//�^�C�}�[���~
	Multi_SioStart();	//�ʐM�J�n
}


/*------------------------------------------------------------------*/
/*                      �ʐM���荞�݊֐�                            */
/*------------------------------------------------------------------*/
void Multi_SioIntr(void)
{
	M_s.ID = ((SioMultiCnt *)REG_SIOCNT)->ID;	//ID�i�[

	switch(M_s.State){
		case MULTI_SIO_MAIN:	//�}���`�v���C�ʐM���C��
			M_s.HardError = ((SioMultiCnt *)REG_SIOCNT)->Error;	//�n�[�h�G���[���o

			SetRecvData();		//��M�f�[�^�̏���
			SetSendData();		//���M�f�[�^�̃Z�b�g
			SetTimer();		//�^�C�}�[�̃Z�b�g

//			SioIntrCount=0;	//SIO���荞�݃J�E���g������

			break;

		case MULTI_SIO_CONNECT_CHECK:			//�ڑ��`�F�b�N
			if(MultiSioLinkCheck()){
				if(M_s.Type){
					M_s.State = MULTI_SIO_CONNECT_DECISION;		//�e�@�͒ʐM����V�[�P���X��
					M_s.SioIntrCheckFlag = MULTI_SIO_SEND_MAX;

				}else
					M_s.State = MULTI_SIO_MAIN;					//�q�@�͒ʐM���C����
			}

		default:	break;
	}

	M_s.SioIntrCheckFlag ++;		//�ʐM���荞�݃`�F�b�N�t���O
	SioIntrCount=0;	//SIO���荞�݃J�E���g������

	if(M_s.SioIntrCheckFlag == MULTI_SIO_SEND_MAX)
		Recv_Buf_Size = MULTI_SIO_RECV_BUF.Size;	//��MFIFO�̒x��������
}


/*==================================================================*/
/*                    ���荞�݊֐��T�u���[�`��                     */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*                      �ʐM�J�n                                   */
/*-----------------------------------------------------------------*/
void Multi_SioStart(void)
{
	*(vu16 *)REG_SIOCNT |= SIO_START;	//�ʐM�X�^�[�g
}


/*---------------------------------------------------*/
/*             �ڑ��`�F�b�N                          */
/*---------------------------------------------------*/
u8 MultiSioLinkCheck(void)
{
	u8	i;
	u8	connect_num = 0;		//�ڑ��l��
	u16 checkdata_bak = 0xffff;	//�v���O�����o�[�W�����Ď��p


	if(M_s.Signal == SIGNAL_DECISION)
		*(vu16 *)REG_SIOMLT_SEND = SIGNAL_DATA;
	else
		*(vu16 *)REG_SIOMLT_SEND = CONNECT_CHECK_DATA;	//�ڑ��`�F�b�N�f�[�^���Z�b�g(�ʐM�v���O�����̃o�[�W�������j

		*(u64 *)M_s.CheckData = *(u64 *)REG_SIOMLT_RECV;
		*(vu64 *)REG_SIOMLT_RECV = 0;

		M_s.Signal = SIGNAL_OFF;	//�V�O�i�����I�t

/*
	//�V�O�i���f�[�^����M������V�[�P���X��i�߂�
	if(M_s.CheckData[MULTI_SIO_ID0] == SIGNAL_DATA)		return 1;
*/
	for(i = 0; i < MULTI_SIO_PLAYERS_MAX; i ++){		//�ڑ��v���C���[�����`�F�b�N
		if((CONNECT_CHECK_DATA&~VERSION_CHECK_MASK)==(M_s.CheckData[i]&~VERSION_CHECK_MASK)
		|| (M_s.CheckData[i]==SIGNAL_DATA) ){
			connect_num ++;

			if((checkdata_bak>M_s.CheckData[i]) && (M_s.CheckData[i] !=0))	//�u0�v�͐ڑ��f�[�^�ł͂Ȃ��I
				checkdata_bak = M_s.CheckData[i];

		}else if(M_s.CheckData[i] != NOT_CONNECTED_DATA){	//�ڑ��n�[�h��1��ł��قȂ�\�t�g���Ȃ����Ă��ꍇ�͐ڑ��l����0�ɂ���
			connect_num = 0;
			break;			//���[�v����

		}else if(M_s.CheckData[i] == NOT_CONNECTED_DATA){
			// 1P,2P,4P�ڑ��ɂ�1P�A2P�̐ڑ��m���A4P���j�^�ƂȂ�Ǐ�ւ̑Ώ�
#if 0
			if( (i==MULTI_SIO_ID3) && (M_s.ID == MULTI_SIO_ID3) ){
				M_s.State = MULTI_SIO_STOP;
				connect_num = 0;
			}
#else
			break;
#endif
//			if(i<3 && M_s.CheckData[i+1] != NOT_CONNECTED_DATA)	connect_num = 0;
//			break;			//���[�v����
		}
	}

	M_s.PlayersNum = connect_num;		//�ڑ��v���C���[���̌���


	//����V�O�i������M���ڑ��l�����O�t���[���̐ڑ��l���Ɠ������ڑ��l����2�l�ȏ�Ȃ�V�[�P���X��i�߂�
	if( (M_s.PlayersNum >= 2) && (M_s.PlayersNum == PlayersNumBak)
	&& (M_s.CheckData[MULTI_SIO_ID0] == SIGNAL_DATA) )		return 1;

	if(M_s.PlayersNum>=2)
		M_s.MultiSioVersion = (checkdata_bak&VERSION_CHECK_MASK)+1;
	else
		M_s.MultiSioVersion = 0;


	PlayersNumBak = M_s.PlayersNum;	//�ڑ��l���̃o�b�N�A�b�v

	return 0;
}


/*---------------------------------------------------*/
/*         ��M�f�[�^�̏���                          */
/*---------------------------------------------------*/
void SetRecvData(void)
{
	u8	i;
	u8	tail;
	u16	RecvDataBak[4];

	*(u64 *)RecvDataBak = *(u64 *)REG_SIOMLT_RECV;	//��M�f�[�^�̃o�b�N�A�b�v���Ƃ�

	if(M_s.SendCounter == 0)		//�_�~�[�ʐM
	{
		//�e�t���[��1��ڂ̃_�~�[�ʐM�𗘗p���āA�T���`�F�b�N���s��
		#ifdef SUM_CHECK_FUNC_ON
		for(i = 0; i < M_s.PlayersNum; i ++){
//			if((M_s.CheckSum != RecvDataBak[i]) && (M_s.CheckSum != 0)){		//�`�F�b�N�T������
			if((M_s.CheckSum != RecvDataBak[i]) && SioStartFlag )	M_s.CheckSumError = 1;	//�`�F�b�N�T���G���[
		}
		#endif //SUM_CHECK_FUNC_ON

		M_s.CheckSum = 0;		//������
		SioStartFlag = 1;

	}else{			//�L���ʐM
		//��MFIFO�o�b�t�@�̂����ۂ��Z�o
		tail = MULTI_SIO_RECV_BUF.Head + MULTI_SIO_RECV_BUF.Size;
		if(tail>=MULTI_SIO_RECV_BUF_SIZE)
			tail -= MULTI_SIO_RECV_BUF_SIZE;

		if(MULTI_SIO_RECV_BUF.Size < MULTI_SIO_RECV_BUF_SIZE){
			for(i = 0; i < M_s.PlayersNum; i ++){			//�ڑ��l�����������[�v
				#ifdef SUM_CHECK_FUNC_ON
				//�ݒ�񐔖ڂ̎�M�f�[�^���Ď��iif�������R�����g�A�E�g�őS��M�f�[�^�T���`�F�b�N�j
//				if(M_s.RecvCounter == CHECK_SUM_NUMBER)
					M_s.CheckSum += RecvDataBak[i];		//��M�f�[�^�̃`�F�b�N�T���v�Z
				#endif //SUM_CHECK_FUNC_ON

				//��M�f�[�^�̘_���a���Ƃ�
				#ifdef BUFFER_COMP_FUNC_ON
				CheckRecvData |= RecvDataBak[i];
				#endif //BUFFER_COMP_FUNC_ON

				//********��M�f�[�^��FIFO�o�b�t�@�Ɋi�[********//
				MULTI_SIO_RECV_BUF.Item[i][M_s.RecvCounter][tail] = RecvDataBak[i];
			}

		}else	M_s.OverFlowError = 2;		//��MFIFO�o�b�t�@�̃I�[�o�[�t���[���o

		M_s.RecvCounter ++;				//�f�[�^��M�񐔂��C���N�������g

		//�ڑ��n�[�h����̂��ׂĂ̎�M�f�[�^��0�Ȃ��M�o�b�t�@���P�i����
		if(M_s.RecvCounter == MULTI_SIO_RECV_MAX){
			#ifdef BUFFER_COMP_FUNC_ON
			if(CheckRecvData != 0)
			#endif //BUFFER_COMP_FUNC_ON
			{
				MULTI_SIO_RECV_BUF.Size ++;		//�L���f�[�^�Ȃ�T�C�Y���C���N�������g
				CheckRecvData = 0;				//������
			}
		}
	}
}


/*---------------------------------------------------------*/
/*           ���M�f�[�^�Z�b�g                              */
/*---------------------------------------------------------*/
void SetSendData(void)
{
	if(M_s.SendCounter == MULTI_SIO_SEND_MAX){		//���M�ŏI��
		#ifdef SUM_CHECK_FUNC_ON
		*(vu16 *)REG_SIOMLT_SEND = M_s.CheckSum;	//�T���`�F�b�N�f�[�^�𑗐M���W�X�^�ɃZ�b�g����
		#endif //SUM_CHECK_FUNC_ON

		//�ݒ�񐔂̑��M�f�[�^�Z�b�g���I�������FIFO�o�b�t�@����
		if(CheckSendBufFlag == 0){
			MULTI_SIO_SEND_BUF.Size --;		//�T�C�Y�����炷
			MULTI_SIO_SEND_BUF.Head ++;

			if(MULTI_SIO_SEND_BUF.Head >= MULTI_SIO_SEND_BUF_SIZE)	//�w�b�h�����炷
				MULTI_SIO_SEND_BUF.Head = 0;

		}else	CheckSendBufFlag = 0;	//�t���O�̏�����

	}else{								//�L�����M�f�[�^�Z�b�g
		if(CheckSendBufFlag == 0){	//���MFIFO�o�b�t�@�̃T�C�Y������(�ʏ��M_s.SendCounter==0�̂Ƃ������Ƀ`�F�b�N����΂������A�ʐM��~�ɂ��J�E���^�̂���␳�ɑΏ����邽�߂ɖ���`�F�b�N���Ă���j
			if(MULTI_SIO_SEND_BUF.Size == 0)	CheckSendBufFlag = 1;	//���M�f�[�^���~�ς���Ă��Ȃ�
		}

		if(CheckSendBufFlag)
			*(vu16 *)REG_SIOMLT_SEND = INVALID_DATA;	//���M�f�[�^�̒~�ς��Ȃ��ꍇ�͖����f�[�^���Z�b�g
		else{
			//********���M�f�[�^�𑗐M���W�X�^�ɃZ�b�g********//
			*(vu16 *)REG_SIOMLT_SEND = MULTI_SIO_SEND_BUF.Item[M_s.SendCounter][MULTI_SIO_SEND_BUF.Head];	//���M�f�[�^���Z�b�g
//			MULTI_SIO_SEND_BUF.Item[M_s.SendCounter][MULTI_SIO_SEND_BUF.Head] = EMPTY_DATA;					//�o�b�t�@���J���f�[�^�ŏ�����
		}

		M_s.SendCounter ++;			//�L�����M�f�[�^ �Z�b�g�񐔂��C���N�������g
	}
}


/*----------------------------------------------------------*/
/*                   �^�C�}�[�X�g�b�v                       */
/*----------------------------------------------------------*/
void StopTimer(void)
{
	if(M_s.Type){
		*(vu16 *)REG_MULTI_SIO_TIMER_H &= ~MULTI_SIO_TIMER_START;	//�^�C�}�[���~�߂�
		*(vu16 *)REG_MULTI_SIO_TIMER_L = MULTI_SIO_TIMER_COUNT;		//�^�C�}�[�l �Đݒ�i����Ȃ������H�j
	}
}


/*------------------------------------------------------------------*/
/*                     �^�C�}�[�Z�b�g                              */
/*-----------------------------------------------------------------*/
void SetTimer(void)
{
	if(M_s.RecvCounter == MULTI_SIO_RECV_MAX){
		M_s.SendCounter = 0;		//���M�񐔂�0�ɖ߂�
		M_s.RecvCounter = 0;		//��M�񐔂�0�ɖ߂�

	}else if(M_s.Type){			//�e�@�Ȃ�^�C�}�[���ăX�^�[�g
		*(vu16 *)REG_MULTI_SIO_TIMER_H |= MULTI_SIO_TIMER_START;
	}
}


/*------------------------------------------------------------------*/
/*                      �o�b�t�@�̏�����                            */
/*------------------------------------------------------------------*/
//���M�o�b�t�@�̏�����
void SendBufInit(void)
{
	u8	i, j;

	MULTI_SIO_SEND_BUF.Size = 0;
	MULTI_SIO_SEND_BUF.Head = 0;

	for(i = 0; i < MULTI_SIO_SEND_MAX; i++){
		for(j = 0; j < MULTI_SIO_SEND_BUF_SIZE; j ++)
			MULTI_SIO_SEND_BUF.Item[i][j] = EMPTY_DATA;		//�J�� �f�[�^������
	}
}


//��M�o�b�t�@�̏�����
void RecvBufInit(void)
{
	u8	i, j, k;

	MULTI_SIO_RECV_BUF.Size = 0;
	MULTI_SIO_RECV_BUF.Head = 0;

	for(i = 0; i < MULTI_SIO_PLAYERS_MAX; i ++){
		for(j = 0; j < MULTI_SIO_RECV_MAX; j ++){
			for(k = 0; k < MULTI_SIO_RECV_BUF_SIZE; k ++)
				MULTI_SIO_RECV_BUF.Item[i][j][k] = EMPTY_DATA;		//�J�� �f�[�^������
		}
	}
}

