

// TODO
#ifndef POOL_H
# define POOL_H


// -- Preprocessor Defines
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
typedef struct Pool Pool;
struct Pool {
	Arena* arena;
	Sll free;
	u64 size;
	u64 cap;
};
// -- Types


// -- Functions

// ---- Pool 
internalf Pool* pool_from_arena(Arena* arena, u64 cap);
internalf void* pool_allocate(Pool* pool, u64 cap_inc);
internalf void  pool_free(Pool* pool, void* chunk);
internalf void  pool_release(Pool* pool);
// ---- Pool

// -- Functions


#endif  // POOL_H
