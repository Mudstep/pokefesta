
//=========================================================================
//	
//=========================================================================

#ifndef FCONTEXT_H
#define FCONTEXT_H
#undef GLOBAL
#ifdef FCONTEXT_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value
//typedef ...

//	�����\���a�f����\����
struct FONT_CONTEXT_STRUCT{
		u8 BGNum;				//�g�p����a�f�̃i���o�[
		u8 CGXBlockNum;			//�g�p����L�����̃u���b�N�w��
		u8 SCRBlockNum;			//�g�p����X�N���[���̃u���b�N�w��
		u8 BGpriority;			//�g�p����a�f�̕\���D�揇��
		u8 BasePalette;			//��{�g�p�p���b�g
		u8 BaseFontColor;		//��{�����J���[
		u8 BaseBGColor;			//��{�w�i�J���[
		u8 BaseShadowColor;		//��{�e�J���[
		u8 BaseFontType;		//��{�t�H���g
		u8 UseMode;				//�����\�����[�h
		u32	CGXadrs;			//�g�p����L�����̃A�h���X
		u32	SCRadrs;			//�g�p����X�N���[���̃A�h���X
		u16 * Screen;			//���z�u�q�`�l�̃A�h���X�i�������j
};
typedef struct FONT_CONTEXT_STRUCT FONT_CONTEXT;


//	�����\�����[�h��`
enum{
		TRANSFONTMODE,		/* �]���t�H���g���[�h */
		FIXFONTMODE,		/* �풓�t�H���g���[�h */
		TRANSCGXMODE,		/* �f�[�^�]�����[�h */
		FIXSCRNMODE,		/* �]���^�A�X�N���[���Œ胂�[�h */
};


//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

//-------------------------------------------------------------------------
//	�R���e�L�X�g�w��h�c����R���e�L�X�g�ւ̃|�C���^���擾
//-------------------------------------------------------------------------
GLOBAL CONST_FONT_CONTEXT_P GetFontContext(CONTEXT_TYPE type);

//-------------------------------------------------------------------------
// MSG����\���̂���R���e�L�X�g�f�[�^���擾
//-------------------------------------------------------------------------
GLOBAL u8 * MsgContextGetCGXadrs( TALK_WIN * tw );
GLOBAL u16 * MsgContextGetSCRadrs( TALK_WIN * tw );

#undef GLOBAL
#endif	/*FCONTEXT_H*/

