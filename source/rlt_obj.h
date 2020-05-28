//========================================================================================
//								�~�j�Q�[���E���[���b�g��OBJ
//									2002/03/28 by nakahiro
//									2002/06/11 by iwasawa
//========================================================================================

//=========================================================================================
//									�V���{����`
//=========================================================================================
// �A�N�^�[�ԍ��Q�Əꏊ
enum {
	//BG2�p�I�u�W�F
	RLT_BG2_OBJ = 0,
	RLT_BALL0 = 0,			//ball
	RLT_BALL1,
	RLT_BALL2,
	RLT_BALL3,
	RLT_BALL4,
	RLT_BALL5 = 5,
	RLT_BALL6,
	RLT_BALL7,
	RLT_BALL8,
	RLT_BALL9,
	RLT_BALL10,
	RLT_BALL11 = 5,

	RLT_CTBL = 6,			// �Z���^�[�e�[�u��

	RLT_RPOKE00,			//��]�՗p�|�P����
	RLT_RPOKE01,
	RLT_RPOKE02,
	RLT_RPOKE03,
	RLT_RPOKE04,
	RLT_RPOKE05,
	RLT_RPOKE06,
	RLT_RPOKE07,
	RLT_RPOKE08,
	RLT_RPOKE09,
	RLT_RPOKE10,
	RLT_RPOKE11,

	RLT_BG2_OBJ_MAX = 19,
	RLT_BG1_OBJ = 20,
	
	WIN_COIN = RLT_BG1_OBJ,		//�R�C���\���p�E�B���h�E
	WIN_COIN00,
	WIN_COIN01,
	WIN_COIN02,
	WIN_COIN03,
	
	WIN_ODDS,					//�I�b�Y�\��
	
	WIN_BALL00,					//�{�[���̎c���\��
	WIN_BALL01,
	WIN_BALL02,
	
	WIN_POKE00,					// �q���e�[�u���̃|�P����
	WIN_POKE01,
	WIN_POKE02,
	WIN_POKE03,
	WIN_POKE04,
	WIN_POKE05,
	WIN_POKE06,
	WIN_POKE07,
	WIN_POKE08,
	WIN_POKE09,
	WIN_POKE10,
	WIN_POKE11,
	WIN_POKE12,
	WIN_POKE13,
	WIN_POKE14,
	WIN_POKE15,
	WIN_POKE16,
	WIN_POKE17,
	WIN_POKE18,
	
	RLT_WAKU,			// �g

	RLT_STCELL00,		//�{�[�����������Z���̕\��
	RLT_STCELL01,
	RLT_STCELL02,
	RLT_STCELL03,
	RLT_STCELL04,
	RLT_STCELL05,

	EV_POKE,					// �G�t�F�N�g�p�|�P����
	EV_SHADOW0,					// �G�t�F�N�g�p�I�u�W�F�N�g
	EV_SHADOW1,		 			//
};

//========================================================================================
//									�v���g�^�C�v�錾
//========================================================================================
extern void AddRoulettePokeWinOBJ(void);		// �|�P�����}�[�N�Z�b�g
extern void DelRoulettePokeWinOBJ(void);		// �|�P�����}�[�N�폜
extern void BanishRoulettePokeControl(u8,u8);	// �|�P�����}�[�N�\���ؑ�
extern void AddRotatePokeOBJ(void);				// ���[���b�g�Ղ̃|�P�����Z�b�g
extern void RouletteObjPal(u8);					// ���[���b�gOBJ�p���b�g�ݒ�
extern void AddRouletteWakuOBJ(void);			// �gOBJ��\��
extern void RouletteWakuOBJBanishControl(u8);	// �\���ؑ�
extern void AddBg1OBJ(void);					// Bg1�p�I�u�W�F

extern void AddBallHoleObj(void);				// ���Z�b�g
extern void AddBallObj(void);					// �{�[���I�u�W�F�N�g�̃Z�b�g
extern void AddSTCellObj(void);					// �e�[�u���p�{�[��OBJ�̃Z�b�g

//�\���R���g���[��
extern void DrawCoinControl(u16 coin);
extern void DrawOddsControl(u8);
extern void DrawRestballControl(u8);
extern void Bg1_DummyActMove(actWork *);
extern void Bg1_ScrollActMove(actWork *);

//�{�[���̓����֘A
extern void G_BallParamClear(void);				// �{�[���̃p�����[�^���N���A
extern void BallMoveAct_ST0(actWork *);			// �{�[���̍ŏ��̓���֐�
extern void BanishSTCellControl( u8 flg,u8 no);
extern void BanishWakuControl(u8 no);

//========================================================================================
//									�O���[�o���ϐ�
//========================================================================================
extern const BET_PARAM		g_rltBet[];
extern const RLT_CELL_PARAM	g_rltCell[];	//�O���[�o���f�[�^�̎Q��
