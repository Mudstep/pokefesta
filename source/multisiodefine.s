@********************************************************************
@*          MultiSioDefine.s                                        *
@*            �}���`�v���C�ʐM���C�u�����萔�iGAS�p�j               *
@*            �i�^�C�}�[���荞�ݑ��M �� �ʐM�f�[�^�����j            *
@*            �i���d���荞�ݐ�p�j                                  *
@*                                                                  *
@*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               *
@********************************************************************


@ ���L�̐ݒ�l�̓\�t�g�̎d�l�ɍ��킹�čœK�����ĉ������B

MULTI_SIO_BLOCK_SIZE            =   16      @ �ʐM�f�[�^�u���b�N�T�C�Y�i�ő�24Byte�j

MULTI_SIO_PLAYERS_MAX           =   4       @ �ő�v���C���[��

MULTI_SIO_SYNC_DATA             =   0xfefe  @ �����f�[�^�i0x0000�^0xffff�͋֎~�j

@ CPU�������[�NRAM�ɗ]�T���Ȃ���΃R�����g�A�E�g���ĉ������B
MULTI_SIO_DI_FUNC_FAST:                     @ SIO���荞�݋֎~�֐��������t���O�iCPU����RAM���s�j


MULTI_SIO_TIMER_NO              =   3       @ �^�C�}�[�m��
MULTI_SIO_TIMER_INTR_FLAG       =   (TIMER0_INTR_FLAG << MULTI_SIO_TIMER_NO)
                                            @ �^�C�}�[���荞�݃t���O
REG_MULTI_SIO_TIMER             =   (REG_TM0CNT + MULTI_SIO_TIMER_NO * 4)
REG_MULTI_SIO_TIMER_L           =    REG_MULTI_SIO_TIMER
REG_MULTI_SIO_TIMER_H           =   (REG_MULTI_SIO_TIMER + 2)
                                            @ �^�C�}�[���W�X�^


@ �}���`�v���C�ʐM�p�P�b�g�\���́E�I�t�Z�b�g

OFS_MSP_FRAME_COUNTER           =   0       @ �t���[���J�E���^
OFS_MSP_RECV_ERROR_FLAGS        =   1       @ ��M�G���[�t���O
OFS_MSP_CHECK_SUM               =   2       @ �`�F�b�N�T��
OFS_MSP_DATA                    =   4       @ �ʐM�f�[�^
OFS_MSP_OVERRUN_CATCH           =   (OFS_MSP_DATA + MULTI_SIO_BLOCK_SIZE)
                                            @ �I�[�o�[�����ی�̈�

MULTI_SIO_PACKET_SIZE           =   (OFS_MSP_OVERRUN_CATCH + 4)
                                            @ �\���̃T�C�Y


@ �}���`�v���C�ʐM���[�N�G���A�\���́E�I�t�Z�b�g

OFS_MS_TYPE                     =   0       @ �ڑ��֌W�i�e�^�q�j
OFS_MS_STATE                    =   1       @ �ʐM�֐��̏��
OFS_MS_CONNECTED_FLAG           =   2       @ �ڑ������t���O
OFS_MS_RECV_SUCCESS_FLAGS       =   3       @ ��M�����t���O

OFS_MS_SYNC_SEND_FLAG           =   4       @ ���M�m�F�t���O
OFS_MS_SYNC_RECV_FLAG           =   5       @ ��M�m�F�t���O

OFS_MS_START_FLAG               =   6       @ �ʐM�J�n�t���O
OFS_MS_HARD_ERROR               =   7       @ �n�[�h�G���[

OFS_MS_SYNC_SEND_FRAME_COUNTER  =   11      @ ���M�t���[���J�E���^
OFS_MS_SYNC_RECV_FRAME_COUNTER  =   12      @ ��M�t���[���J�E���^

OFS_MS_SEND_BUF_COUNTER         =   20      @ ���M�o�b�t�@�J�E���^
OFS_MS_RECV_BUF_COUNTER         =   24      @ ��M�o�b�t�@�J�E���^

OFS_MS_NEXT_SEND_BUF_P          =   28      @ ���M�o�b�t�@�|�C���^
OFS_MS_CURRENT_SEND_BUF_P       =   32
OFS_MS_CURRENT_RECV_BUF_P       =   36      @ ��M�o�b�t�@�|�C���^
OFS_MS_LAST_RECV_BUF_P          =   40
OFS_MS_RECV_CHECK_BUF_P         =   44

OFS_MS_SEND_BUF                 =   48      @ ���M�o�b�t�@�i�_�u���o�b�t�@�j
OFS_MS_RECV_BUF                 =   (OFS_MS_SEND_BUF + MULTI_SIO_PACKET_SIZE * 2)
                                            @ ��M�o�b�t�@�i�g���v���o�b�t�@�j

MULTI_SIO_AREA_SIZE             =   (OFS_MS_RECV_BUF + MULTI_SIO_PACKET_SIZE * 3 * MULTI_SIO_PLAYERS_MAX)
                                            @ �\���̃T�C�Y


    .EXTERN RecvFuncBuf                     @ CPU����RAM���s�p�o�b�t�@
    .EXTERN IntrFuncBuf


@--------------------------------------------------------------------
@-                      �}���`�v���C�ʐM������                      -
@--------------------------------------------------------------------

    .EXTERN MultiSioInit

@--------------------------------------------------------------------
@-                      �}���`�v���C�ʐM�X�^�[�g                    -
@--------------------------------------------------------------------

    .EXTERN MultiSioStart

@--------------------------------------------------------------------
@-                      �}���`�v���C�ʐM�X�g�b�v                    -
@--------------------------------------------------------------------

    .EXTERN MultiSioStop

@--------------------------------------------------------------------
@-                      �}���`�v���C�ʐM���C��                      -
@--------------------------------------------------------------------

    .EXTERN MultiSioMain

@--------------------------------------------------------------------
@-                      �}���`�v���C�ʐM�u�u�����N���荞��          -
@--------------------------------------------------------------------

    .EXTERN MultiSioVSync

@--------------------------------------------------------------------
@-                      �}���`�v���C�ʐM���荞��                    -
@--------------------------------------------------------------------

    .EXTERN MultiSioIntr

@--------------------------------------------------------------------
@-                      ��M�f�[�^ �`�F�b�N                         -
@--------------------------------------------------------------------

    .EXTERN MultiSioRecvDataCheck

@--------------------------------------------------------------------
@-                      ���M�f�[�^ �Z�b�g                           -
@--------------------------------------------------------------------

    .EXTERN MultiSioSendDataSet


