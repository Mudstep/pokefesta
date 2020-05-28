
#ifndef __FLD_SIO_H__
#define __FLD_SIO_H__


//=========================================================================
//	
//=========================================================================

#undef GLOBAL
#ifdef __FLD_SIO_H_GLOBAL_
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================

//�t�B�[���h�ł̒ʐM��ԊĎ��@�\��L���ɂ��邽�߂̒�`�i�f�o�b�O�j
//#define	FIELD_TUUSHIN_DEBUG



//FIFO�����V�X�e���p�̒�`
enum {
	//FIFO�������~����FIFO�i��
	FLD_RFIFO_LIMIT_VALUE	=	2,
	FLD_SFIFO_LIMIT_VALUE	=	2,

	//FIFO�������J�n����FIFO�i��
	FLD_RFIFO_CLEAR_VALUE	=	4,
	FLD_SFIFO_CLEAR_VALUE	=	4,
};


#if	defined(FIELD_TUUSHIN_DEBUG) && defined(PM_DEBUG)
enum {
	//�f�o�b�O�p���[�N�̑傫����`
	DB_INFO_MAX		=	4,

};
#define	DB_SIO_PLAYER_MAX	(4)

	#if	defined(MULTI_SIO_PLAYERS_MAX) && (MULTI_SIO_PLAYERS_MAX!=DB_SIO_PLAYER_MAX)
	#error SIO�v���C�̍ő�l�����Ⴄ�I�I
	#endif

	#if	defined(COMM_PLAYER_MAX) && (COMM_PLAYER_MAX != DB_SIO_PLAYER_MAX)
	#error SIO�v���C�̍ő�l�����Ⴄ�I�I
	#endif

#endif

//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

#if	defined(PM_DEBUG) && defined(FIELD_TUUSHIN_DEBUG)
extern u8 debug_comm_info[DB_INFO_MAX][DB_SIO_PLAYER_MAX];
extern u16 debug_player_cmd[DB_SIO_PLAYER_MAX];
#endif

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

//-------------------------------------------------------------
//��MFIFO�̉����҂�
//<�߂�l>	TRUE	�����҂���
//			FALSE	�����҂��I��
//-------------------------------------------------------------
extern int FieldRecvFifoClearWait( void );	//fld_main.c

//-------------------------------------------------------------
//��MFIFO�̉����������邩?
//-------------------------------------------------------------
extern int IsEnableClearRecvFifo( void );	//fld_main.c

//-------------------------------------------------------------
//���MFIFO�̉����������邩?
//-------------------------------------------------------------
extern int IsEnableClearSendFifo( void );	//fld_main.c

//-------------------------------------------------------------
//���C�������̃X�L�b�v�������邩?
//-------------------------------------------------------------
extern int IsEnableSkipFieldMain( void );	//fld_main.c


#undef GLOBAL
#endif	/*__FLD_SIO_H__*/

