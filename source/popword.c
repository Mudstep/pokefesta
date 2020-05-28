/***********************************************************************
**	���s��f�[�^
***********************************************************************/
#include "common.h"
#include "pm_str.h"
#include "kaiwa.h"
#include "calctool.h"
#include "ev_flag.h"
#include "popword.h"
#include "pokesio.h"
#include "sysflag.h"
#include "kaiwa_code.h"

/*======================================
==	�萔
======================================*/
#define MIX_DATA_SIZE    (sizeof(Fld.PopWord))                // �ʐM���̃f�[�^�T�C�Y
#define MIX_TMP_BUFFER   (PopularWord*)(&(UserWork[0x7800]))  // �ʐM���ɑS�f�[�^���R�s�[���Ă����ꎞ�o�b�t�@
#define MIX_SORT_BUFFER  (PopularWord*)(&(UserWork[0x7900]))  // �ʐM���\�[�g�Ɏg���o�b�t�@

/*======================================
==	�����֐��錾
======================================*/
static void set_rand_power(PopularWord *word);
static u8 is_greater(PopularWord *word1, PopularWord *word2, u8 type);
static u8 is_dup_regist_word(u16 *word);
static u8 is_same_word(u16 *word1, u16 *word2);
static s16 check_mix_dup_word(PopularWord *pop, u16 record);

/*====================================================================*/
/*	���s��f�[�^�������i�Q�[���J�n���j                                */
/*====================================================================*/
void PopWord_Init(void)
{
	u16 i;
	for(i = 0; i < POPWORD_MAX; i++){
		Fld.PopWord[i].word[0] = KaiwaGetRandomWord(KWC_JOUTAI);
		if(pp_rand() & 1){
			Fld.PopWord[i].word[1] = KaiwaGetRandomWord(KWC_SEIKATU);
		}else{
			Fld.PopWord[i].word[1] = KaiwaGetRandomWord(KWC_SYUMI);
		}

		Fld.PopWord[i].powerup_flag = pp_rand() & 1;

		set_rand_power(&(Fld.PopWord[i]));
	}
	PopWord_Sort(Fld.PopWord, POPWORD_MAX, POPSORT_NOWPOWER);
}
/*====================================================================*/
/*	���s��f�[�^�X�V�i�P�����Ƃ̏���N�����j                          */
/*                                                                    */
/*  days   ... �����������߂邩�H                                     */
/*                                                                    */
/*====================================================================*/
#define POW_RATIO  (5)   // �P��������̑�����
void PopWord_Update(u16 days)
{
	u16 i;
	u32 tmp, rem, pow, Power;
	PopularWord  *data;

	if(days == 0){ return; }

	Power = POW_RATIO * days;

	for(i = 0; i < POPWORD_MAX; i++){
		pow = Power;

		data = &(Fld.PopWord[i]);
		if(data->powerup_flag == 0){
			if((u16)(data->now_power) >= (u16)pow){
				data->now_power -= pow;
				if(data->now_power==0){
					data->powerup_flag=1;
				}
				continue;
			}
			pow -= data->now_power;
			data->now_power = 0;
			data->powerup_flag = 1;
		}

		tmp = data->now_power + pow;
		if((u16)tmp > (u16)(data->max_power)){
			rem = tmp % data->max_power;
			tmp /= data->max_power;
			data->powerup_flag = !(tmp & 1);
			if(data->powerup_flag){
				data->now_power = rem;
			}else{
				data->now_power = data->max_power - rem;
			}
		}else{
			data->now_power = tmp;
			if(data->now_power == data->max_power){
				data->powerup_flag = 0;
			}
		}
	}
	PopWord_Sort(Fld.PopWord, POPWORD_MAX, POPSORT_NOWPOWER);
}
/*====================================================================*/
/*	�V�K�ɓ��͂��ꂽ���s��̔\�͒l���聕�o�^                          */
/*                                                                    */
/*  word   �ȈՉ�b�f�[�^                                             */
/*                                                                    */
/*  �߂�l   1 ... �ŐV���s��Ƃ��ēo�^���ꂽ                         */
/*           0 ... ���ݗ��s��Ƃ��ēo�^���ꂽ                         */
/*                                                                    */
/*====================================================================*/
u8 PopWord_Add(u16 *word)
{
	PopularWord tmp = {};
	u16 i, j;

	// �d�����Ă�����o�^���Ȃ�
	if(is_dup_regist_word(word)){
		return 0;
	}

	// ���R�[�h�R�[�i�[�ʐM���o���ŁA���߂ē��͂���ꍇ��
	// �������ɍŐV���s��Ƃ��ď㏑������
	if(EventFlagCheck(SYS_POPWORD_INPUT) == 0){
		EventFlagSet(SYS_POPWORD_INPUT);
		if(EventFlagCheck(SYS_MIX_RECORD) == 0){
			Fld.PopWord[0].word[0] = word[0];
			Fld.PopWord[0].word[1] = word[1];
			return 1;
		}
	}

	// �ȉ��A�ʏ폈��
	tmp.word[0] = word[0];
	tmp.word[1] = word[1];
	tmp.powerup_flag = 1;
	set_rand_power(&tmp);

	for(i = 0; i < POPWORD_MAX; i++){
		if(is_greater(&tmp, Fld.PopWord + i, POPSORT_NOWPOWER)){
			for(j = POPWORD_MAX-1; j > i; j--){
				Fld.PopWord[j] = Fld.PopWord[j-1];
			}
			Fld.PopWord[i] = tmp;
			return (i == 0);
		}
	}
	// �ŉ��ʂɂ͕K�������
	Fld.PopWord[POPWORD_MAX-1] = tmp;

	return 0;
}
/*====================================================================*/
/*	���s��f�[�^��\�͒l�ō~���Ƀ\�[�g                                */
/*                                                                    */
/*  word  PopularWord �\���̂̔z��                                    */
/*  max   �f�[�^����                                                  */
/*  type  �\�[�g�^�C�v�ienum POPWORD_SORT_TYPE�j                      */
/*                                                                    */
/*====================================================================*/
void PopWord_Sort(PopularWord *word, u16 max, u8 type)
{
	u16 i, j;
	PopularWord tmp;

	for(i = 0; i < max; i++){
		for(j = i + 1; j < max; j++){
			if(is_greater(word+j, word+i, type)){
				tmp = word[j];
				word[j] = word[i];
				word[i] = tmp;
			}
		}
	}
}
/*====================================================================*/
/*	���s��f�[�^��������i���R�[�h�R�[�i�[�ʐM�j                      */
/*                                                                    */
/*  adrs   ���s��f�[�^�z��̃A�h���X                                 */
/*  size   ���̔z��܂ł̃I�t�Z�b�g                                   */
/*  id     �ʐM�h�c                                                   */
/*                                                                    */
/*====================================================================*/
void PopWordMixer(void *adrs, u32 size, u8 id)
{
	PopularWord *to, *from, *dup;
	u16 members, record, i, j;
	s16 dup_no;

	// �����P��g�ݍ��킹���d�����Ȃ��悤��
	// �S�����o�[�̃f�[�^���\�[�g�p�o�b�t�@�ɃR�s�[
	members = GetConnectSioMenberNum();

	for(i = 0; i < members; i++){
		memcpy((u8*)MIX_TMP_BUFFER+i*MIX_DATA_SIZE, (u8*)adrs+i*size, MIX_DATA_SIZE);
	}

	from = (PopularWord*)MIX_TMP_BUFFER;
	to = MIX_SORT_BUFFER;
	record = 0;

	for(i = 0; i < members; i++){
		for(j = 0; j < POPWORD_MAX; j++){
			dup_no = check_mix_dup_word(from, record);
			if(dup_no < 0){
				*to++ = *from;
				record++;
			}else{
				dup = MIX_SORT_BUFFER + dup_no;
				if(dup->now_power < from->now_power){
					*dup = *from;
				}
			}
			from++;
		}
	}

	// �\�[�g��A��ʃf�[�^�������̃f�[�^�Ƃ���
	PopWord_Sort(MIX_SORT_BUFFER, record, POPSORT_MIXER);

	from = MIX_SORT_BUFFER;
	to = Fld.PopWord;

	for(i = 0; i < POPWORD_MAX; i++){
		*to++ = *from++;
	}
}
/*====================================================================*/
/*	���s�ꕶ����� StrTempBuffer0 �ɃR�s�[                            */
/*                                                                    */
/*  ���X�N���v�g�Ή�   ScriptParameter0 ... ���Ԗڂ̗��s�ꂩ�H        */
/*                                                                    */
/*====================================================================*/
void PopWord_SetBuffer(void)
{
	PopularWord *pop = &(Fld.PopWord[ScriptParameter0]);
	KaiwaCopySentence(StrTempBuffer0, pop->word, 2, 1);
}
/*====================================================================*/
/*	���݂̗��s�ꂪ�A�܂��Ȃ��ւ�肻�����ǂ����`�F�b�N                */
/*                                                                    */
/*  ���X�N���v�g�Ή�   AnswerWork  0 ... �܂��^1...�ւ�肻��         */
/*                                                                    */
/*====================================================================*/
void PopWord_CheckComingWord(void)
{
	u8 result = 0;

	do {
		s16 diff;

		diff = Fld.PopWord[0].now_power - Fld.PopWord[1].now_power;
		if(diff > 1){ break; }
		if(Fld.PopWord[0].powerup_flag){ break; }
		if(!(Fld.PopWord[1].powerup_flag)){ break; }

		result = 1;

	}while(0);

	AnswerWork = result;
}
/*====================================================================*/
/*	���s��g�ݍ��킹�ɂ���ĈقȂ鐔�l(3bit)��AnswerWork �ɃZ�b�g     */
/*                                                                    */
/*  ���X�N���v�g�Ή�                                                  */
/*                                                                    */
/*====================================================================*/
void PopWord_GetValue(void)
{
	AnswerWork = (Fld.PopWord[0].word[0] + Fld.PopWord[0].word[1]) & 0x07;
}


/**********************************************************************
**	�Q�̗��s��̂����A�ǂ��炪��ʂ�����
**
**  word1, word2  ���s��A�h���X
**  type          ����^�C�v�ienum POPWORD_SORT_TYPE�j
**
**  �߂�l   1 ... word1 �����   0 ... word2 �����
**
**********************************************************************/
static u8 is_greater(PopularWord *word1, PopularWord *word2, u8 type)
{
	switch(type){
	case POPSORT_NOWPOWER:
		if(word1->now_power > word2->now_power) return 1;
		if(word1->now_power < word2->now_power) return 0;
		if(word1->max_power > word2->max_power) return 1;
		if(word1->max_power < word2->max_power) return 0;
		break;

	case POPSORT_MAXPOWER:
		if(word1->max_power > word2->max_power) return 1;
		if(word1->max_power < word2->max_power) return 0;
		if(word1->now_power > word2->now_power) return 1;
		if(word1->now_power < word2->now_power) return 0;
		break;

	case POPSORT_MIXER:
		if(word1->now_power > word2->now_power) return 1;
		if(word1->now_power < word2->now_power) return 0;
		if(word1->max_power > word2->max_power) return 1;
		if(word1->max_power < word2->max_power) return 0;
		if(word1->random > word2->random) return 1;
		if(word1->random < word2->random) return 0;
		if(word1->word[0] > word2->word[0]) return 1;
		if(word1->word[0] < word2->word[0]) return 0;
		if(word1->word[1] > word2->word[1]) return 1;
		if(word1->word[1] < word2->word[1]) return 0;
		return 1;
		// ���ʐM���\�[�g�ł̓����_����������Ȃ��B
		// �P��f�[�^�l�ł����肵�Ă���̂ŕK������ł���͂������A
		// �O�̂��߂ɂP��Ԃ��Ă���

	}
	return pp_rand() & 1;
}
/**********************************************************************
**	�\�͒l�A���ݔ\�͒l�A�����������_���Ɍ��肷��
**
**  word   �\�͒l�����肷��f�[�^�\���̃A�h���X
**
**********************************************************************/
static void set_rand_power(PopularWord *word)
{
	u16 pow = pp_rand() % 98;
	if(pow > 50){
		pow = pp_rand() % 98;
		if(pow > 80){
			pow = pp_rand() % 98;
		}
	}
	word->max_power = 30 + pow;
	word->now_power = 30 + (pp_rand() % (pow+1));

	word->random = pp_rand();
}
/**********************************************************************
**	���łɓo�^����Ă��闬�s��Əd�����Ă��邩�`�F�b�N
**
**  word   �ȈՉ�b�f�[�^�A�h���X
**
**********************************************************************/
static u8 is_dup_regist_word(u16 *word)
{
	u16 i;
	for(i = 0; i < POPWORD_MAX; i++){
		if(is_same_word(word, Fld.PopWord[i].word)){ return 1; }
	}
	return 0;
}
/**********************************************************************
**	�Q�̗��s��̒P�ꂪ�������ǂ����`�F�b�N
**
**  word1, word2  �ȈՉ�b�f�[�^�A�h���X
**
**********************************************************************/
static u8 is_same_word(u16 *word1, u16 *word2)
{
	u16 i;

	for(i = 0; i < 2; i++){
		if(*word1++ != *word2++){ return 0; }
	}
	return 1;
}
/**********************************************************************
**	�ʐM�p���[�N�ɂ��闬�s��f�[�^�ƁA�d�����Ă��Ȃ����`�F�b�N
**
**  pop     �`�F�b�N���闬�s��f�[�^
**	record  �ʐM�p���[�N�ɂ���f�[�^����
**
**	return:  0 �ȏ� ... �d�����������ꍇ�̃f�[�^�ԍ�
**	        -1      ... �d���Ȃ�
**
**
**********************************************************************/
static s16 check_mix_dup_word(PopularWord *pop, u16 record)
{
	PopularWord *work_pop;
	s16 i;

	work_pop = MIX_SORT_BUFFER;
	for(i = 0; i < record; i++){
		if(is_same_word(pop->word, work_pop->word)){ return i; }
		work_pop++;
	}
	return -1;
}

