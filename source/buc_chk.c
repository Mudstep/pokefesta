//=========================================================================
//	
//	�o�b�N�A�b�v�J�[�g���b�W�������
//
//=========================================================================
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "palanm.h"
#include "task.h"
#include "print.h"
#include "poketool.h"
#include "../library/AgbFlash1M.h"
#include "decord.h"
#include "menu.h"
#include "window.h"
#include "calctool.h"
#include "mus_tool.h"
#include "monsno.def"
#include "message.h"

void			BuckUpRAMcheckSet(u8 mode);
static	void	BuckUpRAMcheckInit(void);
static	void	BuckUpRAMcheckMain(void);
static	void	BuckUpRAMcheckTalkExitInit(void);
static	void	BuckUpRAMcheckExitInit(void);
static	void	BuckUpRAMcheckExit(void);

static	u8		BuckUpRAMcheckRecover(u32 sector_bit);

extern	u8		PokeAGBSaveMain(u8 mode);
extern	void	FieldMenuRecover(void);
extern	pFunc	SvRecoverReturnProc;		// �Z�N�^����������ɕ��A�����̊֐��|�C���^
extern	u32		svld_error_sect_bit;		//�Z�N�^�ُ픻��t���O

extern	const u16	opball_Palette[];
extern	const u16	op_bobj_Palette[];
extern	const u16	op_wobj_Palette[];
extern	const u8	opselsc1_LZ[];
extern	const u8	opselsc2_LZ[];
extern	const u8	opselch_LZ[];
extern	const u8	opseloch_LZ[];
extern	const u8	opselwch_LZ[];
const u16			buc_chk_Palette[];
const u8			buc_chk_LZ[];

static void BuckUpRAMcheckITask(void);

//VPos,AffineMode,ObjMode,Mosaic,ColorMode,Shape,
//HPos,AffineParamNo,Size,CharNo,Priority,Pltt,(Affine)
static const SysOamData TimerOam = {160,0,0,0,0,0,0,0,1,0,0,0,0};

extern	const	u8	buc_chk_msg[];
extern	const	u8	buc_chk_NG_msg[];
extern	const	u8	buc_chk_NG2_msg[];
extern	const	u8	buc_chk_retry_msg[];
extern	const	u8	buc_chk_OKreset_msg[];
extern	const	u8	buc_chk_OKreturn_msg[];

u16	BUCchkMode = {0};
u16	BUCchkWork[8] = {0};
/********************************************************************/
/*                                                                  */
/*				�o�b�N�A�b�N�J�[�g���b�W���؃v���Z�X�Z�b�g			*/
/*                                                                  */
/********************************************************************/
void	BuckUpRAMcheckSet(u8 mode)
{
	MainProcChange(BuckUpRAMcheckInit);
	BUCchkMode = mode;
	BUCchkWork[0] = 0;	//�^�C�}�[�G�t�F�N�g�\���t���O
}

/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static	void VInterFunc( void )
{
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}


/********************************************************************/
/*                                                                  */
/*				������												*/
/*                                                                  */
/********************************************************************/
static	void	BuckUpRAMcheckInit(void)
{
	switch(sys.InitSeqWork){
	default:
	case 0:
		SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

		*(vu16 *)REG_DISPCNT	= 0;
		*(vu16 *)REG_BG3CNT		= 0;
		*(vu16 *)REG_BG2CNT		= 0;
		*(vu16 *)REG_BG1CNT		= 0;
		*(vu16 *)REG_BG0CNT		= 0;

		*(vu16 *)REG_BG3HOFS	= 0;
		*(vu16 *)REG_BG3VOFS	= 0;
		*(vu16 *)REG_BG2HOFS	= 0;
		*(vu16 *)REG_BG2VOFS	= 0;
		*(vu16 *)REG_BG1HOFS	= 0;
		*(vu16 *)REG_BG1VOFS	= 0; 
		*(vu16 *)REG_BG0HOFS	= 0;
		*(vu16 *)REG_BG0VOFS	= 0; 

		DmaClear(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
		DmaClear(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
		DmaClear(3, 0, PLTT, PLTT_SIZE,           16);	// ��گ�  ������

		LZ77UnCompVram((void*)opselch_LZ,(void*)(BG_VRAM+0x0000));	//BG  CGX�]��
		LZ77UnCompVram((void*)opselsc1_LZ,(void*)(BG_VRAM+0x3000));	//BG2 MAP�]��(�o�b�O)
		LZ77UnCompVram((void*)opselsc2_LZ,(void*)(BG_VRAM+0x3800));	//BG3 MAP�]��(�w�i)
		LZ77UnCompVram((void*)buc_chk_LZ,(void*)(OBJ_MODE0_VRAM+0x0020));	//OBJ CGX�]��

		InitActor();
		InitTask();	
		PaletteAnimeInit();
		PaletteWorkSet(opball_Palette,PA_BG0,0x40);		//BG2&BG3 COL�]��
		PaletteWorkSet(buc_chk_Palette,PA_OBJ0,0x20);	//OBJ COL�]��

		PrintScreenInit(DEFAULT_TYPE);
		NMenuScreenInit(FIELD_MENU_TYPE);
		NMenuWinBoxWrite(13, 8,16,11);

		NMenuWinBoxWrite( 1,12,28,19);
		NMenuMsgWrite(buc_chk_msg,2,13);

		PaletteFadeReq(0xffffffff,0,16,0,0);

		REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
		*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

		SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)

		*(vu16 *)REG_BG3CNT = BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_PRIORITY_3
						| 7 << BG_SCREEN_BASE_SHIFT | 0 << BG_CHAR_BASE_SHIFT;
		*(vu16 *)REG_BG2CNT = BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_PRIORITY_2
						| 6 << BG_SCREEN_BASE_SHIFT | 0 << BG_CHAR_BASE_SHIFT;
		*(vu16 *)REG_BG0CNT = BG_COLOR_16 | BG_SCREEN_SIZE_0 | BG_PRIORITY_0
						| 31 << BG_SCREEN_BASE_SHIFT | 2 << BG_CHAR_BASE_SHIFT;

		*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
			                  |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							  | DISP_BG3_ON					// BG3�L��
							  | DISP_BG2_ON					// BG2�L��
							  | DISP_BG0_ON					// BG0�L��
							  | DISP_OBJ_ON;
		sys.InitSeqWork++;
		break;
	case 1:
		if(PaletteAnime())	break;

		MainProcChange(BuckUpRAMcheckMain);
		SetVBlankFunc(BuckUpRAMcheckITask);	//�����݂݂̂ŕ\���n�̏��������邽�ߐ؂�ւ�
		break;
	}
}


/********************************************************************/
/*                                                                  */
/*				���C��												*/
/*                                                                  */
/********************************************************************/
#define	RETRY_COUNT	(3)

static	void	BuckUpRAMcheckMain(void)
{
	u8	retry_counter;
	u8	recover_flag;

	retry_counter = 0;
	
	BUCchkWork[0] = 1;//�^�C�}�[�G�t�F�N�g�\���t���O�i�n�m�j

	while((svld_error_sect_bit) && (retry_counter < RETRY_COUNT)){	
		//�Z�N�^�ُ픭�����i���g���C�R��܂ŁE�b��j

		recover_flag = BuckUpRAMcheckRecover(svld_error_sect_bit); 

		if(recover_flag){
			//�Z�N�^���A���s�i�����j
			NMenuWinBoxWrite( 1,12,28,19);
			NMenuMsgWrite(buc_chk_NG_msg,2,13);
			MainProcChange(BuckUpRAMcheckTalkExitInit);
			return;
		}else{
			//�Z�N�^���A�����i�ăZ�[�u�j
			NMenuWinBoxWrite( 1,12,28,19);
			NMenuMsgWrite(buc_chk_retry_msg,2,13);

			PokeAGBSaveMain(BUCchkMode);
			if(svld_error_sect_bit){
				//���g���C���b�Z�[�W
				NMenuMsgWrite(buc_chk_msg,2,13);
			}
		}
		retry_counter++;
	}
	if(retry_counter == RETRY_COUNT){
		//�Z�N�^���A���s�i�����j
		NMenuWinBoxWrite( 1,12,28,19);
		NMenuMsgWrite(buc_chk_NG_msg,2,13);
		MainProcChange(BuckUpRAMcheckExitInit);
	}else{
		//�Z�N�^���A���Z�[�u����
		NMenuWinBoxWrite( 1,12,28,19);
		if(SvRecoverReturnProc == NULL){		// �Z�N�^����������ɕ��A�����̊֐��|�C���^
			//���Z�b�g
			NMenuMsgWrite(buc_chk_OKreset_msg,2,13);
		}else{
			//���^�[��
			NMenuMsgWrite(buc_chk_OKreturn_msg,2,13);
		}
	}
	MainProcChange(BuckUpRAMcheckExitInit);
}


/********************************************************************/
/*                                                                  */
/*				�I��												*/
/*                                                                  */
/********************************************************************/
static	void	BuckUpRAMcheckTalkExitInit(void)
{
	BUCchkWork[0] = 0;//�^�C�}�[�G�t�F�N�g�\���t���O�i�n�e�e�j

	if(sys.Trg & A_BUTTON){
		NMenuWinBoxWrite( 1,12,28,19);
		NMenuMsgWrite(buc_chk_NG2_msg,2,13);
		SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
		MainProcChange(BuckUpRAMcheckExitInit);
	}
}

static	void	BuckUpRAMcheckExitInit(void)
{
	BUCchkWork[0] = 0;//�^�C�}�[�G�t�F�N�g�\���t���O�i�n�e�e�j

	if(sys.Trg & A_BUTTON){
		PaletteFadeReq(0xffffffff,0,0,16,0);
		SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
		MainProcChange(BuckUpRAMcheckExit);
	}
}

static	void	BuckUpRAMcheckExit(void)
{
	if(PaletteAnime())	return;

	if(SvRecoverReturnProc == NULL){		// �Z�N�^����������ɕ��A�����̊֐��|�C���^
		PM_SoftReset();
	}else{
		MainProcChange(SvRecoverReturnProc);
		SvRecoverReturnProc = NULL;		// �Z�N�^����������ɕ��A�����̊֐��|�C���^ ������
	}
}


/********************************************************************/
/*                                                                  */
/*				���C������i�u���荞�ݓ��j							*/
/*                                                                  */
/********************************************************************/
static	const u8	TimerAnmData[][3]={
{0*4+1,0,0},{1*4+1,0,0},{2*4+1,0,0},{1*4+1,0,1},{0*4+1,0,1},{1*4+1,1,1},{2*4+1,1,0},{1*4+1,1,0},
};

static void BuckUpRAMcheckITask(void)
{
	u8	i;
	
	i = ((sys.vsync_counter >> 3)&0x0007);

	sys.OAMbuf[0] = TimerOam;
	sys.OAMbuf[0].HPos = 8*14;
	sys.OAMbuf[0].VPos = 8*9;

	if(BUCchkWork[0]){//�^�C�}�[�G�t�F�N�g�\���t���O
		sys.OAMbuf[0].CharNo= TimerAnmData[i][0];
		sys.OAMbuf[0].AffineParamNo = (TimerAnmData[i][2]<<4)|(TimerAnmData[i][1]<<3);
	}else{
		sys.OAMbuf[0].CharNo= 1;
	}
	CpuFastCopy(sys.OAMbuf, OAM	,4);		//OAM�]��

	if(BUCchkWork[1])	BUCchkWork[1]--;
}


/********************************************************************/
/*                                                                  */
/*				�Z�N�^��������										*/
/*                                                                  */
/********************************************************************/
#define	RECOVER_RETRY_COUNT	(130)
static	u8	BuckUpRAMcheckRecoverCheck(u16 sectnum) 
{
	u32*	rd;
	u16		offs;

	ReadFlash(sectnum,0,(u8*)(UserWork+0x000000),0x1000);
	rd = (u32*)(UserWork+0x000000);

	for(offs=0;offs<0x1000/4;offs++,rd++){
		if(*rd)	return 1;
	}
	return 0;
}

static	u8	BuckUpRAMcheckRecoverMain(u16 sectnum) 
{
	u16	retry_count,recover_flag,offs;

	retry_count		= 0;
	recover_flag	= 1;

	while((recover_flag)&&(retry_count < RECOVER_RETRY_COUNT)){

		//�I�[�o�[�C���[�X��Ԃ̃Z�N�^�ɃC���[�X�����ɂP�o�C�g�f�[�^�i0x00�j����������
		for(offs=0;offs<0x1000;offs++)	ProgramFlashByte(sectnum,offs,0x00);

		//�Z�N�^�S��ɂP�o�C�g�f�[�^�i0x00�j���������݂���Ă��邩�`�F�b�N
		recover_flag = BuckUpRAMcheckRecoverCheck(sectnum);
		retry_count++;
	}
	return recover_flag;	//0:���A�A1:���S
}

static	u8	BuckUpRAMcheckRecover(u32 sector_bit) 
{
	u32	mask;
	u16	sectnum;

	for(sectnum=0;sectnum<32;sectnum++){
		mask = (0x00000001 << sectnum);
		//�ُ픻�肪�o�Ă���Z�N�^�ɕ����������s�Ȃ�
		if(sector_bit & mask){
			if(!(BuckUpRAMcheckRecoverMain(sectnum))){
				//��������
				sector_bit &= (mask^0xffffffff);
			}
		}
	}
	if(!sector_bit)	return 0;	//�S�ُ�Z�N�^��������
	else			return 1;	//�ُ�Z�N�^����
}

/********************************************************************/
/*                                                                  */
/*				�O���t�B�b�N�f�[�^									*/
/*                                                                  */
/********************************************************************/
const u16 buc_chk_Palette[16] = {
0x2E2C,0x0000,0x1CE7,0x3DEF,0x5EF7,0x7FFF,0x151F,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

const u8 buc_chk_LZ[0x90] = {
0x10,0x80,0x01,0x00,

0x0a,0x00,0x00,0x00,0x22,0x00,0x02,0x44,0x00,0x02,0x55,0x02,0x00,0x42,0x55,0x65,
0x20,0x54,0x30,0x03,0x42,0x65,0x55,0x30,0x03,0x00,0x1c,0x00,0x44,0x00,0x04,0x55,
0x00,0x04,0x12,0x56,0x55,0x24,0x00,0x03,0x45,0x02,0x30,0x03,0x55,0x6e,0x24,0x10,
0x03,0x40,0x27,0x55,0x00,0x33,0x20,0x03,0x00,0x43,0x55,0xfc,0x20,0x4b,0x10,0x53,
0x00,0x04,0x10,0x23,0x00,0x1e,0x00,0x03,0x45,0x02,0xbf,0x30,0x03,0x24,0x20,0x4b,
0x10,0x53,0x10,0x5b,0xc0,0x7f,0x60,0x47,0x10,0x53,0xc2,0xd0,0x7f,0x20,0x3b,0x66,
0x45,0x02,0x65,0x00,0x03,0x66,0xbf,0x00,0x8c,0x66,0xf0,0x7f,0xf0,0x7f,0xf0,0x7f,
0xf0,0x7f,0xf0,0x7f,0xf0,0x7f,0xe7,0x50,0x7f,0x60,0x47,0x00,0x7f,0x66,0x56,0xf0,
0x7f,0xc0,0x7f,0x10,0x23,0xc0,0xf0,0x7f,0x70,0x7f,0x00,0x00,
};





