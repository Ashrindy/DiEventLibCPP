#pragma once
#include "internal.h"
#include <vector>

namespace dv {
	struct RawDvNode {
	public:
		internal::Guid guid;
		unsigned int category;
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
			return reinterpret_cast<T*>(((uintptr_t)this) + sizeof(RawDvNode));
		}

		std::vector<RawDvNode*> getChildNodes() {
			std::vector<RawDvNode*> childnodes;
			uintptr_t pos = (((uintptr_t)this) + sizeof(RawDvNode) + nodeSize * 4);
			for (unsigned int x = 0; x < childCount; x++) {
				RawDvNode* node = (RawDvNode*)pos;
				pos += sizeof(RawDvNode);
				pos += node->nodeSize * 4;
				for (unsigned int y = 0; y < node->childCount; y++)
					skipChildNodes(pos);

				childnodes.push_back(node);
			}
			return childnodes;
		}

	private:
		void skipChildNodes(uintptr_t& pos) {
			RawDvNode* node = (RawDvNode*)pos;
			pos += sizeof(RawDvNode);
			pos += node->nodeSize * 4;
			for (unsigned int x = 0; x < node->childCount; x++)
				skipChildNodes(pos);
		}
	};

	struct DvNode {
		internal::Guid guid;
		unsigned int category;
		unsigned int nodeFlags;
		int priority;
		char name[64];
		size_t dataSize;
		char* data;
		std::vector<DvNode> childNodes;

		template<typename T>
		inline T* getData() {
			return reinterpret_cast<T*>(data);
		}
	};
}