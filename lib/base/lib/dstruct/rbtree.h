

// TODO
#ifndef BASE_DSTRUCT_RBTREE_H
# define BASE_DSTRUCT_RBTREE_H


// -- Types
typedef struct RBTree RBTree;
struct RBTree {
	RBTree* parent;
	RBTree* left;
	RBTree* right;
};
// -- Types


// -- Functions
internalf void rbtree_rotate_left(RBTree** root, RBTree* node);
internalf b8 rbtree_push(RBTree* root, RBTree* node, b8(*less)(RBTree* node_in, RBTree* node));
internalf RBTree* rbtree_pop(RBTree* root, void* key, s8(*cmp)(void* key, RBTree* node));
internalf RBTree* rbtree_find(RBTree* root, void* key, s8(*cmp)(void* key, RBTree* node));
internalf RBTree* rbtree_next_preorder(RBTree* node);
internalf RBTree* rbtree_next_postorder(RBTree* node);
// -- Functions


#endif  // BASE_DSTRUCT_RBTREE_H
