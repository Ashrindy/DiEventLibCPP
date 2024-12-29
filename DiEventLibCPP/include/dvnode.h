#pragma once
#include "internal.h"
#include <vector>

struct Path {
public:
	float mat[4][4];
	unsigned int flags;
private:
	char unknown0[12];
};

class DvNode {
public:
	enum class Category : unsigned int {
		Path = 1,
		Camera = 3,
		CameraMotion,
		Character,
		CharacterMotion,
		ModelCustom = 8,
		MotionModel = 10,
		ModelNode,
		Element
	};

	Guid guid;
	Category category;
	unsigned int nodeSize;
	unsigned int childCount;
	unsigned int nodeFlags;
	int priority;
private:
	char unknown0[12];
public:
	char name[64];

	template<typename T>
	T* getData() {
		return reinterpret_cast<T*>(((uintptr_t)this) + 112);
	}

	std::vector<DvNode*> getChildNodes() {
		std::vector<DvNode*> childnodes;
		uintptr_t pos = (((uintptr_t)this) + 112 + nodeSize * 4);
		for (auto x = 0; x < childCount; x++) {
			DvNode* node = (DvNode*)pos;
			pos += 112;
			pos += node->nodeSize * 4;
			for (auto y = 0; y < node->childCount; y++) {
				skipChildNodes(&pos);
			}
			childnodes.push_back(node);
		}
		return childnodes;
	}

private:
	void skipChildNodes(uintptr_t* pos) {
		DvNode* node = (DvNode*)(*pos);
		(*pos) += 112;
		(*pos) += node->nodeSize * 4;
		for (auto x = 0; x < node->childCount; x++) {
			skipChildNodes(pos);
		}
	}
};