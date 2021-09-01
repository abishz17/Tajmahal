#pragma once

#include "vector.h"

mat4x4 Matrix_MakeRotationX(float fAngleRad);
mat4x4  Matrix_MakeRotationY(float fAngleRad);
mat4x4 Matrix_MakeRotationZ(float fAngleRad);

//Multiply between vec4f and matrix/////////////
vec4f multiply(mat4x4 matrix, vec4f &v)
{
    vec4f dummy;
    dummy.x = matrix.mat[0][0] * v.x + matrix.mat[0][1] * v.y + matrix.mat[0][2] * v.z + matrix.mat[0][3];
    dummy.y = matrix.mat[1][0] * v.x + matrix.mat[1][1] * v.y + matrix.mat[1][2] * v.z + matrix.mat[1][3];
    dummy.z = matrix.mat[2][0] * v.x + matrix.mat[2][1] * v.y + matrix.mat[2][2] * v.z + matrix.mat[2][3];
    dummy.w = v.w;
    return dummy;
}


////Multipplication between 2 matrices//////////////
mat4x4 multiply(mat4x4 a, mat4x4 b)
{
    mat4x4 result = {0};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result.mat[i][j] += a.mat[i][k] * b.mat[k][j];
            }
        }
    }
    return result;
}


mat4x4 perspectiveMatrix(float fov, float aspect)
{
    float zNear = 0.1;
    float zFar = -100.0f;
    float zRange = zNear - zFar;
    mat4x4 projection = {{{1 / (aspect * tan(fov / 2)), 0, 0, 0},
                         {0, 1 / tan(fov / 2), 0, 0},
                         {0, 0, (zFar + zNear) / zRange, (2 * zFar * zNear) / zRange},
                         {0, 0, -1, 0}}};

   
    return projection;

}







//    mat4x4  Matrix_MakeRotationX(float fAngleRad)
// 	{
// 		mat4x4 matrix;
// 		matrix.mat[0][0] = 1.0f;
// 		matrix.mat[1][1] = cosf(fAngleRad);
// 		matrix.mat[1][2] = sinf(fAngleRad);
// 		matrix.mat[2][1] = -sinf(fAngleRad);
// 		matrix.mat[2][2] = cosf(fAngleRad);
// 		matrix.mat[3][3] = 1.0f;
// 		return matrix;
// 	}
// 	mat4x4   Matrix_MakeRotationY(float fAngleRad)
// 	{
// 		mat4x4 matrix;
// 		matrix.mat[0][0] = cosf(fAngleRad);
// 		matrix.mat[0][2] = sinf(fAngleRad);
// 		matrix.mat[2][0] = -sinf(fAngleRad);
// 		matrix.mat[1][1] = 1.0f;
// 		matrix.mat[2][2] = cosf(fAngleRad);
// 		matrix.mat[3][3] = 1.0f;
// 		return matrix;
// 	}

// 	mat4x4 Matrix_MakeRotationZ(float fAngleRad)
// 	{
// 		mat4x4 matrix;
// 		matrix.mat[0][0] = cosf(fAngleRad);
// 		matrix.mat[0][1] = sinf(fAngleRad);
// 		matrix.mat[1][0] = -sinf(fAngleRad);
// 		matrix.mat[1][1] = cosf(fAngleRad);
// 		matrix.mat[2][2] = 1.0f;
// 		matrix.mat[3][3] = 1.0f;
// 		return matrix;
// 	}


    