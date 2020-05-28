/********************************************************************/
/*                �����֘AҲ�                                       */
/********************************************************************/
/*==================================================================*/
#ifndef ACTOR_H
#define ACTOR_H

#include "actanm.def"	//add

//----------------------------------------------------------
//	�n�`�l�\����
//	agbtypes.h�ɂ��������̂���AffineParamNo_L3��HFlip,VFlip���ꏏ�ɂ�������
//	�V�X�e���p
//----------------------------------------------------------
typedef struct {
    u32 VPos:8;     	    // �x���W
    u32 AffineMode:2;       // �A�t�B�����[�h
    u32 ObjMode:2;          // �n�a�i���[�h
    u32 Mosaic:1;           // ���U�C�N
    u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
    u32 Shape:2;            // �n�a�i�`��

    u32 HPos:9;             // �w���W
    u32 AffineParamNo:5;	// �A�t�B���ϊ��p�����[�^�m��&H�د��,V�د��
    u32 Size:2;             // �n�a�i�T�C�Y

#if 0
    u32 CharNo:10;          // �L�����N�^�m��
    u32 Priority:2;         // �\���D��
    u32 Pltt:4;             // �p���b�g�m��
#else
    u16 CharNo:10;          // �L�����N�^�m��
    u16 Priority:2;         // �\���D��
    u16 Pltt:4;             // �p���b�g�m��
#endif

    u16 AffineParam;        // �A�t�B���ϊ��p�����[�^
} SysOamData;

typedef SysOamData ActOamData;	//ActOamData����AffineParam���Ȃ������Ƃ͉\(���͂��ĂȂ�)

/*------------------------------------------------------------------*/
/*	DMA�]���pð��ٍ\����											*/
/*------------------------------------------------------------------*/
typedef struct{
	const u8 *dat;	        //��׸���ް����ڽ
	u32 	dest;	        //�]������ڽ(�̾��)
	u16 	size;			//����
}dmaTransTbl;

/*------------------------------------------------------------------*/
/*	��������ް��\����												*/
/*------------------------------------------------------------------*/
typedef struct {
	const u8 *dat;	        //��׸���ް����ڽ
	u16 	size;			//����
}actAnmData;

/*------------------------------------------------------------------*/
/*	�����������ݍ\����												*/
/*------------------------------------------------------------------*/
typedef struct {
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
}actAnm;

/*------------------------------------------------------------------*/
/*	����ܰ��\����													*/
/*------------------------------------------------------------------*/
#define ACTOR_WORK_MAX		8				//�������������Ă���ܰ��̐�

typedef struct actWork_tag
{
	ActOamData oamData;						// OAM ��{�ް�
	const actAnm * const *anm_tbl;			// ���ð���
	const actAnmData *anm_trans; 			// �����ݽ�ް�
	const actAffAnm * const *aff_tbl;		// �̨ݱ��ð���
	const struct actHeader_tag *actHead;	// �\������ͯ��
	const OAMPatSt *oamPat;					// OAM�߯�

	void (*move)(struct actWork_tag *);		// ����֐�
	
	s16 	x;								// X���W
	s16 	y;								// Y���W
	s16 	dx;								// X���W+��
	s16 	dy;								// Y���W+��
	s8 		cx;								// X���W+���S���W�擾�p�␳
	s8 		cy;								// Y���W+���S���W�擾�p�␳
	u8 		anm_no;							// �������
	u8 		anm_offs;						// ������޵̾��

	u8 		anm_wait:6;						// ��ҳ���
	u8		anm_pause:1;					// ����߰��
	u8		aff_pause:1;					// �̨ݱ���߰��
	u8		anm_count;						// ���ٰ��

	s16		work[ACTOR_WORK_MAX];						// ����ܰ�

	u16		act_sw	:1;						// �����ғ��׸�(1:ON)
	u16		scr		:1;						// BG��۰وˑ��׸� 1:�ˑ� 0:OFF
	u16		banish	:1;						// ���ײĕ\���׸�(1:OFF(�\�����Ȃ�))
	u16		:5;								// ��а
	
	u16		Hflip	:1;						// �����]�׸�(1:���])
	u16		Vflip	:1;						// �c���]�׸�(1:���])
	u16 	anm_sw	:1;						// ��ҕύX�׸�(1:�J�n)
	u16		affine_sw:1;					// �̨ݱ�ҕύX�׸�
	u16 	anmend_sw:1;					// ��ҏI���׸�(1:�I��)
	u16		affend_sw:1;					// �̨ݱ�ҏI���׸�
	u16		t_mode:1;						// CGX�]���׸�(0:�]�� 1:�����)
	
	u16		feature_flg:1;					// ���������׸�
	u16		char_num;						// �̾�ķ������
	
	u8		oampat_no:6;					// �߯�����
	u8		oampat_sw:2;					// �߯Ľ���

	u8		pri;							// �����\����ײ��è(�� 0 ~ 255 ��)
}actWork, *actWork2;

/*------------------------------------------------------------------*/
/*	����ͯ�ލ\����													*/
/*------------------------------------------------------------------*/
typedef struct actHeader_tag{
#if 0										//change 2001.08.02(��) by matsuda
	const u16 cellNo;						//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
	const u16 palNo;						//��گ��Ȱ�ެ�o�^����
#else
	u16 cellNo;								//�����ް(0xffff�̏ꍇ�ͷ�ד]���^)
	u16 palNo;								//��گ��Ȱ�ެ�o�^����
#endif
	const ActOamData *oamData_p;			//OAM��{�ް����ڽ
	const actAnm * const *anmData_p;		//ACTOR�\�������ð��ٱ��ڽ
	const actAnmData *anmTrans_p;			//���-��ݽ�ް����ڽ
	const actAffAnm * const *affData_p;		// �g�k�߯�
	void (*moveData_p)(actWork *);			//ACTOR��������݊֐����ڽ
}actHeader;

//----------------------------------------------------------
//	�^�錾
//----------------------------------------------------------
typedef void (*pActFunc)(actWork *);		//�����̓���֐�����

//----------------------------------------------------------
//	�萔��`
//----------------------------------------------------------
#define ACT_MAX				(64)			// ����MAX
#define ACT_BUF_MAX			(ACT_MAX+1)		// ����ܰ�MAX
#define ACTWORK_SIZEOF		(sizeof(actWork))				// ܰ�����
#define ACTWORK_SIZEOF_ALL	(ACTWORK_SIZEOF*ACT_BUF_MAX)	// ܰ����ޑS��
#define ACTWORK_SIZEOF_OPERA (ACTWORK_SIZEOF*ACT_MAX)		// ܰ����މғ���

//-- ����ٰ�ݓ��Ŏg�p���Ă���DMA�]����CPU�]���ɕύX���邽�߂̒�` --//
#define DMA_CPU_SELECT		0				// 0����CpuCopy���g�p	1����DmaCopy���g�p

//----------------------------------------------------------
//	�O���[�o���ϐ�
//----------------------------------------------------------
extern actWork ActWork[ACT_BUF_MAX];		// ����ܰ�
extern u8 ActSortTbl[ACT_MAX];				//������ėpܰ�
extern u8 DMAtransCount;					//DMA�]���p����
extern dmaTransTbl DMAtransTbl[];			//DMA�]���pð���
extern u8 OamWriteEd;						//�g�pOAM�͈͌���(0�`OamWriteEd(MAX:128))
extern u16 ObjDataTransSt;					//OBJ��׸��ް�VRAM�]���J�n�ʒu�w��(��׸�No)
extern u8 TransAreaFlag[128];				//�]���󂫗̈������׸�(1024bit)

extern s16 ActOX;							//��۰��ٱ����␳X
extern s16 ActOY;							//��۰��ٱ����␳Y
extern ObjAffineDestData OBJ_AffineWork[32];
extern u8 AffAnmStopFlag;

//----------------------------------------------------------
//	��а�ް� �O���錾
//----------------------------------------------------------
extern const actWork DummyActWork;					//��а����ܰ�
extern const ActOamData DummyActOam;				//��аOAM�ް�
extern const actAnm* const DummyActAnmTbl[];		//��а��Ұ���ð���
extern const actAffAnm * const DummyActAffTbl[];	//��а�g�k���ð���
extern const actHeader DummyActHeader;

//----------------------------------------------------------
//	�O���ް�
//----------------------------------------------------------
extern const u8 ActSizeTbl[2][3][4];

//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern void affineWorkinit(void);
extern void actOAMinit(u8 start, u8 end);
extern void SetActor(void);
extern void actOAMtrans(void);
extern void actDMAtrans(void);
extern void InitActor(void);
extern void JumpActor(void);
extern void SetActor(void);
extern u8 AddActor(const actHeader* headAdrs, s16 posX, s16 posY, u8 pri);
extern void DelActor(actWork *);
extern void actCenterCalc( actWork *, u8, u8, u8 );
extern void DummyActMove(actWork *);

extern void DmaTransReq(u8 *datAdr, u32 destAdr, u16 size);

extern void PushActor(void *destAdr);
extern void PopActor(void *destAdr);
extern void InitActWork(void);
extern void actWorkInit(actWork *actStruct);
extern u8 AddActorBottom(const actHeader* headAdrs, s16 posX, s16 posY, u8 pri);

extern void CellDelActor(actWork *actst);
extern void PalDelActor(actWork *actst);
extern void AffineDelActor(actWork *actst);
extern void AllDelActor(actWork *actst);
extern u8 AddActorJump(const actHeader* headAdrs, s16 posX, s16 posY, u8 pri);

extern u8 AddDummyActor( void (*move)(actWork*) );

//----------------------------------------------------------
//	ϸ�
//----------------------------------------------------------
#define	actNoCenterCalc(no,shape,size,aff)	actCenterCalc(&ActWork[no],shape,size,aff)

#endif	//ACTOR_H
