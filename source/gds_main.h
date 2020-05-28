//=========================================================================================
//									�閧��n�O�b�Y����
//						base_gds.c �𕪊�	2002/02/07 by nakahiro
//=========================================================================================
#ifndef	GDS_MAIN_H
#define GDS_MAIN_H

//=========================================================================================
//									�V���{����`
//=========================================================================================
#define	GOODS_MAX		32		// �O�b�Y�����ő吔

#define	YESNO_WINX		20
#define	YESNO_WINY		8

#define	MARK_OBJ_PAL	6		// �}�[�N��OBJ�p���b�g�ԍ�

// OBJ�T�C�Y
#define	size_16x16		0
#define	size_32x16		1
#define	size_48x16		2
#define	size_64x32		3
#define	size_32x32		4
#define	size_16x32		5
#define	size_16x48		6
#define	size_32x64		7
#define	size_48x48		8
#define	size_48x32		9

// ���悤�����^�X�N���[�N�ԍ�
#define	CELL_POSX		0		// �Z�����������ꏊ
#define	CELL_POSY		1		// 
#define	FADE_WORK		2		// �t�F�[�h�����ԍ�
#define	HERO_POSX		3		// ��l���̈ʒu
#define	HERO_POSY		4		// 
#define	GOODS_H			5		// �O�b�Y��
#define	GOODS_V			6		// �O�b�Y����
#define	RETURN_ADR_H	8
#define	RETURN_ADR_L	9
#define	BUTTON_FLG		10		// �{�^���̃t���O
#define	SCENE_NO		11		// �V�[���ԍ�
#define	MENU_NO			12		// ���j���[�ԍ�
#define	NEXT_GG			13		//

// �O�b�Y�^�C�v
#define FLOOR			0
#define WALK			1
#define CORNER			2
#define WALL			3
#define DESK_TOP		4

// �ۑ��ꏊ
enum {
	GOODS_DESK_FOLDER = 0,	// ����
	GOODS_CHAIR_FOLDER,		// ����
	GOODS_PLANT_FOLDER,		// ���A��
	GOODS_ETC_FOLDER,		// ���炭��
	GOODS_MAT_FOLDER,		// �}�b�g
	GOODS_POSTER_FOLDER,	// �|�X�^�[
	GOODS_DOLL_FOLDER,		// �ʂ������
	GOODS_CUSHION_FOLDER,	// �N�b�V����
};

#define	GOODS_NAME_SIZE		10
#define	GOODS_NAME_LEN		( GOODS_NAME_SIZE + EOM_SIZE )


// �O�b�Y�f�[�^
typedef struct {
	const u8    id;						// �O�b�Y�ԍ�
	const u8	name[ GOODS_NAME_LEN ];	// �O�b�Y��
	const u8	type;					// �^�C�v
	const u8	size;					// �T�C�Y
	const u8	box;					// �ۑ��ꏊ
	const u16	price;					// �l�i
	const u8 *  msg;					// �������b�Z�[�W
	const u16 *	cell;					// �g�p�Z���f�[�^
}GoodsData;

// �O�b�YOBJ�f�[�^
typedef struct {
	const GoodsData	* goods;	// �O�b�Y�f�[�^
	u16		new_cell[ 64 ];		// �Z���쐬�ꏊ
	u8		CharData[ 64*8*4 ];	// �L�����W�J�ꏊ
	u16		PalData[ 16 ];		// �p���b�g�W�J�ꏊ
}GoodsOBJ;

// BG�Z������OBJ���쐬���邽�߂̃e�[�u��
typedef struct {
	const u8 * ptn;		// �����̔z��ԍ�
	const u8 * num;		// �Q�Ɛ�̔z��ԍ�
	const u8 * pls;		// �Z�����̃L�����ԍ�
	u8	loop;			// ���[�v��
}BgObjTbl;

// �T�C�Y���Ƃ̃f�[�^
typedef struct {
	u8	type;		// OBJ�`��
	u8	size;		// OBJ�T�C�Y
	u8	posX;		// ����Y���W
	u8	posY;		// ����Y���W
}GoodsSizeData;

// �������O�b�Y�̃f�[�^
typedef struct {
	u8	num;
	u8	Xsize;
	u8	Ysize;
	u16	flg;
}GetGoodsData;

//=========================================================================================
//									�v���g�^�C�v�錾
//=========================================================================================
extern void RearrangeInit2(u8);				// ���j���[�I�������ݒ�
extern void RearrangeTaskInit(u8);			// ���j���[�I�������ݒ�
extern void RearrangeTaskMain(u8);			// ���j���[�I��
extern void RaarrangeParamInt(void);
extern void ReturnGoodsSelectWin(u8);		// �O�b�Y�I���E�B���h�E�֖߂�
extern void GoodsSelectInit(u8);			// �O�b�Y�I�������ݒ�
extern void GoodsSelectParamInit(u8);
extern void GoodsSelectTask(u8);			// �O�b�Y�I��
extern void SelectGoodsParamInit(u8);		// �O�b�Y�I��p�p�����[�^�ݒ�
extern void DelGoodsWindow(void);			// �O�b�Y�E�B���h�E�폜
extern void DelGoodsTypeInfoWin(void);		// ��ޖ��E�O�b�Y�����E�B���h�E����
extern u8	GoodsPutCheck(void);			// �I�������O�b�Y���u����Ă��邩
extern void SelectGoodsPutNot(u8);			// �u����Ă���
extern void SetPutGoodsNo(u8);				// �u����Ă���O�b�Y���L��
extern void NoGoods(u8);					// �O�b�Y���������Ă��Ȃ��Ƃ�
extern void NoGoods2(u8);					// �O�b�Y���������Ă��Ȃ��Ƃ�
extern void DelGoodsParamSet(u8);			// �O�b�Y���������Ƃ��̃p�����[�^�Đݒ�
extern void GoodsFuncGlobalPosition(u8);	// ������ʒu�ݒ�
extern void SetHeroGlobalPosition(u8);		// �Z�������������Ď�l���𒆐S��
extern void DrawGoodsType( u8 id );			// ��ޕ\��
extern void GoodsTypeSelectInit( u8 id );	// ��ޑI�������ݒ�
extern void MyRoomRearrangeInit( u8 id );	// ����͗l�ւ����������ݒ�

extern void TradeOyajiMyGoodsSelectInit( u8 id );	// �������₶�E���ڑI����
extern void GetGoodsTypeStr( u8 * str, u8 no );		// �������₶�E���ڕ�����擾


//=========================================================================================
//									�O���[�o���ϐ�
//=========================================================================================
extern const GoodsData base_goods[];
extern u8	goods_max;				// �����O�b�Y��
extern u8	set_goods[];			// �u����Ă�O�b�Y���L���i��n�j
extern u8	set_room[];				// �u����Ă�O�b�Y���L���i�����̕����j
extern u8	select_goods_pos;		// pos + scr
extern u8	type_pos;				// ��ނ̃J�[�\���ʒu
extern u8	mark_act[];				// �z�u�ς݃A�N�^�[�ԍ�

extern u8 * GoodsType;			// �I������O�b�Y�̃|�C���^


#endif	// GDS_MAIN_H
