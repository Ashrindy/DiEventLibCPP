#pragma once
#include "../internal.h"
#include <vector>
#include <string>
#include <map>

namespace dv::db {
	static size_t dataTypeSizes[] =
	{
		sizeof(uint8_t),
		sizeof(int8_t),
		sizeof(uint16_t),
		sizeof(int16_t),
		sizeof(uint32_t),
		sizeof(int32_t),
		sizeof(float),
		sizeof(internal::Vector2),
		sizeof(internal::Vector3),
		sizeof(internal::Vector4),
		sizeof(internal::Matrix4x4),
		sizeof(0),
		sizeof(0),
		sizeof(0),
		sizeof(0),
		sizeof(internal::Guid),
		sizeof(0),
		sizeof(0),
		sizeof(bool),
		sizeof(internal::RGBA8),
		sizeof(internal::RGB32F),
		sizeof(internal::RGBA32),
		sizeof(0),
	};

	class DiEventDataBase {
	public:
		struct Base {
		public:
			std::string name;
			std::map<std::string, std::string> descriptions;
		};

		struct Field : Base {
		public:
			enum class DataType : int8_t {
				none = -1,
				u8,
				s8,
				u16,
				s16,
				u32,
				s32,
				f32,
				vec2,
				vec3,
				vec4,
				mat4x4,
				curve,
				str,
				enm,
				strct,
				guid,
				array,
				arraysize,
				boolean,
				rgba8,
				rgb32,
				rgba32,
				padding
			};

			DataType dataType;
			unsigned short size;

			struct Enum {
			public:
				std::string name;
				DataType dataType;
				std::map<std::string, int> values;
			};

			struct Struct {
			public:
				std::string name;
				std::vector<Field*> fields;
			};
		};

		struct FieldEnum : Field {
		public:
			Field::Enum enumType;
		};

		struct FieldStruct : Field {
		public:
			Field::Struct structValue;
		};

		struct FieldArray : Field {
		public:
			Field::DataType subType;
		};

		struct FieldStructArray : Field {
		public:
			Field::DataType subType;
			Field::Struct structValue;
		};

		struct FieldDynamicArray : Field {
		public:
			Field::DataType subType;
			std::string arraySizeField;
		};

		struct FieldStructDynamicArray : Field {
		public:
			Field::DataType subType;
			std::string arraySizeField;
			Field::Struct structValue;
		};

		struct FieldArraySize : Field {
		public:
			Field::DataType subType;
			std::string arraySizeField;
		};

		struct Node : Base {
		public:
			std::string fullname;
			unsigned int nodeCategory;
			std::vector<Field*> fields;
		};

	public:
		unsigned int version = 1;
		std::vector<Node> nodes;
		std::vector<Node> elements;

		void read(const char* data, size_t size);
	};
}