/*
** ###################################################################
**     Processors:          LPC804M101JDH20
**                          LPC804M101JDH24
**                          LPC804M101JHI33
**                          LPC804M111JDH24
**                          LPC804UK
**
**     Compiler:            GNU C Compiler
**     Reference manual:    LPC804 User manual Rev.1.0  24 Jan 2018
**     Version:             rev. 1.0, 2018-01-09
**     Build:               b200408
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2020 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/



/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x040;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x040;

/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000200
  m_crp                 (RX)  : ORIGIN = 0x000002FC, LENGTH = 0x00000004
  m_app0                (RX)  : ORIGIN = 0x00000300, LENGTH = 0x00003A80
  m_app1                (RX)  : ORIGIN = 0x00003D80, LENGTH = 0x00003A80/*inicia em 15744 e finalizaem 30719 com tamanho 14976*/
  m_eeprom              (RX)  : ORIGIN = 0x00007800, LENGTH = 0x00000380/*inicia em 30720 e finalizaem 31615 com tamanho 896*/ 
  m_reg_factory         (RX)  : ORIGIN = 0x00007B80, LENGTH = 0x00000380/*inicia em 31616 e finalizaem 32512 com tamanho 896*/
  m_data                (RW)  : ORIGIN = 0x10000000, LENGTH = 0x00000FE0
}

/* Define output sections */
SECTIONS
{
  /* Factory defaults (const): será incluído no .hex e gravado pelo programador */
  .nv_defaults :
  {
    . = ALIGN(4);
    __nv_defaults_start__ = .;
    KEEP(*(.nv_defaults))     /* mantém mesmo se não-referenciado */
    . = ALIGN(4);
    __nv_defaults_end__ = .;
  } > m_reg_factory
  
  /* EEPROM simulada em Flash:
     - NOLOAD para não ir no .hex pelo linker
     - Alinhada a 64B (tamanho de página do LPC804)
     - Colocada na região m_eeprom (0x7800..0x7FFF)
     - Posicionada explicitamente em 0x00007C00 (backup/ativos), opcional
  */
  .eeprom_storage 0x00007C00 (NOLOAD) :
  {
      . = ALIGN(64);
      __eeprom_storage_start__ = .;
      KEEP(*(.eeprom_storage*))   /* ex.: uint8_t eeprom_area[...] */
      . = ALIGN(64);
      __eeprom_storage_end__   = .;
  } > m_eeprom

  /* The startup code goes first into internal flash */
  .interrupts :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  .crp :
  {
    . = ALIGN(4);
    KEEP(*(.crp))    /* Code Read Protection level (CRP) */
    . = ALIGN(4);
  } > m_crp

  /* The program code and other data goes into internal flash */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_app0

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_app0

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_app0


 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_app0

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_app0

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_app0

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_app0

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_app0

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.ramfunc*)             /* for functions in ram */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_app0) + LENGTH(m_app0);
  ASSERT(__DATA_END <= text_end, "region m_app0 overflowed with text and data")

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")

}

__app0_start      = 0x00000300;
__app0_end        = 0x00003D7F;
__app1_start      = 0x00003D80;
__app1_end        = 0x000077FF;
__eeprom_start    = 0x00007800;
__reg_factory     = 0x00007B80;
__flash_end       = 0x00007F00;
