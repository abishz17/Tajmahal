#pragma once

#include "line.h"
#include "modeltransformation.h"
#include "vertex.h"
#include "interpolate.h"




//Illumination model constants

float lightX = 600.0f;
float lightY = 500.0f;
float lightZ = 0.0f;

vec4f view = vec4f{0.0f, 0.0f, 1.0f};
vec4f Ka = vec4f{1.0f, 1.0f, 1.0f};
vec4f Kd = vec4f{0.8f, 0.8f, 0.8f};
vec4f Ks = vec4f{0.5f, 0.5f, 0.5f};
vec4f light = vec4f{lightX, lightY, lightZ};
float ns = 200.0f;
vec4f Ia = vec4f{0.3f, 0.1f, 0.1f};
vec4f Il = vec4f{0.9f, 0.9f, 0.9f};



void DrawTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2);
void DrawFlatTopTriangle(const Vertex &it0,
                         const Vertex &it1,
                         const Vertex &it2);
void DrawFlatBottomTriangle(const Vertex &it0,
                            const Vertex &it1,
                            const Vertex &it2);
void DrawFlatTriangle(const Vertex &it0,
                      const Vertex &it1,
                      const Vertex &it2,
                      const Vertex &dv0,
                      const Vertex &dv1,
                      Vertex itEdge1);

void fillTriangle(vec4f v1, vec4f v2, vec4f v3, Color c);
void fillBottomFlatTriangle(vec4f v1, vec4f v2, vec4f v3, Color c);
void fillTopFlatTriangle(vec4f v1, vec4f v2, vec4f v3, Color c);

void wireframe_draw(vec4f v1, vec4f v2, vec4f v3, Color c);
void drawWireframe_model(std::vector<Triangle> &model);
void draw_model(std::vector<Triangle> &model);

vec4f intensity(vec4f &Ka, vec4f &Kd, vec4f &Ks, float ns, vec4f &point, vec4f &light, vec4f &view, vec4f &normal, vec4f &Ia, vec4f &Il);




void DrawTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2)
{
    // using pointers so we can swap (for sorting purposes)
    const Vertex *pv0 = &v0;
    const Vertex *pv1 = &v1;
    const Vertex *pv2 = &v2;
    // sorting vertices by y
    if (pv1->pos.y < pv0->pos.y)
        std::swap(pv0, pv1);
    if (pv2->pos.y < pv1->pos.y)
        std::swap(pv1, pv2);
    if (pv1->pos.y < pv0->pos.y)
        std::swap(pv0, pv1);
    if (pv0->pos.y == pv1->pos.y) // natural flat top
    {
        // sorting top vertices by x
        if (pv1->pos.x < pv0->pos.x)
            std::swap(pv0, pv1);
        DrawFlatTopTriangle(*pv0, *pv1, *pv2);
    }
    else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
    {
        // sorting bottom vertices by x
        if (pv2->pos.x < pv1->pos.x)
            std::swap(pv1, pv2);
        DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
    }
    else // general triangle
    {
        // find splitting vertex interpolant
        const float alphaSplit =
            (pv1->pos.y - pv0->pos.y) /
            (pv2->pos.y - pv0->pos.y);
        const auto vi = interpolate(*pv0, *pv2, alphaSplit);
        if (pv1->pos.x < vi.pos.x) // major right
        {
            DrawFlatBottomTriangle(*pv0, *pv1, vi);
            DrawFlatTopTriangle(*pv1, vi, *pv2);
        }
        else // major left
        {
            DrawFlatBottomTriangle(*pv0, vi, *pv1);
            DrawFlatTopTriangle(vi, *pv1, *pv2);
        }
    }
}
// does flat *TOP* tri-specific calculations and calls DrawFlatTriangle
void DrawFlatTopTriangle(const Vertex &it0,
                         const Vertex &it1,
                         const Vertex &it2)
{
    // calulcate dVertex / dy
    // change in interpolant for every 1 change in y
    const float delta_y = it2.pos.y - it0.pos.y;
    const auto dit0 = (it2 - it0) / delta_y;
    const auto dit1 = (it2 - it1) / delta_y;
    // create right edge interpolant
    auto itEdge1 = it1;
    // call the flat triangle render routine
    DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
}
// does flat *BOTTOM* tri-specific calculations and calls DrawFlatTriangle
void DrawFlatBottomTriangle(const Vertex &it0,
                            const Vertex &it1,
                            const Vertex &it2)
{
    // calulcate dVertex / dy
    // change in interpolant for every 1 change in y
    const float delta_y = it2.pos.y - it0.pos.y;
    const auto dit0 = (it1 - it0) / delta_y;
    const auto dit1 = (it2 - it0) / delta_y;
    // create right edge interpolant
    auto itEdge1 = it0;
    // call the flat triangle render routine
    DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
}
// does processing common to both flat top and flat bottom tris
// scan over triangle in screen space, interpolate attributes,
// invoke ps and write pixel to screen


////////FOR GOURAUD SHADING , RASTERIZE IS DONE THIS WAY///////////

void DrawFlatTriangle(const Vertex &it0,
                      const Vertex &it1,
                      const Vertex &it2,
                      const Vertex &dv0,
                      const Vertex &dv1,
                      Vertex itEdge1)
{
    // create edge interpolant for left edge (always v0)
    auto itEdge0 = it0;
    // calculate start and end scanlines
    const int yStart = (int)ceil(it0.pos.y - 0.5f);
    const int yEnd = (int)ceil(it2.pos.y - 0.5f); // the scanline AFTER the last line drawn
    // do interpolant prestep
    itEdge0 += dv0 * (float(yStart) + 0.5f - it0.pos.y);
    itEdge1 += dv1 * (float(yStart) + 0.5f - it0.pos.y);
    for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
    {
        // calculate start and end pixels
        const int xStart = (int)ceil(itEdge0.pos.x - 0.5f);
        const int xEnd = (int)ceil(itEdge1.pos.x - 0.5f); // the pixel AFTER the last pixel drawn
        // create scanline interpolant startpoint
        // (some waste for interpolating x,y,z, but makes life easier not having
        //  to split them off, and z will be needed in the future anyways...)
        auto iLine = itEdge0;
        // calculate delta scanline interpolant / dx
        const float dx = itEdge1.pos.x - itEdge0.pos.x;
        const auto diLine = (itEdge1 - iLine) / dx;
        // prestep scanline interpolant
        iLine += diLine * (float(xStart) + 0.5f - itEdge0.pos.x);
        for (int x = xStart; x < xEnd; x++, iLine += diLine)
        {
            // invoke pixel shader and write resulting color value
            Color color = {iLine.color.x, iLine.color.y, iLine.color.z};
            putpixel(x, y, color);

    }
}
}


////FOR FLAT SHADING , RASTERIZE IS DONE THIS WAY//////////////
void fillTriangle(vec4f v1, vec4f v2, vec4f v3, Color c)
{
    if(v1.y > v2.y) {std::swap(v2, v1);}
    if(v1.y > v3.y) {std::swap(v3, v1);}
    if(v2.y > v3.y) {std::swap(v3, v2);}

    if(int(v2.y) == int(v3.y)){fillBottomFlatTriangle(v1, v2, v3, c);}
    else if(int(v1.y) == int(v2.y)){fillTopFlatTriangle(v1, v2, v3, c);}
    else
    {
        // general case - split the triangle in a topflat and bottom-flat one
        vec4f *v4 = new vec4f({
            (v1.x + ((float)(v2.y-v1.y)/(float)(v3.y-v1.y))*(v3.x-v1.x)),v2.y, 0
        });
        fillBottomFlatTriangle(v1, v2, *v4, c);
        fillTopFlatTriangle(v2, *v4, v3, c);
    }
}

void fillBottomFlatTriangle(vec4f v1, vec4f v2, vec4f v3, Color c)
{
    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    // cout <<"a"<< (v2.x - v1.x) / (v2.y - v1.y)<<endl;
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float curx1 = v1.x;
    float curx2 = v1.x;

    BressenhamAlgo(v1.x, v1.y, v2.x, v2.y, c);
    BressenhamAlgo(v2.x, v2.y, v3.x, v3.y, c);
    BressenhamAlgo(v3.x, v3.y, v1.x, v1.y, c);

    for (int scanlineY = v1.y; scanlineY < v2.y - 0.5f; scanlineY++)
    {
        BressenhamAlgo(curx1, scanlineY, curx2, scanlineY, c);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void fillTopFlatTriangle(vec4f v1, vec4f v2, vec4f v3, Color c)
{
    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    float curx1 = v3.x;
    float curx2 = v3.x;
    BressenhamAlgo(v1.x, v1.y, v2.x, v2.y, c);
    BressenhamAlgo(v2.x, v2.y, v3.x, v3.y, c);
    BressenhamAlgo(v3.x, v3.y, v1.x, v1.y, c);

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
    {
        BressenhamAlgo(curx1, scanlineY, curx2, scanlineY, c);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}


vec4f intensity(vec4f &Ka, vec4f &Kd, vec4f &Ks, float ns, vec4f &point, vec4f &light, vec4f &view, vec4f &normal, vec4f &Ia, vec4f &Il)
{
    vec4f ambientIntensity = Ka * Ia;
    vec4f lightVec = light - point;
    vec4f unitLight = lightVec.normalize();
    vec4f diffuseIntensity = Kd * Il * dotProduct(normal, unitLight);
    vec4f reflection = (normal * 2.0 * dotProduct(normal, unitLight)) - unitLight;
    vec4f specularIntensity = Ks * Il * pow(dotProduct(view, reflection), ns);
    vec4f intensity = ambientIntensity + diffuseIntensity + specularIntensity;
    return intensity;
}

void wireframe_draw(vec4f v1, vec4f v2, vec4f v3, Color c)
{
    BressenhamAlgo(v1.x, v1.y, v2.x, v2.y, c);
    BressenhamAlgo(v2.x, v2.y, v3.x, v3.y, c);
    BressenhamAlgo(v3.x, v3.y, v1.x, v1.y, c);
}

void drawWireframeObject(std::vector<Triangle> &model)
{
    for (int i = 0; i < model.size(); i++)
    {
       wireframe_draw(model[i].vertices[0], model[i].vertices[1], model[i].vertices[2], BLACK);
    }
}


void drawObject(std::vector<Triangle> &model)
{   

        if(plainshade){

        for (int i = 0; i < model.size(); i++)
        {
            fillTriangle(model[i].vertices[0], model[i].vertices[1], model[i].vertices[2], model[i].color);

        }

        }

        else if (gouraudshade)
        {
    
        for (int i = 0; i < model.size(); i++)
        {
            vec4f vi0 = intensity(Ka, Kd, Ks, ns, model[i].vertices[0], light, view, model[i].normals[0], Ia, Il);
            vec4f vi1 = intensity(Ka, Kd, Ks, ns, model[i].vertices[1], light, view, model[i].normals[1], Ia, Il);
            vec4f vi2 = intensity(Ka, Kd, Ks, ns, model[i].vertices[2], light, view, model[i].normals[2], Ia, Il);
            Vertex v1(model[i].vertices[0], model[i].normals[0], vi0);
            Vertex v2(model[i].vertices[1], model[i].normals[1], vi1);
            Vertex v3(model[i].vertices[2], model[i].normals[2], vi2);
            DrawTriangle(v1, v2, v3);
        }

        }

    

}