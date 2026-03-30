

// -- Global State

// ---- Command Line Arguments
global CmdLineArgParams base_cmd_line_args_params[] = {
	{ str8_lit_comp("-async_thread_n"), 1 },
};

global CmdLineArgParams empty_custom_cmd_line_args_params[] = {
	{ str8_lit_comp(""), 1 },
};
global CmdLineArgs cmd_line_args;
// ---- Command Line Arguments

// -- Global State


// -- Functions

internalf void base_main_thread_init(s32 argc, s8* argv[]) {

	cmd_line_args_init(argc, argv);

	base_threads_init(str8_to_u32(cmd_line_args_get_value(str8_lit("-async_thread_n"), 0)));
	base_time_init();
	base_files_init();
	
	#ifdef PROFILE
	base_profile_init();
	#endif
    
	#ifdef GFX_INC_H
	gfx_init();
	#endif
    
	#ifdef MIDI_INC_H
	midi_init();
	#endif
  
	// FILLER: other libraries initializers
}

internalf void base_main_thread_deinit(void) {
	
	// FILLER: ohter libraries deinitializers
	
	#ifdef MIDI_INC_H
	midi_deinit();
	#endif
    
	#ifdef GFX_INC_H
	gfx_deinit();
	#endif
    
	#ifdef PROFILE
	base_profile_deinit();
	#endif
    
	base_files_deinit();
	base_time_deinit();
	base_threads_deinit();
}

// ---- Thread Entry Points
internalf void base_main_thread_entry_point(s32 argc, char* argv[]) {
	base_main_thread_init(argc, argv);
	entry_point_udf();
	base_main_thread_deinit();
}

internalf void base_supplement_thread_entry_point(ThreadEntryPoint_FunctionType* func, void* params) {
	base_thread_context_alloc();
	func(params);
	base_thread_context_dealloc();
}

internalf void base_async_thread_entry_point(void* params) {
    
	LaneContext* lane_context = (LaneContext*)params;
	u32 join;
    
	base_thread_context_set_lane(*lane_context);
    
	join = 0;
	for(;;) {
		
		// -- Respect tick interval if there are no requested ticks
		if(base_lane_get_thread_index() == 0) {
			if(atomic_u32_eval(&async_state.requested_tick_n) == 0) {
				mutex_lock(async_state.mutex);
				condvar_timed_wait_mutex(async_state.condvar, async_state.mutex, 1000000000);
				mutex_unlock(async_state.mutex);
			}
		}
		base_lane_barrier_wait();
		// -- Respect tick interval if there are no requested ticks
        
		// -- Async Procedures
		base_async_thread_entry_point_udf(params);
		// -- Async Procedures
        
		// -- Check for exit call from main thread
		base_lane_barrier_wait();
		if(lane_context->thread_index == 0) {
			join = atomic_u32_eval(&async_state.exit);
		}
		base_lane_barrier_wait_on_data(&join, sizeof(b8), 0);
		if(join) {
			break;
		}
		// -- Check for exit call from main thread
	}
}
// ---- Thread Entry Points

// ---- Command Line Arguments
internalf void cmd_line_args_init(s32 argc, s8* argv[]) {
	
	// -- Allocate hash table
	cmd_line_args.arena = arena_allocate_default();
	cmd_line_args.cap = 128;
	cmd_line_args.hashmap = hashmap_alloc(&cmd_line_args.arena->allocator, &cmd_line_args.cap);
	// -- Allocate hash table
    
    
	// -- Insert valid flags as entries into the hash table
	cmd_line_args_add_flags_from_params(base_cmd_line_args_params, array_cap(base_cmd_line_args_params));
	// FILLER: put here cmd line argument flags for other libraries, e.g: cmd_line_args_add_flags_from_params(gfx_cmd_line_args_params, array_cap(gfx_cmd_line_args_params));
	cmd_line_args_add_flags_from_params(CUSTOM_CMD_LINE_ARGS_PARAMS, array_cap(CUSTOM_CMD_LINE_ARGS_PARAMS));
	// -- Insert valid flags as entries into the hash table
    
    
	// -- Fill flags with passed in values
	{
		u32 arg_i;
		s8* arg_cstr;
		String8 arg_str;
		CmdLineArg* curr_arg;
		CmdLineArg* flag_arg = 0;
		u32 flag_arg_value_i;
		for(arg_i = 0; arg_i < (u32)argc; arg_i += 1) {
			arg_cstr = argv[arg_i];
			arg_str = str8_cstr((u8*)arg_cstr);
			curr_arg = cmd_line_args_get_arg_(arg_str, 0);
			if(curr_arg) {

				if(flag_arg) {
					if(flag_arg_value_i == flag_arg->values_cap) {
						flag_arg->set = 1;
					} else {
						raise();  // DEBUG: if there is a flag_arg all the values up to cap have to be passed in	
					}
				}
				
				flag_arg = curr_arg;
				flag_arg_value_i = 0;
				
				if(flag_arg->values_cap == 0) {  // NOTE: needed if the last arg has no values
					flag_arg->set = 1;
				}
			} else if(flag_arg && (flag_arg_value_i < flag_arg->values_cap)) {
				flag_arg->values[flag_arg_value_i] = arg_str;
				flag_arg_value_i += 1;
			}
		}
	}
	// -- Fill flags with passed in values
    
}

internalf void cmd_line_args_add_flags_from_params(CmdLineArgParams* args_params, u32 args_params_len) {
	
	CmdLineArg* arg;
	CmdLineArgParams arg_params;
	u32 param_i;
    
	for(param_i = 0; param_i < args_params_len; param_i += 1) {
		arg_params = args_params[param_i];
		arg = (CmdLineArg*)cmd_line_args_get_arg(arg_params.name);
		if(!arg) {
			arg = (CmdLineArg*)allocate(&cmd_line_args.arena->allocator, sizeof(CmdLineArg));
			
			// -- Fill arg
			arg->name = arg_params.name;
			arg->values_cap = arg_params.values_cap;
			arg->values = (String8*)allocate(&cmd_line_args.arena->allocator, sizeof(String8) * arg->values_cap);
			arg->set = 0;
			// -- Fill arg
            
			// -- Push arg to hash table
			arg->entry.key = cstr8_hash(arg_params.name.str, arg_params.name.len);
			hashmap_push_entry(cmd_line_args.hashmap, cmd_line_args.cap, &arg->entry);
			// -- Push arg to hash table
		} else {
			raise();  // repeated command line argument
		}
	}
}

internalf CmdLineArg* cmd_line_args_get_arg_(String8 name, b8 has_to_be_set) {
	CmdLineArg* result = (CmdLineArg*)0;
	HashMapEntry* entry;
	entry = hashmap_get_entry(cmd_line_args.hashmap, cmd_line_args.cap, cstr8_hash(name.str, name.len));
	if(entry) {
		result = container_of(entry, CmdLineArg, entry);
		if(has_to_be_set && !result->set) {
			result = (CmdLineArg*)0;
		}
	}
	return result;
}

internalf CmdLineArg* cmd_line_args_get_arg(String8 name) {
	return cmd_line_args_get_arg_(name, 1);
}

internalf b8 cmd_line_arg_is_set(String8 name) {
	return (b8)(cmd_line_args_get_arg(name) != (CmdLineArg*)0);
}

internalf String8 cmd_line_args_get_value(String8 name, u32 index) {
	
	String8 result = str8_nil();
	
	CmdLineArg* arg = cmd_line_args_get_arg(name);
	if((arg) && (index < arg->values_cap)) {
		result = arg->values[index];
	}
	
	return result;
}
// ---- Command Line Arguments

// -- Functions
