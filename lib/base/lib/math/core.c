

#include "../../../externals/inc_begin.h"
#include <math.h>
#include "../../../externals/inc_end.h"


// ---- Rounding
internalf r32 floor_r32(r32 f) {
	return (r32)(f < 0 ? (s64)(f) - 1 : (s64)f);
}

internalf r64 floor_r64(r64 f) {
	return (r64)(f < 0 ? (s64)(f) - 1 : (s64)f);
}

internalf r32 ceil_r32(r32 f) {
	return (r32)(f == (r32)(s64)f ? (s64)f : (f > 0 ? (s64)f + 1 : (s64)f));
}

internalf r64 ceil_r64(r64 f) {
	return (r64)(f == (r64)(s64)f ? (s64)f : (f > 0 ? (s64)f + 1 : (s64)f));
}

internalf r32 round_r32(r32 f) {
  
	r32 result;
	if((f - (r64)(s32)f) < 0.5f) {
		result = floor_r32(f);
	} else {
		result = ceil_r32(f);
	}
  
	return result;
}

internalf r64 round_r64(r64 f) {
  
	r64 result;
	if((f - (r64)(s64)f) < 0.5) {
		result = floor_r64(f);
	} else {
		result = ceil_r64(f);
	}
  
	return result;
}
// ---- Rounding


// ---- Intervals
internalf b8 is_between_r32(r32 min, r32 v, r32 max, IntervalFlag flag) {

	b8 result = 0;

	switch(flag) {
		default: {} break;
		case IntervalFlag_None: {
			result = (min < v) && (v < max);
		} break;
		case IntervalFlag_BottomInclusive: {
			result = (min <= v) && (v < max);
		} break;
		case IntervalFlag_TopInclusive: {
			result = (min < v) && (v <= max);
		} break;
		case IntervalFlag_Inclusive: {
			result = (min <= v) && (v <= max);
		} break;
	};

	return result;
}

internalf b8 is_between_r64(r64 min, r64 v, r64 max, IntervalFlag flag) {

	b8 result = 0;

	switch(flag) {
		default: {} break;
		case IntervalFlag_None: {
			result = (min < v) && (v < max);
		} break;
		case IntervalFlag_BottomInclusive: {
			result = (min <= v) && (v < max);
		} break;
		case IntervalFlag_TopInclusive: {
			result = (min < v) && (v <= max);
		} break;
		case IntervalFlag_Inclusive: {
			result = (min <= v) && (v <= max);
		} break;
	};

	return result;
}

internalf b8 is_between_u32(u32 min, u32 v, u32 max, IntervalFlag flag) {

	b8 result = 0;

	switch(flag) {
		default: {} break;
		case IntervalFlag_None: {
			result = (min < v) && (v < max);
		} break;
		case IntervalFlag_BottomInclusive: {
			result = (min <= v) && (v < max);
		} break;
		case IntervalFlag_TopInclusive: {
			result = (min < v) && (v <= max);
		} break;
		case IntervalFlag_Inclusive: {
			result = (min <= v) && (v <= max);
		} break;
	};

	return result;
}

internalf b8 is_between_u64(u64 min, u64 v, u64 max, IntervalFlag flag) {

	b8 result = 0;

	switch(flag) {
		default: {} break;
		case IntervalFlag_None: {
			result = (min < v) && (v < max);
		} break;
		case IntervalFlag_BottomInclusive: {
			result = (min <= v) && (v < max);
		} break;
		case IntervalFlag_TopInclusive: {
			result = (min < v) && (v <= max);
		} break;
		case IntervalFlag_Inclusive: {
			result = (min <= v) && (v <= max);
		} break;
	};

	return result;
}
// ---- Intervals

// ---- Absolute Value
internalf r32 abs_r32(r32 number) {
	union { r32 f; u64 u; } r;
	r.f = number;
	r.u &= 0x7fffffff;
	return r.f;
}

internalf r64 abs_r64(r64 number) {
	union { r64 f; u64 u; } r;
	r.f = number;
	r.u &= 0x7fffffffffffffff;
	return r.f;
}
// ---- Absolute Value

// ---- Exponentation
internalf r32 sqrt_r32(r32 number)
{
	s32 i;
	r32 x, y;
	x = number * 0.5f;
	y = number;
	i = * (s32 * ) &y;
	i = 0x5f3759df - (i >> 1);
	y = * ( r32 * ) &i;
	y = y * (1.5f - (x * y * y));
	y = y * (1.5f - (x * y * y));
	return number * y;
}

internalf r64 sqrt_r64(r64 number) {
	return sqrt(number);
}

internalf r32 pow_r32(r32 val, r32 power) {
	return powf(val, power);
}

internalf r64 pow_r64(r64 val, r64 power) {
	return pow(val, power);
}
// ---- Exponentation

// ---- Trigonometry
internalf r32 degrees_to_radians_r32(r32 degrees) {
	return degrees * PI_R32 / 180.0f;
}

internalf r64 degrees_to_radians_r64(r64 degrees) {
	return degrees * PI_R64 / 180.0;
}

internalf r32 radians_to_degrees_r32(r32 radians) {
	return radians * 180.0f / PI_R32;
}

internalf r64 radians_to_degrees_r64(r64 radians) {
	return radians * 180.0 / PI_R64;
}

internalf r32 sin_r32(r32 number) {
	return sinf(number);
}

internalf r64 sin_r64(r64 number) {
	return sin(number);
}

internalf r32 cos_r32(r32 number) {
	return cosf(number);
}

internalf r64 cos_r64(r64 number) {
	return cos(number);
}

internalf r32 tan_r32(r32 number) {
	return tanf(number);
}

internalf r64 tan_r64(r64 number) {
	return tan(number);
}

internalf r32 acos_r32(r32 number) {
	return acosf(number);
}

internalf r64 acos_r64(r64 number) {
	return acos(number);
}

internalf r32 atan2_r32(r32 x, r32 y) {
	return atan2f(y, x);
}

internalf r64 atan2_r64(r64 x, r64 y) {
	return atan2(y, x);
}
// ---- Trigonometry

// ---- Interpolation
internalf r32 lerp_r32(r32 begin, r32 end, r32 factor) {
	return begin + (end - begin) * factor;
}

internalf r32 unlerp_r32(r32 begin, r32 val, r32 end) {
	return (val - begin) / (end - begin);
}

internalf r64 lerp_r64(r64 begin, r64 end, r64 factor) {
	return begin + (end - begin) * factor;
}

internalf r64 unlerp_r64(r64 begin, r64 val, r64 end) {
	return (val - begin) / (end - begin);
}
// ---- Interpolation

// ---- Vectors/Ranges

// ------ Constructors
internalf Vec2s32 vec2s32(s32 x, s32 y) {
	Vec2s32 result;
	result.v.x = x;
	result.v.y = y;
	return result;
}

internalf Rng1s32 rng1s32(s32 min, s32 max) {
	Rng1s32 result;
	result.r.min = min;
	result.r.max = max;
	return result;
}

internalf Vec2u32 vec2u32(u32 x, u32 y) {
	Vec2u32 result;
	result.v.x = x;
	result.v.y = y;
	return result;
}

internalf Rng1u32 rng1u32(u32 min, u32 max) {
	Rng1u32 result;
	result.r.min = min;
	result.r.max = max;
	return result;
}

internalf Vec2r32 vec2r32(r32 x, r32 y) {
	Vec2r32 result;
	result.v.x = x;
	result.v.y = y;
	return result;
}

internalf Rng1r32 rng1r32(r32 min, r32 max) {
	Rng1r32 result;
	result.r.min = min;
	result.r.max = max;
	return result;
}

internalf Vec2r64 vec2r64(r64 x, r64 y) {
	Vec2r64 result;
	result.v.x = x;
	result.v.y = y;
	return result;
}

internalf Rng1r64 rng1r64(r64 min, r64 max) {
	Rng1r64 result;
	result.r.min = min;
	result.r.max = max;
	return result;
}

internalf Vec3s32 vec3s32(s32 x, s32 y, s32 z) {
	Vec3s32 result;
	result.v.x = x;
	result.v.y = y;
	result.v.z = z;
	return result;
}

internalf Vec3u32 vec3u32(u32 x, u32 y, u32 z) {
	Vec3u32 result;
	result.v.x = x;
	result.v.y = y;
	result.v.z = z;
	return result;
}

internalf Vec3r32 vec3r32(r32 x, r32 y, r32 z) {
	Vec3r32 result;
	result.v.x = x;
	result.v.y = y;
	result.v.z = z;
	return result;
}

internalf Vec3r64 vec3r64(r64 x, r64 y, r64 z) {
	Vec3r64 result;
	result.v.x = x;
	result.v.y = y;
	result.v.z = z;
	return result;
}

internalf Vec4s32 vec4s32(s32 x, s32 y, s32 z, s32 w) {
	Vec4s32 result;
	result.v.x = x;
	result.v.y = y;
	result.v.z = z;
	result.v.w = w;
	return result;
}

internalf Rng2s32 rng2s32(s32 min_x, s32 min_y, s32 max_x, s32 max_y) {
	Rng2s32 result;
	result.r.min.v.x = min_x;
	result.r.min.v.y = min_y;
	result.r.max.v.x = max_x;
	result.r.max.v.y = max_y;
	return result;
}

internalf Vec4u32 vec4u32(u32 x, u32 y, u32 z, u32 w) {
	Vec4u32 result;
	result.v.x = x;
	result.v.y = y;
	result.v.z = z;
	result.v.w = w;
	return result;
}

internalf Rng2u32 rng2u32(u32 min_x, u32 min_y, u32 max_x, u32 max_y) {
	Rng2u32 result;
	result.r.min.v.x = min_x;
	result.r.min.v.y = min_y;
	result.r.max.v.x = max_x;
	result.r.max.v.y = max_y;
	return result;
}

internalf Vec4r32 vec4r32(r32 x, r32 y, r32 z, r32 w) {
	Vec4r32 result;
	result.v.x = x;
	result.v.y = y;
	result.v.z = z;
	result.v.w = w;
	return result;
}

internalf Rng2r32 rng2r32(r32 min_x, r32 min_y, r32 max_x, r32 max_y) {
	Rng2r32 result;
	result.r.min.v.x = min_x;
	result.r.min.v.y = min_y;
	result.r.max.v.x = max_x;
	result.r.max.v.y = max_y;
	return result;
}

internalf Vec4r64 vec4r64(r64 x, r64 y, r64 z, r64 w) {
	Vec4r64 result;
	result.v.x = x;
	result.v.y = y;
	result.v.z = z;
	result.v.w = w;
	return result;
}

internalf Rng2r64 rng2r64(r64 min_x, r64 min_y, r64 max_x, r64 max_y) {
	Rng2r64 result;
	result.r.min.v.x = min_x;
	result.r.min.v.y = min_y;
	result.r.max.v.x = max_x;
	result.r.max.v.y = max_y;
	return result;
}
// ------ Constructors

// ------ Operations

// -------- Vec2/Rng1
internalf Vec2s32 vec2s32_scale(Vec2s32 vec, Vec2r32 scale) {
	Vec2s32 result;
	result.v.x = (s32)((r32)vec.v.x * scale.v.x);
	result.v.y = (s32)((r32)vec.v.y * scale.v.y);
	return result;
}

internalf Vec2s32 vec2s32_normalize(Vec2s32 vec) {
	s32 length = vec2s32_norm(vec);
	Vec2s32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	return result;
}

internalf Vec2s32 vec2s32_reverse(Vec2s32 vec) {
	Vec2s32 result;
	result.v.x = -1 * vec.v.x;
	result.v.y = -1 * vec.v.y;
	return result;
}

internalf s32 vec2s32_norm(Vec2s32 vec) {
	return (s32)sqrt_r32((r32)(vec.v.x * vec.v.x + vec.v.y * vec.v.y));
}

internalf Vec2s32 vec2s32_nil(void) {
	Vec2s32 result;
	result.v.x = 0;
	result.v.y = 0;
	return result;
}

internalf Vec2s32 vec2s32_fill(s32 number) {
	Vec2s32 result;
	result.v.x = number;
	result.v.y = number;
	return result;
}

internalf b8 vec2s32_is_nil(Vec2s32 vec) {
	return (b8)((vec.v.x == 0) && (vec.v.y == 0));
}

internalf Vec2s32 vec2s32_add(Vec2s32 vec1, Vec2s32 vec2) {
	Vec2s32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	return result;
}

internalf Vec2s32 vec2s32_sub(Vec2s32 vec1, Vec2s32 vec2) {
	Vec2s32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	return result;
}

internalf Vec2s32 vec2s32_mul(Vec2s32 vec1, Vec2s32 vec2) {
	Vec2s32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	return result;
}

internalf Vec2s32 vec2s32_div(Vec2s32 vec1, Vec2s32 vec2) {
	Vec2s32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	return result;
}

internalf Vec2s32 vec2s32_lerp(Vec2s32 vec1, Vec2s32 vec2, Vec2r32 factor) {
	Vec2s32 result;
	result.v.x = vec1.v.x + (s32)((r32)(vec2.v.x - vec1.v.x) * factor.v.x);
	result.v.y = vec1.v.y + (s32)((r32)(vec2.v.y - vec1.v.y) * factor.v.y);
	return result;
}

internalf Vec2r32 vec2s32_unlerp(Vec2s32 start, Vec2s32 vec, Vec2s32 end) {
	Vec2r32 result;
	result.v.x = (r32)vec.v.x / (r32)(end.v.x - start.v.x);
	result.v.y = (r32)vec.v.y / (r32)(end.v.y - start.v.y);
	return result;
}

internalf s32 vec2s32_dot(Vec2s32 vec1, Vec2s32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y;
}

internalf s32 vec2s32_crossprod(Vec2s32 vec1, Vec2s32 vec2) {
	return vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
}

internalf r32 vec2s32_angle_undirected_radians(Vec2s32 vec1, Vec2s32 vec2) {
	r32 cosine = (r32)vec2s32_dot(vec1, vec2) / (r32)(vec2s32_norm(vec1) * vec2s32_norm(vec2));
	return acos_r32(cosine);
}

internalf r32 vec2s32_angle_directed_radians(Vec2s32 vec1, Vec2s32 vec2) {
	return atan2_r32((r32)vec2s32_crossprod(vec1, vec2), (r32)vec2s32_dot(vec1, vec2));
}

internalf b8 vec2s32_match(Vec2s32 vec1, Vec2s32 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y));
}

internalf s32 rng1s32_measure(Rng1s32 rng) {
	return rng.r.max - rng.r.min;
}

internalf s32 rng1s32_center(Rng1s32 rng) {
	return rng.r.min + (s32)((r32)(rng.r.max - rng.r.min) * 0.5f);
}

internalf Rng1s32 rng1s32_pad(Rng1s32 rng, Rng1s32 pad) {
	Rng1s32 result;
	result.r.min = rng.r.min - pad.r.min;
	result.r.max = rng.r.max + pad.r.max;
	return result;
}

internalf Rng1s32 rng1s32_clamp(Rng1s32 rng, Rng1s32 clamp) {
	Rng1s32 result;
	result.r.min = clamp(clamp.r.min, rng.r.min, clamp.r.max);
	result.r.max = clamp(clamp.r.min, rng.r.max, clamp.r.max);
	return result;
}

internalf b8 rng1s32_contains_s32(Rng1s32 rng, s32 number) {
	return (b8)((number >= rng.r.min) && (number <= rng.r.max));
}

internalf Rng1s32 rng1s32_union(Rng1s32 rng1, Rng1s32 rng2) {
	Rng1s32 result;
	result.r.min = min(rng1.r.min, rng2.r.min);
	result.r.max = max(rng1.r.max, rng2.r.max);
	return result;
}

internalf Rng1s32 rng1s32_intersect(Rng1s32 rng1, Rng1s32 rng2) {
	Rng1s32 result;
  
	s32 min = max(rng1.r.min, rng2.r.min);
	s32 max = min(rng1.r.max, rng2.r.max);
	if (min > max) {
		min = 0;
		max = 0;
	}
  
	result.r.min = min;
	result.r.max = max;
	return result;
}

internalf Rng1s32 rng1s32_clamp_rng1s32(Rng1s32 rng1, Rng1s32 rng2, Rng1s32 rng3) {
	Rng1s32 rng_intersect = rng1s32_intersect(rng1, rng2);
	Rng1s32 result;
	result.r.max = clamp(rng_intersect.r.min, rng3.r.max, rng_intersect.r.max);
	result.r.min = clamp(rng_intersect.r.min, rng3.r.min, rng_intersect.r.max);
	return result;
}


internalf Vec2u32 vec2u32_scale(Vec2u32 vec, Vec2r32 scale) {
	Vec2u32 result;
	result.v.x = (u32)((r32)vec.v.x * scale.v.x);
	result.v.y = (u32)((r32)vec.v.y * scale.v.y);
	return result;
}

internalf Vec2u32 vec2u32_normalize(Vec2u32 vec) {
	u32 length = vec2u32_norm(vec);
	Vec2u32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	return result;
}

internalf u32 vec2u32_norm(Vec2u32 vec) {
	return (u32)sqrt_r32((r32)(vec.v.x * vec.v.x + vec.v.y * vec.v.y));
}

internalf Vec2u32 vec2u32_nil(void) {
	Vec2u32 result;
	result.v.x = 0;
	result.v.y = 0;
	return result;
}

internalf Vec2u32 vec2u32_fill(u32 number) {
	Vec2u32 result;
	result.v.x = number;
	result.v.y = number;
	return result;
}

internalf b8 vec2u32_is_nil(Vec2u32 vec) {
	return (b8)((vec.v.x == 0) && (vec.v.y == 0));
}

internalf Vec2u32 vec2u32_add(Vec2u32 vec1, Vec2u32 vec2) {
	Vec2u32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	return result;
}

internalf Vec2u32 vec2u32_sub(Vec2u32 vec1, Vec2u32 vec2) {
	Vec2u32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	return result;
}

internalf Vec2u32 vec2u32_mul(Vec2u32 vec1, Vec2u32 vec2) {
	Vec2u32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	return result;
}

internalf Vec2u32 vec2u32_div(Vec2u32 vec1, Vec2u32 vec2) {
	Vec2u32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	return result;
}

internalf Vec2u32 vec2u32_lerp(Vec2u32 vec1, Vec2u32 vec2, Vec2r32 factor) {
	Vec2u32 result;
	result.v.x = vec1.v.x + (u32)((r32)(vec2.v.x - vec1.v.x) * factor.v.x);
	result.v.y = vec1.v.y + (u32)((r32)(vec2.v.y - vec1.v.y) * factor.v.y);
	return result;
}

internalf Vec2r32 vec2u32_unlerp(Vec2u32 start, Vec2u32 vec, Vec2u32 end) {
	Vec2r32 result;
	result.v.x = (r32)vec.v.x / (r32)(end.v.x - start.v.x);
	result.v.y = (r32)vec.v.y / (r32)(end.v.y - start.v.y);
	return result;
}

internalf u32 vec2u32_dot(Vec2u32 vec1, Vec2u32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y;
}

internalf u32 vec2u32_crossprod(Vec2u32 vec1, Vec2u32 vec2) {
	return vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
}

internalf r32 vec2u32_angle_undirected_radians(Vec2u32 vec1, Vec2u32 vec2) {
	r32 cosine = (r32)vec2u32_dot(vec1, vec2) / (r32)(vec2u32_norm(vec1) * vec2u32_norm(vec2));
	return acos_r32(cosine);
}

internalf r32 vec2u32_angle_directed_radians(Vec2u32 vec1, Vec2u32 vec2) {
	return atan2_r32((r32)vec2u32_crossprod(vec1, vec2), (r32)vec2u32_dot(vec1, vec2));
}

internalf b8 vec2u32_match(Vec2u32 vec1, Vec2u32 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y));
}

internalf u32 rng1u32_measure(Rng1u32 rng) {
	return rng.r.max - rng.r.min;
}

internalf u32 rng1u32_center(Rng1u32 rng) {
	return rng.r.min + (u32)((r32)(rng.r.max - rng.r.min) * 0.5f);
}

internalf Rng1u32 rng1u32_pad(Rng1u32 rng, Rng1u32 pad) {
	Rng1u32 result;
	result.r.min = rng.r.min - pad.r.min;
	result.r.max = rng.r.max + pad.r.max;
	return result;
}

internalf Rng1u32 rng1u32_clamp(Rng1u32 rng, Rng1u32 clamp) {
	Rng1u32 result;
	result.r.min = clamp(clamp.r.min, rng.r.min, clamp.r.max);
	result.r.max = clamp(clamp.r.min, rng.r.max, clamp.r.max);
	return result;
}

internalf b8 rng1u32_contains_u32(Rng1u32 rng, u32 number) {
	return (b8)((number >= rng.r.min) && (number <= rng.r.max));
}

internalf Rng1u32 rng1u32_union(Rng1u32 rng1, Rng1u32 rng2) {
	Rng1u32 result;
	result.r.min = min(rng1.r.min, rng2.r.min);
	result.r.max = max(rng1.r.max, rng2.r.max);
	return result;
}

internalf Rng1u32 rng1u32_intersect(Rng1u32 rng1, Rng1u32 rng2) {
	u32 min = max(rng1.r.min, rng2.r.min);
	u32 max = min(rng1.r.max, rng2.r.max);
	if (min > max) {
		min = 0;
		max = 0;
	}
	Rng1u32 result;
	result.r.min = min;
	result.r.max = max;
	return result;
}

internalf Rng1u32 rng1u32_clamp_rng1u32(Rng1u32 rng1, Rng1u32 rng2, Rng1u32 rng3) {
	Rng1u32 rng_intersect = rng1u32_intersect(rng1, rng2);
	Rng1u32 result;
	result.r.max = clamp(rng_intersect.r.min, rng3.r.max, rng_intersect.r.max);
	result.r.min = clamp(rng_intersect.r.min, rng3.r.min, rng_intersect.r.max);
	return result;
}


internalf Vec2r32 vec2r32_scale(Vec2r32 vec, Vec2r32 scale) {
	Vec2r32 result;
	result.v.x = vec.v.x * scale.v.x;
	result.v.y = vec.v.y * scale.v.y;
	return result;
}

internalf Vec2r32 vec2r32_scale_r32(Vec2r32 vec, r32 scale) {
	Vec2r32 result;
	result.v.x = vec.v.x * scale;
	result.v.y = vec.v.y * scale;
	return result;
}

internalf Vec2r32 vec2r32_normalize(Vec2r32 vec) {
	r32 length = vec2r32_norm(vec);
	Vec2r32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	return result;
}

internalf Vec2r32 vec2r32_abs(Vec2r32 vec) {
	Vec2r32 result;
	result.v.x = abs_r32(vec.v.x);
	result.v.y = abs_r32(vec.v.y);
	return result;
}

internalf Vec2r32 vec2r32_reverse(Vec2r32 vec) {
	Vec2r32 result;
	result.v.x = -1.0f * vec.v.x;
	result.v.y = -1.0f * vec.v.y;
	return result;
}

internalf r32 vec2r32_norm(Vec2r32 vec) {
	return sqrt_r32(vec.v.x * vec.v.x + vec.v.y * vec.v.y);
}

internalf Vec2r32 vec2r32_nil(void) {
	Vec2r32 result;
	result.v.x = 0.0f;
	result.v.y = 0.0f;
	return result;
}

internalf Vec2r32 vec2r32_fill(r32 number) {
	Vec2r32 result;
	result.v.x = number;
	result.v.y = number;
	return result;
}

internalf b8 vec2r32_is_nil(Vec2r32 vec) {
	return (b8)((vec.v.x == 0.0f) && (vec.v.y == 0.0f));
}

internalf Vec2r32 vec2r32_add(Vec2r32 vec1, Vec2r32 vec2) {
	Vec2r32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	return result;
}

internalf Vec2r32 vec2r32_sub(Vec2r32 vec1, Vec2r32 vec2) {
	Vec2r32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	return result;
}

internalf Vec2r32 vec2r32_mul(Vec2r32 vec1, Vec2r32 vec2) {
	Vec2r32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	return result;
}

internalf Vec2r32 vec2r32_div(Vec2r32 vec1, Vec2r32 vec2) {
	Vec2r32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	return result;
}

internalf Vec2r32 vec2r32_lerp(Vec2r32 vec1, Vec2r32 vec2, Vec2r32 factor) {
	Vec2r32 result;
	result.v.x = vec1.v.x + (vec2.v.x - vec1.v.x) * factor.v.x;
	result.v.y = vec1.v.y + (vec2.v.y - vec1.v.y) * factor.v.y;
	return result;
}

internalf Vec2r32 vec2r32_unlerp(Vec2r32 start, Vec2r32 vec, Vec2r32 end) {
	Vec2r32 result;
	result.v.x = vec.v.x / (end.v.x - start.v.x);
	result.v.y = vec.v.y / (end.v.y - start.v.y);
	return result;
}

internalf r32 vec2r32_dot(Vec2r32 vec1, Vec2r32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y;
}

internalf r32 vec2r32_crossprod(Vec2r32 vec1, Vec2r32 vec2) {
	return vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
}

internalf r32 vec2r32_angle_undirected_radians(Vec2r32 vec1, Vec2r32 vec2) {
	r32 cosine = vec2r32_dot(vec1, vec2) / (vec2r32_norm(vec1) * vec2r32_norm(vec2));
	return acos_r32(cosine);
}

internalf r32 vec2r32_angle_directed_radians(Vec2r32 vec1, Vec2r32 vec2) {
	return atan2_r32(vec2r32_crossprod(vec1, vec2), vec2r32_dot(vec1, vec2));
}

internalf b8 vec2r32_match(Vec2r32 vec1, Vec2r32 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y));
}

internalf r32 rng1r32_measure(Rng1r32 rng) {
	return rng.r.max - rng.r.min;
}

internalf r32 rng1r32_center(Rng1r32 rng) {
	return rng.r.min + (rng.r.max - rng.r.min) * 0.5f;
}

internalf Rng1r32 rng1r32_pad(Rng1r32 rng, Rng1r32 pad) {
	Rng1r32 result;
	result.r.min = rng.r.min - pad.r.min;
	result.r.max = rng.r.max + pad.r.max;
	return result;
}

internalf Rng1r32 rng1r32_clamp(Rng1r32 rng, Rng1r32 clamp) {
	Rng1r32 result;
	result.r.min = clamp(clamp.r.min, rng.r.min, clamp.r.max);
	result.r.max = clamp(clamp.r.min, rng.r.max, clamp.r.max);
	return result;
}

internalf b8 rng1r32_contains_r32(Rng1r32 rng, r32 number) {
	return (b8)((number >= rng.r.min) && (number <= rng.r.max));
}

internalf Rng1r32 rng1r32_union(Rng1r32 rng1, Rng1r32 rng2) {
	Rng1r32 result;
	result.r.min = min(rng1.r.min, rng2.r.min);
	result.r.max = max(rng1.r.max, rng2.r.max);
	return result;
}

internalf Rng1r32 rng1r32_intersect(Rng1r32 rng1, Rng1r32 rng2) {
	r32 min = max(rng1.r.min, rng2.r.min);
	r32 max = min(rng1.r.max, rng2.r.max);
	if (min > max) {
		min = 0.0f;
		max = 0.0f;
	}
	Rng1r32 result;
	result.r.min = min;
	result.r.max = max;
	return result;
}

internalf Rng1r32 rng1r32_clamp_rng1r32(Rng1r32 rng1, Rng1r32 rng2, Rng1r32 rng3) {
	Rng1r32 rng_intersect = rng1r32_intersect(rng1, rng2);
	Rng1r32 result;
	result.r.max = clamp(rng_intersect.r.min, rng3.r.max, rng_intersect.r.max);
	result.r.min = clamp(rng_intersect.r.min, rng3.r.min, rng_intersect.r.max);
	return result;
}


internalf Vec2r64 vec2r64_scale(Vec2r64 vec, Vec2r64 scale) {
	Vec2r64 result;
	result.v.x = vec.v.x * scale.v.x;
	result.v.y = vec.v.y * scale.v.y;
	return result;
}

internalf Vec2r64 vec2r64_normalize(Vec2r64 vec) {
	r64 length = vec2r64_norm(vec);
	Vec2r64 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	return result;
}

internalf Vec2r64 vec2r64_reverse(Vec2r64 vec) {
	Vec2r64 result;
	result.v.x = -1 * vec.v.x;
	result.v.y = -1 * vec.v.y;
	return result;
}

internalf r64 vec2r64_norm(Vec2r64 vec) {
	return sqrt_r64(vec.v.x * vec.v.x + vec.v.y * vec.v.y);
}

internalf Vec2r64 vec2r64_nil(void) {
	Vec2r64 result;
	result.v.x = 0.0;
	result.v.y = 0.0;
	return result;
}

internalf Vec2r64 vec2r64_fill(r64 number) {
	Vec2r64 result;
	result.v.x = number;
	result.v.y = number;
	return result;
}

internalf b8 vec2r64_is_nil(Vec2r64 vec) {
	return (b8)((vec.v.x == 0.0) && (vec.v.y == 0.0));
}

internalf Vec2r64 vec2r64_add(Vec2r64 vec1, Vec2r64 vec2) {
	Vec2r64 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	return result;
}

internalf Vec2r64 vec2r64_sub(Vec2r64 vec1, Vec2r64 vec2) {
	Vec2r64 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	return result;
}

internalf Vec2r64 vec2r64_mul(Vec2r64 vec1, Vec2r64 vec2) {
	Vec2r64 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	return result;
}

internalf Vec2r64 vec2r64_div(Vec2r64 vec1, Vec2r64 vec2) {
	Vec2r64 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	return result;
}

internalf Vec2r64 vec2r64_lerp(Vec2r64 vec1, Vec2r64 vec2, Vec2r64 factor) {
	Vec2r64 result;
	result.v.x = vec1.v.x + (vec2.v.x - vec1.v.x) * factor.v.x;
	result.v.y = vec1.v.y + (vec2.v.y - vec1.v.y) * factor.v.y;
	return result;
}

internalf Vec2r64 vec2r64_unlerp(Vec2r64 start, Vec2r64 vec, Vec2r64 end) {
	Vec2r64 result;
	result.v.x = vec.v.x / (end.v.x - start.v.x);
	result.v.y = vec.v.y / (end.v.y - start.v.y);
	return result;
}

internalf r64 vec2r64_dot(Vec2r64 vec1, Vec2r64 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y;
}

internalf r64 vec2r64_crossprod(Vec2r64 vec1, Vec2r64 vec2) {
	return vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
}

internalf r64 vec2r64_angle_undirected_radians(Vec2r64 vec1, Vec2r64 vec2) {
	r64 cosine = vec2r64_dot(vec1, vec2) / (vec2r64_norm(vec1) * vec2r64_norm(vec2));
	return acos_r64(cosine);
}

internalf r64 vec2r64_angle_directed_radians(Vec2r64 vec1, Vec2r64 vec2) {
	return atan2_r64(vec2r64_crossprod(vec1, vec2), vec2r64_dot(vec1, vec2));
}

internalf b8 vec2r64_match(Vec2r64 vec1, Vec2r64 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y));
}

internalf r64 rng1r64_measure(Rng1r64 rng) {
	return rng.r.max - rng.r.min;
}

internalf r64 rng1r64_center(Rng1r64 rng) {
	return rng.r.min + (rng.r.max - rng.r.min) * 0.5;
}

internalf Rng1r64 rng1r64_pad(Rng1r64 rng, Rng1r64 pad) {
	Rng1r64 result;
	result.r.min = rng.r.min - pad.r.min;
	result.r.max = rng.r.max + pad.r.max;
	return result;
}

internalf Rng1r64 rng1r64_clamp(Rng1r64 rng, Rng1r64 clamp) {
	Rng1r64 result;
	result.r.min = clamp(clamp.r.min, rng.r.min, clamp.r.max);
	result.r.max = clamp(clamp.r.min, rng.r.max, clamp.r.max);
	return result;
}

internalf b8 rng1r64_contains_r64(Rng1r64 rng, r64 number) {
	return (b8)((number >= rng.r.min) && (number <= rng.r.max));
}

internalf Rng1r64 rng1r64_union(Rng1r64 rng1, Rng1r64 rng2) {
	Rng1r64 result;
	result.r.min = min(rng1.r.min, rng2.r.min);
	result.r.max = max(rng1.r.max, rng2.r.max);
	return result;
}

internalf Rng1r64 rng1r64_intersect(Rng1r64 rng1, Rng1r64 rng2) {
	r64 min = max(rng1.r.min, rng2.r.min);
	r64 max = min(rng1.r.max, rng2.r.max);
	if (min > max) {
		min = 0.0;
		max = 0.0;
	}
	Rng1r64 result;
	result.r.min = min;
	result.r.max = max;
	return result;
}

internalf Rng1r64 rng1r64_clamp_rng1r64(Rng1r64 rng1, Rng1r64 rng2, Rng1r64 rng3) {
	Rng1r64 rng_intersect = rng1r64_intersect(rng1, rng2);
	Rng1r64 result;
	result.r.max = clamp(rng_intersect.r.min, rng3.r.max, rng_intersect.r.max);
	result.r.min = clamp(rng_intersect.r.min, rng3.r.min, rng_intersect.r.max);
	return result;
}
// -------- Vec2/Rng1

// -------- Vec3
internalf Vec3s32 vec3s32_scale(Vec3s32 vec, Vec3r32 scale) {
	Vec3s32 result;
	result.v.x = (s32)((r32)vec.v.x * scale.v.x);
	result.v.y = (s32)((r32)vec.v.y * scale.v.y);
	result.v.z = (s32)((r32)vec.v.z * scale.v.z);
	return result;
}

internalf Vec3s32 vec3s32_normalize(Vec3s32 vec) {
	s32 length = vec3s32_norm(vec);
	Vec3s32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	result.v.z = vec.v.z / length;
	return result;
}

internalf Vec3s32 vec3s32_reverse(Vec3s32 vec) {
	Vec3s32 result;
	result.v.x = -1 * vec.v.x;
	result.v.y = -1 * vec.v.y;
	result.v.z = -1 * vec.v.z;
	return result;
}

internalf s32 vec3s32_norm(Vec3s32 vec) {
	return (s32)sqrt_r32((r32)(vec.v.x * vec.v.x + vec.v.y * vec.v.y + vec.v.z * vec.v.z));
}

internalf Vec3s32 vec3s32_nil(void) {
	Vec3s32 result;
	result.v.x = 0; 
	result.v.y = 0;
	result.v.z = 0;
	return result;
}

internalf Vec3s32 vec3s32_fill(s32 number) {
	Vec3s32 result;
	result.v.x = number;
	result.v.y = number;
	result.v.z = number;
	return result;
}

internalf b8 vec3s32_is_nil(Vec3s32 vec) {
	return (vec.v.x == 0) && (vec.v.y == 0) && (vec.v.z == 0);
}

internalf Vec3s32 vec3s32_add(Vec3s32 vec1, Vec3s32 vec2) {
	Vec3s32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	result.v.z = vec1.v.z + vec2.v.z;
	return result;
}

internalf Vec3s32 vec3s32_sub(Vec3s32 vec1, Vec3s32 vec2) {
	Vec3s32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	result.v.z = vec1.v.z - vec2.v.z;
	return result;
}

internalf Vec3s32 vec3s32_mul(Vec3s32 vec1, Vec3s32 vec2) {
	Vec3s32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	result.v.z = vec1.v.z * vec2.v.z;
	return result;
}

internalf Vec3s32 vec3s32_div(Vec3s32 vec1, Vec3s32 vec2) {
	Vec3s32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	result.v.z = vec1.v.z / vec2.v.z;
	return result;
}

internalf Vec3s32 vec3s32_lerp(Vec3s32 vec1, Vec3s32 vec2, Vec3r32 factor) {
	Vec3s32 result;
	result.v.x = vec1.v.x + (s32)((r32)(vec2.v.x - vec1.v.x) * factor.v.x);
	result.v.y = vec1.v.y + (s32)((r32)(vec2.v.y - vec1.v.y) * factor.v.y);
	result.v.z = vec1.v.z + (s32)((r32)(vec2.v.z - vec1.v.z) * factor.v.z);
	return result;
}

internalf Vec3r32 vec3s32_unlerp(Vec3s32 start, Vec3s32 vec, Vec3s32 end) {
	Vec3r32 result;
	result.v.x = (r32)vec.v.x / (r32)(end.v.x - start.v.x);
	result.v.y = (r32)vec.v.y / (r32)(end.v.y - start.v.y);
	result.v.z = (r32)vec.v.z / (r32)(end.v.z - start.v.z);
	return result;
}

internalf s32 vec3s32_dot(Vec3s32 vec1, Vec3s32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y + vec1.v.z * vec2.v.z;
}

internalf Vec3s32 vec3s32_crossprod(Vec3s32 vec1, Vec3s32 vec2) {
	Vec3s32 result;
	result.v.x = vec1.v.y * vec2.v.z - vec1.v.z * vec2.v.y;
	result.v.y = vec1.v.z * vec2.v.x - vec1.v.x * vec2.v.z;
	result.v.z = vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
	return result;
}

internalf r32 vec3s32_angle_undirected_radians(Vec3s32 vec1, Vec3s32 vec2) {
	r32 cosine = (r32)vec3s32_dot(vec1, vec2) / (r32)(vec3s32_norm(vec1) * vec3s32_norm(vec2));
	return acos_r32(cosine);
}


internalf r32 vec3s32_angle_directed_radians(Vec3s32 vec1, Vec3s32 vec2, Vec3s32 normal) {
	Vec3s32 crossprod = vec3s32_crossprod(vec1, vec2);
  
	if(vec3s32_is_nil(normal)) {
		s32 normal_norm = vec3s32_norm(crossprod);
		normal.v.x = crossprod.v.x / normal_norm;
		normal.v.y = crossprod.v.y / normal_norm;
		normal.v.z = crossprod.v.z / normal_norm;
	}
  
	return atan2_r32((r32)vec3s32_dot(crossprod, normal), (r32)vec3s32_dot(vec1, vec2));
}

internalf b8 vec3s32_match(Vec3s32 vec1, Vec3s32 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y) && (vec1.v.z == vec2.v.z));
}


internalf Vec3u32 vec3u32_scale(Vec3u32 vec, Vec3r32 scale) {
	Vec3u32 result;
	result.v.x = (u32)((r32)vec.v.x * scale.v.x);
	result.v.y = (u32)((r32)vec.v.y * scale.v.y);
	result.v.z = (u32)((r32)vec.v.z * scale.v.z);
	return result;
}

internalf Vec3u32 vec3u32_normalize(Vec3u32 vec) {
	u32 length = vec3u32_norm(vec);
	Vec3u32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	result.v.z = vec.v.z / length;
	return result;
}

internalf u32 vec3u32_norm(Vec3u32 vec) {
	return (u32)sqrt_r32((r32)(vec.v.x * vec.v.x + vec.v.y * vec.v.y + vec.v.z * vec.v.z));
}

internalf Vec3u32 vec3u32_nil(void) {
	Vec3u32 result;
	result.v.x = 0;
	result.v.y = 0;
	result.v.z = 0;
	return result;
}

internalf Vec3u32 vec3u32_fill(u32 number) {
	Vec3u32 result;
	result.v.x = number;
	result.v.y = number;
	result.v.z = number;
	return result;
}

internalf b8 vec3u32_is_nil(Vec3u32 vec) {
	return (vec.v.x == 0) && (vec.v.y == 0) && (vec.v.z == 0);
}

internalf Vec3u32 vec3u32_add(Vec3u32 vec1, Vec3u32 vec2) {
	Vec3u32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	result.v.z = vec1.v.z + vec2.v.z;
	return result;
}

internalf Vec3u32 vec3u32_sub(Vec3u32 vec1, Vec3u32 vec2) {
	Vec3u32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	result.v.z = vec1.v.z - vec2.v.z;
	return result;
}

internalf Vec3u32 vec3u32_mul(Vec3u32 vec1, Vec3u32 vec2) {
	Vec3u32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	result.v.z = vec1.v.z * vec2.v.z;
	return result;
}

internalf Vec3u32 vec3u32_div(Vec3u32 vec1, Vec3u32 vec2) {
	Vec3u32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	result.v.z = vec1.v.z / vec2.v.z;
	return result;
}

internalf Vec3u32 vec3u32_lerp(Vec3u32 vec1, Vec3u32 vec2, Vec3r32 factor) {
	Vec3u32 result;
	result.v.x = vec1.v.x + (u32)((r32)(vec2.v.x - vec1.v.x) * factor.v.x);
	result.v.y = vec1.v.y + (u32)((r32)(vec2.v.y - vec1.v.y) * factor.v.y);
	result.v.z = vec1.v.z + (u32)((r32)(vec2.v.z - vec1.v.z) * factor.v.z);
	return result;
}

internalf Vec3r32 vec3u32_unlerp(Vec3u32 start, Vec3u32 vec, Vec3u32 end) {
	Vec3r32 result;
	result.v.x = (r32)vec.v.x / (r32)(end.v.x - start.v.x);
	result.v.y = (r32)vec.v.y / (r32)(end.v.y - start.v.y);
	result.v.z = (r32)vec.v.z / (r32)(end.v.z - start.v.z);
	return result;
}

internalf u32 vec3u32_dot(Vec3u32 vec1, Vec3u32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y + vec1.v.z * vec2.v.z;
}

internalf Vec3u32 vec3u32_crossprod(Vec3u32 vec1, Vec3u32 vec2) {
	Vec3u32 result;
	result.v.x = vec1.v.y * vec2.v.z - vec1.v.z * vec2.v.y;
	result.v.y = vec1.v.z * vec2.v.x - vec1.v.x * vec2.v.z;
	result.v.z = vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
	return result;
}

internalf r32 vec3u32_angle_undirected_radians(Vec3u32 vec1, Vec3u32 vec2) {
	r32 cosine = (r32)vec3u32_dot(vec1, vec2) / (r32)(vec3u32_norm(vec1) * vec3u32_norm(vec2));
	return acos_r32(cosine);
}

internalf r32 vec3u32_angle_directed_radians(Vec3u32 vec1, Vec3u32 vec2, Vec3u32 normal) {
	Vec3u32 crossprod = vec3u32_crossprod(vec1, vec2);
  
	if(vec3u32_is_nil(normal)) {
		u32 normal_norm = vec3u32_norm(crossprod);
		normal.v.x = crossprod.v.x / normal_norm;
		normal.v.y = crossprod.v.y / normal_norm;
		normal.v.z = crossprod.v.z / normal_norm;
	}
  
	return atan2_r32((r32)vec3u32_dot(crossprod, normal), (r32)vec3u32_dot(vec1, vec2));
}

internalf b8 vecs3u32_match(Vec3u32 vec1, Vec3u32 vec2) {
	return (b8)(vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y) && (vec1.v.z == vec2.v.z);
}


internalf Vec3r32 vec3r32_scale(Vec3r32 vec, Vec3r32 scale) {
	Vec3r32 result;
	result.v.x = vec.v.x * scale.v.x;
	result.v.y = vec.v.y * scale.v.y;
	result.v.z = vec.v.z * scale.v.z;
	return result;
}

internalf Vec3r32 vec3r32_normalize(Vec3r32 vec) {
	r32 length = vec3r32_norm(vec);
	Vec3r32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	result.v.z = vec.v.z / length;
	return result;
}

internalf Vec3r32 vec3r32_reverse(Vec3r32 vec) {
	Vec3r32 result;
	result.v.x = -1.0f * vec.v.x;
	result.v.y = -1.0f * vec.v.y;
	result.v.z = -1.0f * vec.v.z;
	return result;
}

internalf r32 vec3r32_norm(Vec3r32 vec) {
	return sqrt_r32(vec.v.x * vec.v.x + vec.v.y * vec.v.y + vec.v.z * vec.v.z);
}

internalf Vec3r32 vec3r32_nil(void) {
	Vec3r32 result;
	result.v.x = 0.0f;
	result.v.y = 0.0f;
	result.v.z = 0.0f;
	return result;
}

internalf Vec3r32 vec3r32_fill(r32 number) {
	Vec3r32 result;
	result.v.x = number;
	result.v.y = number;
	result.v.z = number;
	return result;
}

internalf b8 vec3r32_is_nil(Vec3r32 vec) {
	return (vec.v.x == 0.0f) && (vec.v.y == 0.0f) && (vec.v.z == 0.0f);
}

internalf Vec3r32 vec3r32_add(Vec3r32 vec1, Vec3r32 vec2) {
	Vec3r32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	result.v.z = vec1.v.z + vec2.v.z;
	return result;
}

internalf Vec3r32 vec3r32_sub(Vec3r32 vec1, Vec3r32 vec2) {
	Vec3r32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	result.v.z = vec1.v.z - vec2.v.z;
	return result;
}

internalf Vec3r32 vec3r32_mul(Vec3r32 vec1, Vec3r32 vec2) {
	Vec3r32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	result.v.z = vec1.v.z * vec2.v.z;
	return result;
}

internalf Vec3r32 vec3r32_div(Vec3r32 vec1, Vec3r32 vec2) {
	Vec3r32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	result.v.z = vec1.v.z / vec2.v.z;
	return result;
}

internalf Vec3r32 vec3r32_lerp(Vec3r32 vec1, Vec3r32 vec2, Vec3r32 factor) {
	Vec3r32 result;
	result.v.x = vec1.v.x + (vec2.v.x - vec1.v.x) * factor.v.x;
	result.v.y = vec1.v.y + (vec2.v.y - vec1.v.y) * factor.v.y;
	result.v.z = vec1.v.z + (vec2.v.z - vec1.v.z) * factor.v.z;
	return result;
}

internalf Vec3r32 vec3r32_unlerp(Vec3r32 start, Vec3r32 vec, Vec3r32 end) {
	Vec3r32 result;
	result.v.x = vec.v.x / (end.v.x - start.v.x);
	result.v.y = vec.v.y / (end.v.y - start.v.y);
	result.v.z = vec.v.z / (end.v.z - start.v.z);
	return result;
}

internalf r32 vec3r32_dot(Vec3r32 vec1, Vec3r32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y + vec1.v.z * vec2.v.z;
}

internalf Vec3r32 vec3r32_crossprod(Vec3r32 vec1, Vec3r32 vec2) {
	Vec3r32 result;
	result.v.x = vec1.v.y * vec2.v.z - vec1.v.z * vec2.v.y;
	result.v.y = vec1.v.z * vec2.v.x - vec1.v.x * vec2.v.z;
	result.v.z = vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
	return result;
}

internalf r32 vec3r32_angle_undirected_radians(Vec3r32 vec1, Vec3r32 vec2) {
	r32 cosine = vec3r32_dot(vec1, vec2) / (vec3r32_norm(vec1) * vec3r32_norm(vec2));
	return acos_r32(cosine);
}

internalf r32 vec3r32_angle_directed_radians(Vec3r32 vec1, Vec3r32 vec2, Vec3r32 normal) {
	Vec3r32 crossprod = vec3r32_crossprod(vec1, vec2);
  
	if(vec3r32_is_nil(normal)) {
		r32 normal_norm = vec3r32_norm(crossprod);
		normal.v.x = crossprod.v.x / normal_norm;
		normal.v.y = crossprod.v.y / normal_norm;
		normal.v.z = crossprod.v.z / normal_norm;
	}
  
	return atan2_r32(vec3r32_dot(crossprod, normal), vec3r32_dot(vec1, vec2));
}

internalf b8 vecs3r32_match(Vec3r32 vec1, Vec3r32 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y) && (vec1.v.z == vec2.v.z));
}


internalf Vec3r64 vec3r64_scale(Vec3r64 vec, Vec3r64 scale) {
	Vec3r64 result;
	result.v.x = vec.v.x * scale.v.x;
	result.v.y = vec.v.y * scale.v.y;
	result.v.z = vec.v.z * scale.v.z;
	return result;
}

internalf Vec3r64 vec3r64_normalize(Vec3r64 vec) {
	r64 length = vec3r64_norm(vec);
	Vec3r64 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	result.v.z = vec.v.z / length;
	return result;
}

internalf Vec3r64 vec3r64_reverse(Vec3r64 vec) {
	Vec3r64 result;
	result.v.x = -1.0 * vec.v.x;
	result.v.y = -1.0 * vec.v.y;
	result.v.z = -1.0 * vec.v.z;
	return result;
}

internalf r64 vec3r64_norm(Vec3r64 vec) {
	return sqrt_r64(vec.v.x * vec.v.x + vec.v.y * vec.v.y + vec.v.z * vec.v.z);
}

internalf Vec3r64 vec3r64_nil(void) {
	Vec3r64 result;
	result.v.x = 0.0;
	result.v.y = 0.0;
	result.v.z = 0.0;
	return result;
}

internalf Vec3r64 vec3r64_fill(r64 number) {
	Vec3r64 result;
	result.v.x = number;
	result.v.y = number;
	result.v.z = number;
	return result;
}

internalf b8 vec3r64_is_nil(Vec3r64 vec) {
	return (vec.v.x == 0.0) && (vec.v.y == 0.0) && (vec.v.z == 0.0);
}

internalf Vec3r64 vec3r64_add(Vec3r64 vec1, Vec3r64 vec2) {
	Vec3r64 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	result.v.z = vec1.v.z + vec2.v.z;
	return result;
}

internalf Vec3r64 vec3r64_sub(Vec3r64 vec1, Vec3r64 vec2) {
	Vec3r64 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	result.v.z = vec1.v.z - vec2.v.z;
	return result;
}

internalf Vec3r64 vec3r64_mul(Vec3r64 vec1, Vec3r64 vec2) {
	Vec3r64 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	result.v.z = vec1.v.z * vec2.v.z;
	return result;
}

internalf Vec3r64 vec3r64_div(Vec3r64 vec1, Vec3r64 vec2) {
	Vec3r64 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	result.v.z = vec1.v.z / vec2.v.z;
	return result;
}

internalf Vec3r64 vec3r64_lerp(Vec3r64 vec1, Vec3r64 vec2, Vec3r64 factor) {
	Vec3r64 result;
	result.v.x = vec1.v.x + (vec2.v.x - vec1.v.x) * factor.v.x;
	result.v.y = vec1.v.y + (vec2.v.y - vec1.v.y) * factor.v.y;
	result.v.z = vec1.v.z + (vec2.v.z - vec1.v.z) * factor.v.z;
	return result;
}

internalf Vec3r64 vec3r64_unlerp(Vec3r64 start, Vec3r64 vec, Vec3r64 end) {
	Vec3r64 result;
	result.v.x = vec.v.x / (end.v.x - start.v.x);
	result.v.y = vec.v.y / (end.v.y - start.v.y);
	result.v.z = vec.v.z / (end.v.z - start.v.z);
	return result;
}

internalf r64 vec3r64_dot(Vec3r64 vec1, Vec3r64 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y + vec1.v.z * vec2.v.z;
}

internalf Vec3r64 vec3r64_crossprod(Vec3r64 vec1, Vec3r64 vec2) {
	Vec3r64 result;
	result.v.x = vec1.v.y * vec2.v.z - vec1.v.z * vec2.v.y;
	result.v.y = vec1.v.z * vec2.v.x - vec1.v.x * vec2.v.z;
	result.v.z = vec1.v.x * vec2.v.y - vec1.v.y * vec2.v.x;
	return result;
}

internalf r64 vec3r64_angle_undirected_radians(Vec3r64 vec1, Vec3r64 vec2) {
	r64 cosine = vec3r64_dot(vec1, vec2) / (vec3r64_norm(vec1) * vec3r64_norm(vec2));
	return acos_r64(cosine);
}

internalf r64 vec3r64_angle_directed_radians(Vec3r64 vec1, Vec3r64 vec2, Vec3r64 normal) {
	Vec3r64 crossprod = vec3r64_crossprod(vec1, vec2);
  
	if(vec3r64_is_nil(normal)) {
		r64 normal_norm = vec3r64_norm(crossprod);
		normal.v.x = crossprod.v.x / normal_norm;
		normal.v.y = crossprod.v.y / normal_norm;
		normal.v.z = crossprod.v.z / normal_norm;
	}
  
	return atan2_r64(vec3r64_dot(crossprod, normal), vec3r64_dot(vec1, vec2));
}

internalf b8 vec3r64_match(Vec3r64 vec1, Vec3r64 vec2) {
	return (b8)(vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y) && (vec1.v.z == vec2.v.z);
}
// -------- Vec3

// -------- Vec4/Rng2
internalf Vec4s32 vec4s32_scale(Vec4s32 vec, Vec4r32 scale) {
	Vec4s32 result;
	result.v.x = (s32)((r32)vec.v.x * scale.v.x);
	result.v.y = (s32)((r32)vec.v.y * scale.v.y);
	result.v.z = (s32)((r32)vec.v.z * scale.v.z);
	result.v.w = (s32)((r32)vec.v.w * scale.v.w);
	return result;
}

internalf Vec4s32 vec4s32_normalize(Vec4s32 vec) {
	s32 length = vec4s32_norm(vec);
	Vec4s32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	result.v.z = vec.v.z / length;
	result.v.w = vec.v.w / length;
	return result;
}

internalf Vec4s32 vec4s32_reverse(Vec4s32 vec) {
	Vec4s32 result;
	result.v.x = -1 * vec.v.x;
	result.v.y = -1 * vec.v.y;
	result.v.z = -1 * vec.v.z;
	result.v.w = -1 * vec.v.w;
	return result;
}

internalf s32 vec4s32_norm(Vec4s32 vec) {
	return (s32)sqrt_r32((r32)(vec.v.x * vec.v.x + vec.v.y * vec.v.y + vec.v.z * vec.v.z + vec.v.w * vec.v.w));
}

internalf Vec4s32 vec4s32_nil(void) {
	Vec4s32 result;
	result.v.x = 0;
	result.v.y = 0;
	result.v.z = 0;
	result.v.w = 0;
	return result;
}

internalf Vec4s32 vec4s32_fill(s32 number) {
	Vec4s32 result;
	result.v.x = number;
	result.v.y = number;
	result.v.z = number;
	result.v.w = number;
	return result;
}

internalf b8 vec4s32_is_nil(Vec4s32 vec) {
	return (b8)((vec.v.x == 0) && (vec.v.y == 0) && (vec.v.z == 0) && (vec.v.w == 0));
}

internalf Vec4s32 vec4s32_add(Vec4s32 vec1, Vec4s32 vec2) {
	Vec4s32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	result.v.z = vec1.v.z + vec2.v.z;
	result.v.w = vec1.v.w + vec2.v.w;
	return result;
}

internalf Vec4s32 vec4s32_sub(Vec4s32 vec1, Vec4s32 vec2) {
	Vec4s32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	result.v.z = vec1.v.z - vec2.v.z;
	result.v.w = vec1.v.w - vec2.v.w;
	return result;
}

internalf Vec4s32 vec4s32_mul(Vec4s32 vec1, Vec4s32 vec2) {
	Vec4s32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	result.v.z = vec1.v.z * vec2.v.z;
	result.v.w = vec1.v.w * vec2.v.w;
	return result;
}

internalf Vec4s32 vec4s32_div(Vec4s32 vec1, Vec4s32 vec2) {
	Vec4s32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	result.v.z = vec1.v.z / vec2.v.z;
	result.v.w = vec1.v.w / vec2.v.w;
	return result;
}

internalf Vec4s32 vec4s32_lerp(Vec4s32 start, Vec4s32 end, Vec4r32 factor) {
	Vec4s32 result;
	result.v.x = start.v.x + (s32)((r32)(end.v.x - start.v.x) * factor.v.x);
	result.v.y = start.v.y + (s32)((r32)(end.v.y - start.v.y) * factor.v.y);
	result.v.z = start.v.z + (s32)((r32)(end.v.z - start.v.z) * factor.v.z);
	result.v.w = start.v.w + (s32)((r32)(end.v.w - start.v.w) * factor.v.w);
	return result;
}

internalf Vec4r32 vec4s32_unlerp(Vec4s32 start, Vec4s32 vec, Vec4s32 end) {
	Vec4r32 result;
	result.v.x = (r32)vec.v.x / (r32)(end.v.x - start.v.x);
	result.v.y = (r32)vec.v.y / (r32)(end.v.y - start.v.y);
	result.v.z = (r32)vec.v.z / (r32)(end.v.z - start.v.z);
	result.v.w = (r32)vec.v.w / (r32)(end.v.w - start.v.w);
	return result;
}

internalf s32 vec4s32_dot(Vec4s32 vec1, Vec4s32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y + vec1.v.z * vec2.v.z + vec1.v.w * vec2.v.w;
}

internalf r32 vec4s32_angle_undirected_radians(Vec4s32 vec1, Vec4s32 vec2) {
	r32 cosine = (r32)vec4s32_dot(vec1, vec2) / (r32)(vec4s32_norm(vec1) * vec4s32_norm(vec2));
	return acos_r32(cosine);
}

internalf b8 vec4s32_match(Vec4s32 vec1, Vec4s32 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y) && (vec1.v.z == vec2.v.z) && (vec1.v.w == vec2.v.w));
}

internalf Vec2s32 rng2s32_measure(Rng2s32 rng) {
	Vec2s32 result;
	result.v.x = rng.r.max.v.x - rng.r.min.v.x;
	result.v.y = rng.r.max.v.y - rng.r.min.v.y;
	return result;
}

internalf Vec2s32 rng2s32_center(Rng2s32 rng) {
	Vec2s32 result;
	result.v.x = rng.r.min.v.x + (s32)((r32)(rng.r.max.v.x - rng.r.min.v.x) * 0.5f);
	result.v.y = rng.r.min.v.y + (s32)((r32)(rng.r.max.v.y - rng.r.min.v.y) * 0.5f);
	return result;
}

internalf Rng2s32 rng2s32_pad(Rng2s32 rng, Rng2s32 pad) {
	Rng2s32 result;
	result.r.min.v.x = rng.r.min.v.x + pad.r.min.v.x;
	result.r.min.v.y = rng.r.min.v.y + pad.r.min.v.y;
	result.r.max.v.x = rng.r.max.v.x + pad.r.max.v.x;
	result.r.max.v.y = rng.r.max.v.y + pad.r.max.v.y;
	return result;
}

internalf Rng2s32 rng2s32_clamp(Rng2s32 rng, Rng2s32 clamp) {
	Rng2s32 result;
	result.r.min.v.x = clamp(clamp.r.min.v.x, rng.r.min.v.x, clamp.r.max.v.x);
	result.r.min.v.y = clamp(clamp.r.min.v.y, rng.r.min.v.y, clamp.r.max.v.y);
	result.r.max.v.x = clamp(clamp.r.min.v.x, rng.r.max.v.x, clamp.r.max.v.x);
	result.r.max.v.y = clamp(clamp.r.min.v.y, rng.r.max.v.y, clamp.r.max.v.y);
	return result;
}

internalf b8 rng2s32_contains_vec2s32(Rng2s32 rng, Vec2s32 vec) {
	return (b8)((vec.v.x >= rng.r.min.v.x) && (vec.v.x <= rng.r.max.v.x) && (vec.v.y >= rng.r.min.v.y) && (vec.v.y <= rng.r.max.v.y));
}

internalf Rng2s32 rng2s32_union(Rng2s32 rng1, Rng2s32 rng2) {
	Rng2s32 result;
	result.r.min.v.x = min(rng1.r.min.v.x, rng2.r.min.v.x);
	result.r.min.v.y = min(rng1.r.min.v.y, rng2.r.min.v.y);
	result.r.max.v.x = max(rng1.r.max.v.x, rng2.r.max.v.x);
	result.r.max.v.y = max(rng1.r.max.v.y, rng2.r.max.v.y);
	return result;
}

internalf Rng2s32 rng2s32_intersect(Rng2s32 rng1, Rng2s32 rng2) {
	Rng2s32 intersect;
	intersect.r.min.v.x = max(rng1.r.min.v.x, rng2.r.min.v.x);
	intersect.r.min.v.y = max(rng1.r.min.v.y, rng2.r.min.v.y);
	intersect.r.max.v.x = min(rng1.r.max.v.x, rng2.r.max.v.x);
	intersect.r.max.v.y = min(rng1.r.max.v.y, rng2.r.max.v.y);

	if(intersect.r.min.v.x > intersect.r.max.v.x || intersect.r.max.v.x > intersect.r.max.v.y) {
		intersect.r.min.v.x = 0;
		intersect.r.min.v.y = 0;
		intersect.r.max.v.x = 0;
		intersect.r.max.v.y = 0;
	}
  
	return intersect;
}

internalf Rng2s32 rng2s32_clamp_rng2s32(Rng2s32 rng1, Rng2s32 rng2, Rng2s32 rng3) {
	Rng2s32 intersect = rng2s32_intersect(rng1, rng2);
	Rng2s32 result;
	result.r.min.v.x = clamp(intersect.r.min.v.x, rng3.r.min.v.x, intersect.r.max.v.x);
	result.r.max.v.x = clamp(intersect.r.min.v.x, rng3.r.max.v.x, intersect.r.max.v.x);
	result.r.min.v.y = clamp(intersect.r.min.v.y, rng3.r.min.v.y, intersect.r.max.v.y);
	result.r.max.v.y = clamp(intersect.r.min.v.y, rng3.r.max.v.y, intersect.r.max.v.y);  
	return result;
}


internalf Vec4u32 vec4u32_scale(Vec4u32 vec, Vec4r32 scale) {
	Vec4u32 result;
	result.v.x = (u32)((r32)vec.v.x * scale.v.x);
	result.v.y = (u32)((r32)vec.v.y * scale.v.y);
	result.v.z = (u32)((r32)vec.v.z * scale.v.z);
	result.v.w = (u32)((r32)vec.v.w * scale.v.w);
	return result;
}

internalf Vec4u32 vec4u32_normalize(Vec4u32 vec) {
	u32 length = vec4u32_norm(vec);
	Vec4u32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	result.v.z = vec.v.z / length;
	result.v.w = vec.v.w / length;
	return result;
}

internalf u32 vec4u32_norm(Vec4u32 vec) {
	return (u32)sqrt_r32((r32)(vec.v.x * vec.v.x + vec.v.y * vec.v.y + vec.v.z * vec.v.z + vec.v.w * vec.v.w));
}

internalf Vec4u32 vec4u32_nil(void) {
	Vec4u32 result;
	result.v.x = 0;
	result.v.y = 0;
	result.v.z = 0;
	result.v.w = 0;
	return result;
}

internalf Vec4u32 vec4u32_fill(u32 number) {
	Vec4u32 result;
	result.v.x = number;
	result.v.y = number;
	result.v.z = number;
	result.v.w = number;
	return result;
}

internalf b8 vec4u32_is_nil(Vec4u32 vec) {
	return (b8)((vec.v.x == 0) && (vec.v.y == 0) && (vec.v.z == 0) && (vec.v.w == 0));
}

internalf Vec4u32 vec4u32_add(Vec4u32 vec1, Vec4u32 vec2) {
	Vec4u32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	result.v.z = vec1.v.z + vec2.v.z;
	result.v.w = vec1.v.w + vec2.v.w;
	return result;
}

internalf Vec4u32 vec4u32_sub(Vec4u32 vec1, Vec4u32 vec2) {
	Vec4u32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	result.v.z = vec1.v.z - vec2.v.z;
	result.v.w = vec1.v.w - vec2.v.w;
	return result;
}

internalf Vec4u32 vec4u32_mul(Vec4u32 vec1, Vec4u32 vec2) {
	Vec4u32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	result.v.z = vec1.v.z * vec2.v.z;
	result.v.w = vec1.v.w * vec2.v.w;
	return result;
}

internalf Vec4u32 vec4u32_div(Vec4u32 vec1, Vec4u32 vec2) {
	Vec4u32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	result.v.z = vec1.v.z / vec2.v.z;
	result.v.w = vec1.v.w / vec2.v.w;
	return result;
}

internalf Vec4u32 vec4u32_lerp(Vec4u32 start, Vec4u32 end, Vec4r32 factor) {
	Vec4u32 result;
	result.v.x = start.v.x + (u32)((r32)(end.v.x - start.v.x) * factor.v.x);
	result.v.y = start.v.y + (u32)((r32)(end.v.y - start.v.y) * factor.v.y);
	result.v.z = start.v.z + (u32)((r32)(end.v.z - start.v.z) * factor.v.z);
	result.v.w = start.v.w + (u32)((r32)(end.v.w - start.v.w) * factor.v.w);
	return result;
}

internalf Vec4r32 vec4u32_unlerp(Vec4u32 start, Vec4u32 vec, Vec4u32 end) {
	Vec4r32 result;
	result.v.x = (r32)vec.v.x / (r32)(end.v.x - start.v.x);
	result.v.y = (r32)vec.v.y / (r32)(end.v.y - start.v.y);
	result.v.z = (r32)vec.v.z / (r32)(end.v.z - start.v.z);
	result.v.w = (r32)vec.v.w / (r32)(end.v.w - start.v.w);
	return result;
}

internalf u32 vec4u32_dot(Vec4u32 vec1, Vec4u32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y + vec1.v.z * vec2.v.z + vec1.v.w * vec2.v.w;
}

internalf r32 vec4u32_angle_undirected_radians(Vec4u32 vec1, Vec4u32 vec2) {
	r32 cosine = (r32)vec4u32_dot(vec1, vec2) / (r32)(vec4u32_norm(vec1) * vec4u32_norm(vec2));
	return acos_r32(cosine);
}

internalf b8 vec4u32_match(Vec4u32 vec1, Vec4u32 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y) && (vec1.v.z == vec2.v.z) && (vec1.v.w == vec2.v.w));
}

internalf Vec2u32 rng2u32_measure(Rng2u32 rng) {
	Vec2u32 result;
	result.v.x = rng.r.max.v.x - rng.r.min.v.x;
	result.v.y = rng.r.max.v.y - rng.r.min.v.y;
	return result;
}

internalf Vec2u32 rng2u32_center(Rng2u32 rng) {
	Vec2u32 result;
	result.v.x = rng.r.min.v.x + (u32)((r32)(rng.r.max.v.x - rng.r.min.v.x) * 0.5f);
	result.v.y = rng.r.min.v.y + (u32)((r32)(rng.r.max.v.y - rng.r.min.v.y) * 0.5f);
	return result;
}

internalf Rng2u32 rng2u32_pad(Rng2u32 rng, Rng2u32 pad) {
	Rng2u32 result;
	result.r.min.v.x = rng.r.min.v.x + pad.r.min.v.x;
	result.r.min.v.y = rng.r.min.v.y + pad.r.min.v.y;
	result.r.max.v.x = rng.r.max.v.x + pad.r.max.v.x;
	result.r.max.v.y = rng.r.max.v.y + pad.r.max.v.y;
	return result;
}

internalf Rng2u32 rng2u32_clamp(Rng2u32 rng, Rng2u32 clamp) {
	Rng2u32 result;
	result.r.min.v.x = clamp(clamp.r.min.v.x, rng.r.min.v.x, clamp.r.max.v.x);
	result.r.min.v.y = clamp(clamp.r.min.v.y, rng.r.min.v.y, clamp.r.max.v.y);
	result.r.max.v.x = clamp(clamp.r.min.v.x, rng.r.max.v.x, clamp.r.max.v.x);
	result.r.max.v.y = clamp(clamp.r.min.v.y, rng.r.max.v.y, clamp.r.max.v.y);
	return result;
}

internalf b8 rng2u32_contains_vec2u32(Rng2u32 rng, Vec2u32 vec) {
	return (b8)((vec.v.x >= rng.r.min.v.x) && (vec.v.x <= rng.r.max.v.x) && (vec.v.y >= rng.r.min.v.y) && (vec.v.y <= rng.r.max.v.y));
}

internalf Rng2u32 rng2u32_union(Rng2u32 rng1, Rng2u32 rng2) {
	Rng2u32 result;
	result.r.min.v.x = min(rng1.r.min.v.x, rng2.r.min.v.x);
	result.r.min.v.y = min(rng1.r.min.v.y, rng2.r.min.v.y);
	result.r.max.v.x = max(rng1.r.max.v.x, rng2.r.max.v.x);
	result.r.max.v.y = max(rng1.r.max.v.y, rng2.r.max.v.y);
	return result;
}

internalf Rng2u32 rng2u32_intersect(Rng2u32 rng1, Rng2u32 rng2) {
	Rng2u32 intersect;
	intersect.r.min.v.x = max(rng1.r.min.v.x, rng2.r.min.v.x);
	intersect.r.min.v.y = max(rng1.r.min.v.y, rng2.r.min.v.y);
	intersect.r.max.v.x = min(rng1.r.max.v.x, rng2.r.max.v.x);
	intersect.r.max.v.y = min(rng1.r.max.v.y, rng2.r.max.v.y);
  
	if(intersect.r.min.v.x > intersect.r.max.v.x || intersect.r.max.v.x > intersect.r.max.v.y) {
		intersect.r.min.v.x = 0;
		intersect.r.min.v.y = 0;
		intersect.r.max.v.x = 0;
		intersect.r.max.v.y = 0;
	}
  
	return intersect;
}

internalf Rng2u32 rngs4u32_clamp_rng2u32(Rng2u32 rng1, Rng2u32 rng2, Rng2u32 rng3) {
	Rng2u32 intersect = rng2u32_intersect(rng1, rng2);
	Rng2u32 result;
	result.r.min.v.x = clamp(intersect.r.min.v.x, rng3.r.min.v.x, intersect.r.max.v.x);
	result.r.max.v.x = clamp(intersect.r.min.v.x, rng3.r.max.v.x, intersect.r.max.v.x);
	result.r.min.v.y = clamp(intersect.r.min.v.y, rng3.r.min.v.y, intersect.r.max.v.y);
	result.r.max.v.y = clamp(intersect.r.min.v.y, rng3.r.max.v.y, intersect.r.max.v.y);
	return result;
}


internalf Vec4r32 vec4r32_scale(Vec4r32 vec, Vec4r32 scale) {
	Vec4r32 result;
	result.v.x = (vec.v.x * scale.v.x);
	result.v.y = (vec.v.y * scale.v.y);
	result.v.z = (vec.v.z * scale.v.z);
	result.v.w = (vec.v.w * scale.v.w);
	return result;
}

internalf Vec4r32 vec4r32_normalize(Vec4r32 vec) {
	r32 length = vec4r32_norm(vec);
	Vec4r32 result;
	result.v.x = vec.v.x / length;
	result.v.y = vec.v.y / length;
	result.v.z = vec.v.z / length;
	result.v.w = vec.v.w / length;
	return result;
}

internalf Vec4r32 vec4r32_reverse(Vec4r32 vec) {
	Vec4r32 result;
	result.v.x = -1.0f * vec.v.x;
	result.v.y = -1.0f * vec.v.y;
	result.v.z = -1.0f * vec.v.z;
	result.v.w = -1.0f * vec.v.w;	
	return result;
}

internalf r32 vec4r32_norm(Vec4r32 vec) {
	return sqrt_r32((vec.v.x * vec.v.x + vec.v.y * vec.v.y + vec.v.z * vec.v.z + vec.v.w * vec.v.w));
}

internalf Vec4r32 vec4r32_nil(void) {
	Vec4r32 result;
	result.v.x = 0.0f;
	result.v.y = 0.0f;
	result.v.z = 0.0f;
	result.v.w = 0.0f;
	return result;
}

internalf Vec4r32 vec4r32_fill(r32 number) {
	Vec4r32 result;
	result.v.x = number;
	result.v.y = number;
	result.v.z = number;
	result.v.w = number;
	return result;
}

internalf b8 vec4r32_is_nil(Vec4r32 vec) {
	return (b8)((vec.v.x == 0.0f) && (vec.v.y == 0.0f) && (vec.v.z == 0.0f) && (vec.v.w == 0.0f));
}

internalf Vec4r32 vec4r32_add(Vec4r32 vec1, Vec4r32 vec2) {
	Vec4r32 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	result.v.z = vec1.v.z + vec2.v.z;
	result.v.w = vec1.v.w + vec2.v.w;
	return result;
}

internalf Vec4r32 vec4r32_sub(Vec4r32 vec1, Vec4r32 vec2) {
	Vec4r32 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	result.v.z = vec1.v.z - vec2.v.z;
	result.v.w = vec1.v.w - vec2.v.w;
	return result;
}

internalf Vec4r32 vec4r32_mul(Vec4r32 vec1, Vec4r32 vec2) {
	Vec4r32 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	result.v.z = vec1.v.z * vec2.v.z;
	result.v.w = vec1.v.w * vec2.v.w;
	return result;
}

internalf Vec4r32 vec4r32_div(Vec4r32 vec1, Vec4r32 vec2) {
	Vec4r32 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	result.v.z = vec1.v.z / vec2.v.z;
	result.v.w = vec1.v.w / vec2.v.w;
	return result;
}

internalf Vec4r32 vec4r32_lerp(Vec4r32 start, Vec4r32 end, Vec4r32 factor) {
	Vec4r32 result;
	result.v.x = start.v.x + ((end.v.x - start.v.x) * factor.v.x);
	result.v.y = start.v.y + ((end.v.y - start.v.y) * factor.v.y);
	result.v.z = start.v.z + ((end.v.z - start.v.z) * factor.v.z);
	result.v.w = start.v.w + ((end.v.w - start.v.w) * factor.v.w);
	return result;
}

internalf Vec4r32 vec4r32_unlerp(Vec4r32 start, Vec4r32 vec, Vec4r32 end) {
	Vec4r32 result;
	result.v.x = vec.v.x / (end.v.x - start.v.x);
	result.v.y = vec.v.y / (end.v.y - start.v.y);
	result.v.z = vec.v.z / (end.v.z - start.v.z);
	result.v.w = vec.v.w / (end.v.w - start.v.w);
	return result;
}

internalf r32 vec4r32_dot(Vec4r32 vec1, Vec4r32 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y + vec1.v.z * vec2.v.z + vec1.v.w * vec2.v.w;
}

internalf r32 vec4r32_angle_undirected_radians(Vec4r32 vec1, Vec4r32 vec2) {
	r32 cosine = vec4r32_dot(vec1, vec2) / (vec4r32_norm(vec1) * vec4r32_norm(vec2));
	return acos_r32(cosine);
}

internalf b8 vec4r32_match(Vec4r32 vec1, Vec4r32 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y) && (vec1.v.z == vec2.v.z) && (vec1.v.w == vec2.v.w));
}

internalf Vec2r32 rng2r32_measure(Rng2r32 rng) {
	Vec2r32 result;
	result.v.x = rng.r.max.v.x - rng.r.min.v.x;
	result.v.y = rng.r.max.v.y - rng.r.min.v.y;
	return result;
}

internalf Rng2r32 rng2r32_nil(void) {
	Rng2r32 result;
	result.r.min.v.x = 0.0f;
	result.r.min.v.y = 0.0f;
	result.r.max.v.x = 0.0f;
	result.r.max.v.y = 0.0f;
	return result;
}

internalf b8 rng2r32_is_nil(Rng2r32 rng) {
	return (b8)((rng.r.min.v.x == 0.0f) && (rng.r.max.v.x == 0.0f) && (rng.r.min.v.y == 0.0f) && (rng.r.max.v.y == 0.0f));
}

internalf Vec2r32 rng2r32_center(Rng2r32 rng) {
	Vec2r32 result;
	result.v.x = rng.r.min.v.x + ((rng.r.max.v.x - rng.r.min.v.x) * 0.5f);
	result.v.y = rng.r.min.v.y + ((rng.r.max.v.y - rng.r.min.v.y) * 0.5f);
	return result;
}

internalf Rng2r32 rng2r32_pad(Rng2r32 rng, Rng2r32 pad) {
	Rng2r32 result;
	result.r.min.v.x = rng.r.min.v.x + pad.r.min.v.x;
	result.r.min.v.y = rng.r.min.v.y + pad.r.min.v.y;
	result.r.max.v.x = rng.r.max.v.x + pad.r.max.v.x;
	result.r.max.v.y = rng.r.max.v.y + pad.r.max.v.y;
	return result;
}

internalf Rng2r32 rng2r32_clamp(Rng2r32 rng, Rng2r32 clamp) {
	Rng2r32 result;
	result.r.min.v.x = clamp(clamp.r.min.v.x, rng.r.min.v.x, clamp.r.max.v.x);
	result.r.min.v.y = clamp(clamp.r.min.v.y, rng.r.min.v.y, clamp.r.max.v.y);
	result.r.max.v.x = clamp(clamp.r.min.v.x, rng.r.max.v.x, clamp.r.max.v.x);
	result.r.max.v.y = clamp(clamp.r.min.v.y, rng.r.max.v.y, clamp.r.max.v.y);
	return result;
}

internalf b8 rng2r32_contains_vec2r32(Rng2r32 rng, Vec2r32 vec) {
	return (b8)((vec.v.x >= rng.r.min.v.x) && (vec.v.x <= rng.r.max.v.x) && (vec.v.y >= rng.r.min.v.y) && (vec.v.y <= rng.r.max.v.y));
}

internalf Rng2r32 rng2r32_union(Rng2r32 rng1, Rng2r32 rng2) {
	Rng2r32 result;
	result.r.min.v.x = min(rng1.r.min.v.x, rng2.r.min.v.x);
	result.r.min.v.y = min(rng1.r.min.v.y, rng2.r.min.v.y);
	result.r.max.v.x = max(rng1.r.max.v.x, rng2.r.max.v.x);
	result.r.max.v.y = max(rng1.r.max.v.y, rng2.r.max.v.y);
	return result;
}

internalf Rng2r32 rng2r32_intersect(Rng2r32 rng1, Rng2r32 rng2) {
	Rng2r32 intersect;
	intersect.r.min.v.x = max(rng1.r.min.v.x, rng2.r.min.v.x);
	intersect.r.min.v.y = max(rng1.r.min.v.y, rng2.r.min.v.y);
	intersect.r.max.v.x = min(rng1.r.max.v.x, rng2.r.max.v.x);
	intersect.r.max.v.y = min(rng1.r.max.v.y, rng2.r.max.v.y);
  
	if(intersect.r.min.v.x > intersect.r.max.v.x || intersect.r.max.v.x > intersect.r.max.v.y) {
		intersect.r.min.v.x = 0.0f;
		intersect.r.min.v.y = 0.0f;
		intersect.r.max.v.x = 0.0f;
		intersect.r.max.v.y = 0.0f;
	}
  
	return intersect;
}

internalf Rng2r32 rng2r32_clamp_rng2r32(Rng2r32 rng1, Rng2r32 rng2, Rng2r32 rng3) {
	Rng2r32 intersect = rng2r32_intersect(rng1, rng2);
	Rng2r32 result;
	result.r.min.v.x = clamp(intersect.r.min.v.x, rng3.r.min.v.x, intersect.r.max.v.x);
	result.r.max.v.x = clamp(intersect.r.min.v.x, rng3.r.max.v.x, intersect.r.max.v.x);
	result.r.min.v.y = clamp(intersect.r.min.v.y, rng3.r.min.v.y, intersect.r.max.v.y);
	result.r.max.v.y = clamp(intersect.r.min.v.y, rng3.r.max.v.y, intersect.r.max.v.y);
	return result;
}


internalf Vec4r64 vec4r64_scale(Vec4r64 vec, Vec4r64 scale) {
	Vec4r64 result;
	result.v.x = (vec.v.x * scale.v.x);
	result.v.y = (vec.v.y * scale.v.y);
	result.v.z = (vec.v.z * scale.v.z);
	result.v.w = (vec.v.w * scale.v.w);
	return result;
}

internalf Vec4r64 vec4r64_normalize(Vec4r64 vec) {
	r64 length = vec4r64_norm(vec);
	Vec4r64 result;
	result.v.x = 	vec.v.x / length;
	result.v.y = vec.v.y / length;
	result.v.z = vec.v.z / length;
	result.v.w = vec.v.w / length;
	return result;
}

internalf Vec4r64 vec4r64_reverse(Vec4r64 vec) {
	Vec4r64 result;
	result.v.x = -1.0f * vec.v.x;
	result.v.y = -1.0f * vec.v.y;
	result.v.z = -1.0f * vec.v.z;
	result.v.w = -1.0f * vec.v.w;
	return result;
}

internalf r64 vec4r64_norm(Vec4r64 vec) {
	return sqrt_r64((vec.v.x * vec.v.x + vec.v.y * vec.v.y + vec.v.z * vec.v.z + vec.v.w * vec.v.w));
}

internalf Vec4r64 vec4r64_nil(void) {
	Vec4r64 result;
	result.v.x = 0.0f;
	result.v.y = 0.0f;
	result.v.z = 0.0f;
	result.v.w = 0.0f;
	return result;
}

internalf Vec4r64 vec4r64_fill(r64 number) {
	Vec4r64 result;
	result.v.x = number;
	result.v.y = number;
	result.v.z = number;
	result.v.w = number;
	return result;
}

internalf b8 vec4r64_is_nil(Vec4r64 vec) {
	return (b8)((vec.v.x == 0.0f) && (vec.v.y == 0.0f) && (vec.v.z == 0.0f) && (vec.v.w == 0.0f));
}

internalf Vec4r64 vec4r64_add(Vec4r64 vec1, Vec4r64 vec2) {
	Vec4r64 result;
	result.v.x = vec1.v.x + vec2.v.x;
	result.v.y = vec1.v.y + vec2.v.y;
	result.v.z = vec1.v.z + vec2.v.z;
	result.v.w = vec1.v.w + vec2.v.w;
	return result;
}

internalf Vec4r64 vec4r64_sub(Vec4r64 vec1, Vec4r64 vec2) {
	Vec4r64 result;
	result.v.x = vec1.v.x - vec2.v.x;
	result.v.y = vec1.v.y - vec2.v.y;
	result.v.z = vec1.v.z - vec2.v.z;
	result.v.w = vec1.v.w - vec2.v.w;
	return result;
}

internalf Vec4r64 vec4r64_mul(Vec4r64 vec1, Vec4r64 vec2) {
	Vec4r64 result;
	result.v.x = vec1.v.x * vec2.v.x;
	result.v.y = vec1.v.y * vec2.v.y;
	result.v.z = vec1.v.z * vec2.v.z;
	result.v.w = vec1.v.w * vec2.v.w;
	return result;
}

internalf Vec4r64 vec4r64_div(Vec4r64 vec1, Vec4r64 vec2) {
	Vec4r64 result;
	result.v.x = vec1.v.x / vec2.v.x;
	result.v.y = vec1.v.y / vec2.v.y;
	result.v.z = vec1.v.z / vec2.v.z;
	result.v.w = vec1.v.w / vec2.v.w;
	return result;
}

internalf Vec4r64 vec4r64_lerp(Vec4r64 start, Vec4r64 end, Vec4r64 factor) {
	Vec4r64 result;
	result.v.x = start.v.x + ((end.v.x - start.v.x) * factor.v.x);
	result.v.y = start.v.y + ((end.v.y - start.v.y) * factor.v.y);
	result.v.z = start.v.z + ((end.v.z - start.v.z) * factor.v.z);
	result.v.w = start.v.w + ((end.v.w - start.v.w) * factor.v.w);
	return result;
}

internalf Vec4r64 vec4r64_unlerp(Vec4r64 start, Vec4r64 vec, Vec4r64 end) {
	Vec4r64 result;
	result.v.x = vec.v.x / (end.v.x - start.v.x);
	result.v.y = vec.v.y / (end.v.y - start.v.y);
	result.v.z = vec.v.z / (end.v.z - start.v.z);
	result.v.w = vec.v.w / (end.v.w - start.v.w);
	return result;
}

internalf r64 vec4r64_dot(Vec4r64 vec1, Vec4r64 vec2) {
	return vec1.v.x * vec2.v.x + vec1.v.y * vec2.v.y + vec1.v.z * vec2.v.z + vec1.v.w * vec2.v.w;
}

internalf r64 vec4r64_angle_undirected_radians(Vec4r64 vec1, Vec4r64 vec2) {
	r64 cosine = vec4r64_dot(vec1, vec2) / (vec4r64_norm(vec1) * vec4r64_norm(vec2));
	return acos_r64(cosine);
}

internalf b8 vec4r64_match(Vec4r64 vec1, Vec4r64 vec2) {
	return (b8)((vec1.v.x == vec2.v.x) && (vec1.v.y == vec2.v.y) && (vec1.v.z == vec2.v.z) && (vec1.v.w == vec2.v.w));
}

internalf Vec2r64 rng2r64_measure(Rng2r64 rng) {
	Vec2r64 result;
	result.v.x = rng.r.max.v.x - rng.r.min.v.x;
	result.v.y = rng.r.max.v.y - rng.r.min.v.y;
	return result;
}

internalf Vec2r64 rng2r64_center(Rng2r64 rng) {
	Vec2r64 result;
	result.v.x = rng.r.min.v.x + ((rng.r.max.v.x - rng.r.min.v.x) * 0.5f);
	result.v.y = rng.r.min.v.y + ((rng.r.max.v.y - rng.r.min.v.y) * 0.5f);
	return result;
}

internalf Rng2r64 rng2r64_pad(Rng2r64 rng, Rng2r64 pad) {
	Rng2r64 result;
	result.r.min.v.x = rng.r.min.v.x + pad.r.min.v.x;
	result.r.min.v.y = rng.r.min.v.y + pad.r.min.v.y;
	result.r.max.v.x = rng.r.max.v.x + pad.r.max.v.x;
	result.r.max.v.y = rng.r.max.v.y + pad.r.max.v.y;	
	return result;
}

internalf Rng2r64 rng2r64_clamp(Rng2r64 rng, Rng2r64 clamp) {
	Rng2r64 result;
	result.r.min.v.x = clamp(clamp.r.min.v.x, rng.r.min.v.x, clamp.r.max.v.x);
	result.r.min.v.y = clamp(clamp.r.min.v.y, rng.r.min.v.y, clamp.r.max.v.y);
	result.r.max.v.x = clamp(clamp.r.min.v.x, rng.r.max.v.x, clamp.r.max.v.x);
	result.r.max.v.y = clamp(clamp.r.min.v.y, rng.r.max.v.y, clamp.r.max.v.y);
	return result;
}

internalf b8 rng2r64_contains_vec2r64(Rng2r64 rng, Vec2r64 vec) {
	return (b8)((vec.v.x >= rng.r.min.v.x) && (vec.v.x <= rng.r.max.v.x) && (vec.v.y >= rng.r.min.v.y) && (vec.v.y <= rng.r.max.v.y));
}

internalf Rng2r64 rng2r64_union(Rng2r64 rng1, Rng2r64 rng2) {
	Rng2r64 result;
	result.r.min.v.x = min(rng1.r.min.v.x, rng2.r.min.v.x);
	result.r.min.v.y = min(rng1.r.min.v.y, rng2.r.min.v.y);
	result.r.max.v.x = max(rng1.r.max.v.x, rng2.r.max.v.x);
	result.r.max.v.y = max(rng1.r.max.v.y, rng2.r.max.v.y);
	return result;
}

internalf Rng2r64 rng2r64_intersect(Rng2r64 rng1, Rng2r64 rng2) {
	Rng2r64 intersect;
	intersect.r.min.v.x = max(rng1.r.min.v.x, rng2.r.min.v.x);
	intersect.r.min.v.y = max(rng1.r.min.v.y, rng2.r.min.v.y);
	intersect.r.max.v.x = min(rng1.r.max.v.x, rng2.r.max.v.x);
	intersect.r.max.v.y = min(rng1.r.max.v.y, rng2.r.max.v.y);
  
	if(intersect.r.min.v.x > intersect.r.max.v.x || intersect.r.max.v.x > intersect.r.max.v.y) {
		intersect.r.min.v.x = 0.0f;
		intersect.r.min.v.y = 0.0f;
		intersect.r.max.v.x = 0.0f;
		intersect.r.max.v.y = 0.0f;
	}
  
	return intersect;
}

internalf Rng2r64 rng2r64_clamp_rng2r64(Rng2r64 rng1, Rng2r64 rng2, Rng2r64 rng3) {
	Rng2r64 intersect = rng2r64_intersect(rng1, rng2);
	Rng2r64 result;
	result.r.min.v.x = clamp(intersect.r.min.v.x, rng3.r.min.v.x, intersect.r.max.v.x);
	result.r.max.v.x = clamp(intersect.r.min.v.x, rng3.r.max.v.x, intersect.r.max.v.x);
	result.r.min.v.y = clamp(intersect.r.min.v.y, rng3.r.min.v.y, intersect.r.max.v.y);
	result.r.max.v.y = clamp(intersect.r.min.v.y, rng3.r.max.v.y, intersect.r.max.v.y);
	return result;
}
// -------- Vec4/Rng2

// ------ Operations

// ---- Vectors/Ranges
