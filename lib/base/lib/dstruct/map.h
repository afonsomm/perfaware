

// TODO
#ifndef BASE_DSTRUCT_MAP_H
# define BASE_DSTRUCT_MAP_H


// -- Types
typedef struct MapEntry MapEntry;
struct MapEntry {
	RBTree node;
	u32 index;
};

typedef struct Map Map;
struct Map {
	Array array;
	MapEntry entries;
};
// -- Types


// -- Functions
internalf void* map_init(Allocator* allocator, Map* map, u64 el_size, u32 cap);
internalf b8 map_insert(Allocator* allocator, Map* map, void** arr, void* key, s8(*cmp)(void*,void*));
internalf b8 map_update(Map* map, void* arr, void* key, s8(*cmp)(void*, void*));
internalf b8 map_remove(Map* map, void* arr, void* key, s8(*cmp)(void*,void*));
internalf MapEntry* map_find(Map* map, void* arr, void* key, s8(*cmp)(void*, void*));
// -- Functions


#endif  // BASE_DSTRUCT_MAP_H
