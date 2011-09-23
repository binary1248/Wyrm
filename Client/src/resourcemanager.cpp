#include <cmath>
#include <queue>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_array.hpp>

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
  boost::shared_array<sf::Uint8> pixels;
};

typedef boost::shared_ptr<struct thread_params> ThreadParamsPtr;
typedef boost::shared_ptr<sf::Thread> ThreadPtr;

inline float InterpolateCosine(float a, float b, float x) {
	float f = (1 - cos(x * M_PI)) * 0.5f;
	return (1 - f)* a + f * b;
}

ResourceManager::ResourceManager() {
  PreparePerlinNoise();
}

ResourceManager::~ResourceManager() {
}

TexturePtr ResourceManager::OpenImage(std::string file) {
  std::map<std::string, TexturePtr>::iterator i;
  i = textures.find(file);

  if( i != textures.end() ) {
    return i->second;
  }

  sf::Image image;
  image.LoadFromFile(file);

  TexturePtr texture = boost::make_shared<sf::Texture>();
  texture->LoadFromImage( image );

  textures.insert( std::make_pair<std::string, TexturePtr>(file, texture) );

  return texture;
}

SoundBufferPtr ResourceManager::OpenSound(std::string file) {
  std::map<std::string, SoundBufferPtr>::iterator i;
  i = sounds.find(file);

  if( i != sounds.end() ) {
    return i->second;
  }

  SoundBufferPtr sound = boost::make_shared<sf::SoundBuffer>();

  sound->LoadFromFile(file);

  sounds.insert( std::make_pair<std::string, SoundBufferPtr>(file, sound) );

  return sound;
}

TexturePtr ResourceManager::GetParticle(int size, float max, float exp) {
  BOOST_FOREACH( ParticleTexturePair particle_texture_pair, particles ) {
		if( particle_texture_pair.first.size == size &&
				particle_texture_pair.first.max == max &&
				particle_texture_pair.first.exp == exp ) {
      return particle_texture_pair.second;
    }
  }

  boost::scoped_array<sf::Uint8> pixels( new sf::Uint8[size*size*4] );

	for( int i = 0; i < size; i++ ) {
    for( int j = 0; j < size; j++ ) {
      float val = size/2 - std::sqrt( std::pow( (size/2 - i), 2 ) + std::pow( (size/2 - j), 2 ) );

			// Clamp value
			val = std::max( 0.f, val - 1 );
			val = std::pow( val, exp );
			val = std::min( max, val );

      pixels[ (i*size+j)*4+0 ] = 255;
      pixels[ (i*size+j)*4+1 ] = 255;
      pixels[ (i*size+j)*4+2 ] = 255;
      pixels[ (i*size+j)*4+3 ] = static_cast<sf::Uint8>( val );
    }
  }

  TexturePtr texture = boost::make_shared<sf::Texture>();
  texture->Create( size, size );
  texture->Update( pixels.get() );

  struct particle_id id;
  id.size = size;
  id.max = max;
  id.exp = exp;

  particles.push_back( std::make_pair<struct particle_id, TexturePtr>(id, texture) );

  return texture;
}

void PerlinThreadFunction2D( void* thread_param ) {
  struct thread_params* params = static_cast<ThreadParamsPtr*>( thread_param )->get();

  int start_y = params->y;

  for( int y = start_y; y < params->height; y += params->step_size ) {
    for( int x = 0; x < params->width; x++ ) {
      float val = 0;

      for( size_t i = 0; i < params->octaves; i++ ) {
        if( params->amplitudes[i] < 0.1f ) {
          continue;
        }

        val += PerlinNoise2D(x, y, params->seed, pow(2,params->octaves-i) ) * params->amplitudes[i];
      }

      int r = val*((float)(params->color.r)/255.0f);
      int g = val*((float)(params->color.g)/255.0f);
      int b = val*((float)(params->color.b)/255.0f);

      params->pixels[(y*params->width+x)*4+0] = r;
      params->pixels[(y*params->width+x)*4+1] = g;
      params->pixels[(y*params->width+x)*4+2] = b;
      params->pixels[(y*params->width+x)*4+3] = 255;
    }
  }
}

void PerlinThreadFunction3D( void* thread_param ) {
  struct thread_params* params = static_cast<ThreadParamsPtr*>( thread_param )->get();

  int start_y = params->y;

  int radius = params->width/2;

  for( int y = start_y; y < params->height; y += params->step_size ) {
    for( int x = 0; x < params->width; x++ ) {
      float u = (float)x/(float)(params->width);
			float v = (float)y/(float)(params->height);

			float r = (u * 2.0f) * M_PI;
			float s = (v - 0.5f) * M_PI;

			float a = cos(r )* cos(s);
			float b = sin(r) * cos(s);
			float c = sin(s);

			a+=1.0f;
			b+=1.0f;
			c+=1.0f;

			a*=(params->width);
			b*=(params->width);
			c*=(params->width);

      float val = 0;

      for( size_t i = 0; i < params->octaves; i++ ) {
        if( params->amplitudes[i] < 0.1f ) {
          continue;
        }

        val += PerlinNoise3D(a, b, c, params->seed, pow(2,params->octaves-i) ) * params->amplitudes[i];
      }

      int cr = val*((float)(params->color.r)/255.0f);
      int cg = val*((float)(params->color.g)/255.0f);
      int cb = val*((float)(params->color.b)/255.0f);

      params->pixels[(y*params->width+x)*4+0] = cr;
      params->pixels[(y*params->width+x)*4+1] = cg;
      params->pixels[(y*params->width+x)*4+2] = cb;

      float dist = sqrt( (x-params->width/2)*(x-params->width/2) + (y-params->height/2)*(y-params->height/2) );



      if( dist <= (float)(radius-8) ) {
        params->pixels[(y*params->width+x)*4+3] = 255;
      } else if( dist <= (float)(radius-4) ) {
        float border = radius - dist;
        params->pixels[(y*params->width+x)*4+0] = (int)InterpolateCosine( params->pixels[(y*params->width+x)*4+0], 255, (10-border)/15);
        params->pixels[(y*params->width+x)*4+1] = (int)InterpolateCosine( params->pixels[(y*params->width+x)*4+1], 255, (10-border)/15);
        params->pixels[(y*params->width+x)*4+2] = (int)InterpolateCosine( params->pixels[(y*params->width+x)*4+2], 255, (10-border)/15);
        params->pixels[(y*params->width+x)*4+3] = (int)(border * 64);
      } else {
        params->pixels[(y*params->width+x)*4+3] = 0;
      }
    }
  }
}

TexturePtr ResourceManager::GetBackground(int width, int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes ) {
  boost::shared_array<sf::Uint8> pixels( new sf::Uint8[height*width*4] );

  size_t num_threads = 4;
  std::queue<ThreadPtr> threads;

  boost::shared_ptr<struct thread_params> params[ num_threads ];

  for( size_t i = 0; i < num_threads; i++ ) {
    params[i] = boost::make_shared<struct thread_params>();
    params[i]->width = width;
		params[i]->height = height;
		params[i]->step_size = num_threads;
		params[i]->color = base_color;
		params[i]->seed = seed;
		params[i]->octaves = num_octaves;
		params[i]->amplitudes = amplitudes;
		params[i]->pixels = pixels;
    params[i]->x = i;
    params[i]->y = i;

    ThreadPtr thread = boost::make_shared<sf::Thread>( &PerlinThreadFunction2D, &params[i] );
    thread->Launch();
    threads.push( thread );
  }

  while( !threads.empty() ) {
    threads.front()->Wait();
    threads.pop();
  }

  TexturePtr texture = boost::make_shared<sf::Texture>();
  texture->Create( width, height );
  texture->Update( pixels.get() );

  return texture;
}

TexturePtr ResourceManager::GetPlanet(int width, int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes ) {
  boost::shared_array<sf::Uint8> pixels( new sf::Uint8[height*width*4] );

  size_t num_threads = 4;
  std::queue<ThreadPtr> threads;

  boost::shared_ptr<struct thread_params> params[ num_threads ];

  for( size_t i = 0; i < num_threads; i++ ) {
    params[i] = boost::make_shared<struct thread_params>();
    params[i]->width = width;
		params[i]->height = height;
		params[i]->step_size = num_threads;
		params[i]->color = base_color;
		params[i]->seed = seed;
		params[i]->octaves = num_octaves;
		params[i]->amplitudes = amplitudes;
		params[i]->pixels = pixels;
    params[i]->x = i;
    params[i]->y = i;

    ThreadPtr thread = boost::make_shared<sf::Thread>( &PerlinThreadFunction3D, &params[i] );
    thread->Launch();
    threads.push( thread );
  }

  while( !threads.empty() ) {
    threads.front()->Wait();
    threads.pop();
  }

  TexturePtr texture = boost::make_shared<sf::Texture>();
  texture->Create( width, height );
  texture->Update( pixels.get() );

  return texture;
}
