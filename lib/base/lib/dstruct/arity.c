

internalf Arity* arity_empty(Allocator* allocator) {
	Arity* result = (Arity*)allocate(allocator, sizeof(Arity));
	result->type = ArityType_Empty;
	return result;
}

internalf Arity* arity_singleton(Allocator* allocator) {
	Arity* result = (Arity*)allocate(allocator, sizeof(Arity));
	result->type = ArityType_Singleton;
	result->coefficient = 1;
	return result;
}

internalf Arity* arity_arrow(Allocator* allocator, Arity* domain, Arity* codomain) {
	Arity* result = 0;
	
	if(domain->type == ArityType_Empty || codomain->type == ArityType_Empty) {  // only one empty arity
		result = codomain;
	} else {
		result = (Arity*)allocate(allocator, sizeof(Arity));
		result->type = ArityType_Arrow;
		result->coefficient = 1;
		result->links.arrow.domain = domain;
		result->links.arrow.codomain = codomain;
	}
		
	return result;
}

internalf Arity* arity_plus(Allocator* allocator, Arity* arity1, Arity* arity2) {
	Arity* result = 0;

	if(arity1->type == ArityType_Empty) {
		result = arity2;
	} else if(arity2->type == ArityType_Empty) {
		result = arity1;
	} else {
		ArenaTemp scratch = scratch_begin((Arena**)&allocator, 1);
		Allocator* scratch_allocator = &scratch.arena->allocator;
		Arity** parts;
		Arity** part_ptr;
		u64 max_part_count = 0;
		
		// -- Allocate temporary parts
		if(arity1->type != ArityType_Tuple) {  // append flattened
			max_part_count += 1;
		} else {
			max_part_count += arity1->links.tuple.part_count;
		}

		if(arity2->type != ArityType_Tuple) {
			max_part_count += 1;
		} else {
			max_part_count += arity2->links.tuple.part_count;
		}

		parts = (Arity**)allocate(scratch_allocator, sizeof(Arity*) * max_part_count);
		part_ptr = parts;
		// -- Allocate temporary parts


		// -- first arity
		if(arity1->type != ArityType_Tuple) {
			*part_ptr = arity_shallow_copy_(allocator, arity1);
			(*part_ptr)->coefficient = 1;
			part_ptr += 1;
		} else {
			Arity* curr_part;
			Arity** curr_part_ptr = arity1->links.tuple.parts;
			Arity** final_part_ptr = arity1->links.tuple.parts + arity1->links.tuple.part_count;
			for(; curr_part_ptr != final_part_ptr; curr_part_ptr += 1) {
				curr_part = *curr_part_ptr;
				*part_ptr = arity_shallow_copy_(allocator, curr_part);
				(*part_ptr)->coefficient = curr_part->coefficient;
				part_ptr += 1;
			}
		}
		// -- first arity


		// -- second arity (need to check for duplicates)
		if(arity2->type != ArityType_Tuple) {
			Arity* arity_match = arity_parts_find_match_(parts, part_ptr, arity2);
			if(arity_match) {
				arity_match->coefficient += 1;
			} else {
				*part_ptr = arity_shallow_copy_(allocator, arity2);
				(*part_ptr)->coefficient = 1;
				part_ptr += 1;
			}
		} else {
			Arity* curr_part;
			Arity* arity_match;
			Arity** curr_part_ptr = arity2->links.tuple.parts;
			Arity** final_part_ptr = arity2->links.tuple.parts + arity2->links.tuple.part_count;
			for(; curr_part_ptr != final_part_ptr; curr_part_ptr += 1) {
				curr_part = *curr_part_ptr;
				arity_match = arity_parts_find_match_(parts, part_ptr, curr_part);
				if(arity_match) {
					arity_match->coefficient += curr_part->coefficient;
				} else {
					*part_ptr = arity_shallow_copy_(allocator, curr_part);
					(*part_ptr)->coefficient = curr_part->coefficient;
					part_ptr += 1;
				}
			}
		}
		// -- second arity

		assert((u64)(part_ptr - parts > 1) || ((u64)((part_ptr - parts) == 1) && parts[0]->coefficient > 1));

		// -- persist parts
		result = (Arity*)allocate(allocator, sizeof(Arity));
		result->links.tuple.part_count = (u64)(part_ptr - parts);
		result->links.tuple.parts = (Arity**)allocate(allocator, sizeof(Arity*) * result->links.tuple.part_count);	
		mem_cpy(result->links.tuple.parts, parts, sizeof(Arity*) * result->links.tuple.part_count);
		result->coefficient = 1;
		result->type = ArityType_Tuple;
		// -- persist parts

		scratch_end(&scratch);
	}
	
	return result;
}

internalf Arity* arity_ktuple(Allocator* allocator, u32 coefficient) {
	Arity* result = (Arity*)allocate(allocator, sizeof(Arity));
	Arity* singleton;
	
	result->links.tuple.part_count = 1;
	result->links.tuple.parts = (Arity**)allocate(allocator, sizeof(Arity*));
	singleton = arity_singleton(allocator);
	singleton->coefficient = coefficient;
	result->links.tuple.parts[0] = singleton;
	
	result->type = ArityType_Tuple;
	result->coefficient = 1;
	
	return result;
}

internalf Arity* arity_sum(Allocator* allocator, ...) {
	
	Arity* result = 0;
	
	va_list arity_args;
	va_list arity_args2;
	u64 max_part_count;
	u32 curr_arg_coefficient;
	Arity* curr_arg_arity;

	u64 final_part_count;
	

	va_start(arity_args, allocator);
	va_copy(arity_args2, arity_args);

	// -- get number of parts
	max_part_count = 0;
	curr_arg_coefficient = va_arg(arity_args, u32);
	curr_arg_arity = va_arg(arity_args, Arity*);
	for(; curr_arg_coefficient; curr_arg_coefficient = va_arg(arity_args, u32), curr_arg_arity = va_arg(arity_args, Arity*)) {
		if(curr_arg_arity->type == ArityType_Empty) {
		} else if(curr_arg_arity->type != ArityType_Tuple) {
			max_part_count += 1;
		} else {
			max_part_count += curr_arg_arity->links.tuple.part_count;
		}
	}
	// -- get number of parts

	if(max_part_count) {
		Arity* arity_match;
		ArenaTemp scratch = scratch_begin((Arena**)&allocator, 1);
		Allocator* scratch_allocator = &scratch.arena->allocator;


		// -- fill parts array
		Arity** parts = (Arity**)allocate(scratch_allocator , sizeof(Arity*) * max_part_count);
		Arity** parts_ptr = parts;

		curr_arg_coefficient = va_arg(arity_args2, u32);
		curr_arg_arity = va_arg(arity_args2, Arity*);
		for(; curr_arg_coefficient; curr_arg_coefficient = va_arg(arity_args2, u32), curr_arg_arity = va_arg(arity_args2, Arity*)) {
			if(curr_arg_arity->type == ArityType_Empty) {
			} else if(curr_arg_arity->type != ArityType_Tuple) {
				arity_match = arity_parts_find_match_(parts, parts_ptr, curr_arg_arity);
				if(arity_match) {
					arity_match->coefficient += curr_arg_coefficient;
				} else {
					*parts_ptr = arity_shallow_copy_(allocator, curr_arg_arity);
					(*parts_ptr)->coefficient = curr_arg_coefficient;
					parts_ptr += 1;
				}
			} else {
				
				Arity* curr_tuple_part;
				Arity** curr_tuple_part_ptr = curr_arg_arity->links.tuple.parts;
				Arity** final_tuple_part_ptr = curr_arg_arity->links.tuple.parts + curr_arg_arity->links.tuple.part_count;
				
				for(; curr_tuple_part_ptr != final_tuple_part_ptr; curr_tuple_part_ptr += 1) {
					curr_tuple_part = *curr_tuple_part_ptr;
					arity_match = arity_parts_find_match_(parts, parts_ptr, curr_tuple_part);
					if(arity_match) {
						arity_match->coefficient += (curr_arg_coefficient * curr_tuple_part->coefficient);
					} else {
						*parts_ptr = arity_shallow_copy_(allocator, curr_tuple_part);
						(*parts_ptr)->coefficient = curr_arg_coefficient * curr_tuple_part->coefficient;
						parts_ptr += 1;
					}
				}
			}
		}
		// -- fill parts array

		// -- fill result
		final_part_count = (u64)(parts_ptr - parts);
		if(final_part_count == 0) {
			result = arity_empty(allocator);
		} else if((final_part_count == 1) && (parts[0]->coefficient == 1)) {
			result = parts[0];
		} else {
			result = (Arity*)allocate(allocator, sizeof(Arity));
			result->links.tuple.part_count = (u64)(parts_ptr - parts);
			result->links.tuple.parts = (Arity**)allocate(allocator, sizeof(Arity*) * result->links.tuple.part_count);
			mem_cpy(result->links.tuple.parts, parts, sizeof(Arity*) * result->links.tuple.part_count);
			result->type = ArityType_Tuple;
			result->coefficient = 1;
		}
		// -- fill result


		scratch_end(&scratch);
	}

	va_end(arity_args2);
	va_end(arity_args);
	
	return result;
}

internalf Arity* arity_copy(Allocator* allocator, Arity* arity) {
	
	Arity* result = (Arity*)allocate(allocator, sizeof(Arity));
	result->type = arity->type;
	result->coefficient = arity->coefficient;
	
	if(arity->type == ArityType_Arrow) {
		result->links.arrow.codomain = arity->links.arrow.codomain;
		result->links.arrow.domain = arity->links.arrow.domain;
	} else if(arity->type == ArityType_Tuple) {
		Arity** curr_part_ptr;
		Arity** curr_part_to_copy_ptr = arity->links.tuple.parts;
		Arity** final_part_to_copy_ptr = arity->links.tuple.parts + arity->links.tuple.part_count;
		
		result->links.tuple.parts = (Arity**)allocate(allocator, sizeof(Arity*) * arity->links.tuple.part_count);
		curr_part_ptr = result->links.tuple.parts;
		for(; curr_part_to_copy_ptr != final_part_to_copy_ptr; curr_part_to_copy_ptr += 1, curr_part_ptr += 1) {
			*curr_part_ptr = arity_copy(allocator, *curr_part_to_copy_ptr);
		}
		result->links.tuple.part_count = arity->links.tuple.part_count;
	}
	
	return result;
}

internalf b8 arity_equal(Arity* arity1, Arity* arity2) {

	b8 result = 0;

	if(arity1->type == arity2->type) {
		if(arity1->type == ArityType_Arrow) {
			if((arity1->links.arrow.domain == arity2->links.arrow.domain) && (arity1->links.arrow.codomain == arity2->links.arrow.codomain)) {
				result = 1;
			}
		} else if(arity1->type == ArityType_Tuple) {
			if(arity1->links.tuple.part_count == arity2->links.tuple.part_count) {
				Arity** curr_part_ptr1 = arity1->links.tuple.parts;
				Arity** curr_part_ptr2 = arity2->links.tuple.parts;
				Arity** final_part_ptr1 = arity1->links.tuple.parts + arity1->links.tuple.part_count;
				Arity** final_part_ptr2 = arity2->links.tuple.parts + arity2->links.tuple.part_count;
				Arity* curr_part1;
				Arity* curr_part2;
				b8 found_match;

				result = 1;
				for(; curr_part_ptr1 != final_part_ptr1; curr_part_ptr1 += 1) {
					curr_part1 = *curr_part_ptr1;
					found_match = 0;
					curr_part_ptr2 = arity2->links.tuple.parts;
					for(; curr_part_ptr2 != final_part_ptr2; curr_part_ptr2 += 1) {
						curr_part2 = *curr_part_ptr2;
						if((curr_part1->coefficient == curr_part2->coefficient) && arity_equal(curr_part1, curr_part2)) {
							found_match = 1;
							break;
						}
					}

					if(!found_match) {
						result = 0;
						break;
					}
				}
			}
		} else {
			result = 1;
		}
	}

	return result;
}

internalf Arity* arity_shallow_copy_(Allocator* allocator, Arity* arity) {
	Arity* result;
	assert(arity->type != ArityType_Tuple);
	result = (Arity*)allocate(allocator, sizeof(Arity));
	result->type = arity->type;
	if(arity->type == ArityType_Arrow) {
		result->links.arrow.domain = arity->links.arrow.domain;
		result->links.arrow.codomain = arity->links.arrow.codomain;
	}
	return result;
}

internalf Arity* arity_parts_find_match_(Arity** parts_first, Arity** parts_opl, Arity* key) {
	Arity* result = (Arity*)0;
	Arity** curr;
	for(curr = parts_first; curr != parts_opl; curr++) {
		if(arity_equal(*curr, key)) {
			result = *curr;
			break;
		}
	}
	return result;
}
