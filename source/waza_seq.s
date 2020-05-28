@======================================================
@		waza_seq.s
@======================================================

	.text
	.code 32

	.include	"waza_seq.def"
@	.include	"fightmsg.def"

@======================================================
@	define�錾
@======================================================

MOJI_WAIT	=	0x040

@======================================================
@	global�錾
@======================================================

	.global		WazaEffectTbl

@======================================================
@	����Z�V�[�P���X�@global�錾
@======================================================

	.global		WAZA_KizetsuA
	.global		WAZA_KizetsuD
	.global		WAZA_Win
	.global		WAZA_Lose
	.global		WAZA_TrainerWin
	.global		WAZA_TuushinWin
	.global		WAZA_BattleTowerWin
	.global		WAZA_ItemEscape
	.global		WAZA_SpeabiEscape
	.global		WAZA_Escape
	.global		WAZA_EnemyEscape
	.global		WAZA_TrainerEscape
	.global		WAZA_EscapeNG
	.global		WAZA_EscapeNG2
	.global		WAZA_SafariBallThrowNG
	.global		WAZA_PokeReshuffle
	.global		WAZA_PokeAppear
	.global		WAZA_Wait
	.global		WAZA_LevelUp
@	.global		WAZA_LevelUpA
@	.global		WAZA_LevelUpD
	.global		WAZA_GetEXP
	.global		WAZA_Zenmetsu
	.global		WAZA_Umakukimaran
	.global		WAZA_NormalNohit
	.global		WAZA_NormalNohitWait
	.global		WAZA_NormalNohit2
	.global		WAZA_HitCalc
	.global		WAZA_HitCalcOnly
	.global		WAZA_Hit

	.global		WAZA_OrderIgnoreSleep
	.global		WAZA_OrderIgnore
	.global		WAZA_Namakeru
	.global		WAZA_Hirune
	.global		WAZA_OwnAttack

	.global		WAZA_MigawariKizetsu
	.global		WAZA_BlankMsg
	.global		WAZA_WazaEnd

@======================================================
@	�V��V�[�P���X
@======================================================

	.global		WAZA_Amefuri
	.global		WAZA_WeatherDamage
	.global		WAZA_WeatherDamageEnd
	.global		WAZA_Hare
	.global		WAZA_HareEnd
@�t�B�[���h�̓V��𔽉f
	.global		WAZA_FieldWeather

@======================================================
@	�t�B�[���h�R���f�B�V�����V�[�P���X
@======================================================

	.global		WAZA_ReflecEnd
	.global		WAZA_Shinpi
	.global		WAZA_ShinpiEnd

@======================================================
@	�Z���ʃV�[�P���X
@======================================================

	.global		WAZA_Yadorigi
	.global		WAZA_Gaman
	.global		WAZA_GamanOff
	.global		WAZA_GamanFaild
	.global		WAZA_Hukitobashi
	.global		WAZA_Shiroikiri
	.global		WAZA_CondUpMsg
	.global		WAZA_CondUpTsuikaMsg
	.global		WAZA_CondDownMsg
	.global		WAZA_CondDownTsuikaMsg
	.global		WAZA_Ikari
	.global		WAZA_Kanashibari
	.global		WAZA_Kanashibari2
	.global		WAZA_KanashibariEnd
	.global		WAZA_EncoreEnd
	.global		WAZA_Michidure
	.global		WAZA_MakibishiA
	.global		WAZA_MakibishiD
	.global		WAZA_MakibishiK
	.global		WAZA_Horobinoutaing
	.global		WAZA_Horobinouta
	.global		WAZA_Genshinochikara
	.global		WAZA_Kousokuspin
	.global		WAZA_ShimeEscape
	.global		WAZA_YadorigiEscape
	.global		WAZA_MakibishiEscape
	.global		WAZA_Miraiyochi
	.global		WAZA_Waruagaki
	.global		WAZA_PPNone
	.global		WAZA_PPNoneAttack
	.global		WAZA_Ichamon
	.global		WAZA_Ichamon2
	.global		WAZA_Chouhatsu
	.global		WAZA_Chouhatsu2
@	.global		WAZA_Mamoru
	.global		WAZA_Negaigoto
	.global		WAZA_Newoharu
	.global		WAZA_Bakadikara
	.global		WAZA_Hatakiotosu
	.global		WAZA_Huuin
	.global		WAZA_Huuin2
	.global		WAZA_Onnen
	.global		WAZA_MagicCort
	.global		WAZA_Yokodori
	.global		WAZA_Koraeta
	.global		WAZA_Kaifuku
	.global		WAZA_Mantan
	.global		WAZA_Ichigeki
	.global		WAZA_OverHeat
	.global		WAZA_Handou

@======================================================
@	�Z���o���O�Ƀ��b�Z�[�W���o���Z�V�[�P���X
@======================================================

	.global		WAZA_KiaiPunch

@======================================================
@	�|�P������Ԉُ�V�[�P���X
@======================================================

	.global		WAZA_Nemuru
	.global		WAZA_Mezame
	.global		WAZA_SawaguMezame
	.global		WAZA_Doku
	.global		WAZA_Yakedo
	.global		WAZA_Koori
	.global		WAZA_Melt
	.global		WAZA_Melt2
	.global		WAZA_Mahi
	.global		WAZA_Hirumu
	.global		WAZA_Sawagu
	.global		WAZA_AbareruEnd
	.global		WAZA_Konran
	.global		WAZA_KonranEnd
	.global		WAZA_Dokudoku
	.global		WAZA_Nekonikoban
	.global		WAZA_Shime
	.global		WAZA_ShimeEnd
	.global		WAZA_Meromero
	.global		WAZA_MeromeroNext
	.global		WAZA_Akumu
@	.global		WAZA_KaengurumaMelt
@	.global		WAZA_Kaenguruma
	.global		WAZA_Noroi
	.global		WAZA_NoMahi

@======================================================
@	�Z�ǉ����ʃV�[�P���X
@======================================================

	.global		WAZA_AddNemuri
	.global		WAZA_AddNemuri2
	.global		WAZA_AddDoku
	.global		WAZA_AddYakedo
	.global		WAZA_AddKoori
	.global		WAZA_AddMahi
	.global		WAZA_AddSawagu
	.global		WAZA_AddDokudoku
	.global		WAZA_AddNekonikoban
	.global		WAZA_AddShime
	.global		WAZA_AddKonran
	.global		WAZA_AddHanekaeri
	.global		WAZA_AddDorobou

@======================================================
@	����\�̓V�[�P���X
@======================================================

	.global		WAZA_Amehurashi
	.global		WAZA_Kasokusouti
	.global		WAZA_Ganzyou
	.global		WAZA_Shimerike
	.global		WAZA_Tikuden
	.global		WAZA_Tyosui
	.global		WAZA_TikudenPPDec
	.global		WAZA_TyosuiPPDec
	.global		WAZA_Hensyoku
	.global		WAZA_Juunan
	.global		WAZA_Donkan
	.global		WAZA_Meneki
	.global		WAZA_Mypace
	.global		WAZA_Moraibi
	.global		WAZA_MoraibiPPDec
	.global		WAZA_Ikaku
	.global		WAZA_IkakuAct
	.global		WAZA_ClearBody
	.global		WAZA_Samehada
	.global		WAZA_Housi
	.global		WAZA_Dokunotoge
	.global		WAZA_Sinkuro
	.global		WAZA_Seidenki
	.global		WAZA_Trace
	.global		WAZA_Bouon
	.global		WAZA_Ameukezara
	.global		WAZA_Sunaokosi
	.global		WAZA_Honoonokarada
	.global		WAZA_Surudoime
	.global		WAZA_Kairikibasami
	.global		WAZA_MeromeroBody
	.global		WAZA_Nentyaku
	.global		WAZA_Dappi
	.global		WAZA_Tenkiya
	.global		WAZA_Tenkiya2
	.global		WAZA_Hideri
	.global		WAZA_Hiraishin
	.global		WAZA_Mizunobeeru
	.global		WAZA_Seisinryoku

	.global		WAZA_NOKaifuku
	.global		WAZA_NOKaifukuPPDec

	.global		WAZA_SpeabiNoJoutaiijou

@======================================================
@	�����A�C�e���V�[�P���X
@======================================================

	.global		WAZA_ItemNoMahi
	.global		WAZA_ItemNoDoku
	.global		WAZA_ItemNoYakedo
	.global		WAZA_ItemNoKoori
	.global		WAZA_ItemNoNemuri
	.global		WAZA_ItemNoKonran
	.global		WAZA_ItemNoJoutaiijou
	.global		WAZA_ItemAbiKaifuku
	.global		WAZA_ItemNoMahi2
	.global		WAZA_ItemNoDoku2
	.global		WAZA_ItemNoYakedo2
	.global		WAZA_ItemNoKoori2
	.global		WAZA_ItemNoNemuri2
	.global		WAZA_ItemNoKonran2
	.global		WAZA_ItemNoJoutaiijou2
	.global		WAZA_ItemAbiKaifuku2

	.global		WAZA_ItemHPKaifuku
	.global		WAZA_ItemPPKaifuku
	.global		WAZA_ItemHPKaifukuEveryTurn
	.global		WAZA_ItemHPKaifukuEveryTurn2
	.global		WAZA_ItemOnaziWazaOnly
	.global		WAZA_ItemKoraeta
	.global		WAZA_ItemHPKaifukuKonran
	.global		WAZA_ItemCondUp
	.global		WAZA_ItemCriticalUp

@======================================================
@	�ʐM���A���[�g���b�Z�[�W
@======================================================

	.global		WAZA_DouguAlert

#include	"server.def"
#include	"tokusyu.def"
#include	"agbmons.def"
#include	"..\snd_file\mus_def.h"
#include	"wazano.def"
#include	"record.h"
#include	"fntequ.h"
#include	"fightmsg.def"
#include	"client.def"

WazaEffectTbl:
	.long	WAZA_NormalHit			@000 �m�[�}��
	.long	WAZA_SleepHit			@001 ����𖰂点��
	.long	WAZA_DokuHit			@002 �ł̒ǉ�����
	.long	WAZA_SuitoriHit			@003 �z��
	.long	WAZA_YakedoHit			@004 �₯�ǂ̒ǉ�����
	.long	WAZA_KooriHit			@005 ������̒ǉ�����
	.long	WAZA_MahiHit			@006 �܂Ђ̒ǉ�����
	.long	WAZA_JibakuHit			@007 ����
	.long	WAZA_YumekuiHit			@008 ����
	.long	WAZA_OumuHit			@009 �I�E���Ԃ�
	.long	WAZA_PowUp1Hit			@010 �����̍U���͂��P�����N�グ��
	.long	WAZA_DefUp1Hit			@011 �����̖h��͂��P�����N�グ��
	.long	WAZA_NormalHit			@012 �����̑f�������P�����N�グ��
	.long	WAZA_SpePowUp1Hit		@013 �����̓���U���͂��P�����N�グ��
	.long	WAZA_NormalHit			@014 �����̓���h��͂��P�����N�グ��
	.long	WAZA_NormalHit			@015 �����̖��������P�����N�グ��
	.long	WAZA_AvoidUp1Hit		@016 �����̉�𗦂��P�����N�グ��
	.long	WAZA_NormalHit			@017 �K���q�b�g����(17�Ƃ��������őΏ��j
	.long	WAZA_PowDown1Hit		@018 ����̍U���͂��P�����N������
	.long	WAZA_DefDown1Hit		@019 ����̖h��͂��P�����N������
	.long	WAZA_AgiDown1Hit		@020 ����̑f�������P�����N������
	.long	WAZA_NormalHit			@021 ����̓���U���͂��P�����N������
	.long	WAZA_NormalHit			@022 ����̓���h��͂��P�����N������
	.long	WAZA_HitDown1Hit		@023 ����̖��������P�����N������
	.long	WAZA_AvoidDown1Hit		@024 ����̉�𗦂��P�����N������
	.long	WAZA_KuroikiriHit		@025 �o���ɂ������Ă����ԕω��ƁA�U���⏕�n�̋Z�̌��ʂ���������
	.long	WAZA_GamanHit			@026 ���܂�
	.long	WAZA_AbareruHit			@027 ���΂��
	.long	WAZA_HukitobashiHit		@028 �ӂ��Ƃ΂�
	.long	WAZA_RenzokuHit			@029 �Q�`�T�^�[���̘A���U��
	.long	WAZA_TextureHit			@030 �e�N�X�`���[
	.long	WAZA_HirumuHit			@031 �ǉ����ʂő��肪�Ђ��
	.long	WAZA_JikosaiseiHit		@032 ���ȍĐ�
	.long	WAZA_DokudokuDirectHit	@033 ����ɓł�^����i�ǂ��ǂ��j
	.long	WAZA_NekonikobanHit		@034 �˂��ɂ��΂�
	.long	WAZA_HikarinokabeHit	@035 ����U���Ŏ󂯂�_���[�W���P�^�Q�ɂȂ�
	.long	WAZA_TriAttackHit		@036 �g���C�A�^�b�N
	.long	WAZA_NemuruHit			@037 �˂ނ�
	.long	WAZA_IchigekiHit		@038 �ꌂ�K�E
	.long	WAZA_KamaitachiHit		@039 �P�^�[�����߂čU���i��p���b�Z�[�W���K�v�j
	.long	WAZA_IkarinomaebaHit	@040 ����̂g�o�𔼕��ɂ���
	.long	WAZA_Kotei40Hit			@041 �Œ�_���[�W40
	.long	WAZA_ShimeHit			@042 �Q�`�T�^�[�����߂�
	.long	WAZA_NormalHit			@043 �N���e�B�J���q�b�g�ɂȂ�₷��(043�Ƃ��������őΏ�)
	.long	WAZA_Renzoku2Hit		@044 �A���łQ��U����������
	.long	WAZA_TobigeriHit		@045 �U�����O���ƁA�^����͂��̃_���[�W�̂P�^�Q���������󂯂�
	.long	WAZA_ShiroiKiriHit		@046 �X�e�[�^�X��ω�������⏕�U�����󂯂Ȃ��Ȃ�
	.long	WAZA_KiaidameHit		@047 �N���e�B�J�������A�b�v����
	.long	WAZA_HanekaeriHit		@048 �^�����_���[�W�̂P�^�S�������ɂ͂˂������Ă���
	.long	WAZA_KonranDirectHit	@049 ���������������
	.long	WAZA_PowUp2Hit			@050 �����̍U���͂��Q�����N�グ��
	.long	WAZA_DefUp2Hit			@051 �����̖h��͂��Q�����N�グ��
	.long	WAZA_AgiUp2Hit			@052 �����̑f�������Q�����N�グ��
	.long	WAZA_SpePowUp2Hit		@053 �����̓���U���͂��Q�����N�グ��
	.long	WAZA_SpeDefUp2Hit		@054 �����̓���h��͂��Q�����N�グ��
	.long	WAZA_NormalHit			@055 �����̖��������Q�����N�グ��
	.long	WAZA_NormalHit			@056 �����̉�𗦂��Q�����N�グ��
	.long	WAZA_HenshinHit			@057 �ւ񂵂�
	.long	WAZA_PowDown2Hit		@058 ����̍U���͂��Q�����N������
	.long	WAZA_DefDown2Hit		@059 ����̖h��͂��Q�����N������
	.long	WAZA_AgiDown2Hit		@060 ����̑f�������Q�����N������
	.long	WAZA_NormalHit			@061 ����̓���U���͂��Q�����N������
	.long	WAZA_SpeDefDown2Hit		@062 ����̓���h��͂��Q�����N������
	.long	WAZA_NormalHit			@063 ����̖��������Q�����N������
	.long	WAZA_NormalHit			@064 ����̉�𗦂��Q�����N������
	.long	WAZA_ReflecHit			@065 �����U���Ŏ󂯂�_���[�W���P�^�Q�ɂȂ�
	.long	WAZA_DokuDirectHit		@066 ����ɓł�^����
	.long	WAZA_MahiDirectHit		@067 ������܂Ђ�����
	.long	WAZA_PowDownAddHit		@068 �ǉ����ʂő���̍U���͂��P�����N������
	.long	WAZA_DefDownAddHit		@069 �ǉ����ʂő���̖h��͂��P�����N������
	.long	WAZA_AgiDownAddHit		@070 �ǉ����ʂő���̑f�������P�����N������
	.long	WAZA_SpePowDownAddHit	@071 �ǉ����ʂő���̓���U���͂��P�����N������
	.long	WAZA_SpeDefDownAddHit	@072 �ǉ����ʂő���̓���h��͂��P�����N������
	.long	WAZA_HitDownAddHit		@073 �ǉ����ʂő���̖��������P�����N������
	.long	WAZA_NormalHit			@074 �ǉ����ʂő���̉�𗦂��P�����N������
	.long	WAZA_GodBirdHit			@075 ���߁{�Ђ�ނ̒ǉ�����
	.long	WAZA_KonranAddHit		@076 �ǉ����ʂő��������������
	.long	WAZA_RenzokuDokuAddHit	@077 �A���łQ��U�����A�ǉ����ʂő�����ǂ���Ԃɂ���
	.long	WAZA_NormalHit			@078 ��U�ɂȂ邪�A�U���͕K��������(78�Ƃ��������őΏ�)
	.long	WAZA_MigawariHit		@079 �݂������o��
	.long	WAZA_HandouHit			@080 �U���̎��̂P�^�[���s���s�\�ɂȂ�
	.long	WAZA_IkariHit			@081 ������
	.long	WAZA_MonomaneHit		@082 ���̂܂�
	.long	WAZA_YubiwohuruHit		@083 ��т��ӂ�
	.long	WAZA_YadorigiHit		@084 ��ǂ肬
	.long	WAZA_HaneruHit			@085 ����������Ȃ�
	.long	WAZA_KanashibariHit		@086 ���肪���O�ɏo�����Z�𕕂���
	.long	WAZA_ChikyuuNageHit		@087 �����̃��x�����̃_���[�W��^����
	.long	WAZA_PsychoWaveHit		@088 �P�`�����̃��x�����̃_���[�W��^����
	.long	WAZA_CounterHit			@089 �J�E���^�[
	.long	WAZA_EncoreHit			@090 �A���R�[��
	.long	WAZA_ItamiwakeHit		@091 �����݂킯
	.long	WAZA_IbikiHit			@092 ���т�
	.long	WAZA_Texture2Hit		@093 �����̃^�C�v�������_���ŕς��Ă��܂�
	.long	WAZA_LockOnHit			@094 ���̎����̍U���̖��������P�O�O���ɂ���
	.long	WAZA_SketchHit			@095 �X�P�b�`
	.long	WAZA_NormalHit			@096 �U���͂��P�i�K�グ��{�������Ԃ̏ꍇ�͂����肪�n����
	.long	WAZA_NegotoHit			@097 �����������Ԃ̎��A���̂R�̋Z�̂P�������_���ŏo��
	.long	WAZA_MichidureHit		@098 ���̑���̍U���Ŏ������Ђ񂵂ɂȂ�ƁA������Ђ񂵂ɂȂ�
	.long	WAZA_JitabataHit		@099 �����̗̑͂����Ȃ��قǍU���͂��オ��
	.long	WAZA_UramiHit			@100 ����̂o�o���Q�`�T���
	.long	WAZA_NormalHit			@101 ����̗̑͂�K���P�c���i101�Ƃ��������őΏ��j
	.long	WAZA_IyashinosuzuHit	@102 �����̏�ԕω��𒼂�
	.long	WAZA_NormalHit			@103 �搧�U���ɂȂ�(�Z���X�g�̃v���C�I���e�B�őΏ��j
	.long	WAZA_TripleKickHit		@104 �g���v���L�b�N�B�P�`�R�񓖂���B�_���[�W��1���Q���R
	.long	WAZA_DorobouHit			@105 ���肪�������Ă���ǂ����𓐂�
	.long	WAZA_KuroimanazashiHit	@106 ����𓦂����Ȃ�����B
	.long	WAZA_AkumuHit			@107 ����
	.long	WAZA_ChiisakunaruHit	@108 ���������Ȃ�
	.long	WAZA_NoroiHit			@109 �̂낢
	.long	WAZA_NormalHit			@110 ���΂�Ȃ�
	.long	WAZA_MamoruHit			@111 ���̑���̍U�����󂯂Ȃ�
	.long	WAZA_MakibishiHit		@112 �܂��т�
	.long	WAZA_MiyaburuHit		@113 �݂�Ԃ�
	.long	WAZA_HorobinoutaHit		@114 �ق�т̂���
	.long	WAZA_SunaarashiHit		@115 ���Ȃ��炵
	.long	WAZA_KoraeruHit			@116 ���̑���̍U�����󂯂Ă��̗͂��P�c��
	.long	WAZA_KorogaruHit		@117 �T�^�[���A���ōU���A�^�[�����ƂɈЗ͂��オ��
	.long	WAZA_IbaruHit			@118 ����̍U���͂��Q�i�K�グ�Ă����񂳂���
	.long	WAZA_RenzokugiriHit		@119 �A���ŏo�����ƂɈЗ͂��オ��
	.long	WAZA_MeromeroHit		@120 ���������������Ԃɂ���
	.long	WAZA_NatsukiHit			@121 �Ȃ��x�������قǈЗ͂�����
	.long	WAZA_PresentHit			@122 �v���[���g
	.long	WAZA_NatsukiHit			@123 �Ȃ��x���Ⴂ�قǈЗ͂�����
	.long	WAZA_ShinpinomamoriHit	@124 �T�^�[���̊ԁA��ԕω��i�ǂ��A�܂Ёj���󂯂Ȃ�
	.long	WAZA_KaengurumaHit		@125 �₯�ǂ̒ǉ����ʁ{�������Ԃ̏ꍇ�͂����肪�n����
	.long	WAZA_MagnitudeHit		@126 �}�O�j�`���[�h
	.long	WAZA_BatontouchHit		@127 �o�g���^�b�`
	.long	WAZA_NormalHit			@128 ���������i�Z�i���o�[�őΉ��j
	.long	WAZA_KousokuSpinHit		@129 ���������X�s��
	.long	WAZA_Kotei20Hit			@130 �Œ�_���[�W20
	.long	WAZA_NormalHit			@131 ���g�p
	.long	WAZA_WeatherkaifukuHit	@132 �̗͂𔼕��񕜁B�����ƑS��
	.long	WAZA_WeatherkaifukuHit	@133 �̗͂𔼕��񕜁B�����ƑS��
	.long	WAZA_WeatherkaifukuHit	@134 �̗͂𔼕��񕜁B��A�[�邾�ƑS��
	.long	WAZA_MezameruPowerHit	@135 �߂��߂�p���[
	.long	WAZA_AmagoiHit			@136 ���܂���
	.long	WAZA_NihonbareHit		@137 �ɂق�΂�
	.long	WAZA_DefUpAddHit		@138 �ǉ����ʂŎ����̖h��͂��P�i�K�グ��
	.long	WAZA_PowUpAddHit		@139 �ǉ����ʂŎ����̍U���͂��P�i�K�グ��
	.long	WAZA_GenshinochikaraHit	@140 �ǉ����ʂłT�̃X�e�[�^�X���P�i�K�オ��
	.long	WAZA_NormalHit			@141 ������Ђ�܂���i���g�p�j
	.long	WAZA_HaradaikoHit		@142 �͂炾����
	.long	WAZA_JikoanjiHit		@143 ����ɂ������Ă���⏕���ʂ��R�s�[����
	.long	WAZA_MirrorCoatHit		@144 ����̓���U���̃_���[�W��{�ɂ��ĕԂ�
	.long	WAZA_RocketzutukiHit	@145  ���߁{�h��̓A�b�v
	.long	WAZA_TatumakiHit		@146 �Ђ�ށ@�{�@����Ƃ�ł��鑊��ɔ{�̃_���[�W
	.long	WAZA_JisinHit			@147 �n�ʂɐ����Ă��鑊��ɔ{�̃_���[�W
	.long	WAZA_MiraiyochiHit		@148 �݂炢�悿�@�R�^�[����ɍU��������
	.long	WAZA_KazeokosiHit		@149 ����Ƃ�ł��鑊��ɔ{�̃_���[�W
	.long	WAZA_HumitukeHit		@150 �������Ȃ��Ă��鑊��ɔ{�̃_���[�W
	.long	WAZA_SolarBeamHit		@151 �\�[���[�r�[���@�͂ꂾ�Ƃ��߂��ɍU���ł���
	.long	WAZA_KaminariHit		@152 �J���Ɩ������A�b�v�@�͂ꂾ�ƃ_�E��
	.long	WAZA_TeleportHit		@153 �e���|�[�g�i�쐫�̃|�P�����Ƃ̐퓬���痣�E�j
	.long	WAZA_HukurodatakiHit	@154 �ӂ��낾����
	.long	WAZA_KieruTameHit		@155 ������ƂԁA���Ȃ��ق�
	.long	WAZA_MarukunaruHit		@156 �܂邭�Ȃ�
	.long	WAZA_KaifukuHit			@157 �~���N�̂݁A�^�}�S����
	.long	WAZA_NekodamashiHit		@158 �˂����܂�
	.long	WAZA_SawaguHit			@159 ���킮
	.long	WAZA_TakuwaeruHit		@160 �������킦��
	.long	WAZA_HakidasuHit		@161 �͂�����
	.long	WAZA_NomikomuHit		@162 �݂̂���
	.long	WAZA_NormalHit			@163 ���g�p
	.long	WAZA_ArareHit			@164 �����
	.long	WAZA_IchamonHit			@165 ���������
	.long	WAZA_OdateruHit			@166 �����Ă�
	.long	WAZA_OnibiHit			@167 ���ɂ�
	.long	WAZA_OkimiyageHit		@168 �����݂₰
	.long	WAZA_KaragenkiHit		@169 ���炰��
	.long	WAZA_KiaiPunchHit		@170 �������p���`
	.long	WAZA_KitsukeHit			@171 ����
	.long	WAZA_KonoyubitomareHit	@172 ���̂�тƂ܂�
	.long	WAZA_ShizennochikaraHit	@173 ������̂�����
	.long	WAZA_JuudenHit			@174 ���イ�ł�
	.long	WAZA_ChouhatsuHit		@175 ���傤�͂�
	.long	WAZA_TedasukeHit		@176 �Ă�����
	.long	WAZA_TrickHit			@177 �g���b�N
	.long	WAZA_NarikiriHit		@178 �Ȃ肫��
	.long	WAZA_NegaigotoHit		@179 �˂�������
	.long	WAZA_NekonoteHit		@180 �˂��̂�
	.long	WAZA_NewoharuHit		@181 �˂��͂�
	.long	WAZA_BakadikaraHit		@182 �΂�������
	.long	WAZA_MagicCortHit		@183 �}�W�b�N�R�[�g
	.long	WAZA_RecycleHit			@184 ���T�C�N��
	.long	WAZA_RevengeHit			@185 ���x���W
	.long	WAZA_KawarawariHit		@186 �������i186�Ƃ��������őΏ��j
	.long	WAZA_AkubiHit			@187 ������
	.long	WAZA_HatakiotosuHit		@188 �͂������Ƃ�
	.long	WAZA_GamusyaraHit		@189 ���ނ����
	.long	WAZA_HunkaHit			@190 �ӂ�
	.long	WAZA_SkillSwapHit		@191 �X�L���X���b�v
	.long	WAZA_HuuinHit			@192 �ӂ�����
	.long	WAZA_RefleshHit			@193 ���t���b�V��
	.long	WAZA_OnnenHit			@194 ����˂�
	.long	WAZA_YokodoriHit		@195 �悱�ǂ�
	.long	WAZA_KetaguriHit		@196 ��������
	.long	WAZA_HimitunoTikaraHit	@197 �Ђ݂̂�����
	.long	WAZA_SutemiTackleHit	@198 ���Ă݃^�b�N��
	.long	WAZA_AllKonranHit		@199 �S�Ẵ|�P����������
	.long	WAZA_YakedoHit			@200 �}���ɂ�����₷���{�₯�ǒǉ�
	.long	WAZA_AsobiHit			@201 �d�C�n�̋Z�̈З͂𔼌�
	.long	WAZA_DokuDokuHit		@202 �ǉ����ʂłǂ��ǂ�
	.long	WAZA_WeatherHit			@203 �V�C�ɂ���ĈЗ͂ƃ^�C�v
	.long	WAZA_OverHeatHit		@204 �����̓��U���A�������Ɖ�����
	.long	WAZA_KusuguruHit		@205 ����̍U���A�h�䂪������
	.long	WAZA_CosmoPowerHit		@206 �h��A���h��������
	.long	WAZA_SkyUpperHit		@207 �����ł��鑊��ɂ�������
	.long	WAZA_BuildUpHit			@208 �U���A�h�䂪������
	.long	WAZA_DokuHit			@209 �}���ɂ�����₷���{�Œǉ�
	.long	WAZA_AsobiHit			@210 ���n�̋Z�̈З͂𔼌�
	.long	WAZA_MeisouHit			@211 ���U�A���h��������
	.long	WAZA_RyuunomaiHit		@212 �U���Ƒf������������
	.long	WAZA_HogosyokuHit		@213 �t�B�[���h�Ɠ����^�C�v�ɂȂ�

@===================================================
@	000 �m�[�}��
@===================================================

WAZA_NormalHit:
	IF16				FLAG_NE,WazaNo,WAZANO_NAMINORI,WAZA_NormalHitStart
	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_DAIBINGU,FLAG_OFF,WAZA_NormalHitStart
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_DAIBINGUHIT
	SETVALUE			UserWork+DAMAGE_VALUE,2
WAZA_NormalHitStart:
	BADGECHECK
WAZA_NormalHit2:		@�o�b�W�`�F�b�N�͂��Ȃ�
	HITCHECK			WAZA_NormalNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
WAZA_HitCalc:
	ATTACK_MESSAGE
	PP_DEC
WAZA_HitCalcOnly:
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	DAMAGE_LOSS
WAZA_Hit:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	TSUIKA_CHECK
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
WAZA_WazaEnd:
	SETVALUE			UserWork+WKC_WORK0,0
	WKC_ACT				WKC_CHECK_ALL,0
	WAZAEND
WAZA_NormalNohitBitSet:
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_NOHIT
WAZA_NormalNohit:
	ATTACK_MESSAGE
	PP_DEC
WAZA_NormalNohitWait:
	WAIT				MOJI_WAIT/2
WAZA_NormalNohit2:
	WAZAKOUKA_SE
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	001 ����𖰂点��
@===================================================

WAZA_SleepHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_NEMURI,WAZA_SleepAlready
	SAWAGU_CHECK		WAZA_SleepNG
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_BAD,WAZA_Umakukimaran
@	HITCHECK			WAZA_SleepNohit,0
	HITCHECK			WAZA_Umakukimaran,0
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			ServerWork+3,COND_NEMURI
	TSUIKA_DIRECT_ACT
	JUMP				WAZA_WazaEnd

WAZA_SleepAlready:
	WAIT				MOJI_WAIT/2
	MESSAGE				ALREADYSLEEPMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd
WAZA_SleepNohit:
	WAIT				MOJI_WAIT/2
	MESSAGE				SIPPAIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd
WAZA_SleepNG:
	WAIT				MOJI_WAIT/2
	MESSAGE_TBL			SleepNGMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd
	
@===================================================
@	002 �ł̒ǉ�����
@===================================================

WAZA_DokuHit:
	SETVALUE			ServerWork+3,COND_DOKU
	JUMP				WAZA_NormalHit

@===================================================
@	003 �z��
@===================================================

WAZA_SuitoriHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	ATTACK_MESSAGE
	PP_DEC
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	DAMAGE_LOSS
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	SUITORI
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_HEDOROEKI,WAZA_SuitoriHedoro
	SETVALUE			ServerWork+5,0
	JUMP				WAZA_SuitoriNext
WAZA_SuitoriHedoro:
	DAMAGE_CHG			DAMAGE_NEG			@�v���}�C���]
	SETVALUE			ServerWork+5,1
WAZA_SuitoriNext:
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_SuitoriNoMsg
	MESSAGE_TBL			SuitoriMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_SuitoriNoMsg:
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	JUMP				WAZA_WazaEnd

@===================================================
@	004 �₯�ǂ̒ǉ�����
@===================================================

WAZA_YakedoHit:
	SETVALUE			ServerWork+3,COND_YAKEDO
	JUMP				WAZA_NormalHit

@===================================================
@	005 ������̒ǉ�����
@===================================================

WAZA_KooriHit:
	SETVALUE			ServerWork+3,COND_KOORI
	JUMP				WAZA_NormalHit

@===================================================
@	006 �܂Ђ̒ǉ�����
@===================================================

WAZA_MahiHit:
	SETVALUE			ServerWork+3,COND_MAHI
	JUMP				WAZA_NormalHit

@===================================================
@	007 ����
@===================================================

WAZA_JibakuHit:
@	BADGECHECK2			@�܂���`�F�b�N�̂Ȃ��o�b�W�`�F�b�N
	BADGECHECK	
	ATTACK_MESSAGE
	PP_DEC
	JIBAKU_INIT
	JIBAKU
	CLIENT_BIT_WAIT
	IF8					FLAG_NBIT,WazaStatusFlag,WAZASTATUSFLAG_NOHIT,WAZA_JibakuStart
	GOSUB				WAZA_JibakuEffect
	JUMP				WAZA_JibakuLoop
WAZA_JibakuStart:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
WAZA_JibakuLoop:
@	SETVALUE			WazaStatusFlag,0
	LOOPINIT			@���[�v����Ƃ��ɏ��������郏�[�N���܂Ƃ܂��Ă���
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK	
	DAMAGE_LOSS
	HITCHECK			WAZA_JibakuNohit,0
	WAZAKOUKA_SE
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	JIBAKU_LOOP			WAZA_JibakuLoop
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	WAZAEND

WAZA_JibakuNohit:
	WAZAKOUKA_SE
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	JIBAKU_LOOP			WAZA_JibakuLoop
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	WAZAEND

WAZA_JibakuEffect:
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_NOHIT
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_NOHIT
	SEQRET

.if 0
WAZA_JibakuMamoru:
	ATTACK_MESSAGE
	PP_DEC
	WAIT				MOJI_WAIT
	MESSAGE				MAMORUNOHITMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JIBAKU_INIT
	JIBAKU
	CLIENT_BIT_WAIT
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_NOHIT
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_NOHIT
	WAZAKOUKA_SE
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	JUMP				WAZA_WazaEnd
.endif

@===================================================
@	008 ����
@===================================================

WAZA_YumekuiHit:
	BADGECHECK
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_YumekuiHitNG
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_NEMURI,WAZA_YumekuiHitOK
WAZA_YumekuiHitNG:
	ATTACK_MESSAGE
	PP_DEC
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_SleepNohit
WAZA_YumekuiHitOK:
	HITCHECK			WAZA_NormalNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	ATTACK_MESSAGE
	PP_DEC
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	DAMAGE_LOSS
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	SUITORI
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_YumekuiNoMsg
	MESSAGE				YUMEKUIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_YumekuiNoMsg:
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	JUMP				WAZA_WazaEnd

@===================================================
@	009 �I�E���Ԃ�
@===================================================

WAZA_OumuHit:
	BADGECHECK
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT
	OUMU_CHECK
	PP_DEC
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_UMAKUKIMARAN
	MESSAGE				OUMUNGMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	010 �����̍U���͂��P�����N�グ��
@===================================================

WAZA_PowUp1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKU1
	JUMP				WAZA_CondUp

@===================================================
@	011 �����̖h��͂��P�����N�グ��
@===================================================

WAZA_DefUp1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKU1
	JUMP				WAZA_CondUp

@===================================================
@	013 �����̓���U���͂��P�����N�グ��
@===================================================

WAZA_SpePowUp1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEPOW+PARA_RANKU1
	JUMP				WAZA_CondUp

@===================================================
@	016 �����̉�𗦂��P�����N�グ��
@===================================================
WAZA_AvoidUp1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_AVOID+PARA_RANKU1

@===================================================
@	�����̃p�����[�^���グ��
@===================================================

WAZA_CondUp:
	BADGECHECK
WAZA_CondUp2:
	ATTACK_MESSAGE
	PP_DEC
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_CondUpEnd
@������Ȃ����b�Z�[�W�̂Ƃ��́A�Z�G�t�F�N�g�������Ȃ�
	IF8					FLAG_NE,ServerWork+5,2,WAZA_CondUpEff
	WAIT				MOJI_WAIT/2
	JUMP				WAZA_CondUpMsg
WAZA_CondUpEff:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
WAZA_CondUpStatusEffect:
	COND_CHG_WORK_SET
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
WAZA_CondUpMsg:
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_CondUpEnd:
	JUMP				WAZA_WazaEnd

WAZA_CondUpTsuikaMsg:
	FIGHT_EFFECT		SIDE_TSUIKA,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@===================================================
@	018 ����̍U���͂��P�����N������
@===================================================

WAZA_PowDown1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKD1
	JUMP				WAZA_CondDown

@===================================================
@	019 ����̖h��͂��P�����N������
@===================================================

WAZA_DefDown1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKD1
	JUMP				WAZA_CondDown

@===================================================
@	020 ����̑f�������P�����N������
@===================================================

WAZA_AgiDown1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_AGI+PARA_RANKD1
	JUMP				WAZA_CondDown

@===================================================
@	023 ����̖��������P�����N������
@===================================================

WAZA_HitDown1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_HIT+PARA_RANKD1
	JUMP				WAZA_CondDown

@===================================================
@	024 ����̉�𗦂��P�����N������
@===================================================

WAZA_AvoidDown1Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_AVOID+PARA_RANKD1

@===================================================
@	����̃p�����[�^��������
@===================================================

WAZA_CondDown:
	BADGECHECK
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_UmakukimaranPP
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	CONDCHG				TSUIKA_DIRECT,WAZA_CondDownEnd
@������Ȃ����b�Z�[�W�̂Ƃ��́A�Z�G�t�F�N�g�������Ȃ�
	IF8					FLAG_NC,ServerWork+5,2,WAZA_CondDownEff
	IF8					FLAG_EQ,ServerWork+5,3,WAZA_CondDownEnd
	WAIT				MOJI_WAIT/2
	JUMP				WAZA_CondDownMsg
WAZA_CondDownEff:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
WAZA_CondDownStatusEffect:
	COND_CHG_WORK_SET
	FIGHT_EFFECT		SIDE_DEFENCE,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
WAZA_CondDownMsg:
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_CondDownEnd:
	JUMP				WAZA_WazaEnd

WAZA_CondDownTsuikaMsg:
	FIGHT_EFFECT		SIDE_TSUIKA,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@===================================================
@	025 �o���ɂ������Ă����ԕω��ƁA�U���⏕�n�̋Z�̌��ʂ���������
@===================================================

WAZA_KuroikiriHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	KUROIKIRI
	MESSAGE				KUROIKIRIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	026 ���܂�
@===================================================

WAZA_GamanHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_TAME
	GAMAN
	JUMP				WAZA_WazaEnd

@===================================================
@	027 ���΂��
@===================================================

WAZA_AbareruHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
@���΂�Ă���Ԃ�PP��DEC���Ȃ�
	ATTACK_MESSAGE
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KEEP,WAZA_AbareruNoPPDec
	PP_DEC
WAZA_AbareruNoPPDec:
	ABARERU
	JUMP				WAZA_HitCalcOnly

@===================================================
@	028 �ӂ��Ƃ΂�
@===================================================

WAZA_HukitobashiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_KYUUBAN,WAZA_Kyuuban
	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_NEWOHARU,FLAG_ON,WAZA_Newoharare
@������ƂԂƂ��Ȃ��ق�
@	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_KIE,FLAG_ON,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,HITCHECK_KIE
	HITCHECK			WAZA_NormalNohit2,0
	HUKITOBASHI			WAZA_Umakukimaran

@===================================================
@	029 �Q�`�T�^�[���̘A���U��
@===================================================

WAZA_RenzokuHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	ATTACK_MESSAGE
	PP_DEC
	RENZOKU				0					@0�̏ꍇ�͂Q�`�T��
	RENZOKU_NUM_INIT
	@�_�u���j�[�h�����g���Ƃ��̃��[�N�ɒl�������Ă���̂ŁA���������Ă����i�O�̂��߁j
	SETVALUE			UserWork+SERVER_WORK3_TEMP,0
WAZA_RenzokuLoop:
	HP_CHECK			SIDE_ATTACK,WAZA_RenzokuNoMsg
	HP_CHECK			SIDE_DEFENCE,WAZA_RenzokuNoHit
	IF16				FLAG_EQ,WazaNoTmp,WAZANO_NEGOTO,WAZA_RenzokuStart
	CONDITION_CHECK		SIDE_ATTACK,CONDITION_NEMURI,WAZA_RenzokuNoHit
WAZA_RenzokuStart:
@	SETVALUE			WazaStatusFlag,0
	LOOPINIT			@���[�v����Ƃ��ɏ��������郏�[�N���܂Ƃ܂��Ă���
	SETWORK				ServerWork+3,UserWork+SERVER_WORK3_TEMP,1
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_RenzokuNoHitWait
	DAMAGE_LOSS
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	MESSAGE				BLANKMSGNO
	MESSAGE_WAIT		1
@	ADDVALUE			EvStrBuf0+4,1
	ADDVALUE			UserWork+RENZOKU_COUNT+4,1
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_KORAETA,WAZA_RenzokuNoHit
	LOOP				WAZA_RenzokuLoop
	JUMP				WAZA_RenzokuNoHit
WAZA_RenzokuNoHitWait:
	WAIT				MOJI_WAIT/2
WAZA_RenzokuNoHit:
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_RenzokuNoMsg
	SETWORK				EvStrBuf0,UserWork+RENZOKU_COUNT,6
	MESSAGE				RENZOKUHITMSGNO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_RenzokuNoMsg:
	TSUIKA_CHECK
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	SETVALUE			UserWork+WKC_WORK0,WKC_SPEABI_SINKURO_DEFENCE
	WKC_ACT				WKC_CHECK_ONLY,0
	SETVALUE			UserWork+WKC_WORK0,WKC_SPEABI_CHECK_ALL
	WKC_ACT				WKC_CHECK_ALL,0
	WAZAEND

@===================================================
@	030 �e�N�X�`���[
@===================================================

WAZA_TextureHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	TEXTURE				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				TEXTUREMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	031 �ǉ����ʂő��肪�Ђ��
@===================================================

WAZA_HirumuHit:
	SETVALUE			ServerWork+3,COND2_HIRUMU
	JUMP				WAZA_NormalHit

@===================================================
@	032 ���ȍĐ�
@===================================================

WAZA_JikosaiseiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	KAIFUKU				WAZA_Mantan,SIDE_ATTACK
Jikosaisei:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	MESSAGE				JIKOSAISEIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	033 ����ɓł�^����i�ǂ��ǂ��j
@===================================================

WAZA_DokudokuDirectHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_MENEKI,WAZA_MenekiJump
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_DOKU,WAZA_DokudokuAlready
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_DOKUDOKU,WAZA_DokudokuAlready
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_BAD,WAZA_Umakukimaran
	CHECK_TYPE			SIDE_DEFENCE,POISON_TYPE,WAZA_Koukanai
	CHECK_TYPE			SIDE_DEFENCE,METAL_TYPE,WAZA_Koukanai
	HITCHECK			WAZA_Umakukimaran,0
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			ServerWork+3,COND_DOKUDOKU
	TSUIKA_DIRECT_ACT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
WAZA_DokudokuDirectHitEnd:
	JUMP				WAZA_WazaEnd
WAZA_DokudokuAlready:
	WAIT				MOJI_WAIT
	MESSAGE				DOKUMSG3NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_MenekiJump:
	SETWORK				TsuikaClient,DefenceClient,1
	SETVALUE			ServerWork+5,0
	GOSUB				WAZA_Meneki
	JUMP				WAZA_WazaEnd
	

@===================================================
@	034 �˂��ɂ��΂�
@===================================================

WAZA_NekonikobanHit:
	SETVALUE			ServerWork+3,COND2_NEKONIKOBAN
	JUMP				WAZA_NormalHit

@===================================================
@	035 ����U���Ŏ󂯂�_���[�W���P�^�Q�ɂȂ�
@===================================================

WAZA_HikarinokabeHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HIKARINOKABE				
	JUMP				WAZA_Kabe

@===================================================
@	036 �g���C�A�^�b�N
@===================================================

WAZA_TriAttackHit:
	SETVALUE			ServerWork+3,COND2_TRIATTACK
	JUMP				WAZA_NormalHit

@===================================================
@	037 �˂ނ�
@===================================================

WAZA_NemuruHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION_CHECK		SIDE_ATTACK,CONDITION_NEMURI,WAZA_SleepAlreadyA
	SAWAGU_CHECK		WAZA_NemuruNG
	NEMURU				WAZA_Mantan
	WAIT				MOJI_WAIT/2
	MESSAGE_TBL			NemuruMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	CONDITION_GAUGE		SIDE_ATTACK
	CLIENT_BIT_WAIT
	JUMP				WAZA_Kaifuku

WAZA_NemuruNG:
	WAIT				MOJI_WAIT
	MESSAGE_TBL			SleepNGMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_SleepAlreadyA:
	WAIT				MOJI_WAIT/2
	MESSAGE				ALREADYSLEEPMSGANO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	038 �ꌂ�K�E
@===================================================

WAZA_IchigekiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
@	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_KIE,FLAG_ON,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,HITCHECK_KIE
	TYPE_CHECK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_Hit
	ICHIGEKI			WAZA_IchigekiNohit
	MICHIDURE_CHECK
	JUMP				WAZA_Hit
WAZA_IchigekiNohit:
	WAIT				MOJI_WAIT
	MESSAGE_TBL			IchigekiMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	039 �P�^�[�����߂čU���i��p���b�Z�[�W���K�v�j
@===================================================

WAZA_KamaitachiHit:
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KEEP,WAZA_TameAftar
	IF32				FLAG_BIT,ServerStatusFlag,SERVER_STATUS_FLAG_DOUBLE_CHECK,WAZA_TameAftar
	SETVALUE			UserWork+TAME_MSG_NO,0		@���܂������̃��b�Z�[�W�i���o�[
	GOSUB				WAZA_TameBefore
	JUMP				WAZA_WazaEnd
WAZA_TameAftar:
	BADGECHECK
	SETVALUE			ServerWork+3,COND2_KEEP
	SETVALUE			UserWork+WAZA_EFF_CNT,1	@���ߌ�̋Z�G�t�F�N�g�Ăяo��
	SAKUJO				SIDE_ATTACK
@���߂����PP_DEC�����Ȃ�
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NOPP_DEC
	IF16				FLAG_NE,WazaNo,WAZANO_GODDOBAADO,WAZA_NormalHit2
	SETVALUE			ServerWork+3,COND2_HIRUMU
	JUMP				WAZA_NormalHit2
WAZA_TameBefore:
	BADGECHECK
@	ATTACK_MESSAGE
@	CLIENT_BIT_WAIT
	MESSAGE				BLANKMSGNO
	PP_DEC
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_TAME
	SETVALUE			ServerWork+3,COND2_KEEP+TSUIKA_ATTACK
	TSUIKA_DIRECT_ACT
	SETWORK				ServerWork+5,UserWork+TAME_MSG_NO,1		@���ߋZ�n�̃��b�Z�[�W�i���o�[
	MESSAGE_TBL			TameMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@===================================================
@	040 ����̂g�o�𔼕��ɂ���
@===================================================

WAZA_IkarinomaebaHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_BATSUGUN|WAZASTATUSFLAG_IMAHITOTSU
	HPHALF
	JUMP				WAZA_Hit

@===================================================
@	041 �Œ�_���[�W40
@===================================================

WAZA_Kotei40Hit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_BATSUGUN|WAZASTATUSFLAG_IMAHITOTSU
	SETVALUE			WazaDamage,40
	SETVALUE			WazaDamage+1,0
	SETVALUE			WazaDamage+2,0
	SETVALUE			WazaDamage+3,0
	KORAERU_CHECK
	JUMP				WAZA_Hit

@===================================================
@	042 �Q�`�T�^�[�����߂�
@===================================================

WAZA_ShimeHit:
	IF16				FLAG_NE,WazaNo,WAZANO_UZUSIO,WAZA_ShimeHitStart
	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_DAIBINGU,FLAG_OFF,WAZA_ShimeHitStart
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_DAIBINGUHIT
	SETVALUE			UserWork+DAMAGE_VALUE,2
WAZA_ShimeHitStart:
	SETVALUE			ServerWork+3,COND2_SHIME
	JUMP				WAZA_NormalHit

@===================================================
@	044 �A���łQ��U����������
@===================================================

WAZA_Renzoku2Hit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	ATTACK_MESSAGE
	PP_DEC
	RENZOKU				2					@0�ȊO�͎w���
	RENZOKU_NUM_INIT
	@�_�u���j�[�h�����g���Ƃ��̃��[�N�ɒl�������Ă���̂ŁA���������Ă����i�O�̂��߁j
	SETVALUE			UserWork+SERVER_WORK3_TEMP,0
	JUMP				WAZA_RenzokuLoop

@===================================================
@	045 �U�����O���ƁA�^����͂��̃_���[�W�̂P�^�Q���������󂯂�
@===================================================

WAZA_TobigeriHit:
	BADGECHECK
	HITCHECK			WAZA_TobigeriNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	JUMP				WAZA_HitCalc
WAZA_TobigeriNohit:
	ATTACK_MESSAGE
	PP_DEC
	WAIT				MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_KOUKANAI,WAZA_WazaEnd
	MESSAGE				TOBIGERIMISSMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	DAMAGE_CALC
	TYPE_CHECK
	DAMAGE_LOSS
	DAMAGE_CHG			DAMAGE_HALF
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_NOHIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_NOHIT
	JUMP				WAZA_WazaEnd

@===================================================
@	046 �X�e�[�^�X��ω�������⏕�U�����󂯂Ȃ��Ȃ�
@===================================================

WAZA_ShiroiKiriHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SHIROIKIRI
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			ShiroikiriMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	047 �N���e�B�J�������A�b�v����
@===================================================

WAZA_KiaidameHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KIAIDAME,WAZA_Umakukimaran
	KIAIDAME
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			KiaidameMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	048 �^�����_���[�W�̂P�^�S�������ɂ͂˂������Ă���
@===================================================

WAZA_HanekaeriHit:
	SETVALUE			ServerWork+3,COND2_HANEKAERI+TSUIKA_ALWAYS+TSUIKA_ATTACK
	IF16				FLAG_NE,WazaNo,WAZANO_WARUAGAKI,WAZA_NormalHit
	INC_RECORD			CNT_WAZA_WARUAGAKI	@��邠�����������񐔂�CountUp
	JUMP				WAZA_NormalHit

@===================================================
@	049 ���������������
@===================================================

WAZA_KonranDirectHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_MAIPEESU,WAZA_Mypace
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_KONRAN,WAZA_KonranAlready
	HITCHECK			WAZA_Umakukimaran,0
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			ServerWork+3,COND2_KONRAN
	TSUIKA_DIRECT_ACT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd
WAZA_KonranAlready:
	WAIT				MOJI_WAIT/2
	MESSAGE				KONRANMSG5NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	050 �����̍U���͂��Q�����N�グ��
@===================================================

WAZA_PowUp2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKU2
	JUMP				WAZA_CondUp

@===================================================
@	051 �����̖h��͂��Q�����N�グ��
@===================================================

WAZA_DefUp2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKU2
	JUMP				WAZA_CondUp

@===================================================
@	052 �����̑f�������Q�����N�グ��
@===================================================

WAZA_AgiUp2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_AGI+PARA_RANKU2
	JUMP				WAZA_CondUp

@===================================================
@	053 �����̓���U���͂��Q�����N�グ��
@===================================================

WAZA_SpePowUp2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEPOW+PARA_RANKU2
	JUMP				WAZA_CondUp

@===================================================
@	054 �����̓���h��͂��Q�����N�グ��
@===================================================

WAZA_SpeDefUp2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEDEF+PARA_RANKU2
	JUMP				WAZA_CondUp

@===================================================
@	057 �ւ񂵂�
@===================================================

WAZA_HenshinHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HENSHIN
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			HenshinMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	058 ����̍U���͂��Q�����N������
@===================================================

WAZA_PowDown2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKD2
	JUMP				WAZA_CondDown

@===================================================
@	059 ����̖h��͂��Q�����N������
@===================================================

WAZA_DefDown2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKD2
	JUMP				WAZA_CondDown

@===================================================
@	060 ����̑f�������Q�����N������
@===================================================

WAZA_AgiDown2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_AGI+PARA_RANKD2
	JUMP				WAZA_CondDown

@===================================================
@	062 ����̓���h��͂��Q�����N������
@===================================================

WAZA_SpeDefDown2Hit:
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEDEF+PARA_RANKD2
	JUMP				WAZA_CondDown

@===================================================
@	065 �����U���Ŏ󂯂�_���[�W���P�^�Q�ɂȂ�
@===================================================

WAZA_ReflecHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	REFLEC				
WAZA_Kabe:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			ReflecMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	066 ����ɓł�^����
@===================================================

WAZA_DokuDirectHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_MENEKI,WAZA_MenekiJump
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_DOKU,WAZA_DokudokuAlready
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_DOKUDOKU,WAZA_DokudokuAlready
	CHECK_TYPE			SIDE_DEFENCE,POISON_TYPE,WAZA_Koukanai
	CHECK_TYPE			SIDE_DEFENCE,METAL_TYPE,WAZA_Koukanai
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_BAD,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,0
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			ServerWork+3,COND_DOKU
	TSUIKA_DIRECT_ACT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	067 ������܂Ђ�����
@===================================================

WAZA_MahiDirectHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_ZYUUNAN,WAZA_JuunanJump
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	TYPE_CHECK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_Umakukimaran
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_MAHI,WAZA_MahiAlready
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_BAD,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,0
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			ServerWork+3,COND_MAHI
	TSUIKA_DIRECT_ACT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_MahiAlready:
	WAIT				MOJI_WAIT/2
	MESSAGE				MAHIMSG3NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_JuunanJump:
	SETWORK				TsuikaClient,DefenceClient,1
	SETVALUE			ServerWork+5,0
	GOSUB				WAZA_Juunan
	JUMP				WAZA_WazaEnd

@===================================================
@	068 �ǉ����ʂő���̍U���͂��P�����N������
@===================================================

WAZA_PowDownAddHit:
	SETVALUE			ServerWork+3,COND2_POWDOWN
	JUMP				WAZA_NormalHit

@===================================================
@	069 �ǉ����ʂő���̖h��͂��P�����N������
@===================================================

WAZA_DefDownAddHit:
	SETVALUE			ServerWork+3,COND2_DEFDOWN
	JUMP				WAZA_NormalHit

@===================================================
@	070 �ǉ����ʂő���̑f�������P�����N������
@===================================================

WAZA_AgiDownAddHit:
	SETVALUE			ServerWork+3,COND2_AGIDOWN
	JUMP				WAZA_NormalHit

@===================================================
@	071 �ǉ����ʂő���̓���U���͂��P�����N������
@===================================================

WAZA_SpePowDownAddHit:
	SETVALUE			ServerWork+3,COND2_SPEPOWDOWN
	JUMP				WAZA_NormalHit

@===================================================
@	072 �ǉ����ʂő���̓���h��͂��P�����N������
@===================================================

WAZA_SpeDefDownAddHit:
	SETVALUE			ServerWork+3,COND2_SPEDEFDOWN
	JUMP				WAZA_NormalHit

@===================================================
@	073 �ǉ����ʂő���̖��������P�����N������
@===================================================

WAZA_HitDownAddHit:
	SETVALUE			ServerWork+3,COND2_HITDOWN
	JUMP				WAZA_NormalHit

@===================================================
@	075 ���߁{�����̒ǉ�����
@===================================================

WAZA_GodBirdHit:
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KEEP,WAZA_TameAftar
	IF32				FLAG_BIT,ServerStatusFlag,SERVER_STATUS_FLAG_DOUBLE_CHECK,WAZA_TameAftar
	SETVALUE			UserWork+TAME_MSG_NO,3		@�S�b�h�o�[�h�̃��b�Z�[�W�i���o�[
	GOSUB				WAZA_TameBefore
	JUMP				WAZA_WazaEnd

@===================================================
@	076 �ǉ����ʂő��������������
@===================================================

WAZA_KonranAddHit:
	SETVALUE			ServerWork+3,COND2_KONRAN
	JUMP				WAZA_NormalHit

@===================================================
@	077 �A���łQ��U�����A�ǉ����ʂő�����ǂ���Ԃɂ���
@===================================================

WAZA_RenzokuDokuAddHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
@	SETVALUE			ServerWork+3,COND_DOKU
	SETVALUE			UserWork+SERVER_WORK3_TEMP,COND_DOKU
	ATTACK_MESSAGE
	PP_DEC
	RENZOKU				2					@0�ȊO�͎w���
	RENZOKU_NUM_INIT
	JUMP				WAZA_RenzokuLoop

@===================================================
@	079 �݂������o��
@===================================================

WAZA_MigawariHit:
	BADGECHECK
	PP_DEC
	ATTACK_MESSAGE
	CLIENT_BIT_WAIT
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_MIGAWARI,WAZA_MigawariAlready
	MIGAWARI
	IF8					FLAG_NE,ServerWork+5,1,WAZA_MigawariEff
	WAIT				MOJI_WAIT/2
	JUMP				WAZA_MigawariNoEff
WAZA_MigawariEff:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
WAZA_MigawariNoEff:
	MESSAGE_TBL			MigawariMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd
WAZA_MigawariAlready:
	WAIT				MOJI_WAIT/2
	MESSAGE				MIGAWARIALREADYMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	080 �U���̎��̂P�^�[���s���s�\�ɂȂ�
@===================================================

WAZA_HandouHit:
@	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_HANDOU,WAZA_Handou
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	SETVALUE			ServerWork+3,COND2_HANDOU+TSUIKA_ALWAYS+TSUIKA_ATTACK
@	TSUIKA_DIRECT_ACT
@	SETVALUE			ServerWork+3,0
	JUMP				WAZA_HitCalc

WAZA_Handou:
	MESSAGE				HANDOUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	081 ������
@===================================================

WAZA_IkariHit:
	BADGECHECK
	HITCHECK			WAZA_IkariNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	SETVALUE			ServerWork+3,COND2_IKARI
	TSUIKA_DIRECT_ACT
	SETVALUE			ServerWork+3,0
	JUMP				WAZA_HitCalc
WAZA_IkariNohit:
	SETVALUE			ServerWork+3,COND2_IKARI
	SAKUJO				SIDE_ATTACK
	JUMP				WAZA_NormalNohit	

@===================================================
@	082 ���̂܂�
@===================================================

WAZA_MonomaneHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
@	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_KIE,FLAG_ON,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,HITCHECK_KIE
	MONOMANE			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				MONOMANEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	083 ��т��ӂ�
@===================================================

WAZA_YubiwohuruHit:
	BADGECHECK
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+WAZA_EFF_CNT,0
	SETVALUE			UserWork+WAZA_EFF_CNT_TMP,0
	YUBIWOHURU

@===================================================
@	084 ��ǂ肬
@===================================================

WAZA_YadorigiHit:
	BADGECHECK
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	HITCHECK			YadorigiNext,0
YadorigiNext:
	YADORIGI		
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			YadorigiMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	085 ����������Ȃ�
@===================================================

WAZA_HaneruHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	INC_RECORD			CNT_WAZA_HANERU		@�͂˂�������񐔂�CountUp
	MESSAGE				NANIMOOKORANMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	086 ���肪���O�ɏo�����Z�𕕂���
@===================================================

WAZA_KanashibariHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
@	GUARDCHECK			WAZA_Umakukimaran			@�܂���A�݂���`�F�b�N
	HITCHECK			WAZA_Umakukimaran,0
	KANASHIBARI			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				KANASHIBARIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	087 �����̃��x�����̃_���[�W��^����
@===================================================

WAZA_ChikyuuNageHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_BATSUGUN|WAZASTATUSFLAG_IMAHITOTSU
	CHIKYUUNAGE
	KORAERU_CHECK
	JUMP				WAZA_Hit

@===================================================
@	088 �P�`�����̃��x�����̃_���[�W��^����
@===================================================

WAZA_PsychoWaveHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_BATSUGUN|WAZASTATUSFLAG_IMAHITOTSU
	PSYCHOWAVE
	KORAERU_CHECK
	JUMP				WAZA_Hit

@===================================================
@	089 ����̕����U���̃_���[�W��{�ɂ��ĕԂ�
@===================================================

WAZA_CounterHit:
.if 0
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	COUNTER				WAZA_UmakukimaranPP
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK_NO_HIT
	KORAERU_CHECK
	JUMP				WAZA_Hit
.else
	BADGECHECK
	COUNTER				WAZA_UmakukimaranPP
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK_NO_HIT
	KORAERU_CHECK
	JUMP				WAZA_Hit
.endif

@===================================================
@	090 �A���R�[��
@===================================================

WAZA_EncoreHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	ENCORE				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				ENCOREMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	091 �����݂킯
@===================================================

WAZA_ItamiwakeHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
@	GUARDCHECK			WAZA_Umakukimaran			@�܂���A�݂���`�F�b�N
@	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_KIE,FLAG_ON,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,HITCHECK_KIE
	ITAMIWAKE			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	SETWORK				WazaDamage,UserWork+ITAMIWAKE_WORK,4
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	MESSAGE				ITAMIWAKEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	092 ���т�
@===================================================

WAZA_IbikiHit:
	BADGECHECK
	CONDITION_CHECK		SIDE_ATTACK,CONDITION_NEMURI,WAZA_Ibiki
	ATTACK_MESSAGE
	PP_DEC
	JUMP				WAZA_Umakukimaran
WAZA_Ibiki:
	@�˂��Ƃ��甭���������т��ł́u���������˂Ă���v���o���Ȃ�
	IF16				FLAG_EQ,WazaNoTmp,WAZANO_NEGOTO,WAZA_IbikiNoMsg
	MESSAGE				SLEEPMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT		SIDE_ATTACK
WAZA_IbikiNoMsg:
	ATTACK_MESSAGE
	PP_DEC
	HITCHECK			WAZA_NormalNohit2,0
	SETVALUE			ServerWork+3,COND2_HIRUMU
	JUMP				WAZA_HitCalcOnly

@===================================================
@	093 �����̃^�C�v�������_���ŕς��Ă��܂�
@===================================================

WAZA_Texture2Hit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	TEXTURE2			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				TEXTUREMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	094 ���̎����̍U���̖��������P�O�O���ɂ���
@===================================================

WAZA_LockOnHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,0
	LOCKON
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				LOCKONMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	095 �X�P�b�`
@===================================================

WAZA_SketchHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	SKETCH				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				SKETCHMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	097 �����������Ԃ̎��A���̂R�̋Z�̂P�������_���ŏo��
@===================================================

WAZA_NegotoHit:
	BADGECHECK
	CONDITION_CHECK		SIDE_ATTACK,CONDITION_NEMURI,WAZA_Negoto
	ATTACK_MESSAGE
WAZA_UmakukimaranJump:
	PP_DEC
	JUMP				WAZA_Umakukimaran
WAZA_Negoto:
	MESSAGE				SLEEPMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT		SIDE_ATTACK
	ATTACK_MESSAGE
	PP_DEC
	@�˂��Ƃ��甭�������Z��PP��Dec���Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NOPP_DEC
	NEGOTO				WAZA_NegotoOK
	WAIT				MOJI_WAIT
	JUMP				WAZA_Umakukimaran
WAZA_NegotoOK:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+WAZA_EFF_CNT,0
	SETVALUE			UserWork+WAZA_EFF_CNT_TMP,0
	WAZASEQADRS			1

@===================================================
@	098 ���̑���̍U���Ŏ������Ђ񂵂ɂȂ�ƁA������Ђ񂵂ɂȂ�
@===================================================

WAZA_MichidureHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	MICHIDURE
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				MICHIDUREMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	099 �����̗̑͂����Ȃ��قǍU���͂��オ��
@===================================================

WAZA_JitabataHit:
	JITABATA
	JUMP				WAZA_NormalHit

@===================================================
@	100 ����̂o�o���Q�`�T���
@===================================================

WAZA_UramiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
@	GUARDCHECK			WAZA_Umakukimaran			@�܂���A�݂���`�F�b�N
	HITCHECK			WAZA_Umakukimaran,0
	URAMI				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				URAMIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	102 �����̏�ԕω��𒼂�
@===================================================

WAZA_IyashinosuzuHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	IYASHINOSUZU
	CLIENT_BIT_WAIT
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			IyashiNoSuzuMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	IF16				FLAG_NE,WazaNo,WAZANO_IYASINOSUZU,WAZA_IyashinosuzuNext
WAZA_IyashinosuzuBouon:
	IF8					FLAG_NBIT,ServerWork+5,1,WAZA_IyashinosuzuBouonNext
	MESSAGE				BOUONMSGNO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_IyashinosuzuBouonNext:
	IF8					FLAG_NBIT,ServerWork+5,2,WAZA_IyashinosuzuNext
	MESSAGE				BOUONMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_IyashinosuzuNext:
	CONDITION_GAUGE		SIDE_ATTACK_DOUBLE
	CLIENT_BIT_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	104 �g���v���L�b�N�B�P�`�R�񓖂���B�_���[�W��1���Q���R
@===================================================

WAZA_TripleKickHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SETVALUE			UserWork+TRIPLE_KICK_POW+0,0
	SETVALUE			UserWork+TRIPLE_KICK_POW+1,0
	RENZOKU_NUM_INIT
	LOOPCNT				3
WAZA_TripleKickLoop:
	HP_CHECK			SIDE_ATTACK,WAZA_TripleKickNoMsg
	HP_CHECK			SIDE_DEFENCE,WAZA_TripleKickNohitWait
	IF16				FLAG_EQ,WazaNoTmp,WAZANO_NEGOTO,WAZA_TripleKickStart
	CONDITION_CHECK		SIDE_ATTACK,CONDITION_NEMURI,WAZA_TripleKickNohitWait
WAZA_TripleKickStart:
@	SETVALUE			WazaStatusFlag,0
	LOOPINIT			@���[�v����Ƃ��ɏ��������郏�[�N���܂Ƃ܂��Ă���
	HITCHECK			WAZA_TripleKickNohitWait,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	ADDVALUE			UserWork+TRIPLE_KICK_POW+0,10
@	ADDVALUE			EvStrBuf0+4,1
	ADDVALUE			UserWork+RENZOKU_COUNT+4,1
	SETWORK				DamagePowWork,UserWork+TRIPLE_KICK_POW+0,2
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	DAMAGE_LOSS
@	KORAERU_CHECK		@���������������Ȃ��Ƃ��߂���
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_TripleKickNohitWait
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	MESSAGE				BLANKMSGNO
	MESSAGE_WAIT		1
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_KORAETA,WAZA_TripleKickNohit
	LOOP				WAZA_TripleKickLoop
	JUMP				WAZA_TripleKickNohit
WAZA_TripleKickNohitWait:
	WAIT				MOJI_WAIT/2
WAZA_TripleKickNohit:
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
@	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_RenzokuNoMsg
	IF8					FLAG_EQ,UserWork+RENZOKU_COUNT+4,0,WAZA_TripleKickNoMsg
	SETWORK				EvStrBuf0,UserWork+RENZOKU_COUNT,6
	MESSAGE				RENZOKUHITMSGNO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_TripleKickNoMsg:
	TSUIKA_CHECK
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	SETVALUE			UserWork+WKC_WORK0,WKC_HIT_FLAG_SET
	WKC_ACT				WKC_CHECK_ALL,0
	WAZAEND

@===================================================
@	105 ���肪�������Ă���ǂ����𓐂�
@===================================================

WAZA_DorobouHit:
	SETVALUE			ServerWork+3,COND2_DOROBOU
	JUMP				WAZA_NormalHit

@===================================================
@	106 ����𓦂����Ȃ�����B
@===================================================

WAZA_KuroimanazashiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
@	GUARDCHECK			WAZA_Umakukimaran			@�܂���A�݂���`�F�b�N
	HITCHECK			WAZA_Umakukimaran,HITCHECK_GUARD
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_KUROIMANAZASHI,WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			ServerWork+3,COND2_KUROIMANAZASHI
	TSUIKA_DIRECT_ACT
	MESSAGE				KUROIMANAZASHIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	107 ����
@===================================================

WAZA_AkumuHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_AKUMU,WAZA_Umakukimaran
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_NEMURI,WAZA_AddAkumu
	JUMP				WAZA_Umakukimaran
WAZA_AddAkumu:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			ServerWork+3,COND2_AKUMU
	TSUIKA_DIRECT_ACT
	MESSAGE				AKUMUMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	108 ���������Ȃ�
@===================================================

WAZA_ChiisakunaruHit:
	BADGECHECK
	CHIISAKUNARU
	SETVALUE			UserWork+COND_CHG_WORK,COND_AVOID+PARA_RANKU1
	JUMP				WAZA_CondUp2

@===================================================
@	109 �̂낢
@===================================================

WAZA_NoroiHit:
	MONS_TYPE			SIDE_ATTACK,GHOST_TYPE,WAZA_GhostNoroi
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDCHG_CHECK		SIDE_ATTACK,IFC,COND_AGI,0,WAZA_NoroiStart
	CONDCHG_CHECK		SIDE_ATTACK,IFNE,COND_POW,12,WAZA_NoroiStart
	CONDCHG_CHECK		SIDE_ATTACK,IFEQ,COND_DEF,12,WAZA_Umakukimaran
WAZA_NoroiStart:
	SETWORK				DefenceClient,AttackClient,1
	SETVALUE			UserWork+WAZA_EFF_CNT,1	@�m�[�}���p�̃G�t�F�N�g�i���o�[
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+COND_CHG_WORK,COND_AGI+PARA_RANKD1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_NoroiNoAgiDown
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_NoroiNoAgiDown:
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_NoroiNoPowUp
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_NoroiNoPowUp:
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_NoroiNoDefUp
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_NoroiNoDefUp:
	JUMP				WAZA_WazaEnd
WAZA_GhostNoroi:
	IFWNE				AttackClient,DefenceClient,1,WAZA_GhostNoroiStart
	SUBROUTINE_CALL		SIDE_ATTACK,DEFENCE_CLIENT_GET
WAZA_GhostNoroiStart:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	NOROI				WAZA_Umakukimaran
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	SETVALUE			UserWork+WAZA_EFF_CNT,0	@�S�[�X�g�p�̃G�t�F�N�g�i���o�[
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	MESSAGE				NOROIMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	JUMP				WAZA_WazaEnd

@===================================================
@	111 ���̑���̍U�����󂯂Ȃ�
@===================================================

@===================================================
@	116 ���̑���̍U�����󂯂Ă��̗͂��P�c��
@===================================================

WAZA_MamoruHit:
WAZA_KoraeruHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	GUARD_SUCCESS_CHECK
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			MamoruMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	112 �܂��т�
@===================================================

WAZA_MakibishiHit:
	BADGECHECK
	MAKIBISHI			WAZA_UmakukimaranPP
	ATTACK_MESSAGE
	PP_DEC
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				MAKIBISHIMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	113 �݂�Ԃ�
@===================================================

WAZA_MiyaburuHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	MIYABURU
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				MIYABURUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	114 �ق�т̂���
@===================================================

WAZA_HorobinoutaHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HOROBINOUTA			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				HOROBINOUTAMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	SETVALUE			UserWork+CLIENT_NO_WORK,0
WAZA_HorobinoutaLoop:
	SPEABI_CHECK		SIDE_CLIENTNOWORK,TOKUSYU_BOUON,WAZA_HorobinoutaBouon
WAZA_HorobinoutaNext:
	ADDVALUE			UserWork+CLIENT_NO_WORK,1
	IFWNE				UserWork+CLIENT_NO_WORK,ClientSetMax,1,WAZA_HorobinoutaLoop
	JUMP				WAZA_WazaEnd

WAZA_HorobinoutaBouon:
	MESSAGE				BOUONMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_HorobinoutaNext

@===================================================
@	115 ���Ȃ��炵
@===================================================

WAZA_SunaarashiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SUNAARASHI
	JUMP				WAZA_Tenkou

@===================================================
@	117 �T�^�[���A���ōU���A�^�[�����ƂɈЗ͂��オ��
@===================================================

WAZA_KorogaruHit:
	BADGECHECK
	ATTACK_MESSAGE
@���낪���Ă���Ԃ�PP��DEC���Ȃ�
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KEEP,WAZA_KorogaruNoPPDec
	PP_DEC
WAZA_KorogaruNoPPDec:
	HITCHECK			WAZA_Korogaru,0
WAZA_Korogaru:
	TYPE_CHECK_NO_HIT
	KOROGARU
	JUMP				WAZA_HitCalcOnly

@===================================================
@	118 ����̍U���͂��Q�i�K�グ�Ă����񂳂���
@===================================================

WAZA_IbaruHit:
	BADGECHECK
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_NormalNohitBitSet
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	IBARU				COND_POW,WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKU2
	CONDCHG				TSUIKA_DIRECT,WAZA_IbaruNoMsg
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_IbaruNoMsg
	COND_CHG_WORK_SET
	FIGHT_EFFECT		SIDE_DEFENCE,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_IbaruNoMsg:
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_MAIPEESU,WAZA_Mypace
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	SETVALUE			ServerWork+3,COND2_KONRAN
	TSUIKA_DIRECT_ACT
	JUMP				WAZA_WazaEnd

@===================================================
@	119 �A���ŏo�����ƂɈЗ͂��オ��
@===================================================

WAZA_RenzokugiriHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HITCHECK			WAZA_Renzokugiri,0
WAZA_Renzokugiri:
	RENZOKUGIRI
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_Renzokugiri
	DAMAGE_LOSS
	JUMP				WAZA_Hit

@===================================================
@	120 ���������������Ԃɂ���
@===================================================

WAZA_MeromeroHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
@	GUARDCHECK			WAZA_Umakukimaran			@�܂���A�݂���`�F�b�N
	HITCHECK			WAZA_Umakukimaran,0
	MEROMERO			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				MEROMEROMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	121 �Ȃ��x�������قǈЗ͂�����
@	123 �Ȃ��x���Ⴂ�قǈЗ͂�����
@===================================================

WAZA_NatsukiHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	NATSUKI
	JUMP				WAZA_HitCalc

@===================================================
@	122 �v���[���g
@===================================================

WAZA_PresentHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK
	PRESENT				

@===================================================
@	124 �T�^�[���̊ԁA��ԕω��i�ǂ��A�܂Ёj���󂯂Ȃ�
@===================================================

WAZA_ShinpinomamoriHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SHINPINOMAMORI		
	JUMP				WAZA_Kabe

@===================================================
@	125 �₯�ǂ̒ǉ����ʁ{�������Ԃ̏ꍇ�͂����肪�n����
@===================================================

WAZA_KaengurumaHit:
@	SETVALUE			ServerWork+3,COND2_KAENGURUMA+TSUIKA_ALWAYS
	SETVALUE			ServerWork+3,COND_YAKEDO
	JUMP				WAZA_NormalHit

@===================================================
@	126 �}�O�j�`���[�h
@===================================================

WAZA_MagnitudeHit:
.if 0
	BADGECHECK
	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_ANAWOHORU,FLAG_OFF,WAZA_Magnitude
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_ANAWOHORUHIT
	SETVALUE			UserWork+DAMAGE_VALUE,2
WAZA_Magnitude:
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	PP_DEC
	MAGNITUDE		
	MESSAGE				MAGNITUDEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_HitCalcOnly
.endif
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	JISIN
	MAGNITUDE		
	WAIT				MOJI_WAIT/2
	MESSAGE				MAGNITUDEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_JisinLoop

@===================================================
@	127 �o�g���^�b�`
@===================================================

WAZA_BatontouchHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	RESHUFFLE_CHECK		SIDE_ATTACK+0x80,WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	POKE_RESHUFFLE		SIDE_ATTACK,WAZA_Umakukimaran
	SIZENKAIHUKU		SIDE_ATTACK
	CLIENT_BIT_WAIT
	POKE_RESHUFFLE_WAIT	SIDE_ATTACK,PRW_NORMAL
	POKEDELACTOR		SIDE_ATTACK
	APPEAR_EFFECT_INIT	SIDE_ATTACK
	APPEAR_EFFECT		SIDE_ATTACK
@	MESSAGE_TBL			PokeAppearMsgTbl
	POKE_HPMAX_RATE		SIDE_ATTACK
	MESSAGE				IREKAEAFTERMSGNO
@	WAIT				MOJI_WAIT/2
	APPEAR_EFFECT_WAIT	SIDE_ATTACK,APPEAR_BATONTOUCH
	CLIENT_BIT_WAIT
	POKE_RESHUFFLE_END	SIDE_ATTACK
	JUMP				WAZA_WazaEnd

@===================================================
@	129 ���������X�s��
@===================================================

WAZA_KousokuSpinHit:
	SETVALUE			ServerWork+3,COND2_KOUSOKUSPIN+TSUIKA_ALWAYS
	JUMP				WAZA_NormalHit

@===================================================
@	130 �Œ�_���[�W20
@===================================================

WAZA_Kotei20Hit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_BATSUGUN|WAZASTATUSFLAG_IMAHITOTSU
	SETVALUE			WazaDamage+0,20
	SETVALUE			WazaDamage+1,0
	SETVALUE			WazaDamage+2,0
	SETVALUE			WazaDamage+3,0
	KORAERU_CHECK
	JUMP				WAZA_Hit

@===================================================
@	132 �̗͂𔼕��񕜁B�����ƑS��
@	133 �̗͂𔼕��񕜁B�����ƑS��
@	134 �̗͂𔼕��񕜁B��A�[�邾�ƑS��
@===================================================

WAZA_WeatherkaifukuHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	WEATHERKAIFUKU		WAZA_Mantan
	JUMP				WAZA_Kaifuku

@===================================================
@	135 �߂��߂�p���[
@===================================================

WAZA_MezameruPowerHit:
	MEZAMERUPOWER
	JUMP				WAZA_NormalHit

@===================================================
@	136 ���܂���
@===================================================

WAZA_AmagoiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	AMAGOI
WAZA_Tenkou:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			TenkouMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	GOSUB				WAZA_TenkiyaCheck
	JUMP				WAZA_WazaEnd

@===================================================
@	137 �ɂق�΂�
@===================================================

WAZA_NihonbareHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	NIHONBARE
	JUMP				WAZA_Tenkou

@===================================================
@	138 �ǉ����ʂŎ����̖h��͂��P�i�K�グ��
@===================================================

WAZA_DefUpAddHit:
	SETVALUE			ServerWork+3,COND2_DEFUP+TSUIKA_ATTACK
	JUMP				WAZA_NormalHit

@===================================================
@	139 �ǉ����ʂŎ����̍U���͂��P�i�K�グ��
@===================================================

WAZA_PowUpAddHit:
	SETVALUE			ServerWork+3,COND2_POWUP+TSUIKA_ATTACK
	JUMP				WAZA_NormalHit

@===================================================
@	140 �ǉ����ʂłT�̃X�e�[�^�X���P�i�K�オ��
@===================================================

WAZA_GenshinochikaraHit:
	SETVALUE			ServerWork+3,COND2_GENSHINOCHIKARA+TSUIKA_ATTACK
	JUMP				WAZA_NormalHit

@===================================================
@	142 �͂炾����
@===================================================

WAZA_HaradaikoHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HARADAIKO			WAZA_Umakukimaran
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	MESSAGE				HARADAIKOMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	143 ����ɂ������Ă���⏕���ʂ��R�s�[����
@===================================================

WAZA_JikoanjiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	JIKOANJI			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				JIKOANJIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	144 ����̓���U���̃_���[�W��{�ɂ��ĕԂ�
@===================================================

WAZA_MirrorCoatHit:
.if 0
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	MIRRORCOAT			WAZA_UmakukimaranPP
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK_NO_HIT
	KORAERU_CHECK
	JUMP				WAZA_Hit
.else
	BADGECHECK
	MIRRORCOAT			WAZA_UmakukimaranPP
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	TYPE_CHECK_NO_HIT
	KORAERU_CHECK
	JUMP				WAZA_Hit
.endif

@===================================================
@	145  ���߁{�h��̓A�b�v
@===================================================

WAZA_RocketzutukiHit:
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KEEP,WAZA_TameAftar
	IF32				FLAG_BIT,ServerStatusFlag,SERVER_STATUS_FLAG_DOUBLE_CHECK,WAZA_TameAftar
	SETVALUE			UserWork+TAME_MSG_NO,2		@���P�b�g�����̃��b�Z�[�W�i���o�[
	GOSUB				WAZA_TameBefore
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_RocketzutukiEnd
@������Ȃ����b�Z�[�W�͏o���Ȃ�
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_RocketzutukiEnd
	COND_CHG_WORK_SET
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_RocketzutukiEnd:
	JUMP				WAZA_WazaEnd

@===================================================
@	146 �Ђ�ށ@�{�@����Ƃ�ł��鑊��ɔ{�̃_���[�W
@===================================================

WAZA_TatumakiHit:
	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_SORAWOTOBU,FLAG_OFF,WAZA_Tatumaki
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_SORAWOTOBUHIT
	SETVALUE			UserWork+DAMAGE_VALUE,2
WAZA_Tatumaki:
	SETVALUE			ServerWork+3,COND2_HIRUMU
	JUMP				WAZA_NormalHit

@===================================================
@	147 �n�ʂɐ����Ă��鑊��ɔ{�̃_���[�W
@===================================================

WAZA_JisinHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	JISIN
WAZA_JisinLoop:
	LOOPINIT			@���[�v����Ƃ��ɏ��������郏�[�N���܂Ƃ܂��Ă���
	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_ANAWOHORU,FLAG_OFF,WAZA_JisinNext
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_ANAWOHORUHIT
	SETVALUE			UserWork+DAMAGE_VALUE,2
	JUMP				WAZA_Jisin
WAZA_JisinNext:
	RESET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_ANAWOHORUHIT
	SETVALUE			UserWork+DAMAGE_VALUE,1
WAZA_Jisin:
@	SETVALUE			WazaStatusFlag,0
	HITCHECK			WAZA_JisinNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	DAMAGE_LOSS
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	MESSAGE				BLANKMSGNO
	MESSAGE_WAIT		1
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	JIBAKU_LOOP			WAZA_JisinLoop
	WAZAEND
WAZA_JisinNohit:
	WAIT				MOJI_WAIT/2
	TYPE_CHECK
	WAZAKOUKA_SE
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	JIBAKU_LOOP			WAZA_JisinLoop
	WAZAEND

@===================================================
@	148 �݂炢�悿�@�R�^�[����ɍU��������
@===================================================

WAZA_MiraiyochiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
@	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_MIRAIYOCHI,WAZA_Umakukimaran
@	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_HENSYOKU_NOHIT
@	MIRAIYOCHI
	MIRAIYOCHI			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			MiraiyochiMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	149 ����Ƃ�ł��鑊��ɔ{�̃_���[�W
@===================================================

WAZA_KazeokosiHit:
	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_SORAWOTOBU,FLAG_OFF,WAZA_NormalHit
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_SORAWOTOBUHIT
	SETVALUE			UserWork+DAMAGE_VALUE,2
	JUMP				WAZA_NormalHit

@===================================================
@	150 �������Ȃ��Ă��鑊��ɔ{�̃_���[�W
@===================================================

WAZA_HumitukeHit:
@WazaKouka�̃t���O�������ĂȂ������Ƃ��̔�ѐ���w��
	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_CHIISAKUNARU,FLAG_OFF,WAZA_Tatumaki
	SETVALUE			UserWork+DAMAGE_VALUE,2
	JUMP				WAZA_Tatumaki

@===================================================
@	151 �\�[���[�r�[���@�͂ꂾ�Ƃ��߂��ɍU���ł���
@===================================================

WAZA_SolarBeamHit:
	SPEABI_HAVE_HP_CHECK	TOKUSYU_NOOTENKI,WAZA_SolarBeamNoHareCheck
	SPEABI_HAVE_HP_CHECK	TOKUSYU_EAROKKU,WAZA_SolarBeamNoHareCheck
	IF16				FLAG_BIT,FieldCondition,FIELD_CONDITION_HIDERING,WAZA_TameAftar2
WAZA_SolarBeamNoHareCheck:
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KEEP,WAZA_TameAftar
	IF32				FLAG_BIT,ServerStatusFlag,SERVER_STATUS_FLAG_DOUBLE_CHECK,WAZA_TameAftar
	SETVALUE			UserWork+TAME_MSG_NO,1		@�\�[���[�r�[���̃��b�Z�[�W�i���o�[
	GOSUB				WAZA_TameBefore
	JUMP				WAZA_WazaEnd
WAZA_TameAftar2:
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_TAME
	SETVALUE			ServerWork+3,COND2_KEEP+TSUIKA_ATTACK
	TSUIKA_DIRECT_ACT
	PP_DEC
	JUMP				WAZA_TameAftar

@===================================================
@	152 �J���Ɩ������A�b�v�@�͂ꂾ�ƃ_�E���i���݂Ȃ�j
@===================================================

WAZA_KaminariHit:
	SETVALUE			ServerWork+3,COND_MAHI
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_SORAWOTOBUHIT
	JUMP				WAZA_NormalHit

@===================================================
@	153 �e���|�[�g�i�쐫�̃|�P�����Ƃ̐퓬���痣�E�j
@===================================================

.if 0
WAZA_TeleportHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_TRAINER,WAZA_Umakukimaran
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_SHIME,WAZA_Umakukimaran
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KUROIMANAZASHI,WAZA_Umakukimaran
	WAZAKOUKA_CHECK		SIDE_ATTACK,WAZAKOUKA_NEWOHARU,FLAG_ON,WAZA_Umakukimaran
	SPEABI_CHECK		SIDE_ENEMY_BENCH,TOKUSYU_KAGEHUMI,WAZA_SpeabiUmakukimaran
	SPEABI_CHECK		SIDE_ATTACK,TOKUSYU_HUYUU,WAZA_TeleportNoCheckArizigoku
	CHECK_TYPE			SIDE_ATTACK,HIKOU_TYPE,WAZA_TeleportNoCheckArizigoku
	SPEABI_CHECK		SIDE_ENEMY_BENCH,TOKUSYU_ARIZIGOKU,WAZA_SpeabiUmakukimaran
WAZA_TeleportNoCheckArizigoku:
	CHECK_TYPE			SIDE_ATTACK,METAL_TYPE,WAZA_TeleportZiryoku
	JUMP				WAZA_TeleportStart
WAZA_TeleportZiryoku:
	SPEABI_CHECK		SIDE_ENEMY_BENCH,TOKUSYU_ZIRYOKU,WAZA_SpeabiUmakukimaran
WAZA_TeleportStart:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				TELEPORTMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SETVALUE			WinLoseFlag,FIGHT_END
	JUMP				WAZA_WazaEnd
.else
WAZA_TeleportHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_TRAINER,WAZA_Umakukimaran
	SUBROUTINE_CALL		SIDE_ATTACK,TELEPORT_CHECK
	IF8					FLAG_EQ,ServerWork+0,1,WAZA_Umakukimaran
	IF8					FLAG_EQ,ServerWork+0,2,WAZA_SpeabiUmakukimaran2
WAZA_TeleportStart:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				TELEPORTMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SETVALUE			WinLoseFlag,FIGHT_END
	JUMP				WAZA_WazaEnd
.endif

@===================================================
@	154 �ӂ��낾����
@===================================================

WAZA_HukurodatakiHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	PP_DEC
	SETVALUE			ServerWork,0
WAZA_Hukurodataki:
	LOOPINIT			@���[�v����Ƃ��ɏ��������郏�[�N���܂Ƃ܂��Ă���
	HUKURODATAKI		WAZA_HukurodatakiEnd,WAZA_Umakukimaran
	MESSAGE				HUKURODATAKIMSGNO
	CRITICAL_CHECK
	IF8					FLAG_NE,CriticalValue,2,WAZA_HukurodatakiNext
	DAMAGE_CHG			DAMAGE_BAI
WAZA_HukurodatakiNext:
	DAMAGE_LOSS
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	JUMP				WAZA_Hukurodataki
WAZA_HukurodatakiEnd:
	WAZAEND

@===================================================
@	155 ������ƂԁA���Ȃ��ق�
@===================================================

WAZA_KieruTameHit:
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KEEP,WAZA_KieruTameAftar
	IF32				FLAG_BIT,ServerStatusFlag,SERVER_STATUS_FLAG_DOUBLE_CHECK,WAZA_KieruTameAftar
	IF16				FLAG_EQ,WazaNo,WAZANO_SORAWOTOBU,WAZA_Sorawotobu
	IF16				FLAG_EQ,WazaNo,WAZANO_DAIBINGU,WAZA_Daibingu
	IF16				FLAG_EQ,WazaNo,WAZANO_TOBIHANERU,WAZA_Ukabu
	SETVALUE			UserWork+TAME_MSG_NO,5		@���Ȃ��ق�̃��b�Z�[�W�i���o�[
	JUMP				WAZA_KieruTame
WAZA_Ukabu:
	SETVALUE			UserWork+TAME_MSG_NO,7		@�����Ԃ̃��b�Z�[�W�i���o�[
	JUMP				WAZA_KieruTame
WAZA_Daibingu:
	SETVALUE			UserWork+TAME_MSG_NO,6		@�_�C�r���O�̃��b�Z�[�W�i���o�[
	JUMP				WAZA_KieruTame
WAZA_Sorawotobu:
	SETVALUE			UserWork+TAME_MSG_NO,4		@������ƂԂ̃��b�Z�[�W�i���o�[
WAZA_KieruTame:
	GOSUB				WAZA_TameBefore
	KIERUTAME
	JUMP				WAZA_WazaEnd
WAZA_KieruTameAftar:
	BADGECHECK
	SETVALUE			ServerWork+3,COND2_KEEP
	SETVALUE			UserWork+WAZA_EFF_CNT,1	@���ߌ�̋Z�G�t�F�N�g�Ăяo��
	SAKUJO				SIDE_ATTACK
@���߂����PP_DEC�����Ȃ�
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NOPP_DEC
@�Z�����Ԃ́A�܂Ђ̒ǉ�����
	IF16				FLAG_NE,WazaNo,WAZANO_TOBIHANERU,WAZA_KieruTameHitOk
	SETVALUE			ServerWork+3,COND_MAHI
WAZA_KieruTameHitOk:
	HITCHECK			WAZA_KieruTameNoHit,0
	KIERUTAME_AFTAR
	JUMP				WAZA_HitCalc
WAZA_KieruTameNoHit:
	KIERUTAME_AFTAR
	JUMP				WAZA_NormalNohit	

@===================================================
@	156 �܂邭�Ȃ�
@===================================================

WAZA_MarukunaruHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	MARUKUNARU
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_MarukunaruNoDefUp
@������Ȃ����b�Z�[�W�̂Ƃ��́A�Z�G�t�F�N�g�������Ȃ�
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_CondUpMsg
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
WAZA_MarukunaruNoDefUp:
	JUMP				WAZA_CondUpStatusEffect

@===================================================
@	157 �~���N�̂݁A�^�}�S����
@===================================================

WAZA_KaifukuHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	KAIFUKU				WAZA_Mantan,SIDE_DEFENCE
WAZA_Kaifuku:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	MESSAGE				JIKOSAISEIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd
WAZA_Mantan:
	WAIT				MOJI_WAIT/2
	MESSAGE				MANTANMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	158 �˂����܂�
@===================================================

WAZA_NekodamashiHit:
	BADGECHECK
	NEKODAMASHI			WAZA_NekodamashiNoHit
	SETVALUE			ServerWork+3,COND2_HIRUMU+TSUIKA_ALWAYS
	JUMP				WAZA_NormalHit
WAZA_NekodamashiNoHit:
WAZA_UmakukimaranPP:
	ATTACK_MESSAGE
WAZA_UmakukimaranPPDec:
	PP_DEC
WAZA_Umakukimaran:
	WAIT				MOJI_WAIT/2
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_UMAKUKIMARAN
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_Koukanai:
	WAIT				MOJI_WAIT/2
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_KOUKANAI
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	159 ���킮
@===================================================

WAZA_SawaguHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
	SETVALUE			ServerWork+3,COND2_SAWAGU+TSUIKA_ATTACK
@���킢�ł���Ԃ�PP��DEC���Ȃ�
	ATTACK_MESSAGE
	CONDITION2_CHECK	SIDE_ATTACK,CONDITION2_KEEP,WAZA_SawaguNoPPDec
	PP_DEC
WAZA_SawaguNoPPDec:
	SAWAGU
	JUMP				WAZA_HitCalcOnly

@===================================================
@	160 �������킦��
@===================================================

WAZA_TakuwaeruHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	TAKUWAERU
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			TakuwaeruMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	161 �͂�����
@===================================================

WAZA_HakidasuHit:
	BADGECHECK
	IF8					FLAG_EQ,ServerWork+6,MSG_MAMORUNOHIT,WAZA_HakidasuMamoru
	ATTACK_MESSAGE
	PP_DEC
	HITCHECK			WAZA_NormalNohit,0
	HAKIDASU			WAZA_HakidasuNoHit
	TYPE_CHECK
	KORAERU_CHECK
	JUMP				WAZA_Hit
WAZA_HakidasuNoHit:
	WAIT				MOJI_WAIT/2
	MESSAGE				HAKIDASUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_HakidasuMamoru:
	ATTACK_MESSAGE
	PP_DEC
	WAIT				MOJI_WAIT
	HAKIDASU			WAZA_HakidasuNoHit
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	162 �݂̂���
@===================================================

WAZA_NomikomuHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	NOMIKOMU			WAZA_NomikomuNoHit
	JUMP				WAZA_Kaifuku
WAZA_NomikomuNoHit:
	WAIT				MOJI_WAIT/2
	MESSAGE_TBL			NomikomuMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

.if 0
@===================================================
@	163 �˂��Ղ�
@===================================================

WAZA_NeppuuHit:
	KYOURYOKU			0
	MESSAGE_WAIT		MOJI_WAIT
	KYOURYOKU			3
	WAZA_EFFECT_WAIT
	KYOURYOKU			1
	MESSAGE_WAIT		MOJI_WAIT
	KYOURYOKU			4
	WAZA_EFFECT_WAIT
	MESSAGE				NEPPUUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@WAZA_DoubleHit:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
WAZA_NeppuuDoubleHitAgain:
	CRITICAL_CHECK
	KYOURYOKU			2
	TYPE_CHECK
	DAMAGE_LOSS
	HITCHECK			WAZA_NeppuuNohit,0
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	CHECK2vs2			WAZA_NeppuuDoubleHitAgain
	WAZAEND
WAZA_NeppuuNohit:
	WAZAKOUKA_SE
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	CHECK2vs2			WAZA_NeppuuDoubleHitAgain
	WAZAEND
.endif

@===================================================
@	164 �����
@===================================================

WAZA_ArareHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	ARARE
	JUMP				WAZA_Tenkou

@===================================================
@	165 ���������
@===================================================

WAZA_IchamonHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HITCHECK			WAZA_Umakukimaran,0
	ICHAMON				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				ICHAMONMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	166 �����Ă�
@===================================================

WAZA_OdateruHit:
	BADGECHECK
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_NormalNohitBitSet
	HITCHECK			WAZA_NormalNohit,0
	ATTACK_MESSAGE
	PP_DEC
	IBARU				COND_SPEPOW,WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEPOW+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT,WAZA_OdateruNoMsg
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_OdateruNoMsg
	COND_CHG_WORK_SET
	FIGHT_EFFECT		SIDE_DEFENCE,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_OdateruNoMsg:
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_MAIPEESU,WAZA_Mypace
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	SETVALUE			ServerWork+3,COND2_KONRAN
	TSUIKA_DIRECT_ACT
	JUMP				WAZA_WazaEnd

@===================================================
@	167 ���ɂ�
@===================================================

WAZA_OnibiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_YAKEDO,WAZA_YakedoAlready
	CHECK_TYPE			SIDE_DEFENCE,FIRE_TYPE,WAZA_Koukanai
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_MIZUNOBEERU,WAZA_MizunobeeruJump
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_BAD,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,0
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			ServerWork+3,COND_YAKEDO
	TSUIKA_DIRECT_ACT
	JUMP				WAZA_WazaEnd
WAZA_MizunobeeruJump:
	SETWORK				TsuikaClient,DefenceClient,1
	SETVALUE			ServerWork+5,0
	GOSUB				WAZA_Mizunobeeru
	JUMP				WAZA_WazaEnd
WAZA_YakedoAlready:
	WAIT				MOJI_WAIT/2
	MESSAGE				ALREADYYAKEDOMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	168 �����݂₰
@===================================================

WAZA_OkimiyageHit:
@	BADGECHECK2			@�܂���`�F�b�N�̂Ȃ��o�b�W�`�F�b�N
	BADGECHECK
	IF8					FLAG_EQ,ServerWork+6,MSG_MAMORUNOHIT,WAZA_OkimiyageMamoru
	ATTACK_MESSAGE
	PP_DEC
	OKIMIYAGE			WAZA_Umakukimaran
	JIBAKU				@������m���ɂ���
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_OkimiyageMigawari
@	GUARDCHECK			WAZA_OkimiyageNoHit		@�܂���A�݂���`�F�b�N
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_DEFENCE,COND_BIT_POW+COND_BIT_SPEPOW,COND_FLAG_DOWN+COND_FLAG_DOUBLE+COND_FLAG_MULTI
	COND_CHG_BIT_CHECK	SIDE_DEFENCE,COND_BIT_POW,COND_FLAG_DOWN+COND_FLAG_DOUBLE
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKD2
	CONDCHG				TSUIKA_DIRECT,WAZA_OkimiyageNext
	IF8					FLAG_C,ServerWork+5,1,WAZA_OkimiyageNext
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_OkimiyageNext:
	COND_CHG_BIT_CHECK	SIDE_DEFENCE,COND_BIT_SPEPOW,COND_FLAG_DOWN+COND_FLAG_DOUBLE
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEPOW+PARA_RANKD2
	CONDCHG				TSUIKA_DIRECT,WAZA_OkimiyageNoHit
	IF8					FLAG_C,ServerWork+5,1,WAZA_OkimiyageNoHit
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_OkimiyageNoHit:
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	JUMP				WAZA_WazaEnd

WAZA_OkimiyageMigawari:
	MESSAGE				OKIMIYAGEMIGAWARIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_OkimiyageNoHit

@���ꂽ�Ƃ��́A�Z�G�t�F�N�g���o���Ȃ�
WAZA_OkimiyageMamoru:
	ATTACK_MESSAGE
	PP_DEC
	OKIMIYAGE			WAZA_OkimiyageMamoruNext
WAZA_OkimiyageMamoruNext:
	JIBAKU
	WAIT				MOJI_WAIT
	WAZAKOUKA_SE
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	JUMP				WAZA_WazaEnd

@===================================================
@	169 ���炰��
@===================================================

WAZA_KaragenkiHit:
	CONDITION_CHECK		SIDE_ATTACK,CONDITION_KARAGENKI,WAZA_Karagenki
	JUMP				WAZA_NormalHit
WAZA_Karagenki:
	SETVALUE			UserWork+DAMAGE_VALUE,2	@2�{�_���[�W
	JUMP				WAZA_NormalHit

@===================================================
@	170 �������p���`
@===================================================

WAZA_KiaiPunchHit:
	BADGECHECK
	KIAIPUNCH			WAZA_NormalHit2
	PP_DEC
	MESSAGE				KIAIPUNCHNOMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	171 ����
@===================================================

WAZA_KitsukeHit:
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_NormalHit
	SETVALUE			ServerWork+3,COND_NOMAHI+TSUIKA_ALWAYS
	CONDITION_CHECK		SIDE_DEFENCE,CONDITION_MAHI,WAZA_Kitsuke
	JUMP				WAZA_NormalHit
WAZA_Kitsuke:
	SETVALUE			UserWork+DAMAGE_VALUE,2	@2�{�_���[�W
	JUMP				WAZA_NormalHit

@===================================================
@	172 ���̂�тƂ܂�
@===================================================

WAZA_KonoyubitomareHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	KONOYUBITOMARE
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				KONOYUBITOMAREMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	173 ������̂�����
@===================================================

WAZA_ShizennochikaraHit:
	BADGECHECK
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	SHIZENNOCHIKARA
	MESSAGE				SHIZENNOCHIKARAMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@===================================================
@	174 ���イ�ł�
@===================================================

WAZA_JuudenHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	JUUDEN
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				JUUDENMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	175 ���傤�͂�
@===================================================

WAZA_ChouhatsuHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HITCHECK			WAZA_Umakukimaran,0
	CHOUHATSU			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				CHOUHATSUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	176 �Ă�����
@===================================================

WAZA_TedasukeHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	TEDASUKE			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				TEDASUKEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	177 �g���b�N
@===================================================

WAZA_TrickHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
	HITCHECK			WAZA_Umakukimaran,0
	TRICK				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				TRICKMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	MESSAGE_TBL			TrickMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	178 �Ȃ肫��
@===================================================

WAZA_NarikiriHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HITCHECK			WAZA_Umakukimaran,HITCHECK_KIE
	NARIKIRI			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				NARIKIRIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	179 �˂�������
@===================================================

WAZA_NegaigotoHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	@0�͓���t���O�i0:�˂������Ƃ�����1:�˂������Ƃ����Ȃ��������j
	NEGAIGOTO			0,WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
@ATTACK_MESSAGE�Ƃ��Ȃ��������̂�
@	MESSAGE				NEGAIGOTOMSGNO
@	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	180 �˂��̂�
@===================================================

WAZA_NekonoteHit:
	BADGECHECK
	ATTACK_MESSAGE
	NEKONOTE			WAZA_UmakukimaranPPDec
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+WAZA_EFF_CNT,0
	SETVALUE			UserWork+WAZA_EFF_CNT_TMP,0
	WAZASEQADRS			1

@===================================================
@	181 �˂��͂�
@===================================================

WAZA_NewoharuHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	NEWOHARU			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				NEWOHARUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	182 �΂�������
@===================================================

WAZA_BakadikaraHit:
	SETVALUE			ServerWork+3,COND2_BAKADIKARA+TSUIKA_ALWAYS+TSUIKA_ATTACK
	JUMP				WAZA_NormalHit

@===================================================
@	183 �}�W�b�N�R�[�g
@===================================================

WAZA_MagicCortHit:
	BADGECHECK
	MAGICCORT			WAZA_UmakukimaranPP
	ATTACK_MESSAGE
	PP_DEC
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				MAGICCORTMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	184 ���T�C�N��
@===================================================

WAZA_RecycleHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	RECYCLE				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				RECYCLEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	185 ���x���W
@===================================================

WAZA_RevengeHit:
	REVENGE
	JUMP				WAZA_NormalHit

@===================================================
@	186 �������
@===================================================

WAZA_KawarawariHit:
	BADGECHECK
	HITCHECK			WAZA_NormalNohit,0
						@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	ATTACK_MESSAGE
	PP_DEC
	KAWARAWARI
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	DAMAGE_LOSS
	IF8					FLAG_EQ,UserWork+WAZA_EFF_CNT,0,WAZA_KawarawariEff
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_KOUKANAI+WAZASTATUSFLAG_NOHIT
WAZA_KawarawariEff:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	IF8					FLAG_NC,UserWork+WAZA_EFF_CNT,2,WAZA_KawarawariNoMes
	MESSAGE				KAWARAWARIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_KawarawariNoMes:
	TYPE_CHECK_NO_HIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	TSUIKA_CHECK
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	JUMP				WAZA_WazaEnd

@===================================================
@	187 ������
@===================================================

WAZA_AkubiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_YARUKI,WAZA_SpeabiUmakukimaran
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_HUMIN,WAZA_SpeabiUmakukimaran
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_Umakukimaran
@	WAZAKOUKA_CHECK		SIDE_DEFENCE,WAZAKOUKA_KIE,FLAG_ON,WAZA_Umakukimaran
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_Shinpi
	HITCHECK			WAZA_Umakukimaran,HITCHECK_KIE
	SAWAGU_CHECK		WAZA_Umakukimaran
	AKUBI				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				AKUBIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_SpeabiUmakukimaran:
	SETWORK				UserWork+CLIENT_NO_WORK,UserWork+SPEABI_CLIENT,1
WAZA_SpeabiUmakukimaran2:
	WAIT				MOJI_WAIT/2
	MESSAGE				TOKUSYUUMAKUKIMARANMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	188 �͂������Ƃ�
@===================================================

WAZA_HatakiotosuHit:
.if 0
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HATAKIOTOSU			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				HATAKIOTOSUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	WAZAEND
.endif
	SETVALUE			ServerWork+3,COND2_HATAKIOTOSU
	JUMP				WAZA_NormalHit

@===================================================
@	189 ���ނ����
@===================================================

WAZA_GamusyaraHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	GAMUSYARA			WAZA_Umakukimaran
	SETWORK				HitDamage,WazaDamage,4
	HITCHECK			WAZA_NormalNohit2,0
	TYPE_CHECK
	IF8					FLAG_BIT,WazaStatusFlag,WAZASTATUSFLAG_NOCRITICAL,WAZA_Hit
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_BATSUGUN|WAZASTATUSFLAG_IMAHITOTSU
	SETWORK				WazaDamage,HitDamage,4
	KORAERU_CHECK
	JUMP				WAZA_Hit

@===================================================
@	190 �ӂ�
@===================================================

WAZA_HunkaHit:
	HUNKA
	JUMP				WAZA_NormalHit

@===================================================
@	191 �X�L���X���b�v
@===================================================

WAZA_SkillSwapHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HITCHECK			WAZA_Umakukimaran,HITCHECK_KIE
	SKILLSWAP			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				SKILLSWAPMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	192 �ӂ�����
@===================================================

WAZA_HuuinHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HUUIN				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				HUUINMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	193 ���t���b�V��
@===================================================

WAZA_RefleshHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	REFLESH				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				REFLESHMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	CONDITION_GAUGE		SIDE_ATTACK
	JUMP				WAZA_WazaEnd

@===================================================
@	194 ����˂�
@===================================================

WAZA_OnnenHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	ONNEN				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				ONNENMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	195 �悱�ǂ�
@===================================================

WAZA_YokodoriHit:
	BADGECHECK
	YOKODORI			WAZA_UmakukimaranPP
	ATTACK_MESSAGE
	PP_DEC
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAIT				MOJI_WAIT/2
	MESSAGE				YOKODORIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	196 ��������
@===================================================

WAZA_KetaguriHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	KETAGURI
	HITCHECK			WAZA_NormalNohit2,0
	JUMP				WAZA_HitCalcOnly

@===================================================
@	197 �Ђ݂̂�����
@===================================================

WAZA_HimitunoTikaraHit:
	HIMITUNOTIKARA
	JUMP				WAZA_NormalHit

@===================================================
@	198 ���Ă݃^�b�N��
@===================================================

WAZA_SutemiTackleHit:
	SETVALUE			ServerWork+3,COND2_HANEKAERI2+TSUIKA_ALWAYS+TSUIKA_ATTACK
	JUMP				WAZA_NormalHit

@===================================================
@	199 �S�Ẵ|�P����������
@===================================================

WAZA_AllKonranHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	SETVALUE			DefenceClient,0
WAZA_AllKonranLoop:
@	SETVALUE			WazaStatusFlag,0
	LOOPINIT			@���[�v����Ƃ��ɏ��������郏�[�N���܂Ƃ܂��Ă���
	SETVALUE			ServerWork+3,COND2_KONRAN
	IFWEQ				AttackClient,DefenceClient,1,WAZA_AllKonranNext
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_MAIPEESU,WAZA_AllKonranMypace
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_AllKonranUmakukimaran
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_KONRAN,WAZA_AllKonranAlready
	HITCHECK			WAZA_AllKonranNohit,0
	SIDECONDITION_CHECK	SIDE_DEFENCE,SIDE_CONDITION_SHINPI,WAZA_AllKonranShinpi
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	TSUIKA_DIRECT_ACT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
WAZA_AllKonranNext:
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	ADDVALUE			DefenceClient,1
	IFWNE				DefenceClient,ClientSetMax,1,WAZA_AllKonranLoop
	WAZAEND

WAZA_AllKonranMypace:
	WAIT				MOJI_WAIT/2
	MESSAGE				MYPACEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_AllKonranNext

WAZA_AllKonranShinpi:
	WAIT				MOJI_WAIT/2
	MESSAGE				SHINPIGUARDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_AllKonranNext

WAZA_AllKonranUmakukimaran:
	WAIT				MOJI_WAIT/2
	MESSAGE				UMAKUKIMARANMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_AllKonranNext

WAZA_AllKonranAlready:
	WAIT				MOJI_WAIT/2
	MESSAGE				KONRANMSG5NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_AllKonranNext

WAZA_AllKonranNohit:
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_AllKonranNext

@===================================================
@	201 �d�C�n�̋Z�̈З͂𔼌�
@===================================================
@===================================================
@	210 ���n�̋Z�̈З͂𔼌�
@===================================================

WAZA_AsobiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	ASOBI				WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE_TBL			AsobiMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	202 �ǉ����ʂłǂ��ǂ�
@===================================================

WAZA_DokuDokuHit:
	SETVALUE			ServerWork+3,COND_DOKUDOKU
	JUMP				WAZA_NormalHit

@===================================================
@	203 �V�C�ɂ���ĈЗ͂ƃ^�C�v
@===================================================

WAZA_WeatherHit:
	WEATHER_BALL
	JUMP				WAZA_NormalHit

@===================================================
@	204 �����̓��U���A�������Ɖ�����
@===================================================

WAZA_OverHeatHit:
	SETVALUE			ServerWork+3,COND2_OVERHEAT+TSUIKA_ALWAYS+TSUIKA_ATTACK
	JUMP				WAZA_NormalHit

@===================================================
@	205 ����̍U���A�h�䂪������
@===================================================

WAZA_KusuguruHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDCHG_CHECK		SIDE_DEFENCE,IFC,COND_POW,0,WAZA_KusuguruStart
	CONDCHG_CHECK		SIDE_DEFENCE,IFEQ,COND_DEF,0,WAZA_KusuguruNoHit
WAZA_KusuguruStart:
	HITCHECK			WAZA_Umakukimaran,0
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_DEFENCE,COND_BIT_POW+COND_BIT_DEF,COND_FLAG_DOWN+COND_FLAG_MULTI
	COND_CHG_BIT_CHECK	SIDE_DEFENCE,COND_BIT_POW,COND_FLAG_DOWN
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKD1
	CONDCHG				TSUIKA_DIRECT,WAZA_KusuguruNext
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_KusuguruNext
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_KusuguruNext:
	COND_CHG_BIT_CHECK	SIDE_DEFENCE,COND_BIT_DEF,COND_FLAG_DOWN
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKD1
	CONDCHG				TSUIKA_DIRECT,WAZA_KusuguruEnd
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_KusuguruEnd
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_KusuguruEnd:
	JUMP				WAZA_WazaEnd
WAZA_KusuguruNoHit:
	WAIT				MOJI_WAIT/2
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_UMAKUKIMARAN
	MESSAGE				NOURYOKUNODOWNMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	206 �h��A���h��������
@===================================================

WAZA_CosmoPowerHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDCHG_CHECK		SIDE_ATTACK,IFNC,COND_DEF,12,WAZA_CosmoPowerStart
	CONDCHG_CHECK		SIDE_ATTACK,IFEQ,COND_SPEDEF,12,WAZA_MeisouNoHit
WAZA_CosmoPowerStart:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_DEF+COND_BIT_SPEDEF,COND_FLAG_UP
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_CosmoPowerNext
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_CosmoPowerNext
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_CosmoPowerNext:
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEDEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_CosmoPowerEnd
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_CosmoPowerEnd
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_CosmoPowerEnd:
	JUMP				WAZA_WazaEnd

@===================================================
@	207 �����ł��鑊��ɂ�������
@===================================================

WAZA_SkyUpperHit:
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_SORAWOTOBUHIT
	JUMP				WAZA_NormalHit

@===================================================
@	208 �U���A�h�䂪������
@===================================================

WAZA_BuildUpHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDCHG_CHECK		SIDE_ATTACK,IFNC,COND_POW,12,WAZA_BuildUpStart
	CONDCHG_CHECK		SIDE_ATTACK,IFEQ,COND_DEF,12,WAZA_MeisouNoHit
WAZA_BuildUpStart:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_POW+COND_BIT_DEF,COND_FLAG_UP
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_BuildUpNext
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_BuildUpNext
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_BuildUpNext:
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_BuildUpEnd
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_BuildUpEnd
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_BuildUpEnd:
	JUMP				WAZA_WazaEnd

@===================================================
@	211 ���U�A���h��������
@===================================================

WAZA_MeisouHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDCHG_CHECK		SIDE_ATTACK,IFNC,COND_SPEPOW,12,WAZA_MeisouStart
	CONDCHG_CHECK		SIDE_ATTACK,IFEQ,COND_SPEDEF,12,WAZA_MeisouNoHit
WAZA_MeisouStart:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_SPEPOW+COND_BIT_SPEDEF,COND_FLAG_UP
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEPOW+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_MeisouNext
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_MeisouNext
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_MeisouNext:
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEDEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_MeisouEnd
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_MeisouEnd
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_MeisouEnd:
	JUMP				WAZA_WazaEnd
WAZA_MeisouNoHit:
	WAIT				MOJI_WAIT/2
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_UMAKUKIMARAN
	MESSAGE				NOURYOKUNOUPMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	212 �U���Ƒf������������
@===================================================

WAZA_RyuunomaiHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	CONDCHG_CHECK		SIDE_ATTACK,IFNC,COND_POW,12,WAZA_RyuunomaiStart
	CONDCHG_CHECK		SIDE_ATTACK,IFEQ,COND_AGI,12,WAZA_MeisouNoHit
WAZA_RyuunomaiStart:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_POW+COND_BIT_AGI,COND_FLAG_UP
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_RyuunomaiNext
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_RyuunomaiNext
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_RyuunomaiNext:
	SETVALUE			UserWork+COND_CHG_WORK,COND_AGI+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_RyuunomaiEnd
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_RyuunomaiEnd
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_RyuunomaiEnd:
	JUMP				WAZA_WazaEnd

@===================================================
@	213 �t�B�[���h�Ɠ����^�C�v�ɂȂ�
@===================================================

WAZA_HogosyokuHit:
	BADGECHECK
	ATTACK_MESSAGE
	PP_DEC
	HOGOSYOKU			WAZA_Umakukimaran
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	MESSAGE				TEXTUREMSGNO	@ATTACK_MONS�́@�H�^�C�v�Ɂ@�Ȃ���
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	����Z�V�[�P���X
@===================================================

WAZA_KizetsuA:
	VOICE				SIDE_ATTACK
	WAIT				MOJI_WAIT
	KIZETSU				SIDE_ATTACK
	KIZETSU_WAIT		SIDE_ATTACK
	MESSAGE				KIZETSUMSGANO
	SEQRET

WAZA_KizetsuD:
	VOICE				SIDE_DEFENCE
	WAIT				MOJI_WAIT
	KIZETSU				SIDE_DEFENCE
	KIZETSU_WAIT		SIDE_DEFENCE
	MESSAGE				KIZETSUMSGDNO
	SEQRET

WAZA_GetEXP:
	SETVALUE			UserWork+GETEXP_SEQNO,0
	GET_EXP				SIDE_DEFENCE
@	WAZAEND
	SEQEND

WAZA_Zenmetsu:
	ZENMETSU_CHECK		WAZA_DoubleNockDown
	IF8					FLAG_NE,WinLoseFlag,0,_ZenmetsuEnd
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_TRAINER,WAZA_MineReshuffleStart
	IF32				FLAG_NBIT,ServerStatusFlag,SERVER_STATUS_FLAG_MINE_KIZETSU,WAZA_MineReshuffleStart
	MESSAGE				NEXTPOKEMSGNO
	SETVALUE			ServerWork+0,0		@�V�[�P���X�i���o�[��������
	YES_NO_WINDOW
	IF8					FLAG_EQ,ServerWork+1,0,WAZA_MineReshuffleStart
	ESCAPE_CHECK		_ZenmetsuEnd
	MESSAGE				ESCAPENGMSG4NO
WAZA_MineReshuffleStart:
	POKE_RESHUFFLE		SIDE_KIZETSU,_ZenmetsuEnd	@2��2�œ���ւ��ł��Ȃ��ꍇ�̔�ѐ�
	POKE_RESHUFFLE_WAIT	SIDE_KIZETSU,PRW_NORMAL
@����ւ����I�����Ă���Ƃ��̏���
	IF16				FLAG_NBIT,FightType,FIGHT_TYPE_TRAINER,WAZA_NoMineReshuffle	
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_TUUSHIN,WAZA_NoMineReshuffle	
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_BATTLE_TOWER,WAZA_NoMineReshuffle	
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_2vs2,WAZA_NoMineReshuffle	
	IF32				FLAG_BIT,ServerStatusFlag,SERVER_STATUS_FLAG_MINE_KIZETSU,WAZA_NoMineReshuffle
	IF8					FLAG_EQ,UserWork+BTL_TYPE,1,WAZA_NoMineReshuffle
	RESHUFFLE_CHECK		SIDE_ATTACK,WAZA_NoMineReshuffle
	MESSAGE				TORIKAERUMSGNO
	SETVALUE			ServerWork+0,0		@�V�[�P���X�i���o�[��������
	YES_NO_WINDOW
	IF8					FLAG_EQ,ServerWork+1,1,WAZA_NoMineReshuffle
	GET_RESHUFFLE_CLIENT
	POKE_RESHUFFLE		SIDE_ATTACK+B_CANCEL_OK,WAZA_NoMineReshuffle
	POKE_RESHUFFLE_WAIT	SIDE_ATTACK,PRW_NORMAL
@�|�P������I�����Ȃ������iB�L�����Z�����܂ށj���̔�ѐ�	
	IF8					FLAG_EQ,ServerWork+0,6,WAZA_NoMineReshuffle
	RESHUFFLE_NICKNAME
	SUBROUTINE_CALL		SIDE_ATTACK,RESHUFFLE_INIT
	POKE_HPDEC_RATE		SIDE_ATTACK
	MESSAGE				IREKAEMSGNO
@	MESSAGE_WAIT		MOJI_WAIT/2
	SIZENKAIHUKU		SIDE_ATTACK
	CLIENT_BIT_WAIT
	MODOSU_EFFECT
	CLIENT_BIT_WAIT
	BALL_GAUGE_SET		SIDE_ATTACK
	APPEAR_EFFECT_INIT	SIDE_ATTACK
	APPEAR_EFFECT		SIDE_ATTACK
	POKE_HPMAX_RATE		SIDE_ATTACK
	MESSAGE				IREKAEAFTERMSGNO
@	WAIT				MOJI_WAIT/2
	BALL_GAUGE_OFF		SIDE_ATTACK
	APPEAR_EFFECT_WAIT	SIDE_ATTACK,APPEAR_NORMAL
	CLIENT_BIT_WAIT
	POKE_RESHUFFLE_END	SIDE_ATTACK
	EXPFLAG_CLEAR		@FightJoinFlag�̃N���A
WAZA_NoMineReshuffle:
	BALL_GAUGE_SET		SIDE_KIZETSU
	APPEAR_EFFECT_INIT	SIDE_KIZETSU
	APPEAR_EFFECT		SIDE_KIZETSU
@	MESSAGE_TBL			PokeAppearMsgTbl
	POKE_HPMAX_RATE		SIDE_KIZETSU
	MESSAGE				IREKAEAFTERMSGNO
@	WAIT				MOJI_WAIT/2
	BALL_GAUGE_OFF		SIDE_KIZETSU
	APPEAR_EFFECT_WAIT	SIDE_KIZETSU,APPEAR_NORMAL
	CLIENT_BIT_WAIT
WAZA_MakibishiAfter:
	POKE_RESHUFFLE_END	SIDE_KIZETSU
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_2vs2,_ZenmetsuEnd
	SCA_END_SET
_ZenmetsuEnd:
@	WAZAEND
	SEQEND

WAZA_DoubleNockDown:
	POKE_RESHUFFLE		SIDE_DOUBLE1,WAZA_DoubleNockDownNext
WAZA_DoubleNockDownNext:
	POKE_RESHUFFLE_WAIT	SIDE_KIZETSU,PRW_AFTER_SELMONS_SET
	POKE_RESHUFFLE		SIDE_DOUBLE2,WAZA_DoubleNockDownEnd
	POKE_RESHUFFLE_WAIT	SIDE_KIZETSU,PRW_AFTER_SELMONS_SET
WAZA_DoubleNockDownStart:
	POKE_RESHUFFLE_WAIT	SIDE_KIZETSU,PRW_NORMAL
	BALL_GAUGE_SET		SIDE_KIZETSU
	APPEAR_EFFECT_INIT	SIDE_KIZETSU
	APPEAR_EFFECT		SIDE_KIZETSU
	POKE_HPMAX_RATE		SIDE_KIZETSU
	MESSAGE				IREKAEAFTERMSGNO
	BALL_GAUGE_OFF		SIDE_KIZETSU
	APPEAR_EFFECT_WAIT	SIDE_KIZETSU,APPEAR_NORMAL
	CLIENT_BIT_WAIT
	POKE_RESHUFFLE_END	SIDE_DOUBLE1
	IFWNE				KizetsuClient,ClientSetMax,1,WAZA_DoubleNockDownStart
WAZA_DoubleNockDownEnd:
@	WAZAEND
	SEQEND

WAZA_TrainerWin:
	MESSAGE				TRAINERWINMSGNO
	TRAINER_APPEAR		SIDE_ENEMY
	CLIENT_BIT_WAIT
	MESSAGE				TRAINERLOSEMSGNO
	OKODUKAI_CALC
	MESSAGE				OKODUKAIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_Win:
	KOBAN_CHECK
	MONOHIROI
	SEQEND

WAZA_Lose:
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_BATTLE_TOWER,WAZA_Lose_END
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_CARD_E,WAZA_Lose_END
	IF16				FLAG_EQ,FieldTrainerID,ORUSUBAN_TRAINER,WAZA_Lose_END
	MESSAGE				LOSEMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	MESSAGE				LOSEMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_Lose_END:
	SEQEND


WAZA_TuushinWin:
	MESSAGE				TAISENENDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	END_FIGHT
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

WAZA_BattleTowerWin:
	MESSAGE				TRAINERWINMSGNO
	TRAINER_APPEAR		SIDE_ENEMY
	CLIENT_BIT_WAIT
	SEQEND

WAZA_ItemEscape:
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_KEMURIDAMA,0
	MESSAGE				ITEMESCAPEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

WAZA_SpeabiEscape:
@	SE_PLAY				SE_NIGERU
	MESSAGE				SPEABIESCAPEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

WAZA_Escape:
	MESSAGE				ESCAPEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

WAZA_EnemyEscape:
	MESSAGE				ENEMYESCAPEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

WAZA_TrainerEscape:
	MESSAGE				TRAINERESCAPEMSGNO
	SEQEND

WAZA_EscapeNG:
	MESSAGE_TBL			EscapeNGMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

WAZA_EscapeNG2:
	ALERT_MESSAGE_TBL	EscapeNGMsgTbl
	ALERTEND

WAZA_SafariBallThrowNG:
	ALERT_MESSAGE		SAFARIBALLNGMSGNO
	ALERTEND

WAZA_PokeReshuffle:
	POKE_HPDEC_RATE		SIDE_ATTACK
	MESSAGE				IREKAEMSGNO
@	MESSAGE_WAIT		MOJI_WAIT/2
	SETVALUE			UserWork+DAMAGE_VALUE,2	@����������2�{�_���[�W
	IF8					FLAG_BIT,FightType,FIGHT_TYPE_2vs2,WAZA_OiuchiCheck2vs2
WAZA_OiuchiCheck1vs1:
	LOOPCNT				1
	JUMP				WAZA_OiuchiCheck
WAZA_OiuchiCheck2vs2:
	LOOPCNT				2
WAZA_OiuchiCheck:
	OIUCHI_CHECK		WAZA_NoOiuchi
	SWITCH_A_D
	MICHIDURE_CHECK
	GOSUB				WAZA_OiuchiAttack
	SWITCH_A_D
WAZA_NoOiuchi:
	LOOP				WAZA_OiuchiCheck
	SIZENKAIHUKU		SIDE_ATTACK
	CLIENT_BIT_WAIT
	MODOSU_EFFECT
	CLIENT_BIT_WAIT
	BALL_GAUGE_SET		SIDE_ATTACK
@POKE_RESHUFFLE_WAIT��ʂ��Ă����Ȃ��͂�
	POKE_RESHUFFLE_WAIT	SIDE_ATTACK,PRW_LISTROW_SET
WAZA_PokeAppear:
	APPEAR_EFFECT_INIT	SIDE_ATTACK
	APPEAR_EFFECT		SIDE_ATTACK
	POKE_HPMAX_RATE		SIDE_ATTACK
	MESSAGE				IREKAEAFTERMSGNO
@	WAIT				MOJI_WAIT/2
	BALL_GAUGE_OFF		SIDE_ATTACK
	APPEAR_EFFECT_WAIT	SIDE_ATTACK,APPEAR_NORMAL
	CLIENT_BIT_WAIT
	POKE_RESHUFFLE_END	SIDE_ATTACK
	SETVALUE			UserWork+WKC_WORK0,WKC_SPEABI_CHECK_ALL
	WKC_ACT				WKC_CHECK_ONLY,0
	SETVALUE			UserWork+WKC_WORK0,WKC_OUMU_WAZA_NO
	WKC_ACT				WKC_CHECK_ONLY,0
	SEQEND

WAZA_OiuchiAttack:
	WAIT				MOJI_WAIT/2
	ATTACK_MESSAGE
	PP_DEC
	CRITICAL_CHECK
	DAMAGE_CALC
	TYPE_CHECK
	DAMAGE_LOSS
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	CRITICAL_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	SETVALUE			UserWork+WKC_WORK0,WKC_SPEABI_CHECK
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_KODAWARI_SET
@	IF32				FLAG_NBIT,ServerStatusFlag,SERVER_STATUS_FLAG_KIZETSUBIT,WAZA_NoKizetsu
	SUBROUTINE_CALL		SIDE_DEFENCE,CHECK_KIZETSU
	IF8					FLAG_EQ,ServerWork+0,0,WAZA_NoKizetsu
	SETVALUE			UserWork+GETEXP_SEQNO,0
	GET_EXP				SIDE_DEFENCE
WAZA_NoKizetsu:
	SEQRET

WAZA_Wait:
	WAIT				MOJI_WAIT/2
	SEQRET

WAZA_LevelUp:
	ME_PLAY				MUS_FANFA1
	MESSAGE				LEVELUPMSGNO
	SETVALUE			UserWork+SW_SEQNO,0
	STATUS_WINDOW
	WAZAOBOE_CHECK		WAZA_WazaOboe,WAZA_NoWazaOboe,1
	JUMP				WAZA_WazaWasureStart
WAZA_WazaOboeLoop:
	WAZAOBOE_CHECK		WAZA_WazaOboe,WAZA_NoWazaOboe,0
WAZA_WazaWasureStart:
	OBOEWAZANO_SET
	MESSAGE				WAZAOBOEMSG2NO
@	MESSAGE_WAIT		MOJI_WAIT
	MESSAGE				WAZAOBOEMSG3NO
@	MESSAGE_WAIT		MOJI_WAIT
	MESSAGE				WAZAWASUREMSGNO
	CLIENT_BIT_WAIT
	SETVALUE			UserWork+WW_SEQNO,0
	WAZAWASURE_CHECK	WAZA_WazaWasure
WAZA_WazaAkirame:
	MESSAGE				WAZAAKIRAMEMSGNO
	CLIENT_BIT_WAIT
	SETVALUE			UserWork+WW_SEQNO,0
	WAZAAKIRAME_CHECK	WAZA_WazaWasureStart
	MESSAGE				WAZAAKIRAMEMSG2NO
@	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaOboeLoop
WAZA_WazaWasure:
	MESSAGE				WAZAWASUREMSG2NO
@	MESSAGE_WAIT		MOJI_WAIT*2
	MESSAGE				WAZAWASUREMSG3NO
@	MESSAGE_WAIT		MOJI_WAIT
	MESSAGE				WAZAWASUREMSG4NO
@	MESSAGE_WAIT		MOJI_WAIT
WAZA_WazaOboe:
	OBOEWAZANO_SET
	ME_PLAY				MUS_FANFA1
	MESSAGE				WAZAOBOEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	SETVALUE			ServerWork,0
@	WAZA_GET
@	CLIENT_BIT_WAIT
	SUBROUTINE_CALL		SIDE_ATTACK,KODAWARI_RESET
	JUMP				WAZA_WazaOboeLoop
WAZA_NoWazaOboe:
@�������n�`�}�L�̋Z��Y�ꂽ���̏���
	SEQRET

@===================================================
@	�J�~��
@===================================================

WAZA_Amefuri:
	MESSAGE_TBL			AmagoiKeepMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_Amefuri_End
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_AMAGOI,0
WAZA_Amefuri_End:
	SEQEND

@===================================================
@	�V��ɂ��_���[�W
@===================================================

.if 0
WAZA_WeatherDamage:
	MESSAGE_TBL			WeatherKeepMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	FIGHT_EFFECT2		SIDE_ATTACK,UserWork+FIGHT_EFFECT_WORK,0
	SETVALUE			ServerWork,0
WAZA_WeatherDamageLoop:
	SETWORKOFS			AttackClient,ActionClientNo,ServerWork,1
	WEATHER_KEEP
	IF32				FLAG_EQ,WazaDamage,0,WAZA_WeatherDamageNext
	MESSAGE_TBL			WeatherDamageMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���B�Z�𖳑����ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI+SERVER_STATUS_FLAG_NOGAMAN+SERVER_STATUS_FLAG_NO_MICHIDURE
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_ATTACK
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	ZENMETSU_CHECK		WAZA_DoubleNockDown
	IF8					FLAG_NE,WinLoseFlag,0,WAZA_WeatherDamageRet
WAZA_WeatherDamageNext:
	ADDVALUE			ServerWork,1
	IFWNE				ServerWork,ClientSetMax,1,WAZA_WeatherDamageLoop
WAZA_WeatherDamageRet:
	RESET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI+SERVER_STATUS_FLAG_NOGAMAN+SERVER_STATUS_FLAG_NO_MICHIDURE
	SEQEND
.else
WAZA_WeatherDamage:
	MESSAGE_TBL			WeatherKeepMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	FIGHT_EFFECT2		SIDE_ATTACK,UserWork+FIGHT_EFFECT_WORK,0
	SETVALUE			ServerWork,0
WAZA_WeatherDamageLoop:
	SETWORKOFS			AttackClient,ActionClientNo,ServerWork,1
	WEATHER_KEEP
	IF32				FLAG_EQ,WazaDamage,0,WAZA_WeatherDamageNext
	MESSAGE_TBL			WeatherDamageMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���B�Z�𖳑����ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI+SERVER_STATUS_FLAG_NOGAMAN+SERVER_STATUS_FLAG_NO_MICHIDURE
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_ATTACK
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	ZENMETSU_CHECK		WAZA_WeatherDamageNext
WAZA_WeatherDamageNext:
	IF8					FLAG_NE,WinLoseFlag,0,WAZA_WeatherDamageRet
	ADDVALUE			ServerWork,1
	IFWNE				ServerWork,ClientSetMax,1,WAZA_WeatherDamageLoop
WAZA_WeatherDamageRet:
	RESET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI+SERVER_STATUS_FLAG_NOGAMAN+SERVER_STATUS_FLAG_NO_MICHIDURE
	SEQEND
.endif

WAZA_WeatherDamageEnd:
	MESSAGE_TBL			WeatherDamageEndMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

@===================================================
@	����
@===================================================

WAZA_Hare:
	MESSAGE				NIHONBAREKEEPMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_NIHONBARE,0
	SEQEND

WAZA_HareEnd:
	MESSAGE				NIHONBAREENDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

@===================================================
@	�t�B�[���h�̓V��𔽉f	
@===================================================

WAZA_FieldWeather:
	MESSAGE_TBL			WeatherMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	FIGHT_EFFECT2		SIDE_ATTACK,UserWork+FIGHT_EFFECT_WORK,0
	SPEABIEND

@===================================================
@	�����K�[�h
@===================================================

WAZA_ReflecEnd:
	MESSAGE				WAZAKOUKAENDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

@===================================================
@	����҂̂܂���
@===================================================

WAZA_Shinpi:
	WAIT				MOJI_WAIT/2
	MESSAGE				SHINPIGUARDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

WAZA_ShinpiEnd:
	WAIT				MOJI_WAIT/2
	MESSAGE				SHINPIENDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

@===================================================
@	�Z���ʃV�[�P���X
@===================================================

@===================================================
@	��ǂ肬
@===================================================

WAZA_Yadorigi:
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_YADORIGI,UserWork+FIGHT_EFFECT_WORK
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	SETWORK				WazaDamage,HitDamage,4
@����\�̓w�h�������́A�̗͂��ւ�
	SPEABI_CHECK		SIDE_ATTACK,TOKUSYU_HEDOROEKI,WAZA_YadorigiHedoro
	DAMAGE_CHG			DAMAGE_NEG			@�v���}�C���]
	SETVALUE			ServerWork+5,3
	JUMP				WAZA_YadorigiNext
WAZA_YadorigiHedoro:
	SETVALUE			ServerWork+5,4
WAZA_YadorigiNext:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	MESSAGE_TBL			YadorigiMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@===================================================
@	���܂񂵂Ă���
@===================================================

WAZA_Gaman:
	MESSAGE				GAMANMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@===================================================
@	���܂񂪂Ƃ��ꂽ
@===================================================

WAZA_GamanOff:
	MESSAGE				GAMANMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
@	HITCHECK			WAZA_GamanMamoruCheck,0
	HITCHECK			WAZA_NormalNohit2,0
	TYPE_CHECK
	RESET8BIT			WazaStatusFlag,WAZASTATUSFLAG_BATSUGUN|WAZASTATUSFLAG_IMAHITOTSU
	SETWORK				WazaDamage,UserWork+GAMANDAMAGE,4
	KORAERU_CHECK
	SETVALUE			UserWork+WAZA_EFF_CNT,1	@���ߌ�̋Z�G�t�F�N�g�Ăяo��
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	JUMP				WAZA_WazaEnd

.if 0
WAZA_GamanMamoruCheck:
	IF8					FLAG_EQ,ServerWork+6,MSG_MAMORUNOHIT,WAZA_MamoruMessage
	JUMP				WAZA_NormalNohit2
.endif

WAZA_GamanFaild:
	MESSAGE				GAMANMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_Umakukimaran

@======================================================
@	�ӂ��Ƃ΂��Ƃ��ق���Ƃ�
@======================================================

WAZA_Hukitobashi:
	WAZA_EFFECT
	WAZA_EFFECT_WAIT
	SIZENKAIHUKU		SIDE_DEFENCE
@	HUKITOBASHI_OIUCHI	WAZA_HikitobashiNoOiuchi
@	GOSUB				WAZA_OiuchiAttack
@	SETWORK				AttackClient,UserWork+PUSH_ATTACK_CLIENT,1
@	IF32				FLAG_BIT,ServerStatusFlag,SERVER_STATUS_FLAG_KIZETSUBIT,WAZA_HukitobashiEnd
@WAZA_HikitobashiNoOiuchi:
	POKEDELACTOR		SIDE_DEFENCE
	CLIENT_BIT_WAIT
	IF16				FLAG_BIT,FightType,FIGHT_TYPE_TRAINER,WAZA_HukitobashiReshuffle
	SETVALUE			WinLoseFlag,FIGHT_END
	ITEMEND				@�퓬�I�������܂�
WAZA_HukitobashiReshuffle:
	APPEAR_EFFECT_INIT	SIDE_DEFENCE
	APPEAR_EFFECT		SIDE_DEFENCE
	APPEAR_EFFECT_WAIT	SIDE_DEFENCE,APPEAR_NORMAL
	CLIENT_BIT_WAIT
	MESSAGE				HIKIZURIMSGNO
	POKE_RESHUFFLE_END	SIDE_DEFENCE
WAZA_HukitobashiEnd:
	JUMP				WAZA_WazaEnd

@======================================================
@	���낢����
@======================================================

WAZA_Shiroikiri:
	WAIT				MOJI_WAIT/2
	MESSAGE				SHIROIKIRIGUARDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND
	SEQRET

@======================================================
@	������
@======================================================

WAZA_Ikari:
	MESSAGE				IKARIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	���Ȃ��΂�
@======================================================

WAZA_Kanashibari:
	MESSAGE				KANASHIBARIMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_Kanashibari2:
	ALERT_MESSAGE		KANASHIBARIMSG2NO
@	WAZAEND
	ALERTEND

WAZA_KanashibariEnd:
	MESSAGE				KANASHIBARIENDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@======================================================
@	�A���R�[��
@======================================================

WAZA_EncoreEnd:
	MESSAGE				ENCOREENDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@======================================================
@	�݂��Â�
@======================================================

WAZA_Michidure:
	MESSAGE				MICHIDUREMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	SEQRET

@======================================================
@	�܂��т�
@======================================================

WAZA_MakibishiA:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	GOSUB				WAZA_Makibishi
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	KIZETSU_CHECK		SIDE_ATTACK,1,WAZA_ZenmetsuJumpA
@	JUMP				WAZA_MakibishiAfter
@	SETWORK				WazaSeqAdrs,PushWazaSeqAdrs,4
	SEQRET
WAZA_ZenmetsuJumpA:
	SETVALUE			UserWork+GETEXP_SEQNO,0
	GET_EXP				SIDE_ATTACK
	JUMP				WAZA_Zenmetsu

WAZA_MakibishiD:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	GOSUB				WAZA_Makibishi
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
	KIZETSU_CHECK		SIDE_DEFENCE,1,WAZA_ZenmetsuJumpD
@	JUMP				WAZA_MakibishiAfter
@	SETWORK				WazaSeqAdrs,PushWazaSeqAdrs,4
	SEQRET
WAZA_ZenmetsuJumpD:
	SETVALUE			UserWork+GETEXP_SEQNO,0
	GET_EXP				SIDE_DEFENCE
	JUMP				WAZA_Zenmetsu

WAZA_MakibishiK:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_KIZETSU
	HP_DEC_WAIT			SIDE_KIZETSU
	GOSUB				WAZA_Makibishi
	KIZETSU_CHECK		SIDE_KIZETSU,0,0
	KIZETSU_CHECK		SIDE_KIZETSU,1,WAZA_ZenmetsuJumpK
@	JUMP				WAZA_MakibishiAfter
@	SETWORK				WazaSeqAdrs,PushWazaSeqAdrs,4
	SEQRET
WAZA_ZenmetsuJumpK:
	SETVALUE			UserWork+GETEXP_SEQNO,0
	GET_EXP				SIDE_KIZETSU
	JUMP				WAZA_Zenmetsu

.if 0
WAZA_MakibishiA:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	GOSUB				WAZA_Makibishi
	KIZETSU_CHECK		SIDE_ATTACK
@	WAZAEND
	SEQRET

WAZA_MakibishiD:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	GOSUB				WAZA_Makibishi
	KIZETSU_CHECK		SIDE_DEFENCE
@	WAZAEND
	SEQRET

WAZA_MakibishiK:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_KIZETSU
	HP_DEC_WAIT			SIDE_KIZETSU
	GOSUB				WAZA_Makibishi
	KIZETSU_CHECK		SIDE_KIZETSU
@	WAZAEND
	SEQRET
.endif

WAZA_Makibishi:
	MESSAGE				MAKIBISHIMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	�ق�т̂���
@======================================================

WAZA_Horobinouta:
	MESSAGE				HOROBINOUTAMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	SEQEND

WAZA_Horobinoutaing:
	MESSAGE				HOROBINOUTAMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@======================================================
@	���񂵂̂�����
@======================================================

WAZA_Genshinochikara:
	CONDCHG_CHECK		SIDE_ATTACK,IFNC,COND_POW,12,WAZA_GenshinochikaraStart
	CONDCHG_CHECK		SIDE_ATTACK,IFNC,COND_DEF,12,WAZA_GenshinochikaraStart
	CONDCHG_CHECK		SIDE_ATTACK,IFNC,COND_AGI,12,WAZA_GenshinochikaraStart
	CONDCHG_CHECK		SIDE_ATTACK,IFNC,COND_SPEPOW,12,WAZA_GenshinochikaraStart
	CONDCHG_CHECK		SIDE_ATTACK,IFEQ,COND_SPEDEF,12,WAZA_GenshiEnd
WAZA_GenshinochikaraStart:
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_POW+COND_BIT_DEF+COND_BIT_AGI+COND_BIT_SPEPOW+COND_BIT_SPEDEF,COND_FLAG_UP
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_GenshiNoPowUp
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_GenshiNoPowUp:
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_GenshiNoDefUp
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_GenshiNoDefUp:
	SETVALUE			UserWork+COND_CHG_WORK,COND_AGI+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_GenshiNoAgiUp
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_GenshiNoAgiUp:
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEPOW+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_GenshiNoSpePowUp
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_GenshiNoSpePowUp:
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEDEF+PARA_RANKU1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_GenshiEnd
	MESSAGE_TBL			CondUpMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_GenshiEnd:
	SEQRET

WAZA_Kousokuspin:
	KOUSOKUSPIN
	SEQRET

WAZA_ShimeEscape:
	MESSAGE				SHIMEESCAPEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

WAZA_YadorigiEscape:
	MESSAGE				YADORIGIESCAPEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

WAZA_MakibishiEscape:
	MESSAGE				MAKIBISHIESCAPEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	�݂炢�悿
@======================================================

WAZA_Miraiyochi:
	MESSAGE				MIRAIYOCHIMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	IF8					FLAG_NE,ServerWork+5,0,WAZA_HametsunonegaiHitCheck
@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	HITCHECK			WAZA_MiraiyochiNoHit,WAZANO_MIRAIYOTI
	JUMP				WAZA_MiraiyochiStart
WAZA_HametsunonegaiHitCheck:
@�q�b�g���Ȃ�������A�w�肵���A�h���X�փW�����v
	HITCHECK			WAZA_MiraiyochiNoHit,WAZANO_HAMETUNONEGAI
WAZA_MiraiyochiStart:
@	DAMAGE_CALC
	DAMAGE_LOSS_ONLY
	IF8					FLAG_NE,ServerWork+5,0,WAZA_HametsunonegaiEff
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_MIRAIYOTI,0
	JUMP				WAZA_MiraiyochiNext
WAZA_HametsunonegaiEff:
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_HAMETUNONEGAI,0
WAZA_MiraiyochiNext:
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_DEFENCE
	CLIENT_BIT_WAIT
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_DEFENCE,0,0
@	ZENMETSU_CHECK		WAZA_DoubleNockDown
	ZENMETSU_CHECK		WAZA_MiraiyochiEnd
WAZA_MiraiyochiEnd:
@������̃{���e�[�W���グ��̂ŁAWazaKoukaCheck���Ƃ���
@	WAZAEND
	SETVALUE			UserWork+WKC_WORK0,WKC_IKARI_CHECK
	WKC_ACT				WKC_CHECK_ONLY,0
	SETVALUE			UserWork+WKC_WORK0,WKC_SOUBI_CHECK_ALL
	WKC_ACT				WKC_CHECK_KOKOMADE,WKC_HIT_FLAG_SET
@�O�ꂽ�t���O�𗎂Ƃ��Ȃ��Ǝ��́u�ق�т̂����v�������Ȃ��Ȃ�
	SETVALUE			WazaStatusFlag,0
	SEQEND

WAZA_MiraiyochiNoHit:
	WAIT				MOJI_WAIT/2
	SETVALUE			WazaStatusFlag,0
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_UMAKUKIMARAN
	WAZAKOUKA_MESSAGE
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND
@�O�ꂽ�t���O�𗎂Ƃ��Ȃ��Ǝ��́u�ق�т̂����v�������Ȃ��Ȃ�
	SETVALUE			WazaStatusFlag,0
	SEQEND

@======================================================
@	��邠����
@======================================================

WAZA_Waruagaki:
@	MESSAGE				WARUAGAKIMSGNO
	ALERT_MESSAGE		WARUAGAKIMSGNO
@	WAZAEND
	ALERTEND

@======================================================
@	PP���Ȃ�
@======================================================

WAZA_PPNone:
@	MESSAGE				PPNONEMSGNO
	ALERT_MESSAGE		PPNONEMSGNO
@	WAZAEND
	ALERTEND

@======================================================
@	PP���Ȃ��i����̋Z�������čU������O��PP���Ȃ��Ȃ����j
@======================================================

WAZA_PPNoneAttack:
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	MESSAGE				PPNONEMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	���������
@======================================================

WAZA_Ichamon:
@	MESSAGE				ICHAMONMSG2NO
	ALERT_MESSAGE		ICHAMONMSG2NO
@	WAZAEND
	ALERTEND

WAZA_Ichamon2:
	MESSAGE				ICHAMONMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	���傤�͂�
@======================================================

WAZA_Chouhatsu:
@	MESSAGE				CHOUHATSUMSG2NO
	ALERT_MESSAGE		CHOUHATSUMSG2NO
@	WAZAEND
	ALERTEND

WAZA_Chouhatsu2:
	MESSAGE				CHOUHATSUMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	�܂���
@======================================================

.if 0
WAZA_Mamoru:
	ATTACK_MESSAGE
	PP_DEC
WAZA_MamoruMessage:
	WAIT				MOJI_WAIT/2
	MESSAGE				MAMORUNOHITMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SETVALUE			ServerWork+6,MSG_MAMORUNOHIT2
	JUMP				WAZA_Umakukimaran
.endif

@======================================================
@	�˂�������
@======================================================

WAZA_Negaigoto:
	@0�͓���t���O�i0:�˂������Ƃ�����1:�˂������Ƃ����Ȃ��������j
	NEGAIGOTO			1,WAZA_NegaigotoMantan
	FIGHT_EFFECT		SIDE_DEFENCE,FE_ID_NEGAIGOTO,0
	MESSAGE				NEGAIGOTOMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	MESSAGE				JIKOSAISEIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND
WAZA_NegaigotoMantan:
	MESSAGE				NEGAIGOTOMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	WAIT				MOJI_WAIT/2
	MESSAGE				MANTANMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

@======================================================
@	�˂��͂�
@======================================================

WAZA_Newoharu:
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_NEWOHARU,0
	MESSAGE				NEWOHARUMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

WAZA_Newoharare:
	WAIT				MOJI_WAIT/2
	MESSAGE				NEWOHARAREMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	�΂�������
@======================================================

@������Ȃ������ł�������悤�ɕύX
WAZA_Bakadikara:
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_POW+COND_BIT_DEF,COND_FLAG_DOWN+COND_FLAG_MULTI+COND_FLAG_NO_TOKUSEI
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_POW,COND_FLAG_DOWN+COND_FLAG_NO_TOKUSEI
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKD1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK+NOCHECK_TOKUSEI,WAZA_BakadikaraNext
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_BakadikaraNext
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_BakadikaraNext:
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_DEF,COND_FLAG_DOWN+COND_FLAG_NO_TOKUSEI
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKD1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK+NOCHECK_TOKUSEI,WAZA_BakadikaraEnd
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_BakadikaraEnd
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_BakadikaraEnd:
	SEQRET
@�����ł�����Ȃ��Ή��������
.if 0
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_POW+COND_BIT_DEF,COND_FLAG_DOWN+COND_FLAG_MULTI
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_POW,COND_FLAG_DOWN
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKD1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_BakadikaraNext
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_BakadikaraNext
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_BakadikaraNext:
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_DEF,COND_FLAG_DOWN
	SETVALUE			UserWork+COND_CHG_WORK,COND_DEF+PARA_RANKD1
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_BakadikaraEnd
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_BakadikaraEnd
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_BakadikaraEnd:
	SEQRET
.endif

@======================================================
@	�͂������Ƃ�
@======================================================

WAZA_Hatakiotosu:
	FIGHT_EFFECT		SIDE_DEFENCE,FE_ID_FUKURO,0
	MESSAGE				HATAKIOTOSUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	�ӂ�����
@======================================================

WAZA_Huuin:
	MESSAGE				HUUINMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

WAZA_Huuin2:
	ALERT_MESSAGE		HUUINMSG2NO
@	WAZAEND
	ALERTEND

@======================================================
@	����˂�
@======================================================

WAZA_Onnen:
	MESSAGE				ONNENMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	�}�W�b�N�R�[�g
@======================================================

WAZA_MagicCort:
	ATTACK_MESSAGE
	PP_DEC
	WAIT				MOJI_WAIT/2
	MESSAGE				MAGICCORTMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NOATTACK_MES+SERVER_STATUS_FLAG_NOPP_DEC+SERVER_STATUS_FLAG_NOPP_CHECK
@	SWITCH_A_D
@���̂�тƂ܂�̂��߂�DefenceClient�����߂�
	SUBROUTINE_CALL		SIDE_ATTACK,MAGICCORT_A_D
	SEQRET

@======================================================
@	�悱�ǂ�
@======================================================

WAZA_Yokodori:
	ATTACK_MESSAGE
	PP_DEC
@	WAIT				MOJI_WAIT/2
	YOKODORI_CLIENT_SET
	FIGHT_EFFECT		SIDE_DEFENCE,FE_ID_YOKODORI,0
	MESSAGE				YOKODORIMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NOATTACK_MES+SERVER_STATUS_FLAG_NOPP_DEC+SERVER_STATUS_FLAG_NOPP_CHECK
	SWITCH_A_D
	SEQRET

@======================================================
@	���炦��
@======================================================

WAZA_Koraeta:
	MESSAGE			KORAETAMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SEQRET

@======================================================
@	���������Ђ�����
@======================================================

WAZA_Ichigeki:
	MESSAGE			ICHIGEKIMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SEQRET

@======================================================
@	�I�[�o�[�q�[�g
@======================================================

WAZA_OverHeat:
	SETVALUE			UserWork+COND_CHG_MULTI,0
	COND_CHG_BIT_CHECK	SIDE_ATTACK,COND_BIT_SPEPOW,COND_FLAG_DOWN+COND_FLAG_DOUBLE+COND_FLAG_NO_TOKUSEI
	SETVALUE			UserWork+COND_CHG_WORK,COND_SPEPOW+PARA_RANKD2
	CONDCHG				TSUIKA_DIRECT+TSUIKA_ATTACK+NOCHECK_TOKUSEI,WAZA_OverHeatEnd
	IF8					FLAG_EQ,ServerWork+5,2,WAZA_OverHeatEnd
	MESSAGE_TBL			CondDownMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_OverHeatEnd:
	SEQRET

@======================================================
@	�Z���o���O�Ƀ��b�Z�[�W���o���Z�V�[�P���X
@======================================================

@======================================================
@	�������p���`
@======================================================

WAZA_KiaiPunch:
	MESSAGE				BLANKMSGNO
	MESSAGE_WAIT		1
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_KIAI_TURN1,0
	MESSAGE				KIAIPUNCHMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQEND

@======================================================
@	�|�P������Ԉُ�V�[�P���X
@======================================================

@======================================================
@	�˂ނ���
@======================================================

WAZA_Nemuru:
	MESSAGE				SLEEPMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT		SIDE_ATTACK
	JUMP				WAZA_WazaEnd

@======================================================
@	�߂��ߏ��
@======================================================

WAZA_Mezame:
@����1��BADGE_CHECK��ʂ����߂�CHECKED�𗎂Ƃ�
	RESET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_CHECKED
	MESSAGE_TBL			SleepMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	CONDITION_GAUGE		SIDE_ATTACK
	SEQRET

WAZA_SawaguMezame:
	MESSAGE				SLEEPMSG4NO
	MESSAGE_WAIT		MOJI_WAIT
	CONDITION_GAUGE		SIDE_ATTACK
	SEQEND

@======================================================
@	�ǂ����&�ǂ��ǂ����
@======================================================

WAZA_Doku:
WAZA_Dokudoku:
	MESSAGE				DOKUMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_HPDecAttack:				 @HP�����炷�����Ȃ̂ŃV�[�P���X�����L����
	STATUS_EFFECT		SIDE_ATTACK
WAZA_HPDecAttack2:				 @HP�����炷�����Ȃ̂ŃV�[�P���X�����L����(STATUS_EFFECT���Ă΂Ȃ��j
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	KIZETSU_CHECK		SIDE_ATTACK,0,0
@	ZENMETSU_CHECK		WAZA_DoubleNockDown
	ZENMETSU_CHECK		WAZA_DokuNext
WAZA_DokuNext:
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@======================================================
@	�₯�Ǐ��
@======================================================

WAZA_Yakedo:
	MESSAGE				YAKEDOMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_HPDecAttack		@HP�����炷����V�[�P���X

@======================================================
@	��������
@======================================================

WAZA_Koori:
	MESSAGE				KOORIMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT		SIDE_ATTACK
	JUMP				WAZA_WazaEnd

@======================================================
@	�Ƃ������
@======================================================

WAZA_Melt:
	MESSAGE_TBL			MeltMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	CONDITION_GAUGE		SIDE_ATTACK
	SEQRET

@======================================================
@	�Ƃ������
@======================================================

WAZA_Melt2:
	MESSAGE				KOORIMSG3NO
	MESSAGE_WAIT		MOJI_WAIT
	CONDITION_GAUGE		SIDE_DEFENCE
	SEQRET

@======================================================
@	�܂Џ��
@======================================================

WAZA_Mahi:
	MESSAGE				MAHIMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT		SIDE_ATTACK
	JUMP				WAZA_WazaEnd


@======================================================
@	�Ђ�񂾏��
@======================================================

WAZA_Hirumu:
	MESSAGE				HIRUMUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	���킢�ł�����
@======================================================

WAZA_Sawagu:
	MESSAGE_TBL			SawaguMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@======================================================
@	���΂ꂨ���������
@======================================================

WAZA_AbareruEnd:
	STATUS_EFFECT_DIRECT	SIDE_ATTACK,CONDNO_COND2,CONDITION2_KONRAN
	MESSAGE				ABARERUENDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@======================================================
@	��������
@======================================================

WAZA_Konran:
	MESSAGE				KONRANMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT2		SIDE_ATTACK,CONDITION2_KONRAN
	IF8					FLAG_EQ,ServerWork+5,0,WAZA_konranRet
WAZA_KonranAct:
	SUBROUTINE_CALL		SIDE_ATTACK,SERVER_KEEP_OFF
	DAMAGE_LOSS_ONLY
	MESSAGE				KONRANMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	WAZAKOUKA_SE
	POKE_BLINK			SIDE_ATTACK
	CLIENT_BIT_WAIT
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	KIZETSU_CHECK		SIDE_ATTACK,0,0
	JUMP				WAZA_WazaEnd
WAZA_konranRet:
	SEQRET

@======================================================
@	������I��
@======================================================

WAZA_KonranEnd:
	MESSAGE				KONRANMSG3NO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	�˂��ɂ��΂�
@======================================================

WAZA_Nekonikoban:
	MESSAGE				NEKONIKOBANMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	���ߋZ�n
@======================================================

WAZA_Shime:
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_SIMETUKE,UserWork+FIGHT_EFFECT_WORK
	MESSAGE				SHIMEKEIZOKUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_HPDecAttack2		@HP�����炷����V�[�P���X

WAZA_ShimeEnd:
	MESSAGE				SHIMEENDMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@======================================================
@	��������
@======================================================

WAZA_Meromero:
	MESSAGE				MEROMEROMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT2		SIDE_ATTACK,CONDITION2_MEROMERO
	SEQRET

WAZA_MeromeroNext:
	MESSAGE				MEROMEROMSG3NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	������
@======================================================

WAZA_Akumu:
	MESSAGE				AKUMUMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT2		SIDE_ATTACK,CONDITION2_AKUMU
	JUMP				WAZA_HPDecAttack2		@HP�����炷����V�[�P���X

@======================================================
@	�̂낢
@======================================================

WAZA_Noroi:
	MESSAGE				NOROIMSG2NO
	MESSAGE_WAIT		MOJI_WAIT
	STATUS_EFFECT2		SIDE_ATTACK,CONDITION2_NOROI
	JUMP				WAZA_HPDecAttack2		@HP�����炷����V�[�P���X

@======================================================
@	�܂Ђ��Ȃ���
@======================================================

WAZA_NoMahi:
	MESSAGE				NOMAHIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	CONDITION_GAUGE		SIDE_DEFENCE
	SEQRET

.if 0
@======================================================
@	�����񂮂��
@======================================================

WAZA_KaengurumaMelt:
	GOSUB				WAZA_Kaenguruma
	GOSUB				WAZA_Melt
	CONDITION_GAUGE		SIDE_ATTACK
	SEQRET

WAZA_Kaenguruma:
	SETVALUE			ServerWork+3,COND_YAKEDO
	TSUIKA_CHECK
	SEQRET
.endif

@======================================================
@	�Z�ǉ����ʃV�[�P���X
@======================================================

@======================================================
@	�˂ނ�
@======================================================

WAZA_AddNemuri:
@	MESSAGE				SLEEPBEGINMSGNO
	STATUS_EFFECT		SIDE_TSUIKA
	MESSAGE_TBL			AddNemuriMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
WAZA_ConditionSet:
	CONDITION_GAUGE		SIDE_TSUIKA
	CLIENT_BIT_WAIT
	SEQRET

WAZA_AddNemuri2:
	STATUS_EFFECT		SIDE_TSUIKA
	MESSAGE				SLEEPBEGINMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	CONDITION_GAUGE		SIDE_TSUIKA
	CLIENT_BIT_WAIT
	VANISH_OFF			SIDE_TSUIKA
@	WAZAEND								@WazaKoukaCheck�����Ȃ��悤�ɂ���΍�
	SEQEND

@======================================================
@	�ǂ�
@======================================================

WAZA_AddDoku:
@	MESSAGE				DOKUMSG1NO
	STATUS_EFFECT		SIDE_TSUIKA
	MESSAGE_TBL			AddDokuMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_ConditionSet

@======================================================
@	�₯��
@======================================================

WAZA_AddYakedo:
@	MESSAGE				YAKEDOMSG1NO
	STATUS_EFFECT		SIDE_TSUIKA
	MESSAGE_TBL			AddYakedoMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_ConditionSet

@======================================================
@	������
@======================================================

WAZA_AddKoori:
@	MESSAGE				KOORIMSG1NO
	STATUS_EFFECT		SIDE_TSUIKA
	MESSAGE_TBL			AddKooriMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_ConditionSet

@======================================================
@	�܂�
@======================================================

WAZA_AddMahi:
@	MESSAGE				MAHIMSG1NO
	STATUS_EFFECT		SIDE_TSUIKA
	MESSAGE_TBL			AddMahiMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_ConditionSet

@======================================================
@	���킮
@======================================================

WAZA_AddSawagu:
	MESSAGE				SAWAGUMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	�ǂ��ǂ�
@======================================================

WAZA_AddDokudoku:
	STATUS_EFFECT		SIDE_TSUIKA
	MESSAGE				DOKUMSG4NO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_ConditionSet

@======================================================
@	�˂��ɂ��΂�
@======================================================

WAZA_AddNekonikoban:
	MESSAGE				NEKONIKOBANMSG1NO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	���ߋZ�n
@======================================================

WAZA_AddShime:
	MESSAGE_TBL			ShimeMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	������
@======================================================

WAZA_AddKonran:
	STATUS_EFFECT_DIRECT	SIDE_TSUIKA,CONDNO_COND2,CONDITION2_KONRAN
	MESSAGE				KONRANMSG4NO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	�͂�ǂ�
@======================================================

WAZA_AddHanekaeri:
@��邠�����́A���Ȃ炸�͂˂�����
	IF16				FLAG_EQ,WazaNo,WAZANO_WARUAGAKI,WAZA_AddHanekaeriStart
@����\�͂��������܂́A�͂˂�������󂯂Ȃ�
	SPEABI_CHECK		SIDE_ATTACK,TOKUSYU_ISIATAMA,WAZA_AddHanekaeriEnd
WAZA_AddHanekaeriStart:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	MESSAGE				HANEKAERIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_ATTACK,0,0
WAZA_AddHanekaeriEnd:
	SEQRET

@======================================================
@	�ǂ�ڂ�
@======================================================

WAZA_AddDorobou:
	FIGHT_EFFECT		SIDE_DEFENCE,FE_ID_NUSUMU,0
	MESSAGE				DOROBOUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	����\�̓V�[�P���X
@======================================================

@======================================================
@	�U���̃^�[���Ŕ������Ȃ�����\�͂�SPEABIEND���g�p����
@======================================================

@======================================================
@	���߂ӂ炵
@======================================================

WAZA_Amehurashi:
	WAIT				MOJI_WAIT/2
	MESSAGE				AMEHURASHIMSGNO
	CLIENT_BIT_WAIT
	FIGHT_EFFECT		SIDE_FIELD,FE_ID_AMAGOI,0
	GOSUB				WAZA_TenkiyaCheck
	SPEABIEND

@======================================================
@	������������
@======================================================

WAZA_Kasokusouti:
	FIGHT_EFFECT		SIDE_ATTACK,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
	MESSAGE				KASOKUSOUTIMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SPEABIEND

@======================================================
@	�g���[�X
@======================================================

WAZA_Trace:
	WAIT				MOJI_WAIT/2
	MESSAGE				TRACEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SPEABIEND

@======================================================
@	���߂�������
@======================================================

WAZA_Ameukezara:
	MESSAGE			AMEUKEZARAMSGNO
	MESSAGE_WAIT	MOJI_WAIT
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT		ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC			SIDE_ATTACK
	HP_DEC_WAIT		SIDE_ATTACK
	SPEABIEND

@======================================================
@	���Ȃ�����
@======================================================

WAZA_Sunaokosi:
	WAIT				MOJI_WAIT/2
	MESSAGE				SUNAOKOSIMSGNO
	CLIENT_BIT_WAIT
	FIGHT_EFFECT		SIDE_FIELD,FE_ID_SUNAARASI,0
	GOSUB				WAZA_TenkiyaCheck
	SPEABIEND

@======================================================
@	������
@======================================================

WAZA_Dappi:
	MESSAGE			DAPPIMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_ATTACK
	SPEABIEND

@======================================================
@	�Ă񂫂�
@======================================================

WAZA_TenkiyaCheck:
	SETVALUE		UserWork+CLIENT_NO_WORK,0
WAZA_TenkiyaCheckLoop:
	TENKIYA_CHECK
	ADDVALUE		UserWork+CLIENT_NO_WORK,1
	IFWNE			UserWork+CLIENT_NO_WORK,ClientSetMax,1,WAZA_TenkiyaCheckLoop
WAZA_TenkiyaCheckEnd:
	SEQRET

WAZA_Tenkiya:
	GOSUB			WAZA_Tenkiya2
	SPEABIEND

WAZA_Tenkiya2:
	TENKIYA
	CLIENT_BIT_WAIT
	MESSAGE			TENKIYAMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SEQRET

@======================================================
@	������
@======================================================

WAZA_Ikaku:
	GOSUB				WAZA_IkakuAct2
	SPEABIEND

WAZA_IkakuAct2:
	WAIT				MOJI_WAIT/2
WAZA_IkakuAct:
	SETVALUE			DefenceClient,0
	SETVALUE			UserWork+COND_CHG_WORK,COND_POW+PARA_RANKD1
WAZA_IkakuLoop:
	IKAKU				WAZA_IkakuEnd
	CONDITION2_CHECK	SIDE_DEFENCE,CONDITION2_MIGAWARI,WAZA_IkakuNoMsg
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_KURIABODHI,WAZA_NoIkaku
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_KAIRIKIBASAMI,WAZA_NoIkaku
	SPEABI_CHECK		SIDE_DEFENCE,TOKUSYU_SIROIKEMURI,WAZA_NoIkaku
	CONDCHG				TSUIKA_DIRECT+IKAKU_CONDCHG,WAZA_IkakuNoMsg
	IF8					FLAG_C,ServerWork+5,1,WAZA_IkakuNoMsg
	COND_CHG_WORK_SET
	FIGHT_EFFECT		SIDE_DEFENCE,FE_ID_STATUS_UPDOWN,UserWork+FIGHT_EFFECT_WORK
	MESSAGE				IKAKUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
WAZA_IkakuNoMsg:
	ADDVALUE			DefenceClient,1
	JUMP				WAZA_IkakuLoop
WAZA_IkakuEnd:
	SEQRET
WAZA_NoIkaku:
	WAIT				MOJI_WAIT/2
	MESSAGE				CLEARBODYNOIKAKUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_IkakuNoMsg

@======================================================
@	�Ђł�
@======================================================

WAZA_Hideri:
	WAIT				MOJI_WAIT/2
	MESSAGE				HIDERIMSGNO
	CLIENT_BIT_WAIT
	FIGHT_EFFECT		SIDE_FIELD,FE_ID_NIHONBARE,0
	GOSUB				WAZA_TenkiyaCheck
	SPEABIEND

@======================================================
@	�Ђ炢����
@======================================================

WAZA_Hiraishin:
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	MESSAGE				HIRAISHINMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NOATTACK_MES
	SEQRET

@======================================================
@	�U���̃^�[���̓r���Ŕ������A
@	���̂܂܃V�[�P���X���I�������Ă悢����\�͂�WAZAEND���g�p����
@======================================================

@======================================================
@	���񂶂傤
@======================================================

WAZA_Ganzyou:
	WAIT				MOJI_WAIT/2
	MESSAGE				GANZYOUMSGNO
	WAIT				MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	���߂肯
@======================================================

WAZA_Shimerike:
	WAIT				MOJI_WAIT/2
	MESSAGE				SHIMERIKEMSGNO
	WAIT				MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	�����ł񁕂��傷��
@======================================================

WAZA_TikudenPPDec:
WAZA_TyosuiPPDec:
	PP_DEC
WAZA_Tikuden:
WAZA_Tyosui:
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC				SIDE_DEFENCE
	HP_DEC_WAIT			SIDE_DEFENCE
	MESSAGE				HPKAIFUKUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_KOUKANAI
	JUMP				WAZA_WazaEnd

WAZA_NOKaifukuPPDec:
	PP_DEC
WAZA_NOKaifuku:
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	MESSAGE				NOHPKAIFUKUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SET8BIT				WazaStatusFlag,WAZASTATUSFLAG_KOUKANAI
	JUMP				WAZA_WazaEnd

@======================================================
@	���炢��
@======================================================

WAZA_MoraibiPPDec:
	PP_DEC
WAZA_Moraibi:
	ATTACK_MESSAGE
	WAIT				MOJI_WAIT/2
	MESSAGE_TBL			MoraibiMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	���イ�΂�
@======================================================

WAZA_Kyuuban:
	WAIT				MOJI_WAIT/2
	MESSAGE				KYUUBANMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	�N���A�{�f�B
@======================================================

WAZA_ClearBody:
	WAIT				MOJI_WAIT/2
	MESSAGE				CLEARBODYMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND
	SEQRET

@======================================================
@	�݂��̃x�[��
@======================================================

WAZA_Mizunobeeru:
	WAIT				MOJI_WAIT/2
	MESSAGE_TBL			MizunobeeruMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	���イ�Ȃ�
@======================================================

WAZA_Juunan:
	WAIT				MOJI_WAIT/2
	MESSAGE_TBL			JuunanMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET			

@======================================================
@	�߂񂦂�
@======================================================

WAZA_Meneki:
	WAIT				MOJI_WAIT/2
	MESSAGE_TBL			MenekiMsgTbl
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	�ǂ񂩂�
@======================================================

WAZA_Donkan:
	WAIT				MOJI_WAIT/2
	MESSAGE				DONKANMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	���������傭
@======================================================

WAZA_Seisinryoku:
	WAIT				MOJI_WAIT/2
	MESSAGE				SEISINRYOKUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	�}�C�y�[�X
@======================================================

WAZA_Mypace:
	WAIT				MOJI_WAIT/2
	MESSAGE				MYPACEMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	�ڂ�����
@======================================================

WAZA_Bouon:
	ATTACK_MESSAGE
	PP_DEC
	WAIT				MOJI_WAIT/2
	MESSAGE				BOUONMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	COND_??�n�������Ȃ����ʂȂ̂ŁA���[�`�������L
@	����ǂ���
@	�����肫�o�T�~
@======================================================

WAZA_Surudoime:
WAZA_Kairikibasami:
	WAIT				MOJI_WAIT/2
	MESSAGE				NOCONDDOWNMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND
	SETVALUE			ServerWork+5,3
	SEQRET				@�u��������v�Ƃ������p�����[�^�ω��n��
						@�r���ł�߂Ȃ��悤�ɂ��邽��

@======================================================
@	�˂񂿂Ⴍ
@======================================================

WAZA_Nentyaku:
	WAIT				MOJI_WAIT/2
	MESSAGE				TOKUSEINOWAZAMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	JUMP				WAZA_WazaEnd

@======================================================
@	�U���̏I���Ŕ����������\�͂�SEQEND���g�p����
@	���邢�́A�r���ł��T�u���[�`���W�����v��
@	�Ă΂ꂽ���̂�SEQRET���g�p����
@======================================================

@======================================================
@	�ւ񂵂傭
@======================================================

WAZA_Hensyoku:
	MESSAGE				HENSYOKUMSGNO
	MESSAGE_WAIT		MOJI_WAIT
@	WAZAEND
	SEQRET

@======================================================
@	���߂͂�
@======================================================

WAZA_Samehada:
@������HP�����炷�̂��݂���肪�󂯂Ȃ��悤�ɂ���
	SET32BIT			ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI+SERVER_STATUS_FLAG_NO_ZOKUSEI
	HP_DEC				SIDE_ATTACK
	HP_DEC_WAIT			SIDE_ATTACK
	MESSAGE				SAMEHADAMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	KIZETSU_CHECK		SIDE_ATTACK,0,0
@	WAZAEND
	SEQRET

@======================================================
@	���������{�f�B
@======================================================

WAZA_MeromeroBody:
	STATUS_EFFECT2		SIDE_ATTACK,CONDITION2_MEROMERO
	MESSAGE				SPEABIMEROMEROMSGNO
	MESSAGE_WAIT		MOJI_WAIT
	SEQRET

@======================================================
@	����\�͂ق����́A�ǂ��A�܂ЁA�˂ނ��ǉ�����̂ŁA���[�`�������L����
@	����\�͂ǂ��̃g�Q�́A�ǂ���ǉ�����̂ŁA���[�`�������L����
@	����\�̓V���N���́A�ǉ����ʂ�Ԃ��̂ŁA���[�`�������L����
@	����\�͂����ł񂫂́A�܂Ђ�ǉ�����̂ŁA���[�`�������L����
@	����\�͂ق̂��̂��炾�́A�₯�ǂ�ǉ�����̂ŁA���[�`�������L����
@======================================================

WAZA_Housi:
WAZA_Dokunotoge:
WAZA_Seidenki:
WAZA_Honoonokarada:
	CLIENT_BIT_WAIT
	TSUIKA_INDIRECT_ACT
	SEQRET

WAZA_Sinkuro:
	CLIENT_BIT_WAIT
	TSUIKA_DIRECT_ACT
	SEQRET

@======================================================
@	�������ʂŏ�Ԉُ����
@======================================================

WAZA_SpeabiNoJoutaiijou:
	MESSAGE			SPEABINOJOUTAIIJOUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_CLIENTNOWORK
	SEQRET

@======================================================
@	���l�̃|�P�������������Ƃ𕷂��Ȃ�����
@======================================================

WAZA_OrderIgnoreSleep:
	MESSAGE			ORDERIGNORESLEEPMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SETVALUE		UserWork+WKC_WORK0,0
	WKC_ACT			WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	WAZAEND

WAZA_OrderIgnore:
	MESSAGE			ORDERIGNOREMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	WAZASEQADRS		0

WAZA_Namakeru:
	MESSAGE_TBL		NamaketeiruMsgTbl
	MESSAGE_WAIT	MOJI_WAIT
	SETVALUE		UserWork+WKC_WORK0,0
	WKC_ACT			WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	WAZAEND

WAZA_Hirune:
	MESSAGE			HIRUNEMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SETVALUE		ServerWork+3,COND_NEMURI+TSUIKA_ATTACK
	TSUIKA_DIRECT_ACT
	SETVALUE		UserWork+WKC_WORK0,0
	WKC_ACT			WKC_CHECK_KOKOMADE,WKC_DOUBLE_ATTACK
	WAZAEND

WAZA_OwnAttack:
	MESSAGE			IUKOTOWOKIKANAIMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	JUMP			WAZA_KonranAct

@======================================================
@	�݂����̋C��
@======================================================

WAZA_MigawariKizetsu:
	FIGHT_EFFECT	SIDE_DEFENCE,FE_ID_MIGAWARI_KIZETU,0
	MESSAGE			MIGAWARIENDMSGNO
	SEQRET

@======================================================
@	�����A�C�e������
@======================================================

WAZA_ItemNoMahi:
	GOSUB			WAZA_ItemNoMahi2
	SEQEND
WAZA_ItemNoMahi2:
	FIGHT_EFFECT	SIDE_CLIENTNOWORK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMNOMAHIMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_CLIENTNOWORK
	KILLITEM		SIDE_CLIENTNOWORK
	SEQRET

WAZA_ItemNoDoku:
	GOSUB			WAZA_ItemNoDoku2
	SEQEND

WAZA_ItemNoDoku2:
	FIGHT_EFFECT	SIDE_CLIENTNOWORK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMNODOKUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_CLIENTNOWORK
	KILLITEM		SIDE_CLIENTNOWORK
	SEQRET

WAZA_ItemNoYakedo:
	GOSUB			WAZA_ItemNoYakedo2
	SEQEND

WAZA_ItemNoYakedo2:
	FIGHT_EFFECT	SIDE_CLIENTNOWORK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMNOYAKEDOMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_CLIENTNOWORK
	KILLITEM		SIDE_CLIENTNOWORK
	SEQRET

WAZA_ItemNoKoori:
	GOSUB			WAZA_ItemNoKoori2
	SEQEND

WAZA_ItemNoKoori2:
	FIGHT_EFFECT	SIDE_CLIENTNOWORK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMNOKOORIMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_CLIENTNOWORK
	KILLITEM		SIDE_CLIENTNOWORK
	SEQRET

WAZA_ItemNoNemuri:
	GOSUB			WAZA_ItemNoNemuri2
	SEQEND

WAZA_ItemNoNemuri2:
	FIGHT_EFFECT	SIDE_CLIENTNOWORK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMNONEMURIMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_CLIENTNOWORK
	KILLITEM		SIDE_CLIENTNOWORK
	SEQRET

WAZA_ItemNoKonran:
	GOSUB			WAZA_ItemNoKonran2
	SEQEND

WAZA_ItemNoKonran2:
	FIGHT_EFFECT	SIDE_CLIENTNOWORK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMNOKONRANMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	KILLITEM		SIDE_CLIENTNOWORK
	SEQRET

WAZA_ItemNoJoutaiijou:
	GOSUB			WAZA_ItemNoJoutaiijou2
	SEQEND

WAZA_ItemNoJoutaiijou2:
	FIGHT_EFFECT	SIDE_CLIENTNOWORK,FE_ID_KAIFUKU,0
@	MESSAGE			ITEMNOJOUTAIIJOUMSGNO
	MESSAGE_TBL		ItemMultiRecoverTbl
	MESSAGE_WAIT	MOJI_WAIT
	CONDITION_GAUGE	SIDE_CLIENTNOWORK
	KILLITEM		SIDE_CLIENTNOWORK
	SEQRET

WAZA_ItemAbiKaifuku:
	GOSUB			WAZA_ItemAbiKaifuku2
	SEQEND

WAZA_ItemAbiKaifuku2:
	FIGHT_EFFECT	SIDE_CLIENTNOWORK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMABIKAIFUKUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	KILLITEM		SIDE_CLIENTNOWORK
	SEQRET

WAZA_ItemHPKaifuku:
	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMHPKAIFUKUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SET32BIT		ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC			SIDE_ATTACK
	HP_DEC_WAIT		SIDE_ATTACK
	KILLITEM		SIDE_ATTACK
	SEQEND

WAZA_ItemPPKaifuku:
	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMPPKAIFUKUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	KILLITEM		SIDE_ATTACK
	SEQEND

WAZA_ItemHPKaifukuEveryTurn:
	GOSUB			WAZA_ItemHPKaifukuEveryTurn2
	SEQEND

WAZA_ItemHPKaifukuEveryTurn2:
	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMHPKAIFUKUEVERYTURNMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SET32BIT		ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC			SIDE_ATTACK
	HP_DEC_WAIT		SIDE_ATTACK
	SEQRET

WAZA_ItemOnaziWazaOnly:
	ALERT_MESSAGE	ITEMONAZIWAZAONLYMSGNO
	ALERTEND

WAZA_ItemKoraeta:
	FIGHT_EFFECT	SIDE_DEFENCE,FE_ID_KIAINOHATIMAKI,0
	MESSAGE			ITEMKORAETAMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SEQRET

WAZA_ItemHPKaifukuKonran:
	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMHPKAIFUKUMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SET32BIT		ServerStatusFlag,SERVER_STATUS_FLAG_NO_MIGAWARI
	HP_DEC			SIDE_ATTACK
	HP_DEC_WAIT		SIDE_ATTACK
	MESSAGE			SEEDKONRANMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	SETVALUE		ServerWork+3,COND2_KONRAN+TSUIKA_ATTACK
	TSUIKA_DIRECT_ACT
	KILLITEM		SIDE_ATTACK
	SEQEND

WAZA_ItemCondUp:
	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_KAIFUKU,0
	CONDCHG			TSUIKA_DIRECT+TSUIKA_ATTACK,WAZA_ItemNoCondUp
WAZA_ItemNoCondUp:
	SETVALUE		ServerWork+5,4
	GOSUB			WAZA_CondUpTsuikaMsg
	KILLITEM		SIDE_ATTACK
	SEQEND

WAZA_ItemCriticalUp:
	FIGHT_EFFECT	SIDE_ATTACK,FE_ID_KAIFUKU,0
	MESSAGE			ITEMCRITICALUPMSGNO
	MESSAGE_WAIT	MOJI_WAIT
	KILLITEM		SIDE_ATTACK
	SEQEND

@======================================================
@	�ʐM���A���[�g���b�Z�[�W
@======================================================

WAZA_DouguAlert:
	ALERT_MESSAGE	DOUGUALERTMSGNO
	ALERTEND

@======================================================
@	���b�Z�[�W������
@======================================================

WAZA_BlankMsg:
	MESSAGE			BLANKMSGNO
	SEQRET

