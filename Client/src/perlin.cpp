#include <cmath>
#include <random>
#include <perlin.hpp>

float noise_vals_2d[512][512];

std::mt19937 rng;
std::uniform_real_distribution<float> u(0.0f, 1.0f);

inline float InterpolateCosine( float a, float b, float x ) {
	float f = ( 1 - cos( x * static_cast<float>( M_PI ) ) ) * 0.5f;
	return ( 1 - f ) * a + f * b;
}

inline float Noise(unsigned int x) {
	x -= ( x << 6  );
  x ^= ( x >> 17 );
  x -= ( x << 9  );
  x ^= ( x << 4  );
  x -= ( x << 3  );
  x ^= ( x << 10 );
  x ^= ( x >> 15 );
	return static_cast<float>( x );
}

inline float Noise(int x, int y) {
  return noise_vals_2d[y][x];

  //return u( rng );
}

inline float Noise(int x, int y, int z) {
  return Noise( x + ( y * 89213 ) + ( z * 8997587 ) ) / static_cast<float>( 0xffffffff );
}

void PreparePerlinNoise() {
  for( int y = 0; y < 512; y++ ) {
    for( int x = 0; x < 512; x++ ) {
      unsigned int a = x + y * 1024;

      float c = Noise( a ) / static_cast<float>( 0xffffffff );

      noise_vals_2d[y][x] = c;
    }
  }
}

float SmoothedNoise2D(int x, int y) {
  float corners = ( Noise( x - 1, y - 1 ) + Noise( x + 1, y - 1 ) + Noise( x - 1, y + 1 ) + Noise( x + 1, y + 1 ) ) / 16;
  float sides   = ( Noise( x - 1, y     ) + Noise( x + 1, y     ) + Noise( x,     y - 1 ) + Noise( x,     y + 1 ) ) /  8;
  float center  =   Noise( x, y ) / 4;
  return corners + sides + center;
}

float SmoothedNoise3D(int x, int y, int z) {
  float corners = ( Noise(x+1, y+1, z+1) + Noise(x+1, y+1, z-1) + Noise(x+1, y-1, z+1) + Noise(x+1, y-1, z-1) +
                    Noise(x-1, y+1, z+1) + Noise(x-1, y+1, z-1) + Noise(x-1, y-1, z+1) + Noise(x-1, y-1, z-1) ) / 16;
  float sides   = ( Noise(x-1, y, z) + Noise(x+1, y, z) +
                    Noise(x, y-1, z) + Noise(x, y+1, z) +
                    Noise(x, y, z-1) + Noise(x, y, z+1) ) /  8;
  float center  =   Noise(x, y, z) / 4;
  return corners + sides + center;
}

float InterpolatedNoise2D(float x, float y) {
  int int_x = (int)(x);
  int int_y = (int)(y);

  float frac_x = static_cast<float>( x - int_x );
  float frac_y = static_cast<float>( y - int_y );

  float v1 = SmoothedNoise2D( int_x,     int_y     );
  float v2 = SmoothedNoise2D( int_x + 1, int_y     );
  float v3 = SmoothedNoise2D( int_x,     int_y + 1 );
  float v4 = SmoothedNoise2D( int_x + 1, int_y + 1 );

  float i1 = InterpolateCosine(v1, v2, frac_x);
  float i2 = InterpolateCosine(v3, v4, frac_x);

  return InterpolateCosine(i1, i2, frac_y);
}

float InterpolatedNoise3D(float x, float y, float z) {
  int int_x = (int)(x);
  int int_y = (int)(y);
  int int_z = (int)(z);

  float frac_x = static_cast<float>( x - int_x );
  float frac_y = static_cast<float>( y - int_y );
  float frac_z = static_cast<float>( z - int_z );

  float v1 = SmoothedNoise3D(int_x,     int_y    , int_z );
  float v2 = SmoothedNoise3D(int_x + 1, int_y    , int_z );
  float v3 = SmoothedNoise3D(int_x,     int_y + 1, int_z );
  float v4 = SmoothedNoise3D(int_x + 1, int_y + 1, int_z );

  float v5 = SmoothedNoise3D(int_x,     int_y    , int_z + 1 );
  float v6 = SmoothedNoise3D(int_x + 1, int_y    , int_z + 1 );
  float v7 = SmoothedNoise3D(int_x,     int_y + 1, int_z + 1 );
  float v8 = SmoothedNoise3D(int_x + 1, int_y + 1, int_z + 1 );

  float i1 = InterpolateCosine(v1, v2, frac_x);
  float i2 = InterpolateCosine(v3, v4, frac_x);
  float i3 = InterpolateCosine(i1, i2, frac_y);

  float j1 = InterpolateCosine(v5, v6, frac_x);
  float j2 = InterpolateCosine(v7, v8, frac_x);
  float j3 = InterpolateCosine(j1, j2, frac_y);

  return InterpolateCosine(i3, j3, frac_z);
}

float PerlinNoise2D(float x, float y, int seed, float wavelength) {
  float freq = 1 / wavelength;

  return InterpolatedNoise2D( static_cast<float>( x * freq + seed ), static_cast<float>( y * freq + seed ) );
}

float PerlinNoise3D(float x, float y, float z, int seed, float wavelength) {
  float freq = 1 / wavelength;

  return InterpolatedNoise3D( static_cast<float>( x * freq + seed ), static_cast<float>( y * freq + seed ), static_cast<float>( z * freq + seed ) );
}
