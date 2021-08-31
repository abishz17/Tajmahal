#pragma once

#include "vector.h"

mat4f Matrix_MakeRotationX(float fAngleRad);
mat4f  Matrix_MakeRotationY(float fAngleRad);
mat4f Matrix_MakeRotationZ(float fAngleRad);


vec4f multiply(mat4f matrix, vec4f &point)
{
    vec4f dummy;
    dummy.x = matrix.matrix4[0][0] * point.x + matrix.matrix4[0][1] * point.y + matrix.matrix4[0][2] * point.z + matrix.matrix4[0][3];
    dummy.y = matrix.matrix4[1][0] * point.x + matrix.matrix4[1][1] * point.y + matrix.matrix4[1][2] * point.z + matrix.matrix4[1][3];
    dummy.z = matrix.matrix4[2][0] * point.x + matrix.matrix4[2][1] * point.y + matrix.matrix4[2][2] * point.z + matrix.matrix4[2][3];
    dummy.w = point.w;
    return dummy;
}

mat4f multiply(mat4f a, mat4f b)
{
    mat4f product = {0};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                product.matrix4[i][j] += a.matrix4[i][k] * b.matrix4[k][j];
            }
        }
    }
    return product;
}

mat4f perspectiveMatrix(vec4f eye)
{
    float zprp = eye.z, xprp = eye.x, yprp = eye.y;
    float zvp = 0;
    float dp = zprp - zvp;
    mat4f persmatrix = {{{1, 0, xprp / dp, -xprp * zvp / dp},
                         {0, 1, yprp / dp, -yprp * zvp / dp},
                         {0, 0, -zvp / dp, zvp * (zprp / dp)},
                         {0, 0, -1 / dp, zprp / dp}}};
    return persmatrix;
}

mat4f newPerspective(float fov, float aspect)
{
    float zNear = 0.1;
    float zFar = -100.0f;
    float zRange = zNear - zFar;
    mat4f projection = {{{1 / (aspect * tan(fov / 2)), 0, 0, 0},
                         {0, 1 / tan(fov / 2), 0, 0},
                         {0, 0, (zFar + zNear) / zRange, (2 * zFar * zNear) / zRange},
                         {0, 0, -1, 0}}};

   
    return projection;

}

mat4f rotateMatrix(float yaw, float pitch = 0, float roll = 0)
{
    
    mat4f rotation = {{{cos(yaw) * cos(pitch), cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll), cos(yaw) * sin(pitch) * cos(roll) + sin(yaw) * sin(roll), 0},
                       {sin(yaw) * cos(pitch), sin(yaw) * sin(pitch) * sin(roll) + cos(yaw) * cos(roll), sin(yaw) * sin(pitch) * cos(roll) - cos(yaw) * sin(roll), 0},
                       {-sin(pitch), cos(pitch) * sin(roll), cos(pitch) * cos(roll), 0},
                       {0, 0, 0, 1}}};
    return rotation;
}

mat4f orthoprojectMatrix()
{
    mat4f ortho = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 1}}};
    return ortho;
}

std::vector<float> interpolate(float i0, float d0, float il, float dl)
{
    std::vector<float> values;

    if (i0 == il)
    {
        values.push_back(d0);
        return values;
    }

    float a = (dl - d0) / (il - i0);
    float d = d0;

    for (int i = i0; i < il; i++)
    {
        values.push_back(d);
        d = d + a;
    }
    return values;
}

vec4f getnormal(vec4f point1, vec4f point2, vec4f point3)
{
    vec4f ver1 = point1 - point2;
    vec4f ver2 = point1 - point3;;

    vec4f normal = (ver1.crossProduct(ver2)).normalize();
    return normal;
}


   mat4f  Matrix_MakeRotationX(float fAngleRad)
	{
		mat4f matrix;
		matrix.matrix4[0][0] = 1.0f;
		matrix.matrix4[1][1] = cosf(fAngleRad);
		matrix.matrix4[1][2] = sinf(fAngleRad);
		matrix.matrix4[2][1] = -sinf(fAngleRad);
		matrix.matrix4[2][2] = cosf(fAngleRad);
		matrix.matrix4[3][3] = 1.0f;
		return matrix;
	}
	mat4f   Matrix_MakeRotationY(float fAngleRad)
	{
		mat4f matrix;
		matrix.matrix4[0][0] = cosf(fAngleRad);
		matrix.matrix4[0][2] = sinf(fAngleRad);
		matrix.matrix4[2][0] = -sinf(fAngleRad);
		matrix.matrix4[1][1] = 1.0f;
		matrix.matrix4[2][2] = cosf(fAngleRad);
		matrix.matrix4[3][3] = 1.0f;
		return matrix;
	}

	mat4f Matrix_MakeRotationZ(float fAngleRad)
	{
		mat4f matrix;
		matrix.matrix4[0][0] = cosf(fAngleRad);
		matrix.matrix4[0][1] = sinf(fAngleRad);
		matrix.matrix4[1][0] = -sinf(fAngleRad);
		matrix.matrix4[1][1] = cosf(fAngleRad);
		matrix.matrix4[2][2] = 1.0f;
		matrix.matrix4[3][3] = 1.0f;
		return matrix;
	}


    