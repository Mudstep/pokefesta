
//=========================================================================
//	
//	�v���C���Ԋ֘A
//
//=========================================================================

#ifndef PLAYTIME_H
#define PLAYTIME_H

#undef GLOBAL
#ifdef PLAYTIME_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value
//
#define PLAYTIME_H_MAX	1000	/* �ő�v���C���� */


//typedef ...


//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);
GLOBAL void PlayTimeInit(void);		// �v���C���ԏ�����
GLOBAL void PlayTimeStart(void);	// �v���C���ԃJ�E���g�X�^�[�g
GLOBAL void PlayTimeStop(void);		// �v���C���ԃJ�E���g�X�g�b�v
GLOBAL void PlayTimeCount(void);	// �v���C���ԃJ�E���g���[�`��



#undef GLOBAL
#endif	/*PLAYTIME_H*/

