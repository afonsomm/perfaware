

// TODO
#ifndef BUCKETED_ARENA_H
# define BUCKETED_ARENA_H


// -- Preprocessor Defines
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
typedef struct BucketedArena BucketedArena;
struct BucketedArena {
	Arena* arena;
	Sll* allocs;
	Sll* frees;
	u64* sizes;
	u64 len;
};
// -- Types


// -- Functions
// -- Functions


#endif  // BUCKETED_ARENA_H
