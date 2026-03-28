

// TODO
#ifndef BASE_DSTRUCT_BTREE_H
# define BASE_DSTRUCT_BTREE_H


// -- Preprocessor Defines
// -- Preprocessor Defines


// -- Enums/Flags
// -- Enums/Flags


// -- Types
typedef struct BTree BTree;
struct BTree {
	BTree* parent;  // store here if node is a leaf in the first lsb
	BTree** children_array;
	Array children_header;
	Dll siblings;
};
// -- Types


// -- Functions
internalf void btree_init(BTree* tree);
internalf b8 btree_push(BTree** root, BTree* node, b8(*less)(BTree* node_in, BTree* node));
internalf BTree* btree_pop(BTree* root, void* key, s8(*cmp)(void* key, BTree* node));
internalf BTree* btree_find(BTree* root, void* key, s8(*cmp)(void* key, BTree* node));
internalf BTree* btree_next_preorder(BTree* node);
internalf BTree* btree_next_postorder(BTree* node);
// -- Functions


#endif  // BASE_DSTRUCT_BTREE_H
