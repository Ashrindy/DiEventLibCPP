#pragma once
#include "dievtdb.h"
#include "../internal.h"
#include <stdexcept>
#include <bitset>

class DiEventDataBaseBinary {
private:
	std::bitset<8> flags;
	DiEventDataBase::Base* readBase(Reader* reader);
	DiEventDataBase::Field* readField(Reader* reader);
	DiEventDataBase::Node* readNode(Reader* reader);

public:
	unsigned int version = 1;
	std::vector<DiEventDataBase::Node> nodes;
	std::vector<DiEventDataBase::Node> elements;

	void read(const char* data, size_t size);
};