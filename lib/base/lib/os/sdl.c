

// -- Global State
global SDLState sdl_state;
// -- Global State


// -- Functions

// ---- (De)Initializers
internalf void sdl_time_init(void) {
	sdl_state.performance_counter_period = 1000000000.0 / (r64)SDL_GetPerformanceFrequency();
}
// ---- (De)Initializers

// ---- State Getters
internalf Allocator* sdl_get_permanent_allocator(void) {
	return &sdl_state.arena->allocator;
}

internalf u64 sdl_mem_get_large_page_size(void) {
	return (u64)sdl_state.mem_large_page_size;
}

internalf u64 sdl_mem_get_page_size(void) {
	return (u64)sdl_state.mem_large_page_size;
}

internalf u32 sdl_get_logical_processor_n(void) {
	return (u32)sdl_state.logical_processor_n;
}

internalf r64 sdl_get_performance_counter_period(void) {
	return sdl_state.performance_counter_period;
}
// ---- State Getters

// ---- Time
internalf TimeDate sdl_datetime_to_timedate(SDL_DateTime datetime) {

	TimeDate result;
	result.msec  = (u16)(datetime.nanosecond / 1000000);
	result.sec   = (u8)datetime.second;
	result.min   = (u8)datetime.minute;
	result.hour  = (u8)datetime.hour;
	result.day   = (u8)datetime.day;
	result.month = (u8)datetime.month;
	result.year  = (s16)datetime.year;
	return result;
}

internalf SDL_DateTime sdl_datetime_from_timedate(TimeDate* date) {

	SDL_DateTime result;
	result.nanosecond = (int)date->msec * 1000000;
	result.second     = (int)date->sec;
	result.minute     = (int)date->min;
	result.hour       = (int)date->hour;
	result.day        = (int)date->day;
	result.month      = (int)date->month;
	result.year       = (int)date->year;
	return result;
}
// ---- Time

// ---- Objects
internalf SDLObject* sdl_object_alloc(void) {

	SDLObject* result;

	Sll* node = sll_stack_pop(&sdl_state.free_objects);
	if(node) {
		result = container_of(node, SDLObject, node);
	} else {
		result = (SDLObject*)allocate(&sdl_state.arena->allocator, sizeof(SDLObject));
	}

	return result;
}

internalf void sdl_object_dealloc(SDLObject* object) {
	sll_stack_push(&sdl_state.free_objects, &object->node);
}
// ---- Objects

// ---- Entry
internalf void sdl_main_thread_entry_point(int argc, char* argv[]) {

	
	// -- Memory
	sdl_state.mem_page_size = SDL_GetSystemPageSize();
	sdl_state.mem_large_page_size = SDL_GetSystemPageSize();
	sdl_state.allocation_granularity = sdl_state.mem_page_size;
	sdl_state.arena = arena_allocate_default();
	// -- Memory

	// -- Objects
	sll_stack_init(&sdl_state.free_objects);
	// -- Objects

	// -- Optimization Variables
	sdl_state.logical_processor_n = SDL_GetNumLogicalCPUCores();
	sdl_state.simd_alignment = SDL_GetSIMDAlignment();
	// -- Optimization Variables

	// -- General Entry Points
	base_main_thread_entry_point(argc, argv);
	// -- General Entry Points

	// -- Cleanup
	arena_deallocate(sdl_state.arena);
	// -- Cleanup

	SDL_Quit();
}

internalf int sdl_supplement_thread_entry_point(void* data) {
	SDLObject* sdl_object = (SDLObject*)data;
	base_supplement_thread_entry_point(sdl_object->object.thread.func, sdl_object->object.thread.params);
	SDL_CleanupTLS();
	return 1;
}

int main(int argc, char* argv[]) {
	sdl_main_thread_entry_point(argc, argv);
	return 1;
}
// ---- Entry

// -- Functions
