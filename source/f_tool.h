/********************************************************************/
/*                     f_tool.h                                     */
/*                   �t�H���g�֘A�c�[��                             */
/*                       by tomo                                    */
/********************************************************************/
/*==================================================================*/
/*             ���b�Z�[�W�n�\����                                   */
/*==================================================================*/
typedef struct{
	const char	*text;		/* TEXT�ް��A�h���X*/

	u8		col_f;  		/* �F�ԍ�(����) */
	u8		col_b;			/* �F�ԍ�(�w�i) */

	u8		status;			/* �X�e�[�^�X */

	u32		cgx_adrs;		/* ���ۂɏ������ރA�h���X(CGX) */
	u8		cgx_block;		/* ��׸���ް���ۯ�(cbb)(���Œ�)*/
	u16		cgx_ofs;		/* CGX�A�h���X��TOP����̵̾��*/
	u16		cgx_p;			/* ofs����̈ړ�����(�ρj*/
	u8		*cgx_area;		/* CGX���������ރG���A�̐擪���ڽ*/

	u32		screen_adrs;	/* ���ۂɏ������ރA�h���X(CGX) */
	u8		screen_block;	/* ��ذ��ް���ۯ�(sbb)(���Œ�)*/
	u16		screen_ofs;		/* SCREEN(CELL)��TOP����̵̾��*/
	u16		screen_p;		/* ofs����̈ړ�����(�ρj*/
	u8		screen_x,screen_y;	/* �\���ʒu(���Œ�) */

	u16		text_p;			/* �ǂݏo���e�L�X�g�ʒu */

	u8		cnt;			/* �\���E�G�C�g�̃J�E���g*/
	u8		spd;			/* �\���E�G�C�g0:�Ȃ� */
	u8		flg;			/* �\������׸� */

	u8		code1,code2;	/* ���s���̃R�[�h�A�p�����[�^*/

}MES;

/*==================================================================*/
/*==================================================================*/
#define H_CHAR (32)						/*�P�s�̃L������*/
#define FONT_SIZE	(8*8/2)				/*�t�H���g�̃T�C�Y�i�o�C�g���j*/
#define FONT2_SIZE		(FONT_SIZE*2)	/*���ݎg�p���Ă���t�H���g�̃o�C�g��*/
#define B_FONT_SIZE	(8)					/*�t�H���g�̈��k���̃T�C�Y*/
#define B_LINE_SIZE	(B_FONT_SIZE*32)	/*�P�s�̈��k���̃T�C�Y*/

#define FONT_CGX_BLOCK		3				/*�t�H���g�\���pCGX�G���A*/
#define FONT_SCREEN_BLOCK	31				/*�t�H���g�\���p��ذ݃G���A*/

#define MES_SPD_CODE		'S'				/*�X�s�[�h*/
#define MES_TRG_CODE		'T'				/*�g���K�[*/
#define MES_GOBI_CODE		'G'				/*���*/
#define MES_KOMMANDO_CODE	'K'				/*�R�}���h�\��*/
#define MES_END_CODE		'$'				/*���b�Z�[�W�I��*/
#define MES_X_CODE			'X'				/*X�\���ʒu*/
#define MES_Y_CODE			'Y'				/*X�\���ʒu*/
#define MES_B_CODE			'B'				/*BG�F*/
#define MES_C_CODE			'C'				/*TEXT�F*/
#define MES_SPACE_CODE		'>'				/*��*/
#define MES_RET_CODE		'\\'			/*���s*/
#define MES_WAIT_CODE		'W'				/*�O�����۰ٗp�E�G�C�g���[�h*/
#define MES_LIST_CODE		'L'				/**/
#define MES_V_CODE			'V'				/*���l*/

#define MES_WAIT			MES_SPD3		/*1�����\���̃E�G�C�g*/
#define MES_SPD1			0				/*�ő��E�G�C�g�Ȃ�*/
#define MES_SPD2			4				/*����*/
#define MES_SPD3			8				/*�ʏ�̑��x*/
#define MES_SPD4			16				/*�ᑬ*/
#define MES_SPD5			32				/*�ݑ�*/

/*�F�̎w��*/
#define COL_NUKE   0x00
#define COL_BLACK  0x01
#define COL_RED    0x02
#define COL_GREEN  0x03
#define COL_BLUE   0x04
#define COL_YELLOW 0x05
#define COL_LBLUE  0x06
#define COL_PURPLE 0x07
#define COL_DGRAY  0x08
#define COL_LGRAY  0x09

#define COL_WHITE  0x0f
/*==================================================================*/
