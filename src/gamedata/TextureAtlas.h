#ifndef ARGOS_TEXTUREATLAS_H
#define ARGOS_TEXTUREATLAS_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <fstream>
#include <Logger.h>

class TextureAtlas {
  sf::Texture texture;
  std::unordered_map<std::string, sf::Sprite > Sprites;
public:
  TextureAtlas(const std::string& basePath) {
    texture.loadFromFile(basePath + ".png");
    std::ifstream infile(basePath + ".atlas");
    std::string id, centerKeyword;
    int x, y, wx, wy, cx, cy;
    bool needsId = true;

    while (true) {
      if (needsId) {
        if (!(infile >> id))
          break;
      } else
        id = centerKeyword;

      if (!(infile >> x >> y >> wx >> wy))
        break;

      sf::Sprite sprite(texture);
      sprite.setTextureRect({x, y, wx, wy});

      infile >> centerKeyword;
      if (centerKeyword == "center") {
        infile >> cx >> cy;
        sprite.setOrigin(cx - x, cy - y);
        needsId = true;
      } else {
        needsId = false;
      }

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
