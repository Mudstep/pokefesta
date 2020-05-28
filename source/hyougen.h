/********************************************************************/
/* File Name : hyougen.h											*/
/* Outline   : �|�P�����d���\���f�[�^								*/
/* Author    : Original (Nori)										*/
/* Date      : 2002.08.14                                           */
/********************************************************************/


static	void	CalcPcount(void) ;
static	void	HyoGenMain(void) ;
static	u8		ArcMoveInit(void) ;
static	u8		ArcMoveMain(void) ;
static	u8		ArcMoveEnd(void) ;
static	void	MoveTblGet(void) ;
static	void	PosOffset(void) ;


#define		LOOP_MAX				512			// �ő僋�[�v�i�J�E���g�j��
#define		HGTBL_PARAM_MAX			10						//HGtbl�̗v�f��
#define		HGWK_PARAM_MAX			HGTBL_PARAM_MAX + 5		//�v�Z�p���[�N�̗v�f��
#define		PAT_TEST				-1

#define		P_ANGLE_INIT		0			// �����p�x
#define		P_ANGLE_ADD			1			// �p�x������
#define		P_SWING_X			2			// �U�蕝X
#define		P_SWING_Y			3			// �U�蕝Y
#define		P_LOOP_CNT			4			// ���[�v��(0:�������Ȃ�,1�`:�v�Z�����)
#define		P_ACCELERATION		5			// �����x
#define		P_END_X				6			// �I�_X(�n�_����̑��Βl)
#define		P_END_Y				7			// �I�_Y(�n�_����̑��Βl)
#define		P_BACK_START_CNT	8			// �����[�v�ڂ���߂���
#define		P_CONTINUE			9			//

#define		P_SIN_MOVE_INIT		10			// �͂��߂�SIN�l(�̾��)�ۑ��p
#define		P_COS_MOVE_INIT		11			// �͂��߂�COS�l(�̾��)�ۑ��p
#define		P_ALL_CNT			12			// �ő�J�E���g���L�^�p(LOOP_CNT�̕ێ��p)
#define		P_START_X			13			// �n�_X(��Βl)��dx
#define		P_START_Y			14			// �n�_Y(��Βl)��dy


actWork		*HGActReg ;					// �A�N�^�[�A�h���X�ۊǗp
actWork		HGActTmp ;					// �A�N�^�[�f�[�^�ۊǗp

pFunc		HGret ;						// �߂�A�h���X(tnavi.c�ŃZ�b�g)�p 
pFunc		*HGsubProc ;				// �R�v���Z�X�ۊǗp

u8			SeqHyoGen ;					// ����Ď��p
u8			HGtp ;						// �ǂݏo���e�[�u���̃|�C���g
u8			HGAtp ;						// �ǂݏo���e�[�u���̃|�C���g

u8			Ka_FF ;
u16			Ka_PMn ;
u8			Ka_Seikaku ;
u8			Ka_act ;
u8			Ka_Mode ;
u16			Ka_Pcount ;					// �p�^�[�����s���̃g�[�^�������J�E���g
u16			pat_c ;

#define		MoveTbl_X	( (s16 *)&UserWork[0x1d000] )				// �S�ړ��f�[�^�i�[�p X
#define		MoveTbl_Y	( (s16 *)&UserWork[0x1d000+LOOP_MAX*2] )	// �S�ړ��f�[�^�i�[�p Y
s16			HGwork[HGWK_PARAM_MAX] ;	// ����p�����[�^�i�[�p


// Renewal


static	const	u8	HGindex[25][2] = {
	{0, 0}, {3, 0}, {4, 1}, {5, 0}, {10, 0},
	{13, 0}, {15, 0}, {16, 2}, {18, 0}, {19, 0},
	{20, 0}, {25, 0}, {27, 3}, {28, 0}, {29, 0},
	{33, 4}, {36, 0}, {37, 0}, {39, 0}, {42, 0},
	{45, 0}, {46, 5}, {47, 6}, {48, 0}, {53, 0}
} ;


static	const	s16		HGtbl[][HGTBL_PARAM_MAX] = {
// No.00(00) ����΂��
	{0, 4, 0, 8, 24, 0, 0, 0, 12, 0},
	{0, 4, 0, 16, 24, 0, 0, 0, 12, 0},
	{0, 4, 0, 32, 32, 0, 0, 0, 16, 1},

// No.01(03) ���݂������
	{0, 3, 6, 0, 48, 0, 0, 0, 24, 1},

// No.02(04) �䂤����
	{64, 16, -24, 0, 32, 0, 0, 0, 0, 1},

// No.03(05) �������ς�
	{0, 4, 8, 0, 16, 0, -8, 0, 0, 0},
	{0, 0, 0, 0, 16, 0, 0, 0, 0, 0},
	{0, 4, 8, 0, 16, 0, -8, 0, 0, 0},
	{0, 0, 0, 0, 16, 0, 0, 0, 0, 0},
	{0, 4, -16, 0, 4, 0, 16, 0, 0, 1},

// No.04(10) ��񂿂�
	{0, 3, 6, 0, 12, 0, 0, 0, 6, 0},
	{0, 3, -6, 0, 12, 0, 0, 0, 6, 0},
	{0, 16, 16, 0, 45, 1, 0, 0, 0, 1},

// No.05(13) ���ԂƂ�
	{0, 16, 0, 24, 32, 0, 0, 0, 16, 0},
	{0, 16, 0, 23, 32, 0, 0, 0, 16, 1},

// No.06(15) ���Ȃ�
	{0, 0, 0, 0, 80, 0, 0, 0, 0, 1},

// No.07(16) �̂�
	{0, 2, 8, 0, 32, 0, 0, 0, 0, 0},
	{0, 2, -8, 0, 32, 0, 0, 0, 0, 1},

// No.08(18) ���ς�
	{0, 32, 2, 1, 48, 1, 0, 0, 24, 1},

// No.09(19) �̂��Ă�
	{0, 2, 16, 16, 128, 0, 0, 0, 0, 1},

// No.10(20) �����т傤
	{0, 2, -8, 0, 48, 0, -24, 0, 0, 0},
	{0, 0, 0, 0, 8, 0, 0, 0, 0, 0},
	{64, 32, 2, 0, 36, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 8, 0, 0, 0, 0, 0},
	{0, 2, 8, 0, 48, 0, 24, 0, 0, 1},

// No.11(25) ��������
	{64, 24, 16, 0, 32, 0, 0, 0, 0, 0},
	{0, 28, 2, 1, 32, 1, 0, 0, 16, 1},

// No.12(27) �܂���
	{0, 0, 0, 0, 32, 0, 0, 0, 0, 1},

// No.13(28) �悤��
	{64, 16, -16, 2, 48, 0, 0, 0, 32, 1},

// No.14(29) �ނ��Ⴋ
	{0, 12, -8, 4, 24, 0, 8, 0, 12, 0},
	{0, 12, 8, 8, 24, 0, -16, 0, 12, 0},
	{0, 12, -8, 16, 24, 0, 16, 0, 12, 0},
	{0, 12, 8, 28, 24, 0, -8, 0, 12, 1},

// No.15(33) �Ђ�����
	{0, 0, 0, 0, 8, 0, 0, 0, 0, 0},
	{64, 16, -4, 0, 32, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 8, 0, 0, 0, 0, 1},

// No.16(36) �����Ƃ�
	{128, 4, 0, 8, 64, 0, 0, 0, 0, 1},

// No.17(37) �ꂢ����
	{0, 2, 16, 0, 48, 0, 0, 0, 0, 0},
	{128, 2, 16, 0, 48, 0, 0, 0, 0, 1},

// No.18(39) �Ă��
	{0, 2, -4, 0, 48, 0, -48, 0, 0, 0},
	{0, 0, 0, 0, 80, 0, 0, 0, 0, 0},
	{0, 2, 8, 0, 24, 0, 48, 0, 0, 1},

// No.19(42) ���������
	{64, 4, 64, 58, 52, 0, -88, 0, 0, 0},
	{0, 0, 0, 0, 80, 0, 0, 0, 0, 0},
	{0, 24, 80, 0, 32, 0, 88, 0, 0, 1},

// No.20(45) �����₩
	{0, 2, 16, 4, 64, 0, 0, 0, 0, 1},

// No.21(46) ���ƂȂ���
	{0, 0, 0, 0, 32, 0, 0, 0, 0, 1},

// No.22(47) �Ȃ܂���
	{0, 0, 0, 0, 42, 0, 0, 0, 0, 1},

// No.23(48) ���񂿂傤
	{0, 4, 0, 8, 24, 0, 0, 0, 12, 0},
	{0, 0, 0, 0, 12, 0, 0, 0, 0, 0},
	{0, 4, 0, 12, 24, 0, 0, 0, 12, 0},
	{0, 0, 0, 0, 12, 0, 0, 0, 0, 0},
	{0, 4, 0, 4, 24, 0, 0, 0, 12, 1},

// No.24(53) ���܂���
	{0, 4, 16, 12, 64, 0, 0, 0, 0, 0},
	{0, -4, 16, 12, 64, 0, 0, 0, 0, 1},
} ;


// ���]
static	const	actAffAnm	Ka_Reverse[] = {
	{-0x0100, 0x0100, 0, ACT_AFSET},
	{ACT_AFEND, 0, 0, 0 }
} ;

// �䂤����
static	const	actAffAnm	Ka_Yuukan[] = {
	{0, 0, 12, 1},
	{0, 0, 0, 30},
	{0, 0, -12, 1},
	{ACT_AFEND, 0, 0, 0}
} ;

static	const	actAffAnm	Ka_YuukanR[] = {
	{-0x0100, 0x0100, 0, ACT_AFSET},
	{0, 0, 12, 1},
	{0, 0, 0, 28},
	{0, 0, -4, 3},
	{ACT_AFEND, 0, 0, 0}
} ;

// �̂�
static	const	actAffAnm	Ka_Nonki[] = {
	{0, 0, 1, 16},
	{0, 0, -1, 32},
	{0, 0, 1, 16},
	{ACT_AFEND, 0, 0, 0}
} ;

static	const	actAffAnm	Ka_NonkiR[] = {
	{-0x0100, 0x0100, 0, ACT_AFSET},
	{0, 0, 1, 16},
	{0, 0, -1, 32},
	{0, 0, 1, 16},
	{ACT_AFEND, 0, 0, 0}
} ;

// �܂���
static	const	actAffAnm	Ka_Majime[] = {
	{0, 0, -1, 8},
	{0, 0, 0, 16},
	{0, 0, 1, 8},
	{ACT_AFEND, 0, 0, 0}
} ;

static	const	actAffAnm	Ka_MajimeR[] = {
	{-0x0100, 0x0100, 0, ACT_AFSET},
	{0, 0, -1, 8},
	{0, 0, 0, 16},
	{0, 0, 1, 8},
	{ACT_AFEND, 0, 0, 0}
} ;

// �Ђ�����
static	const	actAffAnm	Ka_Hikaeme[] = {
	{0, 0, -1, 8},
	{0, 0, 0, 32},
	{0, 0, 1, 8},
	{ACT_AFEND, 0, 0, 0}
} ;

static	const	actAffAnm	Ka_HikaemeR[] = {
	{-0x0100, 0x0100, 0, ACT_AFSET},
	{0, 0, -1, 8},
	{0, 0, 0, 32},
	{0, 0, 1, 8},
	{ACT_AFEND, 0, 0, 0}
} ;

// ���ƂȂ���
static	const	actAffAnm	Ka_Otonashii[] = {
	{0, 0, -1, 4},
	{0, 0, 0, 24},
	{0, 0, 1, 4},
	{ACT_AFEND, 0, 0, 0}
} ;

static	const	actAffAnm	Ka_OtonashiiR[] = {
	{-0x0100, 0x0100, 0, ACT_AFSET},
	{0, 0, -1, 4},
	{0, 0, 0, 24},
	{0, 0, 1, 4},
	{ACT_AFEND, 0, 0, 0}
} ;

// �Ȃ܂���
static	const	actAffAnm	Ka_Namaiki[] = {
	{0, 0, 1, 24},
	{0, 0, 0, 16},
	{0, 0, -12, 2},
	{ACT_AFEND, 0, 0, 0}
} ;

static	const	actAffAnm	Ka_NamaikiR[] = {
	{-0x0100, 0x0100, 0, ACT_AFSET},
	{0, 0, 1, 24},
	{0, 0, 0, 16},
	{0, 0, -12, 2},
	{ACT_AFEND, 0, 0, 0}
} ;


static	const	actAffAnm	*const	HGAnmTbl[] = {
	Ka_Reverse,					// 0 -- ���]
//-------------------------------------- �񔽓]�{����
	Ka_Yuukan,					// 1 -- �䂤����
	Ka_Nonki,					// 2 -- �̂�
	Ka_Majime,					// 3 -- �܂���
	Ka_Hikaeme,					// 4 -- �Ђ�����
	Ka_Otonashii,				// 5 -- ���ƂȂ���
	Ka_Namaiki,					// 6 -- �Ȃ܂���
	Ka_Reverse,					// 7
	Ka_Reverse,					// 8
	Ka_Reverse,					// 9
	Ka_Reverse,					// 10
//-------------------------------------- ���]�{����
	Ka_YuukanR,					// 11 -- �䂤����
	Ka_NonkiR,					// 12 -- �̂�
	Ka_MajimeR,					// 13 -- �܂���
	Ka_HikaemeR,				// 14 -- �Ђ�����
	Ka_OtonashiiR,				// 15 -- ���ƂȂ���
	Ka_NamaikiR,				// 16 -- �Ȃ܂���
	Ka_Reverse,					// 17
	Ka_Reverse,					// 18
	Ka_Reverse,					// 19
	Ka_Reverse,					// 20
} ;






