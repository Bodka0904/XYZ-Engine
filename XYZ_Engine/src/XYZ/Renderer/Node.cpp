#include "stdafx.h"
#include "Node.h"


namespace XYZ {
	void Node::SetParent(Node* parent)
	{
		Parent = parent;
		NextSibling = parent->FirstChild;
		parent->FirstChild = this;
	}
}