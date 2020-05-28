@********************************************************************
@*          AgbMacro.s                                              *
@*            AGB�W���}�N���֐��iGAS�p�j                            *
@*                                                                  *
@*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               *
@********************************************************************


@--------------------------------------------------------------------
@-                      �V�X�e���R�[��                              -
@--------------------------------------------------------------------

    .MACRO    SystemCall16 No
        swi \No
    .ENDM
    .MACRO    SystemCall32 No
        swi \No << 16
    .ENDM

    .MACRO    Halt                Bits    @ �g�`�k�s
        SystemCall\Bits 2
    .ENDM
    .MACRO    Stop                Bits    @ �r�s�n�o
        SystemCall\Bits 3
    .ENDM


