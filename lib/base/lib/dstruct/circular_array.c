


internalf ArrayQueue* circular_array_alloc(u64 slot_size, u32 cap) {

	ArrayQueue* result;
	cap = round_up_pow2_u32(cap);
	result = array_queue_alloc(slot_size, cap);
	return result;
}

internalf ArrayQueue* circular_array_alloc_custom(Allocator* allocator, u64 slot_size, u32 cap) {

	ArrayQueue* result;
	cap = round_up_pow2_u32(cap);
	result = array_queue_alloc_custom(allocator, slot_size, cap);
	return result;
}

internalf void circular_array_dealloc(ArrayQueue* array) {
	array_queue_dealloc(array);
}

internalf void circular_array_dealloc_custom(ArrayQueue* array) {
	array_queue_dealloc_custom(array);
}


internalf void circular_array_write(ArrayQueue* array, void* el) {
	u32 slot_n = (u32)mod_pow2(array->last, array->array.cap);	
	mem_cpy((u8*)array->array.data + (slot_n * array->array.slot_size), el, array->array.slot_size);
	array->last += 1;
}

internalf b8 circular_array_bounded_write(ArrayQueue* array, void* el) {

	b8 result = 0;

	if((array->last - array->first) < array->array.cap) {
		circular_array_write(array, el);
		array->last += 1;
		result = 1;
	} else {
		debug_print_cstr8_lit("circular_array_bounded_write: Array is full.");
	}

	return result;
}

internalf void circular_array_read(ArrayQueue* array, void* out, void* nil) {

	if(array->last != array->first) {
		u32 slot_n = (u32)mod_pow2(array->first, array->array.cap);		
		mem_cpy(out, (u8*)array->array.data + (slot_n * array->array.slot_size), array->array.slot_size);
		array->first += 1;
	} else {
		if(nil) {
			mem_cpy(out, nil, array->array.slot_size);
		}
		debug_print_cstr8_lit("circular_array_read: Array is empty.");
	}
}
