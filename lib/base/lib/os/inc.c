

#if OS_SDL
# include "./sdl.c"
#endif

#if OS_ANDROID
# include "./android.c"
#elif OS_IOS
# include "./ios.c"
#elif OS_LINUX
# include "./linux.c"
#elif OS_MAC
# include "./mac.c"
#elif OS_WINDOWS
# include "./win32.c"
#else
# error "base/os/inc.c: library not included for this operating system."
#endif
