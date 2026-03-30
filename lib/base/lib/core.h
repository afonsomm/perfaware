

/**

----- Platform -----
Macros that define which compiler, CPU architecture, OS, and language one is currently using.
----- Platform -----

----- Memory -----
Basic memory operations that also abstract the OS layer, and define a base allocator interface.
All the custom allocators have to adhere to this interface.
If compiled with MEMORY_SAFE defined, then Fill-C allocation functions replace all the calls to 
"allocate(...)", "reallocate(...)", and "deallocate(...)".
If compiled with the address sanitizer turned on, then allocators can use the "asan_poison_memory_region(...)" and
"asan_unpoison_memory_region(...)" macros to mark regions of allocated memory that are not supposed to be touched,
and that are available, respectively.
----- Memory -----

----- Macro Helpers -----
----- Macro Helpers -----

----- Pointers -----
Basic pointer arithmetic macros and pointer tagging functions.
In pointer tagging, it only uses the lower available bits of an address, assuming a maximum of 3 free bits, meaning 16 byte aligned allocations.
----- Pointers -----

----- Data Types -----
Primitive data types, thresholds, and conversions.
----- Data Types -----

----- Array -----
Basic array data structure, that supports resizing, and stack and queue on an array.
----- Array -----

----- Linked Lists -----
Linked lists with a circular implementation.
For singly linked lists (Sll) to be circular, the last element of the list points to the head of the list, which in turn points to the first element. 
This means the list starts as "head.next". For queues implemented as singly linked lists, there is also a header for the last element, which points to the last element, 
which in turn points to the header for the first element.
For doubly linked lists (Dll), the "header.next" and "header.prev" point to the first and last elements, respectively.
For an object to belong to a linked list, we embed one of these structs (Sll or Dll) in the struct corresponding to that object. 
When transversing the list, we use container_of(object.node.next, ObjectType, node_attribute_name), to get the next element in the list.
----- Linked Lists -----

*/


#ifndef BASE_CORE_H
# define BASE_CORE_H


// -- Includes
#ifdef MEMORY_SAFE
# include <stdfil.h>
#endif  // MEMORY_SAFE
// -- Includes


// -- Pre-processor Defines

// ---- Platform (https://sourceforge.net/p/predef/wiki/Home)

// ------ Architecture
#if defined(__amd64__) || defined(_M_AMD64)
# define ARCHITECTURE_X64 1
#elif defined(__i386__) || defined(_M_IX86)
# define ARCHITECTURE_X86 1
#elif defined(__arm__) || defined(_M_ARM)
# define ARCHITECTURE_ARM 1
#elif defined(__aarch64__)
# define ARCHITECTURE_ARM64 1
#else
# error "Architecture not supported."
#endif

#ifndef ARCHITECTURE_X64
# define ARCHITECTURE_X64 0
#endif
#ifndef ARCHITECTURE_X86
# define ARCHITECTURE_X86 0
#endif
#ifndef ARCHITECTURE_ARM
# define ARCHITECTURE_ARM 0
#endif
#ifndef ARCHITECTURE_ARM64
# define ARCHITECTURE_ARM64 0
#endif
// ------ Architecture

// ------ Compiler
#if defined(__clang__)
# define COMPILER_CLANG 1
#elif defined(__GNUC__)
# define COMPILER_GCC 1
#elif defined(_MSC_VER)
# define COMPILER_MSVC 1
#else
# error "Compiler not supported."
#endif

#ifndef COMPILER_CLANG
# define COMPILER_CLANG 0
#endif
#ifndef COMPILER_GCC
# define COMPILER_GCC 0
#endif
#ifndef COMPILER_MSVC
# define COMPILER_MSVC 0
#endif
// ------ Compiler

// ------ Operating System
#if defined(_WIN32)
# define OS_WINDOWS 1
#elif defined(__APPLE__)
# include <TargetConditionals.h>
# if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#  define OS_IOS 1
# elif TARGET_OS_MAC
#  define OS_MAC 1
# else
#  error "Operating System not supported."
# endif
#elif defined(__ANDROID__)
# define OS_ANDROID 1
#elif defined(__linux__)
# define OS_LINUX 1
#else
# error "Operating System not supported."
#endif

#ifndef OS_WINDOWS
# define OS_WINDOWS 0
#endif
#ifndef OS_IOS
# define OS_IOS 0
#endif
#ifndef OS_MAC
# define OS_MAC 0
#endif
#ifndef OS_ANDROID
# define OS_ANDROID 0
#endif
#ifndef OS_LINUX
# define OS_LINUX 0
#endif
// ------ Operating System

// ------ External Libraries that abstract the plaftorm and OS
#ifdef OS_SDL
# define OS_SDL 1
#else
# define OS_SDL 0
#endif
// ------ External Libraries that abstract the plaftorm and OS

// ---- Architecture number of bits
#if ARCHITECTURE_X64 || ARCHITECTURE_ARM64
# define ADDR_SIZE 64
#else 
# define ADDR_SIZE 32
#endif
// ---- Architecture number of bits

// ---- C++
#ifdef __cplusplus
# define C_LINKAGE_BEGIN extern "C" {
# define C_LINKAGE_END   }
# define C_LINKAGE       extern "C"
#else
# define C_LINKAGE_BEGIN
# define C_LINKAGE_END
# define C_LINKAGE
#endif
// ---- C++

// ---- Platform

// ---- Helpers
#define do_once(statement) do { statement; } while(0)

// ------ Defer loops
// NOTE: commented out because it is not c89 compliant and the only use case that i have used it for is for GUI builder code, 
//       where there are a lot of push/pop from styling stacks.
// #define defer(begin, end)       for(u32 _i_ = ((begin), 0); !_i_; _i_ += 1, (end))
// #define defer_check(begin, end) for(u32 _i_ = !!(begin); (_i_ ? (_i_) : ((end), _i_)); _i_ = !_i_)
// ------ Defer loops

// ------ Tokens
#define glue_(a, b)   a##b
#define glue(a, b)    glue_(a, b)
// ----- Tokens

// ---- Helpers

// ---- Data Types
#define global        static
#define internalf     static
#define local_persist static

#define enum8(T)  u8
#define enum16(T) u16
#define enum32(T) u32
#define enum64(T) u64

// ------ Integer Thresholds
#define S8_MIN  0x80
#define S16_MIN 0x8000
#define S32_MIN 0x80000000
#define S64_MIN 0x8000000000000000ll

#define S8_MAX  0x7f
#define S16_MAX 0x7fff
#define S32_MAX 0x7fffffff
#define S64_MAX 0x7fffffffffffffffll

#define U8_MAX  0xff
#define U16_MAX 0xffff
#define U32_MAX 0xffffffff
#define U64_MAX 0xffffffffffffffffull
// ------ Integer Thresholds

// ---- Data Types

// ---- Memory

// ------ Sizes
#define kilobytes(n) (1024u * n)
#define megabytes(n) (1048576u * n)
#define gigabytes(n) (1073741824u * n)
#define terabytes(n) (1099511627780ull * n)
// ------ Sizes

// ------ Address Sanitizer
#if COMPILER_MSVC
# if defined(__SANITIZE_ADDRESS__)
# if ARCHITECTURE_X64
#  pragma comment(lib, "clang_rt.asan-x86_64.lib")
#  define ASAN_ENABLED 1
#  define NO_ASAN __declspec(no_sanitize_address)
# elif ARCHITECTURE_X86
#  pragma comment(lib, "clang_rt.asan-i386.lib")
#  define ASAN_ENABLED 1
#  define NO_ASAN __declspec(no_sanitize_address)
# endif
#endif
#elif COMPILER_CLANG || COMPILER_GCC
# if defined(__has_feature)
#  if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#   if ARCHITECTURE_X64
#    pragma comment(lib, "clang_rt.asan-x86_64.lib")
#   elif ARCHITECTURE_X86
#    pragma comment(lib, "clang_rt.asan-i386.lib")
#   endif
#   define ASAN_ENABLED 1
#   define NO_ASAN __attribute__((no_sanitize("address")))
#  endif
# endif
#endif

#if ASAN_ENABLED
C_LINKAGE void __asan_poison_memory_region(void const volatile * addr, u64 size);
C_LINKAGE void __asan_unpoison_memory_region(void const volatile * addr, u64 size);
# define asan_poison_memory_region(addr, size)   __asan_poison_memory_region((addr), (size))
# define asan_unpoison_memory_region(addr, size) __asan_unpoison_memory_region((addr), (size))
#else
# define asan_poison_memory_region(addr, size)   ((void)(addr), (void)(size))
# define asan_unpoison_memory_region(addr, size) ((void)(addr), (void)(size))
#endif
// ------ Address Sanitizer

// ---- Memory

// ---- Pointers

// ------ Address to/from Pointer Conversions
#define ptr_from_addr(addr)  (void*)(addr)
#define addr_from_ptr(ptr)   (u64)(ptr)
// ------ Address to/from Pointer Conversions

// ------ Struct Utils
#define member_offset(T, m)             addr_from_ptr(&((T *)0)->m)
#define member_of(ptr, T, m)            (T *)(&(ptr)->m)
#define container_of(ptr, T, m)         (T *)((u8*)(ptr) - member_offset(T, m))
#define container_of_checked(ptr, T, m)  ((ptr) ? container_of(ptr, T, m) : (T*)0)

#define member_of_(ptr, o)      ((u8*)(ptr) + (o))
#define container_of_(ptr, o)   ((u8*)(ptr) - (o))
// ------ Struct Utils

// ---- Pointers

// ---- Debug

// ------ Asserts
#if COMPILER_MSVC
#define trap() __debugbreak()
#elif COMPILER_CLANG || COMPILER_GCC
#define trap() __builtin_trap()
#else
# error "Unknown trap intrinsic for this compiler."
#endif

#ifdef DEBUG
# define raise()      do_once(trap())
# ifdef assert
#  undef assert
# endif
# define assert(cond) do_once(if(!(cond)) { trap(); })
#else
# define raise()      (void)(0)
# define assert(cond) (void)(cond)
#endif
// ------ Asserts

// ------ Uninitialized memory filling for debugging
// TODO: not being used because i still do not understand this.
#ifdef DEBUG
# if COMPILER_MSVC
#  define DEFAULT_NIL 0xCD
# elif COMPILER_GCC
#  define DEFAULT_NIL 0xFA
# elif COMPILER_CLANG
#  define DEFAULT_NIL 0xFB
# endif
#else
# define DEFAULT_NIL 0x00
#endif
// ------ Uninitialized memory filling for debugging

// ---- Debug

// ---- Array
#define array_cap(a) (sizeof(a) / sizeof((a)[0]))
// ---- Array

// -- Pre-processor Defines


// -- Enums/Flags

// ---- Platform
typedef enum OperatingSystem {
	OperatingSystem_Null,
	OperatingSystem_Windows,
	OperatingSystem_Linux,
	OperatingSystem_Mac,
	OperatingSystem_Android,
	OperatingSystem_Ios,
	OperatingSystem_COUNT
} OperatingSystem;

typedef enum Architecture {
	Architecture_Null,
	Architecture_X64,
	Architecture_X86,
	Architecture_Arm,
	Architecture_Arm64,
	Architecture_COUNT
} Architecture;
// ---- Platform

// ---- Pointers
typedef enum TaggedPointerBit {
	TaggedPointerBit_LSB1 = 1 << 0,  // object has to be 2 byte aligned
	TaggedPointerBit_LSB2 = 1 << 1,  // object has to be 4 byte aligned
	TaggedPointerBit_LSB3 = 1 << 2,  // object has to be 8 byte aligned

	// -- Compounds
	TaggedPointerBits_LSB = TaggedPointerBit_LSB1 | TaggedPointerBit_LSB2 | TaggedPointerBit_LSB3,
	// -- Compounds
} TaggedPointerBit;
// ---- Pointers

// -- Enums/Flags


// -- Types

// ---- Data Types
typedef char      s8;
typedef short     s16;
typedef int       s32;
typedef long long s64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef s8  b8;
typedef s16 b16;
typedef s32 b32;
typedef s64 b64;

typedef float  r32;
typedef double r64;

typedef void voidf(void);
// ---- Data Types

// ---- Base Allocator Interface
typedef struct Allocator Allocator;
struct Allocator {
	void* (*allocate)(Allocator* allocator, u64 size, u64 align, b8 zero);
	void* (*reallocate)(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align, b8 zero);
	void  (*deallocate)(Allocator* allocator, void* ptr, u64 size);
};
// ---- Base Allocator Interface

// ---- Pointers

// ------ Handles
typedef union Handle Handle;
union Handle {
  u8    u8[8];
  u16   u16[4];
  u32   u32[2];
  u64   u64[1];
  void* ptr;
};
// ------ Handles

// ---- Pointers

// ---- Array

// ------ Array
typedef struct Array Array;
struct Array {
	Allocator* allocator;
	u32 len;
	u32 cap;
	u64 slot_size;
	void* data;
};
// ------ Array

// ------ Queue
typedef struct ArrayQueue ArrayQueue;
struct ArrayQueue {
	Array array;
	u32 first;
	u32 last;
};
// ------ Queue

// ---- Array

// ---- Linked Lists

// ------ Singly Linked
typedef struct Sll Sll;
struct Sll {
	Sll* next;
};
// ------ Singly Linked

// ------ Doubly Linked
typedef struct Dll Dll;
struct Dll {
	Dll* next;
	Dll* prev;
};
// ------ Doubly Linked

// ---- Linked Lists

// -- Types


// -- Functions

// ---- Symbolic Constants
static OperatingSystem operating_system_from_context(void);
static Architecture architecture_from_context(void);

static const s8* cstr8_from_operating_system(OperatingSystem os);
static const s8* cstr8_from_architecture(Architecture arch);
// ---- Symbolic Constants

// ---- Memory Operations
internalf void* mem_cpy(void* dst, void* src, u64 size);
internalf void mem_fill(void* ptr, u8 c, u64 size);
internalf void mem_zero(void* ptr, u64 size);
internalf b8 mem_match(void* ptr1, void* ptr2, u64 size);

internalf void mem_cpy_cstr8_lit(void* dest, const s8* lit, u64 size);
internalf b8 mem_match_cstr8_lit(void* ptr, const s8* lit, u64 size);


internalf void mem_swap_8(void* a, void* b, u64 n);
internalf void mem_swap_32(void* a, void* b, u64 n);
internalf void mem_swap_64(void* a, void* b, u64 n);
// ---- Memory Operations

// ---- Base Allocator
internalf Allocator allocator_create_default(void);
// ---- Base Allocator

// ---- Lower-level memory operations
internalf u64 mem_get_large_page_size(void);
internalf u64 mem_get_page_size(void);

internalf void* mem_reserve(u64 size);
internalf void mem_commit(void* ptr, u64 size);
internalf void mem_decommit(void* ptr, u64 size);
internalf void mem_release(void* ptr, u64 size);
// ---- Lower-level memory operations

// ---- Allocate
internalf void* allocate(Allocator* allocator, u64 size);
internalf void* allocate_no_zero(Allocator* allocator, u64 size);
internalf void* allocate_aligned(Allocator* allocator, u64 size, u64 align);
internalf void* allocate_aligned_no_zero(Allocator* allocator, u64 size, u64 align);
// ---- Allocate

// ---- Reallocate
internalf void* reallocate(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize);
internalf void* reallocate_no_zero(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize);
internalf void* reallocate_aligned(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align);
internalf void* reallocate_aligned_no_zero(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align);
// ---- Reallocate

// ---- Deallocate
internalf void deallocate(Allocator* allocator, void* ptr, u64 size);
// ---- Deallocate

// ---- Base Allocator
internalf void* allocate_(Allocator* allocator, u64 size, u64 align, b8 zero);
internalf void* reallocate_(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align, b8 zero);
internalf void deallocate_(Allocator* allocator, void* ptr, u64 size);
// ---- Base Allocator

// ---- Pointers

// ------ Handles
internalf Handle handle_from_ptr(void* ptr);
internalf void* ptr_from_handle(Handle handle);
internalf b8 handle_is_valid(Handle handle);
// ------ Handles

// ------ Tagged Pointers
internalf void* tagged_ptr_get(void* ptr, enum8(TaggedPointerBit) bitmask);
internalf enum8(TaggedPointerBit) tagged_ptr_get_bits(void* ptr, enum8(TaggedPointerBit) bitmask);
internalf void* tagged_ptr_set_bits(void* ptr, enum8(TaggedPointerBit) bitmask, enum8(TaggedPointerBit) bits);
internalf void* tagged_ptr_set_bits_from_ptr(void* ptr, void* src_ptr, enum8(TaggedPointerBit) bitmask);
// ------ Tagged Pointers

// ---- Pointers

// ---- Data Types

// ------ Float/Double Infinite Values
internalf r32 inf_r32(void);
internalf r64 inf_r64(void);
internalf r32 neg_inf_r32(void);
internalf r64 neg_inf_r64(void);
// ------ Float/Double Infinite Values

// ------ Sign Extend
internalf s16 sign_extend_u8(u8 b);
// ------ Sign Extend

// ------ Signed Encode/Decode
internalf u64 encode_u64_from_s64(s64 x);
internalf s64 decode_s64_from_u64(u64 x);
// ------ Signed Encode/Decode

// ---- Data Types

// ---- Array

// ------ Plain data
internalf void* array_from_member(Allocator* allocator, void* data, u32 len, u64 slot_size, u64 member_offset, u64 member_size);
// ------ Plain data

// ------ Array

// -------- Constructors/Destructors
internalf Array* array_alloc(u64 slot_size, u32 cap);
internalf Array* array_alloc_custom(Allocator* allocator, u64 slot_size, u32 cap);
internalf void array_dealloc(Array* array);
internalf void array_dealloc_custom(Array* array);
internalf Array* array_copy_subarray(Array* array, u32 start_index, u32 copy_n);
internalf Array* array_copy(Array* array);
//internalf Array* array_from_data(Allocator* allocator, void* data, u32 len, u64 slot_size)
// -------- Constructors/Destructors

internalf b8 array_grow(Array* array, u32 cap_inc);

internalf b8 array_push(Array* array, void* el, u32 i, u32 cap_inc);
internalf b8 array_push_back(Array* array, void* el, u32 cap_inc);
internalf b8 array_push_front(Array* array, void* el, u32 cap_inc);

internalf b8 array_static_push(Array* array, void* el, u32 i);
internalf b8 array_static_push_back(Array* array, void* el);
internalf b8 array_static_push_front(Array* array, void* el);

internalf void array_push_subarray(Array* dest, Array* src, u32 dest_start_index, u32 src_start_index, u32 copy_n);

internalf b8 array_pop(Array* array, u32 i, b8 ordered);
internalf b8 array_pop_back(Array* array);
internalf b8 array_pop_front(Array* array, b8 ordered);

internalf void* array_get(Array* array, u32 i);
internalf void* array_get_back(Array* array);
internalf void* array_get_front(Array* array);
internalf b8 array_try_get(Array* array, u32 i, void* out);
internalf b8 array_try_get_back(Array* array, void* out);
internalf b8 array_try_get_front(Array* array, void* out);
// ------ Array

// ------ Stack
internalf b8    array_stack_push(Array* array, void* el, u32 cap_inc);
internalf b8    array_stack_static_push(Array* array, void* el);

internalf b8    array_stack_pop(Array* array);

internalf void* array_stack_get(Array* array);
internalf b8    array_stack_try_get(Array* array, void* out);
// ------ Stack

// ------ Queue
internalf ArrayQueue* array_queue_alloc(u64 slot_size, u32 cap);
internalf ArrayQueue* array_queue_alloc_custom(Allocator* allocator, u64 slot_size, u32 cap);
internalf void array_queue_dealloc(ArrayQueue* array);
internalf void array_queue_dealloc_custom(ArrayQueue* array);

internalf b8 array_queue_grow(ArrayQueue* array, u32 cap_inc);

internalf b8 array_queue_push_front(ArrayQueue* array, void* el, u32 cap_inc);
internalf b8 array_queue_push_back(ArrayQueue* array, void* el, u32 cap_inc);
internalf b8 array_queue_static_push_front(ArrayQueue* array, void* el);
internalf b8 array_queue_static_push_back(ArrayQueue* array, void* el);

internalf b8 array_queue_pop_front(ArrayQueue* array);
internalf b8 array_queue_pop_back(ArrayQueue* array);

internalf void* array_queue_get_front(ArrayQueue* array);
internalf void* array_queue_get_back(ArrayQueue* array);
internalf b8 array_queue_try_get_front(ArrayQueue* array, void* out);
internalf b8 array_queue_try_get_back(ArrayQueue* array, void* out);
// ------ Queue

// ---- Array

// ---- Linked Lists

// ------ Singly Linked
internalf void sll_queue_init(Sll* header_first, Sll* header_last);

internalf void sll_queue_push_back(Sll* header_first, Sll* header_last, Sll* node);
internalf void sll_queue_push_front(Sll* header_first, Sll* header_last, Sll* node);

internalf Sll* sll_queue_pop_front(Sll* header_first, Sll* header_last);

internalf Sll* sll_queue_get_front(Sll* header_first, Sll* header_last);
internalf Sll* sll_queue_get_back(Sll* header_first, Sll* header_last);
internalf Sll* sll_queue_get_next(Sll* curr, Sll* header_first);
internalf Sll* sll_queue_circular_get_next(Sll* curr, Sll* header_first);

internalf void sll_stack_init(Sll* header);

internalf void sll_stack_push(Sll* header, Sll* node);

internalf Sll* sll_stack_pop(Sll* header);

internalf Sll* sll_stack_get(Sll* header);
internalf Sll* sll_stack_get_next(Sll* curr, Sll* header);
internalf Sll* sll_stack_circular_get_next(Sll* curr, Sll* header);
// ------ Singly Linked

// ------ Doubly Linked
internalf void dll_push(Dll* header, Dll* prev, Dll* node);
internalf void dll_push_front(Dll* header, Dll* node);
internalf void dll_push_back(Dll* header, Dll* node);

internalf Dll* dll_pop_front(Dll* header);
internalf Dll* dll_pop_back(Dll* header);

internalf Dll* dll_get_front(Dll* header);
internalf Dll* dll_get_back(Dll* header);

internalf void dll_remove(Dll* node);

internalf Dll* dll_get_next(Dll* curr, Dll* header);
internalf Dll* dll_get_prev(Dll* curr, Dll* header);
internalf Dll* dll_circular_get_next(Dll* curr, Dll* header);
internalf Dll* dll_circular_get_prev(Dll* curr, Dll* header);
// ------ Doubly Linked

// ---- Linked Lists

// -- Functions


#endif  // BASE_CORE_H
