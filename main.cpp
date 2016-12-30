#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <Logger.h>
#include <gamedata/GameData.h>
#include <level/Level.h>
#include <players/Player.h>
#include <enemies/Snake.h>
#include <objects/Stone.h>
#include <objects/Collectable.h>


int main() {
  Logger mainLogger;

  srand(time(NULL));

  /** Prepare the window */
  sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
  Window.setFramerateLimit(60);

  GameData Data(".");

  Level level(40, 40, Data);

  for (int i = 0; i < 4; ++i)
    level.add(new Collectable(level, 80 * i, 0, Collectable::Health));

  Character* player = new Character(level, 40, 40);
  level.add(player);

  for (int i = 0; i < 10; ++i)
    level.add(new Snake(level, rand() % 1000, 0));

  while (Window.isOpen()) {
    sf::Event event;
    while (Window.pollEvent(event)) {
      if (event.type == sf::Event::EventType::Closed) {
        return 0;
      }
      if (event.type == sf::Event::EventType::KeyPressed) {
        if (event.key.code == sf::Keyboard::LShift)
          player->jump();
        if (event.key.code == sf::Keyboard::Escape)
          return 0;
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)
        == sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      player->MoveDir = Character::MoveDirection::NONE;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      player->MoveDir = Character::MoveDirection::RIGHT;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      player->MoveDir = Character::MoveDirection::LEFT;
    }
    level.update();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      int MouseX = sf::Mouse::getPosition(Window).x;
      int MouseY = sf::Mouse::getPosition(Window).y;
      Stone* stone = new Stone(level, MouseX / SCALE, MouseY / SCALE);
      level.add(stone);
    }
    level.World.Step(1 / 60.f, 8, 3);

    Window.clear(sf::Color::Black);

    level.render(Window);
    Window.display();
  }

  return 0;
}
