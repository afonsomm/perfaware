

/**

----- Brief -----
Threading library. It also implements a thread context, a lane, and abstract thread local storage management from the OS layer.

A thread context is a set of pre-allocated arenas that can be used safely to allocate objects for a single thread.
A thread context is can be associated with a lane, in which case the thread belongs to that lane.

A lane groups threads using a synchronization barrier, and a shared memory pointer.
This allows to have multiple threads executing a single job/task concurrently, and easily synchronize them as certain points
using that barrier. This sincronization can be just to make sure, for example, for threads move on from that point with 
certain data available from the shared memory pointer, which was set by one of them.

Also there is an asyncrhonous state for a set of threads that execute in the background from the program's start, and can
perform tasks like watching a given directory for changes continuously. This state has a condition variable and a mutex, used
as a timed sleep, to block the threads so loop isn't consuming to many resources. If more ticks were requested from previous 
iterations, then the sleep is skipped. The condition variable is also used by the main thread to notify the asynchronous threads
that the program executed is to end, and they should join the main thread.
----- Brief -----

*/


#ifndef BASE_THREADS_CORE_H
# define BASE_THREADS_CORE_H


// -- Preprocessor Defines
#define THREAD_CONTEXT_SCRATCH_POOL_CAP 4
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
typedef struct Thread Thread;
struct Thread {
    Handle handle;
};

typedef struct Barrier Barrier;
struct Barrier {
    Handle handle;
};

typedef struct Mutex Mutex;
struct Mutex {
    Handle handle;
};

typedef struct ReadWriteLock ReadWriteLock;
struct ReadWriteLock {
    Handle handle;
};

typedef struct ConditionVariable ConditionVariable;
struct ConditionVariable {
    Handle handle;
};

typedef struct ThreadLocalStorage ThreadLocalStorage;
struct ThreadLocalStorage {
    Handle handle;
};

typedef void ThreadEntryPoint_FunctionType(void* params);

typedef struct LaneContext LaneContext;
struct LaneContext {
	u32 thread_index;
	u32 thread_n;
	Barrier barrier;
	u64* shared_memory;
};

typedef struct ThreadContext ThreadContext;
struct ThreadContext {
	Arena* scratch_pool[THREAD_CONTEXT_SCRATCH_POOL_CAP];
	LaneContext lane_context;
};

typedef struct AsyncState AsyncState;
struct AsyncState {
	Thread* threads;
	
	// -- Lane Info
	Barrier barrier;
	u64 shared_memory;
	u32 thread_n;
	// -- Lane Info
    
	// -- Ticks
	u32 requested_tick_n;
	// -- Ticks
    
	// -- Locks
	ConditionVariable condvar;
	Mutex mutex;
	// -- Locks
    
	u32 exit;
};
// -- Types


// -- Functions
internalf void base_threads_init(u32 async_thread_n);
internalf void base_threads_deinit(void);

// ---- Logical Processors
internalf u32 thread_get_logical_processor_n(void);
// ---- Logical Processors

// ---- Thread Local Storage
internalf ThreadLocalStorage thread_local_storage_alloc(void);
internalf void* thread_local_storage_get(ThreadLocalStorage tls);
internalf void thread_local_storage_set(ThreadLocalStorage tls, void* data);
internalf void thread_local_storage_dealloc(ThreadLocalStorage tls);
// ---- Thread Local Storage

// ---- Thread Management
internalf Thread thread_launch(ThreadEntryPoint_FunctionType* entry_point, void* params, const s8* name_fmt, ...);
internalf b8 thread_join(Thread handle);
internalf void thread_detach(Thread handle);
// ---- Thread Management

// ---- Barrier Management
internalf Barrier barrier_alloc(u32 thread_max_n);
internalf void barrier_wait(Barrier handle);
internalf void barrier_dealloc(Barrier handle);
// ---- Barrier Management

// ---- Mutex Management
internalf Mutex mutex_alloc(void);
internalf void mutex_lock(Mutex handle);
internalf void mutex_unlock(Mutex handle);
internalf void mutex_dealloc(Mutex handle);
// ---- Mutex Management

// ---- Read-Write Lock Management
internalf ReadWriteLock rwlock_alloc(void);
internalf void rwlock_lock(ReadWriteLock handle, b8 write_mode);
internalf void rwlock_unlock(ReadWriteLock handle, b8 write_mode);
internalf void rwlock_dealloc(ReadWriteLock handle);
// ---- Read-Write Lock Management

// ---- Condition Variable Management
internalf ConditionVariable condvar_alloc(void);
internalf b8 condvar_signal(ConditionVariable handle);
internalf b8 condvar_broadcast(ConditionVariable handle);
internalf b8 condvar_wait_mutex(ConditionVariable condvar_handle, Mutex mutex_handle);
internalf b8 condvar_wait_rwlock(ConditionVariable condvar_handle, ReadWriteLock rwlock_handle, b8 write_mode);
internalf b8 condvar_timed_wait_mutex(ConditionVariable condvar_handle, Mutex mutex_handle, u64 duration_ns);
internalf b8 condvar_timed_wait_rwlock(ConditionVariable condvar_handle, ReadWriteLock rwlock_handle, u64 duration_ns, b8 write_mode);
internalf void condvar_dealloc(ConditionVariable handle);
// ---- Condition Variable Management

// ---- Lanes
internalf u32 lane_get_thread_index(LaneContext* lane_context);
internalf u32 lane_get_thread_n(LaneContext* lane_context);
internalf void lane_barrier_wait_on_data(LaneContext* lane_context, void* ptr, u64 size, u64 src_thread_index);
internalf void lane_barrier_wait(LaneContext* lane_context);

internalf u32 base_lane_get_thread_index(void);
internalf u32 base_lane_get_thread_n(void);
internalf void base_lane_barrier_wait_on_data(void* ptr, u64 size, u64 src_thread_index);
internalf void base_lane_barrier_wait(void);
// ---- Lanes

// ---- Base Thread Context
internalf void base_thread_context_set_lane(LaneContext lane_context);
internalf void base_thread_context_alloc(void);
internalf void base_thread_context_dealloc(void);
internalf Arena* base_thread_context_get_scratch_arena(Arena** conflict_array, u32 count);

internalf ArenaTemp scratch_begin(Arena** conflict_arenas, u32 count);
internalf void scratch_end(ArenaTemp* arena);
// ---- Base Thread Context

// -- Functions


#endif  // BASE_THREADS_CORE_H
