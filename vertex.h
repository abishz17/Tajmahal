#pragma once

#include "vector.h"
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

