/********************************************************************/
/*          AgbSound.h                                              */
/*            AGB�W���T�E���h�h���C�o�p�錾                         */
/*                                                                  */
/*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               */
/*          Programed by SMASH Co.,Ltd.                             */
/********************************************************************/
#ifndef _AGB_SOUND_H
#define _AGB_SOUND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <AgbTypes.h>


//�g�`�f�[�^�̍\��
typedef struct {
    u16 type;   //�^�C�v
    u16 stat;   //�X�e�[�^�X
    u32 freq;   //���g���v�Z�l
    u32 loop;   //���[�v�J�n�ʒu
    u32 size;   //�T���v�����O��
    s8 data[1]; //�T���v�����O�f�[�^�z��
} WaveData;

#define WAVE_STAT_LOOP  0xc000


//���F�f�[�^�̍\��
typedef struct {
    u8 typ;         //�^�C�v
    u8 key;         //�I���W�i���L�[
    u8 len;         //���̒����i�݊��T�E���h�j
    u8 p_s;         //�p���|�b�g or �X�C�[�v�i�݊��T�E���h�P�j
    WaveData *wav;  //�g�`�f�[�^�̃A�h���X
    u8 att;         //�A�^�b�N
    u8 dec;         //�f�B�P�C
    u8 sus;         //�T�X�e�B��
    u8 rel;         //�����[�X
} ToneData;

#define TONEDATA_TYP_RHY    0x80
#define TONEDATA_TYP_SPL    0x40
#define TONEDATA_TYP_FIX    0x08
#define TONEDATA_TYP_CGB    0x07

#define TONEDATA_P_S_PAN    0xc0
#define TONEDATA_P_S_PAM    TONEDATA_P_S_PAN

typedef struct { u32 typ; ToneData *ta; u8 *st; } SplitTone;
typedef union { ToneData t; SplitTone s; } ToneDataArr;

#define RHYTHM_TONE(TA)     {TONEDATA_TYP_RHY,0,0,0,(WaveData *)(TA)}
#define SPLIT_TONE(TA,ST)   (ToneDataArr)(SplitTone)\
                {TONEDATA_TYP_SPL, (ToneData *)(TA), (u8 *)(ST)}
#define RHY_SPL_TONE(TA,ST) (ToneDataArr)(SplitTone)\
                {TONEDATA_TYP_RHY|TONEDATA_TYP_SPL, (ToneData *)(TA), (u8 *)(ST)}


//���z�`�����l���̍\��
typedef struct {
    u8 sf;          //�X�e�[�^�X
    u8 ty;          //�^�C�v
    u8 rv;          //�E����
    u8 lv;          //������
    u8 at;          //�A�^�b�N
    u8 de;          //�f�B�P�C
    u8 su;          //�T�X�e�B��
    u8 re;          //�����[�X
    u8 r1[24];      // (reserved)
    u32 fr;         //���g���v�Z�l
    WaveData *wp;   //�g�`�|�C���^
    u32 r3[6];      // (reserved)
} SoundChannel;

#define MAX_DIRECTSOUND_CHANNEL 12
#define SOUND_CHANNEL_SF_START  0x80
#define SOUND_CHANNEL_SF_STOP   0x40
#define SOUND_CHANNEL_SF_LOOP   0x10
#define SOUND_CHANNEL_SF_IEC    0x04
#define SOUND_CHANNEL_SF_ENV    0x03
#define SOUND_CHANNEL_SF_ON     (SOUND_CHANNEL_SF_START|SOUND_CHANNEL_SF_STOP\
                                |SOUND_CHANNEL_SF_IEC|SOUND_CHANNEL_SF_ENV)

#define SOUND_CHANNEL_TY_FIX    TONEDATA_TYP_FIX
#define SOUND_CHANNEL_TY_CGB    TONEDATA_TYP_CGB


//�_�C���N�g�T�E���h�]���o�b�t�@�̃T�C�Y
#define PCM_DMA_BUF 1584


//�T�E���h�E���[�N�G���A�̍\��
typedef struct {
    u32 r1;     // (reserved)
    vu8 DmaCount;
    u8 reverb;  //�_�C���N�g�T�E���h�E���o�[�u�l
    u8 maxchn;  //�_�C���N�g�T�E���h�E����������
    u8 masvol;  //�_�C���N�g�T�E���h�E�}�X�^�[�{�����[��
    u8 freq;    //�_�C���N�g�T�E���h�E�Đ����g���ԍ�(1=5734Hz �` 12=42048Hz)
    u8 mode;    //�T�E���h�h���C�o�E���샂�[�h
    u8 r2[6];   // (reserved)
    u32 r3[16]; // (reserved)
    SoundChannel vchn[MAX_DIRECTSOUND_CHANNEL];
#ifndef ONLY_DMA1_SOUND
    s8 pcmbuf[PCM_DMA_BUF*2];
#else
    s8 pcmbuf[PCM_DMA_BUF];
#endif
} SoundArea;

#define SOUND_MODE_REVERB_VAL   0x0000007f
#define SOUND_MODE_REVERB_SET   0x00000080
#define SOUND_MODE_MAXCHN       0x00000f00
#define SOUND_MODE_MAXCHN_SHIFT 8
#define SOUND_MODE_MASVOL       0x0000f000
#define SOUND_MODE_MASVOL_SHIFT 12
#define SOUND_MODE_FREQ_05734   0x00010000
#define SOUND_MODE_FREQ_07884   0x00020000
#define SOUND_MODE_FREQ_10512   0x00030000
#define SOUND_MODE_FREQ_13379   0x00040000
#define SOUND_MODE_FREQ_15768   0x00050000
#define SOUND_MODE_FREQ_18157   0x00060000
#define SOUND_MODE_FREQ_21024   0x00070000
#define SOUND_MODE_FREQ_26758   0x00080000
#define SOUND_MODE_FREQ_31536   0x00090000
#define SOUND_MODE_FREQ_36314   0x000a0000
#define SOUND_MODE_FREQ_40137   0x000b0000
#define SOUND_MODE_FREQ_42048   0x000c0000
#define SOUND_MODE_FREQ_SHIFT   16
#define SOUND_MODE_DA_BIT_9     0x00800000
#define SOUND_MODE_DA_BIT_8     0x00900000
#define SOUND_MODE_DA_BIT_7     0x00a00000
#define SOUND_MODE_DA_BIT_6     0x00b00000
#define SOUND_MODE_DA_BIT_SHIFT 20


//�\���O�f�[�^�̍\��
typedef struct {
    u8 trks;            //�g���b�N��
    u8 blks;            //�u���b�N��
    u8 prio;            //�v���C�I���e�B
    u8 rvrb;            //���o�[�u
    ToneData *tone;     //���F�e�[�u��
    u8 *part[1];
} SongHeader;


//�~���[�W�b�N�v���C���[�E���[�N�G���A�̍\��
typedef struct {
    SongHeader *song;   //���݂̃\���O�|�C���^
    u32 stat;           //�X�e�[�^�X
    u8 mtrk;            //�g���b�N��
    u8 prio;            //���݂̃v���C�I���e�B
    u8 d1[2];
    u32 clock;          //���t�J�n����̒ʎZ�N���b�N
    u8 d2[48];
} MusicPlayerArea;

//�~���[�W�b�N�v���C���[�̊e�g���b�N�̍\��
typedef struct {
    u8 d[80];
} MusicPlayerTrack;

#define MAX_MUSICPLAYER_TRACK       16
#define MUSICPLAYER_STATUS_TRACK    0x0000ffff
#define MUSICPLAYER_STATUS_PAUSE    0x80000000


#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif // _AGB_SOUND_H
