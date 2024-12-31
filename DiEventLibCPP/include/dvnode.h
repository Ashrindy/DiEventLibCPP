#pragma once
#include "internal.h"
#include "template/dievtdb.h"
#include <vector>
#include <map>
#include <optional>

struct Path {
public:
	float mat[4][4];
	unsigned int flags;
private:
	char unknown0[12];
};

namespace dv {
	struct RawDvNode {
	public:
		internal::Guid guid;
		unsigned int category;
		unsigned int nodeSize;
		unsigned int childCount;
		unsigned int nodeFlags;
		int priority;
	private:
		char unknown0[12];
	public:
		char name[64];

		template<typename T>
		T* getData() {
			return reinterpret_cast<T*>(((uintptr_t)this) + sizeof(RawDvNode));
		}

		std::vector<RawDvNode*> getChildNodes() {
			std::vector<RawDvNode*> childnodes;
			uintptr_t pos = (((uintptr_t)this) + sizeof(RawDvNode) + nodeSize * 4);
			for (auto x = 0; x < childCount; x++) {
				RawDvNode* node = (RawDvNode*)pos;
				pos += sizeof(RawDvNode);
				pos += node->nodeSize * 4;
				for (auto y = 0; y < node->childCount; y++) {
					skipChildNodes(&pos);
				}
				childnodes.push_back(node);
			}
			return childnodes;
		}

	private:
		void skipChildNodes(uintptr_t* pos) {
			RawDvNode* node = (RawDvNode*)(*pos);
			(*pos) += sizeof(RawDvNode);
			(*pos) += node->nodeSize * 4;
			for (auto x = 0; x < node->childCount; x++) {
				skipChildNodes(pos);
			}
		}
	};

	struct DvNode {
	public:
		enum class Category : unsigned int {
			Path = 1,
			Camera = 3,
			CameraMotion,
			Character,
			CharacterMotion,
			ModelCustom = 8,
			MotionModel = 10,
			ModelNode,
			Element
		};

		internal::Guid guid;
		unsigned int category;
		unsigned int nodeFlags;
		int priority;
		char name[64];
		size_t dataSize;
		char* data;
		std::vector<DvNode> childNodes;

		template<typename T>
		T* getData() {
			return reinterpret_cast<T*>(data);
		}

		struct Field {
		public:
			char* value;
			std::optional<std::map<std::string, Field>*> structValue;
			std::optional<std::map<std::string, int>> enumValue;
			std::map<std::string, std::string> descriptions;
			db::DiEventDataBase::Field::DataType dataType;
			std::optional<db::DiEventDataBase::Field::DataType> subType;
			std::optional<size_t> size;
		};

	private:
		Field* getTemplateField(db::DiEventDataBase::Field* x, size_t* pos) {
			if (x->dataType != db::DiEventDataBase::Field::DataType::padding) {
				Field* fld = new Field();
				fld->dataType = x->dataType;
				fld->descriptions = x->descriptions;
				fld->value = &data[*pos];
				size_t skip = 0;
				switch (x->dataType) {
				case db::DiEventDataBase::Field::DataType::curve:
					fld->size = x->size;
					skip = sizeof(float) * x->size;
					break;
				case db::DiEventDataBase::Field::DataType::strct:
				{
					std::map<std::string, Field> structValue{};
					auto* structFld = (db::DiEventDataBase::FieldStruct*)x;
					for (auto& z : structFld->structValue.fields)
						if (auto* y = getTemplateField(z, pos))
							structValue.emplace(z->name, *y);
					fld->structValue = &structValue;
					break;
				}
				case db::DiEventDataBase::Field::DataType::str:
					fld->size = x->size;
					skip = x->size;
					break;
				case db::DiEventDataBase::Field::DataType::array:
				{
					fld->size = x->size;
					auto* arrayFld = (db::DiEventDataBase::FieldArray*)x;
					if (arrayFld->subType == db::DiEventDataBase::Field::DataType::strct) {
						auto* arraystructFld = (db::DiEventDataBase::FieldStructArray*)x;
						std::vector<std::map<std::string, Field>> structValues{};
						for (auto i = 0; i < arrayFld->size; i++) {
							std::map<std::string, Field> structValue{};
							for (auto& z : arraystructFld->structValue.fields)
								if (auto* y = getTemplateField(z, pos))
									structValue.emplace(z->name, *y);
							structValues.push_back(structValue);
						}
						fld->structValue = structValues.data();
						break;
					}
					skip = x->size * db::dataTypeSizes[(int)x->dataType];
					break;
				}
				case db::DiEventDataBase::Field::DataType::enm:
				{
					auto* enmFld = (db::DiEventDataBase::FieldEnum*)x;
					fld->subType = enmFld->enumType.dataType;
					fld->enumValue = enmFld->enumType.values;
					skip = db::dataTypeSizes[(int)enmFld->enumType.dataType];
					break;
				}
				default:
					skip = db::dataTypeSizes[(int)fld->dataType];
					break;
				}
				*pos += skip;
				return fld;
			}
			else if(x->dataType == db::DiEventDataBase::Field::DataType::padding) {
				*pos += x->size;
				return nullptr;
			}
		}

	public:
		std::map<std::string, Field> getTemplateData(db::DiEventDataBase dievtdb) {
			std::map<std::string, Field> result{};
			db::DiEventDataBase::Node* node = new db::DiEventDataBase::Node();
			for (auto& x : dievtdb.nodes) {
				if (x.nodeCategory == category) {
					node = &x;
					break;
				}
			}
			if (node) {
				size_t pos = 0;
				for (auto& x : node->fields)
					if(auto* y = getTemplateField(x, &pos))
						result.emplace(x->name, *y);
				auto isElement = node->descriptions.find("nodeElement");
				if (isElement != node->descriptions.end()) {
					int type = *((int*)result[node->descriptions["nodeElement"]].value);
					db::DiEventDataBase::Node* element = new db::DiEventDataBase::Node();
					for (auto& x : dievtdb.elements) {
						if (x.nodeCategory == type) {
							element = &x;
							break;
						}
					}
					if (element) {
						for (auto& x : element->fields)
							if (auto* y = getTemplateField(x, &pos))
								result.emplace(x->name, *y);
					}
				}
			}
			return result;
		}
	};
}