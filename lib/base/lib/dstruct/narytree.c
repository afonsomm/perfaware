

internalf void nary_tree_init(NaryTree* tree) {
	dll_init(&tree->children_header);
}

internalf void nary_tree_push_child(NaryTree* parent, NaryTree* prev, NaryTree* node) {
	dll_push(&parent->children_header, &prev->siblings_links, &node->siblings_links);
	node->parent = parent;
	parent->children_count += 1;
}

internalf void nary_tree_push_child_back(NaryTree* parent, NaryTree* node) {
	dll_push(&parent->children_header, parent->children_header.prev, &node->siblings_links);
	node->parent = parent;
	parent->children_count += 1;
}

internalf void nary_tree_push_child_front(NaryTree* parent, NaryTree* node) {
	dll_push(&parent->children_header, &parent->children_header, &node->siblings_links);
	node->parent = parent;
	parent->children_count += 1;
}

internalf void nary_tree_pop(NaryTree* node) {

	dll_remove(&node->siblings_links);
	if(node->parent) {
		node->parent->children_count -= 1;
	}
}

internalf NaryTree* nary_tree_next_preorder(NaryTree* node) {
	
	NaryTree* result = container_of(node->children_header.next, NaryTree, siblings_links);
	if(result == node) {  // no children
		result = container_of(node->siblings_links.next, NaryTree, siblings_links);
		while(result == result->parent) {  // no next sibling
			result = container_of(node->siblings_links.next, NaryTree, siblings_links);
		}
	}

	return result;
}

internalf NaryTree* nary_tree_next_postorder(NaryTree* node) {

	NaryTree* result = container_of(node->siblings_links.next, NaryTree, siblings_links);
	if(result != node->parent) {  // has a sibling
		while(result->children_header.next != &result->children_header) {  // go to the deepest firt children from the sibling
			result = container_of(result->children_header.next, NaryTree, siblings_links);
		}
	}

	return result;
}
