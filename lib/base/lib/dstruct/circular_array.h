

#ifndef BASE_DSTRUCT_CIRCULAR_ARRAY_H
# define BASE_DSTRUCT_CIRCULAR_ARRAY_H


// -- Functions
internalf ArrayQueue* circular_array_alloc(u64 slot_size, u32 cap);
internalf ArrayQueue* circular_array_alloc_custom(Allocator* allocator, u64 slot_size, u32 cap);
internalf void circular_array_dealloc(ArrayQueue* array);
internalf void circular_array_dealloc_custom(ArrayQueue* array);

internalf void circular_array_write(ArrayQueue* array, void* el);
internalf b8 circular_array_bounded_write(ArrayQueue* array, void* el);
internalf void circular_array_read(ArrayQueue* array, void* out, void* nil);
// -- Functions


#endif  // BASE_DSTRUCT_CIRCULAR_ARRAY_H
