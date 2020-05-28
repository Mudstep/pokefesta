//=========================================================================
//
//	pm_str.c
//
//	�|�P�����`�f�a�p���b�Z�[�W������֘A
//
//=========================================================================


#include "common.h"
#include "print.h"
#include "fnt_tenji.h"
#include "message.h"

#define PM_STR_H_GLOBAL
#include "pm_str.h"

//#define PM_VERSION	VERSION_SAPPHIRE


//=========================================================================
//
//=========================================================================
extern const u8 * GetIndirectString( int i );

u8 StrTempBuffer0[STR_TEMP_BUF_LEN] ={};
u8 StrTempBuffer1[STR_TEMP_BUF_LEN] ={};
u8 StrTempBuffer2[STR_TEMP_BUF_LEN] ={};

u8 MsgExpandBuffer[MSG_EXPAND_BUF_LEN] ={};

//=========================================================================
//
//=========================================================================

u8 NumBuffer0[10];


static const u8 n_ch[] = {n0_,n1_,n2_,n3_,n4_,n5_,n6_,n7_,n8_,n9_,A__,B__,C__,D__,E__,F__};

static const int n_max[] = 
{
	1,
	10,
	100,
	1000,
	10000,
	100000,
	1000000,
	10000000,
	100000000,
	1000000000,
};

//=========================================================================
//
//=========================================================================

//u8 * PM_strncpy(u8 * to_str, const u8 * from_str, u16 n);
//u8 * PM_strncat(u8 * to_str, const u8 * from_str, u16 n);
//u8 * PM_StrToDecimal(int num, u8 * buf, u16 sw);

//=========================================================================
//
//=========================================================================

//-------------------------------------------------------------------------
//	�|�P�������̃o�b�t�@�ԃR�s�[
//	�����ʃo�[�W�����ɂ���čő啶�����������Ⴄ
//	<����>	to_str		�R�s�[��o�b�t�@
//			from_str	�R�s�[���o�b�t�@
//	<�߂�l>
//			�R�s�[��o�b�t�@��EOM_���������񂾃A�h���X
//-------------------------------------------------------------------------
u8 * NickNameCopy( u8 * to_str, const u8 * from_str )
{
	u8 i,max;
#if	( PM_LANG== LANG_JAPAN )
	max = 5;
#else
	max = MONS_NAME_SIZE;
#endif
	for ( i=0; i<max; i++ ) {
		to_str[i] = from_str[i];
		if ( from_str[i] == EOM_ ) return to_str + i;
	}
	to_str[i] = EOM_;
	return to_str + i;
}

//-------------------------------------------------------------------------
//	�|�P�������̏I�[�`�F�b�N����
//	�����{�ł̏ꍇ�A6�����ȏ゠��ꍇ��EOM_�ō폜����
//	<����>	s		���O�������Ă���o�b�t�@
//	<�߂�l>		EOM_�A�h���X
//-------------------------------------------------------------------------
u8 * NickNameCutOff( u8 * s )
{
	u8 i,max;
#if	( PM_LANG== LANG_JAPAN )
	max = 5;
#else
	max = MONS_NAME_SIZE;
#endif
	for ( i=0; i<max; i++ ) {
		if ( s[i] == EOM_ ) return s + i;
	}
	s[i] = EOM_;
	return s + i;
}

//-------------------------------------------------------------------------
//	�e���̃R�s�[
//	����������Ă��ő啶���������͓���
//	<����>	to_str		�R�s�[��o�b�t�@
//			from_str	�R�s�[���o�b�t�@
//	<�߂�l>
//			�R�s�[��o�b�t�@��EOM_���������񂾃A�h���X
//-------------------------------------------------------------------------
u8 * OyaNameCopy( u8 * to_str, const u8 * from_str )
{
	int i;
	for ( i=0; i<PERSON_NAME_SIZE+EOM_SIZE; i++ ) {
		to_str[i] = from_str[i];
		if ( to_str[i] == EOM_ ) {
			return to_str + i;
		}
	}
	to_str[i] = EOM_;
	return to_str + i;
}

//-------------------------------------------------------------------------
// PM_strcpy
// ������R�s�[
//
//	<���̓p�����[�^>
// 	to_str		�R�s�[��o�b�t�@
// 	from_str	������f�[�^
//
// 	<�Ԓl>
// 	�o�b�t�@�ɃR�s�[�����f�[�^�Ō��EOM_�ւ̃|�C���^��Ԃ�
// 	�f�[�^���Ȃ�(������f�[�^�擪��EOM_������)�ꍇ�A
// 	EOM_���o�b�t�@�擪�ɏ������݁A�����ւ̃|�C���^��Ԃ�
//-------------------------------------------------------------------------
u8 * PM_strcpy(u8 * to_str, const u8 * from_str)
{
		while(*from_str != EOM_)
		{
				*to_str = * from_str;
				to_str++; from_str++;
		};
		*to_str = EOM_;
		return to_str;
};

//-------------------------------------------------------------------------
// PM_strcat
// ������A��
//
//	<���̓p�����[�^>
//	to_str		�A�������f�[�^�iram��ɂ��邱�ƁI�I�j
//	from_str	�A������f�[�^
//
// 	<�Ԓl>
//	�A����̃o�b�t�@��̃f�[�^����EOM_�ւ̃|�C���^��Ԃ�
//-------------------------------------------------------------------------
u8 * PM_strcat(u8 * to_str, const u8 * from_str)
{
		while(*to_str != EOM_)to_str++;
		return PM_strcpy(to_str, from_str);
};


//-------------------------------------------------------------------------
// PM_strncpy
// ������R�s�[		2002/05/15 by nakahiro
//
//	<���̓p�����[�^>
// 	to_str		�R�s�[��o�b�t�@
// 	from_str	������f�[�^
//	len			�R�s�[�o�C�g��
//
// 	<�Ԓl>
//	�u�R�s�[��o�b�t�@ + �R�s�[�o�C�g���v�ւ̃|�C���^
//-------------------------------------------------------------------------
u8 * PM_strncpy( u8 * to_str, const u8 * from_str, u8 len )
{
	u16	i;

	for( i=0; i<len; i++ ){
		to_str[i] = from_str[i];
	}
	return &to_str[len];
};

//-------------------------------------------------------------------------
// PM_strncat
// ������A��		2002/05/15 by nakahiro
//
//	<���̓p�����[�^>
//	to_str		�A�������f�[�^�iram��ɂ��邱�ƁI�I�j
//	from_str	�A������f�[�^
//	len			�A������o�C�g��
//
// 	<�Ԓl>
//	�A����̃o�b�t�@��̃f�[�^���ւ̃|�C���^��Ԃ�
//-------------------------------------------------------------------------
u8 * PM_strncat( u8 * to_str, const u8 * from_str, u8 len )
{
		while(*to_str != EOM_)to_str++;
		return PM_strncpy( to_str, from_str, len );
};


//-------------------------------------------------------------------------
// PM_strlen
// �����񒷎擾
//
//	<���̓p�����[�^>
//	s		�Ώە�����ւ̃|�C���^
//
//	<�Ԓl>
//	�Ώە�����̒����iEOM_�܂ł̕������j��Ԃ�
//-------------------------------------------------------------------------
u16 PM_strlen(const u8 * s)
{
		u16 len = 0;
		while(s[len] != EOM_)
		{
				len++;
		};
		return len;
};

//-------------------------------------------------------------------------
//	�������r
//	���b�W���֐��ƈႢ�����̔�r�͂��Ă��Ȃ�
//
//	<���̓p�����[�^>
//	s,t		�Ώە�����ւ̃|�C���^
//
//	<�Ԓl>
//	��v�������O�A����ȊO�̏ꍇ��O�̒l��Ԃ�
//-------------------------------------------------------------------------
int PM_strcmp(const u8 *s, const u8 *t)
{
	while ( *s == *t ) {
		if ( *s == EOM_ ) return 0;
		s++;t++;
	}
	return *s - *t;
}

//-------------------------------------------------------------------------
//	�������r
//	���b�W���֐��ƈႢ�����̔�r�͂��Ă��Ȃ�
//
//	<���̓p�����[�^>
//	s,t		�Ώە�����ւ̃|�C���^
//	n		��r����ő�̕�����
//
//	<�Ԓl>
//	��v�������O�A����ȊO�̏ꍇ��O�̒l��Ԃ�
//-------------------------------------------------------------------------
int PM_strncmp(const u8 *s, const u8 *t, int n )
{
	while ( *s == *t ) {
		if ( *s == EOM_ ) return 0;
		s++;t++;
		n --;
		if ( n == 0 ) return 0;
	}
	return *s - *t;
}

//-------------------------------------------------------------------------
// PM_NumMsgSet
// ���l��������ϊ�
//
//	<���̓p�����[�^>
//	buf		��������������ރo�b�t�@�̃|�C���^
//	num		������ɕϊ����������l
//	type	�`���w��
//			== NUM_MODE_LEFT		 ���l
//			== NUM_MODE_SPACE		 �E�l�A����Ȃ������͋�
//			== NUM_MODE_ZERO		 �E�l�A����Ȃ������͐����̃[��
//	keta	�\������
//
//	<�Ԓl>
//	���l�̌��ɏ�������EOM_�ւ̃|�C���^��Ԃ�
//-------------------------------------------------------------------------

#if 1
#define NUM_STR(n)	( n>=10 ? hate_ : n_ch[n] )
#define HEX_STR(n)	( n>=16 ? hate_ : n_ch[n] )
#else
u8 NUM_STR(u8 n)
{
		if(n >= 10)return hate_;
		return n_ch[n];
};
#endif

enum{
		STAT_NO_VAL,
		STAT_NEED_NUMBER,
		STAT_NEED_SPACE,
};


u8 * PM_NumMsgSet(u8 * buf, int num, NUM_MODE_TYPE type, u8 keta)
{
	int	i,max;
	u8 flag;
	u16	num_i;
	int num_f;

	max = n_max[keta-1];

	flag = STAT_NO_VAL;
	if(type == NUM_MODE_SPACE) { flag = STAT_NEED_SPACE ; }
	if(type == NUM_MODE_ZERO) { flag = STAT_NEED_NUMBER ; }

	for(i=max;i>0;i/=10){
		num_i=num/i;
		num_f = num - ( i * num_i );
	//	num_f=num%i;
		if(flag == STAT_NEED_NUMBER)
		{
				*(buf++) = NUM_STR(num_i);
		}
		else if( num_i != 0 || i==1)
		{
				flag = STAT_NEED_NUMBER;
				*(buf++) = NUM_STR(num_i);
		}
		else if(flag == STAT_NEED_SPACE)
		{
				*(buf++) = spc_;
		};
		num=num_f;
	//	num -= i * num_i;
	}
	*buf = EOM_;
	return buf;
};

u8 * PM_HexNumMsgSet(u8 * buf, int num, NUM_MODE_TYPE type, u8 keta)
{
	int	i,max;
	u8 flag;
	u32	num_i,num_f;
	u8 n;

	max = 1;
	for(n=1;n<keta;n++) { max *= 16; }

	flag = STAT_NO_VAL;
	if(type == NUM_MODE_SPACE) { flag = STAT_NEED_SPACE ; }
	if(type == NUM_MODE_ZERO) { flag = STAT_NEED_NUMBER ; }

	for(i=max;i>0;i/=16){
		num_i=num/i;
		num_f=num%i;
		if(flag == STAT_NEED_NUMBER)
		{
				*(buf++) = HEX_STR(num_i);
		}
		else if( num_i != 0 || i==1)
		{
				flag = STAT_NEED_NUMBER;
				*(buf++) = HEX_STR(num_i);
		}
		else if(flag == STAT_NEED_SPACE)
		{
				*(buf++) = spc_;
		};
		num=num_f;
	//	num -= i * num_i;
	}
	*buf = EOM_;
	return buf;
};




//=================================================================
// PM_MsgExpand
// ���b�Z�[�W�W�J���[�`��
// 	I_MSG_�ŎQ�Ƃ���镔�����b�Z�[�W���܂񂾃��b�Z�[�W��W�J����
//
//	<���̓p�����[�^>
//	buf		���b�Z�[�W��W�J����o�b�t�@�ւ̃|�C���^
//	data	�W�J���錳�f�[�^�ւ̃|�C���^
//
//	<�Ԓl>
//	�W�J��̃��b�Z�[�W�f�[�^����EOM_�ւ̃|�C���^��Ԃ�
//=================================================================

u8 * PM_MsgExpand(u8 * buf, const u8 * data)
{
	u8 c = 0;

	while ( TRUE ) {

		c = *data;
		data++;
		switch ( c ) {
		case EOM_:
			*buf = EOM_;
			return buf;

		case I_MSG_:
			buf = PM_MsgExpand( buf, GetIndirectString(*(data++)) );
			break;

		case CTRL_:
			*(buf++) = c;
			c = *data; data++;
			*(buf++) = c;
			switch ( c ) {
			/*�p�����[�^�Ȃ�*/
			case C_FNML_:
			case C_TRGWAIT_:
			case C_CLR_:
				break;
			/*�p�����[�^3��*/
			case C_ACOL_:
				*(buf++) = *(data++);
				*(buf++) = *(data++);
				*(buf++) = *(data++);
				break;
			/*�ʏ�̓p�����[�^1��*/
			default:
				*(buf++) = *(data++);
				break;
			}
			break;

		case CR_:
		case SCROLL_WAIT_:
		case NORMAL_WAIT_:
		default:
			*buf = c;
			buf++;

		}
	}
	return NULL;
}

//=================================================================
//	�_���f�[�^�W�J���[�`��
//		���_���f�[�^�Ƀw�b�_��t���A�ꕶ�����Ԃ��󂯂�
//	<����>	buf		�W�J��o�b�t�@
//			data	�ǂݍ��݃f�[�^
//	<�߂�l>		�Ō��EOM_���������񂾃o�b�t�@�̃A�h���X
//
//	�_��1������2X2�L�����ō\�������B
//	���̃��[�`���Œʏ�̓_��1�����͍������{�E�����ɃR���o�[�g�����
//	�������ƉE������8X16�T�C�Y�����Ƃ��ēo�^����Ă���B
//
//=================================================================
u8 * PM_TenjiExpand( u8 * buf, const u8 * data )
{
	u8 c;
	const u8 header[] = { CTRL_,C_FCHG_,FONTTENJITYPE,EOM_ };
	const u8 cr_line[] = { CR_,CTRL_,C_YCHG_,2,EOM_ };

	buf = PM_strcpy( buf, header );
	c = 0;

	while (TRUE) {

		c = *data;	data++;

		switch (c) {

		case EOM_:
			*buf = c;
			return buf;

		case CR_:
			buf = PM_strcpy( buf, cr_line );
			break;

		default:
			*buf = c;			//������
			buf ++;
			*buf = c + 0x40;	//�E�����A�_���͑S����64==0x40���
			buf ++;
			break;
		}
	}
}


//=========================================================================
//
// �Q�ƃo�b�t�@�Ăяo��
//
//	��I_MSG_�R�[�h�̓W�J�p
//=========================================================================

//-------------------------------------------------------------------------
//	������擾�p���[�`��
//-------------------------------------------------------------------------
static const u8 * Get_NumBuffer(void)
{
	return NumBuffer0;
}

static const u8 * Get_my_name(void)
{
	return MyData.my_name;
}

static const u8 * Get_StrTemp0(void)
{
	return StrTempBuffer0;
}

static const u8 * Get_StrTemp1(void)
{
	return StrTempBuffer1;
}

static const u8 * Get_StrTemp2(void)
{
	return StrTempBuffer2;
}

static const u8 * Get_MyNameSuffix(void)
{
	if ( MyData.my_sex == MALE ) {
		return kun_string;
	} else {
		return chan_string;
	};
}

static const u8 * Get_SupportCharacterName( void )
{
	if ( MyData.my_sex == MALE ) {
		return str_support_female;
	} else {
		return str_support_male;
	}
}

static const u8 * Get_VersionString( void )
{
#if ( PM_VERSION == VERSION_SAPPHIRE )
	return str_ver_sapphire;
#elif ( PM_VERSION == VERSION_RUBY )
	return str_ver_ruby;
#endif
}

static const u8 * Get_EnemyGroupName( void )
{
#if ( PM_VERSION == VERSION_SAPPHIRE )
	return str_aqua;
#elif ( PM_VERSION == VERSION_RUBY )
	return str_maguma;
#endif
}

static const u8 * Get_FriendlyGroupName( void )
{
#if ( PM_VERSION == VERSION_SAPPHIRE )
	return str_maguma;
#elif ( PM_VERSION == VERSION_RUBY )
	return str_aqua;
#endif
}

static const u8 * Get_EnemyLeaderName( void )
{
#if ( PM_VERSION == VERSION_SAPPHIRE )
	return str_aogiri;
#elif ( PM_VERSION == VERSION_RUBY )
	return str_matubusa;
#endif
}

static const u8 * Get_FriendlyLeaderName( void )
{
#if ( PM_VERSION == VERSION_SAPPHIRE )
	return str_matubusa;
#elif ( PM_VERSION == VERSION_RUBY )
	return str_aogiri;
#endif
}

static const u8 * Get_MainKeyPokemonName( void )
{
#if ( PM_VERSION == VERSION_SAPPHIRE )
	return str_kaiouga;
#elif ( PM_VERSION == VERSION_RUBY )
	return str_guradon;
#endif
}

static const u8 * Get_SubKeyPokemonName( void )
{
#if ( PM_VERSION == VERSION_SAPPHIRE )
	return str_guradon;
#elif ( PM_VERSION == VERSION_RUBY )
	return str_kaiouga;
#endif
}


// �o�b�t�@��`�e�[�u��
#include "i_msg.tbl"


//-------------------------------------------------------------------------
//	������擾�֐�
//-------------------------------------------------------------------------
const u8 * GetIndirectString( int i )
{
	if ( i >= NELEMS(GetStrFuncList) ) return dummy_string;
	return (GetStrFuncList[i])();
}



/*======================================================================
==	������o�b�t�@���w�蕶���R�[�h�ŃN���A
==	to     ������o�b�t�@
==	code   �����R�[�h
==	len    �N���A���镶����
==
==	return : EOM_ �A�h���X                                       TAYA
======================================================================*/
u8* PM_strclear(u8 *to, u8 code, u16 len)
{
	u16 i;
	for(i = 0; i < len; i++){
		*to++ = code;
	}
	*to = EOM_;
	return to;
}
/*======================================================================
==	������R�s�[��A�w�蕶�����ɖ����Ȃ��������A�w�蕶���R�[�h�ŃN���A
==	to     �R�s�[�敶����o�b�t�@
==	from   �R�s�[��������
==	code   �N���A�����R�[�h
==	len    �N���A���镶����
==
==	return : EOM_ �A�h���X                                        TAYA
======================================================================*/
u8* PM_strcpy_clear(u8 *to, const u8 *from, u8 code, u16 len)
{
	while(*from != EOM_){
		*to++ = *from++;
		if(len){ len--; }
	}
	while(len--){
		*to++ = code;
	}
	*to = EOM_;
	return to;
}
/*======================================================================
==	������o�b�t�@��EOM_�R�[�h�ŃN���A
==	to     ������o�b�t�@
==	len    �N���A���镶����
==
==	return : �N���A�I�[��EOM_ �A�h���X
======================================================================*/
u8* PM_strclearEOM_(u8 *to, u16 len)
{
	return PM_strclear(to, EOM_, len);
}

