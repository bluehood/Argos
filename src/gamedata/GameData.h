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

  void addTile(const std::string& name, const std::string& baseName, bool passable = false) {
    Tiles[name] = new TileData(name, baseName);
    Tiles[name]->sprite(getSprite(name));
    Tiles[name]->passable(passable);
  }

  void addMetaTile(const std::string& name, bool passable = false) {
    addTile(name + "_ul", name, passable);
    addTile(name + "_u", name, passable);
    addTile(name + "_ur", name, passable);
    addTile(name + "_l", name, passable);
    addTile(name, name, passable);
    addTile(name + "_r", name, passable);
    addTile(name + "_bl", name, passable);
    addTile(name + "_b", name, passable);
    addTile(name + "_br", name, passable);
    addTile(name + "_wvu", name, passable);
    addTile(name + "_wv", name, passable);
    addTile(name + "_wvb", name, passable);
    addTile(name + "_whl", name, passable);
    addTile(name + "_wh", name, passable);
    addTile(name + "_whr", name, passable);
    addTile(name + "_f1", name, passable);
    addTile(name + "_f2", name, passable);
  }

public:
  GameData(const std::string& path) {
    parseMetaFile(path);
    addMetaTile("cave");

    addTile("platform1", "platform", false);
    Tiles["platform1"]->platform(true);
    Tiles["platform1"]->height(6);
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
