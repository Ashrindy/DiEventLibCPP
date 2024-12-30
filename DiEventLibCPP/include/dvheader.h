#pragma once
#include "dvcommon.h"
#include "dvresource.h"

namespace dv {
	struct DvHeader {
	public:
		internal::ptr32<RawDvCommon, 0 - 0x20> dvCommon;
		internal::ptr32<DvObject<Resource>, 0x04 - 0x20> dvResource;
	private:
		char padding[0x18];
	};
}