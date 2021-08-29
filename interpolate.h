#pragma once
template <typename T>
constexpr T interpolate(const T &src, const T &dst, float alpha)
{
    return src + (dst - src) * alpha;
}