/******************************************************************************/
/*                                                                            */
/*  �}�b�v�{�ݖ��f�[�^                                                        */
/*                                                                   taya     */
/******************************************************************************/
#include "common.h"
#include "intr.h"
#include "decord.h"
#include "ev_flag.h"
#include "map_inst.dat"

/***********************************************************************
**  �v���g�^�C�v                                                      **
***********************************************************************/
static const PLACE * const * get_place(u8 mapno, u8 subno);

/*==============================================================
==  �n��ɂ���{�݁E�n�`�̖��́i������|�C���^�j��Ԃ�
==
==  mapno  �n��i���o�[
==  subno  �n��T�u�i���o�[
==  num    �{�ݔԍ�
==
== return: ������|�C���^�i����ȏ㖳����� NULL�j
==
==============================================================*/
const u8* GetAreaInstitution(u8 mapno, u8 subno, u8 num)
{
	const PLACE* const * place;

	place = get_place(mapno, subno);
	if(place == NULL){ return NULL; }

	while(1)
	{
		if(((*place)->flag == 0xffff)
		|| (EventFlagCheck((*place)->flag) == 1)
		)
		{
			if(num == 0)
			{
				break;
			}
			num--;
		}
		place++;
		if((*place) == NULL)
		{
			return NULL;
		}
	}
	return (*place)->name;
}
/*====================================================*/
/*  �n��i���o�[�E�T�u�i���o�[�ɍ��v����A�{�ݖ��f�[�^*/
/*  �z���Ԃ�                                        */
/*                                                    */
/*  mapno  �n��i���o�[                               */
/*  subno  �n��T�u�i���o�[                           */
/*                                                    */
/* return:  ������|�C���^�̔z��                      */
/*        �i���v����f�[�^���Ȃ����NULL�j            */
/*                                                    */
/*====================================================*/
static const PLACE * const * get_place(u8 mapno, u8 subno)
{
	u16 i;

	for(i = 0; InstitutionData[i].mapno != MAPPOS_END; i++){
		if(InstitutionData[i].mapno > mapno){ return NULL; }
		if(InstitutionData[i].mapno == mapno){ break; }
	}

	if(InstitutionData[i].mapno == MAPPOS_END){ return NULL; }

	while(InstitutionData[i].mapno == mapno){
		if(InstitutionData[i].subno ==subno){
			return InstitutionData[i].place;
		}
		i++;
	}

	return NULL;

#if 0
	for(i = 0; i < InstitutionData[i].mapno < mapno; i++){
		if(InstitutionData[i].mapno == MAPPOS_END){ return NULL; }
	}

	while(InstitutionData[i].mapno == mapno){
		if(InstitutionData[i].subno ==subno){
			return InstitutionData[i].name;
		}
		i++;
	}

	return NULL;
#endif
}
