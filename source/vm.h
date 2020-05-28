
//=========================================================================
//	
//=========================================================================

#ifndef __VM_H
#define __VM_H

#undef GLOBAL
#ifdef __VM_H_GLOBAL
#define GLOBAL /*	*/
#else
#define GLOBAL extern
#endif

//=========================================================================
//	��`
//=========================================================================
//#define name value
//typedef ...

//-------------------------------------------------------------------------
// ���z�}�V���\���̐錾
//-------------------------------------------------------------------------

#define VM_STACK_MAX	20	// ���z�}�V���̃X�^�b�N�̐[��
#define	VM_REG_MAX		4	// ���z�}�V���̃��W�X�^�̐�

typedef struct VM_MACHINE_ST * VM_CORE_P;
typedef u8 ( * VM_CMD)(VM_CORE_P);

typedef struct VM_MACHINE_ST{
		u8	SP;							// �X�^�b�N�|�C���^
		u8 status;						// ������
		u8 cmp_flag;					// �v�Z���ʃt���O
		u8 (*routine)(void);
		const VM_CODE * PC;				// ���݂̃X�N���v�g�A�h���X
		const VM_CODE * array[VM_STACK_MAX];	// �X�^�b�N
		const VM_CMD * command_table;
		const VM_CMD * command_table_end;
		u32 reg[VM_REG_MAX];			// �ėp���W�X�^
}VM_MACHINE;



//=========================================================================
//	�ϐ�
//=========================================================================
//GLOBAL type valiable;


//=========================================================================
//	�֐��v���g�^�C�v
//=========================================================================
//GLOBAL type function(arg...);

//-------------------------------------------------------------------------
//�֐��v���g�^�C�v:���z�}�V������p
//-------------------------------------------------------------------------

GLOBAL void VM_Init(VM_MACHINE * core, const VM_CMD * start, const VM_CMD * end );
GLOBAL u8 VM_Start(VM_MACHINE * core,const VM_CODE * start);
GLOBAL u8 VM_Control(VM_MACHINE * core);
GLOBAL void VM_SetWait( VM_MACHINE * core, u8(*func)(void) );
GLOBAL void VM_End(VM_MACHINE * core);

//-------------------------------------------------------------------------
// �X�^�b�N����
//-------------------------------------------------------------------------
GLOBAL u8 VMStackPush(VM_MACHINE * core, const VM_CODE * val);
GLOBAL const VM_CODE * VMStackPop(VM_MACHINE * core);

//-------------------------------------------------------------------------
//	���򖽗�
//-------------------------------------------------------------------------
GLOBAL void VMJump( VM_MACHINE * core, VM_CODE * adrs );
GLOBAL void VMCall( VM_MACHINE * core, VM_CODE * adrs );
GLOBAL void VMRet( VM_MACHINE * core );

//-------------------------------------------------------------------------
//	�f�[�^�擾
//-------------------------------------------------------------------------
//	u8�T�C�Y�̃f�[�^���擾���邽�߂̃}�N��
#define VMGetU8(core)	(*((core)->PC++))

GLOBAL u16 VMGetU16(VM_MACHINE * core);

GLOBAL u32 VMGetU32(VM_MACHINE * core);


#undef GLOBAL
#endif	/*__VM_H*/

