#pragma once
#include <cstddef>

namespace dv {
	template<typename T>
	class DvObject {
	public:
		int count;
		int size;
	private:
		long long unk0;

	public:
		inline T* getItems() {
			return reinterpret_cast<T*>((uintptr_t)this + 16);
		}
	};
}