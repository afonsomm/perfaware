

/**

----- Encoding -----
k points - ("singleton") represented by a "coefficient" of k. This "singleton" is a member of a "tuple" arity of a single part pointing to the former.
(x -> y) - represented by the "arrow" inner struct, the "domain" being the input arity (x) and the "codomain" being the output arity (y).
(x y z ...) - represented by the "tuple" inner struct, using an array of pointers to other arity objects.
-----


----- Pros and Cons of the current implementation -----	

- High memory use. Each Arity Object is 24 bytes.

- High computation cost. Some operations are really costy, like "arity + arity".

- In-memory / "hot" data structure.
	This is an in-memory/"hot" structure, meaning it has direct references to data that is ready to be used in memory ("it is wired up ready to be used immediately").
	A "cold" structure determines information independently of position and time, so it is ready to be transported.
	So to transfer an Arity Object to a file or to another process (via pipe or socket), i would first need to serialize it, transfer it, and then deserialize it.
	Serialization converts a "hot" data structure into a "cold" data structure, while preserving the information needed to later reconstruct the "hot" data structure.

- High complexity construction code. If i was going to specify Arity Objects as part of some larger system, constructing them with this API would weight it all down.
	It takes multiple lines of putting pieces together to build any interesting Arity Objects. Besides, the usage code makes it hard to reason about the whole object
	being constructed. Besides, it is not possible to specify an Arity Object as a data literal.
-----


----- Exercises -----
1. N Distinct Singleton Kinds.
2. Make tuples ordered (+ becomes non-commutative).
3. Make arrows bidirectional (-> becomes commutative).
4. Comparison operator on arity nodes to sort them. Keeping parts sorted would turn an O(N * M) merging into an O(N + M).

NOTE: 4. (solved in the next lesson with a different data structure that does not need the total order / comparison operator).

Meus:
- Make a visualizer for arity objects with the square, arrows and points as in the video, along with the tables and the arrows.
- Implement the examples from minutes 17-19, also using the visualizer.
-----


---- References -----
Lesson: https://youtu.be/gh2A7yFsBZ4
Book Mentioned: Nordström, Bengt, Kent Petersson, and Jan M. Smith. Programming in Martin-Löf’s type theory. https://www.cse.chalmers.se/research/group/logic/book/book.pdf
-----

*/


#ifndef BASE_DSTRUCT_ARITY_H
# define BASE_DSTRUCT_ARITY_H


#include "../../../externals/inc_begin.h"
#include <stdarg.h>
#include "../../../externals/inc_end.h"


// -- Enums/Flags
typedef enum ArityType {
	ArityType_Empty = 0,
	ArityType_Singleton,
	ArityType_Arrow,
	ArityType_Tuple,
	ArityType_COUNT,
} ArityType;
// -- Enums/Flags


// -- Types
typedef struct Arity Arity;  // 24
struct Arity {
	enum32(ArityType) type;
	u32 coefficient;
	union {
		struct {
			Arity* codomain;
			Arity* domain;
		} arrow;
		struct {
			Arity** parts;
			u64 part_count;
		} tuple;
	} links;
};
// -- Types


// -- Functions
internalf Arity* arity_empty(Allocator* allocator);
internalf Arity* arity_singleton(Allocator* allocator);
internalf Arity* arity_arrow(Allocator* allocator, Arity* domain, Arity* codomain);
internalf Arity* arity_plus(Allocator* allocator, Arity* arity1, Arity* arity2);

internalf Arity* arity_ktuple(Allocator* allocator, u32 coefficient);
internalf Arity* arity_sum(Allocator* allocator, ...);

internalf Arity* arity_copy(Allocator* allocator, Arity* arity);
internalf b8 arity_equal(Arity* arity1, Arity* arity2);

internalf Arity* arity_shallow_copy_(Allocator* allocator, Arity* arity);  // does not work for tuple arities
internalf Arity* arity_parts_find_match_(Arity** parts, Arity** parts_opl, Arity* key);
// -- Functions


#endif  // BASE_DSTRUCT_ARITY_H
