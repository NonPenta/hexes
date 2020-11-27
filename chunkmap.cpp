#include "chunkmap.hpp"
#include <iostream>

ChunkMap::ChunkMap() : map{}, pos0{}, dpos{} {}

ChunkMap::ChunkMap(int x0, int y0, int dx, int dy) : map{}, pos0{x0, y0}, dpos{dx, dy} {
  for (int x = x0; x < x0+dx; x++) {
    for (int y = y0; y < y0+dy; y++) {
      map[std::pair<int,int>{x,y}] = Chunk{x,y};
    }
  }
}

void ChunkMap::setView(sf::View& view) {
  for (int cx = pos0.x; cx < pos0.x+dpos.x; cx++) {             // Looping through chunk map's chunk coordinates
    for (int cy = pos0.y; cy < pos0.y+dpos.y; cy++) {
      std::pair<int, int> pos{cx,cy};
      map[pos].setView(view);
    }
  }
}

void ChunkMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (int cx = pos0.x; cx < pos0.x+dpos.x; cx++) {
    for (int cy = pos0.y; cy < pos0.y+dpos.y; cy++) {
      target.draw(map.at(std::pair<int, int>{cx,cy}), states);
    }
  }
}
