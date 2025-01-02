#pragma once
#include "../internal.h"
#include <vector>
#include <string>
#include <map>

namespace dv::db {
	static size_t dataTypeSizes[] =
	{
		sizeof(uint8_t),				//u8
		sizeof(int8_t),					//s8
		sizeof(uint16_t),				//u16
		sizeof(int16_t),				//s16
		sizeof(uint32_t),				//u32
		sizeof(int32_t),				//s32
		sizeof(float),					//f32
		sizeof(internal::Vector2),		//vec2
		sizeof(internal::Vector3),		//vec3
		sizeof(internal::Vector4),		//vec4
		sizeof(internal::Matrix4x4),	//mat4x4
		sizeof(0),						//curve
		sizeof(0),						//str
		sizeof(0),						//enm
		sizeof(0),						//strct
		sizeof(internal::Guid),			//guid
		sizeof(0),						//array
		sizeof(0),						//arraysize
		sizeof(bool),					//boolean
		sizeof(internal::RGBA8),		//rgba8
		sizeof(internal::RGB32F),		//rgb32
		sizeof(internal::RGBA32),		//rgba32
		sizeof(0),						//padding
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