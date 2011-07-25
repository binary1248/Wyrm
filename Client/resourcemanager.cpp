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
