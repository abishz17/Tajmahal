#pragma once

#include<fstream>
#include<sstream>
#include<algorithm>
#include "camera.h"
#include "3dtransformations.h"
#include "modeltransformation.h"
#include "triangleRasterize.h"


//sorting function to check depth
void painterSort(std::vector<Triangle> &tri, float low, float high); 

class Model
{
private:
    std::vector<Triangle> triangles;
    std::vector<Triangle> final_triangles;

public:
    void loadObj(std::string);
    void originConversion();
    void rotate(float);
    void translate(vec4f);
    void scale(float);
    void updateModel(mat4f &, mat4f &);
    bool backfaceDetection(Triangle &tri);
    void draw();
    float calculateIntensity(vec4f, vec4f, vec4f);

    Camera *camera;
};

void Model::draw()
{   
     drawObject(final_triangles);
    if(wireframe){
     drawWireframeObject(final_triangles);
     }

    
}


void Model::loadObj(std::string filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
    {
        std::cout << "Cannot Read" << std::endl;
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
            Triangle tri;
            tri.setVertex(verts[f[0].x], verts[f[1].x], verts[f[2].x]);
            // std::cout  << f[0][0] <<'\n';
            tri.setTexCoords(textures[f[0].y], textures[f[1].y], textures[f[2].y]);
            tri.setNormals(normals[f[0].z], normals[f[1].z], normals[f[2].z]);
            triangles.push_back(tri);
        }
    }
    for (size_t i = 0; i < triangles.size() - 1; i++)
    {
        triangles[i].zbuffer = (triangles[i].vertices[0].z + triangles[i].vertices[1].z + triangles[i].vertices[2].z);
    }
    final_triangles = triangles;
}

void Model::originConversion()
{
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            triangles[i].vertices[j] = triangles[i].vertices[j].translatetoScreenOrigin();
        }
    }
}

void Model::translate(vec4f pt)
{
    for (int i = 0; i < triangles.size(); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                transate_polygon(triangles[i].vertices[j], pt); // translate the cube back to its original position
            }
        }
}

void Model::scale(float pt)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            scale_polygon(triangles[i].vertices[j], pt);
        }
    }
}

void Model::rotate(float angle)
{
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            rotateX(triangles[i].vertices[j], angle);
            rotateY(triangles[i].vertices[j], angle);
            rotateZ(triangles[i].vertices[j], angle);
        }
    }
}

void Model::updateModel(mat4f &view, mat4f &projection)
{
    final_triangles.clear();

    for (auto &tri : triangles)
    {
        Triangle temptri = tri;
        temptri.vertices[0] = mul(view, tri.vertices[0]);
        temptri.vertices[1] = mul(view, tri.vertices[1]);
        temptri.vertices[2] = mul(view, tri.vertices[2]);
        bool backface = backfaceDetection(temptri);
        if(!backface)  
            continue;
    }



    sort(final_triangles.begin(), final_triangles.end(), [](Triangle &t1, Triangle &t2)
         {
             float z1 = (t1.vertices[0].z + t1.vertices[1].z + t1.vertices[2].z) / 3.0f;
             float z2 = (t2.vertices[0].z + t2.vertices[1].z + t2.vertices[2].z) / 3.0f;
             return z1>z2;
         });

    for (auto &tri : final_triangles)
    {
       

        //Projection Transformation
        tri.vertices[0] = mul(projection, tri.vertices[0]);
        tri.vertices[1] = mul(projection, tri.vertices[1]);
        tri.vertices[2] = mul(projection, tri.vertices[2]);
    }
}

bool Model::backfaceDetection(Triangle &tri)
{
    vec4f v1 = tri.vertices[0], v2 = tri.vertices[1], v3 = tri.vertices[2];
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
        final_triangles.push_back(tri);
        return false;    
    }
    final_triangles.push_back(tri);
    return true;
}

void painterSort(std::vector<Triangle>&tri, float low, float high)  // Quick sort algo
{
    float i = low;
    float j = high;
    float pivot = tri[(i+j)/2].zbuffer;
    Triangle temp;

    while (i <= j)
    {
        while (tri[i].zbuffer < pivot)
            i++;
        while (tri[j].zbuffer > pivot)
            j--;
        if(i <= j)
        {
            temp = tri[i];
            tri[i] = tri[j];
            tri[j] = temp;
            i++;
            j--;
        }
        
    }
    if (j > low)
        painterSort(tri,low,j);
    if (i < high)
        painterSort(tri, i, high);

}




    