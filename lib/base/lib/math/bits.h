

/**

----- Brief -----
Binary logic operations.
As said in the general guidelines, almost all this functions are one-lines, which are prefered instead of macros.
----- Brief -----

*/


#ifndef BASE_MATH_BITS_H
# define BASE_MATH_BITS_H


// -- Preprocessor Defines

// ---- Bitmasks
#define bitmask1  0x00000001
#define bitmask2  0x00000003
#define bitmask3  0x00000007
#define bitmask4  0x0000000f
#define bitmask5  0x0000001f
#define bitmask6  0x0000003f
#define bitmask7  0x0000007f
#define bitmask8  0x000000ff
#define bitmask9  0x000001ff
#define bitmask10 0x000003ff
#define bitmask11 0x000007ff
#define bitmask12 0x00000fff
#define bitmask13 0x00001fff
#define bitmask14 0x00003fff
#define bitmask15 0x00007fff
#define bitmask16 0x0000ffff
#define bitmask17 0x0001ffff
#define bitmask18 0x0003ffff
#define bitmask19 0x0007ffff
#define bitmask20 0x000fffff
#define bitmask21 0x001fffff
#define bitmask22 0x003fffff
#define bitmask23 0x007fffff
#define bitmask24 0x00ffffff
#define bitmask25 0x01ffffff
#define bitmask26 0x03ffffff
#define bitmask27 0x07ffffff
#define bitmask28 0x0fffffff
#define bitmask29 0x1fffffff
#define bitmask30 0x3fffffff
#define bitmask31 0x7fffffff
#define bitmask32 0xffffffff

#define bitmask33 0x00000001ffffffffull
#define bitmask34 0x00000003ffffffffull
#define bitmask35 0x00000007ffffffffull
#define bitmask36 0x0000000fffffffffull
#define bitmask37 0x0000001fffffffffull
#define bitmask38 0x0000003fffffffffull
#define bitmask39 0x0000007fffffffffull
#define bitmask40 0x000000ffffffffffull
#define bitmask41 0x000001ffffffffffull
#define bitmask42 0x000003ffffffffffull
#define bitmask43 0x000007ffffffffffull
#define bitmask44 0x00000fffffffffffull
#define bitmask45 0x00001fffffffffffull
#define bitmask46 0x00003fffffffffffull
#define bitmask47 0x00007fffffffffffull
#define bitmask48 0x0000ffffffffffffull
#define bitmask49 0x0001ffffffffffffull
#define bitmask50 0x0003ffffffffffffull
#define bitmask51 0x0007ffffffffffffull
#define bitmask52 0x000fffffffffffffull
#define bitmask53 0x001fffffffffffffull
#define bitmask54 0x003fffffffffffffull
#define bitmask55 0x007fffffffffffffull
#define bitmask56 0x00ffffffffffffffull
#define bitmask57 0x01ffffffffffffffull
#define bitmask58 0x03ffffffffffffffull
#define bitmask59 0x07ffffffffffffffull
#define bitmask60 0x0fffffffffffffffull
#define bitmask61 0x1fffffffffffffffull
#define bitmask62 0x3fffffffffffffffull
#define bitmask63 0x7fffffffffffffffull
#define bitmask64 0xffffffffffffffffull
// ---- Bitmasks

// ---- Single Bits
#define bit0  (1 << 0)
#define bit1  (1 << 1)
#define bit2  (1 << 2)
#define bit3  (1 << 3)
#define bit4  (1 << 4)
#define bit5  (1 << 5)
#define bit6  (1 << 6)
#define bit7  (1 << 7)
#define bit8  (1 << 8)
#define bit9  (1 << 9)
#define bit10 (1 << 10)
#define bit11 (1 << 11)
#define bit12 (1 << 12)
#define bit13 (1 << 13)
#define bit14 (1 << 14)
#define bit15 (1 << 15)
#define bit16 (1 << 16)
#define bit17 (1 << 17)
#define bit18 (1 << 18)
#define bit19 (1 << 19)
#define bit20 (1 << 20)
#define bit21 (1 << 21)
#define bit22 (1 << 22)
#define bit23 (1 << 23)
#define bit24 (1 << 24)
#define bit25 (1 << 25)
#define bit26 (1 << 26)
#define bit27 (1 << 27)
#define bit28 (1 << 28)
#define bit29 (1 << 29)
#define bit30 (1 << 30)
#define bit31 (1 << 31)

#define bit32 (1ull << 32)
#define bit33 (1ull << 33)
#define bit34 (1ull << 34)
#define bit35 (1ull << 35)
#define bit36 (1ull << 36)
#define bit37 (1ull << 37)
#define bit38 (1ull << 38)
#define bit39 (1ull << 39)
#define bit40 (1ull << 40)
#define bit41 (1ull << 41)
#define bit42 (1ull << 42)
#define bit43 (1ull << 43)
#define bit44 (1ull << 44)
#define bit45 (1ull << 45)
#define bit46 (1ull << 46)
#define bit47 (1ull << 47)
#define bit48 (1ull << 48)
#define bit49 (1ull << 49)
#define bit50 (1ull << 50)
#define bit51 (1ull << 51)
#define bit52 (1ull << 52)
#define bit53 (1ull << 53)
#define bit54 (1ull << 54)
#define bit55 (1ull << 55)
#define bit56 (1ull << 56)
#define bit57 (1ull << 57)
#define bit58 (1ull << 58)
#define bit59 (1ull << 59)
#define bit60 (1ull << 60)
#define bit61 (1ull << 61)
#define bit62 (1ull << 62)
#define bit63 (1ull << 63)
// ---- Single Bits

// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
// -- Types


// -- Functions

// ---- Bitmasks
internalf u64 bitmask(u8 index);
// ---- Bitmasks

// ---- Single Bits
internalf u64 bit(u8 index);
// ---- Single Bits

// ------ Bitsets
internalf u64 bitset_set(u64 bitset, u64 flags);
internalf b8  bitset_is_set(u64 bitset, u64 flags);
internalf u64 bitset_clear(u64 bitset, u64 flags);
internalf u64 bitset_toggle(u64 bitset, u64 flags);
internalf b8  bitset_match(u64 bitset, u64 flags);
internalf b8  bitset_intersect(u64 bitset, u64 flags);

internalf u64 bitset_bit_set(u64 bitset, u8 index);
internalf b8  bitset_bit_is_set(u64 bitset, u8 index);
internalf u64 bitset_bit_clear(u64 bitset, u8 index);
internalf u64 bitset_bit_toggle(u64 bitset, u8 index);

internalf u64 bitset_get(u64 bitset, u8 position, u8 n_bits);
internalf u8  bit_from_bitset(u64 bitset, u8 index);
internalf u8  byte_from_bitset(u64 bitset, u8 index);
internalf u16 word_from_bitset(u64 bitset, u8 index);
internalf u32 dword_from_bitset(u64 bitset, u8 index);
// ------ Bitsets

// ------ Alignment
internalf u64 align_up_pow2(u64 number, u64 base);
internalf u64 align_down_pow2(u64 number, u64 base);
internalf b8  is_aligned_pow2(u64 number, u64 base);
// ------ Alignment

// ------ Modulos
internalf u64 mod_pow2(u64 number, u64 base);
// ------ Modulos

// ---- Concat
internalf u16 word_from_bytes(u8 hi, u8 lo);
internalf u32 dword_from_bytes(u8 hi, u8 mid_hi, u8 mid_lo, u8 lo);
internalf u32 dword_from_words(u16 hi, u16 lo);
internalf u64 qword_from_bytes(u8 hi, u8 mid_hi_2, u8 mid_hi_1, u8 mid_hi, u8 mid_lo, u8 mid_lo_1, u8 mid_lo_2, u8 lo);
internalf u64 qword_from_words(u16 hi, u16 mid_hi, u16 mid_lo, u16 lo);
internalf u64 qword_from_dwords(u32 hi, u32 lo);
// ---- Concat

// ---- Rounding
internalf u32 round_up_pow2_u32(u32 n);
internalf u64 round_up_pow2_u64(u64 n);
// ---- Rounding

// ---- Rotations
internalf u32 rotate_left_u32(u32 number, s32 shift);
internalf u64 rotate_left_u64(u64 number, s32 shift);
// ---- Rotations

// -- Functions


#endif  // BASE_MATH_BITS_H
