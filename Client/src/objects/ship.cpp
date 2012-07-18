#include <GL/glew.h>

#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <objects/ship.hpp>
#include <objects/objects.hpp>
#include <game.hpp>
#include <utility.hpp>
#include <objectmanager.hpp>

REGISTER_FACTORY(ObjectType::SHIP,Ship);

Ship::Ship( sf::Uint16 id, sf::Packet& packet ) :
	Object( ObjectType::SHIP, id, packet ),
	m_vertex_vbo( 0 ) {
	packet >> m_thrust;

	Init();
}

Ship::~Ship() {
	glDeleteBuffers( 1, &m_vertex_vbo );
}

void Ship::Init() {
	m_is_player = false;

	m_acceleration.x = 0;
	m_acceleration.y = 0;

	m_text.setFont( sf::Font::getDefaultFont() );
	m_text.setColor( sf::Color( 0, 255, 0 ) );
	m_text.setScale( 0.4f, 0.4f );

	m_mesh = Game::GetGame()->GetResourceManager()->GetMesh( GetResourceId() );

	if( !m_mesh ) {
		return;
	}

	std::size_t face_count = m_mesh->faces.size();

	std::vector<GLfloat> fbo_data;
	fbo_data.reserve( face_count * 27 );

	float scale_x = GetSize().x;
	float scale_y = GetSize().x;
	float scale_z = ( scale_x + scale_y ) / 2.0f;

	sf::Vector3f color( 1.0f, 0.0f, 0.0f );

	for( std::size_t index = 0; index < face_count; ++index ) {
		Mesh::Face face = m_mesh->faces[ index ];

		fbo_data.push_back( m_mesh->vertices[ face.v0 ].x * scale_x );
		fbo_data.push_back( m_mesh->vertices[ face.v0 ].z * scale_z );
		fbo_data.push_back( m_mesh->vertices[ face.v0 ].y * scale_y * -1.0f );

		fbo_data.push_back( m_mesh->normals[ index ].x );
		fbo_data.push_back( m_mesh->normals[ index ].z );
		fbo_data.push_back( m_mesh->normals[ index ].y * -1.0f );

		fbo_data.push_back( color.x );
		fbo_data.push_back( color.y );
		fbo_data.push_back( color.z );

		fbo_data.push_back( m_mesh->vertices[ face.v1 ].x * scale_x );
		fbo_data.push_back( m_mesh->vertices[ face.v1 ].z * scale_z );
		fbo_data.push_back( m_mesh->vertices[ face.v1 ].y * scale_y * -1.0f );

		fbo_data.push_back( m_mesh->normals[ index ].x );
		fbo_data.push_back( m_mesh->normals[ index ].z );
		fbo_data.push_back( m_mesh->normals[ index ].y * -1.0f );

		fbo_data.push_back( color.x );
		fbo_data.push_back( color.y );
		fbo_data.push_back( color.z );

		fbo_data.push_back( m_mesh->vertices[ face.v2 ].x * scale_x );
		fbo_data.push_back( m_mesh->vertices[ face.v2 ].z * scale_z );
		fbo_data.push_back( m_mesh->vertices[ face.v2 ].y * scale_y * -1.0f );

		fbo_data.push_back( m_mesh->normals[ index ].x );
		fbo_data.push_back( m_mesh->normals[ index ].z );
		fbo_data.push_back( m_mesh->normals[ index ].y * -1.0f );

		fbo_data.push_back( color.x );
		fbo_data.push_back( color.y );
		fbo_data.push_back( color.z );
	}

	glGenBuffers( 1, &m_vertex_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_vertex_vbo );
	glBufferData( GL_ARRAY_BUFFER, fbo_data.size() * sizeof( GLfloat ), &fbo_data[0], GL_STATIC_DRAW );
}

void Ship::Update( float time ) {
	m_acceleration.x = static_cast<float>( -cos( clean_angle( GetRotation() ) ) ) * ( -m_thrust );
	m_acceleration.y = static_cast<float>(	sin( clean_angle( GetRotation() ) ) ) * ( -m_thrust );
	SetVelocity( GetVelocity() + ( m_acceleration * time ) );

	Object::Update( time );

	sf::FloatRect rect = m_text.getLocalBounds();
	m_text.setPosition( GetPosition() + sf::Vector2f( -rect.width / 2 , m_image_height / 3 ) );
}

void Ship::Draw( sf::RenderWindow& /*target*/ ) {
	glPushMatrix();

	glTranslatef( ( GetPosition().x ),
								( GetPosition().y ),
								64.0f );

	glRotatef( -GetRotation(), 0.0f, 0.0f, 1.0f );

	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_LIGHTING );

	glBindBuffer( GL_ARRAY_BUFFER, m_vertex_vbo );

	glVertexPointer( 3, GL_FLOAT, 9 * sizeof( GLfloat ), 0 );
	glNormalPointer( GL_FLOAT, 9 * sizeof( GLfloat ), reinterpret_cast<GLvoid*>( 3 * sizeof( GLfloat ) ) );
	glColorPointer( 3, GL_FLOAT, 9 * sizeof( GLfloat ), reinterpret_cast<GLvoid*>( 6 * sizeof( GLfloat ) ) );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

	glDrawArrays( GL_TRIANGLES, 0, m_mesh->faces.size() * 3 );

	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );

	glPopMatrix();

	m_text.setString( GetName() );
	//target.Draw( m_text );
}

void Ship::HandlePacket( sf::Packet& packet ) {
	sf::Uint16 type1;
	packet >> type1;

	switch( type1 ) {
		case ServerToClientObject::OBJECT_UPDATE: {
			Object::HandlePacket( packet );
			packet >> m_thrust;
			break;
		}
		case ServerToClientObject::OBJECT_STATE: {
			sf::Uint16 type;
			sf::String name;
			sf::Uint32 resource_id;
			sf::Vector2f size;
			packet >> type >> name >> resource_id >> size.x >> size.y;
			assert( type == GetType() );
			SetName( name );
			SetResourceId( resource_id );
			SetSize( size );
			Object::HandlePacket( packet );
			packet >> m_thrust;
			break;
		}
		case ServerToClientObject::OBJECT_REMOVE: {
			Delete();
			break;
		}
	}
}

void Ship::SetPlayer( bool is_player ) {
	m_is_player = is_player;
}

bool Ship::IsPlayer() const {
	return m_is_player;
}
