//=======================================================================================
//								�G�X�J���[�^�[BG�A�j��
//								2002/04/04 by nakahiro
//=======================================================================================
enum {
	ESCALATOR_DOWN = 0,
	ESCALATOR_UP,
};

extern void AddEscalatorBGAnm( u8 no );		// �A�j���J�n	no : 0 = DOWN, 1 = UP
extern void DelEscalatorBGAnm(void);		// �A�j���I��
extern u8	CheckEscalatorBGAnm(void);		// �A�j���`�F�b�N TRUE = ������ FALSE = �I��
