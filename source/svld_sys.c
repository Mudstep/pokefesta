//=========================================================================
//	
//	svld_sys.��
//	�Z�[�u�֘A�Z�N�^���[�e�[�V�����\��
//
//=========================================================================
#include "common.h"
#include "intr.h"
#include "saveload.h"
#include "intr.h"
#include "../library/AgbFlash1M.h"

#include "svld_ini.h"
#define __SVLD_SYS_H_GLOBAL__
#include "svld_sys.h"





//=========================================================================
//  extern�ϐ�
//=========================================================================
//=========================================================================
//  extern�֐�
//=========================================================================
//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
static	u16		SumCheckCalc(u32* dat_adr,u16 size);
static	u8		SaveSector(u8 sector_num,u8* data);
static	u8		LoadSector(u8 sector_num,u8* data);
static	u16		LoadStatusCheck(SVLDSYS_BLOCK_TABLE* svldadr_tbl);

//=========================================================================
//	��`
//=========================================================================
SVLDSYS_WORK*	svld;						//�e�v���W�F�N�g�Ń��[�N�G���A���Ǘ�����
//�����J�E���^�[
u32	svld_main_counter;
//�������݃X�^�[�g�ʒu
u16	svld_sector_start;
//�Z�N�^�ُ픻��t���O
u32	svld_error_sect_bit;
//�����J�E���^�[�o�b�N�A�b�v
u32	svld_main_counter_backup;
//�������݃X�^�[�g�ʒu�o�b�N�A�b�v
u16	svld_sector_start_backup;
//�����Z�[�u�p�J�E���^�[
u16	svld_div_sector_no;

//*************************************************************************
//=========================================================================
//				�e�k�`�r�g�Q�q�`�l������
//=========================================================================
//*************************************************************************
void	EraseBackUpData(void)
{
	u16	i;

	for(i=0;i<32;i++) EraseFlashSector(i);
}

//*************************************************************************
//=========================================================================
//				��{�������i�Q�[���X�^�[�g���A�f�[�^���[�h�O�j
//=========================================================================
//*************************************************************************
void	SaveLoadSysInit(void)
{
	svld_main_counter	= 0;
	svld_sector_start	= 0;
	svld_error_sect_bit	= 0;
}

//*************************************************************************
//=========================================================================
//				�Z�N�^�ُ픻��r�b�g����
//=========================================================================
//*************************************************************************
u8		SaveLoadSysSectFlagCont(u8 mode,u8 sector_num)
{
	u8	ret_flag;

	ret_flag = 0;

	switch(mode){
	case SVLD_SECT_FSET:
		svld_error_sect_bit	|= (0x00000001 << sector_num);
		break;
	case SVLD_SECT_FCLR:
		svld_error_sect_bit	&= ((0x00000001 << sector_num)^0xffffffff);
		break;
	case SVLD_SECT_FCHK:
		if(svld_error_sect_bit & (0x00000001 << sector_num)){
			ret_flag = 1;
		}
		break;
	}
	return ret_flag;
}

//*************************************************************************
//=========================================================================
//				���C��
//=========================================================================
//*************************************************************************
//*************************************************************************
//=========================================================================
//				�Z�[�u
//=========================================================================
//*************************************************************************
u8		AGBSaveSys(u16 mode,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u16	i;
	u8	ret_flg;

	//�V�X�e�����[�N�|�C���^�ݒ�
	svld = (SVLDSYS_WORK*)(UserWork + 0x00000000);	

	switch(mode){

	default:
		//�e�u���b�N�P�̂̃Z�[�u�i�Z�N�^���~���[�ړ��Ȃ��jmode = �u���b�N�i���o�[
		ret_flg = SaveLoadSysSet(mode,svldadr_tbl);
		break;
	case	SVLD_MODE_ALL:
		//�S�u���b�N�̃Z�[�u�i�Z�N�^���~���[�ړ��j
		svld_sector_start_backup = svld_sector_start;	//�o�b�N�A�b�v
		svld_main_counter_backup = svld_main_counter;	//�o�b�N�A�b�v

		svld_sector_start++;
		svld_sector_start %= SVLD_MIRROR_SIZE;	//�Z�N�^�ړ�
		svld_main_counter++;				//�~���[�ړ�

		ret_flg = SVLD_RET_OK;

		for(i=0;i<SVLD_BLOCK_COUNT;i++){
			SaveLoadSysSet(i,svldadr_tbl);
		}
		if(svld_error_sect_bit){
			ret_flg = SVLD_RET_NG;
			svld_sector_start = svld_sector_start_backup;	//�o�b�N�A�b�v�߂�
			svld_main_counter = svld_main_counter_backup;	//�o�b�N�A�b�v�߂�
		}
		break;
	}
	return	ret_flg;
}

//-------------------------------------------------------------------------
//				�Z�[�u�Z�N�^�ݒ聕�Z�[�u���s
//-------------------------------------------------------------------------
u8		SaveLoadSysSet(u16 sv_blocknum,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u16	i,p,sumcheck,size;
	u8*	svdat_adr;

	//�������݃Z�N�^�ݒ�
	p = svld_sector_start + sv_blocknum;
	p %= SVLD_MIRROR_SIZE;
	p += (svld_main_counter & 0x00000001) * SVLD_MIRROR_SIZE;

	svdat_adr	= svldadr_tbl[sv_blocknum].dat_adr;
	size		= svldadr_tbl[sv_blocknum].size;

	//�Z�[�u�f�[�^�̈�N���A
	for(i=0;i<SVLD_SECTOR_SIZE;i++) ((u8*)svld)[i] = 0;
	//�u���b�N�i���o�[�ݒ�
	svld->svdata.blocknum	= sv_blocknum;	
	//�Z�[�u���ʃf�[�^�ݒ�
	svld->svdata.save_id	= SVLD_ID;
	//�Z�[�u�J�E���^�[�i�����j�X�V
	svld->svdata.save_count = svld_main_counter;
	//�Z�[�u�f�[�^�R�s�[
	for(i=0;i<size;i++) svld->svdata.data[i] = svdat_adr[i];
	//�T���`�F�b�N�ݒ�
	sumcheck = SumCheckCalc((u32*)svdat_adr,size);
	svld->svdata.sumcheck	= sumcheck;

	//�Z�N�^�Z�[�u������
	return	SaveSector(p,(u8*)(&svld->svdata));
}

//-------------------------------------------------------------------------
//			�Z�N�^�w��Z�[�u
//-------------------------------------------------------------------------
u8		ExtraDataSave(u8 sectnum,u8* dat_adr,u16 size)
{
	u16	i,sumcheck;
	EXTRA_DATA	*ex_svld;

	ex_svld = (EXTRA_DATA*)(UserWork + 0x00000000);	
	//�Z�[�u�f�[�^�̈�N���A
	for(i=0;i<SVLD_SECTOR_SIZE;i++) ((u8*)ex_svld)[i] = 0;
	//�Z�[�u���ʃf�[�^�ݒ�
	ex_svld->save_id	= SVLD_ID;
	//�Z�[�u�f�[�^�R�s�[
	for(i=0;i<size;i++) ex_svld->data[i] = dat_adr[i];
	//�T���`�F�b�N�ݒ�
	sumcheck = SumCheckCalc((u32*)dat_adr,size);
	ex_svld->sumcheck	= sumcheck;

	//�Z�N�^�Z�[�u������
	return	SaveSector(sectnum,(u8*)ex_svld);
}

//-------------------------------------------------------------------------
//				�Z�[�u���C��
//-------------------------------------------------------------------------
static	u8		SaveSector(u8 sector_num,u8* data)
{
	if(ProgramFlashSectorEx(sector_num,data)){
		SaveLoadSysSectFlagCont(SVLD_SECT_FSET,sector_num);
		return	SVLD_RET_NG;
	}else{
		SaveLoadSysSectFlagCont(SVLD_SECT_FCLR,sector_num);
		return	SVLD_RET_OK;
	}
}










//*************************************************************************
//=========================================================================
//				�ʐM�����������Z�[�u�i�S�̂̂݁j
//=========================================================================
u8		SaveLoadSysSetByte(u16 sv_blocknum,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
//*************************************************************************
u8		AGBSaveSysSioInit(SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	//�V�X�e�����[�N�|�C���^�ݒ�
	svld = (SVLDSYS_WORK*)(UserWork + 0x00000000);	

	//�S�u���b�N�̃Z�[�u�i�Z�N�^���~���[�ړ��j
	svld_sector_start_backup = svld_sector_start;	//�o�b�N�A�b�v
	svld_main_counter_backup = svld_main_counter;	//�o�b�N�A�b�v

	svld_sector_start++;
	svld_sector_start %= SVLD_MIRROR_SIZE;	//�Z�N�^�ړ�
	svld_main_counter++;				//�~���[�ړ�

	svld_div_sector_no = 0;
	svld_error_sect_bit= 0;
	return	0;
}

u8		AGBSaveSysSioInitNoMirror(SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	//�V�X�e�����[�N�|�C���^�ݒ�
	svld = (SVLDSYS_WORK*)(UserWork + 0x00000000);	

	svld_sector_start_backup = svld_sector_start;	//�o�b�N�A�b�v
	svld_main_counter_backup = svld_main_counter;	//�o�b�N�A�b�v
	svld_div_sector_no = 0;
	svld_error_sect_bit= 0;
	return	0;
}

//-------------------------------------------------------------------------
//				�����Z�[�u���s
//-------------------------------------------------------------------------
u8		AGBSaveSysSioDiv(u16 sect_max,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u8	ret_flg;

//	if(svld_div_sector_no < (SVLD_BLOCK_COUNT-1)){
	if(svld_div_sector_no < (sect_max-1)){
		ret_flg = SVLD_RET_OK;

		SaveLoadSysSet(svld_div_sector_no,svldadr_tbl);
		svld_div_sector_no++;
	
		if(svld_error_sect_bit){
			ret_flg = SVLD_RET_NG;	//���f
			svld_sector_start = svld_sector_start_backup;	//�o�b�N�A�b�v�߂�
			svld_main_counter = svld_main_counter_backup;	//�o�b�N�A�b�v�߂�
		}
	}else{
		ret_flg = SVLD_RET_NG;	//�I��
	}
	return	ret_flg;
}

//-------------------------------------------------------------------------
//		�ŏI�Z�N�^�O��	
//-------------------------------------------------------------------------
u8		AGBSaveSysSioByte1(u16 sect_max,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u8	ret_flg;

	ret_flg = SVLD_RET_OK;
//	SaveLoadSysSetByte(SVLD_BLOCK_COUNT-1,svldadr_tbl);	//�Ō�̂P�Z�N�^�̃Z�[�u
	SaveLoadSysSetByte(sect_max-1,svldadr_tbl);	//�Ō�̂P�Z�N�^�̃Z�[�u
	if(svld_error_sect_bit){
		ret_flg = SVLD_RET_NG;
		svld_sector_start = svld_sector_start_backup;	//�o�b�N�A�b�v�߂�
		svld_main_counter = svld_main_counter_backup;	//�o�b�N�A�b�v�߂�
	}
	return	ret_flg;
}

//-------------------------------------------------------------------------
//				�Z�[�u�Z�N�^�ݒ聕�Z�[�u���s�i�h�c���������o�C�g�P�ʏ������݁j
//-------------------------------------------------------------------------
u8		SaveLoadSysSetByte(u16 sv_blocknum,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u16	i,p,sumcheck,size,f;
	u8*	svdat_adr;

	//�������݃Z�N�^�ݒ�
	p = svld_sector_start + sv_blocknum;
	p %= SVLD_MIRROR_SIZE;
	p += (svld_main_counter & 0x00000001) * SVLD_MIRROR_SIZE;

	svdat_adr	= svldadr_tbl[sv_blocknum].dat_adr;
	size		= svldadr_tbl[sv_blocknum].size;

	//�Z�[�u�f�[�^�̈�N���A
	for(i=0;i<SVLD_SECTOR_SIZE;i++) ((u8*)svld)[i] = 0;
	//�u���b�N�i���o�[�ݒ�
	svld->svdata.blocknum	= sv_blocknum;	
	//�Z�[�u���ʃf�[�^�ݒ�
	svld->svdata.save_id	= SVLD_ID;
	//�Z�[�u�J�E���^�[�i�����j�X�V
	svld->svdata.save_count = svld_main_counter;
	//�Z�[�u�f�[�^�R�s�[
	for(i=0;i<size;i++) svld->svdata.data[i] = svdat_adr[i];
	//�T���`�F�b�N�ݒ�
	sumcheck = SumCheckCalc((u32*)svdat_adr,size);
	svld->svdata.sumcheck	= sumcheck;

	//�Z�N�^����
	EraseFlashSector(p);

	//�Z�N�^�������ݑO��
	f = SVLD_RET_OK;
	for(i=0;i<SVLD_BYTE_WSIZE1;i++){
		if(ProgramFlashByte(p,i,((u8*)svld)[i])){
			f = SVLD_RET_NG;
			break;
		}
	}
	if(f == SVLD_RET_NG){
		SaveLoadSysSectFlagCont(SVLD_SECT_FSET,p);
		return	SVLD_RET_NG;
	}

	//�Z�N�^�������݌㔼
	f = SVLD_RET_OK;
	for(i=0;i<SVLD_BYTE_WSIZE2;i++){
		if(ProgramFlashByte(p,(i+SVLD_BYTE_WSIZE1+1),((u8*)svld)[i+SVLD_BYTE_WSIZE1+1])){
			f = SVLD_RET_NG;
			break;
		}
	}
	if(f == SVLD_RET_NG){
		SaveLoadSysSectFlagCont(SVLD_SECT_FSET,p);
		return	SVLD_RET_NG;
	}else{
		SaveLoadSysSectFlagCont(SVLD_SECT_FCLR,p);
		return	SVLD_RET_OK;
	}
}

//-------------------------------------------------------------------------
//		�ŏI�Z�N�^�㔼	
//-------------------------------------------------------------------------
u8		AGBSaveSysSioByte2(u16 sect_max,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u16	p;
	
	//�������݃Z�N�^�ݒ�
//	p = svld_sector_start + SVLD_BLOCK_COUNT-1;
	p = svld_sector_start + sect_max-1;
	p %= SVLD_MIRROR_SIZE;
	p += (svld_main_counter & 0x00000001) * SVLD_MIRROR_SIZE;

	//�Z�N�^�������݂h�c�i1byte�������j
	if(ProgramFlashByte(p,SVLD_BYTE_WSIZE1,((u8*)svld)[SVLD_BYTE_WSIZE1])){
		SaveLoadSysSectFlagCont(SVLD_SECT_FSET,p);
		svld_sector_start = svld_sector_start_backup;	//�o�b�N�A�b�v�߂�
		svld_main_counter = svld_main_counter_backup;	//�o�b�N�A�b�v�߂�
		return	SVLD_RET_NG;
	}else{
		SaveLoadSysSectFlagCont(SVLD_SECT_FCLR,p);
		return	SVLD_RET_OK;
	}
}

//-------------------------------------------------------------------------
//		�ŏI�Z�N�^�㔼(�F�؂h�c���߂�����)	
//-------------------------------------------------------------------------
u8		AGBSaveSysSioByte2Ex(u16 sect_max,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u16	p;
	
	//�������݃Z�N�^�ݒ�
	p = svld_sector_start + sect_max-1;
	p %= SVLD_MIRROR_SIZE;
	p += (svld_main_counter & 0x00000001) * SVLD_MIRROR_SIZE;

	//�Z�N�^�������݂h�c�i1byte�������j
	if(ProgramFlashByte(p,SVLD_BYTE_WSIZE1,(u8)(SVLD_ID & 0xff))){
		SaveLoadSysSectFlagCont(SVLD_SECT_FSET,p);
		svld_sector_start = svld_sector_start_backup;	//�o�b�N�A�b�v�߂�
		svld_main_counter = svld_main_counter_backup;	//�o�b�N�A�b�v�߂�
		return	SVLD_RET_NG;
	}else{
		SaveLoadSysSectFlagCont(SVLD_SECT_FCLR,p);
		return	SVLD_RET_OK;
	}
}






















//*************************************************************************
//=========================================================================
//				���[�h
//=========================================================================
//*************************************************************************
u8		AGBLoadSys(u16 mode,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u8	ret_flag;

	//�V�X�e�����[�N�|�C���^�ݒ�
	svld = (SVLDSYS_WORK*)(UserWork + 0x00000000);	

	switch(mode){

	default:
		//�e�u���b�N�P�̂̃��[�h mode = �u���b�N�i���o�[
//		ret_flag = SaveLoadSysGet(mode,svldadr_tbl);
		ret_flag = SVLD_RET_NG;
		break;
	case	SVLD_MODE_ALL:
		//�ŐV�f�[�^���ʁ������J�E���^�擾
		ret_flag = LoadStatusCheck(svldadr_tbl);
		if((ret_flag == SVLD_RET_NULL)&&(ret_flag == SVLD_RET_DESTROY)){
			//�Z�[�u�f�[�^�����݂��Ă��Ȃ��A�܂��͕��A�s�\�Ȃ̂ŏ��������ďI��
			SaveLoadSysInit();
			break;
		}
		//�S�u���b�N�̃��[�h
		SaveLoadSysGet(mode,svldadr_tbl);
		break;
	}
	return	ret_flag;
}

//-------------------------------------------------------------------------
//				���[�h���s
//-------------------------------------------------------------------------
u8		SaveLoadSysGet(u16 ld_blocknum,SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u16	i,j,p,blocknum,sumcheck;

	//�ǂݍ��݃Z�N�^�ݒ�
	p = (svld_main_counter & 0x00000001) * SVLD_MIRROR_SIZE;

	for(i=0;i<SVLD_MIRROR_SIZE;i++){
		LoadSector(i+p,(u8 *)(&svld->svdata));

		//�������݃X�^�[�g�ʒu�ݒ�i����Z�[�u�p�j
		if(!(blocknum = svld->svdata.blocknum))	svld_sector_start = i;

		//�h�c�m�F���T���`�F�b�N�m�F
		sumcheck = SumCheckCalc((u32*)svld->svdata.data,svldadr_tbl[blocknum].size);
		if((svld->svdata.save_id == SVLD_ID)&&(svld->svdata.sumcheck == sumcheck)){
			//���[�h�f�[�^�z�u
			for(j=0;j<(svldadr_tbl[blocknum].size);j++){
				svldadr_tbl[blocknum].dat_adr[j] = svld->svdata.data[j];
			}
		}
	}
	return	SVLD_RET_OK;	//���[�h���튮��
}

//-------------------------------------------------------------------------
//				�ŐV�f�[�^���ʁ������J�E���^�擾�������`�F�b�N
//-------------------------------------------------------------------------
static	u16		LoadStatusCheck(SVLDSYS_BLOCK_TABLE* svldadr_tbl)
{
	u16	i,f1,f2,sumcheck,null_f;
	u32	counter1,counter2;
	u32	block_flag;

	counter1 = 0;
	counter2 = 0;

	//�~���[�P��Ԏ擾
	block_flag = 0;
	null_f = 0;
	for(i=0;i<SVLD_MIRROR_SIZE;i++){
		LoadSector(i,(u8 *)(&svld->svdata));
		//�h�c�m�F
		if(svld->svdata.save_id == SVLD_ID){
			null_f = 1;
			//�T���`�F�b�N�m�F
			sumcheck = 
				SumCheckCalc((u32*)svld->svdata.data,svldadr_tbl[svld->svdata.blocknum].size);
			if(svld->svdata.sumcheck == sumcheck){
				counter1 = svld->svdata.save_count;
				block_flag |= (0x00000001 << svld->svdata.blocknum);	//�u���b�N�⊮���
			}
		}
	}
	//�~���[�P�⊮���m�F
	if(null_f){
		if(block_flag == ((0xffffffff << SVLD_BLOCK_COUNT)^0xffffffff)){
			f1 = SVLD_RET_OK;
		}else{
			f1 = SVLD_RET_NG;
		}
	}else{
		f1 = SVLD_RET_NULL;
	}

	//�~���[�Q��Ԏ擾
	block_flag = 0;
	null_f = 0;
	for(i=0;i<SVLD_MIRROR_SIZE;i++){
		LoadSector(i+SVLD_MIRROR_SIZE,(u8 *)(&svld->svdata));
		//�h�c�m�F
		if(svld->svdata.save_id == SVLD_ID){
			null_f = 1;
			//�T���`�F�b�N�m�F
			sumcheck = 
				SumCheckCalc((u32*)svld->svdata.data,svldadr_tbl[svld->svdata.blocknum].size);
			if(svld->svdata.sumcheck == sumcheck){
				counter2 = svld->svdata.save_count;
				block_flag |= (0x00000001 << svld->svdata.blocknum);	//�u���b�N�⊮���
			}
		}
	}
	//�~���[�Q�⊮���m�F
	if(null_f){
		if(block_flag == ((0xffffffff << SVLD_BLOCK_COUNT)^0xffffffff)){
			f2 = SVLD_RET_OK;
		}else{
			f2 = SVLD_RET_NG;
		}
	}else{
		f2 = SVLD_RET_NULL;
	}

	//��Ԕ���
	if((f1 == SVLD_RET_OK)&&(f2 == SVLD_RET_OK)){
		if(((counter1 == 0xffffffff)&&(counter2 == 0x00000000))
			||((counter1 == 0x00000000)&&(counter2 == 0xffffffff))){
			//�����J���X�g�����i�Ȃ��Ǝv�����O�̂��߁j
			if((counter1+1) < (counter2+1)){
				svld_main_counter	= counter2;
			}else{
				svld_main_counter	= counter1;
			}
		}else{
			if(counter1 < counter2){
				svld_main_counter	= counter2;
			}else{
				svld_main_counter	= counter1;
			}
		}
		return	SVLD_RET_OK;//�~���[�P�F�����A�~���[�Q�F����
	}
	if((f1 == SVLD_RET_OK)&&(f2 != SVLD_RET_OK)){
		svld_main_counter	= counter1;
		if(f2 == SVLD_RET_NG){
			return	SVLD_RET_NG;//�~���[�P�F�����A�~���[�Q�F���S�i�x���j
		}else{
			return	SVLD_RET_OK;//�~���[�P�F�����A�~���[�Q�F�����i���肦�Ȃ����ꉞ�j
		}
	}
	if((f2 == SVLD_RET_OK)&&(f1 != SVLD_RET_OK)){
		svld_main_counter	= counter2;
		if(f1 == SVLD_RET_NG){
			return	SVLD_RET_NG;//�~���[�P�F���S�A�~���[�Q�F�����i�x���j
		}else{
			return	SVLD_RET_OK;//�~���[�P�F�����A�~���[�Q�F�����i�ŏ��͂���j
		}
	}
	if((f1 == SVLD_RET_NULL)&&(f2 == SVLD_RET_NULL)){
		svld_main_counter	= 0;
		svld_sector_start	= 0;
		return	SVLD_RET_NULL;//�~���[�P�F�����A�~���[�Q�F����
	}
	//���̑��i���肦�Ȃ����O�̂��߁j
	//�~���[�P�F���S�A�~���[�Q�F���S
	//�~���[�P�F�����A�~���[�Q�F���S
	//�~���[�P�F���S�A�~���[�Q�F�����i�����s�ǂł��邩���H�j
	svld_main_counter	= 0;
	svld_sector_start	= 0;
	return	SVLD_RET_DESTROY;
}

//-------------------------------------------------------------------------
//			�Z�N�^�w�胍�[�h
//-------------------------------------------------------------------------
u8		ExtraDataLoad(u8 sectnum,u8* dat_adr,u16 size)
{
	u16	i,sumcheck;
	EXTRA_DATA	*ex_svld;

	ex_svld = (EXTRA_DATA*)(UserWork + 0x00000000);	
	LoadSector(sectnum,(u8 *)ex_svld);

	//�h�c�m�F
	if(ex_svld->save_id == SVLD_ID){
		//�T���`�F�b�N�m�F
		sumcheck = SumCheckCalc((u32*)ex_svld->data,size);
		if(ex_svld->sumcheck == sumcheck){
			//���[�h�f�[�^�z�u
			for(i=0;i<size;i++){
				dat_adr[i] = ex_svld->data[i];
			}
			return	SVLD_RET_OK;	//����I��
		}
		return	SVLD_RET_DESTROY;	//�T���`�F�b�N�G���[�i���A�s�j
	}
	return	SVLD_RET_NULL;			//�h�c�G���[�i�f�[�^�Ȃ��j
}

//-------------------------------------------------------------------------
//				���[�h���C��
//-------------------------------------------------------------------------
static	u8		LoadSector(u8 sector_num,u8* data)
{
	ReadFlash(sector_num,0,data,SVLD_SECTOR_SIZE);
	return	SVLD_RET_OK;
}





//*************************************************************************
//=========================================================================
//				�T�u���[�`��
//=========================================================================
//*************************************************************************
static	u16		SumCheckCalc(u32* dat_adr,u16 size)
{
	u16	i;
	u32	sumcheck;

	sumcheck = 0;
	for(i=0;i<(size/4);i++){
		sumcheck += *dat_adr;
		dat_adr++;
	}
	return	(u16)(((sumcheck >> 16) & 0xffff) + (sumcheck & 0xffff));
}





#ifdef PM_DEBUG
extern	const	SVLDSYS_BLOCK_TABLE		PokeAGBsavedataAdr[];
extern	const	SVLDSYS_BLOCK_TABLE		PokeAGBexdataAdr[];
//*************************************************************************
//=========================================================================
//				���[�h�f�[�^�R���o�[�g
//=========================================================================
//*************************************************************************
void	LoadDataConv(void)
{
	u16	i,sumcheck;
	SVLDSYS_WORK*	Dsvld1;
	EXTRA_DATA*		Dsvld2;
	const	SVLDSYS_BLOCK_TABLE* Dsvld1Tbl;
	const	SVLDSYS_BLOCK_TABLE* Dsvld2Tbl;

	Dsvld1		= (SVLDSYS_WORK*)(UserWork + 0x00000000);	
	Dsvld1Tbl	= PokeAGBsavedataAdr;

	for(i=0;i<28;i++){
		LoadSector(i,(u8 *)(&Dsvld1->svdata));
		sumcheck = 
			SumCheckCalc((u32*)Dsvld1->svdata.data,Dsvld1Tbl[Dsvld1->svdata.blocknum].size);
		Dsvld1->svdata.sumcheck = sumcheck;
		ProgramFlashSectorEx(i,(u8 *)(&Dsvld1->svdata));
	}

	Dsvld2		= (EXTRA_DATA*)(UserWork + 0x00000000);	
	Dsvld2Tbl	= PokeAGBexdataAdr;
	for(i=0;i<2;i++){
		LoadSector(i+28,(u8 *)Dsvld2);
		sumcheck = 
			SumCheckCalc((u32*)Dsvld2->data,Dsvld2Tbl[i].size);
		Dsvld2->sumcheck = sumcheck;
		ProgramFlashSectorEx(i+28,(u8 *)Dsvld2);
	}
}


#endif
