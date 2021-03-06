#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <Logger.h>
#include <Character.h>
#include <gamedata/GameData.h>
#include <level/Level.h>
#include <level/LevelGen.h>
#include <controls/PlayerControls.h>


int main() {
  Logger mainLogger;

  srand(2);

  /** Prepare the window */
  sf::RenderWindow Window(sf::VideoMode(1024, 768, 32), "Test");
  Window.setFramerateLimit(60);

  sf::View view = Window.getView();
  view.zoom(0.3f);

  GameData Data(".");

  LevelGen gen;

  Level& level = *gen.generate(Data, 100, 100);

  Character* player1 = new Character(level, Vec2(22, 32));
  level.add(player1);

  Character* player2 = new Character(level, Vec2(22, 32));
  level.add(player2);


  level.add(new Character(level, Vec2(55, 22)));

  sf::Vector2f viewCenter;

  PlayerControls controls1(1);
  player1->setPlayerControls(&controls1);

  PlayerControls controls2(2);
  player2->setPlayerControls(&controls2);

  sf::Clock clock;

  while (Window.isOpen()) {
    std::vector<sf::Event> events;
    sf::Event event;
    while (Window.pollEvent(event)) {
      events.push_back(event);
      if (event.type == sf::Event::EventType::Closed) {
        return 0;
      }
      if (event.type == sf::Event::MouseWheelScrolled) {
        view.zoom(1 - event.mouseWheelScroll.delta / 30.0f);
      }
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        return 0;

      if (event.type == sf::Event::Resized) {
        // update the view to the new size of the window
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        Window.setView(sf::View(visibleArea));
        view = Window.getView();
        view.zoom(0.3f);
      }
    }
    viewCenter.x = player1->getPos().getX();
    viewCenter.y = player1->getPos().getY();

    controls1.update();
    controls2.update();

    sf::Time elapsed = clock.restart();
    level.update(elapsed.asSeconds());

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      viewCenter.x = sf::Mouse::getPosition(Window).x;
      viewCenter.y = sf::Mouse::getPosition(Window).y;
    }
    //level.World.Step(1 / 60.f, 8, 3);

    Window.clear(sf::Color(43, 66, 93));

    //sf::Vector2f playerPos(player->getBody()->GetPosition().x * SCALE, player->getBody()->GetPosition().y * SCALE);

    view.setCenter(viewCenter);
    Window.setView(view);


    //viewCenter += (playerPos - viewCenter) * 0.3f;

    level.render(Window, viewCenter);
    Window.display();
  }

  return 0;
}
