#pragma once

template<typename T>
struct ManagerTemplate
{
	char pad_0000[0x4];
	T** entities;
	size_t size;
	size_t max_size;
};