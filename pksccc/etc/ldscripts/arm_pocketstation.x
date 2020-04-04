/*
 * linker script for pocketstation (arm)
 * ポケットステーション用 リンカスクリプト
 */
OUTPUT_ARCH(arm)

MEMORY
{
  /* 0x200 から始めるとexitのメニューがでてくる? */
  ram : o = 0x00000204, l = 0x5fc
  rom : o = 0x02000000, l = 0x2000
}

SECTIONS
{
  /*
   * 使用するブロック数を計算する
   * 15を超えるとエラー
   */
  __frame_size = 0x80;
  __block_size = (__frame_size * 64);
  __rom_size = (__rom_end - __rom_start);
  __block_num = (__rom_size - 1) / __block_size + 1;
  ASSERT((__block_num <= 15), "ERROR: over 15 blocks")

  .text :
  {
    __rom_start = .;

    *(scheader)
    . = ALIGN(__frame_size);  /* frameを跨がないようにする */
    *(.text)
    *(.strings)
    *(.rodata*)
    *(.rdata)
    *(.glue_7)
    *(.glue_7t)
    _etext = .;
  } > rom

  .data :
  {
    *(.data)
    __rom_end = .;
    _edata = .;
  } > rom

  .bss :
  {
    _bss_start = .;
    *(.bss)
    *(COMMON)
    _end = .;
  } > ram
}
