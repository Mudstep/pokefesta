#ifndef __PKJB_DOWNLOADER_H__
#define __PKJB_DOWNLOADER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  vu8  timeout_counter;
  vu8  frame_counter;
  vu8  decode_mode;
  vu8  vcount_bak;

  vu8  keya[4];
  vu8  keyb[4];
  vu8  keyc[4];

  vu16 boot_key;
  vu16 dl_count;

  vu32 decode_key;
  vu32 decode_work_crc;
  vu32 decode_result_crc;
  vu32 decode_bufp;
  vu32 dl_bufp;
  vu32 joyint_vector;

} PKJBDownParam;


extern void PKJBDownInit(PKJBDownParam *paramp);
// PK-JOY�u�[�g �_�E�����[�_�̏��������s���܂��B
// REG_IME �ȊO�̃��W�X�^��ύX���A�uJOY�o�X�ʐM���荞�݉\�v�ȏ�Ԃ�
// ���܂��B���� REG_IME = 1 �ɂȂ��Ă���΁A�ďo���シ����JOY�o�X
// �ʐM���荞�݂���������\��������܂��B

extern void PKJBDownMainFrame(PKJBDownParam *paramp);
// PK-JOY�u�[�g �_�E�����[�_ ���C�����[�`��
// ���Q�[���t���[����1��Ă�ł��������B
// �u�Q�[���t���[���v�́A������������2-3�r�f�I�t���[�����ƂɂȂ��Ă����܂��܂���B

extern void PKJBDownStop(void);
// PK-JOY�u�[�g �_�E�����[�_���~���A�ėp�ʐM���[�h�ɂ��܂��B
// SIO���荞�݂��֎~���܂��BREG_IME �̏�Ԃ͕ێ����܂��B
// GC�����GBA�P�[�u�����Ȃ����Ă��Ă��F������Ȃ��Ȃ�܂��B
// ���̌�APKJBDownMainFrame() ���ĂԂ��Ƃ͂ł��܂���B
// (�_�E�����[�_���ĊJ���Ă��܂��܂�)
// PKJBDownInit() ���Ă�ł��� PKJBDownMainFrame() ���ĂԂ��Ƃ͉\�ł��B

extern void PKJBDownTryBoot(PKJBDownParam *paramp);
// PK-JOY�u�[�g �_�E�����[�h���������Ă���΃u�[�g���܂��B
// �������Ă��Ȃ���Ή������܂���B
// �܂�A�u�[�g���Ă��ǂ��^�C�~���O�Ɍ���Ă�ł��������B
// �u�[�g����O�ɁA������ REG_IME ���� 0 �Ƀ��Z�b�g���܂��B

extern void PKJBDownJoyIntr(PKJBDownParam *paramp);
// PK-JOY�u�[�g JOY�o�X�ʐM���荞�݃��[�`���{�̂ł��B

#define PKJBDownCheckDLStart(paramp) ((paramp)->decode_mode)
// 0 �łȂ���΁AGC����_�E�����[�h���n�߂Ă��܂��B(����������������܂���)

#ifdef __cplusplus
}
#endif

#endif  // __PKJB_DOWNLOADER_H__
