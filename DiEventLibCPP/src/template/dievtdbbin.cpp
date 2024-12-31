#include "../../include/template/dievtdbbin.h"

dv::db::DiEventDataBase::Base* dv::db::DiEventDataBaseBinary::readBase(dv::internal::Reader* reader)
{
	bool bit = flags.test(2);
	bool descriptions = flags.test(1);
	DiEventDataBase::Base* base = new DiEventDataBase::Base();
	base->name = reader->readStringTableEntry(bit);
	if (descriptions) {
		unsigned int descriptionCount = reader->read<unsigned int>();
		for (auto y = 0; y < descriptionCount; y++) {
			std::string key = reader->readStringTableEntry(bit);
			std::string value = reader->readStringTableEntry(bit);
			base->descriptions.emplace(key, value);
		}
	}
	return base;
}

dv::db::DiEventDataBase::Field* dv::db::DiEventDataBaseBinary::readField(dv::internal::Reader* reader)
{
	bool bit = flags.test(2);
	DiEventDataBase::Base* fieldBase = readBase(reader);
	DiEventDataBase::Field::DataType type = (DiEventDataBase::Field::DataType)reader->read<int8_t>();
	DiEventDataBase::Field::DataType subtype = (DiEventDataBase::Field::DataType)reader->read<int8_t>();
	unsigned short size = reader->read<unsigned short>();
	switch (type) {
	case DiEventDataBase::Field::DataType::array:
	case DiEventDataBase::Field::DataType::arraysize: {
		std::string arraySizeField = reader->readStringTableEntry(bit);
		if (subtype == DiEventDataBase::Field::DataType::strct) {
			if (arraySizeField != "") {
				DiEventDataBase::FieldStructDynamicArray* field = new DiEventDataBase::FieldStructDynamicArray();
				field->name = fieldBase->name;
				field->descriptions = fieldBase->descriptions;
				field->dataType = type;
				field->subType = subtype;
				field->size = size;
				field->arraySizeField = arraySizeField;
				field->structValue.name = reader->readStringTableEntry(bit);
				unsigned int fieldCount = reader->read<unsigned int>();
				for (auto x = 0; x < fieldCount; x++)
					field->structValue.fields.push_back(readField(reader));
				return field;
				break;
			}
			DiEventDataBase::FieldStructArray* field = new DiEventDataBase::FieldStructArray();
			field->name = fieldBase->name;
			field->descriptions = fieldBase->descriptions;
			field->dataType = type;
			field->subType = subtype;
			field->size = size;
			field->structValue.name = reader->readStringTableEntry(bit);
			unsigned int fieldCount = reader->read<unsigned int>();
			for (auto x = 0; x < fieldCount; x++)
				field->structValue.fields.push_back(readField(reader));
			return field;
			break;
		}
		else {
			if (arraySizeField != "") {
				DiEventDataBase::FieldDynamicArray* field = new DiEventDataBase::FieldDynamicArray();
				field->name = fieldBase->name;
				field->descriptions = fieldBase->descriptions;
				field->dataType = type;
				field->subType = subtype;
				field->size = size;
				field->arraySizeField = arraySizeField;
				return field;
				break;
			}
			DiEventDataBase::FieldArray* field = new DiEventDataBase::FieldArray();
			field->name = fieldBase->name;
			field->descriptions = fieldBase->descriptions;
			field->dataType = type;
			field->subType = subtype;
			field->size = size;
			return field;
			break;
		}
		break;
	}
	case DiEventDataBase::Field::DataType::strct:
	{
		DiEventDataBase::FieldStruct* field = new DiEventDataBase::FieldStruct();
		field->name = fieldBase->name;
		field->descriptions = fieldBase->descriptions;
		field->dataType = type;
		field->structValue.name = reader->readStringTableEntry(bit);
		unsigned int fieldCount = reader->read<unsigned int>();
		for (auto x = 0; x < fieldCount; x++)
			field->structValue.fields.push_back(readField(reader));
		return field;
		break;
	}
	case DiEventDataBase::Field::DataType::enm:
	{
		DiEventDataBase::FieldEnum* field = new DiEventDataBase::FieldEnum();
		field->name = fieldBase->name;
		field->descriptions = fieldBase->descriptions;
		field->dataType = type;
		field->enumType.name = reader->readStringTableEntry(bit);
		field->enumType.dataType = subtype;
		unsigned int valueCount = reader->read<unsigned int>();
		for (auto x = 0; x < valueCount; x++) {
			std::string key = reader->readStringTableEntry();
			int value = reader->read<int>();
			field->enumType.values.emplace(key, value);
		}
		return field;
		break;
	}
	default:
	{
		DiEventDataBase::Field* field = new DiEventDataBase::Field();
		field->name = fieldBase->name;
		field->descriptions = fieldBase->descriptions;
		field->dataType = type;
		field->size = size;
		return field;
		break;
	}
	}
}

dv::db::DiEventDataBase::Node* dv::db::DiEventDataBaseBinary::readNode(dv::internal::Reader* reader)
{
	bool bit = flags.test(2);
	DiEventDataBase::Base* base = readBase(reader);
	DiEventDataBase::Node* node = new DiEventDataBase::Node();
	node->name = base->name;
	node->descriptions = base->descriptions;
	node->fullname = reader->readStringTableEntry(bit);
	node->nodeCategory = reader->read<unsigned int>();
	int fieldCount = reader->read<int>();
	for (auto y = 0; y < fieldCount; y++)
		node->fields.push_back(readField(reader));
	return node;
}

void dv::db::DiEventDataBaseBinary::read(const char* data, size_t size)
{
	dv::internal::Reader* reader = new dv::internal::Reader((char*)data, size);
	std::string signature = reader->readString(dv::internal::Reader::StringType::FixedLength, 7);
	if (signature != "DiEvtDB")
		throw std::runtime_error("Not a DiEvtDB file!");
	flags = std::bitset<8>(reader->read<int8_t>());
	bool bit = flags.test(2);
	bool descriptions = flags.test(1);
	version = reader->read<unsigned int>();
	uint16_t nodeCount = reader->read<uint16_t>();
	uint16_t elementCount = reader->read<uint16_t>();
	for (auto x = 0; x < nodeCount; x++)
		nodes.push_back(*readNode(reader));
	for (auto x = 0; x < elementCount; x++)
		elements.push_back(*readNode(reader));
	reader->dispose();
}
