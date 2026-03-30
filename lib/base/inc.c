/**

----- TODO -----
[ ] test on win32 and linux again
[ ] define nil/is_valid functions to object types that need that
[ ] (after perfaware course part 2) instrument code for execution time profiling with spall (alocações de memória com Arena vs malloc/realloc)
[ ] change sizeof(Type) to sizeof(variable) when i already have an object for it.
[ ] verificar porque estou a ter problemas com o Fil-C no uso the pthread_barrier
----- TODO -----

*/


#include "./lib/core.c"
#include "./lib/math/inc.c"
#include "./lib/dstruct/inc.c"
#include "./lib/allocators/inc.c"
#include "./lib/io/inc.c"
#include "./lib/sort/inc.c"
#include "./lib/strings/inc.c"
#include "./lib/threads/inc.c"
#include "./lib/time/inc.c"
#include "./lib/shared_objects/inc.c"
#include "./lib/debug/inc.c"
#include "./lib/entry/inc.c"
#include "./lib/os/inc.c"
