//**********************************************************************
//		evobj.h
//**********************************************************************
#ifndef	EVOBJ_H
#define	EVOBJ_H

#include "actor.h"
#include "actanm.h"
#include "evobj.def"
#include "evobjdef.h"
#include "evobjmv.h"
#include "evobjdat.h"
#include "evobjcmd.h"
#include "evobjeff.h"
#include "ev_trai.h"

#undef	EXT
#ifdef 	EVOBJ_H_PROTO
#define	EXT	/**/
#else
#define	EXT extern
#endif

EXT EventOBJ EvObj[];												// ����� OBJ ܰ�
EXT MvCmd	MyMoveState;											// ���@����Ǘ�
EXT const u8 EvObjRefPalNoTbl[];									// �f�荞����گ�Noð���

EXT void EvObjWorkInit(EventOBJ*);									// ����� OBJ ܰ�������
EXT void EvObjWorkAllInit(void);									// ����� OBJ ܰ��S������
EXT void EvObjInit(void);											// ܰ��S������
EXT u8 EvObjEmptyWorkSeek(void);									// ��ܰ����
EXT u8 EvObjIdSeek(u8,u8,u8);										// ID -> ܰ�����
EXT u8 EvObjIdPosGet(u8,u8,u8,u8*);									// ܰ����� & �׸�
EXT u8 EvObjWorkMapPosGet(s16,s16);									// ϯ�ߍ��W -> ܰ�����
EXT u8 EvObjAddWork(const evObjData*,u8,u8);						// ����� OBJ ܰ����
EXT u8 EvObjWorkAdd(u8);											// ����� OBJ ܰ��ǉ�
EXT void EvObjDel(EventOBJ*);										// ����� OBJ �폜
EXT void EvObjDelId(u8,u8,u8);										// ����� OBJ ID �폜
EXT void EvObjMapDel(void);											// ��l���ȊO�폜
EXT u8 EvObjAdd(const evObjData*,actHeader*,u8,u8,s16,s16);			// ����� OBJ�o�^
EXT u8 EvObjSxyAdd(const evObjData*,u8,u8,s16,s16);					// SXY�ް� -> �����OBJ�o�^
EXT u8 EvObjSxyAddNow(const evObjData*);							// SXY�ް� -> �����OBJ�o�^
EXT u8 EvObjMakeSxyAdd(u8,u8,u8,s16,s16,u8);						// SXY����->�����OBJ�o�^
EXT u8 EvObjAddId(u8,u8,u8);										// ID -> �����OBJ�o�^
EXT void EvMoveMakeHead(u16,void (*)(actWork*),actHeader*,OAMPatSt**);	// ����ͯ�ސ���
EXT u8 EvObjMakeAddAct(u16,void (*)(actWork*),s16,s16,u8);			// ����ͯ�ސ���,�ǉ�
EXT u8 EvObjDmyAddAct(u8,u8,s16,s16,u8,u8);							// ̨����OBJ��а�����ǉ�
EXT void EvMakeHead(u16,u16,actHeader*,OAMPatSt**);					// ����ͯ�ސ��� ����
EXT void MakeSxyActHead(const evObjData*,actHeader*,OAMPatSt**);	// SXY�ް� -> ����ͯ��
EXT void EvObjRecover(s16,s16);										// ����� OBJ ���A
EXT void EvObjAddControl(s16,s16);									// ϯ���ް� OBJ �o�^
EXT void EvObjPosChg(EventOBJ*,s16,s16);							// ���W�X�V
EXT void EvObjPosInit(EventOBJ*,s16,s16);							// ���W������
EXT	void EvObjPosAllChg(EventOBJ*,s16,s16);							// ���W�X�V
EXT	void EvObjPosAllChgId(u8,u8,u8,s16,s16);						// ���W�X�V
EXT void EvObjPosMove(EventOBJ*,s16,s16);							// ���W�ړ�, �X�V
EXT void EvObjPosRenew(EventOBJ*);									// �����W�X�V
EXT void EvObjMapChgCont(void);										// ϯ�ߐ؂�ւ����W�C��
EXT u8 EvObjHitCheck(u16,u16,u8);									// ����ėp�@OBJ���W����
EXT void EvObjMainControl(s16,s16);									// ����� OBJ ���۰�
EXT u8 SetScrollTaskAct(u8);										// ����������۰�������
EXT void FldScrollPosInit(void);									// ��۰ُ�����
EXT void FldScrollActChg(u8);										// ��۰ٱ�����ݼ�
EXT u8 FldScrollActNo(void);										// ��۰ٱ�������
EXT void FldScrollActStop(void);									// ��۰ٽį��
EXT void EvObjPalManInit(void);										// ��گ��Ȱ�ެ������
EXT void EvObjPalSet(u16);											// ����� OBJ �p��گēo�^
EXT void EvObjPalSets(u16*);										// ��گēo�^ ����
EXT	void EvObjStayPalSet(u16,u8);									// �풓��گľ��
EXT void EvObjStayPalSets(const u16*,u8,u8);						// �풓��گľ�� ����
EXT u8 EvObjPalTblSeek(u16);										// ��گ�ð��ټ��
EXT void EvObjHeroPalSet(u16,u8);									// ��l����گľ��
EXT void EvObjSpPalSet(u16,u8);										// ������گľ��
EXT u8 CopyActPri0(actWork*,s16,s16,u8);							// ��߰����
EXT u8 CopyActPri1(actWork*,s16,s16,u8);							// ��߰����
EXT void EvObjSiteSet(EventOBJ*,u8);								// �������
EXT void EvObjGraphicInit(u8);										// OBJ���̨���֘A������
EXT u16 EvObjPalIdGet(u8);											// ��گ�No -> ID
EXT	void EvObjChgParam(EventOBJ*,u8);								// �����OBJ������ύX
EXT void EvObjChgParamId(u8,u8,u8,u8);								// �����OBJ������ύX
EXT	void EvObjChgSite(EventOBJ*,u8);								// �����OBJ�����ύX
EXT	void EvObjChgSiteId(u8,u8,u8,u8);								// �����OBJ�����ύX
EXT void MvCmdChgSite(MvCmd*,u8);									// MvCmd�^�����ύX
EXT void EvObjSeedSetParam(EventOBJ*,actWork*);						// ��p������ύX
EXT SXYObjHead* SxyObjHeadGet(u8);									// OBJ�ð��ͯ�ގ擾
EXT void EvObjCodeWorkChk(EventOBJ*);								// OBJ ����ܰ�����
EXT void EvObjBanishSet(u8,u8,u8,u8);								// OBJ ��Ư�����
EXT void EvObjIdWorkSet(EventOBJ*,void*,void*,void*);				// ID ܰ��i�[
EXT void EvObjMapPosMvFlagOn(s16,s16);								// ϯ�ߍ��WOBJ�׸�ON
EXT void EvObjPriChg(u8,u8,u8,u8);									// �\����ײ��è�ύX
EXT void EvObjPriChgOff(u8,u8,u8);									// �\����ײ��è�׸޵�
EXT void EvObjSetOffset(u8,u8,u8,s16,s16);							// �\���̾�ľ��
EXT const VM_CODE* EvObjEvLabelGet(u8,u8,u8);						// ��������َ擾
EXT u16	EvObjEvFlagGet(u8,u8,u8);									// ������׸ގ擾
EXT u8 EvObjEvTypeGet(u8,u8,u8);									// ��������ߎ擾
EXT u8 EvObjEvWorkGet(u8,u8,u8);									// �����ܰ��擾
EXT const VM_CODE* EvObjNoEvLabelGet(u8);							// ��������َ擾 ܰ�����
EXT u16	EvObjNoEvFlagGet(u8);										// ������׸ގ擾 ܰ�����
EXT u8 EvObjNoEvTypeGet(u8);										// ��������ߎ擾 ܰ�����
EXT u8 EvObjNoEvWorkGet(u8);										// �����ܰ��擾	ܰ�����

EXT void EvObjSxyIdPosChange( u8 obj_id, u8 map_id, u8 div_id );
EXT void EvObjSxyMvCodeChange( EventOBJ * obj, u8 code );
EXT void EvObjSxyPosChange( EventOBJ * obj );

#define	EvObjId(no)				(EvObj[no].objID)					// ܰ����� -> OBJ ID
#define	EvObjMapId(no)			(EvObj[no].mapID)					// ܰ����� -> MAP ID
#define	EvObjDivId(no)			(EvObj[no].divID)					// ܰ����� -> DIV ID
#define	EvObjWorkAdrs(o,m,d)	(&EvObj[EvObjIdSeek(o,m,d)])		// �eID -> ܰ����ڽ�擾
#define	EvObjSpWorkAdrs(o)		(&EvObj[EvObjIdSeek(o,0,0)])		// ����ID -> ܰ����ڽ
#define EvObjActNo(n)			(EvObj[n].actno)					// 

#define	EvObjDelNo(no)			(EvObjDel(&EvObj[no]))				// �����OBJ No �폜
#define	EvObjWorkInitNo(no)		(EvObjWorkInit(&EvObj[no]))			// �����OBJ No ܰ�������

#define	EvObjRefPalNo(no) 		(EvObjRefPalNoTbl[no])				// �f�肱����گ�

#define	EvObjNoBanishOn(n)		(EvObj[n].banish_sw = 1)			// ��Ư���׸� ON
#define	EvObjNoBanishOff(n)		(EvObj[n].banish_sw = 0)			// ��Ư���׸� OFF
#define	EvObjNoBanishSet(n,b)	(EvObj[n].banish_sw = b)			// ��Ư���׸޾��
#define	EvObjScrSwOn(n)			(EvObj[n].scr_main_sw = 1)			// ��۰ٽ��� ON

#define	EvObjAnmChgSwSet(n,sw)	(EvObj[n].anm_change_sw = sw)		// �ٱ�ҽ������

#undef EXT
#endif // EVOBJ_H
