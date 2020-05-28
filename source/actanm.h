//**********************************************************************
//		act_anm.h
//**********************************************************************
#ifndef	ACTANM_H
#define	ACTANM_H

#include "actanm.def"

extern u8 ObjPalManStart;										// ��گ��Ȱ�ެNo�J�n
extern u32 AffineWorkUse;							// �̨��Ȱ�ެ

extern void	ActAnm( actWork * );								// �������Ҳ�
extern void	ActAnmChg( actWork *, u8 );							// ��Ұ���No��ݼ�
extern void	ActAnmChkChg( actWork *, u8 );						// �����L��
extern void ActAnmOffsChg( actWork *, u8 );						// ��ҵ̾��No��ݼ�
extern void	ActAffAnmChg( actWork *, u8 );						// �g�k���No��ݼ�
extern void	ActAffAnmChkChg( actWork *, u8 );					// �����L��
extern void	ActAffAnmHoldChg( actWork *, u8 );					// �g�k���No��ݼފg�k���ێ�
extern void	ActAffAnmHoldChkChg( actWork *, u8 );				// �����L��
extern void ActCharNumSet( actWork * );							// ������޾��(��]��)

extern void	AffineWorkManInit( void );							// �̨��Ȱ�ެ ܰ�������
extern void	AffineNumSet( actWork * );							// �̨�No ���
extern u8	AffineWorkNumGet( void );							// �̨��Ȱ�ެ �g�p�\���擾
extern void	AffineWorkNumDel( u8 );								// �̨��Ȱ�ެ �g�pNo�폜
extern void OBJ_AffineWorkCalc(u8,s16,s16,s16);					// �̨�ܰ���������

extern u16	CellSet( const CellData * );						// �ٓo�^�P��
extern void	CellSets( const CellData * );						// �ٓo�^����
extern void	CellDel( u16 );										// �ٓo�^�ԍ��폜
extern void	CellManAllInit( void );								// ���Ȱ�ެ������
extern u16	CellCharNumGet( u16 );								// �o�^�ԍ� -> �������
extern u16	CellCharNumIdGet( u16 );							// ������� -> �o�^�ԍ�

extern void	ObjPalManInit( void );								// ��޼ު��گ��Ȱ�ެ������
extern u8	ObjPalSet( const PalData * );								// ��޼ު��گľ�ĒP��
extern void	ObjPalSets( const PalData * );							// ��گľ�ĕ���
extern u8	ObjPalNumSet( u16 );								// ��گ�ID�o�^
extern u8	ObjPalNumGet( u16 );								// �o�^���� -> ��گ�����
extern u16	ObjPalNumIdGet( u8 );								// ��گ�No -> ID
extern void	ObjPalRegNumDel( u16 );								// �o�^�ԍ��폜

extern void SetActOamPat( actWork *, const OAMPatSt * );		// OAM �߯ľ��
extern u8	ActOamSet( actWork *, u8 * );						// OAM ���
extern u8	OamPatDevelop( actWork *, ActOamData *, u8 * );		// OAM �߯ēW�J

#endif															// ACTANM_H
