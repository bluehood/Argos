#ifndef ARGOS_GAMEDATA_H
#define ARGOS_GAMEDATA_H

#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <cassert>
#include <iostream>
#include <Logger.h>
#include "TileData.h"
#include "TextureAtlas.h"

class GameData {

  std::vector<TextureAtlas*> TextureAtlas_;
  std::unordered_map<std::string, TileData*> Tiles;

  void addTile(const std::string& name, const std::string& baseName, bool passable = false, bool platform = false, unsigned height = 32) {
    Tiles[name] = new TileData(name, baseName);
    Tiles[name]->sprite(getSprite(name));
    Tiles[name]->passable(passable);
    Tiles[name]->platform(platform);
    Tiles[name]->height(height);
  }

  void addMetaTile(const std::string& name, bool passable = false, bool platform = false, unsigned height = 32) {
    addTile(name + "_ul", name, passable, platform, height);
    addTile(name + "_u", name, passable, platform, height);
    addTile(name + "_ur", name, passable, platform, height);
    addTile(name + "_l", name, passable, platform, height);
    addTile(name, name, passable, platform, height);
    addTile(name + "_r", name, passable, platform, height);
    addTile(name + "_bl", name, passable, platform, height);
    addTile(name + "_b", name, passable, platform, height);
    addTile(name + "_br", name, passable, platform, height);
    addTile(name + "_wvu", name, passable, platform, height);
    addTile(name + "_wv", name, passable, platform, height);
    addTile(name + "_wvb", name, passable, platform, height);
    addTile(name + "_whl", name, passable, platform, height);
    addTile(name + "_wh", name, passable, platform, height);
    addTile(name + "_whr", name, passable, platform, height);
    addTile(name + "_f1", name, passable, platform, height);
    addTile(name + "_f2", name, passable, platform, height);
  }

public:
  GameData(const std::string& path) {
    parseMetaFile(path);
    addMetaTile("cave");

    addMetaTile("platform", false, true, 6);

  }

  virtual ~GameData() {
    for (TextureAtlas* s : TextureAtlas_)
      delete s;
    for (auto& s : Tiles)
      delete s.second;
  }

  const sf::Sprite& getSprite(const std::string& id) {
    for (TextureAtlas* ts : TextureAtlas_) {
      if (ts->contains(id)) {
        return (*ts)[id];
      }
    }
    assert(false);
  }

  TileData* operator[](const std::string& id) {
    auto I = Tiles.find(id);
    if (I != Tiles.end())
      return I->second;
    return nullptr;
  }

  Logger mainLogger;

  void parseMetaFile(const std::string& path) {
    mainLogger << "Parsing " << path << "/meta.dat" << mainLogger;
    std::ifstream input(path + "/meta.dat");
    std::string fileType;
    std::string filePath;
    while (input >> fileType >> filePath) {
      filePath = path + "/" + filePath;
      if (fileType == "meta") {
        parseMetaFile(filePath);
      } else if (fileType == "texatlas") {
        mainLogger << "Loading tile set " << filePath << mainLogger;
        TextureAtlas_.push_back(new TextureAtlas(filePath));
      } else {
        mainLogger << "Unknown file type in meta file " << path << "/meta.dat: "
                   << mainLogger;
      }
    }
  }
};


#endif //ARGOS_GAMEDATA_H
