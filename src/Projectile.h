#ifndef ARGOS_PROJECTILE_H
#define ARGOS_PROJECTILE_H


#include "GameObject.h"
#include "level/Level.h"

class Projectile : public GameObject {

  Level* level_;
  b2Vec2 speed_;

public:
  Projectile(Level& level, float x, float y, b2Vec2 speed) : speed_(speed), level_(&level) {

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x, y);
    BodyDef.type = b2_dynamicBody;
    BodyDef.fixedRotation = true;
    BodyDef.bullet = true;
    Body = level.World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((1.0f / 2) / SCALE, (1.0f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0;
    FixtureDef.friction = 1.0f;
    FixtureDef.shape = &Shape;
    FixtureDef.userData = (void *) this;
    FixtureDef.filter.categoryBits = B2D_BULLET;
    FixtureDef.filter.maskBits = B2D_ENEMY | B2D_LEVEL;
    Body->CreateFixture(&FixtureDef);
    Body->SetUserData((void*)this);
    Body->SetLinearVelocity({10, 0});
  }

  virtual void update() override {
    if (Body->GetLinearVelocity().Length() < 0.1f)
      ShouldBeRemoved_ = true;

    Body->SetLinearVelocity(speed_);
  }

  virtual void render(sf::RenderTarget& target) override {
    sf::Sprite sprite = level_->getData().getSprite("bullet_plasma");

    sprite.setRotation((float) level_->time);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    target.draw(sprite);
  }

};


#endif //ARGOS_PROJECTILE_H
