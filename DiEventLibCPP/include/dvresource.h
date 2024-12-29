#pragma once
#include "internal.h"

struct Resource {
public:
	enum class Type : int {
		Character = 2,
		CameraMotion = 4,
		CharacterMotion = 7,
		Model = 10
	};

	Guid guid;
	Type type;
	int flags0;
	int flags1;
	char filename[192];
	char unkData[596];
};