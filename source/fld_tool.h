
//=========================================================================
//	
//=========================================================================

#ifndef FLD_TOOL_H
#define FLD_TOOL_H

#include "map.h"

#undef GLOBAL
#ifdef FLD_TOOL_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================

typedef struct{
	u8 form;
	u8 site;
}HERO_INIT_INFO;

//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;

extern MapID BeforeMapID;

// �}�b�v�؂�ւ��^�C�~���O�ł����L���Ȓl�������Ă��Ȃ��̂�
// �A�N�Z�X����ۂ͗v���ӁI �ł���΃A�N�Z�X����ׂ��łȂ�
extern MapID NextMapID;

//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

//-----------------------------------------------------------------
//	SXY����֘A
//-----------------------------------------------------------------
GLOBAL void SxyObjDataSet( void );
GLOBAL void SxyObjDataRecover( void );
GLOBAL void SxyObjDataPosChange( u8 id, s16 x, s16 y );
GLOBAL void SxyObjDataSiteChange( u8 id, u8 site );

//-----------------------------------------------------------------------------
//	�}�b�v�f�[�^�ݒ�֘A
//-----------------------------------------------------------------------------
GLOBAL void SetNowMapData(void);
GLOBAL void RecoverNowMapData( void );
GLOBAL void InitNowMapVisual(void);
GLOBAL void ChangeNowMapScreen( u16 id );	//�}�b�v�X�N���[����ύX


//-----------------------------------------------------------------
//	�}�b�v�J�ڎ��̃f�[�^������
//	<����>		�ʐM���[�h�����ǂ����̎w��
//-----------------------------------------------------------------
GLOBAL void FieldInitData(int is_comm_mode);

//-----------------------------------------------------------------
//	�t�B�[���h�ł̗אڃ}�b�v�ړ����̏���
//-----------------------------------------------------------------
GLOBAL void ChangeMapData(u8 div_id, u8 map_id);

//-----------------------------------------------------------------------------
//	�}�b�v�J�ڊ֘A
//-----------------------------------------------------------------------------
GLOBAL void NextMapSetDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y);
GLOBAL void NextMapSetByID(s8 div_id, s8 map_id, s8 exit_id);
GLOBAL void NextMapSetBySpID( u8 sp_id );
GLOBAL void SetSpecialMapID( u8 sp, s8 div_id, s8 map_id, s8 exit_id );
GLOBAL void SetSpecialMapDirect( u8 sp,
		s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y ); // ����ڑ��w��̃Z�b�g
GLOBAL void SetNextWorldPosition(void);

GLOBAL void EscapeMapSetDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y);
GLOBAL void TeleportMapSetByWarpID( u8 id );	//�e���|�[�g�ʒu�����[�v�h�c�ŃZ�b�g
GLOBAL void NextMapSetByTelMapID( void );		//�e���|�[�g�ʒu�ɑJ��
GLOBAL void NextMapSetByWarpID( u8 id );		//���[�v�h�c�̈ʒu�ɑJ��
GLOBAL void NextMapSetByWarpMapID( s8 div_id, s8 map_id );
												//���[�v�ʒu�ɑJ�ځi�}�b�v�h�c�Ŏw��j
GLOBAL void NextMapSetByEscMapID( void );		//���Ȃʂ��̈ʒu�ɑJ��
GLOBAL void SetDivingMapDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y );
												//�_�C�r���O�ʒu���w��
GLOBAL void NextMapSetByDivingID( void );		//�_�C�r���O�w��ʒu�ɑJ��

GLOBAL u8 NextMapSetAbove( u16 x, u16 y );		//�V�����̃}�b�v�ɑJ��
GLOBAL u8 NextMapSetBelow( u16 x, u16 y );		//�n�����̃}�b�v�ɑJ��

GLOBAL void SetPitfallMapDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y );
GLOBAL void NextMapSetByPitfall( s16 x, s16 y );

//�R���e�B�j���[���̓��ʃ}�b�v�J�ڊ֘A
GLOBAL void SetContinueMapDirect( s8 div_id, s8 map_id, s8 exit_id, s8 x, s8 y );
GLOBAL void SetContinueMapByWarpID( u8 id );
GLOBAL void SetContinueMapBySpID( u8 id );
GLOBAL void NextMapSetByContinueMapID( void );

//	NextMapID/Fld.MapID���炠�Ȃʂ��J�ڐ���Z�b�g
//	�����̑J�ڐ��ݒ肵������ɌĂԂ���
GLOBAL void NextEscapePosSet(s16 x, s16 y);

//-------------------------------------------------------------
//	���@��Ԃ̐���֘A
//-------------------------------------------------------------
//���@�`�ԏ���������
GLOBAL void InitHeroLastStatus(void);
//���݂̎��@��ԏ���ۑ�
GLOBAL void SetHeroLastStatus( void );
//�}�b�v�o�����̎��@��ԏ����擾
GLOBAL const HERO_INIT_INFO * GetHeroFirstStatus(void);
//�}�b�v�o�����̃A�g���r���[�g���擾
GLOBAL u16 GetFirstAttr( void );


GLOBAL int IsEnableCycleNowMap(void);	//���]�Ԃɏ���}�b�v���ǂ����̃`�F�b�N


//-----------------------------------------------------------------
//	�C�x���g�\���̊֘A
//-----------------------------------------------------------------
GLOBAL const MapTotalData * GetTotalMapData(u16 div_id, u16 map_id);
GLOBAL const MapTotalData * GetNextMapData( void );


//-----------------------------------------------------------------
//	���邳�֘A
//-----------------------------------------------------------------
GLOBAL void InitBright(void);
GLOBAL void SetBright( int r );
GLOBAL u8 GetBright(void);


#undef GLOBAL

#include "fld_bgm.h"

#endif	/*FLD_TOOL_H*/

