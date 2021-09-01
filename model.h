#pragma once

#include<fstream>
#include<sstream>
#include<algorithm>
#include "camera.h"
#include "3dtransformations.h"
#include "modeltransformation.h"
#include "triangleRasterize.h"



class Object
{
private:
    std::vector<Triangle> triangles;
    std::vector<Triangle> projectedTriangles;

public:
    void loadObj(std::string);
    void originConversion();
    void rotateObject(float);
    void translateObject(vec4f);
    void scaleObject(float);
    void updateModel(mat4x4 &, mat4x4 &);
    bool backfaceDetection(Triangle &triangle);
    void draw();
    Camera *camera;
};

void Object::draw()
{   
     drawObject(projectedTriangles);
    if(wireframe){
     drawWireframeObject(projectedTriangles);
     }

    
}


void Object::loadObj(std::string filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
    {
        std::cout << "File cant be opened" << std::endl;
        exit(-1);
    }
    std::string line;
    std::vector<vec4f> verts;
    std::vector<vec4f> normals;
    std::vector<vec2f> textures;

    int count = 1;
    while (!in.eof())
    {
        //get one line at a time
        std::getline(in, line);
        //string object
        std::istringstream iss(line.c_str());

        char trash;
        if (!line.compare(0, 2, "v ")) //starts with v<space>
        {

            // std::cout << count;
            iss >> trash; // first character is v
            vec4f v;
            // followed by xyz co-ords
            iss >> v.x;
            iss >> v.y;
            iss >> v.z;
            // maths::printvec(v);
            verts.push_back(v);
            count++;
        }
        else if (!line.compare(0, 3, "vt ")) //starts with vt<space>
        {
            iss >> trash >> trash; //Ignore vt
            vec2f uv;
            iss >> uv.x;
            iss >> uv.y;
            textures.push_back(uv);
        }

        else if (!line.compare(0, 3, "vn ")) //starts with vn<space>
        {
            iss >> trash >> trash;
            vec4f n;
            iss >> n.x;
            iss >> n.y;
            iss >> n.z;
            normals.push_back(n);
        }
        else if (!line.compare(0, 2, "f ")) //starts with f<space>
        {
            std::vector<vec4f> f;
            vec4f temp;

            iss >> trash; //first charecter is f

            while (iss >> temp.x >> trash >> temp.y >> trash >> temp.z)
            // in the form vert/vertTex/norm (vert is read, the rest are treated as trash)
            {
                //in wavefront obj all indices start at 1, not zero
                temp.x--; //vert
                temp.y--; //texture
                temp.z--; // normal
                f.push_back(temp);
            }
            Triangle triangle;
            triangle.setVertex(verts[f[0].x], verts[f[1].x], verts[f[2].x]);
            // std::cout  << f[0][0] <<'\n';
            triangle.setTexCoords(textures[f[0].y], textures[f[1].y], textures[f[2].y]);
            triangle.setNormals(normals[f[0].z], normals[f[1].z], normals[f[2].z]);
            triangles.push_back(triangle);
        }
    }
    projectedTriangles = triangles;
}

void Object::originConversion()
{
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            triangles[i].vertices[j] = triangles[i].vertices[j].translatetoScreenOrigin();
        }
    }
}

void Object::translateObject(vec4f pt)
{
    for (int i = 0; i < triangles.size(); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                translate(triangles[i].vertices[j], pt); // translate the cube back to its original position
            }
        }
}

void Object::scaleObject(float pt)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            scale(triangles[i].vertices[j], pt);
        }
    }
}

void Object::rotateObject(float angle)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {   
            
            x_rotation(triangles[i].vertices[j], angle);
            y_rotation(triangles[i].vertices[j], angle);
            z_rotation(triangles[i].vertices[j], angle);
        }
    }
}

void Object::updateModel(mat4x4 &view, mat4x4 &projection)
{
    projectedTriangles.clear();
    for (auto &triangle : triangles)
    {
        Triangle temptri = triangle;
        temptri.vertices[0] = multiply(view, triangle.vertices[0]);
        temptri.vertices[1] = multiply(view, triangle.vertices[1]);
        temptri.vertices[2] = multiply(view, triangle.vertices[2]);
        bool backface = backfaceDetection(temptri);
        if(!backface)  
            continue;
    }



    sort(projectedTriangles.begin(), projectedTriangles.end(), [](Triangle &t1, Triangle &t2)
         {
             float z1 = (t1.vertices[0].z + t1.vertices[1].z + t1.vertices[2].z) / 3.0f;
             float z2 = (t2.vertices[0].z + t2.vertices[1].z + t2.vertices[2].z) / 3.0f;
             return z1>z2;
         });

    for (auto &triangle : projectedTriangles)
    {
       

        //Projection Transformation
        triangle.vertices[0] = multiply(projection, triangle.vertices[0]);
        triangle.vertices[1] = multiply(projection, triangle.vertices[1]);
        triangle.vertices[2] = multiply(projection, triangle.vertices[2]);
    }

    if(rotatex){
        rotateObject(60);
    }
}

bool Object::backfaceDetection(Triangle &triangle)
{
    vec4f v1 = triangle.vertices[0], v2 = triangle.vertices[1], v3 = triangle.vertices[2];
    vec4f centroid;
    centroid.x = (v1.x + v2.x + v3.x) / 3;
    centroid.y = (v1.y + v2.y + v3.y) / 3;
    centroid.z = (v1.z + v2.z + v3.z) / 3;

    vec4f V = (camera->Position - centroid).normalize();


    v2 = v2 - v1;
    v3 = v3 - v1;

    vec4f normal = v2.crossProduct(v3);
    normal = normal.normalize();

    float product = dotProduct(normal, V);
    if(product<0)
    {
        projectedTriangles.push_back(triangle);
        return false;    
    }
    projectedTriangles.push_back(triangle);
    return true;
}





    