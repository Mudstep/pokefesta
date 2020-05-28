//=========================================================================
//	
//	�T�E���h�c�[���֘A
//
//	2001	GAME FREAK Inc.
//	2001.10.26	by tama �؂���ɂ�������\�[�X�����ɉ��쐬
//=========================================================================

#include "common.h"
#include "task.h"
#include "poketool.h"		//add by soga 2002.07.11
#include "server.h"			//add by matsuda 2002.09.30(��)
#include "fight.h"

#define	MUS_TOOL_H_GLOBAL
#include "mus_tool.h"


//==============================================================
//
//==============================================================
enum{
	MMSTAT_INIT = 0,
	MMSTAT_PLAYSTART,
	MMSTAT_PLAY,
	MMSTAT_FADEIN,
	MMSTAT_FADEIN2,
	MMSTAT_FADEOUT,
	MMSTAT_FADENEXT,
	MMSTAT_FADENEXTFADE,
};

static u16 NowMusicNo;		//���݂̂a�f�l�i���o�[
static u16 NextMusicNo;		//���̂a�f�l�i���o�[
static u8 MusicStatus;		//���t���
static u8 MusicFadeCount;	//�t�F�[�h�^�C�~���O

u8 BGMStopFlag;		//BGM�̃I���I�t�t���O

//==============================================================
//
//==============================================================
//----------------------------------------------
//----------------------------------------------
void BGM_SystemInit( void )
{
	BGMStopFlag = 0;
	BGM_Init();
}

//----------------------------------------------
//	�a�f�l���C���i���C�����[�v���ɂ����j
//----------------------------------------------
void BGM_Main( void )
{
	switch( MusicStatus ) {

	case MMSTAT_INIT:
		/* ���������� */
		break;

	case MMSTAT_PLAYSTART:
		/* BGM�Z�b�g���� */
		MusicStatus = MMSTAT_PLAY;
		MusicPlay( NowMusicNo );
		break;

	case MMSTAT_PLAY:
		/* ���t���A�������Ȃ� */
		break;

	case MMSTAT_FADEOUT:
		/* �t�F�[�h�A�E�g���� */
		if( MusicFadeOutEndWait() == FALSE ) break;
		NextMusicNo = 0;
		MusicStatus = MMSTAT_INIT;
		break;

	case MMSTAT_FADENEXT:
		/* �t�F�[�h�A�E�g���a�f�l�Z�b�g���� */
		if( MusicFadeOutEndWait() == FALSE ) break;
		if( JingleWait() == 0) break;

		NowMusicNo = NextMusicNo;
		NextMusicNo = 0;
		MusicStatus = MMSTAT_PLAY;
		MusicPlay( NowMusicNo );
		break;

	case MMSTAT_FADENEXTFADE:
		/* �t�F�[�h�A�E�g���a�f�l�t�F�[�h�C�� */
		if( MusicFadeOutEndWait() == FALSE ) break;
		if( JingleWait() == 0) break;

		MusicFadeInPlay( NextMusicNo, MusicFadeCount );

		NowMusicNo = NextMusicNo;
		NextMusicNo = 0;
		MusicStatus = MMSTAT_PLAY;
		MusicFadeCount = 0;
		break;
	}
}

//----------------------------------------------
//	�a�f�l������
//----------------------------------------------
void BGM_Init( void )
{
	NowMusicNo = 0;
	NextMusicNo = 0;
	MusicStatus = MMSTAT_INIT;
	MusicFadeCount = 0;
}

//----------------------------------------------
//	���݂̂a�f�l�擾
//	<�߂�l>	���݂̂a�f�l�ԍ�
//----------------------------------------------
u16 BGM_GetNowNo( void )
{
	return NowMusicNo;
}

//----------------------------------------------
//	�a�f�l�Z�b�g
//	<����>	music	�a�f�l�̔ԍ�
//----------------------------------------------
void BGM_PlaySet( u16 music )
{
	NowMusicNo = music;
	NextMusicNo = 0;
	MusicStatus = MMSTAT_PLAYSTART;
}

//----------------------------------------------
//	�a�f�l���X�g�b�v
//----------------------------------------------
void BGM_PlayStop( void )
{
	NowMusicNo = 0;
	NextMusicNo = 0;
	MusicStatus = MMSTAT_PLAYSTART;
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g�Z�b�g
//	<����> sec		�t�F�[�h�I���܂ł̎��ԁi�w��l�~16 / 60 �b )
//----------------------------------------------
void BGM_FadeOutSet( u8 sec )
{
	if ( BGM_FadeOutCheck() ) {
		MusicFadeOutEnd( sec );
	}
	NowMusicNo = 0;
	NextMusicNo = 0;
	MusicStatus = MMSTAT_FADEOUT;
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g�����̂a�f�l�Z�b�g
//	<����>	music	�a�f�l�̔ԍ�
//			sec		�t�F�[�h�I���܂ł̎��ԁi�w��l�~16 / 60 �b )
//	�����̖��߂��g�p�����BGM_Main����JingleWait->CheckTask��
//	�@�ĂԁB�W���O���炵���ςȂ�&�^�X�N�����Ȃ��󋵂������
//	�@�n���O�A�b�v����̂Œ��ӁI
//----------------------------------------------
void BGM_FadeNextSet( u16 music ,u8 sec )
{
	BGM_FadeOutSet( sec );

	NowMusicNo = 0;
	NextMusicNo = music;
	MusicStatus = MMSTAT_FADENEXT;
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g�����̂a�f�l�t�F�[�h�C���Z�b�g
//	<����>	music	�a�f�l�̔ԍ�
//			sec_out		�t�F�[�h�I���܂ł̎��ԁi�w��l�~16 / 60 �b )
//			sec_in		�t�F�[�h�C���̎��ԁi�w��l�~16 / 60 �b )
//	�����̖��߂��g�p�����BGM_Main����JingleWait->CheckTask��
//	�@�ĂԁB�W���O���炵���ςȂ�&�^�X�N�����Ȃ��󋵂������
//	�@�n���O�A�b�v����̂Œ��ӁI
//----------------------------------------------
void BGM_FadeNextFadeSet( u16 music ,u8 sec_out, u8 sec_in )
{
	BGM_FadeOutSet( sec_out );

	NowMusicNo = 0;
	NextMusicNo = music;
	MusicStatus = MMSTAT_FADENEXTFADE;
	MusicFadeCount = sec_in;
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�C���Z�b�g
//	<����>	music	�a�f�l�̔ԍ�
//			sec		�t�F�[�h�C���̎��ԁi�w��l�~16 / 60 �b )
//----------------------------------------------
void BGM_FadeInPlaySet( u16 music, u8 sec )
{
	MusicFadeInPlay( music, sec );

	NowMusicNo = music;
	NextMusicNo = 0;
	MusicStatus = MMSTAT_PLAY;
	MusicFadeCount = 0;
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g�����ǂ����̃`�F�b�N
//	<�߂�l>	TRUE	�t�F�[�h�A�E�g���łȂ�
//				FALSE	�t�F�[�h�A�E�g��
//----------------------------------------------
u8 BGM_FadeOutCheck( void )
{
	if( MusicStatus == MMSTAT_FADENEXT ) return FALSE;
	if( MusicStatus == MMSTAT_FADEOUT ) return FALSE;
	if( MusicStatus == MMSTAT_FADENEXTFADE ) return FALSE;
	return TRUE;
}




//==================================================================
//
//	�ϐ���`�Ȃ�
//
//==================================================================
static u16 me_timer;

typedef struct {
	u16 MusicNo;	/* �Ȕԍ� */
	u16 wait;		/* �E�F�C�g */
}ME_TABLE;

//----------------------------------------------
//	�W���O���o�^�e�[�u��
//----------------------------------------------
static const ME_TABLE	MeTable[] =
{
	{ MUS_FANFA1,	 80 },		/* �A�C�e������������Ƃ� */
	{ MUS_FANFA4,	160 },		/* �A�C�e������������Ƃ� */
	{ MUS_FANFA5,	220 },		/* �|�P�����i���� */
	{ MUS_ME_WAZA,	220 },		/* �Z�}�V���Q�b�g�� */
	{ MUS_ME_ASA,	160	},		/* PC�Ń|�P�����񕜎� */
	{ MUS_ME_BACHI,	340	},		/* �W���o�b�`�Q�b�g�� */
	{ MUS_ME_WASURE, 180 },		/* �Z�Y�� */
	{ MUS_ME_KINOMI, 120 },		/* ���݂̂��Q�b�g */
	{ MUS_ME_TAMA, 710 },		/* �J�C�I�[�K�̃C�x���g�p */
	{ MUS_ME_B_BIG, 250 },		/* �~�j�Q�[���吳�� */
	{ MUS_ME_B_SMALL, 150 },	/* �~�j�Q�[������ */
	{ MUS_ME_ZANNEN, 160 },		/* �~�j�Q�[���c�O */

//	{ MUS_FANFA1,	120 },		/* �A�C�e������������Ƃ� */
//	{ MUS_FANFA4,	120 },		/* �A�C�e������������Ƃ� */
//	{ MUS_FANFA5,	120 },		/* �|�P�����i���� */
//	{ MUS_ME_WAZA,	120 },		/* �Z�}�V���Q�b�g�� */
//	{ MUS_ME_ASA,	120	},		/* PC�Ń|�P�����񕜎� */
//	{ MUS_ME_BACHI,	120	},		/* �W���o�b�`�Q�b�g�� */
};

#define	ME_TABLE_MAX	NELEMS(MeTable)


//==================================================================
//
//	�~���[�W�b�N�G�t�F�N�g����
//
//	original by aoki
//==================================================================
/*----------------------------------------------*/
/*		�~���[�W�b�N�G�t�F�N�g�E���N�G�X�g		*/
/*----------------------------------------------*/
// MePlay( u8 me_num, u8 mode );
// me_num �c �l�d�i���o�[
void MePlay( u8 me_num )
{
	u16	play_num;

	m4aMPlayStop( &m4a_mplay000 );
	play_num = MeTable[ me_num ].MusicNo;
	me_timer = MeTable[ me_num ].wait;
	m4aSongNumStart( play_num );
}

/*----------------------------------------------*/
/*		�~���[�W�b�N�G�t�F�N�g�I���҂�			*/
/*		�����O�ɕK��MePlay()���ĂԁI			*/
/*----------------------------------------------*/
// mode �c 0=MePlay�ňꎞ��~�����a�f�l���ĊJ
//         1=MePlay�ňꎞ��~�����a�f�l���ĊJ���Ȃ�
// return �c 0=�l�d���t�� 1=�l�d���t�I��
u8 MeWait( u8 mode )
{
	if( me_timer ){
		me_timer--;
		return 0;
	}

	if( mode == 0 ){
		m4aMPlayContinue( &m4a_mplay000 );
	}else
		m4aSongNumStart( 0 );
	return 1;
}

/*----------------------------------------------*/
/*	�l�d���~�߂�								*/
/*----------------------------------------------*/
void MeStop( u8 me_num )
{
	m4aSongNumStop( MeTable[ me_num ].MusicNo );
}


//==================================================================
//
//	�ȒP�W���O������
//
//==================================================================
static void add_me_wait_task(void);
static void me_wait_task(u8 no);

//----------------------------------------------
//�ȒP�W���O���J�n
//----------------------------------------------
void JinglePlay( u16 music )
{
	unsigned int i;
	for( i=0; i<ME_TABLE_MAX; i++ )
	{
		if( MeTable[i].MusicNo == music )
		{
			MePlay(i);
			add_me_wait_task();
			return;
		}
	}
	MePlay(0);	//�o�^����Ă��Ȃ��ꍇ�͂Ƃ肠����
	add_me_wait_task();
}

//----------------------------------------------
//�ȒP�W���O���҂�
//<�߂�l>	0	���t��
//			1	�I��
//----------------------------------------------
u8 JingleWait(void)
{
	if( CheckTask(me_wait_task) == TRUE )return FALSE;
	return TRUE;
//	return MeWait(0);
}

//----------------------------------------------
//----------------------------------------------
static void me_wait_task(u8 no)
{
	if( me_timer )
	{
		me_timer --;
		return;
	}else{
	//	if( !BGMStopFlag )
			m4aMPlayContinue( &m4a_mplay000 );
		DelTask(no);
	};
}

void add_me_wait_task(void)
{
	if( CheckTask(me_wait_task) == TRUE )return;
	AddTask( me_wait_task, 80 );
}


//==================================================================
//
//	�a�f�l�t�F�[�h
//
//==================================================================

//----------------------------------------------
//	BGM�t�F�[�h�C���Z�b�g
//	���Ȃ��Ȃ��Ă��Ȃ���Ԃ��炢���Ȃ�t�F�[�h�C������
//	<����>		music	�w��ȃi���o�[
//				sec		�t�F�[�h�^�C�~���O
//----------------------------------------------
void MusicFadeInPlay( u16 music, u8 sec )
{
//	if ( BGMStopFlag != 0 ) return;
	if (BGMStopFlag != 0) music = MUS_DUMMY;
	m4aSongNumStart( music );
	m4aMPlayImmInit( &m4a_mplay000 );
	m4aMPlayVolumeControl( &m4a_mplay000, 0xffff, 0 );
	m4aSongNumStop( music );
	m4aMPlayFadeInContinue(&m4a_mplay000, sec );
}

//----------------------------------------------
//	BGM�t�F�[�h�A�E�g�Z�b�g
//	���I���ł͂Ȃ��A�����I�ɂ̓|�[�Y��ԂɂȂ��Ă���
//	<����>		sec		�t�F�[�h�^�C�~���O
//----------------------------------------------
void MusicFadeOutPause( u8 sec )
{
	m4aMPlayFadeOutPause( &m4a_mplay000, sec );
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g���|�[�Y�@�҂�
//	<�߂�l>	TRUE	�I��
//				FALSE	�t�F�[�h��
//----------------------------------------------
u8 MusicFadeOutPauseWait(void)
{
	// �|�[�Y��ԂɂȂ��Ă���
	if( m4a_mplay000.stat & MUSICPLAYER_STATUS_PAUSE ) return TRUE;
	// ���t�Ȃ��Ȃ�
	if( ( m4a_mplay000.stat & MUSICPLAYER_STATUS_TRACK ) == 0 ) return TRUE;

	return FALSE;
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�C���Z�b�g
//	��MusicFadeOutPause�Ńt�F�[�h�A�E�g���|�[�Y��Ԃ�
//	�@�Ȃ��Ă���Ȃ�������x�t�F�[�h�C���ōĊJ����
//	<����>		sec		�t�F�[�h�^�C�~���O
//----------------------------------------------
void MusicFadeInContinue( u8 sec )
{
	m4aMPlayFadeInContinue( &m4a_mplay000, sec );
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g�Z�b�g
//	���t�F�[�h�A�E�g��A�I��������
//	<����>		sec		�t�F�[�h�^�C�~���O
//----------------------------------------------
void MusicFadeOutEnd( u8 sec )
{
	m4aMPlayFadeOut( &m4a_mplay000, sec );
}

//----------------------------------------------
//	�a�f�l�t�F�[�h�A�E�g�I���҂�
//	<�߂�l>	TRUE	�I��
//				FALSE	�t�F�[�h��
//----------------------------------------------
u8 MusicFadeOutEndWait(void)
{
	// ���t�Ȃ��Ȃ�
	if( ( m4a_mplay000.stat & MUSICPLAYER_STATUS_TRACK ) == 0 ) return TRUE;

	return FALSE;
}

//==================================================================
//
//	�����Đ��֘A
//
//==================================================================

extern	ToneData	voicegroup200[];
extern	ToneData	voicegroup201[];
extern	ToneData	voicegroup202[];
extern	ToneData	voicegroup203[];
extern	ToneData	voicegroup204[];
extern	ToneData	voicegroup205[];
extern	ToneData	voicegroup206[];
extern	ToneData	voicegroup207[];

static void vp_wait_task( u8 no );
static void add_vp_wait_task( void );

MusicPlayerArea*	vp_end = 0;
u8	vp_timer = 0;

//----------------------------------------------
//�ȒP�����Đ�
//----------------------------------------------
#define VOICE_MASTER_VOL	125
#define VOICE_SMALL_VOL		80
#define VOICE_PRI_HI		10

//----------------------------------------------
//�ǂ��ł��g���p
void VoicePlay( u16 no, s8 pan )
{
	m4aMPlayVolumeControl( &m4a_mplay000, 0xffff, 256/3 );
	VoicePlaySub( no, pan, VOICE_MASTER_VOL, VOICE_PRI_HI, 0 );
	vp_timer = 2;			//�g�`�̍Đ��I������R���h������
	add_vp_wait_task();
}

//----------------------------------------------
//�t�B�[���h��Ń����_���Ŗ�����p
void VoicePlay2( u16 no, s8 pan, s8 vol, u8 pri )
{
	VoicePlaySub( no, pan, vol, pri, 0 );
}

//----------------------------------------------
//�퓬�p
void VoicePlay3( u16 no, s8 pan, u8 ptn )
{
	if( ptn == 1 ){
		VoicePlaySub( no, pan, VOICE_MASTER_VOL, VOICE_PRI_HI, ptn );
	}else{
		m4aMPlayVolumeControl( &m4a_mplay000, 0xffff, 256/3 );
		VoicePlaySub( no, pan, VOICE_MASTER_VOL, VOICE_PRI_HI, ptn );
		vp_timer = 2;			//�g�`�̍Đ��I������R���h������
		add_vp_wait_task();
	}
}

//add by matsuda 2002.09.30(��)
void VoicePlay4( u16 no, s8 pan, u8 ptn )
{
	if( ptn == 1 ){
		VoicePlaySub( no, pan, VOICE_MASTER_VOL, VOICE_PRI_HI, ptn );
	}else{
		if((FightType&FIGHT_TYPE_4_2vs2) == 0)
			m4aMPlayVolumeControl( &m4a_mplay000, 0xffff, 256/3 );
		VoicePlaySub( no, pan, VOICE_MASTER_VOL, VOICE_PRI_HI, ptn );
//		vp_timer = 2;			//�g�`�̍Đ��I������R���h������
//		add_vp_wait_task();
	}
}

//----------------------------------------------
//�t�B�[���h��̃C�x���g�p
void VoicePlayEvent( u16 no, u8 ptn )
{
	m4aMPlayVolumeControl( &m4a_mplay000, 0xffff, 256/3 );
	VoicePlaySub( no, 0, VOICE_MASTER_VOL, VOICE_PRI_HI, ptn );
	vp_timer = 2;			//�g�`�̍Đ��I������R���h������
	add_vp_wait_task();
}

//----------------------------------------------
void VoicePlaySub( u16 no, s8 pan, s8 vol, u8 pri, u8 ptn )
//ptn = 0 �ʏ�Đ�
//    = 1 �Đ����ԒZ�k�i�Q�����Q�o�g���Ń|�P�����o�ꎞ�j
//    = 2 �{�����|�P�����i�t�B�[���h�C�x���g�j�A�Z���ʉ��E�ق���
//    = 3 �Z���ʉ��E�Ƃ��ڂ��p
//    = 4 �Z���ʉ��E�n�C�p�[�{�C�X�p
//    = 5 �|�P���������ꂽ��

{
	u8	bank;
	u8	num;
	u8	reverse;		//�t�Đ�����p
	u8	release;		//�L�[�I�t��̃����[�X�w��p
	u16	play_length;	//�Đ����Ԏw��p
	u16	pitch;
	s8	chorus;

	no -= 1;
	reverse = 0;
	release = 0;
	play_length = 140;
	pitch = 15360;
	chorus = 0;

	switch( ptn ){
		//�ʏ�Đ�
		case 0:
		//	play_length = 140;
		//	release = 0;
			break;
		//���ԒZ�k�Đ�
		case 1:
			play_length = 20;
			release = 225;
			break;
		//�{�����|�P����
		case 2:
			play_length = 30;
			release = 225;
			pitch = 15600;
			chorus = 20;
			vol = VOICE_SMALL_VOL;
			break;
		//�Z���ʉ��E�Ƃ��ڂ��p
		case 3:
			play_length = 50;
			release = 200;
			pitch = 14800;
			break;
		//�Z���ʉ��E�n�C�p�[�{�C�X�p
		case 4:
			play_length = 20;
			release = 220;
			pitch = 15800;
			break;
		//�|�P���������ꂽ��
		case 5:
		//	play_length = 50;
			release = 200;
			pitch = 14500;
			break;
	}

	pokVoiceVolume( vol );				//�������ʐݒ� (0�`127)
	pokVoicePanpot( pan );				//�����p���|�b�g�w�� (-127�`127)
	pokVoicePitch( pitch );				//�����s�b�`�w�� (256=����)
	pokVoiceLength( play_length );		//�Đ����Ԏw�� (60=1�b)
	pokVoiceProgress( 0 );				//�g�`�Đ��J�n�ʒu�w�� (0�`n�o�C�g)
	pokVoiceRelease( release );			//�g�`�Đ��̌��������w��
	pokVoiceChorus( chorus );			//�R�[���X�w��
	pokVoicePriority( pri );			//�v���C�I���e�B�[�w��

	no=PokeMonsNo2VoiceNoGet(no);		//�|�P�����i���o�[��������i���o�[���擾
										//add by soga 2002.07.11

	num = no % 128;
	bank = no / 128;
	switch( bank ){
		case 0:
			if( reverse )
				vp_end = pokVoiceStart( &voicegroup204[ num ] );		//�g�`���t�Đ�
			else
				vp_end = pokVoiceStart( &voicegroup200[ num ] );		//�g�`�𒼐ڍĐ�
			break;
		case 1:
			if( reverse )
				vp_end = pokVoiceStart( &voicegroup205[ num ] );		//�g�`���t�Đ�
			else
				vp_end = pokVoiceStart( &voicegroup201[ num ] );		//�g�`�𒼐ڍĐ�
			break;
		case 2:
			if( reverse )
				vp_end = pokVoiceStart( &voicegroup206[ num ] );		//�g�`���t�Đ�
			else
				vp_end = pokVoiceStart( &voicegroup202[ num ] );		//�g�`�𒼐ڍĐ�
			break;
		case 3:
			if( reverse )
				vp_end = pokVoiceStart( &voicegroup207[ num ] );		//�g�`���t�Đ�
			else
				vp_end = pokVoiceStart( &voicegroup203[ num ] );		//�g�`�𒼐ڍĐ�
			break;
	}
}

extern	void	ImmSngClear(void);
//----------------------------------------------
//�ȒP�����Đ��I���҂�
//<�߂�l>	0	���t��
//			1	�I��
//----------------------------------------------
u8 VoicePlayWait( void )
{
	if( CheckTask( vp_wait_task ) == TRUE )
		return FALSE;
	ImmSngClear();
	return TRUE;
}

//----------------------------------------------
//����������~
//----------------------------------------------
void VoicePlayStop( void )
{
	m4aMPlayStop( vp_end );
	ImmSngClear();
}

void VoicePlayStop2( void )	//�}�Ӑ�p
{
	m4aMPlayStop( vp_end );
}

//----------------------------------------------
//�����̍Đ��I���𒲂ׂ�
//----------------------------------------------
//�Ԃ�l
// 0�c�I�����Ă���
// 1�c�Đ���
u8 VoiceEndCheck( void )
{
	if( pokVoiceCheck( vp_end ) )
		return 1;
	ImmSngClear();
	return 0;
}

u8 VoiceEndCheck2( void )	//�}�Ӑ�p
{
	if( pokVoiceCheck( vp_end ) )
		return 1;
	return 0;
}

//----------------------------------------------
//----------------------------------------------
static void vp_wait_task( u8 no )
{
	if( vp_timer ){
		vp_timer--;
		return;
	}
	if( pokVoiceCheck( vp_end ) == 0 ){
		m4aMPlayVolumeControl( &m4a_mplay000, 0xffff, 256 );
		DelTask(no);
	}
}

static void add_vp_wait_task( void )
{
	if( CheckTask( vp_wait_task ) == TRUE )
		return;
	AddTask( vp_wait_task, 80 );
}

//==================================================================
//
//	BGM�Đ��ASE�Đ�
//
//==================================================================
//----------------------------------------------
//BGM�Đ�
//----------------------------------------------
void MusicPlay( u16 no )
{
//	if ( BGMStopFlag != 0 ) return;
	if (BGMStopFlag != 0) no = MUS_DUMMY;
	m4aSongNumStart( no );
}

//----------------------------------------------
//SE�Đ�
//----------------------------------------------
void SePlay( u16 no )
{
	m4aSongNumStart( no );
}

//----------------------------------------------
//SE�Đ��i�p���t���j
//----------------------------------------------
void SePlayPan( u16 no, s8 pan )
{
	m4aSongNumStart( no );
	m4aMPlayImmInit( &m4a_mplay001 );
	m4aMPlayImmInit( &m4a_mplay002 );
	m4aMPlayPanpotControl( &m4a_mplay001, 0xffff, pan );
	m4aMPlayPanpotControl( &m4a_mplay002, 0xffff, pan );
}

//----------------------------------------------
//SE�Đ��i�`�����l��1�݂̂̃p�����j
//----------------------------------------------
void SePlayPanCh1( u16 no, s8 pan )
{
	m4aSongNumStart( no );
	m4aMPlayImmInit( &m4a_mplay001 );
	m4aMPlayPanpotControl( &m4a_mplay001, 0xffff, pan );
}

//----------------------------------------------
//SE�Đ��i�`�����l���Q�݂̂̃p�����j
//----------------------------------------------
void SePlayPanCh2( u16 no, s8 pan )
{
	m4aSongNumStart( no );
	m4aMPlayImmInit( &m4a_mplay002 );
	m4aMPlayPanpotControl( &m4a_mplay002, 0xffff, pan );
}

//----------------------------------------------
//SE�̃p���̂ݕύX�iSE�Đ����̂ݗL���j
//----------------------------------------------
void SePan( s8 pan )
{
	m4aMPlayPanpotControl( &m4a_mplay001, 0xffff, pan );
	m4aMPlayPanpotControl( &m4a_mplay002, 0xffff, pan );
}

//----------------------------------------------
//SE�̍Đ��I���𒲂ׂ�
//----------------------------------------------
//�Ԃ�l
// 0�c�I�����Ă���
// 1�c�Đ���
u8 SeEndCheck( void )
{
	// �|�[�Y��ԂɂȂ��Ă���
	if( ( m4a_mplay001.stat & MUSICPLAYER_STATUS_PAUSE ) &&
		( m4a_mplay002.stat & MUSICPLAYER_STATUS_PAUSE ) )
		 return 0;
	// ���t�Ȃ��Ȃ�
	if( ( ( m4a_mplay001.stat & MUSICPLAYER_STATUS_TRACK ) == 0 ) &&
		( ( m4a_mplay002.stat & MUSICPLAYER_STATUS_TRACK ) == 0 ) )
		return 0;

	return 1;
}

//-- add 2002.09.30(��) by matsuda --//
u8 MusEndCheck( void )
{
	// �|�[�Y��ԂɂȂ��Ă���
	if( m4a_mplay000.stat & MUSICPLAYER_STATUS_PAUSE )
		 return 0;
	// ���t�Ȃ��Ȃ�
	if( ( m4a_mplay000.stat & MUSICPLAYER_STATUS_TRACK ) == 0 )
		return 0;

	return 1;
}

//----------------------------------------------
//SE�̍Đ��I���𒲂ׂ� ( music_player_ 3 ���� �j
//----------------------------------------------
//�Ԃ�l
// 0�c�I�����Ă���
// 1�c�Đ���
u8 SeEndCheckPlayer003( void )
{
	// �|�[�Y��ԂɂȂ��Ă���
	if( m4a_mplay003.stat & MUSICPLAYER_STATUS_PAUSE )
		 return 0;
	// ���t�Ȃ��Ȃ�
	if( ( m4a_mplay003.stat & MUSICPLAYER_STATUS_TRACK ) == 0 )
		return 0;

	return 1;
}

