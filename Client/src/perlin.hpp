#ifndef PERLIN_HPP_INCLUDED
#define PERLIN_HPP_INCLUDED

inline float InterpolateCosine( float a, float b, float x );

void PreparePerlinNoise();

float PerlinNoise2D( float x, float y, int seed, float wavelength );
float PerlinNoise3D( float x, float y, float z, int seed, float wavelength );

#endif // PERLIN_HPP_INCLUDED
