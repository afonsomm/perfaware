

/**



*/


#ifndef BASE_DEBUG_PRINT_H
# define BASE_DEBUG_PRINT_H


#ifdef DEBUG
internalf void debug_printf(const s8* fmt, ...);
internalf void debug_print_u32(u32 n)  ;
internalf void debug_print_u64(u64 n)  ;
internalf void debug_print_r32(r32 n)  ;
internalf void debug_print_r64(r64 n)  ;
internalf void debug_print_cstr8(s8* s);
internalf void debug_print_cstr8_u32(s8* s, u32 n) ;
internalf void debug_print_cstr8_u64(s8* s, u64 n) ;
internalf void debug_print_cstr8_r32(s8* s, r32 n) ;
internalf void debug_print_cstr8_r64(s8* s, r64 n) ;
internalf void debug_print_cstr8_lit(const s8* s);
internalf void debug_print_cstr8_lit_u32(const s8* s, u32 n);
internalf void debug_print_cstr8_lit_u64(const s8* s, u64 n);
internalf void debug_print_cstr8_lit_r32(const s8* s, r32 n);
internalf void debug_print_cstr8_lit_r64(const s8* s, r64 n);
internalf void debug_print_str8(String8 s);
internalf void debug_print_str16(String16 s);
internalf void debug_print8(u8* str, u64 len);
internalf void debug_print16(u16* str, u64 len);
#else
# define debug_printf(fmt, ...)
# define debug_print_u32(n)
# define debug_print_u64(n)
# define debug_print_r32(n)
# define debug_print_r64(n)
# define debug_print_cstr8(s)
# define debug_print_cstr8_u32(s, n)
# define debug_print_cstr8_u64(s, n)
# define debug_print_cstr8_r32(s, n)
# define debug_print_cstr8_r64(s, n)
# define debug_print_cstr8_lit(s, n)
# define debug_print_cstr8_lit_u32(s, n)
# define debug_print_cstr8_lit_u64(s, n)
# define debug_print_cstr8_lit_r32(s, n)
# define debug_print_cstr8_lit_r64(s, n)
# define debug_print_str8(s)
# define debug_print_str16(s)
# define debug_print8(str, len)
# define debug_print16(str, len)
#endif


#endif  // BASE_DEBUG_PRINT_H
