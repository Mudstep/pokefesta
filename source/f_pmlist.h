
//=========================================================================
//	
//=========================================================================

#ifndef F_PMLIST_H
#define F_PMLIST_H
#undef GLOBAL
#ifdef F_PMLIST_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

#define MENUPOKE_BG_NUMBER	2
#define MENUPOKE_CGX_BLK	2
#define MENUPOKE_SCRN_BLK	15

// ����̎��
enum {
	DUMMY_ITEM = 0,		// �_�~�[
	SKILL_ITEM,			// �Z�}�V��
	EVO_ITEM,			// �i���A�C�e��
};

// �A�C�e���g�p�����̎��
enum {
	PM_ITEM_USE_MODE = 0,			// ����
	PM_ITEM_PUT_MODE,				// ��������
	PM_ITEM_CUBE_USE_MODE,			// �L���[�u������
	PM_ITEM_MAIL_BOX_MODE,			// ���[���{�b�N�X���烁�[������������
	PM_ITEM_USE_ALLDETH_RCV_MODE,	// �S���m���񕜃A�C�e���g�p
};

//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;
extern u8	FPokeNo;	//�I�����ꂽ�|�P����


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);
extern void ReturnFldPokeSora(void);	//�u������Ƃԁv�L�����Z���E�|�P�������X�g�ɕ��A
extern u16 GetHidenMonsNo(void);		// �|�P�����i���o�[��Ԃ�



#undef GLOBAL
#endif	/*F_PMLIST_H*/

