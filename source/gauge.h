
#include "param.h"
#include "client.h"

#ifndef __GAUGE_H__
#define __GAUGE_H__


#define GAUGE_CHANGE	0	//�ꎞ�f�o�b�O�p



//==================================================================
//			�Q�[�W�v�Z��\�� �����ꗗ
//
//    �� HP_Work:ܰ��Ƃ��Ďg�p ��0����ܰ��̏��������s���� �����0�����Ă�������
//
// ���� : MaxHP:�ő�HP
//		  NowHP:����HP(��Ұ�ނ��󂯂�O)
//		  beHP:�󂯂���Ұ��(�񕜂���ꍇ��ϲŽ�œ����)
//        *HP_Work : ܰ��̱��ڽ(�ް�ނ̓r���o�߂��ް�������)
//								���0�`3�ޯ� �������A 4�`7 �׸ށA 8�`15 ���q �Ŏg�p����
//		  *TransPos:�ް�ނ�\�������齸ذ��ް��̍��W
//		  TopChr:�ް�ނ̷�ׂ�į�߱��ڽ
//		  *pl_buf:��گ��ޯ̧
// �߂�l : -1:�I��		0�ȏ�̐��l:�ް�ނƈꏏ�ɕ\�������鐔�l�̒l
//==================================================================

//----------------------------------------------------------
//	�Q�[�W�\����
//----------------------------------------------------------
typedef struct
{
#if 0	//2002.09.14(�y)
	s16 MaxHP;			//�ő�HP(�ް��ϯ���l)
	s16 NowHP;			//���݂�HP(��Ұ�ނ��󂯂�O)
	s16 beHP;			//�󂯂���Ұ��(�񕜂���ꍇ��ϲŽ�œ����) �ϓ�������l
	u16 color:5;
	u16 CharNo;			//�ް�ނ̐擪�̷�׸��Ȱ�
#else
	s32 MaxHP;			//�ő�HP(�ް��ϯ���l)
	s32 NowHP;			//���݂�HP(��Ұ�ނ��󂯂�O)
	s32 beHP;			//�󂯂���Ұ��(�񕜂���ꍇ��ϲŽ�œ����) �ϓ�������l
	u32 color:5;
	u32 CharNo;			//�ް�ނ̐擪�̷�׸��Ȱ�
#endif
}GaugeDataBG;

typedef struct	//�ް�ތv�Zٰ�݂��ĂԑO�Ɉ����Ƃ��āA���炩�����ް���Ă��Ă�������
{
	s16 actno;			//�Ď��p�����̔z��ԍ�	������ܰ��ɍ��킹�邽�߂�s16
	s16 MaxHP;			//�ő�HP(�ް��ϯ���l)
	s16 NowHP;			//���݂�HP(��Ұ�ނ��󂯂�O)
	s16 beHP;			//�󂯂���Ұ��(�񕜂���ꍇ��ϲŽ�œ����) �ϓ�������l
	s16 HP_Work;		//��Ɨpܰ�
}GaugeDataOBJ;

typedef struct	//�ް�ތv�Zٰ�ݓ����ް��擾�B���ɊO������l�����Ă�邱�Ƃ͕K�v�Ȃ�
{
	s16 AfterHP;
	s16 resultHP;
}GaugeDataOBJ2;

//----------------------------------------------------------
//	�萔��`
//----------------------------------------------------------
//GaugeTransChrSet�Ŏg�p
enum{	//user
	G_MINE0,
	G_MINE1,
	G_ENEMY0,
	G_ENEMY1,
};
enum{	//mode
	G_ALL,
	G_HP,
	G_MAXHP,
	G_LV,
	G_NAME,
	G_HPGAUGE,
	G_EXPGAUGE,
	G_BALL,
	G_GETCHKBALL,
	G_STATUS,
	G_SAFARI,
	G_SAFARIBALL,
	G_SAFARIDEBUG,
};

//-- HP_Work�̏����l�Ƃ��Ďg�p --//
#define HP_WORK_INIT_VALUE	(-32768)


//----------------------------------------------------------
//	�}�N��
//----------------------------------------------------------
//---- �p�[�Z���e�[�W�ϊ� ----//
// NowPercentage:�ϊ����錻�݂�%  NowMaxPercent:�ϊ����錻�݂�Max%
// ConvPercent:�ϊ����Max%
//��)NowPercentage = 80, NowMaxParcent = 100, ConvParcent = 120 �̏ꍇ
//  ���� 80%/100% �� xxx%/120% �ɕϊ������Ƃ���xxx�����ߏo��
#define PERCENT_CONV(NowPercentage, NowMaxPercent, ConvPercent) \
			(ConvPercent * NowPercentage / NowMaxPercent)


//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern s16 GaugeBG(GaugeDataBG *gdata, s32 *HP_Work, u16 *TransPos, u16 *pl_buf);
extern void PutHPNumber(s16 MaxHP, s16 resultHP, u16 *TransPos);
extern void PutNumber(s16 number, u16 *TransPos, u8 yose);

//extern s16 GaugeOBJ(u8 actno, u8 req_data, u16 *pl_buf);
extern s32 GaugeOBJ(u8 client_no, u8 actno, u8 req_data, u16 *pl_buf);
extern u8 GaugeActorSet(u8);
extern u8 GaugeActorSetSafari(void);
extern void GaugeOFF(u8 actno);
extern void GaugeON(u8 actno);
extern void GaugePosSet(u8 actno, s16 xpos, s16 ypos);

//extern void GaugeDataSet(u8 actno, s16 MaxHP, s16 NowHP, s16 beHP);
extern void GaugeDataSet(u8 client_no, u8 actno, s32 MaxHP, s32 NowHP, s32 beHP);
extern void GaugeLevelTrans(u8 actno, u8 level);
extern void GaugeHPTrans(u8 actno, s16 hp, u8);

extern	void	GaugePokeNameTrans(u8 actno, PokemonParam *);

#if 0
extern void NumberON(u8 actno);
extern void NumberOFF(u8 actno);
extern void PutNumOBJ(u8 actno, u16 number);
extern void NumberPosSet(u8 actno, s16 xpos, s16 ypos);
extern u8 NumberActorSet(void);
extern u8 NumberActorSet2(void);
#endif

extern void PokeListGaugeDataSet(u8 actno, s16 MaxHP, s16 NowHP, s16 beHP);
extern s16 PokeListGauge(u8 actno, u16 *pl_buf);

extern void GaugeTransChrSet(u8 actno, PokemonParam *PokePara, u8 mode);
extern void GaugeDefaultPosSet(u8 client_no);
extern void GaugePriSet(u8 actno, u8 pri);

extern u8 GaugeArrowActorSet(void);
extern void GaugeArrowON(u8 client_no);
extern void GaugeArrowOFF(void);
extern void GaugeArrowCheckON(u8 client_no);
extern void GaugeGetBallTrans(u8 actno, u8);
extern void HPNumGaugeChangePut(void);
extern u8 TemotiBallPut(u8 client_no, BallGaugePara *bgd, u8 zahyou, u8 syokai);
extern void TemotiBallOff(u8 id);
extern void GaugeHardPriSet(u8 hard_pri);
extern u8 GetNumDotto(s16 NowHP, s16 MaxHP, u8 GaugeMaxDot);

//----------------------------------------------------------
//	�O���f�[�^
//----------------------------------------------------------
#if GAUGE_CHANGE
extern const CellData BattleGaugeDataCell_1vs1;
extern const CellData BattleGaugeDataCellMine2[];
extern const CellData BattleGaugeDataCellEnemy2[];
#else
extern const CellData MineGaugeCell_1vs1;
extern const CellData EnemyGaugeCell_1vs1;
extern const CellData MineGaugeCell_2vs2[];
extern const CellData EnemyGaugeCell_2vs2[];
#endif
extern const CellData MineGaugeCell_Safari;				//add by soga 2002.06.01
extern const PalData BattleGaugeDataPal[];
extern const actHeader PokeListGaugeHeader[];
extern const CellData PokeListGaugeCell[];
extern const PalData PokeListGaugePal[];
extern const actHeader GaugeMineHeader[];
extern const actHeader GaugeEnemyHeader[];
extern const CellData BattleGaugeCursorCell;
extern const actHeader GaugeCursorHeader[];
extern const CellData BattleGaugePartsCell[];
extern u8 GetGaugeDottoColor(s16 NowHP, s16 MaxHP);

//----------------------------------------------------------
//	�Q�[�W�p�[�c�擾ID
//----------------------------------------------------------
enum{
	GP_SPACE,
	GP_HP,
	GP_HPBAR = 3,
	GP_EXPBAR = 0xc,
	GP_DOKU = 0x15,
	GP_MAHI = 0x18,
	GP_NEMURI = 0x1b,
	GP_KOORI = 0x1e,
	GP_YAKEDO = 0x21,
	GP_KUUHAKU = 0x27,
	GP_CORON = 0x2a,
	GP_TENNASI = 0x2b,
	GP_DAKUTEN,
	GP_HANDAKUTEN,
	GP_LV = 0x2e,
	GP_HPBAR_YELLOW = 0x2f,
	GP_HPBAR_RED = 0x38,
	GP_HPHEAD = 0x41,
	GP_BALLON = 0x42,
	GP_BALLOFF,	//�莝���Ȃ�
	GP_BALLERR,	//�X�e�[�^�X�ُ�
	GP_BALLDEAD,	//�C��
	GP_GETBALL,	//�ޯčς�ϰ�(���ɐ}�ӂɋL�^����Ă��߹�݂�����ϰ�)
	GP_DOKU1 = 0x47,
	GP_MAHI1 = 0x4a,
	GP_NEMURI1 = 0x4d,
	GP_KOORI1 = 0x50,
	GP_YAKEDO1 = 0x53,
	GP_DOKU2 = 0x56,
	GP_MAHI2 = 0x59,
	GP_NEMURI2 = 0x5c,
	GP_KOORI2 = 0x5f,
	GP_YAKEDO2 = 0x62,
	GP_DOKU3 = 0x65,
	GP_MAHI3 = 0x68,
	GP_NEMURI3 = 0x6b,
	GP_KOORI3 = 0x6e,
	GP_YAKEDO3 = 0x71,
	GP_HPNUMCLOSE = 0x74,
	GP_HPBARCLOSE,

	GP_ARROW,	//����͌�ō폜
};



#endif __GAUGE_H__
