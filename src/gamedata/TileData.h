#ifndef ARGOS_TILEDATA_H
#define ARGOS_TILEDATA_H


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <fstream>

class TileSet {
  sf::Texture texture;
  std::unordered_map<std::string, sf::Sprite > Sprites;
public:
  TileSet(const std::string& basePath) {
    texture.loadFromFile(basePath + ".png");
    std::ifstream infile(basePath + ".dat");
    std::string id;
    int x, y, wx, wy;

    while (infile >> id >> x >> y >> wx >> wy) {
      sf::Sprite sprite(texture);
      sprite.setTextureRect({x, y, wx, wy});
      Sprites[id] = sprite;
    }
  }

  bool contains(const std::string& id) const {
    return Sprites.find(id) != Sprites.end();
  }

  sf::Sprite& operator[](const std::string& id) {
    return Sprites[id];
  }
};

class TileData {

  bool passable_ = false;
  sf::Sprite sprite_;
  std::string name_;
  std::string baseName_;

public:
  TileData(const std::string& name, const std::string& baseName)
      : name_(name), baseName_(baseName) {
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
