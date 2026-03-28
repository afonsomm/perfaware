

// -- Global State
// -- Global State


// -- Functions
internalf void base_files_init(void) {
	file_system_get_executable_path();
	file_system_get_user_directory();
	file_system_get_temp_directory();
}

internalf void base_files_deinit(void) {

	#if OS_SDL
	#elif OS_WINDOWS
	#elif OS_LINUX
	#else
	#error "base_files_deinit: not implemented for this operating system."
	#endif
    
}

// ---- Validators
internalf FileInfo file_info_nil(void) {
	FileInfo result;
	result.path = str8_nil();
	return result;
}

internalf b8 file_info_is_valid(FileInfo info) {
	return str8_is_valid(info.path);
}

internalf File file_nil(void) {
	File result;
	result.content = (void*)0;
	return result;
}

internalf b8 file_is_valid(File file) {
	return (b8)(file.content != (void*)0);
}
// ---- Validators

// ---- File System Paths
internalf String8 file_system_get_current_directory(Allocator* allocator) {
    
	String8 result;
    
	#if OS_SDL
	s8* result_cstr8 = SDL_GetCurrentDirectory();
	if(result_cstr8) {
		result = str8_copy(allocator, str8_cstr((u8*)result_cstr8));
		SDL_free(result_cstr8);
	} else {
		result = str8_nil();
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = win32_file_system_get_current_directory(allocator);
	#elif OS_LINUX
	result = linux_file_system_get_current_directory(allocator);
	#else
	#error "file_system_get_current_directory: not implemented for this operating system."
	#endif
    
	return result;
}

internalf String8 file_system_get_executable_path(void) {
    
	String8 result;
    
	#if OS_SDL
	const s8* result_cstr8 = SDL_GetBasePath();
	if(result_cstr8) {
		result = str8_cstr((u8*)result_cstr8);
	} else {
		result = str8_nil();
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = win32_file_system_get_executable_path();
	#elif OS_LINUX
	result = linux_file_system_get_executable_path();
	#else
	#error "file_system_get_executable_path: not implemented for this operating system."
	#endif
    
	return result;
}

internalf String8 file_system_get_user_directory(void) {
    
	String8 result;
    
	#if OS_SDL
	const s8* result_cstr8 = SDL_GetUserFolder(SDL_FOLDER_HOME);
	if(result_cstr8) {
		result = str8_cstr((u8*)result_cstr8);
	} else {
		result = str8_nil();
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = win32_file_system_get_user_directory();
	#elif OS_LINUX
	result = linux_file_system_get_user_directory();
	#else
	#error "file_system_get_user_directory: not implemented for this operating system."
	#endif

	return result;
}

internalf String8 file_system_get_temp_directory(void) {
    
	String8 result;
 
	Allocator* allocator = (Allocator*)0;
	#if OS_SDL
	allocator = sdl_get_permanent_allocator();
	#endif

	#if OS_WINDOWS
	result = win32_file_system_get_temp_directory(allocator);
	#elif OS_LINUX
	result = linux_file_system_get_temp_directory(allocator);
	#else
	#error "file_system_get_temp_directory: not implemented for this operating system."
	#endif
    
	return result;
}
// ---- File System Paths

// ---- Names/Paths
internalf void file_info_path_parcellate(FileInfo* info) {
    
	if(info->path.str != (u8*)0) {
        
		if((info->properties & FilePropertyFlag_Directory)) {
            
			// -- Directory
			u64 pos = str8_find_char_pos(info->path, FILE_PATH_SEPARATOR_CHAR, 1, StrFindFlag_Backwards);
			info->dir = info->path.str + pos;
			// -- Directory
            
		} else {
			u64 pos;
            
			// -- Filename
			pos = str8_find_char_pos(info->path, FILE_PATH_SEPARATOR_CHAR, 1, StrFindFlag_Backwards);
			info->name = info->path.str + pos;
			// -- Filename
            
			if(!pos) {
				info->dir = (u8*)0;
			} else {
                
				// -- Directory
				pos = str8_find_char_pos(info->path, FILE_PATH_SEPARATOR_CHAR, 2, StrFindFlag_Backwards);
				info->dir = info->path.str + pos;
				// -- Directory
                
			}
            
			// -- Extension
			pos = str8_find_char_pos(info->path, '.', 1, StrFindFlag_Backwards);
			if(pos) {
				info->extension = info->path.str + pos;
			} else {
				info->extension = 0;
			}
			// -- Extension
		}
	}
}

internalf b8 file_name_match_extension(String8 filename, String8 extension, u64 extension_pos) {
    
	b8 result = 0;
    
	if(!extension_pos) {
		extension_pos = str8_find_char_pos(filename, '.', 1, StrFindFlag_Backwards);
	}
    
	if(filename.len - extension_pos) {
		result = mem_match(filename.str + extension_pos, extension.str, sizeof(u8) * (filename.len - extension_pos));
	}
    
	return result;
}

internalf b8 file_rename(String8 current, String8 newname) {
    
	b8 result;
    
	#if OS_SDL
	result = SDL_RenamePath((s8*)current.str, (s8*)newname.str);
	if(result) {
	} else {
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = win32_file_rename(current, newname);
	#elif OS_LINUX
	result = !rename((s8*)current.str, (s8*)newname.str);
	#else
	#error "file_rename: not implemented for this operating system."
	#endif
    
	return result;
}

internalf String8 file_fullpath_from_relative(Allocator* allocator, String8 relative) {
    
	String8 result;
 
	#if OS_WINDOWS
	result = win32_file_fullpath_from_relative(allocator, relative);
	#elif OS_LINUX
	result.str = allocate(allocator, FILE_PATH_DEFAULT_LEN);
	if(!realpath((s8*)relative.str, (s8*)result.str)) {
		deallocate(allocator, result.str, FILE_PATH_DEFAULT_LEN);
		result = str8_nil();
	} else {
		result.len = cstr8_len(result.str);
	}
	#else
	#error "file_fullpath_from_relative: not implemented for this operating system."
	#endif
    
	return result;
}
// ---- Names/Paths

// ---- Loading
internalf File file_load(Allocator* allocator, String8 path) {
	File result;
	FileInfo finfo = file_info_load(allocator, path);
	if(str8_is_valid(finfo.path)) {
		result = file_load_from_info(allocator, finfo);
	} else {
		result = file_nil();
	}
	return result;
}

internalf FileInfo file_info_load(Allocator* allocator, String8 path) {
	// Load metadata from file in path. If the metadata could not be loaded, result.dir is null, so it works for files and directories.
    
	FileInfo result = file_info_nil();
    
	#if OS_SDL
	SDL_PathInfo sdl_info;
	path = file_fullpath_from_relative(allocator, path);
	if(SDL_GetPathInfo((s8*)path.str, &sdl_info)) {
		result.path = path;

		if(sdl_info.type == SDL_PATHTYPE_DIRECTORY) {
			result.properties = FilePropertyFlag_Directory;
		}

		file_info_path_parcellate(&result);
		
		result.update_time = (u64)sdl_info.modify_time;
		result.create_time = (u64)sdl_info.create_time;
		result.size = (u64)sdl_info.size;
		result.access = FileAccessFlag_Readable | FileAccessFlag_Writeable | FileAccessFlag_Executable;  // LIMITATION: SDL cannot get access permissions, so the file info gets every permission
	} else {
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = win32_file_info_load(allocator, path);
	#elif OS_LINUX
	struct stat file_info;
	if(!stat((s8*)path.str, &file_info)) {
		result.path = file_fullpath_from_relative(allocator, path);
        
		result.properties = 0;
		if(S_ISDIR(file_info.st_mode)) {  // Check if it is a directory (has to be done before "file_info_path_parcellate()")
			result.properties |= FilePropertyFlag_Directory;
		}
        
		file_info_path_parcellate(&result);
        
		// -- Convert time in seconds to nanoseconds
		// LIMITATION: update time is equal to the creation time (TODO: use "struct statx" instead of "struct stat" to get the creation/birth time)
		result.create_time = (u64)(file_info.st_mtime * 1000000000);
		result.update_time = (u64)(file_info.st_mtime * 1000000000);
		// -- Convert time in seconds to nanoseconds
        
		result.size = (u64)file_info.st_size;
        
		result.access = 0;
		result.access |= (FileAccessFlag_Readable * !access((s8*)path.str, R_OK));
		result.access |= (FileAccessFlag_Writeable * !access((s8*)path.str, W_OK));
		result.access |= (FileAccessFlag_Executable * !access((s8*)path.str, X_OK));
	} else {
		result.dir = (u8*)0;
	}
	#else
	#error "file_info_load: not implemented for this operating system."
	#endif
    
	return result;
}

internalf File file_load_from_info(Allocator* allocator, FileInfo info) {
    
	File result;
    
	#if OS_SDL
	u64 read_bytes;
	void* content = SDL_LoadFile((s8*)info.path.str, &read_bytes);
	if(content) {
		assert(info.size == read_bytes);
		result.content = content;
		result.info = info;
	} else {
		result.content = (void*)0;
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = win32_file_load_from_info(allocator, info);
	#elif OS_LINUX
	s32 fd;
	u64 bytes_read;
    
	fd = open((s8*)info.path.str, O_RDONLY);
	if(fd > 0) {
		bytes_read = 0;
		do {
			bytes_read += (u64)read(fd, result.content, info.size);
		} while(bytes_read < info.size);
        
		result.info = info;
		close(fd);
	} else {
		result.content = (void*)0;
	}
	#else
	#error "file_load_from_info: not implemented for this operating system."
	#endif
    
	return result;
}
// ---- Loading

// ---- Writing
internalf b8 file_write(String8 path, String8 data) {
    
	b8 result;
    
	#if OS_SDL
	result = SDL_SaveFile((s8*)path.str, data.str, data.len);
	if(result) {
	} else {
		debug_print_cstr8_lit(SDL_GetError());
	}
	#elif OS_WINDOWS
	result = win32_file_write(path, data);
	#elif OS_LINUX
	s32 fd;
    
	fd = open((s8*)path.str, O_CREAT | O_WRONLY, 0666);
	if(fd >= 0) {
		write(fd, (s8*)data.str, data.len);
		close(fd);
		result = 1;
	} else {
		result = 0;
	}
	#else
	#error "file_write: not implemented for this operating system."
	#endif
    
	return result;
}

internalf b8 file_write_list(String8 path, String8List* data) {
    
	b8 result;
    
	#if OS_SDL
	SDL_IOStream* fd = SDL_IOFromFile((s8*)path.str, "wb");
	result = 1;
	if(fd) {
		String8Node* str_node = container_of(data->first.next, String8Node, node);
		for(; &str_node->node != &data->first; str_node = container_of(str_node->node.next, String8Node, node)) {
			if(SDL_WriteIO(fd, str_node->str.str, str_node->str.len) < str_node->str.len) {
				debug_print_cstr8_lit(SDL_GetError());
				result = 0;
				break;
			}
		}
		SDL_CloseIO(fd);
	} else {
		result = 0;
	}
	#elif OS_WINDOWS
	result = win32_file_write_list(path, data);
	#elif OS_LINUX
	s32 fd;
    
	fd = open((s8*)path.str, O_CREAT | O_WRONLY, 0666);
	if(fd > 0) {
		String8Node* str_node = container_of(data->first.next, String8Node, node);
		for(; &str_node->node != &data->first; str_node = container_of(str_node->node.next, String8Node, node)) {
			write(fd, (s8*)str_node->str.str, str_node->str.len);
		}
        
		close(fd);
		result = 1;
	} else {
		result = 0;
	}
	#else
	#error "file_write_list: not implemented for this operating system."
	#endif
    
	return result;
}

internalf b8 file_delete(String8 path) {
    
	b8 result;
    
	#if OS_SDL
	result = SDL_RemovePath((s8*)path.str);
	#elif OS_WINDOWS
	result = win32_file_delete(path);
	#elif OS_LINUX
	result = !remove((s8*)path.str);
	#else
	#error "file_delete: not implemented for this operating system."
	#endif
    
	return result;
}

internalf b8 directory_create(String8 path) {
    
	b8 result;

	#if OS_SDL
	result = SDL_CreateDirectory((s8*)path.str);
	#elif OS_WINDOWS
	result = win32_directory_create(path);
	#elif OS_LINUX
	result = !mkdir((s8*)path.str, 0755);
	#else
	#error "directory_create: not implemented for this operating system."
	#endif
    
	return result;
}

internalf b8 directory_delete(String8 path) {
    
	b8 result;
    
	#if OS_SDL
	result = SDL_RemovePath((s8*)path.str);
	#elif OS_WINDOWS
	result = win32_directory_delete(path);
	#elif OS_LINUX
	result = !rmdir((s8*)path.str);
	#else
	#error "directory_delete: not implemented for this operating system."
	#endif
    
	return result;
}
// ---- Writing

// ---- Iterating
internalf FileIterator file_iterator_init(Allocator* allocator, String8 dir, String8 wildcard) {
  // PROBLEM: default behaviour in SDL is recursive, but the others are not

	FileIterator result;
	result.handle = handle_from_ptr((void*)0);
	#if OS_SDL
	SDLFileIterator* sdl_iterator = (SDLFileIterator*)allocate(allocator, sizeof(SDLFileIterator));
	sdl_iterator->paths = SDL_GlobDirectory((s8*)dir.str, (s8*)wildcard.str, 0, (int*)&sdl_iterator->len);
	if(sdl_iterator->paths) {
		ArenaTemp scratch = scratch_begin((Arena**)&allocator, 1);
		String8 base_dir = file_fullpath_from_relative(&scratch.arena->allocator, dir);
		sdl_iterator->base_dir = str8_cat(allocator, base_dir, str8_lit(FILE_PATH_SEPARATOR_STR));
		result.handle = handle_from_ptr(sdl_iterator);
		scratch_end(&scratch);
	} else {
		deallocate(allocator, sdl_iterator, sizeof(SDLFileIterator));
	}
	#elif OS_WINDOWS
	result = win32_file_iterator_init(allocator, dir, wildcard);
	#elif OS_LINUX
	{
		DIR* dir_handle = opendir((s8*)dir.str);
		if(dir_handle) {
			LinuxFileIterator* linux_file_iter = (LinuxFileIterator*)allocate(allocator, sizeof(LinuxFileIterator));
			if(linux_file_iter) {
				ArenaTemp scratch;
				String8 base_dir;
                
				linux_file_iter->dir = dir_handle;
                
				scratch = scratch_begin((Arena**)&allocator, 1);
                
				base_dir = file_fullpath_from_relative(&scratch.arena->allocator, dir);
				linux_file_iter->base_dir = str8_cat(allocator, base_dir, str8_lit(FILE_PATH_SEPARATOR_STR));
				linux_file_iter->wildcard = wildcard;
				result.handle = handle_from_ptr(linux_file_iter);
                
				scratch_end(&scratch);
			}
		}
	}
	#else
	#error "file_iterator_init: not implemented for this operating system."
	#endif
    
	return result;
}

internalf b8 file_iterator_next(Allocator* allocator, FileIterator iterator, FileInfo* out, enum32(FileIteratorFlag) flags) {
    
	b8 result;
    
	#if OS_SDL
	SDLFileIterator* sdl_iterator = (SDLFileIterator*)ptr_from_handle(iterator.handle);
	if(sdl_iterator->curr < sdl_iterator->len) {
		s8* path = sdl_iterator->paths[sdl_iterator->curr];
		String8 path_str8 = str8_cat(allocator, sdl_iterator->base_dir, str8_cstr((u8*)path));
		*out = file_info_load(allocator, path_str8);
		sdl_iterator->curr += 1;
		result = 1;
	} else {
		result = 0;
	}
	#elif OS_WINDOWS
	result = win32_file_iterator_next(allocator, iterator, out, flags);
	#elif OS_LINUX
	LinuxFileIterator* linux_iter = (LinuxFileIterator*)ptr_from_handle(iterator.handle);
	struct dirent* entry;
    
	do {
		entry = readdir(linux_iter->dir);
	} while(entry
		&& ((entry->d_name[0] == '.' && entry->d_name[1] == '\0')
			|| (entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0')));  // skip "." and ".."
    
	if(entry && !fnmatch((s8*)linux_iter->wildcard.str, entry->d_name, 0)) {
		String8 path = str8_cstr((u8*)entry->d_name);
		path = str8_cat(allocator, linux_iter->base_dir, path);
		*out = file_info_load(allocator, path);
		result = 1;
	} else {
		result = 0;
	}
	#else
	#error "file_iterator_next: not implemented for this operating system."
	#endif
    
	return result;
}

internalf void file_iterator_end(FileIterator iterator) {
  
	#if OS_SDL
	SDLFileIterator* sdl_iterator = (SDLFileIterator*)ptr_from_handle(iterator.handle);
	SDL_free(sdl_iterator->paths);
	#elif OS_WINDOWS
	win32_file_iterator_end(iterator);
	#elif OS_LINUX
	LinuxFileIterator* linux_iter = (LinuxFileIterator*)ptr_from_handle(iterator.handle);
	closedir(linux_iter->dir);
	#else
	#error "file_iterator_end: not implemented for this operating system."
	#endif
}
// ---- Iterating

// -- Functions
