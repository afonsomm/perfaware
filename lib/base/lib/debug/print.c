

// -- Global State
// -- Global State


// -- Functions

// ---- Debug prints
#ifdef DEBUG
internalf void debug_printf(const s8* fmt, ...) {
    
	Allocator allocator = allocator_create_default();
	String8 str;
    
	va_list args;
	va_start(args, fmt);
	str = str8fv(&allocator, fmt, args);
	va_end(args);
    
	debug_print8(str.str, str.len);
    
	deallocate(&allocator, str.str, sizeof(u8) * (str.len + 1));
}

internalf void debug_print_u32(u32 n)  { 
	debug_printf("%d\n", n); 
}

internalf void debug_print_u64(u64 n)  { 
	debug_printf("%llu\n", n); 
}

internalf void debug_print_r32(r32 n)  { 
	debug_printf("%f\n", n); 
}

internalf void debug_print_r64(r64 n)  { 
	debug_printf("%f\n", n); 
}

internalf void debug_print_cstr8(s8* s) {
	debug_printf("%s\n", s);
}

internalf void debug_print_cstr8_u32(s8* s, u32 n) { 
	debug_printf("%s %d\n", s, n); 
}

internalf void debug_print_cstr8_u64(s8* s, u64 n) { 
	debug_printf("%s %llu\n", s, n); 
}

internalf void debug_print_cstr8_r32(s8* s, r32 n) { 
	debug_printf("%s %f\n", s, n); 
}

internalf void debug_print_cstr8_r64(s8* s, r64 n) { 
	debug_printf("%s %f\n", s, n); 
}

internalf void debug_print_cstr8_lit(const s8* s) {
	debug_printf("%s\n", s);
}

internalf void debug_print_cstr8_lit_u32(const s8* s, u32 n) {
	debug_printf("%s %d\n", s, n);
}

internalf void debug_print_cstr8_lit_u64(const s8* s, u64 n) {
	debug_printf("%s %llu\n", s, n);
}

internalf void debug_print_cstr8_lit_r32(const s8* s, r32 n) {
	debug_printf("%s %f\n", s, n);
}

internalf void debug_print_cstr8_lit_r64(const s8* s, r64 n) {
	debug_printf("%s %f\n", s, n);
}

internalf void debug_print_str8(String8 s) {
	debug_printf("%s\n", s.str);
}

internalf void debug_print_str16(String16 s) {
	debug_printf("%s\n", s.str);
}

internalf void debug_print8(u8* str, u64 len) {
    
	if(len) {
# if OS_SDL
		SDL_Log("%s", str);
# elif OS_WINDOWS
		OutputDebugStringA((s8*)str);
# elif OS_LINUX
		openlog("", LOG_DEBUG, LOG_DEBUG);
		syslog(LOG_DEBUG, "%s", (s8*)str);
		closelog();
# elif OS_IOS
# error "debug_print8 not implemented for IOS."
# elif OS_MAC
# error "debug_print8 not implemented for MAC."
# elif OS_ANDROID
# error "debug_print8 not implemented for ANDROID."
# endif
	}
}

internalf void debug_print16(u16* str, u64 len) {
    
	if(len) {
# if OS_SDL
# elif OS_WINDOWS
		OutputDebugStringW((LPCWSTR)str);
# elif OS_LINUX
		openlog("", LOG_DEBUG, LOG_DEBUG);
		syslog(LOG_DEBUG, "debug_print16: this function should only be used on Windows.\n");
		closelog();
# elif OS_IOS
# error "debug_print16 not implemented for IOS."
# elif OS_MAC
# error "debug_print16 not implemented for MAC."
# elif OS_ANDROID
# error "debug_print16 not implemented for ANDROID."
# endif
	}
}
#endif
// ---- Debug prints

// -- Functions
