#pragma once
#include "dievtdb.h"
#include "../internal.h"
#include <stdexcept>
#include <bitset>

namespace dv::db {
	class DiEventDataBaseBinary {
	private:
		std::bitset<8> flags;
		DiEventDataBase::Base* readBase(dv::internal::Reader* reader);
		DiEventDataBase::Field* readField(dv::internal::Reader* reader);
		DiEventDataBase::Node* readNode(dv::internal::Reader* reader);

	public:
		unsigned int version = 1;
		std::vector<DiEventDataBase::Node> nodes;
		std::vector<DiEventDataBase::Node> elements;

		void read(const char* data, size_t size);
	};
}