

// TODO
#ifndef BASE_DSTRUCT_NARYTREE_H
# define BASE_DSTRUCT_NARYTREE_H


// -- Types
typedef struct NaryTree NaryTree;
struct NaryTree {
	NaryTree* parent;
	Dll siblings_links;
	Dll children_header;
	u32 children_count;
};
// -- Types


// -- Functions
internalf void nary_tree_init(NaryTree* tree);
internalf void nary_tree_push_child(NaryTree* parent, NaryTree* prev, NaryTree* node);
internalf void nary_tree_push_child_back(NaryTree* parent, NaryTree* node);
internalf void nary_tree_push_child_front(NaryTree* parent, NaryTree* node);
internalf void nary_tree_pop(NaryTree* node);
internalf NaryTree* nary_tree_next_preorder(NaryTree* node);
internalf NaryTree* nary_tree_next_postorder(NaryTree* node);
// -- Functions


#endif  // BASE_DSTRUCT_NARYTREE_H
