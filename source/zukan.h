#define	Z_SEE_CHK	(0)
#define	Z_GET_CHK	(1)
#define	Z_SEE_SET	(2)
#define	Z_GET_SET	(3)
#define	Z_SEE_RESET	(4)
#define	Z_GET_RESET	(5)

#define	ZDATA_TAKASA	(0)
#define	ZDATA_OMOSA		(1)

#define	ZUKAN_TYPE_SIZE	(5)

#define	NUM_INC			(0)
#define	NUM_DEC			(1)
#define	NUM_INC_LOOP	(2)
#define	NUM_DEC_LOOP	(3)

#define	ZUKANCOL_BLUE	(0)
#define	ZUKANCOL_RED	(1)

#define	POKE_NULL	(0xffff)
#define	LISTOVER	(0xffff)

#define	LIST_SCR_Y	(8)	//���X�g�����ݒ�i�X�N���[���j
#define	SCR_SP		(2)
#define	SCR_OFFS	(0x20/SCR_SP)
#define	SCR_SP2		(4)
#define	SCR_OFFS2	(0x20/SCR_SP2)
#define	SCR_SP3		(8)
#define	SCR_OFFS3	(0x20/SCR_SP3)
#define	SCR_SP4		(0x10)
#define	SCR_OFFS4	(0x20/SCR_SP4)
#define	SCR_SP5		(0x20)
#define	SCR_OFFS5	(0x20/SCR_SP5)

#define	POKE_DRAM_X	(96)	//���[���\���|�P�����w���W
#define	POKE_DRAM_Y	(80)	//���[���\���|�P�����x���W
#define	POKE_DRAM_H	(0x20)	//���[���\���|�P�����\�����I�t�Z�b�g
#define	POKE_DRAM_R	(76)	//���[�����a

#define	Z_SCR_NON	(0)
#define	Z_SCR_U		(1)
#define	Z_SCR_D		(2)

#define	Z_MODE0		(0) //	Ҳ݉�ʑ���
#define	Z_MODE1		(1) //	�߹���ް���ʑ���
#define	Z_MODE2		(2) //	������ʑ���
#define	Z_MODE3		(3) //	�������ʉ�ʑ���
#define	Z_MODE4		(4) //	�Ȃ�т�����ʑ���
#define	Z_MODE5		(5) //	���z�}��ʑ���
#define	Z_MODE6		(6) //	������ʑ���
#define	Z_MODE7		(7) //	������������׉�ʑ���

#define	ZLIST_NEW	(0) //	γ�ݒn���}��ؽ�
#define	ZLIST_WORLD	(1) //	�S���}��ؽ�

#define	ZSORT_NUM	(0) //	�ԍ���
#define	ZSORT_AIU	(1) //	�܏\����	
#define	ZSORT_OMOI	(2) //	�d����
#define	ZSORT_KARUI	(3) //	�y����
#define	ZSORT_TAKAI	(4) //	������
#define	ZSORT_HIKUI	(5) //	�Ⴂ��

#define	LIST_PRINT_ALL		(0)
#define	LIST_PRINT_UPPER	(1)
#define	LIST_PRINT_LOWER	(2)

#define	CELLID_ZUKANCURSOR	(0x1000)
#define	CELLID_ZUKANCURSOR3	(0x1001)
#define	PALID_ZUKANCURSOR	(0x1000)

#define	MODE1_POKE_POS_X	(0x30)
#define	MODE1_POKE_POS_Y	(0x38)

#define	DIAL_DEFAULT		(0x40)
#define	DIAL_OFFS			(0x20/2)
//#define	DIAL_SPEED			(8)

//ZUKANMAIN_TYPE	�}�Ӄv�����g�ݒ�(BG3,CGX_B = 2,SCR_B = 15)
//ZUKANLIST_TYPE	�}�Ӄv�����g�ݒ�(BG2,CGX_B = 2,SCR_B = 14)

/*  ���C�����X�g���  */
#define	MODE0_BG0_SCRVRAM	(12)		//���{�}�X�N�i�������j
#define	MODE0_BG0_CHRVRAM	(0)			//���{�}�X�N�i�������j
#define	MODE0_BG1_SCRVRAM	(13)		//�}�Ӄ��C�����
#define	MODE0_BG1_CHRVRAM	(0)			//�}�Ӄ��C�����
#define	MODE0_BG2_SCRVRAM	(14)		//ZUKANLIST_TYPE�g�p
#define	MODE0_BG2_CHRVRAM	(2)			//ZUKANLIST_TYPE�g�p
#define	MODE0_BG3_SCRVRAM	(15)		//���X�g���h�����w�i
#define	MODE0_BG3_CHRVRAM	(0)			//���X�g���h�����w�i

/*	�|�P�����f�[�^��ʑ���	*/
#define	MODE1_BG0_SCRVRAM	(0)			//���g�p
#define	MODE1_BG0_CHRVRAM	(0)			//���g�p
#define	MODE1_BG1_SCRVRAM	(13)		//�̃��j���[
#define	MODE1_BG1_CHRVRAM	(0)			//�̃��j���[
#define	MODE1_BG2_SCRVRAM	(14)		//ZUKANLIST_TYPE�g�p
#define	MODE1_BG2_CHRVRAM	(2)			//ZUKANLIST_TYPE�g�p
#define	MODE1_BG3_SCRVRAM	(15)		//�w�i
#define	MODE1_BG3_CHRVRAM	(0)			//�w�i

#define	MODE1_FOOTCGX_NO	(1024-4)	//���Ճ}�[�N�]���I�t�Z�b�g�iVRAM �Ō���Ȃ̂Œ��Ӂj

/*	������ʑ���  */
#define	MODE2_BG0_SCRVRAM	(0)			//���g�p
#define	MODE2_BG0_CHRVRAM	(0)			//���g�p
#define	MODE2_BG1_SCRVRAM	(0)			//���g�p	
#define	MODE2_BG1_CHRVRAM	(0)			//���g�p
#define	MODE2_BG2_SCRVRAM	(14)		//�w�i
#define	MODE2_BG2_CHRVRAM	(0)			//�w�i
#define	MODE2_BG3_SCRVRAM	(15)		//ZUKANMAIN_TYPE�g�p
#define	MODE2_BG3_CHRVRAM	(2)			//ZUKANMAIN_TYPE�g�p

/*  �������ʃ��X�g��ʁi�g�p���Ă��Ȃ��FMODE0�Ɠ����j  */
#define	MODE3_BG0_SCRVRAM	(12)		//���{�}�X�N�i�������j
#define	MODE3_BG0_CHRVRAM	(0)			//���{�}�X�N�i�������j
#define	MODE3_BG1_SCRVRAM	(13)		//�}�Ӄ��C�����
#define	MODE3_BG1_CHRVRAM	(0)			//�}�Ӄ��C�����
#define	MODE3_BG2_SCRVRAM	(14)		//ZUKANLIST_TYPE�g�p
#define	MODE3_BG2_CHRVRAM	(2)			//ZUKANLIST_TYPE�g�p
#define	MODE3_BG3_SCRVRAM	(15)		//���X�g���h�����w�i
#define	MODE3_BG3_CHRVRAM	(0)			//���X�g���h�����w�i

/*	�Ȃ�т�����ʑ���  */
#define	MODE4_BG0_SCRVRAM	(0)			//���g�p
#define	MODE4_BG0_CHRVRAM	(0)			//���g�p
#define	MODE4_BG1_SCRVRAM	(0)			//���g�p	
#define	MODE4_BG1_CHRVRAM	(0)			//���g�p
#define	MODE4_BG2_SCRVRAM	(14)		//�w�i
#define	MODE4_BG2_CHRVRAM	(0)			//�w�i
#define	MODE4_BG3_SCRVRAM	(15)		//ZUKANMAIN_TYPE�g�p
#define	MODE4_BG3_CHRVRAM	(2)			//ZUKANMAIN_TYPE�g�p

/*	���z�}��ʑ���  */
#define	MODE5_BG0_SCRVRAM	(0)			//���z�}process�Ŏg�p
#define	MODE5_BG0_CHRVRAM	(0)			//���z�}process�Ŏg�p
#define	MODE5_BG1_SCRVRAM	(13)		//�̃��j���[
#define	MODE5_BG1_CHRVRAM	(0)			//�̃��j���[
#define	MODE5_BG2_SCRVRAM	(0)			//���z�}process�Ŏg�p
#define	MODE5_BG2_CHRVRAM	(0)			//���z�}process�Ŏg�p
#define	MODE5_BG3_SCRVRAM	(0)			//���z�}process�Ŏg�p
#define	MODE5_BG3_CHRVRAM	(0)			//���z�}process�Ŏg�p

/*	������ʑ���  */
#define	MODE6_BG0_SCRVRAM	(31)		//����process�Ŏg�p�i�g�`�j
#define	MODE6_BG0_CHRVRAM	(1)			//����process�Ŏg�p�i�g�`�j
#define	MODE6_BG1_SCRVRAM	(13)		//�̃��j���[
#define	MODE6_BG1_CHRVRAM	(0)			//�̃��j���[
#define	MODE6_BG2_SCRVRAM	(14)		//�w�i�{����process�Ŏg�p�i���[�^�[�j
#define	MODE6_BG2_CHRVRAM	(0)			//�w�i�{����process�Ŏg�p�i���[�^�[�j
#define	MODE6_BG3_SCRVRAM	(15)		//ZUKANMAIN_TYPE�g�p
#define	MODE6_BG3_CHRVRAM	(2)			//ZUKANMAIN_TYPE�g�p

/*	������������׉�ʑ���  */
#define	MODE7_BG0_SCRVRAM	(0)			//���g�p
#define	MODE7_BG0_CHRVRAM	(0)			//���g�p
#define	MODE7_BG1_SCRVRAM	(13)		//�̃��j���[
#define	MODE7_BG1_CHRVRAM	(0)			//�̃��j���[
#define	MODE7_BG2_SCRVRAM	(14)		//�w�i
#define	MODE7_BG2_CHRVRAM	(0)			//�w�i
#define	MODE7_BG3_SCRVRAM	(15)		//ZUKANMAIN_TYPE�g�p
#define	MODE7_BG3_CHRVRAM	(2)			//ZUKANMAIN_TYPE�g�p

#define	LIST_NUM_CHR		(0xfe*2)	//���X�g�pNo.�L����
#define	LIST_BALL_CHR		(0xff*2)	//���X�g�p�{�[���L�����i���܂�����j

typedef struct {

u16	num;			//�߹�����ް
u16	look:1;			//�������׸�
u16	get:1;			//�߂܂����׸�

}ZUKAN_LIST;

typedef struct {

ZUKAN_LIST	listWork[POKE_NUM_MAX];
ZUKAN_LIST	monsData;
u16	listMax;
u16	list_p;
u16	list_p_bak;
u16	list_mode;
u16	list_mode_backup;
u16	list_sort;
u16	list_sort_backup;
u16	see_num;
u16	get_num;

u16	poke_str[4];
u16	poke_str_center;

s16	dial_sp;
s16	dial_bak;
u8	dial_theta;

u8	scroll_def;
u8	scroll_count;
u8	scroll_dir;
s16	scroll_line;
s16	scroll_line_work;
s16	scr_sp;
s16	scr_offs;
u16	list_scr_count;
u16	work[8];

u8	mode;
u8	mode_backup;

u8	back_col:1;
u8	menu_p;

u8	DataModeRetFlag;

u8	submenu_mode;
u16	submenu_p;
s16	submenu_BGoffs;

u8	Dummywork1[8];
u8	Dummywork2[8];
}ZUKAN_WORK;


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

#undef GLOBAL
#ifdef __ZUKAN_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

typedef void (*zpfunc)(u8);

GLOBAL	void	ZukanInit(void);
GLOBAL	u8		ZukanDataReturn(zpfunc TaskAdrs);
GLOBAL	void	ZukanMain(void);
GLOBAL	void	ZukanFirstInit(void);
GLOBAL	void	ZukanWorkInit(void);
GLOBAL	s8		ZukanCheck(u16	poke_num,u8	mode);
GLOBAL	u16		ZukanTotal(u8 mode);
GLOBAL	u8*		ZukanPokeTypeGet(u16 poke_num);
GLOBAL	u16		ZukanPokeDataGet(u16 poke_num,u8 mode);
GLOBAL	void	ZukanPaletteSet(void);
GLOBAL	void	ZukanListMake(u8 mode,u8 sort);

GLOBAL	ZUKAN_WORK*		zw;
GLOBAL	u16		ZukanListPos;
GLOBAL	u8		ZukanDialTheta;
GLOBAL	u8		ZukanDebugFlag;

GLOBAL	const u16	zukan1_Palette[];
GLOBAL	const u16	zukan2_Palette[];
GLOBAL	const u8	zkn_sc0_LZ[];
GLOBAL	const u8	zkn_sc1_LZ[];
GLOBAL	const u8	zkn_sc3_LZ[];
GLOBAL	const u8	zkn_scp_LZ[];
GLOBAL	const u8	zkn_scv_LZ[];
GLOBAL	const u8	zkn_scc_LZ[];
GLOBAL	const u8	zkn_scm1_LZ[];
GLOBAL	const u8	zkn_scm2_LZ[];
GLOBAL	const u8	zkn_scb_LZ[];
GLOBAL	const u8	zkn_tch_LZ[];
GLOBAL	const u8	zkn_ch_LZ[];

#undef GLOBAL

#ifdef __ZUKAN_DATA_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

GLOBAL	u8		ZukanDataTaskSet(ZUKAN_LIST* data,u8 actstr);
GLOBAL	u8		ZukanDataTaskChange(ZUKAN_LIST* data,u8 id);
GLOBAL	u8		ZukanSampleSet(u16 pokenum,u32 id,u32 per_rnd);
GLOBAL	const	ZUKAN_DATA	zukanData[];

#undef GLOBAL

#ifdef __ZTOOL_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

GLOBAL	u8*		ZukanPokeTypeGet(u16 poke_num);
GLOBAL	u16		ZukanPokeDataGet(u16 poke_num,u8 mode);
GLOBAL	s8		ZukanCheck(u16	poke_num,u8	mode);
GLOBAL	u16		ZukanTotal(u8 mode);
GLOBAL	u16		ZukanHouenTotal(u8 mode);
GLOBAL	void	ZukanDataTypeNamePut(u8* name,u8 x,u8 y);
GLOBAL	void	ZukanDataNumPut(u16 num,u8 x,u8 y);
GLOBAL	u16		Z_NumIncDec(u8 mode,u16 p,u16 min,u16 max);
GLOBAL	void	ZukanScreenRegInit(u16 val);
GLOBAL	u16		ZukanPokeActorSet(u16 pokenum,s16 x,s16 y,u16 decord_p);
GLOBAL	u16		ZukanTrainerActorSet(u16 trainernum,s16 x,s16 y,u16 decord_p);
GLOBAL	void	ZukanPokeNumPut(u16 pokenum,u8 x,u8 y);
GLOBAL	u8		ZukanPokeNamePut(u16 pokenum,u8 x,u8 y);
GLOBAL	void	ZukanPokeFootTrans(u16 pokenum,u16 cgx_num,u16 offs);
GLOBAL	void	ZukanPokeFootPut(u16 scr_num,u16 chr_num);

GLOBAL	u16			ZukanHyousyouCheckHouen(void);
GLOBAL	u16			ZukanHyousyouCheckZenkoku(void);

#undef GLOBAL

