//=========================================================================
//	
//=========================================================================

#ifndef SCREEN_H
#define SCREEN_H
#undef GLOBAL
#ifdef SCREEN_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

#define TEXT_BG_X_LEN	32
#define TEXT_BG_Y_LEN	32
#define	TEXT_BG_SIZE	( TEXT_BG_X_LEN *TEXT_BG_Y_LEN )

//typedef ...


//=========================================================================
//	�ϐ�
//=========================================================================
//	�萔
extern vu16 * const RegBGctrl[];		//	�a�f�R���g���[�����W�X�^
extern vu16 * const RegBGHofs[];		//	�w�����a�f�\���J�n�ʒu�w�背�W�X�^
extern vu16 * const RegBGVofs[];		//	�x�����a�f�\���J�n�ʒu�w�背�W�X�^
extern const u16 BGvisibleSwitch[];		//	�\���R���g���[���iDISPCNT�j�������ݗp�萔
extern const u16 BGblend2ndValue[];		//	�u�����h���W�X�^�iBLDCNT�j�p�萔�i���Ώۖʁj
extern const u16 BGblend1stValue[];		//	�u�����h���W�X�^�iBLDCNT�j�p�萔�i���Ώۖʁj

//	��ʓ]���p�_�~�[�a�f�̈�
extern u16 dmy_BG[4][ TEXT_BG_SIZE ];

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);


#undef GLOBAL
#endif	/*SCREEN_H*/

