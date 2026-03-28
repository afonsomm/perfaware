

// TODO
#ifndef BASE_DSTRUCT_NDARRAY_H
# define BASE_DSTRUCT_NDARRAY_H


// -- Preprocessor Defines
// -- Preprocessor Defines


// -- Enums/Flags
typedef enum NdArray_DType {
	NdArray_DType_S8 = 0,
	NdArray_DType_U8,
	NdArray_DType_S16,
	NdArray_DType_U16,
	NdArray_DType_S32,
	NdArray_DType_U32,
	NdArray_DType_S64,
	NdArray_DType_U64,
	NdArray_DType_R32,
	NdArray_DType_R64,
	NdArray_DType_NotNumeric
} NdArray_DType;
// -- Enums/Flags


// -- Types
typedef struct NdArray NdArray;
struct NdArray {
	Allocator* header_allocator;
	Allocator* data_allocator;
	void* data;
	u32* shapes;
	u64* strides;
	u64 el_size;
	u64 size;
	u64 len;
	u32 shapes_n;
	enum32(NdArray_DType) dtype;
	b8 is_view;
};
// -- Types


// -- Functions
internalf NdArray* ndarray_alloc(u64 el_size, u32* shapes, u32 shapes_n, NdArray_DType dtype);
internalf void ndarray_dealloc(NdArray* array);

internalf void ndarray_view(NdArray* array1, u32* indexes[3], u32 indexes_n);  // 0 = start; 1 = end; 2 = step
internalf NdArray* ndarray_copy(NdArray* array);

internalf void ndarray_reshape(NdArray* array, s32* new_shapes, u32 shapes_n);
internalf void ndarray_transpose(NdArray* array);
internalf void ndarray_cast(NdArray* array, NdArray_DType new_dtype);

internalf void ndarray_sum(NdArray* array);
internalf void ndarray_mean(NdArray* array);

internalf void ndarray_add(NdArray* array1, NdArray* array2, void* (*add_cb)(void*, void*));
internalf void ndarray_sub(NdArray* array1, NdArray* array2, void* (*sub_cb)(void*, void*));
internalf void ndarray_mul(NdArray* array1, NdArray* array2, void* (*mul_cb)(void*, void*));
internalf void ndarray_div(NdArray* array1, NdArray* array2, void* (*div_cb)(void*, void*));
internalf void ndarray_dot(NdArray* array1, NdArray* array2, void* (*add_cb)(void*, void*), void* (*mul_cb)(void*, void*));

internalf void ndarray_apply_along_axis(NdArray* array1, u32 axis, void* (*cb)(void*, void*));
// -- Functions


#endif  // BASE_DSTRUCT_NDARRAY_H
