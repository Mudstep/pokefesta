//=========================================================================
//	
//	svld_ini.h
//	�Z�[�u�֘A(�|�P�����`�f�a)
//
//=========================================================================
#undef GLOBAL
#ifdef __SVLD_INI_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
#define	SVLD_ID				(0x08012025)	//�w�b�_�X�e�[�^�X��`�i�Q�[���F�؂h�c�j
#define	SVLD_BLOCK_COUNT	(14)		//�Z�[�u�u���b�N��
#define	SVLD_SECTOR_COUNT	(30)		//�Z�[�u�Z�N�^��
#define	SVLD_SECTCHECK_MAX	(1)			//�Z�[�u�����i�m�f����c��Z�N�^���j

#define	SVLD_RET_UPD_NULL	(0)		//�A�b�v���[�h�Ȃ�
#define	SVLD_RET_UPD_OK		(1)		//�A�b�v���[�h���� ����
#define	SVLD_RET_UPD_FULL	(2)		//�A�b�v���[�h���� �������ݕs��
#define	SVLD_RET_UPD_NG		(3)		//�A�b�v���[�h���� ���s

#define	SVLD_BLOCK_SIZE		(0xf80)		//�Z�N�^������̃Z�[�u�f�[�^�T�C�Y

#define	POKEAGB_SVALL		(0)
#define	POKEAGB_SVFLD		(1)
#define	POKEAGB_SVMYD		(2)
#define	POKEAGB_SVDENDOU	(3)
#define	POKEAGB_SVCLRALL	(4)
#define	POKEAGB_SVCLRDENDOU	(5)

#define	POKEAGB_LDALL		(0)
#define	POKEAGB_LDDENDOU	(3)

GLOBAL	u8		PokeAGBSave(u8 mode);
GLOBAL	u8		PokeAGBSaveTrans(u8 mode);
GLOBAL	u8		PokeAGBSaveTransWait(void);
GLOBAL	u8		PokeAGBLoad(u8 mode);
GLOBAL	u8		UpLoadDataGet(u8* upld_p);
GLOBAL	u8		UpLoadDataSet(void);
GLOBAL	u16		SaveDataCheckInfo;
GLOBAL	u16		UpDataCheckInfo;
GLOBAL	pFunc	SvRecoverReturnProc;		// �Z�N�^����������ɕ��A�����̊֐��|�C���^
GLOBAL	u8		PokeAGBSaveSioDivInit(void);
GLOBAL	u8		PokeAGBSaveSioDiv(void);	//�I��(1)���쒆(0)
GLOBAL	u8		PokeAGBSaveSioDiv1(void);
GLOBAL	u8		PokeAGBSaveSioDiv2(void);
GLOBAL	u8		PokeAGBSaveFieldDivInit(void);
GLOBAL	u8		PokeAGBSaveFieldDiv(void);	//�I��(1)���쒆(0)

#undef GLOBAL

