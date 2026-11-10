#pragma once

namespace dv::internal {
	template<typename T, long offset>
	struct ptr32 {
		int ptr;

		inline T* get() {
			return reinterpret_cast<T*>(((uintptr_t)this - offset) + ptr);
		}
	};
}