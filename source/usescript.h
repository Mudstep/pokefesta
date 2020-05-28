#ifndef	USESCRIPT_H
#define	USESCRIPT_H

//�@C�\�[�X�Ƃ̋��L�w�b�_�t�@�C��
#include "vmstat.h"
#include "version.h"				// �o�[�W�����w��t�@�C��

#include "fntequ.h"					// MSG�p��`
#include "fnt_tenji.h"				// �_���p��`
#include "i_msg.ndw"				// MSG�o�b�t�@��`
#include "sxytype.h"				// 
#include "evobjdef.h"				// OBJ�A�j���p��`

#include "../data/itemsym.h"		// �ǂ����h�c��`
#include "../data/gds_sym.h"		// �O�b�Y�h�c��`
#include "../snd_file/mus_def.h"	// ���yID�p��`
#include "../evdata/maparray.h"		// �ڑ��p�}�b�vID��`
#include "../evdata/sxysym.h"		// SXY�V���{����`
#include "../mapdata/scrn_id.h"		// �}�b�v�X�N���[���V���{����`

#include "syswork.h"				// �Z�[�u���[�N��`(�v���O���}�Ǘ��A�V�X�e���p�j
#include "sysflag.h"				// �Z�[�u�t���O��`(�v���O���}�Ǘ��A�V�X�e���p�j
#include "../script/savework.h"		// �Z�[�u���[�N��`(�X�N���v�g�X�^�b�t�Ǘ�)
#include "../script/saveflag.h"		// �Z�[�u�t���O��`(�X�N���v�g�X�^�b�t�Ǘ�)
#include "../script/ballflag.h"		// �Z�[�u�t���O��`(�X�N���v�g�X�^�b�t�Ǘ��E�A�C�e���{�[��)

#include "../data/trainer.def"		// �g���[�i�[�h�c��`�i���j
#include "../data/warpsym.h"		// ���[�v���W�w��V���{����`

#include "../sxydata/seed_def.h"	// ���݂̂h�c��`

#include "ev_win.h"					// �E�B���h�E��ʗp��`
#include "seed.h"					// ���˃C�x���g�p��`
#include "conscr.h"					// �R���e�X�g�p��`
#include "ev_comm.h"				// �ʐM��t�p��`
#include "tv_def.h"					// �s�u�p��`
#include "cm.h"						// �b�l�p��`
#include "kaiwa_mode.h"				// �ȈՉ�b����
#include "fan.h"					// �t�@���l����`
#include "pocketno.h"				// �|�P�b�g�i���o�[��`
#include "oyaji.h"					// ���₶�^�C�v��`
#include "scr_tool.h"				// �X�N���v�g�֘A��`

#include "monsno.def"				// �|�P�����i���o�[�̒�`
#include "wazano.def"				// �Z�i���o�[�̒�`
#include "fld_eff.h"				// �t�B�[���h�G�t�F�N�g�̒�`
#include "weather.h"				// �V��̒�`
#include "wea_ctrl.h"				// �V��̒�`
#include "blend_m.h"				// �^�l�u�����_�[�̒�`
#include "ev_sub.h"					// POS�C�x���g/TASK�ݒ�֘A�̒�`
#include "server.def"				// �퓬�֘A�̒�`
#include "b_tower.def"				// �o�g���^���[�̒�`
#include "record.h"					// �ł񂹂L�^�̒�`

	.TEXT
	.CODE 32

// C�v���v���Z�b�T�Ń}�N���u�������ׂčs����悤��
// C�\�[�X�`����include���g�p���Ă���

#include	"ev_cmd.s"	//�X�N���v�g�R�[�h��`�e�[�u��
#include 	"ev_work.s"	//�X�N���v�g�p���[�N��`�e�[�u��
#include 	"ev_call.s"	//�X�N���v�g����̃v���O�����Ăяo����`�e�[�u��
#include	"scr_tbl.s"	//���ʃX�N���v�g�̂h�c�Ăяo���p��`�e�[�u��

#endif
