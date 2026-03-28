

internalf HashMapSlot* hashmap_alloc(Allocator* allocator, u32* cap) {
	// out_cap is a power of 2
    
	HashMapSlot* result = (HashMapSlot*)0;
	u32 new_cap = round_up_pow2_u32(*cap);
	result = (HashMapSlot*)allocate(allocator, sizeof(HashMapSlot) * new_cap);
	if(result) {
		HashMapSlot* slot;
		HashMapSlot* end = &result[new_cap];
		for(slot = result; slot != end; slot++) {
			dll_init(&slot->entries);
		}
		*cap = new_cap;
	}
	return result;
}

internalf HashMapEntry* hashmap_slot_get_entry(HashMapSlot* slot, u64 key) {
	
	HashMapEntry* result = (HashMapEntry*)0;
	HashMapEntry* entry = container_of(slot->entries.next, HashMapEntry, links);
	for(; &entry->links != &slot->entries; entry = container_of(entry->links.next, HashMapEntry, links)) {
		if(entry->key == key) {
			result = entry;
			break;
		}
	}
	
	return result;
}

internalf b8 hashmap_slot_push_entry(HashMapSlot* slot, HashMapEntry* entry) {
    
	b8 result = 0;
	if(!hashmap_slot_get_entry(slot, entry->key)) {  // check if already exists
		dll_push_back(&slot->entries, &entry->links);
		result = 1;
	}
	return result;
}

internalf HashMapEntry* hashmap_slot_pop_entry(HashMapSlot* slot, u64 key) {
    
	HashMapEntry* result = hashmap_slot_get_entry(slot, key);
	if(result) {
		dll_remove(&result->links);
	}
	return result;
}

internalf HashMapSlot* hashmap_get_slot(HashMapSlot* hashmap, u32 cap, u64 key) {
	u32 slot_i = (u32)mod_pow2(key, (u64)cap);
	return &hashmap[slot_i];
}

internalf b8 hashmap_push_entry(HashMapSlot* hashmap, u32 cap, HashMapEntry* entry) {
	HashMapSlot* slot = hashmap_get_slot(hashmap, cap, entry->key);
	return hashmap_slot_push_entry(slot, entry);
}

internalf HashMapEntry* hashmap_pop_entry(HashMapSlot* hashmap, u32 cap, u64 key) {
	HashMapSlot* slot = hashmap_get_slot(hashmap, cap, key);
	return hashmap_slot_pop_entry(slot, key);
}

internalf HashMapEntry* hashmap_get_entry(HashMapSlot* hashmap, u32 cap, u64 key) {
	HashMapSlot* slot = hashmap_get_slot(hashmap, cap, key);
	return hashmap_slot_get_entry(slot, key);
}

internalf void hashmap_entry_remove(HashMapEntry* entry) {
	dll_remove(&entry->links);
}
