

/**

----- Brief -----
Entry point for different parts of the program.
The user has to define "entry_point_udf" as the custom entry point for their application.

To define custom command line arguments for an application define CMD_LINE_ARGS_FLAGS_CUSTOM as an array of String8s like base_cmd_line_args_flags in the body of this module.

NOTE: if EXTERNAL_ENTRY is defined, then it is assumed that an external entry point is provided, so the operating system one from this library is not
compiled int. This means base_main_thread_entry_point() is not called. To use this library then, you have to call base_main_thread_init() and 
base_main_thread_deinit() in your application's initializer and deinitializer, respectively.
----- Brief -----

*/


#ifndef BASE_ENTRY_CORE_H
# define BASE_ENTRY_CORE_H


// -- Preprocessor Defines
#ifndef base_async_thread_entry_point_udf
# define base_async_thread_entry_point_udf(params)
#endif

#ifndef CUSTOM_CMD_LINE_ARGS_PARAMS
# define CUSTOM_CMD_LINE_ARGS_PARAMS empty_custom_cmd_line_args_params
#endif
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
typedef struct CmdLineArgParams CmdLineArgParams;
struct CmdLineArgParams {
    String8 name;
    u32 values_cap;
};

typedef struct CmdLineArg CmdLineArg;
struct CmdLineArg {
	HashMapEntry entry;
	String8 name;
  String8* values;
  u32 values_cap;
	b8 set;
};

typedef struct CmdLineArgs CmdLineArgs;
struct CmdLineArgs {
  Arena* arena;
	HashMapSlot* hashmap;
	u32 cap;
};
// -- Types


// -- Functions

// ---- User-Defined Entry Point Callback
internalf void entry_point_udf(void);
// ---- User-Defined Entry Point Callback

// ---- (De)Initializer
internalf void base_main_thread_init(s32 argc, s8* argv[]);
internalf void base_main_thread_deinit(void);
// ---- (De)Initializer

// ---- Thread Entry Points
internalf void base_main_thread_entry_point(s32 argc, s8* argv[]);
internalf void base_supplement_thread_entry_point(ThreadEntryPoint_FunctionType* func, void* params);
internalf void base_async_thread_entry_point(void* params);
// ---- Thread Entry Points

// ---- Command Line Arguments
internalf void cmd_line_args_init(s32 argc, s8* argv[]);
internalf void cmd_line_args_add_flags_from_params(CmdLineArgParams* args_params, u32 args_params_len);
internalf CmdLineArg* cmd_line_args_get_arg_(String8 name, b8 has_to_be_set);
internalf CmdLineArg* cmd_line_args_get_arg(String8 name);
internalf b8 cmd_line_arg_is_set(String8 name);
internalf String8 cmd_line_args_get_value(String8 name, u32 index);
// ---- Command Line Arguments

// -- Functions


#endif  // BASE_ENTRY_CORE_H
