#include <array>
#include <boost/container_hash/hash.hpp>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "chunk.hpp"
#include "hex_utils.hpp"
#include "chunkmap.hpp"

int main() {

  // ----------------- INITIALISATION ---------------------

    // chunk_storage initialisation
  ChunkMap chunk_storage{-1,-1,3,3};


  // ---- Window, window settings and view initialisation
  // sf::ContextSettings settings;
  // settings.antialiasingLevel = 16;

  sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Hex Game Engine Or Shit. Note : I Can't Code Shit");

  sf::View gameview(sf::Vector2f(0.f, 0.f), sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
  float zoom = .5;
  gameview.zoom(zoom);


// ------------------------------- MAIN LOOP -----------------------------------

  while (window.isOpen())
  {
    // -------------------- EVENTS ----------------------------------------
    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type) {
        case sf::Event::Closed: window.close();         // Close the window
                                break;
        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape: window.close();  // Close the window
                                       break;

            default: break;
          }
      default: break;
      }
    }


    // -------------------- Coloring the hex the mouse is on -------------------

    // sf::Vector2i pixel_mouse_pos = sf::Mouse::getPosition(window);              // Getting the mouse pos on screen
    //
    // sf::Vector2f world_mouse_pos = window.mapPixelToCoords(pixel_mouse_pos);    // Converting it to world position
    //
    // sf::Vector2i hex_pos = hex_from_pix(world_mouse_pos);
    // sf::Vector2i chunk_pos = chunk_from_hex(hex_pos);
    // sf::Vector2i hex_in_chunk_pos = hex_within_chunk(hex_pos);
    //
    // std::pair<int, int> mpos{chunk_pos.x,chunk_pos.y};
    // chunk_storage.map[mpos].c[hex_in_chunk_pos.x][hex_in_chunk_pos.y].s.setFillColor(sf::Color(0, 255, 0));

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

    window.draw(chunk_storage);

    window.display();

    // decoloring the hex the mouse is on because else it leaves a trace and i dont want that i just want to check
    // wether or not i can convert back from pixel to hex

    // chunk_storage.map[mpos].c[hex_in_chunk_pos.x][hex_in_chunk_pos.y].s.setFillColor(sf::Color(32, 31, 35));

  }


  return 0;
}
