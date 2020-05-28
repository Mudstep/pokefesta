//=========================================================================
//	
//	svld_ini.��
//	�Z�[�u�֘A(�|�P�����`�f�a)
//
//=========================================================================
#include "common.h"
#include "calctool.h"
#include "../library/AgbFlash1M.h"

#define __SVLD_INI_H_GLOBAL__
#include "svld_ini.h"
#include "svld_sys.h"

#include "saveload.h"
#include "pasocom.h"
#include "record.h"
#include "task.h"

//=========================================================================
//  extern�ϐ�
//=========================================================================
//=========================================================================
//  extern�֐�
//=========================================================================
extern	void	BuckUpRAMcheckSet(u8 mode);	//�o�b�N�A�b�v���؃v���Z�X
extern	u32		svld_error_sect_bit;//�Z�N�^�ُ픻��t���O
//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//=========================================================================
//	��`
//=========================================================================
u16	SaveDataCheckInfo;
u16	UpDataCheckInfo;
pFunc SvRecoverReturnProc;		// �Z�N�^����������ɕ��A�����̊֐��|�C���^

u16	SaveTaskNo = {0};			//�ʐM�m�F�^�Z�[�u�^�X�N���g�p����ۂɎQ��

//=========================================================================
//	�f�[�^
//=========================================================================
//	�Z�[�u���[�h�u���b�N�z�u�e�[�u��
const	SVLDSYS_BLOCK_TABLE		PokeAGBsavedataAdr[]={
	/* �v���[���[�f�[�^ */
	{ (u8*)&MyData,		sizeof(MyData) },

	/* �t�B�[���h�f�[�^ */
	{ ((u8*)&Fld)+SVLD_BLOCK_SIZE*0,	SVLD_BLOCK_SIZE },
	{ ((u8*)&Fld)+SVLD_BLOCK_SIZE*1,	SVLD_BLOCK_SIZE },
	{ ((u8*)&Fld)+SVLD_BLOCK_SIZE*2,	SVLD_BLOCK_SIZE },
	{ ((u8*)&Fld)+SVLD_BLOCK_SIZE*3,	sizeof(FIELD_SAVE_DATA) % SVLD_BLOCK_SIZE },

	/* �{�b�N�X�f�[�^ */
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*0,	SVLD_BLOCK_SIZE },
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*1,	SVLD_BLOCK_SIZE },
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*2,	SVLD_BLOCK_SIZE },
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*3,	SVLD_BLOCK_SIZE },
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*4,	SVLD_BLOCK_SIZE },
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*5,	SVLD_BLOCK_SIZE },
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*6,	SVLD_BLOCK_SIZE },
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*7,	SVLD_BLOCK_SIZE },
	{ ((u8*)&BoxData)+SVLD_BLOCK_SIZE*8,	sizeof(BoxData) % SVLD_BLOCK_SIZE },
};

//	�a������؂̎��u���b�N�z�u�e�[�u��
const	SVLDSYS_BLOCK_TABLE		PokeAGBexdataAdr[]={
	{ (u8*)(&(UserWork[0x01e000])),					SVLD_BLOCK_SIZE },
	{ (u8*)(&(UserWork[0x01e000+SVLD_BLOCK_SIZE])),	SVLD_BLOCK_SIZE },
};

//	�A�b�v���[�h�ݒ�Z�N�^�e�[�u��
const	u8		PokeAGBUploadSectorAdr[]={
	0x1e,0x1f,
};

//=========================================================================
//				�Z�[�u�֘A�ifor �|�P�����`�f�a�j
//=========================================================================
u8		PokeAGBSaveMain(u8 mode)
{
	u8	i;

	switch(mode){

	case	POKEAGB_SVCLRDENDOU://�a���f�[�^���i�Œ�G���A�j�N���A���a���Z�[�u
		for(i=0x1c;i<0x20;i++)	EraseFlashSector(i);
	case	POKEAGB_SVDENDOU://�a���f�[�^�Z�[�u
		if(GetRecord( CNT_DENDOU ) < 999)	IncRecord( CNT_DENDOU ); //�a���J�E���^����

		for(i=0;i<2;i++)
			ExtraDataSave(0x1c+i,PokeAGBexdataAdr[i].dat_adr,PokeAGBexdataAdr[i].size);
	default:
	case	POKEAGB_SVALL://�S�̃Z�[�u
		BackUpGameData();
		AGBSaveSys(SVLD_MODE_ALL,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		break;
	case	POKEAGB_SVFLD://�Q�[���i�s�f�[�^�Z�[�u
		BackUpGameData();
		for(i=0;i<5;i++) AGBSaveSys(i,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		break;
	case	POKEAGB_SVMYD://�v���[���[�f�[�^�Z�[�u
		BackUpGameData();
		AGBSaveSys(0,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		break;
	case	POKEAGB_SVCLRALL://�a���f�[�^���i�Œ�G���A�j�N���A���S�̃Z�[�u
		for(i=0x1c;i<0x20;i++)	EraseFlashSector(i);
		BackUpGameData();
		AGBSaveSys(SVLD_MODE_ALL,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		break;
	}
	return 0;
}

//=========================================================================
u8		PokeAGBSave(u8 mode)
{
	//�t���b�V�����F������Ă��Ȃ��ꍇ�̓G���[
	if(SaveMemType != FLASH_1M_TYPE)	return SVLD_RET_NG;

	//�Z�[�u���s
	PokeAGBSaveMain(mode);
	if(!svld_error_sect_bit)	return SVLD_RET_OK;//�Z�N�^�ُ�Ȃ�

	//�Z�N�^���؁i���،�̓��Z�b�g�����j
	BuckUpRAMcheckSet(mode);	//�o�b�N�A�b�v���؃v���Z�X�Z�b�g

	return SVLD_RET_NG;
}


//=========================================================================
//				�ʐM�m�F�^�Z�[�u�֘A�i�����S�̃Z�[�u�j�ifor �|�P�����`�f�a�j
//=========================================================================
u8		PokeAGBSaveSioDivInit(void)
{
	//�t���b�V�����F������Ă��Ȃ��ꍇ�̓G���[
	if(SaveMemType != FLASH_1M_TYPE)	return 1;

	BackUpGameData();
	AGBSaveSysSioInit((SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
	return 0;
}

u8		PokeAGBSaveSioDiv(void)	//�I��(1)���쒆(0)
{
	u8	ret_flag;

	//�Z�[�u���s
	ret_flag = AGBSaveSysSioDiv(SVLD_BLOCK_COUNT,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
	
	if(svld_error_sect_bit){
		//�Z�N�^���؁i���،�̓��Z�b�g�����j
		BuckUpRAMcheckSet(POKEAGB_SVALL);	//�o�b�N�A�b�v���؃v���Z�X�Z�b�g
	}
	if(ret_flag == SVLD_RET_NG)	return 1;
	else						return 0;
}

u8		PokeAGBSaveSioDiv1(void)
{
	AGBSaveSysSioByte1(SVLD_BLOCK_COUNT,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		
	if(svld_error_sect_bit){
		//�Z�N�^���؁i���،�̓��Z�b�g�����j
		BuckUpRAMcheckSet(POKEAGB_SVALL);	//�o�b�N�A�b�v���؃v���Z�X�Z�b�g
	}
	return 0;	//���ɈӖ��Ȃ�
}

u8		PokeAGBSaveSioDiv2(void)
{
	AGBSaveSysSioByte2(SVLD_BLOCK_COUNT,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		
	if(svld_error_sect_bit){
		//�Z�N�^���؁i���،�̓��Z�b�g�����j
		BuckUpRAMcheckSet(POKEAGB_SVALL);	//�o�b�N�A�b�v���؃v���Z�X�Z�b�g
	}
	return 0;	//���ɈӖ��Ȃ�
}

//=========================================================================
//				�Z�[�u�֘A�i�����t�B�[���h�Z�[�u�j�ifor �|�P�����`�f�a�j
extern	u16	svld_div_sector_no;	//�����J�E���^�[
#define		FIELD_BLOCK_COUNT	(5)
//=========================================================================
u8		PokeAGBSaveFieldDivInit(void)
{
	//�t���b�V�����F������Ă��Ȃ��ꍇ�̓G���[
	if(SaveMemType != FLASH_1M_TYPE)	return 1;

	BackUpGameData();
	AGBSaveSysSioInitNoMirror((SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);

	AGBSaveSysSioByte1(svld_div_sector_no+1,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
	return 0;
}

u8		PokeAGBSaveFieldDiv(void)	//�I��(1)���쒆(0)
{
	u16	sect_b;
	u8	ret_flag;

	ret_flag = 0;

	sect_b = svld_div_sector_no;
	svld_div_sector_no++;	//�����J�E���^�[

	if(svld_div_sector_no < FIELD_BLOCK_COUNT){
		AGBSaveSysSioByte1(svld_div_sector_no+1,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		AGBSaveSysSioByte2Ex(sect_b+1,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		ret_flag = 0;
	}else{
		AGBSaveSysSioByte2Ex(sect_b+1,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		ret_flag = 1;
	}
	if(svld_error_sect_bit){
		//�Z�N�^���؁i���،�̓��Z�b�g�����j
		BuckUpRAMcheckSet(POKEAGB_SVFLD);	//�o�b�N�A�b�v���؃v���Z�X�Z�b�g
	}
	return ret_flag;
}


















//=========================================================================
//				���[�h�֘A�ifor �|�P�����`�f�a�j
//=========================================================================
u8		PokeAGBLoad(u8 mode)
{
	u8	ret_flg;

	ret_flg = SVLD_RET_NG;
	if(SaveMemType != FLASH_1M_TYPE){
		SaveDataCheckInfo = SVLD_RET_HARDERROR;
		return	ret_flg;
	}

	switch(mode){

	default:
	case	POKEAGB_LDALL://�S�̃��[�h
		ret_flg = AGBLoadSys(SVLD_MODE_ALL,(SVLDSYS_BLOCK_TABLE*)PokeAGBsavedataAdr);
		RecoverGameData();
		SaveDataCheckInfo = ret_flg;
		SvRecoverReturnProc = NULL;		// �Z�N�^����������ɕ��A�����̊֐��|�C���^
		break;
	case	POKEAGB_LDDENDOU://�a���f�[�^���[�h
		ret_flg = ExtraDataLoad(0x1c,PokeAGBexdataAdr[0].dat_adr,PokeAGBexdataAdr[0].size);
		if(ret_flg != SVLD_RET_OK)	break;
		ret_flg = ExtraDataLoad(0x1d,PokeAGBexdataAdr[1].dat_adr,PokeAGBexdataAdr[1].size);
	}
	return	ret_flg;
}





//*************************************************************************
//=========================================================================
//				�O���f�[�^���[�h����
//=========================================================================
typedef struct {

u8		data[SVLD_DATA_SIZE];	//�Z�[�u�f�[�^
u32		save_id;	//�Z�[�u���ʃf�[�^

}UPLOAD_DATA;
//*************************************************************************
u8		UpLoadDataGet(u8* upld_p)
{
	u16	i;
	UPLOAD_DATA	*upld;

	upld = (UPLOAD_DATA*)upld_p;

	for(i=0;i<SVLD_SECTOR_SIZE;i++) upld_p[i] = 0;
	ReadFlash(PokeAGBUploadSectorAdr[0],0,(u8*)upld,SVLD_SECTOR_SIZE);
	if(upld->save_id != SVLD_ID)	return	SVLD_RET_UPD_NULL;
	return	SVLD_RET_UPD_OK;
}

u8		UpLoadDataSet(void)
{
	u16	i;
	u8	ret_flg;

	ret_flg = PokeAGBSave(POKEAGB_SVALL);
	for(i=0;i<2;i++){
		EraseFlashSector(PokeAGBUploadSectorAdr[i]);
	}
	if(ret_flg == SVLD_RET_NG)		return	SVLD_RET_UPD_NG;
	if(ret_flg == SVLD_RET_FULL)	return	SVLD_RET_UPD_FULL;

	PokeAGBLoad(POKEAGB_SVALL);
	return	SVLD_RET_UPD_OK;
}


//-------------------------------------------------------------------------
//			�O���f�[�^�p�Z�[�u���C���i���j
//-------------------------------------------------------------------------
u32 upload_sv_flag = 0;

u8		UpLoadDataSave(u8* dat_adr,u16 size)
{
	u16	i;
	UPLOAD_DATA	*upld;

	upld = (UPLOAD_DATA*)(UserWork + 0x00000000);	
	//�Z�[�u�f�[�^�̈�N���A
	for(i=0;i<SVLD_SECTOR_SIZE;i++) ((u8*)upld)[i] = 0;
	//�Z�[�u���ʃf�[�^�ݒ�
	upld->save_id	= SVLD_ID;
	//�Z�[�u�f�[�^�R�s�[
	for(i=0;i<size;i++) upld->data[i] = dat_adr[i];

	//�Z�N�^�Z�[�u������
	upload_sv_flag = ProgramFlashSectorEx2(
			PokeAGBUploadSectorAdr[0],(u8*)upld,SVLD_SECTOR_SIZE);
	if(upload_sv_flag){
		return	SVLD_RET_NG;
	}else{
		return	SVLD_RET_OK;
	}
}

//-------------------------------------------------------------------------
//			�J�[�h�d�p�Z�[�u���C���i���j
//-------------------------------------------------------------------------
u8		SaveExtraSector(u8 sector_num, u8 *p)
{
	if(ProgramFlashSectorEx(sector_num,p)){
		return	SVLD_RET_NG;
	}else{
		return	SVLD_RET_OK;
	}
}

//-------------------------------------------------------------------------
//			�J�[�h�d�p���[�h���C���i���j
//-------------------------------------------------------------------------
u8		LoadExtraSector(u8 sector_num, u8 *p)
{
	ReadFlash(sector_num,0,(u8 *)p,SVLD_SECTOR_SIZE);
	return	SVLD_RET_OK;
}






