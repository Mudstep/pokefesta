//=========================================================================
//
//		�|�P�����Ɏ������郁�[��
//												by matsuda	2001.08.28(��)
//=========================================================================

#include "common.h"
#include "poketool.h"
#include "print.h"
#include "mail.h"
#include "item.h"

#include "pokeicon.h"
#include "monsno.def"


//----------------------------------------------------------
//	Ұق��޻޲�No
//----------------------------------------------------------
enum{
	FlowerDesign,
	NaminoriDesign,
	MizuiroDesign,
	LovelyDesign,
	NigaoeDesign,
	OnpuDesign,
	BuibuiDesign,
	HensinDesign,
};

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
void MailDataInit(MAIL_DATA *md);
u16 MailHoldMonsNoMake(u16 monsno, u32 personal_rnd);


//=========================================================================
//	���[�����[�N������
//=========================================================================
void MailWorkInit(void)
{
	u8 i;
	
	for(i = 0; i < MAIL_STOCK_MAX; i++)
		MailDataInit(&Fld.MailData[i]);
}

void MailDataInit(MAIL_DATA *md)
{
	int k;
	
	for(k = 0; k < MAIL_MSG_LEN; k++)
		md->msg[k] = 0xffff;
	for(k = 0; k < PERSON_NAME_SIZE+EOM_SIZE; k++)
		md->writer_name[k] = EOM_;
	for(k = 0; k < 4; k++)
		md->writer_id[k] = 0;
	md->hold_monsno = 1;	//�ꉞ̼����Ȃ����ް�����Ƃ�
	md->design_no = NON_ITEM;
}

//=========================================================================
//	�莝���|�P���������[���������Ă��邩����
// ����: pokeno = �莝���̉��Ԗڂ��߹�݂��������邩
// �߂�l: 0=�����ĂȂ�   1=�����Ă�
//=========================================================================
u8 PokeMailCheck(PokemonParam *pp)
{
	if(MailItemNoCheck(PokeParaGet(pp, ID_item)==0)){
		return 0;
	}

	if(PokeParaGet(pp, ID_mailid) == NON_MAIL)
		return 0;
	else
		return 1;
}


//=========================================================================
//	�V�K��Ұق��莝���߹�݂Ɏ�������
// �߂�l�F��Ă����z��ԍ��@�@0xff=�󂫂��Ȃ�����
//=========================================================================
u8 PokeAddNewMail(PokemonParam *pp, u16 itemno)
{
	u8 i, k;
	u8 buff[2];
	u16 monsno;
	u32 personal_rnd;
	
	buff[0] = itemno & 0xff;
	buff[1] = itemno >> 8;
	
	for(i = 0; i < MAIL_TEMOTI_STOCK; i++)
	{
		if(Fld.MailData[i].design_no == NON_ITEM)
		{
			for(k = 0; k < MAIL_MSG_LEN; k++)
				Fld.MailData[i].msg[k] = 0xffff;
			for(k = 0; k < PERSON_NAME_SIZE; k++)
				Fld.MailData[i].writer_name[k] = MyData.my_name[k];
			Fld.MailData[i].writer_name[k] = EOM_;
			for(k = 0; k < 4; k++)
				Fld.MailData[i].writer_id[k] = MyData.ID[k];
			monsno = PokePasoParaGet((PokemonPasoParam *)&pp->PPP,ID_monsno);
			personal_rnd=PokePasoParaGet((PokemonPasoParam *)&pp->PPP,ID_personal_rnd);
			Fld.MailData[i].hold_monsno = MailHoldMonsNoMake(monsno, personal_rnd);
			Fld.MailData[i].design_no = itemno;//GetMailDesignNo(itemno);
			PokeParaPut(pp, ID_mailid, &i);
			PokeParaPut(pp, ID_item, buff);
			return i;
		}
	}
	return 0xff;
}

//----------------------------------------------------------
//	�A���m�[���̎�������������hold_monsno�ɃZ�b�g����l�����o��
//----------------------------------------------------------
u16 MailHoldMonsNoMake(u16 monsno, u32 personal_rnd)
{
	u16 hold_monsno;
	u16 annon_rnd;
	
	if(monsno == MONSNO_ANNOON){
		annon_rnd = GetAnnoonNo(personal_rnd);
		hold_monsno = MAIL_HOLDMONSNO_ANNOON + annon_rnd;
	}
	else
		hold_monsno = monsno;
	return hold_monsno;
}

//----------------------------------------------------------
//	Mail�ް���hold_monsno����|�P����No�����o��
//	��ɰ݂��������ͱ�ɰ݂��߹��No��Ԃ��āAannon_id�ɱ�̧�ޯĔԍ���Ă���
//----------------------------------------------------------
u16 GetMailHoldMonsNo(u16 hold_monsno, u16 *annon_id)
{
	u16 monsno;
	
	if(hold_monsno >= MAIL_HOLDMONSNO_ANNOON 
		&& hold_monsno < MAIL_HOLDMONSNO_ANNOON + ANNOON_IDMAX)
	{	//��ɰ�
		monsno = MONSNO_ANNOON;
		*annon_id = hold_monsno - MAIL_HOLDMONSNO_ANNOON;
	}
	else
		monsno = hold_monsno;
	return monsno;
}

//=========================================================================
//	���ɕ��͂�������Ă��郁�[��(�߿�ݓ���)���|�P�����Ɏ�������
//   ��mailid�͐V���Ɏ����擾����
// �߂�l�F��Ă����z��ԍ��@�@0xff=�󂫂��Ȃ�����
//=========================================================================
u8 PokeAddWrittenMail(PokemonParam *pp, MAIL_DATA *md)
{
	u8 mailid;
	u8 buff[2];
	u16 itemno;
	
	itemno = md->design_no;
	mailid = PokeAddNewMail(pp, itemno);
	if(mailid == NON_MAIL)
		return 0xff;
	
	Fld.MailData[mailid] = *md;
	PokeParaPut(pp, ID_mailid, &mailid);
	buff[0] = itemno & 0xff;
	buff[1] = itemno >> 8;
	PokeParaPut(pp, ID_item, buff);
	return mailid;
}

//=========================================================================
//	�w��ID�̌��Ƀ��[���f�[�^���Z�b�g����
//=========================================================================
u8 ThisIDAddMail(u8 mailid, MAIL_DATA *md, PokemonParam *pp)
{
	return 0;
}

//=========================================================================
//	�莝���߹�݂������Ă���Ұق��폜����
//=========================================================================
void PokeDelMail(PokemonParam *pp)
{
	u8 mailid;
	u8 buff[2];
	
	if(PokeMailCheck(pp) == 0)
		return;
	
	mailid = PokeParaGet(pp, ID_mailid);
	Fld.MailData[mailid].design_no = NON_ITEM;
	
	mailid = NON_MAIL;
	buff[0] = 0xff & NON_ITEM;
	buff[1] = NON_ITEM >> 8;
	PokeParaPut(pp, ID_mailid, &mailid);
	PokeParaPut(pp, ID_item, buff);
}

//=========================================================================
//	�߿�ݓ���Ұق��폜����
//=========================================================================
void PasoDelMail(u8 mailid)
{
	Fld.MailData[mailid].design_no = NON_ITEM;
}

//=========================================================================
//	�莝���߹�݂������Ă���Ұق��߿�݂ɓ]������
// ����: pokeno = �莝���̉��Ԗڂ��߹�݂�Ұق�]�����邩
// �߂�l: ��Ă����z��ԍ�  0xff = �󂫂��Ȃ�����
//=========================================================================
u8 MailTrans(PokemonParam *pp)
{
	u8 i;
	u8 buff[2];
	u8 mail;
	
	buff[0] = 0xff & NON_ITEM;
	buff[1] = NON_ITEM >> 8;
	mail = NON_MAIL;
	
	for(i = MAIL_TEMOTI_STOCK; i < MAIL_STOCK_MAX; i++)
	{
		if(Fld.MailData[i].design_no == NON_ITEM)
		{
			memcpy(&Fld.MailData[i], &Fld.MailData[PokeParaGet(pp,ID_mailid)], sizeof(MAIL_DATA));
			Fld.MailData[PokeParaGet(pp,ID_mailid)].design_no = NON_ITEM;
			PokeParaPut(pp, ID_mailid, &mail);
			PokeParaPut(pp, ID_item, buff);
			return i;
		}
	}
	return 0xff;
}

//=========================================================================
//	���т�Ұق���������
// �߂�l: 1 = Ұ�    0 = ҰوȊO�̱���
//=========================================================================
u8 MailItemNoCheck(u16 itemno)
{
	switch(itemno)
	{
		case ITEM_ORENZIMEERU:
		case ITEM_HAABAAMEERU:
		case ITEM_KIRAKIRAMEERU:
		case ITEM_MEKANIKARUMEERU:
		case ITEM_UDDHIMEERU:
		case ITEM_KUROSUMEERU:
		case ITEM_TOREZYAAMEERU:
		case ITEM_SYADOOMEERU:
		case ITEM_TOROPIKARUMEERU:
		case ITEM_DORIIMUMEERU:
		case ITEM_MIRAKURUMEERU:
		case ITEM_RETOROMEERU:
			return 1;
	}
	return 0;
}
