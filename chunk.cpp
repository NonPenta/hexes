#include <iostream>
#include "chunk.hpp"

Chunk::Chunk(): p{}, viewPos{}, viewSizeDelta{} {}
Chunk::Chunk(int x_, int y_): p{x_, y_}, viewPos{}, viewSizeDelta{} {
  for (int hx = -32; hx <= 32; hx++) {
    for (int hy = -32; hy <= 32; hy++) {
      c[hx+32][hy+32] = Hex{hx+65*x, hy+65*y};
    }
  }
}

void Chunk::setView(sf::View& view) {
  viewPos = view.getCenter();
  viewSizeDelta = sf::Vector2f{view.getSize().x/2, view.getSize().y/2};
}

std::array<std::array<Hex, 65>, 65> Chunk::getc() const {
  return c;
}

void Chunk::setHexType(int hexX, int hexY, std::string type) {
  c[hexX+32][hexY+32] = Hex{x*65+hexX,y*65+hexY,type};
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (int x = -32; x <= 32; x++) {                         // Looping through chunk's hex coordinates
    for (int y = -32; y <= 32; y++) {

      // --- IF THE HEXAGON IS VISIBLE ---
      if (c.at(x+32)[y+32].isInView(viewPos, viewSizeDelta)) // viewPos and viewSizeDelta to pass as const
        target.draw(c.at(x+32)[y+32], states);
      }
    }

}
