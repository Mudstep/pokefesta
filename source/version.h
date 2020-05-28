
#ifndef __VERSION_H__
#define __VERSION_H__

//=========================================================================
//	
//
//	�\�[�X�̃o�[�W�����w��֘A�t�@�C��
//
//
//=========================================================================

#undef GLOBAL
#ifdef __VERSION_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================

//�@���F�Ⴂ�o�[�W�����ɂ���ď����𕪊򂷂�ꍇ��
//	�@PM_VERSION�@�ɂ���ċL�q��������
//
//	��j
//	#if ( PM_VERSION == VERSION_RUBY )
//		���r�[�o�[�W�����̏ꍇ�̏������L�q
//
//	#elif ( PM_VERSION == VERSION_SAPPHIRE )
//		�T�t�@�C�A�o�[�W�����̏ꍇ�̏������L�q
//
//	#endif
//


//	�F�Ⴂ�o�[�W�����w��
#define		VERSION_SAPPHIRE	1
#define		VERSION_RUBY		2
#define		VERSION_EMERALD		3


//	���ʃR�[�h�w��
#define		LANG_JAPAN		1
#define		LANG_ENGLISH	2
#define		LANG_FRANCE		3
#define		LANG_ITALY		4
#define		LANG_GERMANY	5
#define		LANG_SPAIN		7



//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

#ifndef	ASM_CPP

extern const u8 CasetteVersion;

extern const u8 CasetteLanguage;

extern const u8 build_date[];
#endif

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);


#undef GLOBAL
#endif	/*__VERSION_H__*/

