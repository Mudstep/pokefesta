//========================================================================================
//									�����̃p�\�R������
//								�@2002/06/25 by nakahiro
//========================================================================================
#ifndef	_MY_PC_H
#define _MY_PC_H


typedef struct {

	u8	pos;		// �I���ʒu
	u8	pos_max;	// �I���J�[�\���ő�l
	u8	scr;		// �X�N���[���J�E���^�[
	u8	max;		// �A�C�e����

	u8	type;		// ����a����̃��j���[�^�C�v
	u8	cng_pos;	// �����ʒu
	u8	cng_flg;	// �����p�t���O

	u16	num;		// ��

}MYPC_DATA;

#define	MYPC_SYS	( (MYPC_DATA *)&UserWork[ 0x1fe00] )


extern void MyPC_ReturnMenu( u8 id );		// ���j���[�I���֖߂�
extern void MyPC_ItemPutReturn(void);		// �o�b�O���畜�A
extern void MyPC_DefItemSet(void);			// �f�t�H���g�A�C�e���Z�b�g
extern void MyPC_MailPokeSetReturn(void);	// �|�P�������X�g����̖߂��


#endif	// _MY_PC_H
