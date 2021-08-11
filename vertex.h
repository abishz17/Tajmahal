#pragma once

#include "structure.h"
#include "colors.h"

class Vertex
{
public:
    vec4f pos;
    vec4f normal;
    vec4f color;

public:
    Vertex() = default;
    /*Vertex(const vec4f& pos)
		:
		pos(pos)
	{}
	Vertex(const vec4f& pos, const Vertex& src)
		:
		color(src.color),
		pos(pos)
	{}*/
    Vertex(const vec4f &pos, const vec4f &normal, const vec4f &c)
        : color(c),
          normal(normal),
          pos(pos)
    {
        // color.x = c.r;
        // color.y = c.g;
        // color.z = c.b;
    }
    Vertex &operator+=(const Vertex &rhs)
    {
        pos = pos + rhs.pos;
        color = color + rhs.color;
        return *this;
    }
    Vertex operator+(const Vertex &rhs) const
    {
        return Vertex(*this) += rhs;
    }
    Vertex &operator-=(const Vertex &rhs)
    {
        pos = pos - rhs.pos;
        color = color - rhs.color;
        return *this;
    }
    Vertex operator-(const Vertex &rhs) const
    {
        return Vertex(*this) -= rhs;
    }
    Vertex &operator*=(float rhs)
    {
        pos = pos * rhs;
        color = color * rhs;
        return *this;
    }
    Vertex operator*(float rhs) const
    {
        return Vertex(*this) *= rhs;
    }
    Vertex &operator/=(float rhs)
    {
        pos = pos / rhs;
        color = color / rhs;
        return *this;
    }
    Vertex operator/(float rhs) const
    {
        return Vertex(*this) /= rhs;
    }
};

// class Vertex
// {
// public:
// 	vec4f pos;
// 	vec4f tc;
// public:
// 	Vertex() = default;
// 	Vertex(const vec4f& pos)
// 		:
// 		pos(pos)
// 	{}
// 	Vertex(const vec4f& pos, const Vertex& src)
// 		:
// 		tc(src.tc),
// 		pos(pos)
// 	{}
// 	Vertex(const vec4f& pos, const vec4f& tc)
// 		:
// 		tc(tc),
// 		pos(pos)
// 	{}
// 	Vertex& operator+=(const Vertex& rhs)
// 	{
// 		pos = pos + rhs.pos;
// 		tc = tc + rhs.tc;
// 		return *this;
// 	}
// 	Vertex operator+(const Vertex& rhs) const
// 	{
// 		return Vertex(*this) += rhs;
// 	}
// 	Vertex& operator-=(const Vertex& rhs)
// 	{
// 		pos = pos - rhs.pos;
// 		tc = tc - rhs.tc;
// 		return *this;
// 	}
// 	Vertex operator-(const Vertex& rhs) const
// 	{
// 		return Vertex(*this) -= rhs;
// 	}
// 	Vertex& operator*=(float rhs)
// 	{
// 		pos = pos * rhs;
// 		tc = tc * rhs;
// 		return *this;
// 	}
// 	Vertex operator*(float rhs) const
// 	{
// 		return Vertex(*this) *= rhs;
// 	}
// 	Vertex& operator/=(float rhs)
// 	{
// 		pos = pos / rhs;
// 		tc = tc / rhs;
// 		return *this;
// 	}
// 	Vertex operator/(float rhs) const
// 	{
// 		return Vertex(*this) /= rhs;
// 	}
// };