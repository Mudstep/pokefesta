
/* �������ׂ�����@�\���g�p�������ꍇ�A */
/* �����1�ɂ��Ĉȉ���L���ɂ��Ă��������B */
/* �����łȂ��ꍇ��0�ɂ��Ă����Ă������� */
#if	0
#define DEBUG_WATCH
extern unsigned short LYstart;
extern unsigned short LYend;
extern unsigned short LYVstart;
extern unsigned short LYVend;
extern void MActorSet(void);

#define	DEBUG_WATCH_START()	{ LYstart= (*(vu16 *)REG_VCOUNT)&0x00ff; }

#define DEBUG_WATCH_END()	{ LYend  = (*(vu16 *)REG_VCOUNT)&0x00ff; }

#else

#define DEBUG_WATCH_START()
#define DEBUG_WATCH_END()

#endif

