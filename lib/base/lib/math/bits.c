

/**

*/


// -- Global State
// -- Global State


// -- Functions

// ---- Bitmasks
internalf u64 bitmask(u8 index) {
    return ((u64)1 << index) - 1;
}
// ---- Bitmasks

// ---- Single Bits
internalf u64 bit(u8 index) {
    return (u64)1 << index;
}
// ---- Single Bits

// ---- Bitsets
internalf u64 bitset_set(u64 bitset, u64 flags) {
  return bitset | flags;
}

internalf b8 bitset_is_set(u64 bitset, u64 flags) {
	return (bitset & flags) == flags;
}

internalf u64 bitset_clear(u64 bitset, u64 flags) {
  return bitset & (~flags);
}

internalf u64 bitset_toggle(u64 bitset, u64 flags) {
  return bitset ^ flags;
}

internalf b8 bitset_match(u64 bitset, u64 flags) {
  return bitset == flags;
}

internalf b8 bitset_intersect(u64 bitset, u64 flags) {
  return !!(bitset & flags);
}


internalf u64 bitset_bit_set(u64 bitset, u8 index) {
	return bitset | ((u64)1 << index);
}

internalf b8 bitset_bit_is_set(u64 bitset, u8 index) {
	return (bitset & ((u64)1 << index)) > 0;
}

internalf u64 bitset_bit_clear(u64 bitset, u8 index) {
	return bitset & (~((u64)1 << index));
}

internalf u64 bitset_bit_toggle(u64 bitset, u8 index) {
	return bitset ^ ((u64)1 << index);
}


internalf u64 bitset_get(u64 bitset, u8 position, u8 n_bits) {
    return (bitset >> position) & bitmask(n_bits);
}

internalf u8 bit_from_bitset(u64 bitset, u8 index) {
    return (u8)((bitset >> index) & 1);
}

internalf u8 byte_from_bitset(u64 bitset, u8 index) {
    return (u8)((bitset >> (index * 8)) & U8_MAX);
}

internalf u16 word_from_bitset(u64 bitset, u8 index) {
    return (u16)((bitset >> (index * 16)) & U16_MAX);
}

internalf u32 dword_from_bitset(u64 bitset, u8 index) {
    return (u32)((bitset >> (index * 32)) & U32_MAX);
}
// ---- Bitsets

// ---- Alignment
internalf u64 align_up_pow2(u64 number, u64 base) {
    return (number + base - 1) & (~(base - 1));
}

internalf u64 align_down_pow2(u64 number, u64 base) {
    return number & (~(base - 1));
}

internalf b8 is_aligned_pow2(u64 number, u64 base) {
    return (number & (base - 1)) == 0;
}
// ---- Alignment

// ---- Modulos
internalf u64 mod_pow2(u64 number, u64 base) {
    return number & (base - 1);
}
// ---- Modulos

// ---- Concat
internalf u16 word_from_bytes(u8 hi, u8 lo) {
	return (u16)(((u16)(hi) << 8) | ((u16)(lo)));
}

internalf u32 dword_from_bytes(u8 hi, u8 mid_hi, u8 mid_lo, u8 lo) {
	return ((u32)(hi) << 24) | ((u32)(mid_hi) << 16) | ((u32)(mid_lo) << 8) | ((u32)(lo));
}

internalf u32 dword_from_words(u16 hi, u16 lo) {
	return ((u32)(hi) << 16) | ((u32)(lo));
}

internalf u64 qword_from_bytes(u8 hi, u8 mid_hi_2, u8 mid_hi_1, u8 mid_hi, u8 mid_lo, u8 mid_lo_1, u8 mid_lo_2, u8 lo) {
	return ((u64)(hi) << 56) | ((u64)(mid_hi_2) << 48) | \
    ((u64)(mid_hi_1) << 40) | ((u64)(mid_hi) << 32) | \
    ((u64)(mid_lo) << 24) | ((u64)(mid_lo_1) << 16) | \
    ((u64)(mid_lo_2) << 8) | ((u64)(lo));
}

internalf u64 qword_from_words(u16 hi, u16 mid_hi, u16 mid_lo, u16 lo) {
	return ((u64)(hi) << 48) | ((u64)(mid_hi) << 32) | ((u64)(mid_lo) << 16) | ((u64)(lo));
}

internalf u64 qword_from_dwords(u32 hi, u32 lo) {
	return ((u64)(hi) << 32) | ((u64)(lo));
}
// ---- Concat

// ---- Rounding
internalf u32 round_up_pow2_u32(u32 n) {
    
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;
    
	return n;
}

internalf u64 round_up_pow2_u64(u64 n) {
    
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	n++;
    
	return n;
}
// ---- Rounding

// ---- Rotations
internalf u32 rotate_left_u32(u32 number, s32 shift) {
	return (number << shift) | (number >> (32 - shift));
}

internalf u64 rotate_left_u64(u64 number, s32 shift) {
	return (number << shift) | (number >> (64 - shift));
}
// ---- Rotations

// -- Functions
