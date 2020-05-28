#ifndef __P_STATUS_H__
#define __P_STATUS_H__


//=========================================================================
//	�O���[�o���ϐ�
//=========================================================================
extern u8 PokeStatusBuf;	//�ð����ʂ�����Ƃ��ɍŌ�ɉ��Ԗڂ����Ă�����������


//=========================================================================
//	�f�[�^
//=========================================================================

//=========================================================================
//	�}�N���֐�
//=========================================================================

//----------------------------------------------------------
//	16bit�^��work[0]��work[+1]��*adrs��Ă���
//----------------------------------------------------------
#define SetWork_Adrs(pWork, pAdrs)				\
{												\
	((u16*)pWork)[0] = (u16)((u32)pAdrs);		\
	((u16*)pWork)[1] = (u16)((u32)pAdrs >> 16);	\
}
//----------------------------------------------------------
//	*adrs��work[0],[1]�ɓ����Ă�����ڽ��Ă���(work��16�ޯČ^)
//----------------------------------------------------------
#define SetAdrs_Work(pWork, pAdrs) 				\
	pAdrs = (void*)((0xffff & ((u16*)pWork)[0]) | (((u16*)pWork)[1] << 16))


//=========================================================================
//	�萔��`
//=========================================================================
#define ST_POKE_X	40		//�߹�ݍ��W
#define ST_POKE_Y	64
#define ST_POKE_PRI	5		//������ײ��è

//-- adrs_type�̃t���O add 2001.08.02(��) --//
#define PP_TYPE		0		//PokemonParam�^
#define PSP_TYPE	1		//PokemonPasoParam�^

enum{
	LIST_ST = 0,	// �|�P�������X�g"�悳���݂�"
	LIST_WAZA,		// �|�P�������X�g"������킴"

	NEW_WAZA,		// �Z���o����
	DEL_WAZA,		// �Z�Y�ꂶ������
	BTL_WAZA,		// �퓬"������킴"

	BOX_ST,			// �{�b�N�X"�悳���݂�"
	BOX_WAZA,		// �{�b�N�X"������킴"
};

// �R���f�B�V����
enum {
	ST_NORMAL = 0,	// ����
	ST_POISON,		// ��
	ST_PARALYSIS,	// ���
	ST_SLEEP,		// ����
	ST_ICE,			// �X
	ST_BURN,		// �Ώ�
	ST_POKERUS,		// �|�P���X
	ST_HINSI,		// �m��
};

//----------------------------------------------------------
//	��^���b�Z�[�W���W
//----------------------------------------------------------

//	���������鍶�@-------------------------------
#define ST_MONSNO_X				1
#define ST_MONSNO_Y				2
#define ST_LV_X					1
#define ST_LV_Y					12
#define	ST_ZYOUTAI_X			1
#define	ST_ZYOUTAI_Y			17
#define	ST_CONDITION_X			( 6 * 8 + 16 )
#define	ST_CONDITION_Y			( 18 * 8 )
#define ST_NAME02_X				5
#define ST_NAME02_Y				12

//	����������E�P�@-----------------------------
#define ST_OYA_X				11
#define ST_OYA_Y				4
#define ST_OYANAME_X			14
#define ST_OYANAME_Y			4
#define ST_ID_X					22
#define ST_ID_Y					4
#define ST_TYPE_X				11
#define ST_TYPE_Y				6
#define ST_SP_X					11
#define ST_SP_Y					9
#define ST_MEMO_X				11
#define ST_MEMO_Y				14

//	����������E�Q�@-----------------------------
#define ST_ITEM_X				11
#define ST_ITEM_Y				4
#define ST_RIBBON_X				21
#define ST_RIBBON_Y				4
#define	PARAM_GX				11
#define	PARAM_GY				7
#define	ST_EXP_X				11
#define	ST_EXP_Y				14

//	�g����Z���@---------------------------------
#define ST_NAME01_X				1
#define ST_NAME01_Y				1
#define	WAZA_PARAM_GX			2
#define	WAZA_PARAM_GY			15

//	�g����Z�E�@---------------------------------
#define ST_WAZA_X				15
#define ST_WAZA_Y				4
#define WAZA_INFO_X				11
#define WAZA_INFO_Y				15
#define ST_WAZA_INTERVAL		2

//	�R�����g�@-----------------------------------
#define	ST_COMMENT_LX			1
#define	ST_COMMENT_RX			0x18
#define	ST_COMMENT_Y			0

//	�y�[�W�؂�ւ��J�[�\���@---------------------
#define	ST_PAGE_CURSOR_LX		( 80 + 4 )
#define	ST_PAGE_CURSOR_RX		( 224 + 8 )
#define	ST_PAGE_CURSOR_Y		( 72 + 8 )

//	�|�P�����؂�ւ��J�[�\���@-------------------
#define	ST_POKE_CURSOR_X		( 32 + 8 )
#define	ST_POKE_CURSOR_UY		( 0 + 8 )
#define	ST_POKE_CURSOR_DY		( 128 + 8 )

//	�Z�I���J�[�\���@-------------------
#define	ST_WAZA_CURSOR_X		10
#define	ST_WAZA_CURSOR_Y		4

//	�{�b�N�X�}�[�N�@-------------------
#define	ST_MARK_X	60
#define	ST_MARK_Y	26

//	�{�[���@---------------------------
#define	P_ST_MONS_BALL_X	16
#define	P_ST_MONS_BALL_Y	120


// �A�N�^�[No
enum {
	ST_TYPE_1 = 0,		// �����P 
	ST_TYPE_2,			// �����Q
	ST_TYPE_3,			// �����R
	ST_TYPE_4,			// �����S
	ST_TYPE_5,			// �����T

	POKE_CURSOR_U,		// �|�P�����؂�ւ��J�[�\����
	POKE_CURSOR_D,		// �|�P�����؂�ւ��J�[�\����

	PAGE_CURSOR_L,		// �y�[�W�؂�ւ��J�[�\����
	PAGE_CURSOR_R,		// �y�[�W�؂�ւ��J�[�\���E

	WAZA_CURSOR_L,		// �Z�I���J�[�\����
	WAZA_CURSOR_N1,		// �Z�I���J�[�\����
	WAZA_CURSOR_N2,		// �Z�I���J�[�\����
	WAZA_CURSOR_N3,		// �Z�I���J�[�\����
	WAZA_CURSOR_N4,		// �Z�I���J�[�\����
	WAZA_CURSOR_N5,		// �Z�I���J�[�\����
	WAZA_CURSOR_N6,		// �Z�I���J�[�\����
	WAZA_CURSOR_N7,		// �Z�I���J�[�\����
	WAZA_CURSOR_N8,		// �Z�I���J�[�\����
	WAZA_CURSOR_R,		// �Z�I���J�[�\���E

	WAZA_CURSOR_DL,		// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DN1,	// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DN2,	// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DN3,	// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DN4,	// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DN5,	// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DN6,	// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DN7,	// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DN8,	// �Z�I���J�[�\�����i�_�~�[�j
	WAZA_CURSOR_DR,		// �Z�I���J�[�\���E�i�_�~�[�j

	ST_IZYOU,			// �X�e�[�^�X�ُ�

	ST_ACT_END,
};


//=========================================================================
//	�O���֐��錾
//=========================================================================
extern void PokeStatusCall2( void *, u8, u8, pFunc, u8 );
extern void PokeNewWazaSet( void *, u8, u8, pFunc, u16 );
extern u8	GetNewWazaSelectPos(void);
extern u8	GetPokeConditionParam( u32 con );
extern u8	GetPokeCondition( PokemonParam * pp );


//=========================================================================
//	UserWork�ɽð����ʂŎg�p���鼽��ܰ���؂�
//=========================================================================
//-- ���э\����	 ��UserWork��ɐ؂�̂�4�޲ċ��E�͂���܂�l���ĂȂ� --//
typedef struct{

	void * ppArray;		// �߹�����Ұ��̑匳�̱��ڽ
	pFunc  ReturnAdrs;	// �ð����ʂ��甲�����Ƃ��ɾ�Ă���֐���ۑ�

	u8	type;			// �X�e�[�^�X��ʂ̃^�C�v�i�Ă΂ꂽ�ꏊ�j

	u8	put_no;			// ���݉��Ԗڂ��߹�݂����Ă��邩
	u8	poke_max;		// ���Ԗڂ܂��߹�݂��ް������邩
	u8	page;			// ���ݕ\�����Ă���ð�����߰��(���z����)
	u8	pokeact;		// �\�����Ă�|�P������ActNo
	u8	ballact;		// �߂܂����{�[����ActNo
	u8	adrs_type;		// PP_ARRAY�̱��ڽ���(PP_TYPE:PokemonParam, PSP_TYPE:PokemonPasoParam)
	u8	id;				// Ҳ���������No
	PokemonParam pp;	// �߹�����Ұ����������(�����]���p�ɒǉ� 2002.03.15(��))
	u8	div_count;		// �����]���p����(sys.InitSeqWork�����ł͑���Ȃ����Ŏg�p)

	u8	page_init;		// �����y�[�W�ԍ�
	u8	page_max;		// �ŏI�y�[�W�ԍ�
	u8	page_loop;		// �y�[�W���[�v		0 = END, 1 = LOOP

	u8	change_lock;	// �|�P�����؂�ւ�����		0 = change, 1 = lock

	u8	waza_pos;		// �I�𒆂̋Z�̈ʒu
	u8	waza_pos2;		// ����ւ���Z
	u8	waza_lock;		// ����ւ��֎~

	u16	new_waza;		// �V�K�̋Z�ԍ�

	u8	comment1;		// �������b�Z�[�W�ԍ��i���j
	u8	comment2;		// �������b�Z�[�W�ԍ��i�E�j

	u8	bg_flg;			// BG1��BG2�̕\���D��@0 = BG1 , 1 = BG2

	void (*PokeTask)(u8);	// �|�P�����؂�ւ����̃^�X�N��ۑ�
//	void (*WazaTask)(u8);	// �Z�I����ʂ̃^�X�N��ۑ�

}P_StatusSys;

#define P_STATUS_USERADRS	0x18000		//���э\���̂Ƃ��Ďg�p����UserWork�̓Y����

//-- ���э\���̂ɱ������鎞�Ɏg�p����}�N�� --//
#define P_SYSTEM	((P_StatusSys*)&UserWork[P_STATUS_USERADRS])

#define	P_ST_ACT	( &UserWork[ P_STATUS_USERADRS+0x2000 ] )	// �A�N�^�[No�i�[�ꏊ


#endif

