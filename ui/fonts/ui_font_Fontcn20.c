/*******************************************************************************
 * Size: 20 px
 * Bpp: 1
 * Opts: --bpp 1 --size 20 --font C:/Users/19178/SquareLine/assets/adobesongstd-light.otf -o C:/Users/19178/SquareLine/assets\ui_font_Fontcn20.c --format lvgl -r 0x20-0x7f --symbols 个人信息诊断意见监护记录用药记录 --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_FONTCN20
#define UI_FONT_FONTCN20 1
#endif

#if UI_FONT_FONTCN20

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xaa, 0xaa, 0xa0, 0xf0,

    /* U+0022 "\"" */
    0x99, 0x99, 0x90,

    /* U+0023 "#" */
    0x9, 0x4, 0x84, 0x42, 0x47, 0xf8, 0x90, 0x48,
    0x48, 0xff, 0x12, 0x9, 0x8, 0x84, 0x80,

    /* U+0024 "$" */
    0x8, 0x8, 0x3f, 0x49, 0x49, 0x48, 0x28, 0x1c,
    0xe, 0x9, 0x9, 0x89, 0x4a, 0x3c, 0x8, 0x8,

    /* U+0025 "%" */
    0x30, 0x21, 0x21, 0x8, 0x44, 0x21, 0x20, 0x85,
    0x2, 0x14, 0xc4, 0xa4, 0x8c, 0xa1, 0x4, 0x84,
    0x12, 0x10, 0x88, 0x44, 0x22, 0x10, 0x30, 0x0,
    0x0,

    /* U+0026 "&" */
    0x1c, 0x1, 0x10, 0x8, 0x80, 0x44, 0x2, 0x40,
    0xc, 0x78, 0x70, 0x8c, 0xc8, 0x83, 0x44, 0xc,
    0x20, 0x31, 0x6, 0x84, 0x26, 0x1e, 0x18,

    /* U+0027 "'" */
    0x44, 0xa0,

    /* U+0028 "(" */
    0x12, 0x44, 0x48, 0x88, 0x88, 0x88, 0x44, 0x42,
    0x10,

    /* U+0029 ")" */
    0x84, 0x22, 0x21, 0x11, 0x11, 0x11, 0x22, 0x24,
    0x80,

    /* U+002A "*" */
    0x12, 0x45, 0xf, 0x52, 0x40, 0x0,

    /* U+002B "+" */
    0x8, 0x4, 0x2, 0x1, 0x0, 0x87, 0xfc, 0x20,
    0x10, 0x8, 0x4, 0x2, 0x0,

    /* U+002C "," */
    0x96,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x8, 0x84, 0x21, 0x10, 0x84, 0x42, 0x11, 0x8,
    0x42, 0x0,

    /* U+0030 "0" */
    0x18, 0x66, 0x42, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x42, 0x42, 0x3c,

    /* U+0031 "1" */
    0x27, 0x8, 0x42, 0x10, 0x84, 0x21, 0x8, 0x47,
    0x0,

    /* U+0032 "2" */
    0x3c, 0x46, 0x2, 0x2, 0x2, 0x4, 0x4, 0x8,
    0x10, 0x10, 0x20, 0x42, 0xfe,

    /* U+0033 "3" */
    0x38, 0x88, 0x10, 0x20, 0x87, 0x81, 0x81, 0x2,
    0x4, 0x14, 0x4f, 0x0,

    /* U+0034 "4" */
    0x4, 0x4, 0xc, 0x14, 0x24, 0x44, 0x44, 0x84,
    0xff, 0x4, 0x4, 0x4, 0xe,

    /* U+0035 "5" */
    0x7c, 0x81, 0x4, 0xb, 0x88, 0x80, 0x81, 0x2,
    0x4, 0x14, 0x4f, 0x0,

    /* U+0036 "6" */
    0x3, 0xc, 0x30, 0x60, 0x40, 0xdc, 0xa2, 0x81,
    0x81, 0x81, 0x81, 0x42, 0x3c,

    /* U+0037 "7" */
    0xfe, 0x82, 0x2, 0x4, 0x4, 0x8, 0x8, 0x10,
    0x10, 0x20, 0x20, 0x60, 0x40,

    /* U+0038 "8" */
    0x39, 0x8a, 0xc, 0x1c, 0x2f, 0x8e, 0x22, 0x83,
    0x6, 0xe, 0x23, 0x80,

    /* U+0039 "9" */
    0x10, 0x8a, 0x1c, 0x18, 0x30, 0x50, 0x9f, 0x4,
    0x8, 0x20, 0x86, 0x0,

    /* U+003A ":" */
    0xf0, 0x3, 0xc0,

    /* U+003B ";" */
    0xf0, 0xb, 0x58,

    /* U+003C "<" */
    0x0, 0xc0, 0xc0, 0x40, 0x60, 0x60, 0x20, 0x6,
    0x0, 0x60, 0x6, 0x0, 0x60, 0x4,

    /* U+003D "=" */
    0xff, 0xc0, 0x0, 0x0, 0x0, 0xff, 0xc0,

    /* U+003E ">" */
    0x80, 0x18, 0x1, 0x80, 0x18, 0x1, 0x0, 0x30,
    0x10, 0x18, 0x18, 0x18, 0x8, 0x0,

    /* U+003F "?" */
    0x64, 0x82, 0x11, 0x11, 0x8, 0x41, 0x0, 0x2,
    0x10,

    /* U+0040 "@" */
    0x7, 0xc0, 0xc1, 0x8, 0x4, 0x83, 0x94, 0x68,
    0xc2, 0x46, 0x22, 0x31, 0x31, 0x8a, 0x94, 0x54,
    0xb3, 0x38, 0x80, 0x2, 0x0, 0xf, 0xc0,

    /* U+0041 "A" */
    0x2, 0x0, 0x18, 0x1, 0x40, 0xa, 0x0, 0x88,
    0x4, 0x40, 0x22, 0x1, 0xf8, 0x10, 0x40, 0x82,
    0x8, 0x8, 0x40, 0x47, 0x7, 0x0,

    /* U+0042 "B" */
    0x7f, 0x8, 0x62, 0x8, 0x82, 0x20, 0x88, 0x43,
    0xe0, 0x86, 0x20, 0x48, 0x12, 0x4, 0x81, 0x20,
    0x9f, 0xc0,

    /* U+0043 "C" */
    0x7, 0xe3, 0x82, 0x40, 0x24, 0x1, 0x80, 0x8,
    0x0, 0x80, 0x8, 0x0, 0x80, 0x8, 0x0, 0x40,
    0x16, 0x1, 0x30, 0x20, 0xfe,

    /* U+0044 "D" */
    0x7f, 0x81, 0x3, 0x8, 0x4, 0x40, 0x22, 0x0,
    0x90, 0x4, 0x80, 0x24, 0x1, 0x20, 0x9, 0x0,
    0x88, 0x4, 0x40, 0x42, 0x4, 0x3f, 0xc0,

    /* U+0045 "E" */
    0x7f, 0x88, 0x22, 0x8, 0x80, 0x20, 0x8, 0x2,
    0x10, 0xfc, 0x21, 0x8, 0x2, 0x0, 0x81, 0x20,
    0x5f, 0xe0,

    /* U+0046 "F" */
    0x7f, 0x90, 0x48, 0x24, 0x12, 0x1, 0x0, 0x84,
    0x7e, 0x21, 0x10, 0x8, 0x4, 0x2, 0x3, 0x80,

    /* U+0047 "G" */
    0x7, 0xe1, 0xc1, 0x90, 0x4, 0x80, 0x8, 0x0,
    0x40, 0x2, 0x0, 0x10, 0x1f, 0x80, 0x14, 0x0,
    0x90, 0x4, 0x40, 0x21, 0x1, 0x7, 0xf0,

    /* U+0048 "H" */
    0x70, 0x38, 0x80, 0x42, 0x1, 0x8, 0x4, 0x20,
    0x10, 0x80, 0x42, 0x1, 0xf, 0xfc, 0x20, 0x10,
    0x80, 0x42, 0x1, 0x8, 0x4, 0x20, 0x11, 0xc0,
    0xe0,

    /* U+0049 "I" */
    0x71, 0x8, 0x42, 0x10, 0x84, 0x21, 0x8, 0x42,
    0x38,

    /* U+004A "J" */
    0x71, 0x8, 0x42, 0x10, 0x84, 0x21, 0x8, 0x42,
    0x11, 0x8, 0x80,

    /* U+004B "K" */
    0x70, 0xe2, 0x8, 0x21, 0x2, 0x20, 0x24, 0x2,
    0x80, 0x38, 0x2, 0xc0, 0x26, 0x2, 0x20, 0x21,
    0x2, 0x8, 0x20, 0x47, 0x7,

    /* U+004C "L" */
    0x70, 0x8, 0x2, 0x0, 0x80, 0x20, 0x8, 0x2,
    0x0, 0x80, 0x20, 0x8, 0x2, 0x0, 0x82, 0x20,
    0x9f, 0xe0,

    /* U+004D "M" */
    0x70, 0x3, 0x18, 0x3, 0xc, 0x1, 0x85, 0x1,
    0x42, 0x80, 0xa1, 0x60, 0x90, 0x90, 0x48, 0x4c,
    0x44, 0x22, 0x22, 0x11, 0xa1, 0x8, 0x50, 0x84,
    0x28, 0x42, 0x8, 0x23, 0x84, 0x38,

    /* U+004E "N" */
    0x60, 0x38, 0x80, 0x43, 0x1, 0xa, 0x4, 0x24,
    0x10, 0x90, 0x42, 0x21, 0x8, 0x44, 0x20, 0x90,
    0x82, 0x42, 0x5, 0x8, 0xc, 0x20, 0x31, 0xc0,
    0x40,

    /* U+004F "O" */
    0xf, 0x81, 0x83, 0x10, 0xc, 0x80, 0x28, 0x0,
    0xc0, 0x6, 0x0, 0x30, 0x1, 0x80, 0xc, 0x0,
    0x50, 0x4, 0xc0, 0x43, 0x6, 0x7, 0xc0,

    /* U+0050 "P" */
    0x7e, 0x10, 0x88, 0x24, 0x12, 0x9, 0x4, 0x84,
    0x6c, 0x28, 0x10, 0x8, 0x4, 0x2, 0x3, 0xc0,

    /* U+0051 "Q" */
    0xf, 0x80, 0xc1, 0x84, 0x3, 0x10, 0x4, 0x80,
    0xa, 0x0, 0x28, 0x0, 0xa0, 0x2, 0x80, 0xa,
    0x0, 0x24, 0x1, 0x18, 0xc, 0x30, 0x20, 0x3f,
    0x0, 0x18, 0x0, 0x18, 0x0, 0x38,

    /* U+0052 "R" */
    0x7e, 0x2, 0x10, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x8, 0x21, 0x3, 0xe0, 0x22, 0x2, 0x30, 0x21,
    0x82, 0x8, 0x20, 0xc7, 0x2,

    /* U+0053 "S" */
    0x1e, 0x62, 0x42, 0x40, 0x40, 0x30, 0x18, 0x6,
    0x3, 0x1, 0x1, 0x81, 0x42, 0x7c,

    /* U+0054 "T" */
    0xff, 0xf0, 0x86, 0x10, 0x82, 0x0, 0x40, 0x8,
    0x1, 0x0, 0x20, 0x4, 0x0, 0x80, 0x10, 0x2,
    0x0, 0x40, 0x1e, 0x0,

    /* U+0055 "U" */
    0x70, 0x38, 0x80, 0x42, 0x1, 0x8, 0x4, 0x20,
    0x10, 0x80, 0x42, 0x1, 0x8, 0x4, 0x20, 0x10,
    0x80, 0x42, 0x3, 0x4, 0x8, 0x18, 0x40, 0x3e,
    0x0,

    /* U+0056 "V" */
    0x70, 0x38, 0x80, 0x41, 0x1, 0x4, 0x8, 0x10,
    0x20, 0x21, 0x0, 0x84, 0x2, 0x20, 0x4, 0x80,
    0x12, 0x0, 0x70, 0x0, 0xc0, 0x3, 0x0, 0x8,
    0x0,

    /* U+0057 "W" */
    0x70, 0x1, 0xc4, 0x8, 0x10, 0x81, 0x82, 0x8,
    0x50, 0x41, 0xa, 0x10, 0x21, 0x62, 0x6, 0x44,
    0x40, 0x48, 0x90, 0x9, 0x12, 0x1, 0x41, 0x40,
    0x18, 0x30, 0x3, 0x6, 0x0, 0x40, 0x40, 0x8,
    0x8, 0x0,

    /* U+0058 "X" */
    0x70, 0xe0, 0x82, 0x4, 0x20, 0x11, 0x0, 0xd0,
    0x3, 0x0, 0x8, 0x0, 0x60, 0x5, 0x0, 0x44,
    0x4, 0x30, 0x20, 0x82, 0x6, 0x38, 0x38,

    /* U+0059 "Y" */
    0xe0, 0xe4, 0x18, 0x82, 0x8, 0x81, 0x10, 0x14,
    0x2, 0x80, 0x20, 0x4, 0x0, 0x80, 0x10, 0x2,
    0x0, 0x40, 0x1c, 0x0,

    /* U+005A "Z" */
    0x7f, 0xc8, 0x1a, 0x2, 0x0, 0x80, 0x30, 0x4,
    0x1, 0x0, 0x60, 0x18, 0x2, 0x0, 0x80, 0x30,
    0x14, 0x3, 0xff, 0xc0,

    /* U+005B "[" */
    0xd2, 0x49, 0x24, 0x92, 0x49, 0x24, 0x9c,

    /* U+005C "\\" */
    0x84, 0x20, 0x84, 0x20, 0x84, 0x21, 0x4, 0x21,
    0x4, 0x20,

    /* U+005D "]" */
    0x64, 0x92, 0x49, 0x24, 0x92, 0x49, 0x3c,

    /* U+005E "^" */
    0x8, 0x18, 0x18, 0x24, 0x24, 0x42, 0x42, 0x81,
    0x81,

    /* U+005F "_" */
    0xff, 0xc0,

    /* U+0060 "`" */
    0x52, 0x20,

    /* U+0061 "a" */
    0x38, 0x44, 0x84, 0x4, 0x1c, 0xe4, 0x84, 0x8c,
    0x76,

    /* U+0062 "b" */
    0x0, 0x60, 0x10, 0x8, 0x4, 0x2, 0x1, 0x78,
    0xc6, 0x41, 0xa0, 0x50, 0x28, 0x14, 0x12, 0x9,
    0xf8,

    /* U+0063 "c" */
    0x1e, 0xc6, 0x4, 0x8, 0x10, 0x30, 0x31, 0x3c,

    /* U+0064 "d" */
    0x0, 0x1, 0xc0, 0x10, 0x4, 0x1, 0x0, 0x41,
    0xf1, 0x84, 0x81, 0x20, 0x48, 0x12, 0x4, 0xc1,
    0x18, 0xc3, 0xd8,

    /* U+0065 "e" */
    0x3c, 0x86, 0xf, 0xf8, 0x10, 0x30, 0x31, 0x3c,

    /* U+0066 "f" */
    0xe, 0x10, 0x20, 0x20, 0x20, 0xf8, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x70,

    /* U+0067 "g" */
    0x0, 0x3f, 0x42, 0x42, 0x42, 0x64, 0x18, 0x60,
    0x40, 0x3e, 0x43, 0x81, 0x81, 0xc2, 0x78,

    /* U+0068 "h" */
    0x0, 0x18, 0x2, 0x0, 0x80, 0x20, 0x8, 0x2,
    0xf0, 0xc6, 0x20, 0x88, 0x22, 0x8, 0x82, 0x20,
    0x88, 0x27, 0x1c,

    /* U+0069 "i" */
    0x48, 0x1, 0x92, 0x49, 0x25, 0xc0,

    /* U+006A "j" */
    0x11, 0x0, 0x3, 0x11, 0x11, 0x11, 0x11, 0x12,
    0x24, 0x40,

    /* U+006B "k" */
    0x20, 0x18, 0x2, 0x0, 0x80, 0x20, 0x8, 0x2,
    0x38, 0x88, 0x24, 0xa, 0x3, 0xc0, 0x90, 0x22,
    0x8, 0x47, 0x8,

    /* U+006C "l" */
    0xc, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0xe0,

    /* U+006D "m" */
    0x27, 0x1c, 0x79, 0xe6, 0x20, 0x82, 0x20, 0x82,
    0x20, 0x82, 0x20, 0x82, 0x20, 0x82, 0x20, 0x82,
    0x71, 0xc7,

    /* U+006E "n" */
    0x4e, 0x79, 0x90, 0x48, 0x24, 0x12, 0x9, 0x4,
    0x82, 0xe3, 0x80,

    /* U+006F "o" */
    0x1e, 0x30, 0xa0, 0x70, 0x18, 0xc, 0x7, 0x2,
    0x82, 0x3c, 0x0,

    /* U+0070 "p" */
    0xce, 0x38, 0x90, 0x28, 0x14, 0xa, 0x5, 0x4,
    0x84, 0x7c, 0x20, 0x10, 0x8, 0x4, 0x7, 0x0,

    /* U+0071 "q" */
    0x1f, 0x18, 0x48, 0x12, 0x4, 0x81, 0x20, 0x4c,
    0x11, 0x84, 0x3f, 0x0, 0x40, 0x10, 0x4, 0x1,
    0x0, 0xe0,

    /* U+0072 "r" */
    0x5b, 0x84, 0x10, 0x41, 0x4, 0x10, 0xe0,

    /* U+0073 "s" */
    0x74, 0x63, 0x87, 0xe, 0x31, 0xf0,

    /* U+0074 "t" */
    0x0, 0x82, 0x1e, 0x20, 0x82, 0x8, 0x20, 0x82,
    0x6,

    /* U+0075 "u" */
    0xc3, 0x10, 0x44, 0x11, 0x4, 0x41, 0x10, 0x44,
    0x11, 0x8c, 0x3d, 0x80,

    /* U+0076 "v" */
    0xe1, 0xb0, 0x88, 0x84, 0x41, 0x40, 0xa0, 0x60,
    0x10, 0x8, 0x0,

    /* U+0077 "w" */
    0xc2, 0x1a, 0x10, 0x90, 0xc8, 0x4a, 0x42, 0x92,
    0x14, 0x60, 0x63, 0x2, 0x18, 0x10, 0x80,

    /* U+0078 "x" */
    0x73, 0x8, 0x81, 0x40, 0x20, 0x8, 0x5, 0x1,
    0x20, 0x88, 0x63, 0x80,

    /* U+0079 "y" */
    0xe1, 0x98, 0x42, 0x20, 0x88, 0x14, 0x5, 0x1,
    0xc0, 0x20, 0x8, 0x4, 0x1, 0x0, 0x80, 0x60,
    0x18, 0x0,

    /* U+007A "z" */
    0xfd, 0x18, 0x20, 0x83, 0x4, 0x10, 0x61, 0xfc,

    /* U+007B "{" */
    0x12, 0x22, 0x22, 0x22, 0xc2, 0x22, 0x22, 0x22,
    0x21,

    /* U+007C "|" */
    0xff, 0xff, 0xf8,

    /* U+007D "}" */
    0x84, 0x44, 0x44, 0x44, 0x34, 0x44, 0x44, 0x44,
    0x48,

    /* U+007E "~" */
    0x78, 0x61, 0xe0,

    /* U+4E2A "个" */
    0x0, 0x40, 0x0, 0x18, 0x0, 0x2, 0x80, 0x0,
    0x88, 0x0, 0x20, 0x80, 0x8, 0x88, 0x2, 0x10,
    0x80, 0x82, 0xe, 0x20, 0x40, 0x88, 0x8, 0x0,
    0x1, 0x0, 0x0, 0x20, 0x0, 0x4, 0x0, 0x0,
    0x80, 0x0, 0x10, 0x0, 0x2, 0x0, 0x0, 0x40,
    0x0, 0x8, 0x0, 0x0, 0x0, 0x0,

    /* U+4EBA "人" */
    0x0, 0x40, 0x0, 0x6, 0x0, 0x0, 0x60, 0x0,
    0x6, 0x0, 0x0, 0x60, 0x0, 0x6, 0x0, 0x0,
    0xa0, 0x0, 0xb, 0x0, 0x0, 0x90, 0x0, 0x9,
    0x0, 0x1, 0x18, 0x0, 0x10, 0x80, 0x1, 0xc,
    0x0, 0x20, 0x40, 0x4, 0x2, 0x0, 0x80, 0x18,
    0x10, 0x0, 0xe6, 0x0, 0x0, 0x0, 0x0, 0x0,

    /* U+4FE1 "信" */
    0x0, 0x0, 0x0, 0x82, 0x0, 0x30, 0x20, 0x4,
    0x4, 0x20, 0xbf, 0xfe, 0x20, 0x0, 0x4, 0x0,
    0x41, 0x8f, 0xfc, 0x30, 0x0, 0xa, 0x0, 0x12,
    0x47, 0xfe, 0x48, 0x0, 0x1, 0x8, 0x10, 0x21,
    0xff, 0x4, 0x20, 0x40, 0x84, 0x8, 0x10, 0x81,
    0x2, 0x1f, 0xe0, 0x42, 0x4, 0x8, 0x40, 0x80,

    /* U+5F55 "录" */
    0x0, 0x4, 0x3, 0xff, 0xc0, 0x0, 0x10, 0x0,
    0x2, 0x0, 0xff, 0xc0, 0x0, 0x8, 0x0, 0x1,
    0x3, 0xff, 0xfe, 0x0, 0x40, 0x2, 0x8, 0x20,
    0x21, 0x8c, 0x6, 0x36, 0x0, 0x1d, 0x0, 0xc,
    0x90, 0x6, 0x11, 0x83, 0x2, 0x1e, 0x0, 0x40,
    0x80, 0x38, 0x0, 0x2, 0x0, 0x0,

    /* U+606F "息" */
    0x0, 0x80, 0x0, 0x10, 0x0, 0x24, 0x0, 0x7,
    0xff, 0x80, 0x80, 0x10, 0x10, 0x2, 0x3, 0xff,
    0xc0, 0x40, 0x8, 0x8, 0x1, 0x1, 0xff, 0xe0,
    0x20, 0x4, 0x4, 0x0, 0x80, 0xff, 0xf0, 0x10,
    0x0, 0x1, 0x90, 0x40, 0x22, 0x26, 0x24, 0x4,
    0x6c, 0x80, 0x83, 0xf, 0xf0, 0x0,

    /* U+610F "意" */
    0x0, 0x40, 0x0, 0x4, 0x10, 0xff, 0xff, 0x0,
    0x4, 0x0, 0x30, 0x85, 0xff, 0xff, 0xc0, 0x0,
    0x80, 0x3f, 0xf8, 0x4, 0x2, 0x0, 0xff, 0xc0,
    0x10, 0x8, 0x2, 0x1, 0x0, 0x7f, 0xe0, 0x0,
    0x84, 0x0, 0x89, 0x20, 0x91, 0x22, 0x32, 0x4,
    0x2c, 0x7f, 0x80,

    /* U+62A4 "护" */
    0x0, 0x0, 0x1, 0x1, 0x0, 0x20, 0x10, 0x4,
    0x22, 0x20, 0x97, 0xbe, 0xfe, 0x80, 0x82, 0x10,
    0x10, 0x42, 0x2, 0xa, 0x40, 0x41, 0x8f, 0xf8,
    0xe1, 0x1, 0x64, 0x20, 0x0, 0x84, 0x0, 0x10,
    0x80, 0x2, 0x20, 0x0, 0x44, 0x0, 0x9, 0x0,
    0x7, 0x20, 0x0, 0x8, 0x0, 0x0,

    /* U+65AD "断" */
    0x2, 0x0, 0x88, 0x40, 0x79, 0x89, 0x70, 0x29,
    0x48, 0x4, 0xa9, 0x0, 0x84, 0x20, 0x10, 0x94,
    0x13, 0xff, 0xff, 0x46, 0x11, 0x8, 0xc2, 0x21,
    0x2e, 0x44, 0x25, 0x28, 0x85, 0x21, 0x10, 0xc4,
    0x22, 0x10, 0x88, 0x42, 0x5, 0x8, 0x7f, 0xc1,
    0x8, 0x8, 0x20, 0x2, 0x4, 0x0,

    /* U+7528 "用" */
    0x10, 0x1, 0xf, 0xff, 0xc4, 0x10, 0x42, 0x8,
    0x21, 0x4, 0x10, 0x82, 0x8, 0x7f, 0xfc, 0x20,
    0x82, 0x10, 0x41, 0x8, 0x20, 0x87, 0xff, 0xc2,
    0x8, 0x21, 0x4, 0x10, 0x82, 0x8, 0x81, 0x4,
    0x40, 0x82, 0x40, 0x41, 0x20, 0x23, 0xa0, 0x0,
    0x0,

    /* U+76D1 "监" */
    0x0, 0x0, 0x0, 0x31, 0x80, 0x44, 0x40, 0x8,
    0x88, 0x21, 0x11, 0xfe, 0x22, 0x40, 0x4, 0x49,
    0x0, 0x8a, 0x10, 0x11, 0x41, 0x2, 0x20, 0x20,
    0x4, 0x0, 0x7, 0xff, 0xc0, 0x89, 0x10, 0x11,
    0x22, 0x2, 0x24, 0x40, 0x44, 0x88, 0x8, 0x91,
    0x4f, 0xff, 0xfc,

    /* U+836F "药" */
    0x3, 0x18, 0x0, 0x84, 0x2f, 0xff, 0xfc, 0x8,
    0x40, 0xa, 0x0, 0x2, 0x4, 0x1, 0x1, 0x8,
    0x8c, 0xfe, 0x42, 0x20, 0x9f, 0x10, 0x20, 0x88,
    0x10, 0x40, 0xc4, 0x20, 0x11, 0x1f, 0x84, 0x40,
    0x0, 0x10, 0x0, 0x4, 0x1f, 0x1, 0x18, 0x7,
    0xc0, 0x0, 0x60,

    /* U+89C1 "见" */
    0x10, 0x4, 0x7, 0xff, 0x81, 0x0, 0x40, 0x40,
    0x10, 0x10, 0xc4, 0x4, 0x21, 0x1, 0x8, 0x40,
    0x42, 0x10, 0x10, 0x84, 0x4, 0x39, 0x1, 0x1c,
    0x40, 0x45, 0x0, 0x1, 0x40, 0x80, 0x90, 0x20,
    0x64, 0x8, 0x31, 0x7, 0x30, 0x3f, 0xb0, 0x0,
    0x0,

    /* U+8BB0 "记" */
    0x30, 0x1, 0x3, 0x3f, 0xf0, 0x20, 0x4, 0x4,
    0x0, 0x80, 0x0, 0x10, 0x20, 0x2, 0x3e, 0x20,
    0x40, 0x87, 0xf8, 0x10, 0x81, 0x2, 0x10, 0x0,
    0x42, 0x0, 0x8, 0x40, 0x1, 0x28, 0x4, 0x29,
    0x0, 0x86, 0x20, 0x11, 0x84, 0x3, 0x20, 0xff,
    0xc0,

    /* U+8BCA "诊" */
    0x0, 0x10, 0x4, 0x6, 0x0, 0x60, 0xc0, 0x4,
    0x14, 0x0, 0x84, 0x40, 0x1, 0xc, 0x4, 0x20,
    0xc7, 0xc8, 0xcc, 0x12, 0x10, 0xc2, 0x84, 0x0,
    0x41, 0x18, 0x8, 0xc4, 0x1, 0x1, 0x10, 0x20,
    0x47, 0x5, 0xb1, 0x80, 0xc0, 0x40, 0x30, 0x30,
    0x4, 0x8, 0x0, 0xe, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 66, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 86, .box_w = 2, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 109, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 8, .adv_w = 149, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 23, .adv_w = 148, .box_w = 8, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 39, .adv_w = 255, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 64, .adv_w = 227, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 87, .adv_w = 76, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 89, .adv_w = 120, .box_w = 4, .box_h = 17, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 98, .adv_w = 120, .box_w = 4, .box_h = 17, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 107, .adv_w = 135, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 113, .adv_w = 194, .box_w = 9, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 76, .box_w = 2, .box_h = 4, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 127, .adv_w = 120, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 128, .adv_w = 76, .box_w = 2, .box_h = 2, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 107, .box_w = 5, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 139, .adv_w = 148, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 148, .box_w = 5, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 148, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 174, .adv_w = 148, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 148, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 148, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 148, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 148, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 237, .adv_w = 148, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 148, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 261, .adv_w = 76, .box_w = 2, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 76, .box_w = 2, .box_h = 11, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 267, .adv_w = 194, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 194, .box_w = 10, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 288, .adv_w = 194, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 302, .adv_w = 110, .box_w = 5, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 239, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 334, .adv_w = 219, .box_w = 13, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 356, .adv_w = 179, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 222, .box_w = 12, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 236, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 418, .adv_w = 180, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 164, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 233, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 475, .adv_w = 254, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 102, .box_w = 5, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 509, .adv_w = 100, .box_w = 5, .box_h = 17, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 520, .adv_w = 213, .box_w = 12, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 541, .adv_w = 168, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 559, .adv_w = 287, .box_w = 17, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 589, .adv_w = 243, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 614, .adv_w = 247, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 637, .adv_w = 174, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 653, .adv_w = 247, .box_w = 14, .box_h = 17, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 683, .adv_w = 201, .box_w = 12, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 704, .adv_w = 149, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 718, .adv_w = 194, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 738, .adv_w = 241, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 763, .adv_w = 228, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 788, .adv_w = 311, .box_w = 19, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 822, .adv_w = 207, .box_w = 13, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 845, .adv_w = 198, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 865, .adv_w = 194, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 885, .adv_w = 120, .box_w = 3, .box_h = 18, .ofs_x = 3, .ofs_y = -4},
    {.bitmap_index = 892, .adv_w = 107, .box_w = 5, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 902, .adv_w = 120, .box_w = 3, .box_h = 18, .ofs_x = 2, .ofs_y = -4},
    {.bitmap_index = 909, .adv_w = 194, .box_w = 8, .box_h = 9, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 918, .adv_w = 160, .box_w = 10, .box_h = 1, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 920, .adv_w = 76, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 922, .adv_w = 133, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 931, .adv_w = 161, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 948, .adv_w = 137, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 956, .adv_w = 169, .box_w = 10, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 975, .adv_w = 133, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 983, .adv_w = 84, .box_w = 8, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 997, .adv_w = 142, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1012, .adv_w = 166, .box_w = 10, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1031, .adv_w = 77, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1037, .adv_w = 74, .box_w = 4, .box_h = 19, .ofs_x = -1, .ofs_y = -5},
    {.bitmap_index = 1047, .adv_w = 158, .box_w = 10, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1066, .adv_w = 73, .box_w = 4, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1074, .adv_w = 254, .box_w = 16, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1092, .adv_w = 169, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1103, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1114, .adv_w = 168, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1130, .adv_w = 161, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1148, .adv_w = 108, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1155, .adv_w = 108, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1161, .adv_w = 89, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1170, .adv_w = 165, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1182, .adv_w = 144, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1193, .adv_w = 209, .box_w = 13, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1208, .adv_w = 149, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1220, .adv_w = 145, .box_w = 10, .box_h = 14, .ofs_x = 0, .ofs_y = -5},
    {.bitmap_index = 1238, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1246, .adv_w = 118, .box_w = 4, .box_h = 18, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1255, .adv_w = 83, .box_w = 1, .box_h = 21, .ofs_x = 2, .ofs_y = -5},
    {.bitmap_index = 1258, .adv_w = 118, .box_w = 4, .box_h = 18, .ofs_x = 2, .ofs_y = -4},
    {.bitmap_index = 1267, .adv_w = 194, .box_w = 10, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 1270, .adv_w = 320, .box_w = 19, .box_h = 19, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1316, .adv_w = 320, .box_w = 20, .box_h = 19, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1364, .adv_w = 320, .box_w = 19, .box_h = 20, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1412, .adv_w = 320, .box_w = 19, .box_h = 19, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1458, .adv_w = 320, .box_w = 19, .box_h = 19, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1504, .adv_w = 320, .box_w = 19, .box_h = 18, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1547, .adv_w = 320, .box_w = 19, .box_h = 19, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1593, .adv_w = 320, .box_w = 19, .box_h = 19, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1639, .adv_w = 320, .box_w = 17, .box_h = 19, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1680, .adv_w = 320, .box_w = 19, .box_h = 18, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1723, .adv_w = 320, .box_w = 18, .box_h = 19, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1766, .adv_w = 320, .box_w = 18, .box_h = 18, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1807, .adv_w = 320, .box_w = 19, .box_h = 17, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1848, .adv_w = 320, .box_w = 19, .box_h = 19, .ofs_x = 1, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x90, 0x1b7, 0x112b, 0x1245, 0x12e5, 0x147a, 0x1783,
    0x26fe, 0x28a7, 0x3545, 0x3b97, 0x3d86, 0x3da0
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 20010, .range_length = 15777, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 14, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Fontcn20 = {
#else
lv_font_t ui_font_Fontcn20 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 23,          /*The maximum line height required by the font*/
    .base_line = 5,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_FONTCN20*/

