//**********************************************************************
//		evobjcmd.c
//**********************************************************************
#ifndef EVOBJCMD_H
#define	EVOBJCMD_H

#undef	EXT
#ifdef 	EVOBJCMD_H_PROTO
#define	EXT	/**/
#else
#define	EXT extern
#endif

EXT u8 AnmCmdSet(u8,u8,u8,ACMD_CODE*);						// ��Ұ��ݺ���޾��
EXT u8 AnmCmdEnd(u8,u8,u8);									// ��Ұ��ݺ���ޏI������
EXT void AnmCmdFree(void);									// ��Ұ��ݺ���ފJ��

#undef EXT
#endif // EVOBJCMD_H
