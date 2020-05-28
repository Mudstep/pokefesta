//-----------------------------------------------------------
//    �A�N�^�[�f�[�^
//-----------------------------------------------------------
static const ActOamData NameOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		1,		//u32 Shape:1;            // �n�a�i�`��(�������`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		2,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x16  3����64x32)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		1,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};

static const actAnm NameAnm0[]={
		{OBJ16_32x16*0,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm NameAnm1[]={
		{OBJ16_32x16*1,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm NameAnm2[]={
		{OBJ16_32x16*2,5,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm NameAnm3[]={
		{OBJ16_32x16*3,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm NameAnm4[]={
		{OBJ16_32x16*4,5,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm NameAnm5[]={
		{OBJ16_32x16*5,5,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm *const NameAnmTbl[]={
		NameAnm0,
		NameAnm1,
		NameAnm2,
		NameAnm3,
		NameAnm4,
		NameAnm5,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const u16 PM_Print_Palette[16] = {
0x0000,		//�����F
0x2529,		//��
0x001F,		//��
0x03E0,		//��
0x7C00,		//��
0x03FF,		//���F
0x7FE0,		//���F
0x7C1F,		//��
//0x675a,0x0000,0x0000,0x0000,0x6e93,0x0000,0x76d5,
0x675a,0x0000,0x0000,0x0000,0x7fff,0x0000,0x39ce,		// 0    111 01    10 110    1 0101,
0x7FFF,		//��
};


static const u16 Test_Palette[16] = {
0x0000,		//�����F
0x2529,		//��
0x001F,		//��
0x03E0,		//��
0x7C00,		//��
0x03FF,		//���F
0x7FE0,		//���F
0x7C1F,		//��
//0x675a,0x0000,0x0000,0x0000,0x6e93,0x0000,0x76d5,
0x39ce,0x0000,0x0000,0x0000,0x7fff,0x0000,0x39ce,		// 0    111 01    10 110    1 0101,
0x7FFF,		//��
};


#define EXPLAIN_CELL	(6*4)			//6�Z���g���Ă��āA1�Z���ɂ�4�������邩��

static u8 *NameCellBuf[13]={};


static const CellData NameActCell[]={
	{&UserWork[0x1d000+0x20*4*2* 0],0x20*4*2,200},		//�����̖��O�Z���P
	{&UserWork[0x1d000+0x20*4*2* 1],0x20*4*2,201},		//			�Z���Q
	{&UserWork[0x1d000+0x20*4*2* 2],0x20*4*2,202},		//			�Z���R
	{&UserWork[0x1d000+0x20*4*2* 3],0x20*4*2,203},		//����̖��O�Z���P
	{&UserWork[0x1d000+0x20*4*2* 4],0x20*4*2,204},		//			�Z���Q
	{&UserWork[0x1d000+0x20*4*2* 5],0x20*4*2,205},		//			�Z���R
	{&UserWork[0x1d000+0x20*4*2* 6],0x20*4*2,206},		//�u��߂�v
	{&UserWork[0x1d000+0x20*4*2* 7],0x20*4*2,207},		//�������Z���P
	{&UserWork[0x1d000+0x20*4*2* 8],0x20*4*2,208},		//�������Z���Q
	{&UserWork[0x1d000+0x20*4*2* 9],0x20*4*2,209},		//�������Z���R
	{&UserWork[0x1d000+0x20*4*2*10],0x20*4*2,210},		//�������Z���S
	{&UserWork[0x1d000+0x20*4*2*11],0x20*4*2,211},		//�������Z���T
	{&UserWork[0x1d000+0x20*4*2*12],0x20*4*2,212},		//�������Z���U

#if 0
	{NameCellBuf[0],0x20*4*2,200},		//�����̖��O�Z���P
	{NameCellBuf[1],0x20*4*2,201},		//			�Z���Q
	{NameCellBuf[2],0x20*4*2,202},		//			�Z���R
	{NameCellBuf[3],0x20*4*2,203},		//����̖��O�Z���P
	{NameCellBuf[4],0x20*4*2,204},		//			�Z���Q
	{NameCellBuf[5],0x20*4*2,205},		//			�Z���R
	{NameCellBuf[6],0x20*4*2,206},		//�u��߂�v
	{NameCellBuf[7],0x20*4*2,207},		//�������Z���P
	{NameCellBuf[8],0x20*4*2,208},		//�������Z���Q
	{NameCellBuf[9],0x20*4*2,209},		//�������Z���R
	{NameCellBuf[10],0x20*4*2,210},		//�������Z���S
	{NameCellBuf[11],0x20*4*2,211},		//�������Z���T
	{NameCellBuf[12],0x20*4*2,212},		//�������Z���U
#endif
};

static const PalData NameActPal = 
{
	PM_Print_Palette,
	4925,
};
//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

const	actHeader ActNameHeader={
		200,
		4925,
		&NameOamData,
		NameAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		DummyActMove,
};



//-----------------------------------------------------------
//    �A�N�^�[�f�[�^
//-----------------------------------------------------------
static const ActOamData CursorOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		1,		//u32 Shape:1;            // �n�a�i�`��(�������`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		3,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x16  3����64x32)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		1,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};

static const actAnm cursorAnm0[]={
		{OBJ16_64x32*0,5,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm cursorAnm1[]={
		{OBJ16_64x32*1,5,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm *const CursorAnmTbl[]={
		cursorAnm0,
		cursorAnm1,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const CellData CursorActCell[]={
	{kk_cusor_Character,0x20*(8*4*2),300},
};

static const PalData CursorActPal = 
{
	&kk_bg_Palette[0x30],		//kk_bg.acl�̂S��ڂ̓A�N�^�[�p�̃p���b�g
	2345,
};

//-------------------------------------------------------------
//		�A�N�^�[�V�[�P���X
//-------------------------------------------------------------

const	actHeader CursorHeader={
		300,
		2345,
		&CursorOamData,
		CursorAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		DummyActMove,
};



//-------------------------------------------------------------
//  �������񎞒ʐM�p�R�}���h��`	
//-------------------------------------------------------------

#define KOUKAN_SAVE_TIMING		(0xAAAA)		//�Z�[�u����ꏊ��ʒm����				�i�e�j
#define KOUKAN_WAIT_IN			(0xAABB)        //�ʐM�ҋ@��Ԃɂ͂���܂����B			�i�q�j
#define KOUKAN_NEXT_TERM		(0xDDDD)		//��������̍ŏI�m�F�󋵂ɂ���܂�	�i�e�j
#define KOUKAN_SEND_YES			(0xBBBB)        //�����ɑ΂��āu�͂��v��������܂�		�i�q�j
#define KOUKAN_SEND_NO			(0xBBCC)        //�����ɑ΂��āu�������v�𑗂�܂�		�i�q�E�e�j
#define KOUKAN_EXCHANGE			(0xCCDD)        //�����̉��ԂƑ���̉��Ԃ��������܂��B	�i�e�j
#define KOUKAN_STOP				(0xDDEE)        //�����̓X�g�b�v						�i�e�j
#define KOUKAN_END_SELECT		(0xEEAA)		//�����I����I������
#define KOUKAN_END_OK			(0xEEBB)		//�����I�����m�F���ꂽ
#define KOUKAN_END_STOP			(0xEECC)		//�����I�����L�����Z�����ꂽ


//-------------------------------------------------------------
//  kw->wait_flag[]�Ɋi�[����l
//-------------------------------------------------------------
#define SELECT_KOUKAN			(1)
#define SELECT_END				(2)

//-------------------------------------------------------------
//  kw->ok_flag[]�Ɋi�[����l
//-------------------------------------------------------------
#define KOUKAN_YES				(1)
#define KOUKAN_NO				(2)
