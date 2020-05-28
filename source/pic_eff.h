/*==================================================================*/
/*                  pic_eff.h                                       */
/*             �@�@ �摜�G�t�F�N�g�֘A                              */
/*                  by tomo                                         */
/*==================================================================*/

//BG�G�t�F�N�g�p�ϐ��\����
typedef struct{
  u8 eff_no;			//�G�t�F�N�g�ԍ��@���K�{
  u16 *bg_adrs;			//�a�f�A�h���X(�v�Z�p�e���|����)�@���K�{
  u16 *pal_adrs;		//�p���b�g�o�̓A�h���X(�v�Z�e���|����)�@���K�{
  u16 *map_adrs;		//�}�b�v�o�̓A�h���X(BGmode0-2)
  u8 *chr_adrs;			//�L�����o�̓A�h���X(BG,OBJ���p)
  u16 mode;				//�F�̃��[�h 0:255,1:223,2:15,3:32g,4:15g,5:b/w
  u16 out_mode;			//�o�͂a�f���[�h 0;256�FBG,1:16�FBG,2:256obj,3:16obj
  u8 pal_p;				//0-15�i�ǂ̃p���b�g����g�����B�ʏ�0)�@���K�{
  u8 eax,eay,eah,eav;	//�G�t�F�N�g������n�_�w�x�Ƃ�������̂g�u���@���K�{
  u8 sah,sav;			//�摜�S�̂̑傫���@���K�{
  u8 pat;				//�G�t�F�N�g�p�^�[��(0:�ʏ�A1-4:�p�^�[��1-4)
}BE_ST;
/*==================================================================*/
/*==================================================================*/
#define COL_MODE_255	0		//255�F
#define COL_MODE_223	1		//223�F(2�p���b�g)
#define COL_MODE_15		2		//15�F
#define COL_MODE_32G	3		//�O���[32�F
#define COL_MODE_15G	4		//�O���[15�F
#define COL_MODE_BW		5		//����2�F

#define OUT_COL_256BG	0		//256�FBG
#define OUT_COL_16BG	1		//16�FBG
#define OUT_COL_256OBJ	2		//256�FOBJ
#define OUT_COL_16OBJ	3		//16�FOBJ
/*==================================================================*/
/*==================================================================*/
#define EFF_KOUSHI		1	//�i�q��
#define EFF_PASUTERU_POKE	2	//�p�X�e����(�|�P����OBJ��p��)
#define EFF_FOCUS_H		3	//�t�H�[�J�X�i�����C���j
#define EFF_FOCUS_V		4	//�t�H�[�J�X�i�c���C���j
#define EFF_FOCUS		5	//�t�H�[�J�X�i�c���j
#define EFF_GRAY32		6	//�O���[�X�P�[���i32�F�j
#define EFF_FUCHI_H		7	//�ӂ��ǂ苭���i�����C���j
#define EFF_FUCHI_V		8	//�ӂ��ǂ苭���i�c���C���j
#define EFF_RINKAKU		9	//�֊s��肾��
#define EFF_SHIRUETTO	10	//�V���G�b�g(����)
#define EFF_KYOUCYOU_BW	11	//���������摜(����)
#define EFF_SUMIE_POKE	12	//�n�G���i�����j(�|�P����OBJ��p��)
#define EFF_AWAI_POKE	13	//�W�F���i�����j(�|�P����OBJ��p��)
#define EFF_RINKAKU_KYOUCYO	30	//�֊s�̂݋���
#define EFF_HANTEN		31	//���Ô��]
#define EFF_FUCHI2_H	32	//��������Łi���j
#define EFF_FUCHI2_V	33	//��������Łi�c�j
#define EFF_GRAY15		34	//�O���[�X�P�[���P�T�F
#define EFF_PASUTERU	35	//�p�X�e����
#define EFF_SUMIE		36	//�n�G���i�����j
#define EFF_AWAI		37	//�W�F���i�����j

/*==================================================================*/
/*==================================================================*/
#define R_MAX 29					//�ł����邢���̒l
#define G_MAX 29
#define B_MAX 29

#define R_MIN 6						//�ł��Â��Ƃ��̒l
#define G_MIN 6
#define B_MIN 6

#define R_MID ((R_MAX-R_MIN)/2)		//�����炢�̒l
#define G_MID ((G_MAX-G_MIN)/2)
#define B_MID ((B_MAX-B_MIN)/2)
/*==================================================================*/
/*==================================================================*/
#define RGB_BLACK	0x0000				//���iRGB:�e0)
#define RGB_WHITE	0x7fff				//���iRGB:�e1f)
#define NUKE_BLACK	0x8000				//�k�P�F���iRGB:�e0�A�k�P�F)

#define RGB_GRAY	0x3def				//�D�F�iRGB:�ef)

/*==================================================================*/
/*==================================================================*/
#define PA_H	(8*8)				//�|�P�����\���G���A�̉��̑傫��
#define PA_V	(8*8)				//�|�P�����\���G���A�̏c�̑傫��

/*==================================================================*/
/*==================================================================*/
#define GRAY_COLOR_UP		3		//���(�w�i)�ɍ��킹�����邳�␳��(�O���[�G�t�F�N�g)
#define SUMIE_COLOR_UP		2		//���(�w�i)�ɍ��킹�����邳�␳��(�n�G�G�t�F�N�g)
#define SUMIE_WHITE_DOWN	4		//���(�w�i)�ɍ��킹�������x���_�E����(�n�G�G�t�F�N�g)

/*==================================================================*/
/*==================================================================*/
extern void poke_bit_effect(BE_ST *est);

/*==================================================================*/
/*==================================================================*/
