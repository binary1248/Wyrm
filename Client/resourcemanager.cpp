#include <cmath>
#include <iostream>
#include "resourcemanager.h"

float noise_vals[512][512];

struct thread_params {
  int x;
  int y;
  int width;
  int height;
  sf::Color color;
  int seed;
  size_t octaves;
  float* amplitudes;
  sf::Uint8* matrix;
};

inline float InterpolateCosine(float a, float b, float x) {
	float f = (1 - cos(x * M_PI)) * 0.5f;
	return (1 - f)* a + f * b;
}

inline float Noise(int x, int y) {
  return noise_vals[y][x];
}

void PrepareNoise() {
  for( int y = 0; y < 512; y++ ) {
    for( int x = 0; x < 512; x++ ) {
      unsigned int a = x + y * 1024;
      a -= (a<<6);
      a ^= (a>>17);
      a -= (a<<9);
      a ^= (a<<4);
      a -= (a<<3);
      a ^= (a<<10);
      a ^= (a>>15);

      float b = (float)a / (float)(0xffffffff);
      noise_vals[y][x] = b;
    }
  }
}

float SmoothedNoise(int x, int y) {
  float corners = ( Noise(x-1, y-1) + Noise(x+1, y-1) + Noise(x-1, y+1) + Noise(x+1, y+1) ) / 16;
  float sides   = ( Noise(x-1, y  ) + Noise(x+1, y  ) + Noise(x,   y-1) + Noise(x,   y+1) ) /  8;
  float center  =   Noise(x, y) / 4;
  return corners + sides + center;
}

float InterpolatedNoise(float x, float y) {
  int int_x = (int)(x);
  int int_y = (int)(y);

  float frac_x = x - int_x;
  float frac_y = y - int_y;

  float v1 = SmoothedNoise(int_x,     int_y    );
  float v2 = SmoothedNoise(int_x + 1, int_y    );
  float v3 = SmoothedNoise(int_x,     int_y + 1);
  float v4 = SmoothedNoise(int_x + 1, int_y + 1);

  float i1 = InterpolateCosine(v1, v2, frac_x);
  float i2 = InterpolateCosine(v3, v4, frac_x);

  return InterpolateCosine(i1, i2, frac_y);
}

float PerlinNoise(float x, float y, int seed, float wavelength){
  float freq = 1 / wavelength;

  return InterpolatedNoise( x * freq + seed, y * freq + seed );
}

ResourceManager::ResourceManager() {
  PrepareNoise();
}

ResourceManager::~ResourceManager() {
  for( std::map<std::string, sf::Image*>::iterator i = images.begin(); i != images.end(); i++ ) {
    if( i->second ) {
      delete i->second;
    }
  }

  images.clear();

  for( std::map<std::string, sf::SoundBuffer*>::iterator i = sounds.begin(); i != sounds.end(); i++ ) {
    if( i->second ) {
      delete i->second;
    }
  }

  sounds.clear();

  while( !particles.empty() ) {
    delete particles.back().second;
    particles.pop_back();
  }
}

sf::Image* ResourceManager::OpenImage(std::string file) {
  std::map<std::string, sf::Image*>::iterator i;
  i = images.find(file);

  if( i != images.end() ) {
    return i->second;
  }

  sf::Image* image = new sf::Image;

  image->LoadFromFile(file);

  images.insert( std::make_pair<std::string, sf::Image*>(file, image) );

  return image;
}

sf::SoundBuffer* ResourceManager::OpenSound(std::string file) {
  std::map<std::string, sf::SoundBuffer*>::iterator i;
  i = sounds.find(file);

  if( i != sounds.end() ) {
    return i->second;
  }

  sf::SoundBuffer* sound = new sf::SoundBuffer;

  sound->LoadFromFile(file);

  sounds.insert( std::make_pair<std::string, sf::SoundBuffer*>(file, sound) );

  return sound;
}

sf::Image* ResourceManager::GetParticle(int size, float sigma2, float exp) {

  for( std::vector< std::pair<particle_id, sf::Image*> >::iterator i = particles.begin(); i != particles.end(); i++ ) {
    if( i->first.size == size && i->first.sigma2 == sigma2 && i->first.exp == exp ) {
      return i->second;
    }
  }

  float pixels[size][size][4];
  sf::Uint8 pixels2[size][size][4];

  float max = 0;

  std::cout.precision(3);

  for( int i = 0; i < size; i++ ) {
    for( int j = 0; j < size; j++ ) {
      float val = 0;
      //float val2 = 0;

      float e = 2.718281828459045f;

      float di = i - (size/2-1);
      float dj = j - (size/2-1);

      val = 1 / (2*M_PI*sigma2) * std::pow(e, -( (di*di+dj*dj)/2*sigma2 ) );

      //val2 = ((size/2-1) - std::sqrt(di*di+dj*dj)) / (size/2-1);

      //val = std::pow(val * val2, exp);
      //val = std::pow(val, exp);

      if( val > max ) {
        max = val;
      }

      pixels[i][j][0] = 255;
      pixels[i][j][1] = 255;
      pixels[i][j][2] = 255;
      pixels[i][j][3] = val;
    }
  }

  for( int i = 0; i < size; i++ ) {
    for( int j = 0; j < size; j++ ) {
      pixels2[i][j][0] = static_cast<sf::Uint8>( pixels[i][j][0] /* * (1/max) * 255.0f */ );
      pixels2[i][j][1] = static_cast<sf::Uint8>( pixels[i][j][1] /* * (1/max) * 255.0f */ );
      pixels2[i][j][2] = static_cast<sf::Uint8>( pixels[i][j][2] /* * (1/max) * 255.0f */ );
      pixels2[i][j][3] = static_cast<sf::Uint8>( pixels[i][j][3] * (1/max) * 255.0f );
    }
  }

  sf::Image* image = new sf::Image();

  image->LoadFromPixels(size,size, (sf::Uint8*)pixels2);

  struct particle_id id;
  id.size = size;
  id.sigma2 = sigma2;
  id.exp = exp;

  particles.push_back( std::make_pair<struct particle_id, sf::Image*>(id, image) );

  return image;
}

void BackgroundThreadFunction(void* UserData) {
  struct thread_params* tp = (struct thread_params*)UserData;

  for( int y = tp->y; y < tp->height; y+=8 ) {
    for( int x = tp->x; x < tp->width; x++ ) {
      float val = 0;

      for( size_t i = 0; i < tp->octaves; i++ ) {
        if( tp->amplitudes[i] < 0.1f ) {
          continue;
        }

        val += PerlinNoise(x, y, tp->seed, pow(2,tp->octaves-i) ) * tp->amplitudes[i];
      }

      int r = val*((float)(tp->color.r)/255.0f);
      int g = val*((float)(tp->color.g)/255.0f);
      int b = val*((float)(tp->color.b)/255.0f);

      tp->matrix[(y*tp->width+x)*4+0] = r;
      tp->matrix[(y*tp->width+x)*4+1] = g;
      tp->matrix[(y*tp->width+x)*4+2] = b;
      tp->matrix[(y*tp->width+x)*4+3] = 255;
    }
  }
}

sf::Image* ResourceManager::GetBackground(int width, int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes ) {
  sf::Uint8* MatrixImage = new (std::nothrow) sf::Uint8[height*width*4];

  struct thread_params* tp0 = new struct thread_params;

  tp0->width = width;
  tp0->height = height;
  tp0->color = base_color;
  tp0->seed = seed;
  tp0->octaves = num_octaves;
  tp0->amplitudes = amplitudes;
  tp0->matrix = MatrixImage;
  tp0->x = 0;
  tp0->y = 0;

  sf::Thread Thread0(&BackgroundThreadFunction,tp0);
  Thread0.Launch();

  struct thread_params* tp1 = new struct thread_params;

  tp1->width = width;
  tp1->height = height;
  tp1->color = base_color;
  tp1->seed = seed;
  tp1->octaves = num_octaves;
  tp1->amplitudes = amplitudes;
  tp1->matrix = MatrixImage;
  tp1->x = 1;
  tp1->y = 1;

  sf::Thread Thread1(&BackgroundThreadFunction,tp1);
  Thread1.Launch();

  struct thread_params* tp2 = new struct thread_params;

  tp2->width = width;
  tp2->height = height;
  tp2->color = base_color;
  tp2->seed = seed;
  tp2->octaves = num_octaves;
  tp2->amplitudes = amplitudes;
  tp2->matrix = MatrixImage;
  tp2->x = 2;
  tp2->y = 2;

  sf::Thread Thread2(&BackgroundThreadFunction,tp2);
  Thread2.Launch();

  struct thread_params* tp3 = new struct thread_params;

  tp3->width = width;
  tp3->height = height;
  tp3->color = base_color;
  tp3->seed = seed;
  tp3->octaves = num_octaves;
  tp3->amplitudes = amplitudes;
  tp3->matrix = MatrixImage;
  tp3->x = 3;
  tp3->y = 3;

  sf::Thread Thread3(&BackgroundThreadFunction,tp3);
  Thread3.Launch();

  struct thread_params* tp4 = new struct thread_params;

  tp4->width = width;
  tp4->height = height;
  tp4->color = base_color;
  tp4->seed = seed;
  tp4->octaves = num_octaves;
  tp4->amplitudes = amplitudes;
  tp4->matrix = MatrixImage;
  tp4->x = 4;
  tp4->y = 4;

  sf::Thread Thread4(&BackgroundThreadFunction,tp4);
  Thread4.Launch();

  struct thread_params* tp5 = new struct thread_params;

  tp5->width = width;
  tp5->height = height;
  tp5->color = base_color;
  tp5->seed = seed;
  tp5->octaves = num_octaves;
  tp5->amplitudes = amplitudes;
  tp5->matrix = MatrixImage;
  tp5->x = 5;
  tp5->y = 5;

  sf::Thread Thread5(&BackgroundThreadFunction,tp5);
  Thread5.Launch();

  struct thread_params* tp6 = new struct thread_params;

  tp6->width = width;
  tp6->height = height;
  tp6->color = base_color;
  tp6->seed = seed;
  tp6->octaves = num_octaves;
  tp6->amplitudes = amplitudes;
  tp6->matrix = MatrixImage;
  tp6->x = 6;
  tp6->y = 6;

  sf::Thread Thread6(&BackgroundThreadFunction,tp6);
  Thread6.Launch();

  struct thread_params* tp7 = new struct thread_params;

  tp7->width = width;
  tp7->height = height;
  tp7->color = base_color;
  tp7->seed = seed;
  tp7->octaves = num_octaves;
  tp7->amplitudes = amplitudes;
  tp7->matrix = MatrixImage;
  tp7->x = 7;
  tp7->y = 7;

  sf::Thread Thread7(&BackgroundThreadFunction,tp7);
  Thread7.Launch();

  Thread0.Wait();
  Thread1.Wait();
  Thread2.Wait();
  Thread3.Wait();
  Thread4.Wait();
  Thread5.Wait();
  Thread6.Wait();
  Thread7.Wait();

  delete tp0;
  delete tp1;
  delete tp2;
  delete tp3;
  delete tp4;
  delete tp5;
  delete tp6;
  delete tp7;

  sf::Image* image = new sf::Image();
  image->LoadFromPixels(width, height, MatrixImage);

  delete[] MatrixImage;

  return image;
}












