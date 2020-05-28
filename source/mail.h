//=========================================================================
//
//		�|�P�����Ɏ������郁�[��
//												by matsuda	2001.08.28(��)
//=========================================================================

#ifndef __MAIL_H__
#define __MAIL_H__

#include "param.h"	// PokemonParam�^�̂���


//=========================================================================
//	�萔��`
//=========================================================================
#define MAIL_MSG_LEN		9		//�ȈՉ�b��(�{��)
#define MAIL_TEMOTI_STOCK	6		//�莝���Žį����Ă����鐔
#define MAIL_PASOCOM_STOCK	10		//�߿�݂Žį����Ēu���鐔
#define MAIL_STOCK_MAX	(MAIL_TEMOTI_STOCK+MAIL_PASOCOM_STOCK)

#define NON_ITEM	0	//���тȂ����(�Ƃ肠����0�����тȂ�)

#define	NON_MAIL	0xff	//Ұق������Ă��Ȃ��Ƃ���IDNo

#define MAIL_HOLDMONSNO_ANNOON	30000	//hold_monsno�ɱ�ɰ݂������Ă������̊�l


//=========================================================================
//	�\���̐錾
//=========================================================================

//-- (SRAM�ɕۑ����Ă�������) --//
typedef struct{
	u16 msg[MAIL_MSG_LEN];			//���[���{��
	u8 writer_name[PERSON_NAME_SIZE+EOM_SIZE];				//�������l�̖��O
	u8 writer_id[4];				//�������l��ID
	u16 hold_monsno;				//���������߹��No
	u16 design_no;					//�G��(����No)  ��NON_ITEM�������ꍇ�͎g�p���ĂȂ�
}MAIL_DATA;



//=========================================================================
//	�O���֐�
//=========================================================================
extern void MailWorkInit(void);
extern u8 PokeMailCheck(PokemonParam *pp);
extern u8 PokeAddNewMail(PokemonParam *pp, u16 itemno);
extern u8 MailTrans(PokemonParam *pp);
extern void PokeDelMail(PokemonParam *pp);
extern u8 MailItemNoCheck(u16 itemno);
extern u8 PokeAddWrittenMail(PokemonParam *pp, MAIL_DATA *md);
extern void MailDataInit(MAIL_DATA *md);
extern void PasoDelMail(u8 mailid);
extern u16 GetMailHoldMonsNo(u16 hold_monsno, u16 *annon_id);
extern u16 MailHoldMonsNoMake(u16 monsno, u32 personal_rnd);



#endif	//__MAIL_H__

