#ifndef ARGOS_LEVEL_H
#define ARGOS_LEVEL_H


#include <gamedata/GameData.h>
#include <GameObject.h>
#include "Tile.h"
#include "ContactChecker.h"
#include "TileMap.h"
#include "AmbientTile.h"

class Level {

  GameData& Data_;
  std::vector<GameObject *> Objects;
  TileMap<Tile> Tiles;
  TileMap<AmbientTile> BackgroundTiles;
  TileMap<AmbientTile> ForegroundTiles;
  size_t w;
  size_t h;

  sf::Texture background;

  std::vector<int> surroundings(int x, int y) {
    std::string n = get(x, y).baseName();
    return {
        get(x-1,y-1).baseName() == n ? 1 : 0,
        get(x  ,y-1).baseName() == n ? 1 : 0,
        get(x+1,y-1).baseName() == n ? 1 : 0,
        get(x-1,y  ).baseName() == n ? 1 : 0,
        get(x+1,y  ).baseName() == n ? 1 : 0,
        get(x-1,y+1).baseName() == n ? 1 : 0,
        get(x  ,y+1).baseName() == n ? 1 : 0,
        get(x+1,y+1).baseName() == n ? 1 : 0
    };
  }

  bool compatible(std::vector<int> is, std::vector<int> expected) {
    assert(is.size() == 8);
    assert(is.size() == expected.size());
    for (size_t i = 0; i < is.size(); i++) {
      if (expected[i] == 2)
        continue;
      if (expected[i] != is[i])
        return false;
    }
    return true;
  }


  std::string getSuffix(std::vector<int> s) {
    if (compatible(s, {2, 0, 2, 0, 1, 2, 1, 2}))
      return "_ul";
    if (compatible(s, {2, 0, 2, 1, 1, 2, 1, 2}))
      return "_u";
    if (compatible(s, {2, 0, 2, 1, 0, 2, 1, 2}))
      return "_ur";

    if (compatible(s, {2, 1, 2, 1, 1, 2, 0, 2}))
      return "_b";

    if (compatible(s, {2, 1, 2, 0, 1, 2, 0, 2}))
      return "_bl";
    if (compatible(s, {2, 1, 2, 1, 0, 2, 0, 2}))
      return "_br";

    if (compatible(s, {2, 1, 2, 0, 1, 2, 1, 2}))
      return "_l";

    if (compatible(s, {2, 1, 2, 1, 0, 2, 1, 2}))
      return "_r";

    if (compatible(s, {2, 1, 2, 0, 0, 2, 1, 2}))
      return "_wv";
    if (compatible(s, {2, 0, 2, 0, 0, 2, 0, 2})) {
      if (rand() % 2)
        return "_f1";
      else
        return "_f2";
    }
    if (compatible(s, {2, 0, 2, 0, 0, 2, 1, 2}))
      return "_wvu";
    if (compatible(s, {2, 1, 2, 0, 0, 2, 0, 2}))
      return "_wvb";
    if (compatible(s, {2, 0, 2, 0, 1, 2, 0, 2}))
      return "_whl";
    if (compatible(s, {2, 0, 2, 1, 0, 2, 0, 2}))
      return "_whr";
    if (compatible(s, {2, 0, 2, 1, 1, 2, 0, 2}))
      return "_wh";
    return "";
  }


public:
  Level(size_t w, size_t h, GameData& data) : Data_(data), World({0, GRAVITY}), w(w), h(h) {
    background.loadFromFile("background.png");
    background.setRepeated(true);
    Tiles = TileMap<Tile>(w, h);
    BackgroundTiles = TileMap<AmbientTile>(w, h);
    ForegroundTiles = TileMap<AmbientTile>(w, h);
    for (size_t x = 0; x < w; ++x) {
      for (size_t y = 0; y < h; ++y) {
        Tiles.get(x, y).x = x;
        Tiles.get(x, y).y = y;
        BackgroundTiles.get(x, y) = AmbientTile(x, y);
        if (y > 4)
          BackgroundTiles.get(x, y).spriteName = y > 5 ? "cave_back" : "cave_to_night";
        ForegroundTiles.get(x, y) = AmbientTile(x, y);
      }
    }
  }
  b2World World;


  void finalize() {
    for (size_t x = 0; x < w; ++x) {
      for (size_t y = 0; y < h; ++y) {
        if (!get(x, y).empty()) {
          std::string name = get(x, y).name();
          if (auto td = Data_[name + getSuffix(surroundings(x, y))])
            get(x, y).setData(World, td);
        }
      }
    }
  }

  GameData& getData() {
    return Data_;
  }

  ContactChecker hasContact;
  double time = 0;

  Tile& get(int x, int y) {
    return Tiles.get(x, y);
  }

  void update() {
    time += 1 / 60.0;
    for (auto I = Objects.begin(); I != Objects.end();) {
      (*I)->update();
      if ((*I)->shouldBeRemoved()) {
        delete *I;
        I = Objects.erase(I);
      } else {
        ++I;
      }
    }
  }

  void add(GameObject* o) {
    Objects.push_back(o);
  }

  void render(sf::RenderTarget& target, sf::Vector2f center) {
//    sf::Sprite back(background);
//    back.setTextureRect({0, 0, target.getView().getSize().x + 100, target.getView().getSize().y + 100});
//    back.setOrigin(back.getLocalBounds().width / 2, back.getLocalBounds().height / 2);
//    back.setPosition(center);
//    target.draw(back);

    for (AmbientTile& t : BackgroundTiles)
      t.render(Data_, target);
    for (Tile& t : Tiles)
      t.render(target);
    for (GameObject* o : Objects)
      o->render(target);
  }
};



#endif //ARGOS_LEVEL_H
