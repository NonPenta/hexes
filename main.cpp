#include "chunk.hpp"
#include "chunkmap.hpp"
#include "context.hpp"
#include "entity.hpp"
#include "events_handlers.hpp"
#include "filehandling.hpp"
#include "hex_utils.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <boost/container_hash/hash.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>

int main() {

  // ----------------- INITIALISATION ---------------------

  // chunkStorage initialisation

  std::pair<ChunkMap, std::unordered_map<std::string, Entity>> loadPair;
  loadPair = load("test_0");

  ChunkMap chunkStorage = loadPair.first;
  std::unordered_map<std::string, Entity> entities = loadPair.second;

  // ---- Window, window settings and view initialisation
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width,
                                        sf::VideoMode::getDesktopMode().height),
                          "Map stuff for RPGs. Note : I Can't Code Shit");

  sf::View gameview(sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(sf::VideoMode::getDesktopMode().width,
                                 sf::VideoMode::getDesktopMode().height));
  float zoom = .5;
  gameview.zoom(zoom);

  window.setView(gameview);

  // Input mode initialisation
  std::unique_ptr<InputMode> currentMode = std::make_unique<SelectionMode>(
      Context{chunkStorage, entities, window, gameview});

  // ------------------------------- MAIN LOOP
  // -----------------------------------

  while (window.isOpen()) {
    // -------------------- EVENTS ----------------------------------------
    sf::Event event;
    while (window.pollEvent(event)) {
      Context context{chunkStorage, entities, window, gameview};
      std::unique_ptr<InputMode> result =
          currentMode->handleEvent(context, event);
      if (result != nullptr) {
        currentMode = std::move(result);
      }
    }

    // ------------------- MOVEMENT INPUTS ------------------------------------
    float spd = 5.f;
    gameview.move(
        -spd * (sf::Keyboard::isKeyPressed(
                   sf::Keyboard::Left)) // if going left, add -spd to x speed
            +
            spd *
                (sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Right)), // if going right, add spd to x speed
        (-spd * sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Up)) // if going up, add -spd to y speed
            +
            spd *
                (sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Down))); // if going down, add spd to x speed

    // CLEARING THE SCREEN
    window.clear();

    // SETTING THE WINDOW'S VIEW
    window.setView(gameview);
    chunkStorage.setView(gameview);
    for (auto &entity : entities) {
      entity.second.setView(gameview);
    }
    Context context{chunkStorage, entities, window, gameview};
    currentMode->updateContext(context);

    // DRAWING STUFF
    window.draw(chunkStorage);
    for (auto &entity : entities) {
      window.draw(entity.second);
    }
    window.draw(*currentMode);

    window.display();

    // decoloring the hex the mouse is on because else it leaves a trace and i
    // dont want that i just want to check wether or not i can convert back from
    // pixel to hex

    // chunkStorage.map[mpos].c[hex_in_chunk_pos.x][hex_in_chunk_pos.y].s.setFillColor(sf::Color(32,
    // 31, 35));
  }

  return 0;
}
