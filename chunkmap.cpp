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

void ChunkMap::setView(sf::View& view) { // Iteration to change
  for (auto &element : map) {
    element.second.setView(view);
  }
}

void ChunkMap::draw(sf::RenderTarget& target, sf::RenderStates states) const { // Iteration to change
  for (const auto &element : map) {
    target.draw(element.second, states);
  }
}

std::unordered_map<std::pair<int,int>, Chunk, boost::hash<std::pair<int,int>>> ChunkMap::getMap() {
  std::unordered_map<std::pair<int,int>, Chunk, boost::hash<std::pair<int,int>>> map_out;
  for (const auto &element : map) {
      map_out[element.first] = element.second;
  }
  return map_out;
}
