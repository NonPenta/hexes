#include <array>
#include <boost/container_hash/hash.hpp>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "chunk.hpp"

int main() {

  std::unordered_map<std::string, Chunk> chunk_storage; // Create chunk storage

  for (int cx = -1; cx <= 1; cx++) {
    for (int cy = -1; cy <= 1; cy++) {
      Chunk chunk{cx, cy};
      chunk_storage[std::to_string(cx)+";"+std::to_string(cy)] = chunk; // Initialize chunk storage
    }
  }



  sf::ContextSettings settings;
  settings.antialiasingLevel = 4;

  sf::RenderWindow window(sf::VideoMode(1280, 720), "Hex Game Engine Or Shit. Note : I Can't Code Shit");

  sf::View gameview(sf::Vector2f(0.f, 0.f), sf::Vector2f(1280.f, 720.f));
  gameview.zoom(.5f);

  while (window.isOpen())
  {

    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type) {
        case sf::Event::Closed: window.close();
                                break;
        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape: window.close();
                                      break;
            default: break;
          }
      default: break;
      }
    }

    float spd = 5.f;
    gameview.move(-spd*(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) /*if going left, add -spd to x speed*/
                  +spd*(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)), /*if going right, add spd to x speed*/
                 (-spd*sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) /*if going up, add -spd to y speed*/
                  +spd*(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))); /*if going down, add spd to x speed*/

    window.clear();

    window.setView(gameview);

    for (int cx = -1; cx <= 1; cx++) {
      for (int cy = -1; cy <= 1; cy++) {
        for (int x = -32; x <= 32; x++) {
          for (int y = -32; y <= 32; y++) {
            float sx = chunk_storage[std::to_string(cx)+";"+std::to_string(cy)].c[x+32][y+32].s.getPosition().x; // Get hexagon x
            float sy = chunk_storage[std::to_string(cx)+";"+std::to_string(cy)].c[x+32][y+32].s.getPosition().y; // Get hexagon y
            if ((-15 + gameview.getCenter().x-gameview.getSize().x/2 < sx) // If the hexagon isnt out the left border
              && (sx < gameview.getCenter().x+gameview.getSize().x/2 - 1) // If the hexagon isnt out the right border
              && (-20 + gameview.getCenter().y - gameview.getSize().y/2 < sy) // If the hexagon isnt out the up border
              && (sy < gameview.getCenter().y+gameview.getSize().y/2)) { // If the hexagon isnt out the down border
                window.draw(chunk_storage[std::to_string(cx)+";"+std::to_string(cy)].c[x+32][y+32].s);
            }
          }
        }
      }
    }

    window.display();
  }


  return 0;
}

int abs(int x) { if (x > 0) { return x; } else { return -x; }}
