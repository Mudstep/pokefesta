#ifndef __D_MATSU_H__
#define __D_MATSU_H__


//----------------------------------------------------------
//	�萔
//----------------------------------------------------------
#define MA_DEBUG	1	//1�F�f�o�b�O���	�O�F��f�o�b�O���

#define DEBUG_MSG_MAX	5

//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern void DebugMsgPrint(u8 *msg, u8 x, u8 y, u8 put_no);
extern void DebugNumMsgPrint(int num, u8 x, u8 y, u8 put_no);
extern void KoutokutenDataSet(void);
extern void ErrorMsgPut(const u8 *err_msg);

//----------------------------------------------------------
//	�f�o�b�N�p�O���[�o���ϐ�
//----------------------------------------------------------
extern u8 DebugContestFlag;	//1=1���R���A2���R�����΂�


#endif __D_MATSU_H__

