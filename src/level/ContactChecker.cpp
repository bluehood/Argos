#include <GameObject.h>
#include <objects/Collectable.h>
#include <players/Player.h>
#include "ContactChecker.h"

static GameObject* getGO(b2Fixture* f) {
  return reinterpret_cast<GameObject *>(f->GetUserData());
}

static void checkCollectable(GameObject* A, GameObject* B) {
  if (auto Player = dynamic_cast<Character *>(B)) {
    if (auto Collect = dynamic_cast<Collectable *>(A)) {
      Collect->apply(*Player);
    }
  }
}

static void checkPlatforms(GameObject* A, GameObject* B, b2Contact* c) {
  if (auto Player = dynamic_cast<Character *>(A)) {
    if (auto T = dynamic_cast<Tile *>(B)) {
      if (T->platform()) {
        c->SetEnabled(Player->getYSpeed() > -0.01f);
      }
    }
  }
}

void ContactChecker::BeginContact(b2Contact *contact) {
  checkCollectable(getGO(contact->GetFixtureA()), getGO(contact->GetFixtureB()));
  checkCollectable(getGO(contact->GetFixtureB()), getGO(contact->GetFixtureA()));
//  checkPlatforms(getGO(contact->GetFixtureA()), getGO(contact->GetFixtureB()), contact);
//  checkPlatforms(getGO(contact->GetFixtureB()), getGO(contact->GetFixtureA()), contact);
}

void ContactChecker::EndContact(b2Contact *contact) {
}
