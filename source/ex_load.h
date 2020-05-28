
#ifndef __EX_LOAD_H__
#define __EX_LOAD_H__

//=========================================================================
//	
//=========================================================================

#undef GLOBAL
#ifdef __EX_LOAD_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

#define	EXRUN_RET_NORMAL_END		0	 	//�ʏ�I��
#define EXRUN_RET_ERROR_END			1		//�G���[�AMSG�Ȃ�
#define	EXRUN_RET_NORMAL_MSG_END	2		//����I���AMSG����
#define	EXRUN_RET_ERROR_MSG_END		3		//�G���[�AMSG����

#ifndef	ASM_CPP
//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

#ifdef	PM_DEBUG
//�ނ���̃`�P�b�g����C�x���g�p�O���f�[�^
extern const u8 ExtraDataStartMugenTicketScript[];
extern const u8 ExtraDataEndMugenTicketScript[];

//�|�P��������O���f�[�^�̃T���v��
extern const u8 ExtraDataStartSamplePokeData[];
extern const u8 ExtraDataEndSamplePokeData[];

//�O���f�[�^�A�h���X�̔z��
extern const u8* const ExDataAdrs[][2];

//�O���f�[�^�̎w��ID
enum{
	EXDATA_MUGEN_TICKET = 0,
	EXDATA_SAMPLE_POKEDATA,
	EXDATA_SAMPLE_RIBBON,
	EXDATA_SAMPLE_ZUKAN,
	EXDATA_SAMPLE_WORD,

	EXDATA_END,
};

//�T���v���f�[�^�������[�`���̃e�[�u��
typedef u32 (*ExDataMakeFunc)(u8 *);
typedef struct _EX_SAMPLE_DATA {
	const u8 * title;
	ExDataMakeFunc make_func;
}EX_SAMPLE_DATA;

extern const EX_SAMPLE_DATA  ExDataMaker[];
extern const u8 SizeOfExDataMaker;

#endif	PM_DEBUG


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);
//-------------------------------------------------------------------------
//�O���ǉ��f�[�^����
//	���O������ǉ����ꂽ�A�h���X�����߁A��������
//	<����>
//		ex_data			�O���f�[�^�A�h���X
//	<�߂�l>
//		EXRUN_RET_�`	����`���Q��
//-------------------------------------------------------------------------
GLOBAL int RunExtraData( const u8 * ex_data );

//-------------------------------------------------------------------------
//	�O���f�[�^�G���[�R�[�h�Z�b�g
//	<����>	�G���[�R�[�h
//			��EXRUN_RET_�`�̒�`���Q��
//-------------------------------------------------------------------------
GLOBAL void SetExtraDataError( u32 error );

//-------------------------------------------------------------------------
//	�z�z�A�C�e���̑��݃`�F�b�N
//-------------------------------------------------------------------------
GLOBAL int CheckExItemExists( void );

//-------------------------------------------------------------------------
//	�z�z�A�C�e���̏���
//-------------------------------------------------------------------------
GLOBAL void ClearExItem( void );

//-------------------------------------------------------------------------
//	�z�z�A�C�e���̐ݒ�
//	<����>	type		�z�z��ʁi���̂Ƃ���P��ނ̂݁j
//			count		�z�z���
//			number		�z�z�A�C�e���i���o�[
//-------------------------------------------------------------------------
GLOBAL void SetExItem( u8 type, u8 count, u16 number );

//-------------------------------------------------------------------------
//	�z�z�A�C�e���擾
//	<�߂�l>		�A�C�e���i���o�[
//					�O�̂Ƃ��A���݂��Ȃ�
//	�������I�ɔz�z�����J�E���g�_�E������
//-------------------------------------------------------------------------
GLOBAL u16 GetExItem( void );


//-------------------------------------------------------------------------
//	�O���f�[�^���t���b�V���ɃZ�[�u����
//	���f�o�b�O�p
//	<����>	�O���f�[�^�w��ID
//-------------------------------------------------------------------------
GLOBAL void DebugExtraDataSave( u8 id );


#endif	/* ASM_CPP */

#undef GLOBAL
#endif	/*__EX_LOAD_H__*/

