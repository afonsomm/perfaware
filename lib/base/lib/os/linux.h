

#ifndef BASE_OS_LINUX_H
# define BASE_OS_LINUX_H


// -- Includes
#include "../../../externals/inc_begin.h"
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <limits.h>
#include <pthread.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "../../../externals/inc_end.h"
// -- Includes


// -- Preprocessor Defines
#define _FILE_OFFSET_BITS 64
// -- Preprocessor Defines


// -- Enums/Flags
typedef enum LinuxObjectType {
	LinuxObjectType_ThreadLocalStorage,
	LinuxObjectType_Thread,
	LinuxObjectType_Barrier,
	LinuxObjectType_ConditionVariable,
	LinuxObjectType_Mutex,
	LinuxObjectType_ReadWriteLock,
} LinuxObjectType;
// -- Enums/Flags


// -- Types

// ---- File Iterator
typedef struct LinuxFileIterator LinuxFileIterator;
struct LinuxFileIterator {
	DIR* dir;
  String8 base_dir;
  String8 wildcard;
};
// ---- File Iterator

// ---- Objects
typedef struct LinuxObject LinuxObject;
struct LinuxObject {
    enum8(LinuxObjectType) type;
    union {
        struct {
            pthread_key_t handle;
        } tls;
        struct {
            pthread_t handle;
            pthread_attr_t attrs;
            ThreadEntryPoint_FunctionType* func;
            void* params;
        } thread;
        struct {
            pthread_barrier_t handle;
            pthread_barrierattr_t attrs;
        } barrier;
        struct {
            pthread_cond_t handle;
            pthread_condattr_t attrs;
            pthread_mutex_t rwlock_mutex_handle;
        } condvar;
        struct {
            pthread_mutex_t handle;
            pthread_mutexattr_t attrs;
        } mutex;
        struct {
            pthread_rwlock_t handle;
            pthread_rwlockattr_t attrs;
        } rwlock;
    } object;
    Sll node;
};
// ---- Objects

typedef struct LinuxState LinuxState;
struct LinuxState {
	Arena* permanent_arena;
	
    // -- Files
    String8 executable_path;
    String8 user_directory;
    String8 temp_directory;
    // -- Files
    
    // -- Time
    u64 time_res_ns;
    // -- Time
    
    // -- Memory
    u64 allocation_granularity;
    u64 memory_page_size;
    u64 memory_large_page_size;
    // -- Memory
    
    // -- Logical Processors
    u32 logical_processor_n;
    // -- Logical Processors
    
    Sll free_objects;
};
// -- Types


// -- Functions

// ---- File System Paths
internalf String8 linux_file_system_get_current_directory(Allocator* allocator);
internalf String8 linux_file_system_get_executable_path(void);
internalf String8 linux_file_system_get_user_directory(void);
internalf String8 linux_file_system_get_temp_directory(Allocator* allocator);
// ---- File System Paths

// ---- File Iterator
// ---- File Iterator

// ---- Time
internalf void linux_time_init(void);
internalf u64 linux_time_in_nanos(void);
internalf struct tm linux_tm_from_time_date(TimeDate* date);
internalf TimeDate linux_time_date_from_tm(struct tm* broken_down_time, u16 msec);
// ---- Time

// ---- Memory
internalf u64 linux_mem_get_large_page_size(void);
internalf u64 linux_mem_get_page_size(void);
// ---- Memory

// ---- Logical Processors
internalf u32 linux_get_logical_processor_n(void);
// ---- Logical Processors

// ---- Objects
internalf LinuxObject* linux_object_alloc(void);
internalf void linux_object_dealloc(LinuxObject* object);
// ---- Objects

// ---- Entry
internalf void linux_main_thread_entry_point(int argc, char* argv[]);
internalf void* linux_supplement_thread_entry_point(void* object);
// ---- Entry

// -- Functions


#endif  // BASE_OS_LINUX_H
