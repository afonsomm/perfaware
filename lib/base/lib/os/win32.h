

#ifndef BASE_OS_WIN32_H
# define BASE_OS_WIN32_H


// -- Includes
#include "../../../externals/inc_begin.h"
#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

#include <timeapi.h>
#include <userenv.h>
#include <bcrypt.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "bcrypt.lib")

# if OS_SDL
#  pragma comment(lib, "user32.lib")
#  pragma comment(lib, "advapi32.lib")
#  pragma comment(lib, "ole32.lib")
#  pragma comment(lib, "gdi32.lib")
#  pragma comment(lib, "shell32.lib")
#  pragma comment(lib, "version.lib")
#  pragma comment(lib, "imm32.lib")
#  pragma comment(lib, "oleaut32.lib")
# endif

#include "../../../externals/inc_end.h"
// -- Includes


// -- Preprocessor Defines
// -- Preprocessor Defines


// -- Enums/Flags
typedef enum Win32ObjectType {
	Win32ObjectType_Thread,
	Win32ObjectType_Barrier,
	Win32ObjectType_Mutex,
	Win32ObjectType_ReadWriteLock,
	Win32ObjectType_ConditionalVar,
} Win32ObjectType;
// -- Enums/Flags


// -- Types

// ---- Objects
typedef struct Win32Object Win32Object;
struct Win32Object {
	Sll node;
	union {
		struct {
			HANDLE handle;
			ThreadEntryPoint_FunctionType* func;
			void* params;
			DWORD id;
		} thread;
		SYNCHRONIZATION_BARRIER barrier;
		CRITICAL_SECTION mutex;
		SRWLOCK rwlock;
		CONDITION_VARIABLE condvar;
	} object;
	enum8(Win32ObjectType) type;
};
// ---- Objects

// ---- File Iterator
typedef struct Win32FileIterator Win32FileIterator;
struct Win32FileIterator {
	HANDLE handle;
	WIN32_FIND_DATAW find_data;
	String8 base_dir;
};
// ---- File Iterator

// ---- State
typedef struct Win32State Win32State;
struct Win32State {
	Arena* permanent_arena;
	HINSTANCE hinstance;
	
	// -- Files
	String8 executable_path;
	String8 user_directory;
	String8 temp_directory;
	// -- Files
    
	// -- Memory
	u64 allocation_granularity;
	u64 mem_page_size;
	u64 mem_large_page_size;
	// -- Memory
    
	// -- Threads
	u32 logical_processor_n;
	// -- Threads
    
	// -- Time
	r64 performance_counter_period;
	// -- Time
    
	// -- Objects
	Sll free_objects;
	// -- Objects
};
// ---- State

// -- Types


// -- Functions

// ---- Memory
internalf u64 win32_mem_get_large_page_size(void);
internalf u64 win32_mem_get_page_size(void);
// ---- Memory

// ---- Threads
internalf u32 win32_get_logical_processor_n(void);
// ---- Threads

// ---- Time
internalf void win32_time_init(void);
internalf u64 win32_time_in_nanos(void);
internalf TimeDate win32_time_local_from_universal(TimeDate* date);
internalf TimeDate win32_time_universal_from_local(TimeDate* date);
internalf SYSTEMTIME win32_time_systemtime_from_date(TimeDate* date);
internalf TimeDate win32_time_date_from_systemtime(SYSTEMTIME* time);
internalf TimeDense win32_time_dense_from_filetime(FILETIME* time);
// ---- Time

// ---- Files

// ------ Names/Paths
internalf b8 win32_file_rename(String8 current, String8 newname);
internalf b8 win32_file_rename(String8 current, String8 newname);
internalf String8 win32_file_fullpath_from_relative(Allocator* allocator, String8 relative);
internalf String8 win32_file_system_get_current_directory(Allocator* allocator);
internalf String8 win32_file_system_get_executable_path(void);
internalf String8 win32_file_system_get_user_directory(void);
internalf String8 win32_file_system_get_temp_directory(Allocator* allocator);
// ------ Names/Paths

// ------ Loading
internalf FileInfo win32_file_info_load(Allocator* allocator, String8 path);
internalf File win32_file_load_from_info(Allocator* allocator, FileInfo info);
// ------ Loading

// ------ Writing
internalf b8 win32_file_write(String8 path, String8 data);
internalf b8 win32_file_write_list(String8 path, String8List* data);
internalf b8 win32_file_delete(String8 path);
internalf b8 win32_directory_create(String8 path);
internalf b8 win32_directory_delete(String8 path);
internalf b8 win32_file_write_from_handle(HANDLE fhandle, String8 data);
// ------ Writing

// ------ File Iterator
internalf FileIterator win32_file_iterator_init(Allocator* allocator, String8 dir, String8 wildcard);
internalf b8 win32_file_iterator_next(Allocator* allocator, FileIterator iterator, FileInfo* out, enum32(FileIteratorFlag) flags);
internalf void win32_file_iterator_end(FileIterator iterator);
// ------ File Iterator

// ---- Files

// ---- Objects
internalf Win32Object* win32_object_alloc(void);
internalf void win32_object_dealloc(Win32Object* object);
// ---- Objects

// ---- Entry
internalf void win32_main_thread_entry_point(int argc, WCHAR** argv);
DWORD WINAPI win32_supplement_thread_entry_point(void* object);
// ---- Entry

// -- Functions


#endif  // BASE_OS_WIN32_H
