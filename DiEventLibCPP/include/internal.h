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
}