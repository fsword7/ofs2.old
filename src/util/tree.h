/*
 * tree.h
 *
 *  Created on: Oct 14, 2018
 *      Author: Tim Stark
 */

#pragma once

#define QTREE_NODES	4	// Quadtree structure
#define OTREE_NODES 8	// Octree structure

template<class Object, int nodes>
class Tree
{
public:
	Tree(Object *parent = nullptr);
	~Tree();

	Object *getParent();
	Object *getChild(int idx);
	const Object *getChild(int idx) const;

	Object *addChild(int idx, Object *child);
	bool delChild(int idx);
	bool delChildren();

private:
	Object *parent;
	Object *child[nodes];
};

template<class Object, int nodes>
Tree<Object, nodes>::Tree(Object *_parent)
{
	parent = _parent;
	for (int idx = 0; idx < nodes; idx++)
		child[idx] = nullptr;
}

template<class Object, int nodes>
Tree<Object, nodes>::~Tree()
{
	if (parent != nullptr)
		parent = nullptr;
	for (int idx = 0; idx < nodes; idx++)
		if (child[idx] != nullptr)
			delete child[idx];
}
template<class Object, int nodes>
Object *Tree<Object, nodes>::getParent()
{
	return parent;
}

template<class Object, int nodes>
Object *Tree<Object, nodes>::getChild(int idx)
{
	if (idx < 0 || idx >= nodes)
		return nullptr;
	return child[idx];
}

template<class Object, int nodes>
const Object *Tree<Object, nodes>::getChild(int idx) const
{
	if (idx < 0 || idx >= nodes)
		return nullptr;
	return child[idx];
}

template<class Object, int nodes>
Object *Tree<Object, nodes>::addChild(int idx, Object *_child)
{
	if (idx < 0 || idx >= nodes)
		return nullptr;
	child[idx] = _child;

	return child[idx];
}

template<class Object, int nodes>
bool Tree<Object, nodes>::delChild(int idx)
{
	if (idx < 0 || idx >= nodes)
		return false;
	if (child[idx] != nullptr) {
		delete child[idx];
		child[idx] = nullptr;
		return true;
	}
	return false;
}

template<class Object, int nodes>
bool Tree<Object, nodes>::delChildren()
{
	bool ok = true;
	for (int idx = 0; idx < nodes; idx++) {
		if (child[idx] != nullptr) {
			if (child[idx]->delChildren() == true) {
				delete child[idx];
				child[idx] = nullptr;
			} else
				ok = false;
		}
	}
	return ok;
}
