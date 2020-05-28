//=========================================================================
//	
//	�|�P����������f�[�^���
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
#include "b_return.h"
#include "config.h"
#include "message.h"
#include "ev_flag.h"

#define __ZUKAN_DATA_GLOBAL__
#include "zukan.h"

#include "zu_d.dat"

extern	void	affineWorkSet(u8 num,u16 H_X,u16 V_X,u16 H_Y,u16 V_Y);
extern	void	VoicePlay( u16 no, s8 pan );
extern	void	BunpuzuInit(u16 pokeno, pFunc ret_func, pFunc ret_func2);
extern	void	BunpuzuTaskInit(u16 pokeno, u8 *flg);

extern	const actAffAnm * const PmAffAnmTblF[];
extern const actAnm *const PokeActAnmTbl[];
extern	const actAnm *const *const TRFAnmTbl[];

static	void	ZukanDataInitTask(u8 id);
static	void	ZukanDataKeyTask(u8 id);
static	void	ZukanDataChangeTask(u8 id);
static	void	ZukanDataExitTask(u8 id);
static	void	ZukanDataBunpuInitTask(u8 id);
static	void	ZukanDataBunpuKeyTask(u8 id);
static	void	ZukanDataBunpuExitTask(u8 id);
static	void	ZukanDataVoiceInitTask(u8 id);
static	void	ZukanDataVoiceKeyTask(u8 id);
static	void	ZukanDataVoiceExitTask(u8 id);
static	void	ZukanDataOokisaInitTask(u8 id);
static	void	ZukanDataOokisaKeyTask(u8 id);
static	void	ZukanDataOokisaExitTask(u8 id);
static	void	ZukanMenuScrSet(u16 vp);
static	void	ZukanMenuCursorSet(u8 pos,u16 vp);
static	void	ZukanMenu2ScrSet(u16 vp);
static	void	ZukanMenu2CursorSet(u8 pos,u16 vp);

ZUKAN_LIST*	zmd = {0};
pFunc		Z_pVBlank_bak;

static	const	u16	monotone_Palette[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#define	CHG_REQ	(0)	//�؂�ւ��t���O
#define	ACT_F	(1)	//�|�P�����A�N�^�[�ݒ�t���O(1 = ���Ȃ�)
#define	FADE_F	(2)	//�t�F�[�h�ݒ�t���O(1 = ���j���[[BG1]�́A���Ȃ�)
#define	VOICE_F	(3)	//�Ȃ������ݒ�t���O(1 = ���Ȃ�)
#define	ACT_STR	(4)	//�|�P�����A�N�^�[STRUCT

#define	MENU_PAL1	(2)
#define	MENU_PAL2	(4)

#define	MENU_PAL_MASK	((0x00000001 << MENU_PAL1) + (0x00000001 << MENU_PAL2))
/*==================================================================*/
/*                                                                  */
/*				�|�P�����f�[�^��ʑ���								*/
/*                                                                  */
/*==================================================================*/
/*==================================================================*/
/*                                                                  */
/*				��ʑ���^�X�N�ݒ�									*/
/*                                                                  */
/*==================================================================*/
u8		ZukanDataTaskSet(ZUKAN_LIST* data,u8 actstr)
{
	u8	id;

	zmd = data;

	id = AddTask(ZukanDataInitTask, 0);

	TaskTable[id].work[CHG_REQ]	= 0;
	TaskTable[id].work[ACT_F]	= 1;
	TaskTable[id].work[FADE_F]	= 0;
	TaskTable[id].work[VOICE_F] = 0;
	TaskTable[id].work[ACT_STR]	= actstr;

	return id;
}

/*==================================================================*/
/*                                                                  */
/*				��ʑ���^�X�N�؂�ւ�								*/
/*                                                                  */
/*==================================================================*/
u8		ZukanDataTaskChangeCheck(u8 id)
{
	if((!TaskTable[id].work[CHG_REQ])&&(TaskTable[id].TaskAdrs == ZukanDataKeyTask)){
		return	0;
	}else{
		return	1;
	}
}

u8		ZukanDataTaskChange(ZUKAN_LIST* data,u8 id)
{
	zmd = data;

	TaskTable[id].work[CHG_REQ] = 1;
	TaskTable[id].work[ACT_F]	= 0;
	TaskTable[id].work[FADE_F]	= 0;
	TaskTable[id].work[VOICE_F] = 0;

	return id;
}

/*==================================================================*/
/*                                                                  */
/*				��ʏ�����											*/
/*                                                                  */
/*==================================================================*/
static	void	ZukanDataInitTask(u8 id)
{
	u32	fade;
	u16	data;

	switch(sys.InitSeqWork){
	default:
	case 0:
		//�t�F�[�h�҂�
		if(FadeData.fade_sw) break;

		zw->mode = Z_MODE1;
		Z_pVBlank_bak = sys.pVBlank;
		SetVBlankFunc(NULL);				//�����݊֐����쒆�~

		data = 0;
		if(TaskTable[id].work[ACT_F])	data |= DISP_OBJ_ON;
		if(TaskTable[id].work[FADE_F])	data |= DISP_BG1_ON;
		ZukanScreenRegInit(data);	//�\�����W�X�^������

		sys.InitSeqWork = 1;
		break;
	case 1:
		//�f�[�^�]��
		LZ77UnCompVram((void*)zkn_ch_LZ,(void*)(BG_VRAM + 0x0000));		//BG CGX�]��
		LZ77UnCompVram((void*)zkn_scp_LZ,
					(void*)(BG_VRAM + (MODE1_BG3_SCRVRAM*0x800)));	//BG3 MAP�]��(�w�i)
		ZukanPokeFootTrans(zmd->num,MODE1_BG2_CHRVRAM,MODE1_FOOTCGX_NO);
		sys.InitSeqWork++;
		break;
	case 2:
		ZukanMenuScrSet(MODE1_BG1_SCRVRAM);	//BG1 MAP�]��(���j���[));
		ZukanMenuCursorSet(zw->menu_p,MODE1_BG1_SCRVRAM);
		ZukanPaletteSet();
		sys.InitSeqWork++;
		break;
	case 3:
		//�\�����[�`���������P
		PrintScreenInit(ZUKANLIST_TYPE);
		NMenuScreenInit(ZUKANLIST_TYPE);
		sys.InitSeqWork++;
		break;
	case 4:
		//�f�[�^�\��
		if(zw->list_mode == ZLIST_NEW){
			ZukanPokeNumPut(PokeZukanNo2HouenNoGet(zmd->num),13,3);	//�i���o�[
		}else{
			ZukanPokeNumPut(zmd->num,13,3);			//�i���o�[
		}
		ZukanPokeNamePut(zmd->num,17,3);		//���O
		NMenuMsgWrite(Z_Datainfo1str,13,5);		//�^�C�v�i�����ݒ�j
		NMenuMsgWrite(Z_Datainfo2str,16,7);		//�������i�����ݒ�j
		NMenuMsgWrite(Z_Datainfo3str,16,9);		//�������i�����ݒ�j

		//�|�P�����擾���̉�ʏ��ω�
		if(zmd->get){
			ZukanDataTypeNamePut((u8*)zukanData[zmd->num].typename,13,5);
			ZukanDataNumPut(zukanData[zmd->num].takasa,16,7);
			ZukanDataNumPut(zukanData[zmd->num].omosa,16,9);
			NMenuMsgWrite(zukanData[zmd->num].text,3,13);
			ZukanPokeFootPut(MODE1_BG2_SCRVRAM,MODE1_FOOTCGX_NO);
		}else{
			NMenuMsgWrite(noget_text,3,13);
			//�p���b�g�G�t�F�N�g
			PaletteWorkSet(&PaletteWork[0x00+0x01],PA_BG3+PA_COL1,0x20-2);
		}
		sys.InitSeqWork++;
		break;
	case 5:
		//�|�P�����n�a�i�ݒ�
		if(!TaskTable[id].work[ACT_F]){
			TaskTable[id].work[ACT_STR]	= 
				ZukanPokeActorSet(zmd->num,MODE1_POKE_POS_X,MODE1_POKE_POS_Y,0);
			ActWork[TaskTable[id].work[ACT_STR]].oamData.Priority = 0;
		}
		sys.InitSeqWork++;
		break;
	case 6:
		//�n�a�i�֘A�\�����t�F�[�h�ݒ�
		fade = 0;
		if(TaskTable[id].work[FADE_F]){
			fade |= MENU_PAL_MASK;
		}
		if(TaskTable[id].work[ACT_F]){
			fade |= (0x00000001 << (16+ActWork[TaskTable[id].work[ACT_STR]].oamData.Pltt));
		}
		PaletteFadeReq((0xffffffff^fade),0,16,0,0);
		SetVBlankFunc(Z_pVBlank_bak);				//�����݊֐��ݒ�(VBLANK)
		sys.InitSeqWork++;
		break;
	case 7:
		//���W�X�^�ݒ�
		*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
		*(vu16 *)REG_BLDALPHA = 0;
		*(vu16 *)REG_BLDY	  = 0;
		*(vu16 *)REG_BG3CNT = BG_COLOR_16					//16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_3					//BG�D�揇��
							| MODE1_BG3_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
							| MODE1_BG3_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
							;
		*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_0					//BG�D�揇��
							| MODE1_BG1_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
							| MODE1_BG1_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
							;
		*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
							  |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							  | DISP_BG3_ON					// BG3�L��
							  | DISP_BG2_ON					// BG2�L��
							  | DISP_BG1_ON					// BG2�L��
							  | DISP_OBJ_ON;
		sys.InitSeqWork++;
		break;
	case 8:
		//�t�F�[�h�҂��������Đ�
		if(FadeData.fade_sw) break;

		sys.InitSeqWork++;
		if(!TaskTable[id].work[VOICE_F]){
			VoicePlayStop();
			VoicePlay2(PokeZukanNo2MonsNoGet(zmd->num),0,125,10);
			break;
		}
		sys.InitSeqWork++;
		break;
	case 9:
		if(VoiceEndCheck())		break;
		sys.InitSeqWork++;
		break;
	case 10:
		TaskTable[id].work[CHG_REQ] = 0;
		TaskTable[id].work[ACT_F]	= 0;
		TaskTable[id].work[FADE_F]	= 1;
		TaskTable[id].work[VOICE_F] = 1;

		TaskTable[id].TaskAdrs = ZukanDataKeyTask;
		sys.InitSeqWork = 0;
		break;
	}
}

/*==================================================================*/
static	void	ZukanDataKeyTask(u8 id)
{
	//�f�[�^�ړ����N�G�X�g
	if(TaskTable[id].work[CHG_REQ]){
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs = ZukanDataChangeTask;
		SePlay(SE_Z_SCROLL);
		return;
	}

	//�a�{�^������(���X�g��ʂ�)
	if(sys.Trg & B_BUTTON){
		PaletteFadeReq(0xffffffff,0,0,16,0);
		TaskTable[id].TaskAdrs = ZukanDataExitTask;
		SePlay(SE_PC_OFF);
		return;
	}
	//�`�{�^������(�e��f�[�^��ʁ��R�}���h)
	if(sys.Trg & A_BUTTON){
		switch(zw->menu_p){
		case 0:
			//���z�}
			PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,0,16,0);
			TaskTable[id].TaskAdrs = ZukanDataBunpuInitTask;
			SePlay(SE_PIN);
			break;
		case 1:
			//����
			PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,0,16,0);
			TaskTable[id].TaskAdrs = ZukanDataVoiceInitTask;
			SePlay(SE_PIN);
			break;
		case 2:
			//�������������(�|�P�����擾���̂�)
			if(!zmd->get){
				SePlay(SE_HAZURE);
			}else{
				PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,0,16,0);
				TaskTable[id].TaskAdrs = ZukanDataOokisaInitTask;
				SePlay(SE_PIN);
			}
			break;
		case 3:
			//���ǂ�
			PaletteFadeReq(0xffffffff,0,0,16,0);
			TaskTable[id].TaskAdrs = ZukanDataExitTask;
			SePlay(SE_PC_OFF);
			break;
		}
		return;
	}
	if(((sys.Trg & L_KEY)||((sys.Trg & L_BUTTON)&&(MyData.key_config == CNT_MD_LR)))
		&&(zw->menu_p > 0)){
		zw->menu_p--;
		ZukanMenuCursorSet(zw->menu_p,MODE1_BG1_SCRVRAM);
		SePlay(SE_Z_PAGE);
		return;
	}
	if(((sys.Trg & R_KEY)||((sys.Trg & R_BUTTON)&&(MyData.key_config == CNT_MD_LR)))
		&&(zw->menu_p < 3)){
		zw->menu_p++;
		ZukanMenuCursorSet(zw->menu_p,MODE1_BG1_SCRVRAM);
		SePlay(SE_Z_PAGE);
		return;
	}
}

/*==================================================================*/
static	void	ZukanDataChangeTask(u8 id)
{
	if(FadeData.fade_sw) return;

	TaskTable[id].TaskAdrs = ZukanDataInitTask;
}

/*==================================================================*/
static	void	ZukanDataExitTask(u8 id)
{
	if(FadeData.fade_sw) return;

	DelTask(id);
}










/*==================================================================*/
/*                                                                  */
/*				���z�}��ʑ���										*/
/*                                                                  */
/*==================================================================*/
static	void	ZukanDataBunpuInitTask(u8 id)
{
	switch(sys.InitSeqWork){
	default:
	case 0:
		//�t�F�[�h�҂�
		if(FadeData.fade_sw) break;

		zw->mode = Z_MODE5;
		Z_pVBlank_bak = sys.pVBlank;
		SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)
		ZukanScreenRegInit(DISP_BG1_ON);	//�\�����W�X�^������

		zw->menu_p = 0;
		sys.InitSeqWork = 1;
		break;
	case 1:
		ZukanMenu2ScrSet(MODE5_BG1_SCRVRAM);	//BG1 MAP�]��(���j���[));
		ZukanMenu2CursorSet(0,MODE5_BG1_SCRVRAM);
		ZukanPaletteSet();
		*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_0					//BG�D�揇��
						| MODE5_BG1_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| MODE5_BG1_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
						;
		sys.InitSeqWork++;
		break;
	case 2:
		//���z�}��ʂ�
		BunpuzuTaskInit(PokeZukanNo2MonsNoGet(zmd->num),(u8*)(&(zw->DataModeRetFlag)));
		SetVBlankFunc(Z_pVBlank_bak);				//�����݊֐��ݒ�(VBLANK)
		zw->DataModeRetFlag = 0;
		sys.InitSeqWork = 0;
		TaskTable[id].TaskAdrs = ZukanDataBunpuKeyTask;
		break;
	}
}

/*==================================================================*/
static	void	ZukanDataBunpuKeyTask(u8 id)
{
	if(!zw->DataModeRetFlag)	return;

	TaskTable[id].TaskAdrs = ZukanDataBunpuExitTask;
}

/*==================================================================*/
static	void	ZukanDataBunpuExitTask(u8 id)
{
	if(FadeData.fade_sw) return;
	//�}�Ӄf�[�^��ʂ�
	switch(zw->DataModeRetFlag){

	default:
	case 1:
		TaskTable[id].TaskAdrs = ZukanDataInitTask;
		break;
	case 2:
		TaskTable[id].TaskAdrs = ZukanDataVoiceInitTask;
		break;
	}
}










/*==================================================================*/
/*                                                                  */
/*				������ʑ���										*/
/*                                                                  */
/*==================================================================*/
extern	u8		VoiceEndCheck2( void );
static	void	ButtonPaletteEffect(u8 mode);
static	void	ZukanDataVoiceInitTask(u8 id)
{
	u8	offs;
	HakeiSet	hs;
	MeterSet	ms;

	switch(sys.InitSeqWork){
	default:
	case 0:
		//�t�F�[�h�҂�
		if(FadeData.fade_sw) return;

		m4aMPlayStop( &m4a_mplay000 );
		zw->mode = Z_MODE6;
		Z_pVBlank_bak = sys.pVBlank;
		SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)
		ZukanScreenRegInit(DISP_BG1_ON);	//�\�����W�X�^������

		zw->menu_p = 1;
		sys.InitSeqWork = 1;
		break;
	case 1:
		//�f�[�^�]��
		LZ77UnCompVram((void*)zkn_ch_LZ,(void*)(BG_VRAM + 0x0000));		//BG CGX�]��
		LZ77UnCompVram((void*)zkn_scv_LZ,
					(void*)(BG_VRAM + (MODE6_BG2_SCRVRAM*0x800)));	//BG2 MAP�]��(�w�i)
		sys.InitSeqWork++;
		break;
	case 2:
		ZukanMenu2ScrSet(MODE6_BG1_SCRVRAM);	//BG1 MAP�]��(���j���[));
		ZukanMenu2CursorSet(1,MODE6_BG1_SCRVRAM);
		ZukanPaletteSet();
		DIV_DMACLEAR(3,0,BG_VRAM + (MODE6_BG0_SCRVRAM * 0x800),0x0500,16);	//BG1 MAP�]��(�N���A)
		sys.InitSeqWork++;
		break;
	case 3:
		//�\�����[�`���������P
		PrintScreenInit(ZUKANMAIN_TYPE);				// �}�Ӄ��X�g�p(CGX_B = 2,SCR_B = 30)
		NMenuScreenInit(ZUKANMAIN_TYPE);				// �}�Ӊ�ʗp(���p)
		PaletteAnimeInit();
		sys.InitSeqWork++;
		break;
	case 4:
		//�f�[�^�\��
		offs = ZukanPokeNamePut(zmd->num,11,4);
		NMenuMsgWrite(Z_DataVoiceindexstr,11+offs,4);
		NMenuMsgWrite(Z_DataVoiceindex2str,11,6);
		sys.InitSeqWork++;
		break;
	case 5:
		TaskTable[id].work[ACT_STR]	= 
			ZukanPokeActorSet(zmd->num,MODE1_POKE_POS_X,MODE1_POKE_POS_Y,0);
		ActWork[TaskTable[id].work[ACT_STR]].oamData.Priority = 0;

		//������ʏ�����(�n�a�i)
		ZNakiInitSeq = 0;
		sys.InitSeqWork++;
		break;
	case 6:
		//������ʏ�����(�g�`�\���ݒ�)
		hs.ChrAddr	= MODE6_BG0_CHRVRAM*0x4000 + 0x20;	//NULL�L�����p��1�L�������炷
		hs.ScrBlock = MODE6_BG0_SCRVRAM;
		hs.PalNo	= 8;
		hs.XSize	= 30;
		hs.YOffs	= 12;
		if(HakeiInit(&hs,0)){
			sys.InitSeqWork++;
			ZNakiInitSeq = 0;
		}
		break;
	case 7:
		//������ʏ�����(VU���[�^�[�\���ݒ�)
		ms.ChrAddr	= MODE6_BG2_CHRVRAM*0x4000 + 0x03000;	//�ߖ�̂��ߌ��Ԃɓ����
		ms.ScrBlock = MODE6_BG2_SCRVRAM;
		ms.PalNo	= 9;
		ms.XPos		= 18;
		ms.YPos		= 3;
		if(VuMeterInit(&ms,1)){
			sys.InitSeqWork++;
		}
		break;
	case 8:
		//�t�F�[�h�ݒ�
		PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,16,0,0);
		SetVBlankFunc(Z_pVBlank_bak);				//�����݊֐��ݒ�(VBLANK)
		sys.InitSeqWork++;
		break;
	case 9:
		//���W�X�^�ݒ�
		*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
		*(vu16 *)REG_BLDALPHA = 0;
		*(vu16 *)REG_BLDY	  = 0;
		
		*(vu16 *)REG_BG2CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| MODE6_BG2_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| MODE6_BG2_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
						;
		*(vu16 *)REG_BG0CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| MODE6_BG0_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| MODE6_BG0_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
						;
		*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_0					//BG�D�揇��
						| MODE6_BG1_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| MODE6_BG1_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
						;
		*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
                          |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG3_ON					// BG3�L��
						  | DISP_BG2_ON					// BG2�L��
						  | DISP_BG1_ON					// BG1�L��
						  | DISP_BG0_ON					// BG0�L��
						  | DISP_OBJ_ON;
		sys.InitSeqWork++;
		break;
	case 10:
//		if(FadeData.fade_sw) break;//�����������̂œ���Ńt�F�[�h�҂����Ȃ�

		zw->DataModeRetFlag = 0;
		sys.InitSeqWork = 0;
		TaskTable[id].TaskAdrs = ZukanDataVoiceKeyTask;
		break;
	}
}

/*==================================================================*/
static	void	ZukanDataVoiceKeyTask(u8 id)
{
	//�g�`�\�����X�N���[������
	HakeiMain(0);

	if(VoiceEndCheck2()){
		ButtonPaletteEffect(1);
	}else{
		ButtonPaletteEffect(0);
	}
	//�`�{�^������(�����Đ�)
	if(sys.Trg & A_BUTTON){
		ButtonPaletteEffect(1);
		HakeiStart(PokeZukanNo2MonsNoGet(zmd->num));
		return;
	}

	//�`�{�^���ȊO�̓t�F�[�h���͔������Ȃ�
	if(FadeData.fade_sw) return;

	//�a�{�^������(�|�P�����f�[�^��ʂ�)
	if(sys.Trg & B_BUTTON){
		PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,0,16,0);
		m4aMPlayContinue( &m4a_mplay000 );
		zw->DataModeRetFlag = 1;
		TaskTable[id].TaskAdrs = ZukanDataVoiceExitTask;
		SePlay(SE_PC_OFF);
		return;
	}
	if((sys.Trg & L_KEY)||((sys.Trg & L_BUTTON)&&(MyData.key_config == CNT_MD_LR))){
		//���z�}
		PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,0,16,0);
		m4aMPlayContinue( &m4a_mplay000 );
		zw->DataModeRetFlag = 2;
		TaskTable[id].TaskAdrs = ZukanDataVoiceExitTask;
		SePlay(SE_Z_PAGE);
		return;
	}
	if((sys.Trg & R_KEY)||((sys.Trg & R_BUTTON)&&(MyData.key_config == CNT_MD_LR))){
		//�������������(�|�P�����擾���̂�)
		if(!zmd->get){
			SePlay(SE_HAZURE);
		}else{
			PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,0,16,0);
			m4aMPlayContinue( &m4a_mplay000 );
			zw->DataModeRetFlag = 3;
			TaskTable[id].TaskAdrs = ZukanDataVoiceExitTask;
			SePlay(SE_Z_PAGE);
			return;
		}
	}
}

/*==================================================================*/
static	void	ZukanDataVoiceExitTask(u8 id)
{
	if(FadeData.fade_sw) return;

	VuMeterEnd();			//�I������(�������)
	switch(zw->DataModeRetFlag){

	default:
	case 1:
		//�}�Ӄf�[�^��ʂ�
		TaskTable[id].TaskAdrs = ZukanDataInitTask;
		break;
	case 2:
		//���z��ʂ�
		TaskTable[id].TaskAdrs = ZukanDataBunpuInitTask;
		break;
	case 3:
		//����������ʂ�
		TaskTable[id].TaskAdrs = ZukanDataOokisaInitTask;
		break;
	}
}

static	void	ButtonPaletteEffect(u8 mode)
{
	u16	pal;

	if(mode)	pal = 0x0392;//�n�m
	else		pal = 0x02af;//�n�e�e
	PaletteWorkSet(&pal,PA_BG5 + PA_COL13,0x02);
}









/*==================================================================*/
/*                                                                  */
/*				������������׉�ʑ���								*/
/*                                                                  */
/*==================================================================*/
static	void	ZukanDataOokisaInitTask(u8 id)
{
	u8	anum;

	switch(sys.InitSeqWork){
	default:
	case 0:
		//�t�F�[�h�҂�
		if(FadeData.fade_sw) break;

		zw->mode = Z_MODE7;
		Z_pVBlank_bak = sys.pVBlank;
		SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)
		ZukanScreenRegInit(DISP_BG1_ON);	//�\�����W�X�^������

		zw->menu_p = 2;
		sys.InitSeqWork = 1;
		break;
	case 1:
		//�f�[�^�]��
		LZ77UnCompVram((void*)zkn_ch_LZ,(void*)(BG_VRAM + 0x0000));		//BG CGX�]��
		LZ77UnCompVram((void*)zkn_scc_LZ,
					(void*)(BG_VRAM + (MODE7_BG2_SCRVRAM*0x800)));	//BG2 MAP�]��(�w�i)
		sys.InitSeqWork++;
		break;
	case 2:
		ZukanMenu2ScrSet(MODE7_BG1_SCRVRAM);	//BG1 MAP�]��(���j���[));
		ZukanMenu2CursorSet(2,MODE7_BG1_SCRVRAM);
		ZukanPaletteSet();
		sys.InitSeqWork++;
		break;
	case 3:
		//�\�����[�`���������P
		PrintScreenInit(ZUKANMAIN_TYPE);				// �}�Ӄ��X�g�p(CGX_B = 2,SCR_B = 31)
		NMenuScreenInit(ZUKANMAIN_TYPE);				// �}�Ӊ�ʗp(���p)
		ZukanPokeNamePut(zmd->num,4,15);
		NMenuMsgWrite(Z_DataOokisaindex1str,9,15);
		NMenuMsgWrite(MyData.my_name,11,15);
		NMenuMsgWrite(Z_DataOokisaindex2str,16,15);
		sys.InitSeqWork++;
		break;
	case 4:
		PaletteAnimeInit();
		sys.InitSeqWork++;
		break;
	case 5:
		//�g���[�i�[�A�N�^�[�ݒ�
		anum = ZukanTrainerActorSet(MyData.my_sex,152,64-8,0);
		ActWork[anum].oamData.AffineMode = 1;
		ActWork[anum].oamData.AffineParamNo = 1;
		ActWork[anum].oamData.Priority = 0;
		ActWork[anum].dy = zukanData[zmd->num].y_t;	//�����w��
		affineWorkSet(1,zukanData[zmd->num].affine_t,0,0,zukanData[zmd->num].affine_t);	//�䗦�w��
		PaletteWorkSet(&monotone_Palette[0],(ActWork[anum].oamData.Pltt+16)*0x10,0x20);
		sys.InitSeqWork++;
		break;
	case 6:
		//�|�P�����A�N�^�[�ݒ�
		anum = ZukanPokeActorSet(zmd->num,88,64-8,1);
		ActWork[anum].oamData.AffineMode = 1;
		ActWork[anum].oamData.AffineParamNo = 2;
		ActWork[anum].oamData.Priority = 0;
		ActWork[anum].dy = zukanData[zmd->num].y_p;	//�����w��
		affineWorkSet(2,zukanData[zmd->num].affine_p,0,0,zukanData[zmd->num].affine_p);	//�䗦�w��
		PaletteWorkSet(&monotone_Palette[0],(ActWork[anum].oamData.Pltt+16)*0x10,0x20);
		sys.InitSeqWork++;
		break;
	case 7:
		//�t�F�[�h�ݒ�
		PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,16,0,0);
		SetVBlankFunc(Z_pVBlank_bak);				//�����݊֐��ݒ�(VBLANK)
		sys.InitSeqWork++;
		break;
	case 8:
		//���W�X�^�ݒ�
		*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
		*(vu16 *)REG_BLDALPHA = 0;
		*(vu16 *)REG_BLDY	  = 0;
		*(vu16 *)REG_BG2CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_3					//BG�D�揇��
						| MODE7_BG2_SCRVRAM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| MODE7_BG2_CHRVRAM << BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
						;
		*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
				              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							  | DISP_BG3_ON					// BG3�L��
							  | DISP_BG2_ON					// BG2�L��
							  | DISP_BG1_ON					// BG2�L��
							  | DISP_OBJ_ON;
		sys.InitSeqWork++;
		break;
	case 9:
		if(FadeData.fade_sw) break;

		zw->DataModeRetFlag = 0;
		sys.InitSeqWork = 0;
		TaskTable[id].TaskAdrs = ZukanDataOokisaKeyTask;
		break;
	}
}

/*==================================================================*/
static	void	ZukanDataOokisaKeyTask(u8 id)
{
	//�a�{�^������(�|�P�����f�[�^��ʂ�)
	if(sys.Trg & B_BUTTON){
		PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,0,16,0);
		zw->DataModeRetFlag = 1;
		TaskTable[id].TaskAdrs = ZukanDataOokisaExitTask;
		SePlay(SE_PC_OFF);
		return;
	}
	if((sys.Trg & L_KEY)||((sys.Trg & L_BUTTON)&&(MyData.key_config == CNT_MD_LR))){
		//������ʂ�
		PaletteFadeReq((0xffffffff^MENU_PAL_MASK),0,0,16,0);
		zw->DataModeRetFlag = 2;
		TaskTable[id].TaskAdrs = ZukanDataOokisaExitTask;
		SePlay(SE_Z_PAGE);
		return;
	}
}

/*==================================================================*/
static	void	ZukanDataOokisaExitTask(u8 id)
{
	if(FadeData.fade_sw) return;

	switch(zw->DataModeRetFlag){

	default:
	case 1:
		//�}�Ӄf�[�^��ʂ�
		TaskTable[id].TaskAdrs = ZukanDataInitTask;
		break;
	case 2:
		//������ʂ�
		TaskTable[id].TaskAdrs = ZukanDataVoiceInitTask;
		break;
	}
}










/*==================================================================*/
/*                                                                  */
/*				���j���[�쐬										*/
/*                                                                  */
/*==================================================================*/
/*==================================================================*/
static	void	ZukanMenuScrSet(u16 vp)
{
	//vp �̓��j���[�X�N���[���ݒ�A�h���X�B
	LZ77UnCompVram((void*)zkn_scm1_LZ,(void*)(BG_VRAM+(vp*0x800)));	//BG1 MAP�]��(���j���[)
	DIV_DMACLEAR(3,0,BG_VRAM + (vp * 0x800) + 0x00c0,0x0440,16);	//BG1 MAP�]��(�c��)
}

/*==================================================================*/
static	void	ZukanMenu2ScrSet(u16 vp)
{
	//vp �̓��j���[�X�N���[���ݒ�A�h���X�B
	LZ77UnCompVram((void*)zkn_scm2_LZ,(void*)(BG_VRAM+(vp*0x800)));	//BG1 MAP�]��(���j���[)
	DIV_DMACLEAR(3,0,BG_VRAM + (vp * 0x800) + 0x00c0,0x0440,16);	//BG1 MAP�]��(�c��)
}

/*==================================================================*/
static	void	ZukanMenuCursorSet(u8 pos,u16 vp)
{
	u8	i,j,x;
	u16	chr,pal;

	for(j=0;j<4;j++){
		x = j*7 + 1;
		if(j == pos){
			pal = 0x2000;
		}else{
			pal = 0x4000;
		}
		for(i=0;i<7;i++){
			chr = *(u16 *)(BG_VRAM + vp*0x800 + (x+i)*2);
			chr &= 0x0fff;
			chr |= pal;
			*(u16 *)(BG_VRAM + vp*0x800 + (x+i)*2) = chr;
	
			chr = *(u16 *)(BG_VRAM + vp*0x800 + 64 + (x+i)*2);
			chr &= 0x0fff;
			chr |= pal;
			*(u16 *)(BG_VRAM + vp*0x800 + 64 + (x+i)*2) = chr;
		}
	}
}

/*==================================================================*/
static	void	ZukanMenu2CursorSet(u8 pos,u16 vp)
{
	u8	i,j,x;
	u16	chr,pal;

	for(j=0;j<4;j++){
		x = j*7 + 1;
		if((j == pos)||(j == 3)){
			pal = 0x2000;
		}else{
			pal = 0x4000;
		}
		for(i=0;i<7;i++){
			chr = *(u16 *)(BG_VRAM + vp*0x800 + (x+i)*2);
			chr &= 0x0fff;
			chr |= pal;
			*(u16 *)(BG_VRAM + vp*0x800 + (x+i)*2) = chr;
	
			chr = *(u16 *)(BG_VRAM + vp*0x800 + 64 + (x+i)*2);
			chr &= 0x0fff;
			chr |= pal;
			*(u16 *)(BG_VRAM + vp*0x800 + 64 + (x+i)*2) = chr;
		}
	}
}










/********************************************************************/
/*                                                                  */
/*				�|�P�����f�[�^�\���i�|�P�����ߊl���j				*/
/*                                                                  */
#define	SAMPLE_BG0_SCRVRAM	(0)			//���g�p
#define	SAMPLE_BG0_CHRVRAM	(0)			//���g�p
#define	SAMPLE_BG1_SCRVRAM	(0)			//���g�p
#define	SAMPLE_BG1_CHRVRAM	(0)			//���g�p
#define	SAMPLE_BG2_SCRVRAM	(14)		//ZUKANLIST_TYPE�g�p
#define	SAMPLE_BG2_CHRVRAM	(2)			//ZUKANLIST_TYPE�g�p
#define	SAMPLE_BG3_SCRVRAM	(15)		//�w�i
#define	SAMPLE_BG3_CHRVRAM	(1)			//�w�i
/********************************************************************/
static void		SamplePokeMove(actWork* aw);
static	void	ZukanSampleInitTask(u8 id);
static	void	ZukanSampleKeyTask(u8 id);
static	void	ZukanSampleExitTask(u8 id);

u8	ZukanSampleSet(u16 pokenum,u32 id,u32 per_rnd)
{
 	u8	taskID;

 	taskID = AddTask(ZukanSampleInitTask, 0);
 	TaskTable[taskID].work[0] = 0;	//�������V�[�P���X
 	TaskTable[taskID].work[1] = pokenum;	//�|�P�����i���o�[

 	TaskTable[taskID].work[12] = (id & 0x0000ffff);
 	TaskTable[taskID].work[13] = ((id>>16) & 0x0000ffff);
 	TaskTable[taskID].work[14] = (per_rnd & 0x0000ffff);
 	TaskTable[taskID].work[15] = ((per_rnd>>16) & 0x0000ffff);

 	return taskID;
}

static	void	ZukanSampleInitTask(u8 id)
{
	u8	anum;
	u16	i,pokenum,scrdata;

	pokenum = TaskTable[id].work[1];

	switch(TaskTable[id].work[0]){
	default:
	case 0:
 		//�t�F�[�h�҂�
		if(FadeData.fade_sw) break;

		Z_pVBlank_bak = sys.pVBlank;
		SetVBlankFunc(NULL);			//�����݊֐��ݒ�(VBLANK)

		ZukanScreenRegInit(DISP_BG0_ON);	//�\�����W�X�^������
		TaskTable[id].work[0] = 1;
 		break;
 	case 1:
		//�f�[�^�]��
		LZ77UnCompVram((void*)zkn_ch_LZ,(void*)(BG_VRAM+0x4000));	//BG CGX�]��
		LZ77UnCompVram((void*)zkn_scp_LZ,
					(void*)(BG_VRAM + (SAMPLE_BG3_SCRVRAM*0x800)));	//BG3 MAP�]��(�w�i)
		for(i=0;i<0x500/2;i++){
			scrdata = *(u16*)(BG_VRAM + (SAMPLE_BG3_SCRVRAM*0x800) + (i*2));//BG3 ATTR�ύX
			scrdata += 0x2000;	//�p���b�g�ʒu�Q���炷 
			*(u16*)(BG_VRAM + (SAMPLE_BG3_SCRVRAM*0x800) + (i*2)) = scrdata;
		}
		ZukanPokeFootTrans(TaskTable[id].work[1],SAMPLE_BG2_CHRVRAM,MODE1_FOOTCGX_NO);

		PaletteAnimeInit();
		PaletteWorkSet(&zukan1_Palette[0x0001],PA_BG2+PA_COL1,0xa0-2);//BG0&BG1&BG3 COL�]��
		TaskTable[id].work[0]++;
		break;
	case 2:
		//�e���[�e�B���e�B�[������
		PrintScreenInit(ZUKANLIST_TYPE);
		NMenuScreenInit(ZUKANLIST_TYPE);
		DIV_DMACLEAR(3,0,BG_VRAM + 0xc000,0x0200,16);	//����
		TaskTable[id].work[0]++;
		break;
 	case 3:
		//�f�[�^�\��
		NMenuMsgWrite(ZukanSampleIndex,3,0);

		if(!ZukanSpecialFlagCheck()){
			//�S���}�ӂ݂�Ȃ�
			ZukanPokeNumPut(PokeZukanNo2HouenNoGet(pokenum),13,3);	//�i���o�[
		}else{
			ZukanPokeNumPut(pokenum,13,3);			//�i���o�[
		}
		ZukanPokeNamePut(pokenum,17,3);		//���O
		NMenuMsgWrite(Z_Datainfo1str,13,5);		//�^�C�v�i�����ݒ�j
		NMenuMsgWrite(Z_Datainfo2str,16,7);		//�������i�����ݒ�j
		NMenuMsgWrite(Z_Datainfo3str,16,9);		//�������i�����ݒ�j

		ZukanDataTypeNamePut((u8*)zukanData[pokenum].typename,13,5);
		ZukanDataNumPut(zukanData[pokenum].takasa,16,7);
		ZukanDataNumPut(zukanData[pokenum].omosa,16,9);
		NMenuMsgWrite(zukanData[pokenum].text,3,13);
		ZukanPokeFootPut(SAMPLE_BG2_SCRVRAM,MODE1_FOOTCGX_NO);

		TaskTable[id].work[0]++;
		break;
	case 4:
		//�n�a�i�֘A�\�����t�F�[�h�ݒ�
		anum = ZukanPokeActorSet(pokenum,MODE1_POKE_POS_X,MODE1_POKE_POS_Y,0);
		ActWork[anum].oamData.Priority = 0;
		PaletteFadeReq(0xffffffff,0,16,0,0);
		SetVBlankFunc(Z_pVBlank_bak);	//�����݊֐��ݒ�(VBLANK)
		TaskTable[id].work[3] = anum;	//�A�N�^�[STRUCT�ۑ�
		TaskTable[id].work[0]++;
		break;
	case 5:
		//���W�X�^�ݒ�
		*(vu16 *)REG_BLDCNT   = 0;	//�������ݒ�
		*(vu16 *)REG_BLDALPHA = 0;
		*(vu16 *)REG_BLDY	  = 0;
		*(vu16 *)REG_BG3CNT = BG_COLOR_16					//16�F�I��
							| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
							| BG_PRIORITY_3					//BG�D�揇��
							| SAMPLE_BG3_SCRVRAM<<BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
							| SAMPLE_BG3_CHRVRAM<<BG_CHAR_BASE_SHIFT	//�L�����N�^�x�[�X�u���b�N
							;
		*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
							  |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
							  | DISP_BG3_ON					// BG3�L��
							  | DISP_BG2_ON					// BG2�L��
							  | DISP_OBJ_ON;
		TaskTable[id].work[0]++;
		break;
	case 6:
 		//�t�F�[�h�҂��������Đ�
		if(FadeData.fade_sw) break;

		VoicePlay(PokeZukanNo2MonsNoGet(pokenum),0);
		TaskTable[id].work[2] = 0;
		TaskTable[id].TaskAdrs = ZukanSampleKeyTask;
		break;
	}
}

/*==================================================================*/
static	void	ZukanSampleKeyTask(u8 id)
{
	//�`���a�{�^������
	if(sys.Trg & (A_BUTTON|B_BUTTON)){
		PaletteFadeReq(0x0000fffc,0,0,16,0);
		ActWork[TaskTable[id].work[3]].move = SamplePokeMove;
		TaskTable[id].TaskAdrs = ZukanSampleExitTask;
		return;
	}
	//�_��
	if((++TaskTable[id].work[2])&0x0010){
		PaletteWorkSet(&zukan1_Palette[0x00+0x01],PA_BG5+PA_COL1,0x10-2);
	}else{
		PaletteWorkSet(&zukan1_Palette[0x30+0x01],PA_BG5+PA_COL1,0x10-2);
	}
}

/*==================================================================*/
extern	void		FightWindowChrSet(void);

static	void	ZukanSampleExitTask(u8 id)
{
	u32	p_id,per_rnd;
	u16	pokenum;
	u8	pal;

 	if(FadeData.fade_sw)	return;

	*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
						  |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
						  | DISP_BG3_ON					// BG3�L��
						  | DISP_BG0_ON					// BG2�L��
						  | DISP_OBJ_ON;
 	pokenum	= PokeZukanNo2MonsNoGet(TaskTable[id].work[1]);
	p_id	= (((u16)TaskTable[id].work[13])<<16) | ((u16)TaskTable[id].work[12]);
	per_rnd = (((u16)TaskTable[id].work[15])<<16) | ((u16)TaskTable[id].work[14]);
	pal		= ActWork[TaskTable[id].work[3]].oamData.Pltt;
	DecordPaletteWork( (void*)PokePalGetPara(pokenum,p_id,per_rnd),PA_OBJ0+(pal*16),0x20 );

	DelTask(id);
	FightWindowChrSet();
}

/*==================================================================*/
static void		SamplePokeMove(actWork* aw)
{
	if(aw->x < 120)	aw->x+=2;
	if(aw->x > 120)	aw->x-=2;
	if(aw->y < 80)	aw->y++;
	if(aw->y > 80)	aw->y--;
}





