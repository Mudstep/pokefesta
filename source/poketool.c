
//======================================================
//    poketool.c                                           
//                                                      
//    Copyright (C) 2001 GAME FREAK inc.
//======================================================

#include "common.h"
#include "fight.h"
#include "tokusyu.def"
#include "agbmons.h"
#include "param.h"
#include "calctool.h"
#include "actor.h"
#include "actanm.h"
#include "a_mine.h"
#include "a_enemy.h"
#include "print.h"
#include "wazatool.h"
#include "bss.h"
#include "server.h"
#include "gamedata.h"
#include "ev_fight.h"
#include "item.h"
#include "pasocom.h"
#include "objdata.h"

#include "poketool.def"

#include "waza_tbl.dat"
#include "kyouwaza.dat"

#include "monsno.def"
#include "shinka.def"
#include "shinka.h"
#include "zukanno.dat"
#include "zenkoku.dat"
#include "houen.dat"
#include "buchi.dat"

#include "..\data\itemtbl.h"
#include "..\data\itemsym.h"

#include "client.h"
#include "multisio.h"		//add
#include "pokesio.h"
#include "cable.h"
#include "trainer.h"
#include "fightmsg.h"
#include "mapparam.h"
#include "chrabi.dat"
#include "trfenum.h"
#include "tr_enum.h"
#include "ev_flag.h"
#include "sysflag.h"
#include "mail.h"
#include "seed.h"
#include "machine.dat"
#define	__POKETOOL_H_
#include "poketool.h"
#include "wazano.def"
#include "..\data\itemeqp.h"
#include "zukan.h"
#include "mus_tool.h"
#include "tr_tbl.h"
#include "tr_tbl.dat"
#include "pv_tbl.dat"
#include "client_t.h"
#include "decord.h"
#include "growtbl.h"
#include "cubecase.h"
#include "b_pmlist.h"

#ifdef	FESTA_VER_ROM
#include "agbmonsp.dat"
#include "wazaoboep.dat"
#include "shinkap.dat"
#else
#include "agbmons.dat"
#include "wazaoboe.dat"
#include "shinka.dat"
#endif

#include "wazaoboe.tbl"
#include "rtctool.h"

#define	ANGOU	1

#define	PPG_MINE	0
#define	PPG_ENEMY	6

#define	PARA_EXP_TOTAL_MAX	510		//�w�͒l�̍��v�̃}�b�N�X
#define	PARA_EXP_MAX		100		//�w�͒l�̃}�b�N�X

void	PokeParaSet(PokemonParam *,u16,u8,u8,u8,u32,u8,u32);
void	PokePasoParaSet(PokemonPasoParam *,u16,u8,u8,u8,u32,u8,u32);
void	PokeParaSetChr(PokemonParam *,u16,u8,u8,u8);
void	PokeParaSetSexChr(PokemonParam *,u16,u8,u8,u8,u8,u8);
void	PokeParaSetPowRnd(PokemonParam *,u16,u8,u32,u32);
void	PokeParaSetPowRndBuf(PokemonParam *,u16,u8,u8*,u32);
void	PokeParaSetParaExp(PokemonParam *,u16,u8,u8,u8);
void	PokeParaSetStock(PokemonParam *,StockBattleTowerPokemon *);
u16		PokeParaCheckSum(PokemonPasoParam *);
void	PokeParaCalc(PokemonParam *);
void	PokeParaNickNameSet(PokemonParam *,u16,u16);
void	PokeReplace(PokemonPasoParam *,PokemonParam *);
void	PokeActHeaderMake(u16,u8);
void	TrainerActHeaderMake(u16,u8);
void	PokeParaCoded(PokemonPasoParam *);
void	PokeParaDecoded(PokemonPasoParam *);
u32		PokeParaAdrsGet(PokemonPasoParam *,u32,u8);
u32		PokeParaGet(PokemonParam *,int,u8 *);
void	PokeParaPut(PokemonParam *,int,const u8 *);
u32		PokePasoParaGet(PokemonPasoParam *,int,u8 *);
void	PokePasoParaPut(PokemonPasoParam *,int,const u8 *);
u8		AddPokemon(PokemonParam *);
void	AddPokemonAct(PokemonParam *,PokemonParam *,int);
u8		AddPasoPokemon(PokemonParam *);
void	OrusubanTaisenSet(SECRET_BASE_DATA *);
u8		OrusubanChrGet(void);
u8		OrusubanTypeGet(void);
u8		BallThrowCheck(void);
u8		BallThrowCheckSafari(void);

u8		Poke2vs2Check(void);

u8		PokeCountMineGet();
u8		PokeCountEnemyGet();

u16		PokeHouenNo2MonsNoGet(u16);
u16		PokeZukanNo2MonsNoGet(u16);
u16		PokeZukanNo2HouenNoGet(u16);
u16		PokeMonsNo2ZukanNoGet(u16);
u16		PokeHouenNo2ZukanNoGet(u16);
u16		PokeMonsNo2HouenNoGet(u16);
u16		PokeMonsNo2VoiceNoGet(u16);

u16		PokeWazaSet(PokemonParam *,u16);
u16		PokePasoWazaSet(PokemonPasoParam *,u16);
void	PokeWazaSetPos(PokemonParam *,u16,u8);
u16		PokeWazaSetServer(PokemonServerParam *,u16);
void	PokeWazaSetPosServer(PokemonServerParam *,u16,u8);
void	PokePPCountClear(PokemonParam *,u8);
void	PokePPCountClearServer(PokemonServerParam *,u8);
void	PokeWazaOboe(PokemonParam *);
void	PokePasoWazaOboe(PokemonPasoParam *);
u16		PokeWazaOboeCheck(PokemonParam *,u8);
void	PokeWazaOboeOshidashi(PokemonParam *,u16);
void	PokePasoWazaOboeOshidashi(PokemonPasoParam *,u16);
s32		WazaDamageCalc(PokemonServerParam *,PokemonServerParam *,int,u16,u16,u8,u8,u8);
u8		HitCountCheck(u8);
u8		GetDefenceClient(u8);
u8		PPMaxGet(u16,u8,u8);
u8		PokeSexGet(PokemonParam *);
u8		PokePasoSexGet(PokemonPasoParam *);
u8		PokeSexGetMonsNo(u16 no,u32 rnd);
u8		PokeLevelCalc(PokemonParam *);
u8		PokePasoLevelCalc(PokemonPasoParam *);
u8		SpecialAbirityGet(u16,u8);
u8		SpecialAbirityNoGet(PokemonParam *);
u8		StatusRecover(PokemonParam *,u16,u8,u8);
u8		StatusRecover2(PokemonParam *,u16,u8,u8,u8);
u8		StatusRecoverAct(PokemonParam *,u8,u32,u8);

void	PokeNameSet(u8 *,u16);

void	PokeParaMineInit(void);
void	PokeParaEnemyInit(void);
void	PokeParaInit(PokemonParam *);

void	PokeBuchiMake(u16 monsno,u32 rnd,u8 *adrs);
void	PokeBuchiMakeFB(u16 monsno,u32 rnd,u8 *adrs,u8 dir);

u8	*FightItemUseMsgGet(u16);

u8	PokeSeikakuGet(PokemonParam *);
u8	PokeSeikakuGetRnd(u32 rnd);

extern	const	u8	PokeNameData[][6];
extern SIOTRAINER_INFO SioTrainerInfo[4];

u16	PokeShinkaCheck(PokemonParam *,u8,u16);

u16	GetTeamPos(void);
u16	GetTeamPos2(u8);
int	GetTeamPosID(u16);
u16	PokeChrAbiCalc(u8 chr,u16 para,u8 cond);

void	FriendCalc(PokemonParam *PP,u8 id);

void	ParaExpGet(PokemonParam *PP,u16 monsno);
u16		ParaExpTotalGet(PokemonParam *PP);
void	PokerusSetCheck(PokemonParam *PP);
u8		PokerusCheck(PokemonParam *PP,u8 checkbit);
u8		PokerusedCheck(PokemonParam *PP,u8 checkbit);
void	PokerusCounterDec(u16 day);
void	PokerusCatchCheck(PokemonParam *PP);

u8		PokeLevelUpCheck(PokemonParam *PP);

u32		PokeWazaMachineCheck(PokemonParam *PP,u8 machineno);

u8		PokeOboeruWazaGet(PokemonParam *PP,u16 *nobuf);
u8		PokeOboeruWazaCheck(PokemonParam *PP);

void	BattleTowerTrainerSet(void);
void	BattleTowerPokemonSet(void);

void	PokeAnmNoInit(void);

void	FightBGMSet(void);
void	FightBGMSet2(u16 music);

u32		PokePalGet(PokemonParam *PP);
u32		PokePalGetPara(u16,u32,u32);

u32		PokePalDataGet(PokemonParam *PP);
u32		PokePalDataGetPara(u16,u32,u32);

u8		PokeReverseCheck(u16 monsno);

u32		HidenWazaCheck(u16);

s8		PokeLikeTasteCheck(PokemonParam *,u8);
s8		PokeLikeTasteCheckRnd(u32,u8);

u8		PokeOneSelfCheck(PokemonParam *);
u8		PokeOneSelfCheckAct(u32,u8 *);

void	PokePPMaxSet(PokemonParam *);
void	PokePasoPPMaxSet(PokemonPasoParam *);

void	PokeNoReshuffleMsgGet(void);

void	PokeItemSet(void);

u8	ItemParamPosGet(u16 itemno,u8 pos,u8 checkbit);

u8		PokeRareGet(PokemonParam *);
u8		PokeRareGetPara(u32,u32);

u8		*TeamTrainerNameGet(void);

u16		wazapow=0;
u16		OboeWazaNo=0;
u8		BattlePokeAnmNo[CLIENT_MAX]={};
u8		wazaoboecnt=0;

extern	const u8 CasetteVersion;
extern	const u8 CasetteLanguage;

//==========================================================
//		�|�P�����p�����[�^�iSRAM�ɕۑ����Ȃ���΂Ȃ�Ȃ�����
//==========================================================

PokemonParam	PokeParaMine[6];
u8		PokeCountMine;

//==========================================================

PokemonParam	PokeParaEnemy[6];
u8		PokeCountEnemy;

actHeader	PokeActHeader={};

const	u8 pp_count_mask[4]={0x03,0x0c,0x30,0xc0};
const	u8 pp_count_off[4]={0xfc,0xf3,0xcf,0x3f};
const	u8 pp_count_value[4]={0x01,0x04,0x10,0x40};

const u8 CondChgTable[][2]={
		{10,40},
		{10,35},
		{10,30},
		{10,25},
		{10,20},
		{10,15},
		{10,10},
		{15,10},
		{20,10},
		{25,10},
		{30,10},
		{35,10},
		{40,10},
};

const u8 OyaNameData[]={
		GE_,bou_,HU_,RI_,EOM_
};


void PokePasoParaInit(PokemonPasoParam *PPP)
{
	int i;
	u8	*adr;

	adr=(u8 *)PPP;

	for(i=0;i<sizeof(PokemonPasoParam);i++)
		adr[i]=0;
}

void	PokeParaInit(PokemonParam *PP)
{
	int	i;

	PokePasoParaInit(&(PP->PPP));
	i=0;
	PokeParaPut(PP,ID_condition,(u8 *)&i);
	PokeParaPut(PP,ID_level,(u8 *)&i);
	PokeParaPut(PP,ID_hp,(u8 *)&i);
	PokeParaPut(PP,ID_hpmax,(u8 *)&i);
	PokeParaPut(PP,ID_pow,(u8 *)&i);
	PokeParaPut(PP,ID_def,(u8 *)&i);
	PokeParaPut(PP,ID_agi,(u8 *)&i);
	PokeParaPut(PP,ID_spepow,(u8 *)&i);
	PokeParaPut(PP,ID_spedef,(u8 *)&i);
	i=NON_MAIL;
	PokeParaPut(PP,ID_mailid,(u8 *)&i);
}

void	PokeParaMineInit(void)
{
	int	i;
	for(i=0;i<6;i++)
		PokeParaInit(&PokeParaMine[i]);
}

void	PokeParaEnemyInit(void)
{
	int	i;
	for(i=0;i<6;i++)
		PokeParaInit(&PokeParaEnemy[i]);
}


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

void	PokeParaSet(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 rndflag,u32 rnd,u8 idflag,u32 id)
{
	u32	i;

	PokeParaInit(PP);

	PokePasoParaSet((PokemonPasoParam *)&PP->PPP,pokeno,pokelevel,pow,rndflag,rnd,idflag,id);

//���x���Z�b�g
	PokeParaPut(PP,ID_level,(u8 *)&pokelevel);

//���[��ID
	i=NON_MAIL;
	PokeParaPut(PP,ID_mailid,(u8 *)&i);

	PokeParaCalc(PP);
}

void	PokePasoParaSet(PokemonPasoParam *PPP,u16 pokeno,u8 pokelevel,u8 pow,u8 rndflag,u32 rnd,u8 idflag,u32 id)
{
	u16	sum;
	u32	i,j;
	u32	p_rnd;
	u8	buf[MONS_NAME_SIZE+EOM_SIZE];

	PokePasoParaInit(PPP);

//�������Z�b�g
	if(rndflag)
		p_rnd=rnd;
	else
		p_rnd=(pp_rand()|(pp_rand()<<16));
	PokePasoParaPut(PPP,ID_personal_rnd,(u8 *)&p_rnd);

//ID�i���o�[�Z�b�g
	if(idflag==ID_NO_RARE){
		do{
			i=(pp_rand()|(pp_rand()<<16));
		}while((((i&0xffff0000)>>16)^(i&0xffff)^((p_rnd&0xffff0000)>>16)^(p_rnd&0xffff))<8);
	}
	else if(idflag==ID_SET)
		i=id;
	else
		i=(MyData.ID[0]<< 0)|
		  (MyData.ID[1]<< 8)|
		  (MyData.ID[2]<<16)|
		  (MyData.ID[3]<<24);
	PokePasoParaPut(PPP,ID_id_no,(u8 *)&i);
	
//�`�F�b�N�T���i�Ƃ肠������������j
	sum=PokeParaCheckSum(PPP);
	PokePasoParaPut(PPP,ID_checksum,(u8 *)&sum);

//��������ID�����܂����Ƃ���ł�������Í������Ă���
	PokeParaCoded(PPP);

//�j�b�N�l�[���Z�b�g
	PokeNameSet(buf,pokeno);
	PokePasoParaPut(PPP,ID_nickname,(u8 *)&buf);

//���R�[�h
	PokePasoParaPut(PPP,ID_country_code,(u8 *)&CasetteLanguage);

//�e�̖��O�Z�b�g
	PokePasoParaPut(PPP,ID_oyaname,(u8 *)&MyData.my_name);

//�|�P�����i���o�[�Z�b�g
	PokePasoParaPut(PPP,ID_monsno,(u8 *)&pokeno);

//�o���l�Z�b�g
	PokePasoParaPut(PPP,ID_exp,(u8 *)&GrowTbl[PPD[pokeno].grow][pokelevel]);

//�F�D�l�Z�b�g
	PokePasoParaPut(PPP,ID_friend,(u8 *)&PPD[pokeno].friend);

//�ߊl�f�[�^�Z�b�g
	i=GetNowWorldPos();	//�ŏ���3�̂�����ł͂���Ƃ����Ȃ�
	PokePasoParaPut(PPP,ID_get_place,(u8 *)&i);
	PokePasoParaPut(PPP,ID_get_level,(u8 *)&pokelevel);
	PokePasoParaPut(PPP,ID_get_cassette,(u8 *)&CasetteVersion);
	i=ITEM_MONSUTAABOORU;	//�f�t�H���g�̓����X�^�[�{�[���ɂ��Ă���	
	PokePasoParaPut(PPP,ID_get_ball,(u8 *)&i);

//�e�̐��ʃZ�b�g
	PokePasoParaPut(PPP,ID_oyasex,(u8 *)&MyData.my_sex);

//�p���[�����Z�b�g
	if(pow<POW_RND){
		PokePasoParaPut(PPP,ID_hp_rnd,(u8 *)&pow);
		PokePasoParaPut(PPP,ID_pow_rnd,(u8 *)&pow);
		PokePasoParaPut(PPP,ID_def_rnd,(u8 *)&pow);
		PokePasoParaPut(PPP,ID_agi_rnd,(u8 *)&pow);
		PokePasoParaPut(PPP,ID_spepow_rnd,(u8 *)&pow);
		PokePasoParaPut(PPP,ID_spedef_rnd,(u8 *)&pow);
	}
	else{
		i=pp_rand();
		j=(i&(0x001f<< 0))>> 0;
		PokePasoParaPut(PPP,ID_hp_rnd,(u8 *)&j);
		j=(i&(0x001f<< 5))>> 5;
		PokePasoParaPut(PPP,ID_pow_rnd,(u8 *)&j);
		j=(i&(0x001f<<10))>>10;
		PokePasoParaPut(PPP,ID_def_rnd,(u8 *)&j);
		i=pp_rand();
		j=(i&(0x001f<< 0))>> 0;
		PokePasoParaPut(PPP,ID_agi_rnd,(u8 *)&j);
		j=(i&(0x001f<< 5))>> 5;
		PokePasoParaPut(PPP,ID_spepow_rnd,(u8 *)&j);
		j=(i&(0x001f<<10))>>10;
		PokePasoParaPut(PPP,ID_spedef_rnd,(u8 *)&j);
	}

//����\�̓Z�b�g
	if(PPD[pokeno].speabi2!=0){
		i=p_rnd&1;
		PokePasoParaPut(PPP,ID_speabi,(u8 *)&i);
	}

//�Z�Z�b�g
	PokePasoWazaOboe(PPP);

}
# if 0
void	PokeParaSet(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 rndflag,u32 rnd,u8 idflag,u32 id)
{
	u16	sum;
	u32	i,j=0;
	u8	buf[MONS_NAME_SIZE+EOM_SIZE];

//�s���^�}�S�t���O�́A�ŏ��ɓ��R���Ƃ�
	PokeParaPut(PP,ID_fusei_tamago_flag,(u8 *)&j);
//�|�P�������݃t���O�́A�ŏ��ɓ��R���Ƃ�
	PokeParaPut(PP,ID_poke_exist_flag,(u8 *)&j);
//�^�}�S���݃t���O�́A�ŏ��ɓ��R���Ƃ�
	PokeParaPut(PP,ID_egg_exist_flag,(u8 *)&j);

//�������Z�b�g
	if(rndflag)
		i=rnd;
	else
		i=(pp_rand()|(pp_rand()<<16));
	PokeParaPut(PP,ID_personal_rnd,(u8 *)&i);

//ID�i���o�[�Z�b�g
	if(idflag)
		i=id;
	else
		i=(MyData.ID[0]<< 0)|
		  (MyData.ID[1]<< 8)|
		  (MyData.ID[2]<<16)|
		  (MyData.ID[3]<<24);
	PokeParaPut(PP,ID_id_no,(u8 *)&i);
	
//�`�F�b�N�T���i�Ƃ肠������������j
	sum=PokeParaCheckSum((PokemonPasoParam*)&PP->PPP);
	PokeParaPut(PP,ID_checksum,(u8 *)&sum);

//��������ID�����܂����Ƃ���ł�������Í������Ă���
	PokeParaCoded((PokemonPasoParam *)&PP->PPP);

//�j�b�N�l�[���Z�b�g
	PokeNameSet(buf,pokeno);
	PokeParaPut(PP,ID_nickname,(u8 *)&buf);

//�}�[�N
	PokeParaPut(PP,ID_mark,(u8 *)&j);

//�e�̖��O�Z�b�g
	PokeParaPut(PP,ID_oyaname,(u8 *)&MyData.my_name);

//�|�P�����i���o�[�Z�b�g
	PokeParaPut(PP,ID_monsno,(u8 *)&pokeno);

//�����A�C�e���Z�b�g
	PokeParaPut(PP,ID_item,(u8 *)&j);

//�o���l�Z�b�g
	PokeParaPut(PP,ID_exp,(u8 *)&GrowTbl[PPD[pokeno].grow][pokelevel]);

//�Z�|�C���g�J�E���g�Z�b�g
	PokeParaPut(PP,ID_pp_count,(u8 *)&j);

//�F�D�l�Z�b�g
	PokeParaPut(PP,ID_friend,(u8 *)&PPD[pokeno].friend);

//�Z�N���A
	for(i=0;i<4;i++){
		PokeParaPut(PP,ID_waza1+i,(u8 *)&j);
		PokeParaPut(PP,ID_pp1+i,(u8 *)&j);
	}

//�w�͒l�N���A
	PokeParaPut(PP,ID_hp_exp,(u8 *)&j);
	PokeParaPut(PP,ID_pow_exp,(u8 *)&j);
	PokeParaPut(PP,ID_def_exp,(u8 *)&j);
	PokeParaPut(PP,ID_agi_exp,(u8 *)&j);
	PokeParaPut(PP,ID_spepow_exp,(u8 *)&j);
	PokeParaPut(PP,ID_spedef_exp,(u8 *)&j);

//�������悳
	PokeParaPut(PP,ID_style,(u8 *)&j);

//��������
	PokeParaPut(PP,ID_beautiful,(u8 *)&j);

//���킢��
	PokeParaPut(PP,ID_cute,(u8 *)&j);

//��������
	PokeParaPut(PP,ID_clever,(u8 *)&j);

//�����܂���
	PokeParaPut(PP,ID_strong,(u8 *)&j);

//�щ�
	PokeParaPut(PP,ID_fur,(u8 *)&j);

//�|�P���X�N���A
	PokeParaPut(PP,ID_pokerus,(u8 *)&j);

//�ߊl�f�[�^�Z�b�g
	i=GetNowWorldPos();	//�ŏ���3�̂�����ł͂���Ƃ����Ȃ�
	PokeParaPut(PP,ID_get_place,(u8 *)&i);
	PokeParaPut(PP,ID_get_level,(u8 *)&pokelevel);
	PokeParaPut(PP,ID_get_cassette,(u8 *)&CasetteVersion);
	i=ITEM_MONSUTAABOORU;	//�f�t�H���g�̓����X�^�[�{�[���ɂ��Ă���	
	PokeParaPut(PP,ID_get_ball,(u8 *)&i);

//�e�̐��ʃZ�b�g
	PokeParaPut(PP,ID_oyasex,(u8 *)&MyData.my_sex);

//�p���[�����Z�b�g
	if(pow<POW_RND){
		PokeParaPut(PP,ID_hp_rnd,(u8 *)&pow);
		PokeParaPut(PP,ID_pow_rnd,(u8 *)&pow);
		PokeParaPut(PP,ID_def_rnd,(u8 *)&pow);
		PokeParaPut(PP,ID_agi_rnd,(u8 *)&pow);
		PokeParaPut(PP,ID_spepow_rnd,(u8 *)&pow);
		PokeParaPut(PP,ID_spedef_rnd,(u8 *)&pow);
	}
	else{
		i=pp_rand();
		j=(i&(0x001f<< 0))>> 0;
		PokeParaPut(PP,ID_hp_rnd,(u8 *)&j);
		j=(i&(0x001f<< 5))>> 5;
		PokeParaPut(PP,ID_pow_rnd,(u8 *)&j);
		j=(i&(0x001f<<10))>>10;
		PokeParaPut(PP,ID_def_rnd,(u8 *)&j);
		i=pp_rand();
		j=(i&(0x001f<< 0))>> 0;
		PokeParaPut(PP,ID_agi_rnd,(u8 *)&j);
		j=(i&(0x001f<< 5))>> 5;
		PokeParaPut(PP,ID_spepow_rnd,(u8 *)&j);
		j=(i&(0x001f<<10))>>10;
		PokeParaPut(PP,ID_spedef_rnd,(u8 *)&j);
	}

//�^�}�S�t���O�N���A
	j=0;
	PokeParaPut(PP,ID_tamago_flag,(u8 *)&j);

//����\�̓Z�b�g
	if(PPD[pokeno].speabi2==0)
		PokeParaPut(PP,ID_speabi,(u8 *)&j);
	else{
		i=pp_rand()&1;
		PokeParaPut(PP,ID_speabi,(u8 *)&i);
	}

//�M��
	PokeParaPut(PP,ID_stylemedal,(u8 *)&j);
	PokeParaPut(PP,ID_beautifulmedal,(u8 *)&j);
	PokeParaPut(PP,ID_cutemedal,(u8 *)&j);
	PokeParaPut(PP,ID_clevermedal,(u8 *)&j);
	PokeParaPut(PP,ID_strongmedal,(u8 *)&j);
	PokeParaPut(PP,ID_champ_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_winning_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_victory_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_bromide_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_ganba_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_marine_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_land_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_sky_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_country_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_national_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_earth_ribbon,(u8 *)&j);
	PokeParaPut(PP,ID_world_ribbon,(u8 *)&j);

//�R���f�B�V�����Z�b�g
	PokeParaPut(PP,ID_condition,(u8 *)&j);

//���x���Z�b�g
	PokeParaPut(PP,ID_level,(u8 *)&pokelevel);

//���[��ID
	i=NON_MAIL;
	PokeParaPut(PP,ID_mailid,(u8 *)&i);

//HP������
	PokeParaPut(PP,ID_hp,(u8 *)&j);
	PokeParaPut(PP,ID_hpmax,(u8 *)&j);
	PokeParaCalc(PP);

//�Z�Z�b�g
	PokeWazaOboe(PP);

}

void	PokePasoParaSet(PokemonPasoParam *PPP,u16 pokeno,u8 pokelevel,u32 id)
{
	u16	sum;
	u32	i,j=0;
	u8	buf[MONS_NAME_SIZE+EOM_SIZE];

//�s���^�}�S�t���O�́A�ŏ��ɓ��R���Ƃ�
	PokePasoParaPut(PPP,ID_fusei_tamago_flag,(u8 *)&j);

//�������Z�b�g
	i=(pp_rand()|(pp_rand()<<16));
	PokePasoParaPut(PPP,ID_personal_rnd,(u8 *)&i);

//ID�i���o�[�Z�b�g
	if(id)
		i=id;
	else
		i=(MyData.ID[0]<< 0)|
		  (MyData.ID[1]<< 8)|
		  (MyData.ID[2]<<16)|
		  (MyData.ID[3]<<24);
	PokePasoParaPut(PPP,ID_id_no,(u8 *)&i);
	
//�`�F�b�N�T���i�Ƃ肠������������j
	sum=PokeParaCheckSum(PPP);
	PokePasoParaPut(PPP,ID_checksum,(u8 *)&sum);

//��������ID�����܂����Ƃ���ł�������Í������Ă���
	PokeParaCoded(PPP);

//�j�b�N�l�[���Z�b�g
	PokeNameSet(buf,pokeno);
	PokePasoParaPut(PPP,ID_nickname,(u8 *)&buf);

//�}�[�N
	PokePasoParaPut(PPP,ID_mark,(u8 *)&j);

//�e�̖��O�Z�b�g
	PokePasoParaPut(PPP,ID_oyaname,(u8 *)&MyData.my_name);

//�|�P�����i���o�[�Z�b�g
	PokePasoParaPut(PPP,ID_monsno,(u8 *)&pokeno);

//�����A�C�e���Z�b�g
	PokePasoParaPut(PPP,ID_item,(u8 *)&j);

//�o���l�Z�b�g
	PokePasoParaPut(PPP,ID_exp,(u8 *)&GrowTbl[PPD[pokeno].grow][pokelevel]);

//�Z�|�C���g�J�E���g�Z�b�g
	PokePasoParaPut(PPP,ID_pp_count,(u8 *)&j);

//�F�D�l�Z�b�g
	PokePasoParaPut(PPP,ID_friend,(u8 *)&PPD[pokeno].friend);

//�Z�N���A
	for(i=0;i<4;i++){
		PokePasoParaPut(PPP,ID_waza1+i,(u8 *)&j);
		PokePasoParaPut(PPP,ID_pp1+i,(u8 *)&j);
	}

//�w�͒l�N���A
	PokePasoParaPut(PPP,ID_hp_exp,(u8 *)&j);
	PokePasoParaPut(PPP,ID_pow_exp,(u8 *)&j);
	PokePasoParaPut(PPP,ID_def_exp,(u8 *)&j);
	PokePasoParaPut(PPP,ID_agi_exp,(u8 *)&j);
	PokePasoParaPut(PPP,ID_spepow_exp,(u8 *)&j);
	PokePasoParaPut(PPP,ID_spedef_exp,(u8 *)&j);

//�������悳
	PokePasoParaPut(PPP,ID_style,(u8 *)&j);

//��������
	PokePasoParaPut(PPP,ID_beautiful,(u8 *)&j);

//���킢��
	PokePasoParaPut(PPP,ID_cute,(u8 *)&j);

//��������
	PokePasoParaPut(PPP,ID_clever,(u8 *)&j);

//�����܂���
	PokePasoParaPut(PPP,ID_strong,(u8 *)&j);

//�щ�
	PokePasoParaPut(PPP,ID_fur,(u8 *)&j);

//�|�P���X�N���A
	PokePasoParaPut(PPP,ID_pokerus,(u8 *)&j);

//�ߊl�f�[�^�Z�b�g
	i=GetNowWorldPos();	//�ŏ���3�̂�����ł͂���Ƃ����Ȃ�
	PokePasoParaPut(PPP,ID_get_place,(u8 *)&i);
	PokePasoParaPut(PPP,ID_get_level,(u8 *)&pokelevel);
	PokePasoParaPut(PPP,ID_get_cassette,(u8 *)&CasetteVersion);
	i=ITEM_MONSUTAABOORU;	//�f�t�H���g�̓����X�^�[�{�[���ɂ��Ă���	
	PokePasoParaPut(PPP,ID_get_ball,(u8 *)&i);

//�e�̐��ʃZ�b�g
	PokePasoParaPut(PPP,ID_oyasex,(u8 *)&MyData.my_sex);

//�p���[�����Z�b�g
	i=pp_rand();
	j=(i&(0x001f<< 0))>> 0;
	PokePasoParaPut(PPP,ID_hp_rnd,(u8 *)&j);
	j=(i&(0x001f<< 5))>> 5;
	PokePasoParaPut(PPP,ID_pow_rnd,(u8 *)&j);
	j=(i&(0x001f<<10))>>10;
	PokePasoParaPut(PPP,ID_def_rnd,(u8 *)&j);
	i=pp_rand();
	j=(i&(0x001f<< 0))>> 0;
	PokePasoParaPut(PPP,ID_agi_rnd,(u8 *)&j);
	j=(i&(0x001f<< 5))>> 5;
	PokePasoParaPut(PPP,ID_spepow_rnd,(u8 *)&j);
	j=(i&(0x001f<<10))>>10;
	PokePasoParaPut(PPP,ID_spedef_rnd,(u8 *)&j);

//�^�}�S�t���O�N���A
	j=0;
	PokePasoParaPut(PPP,ID_tamago_flag,(u8 *)&j);

//����\�̓Z�b�g
	if(PPD[pokeno].speabi2==0)
		PokePasoParaPut(PPP,ID_speabi,(u8 *)&j);
	else{
		i=pp_rand()&1;
		PokePasoParaPut(PPP,ID_speabi,(u8 *)&i);
	}

//�M��
	PokePasoParaPut(PPP,ID_stylemedal,(u8 *)&j);
	PokePasoParaPut(PPP,ID_beautifulmedal,(u8 *)&j);
	PokePasoParaPut(PPP,ID_cutemedal,(u8 *)&j);
	PokePasoParaPut(PPP,ID_clevermedal,(u8 *)&j);
	PokePasoParaPut(PPP,ID_strongmedal,(u8 *)&j);

//�Z�Z�b�g
	PokePasoWazaOboe(PPP);

}
#endif

//=============================================================
//
//	PokeParaSetChr(�|�P�����̃p�����[�^���Z�b�g����B���i�����߂����Łj
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//	pow			=	�Z�b�g����p���[����(POW_RND�̎��͗����ŃZ�b�g�j
//	chr			=	�Z�b�g���鐫�i�i�O�`�Q�S�j
//
//=============================================================

void	PokeParaSetChr(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 chr)
{
	u32	i;

	do{
		i=(pp_rand()|(pp_rand()<<16));
	}while(chr!=PokeSeikakuGetRnd(i));

	PokeParaSet(PP,pokeno,pokelevel,pow,RND_SET,i,ID_NO_SET,0);
}

//=============================================================
//
//	PokeParaSetSexChr(�|�P�����̃p�����[�^���Z�b�g����B���ʁA���i�����߂����Łj
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//	pow			=	�Z�b�g����p���[����(POW_RND�̎��͗����ŃZ�b�g�j
//	chr			=	�Z�b�g���鐫�i�i�O�`�Q�S�j
//	annon		=	�A���m�[���̌`����i0:�֌W�����j
//
//=============================================================

void	PokeParaSetSexChr(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 sex,u8 chr,u8 annon)
{
	u32	i;
	u16	annon_rnd;

	if((annon)&&(annon<29)){
		do{
			i=(pp_rand()|(pp_rand()<<16));
			annon_rnd=(((i&0x03000000)>>18)|((i&0x030000)>>12)|((i&0x0300)>>6)|(i&0x03))%28;
		}while((chr!=PokeSeikakuGetRnd(i))||
			   (sex!=PokeSexGetMonsNo(pokeno,i))||
			   (annon_rnd!=(annon-1)));
	}
	else{
		do{
			i=(pp_rand()|(pp_rand()<<16));
		}while((chr!=PokeSeikakuGetRnd(i))||(sex!=PokeSexGetMonsNo(pokeno,i)));
	}
	PokeParaSet(PP,pokeno,pokelevel,pow,RND_SET,i,ID_NO_SET,0);
}

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

void	PokeParaSetDemo(PokemonParam *PP,u16 pokeno,u8 pokelevel)
{
	u32	id;
	u32	rnd;

	do{
		id=(pp_rand()|(pp_rand()<<16));
		rnd=(pp_rand()|(pp_rand()<<16));
		if( ( ((id&0xffff0000)>>16) ^ (id&0xffff) ^ ((rnd&0xffff0000)>>16) ^ (rnd&0xffff) ) < 8 )
			continue;
	}while(MONS_MALE!=PokeSexGetMonsNo(pokeno,rnd));

	PokeParaSet(PP,pokeno,pokelevel,POW_RND,RND_SET,rnd,ID_SET,id);
}

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

void	PokeParaSetPowRnd(PokemonParam *PP,u16 pokeno,u8 pokelevel,u32 pow,u32 rnd)
{
	PokeParaSet(PP,pokeno,pokelevel,0,RND_SET,rnd,ID_NO_SET,0);

	PokeParaPut(PP,ID_power_rnd,(u8 *)&pow);

	PokeParaCalc(PP);
}

//======================================================================================
//
//	PokeParaSetPowRnd(�|�P�����̃p�����[�^���Z�b�g����B�p���[������u8 *�Ŏ󂯎���đ���j
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	pokeno		=�@�|�P�����i���o�[
//	pokelevel	=�@�|�P�����̃��x��
//	pow			=	�Z�b�g����p���[����
//
//======================================================================================

void	PokeParaSetPowRndBuf(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 *pow,u32 id)
{
	PokeParaSet(PP,pokeno,pokelevel,0,RND_NO_SET,0,ID_SET,id);

	PokeParaPut(PP,ID_hp_rnd,(u8 *)&pow[0]);
	PokeParaPut(PP,ID_pow_rnd,(u8 *)&pow[1]);
	PokeParaPut(PP,ID_def_rnd,(u8 *)&pow[2]);
	PokeParaPut(PP,ID_agi_rnd,(u8 *)&pow[3]);
	PokeParaPut(PP,ID_spepow_rnd,(u8 *)&pow[4]);
	PokeParaPut(PP,ID_spedef_rnd,(u8 *)&pow[5]);

	PokeParaCalc(PP);
}

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

void	PokeParaSetParaExp(PokemonParam *PP,u16 pokeno,u8 pokelevel,u8 pow,u8 expbit)
{
	int	i;
	int	cnt=0;
	u16	exp;
	u8	bit;

	PokeParaSet(PP,pokeno,pokelevel,pow,RND_NO_SET,0,ID_NO_SET,0);

	bit=expbit;
	for(i=0;i<6;i++){
		if(bit&1)
			cnt++;
		bit=bit>>1;
	}
	exp=PARA_EXP_TOTAL_MAX/cnt;
	bit=1;
	for(i=0;i<6;i++){
		if(expbit&bit)
			PokeParaPut(PP,ID_hp_exp+i,(u8 *)&exp);
		bit=bit<<1;
	}
	PokeParaCalc(PP);
}

//======================================================================================
//
//	PokeParaSetStock(�|�P�����̃p�����[�^���Z�b�g����B�o�g���^���[�X�g�b�N�f�[�^��p�j
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	SBTP		=	�o�g���^���[�X�g�b�N�f�[�^
//
//======================================================================================

void	PokeParaSetStock(PokemonParam *PP,StockBattleTowerPokemon *SBTP)
{
	int	i;
	u8	buf;

	PokeParaSet(PP,SBTP->monsno,SBTP->level,0,RND_SET,SBTP->personal_rnd,ID_SET,SBTP->id_no);

	for(i=0;i<4;i++)
		PokeWazaSetPos(PP,SBTP->waza[i],i);

	PokeParaPut(PP,ID_pp_count,(u8 *)&SBTP->pp_count);

	PokeParaPut(PP,ID_item,(u8 *)&SBTP->item);
//	PokeParaPut(PP,ID_id_no,(u8 *)&SBTP->id_no);
//	PokeParaPut(PP,ID_personal_rnd,(u8 *)&SBTP->personal_rnd);
	PokeParaPut(PP,ID_nickname,(u8 *)&SBTP->nickname);
	PokeParaPut(PP,ID_friend,(u8 *)&SBTP->friend);

	PokeParaPut(PP,ID_hp_exp,(u8 *)&SBTP->hp_exp);
	PokeParaPut(PP,ID_pow_exp,(u8 *)&SBTP->pow_exp);
	PokeParaPut(PP,ID_def_exp,(u8 *)&SBTP->def_exp);
	PokeParaPut(PP,ID_agi_exp,(u8 *)&SBTP->agi_exp);
	PokeParaPut(PP,ID_spepow_exp,(u8 *)&SBTP->spepow_exp);
	PokeParaPut(PP,ID_spedef_exp,(u8 *)&SBTP->spedef_exp);

	buf=SBTP->speabi;
	PokeParaPut(PP,ID_speabi,(u8 *)&buf);
	buf=SBTP->hp_rnd;
	PokeParaPut(PP,ID_hp_rnd,(u8 *)&buf);
	buf=SBTP->pow_rnd;
	PokeParaPut(PP,ID_pow_rnd,(u8 *)&buf);
	buf=SBTP->def_rnd;
	PokeParaPut(PP,ID_def_rnd,(u8 *)&buf);
	buf=SBTP->agi_rnd;
	PokeParaPut(PP,ID_agi_rnd,(u8 *)&buf);
	buf=SBTP->spepow_rnd;
	PokeParaPut(PP,ID_spepow_rnd,(u8 *)&buf);
	buf=SBTP->spedef_rnd;
	PokeParaPut(PP,ID_spedef_rnd,(u8 *)&buf);

	PokeParaCalc(PP);
}

//======================================================================================
//PokemonParam����StockBattleTowerPokemon�^�f�[�^�𐶐�
//	���o�g���^���[�E�J�[�he�g���[�i�[��p
//
//	�����F
//	PP			=	�p�����[�^���Z�b�g����\���̂̃|�C���^
//	SBTP		=	�o�g���^���[�X�g�b�N�f�[�^
//======================================================================================
void PokeParaGetStock(PokemonParam * PP, StockBattleTowerPokemon * SBTP)
{
	int j;
	u16	item;

	SBTP->monsno= PokeParaGet(PP, ID_monsno, 0);
	item=PokeParaGet(PP, ID_item, 0);
	if (item == ITEM_NAZONOMI) {
	//�Ȃ��݂̂́A�X�g�b�N���Ȃ�
		SBTP->item=0;
	} else {
		SBTP->item=item;
	}
	for (j=0;j<4;j++) {
		SBTP->waza[j] = PokeParaGet(PP, ID_waza1+j, 0);
	}
	SBTP->level =			PokeParaGet(PP, ID_level, 0);
	SBTP->pp_count =		PokeParaGet(PP, ID_pp_count, 0);
	SBTP->id_no =			PokeParaGet(PP, ID_id_no, 0);
	SBTP->hp_exp =			PokeParaGet(PP, ID_hp_exp, 0);
	SBTP->pow_exp =			PokeParaGet(PP, ID_pow_exp, 0);
	SBTP->def_exp =			PokeParaGet(PP, ID_def_exp, 0);
	SBTP->agi_exp =			PokeParaGet(PP, ID_agi_exp, 0);
	SBTP->spepow_exp =		PokeParaGet(PP, ID_spepow_exp, 0);
	SBTP->spedef_exp =		PokeParaGet(PP, ID_spedef_exp, 0);
	SBTP->friend =			PokeParaGet(PP, ID_friend, 0);
	SBTP->hp_rnd =			PokeParaGet(PP, ID_hp_rnd, 0);
	SBTP->pow_rnd =			PokeParaGet(PP, ID_pow_rnd, 0);
	SBTP->def_rnd =			PokeParaGet(PP, ID_def_rnd, 0);
	SBTP->agi_rnd =			PokeParaGet(PP, ID_agi_rnd, 0);
	SBTP->spepow_rnd =		PokeParaGet(PP, ID_spepow_rnd, 0);
	SBTP->spedef_rnd =		PokeParaGet(PP, ID_spedef_rnd, 0);
	SBTP->speabi =			PokeParaGet(PP, ID_speabi, 0);
	SBTP->personal_rnd =	PokeParaGet(PP, ID_personal_rnd, 0);
	PokeParaGet(PP, ID_nickname, SBTP->nickname);
}

//=============================================================
//
//		�|�P�����f�[�^�̃`�F�b�N�T�����Z�o
//
//=============================================================

u16	PokeParaCheckSum(PokemonPasoParam *PPP)
{
	int	i;
	u16				*CP1;
	u16				*CP2;
	u16				*CP3;
	u16				*CP4;
	u16				sum=0;

	CP1=(u16 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA1);
	CP2=(u16 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA2);
	CP3=(u16 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA3);
	CP4=(u16 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA4);

	for(i=0;i<12/2;i++)
		sum+=CP1[i];
	for(i=0;i<12/2;i++)
		sum+=CP2[i];
	for(i=0;i<12/2;i++)
		sum+=CP3[i];
	for(i=0;i<12/2;i++)
		sum+=CP4[i];

	return sum;
}

//=============================================================
//
//	PokeParaCalc�i�p�����[�^���Čv�Z����j
//
//	�����F
//	PP	=	�Čv�Z����\���̂̃|�C���^
//=============================================================

void	PokeParaCalc(PokemonParam *PP)
{
	int	oldhpmax;
	int	hp;
	int	hpmax;
	int	pow;
	int	def;
	int	agi;
	int	spepow;
	int	spedef;
	int	level;
	int	hp_rnd;
	int	hp_exp;
	int	pow_rnd;
	int	pow_exp;
	int	def_rnd;
	int	def_exp;
	int	agi_rnd;
	int	agi_exp;
	int	spepow_rnd;
	int	spepow_exp;
	int	spedef_rnd;
	int	spedef_exp;
	u16	monsno;

	oldhpmax=PokeParaGet(PP,ID_hpmax,0);
	hp=PokeParaGet(PP,ID_hp,0);
	hp_rnd=PokeParaGet(PP,ID_hp_rnd,0);
	hp_exp=PokeParaGet(PP,ID_hp_exp,0);
	pow_rnd=PokeParaGet(PP,ID_pow_rnd,0);
	pow_exp=PokeParaGet(PP,ID_pow_exp,0);
	def_rnd=PokeParaGet(PP,ID_def_rnd,0);
	def_exp=PokeParaGet(PP,ID_def_exp,0);
	agi_rnd=PokeParaGet(PP,ID_agi_rnd,0);
	agi_exp=PokeParaGet(PP,ID_agi_exp,0);
	spepow_rnd=PokeParaGet(PP,ID_spepow_rnd,0);
	spepow_exp=PokeParaGet(PP,ID_spepow_exp,0);
	spedef_rnd=PokeParaGet(PP,ID_spedef_rnd,0);
	spedef_exp=PokeParaGet(PP,ID_spedef_exp,0);

	monsno=PokeParaGet(PP,ID_monsno,0);

//���x���Z�b�g
	level=PokeLevelCalc(PP);
	PokeParaPut(PP,ID_level,(u8 *)&level);

	if(monsno==MONSNO_NUKENIN)
		hpmax=1;
	else
		hpmax=((2*PPD[monsno].basic_hp+hp_rnd+hp_exp/4)*level/100+level+10);

	UserWork[HPMAX_UP]=hpmax-oldhpmax;
	if(UserWork[HPMAX_UP]==0)
		UserWork[HPMAX_UP]=1;

	PokeParaPut(PP,ID_hpmax,(u8 *)&hpmax);

	pow=((2*PPD[monsno].basic_pow+pow_rnd+pow_exp/4)*level/100+5);
	pow=PokeChrAbiCalc(PokeSeikakuGet(PP),pow,COND_POW);
	PokeParaPut(PP,ID_pow,(u8 *)&pow);

	def=((2*PPD[monsno].basic_def+def_rnd+def_exp/4)*level/100+5);
	def=PokeChrAbiCalc(PokeSeikakuGet(PP),def,COND_DEF);
	PokeParaPut(PP,ID_def,(u8 *)&def);

	agi=((2*PPD[monsno].basic_agi+agi_rnd+agi_exp/4)*level/100+5);
	agi=PokeChrAbiCalc(PokeSeikakuGet(PP),agi,COND_AGI);
	PokeParaPut(PP,ID_agi,(u8 *)&agi);

	spepow=((2*PPD[monsno].basic_spepow+spepow_rnd+spepow_exp/4)*level/100+5);
	spepow=PokeChrAbiCalc(PokeSeikakuGet(PP),spepow,COND_SPEPOW);
	PokeParaPut(PP,ID_spepow,(u8 *)&spepow);

	spedef=((2*PPD[monsno].basic_spedef+spedef_rnd+spedef_exp/4)*level/100+5);
	spedef=PokeChrAbiCalc(PokeSeikakuGet(PP),spedef,COND_SPEDEF);
	PokeParaPut(PP,ID_spedef,(u8 *)&spedef);
	
	if(monsno==MONSNO_NUKENIN){
		if((hp==0)&&(oldhpmax!=0)) return;
		hp=1;
	}
	else{
		if((hp==0)&&(oldhpmax!=0)) return;
		if(hp==0)
			hp=hpmax-oldhpmax;
		else
			hp+=(hpmax-oldhpmax);
	}
	PokeParaPut(PP,ID_hp,(u8 *)&hp);
}

//=============================================================
//
//	PokeReplace
//
//	�{�b�N�X�̍\���̐錾����莝���̍\���̐錾�փf�[�^��
//	�R�s�[���čČv�Z������
//
//	�����F
//	PPP�@=	�{�b�N�X�̍\���̂̃|�C���^
//	PP�@=	�R�s�[����莝���̍\���̂̃|�C���^
//
//=============================================================

void	PokeReplace(PokemonPasoParam *PPP,PokemonParam *PP)
{
	u32	i=0;

	PP->PPP=*PPP;
//�R���f�B�V�����Z�b�g
	PokeParaPut(PP,ID_condition,(u8 *)&i);
//HP������
	PokeParaPut(PP,ID_hp,(u8 *)&i);
	PokeParaPut(PP,ID_hpmax,(u8 *)&i);
//MailID������
	i=NON_MAIL;
	PokeParaPut(PP,ID_mailid,(u8 *)&i);

	PokeParaCalc(PP);
}

u8	PokeLevelCalc(PokemonParam *PP)
{
	int	i;
	u16	monsno;
	u32	exp;

	monsno=PokeParaGet(PP,ID_monsno,0);
	exp=PokeParaGet(PP,ID_exp,0);

	for(i=1;i<101;i++){
		if(GrowTbl[PPD[monsno].grow][i]>exp) break;
	}
	return i-1;
}

u8	PokePasoLevelCalc(PokemonPasoParam *PPP)
{
	int	i;
	u16	monsno;
	u32	exp;

	monsno=PokePasoParaGet(PPP,ID_monsno,0);
	exp=PokePasoParaGet(PPP,ID_exp,0);

	for(i=1;i<101;i++){
		if(GrowTbl[PPD[monsno].grow][i]>exp) break;
	}
	return i-1;
}

//=============================================================
//
//	PokeWazaSet
//
//	�����Ă���ꏊ�ɋZ��ǉ�����B�����Ă��Ȃ��ꍇ�͂��̎|��Ԃ��B
//
//	�����F
//	PP	=	�Z�b�g����\���̂̃|�C���^
//	wazano	=	�Z�b�g����Z�i���o�[
//
//	�Ԃ�l�F
//	wazano:				����I��
//	NO_WAZA_OBOE:		�ꏊ���󂢂Ă��Ȃ�
//
//=============================================================

u16	PokeWazaSet(PokemonParam *PP,u16 wazano)
{
#if 0
	int	i;
	u16	waza;

	for(i=0;i<4;i++){
		if((waza=PokeParaGet(PP,ID_waza1+i,0))==0){
			PokeParaPut(PP,ID_waza1+i,(u8 *)&wazano);
			PokeParaPut(PP,ID_pp1+i,(u8 *)&WTD[wazano].pp);
			return	wazano;
		}
		else{
			//�����Z���o�����Ⴞ��
			if(waza==wazano)
				return SAME_WAZA_SET;
		}
	}
	return	NO_WAZA_SET;
#endif
	return PokePasoWazaSet(&PP->PPP,wazano);
}

u16	PokePasoWazaSet(PokemonPasoParam *PPP,u16 wazano)
{
	int	i;
	u16	waza;

	for(i=0;i<4;i++){
		if((waza=PokePasoParaGet(PPP,ID_waza1+i,0))==0){
			PokePasoParaPut(PPP,ID_waza1+i,(u8 *)&wazano);
			PokePasoParaPut(PPP,ID_pp1+i,(u8 *)&WTD[wazano].pp);
			return	wazano;
		}
		else{
			//�����Z���o�����Ⴞ��
			if(waza==wazano)
				return SAME_WAZA_SET;
		}
	}
	return	NO_WAZA_SET;
}

u16	PokeWazaSetServer(PokemonServerParam *PSP,u16 wazano)
{
	int	i;
	for(i=0;i<4;i++){
		if(PSP->waza[i]==0){
			PSP->waza[i]=wazano;
			PSP->pp[i]=WTD[wazano].pp;
			return	wazano;
		}
	}
	return	NO_WAZA_SET;
}

//=============================================================
//
//	PokeWazaSetPos
//
//�@�ꏊ���w�肵�ċZ���Z�b�g����B
//
//	�����F
//	PP	=	�Z�b�g����\���̂̃|�C���^
//	wazano	=	�Z�b�g����Z�i���o�[
//	pos	=	�Z���Z�b�g����ꏊ
//
//=============================================================

void	PokeWazaSetPos(PokemonParam *PP,u16 wazano,u8 pos)
{
	PokeParaPut(PP,ID_waza1+pos,(u8 *)&wazano);
	PokeParaPut(PP,ID_pp1+pos,(u8 *)&WTD[wazano].pp);
}

void	PokeWazaSetPosServer(PokemonServerParam *PSP,u16 wazano,u8 pos)
{
	PSP->waza[pos]=wazano;
	PSP->pp[pos]=WTD[wazano].pp;
}

//=============================================================
//
//	PokeWazaOboe
//
//�@�����X�^�[�i���o�[�ƃ��x������Z���Z�b�g����B
//
//	�����F
//	PP	=	�Z�b�g����\���̂̃|�C���^
//
//=============================================================

void	PokeWazaOboe(PokemonParam *PP)
{
#if 0
	int	i,j;
	u16	wazano[4];
	u16	monsno;
	u8	level;

	monsno=PokeParaGet(PP,ID_monsno,0);
	level=PokeParaGet(PP,ID_level,0);

	for(i=0;i<4;i++)
		wazano[i]=0;

	i=0;
	j=0;
	while(WazaOboeData[monsno][i++]!=0xffff);
	for(i--;i>-1;i--){
		if((WazaOboeData[monsno][i]&0xfe00)<=(level<<9))
			wazano[j++]=WazaOboeData[monsno][i]&0x1ff;
		if(j==4)
			break;
	}
	for(i=j-1;i>-1;i--)
		PokeWazaSet(PP,wazano[i]);
#endif
	PokePasoWazaOboe(&PP->PPP);
}

void	PokePasoWazaOboe(PokemonPasoParam *PPP)
{
	int	i;
	u16	monsno;
	u16	wazano;
	u16	ret;
	u8	level;

	monsno=PokePasoParaGet(PPP,ID_monsno,0);
	level=PokePasoLevelCalc(PPP);

	i=0;
	while(WazaOboeData[monsno][i]!=0xffff){
		if((WazaOboeData[monsno][i]&0xfe00)<=(level<<9)){
			wazano=WazaOboeData[monsno][i]&0x1ff;
			ret=PokePasoWazaSet(PPP,wazano);
			if(ret==NO_WAZA_SET)
				PokePasoWazaOboeOshidashi(PPP,wazano);
		}
		else
			break;
		i++;
	}
#if 0
	int	i,j;
	u16	wazano[4];
	u16	monsno;
	u8	level;

	monsno=PokePasoParaGet(PPP,ID_monsno,0);
	level=PokePasoLevelCalc(PPP);

	for(i=0;i<4;i++)
		wazano[i]=0;

	i=0;
	j=0;
	while(WazaOboeData[monsno][i++]!=0xffff);
	for(i--;i>-1;i--){
		if((WazaOboeData[monsno][i]&0xfe00)<=(level<<9))
			wazano[j++]=WazaOboeData[monsno][i]&0x1ff;
		if(j==4)
			break;
	}
	for(i=j-1;i>-1;i--)
		PokePasoWazaSet(PPP,wazano[i]);
#endif
}

//=============================================================
//
//	PokeWazaOboeCheck
//
//�@���x���A�b�v���Ɋo����Z���Z�b�g����B
//
//	�����F
//	PP	=	�Z�b�g����\���̂̃|�C���^
//	flg =	�J�E���^�̏����������邩���Ȃ����̃t���O�i0:���Ȃ�1:����j
//
//	�Ԃ�l�F
//	ret	=	�Z�b�g�����Z�i���o�[�i0:�o���Ȃ����� -1:�Z�������ς��j
//
//=============================================================

u16	PokeWazaOboeCheck(PokemonParam *PP,u8 flg)
{
	u16	ret=0;
	u16	monsno;
	u8	level;

	monsno=PokeParaGet(PP,ID_monsno,0);
	level=PokeParaGet(PP,ID_level,0);

	if(flg){
		wazaoboecnt=0;
		while((WazaOboeData[monsno][wazaoboecnt]&0xfe00)!=(level<<9)){
			wazaoboecnt++;
			if(WazaOboeData[monsno][wazaoboecnt]==0xffff)
				return 0;
		}
	}
	if((WazaOboeData[monsno][wazaoboecnt]&0xfe00)==(level<<9)){
		OboeWazaNo=WazaOboeData[monsno][wazaoboecnt]&0x1ff;
		wazaoboecnt++;
		ret=PokeWazaSet(PP,OboeWazaNo);
	}
	return	ret;
#if 0
	int	i;
	u16	ret=0;
	u16	monsno;
	u8	level;

	monsno=PokeParaGet(PP,ID_monsno,0);
	level=PokeParaGet(PP,ID_level,0);

	if(flg){
		wazaoboecnt=0;
		while(WazaOboeData[monsno][wazaoboecnt++]!=0xffff);
		wazaoboecnt--;
	}
	for(i=wazaoboecnt;i>-1;i--){
		if((WazaOboeData[monsno][i]&0xfe00)==(level<<9)){
			OboeWazaNo=WazaOboeData[monsno][i]&0x1ff;
			ret=PokeWazaSet(PP,OboeWazaNo);
			if(ret!=SAME_WAZA_SET){
				wazaoboecnt=i-1;
				break;
			}
			else
				ret=0;
		}
	}
	return	ret;
#endif
}

//=============================================================
//
//	PokeWazaOboeOshidashi
//
//�@�����o�����ɋZ���o����B
//
//	�����F
//	PP		=	�Z�b�g����\���̂̃|�C���^
//	wazano	=	�o����Z
//
//=============================================================

void	PokeWazaOboeOshidashi(PokemonParam *PP,u16 wazano)
{
	int	i;
	u16	waza[4];
	u8	pp[4];
	u8	ppcnt;

	for(i=0;i<3;i++){
		waza[i]=PokeParaGet(PP,ID_waza2+i,0);
		pp[i]=PokeParaGet(PP,ID_pp2+i,0);
	}
	ppcnt=PokeParaGet(PP,ID_pp_count,0);
	ppcnt=ppcnt>>2;

	waza[3]=wazano;
	pp[3]=WTD[wazano].pp;
	for(i=0;i<4;i++){
		PokeParaPut(PP,ID_waza1+i,(u8 *)&waza[i]);
		PokeParaPut(PP,ID_pp1+i,(u8 *)&pp[i]);
	}
	PokeParaPut(PP,ID_pp_count,(u8 *)&ppcnt);
}

void	PokePasoWazaOboeOshidashi(PokemonPasoParam *PPP,u16 wazano)
{
	int	i;
	u16	waza[4];
	u8	pp[4];
	u8	ppcnt;

	for(i=0;i<3;i++){
		waza[i]=PokePasoParaGet(PPP,ID_waza2+i,0);
		pp[i]=PokePasoParaGet(PPP,ID_pp2+i,0);
	}
	ppcnt=PokePasoParaGet(PPP,ID_pp_count,0);
	ppcnt=ppcnt>>2;

	waza[3]=wazano;
	pp[3]=WTD[wazano].pp;
	for(i=0;i<4;i++){
		PokePasoParaPut(PPP,ID_waza1+i,(u8 *)&waza[i]);
		PokePasoParaPut(PPP,ID_pp1+i,(u8 *)&pp[i]);
	}
	PokePasoParaPut(PPP,ID_pp_count,(u8 *)&ppcnt);
}

//=============================================================
//
//	WazaDamageCalc
//
//	�Z�̃_���[�W�v�Z������B
//
//	�����F
//	PSPA	=	�Z���o�����|�P�����̍\���̂̃|�C���^
//	PSPD	=	�Z���󂯂�|�P�����̍\���̂̃|�C���^
//	wazano	=	�v�Z����Z�i���o�[
//	cond	=	��ɂ������Ă������
//	pow		=	�Z�̈З�(0:�e�[�u�����Q�Ɓ@0�ȊO:���̒l���Q��)
//	type	=	�Z�̃^�C�v(0:�e�[�u�����Q�Ɓ@0�ȊO:���̒l���Q��)
//
//	�Ԃ�l�F
//	damage:	�v�Z���ʂ̃_���[�W��
//
//=============================================================

const u8 SoubiItemWazaTypePowUpTbl[][2]={
	{SOUBI_MUSIIRYOKUUP,MUSHI_TYPE},
	{SOUBI_HAGANEIRYOKUUP,METAL_TYPE},
	{SOUBI_ZIMENUP,JIMEN_TYPE},
	{SOUBI_IWAUP,IWA_TYPE},
	{SOUBI_KUSAUP,KUSA_TYPE},
	{SOUBI_AKUUP,AKU_TYPE},
	{SOUBI_KAKUTOUUP,BATTLE_TYPE},
	{SOUBI_DENKIUP,ELECTRIC_TYPE},
	{SOUBI_MIZUUP,WATER_TYPE},
	{SOUBI_TORIUP,HIKOU_TYPE},
	{SOUBI_DOKUBARIUP,POISON_TYPE},
	{SOUBI_KOORIUP,KOORI_TYPE},
	{SOUBI_GOOSUTOUP,GHOST_TYPE},
	{SOUBI_ESUPAAUP,SP_TYPE},
	{SOUBI_HONOOUP,FIRE_TYPE},
	{SOUBI_DORAGONUP,DRAGON_TYPE},
	{SOUBI_NOOMARUUP,NORMAL_TYPE},
};

int	WazaDamageCalc(PokemonServerParam *PSPA,
				   PokemonServerParam *PSPD,
				   int	wazano,
				   u16  cond,
				   u16	pow,
				   u8	type,
				   u8	attack,
				   u8	defence)
{
	int	i;
	s32	damage=0;
	s32	damage2=0;
	u8	wazatype;
	u16	pokepow;
	u16	pokedef;
	u16	pokespepow;
	u16	pokespedef;
	u8	eqpa;
	u8	eqpd;
	u8	atca;
	u8	atcd;

	if(pow==0)
		wazapow=WTD[wazano].damage;
	else
		wazapow=pow;

	if(type==0)
		wazatype=WTD[wazano].wazatype;
	else
		wazatype=type&0x3f;

	pokepow=PSPA->pow;
	pokedef=PSPD->def;
	pokespepow=PSPA->spepow;
	pokespedef=PSPD->spedef;

	if(PSPA->item==ITEM_NAZONOMI){
//		eqpa=Fld.RamSeed.item_equip;
//		atca=Fld.RamSeed.item_power;
		eqpa=FRS[attack].item_equip;
		atca=FRS[attack].item_power;
	}
	else{
		eqpa=ItemEquipGet(PSPA->item);
		atca=ItemAttackGet(PSPA->item);
	}

	if(PSPD->item==ITEM_NAZONOMI){
//		eqpd=Fld.RamSeed.item_equip;
//		atcd=Fld.RamSeed.item_power;
		eqpd=FRS[defence].item_equip;
		atcd=FRS[defence].item_power;
	}
	else{
		eqpd=ItemEquipGet(PSPD->item);
		atcd=ItemAttackGet(PSPD->item);
	}

	//����\�͂���������́A�U���͂�{
	if((PSPA->speabino==TOKUSYU_TIKARAMOTI)||
	//����\�̓��K�p���[�́A�U���͂�{
	   (PSPA->speabino==TOKUSYU_YOGAPAWAA))
		pokepow=pokepow*2;


	//�X�g�[���o�b�W�������Ă���Ƃ��́A�U���͂��P�O���A�b�v
	if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
	   ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0)&&
	   ((FightType&FIGHT_TYPE_CARD_E)==0)&&
	   ((FightType&FIGHT_TYPE_TRAINER)&&(FieldTrainerID!=ORUSUBAN_TRAINER))&&
		(EventFlagCheck(SYS_BATCH01_GET))&&
		(MineEnemyCheck(attack)==SIDE_MINE))
		pokepow=pokepow*110/100;

	//�o�����X�o�b�W�������Ă���Ƃ��́A�h��͂��P�O���A�b�v
	if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
	   ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0)&&
	   ((FightType&FIGHT_TYPE_CARD_E)==0)&&
	   ((FightType&FIGHT_TYPE_TRAINER)&&(FieldTrainerID!=ORUSUBAN_TRAINER))&&
		(EventFlagCheck(SYS_BATCH05_GET))&&
		(MineEnemyCheck(defence)==SIDE_MINE))
		pokedef=pokedef*110/100;

	//�}�C���h�o�b�W�������Ă���Ƃ��́A���U���P�O���A�b�v
	if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
	   ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0)&&
	   ((FightType&FIGHT_TYPE_CARD_E)==0)&&
	   ((FightType&FIGHT_TYPE_TRAINER)&&(FieldTrainerID!=ORUSUBAN_TRAINER))&&
		(EventFlagCheck(SYS_BATCH07_GET))&&
		(MineEnemyCheck(attack)==SIDE_MINE)){
		pokespepow=pokespepow*110/100;
	}

	//�}�C���h�o�b�W�������Ă���Ƃ��́A���h���P�O���A�b�v
	if(((FightType&FIGHT_TYPE_TUUSHIN)==0)&&
	   ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0)&&
	   ((FightType&FIGHT_TYPE_CARD_E)==0)&&
	   ((FightType&FIGHT_TYPE_TRAINER)&&(FieldTrainerID!=ORUSUBAN_TRAINER))&&
		(EventFlagCheck(SYS_BATCH07_GET))&&
		(MineEnemyCheck(defence)==SIDE_MINE)){
		pokespedef=pokespedef*110/100;
	}

#if 0
	//�A�C�e���Œ��З̓A�b�v�̑������ʂ̂Ƃ��́A�З́����U���̓A�b�v
	if((eqpa==SOUBI_MUSIIRYOKUUP)&&
	   (wazatype==MUSHI_TYPE)){
		pokepow=pokepow*(100+atca)/100;
	}

	//�A�C�e���ł͂��ˈЗ̓A�b�v�̑������ʂ̂Ƃ��́A�З́����U���̓A�b�v
	if((eqpa==SOUBI_HAGANEIRYOKUUP)&&
	   (wazatype==METAL_TYPE)){
		pokepow=pokepow*(100+atca)/100;
	}
#endif

	//�A�C�e���ŋZ�^�C�v�ɂ��З̓A�b�v�̑������ʂ̂Ƃ��́A�З́����U���̓A�b�v
	for(i=0;i<NELEMS(SoubiItemWazaTypePowUpTbl);i++){
		if((eqpa==SoubiItemWazaTypePowUpTbl[i][0])&&
		   (wazatype==SoubiItemWazaTypePowUpTbl[i][1])){
			if(wazatype<HATE_TYPE)		//�����U��
				pokepow=pokepow*(100+atca)/100;
			else
				pokespepow=pokespepow*(100+atca)/100;
			break;
		}
	}

	//�A�C�e���œ����Z���������Ȃ��̑������ʂ̂Ƃ��́A�U����1.5�{�A�b�v
	if(eqpa==SOUBI_ONAZIWAZAONLY){
		pokepow=pokepow*150/100;
	}

	//�A�C�e���œ��U�����h��1.5�{�̑������ʂ̂Ƃ��́A���U�����h1.5�{�A�b�v
	if((eqpa==SOUBI_TOKUKOUTOKUBOUUP)&&
	  ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0)&&
	  ((PSPA->monsno==MONSNO_RATHIOSU)||(PSPA->monsno==MONSNO_RATHIASU)))
		pokespepow=pokespepow*150/100;

	//�A�C�e���œ��U�����h��1.5�{�̑������ʂ̂Ƃ��́A���U�����h1.5�{�A�b�v
	if((eqpd==SOUBI_TOKUKOUTOKUBOUUP)&&
	  ((FightType&FIGHT_TYPE_BATTLE_TOWER)==0)&&
	  ((PSPD->monsno==MONSNO_RATHIOSU)||(PSPD->monsno==MONSNO_RATHIASU)))
		pokespedef=pokespedef*150/100;

	//�A�C�e���Ńp�[���������Ɠ��U��2�{�̑������ʂ̂Ƃ��́A���U2�{�A�b�v
	if((eqpa==SOUBI_PAARURUTOKUKOUNIBAI)&&
	   (PSPA->monsno==MONSNO_PAARURU)){
		pokespepow*=2;
	}

	//�A�C�e���Ńp�[���������Ɠ��h��2�{�̑������ʂ̂Ƃ��́A���h2�{�A�b�v
	if((eqpd==SOUBI_PAARURUTOKUBOUNIBAI)&&
	   (PSPD->monsno==MONSNO_PAARURU)){
		pokespedef*=2;
	}

	//�A�C�e���Ńs�J�`���E�����Ɠ��U��2�{�̑������ʂ̂Ƃ��́A���U2�{�A�b�v
	if((eqpa==SOUBI_PIKATYUUTOKUKOUNIBAI)&&
	   (PSPA->monsno==MONSNO_PIKATYUU)){
		pokespepow*=2;
	}

	//�A�C�e���Ń��^���������Ɩh��͂�2�{�̑������ʂ̂Ƃ��́A�h���2�{�A�b�v
	if((eqpd==SOUBI_METAMONDATOBOUGYORYOKUUP)&&
	   (PSPD->monsno==MONSNO_METAMON)){
		pokedef*=2;
	}

	//�A�C�e���ŃJ���J���A�K���K�������ƍU���͂�2�{�̑������ʂ̂Ƃ��́A�U����2�{�A�b�v
	if((eqpa==SOUBI_KARAGARADATOKOUGEKINIBAI)&&
	  ((PSPA->monsno==MONSNO_KARAKARA)||
	   (PSPA->monsno==MONSNO_GARAGARA))){
		pokepow*=2;
	}

	//����\�͂������ڂ��́A�ق̂��A������ɂ悢
	if((PSPD->speabino==TOKUSYU_ATUISIBOU)&&
	  ((wazatype==FIRE_TYPE)||(wazatype==KOORI_TYPE)))
		pokespepow/=2;

	//����\�͂͂肫��́A�U����1.5�{�A�b�v
	if(PSPA->speabino==TOKUSYU_HARIKIRI){
		pokepow=pokepow*150/100;
	}

	//����\�̓v���X�A�}�C�i�X�͂��݂�����ɂ���Ɠ���U���͂�1.5�{�A�b�v
	if((PSPA->speabino==TOKUSYU_PURASU)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI,0,TOKUSYU_MAINASU,0,0))){
		pokespepow=pokespepow*150/100;
	}
	if((PSPA->speabino==TOKUSYU_MAINASU)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI,0,TOKUSYU_PURASU,0,0))){
		pokespepow=pokespepow*150/100;
	}

	//����\�͂��񂶂傤�́A��Ԉُ�̂Ƃ��ɍU����1.5�{�A�b�v
	if((PSPA->speabino==TOKUSYU_KONZYOU)&&
	   (PSPA->condition)){
		pokepow=pokepow*150/100;
	}

	//����\�͂ӂ����Ȃ��낱�́A��Ԉُ�̂Ƃ��ɖh���1.5�{�A�b�v
	if((PSPD->speabino==TOKUSYU_HUSIGINAUROKO)&&
	   (PSPD->condition)){
		pokedef=pokedef*150/100;
	}

	//�Z���ʂǂ날���т́A�d�C�n�̋Z�̈З͂𔼌�
	if((wazatype==ELECTRIC_TYPE)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI,0,0,TOKUSYU_DOROASOBI,0))){
		wazapow/=2;
	}

	//�Z���ʂ݂������т́A���n�̋Z�̈З͂𔼌�
	if((wazatype==FIRE_TYPE)&&
	   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI,0,0,TOKUSYU_MIZUASOBI,0))){
		wazapow/=2;
	}

	//����\�͂����傭��HP1/3�ȉ��ő��^�C�v�̈З͂�1.5�{�ɂ���
	if((wazatype==KUSA_TYPE)&&
	   (PSPA->speabino==TOKUSYU_SINRYOKU)&&
	   (PSPA->hp<=(PSPA->hpmax/3))){
		wazapow=wazapow*150/100;
	}

	//����\�͂�������HP1/3�ȉ��ŉ��^�C�v�̈З͂�1.5�{�ɂ���
	if((wazatype==FIRE_TYPE)&&
	   (PSPA->speabino==TOKUSYU_MOUKA)&&
	   (PSPA->hp<=(PSPA->hpmax/3))){
		wazapow=wazapow*150/100;
	}

	//����\�͂�����イ��HP1/3�ȉ��Ő��^�C�v�̈З͂�1.5�{�ɂ���
	if((wazatype==WATER_TYPE)&&
	   (PSPA->speabino==TOKUSYU_GEKIRYUU)&&
	   (PSPA->hp<=(PSPA->hpmax/3))){
		wazapow=wazapow*150/100;
	}
	//����\�͂ނ��̂��点��HP1/3�ȉ��Œ��^�C�v�̈З͂�1.5�{�ɂ���
	if((wazatype==MUSHI_TYPE)&&
	   (PSPA->speabino==TOKUSYU_MUSINOSIRASE)&&
	   (PSPA->hp<=(PSPA->hpmax/3))){
		wazapow=wazapow*150/100;
	}
	//�u���΂��v�u�����΂��͂v�́A�h��͂𔼕��ɂ��Čv�Z����
	if(WTD[WazaNo].battleeffect==7)
		pokedef=pokedef/2;

	if(wazatype<HATE_TYPE){		//�����U��
		if(CriticalValue==2){
			if(PSPA->abiritycnt[COND_POW]>6){
				damage=pokepow*CondChgTable[PSPA->abiritycnt[COND_POW]][0];
				damage/=CondChgTable[PSPA->abiritycnt[COND_POW]][1];
			}
			else
				damage=pokepow;
		}
		else{
			damage=pokepow*CondChgTable[PSPA->abiritycnt[COND_POW]][0];
			damage/=CondChgTable[PSPA->abiritycnt[COND_POW]][1];
		}

		damage*=wazapow;
		damage*=(PSPA->level*2/5+2);

		if(CriticalValue==2){
			if(PSPD->abiritycnt[COND_DEF]<6){
				damage2=pokedef*CondChgTable[PSPD->abiritycnt[COND_DEF]][0];
				damage2/=CondChgTable[PSPD->abiritycnt[COND_DEF]][1];
			}
			else
				damage2=pokedef;
		}
		else{
			damage2=pokedef*CondChgTable[PSPD->abiritycnt[COND_DEF]][0];
			damage2/=CondChgTable[PSPD->abiritycnt[COND_DEF]][1];
		}

		damage/=damage2;
		damage/=50;

		/*
		damage=
		(PSPA->pow*
		 CondChgTable[PSPA->abiritycnt[COND_POW]][0]/
		 CondChgTable[PSPA->abiritycnt[COND_POW]][1])*
		wazapow*(PSPA->level*2/5+2)/
		(PSPD->def*
		 CondChgTable[PSPD->abiritycnt[COND_DEF]][0]/
		 CondChgTable[PSPD->abiritycnt[COND_DEF]][1])/50;
		 */

		//����\�͂��񂶂傤�́A��Ԉُ�ōU���͂�������̂ł��̔���͖���
		if((PSPA->condition&CONDITION_YAKEDO)&&
		   (PSPA->speabino!=TOKUSYU_KONZYOU))
				damage/=2;

		if(((cond&SIDE_CONDITION_BUTSURIGUARD)!=0)&&
			(CriticalValue==1)){
//			(CriticalValue==1)&&
//			(WTD[WazaNo].battleeffect!=186)){	//186�̓K�[�h�̉e�����󂯂Ȃ�
			if((FightType&FIGHT_TYPE_2vs2)&&
			   (HitCountCheck(HCC_DEFENCE)==2))
				damage=damage/3*2;
			else
				damage/=2;
		}

		if((FightType&FIGHT_TYPE_2vs2)&&
		   (WTD[wazano].attackrange==RANGE_DOUBLE)&&
		   (HitCountCheck(HCC_DEFENCE)==2))
			damage/=2;

		if(damage==0) damage=1;
	}
	if(wazatype==HATE_TYPE){				//�C���[�K���U��
		damage=0;							//�b��
	}
	if(wazatype>HATE_TYPE){					//����U��
		if(CriticalValue==2){
			if(PSPA->abiritycnt[COND_SPEPOW]>6){
				damage=pokespepow*CondChgTable[PSPA->abiritycnt[COND_SPEPOW]][0];
				damage/=CondChgTable[PSPA->abiritycnt[COND_SPEPOW]][1];
			}
			else
				damage=pokespepow;
		}
		else{
			damage=pokespepow*CondChgTable[PSPA->abiritycnt[COND_SPEPOW]][0];
			damage/=CondChgTable[PSPA->abiritycnt[COND_SPEPOW]][1];
		}

		damage*=wazapow;
		damage*=(PSPA->level*2/5+2);

		if(CriticalValue==2){
			if(PSPD->abiritycnt[COND_SPEDEF]<6){
				damage2=pokespedef*CondChgTable[PSPD->abiritycnt[COND_SPEDEF]][0];
				damage2/=CondChgTable[PSPD->abiritycnt[COND_SPEDEF]][1];
			}
			else
				damage2=pokespedef;
		}
		else{
			damage2=pokespedef*CondChgTable[PSPD->abiritycnt[COND_SPEDEF]][0];
			damage2/=CondChgTable[PSPD->abiritycnt[COND_SPEDEF]][1];
		}

		damage/=damage2;
		damage/=50;
		/*
		damage=
		(PSPA->spepow*
		 CondChgTable[PSPA->abiritycnt[COND_SPEPOW]][0]/
		 CondChgTable[PSPA->abiritycnt[COND_SPEPOW]][1])*
		wazapow*(PSPA->level*2/5+2)/
		(PSPD->spedef*
		 CondChgTable[PSPD->abiritycnt[COND_SPEDEF]][0]/
		 CondChgTable[PSPD->abiritycnt[COND_SPEDEF]][1])/50;
		 */
		if(((cond&SIDE_CONDITION_TOKUSYUGUARD)!=0)&&
			(CriticalValue==1)){
//			(CriticalValue==1)&&
//			(WTD[WazaNo].battleeffect!=186)){	//186�̓K�[�h�̉e�����󂯂Ȃ�
			if((FightType&FIGHT_TYPE_2vs2)&&
			   (HitCountCheck(HCC_DEFENCE)==2))
				damage=damage/3*2;
			else
				damage/=2;
		}
		if((FightType&FIGHT_TYPE_2vs2)&&
		   (WTD[wazano].attackrange==RANGE_DOUBLE)&&
		   (HitCountCheck(HCC_DEFENCE)==2))
			damage/=2;

		//�V��`�F�b�N�i�̂��Ă񂫂̃|�P�������퓬�ɏo�Ă���ꍇ�͓V��`�F�b�N���Ȃ��j
		if((SpecialAbirityCheck(SPEABI_HAVE_SPEABI,0,TOKUSYU_NOOTENKI,0,0)==0)&&
		   (SpecialAbirityCheck(SPEABI_HAVE_SPEABI,0,TOKUSYU_EAROKKU,0,0)==0)){
			if(FieldCondition&FIELD_CONDITION_AME){
				switch(wazatype){
					case FIRE_TYPE:
						damage/=2;
						break;
					case WATER_TYPE:
						damage=damage*15/10;
						break;
				}
			}

			if((FieldCondition&FIELD_CONDITION_NO_SOLAR)&&
			   (WazaNo==WAZANO_SOORAABIIMU))
						damage/=2;
#if 0
			if(FieldCondition&FIELD_CONDITION_OOAME){
				switch(wazatype){
					case FIRE_TYPE:
						damage/=4;
						break;
					case WATER_TYPE:
						damage*=2;
						break;
				}
				switch(WazaNo){
					case WAZANO_SOORAABIIMU:
						damage/=4;
						break;
				}
			}
#endif
			if(FieldCondition&FIELD_CONDITION_HIDERING){
				switch(wazatype){
					case FIRE_TYPE:
						damage=damage*15/10;
						break;
					case WATER_TYPE:
						damage/=2;
						break;
				}
			}
		}
		//����\�͂��炢�у`�F�b�N
		if((SAD_WORK->abirityflag[attack]&SAD_MORAIBI)&&
		   (wazatype==FIRE_TYPE)){
			damage=damage*15/10;
		}

//		if(damage==0) damage=1;
	}
	return	damage+2;
}

u8	HitCountCheck(u8 flag)
{
	int	i;
	u8	ret=0;

	switch(flag){
		case HCC_NORMAL:
			for(i=0;i<4;i++){
				if((i!=ClientNo)&&
				  ((NoReshuffleClient&no2bittbl[i])==0))
					ret++;
			}
			break;
		case HCC_ATTACK:
			for(i=0;i<4;i++){
				if((MineEnemyCheck(i)==MineEnemyCheck(AttackClient))&&
				  ((NoReshuffleClient&no2bittbl[i])==0))
					ret++;
			}
			break;
		case HCC_DEFENCE:
			for(i=0;i<4;i++){
				if((MineEnemyCheck(i)==MineEnemyCheck(DefenceClient))&&
				  ((NoReshuffleClient&no2bittbl[i])==0))
					ret++;
			}
			break;
	}
	return ret;
}

u8	GetDefenceClient(u8 no)
{
	u8	dir;

	dir=(ClientTypeGet(no)&0x01)^0x01;
	if((FightType&FIGHT_TYPE_2vs2)==0)
		return ClientNoGet(dir);
	if(HitCountCheck(HCC_NORMAL)>1)
		return ClientNoGet((pp_rand()&1)?dir:dir^2);
	if(NoReshuffleClient&no2bittbl[dir])
		return ClientNoGet(dir^2);
	else
		return ClientNoGet(dir);
}

u8	PokeSexGet(PokemonParam *PP)
{
	return(PokePasoSexGet((PokemonPasoParam *)&PP->PPP));
}

u8	PokePasoSexGet(PokemonPasoParam *PPP)
{
	u16	monsno;
	u32	rnd;

	monsno=PokePasoParaGet(PPP,ID_monsno,0);
	rnd=PokePasoParaGet(PPP,ID_personal_rnd,0);

	switch(PPD[monsno].sex){
		case	MONS_MALE:
		case	MONS_FEMALE:
		case	MONS_UNKNOWN:
			return	PPD[monsno].sex;
	}
	if(PPD[monsno].sex>(rnd&0xff))
		return	MONS_FEMALE;
	else
		return	MONS_MALE;
}

u8	PokeSexGetMonsNo(u16 no,u32 rnd)
{
	switch(PPD[no].sex){
		case	MONS_MALE:
		case	MONS_FEMALE:
		case	MONS_UNKNOWN:
			return	PPD[no].sex;
	}
	if(PPD[no].sex>(rnd&0xff))
		return	MONS_FEMALE;
	else
		return	MONS_MALE;
}

extern const actAffAnm * const PmAffAnmTblB[];
extern const actAffAnm * const PmAffAnmTblF[];
extern const actAnmData actAnmTransDataTableF[];
extern const actAnmData actAnmTransDataTableB[];
extern const actAnmData actAnmTransDataTableF2[];
extern const actAnmData actAnmTransDataTableB2[];
extern const actAnm *const PokeActAnmTbl[];
extern const actAnm *const *const TRBAnmTbl[];
extern const actAnm *const *const TRFAnmTbl[];


const actHeader PAHMTbl[]={
	{
	-1,
	0,
	&MineOamData,
	0,
	actAnmTransDataTableB,
	PmAffAnmTblB,
	SeqMinePokeAppearInit,
	},
	{
	-1,
	0,
	&EnemyOamData,
	0,
	actAnmTransDataTableF,
	PmAffAnmTblF,
	SeqEnemyPokeAppearInit,
	},
	{
	-1,
	0,
	&MineOamData,
	0,
	actAnmTransDataTableB2,
	PmAffAnmTblB,
	SeqMinePokeAppearInit,
	},
	{
	-1,
	0,
	&EnemyOamData,
	0,
	actAnmTransDataTableF2,
	PmAffAnmTblF,
	SeqEnemyPokeAppearInit,
	},
};

// ��2������no��ClientTypeGet�Ŏ擾�ł���l�ł� by matsuda
void	PokeActHeaderMake(u16 monsno,u8 no)
{
	PokeActHeader=PAHMTbl[no];
	PokeActHeader.palNo=monsno;
	PokeActHeader.anmData_p = PokeActAnmTbl;
}

void	TrainerActHeaderMake(u16 monsno,u8 no)
{
	PokeActHeader=PAHMTbl[no];
	PokeActHeader.palNo=monsno;

	if(no == BSS_CLIENT_MINE || no == BSS_CLIENT_MINE2)
		PokeActHeader.anmData_p = TRBAnmTbl[monsno];
	else
		PokeActHeader.anmData_p = TRFAnmTbl[monsno];
}

//=========================================================================
//	�|�P�����p�����[�^�Í���
//=========================================================================

void	PokeParaCoded(PokemonPasoParam *PPP)
{
	int	i;
	u32	*data;

	data=(u32 *)&PPP->paradata;

	for(i=0;
		i<(sizeof(PokemonPara1)+sizeof(PokemonPara2)+sizeof(PokemonPara3)+sizeof(PokemonPara4))/4;
		i++){
		data[i]^=PPP->personal_rnd;
		data[i]^=PPP->id_no;
	}
}

//=========================================================================
//	�|�P�����p�����[�^������
//=========================================================================

void	PokeParaDecoded(PokemonPasoParam *PPP)
{
	int	i;
	u32	*data;

	data=(u32 *)&PPP->paradata;

	for(i=0;
		i<(sizeof(PokemonPara1)+sizeof(PokemonPara2)+sizeof(PokemonPara3)+sizeof(PokemonPara4))/4;
		i++){
		data[i]^=PPP->id_no;
		data[i]^=PPP->personal_rnd;
	}
}

//=========================================================================
//	�|�P�����p�����[�^�A�h���X�擾
//=========================================================================

u32		PokeParaAdrsGet(PokemonPasoParam *PPP,u32 rnd,u8 id)
{
	u32	ret=0;

	PokemonParaData11	*PPD11;
	PokemonParaData12	*PPD12;
	PokemonParaData13	*PPD13;
	PokemonParaData14	*PPD14;
	PokemonParaData15	*PPD15;
	PokemonParaData16	*PPD16;
	PokemonParaData21	*PPD21;
	PokemonParaData22	*PPD22;
	PokemonParaData23	*PPD23;
	PokemonParaData24	*PPD24;
	PokemonParaData25	*PPD25;
	PokemonParaData26	*PPD26;
	PokemonParaData31	*PPD31;
	PokemonParaData32	*PPD32;
	PokemonParaData33	*PPD33;
	PokemonParaData34	*PPD34;
	PokemonParaData35	*PPD35;
	PokemonParaData36	*PPD36;
	PokemonParaData41	*PPD41;
	PokemonParaData42	*PPD42;
	PokemonParaData43	*PPD43;
	PokemonParaData44	*PPD44;
	PokemonParaData45	*PPD45;
	PokemonParaData46	*PPD46;

	PPD11=(PokemonParaData11 *)PPP->paradata;
	PPD12=(PokemonParaData12 *)PPP->paradata;
	PPD13=(PokemonParaData13 *)PPP->paradata;
	PPD14=(PokemonParaData14 *)PPP->paradata;
	PPD15=(PokemonParaData15 *)PPP->paradata;
	PPD16=(PokemonParaData16 *)PPP->paradata;
	PPD21=(PokemonParaData21 *)PPP->paradata;
	PPD22=(PokemonParaData22 *)PPP->paradata;
	PPD23=(PokemonParaData23 *)PPP->paradata;
	PPD24=(PokemonParaData24 *)PPP->paradata;
	PPD25=(PokemonParaData25 *)PPP->paradata;
	PPD26=(PokemonParaData26 *)PPP->paradata;
	PPD31=(PokemonParaData31 *)PPP->paradata;
	PPD32=(PokemonParaData32 *)PPP->paradata;
	PPD33=(PokemonParaData33 *)PPP->paradata;
	PPD34=(PokemonParaData34 *)PPP->paradata;
	PPD35=(PokemonParaData35 *)PPP->paradata;
	PPD36=(PokemonParaData36 *)PPP->paradata;
	PPD41=(PokemonParaData41 *)PPP->paradata;
	PPD42=(PokemonParaData42 *)PPP->paradata;
	PPD43=(PokemonParaData43 *)PPP->paradata;
	PPD44=(PokemonParaData44 *)PPP->paradata;
	PPD45=(PokemonParaData45 *)PPP->paradata;
	PPD46=(PokemonParaData46 *)PPP->paradata;

	switch(rnd%24){
		case ID_POKEPARADATA11:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD11->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD11->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD11->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD11->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA12:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD12->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD12->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD12->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD12->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA13:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD13->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD13->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD13->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD13->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA14:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD14->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD14->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD14->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD14->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA15:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD15->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD15->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD15->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD15->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA16:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD16->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD16->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD16->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD16->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA21:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD21->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD21->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD21->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD21->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA22:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD22->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD22->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD22->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD22->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA23:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD23->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD23->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD23->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD23->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA24:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD24->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD24->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD24->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD24->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA25:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD25->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD25->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD25->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD25->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA26:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD26->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD26->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD26->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD26->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA31:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD31->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD31->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD31->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD31->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA32:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD32->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD32->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD32->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD32->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA33:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD33->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD33->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD33->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD33->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA34:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD34->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD34->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD34->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD34->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA35:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD35->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD35->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD35->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD35->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA36:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD36->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD36->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD36->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD36->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA41:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD41->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD41->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD41->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD41->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA42:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD42->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD42->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD42->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD42->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA43:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD43->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD43->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD43->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD43->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA44:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD44->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD44->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD44->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD44->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA45:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD45->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD45->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD45->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD45->PP4;
					break;
			}
			break;
		case ID_POKEPARADATA46:
			switch(id){
				case ID_POKEPARA1:
					ret=(u32)&PPD46->PP1;
					break;
				case ID_POKEPARA2:
					ret=(u32)&PPD46->PP2;
					break;
				case ID_POKEPARA3:
					ret=(u32)&PPD46->PP3;
					break;
				case ID_POKEPARA4:
					ret=(u32)&PPD46->PP4;
					break;
			}
			break;
	}
	return	ret;
}

//=========================================================================
//	�|�P�����p�����[�^�擾�i�Í����΍�j
//=========================================================================

u32	PokeParaGet(PokemonParam *PP,int id,u8 *buf)
{
	u32	ret=0;

	switch(id){
		case ID_condition:
			ret=PP->condition;
			break;
		case ID_level:
			ret=PP->level;
			break;
		case ID_hp:
			ret=PP->hp;
			break;
		case ID_hpmax:
			ret=PP->hpmax;
			break;
		case ID_pow:
			ret=PP->pow;
			break;
		case ID_def:
			ret=PP->def;
			break;
		case ID_agi:
			ret=PP->agi;
			break;
		case ID_spepow:
			ret=PP->spepow;
			break;
		case ID_spedef:
			ret=PP->spedef;
			break;
		case ID_mailid:
			ret=PP->mailid;
			break;
		default:
			ret=PokePasoParaGet((PokemonPasoParam *)&PP->PPP,id,buf);
	}

	return	ret;
}

u32	PokePasoParaGet(PokemonPasoParam *PPP,int id,u8 *buf)
{
	int	i;
	u32	ret=0;
	PokemonPara1		*PP1=0;
	PokemonPara2		*PP2=0;
	PokemonPara3		*PP3=0;
	PokemonPara4		*PP4=0;
	u16	sum;
	u16	*wazano;

	if(id>ID_amari){
		PP1=(PokemonPara1 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA1);
		PP2=(PokemonPara2 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA2);
		PP3=(PokemonPara3 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA3);
		PP4=(PokemonPara4 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA4);

//�`�F�b�N�T���ł͕��������Ȃ��̂ŁA���ӁI�I
		PokeParaDecoded(PPP);
		sum=PokeParaCheckSum(PPP);

		if(sum!=PPP->checksum){
			PPP->fusei_tamago_flag=1;
			PPP->egg_exist_flag=1;
			PP4->tamago_flag=1;
		}
	}

	switch(id){
		case ID_personal_rnd:
			ret=PPP->personal_rnd;
			break;
		case ID_id_no:
			ret=PPP->id_no;
			break;
		case ID_nickname:
			if(PPP->fusei_tamago_flag){
				for(ret=0;ret<5;ret++)
					buf[ret]=HuseiNaTamagoMsg[ret];
			}
			else{
//#if PM_LANG==LANG_JAPAN
#if 0
				//���{�ł͋����I��5�����Ő؂�
				for(ret=0;ret<5;ret++)
					buf[ret]=PPP->nickname[ret];
#else
				for(ret=0;ret<MONS_NAME_SIZE;ret++)
					buf[ret]=PPP->nickname[ret];
#endif
			}
			buf[ret]=EOM_;
			break;
		case ID_country_code:
			ret=PPP->country_code;
			break;
		case ID_fusei_tamago_flag:
			ret=PPP->fusei_tamago_flag;
			break;
		case ID_poke_exist_flag:
			ret=PPP->poke_exist_flag;
			break;
		case ID_egg_exist_flag:
			ret=PPP->egg_exist_flag;
			break;
		case ID_oyaname:
			for(ret=0;ret<PERSON_NAME_SIZE;ret++)
				buf[ret]=PPP->oyaname[ret];
			buf[ret]=EOM_;
			break;
		case ID_mark:
			ret=PPP->mark;
			break;
		case ID_checksum:
			ret=PPP->checksum;
			break;
		case ID_amari:
			ret=PPP->amari;
			break;

		case ID_monsno:
			if(PPP->fusei_tamago_flag)
				ret=MONSNO_TAMAGO;
			else
				ret=PP1->monsno;
			break;
		case ID_item:
			ret=PP1->item;
			break;
		case ID_exp:
			ret=PP1->exp;
			break;
		case ID_pp_count:
			ret=PP1->pp_count;
			break;
		case ID_friend:
			ret=PP1->friend;
			break;

		case ID_waza1:
		case ID_waza2:
		case ID_waza3:
		case ID_waza4:
			ret=PP2->waza[id-ID_waza1];
			break;
		case ID_pp1:
		case ID_pp2:
		case ID_pp3:
		case ID_pp4:
			ret=PP2->pp[id-ID_pp1];
			break;

		case ID_hp_exp:
			ret=PP3->hp_exp;
			break;
		case ID_pow_exp:
			ret=PP3->pow_exp;
			break;
		case ID_def_exp:
			ret=PP3->def_exp;
			break;
		case ID_agi_exp:
			ret=PP3->agi_exp;
			break;
		case ID_spepow_exp:
			ret=PP3->spepow_exp;
			break;
		case ID_spedef_exp:
			ret=PP3->spedef_exp;
			break;
		case ID_style:
			ret=PP3->style;
			break;
		case ID_beautiful:
			ret=PP3->beautiful;
			break;
		case ID_cute:
			ret=PP3->cute;
			break;
		case ID_clever:
			ret=PP3->clever;
			break;
		case ID_strong:
			ret=PP3->strong;
			break;
		case ID_fur:
			ret=PP3->fur;
			break;

		case ID_pokerus:
			ret=PP4->pokerus;
			break;
		case ID_get_place:
			ret=PP4->get_place;
			break;
		case ID_get_level:
			ret=PP4->get_level;
			break;
		case ID_get_cassette:
			ret=PP4->get_cassette;
			break;
		case ID_get_ball:
			ret=PP4->get_ball;
			break;
		case ID_oyasex:
			ret=PP4->oyasex;
			break;
		case ID_hp_rnd:
			ret=PP4->hp_rnd;
			break;
		case ID_pow_rnd:
			ret=PP4->pow_rnd;
			break;
		case ID_def_rnd:
			ret=PP4->def_rnd;
			break;
		case ID_agi_rnd:
			ret=PP4->agi_rnd;
			break;
		case ID_spepow_rnd:
			ret=PP4->spepow_rnd;
			break;
		case ID_spedef_rnd:
			ret=PP4->spedef_rnd;
			break;
		case ID_tamago_flag:
			ret=PP4->tamago_flag;
			break;
		case ID_speabi:
			ret=PP4->speabi;
			break;
		case	ID_stylemedal:		//12h	�������悳�M��
			ret=PP4->stylemedal;
			break;
		case	ID_beautifulmedal:		//13h	���������M��
			ret=PP4->beautifulmedal;
			break;
		case	ID_cutemedal:		//14h	���킢���M��
			ret=PP4->cutemedal;
			break;
		case	ID_clevermedal:		//24h	���������M��
			ret=PP4->clevermedal;
			break;
		case	ID_strongmedal:		//2fh	�����܂����M��
			ret=PP4->strongmedal;
			break;
		case	ID_champ_ribbon:			//�`�����v���{��
			ret = PP4->champ_ribbon;
			break;
		case	ID_winning_ribbon:		//�E�B�j���O���{��
			ret = PP4->winning_ribbon;
			break;
		case	ID_victory_ribbon:		//�r�N�g���[���{��
			ret = PP4->victory_ribbon;
			break;
		case	ID_bromide_ribbon:		//�u���}�C�h���{��
			ret = PP4->bromide_ribbon;
			break;
		case	ID_ganba_ribbon:			//����΃��{��
			ret = PP4->ganba_ribbon;
			break;
		case	ID_marine_ribbon:		//�}�������{��
			ret = PP4->marine_ribbon;
			break;
		case	ID_land_ribbon:			//�����h���{��
			ret = PP4->land_ribbon;
			break;
		case	ID_sky_ribbon:			//�X�J�C���{��
			ret = PP4->sky_ribbon;
			break;
		case	ID_country_ribbon:		//�J���g���[���{��
			ret = PP4->country_ribbon;
			break;
		case	ID_national_ribbon:		//�i�V���i�����{��
			ret = PP4->national_ribbon;
			break;
		case	ID_earth_ribbon:			//�A�[�X���{��
			ret = PP4->earth_ribbon;
			break;
		case	ID_world_ribbon:			//���[���h���{��
			ret = PP4->world_ribbon;
			break;
		case	ID_amari_ribbon:			//���܂�
			ret = PP4->amari_ribbon;
			break;

		case ID_monsno_egg:
			ret=PP1->monsno;
			if(ret==0)
				break;
			if((PP4->tamago_flag)||
			   (PPP->fusei_tamago_flag))
				ret=MONSNO_TAMAGO;
			break;
		case ID_power_rnd:
			ret=(PP4->hp_rnd	<< 0)|
				(PP4->pow_rnd	<< 5)|
				(PP4->def_rnd	<<10)|
				(PP4->agi_rnd	<<15)|
				(PP4->spepow_rnd<<20)|
				(PP4->spedef_rnd<<25);
			break;
		case ID_waza_have_check:
			wazano=(u16 *)buf;
			i=0;
			if((PP1->monsno)&&(PP4->tamago_flag==0)){
				while(wazano[i]!=WAZANO_MAX+1){
					if((PP2->waza[0]==wazano[i])||
					   (PP2->waza[1]==wazano[i])||
					   (PP2->waza[2]==wazano[i])||
					   (PP2->waza[3]==wazano[i]))
						ret|=no2bittbl[i];
					i++;
				}
			}
			break;

		// ���{���̑����J�E���g   taya
		case ID_ribbon_cnt:
			ret = 0;
			if((PP1->monsno)&&(PP4->tamago_flag==0)){
				ret += PP4->stylemedal;
				ret += PP4->beautifulmedal;
				ret += PP4->cutemedal;
				ret += PP4->clevermedal;
				ret += PP4->strongmedal;
				ret += PP4->champ_ribbon;
				ret += PP4->winning_ribbon;
				ret += PP4->victory_ribbon;
				ret += PP4->bromide_ribbon;
				ret += PP4->ganba_ribbon;
				ret += PP4->marine_ribbon;
				ret += PP4->land_ribbon;
				ret += PP4->sky_ribbon;
				ret += PP4->country_ribbon;
				ret += PP4->national_ribbon;
				ret += PP4->earth_ribbon;
				ret += PP4->world_ribbon;
			}
			break;

		// �擾���{���ɉ������ޯĂ𗧂ĂĕԂ�  taya
		case ID_ribbon_bit:
			ret = 0;
			if((PP1->monsno)&&(PP4->tamago_flag==0)){

				ret = PP4->champ_ribbon;

				ret |= (PP4->stylemedal << 1);
				ret |= (PP4->beautifulmedal << 4);
				ret |= (PP4->cutemedal << 7);
				ret |= (PP4->clevermedal << 10);
				ret |= (PP4->strongmedal << 13);

				ret |= (PP4->winning_ribbon << 16);
				ret |= (PP4->victory_ribbon << 17);
				ret |= (PP4->bromide_ribbon << 18);
				ret |= (PP4->ganba_ribbon << 19);
				ret |= (PP4->marine_ribbon << 20);
				ret |= (PP4->land_ribbon << 21);
				ret |= (PP4->sky_ribbon << 22);
				ret |= (PP4->country_ribbon << 23);
				ret |= (PP4->national_ribbon << 24);
				ret |= (PP4->earth_ribbon << 25);
				ret |= (PP4->world_ribbon << 26);
			}
			break;

	}
	if(id>ID_amari)
		PokeParaCoded(PPP);
	return	ret;
}

//=========================================================================
//	�|�P�����p�����[�^�Z�b�g�i�Í����΍�j
//=========================================================================

void	PokeParaPut(PokemonParam *PP,int id,const u8 *buf)
{

	switch(id){
		case ID_condition:
			PP->condition=buf[0]+(buf[1]<<8)+(buf[2]<<16)+(buf[3]<<24);
			break;
		case ID_level:
			PP->level=buf[0];
			break;
		case ID_hp:
			PP->hp=buf[0]+(buf[1]<<8);
			break;
		case ID_hpmax:
			PP->hpmax=buf[0]+(buf[1]<<8);
			break;
		case ID_pow:
			PP->pow=buf[0]+(buf[1]<<8);
			break;
		case ID_def:
			PP->def=buf[0]+(buf[1]<<8);
			break;
		case ID_agi:
			PP->agi=buf[0]+(buf[1]<<8);
			break;
		case ID_spepow:
			PP->spepow=buf[0]+(buf[1]<<8);
			break;
		case ID_spedef:
			PP->spedef=buf[0]+(buf[1]<<8);
			break;
		case ID_mailid:
			PP->mailid=buf[0];	
			break;
		case ID_monsno_egg:
			break;
		default:
			PokePasoParaPut((PokemonPasoParam *)&PP->PPP,id,buf);
			break;
	}

}

void	PokePasoParaPut(PokemonPasoParam *PPP,int id,const u8 *buf)
{
	int	i;
	u32	work;
	PokemonPara1	*PP1=0;
	PokemonPara2	*PP2=0;
	PokemonPara3	*PP3=0;
	PokemonPara4	*PP4=0;
	u16				sum;

	if(id>ID_amari){
		PP1=(PokemonPara1 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA1);
		PP2=(PokemonPara2 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA2);
		PP3=(PokemonPara3 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA3);
		PP4=(PokemonPara4 *)PokeParaAdrsGet(PPP,PPP->personal_rnd,ID_POKEPARA4);

		//�`�F�b�N�T���͕��������Ȃ��̂Œ��ӁI�I
		PokeParaDecoded(PPP);
		sum=PokeParaCheckSum(PPP);
	
		//�s���ȃf�[�^�̎��͏������݂����Ȃ�
		if(sum!=PPP->checksum){
			PPP->fusei_tamago_flag=1;		
			PPP->egg_exist_flag=1;		
			PP4->tamago_flag=1;
			PokeParaCoded(PPP);
			return;
		}
	}

	switch(id){
		case ID_personal_rnd:
			PPP->personal_rnd=buf[0]+(buf[1]<<8)+(buf[2]<<16)+(buf[3]<<24);
			break;
		case ID_id_no:
			PPP->id_no=buf[0]+(buf[1]<<8)+(buf[2]<<16)+(buf[3]<<24);
			break;
		case ID_nickname:
			for(i=0;i<MONS_NAME_SIZE;i++)
				PPP->nickname[i]=buf[i];
			break;
		case ID_country_code:
			PPP->country_code=buf[0];		
			break;
		case ID_fusei_tamago_flag:
			PPP->fusei_tamago_flag=buf[0];		
			break;
		case ID_poke_exist_flag:
			PPP->poke_exist_flag=buf[0];
			break;
		case ID_egg_exist_flag:
			PPP->egg_exist_flag=buf[0];
			break;
		case ID_oyaname:
			for(i=0;i<PERSON_NAME_SIZE;i++)
				PPP->oyaname[i]=buf[i];
			break;
		case ID_mark:
			PPP->mark=buf[0];		
			break;
		case ID_checksum:
			PPP->checksum=buf[0]+(buf[1]<<8);
			break;
		case ID_amari:
			PPP->amari=buf[0]+(buf[1]<<8);
			break;

		case ID_monsno:
			PP1->monsno=buf[0]+(buf[1]<<8);
			if(PP1->monsno)
				PPP->poke_exist_flag=1;
			else
				PPP->poke_exist_flag=0;
			break;
		case ID_item:
			PP1->item=buf[0]+(buf[1]<<8);
			break;
		case ID_exp:
			PP1->exp=buf[0]+(buf[1]<<8)+(buf[2]<<16)+(buf[3]<<24);
			break;
		case ID_pp_count:
			PP1->pp_count=buf[0];
			break;
		case ID_friend:
			PP1->friend=buf[0];
			break;

		case ID_waza1:
		case ID_waza2:
		case ID_waza3:
		case ID_waza4:
			PP2->waza[id-ID_waza1]=buf[0]+(buf[1]<<8);
			break;
		case ID_pp1:
		case ID_pp2:
		case ID_pp3:
		case ID_pp4:
			PP2->pp[id-ID_pp1]=buf[0];
			break;

		case ID_hp_exp:
			PP3->hp_exp=buf[0];
			break;
		case ID_pow_exp:
			PP3->pow_exp=buf[0];
			break;
		case ID_def_exp:
			PP3->def_exp=buf[0];
			break;
		case ID_agi_exp:
			PP3->agi_exp=buf[0];
			break;
		case ID_spepow_exp:
			PP3->spepow_exp=buf[0];
			break;
		case ID_spedef_exp:
			PP3->spedef_exp=buf[0];
			break;
		case	ID_style:						//12h	�������悳
			PP3->style=buf[0];
			break;
		case	ID_beautiful:					//13h	��������
			PP3->beautiful=buf[0];
			break;
		case	ID_cute:						//14h	���킢��
			PP3->cute=buf[0];
			break;
		case	ID_clever:						//24h	��������
			PP3->clever=buf[0];
			break;
		case	ID_strong:						//2fh	�����܂���
			PP3->strong=buf[0];	
			break;
		case	ID_fur:						//30h	�щ�
			PP3->fur=buf[0];		
			break;

		case ID_pokerus:
			PP4->pokerus=buf[0];
			break;
		case ID_get_place:
			PP4->get_place=buf[0];
			break;
		case ID_get_level:
			PP4->get_level=buf[0];
			break;
		case ID_get_cassette:
			PP4->get_cassette=buf[0];
			break;
		case ID_get_ball:
			PP4->get_ball=buf[0];
			break;
		case ID_oyasex:
			PP4->oyasex=buf[0];
			break;
		case ID_hp_rnd:
			PP4->hp_rnd=buf[0];
			break;
		case ID_pow_rnd:
			PP4->pow_rnd=buf[0];
			break;
		case ID_def_rnd:
			PP4->def_rnd=buf[0];
			break;
		case ID_agi_rnd:
			PP4->agi_rnd=buf[0];
			break;
		case ID_spepow_rnd:
			PP4->spepow_rnd=buf[0];
			break;
		case ID_spedef_rnd:
			PP4->spedef_rnd=buf[0];
			break;
		case ID_tamago_flag:
			PP4->tamago_flag=buf[0];
			if(PP4->tamago_flag)
				PPP->egg_exist_flag=1;
			else
				PPP->egg_exist_flag=0;
			break;
		case ID_speabi:
			PP4->speabi=buf[0];
			break;
		case	ID_stylemedal:		//12h	�������悳�M��
			PP4->stylemedal=buf[0];		
			break;
		case	ID_beautifulmedal:		//13h	���������M��
			PP4->beautifulmedal=buf[0];		
			break;
		case	ID_cutemedal:		//14h	���킢���M��
			PP4->cutemedal=buf[0];
			break;
		case	ID_clevermedal:		//24h	���������M��
			PP4->clevermedal=buf[0];
			break;
		case	ID_strongmedal:		//2fh	�����܂����M��
			PP4->strongmedal=buf[0];
			break;
		case	ID_champ_ribbon:			//�`�����v���{��
			PP4->champ_ribbon = buf[0];
			break;
		case	ID_winning_ribbon:		//�E�B�j���O���{��
			PP4->winning_ribbon = buf[0];
			break;
		case	ID_victory_ribbon:		//�r�N�g���[���{��
			PP4->victory_ribbon = buf[0];
			break;
		case	ID_bromide_ribbon:		//�u���}�C�h���{��
			PP4->bromide_ribbon = buf[0];
			break;
		case	ID_ganba_ribbon:			//����΃��{��
			PP4->ganba_ribbon = buf[0];
			break;
		case	ID_marine_ribbon:		//�}�������{��
			PP4->marine_ribbon = buf[0];
			break;
		case	ID_land_ribbon:			//�����h���{��
			PP4->land_ribbon = buf[0];
			break;
		case	ID_sky_ribbon:			//�X�J�C���{��
			PP4->sky_ribbon = buf[0];
			break;
		case	ID_country_ribbon:		//�J���g���[���{��
			PP4->country_ribbon = buf[0];
			break;
		case	ID_national_ribbon:		//�i�V���i�����{��
			PP4->national_ribbon = buf[0];
			break;
		case	ID_earth_ribbon:			//�A�[�X���{��
			PP4->earth_ribbon = buf[0];
			break;
		case	ID_world_ribbon:			//���[���h���{��
			PP4->world_ribbon = buf[0];
			break;
		case	ID_amari_ribbon:			//���܂�
			PP4->amari_ribbon = buf[0];
			break;

		case ID_power_rnd:
			work=(u32)buf[0];
			PP4->hp_rnd		=(work>> 0)&0x0000001f;
			PP4->pow_rnd	=(work>> 5)&0x0000001f;
			PP4->def_rnd	=(work>>10)&0x0000001f;
			PP4->agi_rnd	=(work>>15)&0x0000001f;
			PP4->spepow_rnd	=(work>>20)&0x0000001f;
			PP4->spedef_rnd	=(work>>25)&0x0000001f;
			break;
	}

	if(id>ID_amari){
		//�`�F�b�N�T���͕��������Ȃ��̂Œ��ӁI�I
		PPP->checksum=PokeParaCheckSum(PPP);
		PokeParaCoded(PPP);
	}
}

//=========================================================================
//	�莝���ɖ쐶�|�P������������
//=========================================================================

void	AddPokemonAct(PokemonParam *destPP,PokemonParam *srcPP,int size)
{
	//���ۂ͈Í�������������
	memcpy(destPP,srcPP,size);
}

u8	AddPokemon(PokemonParam *PP)
{
	int i;
	
	PokeParaPut(PP,ID_oyaname,(u8 *)&MyData.my_name);
	PokeParaPut(PP,ID_oyasex,(u8 *)&MyData.my_sex);
	PokeParaPut(PP,ID_id_no,(u8 *)&MyData.ID);

	for(i = 0; i < 6; i++)
	{
		if(PokeParaGet(&PokeParaMine[i],ID_monsno,0) == 0)
			break;
	}
	if(i >= 6){
		return AddPasoPokemon(PP);
	}
	
	AddPokemonAct(&PokeParaMine[i],PP,sizeof(PokemonParam));
	PokeCountMine=i+1;
	return 0;
}

u8	AddPasoPokemon(PokemonParam *PP)
{
	int	i,j;

	i=BoxData.CurrentBoxNo;
	while(1){
		for(j=0;j<BOX_PM_KAZU;j++){
			if(PokePasoParaGet(&BoxData.PPPBox[i][j],ID_monsno,0)==0){
				PokePPMaxSet(PP);
				AddPokemonAct((PokemonParam *)&(BoxData.PPPBox[i][j]),PP,sizeof(PokemonPasoParam));
				return 1;
			}
		}
		i++;
		if(i==BOX_KAZU)
			i=0;
		if(i==BoxData.CurrentBoxNo)
			break;
	}
	return 2;
}

//=========================================================================
//	�莝�����𐔂���
//=========================================================================

u8		PokeCountMineGet()
{
	for(PokeCountMine=0;PokeCountMine<6;PokeCountMine++){
		if(PokeParaGet(&PokeParaMine[PokeCountMine],ID_monsno,0)==0) break;
	}
	return PokeCountMine;
}

u8		PokeCountEnemyGet()
{
	for(PokeCountEnemy=0;PokeCountEnemy<6;PokeCountEnemy++){
		if(PokeParaGet(&PokeParaEnemy[PokeCountEnemy],ID_monsno,0)==0) break;
	}
	return PokeCountEnemy;
}

//=======================================================================
//		2��2�ΐ킪�\���ǂ������ׂ�
//=======================================================================

u8		Poke2vs2Check(void)
{
	int	i,j=0;

	PokeCountMineGet();
	if(PokeCountMine==1) return PokeCountMine;

	for(i=0;i<PokeCountMine;i++){
		if((PokeParaGet(&PokeParaMine[i],ID_hp,0))&&
		   (PokeParaGet(&PokeParaMine[i],ID_monsno_egg,0))&&
		   (PokeParaGet(&PokeParaMine[i],ID_monsno_egg,0)!=MONSNO_TAMAGO))
		   j++;
	}

	return (j>1?POKE2VS2_OK:POKE2VS2_NOTWO);
}

//=======================================================================
//		�����ɂ��Ă������\�͂��Q�b�g����
//=======================================================================

u8		SpecialAbirityGet(u16 monsno,u8 abiflag)
{
	if(abiflag)
		SpeabiNo=PPD[monsno].speabi2;
	else
		SpeabiNo=PPD[monsno].speabi1;

	return SpeabiNo;
}

//=======================================================================
//		�����ɂ��Ă������\�͂��Q�b�g����
//=======================================================================

u8		SpecialAbirityNoGet(PokemonParam *PP)
{
	u8	speabino;
	u16	monsno;
	u8	abiflag;

	monsno=PokeParaGet(PP,ID_monsno,0);
	abiflag=PokeParaGet(PP,ID_speabi,0);

	speabino=SpecialAbirityGet(monsno,abiflag);

	return speabino;
}

//=======================================================================
//		������ԑΐ�p�̃g���[�i�[�f�[�^�Z�b�g
//=======================================================================

void	OrusubanTaisenSet(SECRET_BASE_DATA *SBD)
{
	int	i,j;

	PokeParaEnemyInit();
	memcpy(SBD_WORK,SBD,sizeof(SECRET_BASE_DATA));

	for(i=0;i<6;i++){
		if(SBD_WORK->battle.mons_no[i]){
			PokeParaSet(&PokeParaEnemy[i],
						SBD_WORK->battle.mons_no[i],
						SBD_WORK->battle.level[i],
						15,
						RND_SET,
						SBD_WORK->battle.p_rnd[i],
						ID_NO_RARE,
						0);
			PokeParaPut(&PokeParaEnemy[i],ID_item,(u8 *)&SBD_WORK->battle.item[i]);
			for(j=0;j<6;j++)
				PokeParaPut(&PokeParaEnemy[i],ID_hp_exp+j,(u8 *)&SBD_WORK->battle.exp[i]);
			for(j=0;j<4;j++){
				PokeParaPut(&PokeParaEnemy[i],ID_waza1+j,(u8 *)&SBD_WORK->battle.waza[i*4+j]);
				PokeParaPut(&PokeParaEnemy[i],ID_pp1+j,(u8 *)&WTD[SBD_WORK->battle.waza[i*4+j]].pp);
			}
		}
	}
	FightType=FIGHT_TYPE_TRAINER|FIGHT_TYPE_1vs1;
	FieldTrainerID=ORUSUBAN_TRAINER;
}

//===============================================================
//		������ԑΐ�̃g���[�i�[�O���t�B�b�NID��Ԃ�
//===============================================================

const u8 OrusubanTRID[2][5]={
	{TR_TANPAN,TR_MUSHI,TR_PRINCE,TR_CAMPB,TR_ELITEM},
	{TR_MINI,TR_SCHOOLW,TR_PRINCESS,TR_PICNICG,TR_ELITEW},
};
//����ς�@�������@�@�@�@�@�@�@�~�j�X�J�[�g
//�ނ��Ƃ�@���傤�˂�@�@�@�@�@���キ������i���j
//���ڂ�����܁@�@�@�@�@�@�@�@�@�����傤����
//�L�����v�{�[�C �@�@�@�@�@�@�@�@�s�N�j�b�N�K�[��
//�G���[�g�g���[�i�[�i�j�j�@�@�@�G���[�g�g���[�i�[�i���j

u8		OrusubanChrGet(void)
{
	u8	id;

	id=OrusubanTRID[SBD_WORK->sex][SBD_WORK->id[0]%5];

	return(TRCellTbl[id]);
}

u8		OrusubanTypeGet(void)
{
	u8	id;

	id=OrusubanTRID[SBD_WORK->sex][SBD_WORK->id[0]%5];

	return(TRTypeTbl[id]);
}

//===============================================================
//
//	�����X�^�[�{�[���𓊂���邩�̃`�F�b�N
//
// out:	0	������܂�
// 		1	������܂���
//===============================================================

u8	BallThrowCheck(void)
{
	int	i;

	for(i=0;i<6;i++){
		if((PokeParaGet(&PokeParaMine[i],ID_monsno,0))==0)
			return 0;
	}
	return BallThrowCheckSafari();
}

u8	BallThrowCheckSafari(void)
{
	int	i,j;

	for(i=0;i<BOX_KAZU;i++){
		for(j=0;j<BOX_PM_KAZU;j++){
			if(PokePasoParaGet(&BoxData.PPPBox[i][j],ID_monsno,0)==0){
				return 0;
			}
		}
	}
	return 1;
}

//===============================================================
//
//	�����X�^�[�l�[�����w��o�b�t�@�ɃR�s�[
//
//	in : buf	�R�s�[����o�b�t�@
//		 monsno	�|�P�����i���o�[
//===============================================================

void	PokeNameSet(u8 *buf,u16 monsno)
{
	int	i;

	for(i=0;i<11;i++){
		if(monsno>MONSNO_END)
			buf[i]=PokeNameData[0][i];
		else
			buf[i]=PokeNameData[monsno][i];
		if(buf[i]==EOM_) break;
	}
	buf[i]=EOM_;		//�ꉞ�A���肦�Ȃ�monsno�������Ƃ��̑΍�
}

//===============================================================
//
//	PPMax���Q�b�g
//
//	in : wazano	�Q�b�g����Z�i���o�[
//		 maxup	�A�C�e���ɂ��}�b�N�X�A�b�v�J�E���^�[
//		 pos	�J�E���^�[�ʒu�iu8��2�r�b�g���g�p���Ă���̂Łj
//===============================================================

u8	PPMaxGet(u16 wazano,u8 maxup,u8 pos)
{
	u8	ppcnt;

	ppcnt=WTD[wazano].pp+
	    ((WTD[wazano].pp*20*((maxup&pp_count_mask[pos])>>(pos*2)))/100);

	return	ppcnt;
}

//===============================================================
//
//	pp_count���N���A
//
//	in : PP		�|�P�����\���̂̃|�C���^
//		 pos	�J�E���^�[�ʒu�iu8��2�r�b�g���g�p���Ă���̂Łj
//===============================================================

void	PokePPCountClear(PokemonParam *PP,u8 pos)
{
	u8	ppcnt;

	ppcnt=PokeParaGet(PP,ID_pp_count,0);
	ppcnt=ppcnt&pp_count_off[pos];
	PokeParaPut(PP,ID_pp_count,(u8 *)&ppcnt);
}

void	PokePPCountClearServer(PokemonServerParam *PSP,u8 pos)
{
	PSP->pp_count=PSP->pp_count&pp_count_off[pos];
}

//===============================================================
//
//	�A�C�e�����g�p���ă|�P�����̃X�e�[�^�X�ُ���񕜂���
//
// in :	PP	�񕜂���|�P�����f�[�^�̃|�C���^
// 		itemno	�g�p����A�C�e��
// 		pos		�|�P�������X�g�ŉ��Ԗڂ̃|�P���������w��
// 		pos2	�Z���X�g�ŉ��Ԗڂ̋Z�����w���iPP�Ƃ�����j
// 		flg		HP�񕜂̂Ƃ��A�v�Z�݂̂�����̂��̃t���O�i0:�v�Z�Ɖ񕜁@1:�v�Z�̂݁j
//
// out:	0	�񕜐���
// 		1	�g�p���Ă����ʂ��Ȃ�
//===============================================================

static const u8 ID_Tbl[]={
	ID_hp_exp,
	ID_pow_exp,
	ID_def_exp,
	ID_agi_exp,
	ID_spedef_exp,
	ID_spepow_exp,
};

#define	FRIEND1		100
#define	FRIEND2		200

void	StatusRecoverFight(u8 clientno,u8 SelectNo)
{
	int	i;
	u16	*hp;
	u8	buff[20];

	PSP[clientno].monsno		=PokeParaGet(&PokeParaMine[SelectNo],ID_monsno,0);
	PSP[clientno].item			=PokeParaGet(&PokeParaMine[SelectNo],ID_item,0);
	for(i=0;i<4;i++){
		PSP[clientno].waza[i]	=PokeParaGet(&PokeParaMine[SelectNo],ID_waza1+i,0);
		PSP[clientno].pp[i]		=PokeParaGet(&PokeParaMine[SelectNo],ID_pp1+i,0);
	}
	PSP[clientno].pp_count		=PokeParaGet(&PokeParaMine[SelectNo],ID_pp_count,0);
	PSP[clientno].friend		=PokeParaGet(&PokeParaMine[SelectNo],ID_friend,0);
	PSP[clientno].exp			=PokeParaGet(&PokeParaMine[SelectNo],ID_exp,0);
	PSP[clientno].hp_rnd		=PokeParaGet(&PokeParaMine[SelectNo],ID_hp_rnd,0);
	PSP[clientno].pow_rnd		=PokeParaGet(&PokeParaMine[SelectNo],ID_pow_rnd,0);
	PSP[clientno].def_rnd		=PokeParaGet(&PokeParaMine[SelectNo],ID_def_rnd,0);
	PSP[clientno].agi_rnd		=PokeParaGet(&PokeParaMine[SelectNo],ID_agi_rnd,0);
	PSP[clientno].spepow_rnd	=PokeParaGet(&PokeParaMine[SelectNo],ID_spepow_rnd,0);
	PSP[clientno].spedef_rnd	=PokeParaGet(&PokeParaMine[SelectNo],ID_spedef_rnd,0);
	PSP[clientno].personal_rnd	=PokeParaGet(&PokeParaMine[SelectNo],ID_personal_rnd,0);
	PSP[clientno].condition		=PokeParaGet(&PokeParaMine[SelectNo],ID_condition,0);
	PSP[clientno].level			=PokeParaGet(&PokeParaMine[SelectNo],ID_level,0);
	PSP[clientno].hp			=PokeParaGet(&PokeParaMine[SelectNo],ID_hp,0);
	PSP[clientno].hpmax			=PokeParaGet(&PokeParaMine[SelectNo],ID_hpmax,0);
	PSP[clientno].pow			=PokeParaGet(&PokeParaMine[SelectNo],ID_pow,0);
	PSP[clientno].def			=PokeParaGet(&PokeParaMine[SelectNo],ID_def,0);
	PSP[clientno].agi			=PokeParaGet(&PokeParaMine[SelectNo],ID_agi,0);
	PSP[clientno].spepow		=PokeParaGet(&PokeParaMine[SelectNo],ID_spepow,0);
	PSP[clientno].spedef		=PokeParaGet(&PokeParaMine[SelectNo],ID_spedef,0);
	PSP[clientno].tamago_flag	=PokeParaGet(&PokeParaMine[SelectNo],ID_tamago_flag,0);
	PSP[clientno].speabi		=PokeParaGet(&PokeParaMine[SelectNo],ID_speabi,0);
	PSP[clientno].id_no			=PokeParaGet(&PokeParaMine[SelectNo],ID_id_no,0);
	PSP[clientno].type1			=PPD[PSP[clientno].monsno].type1;
	PSP[clientno].type2			=PPD[PSP[clientno].monsno].type2;
	PSP[clientno].speabino		=SpecialAbirityGet(PSP[clientno].monsno,PSP[clientno].speabi);
	PokeParaGet(&PokeParaMine[SelectNo],ID_nickname,buff);
	NickNameCopy(PSP[clientno].nickname, buff);
	PokeParaGet(&PokeParaMine[SelectNo],ID_oyaname,PSP[clientno].oyaname);
	hp=(u16 *)&UserWork[MONS_HP_TEMP+(MineEnemyCheck(clientno)*2)];
	hp[0]=PSP[clientno].hp;
	for(i=0;i<COND_MAX;i++)
		PSP[clientno].abiritycnt[i]=6;
	PSP[clientno].condition2=0;
	FightJoinFlagSet(clientno);
	ClientPokeSysFlagClear(clientno, 0);
}

u8	StatusRecover(PokemonParam *PP,u16 itemno,u8 pos,u8 pos2)
{
	return StatusRecover2(PP,itemno,pos,pos2,0);
}

u8	StatusRecover2(PokemonParam *PP,u16 itemno,u8 pos,u8 pos2,u8 hpflag)
{
	int	i,j;
	u8	ret=1;
	u8	flg;
	u8	*adr;
	u8	para=6;
	u32	buf;
	u32	buf2;
	u32	buf3;
	s8	friendpara=0;
	s32	friend;
	u16	totalexp;
	u16	aftermons;
	u16	soubiitem;
	u8	eqp;
	u8	PSPClientNo=4;

	soubiitem=PokeParaGet(PP,ID_item,0);
	if(soubiitem==ITEM_NAZONOMI){
		if(sys.FieldBattleFlag==CALL_BATTLE)
			eqp=FRS[TempClientNo].item_equip;
		else
			eqp=Fld.RamSeed.item_equip;
	}
	else
		eqp=ItemEquipGet(soubiitem);

	ItemClient=TempClientNo;

	if(sys.FieldBattleFlag==CALL_BATTLE){
		ClientNo=TempClientNo;
		if(MineEnemyCheck(ClientNo)==SIDE_MINE)
			i=0;
		else
			i=1;
		for(;i<ClientSetMax;i+=2){
			if(SelMonsNo[i]==pos){
				PSPClientNo=i;
				break;
			}
		}
	}
	else{
		ClientNo=0;
		PSPClientNo=4;
	}

	if((itemno<ITEM_NORMAL_START)||
	   (itemno>ITEM_SEED_END))
		return 1;

//	itemno-=ITEM_NORMAL_START;

	if((ItemPokeParmTbl[itemno-ITEM_NORMAL_START]==NULL)&&
	   (itemno!=ITEM_NAZONOMI))
		return 1;
	else{
		if(itemno==ITEM_NAZONOMI){
			if(sys.FieldBattleFlag==CALL_BATTLE)
				adr=(s8 *)&FRS[ClientNo].item_param;
			else
				adr=(s8 *)&Fld.RamSeed.item_param;
		}
		else
			adr=(s8 *)ItemPokeParmTbl[itemno-ITEM_NORMAL_START];
		for(i=0;i<6;i++){
			switch(i){
				case 0:		//���������񕜁��N���e�B�J�����U����
					//����������				
					if(adr[i]&0x80){
						if((sys.FieldBattleFlag==CALL_BATTLE)&&
						   (PSPClientNo!=4)&&
						   (PSP[PSPClientNo].condition2&CONDITION2_MEROMERO)){
							PSP[PSPClientNo].condition2&=CONDITION2_MEROMERO_OFF;
							ret=0;
						}
					}
					//�N���e�B�J��				
					if(adr[i]&0x30){
						if((PSP[ClientNo].condition2&CONDITION2_KIAIDAME)==0){
							PSP[ClientNo].condition2|=CONDITION2_KIAIDAME;
							ret=0;
						}
					}
					//�U����				
					if(adr[i]&0x0f){
						if(PSP[ClientNo].abiritycnt[COND_POW]<12){
							PSP[ClientNo].abiritycnt[COND_POW]+=(adr[i]&0x0f);
							if(PSP[ClientNo].abiritycnt[COND_POW]>12)
								PSP[ClientNo].abiritycnt[COND_POW]=12;
							ret=0;
						}
					}
//					if(adr[i]&0xff)
//						FriendCalc(PP,FRIEND_BATTLE_ITEM);
					break;
				case 1:		//�h��́��f����
					//�h���
					if(adr[i]&0xf0){
						if(PSP[ClientNo].abiritycnt[COND_DEF]<12){
							PSP[ClientNo].abiritycnt[COND_DEF]+=(adr[i]&0xf0)>>4;
							if(PSP[ClientNo].abiritycnt[COND_DEF]>12)
								PSP[ClientNo].abiritycnt[COND_DEF]=12;
							ret=0;
						}
					}
					//�f����
					if(adr[i]&0x0f){
						if(PSP[ClientNo].abiritycnt[COND_AGI]<12){
							PSP[ClientNo].abiritycnt[COND_AGI]+=(adr[i]&0x0f);
							if(PSP[ClientNo].abiritycnt[COND_AGI]>12)
								PSP[ClientNo].abiritycnt[COND_AGI]=12;
							ret=0;
						}
					}
//					if(adr[i]&0xff)
//						FriendCalc(PP,FRIEND_BATTLE_ITEM);
					break;
				case 2:		//������������U��
					//������
					if(adr[i]&0xf0){
						if(PSP[ClientNo].abiritycnt[COND_HIT]<12){
							PSP[ClientNo].abiritycnt[COND_HIT]+=(adr[i]&0xf0)>>4;
							if(PSP[ClientNo].abiritycnt[COND_HIT]>12)
								PSP[ClientNo].abiritycnt[COND_HIT]=12;
							ret=0;
						}
					}
					//����U��
					if(adr[i]&0x0f){
						if(PSP[ClientNo].abiritycnt[COND_SPEPOW]<12){
							PSP[ClientNo].abiritycnt[COND_SPEPOW]+=(adr[i]&0x0f);
							if(PSP[ClientNo].abiritycnt[COND_SPEPOW]>12)
								PSP[ClientNo].abiritycnt[COND_SPEPOW]=12;
							ret=0;
						}
					}
//					if(adr[i]&0xff)
//						FriendCalc(PP,FRIEND_BATTLE_ITEM);
					break;
				case 3:		//�\�Ͷް��/LvUp/����/��/�Ώ�/�X/�}�q/����
					//�\�Ͷް�
					if(adr[i]&0x80){
						if(SCC[MineEnemyCheck(ClientNo)].shiroikiricnt==0){
							SCC[MineEnemyCheck(ClientNo)].shiroikiricnt=5;
							ret=0;
						}
					}
					//LvUp
					if(adr[i]&0x40){
						if(PokeParaGet(PP,ID_level,0)!=100){
							buf=GrowTbl[PPD[PokeParaGet(PP,ID_monsno,0)].grow][PokeParaGet(PP,ID_level,0)+1];
							PokeParaPut(PP,ID_exp,(u8 *)&buf);
							PokeParaCalc(PP);
							ret=0;
						}
					}
					//����
					if(adr[i]&0x20){
						if(StatusRecoverAct(PP,pos,CONDITION_NEMURI,PSPClientNo)==0){
							if(PSPClientNo!=4)
								PSP[PSPClientNo].condition2&=CONDITION2_AKUMU_OFF;
							ret=0;
						}
					}
					//��
					if(adr[i]&0x10){
						if(StatusRecoverAct(PP,pos,CONDITION_DOKU|
							CONDITION_DOKUDOKU|CONDITION_DOKUDOKU_CNT,PSPClientNo)==0)
							ret=0;
					}
					//�Ώ�
					if(adr[i]&0x08){
						if(StatusRecoverAct(PP,pos,CONDITION_YAKEDO,PSPClientNo)==0)
							ret=0;
					}
					//�X
					if(adr[i]&0x04){
						if(StatusRecoverAct(PP,pos,CONDITION_KOORI,PSPClientNo)==0)
							ret=0;
					}
					//�}�q
					if(adr[i]&0x02){
						if(StatusRecoverAct(PP,pos,CONDITION_MAHI,PSPClientNo)==0)
							ret=0;
					}
					//����
					if(adr[i]&0x01){
						if((sys.FieldBattleFlag==CALL_BATTLE)&&
						   (PSPClientNo!=4)&&
						   (PSP[PSPClientNo].condition2&CONDITION2_KONRAN)){
							PSP[PSPClientNo].condition2&=CONDITION2_KONRAN_OFF;
							ret=0;
						}
					}
					break;
				case 4:	//�i���t���O/�m����flg/ppUP/PP��flg/pp��/HP��/�U��/HP
					flg=adr[i];
					//ppUP
					if(flg&0x20){
						flg&=(0x20^0xff);
						buf=(PokeParaGet(PP,ID_pp_count,0)&pp_count_mask[pos2])
							>>(pos2*2);
						buf2=PPMaxGet(PokeParaGet(PP,ID_waza1+pos2,0),
									  PokeParaGet(PP,ID_pp_count,0),
									  pos2);
						if((buf<3)&&(buf2>4)){
							buf=PokeParaGet(PP,ID_pp_count,0)
								+pp_count_value[pos2];
							PokeParaPut(PP,ID_pp_count,(u8 *)&buf);
							buf=PPMaxGet(PokeParaGet(PP,ID_waza1+pos2,0),buf,pos2)-buf2;
							buf=PokeParaGet(PP,ID_pp1+pos2,0)+buf;
							PokeParaPut(PP,ID_pp1+pos2,(u8 *)&buf);
							ret=0;
						}
					}
					buf2=0;
					while(flg){
						if(flg&1){
							switch(buf2){
								case 0:	//HP
								case 1:	//�U��
									totalexp=ParaExpTotalGet(PP);
									if(totalexp>=PARA_EXP_TOTAL_MAX)
										return 1;
									buf=PokeParaGet(PP,ID_Tbl[buf2],0);
									if(buf>=PARA_EXP_MAX)
										break;
									if((buf+adr[para])>PARA_EXP_MAX)
										buf3=adr[para]-((buf+adr[para])-PARA_EXP_MAX);
									else
										buf3=adr[para];
									if(totalexp+buf3>PARA_EXP_TOTAL_MAX)
										buf3-=((totalexp+buf3)-PARA_EXP_TOTAL_MAX);
									buf+=buf3;
									PokeParaPut(PP,ID_Tbl[buf2],(u8 *)&buf);
									PokeParaCalc(PP);
									para++;
									ret=0;
									break;
								case 2:	//HP��
									if(flg&0x10){	//�m����flg�`�F�b�N
//										flg&=(0x10^0xff);
										if((PokeParaGet(PP,ID_hp,0))!=0){
											para++;
											break;
										}
										if((sys.FieldBattleFlag==CALL_BATTLE)&&
										   (PSPClientNo!=4)){
											NoReshuffleClient&=(no2bittbl[PSPClientNo]^0xffffffff);
											StatusRecoverFight(PSPClientNo,
															   SearchListRow(SelMonsNo[PSPClientNo]));
											//TV���p�̃J�E���^���C���N�������g
											if(MineEnemyCheck(ClientNo)==SIDE_MINE){
												if(FTW.ItemUseFukkatsu<0xff)
													FTW.ItemUseFukkatsu++;
											}
										}
										else if(sys.FieldBattleFlag==CALL_BATTLE){
											NoReshuffleClient&=(no2bittbl[ClientNo^2]^0xffffffff);
											//TV���p�̃J�E���^���C���N�������g
											if(MineEnemyCheck(ClientNo)==SIDE_MINE){
												if(FTW.ItemUseFukkatsu<0xff)
													FTW.ItemUseFukkatsu++;
											}
										}
									}
									else{
										if((PokeParaGet(PP,ID_hp,0))==0){
											para++;
											break;
										}
									}
									buf=adr[para++];
									switch(buf){
										case 255:	//�S��
											buf=PokeParaGet(PP,ID_hpmax,0)-
												PokeParaGet(PP,ID_hp,0);
											break;
										case 254:	//����
											buf=PokeParaGet(PP,ID_hpmax,0)/2;
											if(buf==0)
												buf=1;
											break;
										case 253:	//HPMAX����������
											buf=UserWork[HPMAX_UP];
											break;
									}
									if(PokeParaGet(PP,ID_hpmax,0)!=PokeParaGet(PP,ID_hp,0)){
										if(hpflag==0){
											buf=PokeParaGet(PP,ID_hp,0)+buf;
											if(buf>PokeParaGet(PP,ID_hpmax,0))
												buf=PokeParaGet(PP,ID_hpmax,0);
											PokeParaPut(PP,ID_hp,(u8 *)&buf);
											if((sys.FieldBattleFlag==CALL_BATTLE)&&
											   (PSPClientNo!=4)){
												PSP[PSPClientNo].hp=buf;
												if(((flg&0x10)==0)&&	//�m����flg�`�F�b�N
												    (MineEnemyCheck(ClientNo)==SIDE_MINE)){
													//TV���p�̃J�E���^���C���N�������g
													if(FTW.ItemUseKaifukuCount<0xff)
														FTW.ItemUseKaifukuCount++;
													j=ClientNo;
													ClientNo=PSPClientNo;
													BSS_POKEPARA_GET_SET(BSS_SERVER,BSS_PARA_FIGHT,BSS_SELMONSNO);
													ClientBitSet(ClientNo);
													ClientNo=j;
												}
											}
										}
										else
											WazaDamage=buf*-1;
										ret=0;
									}
									flg&=(0x10^0xff);
									break;
								case 3:	//pp�� PP��flg
									if((flg&2)==0){
										for(j=0;j<4;j++){
											buf=PokeParaGet(PP,ID_pp1+j,0);
											if(buf!=PPMaxGet(PokeParaGet(PP,ID_waza1+j,0),PokeParaGet(PP,ID_pp_count,0),j)){
												buf+=adr[para];
												if(buf>PPMaxGet(PokeParaGet(PP,ID_waza1+j,0),PokeParaGet(PP,ID_pp_count,0),j))
													buf=PPMaxGet(PokeParaGet(PP,ID_waza1+j,0),PokeParaGet(PP,ID_pp_count,0),j);
												PokeParaPut(PP,ID_pp1+j,(u8 *)&buf);
												if((sys.FieldBattleFlag==CALL_BATTLE)&&
												   (PSPClientNo!=4)){
													if(((PSP[PSPClientNo].condition2&CONDITION2_HENSHIN)==0)&&
													   ((WKC[PSPClientNo].monomanebit&no2bittbl[j])==0))
														PSP[PSPClientNo].pp[j]=buf;
												}
												ret=0;
											}
										}
										para++;
									}
									else{
										buf=PokeParaGet(PP,ID_pp1+pos2,0);
										if(buf!=PPMaxGet(PokeParaGet(PP,ID_waza1+pos2,0),PokeParaGet(PP,ID_pp_count,0),pos2)){
											buf+=adr[para++];
											if(buf>PPMaxGet(PokeParaGet(PP,ID_waza1+pos2,0),PokeParaGet(PP,ID_pp_count,0),pos2))
												buf=PPMaxGet(PokeParaGet(PP,ID_waza1+pos2,0),PokeParaGet(PP,ID_pp_count,0),pos2);
											PokeParaPut(PP,ID_pp1+pos2,(u8 *)&buf);
												if((sys.FieldBattleFlag==CALL_BATTLE)&&
												   (PSPClientNo!=4)){
													if(((PSP[PSPClientNo].condition2&CONDITION2_HENSHIN)==0)&&
													   ((WKC[PSPClientNo].monomanebit&no2bittbl[pos2])==0))
														PSP[PSPClientNo].pp[pos2]=buf;
												}
											ret=0;
										}
									}
									break;
								case 7:		//�i��
									if((aftermons=PokeShinkaCheck(PP,ITEM_SHINKA,itemno))){
										InitShinka(PP,aftermons,SHINKA_CANCEL_OFF,pos);
										return 0;
									}
									break;
							}
						}
						buf2++;
						flg=flg>>1;
					}
					break;
				case 5:		//�Ȃ��x3/�Ȃ��x2/�Ȃ��x1/pp�l3up/���U/���h/�f����/�h��
					flg=adr[i];
					buf2=0;
					while(flg){
						if(flg&1){
							switch(buf2){
								case 0:		//�h��
								case 1:		//�f����
								case 2:		//���h
								case 3:		//���U
									totalexp=ParaExpTotalGet(PP);
									if(totalexp>=PARA_EXP_TOTAL_MAX)
										return 1;
									buf=PokeParaGet(PP,ID_Tbl[buf2+2],0);
									if(buf>=PARA_EXP_MAX)
										break;
									if((buf+adr[para])>PARA_EXP_MAX)
										buf3=adr[para]-((buf+adr[para])-PARA_EXP_MAX);
									else
										buf3=adr[para];
									if(totalexp+buf3>PARA_EXP_TOTAL_MAX)
										buf3-=((totalexp+buf3)-PARA_EXP_TOTAL_MAX);
									buf+=buf3;
									PokeParaPut(PP,ID_Tbl[buf2+2],(u8 *)&buf);
									PokeParaCalc(PP);
									ret=0;
									para++;
									break;
								case 4:		//pp�l3up
									buf=(PokeParaGet(PP,ID_pp_count,0)&
										pp_count_mask[pos2])>>(pos2*2);
									if(buf<3){
										buf3=PPMaxGet(
											PokeParaGet(PP,ID_waza1+pos2,0),
											PokeParaGet(PP,ID_pp_count,0),
											pos2);
										buf=PokeParaGet(PP,ID_pp_count,0);
										buf&=pp_count_off[pos2];
										buf+=pp_count_value[pos2]*3;
										PokeParaPut(PP,ID_pp_count,(u8 *)&buf);
										buf=PPMaxGet(
											PokeParaGet(PP,ID_waza1+pos2,0),
											buf,
											pos2)-buf3;
										buf=PokeParaGet(PP,ID_pp1+pos2,0)+buf;
										PokeParaPut(PP,ID_pp1+pos2,(u8 *)&buf);
										ret=0;
									}
									break;
								case 5:		//�Ȃ��x1
									if((PokeParaGet(PP,ID_friend,0)<FRIEND1)&&
									   (ret==0)&&
									   (friendpara==0)){
										friendpara=(s8)adr[para];
										friend=PokeParaGet(PP,ID_friend,0);
										if((friendpara>0)&&(eqp==SOUBI_NATUKIDOUP))
											friend+=(friendpara*150/100);
										else
											friend+=friendpara;
										
										if((friendpara>0)&&
										   (PokeParaGet(PP,ID_get_ball,0)==ITEM_GOOZYASUBOORU))
											friend++;
										if((friendpara>0)&&
										   (PokeParaGet(PP,ID_get_place,0)==GetNowWorldPos()))
											friend++;
										if(friend<0)
											friend=0;
										if(friend>255)
											friend=255;

										PokeParaPut(PP,ID_friend,(u8 *)&friend);
									}
									para++;
#if 0
									if((PokeParaGet(PP,ID_friend,0)<FRIEND1)&&
									   (ret==0)){
										friendpara=(s8)adr[para];
										friend=PokeParaGet(PP,ID_friend,0);
										if((friendpara>0)&&(eqp==SOUBI_NATUKIDOUP))
											friend+=(friendpara*150/100);
										else
											friend+=friendpara;
										
										if((friendpara>0)&&
										   (PokeParaGet(PP,ID_get_ball,0)==ITEM_GOOZYASUBOORU))
											friend++;
										if(friend<0)
											friend=0;
										if(friend>255)
											friend=255;

										PokeParaPut(PP,ID_friend,(u8 *)&friend);
									}
									para++;
#endif
									break;
								case 6:		//�Ȃ��x2
									if((PokeParaGet(PP,ID_friend,0)>=FRIEND1)&&
									   (PokeParaGet(PP,ID_friend,0)<FRIEND2)&&
									   (ret==0)&&
									   (friendpara==0)){
										friendpara=(s8)adr[para];
										friend=PokeParaGet(PP,ID_friend,0);
										if((friendpara>0)&&(eqp==SOUBI_NATUKIDOUP))
											friend+=(friendpara*150/100);
										else
											friend+=friendpara;

										if((friendpara>0)&&
										   (PokeParaGet(PP,ID_get_ball,0)==ITEM_GOOZYASUBOORU))
											friend++;
										if((friendpara>0)&&
										   (PokeParaGet(PP,ID_get_place,0)==GetNowWorldPos()))
											friend++;
										if(friend<0)
											friend=0;
										if(friend>255)
											friend=255;

										PokeParaPut(PP,ID_friend,(u8 *)&friend);
									}
									para++;
#if 0
									if((PokeParaGet(PP,ID_friend,0)>=FRIEND1)&&
									   (PokeParaGet(PP,ID_friend,0)<FRIEND2)&&
									   (ret==0)){
										friendpara=(s8)adr[para];
										friend=PokeParaGet(PP,ID_friend,0);
										if((friendpara>0)&&(eqp==SOUBI_NATUKIDOUP))
											friend+=(friendpara*150/100);
										else
											friend+=friendpara;

										if(PokeParaGet(PP,ID_get_ball,0)==ITEM_GOOZYASUBOORU)
											friend++;
										if(friend<0)
											friend=0;
										if(friend>255)
											friend=255;

										PokeParaPut(PP,ID_friend,(u8 *)&friend);
									}
									para++;
#endif
									break;
								case 7:		//�Ȃ��x3
									if((PokeParaGet(PP,ID_friend,0)>=FRIEND2)&&
									   (ret==0)&&
									   (friendpara==0)){
										friendpara=(s8)adr[para];
										friend=PokeParaGet(PP,ID_friend,0);
										if((friendpara>0)&&(eqp==SOUBI_NATUKIDOUP))
											friend+=(friendpara*150/100);
										else
											friend+=friendpara;

										if((friendpara>0)&&
										   (PokeParaGet(PP,ID_get_ball,0)==ITEM_GOOZYASUBOORU))
											friend++;
										if((friendpara>0)&&
										   (PokeParaGet(PP,ID_get_place,0)==GetNowWorldPos()))
											friend++;
										if(friend<0)
											friend=0;
										if(friend>255)
											friend=255;

										PokeParaPut(PP,ID_friend,(u8 *)&friend);
									}
									para++;
#if 0
									if((PokeParaGet(PP,ID_friend,0)>FRIEND2)&&
									   (ret==0)){
										friendpara=(s8)adr[para];
										friend=PokeParaGet(PP,ID_friend,0);
										if(eqp==SOUBI_NATUKIDOUP)
											friend+=(friendpara*150/100);
										else
											friend+=friendpara;

										if(PokeParaGet(PP,ID_get_ball,0)==ITEM_GOOZYASUBOORU)
											friend++;
										if(friend<0)
											friend=0;
										if(friend>255)
											friend=255;

										PokeParaPut(PP,ID_friend,(u8 *)&friend);
									}
									para++;
#endif
									break;
							}
						}
						buf2++;
						flg=flg>>1;
					}
					break;
			}
		}
	}
	return ret;
}

u8	StatusRecoverAct(PokemonParam *PP,u8 pos,u32 flagon,u8 pspclientno)
{
	u32	buf;
	buf=PokeParaGet(PP,ID_condition,0);
	if(buf&flagon){
		buf&=(flagon^0xffffffff);
		PokeParaPut(PP,ID_condition,(u8 *)&buf);
		if((sys.FieldBattleFlag==CALL_BATTLE)&&
		   (pspclientno!=4))
			PSP[pspclientno].condition&=(flagon^0xffffffff);
		return 0;
	}
	return 1;
}

u8	ItemParamPosGet(u16 itemno,u8 pos,u8 checkbit)
{
	int	i;
	u8	para=6;
	u8	*adr;
	u8	buf;
	u8	flg;

	if((ItemPokeParmTbl[itemno-ITEM_NORMAL_START]==NULL)&&
	   (itemno!=ITEM_NAZONOMI))
		return 0;
	else{
		if(itemno==ITEM_NAZONOMI)
//			adr=(u8 *)&Fld.RamSeed.item_param;
			adr=(u8 *)&FRS[ClientNo].item_param;
		else
			adr=(u8 *)ItemPokeParmTbl[itemno-ITEM_NORMAL_START];
		for(i=0;i<6;i++){
			switch(i){
				case 0:		//���������񕜁��N���e�B�J�����U����
				case 1:		//�h��́��f����
				case 2:		//������������U��
				case 3:		//�\�Ͷް��/LvUp/����/��/�Ώ�/�X/�}�q/����
					if(i==pos)
						return 0;
					break;
				case 4:	//�i���t���O/�m����flg/ppUP/PP��flg/pp��/HP��/�U��/HP
					flg=adr[i];
					//ppUP
					if(flg&0x20)
						flg&=(0x20^0xff);
					buf=0;
					while(flg){
						if(flg&1){
							switch(buf){
								case 0:	//HP
								case 1:	//�U��
									if((i==pos)&&(flg&checkbit))
										return para;
									para++;
									break;
								case 2:	//HP��
									if(flg&0x10)	//�m����flg�`�F�b�N
										flg&=(0x10^0xff);
									if((i==pos)&&(flg&checkbit))
										return para;
									para++;
									break;
								case 3:	//pp�� PP��flg
									if((i==pos)&&(flg&checkbit))
										return para;
									para++;
									break;
								case 7:		//�i��
									if(i==pos)
										return 0;
									break;
							}
						}
						buf++;
						flg=flg>>1;
						if(i==pos)
							checkbit=checkbit>>1;
					}
					break;
				case 5:		//pp�l3up/�Ȃ��x3/�Ȃ��x2/�Ȃ��x1/���U/���h/�f����/�h��
					flg=adr[i];
					buf=0;
					while(flg){
						if(flg&1){
							switch(buf){
								case 0:		//�h��
								case 1:		//�f����
								case 2:		//���h
								case 3:		//���U
								case 4:		//�Ȃ��x1
								case 5:		//�Ȃ��x2
								case 6:		//�Ȃ��x3
									if((i==pos)&&(flg&checkbit))
										return para;
									para++;
									break;
								case 7:		//pp�l3up
									if(i==pos)
										return 0;
									break;
							}
						}
						buf++;
						flg=flg>>1;
						if(i==pos)
							checkbit=checkbit>>1;
					}
					break;
			}
		}
	}
	return para;
}

//===============================================================
//
//	�퓬�Ŏg����A�C�e�����`�F�b�N���ă��b�Z�[�W�̃|�C���^��Ԃ�
//
//	in : itemno	�g���A�C�e���i���o�[
//===============================================================

const u8 FIUC_TBL[]={
	COND_POW,		//�U����
	COND_POW,		//�_�~�[
	COND_AGI,		//�f����
	COND_DEF,		//�h���
	COND_SPEPOW,	//����U��
	COND_HIT,		//������
};

extern	const	u8	ShiroikiriMsg[];
extern	const	u8	KiaidameMsg[];
extern	const	u8	* const AbirityMsgTbl[];
extern	const	u8	AgattaMsg[];
extern	const	u8	CondUpMsgD[];
extern	u8	EvStrBuf0[16];
extern	u8	EvStrBuf1[16];

void	CondMsgCreate(int no)
{
	DefenceClient=TempClientNo;
	PM_strcpy(EvStrBuf0,AbirityMsgTbl[FIUC_TBL[no]]);
	PM_strcpy(EvStrBuf1,AgattaMsg);
	FightMsgCreate((u8 *)CondUpMsgD);
}

u8	*FightItemUseMsgGet(u16 itemno)
{
	int	i;
	u8	*adrs;
	u8	*param;

	if(itemno==ITEM_NAZONOMI){
		if(sys.FieldBattleFlag==CALL_BATTLE)
			param=(u8 *)&FRS[TempClientNo].item_param;
		else
			param=(u8 *)&Fld.RamSeed.item_param;
	}
	else
		param=(u8 *)ItemPokeParmTbl[itemno-ITEM_NORMAL_START];

	ItemClient=TempClientNo;

	for(i=0;i<3;i++){
		if(param[i]&0x0f)
			CondMsgCreate(i*2);
		if(param[i]&0xf0){
			if(i)
				CondMsgCreate(i*2+1);
			else{
				AttackClient=TempClientNo;
				FightMsgCreate((u8 *)&KiaidameMsg);
			}
		}
	}
	if(param[3]&0x80){
		AttackClient=TempClientNo;
		FightMsgCreate((u8 *)&ShiroikiriMsg);
	}

	adrs=FightMsgBuf;

	return adrs;
}

#if 0
	#if 1
const u8 FIUC_TBL[]={
	COND_POW,		//�U����
	COND_POW,		//�_�~�[
	COND_AGI,		//�f����
	COND_DEF,		//�h���
	COND_SPEPOW,	//����U��
	COND_HIT,		//������
};

extern	const	u8	ShiroikiriMsg[];
extern	const	u8	KiaidameMsg[];
extern	const	u8	* const AbirityMsgTbl[];
extern	const	u8	AgattaMsg[];
extern	const	u8	CondUpMsgD[];
//extern	u8	FightMsgBuf[64];
extern	u8	EvStrBuf0[16];
extern	u8	EvStrBuf1[16];

u8	CondMsgCreate(int no)
{
	u8	ret=1;

	if(PSP[TempClientNo].abiritycnt[FIUC_TBL[no]]<12){
		PSP[TempClientNo].abiritycnt[FIUC_TBL[no]]++;
		DefenceClient=TempClientNo;
		PM_strcpy(EvStrBuf0,AbirityMsgTbl[FIUC_TBL[no]]);
		PM_strcpy(EvStrBuf1,AgattaMsg);
		FightMsgCreate((u8 *)CondUpMsgD);
		ret=0;
	}
	return ret;
}

u8	*FightItemUseCheck(u16 itemno)
{
	int	i;
	u8	ret=1;
	u8	*adrs;
	u8	*param;

	if(itemno==ITEM_NAZONOMI){
		if(sys.FieldBattleFlag==CALL_BATTLE)
			param=(u8 *)&FRS[TempClientNo].item_param;
		else
			param=(u8 *)&Fld.RamSeed.item_param;
	}
	else
		param=(u8 *)ItemPokeParmTbl[itemno-ITEM_NORMAL_START];

	ItemClient=TempClientNo;

	for(i=0;i<3;i++){
		if(param[i]&0x0f)
			ret=CondMsgCreate(i*2);
		if(param[i]&0xf0){
			if(i)
				ret=CondMsgCreate(i*2+1);
			else{
				if((PSP[TempClientNo].condition2&CONDITION2_KIAIDAME)==0){
					PSP[TempClientNo].condition2|=CONDITION2_KIAIDAME;
					AttackClient=TempClientNo;
					FightMsgCreate((u8 *)&KiaidameMsg);
					ret=0;
				}
			}
		}
	}
	if(param[3]&0x80){
//		case ITEM_EFEKUTOGAADO:
		if(SCC[MineEnemyCheck(TempClientNo)].shiroikiricnt==0){
			SCC[MineEnemyCheck(TempClientNo)].shiroikiricnt=5;
			AttackClient=TempClientNo;
			FightMsgCreate((u8 *)&ShiroikiriMsg);
			ret=0;
		}
	}
	if(ret)
		adrs=NULL;
	else
		adrs=FightMsgBuf;

	return adrs;
}
	#else
const u8 FIUC_TBL[][2]={
	{ITEM_PURASUPAWAA,COND_POW},
	{ITEM_DHIFENDAA,COND_DEF},
	{ITEM_SUPIIDAA,COND_AGI},
	{ITEM_SUPESYARUAPPU,COND_SPEPOW},
	{ITEM_YOKUATAARU,COND_HIT},
};

extern	const	u8	ShiroikiriMsg[];
extern	const	u8	KiaidameMsg[];
extern	const	u8	* const AbirityMsgTbl[];
extern	const	u8	AgattaMsg[];
extern	const	u8	CondUpMsgD[];
//extern	u8	FightMsgBuf[64];
extern	u8	EvStrBuf0[16];
extern	u8	EvStrBuf1[16];

u8	*FightItemUseCheck(u16 itemno)
{
	u8	ret=1;
	int	i;

	u8	*adrs;

	switch(itemno){
		case ITEM_EFEKUTOGAADO:
			if(SCC[MineEnemyCheck(TempClientNo)].shiroikiricnt==0){
				SCC[MineEnemyCheck(TempClientNo)].shiroikiricnt=5;
				AttackClient=TempClientNo;
				FightMsgCreate((u8 *)&ShiroikiriMsg);
				ret=0;
			}
			break;
		case ITEM_KURITHIKATTAA:
			if((PSP[TempClientNo].condition2&CONDITION2_KIAIDAME)==0){
				PSP[TempClientNo].condition2|=CONDITION2_KIAIDAME;
				AttackClient=TempClientNo;
				FightMsgCreate((u8 *)&KiaidameMsg);
				ret=0;
			}
			break;
		case ITEM_PURASUPAWAA:
		case ITEM_DHIFENDAA:
		case ITEM_SUPIIDAA:
		case ITEM_SUPESYARUAPPU:
		case ITEM_YOKUATAARU:
			for(i=0;i<5;i++){
				if(FIUC_TBL[i][0]==itemno) break;
			}
			if(PSP[TempClientNo].abiritycnt[FIUC_TBL[i][1]]<12){
				PSP[TempClientNo].abiritycnt[FIUC_TBL[i][1]]++;
				DefenceClient=TempClientNo;
				PM_strcpy(EvStrBuf0,AbirityMsgTbl[FIUC_TBL[i][1]]);
				PM_strcpy(EvStrBuf1,AgattaMsg);
				FightMsgCreate((u8 *)CondUpMsgD);
				ret=0;
			}
			break;
	}
	if(ret)
		adrs=NULL;
	else
		adrs=FightMsgBuf;

	return adrs;
}
	#endif
#endif

//=======================================================
//		�|�P�����̐��i���Q�b�g
//=======================================================

u8	PokeSeikakuGet(PokemonParam *PP)
{
	u32	chr;

	chr=PokeParaGet(PP,ID_personal_rnd,0);			//������

	return((u8)(chr%25));
}

u8	PokeSeikakuGetRnd(u32 rnd)
{
	return(rnd%25);
}

//===============================================================
//
//	�i���`�F�b�N
//
//	in : PP		�`�F�b�N����|�P�����̍\���̂̃|�C���^
//		 type	�i�������ipoketool.def�ɒ�`�j
//		 itemno	�A�C�e���i���`�F�b�N���̎g�p�A�C�e��
//
//	out : 0 �i������
//		  0�ȊO	�i�������|�P�����i���o�[
//
//===============================================================

u16	PokeShinkaCheck(PokemonParam *PP,u8 type,u16 itemno)
{
	u16	monsno;
	u16	item;
	u8	level;
	int	i;
	u16	ret=0;
	u16	friend;
	u32	personal_rnd;
	u8	eqp;
	u8	beautiful;
	u16	rnd=0;

	monsno=PokeParaGet(PP,ID_monsno,0);
	item=PokeParaGet(PP,ID_item,0);
	personal_rnd=PokeParaGet(PP,ID_personal_rnd,0);
	beautiful=PokeParaGet(PP,ID_beautiful,0);
	rnd=(personal_rnd&0xffff0000)>>16;

	if(item==ITEM_NAZONOMI)
		eqp=Fld.RamSeed.item_equip;
	else
		eqp=ItemEquipGet(item);

	//�A�C�e���Ői�����Ȃ��������ʂ͐i�����Ȃ�
	if((eqp==SOUBI_SINKASINAI)&&
	   (type!=ITEM_SHINKA_CHECK))
		return 0;

	switch(type){
		case LEVELUP_SHINKA:
			level=PokeParaGet(PP,ID_level,0);
			friend=PokeParaGet(PP,ID_friend,0);
			for(i=0;i<5;i++){
				switch(PSD[monsno][i].ShinkaCond){
					case SHINKA_FRIEND_HIGH:
						if(SHINKA_FRIEND<=friend)
							ret=PSD[monsno][i].ShinkaMons;
						break;
					case SHINKA_FRIEND_HIGH_NOON:
						GetGameTime();
						if((GameTime.hour>=12)&&
						   (GameTime.hour<=23)&&
						   (SHINKA_FRIEND<=friend))
							ret=PSD[monsno][i].ShinkaMons;
						break;
					case SHINKA_FRIEND_HIGH_NIGHT:
						GetGameTime();
						if((GameTime.hour>=0)&&
						   (GameTime.hour<12)&&
						   (SHINKA_FRIEND<=friend))
								ret=PSD[monsno][i].ShinkaMons;
						break;
					case SHINKA_LEVELUP:
						if(PSD[monsno][i].ShinkaData<=level)
							ret=PSD[monsno][i].ShinkaMons;
						break;
					case SHINKA_SPECIAL_POW:
						if(PSD[monsno][i].ShinkaData<=level){
							if((PokeParaGet(PP,ID_pow,0))>(PokeParaGet(PP,ID_def,0)))
								ret=PSD[monsno][i].ShinkaMons;
						}
						break;
					case SHINKA_SPECIAL_EVEN:
						if(PSD[monsno][i].ShinkaData<=level){
							if((PokeParaGet(PP,ID_pow,0))==(PokeParaGet(PP,ID_def,0)))
								ret=PSD[monsno][i].ShinkaMons;
						}
						break;
					case SHINKA_SPECIAL_DEF:
						if(PSD[monsno][i].ShinkaData<=level){
							if((PokeParaGet(PP,ID_pow,0))<(PokeParaGet(PP,ID_def,0)))
								ret=PSD[monsno][i].ShinkaMons;
						}
						break;
					case SHINKA_SPECIAL_RND_EVEN:		//���������2�o�C�g������
						if(PSD[monsno][i].ShinkaData<=level){
							if((rnd%10)<5)
								ret=PSD[monsno][i].ShinkaMons;
						}
						break;
					case SHINKA_SPECIAL_RND_ODD:		//���������2�o�C�g���
						if(PSD[monsno][i].ShinkaData<=level){
							if((rnd%10)>=5)
								ret=PSD[monsno][i].ShinkaMons;
						}
						break;
					case SHINKA_SPECIAL_LEVELUP:		//�莝���ɋ󂫂�����΁A�i�����A
														//�J�����c��
						if(PSD[monsno][i].ShinkaData<=level)
							ret=PSD[monsno][i].ShinkaMons;
						break;
					case SHINKA_SPECIAL_NUKENIN:		//�莝���ɋ󂫂������
						break;
					case SHINKA_SPECIAL_BEAUTIFUL:		//�������Ői��
						if(PSD[monsno][i].ShinkaData<=beautiful)
							ret=PSD[monsno][i].ShinkaMons;
						break;
				}
			}
			break;
		case TUUSHIN_SHINKA:
			for(i=0;i<5;i++){
				switch(PSD[monsno][i].ShinkaCond){
					case SHINKA_TUUSHIN:
						ret=PSD[monsno][i].ShinkaMons;
						break;
					case SHINKA_TUUSHIN_ITEM:
						if(PSD[monsno][i].ShinkaData==item){
							item=0;
							PokeParaPut(PP,ID_item,(u8 *)&item);
							ret=PSD[monsno][i].ShinkaMons;
						}
						break;
				}
			}
			break;
		case ITEM_SHINKA:
		case ITEM_SHINKA_CHECK:
			for(i=0;i<5;i++){
				if((PSD[monsno][i].ShinkaCond==SHINKA_ITEM)&&
				   (PSD[monsno][i].ShinkaData==itemno)){
					ret=PSD[monsno][i].ShinkaMons;
					break;
				}
			}
			break;
	}

	return ret;
}

//===============================================================
//
//		�}�Ӄi���o�[���烂���X�^�[�i���o�[���Q�b�g
//
//	in : zukanno	�}�Ӄi���o�[
//
//	out : monsno	�����X�^�[�i���o�[
//
//===============================================================

u16		PokeHouenNo2MonsNoGet(u16 zukanno)
{
	u16	monsno;

	if(zukanno==0)
		return 0;

	for(monsno=0;monsno<sizeof(MonsNo2ZukanNoData)/2;monsno++){
		if(MonsNo2ZukanNoData[monsno]==zukanno)
			break;
	}
	if(monsno==sizeof(MonsNo2ZukanNoData)/2)
		return 0;

	return monsno+1;
}

u16		PokeZukanNo2MonsNoGet(u16 zukanno)
{
	u16	monsno;

	if(zukanno==0)
		return 0;

	for(monsno=0;monsno<sizeof(MonsNo2ZenkokuNoData)/2;monsno++){
		if(MonsNo2ZenkokuNoData[monsno]==zukanno)
			break;
	}
	if(monsno==sizeof(MonsNo2ZenkokuNoData)/2)
		return 0;

	return monsno+1;
}

#if 0
u16		PokeZukanNo2HouenNoGet(u16 zukanno)
{
	u16	monsno;

	if(zukanno==0)
		return 0;

	monsno=PokeZukanNo2MonsNoGet(zukanno);
	return(PokeMonsNo2HouenNoGet(monsno));
}
#else
u16		PokeZukanNo2HouenNoGet(u16 zukanno)
{
	u16	monsno;

	if(zukanno==0)
		return 0;

	for(monsno=0;monsno<sizeof(HouenNo2ZenkokuNoData)/2;monsno++){
		if(HouenNo2ZenkokuNoData[monsno]==zukanno)
			break;
	}
	if(monsno==sizeof(HouenNo2ZenkokuNoData)/2)
		return 0;

	return monsno+1;
}
#endif

//===============================================================
//
//		�����X�^�[�i���o�[����}�Ӄi���o�[���Q�b�g
//
//	in : monsno	�����X�^�[�i���o�[
//
//	out : zukanno	�}�Ӄi���o�[
//
//===============================================================

u16		PokeMonsNo2ZukanNoGet(u16 monsno)
{
	if(monsno==0) 
		return 0;
	return MonsNo2ZenkokuNoData[monsno-1];
}

//-----------------�ً}�쐬2002.5.7 tetsu-------------------
//�����X�^�[�i���o�[�@���@�z�E�G���}�ӂɕϊ�
u16		PokeMonsNo2HouenNoGet(u16 monsno)
{
	if(monsno==0) 
		return 0;
	return MonsNo2ZukanNoData[monsno-1];
}
//-----------------�ً}�쐬2002.5.7 tetsu-------------------

#if 0
u16		PokeHouenNo2ZukanNoGet(u16 zukanno)
{
	u16	monsno;

	if(zukanno==0)
		return 0;

	monsno=PokeHouenNo2MonsNoGet(zukanno);
	return(PokeMonsNo2ZukanNoGet(monsno));
}
#else
u16		PokeHouenNo2ZukanNoGet(u16 zukanno)
{
	if(zukanno==0)
		return 0;

	return(HouenNo2ZenkokuNoData[zukanno-1]);
}
#endif

//===============================================================
//
//		�����X�^�[�i���o�[����{�C�X�i���o�[���Q�b�g
//
//	in : monsno	�����X�^�[�i���o�[
//
//	out : voiceno	�����i���o�[
//
//===============================================================

u16		PokeMonsNo2VoiceNoGet(u16 monsno)
{
	if(monsno<PV_ANNON_START_NO)
		return	monsno;
	if(monsno<PV_NEWPOKE_START_NO)
		return MONSNO_ANNOON-1;

	return MonsNo2VoiceNoData[monsno-PV_NEWPOKE_START_NO];
}

//===============================================================
//
//		�u�`�|�P�����Ɍ���������u�`������
//
//	in : monsno	�u�`�|�P�����̃|�P�����i���o�[
//	     rnd	�u�`�|�P�����̌�����
//		 adrs	�O���t�B�b�N�f�[�^���W�J����Ă���̈�̐擪�A�h���X
//
//		�u�`�|�P�����ł͂Ȃ����邢�͌�p�̏ꍇ�́A�����������^�[��
//
//===============================================================

void	PokeBuchiMake(u16 monsno,u32 rnd,u8 *adrs)
{
	int	i,x,y;
	u8	*writeadrs;
	u8	setx,sety;
	int	bit;

	if(monsno!=MONSNO_PATTIIRU)		//�u�`�|�P�����̃|�P�����i���o�[
		return;

	if(((u32)adrs==DecordAdrs[BSS_CLIENT_MINE])||
	   ((u32)adrs==DecordAdrs[BSS_CLIENT_MINE2]))
		return;

	for(i=0;i<4;i++){
		setx=BuchiData[i].posx+((rnd&0x0f)-8);
		sety=BuchiData[i].posy+(((rnd&0xf0)>>4)-8);
		for(y=0;y<16;y++){
			bit=BuchiData[i].dotdata[y];
			for(x=setx;x<setx+16;x++){
				writeadrs=adrs+(0x20*(x/8))+((x%8)/2)+(0x100*(sety/8))+(0x004*(sety%8));
				if(bit&1){
					if(x&1){
						if(((writeadrs[0]&0xf0)>=0x10)&&((writeadrs[0]&0xf0)<=0x30))
							writeadrs[0]+=0x40;
					}
					else{
						if(((writeadrs[0]&0x0f)>=0x01)&&((writeadrs[0]&0x0f)<=0x03))
							writeadrs[0]+=0x04;
					}
				}
				bit=bit>>1;
			}
			sety++;
		}
		rnd=rnd>>8;
	}
}

void	PokeBuchiMakeFB(u16 monsno,u32 rnd,u8 *adrs,u8 dir)
{
	int	i,x,y;
	u8	*writeadrs;
	u8	setx,sety;
	int	bit;

	if(monsno!=MONSNO_PATTIIRU)		//�u�`�|�P�����̃|�P�����i���o�[
		return;

	if(dir==POKE_BACK)				//�w�ʂɂ̓u�`�����Ȃ�
		return;

	for(i=0;i<4;i++){
		setx=BuchiData[i].posx+((rnd&0x0f)-8);
		sety=BuchiData[i].posy+(((rnd&0xf0)>>4)-8);
		for(y=0;y<16;y++){
			bit=BuchiData[i].dotdata[y];
			for(x=setx;x<setx+16;x++){
				writeadrs=adrs+(0x20*(x/8))+((x%8)/2)+(0x100*(sety/8))+(0x004*(sety%8));
				if(bit&1){
					if(x&1){
						if(((writeadrs[0]&0xf0)>=0x10)&&((writeadrs[0]&0xf0)<=0x30))
							writeadrs[0]+=0x40;
					}
					else{
						if(((writeadrs[0]&0x0f)>=0x01)&&((writeadrs[0]&0x0f)<=0x03))
							writeadrs[0]+=0x04;
					}
				}
				bit=bit>>1;
			}
			sety++;
		}
		rnd=rnd>>8;
	}
}

//===============================================================
//
//		�i�����̃j�b�N�l�[����
//
//	in : PP		�|�P�����̍\���̃|�C���^
//
//===============================================================

void	PokeParaNickNameSet(PokemonParam *PP,u16 beforemons,u16 aftermons)
{
	PokeParaGet(PP,ID_nickname,StrTempBuffer0);
	if(PM_strcmp(PokeNameData[beforemons],StrTempBuffer0)==0)
		PokeParaPut(PP,ID_nickname,(u8 *)&PokeNameData[aftermons]);
}

//===============================================================
//
//		�}���`�v���C�ʐM���̃`�[�����ł̑O�q�E��q���擾
//
//	out :   0		�O�q
//			0�ȊO	��q
//
//===============================================================

u16	GetTeamPos(void)
{
	u16	ret=0;

	switch(SioTrainerInfo[GetSioConnectId()].StandPos){
		case 0:
		case 3:
			ret=0;
			break;
		case 1:
		case 2:
			ret=1;
			break;
	}
	return ret;
}

u16	GetTeamPos2(u8 id)
{
	u16	ret=0;

	switch(SioTrainerInfo[id].StandPos){
		case 0:
		case 3:
			ret=0;
			break;
		case 1:
		case 2:
			ret=1;
			break;
	}
	return ret;
}

//===============================================================
//
//		�}���`�v���C�ʐM���̃`�[�����ł�ClientNo����ID���擾
//
//	out :   i		ID�i���o�[
//
//===============================================================

int	GetTeamPosID(u16 pos)
{
	int	i;

	for(i=0;i<4;i++){
		if(SioTrainerInfo[i].StandPos==pos)
			break;
	}
	return i;
}

//===============================================================
//
//		�g���[�i�[�̐��ʂ��擾�i�������[�`���Ȃ̖炵�����p�j
//
//	in : trainerID	�g���[�i�[��ID
//	out :   0:�j�@1:���@2:�������l
//
//===============================================================

u8	GetTrainerSex(u16 trainerID)
{
	return TD[trainerID].sex&0x7f;
}

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

u16	PokeChrAbiCalc(u8 chr,u16 para,u8 cond)
{
	u16	ret;

	if((cond<COND_POW)||(cond>COND_SPEDEF)) return para;

	switch(SeikakuAbiTbl[chr][cond-1]){
		case 1:
			ret=para*110;
			ret/=100;
			break;
		case -1:
			ret=para*90;
			ret/=100;
			break;
		default:
			ret=para;
			break;
	}
	return	ret;
}

//===============================================================
//
//		�Ȃ��x�v�Z
//
//	in	:	PP	�Ȃ��x���v�Z����|�P�����\���̂̃|�C���^
//			id	�Ȃ��x�v�Z����|�P�����ɍs�����s��ID�i���o�[�ipoketool.def�ɒ�`�j
//
//===============================================================

const	s8	FriendParaTbl[][3]={
	{  5,  3,  2},		//���x���A�b�v
	{  5,  3,  2},		//�w�͒l�A�b�v�A�C�e���i�^�E�������j�g�p
	{  1,  1,  0},		//�U���⏕�A�C�e���i�X�s�[�_�[���j�g�p
	{  3,  2,  1},		//�{�X��ɎQ���i�W�����[�_�[�A�l�V���A�`�����v�j
	{  1,  1,  0},		//�Z�}�V���g�p
	{  1,  1,  1},		//�A�����
	{ -1, -1, -1},		//�m��
	{ -5, -5,-10},		//�ǂ��̘A������ɂ��m��
	{ -5, -5,-10},		//���x���R�O���ȏ�̕m��
};

void	FriendCalc(PokemonParam *PP,u8 id)
{
	u16	monsno;
	u16	itemno;
	s16	friend;
	s8	friendtmp;
	u8	ofs;
	u8	eqp;

	monsno=PokeParaGet(PP,ID_monsno_egg,0);
	itemno=PokeParaGet(PP,ID_item,0);

	if(itemno==ITEM_NAZONOMI){
		if(sys.FieldBattleFlag==CALL_BATTLE)
			eqp=FRS[0].item_equip;
		else
			eqp=Fld.RamSeed.item_equip;
	}
	else
		eqp=ItemEquipGet(itemno);

	if((monsno==0)||(monsno==MONSNO_TAMAGO))
		return;

	ofs=0;

	friend=PokeParaGet(PP,ID_friend,0);

	if(friend>=100)
		ofs++;
	if(friend>=200)
		ofs++;

	if(id==FRIEND_TSUREARUKI){
		if(pp_rand()&1)
			return;
	}
	if(id==FRIEND_BOSS_BATTLE){
		if((FightType&FIGHT_TYPE_TRAINER)==0)
			return;
		if((TD[FieldTrainerID].type!=TRTYPE_ZIMURIIDAA)&&
		   (TD[FieldTrainerID].type!=TRTYPE_SITENNOU)&&
		   (TD[FieldTrainerID].type!=TRTYPE_TYANPION))
			return;
	}

	friendtmp=FriendParaTbl[id][ofs];

	if(friendtmp>0){
		if(eqp==SOUBI_NATUKIDOUP)
			friendtmp=friendtmp*150/100;
	}

	friend+=friendtmp;

	if( friendtmp>0 && (PokeParaGet(PP,ID_get_ball,0)==ITEM_GOOZYASUBOORU ))
		friend++;

	if( friendtmp>0 && (PokeParaGet(PP,ID_get_place,0)==GetNowWorldPos() ))
		friend++;

	if(friend<0)
		friend=0;
	if(friend>255)
		friend=255;

	PokeParaPut(PP,ID_friend,(u8 *)&friend);
}

//===============================================================
//
//		�w�͒l�Q�b�g���[�`��
//
//	in	:	PP		�w�͒l���Q�b�g����|�P�����\���̂̃|�C���^
//			monsno	�|��������|�P�����i���o�[
//
//===============================================================

void	ParaExpGet(PokemonParam *PP,u16 monsno)
{
	int	i;
	s16	work=0;
	u8	bai;
	u8	para[6];
	u16	totalpara;
	u16	itemno;
	u8	eqp;

	totalpara=0;
	for(i=0;i<6;i++){
		para[i]=PokeParaGet(PP,ID_hp_exp+i,0);
		totalpara+=para[i];
	}
	for(i=0;i<6;i++){
		if(totalpara>=PARA_EXP_TOTAL_MAX)
			break;
		if(PokerusedCheck(PP,0))
			bai=2;
		else
			bai=1;
		switch(i){
			case 0:
				work=PPD[monsno].pains_hp*bai;
				break;
			case 1:
				work=PPD[monsno].pains_pow*bai;
				break;
			case 2:
				work=PPD[monsno].pains_def*bai;
				break;
			case 3:
				work=PPD[monsno].pains_agi*bai;
				break;
			case 4:
				work=PPD[monsno].pains_spepow*bai;
				break;
			case 5:
				work=PPD[monsno].pains_spedef*bai;
				break;
		}
		itemno=PokeParaGet(PP,ID_item,0);
		if(itemno==ITEM_NAZONOMI){
			if(sys.FieldBattleFlag==CALL_BATTLE)
				eqp=FRS[0].item_equip;
			else
				eqp=Fld.RamSeed.item_equip;
		}
		else
			eqp=ItemEquipGet(itemno);
		//�A�C�e���œw�͒l2�{�̌��ʂ𑕔����Ă���ꍇ��2�{����
		if(eqp==SOUBI_DORYOKUTINIBAI)
			work=work*2;
		if(totalpara+work>PARA_EXP_TOTAL_MAX)
			work-=((totalpara+work)-PARA_EXP_TOTAL_MAX);
		if(para[i]+work>255)
			work-=((para[i]+work)-255);
		para[i]+=work;
		totalpara+=work;
		PokeParaPut(PP,ID_hp_exp+i,(u8 *)&para[i]);
	}
}

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

u16		ParaExpTotalGet(PokemonParam *PP)
{
	int	i;
	u16	ret;

	ret=0;
	for(i=0;i<6;i++)
		ret+=PokeParaGet(PP,ID_hp_exp+i,0);

	return ret;
}

//===============================================================
//
//		�|�P�����Ƀ|�P���X������������
//
//	in	:	PP		����������|�P�����\���̃|�C���^
//
//===============================================================

void	PokerusSetCheck(PokemonParam *PP)
{
	u16	rnd;
	u8	pokerus;

	rnd=pp_rand();

	if((rnd==16384)||
	   (rnd==32768)||
	   (rnd==49152)){
		do{
			rnd=pp_rand()%6;
			if((PokeParaGet(&PP[rnd],ID_monsno,0))&&
			   (PokeParaGet(&PP[rnd],ID_tamago_flag,0)==0))
				break;
			else
				rnd=6;
		}while(rnd==6);
		if(PokerusedCheck(PP,no2bittbl[rnd])==0){
			do{
				pokerus=pp_rand()&0xff;
			}while(pokerus==0);
			if(pokerus&0xf0)
				pokerus&=0x07;
			pokerus|=(pokerus<<4);
			pokerus&=0xf3;
			pokerus++;
			PokeParaPut(&PP[rnd],ID_pokerus,(u8 *)&pokerus);
		}
	}
}

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
//	out :	�������Ă���|�P�������r�b�g�ŕԂ�
//
//===============================================================

u8		PokerusCheck(PokemonParam *PP,u8 checkbit)
{
	int	i=0,j=1;
	u8	ret=0;

	if(checkbit){
		do{
			if(checkbit&1){
				if(PokeParaGet(&PP[i],ID_pokerus,0)&0x0f)
					ret|=j;
			}
			i++;
			j=j<<1;
			checkbit=checkbit>>1;
		}while(checkbit!=0);
	}
	else{
		if(PokeParaGet(PP,ID_pokerus,0)&0x0f)
			ret++;
	}
	return ret;
}

//�ߋ��Ɋ������Ă������Ƃ�����
u8		PokerusedCheck(PokemonParam *PP,u8 checkbit)
{
	int	i=0,j=1;
	u8	ret=0;

	if(checkbit){
		do{
			if(checkbit&1){
				if(PokeParaGet(&PP[i],ID_pokerus,0))
					ret|=j;
			}
			i++;
			j=j<<1;
			checkbit=checkbit>>1;
		}while(checkbit!=0);
	}
	else{
		if(PokeParaGet(PP,ID_pokerus,0))
			ret++;
	}
	return ret;
}

//===============================================================
//
//		�|�P���X�̓����J�E���^�[���f�N�������g����
//
//	in	:	day		�o�ߓ���
//
//===============================================================

void	PokerusCounterDec(u16 day)
{
//	int	i,j;
	int	i;
	u8	pokerus;

	for(i=0;i<6;i++){
		if(PokeParaGet(&PokeParaMine[i],ID_monsno,0)){
			pokerus=PokeParaGet(&PokeParaMine[i],ID_pokerus,0);
			if(pokerus&0x0f){
				if(((pokerus&0x0f)<day)||(day>4))
					pokerus&=0xf0;
				else
					pokerus-=day;
				PokeParaPut(&PokeParaMine[i],ID_pokerus,(u8 *)&pokerus);
			}
		}
	}
#if 0
	for(i=0;i<BOX_KAZU;i++){
		for(j=0;j<BOX_PM_KAZU;j++){
			if(PokePasoParaGet(&BoxData.PPPBox[i][j],ID_monsno,0)){
				pokerus=PokePasoParaGet(&BoxData.PPPBox[i][j],ID_pokerus,0);
				if(pokerus&0x0f){
					if(((pokerus&0x0f)<day)||(day>4))
						pokerus&=0xf0;
					else
						pokerus-=day;
					PokePasoParaPut(&BoxData.PPPBox[i][j],ID_pokerus,(u8 *)&pokerus);
				}
			}
		}
	}
#endif
}

//===============================================================
//
//		�|�P���X�̊����`�F�b�N���[�`��
//
//	in	:	PP			�������`�F�b�N����|�P�����\���̂̃|�C���^
//
//===============================================================

void	PokerusCatchCheck(PokemonParam *PP)
{
	int	i;
	u8	pokerus;

	if(pp_rand()%3==0){
		for(i=0;i<6;i++){
			if(PokeParaGet(&PP[i],ID_monsno,0)){
				if((pokerus=PokeParaGet(&PP[i],ID_pokerus,0))){
					if(pokerus&0x0f){
						if(i!=0){
							if((PokeParaGet(&PP[i-1],ID_pokerus,0)&0xf0)==0)
								PokeParaPut(&PP[i-1],ID_pokerus,(u8 *)&pokerus);
						}
						if(i!=5){
							if((PokeParaGet(&PP[i+1],ID_pokerus,0)&0xf0)==0){
								PokeParaPut(&PP[i+1],ID_pokerus,(u8 *)&pokerus);
								i++;
							}
						}
					}
				}
			}
		}
	}
}

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

u8	PokeLevelUpCheck(PokemonParam *PP)
{
	u16	monsno;
	u8	level;
	u32	exp;

	monsno=PokeParaGet(PP,ID_monsno,0);
	level=PokeParaGet(PP,ID_level,0)+1;
	exp=PokeParaGet(PP,ID_exp,0);

	if(exp>GrowTbl[PPD[monsno].grow][level]){
		PokeParaPut(PP,ID_level,(u8 *)&level);
		return 1;
	}
	return 0;
}

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

u32		PokeWazaMachineCheck(PokemonParam *PP,u8 machineno)
{
	u16	monsno;
	u32	machinebit;

	monsno=PokeParaGet(PP,ID_monsno_egg,0);

	if(monsno==MONSNO_TAMAGO)
		return 0;

	if(machineno<32){
		machinebit=(1<<machineno);
		return(MachineBitTable[monsno].machinebit1&machinebit);
	}

	machinebit=(1<<(machineno-32));
	return(MachineBitTable[monsno].machinebit2&machinebit);
}

//===============================================================
//
//		�|�P�������o����Z���w�肳�ꂽ�o�b�t�@�Ɋi�[����
//		���Ɋo���Ă���Z�͊i�[���Ȃ�
//
//	in	:	PP			�Z�����ڂ���|�P�����\���̂̃|�C���^
//			nobuf		�Z�i���o�[���i�[����o�b�t�@�̃|�C���^
//
//	out	:	�i�[�����Z�i���o�[�̐�
//
//===============================================================

u8		PokeOboeruWazaGet(PokemonParam *PP,u16 *nobuf)
{
	int	i,j;
	u8	wazacnt=0;
	u16	wazano[4];
	u16	monsno;
	u8	level;

	monsno=PokeParaGet(PP,ID_monsno,0);
	level=PokeParaGet(PP,ID_level,0);

	for(i=0;i<4;i++)
		wazano[i]=PokeParaGet(PP,ID_waza1+i,0);

	for(i=0;i<20;i++){
		if(WazaOboeData[monsno][i]==0xffff)
			break;
		if((WazaOboeData[monsno][i]&0xfe00)<=(level<<9)){
			for(j=0;j<4;j++){
				if(wazano[j]==(WazaOboeData[monsno][i]&0x01ff))
					break;
			}
			if(j==4){
				for(j=0;j<wazacnt;j++){
					if(nobuf[j]==(WazaOboeData[monsno][i]&0x01ff))
						break;
				}
				if(j==wazacnt)
					nobuf[wazacnt++]=WazaOboeData[monsno][i]&0x1ff;
			}
		}
	}
	return wazacnt;
}

//===============================================================
//
//		�|�P�������o���邷�ׂĂ̋Z���w�肳�ꂽ�o�b�t�@�Ɋi�[����
//		�|�P���������݊o���Ă���Z�����ׂē����
//
//	in	:	PP			�Z�����ڂ���|�P�����\���̂̃|�C���^
//			nobuf		�Z�i���o�[���i�[����o�b�t�@�̃|�C���^
//
//	out	:	�i�[�����Z�i���o�[�̐�
//
//===============================================================
u8		PokeOboeruWazaAllGet(u16 monsno,u16 *nobuf)
{
	int	i;
	u8	wazacnt=0;

	for(i=0;i<20;i++){									//�|�P�������o����Z���I�[�܂Ŋi�[
		if(WazaOboeData[monsno][i]==0xffff) break;
		nobuf[wazacnt++]=WazaOboeData[monsno][i]&0x1ff;
	}
	return wazacnt;
}

u8		PokeOboeruWazaCheck(PokemonParam *PP)
{
	int	i,j;
	u8	wazacnt=0;
	u16	wazano[4];
	u16	monsno;
	u8	level;
	u16	nobuf[20];

	monsno=PokeParaGet(PP,ID_monsno_egg,0);
	level=PokeParaGet(PP,ID_level,0);

	//�^�}�S�͖������ŁA���ڂ��Ȃ��ŕԂ�
	if(monsno==MONSNO_TAMAGO)
		return 0;

	for(i=0;i<4;i++)
		wazano[i]=PokeParaGet(PP,ID_waza1+i,0);

	for(i=0;i<20;i++){
		if(WazaOboeData[monsno][i]==0xffff)
			break;
		if((WazaOboeData[monsno][i]&0xfe00)<=(level<<9)){
			for(j=0;j<4;j++){
				if(wazano[j]==(WazaOboeData[monsno][i]&0x01ff))
					break;
			}
			if(j==4){
				for(j=0;j<wazacnt;j++){
					if(nobuf[j]==(WazaOboeData[monsno][i]&0x01ff))
						break;
				}
				if(j==wazacnt)
					nobuf[wazacnt++]=WazaOboeData[monsno][i]&0x1ff;
			}
		}
	}
	return wazacnt;
}

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
u16	PokemonNumberGet(u16 pokenum)
{
#ifdef FESTA_VER_ROM
	return		0xffff;
#else
#ifdef PR_VER_ROM
	return		0xffff;
#endif
#endif
	//�S���}�ӂ��݂�邩�ǂ����i0:�݂�Ȃ�,1:�݂��j
	if(ZukanSpecialFlagCheck()){
		return		PokeMonsNo2ZukanNoGet(pokenum);
	}else{
		pokenum = PokeMonsNo2HouenNoGet(pokenum);
		//�z�E�G���}�ӂɓo�^����Ă��邩�ǂ���
		if(pokenum <= NEW_POKE_NUM_MAX){
			//�o�^����Ă���
			return		pokenum;
		}else{
			//�o�^����Ă��Ȃ�
			return		0xffff;
		}
	}
	return		0xffff;
}

//===============================================================
//		�|�P�����̃A�j���[�V�����i���o�[�ێ����[�N�̏�����
//===============================================================

void	PokeAnmNoInit(void)
{
	int	i;
	
	for(i=0;i<CLIENT_MAX;i++)
		BattlePokeAnmNo[i]=0;
}

//===============================================================
//		�퓬�Ȃ�炷
//===============================================================

u16	GetFightBGM(void)
{
	if(FightType&FIGHT_TYPE_HAIKEI_SET)
		return MUS_BATTLE34;
	else if(FightType&FIGHT_TYPE_TENJI)
		return MUS_BATTLE36;
	else if(FightType&FIGHT_TYPE_TUUSHIN)
		return MUS_BATTLE20;
	else if(FightType&FIGHT_TYPE_TRAINER){
		switch(TD[FieldTrainerID].type){
			case TRTYPE_AKUADANRIIDAA:
			case TRTYPE_MAGUMADANRIIDAA:
				return MUS_BATTLE30;
			case TRTYPE_AKUADAN:
			case TRTYPE_AKUADANKANBU:
			case TRTYPE_MAGUMADAN:
			case TRTYPE_MAGUMADANKANBU:
				return MUS_BATTLE31;
			case TRTYPE_ZIMURIIDAA:
				return MUS_BATTLE32;
			case TRTYPE_TYANPION:
				return MUS_BATTLE33;
			case TRTYPE_POKEMONTOREENAA:
				if(PM_strcmp(TD[FieldTrainerID].name,MitsuruNameMsg)==0)
					return MUS_BATTLE20;
				else
					return MUS_BATTLE35;
			case TRTYPE_SITENNOU:
				return MUS_BATTLE38;
			default:
				return MUS_BATTLE20;
		}
	}
	else
		return MUS_BATTLE27;
}

void	FightBGMSet(void)
{
	BGM_Init();
	m4aMPlayAllStop();
	MusicPlay(GetFightBGM());
}

void	FightBGMSet2(u16 music)
{
	BGM_Init();
	m4aMPlayAllStop();
	if(music != 0){
		//�a�f�l�w�肠��
		BGM_PlaySet(music);
	}else{
		BGM_PlaySet(GetFightBGM());
	}
}

//===============================================================
//
//		�|�P�����̃J���[���Q�b�g
//
//	in	:	PP	�Q�b�g����|�P�����̍\���̂̃|�C���^
//
//	out	:	�J���[�̃A�h���X
//
//===============================================================

u32		PokePalGet(PokemonParam *PP)
{
	u16	monsno;
	u32	id;
	u32	rnd;

	monsno=PokeParaGet(PP,ID_monsno_egg,0);
	id=PokeParaGet(PP,ID_id_no,0);
	rnd=PokeParaGet(PP,ID_personal_rnd,0);

	return PokePalGetPara(monsno,id,rnd);
}

//===============================================================
//
//		�|�P�����̃J���[���Q�b�g�i�p�����[�^�w��j
//
//	in	:	monsno	�Q�b�g���郂���X�^�[No
//			id		�Q�b�g����|�P������ID
//			rnd		�Q�b�g����|�P�����̌�����
//
//	out	:	�J���[�̃A�h���X
//
//===============================================================

u32		PokePalGetPara(u16 monsno,u32 id,u32 rnd)
{
#ifdef FESTA_VER_ROM
	return (u32)PM2NObjDataPal[monsno].pal_data;
#else
	if(monsno>MONSNO_END)
		return (u32)PM2NObjDataPal[0].pal_data;
	else{
		if( ( ((id&0xffff0000)>>16) ^ (id&0xffff) ^ ((rnd&0xffff0000)>>16) ^ (rnd&0xffff) ) < 8 )
			return (u32)PM2RObjDataPal[monsno].pal_data;
		else
			return (u32)PM2NObjDataPal[monsno].pal_data;
	}
#endif
}

//===============================================================
//
//		�|�P�����̃J���[���Q�b�g
//
//	in	:	PP	�Q�b�g����|�P�����̍\���̂̃|�C���^
//
//	out	:	�J���[�e�[�u���̃A�h���X
//
//===============================================================

u32		PokePalDataGet(PokemonParam *PP)
{
	u16	monsno;
	u32	id;
	u32	rnd;

	monsno=PokeParaGet(PP,ID_monsno_egg,0);
	id=PokeParaGet(PP,ID_id_no,0);
	rnd=PokeParaGet(PP,ID_personal_rnd,0);

	return PokePalDataGetPara(monsno,id,rnd);
}

//===============================================================
//
//		�|�P�����̃J���[���Q�b�g�i�p�����[�^�w��j
//
//	in	:	monsno	�Q�b�g���郂���X�^�[No
//			id		�Q�b�g����|�P������ID
//			rnd		�Q�b�g����|�P�����̌�����
//
//	out	:	�J���[�̃A�h���X
//
//===============================================================

u32		PokePalDataGetPara(u16 monsno,u32 id,u32 rnd)
{
#ifdef FESTA_VER_ROM
	return (u32)&PM2NObjDataPal[monsno];
#else
	if( ( ((id&0xffff0000)>>16) ^ (id&0xffff) ^ ((rnd&0xffff0000)>>16) ^ (rnd&0xffff) ) < 8 )
		return (u32)&(PM2RObjDataPal[monsno]);
	else
		return (u32)&(PM2NObjDataPal[monsno]);
#endif
}

//===============================================================
//
//		��`�Z���ǂ����`�F�b�N
//
//	in	:	wazano	�`�F�b�N����ZNo
//
//	out	:	0:��`�Z�ł͂Ȃ��@1:��`�Z�ł�
//
//===============================================================

const u16 HidenWazaTbl[]={
	WAZANO_IAIGIRI,		//����������
	WAZANO_SORAWOTOBU,	//������Ƃ�
	WAZANO_NAMINORI,	//�Ȃ݂̂�
	WAZANO_KAIRIKI,		//�����肫
	WAZANO_HURASSYU,	//�t���b�V��
	WAZANO_IWAKUDAKI,	//��������
	WAZANO_TAKINOBORI,	//�����̂ڂ�
	WAZANO_DAIBINGU,	//�_�C�r���O
	0xffff,
};

u32		HidenWazaCheck(u16 wazano)
{
	int	i=0;

	while(HidenWazaTbl[i]!=0xffff){
		if(HidenWazaTbl[i++]==wazano)
			return 1;
	}
	return 0;
}

//===============================================================
//
//		���]�\���\�ȃ|�P�������ǂ����`�F�b�N
//
//	in	:	monsno	�`�F�b�N���郂���X�^�[No
//
//	out	:	0:���]�\�@1:���]�s��
//
//===============================================================

u8		PokeReverseCheck(u16 monsno)
{
	return PPD[monsno].reverse;
}

//===============================================================
//
//		�|�P�����̍D�݂̖����ǂ����`�F�b�N
//
//	in	:	PP		�`�F�b�N����|�P�����\���̂̃|�C���^
//			taste	�`�F�b�N���閡
//
//	out	:	-1:�����@0:����	1:�D��
//
//===============================================================

s8		PokeLikeTasteCheck(PokemonParam *PP,u8 taste)
{
	u8	chr;

	chr=PokeSeikakuGet(PP);

	return (cube_like_tbl[chr*5+taste]);
}

s8		PokeLikeTasteCheckRnd(u32 rnd,u8 taste)
{
	u8	chr;

	chr=PokeSeikakuGetRnd(rnd);

	return (cube_like_tbl[chr*5+taste]);
}

//===============================================================
//
//		�����̎����|�P�������ǂ����`�F�b�N
//
//	in	:	PP		�`�F�b�N����|�P�����\���̂̃|�C���^
//
//	out	:	0:������	1:���l��
//
//===============================================================

u8		PokeOneSelfCheck(PokemonParam *PP)
{
#if 0
	int	i;
	u32	idno;
	u8	buf[PERSON_NAME_SIZE+EOM_SIZE];

	idno=(MyData.ID[0]<< 0)|
		 (MyData.ID[1]<< 8)|
		 (MyData.ID[2]<<16)|
		 (MyData.ID[3]<<24);

	if(PokeParaGet(PP,ID_id_no,0)==idno){
		i=0;
		PokeParaGet(PP,ID_oyaname,buf);
		while(buf[i]!=EOM_){
			if(buf[i]!=MyData.my_name[i])
				return 1;
			i++;
		}
		return 0;
	}
	return 1;
#endif
	u8	buf[PERSON_NAME_SIZE+EOM_SIZE];
	PokeParaGet(PP,ID_oyaname,buf);
	return PokeOneSelfCheckAct(PokeParaGet(PP,ID_id_no,0),(u8 *)&buf);

}

u8		PokeOneSelfCheckAct(u32 id,u8 *oyaname)
{
	int	i;
	u32	idno;

	idno=(MyData.ID[0]<< 0)|
		 (MyData.ID[1]<< 8)|
		 (MyData.ID[2]<<16)|
		 (MyData.ID[3]<<24);

	if(id==idno){
		i=0;
		while(oyaname[i]!=EOM_){
			if(oyaname[i]!=MyData.my_name[i])
				return 1;
			i++;
		}
		return 0;
	}
	return 1;
}

//===============================================================
//
//		pp��max�ɂ���i�{�b�N�X�ɗa����Ƃ���p�j
//
//	in	:	PP		max�ɂ���|�P�����\���̂̃|�C���^
//			PPP		max�ɂ���|�P�����\���̂̃|�C���^
//
//===============================================================

void	PokePPMaxSet(PokemonParam *PP)
{
	PokePasoPPMaxSet((PokemonPasoParam *)&PP->PPP);
}

void	PokePasoPPMaxSet(PokemonPasoParam *PPP)
{
	int	i;
	u8	pp;

	for(i=0;i<4;i++){
		if(PokePasoParaGet(PPP,ID_waza1+i,0)!=0){
			pp=PPMaxGet(PokePasoParaGet(PPP,ID_waza1+i,0),PokePasoParaGet(PPP,ID_pp_count,0),i);
			PokePasoParaPut(PPP,ID_pp1+i,(u8 *)&pp);
		}
	}
}

//===============================================================
//
//		�����œ���ւ��ł��Ȃ��Ƃ��ɕ\�����郁�b�Z�[�W���Q�b�g
//
//===============================================================

void	PokeNoReshuffleMsgGet()
{
	SpeabiNo=UserWork[SPEABINO];

	EvStrBuf0[0]=I_MSG;
	EvStrBuf0[1]=EV_NICKNAME;
	EvStrBuf0[2]=UserWork[TOKUSEI_CLIENTNO];
	EvStrBuf0[4]=EOM_;
	if(MineEnemyCheck(UserWork[TOKUSEI_CLIENTNO])==SIDE_MINE)
		EvStrBuf0[3]=SearchListRow(SelMonsNo[UserWork[TOKUSEI_CLIENTNO]]);
	else
		EvStrBuf0[3]=SelMonsNo[UserWork[TOKUSEI_CLIENTNO]];

	EvStrBuf1[0]=I_MSG;
	EvStrBuf1[1]=EV_NICKNAME;
	EvStrBuf1[2]=TempClientNo;
	EvStrBuf1[3]=SearchListRow(SelMonsNo[TempClientNo]);
	EvStrBuf1[4]=EOM_;

	FightMsgCreateAct((u8 *)&TokuseiNoReshuffleMsg,(u8 *)&MsgExpandBuffer);
}

//===============================================================
//
//		�쐫�|�P�����Ɏ�������A�C�e���̃Z�b�g
//
//===============================================================

void	PokeItemSet(void)
{
	u32	rnd;
	u16	monsno;

	if(FightType&(FIGHT_TYPE_TRAINER|FIGHT_TYPE_SPECIAL))
		return;

	rnd=pp_rand()%100;
	monsno=PokeParaGet(&PokeParaEnemy[0],ID_monsno,0);

	if(PPD[monsno].item1==PPD[monsno].item2){
		PokeParaPut(&PokeParaEnemy[0],ID_item,(u8 *)&PPD[monsno].item1);
		return;
	}
	if(rnd<45)
		return;
	else if(rnd<95)
		PokeParaPut(&PokeParaEnemy[0],ID_item,(u8 *)&PPD[monsno].item1);
	else
		PokeParaPut(&PokeParaEnemy[0],ID_item,(u8 *)&PPD[monsno].item2);
}

//===============================================================
//
//		�|�P���������A���ǂ����`�F�b�N
//
//===============================================================

u8		PokeRareGet(PokemonParam *PP)
{
	u32	id;
	u32	rnd;

	id=PokeParaGet(PP,ID_id_no,0);
	rnd=PokeParaGet(PP,ID_personal_rnd,0);

	return(PokeRareGetPara(id,rnd));
}

u8		PokeRareGetPara(u32 id,u32 rnd)
{
	return((((id&0xffff0000)>>16)^(id&0xffff)^((rnd&0xffff0000)>>16)^(rnd&0xffff))<8);
}

//===============================================================
//
//		���Ԃ̃g���[�i�[�����Q�b�g
//
//===============================================================

u8		*TeamTrainerNameGet(void)
{
	u8 id;

	id=GetSioConnectId();
	return (u8 *)&SioTrainerInfo[GetTeamPosID(SioTrainerInfo[id].StandPos^2)].name;
}
