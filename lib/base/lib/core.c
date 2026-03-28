

// -- Functions

// ---- Platform
 static OperatingSystem operating_system_from_context(void) {
#if OS_WINDOWS
	return OperatingSystem_Windows;
#elif OS_LINUX
	return OperatingSystem_Linux;
#elif OS_ANDROID
	return OperatingSystem_Android;
#elif OS_MAC
	return OperatingSystem_Mac;
#elif OS_IOS
	return OperatingSystem_Ios;
#else
	return OperatingSystem_Null;
#endif
}

static Architecture architecture_from_context(void) {
#if ARCHITECTURE_X64
	return Architecture_X64;
#elif ARCHITECTURE_X86
	return Architecture_X86;
#elif ARCHITECTURE_ARM
	return Architecture_Arm;
#elif ARCHITECTURE_ARM64
	return Architecture_Arm64;
#else
	return Architecture_Null;
#endif
}

static const s8* cstr8_from_operating_system(OperatingSystem os) {
	const s8* result = "(null)";
	switch(os) {
		case OperatingSystem_Windows: { result = "windows"; } break;
		case OperatingSystem_Linux: { result = "linux"; } break;
		case OperatingSystem_Mac: { result = "mac"; } break;
		case OperatingSystem_Android: { result = "android"; } break;
		case OperatingSystem_Ios: { result = "ios"; } break;
		default: {} break;
	}
	return result;
}

static const s8* cstr8_from_architecture(Architecture arch) {
	const s8* result = "(null)";
	switch(arch) {
		case Architecture_X64: { result = "x64"; } break;
		case Architecture_X86: { result = "x86"; } break;
		case Architecture_Arm: { result = "arm"; } break;
		case Architecture_Arm64: { result = "arm64"; } break;
		default: {} break;
	}
	return result;
}
// ---- Platform

// ---- Memory Operations
internalf void* mem_cpy(void* dst, void* src, u64 size) {
	
	if(size && (dst != src)) {
		if(dst > src) {
			u8* curr_dst = (u8*)dst + size - 1;
			u8* curr_src = (u8*)src + size - 1;
			u8* end_src = (u8*)src - 1;
            
			for(; curr_src != end_src; curr_src -= 1, curr_dst -= 1) {
				*curr_dst = *curr_src;
			}
		} else {
			u8* curr_dst = (u8*)dst;
			u8* curr_src = (u8*)src;
			u8* end_src = curr_src + size;
            
			for(; curr_src != end_src; curr_src += 1, curr_dst += 1) {
				*curr_dst = *curr_src;
			}
		}
	}
    
	return dst;
}

internalf void mem_fill(void* ptr, u8 c, u64 size) {
	u8* curr = (u8*)ptr;
	u8* end = (u8*)ptr + size;
	for(; curr != end; curr += 1) {
		*curr = c;
	}
}

internalf void mem_zero(void* ptr, u64 size) {
	u8* curr = (u8*)ptr;
	u8* end = (u8*)ptr + size;
	for(; curr != end; curr += 1) {
		#ifdef DEBUG
		*curr = 0;  // TODO: still do not understand how to use the non-zero debug value correctly
		#else
		*curr = 0;
		#endif
	}
}

internalf b8 mem_match(void* ptr1, void* ptr2, u64 size) {
	u8* curr1 = (u8*)ptr1;
	u8* curr2 = (u8*)ptr2;
	u8* end = curr1 + size;
	b8 result = 1;
	for(; curr1 != end; curr1 += 1, curr2 += 1) {
		if(*curr1 != *curr2) {
			result = 0;
			break;
		}
	}
	return result;
}

internalf void mem_cpy_cstr8_lit(void* dest, const s8* lit, u64 size) {
	mem_cpy(dest, (void*)lit, size);
}

internalf b8 mem_match_cstr8_lit(void* ptr, const s8* lit, u64 size) {
	return mem_match(ptr, (void*)lit, size);
}


internalf void mem_swap_8(void* a, void* b, u64 n) {
	
	u8* curr1 = (u8*)a;
	u8* curr2 = (u8*)b;
	u8* end = curr1 + n;
	u8 t;
	for(; curr1 != end; curr1 += 1, curr2 += 1) {
		t = *curr1;
		*curr1 = *curr2;
		*curr2 = t;
	}
}

internalf void mem_swap_32(void* a, void* b, u64 n) {
	// n is number of 32-bit blocks
	
	u32* curr1 = (u32*)a;
	u32* curr2 = (u32*)b;
	u32* end = curr1 + n;
	u32 t;
	for(; curr1 != end; curr1 += 1, curr2 += 1) {
		t = *curr1;
		*curr1 = *curr2;
		*curr2 = t;
	}
}

internalf void mem_swap_64(void* a, void* b, u64 n) {
	// n is number of 64-bit blocks
	
	u64* curr1 = (u64*)a;
	u64* curr2 = (u64*)b;
	u64* end = curr1 + n;
	u64 t;
	for(; curr1 != end; curr1 += 1, curr2 += 1) {
		t = *curr1;
		*curr1 = *curr2;
		*curr2 = t;
	}
}
// ---- Memory Operations

// ---- Lower-level memory operations
internalf u64 mem_get_large_page_size(void) {
	u64 result;
	#ifdef MEMORY_SAFE
	result = 0;
	#elif OS_SDL
	result = sdl_mem_get_large_page_size();
	#elif OS_WINDOWS
	result = win32_mem_get_large_page_size();
	#elif OS_LINUX
	result = linux_mem_get_large_page_size();
	#else
	# error "mem_get_large_page_size: not implemented for this operating system."
	#endif
	return result;
}

internalf u64 mem_get_page_size(void) {
	u64 result;
	#ifdef MEMORY_SAFE
	result = 0;
	#elif OS_SDL
	result = sdl_mem_get_page_size();
	#elif OS_WINDOWS
	result = win32_mem_get_page_size();
	#elif OS_LINUX
	result = linux_mem_get_page_size();
	#else
	# error "mem_get_page_size: not implemented for this operating system."
	#endif
	return result;
}


internalf void* mem_reserve(u64 size) {
	void* result = 0;
	if(size) {
		#ifdef MEMORY_SAFE
		#elif OS_SDL
		result = SDL_malloc(size);
		#elif OS_WINDOWS
		result = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
		#elif OS_ANDROID | OS_IOS | OS_LINUX | OS_MAC
		result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		#else
		# error "mem_reserve: not implemented for this operating system."
		#endif
	}
	return result;
}

internalf void mem_commit(void* ptr, u64 size) {
    
	#ifdef MEMORY_SAFE
	#elif OS_SDL
	#elif OS_WINDOWS
	VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
	#elif OS_ANDROID | OS_IOS | OS_LINUX | OS_MAC
	mprotect(ptr, size, PROT_READ | PROT_WRITE);
	#else
	# error "mem_commit: not implemented for this operating system."
	#endif
}

internalf void mem_decommit(void* ptr, u64 size) {
    
	#ifdef MEMORY_SAFE
	#elif OS_SDL
	#elif OS_WINDOWS
	VirtualFree(ptr, size, MEM_DECOMMIT);
	#elif OS_ANDROID | OS_IOS | OS_LINUX | OS_MAC
	madvise(ptr, size, MADV_DONTNEED);
	mprotect(ptr, size, PROT_NONE);
	#else
	# error "mem_decommit: not implemented for this operating system."
	#endif
}

internalf void mem_release(void* ptr, u64 size) {
    
	#ifdef MEMORY_SAFE
	#elif OS_SDL
	SDL_free(ptr);
	#elif OS_WINDOWS
	VirtualFree(ptr, 0, MEM_RELEASE);
	#elif OS_ANDROID | OS_IOS | OS_LINUX | OS_MAC
	munmap(ptr, size);
	#else
	# error "mem_release: not implemented for this operating system."
	#endif
}
// ---- Lower-level memory operations

// ---- Allocate
internalf void* allocate(Allocator* allocator, u64 size) {
	#ifdef MEMORY_SAFE
	return zgc_alloc(size);
	#else
	return allocator->allocate(allocator, size, 16, 1);
	#endif  // MEMORY_SAFE
}

internalf void* allocate_no_zero(Allocator* allocator, u64 size) {
	#ifdef MEMORY_SAFE
	return zgc_alloc(size);
	#else
	return allocator->allocate(allocator, size, 16, 0);
	#endif  // MEMORY_SAFE
}

internalf void* allocate_aligned(Allocator* allocator, u64 size, u64 align) {
	#ifdef MEMORY_SAFE
	return zgc_aligned_alloc(align, size);
	#else
	return allocator->allocate(allocator, size, max(16, align), 1);
	#endif  // MEMORY_SAFE
}

internalf void* allocate_aligned_no_zero(Allocator* allocator, u64 size, u64 align) {
	#ifdef MEMORY_SAFE
	return zgc_aligned_alloc(align, size);
	#else
	return allocator->allocate(allocator, size, max(16, align), 0);
	#endif  // MEMORY_SAFE
}
// ---- Allocate

// ---- Reallocate
internalf void* reallocate(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize) {
	#ifdef MEMORY_SAFE
	return zgc_realloc(ptr, newsize);
	#else
	return allocator->reallocate(allocator, ptr, oldsize, newsize, 16, 1);
	#endif  // MEMORY_SAFE
}

internalf void* reallocate_no_zero(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize) {
	#ifdef MEMORY_SAFE
	return zgc_realloc(ptr, newsize);
	#else
	return allocator->reallocate(allocator, ptr, oldsize, newsize, 16, 0);
	#endif  // MEMORY_SAFE
}

internalf void* reallocate_aligned(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align) {
	#ifdef MEMORY_SAFE
	return zgc_aligned_realloc(ptr, align, newsize);
	#else
	return allocator->reallocate(allocator, ptr, oldsize, newsize, max(16, align), 1);
	#endif  // MEMORY_SAFE
}

internalf void* reallocate_aligned_no_zero(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align) {
	#ifdef MEMORY_SAFE
	return zgc_aligned_realloc(ptr, align, newsize);
	#else
	return allocator->reallocate(allocator, ptr, oldsize, newsize, max(16, align), 0);
	#endif  // MEMORY_SAFE
}
// ---- Reallocate

// ---- Deallocate
internalf void deallocate(Allocator* allocator, void* ptr, u64 size) {
	#ifdef MEMORY_SAFE
	zgc_free(ptr);
	#else
	allocator->deallocate(allocator, ptr, size);
	#endif  // MEMORY_SAFE
}
// ---- Deallocate

// ---- Base Allocator
internalf Allocator allocator_create_default(void) {
	Allocator result;
	#ifndef MEMORY_SAFE
	result.allocate = allocate_;
	result.reallocate = reallocate_;
	result.deallocate = deallocate_;
	#endif
	return result;
}

internalf void* allocate_(Allocator* allocator, u64 size, u64 align, b8 zero) {
	
	void* result = malloc(size);
	if(zero) {
		mem_zero(result, size);
	}

	return result;
}

internalf void* reallocate_(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align, b8 zero) {
	
	void* result = realloc(ptr, newsize);
	if(zero && (newsize > oldsize)) {
		mem_zero((u8*)result + oldsize, newsize - oldsize);
	}
  
	return result;
}

internalf void deallocate_(Allocator* allocator, void* ptr, u64 size) {
	free(ptr);
}
// ---- Base Allocator

// ---- Pointers

// ------ Handles
internalf Handle handle_from_ptr(void* ptr) {
    
	Handle result;
    
	#ifdef MEMORY_SAFE
	result.ptr = ptr;
	#elif (ADDR_SIZE == 64)
	result.u64[0] = addr_from_ptr(ptr);
	#elif (ADDR_SIZE == 32)
	result.u32[0] = addr_from_ptr(ptr);
	#endif  // MEMORY_SAFE
    
	return result;
}

internalf void* ptr_from_handle(Handle handle) {
    
	void* result;
    
	#ifdef MEMORY_SAFE
	result = handle.ptr;
	#elif (ADDR_SIZE == 64)
	result = ptr_from_addr(handle.u64[0]);
	#elif (ADDR_SIZE == 32)
	result = ptr_from_addr(handle.u32[0]);
	#endif  // MEMORY_SAFE
    
	return result;
}

internalf b8 handle_is_valid(Handle handle) {
	return (b8)(ptr_from_handle(handle) != (void*)0);
}
// ------ Handles

// ------ Tagged Pointers
internalf void* tagged_ptr_get(void* ptr, enum8(TaggedPointerBit) bitmask) {
	return ptr_from_addr(addr_from_ptr(ptr) & (bitmask64 ^ bitmask));
}

internalf enum8(TaggedPointerBit) tagged_ptr_get_bits(void* ptr, enum8(TaggedPointerBit) bitmask) {
	return (enum8(TaggedPointerBit))(addr_from_ptr(ptr) & bitmask);
}

internalf void* tagged_ptr_set_bits(void* ptr, enum8(TaggedPointerBit) bitmask, enum8(TaggedPointerBit) bits) {
	return ptr_from_addr((addr_from_ptr(ptr) & (bitmask64 ^ bitmask)) | (u64)bits);
}

internalf void* tagged_ptr_set_bits_from_ptr(void* ptr, void* src_ptr, enum8(TaggedPointerBit) bitmask) {
	return tagged_ptr_set_bits(ptr, bitmask, tagged_ptr_get_bits(src_ptr, bitmask));
}
// ------ Tagged Pointers

// ---- Pointers

// ---- Data Types

// ------ Float/Double Infinite Values
internalf r32 inf_r32(void) {
	union { r32 f; u32 u; } r;
	r.u = 0x7f800000;
	return r.f;
}

internalf r32 neg_inf_r32(void) {
	union { r32 f; u32 u; } r;
	r.u = 0xff800000;
	return r.f;
}

internalf r64 inf_r64(void) {
	union { r64 f; u64 u; } r;
	r.u = 0x7ff0000000000000;
	return r.f;
}

internalf r64 neg_inf_r64(void) {
	union { r64 f; u64 u; } r;
	r.u = 0xfff0000000000000;
	return r.f;
}
// ------ Float/Double Infinite Values

// ------ Sign Extend
internalf s16 sign_extend_u8(u8 b) {
	
	s16 result = 0;
	
	if(bitset_bit_is_set(b, 7)) {
		result = (s16)word_from_bytes(U8_MAX, b);
	} else {
		result = (s16)b;
	}
    
	return result;
}
// ------ Sign Extend

// ------ Signed Encode/Decode
internalf u64 encode_u64_from_s64(s64 x) {
	return ((u64)x << 1) | ((u64)x >> 63);
}

internalf s64 decode_s64_from_u64(u64 x) {
	return (s64)((x << 63) | (x >> 1));
}
// ------ Signed Encode/Decode

// ---- Data Types



// ---- Array

// ------ Plain Data
internalf void* array_from_member(Allocator* allocator, void* data, u32 len, u64 slot_size, u64 member_offset, u64 member_size) {
    
	void* result;
	u32 i;
    
	result = allocate(allocator, member_size * len);
	for(i = 0; i < len; i += 1) {
		mem_cpy((u8*)result + (i * member_size), (u8*)data + (i * slot_size) + member_offset, member_size);
	}
    
	return result;
}
// ------ Plain Data

// ------ Array

// -------- Constructors/Destructors
internalf Array* array_alloc(u64 slot_size, u32 cap) {
    
	Array* result;
	ArenaParams params;
	Arena* arena;
	u64 total_size = slot_size * cap;
    
	params.arena = 0;
	params.res_size = align_up_pow2(total_size, megabytes(64));
	params.res_size = max(params.res_size, ARENA_RESERVE_SIZE_DEFAULT);
	params.cmt_size = align_up_pow2(total_size, megabytes(1));
	params.cmt_size = max(params.cmt_size, ARENA_COMMIT_SIZE_DEFAULT);
	params.flags |= ArenaFlag_NoChain;
	arena = arena_allocate(&params);
    
	result = array_alloc_custom(&arena->allocator, slot_size, cap);
	
	if(!result) {
		arena_deallocate(arena);
		result = (Array*)0;
		raise();
	}
	
	return result;
}

internalf Array* array_alloc_custom(Allocator* allocator, u64 slot_size, u32 cap) {
    
	Array* result;
	
	result = (Array*)allocate(allocator, sizeof(Array));
	result->allocator = allocator;
	result->data = allocate(allocator, slot_size * (u64)cap);
	
	if(result->data) {
		result->len = 0;
		result->cap = cap;
		result->slot_size = slot_size;
	} else {
		deallocate(result->allocator, result, sizeof(Array));
		result = (Array*)0;
		raise();
	}
	
	return result;
}

internalf void array_dealloc(Array* array) {
	#ifdef MEMORY_SAFE
	array_dealloc_custom(array); 
	#endif  // MEMORY_SAFE
	arena_deallocate((Arena*)array->allocator);
}

internalf void array_dealloc_custom(Array* array) {
	deallocate(array->allocator, array->data, (u64)array->cap * array->slot_size);
	deallocate(array->allocator, array, sizeof(Array));
}

internalf Array* array_copy_subarray(Array* array, u32 start_index, u32 copy_n) {

	Array* result;
	ArenaParams params;
	Arena* arena;

	if((start_index + copy_n) <= array->len) {
		params = arena_params_default_static();
		arena = arena_allocate(&params);
		result = (Array*)allocate(&arena->allocator, sizeof(Array));
		result->allocator = &arena->allocator;

		result->slot_size = array->slot_size;
		array_grow(result, array->cap);
		result->len = copy_n;
		mem_cpy(result->data, (u8*)array->data + (start_index * array->slot_size), array->slot_size * copy_n);
	} else {
		debug_print_cstr8_lit("array_copy_subarray: start_index + copy_n have to be inside array len bound.");
		raise();
		result = (Array*)0;
	}

	return result;
}

internalf Array* array_copy(Array* array) {
	return array_copy_subarray(array, 0, array->len);
}
// -------- Constructors/Destructors

internalf b8 array_grow(Array* array, u32 cap_inc) {
	
	b8 result = 0;
	array->data = reallocate(array->allocator, array->data, array->slot_size * array->cap, array->slot_size * (array->cap + cap_inc));
	
	if(array->data) {
		array->cap += cap_inc;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_grow: Could not grow array.");
		raise();
	}
    
	return result;
}

internalf b8 array_push(Array* array, void* el, u32 i, u32 cap_inc) {

	if((array->len == array->cap) && (cap_inc > 0)) {	
		array_grow(array, cap_inc);
	}
	return array_static_push(array, el, i);
}

internalf b8 array_push_back(Array* array, void* el, u32 cap_inc) {
    
	if((array->len == array->cap) && (cap_inc > 0)) {	
		array_grow(array, cap_inc);
	}  
	return array_static_push_back(array, el);
}

internalf b8 array_push_front(Array* array, void* el, u32 cap_inc) {
    
	if((array->len == array->cap) && (cap_inc > 0)) {	
		array_grow(array, cap_inc);
	}
	return array_static_push_front(array, el);
}

internalf b8 array_static_push(Array* array, void* el, u32 i) {

	b8 result = 0;

	if(array->len < array->cap) {
		if(i <= array->len) {
			void* slot = (u8*)(array->data) + i * array->slot_size;
            
			if(i < array->len) {  // shift elements to the right
				mem_cpy((u8*)slot + array->slot_size, slot, (array->len - i) * array->slot_size);
			}
			mem_cpy(slot, el, array->slot_size);
            
			array->len += 1;
			result = 1;
		} else {
			debug_printf("Cannot push an element over the current array length: index = %d, len = %d", i, array->len);
			raise();
		}
	} else {
		debug_print_cstr8_lit("array_static_push: Array is full.");
	}

	return result;
}

internalf b8 array_static_push_back(Array* array, void* el) {

	b8 result = 0;

	if(array->len < array->cap) {
		mem_cpy((u8*)(array->data) + array->len * array->slot_size, el, array->slot_size);
		array->len += 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_static_push_back: Array is full.");
	}

	return result;
}

internalf b8 array_static_push_front(Array* array, void* el) {

	b8 result = 0;

	if(array->len < array->cap) {
		mem_cpy((u8*)(array->data) + array->slot_size, array->data, array->slot_size * array->len);
		mem_cpy(array->data, el, array->slot_size);
		array->len += 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_static_push_front: Array is full.");
	}

	return result;
}

internalf void array_push_subarray(Array* dest, Array* src, u32 dest_start_index, u32 src_start_index, u32 copy_n) {

	void* dest_start;

	if(dest->slot_size != src->slot_size) {
		debug_print_cstr8_lit("array_push_subarray: arrays dest and src need to have slots of the same size.");
		raise();
		return;
	}
	
	if(dest_start_index >= dest->len) {
		debug_print_cstr8_lit("array_push_subarray: dest_start_index outside dest array len bound.");
		raise();
		return;
	}

	if((src_start_index + copy_n) > src->len) {
		debug_print_cstr8_lit("array_push_subarray: src_start_index + copy_n outside src array len bound.");
		raise();
		return;
	}

	if((dest->len + copy_n) > dest->cap) {
		array_grow(dest, copy_n);	
	}
	
	dest_start = array_get(dest, dest_start_index);
	mem_cpy(array_get(dest, dest_start_index + copy_n), dest_start, dest->slot_size * (dest->len - dest_start_index));
	mem_cpy(dest_start, array_get(src, src_start_index), dest->slot_size * copy_n);
}

internalf b8 array_pop(Array* array, u32 i, b8 ordered) {

	b8 result = 0;

	if(i < array->len) {
		void* slot = (u8*)array->data + i * array->slot_size;
        
		if(i < (array->len - 1)) {
			if(ordered) {
				u64 quantity = (array->len - i) * array->slot_size;
				mem_cpy(slot, (u8*)slot + array->slot_size, quantity);
			} else {
				void* last = (u8*)array->data + (array->len - 1) * array->slot_size;
				mem_cpy(slot, last, array->slot_size);
			}
		}

		array->len -= 1;
		result = 1;
	} else if(array->len == 0) {
		debug_print_cstr8_lit("array_pop: Array is empty.");
	} else {
		debug_printf("array_pop: Cannot pop an element over the current array length: index = %d, len = %d", i, array->len);
		raise();
	}

	return result;
}

internalf b8 array_pop_back(Array* array) {
	
	b8 result = 0;

	if(array->len) {
		array->len -= 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_pop_back: Array is empty.");
	}

	return result;
}

internalf b8 array_pop_front(Array* array, b8 ordered) {
	
	b8 result = 0;

	if(array->len) {
		if(ordered) {
			mem_cpy(array->data, (u8*)array->data + array->slot_size, (array->len - 1) * array->slot_size);
		} else {
			mem_cpy(array->data, (u8*)array->data + ((array->len - 1) * array->slot_size), array->slot_size);
		}
		array->len -= 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_pop_front: Array is empty.");
	}

	return result;
}

internalf void* array_get(Array* array, u32 i) {
    
	void* result = 0;

	if(i < array->len) {
		result = (u8*)array->data + (array->slot_size * i);
	} else if(array->len == 0) {
		debug_print_cstr8_lit("array_get_front: Array is empty.");
	} else {
		debug_printf("array_get: Cannot get an element over the current array length: index = %d, len = %d", i, array->len);
		raise();
	}

	return result;
}

internalf void* array_get_back(Array* array) {

	void* result = 0;

	if(array->len) {
		result = (u8*)array->data + (array->len - 1) * array->slot_size;
	} else {
		debug_print_cstr8_lit("array_get_back: Array is empty.");
	}

	return result;
}

internalf void* array_get_front(Array* array) {

	void* result = 0;

	if(array->len) {
		result = array->data;
	} else {
		debug_print_cstr8_lit("array_get_front: Array is empty.");
	}

	return result;
}

internalf b8 array_try_get(Array* array, u32 i, void* out) {
	
	b8 result = 0;

	void* data = array_get(array, i);
	if(data) {
		mem_cpy(out, data, array->slot_size);
		result = 1;
	}

	return result;
}

internalf b8 array_try_get_back(Array* array, void* out) {

	b8 result = 0;

	void* data = array_get_back(array);
	if(data) {
		mem_cpy(out, data, array->slot_size);
		result = 1;
	}

	return result;
}

internalf b8 array_try_get_front(Array* array, void* out) {

	b8 result = 0;

	void* data = array_get_front(array);
	if(data) {
		mem_cpy(out, data, array->slot_size);
		result = 1;
	}

	return result;
}
// ------ Array

// ------ Stack
internalf b8 array_stack_push(Array* array, void* el, u32 cap_inc) {
	return array_push_back(array, el, cap_inc);
}

internalf b8 array_stack_static_push(Array* array, void* el) {
	return array_static_push_back(array, el);
}

internalf b8 array_stack_pop(Array* array) {
	return array_pop_back(array);
}

internalf void* array_stack_get(Array* array) {
	return array_get_back(array);
}

internalf b8 array_stack_try_get(Array* array, void* out) {
	return array_try_get_back(array, out);
}
// ------ Stack

// ------ Queue
internalf ArrayQueue* array_queue_alloc(u64 slot_size, u32 cap) {
    
	ArrayQueue* result;
	ArenaParams params;
	Arena* arena;
	u64 total_size = slot_size * cap;
    
	params.arena = 0;
	params.res_size = align_up_pow2(total_size, megabytes(64));
	params.res_size = max(params.res_size, ARENA_RESERVE_SIZE_DEFAULT);
	params.cmt_size = align_up_pow2(total_size, megabytes(1));
	params.cmt_size = max(params.cmt_size, ARENA_COMMIT_SIZE_DEFAULT);
	params.flags |= ArenaFlag_NoChain;
	arena = arena_allocate(&params);
    
	result = array_queue_alloc_custom(&arena->allocator, slot_size, cap);
	
	if(!result) {
		arena_deallocate(arena);
		result = (ArrayQueue*)0;
		raise();
	}
	
	return result;
}

internalf ArrayQueue* array_queue_alloc_custom(Allocator* allocator, u64 slot_size, u32 cap) {
    
	ArrayQueue* result;
	
	result = (ArrayQueue*)allocate(allocator, sizeof(ArrayQueue));
	result->array.allocator = allocator;
	result->array.data = allocate(allocator, slot_size * (u64)cap);
	
	if(result->array.data) {
		result->array.len = 0;
		result->array.cap = cap;
		result->array.slot_size = slot_size;
	} else {
		deallocate(result->array.allocator, result, sizeof(ArrayQueue));
		result = (ArrayQueue*)0;
		raise();
	}
	
	return result;
}

internalf void array_queue_dealloc(ArrayQueue* array) {
	#ifdef MEMORY_SAFE
	array_queue_dealloc_custom(array);
	#endif  // MEMORY_SAFE
	arena_deallocate((Arena*)array->array.allocator);
}

internalf void array_queue_dealloc_custom(ArrayQueue* array) {
	deallocate(array->array.allocator, array->array.data, (u64)array->array.cap * array->array.slot_size);
	deallocate(array->array.allocator, array, sizeof(ArrayQueue));
}


internalf b8 array_queue_grow(ArrayQueue* array, u32 cap_inc) {
    
	b8 result = 0;

	if(array_grow(&array->array, cap_inc)) {
		if(array->array.len && (array->last <= array->first)) {
			// copy memory from the beginning of the array to the front
			u32 n_els_to_cpy = min(cap_inc, array->last);
			void* dest = (u8*)(array->array.data) + (array->array.cap - cap_inc) * array->array.slot_size;
			mem_cpy(dest, array->array.data, n_els_to_cpy * array->array.slot_size);
            
			// update last cursor and, if there is data left in the front of the array, shift it back
			if(n_els_to_cpy < array->last) {
				mem_cpy(array->array.data, (u8*)(array->array.data) + n_els_to_cpy * array->array.slot_size, (array->last - n_els_to_cpy) * array->array.slot_size);
				array->last -= n_els_to_cpy;
			} else {
				array->last = array->array.cap - cap_inc + n_els_to_cpy;
			}
		}
		result = 1;
	}
    
	return result;
}

internalf b8 array_queue_push_front(ArrayQueue* array, void* el, u32 cap_inc) {
    
	if((array->array.len == array->array.cap) && cap_inc > 0) {
		array_queue_grow(array, cap_inc);
	}
	return array_queue_static_push_front(array, el);
}

internalf b8 array_queue_push_back(ArrayQueue* array, void* el, u32 cap_inc) {
	
	if((array->array.len == array->array.cap) && cap_inc > 0) {
		array_queue_grow(array, cap_inc);
	}  
  return array_queue_static_push_back(array, el);
}

internalf b8 array_queue_static_push_front(ArrayQueue* array, void* el) {

	b8 result = 0;

	if(array->array.len < array->array.cap) {
		if(array->first == 0) {
			array->first = array->array.cap - 1;
		} else {
			array->first -= 1;
		}
        
		mem_cpy((u8*)(array->array.data) + array->first * array->array.slot_size, el, array->array.slot_size);
        
		array->array.len += 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_queue_static_push_front: Array is full.");
	}

	return result;
}

internalf b8 array_queue_static_push_back(ArrayQueue* array, void* el) {

	b8 result = 0;

	if(array->array.len < array->array.cap) {
		mem_cpy((u8*)(array->array.data) + array->last * array->array.slot_size, el, array->array.slot_size);
        
		if(array->last < (array->array.cap - 1)) {
			array->last += 1;
		} else {
			array->last = 0;
		}
        
		array->array.len += 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_queue_static_push_back: Array is full.");
	}

	return result;
}

internalf b8 array_queue_pop_front(ArrayQueue* array) {
    
	b8 result = 0;

	if(array->array.len) {

		if(array->first < (array->array.cap - 1)) {
			array->first += 1;
		} else {
			array->first = 0;
		}
        
		array->array.len -= 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_queue_pop_front: Array is empty.");
	}

	return result;
}

internalf b8 array_queue_pop_back(ArrayQueue* array) {
	
	b8 result = 0;

	if(array->array.len) {
		if(array->last > 0) {
			array->last -= 1;
		} else {
			array->last = array->array.cap - 1;
		}

		array->array.len -= 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("array_queue_pop_back: Array is empty.");
	}

	return result;
}

internalf void* array_queue_get_front(ArrayQueue* array) {
	
	void* result = 0;

	if(array->array.len) {
		result = (u8*)array->array.data + array->first * array->array.slot_size;
	} else {
		debug_print_cstr8_lit("array_queue_get_front: Array is empty.");
	}
	
	return result;
}

internalf void* array_queue_get_back(ArrayQueue* array) {
	
	void* result = 0;

	if(array->array.len) {
		u32 i;
		if(array->last > 0) {
			i = array->last - 1;
		} else {
			i = 0;
		}
     
		result = (u8*)array->array.data + i * array->array.slot_size;
	} else {
		debug_print_cstr8_lit("array_queue_get_back: Array is empty.");
	}

	return result;
}

internalf b8 array_queue_try_get_front(ArrayQueue* array, void* out) {

	b8 result = 0;
	
	void* data = array_queue_get_front(array);
	if(data) {
		mem_cpy(out, data, array->array.slot_size);
		result = 1;
	}

	return result;
}

internalf b8 array_queue_try_get_back(ArrayQueue* array, void* out) {

	b8 result = 0;

	void* data = array_queue_get_back(array);
	if(data) {
		mem_cpy(out, data, array->array.slot_size);
		result = 1;
	}

	return result;
}
// ------ Queue

// ---- Array



// ---- Linked Lists

// ------ Singly Linked

// -------- Queues
internalf void sll_queue_init(Sll* header_first, Sll* header_last) {
	header_first->next = header_first;
	header_last->next = header_first;
}

internalf void sll_queue_push_back(Sll* header_first, Sll* header_last, Sll* node) {
	node->next = header_first;
	header_last->next->next = node;  // works the first time because of sll_queue_init 2nd line
	header_last->next = node;
}

internalf void sll_queue_push_front(Sll* header_first, Sll* header_last, Sll* node) {
	node->next = header_first->next;
	header_first->next = node;
	if(header_last->next == header_first) {  // in case of the first insert
		header_last->next = node;
	}
}

internalf Sll* sll_queue_pop_front(Sll* header_first, Sll* header_last) {
	
	Sll* result = (Sll*)0;
	
	if(header_first->next != header_first) {
		result = header_first->next;
		header_first->next = header_first->next->next;
		result->next = (Sll*)0;
		if(header_first->next == header_first) {
			header_last->next = header_first;
		}
	}

	return result;
}

internalf Sll* sll_queue_get_front(Sll* header_first, Sll* header_last) {
	
	Sll* result = (Sll*)0;
    
	if(header_first->next != header_first) {
		result = header_first->next;
	}
    
	return result;
}

internalf Sll* sll_queue_get_back(Sll* header_first, Sll* header_last) {
	
	Sll* result = (Sll*)0;
    
	if(header_last->next != header_first) {
		result = header_last->next;
	}
    
	return result;
}

internalf Sll* sll_queue_get_next(Sll* curr, Sll* header_first) {
	
	Sll* result = (Sll*)0;

	if(curr->next != header_first) {
		result = curr->next;
	}

	return result;
}

internalf Sll* sll_queue_circular_get_next(Sll* curr, Sll* header_first) {

	Sll* result = (Sll*)0;

	if(curr->next != header_first) {
		result = curr->next;
	} else if(curr->next->next != header_first) {
		result = curr->next->next;
	}

	return result;
}
// -------- Queues

// -------- Stacks
internalf void sll_stack_init(Sll* header) {
	header->next = header;
}

internalf void sll_stack_push(Sll* header, Sll* node) {
	node->next = header->next;
	header->next = node;
}

internalf Sll* sll_stack_pop(Sll* header) {
	
	Sll* result = (Sll*)0;
	
	if(header->next != header) {
		result = header->next;
		header->next = header->next->next;
		result->next = (Sll*)0;
	}
	
	return result;
}

internalf Sll* sll_stack_get(Sll* header) {
	
	Sll* result = (Sll*)0;
	
	if(header->next != header) {
		result = header->next;
	}
	
	return result;
}

internalf Sll* sll_stack_get_next(Sll* curr, Sll* header) {
	
	Sll* result = (Sll*)0;

	if(curr->next != header) {
		result = curr->next;
	}

	return result;
}

internalf Sll* sll_stack_circular_get_next(Sll* curr, Sll* header) {
	
	Sll* result = (Sll*)0;

	if(curr->next != header) {
		result = curr->next;
	} else if(curr->next->next != header) {
		result = curr->next->next;
	}

	return result;
}
// -------- Stacks

// ------ Singly Linked

// ------ Doubly Linked
internalf void dll_init(Dll* header) {
	header->next = header;
	header->prev = header;
}

internalf void dll_push(Dll* header, Dll* prev, Dll* node) {
	
	if(prev == header) {  // push front
		node->prev = header->next->prev;
		node->next = header->next;
		header->next->prev = node;
		header->next = node;
	} else if(prev == header->prev) {   // push back
		node->prev = header->prev;
		node->next = header->prev->next;
		header->prev->next = node;
		header->prev = node;
	} else {
		node->next = prev->next;
		node->prev = prev;
		prev->next->prev = node;
		prev->next = node;
	}
}

internalf void dll_push_front(Dll* header, Dll* node) {
	dll_push(header, header, node);
}

internalf void dll_push_back(Dll* header, Dll* node) {
	dll_push(header, header->prev, node);
}

internalf Dll* dll_pop_front(Dll* header) {
	
	Dll* result = (Dll*)0;
    
	if(header->next != header) {
		result = header->next;
		dll_remove(header->next);
	}
    
	return result;
}

internalf Dll* dll_pop_back(Dll* header) {
	
	Dll* result = (Dll*)0;
    
	if(header->prev != header) {
		result = header->prev;
		dll_remove(header->prev);
	}
    
	return result;
}

internalf Dll* dll_get_front(Dll* header) {
    
	Dll* result = (Dll*)0;
    
	if(header->next != header) {
		result = header->next;
	}
    
	return result;
}

internalf Dll* dll_get_back(Dll* header) {
    
	Dll* result = (Dll*)0;
    
	if(header->prev != header) {
		result = header->prev;
	}
    
	return result;
}

internalf void dll_remove(Dll* node) {
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->next = (Dll*)0;
	node->prev = (Dll*)0;
}

internalf Dll* dll_get_next(Dll* curr, Dll* header) {

	Dll* result = (Dll*)0;

	if(curr->next != header) {
		result = curr->next;
	}

	return result;
}

internalf Dll* dll_get_prev(Dll* curr, Dll* header) {

	Dll* result = (Dll*)0;

	if(curr->prev != header) {
		result = curr->prev;
	}

	return result;
}

internalf Dll* dll_circular_get_next(Dll* curr, Dll* header) {

	Dll* result = (Dll*)0;

	if(curr->next != header) {
		result = curr->next;
	} else if(curr->next->next != header) {
		result = curr->next->next;
	}

	return result;
}

internalf Dll* dll_circular_get_prev(Dll* curr, Dll* header) {

	Dll* result = (Dll*)0;

	if(curr->prev != header) {
		result = curr->prev;
	} else if(curr->prev->prev != header) {
		result = curr->prev->prev;
	}

	return result;
}
// ------ Doubly Linked

// ---- Linked Lists

// -- Functions
