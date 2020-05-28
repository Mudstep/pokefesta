//=========================================================================================
//					bag,fd_shop,base,f_pmlist�ȂǂŎg�p���Ă���֗��Ȋ֐�
//									2002/01/09 by nakahiro
//=========================================================================================
#ifndef	ITEMTOOL_H
#define ITEMTOOL_H

// 2�������f�[�^
typedef struct {

	void (*set_func)(u8);		//����
	void (*cancel_func)(u8);	//�L�����Z��

}SELECT_ITEM;

// LR Mode Key Check
enum {
	NOT_LR_MODE = 0,	// Not LR Mode / Not LR Button
	LRM_L,				// L Button Press
	LRM_R,				// R Button Press
};

// �}�[�N�A�N�^�[�ԍ�
enum {
	MARK_ACT_U_CURSOR = 0,
	MARK_ACT_D_CURSOR,
	MARK_ACT_L_CURSOR,
	MARK_ACT_R_CURSOR,
	MARK_ACT_MARK1,
	MARK_ACT_MARK2,

	MARK_ACT_MAX,
};

extern u8	LR_TrgCheck(void);
extern u8	LR_RepeatCheck(void);
extern void BGDataInit(void);							// BG�f�[�^������
extern void SetTalkMsg(u8,const u8 *,void *,u16);		// ���胁�b�Z�[�W�Z�b�g
extern void SelectToolInit(u8,const SELECT_ITEM *);		// �͂��E����������
extern void PalChangeMojiPut( const u8 *, u8, u8, u8 );	// �w��p���b�g�Ńt�H���g�\��
extern void PalChangeNumPut( u32, u8, u8, u8, u8, u8 );	// �w��p���b�g�Ő��l�\��
extern void PalChangeCursorPut( u8, u8, u8 );			// �w��p���b�g�̃J�[�\����\��
extern void ItemToolAGBRegInit(void);					// ���W�X�^������
extern void InitHVBlankFunc(void);						// HVBlank������
extern void MakeRAMKinomiName( u8 * str );				// RAM�؂̎����O�쐬
extern u8	ItemToolSioCheck(void);						// �ʐM�`�F�b�N
extern void ScrActInit(void);							// �A�N�^�[�ԍ�������
extern void MarkActInit( u8 * act, u8 max );			// �A�N�^�[�ԍ�������
extern void AddMarkActor( u8 id, u8 * act, s16 x, s16 y );	// �}�[�N�A�N�^�[�Z�b�g
extern void DelMarkActor( u8 * act );						// �}�[�N�A�N�^�[�폜
extern void DelMarkActorAll( u8 * act, u8 max );
extern void AddScrCurActor( u8 id, s16 x, s16 y );	// �X�N���[���J�[�\���A�N�^�[�Z�b�g
extern void BanishScrCurActor( u8 id, u8 flg );		// �X�N���[���J�[�\���A�N�^�[�\���ؑ�
extern void DelScrCurActor( u8 id );				// �X�N���[���J�[�\���A�N�^�[�폜
extern void AddMarkPal(void);						// �}�[�N�p���b�g�Z�b�g
extern void DelMarkPal(void);						// �}�[�N�p���b�g�폜
extern void ScrCurActAnmStop( u8 id );				// �X�N���[���J�[�\���A�j���X�g�b�v
extern void ScrCurActAnmPlay( u8 id );				// �A�j���ĊJ
extern void ScrCurActAnmInit( u8 id );				// �A�j���N���A
extern void ScrCurActAnmStopInit( u8 id );			// �A�j���X�g�b�v���N���A
extern void ScrCurActPrioritySet( u8 id, u8 pri );	// �X�N���[���J�[�\���\���D��

extern u8 ChangeRoomRAMKinomiCheck( u16 item );		// ���������ł́u�i�]�̂݁v�`�F�b�N
extern u8 SioMailNotCheck( u16 item );				// �ʐM�����[�������ĂȂ��`�F�b�N

extern u8 ScrActNo[];	// �}�[�N�A�N�^�[�ԍ�



#endif	// ITEMTOOL_H
