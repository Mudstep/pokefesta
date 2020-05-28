
#ifndef	__ISAGBPRN_H__
#define	__ISAGBPRN_H__

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////
/// Copyright 2000-2001 INTELLIGENT SYSTEMS Co.,Ltd.
/// IS-AGB-EMULATOR
/// PRINT DEBUG LIBRARY [libagbprn.a]

/////////////////////////////////////////////////////////////////////////
/// �֐�����
/// 
/// [�v�����g�f�o�b�O������]
/// void AGBPrintInit(void);
///						�v�����g�f�o�b�O�Ɏg�p����o�b�t�@�����������܂��B���݂́A
///						0x08fd0000 - 0x08fdffff �܂ł��g�p���܂��B�������̃A�h���X
///						��IS-CGB-EMULATOR�̃G�~�����[�V�������ڗe�ʂɂ���Ĉړ�����
///						�\��������܂��B
///
/// [�P�����o��]
///	 void AGBPutc(const char cChar);
///		cChar:	�\���p�L���[�o�b�t�@�ɂP�����ǉ����܂��B���̊֐����Ă�ł�
///						�����ɕ\������邱�Ƃ͂���܂���B�������A�L���[�o�b�t�@��
///						�����ς��ɂȂ����ꍇ�ɂ́A�P�u���b�N���\���o����܂ő҂���
///						�Â��܂�
///
/// [������o��]
/// void AGBPrint(const char* pBuf);
///		 pBuf: �\���p�L���[�o�b�t�@�ɕ������ǉ����܂��B���̊֐����Ă�ł�
///						�����ɕ\������邱�Ƃ͂���܂���B�������A�L���[�o�b�t�@��
///						�����ς��ɂȂ����ꍇ�ɂ́A�P�u���b�N���\���o����܂ő҂���
///						�Â��܂�
///
/// [�t�H�[�}�b�g�t��������o��]
///	void AGBPrintf(const char *pBuf, ...)
///		 pBuf:	�\���p�L���[�o�b�t�@�Ƀt�H�[�}�b�g�����������ǉ����܂��B
///						���̊֐����Ă�ł������ɕ\������邱�Ƃ͂���܂���B�������A
///						�L���[�o�b�t�@�������ς��ɂȂ����ꍇ�ɂ́A�P�u���b�N���\��
///						�o����܂ő҂��Â��܂�(�g�p���@�́AANSI��printf()�֐���
///						���l�ł��B�j
///
/// [������P�u���b�N�\��]
/// void AGBPrintFlush1Block(void);
///						�\���p�L���[�o�b�t�@����AGB���̓���ɂ��܂�e����^���Ȃ�
///						�e�ʕ����t���b�V�����܂��B���̊֐����AV-BLANK���ɌĂяo��
///						�ȂǂŁAAGB�������S�ɒ�~�����邱�ƂȂ��A���������A
///						IS-AGB-EMULATOR�AIS-AGB-DEBUGGER�̃R�}���h�r���[�ɂɕ������
///						�t���b�V�����邱�Ƃ��o���܂��B���t���[���������\������悤
///						�ȏꍇ�ɕ֗��ł��B
///
/// [������t���b�V��]
/// void AGBPrintFlush(void);
///						�\���p�L���[�o�b�t�@����ɂȂ�܂ŁA���S�ɂȂ��Ȃ�܂�
///						IS-AGB-EMULATOR�AIS-AGB-DEBUGGER�̃R�}���h�r���[�ɂɕ�����
///						���t���b�V�����邱�Ƃ��o���܂��B
///
/// [�v���O�����������`�F�b�N�}�N��]
/// AGB_ASSERT( �]���� );
///		 �]���� �ɂ́AC����̕]�������L�q���܂��B�]���� �̒l���^(ZERO��
///		 �O)�Ȃ�Ύ��̃v���O���������s���܂��B�U(ZERO)�Ȃ�΁A
///		 AGB_ASSERT()����`����Ă���\�[�X�t�@�C�����Ƃ��̃��C���i���o�[
///		 �A�]���� ��\�����āA�v���O�����͒�~���܂��B
///
/// [�v���O�����������`�F�b�N�}�N��]
/// AGB_WARNING( �]���� );
///		 AGB_ASSERT()�Ɠ����g�p���@�ł��BAGB_ASSERT()�Ƃ̈Ⴄ�_�́AASSERT
///		 �́A�v���O�������~���ȍ~�̏����͍s��Ȃ��i�n���O�A�b�v��ԁj��
///		 �Ƃɑ΂��AAGB_WARNNING()�͈��������v���O���������s����_�ł��B

/////////////////////////////////////////////////////////////////////////
/// �g�p��̒���
/// 
/// �E���̃��C�u������gcc�p�ł��B
/// �E���̃��C�u������thumb��p�ł��Barm�ł͎g�p���邱�Ƃ͏o���܂���B
/// �E��������o�͂��Ă��A�����Ƀz�X�g�ɏo�͂�����ł͂���܂���B�o�b
///		�t�@�������ς��ɂȂ邩�A�t���b�V���n�֐����Ăяo�����ƂŃz�X�g���ɏo
///		�͂��s���܂��B
/// �E�v�����g�f�o�b�O�Ŏg�p����G���A�̓��[�U�[���ύX���Ă͂����܂���B
/// �EIS-AGB-EMULATOR�n�[�h�E�F�A�̓s����A�z�X�g���Ƀf�[�^�𑗂�Ԃɂ�
///		(AGBFlash()���Ă΂���)[�ꎞ�I]�Ɋ����݂��֎~�A�������E�F�C�g����
///		�X����܂��B
/// �Egcc�R���p�C���ɂ́A�K���I�v�V���� -mthumb-interwork ���w�肵�Ă���
///		�����B
/// �EAGB�J�[�g���b�W64M�t���b�V����o�׎���ROM�Ȃǂ̃����[�X�o�[�W������
///   �쐬����ۂɂ́A NDEBUG ���`���邱�ƂŃv�����g�f�o�b�O�֐��E�}�N��
///   �͕K����菜���Ă��������B
/// �Elibisagbprn.a�������N����ƁA�����I��libg.a�������N����܂��B
///   libg.a�Ɋ܂܂�Ă���֐����ƃ��[�U�[���g�p����֐������Փ˂��Ȃ��悤
///   �����ӂ��������B
///
/////////////////////////////////////////////////////////////////////////
/// Makefile �����N�s�̗�
/// -L. -l isagbprn

/////////////////////////////////////////////////////////////////////////
/// �g�p��
/// 
/// AGBPrintInit();
///
/// while(1) {
///   static int i;
///		AGBPrintf("TEST PROGRAM %d \n", i++);
///			pSelf->X++;
///			AGB_ASSERT((pSelf != NULL) && (pSelf->X > 0));
///   AGBFlush1Block();
/// }
///
/////////////////////////////////////////////////////////////////////////
///

#ifdef NDEBUG
	#define AGBPrintInit()
	#define AGBPutc(cChar)
	#define AGBPrint(pBuf)
	#define	AGBPrintf(arg...)
	#define AGBPrintFlush1Block()
	#define AGBPrintFlush()
	#define AGBAssert(file,line,info)
#else
	void AGBPrintInit(void);
	void AGBPutc(const char pBuf);
	void AGBPrint(const char* pBuf);
	void AGBPrintf(const char *pBuf, ...);
	void AGBPrintFlush1Block(void);
	void AGBPrintFlush(void);
	void AGBAssert(const char* pFile, int nLine, const char* pExpression, int nStopProgram);
#endif

/////////////////////////////////////////////////////////////////////////
///

#undef AGB_ASSERT
#ifdef NDEBUG
#define	AGB_ASSERT(exp)
#else
#define	AGB_ASSERT(exp)	(exp) ? ((void)0) : AGBAssert(__FILE__, __LINE__, #exp, 1);
#endif

#undef AGB_WARNING
#ifdef NDEBUG
#define	AGB_WARNING(exp)
#else
#define	AGB_WARNING(exp)	 (exp) ? ((void)0) : AGBAssert(__FILE__, __LINE__, #exp, 0);
#endif

/////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}	 // extern "C"
#endif

#endif

