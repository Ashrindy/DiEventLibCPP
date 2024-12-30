#pragma once

template<typename T, long offset>
class ptr32 {
public:
	int ptr;

	T* get() {
		return reinterpret_cast<T*>(((uintptr_t)this - offset) + ptr);
	}
};