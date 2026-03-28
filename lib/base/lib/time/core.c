

// -- Global State
// -- Global State


// -- Functions
internalf void base_time_init(void) {

#if OS_SDL
	sdl_time_init();
#elif OS_WINDOWS
	win32_time_init();
#elif OS_LINUX
   linux_time_init();
#else
# error "time_init: not implemented for this operating system."
#endif
} 

internalf void base_time_deinit(void) {

#if OS_SDL
#elif OS_WINDOWS
#elif OS_LINUX
#else
# error "time_init: not implemented for this operating system."
#endif
}


internalf const s8* cstr8_from_month(Month month) {
	const s8* result = "(null)";
	switch(month) {
		case Month_Jan: { result = "jan"; } break;
		case Month_Feb: { result = "feb"; } break;
		case Month_Mar: { result = "mar"; } break;
		case Month_Apr: { result = "apr"; } break;
		case Month_May: { result = "may"; } break;
		case Month_Jun: { result = "jun"; } break;
		case Month_Jul: { result = "jul"; } break;
		case Month_Aug: { result = "aug"; } break;
		case Month_Sep: { result = "sep"; } break;
		case Month_Oct: { result = "oct"; } break;
		case Month_Nov: { result = "nov"; } break;
		case Month_Dec: { result = "dec"; } break;
		default: {} break;
	}
	return result;
}

internalf const s8* cstr8_from_day_of_week(DayOfWeek day_of_week) {
	const s8* result = "(null)";
	switch(day_of_week) {
		case DayOfWeek_Sunday: { result = "sunday"; } break;
		case DayOfWeek_Monday: { result = "monday"; } break;
		case DayOfWeek_Tuesday: { result = "tuesday"; } break;
		case DayOfWeek_Wednesday: { result = "wednesday"; } break;
		case DayOfWeek_Thursday: { result = "thursday"; } break;
		case DayOfWeek_Friday: { result = "friday"; } break;
		case DayOfWeek_Saturday: { result = "saturday"; } break;
		default: {} break;
	}
	return result;
}

internalf TimeDense time_dense_from_date(TimeDate* date) {
	
	TimeDense result;
	u32 year_encoded = (u32)((s32)date->year + 0x8000);  // NOTE: resolves dense time rolling hover and growing larger when the year goes negative
	result = (u64)(year_encoded * 12);
	result = (result + date->month - 1) * 31;
	result = (result + date->day) * 24;
	result = (result + date->hour) * 60;
	result = (result + date->min) * 60;
	result = (result + date->sec) * 1000;
	result += date->msec;
    
	return result;
}

internalf TimeDate time_date_from_dense(TimeDense dense) {
    
	TimeDate result;
    
	result.msec = dense % 1000;
	dense /= 1000;
	result.sec = dense % 60;
	dense /= 60;
	result.min = dense % 60;
	dense /= 60;
	result.hour = dense % 24;
	dense /= 24;
	result.day = dense % 31;
	dense /= 31;
	result.month = dense % 12;
	dense /= 12;
	result.year = (s16)((s32)dense - 0x8000);
    
	return result;
}


internalf u64 time_in_nanos(void) {
    
	u64 result;

#if OS_SDL
	result = (u64)((r64)SDL_GetPerformanceCounter() * sdl_get_performance_counter_period());
#elif OS_WINDOWS
	result = win32_time_in_nanos();
#elif OS_LINUX
	result = linux_time_in_nanos();
#else
# error "time_in_nanos: not implemented for this operating system."
#endif
    
	return result;
}

internalf void time_sleep_in_nanos(u64 time) {
    
#if OS_SDL
	SDL_DelayNS(time);
#elif OS_WINDOWS
	Sleep((DWORD)(time / 1000000));
#elif OS_LINUX
	usleep((useconds_t)(time / 1000));
#else
# error "time_sleep_in_nanos: not implemented for this operating system."
#endif
}



internalf TimeDate time_now_universal(void) {
	
	TimeDate result;

#if OS_SDL
	SDL_Time sdl_ticks;
	SDL_DateTime sdl_datetime;
	SDL_GetCurrentTime(&sdl_ticks);
	SDL_TimeToDateTime(sdl_ticks, &sdl_datetime, 0);
	result = sdl_datetime_to_timedate(sdl_datetime);
#elif OS_WINDOWS
	SYSTEMTIME system_time;
	mem_zero(&system_time, sizeof(system_time));
	GetSystemTime(&system_time);
	result = win32_time_date_from_systemtime(&system_time);
#elif OS_LINUX
    time_t utc_time = {0};
    struct tm* broken_down_time;
    
    time(&utc_time);
    broken_down_time = gmtime(utc_time);
    result = linux_time_date_from_tm(broken_down_time, 0);
#else
# error "time_now_universal: not implemented for this operating system."
#endif
    
	return result;
}

internalf TimeDate time_local_from_universal(TimeDate* date) {
    
	TimeDate result;

#if OS_SDL
	SDL_Time sdl_ticks;
	SDL_DateTime sdl_datetime = sdl_datetime_from_timedate(date);
	SDL_DateTimeToTime(&sdl_datetime, &sdl_ticks);
	SDL_TimeToDateTime(sdl_ticks, &sdl_datetime, 1);
	result = sdl_datetime_to_timedate(sdl_datetime);
#elif OS_WINDOWS
	result = win32_time_local_from_universal(date);
#elif OS_LINUX
    struct tm broken_down_time_src = linux_tm_from_time_date(date);
    time_t time = mktime(&broken_down_time_src);
    struct tm* broken_down_time_dest = localtime(time);
    result = linux_time_date_from_tm(broken_down_time_dest, date->msec);
#else
# error "time_local_from_universal: not implemented for this operating system."
#endif
    
	return result;
}

internalf TimeDate time_universal_from_local(TimeDate* date) {
    
    TimeDate result;
   
#if OS_SDL
		SDL_Time sdl_ticks;
		SDL_DateTime sdl_datetime = sdl_datetime_from_timedate(date);
		SDL_DateTimeToTime(&sdl_datetime, &sdl_ticks);
		SDL_TimeToDateTime(sdl_ticks, &sdl_datetime, 0);
		result = sdl_datetime_to_timedate(sdl_datetime);
#elif OS_WINDOWS
    result = win32_time_universal_from_local(date);
#elif OS_LINUX
    struct tm broken_down_time_src = linux_tm_from_time_date(date);
    time_t time = mktime(&broken_down_time_src);
    struct tm* broken_down_time_dest = gmtime(time);
    result = linux_time_date_from_tm(broken_down_time_dest, date->msec);
#else
# error "time_universal_from_local: not implemented for this operating system."
#endif
    
    return result;
}
// -- Functions
