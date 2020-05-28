/************************************************************************/
/*      evobjdef.h                                                      */
/*		̨���� OBJ *.c *.s ���ʎQ��                                     */
/************************************************************************/
#ifndef	EVOBJDEF_H
#define	EVOBJDEF_H

/*------------------------------------------*/
/*  ���� OBJ ID                             */
/*------------------------------------------*/
#define EV_OBJ_SPID             0xff            /* ���@ */

/*------------------------------------------*/
/*  ����                                    */
/*------------------------------------------*/
#define SITE_N                  0x00            /* �������� */
#define SITE_D                  0x01            /* ��       */
#define SITE_U                  0x02            /* ��       */
#define SITE_L                  0x03            /* ��       */
#define SITE_R                  0x04            /* �E       */
#define SITE_DL                 0x05            /* ����     */
#define SITE_DR                 0x06            /* ����     */
#define SITE_UL                 0x07            /* �E��     */
#define SITE_UR                 0x08            /* �E��     */

/*------------------------------------------*/
/*  ��Ұ��ݺ���޺���                        */
/*------------------------------------------*/
#define AC_SITE_D               0x00            /* 000 �������Ɍ����ύX         */
#define AC_SITE_U               0x01            /* 001 ������Ɍ����ύX         */
#define AC_SITE_L               0x02            /* 002 �������Ɍ����ύX         */
#define AC_SITE_R               0x03            /* 003 �E�����Ɍ����ύX         */
#define AC_WALK_D_05            0x04            /* 004 ���ֈړ�(0.5�ޯ�)        */
#define AC_WALK_U_05            0x05            /* 005 ��ֈړ�                 */
#define AC_WALK_L_05            0x06            /* 006 ���ֈړ�                 */
#define AC_WALK_R_05            0x07            /* 007 �E�ֈړ�                 */
#define AC_WALK_D_1             0x08            /* 008 ���ֈړ�(1�ޯ�)          */
#define AC_WALK_U_1             0x09            /* 009 ��ֈړ�                 */
#define AC_WALK_L_1             0x0a            /* 010 ���ֈړ�                 */
#define AC_WALK_R_1             0x0b            /* 011 �E�ֈړ�                 */
#define AC_JUMP_D_2             0x0c            /* 012 ���ּެ���(2��)          */
#define AC_JUMP_U_2             0x0d            /* 013 ��ּެ���               */
#define AC_JUMP_L_2             0x0e            /* 014 ���ּެ���               */
#define AC_JUMP_R_2             0x0f            /* 015 �E�ּެ���               */
#define AC_WAIT_1               0x10            /* 016 1���ĳ���                */
#define AC_WAIT_2               0x11            /* 017 2���ĳ���                */
#define AC_WAIT_4               0x12            /* 018 4���ĳ���                */
#define AC_WAIT_8               0x13            /* 019 8���ĳ���                */
#define AC_WAIT_16              0x14            /* 020 16���ĳ���               */
#define AC_WALK_D_2             0x15            /* 021 ���֑��s�ړ�(2�ޯ�)      */
#define AC_WALK_U_2             0x16            /* 022 ��֑��s�ړ�             */
#define AC_WALK_L_2             0x17            /* 023 ���֑��s�ړ�             */
#define AC_WALK_R_2             0x18            /* 024 �E�֑��s�ړ�             */
#define AC_STAY_WALK_D_05       0x19            /* 025 �����̏����(0.5�ޯ�)    */
#define AC_STAY_WALK_U_05       0x1a            /* 026 �セ�̏����             */
#define AC_STAY_WALK_L_05       0x1b            /* 027 �����̏����             */
#define AC_STAY_WALK_R_05       0x1c            /* 028 �E���̏����             */
#define AC_STAY_WALK_D_1        0x1d            /* 029 �����̏����(1�ޯ�)      */
#define AC_STAY_WALK_U_1        0x1e            /* 030 �セ�̏����             */
#define AC_STAY_WALK_L_1        0x1f            /* 031 �����̏����             */
#define AC_STAY_WALK_R_1        0x20            /* 032 �E���̏����             */
#define AC_STAY_WALK_D_2        0x21            /* 033 �����̏����(2�ޯ�)      */
#define AC_STAY_WALK_U_2        0x22            /* 034 �セ�̏����             */
#define AC_STAY_WALK_L_2        0x23            /* 035 �����̏����             */
#define AC_STAY_WALK_R_2        0x24            /* 036 �E���̏����             */
#define AC_STAY_WALK_D_4        0x25            /* 037 �����̏����(4�ޯ�)      */
#define AC_STAY_WALK_U_4        0x26            /* 038 �セ�̏����             */
#define AC_STAY_WALK_L_4        0x27            /* 039 �����̏����             */
#define AC_STAY_WALK_R_4        0x28            /* 040 �E���̏����             */
#define AC_WALK_D_3             0x29            /* 041 ���֑��s�ړ�(3�ޯ�)      */
#define AC_WALK_U_3             0x2a            /* 042 ��֑��s�ړ�             */
#define AC_WALK_L_3             0x2b            /* 043 ���֑��s�ړ�             */
#define AC_WALK_R_3             0x2c            /* 044 �E�֑��s�ړ�             */
#define AC_WALK_D_4             0x2d            /* 045 ���֑��s�ړ�(4�ޯ�)      */
#define AC_WALK_U_4             0x2e            /* 046 ��֑��s�ړ�             */
#define AC_WALK_L_4             0x2f            /* 047 ���֑��s�ړ�             */
#define AC_WALK_R_4             0x30            /* 048 �E�֑��s�ړ�             */
#define AC_WALK_D_8             0x31            /* 049 ���֑��s�ړ�(8�ޯ�)      */
#define AC_WALK_U_8             0x32            /* 050 ��֑��s�ړ�             */
#define AC_WALK_L_8             0x33            /* 051 ���֑��s�ړ�             */
#define AC_WALK_R_8             0x34            /* 052 �E�֑��s�ړ�             */
#define AC_DASH_D_2             0x35            /* 053 ���֑��s�ړ�(2�ޯ�,��p) */
#define AC_DASH_U_2             0x36            /* 054 ��֑��s�ړ�             */
#define AC_DASH_L_2             0x37            /* 055 ���֑��s�ړ�             */
#define AC_DASH_R_2             0x38            /* 056 �E�֑��s�ړ�             */
#define AC_ANM_END_WAIT         0x39            /* 057 �ٱ�ҏI���҂�            */
#define AC_SWIM_JUMP_D          0x3a            /* 058 ���g���ެ���(��p)     */
#define AC_SWIM_JUMP_U          0x3b            /* 059 ��g���ެ���(��p)     */
#define AC_SWIM_JUMP_L          0x3c            /* 060 ���g���ެ���(��p)     */
#define AC_SWIM_JUMP_R          0x3d            /* 061 �E�g���ެ���(��p)     */
#define AC_TURN_HERO_SITE       0x3e            /* 062 ��l���ʒu�U�����       */
#define AC_TURN_REV_HERO_SITE   0x3f            /* 063 ��l���ʒu�t�����U����� */
#define AC_SITE_PAUSE_ON        0x40            /* 064 �����Œ�                 */
#define AC_SITE_PAUSE_OFF       0x41            /* 065 �����Œ����             */
#define AC_JUMP_D_1             0x42            /* 066 ���ּެ���(1��)          */
#define AC_JUMP_U_1             0x43            /* 067 ��ּެ���               */
#define AC_JUMP_L_1             0x44            /* 068 ���ּެ���               */
#define AC_JUMP_R_1             0x45            /* 069 �E�ּެ���               */
#define AC_STAY_JUMP_D          0x46            /* 070 �����̏�ެ���           */
#define AC_STAY_JUMP_U          0x47            /* 071 �セ�̏�ެ���           */
#define AC_STAY_JUMP_L          0x48            /* 072 �����̏�ެ���           */
#define AC_STAY_JUMP_R          0x49            /* 073 �E���̏�ެ���           */
#define AC_STAY_TURN_JUMP_D     0x4a            /* 074 �����̏�ެ��ߐU��Ԃ�   */
#define AC_STAY_TURN_JUMP_U     0x4b            /* 075 �セ�̏�ެ��ߐU��Ԃ�   */
#define AC_STAY_TURN_JUMP_L     0x4c            /* 076 �����̏�ެ��ߐU��Ԃ�   */
#define AC_STAY_TURN_JUMP_R     0x4d            /* 077 �E���̏�ެ��ߐU��Ԃ�   */
#define AC_DEFAULT_SITE         0x4e            /* 078 ��{�����ɕύX           */
#define AC_PC_BOW               0x4f            /* 079 PC�����V(��p)           */
#define AC_UNDER_EFFECT_ON      0x50            /* 080 �����̪�ı�              */
#define AC_UNDER_EFFECT_OFF     0x51            /* 081 �����̪�Ė���            */
#define AC_ANM_CHG_HOLD_ON      0x52            /* 082 ��Ұ��݌Œ艻            */
#define AC_ANM_CHG_HOLD_OFF     0x53            /* 083 ��Ұ��݌Œ����          */
#define AC_VANISH_ON            0x54            /* 084 OBJ ��\��               */
#define AC_VANISH_OFF           0x55            /* 085 OBJ �\��                 */
#define AC_MARK_GYOE            0x56            /* 086 ����� ! ϰ�             */
#define AC_MARK_HATENA          0x57            /* 087 ����� ? ϰ�             */
#define AC_MARK_HEART           0x58            /* 088 ����� ʰ� ϰ�           */
#define AC_HIDE_PULLOFF         0x59            /* 089 �B�ꖪ�߂���             */
#define AC_ROCK_CRUSH           0x5a            /* 090 ��ӂ�(��p)             */
#define AC_TREE_CUT             0x5b            /* 091 �����؂�(��p)           */
#define AC_PRI_HOLD_SW_ON       0x5c            /* 092 �\����ײ��è�Œ�         */
#define AC_PRI_HOLD_SW_OFF      0x5d            /* 093 �\����ײ��è�Œ����     */
#define AC_AFFUSE_ON            0x5e            /* 094 �g�k�g�p ON              */
#define AC_AFFUSE_OFF           0x5f            /* 095 �g�k�g�p OFF             */
#define AC_KIOG_WALK_S_D_05     0x60            /* 096 �������ވړ��J�n(��p)   */
#define AC_KIOG_WALK_D_05       0x61            /* 097 �������ވړ�(��p)       */
#define AC_MTB_FTUP_SITE_D      0x62            /* 098 ��������Ա��ߒ�~(��p)  */
#define AC_MTB_FTUP_SITE_U      0x63            /* 099 ��������Ա��ߒ�~(��p)  */
#define AC_MTB_FTUP_SITE_L      0x64            /* 100 ��������Ա��ߒ�~(��p)  */
#define AC_MTB_FTUP_SITE_R      0x65            /* 101 �E������Ա��ߒ�~(��p)  */
#define AC_MTB_FTUP_D           0x66            /* 102 ��������Ա���(��p)      */
#define AC_MTB_FTUP_U           0x67            /* 103 ��������Ա���(��p)      */
#define AC_MTB_FTUP_L           0x68            /* 104 ��������Ա���(��p)      */
#define AC_MTB_FTUP_R           0x69            /* 105 �E������Ա���(��p)      */
#define AC_MTB_FTDOWN_D         0x6a            /* 106 ����������޳�(��p)      */
#define AC_MTB_FTDOWN_U         0x6b            /* 107 ����������޳�(��p)      */
#define AC_MTB_FTDOWN_L         0x6c            /* 108 ����������޳�(��p)      */
#define AC_MTB_FTDOWN_R         0x6d            /* 109 �E��������޳�(��p)      */
#define AC_MTB_DANIEL_D         0x6e            /* 110 ����ƴ�(��p)            */
#define AC_MTB_DANIEL_U         0x6f            /* 111 ����ƴ�(��p)            */
#define AC_MTB_DANIEL_L         0x70            /* 112 ����ƴ�(��p)            */
#define AC_MTB_DANIEL_R         0x71            /* 113 �E��ƴ�(��p)            */
#define AC_MTB_DANIEL_D_1       0x72            /* 114 ����ƴوړ�(1��,��p)    */
#define AC_MTB_DANIEL_U_1       0x73            /* 115 ����ƴوړ�(1��,��p)    */
#define AC_MTB_DANIEL_L_1       0x74            /* 116 ����ƴوړ�(1��,��p)    */
#define AC_MTB_DANIEL_R_1       0x75            /* 117 �E��ƴوړ�(1��,��p)    */
#define AC_MTB_DANIEL_D_2       0x76            /* 118 ����ƴوړ�(2��,��p)    */
#define AC_MTB_DANIEL_U_2       0x77            /* 119 ����ƴوړ�(2��,��p)    */
#define AC_MTB_DANIEL_L_2       0x78            /* 120 ����ƴوړ�(2��,��p)    */
#define AC_MTB_DANIEL_R_2       0x79            /* 121 �E��ƴوړ�(2��,��p)    */
#define AC_MTB_WHEELIE_D        0x7a            /* 122 �����̏곲ذ(��p)       */
#define AC_MTB_WHEELIE_U        0x7b            /* 123 �セ�̏곲ذ(��p)       */
#define AC_MTB_WHEELIE_L        0x7c            /* 124 �����̏곲ذ(��p)       */
#define AC_MTB_WHEELIE_R        0x7d            /* 125 �E���̏곲ذ(��p)       */
#define AC_MTB_WHEELIE_S_D_2    0x7e            /* 126 ����ذ�J�n(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_S_U_2    0x7f            /* 127 �㳲ذ�J�n(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_S_L_2    0x80            /* 128 ����ذ�J�n(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_S_R_2    0x81            /* 129 �E��ذ�J�n(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_D_2      0x82            /* 130 ����ذ�ړ�(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_U_2      0x83            /* 131 �㳲ذ�ړ�(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_L_2      0x84            /* 132 ����ذ�ړ�(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_R_2      0x85            /* 133 �E��ذ�ړ�(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_E_D_2    0x86            /* 134 ����ذ�I��(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_E_U_2    0x87            /* 135 �㳲ذ�I��(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_E_L_2    0x88            /* 136 ����ذ�I��(2�ޯ�,��p)   */
#define AC_MTB_WHEELIE_E_R_2    0x89            /* 137 �E��ذ�I��(2�ޯ�,��p)   */

#define ACMD_END                0xfe            /* 254 ����ޏI��                */
#define NOT_ACMD                0xff            /* 255 ���Ұ��ݺ���            */

#define AC_STOP_WALK_D AC_STAY_WALK_D_1         /* ������                       */
#define AC_STOP_WALK_U AC_STAY_WALK_U_1         /* ������                       */
#define AC_STOP_WALK_L AC_STAY_WALK_L_1         /* ������                       */
#define AC_STOP_WALK_R AC_STAY_WALK_R_1         /* ������                       */
#define AC_WALK_TURN_D AC_STAY_WALK_D_4         /* ������                       */
#define AC_WALK_TURN_U AC_STAY_WALK_U_4         /* ������                       */
#define AC_WALK_TURN_L AC_STAY_WALK_L_4         /* ������                       */
#define AC_WALK_TURN_R AC_STAY_WALK_R_4         /* ������                       */

/*------------------------------------------*/
/*  OBJ ��׸������                          */
/*------------------------------------------*/
#define MAINHERO                0x00            /* 000 ��l��                   */
#define MAINHEROCYCLER          0x01            /* 001 ���]�Ԏ�l�� ڰ�����     */
#define MAINHEROSWIM            0x02            /* 002 �g����l��             */
#define MAINHEROHIDEN           0x03            /* 003 ��`�Z��l��             */
#define BIGMAN0                 0x04            /* 004 ��j                     */
#define BOYA1                   0x05            /* 005 �V��                     */
#define OJYOU1                  0x06            /* 006 ����                     */
#define BOY1                    0x07            /* 007 ���N                     */
#define GIRL1                   0x08            /* 008 ����                     */
#define BOY2                    0x09            /* 009 ���N                     */
#define GIRL2                   0x0a            /* 010 ����                     */
#define BOYA2                   0x0b            /* 011 �V��                     */
#define OJYOU2                  0x0c            /* 012 ����                     */
#define BOY3                    0x0d            /* 013 ���N                     */
#define GIRL3                   0x0e            /* 014 ����                     */
#define BOY4                    0x0f            /* 015 ���N                     */
#define GIRL4                   0x10            /* 016 ����                     */
#define BIGMAN1                 0x11            /* 017 ��j                     */
#define MIDDLEW1                0x12            /* 018 ���΂���                 */
#define MIDDLEM1                0x13            /* 019 ��������                 */
#define WOMAN1                  0x14            /* 020 ����                     */
#define OLDMAN1                 0x15            /* 021 �V�l                     */
#define OLDWOMAN1               0x16            /* 022 �V�k                     */
#define WORKERM1                0x17            /* 023 ��ذ��                   */
#define WORKERW1                0x18            /* 024 ��ذ����                 */
#define MIDDLEM2                0x19            /* 025 ��������                 */
#define WOMAN2                  0x1a            /* 026 ����                     */
#define COOK1                   0x1b            /* 027 �����l                   */
#define WAITRESS1               0x1c            /* 028 ����ڽ                   */
#define OLDMAN2                 0x1d            /* 029 �V�l                     */
#define OLDWOMAN2               0x1e            /* 030 �V�k                     */
#define BOYSCOUT1               0x1f            /* 031 �ް�����                 */
#define GIRLSCOUT1              0x20            /* 032 �ްٽ���                 */
#define TRAINERM1               0x21            /* 033 �ڰŰ�j                  */
#define TRAINERW1               0x22            /* 034 �ڰŰ��                  */
#define CAPBOY1                 0x23            /* 035 �X�q���N                 */
#define CAPBOY2                 0x24            /* 036 �X�q���N                 */
#define BOY5                    0x25            /* 037 ���N                     */
#define SCHOOLM1                0x26            /* 038 �j�q�w��                 */
#define FREAK1                  0x27            /* 039 �Ʊ                      */
#define OCCULT1                 0x28            /* 040 �����Ʊ                  */
#define EVILW1                  0x29            /* 041 ���ҏ�                   */
#define SWIMERM1                0x2a            /* 042 �C��ݖ�Y                */
#define SWIMERW1                0x2b            /* 043 �C��ݏ��Y                */
#define FIGHTER1                0x2c            /* 044 �i����                   */
#define WORKERW2                0x2d            /* 045 ��ذ����                 */
#define STUDYM1                 0x2e            /* 046 ������                   */
#define SCHOOLW1                0x2f            /* 047 ���q�w��                 */
#define GENTLEM1                0x30            /* 048 �V�a�m                   */
#define SEAMAN1                 0x31            /* 049 �D���                   */
#define FISHING1                0x32            /* 050 �ނ�l                   */
#define SPORTSM1                0x33            /* 051 ��߰���                  */
#define SPORTSW1                0x34            /* 052 ��߰³���                */
#define BEACHGIRL1              0x35            /* 053 �����ւ���               */
#define BEACHBOY1               0x36            /* 054 �����֖V��               */
#define HILL1                   0x37            /* 055 �R�j                     */
#define CYCLEM1                 0x38            /* 056 ���]�Ԓj                 */
#define CYCLEW1                 0x39            /* 057 ���]�ԏ�                 */
#define PCWOMAN1                0x3a            /* 058 �߹�ݎo����              */
#define CAPSULE1                0x3b            /* 059 ����1                    */
#define SEED                    0x3c            /* 060 ��                       */
#define SEED16x16               0x3d            /* 061 ��16x16                  */
#define SEED16x32               0x3e            /* 062 ��16x32                  */
#define MAINHEROCYCLES          0x3f            /* 063 ���]�Ԏ�l�� ��߰�����   */
#define ODAMAKI                 0x40            /* 064 ���Ϸ                    */
#define YOUNGM1                 0x41            /* 065 �N�j��                 */
#define YOUNGM2                 0x42            /* 066 �N�j��                 */
#define INTERVM1                0x43            /* 067 ����ޭ��j                */
#define INTERVW1                0x44            /* 068 ����ޭ���                */
#define OYAJI1                  0x45            /* 069 ���₶1                  */
#define OYAJI2                  0x46            /* 070 ���₶2                  */
#define OYAJI3                  0x47            /* 071 ���₶3                  */
#define OYAJI4                  0x48            /* 072 ���₶4                  */
#define OYAJI5                  0x49            /* 073 ���₶5                  */
#define OYAJI6                  0x4a            /* 074 ���₶6                  */
#define OYAJI7                  0x4b            /* 075 ���₶7                  */
#define GOODS1                  0x4c            /* 076 �ޯ��1 Ȳè              */
#define GOODS2                  0x4d            /* 077 �ޯ��2 ���               */
#define GOODS3                  0x4e            /* 078 �ޯ��3 ��ƶ��            */
#define GOODS4                  0x4f            /* 079 �ޯ��4 ��߰              */
#define GOODS5                  0x50            /* 080 �ޯ��5 �߶���            */
#define GOODS6                  0x51            /* 081 �ޯ��6 ��غ��            */
#define TREE1                   0x52            /* 082 ��                       */
#define SHOPM1                  0x53            /* 083 �X���j                   */
#define SHOPW1                  0x54            /* 084 �X����                   */
#define PCWOMAN2                0x55            /* 085 �߹�ݎo����              */
#define CRUSHROCK1              0x56            /* 086 �ӂ����                 */
#define PUSHROCK1               0x57            /* 087 �������                 */
#define SHIP1                   0x58            /* 088 �D                       */
#define MAINHEROINE             0x59            /* 089 ����l��                 */
#define MAINHINECYCLER          0x5a            /* 090 ����l�����]�� ڰ�����   */
#define MAINHINECYCLES          0x5b            /* 091 ����l�����]�� ��߰����� */
#define MAINHINESWIM            0x5c            /* 092 ����l���g���           */
#define MAINHINEHIDEN           0x5d            /* 093 ����l����`�Z           */
#define TRUCK1                  0x5e            /* 094 �ׯ�                     */
#define MOVER1                  0x5f            /* 095 ���z���߹��              */
#define MOVER2                  0x60            /* 096 ���z���߹��              */
#define RUCK1                   0x61            /* 097 ح�����                  */
#define ATTACK1                 0x62            /* 098 �P���߹��                */
#define ARTIST1                 0x63            /* 099 �G�`������               */
#define HERO                    0x64            /* 100 �j��l��                 */
#define HEROCYCLER              0x65            /* 101 ���]�Ԏ�l�� ڰ�����     */
#define HEROCYCLES              0x66            /* 102 ���]�Ԏ�l�� ڰ�����     */
#define HEROSWIM                0x67            /* 103 �g����l��             */
#define HEROHIDEN               0x68            /* 104 ��`�Z��l��             */
#define HEROINE                 0x69            /* 105 ����l��                 */
#define HINECYCLER              0x6a            /* 106 ����l�����]�� ڰ�����   */
#define HINECYCLES              0x6b            /* 107 ����l�����]�� ��߰����� */
#define HINESWIM                0x6c            /* 108 ����l���g���           */
#define HINEHIDEN               0x6d            /* 109 ����l����`�Z           */
#define CAMERA1                 0x6e            /* 110 �����                    */
#define MAINHERODIV             0x6f            /* 111 �j��l���޲��ݸ�         */
#define MAINHINEDIV             0x70            /* 112 ����l���޲��ݸ�         */
#define CARGO1                  0x71            /* 113 �ו�                     */
#define ROPEWAY1                0x72            /* 114 ۰�߳��                  */
#define STUDYM2                 0x73            /* 115 ������                   */
#define DEVON1                  0x74            /* 116 �����                    */
#define AQUALM1                 0x75            /* 117 ����c                    */
#define AQUALF1                 0x76            /* 118 ����c                    */
#define MAGMAM1                 0x77            /* 119 ϸ�ϒc                   */
#define MAGMAF1                 0x78            /* 120 ϸ�ϒc                   */
#define KAGETSU                 0x79            /* 121 �J�Q�c                   */
#define FUYOU                   0x7a            /* 122 �t���E                   */
#define PURIMU                  0x7b            /* 123 �v����                   */
#define GENJI                   0x7c            /* 124 �Q���W                   */
#define TSUTSUJI                0x7d            /* 125 �c�c�W                   */
#define TOUKI                   0x7e            /* 126 �g�E�L                   */
#define TETTSUSEN               0x7f            /* 127 �e�b�Z��                 */
#define AIRI                    0x80            /* 128 �A�C��                   */
#define SENRI                   0x81            /* 129 �Z����                   */
#define NAGI                    0x82            /* 130 �i�M                     */
#define FUU                     0x83            /* 131 �t�E                     */
#define RUN                     0x84            /* 132 ����                     */
#define MIKURI                  0x85            /* 133 �~�N��                   */
#define DAIGO                   0x86            /* 134 �_�C�S                   */
#define MITSURU                 0x87            /* 135 �~�c��                   */
#define SUPPORTM1               0x88            /* 136 �T�|�[�g                 */
#define MAINHEROFISH            0x89            /* 137 ��l���ނ�               */
#define MAINHINEFISH            0x8a            /* 138 ��l���ނ�               */
#define ONSENW1                 0x8b            /* 139 ����V�k                 */
#define HISHIP1                 0x8c            /* 140 ������                   */
#define SUBMARINE1              0x8d            /* 141 ������                   */
#define DOLL1                   0x8e            /* 142 �ʂ������               */
#define DOLL2                   0x8f            /* 143 �ʂ������               */
#define DOLL3                   0x90            /* 144 �ʂ������               */
#define DOLL4                   0x91            /* 145 �ʂ������               */
#define DOLL5                   0x92            /* 146 �ʂ������               */
#define DOLL6                   0x93            /* 147 �ʂ������               */
#define DOLL7                   0x94            /* 148 �ʂ������               */
#define DOLL8                   0x95            /* 149 �ʂ������               */
#define DOLL9                   0x96            /* 150 �ʂ������               */
#define DOLL10                  0x97            /* 151 �ʂ������               */
#define DOLL11                  0x98            /* 152 �ʂ������               */
#define DOLL12                  0x99            /* 153 �ʂ������               */
#define DOLL13                  0x9a            /* 154 �ʂ������               */
#define DOLL14                  0x9b            /* 155 �ʂ������               */
#define DOLL15                  0x9c            /* 156 �ʂ������               */
#define DOLL16                  0x9d            /* 157 �ʂ������               */
#define DOLL17                  0x9e            /* 158 �ʂ������               */
#define DOLL18                  0x9f            /* 159 �ʂ������               */
#define DOLL19                  0xa0            /* 160 �ʂ������               */
#define DOLL20                  0xa1            /* 161 �ʂ������               */
#define DOLL21                  0xa2            /* 162 �ʂ������               */
#define DOLL22                  0xa3            /* 163 �ʂ������               */
#define DOLL23                  0xa4            /* 164 �ʂ������               */
#define DOLL24                  0xa5            /* 165 �ʂ������               */
#define DOLL25                  0xa6            /* 166 �ʂ������               */
#define CUSHION1                0xa7            /* 167 �����                    */
#define CUSHION2                0xa8            /* 168 �����                    */
#define CUSHION3                0xa9            /* 169 �����                    */
#define CUSHION4                0xaa            /* 170 �����                    */
#define CUSHION5                0xab            /* 171 �����                    */
#define CUSHION6                0xac            /* 172 �����                    */
#define CUSHION7                0xad            /* 173 �����                    */
#define CUSHION8                0xae            /* 174 �����                    */
#define CUSHION9                0xaf            /* 175 �����                    */
#define CUSHION10               0xb0            /* 176 �����                    */
#define BIGDOLL1                0xb1            /* 177 �傫�Ȃʂ������         */
#define BIGDOLL2                0xb2            /* 178 �傫�Ȃʂ������         */
#define BIGDOLL3                0xb3            /* 179 �傫�Ȃʂ������         */
#define BIGDOLL4                0xb4            /* 180 �傫�Ȃʂ������         */
#define BIGDOLL5                0xb5            /* 181 �傫�Ȃʂ������         */
#define BIGDOLL6                0xb6            /* 182 �傫�Ȃʂ������         */
#define BIGDOLL7                0xb7            /* 183 �傫�Ȃʂ������         */
#define BIGDOLL8                0xb8            /* 184 �傫�Ȃʂ������         */
#define BIGDOLL9                0xb9            /* 185 �傫�Ȃʂ������         */
#define BIGDOLL10               0xba            /* 186 �傫�Ȃʂ������         */
#define RATIASU                 0xbb            /* 187 �è��                    */
#define RATIOSU                 0xbc            /* 188 �è��                    */
#define AGBOY1                  0xbd            /* 189 AGB���N                  */
#define JUDGEM1                 0xbe            /* 190 �R����                   */
#define MAINHEROJORRO           0xbf            /* 191 ��l���@�J�I             */
#define MAINHINEJORRO           0xc0            /* 192 ��l���@�J�I             */
#define HEROREMODEL             0xc1            /* 193 ��l���͗l�ւ�           */
#define HINEREMODEL             0xc2            /* 194 ����l���͗l�ւ�         */
#define AQUABOSS                0xc3            /* 195 ����c�޽                 */
#define MAGMABOSS               0xc4            /* 196 ϸ�ϒc�޽                */
#define KAIOGA                  0xc5            /* 197 ������                   */
#define GURADON                 0xc6            /* 198 ��װ���                  */
#define FOSSIL1                 0xc7            /* 199 ����                     */
#define LEGEND1                 0xc8            /* 200 �`��                     */
#define LEGEND2                 0xc9            /* 201 �`��                     */
#define LEGEND3                 0xca            /* 202 �`��                     */
#define ENECO                   0xcb            /* 203 �G�l�R                   */
#define KAKUREON                0xcc            /* 204 ��ڵ�                    */
#define KAIOGAMC                0xcd            /* 205 �������ɸ�               */
#define GURADONMC               0xce            /* 206 ��װ����ɸ�              */
#define TYCOON                  0xcf            /* 207 �^�C�N�[��               */
#define JIGUMA                  0xd0            /* 208 �W�O�U�O�}               */
#define PIKACHUU                0xd1            /* 209 �s�J�`���E               */
#define MARIRURI                0xd2            /* 210 �}������                 */
#define CAMOME                  0xd3            /* 211 �L������                 */
#define KAKUREONB               0xd4            /* 212 ��ڵ݋�                  */
#define BEACHBOY2               0xd5            /* 213 �����֖V��2              */
#define RURIRI                  0xd6            /* 214 ������                   */
#define MYMAM                   0xd7            /* 215 �}�}                     */
#define CTHERO                  0xd8            /* 216 �j��l����ýėp          */
#define CTHINE                  0xd9            /* 217 ����l����ýėp          */
#define EVOBJCODEMAX            0xda            /* 218 OBJ���ލő�              */

#define WKOBJCODE1              0xf0            /* 240 ܰ�1�Q��OBJ����          */
#define WKOBJCODE2              0xf1            /* 241 ܰ�2�Q��OBJ����          */
#define WKOBJCODE3              0xf2            /* 242 ܰ�3�Q��OBJ����          */
#define WKOBJCODE4              0xf3            /* 243 ܰ�4�Q��OBJ����          */
#define WKOBJCODE5              0xf4            /* 244 ܰ�5�Q��OBJ����          */
#define WKOBJCODE6              0xf5            /* 245 ܰ�6�Q��OBJ����          */
#define WKOBJCODE7              0xf6            /* 246 ܰ�7�Q��OBJ����          */
#define WKOBJCODE8              0xf7            /* 247 ܰ�8�Q��OBJ����          */
#define WKOBJCODE9              0xf8            /* 248 ܰ�9�Q��OBJ����          */
#define WKOBJCODE10             0xf9            /* 249 ܰ�10�Q��OBJ����         */
#define WKOBJCODE11             0xfa            /* 250 ܰ�11�Q��OBJ����         */
#define WKOBJCODE12             0xfb            /* 251 ܰ�12�Q��OBJ����         */
#define WKOBJCODE13             0xfc            /* 252 ܰ�13�Q��OBJ����         */
#define WKOBJCODE14             0xfd            /* 253 ܰ�14�Q��OBJ����         */
#define WKOBJCODE15             0xfe            /* 254 ܰ�15�Q��OBJ����         */
#define WKOBJCODE16             0xff            /* 255 ܰ�16�Q��OBJ����         */

#define EVILM1 (OCCULT1) /* �u������ */

/*------------------------------------------*/
/*  OBJ ���캰��                            */
/*------------------------------------------*/
#define MV_DMY                  0x00            /* 000 �ړ�����                 */
#define MV_OFF                  0x01            /* 001 ����� ����               */
#define MV_ON                   0x02            /* 002 ����� �ړ�               */
#define MV_VON                  0x03            /* 003 ����� ����               */
#define MV_VOND                 0x04            /* 004 ����� ��������������     */
#define MV_HON                  0x05            /* 005 ����� ����               */
#define MV_HONR                 0x06            /* 006 ����� �������������E     */
#define MV_UP                   0x07            /* 007 ��Œ�                   */
#define MV_DOWN                 0x08            /* 008 ���Œ�                   */
#define MV_LEFT                 0x09            /* 009 ���Œ�                   */
#define MV_RIGHT                0x0a            /* 010 �E�Œ�                   */
#define MV_HERO                 0x0b            /* 011 ���@��p                 */
#define MV_SEED                 0x0c            /* 012 ���ː�p                 */
#define MV_V                    0x0d            /* 013 ����� ��������           */
#define MV_H                    0x0e            /* 014 ����� ��������           */
#define MV_UL                   0x0f            /* 015 ����я㍶����            */
#define MV_UR                   0x10            /* 016 ����я�E����            */
#define MV_DL                   0x11            /* 017 ����щ�������            */
#define MV_DR                   0x12            /* 018 ����щ��E����            */
#define MV_LV                   0x13            /* 019 ����я㉺������          */
#define MV_RV                   0x14            /* 020 ����я㉺�E����          */
#define MV_UH                   0x15            /* 021 ����э��E�����          */
#define MV_DH                   0x16            /* 022 ����э��E������          */
#define MV_SPINL                0x17            /* 023 ������]��               */
#define MV_SPINR                0x18            /* 024 ������]�E               */
#define MV_RTU                  0x19            /* 025 ٰ�2 ��                  */
#define MV_RTD                  0x1a            /* 026 ٰ�2 ��                  */
#define MV_RTL                  0x1b            /* 027 ٰ�2 ��                  */
#define MV_RTR                  0x1c            /* 028 ٰ�2 �E                  */
#define MV_RTURLD               0x1d            /* 029 ٰ�3 ��E����            */
#define MV_RTRLDU               0x1e            /* 030 ٰ�3 �E������            */
#define MV_RTDURL               0x1f            /* 031 ٰ�3 ����E��            */
#define MV_RTLDUR               0x20            /* 032 ٰ�3 ������E            */
#define MV_RTULRD               0x21            /* 033 ٰ�3 �㍶�E��            */
#define MV_RTLRDU               0x22            /* 034 ٰ�3 ���E����            */
#define MV_RTDULR               0x23            /* 035 ٰ�3 ���㍶�E            */
#define MV_RTRDUL               0x24            /* 036 ٰ�3 �E���㍶            */
#define MV_RTLUDR               0x25            /* 037 ٰ�3 ���㉺�E            */
#define MV_RTUDRL               0x26            /* 038 ٰ�3 �㉺�E��            */
#define MV_RTRLUD               0x27            /* 039 ٰ�3 �E���㉺            */
#define MV_RTDRLU               0x28            /* 040 ٰ�3 ���E����            */
#define MV_RTRUDL               0x29            /* 041 ٰ�3 �E�㉺��            */
#define MV_RTUDLR               0x2a            /* 042 ٰ�3 �㉺���E            */
#define MV_RTLRUD               0x2b            /* 043 ٰ�3 ���E�㉺            */
#define MV_RTDLRU               0x2c            /* 044 ٰ�3 �����E��            */
#define MV_RTUL                 0x2d            /* 045 ٰ�4 �㍶                */
#define MV_RTDR                 0x2e            /* 046 ٰ�4 ���E                */
#define MV_RTLD                 0x2f            /* 047 ٰ�4 ����                */
#define MV_RTRU                 0x30            /* 048 ٰ�4 �E��                */
#define MV_RTUR                 0x31            /* 049 ٰ�4 ��E                */
#define MV_RTDL                 0x32            /* 050 ٰ�4 ����                */
#define MV_RTLU                 0x33            /* 051 ٰ�4 ����                */
#define MV_RTRD                 0x34            /* 052 ٰ�4 �E��                */
#define MV_COPYU                0x35            /* 053 ���̂܂ˏ�               */
#define MV_COPYD                0x36            /* 054 ���̂܂ˉ�               */
#define MV_COPYL                0x37            /* 055 ���̂܂ˉE               */
#define MV_COPYR                0x38            /* 056 ���̂܂ˍ�               */
#define MV_HIDEW                0x39            /* 057 �B�ꖪ��                 */
#define MV_HIDER                0x3a            /* 058 �B�ꖪ��                 */
#define MV_COPYGRASSU           0x3b            /* 059 ���̂܂ˑ���             */
#define MV_COPYGRASSD           0x3c            /* 060 ���̂܂ˑ���             */
#define MV_COPYGRASSL           0x3d            /* 061 ���̂܂ˑ���             */
#define MV_COPYGRASSR           0x3e            /* 062 ���̂܂ˑ��E             */
#define MV_HIDES                0x3f            /* 063 �B�ꖪ��                 */
#define MV_STAYWALK1D           0x40            /* 064 ���̏������1�ޯ�        */
#define MV_STAYWALK1U           0x41            /* 065 ���̏������1�ޯ�        */
#define MV_STAYWALK1L           0x42            /* 066 ���̏������1�ޯ�        */
#define MV_STAYWALK1R           0x43            /* 067 ���̏�����E1�ޯ�        */
#define MV_STAYWALK2D           0x44            /* 068 ���̏������2�ޯ�        */
#define MV_STAYWALK2U           0x45            /* 069 ���̏������2�ޯ�        */
#define MV_STAYWALK2L           0x46            /* 070 ���̏������2�ޯ�        */
#define MV_STAYWALK2R           0x47            /* 071 ���̏�����E2�ޯ�        */
#define MV_STAYWALK4D           0x48            /* 072 ���̏������4�ޯ�        */
#define MV_STAYWALK4U           0x49            /* 073 ���̏������4�ޯ�        */
#define MV_STAYWALK4L           0x4a            /* 074 ���̏������4�ޯ�        */
#define MV_STAYWALK4R           0x4b            /* 075 ���̏�����E4�ޯ�        */
#define MV_VANISH               0x4c            /* 076 ����                     */

#define	MV_COPY (MV_COPYU)           /* �� */
#define	MV_COPYGRASS (MV_COPYGRASSU) /* �� */

#endif		/* EVOBJDEF_H */