#pragma once

#define WHITE {1.0f, 1.0f, 1.0f}
#define RED {1.0f, 0.0f, 0.0f}
#define GREEN {0.0f, 1.0f, 0.0f}
#define BLUE {0.0f, 0.0f, 1.0f}
#define YELLOW {1.0f, 1.0f, 0.0f}
#define BLACK {0.0f, 0.0f, 0.0f}
#define ORANGE {1.0f, 0.5f, 0.0f}
#define PURPLE {1.0f, 0.0f, 1.0f}

#include<iostream>

struct Color
{
    float r, g, b;

    Color operator*(float mul)
    {
        return {float(mul * r), float(mul * g), float(mul * b)};
    }

};
