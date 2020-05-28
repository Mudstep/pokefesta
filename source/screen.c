//=========================================================================
//	
//	��ʊ֘A�ϐ��Ȃ�
//
//=========================================================================

#include "common.h"

#define	SCREEN_H_GLOBAL
#include "screen.h"

//=========================================================================
//
//	BG���䃌�W�X�^�A�h���X�̔z��萔
//
//=========================================================================

//	�a�f�R���g���[�����W�X�^
vu16 * const RegBGctrl[] = {
		(vu16 *)REG_BG0CNT,
		(vu16 *)REG_BG1CNT,
		(vu16 *)REG_BG2CNT,
		(vu16 *)REG_BG3CNT,
};

//	�w�����a�f�\���J�n�ʒu�w�背�W�X�^
vu16 * const RegBGHofs[] =
{
		(vu16 *)REG_BG0HOFS,
		(vu16 *)REG_BG1HOFS,
		(vu16 *)REG_BG2HOFS,
		(vu16 *)REG_BG3HOFS,
};

//	�x�����a�f�\���J�n�ʒu�w�背�W�X�^
vu16 * const RegBGVofs[] =
{
		(vu16 *)REG_BG0VOFS,
		(vu16 *)REG_BG1VOFS,
		(vu16 *)REG_BG2VOFS,
		(vu16 *)REG_BG3VOFS,
};

//	�\���R���g���[���iDISPCNT�j�������ݗp�萔
const u16 BGvisibleSwitch[] =
{
		DISP_BG0_ON,
		DISP_BG1_ON,
		DISP_BG2_ON,
		DISP_BG3_ON,
};

//	�u�����h���W�X�^�iBLDCNT�j�p�萔�i���Ώۖʁj
const u16 BGblend2ndValue[] =
{
		BLD_BG0_2ND,
		BLD_BG1_2ND,
		BLD_BG2_2ND,
		BLD_BG3_2ND,
};

//	�u�����h���W�X�^�iBLDCNT�j�p�萔�i���Ώۖʁj
const u16 BGblend1stValue[] =
{
		BLD_BG0_1ST,
		BLD_BG1_1ST,
		BLD_BG2_1ST,
		BLD_BG3_1ST,
};

//=========================================================================
//
//=========================================================================

//	��ʓ]���p�_�~�[�a�f�̈�
u16 dmy_BG[4][ TEXT_BG_SIZE ] ={};

