#pragma once
#include "dvobject.h"
#include "dvdisableframe.h"
#include "dvpage.h"
#include "dvnode.h"
#include "memory.h"

namespace dv {
	struct RawDvCommon {
	private:
		long long unk;
	public:
		float frameStart;
		float frameEnd;
		int drawNodeNumber;
		internal::ptr32<DvObject<float>, 0x14> cuts;
		internal::ptr32<DvObject<char>, 0x18> pages;
		internal::ptr32<DvObject<DvDisableFrame>, 0x1C> disableFrameInfo;
		internal::ptr32<DvObject<float>, 0x20> resourceCutInfo;
		internal::ptr32<DvObject<float>, 0x24> soundCutInfo;
		internal::ptr32<RawDvNode, 0x28> node;
		float chainCameraIn;
		float chainCameraOut;
		int type;
		int skipPointTick;
	};

	struct DvCommon {
	public:
		float frameStart;
		float frameEnd;
		int drawNodeNumber;
		std::vector<float> cuts;
		std::vector<DvPage> pages;
		std::vector<DvDisableFrame> disableFrameInfo;
		std::vector<float> resourceCutInfo;
		std::vector<float> soundCutInfo;
		DvNode* node;
		float chainCameraIn;
		float chainCameraOut;
		int type;
		int skipPointTick;
	};
}