#ifndef SIMPLEX_H_INCLUDED
#define SIMPLEX_H_INCLUDED

namespace Engine
{
namespace Math
{
    float noise(float xin, float yin, float zin);
    float simplex_noise(int octaves, float x, float y, float z);
};
};

#endif // SIMPLEX_H_INCLUDED
