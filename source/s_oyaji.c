//=========================================================================
//
//	�V���K�[�\���O���₶�֘A
//
//=========================================================================
#include "common.h"
#include "task.h"

void SongOyajiInit( s16* work, u16 tango, u16 melody );
void SongOyajiPlay( s16* work, u8 code );
void SongOyajiPicthChange( s16* work );

#define	C3___	((256* 0)-(256*3))
#define CS3__	((256* 1)-(256*3))
#define D3___	((256* 2)-(256*3))
#define DS3__	((256* 3)-(256*3))
#define E3___	((256* 4)-(256*3))
#define F3___	((256* 5)-(256*3))
#define FS3__	((256* 6)-(256*3))
#define G3___	((256* 7)-(256*3))
#define GS3__	((256* 8)-(256*3))
#define A3___	((256* 9)-(256*3))
#define AS3__	((256*10)-(256*3))
#define B3___	((256*11)-(256*3))
#define C4___	((256*12)-(256*3))
#define CS4__	((256*13)-(256*3))
#define D4___	((256*14)-(256*3))
#define DS4__	((256*15)-(256*3))
#define E4___	((256*16)-(256*3))
#define F4___	((256*17)-(256*3))
#define FS4__	((256*18)-(256*3))
#define G4___	((256*19)-(256*3))
#define GS4__	((256*20)-(256*3))
#define A4___	((256*21)-(256*3))
#define AS4__	((256*22)-(256*3))
#define B4___	((256*23)-(256*3))

#define END__	(256*24)

#define OYJ_VOLUME_S	(0.75*256)
#define OYJ_NOBASU		-1
#define OYJ_KILL		-2

enum{
	OYJ_VOLUME = 10,
	OYJ_COUNT,
	OYJ_TANGO,
	OYJ_MELODY,
	OYJ_FLAG,
};

//�\���O�f�[�^�i�P�����j
static const s16 SongOyaji101[] = { C3___, END__, };
static const s16 SongOyaji102[] = { C4___, END__, };
static const s16 SongOyaji103[] = { E3___, END__, };
static const s16 SongOyaji104[] = { G3___, END__, };
static const s16 SongOyaji105[] = { D4___, END__, };

//�\���O�f�[�^�i�Q�����j
static const s16 SongOyaji201[] = { C3___, D3___, END__, };
static const s16 SongOyaji202[] = { C3___, F3___, END__, };
static const s16 SongOyaji203[] = { F3___, G3___, END__, };
static const s16 SongOyaji204[] = { A3___, B3___, END__, };
static const s16 SongOyaji205[] = { C4___, B3___, END__, };

//�\���O�f�[�^�i�R�����j
static const s16 SongOyaji301[] = { C3___, D3___, C3___, END__, };
static const s16 SongOyaji302[] = { G3___, C3___, G3___, END__, };
static const s16 SongOyaji303[] = { C4___, B3___, A3___, END__, };
static const s16 SongOyaji304[] = { E3___, F3___, G3___, END__, };
static const s16 SongOyaji305[] = { A3___, G4___, E4___, END__, };

//�\���O�f�[�^�i�S�����j
static const s16 SongOyaji401[] = { G3___, C4___, G3___, C4___, END__, };
static const s16 SongOyaji402[] = { C4___, G3___, E4___, G3___, END__, };
static const s16 SongOyaji403[] = { E3___, F3___, G3___, A3___, END__, };
static const s16 SongOyaji404[] = { B3___, A3___, G3___, F3___, END__, };
static const s16 SongOyaji405[] = { FS4__, E4___, D4___, CS4__, END__, };

//�\���O�f�[�^�i�T�����j
static const s16 SongOyaji501[] = { C3___, D3___, E3___, F3___, G3___, END__, };
static const s16 SongOyaji502[] = { C4___, B3___, A3___, G3___, F3___, END__, };
static const s16 SongOyaji503[] = { E3___, G3___, C4___, G3___, E3___, END__, };
static const s16 SongOyaji504[] = { C4___, G3___, C4___, G3___, C3___, END__, };
static const s16 SongOyaji505[] = { D4___, B3___, G3___, G3___, A3___, END__, };

//�\���O�f�[�^�i�U�����j
static const s16 SongOyaji601[] = { C3___, D3___, E3___, F3___, G3___, A3___, END__, };
static const s16 SongOyaji602[] = { B3___, A3___, G3___, F3___, E3___, D3___, END__, };
static const s16 SongOyaji603[] = { E3___, F3___, G3___, E3___, F3___, G4___, END__, };
static const s16 SongOyaji604[] = { G3___, C3___, C4___, G3___, C4___, G3___, END__, };
static const s16 SongOyaji605[] = { B3___, C4___, B3___, C4___, B3___, C4___, END__, };

//�\���O�f�[�^�i�V�����j
static const s16 SongOyaji701[] = { F3___, E3___, F3___, E3___, F3___, G3___, F3___, END__, };
static const s16 SongOyaji702[] = { E3___, E3___, D3___, D3___, C3___, G3___, C3___, END__, };
static const s16 SongOyaji703[] = { B3___, C4___, D4___, E4___, F4___, E4___, D4___, END__, };
static const s16 SongOyaji704[] = { B3___, A3___, G3___, F3___, E4___, D4___, C4___, END__, };
static const s16 SongOyaji705[] = { FS3__, G3___, A3___, B3___, AS3__, B3___, G3___, END__, };

static const s16* const SongOyajiData[]={
	SongOyaji101, SongOyaji102, SongOyaji103, SongOyaji104, SongOyaji105,
	SongOyaji201, SongOyaji202, SongOyaji203, SongOyaji204, SongOyaji205,
	SongOyaji301, SongOyaji302, SongOyaji303, SongOyaji304, SongOyaji305,
	SongOyaji401, SongOyaji402, SongOyaji403, SongOyaji404, SongOyaji405,
	SongOyaji501, SongOyaji502, SongOyaji503, SongOyaji504, SongOyaji505,
	SongOyaji601, SongOyaji602, SongOyaji603, SongOyaji604, SongOyaji605,
	SongOyaji701, SongOyaji702, SongOyaji703, SongOyaji704, SongOyaji705,
};

//�����������f�[�^
static const s16 SongOyajiVoiceData[] = {
	//se code, volume
	OYJ_KILL,	1*256,		/* "�@" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_N,		1*256,		/* "��" */

	SE_A,		OYJ_VOLUME_S,	/* "��" */
	SE_I,		OYJ_VOLUME_S,	/* "��" */
	SE_U,		OYJ_VOLUME_S,	/* "��" */
	SE_E,		OYJ_VOLUME_S,	/* "��" */
	SE_O,		OYJ_VOLUME_S,	/* "��" */
	SE_A,		OYJ_VOLUME_S,	/* "��" */
	SE_U,		OYJ_VOLUME_S,	/* "��" */
	SE_O,		OYJ_VOLUME_S,	/* "��" */

//	OYJ_NOBASU,	OYJ_VOLUME_S,	/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,	/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,	/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,	/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,	/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,	/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,	/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,	/* "��" */

	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	OYJ_KILL,	1*256,		/* "��" */
	SE_A,		1*256,		/* "�A" */
	SE_I,		1*256,		/* "�C" */
	SE_U,		1*256,		/* "�E" */
	SE_E,		1*256,		/* "�G" */
	SE_O,		1*256,		/* "�I" */
	SE_A,		1*256,		/* "�J" */
	SE_I,		1*256,		/* "�L" */
	SE_U,		1*256,		/* "�N" */
	SE_E,		1*256,		/* "�P" */
	SE_O,		1*256,		/* "�R" */
	SE_A,		1*256,		/* "�T" */
	SE_I,		1*256,		/* "�V" */
	SE_U,		1*256,		/* "�X" */
	SE_E,		1*256,		/* "�Z" */
	SE_O,		1*256,		/* "�\" */
	SE_A,		1*256,		/* "�^" */
	SE_I,		1*256,		/* "�`" */
	SE_U,		1*256,		/* "�c" */
	SE_E,		1*256,		/* "�e" */
	SE_O,		1*256,		/* "�g" */
	SE_A,		1*256,		/* "�i" */
	SE_I,		1*256,		/* "�j" */
	SE_U,		1*256,		/* "�k" */
	SE_E,		1*256,		/* "�l" */
	SE_O,		1*256,		/* "�m" */
	SE_A,		1*256,		/* "�n" */
	SE_I,		1*256,		/* "�q" */
	SE_U,		1*256,		/* "�t" */
	SE_E,		1*256,		/* "�w" */
	SE_O,		1*256,		/* "�z" */
	SE_A,		1*256,		/* "�}" */
	SE_I,		1*256,		/* "�~" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_N,		1*256,		/* "��" */

	SE_A,		OYJ_VOLUME_S,		/* "�@" */
	SE_I,		OYJ_VOLUME_S,		/* "�B" */
	SE_U,		OYJ_VOLUME_S,		/* "�D" */
	SE_E,		OYJ_VOLUME_S,		/* "�F" */
	SE_O,		OYJ_VOLUME_S,		/* "�H" */
	SE_A,		OYJ_VOLUME_S,		/* "��" */
	SE_U,		OYJ_VOLUME_S,		/* "��" */
	SE_O,		OYJ_VOLUME_S,		/* "��" */

//	OYJ_NOBASU,	OYJ_VOLUME_S,		/* "�@" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,		/* "�B" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,		/* "�D" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,		/* "�F" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,		/* "�H" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,		/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,		/* "��" */
//	OYJ_NOBASU,	OYJ_VOLUME_S,		/* "��" */

	SE_A,		1*256,		/* "�K" */
	SE_I,		1*256,		/* "�M" */
	SE_U,		1*256,		/* "�O" */
	SE_E,		1*256,		/* "�Q" */
	SE_O,		1*256,		/* "�S" */
	SE_A,		1*256,		/* "�U" */
	SE_I,		1*256,		/* "�W" */
	SE_U,		1*256,		/* "�Y" */
	SE_E,		1*256,		/* "�[" */
	SE_O,		1*256,		/* "�]" */
	SE_A,		1*256,		/* "�_" */
	SE_I,		1*256,		/* "�a" */
	SE_U,		1*256,		/* "�d" */
	SE_E,		1*256,		/* "�f" */
	SE_O,		1*256,		/* "�h" */
	SE_A,		1*256,		/* "�o" */
	SE_I,		1*256,		/* "�r" */
	SE_U,		1*256,		/* "�u" */
	SE_E,		1*256,		/* "�x" */
	SE_O,		1*256,		/* "�{" */
	SE_A,		1*256,		/* "�p" */
	SE_I,		1*256,		/* "�s" */
	SE_U,		1*256,		/* "�v" */
	SE_E,		1*256,		/* "�y" */
	SE_O,		1*256,		/* "�|" */
	OYJ_KILL,	1*256,		/* "�b" */
	SE_E,		1*256,		/* "�O" */
	SE_I,		1*256,		/* "�P" */
	SE_I,		1*256,		/* "�Q" */
	SE_A,		1*256,		/* "�R" */
	SE_I,		1*256,		/* "�S" */
	SE_O,		1*256,		/* "�T" */
	SE_O,		1*256,		/* "�U" */
	SE_A,		1*256,		/* "�V" */
	SE_A,		1*256,		/* "�W" */
	SE_U,		1*256,		/* "�X" */
	OYJ_KILL,	1*256,		/* "�I" */
	OYJ_KILL,	1*256,		/* "�H" */
	OYJ_KILL,	1*256,		/* "�B" */
	OYJ_NOBASU,	1*256,		/* "�[" */
	OYJ_KILL,	1*256,		/* "�E" */
	0,			1*256,		/* "�c" */
	OYJ_KILL,	1*256,		/* "�w" */
	OYJ_KILL,	1*256,		/* "�x" */
	OYJ_KILL,	1*256,		/* "�u" */
	OYJ_KILL,	1*256,		/* "�v" */
	OYJ_KILL,	1*256,		/* "��" */
	OYJ_KILL,	1*256,		/* "��" */
	SE_A,		1*256,		/* "�~" */
	OYJ_KILL,	1*256,		/* "�D" */
	OYJ_KILL,	1*256,		/* "�~" */
	OYJ_KILL,	1*256,		/* "�^" */
	SE_E,		1*256,		/* "�`" */
	SE_I,		1*256,		/* "�a" */
	SE_I,		1*256,		/* "�b" */
	SE_I,		1*256,		/* "�c" */
	SE_I,		1*256,		/* "�d" */
	SE_E,		1*256,		/* "�e" */
	SE_I,		1*256,		/* "�f" */
	SE_E,		1*256,		/* "�g" */
	SE_A,		1*256,		/* "�h" */
	SE_E,		1*256,		/* "�i" */
	SE_E,		1*256,		/* "�j" */
	SE_E,		1*256,		/* "�k" */
	SE_E,		1*256,		/* "�l" */
	SE_E,		1*256,		/* "�m" */
	SE_O,		1*256,		/* "�n" */
	SE_I,		1*256,		/* "�o" */
	SE_U,		1*256,		/* "�p" */
	SE_A,		1*256,		/* "�q" */
	SE_E,		1*256,		/* "�r" */
	SE_I,		1*256,		/* "�s" */
	SE_U,		1*256,		/* "�t" */
	SE_U,		1*256,		/* "�u" */
	SE_A,		1*256,		/* "�v" */
	SE_E,		1*256,		/* "�w" */
	SE_A,		1*256,		/* "�x" */
	SE_E,		1*256,		/* "�y" */
	SE_E,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_O,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_I,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_U,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	SE_A,		1*256,		/* "��" */
	SE_E,		1*256,		/* "��" */
	OYJ_KILL,	1*256,		/*�J�[�\�� */
};

void SongOyajiInit( s16* work, u16 tango, u16 melody )
{
	work[ OYJ_COUNT ] = 0;
	work[ OYJ_TANGO ] = tango-1;
	work[ OYJ_MELODY ] = melody;
	work[ OYJ_FLAG ] = 0;
}

void SongOyajiPlay( s16* work, u8 code )
{
	s16	se_no;

	work[ OYJ_VOLUME ] = SongOyajiVoiceData[ code*2+1 ];

	se_no = SongOyajiVoiceData[ code*2 ];

	switch( se_no ){
		case OYJ_KILL:
			m4aMPlayStop( &m4a_mplay001 );
			work[ OYJ_FLAG ]++;
			break;
		case OYJ_NOBASU:
		case 0:
			work[ OYJ_FLAG ]++;
			break;
		default:
			m4aSongNumStart( se_no );
			break;
	};
}

void SongOyajiPicthChange( s16* work )
{
	s16*	oyaji_addr;

	if( work[ OYJ_FLAG ] ){
		work[ OYJ_FLAG ] = 0;
	}else{
		m4aMPlayVolumeControl( &m4a_mplay001, 0xffff, (u16)work[ OYJ_VOLUME ] );
		oyaji_addr = (s16*)SongOyajiData[ work[OYJ_TANGO] * 5 + work[OYJ_MELODY] ];
		m4aMPlayPitchControl( &m4a_mplay001, 0xffff, oyaji_addr[ work[OYJ_COUNT] ] );
	}
	work[ OYJ_COUNT ]++;
}

