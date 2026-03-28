

// -- Global State
global LinuxState linux_state;
// -- Global State


// -- Functions

// ---- Files
internalf String8 linux_file_system_get_current_directory(Allocator* allocator) {
    
    String8 result;
    
    result.len = FILE_PATH_DEFAULT_LEN + 1;
    result.str = allocate(allocator, result.len);
    if(getcwd((s8*)result.str, result.len) == (s8*)0) {
        deallocate(allocator, result.str, result.len);
        result = str8_nil();
    }
    
    return result;
}

internalf String8 linux_file_system_get_executable_path(void) {
    
    String8 result;
    
    if(linux_state.executable_path.str == (u8*)0) {
        ArenaTemp scratch;
        u64 buffer_cap;
        
        scratch = scratch_begin(0, 0);
        
        buffer_cap = FILE_PATH_DEFAULT_LEN + 1;
        linux_state.executable_path.str = allocate(&linux_state.permanent_arena->allocator, buffer_cap);
        linux_state.executable_path.len = readlink("/proc/self/exe", (s8*)linux_state.executable_path.str, buffer_cap);
        
        while(linux_state.executable_path.len == buffer_cap) {
            deallocate(&linux_state.permanent_arena->allocator, linux_state.executable_path.str, buffer_cap);
            buffer_cap *= 2;
            linux_state.executable_path.str = allocate(&linux_state.permanent_arena->allocator, buffer_cap);
            linux_state.executable_path.len = readlink("/proc/self/exe", (s8*)linux_state.executable_path.str, buffer_cap);
        }
        
        if(linux_state.executable_path.len == -1) {
            deallocate(&linux_state.permanent_arena->allocator, linux_state.executable_path.str, buffer_cap);
            linux_state.executable_path = str8_nil();
        }
        
        scratch_end(&scratch);
    }
    
    result = linux_state.executable_path;
    
    return result;
}

internalf String8 linux_file_system_get_user_directory(void) {
    
    String8 result;
    
    if(linux_state.user_directory.str == (u8*)0) {
        uid_t uid = getuid();
        struct passwd* pw = getpwuid(uid);
        if(pw) {
            u64 len;
            
            linux_state.user_directory.len = FILE_PATH_DEFAULT_LEN + 1;
            linux_state.user_directory.str = allocate(&linux_state.permanent_arena->allocator, sizeof(u8) * linux_state.user_directory.len);
            
            len = cstr8_len((u8*)pw->pw_dir);
            mem_cpy(linux_state.user_directory.str, pw->pw_dir, sizeof(u8) * len);
        }
    }
    
    result = linux_state.user_directory;
    
    return result;
}

internalf String8 linux_file_system_get_temp_directory(Allocator* allocator) {
    
    String8 result;

		if(!allocator) {
			allocator = &linux_state.permanent_arena->allocator;
		}
    
    if(linux_state.temp_directory.str == (u8*)0) {
        s8* temp_dir;
        u64 len;
        
        temp_dir = getenv("TMPDIR");
        if(!temp_dir) {
            temp_dir = "/tmp";
        }
        
        linux_state.temp_directory.len = FILE_PATH_DEFAULT_LEN + 1;
        linux_state.temp_directory.str = allocate(allocator, sizeof(u8) * linux_state.temp_directory.len);
        
        len = cstr8_len((u8*)temp_dir);
        mem_cpy(linux_state.temp_directory.str, temp_dir, sizeof(u8) * len);
    }
    
    result = linux_state.temp_directory;
    
    return result;
}
// ---- Files

// ---- Time
internalf void linux_time_init(void) {
    
    clockid_t clockid;
    
    if(!clock_getcpuclockid(0, &clockid)) {
        struct timespec spec;
        if(!clock_getres(clockid, &spec)) {
            linux_state.time_res_ns = (u64)spec.tv_sec * 1000000000 + (u64)spec.tv_nsec;
        }
    }
}

internalf u64 linux_time_in_nanos(void) {
	
	u64 result;
    
	struct timespec spec;
	clock_gettime(CLOCK_MONOTONIC, &spec);
	result = (u64)spec.tv_sec * 1000000000 + (u64)spec.tv_nsec;
    
	return result;
}

internalf struct tm linux_tm_from_time_date(TimeDate* date) {
    
    struct tm result = {0};
    
    result.tm_sec  = (s32)round_r32((r32)(date->sec * 1000 + date->msec));
    result.tm_min  = (s32)date->min;
    result.tm_hour = (s32)date->hour;
    result.tm_mday = (s32)date->day;
    result.tm_mon  = (s32)date->month;
    result.tm_year = (s32)date->year;
    
    return result;
}

internalf TimeDate linux_time_date_from_tm(struct tm* broken_down_time, u16 msec) {
    
    TimeDate result = {0};
    
    result.msec  = msec;
    result.sec   = (u8)broken_down_time->tm_sec;
    result.min   = (u8)broken_down_time->tm_min;
    result.hour  = (u8)broken_down_time->tm_hour;
    result.day   = (u8)broken_down_time->tm_mday;
    result.month = (u8)broken_down_time->tm_mon;
    result.year  = (s16)broken_down_time->tm_year;
    
    return result;
}

// ---- Time

// ---- Memory
internalf u64 linux_mem_get_large_page_size(void) {
    return linux_state.memory_large_page_size;
}

internalf u64 linux_mem_get_page_size(void) {
    return linux_state.memory_page_size;
}
// ---- Memory

// ---- Logical Processors
internalf u32 linux_get_logical_processor_n(void) {
    return linux_state.logical_processor_n;
}
// ---- Logical Processors

// ---- Objects
internalf LinuxObject* linux_object_alloc(void) {
    LinuxObject* result;
    
    Sll* node = sll_stack_pop(&linux_state.free_objects);
    if(node) {
        result = container_of(node, LinuxObject, node);
    } else {
        result = allocate(&linux_state.permanent_arena->allocator, sizeof(LinuxObject));
    }
    
    return result;
}

internalf void linux_object_dealloc(LinuxObject* object) {
    sll_stack_push(&linux_state.free_objects, &object->node);
}
// ---- Objects

// ---- Entry
internalf void linux_main_thread_entry_point(int argc, char* argv[]) {
    
    struct sysinfo* info;
    
    // -- Initialize State
    linux_state.memory_page_size = sysconf(_SC_PAGESIZE);
    linux_state.allocation_granularity = linux_state.memory_page_size;
    linux_state.logical_processor_n = get_nprocs();
    
    {
        FILE* fp = fopen("/proc/meminfo", "r");
        linux_state.memory_large_page_size = 0;
        if(fp) {
            s8 buffer[1024];
            while(fgets(buffer, 1024, fp)) {
                if(mem_match(buffer, "Hugepagesize:", sizeof("Hugepagesize:") - 1)) {
                    sscanf(buffer, "Hugepagesize: %ld kB", &linux_state.memory_large_page_size);
                    linux_state.memory_large_page_size = kilobytes(linux_state.memory_large_page_size);
                    break;
                }
            }
        }
        fclose(fp);
        
        if(!linux_state.memory_large_page_size) {
            linux_state.memory_large_page_size = linux_state.memory_page_size;
        }
    }
    
    linux_state.permanent_arena = arena_allocate_default();
    sll_stack_init(&linux_state.free_objects);
    // -- Initialize State

    
		base_main_thread_entry_point(argv, argv);
		
    arena_deallocate(linux_state.permanent_arena);
}

internalf void* linux_supplement_thread_entry_point(void* object) {
    LinuxObject* thread_object = (LinuxObject*)object;
    base_supplement_thread_entry_point(thread_object->object.thread.func, thread_object->object.thread.params);
}

#ifndef EXTERNAL_ENTRY
int main(int argc, char* argv[]) {
    linux_main_thread_entry_point(argc, argv);
}
#endif
// ---- Entry

// -- Functions
