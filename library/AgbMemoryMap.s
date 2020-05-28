@********************************************************************
@*          AgbMemoryMap.s                                          *
@*            AGB�������}�b�v�萔�iGAS�p�j                          *
@*                                                                  *
@*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               *
@********************************************************************


@--------------------------------------------------------------------
@-                      �������E�}�b�v                              -
@--------------------------------------------------------------------

BOOT_ROM                =   0x00000000          @ �u�[�g�q�n�l
BOOT_ROM_END            =   (BOOT_ROM + 0x4000)
EX_WRAM                 =   0x02000000          @ �b�o�t�O�����[�N�q�`�l
EX_WRAM_END             =   (EX_WRAM + 0x40000)
CPU_WRAM                =   0x03000000          @ �b�o�t�������[�N�q�`�l
CPU_WRAM_END            =   (CPU_WRAM + 0x8000)
WRAM                    =    EX_WRAM            @ ���[�N�q�`�l�S��
WRAM_END                =    CPU_WRAM_END
SOUND_AREA_ADDR_BUF     =   (CPU_WRAM_END - 0x10)@�T�E���h�h���C�o ���[�N�A�h���X
INTR_CHECK_BUF          =   (CPU_WRAM_END - 0x8)@ ���荞�݃`�F�b�N
SOFT_RESET_DIRECT_BUF   =   (CPU_WRAM_END - 0x6)@ SoftReset() ���A��w��
INTR_VECTOR_BUF         =   (CPU_WRAM_END - 0x4)@ ���荞�ݕ���A�h���X
REG_BASE                =   0x04000000          @ ���W�X�^�Q
REG_END                 =   (REG_BASE +  0x300)
PLTT                    =   0x05000000          @ �p���b�g�q�`�l
PLTT_END                =   (PLTT +      0x400)
BG_PLTT                 =   (PLTT +        0x0) @   �a�f�p���b�g�q�`�l
BG_PLTT_END             =   (PLTT +      0x200)
OBJ_PLTT                =   (PLTT +      0x200) @ �n�a�i�p���b�g�q�`�l
OBJ_PLTT_END            =   (PLTT +      0x400)
VRAM                    =   0x06000000          @ �u�q�`�l
VRAM_END                =   (VRAM +    0x18000)
BG_VRAM                 =   (VRAM +        0x0) @ �a�f�L�����N�^/�X�N���[���q�`�l
BG_BITMAP0_VRAM         =   (VRAM +        0x0) @ �a�f�r�b�g�}�b�v�O�q�`�l
BG_BITMAP1_VRAM         =   (VRAM +     0xa000) @ �a�f�r�b�g�}�b�v�P�q�`�l
OBJ_MODE0_VRAM          =   (VRAM +    0x10000) @ �n�a�i�L�����N�^�q�`�l
OBJ_MODE1_VRAM          =   (VRAM +    0x10000)
OBJ_MODE2_VRAM          =   (VRAM +    0x10000)
OBJ_MODE3_VRAM          =   (VRAM +    0x14000)
OBJ_MODE4_VRAM          =   (VRAM +    0x14000)
OBJ_MODE5_VRAM          =   (VRAM +    0x14000)
OBJ_VRAM_END            =   (VRAM +    0x18000)
OAM                     =   0x07000000          @ �n�`�l
OAM_END                 =   (OAM +       0x400)

ROM_BANK0               =   0x08000000          @ �q�n�l �o���N�O
ROM_BANK0_END           =   0x0a000000
ROM_BANK1               =   0x0a000000          @ �q�n�l �o���N�P
ROM_BANK1_END           =   0x0c000000
ROM_BANK2               =   0x0c000000          @ �q�n�l �o���N�Q
ROM_BANK2_END           =   0x0e000000
RAM_BANK                =   0x0c000000          @ �q�`�l �o���N
RAM_BANK_END            =   0x10000000

FLASH_1M                =   0x09fe0000          @ �P�l�t���b�V��������
FLASH_1M_END            =   0x0a000000

@--------------------------------------------------------------------
@-                      �������E�T�C�Y                              -
@--------------------------------------------------------------------

BOOT_ROM_SIZE           =   0x4000              @ �u�[�g�q�n�l
EX_WRAM_SIZE            =   0x40000             @ �b�o�t�O�����[�N�q�`�l
CPU_WRAM_SIZE           =   0x8000              @ �b�o�t�������[�N�q�`�l
WRAM_SIZE               =   0x48000             @ ���[�N�q�`�l�S��
USR_BUF_SIZE            =   0x47fa0             @ ���[�U�̈�
SYS_BUF_SIZE            =   0x80                @ �V�X�e���̈�
PLTT_SIZE               =   (2*256*2)           @ �p���b�g�q�`�l
BG_PLTT_SIZE            =   (2*256)             @   �a�f�p���b�g�q�`�l
OBJ_PLTT_SIZE           =   (2*256)             @ �n�a�i�p���b�g�q�`�l
VRAM_SIZE               =   0x18000             @ �u�q�`�l
BG_MODE0_VRAM_SIZE      =   0x10000             @ �a�f�L�����N�^/�X�N���[��
BG_MODE1_VRAM_SIZE      =   0x10000
BG_MODE2_VRAM_SIZE      =   0x10000
BG_MODE3_VRAM_SIZE      =   0x14000             @ �a�f�r�b�g�}�b�v
BG_MODE4_VRAM_SIZE      =   0x0a000
BG_MODE5_VRAM_SIZE      =   0x0a000
OBJ_MODE0_VRAM_SIZE     =   0x08000             @ �n�a�i�L�����N�^
OBJ_MODE1_VRAM_SIZE     =   0x08000
OBJ_MODE2_VRAM_SIZE     =   0x08000
OBJ_MODE3_VRAM_SIZE     =   0x04000
OBJ_MODE4_VRAM_SIZE     =   0x04000
OBJ_MODE5_VRAM_SIZE     =   0x04000
OAM_SIZE                =   (8*128)             @ �n�`�l

ROM_BANK_SIZE           =   0x02000000          @ �q�n�l �o���N
ROM_BANK0_SIZE          =   0x02000000          @ �q�n�l �o���N�O
ROM_BANK1_SIZE          =   0x02000000          @ �q�n�l �o���N�P
ROM_BANK2_SIZE          =   0x02000000          @ �q�n�l �o���N�Q
RAM_BANK_SIZE           =   0x02000000          @ �q�`�l �o���N

FLASH_1M_SIZE           =   0x20000             @ �P�l�t���b�V��������

@--------------------------------------------------------------------
@-                      ���W�X�^�E�A�h���X                          -
@--------------------------------------------------------------------

REG_IME         =   (REG_BASE + 0x208)  @ ���荞�݃}�X�^�C�l�[�u��
REG_IE          =   (REG_BASE + 0x200)  @ ���荞�݋���
REG_IF          =   (REG_BASE + 0x202)  @ ���荞�ݗv��

REG_WAITCNT     =   (REG_BASE + 0x204)  @ �J�Z�b�g�E�F�C�g�R���g���[��

REG_DISPCNT     =   (REG_BASE + 0x0)    @ �\���R���g���[��

REG_STAT        =   (REG_BASE + 0x4)    @ �X�e�[�^�X
REG_VCOUNT      =   (REG_BASE + 0x6)    @ �u�J�E���^

REG_MOSAIC      =   (REG_BASE + 0x4c)   @ ���U�C�N �T�C�Y

REG_BLDCNT      =   (REG_BASE + 0x50)   @ �u�����h���[�h �R���g���[��
REG_BLDALPHA    =   (REG_BASE + 0x52)   @   ������ �p�����[�^
REG_BLDY        =   (REG_BASE + 0x54)   @ �P�x�ύX �p�����[�^

REG_WINCNT      =   (REG_BASE + 0x40)   @ �E�C���h�E  �R���g���[��
REG_WIN0H       =   (REG_BASE + 0x40)   @ �E�C���h�E�O�����̈�
REG_WIN1H       =   (REG_BASE + 0x42)   @ �E�C���h�E�P�����̈�
REG_WIN0V       =   (REG_BASE + 0x44)   @ �E�C���h�E�O�����̈�
REG_WIN1V       =   (REG_BASE + 0x46)   @ �E�C���h�E�P�����̈�
REG_WININ       =   (REG_BASE + 0x48)   @ �E�C���h�E���R���g���[��
REG_WIN0        =   (REG_BASE + 0x48)   @ �E�C���h�E�O�R���g���[��
REG_WIN1        =   (REG_BASE + 0x49)   @ �E�C���h�E�P�R���g���[��
REG_WINOUT      =   (REG_BASE + 0x4a)   @ �E�C���h�E�O�R���g���[��
REG_OBJWIN      =   (REG_BASE + 0x4b)   @ �n�a�i�E�C���h�E�R���g���[��


REG_BGCNT       =   (REG_BASE + 0x8)    @ �a�f�R���g���[��
REG_BG0CNT      =   (REG_BASE + 0x8)    @ �a�f�O�R���g���[��
REG_BG1CNT      =   (REG_BASE + 0xa)    @ �a�f�P�R���g���[��
REG_BG2CNT      =   (REG_BASE + 0xc)    @ �a�f�Q�R���g���[��
REG_BG3CNT      =   (REG_BASE + 0xe)    @ �a�f�R�R���g���[��
REG_BGOFS       =   (REG_BASE + 0x10)   @ �a�f�I�t�Z�b�g
REG_BG0HOFS     =   (REG_BASE + 0x10)   @ �a�f�O�g�I�t�Z�b�g
REG_BG0VOFS     =   (REG_BASE + 0x12)   @ �a�f�O�u�I�t�Z�b�g
REG_BG1HOFS     =   (REG_BASE + 0x14)   @ �a�f�P�g�I�t�Z�b�g
REG_BG1VOFS     =   (REG_BASE + 0x16)   @ �a�f�P�u�I�t�Z�b�g
REG_BG2HOFS     =   (REG_BASE + 0x18)   @ �a�f�Q�g�I�t�Z�b�g
REG_BG2VOFS     =   (REG_BASE + 0x1a)   @ �a�f�Q�u�I�t�Z�b�g
REG_BG3HOFS     =   (REG_BASE + 0x1c)   @ �a�f�R�g�I�t�Z�b�g
REG_BG3VOFS     =   (REG_BASE + 0x1e)   @ �a�f�R�u�I�t�Z�b�g
REG_BG2AFFINE   =   (REG_BASE + 0x20)   @ �a�f�Q�A�t�B���ϊ��p�����[�^�Q
REG_BG2PA       =   (REG_BASE + 0x20)   @ �a�f�Q���C�������w���W����
REG_BG2PB       =   (REG_BASE + 0x22)   @ �a�f�Q  ���������w���W����
REG_BG2PC       =   (REG_BASE + 0x24)   @ �a�f�Q���C�������x���W����
REG_BG2PD       =   (REG_BASE + 0x26)   @ �a�f�Q  ���������x���W����
REG_BG2X        =   (REG_BASE + 0x28)   @ �a�f�Q�X�^�[�g�w���W
REG_BG2X_L      =   (REG_BASE + 0x28)
REG_BG2X_H      =   (REG_BASE + 0x2a)
REG_BG2Y        =   (REG_BASE + 0x2c)   @ �a�f�Q�X�^�[�g�x���W
REG_BG2Y_L      =   (REG_BASE + 0x2c)
REG_BG2Y_H      =   (REG_BASE + 0x2e)
REG_BG3AFFINE   =   (REG_BASE + 0x30)   @ �a�f�R�A�t�B���ϊ��p�����[�^�Q
REG_BG3PA       =   (REG_BASE + 0x30)   @ �a�f�R���C�������w���W����
REG_BG3PB       =   (REG_BASE + 0x32)   @ �a�f�R  ���������w���W����
REG_BG3PC       =   (REG_BASE + 0x34)   @ �a�f�R���C�������x���W����
REG_BG3PD       =   (REG_BASE + 0x36)   @ �a�f�R  ���������x���W����
REG_BG3X        =   (REG_BASE + 0x38)   @ �a�f�R�X�^�[�g�w���W
REG_BG3X_L      =   (REG_BASE + 0x38)
REG_BG3X_H      =   (REG_BASE + 0x3a)
REG_BG3Y        =   (REG_BASE + 0x3c)   @ �a�f�R�X�^�[�g�x���W
REG_BG3Y_L      =   (REG_BASE + 0x3c)
REG_BG3Y_H      =   (REG_BASE + 0x3e)


REG_SOUNDCNT    =   (REG_BASE + 0x80)   @ �T�E���h�R���g���[��
REG_SOUNDCNT_L  =   (REG_BASE + 0x80)
REG_SOUNDCNT_H  =   (REG_BASE + 0x82)
REG_SOUNDCNT_X  =   (REG_BASE + 0x84)

REG_SOUNDBIAS   =   (REG_BASE + 0x88)   @ �T�E���h�a�h�`�r

REG_SOUND1CNT   =   (REG_BASE + 0x60)   @ �T�E���h�P�R���g���[��
REG_SOUND1CNT_L =   (REG_BASE + 0x60)
REG_SOUND1CNT_H =   (REG_BASE + 0x62)
REG_SOUND1CNT_X =   (REG_BASE + 0x64)
REG_SOUND2CNT   =   (REG_BASE + 0x68)   @ �T�E���h�Q�R���g���[��
REG_SOUND2CNT_L =   (REG_BASE + 0x68)
REG_SOUND2CNT_H =   (REG_BASE + 0x6c)
REG_SOUND3CNT   =   (REG_BASE + 0x70)   @ �T�E���h�R�R���g���[��
REG_SOUND3CNT_L =   (REG_BASE + 0x70)
REG_SOUND3CNT_H =   (REG_BASE + 0x72)
REG_SOUND3CNT_X =   (REG_BASE + 0x74)
REG_SOUND4CNT   =   (REG_BASE + 0x78)   @ �T�E���h�S�R���g���[��
REG_SOUND4CNT_L =   (REG_BASE + 0x78)
REG_SOUND4CNT_H =   (REG_BASE + 0x7c)

REG_WAVE_RAM    =   (REG_BASE + 0x90)   @ �T�E���h�R �g�`�q�`�l
REG_WAVE_RAM0   =   (REG_BASE + 0x90)
REG_WAVE_RAM0_L =   (REG_BASE + 0x90)
REG_WAVE_RAM0_H =   (REG_BASE + 0x92)
REG_WAVE_RAM1   =   (REG_BASE + 0x94)
REG_WAVE_RAM1_L =   (REG_BASE + 0x94)
REG_WAVE_RAM1_H =   (REG_BASE + 0x96)
REG_WAVE_RAM2   =   (REG_BASE + 0x98)
REG_WAVE_RAM2_L =   (REG_BASE + 0x98)
REG_WAVE_RAM2_H =   (REG_BASE + 0x9a)
REG_WAVE_RAM3   =   (REG_BASE + 0x9c)
REG_WAVE_RAM3_L =   (REG_BASE + 0x9c)
REG_WAVE_RAM3_H =   (REG_BASE + 0x9e)

REG_FIFO        =   (REG_BASE + 0xa0)   @ �T�E���h �e�h�e�n
REG_FIFO_A      =   (REG_BASE + 0xa0)
REG_FIFO_A_L    =   (REG_BASE + 0xa0)
REG_FIFO_A_H    =   (REG_BASE + 0xa2)
REG_FIFO_B      =   (REG_BASE + 0xa4)
REG_FIFO_B_L    =   (REG_BASE + 0xa4)
REG_FIFO_B_H    =   (REG_BASE + 0xa6)


REG_DMA0        =   (REG_BASE + 0xb0)   @ �c�l�`�O
REG_DMA0SAD     =   (REG_BASE + 0xb0)   @ �i�����������������������j
REG_DMA0SAD_L   =   (REG_BASE + 0xb0)
REG_DMA0SAD_H   =   (REG_BASE + 0xb2)
REG_DMA0DAD     =   (REG_BASE + 0xb4)
REG_DMA0DAD_L   =   (REG_BASE + 0xb4)
REG_DMA0DAD_H   =   (REG_BASE + 0xb6)
REG_DMA0CNT     =   (REG_BASE + 0xb8)
REG_DMA0CNT_L   =   (REG_BASE + 0xb8)
REG_DMA0CNT_H   =   (REG_BASE + 0xba)
REG_DMA1        =   (REG_BASE + 0xbc)   @ �c�l�`�P
REG_DMA1SAD     =   (REG_BASE + 0xbc)   @ �i�������������T�E���hFIFO�^�����������j
REG_DMA1SAD_L   =   (REG_BASE + 0xbc)
REG_DMA1SAD_H   =   (REG_BASE + 0xbe)
REG_DMA1DAD     =   (REG_BASE + 0xc0)
REG_DMA1DAD_L   =   (REG_BASE + 0xc0)
REG_DMA1DAD_H   =   (REG_BASE + 0xc2)
REG_DMA1CNT     =   (REG_BASE + 0xc4)
REG_DMA1CNT_L   =   (REG_BASE + 0xc4)
REG_DMA1CNT_H   =   (REG_BASE + 0xc6)
REG_DMA2        =   (REG_BASE + 0xc8)   @ �c�l�`�Q
REG_DMA2SAD     =   (REG_BASE + 0xc8)   @ �i�������������T�E���hFIFO�^�����������j
REG_DMA2SAD_L   =   (REG_BASE + 0xc8)
REG_DMA2SAD_H   =   (REG_BASE + 0xca)
REG_DMA2DAD     =   (REG_BASE + 0xcc)
REG_DMA2DAD_L   =   (REG_BASE + 0xcc)
REG_DMA2DAD_H   =   (REG_BASE + 0xce)
REG_DMA2CNT     =   (REG_BASE + 0xd0)
REG_DMA2CNT_L   =   (REG_BASE + 0xd0)
REG_DMA2CNT_H   =   (REG_BASE + 0xd2)
REG_DMA3        =   (REG_BASE + 0xd4)   @ �c�l�`�R
REG_DMA3SAD     =   (REG_BASE + 0xd4)   @ �i�J�Z�b�g�^�������������������������j
REG_DMA3SAD_L   =   (REG_BASE + 0xd4)
REG_DMA3SAD_H   =   (REG_BASE + 0xd6)
REG_DMA3DAD     =   (REG_BASE + 0xd8)
REG_DMA3DAD_L   =   (REG_BASE + 0xd8)
REG_DMA3DAD_H   =   (REG_BASE + 0xda)
REG_DMA3CNT     =   (REG_BASE + 0xdc)
REG_DMA3CNT_L   =   (REG_BASE + 0xdc)
REG_DMA3CNT_H   =   (REG_BASE + 0xde)


REG_TM0CNT      =   (REG_BASE + 0x100)  @ �^�C�}�[�O
REG_TM0CNT_L    =   (REG_BASE + 0x100)
REG_TM0CNT_H    =   (REG_BASE + 0x102)
REG_TM1CNT      =   (REG_BASE + 0x104)  @ �^�C�}�[�P
REG_TM1CNT_L    =   (REG_BASE + 0x104)
REG_TM1CNT_H    =   (REG_BASE + 0x106)
REG_TM2CNT      =   (REG_BASE + 0x108)  @ �^�C�}�[�Q
REG_TM2CNT_L    =   (REG_BASE + 0x108)
REG_TM2CNT_H    =   (REG_BASE + 0x10a)
REG_TM3CNT      =   (REG_BASE + 0x10c)  @ �^�C�}�[�R
REG_TM3CNT_L    =   (REG_BASE + 0x10c)
REG_TM3CNT_H    =   (REG_BASE + 0x10e)


REG_RCNT		=	(REG_BASE + 0x134)	@ �ėp���o�̓R���g���[��

REG_SIOCNT      =   (REG_BASE + 0x128)  @ �V���A���ʐM�R���g���[��
REG_SIODATA8    =	(REG_BASE + 0x12a)  @  8bit�V���A���ʐM�f�[�^
REG_SIODATA32   =	(REG_BASE + 0x120)  @ 32bit�V���A���ʐM�f�[�^
REG_SIOMLT_SEND =   (REG_BASE + 0x12a)  @ �}���`�v���C�r�h�n ���M�f�[�^
REG_SIOMLT_RECV =	(REG_BASE + 0x120)  @ �}���`�v���C�r�h�n ��M�f�[�^
REG_SIOMULTI0   =   (REG_BASE + 0x120)
REG_SIOMULTI1   =   (REG_BASE + 0x122)
REG_SIOMULTI2   =   (REG_BASE + 0x124)
REG_SIOMULTI3   =   (REG_BASE + 0x126)

REG_JOYCNT      =   (REG_BASE + 0x140)  @ �i�n�x�o�X�ʐM�R���g���[��
REG_JOYSTAT     =   (REG_BASE + 0x158)  @ �i�n�x�o�X�@�X�e�[�^�X
REG_JOY_RECV    =   (REG_BASE + 0x150)  @ �i�n�x�o�X�@��M�f�[�^
REG_JOY_RECV_L  =   (REG_BASE + 0x150)
REG_JOY_RECV_H  =   (REG_BASE + 0x152)
REG_JOY_TRANS   =   (REG_BASE + 0x154)  @ �i�n�x�o�X�@���M�f�[�^
REG_JOY_TRANS_L =   (REG_BASE + 0x154)
REG_JOY_TRANS_H =   (REG_BASE + 0x156)


REG_KEYINPUT    =   (REG_BASE + 0x130)  @ �L�[����
REG_KEYCNT      =   (REG_BASE + 0x132)  @ �L�[�R���g���[��


@--------------------------------------------------------------------
@-                      ���W�X�^�E�I�t�Z�b�g                        -
@--------------------------------------------------------------------

OFFSET_REG_IME          =   0x208       @ ���荞�݃}�X�^�C�l�[�u��
OFFSET_REG_IE           =   0x200       @ ���荞�݋���
OFFSET_REG_IF           =   0x202       @ ���荞�ݗv��

OFFSET_REG_WAITCNT      =   0x204       @ �J�Z�b�g�E�F�C�g�R���g���[��

OFFSET_REG_DISPCNT      =   0x0         @ �\���R���g���[��

OFFSET_REG_STAT         =   0x4         @ �X�e�[�^�X
OFFSET_REG_VCOUNT       =   0x6         @ �u�J�E���^

OFFSET_REG_MOSAIC       =   0x4c        @ ���U�C�N �T�C�Y

OFFSET_REG_BLDCNT       =   0x50        @ �u�����h���[�h �R���g���[��
OFFSET_REG_BLDALPHA     =   0x52        @   ������ �p�����[�^
OFFSET_REG_BLDY         =   0x54        @ �P�x�ύX �p�����[�^

OFFSET_REG_WINCNT       =   0x40        @ �E�C���h�E  �R���g���[��
OFFSET_REG_WIN0H        =   0x40        @ �E�C���h�E�O�����̈�
OFFSET_REG_WIN1H        =   0x42        @ �E�C���h�E�P�����̈�
OFFSET_REG_WIN0V        =   0x44        @ �E�C���h�E�O�����̈�
OFFSET_REG_WIN1V        =   0x46        @ �E�C���h�E�P�����̈�
OFFSET_REG_WININ        =   0x48        @ �E�C���h�E���R���g���[��
OFFSET_REG_WIN0         =   0x48        @ �E�C���h�E�O�R���g���[��
OFFSET_REG_WIN1         =   0x49        @ �E�C���h�E�P�R���g���[��
OFFSET_REG_WINOUT       =   0x4a        @ �E�C���h�E�O�R���g���[��
OFFSET_REG_OBJWIN       =   0x4b        @ �n�a�i�E�C���h�E�R���g���[��


OFFSET_REG_BGCNT        =   0x8         @ �a�f�R���g���[��
OFFSET_REG_BG0CNT       =   0x8         @ �a�f�O�R���g���[��
OFFSET_REG_BG1CNT       =   0xa         @ �a�f�P�R���g���[��
OFFSET_REG_BG2CNT       =   0xc         @ �a�f�Q�R���g���[��
OFFSET_REG_BG3CNT       =   0xe         @ �a�f�R�R���g���[��
OFFSET_REG_BGOFS        =   0x10        @ �a�f�I�t�Z�b�g
OFFSET_REG_BG0HOFS      =   0x10        @ �a�f�O�g�I�t�Z�b�g
OFFSET_REG_BG0VOFS      =   0x12        @ �a�f�O�u�I�t�Z�b�g
OFFSET_REG_BG1HOFS      =   0x14        @ �a�f�P�g�I�t�Z�b�g
OFFSET_REG_BG1VOFS      =   0x16        @ �a�f�P�u�I�t�Z�b�g
OFFSET_REG_BG2HOFS      =   0x18        @ �a�f�Q�g�I�t�Z�b�g
OFFSET_REG_BG2VOFS      =   0x1a        @ �a�f�Q�u�I�t�Z�b�g
OFFSET_REG_BG3HOFS      =   0x1c        @ �a�f�R�g�I�t�Z�b�g
OFFSET_REG_BG3VOFS      =   0x1e        @ �a�f�R�u�I�t�Z�b�g
OFFSET_REG_BG2AFFINE    =   0x20        @ �a�f�Q�A�t�B���ϊ��p�����[�^�Q
OFFSET_REG_BG2PA        =   0x20        @ �a�f�Q���C�������w���W����
OFFSET_REG_BG2PB        =   0x22        @ �a�f�Q  ���������w���W����
OFFSET_REG_BG2PC        =   0x24        @ �a�f�Q���C�������x���W����
OFFSET_REG_BG2PD        =   0x26        @ �a�f�Q  ���������x���W����
OFFSET_REG_BG2X         =   0x28        @ �a�f�Q�X�^�[�g�w���W
OFFSET_REG_BG2X_L       =   0x28
OFFSET_REG_BG2X_H       =   0x2a
OFFSET_REG_BG2Y         =   0x2c        @ �a�f�Q�X�^�[�g�x���W
OFFSET_REG_BG2Y_L       =   0x2c
OFFSET_REG_BG2Y_H       =   0x2e
OFFSET_REG_BG3AFFINE    =   0x30        @ �a�f�R�A�t�B���ϊ��p�����[�^�Q
OFFSET_REG_BG3PA        =   0x30        @ �a�f�R���C�������w���W����
OFFSET_REG_BG3PB        =   0x32        @ �a�f�R  ���������w���W����
OFFSET_REG_BG3PC        =   0x34        @ �a�f�R���C�������x���W����
OFFSET_REG_BG3PD        =   0x36        @ �a�f�R  ���������x���W����
OFFSET_REG_BG3X         =   0x38        @ �a�f�R�X�^�[�g�w���W
OFFSET_REG_BG3X_L       =   0x38
OFFSET_REG_BG3X_H       =   0x3a
OFFSET_REG_BG3Y         =   0x3c        @ �a�f�R�X�^�[�g�x���W
OFFSET_REG_BG3Y_L       =   0x3c
OFFSET_REG_BG3Y_H       =   0x3e


OFFSET_REG_SOUNDCNT     =   0x80        @ �T�E���h�R���g���[��
OFFSET_REG_SOUNDCNT_L   =   0x80
OFFSET_REG_SOUNDCNT_H   =   0x82
OFFSET_REG_SOUNDCNT_X   =   0x84

OFFSET_REG_SOUNDBIAS    =   0x88        @ �T�E���h�a�h�`�r

OFFSET_REG_SOUND1CNT    =   0x60        @ �T�E���h�P�R���g���[��
OFFSET_REG_SOUND1CNT_L  =   0x60
OFFSET_REG_SOUND1CNT_H  =   0x62
OFFSET_REG_SOUND1CNT_X  =   0x64
OFFSET_REG_SOUND2CNT    =   0x68        @ �T�E���h�Q�R���g���[��
OFFSET_REG_SOUND2CNT_L  =   0x68
OFFSET_REG_SOUND2CNT_H  =   0x6c
OFFSET_REG_SOUND3CNT    =   0x70        @ �T�E���h�R�R���g���[��
OFFSET_REG_SOUND3CNT_L  =   0x70
OFFSET_REG_SOUND3CNT_H  =   0x72
OFFSET_REG_SOUND3CNT_X  =   0x74
OFFSET_REG_SOUND4CNT    =   0x78        @ �T�E���h�S�R���g���[��
OFFSET_REG_SOUND4CNT_L  =   0x78
OFFSET_REG_SOUND4CNT_H  =   0x7c

OFFSET_REG_WAVE_RAM     =   0x90        @ �T�E���h�R �g�`�q�`�l
OFFSET_REG_WAVE_RAM0    =   0x90
OFFSET_REG_WAVE_RAM0_L  =   0x90
OFFSET_REG_WAVE_RAM0_H  =   0x92
OFFSET_REG_WAVE_RAM1    =   0x94
OFFSET_REG_WAVE_RAM1_L  =   0x94
OFFSET_REG_WAVE_RAM1_H  =   0x96
OFFSET_REG_WAVE_RAM2    =   0x98
OFFSET_REG_WAVE_RAM2_L  =   0x98
OFFSET_REG_WAVE_RAM2_H  =   0x9a
OFFSET_REG_WAVE_RAM3    =   0x9c
OFFSET_REG_WAVE_RAM3_L  =   0x9c
OFFSET_REG_WAVE_RAM3_H  =   0x9e

OFFSET_REG_FIFO         =   0xa0        @ �T�E���h �e�h�e�n
OFFSET_REG_FIFO_A       =   0xa0
OFFSET_REG_FIFO_A_L     =   0xa0
OFFSET_REG_FIFO_A_H     =   0xa2
OFFSET_REG_FIFO_B       =   0xa4
OFFSET_REG_FIFO_B_L     =   0xa4
OFFSET_REG_FIFO_B_H     =   0xa6


OFFSET_REG_DMA0         =   0xb0        @ �c�l�`�O
OFFSET_REG_DMA0SAD      =   0xb0        @ �i�����������������������j
OFFSET_REG_DMA0SAD_L    =   0xb0
OFFSET_REG_DMA0SAD_H    =   0xb2
OFFSET_REG_DMA0DAD      =   0xb4
OFFSET_REG_DMA0DAD_L    =   0xb4
OFFSET_REG_DMA0DAD_H    =   0xb6
OFFSET_REG_DMA0CNT      =   0xb8
OFFSET_REG_DMA0CNT_L    =   0xb8
OFFSET_REG_DMA0CNT_H    =   0xba
OFFSET_REG_DMA1         =   0xbc        @ �c�l�`�P
OFFSET_REG_DMA1SAD      =   0xbc        @ �i�������������T�E���hFIFO�^�����������j
OFFSET_REG_DMA1SAD_L    =   0xbc
OFFSET_REG_DMA1SAD_H    =   0xbe
OFFSET_REG_DMA1DAD      =   0xc0
OFFSET_REG_DMA1DAD_L    =   0xc0
OFFSET_REG_DMA1DAD_H    =   0xc2
OFFSET_REG_DMA1CNT      =   0xc4
OFFSET_REG_DMA1CNT_L    =   0xc4
OFFSET_REG_DMA1CNT_H    =   0xc6
OFFSET_REG_DMA2         =   0xc8        @ �c�l�`�Q
OFFSET_REG_DMA2SAD      =   0xc8        @ �i�������������T�E���hFIFO�^�����������j
OFFSET_REG_DMA2SAD_L    =   0xc8
OFFSET_REG_DMA2SAD_H    =   0xca
OFFSET_REG_DMA2DAD      =   0xcc
OFFSET_REG_DMA2DAD_L    =   0xcc
OFFSET_REG_DMA2DAD_H    =   0xce
OFFSET_REG_DMA2CNT      =   0xd0
OFFSET_REG_DMA2CNT_L    =   0xd0
OFFSET_REG_DMA2CNT_H    =   0xd2
OFFSET_REG_DMA3         =   0xd4        @ �c�l�`�R
OFFSET_REG_DMA3SAD      =   0xd4        @ �i�J�Z�b�g�^�������������������������j
OFFSET_REG_DMA3SAD_L    =   0xd4
OFFSET_REG_DMA3SAD_H    =   0xd6
OFFSET_REG_DMA3DAD      =   0xd8
OFFSET_REG_DMA3DAD_L    =   0xd8
OFFSET_REG_DMA3DAD_H    =   0xda
OFFSET_REG_DMA3CNT      =   0xdc
OFFSET_REG_DMA3CNT_L    =   0xdc
OFFSET_REG_DMA3CNT_H    =   0xde


OFFSET_REG_TM0CNT       =   0x100       @ �^�C�}�[�O
OFFSET_REG_TM0CNT_L     =   0x100
OFFSET_REG_TM0CNT_H     =   0x102
OFFSET_REG_TM1CNT       =   0x104       @ �^�C�}�[�P
OFFSET_REG_TM1CNT_L     =   0x104
OFFSET_REG_TM1CNT_H     =   0x106
OFFSET_REG_TM2CNT       =   0x108       @ �^�C�}�[�Q
OFFSET_REG_TM2CNT_L     =   0x108
OFFSET_REG_TM2CNT_H     =   0x10a
OFFSET_REG_TM3CNT       =   0x10c       @ �^�C�}�[�R
OFFSET_REG_TM3CNT_L     =   0x10c
OFFSET_REG_TM3CNT_H     =   0x10e


OFFSET_REG_RCNT         =	0x134       @ �ėp���o�̓R���g���[��

OFFSET_REG_SIOCNT       =   0x128       @ �V���A���ʐM�R���g���[��
OFFSET_REG_SIODATA8     =	0x12a       @  8bit�V���A���ʐM�f�[�^
OFFSET_REG_SIODATA32    =	0x120       @ 32bit�V���A���ʐM�f�[�^
OFFSET_REG_SIOMLT_SEND  =   0x12a       @ �}���`�v���C�r�h�n ���M�f�[�^
OFFSET_REG_SIOMLT_RECV  =	0x120       @ �}���`�v���C�r�h�n ��M�f�[�^
OFFSET_REG_SIOMULTI0    =   0x120
OFFSET_REG_SIOMULTI1    =   0x122
OFFSET_REG_SIOMULTI2    =   0x124
OFFSET_REG_SIOMULTI3    =   0x126

OFFSET_REG_JOYCNT       =   0x140       @ �i�n�x�o�X�ʐM�R���g���[��
OFFSET_REG_JOYSTAT      =   0x158       @ �i�n�x�o�X�@�X�e�[�^�X
OFFSET_REG_JOY_RECV     =   0x150       @ �i�n�x�o�X�@��M�f�[�^
OFFSET_REG_JOY_RECV_L   =   0x150
OFFSET_REG_JOY_RECV_H   =   0x152
OFFSET_REG_JOY_TRANS    =   0x154       @ �i�n�x�o�X�@���M�f�[�^
OFFSET_REG_JOY_TRANS_L  =   0x154
OFFSET_REG_JOY_TRANS_H  =   0x156


OFFSET_REG_KEYINPUT     =   0x130       @ �L�[����
OFFSET_REG_KEYCNT       =   0x132       @ �L�[�R���g���[��


