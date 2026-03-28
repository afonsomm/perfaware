

internalf void btree_init(BTree* tree) {
	
}

internalf b8 btree_push(BTree** root, BTree* node, b8(*less)(BTree* node_in, BTree* node)) {
	return (b8)0;
}

internalf BTree* btree_pop(BTree* root, void* key, s8(*cmp)(void* key, BTree* node)) {
	return (BTree*)0;
}

internalf BTree* btree_find(BTree* root, void* key, s8(*cmp)(void* key, BTree* node)) {
	return (BTree*)0;
}

internalf BTree* btree_next_preorder(BTree* node) {
	return (BTree*)0;
}

internalf BTree* btree_next_postorder(BTree* node) {
	return (BTree *)0;
}
