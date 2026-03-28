
internalf void rbtree_rotate_left(RBTree** root, RBTree* node) {

	RBTree* other = node->right;
	node->right = other->left;
	other->left->parent = node;
	other->parent = node->parent;
	if(!node->parent) {
		*root = other;
	} else {
		if(node == node->parent->left) {
			node->parent->left = other;
		} else {
			node->parent->right = other;
		}
	}
	other->left = node;
	node->parent = other;
}

internalf b8 rbtree_push(RBTree* root, RBTree* node, b8(*less)(RBTree* node_in, RBTree* node)) {

	b8 result = 0;

	return result;
}

internalf RBTree* rbtree_pop(RBTree* root, void* key, s8(*cmp)(void* key, RBTree* node)) {

	RBTree* result = (RBTree*)0;

	return result;
}

internalf RBTree* rbtree_find(RBTree* root, void* key, s8(*cmp)(void* key, RBTree* node)) {

	RBTree* result = (RBTree*)0;

	return result;
}

internalf RBTree* rbtree_next_preorder(RBTree* node) {

	RBTree* result = (RBTree*)0;

	return result;
}

internalf RBTree* rbtree_next_postorder(RBTree* node) {

	RBTree* result = (RBTree*)0;

	return result;
}
