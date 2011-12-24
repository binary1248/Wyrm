#include <cmath>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>

#include <config.hpp>
#include <utility.hpp>
#include <resourcemanager.hpp>

ResourceManager::ResourceManager() :
	m_resources_loaded( false ) {
}

ResourceManager::~ResourceManager() {
}

void ResourceManager::Tick( float /*time*/ ) {
	if( !m_resources_loaded ) {
    LoadResources();
  }


}

void ResourceManager::FillResourcePacket( sf::Uint32 id, PacketPtr packet ) const {
	std::map<sf::Uint32, const PacketPtr>::const_iterator iter( m_resources.find( id ) );

	if( iter == m_resources.end() ) {
		Die( "Could not find resource with ID: " + string_cast( id ) + "\n" );
	}

	packet->Append( iter->second->GetData(), iter->second->GetDataSize() );
}

void ResourceManager::AddPlaneTexture( sf::Uint32 id, int width, int height, int octaves, float frequency, float persistence,
		                                   float contrast, float brightness,
		                                   GradientPoint* gradient, std::size_t num_gradient_points ) {
	if( m_resources.find( id ) != m_resources.end() ) {
		Die( "Resource " + string_cast( id ) + " already exists.\n" );
	}

	PacketPtr packet( std::make_shared<sf::Packet>() );

	(*packet) << static_cast<sf::Uint16>( ResourceType::PLANE )
	          << id << width << height
	          << octaves << frequency << persistence << contrast << brightness << num_gradient_points;

	for( std::size_t index = 0; index < num_gradient_points; ++index ) {
		(*packet) << gradient[ index ].position;
		(*packet) << gradient[ index ].r;
		(*packet) << gradient[ index ].g;
		(*packet) << gradient[ index ].b;
		(*packet) << gradient[ index ].a;
  }

	m_resources.insert( std::pair<sf::Uint32, const PacketPtr>( id, packet ) );
}

void ResourceManager::AddSphereTexture( sf::Uint32 id, int size, int octaves, float frequency, float persistence,
		                                    float contrast, float brightness,
		                                    GradientPoint* gradient, std::size_t num_gradient_points ) {
	if( m_resources.find( id ) != m_resources.end() ) {
		Die( "Resource " + string_cast( id ) + " already exists.\n" );
	}

	PacketPtr packet( std::make_shared<sf::Packet>() );

	(*packet) << static_cast<sf::Uint16>( ResourceType::SPHERE )
	          << id << size
	          << octaves << frequency << persistence << contrast << brightness << num_gradient_points;

	for( std::size_t index = 0; index < num_gradient_points; ++index ) {
		(*packet) << gradient[ index ].position;
		(*packet) << gradient[ index ].r;
		(*packet) << gradient[ index ].g;
		(*packet) << gradient[ index ].b;
		(*packet) << gradient[ index ].a;
  }

	m_resources.insert( std::pair<sf::Uint32, const PacketPtr>( id, packet ) );
}

void ResourceManager::AddParticleSystem( sf::Uint32 id, ParticleSystem particle_system ) {
	if( m_resources.find( id ) != m_resources.end() ) {
		Die( "Resource " + string_cast( id ) + " already exists.\n" );
	}

	PacketPtr packet( std::make_shared<sf::Packet>() );

	(*packet) << static_cast<sf::Uint16>( ResourceType::PARTICLESYSTEM )
	          << id
	          << particle_system.texture_size
	          << particle_system.texture_sigma2
	          << particle_system.texture_exp;

	std::size_t emitters_size = particle_system.emitters.size();

	(*packet) << emitters_size;

	for( std::size_t index = 0; index < emitters_size; ++index ) {
		(*packet) << particle_system.emitters[ index ].rate;

		(*packet) << particle_system.emitters[ index ].spread;

		(*packet) << particle_system.emitters[ index ].radius_start;
		(*packet) << particle_system.emitters[ index ].radius_end;

		(*packet) << particle_system.emitters[ index ].prototype.lifetime;

		(*packet) << particle_system.emitters[ index ].prototype.velocity.x;
		(*packet) << particle_system.emitters[ index ].prototype.velocity.y;

		(*packet) << particle_system.emitters[ index ].prototype.acceleration.x;
		(*packet) << particle_system.emitters[ index ].prototype.acceleration.y;

		(*packet) << particle_system.emitters[ index ].prototype.size_start.x;
		(*packet) << particle_system.emitters[ index ].prototype.size_start.y;

		(*packet) << particle_system.emitters[ index ].prototype.size_end.x;
		(*packet) << particle_system.emitters[ index ].prototype.size_end.y;

		(*packet) << particle_system.emitters[ index ].prototype.color_start.r;
		(*packet) << particle_system.emitters[ index ].prototype.color_start.g;
		(*packet) << particle_system.emitters[ index ].prototype.color_start.b;
		(*packet) << particle_system.emitters[ index ].prototype.color_start.a;

		(*packet) << particle_system.emitters[ index ].prototype.color_end.r;
		(*packet) << particle_system.emitters[ index ].prototype.color_end.g;
		(*packet) << particle_system.emitters[ index ].prototype.color_end.b;
		(*packet) << particle_system.emitters[ index ].prototype.color_end.a;
	}

	m_resources.insert( std::pair<sf::Uint32, const PacketPtr>( id, packet ) );
}

void ResourceManager::AddMesh( sf::Uint32 id, std::string mesh_file ) {
	std::ifstream file( mesh_file.c_str() );

	if( !file.good() ) {
		LogConsole( "Failed to load Mesh resource " + string_cast( id ) + ", file not good." );
		return;
	}

	std::vector<sf::Vector3f> vertices;
	std::vector<sf::Vector3i> faces;

	std::string line;

	getline( file, line );

	char type;
	sf::Vector3f vertex;
	sf::Vector3i face;

	while( file.good() ) {
		std::stringstream line_stream( line );
		line_stream >> type;

		if( !line_stream.good() ) {
			LogConsole( "Failed to load Mesh resource " + string_cast( id ) + ", line stream not good." );
			return;
		}

		if( type == 'v' ) {
			line_stream >> vertex.x >> vertex.y >> vertex.z;

			vertices.push_back( vertex );
		} else if( type == 'f' ) {
			line_stream >> face.x >> face.y >> face.z;

			faces.push_back( face );
		}

		getline( file, line );
	}

	file.close();

	if( vertices.size() > 256 ) {
		LogConsole( "Failed to load Mesh resource " + string_cast( id ) + ", more than 256 vertices." );
		return;
	}

	PacketPtr packet( std::make_shared<sf::Packet>() );

	(*packet) << static_cast<sf::Uint16>( ResourceType::MESH ) << id;

	(*packet) << vertices.size();
	(*packet) << faces.size();

	for( std::size_t index = 0; index < vertices.size(); ++index ) {
		(*packet) << vertices[ index ].x << vertices[ index ].y << vertices[ index ].z;
	}

	for( std::size_t index = 0; index < faces.size(); ++index ) {
		if( faces[ index ].x < 1 || faces[ index ].x > 256 ||
		    faces[ index ].y < 1 || faces[ index ].y > 256 ||
		    faces[ index ].z < 1 || faces[ index ].z > 256 ) {
			LogConsole( "Failed to load Mesh resource " + string_cast( id ) + ", invalid vertice index." );
			return;
		}

		(*packet) << static_cast<sf::Uint8>( faces[ index ].x - 1 )
		          << static_cast<sf::Uint8>( faces[ index ].y - 1 )
		          << static_cast<sf::Uint8>( faces[ index ].z - 1 );
	}

	m_resources.insert( std::pair<sf::Uint32, const PacketPtr>( id, packet ) );

	//LogConsole( "Size: " + string_cast( packet->GetDataSize() ) );

	//LogConsole( "Loaded Mesh " + string_cast( id ) + ": " + string_cast( vertices.size() ) +
	//            " vertices, " + string_cast( faces.size() ) + " faces." );
}

void ResourceManager::LoadResources() {
	LogConsole( "Loading resources..." );

	{
		GradientPoint points[2] = {
			{ -1.0000f, 0,  0,  0, 255 },
			{  1.0000f, 0, 60, 80, 255 }
		};

		AddPlaneTexture( 1,
		                 0,
		                 0,
		                 4,
		                 1.0f,
		                 0.5f,
		                 1.0f,
		                 1.0f,
		                 points,
		                 2 );
	}

	{
		GradientPoint points[8] = {
			{ -1.0000f,   0,   0, 128, 255 }, // deeps
			{ -0.2500f,   0,   0, 255, 255 }, // shallow
			{  0.0000f,   0, 128, 255, 255 }, // shore
			{  0.0625f, 240, 240,  64, 255 }, // sand
			{  0.1250f,  32, 160,   0, 255 }, // grass
			{  0.3750f, 224, 224,   0, 255 }, // dirt
			{  0.7500f, 128, 128, 128, 255 }, // rock
			{  1.0000f, 255, 255, 255, 255 }  // snow
		};

		GradientPoint points_bump_map[2] = {
			{ -1.0000f,   0,   0,   0, 255 },
			{  1.0000f, 255, 255, 255, 255 }
		};

		AddSphereTexture( 2,
											256,
											8,
											1.0f,
											0.5f,
											1.0f,
											1.0f,
											points,
											8 );

		AddSphereTexture( 3,
											256,
											8,
											1.0f,
											0.5f,
											1.0f,
											1.0f,
											points_bump_map,
											2 );
	}

	{
		ParticleSystem particle_system;

		particle_system.texture_size = 128;
		particle_system.texture_sigma2 = 255.0f;
		particle_system.texture_exp = 1.8f;

		ParticleSystem::ParticleEmitter emitter;

		emitter.radius_start = 0.0f;
		emitter.radius_end = 1.0f;
		emitter.rate = 16.0f;
		emitter.spread = 180.0f;

		emitter.prototype.velocity = sf::Vector2f( 0.5f, 0.0f );
		emitter.prototype.color_start = { 1.0f, 1.0f, 0.85f, 1.0f };
		emitter.prototype.color_end = { 1.0f, 0.75f, 0.0f, 0.0f };
		emitter.prototype.lifetime = 1.0f;
		emitter.prototype.size_start = sf::Vector2f( 200.0f, 200.0f );
		emitter.prototype.size_end = sf::Vector2f( 550.0f, 550.0f );

		particle_system.emitters.push_back( emitter );

		AddParticleSystem( 4, particle_system );
	}

	AddMesh( 5, "data/ship.obj" );

	m_resources_loaded = true;

	LogConsole( "Resources loaded" );
}
