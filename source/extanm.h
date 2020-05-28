
#ifndef __EXTANM_H__
#define __EXTANM_H__


//----------------------------------------------------------
//	�\���̐錾
//----------------------------------------------------------
typedef struct{
	ActOamData *oamData;			//Add���鱸����Oam�ް�
	u8 *DataAdrs;					//�]�������ް��̐擪���ڽ
	const actAnm * const *anm_tbl;	// ���ð���
	const actAffAnm * const *aff_tbl;	// �g�k�߯�
	void (*move)(struct actWork_tag *);
	u16 pal_id;
}ExtActHeader;


//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern u8 ExtAnm(actWork *Xreg);
extern u8 ExtAddActor(ExtActHeader *eah, s16 x, s16 y, u8 pri);
extern void ExtDelActor(actWork *Xreg);
extern void ExtAnmChg(actWork *xreg, u8 anmNo);


#endif
