//=========================================================================
//
//	�t�B�[���h�C�x���g�ŋN�������⏕�v���O����
//	
//	2001	GAME FREAK Inc.
//	2001.10.25	by tama
//=========================================================================

#ifndef EV_SUB_H
#define EV_SUB_H
#undef GLOBAL
#ifdef EV_SUB_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//�V�󐧌�֘A�w��
#define NON_FORCE_CTRL			0		/* ������X�N���v�g�Ŏw�� */

#define SHINE_CTRL				1		/*�V�󐧌�	����*/
#define SHINE2_CTRL				2		/*�V�󐧌�	����(�_�Ȃ��j*/
#define RAIN_CTRL				3		/*�V�󐧌�	�J*/
#define	SNOW_CTRL				4		/*�V�󐧌�	��*/
#define	THUNDER_CTRL			5		/*�V�󐧌�	���݂Ȃ�*/
#define MIST_CTRL				6		/*�V�󐧌�	��1*/
#define MIST2_CTRL				7		/*�V�󐧌�	��2*/
#define ASH_CTRL				8		/*�V�󐧌�	�ΎR�D*/
#define STORM_CTRL				9		/*�V�󐧌�	����*/
#define CLOUD_CTRL				10		/*�V�󐧌�	������*/
#define SHINE3_CTRL				11		/*�V�󐧌�	����3 */

#define	SRTR_CTRL				20		/*�V�󐧌�	�����ω��P */
#define	SSRS_CTRL				21		/*�V�󐧌�	�����ω��Q */


//�G�t�F�N�g����֘A�w��
#define	MAP_EFFECT_NOTHING		0		/*�G�t�F�N�g����@�Ȃ� */
#define	MAP_EFFECT_ASH			1		/*�G�t�F�N�g����@�D */
#define MAP_EFFECT_BRIDGE		2		/*�G�t�F�N�g����@�苴 */
#define	MAP_EFFECT_LOG			3		/*�G�t�F�N�g����@�ۑ� */
#define MAP_EFFECT_ICETRAP		4		/*�G�t�F�N�g����@���l�W���g���b�v */
#define MAP_EFFECT_CARGO		5		/*�G�t�F�N�g����@�g���b�N�� */
#define MAP_EFFECT_BASE			6		/*�G�t�F�N�g����@�閧��n */
#define MAP_EFFECT_BREAKFLOOR	7		/*�G�t�F�N�g����@���鏰 */


#define	SP_MUS_ROAD118		0x7fff		/*����BGM�R�[�h */


//typedef ...


//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

#ifndef ASM_CPP	/* �A�Z���u���łȂ��Ƃ��̂ݗL�� */

//-------------------------------------------------------------
//	POS�C�x���g�ɂ��v���O�����Ăяo��
//-------------------------------------------------------------
GLOBAL void EventPosProgramCall( u8 id );

//-------------------------------------------------------------
//	�D�G�t�F�N�g�̐���^�X�N�Z�b�g
//-------------------------------------------------------------
GLOBAL void AddFieldAshEffectTask(void);


//-------------------------------------------------------------
//	�}�b�v�ŗL�G�t�F�N�g����̏�����
//-------------------------------------------------------------
GLOBAL void InitMapEffectControl( void );

//-------------------------------------------------------------
//	�}�b�v�ŗL�G�t�F�N�g�̃}�b�v�؂�ւ��ɂ��X�V
//-------------------------------------------------------------
GLOBAL void UpdateMapEffectControl( void );

//-------------------------------------------------------------
//	�}�b�v�ŗL�G�t�F�N�g�����ID�ݒ�
//	<����>	id		�}�b�v�ŗL�G�t�F�N�g��ID
//-------------------------------------------------------------
GLOBAL void SetMapEffectControl( u8 id );

#endif

#undef GLOBAL
#endif	/*EV_SUB_H*/

