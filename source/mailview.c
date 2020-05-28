/***********************************************************************
**	���[���{��
***********************************************************************/
#include "common.h"
#include "intr.h"
#include "actor.h"
#include "actanm.h"
#include "palanm.h"
#include "extanm.h"
#include "mail.h"
#include "pm_str.h"
#include "print.h"
#include "kaiwa.h"
#include "boxsub.h"
#include "pokeicon.h"
#include "item.h"
#include "itemtool.h"
#include "fld_sio.h"
#include "laster.h"

#include "poketool.h"
#include "gamedata.h"
#include "menu.h"
#include "task.h"
#include "script.h"
#include "fld_main.h"
#include "fld_task.h"
#include "../graphic/pokegra.h"
//#include "../scaddata/mailview.acl"
//#include "../scaddata/mailview.ach"
//#include "../scaddata/mailview.asc"
#include "mailview.dat"

/*===========================================================*/
/*  �萔                                                     */
/*===========================================================*/
#define MAP_ADDR(b)   (BG_VRAM + (b) * 0x800)
#define CHAR_ADDR(b)  (BG_VRAM + (b) * 0x4000)

//����G���[���p
#define PORTRAIT_MAP_BLOCK	(28)
#define PORTRAIT_CHAR_BLOCK	(1)
#define PORTRAIT_MAP_ADDR  (MAP_ADDR(PORTRAIT_MAP_BLOCK))
#define PORTRAIT_CHAR_ADDR (CHAR_ADDR(PORTRAIT_CHAR_BLOCK))
#define PORTRAIT_PAL        (PA_BG0)

//�w�i
#define BACK_MAP_BLOCK	 (9)
#define BACK_MAP_ADDR	 (MAP_ADDR(BACK_MAP_BLOCK))
#define IMAGE_MAP_BLOCK  (8)
#define IMAGE_CHAR_BLOCK (0)
#define IMAGE_MAP_ADDR  (MAP_ADDR(IMAGE_MAP_BLOCK))
#define IMAGE_CHAR_ADDR (CHAR_ADDR(IMAGE_CHAR_BLOCK))
#define IMAGE_PAL        (PA_BG0)


#define LINELEN_MAX  (24)   // �P�s������̕������ő�
#define LINE_MAX     (8)    // �s���ő�
#define NAMELEN_MAX  (5)    // ���O�ő啶����

#define MSG_OX       (4)    // ���b�Z�[�W�\������w���W
#define MSG_OY       (3)    // ���b�Z�[�W�\������x���W
#define WRITER_OY    (11)   // ���o�l���\������x���W
#define WRITER_EX    (26)   // ���o�l���\���E���w���W

#define MAILNO_MIN	(ITEM_ORENZIMEERU)	//���[���̃A�C�e���i���o�[
#define MAILNO_MAX	(ITEM_RETOROMEERU)
#define NAME_EX_SIZE	3		//�u_���v�̕�
/*===========================================================*/
/*  ���[�N�G���A                                             */
/*===========================================================*/
//�ȈՉ�b�R�[�h��������
typedef u8*	(*pKaiwaCnvW)(u16 *,u16);				//1code to 1word
typedef u8*	(*pKaiwaCnvS)(u16 *,u16 *,u16,u16);		//codes to words

typedef struct {
	u8 msg[LINE_MAX][LINELEN_MAX];
	u8 writer_name[PERSON_NAME_SIZE+NAME_EX_SIZE+EOM_SIZE];
	pFunc  return_func;			//���A����֐�

	pFunc  sub_proc;

	MAIL_DATA		*mail;		//���[���f�[�^�ւ̃|�C���^
	u8  writing_flg;			//0:�C���[�W�̂ݕ\��,1:���͂��\��
	u8  writer_ox;				//���o�l�̕\���I�t�Z�b�gX
	u8  img_type;				//�C���[�W�^�C�v
	u8	portrait_sw;			//����G���[��on = 1,���̑�off = 0
	u8	act_id[3];				//����G���[���p�́A�A�N�^�[ID�ۑ��ꏊ
	
	u8	langCode;				//���ʃR�[�h
	u8	loType;					//����ʃ��C�A�E�g�^�C�v(0:jp,1:alphabet)
	pKaiwaCnvW	p_wcnv;			//��b�R�[�h�Q�P��
	pKaiwaCnvS	p_scnv;			//��b�R�[�h�Q����
	const MailLayout*	pLo;	//���C�A�E�g�f�[�^�ւ̃|�C���^
}MAILVIEW_WORK;
static MAILVIEW_WORK* const wp = (MAILVIEW_WORK*)UserWork;

#ifdef PM_DEBUG
static u8	sg_flg;
#endif	//#ifdef PM_DEBUG

/*===========================================================*/
/*  �v���g�^�C�v                                             */
/*===========================================================*/
static u8 mailview_init_proc(void);
static void mailview_init_loop(void);
static void write_mail_writing(void);
static void init_mail_writing(void);
static void MailViewVblankIntr(void);
static void MailViewMain(void);
static void sub_fadein(void);
static void sub_keywait(void);
static void sub_fadeout(void);

/***********************************************************************
**	������
***********************************************************************/
/*---------------------------------------
	���[���`��Z�b�g�֐�
	������ĂԂƁA���[���\���^�X�N�ɓ˓�

	void MailViewInit(MAIL_DATA *mail,pFunc return_func,u8 writing_flg);

	�����F
	i	MAIL_DATA*	���[���f�[�^�ւ̃|�C���^
	i	pFunc		���[���`���A���A����֐��ւ̃|�C���^
	i	u8			0:�C���[�W�̂ݕ`��,1:���͂��`��
-----------------------------------------*/
void MailViewInit(MAIL_DATA *mail, pFunc return_func, u8 writing_flg)
{
	u16 monsno, unnon_id;
	
#ifdef PM_DEBUG	//�f�o�b�N���́A�Z�b�g�֐���������p�����p�����[�^���g�p
	if(sg_flg){		//�f�o�b�O���j���[����Ă΂ꂽ�Ƃ�
		wp->langCode = (u8)PM_LANG;
		wp->loType = ScriptParameter0;
		wp->p_wcnv = (pKaiwaCnvW)FUNC_KAIWA2WORD;
		wp->p_scnv = (pKaiwaCnvS)FUNC_KAIWA2SENTENCE;
		wp->img_type = ScriptParameter2;
	}
	else{			//�t�B�[���h����Ă΂ꂽ�Ƃ�
		wp->langCode = (u8)PM_LANG;
		wp->loType = (u8)MAIL_LOTYPE;
		wp->p_wcnv = (pKaiwaCnvW)FUNC_KAIWA2WORD;
		wp->p_scnv = (pKaiwaCnvS)FUNC_KAIWA2SENTENCE;

		//�L���ȃf�U�C���i���o�[�̂Ƃ�
		if(mail->design_no >= MAILNO_MIN && mail->design_no <= MAILNO_MAX){
			wp->img_type = mail->design_no - MAILNO_MIN;
		
		}else{
			wp->img_type = 0;
			writing_flg = 0;
		}
	}
#else	
	//���ʃR�[�h���擾
	wp->langCode = (u8)PM_LANG;
	wp->loType = (u8)MAIL_LOTYPE;
	wp->p_wcnv = (pKaiwaCnvW)FUNC_KAIWA2WORD;
	wp->p_scnv = (pKaiwaCnvS)FUNC_KAIWA2SENTENCE;
	if(mail->design_no >= MAILNO_MIN && mail->design_no <= MAILNO_MAX){
		wp->img_type = mail->design_no-MAILNO_MIN; // ��
	}
	else{	//�f�U�C���i���o�[������
		wp->img_type = 0;
		writing_flg = 0;
	}
#endif	//ifdef PM_DEBUG
	
	//�擾�������ʃR�[�h����A���C�A�E�g�^�C�v�����肷��
	switch(wp->loType)
	{
	default:
	case LOTYPE_JP:
		wp->pLo = &(mLO_jp[wp->img_type]);
		break;
	case LOTYPE_AL:
		wp->pLo = &(mLO_al[wp->img_type]);
		break;
	}

	//����G���[���H
	monsno = GetMailHoldMonsNo(mail->hold_monsno, &unnon_id);
	if(monsno > 0 && monsno <= KAIHATSU_NUM_MAX){
		switch(wp->img_type)
		{
		case 6:
			wp->portrait_sw = 1;
			break;
		case 9:
			wp->portrait_sw = 2;
			break;
		default:
			wp->portrait_sw = 0;
		}
	}
	else{
		wp->portrait_sw = 0;
	}

	//�p�����[�^�̕ۑ�
	wp->mail = mail;
	wp->return_func = return_func;
	wp->writing_flg = writing_flg;
		
	//���C���^�X�N�`�F���W
	MainProcChange(mailview_init_loop);
}

static u8 mailview_init_proc(void)
{
	u16 icon_chrid;
	
	switch(sys.InitSeqWork){
	case 0:
		SetVBlankFunc(NULL);
		LasterInit();						//HDMA������
		*(vu16 *)REG_DISPCNT = 0; 
		break;
	case 1:
		CpuClear(0,OAM,OAM_SIZE,16);		
		break;
	case 2:
		PaletteAnimeInit();
		break;
	case 3:
		InitTask();
		break;
	case 4:
		InitActor();
		break;
	case 5:
		ObjPalManInit();
		*(vu16 *)REG_BG0HOFS = 0;
		*(vu16 *)REG_BG0VOFS = 0;
		*(vu16 *)REG_BG1HOFS = 0;
		*(vu16 *)REG_BG1VOFS = 0;
		*(vu16 *)REG_BG2VOFS = 0;
		*(vu16 *)REG_BG2HOFS = 0;
		*(vu16 *)REG_BG3HOFS = 0;
		*(vu16 *)REG_BG3VOFS = 0;
		*(vu16 *)REG_BLDCNT  = 0;
		*(vu16 *)REG_BLDALPHA  = 0;
		break;
	case 6:
		PrintScreenInit(BAG_TYPE);
		break;
	case 7:
		NMenuScreenInitStart(BAG_TYPE);
		break;
	case 8:
		if(NMenuScreenInitMain() == FALSE) return FALSE;
		break;
	case 9:
		NMenuScreenClear();
		break;

	case 10:		//�w�i�摜�����
		CpuClear(0x0001,(void *)BACK_MAP_ADDR,0x800,16);
		break;
	case 11:
		//�p���b�g�Z�b�g
		PaletteWorkSet(MailImageTbl[wp->img_type].pal_addr, IMAGE_PAL, 0x20);
		break;
	case 12:
		//�}�b�v�f�[�^�Z�b�g
		LZ77UnCompVram(MailImageTbl[wp->img_type].map_addr, (void*)IMAGE_MAP_ADDR);
		break;
	case 13:
		//�L�����N�^�f�[�^�Z�b�g
		LZ77UnCompVram(MailImageTbl[wp->img_type].char_addr, (void*)IMAGE_CHAR_ADDR);
		//�t�H���g�J���[�̃Z�b�g
		PaletteWork[PA_BG15 + 1] = MailImageTbl[wp->img_type].font_color;
		PaletteWork[PA_BG15 + 8] = MailImageTbl[wp->img_type].shadow_color;

		//���ʔw�i�J���[�̃Z�b�g
		PaletteWork[PA_BG0+10] = palData[MyData.my_sex][0];
		PaletteWork[PA_BG0+11] = palData[MyData.my_sex][1];
		break;
	case 14:
		if(wp->writing_flg){
			init_mail_writing();
		}
		break;
	case 15:
		if(wp->writing_flg){
			write_mail_writing();
		}
		SetVBlankFunc(MailViewVblankIntr);
		FadeData.trans_stop = 1;
		break;
	case 16:		//����G���[���̏ꍇ�A�A�N�^�[�Z�b�g
		icon_chrid = MailHoldMonsNo_IconCharIDConv(wp->mail->hold_monsno);
		switch(wp->portrait_sw){
		case 1:
			PokeIconPaletteSetCharID(icon_chrid);
			wp->act_id[0] = PokeIconSetCharID(icon_chrid,DummyActMove,0x0c*8,0x10*8,0);
			break;
		case 2:
			PokeIconPaletteSetCharID(icon_chrid);
			wp->act_id[0] = PokeIconSetCharID(icon_chrid,DummyActMove,0x05*8,0x10*8,0);
			break;
		default:
			break;
		}
		break;
	case 17:
		if( FieldRecvFifoClearWait() == TRUE )	return FALSE;
		break;
	case 18:
	// �a�f�O�i�����j
		*(vu16 *)REG_BG0CNT = BG_SCREEN_SIZE_2 | BG_PRIORITY_0
		| 31 << BG_SCREEN_BASE_SHIFT | 2 << BG_CHAR_BASE_SHIFT
		| BG_COLOR_16 | BG_MOS_OFF;
		
	// �a�f�P�i�w�i�j
		*(vu16 *)REG_BG1CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_1
		| IMAGE_MAP_BLOCK << BG_SCREEN_BASE_SHIFT
		| IMAGE_CHAR_BLOCK << BG_CHAR_BASE_SHIFT
		| BG_COLOR_16 ;

	//BG2
		*(vu16 *)REG_BG2CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_2
		| BACK_MAP_BLOCK << BG_SCREEN_BASE_SHIFT
		| IMAGE_CHAR_BLOCK << BG_CHAR_BASE_SHIFT
		| BG_COLOR_16 ;
		
		*(vu16 *)REG_BLDCNT = 0;
		*(vu16 *)REG_DISPCNT = DISP_MODE_0 | DISP_OBJ_CHAR_1D_MAP 
		| DISP_BG0_ON
		| DISP_BG1_ON
		| DISP_BG2_ON
		| DISP_OBJ_ON
		;
		PaletteFadeReq(0xffffffff, 0, 16, 0, 0x0000);
		FadeData.trans_stop = 0;
		wp->sub_proc = sub_fadein;
		return TRUE;

	default:
		return FALSE;
	}

	sys.InitSeqWork++;
	return FALSE;
}
static void mailview_init_loop(void)
{
	while(1){
		if(mailview_init_proc() == TRUE){
			MainProcChange(MailViewMain);
			break;
		}
		//�ʐM���Ȃ甲����
		if(ItemToolSioCheck() == TRUE) break;
	}
}
/*====================================================*/
/*  ���[���̓��e�ƍ��o�l�����A���`���ăo�b�t�@�R�s�[  */
/*	�����F�C���[�W�i���o�[							  */
/*====================================================*/
static void init_mail_writing(void)
{
	u8 name_fooder[] = {
		spc_, yo_, ri_, EOM_,
	};
	u8 *cp;
	u8	no = 0;
	u16 i;

	//���C�A�E�g�̏����ɂ��������āA�R�[�h��W�J
	for(i = 0; i < wp->pLo->nRow; i++){
		KaiwaCopySentence(	wp->msg[i], &(wp->mail->msg[no]),wp->pLo->p_mrLO[i].nCode, 1);
		no += wp->pLo->p_mrLO[i].nCode;
	}

	//���o�l��
	cp = PM_strcpy(wp->writer_name, wp->mail->writer_name);
	
	if(wp->loType == LOTYPE_JP){
		PM_strcpy(cp, name_fooder);
		wp->writer_ox = wp->pLo->ofs_nx - PM_strlen(wp->writer_name);
	}
	else{
		wp->writer_ox = wp->pLo->ofs_nx;
	}
}
/*====================================================*/
/*  ���[���̓��e�ƍ��o�l�����A�`��                    */
/*====================================================*/
static void write_mail_writing(void)
{
	u16 i;
	u8	dx = 0,dy = 0;
	
	//�{��
	for(i = 0; i < wp->pLo->nRow; i++){
		if(wp->msg[i][0] == EOM_ || wp->msg[i][0] == 0) continue;

		dx =	  wp->pLo->p_mrLO[i].ofsX;
		dy += (u8)wp->pLo->p_mrLO[i].ofsY;
		NMenuMsgWrite(wp->msg[i], wp->pLo->ofs_x+dx,wp->pLo->ofs_y + dy);
		dy += 2;
	}
	//���o�l
	NMenuMsgWrite(wp->writer_name, wp->writer_ox, wp->pLo->ofs_ny);
}

/***********************************************************************
**	V Blank
***********************************************************************/
static void MailViewVblankIntr(void)
{
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}
/***********************************************************************
**	���C��
***********************************************************************/
static void MailViewMain(void)
{
	if(wp->portrait_sw){
		JumpActor();
		SetActor();
	}
	(*(wp->sub_proc))();
}
/*====================================*/
/*  �t�F�[�h�C��                      */
/*====================================*/
static void sub_fadein(void)
{
	if(!PaletteAnime()){
		wp->sub_proc = sub_keywait;
	}
}
/*====================================*/
/*  �L�[�E�F�C�g                      */
/*====================================*/
static void sub_keywait(void)
{
	if(sys.Trg & (A_BUTTON|B_BUTTON)){
		PaletteFadeReq(0xffffffff, 0, 0, 16, 0x0000);
		wp->sub_proc = sub_fadeout;
	}
}
/*====================================*/
/*  �t�F�[�h�C��                      */
/*====================================*/
static void sub_fadeout(void)
{
	u16 icon_chrid;
	
	if(!PaletteAnime()){
		MainProcChange(wp->return_func);

		//����G���[���������Ƃ��́A�g�p�����A�N�^�[�폜
		switch(wp->portrait_sw){
		case 1:
		case 2:
			icon_chrid = MailHoldMonsNo_IconCharIDConv(wp->mail->hold_monsno);
			PokeIconPaletteDelCharID(icon_chrid);
			PokeIconDel(&(ActWork[wp->act_id[0]]));
		}

#ifdef PM_DEBUG
		PaletteAnimeInit();				//�p���b�g�N���A
#else		
		//�g�p����RAM�G���A�̃N���A
		memset(wp,0,sizeof(MAILVIEW_WORK));
		PaletteAnimeInit();				//�p���b�g�N���A
#endif	//PM_DEBUG	
	}
}

///////////////////////////////////////////////////////////////////
//	�f�o�b�O����̌Ăяo��
//////////////////////////////////////////////////////////////////
#ifdef PM_DEBUG
static void d_mailview_return(void);
static void d_mailview_init(void);
static void d_mailview_menuset(void);
static void d_mailview_menutask(u8 id);
static void d_view_sLang(u8 id);
/*----------------------------------------
	�C���[�W��I��
------------------------------------------*/
static void d_view_fadeout(u8 id)
{
	if(FadeData.fade_sw != 0) return;

	//���C���^�X�N�`�F���W�B���[���`��Ăяo��
	sg_flg = 1;
	NMenuScreenClear();
	DelTask(id);
		
	if(Fld.MailData[ScriptParameter1].design_no == 0){
		MailViewInit(&(Fld.MailData[ScriptParameter1]),d_mailview_init,0);
	}
	else{
		MailViewInit(&(Fld.MailData[ScriptParameter1]),d_mailview_init,1);
	}
	return;	
}
static void d_view_sImg(u8 id)
{
	if(sys.Trg & (D_KEY | L_KEY)){
		ScriptParameter2 = (ScriptParameter2+11)%12;
		NMenuWinBoxWrite( 1, 1, 10, 4 );	//menutool.c
		NMenuMsgWrite(msg_imgType[ScriptParameter2],2,2);
		return;
	}
	if(sys.Trg & (U_KEY | R_KEY)){
		ScriptParameter2 = (ScriptParameter2+1)%12;
		NMenuWinBoxWrite( 1, 1, 10, 4 );	//menutool.c
		NMenuMsgWrite(msg_imgType[ScriptParameter2],2,2);
		return;
	}

	if(sys.Trg & A_BUTTON){
		PaletteFadeReq(0xFFFFFFFF,0,0,16,0);
		TaskTable[id].TaskAdrs = d_view_fadeout;
		return;
	}

	if(sys.Trg & B_BUTTON){
		MsgWrite(msg_selectLang);
		NMenuMsgWrite(msg_lang[ScriptParameter0],2,2);
		TaskTable[id].TaskAdrs = d_view_sLang;
		return;
	}
	return;
}
/*----------------------------------------
	���C�A�E�g��I��
------------------------------------------*/
static void d_view_sLang(u8 id)
{
	if(sys.Trg & (U_KEY | D_KEY | R_KEY | L_KEY)){
		ScriptParameter0 ^= 1;
		NMenuMsgWrite(msg_lang[ScriptParameter0],2,2);
		return;
	}
	//���j���[����̉������ςȂ��i�s���(A_BUTTON)
	if(sys.Trg & A_BUTTON){
		if(sys.TrgFlag){
			sys.TrgFlag = 0;
			MsgWrite(msg_selectLang);
			NMenuWinBoxWrite( 1, 1, 10, 4 );	//menutool.c
			NMenuMsgWrite(msg_lang[ScriptParameter0],2,2);
			return;
		}
		MsgWrite(msg_imgSelect);
		NMenuWinBoxWrite( 1, 1, 10, 4 );	//menutool.c
		NMenuMsgWrite(msg_imgType[ScriptParameter2],2,2);
		TaskTable[id].TaskAdrs = d_view_sImg;
		return;
	}
	return;
}
/*----------------------------------------
	���[������������|�P������I��
------------------------------------------*/
static void d_sub_sPoke(u8 id)
{
	u8	msg1[] = {I_MSG_,STR_TEMP_BUF0,spc_,I_MSG_,STR_TEMP_BUF1,EOM_};
	u8	msg2[] = {ME_,bou_,RU_,I_MSG_,STR_TEMP_BUF0,spc_,I_MSG_,STR_TEMP_BUF1,EOM_};
	u8	chg_flg = 0;
	
	//����
	if(sys.Trg & A_BUTTON){
		PM_NumMsgSet( StrTempBuffer0,ScriptParameter1, NUM_MODE_ZERO, 1 );	//pm_str.c
		PM_strcpy( StrTempBuffer1,PokeNameData[TaskTable[id].work[0]]); 
		
		Fld.MailData[ScriptParameter1].hold_monsno = TaskTable[id].work[0];
		PM_MsgExpand( MsgExpandBuffer,msg2);
		MsgWrite(MsgExpandBuffer);

		DelTask(id);
		//���j���[�^�X�N�Z�b�g
		AddTask(d_mailview_menutask,0);
		d_mailview_menuset();
		return;
	}
	//�L�����Z��
	if(sys.Trg & B_BUTTON)
	{
		DelTask(id);
		NMenuScreenClear();
		//���j���[�^�X�N�Z�b�g
		AddTask(d_mailview_menutask,0);
		d_mailview_menuset();
		return;	
	}

	//�I��
	if(sys.Trg & (D_KEY | L_KEY)){
		TaskTable[id].work[0] = (TaskTable[id].work[0]+KAIHATSU_NUM_MAX)%(KAIHATSU_NUM_MAX+1);
		chg_flg = 1;
	}
	else if(sys.Trg & (U_KEY | R_KEY)){
		TaskTable[id].work[0] = (TaskTable[id].work[0]+1)%(KAIHATSU_NUM_MAX+1);
		chg_flg = 1;
	}

	//���������Ȃ��Ȃ�A��
	if(chg_flg == 0) return;

	//��ʏ�������
	PM_NumMsgSet( StrTempBuffer0,TaskTable[id].work[0], NUM_MODE_ZERO, 3 );	//pm_str.c
	PM_strcpy( StrTempBuffer1,PokeNameData[TaskTable[id].work[0]]); 
	PM_MsgExpand( MsgExpandBuffer,msg1);

	NMenuWinBoxWrite(0,0,12,3);
	NMenuMsgWrite(MsgExpandBuffer,1,1);
	return;
}
static void d_sub_sPokeInit(u8 id)
{
	u8	msg[] = {ME_,bou_,RU_,I_MSG_,STR_TEMP_BUF0,wo_,spc_,
				 da_,re_,ni_,spc_,mo_,ta_,se_,ru_,hate_,EOM_};
	u8	msg1[] = {n0_,n0_,n0_,spc_,I_MSG_,STR_TEMP_BUF0,EOM_};
	
	PM_NumMsgSet( StrTempBuffer0,ScriptParameter1, NUM_MODE_SPACE, 1 );	//pm_str.c
	PM_MsgExpand( MsgExpandBuffer,msg);
	MsgWrite(MsgExpandBuffer);

	PM_strcpy(StrTempBuffer0,PokeNameData[0]);
	PM_MsgExpand(MsgExpandBuffer,msg1);
	NMenuWinBoxWrite(0,0,12,3);
	NMenuMsgWrite(msg1,1,1);

	TaskTable[id].TaskAdrs = d_sub_sPoke;
}
/*----------------------------------------
	�f�o�b�N����̌Ăяo��
------------------------------------------*/
u8 MailViewSet(void)
{
	MenuEnd();
	StopFieldScript();	
	SetForceEvent();

	sg_flg = 0;
	ScriptParameter0 = ScriptParameter1 = ScriptParameter2 = 0;
	MainProcChange(d_mailview_init);
	return 1;
}

/*-------------------------------------------
	MailViewSet()�Ő؂�ւ���
---------------------------------------------*/
//�f�o�b�O�p�̃��C��
static void d_mailview_main(void)
{
	PaletteAnime();
	MainTask();
}
//�f�o�b�O����t�B�[���h�ɕ��A����
static void d_mailview_return(void)
{
	//�g���K�[�}�X�N�ݒ����
	sys.TrgFlag = 0;
	sys.TrgFlagMask = 0;

	pFieldRecoverFunc = AddFieldInTask;			//fld_main.h / fld_task.h
	MainProcChange( FieldMainRecover );			//���C���^�X�N�`�F���W
}
//�f�o�b�O���̃��j���[�p�^�X�N
extern u8	(*MenuFunc)();
static void d_mailview_menutask(u8 id)
{
	if(MenuFunc() == 1){
		MenuEnd();
		DelTask(id);
	}
}
#include "ev_flag.h"
//���[�����݂郁�j���[
static u8	d_viewmail_select(void)
{
	s8	num;

	num = NMenuVCursorControl();
	switch(num)
	{
	case CURSOR_CANCEL_VALUE:
		NMenuScreenClear();
		d_mailview_menuset();
		return 0;
	case CURSOR_DEFAULT_VALUE:
		return 0;
	default:
		ScriptParameter1 = num;
		ScriptParameter2 = ScriptParameter2%12;

		AddTask(d_view_sLang,0);
		
		//�g���K�[�}�X�N�ݒ�
		sys.TrgFlag = 1;
		sys.TrgFlagMask = 0;
		return 1;
	}
}
//�ȈՉ�b�E���[�����[�h�Ăяo��
extern void KaiwaInput(void);
static u8 ChangeKaiwaFade(void)
{
	if(FadeData.fade_sw == 0){
		ScriptParameter0 = KAIWA_MODE_MAIL;
		KaiwaInput();
		return 1;
	}
	return 0;
}
//���[���Ƃ��낭���j���[
static u8	d_addmail_select(void)
{
	u8	msg[] = {se_,i_,ki_,no_,spc_,DE_,bou_,TA_,ga_,spc_,to_,u_,ro_,ku_,zu_,mi_,spc_,de_,su_,CR_,
				 si_,n_,ki_,spc_,to_,u_,ro_,ku_,spc_,de_,ki_,ma_,se_,n_,EOM_};
	s8 num;
	
	num = NMenuVCursorControl();
	switch(num){
	case CURSOR_CANCEL_VALUE:
		NMenuScreenClear();
		d_mailview_menuset();
		return 0;
	case CURSOR_DEFAULT_VALUE:
		return 0;
	default:
		break;
	}

	//�I�����ꂽ��A�o�^���ɍs��
	ScriptParameter1 = num;

	if(	Fld.MailData[num].design_no >= MAILNO_MIN && 
		Fld.MailData[num].design_no <= MAILNO_MAX){
		NMenuScreenClear();
		MsgWrite(msg);
		d_mailview_menuset();
		return 0;
	}
	Fld.MailData[ScriptParameter1].design_no = 0xFFFF;
	PM_strcpy(Fld.MailData[ScriptParameter1].writer_name, MyData.my_name);
	*((u32 *)Fld.MailData[ScriptParameter1].writer_id) = *((u32 *)(MyData.ID));
	
	MenuFunc = ChangeKaiwaFade;
	return 0;
}
//���[�����|�P�����Ɏ�������
static u8 d_pokemail_select(void)
{
	u8	msg[] = {se_,i_,ki_,ni_,spc_,to_,u_,ro_,ku_,sa_,re_,ta_,
				ME_,bou_,RU_,DE_,bou_,TA_,ha_,CR_,he_,n_,ko_,u_,spc_,
				de_,ki_,ma_,se_,n_,EOM_};
	s8	num = NMenuVCursorControl();

	switch(num){
	case CURSOR_CANCEL_VALUE:
		NMenuScreenClear();
		d_mailview_menuset();
		return 0;
	case CURSOR_DEFAULT_VALUE:
		return 0;
	}

	ScriptParameter1 = num;

	//�f�o�b�O����o�^�����f�[�^�ȊO�́A������Ȃ�
	if(	Fld.MailData[num].design_no >= MAILNO_MIN && 
		Fld.MailData[num].design_no <= MAILNO_MAX){
		NMenuScreenClear();
		MsgWrite(msg);
		d_mailview_menuset();
		return 0;
	}
	AddTask(d_sub_sPokeInit,0);
	return 1;
}
//���[���폜���j���[
static u8 d_delmail_select(void)
{
	u8	msg[] = {ME_,bou_,RU_,I_MSG_,STR_TEMP_BUF0,spc_,to_,u_,ro_,ku_,spc_,ka_,i_,zi_,yyo_,EOM_};
	u8	cant[] = {DE_,BA_,TTU_,GU_,ME_,NI_,YYU_,bou_,ka_,ra_,spc_,
				to_,u_,ro_,ku_,si_,ta_,CR_,
				ME_,bou_,RU_,spc_,i_,ga_,i_,ha_,spc_,ke_,se_,ma_,se_,n_,EOM_};
	s8	num = NMenuVCursorControl();

	switch(num)
	{
	case CURSOR_CANCEL_VALUE:
		NMenuScreenClear();
		d_mailview_menuset();
		return 0;
	case CURSOR_DEFAULT_VALUE:
		return 0;
	}

	ScriptParameter1 = num;
	
	//�f�o�b�O�œo�^�������[���ȊO�͏����Ȃ�
	if(	Fld.MailData[ScriptParameter1].design_no != 0xFFFF && 
		Fld.MailData[ScriptParameter1].design_no != 0){
		NMenuScreenClear();
		MsgWrite(cant);	
		d_mailview_menuset();
		return 0;
	}

	//�o�^����
	Fld.MailData[ScriptParameter1].design_no = 0;
	Fld.MailData[ScriptParameter1].hold_monsno = 0;
	Fld.MailData[ScriptParameter1].writer_name[0] = EOM_;
	*((u32 *)Fld.MailData[ScriptParameter1].writer_id) = 0x00000000;

	//���b�Z�[�W�R�[�h�N���A
	for(num = 0;num < 9;num++){
		Fld.MailData[ScriptParameter1].msg[num] = 0xFFFF;
	}
	PM_NumMsgSet( StrTempBuffer0,ScriptParameter1, NUM_MODE_SPACE, 1 );	//pm_str.c
	PM_MsgExpand(MsgExpandBuffer,msg);

	NMenuScreenClear();
	MsgWrite(MsgExpandBuffer);

	d_mailview_menuset();
	return 0;
}
//���[���Z���N�g���j���[������
static void d_mailselect_init(u8 no)
{
	u8	m_mailno[] = {	ME_,bou_,RU_,I_MSG_,STR_TEMP_BUF0,spc_,
						I_MSG_,STR_TEMP_BUF1,spc_,
						I_MSG_,STR_TEMP_BUF2,EOM_};
	u8	i = 0;
	u8	m_expand[9][20];
	MENU_ITEM	MenuTbl[9];
	
	for(i = 0;i < 9;i++)
	{
		PM_NumMsgSet( StrTempBuffer0,i, NUM_MODE_SPACE, 1 );	//pm_str.c
		if(Fld.MailData[i].design_no == 0xFFFF){	//�f�o�b�O���j���[�œo�^
			PM_NumMsgSet( StrTempBuffer1,1, NUM_MODE_SPACE, 1 );	//pm_str.c
		}
		else if(Fld.MailData[i].design_no >= MAILNO_MIN && Fld.MailData[i].design_no <= MAILNO_MAX){
			PM_NumMsgSet( StrTempBuffer1,2, NUM_MODE_SPACE, 1 );
		}
		else{
			PM_NumMsgSet( StrTempBuffer1,0, NUM_MODE_SPACE, 1 );	//pm_str.c
			Fld.MailData[i].design_no = 0;
		}
		PM_strcpy(StrTempBuffer2,PokeNameData[Fld.MailData[i].hold_monsno]);
	
		PM_MsgExpand( m_expand[i],m_mailno);	//I_MSG_���o�b�t�@�ɓW�J		
		MenuTbl[i].msg = (const u8 *)m_expand[i];
		MenuTbl[i].ptr = NULL;
	}
	NMenuWinBoxWrite(0, 0, 16, 18+1);
	NMenuFixListWrite(2, 1, 9, (const MENU_ITEM *)MenuTbl);
	NMenuVCursorInit(1, 1, 9, 0);

	//�t���O�Ŕ�ѐ��ς���
	switch(no)
	{
	case 0:
		MenuFunc = d_viewmail_select;
		break;
	case 1:
		MenuFunc = d_addmail_select;
		break;
	case 2:
		MenuFunc = d_pokemail_select;
		break;
	case 3:
		MenuFunc = d_delmail_select;
		break;
	default:
		break;
	}
}
//�f�o�b�O���̃��j���[�Z���N�g�A�L�[�擾
static u8 d_mailview_select(void)
{
	s8 num;
	
	num = NMenuVCursorControl();
	switch(num){
	case CURSOR_CANCEL_VALUE:
		MainProcChange(d_mailview_return);
		return 1;
	case CURSOR_DEFAULT_VALUE:
		return 0;
	default:
		break;
	}

	switch(num){
	case 0:	//���[��������
		d_mailselect_init(num);
		return 0;
	case 1:	//���[���Ƃ��낭
		d_mailselect_init(num);
		return 0;
	case 2:
		d_mailselect_init(num);
		return 0;
	case 3:	//���[��������
		d_mailselect_init(num);
		return 0;
	}
	return 1;
}
//�f�o�b�O���̃��j���[������
static void d_mailview_menuset(void)
{
	static const MENU_ITEM MenuTbl[] = {
		{ dmvm_view,		NULL },
		{ dmvm_mailset,	NULL },
		{ dmvm_pmset,	NULL },
		{ dmvm_delmail,  NULL },
	};

#define MENU_CNT  (sizeof(MenuTbl)/sizeof(MenuTbl[0]))

	NMenuWinBoxWrite(0, 0, 9, MENU_CNT*2+1);
	NMenuFixListWrite(2, 1, MENU_CNT, MenuTbl);
	NMenuVCursorInit(1, 1, MENU_CNT, 0);
	MenuFunc = d_mailview_select;
	
#undef MENU_CNT
	return;
}
//�f�o�b�O���̏�����,MailViewSet()�Ő؂�ւ�
static void d_mailview_init(void)
{
	switch(sys.InitSeqWork){
	case 0:
		if(sg_flg){
			sys.InitSeqWork = 2;
			sg_flg = 0;
			return;
		}
		LasterInit();	//HDMA������
		PaletteAnimeInit();
		SetVBlankFunc(MailViewVblankIntr);
		PaletteFadeReq(0xffffffff,0,0,16,0x0000);
		break;
	case 1:
		if(PaletteAnime()) return;
		break;
	case 2:
		SetVBlankFunc(NULL);
		*(vu16 *)REG_BG0HOFS = 0;
		*(vu16 *)REG_BG0VOFS = 0;
		*(vu16 *)REG_BG1HOFS = 0;
		*(vu16 *)REG_BG1VOFS = 0;
		*(vu16 *)REG_BG2VOFS = 0;
		*(vu16 *)REG_BG2HOFS = 0;
		*(vu16 *)REG_BG3HOFS = 0;
		*(vu16 *)REG_BG3VOFS = 0;
		break;
	case 3:
		InitActor();
		InitTask();
		ObjPalManInit();
		break;
	case 4:
		PrintScreenInit(FIELD_MENU_TYPE);
		NMenuScreenInit(FIELD_MENU_TYPE);
		NMenuScreenClear();
		break;
	case 5:		//�w�i�摜�����
		//�p���b�g�Z�b�g
		PaletteWorkSet(MailImageTbl[0].pal_addr, IMAGE_PAL, 0x20);
		//�L�����N�^�f�[�^�Z�b�g
		LZ77UnCompVram(MailImageTbl[0].char_addr, (void*)IMAGE_CHAR_ADDR);

		//�}�b�v�f�[�^�쐬
		DmaClear(3,0x0001,(void*)IMAGE_MAP_ADDR,0x500,16);
		break;
	case 6:
	// �a�f�O�i�����j
		*(vu16 *)REG_BG0CNT = BG_SCREEN_SIZE_2 | BG_PRIORITY_0
		| 31 << BG_SCREEN_BASE_SHIFT | 2 << BG_CHAR_BASE_SHIFT
		| BG_COLOR_16 | BG_MOS_OFF;
		
	// �a�f�P�i�w�i�j
		*(vu16 *)REG_BG1CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_1
		| IMAGE_MAP_BLOCK << BG_SCREEN_BASE_SHIFT
		| IMAGE_CHAR_BLOCK << BG_CHAR_BASE_SHIFT
		| BG_COLOR_16 ;

		*(vu16 *)REG_BLDCNT = 0;
		*(vu16 *)REG_DISPCNT = DISP_MODE_0 | DISP_OBJ_CHAR_1D_MAP 
		| DISP_BG0_ON
		| DISP_BG1_ON;

		d_mailview_menuset();
		PaletteFadeReq(0xffffffff, 0, 16, 0, 0x0000);
		SetVBlankFunc(MailViewVblankIntr);
		break;
	case 7:
		if(!PaletteAnime()) return;
		break;
	case 8:
		AddTask(d_mailview_menutask,0);
		MainProcChange(d_mailview_main);
		break;
	default:
		return;
	}
	sys.InitSeqWork++;
	return;
}
#endif	//PM_DEBUG

