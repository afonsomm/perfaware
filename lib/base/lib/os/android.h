

#ifndef BASE_OS_ANDROID_H
# define BASE_OS_ANDROID_H


// -- Includes
#include "../../../externals/inc_begin.h"
#include <sys/mman.h>
#include "../../../externals/inc_end.h"
// -- Includes


// -- Preprocessor Defines
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
// -- Types


// -- Functions
internalf String8 android_str8_from_jstring(Allocator* allocator, JNIEnv* jenv, jstring jstring_obj);
// -- Functions


#endif  // BASE_OS_ANDROID_H
