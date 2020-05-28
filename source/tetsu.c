#ifdef	PM_DEBUG
//=========================================================================
//	
//	TETSU.��
//
//=========================================================================
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "task.h"
#include "print.h"
#include "task.h"
#include "poketool.h"
#include "decord.h"
#include "objdata.h"
#include "menu.h"
#include "window.h"
#include "fld_main.h"
#include "mapparam.h"
#include "calctool.h"
#include "item.h"
#include "fight.h"
#include "ev_fight.h"
#include "wazano.def"
#include "server.h"
#include "script.h"
#include "../library/AgbFlash1M.h"
#include "saveload.h"
#include "svld_ini.h"
#include "svld_sys.h"
#include "mus_tool.h"
#include "zukan.h"
#include "laster.h"
#include "pokeicon.h"
#include "fieldmap.h"

//extern	const	u8	TrainerTypeName[][11];
extern	void	InitEncount(void);
extern	void	FieldNormalRecover(void);
extern	const	u32	DecordAdrs[];
extern	const	u32	GrowTbl[][101];
extern	u8		AddPasoPokemon(PokemonParam *PP);

#define	NUM_INC		(0)
#define	NUM_DEC		(1)

static	void	actNoMove(actWork* aw){ }
static	const	CellData	GraChkcell[];
static	const	PalData		GraChkpal[];
static	void	ColDataPut(u16 data,u8 x,u8 y,u8 len);

/********************************************************************/
/*                                                                  */
/*				��{������											*/
/*                                                                  */
/********************************************************************/
static	void	InitRegWork(void)
{
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

    DIV_DMACLEAR(3, 0, VRAM, VRAM_SIZE,           16);	// VRAM   ������
    DIV_DMACLEAR(3, 0, OAM,  OAM_SIZE,            32);	// OAM    ������
    DIV_DMACLEAR(3, 0, PLTT, PLTT_SIZE,           16);	// ��گ�  ������

	PaletteAnimeInit();

	LasterInit();									// HDMA������
	InitTask();										// ���  ������
	InitActor();									// ���� ������
	PaletteAnimeInit();
	ObjPalManInit();								// ��޼ު��گ��Ȱ�ެ������
	CellSet((CellData*)GraChkcell);
	ObjPalSet((PalData*)GraChkpal);
	PrintScreenInit(DEFAULT_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);
}


/********************************************************************/
/*                                                                  */
/*				Ҳ�													*/
/*                                                                  */
/********************************************************************/
//���C������
static	void	MainJob(void)
{
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
}

//�t�F�[�h�����C������
static	void	FadeJob(void)
{
	if(FadeData.fade_sw == 0){
		MainProcChange(MainJob);
		return;
	}
	JumpActor();
	SetActor();
	PaletteAnime();
}

//�I���^�X�N
static	void	ExitTaskSeq(u8 id)
{
	DelTask(id);
	ResetForceEvent();
	MainProcChange(FieldNormalRecover);
}


/********************************************************************/
/*                                                                  */
/*				���荞��											*/
/*                                                                  */
/********************************************************************/
static	void	VInterFunc( void )
{
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}





/********************************************************************/
/*                                                                  */
/*				�T�u���[�`��										*/
/*                                                                  */
/********************************************************************/
//�i���o�[�\��
static	void	NumPut(u16 no,u8 x,u8 y)
{
	u8	numstr[4];
	u8	i,numw,f;

	for(i=0;i<3;i++) numstr[i] = spc_;
	numstr[3] = EOM_;

	f = 0;
	if((numw = no/100)){
		numstr[0] = n0_+numw; 
		f = 1;
	}
	if((numw = no%100/10)||f)	numstr[1] = n0_+numw; 
	numstr[2] = n0_+(no%100%10); 
	NMenuMsgWrite(numstr,x,y);
}

//�|�P�������\��
static	void	PokeNamePut(u16 num,u8 x,u8 y)
{
	u8	numstr[6];
	u8	i;

	for(i=0;i<5;i++) numstr[i] = spc_;
	numstr[5] = EOM_;

	i = 0;
	while((PokeNameData[num][i]!=EOM_)&&(i<5)){
		numstr[i] = PokeNameData[num][i];
		i++;
	}
	NMenuMsgWrite(numstr,x,y);
}

#if 0
//�g���[�i�[���\��
static	void	TrainerNamePut(u16 num,u8 x,u8 y)
{
	u8	numstr[11];
	u8	i;

	for(i=0;i<10;i++) numstr[i] = spc_;
	numstr[10] = EOM_;

	i = 0;
	while((TrainerTypeName[num][i]!=EOM_)&&(i<10)){
		numstr[i] = TrainerTypeName[num][i];
		i++;
	}
	NMenuMsgWrite(numstr,x,y);
}
#endif

//�|�P�����i���o�[����
static	u16		PokeNumIncDec(u8 mode,u16 num)
{
	switch(mode){
	default:
	case	NUM_INC:
		if(num>1)	num--;
		else		num = 411;
		break;
	case	NUM_DEC:
		if(num<411)	num++;
		else		num = 1;
		break;
	}
	return num;
}

//�g���[�i�[�i���o�[����
static	u16		TrainerNumIncDec(u8 mode,u16 num)
{
	switch(mode){
	default:
	case	NUM_INC:
		if(num>0)	num--;
		else		num = 83-1;
		break;
	case	NUM_DEC:
		if(num<83-1)	num++;
		else		num = 0;
		break;
	}
	return num;
}

#if 0
//�|�P�����A�N�^�[ADD
static	u8		PokeActAdd(u16 pnum,u16 x,u16 y)
{
	u8	actno;

	DecordPokeCellMake( &PM2FObjDataCell[pnum],
						pm2fpara[pnum].size,
						pm2fpara[pnum].patcnt,
						(void *)DecordAdrs[0],
						(void *)DecordAdrs[1],
						pnum);
	DecordPalSet( (PalData*)&PM2NObjDataPal[pnum] );
	PokeActHeaderMake( pnum, 1 );
	actno = AddActor( &PokeActHeader, x, y, 0 );
	ActWork[actno].move = actNoMove;
	ActWork[actno].oamData.Priority = 0;
	return	actno;
}

//�|�P�����A�N�^�[DEL
static	void	PokeActDel(u8 actno)
{
	ObjPalRegNumDel( ObjPalNumIdGet(ActWork[actno].oamData.Pltt) );
	DelActor( &ActWork[actno] );
}
#endif

//�J�[�\���\��
static	void	PmakeCursorPut(u8 cp,u8 x,u8 y1,u8 y2,u8 mode)
{
	u8	curstr[] = {cursor_,EOM_};

	NMenuBoxWrite(x,y1,x,y2);
	if(mode){
		NMenuMsgWrite(curstr,x,cp);
	}
}







/********************************************************************/
/********************************************************************/
/********************************************************************/
/*                                                                  */
/*				�O���ǉ��f�[�^����									*/
/*                                                                  */
/********************************************************************/
#define	TEST_ID1	(SVLD_ID>> 0)&0xff
#define	TEST_ID2	(SVLD_ID>> 8)&0xff
#define	TEST_ID3	(SVLD_ID>>16)&0xff
#define	TEST_ID4	(SVLD_ID>>24)&0xff
/********************************************************************/
static	const	u8	testOutSideData[] = {

TEST_ID1,TEST_ID2,TEST_ID3,TEST_ID4,

};

void	SetOutSideData(void)
{
	ProgramFlashSectorEx(0x1e,(u8*)testOutSideData);
}





/********************************************************************/
/********************************************************************/
/********************************************************************/
/*                                                                  */
/*				�Z�[�u���[�h�A���S���Y���m�F���					*/
/*                                                                  */
/********************************************************************/
//static	void	SaveLoadSysCheckTask(u8 id);
//static	void	SaveLoadSysCheckInitTask(u8 id);

/********************************************************************/
//extern	void	SaveStatusDebug(void);
void	SaveLoadSysCheckInit(void)
{
	switch(sys.InitSeqWork){
	default:
	case 0:
		BackupVirtualMap();
		PokeAGBSaveFieldDivInit();
		sys.InitSeqWork = 1;
		break;
	case 1:
		if(PokeAGBSaveFieldDiv()) sys.InitSeqWork++;
		break;
	case 2:
		SePlay( SE_SAVE );
		sys.InitSeqWork++;
		break;
	case 3:
		if(SeEndCheck())	break;
		ResetForceEvent();
		MainProcChange(FieldNormalRecover);
		break;
	}

//	SaveStatusDebug();
#if 0
//	u8	id;
	u8	i;
	SVLDSYS_DATA*	ldp;

	ldp = (SVLDSYS_DATA*)(UserWork + 0x00000000);	

//	ProgramFlashSectorEx(0x1e,(u8*)testOutSideData);
	for(i=0;i<32;i++){
		ReadFlash(i,0,(u8*)ldp,SVLD_SECTOR_SIZE);
		if((ldp->blocknum == 1)&&(ldp->save_id	== SVLD_ID)){
			EraseFlashSector(i);
		}
	}
//	PokeAGBSave(POKEAGB_SVFLD);
	return;
#endif
#if 0
	InitRegWork();

	DIV_DMACOPY(3,svld_tes_Character,BG_VRAM+0x0000	,0x2200	,16);	//BG CGX�]��
	DIV_DMACOPY(3,svld_tes_Map		,BG_VRAM+0x3800	,0x500	,16);	//BG MAP�]��
	PaletteWorkSet(svld_tes_Palette	,PA_BG0	,0x20*8);//�p���b�g�]��

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������

	PaletteFadeReq(0xffffffff,0,16,0,0);
	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(FadeJob);

	*(vu16 *)REG_BG2CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 7 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
	*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 6 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG2_ON					// BG2�L��
						  | DISP_BG1_ON					// BG1�L��
						  | DISP_OBJ_ON;
	id = AddTask(SaveLoadSysCheckInitTask, 0);
	PokeAGBSaveLoadSysCheckInit();
#endif
}

#if 0
static	void	SaveLoadSysCheckInitTask(u8 id)
{
	PokeAGBSaveLoadSysCheckDataMake();
	PokeAGBSaveLoadSysCheckScreenMake(0x3000);
	TaskTable[id].TaskAdrs = SaveLoadSysCheckTask;
}

static	void	SaveLoadSysCheckTask(u8 id)
{
	if(sys.Trg & B_BUTTON){
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);
		MainProcChange(FadeJob);
		TaskTable[id].TaskAdrs = ExitTaskSeq;
		return;
	}
	if(sys.Trg & A_BUTTON){
		SePlay(SE_SELECT);
		PokeAGBSave(POKEAGB_LDALL);
		PokeAGBSaveLoadSysCheckDataMake();
		PokeAGBSaveLoadSysCheckScreenMake(0x3000);
		return;
	}
	if(sys.Trg & START_BUTTON){
		PokeAGBLoad(POKEAGB_LDALL);
		PokeAGBSaveLoadSysCheckScreenMake(0x3000);
	}
	if(sys.Trg & SELECT_BUTTON){
		PokeAGBSaveLoadSysCheckInit();
		TaskTable[id].TaskAdrs = SaveLoadSysCheckInitTask;
	}
	if(sys.Trg & L_BUTTON){
		PokeAGBSaveLoadSysCheckDataClear();
		PokeAGBSaveLoadSysCheckScreenMake(0x3000);
	}
}
#endif





/********************************************************************/
/********************************************************************/
/********************************************************************/
/*                                                                  */
/*				������������׍쐬���								*/
/*                                                                  */
/********************************************************************/
/********************************************************************/
#define	OOKISA_POKE	(0)
#define	OOKISA_TRNR	(1)
#define	OOKISA_PACT	(2)
#define	OOKISA_TACT	(3)
#define	OOKISA_TY	(4)
#define	OOKISA_PY	(5)
#define	OOKISA_TAF	(6)
#define	OOKISA_PAF	(7)
#define	OOKISA_MODE	(8)
#define	OOKISA_CHGF	(9)

#if 0
//�f�o�b�O�p�v���O�����̂��߁A�Ƃ肠����
#define	ZUKAN_TYPE_SIZE	(5)
typedef struct {

u8			typename[ZUKAN_TYPE_SIZE];	//��ޖ�
u16			takasa;			//������
u16			omosa;			//������

const u8*	text;			//÷��

//������������חp
u16			t_no;			//�ڰŰ���ް
u16			affine_p;		//�䗦(�߹��)
u16			y_p;			//�\��Y(�߹��)
u16			affine_t;		//�䗦(�ڰŰ)
u16			y_t;			//�\��Y(�ڰŰ)

}ZUKAN_DATA;
extern	const	ZUKAN_DATA	zukanData[];
//�f�o�b�O�p�v���O�����̂��߁A�Ƃ肠����
#endif
extern	void	affineWorkSet(u8 num,u16 H_X,u16 V_X,u16 H_Y,u16 V_Y);

static	void	OokisakurabeMakeInitTask(u8 id);
static	void	OokisakurabeMakMain(u8 id);
static	u16		MonotonePal[] = {
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
};
static	const u8	OokisaIndex1[] = {hi_,ri_,tu_,EOM_};
static	const u8	OokisaIndex2[] = {ta_,ka_,sa_,EOM_};
static	const u8	OokisaIndex3[] = {L__,KI_,bou_,cursor_,si_,yyu_,ku_,si_,yyo_,u_,EOM_};
static	const u8	OokisaIndex4[] = {R__,KI_,bou_,cursor_,ka_,ku_,da_,i_,EOM_};
static	const u8	OokisaIndex5[] = {U__,KI_,bou_,cursor_,u_,e_,spc_,i_,do_,u_,EOM_};
static	const u8	OokisaIndex6[] = {D__,KI_,bou_,cursor_,si_,ta_,spc_,i_,do_,u_,EOM_};
static	const u8	OokisaIndex7[] = {A__,BO_,TA_,N_,cursor_,so_,u_,sa_,ki_,ri_,ka_,e_,EOM_};
static	const u8	OokisaIndex8[] = {B__,BO_,TA_,N_,cursor_,ya_,me_,ru_,EOM_};
static	const u8	OokisaIndex9[] = {SU_,TA_,bou_,TO_,colon_,EOM_};
static	const u8	OokisaIndex10[]= {colon_,SE_,RE_,KU_,TO_,EOM_};

void	OokisakurabeMakeInit(void)
{
	u8	id;

	InitRegWork();

	ObjPalManStart = 1;						// ��گ��Ȱ�ެNo�J�n
	PaletteWorkSet(MonotonePal,PA_OBJ0,0x20);//���m�g�[���p���b�g�]��

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������

	PaletteFadeReq(0xffffffff,0,16,0,0);
	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(FadeJob);

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_OBJ_ON;
	id = AddTask(OokisakurabeMakeInitTask, 0);
	TaskTable[id].work[OOKISA_POKE] = 252;
}

static	void	OokisakurabeMakeInitTask(u8 id)
{
	u16	pnum,tnum,anum,znum;

	znum = TaskTable[id].work[OOKISA_POKE];
	pnum = PokeZukanNo2MonsNoGet(znum);		//�J���i���o�[
	tnum = MyData.my_sex;	//0:�j,1:��

	NMenuWinBoxWrite(6,0,23,9);

	NMenuWinBoxWrite(0,0,5,9);
	NMenuMsgWrite(OokisaIndex1,1,1);
	NMenuMsgWrite(OokisaIndex2,1,5);
	NMenuWinBoxWrite(24,0,29,9);
	NMenuMsgWrite(OokisaIndex1,25,1);
	NMenuMsgWrite(OokisaIndex2,25,5);

	NMenuWinBoxWrite(0,10,11,19);
	NMenuMsgWrite(OokisaIndex3,1,11);
	NMenuMsgWrite(OokisaIndex4,1,13);
	NMenuMsgWrite(OokisaIndex5,1,15);
	NMenuMsgWrite(OokisaIndex6,1,17);

	NMenuWinBoxWrite(12,10,29,15);
	NMenuMsgWrite(OokisaIndex7,13,11);
	NMenuMsgWrite(OokisaIndex8,13,13);
	NMenuWinBoxWrite(12,16,29,19);
	NMenuMsgWrite(OokisaIndex9,13,17);
	NMenuMsgWrite(OokisaIndex10,24,17);
	PokeNamePut(pnum,18,17);

	//�g���[�i�[�A�N�^�[�ݒ�
	anum = ZukanTrainerActorSet(MyData.my_sex,120+32,8+32,0);
	ActWork[anum].oamData.AffineMode = 1;
	ActWork[anum].oamData.AffineParamNo = 2;
	ActWork[anum].oamData.Priority = 0;
	ActWork[anum].oamData.Pltt = 0;
	ObjPalRegNumDel(ObjPalNumIdGet(ActWork[anum].oamData.Pltt));
	ActWork[anum].dy = zukanData[znum].y_t;	//�����w��
	affineWorkSet(2,zukanData[znum].affine_t,0,0,zukanData[znum].affine_t);	//�䗦�w��
	TaskTable[id].work[OOKISA_TACT] = anum;
	TaskTable[id].work[OOKISA_TY]	= zukanData[znum].y_t;	//�����w��
	TaskTable[id].work[OOKISA_TAF]	= zukanData[znum].affine_t;	//�䗦�w��

	//�|�P�����A�N�^�[�ݒ�
	anum = ZukanPokeActorSet(znum,120-32,8+32,1);
	ActWork[anum].oamData.AffineMode = 1;
	ActWork[anum].oamData.AffineParamNo = 1;
	ActWork[anum].oamData.Priority = 0;
	ActWork[anum].oamData.Pltt = 0;
	ObjPalRegNumDel(ObjPalNumIdGet(ActWork[anum].oamData.Pltt));
	ActWork[anum].dy = zukanData[znum].y_p;	//�����w��
	affineWorkSet(1,zukanData[znum].affine_p,0,0,zukanData[znum].affine_p);	//�䗦�w��
	TaskTable[id].work[OOKISA_PACT] = anum;
	TaskTable[id].work[OOKISA_PY]	= zukanData[znum].y_p;	//�����w��
	TaskTable[id].work[OOKISA_PAF]	= zukanData[znum].affine_p;	//�䗦�w��

	TaskTable[id].work[OOKISA_MODE] = 0;
	TaskTable[id].work[OOKISA_CHGF] = 0;

	*(vu16 *)REG_WININ	= 0x3f1f;
	*(vu16 *)REG_WINOUT	= 0x001f;
	*(vu16 *)REG_WIN0H = (56 + (TaskTable[id].work[OOKISA_MODE])*64)*0x100
						+(120 + (TaskTable[id].work[OOKISA_MODE])*64);
	*(vu16 *)REG_WIN0V = (8)*0x100+(8 + 64);
	*(vu16 *)REG_WIN1H = ((48+1)*0x100) + (192-1);
	*(vu16 *)REG_WIN1V = (0+1)*0x100+(80-1);
   	*(vu16 *)REG_BLDCNT   = 0x00e1;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 7;
	*(vu16 *)REG_DISPCNT |= DISP_WIN01_ON;				// WIN01 ON

	TaskTable[id].TaskAdrs = OokisakurabeMakMain;
}

static	void	OokisakurabeMakMain(u8 id)
{
	u16	y,af;

	if(sys.Trg & B_BUTTON){
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);
		MainProcChange(FadeJob);
		TaskTable[id].TaskAdrs = ExitTaskSeq;
		return;
	}
	if(sys.Trg & A_BUTTON){
		SePlay(SE_SELECT);
		TaskTable[id].work[OOKISA_MODE] ^= 1;
		return;
	}
	if(sys.Repeat & L_KEY){
		if(TaskTable[id].work[OOKISA_MODE]){
			if(sys.Cont & R_BUTTON)	TaskTable[id].work[OOKISA_TAF]+=0x10;
			else					TaskTable[id].work[OOKISA_TAF]++;
			if((u16)TaskTable[id].work[OOKISA_TAF]>0x8000) TaskTable[id].work[OOKISA_TAF]=0x8000;
		}else{
			if(sys.Cont & R_BUTTON)	TaskTable[id].work[OOKISA_PAF]+=0x10;
			else					TaskTable[id].work[OOKISA_PAF]++;
			if((u16)TaskTable[id].work[OOKISA_PAF]>0x8000) TaskTable[id].work[OOKISA_PAF]=0x8000;
		}
		return;
	}
	if(sys.Repeat & R_KEY){
		if(TaskTable[id].work[OOKISA_MODE]){
			if(sys.Cont & R_BUTTON)	TaskTable[id].work[OOKISA_TAF]-=0x10;
			else					TaskTable[id].work[OOKISA_TAF]--;
			if(TaskTable[id].work[OOKISA_TAF]<0x100) TaskTable[id].work[OOKISA_TAF]=0x100;
		}else{
			if(sys.Cont & R_BUTTON)	TaskTable[id].work[OOKISA_PAF]-=0x10;
			else					TaskTable[id].work[OOKISA_PAF]--;
			if(TaskTable[id].work[OOKISA_PAF]<0x100) TaskTable[id].work[OOKISA_PAF]=0x100;
		}
		return;
	}
	if(sys.Repeat & U_KEY){
		if(TaskTable[id].work[OOKISA_MODE]){
			TaskTable[id].work[OOKISA_TY]--;
		}else{
			TaskTable[id].work[OOKISA_PY]--;
		}
		return;
	}
	if(sys.Repeat & D_KEY){
		if(TaskTable[id].work[OOKISA_MODE]){
			TaskTable[id].work[OOKISA_TY]++;
		}else{
			TaskTable[id].work[OOKISA_PY]++;
		}
		return;
	}
	if(sys.Repeat & START_BUTTON){
		if(TaskTable[id].work[OOKISA_POKE] < POKE_NUM_MAX){
			TaskTable[id].work[OOKISA_POKE]++;
		}else{
			TaskTable[id].work[OOKISA_POKE] = 1;
		}
		DelActor(&ActWork[TaskTable[id].work[OOKISA_PACT]]);
		DelActor(&ActWork[TaskTable[id].work[OOKISA_TACT]]);
		TaskTable[id].TaskAdrs = OokisakurabeMakeInitTask;
		return;
	}
	if(sys.Repeat & SELECT_BUTTON){
		if(TaskTable[id].work[OOKISA_POKE] > 1){
			TaskTable[id].work[OOKISA_POKE]--;
		}else{
			TaskTable[id].work[OOKISA_POKE] = POKE_NUM_MAX;
		}
		DelActor(&ActWork[TaskTable[id].work[OOKISA_PACT]]);
		DelActor(&ActWork[TaskTable[id].work[OOKISA_TACT]]);
		TaskTable[id].TaskAdrs = OokisakurabeMakeInitTask;
		return;
	}

	af	= TaskTable[id].work[OOKISA_PAF];
	y	= TaskTable[id].work[OOKISA_PY];
	affineWorkSet(1,af,0,0,af);	//�䗦�w��
	ColDataPut(af,1,3,4);
	ColDataPut(y,1,7,4);
	ActWork[TaskTable[id].work[OOKISA_PACT]].dy = y;

	af	= TaskTable[id].work[OOKISA_TAF];
	y	= TaskTable[id].work[OOKISA_TY];
	affineWorkSet(2,af,0,0,af);	//�䗦�w��
	ColDataPut(af,25,3,4);
	ColDataPut(y,25,7,4);
	ActWork[TaskTable[id].work[OOKISA_TACT]].dy = y;

	*(vu16 *)REG_WIN0H = (56 + (TaskTable[id].work[OOKISA_MODE])*64)*0x100
						+(120 + (TaskTable[id].work[OOKISA_MODE])*64);
}










/********************************************************************/
/********************************************************************/
/********************************************************************/
/*                                                                  */
/*				�f�o�b�O�p�퓬										*/
/*                                                                  */
/********************************************************************/
/********************************************************************/
extern	void InitBox_DebugPokeSet(PokemonPasoParam *ppp, pFunc ret_func);

static	void	DebugPokeBattleInitSub(u8 mode);
static	void	DPbattleSeq0(u8 id);
static	void	DPbattleSeq1_1(u8 id);
static	void	DPbattleSeq1_2(u8 id);
static	void	DPbattleSeq2_1(u8 id);
static	void	DPbattleSeq2_2(u8 id);
static	void	DPbattleSeq3(u8 id);
static	void	DPbattleSeq3_1(u8 id);
static	void	DPbattleSeq3_2(u8 id);
static	void	DPbattleSeq3_3(u8 id);
static	void	DPbattleSeq3_4(u8 id);
static	void	DPbattleSeq4(u8 id);
static	void	DPbattleSeq4_1(u8 id);
static	void	DPbattleSeq4_2(u8 id);
static	void	DPbattleSeq5(u8 id);
static	void	DPbattleSeq5_1(u8 id);
static	void	DPbattleSeq5_2(u8 id);
static	void	DPbattleSeq5_3(u8 id);
static	void	DPbattleSeq5_4(u8 id);
static	void	DPbattleSeq5_5(u8 id);
static	void	DPbattleSeq6(u8 id);
static	void	DPbattleSeq6_1(u8 id);
static	void	DPbattleSeq6_2(u8 id);

static	void	DPbattleListPutMine(void);
static	void	DPbattleListPutEnemy(void);
static	u8		DPbattleCountCheck(void);
static	void	DPbattleTrainerListPut(u8 p);
static	void	DPbattlePlaceListPut(u8 p);

typedef struct
{
const u8*	label;
u8			mode_id;
u8			pm_min;
}DPBmodeMenuLabel;

typedef struct
{
const u8*	label;
u32			ai_id;
}DPBaiMenuLabel;

typedef struct
{
const u8*	label;
u8			place_id;
}DPBplaceMenuLabel;

typedef struct
{
u8	dir;		//�|�P�����I��(0:����,1:����)
u8	sel_mine;	//�|�P�����I���|�W�V����(����)
u8	sel_enemy;	//�|�P�����I���|�W�V����(����)
u8	mode;		//���[�h�I���|�W�V����
u8	ai_lp;		//���[�h�I��(�g���[�i�[���X�g)���X�g�|�W�V����
u8	ai_cp;		//���[�h�I��(�g���[�i�[���X�g)�J�[�\���|�W�V����
u8	place_lp;	//�n�`�I�����X�g�|�W�V����
u8	place_cp;	//�n�`�I���J�[�\���|�W�V����
u8	sex_cp;		//���ʑI���J�[�\���|�W�V����
u16	bgm_back;	//BGM�i���o�[�o�b�N�A�b�v
u8	pm_min;		//�|�P�����ŏ��I��

u8	mine_backup;	//�����̐��ʃo�b�N�A�b�v

}DPB_WORK_BUF;

#define	DPB_W_DIR	(0)			//�|�P�����I��(0:����,1:����)
#define	DPB_W_SEL_M	(1)			//�|�P�����I���|�W�V����(����)
#define	DPB_W_SEL_E	(2)			//�|�P�����I���|�W�V����(����)
#define	DPB_W_MODE	(3)			//���[�h�I���|�W�V����
#define	DPB_W_TLP	(4)			//���[�h�I��(�g���[�i�[���X�g)���X�g�|�W�V����
#define	DPB_W_TCP	(5)			//���[�h�I��(�g���[�i�[���X�g)�J�[�\���|�W�V����
#define	DPB_W_PLP	(6)			//�n�`�I�����X�g�|�W�V����
#define	DPB_W_PCP	(7)			//�n�`�I���J�[�\���|�W�V����

#define	DPB_MODE_MAX	(5)			//���[�h��
#define	DPB_AI_MAX		(10)		//AI��
#define	DPB_PLACE_MAX	(9)			//�n�`��

static	const	u8	DPBmodeMenu1[]={ya_,se_,i_,ta_,i_,se_,n_,EOM_};
static	const	u8	DPBmodeMenu2[]={n1_,V__,S__,n1_,ta_,i_,se_,n_,EOM_};
//static	const	u8	DPBmodeMenu3[]={CTRL_,C_FCOL_,4,n2_,V__,S__,n2_,ta_,i_,se_,n_,EOM_};
static	const	u8	DPBmodeMenu3[]={n2_,V__,S__,n2_,ta_,i_,se_,n_,EOM_};
static	const	u8	DPBmodeMenu4[]={CTRL_,C_FCOL_,4,n1_,V__,S__,n1_,tu_,u_,si_,n_,ta_,i_,se_,n_,EOM_};
static	const	u8	DPBmodeMenu5[]={CTRL_,C_FCOL_,4,n2_,V__,S__,n2_,tu_,u_,si_,n_,ta_,i_,se_,n_,EOM_};

static	const	DPBmodeMenuLabel	DPBmodeMenuTable[DPB_MODE_MAX] = {
	{DPBmodeMenu1,FIGHT_TYPE_1vs1,1},
	{DPBmodeMenu2,FIGHT_TYPE_1vs1|FIGHT_TYPE_TRAINER,1},
	{DPBmodeMenu3,FIGHT_TYPE_2vs2|FIGHT_TYPE_TRAINER,2},
	{DPBmodeMenu4,FIGHT_TYPE_1vs1|FIGHT_TYPE_TRAINER|FIGHT_TYPE_TUUSHIN,1},
	{DPBmodeMenu5,FIGHT_TYPE_2vs2|FIGHT_TYPE_TRAINER|FIGHT_TYPE_TUUSHIN,2},
};

static	const	u8	DPBaiMenu1[]={ki_,ho_,n_,TA_,I_,PU_,EOM_};
static	const	u8	DPBaiMenu2[]={A__,I__,TA_,I_,PU_,n2_,EOM_};
static	const	u8	DPBaiMenu3[]={A__,I__,TA_,I_,PU_,n3_,EOM_};
static	const	u8	DPBaiMenu4[]={A__,I__,TA_,I_,PU_,n4_,EOM_};
static	const	u8	DPBaiMenu5[]={A__,I__,TA_,I_,PU_,n5_,EOM_};
static	const	u8	DPBaiMenu6[]={A__,I__,TA_,I_,PU_,n6_,EOM_};
static	const	u8	DPBaiMenu7[]={A__,I__,TA_,I_,PU_,n7_,EOM_};
static	const	u8	DPBaiMenu8[]={A__,I__,TA_,I_,PU_,n8_,EOM_};
static	const	u8	DPBaiMenu9[]={A__,I__,TA_,I_,PU_,n9_,EOM_};
static	const	u8	DPBaiMenu10[]={A__,I__,TA_,I_,PU_,n1_,n0_,EOM_};

static	const	DPBaiMenuLabel	DPBaiMenuTable[DPB_AI_MAX] = {
	{DPBaiMenu1	,0x00000001},
	{DPBaiMenu2	,0x00000002},
	{DPBaiMenu3	,0x00000004},
	{DPBaiMenu4	,0x00000008},
	{DPBaiMenu5	,0x00000010},
	{DPBaiMenu6	,0x00000020},
	{DPBaiMenu7	,0x00000040},
	{DPBaiMenu8	,0x00000080},
	{DPBaiMenu9	,0x00000100},
	{DPBaiMenu10,0x00000200},
};

u32	DPBwork[8];
DPB_WORK_BUF	DPBsv = {0};

static	const	u8	DPBplaceMenu1[]={ku_,sa_,EOM_};
static	const	u8	DPBplaceMenu2[]={na_,ga_,i_,spc_,ku_,sa_,EOM_};
static	const	u8	DPBplaceMenu3[]={su_,na_,EOM_};
static	const	u8	DPBplaceMenu4[]={mi_,zu_,EOM_};
static	const	u8	DPBplaceMenu5[]={u_,mi_,EOM_};
static	const	u8	DPBplaceMenu6[]={i_,ke_,EOM_};
static	const	u8	DPBplaceMenu7[]={i_,wa_,EOM_};
static	const	u8	DPBplaceMenu8[]={do_,u_,ku_,tu_,EOM_};
static	const	u8	DPBplaceMenu9[]={so_,no_,ta_,EOM_};

static	const	DPBplaceMenuLabel	DPBplaceMenuTable[DPB_PLACE_MAX] = {
	{DPBplaceMenu1,0},
	{DPBplaceMenu2,1},
	{DPBplaceMenu3,2},
	{DPBplaceMenu4,3},
	{DPBplaceMenu5,4},
	{DPBplaceMenu6,5},
	{DPBplaceMenu7,6},
	{DPBplaceMenu8,7},
	{DPBplaceMenu9,8},
};

static	void	DPbattlePokeNamePutBuffer(u8* buf,u16 pnum)
{
	u8	i;

	for(i=0;i<5;i++)	buf[i] = spc_;
	buf[5] = EOM_;
	buf[0] = bou_;

	if(pnum){
		i = 0;
		while((PokeNameData[pnum][i]!=EOM_)&&(i<5)){
			buf[i] = PokeNameData[pnum][i];
			i++;
		}
	}
}

static	void	DPbattlePokeCalc(u8 mode)
{
	u8	i;
	u16	hpmax;

	for(i=0;i<6;i++){
		if(mode){
			if(PokeParaGet(&PokeParaEnemy[i],ID_monsno)){
				PokeParaCalc(&PokeParaEnemy[i]);
				hpmax=PokeParaGet(&PokeParaEnemy[i],ID_hpmax);
				PokeParaPut(&PokeParaEnemy[i],ID_hp,(u8 *)&hpmax);	//HP�����Z�b�g
			}
		}else{
			if(PokeParaGet(&PokeParaMine[i],ID_monsno)){
				PokeParaCalc(&PokeParaMine[i]);
				hpmax=PokeParaGet(&PokeParaMine[i],ID_hpmax);
				PokeParaPut(&PokeParaMine[i],ID_hp,(u8 *)&hpmax);	//HP�����Z�b�g
			}
		}
	}
}

/********************************************************************/
/********************************************************************/
/*				������												*/
//���݂̗���	SEQ3:		�ΐ�^�C�v�I��
//				SEQ6:		���ʑI��
//				SEQ0&1&2:	�|�P�����I��
//				SEQ4:		�n�`�I��
//				SEQ5:		�퓬��
/********************************************************************/
void	DebugPokeBattleInit(void)
{
	BackUpPokeParaMine();
	DPBsv.mine_backup = MyData.my_sex;	//�����̐��ʃo�b�N�A�b�v

//	DPBsv.bgm_back	= Fld.MapMusicNo;	//BGM�i���o�[�o�b�N�A�b�v

	DPBsv.dir		= 0;	//�|�P�����I��(0:����,1:����)
	DPBsv.sel_mine	= 0;	//�|�P�����I���|�W�V����(����)
	DPBsv.sel_enemy	= 0;	//�|�P�����I���|�W�V����(����)
	DPBsv.mode		= 0;	//���[�h�I���|�W�V����
	DPBsv.ai_lp		= 0;	//���[�h�I��(�g���[�i�[���X�g)���X�g�|�W�V����
	DPBsv.ai_cp		= 0;	//���[�h�I��(�g���[�i�[���X�g)�J�[�\���|�W�V����
	DPBsv.place_lp	= 0;	//�n�`�I�����X�g�|�W�V����
	DPBsv.place_cp	= 0;	//�n�`�I���J�[�\���|�W�V����
	DPBsv.sex_cp	= 0;	//���ʑI���J�[�\���|�W�V����
	DPBsv.pm_min	= 0;	//�|�P�����ŏ��I��
	DPBwork[0]		= 0;	//�����[�N(AI�p)

	PokeParaMineInit();
	PokeParaEnemyInit();
	DebugPokeBattleInitSub(0);
	AddTask(DPbattleSeq3, 0);
}

void	DebugPokeBattleInitReturn(void)
{
	DPBsv.sel_mine	= 0;	//�|�P�����I���|�W�V����(����)
	DPBsv.sel_enemy	= 0;	//�|�P�����I���|�W�V����(����)
//	DPbattlePokeCalc(0);
//	DPbattlePokeCalc(1);
	PokeParaMineInit();
	PokeParaEnemyInit();
	DebugPokeBattleInitSub(0);
	AddTask(DPbattleSeq3, 0);
}

void	DebugPokeBattleReturnMine(void)
{
	DPbattlePokeCalc(0);
	DebugPokeBattleInitSub(0);
	AddTask(DPbattleSeq0, 0);
}

void	DebugPokeBattleReturnEnemy(void)
{
	DPbattlePokeCalc(1);
	DebugPokeBattleInitSub(1);
	AddTask(DPbattleSeq0, 0);
}

static	void	DebugPokeBattleInitSub(u8 mode)
{
//	u8	taskID;

	InitRegWork();

//	PaletteFadeReq(0xffffffff,0,16,0,0);

	*(vu16 *)REG_WIN0H	= 0;
	*(vu16 *)REG_WIN0V	= 0;
	*(vu16 *)REG_WIN1H	= 0;
	*(vu16 *)REG_WIN1V	= 0;

	*(vu16 *)REG_WININ	= 0x1111;
 	*(vu16 *)REG_WINOUT	= 0x0031;

   	*(vu16 *)REG_BLDCNT   = 0x00e1;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 7;

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(FadeJob);

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_WIN01_ON				// WIN0&1 ON
						  | DISP_OBJ_ON;

	*(vu16 *)REG_WIN0H	= (1*0x100)+239;
	*(vu16 *)REG_WIN0V	= 129*0x100+159;
}

/********************************************************************/
/*				�|�P�����I��										*/
/********************************************************************/
static	void	DPbattleSeq0(u8 id)
{
	u8	indexstr[] =	{PO_,KE_,MO_,N_,wo_,spc_,e_,ra_,n_,de_,ne_,EOM_};
	u8	index2str[] =	{CTRL_,C_FCOL_,2,S__,T__,A__,R__,T__,colon_,tu_,gi_,he_,EOM_};

	NMenuWinBoxWrite(0,16,29,19);
	NMenuMsgWrite(indexstr,1,17);
	NMenuMsgWrite(index2str,20,17);

	DPbattleListPutMine();
	DPbattleListPutEnemy();

	if(DPBsv.dir){
		TaskTable[id].TaskAdrs = DPbattleSeq2_1;	//����
	}else{
		TaskTable[id].TaskAdrs = DPbattleSeq1_1;	//����
	}
}
/*---------		�������|�P�����I��	-------------*/
/*	�J�[�\���\��	*/
static	void	DPbattleSeq1_1(u8 id)
{
	PmakeCursorPut(DPBsv.sel_mine*2+3,1,1,14,1);

	*(vu16 *)REG_WIN1H	= (1*0x100)+119;
	*(vu16 *)REG_WIN1V	= 1*0x100+127;

	TaskTable[id].TaskAdrs = DPbattleSeq1_2;
}

/*	�J�[�\���ړ�	*/
static	void	DPbattleSeq1_2(u8 id)
{
	if(sys.Trg & A_BUTTON){
		SePlay(SE_SELECT);
		InitBox_DebugPokeSet(&PokeParaMine[DPBsv.sel_mine].PPP,DebugPokeBattleReturnMine);
		DelTask(id);
		return;
	}
	if(sys.Trg & B_BUTTON){
		TaskTable[id].TaskAdrs = DPbattleSeq6;
		return;
	}
	if(sys.Trg & START_BUTTON){
//		if((PokeParaMine[0].PPP.monsno)&&(PokeParaEnemy[0].PPP.monsno)){	//�|�P�����m�F
		if(DPbattleCountCheck()){	//�|�P�����m�F
			SePlay(SE_SELECT);
			PokeCountMineGet();
			PokeCountEnemyGet();
			TaskTable[id].TaskAdrs = DPbattleSeq4;
		}
		return;
	}
	if((sys.Trg & R_BUTTON)||(sys.Trg & R_KEY)){
		PmakeCursorPut(0,1,1,14,0);
		TaskTable[id].TaskAdrs = DPbattleSeq2_1;
		return;
	}
	if((sys.Repeat & U_KEY)&&(DPBsv.sel_mine > 0)){
		DPBsv.sel_mine--;
		TaskTable[id].TaskAdrs = DPbattleSeq1_1;
		return;
	}
	if((sys.Repeat & D_KEY)&&(DPBsv.sel_mine < 6-1)){
		if(PokeParaGet(&PokeParaMine[DPBsv.sel_mine],ID_monsno)){	//�|�P�������Z�b�g������OK
			DPBsv.sel_mine++;
			TaskTable[id].TaskAdrs = DPbattleSeq1_1;
		}
		return;
	}
}

/*---------		���葤�|�P�����I��	-------------*/
/*	�J�[�\���\��	*/
static	void	DPbattleSeq2_1(u8 id)
{
	PmakeCursorPut(DPBsv.sel_enemy*2+3,16,1,14,1);

	*(vu16 *)REG_WIN1H	= (121*0x100)+239;
	*(vu16 *)REG_WIN1V	= 1*0x100+127;

	TaskTable[id].TaskAdrs = DPbattleSeq2_2;
}

/*	�J�[�\���ړ�	*/
static	void	DPbattleSeq2_2(u8 id)
{
	if(sys.Trg & A_BUTTON){
		SePlay(SE_SELECT);
		InitBox_DebugPokeSet(&PokeParaEnemy[DPBsv.sel_enemy].PPP,DebugPokeBattleReturnEnemy);
		DelTask(id);
		return;
	}
	if(sys.Trg & B_BUTTON){
		TaskTable[id].TaskAdrs = DPbattleSeq6;
		return;
	}
	if(sys.Trg & START_BUTTON){
//		if((PokeParaMine[0].PPP.monsno)&&(PokeParaEnemy[0].PPP.monsno)){	//�|�P�����m�F
		if(DPbattleCountCheck()){	//�|�P�����m�F
			SePlay(SE_SELECT);
			PokeCountMineGet();
			PokeCountEnemyGet();
			TaskTable[id].TaskAdrs = DPbattleSeq4;
		}
		return;
	}
	if((sys.Trg & L_BUTTON)||(sys.Trg & L_KEY)){
		PmakeCursorPut(0,16,1,14,0);
		TaskTable[id].TaskAdrs = DPbattleSeq1_1;
		return;
	}
	if((sys.Repeat & U_KEY)&&(DPBsv.sel_enemy > 0)){
		DPBsv.sel_enemy--;
		TaskTable[id].TaskAdrs = DPbattleSeq2_1;
		return;
	}
	if((sys.Repeat & D_KEY)&&(DPBsv.sel_enemy < 6-1)){
		if(PokeParaGet(&PokeParaEnemy[DPBsv.sel_enemy],ID_monsno)){	//�|�P�������Z�b�g������OK
			DPBsv.sel_enemy++;
			TaskTable[id].TaskAdrs = DPbattleSeq2_1;
		}
		return;
	}
}

/*---------		�������|�P�������X�g�\��	-------------*/
static	void	DPbattleListPutMine(void)
{
	u8	i;
	u8	indexstr[] =	{CTRL_,C_FCOL_,2,zi_,bu_,n_,no_,spc_,PO_,KE_,MO_,N_,EOM_};
	u8	buf[5+1];

	NMenuWinBoxWrite(0,0,14,15);
	NMenuMsgWrite(indexstr,2,1);

	for(i=0;i<6;i++){
		DPbattlePokeNamePutBuffer(buf,PokeParaGet(&PokeParaMine[i],ID_monsno));
		NMenuMsgWrite(buf,2,i*2+3);
	}
}

/*---------		���葤�|�P�������X�g�\��	-------------*/
static	void	DPbattleListPutEnemy(void)
{
	u8	i;
	u8	indexstr[] =	{CTRL_,C_FCOL_,2,a_,i_,te_,no_,spc_,PO_,KE_,MO_,N_,EOM_};
	u8	buf[5+1];

	NMenuWinBoxWrite(15,0,29,15);
	NMenuMsgWrite(indexstr,17,1);

	for(i=0;i<6;i++){
		DPbattlePokeNamePutBuffer(buf,PokeParaGet(&PokeParaEnemy[i],ID_monsno));
		NMenuMsgWrite(buf,17,i*2+3);
	}
}

/*---------		�|�P�������`�F�b�N	-------------*/
static	u8	DPbattleCountCheck(void)
{
	u8	min;

	min = DPBsv.pm_min-1;

	if((PokeParaGet(&PokeParaMine[min],ID_monsno))&&
	   (PokeParaGet(&PokeParaEnemy[min],ID_monsno))){	//�|�P�����m�F
		return 1;
	}
	return 0;
}

/********************************************************************/
/*				�ΐ탂�[�h�I��										*/
#if 0
typedef struct {
	u8 pos;								// �閧��n�̏ꏊ

	u8 entry_flag:4;					// �o�^�t���O
	u8 sex:1;							// ����
	u8 fight_flag:1;					// �ΐ�t���O
	u8 flg:2;							// �o�^�p�t���O(0:���o�^ 1:�o�^)

	u8 name[PERSON_NAME_SIZE];			// ������̖��O
	u8 id[4];							// �������ID
	u16 org_count;						// �I���W�i���J�E���g(�^�C���X�^���v�̂悤�Ȃ��́j
	u8 enter_count;						// �K�ꂽ��
	u8 exchange_item;					// �����p�A�C�e���i�͗l�ւ��O�b�Y�̂݁j

	// �͗l�ւ��f�[�^
	u8 furniture_id[16];				// id
	u8 furniture_pos[16];				// ���4�r�b�g:x ����4�r�b�g:y

	BASE_BATTLE_DATA battle;

}SECRET_BASE_DATA;
#endif
/********************************************************************/
static	void	DPbattleSeq3(u8 id)
{
	u8	i;
	u8	indexstr[] =	{ta_,i_,se_,n_,MO_,bou_,DO_,wo_,spc_,e_,ra_,n_,de_,ne_,EOM_};
	u8	index2str[] =	{CTRL_,C_FCOL_,2,S__,T__,A__,R__,T__,colon_,tu_,gi_,he_,EOM_};
	u8	index3str[] =	{CTRL_,C_FCOL_,2,BA_,TO_,RU_,MO_,bou_,DO_,spc_,se_,n_,ta_,ku_,EOM_};
	u8	index4str[] =	{CTRL_,C_FCOL_,2,TO_,RE_,bou_,NA_,bou_,A__,I__,spc_,se_,n_,ta_,ku_,EOM_};

	NMenuWinBoxWrite(0,16,29,19);
	NMenuMsgWrite(indexstr,1,17);
	NMenuMsgWrite(index2str,20,17);

	NMenuWinBoxWrite(0,0,14,15);
	NMenuMsgWrite(index3str,2,1);
	for(i=0;i<DPB_MODE_MAX;i++){
		NMenuMsgWrite(DPBmodeMenuTable[i].label,2,i*2+3);
	}
	NMenuWinBoxWrite(15, 0,29,15);
	NMenuMsgWrite(index4str,17,1);
	DPbattleTrainerListPut(DPBsv.ai_lp);

	TaskTable[id].TaskAdrs = DPbattleSeq3_1;
}

/*	�J�[�\���\��(MODE)	*/
static	void	DPbattleSeq3_1(u8 id)
{
	PmakeCursorPut(DPBsv.mode*2+3,1,1,14,1);

	*(vu16 *)REG_WIN1H	= (1*0x100)+119;
	*(vu16 *)REG_WIN1V	= 1*0x100+127;

	TaskTable[id].TaskAdrs = DPbattleSeq3_2;
}

/*	�J�[�\���ړ�(MODE)	*/
static	void	DPbattleSeq3_2(u8 id)
{
	if(sys.Trg & B_BUTTON){
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);
		MainProcChange(FadeJob);
		RecoverPokeParaMine();
		MyData.my_sex = DPBsv.mine_backup;	//�����̐��ʕ��A
		//----�퓬�֘A������---
		FightType = 0;
		DebugFightFlag = 0;
		//----�퓬�֘A������---
		TaskTable[id].TaskAdrs = ExitTaskSeq;
		return;
	}
	if((sys.Trg & START_BUTTON)||(sys.Trg & A_BUTTON)){
		SePlay(SE_SELECT);
		FightType = (DPBmodeMenuTable[DPBsv.mode].mode_id);
		DebugFightFlag=DEBUG_FIGHT_DEBUG;
		DPBsv.pm_min = DPBmodeMenuTable[DPBsv.mode].pm_min;

		if((DPBsv.mode == 1)||(DPBsv.mode == 2)){
			PmakeCursorPut(0,1,1,14,0);
			TaskTable[id].TaskAdrs = DPbattleSeq3_3;	//AI�^�C�v�I��
		}else{
			TaskTable[id].TaskAdrs = DPbattleSeq6;
		}
		return;
	}
	if((sys.Repeat & U_KEY)&&(DPBsv.mode > 0)){
		DPBsv.mode--;
		TaskTable[id].TaskAdrs = DPbattleSeq3_1;
		return;
	}
//	if((sys.Repeat & D_KEY)&&(DPBsv.mode < DPB_MODE_MAX-1)){
	if((sys.Repeat & D_KEY)&&(DPBsv.mode < 3-1)){
		DPBsv.mode++;
		TaskTable[id].TaskAdrs = DPbattleSeq3_1;
		return;
	}
}

/*	�J�[�\���\��(AI)	*/
static	void	DPbattleSeq3_3(u8 id)
{
	PmakeCursorPut(DPBsv.ai_cp*2+3,16,1,14,1);

	*(vu16 *)REG_WIN1H	= (121*0x100)+239;
	*(vu16 *)REG_WIN1V	= 1*0x100+127;

	TaskTable[id].TaskAdrs = DPbattleSeq3_4;
}

/*	�J�[�\���ړ�(AI)	*/
static	void	DPbattleSeq3_4(u8 id)
{
	u8	lp;
	u32	ai_mask;

	lp = DPBsv.ai_lp + DPBsv.ai_cp;

	if(sys.Trg & A_BUTTON){
		ai_mask = DPBaiMenuTable[lp].ai_id;
		if(DPBwork[0] & ai_mask){
			ai_mask^=0xffff;
			DPBwork[0] &= ai_mask;	//OFF
		}else{
			DPBwork[0] |= ai_mask;	//ON
		}
		DPbattleTrainerListPut(DPBsv.ai_lp);
		TaskTable[id].TaskAdrs = DPbattleSeq3_3;
		return;
	}
	if(sys.Trg & B_BUTTON){
		PmakeCursorPut(0,16,1,14,0);
		TaskTable[id].TaskAdrs = DPbattleSeq3_1;
		return;
	}
	if(sys.Trg & START_BUTTON){
		SePlay(SE_SELECT);

//		AIType = DPBwork[0];	//�����[�N(AI�p)
		FieldTrainerID		= ORUSUBAN_TRAINER;
		SBD_WORK->sex		= 0;
		SBD_WORK->name[0]	= hate_;
		SBD_WORK->name[1]	= EOM_;
		SBD_WORK->id[0]		= 0;		
		SBD_WORK->id[1]		= 0;		
		SBD_WORK->id[2]		= 0;		
		SBD_WORK->id[3]		= 0;		

		TaskTable[id].TaskAdrs = DPbattleSeq6;
		return;
	}
	if((sys.Repeat & U_KEY)){
		if(DPBsv.ai_cp > 0) DPBsv.ai_cp--;
		else{
			if(DPBsv.ai_lp > 0){
				DPBsv.ai_lp--;
				DPbattleTrainerListPut(DPBsv.ai_lp);
			}
		}
		TaskTable[id].TaskAdrs = DPbattleSeq3_3;
		return;
	}
	if((sys.Repeat & D_KEY)&&(DPBsv.ai_cp < DPB_AI_MAX-1)){
		if(DPBsv.ai_cp < 5) DPBsv.ai_cp++;
		else{
			if(DPBsv.ai_lp < DPB_AI_MAX-5-1){
				DPBsv.ai_lp++;
				DPbattleTrainerListPut(DPBsv.ai_lp);
			}
		}
		TaskTable[id].TaskAdrs = DPbattleSeq3_3;
		return;
	}
}

static	void	DPbattleTrainerListPut(u8 p)
{
	u8	i,j;
	u8	strbuf[3+12+1];
	u32	mask;

	for(i=0;i<6;i++){
		if((i+p) < DPB_AI_MAX){
			j = 0;
			while(((DPBaiMenuTable[i+p].label)[j]!=EOM_)&&(j<12)){
				strbuf[j+3] = (DPBaiMenuTable[i+p].label)[j];
				j++;
			}
			for(;j<12;j++)	strbuf[j+3] = spc_;
			strbuf[2+12] = EOM_;
			strbuf[0] = CTRL_;
			strbuf[1] = C_FCOL_;
			mask = 0x00000001 << (i+p);
			if(DPBwork[0] & mask){
				strbuf[2] = 3;
			}else{
				strbuf[2] = 1;
			}
			NMenuMsgWrite(strbuf,17,i*2+3);
		}
	}
}

/********************************************************************/
/*				�ΐ�n�`�I��										*/
/********************************************************************/
static	void	DPbattleSeq4(u8 id)
{
	u8	indexstr[] =	{ta_,i_,se_,n_,MO_,bou_,DO_,wo_,spc_,e_,ra_,n_,de_,ne_,EOM_};
	u8	index2str[] =	{CTRL_,C_FCOL_,2,S__,T__,A__,R__,T__,colon_,ka_,i_,si_,EOM_};
	u8	index3str[] =	{CTRL_,C_FCOL_,2,BA_,TO_,RU_,ti_,ke_,i_,spc_,se_,n_,ta_,ku_,EOM_};

	NMenuWinBoxWrite(0,16,29,19);
	NMenuMsgWrite(indexstr,1,17);
	NMenuMsgWrite(index2str,20,17);

	NMenuWinBoxWrite(0,0,14,15);
	NMenuMsgWrite(index3str,2,1);
	DPbattlePlaceListPut(DPBsv.place_lp);

	NMenuWinBoxWrite(15, 0,29,15);

	TaskTable[id].TaskAdrs = DPbattleSeq4_1;
}

/*	�J�[�\���\��(�n�`)	*/
static	void	DPbattleSeq4_1(u8 id)
{
	PmakeCursorPut(DPBsv.place_cp*2+3,1,1,14,1);

	*(vu16 *)REG_WIN1H	= (1*0x100)+119;
	*(vu16 *)REG_WIN1V	= 1*0x100+127;

	TaskTable[id].TaskAdrs = DPbattleSeq4_2;
}

/*	�J�[�\���ړ�(�n�`)	*/
static	void	DPbattleSeq4_2(u8 id)
{
	u8	lp;

	lp = DPBsv.place_lp + DPBsv.place_cp;

	if(sys.Trg & B_BUTTON){
//		TaskTable[id].TaskAdrs = DPbattleSeq3;
		TaskTable[id].TaskAdrs = DPbattleSeq0;
		return;
	}
	if((sys.Trg & START_BUTTON)||(sys.Trg & A_BUTTON)){
		SePlay(SE_SELECT);
		GroundNo = DPBplaceMenuTable[lp].place_id;
		TaskTable[id].TaskAdrs = DPbattleSeq5;
		return;
	}
	if((sys.Repeat & U_KEY)){
		if(DPBsv.place_cp > 0) DPBsv.place_cp--;
		else{
			if(DPBsv.place_lp > 0){
				DPBsv.place_lp--;
				DPbattlePlaceListPut(DPBsv.place_lp);
			}
		}
		TaskTable[id].TaskAdrs = DPbattleSeq4_1;
		return;
	}
	if((sys.Repeat & D_KEY)&&(DPBsv.place_cp < DPB_PLACE_MAX-1)){
		if(DPBsv.place_cp < 5) DPBsv.place_cp++;
		else{
			if(DPBsv.place_lp < DPB_PLACE_MAX-5-1){
				DPBsv.place_lp++;
				DPbattlePlaceListPut(DPBsv.place_lp);
			}
		}
		TaskTable[id].TaskAdrs = DPbattleSeq4_1;
		return;
	}
}

static	void	DPbattlePlaceListPut(u8 p)
{
	u8	i;

	NMenuBoxWrite(1, 3,13,14);
	for(i=0;i<6;i++){
		if(i < DPB_PLACE_MAX) NMenuMsgWrite(DPBplaceMenuTable[i+p].label,2,i*2+3);
	}
}

/********************************************************************/
/*				�����̐��ʑI��										*/
/********************************************************************/
static	void	DPbattleSeq6(u8 id)
{
	u8	indexstr[] =	{zi_,bu_,n_,no_,spc_,se_,i_,be_,tu_,wo_,spc_,e_,ra_,n_,de_,ne_,EOM_};
	u8	index2str[] =	{CTRL_,C_FCOL_,2,S__,T__,A__,R__,T__,colon_,tu_,gi_,he_,EOM_};
	u8	index3str[] =	{CTRL_,C_FCOL_,2,se_,i_,be_,tu_,spc_,se_,n_,ta_,ku_,EOM_};
	u8	list1[] =	{o_,to_,ko_,EOM_};
	u8	list2[] =	{o_,n_,na_,EOM_};

	NMenuWinBoxWrite(0,16,29,19);
	NMenuMsgWrite(indexstr,1,17);
	NMenuMsgWrite(index2str,20,17);

	NMenuWinBoxWrite(0,0,14,15);
	NMenuMsgWrite(index3str,2,1);

	NMenuBoxWrite(1, 3,13,14);
	NMenuMsgWrite(index3str,2,1);
	NMenuMsgWrite(list1,2,3);
	NMenuMsgWrite(list2,2,5);

	NMenuWinBoxWrite(15, 0,29,15);

	TaskTable[id].TaskAdrs = DPbattleSeq6_1;
}

/*	�J�[�\���\��(����)	*/
static	void	DPbattleSeq6_1(u8 id)
{
	PmakeCursorPut(DPBsv.sex_cp*2+3,1,1,14,1);

	*(vu16 *)REG_WIN1H	= (1*0x100)+119;
	*(vu16 *)REG_WIN1V	= 1*0x100+127;

	TaskTable[id].TaskAdrs = DPbattleSeq6_2;
}

/*	�J�[�\���ړ�(����)	*/
static	void	DPbattleSeq6_2(u8 id)
{
	if(sys.Trg & B_BUTTON){
		TaskTable[id].TaskAdrs = DPbattleSeq3;
		return;
	}
	if((sys.Trg & START_BUTTON)||(sys.Trg & A_BUTTON)){
		SePlay(SE_SELECT);
		MyData.my_sex = DPBsv.sex_cp;
		TaskTable[id].TaskAdrs = DPbattleSeq0;
		return;
	}
	if((sys.Repeat & U_KEY)&&(DPBsv.sex_cp > 0)){
		DPBsv.sex_cp--;
		TaskTable[id].TaskAdrs = DPbattleSeq6_1;
		return;
	}
	if((sys.Repeat & D_KEY)&&(DPBsv.sex_cp < 1)){
		DPBsv.sex_cp++;
		TaskTable[id].TaskAdrs = DPbattleSeq6_1;
		return;
	}
}

/********************************************************************/
/*				�ΐ�J�n											*/
/********************************************************************/
static	void	DPbattleSeq5(u8 id)
{
	TaskTable[id].TaskAdrs = DPbattleSeq5_1;
}

static	void	DPbattleSeq5_1(u8 id)
{
	TaskTable[id].TaskAdrs = DPbattleSeq5_2;
}

static	void	DPbattleSeq5_2(u8 id)
{
	TaskTable[id].TaskAdrs = DPbattleSeq5_3;
}

static	void	DPbattleSeq5_3(u8 id)
{
	TaskTable[id].TaskAdrs = DPbattleSeq5_4;
}

static	void	DPbattleSeq5_4(u8 id)
{
	TaskTable[id].TaskAdrs = DPbattleSeq5_5;
}

static	void	DPbattleSeq5_5(u8 id)
{
//	MusicPlay(MUS_SUIKUN);
	FightBGMSet();
	sys.pReturnProc	=	DebugPokeBattleInitReturn;			// �퓬�I����̕��A��
	MainProcChange(InitEncount);
}














/********************************************************************/
/********************************************************************/
/********************************************************************/
/*                                                                  */
/*				�|�P�����쐬										*/
/*                                                                  */
/********************************************************************/
/********************************************************************/
extern	const	u8	* const SeikakuMsgTbl[];
/********************************************************************/
static	void	PmakeSeq0(u8 id);
static	void	NumPutBuffer(u8* buf,u32 num,u8 cnt,u8 pal);
static	void	NamePutBuffer(u8* buf,u8* str,u8 cnt);
static	u8		putproc(u8* buf,u8 id,u8 pal);
static	u32		getproc(u8 id);
static	void	setproc(u32 dat,u8 id);
static	void	PagePut(void);
static	void	PageValuePut(void);
static	void	ValueControl(u8 mode);
static	void	CursorPut(u8 mode);
static	void	PokeMakePokeParaCalcInit(void);
static	void	PokeMakePokeParaCalcGetExp(void);
static	void	PokeMakePokeParaCalcGetBattleParam(void);
static	void	PokeMakePokeParaCalcEnd(void);

typedef struct
{
PokemonParam	PokeMakeData;
u32	PMD[64];
u32	data_bak;

u8	page_p;
u8	cursor_p;
u8	value_p;

u8	str[0x100];

}PokeMakeWork;

typedef struct
{
u32			min;
u32			max;
u8			mode;
u8			count;
}PmakeCont;

#define	PMC_INCDEC		(0)

#define	PMC_INIT		(0)
#define	PMC_INC			(1)
#define	PMC_DEC			(2)

typedef struct
{
const u8*			label;
const PmakeCont*	cont;
}PmakeParamData;

typedef struct
{
const u8*	page;
u8			listcount;
}PmakePageTable;

typedef struct
{
const u8*	name;
u32			setbit;
}PmakeCondition;

PokeMakeWork* dpw = {0};

#define	PMAKE_NAME		( 0)//�|�P�����i���o�[�i16bit�j
static	const u8		label00[]	= {si_,yyu_,ru_,i_,EOM_};
static	const PmakeCont	cont00		= {1,386,PMC_INCDEC,3};
#define	PMAKE_LEVEL		( 1)//���x���i8bit�j
static	const u8		label01[]	= {RE_,BE_,RU_,EOM_};
static	const PmakeCont	cont01		= {1,100,PMC_INCDEC,3};
#define	PMAKE_EXP		( 2)//�o���l�i32bit�j
static	const u8		label02[]	= {ke_,i_,ke_,n_,ti_,EOM_};
static	const PmakeCont	cont02		= {1,2000000,PMC_INCDEC,7};
#define	PMAKE_ID		( 3)//�h�c�i32bit�j
static	const u8		label03[]	= {I__,D__,EOM_};
static	const PmakeCont	cont03		= {0,0xffffffff,PMC_INCDEC,10};
#define	PMAKE_PERRND	( 4)//�̗����i32bit�j
static	const u8		label04[]	= {ko_,ta_,i_,spc_,ra_,n_,su_,u_,EOM_};
static	const PmakeCont	cont04		= {0,0xffffffff,PMC_INCDEC,10};
#define	PMAKE_SEX		( 5)//���ʁi8bit�j
static	const u8		label05[]	= {se_,i_,be_,tu_,EOM_};
static	const PmakeCont	cont05		= {0,2,PMC_INCDEC,0xff};
#define	PMAKE_PERSONAL	( 6)//���i�i8bit�j
static	const u8		label06[]	= {se_,i_,ka_,ku_,EOM_};
static	const PmakeCont	cont06		= {0,24,PMC_INCDEC,0xff};
#define	PMAKE_WAZA1		( 7)//�����Ă���Z�P�i8bit�j
static	const u8		label07[]	= {wa_,za_,n1_,EOM_};
static	const PmakeCont	cont07		= {0,WAZANO_MAX,PMC_INCDEC,3};
#define	PMAKE_WAZA2		( 8)//�����Ă���Z�Q�i8bit�j
static	const u8		label08[]	= {wa_,za_,n2_,EOM_};
static	const PmakeCont	cont08		= {0,WAZANO_MAX,PMC_INCDEC,3};
#define	PMAKE_WAZA3		( 9)//�����Ă���Z�R�i8bit�j
static	const u8		label09[]	= {wa_,za_,n3_,EOM_};
static	const PmakeCont	cont09		= {0,WAZANO_MAX,PMC_INCDEC,3};
#define	PMAKE_WAZA4		(10)//�����Ă���Z�S�i8bit�j
static	const u8		label10[]	= {wa_,za_,n4_,EOM_};
static	const PmakeCont	cont10		= {0,WAZANO_MAX,PMC_INCDEC,3};
#define	PMAKE_ITEM		(11)//�����A�C�e���i8bit�j
static	const u8		label11[]	= {so_,u_,bi_,do_,u_,gu_,EOM_};
static	const PmakeCont	cont11		= {0,255,PMC_INCDEC,3};
#define	PMAKE_SPABI		(12)//����\�́i1bit�j
static	const u8		label12[]	= {to_,ku_,se_,i_,EOM_};
static	const PmakeCont	cont12		= {0,1,PMC_INCDEC,0xff};
#define	PMAKE_HP_RND	(13)//�g�o�����i5bit�j
static	const u8		label13[]	= {H__,P__,r__,n__,d__,EOM_};
static	const PmakeCont	cont13		= {0,0x1f,PMC_INCDEC,2};
#define	PMAKE_HP_EXP	(14)//�g�o�o���l�i8bit�j
static	const u8		label14[]	= {H__,P__,e__,x__,p__,EOM_};
static	const PmakeCont	cont14		= {0,255,PMC_INCDEC,3};
#define	PMAKE_POW_RND	(15)//�������������i5bit�j
static	const u8		label15[]	= {ko_,u_,ge_,ki_,r__,n__,d__,EOM_};
static	const PmakeCont	cont15		= {0,0x1f,PMC_INCDEC,2};
#define	PMAKE_POW_EXP	(16)//���������o���l�i8bit�j
static	const u8		label16[]	= {ko_,u_,ge_,ki_,e__,x__,p__,EOM_};
static	const PmakeCont	cont16		= {0,255,PMC_INCDEC,3};
#define	PMAKE_DEF_RND	(17)//�ڂ����嗐���i5bit�j
static	const u8		label17[]	= {bo_,u_,gi_,yyo_,r__,n__,d__,EOM_};
static	const PmakeCont	cont17		= {0,0x1f,PMC_INCDEC,2};
#define	PMAKE_DEF_EXP	(18)//�ڂ�����o���l�i8bit�j
static	const u8		label18[]	= {bo_,u_,gi_,yyo_,e__,x__,p__,EOM_};
static	const PmakeCont	cont18		= {0,255,PMC_INCDEC,3};
#define	PMAKE_AGI_RND	(19)//���΂₳�����i5bit�j
static	const u8		label19[]	= {su_,ba_,ya_,sa_,r__,n__,d__,EOM_};
static	const PmakeCont	cont19		= {0,0x1f,PMC_INCDEC,2};
#define	PMAKE_AGI_EXP	(20)//���΂₳�o���l�i8bit�j
static	const u8		label20[]	= {su_,ba_,ya_,sa_,e__,x__,p__,EOM_};
static	const PmakeCont	cont20		= {0,255,PMC_INCDEC,3};
#define	PMAKE_EXPOW_RND	(21)//���ꂱ�����������i5bit�j
static	const u8		label21[]	= {to_,ku_,ko_,u_,r__,n__,d__,EOM_};
static	const PmakeCont	cont21		= {0,0x1f,PMC_INCDEC,2};
#define	PMAKE_EXPOW_EXP	(22)//���ꂱ�������o���l�i8bit�j
static	const u8		label22[]	= {to_,ku_,ko_,u_,e__,x__,p__,EOM_};
static	const PmakeCont	cont22		= {0,255,PMC_INCDEC,3};
#define	PMAKE_EXDEF_RND	(23)//����ڂ����嗐���i5bit�j
static	const u8		label23[]	= {to_,ku_,bo_,u_,r__,n__,d__,EOM_};
static	const PmakeCont	cont23		= {0,0x1f,PMC_INCDEC,2};
#define	PMAKE_EXDEF_EXP	(24)//����ڂ�����o���l�i8bit�j
static	const u8		label24[]	= {to_,ku_,bo_,u_,e__,x__,p__,EOM_};
static	const PmakeCont	cont24		= {0,255,PMC_INCDEC,3};
#define	PMAKE_STYLE		(25)//�������悳�i8bit�j
static	const u8		label25[]	= {ka_,ttu_,ko_,yo_,sa_,EOM_};
static	const PmakeCont	cont25		= {0,255,PMC_INCDEC,3};
#define	PMAKE_BEAUTIFUL	(26)//���������i8bit�j
static	const u8		label26[]	= {u_,tu_,ku_,si_,sa_,EOM_};
static	const PmakeCont	cont26		= {0,255,PMC_INCDEC,3};
#define	PMAKE_CUTE		(27)//���킢���i8bit�j
static	const u8		label27[]	= {ka_,wa_,i_,sa_,EOM_};
static	const PmakeCont	cont27		= {0,255,PMC_INCDEC,3};
#define	PMAKE_CLEVER	(28)//���������i8bit�j
static	const u8		label28[]	= {ka_,si_,ko_,sa_,EOM_};
static	const PmakeCont	cont28		= {0,255,PMC_INCDEC,3};
#define	PMAKE_STRONG	(29)//�����܂����i8bit�j
static	const u8		label29[]	= {ta_,ku_,ma_,si_,sa_,EOM_};
static	const PmakeCont	cont29		= {0,255,PMC_INCDEC,3};
#define	PMAKE_FUR		(30)//�щ��i8bit�j
static	const u8		label30[]	= {ke_,du_,ya_,no_,yo_,sa_,EOM_};
static	const PmakeCont	cont30		= {0,255,PMC_INCDEC,3};
#define	PMAKE_FRIEND	(31)//�Ȃ��x�i8bit�j
static	const u8		label31[]	= {na_,tu_,ki_,do_,EOM_};
static	const PmakeCont	cont31		= {0,255,PMC_INCDEC,3};
#define	PMAKE_POKERUS	(32)//�|�P���X�i8bit�j
static	const u8		label32[]	= {PO_,KE_,RU_,SU_,EOM_};
static	const PmakeCont	cont32		= {0,255,PMC_INCDEC,3};
#define	PMAKE_TAMAGO	(33)//�^�}�S��ԃt���O�i1bit�j
static	const u8		label33[]	= {ta_,ma_,go_,EOM_};
static	const PmakeCont	cont33		= {0,1,PMC_INCDEC,0xff};
#define	PMAKE_CONDITION	(34)//���炾�̏�ԁi32bit�j
static	const u8		label34[]	= {zi_,yyo_,u_,ta_,i_,EOM_};
static	const PmakeCont	cont34		= {0,6,PMC_INCDEC,0xff};

//�ȉ��A����ł��Ȃ�
#define	PMAKE_HP	(35)//�g�o
static	const u8		label35[]	= {CTRL_,C_FCOL_,2,H__,P__,spc_,spc_,EOM_};
#define	PMAKE_POW	(36)//��������
static	const u8		label36[]	= {CTRL_,C_FCOL_,2,ko_,u_,ge_,ki_,EOM_};
#define	PMAKE_DEF	(37)//�ڂ�����
static	const u8		label37[]	= {CTRL_,C_FCOL_,2,bo_,u_,gi_,yyo_,EOM_};
#define	PMAKE_AGI	(38)//���΂₳
static	const u8		label38[]	= {CTRL_,C_FCOL_,2,su_,ba_,ya_,sa_,EOM_};
#define	PMAKE_EXPOW	(39)//���ꂱ������
static	const u8		label39[]	= {CTRL_,C_FCOL_,2,to_,ku_,ko_,u_,EOM_};
#define	PMAKE_EXDEF	(40)//����ڂ�����
static	const u8		label40[]	= {CTRL_,C_FCOL_,2,to_,ku_,bo_,u_,EOM_};

#define PMAKE_BATTLEPARAM1	(0xfe)
#define PMAKE_BATTLEPARAM2	(0xfd)

static	const u8	str00[]	= {na_,si_,EOM_};
static	const u8	str01[]	= {osu_,EOM_};
static	const u8	str02[]	= {mesu_,EOM_};
static	const u8	str03[]	= {bou_,EOM_};
static	const u8	str04[]	= {O__,N__,EOM_};
static	const u8	str05[]	= {O__,F__,F__,EOM_};

static	const PmakeParamData	PMakelabelTable[] = {
{label00,&cont00},{label01,&cont01},{label02,&cont02},{label03,&cont03},{label04,&cont04},
{label05,&cont05},{label06,&cont06},{label07,&cont07},{label08,&cont08},{label09,&cont09},
{label10,&cont10},{label11,&cont11},{label12,&cont12},{label13,&cont13},{label14,&cont14},
{label15,&cont15},{label16,&cont16},{label17,&cont17},{label18,&cont18},{label19,&cont19},
{label20,&cont20},{label21,&cont21},{label22,&cont22},{label23,&cont23},{label24,&cont24},
{label25,&cont25},{label26,&cont26},{label27,&cont27},{label28,&cont28},{label29,&cont29},
{label30,&cont30},{label31,&cont31},{label32,&cont32},{label33,&cont33},{label34,&cont34},
};

static	const u8	Page1[] = {
PMAKE_NAME,PMAKE_LEVEL,PMAKE_EXP,PMAKE_ID,PMAKE_PERRND,PMAKE_SEX,PMAKE_PERSONAL,
0xff};
static	const u8	Page2[] = {
PMAKE_CONDITION,PMAKE_FRIEND,PMAKE_POKERUS,
//PMAKE_TAMAGO,
0xff};
static	const u8	Page3[] = {
PMAKE_WAZA1,PMAKE_WAZA2,PMAKE_WAZA3,PMAKE_WAZA4,PMAKE_ITEM,PMAKE_SPABI,
0xff};
static	const u8	Page4[] = {
PMAKE_HP_RND,PMAKE_HP_EXP,PMAKE_POW_RND,PMAKE_POW_EXP,PMAKE_DEF_RND,PMAKE_DEF_EXP,
PMAKE_BATTLEPARAM1,0xff};
static	const u8	Page5[] = {
PMAKE_AGI_RND,PMAKE_AGI_EXP,PMAKE_EXPOW_RND,PMAKE_EXPOW_EXP,PMAKE_EXDEF_RND,PMAKE_EXDEF_EXP,
PMAKE_BATTLEPARAM2,0xff};
static	const u8	Page6[] = {
PMAKE_STYLE,PMAKE_BEAUTIFUL,PMAKE_CUTE,PMAKE_CLEVER,PMAKE_STRONG,PMAKE_FUR,
0xff};

static	const	PmakePageTable	PageTable[] =
{{Page1,5},{Page2,3},{Page3,6},{Page4,6},{Page5,6},{Page6,6}};

static	const u8	condname1[]	= {hu_,tu_,u_,EOM_};
static	const u8	condname2[]	= {ne_,mu_,ri_,EOM_};
static	const u8	condname3[]	= {do_,ku_,EOM_};
static	const u8	condname4[]	= {ya_,ke_,do_,EOM_};
static	const u8	condname5[]	= {ko_,o_,ri_,EOM_};
static	const u8	condname6[]	= {ma_,hi_,EOM_};
static	const u8	condname7[]	= {do_,ku_,do_,ku_,EOM_};

static	const PmakeCondition	CondTable[] = {
{condname1,0x00000000},	//�ʏ�
{condname2,0x00000001},	//����	�i0x00000007�j
{condname3,0x00000008},	//��	�i0x00000008�j
{condname4,0x00000010},	//�₯�ǁi0x00000010�j
{condname5,0x00000020},	//������i0x00000020�j
{condname6,0x00000040},	//�܂�	�i0x00000040�j
{condname7,0x00000080},	//�ǂ��ǂ��i0x00000080�j
};

static	const u8	PokeMakeStr1[]	= 
{PO_,KE_,MO_,N_,wo_,spc_,te_,mo_,ti_,ni_,spc_,ku_,wa_,e_,ma_,si_,ta_,EOM_};
static	const u8	PokeMakeStr2[]	= 
{PO_,KE_,MO_,N_,wo_,spc_,BO_,TTU_,KU_,SU_,ni_,spc_,ku_,wa_,e_,ma_,si_,ta_,EOM_};

/********************************************************************/
/*				�쐬�f�[�^������									*/
/********************************************************************/
static	void	PokeMakeWorkInit(void)
{
	u16	i;

	dpw = (PokeMakeWork*)(&UserWork[0x18000]);

	for(i=0;i<64;i++)	dpw->PMD[i]	= 0;
	//�|�P�����i���o�[�i�쐬���͑S���}�Ӄi���o�[�j
	dpw->PMD[PMAKE_NAME] = 252;	
	//�h�c
	dpw->PMD[PMAKE_ID] = (MyData.ID[0]<<0)|(MyData.ID[1]<<8)
						|(MyData.ID[2]<<16)|(MyData.ID[3]<<24);
	//�̗���
	dpw->PMD[PMAKE_PERRND] = (pp_rand()<<16)|(pp_rand());
	//�o���l�����x��
	dpw->PMD[PMAKE_LEVEL]	= 1;	
	dpw->PMD[PMAKE_EXP]		= 1;	
	//�����Ă���Z
	//�R���e�X�g�p�p�����[�^
	//�퓬�p�p�����[�^
	//���̑����
	PokeMakePokeParaCalcInit();
}

/********************************************************************/
/*				�莝���|�P�����Z�b�g								*/
/********************************************************************/
static	u8	PokeMakeCopy(PokemonParam* data)
{
	u8	i;

	for(i=0;i<6;i++){
		if(PokeParaGet(&PokeParaMine[i],ID_monsno,0) == 0){
			PokeParaMine[i] = *data;
			PokeCountMine=i+1;
			return 0;
		}
	}
	//�莝���������ς��Ȃ̂ŁA�{�b�N�X�]��
	AddPasoPokemon(&(dpw->PokeMakeData));
	return 1;
}

/********************************************************************/
/*				������												*/
/********************************************************************/
void	PokeMakeInit(void)
{
	u8	taskID;
	u8	indexstr[] =	{PO_,KE_,MO_,N_,sa_,ku_,se_,i_,spc_,
						 spc_,L__,R__,colon_,ki_,ri_,ka_,e_,spc_,
						 S__,T__,A__,R__,T__,colon_,ku_,wa_,e_,ru_,
						 EOM_};

	InitRegWork();

	PaletteFadeReq(0xffffffff,0,16,0,0);

   	*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 0;

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
	*(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(FadeJob);

	NMenuWinBoxWrite(0,0,29,3);
	NMenuMsgWrite(indexstr,1,1);

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_OBJ_ON;
	taskID = AddTask(PmakeSeq0, 0);
	PokeMakeWorkInit();
	dpw->page_p		= 0;
	dpw->cursor_p	= 0;
}

/********************************************************************/
/*				��|�P�����I��									*/
/********************************************************************/
static	void	PmakeSeq0_1(u8 id);
static	void	PmakeSeq0_2(u8 id);
static	void	PmakeSeq0_3(u8 id);
static	void	PmakeSeq0_4(u8 id);
/*	������	*/
static	void	PmakeSeq0(u8 id)
{
	TaskTable[id].TaskAdrs = PmakeSeq0_1;
}

/*	�X�N���[���`��	*/
static	void	PmakeSeq0_1(u8 id)
{
	PagePut();
	CursorPut(PMC_INIT);
	TaskTable[id].TaskAdrs = PmakeSeq0_2;
}

/*	���ڑI�� */
static	void	PmakeSeq0_2(u8 id)
{
	u8	f;

	if(sys.Trg & A_BUTTON){
		//�ύX
		dpw->data_bak = dpw->PMD[PageTable[dpw->page_p].page[dpw->cursor_p]];
		dpw->value_p = 0;
		PageValuePut();
		TaskTable[id].TaskAdrs = PmakeSeq0_3;
		return;
	}
	if(sys.Trg & B_BUTTON){
		//��߂�
		PaletteFadeReq(0xffffffff,0,0,16,0);
		MainProcChange(FadeJob);
		TaskTable[id].TaskAdrs = ExitTaskSeq;
		return;
	}
	if(sys.Trg & START_BUTTON){
		//�|�P�����p�����[�^�[�ݒ�
		SePlay(SE_SELECT);
		PokeMakePokeParaCalcEnd();
		f = PokeMakeCopy(&(dpw->PokeMakeData));
		NMenuWinBoxWrite(3,8,26,11);
		if(f)	NMenuMsgWrite(PokeMakeStr2,4,9);
		else	NMenuMsgWrite(PokeMakeStr1,4,9);
		TaskTable[id].TaskAdrs = PmakeSeq0_4;
		return;
	}
	if(sys.Trg & U_KEY){
		CursorPut(PMC_DEC);
		return;
	}
	if(sys.Trg & D_KEY){
		CursorPut(PMC_INC);
		return;
	}
	if((sys.Trg & L_BUTTON)&&(dpw->page_p > 0)){
		dpw->page_p--;
		dpw->cursor_p = 0;
		TaskTable[id].TaskAdrs = PmakeSeq0_1;
		return;
	}
	if((sys.Trg & R_BUTTON)&&(dpw->page_p < 5)){
		dpw->page_p++;
		dpw->cursor_p = 0;
		TaskTable[id].TaskAdrs = PmakeSeq0_1;
		return;
	}
}

/*	���ڕύX */
static	void	PmakeSeq0_3(u8 id)
{
	u8	data_id;

	data_id = PageTable[dpw->page_p].page[dpw->cursor_p];

	if(sys.Trg & A_BUTTON){
		//����
		TaskTable[id].TaskAdrs = PmakeSeq0_1;
		return;
	}
	if(sys.Trg & B_BUTTON){
		//�L�����Z��
		dpw->PMD[data_id] = dpw->data_bak;
		TaskTable[id].TaskAdrs = PmakeSeq0_1;
		return;
	}
	if(sys.Repeat & U_KEY){
		ValueControl(PMC_INC);
		PageValuePut();
		return;
	}
	if(sys.Repeat & D_KEY){
		ValueControl(PMC_DEC);
		PageValuePut();
		return;
	}
	if(PMakelabelTable[data_id].cont->count != 0xff){
		if((sys.Trg & L_KEY)&&(dpw->value_p < PMakelabelTable[data_id].cont->count-1)){
			dpw->value_p++;
			PageValuePut();
			return;
		}
		if((sys.Trg & R_KEY)&&(dpw->value_p > 0)){
			dpw->value_p--;
			PageValuePut();
			return;
		}
	}
}

static	void	PmakeSeq0_4(u8 id)
{
	if(sys.Trg & A_BUTTON){
		TaskTable[id].TaskAdrs = PmakeSeq0_1;
		return;
	}
}

/********************************************************************/
/*				�e���ڕ\������										*/
/*				�e���ڐݒ萧��										*/
/********************************************************************/
//�\���f�[�^�쐬
static	u8	putproc(u8* buf,u8 id,u8 pal)
{
	u16	i;
	u8	vp,vpl;

	if(id == PMAKE_BATTLEPARAM1){
		NamePutBuffer(buf,(u8*)label35,7);
		NumPutBuffer(buf+7,dpw->PMD[PMAKE_HP],3,1);
		buf[19]=spc_;
		buf[20]=spc_;
		NamePutBuffer(buf+21,(u8*)label36,7);
		NumPutBuffer(buf+21+7,dpw->PMD[PMAKE_POW],3,1);
		buf[40]=spc_;
		buf[41]=spc_;
		NamePutBuffer(buf+42,(u8*)label37,9);
		NumPutBuffer(buf+42+7,dpw->PMD[PMAKE_DEF],3,1);
		return 0;
	}
	if(id == PMAKE_BATTLEPARAM2){
		NamePutBuffer(buf,(u8*)label38,7);
		NumPutBuffer(buf+7,dpw->PMD[PMAKE_AGI],3,1);
		buf[19]=spc_;
		buf[20]=spc_;
		NamePutBuffer(buf+21,(u8*)label39,7);
		NumPutBuffer(buf+21+7,dpw->PMD[PMAKE_EXPOW],3,1);
		buf[40]=spc_;
		buf[41]=spc_;
		NamePutBuffer(buf+42,(u8*)label40,9);
		NumPutBuffer(buf+42+7,dpw->PMD[PMAKE_EXDEF],3,1);
		return 0;
	}

	for(i=0;i<10;i++)		buf[i] = spc_;
	NamePutBuffer(buf,(u8*)PMakelabelTable[id].label,9);

	for(i=10;i<0x100;i++)	buf[i] = EOM_;
	vp	= PMakelabelTable[id].cont->count;
	vpl = vp*4;

	switch(id){

	default:
		buf[0] = EOM_;
		break;

	case PMAKE_NAME:
		NumPutBuffer(buf+10,dpw->PMD[id],vp,pal);
		buf[10+vpl] = spc_;
		buf[11+vpl] = CTRL_;
		buf[12+vpl] = C_FCOL_;
		buf[13+vpl] = pal;
		NamePutBuffer(buf+14+vpl,(u8*)PokeNameData[PokeZukanNo2MonsNoGet(dpw->PMD[id])],5);
		break;
	case PMAKE_SEX:
		switch(dpw->PMD[id]){
		case 0:
			NamePutBuffer(buf+10,(u8*)str02,2);	//���X
			break;
		case 1:
			NamePutBuffer(buf+10,(u8*)str00,2);	//�Ȃ�
			break;
		case 2:
			NamePutBuffer(buf+10,(u8*)str01,2);	//�I�X
			break;
		}
		break;
	case PMAKE_WAZA1:
	case PMAKE_WAZA2:
	case PMAKE_WAZA3:
	case PMAKE_WAZA4:
		NumPutBuffer(buf+10,dpw->PMD[id],vp,pal);
		buf[10+vpl] = spc_;
		buf[11+vpl] = CTRL_;
		buf[12+vpl] = C_FCOL_;
		buf[13+vpl] = pal;
		NamePutBuffer(buf+14+vpl,(u8*)WazaNameData[dpw->PMD[id]],10);
		break;
	case PMAKE_ITEM:
		NumPutBuffer(buf+10,dpw->PMD[id],vp,pal);
		buf[10+vpl] = spc_;
		buf[11+vpl] = CTRL_;
		buf[12+vpl] = C_FCOL_;
		buf[13+vpl] = pal;
//		if(dpw->PMD[id])	NamePutBuffer(buf+14+vpl,(u8*)ItemData[dpw->PMD[id]].name,9);
		if(dpw->PMD[id])	NamePutBuffer(buf+14+vpl,(u8*)ItemNameGet(dpw->PMD[id]),9);
		else				NamePutBuffer(buf+14+vpl,(u8*)str00,9);
		break;
	case PMAKE_SPABI:
		NamePutBuffer(buf+10,(u8*)SpecialAbirityMsg[
			SpecialAbirityGet(PokeZukanNo2MonsNoGet(dpw->PMD[PMAKE_NAME]),dpw->PMD[id])
			],8);
		break;
	case PMAKE_CONDITION:
		NamePutBuffer(buf+10,(u8*)CondTable[dpw->PMD[id]].name,4);
		break;
	case PMAKE_ID:
	case PMAKE_PERRND:
	case PMAKE_HP_RND:
	case PMAKE_POW_RND:
	case PMAKE_DEF_RND:
	case PMAKE_AGI_RND:
	case PMAKE_EXPOW_RND:
	case PMAKE_EXDEF_RND:
	case PMAKE_LEVEL:
	case PMAKE_HP_EXP:
	case PMAKE_POW_EXP:
	case PMAKE_DEF_EXP:
	case PMAKE_AGI_EXP:
	case PMAKE_EXPOW_EXP:
	case PMAKE_EXDEF_EXP:
	case PMAKE_STYLE:
	case PMAKE_BEAUTIFUL:
	case PMAKE_CUTE:
	case PMAKE_CLEVER:
	case PMAKE_STRONG:
	case PMAKE_FUR:
	case PMAKE_FRIEND:
	case PMAKE_POKERUS:
	case PMAKE_EXP:
		NumPutBuffer(buf+10,dpw->PMD[id],vp,pal);
		break;
	case PMAKE_TAMAGO:
		if(dpw->PMD[id])	NamePutBuffer(buf+10,(u8*)str04,4);
		else				NamePutBuffer(buf+10,(u8*)str05,4);
		break;
	case PMAKE_PERSONAL:
		NamePutBuffer(buf+10,(u8*)SeikakuMsgTbl[dpw->PMD[id]],5);
		break;
	}
	return	0;
}

//�f�[�^�擾
static	u32		getproc(u8 id)
{
	u32	dat;

	switch(id){

	default:
		dat	= dpw->PMD[id];
		break;
	}
	return	dat;
}

//�f�[�^�Z�b�g
static	void	setproc(u32 dat,u8 id)
{
	switch(id){

	default:
		dpw->PMD[id] = dat;
		break;
	case PMAKE_NAME:
	case PMAKE_PERRND:
	case PMAKE_ID:
		dpw->PMD[id] = dat;
		PokeMakePokeParaCalcInit();
		break;
	case PMAKE_LEVEL:
		dpw->PMD[id] = dat;
		dpw->PMD[PMAKE_EXP] 
			= GrowTbl[PPD[PokeZukanNo2MonsNoGet(dpw->PMD[PMAKE_NAME])].grow][dat];
		PokeMakePokeParaCalcInit();
		break;
	case PMAKE_EXP:
		dpw->PMD[id] = dat;
		PokeMakePokeParaCalcGetExp();
		break;
	case PMAKE_HP_RND:
	case PMAKE_POW_RND:
	case PMAKE_DEF_RND:
	case PMAKE_AGI_RND:
	case PMAKE_EXPOW_RND:
	case PMAKE_EXDEF_RND:
	case PMAKE_HP_EXP:
	case PMAKE_POW_EXP:
	case PMAKE_DEF_EXP:
	case PMAKE_AGI_EXP:
	case PMAKE_EXPOW_EXP:
	case PMAKE_EXDEF_EXP:
		dpw->PMD[id] = dat;
		PokeMakePokeParaCalcGetBattleParam();
		break;
	}
}

/********************************************************************/
/*				�f�[�^�v�Z											*/
/********************************************************************/
static	void	PokeMakePokeParaCalcInit(void)
{
	u32	pow_rnd;

	pow_rnd =	((dpw->PMD[PMAKE_HP_RND]&0x0000001f)<< 0)
			   |((dpw->PMD[PMAKE_POW_RND]&0x0000001f)<< 5)
			   |((dpw->PMD[PMAKE_DEF_RND]&0x0000001f)<< 10)
			   |((dpw->PMD[PMAKE_AGI_RND]&0x0000001f)<< 15)
			   |((dpw->PMD[PMAKE_EXPOW_RND]&0x0000001f)<< 20)
			   |((dpw->PMD[PMAKE_EXDEF_RND]&0x0000001f)<< 25);
	
	PokeParaSet(&(dpw->PokeMakeData),
				PokeZukanNo2MonsNoGet(dpw->PMD[PMAKE_NAME]),
				dpw->PMD[PMAKE_LEVEL],
				pow_rnd,
				1,dpw->PMD[PMAKE_PERRND],
				1,dpw->PMD[PMAKE_ID]);

	dpw->PMD[PMAKE_SEX]			= ((u8)(PokeSexGet(&(dpw->PokeMakeData))+2));
	dpw->PMD[PMAKE_PERSONAL]	= PokeSeikakuGet(&(dpw->PokeMakeData));
	dpw->PMD[PMAKE_WAZA1]		= PokeParaGet(&(dpw->PokeMakeData),ID_waza1);
	dpw->PMD[PMAKE_WAZA2]		= PokeParaGet(&(dpw->PokeMakeData),ID_waza2);
	dpw->PMD[PMAKE_WAZA3]		= PokeParaGet(&(dpw->PokeMakeData),ID_waza3);
	dpw->PMD[PMAKE_WAZA4]		= PokeParaGet(&(dpw->PokeMakeData),ID_waza4);
	dpw->PMD[PMAKE_EXP]			= PokeParaGet(&(dpw->PokeMakeData),ID_exp);

	dpw->PMD[PMAKE_HP]			= PokeParaGet(&(dpw->PokeMakeData),ID_hpmax);
	dpw->PMD[PMAKE_POW]			= PokeParaGet(&(dpw->PokeMakeData),ID_pow);
	dpw->PMD[PMAKE_DEF]			= PokeParaGet(&(dpw->PokeMakeData),ID_def);
	dpw->PMD[PMAKE_AGI]			= PokeParaGet(&(dpw->PokeMakeData),ID_agi);
	dpw->PMD[PMAKE_EXPOW]		= PokeParaGet(&(dpw->PokeMakeData),ID_spepow);
	dpw->PMD[PMAKE_EXDEF]		= PokeParaGet(&(dpw->PokeMakeData),ID_spedef);
}

static	void	PokeMakePokeParaCalcGetExp(void)
{
	u32	nulldata;

	nulldata = 0;

	PokeParaPut(&(dpw->PokeMakeData),ID_exp,(u8*)(&(dpw->PMD[PMAKE_EXP])));
	//�Z�N���A
	PokeParaPut(&(dpw->PokeMakeData),ID_waza1,(u8 *)&nulldata);
	PokeParaPut(&(dpw->PokeMakeData),ID_waza2,(u8 *)&nulldata);
	PokeParaPut(&(dpw->PokeMakeData),ID_waza3,(u8 *)&nulldata);
	PokeParaPut(&(dpw->PokeMakeData),ID_waza4,(u8 *)&nulldata);

	PokeParaCalc(&(dpw->PokeMakeData));
	PokeWazaOboe(&(dpw->PokeMakeData));

	dpw->PMD[PMAKE_LEVEL]		= PokeParaGet(&(dpw->PokeMakeData),ID_level);
	dpw->PMD[PMAKE_WAZA1]		= PokeParaGet(&(dpw->PokeMakeData),ID_waza1);
	dpw->PMD[PMAKE_WAZA2]		= PokeParaGet(&(dpw->PokeMakeData),ID_waza2);
	dpw->PMD[PMAKE_WAZA3]		= PokeParaGet(&(dpw->PokeMakeData),ID_waza3);
	dpw->PMD[PMAKE_WAZA4]		= PokeParaGet(&(dpw->PokeMakeData),ID_waza4);

	dpw->PMD[PMAKE_HP]			= PokeParaGet(&(dpw->PokeMakeData),ID_hpmax);
	dpw->PMD[PMAKE_POW]			= PokeParaGet(&(dpw->PokeMakeData),ID_pow);
	dpw->PMD[PMAKE_DEF]			= PokeParaGet(&(dpw->PokeMakeData),ID_def);
	dpw->PMD[PMAKE_AGI]			= PokeParaGet(&(dpw->PokeMakeData),ID_agi);
	dpw->PMD[PMAKE_EXPOW]		= PokeParaGet(&(dpw->PokeMakeData),ID_spepow);
	dpw->PMD[PMAKE_EXDEF]		= PokeParaGet(&(dpw->PokeMakeData),ID_spedef);
}

static	void	PokeMakePokeParaCalcGetBattleParam(void)
{
	PokeParaPut(&(dpw->PokeMakeData),ID_hp_rnd,(u8*)(&(dpw->PMD[PMAKE_HP_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_hp_exp,(u8*)(&(dpw->PMD[PMAKE_HP_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_pow_rnd,(u8*)(&(dpw->PMD[PMAKE_POW_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_pow_exp,(u8*)(&(dpw->PMD[PMAKE_POW_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_def_rnd,(u8*)(&(dpw->PMD[PMAKE_DEF_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_def_exp,(u8*)(&(dpw->PMD[PMAKE_DEF_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_agi_rnd,(u8*)(&(dpw->PMD[PMAKE_AGI_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_agi_exp,(u8*)(&(dpw->PMD[PMAKE_AGI_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_spepow_rnd,(u8*)(&(dpw->PMD[PMAKE_EXPOW_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_spepow_exp,(u8*)(&(dpw->PMD[PMAKE_EXPOW_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_spedef_rnd,(u8*)(&(dpw->PMD[PMAKE_EXDEF_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_spedef_exp,(u8*)(&(dpw->PMD[PMAKE_EXDEF_EXP])));

	PokeParaCalc(&(dpw->PokeMakeData));

	dpw->PMD[PMAKE_HP]			= PokeParaGet(&(dpw->PokeMakeData),ID_hpmax);
	dpw->PMD[PMAKE_POW]			= PokeParaGet(&(dpw->PokeMakeData),ID_pow);
	dpw->PMD[PMAKE_DEF]			= PokeParaGet(&(dpw->PokeMakeData),ID_def);
	dpw->PMD[PMAKE_AGI]			= PokeParaGet(&(dpw->PokeMakeData),ID_agi);
	dpw->PMD[PMAKE_EXPOW]		= PokeParaGet(&(dpw->PokeMakeData),ID_spepow);
	dpw->PMD[PMAKE_EXDEF]		= PokeParaGet(&(dpw->PokeMakeData),ID_spedef);
}

static	void	PokeMakePokeParaCalcEnd(void)
{
	u32	pow_rnd;
	u8	tamago_name[] = {ta_,ma_,go_,EOM_};
	u8	debug_name[] = {DE_,BA_,TTU_,GU_,PO_,KE_,n1_,EOM_};
	u16	i;

	pow_rnd =	((dpw->PMD[PMAKE_HP_RND]&0x0000001f)<< 0)
			   |((dpw->PMD[PMAKE_POW_RND]&0x0000001f)<< 5)
			   |((dpw->PMD[PMAKE_DEF_RND]&0x0000001f)<< 10)
			   |((dpw->PMD[PMAKE_AGI_RND]&0x0000001f)<< 15)
			   |((dpw->PMD[PMAKE_EXPOW_RND]&0x0000001f)<< 20)
			   |((dpw->PMD[PMAKE_EXDEF_RND]&0x0000001f)<< 25);
	
	PokeParaSet(&(dpw->PokeMakeData),
				PokeZukanNo2MonsNoGet(dpw->PMD[PMAKE_NAME]),
				dpw->PMD[PMAKE_LEVEL],
				pow_rnd,
				1,dpw->PMD[PMAKE_PERRND],
				1,dpw->PMD[PMAKE_ID]);

	if(dpw->PMD[PMAKE_WAZA1]){
		PokeWazaSetPos(&(dpw->PokeMakeData),dpw->PMD[PMAKE_WAZA1],0);
	}
	if(dpw->PMD[PMAKE_WAZA2]){
		PokeWazaSetPos(&(dpw->PokeMakeData),dpw->PMD[PMAKE_WAZA2],1);
	}
	if(dpw->PMD[PMAKE_WAZA3]){
		PokeWazaSetPos(&(dpw->PokeMakeData),dpw->PMD[PMAKE_WAZA3],2);
	}
	if(dpw->PMD[PMAKE_WAZA4]){
		PokeWazaSetPos(&(dpw->PokeMakeData),dpw->PMD[PMAKE_WAZA4],3);
	}
	PokeParaPut(&(dpw->PokeMakeData),ID_exp,(u8*)(&(dpw->PMD[PMAKE_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_hp_rnd,(u8*)(&(dpw->PMD[PMAKE_HP_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_hp_exp,(u8*)(&(dpw->PMD[PMAKE_HP_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_pow_rnd,(u8*)(&(dpw->PMD[PMAKE_POW_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_pow_exp,(u8*)(&(dpw->PMD[PMAKE_POW_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_def_rnd,(u8*)(&(dpw->PMD[PMAKE_DEF_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_def_exp,(u8*)(&(dpw->PMD[PMAKE_DEF_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_agi_rnd,(u8*)(&(dpw->PMD[PMAKE_AGI_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_agi_exp,(u8*)(&(dpw->PMD[PMAKE_AGI_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_spepow_rnd,(u8*)(&(dpw->PMD[PMAKE_EXPOW_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_spepow_exp,(u8*)(&(dpw->PMD[PMAKE_EXPOW_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_spedef_rnd,(u8*)(&(dpw->PMD[PMAKE_EXDEF_RND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_spedef_exp,(u8*)(&(dpw->PMD[PMAKE_EXDEF_EXP])));
	PokeParaPut(&(dpw->PokeMakeData),ID_style,(u8*)(&(dpw->PMD[PMAKE_STYLE])));
	PokeParaPut(&(dpw->PokeMakeData),ID_beautiful,(u8*)(&(dpw->PMD[PMAKE_BEAUTIFUL])));
	PokeParaPut(&(dpw->PokeMakeData),ID_cute,(u8*)(&(dpw->PMD[PMAKE_CUTE])));
	PokeParaPut(&(dpw->PokeMakeData),ID_clever,(u8*)(&(dpw->PMD[PMAKE_CLEVER])));
	PokeParaPut(&(dpw->PokeMakeData),ID_strong,(u8*)(&(dpw->PMD[PMAKE_STRONG])));
	PokeParaPut(&(dpw->PokeMakeData),ID_fur,(u8*)(&(dpw->PMD[PMAKE_FUR])));
	PokeParaPut(&(dpw->PokeMakeData),ID_friend,(u8*)(&(dpw->PMD[PMAKE_FRIEND])));
	PokeParaPut(&(dpw->PokeMakeData),ID_pokerus,(u8*)(&(dpw->PMD[PMAKE_POKERUS])));
	PokeParaPut(&(dpw->PokeMakeData),ID_item,(u8*)(&(dpw->PMD[PMAKE_ITEM])));
	PokeParaPut(&(dpw->PokeMakeData),ID_speabi,(u8*)(&(dpw->PMD[PMAKE_SPABI])));
	PokeParaPut(&(dpw->PokeMakeData),ID_condition,
			(u8*)(&(CondTable[dpw->PMD[PMAKE_CONDITION]].setbit)));
	PokeParaPut(&(dpw->PokeMakeData),ID_tamago_flag,(u8*)(&(dpw->PMD[PMAKE_TAMAGO])));
	if(dpw->PMD[PMAKE_TAMAGO]){
		PokeParaPut(&(dpw->PokeMakeData),ID_nickname,(u8*)tamago_name);	//���O�Ɂu�^�}�S�v�Z�b�g
	}
	PokeParaPut(&(dpw->PokeMakeData),ID_oyaname,(u8*)debug_name);//�e���Ɂu�f�o�b�O�|�P�P�v�Z�b�g
	i = 255;
	PokeParaPut(&(dpw->PokeMakeData),ID_get_place,(u8*)&i);//�e���Ɂu�f�o�b�O�|�P�P�v�Z�b�g

	PokeParaCalc(&(dpw->PokeMakeData));
}

/********************************************************************/
/*				�T�u												*/
/********************************************************************/
static	void	CursorPut(u8 mode)
{
	u8	cursor_str[]={cursor_,EOM_};
	u8	spc_str[]={spc_,EOM_};

	switch(mode){

	case PMC_INIT:
		NMenuMsgWrite(cursor_str, 1,5+dpw->cursor_p*2);
		break;
	case PMC_INC:
		if(dpw->cursor_p < (PageTable[dpw->page_p].listcount-1)){
			NMenuMsgWrite(spc_str, 1,5+dpw->cursor_p*2);
			dpw->cursor_p++;
			NMenuMsgWrite(cursor_str, 1,5+dpw->cursor_p*2);
		}
		break;
	case PMC_DEC:
		if(dpw->cursor_p > 0){
			NMenuMsgWrite(spc_str, 1,5+dpw->cursor_p*2);
			dpw->cursor_p--;
			NMenuMsgWrite(cursor_str, 1,5+dpw->cursor_p*2);
		}
		break;
	}
}

/********************************************************************/
static	void	PagePut(void)
{
	u16	i;

	NMenuWinBoxWrite( 0, 4,29,19);
	i = 0;
	while(PageTable[dpw->page_p].page[i]!=0xff){
		dpw->str[0] = CTRL_;
		dpw->str[1] = C_FCOL_;
		dpw->str[2] = 1;
		putproc(&dpw->str[3],PageTable[dpw->page_p].page[i],1);
		NMenuMsgWrite(dpw->str,2,5+i*2);
		i++;
	}
}

/********************************************************************/
static	void	PageValuePut(void)
{
	u16	i;

	NMenuBoxWrite( 2, 5,28,18);
	i = 0;
	while(PageTable[dpw->page_p].page[i]!=0xff){
		dpw->str[0] = CTRL_;
		dpw->str[1] = C_FCOL_;
		if(i==dpw->cursor_p){
			dpw->str[2] = 2;
			putproc(&dpw->str[3],PageTable[dpw->page_p].page[i],2);
		}else{
			dpw->str[2] = 1;
			putproc(&dpw->str[3],PageTable[dpw->page_p].page[i],1);
		}
		NMenuMsgWrite(dpw->str,2,5+i*2);
		i++;
	}
}

/********************************************************************/
static	const	u32	PmakeValueTable[] =
{1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};

static	void	ValueControl(u8 mode)
{
	u32	data,datamin,datamax,value;

	data	= getproc(PageTable[dpw->page_p].page[dpw->cursor_p]);
	datamin = PMakelabelTable[PageTable[dpw->page_p].page[dpw->cursor_p]].cont->min;
	datamax = PMakelabelTable[PageTable[dpw->page_p].page[dpw->cursor_p]].cont->max;
	value	= PmakeValueTable[dpw->value_p];

	switch(PMakelabelTable[PageTable[dpw->page_p].page[dpw->cursor_p]].cont->mode){
	case PMC_INCDEC:
		switch(mode){
		case PMC_INC:
			if(data < datamax-value)	data+=value;
			else{
				if(data < datamax)	data = datamax;
				else				data = datamin;
			}
			break;
		case PMC_DEC:
			if(data > datamin+value)	data-=value;
			else{
				if(data > datamin)	data = datamin;
				else				data = datamax;
			}
			break;
		}
		break;
	}
	setproc(data,PageTable[dpw->page_p].page[dpw->cursor_p]);
}

/********************************************************************/
static	void	NumPutBufferSet(u8* buf,u8 num,u8 pal,u8 cp)
{
	*buf = CTRL_;
	buf++;
	*buf = C_FCOL_;
	buf++;
	if(pal == 2){
		if(cp == dpw->value_p)	*buf = 4;
		else					*buf = 2;
	}else{
		*buf = 1;
	}
	buf++;
	*buf = n0_ + num;
}
//�i���o�[�f�[�^�쐬
static	void	NumPutBuffer(u8* buf,u32 num,u8 cnt,u8 pal)
{
	u8	i;

	i = 0;

	switch(cnt){
	default:
	case 10:
		NumPutBufferSet(&buf[i],num/1000000000,pal,9);
//		buf[i] = n0_ + num/1000000000;
		num = num%1000000000;
		i+=4;
	case 9:
		NumPutBufferSet(&buf[i],num/100000000,pal,8);
		num = num%100000000;
		i+=4;
	case 8:
		NumPutBufferSet(&buf[i],num/10000000,pal,7);
		num = num%10000000;
		i+=4;
	case 7:
		NumPutBufferSet(&buf[i],num/1000000,pal,6);
		num = num%1000000;
		i+=4;
	case 6:
		NumPutBufferSet(&buf[i],num/100000,pal,5);
		num = num%100000;
		i+=4;
	case 5:
		NumPutBufferSet(&buf[i],num/10000,pal,4);
		num = num%10000;
		i+=4;
	case 4:
		NumPutBufferSet(&buf[i],num/1000,pal,3);
		num = num%1000;
		i+=4;
	case 3:
		NumPutBufferSet(&buf[i],num/100,pal,2);
		num = num%100;
		i+=4;
	case 2:
		NumPutBufferSet(&buf[i],num/10,pal,1);
		num = num%10;
		i+=4;
	case 1:
		NumPutBufferSet(&buf[i],num,pal,0);
//		buf[i] = n0_ + num;
	}
}

/********************************************************************/
//�����f�[�^�쐬
static	void	NamePutBuffer(u8* buf,u8* str,u8 cnt)
{
	u8	i;

	i = 0;
	while((str[i]!=EOM_)&&(i<cnt)){
		buf[i] = str[i];
		i++;
	}
//	str[cnt] = EOM_;
}


































/********************************************************************/
/********************************************************************/
/********************************************************************/
/*                                                                  */
/*				�O���t�B�b�N�m�F									*/
/*                                                                  */
/********************************************************************/
/********************************************************************/
/********************************************************************/
static	void	GcheckSeq1(u8 id);
static	void	GcheckSeq2(u8 id);
static	void	GcheckSeq3(u8 id);
static	void	GcheckSeq4(u8 id);
static	void	GcheckSeq5(u8 id);
static	void	GcheckSeq6(u8 id);
static	void	GcheckSeq7(u8 id);

static	const u16	gra_chk_Palette[16*3];
static	const u8	gra_chk_Character[2048];
static	const actHeader GraChkActHeader;

static	const u16	CursorPalette[32] = {
0x0000,0x0002,0x0004,0x0006,0x0008,0x000a,0x000c,0x000e,
0x0010,0x0012,0x0014,0x0016,0x0018,0x001a,0x001c,0x001e,
0x001f,0x001e,0x001c,0x001a,0x0018,0x0016,0x0014,0x0012,
0x0010,0x000e,0x000c,0x000a,0x0008,0x0006,0x0004,0x0002,
};

typedef struct
{
u16	R:5;
u16	G:5;
u16	B:5;
u16	mb:1;

}COLSTRUCT;

typedef struct
{
u16			pokenum;
u8			pactnum1;
u8			pactnum2;
u8			pactnum3;

u8			mode;
u8			mons_palno;
u8			col_p;
u8			cur_timer;

u8			col_mode;
u8			rgb_p;
COLSTRUCT	Col;
u16			pal_bak[16];

}GraChkWork;

GraChkWork* gcw = {0};

static	void	ColDataPut(u16 data,u8 x,u8 y,u8 len)
{
	u8	i,offs;
	u8	numstr[5];

	for(i=0;i<4;i++)	numstr[i] = spc_;
	numstr[4] = EOM_;

	switch(len){
	default:
	case 4:
		numstr[0] = (data&0xf000)>>12;
	case 3:
		numstr[1] = (data&0x0f00)>>8;
	case 2:
		numstr[2] = (data&0x00f0)>>4;
	case 1:
		numstr[3] = (data&0x000f)>>0;
	}
	for(i=0;i<4;i++){
		if(numstr[i] <= 9){
			numstr[i] += n0_;
		}else{
			numstr[i] += (A__-10);
		}
	}
	offs = 4-len;
	NMenuMsgWrite(&numstr[offs],x,y);
}

static	const u8	gc_indexstr0L[]=	{CTRL_,C_FCOL_,2,L__,CTRL_,C_EXFONT_,ArrowL__,EOM_};
static	const u8	gc_indexstr0R[]=	{CTRL_,C_FCOL_,2,CTRL_,C_EXFONT_,ArrowR__,R__,EOM_};
static	const u8	gc_indexstr1[]	=	{ka_,i_,ha_,tu_,NA_,N_,BA_,bou_,EOM_};
static	const u8	gc_indexstr2[] =	{GU_,RA_,HU_,II_,TTU_,KU_,NA_,N_,BA_,bou_,EOM_};
static	const u8	gc_indexstr3[] =	{CTRL_,C_FCOL_,2,A__,colon_,KA_,RA_,bou_,se_,ttu_,te_,i_,EOM_};
static	const u8	gc_indexstr4[] =	{CTRL_,C_FCOL_,2,A__,colon_,ke_,ttu_,te_,i_,spc_,
						B__,colon_,KI_,YYA_,N_,SE_,RU_,EOM_};
static	const u8	gc_indexColonstr[]={colon_,EOM_};

/********************************************************************/
/*				������												*/
/********************************************************************/
void	GraphicCheckInit(void)
{
	u8	taskID,act_no;
	u16	pal;

	InitRegWork();

	DIV_DMACOPY(3,gra_chk_Character	,BG_VRAM+0xe000	,0x800	,16);	//BG CGX�]��
	PaletteWorkSet(gra_chk_Palette,PA_BG8,0x60);				//BG0&BG1 COL�]��
	pal = 0x7fff;
	PaletteWorkSet(&pal,PA_BG0,0x02);

	PaletteFadeReq(0xffffffff,0,16,0,0);

	*(vu16 *)REG_WIN0H	= 0;
	*(vu16 *)REG_WIN0V	= 0;
	*(vu16 *)REG_WIN1H	= 0;
	*(vu16 *)REG_WIN1V	= 0;

	*(vu16 *)REG_WININ	= 0x003f;
 	*(vu16 *)REG_WINOUT	= 0x001f;

   	*(vu16 *)REG_BLDCNT   = 0x00f1;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 7;

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(FadeJob);
	PokeIconPaletteAllSet();

	*(vu16 *)REG_BG0CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| 31 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 2 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
	*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 30 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 2 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_BG1_ON					// BG1�L��
						  | DISP_WIN0_ON				// WIN0 ON
						  | DISP_OBJ_ON;
	taskID = AddTask(GcheckSeq1, 0);
	gcw = (GraChkWork*)(&UserWork[0x18000]);
	gcw->pokenum	= 277;
	gcw->pactnum1	= 0;
	gcw->pactnum2	= 0;
	gcw->mode		= 0;
	gcw->col_p		= 0;
	gcw->rgb_p		= 0;
	gcw->cur_timer	= 0;

	act_no = AddActor(&GraChkActHeader,108,116,0);
	ActWork[act_no].work[0] = 0;	//R
	ActAnmChg(&ActWork[act_no],0);
	act_no = AddActor(&GraChkActHeader,108,116,0);
	ActWork[act_no].work[0] = 1;	//G
	ActAnmChg(&ActWork[act_no],1);
	act_no = AddActor(&GraChkActHeader,108,116,0);
	ActWork[act_no].work[0] = 2;	//B
	ActAnmChg(&ActWork[act_no],2);
}

/********************************************************************/
/*				����												*/
/********************************************************************/
static	void	GcheckSeq1(u8 id)
{
	u8	i;
	u8	gc_RGBstr[]	=	{0,0x10,
						0x20,0x20,0x20,0x21,
						0x20,0x20,0x20,0x21,
						0x20,0x20,0x20,0x21,
						0x20,0x20,0x20,0x21};

	NMenuWinBoxWrite(10, 0,15, 7);
	NMenuWinBoxWrite( 0, 0, 9, 9);
	NMenuWinBoxWrite( 0,10, 9,19);

	NMenuWinBoxWrite(16, 0,29, 7);
	NMenuMsgWrite(gc_indexstr0L,17,1);
	NMenuMsgWrite(gc_indexstr0R,27,1);
	NMenuMsgWrite(gc_indexstr1	,17,5);

	NMenuWinBoxWrite(10, 8,29,12);
	for(i=0;i<15;i++){
		*(u16 *)(BG_VRAM+0xf800 + 9*64 + (11+i)*2) = (0x300+0x11+i)+0xa000;
	}
	for(i=0;i<15;i++){
		*(u16 *)(BG_VRAM+0xf000 + 9*64 + (11+i)*2) = (0x300+0x01+i)+0x8000;
	}
	NMenuMsgWrite(gc_indexstr3	,20,10);

	NMenuWinBoxWrite(10,13,29,19);
	gc_RGBstr[0] =	0x23;	//R
	for(i=0;i<18;i++){
		*(u16 *)(BG_VRAM+0xf000 + 14*64 + (11+i)*2) = (0x300+gc_RGBstr[i])+0x9000;
	}
	gc_RGBstr[0] =	0x24;	//G
	for(i=0;i<18;i++){
		*(u16 *)(BG_VRAM+0xf000 + 15*64 + (11+i)*2) = (0x300+gc_RGBstr[i])+0x9000;
	}
	gc_RGBstr[0] =	0x25;	//B
	for(i=0;i<18;i++){
		*(u16 *)(BG_VRAM+0xf000 + 16*64 + (11+i)*2) = (0x300+gc_RGBstr[i])+0x9000;
	}
	NMenuMsgWrite(gc_indexstr4	,15,17);

	*(vu16 *)REG_WIN0H	= (81*0x100)+239;
	*(vu16 *)REG_WIN0V	= (105*0x100)+159;
	TaskTable[id].TaskAdrs = GcheckSeq2;
}

/********************************************************************/
/*				��ذݕ`��											*/
/********************************************************************/
static	void	GcheckSeq2(u8 id)
{
	//�|�P�������ʕ\��
	DecordPokeCellMake( &PM2FObjDataCell[gcw->pokenum],
						pm2fpara[gcw->pokenum].size,
						pm2fpara[gcw->pokenum].patcnt,
						(void *)DecordAdrs[0],
						(void *)DecordAdrs[1],
						gcw->pokenum);
	DecordPalSet( (PalData*)&PM2NObjDataPal[gcw->pokenum] );
	PokeActHeaderMake( gcw->pokenum, 1 );
	gcw->pactnum1 = AddActor( &PokeActHeader, 32+8, 32+8, 0 );
	ActWork[gcw->pactnum1].move = actNoMove;
	ActWork[gcw->pactnum1].oamData.Priority = 0;

	//�|�P�����w�ʕ\��
	DecordPokeCellMake( &PM2BObjDataCell[gcw->pokenum],
						pm2bpara[gcw->pokenum].size,
						pm2bpara[gcw->pokenum].patcnt,
						(void *)DecordAdrs[0], 
						(void *)DecordAdrs[2],
						gcw->pokenum);
	DecordPalSet( (PalData*)&PM2NObjDataPal[gcw->pokenum] );
	PokeActHeaderMake( gcw->pokenum, 2 );
	gcw->pactnum2 = AddActor( &PokeActHeader, 32+8, 80+32+8, 0 );
	ActWork[gcw->pactnum2].move = actNoMove;
	ActWork[gcw->pactnum2].oamData.Priority = 0;

	gcw->pactnum3 = PokeIconSet2(gcw->pokenum,PokeIconDmmyMove,104,44,0,0);
	ZukanPokeFootTrans(PokeMonsNo2ZukanNoGet(gcw->pokenum),2,0x3fc);
	*(u16*)(BG_VRAM + 31*0x0800 + (1+0)*64 + (12+0)*2) = 0xf000 + 0x3fc+0;
	*(u16*)(BG_VRAM + 31*0x0800 + (1+0)*64 + (12+1)*2) = 0xf000 + 0x3fc+1;
	*(u16*)(BG_VRAM + 31*0x0800 + (1+1)*64 + (12+0)*2) = 0xf000 + 0x3fc+2;
	*(u16*)(BG_VRAM + 31*0x0800 + (1+1)*64 + (12+1)*2) = 0xf000 + 0x3fc+3;

	PokeNamePut(gcw->pokenum,17,3);

	//�O���t�B�b�N�f�[�^�i���o�[�\��
	NumPut(gcw->pokenum,26,5);

	gcw->mons_palno = ActWork[gcw->pactnum1].oamData.Pltt;
	CpuCopy(&PaletteWork[PA_OBJ0 + gcw->mons_palno*16],&PaletteWork[PA_BG8], 0x20, 16);
	CpuCopy(&PaletteWork[PA_OBJ0 + gcw->mons_palno*16],&PaletteWorkTrans[PA_BG8], 0x20, 16);
	TaskTable[id].TaskAdrs = GcheckSeq3;
	gcw->col_mode	= 0;
	VoicePlayStop();
	VoicePlay(gcw->pokenum,0);
}

/********************************************************************/
/*				�J���[�ݒ菉����									*/
/********************************************************************/
static	void	GcheckSeq3(u8 id)
{
	u16	col;

	CpuCopy(&PaletteWork[PA_BG8], &gcw->pal_bak[0], 0x20, 16);

	col = PaletteWork[PA_BG8+(gcw->col_p + PA_COL1)];
	gcw->Col.R	= (col & 0x001f)>>0;
	gcw->Col.G	= (col & 0x03e0)>>5;
	gcw->Col.B	= (col & 0x7c00)>>10;
	gcw->Col.mb = 0;

	ColDataPut(gcw->col_p+1,11,10,2);
	NMenuMsgWrite(gc_indexColonstr,13,10);
	ColDataPut(col,14,10,4);
	TaskTable[id].TaskAdrs = GcheckSeq4;
}

/********************************************************************/
/*				���C������											*/
/********************************************************************/
static	void	GcheckSeq4(u8 id)
{
	if(sys.Trg & B_BUTTON){
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);
		MainProcChange(FadeJob);
		TaskTable[id].TaskAdrs = ExitTaskSeq;
		return;
	}
	if(sys.Repeat & R_BUTTON){
		gcw->pokenum = PokeNumIncDec(NUM_INC,gcw->pokenum);
		TaskTable[id].TaskAdrs = GcheckSeq5;
		return;
	}
	if(sys.Repeat & L_BUTTON){
		gcw->pokenum = PokeNumIncDec(NUM_DEC,gcw->pokenum);
		TaskTable[id].TaskAdrs = GcheckSeq5;
		return;
	}
	if(sys.Trg & A_BUTTON){
		gcw->mode = 1;
		*(vu16 *)REG_WIN0H	= (81*0x100)+239;
		*(vu16 *)REG_WIN0V	= (65*0x100)+103;
		TaskTable[id].TaskAdrs = GcheckSeq6;
		return;
	}
	if((sys.Repeat & R_KEY)&&(gcw->col_p < 15-1)){
		*(u16 *)(BG_PLTT+(8*0x20) + (gcw->col_p+1)*2) = 0x0000;
		gcw->col_p++;
		TaskTable[id].TaskAdrs = GcheckSeq3;
		return;
	}
	if((sys.Repeat & L_KEY)&&(gcw->col_p > 0)){
		*(u16 *)(BG_PLTT+(8*0x20) + (gcw->col_p+1)*2) = 0x0000;
		gcw->col_p--;
		TaskTable[id].TaskAdrs = GcheckSeq3;
		return;
	}
	if(sys.Trg & START_BUTTON){
		gcw->col_mode^= 1;
		if(gcw->col_mode){
			DecordPaletteWork
				((void*)PokePalGetPara(gcw->pokenum,0,0),PA_OBJ0 + gcw->mons_palno*16,0x20 );
		}else{
			DecordPaletteWork
				((void*)PokePalGetPara(gcw->pokenum,0,9),PA_OBJ0 + gcw->mons_palno*16,0x20 );
		}
		CpuCopy(&PaletteWork[PA_OBJ0 + gcw->mons_palno*16],&PaletteWork[PA_BG8], 0x20, 16);
		CpuCopy(&PaletteWork[PA_OBJ0 + gcw->mons_palno*16],&PaletteWorkTrans[PA_BG8], 0x20, 16);
		TaskTable[id].TaskAdrs = GcheckSeq3;
		return;
	}
	if(sys.Trg & SELECT_BUTTON){
		VoicePlayStop();
		VoicePlay(gcw->pokenum,0);
	}
	gcw->cur_timer+=4;
	gcw->cur_timer &= 0x001f;
	*(u16 *)(BG_PLTT+(10*0x20) + (gcw->col_p+1)*2) = CursorPalette[gcw->cur_timer];
}

/********************************************************************/
/*				�|�P�����؂�ւ�									*/
/********************************************************************/
static	void	GcheckSeq5(u8 id)
{
	ObjPalRegNumDel( ObjPalNumIdGet(ActWork[gcw->pactnum1].oamData.Pltt) );
	DelActor( &ActWork[gcw->pactnum1] );
	ObjPalRegNumDel( ObjPalNumIdGet(ActWork[gcw->pactnum2].oamData.Pltt) );
	DelActor( &ActWork[gcw->pactnum2] );
	PokeIconDel( &ActWork[gcw->pactnum3] );

	TaskTable[id].TaskAdrs = GcheckSeq2;
}

/********************************************************************/
/*				�J���[�ݒ萧��										*/
/********************************************************************/
static	void	GcheckSeq6(u8 id)
{
	if(sys.Trg & A_BUTTON){
		gcw->mode = 0;
		*(vu16 *)REG_WIN0H	= (81*0x100)+239;
		*(vu16 *)REG_WIN0V	= (105*0x100)+159;
		TaskTable[id].TaskAdrs = GcheckSeq3;
		return;
	}
	if(sys.Trg & B_BUTTON){
		gcw->mode = 0;
		*(vu16 *)REG_WIN0H	= (81*0x100)+239;
		*(vu16 *)REG_WIN0V	= (105*0x100)+159;
		CpuCopy(&gcw->pal_bak[0],&PaletteWork[PA_BG8], 0x20, 16);
		CpuCopy(&gcw->pal_bak[0],&PaletteWorkTrans[PA_BG8], 0x20, 16);
		CpuCopy(&gcw->pal_bak[0],&PaletteWork[PA_OBJ0 + gcw->mons_palno*16], 0x20, 16);
		CpuCopy(&gcw->pal_bak[0],&PaletteWorkTrans[PA_OBJ0 + gcw->mons_palno*16], 0x20, 16);
		TaskTable[id].TaskAdrs = GcheckSeq3;
		return;
	}
	if((sys.Repeat & D_KEY)&&(gcw->rgb_p < 2)){
		gcw->rgb_p++;
		return;
	}
	if((sys.Repeat & U_KEY)&&(gcw->rgb_p > 0)){
		gcw->rgb_p--;
		return;
	}
	if(sys.Repeat & R_KEY){
		switch(gcw->rgb_p){
		case 0:
			if(gcw->Col.R < 31)	gcw->Col.R++;
			TaskTable[id].TaskAdrs = GcheckSeq7;
			break;
		case 1:
			if(gcw->Col.G < 31)	gcw->Col.G++;
			TaskTable[id].TaskAdrs = GcheckSeq7;
			break;
		case 2:
			if(gcw->Col.B < 31)	gcw->Col.B++;
			TaskTable[id].TaskAdrs = GcheckSeq7;
			break;
		}
		return;
	}
	if(sys.Repeat & L_KEY){
		switch(gcw->rgb_p){
		case 0:
			if(gcw->Col.R > 0)	gcw->Col.R--;
			TaskTable[id].TaskAdrs = GcheckSeq7;
			break;
		case 1:
			if(gcw->Col.G > 0)	gcw->Col.G--;
			TaskTable[id].TaskAdrs = GcheckSeq7;
			break;
		case 2:
			if(gcw->Col.B > 0)	gcw->Col.B--;
			TaskTable[id].TaskAdrs = GcheckSeq7;
			break;
		}
		return;
	}
}

/********************************************************************/
/*				�J���[�ύX											*/
/********************************************************************/
static	void	GcheckSeq7(u8 id)
{
	u16	col;

	col = (gcw->Col.R)+(gcw->Col.G<<5)+(gcw->Col.B<<10);
	PaletteWork[PA_BG8+(gcw->col_p + PA_COL1)] = col;
	PaletteWorkTrans[PA_BG8+(gcw->col_p + PA_COL1)] = col;
	PaletteWork[PA_OBJ0 + gcw->mons_palno*16 + (gcw->col_p + PA_COL1)] = col;
	PaletteWorkTrans[PA_OBJ0 + gcw->mons_palno*16 + (gcw->col_p + PA_COL1)] = col;
	ColDataPut(col,14,10,4);
	TaskTable[id].TaskAdrs = GcheckSeq6;
}

/********************************************************************/
/*				OBJ													*/
/********************************************************************/
#define	CELLID_GRACHK	0x1000
#define	PALID_GRACHK	0x1000

static	const	CellData	GraChkcell[] = {
	{gra_chk_Character,sizeof(gra_chk_Character),CELLID_GRACHK},{CELL_END,0,0}
};
static	const	PalData		GraChkpal[] = {
	{gra_chk_Palette,PALID_GRACHK},{OBJPAL_END,0},
};
static const ActOamData		GraphMeterOam = {160,0,0,0,0,0,0,0,0,0,0,0,0};
static const actAnm			GraphMeterRAnm[] = {{0x26,30,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm			GraphMeterGAnm[] = {{0x27,30,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm			GraphMeterBAnm[] = {{0x28,30,0,0},{ACT_ANMEND,0,0,0}};
static const actAnm* const	GraphMeterAnmTbl[]={GraphMeterRAnm,GraphMeterGAnm,GraphMeterBAnm};

static void	GraphMeterActMove(actWork* aw)
{
	u8	xpos;

	if(!gcw->mode){
		aw->banish = 1;
		return;
	}
	switch(aw->work[0]){
	default:
	case 0:	//R
		xpos = gcw->Col.R;
		break;
	case 1:	//G
		xpos = gcw->Col.G;
		break;
	case 2:	//B
		xpos = gcw->Col.B;
		break;
	}
	aw->dx = xpos*4;
	aw->dy = aw->work[0]*8;

	aw->work[1]++;
	if((aw->work[0] == gcw->rgb_p)&&(!(aw->work[1]&0x0008))){
		aw->banish = 1;
	}else{
		aw->banish = 0;
	}
}

static const actHeader GraChkActHeader ={
CELLID_GRACHK,	//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
PALID_GRACHK,	//��گ��Ȱ�ެ�o�^����
&GraphMeterOam,
GraphMeterAnmTbl,
ACT_NTRANS,
DummyActAffTbl,
GraphMeterActMove,
};









/********************************************************************/
/********************************************************************/
/********************************************************************/
/*                                                                  */
/*				�O���t�B�b�N�m�F�i�g���[�i�[�j						*/
/*                                                                  */
/********************************************************************/
/********************************************************************/
/********************************************************************/
static	void	Gcheck2Seq1(u8 id);
static	void	Gcheck2Seq2(u8 id);
static	void	Gcheck2Seq3(u8 id);
static	void	Gcheck2Seq4(u8 id);
static	void	Gcheck2Seq5(u8 id);
static	void	Gcheck2Seq6(u8 id);
static	void	Gcheck2Seq7(u8 id);

/********************************************************************/
/*				������												*/
/********************************************************************/
void	GraphicCheck2Init(void)
{
	u8	taskID,act_no;

	InitRegWork();

	DIV_DMACOPY(3,gra_chk_Character	,BG_VRAM+0xe000	,0x800	,16);	//BG CGX�]��
	PaletteWorkSet(gra_chk_Palette,PA_BG8,0x60);				//BG0&BG1 COL�]��

	PaletteFadeReq(0xffffffff,0,16,0,0);

	*(vu16 *)REG_WIN0H	= 0;
	*(vu16 *)REG_WIN0V	= 0;
	*(vu16 *)REG_WIN1H	= 0;
	*(vu16 *)REG_WIN1V	= 0;

	*(vu16 *)REG_WININ	= 0x003f;
 	*(vu16 *)REG_WINOUT	= 0x001f;

   	*(vu16 *)REG_BLDCNT   = 0x00f1;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 7;

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(FadeJob);

	*(vu16 *)REG_BG0CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| 31 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 2 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
	*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| 30 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 2 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_BG1_ON					// BG1�L��
						  | DISP_WIN0_ON				// WIN0 ON
						  | DISP_OBJ_ON;
	taskID = AddTask(Gcheck2Seq1, 0);
	gcw = (GraChkWork*)(&UserWork[0x18000]);
	gcw->pokenum	= 0;
	gcw->pactnum1	= 0;
	gcw->pactnum2	= 0;
	gcw->mode		= 0;
	gcw->col_p		= 0;
	gcw->rgb_p		= 0;
	gcw->cur_timer	= 0;

	act_no = AddActor(&GraChkActHeader,108,116,0);
	ActWork[act_no].work[0] = 0;	//R
	ActAnmChg(&ActWork[act_no],0);
	act_no = AddActor(&GraChkActHeader,108,116,0);
	ActWork[act_no].work[0] = 1;	//G
	ActAnmChg(&ActWork[act_no],1);
	act_no = AddActor(&GraChkActHeader,108,116,0);
	ActWork[act_no].work[0] = 2;	//B
	ActAnmChg(&ActWork[act_no],2);
}

/********************************************************************/
/*				����												*/
/********************************************************************/
static	void	Gcheck2Seq1(u8 id)
{
	u8	i;
	u8	gc_RGBstr[]	=	{0,0x10,
						0x20,0x20,0x20,0x21,
						0x20,0x20,0x20,0x21,
						0x20,0x20,0x20,0x21,
						0x20,0x20,0x20,0x21};

	NMenuWinBoxWrite( 9, 0,14, 7);
	NMenuWinBoxWrite( 0, 0, 9, 9);
	NMenuWinBoxWrite( 0,10, 9,19);

	NMenuWinBoxWrite(14, 0,29, 7);
	NMenuMsgWrite(gc_indexstr0L,15,1);
	NMenuMsgWrite(gc_indexstr0R,25,1);
	NMenuMsgWrite(gc_indexstr1	,15,5);

	NMenuWinBoxWrite(10, 8,29,12);
	for(i=0;i<15;i++){
		*(u16 *)(BG_VRAM+0xf800 + 9*64 + (11+i)*2) = (0x300+0x11+i)+0xa000;
	}
	for(i=0;i<15;i++){
		*(u16 *)(BG_VRAM+0xf000 + 9*64 + (11+i)*2) = (0x300+0x01+i)+0x8000;
	}
	NMenuMsgWrite(gc_indexstr3	,20,10);

	NMenuWinBoxWrite(10,13,29,19);
	gc_RGBstr[0] =	0x23;	//R
	for(i=0;i<18;i++){
		*(u16 *)(BG_VRAM+0xf000 + 14*64 + (11+i)*2) = (0x300+gc_RGBstr[i])+0x9000;
	}
	gc_RGBstr[0] =	0x24;	//G
	for(i=0;i<18;i++){
		*(u16 *)(BG_VRAM+0xf000 + 15*64 + (11+i)*2) = (0x300+gc_RGBstr[i])+0x9000;
	}
	gc_RGBstr[0] =	0x25;	//B
	for(i=0;i<18;i++){
		*(u16 *)(BG_VRAM+0xf000 + 16*64 + (11+i)*2) = (0x300+gc_RGBstr[i])+0x9000;
	}
	NMenuMsgWrite(gc_indexstr4	,15,17);

	*(vu16 *)REG_WIN0H	= (81*0x100)+239;
	*(vu16 *)REG_WIN0V	= (105*0x100)+159;
	TaskTable[id].TaskAdrs = Gcheck2Seq2;
}

/********************************************************************/
/*				��ذݕ`��											*/
/********************************************************************/
static	void	Gcheck2Seq2(u8 id)
{
	//�g���[�i�[���ʕ\��
	DecordPokeCellMake( &TRFObjDataCell[gcw->pokenum],
						trfpara[gcw->pokenum].size,
						trfpara[gcw->pokenum].patcnt,
						(void *)DecordAdrs[0],
						(void *)DecordAdrs[1],
						gcw->pokenum);
	DecordPalSet( (PalData*)&TRFObjDataPal[gcw->pokenum] );

	TrainerActHeaderMake(gcw->pokenum,1);
	gcw->pactnum1 = AddActor( &PokeActHeader, 32+8, 32+8, 0 );
	ActWork[gcw->pactnum1].move = actNoMove;
	ActWork[gcw->pactnum1].oamData.Priority = 0;

//	TrainerNamePut(gcw->pokenum,17,3);

	//�O���t�B�b�N�f�[�^�i���o�[�\��
	NumPut(gcw->pokenum,26,5);

	gcw->mons_palno = ActWork[gcw->pactnum1].oamData.Pltt;
	CpuCopy(&PaletteWork[PA_OBJ0 + gcw->mons_palno*16],&PaletteWork[PA_BG8], 0x20, 16);
	CpuCopy(&PaletteWork[PA_OBJ0 + gcw->mons_palno*16],&PaletteWorkTrans[PA_BG8], 0x20, 16);
	TaskTable[id].TaskAdrs = Gcheck2Seq3;
	gcw->col_mode	= 0;
}

/********************************************************************/
/*				�J���[�ݒ菉����									*/
/********************************************************************/
static	void	Gcheck2Seq3(u8 id)
{
	u16	col;

	CpuCopy(&PaletteWork[PA_BG8], &gcw->pal_bak[0], 0x20, 16);

	col = PaletteWork[PA_BG8+(gcw->col_p + PA_COL1)];
	gcw->Col.R	= (col & 0x001f)>>0;
	gcw->Col.G	= (col & 0x03e0)>>5;
	gcw->Col.B	= (col & 0x7c00)>>10;
	gcw->Col.mb = 0;

	ColDataPut(gcw->col_p+1,11,10,2);
	NMenuMsgWrite(gc_indexColonstr,13,10);
	ColDataPut(col,14,10,4);
	TaskTable[id].TaskAdrs = Gcheck2Seq4;
}

/********************************************************************/
/*				���C������											*/
/********************************************************************/
static	void	Gcheck2Seq4(u8 id)
{
	if(sys.Trg & B_BUTTON){
		SePlay(SE_SELECT);
		PaletteFadeReq(0xffffffff,0,0,16,0);
		MainProcChange(FadeJob);
		TaskTable[id].TaskAdrs = ExitTaskSeq;
		return;
	}
	if(sys.Repeat & R_BUTTON){
		gcw->pokenum = TrainerNumIncDec(NUM_INC,gcw->pokenum);
		TaskTable[id].TaskAdrs = Gcheck2Seq5;
		return;
	}
	if(sys.Repeat & L_BUTTON){
		gcw->pokenum = TrainerNumIncDec(NUM_DEC,gcw->pokenum);
		TaskTable[id].TaskAdrs = Gcheck2Seq5;
		return;
	}
	if(sys.Trg & A_BUTTON){
		gcw->mode = 1;
		*(vu16 *)REG_WIN0H	= (81*0x100)+239;
		*(vu16 *)REG_WIN0V	= (65*0x100)+103;
		TaskTable[id].TaskAdrs = Gcheck2Seq6;
		return;
	}
	if((sys.Repeat & R_KEY)&&(gcw->col_p < 15-1)){
		*(u16 *)(BG_PLTT+(8*0x20) + (gcw->col_p+1)*2) = 0x0000;
		gcw->col_p++;
		TaskTable[id].TaskAdrs = Gcheck2Seq3;
		return;
	}
	if((sys.Repeat & L_KEY)&&(gcw->col_p > 0)){
		*(u16 *)(BG_PLTT+(8*0x20) + (gcw->col_p+1)*2) = 0x0000;
		gcw->col_p--;
		TaskTable[id].TaskAdrs = Gcheck2Seq3;
		return;
	}
	if(sys.Trg & START_BUTTON){
		gcw->col_mode^= 1;
		if(gcw->col_mode){
			DecordPaletteWork
				((void*)PokePalGetPara(gcw->pokenum,0,0),PA_OBJ0 + gcw->mons_palno*16,0x20 );
		}else{
			DecordPaletteWork
				((void*)PokePalGetPara(gcw->pokenum,0,9),PA_OBJ0 + gcw->mons_palno*16,0x20 );
		}
		CpuCopy(&PaletteWork[PA_OBJ0 + gcw->mons_palno*16],&PaletteWork[PA_BG8], 0x20, 16);
		CpuCopy(&PaletteWork[PA_OBJ0 + gcw->mons_palno*16],&PaletteWorkTrans[PA_BG8], 0x20, 16);
		TaskTable[id].TaskAdrs = Gcheck2Seq3;
		return;
	}
	gcw->cur_timer+=4;
	gcw->cur_timer &= 0x001f;
	*(u16 *)(BG_PLTT+(10*0x20) + (gcw->col_p+1)*2) = CursorPalette[gcw->cur_timer];
}

/********************************************************************/
/*				�|�P�����؂�ւ�									*/
/********************************************************************/
static	void	Gcheck2Seq5(u8 id)
{
	ObjPalRegNumDel( ObjPalNumIdGet(ActWork[gcw->pactnum1].oamData.Pltt) );
	DelActor( &ActWork[gcw->pactnum1] );
	ObjPalRegNumDel( ObjPalNumIdGet(ActWork[gcw->pactnum2].oamData.Pltt) );
	DelActor( &ActWork[gcw->pactnum2] );
	PokeIconDel( &ActWork[gcw->pactnum3] );

	TaskTable[id].TaskAdrs = Gcheck2Seq2;
}

/********************************************************************/
/*				�J���[�ݒ萧��										*/
/********************************************************************/
static	void	Gcheck2Seq6(u8 id)
{
	if(sys.Trg & A_BUTTON){
		gcw->mode = 0;
		*(vu16 *)REG_WIN0H	= (81*0x100)+239;
		*(vu16 *)REG_WIN0V	= (105*0x100)+159;
		TaskTable[id].TaskAdrs = Gcheck2Seq3;
		return;
	}
	if(sys.Trg & B_BUTTON){
		gcw->mode = 0;
		*(vu16 *)REG_WIN0H	= (81*0x100)+239;
		*(vu16 *)REG_WIN0V	= (105*0x100)+159;
		CpuCopy(&gcw->pal_bak[0],&PaletteWork[PA_BG8], 0x20, 16);
		CpuCopy(&gcw->pal_bak[0],&PaletteWorkTrans[PA_BG8], 0x20, 16);
		CpuCopy(&gcw->pal_bak[0],&PaletteWork[PA_OBJ0 + gcw->mons_palno*16], 0x20, 16);
		CpuCopy(&gcw->pal_bak[0],&PaletteWorkTrans[PA_OBJ0 + gcw->mons_palno*16], 0x20, 16);
		TaskTable[id].TaskAdrs = Gcheck2Seq3;
		return;
	}
	if((sys.Repeat & D_KEY)&&(gcw->rgb_p < 2)){
		gcw->rgb_p++;
		return;
	}
	if((sys.Repeat & U_KEY)&&(gcw->rgb_p > 0)){
		gcw->rgb_p--;
		return;
	}
	if(sys.Repeat & R_KEY){
		switch(gcw->rgb_p){
		case 0:
			if(gcw->Col.R < 31)	gcw->Col.R++;
			TaskTable[id].TaskAdrs = Gcheck2Seq7;
			break;
		case 1:
			if(gcw->Col.G < 31)	gcw->Col.G++;
			TaskTable[id].TaskAdrs = Gcheck2Seq7;
			break;
		case 2:
			if(gcw->Col.B < 31)	gcw->Col.B++;
			TaskTable[id].TaskAdrs = Gcheck2Seq7;
			break;
		}
		return;
	}
	if(sys.Repeat & L_KEY){
		switch(gcw->rgb_p){
		case 0:
			if(gcw->Col.R > 0)	gcw->Col.R--;
			TaskTable[id].TaskAdrs = Gcheck2Seq7;
			break;
		case 1:
			if(gcw->Col.G > 0)	gcw->Col.G--;
			TaskTable[id].TaskAdrs = Gcheck2Seq7;
			break;
		case 2:
			if(gcw->Col.B > 0)	gcw->Col.B--;
			TaskTable[id].TaskAdrs = Gcheck2Seq7;
			break;
		}
		return;
	}
}

/********************************************************************/
/*				�J���[�ύX											*/
/********************************************************************/
static	void	Gcheck2Seq7(u8 id)
{
	u16	col;

	col = (gcw->Col.R)+(gcw->Col.G<<5)+(gcw->Col.B<<10);
	PaletteWork[PA_BG8+(gcw->col_p + PA_COL1)] = col;
	PaletteWorkTrans[PA_BG8+(gcw->col_p + PA_COL1)] = col;
	PaletteWork[PA_OBJ0 + gcw->mons_palno*16 + (gcw->col_p + PA_COL1)] = col;
	PaletteWorkTrans[PA_OBJ0 + gcw->mons_palno*16 + (gcw->col_p + PA_COL1)] = col;
	ColDataPut(col,14,10,4);
	TaskTable[id].TaskAdrs = Gcheck2Seq6;
}












/********************************************************************/
/*				�f�[�^												*/
/********************************************************************/
static	const u16	gra_chk_Palette[16*3] = {
0x222E,0x001F,0x01FF,0x03FF,0x03EF,0x03E0,0x43E0,0x7FE0,
0x7DE0,0x7C00,0x7C0F,0x7C1F,0x3C1F,0x0000,0x3DEF,0x7FFF,
0x222E,0x001F,0x01FF,0x03FF,0x03EF,0x03E0,0x43E0,0x7FE0,
0x7DE0,0x7C00,0x7C0F,0x7C1F,0x3C1F,0x0000,0x3DEF,0x7FFF,
0x222E,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,
0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,
};
static	const u8	gra_chk_Character[2048] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x10,0x11,0x11,0x01,0x10,0x11,0x11,0x01,0x10,0x11,0x11,0x01,
0x10,0x11,0x11,0x01,0x10,0x11,0x11,0x01,0x10,0x11,0x11,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x22,0x22,0x02,0x20,0x22,0x22,0x02,0x20,0x22,0x22,0x02,
0x20,0x22,0x22,0x02,0x20,0x22,0x22,0x02,0x20,0x22,0x22,0x02,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x30,0x33,0x33,0x03,0x30,0x33,0x33,0x03,0x30,0x33,0x33,0x03,
0x30,0x33,0x33,0x03,0x30,0x33,0x33,0x03,0x30,0x33,0x33,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x40,0x44,0x44,0x04,0x40,0x44,0x44,0x04,0x40,0x44,0x44,0x04,
0x40,0x44,0x44,0x04,0x40,0x44,0x44,0x04,0x40,0x44,0x44,0x04,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x50,0x55,0x55,0x05,0x50,0x55,0x55,0x05,0x50,0x55,0x55,0x05,
0x50,0x55,0x55,0x05,0x50,0x55,0x55,0x05,0x50,0x55,0x55,0x05,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x60,0x66,0x66,0x06,0x60,0x66,0x66,0x06,0x60,0x66,0x66,0x06,
0x60,0x66,0x66,0x06,0x60,0x66,0x66,0x06,0x60,0x66,0x66,0x06,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x70,0x77,0x77,0x07,0x70,0x77,0x77,0x07,0x70,0x77,0x77,0x07,
0x70,0x77,0x77,0x07,0x70,0x77,0x77,0x07,0x70,0x77,0x77,0x07,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x80,0x88,0x88,0x08,0x80,0x88,0x88,0x08,0x80,0x88,0x88,0x08,
0x80,0x88,0x88,0x08,0x80,0x88,0x88,0x08,0x80,0x88,0x88,0x08,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x90,0x99,0x99,0x09,0x90,0x99,0x99,0x09,0x90,0x99,0x99,0x09,
0x90,0x99,0x99,0x09,0x90,0x99,0x99,0x09,0x90,0x99,0x99,0x09,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xA0,0xAA,0xAA,0x0A,0xA0,0xAA,0xAA,0x0A,0xA0,0xAA,0xAA,0x0A,
0xA0,0xAA,0xAA,0x0A,0xA0,0xAA,0xAA,0x0A,0xA0,0xAA,0xAA,0x0A,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xB0,0xBB,0xBB,0x0B,0xB0,0xBB,0xBB,0x0B,0xB0,0xBB,0xBB,0x0B,
0xB0,0xBB,0xBB,0x0B,0xB0,0xBB,0xBB,0x0B,0xB0,0xBB,0xBB,0x0B,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xC0,0xCC,0xCC,0x0C,0xC0,0xCC,0xCC,0x0C,0xC0,0xCC,0xCC,0x0C,
0xC0,0xCC,0xCC,0x0C,0xC0,0xCC,0xCC,0x0C,0xC0,0xCC,0xCC,0x0C,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xD0,0xDD,0xDD,0x0D,0xD0,0xDD,0xDD,0x0D,0xD0,0xDD,0xDD,0x0D,
0xD0,0xDD,0xDD,0x0D,0xD0,0xDD,0xDD,0x0D,0xD0,0xDD,0xDD,0x0D,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xE0,0xEE,0xEE,0x0E,0xE0,0xEE,0xEE,0x0E,0xE0,0xEE,0xEE,0x0E,
0xE0,0xEE,0xEE,0x0E,0xE0,0xEE,0xEE,0x0E,0xE0,0xEE,0xEE,0x0E,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x0F,0xF0,0xFF,0xFF,0x0F,0xF0,0xFF,0xFF,0x0F,
0xF0,0xFF,0xFF,0x0F,0xF0,0xFF,0xFF,0x0F,0xF0,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0D,0x00,0x00,0x00,0xDD,0x00,0x00,0x00,0xDD,0x0D,0x00,0x00,
0xDD,0xDD,0x00,0x00,0xDD,0xDD,0x0D,0x00,0xDD,0xDD,0xDD,0x00,0xDD,0xDD,0xDD,0xDD,
0x11,0x11,0x11,0x11,0x01,0x00,0x00,0x10,0x01,0x00,0x00,0x10,0x01,0x00,0x00,0x10,
0x01,0x00,0x00,0x10,0x01,0x00,0x00,0x10,0x01,0x00,0x00,0x10,0x11,0x11,0x11,0x11,
0x22,0x22,0x22,0x22,0x02,0x00,0x00,0x20,0x02,0x00,0x00,0x20,0x02,0x00,0x00,0x20,
0x02,0x00,0x00,0x20,0x02,0x00,0x00,0x20,0x02,0x00,0x00,0x20,0x22,0x22,0x22,0x22,
0x33,0x33,0x33,0x33,0x03,0x00,0x00,0x30,0x03,0x00,0x00,0x30,0x03,0x00,0x00,0x30,
0x03,0x00,0x00,0x30,0x03,0x00,0x00,0x30,0x03,0x00,0x00,0x30,0x33,0x33,0x33,0x33,
0x44,0x44,0x44,0x44,0x04,0x00,0x00,0x40,0x04,0x00,0x00,0x40,0x04,0x00,0x00,0x40,
0x04,0x00,0x00,0x40,0x04,0x00,0x00,0x40,0x04,0x00,0x00,0x40,0x44,0x44,0x44,0x44,
0x55,0x55,0x55,0x55,0x05,0x00,0x00,0x50,0x05,0x00,0x00,0x50,0x05,0x00,0x00,0x50,
0x05,0x00,0x00,0x50,0x05,0x00,0x00,0x50,0x05,0x00,0x00,0x50,0x55,0x55,0x55,0x55,
0x66,0x66,0x66,0x66,0x06,0x00,0x00,0x60,0x06,0x00,0x00,0x60,0x06,0x00,0x00,0x60,
0x06,0x00,0x00,0x60,0x06,0x00,0x00,0x60,0x06,0x00,0x00,0x60,0x66,0x66,0x66,0x66,
0x77,0x77,0x77,0x77,0x07,0x00,0x00,0x70,0x07,0x00,0x00,0x70,0x07,0x00,0x00,0x70,
0x07,0x00,0x00,0x70,0x07,0x00,0x00,0x70,0x07,0x00,0x00,0x70,0x77,0x77,0x77,0x77,
0x88,0x88,0x88,0x88,0x08,0x00,0x00,0x80,0x08,0x00,0x00,0x80,0x08,0x00,0x00,0x80,
0x08,0x00,0x00,0x80,0x08,0x00,0x00,0x80,0x08,0x00,0x00,0x80,0x88,0x88,0x88,0x88,
0x99,0x99,0x99,0x99,0x09,0x00,0x00,0x90,0x09,0x00,0x00,0x90,0x09,0x00,0x00,0x90,
0x09,0x00,0x00,0x90,0x09,0x00,0x00,0x90,0x09,0x00,0x00,0x90,0x99,0x99,0x99,0x99,
0xAA,0xAA,0xAA,0xAA,0x0A,0x00,0x00,0xA0,0x0A,0x00,0x00,0xA0,0x0A,0x00,0x00,0xA0,
0x0A,0x00,0x00,0xA0,0x0A,0x00,0x00,0xA0,0x0A,0x00,0x00,0xA0,0xAA,0xAA,0xAA,0xAA,
0xBB,0xBB,0xBB,0xBB,0x0B,0x00,0x00,0xB0,0x0B,0x00,0x00,0xB0,0x0B,0x00,0x00,0xB0,
0x0B,0x00,0x00,0xB0,0x0B,0x00,0x00,0xB0,0x0B,0x00,0x00,0xB0,0xBB,0xBB,0xBB,0xBB,
0xCC,0xCC,0xCC,0xCC,0x0C,0x00,0x00,0xC0,0x0C,0x00,0x00,0xC0,0x0C,0x00,0x00,0xC0,
0x0C,0x00,0x00,0xC0,0x0C,0x00,0x00,0xC0,0x0C,0x00,0x00,0xC0,0xCC,0xCC,0xCC,0xCC,
0xDD,0xDD,0xDD,0xDD,0x0D,0x00,0x00,0xD0,0x0D,0x00,0x00,0xD0,0x0D,0x00,0x00,0xD0,
0x0D,0x00,0x00,0xD0,0x0D,0x00,0x00,0xD0,0x0D,0x00,0x00,0xD0,0xDD,0xDD,0xDD,0xDD,
0xEE,0xEE,0xEE,0xEE,0x0E,0x00,0x00,0xE0,0x0E,0x00,0x00,0xE0,0x0E,0x00,0x00,0xE0,
0x0E,0x00,0x00,0xE0,0x0E,0x00,0x00,0xE0,0x0E,0x00,0x00,0xE0,0xEE,0xEE,0xEE,0xEE,
0xFF,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0xF0,0x0F,0x00,0x00,0xF0,0x0F,0x00,0x00,0xF0,
0x0F,0x00,0x00,0xF0,0x0F,0x00,0x00,0xF0,0x0F,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xD0,0x00,0xD0,0x00,0xD0,0x00,0xD0,0xDD,0xDD,0xDD,0xDD,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD0,
0x00,0x00,0x00,0xD0,0x00,0xD0,0x00,0xD0,0x00,0xD0,0x00,0xD0,0xDD,0xDD,0xDD,0xDD,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD0,0x00,0x00,
0x00,0xD0,0x00,0x00,0x00,0xD0,0x00,0xD0,0x00,0xD0,0x00,0xD0,0xDD,0xDD,0xDD,0xDD,
0xDD,0xDD,0xDD,0xDD,0x1D,0x11,0x11,0xD1,0x1D,0xD1,0x1D,0xD1,0x1D,0x11,0x11,0xD1,
0x1D,0xD1,0x1D,0xDD,0x1D,0xD1,0x1D,0xD1,0x1D,0xD1,0x1D,0xD1,0xDD,0xDD,0xDD,0xDD,
0xDD,0xDD,0xDD,0xDD,0x5D,0x55,0x55,0xD5,0x5D,0xD5,0xDD,0xDD,0x5D,0xD5,0x55,0xD5,
0x5D,0xD5,0x5D,0xD5,0x5D,0xD5,0x5D,0xD5,0x5D,0x55,0x55,0xD5,0xDD,0xDD,0xDD,0xDD,
0xDD,0xDD,0xDD,0xDD,0x8D,0x88,0x88,0xD8,0x8D,0xD8,0x8D,0xD8,0x8D,0x88,0x88,0xDD,
0x8D,0xD8,0x8D,0xD8,0x8D,0xD8,0x8D,0xD8,0x8D,0x88,0x88,0xD8,0xDD,0xDD,0xDD,0xDD,
0x00,0x00,0x00,0x00,0xDD,0xDD,0xDD,0x0D,0x1D,0x11,0x11,0x0D,0xD0,0x11,0xD1,0x00,
0x00,0x1D,0x0D,0x00,0x00,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xDD,0xDD,0xDD,0x0D,0x5D,0x55,0x55,0x0D,0xD0,0x55,0xD5,0x00,
0x00,0x5D,0x0D,0x00,0x00,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xDD,0xDD,0xDD,0x0D,0x8D,0x88,0x88,0x0D,0xD0,0x88,0xD8,0x00,
0x00,0x8D,0x0D,0x00,0x00,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};




#endif	PM_DEBUG
