

/**

----- Brief -----
Arena allocator implementation.

If MEMORY_SAFE is defined, then calls to this allocator are ignored.

As for every allocator, this one adheres to the Allocator interface, which should always be used.
When using any allocator, one should use the "allocate(...)", "reallocate(...)" and "deallocate(...)" functions, 
which get replaced by Fill-C function calls if MEMORY_SAFE is defined.

An arena is a stack of memory blocks, with a position which defines were in the block we are at.
When we allocate memory, we bump up the position using the asked for allocation size, and return the address corresponding to the
position before the bump.
When we deallocate memory, we bump the position down, so we need either an absolute position or a size to decrement the position by.
A memory block is a reserved range of memory addresses, some of which get commited as needed according to the position variable.
An arena can optionally not be chained, in which case there is only one block on the stack, and once the position surpasses the
reserved block size, then the allocation fails. This can be used for dynamic arrays.
Currently, there is no way to dynamically reallocate an unchained arena, if the dynamic array's size surpasses the reserved memory
block size. It is up for the user to verify if an null pointer is returned from the allocation call, allocate another arena with the desired size, 
copy the data to persist onto it, and deallocate the previous arena.

A temporary arena (ArenaTemp) is a checkpoint on a given arena. It saves the current position, to which we can pop to later, deallocating
objects after it. It can be used like a stack of temporary memory, where we start a block of temporary memory, then inside this one,
we start another, each with its corresponding ending. This creates a scope/lifetime for allocations for each temporary block. To end the lifetime of a temporary memory block, we pop the arena to the last position saved, which is in the
ArenaTemp struct. We may not end the lifetime of a temporary memory block if the end of it coincides with the end of lifetime of another
one upper in the stack, and both refer to the same arena.
 
IMPORTANT: If the end of lifetimes from temporary blocks in the same arena do not coincide, we need to be carefull not to allocate memory that belongs to a scope upper in the stack, 
inside a nested scope, otherwise it will be popped at the end of the later, and when we try to access that later, the memory may be corrupted (i.e.: overwritten by other things).
----- Brief -----

*/


#ifndef BASE_ARENA_H
# define BASE_ARENA_H


// -- Preprocessor Defines

// ---- Arena
#define ARENA_HEADER_SIZE 128
#define ARENA_RESERVE_SIZE_DEFAULT megabytes(64)
#define ARENA_COMMIT_SIZE_DEFAULT  kilobytes(64)
// ---- Arena

// -- Preprocessor Defines


// -- Enums/Flags

// ---- Arena
typedef enum ArenaFlag {
	ArenaFlag_NoChain    = (1 << 0),
	ArenaFlag_LargePages = (1 << 1),
} ArenaFlag;
// ---- Arena

// -- Enums/Flags


// -- Types

// ---- Arena
typedef struct Arena Arena;
struct Arena {
	Allocator allocator;
	u64 res_size;
	u64 cmt_size;
	enum32(ArenaFlag) flags;
    
	// -- stack of used and free arenas (headers that are valid only in the original arena)
	Sll curr;
	Sll free;
	// -- stack of used and free arenas (headers that are valid only in the original arena)
    
	u64 cmt;
	u64 res;
    
	u64 base;
	u64 used;
};

typedef struct ArenaParams ArenaParams;
struct ArenaParams {
	Arena* arena;
	u64 res_size;
	u64 cmt_size;
	enum32(ArenaFlag) flags;
};

typedef struct ArenaTemp ArenaTemp;
struct ArenaTemp {
	Arena* arena;
	u64 pos;
};
// ---- Arena

// -- Types


// -- Functions

// ---- Arena
internalf ArenaParams arena_params_default(Arena* arena);
internalf ArenaParams arena_params_default_static(void);

#define arena_allocate_default() arena_allocate(0)
internalf Arena* arena_allocate(ArenaParams* params);
internalf void arena_deallocate(Arena* arena);

internalf u64 arena_pos(Arena* arena);
internalf void arena_clear(Arena* arena);
internalf void arena_pop_to(Arena* arena, u64 pos);
internalf void arena_pop(Arena* arena, u64 size);

internalf void* arena_push_(Arena* arena, u64 size, u64 align, b8 zero);
internalf void* arena_push(Arena* arena, u64 size);
internalf void* arena_push_no_zero(Arena* arena, u64 size);
internalf void* arena_push_aligned(Arena* arena, u64 size, u64 align);
internalf void* arena_push_aligned_no_zero(Arena* arena, u64 size, u64 align);

internalf void* arena_allocate_(Allocator* allocator, u64 size, u64 align, b8 zero);
internalf void* arena_reallocate_(Allocator* allocator, void* ptr, u64 oldsize, u64 newsize, u64 align, b8 zero);
internalf void arena_deallocate_(Allocator* allocator, void* ptr, u64 size);
// ---- Arena

// ---- Temporary Checkpoint
internalf ArenaTemp arena_temp_begin(Arena* arena);
internalf void arena_temp_end(ArenaTemp* temp);
// ---- Temporary Checkpoint

// -- Functions


#endif  // BASE_ARENA_H
