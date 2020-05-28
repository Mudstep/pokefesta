/********************************************************************/
/*          AgbTypes.h                                              */
/*            AGB�W���^�錾                                         */
/*                                                                  */
/*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               */
/********************************************************************/
#ifndef _AGB_TYPES_H
#define _AGB_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------*/
/*                      �f�[�^�T�C�Y�Ē�`                          */
/*------------------------------------------------------------------*/

typedef     unsigned char           u8;
typedef     unsigned short int      u16;
typedef     unsigned int            u32;
typedef     unsigned long long int  u64;

typedef     signed char             s8;
typedef     signed short int        s16;
typedef     signed int              s32;
typedef     signed long long int    s64;

typedef     float                   f32;
typedef     double                  f64;


#define     vl                      volatile

typedef     vl u8                   vu8;
typedef     vl u16                  vu16;
typedef     vl u32                  vu32;
typedef     vl u64                  vu64;

typedef     vl s8                   vs8;
typedef     vl s16                  vs16;
typedef     vl s32                  vs32;
typedef     vl s64                  vs64;

typedef     vl f32                  vf32;
typedef     vl f64                  vf64;


/*------------------------------------------------------------------*/
/*                          �\����                                  */
/*------------------------------------------------------------------*/

// �v���O�����X�e�[�^�X���W�X�^�\����
typedef struct {
    u32 CpuMode:5;          // �b�o�t���[�h
    u32 Thumb_State:1;      // �s�g�t�l�a�X�e�[�g
    u32 Fiq_Disable:1;      // �e�h�p�s����
    u32 Irq_Disable:1;      // �h�q�p�s����
    u32 Dummy_27_8:20;
    u32 V_Flag:1;           // �I�[�o�[�t���[
    u32 C_Flag:1;           // �L�����[�^�{���[�^�g��
    u32 Z_Flag:1;           // �[��
    u32 N_Flag:1;           // ���^����
} PsrData;
typedef  vl PsrData    vPsrData;

#define ST_PSR_USER_MODE            0x10    // ���[�U
#define ST_PSR_FIQ_MODE             0x11    // �e�h�p
#define ST_PSR_IRQ_MODE             0x12    // �h�q�p
#define ST_PSR_SVC_MODE             0x13    // �X�[�p�o�C�U
#define ST_PSR_ABORT_MODE           0x17    // �A�{�[�g�i�v���t�F�b�`�^�f�[�^�j
#define ST_PSR_UNDEF_MODE           0x1b    // ����`����
#define ST_PSR_SYS_MODE             0x1f    // �V�X�e��


// ���荞�ݗv���^���t���O�\����
typedef struct {
    u16 V_Blank:1;          // �u�u�����N
    u16 H_Blank:1;          // �g�u�����N
    u16 V_Count:1;          // �u�J�E���^��v
    u16 Timer0:1;           // �^�C�}�[�O
    u16 Timer1:1;           // �^�C�}�[�P
    u16 Timer2:1;           // �^�C�}�[�Q
    u16 Timer3:1;           // �^�C�}�[�R
    u16 Sio:1;              // �V���A���ʐM
    u16 Dma0:1;             // �c�l�`�O
    u16 Dma1:1;             // �c�l�`�P
    u16 Dma2:1;             // �c�l�`�Q
    u16 Dma3:1;             // �c�l�`�R
    u16 Key:1;              // �L�[
    u16 Cassette:1;         // �J�Z�b�g
    u16 Dummy_15_14:2;
} IntrFlags;
typedef  vl IntrFlags    vIntrFlags;


// �\���R���g���[���\����
typedef struct {
    u16 BgMode:3;           //   �a�f���[�h�I��
    u16 CgbMode:1;          // �b�f�a���[�h�I��
    u16 Bmp_FrameNo:1;      // �r�b�g�}�b�v���[�h �\���t���[�� �I��
    u16 Obj_H_Off:1;        // �g�u�����N���n�a�i���� �n�e�e
    u16 ObjCharMapType:1;   // �n�a�i�L�����N�^�f�[�^ �}�b�s���O�^�C�v
    u16 Lcdc_Off:1;         // �k�b�c�b �n�e�e
    u16 Bg0_On:1;           // �a�f�O �n�m
    u16 Bg1_On:1;           // �a�f�P �n�m
    u16 Bg2_On:1;           // �a�f�Q �n�m
    u16 Bg3_On:1;           // �a�f�R �n�m
    u16 Obj_On:1;           // �n�a�i �n�m
    u16 Win0_On:1;          // �E�C���h�E�O �n�m
    u16 Win1_On:1;          // �E�C���h�E�P �n�m
    u16 ObjWin_On:1;        // �n�a�i�E�C���h�E �n�m
} DispCnt;
typedef  vl DispCnt   vDispCnt;

#define ST_DISP_OBJ_CHAR_2D_MAP     0       // �n�a�i�L�����N�^�f�[�^ �Q�c�}�b�s���O
#define ST_DISP_OBJ_CHAR_1D_MAP     1       // �n�a�i�L�����N�^�f�[�^ �P�c�}�b�s���O


// �X�e�[�^�X�R���g���[���\����
typedef struct {
    u16 V_Blank_Flag:1;     // �u�u�����N���Ԓ�
    u16 H_Blank_Flag:1;     // �g�u�����N���Ԓ�
    u16 V_Count_Flag:1;     // �u�J�E���^��v��
    u16 V_Blank_IF_Enable:1;// �u�u�����N���荞�ݗv�� ����
    u16 H_Blank_IF_Enable:1;// �g�u�����N���荞�ݗv�� ����
    u16 V_Count_IF_Enable:1;// �u�J�E���^��v���荞�ݗv�� ����
    u16 Dummy_7_6:2;
    u8  V_Count_Cmp;        // �u�J�E���^��r�l
    u8  V_Count;            // �u�J�E���^�l
    u8  Dummy_31_24;
} StatCnt;
typedef  vl StatCnt   vStatCnt;


// ���U�C�N�R���g���[���\����
typedef struct {
    u16 Bg_HSize:4;         //   �a�f���U�C�N �g�T�C�Y
    u16 Bg_VSize:4;         //   �a�f���U�C�N �u�T�C�Y
    u16 Obj_HSize:4;        // �n�a�i���U�C�N �g�T�C�Y
    u16 Obj_VSize:4;        // �n�a�i���U�C�N �u�T�C�Y
} MosCnt;
typedef  vl MosCnt    vMosCnt;


// �u�����h�R���g���[���\����
typedef struct {
    u16 Pixel_1st_Bg0_On:1; // �P�����s�N�Z��
    u16 Pixel_1st_Bg1_On:1;
    u16 Pixel_1st_Bg2_On:1;
    u16 Pixel_1st_Bg3_On:1;
    u16 Pixel_1st_Obj_On:1;
    u16 Pixel_1st_Bd_On:1;
    u16 BlendMode:2;        // �u�����h���[�h
    u16 Pixel_2nd_Bg0_On:1; // �Q�����s�N�Z��
    u16 Pixel_2nd_Bg1_On:1;
    u16 Pixel_2nd_Bg2_On:1;
    u16 Pixel_2nd_Bg3_On:1;
    u16 Pixel_2nd_Obj_On:1;
    u16 Pixel_2nd_Bd_On:1;
    u16 Dummy_15_14:2;
    u16 Value_A:5;          // �p�����[�^ �`
    u16 Dummy_23_21:3;
    u16 Value_B:5;          // �p�����[�^ �a
    u16 Dummy_31_29:3;
    u16 Value_Y:5;          // �p�����[�^ �x
    u16 Dummy_39_37:3;
} BlendCnt;
typedef  vl BlendCnt    vBlendCnt;

#define ST_BLD_NORMAL_MODE          0       // �m�[�}�����[�h
#define ST_BLD_A_BLEND_MODE         1       // ���������[�h
#define ST_BLD_UP_MODE              2       // �P�x�t�o���[�h
#define ST_BLD_DOWN_MODE            3       // �P�x�c�n�v�m���[�h


// �a�f�R���g���[���\����
typedef struct {
    u16 Priority:2;         // �\���D��
    u16 CharBasep:2;        // �L�����N�^ �x�[�X�A�h���X
    u16 Dummy_5_4:2;
    u16 Mosaic:1;           // ���U�C�N
    u16 ColorMode:1;        // �P�U�F�^�Q�T�U�F �I��
    u16 ScBasep:5;          // �X�N���[�� �x�[�X�A�h���X
    u16 Loop:1;             // ���[�v
    u16 Size:2;             // �X�N���[���T�C�Y
} BgCnt;
typedef  vl BgCnt    vBgCnt;

#define ST_BG_COLOR_16              0       // �P�U�F   �I��
#define ST_BG_COLOR_256             1       // �Q�T�U�F �I��


// �a�f�I�t�Z�b�g�R���g���[���\����
typedef struct {
    u16 H;
    u16 V;
} BgOffsetCnt;
typedef  vl BgOffsetCnt    vBgOffsetCnt;


// �a�f�X�N���[���\����
typedef struct {
    u16 CharNo:10;          // �L�����N�^�m��
    u16 HFlip:1;            // �g�t���b�v
    u16 VFlip:1;            // �u�t���b�v
    u16 Pltt:4;             // �p���b�g�m��
} BgScData;
typedef  vl BgScData       vBgScData;


// �a�f�A�t�B���ϊ��\�[�X�f�[�^�\����
typedef struct {
    s32 SrcCenterX;         // ���f�[�^���S���W << 8
    s32 SrcCenterY;
    s16 DispCenterX;        // �\�����S���W
    s16 DispCenterY;
    s16 RatioX;             // �g��k���� << 8
    s16 RatioY;
    u16 Theta;              // ��]�p�i0 - 255�j<< 8
} BgAffineSrcData;
typedef  vl BgAffineSrcData    vBgAffineSrcData;

// �a�f�A�t�B���ϊ��f�X�e�B�l�[�V�����f�[�^�\����
typedef struct {
    s16 H_DiffX;            // ���C�������w���W����
    s16 V_DiffX;            //   ���������w���W����
    s16 H_DiffY;            // ���C�������x���W����
    s16 V_DiffY;            //   ���������x���W����
    s32 StartX;             // �X�^�[�g�w���W
    s32 StartY;             // �X�^�[�g�x���W
} BgAffineDestData;
typedef  vl BgAffineDestData   vBgAffineDestData;


// �n�`�l�\����
typedef struct {
    u32 VPos:8;             // �x���W
    u32 AffineMode:2;       // �A�t�B�����[�h
    u32 ObjMode:2;          // �n�a�i���[�h
    u32 Mosaic:1;           // ���U�C�N
    u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
    u32 Shape:2;            // �n�a�i�`��

    u32 HPos:9;             // �w���W
    u32 AffineParamNo_L:3;  // �A�t�B���ϊ��p�����[�^�m�� ���ʂR�r�b�g
    u32 HFlip:1;            // �g�t���b�v
    u32 VFlip:1;            // �u�t���b�v
    u32 Size:2;             // �n�a�i�T�C�Y

    u16 CharNo:10;          // �L�����N�^�m��
    u16 Priority:2;         // �\���D��
    u16 Pltt:4;             // �p���b�g�m��
    u16 AffineParam;        // �A�t�B���ϊ��p�����[�^
} OamData;
typedef  vl OamData    vOamData;

#define ST_OAM_OBJ_NORMAL           0       // �n�a�i�m�[�}�����[�h
#define ST_OAM_OBJ_BLEND            1       // �n�a�i���������[�h
#define ST_OAM_OBJ_WINDOW           2       // �n�a�i�E�C���h�E���[�h
#define ST_OAM_AFFINE_NONE          0       // �A�t�B���������[�h
#define ST_OAM_AFFINE_NORMAL        1       // �A�t�B���m�[�}�����[�h
#define ST_OAM_AFFINE_TWICE         3       // �A�t�B���Q�{�p���[�h
#define ST_OAM_AFFINE_ERASE         2       // �A�t�B����\�����[�h
#define ST_OAM_COLOR_16             0       // �P�U     �I��
#define ST_OAM_COLOR_256            1       // �Q�T�U�F �I��
#define ST_OAM_SQUARE               0       // �����`�n�a�i
#define ST_OAM_H_RECTANGLE          1       // �������`�n�a�i
#define ST_OAM_V_RECTANGLE          2       // �c�����`�n�a�i


// �n�a�i�A�t�B���ϊ��\�[�X�f�[�^�\����
typedef struct {
    s16 RatioX;             // �g��k����
    s16 RatioY;
    u16 Theta;              // ��]�p�i0 - 255�j<< 8
} ObjAffineSrcData;
typedef  vl ObjAffineSrcData   vObjAffineSrcData;

// �n�a�i�A�t�B���ϊ��f�X�e�B�l�[�V�����f�[�^�\����
typedef struct {
    s16 H_DiffX;            // ���C�������w���W����
    s16 V_DiffX;            //   ���������w���W����
    s16 H_DiffY;            // ���C�������x���W����
    s16 V_DiffY;            //   ���������x���W����
} ObjAffineDestData;
typedef  vl ObjAffineDestData  vObjAffineDestData;


// �J���[�p���b�g�\����
typedef struct {
    u16 Red:5;              // ��
    u16 Green:5;            // ��
    u16 Blue:5;             // ��
    u16 Dummy_15:1;
} PlttData;
typedef  vl PlttData    vPlttData;


// �E�C���h�E�R���g���[���\����
typedef struct {
    u8  Bg0_On:1;           // �a�f�O �n�m
    u8  Bg1_On:1;           // �a�f�P �n�m
    u8  Bg2_On:1;           // �a�f�Q �n�m
    u8  Bg3_On:1;           // �a�f�R �n�m
    u8  Obj_On:1;           // �n�a�i �n�m
    u8  Blend_On:1;         // �J���[������� �n�m
    u8  Dummy_7_6:2;
} WindCnt;
typedef  vl WindCnt     vWindCnt;


// �T�E���h�R���g���[���\����
typedef struct {
    u8  So1_Level:3;        // �r�n�P�o�̓��x��
    u8  So1_Vin_On:1;       // �u�������r�n�P �n�m
    u8  So2_Level:3;        // �r�n�Q�o�̓��x��
    u8  So2_Vin_On:1;       // �u�������r�n�Q �n�m
    u8  So1_Sound1_On:1;    // �T�E���h�P���r�n�P �n�m
    u8  So1_Sound2_On:1;    // �T�E���h�Q���r�n�P �n�m
    u8  So1_Sound3_On:1;    // �T�E���h�R���r�n�P �n�m
    u8  So1_Sound4_On:1;    // �T�E���h�S���r�n�P �n�m
    u8  So2_Sound1_On:1;    // �T�E���h�P���r�n�Q �n�m
    u8  So2_Sound2_On:1;    // �T�E���h�Q���r�n�Q �n�m
    u8  So2_Sound3_On:1;    // �T�E���h�R���r�n�Q �n�m
    u8  So2_Sound4_On:1;    // �T�E���h�S���r�n�Q �n�m
    u8  Sound_1_4_Mix:2;    // �c�l�f�݊��T�E���h ������
    u8  DirectA_Mix:1;      // �_�C���N�g�T�E���h�` ������
    u8  DirectB_Mix:1;      // �_�C���N�g�T�E���h�a ������
    u8  Dummy_23_20:4;
    u8  So1_DirectA_On:1;   // �_�C���N�g�T�E���h�`���r�n�P �n�m
    u8  So2_DirectA_On:1;   // �_�C���N�g�T�E���h�`���r�n�Q �n�m
    u8  DirectA_Timer:1;    // �_�C���N�g�T�E���h�`�p�^�C�}�[�I��
    u8  FifoA_Reset:1;      // �_�C���N�g�T�E���h�`�p�e�h�e�n ���Z�b�g
    u8  So1_DirectB_On:1;   // �_�C���N�g�T�E���h�a���r�n�P �n�m
    u8  So2_DirectB_On:1;   // �_�C���N�g�T�E���h�a���r�n�Q �n�m
    u8  DirectB_Timer:1;    // �_�C���N�g�T�E���h�a�p�^�C�}�[�I��
    u8  FifoB_Reset:1;      // �_�C���N�g�T�E���h�a�p�e�h�e�n ���Z�b�g
    u8  Sound1_On:1;        // �T�E���h�P �n�m
    u8  Sound2_On:1;        // �T�E���h�Q �n�m
    u8  Sound3_On:1;        // �T�E���h�R �n�m
    u8  Sound4_On:1;        // �T�E���h�S �n�m
    u8  Dummy_38_36:3;
    u8  Sound_1_4_On:1;     // �c�l�f�݊��T�E���h �n�m
    u8  Dummy_47_40;
    u16 Dummy_63_48;
    u16 BiasLevel:10;       // �a�h�`�r���x��
    u16 Dummy_77_74:4;
    u16 OutFrequency:2;     // �Đ����g��
} SoundCnt;
typedef  vl SoundCnt     vSoundCnt;

#define ST_SOUND_DMG_MIX_1_4        0       // �c�l�f�݊��T�E���h������ �P�^�S
#define ST_SOUND_DMG_MIX_2_4        1       //                          �Q�^�S
#define ST_SOUND_DMG_MIX_FULL       2       //                          �t�������W
#define ST_SOUND_DIRECT_MIX_1_2     0       // �_�C���N�g�T�E���h������ �P�^�Q
#define ST_SOUND_DIRECT_MIX_FULL    1       //                          �t�������W


// �T�E���h�P�R���g���[���\����
typedef struct {
    u8  SweepShift:3;       // �X�C�[�v �V�t�g��
    u8  SweepUpDownt:1;     // �X�C�[�v ����
    u8  SweepTime:3;        // �X�C�[�v ����
    u8  Dummy_7:1;
    u8  Dummy_15_8;
    u8  Counts:6;           // ���̒���(�J�E���g��)
    u8  Duty:2;             // �g�`�̃f���[�e�B
    u8  EnvSteps:3;         // �G���x���[�v �X�e�b�v��
    u8  EnvUp_Down:1;       // �G���x���[�v �t�o�^�c�n�v�m
    u8  EnvInitData:4;      // �G���x���[�v �����l
    u16 Frequency:11;       // ���g��
    u16 Dummy_45_43:3;
    u16 Counter_On:1;       // �J�E���^�[ �n�m
    u16 Start:1;            // �X�^�[�g
} Sound1Cnt;
typedef  vl Sound1Cnt    vSound1Cnt;

// �T�E���h�Q�R���g���[���\����
typedef struct {
    u8  Counts:6;           // ���̒���(�J�E���g��)
    u8  Duty:2;             // �g�`�̃f���[�e�B
    u8  EnvSteps:3;         // �G���x���[�v �X�e�b�v��
    u8  EnvUp_Down:1;       // �G���x���[�v �t�o�^�c�n�v�m
    u8  EnvInitData:4;      // �G���x���[�v �����l
    u16 Dummy_31_16;
    u16 Frequency:11;       // ���g��
    u16 Dummy_45_43:3;
    u16 Counter_On:1;       // �J�E���^�[ �n�m
    u16 Start:1;            // �X�^�[�g
} Sound2Cnt;
typedef  vl Sound2Cnt    vSound2Cnt;

#define ST_SOUND_SWEEP_UP           0       // �X�C�[�v ���Z
#define ST_SOUND_SWEEP_DOWN         1       //          ���Z
#define ST_SOUND_DUTY_1_8           0       // �g�`�̃f���[�e�B �P�^�W
#define ST_SOUND_DUTY_2_8           1       //                  �Q�^�W
#define ST_SOUND_DUTY_4_8           2       //                  �S�^�W
#define ST_SOUND_DUTY_6_8           3       //                  �U�^�W
#define ST_SOUND_ENV_UP             1       // �G���x���[�v �t�o
#define ST_SOUND_ENV_DOWN           0       //              �c�n�v�m


// �T�E���h�R�R���g���[���\����
typedef struct {
    u8  Dummy_6_0:5;
    u8  Connect:1;          // �g�`�q�`�l����
    u8  Bank:1;             // �g�`�q�`�l�o���N
    u8  On:1;               // �T�E���h�R �n�m
    u8  Dummy_15_8;
    u8  Counts;             // ���̒���(�J�E���g��)
    u8  Dummy_28_24:5;
    u8  OutputLevel:3;      // �o�̓��x��
    u16 Frequency:11;       // ���g��
    u16 Dummy_45_43:3;
    u16 Counter_On:1;       // �J�E���^�[ �n�m
    u16 Start:1;            // �X�^�[�g
} Sound3Cnt;
typedef  vl Sound3Cnt    vSound3Cnt;

#define ST_SOUND_3_LEVEL_0_4        0       // �T�E���h�R �o�� �Ȃ�
#define ST_SOUND_3_LEVEL_FULL       1       //                 �S�^�S
#define ST_SOUND_3_LEVEL_2_4        2       //                 �Q�^�S
#define ST_SOUND_3_LEVEL_1_4        3       //                 �P�^�S
#define ST_SOUND_3_LEVEL_3_4        4       //                 �R�^�S


// �T�E���h�S�R���g���[���\����
typedef struct {
    u8  Counts:6;           // ���̒���(�J�E���g��)
    u8  Dummy_15_14:2;
    u8  EnvSteps:3;         // �G���x���[�v �X�e�b�v��
    u8  EnvUp_Down:1;       // �G���x���[�v �t�o�^�c�n�v�m
    u8  EnvInitData:4;      // �G���x���[�v �����l
    u16 Dummy_31_16;
    u8  PreScaler:3;        // �v���X�P�[���I��
    u8  PolySteps:1;        // �|���m�~�J���E�J�E���^�̒i���I��
    u8  PolyShift:4;        // �|���m�~�J���E�J�E���^ �V�t�g��
    u8  Dummy45_40:6;
    u8  Counter_On:1;       // �J�E���^�[ �n�m
    u8  Start:1;            // �X�^�[�g
} Sound4Cnt;
typedef  vl Sound4Cnt    vSound4Cnt;

#define ST_SOUND_4_POLYSTEP_15      0       // �T�E���h�S �|���m�~�J���E�J�E���^ �P�T�i
#define ST_SOUND_4_POLYSTEP_7       1       //                                     �V�i


// �m�[�}���r�h�n�R���g���[���\����
typedef struct {
    u16 Sck_I_O:1;          // �N���b�N�h/�n�I��
    u16 Sck:1;              // �����N���b�N�I��
    u16 AckRecv:1;          // �]�����t���O��M
    u16 AckSend:1;          // �]�����t���O���M
    u16 Dummy_6_4:3;
    u16 Enable:1;           // �r�h�n����
    u16 Dummy_11_8:4;
    u16 Mode:2;             // �ʐM���[�h�I��
    u16 IF_Enable:1;        // ���荞�ݗv�� ����
    u16 Dammy_15:1;
    u8  Data;               // �f�[�^
    u8  Dummy_31_24;
} SioNormalCnt;
typedef  vl SioNormalCnt    vSioNormalCnt;

#define ST_SIO_8BIT_MODE            0       // �m�[�}���W�r�b�g�ʐM���[�h
#define ST_SIO_32BIT_MODE           1       // �m�[�}���R�Q�r�b�g�ʐM���[�h

#define ST_SIO_SCK_OUT              0       // �O���N���b�N�I��
#define ST_SIO_SCK_IN               1       // �����N���b�N�I��
#define ST_SIO_IN_SCK_256K          0       // �����N���b�N �Q�T�U�j�g���I��
#define ST_SIO_IN_SCK_2M            1       //                  �Q�l�g���I��


// �}���`�v���C�r�h�n�R���g���[���\����
typedef struct {
    u16 BaudRate:2;         // �{�[���[�g
    u16 SI:1;               // �r�h�[�q
    u16 SD:1;               // �r�c�[�q
    u16 ID:2;               // �h�c
    u16 Error:1;            // �G���[���o
    u16 Enable:1;           // �r�h�n����
    u16 Dummy_11_8:4;
    u16 Mode:2;             // �ʐM���[�h�I��
    u16 IF_Enable:1;        // ���荞�ݗv�� ����
    u16 Dammy_15:1;
    u16 Data;               // �f�[�^
} SioMultiCnt;
typedef  vl SioMultiCnt     vSioMultiCnt;

#define ST_SIO_MULTI_MODE           2       // �}���`�v���C�ʐM���[�h

#define ST_SIO_9600_BPS             0       // �{�[���[�g �X�U�O�O�a�o�r
#define ST_SIO_38400_BPS            1       //          �R�W�S�O�O�a�o�r
#define ST_SIO_57600_BPS            2       //          �T�V�U�O�O�a�o�r
#define ST_SIO_115200_BPS           3       //        �P�P�T�Q�O�O�a�o�r
#define ST_SIO_MULTI_PARENT         1       // �}���`�v���C�ʐM �e�ڑ�
#define ST_SIO_MULTI_CHILD          0       //                  �q�ڑ�


// �t�`�q�s�|�r�h�n�R���g���[���\����
typedef struct {
    u16 BaudRate:2;         // �{�[���[�g
    u16 Cts_Enable:1;       // ���M�\�M�� �C�l�[�u��
    u16 ParitySelect:1;     // �p���e�B �����^�
    u16 TransDataFull:1;    // �g�����X�~�b�g�f�[�^ �t��
    u16 RecvDataEmpty:1;    // ���V�[�u�f�[�^ �G���v�e�B
    u16 Error:1;            // �G���[���o
    u16 Length:1;           // �f�[�^��
    u16 Fifo_Enable:1;      // �e�h�e�n �C�l�[�u��
    u16 Parity_Enable:1;    // �p���e�B �C�l�[�u��
    u16 Trans_Enable:1;     // �g�����X�~�b�^ �C�l�[�u��
    u16 Recv_Enable:1;      // ���V�[�o �C�l�[�u��
    u16 Mode:2;             // �ʐM���[�h�I��
    u16 IF_Enable:1;        // ���荞�ݗv�� ����
    u16 Dammy_15:1;
    u8  Data;               // �f�[�^
    u8  Dummy_31_24;
} SioUartCnt;
typedef  vl SioUartCnt		vSioUartCnt;

#define ST_SIO_UART_MODE            3       // �t�`�q�s�ʐM���[�h

#define ST_SIO_UART_7BIT            0       // �t�`�q�s�ʐM �f�[�^�� �V������
#define ST_SIO_UART_8BIT            1       //                       �W������
#define ST_SIO_PARITY_EVEN          0       // �����p���e�B
#define ST_SIO_PARITY_ODD           1       // ��p���e�B


// �i�n�x�o�X�ʐM�R���g���[���\����
typedef struct {
    u8  IF_Reset:1;         // �i�n�x�o�X�E���Z�b�g ���荞�ݗv��
    u8  IF_Recv:1;          // �i�n�x�o�X��M����   ���荞�ݗv��
    u8  IF_Send:1;          // �i�n�x�o�X���M����   ���荞�ݗv��
    u8  Dummy_5_3:3;
    u8  IF_Enable:1;        // ���荞�ݗv�� ����
    u8  Dummy_7:1;
} JoyCnt;
typedef  vl JoyCnt     vJoyCnt;

// �i�n�x�o�X�ʐM�X�e�[�^�X�\����
typedef struct {
    u8  Dummy_0:1;
    u8  Recv:1;             // ��M�X�e�[�^�X
    u8  Dummy_2:1;
    u8  Send:1;             // ���M�X�e�[�^�X
    u8  Flags:2;            // �ėp�t���O
    u8  Dummy_7_6:2;
} JoyStat;
typedef  vl JoyStat    vJoyStat;


// �ėp���o�̓R���g���[���\����
typedef struct {
    u8  SC:1;               // �f�[�^
    u8  SD:1;
    u8  SI:1;
    u8  SO:1;
    u8  SC_I_O:1;           // �h/�n�I��
    u8  SD_I_O:1;
    u8  SI_I_O:1;
    u8  SO_I_O:1;
    u8  IF_Enable:1;        // ���荞�ݗv�� ����
    u8  Dummy_13_9:5;
    u8  SioModeMaster:2;   	// �r�h�n���[�h�E�}�X�^
} RCnt;
typedef  vl RCnt      vRCnt;

#define ST_R_SIO_MASTER_MODE        0       // �r�h�n�}�X�^�E���[�h
#define ST_R_DIRECT_MODE            2       // �ėp���o�͒ʐM���[�h
#define ST_R_JOY_MODE               3       // �i�n�x�ʐM���[�h

#define ST_R_IN                     0       // ���͑I��
#define ST_R_OUT                    1       // �o�͑I��


// �c�l�`�R���g���[���\����
typedef struct {
    u16 Count;              // �]����
    u16 Dummy_21_16:5;
    u16 DestpCnt:2;         // �]����A�h���X �R���g���[��
    u16 SrcpCnt:2;          // �]�����A�h���X �R���g���[��
    u16 ContinuousON:1;     // �R���e�B�j���A�X���[�h
    u16 BusSize:1;          // �o�X�T�C�Y �P�U/�R�Q�a�����I��
    u16 DataRequest:1;      // �f�[�^�v���������[�h
    u16 Timming:2;          // �^�C�~���O�I��
    u16 IF_Enable:1;        // ���荞�ݗv�� ����
    u16 Enable:1;           // �c�l�`����
} DmaCnt;
typedef  vl DmaCnt    vDmaCnt;

#define ST_DMA_TIMMING_IMM          0       // �����ɋN��
#define ST_DMA_TIMMING_V_BLANK      1       // �u�u�����N�N��
#define ST_DMA_TIMMING_H_BLANK      2       // �g�u�����N�N��
#define ST_DMA_TIMMING_DISP         3       // �\���N��
#define ST_DMA_TIMMING_SOUND        3       // �T�E���h�e�h�e�n���N�G�X�g�N��
#define ST_DMA_16BIT_BUS            0       // �o�X�T�C�Y�P�U�a�����I��
#define ST_DMA_32BIT_BUS            1       // �o�X�T�C�Y�R�Q�a�����I��
#define ST_DMA_INC                  0       // �A�h���X�E�C���N�������g �I��
#define ST_DMA_DEC                  1       // �A�h���X�E�f�N�������g   �I��
#define ST_DMA_FIX                  2       // �A�h���X�Œ� �I��
#define ST_DMA_RELOAD               3       // �A�h���X�E�C���N�������g/�����[�h �I��


// �^�C�}�[�R���g���[���\����
typedef struct {
    u16 Count;              // �J�E���g��
    u16 PreScaler:2;        // �v���X�P�[���I��
    u16 Connect:1;          // ���ʃ^�C�}�[�ڑ�
    u16 Dummy_5_3:3;
    u16 IF_Enable:1;        // ���荞�ݗv�� ����
    u16 Enable:1;           // �^�C�}�[����
    u16 Dummy_15_8:8;
} TimerCnt;
typedef  vl TimerCnt    vTimerCnt;

#define ST_TMR_PRESCALER_1CK        0        // �v���X�P�[�� �P����
#define ST_TMR_PRESCALER_64CK       1        //            �U�S����
#define ST_TMR_PRESCALER_256CK      2        //          �Q�T�U����
#define ST_TMR_PRESCALER_1024CK     3        //        �P�O�Q�S����


// �L�[�f�[�^�\����
typedef struct {
    u16 A:1;                // �`
    u16 B:1;                // �a
    u16 Select:1;           // �Z���N�g
    u16 Start:1;            // �X�^�[�g
    u16 Plus_R:1;           // �E
    u16 Plus_L:1;           // ��
    u16 Plus_U:1;           // ��
    u16 Plus_D:1;           // ��
    u16 R:1;                // �q
    u16 L:1;                // �k
    u16 Dummy_15_10:6;
} KeyData;
typedef  vl KeyData    vKeyData;

// �L�[�R���g���[���\����
typedef struct {
    u16 Sel_A:1;            //       �`�{�^�� �I��
    u16 Sel_B:1;            //       �a�{�^�� �I��
    u16 Sel_Select:1;       // �Z���N�g�{�^�� �I��
    u16 Sel_Start:1;        // �X�^�[�g�{�^�� �I��
    u16 Sel_Plus_R:1;       //         �E�L�[ �I��
    u16 Sel_Plus_L:1;       //         ���L�[ �I��
    u16 Sel_Plus_U:1;       //         ��L�[ �I��
    u16 Sel_Plus_D:1;       //         ���L�[ �I��
    u16 Sel_R:1;            //       �q�{�^�� �I��
    u16 Sel_L:1;            //       �k�{�^�� �I��
    u16 Dummy_13_10:4;
    u16 IF_Enable:1;        // ���荞�ݗv�� ����
    u16 IntrType:1;         // ���荞�݃^�C�v �I��
} KeyCnt;
typedef  vl KeyCnt    vKeyCnt;

#define ST_KEY_OR_INTR              0        //   �ʏ�L�[���荞��
#define ST_KEY_AND_INTR             1        // �`�m�c�L�[���荞��


// �J�Z�b�g�E�F�C�g�R���g���[���\����
typedef struct {
    u16 Sram:2;             // �r�q�`�l�E�F�C�g
    u16 Rom0_1st:2;         // �q�n�l�O �P�����E�F�C�g
    u16 Rom0_2nd:1;         //          �Q�����E�F�C�g
    u16 Rom1_1st:2;         // �q�n�l�P �P�����E�F�C�g
    u16 Rom1_2nd:1;         //          �Q�����E�F�C�g
    u16 Rom2_1st:2;         // �q�n�l�Q �P�����E�F�C�g
    u16 Rom2_2nd:1;         //          �Q�����E�F�C�g
    u16 OutClock:2;         // �o�̓N���b�N �I��
    u16 Dummy_13:1;
    u16 PrefetchEnale:1;    // �v���t�F�b�`�o�b�t�@ ����
    u16 CassetteType:1;     // �J�Z�b�g�^�C�v
} CstWaitCnt;
typedef  vl CstWaitCnt    vCstWaitCnt;

#define ST_CST_SRAM_4WAIT           0        // �r�q�`�l        �S�E�F�C�g
#define ST_CST_SRAM_3WAIT           1        //                 �R�E�F�C�g
#define ST_CST_SRAM_2WAIT           2        //                 �Q�E�F�C�g
#define ST_CST_SRAM_8WAIT           3        //                 �W�E�F�C�g
#define ST_CST_ROM_1ST_4WAIT        0        // �q�n�l   �P���� �S�E�F�C�g
#define ST_CST_ROM_1ST_3WAIT        1        //                 �R�E�F�C�g
#define ST_CST_ROM_1ST_2WAIT        2        //                 �Q�E�F�C�g
#define ST_CST_ROM_1ST_8WAIT        3        //                 �W�E�F�C�g
#define ST_CST_ROM0_2ND_2WAIT       0        // �q�n�l�O �Q���� �Q�E�F�C�g
#define ST_CST_ROM0_2ND_1WAIT       1        //                 �P�E�F�C�g
#define ST_CST_ROM1_2ND_4WAIT       0        // �q�n�l�P �Q���� �S�E�F�C�g
#define ST_CST_ROM1_2ND_1WAIT       1        //                 �P�E�F�C�g
#define ST_CST_ROM2_2ND_8WAIT       0        // �q�n�l�Q �Q���� �W�E�F�C�g
#define ST_CST_ROM2_2ND_1WAIT       1        //                 �P�E�F�C�g

#define ST_CST_PHI_OUT_NONE         0        // �Ӓ[�q�o�̓N���b�N �k���Œ�
#define ST_CST_PHI_OUT_4MCK         1        //                    �S�l�g��
#define ST_CST_PHI_OUT_8MCK         2        //                    �W�l�g��
#define ST_CST_PHI_OUT_16MCK        3        //                  �P�U�l�g��
#define ST_CST_AGB                  0        // �`�f�a�J�Z�b�g
#define ST_CST_CGB                  1        // �b�f�a�J�Z�b�g


// Bit���k�f�[�^�W�J�p�p�����[�^
typedef struct {
    u16 SrcNum;             // �\�[�X�f�[�^�E�o�C�g��
    u8  SrcBitNum;          // �P�\�[�X�f�[�^�E�r�b�g��
    u8  DestBitNum;         // �P�f�X�e�B�l�[�V�����f�[�^�E�r�b�g��
    u32 DestOffset:31;      // �\�[�X�f�[�^�ɉ��Z���鐔
    u32 DestOffset0_On:1;   // �O�̃f�[�^�ɃI�t�Z�b�g�����Z���邩�ۂ��̃t���O
} BitUnPackParam;
typedef  vl BitUnPackParam    vBitUnPackParam;


#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif /* _AGB_TYPES_H */
