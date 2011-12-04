#include <cmath>
#include <queue>

#include <noise/noise.h>
#include <noise/noiseutils.h>

#include <config.hpp>
#include <perlin.hpp>
#include <resourcemanager.hpp>

struct thread_params {
  int x;
  int y;
  size_t step_size;
  unsigned int width;
  unsigned int height;
  sf::Color color;
  int seed;
  size_t octaves;
  float* amplitudes;
  sf::Uint8* pixels;
};

typedef std::shared_ptr<struct thread_params> ThreadParamsPtr;
typedef std::shared_ptr<sf::Thread> ThreadPtr;

inline float InterpolateCosine(float a, float b, float x) {
	float f = ( 1 - cos(x * static_cast<float>( M_PI ) ) ) * 0.5f;
	return (1 - f) * a + f * b;
}

ResourceManager::ResourceManager() {
  PreparePerlinNoise();
}

ResourceManager::~ResourceManager() {
}

const TexturePtr ResourceManager::OpenImage( std::string filename ) {
  std::map<std::string, TexturePtr>::iterator i = m_textures.find( filename );

  if( i != m_textures.end() ) {
    return i->second;
  }

  sf::Image image;
  image.LoadFromFile( filename );

  TexturePtr texture = std::make_shared<sf::Texture>();
  texture->LoadFromImage( image );

  m_textures.insert( std::pair<std::string, TexturePtr>( filename, texture ) );

  return texture;
}

const SoundBufferPtr ResourceManager::OpenSound( std::string filename ) {
  std::map<std::string, SoundBufferPtr>::iterator i = m_sounds.find( filename );

  if( i != m_sounds.end() ) {
    return i->second;
  }

  SoundBufferPtr sound = std::make_shared<sf::SoundBuffer>();

  sound->LoadFromFile( filename );

  m_sounds.insert( std::pair<std::string, SoundBufferPtr>( filename, sound ) );

  return sound;
}

const TexturePtr ResourceManager::GetParticle(int size, float max, float exp) {
	std::vector<ParticleTexturePair>::const_iterator particle_texture_pair( m_particles.begin() );
	std::vector<ParticleTexturePair>::const_iterator end( m_particles.end() );

  for( ; particle_texture_pair != end; ++particle_texture_pair ) {
		if( particle_texture_pair->first.size == size &&
				particle_texture_pair->first.max == max &&
				particle_texture_pair->first.exp == exp ) {
      return particle_texture_pair->second;
    }
  }

  sf::Uint8* pixels = new sf::Uint8[size*size*4];

	for( int i = 0; i < size; i++ ) {
    for( int j = 0; j < size; j++ ) {
      float val = static_cast<float>( size ) / 2.f -
                  std::sqrt( std::pow( ( static_cast<float>( size ) / 2.f - static_cast<float>( i ) ), 2 ) +
			                       std::pow( ( static_cast<float>( size ) / 2.f - static_cast<float>( j ) ), 2 ) );

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

  TexturePtr texture = std::make_shared<sf::Texture>();
  texture->Create( size, size );
  texture->Update( pixels );

  delete[] pixels;

  struct particle_id id;
  id.size = size;
  id.max = max;
  id.exp = exp;

  m_particles.push_back( std::pair<struct particle_id, TexturePtr>(id, texture) );

  return texture;
}

void PerlinThreadFunction2D( void* thread_param ) {
  struct thread_params* params = static_cast<thread_params*>( thread_param );

  std::size_t start_y = params->y;

  for( std::size_t y = start_y; y < params->height; y += params->step_size ) {
    for( std::size_t x = 0; x < params->width; x++ ) {
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
  struct thread_params* params = static_cast<thread_params*>( thread_param );

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

      sf::Uint8 cr = static_cast<sf::Uint8>( val * ( static_cast<float>( params->color.r ) / 255.f ) );
      sf::Uint8 cg = static_cast<sf::Uint8>( val * ( static_cast<float>( params->color.g ) / 255.f ) );
      sf::Uint8 cb = static_cast<sf::Uint8>( val * ( static_cast<float>( params->color.b ) / 255.f ) );

      params->pixels[(y*params->width+x)*4+0] = cr;
      params->pixels[(y*params->width+x)*4+1] = cg;
      params->pixels[(y*params->width+x)*4+2] = cb;

      float dist = static_cast<float>( sqrt( (x-params->width/2)*(x-params->width/2) + (y-params->height/2)*(y-params->height/2) ) );



      if( dist <= static_cast<float>( radius-8 ) ) {
        params->pixels[(y*params->width+x)*4+3] = 255;
      } else if( dist <= (float)(radius-4) ) {
        float border = radius - static_cast<float>( dist );
        params->pixels[(y*params->width+x)*4+0] = static_cast<sf::Uint8>( InterpolateCosine( params->pixels[(y*params->width+x)*4+0], 255, (10-border)/15 ) );
        params->pixels[(y*params->width+x)*4+1] = static_cast<sf::Uint8>( InterpolateCosine( params->pixels[(y*params->width+x)*4+1], 255, (10-border)/15 ) );
        params->pixels[(y*params->width+x)*4+2] = static_cast<sf::Uint8>( InterpolateCosine( params->pixels[(y*params->width+x)*4+2], 255, (10-border)/15 ) );
        params->pixels[(y*params->width+x)*4+3] = static_cast<sf::Uint8>(border * 64);
      } else {
        params->pixels[(y*params->width+x)*4+3] = 0;
      }
    }
  }
}

const TexturePtr ResourceManager::GetBackground( unsigned int width, unsigned int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes ) {
  sf::Uint8* pixels = new sf::Uint8[height*width*4];

  int num_threads = 4;
  std::queue<ThreadPtr> threads;

  struct thread_params* params = new struct thread_params[ num_threads ];

  for( int i = 0; i < num_threads; i++ ) {
    params[i].width = width;
		params[i].height = height;
		params[i].step_size = num_threads;
		params[i].color = base_color;
		params[i].seed = seed;
		params[i].octaves = num_octaves;
		params[i].amplitudes = amplitudes;
		params[i].pixels = pixels;
    params[i].x = i;
    params[i].y = i;

    ThreadPtr thread = std::make_shared<sf::Thread>( &PerlinThreadFunction2D, &params[i] );
    thread->Launch();
    threads.push( thread );
  }

  while( !threads.empty() ) {
    threads.front()->Wait();
    threads.pop();
  }

  delete[] params;

  TexturePtr texture = std::make_shared<sf::Texture>();
  texture->Create( width, height );
  texture->Update( pixels );

  delete[] pixels;

  return texture;
}

const TexturePtr ResourceManager::GetPlanet( unsigned int width, unsigned int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes ) {
  sf::Uint8* pixels = new sf::Uint8[height*width*4];

  int num_threads = 4;
  std::queue<ThreadPtr> threads;

  struct thread_params* params = new struct thread_params[ num_threads ];

  for( int i = 0; i < num_threads; i++ ) {
    params[i].width = width;
		params[i].height = height;
		params[i].step_size = num_threads;
		params[i].color = base_color;
		params[i].seed = seed;
		params[i].octaves = num_octaves;
		params[i].amplitudes = amplitudes;
		params[i].pixels = pixels;
    params[i].x = i;
    params[i].y = i;

    ThreadPtr thread = std::make_shared<sf::Thread>( &PerlinThreadFunction3D, &params[i] );
    thread->Launch();
    threads.push( thread );
  }

  while( !threads.empty() ) {
    threads.front()->Wait();
    threads.pop();
  }

  delete[] params;
/*
	noise::module::Perlin perlin_module;
	noise::utils::NoiseMap noise_map;
	noise::utils::NoiseMapBuilderPlane plane_builder;
  plane_builder.SetSourceModule( perlin_module );
  plane_builder.SetDestNoiseMap( noise_map );
  plane_builder.SetDestSize( width, height );
  plane_builder.SetBounds( 0.0, 4.0, 0.0, 4.0 );
  plane_builder.Build();

	noise::utils::RendererImage renderer;
  noise::utils::Image image;
  renderer.SetSourceNoiseMap( noise_map );
  renderer.SetDestImage( image );
  renderer.ClearGradient ();
  renderer.AddGradientPoint(-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
  renderer.AddGradientPoint(-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
  renderer.AddGradientPoint( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
  renderer.AddGradientPoint( 0.0625, utils::Color (240, 240,  64, 255)); // sand
  renderer.AddGradientPoint( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
  renderer.AddGradientPoint( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
  renderer.AddGradientPoint( 0.7500, utils::Color (128, 128, 128, 255)); // rock
  renderer.AddGradientPoint( 1.0000, utils::Color (255, 255, 255, 255)); // snow
  renderer.EnableLight();
  renderer.SetLightContrast(1.0);
  renderer.SetLightBrightness(1.0);
  renderer.Render();

	for( std::size_t index_h = 0; index_h < height; ++index_h ) {
		for( std::size_t index_w = 0; index_w < width; ++index_w ) {
			pixels[ ( index_h * width + index_w ) * 4 + 0 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).red );
			pixels[ ( index_h * width + index_w ) * 4 + 1 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).green );
			pixels[ ( index_h * width + index_w ) * 4 + 2 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).blue );
			pixels[ ( index_h * width + index_w ) * 4 + 3 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).alpha );
		}
	}
*/
  TexturePtr texture = std::make_shared<sf::Texture>();
  texture->Create( width, height );
  texture->Update( pixels );

  delete[] pixels;

  return texture;
}
