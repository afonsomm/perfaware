
#include "../../../externals/inc_begin.h"
#include <stdio.h>  // vsnprintf
#include "../../../externals/inc_end.h"


// -- Functions

// ---- CStrings
internalf u64 cstr8_len(u8* str) {
	u64 result = 0;
	u8* c;
	for(c = str; *c; c++) {
		result++;
	}
	return result;
}

internalf u64 cstr16_len(u16* str) {
	u64 result = 0;
	u16* c;
	for(c = str; *c; c++) {
		result++;
	}
	return result;
}

internalf u64 cstr32_len(u32* str) {
	u64 result = 0;
	u32* c;
	for(c = str; *c; c++) {
		result++;
	}
	return result;
}
// ---- CStrings

// ---- String8
internalf String8 str8(u8* cstr, u64 len) {
	String8 result;
	result.str = cstr;
	result.len = len;
	return result;
}

internalf b8 str8_is_valid(String8 str) {
	return (b8)!((str.str == (u8*)0) | (str.len == 0));
}

internalf String8 str8_cstr(u8* cstr) {
	u64 len = cstr8_len(cstr);
	String8 result;
	result.str = cstr;
	result.len = len;
	return result;
}

internalf String8 str8_range(u8* first, u8* last) {
	String8 result;
	assert(first <= last);
	result.str = first;
	result.len = (u64)(last - first);
	return result;
}

internalf String8 str8_prefix(String8 str, u64 len) {
	u64 len_clamped = min(len, str.len);
	String8 result;
	result.str = str.str;
	result.len = len_clamped;
	return result;
}

internalf String8 str8_chop(String8 str, u64 amount) {
	u64 amount_choped = min(amount, str.len);
	String8 result;
	result.str = str.str;
	result.len = str.len - amount_choped;
	return result;
}

internalf String8 str8_postfix(String8 str, u64 len) {
	u64 len_clamped = min(len, str.len);
	String8 result;
	result.str = str.str + (str.len - len_clamped);
	result.len = len_clamped;
	return result;
}

internalf String8 str8_skip(String8 str, u64 amount) {
	u64 amount_skipped = min(amount, str.len);
	String8 result;
	result.str = str.str + amount_skipped;
	result.len = str.len - amount_skipped;
	return result;
}

internalf String8 str8_substr(String8 str, u64 first, u64 last) {
	String8 result;
	result.str = str.str + first;
	result.len = last - first;
	return result;
}

internalf u64 str8s_lenv(u32 count, va_list str_args) {
    
	u64 result = 0;
	u32 str_i;
    
	va_list str_args_cpy;
	va_copy(str_args_cpy, str_args);
    
	{
		String8 str;
		for(str_i = 0; str_i < count; str_i++) {
			str = va_arg(str_args_cpy, String8);
			result += str.len;
		}
	}
    
	va_end(str_args_cpy);
    
	return result;
}

internalf u64 str8s_len(u32 count, ...) {
    
	u64 result = 0;
    
	va_list str_args;
	va_start(str_args, count);
	result = str8s_lenv(count, str_args);
	va_end(str_args);
    
	return result;
}

internalf u64 str8_find_char_pos(String8 str, u8 character, u32 occurrence, enum8(StrFindFlag) flags) {
    
	u64 result = 0;
    
	u32 original_occurence;
	u8* curr;
	u8* end;
	s8 inc;
    
	assert(occurrence);
    
	original_occurence = occurrence;
	if(flags & StrFindFlag_Backwards) {
		curr = &str.str[str.len - 1];
		end = str.str - 1;
		inc = -1;
	} else {
		curr = str.str;
		end = &str.str[str.len];
		inc = 1;
	}
    
	for(; occurrence && (curr != end); curr += inc) {
		if((*curr) == character) {
			occurrence -= 1;
		}
	}
    
	if(!occurrence) {
		curr -= inc;
		result = (u64)(curr - str.str) + 1;
	} else if(flags & StrFindFlag_ReturnCountOnFail) {
		result = original_occurence - occurrence;
	}
    
	return result;
}

internalf u64 str8_find_substr_pos(String8 str, String8 substr, u32 occurrence, enum8(StrFindFlag) flags) {
    
	u64 result = 0;
    
	u32 original_occurence;
	u8* curr;
	u8* end;
	s8 inc;
    
	assert(str.len >= substr.len);
	assert(occurrence);
    
	original_occurence = occurrence;
	if(flags & StrFindFlag_Backwards) {
		curr = str.str + str.len - substr.len;
		end = str.str - 1;
		inc = -1;
	} else {
		curr = str.str;
		end = str.str + str.len - substr.len + 1;
		inc = 1;
	}
	
	for(; occurrence && (curr != end); curr += inc) {
		if(mem_match(curr, substr.str, sizeof(u8) * substr.len)) {
			occurrence -= 1;
		}
	}
    
	if(!occurrence) {
		result = (u64)(curr - str.str) + 1;
	} else if(flags & StrFindFlag_ReturnCountOnFail) {
		result = original_occurence - occurrence;
	}
	
	return result;
}

internalf String8 str8_copy(Allocator* allocator, String8 str) {
    
	String8 result = str8_nil();
    
	result.str = (u8*)allocate(allocator, sizeof(u8) * (str.len + 1));
	if(result.str) {
		mem_cpy(result.str, str.str, sizeof(u8) * str.len);
		result.len = str.len;
		result.str[result.len] = '\0';
	}
    
	return result;
}

internalf String8 str8_cat(Allocator* allocator, String8 start, String8 end) {
    
	String8 result = str8_nil();
    
	result.str = (u8*)allocate(allocator, sizeof(u8) * (start.len + end.len + 1));
	if(result.str) {
		mem_cpy(result.str, start.str, sizeof(u8) * start.len);
		mem_cpy(result.str + start.len, end.str, sizeof(u8) * end.len);
		result.len = start.len + end.len;
		result.str[result.len] = '\0';
	}
    
	return result;
}

internalf String8 str8s_catv(Allocator* allocator, u32 count, u64 len, va_list str_args) {
    
	String8 result = str8_nil();
	u64 curr_str_pos = 0;
	u32 str_i;
    
	va_list str_args_cpy;
	va_copy(str_args_cpy, str_args);
    
	result.str = (u8*)allocate(allocator, sizeof(u8) * (len + 1));
	result.len = len;
    
	for(str_i = 0; str_i < count; str_i++) {
		String8 curr = va_arg(str_args_cpy, String8);
		mem_cpy(result.str + curr_str_pos, curr.str, curr.len);
		curr_str_pos += curr.len;
	}
	result.str[len] = '\0';
    
	va_end(str_args_cpy);
    
	return result;
}

internalf String8 str8s_cat(Allocator* allocator, u32 count, ...) {
    
	String8 result = str8_nil();
	u64 len;
    
	va_list str_args;
	va_start(str_args, count);
    
	len = str8s_lenv(count, str_args);
	result = str8s_catv(allocator, count, len, str_args);
	va_end(str_args);
    
	return result;
}

internalf String8 str8fv(Allocator* allocator, const s8* fmt, va_list args) {
    
	u64 buffer_size;
	u8* buffer;
	u64 actual_size;
	String8 result = str8_nil();
	
	va_list args2;
	va_copy(args2, args);
    
	buffer_size = 1024;
	buffer = (u8*)allocate(allocator, sizeof(u8) * buffer_size);
	actual_size = (u64)vsnprintf((s8*)buffer, buffer_size, fmt, args2);
    
	buffer = (u8*)reallocate(allocator, buffer, sizeof(u8) * buffer_size, sizeof(u8) * (actual_size + 1));
	if(actual_size > buffer_size) {  // need to reconstruct string because buffer_size was too small
		vsnprintf((s8*)buffer, actual_size, fmt, args2);
	}
	result = str8(buffer, actual_size);
	result.str[result.len] = '\0';
    
	va_end(args2);
    
	return result;
}

internalf String8 str8f(Allocator* allocator, const s8* fmt, ...) {
	String8 result;
	va_list args;
	va_start(args, fmt);
	result = str8fv(allocator, fmt, args);
	va_end(args);
	return result;
}
// ---- String8

// ---- String8 Lists
internalf void str8_list_init(String8List* list) {
	list->first.next = 0;
	list->last.next = 0;
	list->len = 0;
	list->size = 0;
	sll_queue_init(&list->first, &list->last);
}

internalf void str8_list_push_explicit(String8List* list, String8 str, String8Node* node_memory) {
	node_memory->str = str;
	sll_queue_push_back(&list->first, &list->last, &node_memory->node);
	list->len += 1;
	list->size += str.len;
}

internalf void str8_list_push(Allocator* allocator, String8List* list, String8 str) {
	String8Node* node = (String8Node*)allocate(allocator, sizeof(String8Node));
	str8_list_push_explicit(list, str, node);
}

internalf String8 str8_join(Allocator* allocator, String8List* list, String8Join* optional_join) {
	
	String8 result;
	u8* temp_ptr;
    
	{
		local_persist String8Join dummy_join = { str8_nil_comp(), str8_nil_comp(), str8_nil_comp() };
		if(!optional_join) {
			optional_join = &dummy_join;
		}
	}
    
	result.len = (optional_join->pre.len + optional_join->post.len + optional_join->mid.len * (list->len - 1) + list->size);
	result.str = (u8*)allocate(allocator, sizeof(u8) * (result.len + 1));
	temp_ptr = result.str;
    
	// -- Prefix
	mem_cpy(temp_ptr, optional_join->pre.str, optional_join->pre.len);
	temp_ptr += optional_join->pre.len;
	// -- Prefix
    
	// -- Join strings with middle separators
	if(list->first.next && (list->first.next != &list->first)) {
		String8Node* node = container_of(list->first.next, String8Node, node);
		mem_cpy(temp_ptr, node->str.str, node->str.len);
		temp_ptr += node->str.len;
		for(node = container_of(node->node.next, String8Node, node); 
			&node->node != &list->first;
			node = container_of(node->node.next, String8Node, node)) {
			mem_cpy(temp_ptr, optional_join->mid.str, optional_join->mid.len);  	
			temp_ptr += optional_join->mid.len;
			mem_cpy(temp_ptr, node->str.str, node->str.len);
			temp_ptr += node->str.len;
		}
	}
	// -- Join strings with middle separators
    
	// -- Postfix
	mem_cpy(temp_ptr, optional_join->post.str, optional_join->post.len);
	result.str[result.len] = '\0';
	// -- Postfix
    
	return result;
}

internalf String8List str8_split(Allocator* allocator, String8 str, const s8* split_chars, u32 split_chars_count) {
	// TODO: now includes empty strings, maybe handle that
    
	String8List result;
  u8* ptr;
	u8* word_first;
	u8* last;
	u32 i;
    
	str8_list_init(&result);
	ptr = str.str;
	word_first = ptr;
	last = str.str + str.len;
	for(; ptr < last; ptr += 1) {
		u8 byte = *ptr;
		b8 is_split = 0;
		for(i = 0; i < split_chars_count; i += 1) {
			if(byte == (u8)split_chars[i]) {
				is_split = 1;
				break;
			}
		}
        
		if(is_split) {
			if(word_first < ptr) {
				str8_list_push(allocator, &result, str8_range(word_first, ptr));
			}
			word_first = ptr + 1;
		}
	}
    
	if(word_first < ptr) {
		str8_list_push(allocator, &result, str8_range(word_first, ptr));
	}
    
	return result;
}

internalf void str8_list_pushf(Allocator* allocator, String8List* list, const s8* fmt, ...) {
	String8 str;
	va_list args;
	va_start(args, fmt);
	str = str8fv(allocator, fmt, args);
	str8_list_push(allocator, list, str);
}
// ---- String8 Lists

// ---- String16
internalf String16 str16(u16* str, u64 len) {
	String16 result;
	result.str = str;
	result.len = len;
	return result;
}

internalf String16 str16_cstr(u16* cstr) {
	String16 result;
	result.str = cstr;
	result.len = cstr16_len(cstr);
	return result;
}

internalf String16 str16_copy(Allocator* allocator, String16 str) {
    
	String16 result = str16_nil();
    
	result.str = (u16*)allocate(allocator, sizeof(u16) * (str.len + 1));
	if(result.str) {
		mem_cpy(result.str, str.str, sizeof(u16) * str.len);
		result.len = str.len;
		result.str[result.len] = L'\0';
	}
    
	return result;
}
// ---- String16


// ---- Unicode

// ------ UTF-8
internalf Codepoint utf8_decode(u8* str, u64 len) {
    
	Codepoint result = { 0, 0 };
    
	if(len) {
		if(!(str[0] & 0x80)) {
			result.bytes = str[0];
			result.len = 1;
		} else if((len >= 2) && ((str[0] >> 5) == 0x06)) {
			result.bytes |= (u32)(((str[0] & bitmask5) << 6) | (str[1] & bitmask6));
			result.len = 2;
		} else if((len >= 3) && ((str[0] >> 4) == 0x0E)) {
			result.bytes |= (u32)(((str[0] & bitmask4) << 12) | ((str[1] & bitmask6) << 6) | (str[2] & bitmask6));
			result.len = 3;
		} else if((len >= 4) && ((str[0] >> 3) == 0x1E)) {
			result.bytes |= (u32)(((str[0] & bitmask3) << 18) | ((str[1] & bitmask6) << 12) | ((str[2] & bitmask6) << 6) | (str[3] & bitmask6));
			result.len = 4;
		}
	}
	
	return result;
}

internalf u32 utf8_encode(u8* dest, Codepoint codepoint) {
    
	u32 result;
    
	switch(codepoint.len) {
		default: {
			dest[0] = '?';
			result = 1;
		} break;
		case 1: {
			dest[0] = (u8)codepoint.bytes;
			result = 1;
		} break;
		case 2: {
			dest[0] = (u8)((bitmask2 << 6) | ((codepoint.bytes >> 6) & bitmask5));
			dest[1] = (u8)(bit8 | (codepoint.bytes & bitmask6));
			result = 2;
		} break;
		case 3: {
			dest[0] = (bitmask3 << 5) | ((codepoint.bytes >> 12) & bitmask4);
			dest[1] = (u8)(bit8 | ((codepoint.bytes >> 6) & bitmask6));
			dest[2] = (u8)(bit8 | (codepoint.bytes & bitmask6));
			result = 3;
		} break;
		case 4: {
			dest[0] = (u8)((bitmask4 << 4) | ((codepoint.bytes >> 18) & bitmask3));
			dest[1] = (u8)(bit8 | ((codepoint.bytes >> 12) & bitmask6));
			dest[2] = (u8)(bit8 | ((codepoint.bytes >> 6) & bitmask6));
			dest[3] = (u8)(bit8 | (codepoint.bytes & bitmask6));
			result = 4;
		} break;
	}
	
	return result;
}
// ------ UTF-8

// ------ UTF-16
internalf Codepoint utf16_decode(u16* str, u64 len) {
    
	Codepoint result = { 0, 0 };
    
	if(len) {
		result.bytes = str[0];
		result.len = 1;
		if((len > 1) 
           && (str[0] >= 0xD800) && (str[0] < 0xDC00)
           && (str[1] >= 0xDC00) && (str[1] < 0xE000)) {
			result.bytes = (((u32)(str[0] - 0xD800) << 10) | ((u32)(str[1] - 0xDC00))) + 0x10000;
			result.len = 2;
		}
	}
    
	return result;
}

internalf u32 utf16_encode(u16* dest, Codepoint codepoint) {
    
	u32 result;
    
	switch(codepoint.len) {
		default: {
			dest[0] = (u16)'?';
			result = 1;
		} break;
		case 1: {
			dest[0] = (u16)codepoint.bytes;
			result = 1;
		} break;
		case 2: {
			codepoint.bytes = codepoint.bytes - 0x10000;
			dest[0] = (u16)(0xD800 + (codepoint.bytes >> 10));
			dest[1] = (u16)(0xDC00 + (codepoint.bytes & bitmask10));
			result = 2;
		} break;
	}
    
	return result;
}
// ------ UTF-16

// ------ Conversions
internalf String8 str8_from_16(Allocator* allocator, String16 str) {
    
	String8 result = str8_nil();
	result.str = (u8*)allocate(allocator, sizeof(u8) * (str.len + 1));
	if(result.str) {
		u16* end = str.str + str.len;
		Codepoint codepoint;
		
		for(; str.str < end; str.str += codepoint.len) {
			codepoint = utf16_decode(str.str, (u64)(end - str.str));
			result.len += utf8_encode(result.str + result.len, codepoint);
		}
		result.str[result.len] = '\0';
        
		deallocate(allocator, result.str + result.len + 1, sizeof(u8) * (str.len - result.len));
	} else {
		debug_print_cstr8_lit("str8_from_16: Could not allocate memory for the result string.");
		raise();
	}
    
	return result;
}

internalf String16 str16_from_8(Allocator* allocator, String8 str) {
	
	String16 result = str16_nil();
	result.str = (u16*)allocate(allocator, sizeof(u16) * (str.len + 1));
	if(result.str) {
		u8* end = str.str + str.len;
		Codepoint codepoint;
        
		for(; str.str < end; str.str += codepoint.len) {
			codepoint = utf8_decode(str.str, (u64)(end - str.str));
			result.len += utf16_encode(result.str + result.len, codepoint);
		}
		result.str[result.len] = L'\0';
        
		deallocate(allocator, result.str + result.len + 1, sizeof(u16) * (str.len - result.len));
	} else {
		debug_print_cstr8_lit("str16_from_8: Could not allocate memory for the result string.");
		raise();
	}
    
	return result;
}
// ------ Conversions

// ---- Unicode

// ---- String to Numeric
internalf u32 str8_to_u32(String8 str) {
    
	u32 result;
	u8 min_char;
	u8 max_char;
	u8 char_i;
	u32 multiplier;
	u8 curr_char;
    
	min_char = 48;
	max_char = 57;
    
	multiplier = 1;
	result = 0;
	for(char_i = 0; char_i < str.len; char_i += 1) {
		curr_char = str.str[char_i];
		if((min_char <= curr_char) && (curr_char <= max_char)) {
			result += ((curr_char - min_char) * multiplier);
			multiplier *= 10;
		} else {
			raise();
			result = 0;
			break;
		}
	}
    
	return result;
}
// ---- String to Numeric

// ---- Hashing
#include "../../../externals/inc_begin.h"
#if !defined(XXH_IMPLEMENTATION)
# define XXH_IMPLEMENTATION
# define XXH_STATIC_LINKING_ONLY
# include "../../../externals/xxHash/xxhash.h"
#endif
#include "../../../externals/inc_end.h"


internalf u64 cstr8_hash_with_seed(u64 seed, u8* str, u64 len) {
	u64 key = XXH3_64bits_withSeed(str, len, seed);
	return key;
}

internalf u64 cstr8_hash(u8* str, u64 len) {
	u64 key = cstr8_hash_with_seed(5381, str, len);
	return key;
}
// ---- Hashing

// -- Functions
