#pragma once
#include <string.h>

namespace dv::internal {
	struct Guid {
		char data[16];

		inline bool operator==(const Guid& other) const {
			return memcmp(data, other.data, 16) == 0;
		}
	};

	struct Buffer {
	public:
		char* data;
		size_t size;
	};


	struct Vector2 {
	public:
		float x, y;
		Vector2(float x = 0, float y = 0) : x(x), y(y) {}
	};

	struct Vector3 {
	public:
		float x, y, z;
		Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
	};

	struct Vector4 {
	public:
		float x, y, z, w;
		Vector4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}
	};

	struct Matrix4x4 {
	public:
		float m[4][4];
		Matrix4x4() { memset(m, sizeof(m), 0); }
	};

	struct RGBA8 {
	public:
		char r, g, b, a;
		RGBA8(char r = 0, char g = 0, char b = 0, char a = 0) : r(r), g(g), b(b), a(a) {}
	};

	struct RGB32F {
	public:
		float r, g, b;
		RGB32F(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}
	};

	struct RGBA32 {
	public:
		unsigned int r, g, b, a;
		RGBA32(unsigned int r = 0, unsigned int g = 0, unsigned int b = 0, unsigned int a = 0) : r(r), g(g), b(b), a(a) {}
	};
}