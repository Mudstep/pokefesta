//========================================================================
//
//
//	�C�x���g�p���[�N�G���A��`
//
//			2001_09_07 GAME FREAK Inc.
//========================================================================

#ifndef	SAVEWORK_H
#define SAVEWORK_H


/*
 �v���O�������Ǝv���郏�[�N��`��source�f�B���N�g����
  syswork.h�Ɉړ�����
*/

#define		SVSCRWK_START				(80+SVWK_START)

//------------------------------------------------------------------------
//							�t�B�[���h�^�E��
//------------------------------------------------------------------------
#define	WK_SCENE_FIELD_T101				(0+SVSCRWK_START)
/* �~�V���^�E���V�[���i���o�[
01 �~�V���̃X�g�b�p�[������(POS)
02 �~�V���̃X�g�b�p�[�u���[�h�P�O�P�ŋ��ѐ������������v����(POS)
03 ��Ƀ����j���O�V���[�Y��Ⴄ�C�x���g�J�n
04 ��Ƀ����j���O�V���[�Y��Ⴄ�C�x���g�I��
*/

#define	WK_SCENE_FIELD_T102				(1+SVSCRWK_START)
/* �R�g�L�^�E���V�[���i���o�[
01 �R�g�L�̃X�g�b�p�[������(POS)
   (�R�g�L�Ń����X�^�[�{�[�������Ă����Ԃ�������
    �����I��01������悤�ɂ��Ă���)
*/

#define	WK_SCENE_FIELD_T103				(2+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_T104				(3+SVSCRWK_START)
/*
00 �����Ȃ�
01 �o�b�W���擾���ăT�|�[�g�Ƒ����\
02 �S�[�S�[�S�[�O������
*/

#define	WK_BASE_POSNO					(4+SVSCRWK_START)
/*
   �閧��n�̃f�[�^�ԍ�(syswork.h����ړ�) 
*/

#define	WK_SCENE_FIELD_T106				(5+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_T107				(6+SVSCRWK_START)
/*
*/

//------------------------------------------------------------------------
//							�t�B�[���h�V�e�B
//------------------------------------------------------------------------
#define	WK_SCENE_FIELD_C101				(7+SVSCRWK_START)
/*
//01 ���Ƙb����
02 �~�c���ƃ|�P������߂܂��ɂ����C�x���g�X�^�[�g
03 �~�c���ƃ|�P������߂܂��ɂ����C�x���g�I��
04 ����g�E�J�̃|�X�C�x���g�I��
*/

#define	WK_SCENE_FIELD_C102				(8+SVSCRWK_START)
/*
01 �J�C�i�ɃN�X�m�L�͒����A���Ă��邱�Ƃ�m��(�J�C�i�s�u������)
02 �N�X�m�L�͒��̃C���^�r���[�C�x���g�I��(�쎟�n��Ԃ����ɖ߂�)
*/

#define	WK_SCENE_FIELD_C103				(9+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_C104				(10+SVSCRWK_START)
/* �J�i�Y�~�V�e�B�V�[���i���o�[
01 �W�����[�_�[�ɏ�������
02 �|�P�����������������
03 �������Ɖ�b����(POS)
04 �f�{���̂ɂ������Ԃ���
05 �f�{���̂ɂ���n����
*/

#define	WK_SCENE_FIELD_C105				(11+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_C106				(12+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_C107				(13+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_C108				(14+SVSCRWK_START)
/*
01 �~�N���B���K�̏�̕��ɂ���
02 �~�N���B�Ɉē����ꂽ
*/

#define	WK_SCENE_FIELD_C109				(15+SVSCRWK_START)
/*
*/

//------------------------------------------------------------------------
//							�t�B�[���h���[�h
//------------------------------------------------------------------------
#define	WK_SCENE_FIELD_R101				(16+SVSCRWK_START)
/*
01 �}�b�v�����o���Ȃ�����
02 �I�_�}�L���m����|�P���������炤�C�x���g��(����)
03 �I�_�}�L���m����|�P���������炤�C�x���g�I��
*/

#define	WK_SCENE_FIELD_R102				(17+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R103				(18+SVSCRWK_START)
/*
01 �T�|�[�g�L�����Ƃ̃o�g���ɏ�������
*/

#define	WK_SCENE_FIELD_R104				(19+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R105				(20+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R106				(21+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R107				(22+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R108				(23+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R109				(24+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R110				(25+SVSCRWK_START)
/*
01 �T�|�[�g�Ƃ̐퓬�I��
*/

#define	WK_SCENE_FIELD_R111				(26+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R112				(27+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R113				(28+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R114				(29+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R115				(30+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R116				(31+SVSCRWK_START)
/*
01 �J�i�V�_�g���l���O�Ńn�M�̃��b�Z�[�W���o��(�n�M�O)
02 ��L	����(�n�M�O)
*/

#define	WK_SCENE_FIELD_R117				(32+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R118				(33+SVSCRWK_START)
/*
01 �_�C�S�C�x���g�I������
*/

#define	WK_SCENE_FIELD_R119				(34+SVSCRWK_START)
/*
01 �T�|�[�g�C�x���g�I������
*/

#define	WK_SCENE_FIELD_R120				(35+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R121				(36+SVSCRWK_START)
/*
01 �A�N�A�c��������юR�̕��֋����Ă���
*/

#define	WK_SCENE_FIELD_R122				(37+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R123				(38+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R124				(39+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R125				(40+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R126				(41+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R127				(42+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R128				(43+SVSCRWK_START)
/*
01 �f�{�����q�C�x���g�J�n
02 �f�{�����q�C�x���g�I��
*/

#define	WK_SCENE_FIELD_R129				(44+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R130				(45+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R131				(46+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R132				(47+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R133				(48+SVSCRWK_START)
/*
*/

#define	WK_SCENE_FIELD_R134				(49+SVSCRWK_START)
/*
*/

//------------------------------------------------------------------------
//							���̑�(�����̒��Ȃ�)
//------------------------------------------------------------------------
#define	WK_SCENE_T101_R0101				(50+SVSCRWK_START)
/* ����P�K �V�[���i���o�[
01 �����񂪕����ɓ���Ƙb���|���Ă��� 
02 �����񂪕����ɓ���Ƙb���|������
03 �Q�[���N���A����
04 �Q�[���N���A���ĂP�K�ɉ���Ă��ăp�p�̋����C�x���g
05 ���C�x���g��t�B�[���h�ɏo��
*/

#define	WK_SCENE_T101_R0102				(51+SVSCRWK_START)
/* �����̕��� �V�[���i���o�[ 
// WK_SCENE_T101_R0202�Ō��Ă���
*/

#define	WK_SCENE_T101_R0301				(52+SVSCRWK_START)
/* �I�_�}�L�������V�[���i���o�[
//01 �I�_�}�L���m������Ȃ̂����肩�畷����
02 �I�_�}�L���m�������ŏ���b��
03 �I�_�}�L���m�ɖ��Ƒΐ킵�Ă����ƌ�����
04 �}�ӖႤ�I�[�g�C�x���g����
05 �}�ӁE�����X�^�[�{�[��������(�I�[�g�C�x���g�I��)
*/

#define	WK_SCENE_C101_R0201				(53+SVSCRWK_START)
/* �g�E�J�V�e�B�W���V�[���i���o�[
01 �~�c�����K���C�x���g�I��
02 �|�P�����ߊl��̃W���C�x���g�I��
03 �o�b�`�P��
04 �o�b�`�Q��
05 �o�b�`�R��
06 �o�b�`�S��
07 �p�p����o�b�`������
*/

#define	WK_SCENE_SP_CONTEST				(54+SVSCRWK_START)
/*�R���e�X�g�̃V�[���i���o�[
01 �Q�������R���e�X�g�J�n
02 �Q�������R���e�X�g�I��
*/

#define	WK_SCENE_SP_PC02				(55+SVSCRWK_START)
/* PC�Q�K�̃V�[���i���o�[

*/

#define	WK_SCENE_SP_CONTEST03				(56+SVSCRWK_START)
/* �R���e�X�g�̉��̃i���o�[(�A��Ƃ����ʐMcheck�Ŏg�p)
01 	�V�_�P
02	�n�W�c�Q
03	�J�C�i
04	�~�i��
05	�~�i���i�ʐM�j
*/

#define	WK_SCENE_BASE_GOODS				(57+SVSCRWK_START)
// �閧��n��OBJ�O�b�Y�z�u�t���O

#define	WK_SCENE_SP_CONTESTITEM				(58+SVSCRWK_START)
/* �R���e�X�g�ŗD�������Ƃ��A�ו��������ς��ŖႦ�Ȃ������ꍇ
01 	�n�W�c�Q�ŗD��
02	�V�Q�^�P�ŗD��
03	�J�C�i�ŗD��
04	�~�i���ŗD��
*/

//#define	WK_SCENE_HAGI_EVENT				(59+SVSCRWK_START)
/*
*/

#define	WK_SCENE_T101_R0201				(60+SVSCRWK_START)
/*
01 �����񂪕����ɓ���Ƙb���|���Ă��� 
02 �����񂪕����ɓ���Ƙb���|������
03 �Q�[���N���A����
04 �Q�[���N���A���ĂP�K�ɉ���Ă��ăp�p�̋����C�x���g
05 ���C�x���g��t�B�[���h�ɏo��
*/

#define	WK_SCENE_T101_R0202				(61+SVSCRWK_START)
/* �I�_�}�L���̕��� �V�[���i���o�[ 
03 �I�_�}�L���Ƙb����
04 �}�ӓ��肵��(�����X�^�[�{�[�����������)
*/

#define	WK_SCENE_HAGI_FUNE				(62+SVSCRWK_START)
/*
01 �n�M�V�l�̏����Ń����ɍs���ɂ�����A�P�ɂȂ�Afield_r104�ŋ����I��
   �D�ړ����n�܂�
02 (����)�n�M�V�l�̏����ɑD�Ŗ߂�Ƌ����I�Ƀ��b�Z�[�W����ׂ�
*/

#define	WK_SCENE_C104_R0103				(63+SVSCRWK_START)
/*
01 ����������f�{���̂ɂ����󂯎��
*/

#define	WK_SCENE_R104_R0101				(64+SVSCRWK_START)
/*
01 �J�i�V�_�g���l���C�x���g�I��
*/

//#define	WK_SCENE_R116_R0101				(65+SVSCRWK_START)
/*
*/

#define	WK_SCENE_SP_OPENING				(66+SVSCRWK_START)
/*
01 (�j�̎q)�Q�[���X�^�[�g
02 (���̎q)�Q�[���X�^�[�g
03 (����)����P�K�C�x���g��
04 (����)����P�K�C�x���g�I��
05 (����)����Q�K�ɍs����
06 (����)����Q�K���v�Z�b�g����
07 (����)����C�x���g�I��
08 (����)�Ƃ���o��
*/

#define	WK_SCENE_C103_R0101				(67+SVSCRWK_START)
/*
01 �P�ڂ̃X�C�b�`�������Ă���
02 �Q�ڂ̃X�C�b�`�������Ă���
03 �R�ڂ̃X�C�b�`�������Ă���
*/

#define	WK_SCENE_C106_R0202				(68+SVSCRWK_START)
/*
01 �@�ْ��Ƙb����
*/

#define	WK_SCENE_C106_R0401				(69+SVSCRWK_START)
/*
01 �@�t�@�����߂��C�x���g��������悤�ɂȂ�
02 �@�t�@�����߂��C�x���g�I��
*/

#define	WK_SCENE_FUNE_POS				(70+SVSCRWK_START)
/*
00 �D�͂Ȃ�
01 �n�M�V�l�̑D�̈ʒu	���[�h�P�O�S
02						����
03						���[�h�P�O�X
*/

#define	WK_SCENE_BASE_MAKE				(71+SVSCRWK_START)
/*
01 �閧��n�쐬���̋����ړ��I��
*/

#define	WK_SCENE_CAVE_D0601				(72+SVSCRWK_START)
/*
01 �g���i�r�C�x���g�I��
*/

#define	WK_SCENE_ARTIST_C106_R0301		(73+SVSCRWK_START)
/*
00 ��Ƒޏ�
01 ��Ɠo��
*/

#define	WK_SCENE_CAVE_D0201				(74+SVSCRWK_START)
/*
02 ��ʒ[�ɃA�N�A�c��������
03 ���Ƃ����肵��
04 �J�i�V�_�g���l���J��(����)
05 �J�i�V�_�g���l���J��(�㑤)
06 �J�i�V�_�g���l���C�x���g�I��
*/

#define	WK_SCENE_CAVE_D1206				(75+SVSCRWK_START)
/*
01 ���Ñ�|�P�����C�x���g�I��
*/

#define	WK_SCENE_POKE_LEAGUE			(76+SVSCRWK_START)
/*
01 r0101
02 r0102
03 r0103
04 r0104
*/

//#define	WK_SCENE_R114_R0101				(77+SVSCRWK_START)
/*
*/

//#define	WK_SCENE_CAVE_D0501				(78+SVSCRWK_START)
/*
*/

//#define	WK_SCENE_WEATHER_CTRL			(79+SVSCRWK_START)
/*
*/

#define	WK_SCENE_C102_R0601				(80+SVSCRWK_START)
/*
01 �N�X�m�L�͒��C�x���g�J�n
02 �N�X�m�L�͒��C�x���g�I��(���������܂ꂽ)
*/

//#define	WK_SCENE_C102_R0602				(81+SVSCRWK_START)
/*
*/

#define	WK_SCENE_CAVE_D1111				(82+SVSCRWK_START)
/*
01 ���Ñ�|�P�����ɏ��߂ĉ���̃C�x���g�I��
*/

#define	WK_SCENE_ROPEWAY				(83+SVSCRWK_START)
/*
01 �������ɍs��
02 �ォ�牺�ɍs��
*/

#define	WK_SCENE_SAFARI_ZONE			(84+SVSCRWK_START)
/*
01 �T�t�@���]�[���A�蒆(r121_r0101)
02 �T�t�@���]�[������(park_safari04)
*/

#define	WK_SCENE_SHISEN					(85+SVSCRWK_START)
/*
00 �J���N���剤�̎������b�Z�[�W��\������
01 �J���N���剤�̎������b�Z�[�W��\�����Ȃ�
*/

#define	WK_SCENE_R110_R0101				(86+SVSCRWK_START)
/*
01 �}�b�v�`�F���W�Ń}�b�v���ς�����Ƃ��̏������s
*/

#define	WK_KARAKURI_DAIOU				(87+SVSCRWK_START)
/*
00 �J���N���剤���B��Ă���
01 �J���N���剤���_���W�����ɂ���
02 �J���N���剤���d�|�����쐬��
03 �J���N���剤�����Ȃ��Ȃ���(���������j
04 �J���N���剤�����Ȃ��Ȃ���(�ʘH���o������ŁA��ʂɂ��Ȃ��d�|�����j
*/

#define	WK_SCENE_CAVE_D0701				(88+SVSCRWK_START)
/*
01 �G�̃{�X�ɏ�������
02 �o���|�X�I��
*/

#define	WK_SCENE_CYCLE_ROAD				(89+SVSCRWK_START)
/*
01 �T�C�N�����O���[�h�X�^�[�g
02 �v���J�n
03 �v���I��
*/

#define	WK_SCENE_C102_R0501				(90+SVSCRWK_START)
/*
01 ���ꗿ���𕥂���
*/

#define	WK_SCENE_R110_R0104				(91+SVSCRWK_START)
/*
00 �������ƂΖ�����
01 �������ƂΓ���
02 �����J����
*/

#define	WK_SCENE_R110_R0105				(92+SVSCRWK_START)
/*
00 �������ƂΖ�����
01 �������ƂΓ���
02 �����J����
*/

#define	WK_SCENE_R110_R0106				(93+SVSCRWK_START)
/*
00 �������ƂΖ�����
01 �������ƂΓ���
02 �����J����
*/

#define	WK_SCENE_R110_R0107				(94+SVSCRWK_START)
/*
00 �������ƂΖ�����
01 �������ƂΓ���
02 �����J����
*/

#define	WK_SCENE_R110_R0108				(95+SVSCRWK_START)
/*
00 �������ƂΖ�����
01 �������ƂΓ���
02 �����J����
*/

#define	WK_SCENE_R110_R0109				(96+SVSCRWK_START)
/*
00 �������ƂΖ�����
01 �������ƂΓ���
02 �����J����
*/

#define	WK_SCENE_R110_R0110				(97+SVSCRWK_START)
/*
00 �������ƂΖ�����
01 �������ƂΓ���
02 �����J����
*/

#define	WK_SCENE_R110_R0111				(98+SVSCRWK_START)
/*
00 �������ƂΖ�����
01 �������ƂΓ���
02 �����J����
*/

#define	WK_SCENE_R119_TENKI				(99+SVSCRWK_START)
/*
01 ���
02 �����A����̈ʒu��ύX����
*/

#define	WK_SCENE_SP_SHIP01				(100+SVSCRWK_START)
/*
01 �J�C�i��������
02 �~�i����������
03 �J�C�i�������ē���
04 �~�i���������ē���
*/

#define	WK_SCENE_R110_R0103				(101+SVSCRWK_START)
/*
01 �ʘH��ʂ���
*/

#define	WK_SCENE_02_R110_R0110			(102+SVSCRWK_START)
/*
01 �}�b�v�`�F���W�Ń}�b�v���ς�����Ƃ��̏������s
*/

#define	WK_SCENE_01_C102_R0401			(103+SVSCRWK_START)
/*
00 ���߂ĉ�ɉ����
01 �Q��ڈȍ~
02 ���ׂẴA�C�e�������
*/

//#define	WK_SCENE_PARK_KOKORO01		(104+SVSCRWK_START)
/*
*/

#define	WK_SCENE_CAVE_D0808			(105+SVSCRWK_START)
/*
01 �}�O�}�c�����o��
02 �}�c�u�T������
03 �}�c�u�T�������Ă���
*/

#define	WK_SCENE_CAVE_D1602			(106+SVSCRWK_START)
/*
01 �����J����
02 ���d�@�̃X�C�b�`�𓥂�
*/

#define	WK_FUTAGO_01_FIELD_R104		(107+SVSCRWK_START)
/*
00 �퓬�O
01 ���ɑނ���
02 �E�ɑނ���
*/

#define	WK_SCENE_BATTLE_TOWER		(108+SVSCRWK_START)
/*
00 �������ĂȂ�
01 �o�g���^���[�ŗV��
*/

#define	WK_SCENE_CAVE_D0702			(109+SVSCRWK_START)
/*
01 �ł��ڂ��R�ŉΎR�D���~�炷(���[�v�E�F�C������~��Ă���)
*/

#define	WK_SCENE_R113_R0101			(110+SVSCRWK_START)
/*
01 �u�͂��Ԃ���v����
02 ��x�}�b�v�A�E�g����
10 �������r�[�h���Ⴆ����
11 ������r�[�h���Ⴆ����
12 �������r�[�h���Ⴆ����
13 ���낢�r�[�h���Ⴆ����
14 ���낢�r�[�h���Ⴆ����
15 �K���X�̂����Ⴆ����
16 �K���X�̂����Ⴆ����
*/

#define	WK_SCENE_CAVE_D0101			(111+SVSCRWK_START)
/*
01 �����̓��A�C�x���g�I��
*/

#define	WK_SCENE_TR_HOUSE			(112+SVSCRWK_START)
/*
01 �ΐ킵��(������)
02 �ΐ킵��(������)
03 �ΐ킵��(��������)
*/

#define	WK_SCENE_R110_R0102				(113+SVSCRWK_START)
/*
01 �A�C�e�������Ă��Ȃ��ŏo�����
*/

#define	WK_SCENE_T107_R0201				(114+SVSCRWK_START)
/*
00 ��T�Ԃ�����
01 - 07��ɃA�C�e����Ⴆ��
*/

#define	WK_SCENE_CAVE_D1301				(115+SVSCRWK_START)
/*
01 �~�c���Ƒΐ킵��
*/

#define	WK_SCENE_C104_R0102				(116+SVSCRWK_START)
/*
00 �J�Z�L��a���Ă��Ȃ�
01 �J�Z�L��a����(�Đ��O)
02 �J�Z�L��a����(�Đ���)
*/

#define	WK_KASEKI_TYPE					(117+SVSCRWK_START)
/*
01 �J�Z�L
02 �^�C�R
*/

#define	WK_SCENE_C107_R0501				(118+SVSCRWK_START)
/*
01 �_�C�S�Ɂu�_�C�r���O�v���炤
*/

#define	WK_SCENE_02_FIELD_T102			(119+SVSCRWK_START)
/*
00 �܂��T�|�[�g�C�x���g�O
01 �T�|�[�g�ҋ@
02 �T�|�[�g�C�x���g�I��
*/

#endif /* SAVEWORK_H */

