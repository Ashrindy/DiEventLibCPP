#pragma once
#include "dvcommon.h"
#include "dvresource.h"

struct DvHeader {
public:
	ptr32<RawDvCommon, 0 - 0x20> dvCommon;
	ptr32<DvObject<Resource>, 0x04 - 0x20> dvResource;
private:
	char padding[0x18];
};