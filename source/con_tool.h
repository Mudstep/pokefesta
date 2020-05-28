#ifndef __CON_TOOL_H__
#define __CON_TOOL_H__


//----------------------------------------------------------
//	�萔
//----------------------------------------------------------
enum{			//GetBP�֐��Ŏg�p
	STYLE_MAIN,			//�i�D�悳
	BEAUTIFUL_MAIN,		//������
	CUTE_MAIN,			//���킢��
	CLEVER_MAIN,		//����
	STRONG_MAIN,		//�����܂���
};
#define HEART_ICON_MAX		8		//�Z�̐����\���ɏo��ʰĂ̍ő吔
#define METER_ONE_AP		10		//Ұ��1�łǂꂾ����AP�l��\����

//-- ��ذ��ް��萔 --//
#define MENUWIN_CLEAR_SCRN	0x000c	//�ʏ��ƭ����\������Ă��镔���̸ر��p��ذ��ް�
#define HEART_REDSPACE_SCRN	0x5035	//�Ԃ̋󔒂�ʰķ��
#define HEART_BLACKSPACE_SCRN	0x5036	//���̋󔒂�ʰķ��
#define HEART_RED_SCRN0		0x5012	//�Ԃ�ʰĂ̽�ذ��ް�
#define HEART_RED_SCRN1		0x5011	//�����ԐF��ʰĂ̽�ذ��ް�
#define HEART_BLACK_SCRN0	0x5014	//����ʰĂ̽�ذ��ް�
#define HEART_BLACK_SCRN1	0x5013	//�D�F��ʰĂ̽�ذ��ް�
#define HEART_CLEAR_SCRN	MSG_CLEAR_NO	//�N���A�p��ʰĂ̽�ذ��ް�

#define STYLE_MSG_SCRN		0x4040	//�Z������ʂ̋Z����(������������)�̽�ذ�
#define BEAUTIFUL_MSG_SCRN	0x4045
#define CUTE_MSG_SCRN		0x404a
#define CLEVER_MSG_SCRN		0x406a
#define STRONG_MSG_SCRN		0x408a

#define BASE_HEART_SCRN0	0x5011	//APҰ�����ް��ƂȂ齸ذ��ް�
#define BASE_HEART_SCRN1	0x6011	//APҰ�����ް��ƂȂ齸ذ��ް�
#define BASE_HEART_SCRN2	0x7011	//APҰ�����ް��ƂȂ齸ذ��ް�
#define BASE_HEART_SCRN3	0x8011	//APҰ�����ް��ƂȂ齸ذ��ް�

#define BASE_METER_SCRN0	0x50a2	//APҰ�����ް��ƂȂ齸ذ��ް�
#define BASE_METER_SCRN1	0x60a2	//APҰ�����ް��ƂȂ齸ذ��ް�
#define BASE_METER_SCRN2	0x70a2	//APҰ�����ް��ƂȂ齸ذ��ް�
#define BASE_METER_SCRN3	0x80a2	//APҰ�����ް��ƂȂ齸ذ��ް�
enum{
	METER_WHITE,		//���F���[�^�[
	METER_GRAY,
	METER_GREEN,
	METER_ORANJI,
	METER_BLUE,
	METER_PINK,
};

enum{
	SHIELD_TYPE,
	KINTYOU_TYPE,
	YASUMI_TYPE,
	BOUGAI_TARGET_TYPE,
	BOUGAI2BAI_TYPE,
};


//----------------------------------------------------------
//	�Z�����p���b�gID
//----------------------------------------------------------
enum{
	CELLID_JYUNNIICON0 = 20000,
	CELLID_JYUNNIICON1,
	CELLID_JYUNNIICON2,
	CELLID_JYUNNIICON3,
	CELLID_HEARTICON,
	CELLID_SINSAIN,
	CELLID_SORTICON0,
	CELLID_SORTICON1,
	CELLID_SORTICON2,
	CELLID_SORTICON3,
};

//----------------------------------------------------------
//	�f�[�^
//----------------------------------------------------------
extern const actHeader JyunniIconHeader[];
extern const CellData JyunniIconCell;
extern const PalData JyunniIconPal[];


//----------------------------------------------------------
//	UserWork�ɔ��p���ް����`�����A�������邽�߂̃}�N��
//----------------------------------------------------------
//��PORTRAIT_DATA�͌�������30byte�����A���ׂ߲̊֌W����ۂɂ�32byte�Ƃ��Ď擾�����
#define TEMP_PORTRAIT_ADRS	(0x15e00-(sizeof(PORTRAIT_DATA)))		//���p���ް����ꎞ�I��UserWork�Ɍ`��
#define PORTRAIT_TEMP	((PORTRAIT_DATA*)&UserWork[TEMP_PORTRAIT_ADRS])

//-- ����G������֐����ĂԎ��Ɉ����Ƃ��Ďg�p --//
#define TEMP_PORTRAIT_MSGFLG	(TEMP_PORTRAIT_ADRS - 1)	//0:�ʏ�\��,1:�����Ȃ��\��
#define TEMP_PORTRAIT_PLACE		(TEMP_PORTRAIT_ADRS - 2)	//0:�n�W�c�Q......12:���p�قT(�d�l�Q��)
	
//----------------------------------------------------------
//	�O���֐��錾
//----------------------------------------------------------
extern void SetBreederData(u8,u8);
extern void PutPlayerInfo(void);
extern void GetBreederNo(void);
extern u8 ContestPokeActorSet(void);
extern void PlayerInfoCopy(void);
extern void PutWazaComment(u16 wazano);
extern void WazaCommentClear(void);
extern void ContestTalkMsgClear(void);
extern void UseWorkGetAll(void);
extern void UsedWorkNamePutAll(u8);
extern void TotalAPCalc(void);
extern void PenaltyCalc(void);
extern u8 ConWazaMsgSet(u8 brd_no, u8 msg_no);
//extern void SetJudgeParam(void);
extern void GetEvaluationValueAll(void);
extern void GetEvaluationValue(u8 brd);
extern void GetBPAll(u8 MainContest);
extern void ConWazaFlagCheckClear(void);
extern void SetMyBreederData(u8 temotino);
extern u16 GetBP(u8 brd, u8 MainContest);
extern void TuusintyuuMsgPut(void);
extern u16 UseWorkGet(u8 pos);
extern u8 ContestPokeActorSet2(u16 monsno, u32,u32);
extern u8 WazaCalcFlagCheck(u8 brd);
extern u8 AppealMeterTaskSet(s16 now_ap, s16 add_ap, u8 col, u8 brd_pos);
extern void AppealMeterClear(void);
extern void WazaKubunIconPut(u16 wazano, u8 brd_pos);
extern void WazaKubunIconPutAll(void);
extern void JyunniIconPut(void);
extern u8 JyunniIconEffectEndCheck(void);
extern void JyunniIconAllDataSet(void);
extern void EyeColorTaskSet(void);
extern void EyeColorFadeStop(u8 brd_pos);
extern void EyeColorFadeStart(u8 brd_pos);
extern void MeterColorTaskSet(void);
extern void MeterColorFlagClear(u8 brd_pos);
extern void MeterColorClearAll(void);
extern void AppealPutMsgSet(u8);
extern u8 YasumiTurnCheck(u8 brd);
extern void EyeColorStatusCheckSet(u8 brd);
extern u8 AppealKoukaColorTaskSet(u8 *color_flg);
extern u8 BreederWindowObjSet(u8 brd);
extern void BrdWinAlphaRegSet(void);
extern void BrdWinWazaPutEffSet(u8 actno, u8);
extern void BrdWinObjDel(u8 actno);
extern void WazaPutBrdWinLight(u8 brd, u8 houkou);
extern void AppealMeterWhiteSet(s16 ap, u8 brd);
extern void UsedWorkNameClear(u8 brd);
extern void UsedWorkNameClearAll(void);
extern void UsedWorkNamePut(u8 brd, u8 deep);
extern void DebugAPPut(void);
extern void DebugAPUpdatingCheckPut(void);
extern u8 AppealMeterTaskSet2(s16 now_ap, s16 add_ap, u8 brd);
extern u8 PortraitDataSet(u8 tiiki_no);
extern u8 PortraitDataSpaceSearch(u8 tiiki_no, u8 slide_flg);
extern void InitPortraitData(void);
extern u8 ContestJudgeActorSet(void);
extern u8 AppealHeartTaskSet(s16 now_ap, s16 add_ap, u8 brd_pos);
extern void SetHeartIconMove(u8 brd);
extern void HeartIconAllSet(void);
extern void SetHeartIconMoveAll(void);
extern u8 CheckHeartIconMoveAll(void);
extern void BreederDataSort(u8 sort_type);
extern void ReWritePlayerInfo(void);
extern void HeartIconYposSet(void);
extern void HearIconEva(u8 evac);
extern void ContestWazaCalc(u8 nowbrd);
extern void BougaiMsgSet(u8, u8 damage);
extern void AppealKoukaMsgSet(u8, u8 msgid);
extern void AppealMsgSet(u8 brd, u8);
extern void ConWazaBrdSort(void);
extern u8 BonusIconPut(u8 brd_pos, u8);
extern void BonusIconResetAll(void);
extern u8 JudgeFukidasiActorSet(void);
extern void JudgeFukidasiSet(u8 fukidasi_type);
extern void TensyonNumPut(void);
extern s8 TensyonCheck(u16 wazano);
extern void TensyonMsgPut(void);
extern void NextSortPrint(u8);
extern u8 WazaKoukaIconPut(u8 brd);
extern void WazaKoukaIconPutAll(void);
extern void KintyouFlgSet(u8 brd);
extern void AppealKoukaMsgSet2nd(u8 brdno, u8 msgid);
extern u8 ContestParticipationCheck(PokemonParam *pp);
extern void BougaiReceiveIconPut(void);
extern u8 Combo1TermCheck(u8 brd);
extern u8 Combo1HasseiCheck(u8 brd);
extern void VoltageIconActorSet(void);
extern void VoltageIconAnmSet(s8 tensyon);
extern u8 TensyonMaxPalAnmSet(void);
extern void VoltageGaugeSlidPut(s8 flg);
extern void KankyakuAnmSet(void);
extern void VoltageGaugeSlidOut(void);
extern void VoltageGaugeSlidIn(void);
extern void KankyakuColorFadeSet(s8 flg, s8 muki);
extern void VoltageGaugePut(void);
extern void VoltageGaugeBanish(void);
extern void InfoMaskEffectDataSet(void);
extern void InfoMaskEffectDataClear(void);
extern s16 GetNijisinsaPoint(u8 brd);
extern void SortIconAllSet(void);
extern void HeartIconAffEffSet(u8 aff_type);
extern void ContestWazaEffectWorkClear(u8 brd);
extern u8 ContestSioOyaCheck(void);
extern void GetTurnKekkaMsgID(void);
extern void SetRangeClient(u16 wazano);
extern void ConWazaEffParaSet(u8 brd);
extern void SetEnemyBanishActor(void);
extern void EnemyBanishActorParamInit(void);
extern u16 GetContWazaNo(u16 wazano);
extern u16 GetContMonsNo(u16 monsno);
extern void ContestWazaEffectWorkSet(u8 brd);
extern u8 ContestPokeFlipCheck(u16 monsno);

//-- �e�X�g�֐� --//
extern void DownMoveEffect(u8 id);
#if 0
extern void RightMoveEffect(u8 id);
extern void BldMoveEffect(u8 id);
extern void LasterMoveEffect(u8 id);
#endif


#endif	//__CON_TOOL_H__

