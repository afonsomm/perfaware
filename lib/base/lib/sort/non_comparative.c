

// -- Functions
internalf void sort_count_u32(u32* arr, u32 len, u32 max) {
    
	ArenaTemp scratch;
	u32* counts_arr;
	u32* sorted_arr;
	u32 i;
	
	
	// -- Get maximum value
	if(max == 0) {
		for(i = 0; i < len; i += 1) {
			if(arr[i] > max) {
				max = arr[i];
			}
		}
	}
	// -- Get maximum value
    
	// -- Allocate counts array and counts as cumsum
	scratch = scratch_begin(0, 0);
	counts_arr = (u32*)allocate(&scratch.arena->allocator, sizeof(u32) * (max + 1));
	for(i = 0; i < len; i += 1) {
		counts_arr[arr[i]] += 1;
	}
    
	for(i = 0; i < max; i += 1) {  // cumsum
		counts_arr[i + 1] += counts_arr[i];
	}
	// -- Allocate counts array and counts as cumsum
    
	// -- Sort array
	sorted_arr = (u32*)allocate(&scratch.arena->allocator, sizeof(u32) * len);
	for(i = len - 1; i != U32_MAX; i -= 1) {
		sorted_arr[counts_arr[arr[i]] - 1] = arr[i];
	}
	// -- Sort array
    
	// -- Copy sorted array onto original
	mem_cpy(arr, sorted_arr, sizeof(u32) * len);
	scratch_end(&scratch);
	// -- Copy sorted array onto original
}

internalf void sort_radix(void* arr, u32 len, u64 slot_size) {
    
	ArenaTemp scratch;
	
	u32 bucket_n;
	u8 bucket[1 << 8];
	u32 bucket_len = 1 << 8;
	
	void* temp_arr;
	u8** arr8;
	
	u32 i;
	s64 j;
	u64 byte_offset;
    
    
	scratch = scratch_begin(0, 0);
	
	bucket_n = (u32)(slot_size / sizeof(u8));
    
	temp_arr = allocate(&scratch.arena->allocator, slot_size * len);
	arr8 = (u8**)allocate(&scratch.arena->allocator, sizeof(u8*) * len);
    
	profile_begin();
    
	for(i = 0; i < bucket_n; i += 1) {
		byte_offset = i * sizeof(u8);
        
		// -- Setup array of pointers
		for(j = 0; j < (s64)len; j += 1) {
			arr8[j] = (u8*)arr + ((u64)j * slot_size) + byte_offset;
		}
		// -- Setup array of pointers
        
		// -- Count occurrences of byte
		mem_zero(bucket, sizeof(bucket));
		for(j = 0; j < (s64)len; j += 1) {
			bucket[*arr8[j]] += 1;
		}
		// -- Count occurrences of byte
		
		// -- Cumsum occurrences
		for(j = 1; j < (s64)bucket_len; j += 1) {
			bucket[j] += bucket[j - 1];
		}
		// -- Cumsum occurrences
        
		// -- Fill temporary array
		for(j = (s64)len - 1; j >= 0; j -= 1) {
			bucket[*arr8[j]] -= 1;
			memcpy((u8*)temp_arr + (bucket[*arr8[j]]) * slot_size, arr8[j] - byte_offset, slot_size);
		}
		// -- Fill temporary array
        
		// -- Order original array according to byte order
		memcpy(arr, temp_arr, slot_size * len);
		// -- Order original array according to byte order
	}
    
	profile_end();
    
	scratch_end(&scratch);
}
// -- Functions
