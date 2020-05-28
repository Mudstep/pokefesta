//====================================================================
//
//							fan.h
//
//====================================================================
#ifndef __FAN_H__
#define __FAN_H__

//====================================================================
//							��`
//====================================================================
#define FAN_BIT				(8)					//�t�@���r�b�g

//�r�b�g�i���o�[
#define	FAN_SCHOOLW1		(FAN_BIT+0)
#define	FAN_MIDDLEM2		(FAN_BIT+1)
#define	FAN_OJYOU2			(FAN_BIT+2)
#define	FAN_BOYA1			(FAN_BIT+3)
#define	FAN_BOY2			(FAN_BIT+4)
#define	FAN_YOUNGM1			(FAN_BIT+5)
#define	FAN_WORKERW1		(FAN_BIT+6)
#define	FAN_OLDWOMAN1		(FAN_BIT+7)

//�t�@���|�C���g�v�Z�p��`
#define	FAN_POINT_DATA_MAX		(4)
#define	FAN_SHITEN_TYPE			(0)				//�l�V����
#define	FAN_BASE_TYPE			(1)				//�閧��n�ΐ�
#define	FAN_CONTEST_TYPE		(2)				//�R���e�X�g
#define	FAN_BATTLE_TOWER_TYPE	(3)				//�o�g���^���[


#ifndef ASM_CPP		/* �A�Z���u���̏ꍇ�݂̂��̃V���{������`����� */


#define FLD(id)				(Fld.EventWork[id-SVWK_START])	//�Z�[�u���[�N
#define FAN_WK				(EV_FAN_WORK)					//���[�N
#define FAN_TM				(EV_FAN_TIME)					//����
#define FAN_MAX				(8)					//�t�@���̐l��
#define FIRST_FAN_MAX		(3)					//�ŏ��̃t�@���̐l��
#define	FAN_TIME_OVER		(12)				//�P�Q���Ԍo�߂��Ă�����t�@�������炷
#define FAN_POINT_OBJ_ADD_MAX	(3)				//�R�l�܂Ńt�@���|�C���g�ő��₹��
#define FAN_POINT_MAX		(20)				//�t�@���|�C���g�̍ő吔
#define FAN_TIME_OBJ_SUB_MAX	(4)				//�S�l�܂Ŏ��ԂŌ��点��

#define FAN_START_FLAG_BIT	(7)					//�t�@���쓮�t���O�r�b�g
#define FAN_START_FLAG_ON	(1 << FAN_START_FLAG_BIT)	//�t�@���쓮�t���O�I��

#define FAN_POINT_BIT		(0)					//�t�@���|�C���g�r�b�g
#define FAN_POINT			( (FLD(FAN_WK) & 0x7f) )	//�t�@���|�C���g


//====================================================================
//							extern�錾
//====================================================================
extern void FanAllInit();
extern void FanCommBattleTimeSet();
extern void FanStart();
extern u8 FanPointCalc(u8);
extern u8 FanBitOn();
extern u8 FanBitOff();
extern u16 FanNoGet();
extern void FanTimeChk();
extern void FanCommBattleWinLoseChk();
extern void FanStartFlagOn();
extern u8 ScrFanPointAdd();

extern const u8 fan_obj_add[FAN_MAX];
extern const u8 fan_obj_sub[FAN_MAX];


#endif	/*ASM_CPP*/
#endif	/*__FAN_H__*/


