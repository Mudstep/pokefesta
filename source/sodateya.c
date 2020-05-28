//======================================================
//
//    sodateya.c
//
//    �����ĉ��E�^�}�S                                  
//                                                      
//    Copyright (C) 2001 GAME FREAK inc.
//======================================================

//#define  SODATEYA_DEBUG
#define SODATEYA_MAIN

#include "common.h"
#include "intr.h"
#include "agbmons.h"
#include "calctool.h"
#include "actor.h"
#include "actanm.h"
#include "task.h"
#include "mus_tool.h"
#include "a_mine.h"
#include "a_enemy.h"
#include "print.h"
#include "window.h"
#include "script.h"
#include "wazatool.h"
#include "gamedata.h"
#include "ev_fight.h"
#include "..\script\saveflag.h"	
#include "item.h"
#include "pasocom.h"
#include "shinka.h"
#include "mail.h"
#include "ev_flag.h"
#include "fld_main.h"
#include "palanm.h"
#include "fld_task.h"
#include "pm_str.h"

#include "MaDefine.h"
#include "objdata.h"
#include "decord.h"
#include "bss.h"
#include "client.h"
//#include "cli_eff.h"
#include "menutool.h"

#include "server.h"
#include "pokelist.h"
#include "f_pmlist.h"
#include "agbmons.h"
#include "poketool.h"
#include "zukan.h"
#include "mapparam.h"

#include "monsno.def"
#include "wazano.def"
#include "kowaza.h"
#include "sodateya.h"

#include "namein.h"
// =================================================================
// ��`
// =================================================================
#define WAZA_MACHINE_MAX 	(58)		//�Z�}�V���̐�
#define	BIRTH_LEVEL			(5)
#define METAMON_NO			(MONSNO_METAMON)
#define HUKA_FIRST_NATUKIDO (120)

// =================================================================
// extern
// =================================================================
extern PokeShinkaData 	PSD[][5];
extern PokePersonalData	PPD[];
extern const u8 sex_msg[][2];

extern s16 BoxTumeMineData();						//�莝�����l�߂�


extern const u32 DecordAdrs[];
extern const u8  PokeNameData[][6];



// =================================================================
// ��ĉ��p���[�N��`
// =================================================================

#ifdef SODATEYA_DEBUG
static SODATEYA_WORK Sodateya;						//���̃��[�N
#endif



// =================================================================
// �v���g�^�C�v�錾
// =================================================================
void PokemonTamagoSet(PokemonParam *poke, u16 monsno, u8 eventflag);
void PokemonTamagoSet2(PokemonParam *poke, u16 monsno, SODATEYA_WORK *sodateya);
void SioMailDataInit(SIO_MAIL_DATA	*smd);
static  u8 SodateyaPokeAffinitySub(SODATEYA_WORK	*sodateya);
static void TamagoDemoMsgTalkSet(const u8 * msg_adrs);
static void TamagoDemoMsgSet(const u8 *msg_adrs);
static u8 TamagoDemoTalkMsgPut();



// =================================================================
// �����萔��`
// =================================================================

static const u8 AffinityMsg1[]={		//�Q�Ђ��̂Ȃ��͂Ƃ��Ă��悢�悤����
	n2_,hi_,ki_,no_,spc_,na_,ka_,ha_,spc_,to_,ttu_,te_,mo_,spc_,yo_,i_,spc_,yo_,u_,zi_,yya_,
	EOM_,
};
static const u8 AffinityMsg2[]={		//�Q�Ђ��̂Ȃ��͂܂��܂��̂悤����
	n2_,hi_,ki_,no_,spc_,na_,ka_,ha_,spc_,ma_,zu_,ma_,zu_,no_,spc_,yo_,u_,zi_,yya_,
	EOM_,
};
static const u8 AffinityMsg3[]={		//�Q�Ђ��̂Ȃ��͂���قǂ悭�Ȃ����Ȃ�
	n2_,hi_,ki_,no_,spc_,na_,ka_,ha_,spc_,so_,re_,ho_,do_,spc_,yo_,ku_,na_,i_,ga_,na_,aa_,
	EOM_,
};
static const u8 AffinityMsg4[]={		//�Q�Ђ��͂��������ɂ������|�P���������Ƃ�����ł��邪�Ȃ�
	n2_,hi_,ki_,ha_,spc_,o_,ta_,ga_,i_,ni_,spc_,ti_,ga_,u_,CR_,
	PO_,KE_,MO_,N_,ta_,ti_,to_,spc_,a_,so_,n_,de_,o_,ru_,ga_,na_,a_,
	EOM_,
};

static const u8 *const AffinityMsg[]={		//����������e�[�u��
	(u8*)AffinityMsg1,
	(u8*)AffinityMsg2,
	(u8*)AffinityMsg3,
	(u8*)AffinityMsg4,
};

static const u8 cr_msg[]		= {CR_,EOM_,};					//���s
static const u8 yameru_msg[]	= {ya_,me_,ru_,EOM_};		//�u��߂�v
static const u8 level_msg[]		= {L__,V__,EOM_};				//�u�k�u�v
static const u8 tamago_name[]	= {TA_,MA_,GO_,EOM_,};		//�u���܂��v


// =================================================================
// PokemonParamNickNameCopy
// �T�v  : �|�P�����̍\���̂ɒ������O�������Ă��Ă��T���������R�s�[����(�C�O�Ή��p)
// ����  : pp	PokemonParam
//       : name
// �߂�l: u8 *
// =================================================================
u8 *PokeParaNickNameCopy(PokemonParam *pp,u8 *name)
{
	u8 strwork[20];

	PokeParaGet(pp,ID_nickname,strwork);		//�|�P�����̖��O�Q�b�g
	return NickNameCopy(name,strwork);
}


// =================================================================
// PokePasoParamNickNameCopy
// �T�v  : �|�P�����̍\���̂ɒ������O�������Ă��Ă��T���������R�s�[����(�C�O�Ή��p)
// ����  : ppp	PokemonPasoParam
//       : name
// �߂�l: u8 *
// =================================================================
u8 *PokePasoParaNickNameCopy(PokemonPasoParam *ppp,u8 *name)
{
	u8 strwork[20];

	PokePasoParaGet(ppp,ID_nickname,strwork);		//�|�P�����̖��O�Q�b�g
	return NickNameCopy(name,strwork);
	
}
// =================================================================
// PokeAzukeruCheck
// �T�v  : �a���Ă���|�P�����̐���Ԃ�
// ����  : none
// �߂�l: u8
// =================================================================
u8 PokeAzukeruCheck(SODATEYA_WORK *sodateya)
{
	u8 result,i;
	
	result=0;
	for(i=0;i<2;i++){					//�a�����Ă���|�P�����̐����J�E���g
		if(PokePasoParaGet(&sodateya->Pokemon[i],ID_monsno)!=0){
			result++;
		}
	}
	return result;
}

// =================================================================
// PokeAzukeInfoCheck
// �T�v  : ��ĉ��ɗa���Ă���|�P�����̃A�C�e���󋵂𒲂ׂ�
// ����  : sodateya
//       : mailinfo
// �߂�l: none
// =================================================================
void PokeAzukeInfoCheck(SODATEYA_WORK *sodateya, SODATEYA_MAIL *mailinfo)
{
	u8 result,i;
	
	result=0;
	for(i=0;i<2;i++){					//�a�����Ă���|�P�����̐����J�E���g
		if(PokePasoParaGet(&sodateya->Pokemon[i],ID_monsno)!=0){	//�|�P������a���Ă�����J�E���g
			result++;
			if(PokePasoParaGet(&sodateya->Pokemon[i],ID_item)!=0){	//�A�C�e���������Ă���
				mailinfo->PokeFlag[i] = 1;
			}else{
				mailinfo->PokeFlag[i] = 0;							//�A�C�e���������Ă��Ȃ��i�܂��̓��[���������Ă�j
			}
		}else{
			mailinfo->PokeFlag[i] = 1;								//�|�P���������Ȃ�
		}
	}
	mailinfo->PokeNum = result;
}

// =================================================================
// PokeAzukeruCheckGet
// �T�v  : �a������Ȃ�󂢂Ă�ԍ���Ԃ�
// ����  : none
// �߂�l: s8	0,1=�󂢂Ă�ԍ�	-1=����
// =================================================================
s8 PokeAzukeruCheckGet(SODATEYA_WORK *sodateya)
{	
	u8 i;
	for(i=0;i<2;i++){					//�a���ĂȂ��̈悪������
		if(PokePasoParaGet(&(sodateya->Pokemon[i]),ID_monsno)==0){
			return i;
		}
	}
	return -1;							//�����a�����Ă�
}



// =================================================================
// PokeSodateSet
// �T�v  : �|�P������a����T�u
// ����  : p
// �߂�l: none
// =================================================================
void PokeSodateSetSub(PokemonParam *p,SODATEYA_WORK *sodateya)
{
	s8 work;
	u8 mail;

	work = PokeAzukeruCheckGet(sodateya);	//�󂢂Ă�ꏊ��T��
	if(PokeMailCheck(p)){					//���[���������Ă��邩
		PM_strcpy(sodateya->SioMail[work].oyaname,MyData.my_name);			//�a�����e�̖��O�Z�b�g
		PokeParaNickNameCopy(p,sodateya->SioMail[work].pokename);			//�|�P�����̖��O�Z�b�g
		mail = PokeParaGet(p,ID_mailid);									//MailID�擾
		sodateya->SioMail[work].Mail = Fld.MailData[mail];				//���[���R�s�[
		PokeDelMail(p);														//�莝���̃��[���폜
	}
	sodateya->Pokemon[work] = p->PPP;		//�|�P�����f�[�^����ĉ����[�N�ɃZ�b�g
	PokePasoPPMaxSet(&sodateya->Pokemon[work]);	//�o�o����
	sodateya->WalkCount[work] = 0;			//����������
	PokeParaInit(p);						//�莝���̃|�P����������
	BoxTumeMineData();						//�莝�����l�߂�
	PokeCountMineGet();						//�|�P�����̐��Ċm�F
}



// =================================================================
// PokeSodataSet
// �T�v  : �|�P������a����
// ����  : none
// �߂�l: none
// =================================================================
void PokeSodateSet()
{
	PokeSodateSetSub(&PokeParaMine[FPokeNo],&Fld.SodateyaWork);
}

// =================================================================
// SodateBoxTume
// �T�v  : ��ĉ����[�N�̃|�P�������l�߂�
// ����  : sodateya
// �߂�l: none
// =================================================================
static void SodateBoxTume(SODATEYA_WORK *sodateya)
{
	if(PokePasoParaGet(&sodateya->Pokemon[1],ID_monsno)!=0){			//Pokemon[1]���߹�݂�����
		if(PokePasoParaGet(&sodateya->Pokemon[0],ID_monsno)==0){		//Pokemon[0]�ɂ��Ȃ�
			sodateya->Pokemon[0] = sodateya->Pokemon[1];
			PokePasoParaInit(&sodateya->Pokemon[1]);						//[0]��[1]�̓��e���ڂ�
			sodateya->SioMail[0]   = sodateya->SioMail[1];				//���[�����ڂ�
			sodateya->WalkCount[0] = sodateya->WalkCount[1];			//�������ڂ�
			sodateya->WalkCount[1] = 0;
			SioMailDataInit(&sodateya->SioMail[1]);						//[1]��������
		}
	}
}


// =================================================================
// PokeAutoLevelUp
// �T�v  : �Z�o���������I�ɍs�����x���A�b�v����
// ����  : pp
// �߂�l: none
// =================================================================
static void PokeAutoLevelUp(PokemonParam *pp)
{
	int i;
	u8 flg;
	u16 waza;
	
	for(i=0;i<100;i++){							//�ǂ����P�O�O�܂ł������x���͂Ȃ���
		if(PokeLevelUpCheck(pp)){				//���x���A�b�v�������H
//			if(PokeWazaOboeCheck(pp)==NO_WAZA_SET){				//�Z���o���邩
			flg = 1;
			while((waza=PokeWazaOboeCheck(pp,flg))){	//�Z���o���邩
				flg=0;
				if(waza==NO_WAZA_SET){
					PokeWazaOboeOshidashi(pp,OboeWazaNo);	//�Z�������ς��ł�������艟������
				}
			}
		}else{									//���x���A�b�v���Ȃ��������I��
			break;
		}
	}
	PokeParaCalc(pp);
}

// =================================================================
// PokeHikitoriSub
// �T�v  : �|�P�������������T�u
// ����  : sodateya
//       : no
// �߂�l: u8 �a���Ă�|�P�����̎c��
// =================================================================
u16 PokeHikitoriSub(SODATEYA_WORK *sodateya,u8 no)
{
	PokemonParam	pp;
	u32 exp;
	u16 monsno;

	PokePasoParaNickNameCopy(&(sodateya->Pokemon[no]),StrTempBuffer0);		//���O�Z�b�g
	monsno = PokePasoParaGet(&(sodateya->Pokemon[no]),ID_monsno);

	PokeReplace(&sodateya->Pokemon[no],&pp);				//���̃|�P�����p�����[�^���擾
	if(PokeParaGet(&pp,ID_level)!=100){
		exp =  PokeParaGet(&pp,ID_exp);							//�o���l�𑫂�
		exp += sodateya->WalkCount[no];
		PokeParaPut(&pp,ID_exp,(u8*)&exp);
		PokeAutoLevelUp(&pp);									//���ٱ��ߏ���
	}
	
	PokeParaMine[5] = pp;									//�Ă����Ƀ|�P�������i�[
	if(sodateya->SioMail[no].Mail.design_no!=NON_ITEM){		//���[������������
		PokeAddWrittenMail(&PokeParaMine[5], &sodateya->SioMail[no].Mail);	//�莝���Ƀ��[�����Z�b�g
		SioMailDataInit(&sodateya->SioMail[no]);							//��ĉ��̃��[����������
	}
	BoxTumeMineData();										//�莝�����l�߂�
	PokePasoParaInit(&sodateya->Pokemon[no]);				//��ĉ����[�N�������
	sodateya->WalkCount[no] = 0;							//�����J�E���g������
	SodateBoxTume(sodateya);								//
	PokeCountMineGet();										//�|�P�����̐��Ċm�F

	return monsno;											//�Ђ��Ƃ����|�P�����̔ԍ�
}

// =================================================================
// PokeHikitori
// �T�v  : �|�P�������������
// ����  : none
// �߂�l: �Ђ��Ƃ����|�P�����̔ԍ��ETempBuffer0�Ɉ���������|�P�����̖��O
// =================================================================
u16 PokeHikitori()
{
	return PokeHikitoriSub(&Fld.SodateyaWork,ScriptParameter0);
}

// =================================================================
// GetSinkaPokeLevel
// �T�v  : �����J�E���g���������̃|�P�����̃��x����Ԃ�
// ����  : p
//       : walk
// �߂�l: u8
// =================================================================
u8 GetSinkaPokeLevel(PokemonPasoParam *p, u32 walk)
{
	PokemonPasoParam	poke;
	u32	exp;

	poke = *p;
	exp   = PokePasoParaGet(p,ID_exp);				//�ŏ��̌o���l���Z�o
	exp   += walk;					//����������
	PokePasoParaPut(&poke,ID_exp,(u8*)&exp);					//�V�����o���l�Ƃ��ăZ�b�g
	return PokePasoLevelCalc(&poke);			//�V�������x�����Z�o
	
}

// =================================================================
// SodatePokeLevelSetSub
// �T�v  : ���x�����𕶎���Ŋi�[����
// ����  : sodateya
//       : no
// �߂�l: none
// =================================================================
u8 GetSodatePokeLevel(SODATEYA_WORK *sodateya,u8 no)
{
	u8 level1,level2;

	level1 = PokePasoLevelCalc(&sodateya->Pokemon[no]);	//�ŏ��̃��x�����Z�o
	level2 = GetSinkaPokeLevel(&sodateya->Pokemon[no], 
			 sodateya->WalkCount[no]);					//�V�������x�����Z�o
	
	return level2-level1;								//���x������Ԃ�
}

// =================================================================
// SodatePokeLevelSetSub
// �T�v  : ��ĂĂ�|�P�����̖��O�ƃ��x�����̕�������Z�b�g
// ����  : sodateya
//       : no
// �߂�l: none
// =================================================================
u8 SodatePokeLevelSetSub(SODATEYA_WORK *sodateya, u8 no)
{
	u8 sa;
	
	sa = GetSodatePokeLevel(sodateya,no);				//��Ă����x�������Z�b�g
	PokePasoParaNickNameCopy(&sodateya->Pokemon[no],StrTempBuffer0);
	PM_NumMsgSet(StrTempBuffer1, sa, NUM_MODE_LEFT, 2);

	return sa;
}

// =================================================================
// PokeHikitoriRyoukinSub
// �T�v  : PokeHikitoriRyoukin�̃T�u���[�`��
// ����  : none
// �߂�l: u16 ��Ă�����
// =================================================================
u16 PokeHikitoriRyoukinSub(SODATEYA_WORK *sodateya, u8 no)
{
	u16 sa;
	sa = GetSodatePokeLevel(sodateya,no);				//��Ă����x�������Z�b�g
	PokePasoParaNickNameCopy(&sodateya->Pokemon[no],StrTempBuffer0);
	sa = sa*100+100;
	PM_NumMsgSet(StrTempBuffer1, sa, NUM_MODE_LEFT, 5);

	return sa;
	
}

// =================================================================
// PokeHikitoriRyoukin
// �T�v  : ��ĂĂ�|�P�����Ɨ����𕶎���ŃZ�b�g����
// ����  : none(������PARAM0������)
// �߂�l: ����
// =================================================================
void PokeHikitoriRyoukin()
{
	ScriptParameter1 = PokeHikitoriRyoukinSub(&Fld.SodateyaWork,ScriptParameter0);
}

// =================================================================
// void	SodateyaPlusWalk
// �T�v  : �w��������������Ƃɂ���
// ����  : count
// �߂�l: none
// =================================================================
void	SodateyaPlusWalk(u16 count)
{
	Fld.SodateyaWork.WalkCount[0]+=count;
	Fld.SodateyaWork.WalkCount[1]+=count;
}

// =================================================================
// SodatePokeLevelSet
// �T�v  : �|�P�����̖��O�ƃ��x���̕�������Z�b�g����
// ����  : none(������PARAM0������)
// �߂�l: none
// =================================================================
u8 SodatePokeLevelSet()
{
	if(PokePasoParaGet(&Fld.SodateyaWork.Pokemon[ScriptParameter0],ID_monsno)){
		return SodatePokeLevelSetSub(&Fld.SodateyaWork,ScriptParameter0);
	}
	return 0;
}



// =================================================================
// SioMailDataInit
// �T�v  : �ʐM���[���\���̏�����
// ����  : smd
// �߂�l: none
// =================================================================
void SioMailDataInit(SIO_MAIL_DATA	*smd)
{
	int i;
	
	for(i=0;i<PERSON_NAME_SIZE+EOM_SIZE;i++) 	
		smd->oyaname[i] = 0;						//�e�̖��O������
	for(i=0;i<MONS_NAME_SIZE+EOM_SIZE;i++) 		
		smd->pokename[i] = 0;						//�|�P�����̖��O������

	MailDataInit(&smd->Mail);						//���[���f�[�^


}




// =================================================================
// PokeSodateyaInit
// �T�v  : ��ĉ����[�N������
// ����  : none
// �߂�l: none
// =================================================================
void PokeSodateyaInit(SODATEYA_WORK *sodateya)
{
	u8 i;

	for(i=0;i<2;i++){			
		PokePasoParaInit(&sodateya->Pokemon[i]);
		sodateya->WalkCount[i] = 0;
		SioMailDataInit(&sodateya->SioMail[i]);
	}
	sodateya->Egg = 0;
	sodateya->EggCount = 0;
}



// =================================================================
// PokeSearchChild
// �T�v  : �n�����|�P�����h�c�̑匳�̎q����Ԃ��܂�
// ����  : pokeid	�|�P�����̂h�c
// �߂�l: u16		�q���̂h�c�i�q���������ꍇ�͂��̂܂܈�����Ԃ��܂��j
// =================================================================
u16 PokeSearchChild(u16 pokeid)
{
	int i,r,w,flg;

	for(i=0;i<5;i++){					//�ō�5��T�[�`(�ݸޑ΍�)
		flg=0;
		for(r=1;r<KAIHATSU_NUM_MAX+1;r++){		//�|�P�����S�T�[�`
			for(w=0;w<5;w++){			//�i�����@5��ނ��T�[�`
				if(PSD[r][w].ShinkaMons==pokeid){
					pokeid = r;
					flg=1;
					break;
				}
			}
			if(flg) break;
		}
		if(r==(KAIHATSU_NUM_MAX+1)) break;		//�S���T���Ă������ɐi������|�P�����͂��Ȃ������B
	}
	
	return pokeid;
}


// =================================================================
// PokeMakeTamago
// �T�v  : �^�}�S�����
// ����  : sodateya
// �߂�l: none
// =================================================================
void PokeMakeTamago(SODATEYA_WORK *sodateya)
{

	sodateya->Egg = (pp_rand()%(RAND_MAX-1))+1;		//�^�}�S�����Z�b�g
	EventFlagSet(FE_SODATE_TAMAGO);					//�����������ړ��t���Oset
	

	//��e�Ɠ����|�P����
	//���x���̓p�[�\�i���̐��܂�Ă��郌�x��
	//�p���[������6���ڒ��A�����_����3���ڂ𕃐e����e�Ɠ������l�ɂ��܂�
	//����\�͕͂�e�Ɠ���
	//�Z�͒ʏ�Ƀf�[�^�����A���ꂩ��p���Z�������Ă����܂�
	//�^�}�S�r�b�g�𗧂Ă܂��B
	//�^�}�S���z��܂ł̕������Ȃ��x�̏ꏊ�ɂ���܂�
}

void PokeMakeNidoranOsuTamago(SODATEYA_WORK *sodateya)
{
	sodateya->Egg = pp_rand() | 0x8000;
	EventFlagSet(FE_SODATE_TAMAGO);					//�����������ړ��t���Oset
}


// =================================================================
// PokeTamagoSet
// �T�v  : �^�}�S���Z�b�g����
// ����  : none
// �߂�l: none
// =================================================================
void PokeTamagoSet()
{
	PokeMakeTamago(&Fld.SodateyaWork);
}

void PokeNidoranOsuSet()
{
	PokeMakeNidoranOsuTamago(&Fld.SodateyaWork);
}


#define BOX_MAX	6

// =================================================================
// SelectBoxTumeru
// �T�v  : �����l�߂�i���I�p�j
// ����  : box
//       : num
// �߂�l: static u8
// =================================================================
static void SelectBoxTumeru(u8 *box, u8 num)
{
	int i,w;
	u8  work[6];
	
	box[num] = 0xff;				//�w�肳�ꂽ�a�n�w���e�e�Ŗ��߂�
	
	for(i=0;i<BOX_MAX;i++){			//���̂܂܃R�s�[
		work[i] = box[i];
	}
	w = 0;
	for(i=0;i<BOX_MAX;i++){			//0xff�������Ă��Ȃ��������l�߂ē���Ȃ���
		if(work[i]!=0xff){
			box[w++] = work[i];
		}
	}
}

// =================================================================
// PokeChildRandSet
// �T�v  : �q����p�̗������Z�b�g
// ����  : p
//       : sodateya
// �߂�l: none
// =================================================================
static void PokeChildRandSet(PokemonParam *p,SODATEYA_WORK *sodateya)
{
	u8 select[3],i,box[6],oya[3],work;

	for(i=0;i<6;i++) box[i] = i;			//�O�`�T�̔���p��

	for(i=0;i<3;i++){						
		select[i] = box[pp_rand()%(6-i)];			//�O�`�T�̂����R�I�ԁi�ǂ̃p���[������ύX���邩�j
		SelectBoxTumeru(box,select[i]);
	}
	
	for(i=0;i<3;i++) oya[i] = pp_rand()%2;	//3�O���P�̗�����I�ԁi���E��ǂ��炩�痐�������o�����j

	for(i=0;i<3;i++){						//���I�̌��ʂ���R�p���[������e����R�s�[����
		switch(select[i]){
			case 0:
				work = PokePasoParaGet(&sodateya->Pokemon[oya[i]],ID_hp_rnd);
				PokeParaPut(p,ID_hp_rnd,(u8*)&work);
				break;
			case 1:
				work = PokePasoParaGet(&sodateya->Pokemon[oya[i]],ID_pow_rnd);
				PokeParaPut(p,ID_pow_rnd,(u8*)&work);
				break;
			case 2:
				work = PokePasoParaGet(&sodateya->Pokemon[oya[i]],ID_def_rnd);
				PokeParaPut(p,ID_def_rnd,(u8*)&work);
				break;
			case 3:
				work = PokePasoParaGet(&sodateya->Pokemon[oya[i]],ID_agi_rnd);
				PokeParaPut(p,ID_agi_rnd,(u8*)&work);
				break;
			case 4:
				work = PokePasoParaGet(&sodateya->Pokemon[oya[i]],ID_spepow_rnd);
				PokeParaPut(p,ID_spepow_rnd,(u8*)&work);
				break;
			case 5:
				work = PokePasoParaGet(&sodateya->Pokemon[oya[i]],ID_spedef_rnd);
				PokeParaPut(p,ID_spedef_rnd,(u8*)&work);
				break;
		}
	}
}

// =================================================================
// KowazaListGet
// �T�v  : �p���Z�e�[�u�����擾����
// ����  : p
//       : list
// �߂�l: none
// =================================================================
static u8 KowazaListGet(PokemonParam *p, u16 *list)
{
	u16 monsno,startno,num,i;

	num = 0;
	startno = 0;
		
	monsno = PokeParaGet(p,ID_monsno);		//������monsno���擾
	for(i=0;i<KOWAZA_TABLE_MAX;i++){			//�q�Z�e�[�u���̎����̏ꏊ���T�[�`
		if(KowazaTable[i]==(20000+monsno)){
			startno = i+1;
			break;
		}
	}
	
	for(i=0;i<10;i++){							//�ō��W�����Ȃ��̂ŁA�ꉞ�������P�O�܂�
		if(KowazaTable[startno+i]>20000){		//���̃|�P�����̐擪��������I��
			break;
		}else{
			list[i] = KowazaTable[startno+i];	//�Z���X�g��o�^
			num++;
		}
	}

	return num;
}

static	u16 wazalist[50];
static	u16 dadlist[4];
static	u16 commonlist[4];
static	u16 kowazalist[10];
static	u16 mamlist[4];
// =================================================================
// KowazaSet
// �T�v  : �p���Z���Z�b�g
// ����  : p		:�^�}�S�̃|�C���^
//       : dad		:���|�P����
//       : mam		:��|�P����
// �߂�l: none
// =================================================================
static void KowazaSet(PokemonParam *p, PokemonPasoParam *dad,PokemonPasoParam *mam)
{
	u16 i,w,n,monsno,levelwazanum,kowazanum;
//	PokemonParam wp;

	n=0;
	for(i=0;i<4;i++){
		mamlist[i]    = 0;
		dadlist[i]    = 0;
		commonlist[i] = 0;
	}
	for(i=0;i<10;i++) kowazalist[i] = 0;
	for(i=0;i<50;i++) wazalist[i]   = 0;

	
	monsno = PokeParaGet(p,ID_monsno);
//	PokeParaSet(&wp,monsno,100,POW_RND,RND_NO_SET,0,ID_NO_SET,0);
	levelwazanum = PokeOboeruWazaAllGet(monsno,wazalist);		//���x���A�b�v�Ŋo����Z�����ׂă��X�g�A�b�v

	for(i=0;i<4;i++){							//�e�̎����Ă���Z���擾
		dadlist[i] = PokePasoParaGet(dad,ID_waza1+i);
		mamlist[i] = PokePasoParaGet(mam,ID_waza1+i);
	}

	kowazanum = KowazaListGet(p,kowazalist);				//�p���Z���X�g���擾
	
	//�p���Z�v�Z
	for(i=0;i<4;i++){										//���e�̎����Ă���Z�Ǝq�̌p���Z���r
		if(dadlist[i]!=0){							
			for(w=0;w<kowazanum;w++){								//�q�Z�̐�����r
				if(dadlist[i]==kowazalist[w]){
					if(PokeWazaSet(p,dadlist[i])==NO_WAZA_SET){		//�Z���o���邩
						PokeWazaOboeOshidashi(p,dadlist[i]);	//�Z�������ς��ł�������艟������
					}
					break;
				}
			}
		}else{
			break;
		}
	}
	
	//���e�̊o���Ă���Z���q�����Z�}�V���Ŋo������Z�̏ꍇ
	for(i=0;i<4;i++){
		if(dadlist[i]!=0){											//���e�̎����Ă���Z��
			for(w=0;w<WAZA_MACHINE_MAX;w++){							//�Z�}�V���ɂ���A
				if(dadlist[i]==GetWazaNoItem(ITEM_WAZAMASIN01+w)){		//�q���͋Z�}�V���ł��邱�Ƃ��ł���Ȃ�
					if(PokeWazaMachineCheck(p,w)){		//�Z���o���邩
						if(PokeWazaSet(p,dadlist[i])==NO_WAZA_SET){		//�Z���o����
							PokeWazaOboeOshidashi(p,dadlist[i]);		//�Z�������ς��ł�������艟������
						}
					}
				}
			}
		}
	}


	//���E��̋��ʂ��Ă�Z�Ŏq�������x���A�b�v�o������Z���擾
	for(i=0;i<4;i++){												//���E��̋��ʂ̋Z���擾
		if(dadlist[i]==0) break;
		for(w=0;w<4;w++){
			if(dadlist[i]==mamlist[w] && dadlist[i]!=0){
				commonlist[n++] = dadlist[i];
			}
		}
	}
	
	for(i=0;i<4;i++){												//���ʋZ�̌p��
		if(commonlist[i]==0) break;										//����̋��ʋZ��
		for(w=0;w<levelwazanum;w++){
			if(wazalist[w]!=0){											//�q���̃��x���A�b�v�Ŋo����Z��
				if(commonlist[i] == wazalist[w]){						//�����Ȃ�
					if(PokeWazaSet(p,commonlist[i])==NO_WAZA_SET){		//�Z���o���邩
						PokeWazaOboeOshidashi(p,commonlist[i]);			//�Z�������ς��ł�������艟������
					}
					break;
				}
			}
		}
	
	}
}


// =================================================================
// DelSodateyaTamago
// �T�v  : �^�}�S������
// ����  : none
// �߂�l: none
// =================================================================
void DelSodateyaTamagoSub(SODATEYA_WORK *sodateya)
{
	//�^�}�S�����̌�n��
	sodateya->Egg = 0;												//�^�}�S�������[�N������
	sodateya->EggCount=0;
}

void DelSodateyaTamago(void)
{
	DelSodateyaTamagoSub(&Fld.SodateyaWork);
}


// =================================================================
// RuririSoonanoCheck
// �T�v  : �������ƃ\�[�i�m�̓��ʏ���
// ����  : no      :���܂��^�}�S�̃|�P�����i���o�[
//       : sodateya:��ĉ����[�N
// �߂�l: static
// =================================================================
static void RuririSoonanoCheck(u16 *no,SODATEYA_WORK *sodateya)
{
	u16 flag,item1,item2;
	flag = 0;
	if(*no != MONSNO_SOONANO && *no != MONSNO_RURIRI){		//���܂��|�P�������\�[�i�m��������
		return;												//�������I��
	}
		
	item1 = PokePasoParaGet(&sodateya->Pokemon[0],ID_item);		//�a���Ă���|�P�����̃A�C�e�����m�F
	item2 = PokePasoParaGet(&sodateya->Pokemon[1],ID_item);

	if(*no == MONSNO_SOONANO){								//�\�[�i�m�ɂ̂񂫂̂��������Ȃ�������
		if(item1 != ITEM_NONKINOOKOU && item2 != ITEM_NONKINOOKOU){
			*no = MONSNO_SOONANSU;							//�\�[�i���X��
		}
	}
	if(*no == MONSNO_RURIRI){								//�������ɂ������̂��������Ȃ�������
		if(item1 != ITEM_USIONOOKOU && item2 != ITEM_USIONOOKOU){
			*no = MONSNO_MARIRU;							//�}������
		}
	}
}






// =================================================================
// PokemonBotaiCheck
// �T�v  : �^�}�S�ɂ͉�������邩
// ����  : sodateya
//       : oya[]		oya[0]�ɕ�e,oya[1]�ɕ��e���i�[
// �߂�l: static u16
// =================================================================
static u16 PokemonBotaiCheck(SODATEYA_WORK *sodateya, u8 oya[])
{
	u16 oyamonsno[2],i,flag,monsno,tamagono,temp;

	flag = 0;
	for(i=0;i<2;i++){
		if((oyamonsno[i]=PokePasoParaGet(&sodateya->Pokemon[i],ID_monsno))==METAMON_NO){	//���^�����`�F�b�N
			oya[0] = i^1;
			oya[1] = i;
		}else if(PokePasoSexGet(&sodateya->Pokemon[i])==MONS_FEMALE){		//��e�̃|�P������T��
			oya[0] = i;
			oya[1] = i^1;
		}
	}

	monsno   = oyamonsno[oya[0]];				//��e�����ɂȂ�
	tamagono = PokeSearchChild(monsno);			//�q����{��

	if(tamagono==MONSNO_NIDORAN_F){								//�j�h�����v�Z
		if(sodateya->Egg & 0x8000){				//�^�}�S�����̍ŏ�ʃr�b�g�������Ă���@�j�h������
			tamagono = MONSNO_NIDORAN_M;
		}else{									//�t�̓j�h������
			tamagono = MONSNO_NIDORAN_F;
		}
	}
	if(tamagono == MONSNO_IRUMIIZE){			//�o���r�[�g�E�C���~�[�[�v�Z
		if(sodateya->Egg & 0x8000){
			tamagono = MONSNO_BARUBIITO;
		}else{
			tamagono = MONSNO_IRUMIIZE;
		}
	}
	
	//���^�����̔��Α����e�Ƃ��Ďq������������A���ۂ́��␫�ʂȂ��̎��͂��̌�
	//���e�Ƃ��Ĉ���Ȃ��Ƃ����Ȃ����ɕ���𔽓]������
	if(oyamonsno[oya[1]]==METAMON_NO && PokePasoSexGet(&sodateya->Pokemon[oya[0]])!=MONS_FEMALE){
		temp   = oya[1];
		oya[1] = oya[0];
		oya[0] = temp;
	}
	
	return tamagono;
//	PokePasoParaGet(&sodateya->Pokemon[oya[0]],ID_monsno);	//��e�����ɂ���

}


// =================================================================
// PokeTamagoDelSub
// �T�v  :	�^�}�S��Ⴄ=���܂��|�P���������
// ����  : sodateya
// �߂�l: none
// =================================================================
static void PokeTamagoDelSub(SODATEYA_WORK *sodateya)
{
	u16 tamagono;
	PokemonParam poke;
	u8 oya[2],flag;

	tamagono   = PokemonBotaiCheck(sodateya,oya);					//��̂Ƃ���|�P������T��
//	tamagono =  PokeSearchChild(monsno);							//�q���|�P������{��

	RuririSoonanoCheck(&tamagono,sodateya);									//�}�����ƃ\�[�i���X�̓��ʏ���

	PokemonTamagoSet2(&poke,tamagono, sodateya);							//�^�}�S���쐬
	PokeChildRandSet(&poke,sodateya);										//�q����p�̗����Z�b�g
	KowazaSet(&poke,&sodateya->Pokemon[oya[1]],&sodateya->Pokemon[oya[0]]);	//�p���Z���Z�b�g

	flag = 1;
	PokeParaPut(&poke,ID_tamago_flag,(u8*)&flag);					//�^�}�S�t���O�Z�b�g

	PokeParaMine[5] = poke;											//�^�}�S�Z�b�g
	BoxTumeMineData();												//�莝�����l�߂�
	PokeCountMineGet();												//�|�P�����̐��Ċm�F


	DelSodateyaTamagoSub(sodateya);
}


	
// =================================================================
// TamagoFlagsSet
// �T�v  : �^�}�S�����(�C�x���g�p�B��ĉ��ł͎g���܂���)
// ����  : poke			:�i�[����ꏊ�̃|�C���^
//       : monsno		:�|�P�����̔ԍ��i�J���i���o�[�j
//		 : flag			:�Q�[�����C�x���g�ł���������H�i�͂����P�j
// �߂�l: none
// =================================================================
void PokemonTamagoSet(PokemonParam *poke, u16 monsno, u8 eventflag)
{
	u8  level,event,flag;
	u16 ball;
	
	PokeParaSet(poke,monsno,BIRTH_LEVEL,POW_RND,RND_NO_SET,0,ID_NO_SET,0);	//���x���T�̃|�P����

	level = 0;
	ball  = ITEM_MONSUTAABOORU;
	PokeParaPut(poke,ID_get_ball,(u8*)&ball);						//�擾�{�[���������X�^�[�{�[����
	PokeParaPut(poke,ID_nickname,(u8*)tamago_name);				//���O�Ɂu�^�}�S�v�Z�b�g
	PokeParaPut(poke,ID_friend,(u8*)&PPD[monsno].egg_birth);		//�z���܂ŃJ�E���^���Z�b�g
	PokeParaPut(poke,ID_get_level,(u8*)&level);					//�^�}�S�͎擾�����x����0��

	
	if(eventflag){													//�^�}�S�����C�x���g�̎���
		event = POS_EVENT_EGG;
		PokeParaPut(poke,ID_get_place,(u8*)&event);				//�ߊl�ꏊ���i�[
	}
	flag = 1;
	PokeParaPut(poke,ID_tamago_flag,(u8*)&flag);					//�^�}�S�t���O�Z�b�g
	
}

// =================================================================
// TamagoFlagsSet
// �T�v  : �^�}�S�����
// ����  : poke			:�i�[����ꏊ�̃|�C���^
//       : monsno		:�|�P�����̔ԍ��i�J���i���o�[�j
//		 : sodateya		:��ĉ����[�N
// �߂�l: none
// =================================================================
void PokemonTamagoSet2(PokemonParam *poke, u16 monsno, SODATEYA_WORK *sodateya)
{
	u8  level;
	u16 ball;
	u32 personal_rnd;
	
	personal_rnd = (sodateya->Egg) | (pp_rand()<<16);
	PokeParaSet(poke,monsno,BIRTH_LEVEL,POW_RND,RND_SET,personal_rnd,ID_NO_SET,0);	//���x���T�̃|�P����

	level = 0;
	ball  = ITEM_MONSUTAABOORU;
	PokeParaPut(poke,ID_get_ball,(u8*)&ball);						//�擾�{�[���������X�^�[�{�[����
	PokeParaPut(poke,ID_nickname,(u8*)tamago_name);				//���O�Ɂu�^�}�S�v�Z�b�g
	PokeParaPut(poke,ID_friend,(u8*)&PPD[monsno].egg_birth);		//�z���܂ŃJ�E���^���Z�b�g
	PokeParaPut(poke,ID_get_level,(u8*)&level);					//�^�}�S�͎擾�����x����0��
	
}

// =================================================================
// PokeTamagoDel
// �T�v  : �^�}�S������&�^�}�S��Ⴄ
// ����  : none
// �߂�l: none
// =================================================================
void PokeTamagoDel()
{
	PokeTamagoDelSub(&Fld.SodateyaWork);
}


// =================================================================
// PokeSodateya
// �T�v  : ��ĉ�����	�i����������ɏ�������܂��j
// ����u8 EventMoveCheck( EV_POS * p, u16 attr, u8 site)�ɓ����
//
// ����  : none
// �߂�l: none
// =================================================================
u8 PokeSodateyaSub(SODATEYA_WORK *sodateya)
{
	u32 i,work,flag,affinity,lottery;

	flag = 0;
	for(i=0;i<2;i++){			
		if(PokePasoParaGet(&sodateya->Pokemon[i],ID_poke_exist_flag)!=0){	//�|�P�������a�����Ă�����
			sodateya->WalkCount[i]++;								//�����J�E���g++
			flag++;
		}
	}
	if(sodateya->Egg==0&&flag==2){									//���܂�������������
		if((sodateya->WalkCount[1]&0xff)==0xff){				
//		if((sodateya->WalkCount[1]&0x1)==0x1){				
			affinity = SodateyaPokeAffinitySub(sodateya);				//�����擾
			lottery  = pp_rand();
			lottery  = (lottery*100)/RAND_MAX;
			if(affinity>lottery){
				PokeTamagoSet();									//�^�}�S�����Z�b�g
			}
		}
	}


	
	if(++sodateya->EggCount==255){								//���܂��J�E���g�����܂킵�Ƃ�
//	if(++sodateya->EggCount==10){								//���܂��J�E���g�����܂킵�Ƃ�
		for(i=0;i<PokeCountMine;i++){
			if(PokeParaGet(&PokeParaMine[i],ID_tamago_flag)){	//���܂��������Ă��邩
//				if(pp_rand()>=RAND_MAX/2){							//���I�Ȃ��Ȃ�܂����i2002/08/21�j
					work=PokeParaGet(&PokeParaMine[i],ID_friend);
					if(work!=0){								//�z���E�F�C�g�����炷
						work--;
						//�f�N�������g�����Ȃ��x���Z�b�g���Ȃ���
						PokeParaPut(&PokeParaMine[i],ID_friend,(u8*)&work);
					}else{										//�^�}�S���z��
						ScriptParameter0 = i;
						return TRUE;
					}
//				}
			}
		}
//		sodateya->EggCount =0;
	}
	return FALSE;
}

// =================================================================
// u8	PokeSodateya
// �T�v  : ����������ƂɌo���l�����Z�iev_check.c�ɌĂ΂�܂��j
// ����  : none
// �߂�l: none
// =================================================================
u8	PokeSodateya()
{
	return PokeSodateyaSub(&Fld.SodateyaWork);
}


// =================================================================
// PokemonTamagoCheck
// �T�v  : �^�}�S�����邩�ǂ���
// ����  : none
// �߂�l: u8		������P	���Ȃ�������O
// =================================================================
u8 PokeTamagoCheck(SODATEYA_WORK *sodateya)
{
	return ((sodateya->Egg)!=0);
}


// =================================================================
// SodateyaNameSet
// �T�v  : ��������̌��t�p�ɕ�����𖄂߂���
// ����  : sodateya
// �߂�l: none
// =================================================================
static void SodateyaNameSet(SODATEYA_WORK *sodateya)
{
	u8 strbuf[MONS_NAME_SIZE+EOM_SIZE];
	if(PokePasoParaGet(&sodateya->Pokemon[0],ID_monsno)!=0){			//��Ԗڂ̃|�P�����̖��O
		PokePasoParaNickNameCopy(&sodateya->Pokemon[0],StrTempBuffer0);
//		NickNameCopy( StrTempBuffer0, strbuf );
//		PM_strcpy( StrTempBuffer0, strbuf );

		PokePasoParaGet(&sodateya->Pokemon[0],ID_oyaname,strbuf);		//�e�̖��O
		PM_strcpy( StrTempBuffer2, strbuf );

	}
	if(PokePasoParaGet(&sodateya->Pokemon[1],ID_monsno)!=0){			//��Ԗڂ̃|�P�����̖��O
		PokePasoParaNickNameCopy(&sodateya->Pokemon[1],StrTempBuffer1);
//		NickNameCopy( StrTempBuffer1, strbuf );
//		PM_strcpy( StrTempBuffer1, strbuf );
	}

}

// =================================================================
// void	SodateyaAzukeName
// �T�v  : �|�P�������X�g�őI�������|�P�����̖��O���Z�b�g����
// ����  : none
// �߂�l: monsno��Ԃ�
// =================================================================
u16	SodateyaAzukeName()
{
	PokePasoParaNickNameCopy(&(PokeParaMine[FPokeNo].PPP),StrTempBuffer0);
	return PokePasoParaGet(&(PokeParaMine[FPokeNo].PPP),ID_monsno);
}

// =================================================================
// SodateyaPokeNameSet
// �T�v  : �Q�̗a���Ă�Ƃ��̂�������̌��t�p�̏���
// ����  : none
// �߂�l: none
// =================================================================
void SodateyaPokeNameSet(void)
{
	SodateyaNameSet(&Fld.SodateyaWork);
}



// =================================================================
// SodateyaZiisannCheck
// �T�v  : ��������ɘb���������Ƃ��Ƀ`�F�b�N
// ����  : none
// �߂�l: u8	0:�Ȃ�	1:�^�}�S����	2:1�C�a���Ă�	3:2�C�a���Ă�
// =================================================================
u8 SodateyaZiisannCheck()
{
	u8 num;
 	if(PokeTamagoCheck(&Fld.SodateyaWork)){								//�^�}�S�`�F�b�N
		return 1;
	}
	if((num=PokeAzukeruCheck(&Fld.SodateyaWork))){						//�a���Ă邩�`�F�b�N
		return num+1;
	}
	return 0;													//����
}


// =================================================================
// TamagoGroupCheck
// �T�v  : �^�}�S�O���[�v���������̂����邩��r����
// ����  : p1
//       : p2
// �߂�l: static u8
// =================================================================
static u8 TamagoGroupCheck(u16 *p1, u16 *p2)
{
	int i,w;
	
	for(i=0;i<2;i++){
		for(w=0;w<2;w++){
			if(p1[i]==p2[w]) return 1;
		}
	}
	return 0;
}

#define NO_TAMAGO_GROUP		(15)
#define METAMON_GROUP		(13)

// =================================================================
// SodateyaPokeAffinitySub
// �T�v  : �|�P�������Q�̂���Ƃ��ɑ�����Ԃ�
// ����  : sodateya
// �߂�l: u8
// =================================================================
static  u8 SodateyaPokeAffinitySub(SODATEYA_WORK	*sodateya)
{
	u16 group[2][2],monsno[2];
	u32	id[2],sex[2],rnd,i;

	for(i=0;i<2;i++){
		monsno[i] = PokePasoParaGet(&sodateya->Pokemon[i],ID_monsno);	//�����X�^�[�m�n
		id[i]     = PokePasoParaGet(&sodateya->Pokemon[i],ID_id_no);	//�h�c�@�m�n

		rnd    = PokePasoParaGet(&sodateya->Pokemon[i],ID_personal_rnd);	//������
		sex[i] = PokeSexGetMonsNo(monsno[i],rnd);							//���ʎ擾
		group[i][0] = PPD[monsno[i]].egg_group1;						//�^�}�S�O���[�v�P
		group[i][1] = PPD[monsno[i]].egg_group2;						//�^�}�S�O���[�v�Q
	}

	
	if(group[0][0]==NO_TAMAGO_GROUP||group[1][0]==NO_TAMAGO_GROUP){	//�����B�O���[�v�Ƃ͐��܂�Ȃ�
		return 0;													
	}
	if(group[0][0]==METAMON_GROUP && group[1][0]==METAMON_GROUP)	//���^�����ƃ��^��������͐��܂�Ȃ�
		return 0;
	
	if(group[0][0]==METAMON_GROUP||group[1][0]==METAMON_GROUP){		//���^�������Е��ɂ���
		if(id[0]==id[1]) 	return 20;								//�h�c�������Ȃ�Q�O��
		else				return 50;								//�Ⴄ�Ȃ�T�O��
	}
	if(sex[0]==sex[1])	return 0;						//�������ʂ͑����[��
	if(sex[0]==MONS_UNKNOWN || sex[1]==MONS_UNKNOWN)	//���^�����`�F�b�N��͐��ʖ����͑����[��
		return 0;

	if(TamagoGroupCheck(group[0],group[1])==0) return 0;//�O���[�v��������瑊���[��

	
	if(monsno[0]==monsno[1]){							//�|�P�����̎�ނ�����
		if(id[0]!=id[1])	return 70;					//�h�c���Ⴄ
		else				return 50;
	}else{												//�|�P�����̎�ނ��Ⴄ
		if(id[0]!=id[1])	return 50;					//�h�c���Ⴄ
		else				return 20;
		
	}
	return 0;
}


// =================================================================
// SodateyaPokeAffinity
// �T�v  : �Q�̂̑�����Ԃ�
// ����  : none
// �߂�l: u8	�����x = 0-70(%)
// =================================================================
u8 SodateyaPokeAffinity()
{
	return SodateyaPokeAffinitySub(&Fld.SodateyaWork);
}

// =================================================================
// SodateyaPokeAffinityMsgSet
// �T�v  : �Q�����̑����ɂ���Ă�������̕Ԃ����t���Z�b�g����
// ����  : none
// �߂�l: u8
// =================================================================
void SodateyaPokeAffinityMsgSet()
{
	u8 work,result;
	work = SodateyaPokeAffinity();				//��̂̑������擾
	result = 0;
	if(work==0)		result=3;	//�ׂׂɂ�����ǂ邪�Ȃ�			
	if(work==20) 	result=2;	//����قǂ悭�Ȃ����Ȃ�
	if(work==50)	result=1;	//�܂��܂��̂悤����
	if(work==70)  	result=0;	//�Ƃ��Ă��悢�悤����
	
	PM_strcpy(MsgExpandBuffer,AffinityMsg[result]);	//�������Ƃ̕��͂��Z�b�g

}


// =================================================================
// SexSymbolCheck
// �T�v  : ������̂Ȃ��ɗv�����鐫�ʋL���͂�������
// ����  : buf
//       : sex
// �߂�l: static u8	1:������	0:�Ȃ������E�������͗���������
// =================================================================
static u8 SexSymbolCheck(u8 *buf, u8 sex)
{
	u8 i,check[2];
	check[0] = check[1] = 0;
	i = 0;
	
	while(buf[i]!=EOM_){
		if(buf[i]==osu_ ) check[0]++;	//���L��������
		if(buf[i]==mesu_) check[1]++;	//���L��������
		i++;
	}
	
	if(sex==MONS_MALE){
		if(check[0]!=0 && check[1]==0) return 1;
	}
	if(sex==MONS_FEMALE){
		if(check[1]!=0 && check[0]==0) return 1;
	}
	
	return 0;
}

// =================================================================
// PM_SexStrcat
// �T�v  : ������̍Ō�ɐ��ʋL����ǉ�����
//         ������������̒��ɐ��ʋL�������ɂȂ������`�F�b�N����
// ����  : buf
//       : sex
// �߂�l: u8 *
// =================================================================
u8 *PM_SexStrcat(u8 *buf,u8 sex)
{
	if(sex==MONS_MALE){								//��
		if(SexSymbolCheck(buf,MONS_MALE)==0){		//���Ɏg���Ă��Ȃ���
			return PM_strcat(buf,sex_msg[0]);
		}
	}else if(sex==MONS_FEMALE){						//��
		if(SexSymbolCheck(buf,MONS_FEMALE)==0){		//���Ɏg���Ă��Ȃ���
			return PM_strcat(buf,sex_msg[1]);		
		}
	}
	return PM_strcat(buf,sex_msg[2]);				//���ʖ����E�������͊��ɐ��ʂ���
	
}
// =================================================================
// PPP_SexStrcat
// �T�v  : ���ʂ̋L�������Ă�A�j�h�����΍��
// ����  : buf		:������o�b�t�@
//       : pokemon	:�|�P�����f�[�^�\����
// �߂�l: u8 *		:������̍Ō�̃|�C���^
// =================================================================
u8 *PPP_SexStrcat(u8 *buf, PokemonPasoParam *pokemon)
{
	u8 sex;
	//����
	sex=PokePasoSexGet(pokemon);						
	return PM_SexStrcat(buf, sex);
}


#define HIKITORI_X1	15								//�Ђ��Ƃ胊�X�g��X���W
#define HIKITORI_Y1	6								//�Ђ��Ƃ胊�X�g��Y���W
#define HIKITORI_W	14								//�Ђ��Ƃ胊�X�g�̕�
#define HIKITORI_H	7								//�Ђ��Ƃ胊�X�g�̍���
#define HIKITORI_X2	(HIKITORI_X1+HIKITORI_W)
#define HIKITORI_Y2	(HIKITORI_Y1+HIKITORI_H)
#define HIKITORI_NAME_LEN		(5+1+1)				//���O�T�����{���ʁ{��
#define HIKITORI_LEVEL_START	(HIKITORI_X1+HIKITORI_NAME_LEN+2)	//���x���\���̏ꏊ�̒�`


// =================================================================
// MakeSelectSodateList
// �T�v  : ��ĉ��ɂ������Ă���|�P�����Q�̖̂��O��\��
// ����  : sodateya
//       : buf
// �߂�l: none
// =================================================================
void MakeSelectSodateList(SODATEYA_WORK *sodateya,u8 *buf)
{
	u8 strbuf[2][20],i;
	
	buf[0] = EOM_;
	for(i=0;i<2;i++){
		//�a���Ă�|�P�����̖��O
		PokePasoParaNickNameCopy(&(sodateya->Pokemon[i]),strbuf[i]);
		
//		PM_strcat(buf,strbuf);			

		PPP_SexStrcat(strbuf[i],&sodateya->Pokemon[i]);

		//���s
//		PM_strcat(buf,cr_msg);
	}
	PM_strcpy(buf,strbuf[0]);
	PM_strcat(buf,cr_msg);
	PM_strcat(buf,strbuf[1]);
	PM_strcat(buf,cr_msg);
	PM_strcat(buf,yameru_msg);											//�u��߂�v

}

// =================================================================
// MakeSodateLevelList
// �T�v  : ���x���\���p�̕�������쐬
// ����  : sodateya
//       : buf
// �߂�l: none
// =================================================================
void MakeSodateLevelList(SODATEYA_WORK *sodateya,u8 *buf)
{
	u8 strbuf[20],i;
	u32 level;

	buf[0] = EOM_;
	for(i=0;i<2;i++){
		//���x���\��
		PM_strcat(buf,level_msg);
		level = GetSinkaPokeLevel(&sodateya->Pokemon[i], sodateya->WalkCount[i]);	
		PM_NumMsgSet(strbuf, level, NUM_MODE_LEFT, 3);
		PM_strcat(buf,strbuf);
		PM_strcat(buf,cr_msg);
	}
}
// =================================================================
// HikitoriCursor
// �T�v  : �I�����j���[�^�X�N
// ����  : no
// �߂�l: none
// =================================================================
static void HikitoriCursor(u8 no)
{
	if(sys.Trg&U_KEY){											//��ړ�
		if(TaskTable[no].work[0]){
			TaskTable[no].work[0]--;
			NMenuVCursorSet(-1);
			SePlay(SE_SELECT);
		}
	}else if(sys.Trg&D_KEY){									//���ړ�
		if(TaskTable[no].work[0]!=2){
			TaskTable[no].work[0]++;
			NMenuVCursorSet(1);
			SePlay(SE_SELECT);
		}
	}else if(sys.Trg&A_BUTTON){									//����
		SePlay(SE_SELECT);
		FPokeNo = AnswerWork = TaskTable[no].work[0];
		
		DelTask(no);					//�I���^�X�N�I��
		NMenuBoxClear(HIKITORI_X1, HIKITORI_Y1, HIKITORI_X2, HIKITORI_Y2);	//���X�g�N���A
		ContinueFieldScript();			//�X�N���v�g�ĊJ
	}else if(sys.Trg&B_BUTTON){									//�L�����Z��
		FPokeNo = AnswerWork = 2;
		DelTask(no);					//�I���^�X�N�I��
		NMenuBoxClear(HIKITORI_X1, HIKITORI_Y1, HIKITORI_X2, HIKITORI_Y2);	//���X�g�N���A
		ContinueFieldScript();			//�X�N���v�g�ĊJ
	}
}



// =================================================================
// SodateyaSelectPoke
// �T�v  : �Ђ��Ƃ�|�P��������̂���Ƃ��ɑI�����郁�j���[
// ����  : none
// �߂�l: none
// =================================================================
void SodateyaSelectPoke()
{
	u8 strbuf[100];
	NMenuWinBoxWrite(HIKITORI_X1, HIKITORI_Y1, HIKITORI_X2, HIKITORI_Y2);//�g�`��

	MakeSelectSodateList(&Fld.SodateyaWork,strbuf);								//���O���X�g�쐬
	NMenuMsgWrite(strbuf, HIKITORI_X1+2, HIKITORI_Y1+1);				//�\��

	MakeSodateLevelList(&Fld.SodateyaWork,strbuf);								//���x�����X�g�쐬
	NMenuMsgWrite(strbuf, HIKITORI_LEVEL_START, HIKITORI_Y1+1);			//�\��

	NMenuVCursorInit(HIKITORI_X1+1, HIKITORI_Y1+1, 3, 0);				//�J�[�\���\��
	AddTask(HikitoriCursor,3);											//�^�X�N�o�^
};

// =================================================================
// SodatePokeListCall
// �T�v  : �|�P�������X�g�̌Ăяo��
// ����  : none
// �߂�l: none
// =================================================================
void SodatePokeListCall()
{
	PokeListCall(GROW_POKELIST_ID,SINGLE_LIST);
	sys.pReturnProc = FieldMainRecover;
//	ContinueFieldScript();			//�X�N���v�g�ĊJ

}

// =================================================================
// Tamago2Pokemon
// �T�v  : �^�}�S���|�P�����ɖ߂�
//         ���l���������^�}�S�͂h�c�ƌ������̂�����
//         �^�}�S�t���O�𗎂Ƃ������ł͏����ł��Ȃ��̂�
//         �K�v�ȃp�����[�^���i�[�����|�P�������\�z���Ȃ������Ƃɂ���
// ����  : p
//       : temp
// �߂�l: none
// =================================================================
static void Tamago2Pokemon(PokemonParam *p, PokemonParam *temp)
{
	//���l����^�}�S���������A�^�}�S����z�������|�P�����͑O�̐l�̂��̂ł͂Ȃ�
	//�z���������l�̂h�c�ɂȂ�B����ɑΉ����邽�߂Ɉ�U�Í�������Ă���
	//�f�[�^�����ׂēW�J���Ċi�[���Ȃ������Ƃɂ���
	
	//monsno,level,waza1,waza2,waza3,waza4,personal_rnd,pow_rnd,hp_rnd,agi_rnd,def_rnd,spepow_rnd,
	//spedef_rnd,oyaname
	u16 monsno,waza[4];
	u32 personal_rnd,rnds[6],pokerus;
	u8  i,country_code,get_cassette,mark,friend;

	monsno = PokeParaGet(p,ID_monsno);						//�|�P�����i���o�[
	for(i=0;i<4;i++)
		waza[i] = PokeParaGet(p,ID_waza1+i);				//�Z�擾
	personal_rnd = PokeParaGet(p,ID_personal_rnd);			//�������擾
	for(i=0;i<6;i++)
		rnds[i] = PokeParaGet(p,ID_hp_rnd+i);				//�U�̗����擾
	country_code = PokeParaGet(p,ID_country_code);			//���R�[�h
	get_cassette = PokeParaGet(p,ID_get_cassette);			//�ߊl�J�Z�b�g
	mark         = PokeParaGet(p,ID_mark);					//�}�[�N
	pokerus      = PokeParaGet(p,ID_pokerus);				//�|�P���X���

	//��������|�P�����쐬

	PokeParaSet(temp,monsno,BIRTH_LEVEL,POW_RND,RND_SET,personal_rnd,ID_NO_SET,0);	//���x���T�̃|�P����

	for(i=0;i<4;i++)
		PokeParaPut(temp,ID_waza1+i,(u8*)&(waza[i]));		//�Z�i�[
	for(i=0;i<6;i++)
		PokeParaPut(temp,ID_hp_rnd+i,(u8*)&(rnds[i]));		//�U�̗����i�[
	PokeParaPut(temp,ID_country_code,(u8*)&country_code);	//���R�[�h
    PokeParaPut(temp,ID_get_cassette,(u8*)&get_cassette);	//�ߊl�J�Z�b�g
	PokeParaPut(temp,ID_mark,(u8*)&mark);					//�}�[�N
	friend = HUKA_FIRST_NATUKIDO;			
	PokeParaPut(temp,ID_friend,(u8*)&friend);				//�Ȃ��x��120��
	PokeParaPut(temp,ID_pokerus,(u8*)&pokerus);				//�|�P���X��Ԃ��Z�b�g

	*p = *temp;												//�f�[�^���R�s�[
}

// =================================================================
// SodateTamagoHuka
// �T�v  : �^�}�S���|�P�����ɂ�����
// ����  : no = �莝���m�n
// �߂�l: none
// =================================================================
static void SodateTamagoHukaSub(u8 no)
{
	u8 	work1,work2,mappos;
	u16	monsno,zukanno,ball,level;
	u8	namebuf[11];

	work1 = 0;
	work2 = 70;



	Tamago2Pokemon(&PokeParaMine[no],&PokeParaEnemy[0]);				//�^�}�S���|�P�����ɖ߂�



//	PokeParaPut(&PokeParaMine[no],ID_tamago_flag,(u8*)&work1);			//�^�}�S�t���OOFF
	PokeParaPut(&PokeParaMine[no],ID_tamago_flag,(u8*)&work2);			//�Ȃ��x�������l��

	monsno  = PokeParaGet(&PokeParaMine[no],ID_monsno);					//�|�P�����m�n���擾

	PokeNameSet(namebuf,monsno);					//���O�Z�b�g
	PokeParaPut(&PokeParaMine[no],ID_nickname,namebuf);
	
	zukanno = PokeMonsNo2ZukanNoGet(monsno);							//�}�ӂm�n�֕ϊ�
	ZukanCheck(zukanno,Z_SEE_SET);										//�}�ӌ����t���O�Z�b�g
	ZukanCheck(zukanno,Z_GET_SET);										//�}�ӃQ�b�g�t���O�Z�b�g

	PokeParaNickNameCopy(&PokeParaMine[no],StrTempBuffer0);

	ball  = ITEM_MONSUTAABOORU;
	PokeParaPut(&PokeParaMine[no],ID_get_ball,(u8*)&ball);				//�擾�{�[���������X�^�[�{�[����
	level = 0;
	PokeParaPut(&PokeParaMine[no],ID_get_level,(u8*)&level);			//�^�}�S�͎擾�����x����0��


	mappos = GetNowWorldPos();											//�z�������ꏊ��ݒ�
	PokeParaPut(&PokeParaMine[no],ID_get_place,&mappos);
	PokePPMaxSet(&PokeParaMine[no]);									//PP����


	PokeParaCalc(&PokeParaMine[no]);
}

// =================================================================
// SodateTamagoHuka
// �T�v  : �^�}�S����|�P�������z������
// ����  : none
// �߂�l: none
// =================================================================
void SodateTamagoHuka()
{
	SodateTamagoHukaSub(ScriptParameter0);
}


static u8 SodateyaMailCheckSub(SODATEYA_WORK *sodateya, u8 no)
{
	u8 pokename[30];

	PokePasoParaNickNameCopy(&sodateya->Pokemon[no],pokename);	//�|�P�����̖��O�擾
	if(sodateya->SioMail[no].Mail.design_no==NON_ITEM) return 0;		//���[���������Ȃ�I��
	
	if((PM_strcmp(pokename,sodateya->SioMail[no].pokename)!=0)||	//�|�P�����̖��O�����[���ƈႤ��
	(PM_strcmp(MyData.my_name,sodateya->SioMail[no].oyaname)!=0)){	//���[���������e�̖��O�Ǝ������Ⴄ�Ȃ�
		PM_strcpy(StrTempBuffer0,pokename);								
		PM_strcpy(StrTempBuffer1,sodateya->SioMail[no].oyaname);		
		PM_strcpy(StrTempBuffer2,sodateya->SioMail[no].pokename);		
		return 1;
	}
	return 0;
}
// =================================================================
// SodateyaMailCheck
// �T�v  : ���R�[�h�R�[�i�[�Ń��[������������Ă��Ȃ����`�F�b�N
// ����  : none�i������ScriptParameter0��0��1�������Ă���j
// �߂�l: u8 0=���[���͖����E���l�̃��[���͖���  1=���l����̃��[��������
// =================================================================
u8 SodateyaMailCheck()
{
	return SodateyaMailCheckSub(&Fld.SodateyaWork,ScriptParameter0);
}


#include "..\scaddata\tamago.acl"
#include "..\scaddata\tamagoan.ach"
#include "..\scaddata\tamago_o.ach"

//-----------------------------------------------------------
//    �^�}�S�z���f���p�A�N�^�[��`
//-----------------------------------------------------------

// --------------�^�}�S-----------------------------------
static void ActTamagoHukaMove(actWork *act);
static void ActTamagoHukaMove2(actWork *act);
static void ActTamagoHukaMove3(actWork *act);
static void ActTamagoHukaMove4(actWork *act);
static void ActTamagoHukaMove5(actWork *act);
static void ActTamagoHukaMove6(actWork *act);
static void TamagoPartsSet(u8 x, u8 y, s16 dx ,s16 dy, s16 gg,u8 n);
static void TamagoPartsActMove(actWork *act);
static void TamagoPartsPut();

//------------���---------------------

static const ActOamData TamagoOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		0,		//u32 Shape:1;            // �n�a�i�`��(�������`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		2,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x32  3����64x64)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		1,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};

static const actAnm TamagoAnm0[]={		{OBJ16_32x32*0,5,0,0},
										{ACT_ANMEND,0,0,0}
};
static const actAnm TamagoAnm1[]={		{OBJ16_32x32*1,5,0,0},
										{ACT_ANMEND,0,0,0}
};
static const actAnm TamagoAnm2[]={		{OBJ16_32x32*2,5,0,0},
										{ACT_ANMEND,0,0,0}
};
static const actAnm TamagoAnm3[]={		{OBJ16_32x32*3,5,0,0},
										{ACT_ANMEND,0,0,0}
};



static const actAnm *const TamagoAnmTbl[]={
		TamagoAnm0,
		TamagoAnm1,
		TamagoAnm2,
		TamagoAnm3,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

static const CellData TamagoActCell={
	tamagoan_Character,0x20*16*4,12345
};
static const CellData TamagoPartsActCell={
	tamago_o_Character,0x20*4,23456
};

static const PalData TamagoActPal = 
{
	tamago_Palette,
	54321,
};


//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------
static const	actHeader ActTamagoHeader={
		12345,
		54321,
		&TamagoOamData,
		TamagoAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		DummyActMove,
};

// --------------�^�}�S�̔j��------------------------------
static void TamagoPartsActMove(actWork *act);

static const ActOamData TamagoPartsOamData = {
		0,		//u32 VPos:8;             // �x���W
		0,		//u32 AffineMode:2;       // �A�t�B�����[�h
		0,		//u32 ObjMode:2;          // �n�a�i���[�h
		OFF,	//u32 Mosaic:1;           // ���U�C�N
		0,		//u32 ColorMode:1;        // �P�U�F/�Q�T�U�F �I��
		0,		//u32 Shape:1;            // �n�a�i�`��(�������`)

		0,		//u32 HPos:9;             // �w���W
		0,		//u32 AffineParamNo:5;  // �A�t�B���ϊ��p�����[�^No & H,VFlip
		0,		//u32 Size:2;             // �n�a�i�T�C�Y(2����32x32  3����64x64)

		0,		//u32 CharNo:10;          // �L�����N�^�m��
		2,      //u32 Priority:2;         // �\���D��
		0,		//u32 Pltt:4;             // �p���b�g�m��
		0,
};

static const actAnm TamagoPartsAnm0[]={		{OBJ16_8x8*0,5,0,0},
											{ACT_ANMEND,0,0,0}
};
static const actAnm TamagoPartsAnm1[]={		{OBJ16_8x8*1,5,0,0},
											{ACT_ANMEND,0,0,0}
};
static const actAnm TamagoPartsAnm2[]={		{OBJ16_8x8*2,5,0,0},
											{ACT_ANMEND,0,0,0}
};
static const actAnm TamagoPartsAnm3[]={		{OBJ16_8x8*3,5,0,0},
											{ACT_ANMEND,0,0,0}
};



static const actAnm *const TamagoPartsAnmTbl[]={
		TamagoPartsAnm0,
		TamagoPartsAnm1,
		TamagoPartsAnm2,
		TamagoPartsAnm3,
};


//�A�j���[�V�����f�[�^�t�H�[�}�b�g
/*
	s16 pat;	        //��׸���ް����ް
	u8 	wait:6;		//����
	u8 	hrev:1;		//H���]
	u8 	vrev:1;		//V���]
*/

//-------------------------------------------------------------
//		�A�N�^�[�w�b�_�[
//-------------------------------------------------------------
static const	actHeader ActTamagoPartsHeader={
		23456,
		54321,
		&TamagoPartsOamData,
		TamagoPartsAnmTbl,
		ACT_NTRANS,
		DummyActAffTbl,
		TamagoPartsActMove,
};



#define TYPE_BEFORE 0
#define TYPE_AFTER 1
#define PROCESS1	0
#define PROCESS2	1

#define BG2_SCR_NUM (8+4)

extern u16 koukan0_Palette[];
extern u8  koukanb1_Character[4864];
extern u16 koukan0_Map[2048];

typedef struct{
	u8	actno[2];
	u8  seqno;
	u8  wait;
	u8  tamagono;
	TALK_WIN 	tw;					//÷�ĺ�÷��
	u8  windowwork;
	u8  selectpos;
	u8	tamagoparts;
	u16 musicno;
}TAMAGODEMO_WORK;

TAMAGODEMO_WORK *tdw;
// =================================================================
// PokemonActorSetProcess
// �T�v  : �|�P�����̃A�N�^�[��o�^����i�����E����j
//         Decord�̏d�����l����2��ĂԎ��ŃA�N�^�[�o�^���I������
//
// ����  : type		TYPE_MINE(����)��TYPE_ENEMY(����)
//       : process	PROCESS1,PROCESS2(���̏��ɌĂ�)
// �߂�l: none
// =================================================================
static u8 PokemonActorSetProcess( u8 type , u8 process, u8 no)
{
	u16          monsno,clienttype;
	PokemonParam *pmp;
	PalData      *pd;
	u32          personal_rnd;
	u8           ret;
	
	clienttype   = 0;
	ret          = 0;
	personal_rnd = 0;
	pmp          = NULL;
	
	if(type == TYPE_BEFORE){					//������
		pmp        = &PokeParaMine[no];
		clienttype = BSS_CLIENT_ENEMY;
	}
	if (type == TYPE_AFTER){					//�z����������
		pmp        = &PokeParaMine[no];
		clienttype = BSS_CLIENT_ENEMY2;
	}

	switch(process){
		case PROCESS1:				//�P�p�X��
			monsno       = PokeParaGet(pmp,ID_monsno);			//�߹�݉摜�ԍ��擾
			personal_rnd = PokeParaGet(pmp,ID_personal_rnd);	//�������擾
			DecordPokeCellMakeBuchi(&PM2FObjDataCell[monsno],
				pm2fpara[monsno].size,pm2fpara[monsno].patcnt,&UserWork[0],(void*)DecordAdrs[type*2+1],monsno,personal_rnd);
			DecordPalSet((PalData*)PokePalDataGet(pmp));
			break;
		case PROCESS2:				//2�p�X��
			pd                  = (PalData*)PokePalDataGet(pmp);
			PokeActHeaderMake(pd->reg_num,clienttype);				//�|�P�����A�N�^�[��`����
			ret                 = AddActor(&PokeActHeader,120,70,6);//�|�P�������A�N�^�[�o�^
			ActWork[ret].banish = 1;								//�B��
			ActWork[ret].move   = DummyActMove;						//�������~�߂�
			
			break;
	}
	return ret;
}


/********************************************************************/
/*                                                                  */
/*				VSync���荞��										*/
/*                                                                  */
/********************************************************************/
static void VInterFunc( void )
{
	actOAMtrans();						//�����]������1
	actDMAtrans();						//�����]������2
	PaletteFadeTrans();
}


// =================================================================
// TamagoDemoCall
// �T�v  : �^�}�S�z���f���J�n�Ăяo��
// ����  : none
// �߂�l: none
// =================================================================
void TamagoHukaDemoMain();
void TamagoHukaDemoInit();
static void InitFadeWait( u8 id );
void TamagoDemoCall(void)
{
	u8	id;

	SetForceEvent();
	id = AddTask( InitFadeWait, 10 );
	PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
}



static void InitFadeWait( u8 id )
{
	if( !(FadeData.fade_sw) ){
		MainProcChange(TamagoHukaDemoInit);
		pFieldRecoverFunc = AddScriptFieldInTask;
//		FadeData.trans_stop = 1;
		DelTask(id);
	}
}

// =================================================================
// TamagoHukaDemoInit
// �T�v  : ������
// ����  : none
// �߂�l: none
// =================================================================
void TamagoHukaDemoInit()
{
	switch(sys.InitSeqWork){
		case 0:
			*(vu16*)REG_DISPCNT = 0;
			tdw = (TAMAGODEMO_WORK*)&UserWork[0x18000];
			tdw->tamagono = ScriptParameter0;
			tdw->tamagoparts = 0;
			InitTask();
			InitActor();				//�A�N�^�[������
			ObjPalManInit();			//OBJ�p���b�g�}�l�[�W���[������
			SetVBlankFunc(VInterFunc);
			sys.InitSeqWork++;
			ScriptParameter1 =  BGM_GetNowNo();
			break;
		case 1:
			PrintScreenInit(KOUKAN_TYPE);
			MsgControlInit(&(tdw->tw), KOUKAN_TYPE);
			tdw->windowwork = NWinFrameInit(20);
			NWinFrameDataSet(&(tdw->tw));

//			NMenuScreenClear();
//			PrintScreenInit(DEFAULT_TYPE);
//			NMenuScreenInit(DEFAULT_TYPE);
//			NMenuScreenClear();
			sys.InitSeqWork++;
			break;

		case 2:
			//�E�C���h�E�摜�]��
			DecordVram((void *)batt_bg1_pch_ADRS,(void *)BG_VRAM);
			DecordWram((void *)batt_bg1_psc_ADRS,(void *)UserWork);
			DmaCopy(3,UserWork,BG_VRAM+5*0x800,0x500,16);
			DecordPaletteWork((void *)batt_bg1_pcl_ADRS,PA_BG0,0x20);
			sys.InitSeqWork++;
			break;
		case 3:
			CellSet((CellData *)&TamagoActCell);
			CellSet((CellData *)&TamagoPartsActCell);
			ObjPalSet((PalData *)&TamagoActPal);
			sys.InitSeqWork++;
			break;
		case 4:
			tdw->actno[0] = AddActor(&ActTamagoHeader,120,75,5);
			SodateTamagoHukaSub(tdw->tamagono);
			sys.InitSeqWork++;
			break;
		case 5:
			PokemonActorSetProcess( TYPE_BEFORE , PROCESS1, tdw->tamagono);
			sys.InitSeqWork++;
			break;
		case 6:
			tdw->actno[1] = PokemonActorSetProcess( TYPE_BEFORE , PROCESS2, tdw->tamagono);
			sys.InitSeqWork++;
			break;
		case 7:
			*(vu16 *)REG_BG2CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_1				// �X�N���[���T�C�Y
						| BG_PRIORITY_2					//BG�D�揇��
						| BG2_SCR_NUM << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 1 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
			PaletteWorkSet(koukan0_Palette,16,5*32);
			DIV_DMAARRAYCOPY(3, koukanb1_Character, BG_VRAM+0x4000, 16);
			DIV_DMAARRAYCOPY(3, koukan0_Map, BG_VRAM+0x800*BG2_SCR_NUM, 16);
			sys.InitSeqWork++;
			break;
		
		case 8:
			*(vu16 *)REG_BG1CNT = BG_COLOR_16					//16�F�I��
						| BG_SCREEN_SIZE_0				// �X�N���[���T�C�Y
						| BG_PRIORITY_1					//BG�D�揇��
						| 5 << BG_SCREEN_BASE_SHIFT	//�X�N���[���x�[�X�u���b�N
						| 0 << BG_CHAR_BASE_SHIFT ;		//�L�����N�^�x�[�X�u���b�N
			*(vu16*)REG_BG0HOFS = 0;
			*(vu16*)REG_BG0VOFS = 0;
			*(vu16*)REG_BG1HOFS = 0;
			*(vu16*)REG_BG1VOFS = 0;
			*(vu16*)REG_BG2HOFS = 0;
			*(vu16*)REG_BG2VOFS = 0;
			MainProcChange(TamagoHukaDemoMain);
			tdw->seqno = 0;
			break;
	}
}

enum{
	TAMAGODEMO_FADEIN,
	TAMAGODEMO_FADEINWAIT,
	TAMAGODEMO_START,
	TAMAGODEMO_ANIME_WAIT,
	TAMAGODEMO_MES_SET,
	TAMAGODEMO_MES_WAIT,
	TAMAGODEMO_JINGLE_WAIT,
	TAMAGODEMO_MES_YESNO,
	TAMAGODEMO_MES_YESNO_WAIT,
	TAMAGODEMO_YESNO_WAIT,
	TAMAGODEMO_FADEOUT,
	TAMAGODEMO_FADEOUTWAIT,
};

static const u8 birth_mes[]={	//�^�}�S����H�H�H�����܂ꂽ�I
	CTRL_,C_FCOL_,0xf,CTRL_,C_BCOL_,0x0,CTRL_,C_SCOL_,1,
	TA_,MA_,GO_,ga_,spc_,ka_,e_,ttu_,te_,CR_,
	I_MSG_,STR_TEMP_BUF0,ga_,spc_,u_,ma_,re_,ta_,gyoe_,EOM_,
};

static const u8 rename_mes[]={	//���܂ꂽ�H�H�H�ɖ��O�����܂����H
	CTRL_,C_FCOL_,0xf,CTRL_,C_BCOL_,0x0,CTRL_,C_SCOL_,1,
	u_,ma_,re_,ta_,spc_,I_MSG_,STR_TEMP_BUF0,ni_,CR_,
	NI_,TTU_,KU_,NE_,bou_,MU_,wo_,spc_,tu_,ke_,ma_,su_,ka_,hate_,EOM_,
};
// =================================================================
// ReturnFieldTask
// �T�v  : 
// ����  : none
// �߂�l: none
// =================================================================
void ReturnFieldTask()
{
//	MusicPlay(ScriptParameter1);
	PokeParaPut(&PokeParaMine[ScriptParameter0],ID_nickname,StrTempBuffer2);
	MainProcChange(FieldMainRecover);
}

// =================================================================
// PlayShinkaMusic
// �T�v  : 
// ����  : no
// �߂�l: none
// =================================================================
void PlayShinkaMusic(u8 no)
{
	if(TaskTable[no].work[0]==0){
		BGM_PlayStop();
	}
	if(TaskTable[no].work[0]==1){
		MusicPlay(MUS_ME_SHINKA);
	}
	
	if(TaskTable[no].work[0] > 60){
//		if(SeEndCheck()==0){
			MusicPlay(MUS_SHINKA);
			DelTask(no);
//		}
	}
	++TaskTable[no].work[0];
}
// =================================================================
// TamagoHukaDemoMain
// �T�v  : �^�}�S���܂�f�����C��
// ����  : none
// �߂�l: none
// =================================================================
void TamagoHukaDemoMain()
{
	s8  select;
	u16 monsno,sex;

	switch(tdw->seqno){
		case TAMAGODEMO_FADEIN:
			PaletteFadeReq( 0xffffffff, 0, 16, 0, 0 );
			*(vu16 *)REG_DISPCNT = 	DISP_MODE_0					// BG���[�h 
    	              |	DISP_OBJ_CHAR_1D_MAP		// OBJ1����ϯ��ݸ�
					  | DISP_OBJ_ON
					  | DISP_BG0_ON					// BG0�L��
					  | DISP_BG1_ON					// BG1�L��
					  | DISP_BG2_ON					// BG1�L��
						;
			tdw->seqno++;
			AddTask(PlayShinkaMusic,5);
			break;
		case TAMAGODEMO_FADEINWAIT:
			if( !(FadeData.fade_sw) ){
				tdw->wait = 0;
				tdw->seqno++;
			}
			break;
		case TAMAGODEMO_START:
			if(++tdw->wait>30){
				tdw->seqno++;
				ActWork[tdw->actno[0]].move = ActTamagoHukaMove;
			}
			break;
		case TAMAGODEMO_ANIME_WAIT:
			if(ActWork[tdw->actno[0]].move == DummyActMove){
				tdw->seqno++;	
			}
			break;
		case TAMAGODEMO_MES_SET:
//			if(sys.Trg&B_BUTTON)
//				ActAffAnmChg(&ActWork[tdw->actno[1]],1);
//			if(sys.Trg&A_BUTTON)
//				tdw->seqno++;
			PokeParaNickNameCopy(&PokeParaMine[tdw->tamagono],StrTempBuffer0);
			PM_MsgExpand(MsgExpandBuffer,birth_mes);
//			TamagoDemoMsgTalkSet(MsgExpandBuffer);
			TamagoDemoMsgSet(MsgExpandBuffer);
//			NMenuTalkMsgSetEasy( MsgExpandBuffer );
			JinglePlay(MUS_FANFA5);
			tdw->seqno++;
			break;
	
		case TAMAGODEMO_MES_WAIT:
//			if(TamagoDemoTalkMsgPut()){
			if(JingleWait()){
//				JinglePlay(MUS_FANFA5);
				tdw->seqno++;
			}
			break;
		case TAMAGODEMO_JINGLE_WAIT:
			if(JingleWait()) tdw->seqno++;
			break;
		case TAMAGODEMO_MES_YESNO:
			PokeParaNickNameCopy(&PokeParaMine[tdw->tamagono],StrTempBuffer0);
			PM_MsgExpand(MsgExpandBuffer,rename_mes);
			TamagoDemoMsgTalkSet( MsgExpandBuffer );
//			NMenuTalkMsgSetEasy( MsgExpandBuffer );
			tdw->seqno++;
			break;
		case TAMAGODEMO_MES_YESNO_WAIT:
			if(TamagoDemoTalkMsgPut()){
				NMenuWinBoxWrite(22,8,22+5,8+5);
				YesNoSelectStrInit(22,8);
				tdw->seqno++;
			}
			break;
		case TAMAGODEMO_YESNO_WAIT:
				if( (select=YesNoSelectMain()) != CURSOR_DEFAULT_VALUE){
					if(select==CURSOR_CANCEL_VALUE || select==CURSOR_NO_VALUE){
						tdw->seqno++;
					}else{
						PokeParaNickNameCopy(&PokeParaMine[tdw->tamagono],StrTempBuffer2);
						monsno = PokeParaGet(&PokeParaMine[tdw->tamagono],ID_monsno);
						sex    = PokeSexGet(&PokeParaMine[tdw->tamagono]);
						NameIn(NAMEIN_POKE_NICK,StrTempBuffer2,monsno,sex,
							PokeParaGet(&PokeParaMine[tdw->tamagono],ID_personal_rnd,0),
							ReturnFieldTask);
					}
				}
			break;
		case TAMAGODEMO_FADEOUT:
			PaletteFadeReq( 0xffffffff, 0, 0, 16, 0 );
			tdw->seqno++;
			break;
		case TAMAGODEMO_FADEOUTWAIT:
			if( !(FadeData.fade_sw) ){
				MainProcChange(FieldMainRecover);
//				MusicPlay(ScriptParameter1);
			}
			break;
			
	}
	MainTask();
	JumpActor();
	SetActor();
	PaletteAnime();
	
}

// =================================================================
// ActTamagoHukaMove
// �T�v  : �^�}�S�Ԃ�Ԃ�
// ����  : act
// �߂�l: none
// =================================================================
static void ActTamagoHukaMove(actWork *act)
{
	act->work[0]++;
	if(act->work[0]>20){
		act->move = ActTamagoHukaMove2;
		act->work[0] = 0;
	}else{
		act->work[1]+=20;
		act->work[1]&=0xff;
		act->dx = SinMove(act->work[1],1);
		if(act->work[0]==15){
			SePlay(SE_BOWA);
			ActAnmChg(act,1);
			TamagoPartsPut();
		}
	}
}

// =================================================================
// ActTamagoHukaMove
// �T�v  : 2��ڂ̃^�}�S�Ԃ�Ԃ�
// ����  : act
// �߂�l: none
// =================================================================
static void ActTamagoHukaMove2(actWork *act)
{
	act->work[2]++;
	if(act->work[2]>30){
		act->work[0]++;
		if(act->work[0]>20){
			act->move = ActTamagoHukaMove3;
			act->work[0] = 0;
			act->work[2] = 0;
		}else{
			act->work[1]+=20;
			act->work[1]&=0xff;
			act->dx = SinMove(act->work[1],2);
			if(act->work[0]==15){
				SePlay(SE_BOWA);
				ActAnmChg(act,2);
			}
		}
	}
}

// =================================================================
// ActTamagoHukaMove
// �T�v  : �R��ڂ̃^�}�S�Ԃ�Ԃ�
// ����  : act
// �߂�l: none
// =================================================================
static void ActTamagoHukaMove3(actWork *act)
{
	u16 monsno;
	act->work[2]++;
	if(act->work[2]>30){
		act->work[0]++;
		if(act->work[0]>38){
			act->move = ActTamagoHukaMove4;
			act->work[0] = 0;

				monsno = PokeParaGet(&PokeParaMine[tdw->tamagono],ID_monsno);
				ActWork[tdw->actno[1]].dx = 0;
				ActWork[tdw->actno[1]].dy = pm2fpara[monsno].patcnt;

		}else{
			act->work[1]+=20;
			act->work[1]&=0xff;
			act->dx = SinMove(act->work[1],2);
			if(act->work[0]==15){
				SePlay(SE_BOWA);
				ActAnmChg(act,2);
				TamagoPartsPut();
				TamagoPartsPut();
			}
			if(act->work[0]==30){
				SePlay(SE_BOWA);
			}
		}
	}
}

// =================================================================
// ActTamagoHukaMove4
// �T�v  : �^�}�S������Ƒ҂�
// ����  : act
// �߂�l: none
// =================================================================
static void ActTamagoHukaMove4(actWork *act)
{
	if(++act->work[0]>50){
		act->move = ActTamagoHukaMove5;
		act->work[0] = 0;
	}
}

// =================================================================
// ActTamagoHukaMove5
// �T�v  : �^�}�S����ăz���C�g�A�E�g
// ����  : act
// �߂�l: none
// =================================================================
static void ActTamagoHukaMove5(actWork *act)
{
	s16 i;
	if(act->work[0]==0){
		PaletteFadeReq(0xffffffff, -1, 0, 16, 0xffff);
	}
	
	if(act->work[0]>=0 && act->work[0]<=3){
		for(i=0;i<4;i++){
			TamagoPartsPut();
		}
	}

	
	act->work[0]++;
	if( !(FadeData.fade_sw) ){
		SePlay(SE_TAMAGO);
		act->banish = 1;
		act->move = ActTamagoHukaMove6;
		act->work[0] = 0;
	}
}

static void ActTamagoHukaMove6(actWork *act)
{
	if(act->work[0]==0){
		ActWork[tdw->actno[1]].banish = 0;
		ActAffAnmChg(&ActWork[tdw->actno[1]],1);
	}
	
	if(act->work[0]==8)
		PaletteFadeReq(0xffffffff, -1, 16, 0, 0xffff);

	if(act->work[0]<10){
		ActWork[tdw->actno[1]].y--;
	}
	if(act->work[0] > 40){
		act->move = DummyActMove;
	}
	act->work[0]++;
}

// =================================================================
// TamagoPartsActMove
// �T�v  : 
// ����  : act
// �߂�l: none
// =================================================================
static void TamagoPartsActMove(actWork *act)
{
	act->work[4] += act->work[1];
	act->work[5] += act->work[2];
	act->dx = act->work[4]/256;
	act->dy = act->work[5]/256;
	act->work[2] += act->work[3];
	
	if((act->y+act->dy) > (act->y+20) && act->work[2] > 0){
		DelActor(act);
	}
}

static const s16 tamagotbl[][2]={
	{-3*128,  -5*192},
	{-10*128, -4*192}, {7*128, -4*192},
	{-8*128,  -5*192}, {4*128, -2*192}, {-1*128, -9*192}, {10*128, -3*192},
	{-3*128,  -5*192}, {9*128, -2*192}, {-2*128, -9*192}, {8*128,  -3*192},
	{-7*128,  -5*192}, {2*128, -2*192}, {-75*12, -9*192}, {9*128,  -3*192},
	{-1*128, -10*192}, {2*128, -6*192}, {-5*128, -3*192}, {5*128, -10*192},
};

// =================================================================
// TamagoPartsPut
// �T�v  : 
// ����  : none
// �߂�l: none
// =================================================================
static void TamagoPartsPut()
{
	s16 wx,wy,parts;
//	wx = (pp_rand()%1700)-(1700-2);
//	wy = -(pp_rand()%2024)-512;
	wx = tamagotbl[tdw->tamagoparts][0];
	wy = tamagotbl[tdw->tamagoparts][1];
	tdw->tamagoparts++;
	parts = pp_rand()%4;
	TamagoPartsSet(120, 60,  wx,wy, 100,parts);
}

// =================================================================
// TamagoPartsSet
// �T�v  : �^�}�S��юU��p�[�c�Z�b�g
// ����  : x	
//       : y
//       : dx	�����_�ȉ�8bit
//       : dy	�����_�ȉ�8bit
//       : gg	�����_�ȉ�8bit
// �߂�l: none
// =================================================================
static void TamagoPartsSet(u8 x, u8 y, s16 dx ,s16 dy, s16 gg, u8 n)
{
	u8 no;
	no = AddActor(&ActTamagoPartsHeader,x,y,4);
	ActWork[no].work[1] = dx;
	ActWork[no].work[2] = dy;
	ActWork[no].work[3] = gg;
	ActAnmChg(&ActWork[no],n);
}

// =================================================================
// TamagoDemoMsgTalkSet
// �T�v  : 
// ����  : 
//       : msg_adrs
// �߂�l: none
// =================================================================
static void TamagoDemoMsgTalkSet(const u8 *msg_adrs)
{
	TalkMsgSet(&tdw->tw, msg_adrs, tdw->windowwork, 3, 15);
}


// =================================================================
// TmagoDemoMsgSet
// �T�v  : 
// ����  : msg_adrs
// �߂�l: none
// =================================================================
static void TamagoDemoMsgSet(const u8 *msg_adrs)
{
	MsgPrint( &tdw->tw, msg_adrs, tdw->windowwork, 3, 15);
}
// =================================================================
// TamagoDemoTalkMsgPut
// �T�v  : 
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 TamagoDemoTalkMsgPut()
{
	return TalkMsgPut(&tdw->tw);
}

#if 0
// =================================================================
// TamagoDemoYesNoMove
// �T�v  : 
// ����  : none
// �߂�l: static u8
// =================================================================
static u8 TamagoDemoYesNoMove()
{
	u8 ret = 0;
	if(sys.Trg&U_KEY){
		if(tdw->selectpos!=0){
			tdw->selectpos = 0;
			SePlay(SE_SELECT);
		}
	}else if(sys.Trg&D_KEY){
		if(tdw->selectpos!=1){
			tdw->selectpos = 1;
			SePlay(SE_SELECT);
		}
	
	}else if(sys.Trg&A_BUTTON){
		SePlay(SE_SELECT);
		ret = tdw->selectpos+1;
	
	}else if(sys.Trg&B_BUTTON){
		SePlay(SE_SELECT);
		ret = 2;
	}
	return 0;
}

����c�c�c�c�c�H
//�^�}�S�̂悤�����c�c�H

�^�}�S���@��������
�H�H�H���@���܂ꂽ�I

���܂ꂽ�@�H�H�H��
�j�b�N�l�[�����@���܂����H

#endif
