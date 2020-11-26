#include "chunkmap.hpp"
#include <iostream>

ChunkMap::ChunkMap() : map{}, pos0{}, dpos{} {}

ChunkMap::ChunkMap(int x0, int y0, int dx, int dy) : map{}, pos0{x0, y0}, dpos{dx, dy} {
  for (int x = x0; x < x0+dx; x++) {
    for (int y = y0; y < y0+dy; y++) {
      map[std::pair<int,int>{x,y}] = Chunk{};
    }
  }
}

void ChunkMap::drawOn(sf::RenderWindow &window, sf::View &view) {
  // ---- getting view position -------
  float view_x = view.getCenter().x;
  float view_y = view.getCenter().y;

  // ---- getting view size ---------------
  float delta_view_width = view.getSize().x/2;
  float delta_view_height = view.getSize().y/2;   // Divided by 2 because then we can get border from center

  // ----------------- DRAWING ------------------------
  for (int cx = pos0.x; cx < pos0.x+dpos.x; cx++) {             // Looping through chunk map's chunk coordinates
    for (int cy = pos0.y; cy < pos0.y+dpos.y; cy++) {

      // -- FOR EACH CHUNK --
      std::pair<int, int> pos{cx,cy};

      for (int x = -32; x <= 32; x++) {                         // Looping through chunk's hex coordinates
        for (int y = -32; y <= 32; y++) {

          // -- FOR EACH HEX --

          float shape_x = map[pos].c[x+32][y+32].s.getPosition().x; // Get hexagon x
          float shape_y = map[pos].c[x+32][y+32].s.getPosition().y; // Get hexagon y

          // --- IF THE HEXAGON IS VISIBLE ---
          if ((-10 + view_x - delta_view_width < shape_x)     // If the hexagon isnt out the left border
          && (shape_x < view_x + delta_view_width + 9.7)      // If the hexagon isnt out the right border
          && (-4 + view_y - delta_view_height < shape_y)      // If the hexagon isnt out the up border
          && (shape_y < view_y + delta_view_height + 10)) {   // If the hexagon isnt out the down border

            window.draw(map[pos].c[x+32][y+32].s);
          }
        }
      }
    }
  }
}
