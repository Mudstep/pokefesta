//==========================================================
//	�ʐM�^�X�N�p��`
//==========================================================

#define	REQ_WORK0		10
#define	REQ_SEQ_NO		11
#define	REQ_BUF_OVER	12
#define	REQ_BUF_WAIT	13
#define	REQ_BUF_SET		14
#define	REQ_BUF_GET		15
#define	TUUSHIN_TASK_BUF_SIZE	0x1000
#define	TUUSHIN_TASK_BUF_START	0x014000

#define	RECV_BUF_OVER	12
#define	RECV_BUF_WAIT	13
#define	RECV_BUF_SET	14
#define	RECV_BUF_GET	15
#define	RECV_TASK_BUF_START		0x015000

//==========================================================

#define	BSS_SERVER		0
#define	BSS_CLIENT		1
#define	BSS_CLIENT_BIT	2

#define	BSS_CLIENT_MINE		0
#define	BSS_CLIENT_ENEMY	1
#define	BSS_CLIENT_MINE2	2
#define	BSS_CLIENT_ENEMY2	3

#define	BSS_SELMONSNO		0
#define	BSS_SELMONS0		0x01
#define	BSS_SELMONS1		0x02
#define	BSS_SELMONS2		0x04
#define	BSS_SELMONS3		0x08
#define	BSS_SELMONS4		0x10
#define	BSS_SELMONS5		0x20

#define	BSS_DATASTART		3
#define	BSS_DATALOW			3
#define	BSS_DATAHIGH		4

#define	SIDE_MINE			0
#define	SIDE_ENEMY			1

//�֐��h�c�錾

enum{
	BSS_POKEPARA_GET=0,		//�w�肵���|�P�����f�[�^���擾����B			0x00
	BSS_POKEPARA_GET2,		//�w�肵���A�h���X�̃|�P�����f�[�^���擾����B	0x01
	BSS_POKEPARA_PUT,		//�w�肵���|�P�����f�[�^��ύX����B			0x02
	BSS_POKEPARA_PUT2,		//�w�肵���|�P�����f�[�^��ύX����B			0x03
	BSS_POKE_ENCOUNT,		//�|�P�����o��G�t�F�N�g���N������B			0x04
	BSS_POKE_APPEAR,		//�|�P�����o��G�t�F�N�g���N������B			0x05
	BSS_POKE_RETURN,		//�|�P������߂��G�t�F�N�g���N������B			0x06
	BSS_TRAINER_ENCOUNT,	//�g���[�i�[�o��G�t�F�N�g���N������B			0x07
	BSS_TRAINER_APPEAR,		//�g���[�i�[�o��G�t�F�N�g���N������B			0x08
	BSS_TRAINER_RETURN,		//�g���[�i�[��߂��G�t�F�N�g���N������B		0x09
	BSS_POKE_KIZETSU,		//�|�P�������C�₷��G�t�F�N�g���N������B		0x0a
	BSS_POKE_ZENMETSU,		//�|�P�������S�ł����Ƃ��̃G�t�F�N�g���N������B0x0b
	BSS_GET_SUCCESS,		//�|�P�����ߊl�����̃G�t�F�N�g���o���B			0x0c
	BSS_GET_FAIL,			//�|�P�����ߊl���s�̃G�t�F�N�g���o���B			0x0d
	BSS_POKE_LEVELUP,		//�|�P�������x���A�b�v�̃G�t�F�N�g���o���B		0x0e
	BSS_WAZA_EFFECT,		//�w�肵���Z�i���o�[�̃G�t�F�N�g���o���B		0x0f
	BSS_MESSAGE,			//�������f�[�^�𕶎��f�[�^�Ɖ��߂���			0x10
							//���b�Z�[�W�\��������B
	BSS_ALERT_MESSAGE,		//�������f�[�^�𕶎��f�[�^�Ɖ��߂���			0x11
							//���b�Z�[�W�\��������B
	BSS_COMMAND_SELECT, 	//�R�}���h�I���E�B���h�E��\�����āA			0x12
							//�I���V�[�P���X���쓮������B
	BSS_COMMAND_SELECT_COPY,//�R�}���h�I�����w�肳�ꂽClientNo�ƁA			0x13
							//����������B
	BSS_WAZA_SELECT,		//�Z�I���E�B���h�E��\�����āA					0x14
							//�I���V�[�P���X���쓮������B
	BSS_ITEM_SELECT,		//�A�C�e���I���E�B���h�E��\������				0x15
							//�I���V�[�P���X���쓮������B
	BSS_POKE_SELECT,		//�|�P�����I���E�B���h�E��\�����āA			0x16
							//�I���V�[�P���X���쓮������B
	BSS_ESCAPE,				//������G�t�F�N�g���N������B					0x17
	BSS_HP_GAUGE,			//�g�o�Q�[�W�𑝌�����B						0x18
	BSS_EXP_GAUGE,			//�d�w�o�Q�[�W�𑝌�����B						0x19
	BSS_STATUS_SET,			//�w�肳�ꂽ�X�e�[�^�X�ُ�G�t�F�N�g���N������B0x1a
							//�Q�[�W�ɏ�Ԉُ��\������̂���������ɂ��
	BSS_STATUS_EFFECT,		//�w�肳�ꂽ�X�e�[�^�X�ُ�G�t�F�N�g���N������B0x1b
	BSS_STATUS_CLEAR,		//�w�肳�ꂽ�X�e�[�^�X�ُ���N���A����B		0x1c
	BSS_DATA_TRANS,			//�f�[�^�v���ɑ΂��鉞���Ƃ��Ďg�p����B		0x1d
	BSS_DATA_RAM_TRANS,		//�w��A�h���X�iRAM�AVRAM�ACGRAM�j��			0x1e
							//�f�[�^��]������B
	BSS_BGM_TRANS,			//�ȃf�[�^��]������B							0x1f
	BSS_VOICE_TRANS,		//�����f�[�^��]������B						0x20
	BSS_SELECT_RETURN,		//���j���[�őI����������ID��Ԃ��B				0x21
	BSS_POKE_RESHUFFLE,		//�|�P��������ւ����s�������Ƃ�m�点��B		0x22
	BSS_USE_ITEM,			//�A�C�e���g�p��m�点��B						0x23
	BSS_USE_BALL,			//�{�[���g�p��m�点��B						0x24
	BSS_LOOPCOUNT_CLEAR,	//�Z�G�t�F�N�g�p�̃J�E���^�̃N���A				0x25
	BSS_LOOPCOUNT_SET,		//�Z�G�t�F�N�g�p�̃J�E���^�̃Z�b�g				0x26
	BSS_SWITCHFLAG_CLEAR,	//�Z�G�t�F�N�g�p�̃t���O�̃N���A				0x27
	BSS_SWITCHFLAG_SET,		//�Z�G�t�F�N�g�p�̃t���O�̃Z�b�g				0x28
	BSS_POKEDAMAGE_BLINK,	//��Ұ�ނ��󂯂��Ƃ����߹�ݓ_��					0x29
	BSS_GAUGE_OFF,			//�Q�[�W������									0x2a
	BSS_SE_PLAY,			//SE��炷									0x2b
	BSS_ME_PLAY,			//SE��炷									0x2c
	BSS_VOICE_PLAY,			//������炷								0x2d
	BSS_ENCOUNT_EFFECT,		//�G���J�E���g�G�t�F�N�g���Z�b�g				0x2e
	BSS_POKE_SENDOUT,		//�|�P�����J��o���V�[�P���X�𔭓�				0x2f
	BSS_BALL_GAUGE_SET,		//�g���[�i�[�펞�̃{�[���Q�[�W��\��			0x30
	BSS_BALL_GAUGE_OFF,		//�g���[�i�[�펞�̃{�[���Q�[�W���\��			0x31
	BSS_GAUGE_YURE_STOP,	//�g���[�i�[�펞�̃Q�[�W�̂����~�߂�			0x32
	BSS_VANISH_ON_OFF,		//�N���C�A���g���Ǘ����Ă���|�P������VANISH ON/OFF	0x33
	BSS_FIGHT_EFFECT,		//�퓬���̓���̃G�t�F�N�g���o��				0x34
	BSS_TUUSHIN_WAIT,		//�ʐM�ҋ@���̃��b�Z�[�W���o��					0x35
	BSS_CSRPOS_CLEAR,		//�L�����Ă���J�[�\���ʒu�̃N���A				0x36
	BSS_FIGHT_END,			//�ΐ�I����m�点��							0x37
	
	BSS_END_ID,				//�i���o�[�̏I�[�i����ȍ~�ɒǉ��͋֎~�j
};

//�擾�p�����[�^�R�[�h�ꗗ

enum{
	BSS_PARA_FIGHT=0,			//�퓬���ɕK�v�ȃp�����[�^�S��
	BSS_PARA_MONSNO,			//�����X�^�[�ԍ�
	BSS_PARA_ITEM,				//�����A�C�e��
	BSS_PARA_WAZA,				//�Z���ׂ�
	BSS_PARA_WAZA1,				//�Z
	BSS_PARA_WAZA2,				//�Z
	BSS_PARA_WAZA3,				//�Z
	BSS_PARA_WAZA4,				//�Z
	BSS_PARA_PP,				//�Z�|�C���g���Z�|�C���g�J�E���^���ׂ�
	BSS_PARA_PP1,				//�Z�|�C���g
	BSS_PARA_PP2,				//�Z�|�C���g
	BSS_PARA_PP3,				//�Z�|�C���g
	BSS_PARA_PP4,				//�Z�|�C���g
	BSS_PARA_PPCOUNT1,			//�Z�|�C���g�J�E���^
	BSS_PARA_PPCOUNT2,			//�Z�|�C���g�J�E���^
	BSS_PARA_PPCOUNT3,			//�Z�|�C���g�J�E���^
	BSS_PARA_PPCOUNT4,			//�Z�|�C���g�J�E���^
	BSS_PARA_ID,				//�h�c�i���o�[
	BSS_PARA_EXP,				//��{�o���l
	BSS_PARA_HPEXP,				//�g�o�w�͒l
	BSS_PARA_POWEXP,			//�U���͓w�͒l
	BSS_PARA_DEFEXP,			//�h��͓w�͒l
	BSS_PARA_AGIEXP,			//�f�����w�͒l
	BSS_PARA_SPEPOWEXP,			//����U���w�͒l
	BSS_PARA_SPEDEFEXP,			//����h��w�͒l
	BSS_PARA_FRIEND,			//�F�D�l
	BSS_PARA_POKERUS,			//�|�P���X
	BSS_PARA_GET_PLACE,			//�ߊl�ꏊ
	BSS_PARA_GET_LEVEL,			//�ߊl���x��
	BSS_PARA_GET_CASSETTE,		//�ߊl�J�Z�b�g
	BSS_PARA_GET_BALL,			//�ߊl�{�[��
	BSS_PARA_POWERRND,			//�p���[�������ׂ�
	BSS_PARA_HPRND,				//HP�p���[����
	BSS_PARA_POWRND,			//�U���̓p���[����
	BSS_PARA_DEFRND,			//�h��̓p���[����
	BSS_PARA_AGIRND,			//�f�����p���[����
	BSS_PARA_SPEPOWRND,			//����U���̓p���[����
	BSS_PARA_SPEDEFRND,			//����h��̓p���[����
	BSS_PARA_CHRRND,			//������
	BSS_PARA_SUM,				//�`�F�b�N�T��
	BSS_PARA_CONDITION,			//�R���f�B�V����
	BSS_PARA_LEVEL,				//���x��
	BSS_PARA_HP,				//�g�o�c��
	BSS_PARA_HPMAX,				//�g�o�l�`�w
	BSS_PARA_POW,				//�U����
	BSS_PARA_DEF,				//�h���
	BSS_PARA_AGI,				//�f����
	BSS_PARA_SPEPOW,			//����U����
	BSS_PARA_SPEDEF,			//����h���
	BSS_PARA_STYLE,				//�������悳	add 2001.09.19(��) by matsuda
	BSS_PARA_BEAUTIFUL,			//������
	BSS_PARA_CUTE,				//���킢��
	BSS_PARA_CLEVER,			//����
	BSS_PARA_STRONG,			//�����܂���
	BSS_PARA_FUR,				//�щ�
	BSS_PARA_STYLEMEDAL,		//�������悳�M��
	BSS_PARA_BEAUTIFULMEDAL,	//�������M��
	BSS_PARA_CUTEMEDAL,			//���킢���M��
	BSS_PARA_CLEVERMEDAL,		//�����M��
	BSS_PARA_STRONGMEDAL,		//�����܂����M��
};

//�X�e�[�^�X�ُ�i���o�[���X�g
enum{
	BSS_STATUS_NEMURI=0,		//����
	BSS_STATUS_MAHI,			//���
	BSS_STATUS_KONRAN,			//����
};

//����ID�ꗗ
enum{
	BSS_SELECT_RETURN_FIGHT=0,			//��������
	BSS_SELECT_RETURN_ITEM,				//�����b�N
	BSS_SELECT_RETURN_POKE,				//�|�P����
	BSS_SELECT_RETURN_ESCAPE,			//�ɂ���
	BSS_SELECT_RETURN_SAFARI,			//�T�t�@���]�[�����L�̃A�N�V����
	BSS_SELECT_RETURN_BALL,				//�T�t�@���{�[���𓊂���
	BSS_SELECT_RETURN_CUBE,				//�L���[�u
	BSS_SELECT_RETURN_APPROACH,			//�����Â�
	BSS_SELECT_RETURN_SAFARI_ESCAPE,	//�ɂ���
	BSS_SELECT_RETURN_GET_DEMO,			//�ߊl�f�����L�̃A�N�V����
	BSS_SELECT_RETURN_WAZA,				//�Z
	BSS_SELECT_RETURN_LEVELUP,			//���x���A�b�v
	BSS_SELECT_RETURN_B_CANCEL,			//B�{�^���������Ė߂��Ă���
};

//�Z���X�g�I���t���O
enum{
	BSS_WAZA_SELECT_NORMAL=0,		//�ʏ�I��
	BSS_WAZA_SELECT_WAZAWASURE,		//�Z�Y��p�I��
};

//�J�[�\���ʒu�N���A�t���O
enum{
	BSS_CSRPOS_CLEAR_ALL=0,			//�J�[�\���ʒu�N���A�I�[��
	BSS_CSRPOS_CLEAR_COMMAND,		//�J�[�\���ʒu�N���A�R�}���h
	BSS_CSRPOS_CLEAR_WAZA,			//�J�[�\���ʒu�N���A�Z
};
