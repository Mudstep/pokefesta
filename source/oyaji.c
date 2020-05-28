/***********************************************************************
**	���₶�f�[�^�Ǘ�
***********************************************************************/
#include "common.h"
#include "ev_flag.h"
#include "pm_str.h"
#include "kaiwa.h"
#include "kaiwa_code.h"
#include "fld_talk.h"
#include "oyaji.h"
#include "evobjdef.h"				//nohara 01/12/22
//#include "../script/savework.h"		//OBJCHRWORK�Q�Ƃ̂���
#include "sysflag.h"				//SVWK_START�Q�Ƃ̂���
#include "syswork.h"
#include "task.h"
#include "script.h"
#include "print.h"
#include "menutool.h"
#include "mus_tool.h"
#include "calctool.h"
#include "oyaji03.h"
#include "message.h"

extern void SongOyajiInit( s16* work, u16 tango, u16 melody );
extern void SongOyajiPlay( s16* work, u8 code );
extern void SongOyajiPicthChange( s16* work );

extern void InitOyajiLegend(void);
extern void ResetOyajiLegend(void);

extern void InitRecycleOyaji(void);	// 2002/02/18 by nakahiro

/*==============================================
==	�v���g�^�C�v
==============================================*/
static void add_oyaji_sing_task(u8 mode);
void SetOBJOyajiType(void);


/***********************************************************************
**	���₶�f�[�^�̈揉�����i�Q�[���J�n���j                            **
***********************************************************************/
/*===========================*/
/* �V���K�[�\���O���₶      */
/*===========================*/
static void init_singer(void)
{
	// �����̉�b�R�[�h
	static const u16 codes[] = {
		0x1a04,    // ������
		0x1228,    // ��������
		0x0a0f,    // ���˂�����
		0x1c25,    // �܂��ɂ�
		0x200e,    // ���炾��
		0x1a1a,    // �_�C�G�b�g
	};
	OYAJIDATA_SINGER *dat = (OYAJIDATA_SINGER *)(Fld.oyaji_work);
	u16 i;

	dat->type = OYAJITYPE_SINGER;
	dat->changed = 0;
	for(i = 0; i < SONG_WORD_MAX; i++){
		dat->word[i] = codes[i];
	}
}
/*===========================*/
/* �i�E�C���₶              */
/*===========================*/
static void init_now(void)
{
	OYAJIDATA_NOW *dat = (OYAJIDATA_NOW *)(Fld.oyaji_work);
	dat->type = OYAJITYPE_NOW;
	dat->told = 0;
}
/*===========================*/
/* �f���Z�c���₶            */
/*===========================*/
static void init_legend(void)
{
	InitOyajiLegend();
}

/*===========================*/
/* �����w�����₶            */
/*===========================*/
static void init_marchen(void)
{
	OYAJIDATA_MARCHEN *dat = (OYAJIDATA_MARCHEN *)(Fld.oyaji_work);
	dat->type = OYAJITYPE_MARCHEN;
	dat->cnt = 0;
}

/*==========================================*/
/* ���T�C�N�����₶ 2002/02/18 by nakahiro  */
/*==========================================*/
static void init_recycle(void)
{
	InitRecycleOyaji();
}

/*===========================*/
/* ���C��                    */
/*===========================*/
void OyajiInit(void)
{
	u16 id;

	id = (MyData.ID[1] << 8) | MyData.ID[0];
	id %= 10;
	id /= 2;

	switch(id){
	case 0: init_singer(); break;
	case 1: init_now(); break;
	case 2: init_recycle(); break;	// 2002/02/18 by nakahiro
	case 3: init_legend(); break;
	case 4: init_marchen(); break;
	}

	SetOBJOyajiType();	//nohara 01/12/22
}
#ifdef PM_DEBUG
/*===========================*/
/* ���₶�ύX�i�f�o�b�O�p�j  */
/*===========================*/
void OyajiChange(u8 type)
{
	switch(type){
	case OYAJITYPE_SINGER: init_singer(); break;

	case OYAJITYPE_NOW:
	{
		// ���������邪�A��������P��͎c��
		u8 tmp[NELEMS(Fld.kaiwa_word_flag)];
		u8 i;
		for(i = 0; i < NELEMS(tmp); i++){ tmp[i] = Fld.kaiwa_word_flag[i]; }
		init_now();
		for(i = 0; i < NELEMS(tmp); i++){ Fld.kaiwa_word_flag[i] = tmp[i]; }
	}
	break;

	case OYAJITYPE_RECYCLE:	init_recycle();	break;	// 2002/02/18 by nakahiro
	case OYAJITYPE_LEGEND:	init_legend();	break;
	case OYAJITYPE_MARCHEN: init_marchen(); break;

	}

	SetOBJOyajiType();	//nohara 01/12/22
}
#endif
/***********************************************************************
**	���݂q�n�l�ɂ��邨�₶�^�C�v�𔻒�                                **
***********************************************************************/
u8 GetOyajiType(void)
{
	return *(u8*)(Fld.oyaji_work);
}
/***********************************************************************
**	�X�N���v�g�Ƃ̂��Ƃ�                                            **
***********************************************************************/
// �^�C�v����
void OyajiScr_GetType(void)
{
	AnswerWork = GetOyajiType();
}
// �V���K�[�F�̎��ύX�t���O�`�F�b�N
void OyajiScr_SingerCheck(void)
{
	AnswerWork = ((OYAJIDATA_SINGER *)(Fld.oyaji_work))->changed;
}
// �V���K�[�F�̎��ύX
void OyajiScr_SingerDone(void)
{
	OYAJIDATA_SINGER *dat = (OYAJIDATA_SINGER *)(Fld.oyaji_work);
	u16 i;

	PM_strcpy(dat->writer_name, MyData.my_name);
	for(i = 0; i < 4; i++){
		dat->writer_id[i] = MyData.ID[i];
	}
	for(i = 0; i < SONG_WORD_MAX; i++){
		dat->word[i] = dat->new_word[i];
	}
	dat->changed = 1;
}
// �V���K�[�F�̎����o�b�t�@�ɃZ�b�g
void OyajiScr_SingerSetWord(void)
{
	OYAJIDATA_SINGER *dat = (OYAJIDATA_SINGER *)(Fld.oyaji_work);
	u16 *word;
	u8 *cp;
	u16 i;

	if(ScriptParameter0 == 0){
		word = dat->word;
	}else{
		word = dat->new_word;
	}

	cp = MsgExpandBuffer;
	for(i = 0; i < 2; i++){
		cp = KaiwaCopyWord(cp, *word++);
		*cp++ = spc_;
		cp = KaiwaCopyWord(cp, *word++);
		*cp++ = CR_;
		cp = KaiwaCopyWord(cp, *word++);

		if(i == 0){
			*cp++ = CTRL_;
			*cp++ = C_CLR_;
		}
	}

//	FieldMsgSetNoSkip(MsgExpandBuffer);
}
// �V���K�[�F�̎��\���^�X�N�o�^
void OyajiScr_AddSingTask(void)
{
	add_oyaji_sing_task(ScriptParameter0);
	NMenuTalkWinWrite();
	StopFieldScript();
}
// �i�E�C�F���Ƃ΂����������ǂ����`�F�b�N
void OyajiScr_NowCheck(void)
{
	AnswerWork = ((OYAJIDATA_NOW *)(Fld.oyaji_work))->told;
}
// �i�E�C�F���Ƃ΂���������t���O�Z�b�g
void OyajiScr_NowDone(void)
{
	((OYAJIDATA_NOW *)(Fld.oyaji_work))->told = 1;
}
// �i�E�C�F�V�������t�������
void OyajiScr_NowWord(void)
{
	u16 code = NowWordGetUnknown();

	if(code == KAIWA_CODE_BLANK){
		AnswerWork = 0;
	}else{
		KaiwaCopyWord(StrTempBuffer0, code);
		AnswerWork = 1;
	}
}
// �����w���F�V�����Z���t�����邩�`�F�b�N
void OyajiScr_MarchenCheckWord(void)
{
	OYAJIDATA_MARCHEN *dat = (OYAJIDATA_MARCHEN *)(Fld.oyaji_work);
	if(dat->cnt == MARCHEN_WORD_MAX){
		AnswerWork = 0;
		dat->cnt = 0;
	}else{
		AnswerWork = 1;
	}
}
// �����w���F�V�����Z���t���o�b�t�@�ɃR�s�[
static void  init_marchen_word(void);

void OyajiScr_MarchenWord(void)
{
	OYAJIDATA_MARCHEN *dat = (OYAJIDATA_MARCHEN *)(Fld.oyaji_work);

	if(dat->cnt == 0){
		init_marchen_word();
	}

	if(dat->word_code[dat->cnt] != KAIWA_CODE_BLANK){
		static const u8 * const subword[] = {
			MeruhenSubWord0, MeruhenSubWord1, MeruhenSubWord2, MeruhenSubWord3,
			MeruhenSubWord4, MeruhenSubWord5, MeruhenSubWord6, MeruhenSubWord7,
		};

		u8 *cp, sub;

		sub = pp_rand() % MARCHEN_SUB_MAX;
		cp = KaiwaCopyWord(MsgExpandBuffer, dat->word_code[dat->cnt]);
		cp = PM_strcpy(cp, MeruhenJoinWord);
		cp = PM_strcpy(cp, subword[sub]);
		PM_strcpy(cp, MeruhenEndWord);

	}else{
		static const u8 * const word[] = {
			MeruhenOriginalWord0, MeruhenOriginalWord1, MeruhenOriginalWord2, MeruhenOriginalWord3,
			MeruhenOriginalWord4, MeruhenOriginalWord5, MeruhenOriginalWord6, MeruhenOriginalWord7,
		};

		PM_strcpy(MsgExpandBuffer, word[dat->exword_no[dat->ex_cnt++]]);
	}

	// 1/10�̊m���œr���I������
	if(pp_rand()%10 == 0){
		dat->cnt = MARCHEN_WORD_MAX;
	}else{
		dat->cnt++;
	}
	AnswerWork = 1;
}
static void init_marchen_word(void)
{
	OYAJIDATA_MARCHEN *dat = (OYAJIDATA_MARCHEN *)(Fld.oyaji_work);
	u16 i, tmp, total, r;
	s16 cnt;
	u16 category_tbl[][2] = {
		{  KWC_POKEMON,  0 },  // �|�P����
		{  KWC_SEIKATU,  0 },  // ��������
		{  KWC_SYUMI,    0 },  // �����
		{  KWC_WAZA1,    0 },  // �킴
		{  KWC_WAZA2,    0 },  // �킴�Q
		{  KWC_POKEMON2, 0 },  // �|�P�����Q
	};

	// ����R�[�h�o�������V���b�t�����Ă���
	for(i = 0; i < MARCHEN_EXWORD_MAX; i++){
		dat->exword_no[i] = i;
	}
	for(i = 0; i < MARCHEN_EXWORD_MAX; i++){
		r = pp_rand() % (i+1);
		tmp = dat->exword_no[i];
		dat->exword_no[i] = dat->exword_no[r];
		dat->exword_no[r] = tmp;
	}

	// �g�p�J�e�S���̂����A�g����P�ꐔ���Ă��Ă���
	total = 0;
	for(i = 0; i < NELEMS(category_tbl); i++)
	{
		category_tbl[i][1] = KaiwaEnableWordCnt(category_tbl[i][0]);
		total += category_tbl[i][1];
	}
	dat->ex_cnt = 0;

	tmp = 0;
	for(i = 0; i < MARCHEN_WORD_MAX; i++){
		r = pp_rand() % 10;
		if(r < 3 && tmp < MARCHEN_EXWORD_MAX)
		{
			dat->word_code[i] = KAIWA_CODE_BLANK;  // ����ү���ނƂ��Ĉ���
			tmp++;
		}
		else
		{

			cnt = pp_rand() % total;
			for(r = 0; i < NELEMS(category_tbl); r++)
			{
				cnt -= category_tbl[r][1];
				if(cnt <= 0){
					break;
				}
			}
			if(r == NELEMS(category_tbl)){ r = 0; } // ���蓾�Ȃ����ǈꉞ

			dat->word_code[i] = KaiwaGetRandomKnownWord(category_tbl[r][0]);
//			dat->word_sub[i] = pp_rand() % MARCHEN_SUB_MAX;
		}
	}

}
/***********************************************************************
**	�ʐM�����I������̏���                                            **
***********************************************************************/
/*===========================*/
/* �V���K�[                  */
/*===========================*/
static void reset_singer(void)
{
	OYAJIDATA_SINGER *dat = (OYAJIDATA_SINGER *)(Fld.oyaji_work);
	dat->changed = 0;
}
/*===========================*/
/* �i�E��                    */
/*===========================*/
static void reset_now(void)
{
	OYAJIDATA_NOW *dat = (OYAJIDATA_NOW *)(Fld.oyaji_work);
	dat->told = 0;
}
/*===========================*/
/* ���T�C�N��                */
/*===========================*/
static void reset_recycle(void)
{
	ResetRGGiveFlag();
}
/*===========================*/
/* �f���Z�c                  */
/*===========================*/
static void reset_legend(void)
{
	ResetOyajiLegend();
}
/*===========================*/
/* ���C��                    */
/*===========================*/
void OyajiReset(void)
{
	switch(GetOyajiType()){
	case OYAJITYPE_SINGER: reset_singer(); break;
	case OYAJITYPE_NOW: reset_now(); break;
	case OYAJITYPE_LEGEND: reset_legend(); break;
	case OYAJITYPE_RECYCLE: reset_recycle(); break;
	case OYAJITYPE_MARCHEN: break;
	}

	SetOBJOyajiType();	//nohara 01/12/22
}
/***********************************************************************
**	�V���K�[�\���O���₶�̉̕\���^�X�N                                **
***********************************************************************/
/* �P��R�[�h�������f�B�i���o�[�ϊ��}�N���i�Ă��Ƃ��j*/
#define CODE_TO_MERODY(code)    (((code)&3)+(((code)>>3)&1))
/* �i���x��` */
enum SING_TASK_PROG {
	SING_INIT_ALL = 0,
	SING_FADEOUT_WAIT,
	SING_INIT_WORD,
	SING_PUT_CHAR,
	SING_PUT_WAIT,
	SING_PUTCHAR_WAIT,
};
/* ���[�N��` */
enum SING_TASK_WORK {
	STW_PROG = 0,
	STW_SUBPROG,
	STW_TIMER,
	STW_COUNT,
	STW_WORD_COUNT,
	STW_MODE,

	// �������牺�̓T�E���h���Ŏg�p
	STW_SOUNDWORK,
};
static void oyaji_sing_task(u8 task_no);
//========================================
// �̎��\���^�X�N�Z�b�g
// mode  (0:�{���܂�  1:��������)
//========================================
static void add_oyaji_sing_task(u8 mode)
{
	u8 no;
	no = AddTask( oyaji_sing_task, 80 );
	TaskTable[no].work[STW_MODE] = mode;
}
//========================================
// �̎��\���^�X�N
//========================================
static void oyaji_sing_task(u8 task_no)
{
	extern TALK_WIN *pWin;
	TASK_TABLE *task = &TaskTable[task_no];

	switch(task->work[STW_PROG]){
	// �S�̏�����
	case SING_INIT_ALL:
		{
			OyajiScr_SingerSetWord();
			MsgControlInit(pWin, FIELD_MENU_TYPE);
			TalkMsgSet(pWin, MsgExpandBuffer, 2, 4, 15);
			task->work[STW_SUBPROG] = 0;
			task->work[STW_TIMER] = 0;
			task->work[STW_COUNT] = 0;
			task->work[STW_WORD_COUNT] = 0;

			MusicFadeOutPause(4);
			task->work[STW_PROG] = SING_FADEOUT_WAIT;
		}
		break;

	// �a�f�l�t�F�[�h�A�E�g�҂�
	case SING_FADEOUT_WAIT:
		if(MusicFadeOutPauseWait()){
			task->work[STW_PROG] = SING_INIT_WORD;
		}
		break;

	// �P�ꂲ�Ƃ̏�����
	case SING_INIT_WORD:
		{
			OYAJIDATA_SINGER *dat = (OYAJIDATA_SINGER *)(Fld.oyaji_work);
			u8 *cp = MsgExpandBuffer + task->work[STW_COUNT];
			u16 word_len = 0;
			static u16 merody_num = 0;
			while(*cp != spc_ && *cp != CR_ && *cp != CTRL_ && *cp != EOM_){
				cp++;
				word_len++;
			}
			if(task->work[STW_MODE]==0){
				merody_num = CODE_TO_MERODY(dat->word[task->work[STW_WORD_COUNT]]);
			}else{
				merody_num = CODE_TO_MERODY(dat->new_word[task->work[STW_WORD_COUNT]]);
			}
			// �������E�P��R�[�h��胁���f�B����
			SongOyajiInit( task->work, word_len, merody_num );
		}
		task->work[STW_WORD_COUNT]++;

		if(task->work[STW_TIMER] == 0){
			task->work[STW_PROG] = SING_PUT_CHAR;
			task->work[STW_SUBPROG] = 0;
		}else{
			task->work[STW_PROG] = SING_PUTCHAR_WAIT;
			task->work[STW_SUBPROG] = 0;
		}
		break;

	case SING_PUTCHAR_WAIT:
		if(task->work[STW_TIMER]){
			task->work[STW_TIMER]--;
		}else{
			task->work[STW_PROG] = SING_PUT_CHAR;
		}
		break;

	// �P�����\��
	case SING_PUT_CHAR:
		// �I�[�Ȃ�X�N���v�g�ĊJ
		if(MsgExpandBuffer[task->work[STW_COUNT]] == EOM_){
			MusicFadeInContinue(6);
			ContinueFieldScript();
			DelTask(task_no);
			break;
		}
		// �X�y�[�X�A���s�A�t�H���g����R�}���h�ŒP�ꂲ�Ƃ̏�������
		if(MsgExpandBuffer[task->work[STW_COUNT]] == spc_){
			TalkMsgPutc(pWin);
			task->work[STW_COUNT]++;
			task->work[STW_PROG] = SING_INIT_WORD;
			task->work[STW_TIMER] = 0;
			break;
		}
		if(MsgExpandBuffer[task->work[STW_COUNT]] == CR_){
			task->work[STW_COUNT]++;
			task->work[STW_PROG] = SING_INIT_WORD;
			task->work[STW_TIMER] = 0;
			break;
		}
		if(MsgExpandBuffer[task->work[STW_COUNT]] == CTRL_){
			task->work[STW_COUNT] += 2;
			task->work[STW_PROG] = SING_INIT_WORD;
			task->work[STW_TIMER] = 8;
			break;
		}

		switch(task->work[STW_SUBPROG]){
		case 0:  // �����\��
			TalkMsgPutc(pWin);
			task->work[STW_SUBPROG]++;
			break;
		case 1:  // ����炷
			SongOyajiPlay( task->work, MsgExpandBuffer[ task->work[STW_COUNT] ] );
			task->work[STW_SUBPROG]++;
			break;
		case 2:  // �s�b�`�ύX
			SongOyajiPicthChange( task->work );
			task->work[STW_COUNT]++;
			task->work[STW_SUBPROG] = 0;
			task->work[STW_TIMER] = 16;	//8; // �����\���E�F�C�g�i���߂���j
			task->work[STW_PROG] = SING_PUT_WAIT;
			break;
		}
		break;

	// �P�����\����E�F�C�g
	case SING_PUT_WAIT:
		if(--(task->work[STW_TIMER]) == 0){
			task->work[STW_PROG] = SING_PUT_CHAR;
		}
		break;

	}

}

/*==============================================================*/
/* ���[�N�Q�ƌ^OBJ�ɂ��₶�̃^�C�v��������                    */
/*==============================================================*/
void SetOBJOyajiType(void)
{
	//Fld.EventWork[OBJCHRWORK1-SVWK_START] = ( OYAJI1+GetOyajiType() );
	SetEventWorkValue( OBJCHRWORK1, ( OYAJI1+GetOyajiType() ) );
}

