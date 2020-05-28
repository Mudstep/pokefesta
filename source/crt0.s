@********************************************************************
@*          crt0.s                                                  *
@*            �X�^�[�g�A�b�v���[�`���iGAS�p�j                       *
@*                                                                  *
@*          Copyright (C) 1999-2000 NINTENDO Co.,Ltd.               *
@********************************************************************
#include		"version.h"
    .INCLUDE    "AgbDefine.s"
    .INCLUDE    "AgbMemoryMap.s"
    .INCLUDE    "AgbSyscallDefine.s"
    .INCLUDE    "AgbMacro.s"
    .TEXT

    .INCLUDE    "MultiSioDefine.s"

    .GLOBAL     _start
_start:
@    .INCLUDE    "rom_h.s"
#include		"rom_h.s"

@--------------------------------------------------------------------
@-                          ���Z�b�g                                -
@--------------------------------------------------------------------
    .EXTERN     AgbMain
    .GLOBAL     start_vector
    .CODE 32
start_vector:
        mov     r0, #PSR_IRQ_MODE       @ IRQ���[�h�֐؂芷��
        msr     cpsr, r0
        ldr     sp, sp_irq              @ SP_irq �̃Z�b�g
        mov     r0, #PSR_SYS_MODE       @ �V�X�e�����[�h�֐؂芷��
        msr     cpsr, r0
        ldr     sp, sp_usr              @ SP_usr �̃Z�b�g
        ldr     r1, =INTR_VECTOR_BUF    @ ���荞�݃A�h���X�̃Z�b�g
        adr     r0, intr_main
        str     r0, [r1]
        ldr     r1, =AgbMain            @ �X�^�[�g �� 16bit�R�[�h�ւ̐؊�
        mov     lr, pc
        bx      r1
        b       start_vector            @ ���Z�b�g

    .ALIGN
sp_usr: .word   WRAM_END - 0x1a0		@changed by tama 2001/01/25
sp_irq: .word   WRAM_END - 0x60


@--------------------------------------------------------------------
@-          ���荞�ݕ��򏈗��i�e�[�u�������j32Bit            23-61c -
@--------------------------------------------------------------------
    .EXTERN     IntrTable
    .GLOBAL     intr_main
    .ALIGN
    .CODE 32
intr_main:
        mov     r3, #REG_BASE           @ IE/IF �ǂݍ���
        add     r3, r3, #OFFSET_REG_IE  @ r3: REG_IE
        ldr     r2, [r3]
        ldrh    r1, [r3, #REG_IME - REG_IE]
		
        mrs     r0, spsr                @ ���W�X�^�ޔ��iIRQ���[�h�j
        stmfd   sp!, {r0-r3, lr}        @ {spsr, IME, IF|IE, REG_IE, lr}

        mov     r0, #1                  @ IME = 1�iIME = 0 ���s���Ɋ��荞�݂����������ꍇ�ɂ�
                                        @          ���d���荞�݂������邽�߁j
        strh    r0, [r3, #REG_IME - REG_IE]

        and     r1, r2, r2, lsr #16     @ IE/IF �`�F�b�N
        mov     r12, #0


        ands    r0, r1, #SIO_INTR_FLAG
        bne     jump_intr                    @ �V���A���ʐM ���荞��
        add     r12,r12, #4                  

        ands    r0, r1, #MULTI_SIO_TIMER_INTR_FLAG
        bne     jump_intr                    @ �}���`�v���C�ʐM�p�^�C�}�[ ���荞��
        add     r12,r12, #4                  

        ands    r0, r1, #H_BLANK_INTR_FLAG   @ H�u�����N ���荞��
        bne     jump_intr
        add     r12, r12, #4

        ands    r0, r1, #V_BLANK_INTR_FLAG   @ V�u�����N ���荞��
        bne     jump_intr
        add     r12, r12, #4

        ands    r0, r1, #V_COUNT_INTR_FLAG   @ V�J�E���^ ���荞��
        bne     jump_intr
        add     r12, r12, #4
        ands    r0, r1, #TIMER0_INTR_FLAG    @ �^�C�}�[0 ���荞��
        bne     jump_intr
        add     r12, r12, #4
        ands    r0, r1, #TIMER1_INTR_FLAG    @ �^�C�}�[1 ���荞��
        bne     jump_intr
        add     r12, r12, #4
        ands    r0, r1, #TIMER2_INTR_FLAG    @ �^�C�}�[2 ���荞��
        bne     jump_intr
        add     r12, r12, #4
@        ands    r0, r1, #TIMER3_INTR_FLAG    @ �^�C�}�[3 ���荞��
@       bne     jump_intr
@        add     r12, r12, #4
@        ands    r0, r1, #SIO_INTR_FLAG       @ �V���A���ʐM ���荞��
@        bne     jump_intr
@        add     r12, r12, #4
        ands    r0, r1, #DMA0_INTR_FLAG      @ DMA0 ���荞��
        bne     jump_intr
        add     r12, r12, #4
        ands    r0, r1, #DMA1_INTR_FLAG      @ DMA1 ���荞��
        bne     jump_intr
        add     r12, r12, #4
        ands    r0, r1, #DMA2_INTR_FLAG      @ DMA2 ���荞��
        bne     jump_intr
        add     r12, r12, #4
        ands    r0, r1, #DMA3_INTR_FLAG      @ DMA3 ���荞��
        bne     jump_intr
        add     r12, r12, #4
        ands    r0, r1, #KEY_INTR_FLAG       @ �L�[ ���荞��
        bne     jump_intr
        add     r12, r12, #4
        ands    r0, r1, #CASSETTE_INTR_FLAG  @ �J�Z�b�g ���荞��
        strneb  r0, [r3, #REG_SOUNDCNT_X - REG_IE]  @ �T�E���h��~
loop:	bne		loop
jump_intr:
        strh    r0, [r3, #2]                    @ IF �N���A         22c
        ldr     r1, =H_BLANK_INTR_FLAG| SIO_INTR_FLAG | MULTI_SIO_TIMER_INTR_FLAG       @ IE �Z�b�g <- ���d���荞�ݑI��
		bic		r2, r2, r0							@���ꊄ�荞�݂����d�Ɋ|����Ȃ��悤�ɂ���
		and		r1, r1, r2							@IE �ɃZ�b�g�����Ă������荞�݂̂ݑ��d���荞�݂�����
        strh    r1, [r3]
        mrs     r3, cpsr                            @ ���d���荞�� ���� �� �V�X�e�����[�h�֐؊�
        bic     r3, r3, #PSR_CPU_MODE_MASK | PSR_IRQ_DISABLE | PSR_FIQ_DISABLE
        orr     r3, r3, #PSR_SYS_MODE
        msr     cpsr, r3
@---------------------------------------------------------------------�V�X�e�����[�h
        ldr     r1, =IntrTable                      @ ���[�UIRQ�����փW�����v
        add     r1, r1, r12
        ldr     r0, [r1]

        stmfd   sp!, {lr}                           @ ���W�X�^�ޔ��i�V�X�e�����[�h�j
        adr     lr, intr_return                     @ ���^�[���A�h���X�̃Z�b�g
        bx      r0
intr_return:
        ldmfd   sp!, {lr}                           @ ���W�X�^���A�i�V�X�e�����[�h�j

        mrs     r3, cpsr                            @ ���d���荞�� �s���� �� IRQ���[�h�֐؊�
        bic     r3, r3, #PSR_CPU_MODE_MASK | PSR_IRQ_DISABLE | PSR_FIQ_DISABLE
        orr     r3, r3, #PSR_IRQ_MODE      | PSR_IRQ_DISABLE
        msr     cpsr, r3
@---------------------------------------------------------------------IRQ���[�h
        ldmfd   sp!, {r0-r3, lr}                    @ ���W�X�^���A�iIRQ���[�h�j
        strh    r2,  [r3]                       @ {spsr, IME, IF|IE, REG_IE, lr}
        strh    r1,  [r3, #REG_IME - REG_IE]
        msr     spsr, r0
        bx      lr                              @ ���荞�ݑO�֕��A


@   .ORG    0x200


    .END

