//=========================================================================
//	
//	�|�P�����}��
//
//=========================================================================
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "task.h"
#include "print.h"
#include "poketool.h"
#include "decord.h"
#include "objdata.h"
#include "menu.h"
#include "window.h"
#include "fld_eff.h"
#include "calctool.h"
#include "z_naki.h"
#include "mus_tool.h"
#include "fight.h"
#include "b_return.h"			//add by soga 2002.05.10
#include "config.h"
#include "monsno.def"
#include "ev_flag.h"
#include "laster.h"

#include "zukan.dat"
#include "aiueo.dat"
#include "z_omosa.dat"
#include "z_takasa.dat"

#define	 __ZUKAN_H_GLOBAL__
#include "zukan.h"

extern	void	FieldMenuRecover(void);
extern	void	affineWorkSet(u8 num,u16 H_X,u16 V_X,u16 H_Y,u16 V_Y);
extern	u16		pp_rand(void);

static	void	VInterFunc( void );
static	u8		ZukanMainScreenSet(u8 mode);

static	void	ZukanListNumPut(u16 listp,u8 x,u8 y,u16 vp);
static	void	ZukanGetMarkPut(u16 f,u8 x,u8 y,u16 vp);
static	u8		ZukanPokeNamePutList(u16 pokenum,u8 x,u8 y);

static	void	ZukanListPrint(u8 mode,u16 center_num,u16 vp);
static	void	ZukanListLineClear(u8 x,u8 y,u16 vp);

static	void	ZukanScrollInit(u16 p,u16 vp);
static	u8		ZukanScrollList(u8 mode,u8 sp,u8 offs);
static	void	ZukanScrollOffsSet(u8 mode,u16 p);
static	u16		ZukanScrollCheck(u16 p,u16 vp);
static	void	ZukanScrollPokeCenterSet(void);
static	u8		ZukanBackGroundScrollCheck(void);

static	u16		ZukanPokeActSetCheck(u16 p);
static	u16		ZukanPokeScrollActSet(u16 pokenum,s16 x,s16 y);
static	void	CursorObjSet(u8 mode);

static	const	CellData	Cursorcell[];
static	const	PalData		Cursorpal[];
static void		PokeRollWait(actWork* aw);
static void		NoActMove(actWork* aw);
static void		ZukanActMove(actWork* aw);
static void		PokeMove(actWork* aw);
static void		CursorActMove(actWork* aw);

static void		MeterActMove(actWork* aw);
static void		KeyUDActMove(actWork* aw);
static void		KeySELSTAActMove(actWork* aw);
static void		DialActMove(actWork* aw);

static	void	ZukanMode0InitTask(u8 id);
static	void	ZukanMode0KeyTask(u8 id);
static	void	ZukanMode0ScrollTask(u8 id);
static	void	ZukanMode0SubMenuKeyTask(u8 id);
static	void	ZukanMode0DataWaitInitTask(u8 id);
static	void	ZukanMode0DataWaitTask(u8 id);
static	void	ZukanMode0SearchWaitTask(u8 id);
static	void	ZukanMode0ExitTask(u8 id);

static	void	ZukanMode3InitTask(u8 id);
static	void	ZukanMode3KeyTask(u8 id);
static	void	ZukanMode3ScrollTask(u8 id);
static	void	ZukanMode3SubMenuKeyTask(u8 id);
static	void	ZukanMode3DataWaitInitTask(u8 id);
static	void	ZukanMode3DataWaitTask(u8 id);
static	void	ZukanMode3ReturnTask(u8 id);
static	void	ZukanMode3ExitTask(u8 id);

static	u8		ZukanPokeActAllDel(void);

ZUKAN_WORK* zw		= {0};
u16	ZukanListPos	= {0};
u8	ZukanDialTheta	= {0};

u8	ZukanDebugFlag;

#if 0
#define	CELLID_DEBUG	(2000)
#define	PALID_DEBUG		(2000)
static	const CellData	DebugZukanCell[];
static	const actHeader	DebugLumpActHeader;
static	const u8		z_debug_LZ[];
static	const u16		z_debug_Palette[16];
#endif

/********************************************************************/
/*                                                                  */
/*				�f�[�^												*/
/*                                                                  */
/********************************************************************/
/*==================================================================*/
/*			�A�N�^�[												*/
/*==================================================================*/
// OAM�ް� 
//VPos,AffineMode,ObjMode,Mosaic,ColorMode,Shape,
//HPos,AffineParamNo,Size,CharNo,Priority,Pltt,(Affine)
static	const ActOamData MeterOam	= {160,0,0,0,0,0,0,0,0,0,1,0,0};
static	const ActOamData KeyUDOam	= {160,0,0,0,0,1,0,0,0,0,0,0,0};
static	const ActOamData KeyOam		= {160,0,0,0,0,1,0,0,2,0,0,0,0};
static	const ActOamData DialOam	= {160,0,2,0,0,0,0,0,2,0,1,0,0};
static	const ActOamData SeeGetOam	= {160,0,0,0,0,1,0,0,3,0,0,0,0};
static	const ActOamData NumOam		= {160,0,0,0,0,2,0,0,0,0,0,0,0};

// ANM�ް� 
static const actAnm MeterAnm0[]		= {{  3,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm KeyUDAnm0[]		= {{  1,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm DialAnm0[]		= {{ 16,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm KeyAnm0[]		= {{ 48,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm KeyAnm1[]		= {{ 40,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm KeyAnm2[]		= {{ 32,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm KeyAnm3[]		= {{ 56,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm SeeAnm[]		= {{ 64,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm GetAnm[]		= {{ 96,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm0[]		= {{128,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm1[]		= {{130,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm2[]		= {{132,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm3[]		= {{134,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm4[]		= {{136,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm5[]		= {{138,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm6[]		= {{140,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm7[]		= {{142,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm8[]		= {{144,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm NumAnm9[]		= {{146,30, 0, 0},{ACT_ANMEND,0,0,0}};
static const actAnm CursorAnm[]		= {{  4,30, 0, 0},{ACT_ANMEND,0,0,0}};

// ANMð���
static const actAnm* const MeterAnmTbl[]	={MeterAnm0};
static const actAnm* const KeyUDAnmTbl[]	={KeyUDAnm0};
static const actAnm* const DialAnmTbl[]		={DialAnm0};
static const actAnm* const KeyAnmTbl[]		={KeyAnm0,KeyAnm1,KeyAnm2,KeyAnm3};
static const actAnm* const SeeGetAnmTbl[]	={SeeAnm,GetAnm};
static const actAnm* const NumAnmTbl[]		={
NumAnm0,NumAnm1,NumAnm2,NumAnm3,NumAnm4,NumAnm5,NumAnm6,NumAnm7,NumAnm8,NumAnm9
};
static const actAnm* const CursorAnmTbl[]	={CursorAnm};

// ACTORͯ��
static const actHeader MeterActHeader ={
CELLID_ZUKANCURSOR,PALID_ZUKANCURSOR,
&MeterOam,MeterAnmTbl,ACT_NTRANS,DummyActAffTbl,MeterActMove
};

const actHeader KeyUDActHeader ={
CELLID_ZUKANCURSOR,PALID_ZUKANCURSOR,
&KeyUDOam,KeyUDAnmTbl,ACT_NTRANS,DummyActAffTbl,KeyUDActMove
};

static const actHeader KeySELSTAActHeader ={
CELLID_ZUKANCURSOR,PALID_ZUKANCURSOR,
&KeyOam,KeyAnmTbl,ACT_NTRANS,DummyActAffTbl,KeySELSTAActMove
};

static const actHeader DialActHeader ={
CELLID_ZUKANCURSOR,PALID_ZUKANCURSOR,
&DialOam,DialAnmTbl,ACT_NTRANS,DummyActAffTbl,DialActMove
};

static const actHeader SeeGetActHeader ={
CELLID_ZUKANCURSOR,PALID_ZUKANCURSOR,
&SeeGetOam,SeeGetAnmTbl,ACT_NTRANS,DummyActAffTbl,ZukanActMove
};

static const actHeader NumActHeader ={
CELLID_ZUKANCURSOR,PALID_ZUKANCURSOR,
&NumOam,NumAnmTbl,ACT_NTRANS,DummyActAffTbl,ZukanActMove
};

static const actHeader CursorActHeader ={
CELLID_ZUKANCURSOR,PALID_ZUKANCURSOR,
&NumOam,CursorAnmTbl,ACT_NTRANS,DummyActAffTbl,CursorActMove
};

// CELLͯ��
const	CellData  ZukanCell[] = {
	{ zkn_cho_LZ, 0x1300	,CELLID_ZUKANCURSOR },
	{ CELL_END, 0, 0 }
};

// PALͯ��
const	PalData ZukanPal[] = {
	{ zukan1_Palette	,PALID_ZUKANCURSOR },
#if 0
	//�ȉ��A�f�o�b�O
	{ z_debug_Palette,PALID_DEBUG },
	//�ȏ�A�f�o�b�O
#endif
	{ OBJPAL_END, 0 },
};

/*==================================================================*/
/*			�f�[�^�e�[�u��											*/
/*==================================================================*/
static	const	u8	scr_sp_table[] = {SCR_SP,SCR_SP2,SCR_SP3,SCR_SP4,SCR_SP5};
static	const	u8	scr_offs_table[] = {SCR_OFFS,SCR_OFFS2,SCR_OFFS3,SCR_OFFS4,SCR_OFFS5};





/********************************************************************/
/*                                                                  */
/*				���[�N�������i�Q�[���J�n���ɌĂ΂��j				*/
/*                                                                  */
/********************************************************************/
void	ZukanFirstInit(void)
{
	u16	i;

	ZukanListPos	= 0;
	ZukanDialTheta	= DIAL_DEFAULT; 

	ZukanDebugFlag	= 0;

	MyData.Zukan.ListMode			= 0;	//�}�Ӄ��[�h ������
	MyData.Zukan.ListSort			= 0;	//�}�ӕ��� ������
	MyData.Zukan.ZenkokuZukanFlag	= 0;	//�S���}�Ӄ��[�h���t���O�P ������
	MyData.Zukan.dummy				= 0;	//�\�� ������
	MyData.Zukan.UnknownRand		= 0;	//�����X�^�[�������ۑ��u�A���m�[���v ������
	MyData.Zukan.PachiRand			= 0;	//�����X�^�[�������ۑ��u�p�b�`�[���v ������
	MyData.Zukan.Rand1				= 0;	//�����X�^�[�������ۑ��\�� ������
	ZukanSpecialFlagInit();					//�S���}�Ӄ��[�h���t���O�Q�`�R ������

	for(i=0;i<POKE_ZUKAN_LEN;i++){
		MyData.Zukan.zukan_get_flag[i]	= 0;	//�|�P�����߂܂����t���O
		MyData.Zukan.zukan_see_flag[i]	= 0;	//�|�P�����݂����t���O���̂P
		Fld.zukan_see_flag2[i]			= 0;	//�|�P�����݂����t���O���̂Q
		Fld.zukan_see_flag3[i]			= 0;	//�|�P�����݂����t���O���̂R
	}
}

void	ZukanWorkInit(void)
{
	ZukanListPos	= 0;
	ZukanDialTheta	= DIAL_DEFAULT; 
}




/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static void VInterFunc( void )
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
static void ZworkInit(ZUKAN_WORK* w)
{
	u16	i;

	for(i=0;i<POKE_NUM_MAX;i++){
		w->listWork[i].num = LISTOVER;
		w->listWork[i].look = 0;
		w->listWork[i].get = 0;
	}
	w->monsData.num = 0;
	w->monsData.look = 0;
	w->monsData.get = 0;

	w->listMax	=0;
	w->list_p	=0;
	w->list_p_bak	=0;
	w->list_mode	=0;
	w->list_mode_backup	=0;
	w->list_sort	=0;
	w->list_sort_backup	=0;
	w->see_num	=0;
	w->get_num	=0;

	for(i=0;i<4;i++)	w->poke_str[i] = POKE_NULL;

	w->dial_sp	=0;
	w->dial_bak	=0;
	w->dial_theta	=0;

	w->scroll_def	=0;
	w->scroll_count	=0;
	w->scroll_dir	=0;
	w->scroll_line	=0;
	w->scroll_line_work	=0;
	w->scr_sp	=0;
	w->scr_offs	=0;
	w->list_scr_count	=0;

	for(i=0;i<4;i++)	w->work[i] = 0;

	w->mode	=0;
	w->mode_backup	=0;

	w->back_col	=0;
	w->menu_p	=0;

	w->DataModeRetFlag	=0;

	w->submenu_mode	=0;
	w->submenu_p	=0;
	w->submenu_BGoffs	=0;

	for(i=0;i<8;i++)	w->Dummywork1[i] = 0;
	for(i=0;i<8;i++)	w->Dummywork2[i] = 0;
}


/*==================================================================*/
void	ZukanInit(void)
{
	switch(sys.InitSeqWork){
	default:
	case 0:
		SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

		ZukanScreenRegInit(0);			//�\�����W�X�^������

		DIV_DMACLEAR(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
		DIV_DMACLEAR(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
		DIV_DMACLEAR(3, 0, PLTT, PLTT_SIZE,           16);	// ��گ�  ������

		sys.InitSeqWork = 1;
		break;
	case 1:
		LasterInit();									// HDMA������
		InitTask();										// ���  ������
		InitActor();									// ���� ������
		PaletteAnimeInit();
		ObjPalManInit();								// ��޼ު��گ��Ȱ�ެ������
		ObjPalManStart = 8;								// ��گ��Ȱ�ެNo�J�n
		PrintScreenInit(ZUKANLIST_TYPE);				// �}�Ӄ��X�g�p(CGX_B = 2,SCR_B = 31)
		NMenuScreenInit(ZUKANLIST_TYPE);				// �}�Ӊ�ʗp(���p)

		sys.InitSeqWork++;
		break;
	case 2:
		//���[�N�|�C���^�ݒ�i�f�[�^�������肸�炭���邽�߁A�ʒu�����炷�j
		switch(pp_rand()&0x0003){
		default:
		case 0:
			zw =(ZUKAN_WORK*)(&UserWork[0x18000]);
			break;
		case 1:
			zw =(ZUKAN_WORK*)(&UserWork[0x18800]);
			break;
		case 2:
			zw =(ZUKAN_WORK*)(&UserWork[0x19000]);
			break;
		case 3:
			zw =(ZUKAN_WORK*)(&UserWork[0x19800]);
			break;
		}
		ZworkInit(zw);
		AddTask(ZukanMode0InitTask, 0);

		zw->list_mode	= MyData.Zukan.ListMode;
		if(!(ZukanSpecialFlagCheck()))	zw->list_mode = ZLIST_NEW;//�ă`�F�b�N
		zw->list_sort	= MyData.Zukan.ListSort;
		zw->list_p		= ZukanListPos;
		zw->dial_theta	= ZukanDialTheta;
		zw->menu_p = 0;
		if(!ZukanSpecialFlagCheck()){
			//�S���}�ӂ݂�Ȃ�
			zw->see_num		= ZukanHouenTotal(Z_SEE_CHK);
			zw->get_num		= ZukanHouenTotal(Z_GET_CHK);
		}else{
			zw->see_num		= ZukanTotal(Z_SEE_CHK);
			zw->get_num		= ZukanTotal(Z_GET_CHK);
		}
		zw->scroll_def	= LIST_SCR_Y;

		sys.InitSeqWork++;
		break;
	case 3:
		REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
		*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

		SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
		MainProcChange(ZukanMain);
		
		ZukanListMake(zw->list_mode,zw->list_sort);
		m4aMPlayVolumeControl(&m4a_mplay000,0xffff,256/2);
		break;
	}
}


/*==================================================================*/
//���A�֐��i�}�Ӄf�[�^���A���v���Z�X����:���j���[����j
u8		ZukanDataReturn(pTaskFunc TaskAdrs)
{
	u8	id;

	SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

	ZukanScreenRegInit(DISP_BG1_ON);	//�\�����W�X�^������

	LasterInit();						// HDMA������
	InitTask();							// ���  ������
	PaletteAnimeInit();

	REG_IE_SET( V_BLANK_INTR_FLAG );	// VBLANK��������
	*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(ZukanMain);
	if(zw->mode_backup == Z_MODE0){
		id = AddTask(ZukanMode0DataWaitTask, 0);
	}else{
		id = AddTask(ZukanMode3DataWaitTask, 0);
	}
	TaskTable[id].work[0] = AddTask(TaskAdrs, 0);

	return TaskTable[id].work[0];
}





/********************************************************************/
/*                                                                  */
/*				���C��												*/
/*                                                                  */
/********************************************************************/
void	ZukanMain(void)
{
		MainTask();
		JumpActor();
		SetActor();
		PaletteAnime();
}





/********************************************************************/
/*                                                                  */
/*				���C������											*/
/*                                                                  */
/********************************************************************/
extern	u8		ZukanSearchTaskSet(void);
/*==================================================================*/
/*                                                                  */
/*				���C�����X�g��ʑ���								*/
/*                                                                  */
/*==================================================================*/
static	void	ZukanMode0InitTask(u8 id)
{
	zw->back_col = ZUKANCOL_BLUE;
	if(!ZukanMainScreenSet(Z_MODE0))	return;

	TaskTable[id].TaskAdrs = ZukanMode0KeyTask;
}

/*==================================================================*/
static	void	ZukanMode0KeyTask(u8 id)
{
	u32	mask;

	*(vu16 *)REG_BG0VOFS = zw->submenu_BGoffs;
	if(zw->submenu_BGoffs != 0){
		zw->submenu_BGoffs -= 8;
		return;
	}
	//�`�{�^������(�|�P�����f�[�^��ʂ�)
	if((sys.Trg & A_BUTTON)&&(zw->listWork[zw->list_p].look)){

		//�|�P�����ȊO�̃t�F�[�h�A�E�g�ݒ�
		ZukanScrollPokeCenterSet();
		mask = 0x00000001 << (16+ActWork[zw->poke_str_center].oamData.Pltt);
		PaletteFadeReq((0xffffffff^mask),0,0,16,0);
		ActWork[zw->poke_str_center].move = PokeMove;

		TaskTable[id].TaskAdrs = ZukanMode0DataWaitInitTask;
		SePlay(SE_PIN);
		return;
	}
	//�r�s�`�q�s�{�^������(�T�u���j���[��ʂ�)
	if(sys.Trg & START_BUTTON){
		zw->submenu_BGoffs = 0;
		zw->submenu_mode = 1;
		zw->submenu_p = 0;
		TaskTable[id].TaskAdrs	= ZukanMode0SubMenuKeyTask;
		SePlay(SE_SELECT);
		return;
	}
	//�r�d�k�d�b�s�{�^������(�|�P����������ʂ�)
	if(sys.Trg & SELECT_BUTTON){
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].work[0]	= ZukanSearchTaskSet();
		zw->DataModeRetFlag		= 0;
		zw->dial_bak			= zw->dial_theta;
		zw->list_p_bak			= zw->list_p;
		zw->list_mode_backup	= zw->list_mode;
		zw->list_sort_backup	= zw->list_sort;
		TaskTable[id].TaskAdrs	= ZukanMode0SearchWaitTask;
		SePlay(SE_PC_LOGIN);
		return;
	}
	//�a�{�^������(�}�ӏI��)
	if(sys.Trg & B_BUTTON){
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs = ZukanMode0ExitTask;
		SePlay(SE_PC_OFF);
		return;
	}
	//�X�N���[������(�t���c�L�[)
	zw->list_p = ZukanScrollCheck(zw->list_p,MODE0_BG2_SCRVRAM);
	if(zw->scroll_count)	TaskTable[id].TaskAdrs = ZukanMode0ScrollTask;
}

/*==================================================================*/
static	void	ZukanMode0ScrollTask(u8 id)
{
	if(ZukanScrollList(zw->scroll_dir,zw->scr_sp,zw->scr_offs)){
		TaskTable[id].TaskAdrs = ZukanMode0KeyTask;
	}
}

/*==================================================================*/
static	void	ZukanMode0SubMenuKeyTask(u8 id)
{
	*(vu16 *)REG_BG0VOFS = zw->submenu_BGoffs;
	if(zw->submenu_BGoffs != 80){
		zw->submenu_BGoffs += 8;
		return;
	}
	//�`�{�^������
	if(sys.Trg & A_BUTTON){
		switch(zw->submenu_p){
		default:
		case 0:
			//���C�����X�g��ʂ�
			sys.Trg |= START_BUTTON;
			break;
		case 1:
			//���X�g�擪��
			zw->list_p = 0;
			zw->dial_theta = DIAL_DEFAULT;
			ZukanPokeActAllDel();
			ZukanScrollInit(zw->list_p,MODE0_BG2_SCRVRAM);
			sys.Trg |= START_BUTTON;
			break;
		case 2:
			//���X�g�Ō��
			zw->list_p = zw->listMax-1;
			zw->dial_theta = DIAL_DEFAULT + (DIAL_OFFS *(zw->listMax-1));
			ZukanPokeActAllDel();
			ZukanScrollInit(zw->list_p,MODE0_BG2_SCRVRAM);
			sys.Trg |= START_BUTTON;
			break;
		case 3:
			PaletteFadeReq(0xffffffff,0,0,16,0);
			TaskTable[id].TaskAdrs = ZukanMode0ExitTask;
			SePlay(SE_PC_OFF);
			break;
		}
	}
	//�a���r�s�`�q�s�{�^������(���C�����X�g��ʂ�)
	if(sys.Trg & (START_BUTTON | B_BUTTON)){
		zw->submenu_mode = 0;
		TaskTable[id].TaskAdrs	= ZukanMode0KeyTask;
		SePlay(SE_SELECT);
		return;
	}
	//�t���c�L�[����
	if((sys.Repeat & U_KEY)&&(zw->submenu_p > 0)){
		zw->submenu_p--;
		SePlay(SE_SELECT);
		return;
	}
	if((sys.Repeat & D_KEY)&&(zw->submenu_p < 4-1)){
		zw->submenu_p++;
		SePlay(SE_SELECT);
		return;
	}
}

/*==================================================================*/
//�f�[�^��ʒ�
static	void	ZukanMode0DataWaitInitTask(u8 id)
{
 	if((ActWork[zw->poke_str_center].x != MODE1_POKE_POS_X)
		||(ActWork[zw->poke_str_center].y != MODE1_POKE_POS_Y))	return;

	zw->mode_backup = zw->mode;

	TaskTable[id].work[0] = 
			ZukanDataTaskSet(&(zw->listWork[zw->list_p]),zw->poke_str_center);
	TaskTable[id].TaskAdrs = ZukanMode0DataWaitTask;
}

/*==================================================================*/
extern	u8		ZukanDataTaskChangeCheck(u8 id);
//�f�[�^��ʒ�����
static	void	ZukanMode0DataWaitTask(u8 id)
{
	//�f�[�^��ʒ����씻��
	if(TaskTable[TaskTable[id].work[0]].occ){

		if((zw->mode == Z_MODE1)&&(!ZukanDataTaskChangeCheck(TaskTable[id].work[0]))){
			if(ZukanBackGroundScrollCheck()){
				ZukanDataTaskChange(&(zw->listWork[zw->list_p]),TaskTable[id].work[0]);
			}
		}
	}else{
		//�f�[�^��ʏI��
		ZukanListPos	= zw->list_p;
		ZukanDialTheta	= zw->dial_theta;
		TaskTable[id].TaskAdrs = ZukanMode0InitTask;
	}
}

/*==================================================================*/
//������ʒ�
static	void	ZukanMode0SearchWaitTask(u8 id)
{
	//������ʒ����씻��
	if(!TaskTable[TaskTable[id].work[0]].occ){
		//������ʏI��
		if(zw->DataModeRetFlag){
			zw->list_p				= 0;
			zw->dial_theta			= DIAL_DEFAULT;
			TaskTable[id].TaskAdrs	= ZukanMode3InitTask;
		}else{
			zw->dial_theta			= zw->dial_bak;
			zw->list_p				= zw->list_p_bak;
			zw->list_mode			= zw->list_mode_backup;
			if(!(ZukanSpecialFlagCheck()))	zw->list_mode = ZLIST_NEW;//�ă`�F�b�N
			zw->list_sort			= zw->list_sort_backup;
			TaskTable[id].TaskAdrs	= ZukanMode0InitTask;
		}
	}
}

/*==================================================================*/
static	void	ZukanMode0ExitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	MyData.Zukan.ListMode	= zw->list_mode;
	if(!(ZukanSpecialFlagCheck()))	MyData.Zukan.ListMode = ZLIST_NEW;//�ă`�F�b�N
	MyData.Zukan.ListSort	= zw->list_sort;

	DelTask(id);
	MainProcChange( FieldMenuRecover );
	m4aMPlayVolumeControl(&m4a_mplay000,0xffff,256);
}





/*==================================================================*/
/*                                                                  */
/*				�������ʉ�ʑ���									*/
/*				(��{��MODE0�Ɠ���)                                 */
/*==================================================================*/
static	void	ZukanMode3InitTask(u8 id)
{
	zw->back_col = ZUKANCOL_RED;
	if(!ZukanMainScreenSet(Z_MODE3))	return;

	TaskTable[id].TaskAdrs = ZukanMode3KeyTask;
}

/*==================================================================*/
static	void	ZukanMode3KeyTask(u8 id)
{
	u32	mask;

	*(vu16 *)REG_BG0VOFS = zw->submenu_BGoffs;
	if(zw->submenu_BGoffs != 0){
		zw->submenu_BGoffs -= 8;
		return;
	}
	//�`�{�^������(�|�P�����f�[�^��ʂ�)
	if((sys.Trg & A_BUTTON)&&(zw->listWork[zw->list_p].look)){
		//�|�P�����ȊO�̃t�F�[�h�A�E�g�ݒ�
		ZukanScrollPokeCenterSet();
		mask = 0x00000001 << (16+ActWork[zw->poke_str_center].oamData.Pltt);
		ActWork[zw->poke_str_center].move = PokeMove;
		PaletteFadeReq((0xffffffff^mask),0,0,16,0);

		TaskTable[id].TaskAdrs = ZukanMode3DataWaitInitTask;
		SePlay(SE_PIN);
		return;
	}
	//�r�s�`�q�s�{�^������(�T�u���j���[��ʂ�)
	if(sys.Trg & START_BUTTON){
		zw->submenu_BGoffs = 0;
		zw->submenu_mode = 1;
		zw->submenu_p = 0;
		TaskTable[id].TaskAdrs	= ZukanMode3SubMenuKeyTask;
		SePlay(SE_SELECT);
		return;
	}
	//�r�d�k�d�b�s�{�^������(�|�P����������ʂ�)
	if(sys.Trg & SELECT_BUTTON){
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].work[0]	= ZukanSearchTaskSet();
		zw->DataModeRetFlag		= 0;
		TaskTable[id].TaskAdrs	= ZukanMode0SearchWaitTask;
		SePlay(SE_PC_LOGIN);
		return;
	}
	//�a�{�^������(�������ʐ}�ӏI��)
	if(sys.Trg & B_BUTTON){
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs = ZukanMode3ReturnTask;
		SePlay(SE_PC_OFF);
		return;
	}
	//�X�N���[������(�t���c�L�[)
	zw->list_p = ZukanScrollCheck(zw->list_p,MODE3_BG2_SCRVRAM);
	if(zw->scroll_count)	TaskTable[id].TaskAdrs = ZukanMode3ScrollTask;
}

/*==================================================================*/
static	void	ZukanMode3ScrollTask(u8 id)
{
	if(ZukanScrollList(zw->scroll_dir,zw->scr_sp,zw->scr_offs)){
		TaskTable[id].TaskAdrs = ZukanMode3KeyTask;
	}
}

/*==================================================================*/
static	void	ZukanMode3SubMenuKeyTask(u8 id)
{
	*(vu16 *)REG_BG0VOFS = zw->submenu_BGoffs;
	if(zw->submenu_BGoffs != 96){
		zw->submenu_BGoffs += 8;
		return;
	}
	//�`�{�^������
	if(sys.Trg & A_BUTTON){
		switch(zw->submenu_p){
		default:
		case 0:
			//���C�����X�g��ʂ�
			sys.Trg |= START_BUTTON;
			break;
		case 1:
			//���X�g�擪��
			zw->list_p = 0;
			zw->dial_theta = DIAL_DEFAULT;
			ZukanPokeActAllDel();
			ZukanScrollInit(zw->list_p,MODE0_BG2_SCRVRAM);
			sys.Trg |= START_BUTTON;
			break;
		case 2:
			//���X�g�Ō��
			zw->list_p = zw->listMax-1;
			zw->dial_theta = DIAL_DEFAULT + (DIAL_OFFS *(zw->listMax-1));
			ZukanPokeActAllDel();
			ZukanScrollInit(zw->list_p,MODE0_BG2_SCRVRAM);
			sys.Trg |= START_BUTTON;
			break;
		case 3:
			PaletteFadeReq(0xffffffff,0,0,16,0);
			TaskTable[id].TaskAdrs = ZukanMode3ReturnTask;
			SePlay(SE_TRACK_DOOR);
			break;
		case 4:
			PaletteFadeReq(0xffffffff,0,0,16,0);
			TaskTable[id].TaskAdrs = ZukanMode3ExitTask;
			SePlay(SE_PC_OFF);
			break;
		}
	}
	//�a���r�s�`�q�s�{�^������(���C�����X�g��ʂ�)
	if(sys.Trg & (START_BUTTON | B_BUTTON)){
		zw->submenu_mode = 0;
		TaskTable[id].TaskAdrs	= ZukanMode3KeyTask;
		SePlay(SE_SELECT);
		return;
	}
	//�t���c�L�[����
	if((sys.Repeat & U_KEY)&&(zw->submenu_p > 0)){
		zw->submenu_p--;
		SePlay(SE_SELECT);
		return;
	}
	if((sys.Repeat & D_KEY)&&(zw->submenu_p < 5-1)){
		zw->submenu_p++;
		SePlay(SE_SELECT);
		return;
	}
}

/*==================================================================*/
//�f�[�^��ʒ�
static	void	ZukanMode3DataWaitInitTask(u8 id)
{
 	if((ActWork[zw->poke_str_center].x != MODE1_POKE_POS_X)
		||(ActWork[zw->poke_str_center].y != MODE1_POKE_POS_Y))	return;

	zw->mode_backup = zw->mode;

	TaskTable[id].work[0] = 
			ZukanDataTaskSet(&(zw->listWork[zw->list_p]),zw->poke_str_center);
	TaskTable[id].TaskAdrs = ZukanMode3DataWaitTask;
}

/*==================================================================*/
//�f�[�^��ʒ�����
static	void	ZukanMode3DataWaitTask(u8 id)
#if 0
{
	//�f�[�^��ʒ����씻��
	if(TaskTable[TaskTable[id].work[0]].occ){

		if(zw->mode == Z_MODE1){
			if(!TaskTable[TaskTable[id].work[0]].work[0]&&(ZukanBackGroundScrollCheck())){
				ZukanDataTaskChange(&(zw->listWork[zw->list_p]),TaskTable[id].work[0]);
			}
		}
	}else{
		//�f�[�^��ʏI��
		TaskTable[id].TaskAdrs = ZukanMode3InitTask;
	}
}
#else
{
	//�f�[�^��ʒ����씻��
	if(TaskTable[TaskTable[id].work[0]].occ){

		if((zw->mode == Z_MODE1)&&(!ZukanDataTaskChangeCheck(TaskTable[id].work[0]))){
			if(ZukanBackGroundScrollCheck()){
				ZukanDataTaskChange(&(zw->listWork[zw->list_p]),TaskTable[id].work[0]);
			}
		}
	}else{
		//�f�[�^��ʏI��
		TaskTable[id].TaskAdrs = ZukanMode3InitTask;
	}
}
#endif

/*==================================================================*/
static	void	ZukanMode3ReturnTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	zw->dial_theta			= zw->dial_bak;
	zw->list_p				= zw->list_p_bak;
	zw->list_mode			= zw->list_mode_backup;
	if(!(ZukanSpecialFlagCheck()))	zw->list_mode = ZLIST_NEW;//�ă`�F�b�N
	zw->list_sort			= zw->list_sort_backup;
	TaskTable[id].TaskAdrs	= ZukanMode0InitTask;
}

/*==================================================================*/
static	void	ZukanMode3ExitTask(u8 id)
{
	if(FadeData.fade_sw)	return;

	zw->dial_theta			= zw->dial_bak;
	zw->list_p				= zw->list_p_bak;
	zw->list_mode			= zw->list_mode_backup;
	if(!(ZukanSpecialFlagCheck()))	zw->list_mode = ZLIST_NEW;//�ă`�F�b�N
	zw->list_sort			= zw->list_sort_backup;
	TaskTable[id].TaskAdrs	= ZukanMode0ExitTask;
}





/********************************************************************/
/*                                                                  */
/*				��ʍ\�z											*/
/*                                                                  */
/********************************************************************/
static	u8	ZukanMainScreenSet(u8 mode)
{
	switch(sys.InitSeqWork){

	default:
	case 0:
		if(FadeData.fade_sw)	break;
		SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)
		zw->mode = mode;

		ZukanScreenRegInit(0);	//�\�����W�X�^������
		*(vu16 *)REG_BG2VOFS	= zw->scroll_def;	//ؽĉ��SCROLL�p

		//�f�[�^�]��
		LZ77UnCompVram((void*)zkn_ch_LZ,(void*)(BG_VRAM + 0x0000));	//BG CGX�]��
		LZ77UnCompVram((void*)zkn_sc1_LZ,
					(void*)(BG_VRAM + (MODE0_BG1_SCRVRAM*0x800)));	//BG1 MAP�]��(Ҳ�)
		LZ77UnCompVram((void*)zkn_sc3_LZ,
					(void*)(BG_VRAM + (MODE0_BG3_SCRVRAM*0x800)));	//BG3 MAP�]��(�w�i)
		//�T�u���j���[�f�[�^�]��
		DIV_DMACLEAR(3,0,BG_VRAM + (MODE0_BG0_SCRVRAM*0x800),0x0500,16);	//BG0 MAP�N���A�]��
		if(mode == Z_MODE0){
			LZ77UnCompVram((void*)zkn_sc01_LZ,
					(void*)(BG_VRAM + (MODE0_BG0_SCRVRAM*0x800) + 0x500));	//BG0 MAP�]��(INFO&Ͻ�)
		}else{
			LZ77UnCompVram((void*)zkn_sc02_LZ,
					(void*)(BG_VRAM + (MODE0_BG0_SCRVRAM*0x800) + 0x500));	//BG0 MAP�]��(INFO&Ͻ�)
		}
		//BG PAL�]��
		PaletteAnimeInit();
		if(mode == Z_MODE0){
			zw->back_col = ZUKANCOL_BLUE;
		}else{
			zw->back_col = ZUKANCOL_RED;
		}
		ZukanPaletteSet();

		sys.InitSeqWork = 1;
		break;
	case 1:
		//�n�a�i�֘A������
		InitActor();									// ���� ������
		ObjPalManInit();								// ��޼ު��گ��Ȱ�ެ������
		ObjPalManStart = 8;								// ��گ��Ȱ�ެNo�J�n
		DecordCellSet((CellData*)ZukanCell);
		ObjPalSets((PalData*)ZukanPal);
		CursorObjSet(mode);

		sys.InitSeqWork++;
		break;
	case 2:
		//�\�����[�`���������P
		PrintScreenInit(ZUKANLIST_TYPE);				// �}�Ӄ��X�g�p(CGX_B = 2,SCR_B = 31)
		NMenuScreenInit(ZUKANLIST_TYPE);				// �}�Ӊ�ʗp(���p)
		LZ77UnCompVram((void*)zkn_tch_LZ,
			(void*)(BG_VRAM + MODE0_BG2_CHRVRAM*0x4000 + (0x20*2*0xfe)));//�}�Ӄ��X�g����CGX�]��
		sys.InitSeqWork++;
		break;
	case 3:
		//���X�g�֘A������
		if(mode == Z_MODE0) ZukanListMake(zw->list_mode,zw->list_sort);	//���X�g�č\��
		ZukanScrollInit(zw->list_p,MODE0_BG2_SCRVRAM);
		zw->submenu_mode = 0;
		zw->submenu_BGoffs = 0;

		sys.InitSeqWork++;
		break;
	case 4:
		//�t�F�[�h�ݒ�
		PaletteFadeReq(0xffffffff,0,16,0,0);
		SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)

		sys.InitSeqWork++;
		break;
	case 5:
		//���W�X�^�ݒ�
		*(vu16 *)REG_WININ	= 0x3f3f;
		*(vu16 *)REG_WINOUT	= 0x1d3f;
		*(vu16 *)REG_WIN0H  = 0;
		*(vu16 *)REG_WIN0V  = 0;
		*(vu16 *)REG_WIN1H  = 0;
		*(vu16 *)REG_WIN1V  = 0;
		*(vu16 *)REG_BLDCNT   = 0;
		*(vu16 *)REG_BLDALPHA = 0;
		*(vu16 *)REG_BLDY	  = 0;

		*(vu16 *)REG_BG3CNT = BG_COLOR_16					//16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_3					//BG�D�揇��
							| MODE0_BG3_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
							| MODE0_BG3_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
							;
		*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_1					//BG�D�揇��
							| MODE0_BG1_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
							| MODE0_BG1_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
							;
		*(vu16 *)REG_BG0CNT = BG_COLOR_16					//16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_0					//BG�D�揇��
							| MODE0_BG0_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
							| MODE0_BG0_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
							;
		*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
			                |DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							|DISP_BG3_ON					// BG3�L��
							|DISP_BG2_ON					// BG2�L��
							|DISP_BG1_ON					// BG1�L��
							|DISP_BG0_ON					// BG0�L��
							|DISP_OBJWIN_ON
							|DISP_OBJ_ON;
		sys.InitSeqWork++;
		break;
	case 6:
		if(FadeData.fade_sw)	break;

		sys.InitSeqWork = 0;
		return 1;
	}
	return 0;
}


/*==================================================================*/
void	ZukanPaletteSet(void)
{
	if(zw->back_col == ZUKANCOL_RED){
		PaletteWorkSet(&zukan2_Palette[0x0001],PA_BG0+PA_COL1,(0x060*2)-2);//BG0&BG1&BG3 COL�]��
	}else{
		if(!ZukanSpecialFlagCheck()){
			//�S���}�ӂ݂�Ȃ�
			PaletteWorkSet(&zukan1_Palette[0x0001],PA_BG0+PA_COL1,(0x060*2)-2);//BG0&BG1&BG3 COL�]��
		}else{
			PaletteWorkSet(&zukan3_Palette[0x0001],PA_BG0+PA_COL1,(0x060*2)-2);//BG0&BG1&BG3 COL�]��
		}
	}
}

/*==================================================================*/






/********************************************************************/
/*                                                                  */
/*				�}��ؽĂ̍쐬(�}��bit -> WorkRAM)					*/
/*				���X�g�i���o�[�͑S���}�ӂ��						*/
/*                                                                  */
/********************************************************************/
void	ZukanListMake(u8 mode,u8 sort)
{
	s16	i,j,f;
	u16	work[4];

	zw->listMax = 0;

	//���[�h�ɂ�郊�X�g�쐬�f�[�^
	switch(mode){
	default:
	case ZLIST_NEW: //�z�E�G���}��
		work[0] = NEW_POKE_NUM_MAX;
		work[1] = 1;
		break;
	case ZLIST_WORLD: //�S���}��
		if(ZukanSpecialFlagCheck()){//�ă`�F�b�N
			work[0] = POKE_NUM_MAX;
			work[1] = 0;
		}else{
			work[0] = NEW_POKE_NUM_MAX;
			work[1] = 1;
		}
		break;
	}
	switch(sort){

	case ZSORT_NUM:	//�ԍ���
		if(work[1]){
			//�z�E�G���}��
			for(i=0;i<work[0];i++){
				work[2] = PokeHouenNo2ZukanNoGet(i+1);
				zw->listWork[i].num	= work[2];
				zw->listWork[i].look= ZukanCheck(work[2],Z_SEE_CHK);
				zw->listWork[i].get = ZukanCheck(work[2],Z_GET_CHK);

				if(zw->listWork[i].look) zw->listMax = i+1;
			}
		}else{
			//�S���}��
			for(i=0,j=0,f=0;i<work[0];i++){
				work[2] = i+1;
				if(ZukanCheck(work[2],Z_SEE_CHK))	f=1;
				if(f){
					zw->listWork[j].num	= work[2];
					zw->listWork[j].look= ZukanCheck(work[2],Z_SEE_CHK);
					zw->listWork[j].get = ZukanCheck(work[2],Z_GET_CHK);
					if(zw->listWork[j].look) zw->listMax = j+1;
					j++;
				}
			}
		}
		break;
	case ZSORT_AIU:	//�܏\����
//		for(i=0;i<POKE_NUM_MAX;i++){
		for(i=0;i<KAIHATSU_NUM_MAX;i++){	//��O�I�ɊJ���e�[�u�����܂킷
			work[2] = PokeAiueoSortTable[i];
			if(PokeZukanNo2HouenNoGet(work[2]) <= (work[0])){
				if(ZukanCheck(work[2],Z_SEE_CHK)){
					zw->listWork[zw->listMax].num	= work[2];
					zw->listWork[zw->listMax].look= 1;
					zw->listWork[zw->listMax].get = ZukanCheck(work[2],Z_GET_CHK);
					zw->listMax++;
				}
			}
		}
		break;
	case ZSORT_OMOI://�d����
		for(i=(POKE_NUM_MAX-1);i>=0;i--){
			work[2] = omosa_tbl[i];
			if(PokeZukanNo2HouenNoGet(work[2]) <= (work[0])){
				if(ZukanCheck(work[2],Z_GET_CHK)){
					zw->listWork[zw->listMax].num	= work[2];
					zw->listWork[zw->listMax].look= 1;
					zw->listWork[zw->listMax].get = 1;
					zw->listMax++;
				}
			}
		}
		break;
	case ZSORT_KARUI://�y����
		for(i=0;i<POKE_NUM_MAX;i++){
			work[2] = omosa_tbl[i];
			if(PokeZukanNo2HouenNoGet(work[2]) <= (work[0])){
				if(ZukanCheck(work[2],Z_GET_CHK)){
					zw->listWork[zw->listMax].num	= work[2];
					zw->listWork[zw->listMax].look= 1;
					zw->listWork[zw->listMax].get = 1;
					zw->listMax++;
				}
			}
		}
		break;
	case ZSORT_TAKAI://������
		for(i=(POKE_NUM_MAX-1);i>=0;i--){
			work[2] = takasa_tbl[i];
			if(PokeZukanNo2HouenNoGet(work[2]) <= (work[0])){
				if(ZukanCheck(work[2],Z_GET_CHK)){
					zw->listWork[zw->listMax].num	= work[2];
					zw->listWork[zw->listMax].look= 1;
					zw->listWork[zw->listMax].get = 1;
					zw->listMax++;
				}
			}
		}
		break;
	case ZSORT_HIKUI://�Ⴂ��
		for(i=0;i<POKE_NUM_MAX;i++){
			work[2] = takasa_tbl[i];
			if(PokeZukanNo2HouenNoGet(work[2]) <= (work[0])){
				if(ZukanCheck(work[2],Z_GET_CHK)){
					zw->listWork[zw->listMax].num	= work[2];
					zw->listWork[zw->listMax].look= 1;
					zw->listWork[zw->listMax].get = 1;
					zw->listMax++;
				}
			}
		}
		break;
	}
	for(i=zw->listMax;i<POKE_NUM_MAX;i++){
		zw->listWork[i].num	 = LISTOVER;
		zw->listWork[i].look = 0;
		zw->listWork[i].get  = 0;
	}
}





/********************************************************************/
/*                                                                  */
/*				�\���֘A											*/
/*                                                                  */
/********************************************************************/
/*==================================================================*/
/*				�}��ؽĂ̕\��(WorkRAM -> VRAM)						*/
/*					center_num:ؽĂ̒��S�߹�����ް					*/
/*==================================================================*/
static	void	ZukanListPrint(u8 mode,u16 center_num,u16 vp)
{
	u16	i,line;
	s16	p;

	switch(mode){
	
	default:
	case LIST_PRINT_ALL:
		//��۰ٗ]��( 2ײ� )���ӂ��߂� 11ײ� �\��
		p = center_num - 5;

		for(i=0;i<11;i++){
			if((p < 0)||(p > POKE_NUM_MAX-1)||(zw->listWork[p].num == LISTOVER)){		//��\��
				ZukanListLineClear(17,0+i*2,vp);
			}else{
				ZukanListLineClear(17,0+i*2,vp);
				if(zw->listWork[p].look){
					ZukanListNumPut(p,18,0+i*2,vp);
					ZukanGetMarkPut(zw->listWork[p].get,17,0+i*2,vp);
					ZukanPokeNamePutList(zw->listWork[p].num,23,0+i*2);
				}else{
					ZukanListNumPut(p,18,0+i*2,vp);
					ZukanGetMarkPut(0,17,0+i*2,vp);
					ZukanPokeNamePutList(0,23,0+i*2);	//�u-----�v�\��
				}
			}
			p++;
		}
		break;
	case LIST_PRINT_UPPER:
		p = center_num - 5;

		if((p < 0)||(p > POKE_NUM_MAX-1)||(zw->listWork[p].num == LISTOVER)){		//��\��
			ZukanListLineClear(17,0+ zw->scroll_line*2,vp);
		}else{
			ZukanListLineClear(17,0+ zw->scroll_line*2,vp);
			if(zw->listWork[p].look){
				ZukanListNumPut(p,18,0 + zw->scroll_line*2,vp);
				ZukanGetMarkPut(zw->listWork[p].get,17,0 + zw->scroll_line*2,vp);
				ZukanPokeNamePutList(zw->listWork[p].num,23,0+ zw->scroll_line*2);
			}else{
				ZukanListNumPut(p,18,0 + zw->scroll_line*2,vp);
				ZukanGetMarkPut(0,17,0 + zw->scroll_line*2,vp);
				ZukanPokeNamePutList(0,23,0+ zw->scroll_line*2);	//�u-----�v�\��
			}
		}
		break;
	case LIST_PRINT_LOWER:
		p = center_num + 5;
		line = zw->scroll_line + 10;
		if(line >= 16)	line -=16;	//�␳

		if((p < 0)||(p > POKE_NUM_MAX-1)||(zw->listWork[p].num == LISTOVER)){		//��\��
			ZukanListLineClear(17,0+ line*2,vp);
		}else{
			ZukanListLineClear(17,0+ line*2,vp);
			if(zw->listWork[p].look){
				ZukanListNumPut(p,18,0 + line*2,vp);
				ZukanGetMarkPut(zw->listWork[p].get,17,0 + line*2,vp);
				ZukanPokeNamePutList(zw->listWork[p].num,23,0+ line*2);
			}else{
				ZukanListNumPut(p,18,0 + line*2,vp);
				ZukanGetMarkPut(0,17,0 + line*2,vp);
				ZukanPokeNamePutList(0,23,0+ line*2);	//�u-----�v�\��
			}
		}
		break;
	}
}


/*==================================================================*/
//�}�Ӄ��X�g�i���o�[�̕\��
static	void ZukanListNumPut(u16 listp,u8 x,u8 y,u16 vp)
{
	u8	numstr[4];
	u16	data[2];
	u16	listnum;

	listnum = zw->listWork[listp].num;
		
	if(zw->list_mode == ZLIST_NEW){
		listnum = PokeZukanNo2HouenNoGet(listnum);	//�z�E�G���i���o�[�ϊ�
	}
	data[0] = LIST_NUM_CHR;
	data[1] = LIST_NUM_CHR+1;
	numstr[0] = n0_ +(listnum/100);
	numstr[1] = n0_ +(listnum%100/10);
	numstr[2] = n0_ +(listnum%100%10); 
	numstr[3] = EOM_;

   	*(u16 *)(BG_VRAM + vp*0x800 + y*64 + x*2)		= data[0];
   	*(u16 *)(BG_VRAM + vp*0x800 + (y+1)*64 + x*2)	= data[1];
	NMenuMsgWrite(numstr,x+1,y);
}

/*==================================================================*/
//���܂����}�[�N�̕\��
static	void ZukanGetMarkPut(u16 f,u8 x,u8 y,u16 vp)
{
	u16	data[2];

	if(f){
		data[0] = LIST_BALL_CHR;
		data[1] = LIST_BALL_CHR+1;
	}else{
		data[0] = 0;		//NULL
		data[1] = 0;		//NULL
	}
   	*(u16 *)(BG_VRAM + vp*0x800 + y*64 + x*2)		= data[0];
   	*(u16 *)(BG_VRAM + vp*0x800 + (y+1)*64 + x*2)	= data[1];
}

/*==================================================================*/
//���O�̕\���i�݂��ĂȂ����̂́u-----�v�j
#define	JPN_MONSNAME_LEN	(5)
static	u8 ZukanPokeNamePutList(u16 pokenum,u8 x,u8 y)
{
	u8	i,numstr[MONS_NAME_SIZE+EOM_SIZE];

//	for(i=0;i<MONS_NAME_SIZE;i++) numstr[i] = spc_;
//	numstr[MONS_NAME_SIZE] = EOM_;
	for(i=0;i<JPN_MONSNAME_LEN;i++) numstr[i] = spc_;
	numstr[JPN_MONSNAME_LEN] = EOM_;

	pokenum = PokeZukanNo2MonsNoGet(pokenum);		//�S�����J���i���o�[
	if(pokenum){
		i = 0;
		while((PokeNameData[pokenum][i]!=EOM_)&&(i<MONS_NAME_SIZE)){
			numstr[i] = PokeNameData[pokenum][i];
			i++;
		}
	}else{
//		for(i=0;i<MONS_NAME_SIZE;i++) numstr[i] = bou_;
		for(i=0;i<JPN_MONSNAME_LEN;i++) numstr[i] = bou_;
	}
	NMenuMsgWrite(numstr,x,y);
	return i;
}

/*==================================================================*/
//�s���C������
static	void ZukanListLineClear(u8 x,u8 y,u16 vp)
{
	u8	i;

	for(i=0;i<12;i++){
		*(u16 *)(BG_VRAM + vp*0x800 + (y+0)*64 + (x+i)*2) = 0;	//NULL�L�����N�^�[
		*(u16 *)(BG_VRAM + vp*0x800 + (y+1)*64 + (x+i)*2) = 0;	//NULL�L�����N�^�[
	}
}





/********************************************************************/
/*                                                                  */
/*				��۰ي֘A(ܰ�,�߹��,ؽ�)							*/
/*		p  : �}�Ӄ��X�g�|�W�V����									*/
/*		vp : �e�L�X�g�X�N���[���i���o�[								*/
/********************************************************************/
/*==================================================================*/
//�����������X�g�S�\��
static	void	ZukanScrollInit(u16 p,u16 vp)
{
	u8	i,actnum;
	u16	pokenum;

	for(i=0;i<4;i++){
		zw->poke_str[i] = POKE_NULL;
	}
	zw->poke_str_center = POKE_NULL;

	if((pokenum = ZukanPokeActSetCheck(p-1)) != POKE_NULL){
		actnum = ZukanPokeScrollActSet(pokenum,POKE_DRAM_X,POKE_DRAM_Y);
		ActWork[actnum].move = PokeRollWait;
		ActWork[actnum].work[5] = -POKE_DRAM_H;
	}
	if((pokenum = ZukanPokeActSetCheck(p+0)) != POKE_NULL){
		actnum = ZukanPokeScrollActSet(pokenum,POKE_DRAM_X,POKE_DRAM_Y);
		ActWork[actnum].move = PokeRollWait;
		ActWork[actnum].work[5] = 0;
	}
	if((pokenum = ZukanPokeActSetCheck(p+1)) != POKE_NULL){
		actnum = ZukanPokeScrollActSet(pokenum,POKE_DRAM_X,POKE_DRAM_Y);
		ActWork[actnum].move = PokeRollWait;
		ActWork[actnum].work[5] = POKE_DRAM_H;
	}
	ZukanListPrint(LIST_PRINT_ALL,p,vp);
	*(vu16 *)REG_BG2VOFS	= zw->scroll_def;	//ؽĉ��SCROLL�p
	zw->scroll_line = 0;
	zw->scroll_line_work = 0;
}

/*==================================================================*/
//�X�N���[��������
static	u8	ZukanScrollList(u8 mode,u8 sp,u8 offs)
{
	u16	i;
	u8	scr_offs,ret_flg;

	if(zw->scroll_count){
		zw->scroll_count--;

		switch(mode){

		case	Z_SCR_U:	//ؽĉ��SCROLL UP
			for(i=0;i<4;i++){
				if(zw->poke_str[i]!=POKE_NULL) ActWork[zw->poke_str[i]].work[5]+=sp;
			}
			scr_offs = (offs - zw->scroll_count) * 16/offs;
			*(vu16 *)REG_BG2VOFS = zw->scroll_def + (zw->scroll_line_work*16) - scr_offs;
			zw->dial_theta -= zw->dial_sp;
			break;
		case	Z_SCR_D:	//ؽĉ��SCROLL DOWN
			for(i=0;i<4;i++){
				if(zw->poke_str[i]!=POKE_NULL) ActWork[zw->poke_str[i]].work[5]-=sp;
			}
			scr_offs = (offs - zw->scroll_count) * 16/offs;
			*(vu16 *)REG_BG2VOFS = zw->scroll_def + (zw->scroll_line_work*16) + scr_offs;
			zw->dial_theta += zw->dial_sp;
			break;
		}
		ret_flg = 0;		//��۰ْ�
	}else{
		*(vu16 *)REG_BG2VOFS = zw->scroll_def + (zw->scroll_line*16);	//ؽĉ�� RESET
		ret_flg = 1;		//��۰ُI��
	}
	return ret_flg;
}

/*==================================================================*/
//�X�N���[���J�n����
static	void	ZukanScrollOffsSet(u8 mode,u16 p)
{
	u8	actnum;
	u16	pokenum;

	zw->scroll_line_work = zw->scroll_line;

	switch(mode){

	case	Z_SCR_U:	//SCROLL UP
		if((pokenum = ZukanPokeActSetCheck(p-1)) != POKE_NULL){
			actnum = ZukanPokeScrollActSet(pokenum,POKE_DRAM_X,POKE_DRAM_Y);
			ActWork[actnum].move = PokeRollWait;
			ActWork[actnum].work[5] = -(POKE_DRAM_H*2);
		}
		if(zw->scroll_line > 0){
			zw->scroll_line--;
		}else{
			zw->scroll_line = 16-1;
		}
		break;
	case	Z_SCR_D:	//SCROLL DOWN
		if((pokenum = ZukanPokeActSetCheck(p+1)) != POKE_NULL){
			actnum = ZukanPokeScrollActSet(pokenum,POKE_DRAM_X,POKE_DRAM_Y);
			ActWork[actnum].move = PokeRollWait;
			ActWork[actnum].work[5] = (POKE_DRAM_H*2);
		}
		if(zw->scroll_line < 16-1){
			zw->scroll_line++;
		}else{
			zw->scroll_line = 0;
		}
		break;
	}
}

/*==================================================================*/
//�X�N���[������
static	u16		ZukanScrollCheck(u16 p,u16 vp)
{
	u8	i,scr_f;
	u16	scr_sp,scr_offs,p_bak;

	scr_f	= Z_SCR_NON;

	// U �L�[����(�}�Ӄ��X�g ��X�N���[��)
	if((sys.Cont & U_KEY)&&(p > 0)){
		scr_f	= Z_SCR_U;
		p = Z_NumIncDec(NUM_DEC,p,0,zw->listMax-1);
		ZukanScrollOffsSet(scr_f,p);
		ZukanListPrint(LIST_PRINT_UPPER,p,vp);
		SePlay(SE_Z_SCROLL);
		goto key_next;	//���������o�O���
	}
	// D �L�[����(�}�Ӄ��X�g ���X�N���[��)
	if((sys.Cont & D_KEY)&&(p < zw->listMax-1)){
		scr_f	= Z_SCR_D;
		p = Z_NumIncDec(NUM_INC,p,0,zw->listMax-1);
		ZukanScrollOffsSet(scr_f,p);
		ZukanListPrint(LIST_PRINT_LOWER,p,vp);
		SePlay(SE_Z_SCROLL);
		goto key_next;	//���������o�O���
	}
	// L �L�[����(�}�Ӄ��X�g ��X�L�b�v�X�N���[��)
	if((sys.Trg & L_KEY)&&(p > 0)){
		p_bak = p;
		for(i=0;i<7;i++) p = Z_NumIncDec(NUM_DEC,p,0,zw->listMax-1);
		zw->dial_theta += (DIAL_OFFS *(p - p_bak));
		ZukanPokeActAllDel();
		ZukanScrollInit(p,MODE0_BG2_SCRVRAM);
		SePlay(SE_Z_PAGE);
		goto key_next;	//���������o�O���
	}
	// R �L�[����(�}�Ӄ��X�g ���X�L�b�v�X�N���[��)
	if((sys.Trg & R_KEY)&&(p < zw->listMax-1)){
		p_bak = p;
		for(i=0;i<7;i++) p = Z_NumIncDec(NUM_INC,p,0,zw->listMax-1);
		zw->dial_theta += (DIAL_OFFS *(p - p_bak));
		ZukanPokeActAllDel();
		ZukanScrollInit(p,MODE0_BG2_SCRVRAM);
		SePlay(SE_Z_PAGE);
		goto key_next;	//���������o�O���
	}
key_next:
	if(scr_f == Z_SCR_NON){
		zw->list_scr_count = 0;
	}else{
		scr_sp	= scr_sp_table[zw->list_scr_count/4];
		scr_offs= scr_offs_table[zw->list_scr_count/4];

		zw->scroll_count= scr_offs;
		zw->scr_offs	= scr_offs;
		zw->scr_sp		= scr_sp;
		zw->scroll_dir	= scr_f;
		zw->dial_sp		= scr_sp/2;
		ZukanScrollList(zw->scroll_dir,zw->scr_sp,zw->scr_offs);

		if(zw->list_scr_count < 12)	zw->list_scr_count++;
	}
	return p;
}

/*==================================================================*/
//���S�̃|�P������ݒ�i�f�[�^��ʑI�����p�j
static	void	ZukanScrollPokeCenterSet(void)
{
	u16	i;

	for(i=0;i<4;i++){
		if((ActWork[zw->poke_str[i]].dx == 0)&&(ActWork[zw->poke_str[i]].dy == 0)
			&&(zw->poke_str[i] != POKE_NULL)){
			zw->poke_str_center = zw->poke_str[i];
		}
	}
}

/*==================================================================*/
//���X�g�ړ����菈���i�f�[�^��ʒ��j
static	u8	ZukanBackGroundScrollCheck(void)
{
	u16	p,pw;

	//�t�c�{�^������(�}�Ӄf�[�^�ړ�)
	p = zw->list_p;

	if((sys.Trg & U_KEY)&&(p > 0)){
		pw = p;
		while(pw > 0){
			pw = Z_NumIncDec(NUM_DEC,pw,0,zw->listMax-1);
			if(zw->listWork[pw].look){
				p = pw;
				break;
			}
		}
		if(zw->list_p == p) return 0;
		zw->list_p = p;
		zw->dial_theta -= DIAL_OFFS;

		return 1;
	}
	if((sys.Trg & D_KEY)&&(p < zw->listMax-1)){
		pw = p;
		while(pw < zw->listMax-1){
			pw = Z_NumIncDec(NUM_INC,pw,0,zw->listMax-1);
			if(zw->listWork[pw].look){
				p = pw;
				break;
			}
		}
		if(zw->list_p == p) return 0;
		zw->list_p = p;
		zw->dial_theta += DIAL_OFFS;

		return 1;
	}
	return 0;
}

/*==================================================================*/
//�|�P�����A�N�^�[�S�����i���j���[��ʂ���̂��肩���j
static	u8	ZukanPokeActAllDel(void)
{
	u16	i;

	for(i=0;i<4;i++){
		if(zw->poke_str[i] != POKE_NULL){
			DelActor(&ActWork[zw->poke_str[i]]);
			zw->poke_str[i] = POKE_NULL;
		}
	}
	return 0;
}





/********************************************************************/
/*                                                                  */
/*				�A�N�^�[�ݒ�֘A									*/
/*                                                                  */
/********************************************************************/
/*==================================================================*/
//	�|�P�����\������
static	u16 ZukanPokeActSetCheck(u16 p)
{
	if((p > POKE_NUM_MAX-1)||(zw->listWork[p].num == LISTOVER)) return POKE_NULL;		//ؽ�OVER

	if(zw->listWork[p].look)	return	zw->listWork[p].num;
	else						return	0;	//?ϰ� = �u�݂����ƂȂ��v
}

/*==================================================================*/
//	�|�P�����ݒ�
static	u16 ZukanPokeScrollActSet(u16 pokenum,s16 x,s16 y)
{
	u8	i;
	u8	actnum;

	i = 0;
	while(i<4){
		if(zw->poke_str[i] == POKE_NULL){
			actnum = ZukanPokeActorSet(pokenum,x,y,i);
			ActWork[actnum].oamData.AffineMode = 1;
			ActWork[actnum].oamData.Priority = 3;
			ActWork[actnum].work[0] = 0;
			ActWork[actnum].work[1] = i;
			ActWork[actnum].work[2] = PokeZukanNo2MonsNoGet(pokenum);		//�S�����J���i���o�[
			zw->poke_str[i] = actnum;

			return actnum;
		}
		i++;
	}
	return POKE_NULL;
}

/*==================================================================*/
//	���̑���ʂn�a�i�ݒ�
static	void CursorObjSet(u8 mode)
{
	u16 act_no,f,num;

	act_no = AddActor(&KeyUDActHeader,184,0+4,0);
	ActWork[act_no].work[1] = 0;
	act_no = AddActor(&KeyUDActHeader,184,160-4,0);
	ActWork[act_no].work[1] = 1;
	ActWork[act_no].Vflip = 1;

	act_no = AddActor(&MeterActHeader,228,16+4,0);

	act_no = AddActor(&KeySELSTAActHeader,16,120,0);
	act_no = AddActor(&KeySELSTAActHeader,48,120,0);
	ActAnmChg(&ActWork[act_no],3);
	act_no = AddActor(&KeySELSTAActHeader,16,144,0);
	ActAnmChg(&ActWork[act_no],2);
	ActWork[act_no].work[2] = 0x80;
	act_no = AddActor(&KeySELSTAActHeader,48,144,0);
	ActAnmChg(&ActWork[act_no],1);

	act_no = AddActor(&DialActHeader,0,80,2);
	ActWork[act_no].oamData.AffineMode = 1;
	ActWork[act_no].oamData.AffineParamNo = 30;
	ActWork[act_no].work[0] = 30;
	ActWork[act_no].work[1] = 0;

	act_no = AddActor(&DialActHeader,0,80,2);
	ActWork[act_no].oamData.AffineMode = 1;
	ActWork[act_no].oamData.AffineParamNo = 31;
	ActWork[act_no].work[0] = 31;
	ActWork[act_no].work[1] = 0x80;

	if(mode == Z_MODE0){
		act_no = AddActor(&SeeGetActHeader,32,40,1);
		act_no = AddActor(&SeeGetActHeader,32,72,1);
		ActAnmChg(&ActWork[act_no],1);

		f = 0;
		act_no = AddActor(&NumActHeader,24,48,1);
		num	= zw->see_num/100;
		ActAnmChg(&ActWork[act_no],num);
		if(num)	f =1;	
		else	ActWork[act_no].banish = 1;
		act_no = AddActor(&NumActHeader,32,48,1);
		num = zw->see_num%100/10;
		if(num||f)	ActAnmChg(&ActWork[act_no],num);
		else		ActWork[act_no].banish = 1;
		act_no = AddActor(&NumActHeader,40,48,1);
		num = zw->see_num%100%10;
		ActAnmChg(&ActWork[act_no],num);

		f = 0;
		act_no = AddActor(&NumActHeader,24,80,1);
		num	= zw->get_num/100;
		ActAnmChg(&ActWork[act_no],num);
		if(num)	f =1;	
		else	ActWork[act_no].banish = 1;
		act_no = AddActor(&NumActHeader,32,80,1);
		num = zw->get_num%100/10;
		if(num||f)	ActAnmChg(&ActWork[act_no],num);
		else		ActWork[act_no].banish = 1;
		act_no = AddActor(&NumActHeader,40,80,1);
		num = zw->get_num%100%10;
		ActAnmChg(&ActWork[act_no],num);

		act_no = AddActor(&CursorActHeader,144,96,1);
		ActWork[act_no].banish = 1;
	}else{
		act_no = AddActor(&CursorActHeader,144,80,1);
		ActWork[act_no].banish = 1;
	}
}





/********************************************************************/
/*                                                                  */
/*				�A�N�^�[����֘A									*/
/*                                                                  */
/********************************************************************/
/*==================================================================*/
//	NoActMove(actWork* aw)
//	�A�N�^�[����֐�
/*==================================================================*/
static void		NoActMove(actWork* aw){}

/*==================================================================*/
/*				�ʏ퓮��											*/
/*==================================================================*/
static void		ZukanActMove(actWork* aw)
{
	if(zw->mode != Z_MODE0){
		DelActor(aw);
		return;
	}
}

/*==================================================================*/
/*				�|�P����											*/
/*==================================================================*/
//	�f�[�^��ʈڍs����
static void		PokeMove(actWork* aw)
{
	aw->oamData.Priority	= 0;
	aw->oamData.AffineMode	= 0;
	aw->dx   = 0;
	aw->dy   = 0;

 	if((aw->x != MODE1_POKE_POS_X)||(aw->y != MODE1_POKE_POS_Y)){ 
 		if(aw->x > MODE1_POKE_POS_X)	aw->x--;
 		if(aw->x < MODE1_POKE_POS_X)	aw->x++;
 		if(aw->y > MODE1_POKE_POS_Y)	aw->y--;
 		if(aw->y < MODE1_POKE_POS_Y)	aw->y++;
	}else{
		aw->move = NoActMove;
	}
}

/*==================================================================*/
//	���X�g��]����
static void		PokeRollWait(actWork* aw)
{
	u8	buf_p;
	u32	affine_val;

	buf_p = aw->work[1];

	if((zw->mode != Z_MODE0)&&(zw->mode != Z_MODE3)){
		DelActor(aw);
		zw->poke_str[buf_p] = POKE_NULL;
		return;
	}
	aw->dy = (s16)(sin_cos_table[(u8)(aw->work[5])]*POKE_DRAM_R/0x100);

	affine_val = 0x100*0x100/sin_cos_table[aw->work[5]+0x40];
	if(affine_val > 0xffff)	affine_val = 0xffff;
	affineWorkSet(aw->work[1]+1,0x100,0,0,(affine_val&0xffff));
	aw->oamData.AffineParamNo = buf_p + 1;

	if((aw->work[5] > -(POKE_DRAM_H*2))&&(aw->work[5] < (POKE_DRAM_H*2))){
		aw->banish	= 0;
		aw->work[0] = 1;
	}else{
		aw->banish	= 1;
	}
	if(((aw->work[5] <= -(POKE_DRAM_H*2))||(aw->work[5] >= (POKE_DRAM_H*2)))&&(aw->work[0])){
		DelActor(aw);
		zw->poke_str[buf_p] = POKE_NULL;
	}
}

/*==================================================================*/
/*				���[�^�[											*/
/*==================================================================*/
static void MeterActMove(actWork* aw)
{
	if((zw->mode != Z_MODE0)&&(zw->mode != Z_MODE3)){
		DelActor(aw);
		return;
	}
	aw->dy = zw->list_p * (15*8)/(zw->listMax-1);
}

/*==================================================================*/
/*				�L�[�J�[�\���i�t���c�j								*/
/*==================================================================*/
static void KeyUDActMove(actWork* aw)
{
	u8	theta;

	if((zw->mode != Z_MODE0)&&(zw->mode != Z_MODE3)){
		DelActor(aw);
		return;
	}
	if(aw->work[1]){
		if(zw->list_p == zw->listMax-1){		//������
			aw->banish = 1;
		}else{
			aw->banish = 0;
		}
		theta = aw->work[2];
	}else{
		if(zw->list_p == 0){					//�㔻��
			aw->banish = 1;
		}else{
			aw->banish = 0;
		}
		theta = aw->work[2]+0x80;
	}
	aw->dy = 0x4*sin_cos_table[theta]/0x100;
	aw->work[2]+=8;
	if((!zw->submenu_mode)&&(!zw->submenu_BGoffs)&&(!aw->banish)){
		aw->banish = 0;
	}else{
		aw->banish = 1;
	}
}

/*==================================================================*/
/*				�L�[�J�[�\���i�r�d�k���r�s�`�q�s�j					*/
/*==================================================================*/
static void KeySELSTAActMove(actWork* aw)
{
	if((zw->mode != Z_MODE0)&&(zw->mode != Z_MODE3)){
		DelActor(aw);
		return;
	}
}

/*==================================================================*/
/*				�_�C�A���G�t�F�N�g									*/
/*==================================================================*/
static void DialActMove(actWork* aw)
{
	s16	sin,cos;
	u8	theta;

	if((zw->mode != Z_MODE0)&&(zw->mode != Z_MODE3)){
		DelActor(aw);
		return;
	}
	theta = zw->dial_theta + aw->work[1];
	sin = sin_cos_table[theta];
	cos = sin_cos_table[theta+0x40];
	affineWorkSet(aw->work[0],cos,sin,-sin,cos);

	theta = zw->dial_theta + 0x40 + aw->work[1];
	sin = sin_cos_table[theta];
	cos = sin_cos_table[theta+0x40];
	aw->dx = 0x28*cos/0x100;
	aw->dy = 0x28*sin/0x100;
}

/*==================================================================*/
/*				�J�[�\��											*/
/*==================================================================*/
static void CursorActMove(actWork* aw)
{
	s16	menu_p;

	if((zw->mode != Z_MODE0)&&(zw->mode != Z_MODE3)){
		DelActor(aw);
		return;
	}
	if(zw->mode == Z_MODE0){
		menu_p = 80;
	}else{
		menu_p = 96;
	}
	if((zw->submenu_mode)&&(zw->submenu_BGoffs == menu_p)){
		aw->banish = 0;
		aw->dy = zw->submenu_p * 16;
		aw->dx = 0x4*sin_cos_table[(u8)(aw->work[2])]/0x100;
		aw->work[2]+=8;
	}else{
		aw->banish = 1;
	}
}





























