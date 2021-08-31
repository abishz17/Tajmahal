#pragma once

#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>
#include <math.h>
#include <array>

#include "colors.h"
// #include "line.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int CUBE_HEIGHT = 500;

//no of sides of a polygon
const int n = 8;
const int m = 4; //matrix size for 3D i.e 4x4

float deg_to_rad(float degree)
{
    return (3.1416 * degree / 180);
}

float rad_to_deg(float radian)
{
    return (180 * radian) / 3.1416;
}

struct vec2f
{  
    float x, y;
};

struct mat4f
{
    float matrix4[4][4];
};

struct vec4f
{
    float x, y, z, w;
    float xfactor = SCR_WIDTH / 8.0f;
    float yfactor = SCR_HEIGHT / 8.0f;
    float cube_height = CUBE_HEIGHT / 4.0f;

    // vec4f() : x(0.0),y(0.0),z(0.0),w(1.0){}

    vec4f translatetoScreenOrigin()
    {
        x = (x + 1.0f) * xfactor;
        y = (y + 1.0f) * yfactor;
        // x = (x) * xfactor;
        // y = (y) * yfactor;
        z = (z) * cube_height;
        return {x,y,z};
    }
    vec4f inverse()
    {
        return {-x, -y, -z, w};
    }
    //vector addition of a point
    vec4f operator+(vec4f translate)
    {
        return {x + translate.x, y + translate.y, z + translate.z, w};
    }
    //inverse the vector direction
    vec4f operator-(vec4f sub)
    {
        return {x - sub.x, y - sub.y, z - sub.z, w};
    }
    vec4f operator*(float mul)
    {
        return {float(mul * x), float(mul * y), float(mul * z), w};
    }
    vec4f operator*(vec4f mul)
    {
        return {float(mul.x * x), float(mul.y * y), float(mul.z * z), w};
    }
    vec4f operator/(float div)
    {
        return {float(x / div), float(y / div), float(z / div), w};
    }
    vec4f scaleProduct(float pt)
    {
        return {x * pt, y * pt, z * pt, w};
    }
    vec4f crossProduct(vec4f pt)
    {
        vec4f ret;
        ret.x = y * pt.z - z * pt.y;
        ret.y = z * pt.x - x * pt.z;
        ret.z = x * pt.y - y * pt.x;
        // std::cout<<ret;
        // ret = normalize(ret);
        return ret;
    }

    friend std::ostream &operator<<(std::ostream &out, vec4f &vec)
    {
        out << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )\n";
        return out;
    }

    vec4f normalize()
    {
        float mag = x * x + y * y + z * z;
        mag = pow(mag, 0.5);
        return {x/mag, y/mag, z/mag};
    }
};

float dotProduct(vec4f pt1, vec4f pt2)
{
    return {pt1.x * pt2.x + pt1.y * pt2.y + pt1.z * pt2.z};
}

struct Triangle{
    vec4f vertices[3];
    vec4f normals[3];
    vec2f texCoords[3];
    Color color;
    vec4f vertex_intensity[3];
    float zbuffer;

    Triangle(const Triangle &tri)
    {
        for(int i=0;i<3;i++){
            vertices[i] = tri.vertices[i];
            normals[i] = tri.normals[i];
            texCoords[i] = tri.texCoords[i];
        }
        color = tri.color;
    }

    Triangle()
    {
        color = {0.8,0.8,0.8};

        vertices[0] = vec4f{0, 0, 0};
        vertices[1] = vec4f{0, 0, 0};
        vertices[2] = vec4f{0, 0, 0};
    }

    Triangle(vec4f a, vec4f b, vec4f c)
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }

    Triangle(vec4f a, vec4f b, vec4f c, Color mcolor)
    {
        color = mcolor;
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }

    void setVertex(vec4f a, vec4f b, vec4f c)
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }

    void setNormals(vec4f na, vec4f nb, vec4f nc)
    {
        normals[0] = na;
        normals[1] = nb;
        normals[2] = nc;
    }

    void setTexCoords(vec2f uv1, vec2f uv2, vec2f uv3)
    {
        texCoords[0] = uv1;
        texCoords[1] = uv2;
        texCoords[2] = uv3;

    }

    void setIntensity(std::vector<float> intense);
    void gauravRasterize1();
    void gauravRasterize2();
};
