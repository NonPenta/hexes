#include <array>
#include <boost/container_hash/hash.hpp>
#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "chunk.hpp"

int main() {

  // ----------------- INITIALISATION ---------------------

    // chunk_storage initialisation
  std::unordered_map<std::string, Chunk> chunk_storage;                         // Create chunk storage

  for (int cx = -1; cx <= 1; cx++) {
    for (int cy = -1; cy <= 1; cy++) {
      Chunk chunk{cx, cy};
      chunk_storage[std::to_string(cx)+";"+std::to_string(cy)] = chunk;         // Initialize chunk storage
    }
  }


  // ---- Window, window settings and view initialisation
  sf::ContextSettings settings;
  settings.antialiasingLevel = 4;

  sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Hex Game Engine Or Shit. Note : I Can't Code Shit");

  sf::View gameview(sf::Vector2f(0.f, 0.f), sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
  float zoom = 1;
  gameview.zoom(zoom);


// ------------------------------- MAIN LOOP -----------------------------------

  while (window.isOpen())
  {
    // -------------------- EVENTS ----------------------------------------
    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type) {
        case sf::Event::Closed: window.close();         // Fermer la fenêtre
                                break;
        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape: window.close();  // Fermer la fenêtre
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



    // ----------------- DRAWING EVERYTHING -----------------------------------

    // ---- getting view position -------
    float view_x = gameview.getCenter().x;
    float view_y = gameview.getCenter().y;

    // ---- getting view size ---------------
    float delta_view_width = gameview.getSize().x/2;
    float delta_view_height = gameview.getSize().y/2;                                  // Divided by 2 because then we can get border from center


    for (int cx = -1; cx <= 1; cx++) {                                          // Looping through chunk_storage's chunk coordinates
      for (int cy = -1; cy <= 1; cy++) {
        for (int x = -32; x <= 32; x++) {                                       // Looping through chunk's hex coordinates
          for (int y = -32; y <= 32; y++) {

            float shape_x = chunk_storage[std::to_string(cx)+";"+std::to_string(cy)].c[x+32][y+32].s.getPosition().x; // Get hexagon x
            float shape_y = chunk_storage[std::to_string(cx)+";"+std::to_string(cy)].c[x+32][y+32].s.getPosition().y; // Get hexagon y

            // here was getting the view's position

            // here was getting the view's height and width

            // ------------- OPTIMISATION because drawing shit you cant see isnt
            // very smort and kills framerate ----------------

            if ((-15 + view_x - delta_view_width < shape_x)                           // If the hexagon isnt out the left border
              && (shape_x < view_x + delta_view_width - 1)                            // If the hexagon isnt out the right border
              && (-20 + view_y - delta_view_height < shape_y)                         // If the hexagon isnt out the up border
              && (shape_y < view_y + delta_view_height)) {                            // If the hexagon isnt out the down border


                // And now we draw teh hexagon if it is in the view ----
                window.draw(chunk_storage[std::to_string(cx)+";"+std::to_string(cy)].c[x+32][y+32].s);
            }
          }
        }
      }
    }

    // -- And we display
    window.display();
  }


  return 0;
}

int abs(int x) { if (x > 0) { return x; } else { return -x; }}
