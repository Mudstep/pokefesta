/**********************************************************************/
/*  �p�\�R���{�b�N�X������  -���j���[-                                */
/**********************************************************************/
#include "common.h"
#include "intr.h"
#include "decord.h"
#include "actanm.h"
#include "actor.h"
#include "palanm.h"
#include "mus_tool.h"

#include "message.h"
#include "box2.h"


/*==============================================
==	������
==============================================*/
void BoxMenuInit(void)
{
	Work->menu_cnt = 0;
	Work->menu_maxlen = 0;
}
/*==============================================
==	�ǉ�
==	menu_type  �ǉ����j���[�R�[�h
==============================================*/
void BoxMenuAdd(u8 menu_type)
{
#ifdef PM_DEBUG
	static const u8 menustr_pokeset[] = { SE_, TTU_, TO_, su_, ru_, EOM_, };
#endif


	static const u8 * const menu_msg[] = {
		BoxMenuMsg_yameru,
		BoxMenuMsg_azukeru,
		BoxMenuMsg_tureteiku,
		BoxMenuMsg_tukamu,
		BoxMenuMsg_irekaeru,
		BoxMenuMsg_oku,
		BoxMenuMsg_yousu,
		BoxMenuMsg_nigasu,
		BoxMenuMsg_marking,
		BoxMenuMsg_kirikaeru,
		BoxMenuMsg_kabegami,
		BoxMenuMsg_namae,
		BoxMenuMsg_wptheme0,
		BoxMenuMsg_wptheme1,
		BoxMenuMsg_wptheme2,
		BoxMenuMsg_wptheme3,
		BoxMenuMsg_wptype00,
		BoxMenuMsg_wptype01,
		BoxMenuMsg_wptype02,
		BoxMenuMsg_wptype03,
		BoxMenuMsg_wptype10,
		BoxMenuMsg_wptype11,
		BoxMenuMsg_wptype12,
		BoxMenuMsg_wptype13,
		BoxMenuMsg_wptype20,
		BoxMenuMsg_wptype21,
		BoxMenuMsg_wptype22,
		BoxMenuMsg_wptype23,
		BoxMenuMsg_wptype30,
		BoxMenuMsg_wptype31,
		BoxMenuMsg_wptype32,
		BoxMenuMsg_wptype33,

#ifdef PM_DEBUG
		menustr_pokeset, // �f�o�b�O�p
#endif
	};

	if(Work->menu_cnt < BOX_MENU_MAX){
		MENU_ITEM *menu = &Work->menu[Work->menu_cnt];
		u8 len;

		menu->msg = menu_msg[menu_type];
		(int)(menu->ptr) = menu_type;

		len = PM_strlen(menu->msg);
		if(len > Work->menu_maxlen){
			Work->menu_maxlen = len;
		}
		Work->menu_cnt++;
	}
}
/*==============================================
==	���j���[��Ԃ𒲂ׂ�
==	pos  ���Ԗڂ̃��j���[�𒲂ׂ邩
==	return:  ���j���[�R�[�h�i�����Ȉʒu�𒲂ׂ��� -1�j
==============================================*/
s8 BoxMenuCheck(u8 pos)
{
	if(pos >= Work->menu_cnt)
	{
		return -1;
	}

	return (s8)((s32)(Work->menu[pos].ptr));
}
/*==============================================
==	�\��
==============================================*/
#define  MENU_X1      (20)   // �E�B���h�E�쐻���� x1 �l
#define  MENU_WIDTH   (6)    // ���j���[�̍ő啶����
#define  MENU_X2      (29) // �E�B���h�E�쐻���� x2 �l
#define  MENU_Y2      (15)   // ���j���[�E�B���h�E�쐻���� y2 �l
#define  MENU_Y1_MIN  (MENU_Y2-(BOX_MENU_MAX*2+1))
void BoxMenuDisp(void)
{
	Work->menu_y1 = MENU_Y2 - ((Work->menu_cnt * 2) + 1);
	Work->menu_x1 = MENU_X2 - (Work->menu_maxlen + 3);

	NMenuWinBoxWrite(Work->menu_x1, Work->menu_y1, MENU_X2, MENU_Y2);
	NMenuFixListWrite(Work->menu_x1+2, Work->menu_y1+1, Work->menu_cnt, Work->menu);
	NMenuVCursorInit(Work->menu_x1+1, Work->menu_y1+1, Work->menu_cnt, 0 );
}
/*==============================================
==	����
==	return:  �I�����ꂽ��  ���j���[�h�c�i�O�`�j
==	         ���I��        CURSOR_DEFAULT_VALUE = -2,
==	         �L�����Z��    CURSOR_CANCEL_VALUE  = -1,
==============================================*/
s16 BoxMenuSelect(void)
{
	int ret;

	do {
		ret = CURSOR_DEFAULT_VALUE;

		if( sys.Trg & A_BUTTON ){
			ret = NMenuVCursorPosGet();
			break;
		}
		if( sys.Trg & B_BUTTON ){
			SePlay(SE_SELECT);
			ret = CURSOR_CANCEL_VALUE;
		}
		if( sys.Trg & U_KEY ){
			SePlay(SE_SELECT);
			NMenuVCursorSet(-1);
			break;
		}
		if( sys.Trg & D_KEY ){
			SePlay(SE_SELECT);
			NMenuVCursorSet(1);
			break;
		}

	}while(0);

	if(ret != CURSOR_DEFAULT_VALUE){
		NMenuBoxClear(Work->menu_x1, Work->menu_y1, MENU_X2, MENU_Y2);
	}

	if(ret >= 0){
		ret = (int)(Work->menu[ret].ptr);
	}

	return ret;
}
