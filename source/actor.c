/********************************************************************/
/*			oam_act.c												*/
/*				  OAM�֘AҲ�										*/
/********************************************************************/
#include "common.h"
#include "actor.h"
#include "actanm.h"
#include  "cell_bitchk.h"

#include "d_matsu.h"
#include "print.h"


//----------------------------------------------------------
//	�萔��`
//---------------------------------------------------------
#define H_OUT	(240+64)			// ��ʊO���W
#define V_OUT	(160)
#define CHARNO_MAX	1024			// ��׸�����ް(��׸��Ȱ�)ϯ���l
#define TRANS_MAX	64
#define PRI_DEBUG	1				// ��ײ��è���ޯ��

/*==================================================================*/
/*						��۰��ٕϐ��w�� 							*/
/*==================================================================*/
actWork		ActWork[ACT_BUF_MAX] = {};
u16			ActSortPriTbl[ACT_MAX] = {0};
u8	 		ActSortTbl[ACT_MAX];	// ������ėpܰ�
u8			SetActorFlag;			// ����OAM����׸�
u8			DMAtransCount;			// DMA�]���p����
dmaTransTbl DMAtransTbl[TRANS_MAX];	// DMA�]���pð���
u8			OamWriteEd;				// �g�p����OAM�͈̔͂����肷��(0�`OamWriteEd(MAX:128))
u16 		ObjDataTransSt;			// OBJ��׸��ް���VRAM�]���J�n�ʒu���w��(��׸��No��)
u8			TransAreaFlag[128];		// �]���󂫗̈������׸ތQ(1024bit)
s16 		ActOX;					// ��۰��ٱ����␳X
s16 		ActOY;					// ��۰��ٱ����␳Y
ObjAffineDestData OBJ_AffineWork[32];

//----------------------------------------------------------
//	�v���g�^�C�v�錾
//----------------------------------------------------------
void affineWorkinit(void);
void actOAMinit(u8 start, u8 end);
void SetActor(void);
void actOAMtrans(void);
void actDMAtrans(void);
void InitActor(void);
void JumpActor(void);
void SetActor(void);
u8 AddActor(const actHeader* headAdrs, s16 posX, s16 posY, u8 pri);
void DelActor(actWork *);
void actCenterCalc(actWork *,u8 shape, u8 size, u8 affine_mode );

u8 TransBitChkSet(u16 num,u8 mode);
s16 TransAreaFlagChk(u16 size);	
void dmaTransInit(void);

#if PRI_DEBUG
static void SetPriority( void );
#endif

static void SortProc(void);
static void OamProc(void);
static void AffineProc(void);

void dmaTransSet(u16 pat,u16 charno, const actAnmData *);	//,u8 mode);
void SetCoordinates(void);

const ActOamData DummyActOam;
const actAnm* const DummyActAnmTbl[];
const actAffAnm * const DummyActAffTbl[];
void DummyActMove(actWork *actst);

void InitActWork(void);
static u8 AddActorSub(u8 actno, const actHeader* headAdrs, s16 posX, s16 posY, u8 pri);

//----------------------------------------------------------
//	Dma�]��ظ��ĵ��ް���̴װү����
//----------------------------------------------------------
static const u8 dma_over_msg[] = {D__,M__,A__,spc_,O__,V__,E__,R__,CR_,
	D__,M__,A__,te_,n_,so_,u_,CR_,RI_,KU_,E_,SU_,TO_,TE_,bou_,BU_,RU_,ga_,CR_,
	O_,bou_,BA_,bou_,si_,ma_,si_,ta_,EOM_};


/*==================================================================*/
/*			�����֌W�S������										*/
/*==================================================================*/
void InitActor(void)
{
//	ActWork = (actWork*)ACTOR_WORK_AREA;	// �O��ܰ��ر�����蓖��

	actOAMinit( 0, 128 );					//����OAM�ر
	InitActWork();

	dmaTransInit();							//DMA�]��ð��� ������

	AffineWorkManInit();					// �g�kܰ�, �Ȱ�ެ������
	CellManAllInit();						// ���Ȱ�ެ������

	OamWriteEd = 64;						//�g�pOAM�͈͌���(0�`OamWriteEd(MAX:128))
	ObjDataTransSt = 0;						//OBJ��׸��ް�VRAM�]���J�n�ʒu�w��(��׸�No)
	TransAreaFlagChk(0);					//�]���󂫗̈������׸�(1024bit)
	ActOX = 0;								// �S����C�����W X
	ActOY = 0;								// �S����C�����W Y
}

/*==================================================================*/
/*						��������									*/
/*==================================================================*/
void JumpActor( void )
{
	u8 num;
	actWork *act;

	for( num = 0; num < ACT_MAX; num++ )
	{
		act = &ActWork[num];

		if( act->act_sw )
		{
			act->move( act );
			
			if( act->act_sw )
			{
				ActAnm( act );
			}
		}
	}
}

//----------------------------------------------------------
//	�A�N�^�[�Z�b�g
//----------------------------------------------------------
void SetActor(void)
{
	u8 temp;

	SetCoordinates();				//add 01/03/14

#if PRI_DEBUG
	SetPriority();
#endif

	SortProc();						//������ײ��è���

	temp = sys.OAMTransFlag;		//add 01/04/16

	sys.OAMTransFlag = 1; 			//OAM�]�� �s����
	OamProc();
	AffineProc();

#if 0
	sys.OAMTransFlag = 0; 			// OAM�]�� ����
#else
	sys.OAMTransFlag = temp;		// 01/04/16
#endif

	SetActorFlag = 1;				// OAM ��Ċ���
}

//----------------------------------------------------------
//	OAM�ޯ̧�ɍ��W���Z�b�g
//----------------------------------------------------------
void SetCoordinates( void )
{
	u8 i;
	actWork *act;

	for( i = 0; i < ACT_MAX; i++ )
	{
		act = &ActWork[i];						// ���ð���

		if( act->act_sw && act->banish == 0 )	// �ғ� & �����׸�
		{
			if( act->scr )						// ��۰وˑ�
			{
				act->oamData.HPos = act->x + act->dx + act->cx + ActOX;
				act->oamData.VPos = act->y + act->dy + act->cy + ActOY;
			}
			else
			{									// ��۰وˑ�����
				act->oamData.HPos = act->x + act->dx + act->cx;
				act->oamData.VPos = act->y + act->dy + act->cy;
			}
		}
	}
}

//----------------------------------------------------------
//	��ײ��è���
//----------------------------------------------------------
#if PRI_DEBUG
static void SetPriority( void )
{
	u16 i,pri;

	for( i = 0; i < ACT_MAX; i++ )
	{
		pri = ActWork[i].pri;
		pri |= ActWork[i].oamData.Priority << 8;
		ActSortPriTbl[i] = pri;
	}
}
#endif

/*==================================================================*/
/*						��ײ��è�����								*/
/*==================================================================*/
static void SortProc( void )
{
	u8 i,j,dmy;
	u16 pri0,pri1;
	s16 y0,y1;
	actWork *act0,*act1;

	for( i = 1; i < ACT_MAX; i++ )
	{
		j = i;

		act0 = &ActWork[ActSortTbl[j-1]];
		act1 = &ActWork[ActSortTbl[j]];
		pri0 = ActSortPriTbl[ActSortTbl[j-1]];
		pri1 = ActSortPriTbl[ActSortTbl[j]];
		y0 = act0->oamData.VPos;
		y1 = act1->oamData.VPos;

		if( y0 >= 160 )										// �\����ʊO
		{
			y0 -= 256;
		}

		if( y1 >= 160 )
		{
			y1 -= 256;
		}

		if( act0->oamData.AffineMode == 3 &&				// �g���荞��
			act0->oamData.Size == 3 &&
			(act0->oamData.Shape == 0 || act0->oamData.Shape == 2) &&
			y0 > 128 )
		{
			y0 -= 256;
		}

		if( act1->oamData.AffineMode == 3 &&
			act1->oamData.Size == 3 &&
			(act1->oamData.Shape == 0 || act1->oamData.Shape == 2) &&
			y1 > 128 )
		{
			y1 -= 256;
		}

		while( j >= 1 && ((pri0 > pri1) || (pri0 == pri1 && y0 < y1)) )
		{
			dmy = ActSortTbl[j];
			ActSortTbl[j] = ActSortTbl[j-1];
			ActSortTbl[j-1] = dmy;

			j--;
		
			act0 = &ActWork[ActSortTbl[j-1]];
			act1 = &ActWork[ActSortTbl[j]];
			pri0 = ActSortPriTbl[ActSortTbl[j-1]];
			pri1 = ActSortPriTbl[ActSortTbl[j]];
			y0 = act0->oamData.VPos;
			y1 = act1->oamData.VPos;

			if( y0 >= 160 )
			{
				y0 -= 256;
			}

			if( y1 >= 160 )
			{
				y1 -= 256;
			}

			if( act0->oamData.AffineMode == 3 &&
				act0->oamData.Size == 3 &&
				(act0->oamData.Shape == 0 || act0->oamData.Shape == 2) &&
				y0 > 128 )
			{
				y0 -= 256;
			}

			if( act1->oamData.AffineMode == 3 &&
				act1->oamData.Size == 3 &&
				(act1->oamData.Shape == 0 || act1->oamData.Shape == 2) &&
				y1 > 128 )
			{
				y1 -= 256;
			}
		}
	}
}

//----
#if 0
//----
static void SortProc(void)
{
	u8 i, j, t;
	u8 pri1, pri2;
	s16 y1, y2;

	for(i = 1; i < ACT_MAX; i++)
	{
		j = i;

		//-------------����ް��擾----------------------//
		pri1 = ActWork[ActSortTbl[j-1]].pri;
		pri2 = ActWork[ActSortTbl[j]].pri;
		y1	 = ActWork[ActSortTbl[j-1]].oamData.VPos;
		y2	 = ActWork[ActSortTbl[j]].oamData.VPos;

		// ��ʏ㕔�A��荞�ݕ\�����̑Ώ�
		if(y1 > 159)
			y1 -= 256;
		if(y2 > 159)
			y2 -= 256;
		
		if(ActWork[ActSortTbl[j-1]].oamData.AffineMode == 3 && 
			ActWork[ActSortTbl[j-1]].oamData.Size == 3 &&
			(ActWork[ActSortTbl[j-1]].oamData.Shape == 0 ||
			ActWork[ActSortTbl[j-1]].oamData.Shape == 2) &&
			y1 > 128)
		{
			y1 -= 256;
		}
		
		if(ActWork[ActSortTbl[j]].oamData.AffineMode == 3 && 
			ActWork[ActSortTbl[j]].oamData.Size == 3 &&
			(ActWork[ActSortTbl[j]].oamData.Shape == 0 ||
			ActWork[ActSortTbl[j]].oamData.Shape == 2) &&
			y2 > 128)
		{
			y2 -= 256;
		}
		//------------�ް��擾END-----------------------//

		while(j >= 1 && //(PriCheck(pri1, pri2, y1, y2)))	//a[j-1] > a[j])
				((pri1 > pri2) || (pri1 == pri2 && y1 < y2)))
		{
			t = ActSortTbl[j];
			ActSortTbl[j] = ActSortTbl[j-1];
			ActSortTbl[j-1] = t;
			j--;
			
			//-------------����ް��擾----------------------//
			pri1 = ActWork[ActSortTbl[j-1]].pri;
			pri2 = ActWork[ActSortTbl[j]].pri;
			y1	 = ActWork[ActSortTbl[j-1]].oamData.VPos;
			y2	 = ActWork[ActSortTbl[j]].oamData.VPos;

			// ��ʏ㕔�A��荞�ݕ\�����̑Ώ�
			if(y1 > 159)
				y1 -= 256;
			if(y2 > 159)
				y2 -= 256;
			
			if(ActWork[ActSortTbl[j-1]].oamData.AffineMode == 3 && 
				ActWork[ActSortTbl[j-1]].oamData.Size == 3 &&
				(ActWork[ActSortTbl[j-1]].oamData.Shape == 0 ||
				ActWork[ActSortTbl[j-1]].oamData.Shape == 2) &&
				y1 > 128)
			{
				y1 -= 256;
			}
			
			if(ActWork[ActSortTbl[j]].oamData.AffineMode == 3 && 
				ActWork[ActSortTbl[j]].oamData.Size == 3 &&
				(ActWork[ActSortTbl[j]].oamData.Shape == 0 ||
				ActWork[ActSortTbl[j]].oamData.Shape == 2) &&
				y2 > 128)
			{
				y2 -= 256;
			}
			//------------�ް��擾END-----------------------//
		}
	}
}
//----
#endif
//----

/*==================================================================*/
/*						�g��k����]�ް��ݒ菈��					*/
/*==================================================================*/
static void AffineProc(void)
{
	u8 i;
	
	for(i = 0; i < 32; i++)
	{
		sys.OAMbuf[(i << 2) + 0].AffineParam = OBJ_AffineWork[i].H_DiffX;
		sys.OAMbuf[(i << 2) + 1].AffineParam = OBJ_AffineWork[i].V_DiffX;
		sys.OAMbuf[(i << 2) + 2].AffineParam = OBJ_AffineWork[i].H_DiffY;
		sys.OAMbuf[(i << 2) + 3].AffineParam = OBJ_AffineWork[i].V_DiffY;
	}
}

/*==================================================================*/
/*						OAM�ݒ菈�� 								*/
/*==================================================================*/
static void OamProc(void)
{
	u8	i,use;
	actWork	*act;

	for( i = 0, use = 0; i < ACT_MAX; i++ )
	{
		act = &(ActWork[ActSortTbl[i]]);
		
		if( act->act_sw && act->banish == 0 )
		{
			if( ActOamSet( act, &use ) )
			{
				return;
			}
		}
	}
	
	for( ; use < OamWriteEd; use++ )				// ������
	{
		sys.OAMbuf[use] = DummyActOam;
	}
}


/*==================================================================*/
/*						�����ǉ�									*/
//	�߂�l: ��Ă���ActWork�̔z��ԍ� ���װ����ACT_MAX
// ���� : headAdrs:����ͯ�ް���ڽ
//        posX,posY:�������W   pri:�������m����ײ��è(OAM�Ɗ֌W�Ȃ�)
/*==================================================================*/
const u8 ActSizeTbl[2][3][4];

u8 AddActor( const actHeader* headAdrs, s16 posX, s16 posY, u8 pri )
{
	u8 i, pos;

	for( i = 0; i < ACT_MAX; i++ )
	{
		if( ActWork[i].act_sw == 0 )				// �ғ��׸�
		{
			pos = AddActorSub( i, headAdrs, posX, posY, pri );
			return( pos );							// ����ܰ��ǉ�����
		}
	}

	return( ACT_MAX );								// ��ĕs��
}

//----------------------------------------------------------
//	����ܰ��̋󂫂���납�璲�ׂāA��Ă���
//----------------------------------------------------------
u8 AddActorBottom(const actHeader* headAdrs, s16 posX, s16 posY, u8 pri)
{
	s16 i;
	u8  pos;

	for( i = ACT_MAX-1; i > -1; i-- )
	{
		if( ActWork[i].act_sw == 0 )				// �����ғ�����
		{
			pos = AddActorSub( i, headAdrs, posX, posY, pri );
#if 0
			ActWork[i].move(&(ActWork[i]));			// ����֐����s
			
			if( ActWork[i].act_sw )
			{
				ActAnm( &(ActWork[i]) );
			}
#endif
			return( pos );							// ����ܰ��ǉ����ް
		}
	}

	return( ACT_MAX	);								// ��ĕs��
}

//----------------------------------------------------------
//	��а�����o�^
//	In:		move		; ��������֐�
//----------------------------------------------------------
u8 AddDummyActor( void (*move)(actWork*) )
{
	u8 i;
	
	i = AddActor( &DummyActHeader, 0, 0, 31 );
	
	if( i == ACT_MAX )
	{
		return( i );
	}
	
	ActWork[i].banish = 1;
	ActWork[i].move = move;

	return( i );
}

//----------------------------------------------------------
//	����ܰ���Ļ��
//----------------------------------------------------------
static u8 AddActorSub(u8 actno, const actHeader* headAdrs, s16 posX, s16 posY, u8 pri)
{
	u8 size;
	s16 Vpos;
	actWork* act;

	act = &ActWork[actno];
	
	actWorkInit( act );

	act->act_sw  = 1;								// �ғ��׸޾��
	act->anm_sw  = 1;								// ��ҕύX�׸޾��
	act->affine_sw = 1;								// �g�k����׸޾��
	act->t_mode = 1;								// �����
	act->pri = pri;
	act->oamData = *(headAdrs->oamData_p);
	act->anm_tbl = headAdrs->anmData_p;
	act->aff_tbl = headAdrs->affData_p;
	act->actHead = headAdrs;
	act->move    = headAdrs->moveData_p;
	act->x = posX;
	act->y = posY;
	
	actCenterCalc( act, act->oamData.Shape,
						act->oamData.Size, act->oamData.AffineMode);

	if( headAdrs->cellNo == ACT_TMODE )				// ACT_TMODE = ��ד]���^
	{
		act->anm_trans = headAdrs->anmTrans_p;		// �����ݽ�ް�
		size = (u8)(act->anm_trans[0].size >> 5);	// �]������
		Vpos = TransAreaFlagChk( size );			// size�����ڽ(�擪��׸��Ȱ�)�擾

		if( Vpos == -1 )
		{
			actWorkInit( act );						// ������
			return( ACT_MAX );						// ERROR
		}

		act->oamData.CharNo = Vpos;					// ������׸�����ް
		act->t_mode = 0;							// �]���^
		act->char_num = 0;
	}
	else											// ����ݔ�]���^
	{
		act->char_num = CellCharNumGet( headAdrs->cellNo );
		ActCharNumSet( act );						// �Ȱ�ެ, ������޾��
	}

	if( (act->oamData.AffineMode & 0x01) )			// �g�k ON
	{
		AffineNumSet( act );						// �̨��Ȱ�ެ
	}

	if( headAdrs->palNo != ACT_OAMPAL )				// ��گ��Ȱ�ެ�ɂ�龯�
	{
		act->oamData.Pltt = ObjPalNumGet( headAdrs->palNo );
	}

	return actno;									// ����ܰ��ǉ����ް
}

//----------------------------------------------------------
//	AddActor�����u�Ԃɓ���֐������s����܂�
//----------------------------------------------------------
u8 AddActorJump(const actHeader* headAdrs, s16 posX, s16 posY, u8 pri)
{
	u8 i, pos;

	for( i = 0; i < ACT_MAX; i++ )
	{
		if( ActWork[i].act_sw == 0 )				// �����ғ�����
		{
			pos = AddActorSub(i, headAdrs, posX, posY, pri);
			
			if( pos == ACT_MAX )
			{
				return( pos );
			}

			ActWork[i].move( &(ActWork[i]) );		//����֐����s

			if( ActWork[i].act_sw )
			{
				ActAnm( &(ActWork[i]) );
			}
			
			return( pos );
		}
	}

	return( ACT_MAX	);								// ��ĕs��
}

//----------------------------------------------------------
//	�]���T�C�Y(��׸�Ȱ�)
//----------------------------------------------------------
const u8 ActSizeTbl[2][3][4] =
{
	{								// 16
		{
			1,
			4,
			16,
			64,
		},
		{
			2,
			4,
			8,
			32,
		},
		{
			2,
			4,
			8,
			32,
		},
	},
	{								// 256
		{
			1,
			4,
			16,
			64,
		},
		{
			2,
			4,
			8,
			32,
		},
		{
			2,
			4,
			8,
			32,
		},
	},
};

/*==================================================================*/
/*						�����폜									*/
/*==================================================================*/
void DelActor( actWork *act )
{
	u16 i,size;
	
	if( act->act_sw == 0 )
	{
		return;
	}

	if( act->t_mode == 0 )				// �]���^
	{
		size = (act->anm_trans[0].size >> 5) + act->oamData.CharNo;

		for( i = act->oamData.CharNo; i < size; i++ )
		{
//			TransBitChkSet( i, 0 );						// ��ׂ�]���ʒu�׸�ؾ��
			TRANS_BIT_CLEAR(i);
		}
	}

	actWorkInit( act );
}

/*==================================================================*/
/*						OAMbuf�������@								*/
/*==================================================================*/
void actOAMinit(u8 start, u8 end)
{
	u8 i;
	
	for( i = start; i < end; i++ )
	{
		*((SysOamData *)(&(sys.OAMbuf[i]))) = *((SysOamData *)(&DummyActOam));
	}
}

/*==================================================================*/
/*						OAM�ւ̓]��(VBLANK) 						*/
/*==================================================================*/
void actOAMtrans(void)
{
	if( sys.OAMTransFlag == 0 )
	{
#if DMA_CPU_SELECT
		DIV_DMAARRAYCOPY(3, sys.OAMbuf, OAM, 32);		//OAM�]��
#else
		CpuArrayCopy(sys.OAMbuf, OAM, 32);			//OAM�]��
#endif	
	}
}

/*==================================================================*/
/*						��׸���ް��]��ð��ُ�����					*/
/*==================================================================*/
void dmaTransInit(void)
{
	u8 i;

	SetActorFlag = 0;
	DMAtransCount = 0;

	for( i = 0 ; i < TRANS_MAX; i++ )
	{
		DMAtransTbl[i].dat = 0;
		DMAtransTbl[i].dest = 0;
		DMAtransTbl[i].size = 0;
	}
}

/*==================================================================*/
/*						�g��k��ܰ��������@�@					   */
/*==================================================================*/
void affineWorkinit(void)
{
	u8 i;
	
	for(i = 0; i < 32; i++)
	{
		OBJ_AffineWork[i].H_DiffX = 0x100;
		OBJ_AffineWork[i].V_DiffX = 0x000;
		OBJ_AffineWork[i].H_DiffY = 0x000;
		OBJ_AffineWork[i].V_DiffY = 0x100;
	}
}

/*==================================================================*/
/*						�g��k��ܰ��ݒ�  �@�@						*/
/*==================================================================*/
void affineWorkSet(u8 num,u16 H_X,u16 V_X,u16 H_Y,u16 V_Y)
{
	OBJ_AffineWork[num].H_DiffX = H_X;
	OBJ_AffineWork[num].V_DiffX = V_X;
	OBJ_AffineWork[num].H_DiffY = H_Y;
	OBJ_AffineWork[num].V_DiffY = V_Y;
}

//----------------------------------------------------------
//	�A�N�^�[���[�N������
// ���� : act = ������ �������ڽ
//----------------------------------------------------------
#if 1
void actWorkInit( actWork *act )
{
	*act = DummyActWork;
}
#else
void actWorkInit( actWork *act )
{
	act->oamData 	= DummyActOam;	  			// OAM��{�ް�
	act->anm_tbl 	= DummyActAnmTbl;			// ACTOR�\�������ð��ٱ��ڽ
	act->anm_trans  = ACT_NTRANS;				// �����ݽ�ް�
	act->aff_tbl	= DummyActAffTbl;			// �̨ݱ��ð���
	act->actHead = &DummyActHeader;				// add 01/04/26
	
	act->move = DummyActMove;					// ����֐�
	act->x	   	= H_OUT;						// X���W
	act->y	   	= V_OUT;//160;					// Y���W
	act->dx	   	= 0;							// X���W+��
	act->dy	   	= 0;							// Y���W+��
	act->cx	   	= 0;							// X���W+���S���W�擾�p�␳
	act->cy	   	= 0;							// Y���W+���S���W�擾�p�␳
	act->anm_no  	= 0;						// ANM���ް
	act->anm_offs	= 0; 						// ANM�̾��
	act->anm_wait	= 0;						// ANM����

	act->anm_pause = 0;							// ����߰��(1:ON)
	act->aff_pause = 0;							// �̨ݱ���߰��(1:ON)
	act->anm_count = 0;							// ���ٰ�߶���

	act->work[0] 	= 0;						// ܰ�
	act->work[1] 	= 0;
	act->work[2] 	= 0;
	act->work[3] 	= 0;
	act->work[4] 	= 0;
	act->work[5] 	= 0;
	act->work[6] 	= 0;
	act->work[7] 	= 0;

	act->Hflip	= 0;							// �����]�׸�(1:���])
	act->Vflip	= 0;							// �c���]�׸�(1:���])
	act->scr	= 0;							// BG��۰وˑ��׸�(1:�ˑ�)
	act->banish	= 1;							// ���ײĕ\���׸�(1:OFF)
	act->anm_sw	= 0;							// ��ҕύX�׸�
	act->anmend_sw= 0;							// ��ҏI���׸�(1:�I��)
	act->affend_sw= 0;							// �̨ݱ�ҏI���׸�(1:�I��)
	act->t_mode	= 0;							// CGX�]���׸�(0:�]�� 1:�����)
	act->act_sw	= 0;							// �����ғ��׸�(1:ON)
	
	act->affine_sw = 0;							// �̨ݱ�ҽ���
	act->char_num = 0;							// ������޵̾��

	act->oamPat = NOT_OAMPAT;					// �߯�ð���
	act->oampat_no = 0;							// �߯�����
	act->oampat_sw = OAMPAT_OFF;				// �߯Ľ���
	
	act->feature_flg = 0;						//add by matsuda 2001.07.03(��)
	
	// ����ײ��è��Ă̊֌W�Ŏg�p���Ă��Ȃ���������ײ��è����Œ�ɂ��Ă�������
	act->pri	= 15;							// ACTOR���m����ײ��è�(�� 0�`15 ��)
};
#endif

//----------------------------------------------------------
//		���S���W�擾�p�␳ �v�Z
//	SHAPE�ASIZE,�{�p�׸ނ���X,Y���W��␳����l���擾���A��Ă���
//����:	act = �ΏۂƂȂ鱸��ܰ�
//		shape = OAM_SHAPE(0�`2)
//		mode2 = OAM_SIZE(0�`3)
//		mode3 = OAM_Affine(0�`3)	���{�p�׸ނ�����2�ޯĂ̂��
//----------------------------------------------------------
static const s8 centerTbl[3][4][2];

void actCenterCalc( actWork *act, u8 shape, u8 size, u8 affine_mode )
{
	s8 cx, cy;
	
	cx = centerTbl[shape][size][0];				// �␳�l�擾
	cy = centerTbl[shape][size][1];
	
	if( (affine_mode & 2) != 0)
	{
		cx = cx << 1;							// �{�p�׸ނɍ��킹�Ĕ{�␳
		cy = cy << 1;
	}
	
	act->cx = cx;
	act->cy = cy;
}

//----------------------------------------------------------
//	�{�p�׸ނ̕���ð��قɔz�u���邩�͕ۗ�
//----------------------------------------------------------
static const s8 centerTbl[3][4][2] = 
{
	//shape 0
	{	//X�␳ Y�␳
		{-4, -4},		//size 0
		{-8, -8},		//size 1
		{-16, -16},	//size 2
		{-32, -32},	//size 3
	},
	//shape 1
	{
		{-8, -4},
		{-16, -4},
		{-16, -8},
		{-32, -16},
	},
	//shape 2
	{
		{-4, -8},
		{-4, -16},
		{-8, -16},
		{-16, -32},
	},
};


//----------------------------------------------------------
//	�]���󂫗̈������׸ތQ ���� (0:������)	��size = 0 �̏ꍇ�͑S�׸ޏ������ɂȂ�
// ���� : size = ��׸��Ȱѐ�(��׸��Ȱѻ���)
// �߂�l: 0 = ����������I��  :  -1 = �װ
//		   top_adrs = size���󂢂Ă���󂫗̈�̐擪��׸��Ȱ�
//----------------------------------------------------------
s16 TransAreaFlagChk(u16 size)
{
	u16 i;
	u16 space_count;	//�A�����đ����Ă���󂫗̈���Ă���
	s16 top_adrs;

	if(size == 0)	//size == 0 �͏�����
	{
		/*--------------------------------------------------------------
		for(i = 0;i < 128; i++)		//�]���󂫗̈������׸ތQ(1024bit)
		{
			TransAreaFlag[i] = 0xff;
		}
		--------------------------------------------------------------*/
		for(i = ObjDataTransSt; i < CHARNO_MAX; i++)
		{
//			TransBitChkSet(i,0);
			TRANS_BIT_CLEAR(i);
		}
		return 0;
	}

	top_adrs = 0;
	space_count = 0;
	i = ObjDataTransSt;	//OAM�]���J�nVRAM�̈�w��(��׸��No)

	do
	{
		while(TRANS_BIT_CHECK(i)) //�擪�󂫗̈挟��
		{
			i++;
			if(i == CHARNO_MAX)
			{
				return -1;	//ERROR �I�[�܂ŋ󂫗̈悪�Ȃ�����
			}
		}

		space_count = 0;
		top_adrs = (s16)i;		//�󂫗̈� �擪���ڽ(��׸��Ȱ�)��ޔ�

		do	 //�擪�󂫗̈悩��size���󂢂Ă��邩����
		{
			space_count++;
			if(space_count == size)
			{
				break;
			}
			
			i++;
			if(i == CHARNO_MAX)
			{
				return -1;	//ERROR Size���̋󂫗̈�𖞂����O�ɏI�[�ɍs���������Ă��܂���
			}
			
		}while(TRANS_BIT_CHECK(i) == 0);
		
	}while(space_count != size);

	for(i = (u16)top_adrs; i < size + top_adrs; i++)	//�ޯĂ�size�����
	{
		TRANS_BIT_SET(i);
	}
	return top_adrs;	//�󂫗̈�̐擪���ڽ(��׸��Ȱ�)��߂�l�Ƃ��ĕԂ�
}

//==================================================================
// ���� : num:�Ώ��ޯ�(0�`1023)  mode : 0=Clear 1=Set 2=Check
// �߂�l : mode Clear,Set�� = 0�Œ�
//			mode Check�� = 0:�Ώ��ޯĂ͋󂢂Ă� 0�ȊO:�Ώ��ޯĂ;�Ă���Ă�
//==================================================================
u8 TransBitChkSet(u16 num,u8 mode)
{
	u8 byte, bit, mask, flag;

	byte = num >> 3;								// num / 8
	bit = num & 7;									// num % 8
	flag = 0;

	if( mode == 0 )									// �ر
	{
		mask = (1 << bit) ^ 0xff;
		TransAreaFlag[byte] &= mask;
	}
	else if( mode == 1 )							// ���
	{
		mask = 1 << bit;
		TransAreaFlag[byte] |= mask;
	}
	else											// ����
	{
		mask = 1 << bit;
		flag = TransAreaFlag[byte] & mask;
	}
	
	return( flag );
}

//----
#if 0												// ��
//----
u8 TransBitChkSet(u16 num,u8 mode)
{
	u8	by,bt,mask,flg;

	flg = 0;
	by = num / 8;
	bt = num % 8;

	switch(mode)
	{
		case 0:		//�ر
			mask = (1 << bt) ^ 0xff;
			TransAreaFlag[by] &= mask;//�]���󂫗̈������׸ތQ(1024bit)
			flg = 0;
			break;
			
		case 1:		//���
			mask = 1 << bt;
			TransAreaFlag[by] |= mask;
			flg = 0;
			break;
			
		case 2:		//����
			mask = 1 << bt;
			flg = TransAreaFlag[by] & mask;
			break;
	}
	return flg;
}
//----
#endif
//----

/*==================================================================*/
/*						DUMMY�ݒ�									*/
/*==================================================================*/
/*------------------------------------------------------------------*/
/*						����ܰ�										*/
/*------------------------------------------------------------------*/
const actWork DummyActWork =
{
	{
		V_OUT,									// V�߼޼��
		0,										// �g��k��Ӱ�ސݒ�(OFF)
		0,										// OBJӰ�ސݒ�(NORMAL)
		0,										// ӻ޲��ݒ�(OFF)
		0,										// �װӰ�ސݒ�(16�F)
		0,										// OBJ�`��ݒ�(�����`)

		H_OUT,									// H�߼޼��
		0,										// �g��k�����Ұ����ސݒ� & H,V�د��
		0,										// OBJ�T�C�Y�ݒ�(8x8)

		0,										// ��׸������
		3,										// �\����ײ��è��ݒ�(��4�D��)
		0,										// ��گ�����
		0,										// �̨������
	},

	DummyActAnmTbl,								// ACTOR�\�������ð��ٱ��ڽ
	ACT_NTRANS,									// �����ݽ�ް�
	DummyActAffTbl,								// �̨ݱ��ð���
	&DummyActHeader,							// ����ͯ��
	NOT_OAMPAT,									// OAM�߯��ް�

	DummyActMove,								// ����֐�
	
	H_OUT,										// X���W
	V_OUT,										// Y���W
	0,											// X���W+��
	0,											// Y���W+��
	0,											// X���W+���S���W�擾�p�␳
	0,											// Y���W+���S���W�擾�p�␳
	0,											// ANM���ް
	0,											// ANM�̾��
	
	0,											// ANM����
	0,											// ����߰��(1:ON)
	0,											// �̨ݱ���߰��(1:ON)
	0,											// ���ٰ�߶���

	{ 0, 0, 0, 0, 0, 0, 0, 0 },					// ܰ�

	0,											// �����ғ��׸�(1:ON)
	0,											// BG��۰وˑ��׸�(1:�ˑ�)
	0,											// ���ײĕ\���׸�(1:OFF)

	0,											// �����]�׸�(1:���])
	0,											// �c���]�׸�(1:���])
	0,											// ��ҕύX�׸�
	0,											// �̨ݱ�ҕύX�׸�
	0,											// ��ҏI���׸�(1:�I��)
	0,											// �̨ݱ�ҏI���׸�(1:�I��)
	0,											// CGX�]���׸�(0:�]�� 1:�����)

	0,											// ���������׸�
	0,											// ������޵̾��

	0,											// �߯�����
	OAMPAT_OFF,									// �߯Ľ���
	
	0xff										// ��ײ��è
};

/*------------------------------------------------------------------*/
/*						OAM��{�ް� 								*/
/*------------------------------------------------------------------*/
const ActOamData DummyActOam =
{
	V_OUT,	//V�߼޼��
	0,		//�g��k��Ӱ�ސݒ�(OFF)
	0,		//OBJӰ�ސݒ�(NORMAL)
	0,		//ӻ޲��ݒ�(OFF)
	0,		//�װӰ�ސݒ�(16�F)
	0,		//OBJ�`��ݒ�(�����`)

	H_OUT,	//H�߼޼��
	0,		//�g��k�����Ұ����ް�ݒ� & H,V�د��
	0,		//OBJ�T�C�Y�ݒ�(8x8)

	0,		//��׸�����ް
	3,		//�\����ײ��è��ݒ�(��4�D��)
	0,		//��گ����ް
	0,		//AffineParam	add 01/02/06
};

/*------------------------------------------------------------------*/
/*						ANM�ް� 									*/
/*------------------------------------------------------------------*/
static const actAnm DummyActAnm[] = 
{
	{ACT_ANMEND, 0, 0, 0},
};

/*------------------------------------------------------------------*/
/*						ANMð���									*/
/*------------------------------------------------------------------*/
const actAnm* const DummyActAnmTbl[] = 
{
	DummyActAnm,
};

/*------------------------------------------------------------------*/
/*                      �g�k�ް�                                    */
/*------------------------------------------------------------------*/
static	const actAffAnm DummyActAff[] = {
	{ACT_AFEND, 0, 0, 0 },
};

/*------------------------------------------------------------------*/
/*                      �g�kð���                                   */
/*------------------------------------------------------------------*/
const actAffAnm * const DummyActAffTbl[] = {
	DummyActAff
};

/*------------------------------------------------------------------*/
/*						MOVE�ް�									*/
/*------------------------------------------------------------------*/
void DummyActMove(actWork *actst)
{
	;
}

const actHeader DummyActHeader = 
{
	0,
	ACT_OAMPAL,
	&DummyActOam,
	DummyActAnmTbl,
	0,
	DummyActAffTbl,
	DummyActMove,
};


/*==================================================================*/
/*                      ��׸���ް��]��(VBLANK)                      */
/*==================================================================*/
void actDMAtrans(void)
{
    u8 i;

	if( SetActorFlag == 0 ) return;

//	for(i=0;i<DMAtransCount;i++)	//����change 2001.07.25(��)
	for(i = 0; DMAtransCount > 0; DMAtransCount--,i++)
    {
#if DMA_CPU_SELECT
		DIV_DMACOPY(3,
				DMAtransTbl[i].dat,		//�]����
				DMAtransTbl[i].dest,	//�]����
				DMAtransTbl[i].size,	//�T�C�Y
				16);					//�]������
#else
		 CpuCopy(DMAtransTbl[i].dat,	//�]����
 	 			  DMAtransTbl[i].dest,	//�]����
  				  DMAtransTbl[i].size,	//�T�C�Y
  	 			  16);					//�]������
#endif
	}

	SetActorFlag = 0;					// OAM����׸޸ر
}

/*==================================================================*/
/*                      ��׸���ް��]��ظ��Đݒ�                     */
/*==================================================================*/
void dmaTransSet(u16 pat,u16 charno, const actAnmData *tbl)	//,u8 mode)
{
	if(DMAtransCount < TRANS_MAX)
	{
		DMAtransTbl[DMAtransCount].dat	= tbl[pat].dat; 		//��ױ��ڽ�擾
	//	DMAtransTbl[DMAtransCount].dest = OBJ_MODE0_VRAM + (charno << 5);	//�ύX01/04/06
		DMAtransTbl[DMAtransCount].dest = OBJ_MODE0_VRAM + (charno << 5);
		DMAtransTbl[DMAtransCount].size = tbl[pat].size;
		DMAtransCount++;
	}
#ifdef PM_DEBUG
	else
		ErrorMsgPut(dma_over_msg);
#endif
}

//=========================================================================
//	V���ݸ���ް��]��ظ���			add 4/9 by matsuda
// ����:datAdr = �]���ް����ڽ
//      destAdr = �]������ڽ
//      size = �]���޲Đ�
//=========================================================================
void DmaTransReq(u8 *datAdr, u32 destAdr, u16 size)
{
	if(DMAtransCount < TRANS_MAX)
	{
		DMAtransTbl[DMAtransCount].dat	= datAdr;
		DMAtransTbl[DMAtransCount].dest = destAdr;
		DMAtransTbl[DMAtransCount].size = size;
		DMAtransCount++;
	}
#ifdef PM_DEBUG
	else
		ErrorMsgPut(dma_over_msg);
#endif
}

//=========================================================================
//	�A�N�^�[���[�N�ޔ�
// destAdr = �ޔ���RAM���ڽ
//=========================================================================
void PushActor(void *destAdr)
{
	u8 *dest;
	u8 *src;
	u32 i;
	
	dest = destAdr;
	src = (u8 *)&ActWork[0];

	for(i = 0; i < ACTWORK_SIZEOF_OPERA; i++)
	{
		*dest = *src;
		dest++;
		src++;
	}
}
//=========================================================================
//	�A�N�^�[���[�N���A
// destAdr = PushActor�Ŏw�肵���ޔ��̱��ڽ
//=========================================================================
void PopActor(void *destAdr)
{
	u8 *dest;
	u8 *src;
	u32 i;
	
	dest = destAdr;
	src = (u8 *)&ActWork[0];
	
	for(i = 0; i < ACTWORK_SIZEOF_OPERA; i++)
	{
		*src = *dest;
		dest++;
		src++;
	}
}

//=========================================================================
//	�A�N�^�[���[�N�̂ݏ�����
// ��InitActor���ƾ��Ȱ�ެ���g�k�e�[�u���Ȃǂ������������̂�
//   PushActor,Pop�Ŏg�p����ꍇ�́A����InitActWork���g�p����
//=========================================================================
void InitActWork(void)
{
	u8 i;
	
	for( i = 0; i < ACT_MAX; i++ )
	{
		actWorkInit( &(ActWork[i]) );
		ActSortTbl[i] = i;					// ������ėpܰ� ������
	}

	actWorkInit( &ActWork[i] );				// ܰ����ް�p�΍��ޯ̧ ACT_BUF_MAX �ر
}

//=========================================================================
//	�Z���A�p���b�g�A�̨��Ȱ�ެ��Ȃǂ̱�������̊Ԑڍ폜�֐�
//=========================================================================
//----------------------------------------------------------
//	CELL�폜	���������A�]���^�̏ꍇ�͉������Ȃ�
//----------------------------------------------------------
void CellDelActor(actWork *actst)
{
	if(actst->actHead->cellNo != ACT_TMODE)
		CellDel(actst->actHead->cellNo);
}
//----------------------------------------------------------
//	Palette�폜
//----------------------------------------------------------
void PalDelActor(actWork *actst)
{
	ObjPalRegNumDel(actst->actHead->palNo);
}
//----------------------------------------------------------
//	Affine�Ȱ�ެ��폜
//     ���������AOAM�̊g�k�׸ނ�ON�ɂȂ��Ă��Ȃ��ꍇ�͉������Ȃ�
//----------------------------------------------------------
void AffineDelActor(actWork *actst)
{
	if(actst->oamData.AffineMode & 1)
	{
		AffineWorkNumDel(actst->oamData.AffineParamNo);
		actst->oamData.AffineMode = 0;
	}
}

//----------------------------------------------------------
//	�������g���܂߂đS�č폜
//----------------------------------------------------------
void AllDelActor(actWork *actst)
{
	CellDelActor(actst);
	PalDelActor(actst);
	AffineDelActor(actst);
	DelActor(actst);
}












