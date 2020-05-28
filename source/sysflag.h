#ifndef	__SYSFLAG_H__
#define	__SYSFLAG_H__

#include "evwkdef.h"

//-------------------------------------------------------------
//	�V�X�e����`�̃t���O�i�Z�[�u������́j
//-------------------------------------------------------------
// SYSFLAG_START��evwkdef.h�Œ�`

#define	SYS_POKEMON_GET			(SYSFLAG_START+ 0)		/* �|�P������߂܂��� */
#define	SYS_ZUKAN_GET			(SYSFLAG_START+ 1)		/* �������������� */
#define	SYS_POKEGEAR_GET		(SYSFLAG_START+ 2)		/* �g���i�r��������� */
//#define	SYS_MENU_HOOK			(SYSFLAG_START+ 3)		/* ���j���[���o���Ȃ� */
#define	SYS_GAME_CLEAR			(SYSFLAG_START+ 4)		/* �Q�[�����N���A���� */
#define SYS_KAIWA_USED			(SYSFLAG_START+ 5)		/* �ȈՉ�b���g���� */
#define SYS_NOW_OYAJI_MEET		(SYSFLAG_START+ 6)		/* �i�E���I���W�ɂ����� */
#define	SYS_BATCH01_GET			(SYSFLAG_START+ 7)		/* �o�b�`01������� */
#define	SYS_BATCH02_GET			(SYSFLAG_START+ 8)		/* �o�b�`02������� */
#define	SYS_BATCH03_GET			(SYSFLAG_START+ 9)		/* �o�b�`03������� */
#define	SYS_BATCH04_GET			(SYSFLAG_START+ 10)		/* �o�b�`04������� */
#define	SYS_BATCH05_GET			(SYSFLAG_START+ 11)		/* �o�b�`05������� */
#define	SYS_BATCH06_GET			(SYSFLAG_START+ 12)		/* �o�b�`06������� */
#define	SYS_BATCH07_GET			(SYSFLAG_START+ 13)		/* �o�b�`07������� */
#define	SYS_BATCH08_GET			(SYSFLAG_START+ 14)		/* �o�b�`08������� */

#define	SYS_T101_ARRIVE		(SYSFLAG_START+ 15)	/* �~�V���ɓ������� */
#define	SYS_T102_ARRIVE		(SYSFLAG_START+ 16)	/* �R�g�L�ɓ������� */
#define	SYS_T103_ARRIVE		(SYSFLAG_START+ 17)	/* �����ɓ������� */
#define	SYS_T104_ARRIVE		(SYSFLAG_START+ 18)	/* �t�G���ɓ������� */
#define	SYS_T105_ARRIVE		(SYSFLAG_START+ 19)	/* �n�W�c�Q�ɓ������� */
#define	SYS_T106_ARRIVE		(SYSFLAG_START+ 20)	/* �V�_�P�ɓ������� */
#define	SYS_T107_ARRIVE		(SYSFLAG_START+ 21)	/* �L�i�M�ɓ������� */

#define	SYS_C101_ARRIVE		(SYSFLAG_START+ 22)	/* �g�E�J�ɓ������� */
#define	SYS_C102_ARRIVE		(SYSFLAG_START+ 23)	/* �J�C�i�ɓ������� */
#define	SYS_C103_ARRIVE		(SYSFLAG_START+ 24)	/* �L���Z�c�ɓ������� */
#define	SYS_C104_ARRIVE		(SYSFLAG_START+ 25)	/* �J�i�Y�~�ɓ������� */
#define	SYS_C105_ARRIVE		(SYSFLAG_START+ 26)	/* �q���}�L�ɓ������� */
#define	SYS_C106_ARRIVE		(SYSFLAG_START+ 27)	/* �~�i���ɓ������� */
#define	SYS_C107_ARRIVE		(SYSFLAG_START+ 28)	/* �g�N�T�l�ɓ������� */
#define	SYS_C108_ARRIVE		(SYSFLAG_START+ 29)	/* ���l�ɓ������� */
#define	SYS_C109_ARRIVE		(SYSFLAG_START+ 30)	/* �T�C���E�ɓ������� */

#define SYS_USE_WAZA_FLASH		(SYSFLAG_START+ 40)	/* �t���b�V���g�p�� */
#define SYS_USE_WAZA_KAIRIKI	(SYSFLAG_START+ 41)	/* �����肫�g�p�� */
#define SYS_WEATHER_CTRL	(SYSFLAG_START+ 42)	/* ��J�� */
#define SYS_CYCLING_ROAD	(SYSFLAG_START+ 43) /* �T�C�N�����O���[�h�ɂ��� */
#define SYS_SAFARI_MODE		(SYSFLAG_START+ 44) /* �T�t�@�����[�h���ǂ��� */
#define	SYS_CRUISE_MODE		(SYSFLAG_START+ 45) /* �D�ړ��f�����[�h���ǂ��� */

#define SYS_TV_HOME			(SYSFLAG_START+ 48)	/* ����TV����邩 */
#define SYS_TV_WATCH		(SYSFLAG_START+ 49)	/* ������TV�������� */
#define SYS_TV_START		(SYSFLAG_START+ 50)	/* TV���X�^�[�g */

#define SYS_POPWORD_INPUT   (SYSFLAG_START+51)  /* ���s��������� */
#define SYS_MIX_RECORD      (SYSFLAG_START+52)  /* ���R�[�h�R�[�i�[�ʐM������ */
#define SYS_CLOCK_SET		(SYSFLAG_START+53)	/* ���v���Z�b�g���� */

#define SYS_SPECIAL_ZUKAN	(SYSFLAG_START+54)	/* ������̑S���A�z�E�G�����[�h */

#define SYS_CAVE_SHIP		(SYSFLAG_START+55)	/* ���A�ɓ������F����ڂ��� */
#define SYS_CAVE_WONDER		(SYSFLAG_START+56)	/* ���A�ɓ������F�ӂ����Ȃǂ����� */
#define SYS_CAVE_BATTLE		(SYSFLAG_START+57)	/* ���A�ɓ������F�o�g���^���[ */

#define	SYS_ASASE_TIDE		(SYSFLAG_START+58)	/* �󐣂̓��A���������ǂ��� */

#define	SYS_RIBBON_GET      (SYSFLAG_START+59)  /* ���{��������������Ƃ����邩�H */

/* ���ꂼ��̎{�݂�K�ꂽ���Ƃ����邩�H */
#define SYS_ARRIVE_FROWERSHOP      (SYSFLAG_START+60)  // �t�����[�V���b�v
#define SYS_ARRIVE_HAGIHOUSE       (SYSFLAG_START+61)  // �n�M�낤����̂���
#define SYS_ARRIVE_JUNKSHIP        (SYSFLAG_START+62)  // ���Ă��Ԃ�
#define SYS_ARRIVE_MARINEHOUSE     (SYSFLAG_START+63)  // ���݂̂���
#define SYS_ARRIVE_NEWKINSETSU     (SYSFLAG_START+64)  // �j���[�L���Z�c
#define SYS_ARRIVE_GENKIHOUSE      (SYSFLAG_START+65)  // ���񂫂ɂȂ邢��
#define SYS_ARRIVE_KARAKURIYASHIKI (SYSFLAG_START+66)  // �J���N���₵��
#define SYS_ARRIVE_BATTLEFAMILY    (SYSFLAG_START+67)  // �o�g��������
#define SYS_ARRIVE_GRASSSHOP       (SYSFLAG_START+68)  // �K���X�������݂̂�
#define SYS_ARRIVE_MAYUMIHOUSE     (SYSFLAG_START+69)  // �}���~�̂���
#define SYS_ARRIVE_POKEMONBREEDER  (SYSFLAG_START+70)  // �|�P���������Ă�
#define SYS_ARRIVE_SUBMARINECAVE   (SYSFLAG_START+71)  // �����Ă��ǂ�����
#define SYS_ARRIVE_BATTLETOWER     (SYSFLAG_START+72)  // �o�g���^���[
#define SYS_ARRIVE_ISLAND          (SYSFLAG_START+73)  // �݂Ȃ݂̂��Ƃ�
#define SYS_ARRIVE_FIREROAD        (SYSFLAG_START+74)  // �ق̂��̂ʂ��݂�

#define SYS_PASO_MAYUMI		       (SYSFLAG_START+75)  // �}���~�̃p�\�R���ɂȂ�

#define	SYS_EXDATA_ENABLE			(SYSFLAG_START+76)	//�w�ӂ����Ȃł����Ɓx�t���O

#define	SYS_ENC_UP_ITEM				(SYSFLAG_START+77)	//���낢�r�[�h�����g����
#define	SYS_ENC_DOWN_ITEM			(SYSFLAG_START+78)	//���낢�r�[�h�����g����

#define	SYS_TENJI_ANAWOHORU			(SYSFLAG_START+79)	//�_���̓��A�u���Ȃ��ق�v�g�p����
#define	SYS_TENJI_KAIRIKI			(SYSFLAG_START+80)	//�_���̓��A�u�����肫�v�g�p����
#define	SYS_TENJI_WAIT				(SYSFLAG_START+81)	//�_���̓��A �҂���
#define	SYS_TENJI_SORAWOTOBU		(SYSFLAG_START+82)	//�_���̓��A�u������Ƃԁv�g�p����

#define	SYS_KOKORO_EVENT			(SYSFLAG_START+83)	//��̌Ǔ��C�x���g���t���O

/* ���ꂼ��̎{�݂�K�ꂽ���Ƃ����邩�H */
#define SYS_ARRIVE_LEAGUE			(SYSFLAG_START+84)  // �|�P�������[�O
#define SYS_ARRIVE_YOKOANA			(SYSFLAG_START+85)  // �����܂̂悱����
#define SYS_ARRIVE_ISEKI			(SYSFLAG_START+86)  // ���΂�������
#define SYS_ARRIVE_KASEKIMANIA		(SYSFLAG_START+87)  // �������}�j�A�̉�
#define SYS_ARRIVE_IWATO			(SYSFLAG_START+88)  // �Ђł�̂����
#define SYS_ARRIVE_KODAIDUKA		(SYSFLAG_START+89)  // �������Â�
#define SYS_ARRIVE_TUMESYO			(SYSFLAG_START+90)  // �����傤����߂���
#define SYS_ARRIVE_HUNTER			(SYSFLAG_START+91)  // �n���^�[�̂���
#define SYS_ARRIVE_SEKISITSU		(SYSFLAG_START+92)  // ���ӂ�̂�������

#define SYS_MOVE_POKE_NEWS			(SYSFLAG_START+93)  // �s�u�ňړ��|�P�����j���[�X�����

/* ���ꂼ��̎{�݂�K�ꂽ���Ƃ����邩�H */
#define SYS_ARRIVE_SORANOHASHIRA	(SYSFLAG_START+94)  // ����̂͂���

#define SYS_ASASE_ITEM				(SYSFLAG_START+95)  // �󐣂̓��A�A�C�e���߂����t���O
#define SYS_B_DASH					(SYSFLAG_START+96)  // B�_�b�V�����t���O

#define	SYS_CTRL_OBJ_DELETE			(SYSFLAG_START+97)	// OBJ�������^�C�~���O�̐���Ɏg�p����
														// ���}�b�v�J�ڎ��ɃN���A�����

#define	SYS_GAMETIME_RESET			(SYSFLAG_START+98)	// �Q�[�������ԍĐݒ苖��
														// �f�o�b�O�@�\�A�Q�[�����ł͐ݒ�ł��Ȃ�

#define SYS_ARRIVE_KINOMIMEIJIN 	(SYSFLAG_START+99)   // ���̂ݖ��l�̉Ƃ�K�ꂽ���Ƃ����邩�H

//-------------------------------------------------------------
//	���Ԍo�߃C�x���g�̃t���O
//	���V�X�e���ɂ������x�N���A�����
//-------------------------------------------------------------
// TIMEFLAG_START ��evwkdef.h�Œ�`
#define TMFLG_SEED_ZYOURO		(TIMEFLAG_START+ 0)		/* �^�l�̐���� */
#define TMFLG_SEED_PRESENT		(TIMEFLAG_START+ 1)		/* �^�l�̃v���[���g */
#define	TMFLG_ORUSUBAN_BATTLE	(TIMEFLAG_START+ 2)		/* ������ԑΐ� */
#define TMFLG_POPWORD_CHANGE	(TIMEFLAG_START+ 3)		/* ���s��̕ϓ� */
#define TMFLG_POPWORD_INPUT		(TIMEFLAG_START+ 4)		/* ���s��̕ύX */
#define	TMFLG_WEATHER			(TIMEFLAG_START+ 5)		/* �V��̕ϓ� */
#define	TMFLG_BATTLETOWER		(TIMEFLAG_START+ 6)		/* �o�g���^���[ */
#define TMFLG_RECORDCORNER		(TIMEFLAG_START+ 7)		/* ���R�[�h�R�[�i�[ */
#define TMFLG_CONTEST			(TIMEFLAG_START+ 8)		/* �R���e�X�g */
#define TMFLG_MUSHITORI			(TIMEFLAG_START+ 9)		/* ������� */
#define TMFLG_POKEMON_LOT		(TIMEFLAG_START+10)		/* �|�P�����������`�F�b�N�������ǂ��� */
#define TMFLG_SEED_PRESENT_02	(TIMEFLAG_START+11)		/* �^�l�̃v���[���g(���[�h�P�P�S) */
#define TMFLG_SEED_PRESENT_03	(TIMEFLAG_START+12)		/* �^�l�̃v���[���g(���[�h�P�P�P) */
#define TMFLG_SEED_PRESENT_04	(TIMEFLAG_START+13)		/* �^�l�̃v���[���g(���[�h�P�Q�R) */
#define TMFLG_SEED_PRESENT_05	(TIMEFLAG_START+14)		/* �^�l�̃v���[���g(���[�h�P�Q�O) */
#define TMFLG_SEED_PRESENT_06	(TIMEFLAG_START+15)		/* �^�l�̃v���[���g(�~�i��) */
#define TMFLG_SEED_PRESENT_07	(TIMEFLAG_START+16)		/* �^�l�̃v���[���g(�ԉ�) */
#define TMFLG_SEED_PRESENT_08	(TIMEFLAG_START+17)		/* �^�l�̃v���[���g(���[�h�P�Q�R) */
#define TMFLG_SEED_PRESENT_09	(TIMEFLAG_START+18)		/* �^�l�̃v���[���g(���l) */
#define TMFLG_WAZA_PRESENT		(TIMEFLAG_START+19)		/* �Z�̃v���[���g */




//-------------------------------------------------------------
//	�V�X�e����`�̃t���O�i�Z�[�u���Ȃ����́j
//-------------------------------------------------------------
#define	CTRL_FLAG_START	SCFLG_START
#define	CTRL_MAPNAME_HIDE	(CTRL_FLAG_START+ 0)		/* �}�b�v���\���̐��� */
#define	CTRL_BGM_CHANGE		(CTRL_FLAG_START+ 1)		/* BGM�؂�ւ��̐��� */

#endif	/* __SYSFLAG_H__ */
