#pragma once
#include <vector>

namespace dv {
	struct RawDvCondition {
		int type;
		int parametersSize;
		long long unknown0;
	};

	struct RawDvTransition {
		int destinationPageID;
		int conditionCount;
		long long conditionSize;
	};

	struct RawDvPage {
		unsigned int version;
		unsigned int flags;
		unsigned int frameStart;
		unsigned int frameEnd;
		unsigned int transitionCount;
		unsigned int transitionSize;
		unsigned int skipFrame;
		unsigned int index;
		unsigned int pageSize;
	private:
		char unknown0[12];
	public:
		char name[32];
	};

	struct DvCondition {
		enum class ConditionType : unsigned int {
			Random = 0,
			PagePlayCount,
			LoopCount,
			PageEnd,
			Language,
			QTE = 1000
		};

		ConditionType type;
		size_t dataSize;
		char* data;
	};

	struct DvTransition {
		int destinationPageID;
		std::vector<DvCondition> conditions;
	};

	struct DvPage {
		unsigned int version;
		unsigned int flags;
		float frameStart;
		float frameEnd;
		float skipFrame;
		unsigned int index;
		char name[32];
		std::vector<int> unkData;
		std::vector<DvTransition> transition;
	};
}