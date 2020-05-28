@********************************************************************
@*          AgbDefine.s                                             *
@*            AGB�W���萔�iGAS�p�j                                  *
@*                                                                  *
@*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               *
@********************************************************************


@--------------------------------------------------------------------
@-                            �萔                                  -
@--------------------------------------------------------------------

ON                      =   1
OFF                     =   0
ENABLE                  =   1
DISABLE                 =   0
TRUE                    =   1
FALSE                   =   0

LCD_WIDTH               =   240                 @ ��ʕ�
LCD_HEIGHT              =   160                 @ ��ʍ���

SYSTEM_CLOCK            =  (16 * 1024 * 1024)   @ �V�X�e���N���b�N


@--------------------------------------------------------------------
@-                ���W�X�^���q�`�l ���Z�b�g �V�X�e���R�[��          -
@--------------------------------------------------------------------

RESET_ALL_FLAG          =   0xff        @ �S���Z�b�g

RESET_ALL_REG_FLAG      =   0xe0        @ �S���W�X�^ ���Z�b�g

RESET_REG_FLAG          =   0x80        @ ���W�X�^�i���L�ȊO�j���Z�b�g
RESET_REG_SOUND_FLAG    =   0x40        @ �T�E���h���W�X�^    ���Z�b�g
RESET_REG_SIO_FLAG      =   0x20        @ �r�h�n���W�X�^      ���Z�b�g

RESET_ALL_RAM_FLAG      =   0x1f        @ �S�q�`�l �N���A

RESET_EX_WRAM_FLAG      =   0x01        @ �b�o�t�O���q�`�l  �N���A
RESET_CPU_WRAM_FLAG     =   0x02        @ �b�o�t�����q�`�l  �N���A
RESET_PLTT_FLAG         =   0x04        @ �p���b�g          �N���A
RESET_VRAM_FLAG         =   0x08        @ �u�q�`�l          �N���A
RESET_OAM_FLAG          =   0x10        @ �n�`�l            �N���A


@--------------------------------------------------------------------
@-                    �a�f�X�N���[���f�[�^                          -
@--------------------------------------------------------------------

BG_SC_CHAR_NO_MASK      =   0x03ff      @ �L�����N�^�m��
BG_SC_FLIP_MASK         =   0x0c00      @ �t���b�v
BG_SC_PLTT_MASK         =   0xf000      @ �J���[�p���b�g�m��

BG_SC_CHAR_NO_SHIFT     =   0
BG_SC_FLIP_SHIFT        =   10
BG_SC_PLTT_SHIFT        =   12

BG_SC_H_FLIP            =   0x0400      @ �g�t���b�v
BG_SC_V_FLIP            =   0x0800      @ �u�t���b�v

@--------------------------------------------------------------------
@-                        �A�t�B���ϊ�                              -
@--------------------------------------------------------------------

SIN_SHIFT               =   14
AFFINE_DIV_SHIFT        =   8
AFFINE_REG_SHIFT        =   8
AFFINE_CAL_SHIFT        =   SIN_SHIFT + AFFINE_DIV_SHIFT - AFFINE_REG_SHIFT

@--------------------------------------------------------------------
@-                        �n�`�l�f�[�^                              -
@--------------------------------------------------------------------

OAM_V_POS_MASK          =   0x000000ff  @ �n�a�i �w���W
OAM_H_POS_MASK          =   0x01ff0000  @ �n�a�i �x���W
OAM_SHAPE_MASK          =   0x0000c000  @ �n�a�i�`��
OAM_SIZE_MASK           =   0xc0000000  @ �n�a�i�T�C�Y
OAM_OBJ_MODE_MASK       =   0x00000c00  @ �n�a�i���[�h
OAM_AFFINE_MODE_MASK    =   0x00000300  @ �A�t�B�����[�h
OAM_AFFINE_NO_MASK      =   0x3e000000  @ �A�t�B�� �p�����[�^�m��
OAM_FLIP_MASK           =   0x30000000  @ �t���b�v
OAM_CHAR_NO_MASK        =   0x000003ff  @ �L�����N�^�m��
OAM_PRIORITY_MASK       =   0x00000c00  @ �a�f���ΗD�揇��
OAM_PLTT_MASK           =   0x0000f000  @ �J���[�p���b�g�m��
OAM_AFFINE_PARAM_MASK   =   0xffff0000  @ �A�t�B�� �p�����[�^

OAM_V_POS_SHIFT         =   0
OAM_H_POS_SHIFT         =   16
OAM_SHAPE_SHIFT         =   14
OAM_SIZE_SHIFT          =   30
OAM_AFFINE_MODE_SHIFT   =   8
OAM_OBJ_MODE_SHIFT      =   10
OAM_AFFINE_NO_SHIFT     =   25
OAM_FLIP_SHIFT          =   28
OAM_CHAR_NO_SHIFT       =   0
OAM_PRIORITY_SHIFT      =   10
OAM_PLTT_SHIFT          =   12
OAM_AFFINE_PARAM_SHIFT  =   16

OAM_OBJ_ON              =   0x00000000  @ �n�a�i �n�m
OAM_OBJ_OFF             =   0x00000200  @ �n�a�i �n�e�e
OAM_OBJ_NORMAL          =   0x00000000  @ �n�a�i�m�[�}�����[�h
OAM_OBJ_BLEND           =   0x00000400  @ �n�a�i���������[�h
OAM_OBJ_WINDOW          =   0x00000800  @ �n�a�i�E�C���h�E���[�h
OAM_AFFINE_NONE         =   0x00000000  @ �A�t�B���������[�h
OAM_AFFINE_NORMAL       =   0x00000100  @ �A�t�B���m�[�}�����[�h
OAM_AFFINE_TWICE        =   0x00000300  @ �A�t�B���Q�{�p���[�h
OAM_AFFINE_ERASE        =   0x00000200  @ �A�t�B����\�����[�h
OAM_MOS_ON              =   0x00001000  @ ���U�C�N �n�m
OAM_MOS_OFF             =   0x00000000  @ ���U�C�N �n�e�e
OAM_COLOR_16            =   0x00000000  @ �P�U     �I��
OAM_COLOR_256           =   0x00002000  @ �Q�T�U�F �I��
OAM_H_FLIP              =   0x10000000  @ �g�t���b�v
OAM_V_FLIP              =   0x20000000  @ �u�t���b�v
OAM_SQUARE              =   0x00000000  @ �����`�n�a�i
OAM_H_RECTANGLE         =   0x00004000  @ �������`�n�a�i
OAM_V_RECTANGLE         =   0x00008000  @ �c�����`�n�a�i
OAM_SIZE_NO_0           =   0x00000000  @ �n�a�i �T�C�Y�m�� �O
OAM_SIZE_NO_1           =   0x40000000  @ �n�a�i �T�C�Y�m�� �P
OAM_SIZE_NO_2           =   0x80000000  @ �n�a�i �T�C�Y�m�� �Q
OAM_SIZE_NO_3           =   0xc0000000  @ �n�a�i �T�C�Y�m�� �R
OAM_SIZE_8x8            =   0x00000000  @ �n�a�i   �W��  �W�h�b�g
OAM_SIZE_16x16          =   0x40000000  @ �n�a�i �P�U���P�U�h�b�g
OAM_SIZE_32x32          =   0x80000000  @ �n�a�i �R�Q���R�Q�h�b�g
OAM_SIZE_64x64          =   0xc0000000  @ �n�a�i �U�S���U�S�h�b�g
OAM_SIZE_16x8           =   0x00004000  @ �n�a�i �P�U��  �W�h�b�g
OAM_SIZE_32x8           =   0x40004000  @ �n�a�i �R�Q��  �W�h�b�g
OAM_SIZE_32x16          =   0x80004000  @ �n�a�i �R�Q���P�U�h�b�g
OAM_SIZE_64x32          =   0xc0004000  @ �n�a�i �U�S���R�Q�h�b�g
OAM_SIZE_8x16           =   0x00008000  @ �n�a�i   �W���P�U�h�b�g
OAM_SIZE_8x32           =   0x40008000  @ �n�a�i   �W���R�Q�h�b�g
OAM_SIZE_16x32          =   0x80008000  @ �n�a�i �P�U���R�Q�h�b�g
OAM_SIZE_32x64          =   0xc0008000  @ �n�a�i �R�Q���U�S�h�b�g


@--------------------------------------------------------------------
@-                      �J���[�p���b�g�f�[�^                        -
@--------------------------------------------------------------------

PLTT_RED_MASK           =   0x001f      @ ��
PLTT_GREEN_MASK         =   0x03e0      @ ��
PLTT_BLUE_MASK          =   0x7c00      @ ��

PLTT_RED_SHIFT          =   0
PLTT_GREEN_SHIFT        =   5
PLTT_BLUE_SHIFT         =   10


@********************************************************************
@*              �`�f�a���W�X�^�E�R���g���[���r�b�g                  *
@********************************************************************

@--------------------------------------------------------------------
@-                  �v���O�����X�e�[�^�X���W�X�^                    -
@--------------------------------------------------------------------

PSR_CPU_MODE_MASK       =   0x1f        @ �b�o�t���[�h

PSR_USER_MODE           =   0x10        @ ���[�U
PSR_FIQ_MODE            =   0x11        @ �e�h�p
PSR_IRQ_MODE            =   0x12        @ �h�q�p
PSR_SVC_MODE            =   0x13        @ �X�[�p�o�C�U
PSR_ABORT_MODE          =   0x17        @ �A�{�[�g�i�v���t�F�b�`�^�f�[�^�j
PSR_UNDEF_MODE          =   0x1b        @ ����`����
PSR_SYS_MODE            =   0x1f        @ �V�X�e��

PSR_THUMB_STATE         =   0x20        @ �s�g�t�l�a�X�e�[�g
PSR_FIQ_DISABLE         =   0x40        @ �e�h�p�s����
PSR_IRQ_DISABLE         =   0x80        @ �h�q�p�s����
PSR_IRQ_FIQ_DISABLE     =   0xc0        @ �h�q�p���e�h�p�s����

PSR_V_FLAG              =   0x10000000  @ �I�[�o�[�t���[
PSR_C_FLAG              =   0x20000000  @ �L�����[�^�{���[�^�g��
PSR_Z_FLAG              =   0x40000000  @ �[��
PSR_N_FLAG              =   0x80000000  @ ���^����


@--------------------------------------------------------------------
@-                  ���荞�ݗv���^���t���O                        -
@--------------------------------------------------------------------

V_BLANK_INTR_FLAG       =   0x0001      @ �u�u�����N���荞��
H_BLANK_INTR_FLAG       =   0x0002      @ �g�u�����N���荞��
V_COUNT_INTR_FLAG       =   0x0004      @ �u�J�E���^��v���荞��
TIMER0_INTR_FLAG        =   0x0008      @ �^�C�}�[�O���荞��
TIMER1_INTR_FLAG        =   0x0010      @ �^�C�}�[�P���荞��
TIMER2_INTR_FLAG        =   0x0020      @ �^�C�}�[�Q���荞��
TIMER3_INTR_FLAG        =   0x0040      @ �^�C�}�[�R���荞��
SIO_INTR_FLAG           =   0x0080      @ �V���A���ʐM���荞��
DMA0_INTR_FLAG          =   0x0100      @ �c�l�`�O���荞��
DMA1_INTR_FLAG          =   0x0200      @ �c�l�`�P���荞��
DMA2_INTR_FLAG          =   0x0400      @ �c�l�`�Q���荞��
DMA3_INTR_FLAG          =   0x0800      @ �c�l�`�R���荞��
KEY_INTR_FLAG           =   0x1000      @ �L�[���荞��
CASSETTE_INTR_FLAG      =   0x2000      @ �J�Z�b�g���荞��


@--------------------------------------------------------------------
@-                      �\���R���g���[��                            -
@--------------------------------------------------------------------

DISP_BG_MODE_MASK       =   0x0007      @ �a�f���[�h
DISP_ON_MASK            =   0x1f00      @ �n�a�i�E�a�f �n�m
DISP_WIN_MASK           =   0x6000      @ �E�C���h�E �n�m

DISP_BG_MODE_SHIFT      =   0
DISP_ON_SHIFT           =   8
DISP_WIN_SHIFT          =   13

DISP_MODE_0             =   0x0000      @ �a�f���[�h�O
DISP_MODE_1             =   0x0001      @ �a�f���[�h�P
DISP_MODE_2             =   0x0002      @ �a�f���[�h�Q
DISP_MODE_3             =   0x0003      @ �a�f���[�h�R
DISP_MODE_4             =   0x0004      @ �a�f���[�h�S
DISP_MODE_5             =   0x0005      @ �a�f���[�h�T
DISP_BMP_FRAME_NO       =   0x0010      @ �r�b�g�}�b�v���[�h �\���t���[��
DISP_OBJ_HOFF           =   0x0020      @ �g�u�����N���n�a�i���� �n�e�e
DISP_OBJ_CHAR_2D_MAP    =   0x0000      @ �n�a�i�L�����N�^�f�[�^ �Q�c�}�b�s���O
DISP_OBJ_CHAR_1D_MAP    =   0x0040      @ �n�a�i�L�����N�^�f�[�^ �P�c�}�b�s���O
DISP_LCDC_OFF           =   0x0080      @ �k�b�c�b �n�e�e
DISP_BG0_ON             =   0x0100      @ �a�f�O �n�m
DISP_BG1_ON             =   0x0200      @ �a�f�P �n�m
DISP_BG2_ON             =   0x0400      @ �a�f�Q �n�m
DISP_BG3_ON             =   0x0800      @ �a�f�R �n�m
DISP_BG_ALL_ON          =   0x0f00      @ �a�f�S�� �n�m
DISP_OBJ_ON             =   0x1000      @ �n�a�i �n�m
DISP_OBJ_BG_ALL_ON      =   0x1f00      @ �n�a�i�E�a�f�S�� �n�m
DISP_WIN0_ON            =   0x2000      @ �E�C���h�E�O �n�m
DISP_WIN1_ON            =   0x4000      @ �E�C���h�E�P �n�m
DISP_WIN01_ON           =   0x6000      @ �E�C���h�E�O�E�P �n�m
DISP_OBJWIN_ON          =   0x8000      @ �n�a�i�E�C���h�E �n�m
DISP_WIN_ALL_ON         =   0xe000      @ �E�C���h�E�S�� �n�m
DISP_ALL_ON             =   0x7f00      @ �S�� �n�m

@--------------------------------------------------------------------
@-                      �X�e�[�^�X�R���g���[��                      -
@--------------------------------------------------------------------

STAT_VCOUNT_CMP_MASK    =   0xff00      @ �u�J�E���^��r�l

STAT_VCOUNT_CMP_SHIFT   =   8

STAT_V_BLANK            =   0x0001      @ �u�u�����N���Ԓ�
STAT_H_BLANK            =   0x0002      @ �g�u�����N���Ԓ�
STAT_V_COUNT            =   0x0004      @ �u�J�E���^��v��
STAT_V_BLANK_IF_ENABLE  =   0x0008      @ �u�u�����N���荞�ݗv�� ����
STAT_H_BLANK_IF_ENABLE  =   0x0010      @ �g�u�����N���荞�ݗv�� ����
STAT_V_COUNT_IF_ENABLE  =   0x0020      @ �u�J�E���^��v���荞�ݗv�� ����

@--------------------------------------------------------------------
@-                      �a�f�R���g���[��                            -
@--------------------------------------------------------------------

BG_PRIORITY_MASK        =   0x0003      @ �a�f�ԗD�揇��
BG_CHAR_BASE_MASK       =   0x000c      @ �L�����N�^ �x�[�X�A�h���X
BG_SCREEN_BASE_MASK     =   0x1f00      @ �X�N���[�� �x�[�X�A�h���X
BG_SCREEN_SIZE_MASK     =   0xc000      @ �X�N���[���T�C�Y(256/512)

BG_PRIORITY_SHIFT       =   0
BG_CHAR_BASE_SHIFT      =   2
BG_SCREEN_BASE_SHIFT    =   8
BG_SCREEN_SIZE_SHIFT    =   14

BG_PRIORITY_0           =   0x0000      @ �a�f��O�D��
BG_PRIORITY_1           =   0x0001      @ �a�f��P�D��
BG_PRIORITY_2           =   0x0002      @ �a�f��Q�D��
BG_PRIORITY_3           =   0x0003      @ �a�f��R�D��
BG_MOS_ON               =   0x0040      @ ���U�C�N �n�m
BG_MOS_OFF              =   0x0000      @ ���U�C�N �n�e�e
BG_COLOR_16             =   0x0000      @ �P�U�F   �I��
BG_COLOR_256            =   0x0080      @ �Q�T�U�F �I��
BG_LOOP_ON              =   0x2000      @ ���[�v �n�m
BG_LOOP_OFF             =   0x0000      @ ���[�v �n�e�e
BG_SCREEN_SIZE_0        =   0x0000      @ �X�N���[���T�C�Y(256x256)
BG_SCREEN_SIZE_1        =   0x4000      @ �X�N���[���T�C�Y(512x256)
BG_SCREEN_SIZE_2        =   0x8000      @ �X�N���[���T�C�Y(256x512)
BG_SCREEN_SIZE_3        =   0xc000      @ �X�N���[���T�C�Y(512x512)

@--------------------------------------------------------------------
@-                      �E�C���h�E �R���g���[��                     -
@--------------------------------------------------------------------

WIN_END_POS_MASK        =   0x00ff      @ �E�C���h�E�I���_
WIN_START_POS_MASK      =   0xff00      @ �E�C���h�E�J�n�_

WIN_END_POS_SHIFT       =   0
WIN_START_POS_SHIFT     =   8

WIN_BG0_ON              =   0x0001      @ �a�f�O �n�m
WIN_BG1_ON              =   0x0002      @ �a�f�P �n�m
WIN_BG2_ON              =   0x0004      @ �a�f�Q �n�m
WIN_BG3_ON              =   0x0008      @ �a�f�R �n�m
WIN_OBJ_ON              =   0x0010      @ �n�a�i �n�m
WIN_BLEND_ON            =   0x0020      @ �u�����h �n�m
WIN_ALL_ON              =   0x003f      @ �S�� �n�m

@--------------------------------------------------------------------
@-                      ���U�C�N �R���g���[��                       -
@--------------------------------------------------------------------

MOS_H_SIZE_MASK         =   0x000f      @       ���U�C�N����
MOS_V_SIZE_MASK         =   0x00f0      @       ���U�C�N��
MOS_BG_H_SIZE_MASK      =   0x000f      @   �a�f���U�C�N����
MOS_BG_V_SIZE_MASK      =   0x00f0      @   �a�f���U�C�N��
MOS_OBJ_H_SIZE_MASK     =   0x000f      @ �n�a�i���U�C�N����
MOS_OBJ_V_SIZE_MASK     =   0x00f0      @ �n�a�i���U�C�N��

MOS_H_SIZE_SHIFT        =   0
MOS_V_SIZE_SHIFT        =   4
MOS_BG_H_SIZE_SHIFT     =   0
MOS_BG_V_SIZE_SHIFT     =   4
MOS_OBJ_H_SIZE_SHIFT    =   8
MOS_OBJ_V_SIZE_SHIFT    =   12

@--------------------------------------------------------------------
@-                      �u�����h �R���g���[��                       -
@--------------------------------------------------------------------

BLD_PIXEL_MASK          =   0x003f      @ �s�N�Z���I��
BLD_1ST_PIXEL_MASK      =   0x003f      @ �P�����s�N�Z���I��
BLD_MODE_MASK           =   0x00c0      @ �u�����h���[�h
BLD_2ND_PIXEL_MASK      =   0x3f00      @ �Q�����s�N�Z���I��

BLD_1ST_PIXEL_SHIFT     =   0
BLD_MODE_SHIFT          =   6
BLD_2ND_PIXEL_SHIFT     =   8

BLD_BG0                 =   0x0001      @ �a�f�O �s�N�Z���I��
BLD_BG1                 =   0x0002      @ �a�f�P �s�N�Z���I��
BLD_BG2                 =   0x0004      @ �a�f�Q �s�N�Z���I��
BLD_BG3                 =   0x0008      @ �a�f�R �s�N�Z���I��
BLD_OBJ                 =   0x0010      @ �n�a�i �s�N�Z���I��
BLD_BD                  =   0x0020      @ �w�i�F �s�N�Z���I��
BLD_ALL                 =   0x003f      @        �s�N�Z���S�I��
BLD_BG0_1ST             =   0x0001      @ �a�f�O �P�����s�N�Z���I��
BLD_BG1_1ST             =   0x0002      @ �a�f�P �P�����s�N�Z���I��
BLD_BG2_1ST             =   0x0004      @ �a�f�Q �P�����s�N�Z���I��
BLD_BG3_1ST             =   0x0008      @ �a�f�R �P�����s�N�Z���I��
BLD_OBJ_1ST             =   0x0010      @ �n�a�i �P�����s�N�Z���I��
BLD_BD_1ST              =   0x0020      @ �w�i�F �P�����s�N�Z���I��
BLD_1ST_ALL             =   0x003f      @        �P�����s�N�Z���S�I��
BLD_NORMAL_MODE         =   0x0000      @ �m�[�}�����[�h
BLD_A_BLEND_MODE        =   0x0040      @ ���������[�h
BLD_UP_MODE             =   0x0080      @ �P�x�t�o���[�h
BLD_DOWN_MODE           =   0x00c0      @ �P�x�c�n�v�m���[�h
BLD_BG0_2ND             =   0x0100      @ �a�f�O �Q�����s�N�Z���I��
BLD_BG1_2ND             =   0x0200      @ �a�f�P �Q�����s�N�Z���I��
BLD_BG2_2ND             =   0x0400      @ �a�f�Q �Q�����s�N�Z���I��
BLD_BG3_2ND             =   0x0800      @ �a�f�R �Q�����s�N�Z���I��
BLD_OBJ_2ND             =   0x1000      @ �n�a�i �Q�����s�N�Z���I��
BLD_BD_2ND              =   0x2000      @ �w�i�F �Q�����s�N�Z���I��
BLD_2ND_ALL             =   0x3f00      @        �Q�����s�N�Z���S�I��


BLD_A_MASK              =   0x001f      @ �u�����h�ϐ� �`
BLD_B_MASK              =   0x1f00      @ �u�����h�ϐ� �a
BLD_Y_MASK              =   0x001f      @ �u�����h�ϐ� �x

BLD_A_SHIFT             =   16
BLD_B_SHIFT             =   24
BLD_Y_SHIFT             =   0


@--------------------------------------------------------------------
@-                      �T�E���h �R���g���[��                       -
@--------------------------------------------------------------------

SOUND_SO1_LEVEL_MASK    =   0x03        @ �r�n�P�o�̓��x��
SOUND_SO2_LEVEL_MASK    =   0x30        @ �r�n�Q�o�̓��x��
SOUND_DMG_SO1_ON_MASK   =   0x0f        @ �c�l�f�݊��T�E���h���r�n�P �n�m
SOUND_DMG_SO2_ON_MASK   =   0xf0        @ �c�l�f�݊��T�E���h���r�n�Q �n�m

SOUND_SWEEP_SHIFT_MASK  =   0x03        @ �X�C�[�v �V�t�g��
SOUND_SWEEP_TIME_MASK   =   0x30        @ �X�C�[�v ����
SOUND_COUNTS_MASK       =   0x3f        @ ���̒���(�J�E���g��)
SOUND_DUTY_MASK         =   0xc0        @ �g�`�̃f���[�e�B
SOUND_ENV_STEPS_MASK    =   0x03        @ �G���x���[�v �X�e�b�v��
SOUND_ENV_INIT_MASK     =   0xf0        @ �G���x���[�v �����l
SOUND_FREQUENCY_MASK    =   0x03ff      @ ���g��
SOUND_FREQUENCY_L_MASK  =   0xff
SOUND_FREQUENCY_H_MASK  =   0x03

SOUND_3_COUNTS_MASK     =   0xff        @ �T�E���h�R ���̒���(�J�E���g��)
SOUND_3_LEVEL_MASK      =   0xe0        @            �o�̓��x��

SOUND_4_PRESCALER_MASK  =   0x03        @ �T�E���h�S �v���X�P�[���I��
SOUND_4_POLYSHIFT_MASK  =   0xf0        @            �|���m�~�J���E�J�E���^ �V�t�g��


SOUND_SO1_LEVEL_SHIFT   =   0
SOUND_SO2_LEVEL_SHIFT   =   4
SOUND_DMG_SO1_ON_SHIFT  =   0
SOUND_DMG_SO2_ON_SHIFT  =   4

SOUND_SWEEP_SHIFT_SHIFT =   0
SOUND_SWEEP_TIME_SHIFT  =   4
SOUND_COUNTS_SHIFT      =   0
SOUND_DUTY_SHIFT        =   6
SOUND_ENV_STEPS_SHIFT   =   0
SOUND_ENV_INIT_SHIFT    =   4
SOUND_FREQUENCY_SHIFT   =   0
SOUND_FREQUENCY_L_SHIFT =   0
SOUND_FREQUENCY_H_SHIFT =   8

SOUND_3_COUNTS_SHIFT    =   0
SOUND_3_LEVEL_SHIFT     =   5

SOUND_4_PRESCALER_SHIFT =   0
SOUND_4_POLYSHIFT_SHIFT =   4


SOUND_1_ON              =   0x01        @ �T�E���h�P �n�m
SOUND_2_ON              =   0x02        @ �T�E���h�Q �n�m
SOUND_3_ON              =   0x04        @ �T�E���h�R �n�m
SOUND_4_ON              =   0x08        @ �T�E���h�S �n�m
SOUND_DMG_ON            =   0x80        @ �c�l�f�݊��T�E���h �n�m

SOUND_1_SO1_ON          =   0x01        @ �T�E���h�P���r�n�P �n�m
SOUND_2_SO1_ON          =   0x02        @ �T�E���h�Q���r�n�P �n�m
SOUND_3_SO1_ON          =   0x04        @ �T�E���h�R���r�n�P �n�m
SOUND_4_SO1_ON          =   0x08        @ �T�E���h�S���r�n�P �n�m
SOUND_1_SO2_ON          =   0x10        @ �T�E���h�P���r�n�Q �n�m
SOUND_2_SO2_ON          =   0x20        @ �T�E���h�Q���r�n�Q �n�m
SOUND_3_SO2_ON          =   0x40        @ �T�E���h�R���r�n�Q �n�m
SOUND_4_SO2_ON          =   0x80        @ �T�E���h�S���r�n�Q �n�m
SOUND_DMG_ALL_SO1_ON    =   0x0f        @ �c�l�f�݊��T�E���h���r�n�P �n�m
SOUND_DMG_ALL_SO2_ON    =   0xf0        @ �c�l�f�݊��T�E���h���r�n�Q �n�m
SOUND_DMG_ALL_SO_ON     =   0xff        @ �c�l�f�݊��T�E���h���r�n�P�^�Q �n�m
SOUND_VIN_SO1_ON        =   0x80        @ �u�������r�n�P �n�m
SOUND_VIN_SO2_ON        =   0x08        @ �u�������r�n�Q �n�m
SOUND_VIN_SO_ON         =   0x08        @ �u�������r�n�P�^�Q �n�m

SOUND_A_SO1_ON          =   0x01        @ �_�C���N�g�T�E���h�`���r�n�P �n�m
SOUND_A_SO2_ON          =   0x02        @ �_�C���N�g�T�E���h�`���r�n�Q �n�m
SOUND_B_SO1_ON          =   0x10        @ �_�C���N�g�T�E���h�a���r�n�P �n�m
SOUND_B_SO2_ON          =   0x20        @ �_�C���N�g�T�E���h�a���r�n�Q �n�m
SOUND_A_ALL_SO_ON       =   0x03        @ �_�C���N�g�T�E���h�`���r�n�P�^�Q �n�m
SOUND_B_ALL_SO_ON       =   0x30        @ �_�C���N�g�T�E���h�a���r�n�P�^�Q �n�m
SOUND_DIRECT_ALL_SO1_ON =   0x11        @ �_�C���N�g�T�E���h�`�^�a���r�n�P �n�m
SOUND_DIRECT_ALL_SO2_ON =   0x22        @ �_�C���N�g�T�E���h�`�^�a���r�n�Q �n�m
SOUND_DIRECT_ALL_SO_ON  =   0x33        @ �_�C���N�g�T�E���h�`�^�a���r�n�P�^�Q �n�m
SOUND_A_TIMER_0         =   0x00        @ �^�C�}�[�O���_�C���N�g�T�E���h�`
SOUND_A_TIMER_1         =   0x04        @ �^�C�}�[�P���_�C���N�g�T�E���h�`
SOUND_B_TIMER_0         =   0x00        @ �^�C�}�[�O���_�C���N�g�T�E���h�a
SOUND_B_TIMER_1         =   0x40        @ �^�C�}�[�P���_�C���N�g�T�E���h�a
SOUND_A_FIFO_RESET      =   0x08        @ �_�C���N�g�T�E���h�`�p�e�h�e�n ���Z�b�g
SOUND_B_FIFO_RESET      =   0x80        @ �_�C���N�g�T�E���h�a�p�e�h�e�n ���Z�b�g

SOUND_DMG_MIX_1_4       =   0x00        @   �c�l�f�݊��T�E���h������ �P�^�S
SOUND_DMG_MIX_2_4       =   0x01        @                            �Q�^�S
SOUND_DMG_MIX_FULL      =   0x02        @                            �t�������W
SOUND_A_MIX_1_2         =   0x00        @ �_�C���N�g�T�E���h�`������ �P�^�Q
SOUND_A_MIX_FULL        =   0x04        @                            �t�������W
SOUND_B_MIX_1_2         =   0x00        @ �_�C���N�g�T�E���h�a������ �P�^�Q
SOUND_B_MIX_FULL        =   0x08        @                            �t�������W
SOUND_ALL_MIX_FULL      =   0x0e        @           �S�T�E���h������ �t�������W


SOUND_INIT_ON           =   0x80        @ �C�j�V���� �n�m
SOUND_INIT_OFF          =   0x00        @ �C�j�V���� �n�m
SOUND_COUNTER_ON        =   0x40        @   �J�E���^ �n�m
SOUND_COUNTER_OFF       =   0x00        @   �J�E���^ �n�e�e

SOUND_SWEEP_UP          =   0x00        @ �X�C�[�v ���Z
SOUND_SWEEP_DOWN        =   0x08        @          ���Z
SOUND_DUTY_1_8          =   0x00        @ �g�`�̃f���[�e�B �P�^�W
SOUND_DUTY_2_8          =   0x40        @                  �Q�^�W
SOUND_DUTY_4_8          =   0x80        @                  �S�^�W
SOUND_DUTY_6_8          =   0xc0        @                  �U�^�W
SOUND_ENV_UP            =   0x08        @ �G���x���[�v �t�o
SOUND_ENV_DOWN          =   0x00        @              �c�n�v�m

SOUND_3_LOCAL_ON        =   0x80        @ �T�E���h�R �n�m
SOUND_3_WAVE_BANK_0     =   0x00        @ �T�E���h�R �g�`�q�`�l �o���N�O
SOUND_3_WAVE_BANK_1     =   0x40        @                       �o���N�P
SOUND_3_WAVE_CONNECT    =   0x20        @ �T�E���h�R �g�`�q�`�l �ڑ�
SOUND_3_WAVE_DISCONNECT =   0x00        @                       ��ڑ�
SOUND_3_LEVEL_0_4       =   0x00        @ �T�E���h�R �o�� �Ȃ�
SOUND_3_LEVEL_FULL      =   0x20        @                 �S�^�S
SOUND_3_LEVEL_2_4       =   0x40        @                 �Q�^�S
SOUND_3_LEVEL_1_4       =   0x60        @                 �P�^�S
SOUND_3_LEVEL_3_4       =   0x80        @                 �R�^�S

SOUND_4_POLYSTEP_15     =   0x00        @ �T�E���h�S �|���m�~�J���E�J�E���^ �P�T�i
SOUND_4_POLYSTEP_7      =   0x08        @                                     �V�i


@--------------------------------------------------------------------
@-                      �r�h�n�R���g���[��                          -
@--------------------------------------------------------------------

SIO_MODE_MASK           =   0x3000      @ �ʐM���[�h
SIO_BAUD_RATE_MASK      =   0x0003      @ �{�[���[�g
SIO_ID_NO_MASK          =   0x0030      @ �ʐM�h�c

SIO_MODE_SHIFT          =   12
SIO_BAUD_RATE_SHIFT     =   0
SIO_ID_NO_SHIFT         =   4

SIO_8BIT_MODE           =   0x0000      @ �m�[�}���W�r�b�g�ʐM���[�h
SIO_32BIT_MODE          =   0x1000      @ �m�[�}���R�Q�r�b�g�ʐM���[�h
SIO_MULTI_MODE          =   0x2000      @ �}���`�v���C�ʐM���[�h
SIO_UART_MODE           =   0x3000      @ �t�`�q�s�ʐM���[�h

SIO_SCK_OUT             =   0x0000      @ �O���N���b�N�I��
SIO_SCK_IN              =   0x0001      @ �����N���b�N�I��
SIO_IN_SCK_256K         =   0x0000      @ �����N���b�N �Q�T�U�j�g���I��
SIO_IN_SCK_2M           =   0x0002      @                  �Q�l�g���I��
SIO_ACK_RECV            =   0x0004      @ �]���v��
SIO_ACK_SEND            =   0x0008      @ �]������
SIO_9600_BPS            =   0x0000      @ �{�[���[�g �X�U�O�O�a�o�r
SIO_38400_BPS           =   0x0001      @          �R�W�S�O�O�a�o�r
SIO_57600_BPS           =   0x0002      @          �T�V�U�O�O�a�o�r
SIO_115200_BPS          =   0x0003      @        �P�P�T�Q�O�O�a�o�r
SIO_MULTI_CONNECT       =   0x0004      @ �}���`�v���C�ʐM �ڑ���
SIO_MULTI_DISCONNECT    =   0x0000      @                  ��ڑ�
SIO_MULTI_PARENT        =   0x0008      @ �}���`�v���C�ʐM �e�ڑ�
SIO_MULTI_CHILD         =   0x0000      @                  �q�ڑ�
SIO_MULTI_SI            =   0x0004      @ �}���`�v���C�ʐM �r�h�[�q
SIO_MULTI_SD            =   0x0008      @                  �r�c�[�q
SIO_MULTI_BUSY          =   0x0080      @ �}���`�v���C�ʐM��
SIO_CTS_ENABLE          =   0x0004      @ �t�`�q�s���M�\�M��  �C�l�[�u��
SIO_UART_7BIT           =   0x0000      @ �t�`�q�s�ʐM �f�[�^�� �V������
SIO_UART_8BIT           =   0x0080      @                       �W������
SIO_ERROR               =   0x0040      @ �G���[���o
SIO_START               =   0x0080      @ �]���X�^�[�g
SIO_ENABLE              =   0x0080      @ �r�h�n����
SIO_FIFO_ENABLE         =   0x0100      @ �e�h�e�n �C�l�[�u��
SIO_PARITY_ENABLE       =   0x0200      @ �p���e�B �C�l�[�u��
SIO_PARITY_EVEN         =   0x0000      @ �����p���e�B
SIO_PARITY_ODD          =   0x0008      @ ��p���e�B
SIO_TRANS_ENABLE        =   0x0400      @ �g�����X�~�b�^ �C�l�[�u��
SIO_TRANS_DATA_FULL     =   0x0010      @ �g�����X�~�b�g�f�[�^ ���t
SIO_RECV_ENABLE         =   0x0800      @ ���V�[�o �C�l�[�u��
SIO_RECV_DATA_EMPTY     =   0x0020      @ ���V�[�u�f�[�^ ����
SIO_IF_ENABLE           =   0x4000      @ ���荞�ݗv�� ����

@--------------------------------------------------------------------
@-                      �i�n�x�o�X �R���g���[��                     -
@--------------------------------------------------------------------

JOY_IF_RESET            =   0x01        @ �i�n�x�o�X�E���Z�b�g ���荞�ݗv��
JOY_IF_RECV             =   0x02        @ �i�n�x�o�X��M����   ���荞�ݗv��
JOY_IF_SEND             =   0x04        @ �i�n�x�o�X���M����   ���荞�ݗv��
JOY_IF_ENABLE           =   0x40        @ ���荞�ݗv�� ����

@--------------------------------------------------------------------
@-                      �i�n�x�o�X �X�e�[�^�X                       -
@--------------------------------------------------------------------

JSTAT_FLAGS_MASK        =   0x30        @ �ėp�t���O

JSTAT_FLAGS_SHIFT       =   4

JSTAT_PSF0              =   0x10        @ �ėp�t���O�O
JSTAT_PSF1              =   0x20        @           �P

JSTAT_RECV              =   0x02        @ ��M�X�e�[�^�X
JSTAT_SEND              =   0x08        @ ���M�X�e�[�^�X

@--------------------------------------------------------------------
@-                      �ėp���o�̓R���g���[��                      -
@--------------------------------------------------------------------

R_SIO_MODE_MASTER_MASK  =   0xc000      @ �r�h�n���[�h�E�}�X�^

R_SIO_MODE_MASTER_SHIFT =   14

R_SIO_MASTER_MODE       =   0x0000      @ �r�h�n�}�X�^�E���[�h
R_DIRECT_MODE           =   0x8000      @ �ėp���o�͒ʐM���[�h
R_JOY_MODE              =   0xc000      @ �i�n�x�ʐM���[�h

R_SC                    =   0x0001      @ �f�[�^
R_SD                    =   0x0002
R_SI                    =   0x0004
R_SO                    =   0x0008
R_SC_I_O                =   0x0010      @ �h/�n�I��
R_SD_I_O                =   0x0020
R_SI_I_O                =   0x0040
R_SO_I_O                =   0x0080
R_SC_IN                 =   0x0000      @ ���͐ݒ�
R_SD_IN                 =   0x0000
R_SI_IN                 =   0x0000
R_SO_IN                 =   0x0000
R_SC_OUT                =   0x0010      @ �o�͐ݒ�
R_SD_OUT                =   0x0020
R_SI_OUT                =   0x0040
R_SO_OUT                =   0x0080
R_IF_ENABLE             =   0x0100      @ ���荞�ݗv�� ����


@--------------------------------------------------------------------
@-                      �c�l�`�R���g���[��                          -
@--------------------------------------------------------------------

DMA_TIMMING_MASK        =   0x30000000  @ �J�n�^�C�~���O
DMA_COUNT_MASK          =   0x0000ffff  @ �]����

DMA_TIMMING_SHIFT       =   30
DMA_COUNT_SHIFT         =   0

DMA_ENABLE              =   0x80000000  @ �c�l�`����
DMA_IF_ENABLE           =   0x40000000  @ ���荞�ݗv�� ����
DMA_TIMMING_IMM         =   0x00000000  @ �����ɋN��
DMA_TIMMING_V_BLANK     =   0x10000000  @ �u�u�����N�N��
DMA_TIMMING_H_BLANK     =   0x20000000  @ �g�u�����N�N��
DMA_TIMMING_DISP        =   0x30000000  @ �\���N��
DMA_TIMMING_SOUND       =   0x30000000  @ �T�E���h�e�h�e�n���N�G�X�g�N��
DMA_DREQ_ON             =   0x08000000  @ �f�[�^�v���������[�h �n�m
DMA_16BIT_BUS           =   0x00000000  @ �o�X�T�C�Y�P�U�a�����I��
DMA_32BIT_BUS           =   0x04000000  @ �o�X�T�C�Y�R�Q�a�����I��
DMA_CONTINUOUS_ON       =   0x02000000  @ �R���e�B�j���A�X���[�h �n�m
DMA_SRC_INC             =   0x00000000  @ �]���� �C���N�������g �I��
DMA_SRC_DEC             =   0x00800000  @ �]���� �f�N�������g   �I��
DMA_SRC_FIX             =   0x01000000  @ �]���� �Œ� �I��
DMA_DEST_INC            =   0x00000000  @ �]���� �C���N�������g �I��
DMA_DEST_DEC            =   0x00200000  @ �]���� �f�N�������g   �I��
DMA_DEST_FIX            =   0x00400000  @ �]���� �Œ� �I��
DMA_DEST_RELOAD         =   0x00600000  @ �]���� �C���N�������g/�����[�h �I��

@--------------------------------------------------------------------
@-                      �^�C�}�[ �R���g���[��                       -
@--------------------------------------------------------------------

TMR_PRESCALER_MASK      =   0x00030000  @ �v���X�P�[�� �N���b�N

TMR_PRESCALER_SHIFT     =   16

TMR_PRESCALER_1CK       =   0x00000000  @ �v���X�P�[�� �P����
TMR_PRESCALER_64CK      =   0x00010000  @           �U�S����
TMR_PRESCALER_256CK     =   0x00020000  @         �Q�T�U����
TMR_PRESCALER_1024CK    =   0x00030000  @       �P�O�Q�S����
TMR_IF_ENABLE           =   0x00400000  @ ���荞�ݗv�� ����
TMR_ENABLE              =   0x00800000  @ �^�C�}�[�N��


@--------------------------------------------------------------------
@-                          �L�[                                    -
@--------------------------------------------------------------------

BUTTON_MASK             =   0x030f      @ �{�^��
PLUS_KEY_MASK           =   0x00f0      @ �\���L�[
ALL_KEY_MASK            =   0x03ff      @ �S�ẴL�[

A_BUTTON                =   0x0001      @ �`�{�^��
B_BUTTON                =   0x0002      @ �a�{�^��
SELECT_BUTTON           =   0x0004      @ �Z���N�g�{�^��
START_BUTTON            =   0x0008      @ �X�^�[�g�{�^��
R_KEY                   =   0x0010      @ �E�{�^��
L_KEY                   =   0x0020      @ ���{�^��
U_KEY                   =   0x0040      @ ��{�^��
D_KEY                   =   0x0080      @ ���{�^��
R_BUTTON                =   0x0100      @ �q�{�^��
L_BUTTON                =   0x0200      @ �k�{�^��
KEY_IF_ENABLE           =   0x4000      @ ���荞�ݗv�� ����
KEY_OR_INTR             =   0x0000      @   �ʏ�L�[���荞��
KEY_AND_INTR            =   0x8000      @ �`�m�c�L�[���荞��


@--------------------------------------------------------------------
@-                  �J�Z�b�g�E�F�C�g �R���g���[��                   -
@--------------------------------------------------------------------

CST_SRAM_4WAIT          =   0x0000      @ �r�q�`�l �S�E�F�C�g
CST_SRAM_3WAIT          =   0x0001      @          �R�E�F�C�g
CST_SRAM_2WAIT          =   0x0002      @          �Q�E�F�C�g
CST_SRAM_8WAIT          =   0x0003      @          �W�E�F�C�g
CST_ROM0_1ST_4WAIT      =   0x0000      @ �q�n�l�O �P���� �S�E�F�C�g
CST_ROM0_1ST_3WAIT      =   0x0004      @                    �R�E�F�C�g
CST_ROM0_1ST_2WAIT      =   0x0008      @                    �Q�E�F�C�g
CST_ROM0_1ST_8WAIT      =   0x000c      @                    �W�E�F�C�g
CST_ROM0_2ND_2WAIT      =   0x0000      @             �Q���� �Q�E�F�C�g
CST_ROM0_2ND_1WAIT      =   0x0010      @                    �P�E�F�C�g
CST_ROM1_1ST_4WAIT      =   0x0000      @ �q�n�l�P �P���� �S�E�F�C�g
CST_ROM1_1ST_3WAIT      =   0x0020      @                    �R�E�F�C�g
CST_ROM1_1ST_2WAIT      =   0x0040      @                    �Q�E�F�C�g
CST_ROM1_1ST_8WAIT      =   0x0060      @                    �W�E�F�C�g
CST_ROM1_2ND_4WAIT      =   0x0000      @             �Q���� �S�E�F�C�g
CST_ROM1_2ND_1WAIT      =   0x0080      @                    �P�E�F�C�g
CST_ROM2_1ST_4WAIT      =   0x0000      @ �q�n�l�Q �P���� �S�E�F�C�g
CST_ROM2_1ST_3WAIT      =   0x0100      @                    �R�E�F�C�g
CST_ROM2_1ST_2WAIT      =   0x0200      @                    �Q�E�F�C�g
CST_ROM2_1ST_8WAIT      =   0x0300      @                    �W�E�F�C�g
CST_ROM2_2ND_8WAIT      =   0x0000      @             �Q���� �W�E�F�C�g
CST_ROM2_2ND_1WAIT      =   0x0400      @                 �P�E�F�C�g

CST_PHI_OUT_NONE        =   0x0000      @ �Ӓ[�q�o�̓N���b�N �k���Œ�
CST_PHI_OUT_4MCK        =   0x0800      @                �S�l�g��
CST_PHI_OUT_8MCK        =   0x1000      @                �W�l�g��
CST_PHI_OUT_16MCK       =   0x1800      @              �P�U�l�g��
CST_PREFETCH_ENABLE     =   0x4000      @ �v���t�F�b�`�o�b�t�@ ����
CST_AGB                 =   0x0000      @ �`�f�a�J�Z�b�g
CST_CGB                 =   0x8000      @ �b�f�a�J�Z�b�g


