//**********************************************************************
//		evobjmv.h
//**********************************************************************
#ifndef EVOBJMV_H
#define	EVOBJMV_H

#include "evobj.def"

#undef	EXT
#ifdef 	EVOBJMV_H_PROTO
#define	EXT	/**/
#else
#define	EXT extern
#endif

EXT	void EvObj_DmyMove(actWork*);							// ���얳��
EXT void EvObj_RandomMove(actWork*);						// ����шړ�
EXT void EvObj_DirectMove(actWork*);						// �����ς�
EXT void EvObj_VMove(actWork*);								// ���������ړ�
EXT void EvObj_HMove(actWork*);								// ���������ړ�
EXT void EvObj_DirectSite(actWork*);						// �Œ����
EXT void EvObj_Seed(actWork*);								// ����
EXT void EvObj_DirectVSite(actWork*);						// ��������
EXT void EvObj_DirectHSite(actWork*);						// ��������
EXT void EvObj_DirectULSite(actWork*);						// �㍶����
EXT void EvObj_DirectURSite(actWork*);						// ��E����
EXT void EvObj_DirectDLSite(actWork*);						// ��������
EXT void EvObj_DirectDRSite(actWork*);						// ���E����
EXT void EvObj_DirectLVSite(actWork*);						// �㉺������
EXT void EvObj_DirectRVSite(actWork*);						// �㉺�E����
EXT void EvObj_DirectUHSite(actWork*);						// ���E�����
EXT void EvObj_DirectDHSite(actWork*);						// ���E������
EXT void EvObj_DirectSpinL(actWork*);						// ��]����������
EXT void EvObj_DirectSpinR(actWork*);						// ��]�����E���
EXT void EvObj_Route2Pos(actWork*);							// ٰ�2
EXT void EvObj_RouteURLD(actWork*);							// ٰ�3 URLD
EXT void EvObj_RouteRLDU(actWork*);							// ٰ�3 RLDU
EXT void EvObj_RouteDURL(actWork*);							// ٰ�3 RLDU
EXT void EvObj_RouteLDUR(actWork*);							// ٰ�3 DURL
EXT void EvObj_RouteULRD(actWork*);							// ٰ�3 LDUR
EXT void EvObj_RouteLRDU(actWork*);							// ٰ�3 LRDU
EXT void EvObj_RouteDULR(actWork*);							// ٰ�3 DULR
EXT void EvObj_RouteRDUL(actWork*);							// ٰ�3 RDUL
EXT void EvObj_RouteLUDR(actWork*);							// ٰ�3 LUDR
EXT void EvObj_RouteUDRL(actWork*);							// ٰ�3 UDRL
EXT void EvObj_RouteRLUD(actWork*);							// ٰ�3 RLUD
EXT void EvObj_RouteDRLU(actWork*);							// ٰ�3 DRLU
EXT void EvObj_RouteRUDL(actWork*);							// ٰ�3 RUDL
EXT void EvObj_RouteUDLR(actWork*);							// ٰ�3 UDLR
EXT void EvObj_RouteLRUD(actWork*);							// ٰ�3 LRUD
EXT void EvObj_RouteDLRU(actWork*);							// ٰ�3 DLRU
EXT void EvObj_RouteUL(actWork*);							// ٰ�4 UL
EXT void EvObj_RouteDR(actWork*);							// ٰ�4 DR
EXT void EvObj_RouteLD(actWork*);							// ٰ�4 LD
EXT void EvObj_RouteRU(actWork*);							// ٰ�4 RU
EXT void EvObj_RouteUR(actWork*);							// ٰ�4 UR
EXT void EvObj_RouteDL(actWork*);							// ٰ�4 DL
EXT void EvObj_RouteLU(actWork*);							// ٰ�4 LU
EXT void EvObj_RouteRD(actWork*);							// ٰ�4 RD
EXT void EvObj_CopyMove(actWork*);							// ���^��
EXT void EvObj_CopyMoveGrass(actWork*);						// ���^����
EXT void EvObj_HideWood(actWork*);							// �B�ꖪ��
EXT void EvObj_HideRock(actWork*);							// �B�ꖪ��
EXT void EvObj_HideSand(actWork*);							// �B�ꖪ��
EXT void EvObj_StayWalk1(actWork*);							// ���̏����
EXT void EvObj_StayWalk2(actWork*);							// ���̏����
EXT void EvObj_StayWalk4(actWork*);							// ���̏����
EXT void EvObj_Vanish(actWork*);							// ����

EXT u8 EvActAnmNoStop(u8);									// �ٱ������ ��~
EXT u8 EvActAnmNoWalk1(u8);									// �ٱ������ 1�ޯ�
EXT u8 EvActAnmNoWalk2(u8);									// �ٱ������ 2�ޯ�
EXT u8 EvActAnmNoWalk4(u8);									// �ٱ������ 4�ޯ�
EXT u8 EvActAnmNoWalk8(u8);									// �ٱ������ 8�ޯ�
EXT u8 EvActAnmNoSwimJump(u8);								// �ٱ������ �g���ެ���
EXT u8 EvActAnmNoFtUp(u8);									// �ٱ������ ������Ա���
EXT u8 EvActAnmNoRtUp(u8);									// �ٱ������ ر��Ա���
EXT u8 EvActAnmNoFtDown(u8);								// �ٱ������ ��������޳�
EXT u8 EvActAnmNoRtDown(u8);								// �ٱ������ ر����޳�
EXT u8 EvActAnmNoWheelie(u8);								// �ٱ������ ��ذ
EXT u8 EvActAnmNoFishingStart(u8);							// �ٱ������ �ނ�J�n
EXT u8 EvActAnmNoDash(u8);									// �ٱ������ �ޯ��
EXT u8 EvActAnmNoFishingEnd(u8);							// �ٱ������ �ނ�I��
EXT u8 EvActAnmNoFishingHit(u8);							// �ٱ������ �ނ�˯�

EXT void EvObjMvCodeChg(EventOBJ*,u8);						// ���캰�ޕύX
EXT u8 EvObjMvCodeStopSite(u8);								// ���캰��	��~

EXT u8 EvObjMoveChkSite(EventOBJ*,u8);						// �ړ�����
EXT u8 EvObjMoveChk(EventOBJ*,s16,s16,u8);					// �ړ�����
EXT u8 EvObjMoveHitChk(EventOBJ*,s16,s16);					// OBJ �����蔻��
EXT u8 EvObjSeedEffectCheck(u8,u8,u8);						// ����OBJ�̪������
EXT void EvObjSeedEffectOff(u8,u8,u8);						// ����OBJ�̪�ĵ�

EXT void SitePosMove(u8,s16*,s16*);							// ���W�ړ�
EXT void SitePosMoveFull(u8,s16*,s16*);						// ���W�ړ� �ْP��
EXT void SitePosAdd(u8,s16*,s16*,s16,s16);					// ���W�ړ� ����
EXT void MapCellSizePos(s16,s16,s16*,s16*);					// ϯ��->�ޯč��W�擾
EXT void MapCellSizePosDiff(s16,s16,s16*,s16*);				// ϯ��->�ޯč��W�擾
EXT void MapCellSizePosDiffOffs(s16*,s16*,s16,s16);			// ϯ��->�ޯč��W�擾
EXT void MapCellSizeDiff(s16*,s16*);						// �ٻ��ޏC�����W�擾
EXT void EvObjSitePosMoveWork(EventOBJ*,u8,s16*,s16*);		// �����W�ړ�ܰ����
EXT u8 SiteOpposite(u8);									// ���� -> ������������
EXT u8 FrontSite(u8,u8);									// ���ʕ���
EXT u8 FrontChgSite(u8,u8,u8);								// ���ʕ����ϊ�

EXT u8 AcmdSetChk(EventOBJ*);								// ����޾������
EXT u8 AcmdActionChk(EventOBJ*);							// ����ޓ�������
EXT u8 AcmdSet(EventOBJ*,u8);								// ����޾��
EXT void AcmdForceSet(EventOBJ*,u8);						// ��������޾��
EXT void AcmdFree(EventOBJ*);								// ����މ���
EXT void AcmdFreeWork(EventOBJ*);							// �����ܰ��ر
EXT u8 AcmdEndChk(EventOBJ*);								// ����ޏI������
EXT	u8 AcmdEnd(EventOBJ*);									// �I������ & ����
EXT u8 AcmdCode(EventOBJ*);									// ����޺��ގ擾
EXT void AcmdBranch(EventOBJ*,actWork*,u8 (*)(EventOBJ*,actWork*));	// �����ٰ�ݼ��ݽ

EXT u8 AcmdCodeStopSite(u8);								// ��������
EXT u8 AcmdCodeWalk05(u8);									// 0.5�ޯĈړ�����
EXT u8 AcmdCodeWalk1(u8);									// 1�ޯĈړ�����
EXT u8 AcmdCodeWalk2(u8);									// 2�ޯĈړ�����
EXT u8 AcmdCodeWalk3(u8);									// 3�ޯĈړ�����
EXT u8 AcmdCodeWalk4(u8);									// 4�ޯĈړ�����
EXT u8 AcmdCodeWalk8(u8);									// 8�ޯĈړ�����
EXT u8 AcmdCodeDash2(u8);									// 2�ޯ��ޯ���ړ�����
EXT u8 AcmdCodeJump2(u8);									// 2�ټެ��ߺ���
EXT u8 AcmdCodeJump1(u8);									// 1�ټެ��ߺ���
EXT u8 AcmdCodeStayJump(u8);								// ���̏�ެ��ߺ���
EXT u8 AcmdCodeStayTurnJump(u8);							// ���̏�ެ��ߐU���������
EXT u8 AcmdCodeSwimJump(u8);								// �g���ެ��ߺ����
EXT u8 AcmdCodeStayWalk05(u8);								// 0.5�ޯĂ��̏��������
EXT u8 AcmdCodeStayWalk1(u8);								// 1�ޯĂ��̏��������
EXT u8 AcmdCodeStayWalk2(u8);								// 2�ޯĂ��̏��������
EXT u8 AcmdCodeStayWalk4(u8);								// 4�ޯĂ��̏��������
EXT u8 AcmdCodeTurnSiteSet(EventOBJ*,u8);					// �U�������p���޾��
EXT u8 AcmdCodeMtbFtUpSite(u8);								// ������Ա��ߺ���
EXT u8 AcmdCodeMtbFtUp(u8);									// ������Ա��ߺ���
EXT u8 AcmdCodeMtbFtDown(u8);								// ��������޳ݺ���
EXT u8 AcmdCodeMtbDaniel(u8);								// ��ƴٺ���
EXT u8 AcmdCodeMtbDaniel1(u8);								// ��ƴ�1����
EXT u8 AcmdCodeMtbDaniel2(u8);								// ��ƴ�2����
EXT u8 AcmdCodeMtbWheelie(u8);								// ��ذ����
EXT u8 AcmdCodeMtbWheelieStart2(u8);						// ��ذ�J�n�ړ�����
EXT u8 AcmdCodeMtbWheelie2(u8);								// ��ذ�ړ�����
EXT u8 AcmdCodeMtbWheelieEnd2(u8);							// ��ذ�I���ړ�2����

EXT void AcmdAction(EventOBJ*,actWork*);					// ��Ұ��ݺ���ޱ����
EXT u8 CmdAction(EventOBJ*,actWork*);						// ����ޱ����
EXT void CmdActionSet(EventOBJ*,actWork*,u8);				// ����ޱ���ݾ��

EXT void EvObjBanish(EventOBJ*,actWork*);					// OBJ�\������
EXT u8 EvMapAttrJumpChk(s16,s16,u8);						// �ެ��߱���ޭ��
EXT u8 EvObjHeightBgPri(u8);								// ����->BG��ײ��è
EXT void EvObjHeight(EventOBJ*);							// �������
EXT void EvActHeightSet(actWork*,u8);						// ������������
EXT void EvActPriControl(u8,actWork*,u8);					// ��ײ��è���
EXT u8 HeightHitChk(u8,u8);									// ����˯�����

EXT u8 EvObjPause(EventOBJ*);								// �߰��
EXT void EvObjPauseAll(void);								// �߰�� ALL
EXT	void EvObjPauseAlmost(u8);								// �߰�� �w��ȊOALL
EXT void EvObjPauseClr(EventOBJ*);							// �߰�޸ر
EXT void EvObjPauseClrAll(void);							// �߰�޸ر ALL

EXT void EvActMove1(actWork*,u8);							// ��� 1dot
EXT void EvActMove2(actWork*,u8);							// ��� 2dot
EXT void EvActMove4(actWork*,u8);							// ��� 4dot
EXT void EvActMove8(actWork*,u8);							// ��� 8dot
EXT s16 EvActMoveJumpVoffs(s16,u8);							// ��� �ެ���
EXT void EvActScrollVanish(actWork*,u8);					// �����۰���Ư��
EXT void EvObjDmyActMove(actWork*);							// ̨����OBJ��а�������
EXT void EvObjDmyActSiteChg(u8,u8);							// ��а���������ύX

#undef EXT
#endif // EVOBJMV_H
