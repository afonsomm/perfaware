

/**

----- Brief -----
Absracts functions to load shared libraries and load procedures from them, from the OS layer.
----- Brief -----

*/

#ifndef BASE_SHARED_OBJECTS_CORE_H
# define BASE_SHARED_OBJECTS_CORE_H


// -- Preprocessor Defines
#if OS_WINDOWS
# define SHARED_EXPORT __declspec(dllexport)
#elif OS_LINUX
# define SHARED_EXPORT
#else
# error "base_shared_libs.h: need to define the followings for this operating system: SHARED_EXPORT."
#endif

# define SHARED_FUNCTION C_LINKAGE SHARED_EXPORT
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
typedef union SharedLibrary SharedLibrary;
union SharedLibrary {
	Handle handle;
};
typedef void SharedLibraryProcedure(void);
// -- Types


// -- Functions
internalf SharedLibrary shared_library_load(String8 path);
internalf SharedLibraryProcedure* shared_library_load_procedure(SharedLibrary lib, const s8* name);
internalf void shared_library_unload(SharedLibrary lib);
// -- Functions


#endif  // BASE_SHARED_OBJECTS_CORE_H
