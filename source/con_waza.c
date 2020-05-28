//=========================================================================
//
//		�R���e�X�g�Z���ʃv���O����
//											by matsuda 2001.10.01(��)
//=========================================================================

#include "common.h"
#include "contest.h"
#include "con_tool.h"
#include "agbmons.h"
#include "calctool.h"
#include "param.h"

#include "..\msgdata\conmsg.h"

//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
#include "con_waza.dat"
extern	const	WazaTblDat	WTD[];

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------

#if 1
static void NormalAppeal(void);
static void Bougai2baiAppeal(void);
static void JibakuAppeal(void);
static void Penalty0Appeal(void);
static void DefendsOnlyOnceAppeal(void);
static void AbsoluteDefenceAppeal(void);
static void Bougai20InvalidAppeal(void);
static void BougaiAll10Appeal(void);
static void BeforeOneSub(void);
static void BeforeAllSub(void);
static void BeforeOneRandSub(void);
static void BeforeAllSubRand(void);
static void Combo1Del(void);
static void Combo1Sub40(void);
static void BeforeAll40Rest(void);
static void SameType40(void);
static void StyleType40(void);
static void BeautifulType40(void);
static void CuteType40(void);
static void CleverType40(void);
static void StrongType40(void);
static void StrainBinds(void);
static void StrainBindsAll(void);
static void BonusErases(void);
static void BonusBougai(void);
static void Itii3bai(void);
static void Yonni3bai(void);
static void HanbunGet(void);
static void BeforeSameAppeal(void);
static void JyunbandeHenka(void);
static void RandomEvaluation(void);
static void BeforeSameType(void);
static void BeforeNotSameType(void);
static void BeforeAppealChange(void);
static void BonusAppeal(void);
static void Bonus4bai(void);
static void NextAppeal1ban(void);
static void NextAppeal4ban(void);
static void NextAppealReverse(void);
static void NextAppealRandom(void);
static void SpecTypeSame(u8 type);
static u8 ShieldCheck(u8 brd);
static u8 BougaiCalc(void);
static void APSub(u8 brd, u8 bougai);
static void AbsoluteTensyonUp(void);
static void AppealHalf(void);
static void TensyondeHenka(void);
void TensyonStop(void);
static s16 HasuuKirisute(s16 ap);
static s16 HasuuKiriage(s16 bougai);

//----------------------------------------------------------
//	�R���e�X�g���ʋZ�V�[�P���X�e�[�u��
//----------------------------------------------------------
const pFunc ConWazaSeqTbl2[] = 
{
	NormalAppeal,
	Bougai2baiAppeal,
	JibakuAppeal,
	Penalty0Appeal,
	DefendsOnlyOnceAppeal,
	AbsoluteDefenceAppeal,
	Bougai20InvalidAppeal,
	BougaiAll10Appeal,
	BeforeOneSub,
	BeforeAllSub,
	BeforeOneSub,
	BeforeAllSub,
	BeforeOneSub,
	BeforeAllSub,
	BeforeOneRandSub,
	BeforeAllSubRand,
	Combo1Del,
	Combo1Sub40,
	BeforeAll40Rest,
	SameType40,
	StyleType40,
	BeautifulType40,
	CuteType40,
	CleverType40,
	StrongType40,
	StrainBinds,
	StrainBindsAll,
	BonusErases,
	BonusBougai,
	Itii3bai,
	Yonni3bai,
	HanbunGet,
	BeforeSameAppeal,
	JyunbandeHenka,
	RandomEvaluation,
	BeforeSameType,
	BeforeNotSameType,
	BeforeAppealChange,
	BonusAppeal,
	Bonus4bai,
	NextAppeal1ban,
	NextAppeal4ban,
	NextAppealReverse,
	NextAppealRandom,
	AbsoluteTensyonUp,
	AppealHalf,
	TensyondeHenka,
	TensyonStop,
};
#endif

//=========================================================================
//	�v���O����
//=========================================================================


//=========================================================================
//	�R���{���������邩�`�F�b�N
// �߂�l�F���ރ��x��(1�`3�@0�̏ꍇ�ͺ��ޕs����)
//=========================================================================
u8 ComboCheck(u16 combo1_waza, u16 combo2_waza)
{
	u8 combo1, combo2[4];
	
	combo1 = ConWazaTbl[combo1_waza].combo1;
	combo2[0] = ConWazaTbl[combo2_waza].combo2_0;
	combo2[1] = ConWazaTbl[combo2_waza].combo2_1;
	combo2[2] = ConWazaTbl[combo2_waza].combo2_2;
	combo2[3] = ConWazaTbl[combo2_waza].combo2_3;
	if(combo1 == 0)
		return 0;
	if(combo1 == combo2[0] || combo1 == combo2[1] || combo1 == combo2[2] || combo1 == combo2[3])
		return ComboLevelTbl[combo1];
	return 0;
}





//=========================================================================
//=========================================================================
//=========================================================================



//=========================================================================
//
//		�V�E�v�Z��
//												by matsuda 2002.04.01(��)
//=========================================================================

#if 1	//------------- �Z�v�Z�� ---------------------------------

//----------------------------------------------------------
//	�O�F�m�[�}��40
//	���ɂȂ�
//----------------------------------------------------------
static void NormalAppeal(void)
{
}

//----------------------------------------------------------
//	�P�F�W�Q��Q2�{
//	�W�Q�����Ƒ啝��AP�l��������B�W�Q�l���W�Q�l�~�Q
//----------------------------------------------------------
static void Bougai2baiAppeal(void)
{
	apsys[CONWAZA->nowbrd].bougai2bai = 1;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL1_MSG);
}

//----------------------------------------------------------
//	�Q�F����
//	������݂����߰قɎQ���ł��Ȃ��Ȃ�B��ٌ߰�A�W�Q���󂯂Ă������ɂȂ�B
//----------------------------------------------------------
static void JibakuAppeal(void)
{
	apsys[CONWAZA->nowbrd].next_endAppeal_flg = 1;
//	apsys[CONWAZA->nowbrd].endAppeal_flg = 1;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL2_MSG);
}

//----------------------------------------------------------
//	�R�F�y�i���e�B0
//	�A�����ďo���Ă��y�i���e�B���󂯂Ȃ��B
//----------------------------------------------------------
static void Penalty0Appeal(void)
{
	apsys[CONWAZA->nowbrd].penalty0 = 1;
	apsys[CONWAZA->nowbrd].penalty = 0;
	apsys[CONWAZA->nowbrd].conti_count = 0;
}

//----------------------------------------------------------
//	�S�F1�񂾂��h��
//	1�x�����W�Q�̌��ʂ�h���B2�x�ڈȍ~�͂��̂܂ܖW�Q���󂯂�B
//----------------------------------------------------------
static void DefendsOnlyOnceAppeal(void)
{
	apsys[CONWAZA->nowbrd].shield_flg = 1;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL4_0_MSG);
}

//----------------------------------------------------------
//	�T�F���S�h��
//	�W�Q��߰ق̌��ʂ�K���h��
//----------------------------------------------------------
static void AbsoluteDefenceAppeal(void)
{
	apsys[CONWAZA->nowbrd].Ashield_flg = 1;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL5_0_MSG);
}

//----------------------------------------------------------
//	�U�F�W�Q-20�𖳌�
//	�W�Q�l20�ȉ����󂯕t���Ȃ�
//----------------------------------------------------------
static void Bougai20InvalidAppeal(void)
{
	apsys[CONWAZA->nowbrd].invalid = 20;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL6_0_MSG);
}

//----------------------------------------------------------
//	�V�F�W�Q�l���P�O
//	�W�Q���󂯂����A�W�Q�l��S��10�ɂ���
//----------------------------------------------------------
static void BougaiAll10Appeal(void)
{
	apsys[CONWAZA->nowbrd].invalid10 = 1;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL7_0_MSG);
}

//----------------------------------------------------------
//	�W�A�P�O�A�P�Q�C�P�S�F�O1�l-X
//	������1�O�ɃA�s�[�������|�P�����̕]����20���炷�B��������Ԃ̎������B
//----------------------------------------------------------
static void BeforeOneSub(void)
{
	u8 brd;
	int i;
	u8 flg = 0;
	
	brd = CONWAZA->nowbrd;
	if(CONWAZA->Sort[brd] != 0){
		for(i = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[brd] - 1 == CONWAZA->Sort[i])
				break;
		}
		CONWAZA->target[0] = i;
		CONWAZA->target[1] = 0xff;
		flg = BougaiCalc();
	}
	if(flg == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�X�A�P�P�C�P�R�A�P�W�F�O�S��-X
//	�����̑O�ɱ�߰ق����|�P�����S���̕]����10���炷�B��������Ԗڂ̎������B
//----------------------------------------------------------
static void BeforeAllSub(void)
{
	u8 brd;
	int i, t;
	u8 flg = 0;
	
	brd = CONWAZA->nowbrd;
	if(CONWAZA->Sort[brd] != 0){
		for(i = 0, t = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[brd] > CONWAZA->Sort[i]){
				CONWAZA->target[t] = i;
				t++;
			}
		}
		CONWAZA->target[t] = 0xff;
		flg = BougaiCalc();
	}
	if(flg == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�P�S�F�O1�l-����
//	������1�O�ɱ�߰ق����߹�݂̕]��������тŌ��炷
//	(20=20%�A40=60%�A60=20%)������1�Ԃ̎�����
//----------------------------------------------------------
static void BeforeOneRandSub(void)
{
	u8 rand;
	s8 subap;
	
	rand = pp_rand() % 10;
	if(rand < 2)
		subap = 20;
	else if(rand < 8)
		subap = 40;
	else
		subap = 60;
	CONWAZA->bougai = subap;
	BeforeOneSub();
}

//----------------------------------------------------------
//	�P�T�F�O�S��-����
//	�����̑O�ɱ�߰ق����߹�ݑS��������тŌ��炷
//	(0=10%, 10=20%, 20=20%, 30=20%, 40=20%, 60=10%)
//----------------------------------------------------------
static void BeforeAllSubRand(void)
{
	u8 rand;
	s8 subap;
	u8 brd;
	int i;
	u8 flg, temp = 0;
	
	brd = CONWAZA->nowbrd;
	if(CONWAZA->Sort[brd] != 0){
		for(i = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[brd] > CONWAZA->Sort[i]){
				CONWAZA->target[0] = i;
				CONWAZA->target[1] = 0xff;
				
				rand = pp_rand() % 10;
				if(rand < 1)
					subap = 0;
				else if(rand < 3)
					subap = 10;
				else if(rand < 5)
					subap = 20;
				else if(rand < 7)
					subap = 30;
				else if(rand < 9)
					subap = 40;
				else
					subap = 60;
				CONWAZA->bougai = subap;
				flg = BougaiCalc();
				if(flg)
					temp++;
			}
		}
	}
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
	if(temp == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
}

//----------------------------------------------------------
//	�P�U�F�R���{1����
//	�R���{1��Ԃ��߹�݂����鎞�A���̌��ʂ�ł������B
//----------------------------------------------------------
static void Combo1Del(void)
{
	int i;
	u8 brd;
	u8 flg = 0;
	
	brd = CONWAZA->nowbrd;
	if(CONWAZA->Sort[brd] != 0){
		for(i = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[brd] > CONWAZA->Sort[i]){
				//if(apsys[i].combo_turn && ShieldCheck(i)){
				if(apsys[i].combo1_flg && ShieldCheck(i))
				{
					//apsys[i].combo_turn = 0;
					apsys[i].combo1_flg = 0;
					apsys[i].combo1_del = 1;
					AppealKoukaMsgSet(i, APPEAL16_1_MSG);
					flg = 1;
				}
			}
		}
	}
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL16_0_MSG);
	if(flg == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
}

//----------------------------------------------------------
//	�P�V�F�R���{1-40
//	�R���{1��Ԃ��߹�݂̕]����40���炷�B����ȊO��10
//----------------------------------------------------------
static void Combo1Sub40(void)
{
	int i;
	u8 brd;
	u8 temp = 0;
	u8 flg;
	
	brd = CONWAZA->nowbrd;
	if(CONWAZA->Sort[brd] != 0){
		for(i = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[brd] > CONWAZA->Sort[i]){
				//if(apsys[i].combo_turn)
				if(apsys[i].combo1_flg)
					CONWAZA->bougai = 50;
				else
					CONWAZA->bougai = 10;	//�����̒l��Ă��Ȃ��Ǝ����߹�݂ł��̒l�ŖW�Q���Ă��܂�
				CONWAZA->target[0] = i;
				CONWAZA->target[1] = 0xff;
				flg = BougaiCalc();
				if(flg)
					temp++;
			}
		}
	}
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
	if(temp == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
}

//----------------------------------------------------------
//	�P�W�F�S��-40��1�x
//	�����̑O�ɱ�߰ق����߹�ݑS���̕]����40���炷�B�������1��x��
//----------------------------------------------------------
static void BeforeAll40Rest(void)
{
	apsys[CONWAZA->nowbrd].next_yasumi_turn = 1;
//	apsys[CONWAZA->nowbrd].yasumi_turn = 2;
	BeforeAllSub();
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�P�X�F���^�C�v��-40
//	�����̑O�ɃA�s�[�������߹�݂̒��ŁA�����Ɠ����^�C�v�̱�߰ق�40���炷
//----------------------------------------------------------
static void SameType40(void)
{
	u16 wazano;
	
	wazano = apsys[CONWAZA->nowbrd].use_waza;
	SpecTypeSame(ConWazaTbl[wazano].con_type);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�Q�O�F����������-40
//	�����̑O�ɱ�߰ق����߹�݂̒��ŁA�������������߂̱�ٕ߰]����40���炷
//----------------------------------------------------------
static void StyleType40(void)
{
	SpecTypeSame(STYLE_TYPE);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�Q�P�F��������-40
//	�����̑O�ɱ�߰ق����߹�݂̒��ŁA���������߂̱�ٕ߰]����40���炷
//----------------------------------------------------------
static void BeautifulType40(void)
{
	SpecTypeSame(BEAUTIFUL_TYPE);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�Q�Q�F���킢��-40
//	�����̑O�ɱ�߰ق����߹�݂̒��ŁA���킢�����߂̱�ٕ߰]����40���炷
//----------------------------------------------------------
static void CuteType40(void)
{
	SpecTypeSame(CUTE_TYPE);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�Q�R�F��������-40
//	�����̑O�ɱ�߰ق����߹�݂̒��ŁA�����������߂̱�ٕ߰]����40���炷
//----------------------------------------------------------
static void CleverType40(void)
{
	SpecTypeSame(CLEVER_TYPE);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�Q�S�F�����܂���-40
//	�����̑O�ɱ�߰ق����߹�݂̒��ŁA�����܂������߂̱�ٕ߰]����40���炷
//----------------------------------------------------------
static void StrongType40(void)
{
	SpecTypeSame(STRONG_TYPE);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�Q�T�F��1�l�ٔ�
//	���̌�A��߰ق����߹�݂��ْ�������B
//	�ْ������߹�݂́A��߰ق����s�����߲�Ă��Ⴆ�Ȃ�
//----------------------------------------------------------
static void StrainBinds(void)
{
	int i;
	u8 flg = 0;
	
	if(CONWAZA->Sort[CONWAZA->nowbrd] != BREEDER_MAX-1){
		for(i = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[CONWAZA->nowbrd]+1 == CONWAZA->Sort[i]){
				if(ShieldCheck(i)){
					KintyouFlgSet(i);
					AppealKoukaMsgSet(i, APPEAL25_1_MSG);
					flg = 1;
				}
				else{
					AppealKoukaMsgSet(i, HEIKIDATTA_MSG);
					flg = 1;
				}
			}
		}
	}
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL25_0_MSG);
	if(flg == 0)
//		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOKINTYOU_MSG);
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
}

//----------------------------------------------------------
//	�Q�U�F��S���ٔ�
//	���̌�A��߰ق����߹�ݑS�����ْ�������B
//	�ْ�����m��=���1�l�̏ꍇ60%�A
//	���2�l�̏ꍇ�A30%
//	���3�l�̏ꍇ�A20%
//----------------------------------------------------------
static void StrainBindsAll(void)
{
	int i, t;
	u8 target[BREEDER_MAX+1];
	s16 syuuseiti[BREEDER_MAX];
	s16 kakuritu[BREEDER_MAX];
	u8 flg = 0;
	u8 heiki_flg = 0;
	
	memset(target, 0xff, sizeof(target));
	for(i = 0, t = 0; i < BREEDER_MAX; i++){
		if(CONWAZA->Sort[CONWAZA->nowbrd] < CONWAZA->Sort[i]
				&& apsys[i].miss_flg == 0 && YasumiTurnCheck(i) == 0)
			target[t++] = i;
	}
	if(t == 1){
			kakuritu[0] = 60;	//70//100;
	}
	else if(t == 2){
		kakuritu[0] = 30;	//40;
		kakuritu[1] = 30;	//60;
	}
	else if(t == 3){
		kakuritu[0] = 20;	//25;
		kakuritu[1] = 20;	//30;
		kakuritu[2] = 20;	//35;
	}
	else{
		for(i = 0; i < BREEDER_MAX; i++)
			kakuritu[i] = 0;	//���ɒN�����Ȃ��̂ŏI��
	}
	
	for(i = 0; i < BREEDER_MAX; i++){
		//����1��Ԃ̎��ْ͋����₷���Ȃ�
		if(apsys[i].combo1_flg && Combo1TermCheck(i))
			syuuseiti[i] = ComboLevelTbl[ConWazaTbl[apsys[i].use_wazaold].combo1] * 10;
		else
			syuuseiti[i] = 0;
		
		//�ްŽ��Ԃ̎��͐���ɂ�10%�ْ������炭�Ȃ� 2002.08.27(��)
		syuuseiti[i] -= (apsys[i].bonus / BONUS_POINT) * 10;
	}
	
	if(kakuritu[0] != 0){
		for(i = 0; target[i] != 0xff; i++){
			if((pp_rand() % 100) < (kakuritu[i] + syuuseiti[target[i]])){
				if(ShieldCheck(target[i])){
					KintyouFlgSet(target[i]);
					AppealKoukaMsgSet(target[i], APPEAL25_1_MSG);
					flg++;
				}
				else
					heiki_flg = 1;
			}else
				heiki_flg = 1;
			
			if(heiki_flg){
				heiki_flg = 0;
				AppealKoukaMsgSet(target[i], HEIKIDATTA_MSG);
				flg++;
			}
			CONWAZA->nowwaza_target[target[i]] = 1;
		}
	}
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL26_MSG);
	if(flg == 0)
//		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMODAREMOKINTYOU_MSG);
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
}

//----------------------------------------------------------
//	�Q�V�F�{�[�i�X����
//	�����̑O�ɱ�߰ق����߹�݂̒��ŁA�ްŽ��Ԃ��߹�݂����鎞�A�ްŽ�l��0�ɂ���
//----------------------------------------------------------
static void BonusErases(void)
{
	int i;
	u8 flg = 0;
	
	for(i = 0; i < BREEDER_MAX; i++){
		if(CONWAZA->Sort[CONWAZA->nowbrd] > CONWAZA->Sort[i]){
			if(apsys[i].bonus > 0){
				if(ShieldCheck(i)){
					//APSub(i, apsys[i].bonus);
					apsys[i].bonus = 0;
					apsys[i].bonus_flg = 2;
					AppealKoukaMsgSet(i, APPEAL27_1_MSG);
					flg++;
				}
			}
		}
	}
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL27_0_MSG);
	if(flg == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOMUSI_MSG);
}

//----------------------------------------------------------
//	�Q�W�F�{�[�i�X�W�Q
//	�����̑O�ɱ�߰ق����߹�݂̒��ŁA�ްŽ��Ԃ��߹�݂������ꍇ�A
//	��ٕ߰]����40���炷�B����ȊO��10
//----------------------------------------------------------
static void BonusBougai(void)
{
	int i;
	u8 flg, temp = 0;
	
	for(i = 0; i < BREEDER_MAX; i++){
		if(CONWAZA->Sort[CONWAZA->nowbrd] > CONWAZA->Sort[i]){
			if(apsys[i].bonus > 0)
				CONWAZA->bougai = 40;
			else
				CONWAZA->bougai = 10;
			CONWAZA->target[0] = i;
			CONWAZA->target[1] = 0xff;
			flg = BougaiCalc();
			if(flg)
				temp++;
		}
	}
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL28_MSG);
	if(temp == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOMUSI_MSG);
}

//----------------------------------------------------------
//	�Q�X�F1��=3�{
//	1�ʂ̏ꍇ�̂ݕ]����3�{�ɂȂ�
//----------------------------------------------------------
static void Itii3bai(void)
{
	u16 wazano;
	
	//if(CONWAZA->Sort[CONWAZA->nowbrd] == 0){
	if(BrdSort[CONWAZA->nowbrd] == 0){
		wazano = apsys[CONWAZA->nowbrd].use_waza;
		apsys[CONWAZA->nowbrd].ap += WazaKoukaTbl[ConWazaTbl[wazano].apno].ap * 2;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL29_MSG);
	}
}

//----------------------------------------------------------
//	�R�O�F4��=3�{
//	4�ʂ̏ꍇ�̂ݕ]����3�{�ɂȂ�
//----------------------------------------------------------
static void Yonni3bai(void)
{
	u16 wazano;
	
	//if(CONWAZA->Sort[CONWAZA->nowbrd] == BREEDER_MAX-1){
	if(BrdSort[CONWAZA->nowbrd] == BREEDER_MAX-1){
		wazano = apsys[CONWAZA->nowbrd].use_waza;
		apsys[CONWAZA->nowbrd].ap += WazaKoukaTbl[ConWazaTbl[wazano].apno].ap * 2;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL30_MSG);
	}
}

//----------------------------------------------------------
//	�R�P�F�S��/2=����
//	�����̑O�܂łɍs��ꂽײ����߹�݂̕\���l���v�̔����������̕]���ɂȂ�B
//	��������Ԃ̎���0�B
//----------------------------------------------------------
static void HanbunGet(void)
{
	int i, hyouka;
	
	for(i = 0, hyouka = 0; i < BREEDER_MAX; i++){
		if(CONWAZA->Sort[CONWAZA->nowbrd] > CONWAZA->Sort[i])
			hyouka += apsys[i].ap;
	}
	if(hyouka < 0)
		hyouka = 0;
	if(CONWAZA->Sort[CONWAZA->nowbrd] == 0 || hyouka == 0)
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL31_1_MSG);
	else{
		apsys[CONWAZA->nowbrd].ap += hyouka / 2;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL31_0_MSG);
	}
	apsys[CONWAZA->nowbrd].ap = HasuuKirisute(apsys[CONWAZA->nowbrd].ap);
}

//----------------------------------------------------------
//	�R�Q�F�O�̐l������
//	�����̑O���߹�݂̱�ٕ߰]���Ɠ��������]���𓾂���B������1�Ԗڂ̎���0
//----------------------------------------------------------
static void BeforeSameAppeal(void)
{
	int i;
	s16 hyouka = 0;
	
	if(CONWAZA->Sort[CONWAZA->nowbrd] != 0){
		for(i = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[CONWAZA->nowbrd] -1 == CONWAZA->Sort[i])
				hyouka = apsys[i].ap;
		}
	}
	
	if(CONWAZA->Sort[CONWAZA->nowbrd] == 0 || hyouka <= 0)
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL32_1_MSG);
	else{
		apsys[CONWAZA->nowbrd].ap += hyouka;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL32_0_MSG);
	}
}

//----------------------------------------------------------
//	�R�R�F1<2<3<4
//	�o�����Ԃɂ���āA�]�����ς��
//	(��قǕ]���������B1�Ԗ�=10�A2�Ԗ�=20�A3�Ԗ�=40�A4�Ԗ�=60)
//----------------------------------------------------------
static void JyunbandeHenka(void)
{
	u8 jyunban;
	
	jyunban = CONWAZA->Sort[CONWAZA->nowbrd];
	if(jyunban == 0)
		apsys[CONWAZA->nowbrd].ap = 10;
	else
		apsys[CONWAZA->nowbrd].ap = jyunban * 20;
	
	if(jyunban == 0)
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL33_0_MSG);
	else if(jyunban == 1)
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL33_1_MSG);
	else if(jyunban == 2)
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL33_2_MSG);
	else
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL33_3_MSG);
}

//----------------------------------------------------------
//	�R�S�F�����ŕ]���ω�
//	�����_���ŕ]�����ς��(10=30%, 20=30%, 40=20%, 60=10%, 80=10%)
//----------------------------------------------------------
static void RandomEvaluation(void)
{
	u8 rand;
	s8 addap;
	
	rand = pp_rand() % 10;
	if(rand < 3){
		addap = 10;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_1_MSG);
	}else if(rand < 6){
		addap = 20;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_2_MSG);
	}else if(rand < 8){
		addap = 40;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_3_MSG);
	}else if(rand < 9){
		addap = 60;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_4_MSG);
	}else{
		addap = 80;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_5_MSG);
	}
	apsys[CONWAZA->nowbrd].ap = addap;
}

//----------------------------------------------------------
//	�R�T�F�����ꍇ3�{
//	�����̑O�ɱ�߰ق����߹�݂Ɠ�����ý����߂̏ꍇ�A��߰ق�3�{�ɂȂ�B1�Ԗڂ͖���
//----------------------------------------------------------
static void BeforeSameType(void)
{
	u16 mywaza;
	s8 mysort, target_sort, t_brd;
	
	mysort = CONWAZA->Sort[CONWAZA->nowbrd];
	target_sort = mysort-1;
	if(mysort == 0)
		return;
	
	while(1){
		for(t_brd = 0; t_brd < BREEDER_MAX; t_brd++){
			if(CONWAZA->Sort[t_brd] == target_sort)
				break;
		}
		
		if(apsys[t_brd].endAppeal_flg 	//��߰ق��I�����Ă�
			|| apsys[t_brd].yasumi_turn 	//�x��
			|| apsys[t_brd].miss_flg)		//�ْ�
		{
			target_sort--;
			if(target_sort < 0)
				return;
		}
		else
			break;
	}

	mywaza = apsys[CONWAZA->nowbrd].use_waza;
	if(ConWazaTbl[mywaza].con_type == ConWazaTbl[apsys[t_brd].use_waza].con_type){
		apsys[CONWAZA->nowbrd].ap += WazaKoukaTbl[ConWazaTbl[mywaza].apno].ap * 2;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL35_MSG);
	}
}

//----------------------------------------------------------
//	�R�U�F�Ⴄ�ꍇ2�{
//	�����̑O�ɱ�߰ق����߹�݂ƈႤ��ý����߂̏ꍇ�A��߰ق�3�{�ɂȂ�B1�Ԗڂ͖���
//----------------------------------------------------------
static void BeforeNotSameType(void)
{
	int i;
	u16 mywaza;

	if(CONWAZA->Sort[CONWAZA->nowbrd] != 0){
		mywaza = apsys[CONWAZA->nowbrd].use_waza;
		for(i = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[CONWAZA->nowbrd] - 1 == CONWAZA->Sort[i]){
				if(ConWazaTbl[mywaza].con_type 
						!= ConWazaTbl[apsys[i].use_waza].con_type){
					apsys[CONWAZA->nowbrd].ap 
						+= WazaKoukaTbl[ConWazaTbl[mywaza].apno].ap * 2;
					AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL36_MSG);
					break;
				}
			}
		}
	}
}

//----------------------------------------------------------
//	�R�V�F�O�̐l�ŕω�
//	���O�̱�߰قɂ��]�����ς��B�O�̐l�̱�߰ق��ゾ������2�{�B
//	����������0�B�������ꍇ�͂��̂܂�
//----------------------------------------------------------
static void BeforeAppealChange(void)
{
	int i;
	u16 mywaza;

	if(CONWAZA->Sort[CONWAZA->nowbrd] != 0){
		mywaza = apsys[CONWAZA->nowbrd].use_waza;
		for(i = 0; i < BREEDER_MAX; i++){
			if(CONWAZA->Sort[CONWAZA->nowbrd]-1 == CONWAZA->Sort[i]){
				if(apsys[CONWAZA->nowbrd].ap > apsys[i].ap){
					apsys[CONWAZA->nowbrd].ap *= 2;
					AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL37_0_MSG);
				}else if(apsys[CONWAZA->nowbrd].ap < apsys[i].ap){
					apsys[CONWAZA->nowbrd].ap = 0;
					AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL37_1_MSG);
				}
			}
		}
	}
}

//----------------------------------------------------------
//	�R�W�F�ްŽ+10
//	�g�p������݂��A����݂̕]�����ްŽ�����B�ްŽ�l��+10�B�ްŽ�͍ő�+30�܂�
//----------------------------------------------------------
static void BonusAppeal(void)
{
	if(apsys[CONWAZA->nowbrd].bonus < BONUS_MAX){
		apsys[CONWAZA->nowbrd].bonus += BONUS_POINT;
		apsys[CONWAZA->nowbrd].bonus_flg = 1;
//		apsys[CONWAZA->nowbrd].ap += 10;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL38_MSG);
	}else
		AppealKoukaMsgSet(CONWAZA->nowbrd, DEMOTYOUSIHA_MSG);
}

//----------------------------------------------------------
//	�R�X�F�ްŽ�~3
//	�ްŽ�l�~3������
//----------------------------------------------------------
static void Bonus4bai(void)
{
	apsys[CONWAZA->nowbrd].bonus3bai = 1;
	if(apsys[CONWAZA->nowbrd].bonus)
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL39_MSG);
	else
		AppealKoukaMsgSet(CONWAZA->nowbrd, TYOUSIGAYOKUNAKUTE_MSG);
}

//----------------------------------------------------------
//	�S�O�F����߰�1��
//	������݁A��߰ق��ŏ��ɂȂ�B������݂ŕ������鎞�́A
//	�ォ��̕����D��x�������B5��ݖڂ͌��ʖ���
//----------------------------------------------------------
static void NextAppeal1ban(void)
{
	s8 i, t;
	u8 sort[BREEDER_MAX];
	
	if(consys->turn == END_TURN-1)
		return;

	for(i = 0; i < BREEDER_MAX; i++)
		sort[i] = apsys[i].next_sort;
	
	sort[CONWAZA->nowbrd] = 0xff;	//�����̏��͋󔒂ɂ��Ă���
	
	for(i = 0; i < BREEDER_MAX; i++){
		for(t = 0; t < BREEDER_MAX; t++){
			if(t == CONWAZA->nowbrd)
				continue;
			if(i == sort[t] && sort[t] == apsys[t].next_sort){
				sort[t]++;
				break;
			}
		}
		if(t == BREEDER_MAX)
			break;
	}
	sort[CONWAZA->nowbrd] = 0;
	apsys[CONWAZA->nowbrd].nextsort_flg = 1;
	
	for(i = 0; i < BREEDER_MAX; i++)
		apsys[i].next_sort = sort[i];
	apsys[CONWAZA->nowbrd].now_nextsort = 1;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL40_MSG);
}

//----------------------------------------------------------
//	�S�P�F����߰�4��
//	������݁A��߰ق��Ō�ɂȂ�B������݂ŕ������鎞�́A
//	�ォ��̕����D��x�������B5��ݖڂ͌��ʖ���
//----------------------------------------------------------
static void NextAppeal4ban(void)
{
	s8 i, t;
	u8 sort[BREEDER_MAX];

	if(consys->turn == END_TURN-1)
		return;

	for(i = 0; i < BREEDER_MAX; i++)
		sort[i] = apsys[i].next_sort;

	sort[CONWAZA->nowbrd] = 0xff;	//�����̏��͋󔒂ɂ��Ă���

	for(i = BREEDER_MAX-1; i > -1; i--){
		for(t = 0; t < BREEDER_MAX; t++){
			if(t == CONWAZA->nowbrd)
				continue;
			if(i == sort[t] && sort[t] == apsys[t].next_sort){
				sort[t]--;
				break;
			}
		}
		if(t == BREEDER_MAX)
			break;
	}
	sort[CONWAZA->nowbrd] = BREEDER_MAX-1;
	apsys[CONWAZA->nowbrd].nextsort_flg = 1;
	
	for(i = 0; i < BREEDER_MAX; i++)
		apsys[i].next_sort = sort[i];
	apsys[CONWAZA->nowbrd].now_nextsort = 2;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL41_MSG);
}

//----------------------------------------------------------
//	�S�Q�F�t���ɱ�߰�
//	������݁A�A�s�[�����鏇�Ԃ�������ɂȂ�B5�^�[���ڂ͌��ʖ���
//----------------------------------------------------------
static void NextAppealReverse(void)
{
	//����A�g�p���Ă�Z���Ȃ����A���R�c��������Ȃ��Ă����ƌ����Ă���̂�
	return;
}

//----------------------------------------------------------
//	�S�R�F���̏��Ԃ𗐐�
//	���̃^�[���A�A�s�[�����鏇�Ԃ������_���ɂȂ�B5�^�[���ڂ͌��ʖ���
//----------------------------------------------------------
static void NextAppealRandom(void)
{
	s8 i, t;
	u8 sort[BREEDER_MAX];
	u8 brdno[BREEDER_MAX];
	u8 temp, temp2;
	
	if(consys->turn == END_TURN-1)
		return;

	for(i = 0; i < BREEDER_MAX; i++){
		sort[i] = apsys[i].next_sort;
		brdno[i] = i;
	}

	for(i = 0; i < BREEDER_MAX; i++){
		temp = pp_rand() % (BREEDER_MAX - i);
		temp2 = temp;
		for(t = 0; t < BREEDER_MAX; t++){
			if(brdno[t] != 0xff){
				if(temp2 == 0){
					sort[t] = i;
					brdno[t] = 0xff;
					break;
				}
				else
					temp2--;
			}
		}
	}
	
	for(i = 0; i < BREEDER_MAX; i++){
		apsys[i].next_sort = sort[i];
		apsys[i].nextsort_flg = 2;
	}
	apsys[CONWAZA->nowbrd].now_nextsort = 3;
	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL43_MSG);
}

//----------------------------------------------------------
//	�S�S�F�K������オ��
//	�R���e�X�g�^�C�v�Ɋ֌W�Ȃ��K������オ��B�e���V����+1�オ��B
//	�e���V�����{�[�i�X+10�Ⴆ��B
//----------------------------------------------------------
static void AbsoluteTensyonUp(void)
{
	if(ConWazaTbl[apsys[CONWAZA->nowbrd].use_waza].con_type == ContestKind)
		return;	//���ɑ�������v���Ă���̂ŁA�����̏����͖���
	
	apsys[CONWAZA->nowbrd].tensyon_up = 1;
//	AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL43_MSG);
}

//----------------------------------------------------------
//	�S�T�F�A�s�[���𔼕��Ɍ��炷
//	�����̑O�ɃA�s�[�������|�P�����S�ẴA�s�[���]���𔼕��Ɍ��炷�B
//	�O�ȉ��̑���ɑ΂��Ă�10���炷�B
//----------------------------------------------------------
static void AppealHalf(void)
{
	int i;
	u8 flg, temp = 0;
	
	for(i = 0; i < BREEDER_MAX; i++){
		if(CONWAZA->Sort[CONWAZA->nowbrd] > CONWAZA->Sort[i]){
			if(apsys[i].ap > 0){
				CONWAZA->bougai = apsys[i].ap / 2;
				CONWAZA->bougai = HasuuKiriage(CONWAZA->bougai);
			}
			else
				CONWAZA->bougai = 10;
			CONWAZA->target[0] = i;
			CONWAZA->target[1] = 0xff;
			flg = BougaiCalc();
			if(flg)
				temp++;
		}
	}

	if(temp == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
	AppealKoukaMsgSet(CONWAZA->nowbrd, BIKKURISASEYOUTOSITA_MSG);
}

//----------------------------------------------------------
//	�S�U�F�e���V�����ŕω�
//	�A�s�[�������Ƃ��̃e���V�����ɂ���ĕ]���l���ω�����B
//	�i�e���V�����F0��10�A1��20�A�Q��30�A3��50�A4��60�j
//----------------------------------------------------------
static void TensyondeHenka(void)
{
	u8 ap;
	
	if(consys->tensyon == 0){
		ap = 10;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_1_MSG);
	}
	else if(consys->tensyon == 1){
		ap = 20;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_2_MSG);
	}
	else if(consys->tensyon == 2){
		ap = 30;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_3_MSG);
	}
	else if(consys->tensyon == 3){
		ap = 50;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_4_MSG);
	}
	else{
		ap = 60;
		AppealKoukaMsgSet(CONWAZA->nowbrd, APPEAL34_5_MSG);
	}
	apsys[CONWAZA->nowbrd].ap = ap;
}

//----------------------------------------------------------
//	�S�V�F�e���V������~
//	�A�s�[����A���ȍ~�̃|�P�����̃A�s�[���ŁA�e���V�������ω����Ȃ��Ȃ�B
//	���̃^�[�����I������܂Ō��ʂ͌p���B
//----------------------------------------------------------
void TensyonStop(void)
{
	if(ccw->tensyon_stop)
		return;		//���Ƀe���V������~�t���O�������Ă���Ȃ珈���͂��Ȃ�
		
	ccw->tensyon_stop = 1;
	ccw->tensyon_stop_brd = CONWAZA->nowbrd;
	AppealKoukaMsgSet(CONWAZA->nowbrd, TYUUMOKUWOATUMETA_MSG);
}


//=========================================================================
//	���ʌv�Z��
//=========================================================================
//----------------------------------------------------------
//	�����̑O�ɱ�߰ق����߹�݂̒��ŁA�w�肵���^�C�v�Ɠ����Ȃ�α�߰��߲�Ă����炷
//	����: ���Z�ΏۂƂ���Z�^�C�v���w��
//----------------------------------------------------------
static void SpecTypeSame(u8 type)
{
	int i, flg, temp = 0;
	
	for(i = 0; i < BREEDER_MAX; i++){
		if(CONWAZA->Sort[CONWAZA->nowbrd] > CONWAZA->Sort[i]){
			if(type == ConWazaTbl[apsys[i].use_waza].con_type)
				CONWAZA->bougai = 40;
			else
				CONWAZA->bougai = 10;
			CONWAZA->target[0] = i;
			CONWAZA->target[1] = 0xff;
			flg = BougaiCalc();
			if(flg)
				temp++;
		}
	}
	if(temp == 0)
		AppealKoukaMsgSet2nd(CONWAZA->nowbrd, DEMOSIPPAI_MSG);
}


//=========================================================================
//	�c�[����
//=========================================================================
//----------------------------------------------------------
//	�W�Q��߰ق��������V�[���h������
//	����:	�Ώۂ���ذ�ްNo
//	�߂�l: 0=�����Ȃ�	1=����
//----------------------------------------------------------
static u8 ShieldCheck(u8 target)
{
	//����������������͕K���W�Q�����ޯĂɂȂ��Ă��Ȃ̂�
	CONWAZA->nowwaza_target[target] = 1;

	if(apsys[target].Ashield_flg){	//��Ζh��
		AppealKoukaMsgSet(target, APPEAL5_MSG);
		return 0;
	}
	if(apsys[target].shield_flg){	//�V�[���h�̑ϋv���������h��
		apsys[target].shield_flg--;
		AppealKoukaMsgSet(target, APPEAL4_MSG);
		return 0;
	}
	if(apsys[target].endAppeal_flg)	//��߰ق��I�����Ă�
		return 0;
	if(apsys[target].yasumi_turn)// | apsys[target].miss_flg)	//�ْ�or�x��
		return 0;
	return 1;
}

//----------------------------------------------------------
//	�W�Q���ʂ̌v�Z
// �߂�l: 1=�N����l�ɂł��W�Q����������	0=�N�ɂ��W�Q�ł��Ȃ�����
//----------------------------------------------------------
static u8 BougaiCalc(void)
{
	u8 target;
	int i;
	s16 buff[BREEDER_MAX];
//	s32 bougai, temp;
	
	memset(buff, 0, sizeof(buff));
	for(i = 0; CONWAZA->target[i] != 0xff; i++){
		target = CONWAZA->target[i];
		
		//�h��n
		if(ShieldCheck(target) == 0)
			continue;
		
		
		CONWAZA->bougai_calc = CONWAZA->bougai;
	
		//�W�Q�l�όn
		if(apsys[target].bougai2bai)	//�W�Q�l2�{
			CONWAZA->bougai_calc *= 2;
		if(apsys[target].invalid10){		//�W�Q�l��10�ɂ���
			CONWAZA->bougai_calc = 10;
			AppealKoukaMsgSet(target, APPEAL7_MSG);
			continue;
		}
		CONWAZA->bougai_calc -= apsys[target].invalid;	//�W�Q�l�����Z
		if(CONWAZA->bougai_calc <= 0){
			CONWAZA->bougai_calc = 0;	//���Z��ϲŽ�ɂȂ�����0�ɂ���
			AppealKoukaMsgSet(target, APPEAL6_MSG);
			continue;
		}
		
		//��߰��߲�ĂɖW�Q�l�������Čv�Z
		APSub(target, CONWAZA->bougai_calc);
		//���b�Z�[�W�Z�b�g
		BougaiMsgSet(target, CONWAZA->bougai_calc);
		buff[target] = CONWAZA->bougai_calc;
	}
	
	for(i = 0; i < BREEDER_MAX; i++){
		if(buff[i] != 0)
			return 1;
	}
	return 0;
}

//----------------------------------------------------------
//	�W�Q�ɂ���Ďw����ذ�ް��AP�l��������
//	brd = �W�Q���󂯂���ذ�ް	bougai = �W�Q�߲��
//----------------------------------------------------------
static void APSub(u8 brd, u8 bougai)
{
	apsys[brd].ap -= bougai;
	apsys[brd].bougai += bougai;
}

//----------------------------------------------------------
//	�]���l�[���؎̂�
//----------------------------------------------------------
static s16 HasuuKirisute(s16 ap)
{
	s16 hasuu;
	
	hasuu = abs(ap) % 10;
	if(ap < 0){
		if(hasuu != 0)
			ap -= 10 - hasuu;
	}
	else
		ap -= hasuu;
	return ap;
}

//----------------------------------------------------------
//	�W�Q�[���؏グ
//----------------------------------------------------------
static s16 HasuuKiriage(s16 bougai)
{
	s16 hasuu;
	
	hasuu = abs(bougai) % 10;
	if(hasuu != 0)
		bougai += 10 - hasuu;
	return bougai;
}


#endif	//--------- �Z�v�Z�� ----------------------------------

