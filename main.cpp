#include <array>
#include <boost/container_hash/hash.hpp>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "chunk.hpp"
#include "hex_utils.hpp"
#include "chunkmap.hpp"
#include "filehandling.hpp"
#include "entity.hpp"

int main() {

  // ----------------- INITIALISATION ---------------------

    // chunk_storage initialisation

  ChunkMap chunk_storage = load("test_0");
  std::vector<Entity> entities;

  entities.push_back(Entity(0, 0, "player", 2));

  // ---- Window, window settings and view initialisation
  // sf::ContextSettings settings;
  // settings.antialiasingLevel = 16;

  sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Map stuff for RPGs. Note : I Can't Code Shit");

  sf::View gameview(sf::Vector2f(0.f, 0.f), sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
  float zoom = .5;
  gameview.zoom(zoom);


// ------------------------------- MAIN LOOP -----------------------------------

  while (window.isOpen())
  {
    // ------- GETTING MOUSE POS (needed in events so moved here) --------

    // Mouse pos on screen converted to position on world
  sf::Vector2f mousePixPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    // Converting it to hex coordinates
  sf::Vector2i mouseHexPos = hex_from_pix(mousePixPos);
    // Getting the chunk pos of the mouse
  sf::Vector2i mouseChunkPos = chunk_from_hex(mouseHexPos);
    // Getting the indexes of the hexes inside the chunk
  sf::Vector2i mouseChunkIndex = hex_within_chunk(mouseHexPos);

  std::pair<int, int> mouseChunkPair{mouseChunkPos.x, mouseChunkPos.y};

    // -------------------- EVENTS ----------------------------------------
    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type) {
        case sf::Event::Closed: window.close();         // Close the window
                                break;
        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape:
              save(chunk_storage, "test_0");
              window.close();  // Save and close the window
              break;

            default: break;
          }
      default: break;
      }
    }

    // ------------------- MOVEMENT INPUTS ------------------------------------
    float spd = 5.f;
    gameview.move(-spd*(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))         // if going left, add -spd to x speed
                  +spd*(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)),       // if going right, add spd to x speed
                 (-spd*sf::Keyboard::isKeyPressed(sf::Keyboard::Up))            // if going up, add -spd to y speed
                  +spd*(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)));       // if going down, add spd to x speed
                                                                                // SEE ? NO IFS : OPTIMISATION !!!

    // CLEARING THE SCREEN
    window.clear();

    // SETTING THE WINDOW'S VIEW
    window.setView(gameview);
    chunk_storage.setView(gameview);
    for (Entity & entity : entities) {
      entity.setView(gameview);
    }

    // DRAWING STUFF
    window.draw(chunk_storage);
    for (Entity & entity : entities) {
      window.draw(entity);
      if (entity.isSelected()) {
        window.draw(entity.getPreviewAt(mouseHexPos.x, mouseHexPos.y));
      }
    }


    window.display();

    // decoloring the hex the mouse is on because else it leaves a trace and i dont want that i just want to check
    // wether or not i can convert back from pixel to hex

    // chunk_storage.map[mpos].c[hex_in_chunk_pos.x][hex_in_chunk_pos.y].s.setFillColor(sf::Color(32, 31, 35));

  }



  return 0;
}
