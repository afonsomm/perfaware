

#ifndef BASE_DSTRUCT_HASHMAP_H
# define BASE_DSTRUCT_HASHMAP_H


// -- Preprocessor Defines
#define hashmap_slot_foreach(h, T, attr, itr) for(itr = container_of((h)->next, T, attr.links); &itr->attr.links != (h); itr = container_of(itr->attr.links.next, T, attr.links))
#define hashmap_foreach(hashmap, cap, slot_itr, entry_itr) \
for(slot_itr = 0; slot_itr < (cap); slot_itr++) \
	hashmap_slot_foreach(&(hashmap)[slot_itr].entries, entry_itr)
// -- Preprocessor Defines


// -- Types
typedef struct HashMapEntry HashMapEntry;
struct HashMapEntry {
	Dll links;
	u64 key;
};

typedef struct HashMapSlot HashMapSlot;  // must have length as a power of 2
struct HashMapSlot {
	Dll entries;
};
// -- Types


// -- Functions
internalf HashMapSlot* hashmap_alloc(Allocator* allocator, u32* cap);

internalf b8 hashmap_push_entry(HashMapSlot* hashmap, u32 cap, HashMapEntry* entry);
internalf b8 hashmap_slot_push_entry(HashMapSlot* slot, HashMapEntry* entry);

internalf HashMapEntry* hashmap_pop_entry(HashMapSlot* hashmap, u32 cap, u64 key);
internalf HashMapEntry* hashmap_slot_pop_entry(HashMapSlot* slot, u64 key);

internalf HashMapSlot*  hashmap_get_slot(HashMapSlot* hashmap, u32 cap, u64 key);
internalf HashMapEntry* hashmap_get_entry(HashMapSlot* hashmap, u32 cap, u64 key);
internalf HashMapEntry* hashmap_slot_get_entry(HashMapSlot* slot, u64 key);

internalf void hashmap_entry_remove(HashMapEntry* entry);
// -- Functions


#endif  // BASE_DSTRUCT_HASHMAP_H
