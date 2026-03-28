

/**

----- Brief -----
Basic file IO utilities that also abstract the OS layer.
----- Brief -----

*/


#ifndef BASE_IO_FILES_H
# define BASE_IO_FILES_H


// -- Preprocessor Defines
#if OS_WINDOWS
# define FILE_PATH_DEFAULT_LEN 260
# define FILE_PATH_SEPARATOR_CHAR '\\'
# define FILE_PATH_SEPARATOR_STR  "\\"
#elif OS_LINUX
# define FILE_PATH_DEFAULT_LEN 4096
# define FILE_PATH_SEPARATOR_CHAR '/'
# define FILE_PATH_SEPARATOR_STR  "/"
#else
# error "base_files.h: need to define the followings for this operating system: FILE_PATH_DEFAULT_LEN."
#endif
// -- Preprocessor Defines


// -- Enums/Flags
typedef enum FilePropertyFlag {
	FilePropertyFlag_Directory = 1 << 0,
} FilePropertyFlag;

typedef enum FileAccessFlag {
	FileAccessFlag_Readable   = 1 << 0,
	FileAccessFlag_Writeable  = 1 << 1,
	FileAccessFlag_Executable = 1 << 2,
} FileAccessFlag;

typedef enum FileIteratorFlag {
	FileIteratorFlag_Recursive       = 1 << 0,  // TODO
	FileIteratorFlag_FilesOnly       = 1 << 1,  // TODO
	FileIteratorFlag_DirectoriesOnly = 1 << 2,  // TODO
} FileIteratorFlag;

typedef enum FileSystemPath {
	FileSystemPath_CurrentDirectory = 0,
	FileSystemPath_ExecutablePath,
	FileSystemPath_UserDirectory,
	FileSystemPath_TempDirectory,
	FileSystemPath_COUNT
} FileSystemPath;
// -- Enums/Flags


// -- Types
typedef struct FileInfo FileInfo;
struct FileInfo {
	String8 path;
	u8* dir;
	u8* name;
	u8* extension;
	u64 update_time;
	u64 create_time;
	u64 size;
	enum32(FilePropertyFlag) properties;
	enum32(FileAccessFlag) access;
};

typedef struct File File;
struct File {
	FileInfo info;
	void* content;
};

typedef union FileIterator FileIterator;
union FileIterator {
	Handle handle;
};
// -- Types


// -- Functions
internalf void base_files_init(void);
internalf void base_files_deinit(void);

// ---- Validators
internalf FileInfo file_info_nil(void);
internalf b8 file_info_is_valid(FileInfo info);
internalf File file_nil(void);
internalf b8 file_is_valid(File file);
// ---- Validators

// ---- File System Paths
internalf String8 file_system_get_current_directory(Allocator* allocator);
internalf String8 file_system_get_executable_path(void);
internalf String8 file_system_get_user_directory(void);
internalf String8 file_system_get_temp_directory(void);
// ---- File System Paths

// ---- Names/Paths
internalf void file_info_path_parcellate(FileInfo* info);
internalf b8 file_name_match_extension(String8 filename, String8 extension, u64 extension_pos);
internalf b8 file_rename(String8 current, String8 newname);
internalf String8 file_fullpath_from_relative(Allocator* allocator, String8 relative);
// ---- Names/Paths

// ---- Loading
internalf File file_load(Allocator* allocator, String8 path);
internalf FileInfo file_info_load(Allocator* allocator, String8 path);
internalf File file_load_from_info(Allocator* allocator, FileInfo info);
// ---- Loading

// ---- Writing
internalf b8 file_write(String8 path, String8 data);
internalf b8 file_write_list(String8 path, String8List* data);
internalf b8 file_delete(String8 path);
internalf b8 directory_create(String8 path);
internalf b8 directory_delete(String8 path);
// ---- Writing

// ---- Iterating
internalf FileIterator file_iterator_init(Allocator* allocator, String8 dir, String8 wildcard);
internalf b8 file_iterator_next(Allocator* allocator, FileIterator iterator, FileInfo* out, enum32(FileIteratorFlag) flags);
internalf void file_iterator_end(FileIterator iterator);
// ---- Iterating

// -- Functions


#endif  // BASE_IO_FILES_H
