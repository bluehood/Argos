#ifndef ARGOS_TILEDATA_H
#define ARGOS_TILEDATA_H


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <fstream>

class TileData {

  bool passable_ = false;
  sf::Sprite sprite_;
  std::string name_;
  std::string baseName_;
  bool platform_ = false;
  size_t height_ = 32;

public:
  TileData(const std::string& name, const std::string& baseName)
      : name_(name), baseName_(baseName) {
  }

  void platform(bool v) {
    platform_ = v;
  }

  bool platform() const {
    return platform_;
  }

  void height(size_t h) {
    height_ = h;
  }

  size_t height() const {
    return height_;
  }

  const std::string& baseName() const {
    return baseName_;
  }

  const std::string& name() const {
    return name_;
  }

  void passable(bool v) {
    passable_ = v;
  }

  bool passable() const {
    return passable_;
  }

  void sprite(const sf::Sprite& s) {
    sprite_ = s;
  }

  const sf::Sprite& sprite() const {
    return sprite_;
  }
};



#endif //ARGOS_TILEDATA_H
