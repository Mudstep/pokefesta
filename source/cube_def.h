#ifndef	__CUBE_DEF_H__
#define	__CUBE_DEF_H__

//==============================================================
//																
//	�L���[�u�\���̒�`
//
//==============================================================

typedef struct CUBE_TAG{
	u8	type;			/* �L���[�u�̎�� */
	u8	karai;			/* �h��		�i�ԁj	0 �` 255 */
	u8	sibui;			/* �a��		�i�j	0 �` 255 */
	u8	amai;			/* �Â�		�i���j	0 �` 255 */
	u8	nigai;			/* �ꂳ		�i�΁j	0 �` 255 */
	u8	suppai;			/* �_���ς�	�i���j	0 �` 255 */
	u8	umai;			/* �|��				0 �` 255 */
}CUBE;

#define		MY_CUBE_MAX		40

#endif /*__CUBE_DEF_H__*/
