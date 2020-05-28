/********************************************************************/
/*          AgbMultiBoot.h                                          */
/*            AGB�}���`�v���C�u�[�g�E���C�u�����p�錾               */
/*                                                                  */
/*          Copyright (C) 2000-2001 NINTENDO Co.,Ltd.               */
/********************************************************************/
#ifndef _AGB_MULTIBOOT_H
#define _AGB_MULTIBOOT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <AgbTypes.h>


/* �ȉ��̒萔�͕ύX���Ȃ��ł������� */
#define MULTIBOOT_NCHILD 3              // �q�@�̍ő��
#define MULTIBOOT_HEADER_SIZE 0xc0      // �w�b�_�T�C�Y
#define MULTIBOOT_SEND_SIZE_MIN 0x100   // �ŏ����M�T�C�Y
#define MULTIBOOT_SEND_SIZE_MAX 0x40000 // �ő呗�M�T�C�Y

// �}���`�v���C�u�[�g�E�p�����[�^�\����
typedef struct {
  u32 system_work[5];   // �V�X�e���R�[�����ŏ�����������B���C�u�����ł��g�p
  u8 handshake_data;    // �N���C�A���g����̃n���h�V�F�C�N�p�f�[�^
  u8 padding;
  u16 handshake_timeout;   // �n���h�V�F�C�N���Ԑ؂�`�F�b�N�p�J�E���^
  u8 probe_count;       // 0 �łȂ���� client �F���� ���邢�� �u�[�g�v���O�����]����
  u8 client_data[MULTIBOOT_NCHILD]; // �N���C�A���g����̃n���h�V�F�C�N�p�f�[�^
  u8 palette_data;      // �N���C�A���g�́A���[�h���p���b�g�_�Ńp�����^
  u8 response_bit;      // �q�@���ڑ�����Ă���\���������ꍇ�A4P-2P: d3-d1 �� 1 �ɂȂ�
  u8 client_bit;        // �N���C�A���g�Ɣ��肳�ꂽ�q�@�́A 4P-2P: d3-d1 �� 1 �ɂȂ�
  u8 reserved1;         // �\��
  u8 *boot_srcp;        // �u�[�g�v���O�����J�n (�w�b�_�̌���w��)
  u8 *boot_endp;        // �u�[�g�v���O���� �I���|�C���^
  u8 *masterp;          // �}�X�^���Q�Ƃ��ďo�͂���w�b�_�ւ̃|�C���^
  u8 *reserved2[MULTIBOOT_NCHILD]; // �\��
  u32 system_work2[4];  // �V�X�e���R�[�����ŏ�����������
  u8 sendflag;          // SC7=1 �ɂ��đ��M���Ȃ� 0 �łȂ�
  u8 probe_target_bit;  // client �F���ΏۂɂȂ��Ă���Ȃ� 4P-2P: d3-d1 �� 1
  u8 check_wait;        // client �F�����s���܂ł̑҂��t���[��
  u8 server_type;       // client �F�����@ 0=�ᑬ�F�� 1=�����F��
} MultiBootParam;

/* �G���[�R�[�h
 * MultiBootMain() ���Ԃ��l���A0�łȂ��Ƃ��́A�ȉ��̉��炩�̃G���[�B
 * �������A���ׂẴG���[�R�[�h�̎�ނɉ������A�ʂ̃G���[���b�Z�[�W��
 * �p�ӂ���K�v�͂Ȃ��B
 * �u�ʐM�G���[�A�ڑ����m�F���Ă��������A�X�^�[�g�{�^���Ń��g���C�v
 * �u�d����؂��āA�ڑ����m�F���A������x�d������꒼���Ă��������v
 * �u�q�@���ڑ�����Ă��܂���v
 * �u�q�@���F���ł��܂���v
 * �ȂǁA�v���C���[���������₷�����b�Z�[�W�ŗǂ��Ǝv����B
 * NO_PROBE_TARGET: client �F�����ɂ��ׂẴ}�V�����ُ�ȃf�[�^��
 * �Ԃ��Ă��āA�F���Ώۂ��Ȃ��Ȃ���
 * NO_DLREADY: client ���_�E�����[�h�����\�Ƃ����Ԏ������Ȃ������B
 * BOOT_FAILURE: MultiBoot() �V�X�e���R�[�����G���[��Ԃ��Ă����B
 * HANDSHAKE_FAILURE: �q�@���u�[�g����ɍs���ׂ��A�e�@�Ƃ̃n���h�V�F�C�N�Ɏ��s�����B
 *   (�q�@�͖������[�v��ԂɂȂ��Ă���\��������̂ŁA�d���̓��꒼����v������)
 */
#define MULTIBOOT_ERROR_04                0x04
#define MULTIBOOT_ERROR_08                0x08
#define MULTIBOOT_ERROR_0c                0x0c
#define MULTIBOOT_ERROR_40                0x40
#define MULTIBOOT_ERROR_44                0x44
#define MULTIBOOT_ERROR_48                0x48
#define MULTIBOOT_ERROR_4c                0x4c
#define MULTIBOOT_ERROR_80                0x80
#define MULTIBOOT_ERROR_84                0x84
#define MULTIBOOT_ERROR_88                0x88
#define MULTIBOOT_ERROR_8c                0x8c
#define MULTIBOOT_ERROR_NO_PROBE_TARGET   0x50
#define MULTIBOOT_ERROR_NO_DLREADY        0x60
#define MULTIBOOT_ERROR_BOOT_FAILURE      0x70
#define MULTIBOOT_ERROR_HANDSHAKE_FAILURE 0x71

/* �G���[��A���邢�� 0xffff �ȊO�̒l���Ԃ��Ă��āA�ڑ��`�F�b�N��
 * ��蒼���܂ł̃`���^�����O����E�G�C�g (�t���[��)
 * �N���C�A���g�̃^�C���A�E�g�� �� 11 �t���[������̂ŁA������
 * �\�������l�ɂ��Ă����B
 * �ʏ�A���̊��Ԃ� MASTER_INFO �𑗏o���邪�A�V�X�e���R�[����
 * �G���[�ɂȂ����Ƃ��́A���̊��ԁA�������M���Ȃ��ő҂B
 * ���ꂩ��A���̓������� MASTER_INFO �𑗏o����B
 */
#define MULTIBOOT_CONNECTION_CHECK_WAIT 15

/* �q�@�̔F�����@
 * MultiBootParam �\���̂̃����o server_type �ɐݒ肷��l
 * MULTIBOOT_SERVER_TYPE_NORMAL: �ᑬ�F�����[�h
 *   MultiBootMain() �֐��͈��̌Ăяo���� 2 �o�C�g�̒ʐM�B
 *   ���̂��� MultiBootMain() �֐�����Z���ԂŋA���Ă���B
 * MULTIBOOT_SERVER_TYPE_QUICK: �����F�����[�h
 *   MultiBootMain() �֐����ŁA�o���邾�������A���ʐM�B
 *   ���̂��� MultiBootMain() �֐�����A���\�t���[���A���Ă��Ȃ��\��������B
 *   ���̊ԁA�T�E���h��\�������Ȃǂ��o���Ȃ��Ă���肪�Ȃ����A
 *   �T�E���h��\���������^�C�}�[���̊����ݏ����ōs���Ă���ꍇ�A�g�p�\�B
 */
#define MULTIBOOT_SERVER_TYPE_NORMAL 0
#define MULTIBOOT_SERVER_TYPE_QUICK  1

/* �q�@�̍ŏI�N���m�F�̃^�C���A�E�g 400 �t���[�� */
#define MULTIBOOT_HANDSHAKE_TIMEOUT 400

#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif    /* _AGB_MULTIBOOT_H */
/* End of AgbMultiBoot.h */
