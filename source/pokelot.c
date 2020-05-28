/**********************************************************************/
/*  �|�P���������f�[�^����                                            */
/*                                                            taya    */
/**********************************************************************/

#include "common.h"
#include "calctool.h"
#include "syswork.h"
#include "ev_flag.h"
#include "pokelist.h"
#include "pasocom.h"
#include "item.h"
#include "poketool.h"

//===========================================
// �}�N��
//===========================================
#define POKELOT_MAXNUMBER  (65535)  // ���I�i���o�[�ő�l
#define POKELOT_MAXDAYS  (0)   // �����X�V�ł̓��������i�O�Ȃ琧�����Ȃ��j

//===========================================
// �v���g�^�C�v
//===========================================
static void set_pokelot_value(u32 val);
static u32  get_pokelot_value(void);
static u8 check_hit_keta(u16 val1, u16 val2);

//===========================================
// �i�i
//===========================================
static const u16 PokeLotPrizeTable[4] = {
	ITEM_POINTOAPPU,         // �R���F�|�C���g�A�b�v
	ITEM_GAKUSYUUSOUTI,  // �Q���F�������イ������
	ITEM_GENKINOKATAMARI, // �P���F���񂫂̂����܂�
	ITEM_MASUTAABOORU,      // �����F�}�X�^�[�{�[��
};



/*==============================================================
==  �Q�[���J�n���̏���������
==============================================================*/
void InitPokeLot(void)
{
	u16  L, H;

	L = pp_rand();
	H = pp_rand();

	set_pokelot_value((H<<16) | L);
	SetEventWorkValue(WK_POKELOT_PRIZE, 0);
}
/*==============================================================
==  �����o�߂ɂ��X�V����
==============================================================*/
void UpdatePokeLot(u16 days)
{
	u32  value;

	value = pp_rand();

#if POKELOT_MAXDAYS
	if(days > POKELOT_MAXDAYS){ days = POKELOT_MAXDAYS; }
#endif

	while(days--){
		value = value * 1103515245L + 12345;
	}

	set_pokelot_value(value);
}
/*==============================================================
==  ���I�i���o�[�� AnswerWork �ɃZ�b�g
==============================================================*/
void GetPokeLotNumber(void)
{
	u32 value;

	value = get_pokelot_value();
//	AnswerWork = (u16)(value / 65536L) % 32768U;
	AnswerWork = (u16)(value % (POKELOT_MAXNUMBER+1));
}
/*==============================================================
==  AnswerWork �ɓ����Ă���i���o�[�œ��I�`�F�b�N
==  �y���ʁz
==    ScriptParameter0  ... 0(�͂���j�`4�i�����j
==  ��ScriptParameter1  ... ���炦��A�C�e���i���o�[
==  ��ScriptParameter2  ... ���I�|�P�������i0:�Ă��� 1:�{�b�N�X�j
==  ��StrTempBuffer0   ... ���I�|�P������
==
==  �����I���Ȃ������ꍇ�A�����̒l�͓��e���ۏ؂���܂���
==============================================================*/
void CheckPokeLotNumber(void)
{
	u32 id;
	u16 monsbox, monspos, i, j;
	u8  keta;

	ScriptParameter0 = 0;
	monsbox = monspos = 0;

	// �莝���`�F�b�N
	for(i = 0; i < TEMOTI_POKEMAX; i++)
	{
		if(PokeParaGet(PokeParaMine+i, ID_monsno) != 0)
		{
			if(PokeParaGet(PokeParaMine+i, ID_tamago_flag) == 0)
			{
				id = (PokeParaGet(PokeParaMine + i, ID_id_no) & 0xffff);
				keta = check_hit_keta(AnswerWork, id);
				if(keta > ScriptParameter0 && keta > 1)
				{
					ScriptParameter0 = keta - 1;
					monsbox = BOX_KAZU;
					monspos = i;
				}
			}
		}else{
			break;
		}
	}

	// �{�b�N�X�`�F�b�N
	for(i = 0; i < BOX_KAZU; i++)
	{
		for(j = 0; j < BOX_PM_KAZU; j++)
		{
			if(PokePasoParaGet(&(BoxData.PPPBox[i][j]), ID_monsno) != 0)
			{
				if(PokePasoParaGet(&(BoxData.PPPBox[i][j]), ID_tamago_flag) == 0)
				{
					id = (PokePasoParaGet(&(BoxData.PPPBox[i][j]), ID_id_no) & 0xffff);
					keta = check_hit_keta(AnswerWork, id);
					if(keta > ScriptParameter0 && keta > 1)
					{
						ScriptParameter0 = keta - 1;
						monsbox = i;
						monspos = j;
					}
				}
			}
		}
	}

	// ���I���Ă���
	if(ScriptParameter0)
	{
		ScriptParameter1 = PokeLotPrizeTable[ScriptParameter0-1];
		if(monsbox == BOX_KAZU){
			ScriptParameter2 = 0;
			PokeParaGet(PokeParaMine + monspos, ID_nickname, StrTempBuffer0);
		}else{
			ScriptParameter2 = 1;
			PokePasoParaGet(&(BoxData.PPPBox[monsbox][monspos]), ID_nickname, StrTempBuffer0);
		}
		NickNameCutOff(StrTempBuffer0);
	}
}
/*====================================================*/
/*  ��v�������v�Z                                    */
/*  val1, val2   ��r���鐔�l                         */
/*  return:  ��v�����i0�`5�j                         */
/*====================================================*/
static u16 V1 = 0, V2 = 0;

static u8 check_hit_keta(u16 val1, u16 val2)
{
	u8  i, hit;

	hit = 0;

	for(i = 0; i < 5; i++)
	{
		V1 = val1 % 10;
		V2 = val2 % 10;
		if((val1%10) != (val2%10)){ break; }
		val1 /= 10;
		val2 /= 10;
		hit++;
	}

	return hit;
}
/*====================================================*/
/*  �|�P���������p�����Z�b�g                          */
/*  val    ����32bit                                  */
/*====================================================*/
static void set_pokelot_value(u32 val)
{
	u16 L, H;

	H = (val >> 16) & 0xffff;
	L = val & 0xffff;
	SetEventWorkValue(WK_POKELOT_RND1, L);
	SetEventWorkValue(WK_POKELOT_RND2, H);
}
/*====================================================*/
/*  �|�P���������p�����Q�b�g                          */
/*====================================================*/
static u32 get_pokelot_value(void)
{
	u16 L, H;
	L = GetEventWorkValue(WK_POKELOT_RND1);
	H = GetEventWorkValue(WK_POKELOT_RND2);
	return ((H<<16)|L);
}


/*==============================================================
==  �C�ӂ̐��l�𓖑I�i���o�[�Ɂi�f�o�b�O�p�j
==============================================================*/
void SetPokeLotNumber(u16 num)
{
	set_pokelot_value(num);
}

