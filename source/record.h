#ifndef __RECORD_H__
#define __RECORD_H__

//=========================================================================
//	
//
//	�Q�[�����o�������낢��J�E���g�V�X�e��
//
//
//	2001.11.19	GAME FREAK Inc.
//=========================================================================

#undef GLOBAL
#ifdef __RECORD_H_GLOBAL__
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value

//typedef ...

// �����L���O�p�J�E���^��`
#define	CNT_REPORT				0			//	���|�[�g����������
#define	SV_TOTAL_TIME			1			//���N���A�܂ł̎���
#define	CNT_TREND				2			//	���s���������񐔁i�����^�E���œ��͂���j
#define	CNT_NUTS				3			//	���݂̂���������
#define	CNT_BICYCLE_TRADE		4			//	���]�Ԃ�����������
#define	CNT_WALK				5			//������
#define	CNT_INTERVIEW			6			//	�C���^�r���[�ɓ�������
#define	CNT_BATTLE				7			//���퓬��
#define	CNT_WILD_BATTLE			8			//���퓬�񐔁i�΁@�쐶�̃|�P�����j
#define	CNT_TRAINER_BATTLE		9			//	�퓬�񐔁i��CPU�̂݁A�ΐ�E�^���[�Ȃǂ͏����j
#define	CNT_DENDOU				10			//���a������̉�
#define	CNT_CATCH_POKE			11			//	�|�P������߂܂�����
#define	CNT_TURI				12			//	�|�P������������
#define	CNT_TAMAGO				13			//	�^�}�S���z������
#define	CNT_SHINKA				14			//	�|�P�������i��������(�ʐM�i���܂ށj
#define	CNT_KAIFUKU_PC			15			//	�o�b�ŉ񕜂�����
#define	CNT_KAIFUKU_HOME		16			//	�����̉Ƃŉ񕜂�����
#define	CNT_SAFARI				17			//	�T�t�@���]�[���ɓ�������
#define	CNT_HIDEN_IAIGIRI		18			//	�t�B�[���h�Łu����������v���g������
#define	CNT_HIDEN_IWAKUDAKI		19			//	�t�B�[���h�Łu���킭�����v��������
#define	CNT_AGITTRANS			20			//	�閧��n�������z������
#define	CNT_COMM_TRADE			21			//���ʐM������������
#define	CNT_COMM_TAISEN			22			//���ʐM�ΐ��������
#define	CNT_KACHI				23			//���ʐM�ΐ�ł̏�����
#define	CNT_MAKE				24			//���ʐM�ΐ�ł̕�����
#define	CNT_HIKIWAKE			25			//���ʐM�ΐ�ł̈��������̐�
#define	CNT_WAZA_HANERU			26			//	�퓬�Łu�͂˂�v����������
#define	CNT_WAZA_WARUAGAKI		27			//	�퓬�Łu��邠�����v������
#define	CNT_SLOT				28			//	�X���b�g�����������肳������
#define	CNT_ROULETTE			29			//	���[���b�g��A���ł��Ă���
#define	CNT_TOWER_BATTLE		30			//	�o�g���^���[�ɂ��ǂ񂾉�
#define	CNT_TOWER_KACHI			31			//���o�g���^���[�ł̏�����
#define	CNT_TOWER_RENSHOU		32			//���o�g���^���[�ł̘A����
#define	CNT_MAKE_CUBE			33			//	�L���[�u���������(�ʐM�ӂ��ށj
#define	CNT_COMM_CUBE_MAKE		34			//���ʐM�ŃL���[�u����������
#define	CNT_COMM_CONTEST_WIN	35			//���P�[�u���R���e�X�g�ł̗D����
#define	CNT_CONTEST				36			//	�R���e�X�g�ɎQ�������񐔁i�ʐM�܂܂Ȃ��j
#define	CNT_CONTEST_WIN			37			//	�R���e�X�g�ł̗D���񐔁i�ʐM�܂܂Ȃ��j
#define CNT_SHOPPING			38			//	��������������
#define	CNT_DOWSING				39			//	�_�E�W���O�}�V������������
#define	CNT_RAIN				40			//	�J�ɍ~��ꂽ��
#define	CNT_POKEDEX				41			//	��������݂���
#define	CNT_RIBBON				42			//	���{�������������
#define	CNT_JUMP				43			//	�i�����э~�肽��
#define	CNT_TV_WATCH			44			//	�e���r���݂���
#define	CNT_CLOCK_WATCH			45			//	���v���݂���
#define	CNT_POKE_LOTO			46			//	�|�P��������������������
#define	CNT_SODATEYA			47			//	��ĉ��ɗa������
#define	CNT_ROPEWAY				48			//	���[�v�E�F�C�ɂ̂�����
#define	CNT_ONSEN				49			//	����ɓ�������

#define	CNT_END					50			//


//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

#ifndef	ASM_CPP		/*�ȉ��͔�A�Z���u���ł̂ݗL��*/
GLOBAL void RecordInit( void );
GLOBAL void IncRecord( u8 id );
GLOBAL u32 GetRecord( u8 id );
GLOBAL void SetRecord( u8 id, u32 value );
//#define IncRecord(id)	( Fld.Record[(id)] == 0xffffffff ? 0 : Fld.Record[(id)]++ )
//#define	GetRecord(id)	( Fld.Record[(id)] )
#endif	/*ASM_CPP*/


#undef GLOBAL
#endif	/*__RECORD_H__*/

