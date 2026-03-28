


internalf NdArray* ndarray_alloc(u64 el_size, u32* shapes, u32 shapes_n, NdArray_DType dtype) {

	NdArray* result;
	ArenaParams params;
	Arena* arena;


	// -- Allocate Header
	params.arena = (Arena*)0;
	params.res_size = kilobytes(4);
	params.cmt_size = kilobytes(4);
	params.flags = ArenaFlag_NoChain;
	arena = arena_allocate(&params);

	result = (NdArray*)allocate(&arena->allocator, sizeof(NdArray));
	result->header_allocator = &arena->allocator;
	// -- Allocate Header

	// TODO calculate total size

	// -- Allocate Data
	params.arena = (Arena*)0;
	params.res_size = align_up_pow2(result->size, megabytes(64));
	params.cmt_size = max(result->size, ARENA_COMMIT_SIZE_DEFAULT);
	params.flags = ArenaFlag_NoChain;
	arena = arena_allocate(&params);
	result->data_allocator = &arena->allocator;
	result->data = allocate(result->data_allocator, result->size);
	// -- Allocate Data

	// TODO ...

	return result;
}

internalf void ndarray_dealloc(NdArray* array) {
	deallocate(array->data_allocator, array->data, array->len * array->el_size);
	arena_deallocate((Arena*)array->header_allocator);
}


internalf void ndarray_view(NdArray* array1, u32* indexes[3], u32 indexes_n) {  // 0 = start; 1 = end; 2 = step

}

internalf NdArray* ndarray_copy(NdArray* array) {

	NdArray* result;
	ArenaParams params;
	Arena* arena;

	// -- Allocate Header
	params.arena = (Arena*)0;
	params.res_size = kilobytes(4);
	params.cmt_size = kilobytes(4);
	params.flags = ArenaFlag_NoChain;
	arena = arena_allocate(&params);

	result = (NdArray*)allocate(&arena->allocator, sizeof(NdArray));
	result->header_allocator = &arena->allocator;
	// -- Allocate Header

	// -- Allocate Data
	params.arena = (Arena*)0;
	params.res_size = align_up_pow2(array->size, megabytes(64));
	params.cmt_size = max(array->size, ARENA_COMMIT_SIZE_DEFAULT);
	params.flags = ArenaFlag_NoChain;
	arena = arena_allocate(&params);
	result->data_allocator = &arena->allocator;
	result->data = allocate(result->data_allocator, array->size);
	// -- Allocate Data

	// TODO ...


	return result;
}


internalf void ndarray_reshape(NdArray* array, s32* new_shapes, u32 shapes_n) {

}

internalf void ndarray_transpose(NdArray* array) {

}

internalf void ndarray_cast(NdArray* array, NdArray_DType new_dtype) {

}


internalf void ndarray_sum(NdArray* array) {

}

internalf void ndarray_mean(NdArray* array) {

}


internalf void ndarray_add(NdArray* array1, NdArray* array2, void* (*add_cb)(void*, void*)) {

}

internalf void ndarray_sub(NdArray* array1, NdArray* array2, void* (*sub_cb)(void*, void*)) {

}

internalf void ndarray_mul(NdArray* array1, NdArray* array2, void* (*mul_cb)(void*, void*)) {

}

internalf void ndarray_div(NdArray* array1, NdArray* array2, void* (*div_cb)(void*, void*)) {

}

internalf void ndarray_dot(NdArray* array1, NdArray* array2, void* (*add_cb)(void*, void*), void* (*mul_cb)(void*, void*)) {

}


internalf void ndarray_apply_along_axis(NdArray* array1, u32 axis, void* (*cb)(void*, void*)) {

}



/*
internalf void* ndarray_init(Allocator* header_allocator, Allocator* data_allocator, NdArray* header, u64 el_size, u32* shapes, u32 shapes_n, NdArray_DType dtype) {

	void* result = 0;
	u32* shapes_copy;
	u64* strides;

	u64 len;
	u32 shape_i;

	shapes_copy = allocate(header_allocator, sizeof(u32) * shapes_n);
	strides = allocate(header_allocator, sizeof(u64) * shapes_n);

	mem_cpy(shapes_copy, shapes, sizeof(u32) * shapes_n);

	len = shapes[shapes_n - 1];
	strides[shapes_n - 1] = el_size;
	for(shape_i = shapes_n - 2; shape_i >= 0; shape_i -= 1) {
		len *= shapes[shape_i];
		strides[shape_i] = strides[shape_i + 1] * shapes[shape_i + 1];
	}

	header->shapes = shapes_copy;
	header->strides = strides;
	header->len= len;
	header->size = len * el_size;
	header->el_size = el_size;
	header->shapes_n = shapes_n;

	result = allocate(data_allocator, header->size);
	return result;
}

internalf NdArray* ndarray_copy_header(Allocator* allocator, NdArray* header) {
	
	NdArray* result = allocate(allocator, sizeof(NdArray));
	mem_cpy(result->shapes, header->shapes, sizeof(u32) * header->shapes_n);
	result->shapes_n = header->shapes_n;
	mem_cpy(result->strides, header->strides, sizeof(u64) * header->shapes_n);
	result->el_size = header->el_size;
	result->size = header->size;
	result->len = header->len;
	result->dtype = header->dtype;
}

internalf void* ndarray_copy_data(Allocator* allocator, NdArray* header, void* data) {

	void* result = allocate(allocator, header->size);
	mem_cpy(result, data, header->size);
}


internalf void ndarray_reshape(Allocator* header_allocator, NdArray* header, void** arr, s32* new_shapes, u32 shapes_n) {
	
	// new_shapes is s32 because if -1, then the ndarray is flattened along that dimension, according to the length of the array and the remaining dimensions.
	// only one -1 is allowed to infer the new dimension.
	// 	if header->shapes == (4, 3, 2):
	// 		new_shapes == (4, -1), then header->shapes = (4, 6)
	// 		new_shapes == (-1,), then header->shapes = (24,)
	// 		new_shapes == (4, -1, 2), then header->shapes = (4, 3, 2)
	// 		new_shapes == (-1, 6), then header->shapes = (4, 6)
	// 		new_shapes == (-1, 2), then header->shapes = (12, 2)
	// 	if header->shapes == (4, 3, 2, 3, 4, 5, 10):
	// 		new_shapes == (4, -1, 3, 4, 5, 10), then header->shapes = (4, 6, 3, 4, 5, 10)

	


	u32 temp_len = 0;
	u32 unknown_dim = 0;
	u32 shape_i;
	for(shape_i = 0; shape_i < shapes_n; shape_i += 1) {
		if(new_shapes[shape_i] != -1) {
			temp_len *= new_shapes[shape_i];
		} else {
			unknown_dim = shape_i + 1;
		}
	}
	
	if((temp_len <= header->len) && ((header->len % temp_len) == 0)) {
		if(unknown_dim) {
			new_shapes[unknown_dim - 1] = header->len / temp_len;
		}

		// Compute new strides


		// Reallocate strides and shapes and copy the memory onto them


	} else {
		raise();
	}

}
	*/
