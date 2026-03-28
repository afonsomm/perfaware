

// -- Global State
global ThreadLocalStorage base_thread_context_tls;
global AsyncState async_state;
// -- Global State


// -- Functions
internalf void base_threads_init(u32 async_thread_n) {
    
	ArenaTemp scratch;
    
	// -- Setup main thread thread context
	base_thread_context_tls = thread_local_storage_alloc();
	base_thread_context_alloc();
	// -- Setup main thread thread context
    
	
	scratch = scratch_begin(0, 0);
    
	// -- Setup async threads with thread contexts and lane
	{
		u32 async_thread_max_n;
		
		// -- Get number of async threads
		async_thread_max_n = thread_get_logical_processor_n() - 1;  // 1 for main thread
		async_thread_n = min(async_thread_max_n, async_thread_n);
		// -- Get number of async threads
        
		
		if(async_thread_n) {
			u32 async_thread_i;
			LaneContext* lane_contexts;
            
			// -- Alloc/Fill async state
			async_state.threads = (Thread*)allocate(&scratch.arena->allocator, sizeof(Thread) * async_thread_n);
			
			async_state.thread_n = async_thread_n;
			async_state.barrier = barrier_alloc(async_thread_n);
			async_state.shared_memory = 0;
            
			async_state.requested_tick_n = 0;
            
			async_state.condvar = condvar_alloc();
			async_state.mutex = mutex_alloc();
			// -- Alloc/Fill async state
            
			
			// -- Fill lane context and launch async threads
			lane_contexts = (LaneContext*)allocate(&scratch.arena->allocator, sizeof(LaneContext) * async_thread_n);
			for(async_thread_i = 0; async_thread_i < async_thread_n; async_thread_i += 1) {
				lane_contexts[async_thread_i].thread_n = async_state.thread_n;
				lane_contexts[async_thread_i].barrier = async_state.barrier;
				lane_contexts[async_thread_i].shared_memory = &async_state.shared_memory;
				lane_contexts[async_thread_i].thread_index = async_thread_i;
				async_state.threads[async_thread_i] = thread_launch(base_async_thread_entry_point, &lane_contexts[async_thread_i], "Async_%d", async_thread_i);
			}
			// -- Fill lane context and launch async threads
		}
	}
	// -- Setup async threads with thread contexts and lane
}

internalf void base_threads_deinit(void) {
    
	u32 async_thread_n;
	u32 async_thread_i;
	
	if(async_state.thread_n) {
		// -- Wake up sleeping threads
		atomic_u32_eval_assign(&async_state.exit, 1);
		condvar_broadcast(async_state.condvar);
		// -- Wake up sleeping threads
        
		// -- Join async threads
		async_thread_n = async_state.thread_n;
		for(async_thread_i = 0; async_thread_i < async_thread_n; async_thread_i += 1) {
			thread_join(async_state.threads[async_thread_i]);
		}
		barrier_dealloc(async_state.barrier);
		condvar_dealloc(async_state.condvar);
		mutex_dealloc(async_state.mutex);
		// -- Join async threads
	}
	
	base_thread_context_dealloc();  // scratch is cleared automatically
	thread_local_storage_dealloc(base_thread_context_tls);
}

// ---- Logical Processors
internalf u32 thread_get_logical_processor_n(void) {
    
	u32 result;
    
	#if OS_SDL
	result = sdl_get_logical_processor_n();
	#elif OS_WINDOWS
	result = win32_get_logical_processor_n();
	#elif OS_LINUX
	result = linux_get_logical_processor_n();
	#else
	#error "thread_get_logical_processor_n: not implemented for this operating system."
	#endif  // OS
    
	return result;
}
// ---- Logical Processors

// ---- Thread Local Storage
internalf ThreadLocalStorage thread_local_storage_alloc(void) {
	
	ThreadLocalStorage result;
    
	#if OS_SDL
	SDLObject* sdl_object = sdl_object_alloc();
	result.handle = handle_from_ptr(sdl_object);
	#elif OS_WINDOWS
	result.handle.u32[0] = TlsAlloc();
	#elif OS_LINUX
	LinuxObject* linux_object = linux_object_alloc();
	if(!pthread_key_create(&linux_object->object.tls.handle, (void( *)(void*))0)) {
		linux_object->type = LinuxObjectType_ThreadLocalStorage;
		result.handle = handle_from_ptr(linux_object);
	} else {
		linux_object_dealloc(linux_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#else
	#error "thread_local_storage_alloc: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf void* thread_local_storage_get(ThreadLocalStorage tls) {
	
	void* result;
	
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(tls.handle);
	result = SDL_GetTLS(&sdl_object->object.tls.id);
	if(!result) {
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = TlsGetValue(tls.handle.u32[0]);
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(tls.handle);
	result = pthread_getspecific(linux_object->object.tls.handle);
	#else
	#error "thread_local_storage_get: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf void thread_local_storage_set(ThreadLocalStorage tls, void* data) {
	
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(tls.handle);
	if(SDL_SetTLS(&sdl_object->object.tls.id, data, (SDL_TLSDestructorCallback)0)) {
	} else {
		debug_print_cstr8_lit(SDL_GetError());
		raise();
	}
	#elif OS_WINDOWS
	if(TlsSetValue(tls.handle.u32[0], data)) {
	} else {
		raise();
	}
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(tls.handle);
	if(!pthread_setspecific(linux_object->object.tls.handle, data)) {
	} else {
		raise();
	}
	#else
	#error "thread_local_storage_set: not implemented for this operating system."
	#endif  // OS
}

internalf void thread_local_storage_dealloc(ThreadLocalStorage tls) {
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(tls.handle);
	sdl_object_dealloc(sdl_object);
	// SDL_CleanupTLS() is called at the end of sdl_supplement_thread_entry_point, since SDL has a single function to clean all TLS data for the thread.
	// NOTE: not called in thread_join, because the thread may be detached, so it just ends without joining.
	#elif OS_WINDOWS
	if(TlsFree(tls.handle.u32[0])) {
	} else {
		raise();
	}
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(tls.handle);
	if(!pthread_key_delete(linux_object->object.tls.handle)) {
	} else {
		raise();
	}
	linux_object_dealloc(linux_object);
	#else
	#error "thread_local_storage_dealloc: not implemented for this operating system."
	#endif  // OS
}
// ---- Thread Local Storage

// ---- Thread Management
internalf Thread thread_launch(ThreadEntryPoint_FunctionType* entry_point, void* params, const s8* name_fmt, ...) {
	// Create and lauch a thread with the given entry point and its parameters, which will be executed after the OS and Base supplement thread entry point in this order.
	// The returned handle is null if the thread could not be created.
    
	Thread result;
  
	#if OS_SDL
	SDLObject* sdl_object = sdl_object_alloc();
	ArenaTemp scratch = scratch_begin(0, 0);
	String8 name;

	if(name_fmt) {
		va_list vargs;
		va_start(vargs, name_fmt);
		name = str8fv(&scratch.arena->allocator, name_fmt, vargs);
		va_end(vargs);
	} else {
		name = str8_nil();
	}
	
	sdl_object->object.thread.func = entry_point;
	sdl_object->object.thread.params = params;
	sdl_object->object.thread.handle = SDL_CreateThread(sdl_supplement_thread_entry_point, (s8*)name.str, sdl_object);
	if(sdl_object->object.thread.handle) {
		result.handle = handle_from_ptr(sdl_object);
	} else {
		sdl_object_dealloc(sdl_object);
		debug_print_cstr8_lit(SDL_GetError());
		raise();
		result.handle = handle_from_ptr((void*)0);
	}
	scratch_end(&scratch);
	#elif OS_WINDOWS
	Win32Object* win32_object = win32_object_alloc();
	win32_object->object.thread.func = entry_point;
	win32_object->object.thread.params = params;
	win32_object->object.thread.handle = CreateThread((SECURITY_ATTRIBUTES*)0, 0, win32_supplement_thread_entry_point, win32_object, CREATE_SUSPENDED, &win32_object->object.thread.id);
	if(win32_object->object.thread.handle) {
		
		if(name_fmt) {
			ArenaTemp scratch = scratch_begin(0, 0);
			String8 name;
			String16 name16;
			va_list vargs;
		
			va_start(vargs, name_fmt);
			name = str8fv(&scratch.arena->allocator, name_fmt, vargs);
			name16 = str16_from_8(&scratch.arena->allocator, name);
			SetThreadDescription(win32_object->object.thread.handle, (LPCWSTR)name16.str);
			va_end(vargs);
			
			scratch_end(&scratch);	
		}
		
		ResumeThread(win32_object->object.thread.handle);
		win32_object->type = Win32ObjectType_Thread;
		result.handle = handle_from_ptr(win32_object);
	} else {
		win32_object_dealloc(win32_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#elif OS_LINUX
	LinuxObject* linux_object = linux_object_alloc();
	linux_object->object.thread.func = entry_point;
	linux_object->object.thread.params = params;
	linux_object->object.thread.name = name;
	if(!pthread_create(&linux_object->object.thread.handle, (pthread_attr_t *)0, linux_supplement_thread_entry_point, linux_object)) {
		
		if(name_fmt) {
			ArenaTemp scratch = scratch_begin(0, 0);
			String8 name;
			va_list vargs;

			va_start(vargs, name_fmt);
			name = str8fv(&scratch.arena->allocator, name_fmt, vargs);
			va_end(vargs);

			pthread_setname_np(linux_object->object.thread.handle, (s8*)name.str);

			scratch_end(&scratch);
		}
		
		linux_object->type = LinuxObjectType_Thread;
		result.handle = handle_from_ptr(linux_object);
	} else {
		linux_object_dealloc(linux_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#else
	#error "thread_launch: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf b8 thread_join(Thread handle) {
    
	b8 result;
    
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(handle.handle);
	int sdl_status;
	SDL_WaitThread(sdl_object->object.thread.handle, &sdl_status);
	if(sdl_status != -1) {
		result = 1;
	} else {
		raise();
		result = 0;
	}
	#elif OS_WINDOWS
	Win32Object* thread_object = (Win32Object*)ptr_from_handle(handle.handle);
	DWORD wait_result = WaitForSingleObject(thread_object->object.thread.handle, INFINITE);
	if(wait_result == WAIT_OBJECT_0) {
		CloseHandle(thread_object->object.thread.handle);
		result = 1;
	} else {
		raise();
		result = 0;
	}
	win32_object_dealloc(thread_object);
	#elif OS_LINUX
	LinuxObject* thread_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_join(thread_object->object.thread.handle, (void* *)0)) {
		result = 1;
	} else {
		raise();
		result = 0;
	}
	linux_object_dealloc(thread_object);
	#else
	#error "thread_join: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf void thread_detach(Thread handle) {
	
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(handle.handle);
	SDL_DetachThread(sdl_object->object.thread.handle);
	#elif OS_WINDOWS
	Win32Object* thread_object = (Win32Object*)ptr_from_handle(handle.handle);
	if(CloseHandle(thread_object->object.thread.handle)) {
	} else {
		raise();
	}
	win32_object_dealloc(thread_object);
	#elif OS_LINUX
	LinuxObject* thread_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_detach(thread_object->object.thread.handle)) {
	} else {
		raise();
	}
	linux_object_dealloc(thread_object);
	#else
	#error "thread_detach: not implemented for this operating system."
	#endif  // OS
}
// ---- Thread Management

// ---- Barrier Management
internalf Barrier barrier_alloc(u32 thread_max_n) {
    
	Barrier result;
    
	#if OS_SDL
	SDLObject* sdl_object = sdl_object_alloc();
	sdl_object->object.barrier.mutex_handle = SDL_CreateMutex();
	sdl_object->object.barrier.condvar_handle = SDL_CreateCondition();
	if(sdl_object->object.barrier.mutex_handle && sdl_object->object.barrier.condvar_handle) {
		sdl_object->object.barrier.max = thread_max_n;
		sdl_object->object.barrier.count = 0;
		sdl_object->object.barrier.generation = 0;
		result.handle = handle_from_ptr(sdl_object);
	} else {
		sdl_object_dealloc(sdl_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#elif OS_WINDOWS
	Win32Object* win32_object = win32_object_alloc();
	if(InitializeSynchronizationBarrier(&win32_object->object.barrier, (LONG)thread_max_n, -1)) {
		win32_object->type = Win32ObjectType_Barrier;
		result.handle = handle_from_ptr(win32_object);
	} else {
		win32_object_dealloc(win32_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#elif OS_LINUX
	LinuxObject* linux_object = linux_object_alloc();
	if(!pthread_barrier_init(&linux_object->object.barrier.handle, (pthread_barrierattr_t *)0, thread_max_n)) {
		linux_object->type = LinuxObjectType_Barrier;
		result.handle = handle_from_ptr(linux_object);
	} else {
		linux_object_dealloc(linux_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#else
	#error "barrier_alloc: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf void barrier_wait(Barrier handle) {

	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(handle.handle);
	u64 generation;

	SDL_LockMutex(sdl_object->object.barrier.mutex_handle);

	generation = sdl_object->object.barrier.generation;
	sdl_object->object.barrier.count += 1;

	if(sdl_object->object.barrier.count == sdl_object->object.barrier.max) {
		sdl_object->object.barrier.count = 0;
		sdl_object->object.barrier.generation += 1;
		SDL_BroadcastCondition(sdl_object->object.barrier.condvar_handle);
	} else {
		while(generation == sdl_object->object.barrier.generation) {  // TODO: understand this loop and the generation variable better
			SDL_WaitCondition(sdl_object->object.barrier.condvar_handle, sdl_object->object.barrier.mutex_handle);
		}
	}

	SDL_UnlockMutex(sdl_object->object.barrier.mutex_handle);
	
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	EnterSynchronizationBarrier(&win32_object->object.barrier, 0);
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	pthread_barrier_wait(&linux_object->object.barrier.handle);
	#else
	#error "barrier_wait: not implemented for this operating system."
	#endif  // OS
}

internalf void barrier_dealloc(Barrier handle) {

	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(handle.handle);
	SDL_DestroyMutex(sdl_object->object.barrier.mutex_handle);
	SDL_DestroyCondition(sdl_object->object.barrier.condvar_handle);
	sdl_object_dealloc(sdl_object);
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	if(DeleteSynchronizationBarrier(&win32_object->object.barrier)) {
	} else {
		raise();
	}
	win32_object_dealloc(win32_object);
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_barrier_destroy(&linux_object->object.barrier.handle)) {
	} else {
		raise();
	}
	linux_object_dealloc(linux_object);
	#else
	#error "barrier_dealloc: not implemented for this operating system."
	#endif  // OS
}
// ---- Barrier Management

// ---- Mutex Management
internalf Mutex mutex_alloc(void) {
    
	Mutex result;
  
	#if OS_SDL
	SDL_Mutex* sdl_mutex = SDL_CreateMutex();
	if(sdl_mutex) {
		result.handle = handle_from_ptr(sdl_mutex);
	} else {
		result.handle = handle_from_ptr((void*)0);
		debug_print_cstr8_lit(SDL_GetError());
		raise();
	}
	#elif OS_WINDOWS
	Win32Object* win32_object = win32_object_alloc();
	InitializeCriticalSection(&win32_object->object.mutex);
	win32_object->type = Win32ObjectType_Mutex;
	result.handle = handle_from_ptr(win32_object);
	#elif OS_LINUX
	LinuxObject* linux_object = linux_object_alloc();
	if(!pthread_mutex_init(&linux_object->object.mutex.handle, (pthread_mutexattr_t *)0)) {
		linux_object->type = LinuxObjectType_Mutex;
		result.handle = handle_from_ptr(linux_object);
	} else {
		linux_object_dealloc(linux_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#else
	#error "mutex_alloc: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf void mutex_lock(Mutex handle) {
	
	#if OS_SDL
	SDL_LockMutex((SDL_Mutex*)ptr_from_handle(handle.handle));
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	EnterCriticalSection(&win32_object->object.mutex);
	#elif OS_LINUX
	LinuxObject* linux_obect = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_mutex_lock(&linux_obect->object.mutex.handle)) {
	} else {
		raise();
	}
	#else
	#error "mutex_lock: not implemented for this operating system."
	#endif  // OS
}

internalf void mutex_unlock(Mutex handle) {

	#if OS_SDL
	SDL_UnlockMutex((SDL_Mutex*)ptr_from_handle(handle.handle));
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	LeaveCriticalSection(&win32_object->object.mutex);
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_mutex_unlock(&linux_object->object.mutex.handle)) {
	} else {
		raise();
	}
	#else
	#error "mutex_unlock: not implemented for this operating system."
	#endif  // OS
}

internalf void mutex_dealloc(Mutex handle) {
	
	#if OS_SDL
	SDL_DestroyMutex((SDL_Mutex*)ptr_from_handle(handle.handle));
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	DeleteCriticalSection(&win32_object->object.mutex);
	win32_object_dealloc(win32_object);
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_mutex_destroy(&linux_object->object.mutex.handle)) {
	} else {
		raise();
	}
	linux_object_dealloc(linux_object);
	#else
	#error "mutex_dealloc: not implemented for this operating system."
	#endif  // OS
}
// ---- Mutex Management

// ---- Read-Write Lock Management
internalf ReadWriteLock rwlock_alloc(void) {
    
	ReadWriteLock result;
   
	#if OS_SDL
	SDL_RWLock* sdl_rwlock = SDL_CreateRWLock();
	if(sdl_rwlock) {
		result.handle = handle_from_ptr(sdl_rwlock);
	} else {
		result.handle = handle_from_ptr((void*)0);
		debug_print_cstr8_lit(SDL_GetError());
		raise();
	}
	#elif OS_WINDOWS
	Win32Object* win32_object = win32_object_alloc();
	InitializeSRWLock(&win32_object->object.rwlock);
	win32_object->type = Win32ObjectType_ReadWriteLock;
	result.handle = handle_from_ptr(win32_object);
	#elif OS_LINUX
	LinuxObject* linux_object = linux_object_alloc();
	if(!pthread_rwlock_init(&linux_object->object.rwlock.handle, (pthread_rwlockattr_t *)0)) {
		linux_object->type = LinuxObjectType_ReadWriteLock;
		result.handle = handle_from_ptr(linux_object);
	} else {
		linux_object_dealloc(linux_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#else
	#error "rwlock_alloc: not implemented for this operating system."
	#endif  // OS

	return result;
}

internalf void rwlock_lock(ReadWriteLock handle, b8 write_mode) {
    
	#if OS_SDL
	if(write_mode) {
		SDL_LockRWLockForWriting((SDL_RWLock*)ptr_from_handle(handle.handle));
	} else {
		SDL_LockRWLockForReading((SDL_RWLock*)ptr_from_handle(handle.handle));
	}
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	if(write_mode) {
		AcquireSRWLockExclusive(&win32_object->object.rwlock);
	} else {
		AcquireSRWLockShared(&win32_object->object.rwlock);
	}
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(write_mode) {
		if(!pthread_rwlock_wrlock(&linux_object->object.rwlock.handle)) {
		} else {
			raise();
		}
	} else {
		if(!pthread_rwlock_rdlock(&linux_object->object.rwlock.handle)) {
		} else {
			raise();
		}
	}
	#else
	#error "rwlock_lock: not implemented for this operating system."
	#endif  // OS
}

internalf void rwlock_unlock(ReadWriteLock handle, b8 write_mode) {
	
	#if OS_SDL
	SDL_UnlockRWLock((SDL_RWLock*)ptr_from_handle(handle.handle));
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	if(write_mode) {
		ReleaseSRWLockExclusive(&win32_object->object.rwlock);
	} else {
		ReleaseSRWLockShared(&win32_object->object.rwlock);
	}
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_rwlock_unlock(&linux_object->object.rwlock.handle)) {
	} else {
		raise();
	}
	#else
	#error "rwlock_unlock: not implemented for this operating system."
	#endif  // OS
}

internalf void rwlock_dealloc(ReadWriteLock handle) {
    
	#if OS_SDL
	SDL_DestroyRWLock((SDL_RWLock*)ptr_from_handle(handle.handle));
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	win32_object_dealloc(win32_object);
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_rwlock_destroy(&linux_object->object.rwlock.handle)) {
	} else {
		raise();
	}
	linux_object_dealloc(linux_object);
	#else
	#error "rwlock_dealloc: not implemented for this operating system."
	#endif  // OS
}
// ---- Read-Write Lock Management

// ---- Condition Variable Management
internalf ConditionVariable condvar_alloc(void) {
    
	ConditionVariable result;
  
	#if OS_SDL
	SDLObject* sdl_object = sdl_object_alloc();
	sdl_object->object.condvar.handle = SDL_CreateCondition();
	sdl_object->object.condvar.rwlock_mutex_handle = SDL_CreateMutex();
	if(sdl_object->object.condvar.handle && sdl_object->object.condvar.rwlock_mutex_handle) {
		result.handle = handle_from_ptr(sdl_object);
	} else {
		sdl_object_dealloc(sdl_object);
		result.handle = handle_from_ptr((void*)0);
		debug_print_cstr8_lit(SDL_GetError());
		raise();
	}
	#elif OS_WINDOWS
	Win32Object* win32_object = win32_object_alloc();
	InitializeConditionVariable(&win32_object->object.condvar);
	win32_object->type = Win32ObjectType_ConditionalVar;
	result.handle = handle_from_ptr(win32_object);
	#elif OS_LINUX
	LinuxObject* linux_object = linux_object_alloc();
	if(!pthread_cond_init(&linux_object->object.condvar.handle, (pthread_condattr_t *)0)
		&& !pthread_mutex_init(&linux_object->object.condvar.rwlock_mutex_handle, (pthread_mutexattr_t *)0)) {
		linux_object->type = LinuxObjectType_ConditionVariable;
		result.handle = handle_from_ptr(linux_object);
	} else {
		linux_object_dealloc(linux_object);
		result.handle = handle_from_ptr((void*)0);
		raise();
	}
	#else
	#error "condvar_alloc: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf b8 condvar_signal(ConditionVariable handle) {
    
	b8 result;
    
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(handle.handle);
	SDL_SignalCondition(sdl_object->object.condvar.handle);
	result = 1;
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	WakeConditionVariable(&win32_object->object.condvar);
	result = 1;
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	pthread_cond_signal(&linux_object->object.condvar.handle);
	result = 1;
	#else
	#error "condvar_signal: not implemented for this operating system."
	#endif  // OS
	
	return result;
}

internalf b8 condvar_broadcast(ConditionVariable handle) {
	
	b8 result;
    
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(handle.handle);
	SDL_BroadcastCondition(sdl_object->object.condvar.handle);
	result = 1;
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	WakeAllConditionVariable(&win32_object->object.condvar);
	result = 1;
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	pthread_cond_broadcast(&linux_object->object.condvar.handle);
	result = 1;
	#else
	#error "condvar_broadcast: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf b8 condvar_wait_mutex(ConditionVariable condvar_handle, Mutex mutex_handle) {
    
	b8 result;
    
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(condvar_handle.handle);
	SDL_WaitCondition(sdl_object->object.condvar.handle, (SDL_Mutex *)ptr_from_handle(mutex_handle.handle));
	result = 1;
	#elif OS_WINDOWS
	Win32Object* win32_condvar_object = (Win32Object*)ptr_from_handle(condvar_handle.handle);
	Win32Object* win32_mutex_object = (Win32Object*)ptr_from_handle(mutex_handle.handle);
	SleepConditionVariableCS(&win32_condvar_object->object.condvar, &win32_mutex_object->object.mutex, INFINITE);
	result = 1;
	#elif OS_LINUX
	LinuxObject* linux_condvar_object = (LinuxObject*)ptr_from_handle(condvar_handle.handle);
	LinuxObject* linux_mutex_object = (LinuxObject*)ptr_from_handle(mutex_handle.handle);
	pthread_cond_wait(&linux_condvar_object->object.condvar.handle, &linux_mutex_object->object.mutex.handle);
	result = 1;
	#else
	#error "condvar_wait_mutex: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf b8 condvar_wait_rwlock(ConditionVariable condvar_handle, ReadWriteLock rwlock_handle, b8 write_mode) {
    
	b8 result;
	
	#if OS_SDL
	// TODO: need to understand this better
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(condvar_handle.handle);
	SDL_Condition* sdl_condvar = sdl_object->object.condvar.handle;
	SDL_Mutex* sdl_mutex = sdl_object->object.condvar.rwlock_mutex_handle;
	SDL_RWLock* sdl_rwlock = (SDL_RWLock*)ptr_from_handle(rwlock_handle.handle);

	SDL_LockMutex(sdl_mutex);
	SDL_UnlockRWLock(sdl_rwlock);

	SDL_WaitCondition(sdl_condvar, sdl_mutex);

	if(write_mode) {
		SDL_LockRWLockForWriting(sdl_rwlock);
	} else {
		SDL_LockRWLockForReading(sdl_rwlock);
	}
	SDL_UnlockMutex(sdl_mutex);
	result = 1;
	#elif OS_WINDOWS
	Win32Object* win32_condvar_object = (Win32Object*)ptr_from_handle(condvar_handle.handle);
	Win32Object* win32_rwlock_object = (Win32Object*)ptr_from_handle(rwlock_handle.handle);
	SleepConditionVariableSRW(&win32_condvar_object->object.condvar, &win32_rwlock_object->object.rwlock, INFINITE, (!write_mode) * CONDITION_VARIABLE_LOCKMODE_SHARED);
	result = 1;
	#elif OS_LINUX
	LinuxObject* linux_condvar_object = (LinuxObject*)ptr_from_handle(condvar_handle.handle);
	LinuxObject* linux_rwlock_object = (LinuxObject*)ptr_from_handle(rwlock_handle.handle);
    
	pthread_mutex_lock(&linux_condvar_object->object.condvar.rwlock_mutex_handle);
	pthread_rwlock_unlock(&linux_rwlock_object->object.rwlock.handle);
	
	pthread_cond_wait(&linux_condvar_object->object.condvar.handle, &linux_condvar_object->object.condvar.rwlock_mutex_handle);
	
	if(write_mode) {
		pthread_rwlock_wrlock(&linux_rwlock_object->object.rwlock.handle);
	} else {
		pthread_rwlock_rdlock(&linux_rwlock_object->object.rwlock.handle);
	}
    
	pthread_mutex_unlock(&linux_condvar_object->object.condvar.rwlock_mutex_handle);
	result = 1;
	#else
	#error "condvar_wait_rwlock: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf b8 condvar_timed_wait_mutex(ConditionVariable condvar_handle, Mutex mutex_handle, u64 duration_ns) {
    
	b8 result;
    
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(condvar_handle.handle);
	result = (b8)SDL_WaitConditionTimeout(sdl_object->object.condvar.handle, (SDL_Mutex*)ptr_from_handle(mutex_handle.handle), (Sint32)(duration_ns / 1000000));
	#elif OS_WINDOWS
	Win32Object* win32_condvar_object = (Win32Object*)ptr_from_handle(condvar_handle.handle);
	Win32Object* win32_mutex_object = (Win32Object*)ptr_from_handle(mutex_handle.handle);
	SleepConditionVariableCS(&win32_condvar_object->object.condvar, &win32_mutex_object->object.mutex, (DWORD)(duration_ns / 1000));
	result = (b8)(GetLastError() != ERROR_TIMEOUT);
	#elif OS_LINUX
	LinuxObject* linux_condvar_object = (LinuxObject*)ptr_from_handle(condvar_handle.handle);
	LinuxObject* linux_mutex_object = (LinuxObject*)ptr_from_handle(mutex_handle.handle);
	struct timespec abstime;
	s32 wait_result;
    
	clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += (duration_ns / 1000000000);
	abstime.tv_nsec += (duration_ns % 1000000000);
	wait_result = pthread_cond_timedwait(&linux_condvar_object->object.condvar.handle, &linux_mutex_object->object.mutex.handle, &abstime);
	result = (b8)(wait_result != ETIMEDOUT);
	#else
	#error "condvar_timed_wait_mutex: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf b8 condvar_timed_wait_rwlock(ConditionVariable condvar_handle, ReadWriteLock rwlock_handle, u64 duration_ns, b8 write_mode) {
    
	b8 result;
    
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(condvar_handle.handle);
	SDL_Condition* sdl_condvar = sdl_object->object.condvar.handle;
	SDL_Mutex* sdl_mutex = sdl_object->object.condvar.rwlock_mutex_handle;
	SDL_RWLock* sdl_rwlock = (SDL_RWLock*)ptr_from_handle(rwlock_handle.handle);

	SDL_LockMutex(sdl_mutex);
	SDL_UnlockRWLock(sdl_rwlock);

	result = (b8)SDL_WaitConditionTimeout(sdl_condvar, sdl_mutex, (Sint32)(duration_ns / 1000000));

	if(write_mode) {
		SDL_LockRWLockForWriting(sdl_rwlock);
	} else {
		SDL_LockRWLockForReading(sdl_rwlock);
	}
	SDL_UnlockMutex(sdl_mutex);
	#elif OS_WINDOWS
	Win32Object* win32_condvar_object = (Win32Object*)ptr_from_handle(condvar_handle.handle);
	Win32Object* win32_rwlock_object = (Win32Object*)ptr_from_handle(rwlock_handle.handle);
	SleepConditionVariableSRW(&win32_condvar_object->object.condvar, &win32_rwlock_object->object.rwlock, (DWORD)(duration_ns / 1000000), (!write_mode) * CONDITION_VARIABLE_LOCKMODE_SHARED);
	result = (b8)(GetLastError() != ERROR_TIMEOUT);
	#elif OS_LINUX
	LinuxObject* linux_condvar_object = (LinuxObject*)ptr_from_handle(condvar_handle.handle);
	LinuxObject* linux_rwlock_object = (LinuxObject*)ptr_from_handle(rwlock_handle.handle);
	struct timespec abstime;
	s32 wait_result;
    
	clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += (duration_ns / 1000000000);
	abstime.tv_nsec += (duration_ns % 1000000000);
    
	pthread_mutex_lock(&linux_condvar_object->object.condvar.rwlock_mutex_handle);
	pthread_rwlock_unlock(&linux_rwlock_object->object.rwlock.handle);
    
	wait_result = pthread_cond_timedwait(&linux_condvar_object->object.condvar.handle, &linux_condvar_object->object.condvar.rwlock_mutex_handle, &abstime);
	result = (b8)(wait_result != ETIMEDOUT);
	  
	if(write_mode) {
		pthread_rwlock_wrlock(&linux_rwlock_object->object.rwlock.handle);
	} else {
		pthread_rwlock_rdlock(&linux_rwlock_object->object.rwlock.handle);
	}
    
	pthread_mutex_unlock(&linux_condvar_object->object.condvar.rwlock_mutex_handle);
	#else
	#error "condvar_timed_wait_rwlock: not implemented for this operating system."
	#endif  // OS
    
	return result;
}

internalf void condvar_dealloc(ConditionVariable handle) {
	
	#if OS_SDL
	SDLObject* sdl_object = (SDLObject*)ptr_from_handle(handle.handle);
	SDL_DestroyCondition(sdl_object->object.condvar.handle);
	SDL_DestroyMutex(sdl_object->object.condvar.rwlock_mutex_handle);
	sdl_object_dealloc(sdl_object);
	#elif OS_WINDOWS
	Win32Object* win32_object = (Win32Object*)ptr_from_handle(handle.handle);
	win32_object_dealloc(win32_object);
	#elif OS_LINUX
	LinuxObject* linux_object = (LinuxObject*)ptr_from_handle(handle.handle);
	if(!pthread_cond_destroy(&linux_object->object.condvar.handle)
		&& !pthread_mutex_destroy(&linux_object->object.condvar.rwlock_mutex_handle)) {
	} else {
		raise();
	}
	linux_object_dealloc(linux_object);
	#else
	#error "condvar_dealloc: not implemented for this operating system."
	#endif  // OS
}
// ---- Condition Variable Management

// ---- Lanes
internalf u32 lane_get_thread_index(LaneContext* lane_context) {
	return lane_context->thread_index;
}

internalf u32 lane_get_thread_n(LaneContext* lane_context) {
	return lane_context->thread_n;
}

internalf void lane_barrier_wait_on_data(LaneContext* lane_context, void* ptr, u64 size, u64 src_thread_index) {
    
	assert(ptr && size);
	if(ptr && size) {
        
		if(size > sizeof(u64)) {
			raise();
			size = sizeof(u64);
		}
        
		if(lane_context->thread_index == src_thread_index) {
			mem_cpy(lane_context->shared_memory, ptr, size);
			barrier_wait(lane_context->barrier);
		} else {
			barrier_wait(lane_context->barrier);
			mem_cpy(ptr, lane_context->shared_memory, size);
		}
	}
}

internalf void lane_barrier_wait(LaneContext* lane_context) {
	barrier_wait(lane_context->barrier);
}


internalf u32 base_lane_get_thread_index(void) {
	ThreadContext* thread_context = (ThreadContext*)thread_local_storage_get(base_thread_context_tls);
	return lane_get_thread_index(&thread_context->lane_context);
}

internalf u32 base_lane_get_thread_n(void) {
	ThreadContext* thread_context = (ThreadContext*)thread_local_storage_get(base_thread_context_tls);
	return lane_get_thread_n(&thread_context->lane_context);
}

internalf void base_lane_barrier_wait_on_data(void* ptr, u64 size, u64 src_thread_index) {
	ThreadContext* thread_context = (ThreadContext*)thread_local_storage_get(base_thread_context_tls);
	lane_barrier_wait_on_data(&thread_context->lane_context, ptr, size, src_thread_index);
}

internalf void base_lane_barrier_wait(void) {
	ThreadContext* thread_context = (ThreadContext*)thread_local_storage_get(base_thread_context_tls);
	lane_barrier_wait(&thread_context->lane_context);
}
// ---- Lanes

// ---- Thread Context
internalf void base_thread_context_set_lane(LaneContext lane_context) {
	ThreadContext* thread_context = (ThreadContext*)thread_local_storage_get(base_thread_context_tls);
	mem_cpy(&thread_context->lane_context, &lane_context, sizeof(LaneContext));
}

internalf void base_thread_context_alloc(void) {
	
	Arena* pre_scratch;
	ThreadContext* thread_context;
	Arena** scratch;
	Arena** end;
    
	pre_scratch = arena_allocate_default();
	thread_context = (ThreadContext*)allocate(&pre_scratch->allocator, sizeof(ThreadContext));
    
	thread_context->scratch_pool[0] = pre_scratch;
	end = thread_context->scratch_pool + THREAD_CONTEXT_SCRATCH_POOL_CAP;
	for(scratch = thread_context->scratch_pool + 1; scratch != end; scratch += 1) {
		*scratch = arena_allocate_default();
	}
    
	thread_local_storage_set(base_thread_context_tls, thread_context);
}

internalf void base_thread_context_dealloc(void) {
	
	ThreadContext* thread_context = (ThreadContext*)thread_local_storage_get(base_thread_context_tls);
	Arena** scratch = thread_context->scratch_pool + THREAD_CONTEXT_SCRATCH_POOL_CAP - 1;
	Arena** end = thread_context->scratch_pool - 1;
	for(; scratch != end; scratch--) {
		arena_deallocate(*scratch);
	}
}

internalf Arena* base_thread_context_get_scratch_arena(Arena** conflict_array, u32 count) {
	
	Arena* result = 0;
	ThreadContext* thread_context = (ThreadContext*)thread_local_storage_get(base_thread_context_tls);
	Arena** scratch = thread_context->scratch_pool;
	Arena** end = thread_context->scratch_pool + THREAD_CONTEXT_SCRATCH_POOL_CAP;
	Arena** conflict;
	Arena** conflict_end = conflict_array + count;
	
	for(; scratch != end; scratch++) {
		if(*scratch) {
			result = *scratch;
			
			for(conflict = conflict_array; conflict != conflict_end; conflict++) {
				if(*scratch == *conflict) {
					result = 0;
					break;
				}
			}
            
			if(result) {
				break;
			}
		}
	}
    
	return result;
}

internalf ArenaTemp scratch_begin(Arena** conflict_arenas, u32 count) {
	ArenaTemp result;
	#ifdef MEMORY_SAFE
	result.arena = (Arena *)0;
	result.pos = 0;
	#else
	Arena* arena = base_thread_context_get_scratch_arena(conflict_arenas, count);
	result = arena_temp_begin(arena);
	#endif  // MEMORY_SAFE
	return result;
}

internalf void scratch_end(ArenaTemp* arena) {
	#ifdef MEMORY_SAFE
	#else
	arena_temp_end(arena);
	#endif  // MEMORY_SAFE
}
// ---- Thread Context

// -- Functions
