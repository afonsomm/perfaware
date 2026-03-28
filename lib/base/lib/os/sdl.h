

#ifndef BASE_OS_SDL_H
# define BASE_OS_SDL_H


// -- Preprocessor Defines
#include "../../../externals/inc_begin.h"
#include "SDL3/SDL.h"
#include "../../../externals/inc_end.h"
// -- Preprocessor Defines


// -- Enumgs/Flags
typedef enum SDLObjectType {
	SDLObjectType_TLS,
	SDLObjectType_Thread,
	SDLObjectType_Barrier,
	SDLObjectType_ConditionVariable,
} SDLObjectType;
// -- Enumgs/Flags


// -- Types

// ---- Objects

typedef struct SDLObject SDLObject;
struct SDLObject {
	Sll node;
	union {
		struct {
			SDL_TLSID id;
		} tls;
		struct {
			SDL_Thread* handle;
			ThreadEntryPoint_FunctionType* func;
			void* params;
		} thread;
		struct {
			SDL_Mutex* mutex_handle;
			SDL_Condition* condvar_handle;
			u32 count;
			u32 max;
			u64 generation;
		} barrier;
		struct {
			SDL_Condition* handle;
			SDL_Mutex* rwlock_mutex_handle;
		} condvar;
	} object;
	enum8(SDLObjectType) type;
};
// ---- Objects

// ---- File Iterator
typedef struct SDLFileIterator SDLFileIterator;
struct SDLFileIterator {
	s8** paths;
	String8 base_dir;
	u32 len;
	u32 curr;
};

// ---- File Iterator

// ---- State
typedef struct SDLState SDLState;
struct SDLState {
	Arena* arena;
	
	// -- Files
	String8 executable_path;
	String8 user_directory;
	String8 temp_directory;
	// -- Files

	// -- Memory
	int allocation_granularity;
	int mem_page_size;
	int mem_large_page_size;
	// -- Memory

	// -- Threads
	int logical_processor_n;
	// -- Threads

	// -- Time
	r64 performance_counter_period;
	// -- Time

	// -- SIMD alignment
	size_t simd_alignment;
	// -- SIMD alignment

	// -- Objects
	Sll free_objects;
	// -- Objects
};
// ---- State

// -- Types


// -- Functions

// ---- (De)Initializers
internalf void sdl_time_init(void);
// ---- (De)Initializers

// ---- State Getters
internalf Allocator* sdl_get_permanent_allocator(void);
internalf u64 sdl_mem_get_large_page_size(void);
internalf u64 sdl_mem_get_page_size(void);
internalf u32 sdl_get_logical_processor_n(void);
internalf r64 sdl_get_performance_counter_period(void);
// ---- State Getters

// ---- Time
internalf TimeDate sdl_datetime_to_timedate(SDL_DateTime datetime);
internalf SDL_DateTime sdl_datetime_from_timedate(TimeDate* date);
// ---- Time

// ---- Objects
internalf SDLObject* sdl_object_alloc(void);
internalf void sdl_object_dealloc(SDLObject* object);
// ---- Objects

// ---- Entry
internalf void sdl_main_thread_entry_point(int argc, char* argv[]);
internalf int sdl_supplement_thread_entry_point(void* data);
// ---- Entry

// -- Functions


#endif  // BASE_OS_SDL_H
