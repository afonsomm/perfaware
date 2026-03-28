

/**

----- Brief -----
String manipulation library.
----- Brief -----

*/


#ifndef BASE_STRINGS_CORE_H
# define BASE_STRINGS_CORE_H


// -- Preprocessor Defines

// ---- Tokens
#define stringify_(s) #s
#define stringify(s)  stringify_(s)
// ---- Tokens

// ---- CStrings
#define cstr8_lit_expand(l) (u8*)(l), sizeof(l) - 1
// ---- CStrings

// ---- String8
#define str8_lit(l) str8((u8*)(l), sizeof(l) - 1)
#define str8_lit_comp(s) { (u8*)s, sizeof(s) - 1 }
#define str8_nil() str8((u8*)0, 0)
#define str8_nil_comp(s) { (u8*)0, 0 }
// ---- String8

// ---- String16
#define str16_nil()  str16((u16*)0, 0)
#define str16_lit(l) str16((u16*)(l), sizeof(l) / 2 - 1)
// ---- String16

// ---- Hashing
#define cstr8_lit_hash(l)  cstr8_hash(cstr8_lit_expand((l)))
// ---- Hashing

// -- Preprocessor Defines


// -- Enums/Flags

// ---- String8
typedef enum StrFindFlag {
	StrFindFlag_Backwards = 1 << 0,
	StrFindFlag_ReturnCountOnFail = 1 << 1,
} StrFindFlag;
// ---- String8

// -- Enums/Flags


// -- Types

// ---- String8
typedef struct String8 String8;
struct String8 {
	u8* str;
	u64 len;
};
// ---- String8

// ---- String8 Lists
typedef struct String8Node String8Node;
struct String8Node {
	Sll node;
	String8 str;
};

typedef struct String8List String8List;
struct String8List {
	Sll first;
	Sll last;
	u64 len;
	u64 size;
};

typedef struct String8Join String8Join;
struct String8Join {
	String8 pre;
	String8 post;
	String8 mid;
};
// ---- String8 Lists

// ---- String16
typedef struct String16 String16;
struct String16 {
	u16* str;
	u64  len;
};
// ---- String16

// ---- Unicode
typedef struct Codepoint Codepoint;
struct Codepoint {
	u32 bytes;
	u32 len;
};
// ---- Unicode

// -- Types


// -- Functions
#include "../../../externals/inc_begin.h"
#include <stdarg.h>
#include "../../../externals/inc_end.h"

// ---- CStrings
internalf u64 cstr8_len(u8* str);
internalf u64 cstr16_len(u16* str);
internalf u64 cstr32_len(u32* str);
// ---- CStrings

// ---- String8
internalf String8 str8(u8* cstr, u64 len);
internalf b8 str8_is_valid(String8 str);
internalf String8 str8_cstr(u8* cstr);

internalf String8 str8_range(u8* first, u8* last);
internalf String8 str8_prefix(String8 str, u64 len);
internalf String8 str8_chop(String8 str, u64 amount);
internalf String8 str8_postfix(String8 str, u64 len);
internalf String8 str8_skip(String8 str, u64 amount);
internalf String8 str8_substr(String8 str, u64 first, u64 last);

internalf u64 str8_find_char_pos(String8 str, u8 character, u32 occurrence, enum8(StrFindFlag) flags);
internalf u64 str8_find_substr_pos(String8 str, String8 substr, u32 occurrence, enum8(StrFindFlag) flags);

internalf String8 str8_copy(Allocator* allocator, String8 str);
internalf String8 str8_cat(Allocator* allocator, String8 start, String8 end);

internalf u64 str8s_lenv(u32 count, va_list str_args);
internalf u64 str8s_len(u32 count, ...);
internalf String8 str8s_catv(Allocator* allocator, u32 count, u64 len, va_list args);
internalf String8 str8s_cat(Allocator* allocator, u32 count, ...);

internalf String8 str8fv(Allocator* allocator, const s8* fmt, va_list args);
internalf String8 str8f(Allocator* allocator, const s8* fmt, ...);
// ---- String8

// ---- String8 Lists
internalf void str8_list_init(String8List* list);
internalf void str8_list_push_explicit(String8List* list, String8 str, String8Node* node_memory);
internalf void str8_list_push(Allocator* allocator, String8List* list, String8 str);
internalf String8 str8_join(Allocator* allocator, String8List* list, String8Join* optional_join);
internalf String8List str8_split(Allocator* allocator, String8 str, const s8* split_chars, u32 split_chars_count);
internalf void str8_list_pushf(Allocator* allocator, String8List* list, const s8* fmt, ...);
// ---- String8 Lists

// ---- String16
internalf String16 str16(u16* str, u64 len);
internalf String16 str16_cstr(u16* cstr);
internalf String16 str16_copy(Allocator* allocator, String16 str);
// ---- String16

// ---- Unicode

// ------ UTF-8 Decoding
internalf Codepoint utf8_decode(u8* str, u64 len);
internalf u32 utf8_encode(u8* dest, Codepoint codepoint);
// ------

// ------ UTF-16 Decoding
internalf Codepoint utf16_decode(u16* str, u64 len);
internalf u32 utf16_encode(u16* dest, Codepoint codepoint);
// ------

// ------ Conversions
internalf String8 str8_from_16(Allocator* allocator, String16 str);
internalf String16 str16_from_8(Allocator* allocator, String8 str);
// ------ Conversions

// ---- Unicode

// ---- String to Numeric
internalf u32 str8_to_u32(String8 str);
// ---- String to Numeric

// ---- Hashing
internalf u64 cstr8_hash_from_with_seed(u8* str, u64 seed, u64 len);
internalf u64 cstr8_hash(u8* str, u64 len);
// ---- Hashing

// -- Functions


#endif  // BASE_STRINGS_CORE_H
