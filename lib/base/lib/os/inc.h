

#ifndef BASE_OS_INC_H
# define BASE_OS_INC_H


#if OS_SDL
# define EXTERNAL_ENTRY  // entry point is from SDL, so do not compile into the program the OS specific one
# include "./sdl.h"
#endif

#if OS_ANDROID
# include "./android.h"
#elif OS_IOS
# include "./ios.h"
#elif OS_LINUX
# include "./linux.h"
#elif OS_MAC
# include "./mac.h"
#elif OS_WINDOWS
# include "./win32.h"
#else
# error "base/os/inc.h: library not included for this operating system."
#endif


#endif  // BASE_OS_INC_H
