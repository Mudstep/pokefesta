#ifndef	__EVWKDEF_H__
#define	__EVWKDEF_H__

//====================================================================
//
//	�Z�[�u�t���O�E���[�N�̈��`
//
//====================================================================

// �t���O�֘A��`
#define	SVFLG_START			0x0000			// �Z�[�u�t���O�X�^�[�g�i���o�[
#define	SCFLG_START			0x4000			// ����t���O�X�^�[�g�i���o�[


#define USRFLAG_START		0
#define USRFLAG_MAX			2048				/* �X�N���v�g��`�̃t���O�ő吔 */
#define USRFLAG_AREA_MAX	(USRFLAG_MAX/8)

#define SYSFLAG_START		(USRFLAG_MAX)
#define	SYSFLAG_MAX			192					/* �V�X�e����`�̃t���O�ő吔 */
#define	SYSFLAG_AREA_MAX	(SYSFLAG_MAX/8)

#define TIMEFLAG_START		(USRFLAG_MAX+SYSFLAG_MAX)
#define TIMEFLAG_MAX		64					/* ���Ԍo�߃C�x���g�p�t���O�ő吔*/
#define	TIMEFLAG_AREA_MAX	(TIMEFLAG_MAX/8)

#define	EVFLAG_MAX			(USRFLAG_MAX+SYSFLAG_MAX+TIMEFLAG_MAX)
#define	EVFLAG_AREA_MAX		(EVFLAG_MAX/8)

#define TRFLG_START			1280				/* �g���[�i�[ID�Ή��t���O�J�n�ʒu */


//�Z�[�u���Ȃ��t���O�֘A��`
#define	CTRLFLAG_MAX		128					/* ����t���O�ő吔 */
#define	CTRLFLAG_AREA_MAX	(CTRLFLAG_MAX/8)


//	�Z�[�u���[�N�֘A��`
#define SYSWORK_MAX			80
#define EVWORK_AREA_MAX		256

#define	SVWK_START			0x4000					// �Z�[�u���[�N��`�X�^�[�g�i���o�[
#define	SCWK_START			0x8000					// �f�[�^�����E�ėp���[�N��`�X�^�[�g�i���o�[


#endif	/*__EVWKDEF_H__*/
