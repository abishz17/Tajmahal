#pragma once

#include "vector.h"


void matrixMul(vec4f &p, float matrix[m][m])
{
    vec4f dummy;
    dummy.x = matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z + matrix[0][3];
    dummy.y = matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z + matrix[1][3];
    dummy.z = matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z + matrix[2][3];
    p = std::move(dummy);
}

void translate(vec4f &point, vec4f translate)
{
    point= point+ translate;
}

void scale(vec4f &point, float scale)
{

    point = point.scaleProduct(scale);
}

void x_rotation(vec4f &point, float angle)
{
    angle = converttoRad(angle);
    float compositeMatrix[m][m] = {{1, 0, 0, 0},
                             {0, cos(angle), -sin(angle), 0},
                             {0, sin(angle), cos(angle), 0},
                             {0, 0, 0, 1}};
    matrixMul(point, compositeMatrix);
}

void y_rotation(vec4f &point, float angle)
{
    angle = converttoRad(angle);
    float compositeMatrix[m][m] = {{cos(angle), 0, sin(angle), 0},
                             {0, 1, 0, 0},
                             {-sin(angle),0 , cos(angle), 0},
                             {0, 0, 0, 1}};
    matrixMul(point, compositeMatrix);
}

void z_rotation(vec4f &point, float angle)
{
    angle = converttoRad(angle);
    float compositeMatrix[m][m] = {{cos(angle), -sin(angle), 0, 0},
                             {sin(angle), cos(angle), 0, 0},
                             {0, 0, 1, 0},
                             {0, 0, 0, 1}};
    matrixMul(point, compositeMatrix);
}

