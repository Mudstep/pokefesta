//=========================================================================
//	
//	�}�b�v�p�����[�^�֘A
//
//=========================================================================

#ifndef MAPPARAM_H
#define MAPPARAM_H

#undef GLOBAL
#ifdef MAPPARAM_H_GLOBAL
#define GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif


//=========================================================================
//	��`
//=========================================================================
/* �}�b�v���ނ̒�` */
#define MAPTYPE_TOWN	1			/* �� */
#define MAPTYPE_CITY	2			/* �X */
#define MAPTYPE_ROAD	3			/* ���H */
#define MAPTYPE_CAVE	4			/* ���A */
#define MAPTYPE_WATER	5			/* ��(�H) */
#define MAPTYPE_SEA		6			/* �C */
#define MAPTYPE_ICE		7			/* �X */
#define MAPTYPE_ROOM	8			/* ���� */
#define MAPTYPE_BASE	9			/* �閧��n */

/* �}�b�v���ނ̒�` GetBattleBGType�p */
#define	BATTLEBGTYPE_NONE			0		/*�@�w�薳���@*/
#define	BATTLEBGTYPE_GYM			1		/*�@�W���@*/
#define	BATTLEBGTYPE_RUBY_AZITO		2		/*�@�A�W�g�i���r�[�j�@*/
#define	BATTLEBGTYPE_SAPPHIRE_AZITO	3		/*�@�A�W�g�i�T�t�@�C�A�j�@*/
#define	BATTLEBGTYPE_BIG4_KAGETSU	4		/*�@�l�V���i�J�Q�c�j�@*/
#define	BATTLEBGTYPE_BIG4_FUYOU		5		/*�@�l�V���i�t���E�j�@*/
#define	BATTLEBGTYPE_BIG4_PURIMU	6		/*�@�l�V���i�v�����j�@*/
#define	BATTLEBGTYPE_BIG4_GENZI		7		/*�@�l�V���i�Q���W�j�@*/
#define	BATTLEBGTYPE_WHITE_ROOM		8		/*�@�������@*/

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================

GLOBAL u8 GetMapTypeByID( s8 div_id, s8 map_id );	//�}�b�v�h�c����}�b�v�^�C�v���擾
GLOBAL u8 GetNowMapType( void );		//���݂̃}�b�v�̃^�C�v���擾
GLOBAL u8 GetBeforeMapType( void );		//���O�̃}�b�v�̃^�C�v���擾


GLOBAL u8 GetNowWorldPos( void );		//���݂̃}�b�v�ł̃��[���h�}�b�v�ʒu��Ԃ�
GLOBAL u8 GetSpecialWorldPos( void );	//����ڑ��̃��[���h�}�b�v�ʒu���擾

GLOBAL u8 MTYPE_IsFieldCheck( u8 type );	//	�t�B�[���h�}�b�v���ǂ����̃`�F�b�N
GLOBAL u8 MTYPE_IsEnableFlyCheck( u8 type );	// ������ƂԂ��ł��邩�ǂ����̃`�F�b�N
GLOBAL u8 MTYPE_IsIndoorCheck( u8 type );	//	�������ǂ����̃`�F�b�N

GLOBAL u8 GetBattleBGType(void);		//���݂̃}�b�v�̐퓬�w�i�w���Ԃ�

#undef GLOBAL
#endif	/* MAPPARAM_H */

