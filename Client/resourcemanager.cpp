#include <cmath>
#include <iostream>
#include "resourcemanager.h"

ResourceManager::ResourceManager() {

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














