#include <cmath>
#include <iostream>
#include "perlin.h"
#include "resourcemanager.h"

struct thread_params {
  int x;
  int y;
  size_t step_size;
  int width;
  int height;
  sf::Color color;
  int seed;
  size_t octaves;
  float* amplitudes;
  sf::Uint8* matrix;
  bool running;
};

ResourceManager::ResourceManager() {
  PreparePerlinNoise();
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
      pixels2[i][j][0] = 255; // static_cast<sf::Uint8>( pixels[i][j][0] /* * (1/max) * 255.0f */ );
      pixels2[i][j][1] = 255; // static_cast<sf::Uint8>( pixels[i][j][1] /* * (1/max) * 255.0f */ );
      pixels2[i][j][2] = 255; // static_cast<sf::Uint8>( pixels[i][j][2] /* * (1/max) * 255.0f */ );
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

void PerlinThreadFunction2D(void* UserData) {
  struct thread_params* tp = (struct thread_params*)UserData;

  int start_y = tp->y;

  tp->running = true;

  for( int y = start_y; y < tp->height; y += tp->step_size ) {
    for( int x = 0; x < tp->width; x++ ) {
      float val = 0;

      for( size_t i = 0; i < tp->octaves; i++ ) {
        if( tp->amplitudes[i] < 0.1f ) {
          continue;
        }

        val += PerlinNoise2D(x, y, tp->seed, pow(2,tp->octaves-i) ) * tp->amplitudes[i];
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

void PerlinThreadFunction3D(void* UserData) {
  struct thread_params* tp = (struct thread_params*)UserData;

  int start_y = tp->y;

  tp->running = true;

  int radius = tp->width/2;

  for( int y = start_y; y < tp->height; y += tp->step_size ) {
    for( int x = 0; x < tp->width; x++ ) {
      float u = (float)x/(float)(tp->width);
			float v = (float)y/(float)(tp->height);

			float r = (u * 2.0f) * M_PI;
			float s = (v - 0.5f) * M_PI;

			float a = cos(r )* cos(s);
			float b = sin(r) * cos(s);
			float c = sin(s);

			a+=1.0f;
			b+=1.0f;
			c+=1.0f;

			a*=(tp->width);
			b*=(tp->width);
			c*=(tp->width);

      float val = 0;

      for( size_t i = 0; i < tp->octaves; i++ ) {
        if( tp->amplitudes[i] < 0.1f ) {
          continue;
        }

        val += PerlinNoise3D(a, b, c, tp->seed, pow(2,tp->octaves-i) ) * tp->amplitudes[i];
      }

      int cr = val*((float)(tp->color.r)/255.0f);
      int cg = val*((float)(tp->color.g)/255.0f);
      int cb = val*((float)(tp->color.b)/255.0f);

      tp->matrix[(y*tp->width+x)*4+0] = cr;
      tp->matrix[(y*tp->width+x)*4+1] = cg;
      tp->matrix[(y*tp->width+x)*4+2] = cb;

      float dist = sqrt( (x-tp->width/2)*(x-tp->width/2) + (y-tp->height/2)*(y-tp->height/2) );



      if( dist <= (float)(radius-8) ) {
        tp->matrix[(y*tp->width+x)*4+3] = 255;
      } else if( dist <= (float)(radius-4) ) {
        float border = radius - dist;
        tp->matrix[(y*tp->width+x)*4+0] = (int)InterpolateCosine( tp->matrix[(y*tp->width+x)*4+0], 255, (10-border)/15);
        tp->matrix[(y*tp->width+x)*4+1] = (int)InterpolateCosine( tp->matrix[(y*tp->width+x)*4+1], 255, (10-border)/15);
        tp->matrix[(y*tp->width+x)*4+2] = (int)InterpolateCosine( tp->matrix[(y*tp->width+x)*4+2], 255, (10-border)/15);
        tp->matrix[(y*tp->width+x)*4+3] = (int)(border * 64);
      } else {
        tp->matrix[(y*tp->width+x)*4+3] = 0;
      }
    }
  }
}

sf::Image* ResourceManager::GetBackground(int width, int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes ) {
  sf::Uint8* MatrixImage = new (std::nothrow) sf::Uint8[height*width*4];

  size_t num_threads = 8;
  std::vector<sf::Thread*> threads;

  struct thread_params* tp = new struct thread_params;

  tp->width = width;
  tp->height = height;
  tp->step_size = num_threads;
  tp->color = base_color;
  tp->seed = seed;
  tp->octaves = num_octaves;
  tp->amplitudes = amplitudes;
  tp->matrix = MatrixImage;

  for( size_t i = 0; i < num_threads; i++ ) {
    tp->x = i;
    tp->y = i;
    tp->running = false;
    sf::Thread* thread = new sf::Thread(&PerlinThreadFunction2D,tp);
    thread->Launch();

    while( !tp->running ) {}
    threads.push_back(thread);
  }

  while( !threads.empty() ) {
    threads.back()->Wait();
    delete threads.back();
    threads.pop_back();
  }

  delete tp;

  sf::Image* image = new sf::Image();
  image->LoadFromPixels(width, height, MatrixImage);

  delete[] MatrixImage;

  return image;
}

sf::Image* ResourceManager::GetPlanet(int width, int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes ) {
  sf::Uint8* MatrixImage = new (std::nothrow) sf::Uint8[height*width*4];

  size_t num_threads = 8;
  std::vector<sf::Thread*> threads;

  struct thread_params* tp = new struct thread_params;

  tp->width = width;
  tp->height = height;
  tp->step_size = num_threads;
  tp->color = base_color;
  tp->seed = seed;
  tp->octaves = num_octaves;
  tp->amplitudes = amplitudes;
  tp->matrix = MatrixImage;

  for( size_t i = 0; i < num_threads; i++ ) {
    tp->x = i;
    tp->y = i;
    tp->running = false;
    sf::Thread* thread = new sf::Thread(&PerlinThreadFunction3D,tp);
    thread->Launch();

    while( !tp->running ) {}
    threads.push_back(thread);
  }

  while( !threads.empty() ) {
    threads.back()->Wait();
    delete threads.back();
    threads.pop_back();
  }

  delete tp;

  sf::Image* image = new sf::Image();
  image->LoadFromPixels(width, height, MatrixImage);

  delete[] MatrixImage;

  return image;
}










