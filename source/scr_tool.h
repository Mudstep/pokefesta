#ifndef __SCR_TOOL_H__
#define __SCR_TOOL_H__

//=========================================================================
//	
//=========================================================================

#undef GLOBAL
#ifdef __SCR_TOOL_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

#define	MSG_1F	0			//�P�K
#define	MSG_2F	1
#define	MSG_3F	2
#define	MSG_4F	3
#define	MSG_5F	4
#define	MSG_6F	5
#define	MSG_7F	6
#define	MSG_8F	7
#define	MSG_9F	8
#define	MSG_10F	9
#define	MSG_11F	10
#define	MSG_B1F	11			//�n���P�K
#define	MSG_B2F	12
#define	MSG_B3F	13
#define	MSG_B4F	14
#define	MSG_TOP	15			//����
#define	MSG_DMY	16			//�_�~�[


#define BICYCLE_COLLISION_MAX	100


//�����D���̃V�[����`�iWK_SCENE_SP_SHIP01�ɑ�������j
#define	SHIPSCENE_TAKEOFF_KAINA		1	//�J�C�i����A�N�A����D����
#define	SHIPSCENE_DEMO_RIGHT		2	//�f�����i�J�C�i���L�i�M�j
#define	SHIPSCENE_FREE_KAINA		3	//�f���I����A�t���[���
#define	SHIPSCENE_ARRIVE_MINAMO		4	//�~�i��������

#define	SHIPSCENE_TAKEOFF_MINAMO	5	//�~�i������A�N�A����D����
#define	SHIPSCENE_FREE_MINAMO		6	//�t���[���
#define	SHIPSCENE_DEMO_LEFT			7	//�f�����i�L�i�M���J�C�i�j
#define	SHIPSCENE_ARRIVE_KAINA		8	//�J�C�i������

#define	SHIPSCENE_END_DEMO_RIGHT	9	//�f���I���i�J�C�i���L�i�M�j
#define	SHIPSCENE_END_DEMO_LEFT		10	//�f���I���i�L�i�M���J�C�i�j

//�D�̈ړ��J�E���g
#define	SHIPMOVE_R134	60
#define	SHIPMOVE_R133	80
#define	SHIPMOVE_R132	65
#define	SHIPMOVE_MAX	(SHIPMOVE_R132+SHIPMOVE_R133+SHIPMOVE_R134)

//�����D���ł̃��[���h�}�b�v��̈ʒu�i�V�[������ϊ��j
#define	SHIPPOS_DEMO			0
#define	SHIPPOS_KAINA_STOP		1
#define	SHIPPOS_MINAMO_STOP		2
#define	SHIPPOS_R124			3
#define	SHIPPOS_R131			4

//�p�\�R���̃^�C�v�̒�`
#define PASO_POKEMON_CENTER		0		//�|�P�Z��
#define PASO_MY_HOME_BOY		1		//��l��(�j)
#define PASO_MY_HOME_GIRL		2		//��l��(��)

#define	DMY_OBJ_ID		0x7f			//�_�~�[�X�N���[��OBJID

//���ʉ���`
#define SE_GYOE_MARK	SE_PIN			//�u�I�v�̌��ʉ�
#define MUS_ITEM_RETURN	MUS_ME_WAZA		//�A�C�e����n�����̌��ʉ�

#ifndef	ASM_CPP		/* ����ň͂�ł�����̓A�Z���u���ł͉��߂���Ȃ� */


//=========================================================================
//	�ϐ�
//=========================================================================

//�T�C�N�����O���[�h�̃C�x���g�֘A
//
extern u8 bicycle_collision_flag;
extern u8 bicycle_collision_count;
extern u32 bicycle_start_time;

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);
GLOBAL u8 SetBlendCommName();
GLOBAL void SetBlendCommOBJ();

//--------------------------------------------------------------------
//�}�{���V�W�}�o������֘A
//--------------------------------------------------------------------
GLOBAL void MaboroshiCycle_Init( void );
GLOBAL void MaboroshiCycle_Update( u16 diff );
GLOBAL u8 Maboroshi_Check( void );

//--------------------------------------------------------------------
//�}�ӕ]���V�X�e���֘A
//--------------------------------------------------------------------
GLOBAL void ZukanHyouka_Init( void );
GLOBAL void ZukanHyouka_Update( u16 diff );

//--------------------------------------------------------------------
//�T�C�N�����O���[�h�̃C�x���g�֘A
//--------------------------------------------------------------------
GLOBAL void CyclingRoadEventInit( void );
GLOBAL void CyclingRoadEventStart( void );

//--------------------------------------------------------------------
//�A�N�A���̃f���֘A
//--------------------------------------------------------------------
GLOBAL void ShipStepCountStart( void );
GLOBAL void ShipStepCountEnd( void );
GLOBAL int ShipStepCountCheck( u16 value );

//--------------------------------------------------------------------
//	�����D�̒n�}��̈ʒu�Z�o
//	<����>	�f�����̈ʒu��Ԃ����߂̃��[�N
//			�i�߂�l��SHIPPOS_DEMO�ȊO�̎��ɂ͒l�͓���Ȃ��j
//			div_id,map_id		�}�b�v�w��ID
//			xpos,ypos			���W
//	<�߂�l>
//			�w�b�_��MAPPOS_�`���Q��
//--------------------------------------------------------------------
GLOBAL u8 GetShipWorldPos(u8 * div_id, u8 * map_id, s16 * xpos, s16 * ypos );

//--------------------------------------------------------------------
//�p�\�R���A�j��
//--------------------------------------------------------------------
GLOBAL void PasoAnimeTaskAdd();

//--------------------------------------------------------------------
//�}�ӕ]�����b�Z�[�W
//--------------------------------------------------------------------
GLOBAL const u8 * const GetZukanHyoukaMsg(u16);	//�擾
GLOBAL void SetZukanHyoukaMsg();				//�\��

//--------------------------------------------------------------------
//�J���N�����~�̓���B���A�C�e������
//--------------------------------------------------------------------
GLOBAL void KarakuriHideItemOn(void);
GLOBAL void KarakuriHideItemOff(void);

//--------------------------------------------------------------------
//�a�����蕜�A����
//--------------------------------------------------------------------
GLOBAL void ScriptDendouRecover();

//--------------------------------------------------------------------
//�󐣂̓��A���ɂ�����
//--------------------------------------------------------------------
GLOBAL void Asase_Update(u16);

//--------------------------------------------------------------------
//�莝���̐擪����^�}�S�ł͂Ȃ��ꏊ��Ԃ�
//--------------------------------------------------------------------
extern u8 GetTemotiBtlPokeNum();

//--------------------------------------------------------------------
//�X���b�g�̓������擾
//--------------------------------------------------------------------
extern u16 ScrSlotAtariGet();

#endif

#undef GLOBAL
#endif	/*__SCR_TOOL_H__*/

