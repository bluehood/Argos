#ifndef ARGOS_STONE_H
#define ARGOS_STONE_H


#include "../GameObject.h"
#include "../level/Level.h"

class Stone : public GameObject {

  sf::Texture texture;

public:
  Stone(Level& level, float x, float y) {
    texture.loadFromFile("box.png");

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x, y);
    BodyDef.type = b2_dynamicBody;
    BodyDef.fixedRotation = true;
    Body = level.World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 5.f;
    FixtureDef.friction = 0.9f;
    FixtureDef.shape = &Shape;
    FixtureDef.userData = (void *) this;
    Body->CreateFixture(&FixtureDef);
    Body->SetUserData((void*)this);
  }

  virtual void update() override {}

  virtual void render(sf::RenderTarget& target) override {
    sf::Sprite sprite(texture);

    sprite.setOrigin(16.f, 16.f);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
    target.draw(sprite);
  }

};


#endif //ARGOS_STONE_H
