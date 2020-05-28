//=========================================================================
//	
//	�t�B�[���h�֘A���\�[�X��`
//
//=========================================================================

#ifndef FIELD_H
#define FIELD_H

//=========================================================================
//	��`
//=========================================================================
//#define name value


//------------------------------------------------------------------------
//
// �\�����\�[�X����U��̂��߂̒�`
//
//------------------------------------------------------------------------

// �a�f�̊��蓖��
#define	MSG_BG_NO		0
#define	EFFECT_BG_NO	1
#define MAP_UP_BG_NO	1
#define MAP_LOW_BG_NO	2
#define MAP_BTM_BG_NO	3

// �a�f�̃v���C�I���e�B
#define MSG_BG_PRI		0
#define	EFFECT_BG_PRI	3
#define MAP_UP_BG_PRI	1
#define MAP_LOW_BG_PRI	2
#define MAP_BTM_BG_PRI	3

// �L�����G���A�̊��蓖��
#define MSG_CHR_BASE		2
#define EFFECT_CHR_BASE		0
#define MAP_UP_CHR_BASE		0
#define MAP_LOW_CHR_BASE	0
#define MAP_BTM_CHR_BASE	0

// �X�N���[���G���A�̊��蓖��
#define MSG_SCRN_BASE		31
#define EFFECT_SCRN_BASE	30
#define MAP_UP_SCRN_BASE	29
#define MAP_LOW_SCRN_BASE	28
#define MAP_BTM_SCRN_BASE	30

#define BASE_CHR_ADRS(n)	( BG_VRAM + n * 0x4000 )
#define BASE_SCRN_ADRS(n)	( BG_VRAM + n * 0x0800 )

#define NAME_LEN		10

#define	WATER_CHR_CODE	(0x14 | (3 << BG_SC_PLTT_SHIFT) )

// �}�b�v�L�����̏풓�E��풓
#define MAP_CHR_1ST_SIZE	0x200	// �풓�L������
#define	MAP_CHR_2ND_SIZE	0x200	// ��풓�L������
#define MAP_CHR_1ST_NO		0x000	// �풓�L�����J�n�i���o�[
#define MAP_CHR_2ND_NO		0x200	// ��풓�L�����J�n�i���o�[

// �}�b�v�p���b�g�̏풓�E��풓
#define	MAP_PLTT_1ST_SIZE	0x06	// �풓�p���b�g��
#define	MAP_PLTT_2ND_SIZE	0x06	// ��풓�p���b�g��
#define MAP_PLTT_1ST_NO		0x00	// �풓�p���b�g�J�n�i���o�[
#define MAP_PLTT_2ND_NO		0x06	// ��풓�p���b�g�J�n�i���o�[

// �}�b�v�Z���̏풓�E��풓
#define	MAP_CELL_1ST_SIZE	0x200	//�풓�Z���̐�
#define	MAP_CELL_2ND_SIZE	0x200	//�풓�Z���̐�
#define	MAP_CELL_1ST_NO		0x000	//��풓�Z���̊J�n�i���o�[
#define	MAP_CELL_2ND_NO		0x200	//��풓�Z���̊J�n�i���o�[

// �t�H���g�g�p�̈�̊��蓖��
#define NWIN_CHR_START_NO	0x000
#define KWIN_CHR_START_NO	0x200



//=========================================================================
//	�ϐ�
//=========================================================================

#endif	/*FIELD_H*/

