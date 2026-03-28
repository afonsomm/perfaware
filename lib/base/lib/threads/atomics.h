

/**

----- Brief -----
Compiler intrinsics for atomic operations.

These should be used when the frequency of concurrent access to a variable is high in order to have good performance. Otherwise, one should use mutexes.
(See: https://dependablec.org/#Concurrency "Mutex vs Atomics" section).
----- Brief -----

*/


#ifndef BASE_THREADS_ATOMICS_H
# define BASE_THREADS_ATOMICS_H


// -- Preprocessor Defines
#if COMPILER_MSVC

#define atomic_u32_eval(ptr) *((volatile u32*)(ptr))
#define atomic_u64_eval(ptr) *((volatile u64*)(ptr))

#define atomic_u32_add_eval(ptr, val) _InterlockedAdd((LONG*)(ptr), (val))
#define atomic_u64_add_eval(ptr, val) _InterlockedAdd64((__int64*)(ptr), (val))

#define atomic_u32_dec_eval(ptr) _InterlockedDecrement((LONG*)(ptr))
#define atomic_u64_dec_eval(ptr) _InterlockedDecrement64((__int64*)(ptr))

#define atomic_u32_inc_eval(ptr) _InterlockedIncrement((LONG*)(ptr))
#define atomic_u64_inc_eval(ptr) _InterlockedIncrement64((__int64*)(ptr))

#define atomic_u32_eval_assign(ptr, val) _InterlockedExchange((LONG*)(ptr), (val))
#define atomic_u64_eval_assign(ptr, val) _InterlockedExchange64((__int64*)(ptr), (val))

#define atomic_u32_eval_cond_assign(ptr, val, comp) _InterlockedCompareExchange((LONG*)(ptr), (val), (comp))
#define atomic_u64_eval_cond_assign(ptr, val, comp) _InterlockedCompareExchange64((__int64*)(ptr), (val), (comp))

#elif COMPILER_CLANG || COMPILER_GCC

#define atomic_u64_eval(ptr)  __atomic_load_n((ptr), __ATOMIC_SEQ_CST)
#define atomic_u32_eval(ptr)  __atomic_load_n((ptr), __ATOMIC_SEQ_CST)

#define atomic_u32_add_eval(ptr, val) __atomic_add_fetch((u32*)(ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_u64_add_eval(ptr, val) __atomic_add_fetch((u64*)(ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_u32_dec_eval(ptr) __atomic_sub_fetch((u32*)(ptr), 1, __ATOMIC_SEQ_CST)
#define atomic_u64_dec_eval(ptr) __atomic_sub_fetch((u64*)(ptr), 1, __ATOMIC_SEQ_CST)

#define atomic_u32_inc_eval(ptr) __atomic_add_fetch((u32*)(ptr), 1, __ATOMIC_SEQ_CST)
#define atomic_u64_inc_eval(ptr) __atomic_add_fetch((u64*)(ptr), 1, __ATOMIC_SEQ_CST)

#define atomic_u64_eval_assign(ptr, val) __atomic_exchange_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_u32_eval_assign(ptr, val) __atomic_exchange_n((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_u32_eval_cond_assign(ptr, val, comp) ({ u32 _comp = (comp); __atomic_compare_exchange_n((ptr), &_comp, (val), 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); _comp; })
#define atomic_u64_eval_cond_assign(ptr, val, comp) ({ u64 _comp = (comp); __atomic_compare_exchange_n((ptr), &_comp, (val), 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); _comp; })

#endif
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
// -- Types


// -- Functions
// -- Functions


#endif  // BASE_THREADS_ATOMICS_H

