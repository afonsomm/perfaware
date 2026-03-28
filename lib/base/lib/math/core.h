

/**

----- Brief -----
Basic math utilities, also for 2D and 3D vectors, 1D and 2D ranges and 3x3/4x4 matrices.
----- Brief -----

*/

#ifndef BASE_MATH_CORE_H
# define BASE_MATH_CORE_H


// -- Preprocessor Defines

// ---- Intervals
#define max(a, b)       (((a) > (b)) ? (a) : (b))
#define min(a, b)       (((a) < (b)) ? (a) : (b))
#define clamp(a, v, b)  (((v) >= (b)) ? (b) : (((v) <= (a)) ? (a) : (v)))
#define clamp_bot(a, b) max(a, b)
#define clamp_top(a, b) min(a, b)
// ---- Intervals

// ---- General Constants
#define MACHINE_EPSILON_R32 1.192092e-7f
#define MACHINE_EPSILON_R64 2.220446e-16f

#define PI_R32 3.14159265359f
#define PI_R64 3.14159265359

#define TAU_R32 6.28318530718f
#define TAU_R64 3.14159265359

#define GOLD_BIG_R32 1.61803398875f
#define GOLD_BIG_R64 1.61803398875

#define GOLD_SMALL_R32 0.61803398875f
#define GOLD_SMALL_R64 0.61803398875

#define E_R32 2.71828182846f
#define E_R64 2.71828182846
// ---- General Constants

// ---- Spaces
#define axis2_flip(a) ((Axis2)(!(a)))
// ---- Spaces

// -- Preprocessor Defines


// -- Enums/Flags

// ---- Intervals
typedef enum IntervalFlag {
	IntervalFlag_None = 0,
	IntervalFlag_BottomInclusive,
	IntervalFlag_TopInclusive,
	IntervalFlag_Inclusive
} IntervalFlag;
// ---- Intervals

// ---- Spaces
typedef enum Axis2 {
	Axis2_X = 0,
	Axis2_Y,
	Axis2_COUNT
} Axis2;

typedef enum Axis3 {
	Axis3_X = 0,
	Axis3_Y,
	Axis3_Z,
	Axis3_COUNT
} Axis3;

typedef enum Axis4 {
	Axis4_X = 0,
	Axis4_Y,
	Axis4_Z,
	Axis4_W,
	Axis4_COUNT
} Axis4;
// ---- Spaces

// -- Enums/Flags


// -- Types

// ---- Vec2/Rng1
typedef union Vec2s32 Vec2s32;
union Vec2s32 {
	struct {
		s32 x;
		s32 y;
	} v;
	s32 xy[2];
};

typedef union Vec2u32 Vec2u32;
union Vec2u32 {
	struct {
		u32 x;
		u32 y;
	} v;
	u32 xy[2];
  
	struct {
		u32 min;
		u32 max;
	} r;
	u32 minmax[2];
};

typedef union Vec2r32 Vec2r32;
union Vec2r32 {
	struct {
		r32 x;
		r32 y;
	} v;
	r32 xy[2];
  
	struct {
		r32 min;
		r32 max;
	} r;
	r32 minmax[2];
};

typedef union Vec2r64 Vec2r64;
union Vec2r64 {
	struct {
		r64 x;
		r64 y;
	} v;
	r64 xy[2];
  
	struct {
		r64 min;
		r64 max;
	} r;
	r64 minmax[2];
};


typedef union Rng1s32 Rng1s32;
union Rng1s32 {
	struct {
		s32 min;
		s32 max;
	} r;
	s32 minmax[2];
};

typedef union Rng1u32 Rng1u32;
union Rng1u32 {
	struct {
		u32 min;
		u32 max;
	} r;
	u32 minmax[2];
};

typedef union Rng1r32 Rng1r32;
union Rng1r32 {
	struct {
		r32 min;
		r32 max;
	} r;
	r32 minmax[2];
};

typedef union Rng1r64 Rng1r64;
union Rng1r64 {
	struct {
		r64 min;
		r64 max;
	} r;
	r64 minmax[2];
};
// ---- Vec2/Rng1

// ---- Vec3
typedef union Vec3s32 Vec3s32;
union Vec3s32 {
	struct {
		s32 x;
		s32 y;
		s32 z;
	} v;
	s32 xyz[3]; 
};

typedef union Vec3u32 Vec3u32;
union Vec3u32 {
	struct {
		u32 x;
		u32 y;
		u32 z;
	} v;
	u32 xyz[3];
};

typedef union Vec3r32 Vec3r32;
union Vec3r32 {
	struct {
		r32 x;
		r32 y;
		r32 z;
	} v;
	r32 xyz[3];
};

typedef union Vec3r64 Vec3r64;
union Vec3r64 {
	struct {
		r64 x;
		r64 y;
		r64 z;
	} v;
	r64 xyz[3];
};
// ---- Vec3

// ---- Vec4/Rng2
typedef union Vec4s32 Vec4s32;
union Vec4s32 {
	struct {
		s32 x;
		s32 y;
		s32 z;
		s32 w;
	} v;
	s32 xyzw[4];

	struct {
		Vec2s32 xy;
		Vec2s32 zw;
	} vs;
};

typedef union Vec4u32 Vec4u32;
union Vec4u32 {
	struct {
		u32 x;
		u32 y;
		u32 z;
		u32 w;
	} v;
	u32 xyzw[4];
  
	struct {
		Vec2u32 xy;
		Vec2u32 zw;
	} vs;
};

typedef union Vec4r32 Vec4r32;
union Vec4r32 {
	struct {
		r32 x;
		r32 y;
		r32 z;
		r32 w;
	} v;
	r32 xyzw[4];
  
	struct {
		Vec2r32 xy;
		Vec2r32 zw;
	} vs;
};

typedef union Vec4r64 Vec4r64;
union Vec4r64 {
	struct {
		r64 x;
		r64 y;
		r64 z;
		r64 w;
	} v;
	r64 xyzw[4];
  
	struct {
		Vec2r64 xy;
		Vec2r64 zw;
	} vs;
};


typedef union Rng2s32 Rng2s32;
union Rng2s32 {
	struct {
		s32 x0;
		s32 y0;
		s32 x1;
		s32 y1;
	} ps;
	struct {
		Vec2s32 v0;
		Vec2s32 v1;
	} vs;
	struct {
		Vec2s32 min;
		Vec2s32 max;
	} r;
	Vec2s32 minmax[2];
};

typedef union Rng2u32 Rng2u32;
union Rng2u32 {
	struct {
		u32 x0;
		u32 y0;
		u32 x1;
		u32 y1;
	} ps;
	struct {
		Vec2u32 v0;
		Vec2u32 v1;
	} vs;
	struct {
		Vec2u32 min;
		Vec2u32 max;
	} r;
	Vec2u32 minmax[2];
};

typedef union Rng2r32 Rng2r32;
union Rng2r32 {
	struct {
		r32 x0;
		r32 y0;
		r32 x1;
		r32 y1;
	} ps;
	struct {
		Vec2r32 v0;
		Vec2r32 v1;
	} vs;
	struct {
		Vec2r32 min;
		Vec2r32 max;
	} r;
	Vec2r32 minmax[2];
};

typedef union Rng2r64 Rng2r64;
union Rng2r64 {
	struct {
		r64 x0;
		r64 y0;
		r64 x1;
		r64 y1;
	} ps;
	struct {
		Vec2r64 v0;
		Vec2r64 v1;
	} vs;
	struct {
		Vec2r64 min;
		Vec2r64 max;
	} r;
	Vec2r64 minmax[2];
};
// ---- Vec4/Rng2

// ---- Matrices
typedef struct Mat3x3r32 Mat3x3r32;
struct Mat3x3r32 {
	r32 v[4][4];
};

typedef struct Mat4x4r32 Mat4x4r32;
struct Mat4x4r32 {
	r32 v[4][4];
};
// ---- Matrices

// -- Types


// -- Functions

// ---- Intervals
internalf b8 is_between_r32(r32 min, r32 v, r32 max, IntervalFlag flag);
internalf b8 is_between_r64(r64 min, r64 v, r64 max, IntervalFlag flag);
internalf b8 is_between_u32(u32 min, u32 v, u32 max, IntervalFlag flag);
internalf b8 is_between_u64(u64 min, u64 v, u64 max, IntervalFlag flag);
// ---- Intervals

// ---- Rounding
internalf r32 floor_r32(r32 n);
internalf r64 floor_r64(r64 n);
internalf r32 ceil_r32(r32 n);
internalf r64 ceil_r64(r64 n);
internalf r32 round_r32(r32 n);
internalf r64 round_r64(r64 n);
// ---- Rounding

// ---- Absolute Value
internalf r32 abs_r32(r32 number);
internalf r64 abs_r64(r64 number);
// ---- Absolute Value

// ---- Exponentation
internalf r32 sqrt_r32(r32 number);
internalf r64 sqrt_r64(r64 number);
internalf r32 pow_r32(r32 val, r32 power);
internalf r64 pow_r64(r64 val, r64 power);
// ---- Exponentation

// ---- Trigonometry
internalf r32 degrees_to_radians_r32(r32 degrees);
internalf r64 degrees_to_radians_r64(r64 degrees);
internalf r32 radians_to_degrees_r32(r32 radians);
internalf r64 radians_to_degrees_r64(r64 radians);
internalf r32 sin_r32(r32 number);
internalf r64 sin_r64(r64 number);
internalf r32 cos_r32(r32 number);
internalf r64 cos_r64(r64 number);
internalf r32 tan_r32(r32 number);
internalf r64 tan_r64(r64 number);
internalf r32 acos_r32(r32 number);
internalf r64 acos_r64(r64 number);
internalf r32 atan2_r32(r32 x, r32 y);
internalf r64 atan2_r64(r64 x, r64 y);
// ---- Trigonometry

// ---- Interpolation
internalf r32 lerp_r32(r32 begin, r32 end, r32 factor);
internalf r32 unlerp_r32(r32 begin, r32 val, r32 end);
internalf r64 lerp_r64(r64 begin, r64 end, r64 factor);
internalf r64 unlerp_r64(r64 begin, r64 val, r64 end);
// ---- Interpolation

// ---- Vectors/Ranges

// ------ Constructors
internalf Vec2s32 vec2s32(s32 x, s32 y);
internalf Rng1s32 rng1s32(s32 min, s32 max);
internalf Vec2u32 vec2u32(u32 x, u32 y);
internalf Rng1u32 rng1u32(u32 min, u32 max);
internalf Vec2r32 vec2r32(r32 x, r32 y);
internalf Rng1r32 rng1r32(r32 min, r32 max);
internalf Vec2r64 vec2r64(r64 x, r64 y);
internalf Rng1r64 rng1r64(r64 min, r64 max);

internalf Vec3s32 vec3s32(s32 x, s32 y, s32 z);
internalf Vec3u32 vec3u32(u32 x, u32 y, u32 z);
internalf Vec3r32 vec3r32(r32 x, r32 y, r32 z);
internalf Vec3r64 vec3r64(r64 x, r64 y, r64 z);

internalf Vec4s32 vec4s32(s32 x, s32 y, s32 z, s32 w);
internalf Rng2s32 rng2s32(s32 min_x, s32 min_y, s32 max_x, s32 max_y);
internalf Vec4u32 vec4u32(u32 x, u32 y, u32 z, u32 w);
internalf Rng2u32 rng2u32(u32 min_x, u32 min_y, u32 max_x, u32 max_y);
internalf Vec4r32 vec4r32(r32 x, r32 y, r32 z, r32 w);
internalf Rng2r32 rng2r32(r32 min_x, r32 min_y, r32 max_x, r32 max_y);
internalf Vec4r64 vec4r64(r64 x, r64 y, r64 z, r64 w);
internalf Rng2r64 rng2r64(r64 min_x, r64 min_y, r64 max_x, r64 max_y);
// ------ Constructors

// ------ Operations

// -------- Vec2/Rng1
internalf Vec2s32 vec2s32_scale(Vec2s32 vec, Vec2r32 scale);
internalf Vec2s32 vec2s32_normalize(Vec2s32 vec);
internalf Vec2s32 vec2s32_reverse(Vec2s32 vec);
internalf s32 vec2s32_norm(Vec2s32 vec);
internalf Vec2s32 vec2s32_nil(void);
internalf Vec2s32 vec2s32_fill(s32 number);
internalf b8 vec2s32_is_nil(Vec2s32 vec);

internalf Vec2s32 vec2s32_add(Vec2s32 vec1, Vec2s32 vec2);
internalf Vec2s32 vec2s32_sub(Vec2s32 vec1, Vec2s32 vec2);
internalf Vec2s32 vec2s32_mul(Vec2s32 vec1, Vec2s32 vec2);
internalf Vec2s32 vec2s32_div(Vec2s32 vec1, Vec2s32 vec2);
internalf Vec2s32 vec2s32_lerp(Vec2s32 start, Vec2s32 end, Vec2r32 factor);
internalf Vec2r32 vec2s32_unlerp(Vec2s32 start, Vec2s32 vec, Vec2s32 end);

internalf s32 vec2s32_dot(Vec2s32 vec1, Vec2s32 vec2);
internalf s32 vec2s32_crossprod(Vec2s32 vec1, Vec2s32 vec2);
internalf r32 vec2s32_angle_undirected_radians(Vec2s32 vec1, Vec2s32 vec2);
internalf r32 vec2s32_angle_directed_radians(Vec2s32 vec1, Vec2s32 vec2);

internalf b8 vec2s32_match(Vec2s32 vec1, Vec2s32 vec2);

internalf s32 rng1s32_measure(Rng1s32 rng);
internalf s32 rng1s32_center(Rng1s32 rng);
internalf Rng1s32 rng1s32_pad(Rng1s32 rng, Rng1s32 pad);
internalf Rng1s32 rng1s32_clamp(Rng1s32 rng, Rng1s32 clamp);

internalf b8 rng1s32_contains_s32(Rng1s32 rng, s32 number);

internalf Rng1s32 rng1s32_union(Rng1s32 rng1, Rng1s32 rng2);
internalf Rng1s32 rng1s32_intersect(Rng1s32 rng1, Rng1s32 rng2);
internalf Rng1s32 rng1s32_clamp_rng1s32(Rng1s32 rng1, Rng1s32 rng2, Rng1s32 rng3);

internalf Vec2u32 vec2u32_scale(Vec2u32 vec, Vec2r32 scale);
internalf Vec2u32 vec2u32_normalize(Vec2u32 vec);
internalf u32 vec2u32_norm(Vec2u32 vec);
internalf Vec2u32 vec2u32_nil(void);
internalf Vec2u32 vec2u32_fill(u32 number);
internalf b8 vec2u32_is_nil(Vec2u32 vec);

internalf Vec2u32 vec2u32_add(Vec2u32 vec1, Vec2u32 vec2);
internalf Vec2u32 vec2u32_sub(Vec2u32 vec1, Vec2u32 vec2);
internalf Vec2u32 vec2u32_mul(Vec2u32 vec1, Vec2u32 vec2);
internalf Vec2u32 vec2u32_div(Vec2u32 vec1, Vec2u32 vec2);
internalf Vec2u32 vec2u32_lerp(Vec2u32 vec1, Vec2u32 vec2, Vec2r32 factor);
internalf Vec2r32 vec2u32_unlerp(Vec2u32 start, Vec2u32 vec, Vec2u32 end);

internalf u32 vec2u32_dot(Vec2u32 vec1, Vec2u32 vec2);
internalf u32 vec2u32_crossprod(Vec2u32 vec1, Vec2u32 vec2);
internalf r32 vec2u32_angle_undirected_radians(Vec2u32 vec1, Vec2u32 vec2);
internalf r32 vec2u32_angle_directed_radians(Vec2u32 vec1, Vec2u32 vec2);

internalf b8 vec2u32_match(Vec2u32 vec1, Vec2u32 vec2);

internalf u32 rng1u32_measure(Rng1u32 rng);
internalf u32 rng1u32_center(Rng1u32 rng);
internalf Rng1u32 rng1u32_pad(Rng1u32 rng, Rng1u32 pad);
internalf Rng1u32 rng1u32_clamp(Rng1u32 rng, Rng1u32 clamp);

internalf b8 rng1u32_contains_u32(Rng1u32 rng, u32 number);

internalf Rng1u32 rng1u32_union(Rng1u32 rng1, Rng1u32 rng2);
internalf Rng1u32 rng1u32_intersect(Rng1u32 rng1, Rng1u32 rng2);
internalf Rng1u32 rng1u32_clamp_rng1u32(Rng1u32 rng1, Rng1u32 rng2, Rng1u32 rng3);


internalf Vec2r32 vec2r32_scale(Vec2r32 vec, Vec2r32 scale);
internalf Vec2r32 vec2r32_scale_r32(Vec2r32 vec, r32 scale);
internalf Vec2r32 vec2r32_normalize(Vec2r32 vec);
internalf Vec2r32 vec2r32_abs(Vec2r32 vec);
internalf Vec2r32 vec2r32_reverse(Vec2r32 vec);
internalf r32 vec2r32_norm(Vec2r32 vec);
internalf Vec2r32 vec2r32_nil(void);
internalf Vec2r32 vec2r32_fill(r32 number);
internalf b8 vec2r32_is_nil(Vec2r32 vec);

internalf Vec2r32 vec2r32_add(Vec2r32 vec1, Vec2r32 vec2);
internalf Vec2r32 vec2r32_sub(Vec2r32 vec1, Vec2r32 vec2);
internalf Vec2r32 vec2r32_mul(Vec2r32 vec1, Vec2r32 vec2);
internalf Vec2r32 vec2r32_div(Vec2r32 vec1, Vec2r32 vec2);
internalf Vec2r32 vec2r32_lerp(Vec2r32 vec1, Vec2r32 vec2, Vec2r32 factor);
internalf Vec2r32 vec2r32_unlerp(Vec2r32 start, Vec2r32 vec, Vec2r32 end);

internalf r32 vec2r32_dot(Vec2r32 vec1, Vec2r32 vec2);
internalf r32 vec2r32_crossprod(Vec2r32 vec1, Vec2r32 vec2);
internalf r32 vec2r32_angle_undirected_radians(Vec2r32 vec1, Vec2r32 vec2);
internalf r32 vec2r32_angle_directed_radians(Vec2r32 vec1, Vec2r32 vec2);

internalf b8 vec2r32_match(Vec2r32 vec1, Vec2r32 vec2);

internalf r32 rng1r32_measure(Rng1r32 rng);
internalf r32 rng1r32_center(Rng1r32 rng);
internalf Rng1r32 rng1r32_pad(Rng1r32 rng, Rng1r32 pad);
internalf Rng1r32 rng1r32_clamp(Rng1r32 rng, Rng1r32 clamp);

internalf b8 rng1r32_contains_r32(Rng1r32 rng, r32 number);

internalf Rng1r32 rng1r32_union(Rng1r32 rng1, Rng1r32 rng2);
internalf Rng1r32 rng1r32_intersect(Rng1r32 rng1, Rng1r32 rng2);
internalf Rng1r32 rng1r32_clamp_rng1r32(Rng1r32 rng1, Rng1r32 rng2, Rng1r32 rng3);


internalf Vec2r64 vec2r64_scale(Vec2r64 vec, Vec2r64 scale);
internalf Vec2r64 vec2r64_normalize(Vec2r64 vec);
internalf Vec2r64 vec2r64_reverse(Vec2r64 vec);
internalf r64 vec2r64_norm(Vec2r64 vec);
internalf Vec2r64 vec2r64_nil(void);
internalf Vec2r64 vec2r64_fill(r64 number);
internalf b8 vec2r64_is_nil(Vec2r64 vec);

internalf Vec2r64 vec2r64_add(Vec2r64 vec1, Vec2r64 vec2);
internalf Vec2r64 vec2r64_sub(Vec2r64 vec1, Vec2r64 vec2);
internalf Vec2r64 vec2r64_mul(Vec2r64 vec1, Vec2r64 vec2);
internalf Vec2r64 vec2r64_div(Vec2r64 vec1, Vec2r64 vec2);
internalf Vec2r64 vec2r64_lerp(Vec2r64 vec1, Vec2r64 vec2, Vec2r64 factor);
internalf Vec2r64 vec2r64_unlerp(Vec2r64 start, Vec2r64 vec, Vec2r64 end);

internalf r64 vec2r64_dot(Vec2r64 vec1, Vec2r64 vec2);
internalf r64 vec2r64_crossprod(Vec2r64 vec1, Vec2r64 vec2);
internalf r64 vec2r64_angle_undirected_radians(Vec2r64 vec1, Vec2r64 vec2);
internalf r64 vec2r64_angle_directed_radians(Vec2r64 vec1, Vec2r64 vec2);

internalf b8 vec2r64_match(Vec2r64 vec1, Vec2r64 vec2);

internalf r64 rng1r64_measure(Rng1r64 rng);
internalf r64 rng1r64_center(Rng1r64 rng);
internalf Rng1r64 rng1r64_pad(Rng1r64 rng, Rng1r64 pad);
internalf Rng1r64 rng1r64_clamp(Rng1r64 rng, Rng1r64 clamp);

internalf b8 rng1r64_contains_r64(Rng1r64 rng, r64 number);

internalf Rng1r64 rng1r64_union(Rng1r64 rng1, Rng1r64 rng2);
internalf Rng1r64 rng1r64_intersect(Rng1r64 rng1, Rng1r64 rng2);
internalf Rng1r64 rng1r64_clamp_rng1r64(Rng1r64 rng1, Rng1r64 rng2, Rng1r64 rng3);
// -------- Vec2/Rng1

// -------- Vec3
internalf Vec3s32 vec3s32_scale(Vec3s32 vec, Vec3r32 scale);
internalf Vec3s32 vec3s32_normalize(Vec3s32 vec);
internalf Vec3s32 vec3s32_reverse(Vec3s32 vec);
internalf s32 vec3s32_norm(Vec3s32 vec);
internalf Vec3s32 vec3s32_nil(void);
internalf Vec3s32 vec3s32_fill(s32 number);
internalf b8 vec3s32_is_nil(Vec3s32 vec);

internalf Vec3s32 vec3s32_add(Vec3s32 vec1, Vec3s32 vec2);
internalf Vec3s32 vec3s32_sub(Vec3s32 vec1, Vec3s32 vec2);
internalf Vec3s32 vec3s32_mul(Vec3s32 vec1, Vec3s32 vec2);
internalf Vec3s32 vec3s32_div(Vec3s32 vec1, Vec3s32 vec2);
internalf Vec3s32 vec3s32_lerp(Vec3s32 vec1, Vec3s32 vec2, Vec3r32 factor);
internalf Vec3r32 vec3s32_unlerp(Vec3s32 start, Vec3s32 vec, Vec3s32 end);

internalf s32 vec3s32_dot(Vec3s32 vec1, Vec3s32 vec2);
internalf Vec3s32 vec3s32_crossprod(Vec3s32 vec1, Vec3s32 vec2);
internalf r32 vec3s32_angle_undirected_radians(Vec3s32 vec1, Vec3s32 vec2);
internalf r32 vec3s32_angle_directed_radians(Vec3s32 vec1, Vec3s32 vec2, Vec3s32 normal);

internalf b8 vec3s32_match(Vec3s32 vec1, Vec3s32 vec2);


internalf Vec3u32 vec3u32_scale(Vec3u32 vec, Vec3r32 scale);
internalf Vec3u32 vec3u32_normalize(Vec3u32 vec);
internalf u32 vec3u32_norm(Vec3u32 vec);
internalf Vec3u32 vec3u32_nil(void);
internalf Vec3u32 vec3u32_fill(u32 number);
internalf b8 vec3u32_is_nil(Vec3u32 vec);

internalf Vec3u32 vec3u32_add(Vec3u32 vec1, Vec3u32 vec2);
internalf Vec3u32 vec3u32_sub(Vec3u32 vec1, Vec3u32 vec2);
internalf Vec3u32 vec3u32_mul(Vec3u32 vec1, Vec3u32 vec2);
internalf Vec3u32 vec3u32_div(Vec3u32 vec1, Vec3u32 vec2);
internalf Vec3u32 vec3u32_lerp(Vec3u32 vec1, Vec3u32 vec2, Vec3r32 factor);
internalf Vec3r32 vec3u32_unlerp(Vec3u32 vec1, Vec3u32 vec, Vec3u32 end);

internalf u32 vec3u32_dot(Vec3u32 vec1, Vec3u32 vec2);
internalf Vec3u32 vec3u32_crossprod(Vec3u32 vec1, Vec3u32 vec2);
internalf r32 vec3u32_angle_undirected_radians(Vec3u32 vec1, Vec3u32 vec2);
internalf r32 vec3u32_angle_directed_radians(Vec3u32 vec1, Vec3u32 vec2, Vec3u32 normal);

internalf b8 vec3u32_match(Vec3u32 vec1, Vec3u32 vec2);
internalf Vec3r32 vec3r32_scale(Vec3r32 vec, Vec3r32 scale);
internalf Vec3r32 vec3r32_normalize(Vec3r32 vec);
internalf Vec3r32 vec3r32_reverse(Vec3r32 vec);
internalf r32 vec3r32_norm(Vec3r32 vec);
internalf Vec3r32 vec3r32_nil(void);
internalf Vec3r32 vec3r32_fill(r32 number);
internalf b8 vec3r32_is_nil(Vec3r32 vec);

internalf Vec3r32 vec3r32_add(Vec3r32 vec1, Vec3r32 vec2);
internalf Vec3r32 vec3r32_sub(Vec3r32 vec1, Vec3r32 vec2);
internalf Vec3r32 vec3r32_mul(Vec3r32 vec1, Vec3r32 vec2);
internalf Vec3r32 vec3r32_div(Vec3r32 vec1, Vec3r32 vec2);
internalf Vec3r32 vec3r32_lerp(Vec3r32 vec1, Vec3r32 vec2, Vec3r32 factor);
internalf Vec3r32 vec3r32_unlerp(Vec3r32 vec1, Vec3r32 vec, Vec3r32 end);

internalf r32 vec3r32_dot(Vec3r32 vec1, Vec3r32 vec2);
internalf Vec3r32 vec3r32_crossprod(Vec3r32 vec1, Vec3r32 vec2);
internalf r32 vec3r32_angle_undirected_radians(Vec3r32 vec1, Vec3r32 vec2);
internalf r32 vec3r32_angle_directed_radians(Vec3r32 vec1, Vec3r32 vec2, Vec3r32 normal);

internalf b8 vec3r32_match(Vec3r32 vec1, Vec3r32 vec2);
internalf Vec3r64 vec3r64_scale(Vec3r64 vec, Vec3r64 scale);
internalf Vec3r64 vec3r64_normalize(Vec3r64 vec);
internalf Vec3r64 vec3r64_reverse(Vec3r64 vec);
internalf r64 vec3r64_norm(Vec3r64 vec);
internalf Vec3r64 vec3r64_nil(void);
internalf Vec3r64 vec3r64_fill(r64 number);
internalf b8 vec3r64_is_nil(Vec3r64 vec);

internalf Vec3r64 vec3r64_add(Vec3r64 vec1, Vec3r64 vec2);
internalf Vec3r64 vec3r64_sub(Vec3r64 vec1, Vec3r64 vec2);
internalf Vec3r64 vec3r64_mul(Vec3r64 vec1, Vec3r64 vec2);
internalf Vec3r64 vec3r64_div(Vec3r64 vec1, Vec3r64 vec2);
internalf Vec3r64 vec3r64_lerp(Vec3r64 vec1, Vec3r64 vec2, Vec3r64 factor);
internalf Vec3r64 vec3r64_unlerp(Vec3r64 vec1, Vec3r64 vec, Vec3r64 end);

internalf r64 vec3r64_dot(Vec3r64 vec1, Vec3r64 vec2);
internalf Vec3r64 vec3r64_crossprod(Vec3r64 vec1, Vec3r64 vec2);
internalf r64 vec3r64_angle_undirected_radians(Vec3r64 vec1, Vec3r64 vec2);
internalf r64 vec3r64_angle_directed_radians(Vec3r64 vec1, Vec3r64 vec2, Vec3r64 normal);

internalf b8 vec3r64_match(Vec3r64 vec1, Vec3r64 vec2);
// -------- Vec3

// -------- Vec4/Rng2
internalf Vec4s32 vec4s32_scale(Vec4s32 vec, Vec4r32 scale);
internalf Vec4s32 vec4s32_normalize(Vec4s32 vec);
internalf Vec4s32 vec4s32_reverse(Vec4s32 vec);
internalf s32 vec4s32_norm(Vec4s32 vec);
internalf Vec4s32 vec4s32_nil(void);
internalf Vec4s32 vec4s32_fill(s32 number);
internalf b8 vec4s32_is_nil(Vec4s32 vec);

internalf Vec4s32 vec4s32_add(Vec4s32 vec1, Vec4s32 vec2);
internalf Vec4s32 vec4s32_sub(Vec4s32 vec1, Vec4s32 vec2);
internalf Vec4s32 vec4s32_mul(Vec4s32 vec1, Vec4s32 vec2);
internalf Vec4s32 vec4s32_div(Vec4s32 vec1, Vec4s32 vec2);
internalf Vec4s32 vec4s32_lerp(Vec4s32 start, Vec4s32 end, Vec4r32 factor);
internalf Vec4r32 vec4s32_unlerp(Vec4s32 start, Vec4s32 vec, Vec4s32 end);

internalf s32 vec4s32_dot(Vec4s32 vec1, Vec4s32 vec2);
internalf r32 vec4s32_angle_undirected_radians(Vec4s32 vec1, Vec4s32 vec2);

internalf b8 vec4s32_match(Vec4s32 vec1, Vec4s32 vec2);

internalf Vec2s32 rng2s32_measure(Rng2s32 rng);
internalf Vec2s32 rng2s32_center(Rng2s32 rng);
internalf Rng2s32 rng2s32_pad(Rng2s32 rng, Rng2s32 pad);
internalf Rng2s32 rng2s32_clamp(Rng2s32 rng, Rng2s32 clamp);

internalf b8 rng2s32_contains_vec2s32(Rng2s32 rng, Vec2s32 vec);

internalf Rng2s32 rng2s32_union(Rng2s32 rng1, Rng2s32 rng2);
internalf Rng2s32 rng2s32_intersect(Rng2s32 rng1, Rng2s32 rng2);
internalf Rng2s32 rng2s32_clamp_rng2s32(Rng2s32 rng1, Rng2s32 rng2, Rng2s32 rng3);


internalf Vec4u32 vec4u32_scale(Vec4u32 vec, Vec4r32 scale);
internalf Vec4u32 vec4u32_normalize(Vec4u32 vec);
internalf u32 vec4u32_norm(Vec4u32 vec);
internalf Vec4u32 vec4u32_nil(void);
internalf Vec4u32 vec4u32_fill(u32 number);
internalf b8 vec4u32_is_nil(Vec4u32 vec);

internalf Vec4u32 vec4u32_add(Vec4u32 vec1, Vec4u32 vec2);
internalf Vec4u32 vec4u32_sub(Vec4u32 vec1, Vec4u32 vec2);
internalf Vec4u32 vec4u32_mul(Vec4u32 vec1, Vec4u32 vec2);
internalf Vec4u32 vec4u32_div(Vec4u32 vec1, Vec4u32 vec2);
internalf Vec4u32 vec4u32_lerp(Vec4u32 start, Vec4u32 end, Vec4r32 factor);
internalf Vec4r32 vec4u32_unlerp(Vec4u32 start, Vec4u32 vec, Vec4u32 end);

internalf u32 vec4u32_dot(Vec4u32 vec1, Vec4u32 vec2);
internalf r32 vec4u32_angle_undirected_radians(Vec4u32 vec1, Vec4u32 vec2);

internalf b8 vec4u32_match(Vec4u32 vec1, Vec4u32 vec2);

internalf Vec2u32 rng2u32_measure(Rng2u32 rng);
internalf Vec2u32 rng2u32_center(Rng2u32 rng);
internalf Rng2u32 rng2u32_pad(Rng2u32 rng, Rng2u32 pad);
internalf Rng2u32 rng2u32_clamp(Rng2u32 rng, Rng2u32 clamp);

internalf b8 rng2u32_contains_vec2u32(Rng2u32 rng, Vec2u32 vec);

internalf Rng2u32 rng2u32_union(Rng2u32 rng1, Rng2u32 rng2);
internalf Rng2u32 rng2u32_intersect(Rng2u32 rng1, Rng2u32 rng2);
internalf Rng2u32 rng2u32_clamp_rng2u32(Rng2u32 rng1, Rng2u32 rng2, Rng2u32 rng3);
internalf Vec4r32 vec4r32_scale(Vec4r32 vec, Vec4r32 scale);
internalf Vec4r32 vec4r32_normalize(Vec4r32 vec);
internalf Vec4r32 vec4r32_reverse(Vec4r32 vec);
internalf r32 vec4r32_norm(Vec4r32 vec);
internalf Vec4r32 vec4r32_nil(void);
internalf Vec4r32 vec4r32_fill(r32 number);
internalf b8 vec4r32_is_nil(Vec4r32 vec);

internalf Vec4r32 vec4r32_add(Vec4r32 vec1, Vec4r32 vec2);
internalf Vec4r32 vec4r32_sub(Vec4r32 vec1, Vec4r32 vec2);
internalf Vec4r32 vec4r32_mul(Vec4r32 vec1, Vec4r32 vec2);
internalf Vec4r32 vec4r32_div(Vec4r32 vec1, Vec4r32 vec2);
internalf Vec4r32 vec4r32_lerp(Vec4r32 start, Vec4r32 end, Vec4r32 factor);
internalf Vec4r32 vec4r32_unlerp(Vec4r32 start, Vec4r32 vec, Vec4r32 end);

internalf r32 vec4r32_dot(Vec4r32 vec1, Vec4r32 vec2);
internalf r32 vec4r32_angle_undirected_radians(Vec4r32 vec1, Vec4r32 vec2);

internalf b8 vec4r32_match(Vec4r32 vec1, Vec4r32 vec2);

internalf Vec2r32 rng2r32_measure(Rng2r32 rng);
internalf Rng2r32 rng2r32_nil(void);
internalf b8 rng2r32_is_nil(Rng2r32 rng);
internalf Vec2r32 rng2r32_center(Rng2r32 rng);
internalf Rng2r32 rng2r32_pad(Rng2r32 rng, Rng2r32 pad);
internalf Rng2r32 rng2r32_clamp(Rng2r32 rng, Rng2r32 clamp);

internalf b8 rng2r32_contains_vec2r32(Rng2r32 rng, Vec2r32 vec);

internalf Rng2r32 rng2r32_union(Rng2r32 rng1, Rng2r32 rng2);
internalf Rng2r32 rng2r32_intersect(Rng2r32 rng1, Rng2r32 rng2);
internalf Rng2r32 rng2r32_clamp_rng2r32(Rng2r32 rng1, Rng2r32 rng2, Rng2r32 rng3);

internalf Vec4r64 vec4r64_scale(Vec4r64 vec, Vec4r64 scale);
internalf Vec4r64 vec4r64_normalize(Vec4r64 vec);
internalf Vec4r64 vec4r64_reverse(Vec4r64 vec);
internalf r64 vec4r64_norm(Vec4r64 vec);
internalf Vec4r64 vec4r64_nil(void);
internalf Vec4r64 vec4r64_fill(r64 number);
internalf b8 vec4r64_is_nil(Vec4r64 vec);

internalf Vec4r64 vec4r64_add(Vec4r64 vec1, Vec4r64 vec2);
internalf Vec4r64 vec4r64_sub(Vec4r64 vec1, Vec4r64 vec2);
internalf Vec4r64 vec4r64_mul(Vec4r64 vec1, Vec4r64 vec2);
internalf Vec4r64 vec4r64_div(Vec4r64 vec1, Vec4r64 vec2);
internalf Vec4r64 vec4r64_lerp(Vec4r64 start, Vec4r64 end, Vec4r64 factor);
internalf Vec4r64 vec4r64_unlerp(Vec4r64 start, Vec4r64 vec, Vec4r64 end);

internalf r64 vec4r64_dot(Vec4r64 vec1, Vec4r64 vec2);
internalf r64 vec4r64_angle_undirected_radians(Vec4r64 vec1, Vec4r64 vec2);

internalf b8 vec4r64_match(Vec4r64 vec1, Vec4r64 vec2);

internalf Vec2r64 rng2r64_measure(Rng2r64 rng);
internalf Vec2r64 rng2r64_center(Rng2r64 rng);
internalf Rng2r64 rng2r64_pad(Rng2r64 rng, Rng2r64 pad);
internalf Rng2r64 rng2r64_clamp(Rng2r64 rng, Rng2r64 clamp);

internalf b8 rng2r64_contains_vec2r64(Rng2r64 rng, Vec2r64 vec);

internalf Rng2r64 rng2r64_union(Rng2r64 rng1, Rng2r64 rng2);
internalf Rng2r64 rng2r64_intersect(Rng2r64 rng1, Rng2r64 rng2);
internalf Rng2r64 rng2r64_clamp_rng2r64(Rng2r64 rng1, Rng2r64 rng2, Rng2r64 rng3);
// -------- Vec4/Rng2

// ------ Operations

// ---- Vectors/Ranges

// -- Functions


#endif  // BASE_MATH_CORE_H
