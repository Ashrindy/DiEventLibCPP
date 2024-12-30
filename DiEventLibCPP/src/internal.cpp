#include "../include/internal.h"

std::string dv::internal::Reader::readString(StringType type, size_t length)
{
	std::string result;
	switch (type) {
	case StringType::NullTerminated:
	{
		size_t size = 0;
		while (buffer[position + size] != '\0') {
			if ((position + size) > bufferSize)
				throw std::runtime_error("Attempt to read beyond buffer size!");
			size++;
		}
		result = std::string(&buffer[position], size);
		position += (size + 1);
		break;
	}
	case StringType::FixedLength:
	{
		if ((position + length) > bufferSize)
			throw std::runtime_error("Attempt to read beyond buffer size!");
		result = std::string(&buffer[position], length);
		position += length;
		break;
	}
	}

	return result;
}

void dv::internal::Reader::seek(size_t ptr)
{
	if (ptr > bufferSize)
		throw std::runtime_error("Attempt to read beyond buffer size!");
	position = ptr;
}

std::string dv::internal::Reader::readStringTableEntry(bool bit)
{
	size_t pos = bit ? read<long long>() : read<int>();
	size_t prepos = position;
	if (pos != 0) {
		seek(pos);
		std::string value = readString(StringType::NullTerminated);
		seek(prepos);
		return value;
	}
	else
		return std::string("");
}

void dv::internal::Reader::skip(size_t offset)
{
	if ((position + offset) > bufferSize)
		throw std::runtime_error("Attempt to read beyond buffer size!");
	position += offset;
}

void dv::internal::Reader::dispose()
{
	if (buffer)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}
