/***********************************************************************
**	�}�b�v�؂�ւ����̒n���\��
**	Masao Taya
***********************************************************************/
#include "common.h"
#include "fntequ.h"
#include "print.h"
#include "fieldmap.h"
#include "fld_main.h"
#include "fld_task.h"
#include "menu.h"
#include "menutool.h"
#include "task.h"
#include "mapname.h"
#include "ev_flag.h"
#include "sysflag.h"
#include "mappos.h"

/*==============================================
==	�萔
==============================================*/
#define  MAPNAME_WIN_X  (11)       // �n���E�B���h�E���L������
#define  MAPNAME_WIN_Y  (3)        // �n���E�B���h�E�c�L������
#define  MAPNAME_VOFS_BEGIN  ((MAPNAME_WIN_Y+1) * 8)  // V OffSet �����l
#define  MAPNAME_VOFS_END    (0)   // V OffSet �I���l
#define  MAPNAME_DISP_WAIT   (120) // �n���\���E�F�C�g
#define  MAPNAME_SPEED   (2)       // �n���\���E�B���h�E�ړ����x
#define  MAPNAME_MAX     (18)      // �n��������̍ő咷
#define  MAPNAME_WIDTH   (10)      // �n��������̕\�����i�L�������j

// �^�X�N���[�N�̗p�r
enum MAPNAME_TASK_WORK {
	MNWK_STATE,    // ���
	MNWK_CNT,      // �Ăяo���񐔃J�E���^
	MNWK_VOFS,     // BG0 �� V OffSet
	MNWK_REQ,      // �ďo�����N�G�X�g
};

// ��ԁiMNWK_STATE�̒l�j
enum MAPNAME_TASK_STATE {
	MNSTATE_MOVEDOWN,  // ���ړ���
	MNSTATE_STOP,      // ��~��
	MNSTATE_MOVEUP,    // ��ړ���
	MNSTATE_ERASE,     // �}�b�v������
	MNSTATE_QUIT,      // ����I������
};
/*==============================================
==	�O���[�o���ϐ�
==============================================*/
static u8 task_no = 0;

/*==============================================
==	�֐��v���g�^�C�v
==============================================*/
static void MapNameTask(u8 task_no);
static void update_mapname(void);
extern u8* CopyMapName(u8 *buf, u16 div, u16 size);

/***********************************************************************
**	�f�o�b�O���j���[����̌Ăяo��
***********************************************************************/
u8 DebugMapNameTest(void)
{
	MenuEnd();
	DispMapName();
	return 1;
}
/***********************************************************************
**	�^�X�N������
***********************************************************************/
void DispMapName(void)
{
	if(EventFlagCheck(CTRL_MAPNAME_HIDE) == TRUE)return;

	if(CheckTask(MapNameTask) == FALSE){
		task_no = AddTask(MapNameTask, 90);
		*(vu16 *)REG_BG0VOFS = MAPNAME_VOFS_BEGIN;

//		NMenuScreenClear();
		update_mapname();

		TaskTable[task_no].work[MNWK_STATE] = MNSTATE_MOVEDOWN;
		TaskTable[task_no].work[MNWK_VOFS] = MAPNAME_VOFS_BEGIN;

	}else{
		if(TaskTable[task_no].work[MNWK_STATE] != MNSTATE_MOVEUP){
			TaskTable[task_no].work[MNWK_STATE] = MNSTATE_MOVEUP;
		}
		TaskTable[task_no].work[MNWK_REQ] = 1;
	}
}
/***********************************************************************
**	�^�X�N�֐�
***********************************************************************/
static void MapNameTask(u8 n)
{
	TASK_TABLE *task = &TaskTable[n];

	switch(task->work[MNWK_STATE]){
	case MNSTATE_MOVEDOWN:
		task->work[MNWK_VOFS] -= MAPNAME_SPEED;
		if(task->work[MNWK_VOFS] <= MAPNAME_VOFS_END){
			task->work[MNWK_STATE] = MNSTATE_STOP;
			TaskTable[task_no].work[MNWK_CNT] = 0;
		}
		break;

	case MNSTATE_STOP:
		if(++(task->work[MNWK_CNT]) > MAPNAME_DISP_WAIT){
			task->work[MNWK_CNT] = 0;
			task->work[MNWK_STATE] = MNSTATE_MOVEUP;
		}
		break;

	case MNSTATE_MOVEUP:
		task->work[MNWK_VOFS] += MAPNAME_SPEED;
		if(task->work[MNWK_VOFS] >= MAPNAME_VOFS_BEGIN){
			if(task->work[MNWK_REQ]){
				update_mapname();
				task->work[MNWK_STATE] = MNSTATE_MOVEDOWN;
				task->work[MNWK_REQ] = 0;
			}else{
				task->work[MNWK_STATE] = MNSTATE_QUIT;
				return;
			}
		}
		break;

	case MNSTATE_QUIT:
//		*(vu16 *)REG_BG0VOFS = 0;
//		DelTask(task_no);
		EraseMapName();
		return;

	}

	*(vu16*)REG_BG0VOFS = task->work[MNWK_VOFS];
}
/***********************************************************************
**	�^�X�N�����I��
***********************************************************************/
void EraseMapName(void)
{
	if(CheckTask(MapNameTask)){
		NMenuWindowReset();
		NMenuBoxClear(0, 0, MAPNAME_WIN_X, MAPNAME_WIN_Y);
		*(vu16 *)REG_BG0VOFS = 0;
		DelTask(task_no);
	}
}
/***********************************************************************
**	�n���E�B���h�E�̓��e�����݂̒n���ɍX�V
***********************************************************************/
static void update_mapname(void)
{
	u8  buf[MAPNAME_MAX + 1];
	CopyMapName(buf, NowMap.WorldPos, 0);

	NMenuWindowCharSet(0);
	NMenuWinBoxWrite(0, 0, MAPNAME_WIN_X, MAPNAME_WIN_Y);

	if(NowMap.WorldPos != MAPPOS_D_AGITO){
		u16 ofs;
		ofs = (MAPNAME_WIDTH - PM_strlen(buf)) / 2;	
		CopyMapName(buf + ofs, NowMap.WorldPos, MAPNAME_WIDTH - ofs);
		while(ofs){
			buf[--ofs] = spc_;
		}
	}else{
		CopyMapName(buf, NowMap.WorldPos, 0);
	}

	NMenuMsgWrite(buf,1,1);
}

