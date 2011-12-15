#include <cmath>
#include <queue>

#include <noise/noise.h>
#include <noise/noiseutils.h>

#include <config.hpp>
#include <resourcemanager.hpp>

ResourceManager::ResourceManager() {
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
                  std::sqrt( std::pow( ( static_cast<float>( size ) / 2.f - static_cast<float>( i ) ), 2.f ) +
			                       std::pow( ( static_cast<float>( size ) / 2.f - static_cast<float>( j ) ), 2.f ) );

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

const TexturePtr ResourceManager::GetBackground( int width, int height, int octaves, float frequency, float persistence,
                                                 float contrast, float brightness,
                                                 GradientPoint* gradient, std::size_t num_gradient_points ) {
  sf::Uint8* pixels = new sf::Uint8[height*width*4];

	noise::module::Perlin perlin_module;
	perlin_module.SetOctaveCount( octaves );
  perlin_module.SetFrequency( frequency );
  perlin_module.SetPersistence( persistence );

  noise::utils::NoiseMap noise_map;

	noise::utils::NoiseMapBuilderPlane plane_builder;

	plane_builder.SetSourceModule( perlin_module );
  plane_builder.SetDestNoiseMap( noise_map );
  plane_builder.SetDestSize( width, height );
  plane_builder.SetBounds( 0.0f, 4.0f, 0.0f, 4.0f * static_cast<float>( height ) / static_cast<float>( width ) );
  plane_builder.Build();

  noise::utils::RendererImage renderer;
  noise::utils::Image image;
  renderer.SetSourceNoiseMap( noise_map );
  renderer.SetDestImage( image );
  renderer.ClearGradient();

  for( std::size_t point_index = 0; point_index < num_gradient_points; ++point_index ) {
		renderer.AddGradientPoint( gradient[ point_index ].position,
		                           noise::utils::Color( gradient[ point_index ].r,
		                                                gradient[ point_index ].g,
		                                                gradient[ point_index ].b,
		                                                gradient[ point_index ].a ) );
  }

  renderer.EnableLight();
  renderer.SetLightContrast( contrast );
  renderer.SetLightBrightness( brightness );
  renderer.Render();

  for( int index_h = 0; index_h < height; ++index_h ) {
		for( int index_w = 0; index_w < width; ++index_w ) {
			pixels[ ( index_h * width + index_w ) * 4 + 0 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).red );
			pixels[ ( index_h * width + index_w ) * 4 + 1 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).green );
			pixels[ ( index_h * width + index_w ) * 4 + 2 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).blue );
			pixels[ ( index_h * width + index_w ) * 4 + 3 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).alpha );
		}
	}

  TexturePtr texture = std::make_shared<sf::Texture>();
  texture->Create( static_cast<unsigned int>( width ), static_cast<unsigned int>( height ) );
  texture->Update( pixels );

  delete[] pixels;

  return texture;
}

const TexturePtr ResourceManager::GetPlanet( int size, int octaves, float frequency, float persistence,
                                             float contrast, float brightness,
                                             GradientPoint* gradient, std::size_t num_gradient_points ) {
  sf::Uint8* pixels = new sf::Uint8[ size * size * 2 ];

	noise::module::Perlin perlin_module;
	perlin_module.SetOctaveCount( static_cast<int>( octaves ) );
  perlin_module.SetFrequency( frequency );
  perlin_module.SetPersistence( persistence );

	noise::utils::NoiseMap noise_map;

	noise::utils::NoiseMapBuilderSphere sphere_builder;

  sphere_builder.SetSourceModule( perlin_module );
  sphere_builder.SetDestNoiseMap( noise_map );
  sphere_builder.SetDestSize( static_cast<int>( size ), static_cast<int>( size ) / 2 );
  sphere_builder.SetBounds( -90.0f, 90.0f, -180.0f, 180.0f );
  sphere_builder.Build();

	noise::utils::RendererImage renderer;
  noise::utils::Image image;
  renderer.SetSourceNoiseMap( noise_map );
  renderer.SetDestImage( image );
  renderer.ClearGradient();

  for( std::size_t point_index = 0; point_index < num_gradient_points; ++point_index ) {
		renderer.AddGradientPoint( gradient[ point_index ].position,
		                           noise::utils::Color( gradient[ point_index ].r,
		                                                gradient[ point_index ].g,
		                                                gradient[ point_index ].b,
		                                                gradient[ point_index ].a ) );
  }

  renderer.EnableLight();
  renderer.SetLightContrast( contrast );
  renderer.SetLightBrightness( brightness );
  renderer.Render();

	for( int index_h = 0; index_h < static_cast<int>( size / 2 ); ++index_h ) {
		for( int index_w = 0; index_w < static_cast<int>( size ); ++index_w ) {
			pixels[ ( index_h * size + index_w ) * 4 + 0 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).red );
			pixels[ ( index_h * size + index_w ) * 4 + 1 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).green );
			pixels[ ( index_h * size + index_w ) * 4 + 2 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).blue );
			pixels[ ( index_h * size + index_w ) * 4 + 3 ] = static_cast<sf::Uint8>( image.GetValue( index_w, index_h ).alpha );
		}
	}

  TexturePtr texture = std::make_shared<sf::Texture>();
  texture->Create( static_cast<unsigned int>( size ), static_cast<unsigned int>( size ) / 2 );
  texture->Update( pixels );

  delete[] pixels;

  return texture;
}
