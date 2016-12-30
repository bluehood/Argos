#ifndef ARGOS_TEXTUREATLAS_H
#define ARGOS_TEXTUREATLAS_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <fstream>

class TextureAtlas {
  sf::Texture texture;
  std::unordered_map<std::string, sf::Sprite > Sprites;
public:
  TextureAtlas(const std::string& basePath) {
    texture.loadFromFile(basePath + ".png");
    std::ifstream infile(basePath + ".atlas");
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


#endif //ARGOS_TEXTUREATLAS_H
