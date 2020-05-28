//**********************************************************************
//	evobjdat.h
//**********************************************************************
#ifndef	EVOBJDAT_H
#define	EVOBJDAT_H

#include	"evobj.def"
#include	"evobjdat.def"

#undef	EXT
#ifdef	EVOBJDAT_H_PROTO
#define	EXT	/**/
#else
#define	EXT	extern
#endif

EXT	void (* const EvObjMoveTbl[])(actWork*);					// ����ð���
EXT const u8 EvObjMoveLimitFlagTbl[];							// �ړ�����ð���
EXT const u8 EvObjDefaultSiteTbl[];								// ��{����ð���
EXT	const SXYObjHead * const SxyObjHeadTbl[];					// OBJ ͯ��ð���
EXT const actHeader * const EvActHeadTbl[];						// ���ͯ��
EXT	const PalData EvObjPalTbl[];								// ��گ�ð���
EXT const STDATA EvObjHeroPalStTbl[];							// ��l����گ��ް�ð���
EXT const STDATA EvObjSpPalStTbl[];								// ������گĽð��ð���
EXT const u16 * const EvObjStayPalTbl[];						// �풓��گ�ð���
EXT const actAnmData * const SeedCellTransTbl[];				// ���
EXT const u8 * const SeedPalNoTbl[];							// ����گ�
EXT const u8 * const SeedObjCodeTbl[];							// ��OBJ����

#define EvObjDefaultSite(code)	(EvObjDefaultSiteTbl[code])		// ��{����
#define	SxyObjHeadAdrs(c) ((SXYObjHead*)SxyObjHeadTbl[c])		// SXY OBJ �ð�����ڽ
#define	EvActHeadGet(c) ((actHeader*)EvActHeadTbl[c])			// ���ͯ��ð��ٱ��ڽ�擾
#define SeedCellTransGet(s) (SeedCellTransTbl[s])				// ��׾َ擾
#define SeedPalNoGet(s,g) (SeedPalNoTbl[s][g])					// ����گĎ擾
#define SeedObjCodeGet(s,g) (SeedObjCodeTbl[s][g])				// ��OBJ����

#undef	EXT
#endif	// EVOBJDAT_H
