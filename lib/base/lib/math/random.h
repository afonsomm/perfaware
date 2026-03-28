

/**

----- Brief -----
Pseudo-random number generators.
----- Brief -----

*/

#ifndef BASE_MATH_RANDOM_H
# define BASE_MATH_RANDOM_H 


// -- Preprocessor Defines
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types

// ---- Jenkins Small Fast (JSF)
typedef struct RandomSeedJSF32 RandomSeedJSF32;
struct RandomSeedJSF32 {
	u32 a;
	u32 b;
	u32 c;
	u32 d;
};

typedef struct RandomSeedJSF64 RandomSeedJSF64;
struct RandomSeedJSF64 {
	u64 a;
	u64 b;
	u64 c;
	u64 d;
};
// ---- Jenkins Small Fast (JSF)

// -- Types


// -- Functions

// ---- Perlin Noise
internalf r32 rand_r32_perlin_noise(u32 index);
internalf r64 rand_r64_perlin_noise(u32 index);
internalf r32 rand_neg_r32_perlin_noise(u32 index);
internalf r64 rand_neg_r64_perlin_noise(u32 index);
internalf u32 rand_u32_perlin_noise(u32 index);
// ---- Perlin Noise

// ---- Jenkins Small Fast (JSF)
internalf RandomSeedJSF32 rand_u32_jsf_seed(u32 seed);
internalf u32 rand_u32_jsf(RandomSeedJSF32* seed);

internalf RandomSeedJSF64 rand_u64_jsf_seed(u64 seed);
internalf u64 rand_u64_jsf(RandomSeedJSF64* seed);
// ---- Jenkins Small Fast (JSF)

// ---- Cryptographic
internalf void rand_get_entropy(void* data, u64 size);
// ---- Cryptographic

// -- Functions


#endif  // BASE_MATH_RANDOM_H
