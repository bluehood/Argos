#ifndef ARGOS_TILE_H
#define ARGOS_TILE_H


#include <GameObject.h>
#include <gamedata/TileData.h>

class Tile : public GameObject {
  b2Body* Body = nullptr;

  TileData* Data = nullptr;

  void destroyBody(b2World& World) {
    if (Body) {
      World.DestroyBody(Body);
      Body = nullptr;
    }
  }

public:
  Tile() {}

  void setData(b2World& world, TileData* d) {
    Data = d;
    if (!d->passable()) {
      createBody(world);
    }
  }

  const std::string& name() const {
    if (Data)
      return Data->name();
    else {
      static std::string noName;
      return noName;
    }
  }

  const std::string& baseName() const {
    if (Data)
      return Data->baseName();
    else {
      static std::string noName;
      return noName;
    }
  }

  bool hasCliff() const {
    if (name().size() < 2)
      return false;
    if (name()[name().size() - 2] == 'u')
      return true;
    if (name()[name().size() - 2] == 'f')
      return true;
    if (name()[name().size() - 2] == 'v' && name()[name().size() - 1] == 'u')
      return true;
    if (name()[name().size() - 2] == 'h' && name()[name().size() - 1] == 'l')
      return true;
    if (name()[name().size() - 2] == 'h' && name()[name().size() - 1] == 'r')
      return true;

    return false;
  }

  void createBody(b2World& World) {
    destroyBody(World);

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x, y);
    BodyDef.type = b2_staticBody;
    BodyDef.fixedRotation = true;
    Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 10.f;
    FixtureDef.friction = 0.9f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    Body->SetUserData((void*)this);
  }

  size_t x, y;

  bool empty() {
    return !Data;
  }

  void render(sf::RenderTarget& target) {
    if (!Body)
      return;
    if (!Data)
      return;

    sf::Sprite sprite = Data->sprite();

    sprite.setOrigin(16.f, 16.f);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
    target.draw(sprite);
  }

};



#endif //ARGOS_TILE_H
