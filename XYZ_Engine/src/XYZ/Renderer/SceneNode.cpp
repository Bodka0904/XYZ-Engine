#include "stdafx.h"
#include "SceneNode.h"


namespace XYZ {
	void SceneNode::SetParent(SceneNode* parent)
	{
		Parent = parent;
		NextSibling = parent->FirstChild;
		parent->FirstChild = this;
	}
}