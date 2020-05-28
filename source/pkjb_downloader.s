@ PK-JOY�u�[�g�p �_�E�����[�_ (�J�[�g���b�WROM�Ɋi�[)
@

    .INCLUDE    "AgbDefine.s"
    .INCLUDE    "AgbMemoryMap.s"
    .INCLUDE    "AgbSyscallDefine.s"
    .INCLUDE    "AgbMacro.s"

@ ���̃t�@�C���ŁA�Q�[���ɉ����ĕK���m�F(�ƕύX)���Ă��������ꏊ
@ 1. "PKJB_GBA_INITIAL_CODE_p:" �̒�`
@ 2. "PKJB_PROGRAM_SIZE =" �̒�`
@ 3. "text_section_addr_p:" �̒�`
@ ��L�̒�`(1. 2.)�́AGC�� pkjb_uploader.h �ł��s���Ă��܂��B
@ ���l�ɕύX���Ă��������B

@ GBA�C�j�V�����R�[�h�ɂ��āA�V���[�Y���ŒʐM�v���g�R����
@ �݊�������������ꍇ�́A�ǂꂩ��̃C�j�V�����R�[�h�����ʂ�
@ �g���Ă��������B
@ ��: �|�P�����ԁA�ǂ��������PK-JOY�u�[�g�C���[�W���g�p�\�Ȃ�
@     �u�|�P�����ԁv�̃C�j�V�����R�[�h���|�P�����ł��g���܂��B


@ �_�E�����[�h�o�b�t�@�T�C�Y (8�̔{��)
@ �Q�[���ɉ������_�E�����[�h�o�b�t�@�̃T�C�Y(�ő�l)�ɕύX���Ă��������B
@ �ő�l�� 0x3fff0 (Bytes) �ł��B

PKJB_PROGRAM_SIZE = 0x20000


@ PKJBDownParam �̍\���̃I�t�Z�b�g��`
	.struct	0
timeout_counter: .struct . + 1; 
@ ��莞�ԒʐM���荞�݂��������Ȃ��ꍇ�^�C���A�E�g�������s�����߂̃J�E���^
frame_counter: .struct . + 1; @ ���������p
decode_mode: .struct . + 1; @ ���������[�h 0=�_�E�����[�h���Ă��Ȃ� 1=�������� 2=����������
vcount_bak: .struct . + 1; @ �Í����������p
keya: .struct . + 4; @ KeyA, byte access ���Ԃ񂠂�
keyb: .struct . + 4; @ KeyB, byte access ����
keyc: .struct . + 4; @ KeyC, byte access ���Ԃ񂠂�
boot_key: .struct . + 2; @ GC����́uPK-JOY�u�[�g�v���v(�������O)
dl_count: .struct . + 2; @ �_�E�����[�h�c�胏�[�h�J�E���g

	.struct . + 0; @ dummy_aligner
decode_key: .struct . + 4; @ �_�E�����[�h�f�[�^�̕�������
decode_work_crc: .struct . + 4; @ �������r���� CRC
decode_result_crc: .struct . + 4; @ ������������GC�֕Ԃ�CRC
@ �ȉ��̃����o�[�́A���������Ƀ[���N���A�ΏۊO�B
decode_bufp: .struct . + 4; @ �������p�o�b�t�@�|�C���^
dl_bufp: .struct . + 4; @ �_�E�����[�h�o�b�t�@�|�C���^
joyint_vector: .struct . + 4; @ JoyIntr �Ŏg���W�����v�x�N�^ - �[���N���A���Ȃ��悤�ɁA�Ō�̃����o�ɂ���B
SIZEOF_PKJB_DOWN_PARAM:

@ ���W�X�^�A�N�Z�X�p�I�t�Z�b�g
MY_OFFSET_REG_JOYCNT	=	REG_JOYCNT    - REG_SIOMULTI0
MY_OFFSET_REG_RCNT	=	REG_RCNT      - REG_SIOMULTI0
MY_OFFSET_REG_JOYSTAT	=	REG_JOYSTAT   - REG_SIOMULTI0
MY_OFFSET_REG_JOY_TRANS	=	REG_JOY_TRANS - REG_SIOMULTI0
MY_OFFSET_REG_JOY_RECV	=	REG_JOY_RECV  - REG_SIOMULTI0

MY_OFFSET_REG_IE	=	0
MY_OFFSET_REG_IME	=	REG_IME       - REG_IE
MY_OFFSET_REG_IF	=	REG_IF        - REG_IE

PKJB_FROMGC_REQUEST_DL   = 0xdd @ d31-d24, �_�E�����[�h�J�n�v��
PKJB_FROMGC_REQUEST_BOOT = 0xbb @ d31-d24, PK-JOY�u�[�g�v��
PKJB_FROMGBA_DL_READY    = 0xee @ d7-d0,   �_�E�����[�h�J�n������
PKJB_FROMGBA_CALC_CRC    = 0xcc @ d7-d0,   �`�F�b�N�T���v�Z������
PKJB_FROMGBA_CRC_READY   = 0xff @ d7-d0,   �`�F�b�N�T���v�Z��������

PKJB_JOYINT_TIMEOUT      = 10   @ �^�C���A�E�g�J�E���^�l
PKJB_GBA_ROM_HEADER_SIZE = 0xa0 @ Nintendo�w�b�_�T�C�Y(��Í���)

PKJB_CRC_POLY            = 0xa1c1     @ ���n������
PKJB_RNG_MUL             = 0x6177614b @ ���������p
PKJB_RNG_ADD             = 1          @ ���������p


	.text
	.align

@ �����̏����ɂ����ӁB
@ static u32 PKJBDownCalcCrc(r0=keep, r1=u32 src, r2=broken, r3=u32 crc);
	.code	16
	.thumb_func
PKJBDownCalcCrc:
@ r4: CRC poly
	push	{r4, lr};
	ldr	r4, crc_poly_p;
	eor	r3, r1;
	mov	r2, #32;
1:
	lsr	r3, #1; @ crc >>= 1 �͏����ɂ�����炸����
	bcc	2f;
	eor	r3, r4;
2:
	sub	r2, #1;
	bne	1b;
	pop	{r4, pc};


@ extern void PKJBDownMainFrame(PKJBDownParam *paramp);

	.global	PKJBDownMainFrame
	.code	16
	.thumb_func
PKJBDownMainFrame:
@ joyint_vector �� 0 �ɂȂ��Ă����珉�����v���B
	ldr	r1, [r0, #joyint_vector];
	cmp	r1, #0;
	beq	main_frame_bcs_init; @ Branch out of range. beq �Ȃ� bcs �����藧��

@ ���������p�t���[���J�E���^�X�V
	ldrb	r1, [r0, #frame_counter];
	add	r1, #1;
	strb	r1, [r0, #frame_counter];

@ ���������������Ă��Ȃ���΁A�^�C���A�E�g�J�E���^�X�V�B
@ ���荞�݃��[�`���ł�����������̂ŁAREG_IME = 0 �ɂ���B
	ldrb	r1, [r0, #decode_mode];
	cmp	r1, #2; @ �����������H
	beq	main_frame_bx_lr;

	ldr	r3, my_reg_200p;
	ldrh	r2, [r3, #MY_OFFSET_REG_IME];
@ r2 = imebak
	mov	r1, #0;
@ REG_IME = 0
	strh	r1, [r3, #MY_OFFSET_REG_IME];
	ldrb	r1, [r0, #timeout_counter];
	cmp	r1, #PKJB_JOYINT_TIMEOUT;
	bgt	1f; @ ���łɃ^�C���A�E�g�ɂȂ��Ă���
	add	r1, #1;
	strb	r1, [r0, #timeout_counter];
1:
@ REG_IME = imebak
	strh	r2, [r3, #MY_OFFSET_REG_IME];

	.thumb_func
main_frame_bcs_init:
	bcs	PKJBDownInit; @ �^�C���A�E�g�G���[�Ȃ̂ŏ��������s��

@ �_�E�����[�h�����H
	ldrb	r1, [r0, #decode_mode];
	cmp	r1, #0; @ �_�E�����[�h���Ă��Ȃ��H
	bne	10f;    @ ���Ƃ� r1 == 1 �����Ȃ��B
@ �_�E�����[�h���Ă��Ȃ��Ǝv����^�C�~���O�ŁAdl_bufp �� decode_bufp ���r�B
@ �قȂ�΁A�_�E�����[�h���n�܂��Ă���B
	ldr	r1, [r0, #dl_bufp];
	ldr	r2, [r0, #decode_bufp];
	sub	r1, r2;
	beq	99f; @ �{���Ɏn�܂��Ă��Ȃ������B
@ �n�܂��Ă���B
@ �������ANintendo�w�b�_���_�E�����[�h���I���܂ł͔������Ȃ��B
	cmp	r1, #PKJB_GBA_ROM_HEADER_SIZE;
	bcc	99f;
@ Nintendo�w�b�_��ROM�J�[�g���b�W�����̂��̂Əƍ��B
	push	{r4, r5, r6};
	mov	r1, #PKJB_GBA_ROM_HEADER_SIZE - 8; @ �擪 4 Bytes �͖����B
	add	r2, #4;                            @ �擪 4 Bytes �͖����B
	ldr	r4, rom_header_p;                  @ �擪 4 Bytes �͖����B
1:
	ldmia	r2!, {r5};
	ldmia	r4!, {r6};
	cmp	r5, r6;
	bne	2f;
	sub	r1, #4;
	bne	1b;
@ �Ō�̃��[�h�� d7-d0 �͖����B
	ldmia	r2!, {r5};
	ldmia	r4!, {r6};
	eor	r5, r6;
	lsr	r5, #8;
	str	r2, [r0, #decode_bufp];
2:
@ bne �Ȃ�s��v�B�ʐM�������B
	pop	{r4, r5, r6};
	bne	PKJBDownInit;
@ ��v�B�����������m�肷��B
	mov	r1, #1;
	strb	r1, [r0, #decode_mode];
	ldr	r1, [r0, #keya];
	ldr	r2, [r0, #keyb];
	eor	r1, r2;
	str	r1, [r0, #decode_work_crc];
	ldr	r2, rng_mul_p;
	mul	r1, r2;
	add	r1, #PKJB_RNG_ADD;
	str	r1, [r0, #decode_key];

	.thumb_func
main_frame_bx_lr:
	bx	lr;

10:
@ decode_bufp < dl_bufp �ł���̈�͕������\�B
@ ip: decode end ptr
@ r1: decode data
@ r2: i
@ r3: CRC
@ r4: decode current ptr
@ r5: RNG mul constant
@ r6: decode key
@ r7: CRC poly

	ldr	r1, [r0, #dl_bufp];
	mov	ip, r1;
	ldr	r3, [r0, #decode_work_crc];

	push	{r4, r5, r6, r7};
	ldr	r4, [r0, #decode_bufp];
	ldr	r5, rng_mul_p;
	ldr	r6, [r0, #decode_key];
	ldr	r7, crc_poly_p;

	.thumb_func
decode_loop:
	cmp	r4, ip;
	bcs	decode_done;

	ldr	r1, [r4];
	eor	r1, r6;
	add	r1, r3;
	stmia	r4!, {r1};

@ update crc
@ {
@  u32 i;
@  crc ^= src;
@  for (i = 32; i > 0; i--) {
@    if (crc & 1) {
@      crc >>= 1;
@      crc ^= CRC_POLY;
@    } else {
@      crc >>= 1;
@    }
@  }
@  return crc;
@ }

	eor	r3, r1;
	mov	r2, #32;
1:
	lsr	r3, #1; @ crc >>= 1 �͏����ɂ�����炸����
	bcc	2f;
	eor	r3, r7;
2:
	sub	r2, #1;
	bne	1b;

@ update decode key
	mul	r6, r5;
	add	r6, #PKJB_RNG_ADD;
	b	decode_loop;

	.thumb_func
decode_done:
	str	r4, [r0, #decode_bufp];
	str	r6, [r0, #decode_key];
	pop	{r4, r5, r6, r7};
	str	r3, [r0, #decode_work_crc];

	ldrh	r1, [r0, #dl_count];
	cmp	r1, #0;
	bne	99f;
@ �S���_�E�����[�h�����B
@ �������A�������͏I������Ƃ͌���Ȃ��̂ŁA
@ dl_bufp == decode_bufp �ł��邱�Ƃ��m�F����B
	ldr	r1, [r0, #dl_bufp];
	ldr	r2, [r0, #decode_bufp];
	cmp	r1, r2;
	bne	99f;

@ �u�`�F�b�N�T���v�Z���������v�͏����ς� or �����ς݂��H
	ldr	r1, [r0, #keyc];
	cmp	r1, #0;
	beq	11f; @ �������ł��Ă��Ȃ��B

@ �uPK-JOY�u�[�g�v���v�͎�M���Ă��邩�H
	ldrh	r1, [r0, #boot_key];
	cmp	r1, #0;
	beq	main_frame_bx_lr; @ ��M���Ă��Ȃ��B

@ �uPK-JOY�u�[�g�v���v�̐����������؁B
	mov	ip, lr;
	mov	r1, #PKJB_FROMGC_REQUEST_BOOT;
	ldr	r3, [r0, #keyc];
	bl	PKJBDownCalcCrc;
	ldrh	r1, [r0, #boot_key];
	mov	lr, ip; @ PKJBDownInit �֔�Ԃ�������Ȃ��̂ŁB
	sub	r1, r3;
	bne	PKJBDownInit; @ �s���Ȓl�B���������s

@ �����������B�u�[�g�\��ԁBdecode_mode �� 2 �ɂ���B
	mov	r1, #2;
	strb	r1, [r0, #decode_mode];
	bx	lr;

11:
	mov	ip, lr;
@ �u�`�F�b�N�T���v�Z���������v����������B
@ KeyC �� (d31-d24 �� vcount_bak) �ɂ������̂��� -1 ����B
	ldrb	r1, [r0, #vcount_bak];
	lsl	r1, #24;
	sub	r1, #1;
	str	r1, [r0, #keyc];
@ r3 �� decode_work_crc �� d15-d0 �͈̔͂ɂ���B
	bl	PKJBDownCalcCrc;

	lsl	r3, #8;
	add	r3, #PKJB_FROMGBA_CRC_READY;
	str	r3, [r0, #decode_result_crc];
	bx	ip;


99:
	bx	lr;


	.align
crc_poly_p:
	.word	PKJB_CRC_POLY
rng_mul_p:
	.word	PKJB_RNG_MUL
rom_header_p:
	.word	ROM_BANK0 + 4


@ extern void PKJBDownTryBoot(PKJBDownParam *paramp);

	.global	PKJBDownTryBoot
	.code	16
	.thumb_func
PKJBDownTryBoot:
@ �����������ł����PK-JOY�u�[�g����B
	ldrb	r1, [r0, #decode_mode];
	cmp	r1, #2;
	bne	1f;     @ ���������������Ă��Ȃ��B

@ REG_IME ���� 0 �ɂ���B
	ldr	r3, my_reg_200p;
	mov	r1, #0;
	strh	r1, [r3, #MY_OFFSET_REG_IME];

	ldr	r1, text_section_addr_p;
	add	r1, #0xc0;
	bx	r1;
1:
	bx	lr;

@ extern void PKJBDownInit(PKJBDownParam *paramp);

	.global	PKJBDownInit
	.code	16
	.thumb_func
PKJBDownInit:
@ JOY�o�X�ʐM���荞�݋��B
@ ������ REG_IME �̐ݒ�͕ύX���Ȃ��B
	ldr	r3, my_reg_200p;
	ldrh	r2, [r3, #MY_OFFSET_REG_IME];
@ r2 = imebak
	mov	r1, #0;
@ REG_IME = 0
	strh	r1, [r3, #MY_OFFSET_REG_IME];

@ �K�v�Ȃ烏�[�N���N���A
@ �Ȍ�Ar0�͔j��B
	adr	r3, joyint_err;
	str	r3, [r0, #joyint_vector];
	ldrb	r3, [r0, #vcount_bak];
	push	{r3};
	ldrb	r3, [r0, #frame_counter];
	push	{r0, r3};

	mov	r3, r0;
	add	r3, #SIZEOF_PKJB_DOWN_PARAM - 12; @ �[���N���A�ΏۊO�̃����o�������B
1:
	stmia	r0!, {r1};
	cmp	r0, r3;
	bcc	1b;
	pop	{r0, r3};
@ vcount_bak �� 0-227 (0xe3) �ł��邱�Ƃ�O��ɂ��Ă���̂� d7 �� 0 �ɂ���B
	lsr	r3, #1;
	strb	r3, [r0, #vcount_bak]; @ ������ push/pop �̏����t�ɂ��Ă݂�
	pop	{r3};
	strb	r3, [r0, #frame_counter];

@ REG_RCNT = R_DIRECT_MODE
@ R_DIRECT_MODE �� 0x8000 �ł���B
@ REG_SIOMULTI0 �� 0x04000120 �Ȃ̂� << (15-5) ����Ή��ʂ� 0x8000 �ɂȂ�B
	ldr	r3, my_reg_120p;
	lsl	r0, r3, #15 - 5;
	strh	r0, [r3, #MY_OFFSET_REG_RCNT];

@ REG_RCNT = R_JOY_MODE
@ R_JOY_MODE = 0xc000
	mov	r0, #R_JOY_MODE >> 8;
	lsl	r0, #8;
	strh	r0, [r3, #MY_OFFSET_REG_RCNT];

@ REG_JOYCNT = JOY_IF_ENABLE | JOY_IF_SEND | JOY_IF_RECV | JOY_IF_RESET
	mov	r0, #JOY_IF_ENABLE | JOY_IF_SEND | JOY_IF_RECV | JOY_IF_RESET;
	strh	r0, [r3, #MY_OFFSET_REG_JOYCNT];

@ REG_JOYSTAT = 0
	strh	r1, [r3, #MY_OFFSET_REG_JOYSTAT];

@ �c���Ă��� IF �� SIO_INTR_FLAG �̓N���A�B
@ REG_IF = SIO_INTR_FLAG
	ldr	r3, my_reg_200p;
	mov	r0, #SIO_INTR_FLAG;
	strh	r0, [r3, #MY_OFFSET_REG_IF];
	ldrh	r1, [r3, #MY_OFFSET_REG_IE];
	orr	r1, r0;
	strh	r1, [r3, #MY_OFFSET_REG_IE];

@ REG_IME = imebak
	strh	r2, [r3, #MY_OFFSET_REG_IME];

	bx	lr;


@ extern void PKJBDownJoyIntr(PKJBDownParam *paramp);

	.global	PKJBDownJoyIntr
	.code	16
	.thumb_func
PKJBDownJoyIntr:
	ldr	r3, my_reg_120p;
@ ���񏈗�(or ����)���� SEND, RECV, RESET �t���O���N���A�B
	ldrh	r1, [r3, #MY_OFFSET_REG_JOYCNT];
	strh	r1, [r3, #MY_OFFSET_REG_JOYCNT];

@ �^�C���A�E�g�J�E���^���N���A�B
	mov	r2, #0;
	strb	r2, [r0, #timeout_counter];

@ �V�[�P���X�ɉ������A�h���X�փW�����v�B
	ldr	r2, [r0, #joyint_vector];
	cmp	r2, #0;
	beq	joyint_bx_lr; @ ���C�����[�`���ŏ��������s���K�v������

@ JOY_IF_RESET ?
	lsr	r1, #1;
	bcs	resp_initial_code;

@ JOY_IF_SEND �� r1 �� d1 �ɂ���B
	mov	pc, r2;

	.align
	.thumb_func
joyint_err:
@ �G���[�̉񕜂́AREG_JOYSTAT = 0 �ɂ��āAJOY�o�X�f�o�C�X���Z�b�g�B
@ �C�j�V�����R�[�h��������B
	mov	r2, #0;
	strh	r2, [r3, #MY_OFFSET_REG_JOYSTAT];
@ r2 = 0 (keep!)
	.thumb_func
joyint_done:
@ ����̃A�h���X���Z�b�g���A���荞�݃��[�`���I���B
	str	r2, [r0, #joyint_vector];
	.thumb_func
joyint_done_samestate:
@ �Í����̂��߂ɁAREG_VCOUNT �̒l��ۑ��B
	ldr	r3, my_reg_dispstat_p;
	ldrh	r1, [r3, #REG_VCOUNT - REG_STAT];
	strb	r1, [r0, #vcount_bak];
	.thumb_func
joyint_bx_lr:
	bx	lr;


	.thumb_func
resp_initial_code:
@ �C�j�V�����R�[�h����
@ REG_JOY_RECV ����ǂ݁B
	ldr	r1, [r3, #MY_OFFSET_REG_JOY_RECV];
@ REG_JOY_TRANS �փC�j�V�����R�[�h���Z�b�g�B
	ldr	r1, PKJB_GBA_INITIAL_CODE_p;
	str	r1, [r3, #MY_OFFSET_REG_JOY_TRANS];
@ REG_JOYSTAT = JSTAT_PSF0
	mov	r1, #JSTAT_PSF0;
	strh	r1, [r3, #MY_OFFSET_REG_JOYSTAT];
@ vcount_bak ���� KeyB d15-d8 ���Z�b�g�B
	ldrb	r1, [r0, #vcount_bak];
	strb	r1, [r0, #keyb + 1]; @ byte access
@ ���������[�h�� 0 �łȂ���΃G���[�B
	ldrb	r1, [r0, #decode_mode];
	cmp	r1, #0;
	bne	joyint_err;
@ �o�b�t�@�|�C���^�����Z�b�g
	ldr	r1, text_section_addr_p;
	str	r1, [r0, #decode_bufp];
	str	r1, [r0, #dl_bufp];
@ JOY_IF_SEND, JOY_IF_RECV �Ƃ��ɖ�������B
@ ���� joyint_000 ����B
	adr	r2, joyint_000;
	b	joyint_done;

	.align
	.thumb_func
joyint_000:
@ �C�j�V�����R�[�h�𑗐M���Ă��Ȃ���΃G���[�B
@ JOY_IF_SEND �� r1 �� d1 �ɂ���B
	lsl	r1, #31;
	bcc	joyint_err;
@ ���� joyint_100 ����B���ł� RECV ���Ă���\��������B
	bmi	joyint_110;  @ ���ł� RECV ���Ă���
	adr	r2, joyint_100;
	b	joyint_done;

	.align
	.thumb_func
joyint_100:
@ �C�j�V�����R�[�h����M���Ă��Ȃ���΃G���[�B
@ JOY_IF_RECV �� r1 �� d0 �ɂ���B
	lsr	r1, #1;
	bcc	joyint_err;

	.thumb_func
joyint_110:
	ldr	r1, [r3, #MY_OFFSET_REG_JOY_RECV];
@ �C�j�V�����R�[�h�́A���҂������̂��H
	ldr	r2, PKJB_GBA_INITIAL_CODE_p;
	cmp	r1, r2;
	bne	joyint_err;
@ vcount_bak ���� KeyB d31-d24 ���Z�b�g�B
	ldrb	r1, [r0, #vcount_bak];
	strb	r1, [r0, #keyb + 3]; @ byte access
@ ���� joyint_200 ����B
	adr	r2, joyint_200;
	b	joyint_done;

	.align
	.thumb_func
joyint_200:
@ �u�_�E�����[�h�J�n�v���{�Í�����A�v����M���Ă��Ȃ���΃G���[�B
	lsr	r1, #1;
	bcc	joyint_err;
	ldr	r1, [r3, #MY_OFFSET_REG_JOY_RECV];
	lsr	r2, r1, #24;
	cmp	r2, #PKJB_FROMGC_REQUEST_DL;
	bne	joyint_err;
@ KeyA ��ۑ��B
	str	r1, [r0, #keya];
@ REG_JOY_TRANS �ցu�_�E�����[�h�J�n�������{�Í�����B�v���Z�b�g�B
	ldrb	r1, [r0, #frame_counter];
	strb	r1, [r0, #keyb + 2]; @ byte access
@ KeyB �ɂ����āAd31-d8 �̒��� 1 �� 7�ȏ�A14�ȓ��ɂ��Ȃ���΂Ȃ�Ȃ��B
@ �����Ȃ��Ă��Ȃ��ꍇ�Ad23-d16 �� all 0 or all 1 �ɂ��邱�ƂŒ�������B
@ d31-d24, d15-d8 �� REG_VCOUNT �̒l�Ȃ̂ŁA0-227 (0xe3) ������ 1 �� 8 �ɂ͂Ȃ�Ȃ��B
	mov	r2, #0; @ bitcount
	mov	r3, #0; @ adc ���邽��
	ldr	r1, [r0, #keyb]; @ word access
	lsr	r1, #8; @ skip d7-d0
1:
	lsr	r1, #1;
	adc	r2, r3;
	cmp	r1, #0;
	bne	1b;
@ r1 = 0 �ɂȂ��Ă���B
	cmp	r2, #14;
	bgt	2f;      @ 1 �� 14 ���������̂� d23-d16 �� all 0 �ɂ���B
	cmp	r2, #7;
	bge	3f;      @ 1 �� 7 �ȏ� 14 �ȓ��Ȃ̂ŁA�����Ȍ��B
@ 1 �� 7 �����B
	mov	r1, #0xff
2:
	strb	r1, [r0, #keyb + 2]; @ byte access
3:
	ldr	r1, [r0, #keyb]; @ word access
	add	r1, r1, #PKJB_FROMGBA_DL_READY;
	ldr	r3, my_reg_120p;
	str	r1, [r3, #MY_OFFSET_REG_JOY_TRANS];
@ �X�e�[�^�X�� JSTAT_PSF1 | JSTAT_PSF0 �ɁB
	mov	r1, #JSTAT_PSF1 | JSTAT_PSF0;
	strh	r1, [r3, #MY_OFFSET_REG_JOYSTAT];
@ ���� joyint_300 ����B
	adr	r2, joyint_300;
	b	joyint_done;

	.align
	.thumb_func
joyint_300:
@ �u�_�E�����[�h�J�n�������{�Í�����B�v�𑗐M���Ă��Ȃ���΃G���[�B
@ JOY_IF_SEND �� r1 �� d1 �ɂ���B
	lsl	r1, #31;
	bcc	joyint_err;
@ ���� joyint_400 ����B���ł� RECV ���Ă���\��������B
	bmi	joyint_410; @ ���ł� RECV ���Ă���
	adr	r2, joyint_400;
	b	joyint_done;

	.align
	.thumb_func
joyint_400:
@ �uPK-JOY�u�[�g�C���[�W�T�C�Y�v����M���Ă��Ȃ���΃G���[�B
@ JOY_IF_RECV �� r1 �� d0 �ɂ���B
	lsr	r1, #1;
	bcc	joyint_err;

	.thumb_func
joyint_410:
	ldr	r1, [r3, #MY_OFFSET_REG_JOY_RECV];
@ ���Ƃ��� 8 �̔{�����A8 �Ŋ����Ď�M�����B
	ldr	r2, =PKJB_PROGRAM_SIZE >> 3;
	cmp	r1, r2;
	bcs	joyint_err;
@ �o�b�t�@�I�[�o�t���[�̓Z�L�����e�B�z�[���ɂȂ蓾��B
@ �f�[�^�T�C�Y�Ƃ��� 0 ��n����Ă����Ȃ��悤�ɂ���B
	add	r1, #1;
	add	r1, r1;
	strh	r1, [r0, #dl_count];
@ ���������[�h�� 0 �łȂ���΃G���[�B
	ldrb	r1, [r0, #decode_mode];
	cmp	r1, #0;
	.thumb_func
joyint_410_bne_joyint_err:
	bne	joyint_err;
@ �o�b�t�@�|�C���^�����Z�b�g
	ldr	r1, text_section_addr_p;
	str	r1, [r0, #decode_bufp];
	str	r1, [r0, #dl_bufp];

@ ���� joyint_500 ����B
	adr	r2, joyint_500;
	b	joyint_done;

	.align
	.thumb_func
joyint_500:
@ �uPK-JOY�u�[�g�C���[�W�v�̎�M���J��Ԃ��B
@ JOY_IF_RECV �� r1 �� d0 �ɂ���B
	lsr	r1, #1;
	bcc	joyint_err;

@ �_�E�����[�h�o�b�t�@�|�C���^�� 0x04 �̈ʂ� 0 or 1 �ŁAREG_JOYSTAT ��
@ JSTAT_PSF0 �� 0 or 1 �ɂ���BJSTAT_PSF1 �͏�� 1
	ldr	r2, [r0, #dl_bufp];
	mov	r1, #0x04;
	and	r1, r2;
	add	r1, #0x08; @ JSTAT_PSF1 = 1
	lsl	r1, #2;    @ 0x0c -> 0x30
	strh	r1, [r3, #MY_OFFSET_REG_JOYSTAT];

	ldr	r1, [r3, #MY_OFFSET_REG_JOY_RECV];
	stmia	r2!, {r1};
	str	r2, [r0, #dl_bufp];
@ �_�E�����[�h�c�胏�[�h�J�E���g�����炷�B
	ldrh	r1, [r0, #dl_count];
	sub	r1, #1;
	strh	r1, [r0, #dl_count];
	bne	joyint_done_samestate;
@ �S�f�[�^�̃_�E�����[�h���I������B
@ �������ƃ`�F�b�N�T���ƍ�����������܂ł́u�`�F�b�N�T���v�Z�������v
@ ���s���B
@ �����������B
	.thumb_func
joyint_550:
	ldrb	r1, [r0, #frame_counter];
	lsl	r1, #8;
	add	r1, #PKJB_FROMGBA_CALC_CRC;
	str	r1, [r3, #MY_OFFSET_REG_JOY_TRANS];
@ ���� joyint_600 ����B
	adr	r2, joyint_600;
	b	joyint_done;

	.align
	.thumb_func
joyint_600:
@ �u�`�F�b�N�T���v�Z�������v�𑗐M���Ă��Ȃ���΃G���[�B
@ JOY_IF_SEND �� r1 �� d1 �ɂ���B
	lsl	r1, #31;
	.thumb_func
joyint_600_bcc_joyint_err:
	bcc	joyint_err;

@ �`�F�b�N�T���v�Z���I����Ă���΂����Ԃ��B
	ldr	r1, [r0, #decode_result_crc];
	cmp	r1, #0;
	beq	joyint_550; @ �I����Ă��Ȃ�

@ �I������B
	str	r1, [r3, #MY_OFFSET_REG_JOY_TRANS];
@ ���� joyint_800 ����B
	adr	r2, joyint_800;
	b	joyint_done;

	.align
	.thumb_func
joyint_800:
@ CRC�𑗐M���Ă��Ȃ���΃G���[�B
@ JOY_IF_SEND �� r1 �� d1 �ɂ���B
	lsl	r1, #31;
	bcc	joyint_600_bcc_joyint_err;
@ ���� joyint_900 ����B���ł� RECV ���Ă���\��������B
	bmi	joyint_910; @ ���ł� RECV ���Ă���
	adr	r2, joyint_900;
	b	joyint_done;

	.align
	.thumb_func
joyint_900:
@ KeyC�ŕ������ł���uPK-JOY�u�[�g�v���v����M���Ă��Ȃ���΃G���[�B
@ JOY_IF_RECV �� r1 �� d0 �ɂ���B
	lsr	r1, #1;
	bcc	joyint_600_bcc_joyint_err;

	.thumb_func
joyint_910:
	ldr	r1, [r3, #MY_OFFSET_REG_JOY_RECV];
	lsr	r2, r1, #24;
	cmp	r2, #PKJB_FROMGC_REQUEST_BOOT;
	bne	joyint_410_bne_joyint_err;
@ ���C�����[�`���Ő����������؂���B
	strh	r1, [r0, #boot_key];
@ ��� RECV, SEND �̓G���[�B
	adr	r2, joyint_999;
	b	joyint_done;

	.align
	.thumb_func
joyint_999:
	b	joyint_err;


@ extern void PKJBDownStop(void);

	.global	PKJBDownStop
	.code	16
	.thumb_func
PKJBDownStop:
@ JOY�o�X�ʐM, SIO���荞�݋֎~�B
@ ������ REG_IME �̐ݒ�͕ύX���Ȃ��B
	ldr	r3, my_reg_200p;
	ldrh	r2, [r3, #MY_OFFSET_REG_IME];
@ r2 = imebak
	mov	r1, #0;
@ REG_IME = 0
	strh	r1, [r3, #MY_OFFSET_REG_IME];

@ REG_JOYCNT = JOY_IF_SEND | JOY_IF_RECV | JOY_IF_RESET
@ ���Ȃ킿 JOY_IF_ENABLE ���N���A����B
	ldr	r3, my_reg_120p;
	mov	r0, #JOY_IF_SEND | JOY_IF_RECV | JOY_IF_RESET;
	strh	r0, [r3, #MY_OFFSET_REG_JOYCNT];

@ REG_RCNT = R_DIRECT_MODE
@ R_DIRECT_MODE �� 0x8000 �ł���B
@ REG_SIOMULTI0 �� 0x04000120 �Ȃ̂� << (15-5) ����Ή��ʂ� 0x8000 �ɂȂ�B
	lsl	r0, r3, #15 - 5;
	strh	r0, [r3, #MY_OFFSET_REG_RCNT];

@ IE, IF �� SIO_INTR_FLAG �̓N���A�B
@ REG_IF = SIO_INTR_FLAG
	ldr	r3, my_reg_200p;
	mov	r0, #SIO_INTR_FLAG;
	strh	r0, [r3, #MY_OFFSET_REG_IF];
	ldrh	r1, [r3, #MY_OFFSET_REG_IE];
	bic	r1, r0;
	strh	r1, [r3, #MY_OFFSET_REG_IE];

@ REG_IME = imebak
	strh	r2, [r3, #MY_OFFSET_REG_IME];

	bx	lr;

	.align
	.ltorg
	.align

my_reg_200p:
	.word	REG_IE
my_reg_120p:
	.word	REG_SIOMULTI0
my_reg_dispstat_p:
	.word	REG_STAT

@ �Q�[���ɉ�����GBA�C�j�V�����R�[�h�̏������ɕς��Ă��������B
PKJB_GBA_INITIAL_CODE_p:
	@ .byte	'a', 'g', 'b', 'j'
	.byte	'A', 'X', 'V', 'J'

@ �Q�[���ɉ������_�E�����[�h�o�b�t�@�A�h���X���w�肵�Ă��������B(4�̔{��)
@ PK-JOY�u�[�g�C���[�W (../jb/jb.bin) �� .text �Z�N�V�����J�n�A�h���X
@ �Ɠ����ɂȂ�悤�A../jb/Makefile �� -Ttext ���ύX���Ă��������B
text_section_addr_p:
	.word	0x02000000

	.end
