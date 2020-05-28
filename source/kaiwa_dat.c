/***********************************************************************
**	�ȈՉ�b�f�[�^
***********************************************************************/
#include "kaiwa_com.h"
#include "kaiwa.dat"
#include "kaiwa.h"
#include "poketool.h"
#include "tv_def.h"
#include "script.h"
#include "pm_str.h"
#include "fld_talk.h"
#include "zukan.h"
#include "sysflag.h"
#include "monsno.def"
#include "wazano.def"


/*====================================================================*/
/* �v���g�^�C�v                                                       */
/*====================================================================*/
static u16 get_known_poke_code(void);
static u8 is_unknown_code(u16 code);

/*====================================================================*/
/* �}�N��                                                             */
/*====================================================================*/
#define IS_EX_CATEGORY(c)   ((c)==KWC_POKEMON||(c)==KWC_POKEMON2||(c)==KWC_WAZA1||(c)==KWC_WAZA2)

/*====================================================================*/
/* �J�e�S����                                                         */
/*====================================================================*/
static const u8 kwc_pokemon[] = {  PO_, KE_, MO_,  N_,EOM_, };  // �|�P����
static const u8 kwc_trainer[] = {  TO_, RE_,bou_, NA_,bou_,EOM_, };  // �g���[�i�[
static const u8 kwc_status[] = {  SU_, TE_,bou_, TA_, SU_,EOM_, };  // �X�e�[�^�X
static const u8 kwc_battle[] = {  BA_, TO_, RU_,EOM_, };  // �o�g��
static const u8 kwc_aisatu[] = {   a_,  i_, sa_, tu_,EOM_, };  // ��������
static const u8 kwc_hito[] = {  hi_, to_,EOM_, };  // �Ђ�
static const u8 kwc_koe[] = {  ko_,  e_,EOM_, };  // ����
static const u8 kwc_kaiwa[] = {  ka_,  i_, wa_,EOM_, };  // ������
static const u8 kwc_musubi[] = {  mu_, su_, bi_,EOM_, };  // �ނ���
static const u8 kwc_kimoti[] = {  ki_, mo_, ti_,EOM_, };  // ������
static const u8 kwc_joutai[] = {  zi_,yyo_,  u_, ta_,  i_,EOM_, };  // ���傤����
static const u8 kwc_koudou[] = {  ko_,  u_, do_,  u_,EOM_, };  // �����ǂ�
static const u8 kwc_seikatu[] = {  se_,  i_, ka_, tu_,EOM_, };  // ��������
static const u8 kwc_syumi[] = {  si_,yyu_, mi_,EOM_, };  // �����
static const u8 kwc_jikan[] = {  zi_, ka_,  n_,EOM_, };  // ������
static const u8 kwc_arekore[] = {   a_, re_, ko_, re_,EOM_, };  // ���ꂱ��
static const u8 kwc_kurikaesi[] = {  ku_, ri_, ka_,  e_, si_,EOM_, };  // ���肩����
static const u8 kwc_event[] = {   I_, BE_,  N_, TO_,EOM_, };  // �C�x���g
static const u8 kwc_waza1[] = {  wa_, za_, n1_,EOM_, };  // �킴�P
static const u8 kwc_waza2[] = {  wa_, za_, n2_,EOM_, };  // �킴�Q
static const u8 kwc_now[] = {  NA_,  U_,  I_, ko_, to_, ba_,EOM_, };  // �i�E�C���Ƃ�
static const u8 kwc_pokemon2[] = {  PO_, KE_, MO_,  N_, n2_,EOM_, };  // �|�P�����Q

static const u8 * const kwc_tbl[] = {
kwc_pokemon,
kwc_trainer,
kwc_status,
kwc_battle,
kwc_aisatu,
kwc_hito,
kwc_koe,
kwc_kaiwa,
kwc_musubi,
kwc_kimoti,
kwc_joutai,
kwc_koudou,
kwc_seikatu,
kwc_syumi,
kwc_jikan,
kwc_arekore,
kwc_kurikaesi,
kwc_event,
kwc_waza1,
kwc_waza2,
kwc_now,
kwc_pokemon2,
};
extern	const	u8	WazaNameData[][8];

static const u8 UnknownCode[] = { hate_, hate_, hate_, EOM_ };

/**********************************************************************
**	�J�e�S�������o�b�t�@�ɃR�s�[
**
**	dst       �R�s�[��o�b�t�@
**	category  �J�e�S���i���o�[
**  spc_len   != 0 �Ȃ�A�w��̒��������󔒂Ŗ��߂�
**
**  return:   �R�s�[��� EOM_ �A�h���X
**
**********************************************************************/
u8* KaiwaCopyCategoryName(u8 *dst, u8 category, u16 spc_len)
{
	u16  len;
	u8 *cp;

	cp = PM_strcpy(dst, kwc_tbl[category]);
	len = (u16)(cp - dst);
	while(len < spc_len){
		*cp++ = spc_;
		len++;
	}
	*cp = EOM_;
	return cp;
}
/**********************************************************************
**	�w�肳�ꂽ�J�e�S�����g���邩�ǂ����t���O�`�F�b�N
**********************************************************************/
u8 KaiwaIsEnableCategory(u8 category)
{
	switch(category){
	case KWC_NOW:
		return EventFlagCheck(SYS_NOW_OYAJI_MEET);

	case KWC_EVENT:
	case KWC_WAZA1:
	case KWC_WAZA2:
		return EventFlagCheck(SYS_GAME_CLEAR);

	case KWC_POKEMON2:
		return KaiwaPoke2CategoryCheck();

	default:
		return 1;
	}
}
/**********************************************************************
**	�w�肳�ꂽ�P�ꂪ�g���邩�ǂ����t���O�`�F�b�N
**********************************************************************/
static u8 is_enable_word(u8 category_no, u16 word_no)
{
	switch(category_no){
	case KWC_NOW:
		if(KaiwaWork->hidden_category_flg[category_no-KWC_BORDER] == 0)
		{
			return 0;
		}
		return NowWordFlagCheck(word_no);

	case KWC_POKEMON:
		return ZukanCheck(PokeMonsNo2ZukanNoGet(word_no), Z_SEE_CHK);

	case KWC_EVENT:
	case KWC_WAZA1:
	case KWC_WAZA2:
		if(KaiwaWork->hidden_category_flg[category_no-KWC_BORDER] == 0)
		{
			return 0;
		}
		break;

	case KWC_POKEMON2:
		return KaiwaPoke2CategoryCheck();

	default:
		break;
	}
	return 1;
}
/**********************************************************************
**	�w�肳�ꂽ�J�e�S�����ŗL���ȒP�ꐔ��Ԃ�
**	�i�J�e�S�����̂������Ȃ�O�j
**********************************************************************/
u16 KaiwaEnableWordCnt(u8 category)
{
	switch(category){
	case KWC_POKEMON:
		return ZukanHouenTotal(Z_SEE_CHK);

	case KWC_NOW:
		return KnownNowWordCnt();

	default:
		if(KaiwaIsEnableCategory(category))
		{
			return CategoryWordCntTbl[category];
		}
		else
		{
			return 0;
		}
	}
}

/**********************************************************************
**	�P��\�[�g�p�e�[�u����������
**********************************************************************/
void KaiwaInitWordSort(void)
{
	u16 i, p, j, begin, end;
	u16 category, word_len, code;

	for(i = 0; i < KWC_MAX; i++){
		KaiwaWork->charsort_work[i] = 0;
	}

	for(i = 0; i < KAIWA_INITIAL_CNT; i++){
		begin = KaiwaCharSortIndexTbl[i];
		end = KaiwaCharSortIndexTbl[i+1];
		p = begin;
		j = 0;
		KaiwaWork->initial_word_cnt[i] = 0;
		while(p < end){
			category = KaiwaCharSortTbl[p++];

			// �d������
			while(category == 0xff){
				unsigned char dup_cnt, find_enable = 0;
				dup_cnt = KaiwaCharSortTbl[p++];
				while(dup_cnt--){
					category = KaiwaCharSortTbl[p++];
					if(!find_enable){
						if(IS_EX_CATEGORY(category)){
							const u16 *tbl;
							tbl = (const u16*)(KaiwaWordTbl[category]);
							code = tbl[KaiwaWork->charsort_work[category]];
						}else{
							code = KaiwaWork->charsort_work[category];
						}
						if(is_enable_word(category, code)){
							KaiwaWork->charsort_tbl[i][j++] = MAKE_CODE(category,code);
							KaiwaWork->initial_word_cnt[i]++;
							find_enable = 1;
						}
					}
					KaiwaWork->charsort_work[category]++;
				}
				if(p >= end){ break; }
				category = KaiwaCharSortTbl[p++];
			}
			if(p >= end){ break; }

			word_len = KaiwaCharSortTbl[p++];

			if(IS_EX_CATEGORY(category)){
				const u16 *tbl;
				tbl = (const u16*)(KaiwaWordTbl[category]);
				while(word_len--){
					if(is_enable_word(category, tbl[KaiwaWork->charsort_work[category]])){
						code = MAKE_CODE(category, tbl[KaiwaWork->charsort_work[category]]);
						KaiwaWork->charsort_tbl[i][j++] = code;
						KaiwaWork->initial_word_cnt[i]++;
					}
					KaiwaWork->charsort_work[category]++;
				}
			}else{
				code = MAKE_CODE(category, KaiwaWork->charsort_work[category]);
				while(word_len--){
					if(is_enable_word(category, KaiwaWork->charsort_work[category])){
						KaiwaWork->charsort_tbl[i][j++] = code;
						KaiwaWork->initial_word_cnt[i]++;
					}
					KaiwaWork->charsort_work[category]++;
					code++;
				}
			}
		}
	}
}
/**********************************************************************
**	�P�ꂲ�Ƃ̕�����|�C���^�e�[�u��������
**********************************************************************/
void KaiwaInitWordPointerTbl(void)
{
	const u8 *cp;
	u16 i, cnt;

	for(i = 0; i < KWC_MAX; i++){
		if(!IS_EX_CATEGORY(i)){
			cp = KaiwaWordTbl[i];
			cnt = 0;
			while(*cp != EOM_){
				KaiwaWork->word_tbl[i][cnt] = cp;
				while(*cp != EOM_){ cp++; }
				cp++;
				cnt++;
			}
		}
	}
}
/**********************************************************************
**	�P��I���E�B���h�E�̃f�[�^�z���������
**********************************************************************/
void KaiwaInitWordWindowData(void)
{
	// �J�e�S�����[�h
	if(KaiwaWork->select_mode == KAIWA_SEL_CATEGORY){
		u16 category, i, j, c;
		category = KaiwaWork->select_group_num;
		KaiwaWork->category_word_cnt[category] = 0;
		i = j = 0;
		if(IS_EX_CATEGORY(category)){
			const u16 *tbl = (u16*)KaiwaWordTbl[category];
			for(c = 0; c < CategoryWordCntTbl[KaiwaWork->select_group_num]; c++)
			{
				if(is_enable_word(category, tbl[c]))
				{
					KaiwaWork->word_code_tbl[i][j] = MAKE_CODE(category, tbl[c]);
					KaiwaWork->category_word_cnt[category]++;
					if(++j > 2){
						j = 0;
						i++;
					}
				}
			}
		}else{
			for(c = 0; c < CategoryWordCntTbl[KaiwaWork->select_group_num]; c++)
			{
				if(is_enable_word(category, c))
				{
					KaiwaWork->word_code_tbl[i][j] = MAKE_CODE(category, c);
					KaiwaWork->category_word_cnt[category]++;
					if(++j > 2){
						j = 0;
						i++;
					}
				}
			}
		}
	// �������������[�h
	}else{
		u16 initial, i, j, c;

		initial = KaiwaWork->select_group_num;
		i = j = 0;
		for(c = 0; c < KaiwaWork->initial_word_cnt[initial]; c++)
		{
			KaiwaWork->word_code_tbl[i][j] = KaiwaWork->charsort_tbl[initial][c];
			if(++j > 2)
			{
				j = 0;
				i++;
			}
		}
	}
}
/**********************************************************************
**	�ȈՉ�b�R�[�h���P�ꕶ����ɕϊ�
**
**	dst   ������p�A�h���X
**	code  ��b�R�[�h
**  space_len   �w��̒����܂ŋ󔒂Ŗ��߂�
**
**  �߂�l  EOM_�A�h���X
**
**********************************************************************/
u8 * KaiwaCodeToWord(u8 *dst, u16 code, u16 space_len)
{
	u16 category, word, len;
	u8 *cp;

	if(is_unknown_code(code)){
		return PM_strcpy(dst, UnknownCode);
	}

	if(code == KAIWA_CODE_BLANK){
		*dst = EOM_;
		return dst;
	}

	category = CODE_TO_CATEGORY(code);
	word = CODE_TO_WORD(code);
	switch(category){
	case KWC_POKEMON:
	case KWC_POKEMON2:
		cp = PM_strcpy(dst, PokeNameData[word]);
		break;

	case KWC_WAZA1:
	case KWC_WAZA2:
		cp = PM_strcpy(dst, WazaNameData[word]);
		break;

	default:
		cp = PM_strcpy(dst, KaiwaWork->word_tbl[category][word]);
		break;
	}

	len = (u16)(cp - dst);
	while(len < space_len){
		*cp++ = spc_;
		len++;
	}
	*cp = EOM_;
	return cp;
}

/**********************************************************************
**	�ȈՉ�b�R�[�h���P��̕������J�E���g
**
**	code    ��b�R�[�h
**  �߂�l  ������
**
**********************************************************************/
u16 KaiwaCodeLen(u16 code)
{
	u16 category, word, len;
	const u8 *cp;

	if(is_unknown_code(code)){
		return PM_strlen(UnknownCode);
	}

	if(code == KAIWA_CODE_BLANK){
		return 0;
	}

	category = CODE_TO_CATEGORY(code);
	word = CODE_TO_WORD(code);
	switch(category){
	case KWC_POKEMON:
	case KWC_POKEMON2:
		cp = PokeNameData[word];
		break;

	case KWC_WAZA1:
	case KWC_WAZA2:
		cp = WazaNameData[word];
		break;

	default:
		cp = KaiwaWork->word_tbl[category][word];
		break;
	}

	for(len = 0; *cp != EOM_; cp++, len++);

	return len;
}
/**********************************************************************
**	�Q�[�����ň�����ȈՉ�b�R�[�h���H
**�i�����ް�ޮ݂Ƃ̒ʐM�E�s������Ή��j
**
**	code    ��b�R�[�h
**  �߂�l  1:�����܂���  0:�����܂�
**
**********************************************************************/
static u8 is_unknown_code(u16 code)
{
	const u16 *tbl;
	u16 category, word, i;

	if(code == KAIWA_CODE_BLANK){
		return 0;
	}

	category = CODE_TO_CATEGORY(code);
	word = CODE_TO_WORD(code);

	if(category >= KWC_MAX) return 1;

	switch(category){
	case KWC_POKEMON:
	case KWC_POKEMON2:
	case KWC_WAZA1:
	case KWC_WAZA2:
		tbl = (const u16*)(KaiwaWordTbl[category]);
		for(i = 0; i < CategoryWordCntTbl[category]; i++)
		{
			if(word == tbl[i]){
				return 0;
			}
		}
		return 1;

	default:
		if(word >= CategoryWordCntTbl[category]){
			return 1;
		}
		break;
	}

	return 0;
}

/*====================================================================*/
/*  �O������̉�b�R�[�h��������ϊ�                                  */
/*====================================================================*/
// �P�P��
u8 *KaiwaCopyWord(u8 *dst, u16 code)
{
	u16 category, word;
	const u8 *cp;

	if(is_unknown_code(code)){
		return PM_strcpy(dst, UnknownCode);
	}

	if(code == KAIWA_CODE_BLANK){
		*dst = EOM_;
		return dst;
	}

	category = CODE_TO_CATEGORY(code);
	word = CODE_TO_WORD(code);

	switch(category){
	case KWC_POKEMON:
	case KWC_POKEMON2:
		dst = PM_strcpy(dst, PokeNameData[word]);
		break;

	case KWC_WAZA1:
	case KWC_WAZA2:
		dst = PM_strcpy(dst, WazaNameData[word]);
		break;

	default:
		cp = KaiwaWordTbl[category];
		while(word--){
			while(*cp++ != EOM_);
		}
		dst = PM_strcpy(dst, cp);
		break;
	}

	*dst = EOM_;
	return dst;

}
// �P����
u8* KaiwaCopySentence(u8 *dst, u16 *code, u16 rows, u16 cols)
{
	u16 x, y;

	rows--;
	for(y = 0; y < cols; y++){
		for(x = 0; x < rows; x++){
			dst = KaiwaCopyWord(dst, *code);
			if(*code != KAIWA_CODE_BLANK){
				*dst++ = spc_;
			}
			code++;
		}
		dst = KaiwaCopyWord(dst, *code++);
		*dst++ = CR_;
	}
	*(--dst) = EOM_;
	return dst;
}
// �����񒷂̎擾
u16 KaiwaWordLenGet(u16 code)
{
	const u8 *cp;
	u16 len, word, category;

	if(code == KAIWA_CODE_BLANK){
		return 0;
	}

	category = CODE_TO_CATEGORY(code);
	word = CODE_TO_WORD(code);

	switch(category){
	case KWC_POKEMON:
	case KWC_POKEMON2:
		cp = PokeNameData[word];
		break;

	case KWC_WAZA1:
	case KWC_WAZA2:
		cp = WazaNameData[word];
		break;

	default:
		cp = KaiwaWordTbl[category];
		while(word--){
			while(*cp++ != EOM_);
		}
		break;
	}

	for(len = 0; *cp != EOM_; cp++, len++);

	return len;
}
/***********************************************************************
**	��b�R�[�h�𕶎��񉻂������ɁA�w�蕶�������I�[�o�[����s�����邩�H
**	code   ��b�R�[�h�擪
**	rows   ��
**	cols   �s��
**	max    �P�s�������}�b�N�X
**
**	ret:  0:���ް���Ȃ�  1:���ް����
***********************************************************************/
u8 KaiwaLineOverCheck(const u16 *code, u8 rows, u8 cols, u16 max)
{
	u16 len;
	u8  x, y;

	for(y = 0; y < cols; y++){
		len = 0;
		for(x = 0; x < rows; x++){
			len += KaiwaWordLenGet(*code++);
		}
		if(len > max)
		{
			return 1;
		}
	}
	return 0;
}
/*====================================================================*/
/*  �s�u�f�[�^�p��b�R�[�h�i�[�G���A��������                          */
/*====================================================================*/
void KaiwaTVWorkInit(u8 mode, u16 num)
{
	u16 *addr, cnt;

	switch(mode){
	case KAIWA_MODE_HAGAKI:
		cnt = 6;
		addr = ((TV_Letter_Work*)(&(Fld.tv_event[num])))->easytalk;
		break;

	case KAIWA_MODE_INTERVIEW:
		cnt = 2;
		addr = &(((TV_Club_Work*)(&(Fld.tv_event[num])))->easytalk_no[0]);
		break;

	case KAIWA_MODE_GYMREADER:
		cnt = 1;
		addr = &(((TV_GYM_Work*)(&(Fld.tv_event[num])))->easytalk_no);
		break;

	default:
		return;
	}

	while(cnt--){
		*addr++ = KAIWA_CODE_BLANK;
	}
}
void KaiwaWorkInit(u16 *addr, u16 size)
{
	while(size--){
		*addr++ = KAIWA_CODE_BLANK;
	}
}

/*====================================================================*/
/*  �w�肳�ꂽ�J�e�S���̒P��R�[�h���P�����_���ɕԂ�                */
/*====================================================================*/
u16 KaiwaGetRandomWord(u16 category)
{
	u16 word;

	word = pp_rand() % CategoryWordCntTbl[category];

	if(IS_EX_CATEGORY(category))
	{
		const u16 *tbl;

		tbl = (const u16*)(KaiwaWordTbl[category]);
		word = tbl[word];
	}

	return MAKE_CODE(category, word);
}
/*====================================================================*/
/*  �w�肳�ꂽ�J�e�S���̒�����A���͂Ŏg����P��������_���ɕԂ�      */
/*====================================================================*/
u16 KaiwaGetRandomKnownWord(u16 category)
{
	if(KaiwaIsEnableCategory(category) == 0){
		return KAIWA_CODE_BLANK;
	}

	switch(category){
	case KWC_NOW:
		return NowWordGetKnown();

	case KWC_POKEMON:
		return get_known_poke_code();

	default:
		return KaiwaGetRandomWord(category);
	}
}
/*====================================================================*/
/*  �X�N���v�g�Ƃ̂��Ƃ�                                            */
/*====================================================================*/
/*
	���ȏЉ�A�퓬�J�n�A�����A�������b�Z�[�W��
	���b�Z�[�W�p�o�b�t�@�ɃR�s�[
*/
void KaiwaCodeToMsgBuffer(void)
{
	u16 *code_p, col, row;
	u8 *cp;

	switch(ScriptParameter0){
	case KAIWA_MODE_SELF:
		code_p = Fld.kaiwa_self;
		col = 2, row = 2;
		break;

	case KAIWA_MODE_BATTLE_START:
		code_p = Fld.kaiwa_battle;
		if(KaiwaLineOverCheck(code_p, 3, 2, 20)==0){
			col = 3; row = 2;
		}else{
			col = 2; row = 3;
		}
		break;

	case KAIWA_MODE_BATTLE_WON:
		code_p = Fld.kaiwa_win;
		col = 3; row = 2;
		break;

	case KAIWA_MODE_BATTLE_LOST:
		code_p = Fld.kaiwa_lose;
		col = 3; row = 2;
		break;

	default:
		return;
	}

	cp = KaiwaCopySentence(MsgExpandBuffer, code_p, col, row);
//	*(cp++) = NORMAL_WAIT_;
//	*cp = EOM_;
	FieldMsgSetNoSkip(MsgExpandBuffer);
}
/***********************************************************************
**	�P��������_���Ƀo�b�t�@�R�s�[
***********************************************************************/
void KaiwaCopyRandomWord(void)
{
	u16 r;

	r = ((pp_rand() & 1)? KWC_SYUMI : KWC_SEIKATU);
	r = KaiwaGetRandomKnownWord(r);
	KaiwaCopyWord(StrTempBuffer1, r);
}



/*====================================================================*/
/*  �B���P��E�J�e�S���Ɋւ���Z�[�u�̈�𑀍삷��                    */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/*====================================================================*/
#define NOW_BYTE_OFS         (0)   // kaiwa_word_flag[8]�̉��o�C�g�ڂ���g�����H
#define NOW_WORD_MAX         (33)  // �P�ꐔ�ő�


#define POKE2_BYTE_OFS       (7)   // kaiwa_word_flag[8]�̉��o�C�g�ڂ���g�����H

/*===============================================*/
/*  �i�E�����Ƃ΃t���O�`�F�b�N                   */
/*===============================================*/
u8 NowWordFlagCheck(u8 word_no)
{
	u8 byte, bit;

	byte = word_no / 8;
	bit = word_no & 7;

	return (Fld.kaiwa_word_flag[byte] & (1 << bit)) != 0;
}
/*===============================================*/
/*  �i�E�����Ƃ΃t���O�Z�b�g                     */
/*===============================================*/
void NowWordFlagSet(u8 word_no)
{
	u8 byte, bit;

	if(word_no < NOW_WORD_MAX){

		byte = word_no / 8;
		bit = word_no & 7;

		Fld.kaiwa_word_flag[byte] |= (1 << bit);
	}
}
/*===============================================*/
/*  �o���Ă���i�E�����Ƃ΂̑���                 */
/*===============================================*/
u8 KnownNowWordCnt(void)
{
	u8 i, cnt;

	for(i = 0, cnt = 0; i <NOW_WORD_MAX; i++)
	{
		if(NowWordFlagCheck(i)){ cnt++; }
	}
	return cnt;
}
/*===============================================*/
/*  �o���Ă��Ȃ��i�E�C���t�������_���ɕԂ�       */
/*�i�S���o���Ă���Ƃ��� KAIWA_CODE_BLANK�j      */
/*===============================================*/
u16 NowWordGetUnknown(void)
{
	u16 no, cnt, i;

	cnt = KnownNowWordCnt();
	if(cnt == NOW_WORD_MAX){ return KAIWA_CODE_BLANK; }

	no = pp_rand() % (NOW_WORD_MAX - cnt);
	for(i = 0; i < NOW_WORD_MAX; i++){
		if(!NowWordFlagCheck(i)){
			if(no == 0){
				NowWordFlagSet(i);
				return MAKE_CODE(KWC_NOW, i);
			}
			no--;
		}
	}
	return KAIWA_CODE_BLANK;
}
/*===============================================*/
/*  �o���Ă���i�E�C���t�������_���ɕԂ�         */
/*�i�o���Ă��Ȃ��Ƃ��� KAIWA_CODE_BLANK)         */
/*===============================================*/
u16 NowWordGetKnown(void)
{
	u16 cnt, i;

	cnt = KnownNowWordCnt();
	if(cnt == 0){ return KAIWA_CODE_BLANK; }

	cnt = pp_rand() % cnt;
	for(i = 0; i < NOW_WORD_MAX; i++){
		if(NowWordFlagCheck(i)){
			if(cnt == 0){
				return MAKE_CODE(KWC_NOW, i);
			}
			cnt--;
		}
	}
	return KAIWA_CODE_BLANK;
}
/*===============================================*/
/*  �|�P�����Q�t���O�`�F�b�N                     */
/*===============================================*/
u8 KaiwaPoke2CategoryCheck(void)
{
	return ZukanSpecialFlagCheck();
//	return (Fld.kaiwa_word_flag[POKE2_BYTE_OFS] != 0);
}
/*===============================================*/
/*  �������Ƃ̂���|�P�������̉�b�R�[�h��       */
/*  �����_���ŕԂ�                               */
/*===============================================*/
static u16 get_known_poke_code(void)
{
	const u16 *tbl;
	u16 cnt, i, z_no;

	cnt = KaiwaEnableWordCnt(KWC_POKEMON);
	if(cnt == 0){ return KAIWA_CODE_BLANK; }
	cnt = pp_rand() % cnt;

	tbl = (const u16*)(KaiwaWordTbl[KWC_POKEMON]);

	for(i = 0; i < CategoryWordCntTbl[KWC_POKEMON]; i++){
		z_no = PokeMonsNo2ZukanNoGet(*tbl);
		if(ZukanCheck(z_no, Z_SEE_CHK))
		{
			if(cnt == 0){
				return MAKE_CODE(KWC_POKEMON, *tbl);
			}
			cnt--;
		}
		tbl++;
	}
	return KAIWA_CODE_BLANK;
}
