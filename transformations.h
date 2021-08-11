#pragma once

#include "structure.h"

//transformation functions
void transate_polygon(vec4f &pt, vec4f translate);
void scale_polygon(vec4f &pt, vec4f scale);
void rotateX(vec4f &pt, float angle);
void rotateY(vec4f &pt, float angle);
void rotateZ(vec4f &pt, float angle);
void matrix_product(vec4f &p, float matrix[m][m]);

///////////////////            3D transformation        ///////////////////

void matrix_product(vec4f &p, float matrix[m][m])
{
    vec4f temp;
    temp.x = matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z + matrix[0][3];
    temp.y = matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z + matrix[1][3];
    temp.z = matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z + matrix[2][3];
    p = std::move(temp);
}

void transate_polygon(vec4f &pt, vec4f translate)
{
    pt = pt + translate;
}

void scale_polygon(vec4f &pt, float scale)
{

    pt = pt.scaleProduct(scale);
}

void rotateX(vec4f &pt, float angle)
{
    angle = deg_to_rad(angle);
    float composite[m][m] = {{1, 0, 0, 0},
                             {0, cos(angle), -sin(angle), 0},
                             {0, sin(angle), cos(angle), 0},
                             {0, 0, 0, 1}};
    matrix_product(pt, composite);
}

void rotateY(vec4f &pt, float angle)
{
    angle = deg_to_rad(angle);
    float composite[m][m] = {{cos(angle), 0, sin(angle), 0},
                             {0, 1, 0, 0},
                             {-sin(angle),0 , cos(angle), 0},
                             {0, 0, 0, 1}};
    matrix_product(pt, composite);
}

void rotateZ(vec4f &pt, float angle)
{
    angle = deg_to_rad(angle);
    float composite[m][m] = {{cos(angle), -sin(angle), 0, 0},
                             {sin(angle), cos(angle), 0, 0},
                             {0, 0, 1, 0},
                             {0, 0, 0, 1}};
    matrix_product(pt, composite);
}

mat4f x_rotation(float pitch){
        mat4f xrotation = {{
            {1, 0,           0,             0},
            {0, cos(pitch), -sin(pitch),    0},
            {0, sin(pitch),  cos(pitch),    0},
            {0, 0,           0,             1}
        }};
        return xrotation;
    }

    mat4f y_rotation(float yaw){
        mat4f yrotation = {{
            {cos(yaw),  0,  sin(yaw),   0},
            {0,         1,  0,          0},
            {-sin(yaw), 0,  cos(yaw),   0},
            {0,         0,  0,          1}
        }};
        return yrotation;
    }

    mat4f z_rotation(float roll){
        mat4f zrotation = {{
            {cos(roll), 0,  -sin(roll), 0},
            {sin(roll), 0,  cos(roll),  0},
            {0,         1,  0,          0},
            {0,         0,  0,          1}
        }};
        return zrotation;
    }
