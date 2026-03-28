

/**

----- Brief -----
Wrappers for the spall profiler.
----- Brief -----

*/

#ifndef BASE_TIME_PROFILE_H
# define BASE_TIME_PROFILE_H


#include "../../../externals/inc_begin.h"
#include "../../../externals/spall/spall.h"
#include "../../../externals/inc_end.h"


// -- Preprocessor Defines
#ifdef PROFILE
# define profile_begin() spall_buffer_begin(&profile_state->ctx, &profile_state->buffer, __FUNCTION__, sizeof(__FUNCTION__) - 1, time_in_nanos())
# define profile_end()   spall_buffer_end(&profile_state->ctx, &profile_state->buffer, time_in_nanos())
#else
# define profile_begin()
# define profile_end()
#endif 
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
typedef struct Profile_State Profile_State;
struct Profile_State {
	Arena* arena;
	SpallProfile ctx;
	SpallBuffer buffer;
};
// -- Types


// -- Functions
#ifdef PROFILE
internalf void base_profile_init(void);
internalf void base_profile_deinit(void);
#else
# define base_profile_init(void)
# define base_profile_deinit(void)
#endif
// -- Functions


#endif  // BASE_TIME_PROFILE_H
