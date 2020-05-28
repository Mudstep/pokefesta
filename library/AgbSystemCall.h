/********************************************************************/
/*          AgbSystemCall.h                                         */
/*            AGB�V�X�e���R�[��                                     */
/*                                                                  */
/*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               */
/********************************************************************/
#ifndef _AGB_SYSTEM_CALL_H
#define _AGB_SYSTEM_CALL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <AgbTypes.h>
#include <AgbSound.h>
#include <AgbMultiBoot.h>


#ifdef  __GNUC__
#ifndef CODE32
#define SystemCall(No)   asm ("SWI    "#No)
#else
#define SystemCall(No)   asm ("SWI    "#No" << 16")
#endif
#else
#ifndef CODE32
#define SystemCall(No) __asm { SWI    No}
#else
#define SystemCall(No) __asm { SWI    No << 16}
#endif
#endif


/*------------------------------------------------------------------*/
/*                   �V�X�e���R�[���m��                             */
/*------------------------------------------------------------------*/

#define SWI_NO_RESTART                  0       // SoftReset()
#define SWI_NO_REGISTER_RAM_RESET       1       // SoftReset()/RegisterRamReset()
#define SWI_NO_HALT                     2       // Halt()
#define SWI_NO_STOP                     3       // Stop()

#define SWI_NO_INTR_WAIT                4       // IntrWait()
#define SWI_NO_VBLANK_INTR_WAIT         5       // VBlankIntrWait()

#define SWI_NO_DIV                      6       // Div()/DivRem()
#define SWI_NO_DIV_ARM                  7       // DivArm()/DivRemArm()
#define SWI_NO_SQRT                     8       // Sqrt()
#define SWI_NO_ARCTAN                   9       // ArcTan()
#define SWI_NO_ARCTAN2                  10      // ArcTan2()

#define SWI_NO_CPU_SET                  11      // CpuSet()
#define SWI_NO_CPU_FAST_SET             12      // CpuFastSet()
#define SWI_NO_BG_AFFINE_SET            14      // BgAffineSet()
#define SWI_NO_OBJ_AFFINE_SET           15      // ObjAffineSet()
#define SWI_NO_BIT_UNPACK               16      // BitUnPack()
#define SWI_NO_LZ77_UNCOMP_WRAM         17      // LZ77UnCompWram()
#define SWI_NO_LZ77_UNCOMP_VRAM         18      // LZ77UnCompVram()
#define SWI_NO_HUFF_UNCOMP              19      // HuffUnComp()
#define SWI_NO_RL_UNCOMP_WRAM           20      // RLUnCompWram()
#define SWI_NO_RL_UNCOMP_VRAM           21      // RLUnCompVram()
#define SWI_NO_DIFF_8BIT_UNFILTER_WRAM  22      // Diff8bitUnFilterWram()
#define SWI_NO_DIFF_8BIT_UNFILTER_VRAM  23      // Diff8bitUnFilterVram()
#define SWI_NO_DIFF_16BIT_UNFILTER      24      // Diff16bitUnFilter()

#define SWI_NO_SOUND_BIAS_CHANGE        25      // SoundBiasSet()/SoundBiasReset()
#define SWI_NO_SOUND_DRIVER_INIT        26      // SoundDriverInit()
#define SWI_NO_SOUND_DRIVER_MODE        27      // SoundDriverMode()
#define SWI_NO_SOUND_DRIVER_MAIN        28      // SoundDriverMain()
#define SWI_NO_SOUND_DRIVER_VSYNC       29      // SoundDriverVSync()
#define SWI_NO_SOUND_DRIVER_VSYNC_OFF   40      // SoundDriverVSyncOff()
#define SWI_NO_SOUND_DRIVER_VSYNC_ON    41      // SoundDriverVSyncOn()
#define SWI_NO_SOUND_CHANNEL_CLEAR      30      // SoundChannelClear()
#define SWI_NO_MIDI_KEY_2_FREQ          31      // MidiKey2Freq()

#define SWI_NO_MULTI_BOOT               37      // MultiBoot()


/*------------------------------------------------------------------*/
/*                   �\�t�g���Z�b�g                                 */
/*------------------------------------------------------------------*/

extern void SoftReset(u32 ResetFlags);

//�EResetFlags�Ŏw�肵�����W�X�^��RAM�����Z�b�g���ASOFT_RESET_DIRECT_BUF
//  (0x03007ffa)�̒l�ɂ���āA�J�[�g���b�W�܂���CPU�O��RAM�̐擪�A�h���X��
//  ���A���܂��B
//�ECPU�O��RAM�֕��A����ꍇ�� RESET_EX_WRAM_FLAG ���w�肵�Ȃ��ŉ������B
//�E�_�E�����[�h�v���O��������J�[�g���b�W�֕��A����ꍇ�� RESET_REG_SIO_FLAG
//  ���w�肵�Ȃ��ŉ������B
//�ECPU�R�A���W�X�^��CPU����RAM��0x3007e00�`0x3007fff�̗̈��
//  �����I�ɃN���A����܂��B
//
//�E�����F
//    u32 ResetFlags      ���Z�b�g���郌�W�X�^��RAM�̎w��iAgbDefine.h�Q�Ɓj
//
//�E*(u8 *)SOFT_RESET_DIRECT_BUF  ���A��w��
//        0:                      0x08000000�Ԓn�i�J�[�g���b�W�j
//        0�ȊO:                  0x02000000�Ԓn�iCPU�O��RAM�j

/*------------------------------------------------------------------*/
/*                   �\�t�g���Z�b�g�i�J�[�g���b�W���A�j             */
/*------------------------------------------------------------------*/

extern void SoftResetRom(u32 ResetFlags);

//�EResetFlags�Ŏw�肵�����W�X�^��RAM�����Z�b�g���A�J�[�g���b�W��
//  �擪�A�h���X(0x08000000)�֕��A���܂��B
//�E�_�E�����[�h�v���O�������畜�A����ꍇ�A�J�[�g���b�W�v���O������
//  �ʏ�̋N���Ǝ��ʂł���悤�� RESET_REG_SIO_FLAG ���w�肵�Ȃ��ŉ������B
//�ECPU�R�A���W�X�^��CPU����RAM��0x3007e00�`0x3007fff�̗̈��
//  �����I�ɃN���A����܂��B
//
//�E�����F
//    u32 ResetFlags      ���Z�b�g���郌�W�X�^��RAM�̎w��iAgbDefine.h�Q�Ɓj

/*------------------------------------------------------------------*/
/*                   �\�t�g���Z�b�g�iCPU�O��RAM���A�j               */
/*------------------------------------------------------------------*/

extern void SoftResetExram(u32 ResetFlags);

//�EResetFlags�Ŏw�肵�����W�X�^��RAM�����Z�b�g���ACPU�O��RAM��
//  �擪�A�h���X(0x02000000)�֕��A���܂��B
//�ECPU�O��RAM�֕��A���邽�� RESET_EX_WRAM_FLAG �̓N���A����܂��B
//�ECPU�R�A���W�X�^��CPU����RAM��0x3007e00�`0x3007fff�̗̈��
//  �����I�ɃN���A����܂��B
//
//�E�����F
//    u32 ResetFlags      ���Z�b�g���郌�W�X�^��RAM�̎w��iAgbDefine.h�Q�Ɓj

/*------------------------------------------------------------------*/
/*               ���W�X�^���q�`�l ���Z�b�g                          */
/*------------------------------------------------------------------*/

extern void RegisterRamReset(u32 ResetFlags);

//�EResetFlags�Ŏw�肵�����W�X�^��RAM�����Z�b�g���܂��B
//�E�A���ACPU����RAM��0x3007e00�`0x3007fff�̗̈�̓N���A���܂���B
//
//�E�����F
//    u32 ResetFlags      ���Z�b�g���郌�W�X�^��RAM�̎w��iAgbDefine.h�Q�Ɓj

/*------------------------------------------------------------------*/
/*                   �g�`�k�s                                       */
/*------------------------------------------------------------------*/

#define Halt()                   SystemCall(2)

//�ECPU�̂ݒ�~�����܂��B
//�E�Y�����銄�荞�݂����iIE�ɃZ�b�g�j����Ă���
//  ���荞�ݗv���iIF�Z�b�g�j�ɂĕ��A���܂��B

/*------------------------------------------------------------------*/
/*                   �r�s�n�o                                       */
/*------------------------------------------------------------------*/

extern void SoundBiasSet(void);
extern void SoundBiasReset(void);

#define Stop()                                               \
{                                                            \
    SoundBiasReset();                                        \
    SystemCall(3);                                           \
    SoundBiasSet();                                          \
}

//�E�����U���~���܂��B
//�E�Y�����銄�荞�݂����iIE�ɃZ�b�g�j����Ă���ꍇ
//  �L�[�܂��̓J�[�g���b�W�܂���SIO����̊��荞�ݗv�������̔����ɂĕ��A���܂��B
//�E�����U����~���Ă��܂��̂�IF�t���O�̓Z�b�g����܂���B
//�E�K��LCDC��OFF�ɂ��Ď��s���ĉ������B

/*------------------------------------------------------------------*/
/*               ���荞�݃E�F�C�g                                   */
/*------------------------------------------------------------------*/

extern void IntrWait(u8 InitCheckClear, u16 IntrFlags);

//�EIntrFlags�Ŏw�肵�����荞�݂���������܂�Halt��Ԃő҂������܂��B
//�E���荞�ݏ����ɂ�INTR_CHECK_BUF(0x3007ff8)�֊Y������t���O���Z�b�g���ĉ������B
//�E�����̊��荞�݂𕹗p�������AHalt()���J��Ԃ��Ăяo�����ꍇ�Ɣ�ׂ�
//  �V�X�e���R�[���Ăяo���̃I�[�o�[�w�b�h���y�����邱�Ƃ��ł��܂��B
//
//�E�����F
//    u8  InitCheckClear  ���ɊY���t���O���Z�b�g����Ă���ꍇ�ɃN���A���邩�ǂ����̎w��
//    u16 IntrFlags       �E�F�C�g���銄�荞�݂̎w��iAgbDefine.h�Q�Ɓj

/*------------------------------------------------------------------*/
/*               �u�u�����N���荞�݃E�F�C�g                         */
/*------------------------------------------------------------------*/

extern void VBlankIntrWait(void);

//�E�u�u�����N���荞�݂���������܂�Halt��Ԃő҂������܂��B
//�E���荞�ݏ����ɂ�INTR_CHECK_BUF(0x3007ff8)�֊Y������t���O���Z�b�g���ĉ������B
//�E�����̊��荞�݂𕹗p�������AHalt()���J��Ԃ��Ăяo�����ꍇ�Ɣ�ׂ�
//  �V�X�e���R�[���Ăяo���̃I�[�o�[�w�b�h���y�����邱�Ƃ��ł��܂��B
//�EIntrWait(1, V_BLANK_INTR_FLAG)�Ɠ����ł��B

/*------------------------------------------------------------------*/
/*                   �����t������Z�i���j                           */
/*------------------------------------------------------------------*/

extern s32 Div(s32 Number, s32 Denom);      // RedHat�Ёi��Cygnus�Ёj���C�u�����Ή�
extern s32 DivArm(s32 Denom, s32 Number);   // ARM�Ѓ��C�u�����Ή�

//�ENumber/Denom���Z�o���܂��B
//�E���W�X�^�̒l�́Ar0=Number/Denom, r1=Number%Denom, 
//  r3=|Number/Denom|�ŕ��A���܂��B
//
//�E�����F
//    s32 Number          ���q
//    s32 Denom           ����

/*------------------------------------------------------------------*/
/*                   �����t������Z�i�]��j                         */
/*------------------------------------------------------------------*/

extern s32 DivRem(s32 Number, s32 Denom);   // RedHat�Ёi��Cygnus�Ёj���C�u�����Ή�
extern s32 DivRemArm(s32 Denom, s32 Number);// ARM�Ѓ��C�u�����Ή�

//�ENumber%Denom���Z�o���܂��B
//�E���W�X�^�̒l�́Ar0=Number%Denom, r1=Number%Denom, 
//  r3=|Number/Denom|�ŕ��A���܂��B
//
//�E�����F
//    s32 Number          ���q
//    s32 Denom           ����

/*------------------------------------------------------------------*/
/*                   ������                                         */
/*------------------------------------------------------------------*/

extern u16 Sqrt(u32 X);

//�E���������Z�o���܂��B
//�E���x��ǂ����邽�߂Ɉ���X��2�̔{���������V�t�g���ēn���A
//  �߂�l���V�t�g���Č����킹���s�Ȃ��ĉ������B

/*------------------------------------------------------------------*/
/*                   �t����                                         */
/*------------------------------------------------------------------*/

extern s16 ArcTan(s16 Tan);

//�E�t���ڒl���Z�o���܂��B
//�E�߂�l��-��/2���Ɓ���/2��0xc000�`0x4000�͈̔͂ŕԂ��܂��B
//�E�A���A�Ɓ�-��/4,��/4���Ƃ͈̔͂͐��x�ɖ�肪����܂��B
//
//�E�����F
//    s16 Tan
//        ����:1
//        ������:1
//        ������:14

/*------------------------------------------------------------------*/
/*                   �t���ڂQ                                       */
/*------------------------------------------------------------------*/

extern u16 ArcTan2(s16 X, s16 Y);

//�E�␳������̋t���ڒl���Z�o���܂��B
//�E�ʏ�͂�������g�p���ĉ������B
//�E�߂�l��0���Ɓ�2�΂�0�`0xffff�͈̔͂ŕԂ��܂��B
//
//�E�����F
//    s16 X, Y
//        ����:1
//        ������:1
//        ������:14

/*------------------------------------------------------------------*/
/*                   �b�o�t�Z�b�g                                   */
/*------------------------------------------------------------------*/

extern void CpuSet(const void *Srcp, void *Destp, u32 DmaCntData);

//�EDmaSet�}�N���݊��̃p�����[�^��RAM�N���A�܂��̓R�s�[���܂��B
//�E32bit�]���ł͋����I��4Byte���E�ɂăA�N�Z�X����܂����A
//  16bit�]���ł͈�����2Byte���E�֍��킹�ēn���K�v������܂��B
//
//�E�����F
//    void *Srcp          �\�[�X�A�h���X
//    void *Destp         �f�X�e�B�l�[�V�����A�h���X
//    u32 DmaCntData      DMA_SRC_FIX�^DMA_32BIT_BUS�^DMA_COUNT_MASK�̂ݗL��
//          DMA_SRC_FIX(  0, 1) = (�\�[�X�A�h���X�E�C���N�������g, �\�[�X�A�h���X�Œ�)
//          DMA_32BIT_BUS(0, 1) = (16bit�]��, 32bit�]��)
//          DMA_COUNT_MASK & DmaCntData = �]����
//
//�E��ʃ}�N���F
//  CpuClear, CpuArrayClear, CpuCopy, CpuArrayCopy

/*------------------------------------------------------------------*/
/*                   �b�o�t�����Z�b�g                               */
/*------------------------------------------------------------------*/

extern void CpuFastSet(const void *Srcp, void *Destp, u32 DmaCntData);

//�EDmaSet�}�N���݊��̃p�����[�^�ō�����RAM�N���A�܂��̓R�s�[���܂��B
//�E�A���A32Byte�i8Word�j�P�ʂ�32bit�]���ł��B
//�E4Byte���E�ȊO�ň�����^���Ă������I��4Byte���E�ɂăA�N�Z�X����܂��B
//
//�E�����F
//    void *Srcp          �\�[�X�A�h���X
//    void *Destp         �f�X�e�B�l�[�V�����A�h���X
//    u32 DmaCntData      DMA_SRC_FIX�^DMA_COUNT_MASK�̂ݗL��
//          DMA_SRC_FIX(0, 1) = (�\�[�X�A�h���X�E�C���N�������g, �\�[�X�A�h���X�Œ�)
//          DMA_COUNT_MASK & DmaCntData = �]����
//
//�E��ʃ}�N���F
//  CpuFastClear, CpuFastArrayClear, CpuFastCopy, CpuFastArrayCopy

/*------------------------------------------------------------------*/
/*                   �a�f�A�t�B���ϊ� �Z�b�g                        */
/*------------------------------------------------------------------*/

extern void BgAffineSet(const BgAffineSrcData *Srcp, BgAffineDestData *Destp, s32 Num);

//�EBG�̃A�t�B���p�����[�^���Z�o���A�Z�b�g���܂��B
//
//�E�����F
//  BgAffineSrcData  *Srcp      �\�[�X�A�h���X
//  BgAffineDestData *Destp     �f�X�e�B�l�[�V�����A�h���X
//              s32   Num       �v�Z��
//
//�EBgAffineSrcData�\����
//    s32 SrcCenterX            ���f�[�^���S���W�i�����_�ȉ�8bit�j
//    s32 SrcCenterY
//    s16 DispCenterX           �\�����S���W
//    s16 DispCenterY
//    s16 RatioX                �g��k�����i�����_�ȉ�8bit�j
//    s16 RatioY
//    u16 Theta                 ��]�p�i�����_�ȉ�8bit�j
//                                  �L���͈�    0 - 0xffff
//�EBgAffineDestData�\����
//    s16 H_DiffX               ���C�������w���W����
//    s16 V_DiffX                 ���������w���W����
//    s16 H_DiffY               ���C�������x���W����
//    s16 V_DiffY                 ���������x���W����
//    s32 StartX                �X�^�[�g�w���W
//    s32 StartY                �X�^�[�g�x���W

/*------------------------------------------------------------------*/
/*                   �n�a�i�A�t�B���ϊ� �Z�b�g                      */
/*------------------------------------------------------------------*/

extern void ObjAffineSet(const ObjAffineSrcData *Srcp, void *Destp, s32 Num, s32 Offset);

//�EOBJ�̃A�t�B���p�����[�^���Z�o���A�Z�b�g���܂��B
//
//�E�����F
//  ObjAffineSrcData *Srcp      �\�[�X�A�h���X
//              void *Destp     �f�X�e�B�l�[�V�����A�h���X
//              s32   Num       �v�Z��
//              s32   Offset    �p�����[�^�A�h���X�̃I�t�Z�b�g�E�o�C�g���i�ʏ� 2|8�j
//                              OAM�ɒ��ڃZ�b�g����ꍇ��"8"���w��
//
//�EObjAffineSrcData�\����
//    s16 RatioX                �g��k�����i�����_�ȉ�8bit�j
//    s16 RatioY
//    u16 Theta                 ��]�p�i�����_�ȉ�8bit�j
//                                  �L���͈�    0 - 0xffff
//�EObjAffineDestData�\����
//    s16 H_DiffX               ���C�������w���W����
//    s16 V_DiffX                 ���������w���W����
//    s16 H_DiffY               ���C�������x���W����
//    s16 V_DiffY                 ���������x���W����

/*------------------------------------------------------------------*/
/*                       Bit���k�f�[�^�W�J                          */
/*------------------------------------------------------------------*/

extern void BitUnPack(const void *Srcp, void *Destp, BitUnPackParam *BitUnPackParamp);

//�E0�Œ��bit���l�߂��f�[�^��W�J���܂��B
//�E�f�X�e�B�l�[�V�����A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//            void *Srcp    �\�[�X�A�h���X
//            void *Destp   �f�X�e�B�l�[�V�����A�h���X
//  BitUnPackParam *Paramp  BitUnPackParam�\���̃f�[�^�̃A�h���X
//
//�EBitUnPackParam�\����
//    u16 SrcNum              �\�[�X�f�[�^�E�o�C�g��
//    u8  SrcBitNum           �P�\�[�X�f�[�^�E�r�b�g��
//    u8  DestBitNum          �P�f�X�e�B�l�[�V�����f�[�^�E�r�b�g��
//    u32 DestOffset:31       �\�[�X�f�[�^�ɉ��Z����I�t�Z�b�g��
//        DestOffset0_On:1    �O�̃f�[�^�ɃI�t�Z�b�g�����Z���邩�ۂ��̃t���O

/*------------------------------------------------------------------*/
/*                   �k�y�V�V���k�f�[�^�W�J�i�����[�N�q�`�l�j       */
/*------------------------------------------------------------------*/

extern void LZ77UnCompWram(const void *Srcp, void *Destp);

//�ELZ77���k�f�[�^��W�J���A8bit�P�ʂŏ������݂܂��B
//�EVRAM�ɒ��ړW�J���邱�Ƃ͂ł��܂���B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *Srcp              �\�[�X�A�h���X
//    void *Destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �\��
//        CompType:4          ���k�^�C�v�i = 1�j
//        DestSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�t���O�f�[�^�t�H�[�}�b�g
//    u8  Flags               ���k�^�����k�t���O
//                            �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j
//�E�R�[�h�f�[�^�t�H�[�}�b�g�iBig Endian�j
//    u16 Length:4            �W�J�f�[�^�� - 3�i��v��3Byte�ȏ㎞�݈̂��k�j
//        Offset:12           ��v�f�[�^�I�t�Z�b�g - 1

/*------------------------------------------------------------------*/
/*                   �k�y�V�V���k�f�[�^�W�J�i���u�q�`�l�j           */
/*------------------------------------------------------------------*/

extern void LZ77UnCompVram(const void *Srcp, void *Destp);

//�ELZ77���k�f�[�^��W�J���A16bit�P�ʂŏ������݂܂��B
//�E���[�NRAM�ɂ��W�J�ł��܂����ALZ77UnCompWram()���ᑬ�ł��B
//�E���k�f�[�^�͈�v�������2Byte�ȑO��茟���������̂ɂ��ĉ������B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *Srcp              �\�[�X�A�h���X
//    void *Destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �\��
//        CompType:4          ���k�^�C�v�i = 1�j
//        DestSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�t���O�f�[�^�t�H�[�}�b�g
//    u8  Flags               ���k�^�����k�t���O
//                            �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j
//�E�R�[�h�f�[�^�t�H�[�}�b�g�iBig Endian�j
//    u16 Length:4            �W�J�f�[�^�� - 3�i��v��3Byte�ȏ㎞�݈̂��k�j
//        Offset:12           ��v�f�[�^�I�t�Z�b�g�i >= 2�j - 1

/*------------------------------------------------------------------*/
/*                   �n�t�}�����k�f�[�^�W�J                         */
/*------------------------------------------------------------------*/

extern void HuffUnComp(const void *Srcp, void *Destp);

//�E�n�t�}�����k�f�[�^��W�J���A32bit�P�ʂŏ������݂܂��B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *Srcp              �\�[�X�A�h���X
//    void *Destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 BitSize:4           �P�f�[�^�E�r�b�g�T�C�Y�i�ʏ� 4|8�j
//        CompType:4          ���k�^�C�v�i = 2�j
//        DestSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�c���[�e�[�u��
//    u8           TreeSize        �c���[�e�[�u���T�C�Y/2 - 1
//    TreeNodeData RootNode        ���[�g�m�[�h
//
//    TreeNodeData LeftNode        ���[�g���m�[�h
//    TreeNodeData RightNode       ���[�g�E�m�[�h
//
//    TreeNodeData LeftleftNode    �����m�[�h
//    TreeNodeData leftRightNode   ���E�m�[�h
//
//    TreeNodeData RightLeftNode   �E���m�[�h
//    TreeNodeData RightRightNode  �E�E�m�[�h
//
//            �E
//            �E
//
//  ���̌�Ɉ��k�f�[�^�{��
//
//�ETreeNodeData�\����
//    u8  NextNodeOffset:6    ���m�[�h�f�[�^�ւ̃I�t�Z�b�g - 1�i2Byte�P�ʁj
//        RightEndFlag:1      �E�m�[�h�I���t���O
//        LeftEndFlag:1       ���m�[�h�I���t���O
//                            �I���t���O���Z�b�g����Ă���ꍇ
//                            ���m�[�h�Ƀf�[�^������

/*------------------------------------------------------------------*/
/*                   ���������O�X���k�f�[�^�W�J�i�����[�N�q�`�l�j   */
/*------------------------------------------------------------------*/

extern void RLUnCompWram(const void *Srcp, void *Destp);

//�E���������O�X���k�f�[�^��W�J���A8bit�P�ʂŏ������݂܂��B
//�EVRAM�ɒ��ړW�J���邱�Ƃ͂ł��܂���B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *Srcp              �\�[�X�A�h���X
//    void *Destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �\��
//        CompType:4          ���k�^�C�v�i = 3�j
//        DestSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�t���O�f�[�^�t�H�[�}�b�g
//    u8  Length:7            �W�J�f�[�^�� - 1�i�����k���j
//                            �W�J�f�[�^�� - 3�i�A����3Byte�ȏ㎞�݈̂��k�j
//        Flag:1              �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j

/*------------------------------------------------------------------*/
/*                   ���������O�X���k�f�[�^�W�J�i���u�q�`�l�j       */
/*------------------------------------------------------------------*/

extern void RLUnCompVram(const void *Srcp, void *Destp);

//�E���������O�X���k�f�[�^��W�J���A16bit�P�ʂŏ������݂܂��B
//�E���[�NRAM�ɂ��W�J�ł��܂����ARLUnCompWram()���ᑬ�ł��B
//�E���k�f�[�^�̃T�C�Y��4�̔{���ɂȂ�Ȃ������ꍇ��
//  �o���邾��0�ŋl�߂Ē������ĉ������B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *Srcp              �\�[�X�A�h���X
//    void *Destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 :4                  �\��
//        CompType:4          ���k�^�C�v�i = 3�j
//        DestSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�t���O�f�[�^�t�H�[�}�b�g
//    u8  Length:7            �W�J�f�[�^�� - 1�i�����k���j
//                            �W�J�f�[�^�� - 3�i�A����3Byte�ȏ㎞�݈̂��k�j
//        Flag:1              �i0, 1�j = �i�����k�f�[�^, ���k�f�[�^�j

/*------------------------------------------------------------------*/
/*                   �����t�B���^�W�J�i8Bit�����[�N�q�`�l�j         */
/*------------------------------------------------------------------*/

extern void Diff8bitUnFilterWram(const void *Srcp, void *Destp);

//�E8bit�����t�B���^���H�f�[�^��W�J���A8bit�P�ʂŏ������݂܂��B
//�EVRAM�ɒ��ړW�J���邱�Ƃ͂ł��܂���B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *Srcp              �\�[�X�A�h���X
//    void *Destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 ByteSize:4          �P�f�[�^�E�o�C�g�T�C�Y�i = 1�j
//        FiltType:4          �t�B���^�^�C�v�i = 8�j
//        DestSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�f�[�^�t�H�[�}�b�g
//    u8  Origin              ���f�[�^
//    u8  Diff                �����f�[�^
//         �E
//         �E

/*------------------------------------------------------------------*/
/*                   �����t�B���^�W�J�i8Bit���u�q�`�l�j             */
/*------------------------------------------------------------------*/

extern void Diff8bitUnFilterVram(const void *Srcp, void *Destp);

//�E8bit�����t�B���^���H�f�[�^��W�J���A16bit�P�ʂŏ������݂܂��B
//�E���[�NRAM�ɂ��W�J�ł��܂����ADiff8bitUnFilterVram()���ᑬ�ł��B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *Srcp              �\�[�X�A�h���X
//    void *Destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 ByteSize:4          �P�f�[�^�E�o�C�g�T�C�Y�i = 1�j
//        FilterType:4        �t�B���^�^�C�v�i = 8�j
//        DestSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�f�[�^�t�H�[�}�b�g
//    u8  Origin              ���f�[�^
//    u8  Diff                �����f�[�^
//         �E
//         �E

/*------------------------------------------------------------------*/
/*                   �����t�B���^�W�J�i16Bit�j                      */
/*------------------------------------------------------------------*/

extern void Diff16bitUnFilter(const void *Srcp, void *Destp);

//�E16bit�����t�B���^���H�f�[�^��W�J���A16bit�P�ʂŏ������݂܂��B
//�E�\�[�X�A�h���X��4Byte���E�ɍ��킹�ĉ������B
//
//�E�����F
//    void *Srcp              �\�[�X�A�h���X
//    void *Destp             �f�X�e�B�l�[�V�����A�h���X
//
//�E�f�[�^�w�b�_
//    u32 ByteSize:4          �P�f�[�^�E�o�C�g�T�C�Y�i = 2�j
//        FilterType:4        �t�B���^�^�C�v�i = 8�j
//        DestSize:24         �W�J��̃f�[�^�T�C�Y
//
//�E�f�[�^�t�H�[�}�b�g
//    u16 Origin              ���f�[�^
//    u16 Diff                �����f�[�^
//         �E
//         �E

/*------------------------------------------------------------------*/
/*              �T�E���h�a�h�`�r �Z�b�g                             */
/*------------------------------------------------------------------*/

extern void SoundBiasSet(void);

//�E�T�E���hBIAS��0���璆�Ԓl�i0x200�j�ֈڍs���܂��B

/*------------------------------------------------------------------*/
/*              �T�E���h�a�h�`�r ���Z�b�g                           */
/*------------------------------------------------------------------*/

extern void SoundBiasReset(void);

//�E�T�E���hBIAS�𒆊Ԓl�i0x200�j����0�ֈڍs���܂��B

/*------------------------------------------------------------------*/
/*              �T�E���h�h���C�o ������                             */
/*------------------------------------------------------------------*/

extern void SoundDriverInit(SoundArea *sa);

//  �T�E���h�h���C�o�����������܂��B
//  �Q�[���̋N�����Ɉ�x�����Ă�ł��������B
//  ���̍ہA��Ɨ̈�ł��� sa �́A���炩���ߊm�ۂ��Ă���
//  �K�v������܂��B���A���Ƃ���Ɨ̈��ʂɗp�ӂ��Ă��A
//  �{�h���C�o�𑽏d�Ɏ��s���邱�Ƃ͏o���܂���B

/*------------------------------------------------------------------*/
/*              �T�E���h�h���C�o ���샂�[�h                         */
/*------------------------------------------------------------------*/

extern void SoundDriverMode(u32 mode);

//  �T�E���h�h���C�o�̓��샂�[�h��ݒ肵�܂��B
//  
//  ���_�C���N�g�T�E���h�̃��o�[�u (�����l 0)
//      mode = SOUND_MODE_REVERB_SET + (���o�[�u�l 0�`127);
//  
//  ���_�C���N�g�T�E���h�̓��������� (�����l 8)
//      mode = (���������� 1�`12) << SOUND_MODE_MAXCHN_SHIFT;
//  
//  ���_�C���N�g�T�E���h�̃}�X�^�[�{�����[�� (�����l 15)
//      mode = (�{�����[�� 1�`15) << SOUND_MODE_MASVOL_SHIFT;
//  
//  ���_�C���N�g�T�E���h�̍Đ����g�� (�����l 13379Hz)
//      mode = SOUND_MODE_FREQ_?????;
//             �� AgbSound.h �Œ�`����Ă���l�B(�P�Q���)
//  
//  ���ŏI�I�Ȃc�^�`�R���o�[�^�̃r�b�g�� (�����l 8bit)
//      mode = SOUND_MODE_DA_BIT_?;
//                            �� 9�`6
//  
//  ���ȏ�̒l�́AOR ���Ĉ�x�ɐݒ肷�邱�Ƃ��ł��܂��B

/*------------------------------------------------------------------*/
/*              �T�E���h�h���C�o ���C��                             */
/*------------------------------------------------------------------*/

extern void SoundDriverMain(void);

//  �T�E���h�h���C�o�̃��C���B
//  1/60�b���ɌĂ�ł��������B�����̗���Ƃ��ẮA
//  V-Blank�����݂̒���Ɍ�q�� SoundDriverVSync() ���ĂсA
//  ���̌� BG �� OBJ �̏������s���Ă���{���[�`����
//  �ĂԂ��ƂɂȂ�܂��B

/*------------------------------------------------------------------*/
/*              �T�E���h�h���C�o �c�l�`�ăZ�b�g                     */
/*------------------------------------------------------------------*/

extern void SoundDriverVSync(void);

//  �T�E���h�c�l�`���ăZ�b�g������ɒZ���V�X�e���R�[���ł��B
//  �^�C�~���O���V�r�A�ł��̂ŁA1/60�b���� V-Blank ��
//  �����݂��������w����x�ɌĂ�ł��������B

/*------------------------------------------------------------------*/
/*              �T�E���h�`�����l�� �N���A                           */
/*------------------------------------------------------------------*/

extern void SoundChannelClear(void);

//  �_�C���N�g�T�E���h�̑S�`�����l�����N���A���A�����~�߂܂��B
//  �Ȃ����̊֐��́A����T�E���h�h���C�o�@�\���g�����郉�C�u������
//  �g���񂾍ۂɐ���ɓ��삵�Ȃ����ꂪ����܂��̂ŁA���̏ꍇ�͎g�p
//  ���Ȃ��ŉ������B

/*------------------------------------------------------------------*/
/*                 �T�E���h�c�l�`�̒�~                             */
/*------------------------------------------------------------------*/

extern void SoundDriverVSyncOff(void);

//  ���C���v���O�����̓s������ V-Blank�����݂��~���铙�A
//  1/60�b���� SoundDriverVSync() ���ĂׂȂ��Ȃ����ꍇ�ɂ́A
//  ���̊֐����g���ăT�E���h�c�l�`���~����K�v������܂��B
//  �����Ȃ��ƁA�o�b�t�@���I�[�o�[���Ă��c�l�`���~�܂炸�ɁA
//  �Ђǂ��m�C�Y���������܂��B

/*------------------------------------------------------------------*/
/*                 �T�E���h�c�l�`�̍ĊJ                             */
/*------------------------------------------------------------------*/

extern void SoundDriverVSyncOn(void);

//  ��L�� SoundDriverVSyncOff() �ɂĒ�~�����T�E���h�c�l�`��
//  �ĊJ������֐��ł��B���̊֐����R�[��������A2/60�b�ȓ���
//  V-Blank �����݂����� SoundDriverVSync() ���Ă΂��悤��
//  ���Ă��������B

/*------------------------------------------------------------------*/
/*               �l�h�c�h�L�[�ɂ�鉹���v�Z                         */
/*------------------------------------------------------------------*/

extern u32  MidiKey2Freq(WaveData *wa, u8 mk, u8 fp);

//  �g�`�f�[�^ wa ���A����(MIDI KEY) mk ����є������l fp �Ŗ炷���A
//  ((SoundArea)sa).vchn[x].fr �ɑ������l���v�Z���܂��B


/*------------------------------------------------------------------*/
/*                   �}���`�v���C�u�[�g                             */
/*------------------------------------------------------------------*/

extern int MultiBoot(MultiBootParam *mp);

//�E�}���`�v���C�u�[�g�̃T�[�o�p���C�������ł��B
//�E���炩���߁A�ڑ�����Ă��邷�ׂẴN���C�A���gAGB�Ƃ̊ԂŁA
//  ����̔F���葱�����s���Ă����Ȃ���΂Ȃ�܂���B


#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif /* _AGB_SYSTEM_CALL_H */
