OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",
		  "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)	/*��:�o��̧�ْ��̍ŏ��Ɏ��s����ݽ�׸��݂��`(̧��:crt0.s)
 /*SEARCH_DIR(/usr/cygnus/armelf-981216/H-i586-cygwin32/thumb-elf/lib);*//*�ύXmatsuda*/
 /*SEARCH_DIR(/usr/cygnus/thumbelf-000512/H-i686-cygwin32/thumb-elf/lib);*/
 /*SEARCH_DIR(/usr/agbact);		matsuda �����DEL*/
/* Dos we need any of these for elf?
   __DYNAMIC = 0;	 */

/* ��add 2001.07.23(��) by matsuda*/
MEMORY
{
	inside_ram : org = 0x03000000, len = 0x8000
	ex_ram : org = 0x02000000, len = 0x40000	/*0x3ff00*/
	ov_ram : org = 0x0203ff00, len = 0x40000 - 0x3ff00
	pokegra_rom : org = 0x867d000, len = 0x400000
}

SECTIONS
{
	/* Read-only sections, merged into text segment: */
	. = 0x8000;	/*��:�������牺���ް������̱��ڽ����z�u����*/
	/*--------------Del matsuda  ��������Ȃ��͂�-----------------------*/
	.interp	  : { *(.interp) 	}
	.hash 		 : { *(.hash)		}
	.dynsym		 : { *(.dynsym)		}
	.dynstr		 : { *(.dynstr)		}
	.gnu.version	 : { *(.gnu.version)	}
	.gnu.version_d   : { *(.gnu.version_d)	}
	.gnu.version_r   : { *(.gnu.version_r)	}
	.rel.text 	 :
	{
		*(.rel.text)
		*(.rel.text.*)
		*(.rel.gnu.linkonce.t*)
	}
	.rela.text	 :
	{
		*(.rela.text)
		*(.rela.text.*)
		*(.rela.gnu.linkonce.t*)
	}
	.rel.data 	 :
	{
		*(.rel.data)
		*(.rel.data.*)
		*(.rel.gnu.linkonce.d*)
	}
	.rela.data	 :
	{
		*(.rela.data)
		*(.rela.data.*)
		*(.rela.gnu.linkonce.d*)
	}
	.rel.rodata	 :
	{
		*(.rel.rodata)
		*(.rel.rodata.*)
		*(.rel.gnu.linkonce.r*)
	}
	.rela.rodata	 :
	{
		*(.rela.rodata)
		*(.rela.rodata.*)
		*(.rela.gnu.linkonce.r*)
	}
	.rel.got		 : { *(.rel.got)		}
	.rela.got 	 : { *(.rela.got)		}
	.rel.ctors	 : { *(.rel.ctors)	}
	.rela.ctors	 : { *(.rela.ctors)	}
	.rel.dtors	 : { *(.rel.dtors)	}
	.rela.dtors	 : { *(.rela.dtors)	}
	.rel.init 	 : { *(.rel.init)	}
	.rela.init	 : { *(.rela.init)	}
	.rel.fini 	 : { *(.rel.fini)	}
	.rela.fini	 : { *(.rela.fini)	}
	.rel.bss		 : { *(.rel.bss)		}
	.rela.bss 	 : { *(.rela.bss)		}
	.rel.plt		 : { *(.rel.plt)		}
	.rela.plt 	 : { *(.rela.plt)		}
	.init 		 : { KEEP (*(.init))	} =0
	.plt		: { *(.plt)	}
	/*---------------------------------------------------------------*/
	
	
	
	
	/*==========================================================
	------------ ROM -------------------------------------------
	==========================================================*/

	.pokegra 0x867d000 :
	{
		pokegra.o(*)
	} > pokegra_rom
	
	.text 	0x08000000 :
	{
		*(.text)
		*(.text.*)
		*(.stub)
		/* .gnu.warning sections are handled specially by elf32.em.  */
		*(.gnu.warning)
		*(.gnu.linkonce.t*)
		*(.glue_7t) *(.glue_7)
	} =0
	_etext = .;
	/*����ma:_etext��PROVIDE�����Ă��Ȃ��̂ŁA������۸��тŒ�`���ꂽ��2�d��`�װ�ɂȂ�*/
	PROVIDE (etext = .);
/*ma:��etext����ق��`�Betext����۸��тŒ�`����Ă�����ݶ�����Ă���۸��тł�2�d��`�װ
  �@�@�ɂȂ邪�APROVIDE�����Ă����ƁAetext����۸��тŒ�`���ꂽ���ݶ�����ē���
  �@�@��`�͖����ƂȂ���۸��тŒ�`���ꂽ�l���L���ɂȂ�B��۸��тŒ�`������etext���Q�Ƃ�
  �@�@�悤�Ƃ�����ݶ�����ĂŒ�`���ꂽ�l���Q�Ƃ����*/
	.fini 	 : { KEEP (*(.fini))		} =0
	
	. = ALIGN(32 / 8);			/*��:����4�޲ċ��E�ɍ��킹�Ă�?*/
	.rodata		. :
	{
		*(.rodata)
		/*a_killer.o(.rodata)*/ /*��:�Ώ�̧�� �S�Ă�̧�ق��Ώۂ̏ꍇ��*(�Ώ��ް���� �S���ް��̏ꍇ��*)*/
		*(.rodata.*)
		*(.gnu.linkonce.r*)
	}
	.rodata1	 : { *(.rodata1) }
	. = ALIGN(32 / 8);			/*��:����4�޲ċ��E�ɍ��킹�Ă�?*/
	
	/*==========================================================
	------------ �I�[�o�[���C�̈� ------------------------------
	==========================================================*/
/*	__load_start_ovdata = . ;	
	OVERLAY 0x0203ff00 : AT(__load_start_ovdata)
	{
		.ov_data0{ ov_fld.o(.data) ov_fld.o(COMMON) }
*/		
		/*!!����!! �V�K�ɵ��ްڲ����݂�ǉ�����ꍇ�͂��̏�ɒǉ����Ă�������!!*/
/*		.ov_data9{ ov_fight.o(.data) ov_fight.o(COMMON) }
	} > ov_ram
	. = ALIGN(32 / 8);
*/
	/*==========================================================
	------------ EX_RAM ----------------------------------------
	==========================================================*/
/*	__load_start_data = . ;		���� change 2002.01.08(��)*/
	__load_start_data = 0x08800000;
/*	__load_start_data = __load_stop_ov_data9;	���̵��ްڲ����ı�Ă��Ă���̂�*/

	/*��:�����l��ROM�ɒu���Ă����Ďg���Ƃ���RAM�ɺ�߰*/
	.data 0x02000000 : AT (__load_start_data)	/*�����l�t�̸�۰��ٕϐ��͊O��RAM��*/
	{
		/* add 01/06/20 */
		decord.o(.data)	
		/*. = 0x28000;	changed 2001.12.21(��)*/
		. = 0x20000;
		
		ex_*.o(COMMON)/*��:COMMON��t���Ă����Ƹ�۰��ق�����z�u���Ă����*/
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
		CONSTRUCTORS
	} > ex_ram
	__load_stop_data = .;
	.data1   : { *(.data1) }
	.ctors		 :
	{
	/* gcc uses crtbegin.o to find the start of the constructors, so
	   we make sure it is first.  Because this is a wildcard, it
	   doesn't matter if the user does not actually link against
	   crtbegin.o; the linker won't look for a file to match a
	   wildcard.  The wildcard also means that it doesn't matter which
	   directory crtbegin.o is in.	*/
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
	}
	.dtors		 :
	{
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
	}
	.got			 : { *(.got.plt) *(.got) }
	.dynamic		 : { *(.dynamic) }
  /* We want the small data sections together, so single-instruction offsets
	 can access them all, and initialized data all before uninitialized, so
	 we can shorten the on-disk segment size.  */
	.sdata		:
	{
		*(.sdata) 
	}
	_edata  =  .;
	PROVIDE (edata = .);
	
	/*==========================================================
	------------ bss -------------------------------------------
	==========================================================*/
	. = 0x03000000;			/*����ܰ�RAM*/
	__bss_start = .;
	.bss		 :
	{
		*(.dynbss)
		*(.bss)
		*(.bss.*)
		*(COMMON)			/*��:bss1�ȊO��COMMON�ް�����������z�u ���ꂪ�Ȃ��Ƒ���COMMON��
   							bss1��COMMON�̂����ォ�炻�̂܂ܔz�u����Ă��܂�*/
	   /* Align here to ensure that the .bss section occupies space up to
    	  _end.  Align after .bss to ensure correct alignment even if the
	      .bss section disappears because there are no input sections.  */
		. = ALIGN(32 / 8);
	} > inside_ram
	.sbss 	 : { *(.sbss) *(.scommon) }	
	__bss_end = .;


/*----------------------------------------------------------------------*/









	
	
	
	
	
	
	
	
	
	
	
	
	
	. = ALIGN(32 / 8);
	_end = . ;
	_bss_end__ = . ; __bss_end__ = . ;
	PROVIDE (end = .);
	/* Stabs debugging sections.	*/
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	/* DWARF debug sections.
	 Symbols in the DWARF debugging sections are relative to the beginning
	 of the section so we begin them at 0.	*/
	/* DWARF 1 */
	.debug		  0 : { *(.debug) }
	.line 		  0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info	  0 : { *(.debug_info) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line	  0 : { *(.debug_line) }
	.debug_frame	  0 : { *(.debug_frame) }
	.debug_str	  0 : { *(.debug_str) }
	.debug_loc	  0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	.stack 0x80000 : { _stack = .; *(.stack) }
	/* These must appear regardless of  .  */
}

