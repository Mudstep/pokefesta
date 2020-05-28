//======================================================================
//
//	�Q�[�����Z�[�u�t���O��`
//
//
//	2001_09_05	GAME FREAK Inc.
//======================================================================

#ifndef SAVEFLAG_H
#define SAVEFLAG_H

//----------------------------------------------------------------------
//	�ėp�t���O			//ev_check.c��id=0�͖����ɂȂ��Ă���
//----------------------------------------------------------------------
#define	FH_00							1
#define	FH_01							2
#define	FH_02							3
#define	FH_03							4
#define	FH_04							5
#define	FH_05							6
#define	FH_06							7
#define	FH_07							8
#define FH_08							9
#define FH_09							10
#define FH_10							11
#define FH_11							12
#define FH_12							13
#define FH_13							14
#define FH_14							15
#define FH_15							16

//�}�b�v�J�ڂ̃^�C�~���O�Ō��ɖ߂�OBJ��flag�Ɏg�p����(MapEdit)
//�u����������v�Ő؂��؂Ȃ�
#define	FH_OBJ00						17
#define	FH_OBJ01						18
#define	FH_OBJ02						19
#define	FH_OBJ03						20
#define	FH_OBJ04						21
#define	FH_OBJ05						22
#define	FH_OBJ06						23
#define	FH_OBJ07						24
#define	FH_OBJ08						25
#define	FH_OBJ09						26
#define	FH_OBJ10						27
#define	FH_OBJ11						28
#define	FH_OBJ12						29
#define	FH_OBJ13						30
#define	FH_OBJ14						31
#define	FH_OBJ15						32

//----------------------------------------------------------------------
//	�C�x���g�i�s����t���O
//
//	(FE_FLAG_START - FH_FLAG_START)
//	(     80       -    600      )
//----------------------------------------------------------------------
#define FE_FLAG_START					(80)

#define FE_MAMA_01_P01_T101_R0101		(FE_FLAG_START+0)	//���ꂳ��ƈ�x��b����
#define FE_CLOCK_SET					(FE_FLAG_START+1)	//�����ݒ肵����
#define FE_ODAMAKI_01_P01_T101_R0301	(FE_FLAG_START+2)	//�|�P��������������H

#define FE_BASHA_01_P01_FIELD_C101		(FE_FLAG_START+3)	//�~�c����Ɖ�b����
#define FE_BASHA_01_P02_FIELD_C101		(FE_FLAG_START+4)	//�~�c���Ɖ�b����
#define FE_PAPA_01_P01_C101_R0201		(FE_FLAG_START+5)	//��������Ƃ̎n�߂Ẳ�b
#define FE_BALL_01_P01_SP_CONTEST		(FE_FLAG_START+6)	//�����X�^�[�{�[��

#define FE_WOMAN2_01_P01_T101_R0201		(FE_FLAG_START+7)	//�I�_�}�L�̍�
#define FE_STUDYM1_01_P01_T101_R0301	(FE_FLAG_START+8)	//�I�_�}�L�̏���P
#define FE_WORKERM1_01_C103_R0201		(FE_FLAG_START+9)	//�J�[�m�ƈ�x��b����

#define FE_CYCLE_01_P01_C103_R0201		(FE_FLAG_START+10)	//���]�ԓ���
#define FE_SCOOP_01_P01_FIELD_R115		(FE_FLAG_START+11)	//�X�R�b�v����
#define FE_ROPE_01_P01_FIELD_R114		(FE_FLAG_START+12)	//���[�v����
#define FE_KAMA_01_P01_FIELD_R119		(FE_FLAG_START+13)	//�J�}����
#define FE_ZYOURO_GET					(FE_FLAG_START+14)	//�W���E������
#define FE_CUBECASE_GET					(FE_FLAG_START+15)	//�L���[�u�P�[�X����
#define FE_HIMITSU_GET					(FE_FLAG_START+16)	//�Ђ݂̂��������(�Z�i���o�[���܂��Ȃ̂�)

#define FE_HOKAKUDEMO_01_P01_FIELD_R102	(FE_FLAG_START+17)	//�ߊl�f��
#define FE_SCHOOLM1_01_P01_FIELD_C103	(FE_FLAG_START+18)	//TV�N��OBJ
#define FE_GYM03_DOOR					(FE_FLAG_START+19)	//�L���Z�c�W���̃h�A�̏��
#define FE_GYM07_SWITCH_01				(FE_FLAG_START+20)	//�g�N�T�l�W���̃X�C�b�`�̏��
#define FE_GYM07_SWITCH_02				(FE_FLAG_START+21)	//�g�N�T�l�W���̃X�C�b�`�̏��
#define FE_GYM07_SWITCH_03				(FE_FLAG_START+22)	//�g�N�T�l�W���̃X�C�b�`�̏��
#define FE_GYM07_SWITCH_04				(FE_FLAG_START+23)	//�g�N�T�l�W���̃X�C�b�`�̏��

//#define FE_LETTER_POKE_01_C101_PC01	(FE_FLAG_START+24)	//�|�P�����A���P�[�g
#define FE_LETTER_RECENTLY_01			(FE_FLAG_START+25)	//�ŋ߂̏o����

#define FE_HWAZA04_01_T106_R0201		(FE_FLAG_START+26)	//�����肫04������
#define FE_HWAZA06_01_C103_R0301		(FE_FLAG_START+27)	//���킭������06������
#define FE_MITSURU_01_T106_R0201		(FE_FLAG_START+28)	//�×{���̃~�c�������Ȃ��Ȃ���
#define FE_HWAZA05_01_CAVE_D0502		(FE_FLAG_START+29)	//�t���b�V��05������
#define FE_HWAZA02_01_FIELD_R119		(FE_FLAG_START+30)	//������Ƃ�02������
#define FE_AKUA_01_CAVE_D0901			(FE_FLAG_START+31)	//������b����(�~�i���A�W�g)
#define FE_AKUA_01_CAVE_D0903			(FE_FLAG_START+32)	//���{�X�|����(�~�i���A�W�g)
#define FE_POKE1_01_CAVE_D1206			(FE_FLAG_START+33)	//�Ñ�|�P�����C�x���g�I��
#define FE_FREAK1_01_R114_R0102			(FE_FLAG_START+34)	//���΃}�j�A�C�x���g�I��
#define FE_INSEKI_GET					(FE_FLAG_START+35)	//�u���񂹂��v����
#define FE_MONSBALL_GET					(FE_FLAG_START+36)	//�����X�^�[�{�[��������(�~�V��)
#define FE_BIGMAN1_01_T107_R0201		(FE_FLAG_START+37)	//�A�C�e�������(�L�i�M)
#define FE_WOMAN1_01_C105_R0301			(FE_FLAG_START+38)	//�A�C�e�������(�q���}�L)
#define FE_WOMAN1_01_C107_R0401			(FE_FLAG_START+39)	//�A�C�e�������(�g�N�T�l)
//#define FE_TRAINERM1_01_C105_R0101		(FE_FLAG_START+40)	//�A�C�e�������(�q���}�L)
#define FE_FIGHTER1_01_C108_R0201		(FE_FLAG_START+41)	//�A�C�e�������(���l)
#define FE_HWAZA03_GET					(FE_FLAG_START+42)	//�Ȃ݂̂�03������
#define FE_HWAZA08_GET					(FE_FLAG_START+43)	//�_�C�r���O08������
//#define FE_KUSUNOKI_01_FIELD_C102		(FE_FLAG_START+44)	//�J�C�i�������C�x���g�I��
#define FE_LEADER08_01_FIELD_C108		(FE_FLAG_START+45)	//�~�N���ē�����
#define FE_MITSURU_01_CAVE_D1301		(FE_FLAG_START+46)	//�~�c���ɏ�������
#define FE_WOMAN1_01_R104_R0201			(FE_FLAG_START+47)	//����(�ԉ�)
#define FE_GIRL1_01_R104_R0201			(FE_FLAG_START+48)	//�O��(�ԉ�)
#define FE_POKE1_01_CAVE_D1111			(FE_FLAG_START+49)	//�s�v�c�ȓ��A�ŃC�x���g����
#define FE_SUPPORT_01_FIELD_R103		(FE_FLAG_START+50)	//�T�|�[�g�ɏ�������(���[�h�P�O�R)
#define FE_MAYUMI_01_R114_R0201			(FE_FLAG_START+51)	//�}���~�Ɖ�b����(���[�h�P�P�S)
#define FE_BOY4_01_FIELD_T102			(FE_FLAG_START+52)	//�A�C�e�������(�R�g�L)
#define FE_OMAMORIKOBAN_GET				(FE_FLAG_START+53)	//�u���܂��肱�΂�v����
#define FE_SODATE_TAMAGO				(FE_FLAG_START+54)	//��ĉ���������u���܂��v�ł���
#define FE_MIDDLEM2_01_C101_R0101		(FE_FLAG_START+55)	//�~�c�����Ɖ�b����
#define FE_HAIBUKURO_GET				(FE_FLAG_START+56)	//�u�͂��Ԃ���v����
#define FE_HWAZA01_GET					(FE_FLAG_START+57)	//����������01������
#define FE_BOY1_01_R116_R0101			(FE_FLAG_START+58)	//�H�H������
#define FE_BOSS_01_CAVE_D0701			(FE_FLAG_START+59)	//����ƂR�œG�̃{�X��|����
#define FE_ITEM_01_R109_R0101			(FE_FLAG_START+60)	//�C�̉ƂŃA�C�e���������
#define FE_WIN_01_R109_R0101			(FE_FLAG_START+61)	//�C�̉ƂŃg���[�i�[�S���ɏ�������
#define FE_DEBON_01_FIELD_C104			(FE_FLAG_START+62)	//����C�x���g��
#define FE_DEBON_02_FIELD_C104			(FE_FLAG_START+63)	//����C�x���g�I��
#define FE_DEBON_NIMOTSU_RETURN			(FE_FLAG_START+64)	//�f�{���̉ו���Ԃ��Ă���
#define FE_DEBON_NIMOTSU_GET			(FE_FLAG_START+65)	//�f�{���̂ɂ�����ɓ��ꂽ
//#define FE_DEBON_NIMOTSU_RETURN			(FE_FLAG_START+66)	//�f�{���̂ɂ���n����
#define FE_HAGI_01_R104_R0101			(FE_FLAG_START+67)	//�n�M�ƈ�x�b����
#define FE_MIDDLEM1_01_C102_R0101		(FE_FLAG_START+68)	//�c�K�ƈ�x�b����
#define FE_AQUA_01_C102_R0502			(FE_FLAG_START+69)	//���D���̃A�N�A�c�C�x���g�I��(+�f�{���̉ו���n����)
#define FE_KONTESUTOPASU_GET			(FE_FLAG_START+70)	//�R���e�X�g�p�X������
#define FE_POKE_GET_R119_R0101			(FE_FLAG_START+71)	//���V�C�������Ń|�P�����Q�b�g
#define FE_SUGOITURIZAO_GET				(FE_FLAG_START+72)	//�������肴���Q�b�g
#define FE_KOUKAN_01					(FE_FLAG_START+73)	//�����t���O�P
#define FE_KOUKAN_02					(FE_FLAG_START+74)	//�����t���O�Q
#define FE_KOUKAN_03					(FE_FLAG_START+75)	//�����t���O�R
//#define FE_HYOUKA_GET_ITEM01			(FE_FLAG_START+76)	//�]���A�C�e��
//#define FE_HYOUKA_GET_ITEM02			(FE_FLAG_START+77)	//�]���A�C�e��
//#define FE_HYOUKA_GET_ITEM03			(FE_FLAG_START+78)	//�]���A�C�e��
#define FE_WORKERM1_04_FIELD_C104		(FE_FLAG_START+79)	//��b����

#define FE_PICTURE_01_C106_R0202		(FE_FLAG_START+80)	//�����������G������
#define FE_PICTURE_02_C106_R0202		(FE_FLAG_START+81)	//���������G������
#define FE_PICTURE_03_C106_R0202		(FE_FLAG_START+82)	//���킢���G������
#define FE_PICTURE_04_C106_R0202		(FE_FLAG_START+83)	//���������G������
#define FE_PICTURE_05_C106_R0202		(FE_FLAG_START+84)	//�����܂����G������

#define FE_WAZAM_01_C104_R0201			(FE_FLAG_START+85)	//�Z�}�V���H�H
#define FE_WAZAM_01_T103_R0201			(FE_FLAG_START+86)	//�Z�}�V���H�H
#define FE_WAZAM_01_C103_R0101			(FE_FLAG_START+87)	//�Z�}�V���H�H
#define FE_WAZAM_01_T104_R0201			(FE_FLAG_START+88)	//�Z�}�V���H�H
#define FE_WAZAM_01_C101_R0201			(FE_FLAG_START+89)	//�Z�}�V���H�H
#define FE_WAZAM_01_C105_R0201			(FE_FLAG_START+90)	//�Z�}�V���H�H
#define FE_WAZAM_01_C107_R0101			(FE_FLAG_START+91)	//�Z�}�V���H�H
#define FE_WAZAM_01_C108_R0101			(FE_FLAG_START+92)	//�Z�}�V���H�H

#define FE_BASE_CHAR_FLAG				(FE_FLAG_START+93)	//������ԃL����
#define FE_BASE_GOODS_FLAG01			(FE_FLAG_START+94)	//�O�b�Y01
#define FE_BASE_GOODS_FLAG02			(FE_FLAG_START+95)	//
#define FE_BASE_GOODS_FLAG03			(FE_FLAG_START+96)	//
#define FE_BASE_GOODS_FLAG04			(FE_FLAG_START+97)	//
#define FE_BASE_GOODS_FLAG05			(FE_FLAG_START+98)	//
#define FE_BASE_GOODS_FLAG06			(FE_FLAG_START+99)	//
#define FE_BASE_GOODS_FLAG07			(FE_FLAG_START+100)	//
#define FE_BASE_GOODS_FLAG08			(FE_FLAG_START+101)	//
#define FE_BASE_GOODS_FLAG09			(FE_FLAG_START+102)	//
#define FE_BASE_GOODS_FLAG10			(FE_FLAG_START+103)	//
#define FE_BASE_GOODS_FLAG11			(FE_FLAG_START+104)	//
#define FE_BASE_GOODS_FLAG12			(FE_FLAG_START+105)	//
#define FE_BASE_GOODS_FLAG13			(FE_FLAG_START+106)	//
#define FE_BASE_GOODS_FLAG14			(FE_FLAG_START+107)	//�O�b�Y14

#define FE_POKEGEAR_GET					(FE_FLAG_START+108)	//�u�g���i�r�v����
#define FE_TEGAMI_CLEAR					(FE_FLAG_START+109)	//�莆�C�x���g�N���A
#define FE_MITSURU_02_FIELD_C103		(FE_FLAG_START+110)	//�L���Z�c�̃~�c�����V�_�P�Ɍ�������
#define FE_WOMAN1_01_T106_R0201			(FE_FLAG_START+111)	//��x�b����
#define FE_TUKINOISI_GET				(FE_FLAG_START+112)	//�u���̂����v����
#define FE_MITSURU_02_T106_R0201		(FE_FLAG_START+113)	//�e�ʂ̉ƂŃ~�c���Ƙb����

#define FE_KARAKURI10_SWITCH_01			(FE_FLAG_START+114)	//�J���N�����~�}�b�v�P�O�̖��ʂ̃X�C�b�`�P
#define FE_KARAKURI10_SWITCH_02			(FE_FLAG_START+115)	//�J���N�����~�}�b�v�P�O�̖��ʂ̃X�C�b�`�Q
#define FE_KARAKURI10_SWITCH_03			(FE_FLAG_START+116)	//�J���N�����~�}�b�v�P�O�̖��ʂ̃X�C�b�`�R
#define FE_KARAKURI10_SWITCH_04			(FE_FLAG_START+117)	//�J���N�����~�}�b�v�P�O�̖��ʂ̃X�C�b�`�S
#define FE_KARAKURI10_SWITCH_05			(FE_FLAG_START+118)	//�J���N�����~�}�b�v�P�O�̖��ʂ̃X�C�b�`�T

#define FE_KANASIDA_GO					(FE_FLAG_START+119)	//�J�i�V�_�g���l���J�ʂ���

#define	FE_DAISUKI_GOODS_FLAG01			(FE_FLAG_START+120)	//�������悳�A�C�e��
#define	FE_DAISUKI_GOODS_FLAG02			(FE_FLAG_START+121)	//���������A�C�e��
#define	FE_DAISUKI_GOODS_FLAG03			(FE_FLAG_START+122)	//���킢���A�C�e��
#define	FE_DAISUKI_GOODS_FLAG04			(FE_FLAG_START+123)	//���������A�C�e��
#define	FE_DAISUKI_GOODS_FLAG05			(FE_FLAG_START+124)	//�����܂����A�C�e��

#define	FE_MUGENNOTIKETTO_GET			(FE_FLAG_START+125)	//�u�ނ���̃`�P�b�g�v����
#define	FE_KOKORO_BATTLE				(FE_FLAG_START+126)	//��̌Ǔ��Ő퓬����
//#define	FE_NEW_KINSETSU					(FE_FLAG_START+127)	//�j���[�L���Z�c�C�x���g�N���A
#define	FE_TIKANOKAGI_GET				(FE_FLAG_START+128)	//�u�����̃J�M�v����
#define	FE_WAZAM24_GET					(FE_FLAG_START+129)	//�u�Z�}�V���Q�S�v����
//#define	FE_HATSUDEN_SW					(FE_FLAG_START+130)	//���d�X�C�b�`������
//#define	FE_TIKANOKAGI_USE				(FE_FLAG_START+131)	//�����̃J�M���g�p����
#define	FE_TAMA_GET						(FE_FLAG_START+132)	//�u�ׂɂ���E��������̂��܁v����

#define FE_PUREMIABOORU_GET				(FE_FLAG_START+133)	//�u�v���~�A�{�[���v����

#define FE_SINKAI_ITEM_GET				(FE_FLAG_START+134)	//�u���񂩂��̃L�o�v���u���񂩂��̂��낱�v�����

#define FE_BOY1_FESTA					(FE_FLAG_START+135)	//���N�Ƙb����(�t�F�X�^)
#define FE_GIRL1_FESTA					(FE_FLAG_START+136)	//�����Ƙb����(�t�F�X�^)

#define FE_TREASURE_HUNTER				(FE_FLAG_START+137)	//�ƈ�x�b����
#define FE_EVILM1_01_FIELD_C106			(FE_FLAG_START+138)	//�A�N�A�c�Ɖ�b����
#define FE_MAGMABOSS_01_CAVE_D0701_TALK	(FE_FLAG_START+139)	//�}�O�}�c�{�X�Ɖ�b����(����ƂR)
#define FE_SORAISI_01_T105_R0201		(FE_FLAG_START+140)	//�\���C�V���m�Ɖ�b����

#define FE_GOOGURU_GET					(FE_FLAG_START+141)	//�S�[�S�[�S�[�O������

#define FE_BOY3_01_C105_R0501			(FE_FLAG_START+142)	//���|�P��������ї�����(�q���}�L����)
#define FE_BOY3_02_C105_R0501			(FE_FLAG_START+143)	//�A�C�e��������
#define FE_POKE_01_C107_R0301			(FE_FLAG_START+144)	//���|�P��������ї�����(�g�N�T�l����)

#define FE_COIN_20_GET_C103_R0401		(FE_FLAG_START+145)	//�R�C���Q�O�����Q�b�g
#define FE_GOODS_GET_C103_R0401			(FE_FLAG_START+146)	//�O�b�Y���Q�b�g

#define FE_IITURIZAO_GET				(FE_FLAG_START+147)	//�����肴���Q�b�g
#define FE_DENSETU_OPEN					(FE_FLAG_START+148)	//�`�����A�J������

#define FE_WAZAM27_GET					(FE_FLAG_START+149)	//�u�Z�}�V���Q�V�v����
#define FE_WAZAM36_GET					(FE_FLAG_START+150)	//�u�Z�}�V���R�U�v����
#define FE_WAZAM05_GET					(FE_FLAG_START+151)	//�u�Z�}�V���O�T�v����
#define FE_WAZAM19_GET					(FE_FLAG_START+152)	//�u�Z�}�V���P�X�v����
#define FE_WAZAM04_GET					(FE_FLAG_START+153)	//�u�Z�}�V���O�S�v����
#define FE_WAZAM44_GET					(FE_FLAG_START+154)	//�u�Z�}�V���S�S�v����
#define FE_WAZAM45_GET					(FE_FLAG_START+155)	//�u�Z�}�V���S�T�v����

#define FE_GOODS_GET_C106_R0202			(FE_FLAG_START+156)	//�K���X�̃I�u�W�F����
#define FE_GINNOTATE_GET				(FE_FLAG_START+157)	//�u����̂��āv����
#define FE_KINNOTATE_GET				(FE_FLAG_START+158)	//�u����̂��āv����

#define FE_SOUKO_OPEN_CAVE_D1704		(FE_FLAG_START+159)	//�������̔����J����(�����g�����j

#define FE_DOOR_OPEN_01_CAVE_D1712		(FE_FLAG_START+160)	//�P�����̔����J����(�����g�����j
#define FE_DOOR_OPEN_02_CAVE_D1712		(FE_FLAG_START+161)	//�Q�����̔����J����(�����g�����j
#define FE_DOOR_OPEN_04_CAVE_D1712		(FE_FLAG_START+162)	//�S�����̔����J����(�����g�����j
#define FE_DOOR_OPEN_06_CAVE_D1712		(FE_FLAG_START+163)	//�U�����̔����J����(�����g�����j

#define FE_SORAISI_01_CAVE_D0101		(FE_FLAG_START+164)	//�\���C�V���m�Ɖ�b����(�����̑�)
#define FE_HOERUKODOORU_GET				(FE_FLAG_START+165)	//�u�z�G���R�h�[���v����
#define FE_ORENNOMI_GET					(FE_FLAG_START+166)	//�u�I�����̂݁v����
#define FE_WIN_01_SP_SHIP01				(FE_FLAG_START+167)	//�D�̏�őS���ɏ�������

#define FE_KINOMI_A_GET					(FE_FLAG_START+168)	//�؂̎��`����
#define FE_KINOMI_B_GET					(FE_FLAG_START+169)	//�؂̎��a����
#define FE_KINOMI_C_GET					(FE_FLAG_START+170)	//�؂̎��b����
#define FE_KINOMI_D_GET					(FE_FLAG_START+171)	//�؂̎��c����
#define FE_KINOMI_E_GET					(FE_FLAG_START+172)	//�؂̎��d����

#define FE_SIROISUKAAHU_GET				(FE_FLAG_START+173)	//�����X�J�[�t�Q�b�g
#define FE_MOKUTAN_GET					(FE_FLAG_START+174)	//�u��������v����
#define FE_MOVE_POKE_WATCH				(FE_FLAG_START+175)	//�ړ��|�P�����j���[�X����
#define FE_SHOP_01_C104_FS01			(FE_FLAG_START+176)	//�V�^�{�[���̘b�𕷂���
#define FE_BORONOTURIZAO_GET			(FE_FLAG_START+177)	//�u�{���̂肴���v����
#define FE_KOINKEESU_GET				(FE_FLAG_START+178)	//�u�R�C���P�[�X�v����
#define FE_OLDWOMAN1_01_CAVE_D0808		(FE_FLAG_START+179)	//���܂�Ԃ���
#define FE_WAZAM49_GET					(FE_FLAG_START+180)	//�u�Z�}�V���S�X�v����
#define FE_WAZAM28_GET					(FE_FLAG_START+181)	//�u�Z�}�V���Q�W�v����
#define FE_WAZAM09_GET					(FE_FLAG_START+182)	//�u�Z�}�V���O�X�v����

#define FE_TRAINERM1_01_C109_R0206		(FE_FLAG_START+183)	//�|�P�������[�O�̃X�g�b�p�[����

#define FE_WAZAM10_GET					(FE_FLAG_START+184)	//�u�Z�}�V���P�O�v����
#define FE_WAZAM41_GET					(FE_FLAG_START+185)	//�u�Z�}�V���S�P�v����

#define FE_SOONANO_TAMAGO_GET			(FE_FLAG_START+186)	//�\�[�i�m�̃^�}�S����
#define FE_KASEKI_RETURN				(FE_FLAG_START+187)	//�u�J�Z�L�v���Đ�������

#define FE_BASE_ENTRY					(FE_FLAG_START+188)	//�閧��n��o�^����

#define FE_WAZAM46_GET					(FE_FLAG_START+189)	//�u�Z�}�V���S�U�v����

#define FE_CONTEST_SUPER_WIN			(FE_FLAG_START+190)	//�X�[�p�[�����N�D��
#define FE_KUSUNOKI_01_C102_R0601		(FE_FLAG_START+191)	//�N�X�m�L�͒�(�D����)
#define FE_GAKUSYUUSOUTI_GET			(FE_FLAG_START+192)	//�u�������イ�������v����
#define FE_PC_POKERUS					(FE_FLAG_START+193)	//�|�P���X�ɏ��߂Ċ�������

#define FE_DASH_SHOES_GET				(FE_FLAG_START+194)	//�_�b�V���V���[�Y����
#define FE_SENSEINOTUME_GET				(FE_FLAG_START+195)	//�u���񂹂��̂߁v����
#define FE_OUZYANOSIRUSI_GET			(FE_FLAG_START+196)	//�u��������̂��邵�v����
#define FE_KYOUSEIGIBUSU_GET			(FE_FLAG_START+197)	//�u���傤�����M�u�X�v����
#define FE_YASURAGINOSUZU_GET			(FE_FLAG_START+198)	//�u�₷�炬�̃X�Y�v����
#define FE_SIROIHAABU_GET				(FE_FLAG_START+199)	//�u���낢�n�[�u�v����
#define FE_YAWARAKAISUNA_GET			(FE_FLAG_START+200)	//�u���炩�����ȁv����
#define FE_KAKURE_01_FIELD_R120			(FE_FLAG_START+201)	//�J�N���I���Ƙb����
#define FE_KIYOMENOOHUDA_GET			(FE_FLAG_START+202)	//�u����߂̂��ӂ��v����
#define FE_KIAINOHATIMAKI_GET			(FE_FLAG_START+203)	//�u�������̂͂��܂��v����
#define FE_MITSURU_01_FIELD_C103		(FE_FLAG_START+204)	//�~�c���ƈ�x�b����
#define FE_DEBONSUKOOPU_GET				(FE_FLAG_START+205)	//�u�f�{���X�R�[�v�v����
#define FE_SUPPORT_01_FIELD_C106		(FE_FLAG_START+206)	//�T�|�[�g�Ɖ�b����

#define FE_WORKERM1_01_FIELD_R116		(FE_FLAG_START+207)	//�f�{���Ј��ƈ�x��b����
#define FE_SUPPORT_01_FIELD_C104		(FE_FLAG_START+208)	//�T�|�[�g�ƈ�x��b����
#define FE_SIRUKUNOSUKAAHU_GET			(FE_FLAG_START+209)	//�u�V���N�̃X�J�[�t�v����

#define FE_DEBON_01_FIELD_R120			(FE_FLAG_START+210)	//120�ԓ��H�Ńf�{���ƈ�x��b����
#define FE_HUNENOTIKETTO_GET			(FE_FLAG_START+211)	//�u�ӂ˂̃`�P�b�g�v����
#define FE_SUPPORT_02_FIELD_C106		(FE_FLAG_START+212)	//�~�i���̃T�|�[�g�C�x���g�I��
#define FE_SUPPORT_01_T101_R0202		(FE_FLAG_START+213)	//����ŃT�|�[�g�Ɖ�b����(�~�i����)
#define FE_TANTIKI_RETURN				(FE_FLAG_START+214)	//�T�m�@��Ԃ���
#define FE_KAKURE_01_FIELD_C105			(FE_FLAG_START+215)	//�q���}�L�̃J�N���I���C�x���g�I��
#define FE_BATCH_04_GET					(FE_FLAG_START+216)	//�o�b�`���S�����Ă���
#define FE_KISEKINOTANE_GET				(FE_FLAG_START+217)	//�u�������̃^�l�v����
#define FE_POKE1_01_C107_R0501			(FE_FLAG_START+218)	//�u�_���o���v�Q�b�g
#define FE_WAZA_GET_T107_R0201			(FE_FLAG_START+219)	//�u�Z�}�V���Q�P�E�Q�V�v����
#define FE_GENTLEM1_01_T107_R0201		(FE_FLAG_START+220)	//��b����
#define FE_SMAMA_01_T101_R0201			(FE_FLAG_START+221)	//�T�|�[�g��̃��b�Z�[�W��ύX����
#define FE_HWAZA08_TALK_GET				(FE_FLAG_START+222)	//�_�C�S���璼�ځu��`�}�V���v���������


//----------------------------------------------------------------------
//	�B���A�C�e���p
//
//	(FH_FLAG_START - FV_FLAG_START)
//	(    600       -    700      )
//----------------------------------------------------------------------
#define FH_FLAG_START					(600)


//----------------------------------------------------------------------
//	�o�j�b�V���t���O
//
//	(FV_FLAG_START - FT_FLAG_START)
//	(    700       -    1200      )
//----------------------------------------------------------------------
#define FV_FLAG_START					(700)

#define FV_BAG1_01_FIELD_R101			(FV_FLAG_START+0)	//�I�_�}�L�̃����b�N
//#define FV_CONTEST_00_T105_R0102		(FV_FLAG_START+4)	//�R���e�X�g�̃L�����S��
//#define FV_CONTEST_01_C106_R0302		(FV_FLAG_START+5)	//�R���e�X�g�̃L����01
//#define FV_CONTEST_02_C106_R0302		(FV_FLAG_START+6)	//�R���e�X�g�̃L����02
//#define FV_CONTEST_03_C106_R0302		(FV_FLAG_START+7)	//�R���e�X�g�̃L����03
//#define FV_CONTEST_04_C106_R0302		(FV_FLAG_START+8)	//�R���e�X�g�̃L����04

#define FV_RECEPTION01_01_SP_PC02		(FV_FLAG_START+9)	//PC�Q�K�󂯕t��
#define FV_RECEPTION01_02_SP_PC02		(FV_FLAG_START+10)	//PC�Q�K�󂯕t��
#define FV_RECEPTION02_01_SP_PC02		(FV_FLAG_START+11)	//PC�Q�K�󂯕t��
#define FV_RECEPTION02_02_SP_PC02		(FV_FLAG_START+12)	//PC�Q�K�󂯕t��
#define FV_RECEPTION03_01_SP_PC02		(FV_FLAG_START+13)	//PC�Q�K�󂯕t��
#define FV_RECEPTION03_02_SP_PC02		(FV_FLAG_START+14)	//PC�Q�K�󂯕t��

#define FV_ODAMAKI_01_FIELD_R101		(FV_FLAG_START+20)	//�I�_�}�L(���[�h�P�O�P)
#define FV_ODAMAKI_01_T101_R0301		(FV_FLAG_START+21)	//�I�_�}�L(������)
#define FV_SUPPORTW_01_T101_R0202		(FV_FLAG_START+22)	//�I�_�}�L��(�Q�K)
#define FV_SUPPORT_01_FIELD_R103		(FV_FLAG_START+23)	//�T�|�[�g(���[�h�P�O�R)
#define FV_WORKERM1_01_CAVE_D0601		(FV_FLAG_START+24)	//�g���i�r�����l
#define FV_SEAMAN1_01_CAVE_D0601		(FV_FLAG_START+25)	//�}�����c(���̃C�x���g)
#define FV_MITSURU_01_FIELD_C101		(FV_FLAG_START+26)	//�~�c��
#define FV_NOTE_01_C107_R0501			(FV_FLAG_START+27)	//�_�C�S�̃m�[�g
#define FV_WOMAN2_01_FIELD_C101			(FV_FLAG_START+28)	//�~�c���̂��ꂳ��
//#define FV_SEAMAN1_01_FIELD_C104		(FV_FLAG_START+30)	//�}�����c�P(�|�P���������)
#define FV_SEAMAN1_02_FIELD_C104		(FV_FLAG_START+31)	//�}�����c�Q(�|�P���������)
#define FV_WORKERM1_04_FIELD_C104		(FV_FLAG_START+32)	//�Ј�(�|�P���������)
#define FV_POKE1_02_CAVE_D1111			(FV_FLAG_START+33)	//���Ñ�|�P�����V���G�b�g(�C�ꓴ�A)
#define FV_SENRI_01_T101_R0101			(FV_FLAG_START+34)	//�Z����(����)
//#define FV_WORKERM1_01_FIELD_C104		(FV_FLAG_START+35)	//�J�i�Y�~�X�g�b�p�[
//#define FV_WORKERM1_02_FIELD_C104		(FV_FLAG_START+36)	//�J�i�Y�~�X�g�b�p�[
#define FV_POKE1_01_CAVE_D0601			(FV_FLAG_START+37)	//�n�M�V�l�̃|�P����
#define FV_HAGI_01_FIELD_R104			(FV_FLAG_START+38)	//�n�M�V�l
#define FV_HAGI_01_R104_R0101			(FV_FLAG_START+39)	//�n�M�V�l(����)
#define FV_HAGI_01_FIELD_T103			(FV_FLAG_START+40)	//�n�M�V�l(����)
#define FV_HAGI_01_FIELD_R109			(FV_FLAG_START+41)	//�n�M�V�l(���[�h�P�O�X)
#define FV_FUNE_01_FIELD_R104			(FV_FLAG_START+42)	//�n���D(���[�h�P�O�S)
#define FV_FUNE_01_FIELD_T103			(FV_FLAG_START+43)	//�n���D(����)
#define FV_FUNE_01_FIELD_R109			(FV_FLAG_START+44)	//�n���D(���[�h�P�O�X)
//#define FV_DEVON1_01_FIELD_C104		(FV_FLAG_START+45)	//�J�i�Y�~�X�g�b�p�[(�ړ���)
//#define FV_DEVON1_02_FIELD_C104		(FV_FLAG_START+46)	//�J�i�Y�~�X�g�b�p�[(�ړ���)
#define FV_EVILM1_01_CAVE_D0601			(FV_FLAG_START+47)	//�A�N�A�c
#define FV_POKE1_02_CAVE_D0601			(FV_FLAG_START+48)	//�n�M�V�l�̃|�P����(�����ȓ��A)
#define FV_EVILM1_02_CAVE_D0601			(FV_FLAG_START+49)	//�A�N�A�c(�����ȓ��A)
#define FV_POKE1_01_FIELD_R101			(FV_FLAG_START+50)	//�I�_�}�L���m�ǂ�������|�P����
#define FV_MITSURU_02_CAVE_D1301		(FV_FLAG_START+51)	//�~�c��(�`�����s�I�����[�h)
#define FV_MAMA_01_FIELD_T101			(FV_FLAG_START+52)	//���z�����̂��ꂳ��(�j�̎q)
#define FV_PAPA_01_OPENING_ROOM01		(FV_FLAG_START+53)	//���X�g�b�p�[�̈ʒu
#define FV_POKE1_01_OPENING_ROOM01		(FV_FLAG_START+54)	//���z����`���|�P����(�P�K)
#define FV_POKE1_02_OPENING_ROOM01		(FV_FLAG_START+55)	//���z����`���|�P����(�P�K)
#define FV_PAPA_02_OPENING_ROOM01		(FV_FLAG_START+56)	//���d���ɏo�|����
#define FV_MAMA_01_OPENING_ROOM02		(FV_FLAG_START+57)	//���z����`���|�P����(�Q�K)
#define FV_MAMA_01_T101_R0101			(FV_FLAG_START+58)	//��(�j�̎q�̉�)
#define FV_MAMA_01_T101_R0201			(FV_FLAG_START+59)	//��(���̎q�̉�)
#define FV_SUPPORTM_01_T101_R0102		(FV_FLAG_START+60)	//�I�_�}�L���q(�Q�K)
#define FV_TRUCK1_01_FIELD_T101			(FV_FLAG_START+61)	//���z���g���b�N
#define FV_TRUCK1_02_FIELD_T101			(FV_FLAG_START+62)	//���z���g���b�N
//#define FV_LETTER_GYM_01_FIELD_C104		(FV_FLAG_START+63)	//�J�i�Y�~
//#define FV_INTERVIEW_01_C101_PC01		(FV_FLAG_START+64)	//�g�E�Jpc�C���^�r���[�̐l
//#define FV_INTERVIEW_01_C102_PC01		(FV_FLAG_START+65)	//�J�C�ipc�C���^�r���[�̐l
//#define FV_INTERVIEW_01_C103_PC01		(FV_FLAG_START+66)	//�L���Z�cpc�C���^�r���[�̐l
//#define FV_INTERVIEW_01_C104_PC01		(FV_FLAG_START+67)	//�J�i�Y�~pc�C���^�r���[�̐l
#define FV_MIDDLEM1_01_FIELD_R111		(FV_FLAG_START+68)	//�o�g�����
#define FV_MIDDLEW1_01_FIELD_R111		(FV_FLAG_START+69)	//�o�g�����
#define FV_SCHOOLW1_01_FIELD_R111		(FV_FLAG_START+70)	//�o�g�����
#define FV_OLDWOMAN1_01_FIELD_R111		(FV_FLAG_START+71)	//�o�g�����
#define FV_PAPA_01_C101_R0201			(FV_FLAG_START+72)	//�W����������
#define FV_POKE1_01_CAVE_D2308			(FV_FLAG_START+73)	//�^�C�N�[��
#define FV_ARTIST_01_C106_R0301			(FV_FLAG_START+74)	//�R���e�X�g�̊G�`��
#define FV_GENTLEM1_01_C106_R0201		(FV_FLAG_START+75)	//���p�قْ̊�
#define FV_GALLERY_01_C106_R0201		(FV_FLAG_START+76)	//���p�ق̋q�i�X�e�b�v�P�j
#define FV_GALLERY_02_C106_R0201		(FV_FLAG_START+77)	//���p�ق̋q�i�X�e�b�v�Q�j
#define FV_GALLERY_03_C106_R0201		(FV_FLAG_START+78)	//���p�ق̋q�i�X�e�b�v�R�j
#define FV_GALLERY_04_C106_R0201		(FV_FLAG_START+79)	//���p�ق̋q�i�X�e�b�v�S�j
#define FV_GALLERY_05_C106_R0201		(FV_FLAG_START+80)	//���p�ق̋q�i�X�e�b�v�T�j
#define FV_DANPEI_01_C101_R0201			(FV_FLAG_START+81)	//�����̃`�����s�H�������l(�g�E�J)
#define FV_MAMA_02_T101_R0101			(FV_FLAG_START+82)	//��ړ�(�j�̎q�̉�)
#define FV_MAMA_02_T101_R0201			(FV_FLAG_START+83)	//��ړ�(���̎q�̉�)
#define FV_WOMAN2_01_T101_R0101			(FV_FLAG_START+84)	//�ׂ̉Ƃ̂��ꂳ��
#define FV_WOMAN2_01_T101_R0201			(FV_FLAG_START+85)	//�ׂ̉Ƃ̂��ꂳ��
//#define FV_BALL1_01_FIELD_R102			(FV_FLAG_START+86)	//����������
//#define FV_BALL1_01_FIELD_R116			(FV_FLAG_START+87)	//����������
//#define FV_BALL1_02_FIELD_R116			(FV_FLAG_START+88)	//����������
#define FV_FAN_01_C106_R0401			(FV_FLAG_START+89)	//�t�@���P
#define FV_FAN_02_C106_R0401			(FV_FLAG_START+90)	//�t�@���Q
#define FV_FAN_03_C106_R0401			(FV_FLAG_START+91)	//�t�@���R
#define FV_FAN_04_C106_R0401			(FV_FLAG_START+92)	//�t�@���S
//#define FV_BALL1_01_CAVE_D0601			(FV_FLAG_START+93)	//����������
//#define FV_BALL1_02_CAVE_D0601			(FV_FLAG_START+94)	//����������
//#define FV_BALL1_03_CAVE_D0601			(FV_FLAG_START+95)	//����������
#define FV_INTERVW1_01_CAMERA			(FV_FLAG_START+96)	//�C���^�r���A�[���J�����}��(r111)
#define FV_INTERVW1_02_CAMERA			(FV_FLAG_START+97)	//�C���^�r���A�[���J�����}��(r118)
#define FV_INTERVW1_03_CAMERA			(FV_FLAG_START+98)	//�C���^�r���A�[���J�����}��(r120)
#define FV_INTERVW1_04_CAMERA			(FV_FLAG_START+99)	//�C���^�r���A�[���J�����}��(r111)
#define FV_INTERVM1_01_T105_R0101		(FV_FLAG_START+100)	//�R���e�X�g��̃C���^�r���[
#define FV_INTERVM1_01_T106_R0101		(FV_FLAG_START+101)	//�R���e�X�g��̃C���^�r���[
#define FV_INTERVM1_01_C106_R0301		(FV_FLAG_START+102)	//�R���e�X�g��̃C���^�r���[
#define FV_INTERVM1_01_C102_R0201		(FV_FLAG_START+103)	//�R���e�X�g��̃C���^�r���[
#define FV_MITSURU_01_FIELD_C103		(FV_FLAG_START+104)	//�~�c��(�L���Z�c�W���O)
#define FV_MIDDLEM2_01_FIELD_C103		(FV_FLAG_START+105)	//��������(�L���Z�c�W���O)
#define FV_MITSURU_01_T106_R0201		(FV_FLAG_START+106)	//�~�c��(�×{��)
#define FV_FIGHTER1_01_CAVE_D0201		(FV_FLAG_START+107)	//�g���l���j(�J�i�V�_�g���l��)
#define FV_FIGHTER1_01_T106_R0201		(FV_FLAG_START+108)	//�g���l���j(�×{��)
#define FV_MIDDLEM2_01_T106_R0201		(FV_FLAG_START+109)	//��������(�×{��)
#define FV_AKUA_01_CAVE_D0701			(FV_FLAG_START+110)	//�A�N�A�c(����ƂR)
#define FV_MIDDLEM1_03_FIELD_C101		(FV_FLAG_START+111)	//�~�c����(�W���O)
#define FV_MIDDLEW1_03_FIELD_C101		(FV_FLAG_START+112)	//�~�c����(�W���O)
#define FV_FREAK1_01_FIELD_R116			(FV_FLAG_START+113)	//�}�j�A(���[�h�P�P�U)
#define FV_SUPPORT_01_FIELD_C104		(FV_FLAG_START+114)	//�T�|�[�g(�J�i�Y�~)
#define FV_LEADER08_01_C108_R0101		(FV_FLAG_START+115)	//�~�N��(�W��)
#define FV_LEADER08_01_FIELD_C108		(FV_FLAG_START+116)	//�~�N��(�Ւd�O)
//#define FV_BALL1_01_CAVE_D1205			(FV_FLAG_START+117)	//�����̂ڂ�(��`�}�V��)
//#define FV_AKUA_01_R114_R0102			(FV_FLAG_START+118)	//�A�N�A�c(���΃}�j�A�̓��A)
#define FV_AKUA_01_FIELD_R112			(FV_FLAG_START+119)	//�A�N�A�c(���[�v�E�F�C�O)
#define FV_MAGMA_01_FIELD_R112			(FV_FLAG_START+120)	//�}�O�}�c(�t�G���ւ̌�����)
#define FV_AKUA_01_CAVE_D0901			(FV_FLAG_START+121)	//�A�N�A�c(�A�W�g������)
#define FV_AKUA_02_CAVE_D0901			(FV_FLAG_START+122)	//�A�N�A�c(�A�W�g������)
#define FV_KUSUNOKI_01_C102_R0102		(FV_FLAG_START+123)	//�N�X�m�L�͒�(���D��)
//#define FV_AKUA_01_CAVE_D1206			(FV_FLAG_START+124)	//�A�N�A�c(�Ւd�Ő[��)
#define FV_POKE1_01_CAVE_D1206			(FV_FLAG_START+125)	//�Ñ�|�P����(�Ւd�Ő[��)
#define FV_FREAK1_01_R114_R0102			(FV_FLAG_START+126)	//���΃}�j�A(���A��)
#define FV_FREAK1_01_R114_R0101			(FV_FLAG_START+127)	//���΃}�j�A(����)
#define FV_AQUABOSS_01_CAVE_D1111		(FV_FLAG_START+128)	//�A�I�M��(�s�v�c�ȓ��A)
#define FV_MAGMABOSS_01_CAVE_D1111		(FV_FLAG_START+129)	//�}�c�u�T(�s�v�c�ȓ��A)
//#define FV_AQUA_01_CAVE_D1111R			(FV_FLAG_START+130)	//����(�s�v�c�ȓ��A)
#define FV_MAGMA_01_CAVE_D1111S			(FV_FLAG_START+131)	//����(�s�v�c�ȓ��A)
//#define FV_BALL1_01_CAVE_D0503			(FV_FLAG_START+132)	//����������
#define FV_DAIGO_01_CAVE_D0504			(FV_FLAG_START+133)	//�_�C�S(�������A)
#define FV_DAIGO_01_FIELD_R128			(FV_FLAG_START+134)	//�_�C�S(���[�h�P�Q�W)
#define FV_CAMERA1_01_FIELD_C102		(FV_FLAG_START+135)	//�J�����}��(�J�C�i)
#define FV_EVILM1_ALL_C106_R0801		(FV_FLAG_START+136)	//����(�~�i���̕���)
//#define FV_SEAMAN1_02_FIELD_C102		(FV_FLAG_START+137)	//�D���(�J�C�i)
//#define FV_EVILM1_03_C106_R0801			(FV_FLAG_START+138)	//����(�~�i���̕���)
//#define FV_OLDMAN1_01_FIELD_C108		(FV_FLAG_START+139)	//�X�g�b�p�[(�Ւd�O)
#define FV_KUSUNOKI_01_FIELD_C102		(FV_FLAG_START+140)	//�N�X�m�L�͒�(�J�C�i)
#define FV_KUSUNOKI_01_C102_R0601		(FV_FLAG_START+141)	//�N�X�m�L�͒�(�D����)
//#define FV_SEAMAN1_02_C102_R0601		(FV_FLAG_START+142)	//�D���(�D����)
#define FV_INTERVW1_01_C102_R0601		(FV_FLAG_START+143)	//�C���^�r���A�[(�D����)
#define FV_CAMERA1_01_C102_R0601		(FV_FLAG_START+144)	//�J�����}��(�D����)
#define FV_EVILM1_01_C102_R0601			(FV_FLAG_START+145)	//�A�N�A�c(�D����)
#define FV_AQUABOSS_01_C102_R0601		(FV_FLAG_START+146)	//�A�I�M��(�D����)
#define FV_HAGI_01_C102_R0601			(FV_FLAG_START+147)	//�n�M�V�l(�D����)
#define FV_SUBMARINE1_01_C102_R0601		(FV_FLAG_START+148)	//�D(�D����)
#define FV_DOLL1_01_T101_R0202			(FV_FLAG_START+149)	//�h�[��(�T�|�[�g����)
#define FV_FUNE_01_C102_R0601			(FV_FLAG_START+150)	//������(�D����)
#define FV_SUPPORT_01_FIELD_R119		(FV_FLAG_START+151)	//�T�|�[�g(���[�h�P�P�X)
#define FV_EVILM1_ALL_FIELD_C106		(FV_FLAG_START+152)	//�A�N�A�c(�~�i��)
//#define FV_FS_ALL_C108_FS01				(FV_FLAG_START+153)	//�����ؼ���߂̋q(���l)
#define FV_OBJ_ALL_FIELD_C108			(FV_FLAG_START+154)	//���l��OBJ(���l)
#define FV_WORKERW2_01_FIELD_C108		(FV_FLAG_START+155)	//�g���[�i�[(�Ւd�O)
#define FV_WORKERW2_02_FIELD_C108		(FV_FLAG_START+156)	//�g���[�i�[(�Ւd�O)
//#define FV_MAGMA_01_CAVE_D1206			(FV_FLAG_START+157)	//�}�O�}�c(�Ւd�Ő[��)
#define FV_MITSURU_01_CAVE_D1301		(FV_FLAG_START+158)	//�~�c��(�`�����s�I�����[�h)
#define FV_POKE1_01_CAVE_D1111			(FV_FLAG_START+159)	//���Ñ�|�P����(�s�v�c�ȓ��A)
#define FV_HISHIP1_01_C102_R0601		(FV_FLAG_START+160)	//�����D(�J�C�i�`�~�i����)
#define FV_HISHIP1_01_C106_R0501		(FV_FLAG_START+161)	//�����D(�~�i���`�J�C�i��)
//#define FV_MIDDLEM1_01_FIELD_C108		(FV_FLAG_START+162)	//���l��OBJ(middlem1,woman2)
//#define FV_BALL1_01_CAVE_D0501			(FV_FLAG_START+163)	//���Ȃʂ��̂Ђ�
//#define FV_BALL1_01_CAVE_D0502			(FV_FLAG_START+164)	//����������
//#define FV_BALL1_01_FIELD_R115			(FV_FLAG_START+165)	//����������
#define FV_MITSURU_01_C101_R0201		(FV_FLAG_START+166)	//�~�c��(�g�E�J�W��)
#define FV_MITSURU_01_FIELD_R102		(FV_FLAG_START+167)	//�~�c��(���[�h�P�O�Q)
#define FV_BIGMAN1_01_FIELD_T101		(FV_FLAG_START+168)	//�r�b�O�}��(�~�V��)
#define FV_HAGI_01_C102_R0101			(FV_FLAG_START+169)	//�n�M(���D��)
#define FV_MAYUMI_01_R114_R0201			(FV_FLAG_START+170)	//�}���~(���[�h�P�P�S)
#define FV_MAYUMI_01_T105_PC01			(FV_FLAG_START+171)	//�}���~(�V�_�P)
#define FV_MIDDLEM1_01_R110_R0101		(FV_FLAG_START+172)	//�J���N���剤
//#define FV_AQUA_01_CAVE_D0701S			(FV_FLAG_START+173)	//�A�N�A�c(��)
//#define FV_MAGMA_01_CAVE_D0701S			(FV_FLAG_START+174)	//�}�O�}�c(��)
//#define FV_AQUA_01_CAVE_D0701R			(FV_FLAG_START+175)	//�A�N�A�c(��)
//#define FV_MAGMA_01_CAVE_D0701R			(FV_FLAG_START+176)	//�}�O�}�c(��)
#define FV_ALL_01_CAVE_D0701			(FV_FLAG_START+177)	//�A�N�A�A�}�O�}�ȊO�̐l�B
#define FV_AQUAM1_01_CAVE_D0201			(FV_FLAG_START+178)	//�A�N�A�c(�J�i�V�_�g���l��)
#define FV_HAGI_01_CAVE_D0201			(FV_FLAG_START+179)	//�n�M(�J�i�V�_�g���l��)
#define FV_POKEMON_01_CAVE_D0201		(FV_FLAG_START+180)	//�s�[�R(�J�i�V�_�g���l��)
#define FV_POKEMON_01_R104_R0101		(FV_FLAG_START+181)	//�s�[�R(����)
#define FV_EVIL_ALL_FIELD_C102			(FV_FLAG_START+182)	//���D���O�̃A�N�A�c�B
#define FV_AQUA_ALL_C102_R0501			(FV_FLAG_START+183)	//���D���P�K�̃A�N�A�c�B
#define FV_AQUAM1_01_C102_R0502			(FV_FLAG_START+184)	//���D���Q�K�̃A�N�A�c
#define FV_AQUAM1_02_C102_R0502			(FV_FLAG_START+185)	//���D���Q�K�̃A�N�A�c
#define FV_AQUABOSS_01_C102_R0502		(FV_FLAG_START+186)	//���D���Q�K�̃A�N�A�c�{�X
#define FV_KUSUNOKI_01_C102_R0502		(FV_FLAG_START+187)	//���D���Q�K�̃N�X�m�L
#define FV_BATTLE_TOWER_TRAINER			(FV_FLAG_START+188)	//�o�g���^���[�̐l
#define FV_SUPPORT_01_T101_R0301		(FV_FLAG_START+189)	//�T�|�[�g(������)
#define FV_AQUA_ALL_FIELD_R119			(FV_FLAG_START+190)	//�A�N�A�c(���[�h�P�P�X)
#define FV_HAGI_01_FIELD_R116			(FV_FLAG_START+191)	//�n�M(���[�h�P�P�U)
#define FV_TENKI_01_R119_R0101			(FV_FLAG_START+192)	//������(�P�K)
#define FV_TENKI_01_R119_R0102			(FV_FLAG_START+193)	//������(�Q�K)
#define FV_FIGHTER1_01_FIELD_R116		(FV_FLAG_START+194)	//�g���l���j(���[�h�P�P�U)
#define FV_ARTIST_02_C106_R0301			(FV_FLAG_START+195)	//�R���e�X�g�̊G�`��:�ʐM
#define FV_ODAMAKI_HYOUKA_T101_R0301	(FV_FLAG_START+196)	//�}�ӕ]���I�_�}�L(������)
#define FV_ODAMAKI_HYOUKA_FIELD_R101	(FV_FLAG_START+197)	//�}�ӕ]���I�_�}�L(���[�h�P�O�P)
#define FV_ODAMAKI_HYOUKA_FIELD_R103	(FV_FLAG_START+198)	//�}�ӕ]���I�_�}�L(���[�h�P�O�R)
#define FV_MIDDLEM1_01_R110_R0102		(FV_FLAG_START+199)	//�J���N���剤(���̊ԁj
#define FV_EVIL_ALL_FIELD_R110			(FV_FLAG_START+200)	//�A�N�A�c(���[�h�P�P�O)
#define FV_INTERVW1_05_CAMERA			(FV_FLAG_START+201)	//�C���^�r���A�[���J�����}��(r118)
#define FV_INTERVW1_06_CAMERA			(FV_FLAG_START+202)	//�C���^�r���A�[���J�����}��(r120)
#define FV_INTERVW1_07_CAMERA			(FV_FLAG_START+203)	//�C���^�r���A�[���J�����}��(r111)
#define FV_INTERVW1_08_CAMERA			(FV_FLAG_START+204)	//�C���^�r���A�[���J�����}��(r118)
#define FV_OBJ_ALL_C102_R0601			(FV_FLAG_START+205)	//�D�����OBJ
#define FV_TRAINERW1_01_FIELD_R104		(FV_FLAG_START+206)	//�g���[�i�[���i�T���E�g�E�J�̑O�j
#define FV_POKE_01_FIELD_T105			(FV_FLAG_START+207)	//�|�P����(�L��p)
#define FV_WORKERW2_01_C106_R0501		(FV_FLAG_START+208)	//�󂯕t��(�~�i���D����)
#define FV_SEAMAN1_01_C106_R0501		(FV_FLAG_START+209)	//�󂯕t��(�~�i���D����)
#define FV_DUMMY_01_PARK_KOKORO02		(FV_FLAG_START+210)	//�_�~�[(��̌Ǔ�)
#define FV_KOKORO_01_PARK_KOKORO02		(FV_FLAG_START+211)	//���e�B�A�X�E���e�B�I�X(��̌Ǔ�)
#define FV_TETTSUSEN_01_FIELD_C103		(FV_FLAG_START+212)	//�e�b�Z��(�L���Z�c)
#define FV_TETTSUSEN_01_C103_R0101		(FV_FLAG_START+213)	//�e�b�Z��(�W��)
#define FV_EVIL_ALL_FIELD_R121			(FV_FLAG_START+214)	//�A�N�A�c(r121)
#define FV_TR_ALL_CAVE_D0803			(FV_FLAG_START+215)	//�g���[�i�[(������юR)
#define FV_AQUABOSS_01_CAVE_D0808S		(FV_FLAG_START+216)	//�A�I�M��(������юR)
#define FV_AQUA_ALL_CAVE_D0808S			(FV_FLAG_START+217)	//�A�N�A�c(������юR)
#define FV_INTERVW1_01_PARK_BATTLE01	(FV_FLAG_START+218)	//�C���^�r���A�[(�o�g���^���[)
#define FV_SUPPORT_01_FIELD_R110		(FV_FLAG_START+219)	//�T�|�[�g(r110)
#define FV_SUPPORT_01_C109_R0105		(FV_FLAG_START+220)	//�T�|�[�g(�`�����s�I���̕���)
#define FV_ODAMAKI_01_C109_R0105		(FV_FLAG_START+221)	//�I�_�}�L(�`�����s�I���̕���)
#define FV_SUPPORT_02_FIELD_R110		(FV_FLAG_START+222)	//�T�|�[�g���]��(r110)
#define FV_SUPPORT_02_FIELD_R119		(FV_FLAG_START+223)	//�T�|�[�g���]��(r119)
#define FV_AQUA_ALL_CAVE_D0901			(FV_FLAG_START+224)	//�A�N�A�c(�A�W�g)
#define FV_GUEST_ALL_C106_R0102			(FV_FLAG_START+225)	//�~�i���̖��h�QF�̂��q�ꊇ�\��
#define FV_AQUA_ALL_CAVE_D0701			(FV_FLAG_START+226)	//�A�N�A�c(����ƂR)
#define FV_MAGMA_ALL_CAVE_D0701			(FV_FLAG_START+227)	//�}�O�}�c(����ƂR)
#define FV_SORAISI_01_T105_R0201		(FV_FLAG_START+228)	//�\���C�V���m(�\���C�V������)
#define FV_SUPPORT_01_FIELD_T104		(FV_FLAG_START+229)	//�T�|�[�g(T104)
#define FV_SUPPORT_02_FIELD_T104		(FV_FLAG_START+230)	//�T�|�[�g���]��(T104)
#define FV_IWA_01_CAVE_D0201			(FV_FLAG_START+231)	//��(�J�i�V�_�g���l������)
#define FV_IWA_02_CAVE_D0201			(FV_FLAG_START+232)	//��(�J�i�V�_�g���l������)
#define FV_POKE_01_C105_R0501			(FV_FLAG_START+233)	//���|�P����(�q���}�L)
#define FV_POKE_01_C107_R0301			(FV_FLAG_START+234)	//���|�P����(�g�N�T�l)
#define FV_POKE_01_CAVE_D0401			(FV_FLAG_START+235)	//�`���|�P����
#define FV_POKE_01_CAVE_D1801			(FV_FLAG_START+236)	//�`���|�P����
#define FV_POKE_01_CAVE_D1901			(FV_FLAG_START+237)	//�`���|�P����
#define FV_AQUA_ALL_CAVE_D0101			(FV_FLAG_START+238)	//�A�N�A�c(�����̓��A)
#define FV_MAGMA_ALL_CAVE_D0101			(FV_FLAG_START+239)	//�}�O�}�c(�����̓��A)
#define FV_FREAK1_01_T103_R0301			(FV_FLAG_START+240)	//�}�j�A(���s����)
#define FV_OWNER_01_PARK_BATTLE01		(FV_FLAG_START+241)	//�I�[�i�[(�o�g���^���[)
#define FV_SORAISI_01_CAVE_D0101		(FV_FLAG_START+242)	//�\���C�V���m(�����̑�)
#define FV_SUBMARINE1_01_CAVE_D0903		(FV_FLAG_START+243)	//������(�~�i���̃A�W�g)
#define FV_AQUABOSS_01_FIELD_R128		(FV_FLAG_START+244)	//�A�I�M��(r128)
#define FV_MAGMABOSS_01_FIELD_R128		(FV_FLAG_START+245)	//�}�c�u�T(r128)
#define FV_AQUA_ALL_CAVE_D1103			(FV_FLAG_START+246)	//�A�N�A�c(�C�ꓴ�A)
#define FV_WORKERM1_01_FIELD_R116		(FV_FLAG_START+247)	//������(r116)
#define FV_SHOPM1_04_FIELD_C102			(FV_FLAG_START+248)	//�X��(�J�C�i)
#define FV_WORKERM1_01_C104_R0103		(FV_FLAG_START+249)	//������(�f�{���{�ЂR�K)
#define FV_HAGI_01_SP_SHIP01			(FV_FLAG_START+250)	//�n�M�E�s�[�R(�A���D)
#define FV_FREAK1_01_SP_SHIP03			(FV_FLAG_START+251)	//�}�j�A(�A���D)
#define FV_SIO_01_ASASE					(FV_FLAG_START+252)	//�u�������̂����v
#define FV_SIO_02_ASASE					(FV_FLAG_START+253)	//�u�������̂����v
#define FV_SIO_03_ASASE					(FV_FLAG_START+254)	//�u�������̂����v
#define FV_SIO_04_ASASE					(FV_FLAG_START+255)	//�u�������̂����v
#define FV_KAI_01_ASASE					(FV_FLAG_START+256)	//�u�������̂�������v
#define FV_KAI_02_ASASE					(FV_FLAG_START+257)	//�u�������̂�������v
#define FV_KAI_03_ASASE					(FV_FLAG_START+258)	//�u�������̂�������v
#define FV_KAI_04_ASASE					(FV_FLAG_START+259)	//�u�������̂�������v
#define FV_BOY1_01_FIELD_R111			(FV_FLAG_START+260)	//�Ђ݂̂�����������l
#define FV_OBJ_ALL_C102_R0501			(FV_FLAG_START+261)	//�����كM�������[
#define FV_HORIDASI_01_C106_R1106		(FV_FLAG_START+262)	//�@��o������
#define FV_KASEKI_01_FIELD_R111			(FV_FLAG_START+263)	//�J�Z�L�i�˂����j
#define FV_KASEKI_02_FIELD_R111			(FV_FLAG_START+264)	//�J�Z�L�i�c���j
#define FV_EVILM1_06_C102_R0501			(FV_FLAG_START+265)	//�A�C�e���������A�N�A�c
#define FV_DAIGO_01_FIELD_R118			(FV_FLAG_START+266)	//�_�C�S(r118)
#define FV_DAIGO_01_C107_R0501			(FV_FLAG_START+267)	//�_�C�S(�g�N�T�l)
#define FV_BALL_01_C107_R0501			(FV_FLAG_START+268)	//�A�C�e���{�[��(�g�N�T�l)
#define FV_KAKURE_01_FIELD_C105			(FV_FLAG_START+269)	//�J�N���I��(�q���}�L)
#define FV_KAKURE_01_FIELD_R120			(FV_FLAG_START+270)	//�J�N���I��(�q���}�L)
#define FV_SUPPORT_01_FIELD_C106		(FV_FLAG_START+271)	//�T�|�[�g(�~�i��)
#define FV_DAIGO_01_FIELD_R120			(FV_FLAG_START+272)	//�_�C�S(�q���}�L)
#define FV_DAIGO_01_FIELD_C108			(FV_FLAG_START+273)	//�_�C�S(���l)
#define FV_BIRIRIDAMA_01				(FV_FLAG_START+274)	//�r�����_�}(�j���[�L���Z�c)
#define FV_BIRIRIDAMA_02				(FV_FLAG_START+275)	//�r�����_�}(�j���[�L���Z�c)
#define FV_BIRIRIDAMA_03				(FV_FLAG_START+276)	//�r�����_�}(�j���[�L���Z�c)
#define FV_MARUMAIN_01					(FV_FLAG_START+277)	//�}���}�C��(�A�W�g)
#define FV_MARUMAIN_02					(FV_FLAG_START+278)	//�}���}�C��(�A�W�g)
#define FV_SUPPORT_01_FIELD_T102		(FV_FLAG_START+279)	//�T�|�[�g(�R�g�L)
#define FV_SUBMARINE_06_CAVE_D1101		(FV_FLAG_START+280)	//������
#define FV_KAKUREONB_01_FIELD_R120		(FV_FLAG_START+281)	//�J�N���I���ʂ肱��(���[�h�P�Q�O)
#define FV_KAKURE_02_FIELD_R120			(FV_FLAG_START+282)	//�J�N���I��(���[�h�P�Q�O)
#define FV_WOMAN1_01_CAVE_D0201			(FV_FLAG_START+283)	//�~�`��(�J�i�V�_�g���l��)
#define FV_WOMAN1_01_T106_R0201			(FV_FLAG_START+284)	//�~�`��(�e�ʂ̉�)
#define FV_KAKURE_03_FIELD_R120			(FV_FLAG_START+285)	//�J�N���I��(���[�h�P�Q�O)
#define FV_KAKURE_04_FIELD_R120			(FV_FLAG_START+286)	//�J�N���I��(���[�h�P�Q�O)
#define FV_KAKURE_05_FIELD_R120			(FV_FLAG_START+287)	//�J�N���I��(���[�h�P�Q�O)
#define FV_KAKURE_06_FIELD_R120			(FV_FLAG_START+288)	//�J�N���I��(���[�h�P�Q�O)
#define FV_KAKURE_01_FIELD_R119			(FV_FLAG_START+289)	//�J�N���I��(���[�h�P�P�X)
#define FV_KAKURE_02_FIELD_R119			(FV_FLAG_START+290)	//�J�N���I��(���[�h�P�P�X)
#define FV_BOY2_01_FIELD_R101			(FV_FLAG_START+291)	//���N(���[�h�P�O�P)

//�t�B�[���h��̃����X�^�[�{�[���̃X�^�[�g��(FV_FLAG_START+300)�ɂȂ��Ă���
//1000 - 1199���m�ۂ���Ă���



//----------------------------------------------------------------------
//	�g���[�i�[�p
//
//	(FT_FLAG_START - TRFLG_START)
//	(    1200       -    1280      )
//----------------------------------------------------------------------
#define FT_FLAG_START					(1200)

//#define FT_GIRL4_01_FIELD_R103			(FT_FLAG_START+0)	//�I�_�}�L��(���[�h�P�O�R)

#define FT_GYM_01_LEADER				(FT_FLAG_START+1)	//�W��(�J�i�Y�~)
/*
#define FT_GYM_01_OBJ01					(FT_FLAG_START+2)	//�W��(�J�i�Y�~)
#define FT_GYM_01_OBJ02					(FT_FLAG_START+3)	//�W��
*/

#define FT_GYM_02_LEADER				(FT_FLAG_START+4)	//�W��(����)
/*
#define FT_GYM_02_OBJ01					(FT_FLAG_START+5)	//�W��
#define FT_GYM_02_OBJ02					(FT_FLAG_START+6)	//�W��
#define FT_GYM_02_OBJ03					(FT_FLAG_START+7)	//�W��
*/

#define FT_GYM_03_LEADER				(FT_FLAG_START+8)	//�W��(�L���Z�c)
/*
#define FT_GYM_03_OBJ01					(FT_FLAG_START+9)	//�W��
#define FT_GYM_03_OBJ02					(FT_FLAG_START+10)	//�W��
#define FT_GYM_03_OBJ03					(FT_FLAG_START+11)	//�W��
#define FT_GYM_03_OBJ04					(FT_FLAG_START+12)	//�W��
*/

#define FT_GYM_04_LEADER				(FT_FLAG_START+13)	//�W��(�t�G��)
/*
#define FT_GYM_04_OBJ01					(FT_FLAG_START+14)	//�W��
#define FT_GYM_04_OBJ02					(FT_FLAG_START+15)	//�W��
#define FT_GYM_04_OBJ03					(FT_FLAG_START+16)	//�W��
*/

#define FT_GYM_05_LEADER				(FT_FLAG_START+17)	//�W��(�g�E�J)
/*
#define FT_GYM_05_OBJ01					(FT_FLAG_START+18)	//�W��
#define FT_GYM_05_OBJ02					(FT_FLAG_START+19)	//�W��
#define FT_GYM_05_OBJ03					(FT_FLAG_START+20)	//�W��
#define FT_GYM_05_OBJ04					(FT_FLAG_START+21)	//�W��
#define FT_GYM_05_OBJ05					(FT_FLAG_START+22)	//�W��
#define FT_GYM_05_OBJ06					(FT_FLAG_START+23)	//�W��
#define FT_GYM_05_OBJ07					(FT_FLAG_START+24)	//�W��
*/

#define FT_GYM_06_LEADER				(FT_FLAG_START+25)	//�W��(�q���}�L)
/*
#define FT_GYM_06_OBJ01					(FT_FLAG_START+26)	//�W��
#define FT_GYM_06_OBJ02					(FT_FLAG_START+27)	//�W��
#define FT_GYM_06_OBJ03					(FT_FLAG_START+28)	//�W��
*/

#define FT_GYM_07_LEADER				(FT_FLAG_START+29)	//�W��(�g�N�T�l)
/*
#define FT_GYM_07_OBJ01					(FT_FLAG_START+30)	//�W��
#define FT_GYM_07_OBJ02					(FT_FLAG_START+31)	//�W��
#define FT_GYM_07_OBJ03					(FT_FLAG_START+32)	//�W��
#define FT_GYM_07_OBJ04					(FT_FLAG_START+33)	//�W��
#define FT_GYM_07_OBJ05					(FT_FLAG_START+34)	//�W��
#define FT_GYM_07_OBJ06					(FT_FLAG_START+35)	//�W��
*/

#define FT_GYM_08_LEADER				(FT_FLAG_START+36)	//�W��(���l)
/*
#define FT_GYM_08_OBJ01					(FT_FLAG_START+37)	//�W��
#define FT_GYM_08_OBJ02					(FT_FLAG_START+38)	//�W��
#define FT_GYM_08_OBJ03					(FT_FLAG_START+39)	//�W��
#define FT_GYM_08_OBJ04					(FT_FLAG_START+40)	//�W��
#define FT_GYM_08_OBJ05					(FT_FLAG_START+41)	//�W��
#define FT_GYM_08_OBJ06					(FT_FLAG_START+42)	//�W��
#define FT_GYM_08_OBJ07					(FT_FLAG_START+43)	//�W��
#define FT_GYM_08_OBJ08					(FT_FLAG_START+44)	//�W��
*/

#define FT_SHITEN_01					(FT_FLAG_START+45)	//�l�V��
#define FT_SHITEN_02					(FT_FLAG_START+46)	//�l�V��
#define FT_SHITEN_03					(FT_FLAG_START+47)	//�l�V��
#define FT_SHITEN_04					(FT_FLAG_START+48)	//�l�V��

#define FT_MITSURU_01_CAVE_D1301		(FT_FLAG_START+49)	//�~�c��(�`�����s�I�����[�h)

#endif  /* SAVEFLAG_H */
