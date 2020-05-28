//=========================================================================
//	
//	svld_sys.h
//	�Z�[�u�֘A�Z�N�^���[�e�[�V�����\��
//
//=========================================================================
#undef GLOBAL
#ifdef __SVLD_SYS_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
#define	SVLD_HEADER_SIZE	(4+2+4+2)	//�w�b�_�T�C�Y
#define	SVLD_SECTOR_SIZE	(0x1000)	//�Z�N�^�T�C�Y
#define	SVLD_DATA_SIZE		(SVLD_SECTOR_SIZE - SVLD_HEADER_SIZE)	//���f�[�^�T�C�Y
#define	SVLD_SECTB_COUNT	(SVLD_SECTOR_COUNT)		//���[�e�[�V�����Z�N�^��
#define SVLD_MIRROR_SIZE	(14)

#define	SVLD_SECTOR_DEATH	(0xff)		//�Z�N�^�X�e�[�^�X��`�i�������ݕs�j
#define	SVLD_SECTOR_LIVE	(0xfe)		//�Z�N�^�X�e�[�^�X��`�i�������݉j

#define	SVLD_RET_NG			(0xff)		//�Z�[�u���ʁi�������ݏo���Ȃ������j
#define	SVLD_RET_NULL		(0x0)		//�Z�[�u���ʁi�o�b�N�A�b�v�Ȃ��j
#define	SVLD_RET_OK			(0x1)		//�Z�[�u���ʁi����ɏI���j
#define	SVLD_RET_DESTROY	(0x2)		//�Z�[�u���ʁi���S�j��F���A�s�\�j
#define	SVLD_RET_FULL		(0x3)		//�Z�[�u���ʁi����ȏ㏑�����ݏo���Ȃ��j
#define	SVLD_RET_HARDERROR	(0x4)		//�n�[�h�G���[�i��ՂȂ��j

#define	SVLD_MODE_SVLDSYS	(0x0000)	//���[�h��`�i�Z�[�u�V�X�e���u���b�N�j
#define	SVLD_MODE_ALL		(0xffff)	//���[�h��`�i�S�Ẵu���b�N�j

#define	SVLD_SECT_FSET		(0)			//�Z�N�^�ُ픻��t���O���샂�[�h�i�Z�b�g�j
#define	SVLD_SECT_FCLR		(1)			//�Z�N�^�ُ픻��t���O���샂�[�h�i���Z�b�g�j
#define	SVLD_SECT_FCHK		(2)			//�Z�N�^�ُ픻��t���O���샂�[�h�i�`�F�b�N�j

#define	SVLD_BYTE_WSIZE1	(SVLD_DATA_SIZE + 4)//�o�C�g�P�ʏ������݃T�C�Y�P�iID�O�܂Łj
#define	SVLD_BYTE_WSIZE2	(3 + 4)				//�o�C�g�P�ʏ������݃T�C�Y�Q�iID�㔼���j

typedef struct {

u8		data[SVLD_DATA_SIZE];	//�Z�[�u�f�[�^

u16		blocknum;	//�u���b�N�i���o�[
u16		sumcheck;	//�T���`�F�b�N�f�[�^
u32		save_id;	//�Z�[�u���ʃf�[�^
u32		save_count;	//�Z�[�u�J�E���^�[�i�����j

}SVLDSYS_DATA;

typedef struct {

SVLDSYS_DATA	svdata;

}SVLDSYS_WORK;

typedef struct {

u8*		dat_adr;	//�f�[�^�|�C���^
u16		size;		//�f�[�^�T�C�Y

}SVLDSYS_BLOCK_TABLE;

typedef struct {

u8		data[SVLD_DATA_SIZE];	//�Z�[�u�f�[�^

u16		sumcheck;	//�T���`�F�b�N�f�[�^
u32		save_id;	//�Z�[�u���ʃf�[�^

}EXTRA_DATA;

GLOBAL	SVLDSYS_WORK*	svld;

GLOBAL	void	EraseBackUpData(void);
GLOBAL	void	SaveLoadSysInit(void);
GLOBAL	u8		AGBSaveSys(u16 mode,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		AGBLoadSys(u16 mode,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		SaveLoadSysSet(u16 sv_blocknum,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		SaveLoadSysGet(u16 ld_blocknum,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		ExtraDataLoad(u8 sectnum,u8* dat_adr,u16 size);
GLOBAL	u8		ExtraDataSave(u8 sectnum,u8* dat_adr,u16 size);
GLOBAL	u8		SaveLoadSysSectFlagCont(u8 mode,u8 sector_num);
GLOBAL	u8		AGBSaveSysSioInit(SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		AGBSaveSysSioInitNoMirror(SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		AGBSaveSysSioDiv(u16 sect_max,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		AGBSaveSysSioByte1(u16 sect_max,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		AGBSaveSysSioByte2(u16 sect_max,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
GLOBAL	u8		AGBSaveSysSioByte2Ex(u16 sect_max,SVLDSYS_BLOCK_TABLE* svldadr_tbl);
#undef GLOBAL

