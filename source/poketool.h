
#ifndef	__POKETOOL_H_
#define	__POKETOOL_H_


#include	"param.h"
#include	"poketool.def"
#include	"chr.def"

extern	const	u32	GrowTbl[8][101];

extern	void	PokeParaCalc(PokemonParam *);
extern	void	PokeReplace(PokemonPasoParam *,PokemonParam *);
extern	void	PokeActHeaderMake(u16,u8);
extern	void	TrainerActHeaderMake(u16,u8);
extern	u8		AddPokemon(PokemonParam *);
extern	void	AddPokemonAct(PokemonParam *,PokemonParam *,int);
extern	u8		Poke2vs2Check(void);
extern	void	PokeWazaOboe(PokemonParam *);
extern	u16		PokeWazaOboeCheck(PokemonParam *,u8);

extern	u8		PokeCountMineGet();
extern	u8		PokeCountEnemyGet();

extern void PokePasoParaInit(PokemonPasoParam *PPP);
extern	void	PokeParaInit(PokemonParam *);
extern	void	PokeParaMineInit(void);
extern	void	PokeParaEnemyInit(void);

extern	u16		PokeWazaSet(PokemonParam *,u16);
extern  u8		PokeOboeruWazaAllGet(u16 monsno,u16 *nobuf);
extern	void	PokeWazaSetPos(PokemonParam *,u16,u8);
extern	u16		PokeWazaSetServer(PokemonServerParam *,u16);
extern	void	PokeWazaSetPosServer(PokemonServerParam *,u16,u8);
extern	void	PokePPCountClear(PokemonParam *,u8);
extern	void	PokePPCountClearServer(PokemonServerParam *,u8);
extern	s32		WazaDamageCalc(PokemonServerParam *,PokemonServerParam *,int,u16,u16,u8,u8,u8);
extern	u8 		HitCountCheck(u8);
extern	u8 		GetDefenceClient(u8);
extern	u8		PPCountGet(PokemonParam *,int);
extern	u8		PokeSexGet(PokemonParam *);
extern	u8		PokePasoSexGet(PokemonPasoParam *);
extern	u8		PokeSexGetMonsNo(u16 no,u32 rnd);
extern	u8		PokeLevelCalc(PokemonParam *);
extern	u8		PokePasoLevelCalc(PokemonPasoParam *);

extern	PokemonParam	PokeParaMine[6];
extern	PokemonParam	PokeParaEnemy[6];
extern	u8	PokeCountMine;
extern	u8	PokeCountEnemy;
extern	const u8 CondChgTable[][2];
extern	actHeader	PokeActHeader;

extern	const	u8	PokeNameData[][6];		//from monsname.h
extern	const	u8	WazaNameData[][8];		//from wazaname.h

extern	u16	wazapow;
extern	u8	BattlePokeAnmNo[4];

extern	u16	OboeWazaNo;

//=============================================================
//
//	PokeParaSet(�|�P�����̃p�����[�^���Z�b�g����j
//
//	�����F
//	PP	=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno	=�@�|�P�����i���o�[
//	pokelevel�@=�@�|�P�����̃��x��
//	pow		=	�Z�b�g����p���[����(POW_RND�̎��͗����ŃZ�b�g�j
//	rndflag =	�������̃Z�b�g�̗L���i0:�����ŃZ�b�g1:����u32���Z�b�g�j
//	rnd		=	�Z�b�g����������i0�̎��͗����ŃZ�b�g�j
//
//=============================================================

extern	void	PokeParaSet(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 rndflag,u32 rnd,u8 idflag,u32 id);

extern	void	PokePasoParaSet(PokemonPasoParam *PPP,u16 pokeno,u8 pokelevel,u8 pow,u8 rndflag,u32 rnd,u8 idflag,u32 id);

//=============================================================
//
//	PokeParaSetChr(�|�P�����̃p�����[�^���Z�b�g����B���i�����߂����Łj
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//	pow			=	�Z�b�g����p���[����(POW_RND�̎��͗����ŃZ�b�g�j
//	chr			=	�Z�b�g���鐫�i�i�O�`�Q�S�jchr.def�ɒ�`����Ă܂�
//
//=============================================================

extern	void	PokeParaSetChr(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 chr);

//=============================================================
//
//	PokeParaSetSexChr(�|�P�����̃p�����[�^���Z�b�g����B���ʁA���i�����߂����Łj
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//	pow			=	�Z�b�g����p���[����(POW_RND�̎��͗����ŃZ�b�g�j
//	chr			=	�Z�b�g���鐫�i�i�O�`�Q�S�jchr.def�ɒ�`����Ă܂�
//	annon		=	�A���m�[���̌`����i0:�֌W�����j
//
//=============================================================

extern	void	PokeParaSetSexChr(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 sex,u8 chr,u8 annon);

//=============================================================
//
//	PokeParaSetDemo(�|�P�����̃p�����[�^���Z�b�g����B�ߊl�f����p�j
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//
//=============================================================

extern	void	PokeParaSetDemo(PokemonParam *PP,u16 pokeno,u8 pokelevel);

//===============================================================
//
//	�|�P�����p�����[�^�Q�b�g�֐�
//
// in :	PP	�Q�b�g����|�P�����p�����[�^�\���̂̃|�C���^
//		id	�Q�b�g����p�����[�^��ID�ipoketool.def�Q�Ɓj
//		...	���ۂ�char�^�̃|�C���^������inickname�Ȃǂ̔z��j
//			�K�v�̂Ȃ��Ƃ��͂Ȃ��Ă��悢
//===============================================================

extern	u32		PokeParaGet(PokemonParam *PP,int ,...);

//===============================================================
//
//	�|�P�����p�����[�^�Q�b�g�֐��i�p�\�R���p�j
//
// in :	PPP	�Q�b�g����|�P�����p�����[�^�\���̂̃|�C���^
//		id	�Q�b�g����p�����[�^��ID�ipoketool.def�Q�Ɓj
//		...	���ۂ�char�^�̃|�C���^������inickname�Ȃǂ̔z��j
//			�K�v�̂Ȃ��Ƃ��͂Ȃ��Ă��悢
//===============================================================

extern	u32		PokePasoParaGet(PokemonPasoParam *PPP,int ,...);

//===============================================================
//
//	�|�P�����p�����[�^�v�b�g�֐�
//
// in :	PP	�v�b�g����|�P�����p�����[�^�\���̂̃|�C���^
//		id	�v�b�g����p�����[�^��ID�ipoketool.def�Q�Ɓj
//		buf	�v�b�g����f�[�^�̃|�C���^������
//===============================================================

extern	void	PokeParaPut(PokemonParam *PP,int,const u8 *);

//===============================================================
//
//	�|�P�����p�����[�^�v�b�g�֐��i�p�\�R���p�j
//
// in :	PPP	�v�b�g����|�P�����p�����[�^�\���̂̃|�C���^
//		id	�v�b�g����p�����[�^��ID�ipoketool.def�Q�Ɓj
//		buf	�v�b�g����f�[�^�̃|�C���^������
//===============================================================

extern	void	PokePasoParaPut(PokemonPasoParam *PPP,int,const u8 *);

//===============================================================
//
//	�����ɂ��Ă������\�͂��擾����֐�
//
// in :	monsno	�擾����|�P�����i���o�[
//		abiflag	�擾�������\�͂̃t���O�i 0 or 1 �j
// out:	����\�̓i���o�[(tokusyu.def�Q�Ɓj
//===============================================================

extern	u8		SpecialAbirityGet(u16 monsno,u8 abiflag);

//===============================================================
//
//	�����ɂ��Ă������\�͂��擾����֐�
//
// in :	PokemonParam	�擾����|�P�����̍\���̂̃|�C���^
// out:	����\�̓i���o�[(tokusyu.def�Q�Ɓj
//===============================================================

extern	u8		SpecialAbirityNoGet(PokemonParam *);

//===============================================================
//
//	������ԑΐ�p�g���[�i�[�f�[�^�Z�b�g�֐�
//
// in :	SBD	�Z�b�g����閧��n�f�[�^�̃|�C���^
//===============================================================

extern	void	OrusubanTaisenSet(SECRET_BASE_DATA *SBD);

//===============================================================
//		������ԑΐ�̃g���[�i�[�O���t�B�b�NID��Ԃ�
//===============================================================

extern	u8		OrusubanChrGet(void);

//===============================================================
//		������ԑΐ�̃g���[�i�[�^�C�vID��Ԃ�
//===============================================================

extern	u8		OrusubanTypeGet(void);

//===============================================================
//
//	�A�C�e�����g�p���ă|�P�����̃X�e�[�^�X�ُ���񕜂���
//
// in :	PP	�񕜂���|�P�����f�[�^�̃|�C���^
// 		itemno	�g�p����A�C�e��
// 		pos		�|�P�������X�g�ŉ��Ԗڂ̃|�P���������w��
// 		pos2	�Z���X�g�ŉ��Ԗڂ̋Z�����w���iPP�Ƃ�����j
// out:	0	�񕜐���
// 		1	�g�p���Ă����ʂ��Ȃ�
//===============================================================

//extern	u8	StatusRecover(PokemonParam *PP,u16 itemno,u8 pos,u8 pos2);
extern	u8	StatusRecover(PokemonParam *PP,u16 itemno,u8 pos,u8 pos2);
extern	u8	StatusRecover2(PokemonParam *PP,u16 itemno,u8 pos,u8 pos2,u8 hpflag);

//===============================================================
//
//	�����X�^�[�{�[���𓊂���邩�̃`�F�b�N
//
// out:	0	������܂�
// 		1	������܂���
//===============================================================

extern	u8	BallThrowCheck(void);
extern	u8	BallThrowCheckSafari(void);

//===============================================================
//
//	�����X�^�[�l�[�����w��o�b�t�@�ɃR�s�[
//
//	in : buf	�R�s�[����o�b�t�@
//		 monsno	�|�P�����i���o�[
//===============================================================

extern	u8	PokeNameSet(u8 *buf,u16 monsno);

//===============================================================
//
//	PPMax���Q�b�g
//
//	in : wazano	�Q�b�g����Z�i���o�[
//		 maxup	�A�C�e���ɂ��}�b�N�X�A�b�v�J�E���^�[
//		 pos	�J�E���^�[�ʒu�iu8��2�r�b�g���g�p���Ă���̂Łj
//===============================================================

extern	u8	PPMaxGet(u16 wazano,u8 maxup,u8 pos);

//===============================================================
//
//	�퓬�Ŏg����A�C�e�����`�F�b�N���ă��b�Z�[�W�̃|�C���^��Ԃ�
//	�i�`�F�b�N�͂Ȃ��Ȃ��āA�P���Ƀ��b�Z�[�W���Q�b�g����֐��ɂȂ�܂����j
//
//	in : itemno	�g���A�C�e���i���o�[
//
//	out: �\�����郁�b�Z�[�W�̃|�C���^
//===============================================================

extern	u8	*FightItemUseMsgGet(u16 itemno);

//===============================================================
//
//	�|�P�����̐��i���Q�b�g
//
//	in : PP	�Q�b�g����|�P�����̍\���̂̃|�C���^
//
//	out: ���i�i���o�[
//===============================================================

extern	u8	PokeSeikakuGet(PokemonParam *PP);

//===============================================================
//
//	�|�P�����̐��i���Q�b�g�i�������������ɂ���j
//
//	in : rnd	������
//
//	out: ���i�i���o�[
//===============================================================

extern	u8	PokeSeikakuGetRnd(u32 rnd);

//===============================================================
//
//	�i���`�F�b�N
//
//	in : PP		�`�F�b�N����|�P�����̍\���̂̃|�C���^
//		 type	�i�������ipoketool.def�ɒ�`�j
//		 itemno	�A�C�e���i���`�F�b�N���̎g�p�A�C�e��(...�ɂ�����B�Ȃ��Ƃ��͎w�肵�Ȃ��Ă��j
//
//	out : 0 �i������
//		  0�ȊO	�i�������|�P�����i���o�[
//
//===============================================================

extern	u16	PokeShinkaCheck(PokemonParam *PP,u8 type,...);

//===============================================================
//
//		�}�Ӄi���o�[���烂���X�^�[�i���o�[���Q�b�g
//
//	in : zukanno	�}�Ӄi���o�[
//
//	out : monsno	�����X�^�[�i���o�[
//
//===============================================================

extern	u16		PokeHouenNo2MonsNoGet(u16 zukanno);

extern	u16		PokeZukanNo2MonsNoGet(u16 zukanno);

extern	u16		PokeZukanNo2HouenNoGet(u16 zukanno);

//===============================================================
//
//		�����X�^�[�i���o�[����}�Ӄi���o�[���Q�b�g
//
//	in : monsno	�����X�^�[�i���o�[
//
//	out : zukanno	�}�Ӄi���o�[
//
//===============================================================

extern	u16		PokeMonsNo2ZukanNoGet(u16 monsno);

extern	u16		PokeMonsNo2HouenNoGet(u16 monsno);

extern	u16		PokeHouenNo2ZukanNoGet(u16 zukanno);

//===============================================================
//
//		�����X�^�[�i���o�[����{�C�X�i���o�[���Q�b�g
//
//	in : monsno	�����X�^�[�i���o�[
//
//	out : voiceno	�����i���o�[
//
//===============================================================

extern	u16		PokeMonsNo2VoiceNoGet(u16 monsno);

//===============================================================
//
//		�u�`�|�P�����Ɍ���������u�`������
//
//	in : monsno	�u�`�|�P�����̃|�P�����i���o�[
//	     rnd	�u�`�|�P�����̌�����
//		 adrs	�O���t�B�b�N�f�[�^���W�J����Ă���̈�̐擪�A�h���X
//
//		�u�`�|�P�����ł͂Ȃ��ꍇ�́A�����������^�[��
//
//===============================================================

extern	void	PokeBuchiMake(u16 monsno,u32 rnd,u8 *adrs);
extern	void	PokeBuchiMakeFB(u16 monsno,u32 rnd,u8 *adrs,u8 dir);

//===============================================================
//
//		�i�����̃j�b�N�l�[����
//
//	in : PP			�|�P�����̍\���̃|�C���^
//		 beforemons	�i���O�|�P�����i���o�[
//		 aftermons	�i����|�P�����i���o�[
//
//===============================================================

extern	void	PokeParaNickNameSet(PokemonParam *PP,u16 beforemons,u16 aftermons);

//===============================================================
//
//		�}���`�v���C�ʐM���̃`�[�����ł̑O�q�E��q���擾
//
//	out :   0		�O�q
//			0�ȊO	��q
//
//===============================================================

extern	u16	GetTeamPos(void);
extern	u16	GetTeamPos2(u8);

//===============================================================
//
//		�}���`�v���C�ʐM���̃`�[�����ł�ClientNo����ID���擾
//
//	out :   i		ID�i���o�[
//
//===============================================================

extern	int	GetTeamPosID(u16 pos);

//===============================================================
//
//		�g���[�i�[�̐��ʂ��擾�i�������[�`���Ȃ̖炵�����p�j
//
//	in : trainerID	�g���[�i�[��ID
//	out :   0:�j�@1:���@3:�������l	...
//
//===============================================================

#define	EYE_BOY			0		//�j
#define	EYE_GIRL		1		//��
#define	EYE_LITTLE_GIRL	2		//���������̎q
#define	EYE_AYASII		3		//�������l
#define	EYE_HAGESII		4		//�͂������l
#define	EYE_KAKKOII		5		//�����������l
#define	EYE_AQUADAN		6		//�A�N�A�c
#define	EYE_MAGUMADAN	7		//�}�O�}�c
#define	EYE_SWIMMER		8		//�X�C�}�[
#define	EYE_FUTAGO		9		//�o�q
#define	EYE_SITENNOU	10		//�l�V��
#define	EYE_DEBU		11		//�o�R��or�ǂ�����n
#define	EYE_INTERVIEWER	12		//�C���^�r���A�[
#define	EYE_GENTLEMAN	13		//�W�F���g���}��

extern	u8	GetTrainerSex(u16 trainerID);

//===============================================================
//
//		�|�P�����̐��i�ɂ��p�����[�^�������v�Z����
//
//	in : chr	���i
//		 para	�v�Z����l
//		 cond	�v�Z����p�����[�^	COND_POW		�U����
//									COND_DEF		�h���
//									COND_AGI		�f����
//									COND_SPEPOW		����U����
//									COND_SPEDEF		����h���
//
//	out : ret	�v�Z����
//
//===============================================================

extern	u8	PokeChrAbiCalc(u8 chr,u16 para,u8 cond);

//===============================================================
//
//		�Ȃ��x�v�Z
//
//	in	:	PP	�Ȃ��x���v�Z����|�P�����\���̂̃|�C���^
//			id	�Ȃ��x�v�Z����|�P�����ɍs�����s��ID�i���o�[�ipoketool.def�ɒ�`�j
//
//===============================================================

extern	void	FriendCalc(PokemonParam *PP,u8 id);

//===============================================================
//
//		�w�͒l�Q�b�g���[�`��
//
//	in	:	PP		�w�͒l���Q�b�g����|�P�����\���̂̃|�C���^
//			monsno	�|��������|�P�����i���o�[
//
//===============================================================

extern	void	ParaExpGet(PokemonParam *PP,u16 monsno);

//===============================================================
//
//		�w�͒l�g�[�^���Q�b�g���[�`��
//		(�����6�p�����[�^�̍��v��510(�b��)���z���Ȃ��悤�ɂ��邽�߁j
//
//	in	:	PP		�w�͒l�̃g�[�^�������߂�|�P�����\���̂̃|�C���^
//
//	out	:	�w�͒l�̍��v
//
//===============================================================

extern	u16		ParaExpTotalGet(PokemonParam *PP);

//===============================================================
//
//		�|�P�����Ƀ|�P���X������������
//
//	in	:	PP		����������|�P�����\���̃|�C���^
//
//===============================================================

extern	void	PokerusSetCheck(PokemonParam *PP);

//===============================================================
//
//		�|�P�������|�P���X�Ɋ������Ă��邩�`�F�b�N
//
//	in	:	PP			�������`�F�b�N����|�P�����\���̂̃|�C���^
//			checkbit	0bit�ڂ�ON�FPP[0]���`�F�b�N
//									:
//									:
//						5bit�ڂ�ON�FPP[6]���`�F�b�N
//						0�̂Ƃ��́APP[0]�������`�F�b�N
//
//===============================================================

extern	u8		PokerusCheck(PokemonParam *PP,u8 checkbit);
//�ߋ��Ɋ������Ă������Ƃ�����
extern	u8		PokerusedCheck(PokemonParam *PP,u8 checkbit);

//===============================================================
//
//		�|�P���X�̓����J�E���^�[���f�N�������g����
//
//	in	:	day		�o�ߓ���
//
//===============================================================

extern	void	PokerusCounterDec(u16 day);

//===============================================================
//
//		�|�P���X�̊����`�F�b�N���[�`��
//
//	in	:	PP			�������`�F�b�N����|�P�����\���̂̃|�C���^
//
//===============================================================

extern	void	PokerusCatchCheck(PokemonParam *PP);

//===============================================================
//
//		�|�P�����̃��x���A�b�v�`�F�b�N������
//
//	in	:	PP		�`�F�b�N����|�P�����\���̂̃|�C���^
//
//	out	:	0:���x���A�b�v���Ȃ�
//			1:���x���A�b�v�����i���̏ꍇlevel��+1����j
//
//===============================================================

extern	u8	PokeLevelUpCheck(PokemonParam *PP);

//=============================================================
//
//�@	�����o�����ɋZ���o����B
//
//	in	:	PP		�Z�b�g����\���̂̃|�C���^
//			wazano	�o����Z
//
//=============================================================

extern	void	PokeWazaOboeOshidashi(PokemonParam *PP,u16 wazano);

//===============================================================
//
//		�|�P�����ɋZ�}�V�����g���邩�`�F�b�N
//
//	in	:	PP			�`�F�b�N����|�P�����\���̂̃|�C���^
//			machineno	�g���Z�}�V���i���o�[
//
//	out	:	0:�����Ȃ�
//			1:������
//
//===============================================================

extern	u32		PokeWazaMachineCheck(PokemonParam *PP,u8 machineno);

//===============================================================
//
//		�|�P�������o����Z���w�肳�ꂽ�o�b�t�@�Ɋi�[����
//
//	in	:	PP			�Z�����ڂ���|�P�����\���̂̃|�C���^
//			nobuf		�Z�i���o�[���i�[����o�b�t�@�̃|�C���^
//
//	out	:	�i�[�����Z�i���o�[�̐�
//
//===============================================================

extern	u8		PokeOboeruWazaGet(PokemonParam *PP,u16 *nobuf);

//===============================================================
//
//		�|�P�������v���o����Z�����邩�`�F�b�N����
//
//	in	:	PP			�Z���v���o���|�P�����\���̂̃|�C���^
//
//	out	:	�i�[�����Z�i���o�[�̐�
//
//===============================================================

extern	u8		PokeOboeruWazaCheck(PokemonParam *PP);

//======================================================================================
//
//	PokeParaSetPowRnd(�|�P�����̃p�����[�^���Z�b�g����B�p���[������u32�Ŏ󂯎���đ���j
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//	pow			=	�Z�b�g����p���[����
//
//======================================================================================

extern	void	PokeParaSetPowRnd(PokemonParam *PP,u16 pokeno,u8 pokelevel,u32 pow,u32 rnd);

//======================================================================================
//
//	PokeParaSetPowRndBuf(�|�P�����̃p�����[�^���Z�b�g����B�p���[������u8 *�Ŏ󂯎���đ���j
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//	pow			=	�Z�b�g����p���[����
//
//======================================================================================

extern	void	PokeParaSetPowRndBuf(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 *pow,u32 id);

//======================================================================================
//
//	PokeParaSetParaExp(�|�P�����̃p�����[�^���Z�b�g����B�w�͒l���Z�b�g����j
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//	pow			=	�Z�b�g����p���[����
//	expbit		=	�Z�b�g����w�͒l�̃r�b�g(0bit:hp_exp 1bit:pow_exp 2bit:def_exp...�j
//
//======================================================================================

extern	void	PokeParaSetParaExp(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 expbit);

//======================================================================================
//
//	PokeParaSetStock(�|�P�����̃p�����[�^���Z�b�g����B�o�g���^���[�X�g�b�N�f�[�^��p�j
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	SBTP		=	�o�g���^���[�X�g�b�N�f�[�^
//
//======================================================================================

extern	void	PokeParaSetStock(PokemonParam *PP,StockBattleTowerPokemon *SBTP);

//======================================================================================
//PokemonParam����StockBattleTowerPokemon�^�f�[�^�𐶐�
//	���o�g���^���[�E�J�[�he�g���[�i�[��p
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	SBTP		=	�o�g���^���[�X�g�b�N�f�[�^
//======================================================================================

extern void PokeParaGetStock(PokemonParam * PP, StockBattleTowerPokemon * SBTP);

//===============================================================
//
//		�|�P�����i���o�[�ϊ��i�|�P�����i���o�[��\������Ƃ��͕K���g�����ƁI�j
//
//	IN	:pokenum		�J���i���o�[
//
//	OUT	:�z�E�G���}�ӂ����Ȃ��@�z�E�G���}�ӂɓo�^����Ă���@�� �z�E�G���i���o�[�o��
//		:�@�@�@�@�@�@�@�@�@�@�@�z�E�G���}�ӂɓo�^����Ă��Ȃ��� 0xffff�i�\���Ȃ��j
//		:�S���}�ӂ��݂��@���@�S���i���o�[�o��
//
//===============================================================
extern	u16	PokemonNumberGet(u16 pokenum);

//===============================================================
//		�|�P�����̃A�j���[�V�����i���o�[�ێ����[�N�̏�����
//===============================================================

extern	void	PokeAnmNoInit(void);

//===============================================================
//		�퓬�Ȃ�炷
//===============================================================

extern	void	FightBGMSet(void);
extern	void	FightBGMSet2(u16 music);

//===============================================================
//
//		�|�P�����̃p���b�g�A�h���X���Q�b�g
//
//	in	:	PP	�`�F�b�N����|�P�����̍\���̂̃|�C���^
//
//	out	:	�p���b�g�f�[�^�̃A�h���X
//===============================================================

extern	u32		PokePalGet(PokemonParam *PP);

//===============================================================
//
//		�|�P�����̃p���b�g�A�h���X���Q�b�g�i�p�����[�^�w��j
//
//	in	:	monsno	�`�F�b�N���郂���X�^�[No
//			id		�`�F�b�N����|�P������ID
//			rnd		�`�F�b�N����|�P�����̌�����
//
//	out	:	�p���b�g�f�[�^�̃A�h���X
//===============================================================

extern	u32		PokePalGetPara(u16 monsno,u32 id,u32 rnd);

//===============================================================
//
//		�|�P�����̃p���b�g�e�[�u���A�h���X���Q�b�g
//
//	in	:	PP	�`�F�b�N����|�P�����̍\���̂̃|�C���^
//
//	out	:	�p���b�g�e�[�u���̃A�h���X
//===============================================================

extern	u32		PokePalDataGet(PokemonParam *PP);

//===============================================================
//
//		�|�P�����̃p���b�g�e�[�u���A�h���X���Q�b�g�i�p�����[�^�w��j
//
//	in	:	monsno	�`�F�b�N���郂���X�^�[No
//			id		�`�F�b�N����|�P������ID
//			rnd		�`�F�b�N����|�P�����̌�����
//
//	out	:	�p���b�g�e�[�u���̃A�h���X
//===============================================================

extern	u32		PokePalDataGetPara(u16 monsno,u32 id,u32 rnd);

//===============================================================
//
//		��`�Z���ǂ����`�F�b�N
//
//	in	:	wazano	�`�F�b�N����ZNo
//
//	out	:	0:��`�Z�ł͂Ȃ��@1:��`�Z�ł�
//
//===============================================================

extern	u32		HidenWazaCheck(u16 wazano);

//===============================================================
//
//		���]�\���\�ȃ|�P�������ǂ����`�F�b�N
//
//	in	:	monsno	�`�F�b�N���郂���X�^�[No
//
//	out	:	0:���]�\�@1:���]�s��
//
//===============================================================

extern	u8		PokeReverseCheck(u16 monsno);

//===============================================================
//
//		�|�P�����̍D�݂̖����ǂ����`�F�b�N
//
//	in	:	PP		�`�F�b�N����|�P�����\���̂̃|�C���^
//			rnd		���i�𓱂��o�����߂̌�����
//			taste	�`�F�b�N���閡
//
//	out	:	-1:�����@0:����	1:�D��
//
//===============================================================

s8		PokeLikeTasteCheck(PokemonParam *PP,u8 taste);
s8		PokeLikeTasteCheckRnd(u32 rnd,u8 taste);

//===============================================================
//
//		�����̎����|�P�������ǂ����`�F�b�N
//
//	in	:	PP		�`�F�b�N����|�P�����\���̂̃|�C���^
//
//	out	:	0:������	1:���l��
//
//===============================================================

extern	u8		PokeOneSelfCheck(PokemonParam *);
extern	u8		PokeOneSelfCheckAct(u32,u8 *);

//===============================================================
//
//		pp��max�ɂ���i�{�b�N�X�ɗa����Ƃ���p�j
//
//	in	:	PP		max�ɂ���|�P�����\���̂̃|�C���^
//			PPP		max�ɂ���|�P�����\���̂̃|�C���^
//
//===============================================================

extern	void	PokePPMaxSet(PokemonParam *);
extern	void	PokePasoPPMaxSet(PokemonPasoParam *);

//===============================================================
//
//		�����œ���ւ��ł��Ȃ��Ƃ��ɕ\�����郁�b�Z�[�W���Q�b�g
//
//===============================================================

extern	void	PokeNoReshuffleMsgGet(void);

//===============================================================
//
//		�쐫�|�P�����Ɏ�������A�C�e���̃Z�b�g
//
//===============================================================

extern	void	PokeItemSet(void);

//===============================================================
//
//		�A�C�e���p�����[�^�e�[�u���̃I�t�Z�b�g�����߂�
//
//===============================================================

extern	u8	ItemParamPosGet(u16 itemno,u8 pos,u8 checkbit);

//===============================================================
//
//		�|�P���������A���ǂ����`�F�b�N
//
//	out	0:���A����Ȃ�
//		1:���A�ł�
//
//===============================================================

extern	u8		PokeRareGet(PokemonParam *PP);
extern	u8		PokeRareGetPara(u32 id,u32 rnd);

//===============================================================
//
//		���Ԃ̃g���[�i�[�����Q�b�g
//
//===============================================================

extern	u8		*TeamTrainerNameGet(void);

#endif

#define	POW_RND		32	//PokeParaSet���g���Ƃ��Ƀp���[�����������_���łƂ邽�߂̒l
#define	RND_NO_SET	0	//PokeParaSet���g���Ƃ��Ɍ������������_���ŃZ�b�g���邽�߂̒l
#define	RND_SET		1	//PokeParaSet���g���Ƃ��Ɍ��������Œ�l�ŃZ�b�g���邽�߂̒l
#define	ID_NO_SET	0	//PokeParaSet���g���Ƃ���ID�������_���ŃZ�b�g���邽�߂̒l
#define	ID_SET		1	//PokeParaSet���g���Ƃ���ID���Œ�l�ŃZ�b�g���邽�߂̒l
#define	ID_NO_RARE	2	//PokeParaSet���g���Ƃ���ID���������Ƃ̑g�ݍ��킹�Ń��A���o�Ȃ��悤�ɂ���̒l
#define	NO_WAZA_SET	0xffff		//�Z�̃Z�b�g���ł��Ȃ������Ƃ��̕Ԃ�l
#define	SAME_WAZA_SET	0xfffe		//���łɊo���Ă����Z�̂Ƃ��̕Ԃ�l
