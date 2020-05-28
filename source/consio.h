#ifndef __CONSIO_H__
#define __CONSIO_H__


//----------------------------------------------------------
//	�ް��v������
//----------------------------------------------------------
enum{	//50�ɓ��ɈӖ��͂Ȃ��@0��1�����v�����ނƂ��ĉ��ƂȂ��K���Ă邩�Ȃ���
	BD_REQ = 50,	//��ذ�ް�ް��v������
	CBT_REQ,		//ChangeBackTask���sظ��ĺ���
	STANDBY_REQ,	//��ýĊJ�n���̓�������邽�߂̽���޲����
	WAZANO_REQ,		//�Z���ް�𑗐M����
	EVALUATION_REQ,	//�]���l�𑗐M����
	CT_REQ,			//��ý����߂𑗐M����
	BD_GET,			//����ް�ް��擾
	JUDGE_GET,		//�R���ް��擾
	TANE_GET,		//������Ȏ擾
	CONKIND_GET,	//conkind�擾
	CONTYPE_GET,	//��ý����ߎ擾
	WAZANO_GET,		//�Z���ް�擾
	EVALUATION_GET,	//�]���l�擾
};


extern void ContestSioInit(u8 id);
extern void BreederTransData(u8 id);
extern void RandTaneTransData(u8 id);
extern void ContestTypeTransData(u8 id);
extern void ContestStandbyCheck(u8 id);
extern void ContWazaNoSioGet(u8 id);
extern void EvaluationSioGet(u8 id);
extern void ContestTypeGetAllData(u8 id);
extern void ContWazaCalcDataSioGet(u8 id);
extern void ContestVersionGetAllData(u8 id);
extern void ContBPAllTransData(u8 id);
extern void BrdSortInitAllTransData(u8 id);


#endif

