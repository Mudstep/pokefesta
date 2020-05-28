/********************************************************************/
/*                      con_pic.c                                   */
/*                    �R���e�X�g�̊G�\��                            */
/*                       by tomo                                    */
/********************************************************************/

/********************************************************************/
/*                      INCLUDE FILE                                */
/********************************************************************/
#include "common.h"
#include "intr.h"
#include "calctool.h"
#include "menu.h"
#include "palanm.h"
#include "actanm.h"
#include "objdata.h"
#include "decord.h"
#include "poketool.h"
#include "message.h"
#include "bss.h"
#include "client_t.h"
#include "pic_eff.h"
#include "con_pic.h"
#include "con_tool.h"
#include "laster.h"

#include "../mapdata/frame.acl"

#include "../mapdata/frame0_sch_RL.c"
#include "../mapdata/frame1_sch_RL.c"
#include "../mapdata/frame2_sch_RL.c"
#include "../mapdata/frame3_sch_RL.c"
#include "../mapdata/frame4_sch_RL.c"
#include "../mapdata/frame_c_sch_RL.c"

#include "../mapdata/frame0_asc_RL.c"
#include "../mapdata/frame1_asc_RL.c"
#include "../mapdata/frame2_asc_RL.c"
#include "../mapdata/frame3_asc_RL.c"
#include "../mapdata/frame4_asc_RL.c"
#include "../mapdata/frame_c_asc_RL.c"

/*==================================================================*/
/*�@�@�@�@�@�@�@�@�@�@�@�R���e�X�g�p                                */
/*==================================================================*/
const u8 * const con_name_tbl[CON_MES_NO_MAX]={
	con_mes_kakkoyosa,
	con_mes_utsukushisa,
	con_mes_kawaisa,
	con_mes_kashikosa,
	con_mes_takumashisa
};

/*==================================================================*/
/*�@�@�@�@�@�@�@�@�@�@�@���p�ٗp  �@�@                              */
/*==================================================================*/
//���p�ٗp���b�Z�[�W
//[x][]=�P�T��ނ̃��b�Z�[�W�Ȃ̂�15(MUS_MES_NO_MAX)
//[][x]="�|�P������"�̑O��ŕ��͂��Q�ɕ����Ǘ����Ă���̂�2

const u8 * const mus_mes_str[MUS_MES_NO_MAX][2]={
	{mus_mes_str_01_01,mus_mes_str_01_02},
	{mus_mes_str_02_01,mus_mes_str_02_02},
	{mus_mes_str_03_01,mus_mes_str_03_02},
	{mus_mes_str_04_01,mus_mes_str_04_02},
	{mus_mes_str_05_01,mus_mes_str_05_02},
	{mus_mes_str_06_01,mus_mes_str_06_02},
	{mus_mes_str_07_01,mus_mes_str_07_02},
	{mus_mes_str_08_01,mus_mes_str_08_02},
	{mus_mes_str_09_01,mus_mes_str_09_02},
	{mus_mes_str_10_01,mus_mes_str_11_02},
	{mus_mes_str_11_01,mus_mes_str_11_02},
	{mus_mes_str_12_01,mus_mes_str_12_02},
	{mus_mes_str_13_01,mus_mes_str_13_02},
	{mus_mes_str_14_01,mus_mes_str_14_02},
	{mus_mes_str_15_01,mus_mes_str_15_02},
};

/********************************************************************/
/*                      DEFINE                                      */
/********************************************************************/
enum{
	CPM_FADE_IN,
	CPM_KEY_CHECK,
	CPM_FADE_OUT
};

/********************************************************************/
/*                      EXTREN                                      */
/********************************************************************/
extern void InitKeyRead(void);
extern void KeyRead(void);

extern void poke_bit_color_down(BE_ST *est);
extern void poke_bit_effect(BE_ST *est);
extern void poke_bit_to_obj(BE_ST *est);

/********************************************************************/
/*                      GLOBAL                                      */
/********************************************************************/
void contest_picture_init(void);
void contest_picture_main(void);

/********************************************************************/
/*                      STATIC                                      */
/********************************************************************/
static void cp_main_sub(void);
static void register_init(void);
static void contest_picture_v_intr(void);
static void pic_set(u8 port,u8 type);
static void poke_bit_decord(u16 p_no,u8 ou);
static void bit_get_obj16(u8 *bin,u16 *pa,u16 *bout);

static void bg_set(u8 port_no,u8 cp_type);
static void obj_set(u8 port_no);
static u8 port_num_to_eff_num(u8 port);
static void work_area_set(void);
static void eff_obj(u8 eff_no);
static void mes_win_clear(u8 cp_type);
static void mes_print(u8 po,u8 type);
static void cp_init_proc(void);
static void mosaic_v_intr(void);
static void mosaic_v_intr_init(u8 no);
/********************************************************************/
/*                      WORK                                        */
/********************************************************************/
u16 *bg_p;		//�f�[�^�̊i�[���Ă���A�h���X
u16 *pal_p;		//�p���b�g�i�[�ꏊ�A�h���X

BE_ST eff_st;		//�G�t�F�N�g�֘A���󂯓n���p���[�N
PORTRAIT_DATA *pd;	//�|�[�g���C�g�f�[�^�Q�Ɨp

u8 con_mes_buf[CON_MES_MAX];	//���b�Z�[�W�\���p�o�b�t�@�[
u8 con_mes_buf2[MONS_NAME_SIZE+EOM_SIZE];	//���b�Z�[�W�\���p�o�b�t�@�[

static u8 cp_main_flag;			//���C�����[�`������t���O

static u16 mos_value;			//���U�C�N�l�ۑ��p
static u16 mos_count;			//���U�C�N�l�\���J�E���g�p
static u8 mos_flag;				//���U�C�N�l�\������p0:�g�p����,1:�g�p

/********************************************************************/
/*                      OAM&etc                                     */
/********************************************************************/
static const ActOamData Effect256Oam =
{
	0,		//V�߼޼��
	0,		//�g��k��Ӱ�ސݒ�(OFF)
	0,		//OBJӰ�ސݒ�(NORMAL)
	1,		//ӻ޲��ݒ�(ON)
	1,		//�װӰ�ސݒ�(256�F)
	0,		//OBJ�`��ݒ�(�����`)

	0,		//H�߼޼��
	0,		//�g��k�����Ұ����ް�ݒ� & H,V�د��
	3,		//OBJ�T�C�Y�ݒ�(64x64)

	0,		//��׸�����ް
	0,		//�\����ײ��è��ݒ�
	0,		//��گ����ް
	0,		//AffineParam
};


void PortraitPictureSet( int number )
{
	PORTRAIT_DATA * pic_data = (PORTRAIT_DATA *)&(UserWork[TEMP_PORTRAIT_ADRS]);
	u8 * pic_number = (u8 *)&(UserWork[TEMP_PORTRAIT_PLACE]);
	u8 * pic_flag = (u8 *)&(UserWork[TEMP_PORTRAIT_MSGFLG]);

	*pic_data = Fld.PortraitData[number - 1];
	*pic_number = number -1;					//1origin����0rigin�֕␳
	*pic_flag = 0;								//0:�ʏ�\��
}

/********************************************************************/
/*                      ROUTIN                                      */
/********************************************************************/
//�R���e�X�g�p�ё���\���i�����������j
//�����F
//UserWork[TEMP_PORTRAIT_ADRS] :�Y������"PORTRAIT_DATA"���R�s�[�����G���A
//UserWork[TEMP_PORTRAIT_MSGFLG] :0�Œʏ�\��,1�Ő����Ȃ��\��(�v���r���[��)
//UserWork[TEMP_PORTRAIT_PLACE] :0-12�́h�G�̏ꏊ�h�ԍ�
//���v���r���[�ł̂Ƃ��́A���p�قփZ�b�g����f�[�^��
//�܂������������̂��Z�b�g����K�v������܂��B(�G�̏ꏊ,�^�C�g����ޓ�)
void contest_picture_init(void)
{
	cp_init_proc();
}

static const u16 black_color[] = {0x0000};

static void cp_init_proc(void)
{
	switch(sys.InitSeqWork){
	case 0:
		LasterInit();									// HDMA������
		SetVBlankFunc(NULL);

		pd = (PORTRAIT_DATA *)(&(UserWork[TEMP_PORTRAIT_ADRS]));

		mosaic_v_intr_init(1);
		register_init();
		sys.InitSeqWork++;
		break;
	case 1:
		PaletteAnimeInit();
		DIV_DMACLEAR(3, 0, VRAM,(VRAM_END - VRAM), 32);		/* �SVRAM������*/

		InitActor();
		sys.InitSeqWork++;
		break;
	case 2:
		pp_srand( sys.main_timer );

		InitKeyRead();

		mes_win_clear(UserWork[TEMP_PORTRAIT_MSGFLG]);
		sys.InitSeqWork++;
		break;
	case 3:
		pic_set(UserWork[TEMP_PORTRAIT_PLACE],UserWork[TEMP_PORTRAIT_MSGFLG]);
		sys.InitSeqWork++;
		break;
	case 4:
		mes_print(UserWork[TEMP_PORTRAIT_PLACE],UserWork[TEMP_PORTRAIT_MSGFLG]);
		PaletteWorkSet(black_color, 0, 2);
		DIV_DMACLEAR(3, 0, PLTT,(PLTT_END - PLTT), 32);		/* �SPALETTE������*/
													/* �v�Z��pal���g�p�����̂� */

		LightFadeReq(B_to_N);

		SetVBlankFunc(contest_picture_v_intr);

		cp_main_flag = CPM_FADE_IN;

		*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h0
							  | DISP_OBJ_ON					// OBJ�L��
							  | DISP_BG0_ON					// BG0�L��
							  | DISP_BG1_ON					// BG1�L��
							  | DISP_OBJ_CHAR_1D_MAP;		//1D OBJ map

		MainProcChange(contest_picture_main);
		break;
	}
}

/*==================================================================*/
/*==================================================================*/
//�R���e�X�g�p�ё���\���i�{�̕����j
void contest_picture_main(void)
{
	cp_main_sub();

	PaletteAnime();
}

/*==================================================================*/
/*==================================================================*/
//�R���e�X�g�p�ё���\���i�I�������j
void contest_picture_end(void)
{
	MainProcChange(sys.pReturnProc);
}

/********************************************************************/
/*                      SUB ROUTIN                                  */
/********************************************************************/
//���C�����[�`����������
static void cp_main_sub(void)
{
	switch(cp_main_flag){
	case CPM_FADE_IN:
		if( FadeData.fade_sw == 0 ){
			cp_main_flag++;
		}

		if(mos_flag){
			if(mos_count>0) {
				mos_count--;
			}
		}
		break;
	case CPM_KEY_CHECK:
		if (( sys.Trg & A_BUTTON )||( sys.Trg & B_BUTTON)){
			cp_main_flag++;
			PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
		}

		if(mos_flag){
			mos_count=0;
		}
		break;
	case CPM_FADE_OUT:
		if( FadeData.fade_sw == 0 ){
			MainProcChange(contest_picture_end);
		}

		if(mos_flag){
			if(mos_count<(MOS_MAX*MOS_BAI)) {
				mos_count++;
			}
		}
		break;
	}
}


/*==================================================================*/
/*==================================================================*/
//�����\���n������
static void mes_win_clear(u8 cp_type)
{
	NMenuScreenInit(CON_PIC_TYPE);
	PrintScreenInit(CON_PIC_TYPE);			// ���b�Z�[�W�E�B���h�E���䏉����
}

/*==================================================================*/
/*==================================================================*/
//�����\��
static void mes_print(u8 port_no,u8 cp_type)
{
	u8 i;
	u8 con_type;

	if(cp_type==1) { //�����\���Ȃ����[�h�̎��͖���
		return;		
	}

	for(i=0;i<CON_MES_MAX;i++) con_mes_buf[i]=EOM_;	//���b�Z�[�W���[�N�N���A
	for(i=0;i<(MONS_NAME_SIZE+EOM_SIZE);i++) con_mes_buf2[i]=EOM_;	//���b�Z�[�W���[�N�N���A

	con_type = pd->pict_type;
	if(port_no < 8){		//�R���e�X�g
		PM_strcpy(con_mes_buf,con_name_tbl[con_type]);	//"�R���e�X�g��"

		PM_strcat(con_mes_buf,con_reg_mes1);			//�R���e�X�g�@�䂤���傤
		PM_strcat(con_mes_buf,pd->oyaname);				//"�u���[�_�[��"
		PM_strcat(con_mes_buf,con_reg_mes2);			//��

		NickNameCopy(con_mes_buf2,pd->nickname);				//"�|�P������"
		PM_strcat(con_mes_buf,con_mes_buf2);				//

		NMenuMsgWrite(con_mes_buf,CON_MES_X,CON_MES_Y);
	}else{					//���p��
		PM_strcpy(con_mes_buf,mus_mes_str[con_type][0]);	//�O����

		NickNameCopy(con_mes_buf2,pd->nickname);				//"�|�P������"
		PM_strcat(con_mes_buf,con_mes_buf2);				//

		PM_strcat(con_mes_buf,mus_mes_str[con_type][1]);	//�㔼��

		NMenuMsgWrite(con_mes_buf,MUS_MES_X,MUS_MES_Y);
	}
}

/*==================================================================*/
/*==================================================================*/
//���W�X�^�[������
static void register_init(void)
{
	*(vu16 *)REG_DISPCNT = 	0;

    *(vu16 *)REG_IE |= V_BLANK_INTR_FLAG;

	*(vu16 *)REG_BG0CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_2
	| BG_SB_NO << BG_SCREEN_BASE_SHIFT | BG_CB_NO << BG_CHAR_BASE_SHIFT
	| BG_COLOR_16 | BG_MOS_ON;

	*(vu16 *)REG_BG1CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_1
	| MES_SB_NO << BG_SCREEN_BASE_SHIFT | MES_CB_NO << BG_CHAR_BASE_SHIFT
	| BG_COLOR_16 | BG_MOS_ON;

   	*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
   	*(vu16 *)REG_BLDALPHA = 0;
   	*(vu16 *)REG_BLDY	  = 0;
}

/*==================================================================*/
/*==================================================================*/
//V�u�����N�ɒu���Ƃ����U�C�N�p�ݒ�̃C�j�V����
static void mosaic_v_intr_init(u8 no)
{
	if(no==0){		//�g�p���Ȃ�
		mos_flag=0;
		mos_value=0;
		mos_count=0;
	}else{			//�g�p����
		mos_flag=1;
		mos_value = MOS_MAX;
		mos_count = mos_value*MOS_BAI;
	}
}

/*==================================================================*/
/*==================================================================*/
//V�u�����N�ɒu���Ƃ����U�C�N�p�ݒ�
static void mosaic_v_intr(void)
{
	if(mos_flag==0) {
		*(vu16 *)REG_MOSAIC = 0;
		return;
	}

//���U�C�N�p�ݒ�(TextBG�͖���Z�b�g���Ȃ��ƌ��ʖ����̂�
	*(vu16 *)REG_BG1CNT = BG_SCREEN_SIZE_0 | BG_PRIORITY_1
	| MES_SB_NO << BG_SCREEN_BASE_SHIFT | MES_CB_NO << BG_CHAR_BASE_SHIFT
	| BG_COLOR_16 | BG_MOS_ON;

	mos_value = mos_count / MOS_BAI;
	*(vu16 *)REG_MOSAIC = (mos_value<<12) | (mos_value<<8) | (mos_value<<4) | mos_value;
}

/*==================================================================*/
/*==================================================================*/
//V�u�����N�p���[�`��
static void contest_picture_v_intr(void)
{
	mosaic_v_intr();

	actOAMtrans();
	actDMAtrans();

	PaletteFadeTrans();
}

/*==================================================================*/
/*			���k�L�������𓀂��h�b�g��                              */
/*==================================================================*/
//�|�P����OBJ�L�����f�[�^���f�R�[�h
//pal_p&bg_p���w�������G���A�փZ�b�g
//p_no:�|�P�����ԍ�
//ou:0:�\�A1:��
static void poke_bit_decord(u16 p_no,u8 ou)				//�L�����̃f�R�[�h
{
	//�J���[�p���b�g�擾
	DecordVram((void *)(PokePalGetPara(p_no,pd->idno,pd->personal_rnd)), (void *)pal_p);

	//��0x2000000�̓f�R�[�h�p�̃e���|�����ł����A���݂̃��[�`���ł͎��ۂɂ͎g���܂���(dummy)�B
	//�̂̃��[�`���Ƃ̌݊�(�f�R�[�h�������ɖ߂����Ƃ��̂���)���l���ĔO�̂��ߒu���Ă��܂��B
	if(ou==0){		//�\
		DecordPokeCellMakeBuchi(&PM2FObjDataCell[p_no],pm2fpara[p_no].size,
			pm2fpara[p_no].patcnt,(void *)0x2000000,
			(void *)DecordAdrs[BSS_CLIENT_ENEMY],p_no,pd->personal_rnd);

		bit_get_obj16((void *)DecordAdrs[BSS_CLIENT_ENEMY],pal_p,bg_p);
	}else{			//��
		DecordPokeCellMakeBuchi(&PM2BObjDataCell[p_no],pm2bpara[p_no].size,
			pm2bpara[p_no].patcnt,(void *)0x2000000,
			(void *)DecordAdrs[BSS_CLIENT_MINE],p_no,pd->personal_rnd);

		bit_get_obj16((void *)DecordAdrs[BSS_CLIENT_MINE],pal_p,bg_p);
	}
}

/*==================================================================*/
/*	�t���J���[dot�փL��������̕ϊ�                                 */
/*==================================================================*/
//16�F�|�P������(8*8char(64*64dot))
//bin:�f�[�^���A�h���X
//pa:�p���b�g�A�h���X
//bout:�o�͐�
static void bit_get_obj16(u8 *bin,u16 *pa,u16 *bout)
{
	u16 p,q,r,s;
	u8 z;

	//���L�����ƂȂ���̂�W�J�iOBJ->Bitmap)
	for(s=0;s<8;s++){					//Yblock
		for(r=0;r<8;r++){				//Xblock
			for(q=0;q<8;q++){				//Ychar
				for(p=0;p<8;p++){			//Xchar
					z = *(u8 *)(bin + ((((s<<3)+r)<<5)+(q<<2) + (p>>1)));

					if(p%2){	//�
						z = (z & 0xf0)>>4;
					}else{		//����
						z = z & 0x0f;
					}

					if(z==0){		//�����F�̂Ƃ�
						*(u16 *)(bout + (((s<<3)+q)<<6) +((r<<3)+p)) = NUKE_BLACK;	//�����F��
					}else{
						*(u16 *)(bout + (((s<<3)+q)<<6) +((r<<3)+p)) = *(pa+z);
					}
				}
			}
		}
	}
}

/*==================================================================*/
/*	BG set                                                          */
/*==================================================================*/

static void bg_set(u8 port_no,u8 cp_type)
{
	u8 x,y;

	PaletteWorkSet(frame_Palette,PA_BG0,16*8*2);
	if(cp_type==1) {		//�����\���Ȃ����[�h�̎��͕ʔw�i
		switch((pd->pict_type/3)){
		case 0:
			RLUnCompVram((void*)frame0_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompWram((void*)frame0_asc_RL,bg_p);
			break;
		case 1:
			RLUnCompVram((void*)frame1_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompWram((void*)frame1_asc_RL,bg_p);
			break;
		case 2:
			RLUnCompVram((void*)frame2_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompWram((void*)frame2_asc_RL,bg_p);
			break;
		case 3:
			RLUnCompVram((void*)frame3_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompWram((void*)frame3_asc_RL,bg_p);
			break;
		case 4:
			RLUnCompVram((void*)frame4_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompWram((void*)frame4_asc_RL,bg_p);
			break;
		}

		//�w�i�𔒂�
		for(y=0;y<20;y++){
			for(x=0;x<32;x++){
				*(u16 *)(PIC_EFF_SCREEN_BASE_ADDR + (y*32+x)*2 ) = DUMMY_WHITE_CHAR;		//��
			}
		}

		//�g�t���œ]��(�N�_�̰1�̈ʒu����dummy_pic_x or y���{2(�g��))
		for(y=0;y<DUMMY_PIC_H+2;y++){
			for(x=0;x<DUMMY_PIC_W+2;x++){
				*(u16 *)(PIC_EFF_SCREEN_BASE_ADDR + ((DUMMY_PIC_Y-1+y)*32+(DUMMY_PIC_X-1+x))*2 )
					= *(bg_p + (DUMMY_PIC_Y-1+y)*32+(DUMMY_PIC_X-1+x));
			}
		}

		//��ԏ�̘g�������I�ɓ������̂�(�ʏ�\��������͂���������)
		for(x=0;x<DUMMY_PIC_W;x++){
			*(u16 *)(PIC_EFF_SCREEN_BASE_ADDR + ((DUMMY_PIC_Y-1)*32+(DUMMY_PIC_X+x))*2 )
					= *(bg_p + (DUMMY_PIC_Y-1)*32+(DUMMY_PIC_X));
		}

		return;
	}

	if(port_no < 8){		//�R���e�X�g�w�i�\��
		RLUnCompVram((void*)frame_c_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
		RLUnCompVram((void*)frame_c_asc_RL,(void*)PIC_EFF_SCREEN_BASE_ADDR);
	}else{
		switch((pd->pict_type/3)){
		case 0:		//�J�b�R�悳
			RLUnCompVram((void*)frame0_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompVram((void*)frame0_asc_RL,(void*)PIC_EFF_SCREEN_BASE_ADDR);
			break;
		case 1:		//��������
			RLUnCompVram((void*)frame1_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompVram((void*)frame1_asc_RL,(void*)PIC_EFF_SCREEN_BASE_ADDR);
			break;
		case 2:		//���킢��
			RLUnCompVram((void*)frame2_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompVram((void*)frame2_asc_RL,(void*)PIC_EFF_SCREEN_BASE_ADDR);
			break;
		case 3:		//��������
			RLUnCompVram((void*)frame3_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompVram((void*)frame3_asc_RL,(void*)PIC_EFF_SCREEN_BASE_ADDR);
			break;
		case 4:		//�����܂���
			RLUnCompVram((void*)frame4_sch_RL,(void*)PIC_EFF_CHAR_BASE_ADDR);
			RLUnCompVram((void*)frame4_asc_RL,(void*)PIC_EFF_SCREEN_BASE_ADDR);
			break;
		}
	}
}

/*==================================================================*/
/*	OBJ set                                                         */
/*==================================================================*/
static void obj_set(u8 port_no)
{
	sys.OAMbuf[0] = Effect256Oam;
	sys.OAMbuf[0].CharNo = 0;
	if(port_no < 8){		//�R���e�X�g
		sys.OAMbuf[0].HPos = POKE_X1;
		sys.OAMbuf[0].VPos = POKE_Y1;
	}else{
		sys.OAMbuf[0].HPos = POKE_X2;
		sys.OAMbuf[0].VPos = POKE_Y2;
	}
}

/*==================================================================*/
/*	 �z�u�ꏊ�ԍ�����G�t�F�N�g�ԍ��ɕϊ�                           */
/*==================================================================*/
static u8 port_num_to_eff_num(u8 port_no)
{
	u8 eff_no;

	if(port_no < 8){
		eff_no = pd->pict_type;
	}else{
		eff_no = (pd->pict_type) / 3;
	}
	switch(eff_no){			//�����ԍ��֕ϊ�
	case 0:			//�������悳->�֊s
		eff_no = EFF_RINKAKU;
		break;
	case 1:			//��������->�W���J���[
		eff_no = EFF_AWAI_POKE;
		break;
	case 2:			//���킢��->�p�X�e��
		eff_no = EFF_PASUTERU_POKE;
		break;
	case 3:			//��������->�n�G
		eff_no = EFF_SUMIE;
		break;
	case 4:			//�����܂���->�O���[�X�P�[��
		eff_no = EFF_GRAY32;
		break;
	}

	return eff_no;
}

/*==================================================================*/
/*	 �v�Z�p���ʃ��[�N�G���A�Z�b�g                                   */
/*==================================================================*/
static void work_area_set(void)
{
	pal_p	= (u16 *)(&(UserWork[TEMP_PAL_AREA]));
	bg_p	= (u16 *)(&(UserWork[TEMP_BG_AREA]));
}

/*==================================================================*/
/*	 �r�b�g�f�[�^->�G�t�F�N�g->�L����&�p���b�g�ϊ�                  */
/*==================================================================*/
static void eff_obj(u8 eff_no)
{
	eff_st.bg_adrs = bg_p;
	eff_st.pal_adrs= pal_p;
	eff_st.pal_p=0;
	eff_st.pat= pd->personal_rnd % 256;		//�̪�đ���256����݂܂ł̑Ή��Ȃ̂�

	//�|�P�����摜�𓀐�p�ݒ�(�v�Z�G���A�Ɖ摜�������傫��)
	eff_st.eax = 0;				
	eff_st.eay = 0;
	eff_st.eah = PA_H;
	eff_st.eav = PA_V;
	eff_st.sah = PA_H;
	eff_st.sav = PA_V;

	switch(eff_no){
	case EFF_SUMIE:
	case EFF_GRAY32:
		eff_st.mode = COL_MODE_32G;		//�R�Q�F�O���[���[�hOBJ
		break;
	case EFF_RINKAKU:
	case EFF_AWAI_POKE:
	case EFF_PASUTERU_POKE:
	default:
		eff_st.mode = COL_MODE_223;		//�J���[�p���b�g�Q�Q�R�F�g�p
		break;
	}
	eff_st.out_mode = OUT_COL_256OBJ;		//256�F���[�hOBJ
	eff_st.eff_no = eff_no;
	eff_st.chr_adrs = (u8 *)(OBJ_MODE0_VRAM+0x20*0);	//16�F0�L������;

	poke_bit_effect(&eff_st);				//�G�t�F�N�g���A
	poke_bit_color_down(&eff_st);			//256�F�֗��Ƃ���
	poke_bit_to_obj(&eff_st);				//OBJ�Ƃ��ĕ\��

	PaletteWorkSet(pal_p,PA_OBJ0,256*2);	//���[�N����p���b�gRAM�֓]��(256�F)
}

/*==================================================================*/
/*==================================================================*/
//�ё���Z�b�g
static void pic_set(u8 port_no,u8 cp_type)
{
	u8 eff_no;

	//pal_p&bg_p�̃G���A�ݒ�
	work_area_set();

	//�|�P�L�����̃f�R�[�h pal_p&bg_p�փr�b�g�f�[�^�W�J
	poke_bit_decord(pd->monsno,0);

	eff_no = port_num_to_eff_num(port_no);

	//pal_p&bg_p�̃r�b�g�f�[�^���G�t�F�N�g���Ăn�a�i�L�������p���b�g�֕ϊ�
	eff_obj(eff_no);

	obj_set(port_no);
	bg_set(port_no,cp_type);
}

/*==================================================================*/
/*==================================================================*/

/********************************************************************/
