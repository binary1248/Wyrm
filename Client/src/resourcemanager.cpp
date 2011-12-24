#include <cmath>
#include <queue>

#if defined( _MSC_VER )
	// TODO: Disable MSVC warnings for these includes too

	#include <noise/noise.h>
	#include <noise/noiseutils.h>
#elif defined( __GNUC__ )
	#pragma GCC push_options
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wreorder"

	#include <noise/noise.h>
	#include <noise/noiseutils.h>

	#pragma GCC pop_options
#else
	#include <noise/noise.h>
	#include <noise/noiseutils.h>
#endif

#include <config.hpp>
#include <game.hpp>
#include <utility.hpp>
#include <resourcemanager.hpp>

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

const TexturePtr ResourceManager::OpenImage( std::string filename ) {
  //std::map<std::string, TexturePtr>::iterator i = m_textures.find( filename );

  //if( i != m_textures.end() ) {
  //  return i->second;
  //}

  sf::Image image;
  image.LoadFromFile( filename );

  TexturePtr texture = std::make_shared<sf::Texture>();
  texture->LoadFromImage( image );

  //m_textures.insert( std::pair<sf::Uint32, TexturePtr>( 0, texture ) );

  return texture;
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

const TexturePtr ResourceManager::GetTexture( sf::Uint32 id ) const {
	std::map<sf::Uint32, TexturePtr>::const_iterator iter( m_textures.find( id ) );
	if( iter == m_textures.end() ) {
		LogConsole( "Couldn't find texture with ID: " + string_cast( id ) );

		return TexturePtr();
	}

	return iter->second;
}

const ParticleSystemPtr ResourceManager::GetParticleSystem( sf::Uint32 id ) const {
	std::map<sf::Uint32, ParticleSystemPtr>::const_iterator iter( m_particle_systems.find( id ) );
	if( iter == m_particle_systems.end() ) {
		LogConsole( "Couldn't find particle system with ID: " + string_cast( id ) );

		return ParticleSystemPtr();
	}

	return std::make_shared<ParticleSystem>( *( iter->second ) );
}

const MeshPtr ResourceManager::GetMesh( sf::Uint32 id ) const {
	std::map<sf::Uint32, MeshPtr>::const_iterator iter( m_meshes.find( id ) );
	if( iter == m_meshes.end() ) {
		LogConsole( "Couldn't find mesh with ID: " + string_cast( id ) );

		return MeshPtr();
	}

	return std::make_shared<Mesh>( *( iter->second ) );
}

void ResourceManager::GenerateResourceFromPacket( sf::Packet& packet ) {
	sf::Uint16 type;

	packet >> type;

	switch( type ) {
		case ResourceType::PLANE:
			GeneratePlaneTexture( packet );
			break;
		case ResourceType::SPHERE:
			GenerateSphereTexture( packet );
			break;
		case ResourceType::MESH:
			GenerateMesh( packet );
			break;
		case ResourceType::PARTICLESYSTEM:
			GenerateParticleSystem( packet );
			break;
		default:
			LogConsole( "Unknown resource type: " + string_cast( type ) );
			break;
	}
}

void ResourceManager::GeneratePlaneTexture( sf::Packet& packet ) {
	sf::Uint32 id;
	int width;
	int height;
	int octaves;
	float frequency;
	float persistence;
  float contrast;
  float brightness;
  std::size_t num_gradient_points;
  GradientPoint* gradient;

  packet >> id;
  packet >> width;
  packet >> height;
  packet >> octaves;
  packet >> frequency;
  packet >> persistence;
  packet >> contrast;
  packet >> brightness;
  packet >> num_gradient_points;

  gradient = new GradientPoint[ num_gradient_points ];

  for( std::size_t index = 0; index < num_gradient_points; ++index ) {
		packet >> gradient[ index ].position;
		packet >> gradient[ index ].r;
		packet >> gradient[ index ].g;
		packet >> gradient[ index ].b;
		packet >> gradient[ index ].a;
  }

  if( !width || !height ) {
		width = static_cast<int>( Game::GetGame()->GetCurrentResolution().x / 4.0f );
		height = static_cast<int>( Game::GetGame()->GetCurrentResolution().y / 4.0f );
  }

  GeneratePlaneTexture( id, width, height, octaves, frequency, persistence, contrast, brightness, gradient, num_gradient_points );
}

void ResourceManager::GeneratePlaneTexture( sf::Uint32 id, int width, int height, int octaves, float frequency, float persistence,
                                            float contrast, float brightness,
                                            GradientPoint* gradient, std::size_t num_gradient_points ) {
  sf::Uint8* pixels = new sf::Uint8[height*width*4];

	noise::module::Perlin noise_module;
	noise_module.SetSeed( 37 );
	noise_module.SetOctaveCount( octaves );
  noise_module.SetFrequency( frequency );
  noise_module.SetPersistence( persistence );
  noise_module.SetNoiseQuality( noise::NoiseQuality::QUALITY_FAST );

  noise::utils::NoiseMap noise_map;

	noise::utils::NoiseMapBuilderPlane plane_builder;

	plane_builder.SetSourceModule( noise_module );
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

  m_textures.insert( std::pair<sf::Uint32, TexturePtr>( id, texture ) );

  LogConsole( "Generated plane texture: " + string_cast( id ) );
}

void ResourceManager::GenerateSphereTexture( sf::Packet& packet ) {
	sf::Uint32 id;
	int size;
	int octaves;
	float frequency;
	float persistence;
  float contrast;
  float brightness;
  std::size_t num_gradient_points;
  GradientPoint* gradient;

  packet >> id;
  packet >> size;
  packet >> octaves;
  packet >> frequency;
  packet >> persistence;
  packet >> contrast;
  packet >> brightness;
  packet >> num_gradient_points;

  gradient = new GradientPoint[ num_gradient_points ];

  for( std::size_t index = 0; index < num_gradient_points; ++index ) {
		packet >> gradient[ index ].position;
		packet >> gradient[ index ].r;
		packet >> gradient[ index ].g;
		packet >> gradient[ index ].b;
		packet >> gradient[ index ].a;
  }

  GenerateSphereTexture( id, size, octaves, frequency, persistence, contrast, brightness, gradient, num_gradient_points );
}

void ResourceManager::GenerateSphereTexture( sf::Uint32 id, int size, int octaves, float frequency, float persistence,
                                             float contrast, float brightness,
                                             GradientPoint* gradient, std::size_t num_gradient_points ) {
  sf::Uint8* pixels = new sf::Uint8[ size * size * 2 ];

	noise::module::Perlin noise_module;
	noise_module.SetOctaveCount( static_cast<int>( octaves ) );
  noise_module.SetFrequency( frequency );
  noise_module.SetPersistence( persistence );
  noise_module.SetNoiseQuality( noise::NoiseQuality::QUALITY_FAST );

	noise::utils::NoiseMap noise_map;

	noise::utils::NoiseMapBuilderSphere sphere_builder;

  sphere_builder.SetSourceModule( noise_module );
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

  //renderer.EnableLight();
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

  m_textures.insert( std::pair<sf::Uint32, TexturePtr>( id, texture ) );

  LogConsole( "Generated sphere texture: " + string_cast( id ) );
}

void ResourceManager::GenerateParticleSystem( sf::Packet& packet ) {
	ParticleSystemPtr particle_system( std::make_shared<ParticleSystem>() );

	sf::Uint32 id;

	packet >> id;

	int size;
	float sigma2;
	float exp;

	packet >> size;
	packet >> sigma2;
	packet >> exp;

	particle_system->SetTextureParameters( size, sigma2, exp );

	std::size_t num_emitters;

	packet >> num_emitters;

	for( std::size_t index = 0; index < num_emitters; ++index ) {
		ParticleSystem::ParticleEmitter emitter;

		packet >> emitter.rate;

		packet >> emitter.spread;

		packet >> emitter.radius_start;
		packet >> emitter.radius_end;

		packet >> emitter.prototype.lifetime;

		packet >> emitter.prototype.velocity.x;
		packet >> emitter.prototype.velocity.y;

		packet >> emitter.prototype.acceleration.x;
		packet >> emitter.prototype.acceleration.y;

		packet >> emitter.prototype.size_start.x;
		packet >> emitter.prototype.size_start.y;

		packet >> emitter.prototype.size_end.x;
		packet >> emitter.prototype.size_end.y;

		packet >> emitter.prototype.color_start.r;
		packet >> emitter.prototype.color_start.g;
		packet >> emitter.prototype.color_start.b;
		packet >> emitter.prototype.color_start.a;

		packet >> emitter.prototype.color_end.r;
		packet >> emitter.prototype.color_end.g;
		packet >> emitter.prototype.color_end.b;
		packet >> emitter.prototype.color_end.a;

		particle_system->AddEmitter( emitter );
	}

	m_particle_systems.insert( std::pair<sf::Uint32, ParticleSystemPtr>( id, particle_system ) );

	LogConsole( "Generated particle system template: " + string_cast( id ) );
}

void ResourceManager::GenerateMesh( sf::Packet& packet ) {
	MeshPtr mesh( std::make_shared<Mesh>() );

	sf::Uint32 id;

	packet >> id;

	std::size_t vertice_count;
	std::size_t face_count;

	packet >> vertice_count;
	packet >> face_count;

	for( std::size_t index = 0; index < vertice_count; ++index ) {
		sf::Vector3f vertex;

		packet >> vertex.x >> vertex.y >> vertex.z;

		mesh->vertices.push_back( vertex );
	}

	for( std::size_t index = 0; index < face_count; ++index ) {
		Mesh::Face face;
		sf::Vector3f normal;

		packet >> face.v0 >> face.v1 >> face.v2;

		sf::Vector3f v0 = mesh->vertices[ face.v0 ];
		sf::Vector3f v1 = mesh->vertices[ face.v1 ];
		sf::Vector3f v2 = mesh->vertices[ face.v2 ];

		sf::Vector3f e0 = v0 - v1;
		sf::Vector3f e1 = v2 - v1;

		normal.x = ( e1.y * e0.z - e1.z * e0.y );
		normal.y = ( e1.z * e0.x - e1.x * e0.z );
		normal.z = ( e1.x * e0.y - e1.y * e0.x );

		float length = static_cast<float>( sqrt( normal.x * normal.x + normal.y * normal.y + normal.z * normal.z ) );

		normal.x /= length;
		normal.y /= length;
		normal.z /= length;

		mesh->faces.push_back( face );
		mesh->normals.push_back( normal );
	}

	m_meshes.insert( std::pair<sf::Uint32, MeshPtr>( id, mesh ) );

	LogConsole( "Generated mesh: " + string_cast( id ) );
}
