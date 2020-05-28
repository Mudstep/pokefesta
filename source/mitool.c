//<mitool.c>
//////////////////////////////////////////////////////////////////
//
//	BG�R���g���[���֌W
//
//									2002.05.30.	by Miyuki Iwasawa
//////////////////////////////////////////////////////////////////

#include "mitool.h"

//////////////////////////////////////////////////////////////////
//	�J���[�P�ʂ̃A�j��
//////////////////////////////////////////////////////////////////
#if 0
static void ColAnmWorkClear(ColAnmWork *pWork)
{
	pWork->st_flg = 0;
	pWork->live_sw = 0;

	pWork->uc_wCt = pWork->uc_dCt = 0;
	pWork->dx = 0;

	memset(&(pWork->dat),0,sizeof(ColAnmData));
}
#endif
void ColorAnimeInit(ColorAnime *pCAnm)
{
	pCAnm->uc_flg = 0;
	pCAnm->w_actsw = 0;

	memset(pCAnm->work,0,sizeof(ColAnmWork)*COLANM_MAX);
}
u8 AddColorAnime(ColorAnime *pCAnm,u8 no,ColAnmData *pData)
{
	if(no >= COLANM_MAX) return 0xFF;			//�����i���o�[
	if(pCAnm->work[no].live_sw) return 0xFF;	//���łɃf�[�^���݂�
	
	//�p�����[�^���擾
	pCAnm->work[no].dat.col		= pData->col;
	pCAnm->work[no].dat.w_colNo = pData->w_colNo;
	pCAnm->work[no].dat.uc_num	= pData->uc_num;
	pCAnm->work[no].dat.uc_wait = pData->uc_wait;
	pCAnm->work[no].dat.uc_loop = pData->uc_loop;
	
	pCAnm->work[no].dat.nStep	= pData->nStep;
	pCAnm->work[no].dat.type	= pData->type;
	pCAnm->work[no].dat.dir		= pData->dir;

	//���[�N�G���A������
	pCAnm->work[no].st_flg = 0;
	pCAnm->work[no].live_sw = 1;

	pCAnm->work[no].uc_wCt = pCAnm->work[no].uc_dCt = 0;

	if(pCAnm->work[no].dat.dir) pCAnm->work[no].dx = -1;
	else						pCAnm->work[no].dx = 1;

	return no;
}
u8 DelColorAnime(ColorAnime* pCAnm,u8 no)
{
	if(no >= COLANM_MAX) return 0xFF;			//�����i���o�[
	if(!pCAnm->work[no].live_sw) return 0xFF;	//�f�[�^���Ȃ�

	memset(&(pCAnm->work[no]),0,sizeof(ColAnmWork));
	return no;
}
//���x�̏グ�����A�j���[�V����
static u8 color_anime(ColAnmWork *pWork)
{
	u8	i = 0;
	_rgb	*col,*org;
	
	for(i = 0;i < pWork->dat.uc_num;i++)
	{
		col = (_rgb *)&(PaletteWorkTrans[pWork->dat.w_colNo+i]);
		org = (_rgb *)&(PaletteWork[pWork->dat.w_colNo+i]);
		switch(pWork->st_flg)
		{
		case 1:		//�s��
			if(col->r+pWork->dx <= 31 && col->r+pWork->dx >= 0)col->r += pWork->dx;
			if(col->g+pWork->dx <= 31 && col->g+pWork->dx >= 0)col->g += pWork->dx;
			if(col->b+pWork->dx <= 31 && col->b+pWork->dx >= 0)col->b += pWork->dx;
			break;
		case 2:		//�A��
			if(pWork->dx < 0){
				if(col->r+pWork->dx >= org->r) col->r += pWork->dx;
				if(col->g+pWork->dx >= org->g) col->g += pWork->dx;
				if(col->b+pWork->dx >= org->b) col->b += pWork->dx;
			}
			else{
				if(col->r+pWork->dx <= org->r) col->r += pWork->dx;
				if(col->g+pWork->dx <= org->g) col->g += pWork->dx;
				if(col->b+pWork->dx <= org->b) col->b += pWork->dx;
			}
			break;
		}
	}
	if(pWork->uc_dCt++ != (u8)(pWork->dat.nStep)) return 0;

	pWork->uc_dCt = 0;	//�J�E���^���Z�b�g
	pWork->dx *= -1;
	if(pWork->st_flg == 1)	pWork->st_flg++;
	else					pWork->st_flg--;

	return 1;
}
//�I���W�i���Ǝw��F�̓_��
static u8 color_anime2(ColAnmWork *pWork)
{
	u8	i = 0;
	
	switch(pWork->st_flg)
	{
	case 1:
		for(i = 0;i < pWork->dat.uc_num;i++){
			PaletteWorkTrans[pWork->dat.w_colNo+i] = pWork->dat.col;
		}
		pWork->st_flg++;
		break;
	case 2:
		for(i = 0;i < pWork->dat.uc_num;i++){
			PaletteWorkTrans[pWork->dat.w_colNo+i] = 
				PaletteWork[pWork->dat.w_colNo+i];
		}
		pWork->st_flg--;
		break;
	}
	return 1;
}

//PaleteFadeTrans�̑O�ɌĂ�
void ColorAnimeLum(ColorAnime *pCAnm)
{
	u8	i = 0;
	
	if(!pCAnm->uc_flg) return;

	for(i = 0;i < COLANM_MAX;i++)
	{
		if(!((pCAnm->w_actsw >> i) & 0x0001) ) continue;

		//�A�j������
//		if(pCAnm->work[i].uc_wCt++ == pCAnm->work[i].dat.uc_wait){
		if(pCAnm->work[i].uc_wCt-- == 0){
			if(pCAnm->work[i].dat.col & 0x8000){
				color_anime(&(pCAnm->work[i]));
			}
			else{
				color_anime2(&(pCAnm->work[i]));
			}
//			pCAnm->work[i].uc_wCt = 0;
			pCAnm->work[i].uc_wCt = pCAnm->work[i].dat.uc_wait;
		}
	}
	return;
}
void ColorAnimeStart(ColorAnime* pCAnm,u16 sw)
{
	u8	i = 0;
	
	pCAnm->uc_flg++;
	
	for(i = 0;i < COLANM_MAX;i++)
	{
		if(!((sw >> i) & 0x0001))	continue;
		if(!pCAnm->work[i].live_sw) continue;

		//�ғ��t���O�𗧂Ă�
		pCAnm->w_actsw |= (0x0001 << i);
		pCAnm->work[i].st_flg = 1;

		//��1��ڂ�����
#if 0
		if(pCAnm->work[i].dat.col & 0x8000){
			color_anime(&(pCAnm->work[i]));
		}
		else{
			color_anime2(&(pCAnm->work[i]));
		}
#endif		
	}	
}
void ColorAnimeEnd(ColorAnime *pCAnm,u16 sw)
{
	u8	i = 0;

	for(i = 0;i < COLANM_MAX;i++)
	{
		if(!((pCAnm->w_actsw >> i) & 0x0001)) continue;
		if(!pCAnm->work[i].live_sw) continue;
		if(!((sw >> i) & 0x0001))	continue;

		//���̃J���[�ɖ߂��Ă���I���
		memcpy(	&(PaletteWorkTrans[pCAnm->work[i].dat.w_colNo]),
				&(PaletteWork[pCAnm->work[i].dat.w_colNo]),
				pCAnm->work[i].dat.uc_num*2);

		pCAnm->work[i].st_flg = 0;
		pCAnm->work[i].uc_wCt = pCAnm->work[i].uc_dCt = 0;

		if(pCAnm->work[i].dat.dir)	pCAnm->work[i].dx = -1;
		else						pCAnm->work[i].dx = 1;
	}
	if(sw == 0xFFFF){ 
		pCAnm->uc_flg = 0;
		pCAnm->w_actsw = 0;
	}
	else{
		pCAnm->w_actsw &= (~sw);
	}
}
//////////////////////////////////////////////////////////////////
//	�X�N���[���f�[�^�̍쐬
//////////////////////////////////////////////////////////////////
/*-----------------------------------------
	256*256�e�L�X�g�X�N���[��(16�F���[�h)���̎w�肵����`��
	�w�肵���L�����N�^�Ŗ��߂�

	void FillChar16(u16* ps,u16 srcChar,u8 x,u8 y,u8 nx,u8 ny);

	�����F
	io	u16*	�PBG���̃X�N���[���f�[�^���i�[����o�b�t�@�ւ̃|�C���^
	i	u16*	���߂�L�����N�^�f�[�^
	i	u8		��`�̎n�_X(�L�����N�^��)
	i	u8		��`�̎n�_Y
	i	u8		��`��X�T�C�Y
	i	u8		��`��Y�T�C�Y
-------------------------------------------*/
void FillChar16(u16* ps,u16 srcChar,u8 x,u8 y,u8 nx,u8 ny)
{
	u8 width = 0,height = 0;
	u16	*p = NULL;

	ps += y*32 + x;		//��`�̎n�܂�ŏ��̃A�h���X

	for(height = 0;height < ny;height++ )
	{
		p = ps + height*32;	//X�����ǂ�����͂��߂邩���v�Z
		
		for(width = 0;width < nx;width++)
		{
			*p = srcChar;
			p += 1;
		}
	}
}

/*-----------------------------------------
	256*256�e�L�X�g�X�N���[��(16�F���[�h)���̎w�肵����`��
	�w�肵���L�����N�^�z��Ŗ��߂�
	*�w�肷��L�����N�^�z��͂P�����}�b�s���O�ɂȂ��Ă��邱��

	void SetChar16(u16* ps,const u16* srcChar,u8 x,u8 y,u8 nx,u8 ny);

	�����F
	io	u16*	�PBG���̃X�N���[���f�[�^���i�[����o�b�t�@�ւ̃|�C���^
	i	u16*	���߂�L�����N�^�f�[�^�����߂��z��ւ̃|�C���^
	i	u8		��`�̎n�_X(�L�����N�^��)
	i	u8		��`�̎n�_Y
	i	u8		��`��X�T�C�Y
	i	u8		��`��Y�T�C�Y
-------------------------------------------*/
void SetChar16(u16* ps,const u16* srcChar,u8 x,u8 y,u8 nx,u8 ny)
{
	u8 width = 0,height = 0;
	u16	*p = NULL;
	u16 *pSrc = (u16 *)srcChar;

	ps += y*32 + x;		//��`�̎n�܂�ŏ��̃A�h���X

	for(height = 0;height < ny;height++ )
	{
		p = ps + height*32;	//X�����ǂ�����͂��߂邩���v�Z
		
		for(width = 0;width < nx;width++)
		{
			*p = *pSrc;
			pSrc++;
			p++;
		}
	}
}

/*-----------------------------------------
	256*256�e�L�X�g�X�N���[��(16�F���[�h)���̎w�肵����`��
	�w�肵���L�����N�^�Ŗ��߂�
	��荞�ݑΉ�
	
	void FillChar16R(_tMap* ps,u16 srcChar,u8 x,u8 y,u8 nx,u8 ny);

	�����F
	io	u16*	�PBG���̃X�N���[���f�[�^���i�[����o�b�t�@�ւ̃|�C���^
	i	u16		���߂�L�����N�^�f�[�^
	i	u8		��`�̎n�_X(�L�����N�^��)
	i	u8		��`�̎n�_Y
	i	u8		��`��X�T�C�Y
	i	u8		��`��Y�T�C�Y
-------------------------------------------*/
void FillChar16R(_tMap* pMap,u16 srcChar,u8 x,u8 y,u8 nx,u8 ny)
{
	u8	height = 0,width = 0;
	u8	sx = x,sy = y;

	for(height = 0;height < ny;height++ )
	{
		sx = x;		//X�l���Z�b�g
		for(width = 0;width < nx;width++)
		{
			pMap->usa[sy][sx] = srcChar;

			sx = (sx+1)%32;	//��荞��
		}
		sy = (sy+1)%32;	//Y�l��荞�ݍX�V
	}
}

/*-----------------------------------------
	256*256�e�L�X�g�X�N���[��(16�F���[�h)���̎w�肵����`��
	�w�肵���L�����N�^�z��Ŗ��߂�
	*�w�肷��L�����N�^�z��͂P�����}�b�s���O�ɂȂ��Ă��邱��
	*��荞�ݑΉ�

	void SetChar16R(_tMap* ps,const u16* srcChar,u8 x,u8 y,u8 nx,u8 ny);

	�����F
	io	u16*	�PBG���̃X�N���[���f�[�^���i�[����o�b�t�@�ւ̃|�C���^
	i	u16*	���߂�L�����N�^�f�[�^�����߂��z��ւ̃|�C���^
	i	u8		��`�̎n�_X(�L�����N�^��)
	i	u8		��`�̎n�_Y
	i	u8		��`��X�T�C�Y
	i	u8		��`��Y�T�C�Y
-------------------------------------------*/
void SetChar16R(_tMap* pMap,const u16* srcChar,u8 x,u8 y,u8 nx,u8 ny)
{
	u8	height = 0,width = 0;
	u16 *pSrc = (u16 *)srcChar;
	u8	sx = x,sy = y;

	for(height = 0;height < ny;height++ )
	{
		sx = x;		//X�l���Z�b�g
		for(width = 0;width < nx;width++)
		{
			pMap->usa[sy][sx] = *pSrc;
			pSrc++;
			sx = (sx+1)%32;	//��荞��
		}
		sy = (sy+1)%32;	//Y�l��荞�ݍX�V
	}
}

//	BG�A�N�^�[�֌W�̃c�[��
//==========================================================
/*--------------------------------------------------------
	_bgAct�\���̂̏�����
	
	void  BgActInit(_bgAct* pAct);
----------------------------------------------------------*/
void  BgActInit(_bgAct* pAct)
{
	pAct->uc_nPal = pAct->uc_nCell = 0;
	pAct->us_nChar = 0;

	pAct->pCharBaseAddr = (u16 *)VRAM;
	DmaClear(3,0,&(pAct->aPal),sizeof(_bgPal)*BGACT_PAL_MAX,16);
	DmaClear(3,0,&(pAct->aCell),sizeof(_bgCell)*BGACT_CELL_MAX,16);
	return;	
}

/*-------------------------------------------------------
	_bgAct��bgCell��o�^
	VRAM�ɃL�����N�^�f�[�^��]������

	u8	BgCellSet(_bgAct pAct,_bgCell* pCell);

	io	_bgAct*		bgCell��o�^����_bgAct�\���̂ւ̃|�C���^
	i	_bgCell*	�o�^����bgCell�f�[�^�ւ̃|�C���^

	�߂�l
	u8	0:		����I��
		else:	�G���[
--------------------------------------------------------*/
u8	BgCellSet(_bgAct* pAct,_bgCell* pCell)
{
	u16		num = 0;
	
	while(1)
	{
		//�z�񂪂����ς��Ȃ̂ł���
		if(pAct->uc_nCell > BGACT_CELL_MAX-1) return 1;
		
		//�L�����N�^�f�[�^��NULL�Ȃ�Δ�����
		if(pCell->pCharData == NULL) break;

		//�o�^�����A�f�[�^���R�s�[
		pAct->aCell[pAct->uc_nCell].pCharData	= pCell->pCharData;
		pAct->aCell[pAct->uc_nCell].uc_ID		= pCell->uc_ID;
		pAct->aCell[pAct->uc_nCell].uc_xSiz		= pCell->uc_xSiz;
		pAct->aCell[pAct->uc_nCell].uc_ySiz		= pCell->uc_ySiz;

		//�o�^�ς݂̃L�����N�^�����X�V
		num = pCell->uc_xSiz*pCell->uc_ySiz;
		if(pAct->us_nChar + num > BGACT_CHAR_MAX) return 2;	//�o�^�ł���L�����N�^�̍ő�l�𒴂���̂ł���

		//VRAM�ɃL�����N�^�f�[�^��]��
		if(pCell->us_ofs == BGACT_NORMAL)	//�m�[�}���f�[�^
		{
			DmaCopy(3,	pCell->pCharData,
						(vu16 *)(pAct->pCharBaseAddr+(32*pAct->us_nChar)),
						32*num,16);
		}
		else	//���k�f�[�^
		{
			DecordWram(	pCell->pCharData,
						(void *)(pAct->pCharBaseAddr+(32*pAct->us_nChar)));
		}
		//�Z���̃L�����N�^�f�[�^�ւ̃I�t�Z�b�g���o���Ă���
		pAct->aCell[pAct->uc_nCell].us_ofs = pAct->us_nChar;
		pAct->us_nChar += num;		//�o�^���ꂽ�L�����N�^���̍X�V

		//�Z���f�[�^�̐����X�V
		pAct->uc_nCell++;

		//�A�h���X�X�V
		pCell++;
	}
	return 0;
}

/*-----------------------------------------------
	_bgAct��_bgPal��o�^

	pPal->uc_palNo��
		���k�f�[�^		BGACT_COMP
		�m�[�}���f�[�^	BGACT_NORMAL
	���̃t���O���`���ăp�����[�^�Ƃ���

	�p���b�gWORK�Ƀp���b�g�f�[�^��]������

	u8	BgPalSet(_bgAct* pAct,_bgPal *pPal);

	����:
	io	_bgAct*		_bgPal��o�^����A_bgAct�\���̂ւ̃|�C���^
	i	_bgPal*		�o�^�������p���b�g�f�[�^��ID���i�[����_bgPal�\���̂ւ̃|�C���^

	�߂�l�F
	u8		0:����
			else:	�G���[�R�[�h
--------------------------------------------------*/
u8	BgPalSet(_bgAct* pAct,_bgPal *pPal)
{
	u16		pal_buf[16];
	
	while(1){
		//�z�񂪂����ς��Ȃ̂ł���
		if(pAct->uc_nPal > BGACT_PAL_MAX-1) return 1;

		//�p���b�g�f�[�^����Ȃ甲����
		if(pPal->pPalData == NULL) break;

		//�f�[�^���R�s�[���ēo�^
		pAct->aPal[pAct->uc_nPal].pPalData = pPal->pPalData;
		pAct->aPal[pAct->uc_nPal].uc_ID = pPal->uc_ID;

		//�p���b�g�f�[�^���p���b�gWORK�ɓ]��
		if(pPal->uc_palNo == BGACT_NORMAL){
			PaletteWorkSet((const u16 *)(pPal->pPalData),PA_BG0+pAct->uc_nPal*16,32);
		}
		else{
			DecordWram(pPal->pPalData,pal_buf);		
			PaletteWorkSet(pal_buf,PA_BG0+pAct->uc_nPal*16,32);
		}

		//�p���b�g�i���o�[���L��
		pAct->aPal[pAct->uc_nPal].uc_palNo = pAct->uc_nPal;
		
		//�o�^���ꂽ�p���b�g�����X�V
		pAct->uc_nPal++;

		//�\�[�X�A�h���X�X�V
		pPal++;
	}
	return 0;
}

/*-----------------------------------------------
	_bgAct��_bgPal��o�^-�����p���b�g������V���{�����ň��k����Ă���ꍇ
	pPal->uc_palNo�Ƀp���b�g�����`���ăp�����[�^�Ƃ���

	�p���b�gWORK�Ƀp���b�g�f�[�^��]������

	u8	BgPalSetComp(_bgAct* pAct,_bgPal *pPal);

	����:
	io	_bgAct*		_bgPal��o�^����A_bgAct�\���̂ւ̃|�C���^
	i	_bgPal*		�o�^�������p���b�g�f�[�^��ID���i�[����_bgPal�\���̂ւ̃|�C���^

	�߂�l�F
	u8		0:����
			else:	�G���[�R�[�h
--------------------------------------------------*/
u8	BgPalSetComp(_bgAct* pAct,_bgPal *pPal)
{
	u16		pal_buf[16][16] = {};
	u8		ct = 0,num = pPal->uc_palNo;

	//���k�f�[�^���o�b�t�@�ɓW�J
	DecordWram(pPal->pPalData,pal_buf);
	
	for(ct = pAct->uc_nPal;ct < num;ct++)
	{
		if(pAct->uc_nPal+ct > BGACT_PAL_MAX-1) return 1;
		
		//�f�[�^��o�^
		pAct->aPal[ct].pPalData = pPal->pPalData;
		pAct->aPal[ct].uc_ID = pPal->uc_ID+ct;		//ID�͘A��
		pAct->aPal[ct].uc_palNo = pAct->uc_nPal;

		PaletteWorkSet(&(pal_buf[ct][0]),PA_BG0+pAct->uc_nPal*16,32);
		pAct->uc_nPal++;
	}
	return 0;
}

