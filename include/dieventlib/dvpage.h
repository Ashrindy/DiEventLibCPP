#pragma once
#include <vector>

namespace dv {
	struct RawDvCondition {
	public:
		int type;
		int parametersSize;
	private:
		long long unknown0;
	};

	struct RawDvTransition {
	public:
		int destinationPageID;
		int conditionCount;
		long long conditionSize;
	};

	struct RawDvPage {
	public:
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
	public:
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
	public:
		int destinationPageID;
		std::vector<DvCondition> conditions;
	};

	struct DvPage {
	private:
		unsigned int version;
		unsigned int flags;
	public:
		float frameStart;
		float frameEnd;
		float skipFrame;
		unsigned int index;
		char name[32];
		std::vector<int> unkData;
		std::vector<DvTransition> transition;
	};
}