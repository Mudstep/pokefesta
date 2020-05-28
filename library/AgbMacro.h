/********************************************************************/
/*          AgbMacro.h                                              */
/*            AGB�W���}�N���֐�                                     */
/*                                                                  */
/*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               */
/********************************************************************/
#ifndef _AGB_MACRO_H
#define _AGB_MACRO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <AgbTypes.h>
#include <AgbSystemCall.h>


/*------------------------------------------------------------------*/
/*                      �f�[�^�ǂݍ���                              */
/*------------------------------------------------------------------*/

#define DataRead(Readp, Bit)    (*(vu##Bit *)(Readp))

/*------------------------------------------------------------------*/
/*                      �f�[�^��������                              */
/*------------------------------------------------------------------*/

#define DataWrite(Writep, Data, Bit)                    \
                                                        \
    *(vu##Bit *)(Writep) = (Data)

/*------------------------------------------------------------------*/
/*                      �f�[�^ �R�s�[                               */
/*------------------------------------------------------------------*/

#define DataCopy(Readp, Writep, Bit)                    \
                                                        \
    *(vu##Bit *)(Writep) = *(vu##Bit *)(Readp)


/*------------------------------------------------------------------*/
/*                  ���荞�݃A�h���X �Z�b�g                         */
/*------------------------------------------------------------------*/

#define IntrAddrSet(Intrp)                              \
                                                        \
    *(vu32 *)INTR_VECTOR_BUF = (vu32 )Intrp

//�E���荞�ݏ����̃A�h���X���Z�b�g���܂��B

/*------------------------------------------------------------------*/
/*                      �c�l�` �Z�b�g                               */
/*------------------------------------------------------------------*/

//#define SIMULATOR
#ifndef SIMULATOR
#ifndef CODE32
#define DmaSet(DmaNo, Srcp, Destp, DmaCntData)          \
{                                                       \
    vu32 *(DmaCntp) = (vu32 *)REG_DMA##DmaNo;           \
    DmaCntp[0] = (vu32 )(Srcp);                         \
    DmaCntp[1] = (vu32 )(Destp);                        \
    DmaCntp[2] = (vu32 )(DmaCntData);                   \
    DmaCntp[2];                                         \
}
#else
#define DmaSet(DmaNo, Srcp, Destp, DmaCntData)                   \
{                                                                \
    vu32 *(RegBasep) = (vu32 *)REG_BASE;                         \
    RegBasep[OFFSET_REG_DMA##DmaNo##SAD/4] = (vu32 )(Srcp);      \
    RegBasep[OFFSET_REG_DMA##DmaNo##DAD/4] = (vu32 )(Destp);     \
    RegBasep[OFFSET_REG_DMA##DmaNo##CNT/4] = (vu32 )(DmaCntData);\
    RegBasep[OFFSET_REG_DMA##DmaNo##CNT/4];                      \
}
#endif
#else
#define DmaSet(DmaNo, Srcp, Destp, DmaCntData)                   \
{                                                                \
    int    i;                                                    \
    for (i=0; i<(DmaCntData & 0x1ffff); i++)                     \
        if ((DmaCntData) & DMA_SRC_FIX) {                        \
            if ((DmaCntData) & DMA_32BIT_BUS)                    \
                    ((vu32 *)(Destp))[i] = ((vu32 *)(Srcp))[0];  \
            else    ((vu16 *)(Destp))[i] = ((vu16 *)(Srcp))[0];  \
         } else {                                                \
            if ((DmaCntData) & DMA_32BIT_BUS)                    \
                ((vu32 *)(Destp))[i] = ((vu32 *)(Srcp))[i];      \
            else    ((vu16 *)(Destp))[i] = ((vu16 *)(Srcp))[i];  \
        }                                                        \
}
#endif

//�EDMA�R���g���[���Ƀp�����[�^���Z�b�g���܂��B
//�ESIMULATER���`�����CPU�ŃV�~�����[�g���܂��B
//  GDB�Ńf�o�b�O����ꍇ�ȂǂɗL���ł��B
//�E�Ō��DMA�N���҂��̂��߂�"LDR"���߂��}������܂��B
//
//�E�����F
//  DmaNo       DMA�ԍ�
//  Srcp        �\�[�X�A�h���X
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  DmaCntData  �p�����[�^�f�[�^
//
//��CPU����RAM��̃v���O�����ɂ�DMA���N�������
//  ���̎��̖��߂���Ɏ��s����܂��B
//  ����āADMA����ɓ]�����CPU�ɂĕύX���悤�Ƃ����ꍇ�A
//  �ǂݍ��݁^�����߂��̊Ԃ�DMA���N������Ă��܂��܂��̂ŁA
//  DMA�̓]����̃f�[�^���Ӑ}���Ă��Ȃ��l�ɂȂ�ꍇ������܂��B
//  ���̏ꍇ�ɂ�DmaWait()�𒼌�ɑ}�����āADMA���I��������
//  �ǂ������`�F�b�N����Ƒ����R�[�h�ւ̉e�����m���ɉ���ł��܂��B


/*------------------------------------------------------------------*/
/*                      �c�l�` �N���A                               */
/*------------------------------------------------------------------*/

#define DmaClear(DmaNo, Data, Destp, Size, Bit)             \
{                                                           \
    vu##Bit Tmp = (vu##Bit )(Data);                         \
    DmaSet(DmaNo, &Tmp, Destp, (                            \
        DMA_ENABLE         | DMA_TIMMING_IMM |              \
        DMA_SRC_FIX        | DMA_DEST_INC    |              \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))));            \
}

#define DmaClearIf(DmaNo, Data, Destp, Size, Bit)           \
{                                                           \
    vu##Bit Tmp = (vu##Bit )(Data);                         \
    DmaSet(DmaNo, &Tmp, Destp, (                            \
        DMA_ENABLE         | DMA_TIMMING_IMM |              \
        DMA_IF_ENABLE      |                                \
        DMA_SRC_FIX        | DMA_DEST_INC    |              \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))));            \
}

#define DmaArrayClear(  DmaNo, Data, Destp, Bit)            \
        DmaClear(       DmaNo, Data, Destp, sizeof(Destp), Bit)

#define DmaArrayClearIf(DmaNo, Data, Destp, Bit)            \
        DmaClearIf(     DmaNo, Data, Destp, sizeof(Destp), Bit)

//�EDMA��RAM�N���A���܂��B
//�E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
//�EDmaClearIf�^DmaArrayClearIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EDmaArrayClear�^DmaArrayClearIf�̓f�X�e�B�l�[�V�����z��S�̂��N���A���܂��B
//
//�E�����F
//  DmaNo       DMA�ԍ�
//  Data        �N���A�f�[�^
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  Size        �N���A�o�C�g��
//  Bit         �]���r�b�g���i16|32�j
//
//
//��CPU����RAM��̃v���O�����ɂ�DMA���N�������
//  ���̎��̖��߂���Ɏ��s����܂��B
//  ����āADMA����ɓ]�����CPU�ɂĕύX���悤�Ƃ����ꍇ�A
//  �ǂݍ��݁^�����߂��̊Ԃ�DMA���N������Ă��܂��܂��̂ŁA
//  DMA�̓]����̃f�[�^���Ӑ}���Ă��Ȃ��l�ɂȂ�ꍇ������܂��B
//  ���̏ꍇ�ɂ�DmaWait()�𒼌�ɑ}�����āADMA���I��������
//  �ǂ������`�F�b�N����Ƒ����R�[�h�ւ̉e�����m���ɉ���ł��܂��B

/*------------------------------------------------------------------*/
/*                      �c�l�` �R�s�[                               */
/*------------------------------------------------------------------*/

#define DmaCopy(DmaNo, Srcp, Destp, Size, Bit)              \
                                                            \
    DmaSet(DmaNo, Srcp, Destp,  (                           \
        DMA_ENABLE         | DMA_TIMMING_IMM |              \
        DMA_SRC_INC        | DMA_DEST_INC    |              \
        DMA_##Bit##BIT_BUS | ((Size)/((Bit)/8))))

#define DmaCopyIf(DmaNo, Srcp, Destp, Size, Bit)            \
                                                            \
    DmaSet(DmaNo, Srcp, Destp,  (                           \
        DMA_ENABLE         | DMA_TIMMING_IMM |              \
        DMA_IF_ENABLE      |                                \
        DMA_SRC_INC        | DMA_DEST_INC    |              \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))))

#define DmaArrayCopy(  DmaNo, Srcp, Destp, Bit)             \
        DmaCopy(       DmaNo, Srcp, Destp, sizeof(Srcp), Bit)

#define DmaArrayCopyIf(DmaNo, Srcp, Destp, Bit)             \
        DmaCopyIf(     DmaNo, Srcp, Destp, sizeof(Srcp), Bit)

//�EDMA�ŃR�s�[���܂��B
//�EDmaCopyIf�^DmaArrayCopyIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EDmaArrayCopy�^DmaArrayCopyIf�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  DmaNo       DMA�ԍ�
//  Srcp        �\�[�X�A�h���X
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  Size        �]���o�C�g��
//  Bit         �]���r�b�g���i16|32�j
//
//
//��CPU����RAM��̃v���O�����ɂ�DMA���N�������
//  ���̎��̖��߂���Ɏ��s����܂��B
//  ����āADMA����ɓ]�����CPU�ɂĕύX���悤�Ƃ����ꍇ�A
//  �ǂݍ��݁^�����߂��̊Ԃ�DMA���N������Ă��܂��܂��̂ŁA
//  DMA�̓]����̃f�[�^���Ӑ}���Ă��Ȃ��l�ɂȂ�ꍇ������܂��B
//  ���̏ꍇ�ɂ�DmaWait()�𒼌�ɑ}�����āADMA���I��������
//  �ǂ������`�F�b�N����Ƒ����R�[�h�ւ̉e�����m���ɉ���ł��܂��B

/*------------------------------------------------------------------*/
/*                  �g�u�����N�c�l�` �R�s�[                         */
/*------------------------------------------------------------------*/

#define H_DmaCopy(DmaNo, Srcp, Destp, Size, Bit)            \
                                                            \
    DmaSet(DmaNo, Srcp, Destp,  (                           \
        DMA_ENABLE         | DMA_TIMMING_H_BLANK |          \
        DMA_SRC_INC        | DMA_DEST_RELOAD     |          \
        DMA_CONTINUOUS_ON  |                                \
        DMA_##Bit##BIT_BUS | ((Size)/((Bit)/8))))

#define H_DmaCopyIf(DmaNo, Srcp, Destp, Size, Bit)          \
                                                            \
    DmaSet(DmaNo, Srcp, Destp,  (                           \
        DMA_ENABLE         | DMA_TIMMING_H_BLANK |          \
        DMA_IF_ENABLE      |                                \
        DMA_SRC_INC        | DMA_DEST_RELOAD     |          \
        DMA_CONTINUOUS_ON  |                                \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))))

#define H_DmaArrayCopy(  DmaNo, Srcp, Destp, Bit)           \
        H_DmaCopy(       DmaNo, Srcp, Destp, sizeof(Srcp), Bit)

#define H_DmaArrayCopyIf(DmaNo, Srcp, Destp, Bit)           \
        H_DmaCopyIf(     DmaNo, Srcp, Destp, sizeof(Srcp), Bit)

//�EH�u�����N�ɓ�������DMA�ŃR�s�[���܂��B
//�EH_DmaCopyIf�^H_DmaArrayCopyIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EH_DmaArrayCopy�^H_DmaArrayCopyIf�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  DmaNo       DMA�ԍ�
//  Srcp        �\�[�X�A�h���X
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  Size        �]���o�C�g��
//  Bit         �]���r�b�g���i16|32�j

/*------------------------------------------------------------------*/
/*                  �u�u�����N�c�l�` �R�s�[                         */
/*------------------------------------------------------------------*/

#define V_DmaCopy(DmaNo, Srcp, Destp, Size, Bit)            \
                                                            \
    DmaSet(DmaNo, Srcp, Destp,  (                           \
        DMA_ENABLE         | DMA_TIMMING_V_BLANK |          \
        DMA_SRC_INC        | DMA_DEST_INC        |          \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))))

#define V_DmaCopyIf(DmaNo, Srcp, Destp, Size, Bit)          \
                                                            \
    DmaSet(DmaNo, Srcp, Destp,  (                           \
        DMA_ENABLE         | DMA_TIMMING_V_BLANK |          \
        DMA_IF_ENABLE      |                                \
        DMA_SRC_INC        | DMA_DEST_INC        |          \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))))

#define V_DmaArrayCopy(  DmaNo, Srcp, Destp, Bit)           \
        V_DmaCopy(       DmaNo, Srcp, Destp, sizeof(Srcp), Bit)

#define V_DmaArrayCopyIf(DmaNo, Srcp, Destp, Bit)           \
        V_DmaCopyIf(     DmaNo, Srcp, Destp, sizeof(Srcp), Bit)

//�EV�u�����N�ɓ�������DMA�ŃR�s�[���܂��B
//�EV_DmaCopyIf�^V_DmaArrayCopyIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//�EV_DmaArrayCopy�^V_DmaArrayCopyIf�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  DmaNo       DMA�ԍ�
//  Srcp        �\�[�X�A�h���X
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  Size        �]���o�C�g��
//  Bit         �]���r�b�g���i16|32�j

/*------------------------------------------------------------------*/
/*                  �c�l�` �T�E���h�e�h�e�n�]��                     */
/*------------------------------------------------------------------*/

#if 1
#define DmaSound(DmaNo, Srcp, FifoName)                             \
{                                                                   \
    u8    i;                                                        \
    *((u8 *)(REG_SOUNDCNT + 3)) |= SOUND_##FifoName##_FIFO_RESET;   \
    for (i=0; i<8; i++)                                             \
        *(u32 *)REG_FIFO_##FifoName = ((u32 *)Srcp)[i];             \
    DmaSet( DmaNo, (u32 *)Srcp + 8, REG_FIFO_##FifoName, (          \
        DMA_ENABLE         | DMA_TIMMING_SOUND |                    \
        DMA_CONTINUOUS_ON  |                                        \
        DMA_SRC_INC        | DMA_DEST_FIX      |                    \
        DMA_32BIT_BUS      | (4*4)/(32/8)))                         \
}

#define DmaSoundIf(DmaNo, Srcp, FifoName)                           \
{                                                                   \
    u8    i;                                                        \
    *((u8 *)(REG_SOUNDCNT + 3)) |= SOUND_##FifoName##_FIFO_RESET;   \
    for (i=0; i<8; i++)                                             \
        *(u32 *)REG_FIFO_##FifoName = ((u32 *)Srcp)[i];             \
    DmaSet( DmaNo, (u32 *)Srcp + 8, REG_FIFO_##FifoName, (          \
        DMA_ENABLE         | DMA_TIMMING_SOUND |                    \
        DMA_IF_ENABLE      |                                        \
        DMA_CONTINUOUS_ON  |                                        \
        DMA_SRC_INC        | DMA_DEST_FIX      |                    \
        DMA_32BIT_BUS      | (4*4)/(32/8)))                         \
}
#else
#define DmaSound(DmaNo, Srcp, FifoName)                             \
                                                                    \
    DmaSet( DmaNo, Srcp, REG_FIFO_##FifoName, (                     \
        DMA_ENABLE         | DMA_TIMMING_SOUND |                    \
        DMA_CONTINUOUS_ON  |                                        \
        DMA_SRC_INC        | DMA_DEST_FIX      |                    \
        DMA_32BIT_BUS      | (4*4)/(32/8)))

#define DmaSoundIf(DmaNo, Srcp, FifoName)                           \
                                                                    \
    DmaSet(DmaNo, Srcp, REG_FIFO_##FifoName, (                      \
        DMA_ENABLE         | DMA_TIMMING_SOUND |                    \
        DMA_IF_ENABLE      |                                        \
        DMA_CONTINUOUS_ON  |                                        \
        DMA_SRC_INC        | DMA_DEST_FIX      |                    \
        DMA_32BIT_BUS      | (4*4)/(32/8)))
#endif

//�E�_�C���N�g�T�E���h�p��DMA���Z�b�g���܂��B
//�EDmaSoundIf�͏I�����Ɋ��荞�ݗv���𔭐����܂��B
//
//�E�����F
//  DmaNo       DMA�ԍ�
//  Srcp        �\�[�X�A�h���X
//  FifoName    �f�X�e�B�l�[�V�����A�h���X


/*------------------------------------------------------------------*/
/*                      �c�l�` �I���҂�                             */
/*------------------------------------------------------------------*/

#define DmaWait(DmaNo)                                      \
{                                                           \
    vu32 *(DmaCntp) = (vu32 *)REG_DMA##DmaNo;               \
    while (DmaCntp[2] & DMA_ENABLE) ;                       \
}

//�EDMA�̏I����҂��܂��B
//
//�E�����F
//  DmaNo       DMA�ԍ�


/*------------------------------------------------------------------*/
/*                      �c�l�` �X�g�b�v                             */
/*------------------------------------------------------------------*/

#define DmaStop(DmaNo)                                      \
{                                                           \
    vu16 *(DmaCntp) = (vu16 *)REG_DMA##DmaNo;               \
    DmaCntp[5] &= ~((DMA_TIMMING_MASK | DMA_CONTINUOUS_ON   \
                   | DMA_DREQ_ON) >> 16);                   \
    DmaCntp[5] &= ~( DMA_ENABLE   >> 16);                   \
    DmaCntp[5];                                             \
}

//�EDMA���~���܂��B
//�E�A���A��������DMA�̎����N�����|�������ꍇ�͈�x����DMA�����s����܂��B
//�E�܂��A�T�E���hDMA���~����ꍇ�͋N�����Ɏ�����ƈ�v����
//  �ݒ��\�ߍs�Ȃ��Ă����ĉ������B
//
//�E�����F
//  DmaNo       DMA�ԍ�


/*------------------------------------------------------------------*/
/*                      �b�o�t �N���A                               */
/*------------------------------------------------------------------*/

#define CpuClear(Data, Destp, Size, Bit)                    \
{                                                           \
    vu##Bit Tmp = (vu##Bit )(Data);                         \
    CpuSet((u8 *)&(Tmp), (u8 *)(Destp), (                   \
        DMA_SRC_FIX        |                                \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8) & 0x1fffff))); \
}

#define CpuArrayClear(Data, Destp, Bit)                     \
        CpuClear(     Data, Destp, sizeof(Destp), Bit)

//�ECPU��RAM�N���A����V�X�e���R�[�����Ăяo���܂��B
//�E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
//�ECpuArrayClear�̓f�X�e�B�l�[�V�����z��S�̂��N���A���܂��B
//
//�E�����F
//  Data        �N���A�f�[�^
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  Size        �N���A�o�C�g��
//  Bit         �]���r�b�g���i16|32�j

/*------------------------------------------------------------------*/
/*                      �b�o�t �R�s�[                               */
/*------------------------------------------------------------------*/

#define CpuCopy(Srcp, Destp, Size, Bit)                     \
                                                            \
    CpuSet((u8 *)(Srcp), (u8 *)(Destp),  (                  \
        DMA_SRC_INC        |                                \
        DMA_##Bit##BIT_BUS | ((Size)/(Bit/8) & 0x1fffff)))

#define CpuArrayCopy(Srcp, Destp, Bit)                      \
        CpuCopy(     Srcp, Destp, sizeof(Srcp), Bit)

//�ECPU�ŃR�s�[����V�X�e���R�[�����Ăяo���܂��B
//�ECpuArrayCopy�̓\�[�X�z��S�̂��R�s�[���܂��B
//
//�E�����F
//  Srcp        �\�[�X�A�h���X
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  Size        �]���o�C�g��
//  Bit         �]���r�b�g���i16|32�j

/*------------------------------------------------------------------*/
/*                  �b�o�t �����N���A(32Byte�P��)                   */
/*------------------------------------------------------------------*/

#define CpuFastClear(Data, Destp, Size)                     \
{                                                           \
    vu32 Tmp = (vu32 )(Data);                               \
    CpuFastSet((u8 *)&(Tmp), (u8 *)(Destp), (               \
        DMA_SRC_FIX        | ((Size)/(32/8) & 0x1fffff)));  \
}

#define CpuFastArrayClear(Data, Destp)                      \
        CpuFastClear(     Data, Destp, sizeof(Destp))

//�ECPU�ō�����RAM�N���A����V�X�e���R�[�����Ăяo���܂��B
//�E�N���A�f�[�^�̓X�^�b�N�ɒu����A������f�X�e�B�l�[�V�����փR�s�[���܂��B
//�ECpuFastArrayClear�̓f�X�e�B�l�[�V�����z��S�̂��N���A���܂��B
//�E32bit�]����8Word�i32Byte�j�P�ʂŃN���A���܂��B
//
//�E�����F
//  Data        �N���A�f�[�^
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  Size        �N���A�o�C�g��

/*------------------------------------------------------------------*/
/*                  �b�o�t �����R�s�[(32Byte�P��)                   */
/*------------------------------------------------------------------*/

#define CpuFastCopy(Srcp, Destp, Size)                      \
                                                            \
    CpuFastSet((u8 *)(Srcp), (u8 *)(Destp),  (              \
        DMA_SRC_INC           | ((Size)/(32/8) & 0x1fffff)))

#define CpuFastArrayCopy(Srcp, Destp)                       \
        CpuFastCopy(     Srcp, Destp, sizeof(Srcp))

//�ECPU�ō����ɃR�s�[����V�X�e���R�[�����Ăяo���܂��B
//�ECpuFastArrayCopy�̓\�[�X�z��S�̂��R�s�[���܂��B
//�E32bit�]����8Word�i32Byte�j�P�ʂŃR�s�[���܂��B
//
//�E�����F
//  Srcp        �\�[�X�A�h���X
//  Destp       �f�X�e�B�l�[�V�����A�h���X
//  Size        �]���o�C�g��


#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif /* _AGB_MACRO_H */
