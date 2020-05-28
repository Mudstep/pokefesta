//=========================================================================
//
//
// ���z�C���^�[�v���^�}�V���@���C������
//
//	���X�N���v�g�R�}���h���s�Ɏg�p�����
//
//
//	2001.11.7 by tama	script.c ����R�A�����̂ݐ؂藣����
//
//=========================================================================

#include "common.h"

#define __VM_H_GLOBAL
#include "vm.h"



//=========================================================================
//	��`
//=========================================================================
// ���z�}�V���̓����Ԓ�`
enum{
	VMSTAT_READY,		// ��~(����I���j
	VMSTAT_RUN,			// ���쒆
	VMSTAT_WAIT,		// �҂���ԁi�`�F�b�N���[�`���Ăяo���j
};

const u32 ScriptBreakPoint = 0;

//=========================================================================
//
//
// ���z�}�V���p�֐�
//
//
//=========================================================================

//-------------------------------------------------------------------------
// ���z�}�V��������
//	<����>	core		���z�}�V������\���̂ւ̃|�C���^
//			start		���߃e�[�u���J�n�A�h���X
//			end			���߃e�[�u���I���A�h���X
//-------------------------------------------------------------------------

void VM_Init(VM_MACHINE * core, const VM_CMD * start, const VM_CMD * end )
{
	int i;

	core->status = VMSTAT_READY;
	core->PC = NULL;
	core->SP = 0;
	core->routine = NULL;
	core->command_table = start;
	core->command_table_end = end;
	for( i=0; i<VM_REG_MAX; i++ ){ core->reg[i] = 0; }

	for( i=0; i<VM_STACK_MAX; i++ ){ core->array[i]=NULL; }
}


//-------------------------------------------------------------------------
//�@���z�}�V���ɃR�[�h��ݒ�
//	<����>	core		���z�}�V������\���̂ւ̃|�C���^
//			start		���s�R�[�h�̊J�n�A�h���X
//-------------------------------------------------------------------------
u8 VM_Start(VM_MACHINE * core, const VM_CODE * start)
{
	core->PC = start;
	core->status = VMSTAT_RUN;
	return TRUE;
}

//-------------------------------------------------------------------------
//	���z�}�V�����E�F�C�g��Ԃɐݒ�
//	<����>	core		���z�}�V������\���̂ւ̃|�C���^
//			func		�E�F�C�g�֐�
//	��TRUE���Ԃ��Ă���܂ŃE�F�C�g�֐��𖈉�Ăт���
//-------------------------------------------------------------------------
void VM_SetWait( VM_MACHINE * core, u8(*func)(void) )
{
	core->status = VMSTAT_WAIT;
	core->routine = func;
}

//-------------------------------------------------------------------------
//	���z�}�V�����s�I��
//	<����>	core		���z�}�V������\���̂ւ̃|�C���^
//-------------------------------------------------------------------------
void VM_End(VM_MACHINE * core)
{
	core->status = VMSTAT_READY;
	core->PC = NULL;
}

//-------------------------------------------------------------------------
//�@���z�}�V�����䃁�C��
//	<����>	core		���z�}�V������\���̂ւ̃|�C���^
//	<�߂�l>
//		TRUE			���s��
//		FALSE			��~���E���s�I��
//-------------------------------------------------------------------------


u8 VM_Control(VM_MACHINE * core)
{
	u8 code,ret;

	if ( core->status == VMSTAT_READY ) return FALSE;

	switch ( core->status ) {

		case VMSTAT_READY:
			return FALSE;

		case VMSTAT_WAIT:
			if ( core->routine != NULL ) {
				if ( core->routine() == TRUE ) {
					core->status = VMSTAT_RUN;
				}
				return TRUE;
			}
			core->status = VMSTAT_RUN;
			/* FALL THROUGH */

		case VMSTAT_RUN:
			while ( TRUE ) {
				if ( core->PC == NULL ) {
					core->status = VMSTAT_READY;
					return FALSE;
				}
				if ( core->PC == (VM_CODE *)ScriptBreakPoint ) {
					while( TRUE ) {
						Halt();
					}
				}

				code = VMGetU8(core);
				if ( &(core->command_table[code]) >= core->command_table_end ) {
					core->status = VMSTAT_READY;
					return FALSE;
				}
				ret = core->command_table[code](core);
				if ( ret == 1 ){
					break;
				}
			}
	}
	return TRUE;
}


//=========================================================================
// ���z�}�V������p�T�u���[�`��
//=========================================================================

//-------------------------------------------------------------------------
// ���z�}�V���X�^�b�N�v�b�V��
//-------------------------------------------------------------------------
u8 VMStackPush(VM_MACHINE * core, const VM_CODE * val)
{
	if(core->SP + 1 >= VM_STACK_MAX)return 1;
	core->array[core->SP] = val;
	core->SP ++;
	return 0;
}

//-------------------------------------------------------------------------
// ���z�}�V���X�^�b�N�|�b�v
//-------------------------------------------------------------------------
const VM_CODE * VMStackPop(VM_MACHINE * core)
{
	if(core->SP == 0)return NULL;
	core->SP --;
	return core->array[core->SP];
}

//-------------------------------------------------------------------------
//	���z�}�V���W�����v����
//-------------------------------------------------------------------------
void VMJump( VM_MACHINE * core, VM_CODE * adrs )
{
	core->PC = adrs;
}

//-------------------------------------------------------------------------
//	���z�}�V���R�[������
//-------------------------------------------------------------------------
void VMCall( VM_MACHINE * core, VM_CODE * adrs )
{
	VMStackPush(core,core->PC);
	core->PC = adrs;
}

//-------------------------------------------------------------------------
//	���z�}�V�����^�[������
//-------------------------------------------------------------------------
void VMRet( VM_MACHINE * core )
{
	core->PC = VMStackPop(core);
}


//-------------------------------------------------------------------------
//	PC�̃A�h���X����16bit(2byte)�f�[�^�擾
// 
//-------------------------------------------------------------------------
u16 VMGetU16(VM_MACHINE * core)
{
	u16 val;
	val = (u16)VMGetU8(core);
	val += (u16)VMGetU8(core) << 8;
	return val;
}

//-------------------------------------------------------------------------
//	PC�̃A�h���X����32bit(4byte)�f�[�^�擾
// 
//-------------------------------------------------------------------------
u32 VMGetU32(VM_MACHINE * core)
{
	u32 val;
	u8 a,b,c,d;
	a = VMGetU8(core);
	b = VMGetU8(core);
	c = VMGetU8(core);
	d = VMGetU8(core);
	val = 0;
	val += (u32)d;
	val <<= 8;
	val += (u32)c;
	val <<= 8;
	val += (u32)b;
	val <<= 8;
	val += (u32)a;
	return val;
}

