//====================================================================
//
//						�s�u���b�l�^��`
//
//====================================================================
#ifndef __CM_H__
#define __CM_H__


//�f�[�^�^�C�v��`
#define CM_NODATA_TYPE			0		//�f�[�^�Ȃ�
#define CM_BARGAIN_TYPE			1		//�������(����΂艮)
#define CM_CASINO_TYPE			2		//�J�W�m�f�[
#define CM_FIND_ITEM_TYPE		3		//�@��o������(�~�i���f�p�[�g����)

#define CM_DATATYPE_MAX			4		//CM�f�[�^�̎��


#ifndef ASM_CPP		/* �A�Z���u���̏ꍇ�݂̂��̃V���{������`����� */


//fd_shop.c�Ŏg�p
#define	SALE_FLG				( CMEventStartChk( CM_BARGAIN_TYPE ) )

#define TODAY_TIME				1			//1��
#define CMDATA_MAX				16			//CM�f�[�^MAX

//100����1�̊m��(65535/100=655)
#define CM_START_ON				655		//CM����������l(�ȏ�Ŕ���)
#define CM_TIME_COUNT			(4)			//�쐬�������ɃZ�b�g�������
#define CM_BROADCAST_TIME		(TODAY_TIME*2)//�����J�n�̓��� 2������
#define CM_BROADCAST_CHANGE_TIME	(20)	//���A�郁�b�Z�[�W�𕪂��鎞��


//====================================================================
//							extern�錾
//====================================================================
extern void CMDataAllInit();
extern u8 CMTimeCountChk();
extern void CMDataSet();
extern u8 CMEventStartChk(u8);
extern s8 CMDataAkiChk(TV_CM *);
extern void CMDataClear(u8);
extern void CMDataSort();
extern void CMData_Update(u16);


#endif	/*ASM_CPP*/
#endif	/*__CM_H__*/

