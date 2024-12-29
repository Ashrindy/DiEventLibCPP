#pragma once

template<typename T, long offset>
class ptr32 {
private:
	int ptr;

public:
	T* get() {
		return reinterpret_cast<T*>(((uintptr_t)this - offset) + ptr);
	}
};