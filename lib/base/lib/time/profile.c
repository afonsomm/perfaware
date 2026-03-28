

// -- Global State
global Profile_State* profile_state = 0;
// -- Global State


// -- Functions
#ifdef PROFILE
internalf void base_profile_init(void) {

	ArenaParams params;
	Arena* arena;
	u64 buffer_size = sizeof(u8) * 1024 * 1024;
	// -- Allocate
	params.arena = 0;
	params.res_size = buffer_size + sizeof(Profile_State) + ARENA_HEADER_SIZE;
	params.cmt_size = params.res_size;
	params.flags |= ArenaFlag_NoChain;
	arena = arena_allocate(&params);
	profile_state = allocate(&arena->allocator, sizeof(Profile_State));
	profile_state->arena = arena;
	// -- Allocate

	if(spall_init_file("profile.spall", 1, &profile_state->ctx)) {
		profile_state->buffer.data = allocate(&arena->allocator, buffer_size);
		profile_state->buffer.length = buffer_size;

		if(spall_buffer_init(&profile_state->ctx, &profile_state->buffer)) {
		} else {
			debug_print_cstr8_lit("Failed to init spall buffer?\n");
			raise();
		}
	} else {
		debug_print_cstr8_lit("Failed to setup spall?\n");
		raise();
	}

}

internalf void base_profile_deinit(void) {
	spall_buffer_quit(&profile_state->ctx, &profile_state->buffer);
	spall_quit(&profile_state->ctx);
	arena_deallocate(profile_state->arena);
}
#endif
// -- Functions
