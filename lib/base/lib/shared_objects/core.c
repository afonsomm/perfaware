

// -- Functions
internalf SharedLibrary shared_library_load(String8 path) {
    
	SharedLibrary result;
	
	#if OS_SDL
	SDL_SharedObject* sdl_object = SDL_LoadObject((s8*)path.str);
	if(sdl_object) {
		result.handle = handle_from_ptr(sdl_object);
	} else {
		result.handle = handle_from_ptr((void*)0);
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	ArenaTemp scratch = scratch_begin(0, 0);
	String16 path16 = str16_from_8(&scratch.arena->allocator, path);
	result.handle = handle_from_ptr(LoadLibraryW((LPCWSTR)path16.str));
	scratch_end(&scratch);
	#elif OS_LINUX
	result.handle = handle_from_ptr(dlopen((s8*)path.str, RTLD_LAZY));
	#else
	# error "shared_library_load: not implemented for this operating system."
	#endif
	
	return result;
}

internalf SharedLibraryProcedure* shared_library_load_procedure(SharedLibrary lib, const s8* name) {
    
	SharedLibraryProcedure* result;
    
	#if OS_SDL
	result = SDL_LoadFunction((SDL_SharedObject*)ptr_from_handle(lib.handle), name);
	if(result) {
	} else {
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = (SharedLibraryProcedure*)GetProcAddress((HMODULE)ptr_from_handle(lib.handle), name);
	#elif OS_LINUX
	result = (SharedLibraryProcedure*)dlsym(ptr_from_handle(lib.handle), name);
	#else
	# error "shared_library_load_procedure: not implemented for this operating system."
	#endif
    
	return result;
}

internalf void shared_library_unload(SharedLibrary lib) {
    
	#if OS_SDL
	SDL_UnloadObject((SDL_SharedObject*)ptr_from_handle(lib.handle));
	#elif OS_WINDOWS
	FreeLibrary((HMODULE)ptr_from_handle(lib.handle));
	#elif OS_LINUX
	dlclose(ptr_from_handle(lib.handle));
	#else
	# error "shared_library_unload: not implemented for this operating system."
	#endif
}
// -- Functions
