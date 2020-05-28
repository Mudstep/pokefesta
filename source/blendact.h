//-----------------------------------------------------------
//    �u�����h�}�V�[���p�A�N�^�[��`
//-----------------------------------------------------------


static void BlendCursorActMove(actWork *act);
//------------���---------------------

static const ActOamData CursorOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		0,		//u32 Shape:1;            // �n�a�i�`��(�������`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		2,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x16  3����64x32)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		1,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};

static const actAnm cursorAnm0[]={		{OBJ16_32x32*1,5,1,1},
										{ACT_ANMEND,0,0,0}
};

static const actAnm cursorAnm1[]={		{OBJ16_32x32*1,5,0,1},
										{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm2[]={		{OBJ16_32x32*1,5,1,0},
										{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm3[]={		{OBJ16_32x32*1,5,0,0},
										{ACT_ANMEND,0,0,0}
};

static const actAnm cursorAnm0b[]={		{OBJ16_32x32*3,2,1,1},
										{OBJ16_32x32*2,5,1,1},
										{OBJ16_32x32*3,3,1,1},
										{OBJ16_32x32*1,5,1,1},
										{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm1b[]={		{OBJ16_32x32*3,2,0,1},
										{OBJ16_32x32*2,5,0,1},
										{OBJ16_32x32*3,3,0,1},
										{OBJ16_32x32*1,5,0,1},
										{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm2b[]={		{OBJ16_32x32*3,2,1,0},
										{OBJ16_32x32*2,5,1,0},
										{OBJ16_32x32*3,3,1,0},
										{OBJ16_32x32*1,5,1,0},
										{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm3b[]={		{OBJ16_32x32*3,2,0,0},
										{OBJ16_32x32*2,5,0,0},
										{OBJ16_32x32*3,3,0,0},
										{OBJ16_32x32*1,5,0,0},
										{ACT_ANMEND,0,0,0}
};

static const actAnm cursorAnm0n[]={		{OBJ16_32x32*0,5,1,1},
										{ACT_ANMEND,0,0,0}
};

static const actAnm cursorAnm1n[]={		{OBJ16_32x32*0,5,0,1},
										{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm2n[]={		{OBJ16_32x32*0,5,1,0},
										{ACT_ANMEND,0,0,0}
};
static const actAnm cursorAnm3n[]={		{OBJ16_32x32*0,5,0,0},
										{ACT_ANMEND,0,0,0}
};


static const actAnm *const CursorAnmTbl[]={
		cursorAnm0,
		cursorAnm1,
		cursorAnm2,
		cursorAnm3,
		cursorAnm0b,
		cursorAnm1b,
		cursorAnm2b,
		cursorAnm3b,
		cursorAnm0n,
		cursorAnm1n,
		cursorAnm2n,
		cursorAnm3n,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const CellData CursorActCell={
	blendpat_Character+0x20*16*9,0x20*16*4,46545
};

static const PalData BlendPatActPal = 
{
	blendpat_Palette,
	46546,
};

static const PalData CursorActPal = 
{
	(u16*)&blendpat_Palette[16],
	12312,
};

//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------

static const	actHeader CursorHeader={
		46545,
		12312,
		&CursorOamData,
		CursorAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		BlendCursorActMove,
};


//-------------------------------------------------------------
//  ����A�C�R���p��`
//-------------------------------------------------------------
static const ActOamData HanteiOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		0,		//u32 Shape:1;            // �n�a�i�`��(�������`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		1,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x16  3����64x32)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		0,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};
static const actAnm HanteiAnm0[]={
		{OBJ16_16x16*0,20,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm HanteiAnm1[]={
		{OBJ16_16x16*1,20,1,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm HanteiAnm2[]={
		{OBJ16_16x16*2,4,0,0},
		{OBJ16_16x16*3,4,0,0},
		{OBJ16_16x16*2,4,0,0},
		{OBJ16_16x16*3,4,0,0},
		{OBJ16_16x16*2,4,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm HanteiAnm3[]={
		{OBJ16_16x16*2,4,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm *const HanteiAnmTbl[]={
		HanteiAnm0,
		HanteiAnm1,
		HanteiAnm2,
		HanteiAnm3,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const CellData HanteiActCell={
	blendpat_Character+0x20*16,0x20*4*4,48888
};


//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[(����A�C�R��)
//-------------------------------------------------------------

static const	actHeader HanteiHeader={
		48888,
		46546,
		&HanteiOamData,
		HanteiAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		HanteiMove,
};

//-----------------------------------------------------------
//    �A�N�^�[�f�[�^(�Ή�)
//-----------------------------------------------------------
static const ActOamData HibanaOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		0,		//u32 Shape:1;            // �n�a�i�`��(�����`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		0,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x16  3����64x32)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		1,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};

static const actAnm HibanaAnm0[]={
		{OBJ16_8x8*0,3,0,0},
		{OBJ16_8x8*1,4,0,0},
		{OBJ16_8x8*3,5,0,0},
		{OBJ16_8x8*1,4,0,0},
		{OBJ16_8x8*0,3,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm HibanaAnm1[]={
		{OBJ16_8x8*0,3,0,0},
		{OBJ16_8x8*2,4,0,0},
		{OBJ16_8x8*4,5,0,0},
		{OBJ16_8x8*2,4,0,0},
		{OBJ16_8x8*0,3,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm HibanaAnm2[]={
		{OBJ16_8x8*0,2,0,0},
		{OBJ16_8x8*1,2,0,0},
		{OBJ16_8x8*2,2,0,0},
		{OBJ16_8x8*4,4,0,0},
		{OBJ16_8x8*3,3,0,0},
		{OBJ16_8x8*2,2,0,0},
		{OBJ16_8x8*1,2,0,0},
		{OBJ16_8x8*0,2,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm barcursorAnm[]={
		{OBJ16_8x8*5,5,1,1},
		{ACT_ANMEND,0,0,0}
};

static const actAnm barpointAnm[]={
		{OBJ16_8x8*6,5,1,1},
		{ACT_ANMEND,0,0,0}
};


static const actAnm *const HibanaAnmTbl[]={
	HibanaAnm0,
	HibanaAnm1,
	HibanaAnm2,
	barcursorAnm,
	barpointAnm,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const CellData HibanaActCell={
	&(blendpat_Character[0x20*16*2]),0x20*7,23456
};


//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------

static const	actHeader HibanaHeader={
		23456,
		46546,
		&HibanaOamData,
		HibanaAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		DummyActMove,
};

//-----------------------------------------------------------
//    �A�N�^�[�f�[�^(�J�E���g)
//-----------------------------------------------------------
static const ActOamData CountOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		0,		//u32 Shape:1;            // �n�a�i�`��(�����`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		2,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x16  3����64x32)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		1,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};

static const actAnm CountAnm1[]={
		{OBJ16_32x32*2,30,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm CountAnm2[]={
		{OBJ16_32x32*1,30,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm CountAnm3[]={
		{OBJ16_32x32*0,30,0,0},
		{ACT_ANMEND,0,0,0}
};

static const actAnm *const CountAnmTbl[]={
	CountAnm1,
	CountAnm2,
	CountAnm3,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const CellData CountActCell={
	&(blendpat_Character[0x20*16*3]),0x20*16*3,12345
};


//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------

static const	actHeader ActCountHeader={
		12345,
		46546,
		&CountOamData,
		CountAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		CountActSeq,
};

//-----------------------------------------------------------
//    �A�N�^�[�f�[�^(�J�E���g)
//-----------------------------------------------------------
static const ActOamData StartOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		1,		//u32 Shape:1;            // �n�a�i�`��(�����`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		3,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x16  3����64x32)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		1,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};

static const actAnm StartAnm1[]={
		{OBJ16_64x32*0,30,0,0},
		{ACT_ANMEND,0,0,0}
};
static const actAnm *const StartAnmTbl[]={
	StartAnm1,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const CellData StartActCell={
	&(blendpat_Character[0x20*16*6]),0x20*16*2,12346
};


//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------

static const	actHeader ActStartHeader={
		12346,
		46546,
		&StartOamData,
		StartAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		StartActSeq,
};
