#pragma once
#include <iostream>
#include <stdexcept>
#include <fstream>

namespace dv::internal {
	class Reader {
	public:
		enum class StringType : uint8_t {
			NullTerminated,
			FixedLength
		};

		const char* buffer;
		size_t bufferSize;
		size_t position;

		Reader(const char* data, size_t dataSize) : buffer(data), bufferSize(dataSize), position(0) {}

		template<typename T>
		T read() {
			if (position + sizeof(T) > bufferSize)
				throw std::runtime_error("Attempt to read beyond buffer size!");
			T value;
			std::memcpy(&value, buffer + position, sizeof(T));
			position += sizeof(T);
			return value;
		}

		std::string readString(StringType type, size_t length = 0);
		void seek(size_t ptr);
		std::string readStringTableEntry(bool bit = true);
		void skip(size_t offset);
		void dispose();
	};

	struct Guid {
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t Data4[8];

		bool operator==(const Guid& other) const {
			return Data1 == other.Data1 &&
				Data2 == other.Data2 &&
				Data3 == other.Data3 &&
				std::equal(std::begin(Data4), std::end(Data4), std::begin(other.Data4));
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
		Matrix4x4() { std::fill(&m[0][0], &m[0][0] + 16, 0.0f); }
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