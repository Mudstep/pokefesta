//=========================================================================
//	
//	�|�P�����I��(�I�[�v�j���O�C�x���g)
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
#include "zukan.h"
#include "fld_eff.h"
#include "calctool.h"
#include "ev_flag.h"
#include "mus_tool.h"
#include "monsno.def"
#include "message.h"
#include "laster.h"
#include "opselect.dat"

void	OpPokeSelEvInit(void);
void	OpPokeSelEvMain(void);

extern	const	u32	DecordAdrs[];
extern	void	VoicePlay( u16 no, s8 pan );

static	const	CellData	ObjCell1[];
static	const	CellData	ObjCell2[];
static	const	PalData		ObjPal[];

static	void	OpPokeSelEvSeq1InitTask(u8 id);
static	void	OpPokeSelEvSeq1Task(u8 id);
static	void	OpPokeSelEvSeq1EffectTask(u8 id);
static	void	OpPokeSelEvSeq2InitTask(u8 id);
static	void	OpPokeSelEvSeq2Task(u8 id);
static	void	OpPokeSelEvSeq2RetTask(u8 id);
static	void	BallInfoWindowSet(u8 clr_p,u8 w_p);

static	u8		PokePut(u16 poke_num,u8 x,u8 y);
static	const	actHeader CursorActHeader;
static	const	actHeader MonsterBallActHeader;
static	const	actHeader BallWindowActHeader;
static	const	actAffAnm*	const	PokemonAffTable[];
static	void	BallWindowActMove(actWork* aw);

static	const u8	BallPosTable[3][2]		= {{ 60, 64},{120, 88},{180, 64}};
static	const u8	BallInfoPosTable[3][2]	= {{ 3, 9},{17,10},{12, 4}};

static	const u16	SelectPokemonNumTable[3]= {
MONSNO_KIMORI,		//�u�W�������v
MONSNO_ATYAMO,		//�u�A�`�����v		
MONSNO_MIZUGOROU,	//�u�~�Y�S���E�v
};

/********************************************************************/
/*                                                                  */
/*				�I���|�P�����i���o�[�擾							*/
/*                                                                  */
/********************************************************************/
u16	FirstPokemonNumGet(u16	index)
{
	if(index >3)	index = 0;		//Error���
	return	SelectPokemonNumTable[index];
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
void	OpPokeSelEvInit(void)
{
	u8	taskID;
	u16 act_no;

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

	LasterInit();									// HDMA������
	InitTask();										// ���  ������
	InitActor();									// ���� ������
	PaletteAnimeInit();
	ObjPalManInit();								// ��޼ު��گ��Ȱ�ެ������

	PaletteWorkSet(opball_Palette,PA_BG0,0x40);		//BG2&BG3 COL�]��
	DecordCellSet((CellData*)ObjCell1);
	DecordCellSet((CellData*)ObjCell2);
	ObjPalSets((PalData*)ObjPal);

	PrintScreenInit(DEFAULT_TYPE);
	NMenuScreenInit(FIELD_MENU_TYPE);

	PaletteFadeReq(0xffffffff,0,16,0,0);

	REG_IE_SET( V_BLANK_INTR_FLAG );// VBLANK��������
    *(vu16 *)REG_STAT |= STAT_V_BLANK_IF_ENABLE;

	SetVBlankFunc(VInterFunc);			//�����݊֐��ݒ�(VBLANK)
	MainProcChange(OpPokeSelEvMain);

	*(vu16 *)REG_WININ	= 0x003f;
 	*(vu16 *)REG_WINOUT	= 0x001f;
	*(vu16 *)REG_WIN0H	= 0;
	*(vu16 *)REG_WIN0V	= 0;

   	*(vu16 *)REG_BLDCNT   = 0x00fe;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 7;

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
						  | DISP_WIN0_ON				// WIN0 ON
						  | DISP_OBJ_ON;

	taskID = AddTask(OpPokeSelEvSeq1InitTask, 0);
	TaskTable[taskID].work[0] = 1;		//DefaultPos

	act_no = AddActor(&CursorActHeader,120,56,2);
	ActWork[act_no].work[0] = taskID;

		
	act_no = AddActor(&MonsterBallActHeader,BallPosTable[0][0],BallPosTable[0][1],2);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].work[1] = 0;
	act_no = AddActor(&MonsterBallActHeader,BallPosTable[1][0],BallPosTable[1][1],2);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].work[1] = 1;
	act_no = AddActor(&MonsterBallActHeader,BallPosTable[2][0],BallPosTable[2][1],2);
	ActWork[act_no].work[0] = taskID;
	ActWork[act_no].work[1] = 2;
}


/********************************************************************/
/*                                                                  */
/*				Ҳ�													*/
/*                                                                  */
/********************************************************************/
void	OpPokeSelEvMain(void)
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
/*==================================================================*/
//		�{�[���I��(������)
/*==================================================================*/
static	void	OpPokeSelEvSeq1InitTask(u8 id)
{
	BallInfoWindowSet(0xff,TaskTable[id].work[0]);

	NMenuWinBoxWrite( 2,14,27,19);
	NMenuMsgWrite(opball_sel_msg,3,15);
	TaskTable[id].TaskAdrs = OpPokeSelEvSeq1Task;
}

/*==================================================================*/
//		�{�[���I��(�L�[����)
/*==================================================================*/
static	void	OpPokeSelEvSeq1Task(u8 id)
{
	u8	p_bak;
	u16 act_no;

	p_bak = TaskTable[id].work[0];

	if(sys.Trg & A_BUTTON){
		NMenuBoxClear(	BallInfoPosTable[p_bak][0],BallInfoPosTable[p_bak][1],
						BallInfoPosTable[p_bak][0]+(9-1),BallInfoPosTable[p_bak][1]+(4-1));
		*(vu16 *)REG_WIN0H	= 0;
		*(vu16 *)REG_WIN0V	= 0;

		act_no = AddActor(&BallWindowActHeader,BallPosTable[p_bak][0],BallPosTable[p_bak][1],1);
		TaskTable[id].work[2] = act_no;
		act_no = PokePut(FirstPokemonNumGet(TaskTable[id].work[0]),
				BallPosTable[p_bak][0],BallPosTable[p_bak][1]);
		ActWork[act_no].aff_tbl = PokemonAffTable;
		ActWork[act_no].move	= BallWindowActMove;
		TaskTable[id].work[1]	= act_no;

		TaskTable[id].TaskAdrs = OpPokeSelEvSeq1EffectTask;
		return;
	}
	if((sys.Trg & L_KEY)&&(p_bak > 0)){
		TaskTable[id].work[0]--;
		BallInfoWindowSet(p_bak,TaskTable[id].work[0]);
		return;
	}
	if((sys.Trg & R_KEY)&&(p_bak < 2)){
		TaskTable[id].work[0]++;
		BallInfoWindowSet(p_bak,TaskTable[id].work[0]);
		return;
	}
}

/*==================================================================*/
//		�{�[���E�C���h�E�W�J�G�t�F�N�g
/*==================================================================*/
static	void	OpPokeSelEvSeq1EffectTask(u8 id)
{
	if((ActWork[TaskTable[id].work[2]].affend_sw)
		&&(ActWork[TaskTable[id].work[2]].x == 120)
		&&(ActWork[TaskTable[id].work[2]].y == 64)){
		TaskTable[id].TaskAdrs = OpPokeSelEvSeq2InitTask;
	}
}

/*==================================================================*/
//		�{�[���I���m�F�u�͂��E�������v(������)
/*==================================================================*/
static	void	OpPokeSelEvSeq2InitTask(u8 id)
{
	u16	poke_num;

	poke_num = FirstPokemonNumGet(TaskTable[id].work[0]);
	VoicePlay(poke_num,0);

	NMenuWinBoxWrite( 2,14,27,19);
	NMenuMsgWrite(opball_yesno_msg,3,15);

	YesNoSelectInit(21,7);

	TaskTable[id].TaskAdrs = OpPokeSelEvSeq2Task;
}

/*==================================================================*/
//		�{�[���I���m�F�u�͂��E�������v(�L�[����)
/*==================================================================*/
static	void	OpPokeSelEvSeq2Task(u8 id)
{
	s8	res;
	u8	actnum;

	res = YesNoSelectMain();

	switch(res){

	case	CURSOR_YES_VALUE:	//�͂�
		AnswerWork = TaskTable[id].work[0];	//�Ԃ�l�ݒ�(�|�P�����f�[�^INDEX)
		MainProcChange(sys.pReturnProc);
		break;
	case	CURSOR_NO_VALUE:	//������
	case	CURSOR_CANCEL_VALUE:	//�L�����Z�����ꂽ
		SePlay(SE_SELECT);

		NMenuBoxClear(21, 7,27,12);

		actnum = TaskTable[id].work[1];
		ObjPalRegNumDel( ObjPalNumIdGet(ActWork[actnum].oamData.Pltt) );
		AffineWorkNumDel(ActWork[actnum].oamData.AffineParamNo);	// �̨��Ȱ�ެ �g�pNo�폜
		DelActor( &ActWork[actnum] );
		actnum = TaskTable[id].work[2];
		AffineWorkNumDel(ActWork[actnum].oamData.AffineParamNo);	// �̨��Ȱ�ެ �g�pNo�폜
		DelActor( &ActWork[actnum] );

		TaskTable[id].TaskAdrs = OpPokeSelEvSeq2RetTask;
		break;
	}
}

/*==================================================================*/
//		�{�[���I���L�����Z��(���ԍ��ɂ��|�P�����\���E�C���h�E����)
/*==================================================================*/
static	void	OpPokeSelEvSeq2RetTask(u8 id)
{
	TaskTable[id].TaskAdrs = OpPokeSelEvSeq1InitTask;
}

/********************************************************************/
/*                                                                  */
/*				���E�C���h�E										*/
/*                                                                  */
#if 0
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
#endif
#define BALL_INFOMSG_HEADER_SIZE	(5)
/********************************************************************/
static	void	BallInfoMsgHeaderSet(u8 *str,u8 colb,u8 colf,u8 cols)
{
	*str = CTRL_;
	str++;
	*str = C_ACOL_;
	str++;
	*str = colf;
	str++;
	*str = colb;
	str++;
	*str = cols;
}

static	void	BallInfoWindowSet(u8 clr_p,u8 w_p)
{
	u8	i,j,wx1,wx2,wy1,wy2;
	u8*	type_p;
	u8	typeinfo[BALL_INFOMSG_HEADER_SIZE + ZUKAN_TYPE_SIZE + 4 + EOM_SIZE];
	u8	namestr[BALL_INFOMSG_HEADER_SIZE + MONS_NAME_SIZE + EOM_SIZE];
	u16	poke_num;

	if(clr_p != 0xff){
		NMenuBoxClear(	BallInfoPosTable[clr_p][0],BallInfoPosTable[clr_p][1],
						BallInfoPosTable[clr_p][0]+(9-1),BallInfoPosTable[clr_p][1]+(4-1));
		*(vu16 *)REG_WIN0H	= 0;
		*(vu16 *)REG_WIN0V	= 0;
	}

	poke_num = FirstPokemonNumGet(w_p);

	type_p = ZukanPokeTypeGet(PokeMonsNo2ZukanNoGet(poke_num));
	BallInfoMsgHeaderSet(typeinfo,COL_NUKE,COL_WHITE,COL_DGRAY);
	i = 0;
	j = BALL_INFOMSG_HEADER_SIZE;
	while((type_p[i]!=spc_)&&(i<ZUKAN_TYPE_SIZE)){
		typeinfo[j] = type_p[i];
		i++;
		j++;
	}
	for(i=0;i<5;i++,j++){
		typeinfo[j] = PokemonStr[i];
	}
	typeinfo[j] = EOM_;

	BallInfoMsgHeaderSet(namestr,COL_NUKE,COL_WHITE,COL_DGRAY);
	i = 0;
	j = BALL_INFOMSG_HEADER_SIZE;
	while((PokeNameData[poke_num][i]!=EOM_)&&(i<MONS_NAME_SIZE)){
		namestr[j] = PokeNameData[poke_num][i];
		i++;
		j++;
	}
	namestr[j] = EOM_;

	NMenuMsgWrite(typeinfo,BallInfoPosTable[w_p][0],BallInfoPosTable[w_p][1]);
	NMenuMsgWrite(namestr,BallInfoPosTable[w_p][0]+(9-i),BallInfoPosTable[w_p][1]+2);

	wx1 = (BallInfoPosTable[w_p][0] * 8)-4;
	wx2 = ((BallInfoPosTable[w_p][0]+9) * 8)+4;
	wy1 = (BallInfoPosTable[w_p][1] * 8);
	wy2 = ((BallInfoPosTable[w_p][1]+4) * 8);

	*(vu16 *)REG_WIN0H	= wx1 * 0x100 + wx2;
	*(vu16 *)REG_WIN0V	= wy1 * 0x100 + wy2;
}


/********************************************************************/
/*                                                                  */
/*				OBJ���&����֘A										*/
/*                                                                  */
/********************************************************************/
/*==================================================================*/
/*				�߹��												*/
/*==================================================================*/
static void PokeWait(actWork* aw)
{
}

static u8	PokePut(u16 poke_num,u8 x,u8 y)
{
	u8	actnum;

	DecordPokeCellMake( &PM2FObjDataCell[poke_num],
						pm2fpara[poke_num].size,
						pm2fpara[poke_num].patcnt,
						(void *)DecordAdrs[0],
						(void *)DecordAdrs[1],
						poke_num);
	DecordPalSet( (PalData*)&PM2NObjDataPal[poke_num] );
	PokeActHeaderMake( poke_num, 1 );
	actnum = AddActor( &PokeActHeader,x,y,0);
	ActWork[actnum].move = PokeWait;
	ActWork[actnum].oamData.Priority = 0;

	return	actnum;
}

/*==================================================================*/
/*				���� & ؽ�Ұ��										*/
/*==================================================================*/
/*------------------------------------------------------------------*/
/*                      OAM��{�ް�                                 */
/*------------------------------------------------------------------*/
static const ActOamData CursorOam = {

160,	//V�߼޼��
0,  	//�g��k��Ӱ�ސݒ�(ON)
0,      //OBJӰ�ސݒ�(������)
0,	//ӻ޲��ݒ�(OFF)
0,	//�װӰ�ސݒ�(16�F)
0,	//OBJ�`��ݒ�(�����`)

0,	//H�߼޼��
0,	//�g��k�����Ұ����ް�ݒ�
2,	//OBJ�T�C�Y�ݒ�(32dot)

0x0,	//��׸�����ް
1,	//�\����ײ��è��ݒ�(��2�D��)
0,	//��گ����ް

0,	//�g��k�����Ұ�
};

static const ActOamData MonsterBallOam = {

160,	//V�߼޼��
0,  	//�g��k��Ӱ�ސݒ�(ON)
0,      //OBJӰ�ސݒ�(������)
0,	//ӻ޲��ݒ�(OFF)
0,	//�װӰ�ސݒ�(16�F)
0,	//OBJ�`��ݒ�(�����`)

0,	//H�߼޼��
0,	//�g��k�����Ұ����ް�ݒ�
2,	//OBJ�T�C�Y�ݒ�(32dot)

0x0,	//��׸�����ް
1,	//�\����ײ��è��ݒ�(��2�D��)
0,	//��گ����ް

0,	//�g��k�����Ұ�
};

static const ActOamData BallWindowOam = {

160,	//V�߼޼��
3,  	//�g��k��Ӱ�ސݒ�(ON)
0,      //OBJӰ�ސݒ�(������)
0,	//ӻ޲��ݒ�(OFF)
0,	//�װӰ�ސݒ�(16�F)
0,	//OBJ�`��ݒ�(�����`)

0,	//H�߼޼��
0,	//�g��k�����Ұ����ް�ݒ�
3,	//OBJ�T�C�Y�ݒ�(32dot)

0x0,	//��׸�����ް
1,	//�\����ײ��è��ݒ�(��2�D��)
0,	//��گ����ް

0,	//�g��k�����Ұ�
};


/*------------------------------------------------------------------*/
/*                      MOVE�ް�                                    */
/*------------------------------------------------------------------*/
static const u8 CursorPosTable[3][2] = {{60,32},{120,56},{180,32}};
static void CursorActMove(actWork* aw)
{
	aw->x = CursorPosTable[TaskTable[aw->work[0]].work[0]][0];
	aw->y = CursorPosTable[TaskTable[aw->work[0]].work[0]][1];

	aw->dy = SinMove(aw->work[1],8);
	aw->work[1] = (u8)aw->work[1]+4;
}

static void MonsterBallActMove(actWork* aw)
{
	if(TaskTable[aw->work[0]].work[0] == aw->work[1]){
		ActAnmChkChg(aw,1);		//����
	}else{
		ActAnmChkChg(aw,0);		//�~�܂�
	}
}

static void BallWindowActMove(actWork* aw)
{
	if(aw->x > 120)	aw->x-=4;
	if(aw->x < 120)	aw->x+=4;
	if(aw->y > 64)	aw->y-=2;
	if(aw->y < 64)	aw->y+=2;
}


/*------------------------------------------------------------------*/
/*                      ANM�ް�                                     */
/*------------------------------------------------------------------*/
static const actAnm CursorAnm0[] = {
	{48,30,0,0},
	{ACT_ANMEND,0,0,0},
};

static const actAnm MonsterBallAnm0[] = {
	{ 0,30,0,0},
	{ACT_ANMEND,0,0,0},
};

static const actAnm MonsterBallAnm1[] = {
	{16,4,0,0},
	{ 0,4,0,0},
	{32,4,0,0},
	{ 0,4,0,0},
	{16,4,0,0},
	{ 0,4,0,0},
	{32,4,0,0},
	{ 0,4,0,0},
	{ 0,32,0,0},
	{16,8,0,0},
	{ 0,8,0,0},
	{32,8,0,0},
	{ 0,8,0,0},
	{16,8,0,0},
	{ 0,8,0,0},
	{32,8,0,0},
	{ 0,8,0,0},
	{ACT_ANMLOOP,0,0,0},
};

static const actAnm BallWindowAnm[] = {
	{0,8,0,0},
	{ACT_ANMEND,0,0,0},
};


/*------------------------------------------------------------------*/
/*                      ANMð���                                    */
/*------------------------------------------------------------------*/
static const actAnm* const CursorAnmTbl[]={
	CursorAnm0,
};

static const actAnm* const MonsterBallAnmTbl[]={
	MonsterBallAnm0,
	MonsterBallAnm1,
};

static const actAnm* const BallWindowAnmTbl[]={
	BallWindowAnm,
};

/*------------------------------------------------------------------*/
/*                      AFF�ް�                                     */
/*------------------------------------------------------------------*/
static	const	actAffAnm	PokemonAffPat[] = {
	{0x010,0x010,0,ACT_AFSET},
	{0x10,0x10,0,0x10-1},
	{ACT_AFEND,0,0,0},
};

static	const	actAffAnm	BallWindowAffPat[] = {
	{0x014,0x014,0,ACT_AFSET},
	{0x14,0x14,0,0x10-1},
	{ACT_AFEND,0,0,0},
};

/*------------------------------------------------------------------*/
/*                      AFFð���                                    */
/*------------------------------------------------------------------*/
static	const	actAffAnm*	const	PokemonAffTable[] = {
	PokemonAffPat,
};

static	const	actAffAnm*	const	BallWindowAffTable[] = {
	BallWindowAffPat,
};

/*------------------------------------------------------------------*/
/*                      ACTORͯ��                                   */
/*------------------------------------------------------------------*/
#define	CELLID_OBJ	(0x1000)
#define	PALID_OBJ	(0x1000)
#define	CELLID_OBJ2	(0x1001)
#define	PALID_OBJ2	(0x1001)

static	const	CellData	ObjCell1[]={{(void*)opseloch_LZ,0x800,CELLID_OBJ},{CELL_END,0,0}};
static	const	CellData	ObjCell2[]={{(void*)opselwch_LZ,0x800,CELLID_OBJ2},{CELL_END,0,0}};
static	const	PalData		ObjPal[] = {
	{ op_bobj_Palette	,PALID_OBJ },
	{ op_wobj_Palette	,PALID_OBJ2 },
	{ OBJPAL_END, 0 },
};

static const actHeader CursorActHeader ={
CELLID_OBJ,		//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
PALID_OBJ,		//��گ��Ȱ�ެ�o�^����
&CursorOam,
CursorAnmTbl,
ACT_NTRANS,
DummyActAffTbl,
CursorActMove,
};

static const actHeader MonsterBallActHeader ={
CELLID_OBJ,		//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
PALID_OBJ,		//��گ��Ȱ�ެ�o�^����
&MonsterBallOam,
MonsterBallAnmTbl,
ACT_NTRANS,
DummyActAffTbl,
MonsterBallActMove,
};

static const actHeader BallWindowActHeader ={
CELLID_OBJ2,		//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
PALID_OBJ2,		//��گ��Ȱ�ެ�o�^����
&BallWindowOam,
BallWindowAnmTbl,
ACT_NTRANS,
BallWindowAffTable,
BallWindowActMove,
};


