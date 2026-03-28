

internalf String8 android_str8_from_jstring(Allocator* allocator, JNIEnv* jenv, jstring jstring_obj) {
  
	// --
	String8 result;
	u32 jstring_length = (u32)(*jenv)->GetStringLength(jenv, jstring_obj);
	const jchar* jchars = (*jenv)->GetStringChars(jenv, jstring_obj, 0);
	int string_i;
	// --
  
  
	// --
	result.len = jstring_length;
	result.str = alloc_array(allocator, u8, result.len);
	for(string_i = 0; string_i < result.len; string_i += 1) {
		result.str[string_i] = (u8)(jchars[string_i]);
	}
	result.str[result.len] = 0;
	(*jenv)->ReleaseStringChars(jenv, jstring_obj, jchars);
	// --
  
	return result;
}