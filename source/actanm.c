//**********************************************************************
//		act_anm.c
//		�������, ���Ȱ�ެ, �̨��Ȱ�ެ, ��گ��Ȱ�ެ,�@OAM �߯�
//**********************************************************************
#include	<agb.h>
#include 	"common.h"
#include	"actor.h"
#include	"palanm.h"

#include  "cell_bitchk.h"

//======================================================================
//		��۰��ٕϐ�
//======================================================================
u8 ObjPalManStart;									// ��گ��Ȱ�ެ
u8 AffAnmStopFlag = 0;								// �̨ݱ�җL�������׸�
u32 AffineWorkUse;							// �̨��Ȱ�ެ

//======================================================================
//		�ϐ�
//======================================================================
static u16 cell_act_num[ACT_MAX];					// ���Ȱ�ެ
static u16 cell_char_num[ACT_MAX][2];				// ���Ȱ�ެ
static ObjAffinePatWork OBJAffinePatWork[32];		// �g�k�߯�ܰ�
static u16 ObjPalManWork[16];						// ��޼ު��گ��Ȱ�ެ

//----------------------------------------------------------------------
//		�O���Q�Ɗ֐�, �ϐ�
//----------------------------------------------------------------------
extern ObjAffineDestData OBJ_AffineWork[32];		// common.c
extern s16 TransAreaFlagChk( u16 );					// oam_act.c
extern u8 TransBitChkSet( u16, u8 );				// oam_act.c
extern void	dmaTransSet( u16, u16, const actAnmData * ); // oam_act.c
extern void	affineWorkinit( void );
extern void affineWorkSet( u8,u16,u16,u16,u16);

//======================================================================
//		�������
//======================================================================
static	void	anm_sw_off( actWork * );
static	void	anm_sw_on( actWork * );
static	void	affine_anm_set( actWork *act );
static	void	affine_anm( actWork *act );
static	void 	affineWorkDestSet( u8, ObjAffineDestData * );
static	u8		act_affine_no_get( actWork *act );
static 	void 	OBJAffinePatWorkInit( u8 );
static 	void 	OBJAffinePatWorkInitAnm( u8 );
static	void	OBJAffinePatWorkInitSet( u8, u8 );
static	void	OBJAffinePatWorkCalc( u8, actAffAnm * );
static	void	act_hv_flip_set( actWork *, u8, u8 );
static	void	anm_wait_check_del( actWork * );
static	u8		aff_wait_check_del( actWork *, u8 );
static	void	affanm_get( u8, actWork *, actAffAnm * );
static	void	affanm_calc( u8, actAffAnm * );

//------------------------------------------
//	ð���
//------------------------------------------
static void (* const anm_sw_tbl[])(actWork *) =
{
	anm_sw_off,
	anm_sw_on
};
			
static void (* const affanm_sw_tbl[])(actWork *) =
{
	affine_anm,
	affine_anm_set
};

//------------------------------------------
//	��� Ҳ�
//------------------------------------------
#if 1													// changed by matsuda 2002.09.01(��)
void ActAnm( actWork *act )								// Ҳ�
{
	anm_sw_tbl[act->anm_sw]( act );						// �ٱ��

	if( AffAnmStopFlag == 0 )
		affanm_sw_tbl[act->affine_sw]( act );			// �g�k���
}
#else
void ActAnm( actWork *act )								// Ҳ�
{
	anm_sw_tbl[act->anm_sw]( act );						// �ٱ��
	affanm_sw_tbl[act->affine_sw]( act );				// �g�k���
}
#endif

//------------------------------------------
//	��Ҿ�� �J�n
//------------------------------------------
static void anm_sw_on( actWork *act )
{
	s16 Pat;
	u8  Wait, Hrev, Vrev;
	
	act->anm_offs = 0;									// �Q�Ƶ̾��
	act->anmend_sw = 0;									// ��ҏI���׸�
	act->anm_count = 0;									// ٰ�߶���
	
	Pat = act->anm_tbl[act->anm_no][act->anm_offs].pat;	// ������擾
	
	if( Pat == ACT_ANMEND )								// ��а
	{
		return;
	}
	
	act->anm_sw = 0;									// ��ҊJ�n�׸޸ر

	Wait = act->anm_tbl[act->anm_no][act->anm_offs].wait;
	Hrev = act->anm_tbl[act->anm_no][act->anm_offs].hrev;
	Vrev = act->anm_tbl[act->anm_no][act->anm_offs].vrev;

	if( Wait )											// ���ĕ␳
	{
		Wait--;
	}
	
	act->anm_wait = Wait;								// ���Ĕ��f

	if( (act->oamData.AffineMode & 0x01) == 0 )
	{
		act_hv_flip_set( act, Hrev, Vrev );				// �د�ߔ��f
	}
	
	if( act->t_mode )									// ��]�����
	{
		act->oamData.CharNo = (u16)Pat + act->char_num;
	}
	else												// �]���^���
	{
		dmaTransSet( Pat, act->oamData.CharNo, act->anm_trans );
	}
}

//------------------------------------------
//	��� 2��ڈȍ~
//------------------------------------------
static void	pat_com_anmend( actWork * );
static void	pat_com_anmloop( actWork * );
static void	pat_com_anmcloop( actWork * );
static void	pat_com_nothing( actWork * );

static void (* const anmpat_command_tbl[])(actWork *) =
{
	pat_com_anmcloop,									// 0 ANMcLOOP(-3)
	pat_com_anmloop,									// 1 ANMLOOP(-2)
	pat_com_anmend,										// 2 ANMEND(-1)
	pat_com_nothing										// 3 �ʏ�
};

static void	anm_sw_off( actWork *act )
{
	s16 Pat, tbl_no;
	u8  Hrev, Vrev;

	if( act->anm_wait )									// ���ėL��
	{
		anm_wait_check_del( act );						// ���Č���

		Hrev = (act->anm_tbl)[act->anm_no][act->anm_offs].hrev;
		Vrev = (act->anm_tbl)[act->anm_no][act->anm_offs].vrev;
		
		if( (act->oamData.AffineMode & 0x01) == 0 )		// �g�k��
		{
			act_hv_flip_set( act, Hrev, Vrev );			// �د�ߔ��f
		}
	}
	else												// ���� 0
	{
		if( act->anm_pause )							// �߰��
		{
			return;
		}

		(act->anm_offs)++;

		Pat = (act->anm_tbl)[act->anm_no][act->anm_offs].pat;
		
		tbl_no = 3;

		if( Pat	< 0 )									// �߯�ϲŽ�� �����
		{
			tbl_no = Pat + 3;							// END = -1, LOOP = -2, cLOOP = -3;
		}
		
		anmpat_command_tbl[tbl_no]( act );				// ����ޕʾ��
	}
}

//------------------------------------------
//	��� ����ޖ���
//------------------------------------------
static void	pat_com_nothing( actWork *act )
{
	s16 Pat;
	u8  Wait, Hrev, Vrev;

	Pat = (act->anm_tbl)[act->anm_no][act->anm_offs].pat;
	Wait = (act->anm_tbl)[act->anm_no][act->anm_offs].wait;
	Hrev = (act->anm_tbl)[act->anm_no][act->anm_offs].hrev;
	Vrev = (act->anm_tbl)[act->anm_no][act->anm_offs].vrev;
	
	if( Wait )
	{
		Wait--;											// ���ĕ␳
	}
	
	act->anm_wait = Wait;

	if( (act->oamData.AffineMode & 0x01) == 0 )
	{
		act_hv_flip_set( act, Hrev, Vrev );				// �د�ߔ��f
	}

	if( act->t_mode )
	{
		act->oamData.CharNo = (u16)Pat + act->char_num;
	}
	else
	{
		dmaTransSet( Pat, act->oamData.CharNo, act->anm_trans );
	}
}

//------------------------------------------
//	ACT_ANMEND
//------------------------------------------
static void	pat_com_anmend( actWork *act )
{
	(act->anm_offs)--;									// �Q�ƈʒu
	act->anmend_sw = 1;									// ��ҏI���׸�(1:�I��)
}

//------------------------------------------
//	ACT_ANMLOOP
//------------------------------------------
static void	pat_com_anmloop( actWork *act )
{
	s16 Pat;
	u8  Wait, Hrev, Vrev;
	
	act->anm_offs =										// LOOP �� wait �ɖ߂��
		(act->anm_tbl)[act->anm_no][act->anm_offs].wait;

	Pat = (act->anm_tbl)[act->anm_no][act->anm_offs].pat;
	Wait = (act->anm_tbl)[act->anm_no][act->anm_offs].wait;
	Hrev = (act->anm_tbl)[act->anm_no][act->anm_offs].hrev;
	Vrev = (act->anm_tbl)[act->anm_no][act->anm_offs].vrev;
				
	if( Wait )
	{
		Wait--;											// ���ĕ␳
	}
	
	act->anm_wait = Wait;

	if( (act->oamData.AffineMode & 0x01) == 0 )
	{
		act_hv_flip_set( act, Hrev, Vrev );				// �د�ߔ��f
	}

	if( act->t_mode )
	{
		act->oamData.CharNo = (u16)Pat + act->char_num;
	}
	else
	{
		dmaTransSet( Pat, act->oamData.CharNo, act->anm_trans );
	}
}

//------------------------------------------
//	ACT_ANMcLOOP
//------------------------------------------
static void	anm_count_set( actWork * );
static void	anm_count_dec( actWork * );
static void	anm_count_back( actWork * );

static void	pat_com_anmcloop( actWork *act )
{
	if( act->anm_count )
	{
		anm_count_dec( act );							// ٰ��
	}
	else
	{
		anm_count_set( act );							// ٰ�ߊJ�n
	}
}

//------------------------------------------
//	ACT_ANMcLOOP	�J�n
//------------------------------------------
static void	anm_count_set( actWork *act )
{
	act->anm_count =									// wait �ʒu�� ٰ�߉�
		(act->anm_tbl)[act->anm_no][act->anm_offs].wait;

	anm_count_back( act );								// �C��
	anm_sw_off( act );									// �ċA
}

//------------------------------------------
//	ACT_ANMcLOOP	ٰ��
//------------------------------------------
static void	anm_count_dec( actWork *act )
{
	act->anm_count--;									// ٰ�߉�--
	anm_count_back( act );								// �C��
	anm_sw_off( act );									// �ċA
}

//------------------------------------------
//	ACT_ANMcLOOP	�ʒu�C��
//------------------------------------------
static void anm_count_back( actWork *act )
{
	if( act->anm_count )								// 0 = �ʉ� or ���َw��
	{
		for( act->anm_offs--;
			 (act->anm_tbl)[act->anm_no][act->anm_offs - 1].pat != ACT_ANMcLOOP &&
			 act->anm_offs > 0; act->anm_offs-- );
		act->anm_offs--;
	}
}

//------------------------------------------
//	�g�k��ҊJ�n
//------------------------------------------
static void	affine_anm_set( actWork *act )
{
	u8	aff_no;
	actAffAnm aff;

	if( (act->oamData.AffineMode & 0x01) == 0 ||
		act->aff_tbl[0][0].rx == ACT_AFEND )			// ��а
	{
		return;
	}
	
	aff_no = act_affine_no_get( act );					// ܰ����ގ擾
	OBJAffinePatWorkInitAnm( aff_no );					// ܰ�������
	affanm_get( aff_no, act, &aff );					// ����ް��擾

	act->affine_sw = 0;									// �����ر
	act->affend_sw = 0;									// ��ҏI���׸޸ر
	affanm_calc( aff_no, &aff );
	OBJAffinePatWork[aff_no].wait = aff.wait;
}

//------------------------------------------
//	�g�k��� 2��ڈȍ~
//------------------------------------------
static void	aff_com_cloop( u8, actWork * );
static void	aff_com_loop( u8, actWork * );
static void	aff_com_end( u8, actWork * );
static void	aff_com_nothing( u8, actWork * );
static void	affine_wait_on( u8, actWork * );

static void (* const affine_com_tbl[])(u8, actWork *) =
{
	aff_com_cloop,
	aff_com_loop,
	aff_com_end,
	aff_com_nothing
};

static void	affine_anm( actWork *act )
{
	u8 aff_no;
	s16 pat, tbl_no;

	if( (act->oamData.AffineMode & 0x01) == 0 )			// ��g�k
	{
		return;
	}
	
	aff_no = act_affine_no_get( act );

	if( OBJAffinePatWork[aff_no].wait )					// ���ėL��
	{
		affine_wait_on( aff_no, act );
	}
	else
	{
		if( act->aff_pause )							// �߰��
		{
			return;
		}

		OBJAffinePatWork[aff_no].offs++;				// �̾�đ���
		
		pat =
		(act->aff_tbl)[OBJAffinePatWork[aff_no].no][OBJAffinePatWork[aff_no].offs].rx;
	
		tbl_no = 3;
	
		if( pat >= ACT_AFcLOOP )						// ���������
		{
			tbl_no = pat - ACT_AFcLOOP;
		}
		
		affine_com_tbl[tbl_no]( aff_no, act );
	}
}	

//------------------------------------------
//	���ėL��
//------------------------------------------
static void	affine_wait_on( u8 aff_no, actWork *act )
{
	actAffAnm aff;
	
	if( aff_wait_check_del( act, aff_no ) == 0 )		// ���� & �߰��
	{
		affanm_get( aff_no, act, &aff );
		OBJAffinePatWorkCalc( aff_no, &aff );
	}
}


//------------------------------------------
//	ACT_AFcLOOP
//------------------------------------------
static void	aff_count_set( u8, actWork * );
static void	aff_count_dec( u8, actWork * );
static void	aff_count_back( u8, actWork * );

static void aff_com_cloop( u8 aff_no, actWork *act )
{
	if( OBJAffinePatWork[aff_no].count )
	{
		aff_count_dec( aff_no, act );					// ٰ��
	}
	else
	{
		aff_count_set( aff_no, act );					// �J�n
	}	
}

//------------------------------------------
//	ACT_AFcLOOP		�J�n
//------------------------------------------
static void	aff_count_set( u8 aff_no, actWork *act )
{
	OBJAffinePatWork[aff_no].count = 					// Y �g�k���ɉ�
		(act->aff_tbl)[OBJAffinePatWork[aff_no].no][OBJAffinePatWork[aff_no].offs].ry;
	
	aff_count_back( aff_no, act );
	affine_anm( act );
}

//------------------------------------------
//	ACT_AFcLOOP		ٰ��
//------------------------------------------
static void aff_count_dec( u8 aff_no, actWork *act )
{
	OBJAffinePatWork[aff_no].count--;
	aff_count_back( aff_no, act );						// �C��
	affine_anm( act );									// �ċA
}

//------------------------------------------
//	ACT_AFcLOOP		�ʒu�C��
//------------------------------------------
static void aff_count_back( u8 no, actWork *act )
{
	if( OBJAffinePatWork[no].count )
	{
		for( OBJAffinePatWork[no].offs--;
			 (act->aff_tbl)[OBJAffinePatWork[no].no]
						   [OBJAffinePatWork[no].offs - 1].rx != ACT_AFcLOOP &&
			 OBJAffinePatWork[no].offs > 0; OBJAffinePatWork[no].offs-- );
		OBJAffinePatWork[no].offs--;					// �C��
	}
}

//------------------------------------------
//	ACT_AFLOOP
//------------------------------------------
static void aff_com_loop( u8 aff_no, actWork *act )
{
	actAffAnm aff;
	
	OBJAffinePatWork[aff_no].offs =
		(act->aff_tbl)[OBJAffinePatWork[aff_no].no][OBJAffinePatWork[aff_no].offs].ry;
	
	affanm_get( aff_no, act, &aff );			
	affanm_calc( aff_no, &aff );
	OBJAffinePatWork[aff_no].wait = aff.wait;
}

//------------------------------------------
//	ACT_AFEND
//------------------------------------------
static void aff_com_end( u8 aff_no, actWork *act )
{
	actAffAnm aff = { 0, 0, 0, 0 };

	act->affend_sw = 1;										// �I���׸� ON
	OBJAffinePatWork[aff_no].offs--;						// �߂�
	OBJAffinePatWorkCalc( aff_no, &aff );
}

//------------------------------------------
//	����ޖ���
//------------------------------------------
static void aff_com_nothing( u8 aff_no, actWork *act )
{
	actAffAnm aff;

	affanm_get( aff_no, act, &aff );
	affanm_calc( aff_no, &aff );
	OBJAffinePatWork[aff_no].wait = aff.wait;
}

//------------------------------------------
//	�̨�ܰ����
//------------------------------------------
static void affineWorkDestSet( u8 num, ObjAffineDestData *dest )
{
	OBJ_AffineWork[num].H_DiffX = dest->H_DiffX;
	OBJ_AffineWork[num].V_DiffX = dest->V_DiffX;
	OBJ_AffineWork[num].H_DiffY = dest->H_DiffY;
	OBJ_AffineWork[num].V_DiffY = dest->V_DiffY;
}

//------------------------------------------
//	�̨ݎQ�ƒl�擾
//------------------------------------------
static u8 act_affine_no_get( actWork *act )
{
	u8	no;
	
	no = 0;

	if( (act->oamData.AffineMode & 0x01) )
	{
		no = (u8)act->oamData.AffineParamNo;				// ܰ�����
	}

	return( no );
}

//------------------------------------------
//	HV �د�߾��
//------------------------------------------
static void act_hv_flip_set( actWork *act, u8 Hflip, u8 Vflip )
{
	act->oamData.AffineParamNo &= 0x007;					// ���2�ޯč폜
	act->oamData.AffineParamNo |=
			((Hflip ^ act->Hflip) & 0x01) << 3;				// H�د�ߔ��f
	act->oamData.AffineParamNo |=
			((Vflip ^ act->Vflip) & 0x01) << 4;				// V�د�ߔ��f
}

//------------------------------------------
//	�g�kܰ�������
//------------------------------------------
static void OBJAffinePatWorkInitAnm( u8 num )				// no ����
{
	OBJAffinePatWork[num].offs = 0;
	OBJAffinePatWork[num].wait = 0;
	OBJAffinePatWork[num].count = 0;
}

static void OBJAffinePatWorkInitSet( u8 num, u8 anm_no )	// no set & init
{
	OBJAffinePatWork[num].no = anm_no;
	OBJAffinePatWork[num].offs = 0;
	OBJAffinePatWork[num].wait = 0;
	OBJAffinePatWork[num].count = 0;
	OBJAffinePatWork[num].src.RatioX = 0x0100;
	OBJAffinePatWork[num].src.RatioY = 0x0100;
	OBJAffinePatWork[num].src.Theta = 0;
}

static void OBJAffinePatWorkInit( u8 num )					// all
{
	OBJAffinePatWork[num].no = 0;
	OBJAffinePatWork[num].offs = 0;
	OBJAffinePatWork[num].wait = 0;
	OBJAffinePatWork[num].count = 0;
	OBJAffinePatWork[num].src.RatioX = 0x0100;
	OBJAffinePatWork[num].src.RatioY = 0x0100;
	OBJAffinePatWork[num].src.Theta = 0;
}

static void OBJAffinePatWorkSet( u8 num, actAffAnm *aff )	// set init
{
	OBJAffinePatWork[num].src.RatioX = aff->rx;
	OBJAffinePatWork[num].src.RatioY = aff->ry;
	OBJAffinePatWork[num].src.Theta = aff->theta << 8;
}

//------------------------------------------
//	��Ұ��ݳ��Č��� �����L��
//------------------------------------------
static void anm_wait_check_del( actWork *act )
{
	if( !(act->anm_pause) )									// �߰������
	{
		(act->anm_wait)--;
	}
}

//------------------------------------------
//	�g�k��Ұ��ݳ��Č��� �����L��
//------------------------------------------
static u8 aff_wait_check_del( actWork *act, u8 num )
{
	if( !(act->aff_pause) )
	{
		OBJAffinePatWork[num].wait--;
	}
	
	return( act->aff_pause );
}

//------------------------------------------
//	�g�k -> �̨�ܰ�
//------------------------------------------
static s16 inverse( s16 );

static void OBJAffinePatWorkCalc( u8 num, actAffAnm *aff )
{
	ObjAffineSrcData src;
	ObjAffineDestData dest;

	OBJAffinePatWork[num].src.RatioX += aff->rx;
	OBJAffinePatWork[num].src.RatioY += aff->ry;
	OBJAffinePatWork[num].src.Theta =
		(OBJAffinePatWork[num].src.Theta + (aff->theta << 8)) & 0xff00;
	
	src.RatioX = inverse( OBJAffinePatWork[num].src.RatioX );
	src.RatioY = inverse( OBJAffinePatWork[num].src.RatioY );
	src.Theta = OBJAffinePatWork[num].src.Theta;

	ObjAffineSet( &src, &dest, 1, 2 );
	affineWorkDestSet( num, &dest );
}

static s16 inverse( s16 s )									//  �t��(1/s)
{
    s32 tmp;
#if 1
    tmp = 0x10000;
    tmp /= s;
#else
	tmp = Div( 0x00010000, s );
#endif
	return (s16)tmp;
}

//------------------------------------------
//	�g�k����ް��擾
//------------------------------------------
static void affanm_get( u8 no, actWork *act, actAffAnm *aff )
{
	aff->rx =
	(act->aff_tbl)[OBJAffinePatWork[no].no][OBJAffinePatWork[no].offs].rx;
	aff->ry =
	(act->aff_tbl)[OBJAffinePatWork[no].no][OBJAffinePatWork[no].offs].ry;
	aff->theta =
	(act->aff_tbl)[OBJAffinePatWork[no].no][OBJAffinePatWork[no].offs].theta;
	aff->wait = 
	(act->aff_tbl)[OBJAffinePatWork[no].no][OBJAffinePatWork[no].offs].wait;
}

//------------------------------------------
//	����ް����f
//------------------------------------------
static void affanm_calc( u8 no, actAffAnm *aff )
{
	actAffAnm dmy = { 0, 0, 0, 0 };							// ��а
	
	if( aff->wait )
	{
		aff->wait--;										// ���ĕ␳
		OBJAffinePatWorkCalc( no, aff );
	}
	else													// 0 = �������w��
	{
		OBJAffinePatWorkSet( no, aff );
		OBJAffinePatWorkCalc( no, &dmy );
	}
}

//======================================================================
//		������ҕύX
//======================================================================
//------------------------------------------
//	��Ұ������ޕύX
//  In:	act			; ��Ă��鱸��
// 		anm_num		; �������
//------------------------------------------
void ActAnmChg( actWork *act, u8 anm_num )
{
	act->anm_no = anm_num;
	act->anm_sw = 1;
	act->anmend_sw = 0;
}

//------------------------------------------
//	��Ұ������ޕύX �����L��
//  In:	act			; ��Ă��鱸��
// 		anm_num		; �������
//------------------------------------------
void ActAnmChkChg( actWork *act, u8 anm_num )
{
    if( act->anm_no != anm_num )
	{
		ActAnmChg( act, anm_num );
	}
}

//------------------------------------------
//	��Ұ��ݵ̾�����ޕύX & ���f
//	In:	act			; ��ı��
//		anm_offs	; ��ҵ̾������
//------------------------------------------
void ActAnmOffsChg( actWork *act, u8 anm_offs )
{
	u8	dmy;
	
	dmy = act->anm_pause;
	act->anm_offs = anm_offs - 1;
	act->anm_wait = 0;
	act->anm_sw = 0;
	act->anmend_sw = 0;
	act->anm_pause = 0;
	anm_sw_off( act );

	if( act->anm_wait )
	{
		act->anm_wait++;
	}
	
	act->anm_pause = dmy;
}

//------------------------------------------
//	�g�k��Ұ������ޕύX
//  In:	act			; ��Ă��鱸��
// 		anm_num		; �������
//------------------------------------------
void ActAffAnmChg( actWork *act, u8 anm_num )
{
	u8	no;
	
	no = act_affine_no_get( act );
	OBJAffinePatWorkInitSet( no, anm_num );
	act->affine_sw = 1;
	act->affend_sw = 0;
}

//------------------------------------------
//	�g�k��Ұ������ޕύX �����L��
//  In:	act			; ��Ă��鱸��
// 		anm_num		; �������
//------------------------------------------
void ActAffAnmChkChg( actWork *act, u8 anm_num )
{
	u8	no;

	no = act_affine_no_get( act );
	
	if( OBJAffinePatWork[no].no != anm_num )
	{
		ActAffAnmChg( act, anm_num );
	}
}

//------------------------------------------
//	�g�k��Ұ������ޕύX,�g�k���ێ�
//  In:	act			; ��Ă��鱸��
// 		anm_num		; �������
//------------------------------------------
void ActAffAnmHoldChg( actWork *act, u8 anm_num )
{
	u8	no;
	
	no = act_affine_no_get( act );
	OBJAffinePatWork[no].no = anm_num;
	act->affine_sw = 1;
	act->affend_sw = 0;
}

//------------------------------------------
//	�g�k��Ұ������ޕύX �g�k���ێ� �����L��
//  In:	act			; ��Ă��鱸��
// 		anm_num		; �������
//------------------------------------------
void ActAffAnmHoldChkChg( actWork *act, u8 anm_num )
{
	u8	no;

	no = act_affine_no_get( act );
	
	if( OBJAffinePatWork[no].no != anm_num )
	{
		ActAffAnmHoldChg( act, anm_num );
	}
}

//------------------------------------------
//	������޾��(��]��)
//------------------------------------------
void ActCharNumSet( actWork *act )
{
	s16 pat;
	
	if( act->t_mode )
	{
		pat = act->anm_tbl[act->anm_no][act->anm_offs].pat;
	
		if( pat < 0 )										// �߯�ϲŽ�� �����
		{
			pat = 0;										// ��а��
		}
	
		act->oamData.CharNo = (u16)pat + act->char_num;
	}
}

//======================================================================
//		�̨�ܰ��Ȱ�ެ
//======================================================================
//------------------------------------------
//	������
//------------------------------------------
void AffineWorkManInit( void )
{
	u8	i;

	AffAnmStopFlag = 0;										// �̨ݱ�җL��	matsuda 02.09.01
	AffineWorkUse = 0;										// �g�p�� 0
	affineWorkinit();										// OAM�g�kܰ�������
	for( i = 0; i < 32; OBJAffinePatWorkInit(i), i++ );
}

//------------------------------------------
//	�̨�ܰ��g�p�\���擾,�o�^
//	Out		u8		; �g�p�\���� (0~31)
//					; 0xff = Error
//------------------------------------------
u8 AffineWorkNumGet( void )
{
	u8	i;
	u32	calc, dmy;

	for( i = 0, calc = 1, dmy = AffineWorkUse; i < 32; i++, calc <<= 1 )
	{
		if( (dmy & calc) == 0 )
		{
			AffineWorkUse |= calc;
			return( i );
		}
	}

	return( 0xff );
}

//------------------------------------------
//	�̨�ܰ��g�p���폜
//	In		num		; �폜No
//------------------------------------------
void AffineWorkNumDel( u8 num )
{
	u8	i;
	u32	calc;

	for( i = 0, calc = 1; i < num; i++, calc <<= 1 );

	calc = ~calc;											// calc = not calc
	
	AffineWorkUse &= calc;

	affineWorkSet( num, 0x0100, 0x0000, 0x0000, 0x0100 );
}
	
//------------------------------------------
//	�̨���������
//	In		act		; ��ı���
//------------------------------------------
void AffineNumSet( actWork *act )
{
	u8	aff_no;
	
	aff_no = AffineWorkNumGet();							// ܰ�����
	
	if( aff_no == 0xff )									// �o�^�s��
	{
		return;
	}
	
	actCenterCalc( act, act->oamData.Shape,					// ���S�␳
			act->oamData.Size, act->oamData.AffineMode );

	act->oamData.AffineParamNo = aff_no;
	act->affine_sw = 1;
	OBJAffinePatWorkInit( aff_no );							// �����������
}

//------------------------------------------
//	OBJ_AffineWork�ւ̊g�k�����
//	In:		no			; �̨�ܰ�����
//			rx,ry		; x,y�g�k��(0x0100=1)
//			theta		; �p�x(0x0100=0)
//------------------------------------------
void OBJ_AffineWorkCalc( u8 no, s16 rx, s16 ry, s16 theta )
{
	ObjAffineSrcData src;
	ObjAffineDestData dest;

	src.RatioX = inverse( rx );
	src.RatioY = inverse( ry );
	src.Theta = theta;

	ObjAffineSet( &src, &dest, 1, 2 );
	affineWorkDestSet( no, &dest );
}

//========================================================================
//			���Ȱ�ެ
//========================================================================
static void	cellman_pos_set( u16 act_no, u16 adrs, u16 size );
static u8	cellman_pos_get( u16 );

//------------------------------------------
//	�ٓo�^�P��
//	In		cell	; ���ް�
//	Out		u16		; ��ķ������
//------------------------------------------
u16	CellSet( const CellData *cell )								// �ْP���o�^
{
	s16	adrs;

	adrs = TransAreaFlagChk( (cell->size) >> 5 );			// size / 32 = chara no size

	if( adrs < 0 )											// ϲŽ�� �o�^�s�� 
	{
		return( 0 );
	}

	cellman_pos_set( cell->act_num,	adrs, (cell->size) >> 5 );	// ���Ȱ�ެ�o�^
	
#if DMA_CPU_SELECT
	DmaCopy( 3, cell->ch_data,
				OBJ_MODE0_VRAM + (adrs << 5), cell->size, 16 );	// ����ް��]��
#else
	CpuCopy(cell->ch_data, OBJ_MODE0_VRAM + (adrs << 5), cell->size, 16);
#endif
	return( adrs );
}

//------------------------------------------
//	�ٓo�^����
//	In		cell	; ���ް�
//	Info
//		cell[] =
//		{
//			//{ ����ް�, �]������, �o�^�ԍ� }
//			{	&chara_data[0], 0x0040, 0 },
//			{	&chara_data[64], 0x0080, 1 },
//			{   CELL_END, 0, 0 }							// �o�^�I��
//		};
//------------------------------------------
void CellSets( const CellData *cell )
{
	u8	i;

	for( i = 0; cell[i].ch_data != CELL_END; CellSet( &cell[i] ), i++ );
}

//------------------------------------------
//	�ٓo�^���폜
//	In		act_no	; �o�^�ԍ�
//------------------------------------------
void CellDel( u16 act_no )									// �o�^�ԍ�
{
	u8	pos;
	u16	i, adrs, size;
	
	pos  = cellman_pos_get( act_no );						// �o�^�ԍ���
	
	if( pos != 0xff )
	{
		adrs = cell_char_num[pos][0];
		size = cell_char_num[pos][1];
	
		for( i = adrs; i < adrs + size; i++ )
		{
//			TransBitChkSet(i,0);								// �o�^�ر�ر
			TRANS_BIT_CLEAR(i);
		}
	
		cell_act_num[pos] = 0xffff;
	}
}
	
//------------------------------------------
//	���Ȱ�ެ�S������
//------------------------------------------
void CellManAllInit( void )
{
	u8	i;

	for( i = 0; i < ACT_MAX; i++ )
	{
		cell_act_num[i] = 0xffff;
		cell_char_num[i][0] = 0x0000;
		cell_char_num[i][1] = 0x0000;
	}
}
//------------------------------------------
//	�o�^�ԍ� -> �o�^�������
//	In:		act_no		; �o�^�ԍ�
//	Out:	u16			; �������
//						; 0xffff = Error
//------------------------------------------
u16	CellCharNumGet( u16 act_no )
{
	u8	i;
	
	i = cellman_pos_get( act_no );
	
	if( i == 0xff )
	{
		return( 0xffff );
	}

	return( cell_char_num[i][0] );
}

//------------------------------------------
//	�o�^�ԍ� -> �o�^�ʒu
//------------------------------------------
static u8 cellman_pos_get( u16 act_no )
{
	u8	i;

	for( i = 0; i < ACT_MAX; i++ )
	{
		if( cell_act_num[i] == act_no )
		{
			return( i );
		}
	}

	return( 0xff );
}

//------------------------------------------
//	������� -> �o�^�ԍ�
//	In:		chnum			; �������
//	Out:	u16				; ID
//							; 0xffff = Error
//------------------------------------------
u16	CellCharNumIdGet( u16 chnum )
{
	u8 i;
	
	for( i = 0; i < ACT_MAX; i++ )
	{
		if( cell_act_num[i] != 0xffff && cell_char_num[i][0] == chnum )
		{
			return( cell_act_num[i] );
		}
	}
	
	return( 0xffff );
}

//------------------------------------------
//	���Ȱ�ެ��������
//------------------------------------------
static void	cellman_pos_set( u16 act_no, u16 adrs, u16 size )
{
	u8	i;
	
	i = cellman_pos_get( 0xffff );							// ���o�^��
	cell_act_num[i] = act_no;
	cell_char_num[i][0] = adrs;
	cell_char_num[i][1] = size;
}


//========================================================================
//			��޼ު��گ��Ȱ�ެ
//========================================================================
static void	obj_palset_work( const u16 *, u16 );
u8 ObjPalNumGet( u16 );

//------------------------------------------
//	������
//------------------------------------------
void ObjPalManInit( void )
{
	u8	i;
	
	ObjPalManStart = 0;
	for( i = 0; i < 16; ObjPalManWork[i] = 0xffff, i++ );
}

//------------------------------------------
//	��گēo�^�P��
//	In		pal		; ��گēo�^�ް�
//	Out		u8		; �o�^��گ�No
//					; 0xff = Error
//------------------------------------------
u8 ObjPalSet( const PalData *pal )
{
	u8	pal_no;

	pal_no = ObjPalNumGet( pal->reg_num );

	if( pal_no != 0xff )
	{
		return( pal_no );
	}

	pal_no = ObjPalNumGet( 0xffff );							// ���o�^��
	
	if( pal_no == 0xff )
	{
		return( pal_no );										// �󂫖���
	}

	ObjPalManWork[pal_no] = pal->reg_num;
	obj_palset_work( pal->pal_data, (u16)((u16)pal_no << 4) );	// pal_no * 16
	return( pal_no );
}

//------------------------------------------
//	��گēo�^����
//	In		pal		; ��گēo�^�ް�
//	Info
//		pal[] = 
//		{ //{ ��گ��ް�, �o�^�ԍ� }
//			{ Palette0, 0 },
//			{ Palette1, 1 },
//			{ OBJPAL_END, 0 }		// �o�^�I��
//		};
//------------------------------------------
void ObjPalSets( const PalData *pal )
{
	u8	i;

	for( i = 0; pal[i].pal_data != OBJPAL_END && (ObjPalSet(&pal[i])) != 0xff; i++ );
}

//------------------------------------------
//	��گē]��
//------------------------------------------
static void	obj_palset_work( const u16 *pal_data, u16 set_pos )
{
//	DmaCopy( 3, pal_data, OBJ_PLTT + set_pos, 0x0020, 32 );
	PaletteWorkSet( pal_data, PA_OBJ0 + set_pos, 32 );
}

//------------------------------------------
//	��گēo�^�ԍ����
//	In:		reg_num		; �o�^�ԍ�
//	Out:	u8			; �o�^��گ�No
//------------------------------------------
u8 ObjPalNumSet( u16 reg_num )
{
	u8 no;

	no = ObjPalNumGet( 0xffff );
		
	if( no == 0xff )
	{
		return( no );
	}
	
	ObjPalManWork[no] = reg_num;

	return( no );
}

//------------------------------------------
//	�o�^�ԍ� -> ��گ�No
//	In		reg_no		; �o�^�ԍ�
//	Out		u8			; ��گ�No(0~15)
//						; 0xff = Error
//------------------------------------------
u8 ObjPalNumGet( u16 reg_no )
{
	u8	i;

	for( i = ObjPalManStart; i < 16; i++ )
	{
		if( ObjPalManWork[i] == reg_no )
		{
			return( i );
		}
	}

	return( 0xff );
}

//------------------------------------------
//	��گ����� -> ID
//	In:		no			; ��گ� No
//	Out:	u16			; ��گ� ID
//						; 0xffff = Error
//------------------------------------------
u16 ObjPalNumIdGet( u8 no )
{
	return( ObjPalManWork[no] );
}

//------------------------------------------
//	�o�^�ԍ��폜
//	In		reg_no		; �o�^�ԍ�
//------------------------------------------
void ObjPalRegNumDel( u16 reg_no )
{
	u8	pal_no;

	pal_no = ObjPalNumGet( reg_no );
	
	if( pal_no == 0xff )
	{
		return;
	}
	
	ObjPalManWork[pal_no] = 0xffff;
}

//========================================================================
//	OAM �߯�
//========================================================================
u8 OamPatDevelop( actWork *, ActOamData *, u8 * );

//------------------------------------------
//	���W
//------------------------------------------
typedef struct
{
	s8	x;
	s8	y;
}POINTs8;

//------------------------------------------
//	�`�滲��
//------------------------------------------
static const POINTs8 OamBltSize[3][4] =
{
	{
		{8,8},
		{16,16},
		{32,32},
		{64,64},
	},
	{
		{16,8},
		{32,8},
		{32,16},
		{64,32},
	},
	{
		{8,16},
		{8,32},
		{16,32},
		{32,64},
	}
};

//------------------------------------------
//	OAM �߯ľ��
//	In:		act			; ��ı��
//			pat			; �߯�ð���
//------------------------------------------
void SetActOamPat( actWork *act, const OAMPatSt *pat )
{
	act->oamPat = pat;
	act->oampat_no = 0;
	act->oampat_sw = OAMPAT_ON;
}

//------------------------------------------
//	OAM �ւ̐ݒ�
//	In:		act			; ����
//			use			; OAM �g�p��
//	Out:	u8			; ON = �g�p�ő�
//------------------------------------------
u8 ActOamSet( actWork *act, u8 *use )
{
	if( *use >= OamWriteEd )
	{
		return( 1 );
	}
	
	if( act->oamPat == NOT_OAMPAT || act->oampat_sw == OAMPAT_OFF )
	{
		sys.OAMbuf[*use] = act->oamData;
		(*use)++;
		return( 0 );
	}
	else
	{
		return( OamPatDevelop(act,&(sys.OAMbuf[*use]),use) );
	}
}

//------------------------------------------
//	OAM �߯ēW�J
//	In:		act			; �W�J����
//			oam_b		; �W�J��OAM
//			oam_use		; OAM �g�p��
//	Out:	u8			; ON = OAM �g�p�ő�
//------------------------------------------
u8 OamPatDevelop( actWork *act, ActOamData *oam_b, u8 *oam_use )
{
	u16	chnum;
	s16	x, y, cx, cy;
	u8	i, max, hflip, vflip;
	OAMPatSt *patst;
	ActOamData *oam_a;

	if( *oam_use >= OamWriteEd )
	{
		return( 1 );
	}
	
	patst = (OAMPatSt*)&(act->oamPat[act->oampat_no]);
	oam_a = (ActOamData*)&(act->oamData);

	if( patst == NOT_OAMPAT || patst->pat == NOT_OAMPAT )		// ���g�p
	{
		*oam_b = *oam_a;
		(*oam_use)++;
		return( 0 );
	}
	
	chnum = oam_a->CharNo;
	max = patst->use;
	hflip = (oam_a->AffineParamNo >> 3) & 0x01;
	vflip = (oam_a->AffineParamNo >> 4) & 0x01;
	cx = (s16)oam_a->HPos - (s16)act->cx;						// X ���S�ʒu
	cy = (s16)oam_a->VPos - (s16)act->cy;						// Y ���S�ʒu
	
	for( i = 0; i < max; i++, (*oam_use)++ )
	{
		if( *oam_use >= OamWriteEd )
		{
			return( 1 );
		}

		x = (s16)(patst->pat[i].x);
		y = (s16)(patst->pat[i].y);
		
		if( hflip )
		{
			x += (s16)OamBltSize[patst->pat[i].shape][patst->pat[i].size].x;
			x = (~x) + 1;
		}
		
		if( vflip )
		{
			y += (s16)OamBltSize[patst->pat[i].shape][patst->pat[i].size].y;
			y = (~y) + 1;
		}

		oam_b[i] = *oam_a;
		oam_b[i].Shape = patst->pat[i].shape;					// �`��
		oam_b[i].Size = patst->pat[i].size;						// ����
		oam_b[i].HPos = cx + x;									// ���W
		oam_b[i].VPos = cy + y;
		oam_b[i].CharNo = chnum + patst->pat[i].ch_offs;		// �������
		
		if( act->oampat_sw != OAMPAT_PRIUNIT )
		{
			oam_b[i].Priority = patst->pat[i].bg_pri;			// BG ��ײ��è
		}
	}

	return( 0 );
}

