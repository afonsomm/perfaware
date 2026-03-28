

// -- Global State
// -- Global State


// -- Functions

// ---- Arena
internalf ArenaParams arena_params_default(Arena* arena) {
	// Create an ArenaParams object with the default values for arena creation. Specify a reference to the base Arena object when creating a new block for a chained arena.
    
	ArenaParams result;
#ifdef MEMORY_SAFE
#else
	result.arena = arena;
	result.res_size = ARENA_RESERVE_SIZE_DEFAULT;
	result.cmt_size = ARENA_COMMIT_SIZE_DEFAULT;
	result.flags = 0;
#endif  // MEMORY_SAFE
	return result;
}

internalf ArenaParams arena_params_default_static(void) {
	ArenaParams result;

#ifdef MEMORY_SAFE
#else
	result = arena_params_default(0);
	result.flags = ArenaFlag_NoChain;
#endif
	return result;
}

internalf Arena* arena_allocate(ArenaParams* params) {
    // Allocate a new Arena object according to the ArenaParams object reference passed in. Return a reference to the allocated object, whose header is in the beginning
    // of the allocated memory block.
    
	Arena* result;
    
#ifdef MEMORY_SAFE
    result = (Arena*)0;
#else
	u64 res_size;
	u64 cmt_size;
	enum32(ArenaFlag) flags;
	Arena* base_arena;
	
    if(!params) {  // -- If params is a null pointer, then set parameters to be the default values
		res_size = ARENA_RESERVE_SIZE_DEFAULT;
		cmt_size = ARENA_COMMIT_SIZE_DEFAULT;
		flags = 0;
		base_arena = 0;
	} else {
		res_size = params->res_size;
		cmt_size = params->cmt_size;
		flags = params->flags;
		base_arena = params->arena;
		assert(res_size && cmt_size);
	}
    
	if(flags & ArenaFlag_LargePages) {  // -- align with memory page size
		res_size = align_up_pow2(res_size, mem_get_large_page_size());
		cmt_size = align_up_pow2(cmt_size, mem_get_large_page_size());
	} else {
		res_size = align_up_pow2(res_size, mem_get_page_size());
		cmt_size = align_up_pow2(cmt_size, mem_get_page_size());	
	}
    
	result = (Arena*)mem_reserve(res_size);
	if(result) {
		mem_commit(result, cmt_size);
        
		result->allocator.allocate = arena_allocate_;
		result->allocator.reallocate = arena_reallocate_;
		result->allocator.deallocate = arena_deallocate_;
        
		result->res_size = res_size;
		result->cmt_size = cmt_size;
		result->flags = flags;
        
		if(base_arena) {  // -- If this is an additional block in the chain, add it to the stack
			sll_stack_push(&base_arena->curr, &result->curr);
			result->free.next = (Sll*)0;
		} else { // -- Else initialize the arena stacks
			sll_stack_init(&result->curr);
			sll_stack_init(&result->free);
		}
        
		result->res = res_size;
		result->cmt = cmt_size;
        
		result->base = 0;
		result->used = ARENA_HEADER_SIZE;
        
		asan_poison_memory_region((u8*)result + result->used, result->cmt_size - result->used);
	} else {
		debug_print_cstr8_lit("arena_allocate: Could not allocate a new arena.");
		raise();
	}
#endif  // MEMORY_SAFE
    
	return result;
}

internalf void arena_deallocate(Arena* arena) {
	// Deallocate an arena (chained or not).
    
#ifdef MEMORY_SAFE
#else
    if(!(arena->flags & ArenaFlag_NoChain)) {
		Arena* curr;
		Arena* next;
		for(curr = container_of(arena->free.next, Arena, free); &curr->free != &arena->free; curr = next) {
			next = container_of(curr->free.next, Arena, free);
			mem_release(curr, curr->res);
		}
		for(curr = container_of(arena->curr.next, Arena, curr); &curr->curr != &arena->curr; curr = next) {
			next = container_of(curr->curr.next, Arena, curr);
			mem_release(curr, curr->res);
		}
	}
	mem_release(arena, arena->res);
#endif  // MEMORY_SAFE
}

internalf u64 arena_pos(Arena* arena) {
	// Get current absolute position in the arena.
    
#ifdef MEMORY_SAFE
    return 0;
#else
    Arena* curr = container_of(arena->curr.next, Arena, curr);
	return curr->base + curr->used;
#endif  // MEMORY_SAFE
}

internalf void arena_clear(Arena* arena) {
    // Decrement the position variable for the arena to its start, so the arena is cleared.
    
#ifdef MEMORY_SAFE
#else
	arena_pop_to(arena, 0);
#endif  // MEMORY_SAFE
}

internalf void arena_pop_to(Arena* arena, u64 pos) {
	// Decrement the position variable for the arena to the passed in position.
    
#ifdef MEMORY_SAFE
#else
	Arena* curr = container_of(arena->curr.next, Arena, curr);
	u64 prev_used;
	
	if(!(arena->flags & ArenaFlag_NoChain)) {
		for(; (curr != arena) && (pos < (curr->base + ARENA_HEADER_SIZE)); curr = container_of(arena->curr.next, Arena, curr)) {
			prev_used = curr->used;
            
			curr->used = ARENA_HEADER_SIZE;
            
			sll_stack_push(&arena->free, &curr->free);
			sll_stack_pop(&arena->curr);
            
			asan_poison_memory_region((u8*)curr + curr->used, min(align_up_pow2(prev_used, arena->cmt_size), arena->res_size) - curr->used);
		}
	}
    
	prev_used = curr->used;
	curr->used = max(pos - curr->base, ARENA_HEADER_SIZE);
	asan_poison_memory_region((u8*)curr + curr->used, min(align_up_pow2(prev_used, arena->cmt_size), arena->res_size) - curr->used);
#endif  // MEMORY_SAFE
}

internalf void arena_pop(Arena* arena, u64 size) {
	// Decrement the position variable for the arena in size bytes.
    
#ifdef MEMORY_SAFE
#else
    arena_pop_to(arena, arena_pos(arena) - size);
#endif  // MEMORY_SAFE
}

internalf void* arena_push_(Arena* arena, u64 size, u64 align, b8 zero) {
    // Increment the position variable for the arena in size bytes. The increment may be more according to the memory alignment passed in.
    // The returned reference is to the aligned position pre-increment. The memory space between the previous and new positions is zeroed if zero is set to 1.
    
	void* result = 0;
    
#ifdef MEMORY_SAFE
#else
	Arena* curr = container_of(arena->curr.next, Arena, curr);
	u64 pre_used = align_up_pow2(curr->used, align);
	u64 post_used = pre_used + size;
    
	if(!(arena->flags & ArenaFlag_NoChain)) {
		
        if(post_used > curr->res) {  // current memory block does not have space for the allocation
			Arena* new_arena;
			Arena* prev;
			
            // -- Search for a free arena in the chain which is large enough
            new_arena = container_of(arena->free.next, Arena, free);
			for(; &new_arena->free != &arena->free; new_arena = container_of(new_arena->free.next, Arena, free)) {
				if(size <= (new_arena->res - ARENA_HEADER_SIZE)) {
					if(prev) {
						prev->free.next = new_arena->free.next;
					} else {
						sll_stack_pop(&arena->free);
					}
					break;
				}
                
				prev = new_arena;
			}
            // -- Search for a free arena in the chain which is large enough
            
            
			if(new_arena == arena) {  // No free arena large enough for the allocation
				ArenaParams params;
				u64 res_size = curr->res_size;
				u64 cmt_size = curr->cmt_size;
				if(size + ARENA_HEADER_SIZE > res_size) {
					res_size = align_up_pow2(size + ARENA_HEADER_SIZE, align);
					cmt_size = align_up_pow2(size + ARENA_HEADER_SIZE, align);
				}
                
				params.arena = arena;
				params.res_size = res_size;
				params.cmt_size = cmt_size;
				params.flags = arena->flags;
				new_arena = arena_allocate(&params);
			} else {
				new_arena->used = ARENA_HEADER_SIZE;
				asan_unpoison_memory_region(new_arena, ARENA_HEADER_SIZE);
			}
            
            
			if(new_arena && new_arena != arena) {
				new_arena->base = curr->base + curr->res;
				curr = new_arena;
				pre_used = curr->used;
				post_used = pre_used + size;
			} else {
				debug_print_cstr8_lit("Could not reserve a new block and no free arena available.");
				raise();
				goto end;
			}
		}
	} else {
		if(post_used > curr->res) {
			debug_print_cstr8_lit("Fixed size arena is out of memory.");
			raise();
			goto end;
		}
	}
    
	if(post_used > curr->cmt) {  // Need to commit memory
		u8* commit_ptr = (u8*)curr + curr->cmt;
		
        // -- commit size cannot surpass reserved size and has to be enough for the requested allocation
        u64 commit_size = min(curr->cmt_size, curr->res - curr->cmt);
		commit_size = max(commit_size, post_used - curr->cmt);
		curr->cmt += commit_size;
        // -- commit size cannot surpass reserved size and has to be enough for the requested allocation
        
        mem_commit(commit_ptr, commit_size);
		asan_poison_memory_region(commit_ptr, commit_size);
	}
    
	if(post_used <= curr->cmt) {
		result = (u8*)curr + pre_used;
		curr->used = post_used;
		asan_unpoison_memory_region(result, size);
        
		if(zero) {
			mem_zero(result, size);
		}
	} else {
		debug_print_cstr8_lit("Could not commit enough memory.");
		raise();
		goto end;
	}
    end:
#endif  // MEMORY_SAFE
    
    return result;
}

internalf void* arena_push(Arena* arena, u64 size) {
	// Increment the position of the arena in size bytes and return the aligned position pre-increment to 16 bytes.
    // The memory between the previous and new positions is zeroed.
    
#ifdef MEMORY_SAFE
    return (void*)0;
#else
    return arena_push_(arena, size, 16, 1);
#endif  // MEMORY_SAFE
}

internalf void* arena_push_no_zero(Arena* arena, u64 size) {
	// Same as arena_push(), but the memory is not zeroed.
    
#ifdef MEMORY_SAFE
    return (void*)0;
#else
    return arena_push_(arena, size, 16, 0);
#endif  // MEMORY_SAFE
}

internalf void* arena_push_aligned(Arena* arena, u64 size, u64 align) {
    // Same as arena_push(), but the returned position is aligned according to align, respecting the minimum value of 16.
    
#ifdef MEMORY_SAFE
    return (void*)0;
#else
	return arena_push_(arena, size, max(16, align), 1);
#endif  // MEMORY_SAFE
}

internalf void* arena_push_aligned_no_zero(Arena* arena, u64 size, u64 align) {
    // Same as arena_push_aligned(), but the memory is not zeroed.
	
#ifdef MEMORY_SAFE
    return (void*)0;
#else
    return arena_push_(arena, size, max(16, align), 0);
#endif  // MEMORY_SAFE
}

internalf void* arena_allocate_(Allocator* allocator, u64 size, u64 align, b8 zero) {
	// Allocation function to adhrere to the Allocator interface.
    
#ifdef MEMORY_SAFE
    return (void*)0;
#else
    return arena_push_((Arena*)allocator, size, align, zero);
#endif  // MEMORY_SAFE
}

internalf void* arena_reallocate_(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align, b8 zero) {
	// Reallocation function to adhrere to the Allocator interface.
    
#ifdef MEMORY_SAFE
#else
	Arena* arena = (Arena*)allocator;
	Arena* curr = container_of(arena->curr.next, Arena, curr);
	
	if(newsize != oldsize) {
		void* new_ptr = 0;
		void* end_old_alloc = (u8*)ptr + oldsize;
		void* end_arena = (u8*)curr + curr->base + curr->used;
        
		if(end_old_alloc == end_arena) {  // object to be reallocated is at the end of the arena
			if(newsize < oldsize) {
				arena_pop(arena, oldsize - newsize);
			} else {
				// -- Save data to temporary storage
				ArenaTemp scratch = scratch_begin(&arena, 1);
				void* temp_data = allocate(&scratch.arena->allocator, oldsize);
				mem_cpy(temp_data, ptr, oldsize);
				// -- Save data to temporary storage
                
				// -- reallocate
				arena_pop(arena, oldsize);
				new_ptr = arena_push_(arena, newsize, align, zero);
				// -- reallocate
                
				// -- copy data in temporary storage to new_ptr and pop temporary data
				mem_cpy(new_ptr, temp_data, oldsize);
				scratch_end(&scratch);
				// -- copy data in temporary storage to new_ptr and pop temporary data
			}
		} else {  // object is in the middle of the arena, so old block if invalidated
			new_ptr = arena_push_(arena, newsize, align, zero);
		}
        
		if(new_ptr && (new_ptr != ptr)) {
			mem_cpy(new_ptr, ptr, oldsize);
			ptr = new_ptr;
		}
	}
#endif  // MEMORY_SAFE
    
	return ptr;
}

internalf void arena_deallocate_(Allocator* allocator, void* ptr, u64 size) {
	// Deallocation function to adhrere to the Allocator interface.
    
#ifdef MEMORY_SAFE
#else
    arena_pop((Arena*)allocator, size);
#endif  // MEMORY_SAFE
}
// ---- Arena

// ---- Temporary Checkpoint
internalf ArenaTemp arena_temp_begin(Arena* arena) {
	// Start temporary checkpoint on the given arena.
    
    ArenaTemp result;
#ifdef MEMORY_SAFE
		result.arena = (Arena*)0;
		result.pos = 0;
#else
	result.arena = arena;
	result.pos = arena_pos(arena);
#endif  // MEMORY_SAFE
	return result;
}

internalf void arena_temp_end(ArenaTemp* temp) {
	// Decrement arena position to the temprary checkpoint.
	
#ifdef MEMORY_SAFE
#else
    arena_pop_to(temp->arena, temp->pos);
#endif  // MEMORY_SAFE
}
// ---- Temporary Checkpoint

// -- Functions
