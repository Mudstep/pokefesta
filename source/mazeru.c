//=========================================================================
//	
//
//		�C�x���g�f�[�^�����ʐM�V�X�e��
//
//
//		2001.10.05 GAME FREAK Inc.
//
//
//
//
//
//
//=========================================================================

#define SIO_INTR_ON										//���荞��ON��


#include "common.h"
#include "multisio.h"
#include "pokesio.h"
#include "cable.h"

#include "calctool.h"
#include "screen.h"
#include "print.h"
#include "palanm.h"
#include "actor.h"
#include "task.h"
#include "ev_comm.h"
#include "intr.h"
#include "menu.h"
#include "mus_tool.h"
#include "tv_mix.h"
#include "popword.h"
#include "saveload.h"
#include "sodateya.h"

#include "script.h"
#include "fld_main.h"
#include "syswork.h"

#include "svld_ini.h"

#include "b_tower.h"		//insert by soga 2002.07.10

#include "sysflag.h"
#include "pocketno.h"
#include "item.h"
#include "ex_load.h"

extern void kanshi_network(u8 taskno);

extern u8  SendPoolBuf[];
//=========================================================================
//	��`
//=========================================================================

//#define RECV_BUF_MAX	(SPLITDATA_POOL_SIZE - 32)
#define RECV_BUF_MAX	(200)

#define COMM_MIX_ENTRY_WAIT		30		/* �ʐM��t����(�b) */



SODATEYA_MAIL	SodateyaMail = {};


enum{
	COMM_NORMAL   = 0,
	COMM_SUCCESS ,
};




/* �o�b�t�@�A�h���X��` */
#define	MIX_RECV_BUFFER		( (u32)&(UserWork[0x8000]) )
#define	MIX_SEND_BUFFER		( (u32)&(UserWork[0x18000]) )


/*�@�f�[�^�̒�` */
static const u32 MIX_BASE_DATA_ADRS =	( (u32)&(Fld.SecretBase) );
#define	MIX_BASE_DATA_SIZE				( (u32)sizeof(Fld.SecretBase) )

static const u32 MIX_TV_DATA_ADRS =		( (u32)&(Fld.tv_event) );
#define MIX_TV_DATA_SIZE				( (u32)sizeof(Fld.tv_event) )

static const u32 MIX_CM_DATA_ADRS =		( (u32)&(Fld.tv_cm) );
#define MIX_CM_DATA_SIZE				( (u32)sizeof(Fld.tv_cm) )

static const u32 MIX_OYAJI_DATA_ADRS	= ((u32)&(Fld.oyaji_work));
#define MIX_OYAJI_DATA_SIZE				( 64)

static const u32  MIX_POPWORD_DATA_ADRS	= ((u32)&(Fld.PopWord));
#define MIX_POPWORD_DATA_SIZE			( (u32)sizeof(Fld.PopWord))

static const u32  MIX_SODATEYA_MAIL_ADRS = ((u32)&(SodateyaMail));
#define MIX_SODATEYA_MAIL_SIZE			( (u32)sizeof(SodateyaMail))

static const u32  MIX_BATTLETOWER_ADRS = ((u32)&(MyData.BattleTowerWork.SBTT_Mine));
#define MIX_BATTLETOWER_DATA_SIZE		( (u32)sizeof(StockBattleTowerTrainer))

#define MIX_SENDITEM_DATA_SIZE			( 4 )

#define MIX_MARGIN_DATA_SIZE				( 100)



#define MIX_BASE_SEND_ADRS			MIX_SEND_BUFFER
#define	MIX_TV_SEND_ADRS			( MIX_BASE_SEND_ADRS + MIX_BASE_DATA_SIZE )
#define MIX_CM_SEND_ADRS			( MIX_TV_SEND_ADRS + MIX_TV_DATA_SIZE )
#define MIX_OYAJI_SEND_ADRS			( MIX_CM_SEND_ADRS + MIX_CM_DATA_SIZE)
#define MIX_POPWORD_SEND_ADRS		( MIX_OYAJI_SEND_ADRS + MIX_OYAJI_DATA_SIZE)
#define MIX_SODATEYA_MAIL_SEND_ADRS	( MIX_POPWORD_SEND_ADRS + MIX_POPWORD_DATA_SIZE)
#define MIX_BATTLETOWER_SEND_ADRS	( MIX_SODATEYA_MAIL_SEND_ADRS + MIX_SODATEYA_MAIL_SIZE)
#define MIX_SENDITEM_SEND_ADRS		( MIX_BATTLETOWER_SEND_ADRS + MIX_BATTLETOWER_DATA_SIZE)

#define MIX_BASE_RECV_ADRS			MIX_RECV_BUFFER
#define	MIX_TV_RECV_ADRS			( MIX_BASE_RECV_ADRS + MIX_BASE_DATA_SIZE )
#define MIX_CM_RECV_ADRS			( MIX_TV_RECV_ADRS + MIX_TV_DATA_SIZE )
#define MIX_OYAJI_RECV_ADRS			( MIX_CM_RECV_ADRS + MIX_CM_DATA_SIZE) 
#define MIX_POPWORD_RECV_ADRS		( MIX_OYAJI_RECV_ADRS + MIX_OYAJI_DATA_SIZE)
#define MIX_SODATEYA_MAIL_RECV_ADRS	( MIX_POPWORD_RECV_ADRS + MIX_POPWORD_DATA_SIZE)
#define MIX_BATTLETOWER_RECV_ADRS	( MIX_SODATEYA_MAIL_RECV_ADRS + MIX_SODATEYA_MAIL_SIZE)
#define MIX_SENDITEM_RECV_ADRS		( MIX_BATTLETOWER_RECV_ADRS + MIX_BATTLETOWER_DATA_SIZE)

#define MIX_SEND_DATA_SIZE		(MIX_BASE_DATA_SIZE + MIX_TV_DATA_SIZE + MIX_CM_DATA_SIZE \
								+ MIX_OYAJI_DATA_SIZE + MIX_POPWORD_DATA_SIZE \
								+ MIX_SODATEYA_MAIL_SIZE + MIX_BATTLETOWER_DATA_SIZE \
								+ MIX_SENDITEM_DATA_SIZE + MIX_MARGIN_DATA_SIZE )

//=========================================================================
//	�ϐ�
//=========================================================================

static u8 CommMixResult;



/* ���b�Z�[�W�f�[�^ */

//������
static const u8 msg_record_exchange[] = {
	RE_,KO_,bou_,DO_,wo_,spc_,ma_,ze_,te_,i_,ma_,su_,EOM_
};

//�����I��
static const u8 msg_end_record_exchange[] = {
	si_,yyu_,u_,ri_,yyo_,u_,spc_,si_,ma_,si_,ta_,
	CR_,
	o_,tu_,ka_,re_,sa_,ma_,de_,si_,ta_,
	EOM_
};

//static const u8 time_over_msg[] = {
//	zi_,ka_,n_,na_,i_,ni_,
//	CR_,
//	se_,tu_,zo_,ku_,spc_,de_,ki_,ma_,se_,n_,de_,si_,ta_,EOM_
//};





//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

void CommMixMainTask( u8 no );
u16 get_recv_buf_size();


static void CommHelloTask( u8 no );

static void EndKeyWaitTask( u8 no );
static void ResultPutTask( u8 no );




static u32 get_u32_value( s16 * p);
static void set_u32_value( u32 val, s16 * p);
static u8 get_recv_id( void );
static u8 * get_recv_buf_adrs( u8 id );
static void SioSaveTask(u8 no);

static void CommDataSetTask(u8 no);
static void CommGetDataRecvTask(u8 no);


/* �閧��n�f�[�^�����������[�`�� */
extern void SioAllBaseSet( void* base , u32 size, u8 id);
extern void	BasePokeSet(void);			//�莝���|�P�������Z�b�g

// ���₶�f�[�^�������[�`��
static void OyajiMixer(void* adr, u32 size, u8 id);
static void BattleTowerMixer( void* adr, u32 size, u8 id);

static void SioMailMixer(void* adr, u32 size, u8 id, void* adr2);
static void DistributeItem(void* adr, u8 id);

extern void OyajiReset(void);



//	�f�[�^�������̃s�R�s�R�G�t�F�N�g
extern u8 StartRecordExchangeEffect( void );
extern void FinishRecordExchangeEffect( void );

//=========================================================================
//
//	�f�[�^�������[�h�ւ̑J��
//
//=========================================================================

//-------------------------------------------------------------------------
//	�X�N���v�g�����т���
//-------------------------------------------------------------------------
void CommMixStart( void )
{
	AddCommEventStartTask(CommMixMainTask);
}

//=========================================================================
//=========================================================================


//-------------------------------------------------------------------------
//	�����f�[�^�쐬���[�`��
//-------------------------------------------------------------------------
void CommMixSendDataMake( void )
{
	BasePokeSet();		//�莝���|�P�������Z�b�g
	TVDataHimselfLetterWatchFlagOff();	//�����s�u�f�[�^��������Ԃɂ���

	//�閧��n
	memcpy( (u8*)MIX_BASE_SEND_ADRS,    (u8 *)MIX_BASE_DATA_ADRS,    MIX_BASE_DATA_SIZE );
	//TV�ECM�ް�
	memcpy( (u8*)MIX_TV_SEND_ADRS,      (u8 *)MIX_TV_DATA_ADRS,      MIX_TV_DATA_SIZE );
	memcpy( (u8*)MIX_CM_SEND_ADRS,      (u8 *)MIX_CM_DATA_ADRS,      MIX_CM_DATA_SIZE );
	//���₶
	memcpy( (u8*)MIX_OYAJI_SEND_ADRS,   (u8 *)MIX_OYAJI_DATA_ADRS,   MIX_OYAJI_DATA_SIZE );
	//���s
	memcpy( (u8*)MIX_POPWORD_SEND_ADRS, (u8 *)MIX_POPWORD_DATA_ADRS, MIX_POPWORD_DATA_SIZE );

	//��ĉ����[��
	SodateyaMail.SioMail[0] = Fld.SodateyaWork.SioMail[0];
	SodateyaMail.SioMail[1] = Fld.SodateyaWork.SioMail[1];
	PokeAzukeInfoCheck(&Fld.SodateyaWork,&SodateyaMail);
	memcpy( (u8*)MIX_SODATEYA_MAIL_SEND_ADRS, (u8 *)MIX_SODATEYA_MAIL_ADRS, MIX_SODATEYA_MAIL_SIZE );

	//�o�g���^���[
	memcpy( (u8*)MIX_BATTLETOWER_SEND_ADRS, (u8 *)MIX_BATTLETOWER_ADRS, MIX_BATTLETOWER_DATA_SIZE );

	//�z�z�A�C�e��
	if(GetSioConnectId() == 0)		//�e�����z���
		*(u16*)MIX_SENDITEM_SEND_ADRS = GetExItem();
}

//-------------------------------------------------------------------------
//	����������т������[�`��
//-------------------------------------------------------------------------
void CommMixerCall( u32 id )
{

	SioAllBaseSet( (void*)MIX_BASE_RECV_ADRS,    MIX_SEND_DATA_SIZE, (u8)id);		//�閧��n
	TVDataMixer(   (void *)MIX_TV_RECV_ADRS,     MIX_SEND_DATA_SIZE, (u8)id);		//TV
	CMDataMixer(   (void *)MIX_CM_RECV_ADRS,     MIX_SEND_DATA_SIZE, (u8)id);		//CM
	OyajiMixer(    (void*)MIX_OYAJI_RECV_ADRS,   MIX_SEND_DATA_SIZE, (u8)id);		//���₶
	PopWordMixer(  (void*)MIX_POPWORD_RECV_ADRS, MIX_SEND_DATA_SIZE, (u8)id);		//���s
	SioMailMixer(  (void*)MIX_SODATEYA_MAIL_RECV_ADRS,
					MIX_SEND_DATA_SIZE,(u8)id, (void *)MIX_TV_RECV_ADRS);			//��ĉ��ɂ��郁�[��
	BattleTowerMixer( (void*)MIX_BATTLETOWER_RECV_ADRS,MIX_SEND_DATA_SIZE,(u8)id);	//�o�g���^���[
	DistributeItem((void*)MIX_SENDITEM_RECV_ADRS, (u8)id);							//�z�z�A�C�e��

}

//=========================================================================
//=========================================================================

enum{
	WK_STAT = 0,			//��ԑJ�ڕێ�	0
	WK_MAX,					//�]���ő��	1
	WK_ADR,					//�A�h���X�ێ�	2
	WK_ADR2,				//	����		3
	WK_CNT,					//�]����		4
	WK_ID,					//�}���`�v���C�]���p�h�c	5
	WK_MEMBER,				//���݉��l�Őڑ����Ă��邩	6
	WK_NOWMEMBER,			//���ݒN���]�����Ă��邩	7
	WK_WAIT,				//�E�F�C�g�p�J�E���^		8
	WK_SIZE,				//�]���f�[�^�T�C�Y�i�f�o�b�O�p�j9
	WK_SUBTASK,				//�T�u�^�X�NNo	10
	WK_PARENTTASK,			//�匳�̐e�^�X�N	11
	WK_FIRSTWAIT,			//�ŏ��̏����������񂩂�����	12
	WK_SIO_WAIT,			// 13
	WK_SUBTASK2,			//�T�u�^�X�N���̂Q 14
	WK_SE_TASK				//SE�炵�p
};



//=========================================================================
//
//	�f�[�^�����S�̐���^�X�N
//
//������̗��ꁄ
//	1)CommEventStartTask�őS�������������J�n�ɂȂ�̂�҂�
//	2)�S�������������A���̃^�X�N�ɐ��䂪�n�����
//	3)���̃^�X�N�Ō��������{�̂̏I����҂�
//	4)�I��������Z�[�u�������s��
//	5)�t�B�[���h������Ԃɕ��A�A�X�N���v�g�ɐ����n����DelTask����
//=========================================================================
enum{
	SEQ_START = 0,
	SEQ_WATCHDOG,
	SEQ_FADE_OUT,
	SEQ_SAVE,
	SEQ_WAIT,
	SEQ_END,
};

void RecordCornerSeTask( u8 no)
{
	TaskTable[no].work[0]++;
	if(TaskTable[no].work[0]==50){
		SePlay(SE_W213);
		TaskTable[no].work[0] = 0;
	}
}



void CommMixMainTask( u8 no )
{
	s16 * twk = TaskTable[no].work;


	switch ( twk[WK_STAT] ) {

	case SEQ_START:
		SetMyTrainerInfoStandPos(ScriptParameter1);			//�����ʒu����ݒ�
		SetEventWorkValue( LOCALWORK0, 1 );					//��x�����N�����Ȃ��悤�ɐ���
		CommMixResult = COMM_NORMAL;
		CommMixSendDataMake();								//�����f�[�^����
		StartRecordExchangeEffect();						//�G�t�F�N�g�J�n
		twk[WK_STAT] = SEQ_WATCHDOG;
		twk[WK_SUBTASK] = AddTask( CommHelloTask, 80 );		//�ʐM�^�X�N�ݒ�
		twk[WK_SE_TASK] = AddTask( RecordCornerSeTask,81);	//SE�炷
		break;

	case SEQ_WATCHDOG:													
		if( TaskTable[twk[WK_SUBTASK]].occ == 0 )			//���R�[�h�ʐM�I���҂�
		{
			twk[WK_STAT] = SEQ_FADE_OUT;					//BGM�t�F�[�h�A�E�g
//			BGM_FadeOutSet( 3);						
			EventFlagSet(SYS_MIX_RECORD);					//���R�[�h�R�[�i�[�ŒʐM�������Ƃ�
															//���邩�t���O�Z�b�g
			FinishRecordExchangeEffect();					//�G�t�F�N�g�I��
			DelTask(twk[WK_SE_TASK]);
		};
		break;

	case SEQ_FADE_OUT:												
//		if(BGM_FadeOutCheck()==TRUE){						//BGM�t�F�[�h�A�E�g�҂�
			twk[WK_SUBTASK] = AddTask(SioSaveTask,10);
			twk[WK_STAT] = SEQ_SAVE;
			SePlay(SE_W226);
//		}
		break;

	case SEQ_SAVE:											//�ʐM�X�g�b�v���Z�[�u����
		if(TaskTable[twk[WK_SUBTASK]].occ == 0)
		{
//			BGMFadeInSet( 3);
			twk[WK_STAT] = SEQ_WAIT;
			twk[WK_SUBTASK] = AddNinshouRoomTask();			//�������֖߂鏈��
			NMenuTalkMsgClr();
			NMenuMsgWrite( msg_end_record_exchange, 4, 15 );
			twk[WK_WAIT] = 0;
			
		}
		break;

	case SEQ_WAIT:
		if(++twk[WK_WAIT] > 60) twk[WK_STAT] = SEQ_END;
		break;

	case SEQ_END:
		if(TaskTable[twk[WK_SUBTASK]].occ == 0){
			CommEventWaitCancel();							//�t�B�[���h�ʐM�����V�[�P���X�؂�ւ�
			NMenuScreenClear();
			DelTask(no);
			ContinueFieldScript();							//�X�N���v�g�ɐ�����ڂ�
		}
		break;
	}
}









//=========================================================================
//
//=========================================================================

//-------------------------------------------------------------------------
//	�f�[�^��������J�n�^�X�N
//-------------------------------------------------------------------------





static void CommHelloTask( u8 no )
{
	u8 test_real_number;
	u8 taskno;

	TASK_TABLE * my = &TaskTable[no];

	switch(my->work[WK_STAT])
	{
	case 0:									//�ʐM������
		get_recv_buf_size();
		NMenuTalkWinWrite();
		NMenuMsgWrite( msg_record_exchange, 4, 15 );
		my->work[WK_WAIT] = 60 * COMM_MIX_ENTRY_WAIT;
		my->work[WK_STAT] = 400;
		MultiSioFunctionClear();

//		DebugNumPrintInit(0,1,29,0);
//		AddTask(kanshi_network,0);
//		PokemonMultiSioInit();
		break;
	case 100:
		my->work[WK_FIRSTWAIT]++;			//�ڑ��܂ų���
		if(my->work[WK_FIRSTWAIT]>20){
			my->work[WK_FIRSTWAIT]=0;
			my->work[WK_STAT]=101;
		}
		break;
	case 101:
		test_real_number = GetSioRealConnectNumber();
		if ( GetMyConnectIsParent() == 1 ) {
			if ( test_real_number == GetSioReliableNumber() ) {
				SePlay( SE_PIN );
				my->work[WK_STAT]=201;
				my->work[WK_FIRSTWAIT] = 0;
			}
		} else {
			SePlay( SE_BOO );
			my->work[WK_STAT]=301;
		}
		break;		
	case 201:									//�e����
		if ( GetSioReliableNumber() == GetSioRealConnectNumber() ) {
//			if(sys.Trg&A_BUTTON){
			if(++my->work[WK_FIRSTWAIT]>30*GetSioRealConnectNumber()){
				MultiSioSignalDecision();
				my->work[WK_STAT]=1;
			}
		}
		break;
	case 301:
		if ( GetSioReliableNumber() == GetSioRealConnectNumber() ) {
			my->work[WK_STAT]=1;
		}
		break;									//�q����

	case 400:
		my->work[WK_FIRSTWAIT]++;
		if(my->work[WK_FIRSTWAIT]>20){
			my->work[WK_STAT] = 1;
			my->work[WK_FIRSTWAIT] = 0;
		}
		break;

	case 1:									//�ڑ��m�F
		if( pokemon_sio_flag )
		{
			PM_NumMsgSet( StrTempBuffer0, get_recv_id(), NUM_MODE_ZERO, 2);
			my->work[WK_STAT]=5;
			break;
		};

		//�ʐM���m�����Ȃ��ꍇ
//		PM_NumMsgSet( StrTempBuffer0, my->work[WK_WAIT]/60, NUM_MODE_ZERO, 3);
//		NMenuMsgWrite( StrTempBuffer0, 3, 17 );
//		my->work[WK_WAIT]--;
//		if( my->work[WK_WAIT] == 0 )		//�^�C���I�[�o�[����
//		{
//			NMenuWinMsgWrite( time_over_msg );
//			my->TaskAdrs = EndKeyWaitTask;	//�^�X�N���I��������
//		};
		break;
	case 2:	
																//���M����M���U�蕪��
		my->work[WK_MEMBER] = GetSioRealConnectNumber();		//�l���m�F
		my->work[WK_STAT] = 0;
		my->work[WK_ID] = get_recv_id();						//������ID��ۑ�
		my->TaskAdrs = CommDataSetTask;

		set_u32_value( MIX_BASE_SEND_ADRS, &my->work[WK_ADR] );
		taskno = AddTask(CommGetDataRecvTask,80);
		my->work[WK_SUBTASK] = taskno;
		TaskTable[taskno].work[0] = no;
		set_u32_value( MIX_BASE_RECV_ADRS, &TaskTable[taskno].work[5] );
		break;
	case 5:														//�ڑ����m�F�ł��Ȃ��ꍇ��
		if(++my->work[WK_SUBTASK]>60){							//1�b�܂��čĊm�F
			my->work[WK_SUBTASK] = 0;
			my->work[WK_STAT] = 2;
		}
		break;
	};

}



// =================================================================
// CommParendTask2
// �T�v  : �f�[�^���Z�b�g���Â���
//		 : �e�̓f�[�^�����R�}���h�����s����
// ����  : no
// �߂�l: none
// =================================================================
static void CommDataSetTask(u8 no)
{
	TASK_TABLE *my;
	u8 * src;


	my = &TaskTable[no];
	switch(my->work[0])
	{
		case 0:										//�f�[�^���M�p�ɏ���
			src = (u8*)(get_u32_value(&my->work[WK_ADR]) + RECV_BUF_MAX * my->work[WK_CNT]);
			memcpy( SendPoolBuf, src, RECV_BUF_MAX );

			my->work[0]++;
			break;
		case 1:
			if(GetSioConnectId()==0){				//�e�Ȃ�f�[�^�����J�n��ʒm
				SplitDataExchangeStart(1);
			}
			my->work[0]++;
			break;
		case 2:
			break;
		case 3:										//�f�[�^���擾�I�������炱���ɂ���̂�
													//�J�E���g��i�߂Ď��̑��M������
			my->work[WK_CNT]++;						
			if(my->work[WK_CNT]==((MIX_SEND_DATA_SIZE/RECV_BUF_MAX)+1)){
				my->work[0]++;		
			}else{
				my->work[0]=0;
			}
			break;
		case 4:										//�����܂�
			if( TaskTable[my->work[WK_SUBTASK]].occ == 0 )
			{
				my->TaskAdrs=ResultPutTask;
			}
			break;
	
	}
}

// =================================================================
// CommGetDataRecvTask
// �T�v  : ��M�����f�[�^���i�[���Â���i�e�q�֌W���������j
// ����  : no
// work0 :CommDataSetTask�̃^�X�N�ԍ�
// work1 :�e�̎�M��
// work2 :�q�P
// work3 :�q�Q
// work4 :�q�R
// work5 :��M�ޯ̧�A�h���X
// �߂�l: none
// =================================================================
static void CommGetDataRecvTask(u8 no)
{
	u8 result,i,end;
	TASK_TABLE * my;
	u32 work;
	u8 * src;
	u8 * dst;

	my = &TaskTable[no];
	result = SplitDataResult();
	end = 0;

	if(result==GetConnectMemberBitData()){					//�S����M������
		for(i=0;i<GetConnectSioMenberNum();i++){			//��M�I�����`�F�b�N
			if((result>>i)&1){
				dst = (u8 *)( get_u32_value( &my->work[5] ) + RECV_BUF_MAX * my->work[i+1] + i * MIX_SEND_DATA_SIZE);
				src = (u8 *)&(get_recv_buf_adrs(i)[0]);
				if((RECV_BUF_MAX*(my->work[i+1]+1))>MIX_SEND_DATA_SIZE){
					work = MIX_SEND_DATA_SIZE - (RECV_BUF_MAX*(my->work[i+1]));
					memcpy( dst, src, work );
				}else{
					memcpy( dst, src, RECV_BUF_MAX );
				}
				SplitDataResultReset2(i);					//��M�t���O��ID�ʂɃ��Z�b�g
				my->work[i+1]++;							//��M�J�E���^���C���N�������g
				if(my->work[i+1]==((MIX_SEND_DATA_SIZE/RECV_BUF_MAX)+1)){
					end++;
				}
			}
		}
		TaskTable[my->work[0]].work[0]++;
	}
	if(end==GetConnectSioMenberNum()) DelTask(no);

}



//=========================================================================
//=========================================================================

static void end_watch_task( u8 no )
{
	TASK_TABLE * my = &TaskTable[no];

	if ( TaskTable[my->work[WK_SUBTASK]].occ == 0 ) DelTask( no );
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
static void EndKeyWaitTask( u8 no )
{
	TASK_TABLE * my = &TaskTable[no];
	my->TaskAdrs = end_watch_task;
	if( CommMixResult == COMM_SUCCESS ) CommMixerCall(TaskTable[no].work[WK_ID]);
	
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
static void ResultPutTask( u8 no )
{
	TaskTable[no].TaskAdrs = EndKeyWaitTask;

	CommMixResult = COMM_SUCCESS;
};



// =============================================
// get_u32_value
// �T�v  : �A�h���X��n�����ꏊ����32�ޯăf�[�^���擾
// ����  : p
// �߂�l: static u32
// =============================================
static u32 get_u32_value( s16 * p)
{
	return  (u16)*p | ( (u16)*(p+1) << 16 );
}

// =============================================
// set_u32_value
// �T�v  : �n�����ꏊ��32�ޯ��ް����
// ����  : val
//       : p
// �߂�l: static void
// =============================================
static void set_u32_value( u32 val, s16 * p)
{
	*p = (s16)(val & 0xffff);
	*(p+1) = (s16)( (val >> 16) & 0xffff );
}

// =============================================
// get_recv_id
// �T�v  : �ʐMID���擾
// ����  : none
// �߂�l: static u8
// =============================================
static u8 get_recv_id( void )
{
	return GetSioConnectId();
};

// =============================================
// get_recv_buf_adrs
// �T�v  : �������擾���ׂ��f�[�^�A�h���X���Z�o
// ����  : id
// �߂�l: static u8 *
// =============================================
static u8 * get_recv_buf_adrs( u8 id )
{
	return(u8 *)&(RecvPoolBuf[id][0]);
}


u32 mix_buf_size;

// =============================================
// get_recv_buf_size
// �T�v  : ���R�[�h�R�[�i�[�������f�[�^�̃o�C�g�����擾
// ����  : none
// �߂�l: u16
// =============================================
u16 get_recv_buf_size()
{
	mix_buf_size = MIX_SEND_DATA_SIZE;
	return MIX_SEND_DATA_SIZE;									//�f�[�^�S�̂̑傫��
//	return MIX_POPWORD_RECV_ADRS	- MIX_BASE_RECV_ADRS;		//���s�f�[�^�̃I�t�Z�b�g
	
}


static const u8 koukantbl2[2]={			//�����pð��فi���e�L�g�[�j
		1,0				
	};
static const u8 koukantbl3[2][3]={
		{1,2,0,},{2,0,1,},
	};
static const u8 koukantbl4[9][4]={
		{ 1,0,3,2,},{ 3,0,1,2,},{ 2,0,3,1,},
		{ 1,3,0,2,},{ 2,3,0,1,},{ 3,2,0,1,},
		{ 1,2,3,0,},{ 2,3,1,0,},{ 3,2,1,0,},
	};



// =================================================================
// MakeKoukanTable
// �T�v  : ��������ւ��p�̃e�[�u���쐬
// ����  : tbl
// �߂�l: none
// =================================================================
static void MakeKoukanTable(u8 *tbl)
{
	u32 key,num,i;
	num = GetConnectSioMenberNum();		//�l��
	
	switch(num){						//�������Ă�l���ɂ���č�������ւ���
		case 2:
			for(i=0;i<2;i++) 
				tbl[i] = koukantbl2[i];
			break;
		case 3:
//			w = pp_rand()%2;
			key = GetSioTrainerId(0)%2;			//�e��ID���擾���č������킹�̎���擾
			for(i=0;i<3;i++) 			//�R�l�̎��͍ő�Q�p�^�[��
				tbl[i] = koukantbl3[key][i];
			break;
		case 4:
//			w = pp_rand()%9;
			key = GetSioTrainerId(0)%9;			//�e��ID���擾���č������킹�̎���擾
			for(i=0;i<4;i++) 			//�S�l�̎��͍ő�X�p�^�[��
				tbl[i] = koukantbl4[key][i];
			break;
	}
	
}
// =================================================================
// OyajiMixer
// �T�v  : ���₶�f�[�^��������
// ����  : adr		�ް����ڽ�̐擪
//       : size		���o�C�g���ɔz�u����Ă��邩�̃T�C�Y
//       : id		������ID
// �߂�l: none
// =================================================================
static void OyajiMixer(void* adr, u32 size,u8 id)
{
	u8  koukantbl[4];
	
	MakeKoukanTable(koukantbl);

	memcpy((u8*)adr+size*id,(u8*)MIX_OYAJI_DATA_ADRS,64);	//�����̃f�[�^���R�s�[
	
	memcpy((u8*)MIX_OYAJI_DATA_ADRS, 
		(u8*)(adr+size*koukantbl[id]),MIX_OYAJI_DATA_SIZE);		//���������f�[�^���󂯎��
	OyajiReset();											//���₶������

}


static u8  testkoukantbl[4];

// =================================================================
// BattleTowerMixer
// �T�v  : �o�g���^���[�f�[�^��������
// ����  : adr		�o�g���^���[�f�[�^ID0�̃A�h���X
//       : size		���R�[�h�R�[�i�[�f�[�^�̑傫��
//       : id		������ID
// �߂�l: none
// =================================================================
static void BattleTowerMixer( void* adr, u32 size, u8 id)
{
	
	MakeKoukanTable(testkoukantbl);
	

	//������ID�̃o�g���^���[�f�[�^�𑼐l��ID�̃o�g���^���[�f�[�^�Œׂ�
	memcpy((u8*)adr+size*id, 
		(u8*)(adr+size*testkoukantbl[id]),MIX_BATTLETOWER_DATA_SIZE);

	//�]�䕔����Ɏ�����ID�̃A�h���X�ɂ���A���l���������o�g���^���[�f�[�^��n��
	BattleTowerStockData(adr+size*id);
}


// =================================================================
// SioMailCheck
// �T�v  : ��ĉ����[�����[�N�Ƀ��[�������邩�Ȃ���
// ����  : smd
// �߂�l: u8
// =================================================================
u8 SioMailCheck(SIO_MAIL_DATA *smd)
{
	return smd->Mail.design_no;
}

// =================================================================
// SodateyaMailExchange
// �T�v  : ��ĉ����[������ꂩ����
// ����  : adr
//       : size
//       : a
//       : b
// �߂�l: none
// =================================================================
static void SodateyaMailExchange(void* adr, u32 size, u8 tbl[4][2], u8 a, u8 b)
{
	SODATEYA_MAIL	*smd1,*smd2;
	SIO_MAIL_DATA	work;

	smd1 = (SODATEYA_MAIL*)(adr+size*tbl[a][0]);			//��U�ۑ����邽�߂�a�̃|�C���^�[�Ă�
	work = smd1->SioMail[tbl[a][1]];							//�w��̃��[����ۑ�

	smd2 = (SODATEYA_MAIL*)(adr+size*tbl[b][0]);			//b�̃|�C���^���[�Ă�

	smd1->SioMail[tbl[a][1]] = smd2->SioMail[tbl[b][1]];		//a�̏ꏊ��b�̃��[�����R�s�[
	smd2->SioMail[tbl[b][1]] = work;
}



static const u8 koukan3tbl2[3][2]={
	{0,1},{1,2},{2,0},	//�S�l�Ń��R�[�h�ɍs���Ď��ۂɂR�l���������ł��Ȃ����Ƃ����肻���Ȃ̂�
						//{0,1}�̌������c��
};
static const u8 koukan4tbl2[3][2][2] = {
	{
		{ 0, 1,},		//���ꂾ�ƂQ�l�Ō�������̂Ɠ����Ȃ̂ł��̎�荇�킹�͂�߂�
		{ 2, 3,},
	},

	{
		{ 0, 2},
		{ 1, 3},
	},
	{
		{ 0, 3},
		{ 2, 1},
	},
};


// =================================================================
// GetKeyNumberSodateyaMail
// �T�v  : ��ĉ����[���̗����p�ɂs�u�f�[�^��u8��256�o�C�g��������������Ԃ�
// ����  : adrs
// �߂�l: static u8
// =================================================================
static u8 GetKeyNumberSodateyaMail(void *adrs)
{
	u8 *p, ret;
	int i;

	p = (u8*)adrs;
	
	ret = 0;
	for(i=0;i<256;i++) ret += p[i];
	
	return ret;
}
// =================================================================
// SioMailMixer
// �T�v  : ��ĉ��ɗa�����Ă��郁�[��������Ɍ�������
// ����  : adr	��ĉ����[���f�[�^
//       : size ���̐l�̃f�[�^�ւ̃o�C�g��
//       : id	�����̂h�c
//		 : adr2 �e�̂s�u�f�[�^�̃A�h���X
// �߂�l: none
// =================================================================
static void SioMailMixer(void* adr, u32 size, u8 id, void* adr2)
{
	SODATEYA_MAIL	*smd;
	u8 check[4];		//���[���������Ă邩�t���O�i0xff=����,0��1=�������邽�߂Ɏw�肳�ꂽ���[���j
	u8 member;			//�ʐM�Őڑ����Ă���l��
	u8 mailnum;			//���[���������Ă���l��
	u8 pokenum[4];		//�|�P������a���Ă���t���O(0=���Ȃ�	1,2=�|�P�����̐�)
	u8 takenum;			//�|�P������a���Ă���l�̐�
	u8 fig;
	u16 randbak,i,w;
	u8 koukan_a,koukan_b;
	u8 *siomailadr[4];
	u8 now_my_id;

	u8 permit[4][2];		//���[�������ւ��Ă������ꏊ�������t���O
	u8 exchangable;
    u8 ablemail[4][2];
	u8 mail1,mail2;

	randbak = pp_rand();					//�����_����ۑ�

	pp_srand(SioTrainerInfo[0].Id);			//�e�̂h�c�Ń����_����������
	member = GetConnectSioMenberNum();		//�l��

	for(i=0;i<4;i++){						//���[�N������
		check[i] = 0xff;
		pokenum[i] = 0;
		permit[i][0] = 0;
		permit[i][1] = 0;
	}

	mailnum = 0;
	takenum = 0;
	exchangable = 0;

	for(i=0;i<member;i++){
		smd = (SODATEYA_MAIL*)(adr+size*i);			
		if(smd->PokeNum==0) continue;
		for(w=0;w<smd->PokeNum;w++){
			if(smd->PokeFlag[w]==0){		//�A�C�e���������Ă��Ȃ������[��
				permit[i][w] = 1;			//�������Ă�����
				exchangable++;
			}
		}
	}
	w = 0;
	for(i=0;i<member;i++){					//���[���������ł���ID�̐l��T��
		smd = (SODATEYA_MAIL*)(adr+size*i);			
		if(permit[i][0]==1 || permit[i][1]==1){
			takenum++;
		}
		if(permit[i][0]==1 && permit[i][1]==0){	//[0]=ID�@�� [1]=�Ԗڂ̃��[�����������邩
			ablemail[w][0] = i;                         //��l�߂̃��X�g�ɂ���
			ablemail[w][1] = 0;
			w++;
		}else if(permit[i][0]==0 && permit[i][1]==1){
			ablemail[w][0] = i;
			ablemail[w][1] = 1;
			w++;
		}else if(permit[i][0]==1 && permit[i][1]==1){	//����OK�̎��̓��[���������Ă�l��D��
			ablemail[w][0] = i;							//�����Ƃ������Ă��邩�����Ƃ������ĂȂ��Ȃ烉���_��
			mail1 = SioMailCheck(&smd->SioMail[0]);
			mail2 = SioMailCheck(&smd->SioMail[1]);
			if((mail1==NON_ITEM && mail2==NON_ITEM) || (mail1!=NON_ITEM && mail2!=NON_ITEM)){
				ablemail[w][1] = pp_rand()%2;
			}else if(mail1!=NON_ITEM && mail2==NON_ITEM){
				ablemail[w][1] = 0;
			}else if(mail1==NON_ITEM && mail2!=NON_ITEM){
				ablemail[w][1] = 1;
			}
			w++;
		}
	}
	


	for(i=0;i<4;i++){
		smd = (SODATEYA_MAIL*)adr+size*id;
		siomailadr[i] = (u8*)smd;
	}
	now_my_id = id;

//	fig = SioTrainerInfo[0].Id%3;			
	fig = GetKeyNumberSodateyaMail(adr2)%3;	//���ꂩ���邽�߂̗��������
	switch(takenum){						//�|�P������a���Ă���g���[�i�[�̐��ɂ���č�������ւ���
		case 2:
			SodateyaMailExchange(adr, size, ablemail, 0,1);			//2�l�a���Ă����
			break;
		case 3:														//3�l�a���Ă����
//			SodateyaMailExchange(adr,size,change,koukan3tbl2[fig][0],koukan3tbl2[fig][1]);
			koukan_a = koukan3tbl2[fig][0];
			koukan_b = koukan3tbl2[fig][1];
			SodateyaMailExchange(adr,size,ablemail,koukan_a,koukan_b);
			break;
		case 4:														//4�l�a���Ă����
			SodateyaMailExchange(adr, size,ablemail,koukan4tbl2[fig][0][0],koukan4tbl2[fig][0][1]);
			SodateyaMailExchange(adr, size,ablemail,koukan4tbl2[fig][1][0],koukan4tbl2[fig][1][1]);
			break;
	}

	//�����������ʂ������̈�ĉ����[���ɖ߂��i����ւ���ĂȂ����Ƃ�����j
	smd = (SODATEYA_MAIL*)(adr+size*id);
	Fld.SodateyaWork.SioMail[0] = smd->SioMail[0];
	Fld.SodateyaWork.SioMail[1] = smd->SioMail[1];
	

	
	pp_srand(randbak);
}

// =================================================================
// DistributeItem
// �T�v  : �A�C�e����z��
// ����  : adr
//       : id
// �߂�l: none
// =================================================================
static void DistributeItem(void* adr, u8 id)
{
	u16 *item;
	if(id==0) return;		//�e���z��̂Őe�͂����Ȃ�

	item = (u16*)adr;
	if(*item!=0){
		if(GetPocketNo(*item)==IMPORTANT_POCKET){
			//�厖�ȓ���̏ꍇ
			if(ItemCheck( *item, 1 )==FALSE && PCBoxItemCheck(*item,1)==FALSE && AddItem(*item,1)){
				SetEventWorkValue( LOCALWORK1, *item );		//�X�N���v�g�ɂ�������A�C�e����ʒm����
				PM_strcpy(StrTempBuffer0,SioTrainerInfo[0].name);
				if(*item == ITEM_MUGENNOTIKETTO) EventFlagSet(SYS_KOKORO_EVENT);	//�ނ���̃`�P�b�g��p����
			}else{
				SetEventWorkValue(LOCALWORK1,0);
			}
		}else{
			//���ʂ̓���̏ꍇ�i�z�邩������Ȃ����ǁj
			if(AddItem(*item,1) == TRUE){
				SetEventWorkValue( LOCALWORK1, *item );		//�X�N���v�g�ɂ�������A�C�e����ʒm����
				PM_strcpy(StrTempBuffer0,SioTrainerInfo[0].name);
			}else{
				SetEventWorkValue(LOCALWORK1,0);
			}
		}
	}
}

// =================================================================
// SioSaveTask
// �T�v  : �ʐM�X�g�b�v��Z�[�u���s���^�X�N
// ����  : none
// �߂�l: none
// =================================================================
static void SioSaveTask(u8 no)
{
	TASK_TABLE * my = &TaskTable[no];
	
	switch(my->work[0]){
			case 0:										//
				my->work[0]++;
				break;
			case 1:										//
					my->work[0]++;
				break;
			case 2:

				SetCommSaveFlag();
//				PokeAGBSave( POKEAGB_SVFLD );			
				PokeAGBSaveFieldDivInit();				//�Z�[�u�J�n
				my->work[0]++;
				break;
				
			case 3:
				if(PokeAGBSaveFieldDiv()){				//�Z�[�u�I���܂ŌĂ�
					ClearCommSaveFlag();
					my->work[0]++;
					my->work[1] = 0;
				}
				break;
			case 4:										//�҂�
				if(++my->work[1]>10){
					SetSioEndDataStart();
					my->work[0]++;
				}
				break;
			case 5:
				if(pokemon_sio_flag==0){				//�ʐM�I���ҋ@
					DelTask(no);				
				}
				break;
	}
}
