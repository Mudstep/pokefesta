//**********************************************************************
//		oam_anm.c
//		��������ð���
//**********************************************************************
#include	<agb.h>
#include 	"common.h"
#include	"actor.h"

//----------------------------------------------------------------------
//		�ް�̧�ٲݸٰ��
//----------------------------------------------------------------------
//#include "../objdata/a_mine.ach"			// const u8 a_mine_Character[14208];
//#include "../objdata/a_debu.ach"			// const u8 a_debu_Character[31296];
//#include "..\objdata\carton.ach" 			// const u8 carton_Character[1792];

//extern	const	u8	carton_Character[];

//======================================================================
//		�����ð���
//======================================================================
const	actAnmData actAnmTransDataTable[] =
{
//---------------------------------------------------------------
// { �]���淬��ް����ڽ, �]���� }
{0,0},															// Dummy
//--------------------------------------------------------------- 00

#if 0
//---------------------------------------------------------------
{ &carton_Character[(0x20*8) * 0], 0x20*8},						// ��	��~	1
{ &carton_Character[(0x20*8) * 1], 0x20*8},						// ��	��~	2
{ &carton_Character[(0x20*8) * 2], 0x20*8},						// ���E ��~	3
{ &carton_Character[(0x20*8) * 3], 0x20*8},						// �� 	���s	4
{ &carton_Character[(0x20*8) * 4], 0x20*8},						// ��	���s	5
{ &carton_Character[(0x20*8) * 5], 0x20*8},						// ���E ���s1	6
{ &carton_Character[(0x20*8) * 6], 0x20*8},						// ���E ���s2	7
//--------------------------------------------------------------- 01
#endif
};

#if 0

typedef struct {
	const u16 cellNo = 0xffff;		// �����ް (0xffff�̏ꍇ�ͷ�ד]���^) add matsuda
	const u16 palNo;				// ��گ��Ȱ�ެ�o�^����

//----	
	ActAnmData *TransAdrs;			// �]��ð��ٱ��ڽ
	
			or 

	const u8 TransTblNo = 2;		// �]��ð��َQ������
//----
	
	const ActOamData *oamData_p;	// OAM��{�ް����ڽ
	const actAnm **anmData_p;		// ACTOR�\�������ð��ٱ��ڽ
	const actAffAnm **affData_p;	// �g�k�߯� kaga
	void (*moveData_p)(actWork *);	// ACTOR��������݊֐����ڽ
}actHeader;	//����ͯ�ލ\����

const	actAnmData **actAnmTransDataTable[] =
{
	Field_Char,							// ̨����				// cellNo 0xffff
	PokeFront_Char,						// �߹�� ����			// cellNo 0xfffe
	PokeBack_Char,						// �߹�� �w��			// cellNo 0xfffd
};


const	actAnmData *Field_Char[] =
{
	carton_Character,					// ��l��				// 0
	TrainBoy_Character,					// ���N�ڰŰ			// 1
	TrainGirl_Character,				// �����ڰŰ			// 2
};

const	actAnmData *PokeFront_Char[] =
{
	Dmmy,								// �s�v�c����
	Dmmy,
	Dmmy,
	Dmmy,
}

#endif

