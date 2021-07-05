#pragma once

#include <memory>
#include <math.h>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Ptr = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Ptr<T> CreatePtr(Args &&...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

enum class SizeUnits
{
	Byte = 0,
	Kilobyte,
	Megabyte,
	Gigabyte
};

inline float BytesTo(float bytes, SizeUnits entities)
{
	return bytes / pow(1024.0f, static_cast<int>(entities));
}

inline float KilobytesToMegabytes(float kilobytes)
{
	return kilobytes / 1024.0f;
}