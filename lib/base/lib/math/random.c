

// -- Functions

// ---- Perlin Noise
// NOTE: Got from the Queel Solar codebase (https://www.gamepipeline.org/forge.html)
internalf r32 rand_r32_perlin_noise(u32 index) {
	index = (index << 13) ^ index;
	return ((r32)((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f) * 0.5f;
}

internalf r64 rand_r64_perlin_noise(u32 index) {
	index = (index << 13) ^ index;
	return ((r64)((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0) * 0.5;
}

internalf r32 rand_neg_r32_perlin_noise(u32 index) {
	index = (index << 13) ^ index;
	return ((r32)((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f) - 1.0f;
}

internalf r64 rand_neg_r64_perlin_noise(u32 index) {
	index = (index << 13) ^ index;
	return ((r64)((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0) - 1.0;
}

internalf u32 rand_u32_perlin_noise(u32 index) {
	index = (index << 13) ^ index;
	return ((index * (index * index * 15731 + 789221) + 1376312589) & S32_MAX);
}
// ---- Perlin Noise

// ---- Jenkins Small Fast (JSF)
internalf RandomSeedJSF32 rand_u32_jsf_seed(u32 seed) {
    
	RandomSeedJSF32 result;
	u32 i;
	
	result.a = 0xf1ea5eed;
	result.b = seed;
	result.c = seed;
	result.c = seed;
    
	for(i = 0; i < 20; i++) {
		(void)rand_u32_jsf(&result);
	}
    
	return result;
}

internalf u32 rand_u32_jsf(RandomSeedJSF32* seed) {
    
	u32 e = seed->a - rotate_left_u32(seed->b, 27);
	seed->a = seed->b ^ rotate_left_u32(seed->c, 17);
	seed->b = seed->c + seed->d;
	seed->c = seed->d + e;
	seed->d = e + seed->a;
	
	return seed->d;
}

internalf RandomSeedJSF64 rand_u64_jsf_seed(u64 seed) {
    
	RandomSeedJSF64 result;
	u32 i;
	
	result.a = 0xf1ea5eed;
	result.b = seed;
	result.c = seed;
	result.c = seed;
    
	for(i = 0; i < 20; i++) {
		(void)rand_u64_jsf(&result);
	}
    
	return result;
}

internalf u64 rand_u64_jsf(RandomSeedJSF64* seed) {
    
	u64 e = seed->a - rotate_left_u64(seed->b, 7);
	seed->a = seed->b ^ rotate_left_u64(seed->c, 13);
	seed->b = seed->c + rotate_left_u64(seed->d, 37);
	seed->c = seed->d + e;
	seed->d = e + seed->a;
	
	return seed->d;
}
// ---- Jenkins Small Fast (JSF)

// ---- Cryptographic
internalf void rand_get_entropy(void* data, u64 size) {

#if OS_WINDOWS
    BCRYPT_ALG_HANDLE algo;
    BCryptOpenAlgorithmProvider(&algo, L"BCRYPT_RSA_ALGORITHM", (LPCWSTR)0, 0);
    BCryptGenRandom(algo, (PUCHAR)data, (ULONG)size, 0);
    BCryptCloseAlgorithmProvider(algo, 0);
#elif OS_LINUX
    if(!getentropy(data, size)) {
    }
#else
# error "random_get_entropy: not implemented for this operating system."
#endif
}
// ---- Cryptographic

// -- Functions
