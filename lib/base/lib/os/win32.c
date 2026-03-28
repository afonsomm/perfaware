

// -- Global State
global Win32State win32_state;
// -- Global State


// -- Functions

// ---- Memory
internalf u64 win32_mem_get_large_page_size(void) {
	return win32_state.mem_large_page_size;
}

internalf u64 win32_mem_get_page_size(void) {
	return win32_state.mem_page_size;
}
// ---- Memory

// ---- Threads
internalf u32 win32_get_logical_processor_n(void) {
	return (u32)win32_state.logical_processor_n;
}
// ---- Threads

// ---- Time
internalf void win32_time_init(void) {

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	win32_state.performance_counter_period = 1000000000.0 / (r64)frequency.QuadPart;
	timeBeginPeriod(1);
}

internalf u64 win32_time_in_nanos(void) {
	
	u64 result = 0;
    
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	result = (u64)((r64)counter.QuadPart * win32_state.performance_counter_period);
    
	return result;
}

internalf TimeDate win32_time_local_from_universal(TimeDate* date) {
    
	TimeDate result;
    
	SYSTEMTIME universal_systemtime = win32_time_systemtime_from_date(date);
	FILETIME universal_filetime;
	FILETIME local_filetime;
	SYSTEMTIME local_systemtime;

	mem_zero(&universal_filetime, sizeof(universal_filetime));
	mem_zero(&local_filetime, sizeof(local_filetime));
	mem_zero(&universal_systemtime, sizeof(local_systemtime));
	SystemTimeToFileTime(&universal_systemtime, &universal_filetime);
	FileTimeToLocalFileTime(&universal_filetime, &local_filetime);
	FileTimeToSystemTime(&local_filetime, &local_systemtime);
	result = win32_time_date_from_systemtime(&local_systemtime);
    
	return result;
}

internalf TimeDate win32_time_universal_from_local(TimeDate* date) {
    
	TimeDate result;
    
	SYSTEMTIME local_systemtime = win32_time_systemtime_from_date(date);
	FILETIME local_filetime;
	FILETIME universal_filetime;
	SYSTEMTIME universal_systemtime;

	mem_zero(&local_filetime, sizeof(local_filetime));
	mem_zero(&universal_filetime, sizeof(universal_filetime));
	mem_zero(&universal_systemtime, sizeof(universal_systemtime));
	SystemTimeToFileTime(&local_systemtime, &local_filetime);
	LocalFileTimeToFileTime(&local_filetime, &universal_filetime);
	FileTimeToSystemTime(&universal_filetime, &universal_systemtime);
	result = win32_time_date_from_systemtime(&universal_systemtime);
    
	return result;
}

internalf SYSTEMTIME win32_time_systemtime_from_date(TimeDate* date) {
    
	SYSTEMTIME result;
    
	result.wYear = (WORD)date->year;
	result.wMonth = (WORD)date->month;
	result.wDay = (WORD)date->day + 1;
	result.wHour = (WORD)date->hour;
	result.wMinute = (WORD)date->min;
	result.wSecond = (WORD)date->sec;
	result.wMilliseconds = (WORD)date->msec;
    
	return result;
}

internalf TimeDate win32_time_date_from_systemtime(SYSTEMTIME* time) {
    
	TimeDate result;
    
	result.year = (s16)time->wYear;
	result.month = (u8)time->wMonth;
	result.day = (u8)time->wDay - 1;
	result.hour = (u8)time->wHour;
	result.min = (u8)time->wMinute;
	result.sec = (u8)time->wSecond;
	result.msec = time->wMilliseconds;
    
	return result;
}

internalf TimeDense win32_time_dense_from_filetime(FILETIME* time) {
	
	SYSTEMTIME system_time;
	TimeDate date;
	
	mem_zero(&system_time, sizeof(SYSTEMTIME));
	FileTimeToSystemTime(time, &system_time);
	date = win32_time_date_from_systemtime(&system_time);
	
	return time_dense_from_date(&date);
}
// ---- Time

// ---- Files

// ------ Names/Paths
internalf b8 win32_file_rename(String8 current, String8 newname) {
	
	b8 result = 1;
    
	ArenaTemp scratch = scratch_begin(0, 0);
	String16 current16 = str16_from_8(&scratch.arena->allocator, current);
	String16 newname16 = str16_from_8(&scratch.arena->allocator, newname);
    
	if(MoveFileW((WCHAR*)current16.str, (WCHAR*)newname16.str)) {
		memcpy(current.str, newname.str, newname.len);
		current.len = newname.len;
		current.str[current.len] = '\0';
	} else {
		result = 0;
	}
    
	scratch_end(&scratch);
    
	return result;
}

internalf String8 win32_file_fullpath_from_relative(Allocator* allocator, String8 relative) {
	
	String8 result = str8_nil();
    
	ArenaTemp scratch = scratch_begin((Arena* *)&allocator, 1);
	String16 relative16 = str16_from_8(&scratch.arena->allocator, relative);
	String16 result16 = str16_nil();
	u64 buffer_len = FILE_PATH_DEFAULT_LEN + 1;
	
	result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * buffer_len);
	result16.len = (u64)GetFullPathNameW((WCHAR*)relative16.str, (DWORD)buffer_len, (WCHAR*)result16.str, 0);
	
	if(result16.len > buffer_len) {  // buffer was too small
		deallocate(&scratch.arena->allocator, result16.str, sizeof(u16) * buffer_len);
		result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * result16.len);
		result16.len = (u64)GetFullPathNameW((WCHAR*)relative16.str, (DWORD)result16.len, (WCHAR*)result16.str, 0);
	}
	
	if(result16.len) {
		result = str8_from_16(allocator, result16);
	}
	
	scratch_end(&scratch);
	
	return result;
}

internalf String8 win32_file_system_get_current_directory(Allocator* allocator) {
    
	String8 result;
	ArenaTemp scratch;
	String16 result16;
	u64 buffer_cap;
    
	scratch = scratch_begin((Arena* *)&allocator, 1);
    
	buffer_cap = FILE_PATH_DEFAULT_LEN + 1;  // "+ 1" for the null terminator
	result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * buffer_cap);
	result16.len = (u64)GetCurrentDirectoryW((DWORD)buffer_cap, (WCHAR*)result16.str);  // get actual size of path
    
	if(result16.len > buffer_cap) {  // reallocate if the actual size is larger that the currently allocated buffer
		deallocate(&scratch.arena->allocator, result16.str, sizeof(u16) * buffer_cap);
		result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * result16.len);
		result16.len = (u64)GetCurrentDirectoryW((DWORD)result16.len, (WCHAR*)result16.str);
	}
    
	if(result16.len) {
		result = str8_from_16(allocator, result16);
	} else {
		result = str8_nil();
	}
    
	scratch_end(&scratch);
    
	return result;
}

internalf String8 win32_file_system_get_executable_path(void) {
    
	String8 result;
    
	if(win32_state.executable_path.str == (u8*)0) {
		ArenaTemp scratch;
		String16 result16;
		u64 buffer_cap;
        
		scratch = scratch_begin(0, 0);
        
		buffer_cap = FILE_PATH_DEFAULT_LEN + 1;
		result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * buffer_cap);
		result16.len = (u64)GetModuleFileNameW(0, (WCHAR*)result16.str, (DWORD)buffer_cap);
        
		while(result16.len == buffer_cap) {
			deallocate(&scratch.arena->allocator, result16.str, sizeof(u16) * buffer_cap);
			buffer_cap *= 2;
			result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * buffer_cap);
			result16.len = (u64)GetModuleFileNameW(0, (WCHAR*)result16.str, (DWORD)buffer_cap);
		}
        
		if(result16.len) {
			win32_state.executable_path = str8_from_16(&win32_state.permanent_arena->allocator, result16);
		}
        
		scratch_end(&scratch);
	}
    
	result = win32_state.executable_path;
    
	return result;
}

internalf String8 win32_file_system_get_user_directory(void) {
    
	String8 result;
    
	if(win32_state.user_directory.str == (u8*)0) {
		ArenaTemp scratch;
		String16 result16;
		u64 buffer_cap;
		HANDLE token;
        
		scratch = scratch_begin(0, 0);
        
		buffer_cap = FILE_PATH_DEFAULT_LEN + 1;
		result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * buffer_cap);
		token = GetCurrentProcessToken();
		if(!GetUserProfileDirectoryW(token, (WCHAR*)result16.str, (DWORD*)&buffer_cap)) {
			deallocate(&scratch.arena->allocator, result16.str, sizeof(u16) * buffer_cap);
			if(GetUserProfileDirectoryW(token, (WCHAR*)result16.str, (DWORD*)&buffer_cap)) {
				result16.len = buffer_cap;
			}
		}
        
		if(result16.len) {
			win32_state.user_directory = str8_from_16(&win32_state.permanent_arena->allocator, result16);
		}
        
		scratch_end(&scratch);
	}
    
	result = win32_state.user_directory;
    
	return result;
}

internalf String8 win32_file_system_get_temp_directory(Allocator* allocator) {
    
	String8 result;

	if(!allocator) {
		allocator = &win32_state.permanent_arena->allocator;
	}
    
	if(win32_state.temp_directory.str == (u8*)0) {
		ArenaTemp scratch;
		String16 result16;
		u64 buffer_cap;
        
		scratch = scratch_begin((Arena**)&allocator, 1);
        
		buffer_cap = FILE_PATH_DEFAULT_LEN + 1;
		result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * buffer_cap);
		result16.len = (u64)GetTempPathW((DWORD)buffer_cap, (WCHAR*)result16.str);
        
		if(result16.len > buffer_cap) {
			deallocate(&scratch.arena->allocator, result16.str, sizeof(u16) * buffer_cap);
			result16.str = (u16*)allocate(&scratch.arena->allocator, sizeof(u16) * result16.len);
			result16.len = (u64)GetTempPathW((DWORD)result16.len, (WCHAR*)result16.str);
		}
        
		if(result16.len) {
			win32_state.temp_directory = str8_from_16(allocator, result16);
		}
        
		scratch_end(&scratch);
	}
    
	result = win32_state.temp_directory;
    
	return result;
}
// ------ Names/Paths

// ------ Loading
internalf FileInfo win32_file_info_load(Allocator* allocator, String8 path) {
    
	FileInfo result = file_info_nil();
    
	WIN32_FILE_ATTRIBUTE_DATA finfo;
	ArenaTemp scratch = scratch_begin((Arena* *)&allocator, 1);
	String16 path16 = str16_from_8(&scratch.arena->allocator, path);
    
	if(GetFileAttributesExW((WCHAR*)path16.str, GetFileExInfoStandard, &finfo)) {
		result.update_time = qword_from_dwords(finfo.ftLastWriteTime.dwHighDateTime, finfo.ftLastWriteTime.dwLowDateTime);
		result.create_time = qword_from_dwords(finfo.ftCreationTime.dwHighDateTime, finfo.ftCreationTime.dwLowDateTime);
		
		// -- Convert 100-nanosecond intervals to nanoseconds
		result.update_time /= 100;
		result.create_time /= 100;
		// -- Convert 100-nanosecond intervals to nanoseconds
		
		result.size = qword_from_dwords(finfo.nFileSizeHigh, finfo.nFileSizeLow);
        
		// -- Properties
		if(finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			result.properties = FilePropertyFlag_Directory;
		}
		// -- Properties
        
		// -- Access
		result.access = FileAccessFlag_Readable | FileAccessFlag_Executable;
		if(!(finfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
			result.access |= FileAccessFlag_Writeable;
		}
		// -- Access
		
		result.path = file_fullpath_from_relative(allocator, path);
	}
    
	scratch_end(&scratch);
    
	file_info_path_parcellate(&result);
    
	return result;
}

internalf File win32_file_load_from_info(Allocator* allocator, FileInfo info) {
    
	File result = file_nil();
    
	if(info.size) {
		ArenaTemp scratch = scratch_begin((Arena* *)&allocator, 1);
		String16 path16 = str16_from_8(&scratch.arena->allocator, info.path);
        
		HANDLE fhandle = CreateFileW((WCHAR*)path16.str, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if(fhandle != INVALID_HANDLE_VALUE) {
			u8* curr;
			u8* end;
			DWORD n_bytes_read;
			DWORD n_bytes_to_read;
            
			result.content = allocate(allocator, sizeof(u8) * (info.size + 1));
			curr = (u8*)result.content;
			end = (u8*)curr + info.size;
			curr[info.size] = '\0';
            
			while(curr < end) {
				n_bytes_to_read = (DWORD)min(end - curr, U32_MAX);
				if(!ReadFile(fhandle, curr, n_bytes_to_read, &n_bytes_read, 0)) {
					deallocate(allocator, result.content, sizeof(u8) * (info.size + 1));
					result.content = (u8*)0;
					break;
				}
				curr += n_bytes_read;
			}
            
			CloseHandle(fhandle);
		}
        
		scratch_end(&scratch);
	}
    
    
	if(result.content) {
		result.info = info;
	}
    
	return result;
}
// ------ Loading

// ------ Writing
internalf b8 win32_file_write(String8 path, String8 data) {
	
	b8 result = 1;
    
	ArenaTemp scratch = scratch_begin(0, 0);
	String16 path16 = str16_from_8(&scratch.arena->allocator, path);
	
	HANDLE fhandle = CreateFileW((WCHAR*)path16.str, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(fhandle != INVALID_HANDLE_VALUE) {
		result = win32_file_write_from_handle(fhandle, data);
		CloseHandle(fhandle);
	}
	
	scratch_end(&scratch);
    
	return result;
}

internalf b8 win32_file_write_list(String8 path, String8List* data) {
    
	b8 result = 1;
    
	ArenaTemp scratch = scratch_begin(0, 0);
	String16 path16 = str16_from_8(&scratch.arena->allocator, path);
    
	HANDLE fhandle = CreateFileW((WCHAR*)path16.str, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(fhandle != INVALID_HANDLE_VALUE) {
		String8Node* node = container_of(data->first.next, String8Node, node);
		for(; &node->node != &data->first; node = container_of(node->node.next, String8Node, node)) {
			result = win32_file_write_from_handle(fhandle, node->str);
		}
		CloseHandle(fhandle);
	}
    
	scratch_end(&scratch);
    
	return result;
}

internalf b8 win32_file_delete(String8 path) {
	
	b8 result = 1;
	
	ArenaTemp scratch = scratch_begin(0, 0);
	String16 path16 = str16_from_8(&scratch.arena->allocator, path);
	result = (b8)DeleteFileW((WCHAR*)path16.str);
	scratch_end(&scratch);
    
    
	return result;
}

internalf b8 win32_directory_create(String8 path) {
	
	b8 result = 1;
	
	ArenaTemp scratch = scratch_begin(0, 0);
	String16 path16 = str16_from_8(&scratch.arena->allocator, path);
	result = !!CreateDirectoryW((WCHAR*)path16.str, 0);
	scratch_end(&scratch);
    
	
	return result;
}

internalf b8 win32_directory_delete(String8 path) {
    
	b8 result = 1;
	
	ArenaTemp scratch = scratch_begin(0, 0);
	String16 path16 = str16_from_8(&scratch.arena->allocator, path);
	result = (b8)RemoveDirectoryW((WCHAR*)path16.str);
	scratch_end(&scratch);
    
    
	return result;
}

internalf b8 win32_file_write_from_handle(HANDLE fhandle, String8 data) {
    
	b8 result = 1;
    
	u8* curr = data.str;
	u8* end = data.str + data.len;
	DWORD n_bytes_to_write;
	DWORD n_bytes_written;
    
	while(curr < end) {
		n_bytes_to_write = (DWORD)min(end - curr, U32_MAX);
		if(!WriteFile(fhandle, data.str, n_bytes_to_write, &n_bytes_written, 0)) {
			result = 0;
			break;
		}
		curr += n_bytes_written;
	}
    
	return result;
}
// ------ Writing

// ------ Iterator
internalf FileIterator win32_file_iterator_init(Allocator* allocator, String8 dir, String8 wildcard) {
    
	FileIterator result;
    
	Win32FileIterator* win32_iterator = (Win32FileIterator*)allocate(allocator, sizeof(Win32FileIterator));
	result.handle = handle_from_ptr((void*)0);
	if(win32_iterator) {
		ArenaTemp scratch;
		String16 dir_with_wildcard16;
		HANDLE fhandle;
        
		scratch = scratch_begin((Arena* *)&allocator, 1);
        
		// -- Get whole string with wildcard
		String8 dir_with_wildcard;
		{
			String8List str_list;
			String8Join str_join = { str8_nil(), str8_nil(), str8_nil() };
			if(dir.str[dir.len - 1] != FILE_PATH_SEPARATOR_CHAR) {
				str_join.mid = str8((u8*)FILE_PATH_SEPARATOR_STR, 1);
			}
			str8_list_init(&str_list);
			str8_list_push(&scratch.arena->allocator, &str_list, dir);
			str8_list_push(&scratch.arena->allocator, &str_list, wildcard);
			dir_with_wildcard = str8_join(&scratch.arena->allocator, &str_list, &str_join);
		}
		// -- Get whole string with wildcard
        
		dir_with_wildcard16 = str16_from_8(&scratch.arena->allocator, dir_with_wildcard);
        
		fhandle = FindFirstFileW((WCHAR*)dir_with_wildcard16.str, &win32_iterator->find_data);
		if(fhandle != INVALID_HANDLE_VALUE) {
			String8 base_path = str8_chop(dir_with_wildcard, wildcard.len);
			win32_iterator->base_dir = file_fullpath_from_relative(allocator, base_path);
			win32_iterator->handle = fhandle;
			result.handle = handle_from_ptr(win32_iterator);
		} else {
			deallocate(allocator, win32_iterator, sizeof(Win32FileIterator));
		}
        
		scratch_end(&scratch);
	}
	
	return result;
}

internalf b8 win32_file_iterator_next(Allocator* allocator, FileIterator iterator, FileInfo* out, enum32(FileIteratorFlag) flags) {
    
	b8 result = 0;
	Win32FileIterator* win32_iterator = (Win32FileIterator*)ptr_from_handle(iterator.handle);
    
	start:
		mem_zero(&win32_iterator->find_data, sizeof(WIN32_FIND_DATAA));
	if(FindNextFileW(win32_iterator->handle, &win32_iterator->find_data)) {
        
		b8 is_dot1 = (win32_iterator->find_data.cFileName[0] == L'.' && win32_iterator->find_data.cFileName[1] == L'\0');
		b8 is_dot2 = (win32_iterator->find_data.cFileName[0] == L'.' && win32_iterator->find_data.cFileName[1] == L'.' && win32_iterator->find_data.cFileName[2] == L'\0');
		
		if(!is_dot1 && !is_dot2) {
			WIN32_FIND_DATAW find_data = win32_iterator->find_data;
            
			mem_zero(out, sizeof(FileInfo));
			
			out->update_time = qword_from_dwords(find_data.ftLastWriteTime.dwHighDateTime, find_data.ftLastWriteTime.dwLowDateTime);
			out->create_time = qword_from_dwords(find_data.ftCreationTime.dwHighDateTime, find_data.ftCreationTime.dwLowDateTime);
			out->size = qword_from_dwords(find_data.nFileSizeHigh, find_data.nFileSizeLow);
            
			// -- Properties
			if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				out->properties = FilePropertyFlag_Directory;
			}
			// -- Properties
            
			// -- Access
			out->access = FileAccessFlag_Readable | FileAccessFlag_Executable;
			if(!(find_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
				out->access |= FileAccessFlag_Writeable;
			}
			// -- Access
			
			{
				ArenaTemp scratch = scratch_begin((Arena* *)&allocator, 1);
				String16 relative_path16 = str16_cstr((u16*)find_data.cFileName);
				String8 relative_path = str8_from_16(&scratch.arena->allocator, relative_path16);
				out->path = str8_cat(allocator, win32_iterator->base_dir, relative_path);
				scratch_end(&scratch);
			}
            
			result = 1;
		} else {
			goto start;
		}
	} else {
		assert(GetLastError() == ERROR_NO_MORE_FILES);
	}
    
	file_info_path_parcellate(out);
    
	return result;
}

internalf void win32_file_iterator_end(FileIterator iterator) {
	Win32FileIterator* win32_iterator = (Win32FileIterator*)ptr_from_handle(iterator.handle);
	FindClose(win32_iterator->handle);
}
// ------ Iterator

// ---- Files

// ---- Objects
internalf Win32Object* win32_object_alloc(void) {
    
	Win32Object* result;
    
	Sll* node = sll_stack_pop(&win32_state.free_objects);
	if(node) {
		result = container_of(node, Win32Object, node);
	} else {
		result = (Win32Object*)allocate(&win32_state.permanent_arena->allocator, sizeof(Win32Object));
	}
    
	return result;
}

internalf void win32_object_dealloc(Win32Object* object) {
	sll_stack_push(&win32_state.free_objects, &object->node);
}
// ---- Objects

// ---- Entry
internalf void win32_main_thread_entry_point(int argc, WCHAR** argv) {
	
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	
	// -- Initialize State
	win32_state.mem_page_size = info.dwPageSize;
	win32_state.logical_processor_n = info.dwNumberOfProcessors;
	win32_state.mem_large_page_size = GetLargePageMinimum();
	win32_state.allocation_granularity = info.dwAllocationGranularity;
	win32_state.permanent_arena = arena_allocate_default();
	sll_stack_init(&win32_state.free_objects);
	// -- Initialize State
    
	{
		s8** argv_cstr8;
		u32 arg_i;
		String16 arg16;
		String8 arg;

		argv_cstr8 = (s8**)allocate(&win32_state.permanent_arena->allocator, sizeof(s8*) * (u32)argc);
        
		for(arg_i = 0; arg_i < (u32)argc; arg_i += 1) {
			arg16 = str16_cstr((u16*)argv[arg_i]);
			arg = str8_from_16(&win32_state.permanent_arena->allocator, arg16);
			argv_cstr8[arg_i] = (s8*)arg.str;
		}
		
		base_main_thread_entry_point(argc, argv_cstr8);
	}
    
	arena_deallocate(win32_state.permanent_arena);
}

DWORD WINAPI win32_supplement_thread_entry_point(void* object) {
	Win32Object* thread_object = (Win32Object*)object;
	base_supplement_thread_entry_point(thread_object->object.thread.func, thread_object->object.thread.params);
	return 1;
}

#ifndef EXTERNAL_ENTRY
#ifdef GFX_INC_H
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	win32_main_thread_entry_point(__argc, __wargv);
	return 0;
}
#else
int wmain(int argc, WCHAR** argv) {
	win32_main_thread_entry_point(argc, argv);
	return 0;
}
#endif
#endif
// ---- Entry

// -- Functions
