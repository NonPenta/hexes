#include "chunkmap.hpp"
#include <iostream>

ChunkMap::ChunkMap() : map{}, pos0{}, dpos{} {}

ChunkMap::ChunkMap(int x0, int y0, int dx, int dy)
    : map{}, pos0{x0, y0}, dpos{dx, dy} {
  for (int x = x0; x < x0 + dx; x++) {
    for (int y = y0; y < y0 + dy; y++) {
      map[std::pair<int, int>{x, y}] = Chunk{x, y};
    }
  }
}

void setHexType(sf::Vector2i p, std::string type) {
  map[std::pair<int, int>{chunk_from_hex(p).x, chunk_from_hex(p).y}].setHexType(
      p.x, p.y, type);
}

void ChunkMap::setView(sf::View &view) {
  for (auto &element : map) {
    element.second.setView(view);
  }
}

void ChunkMap::setChunk(std::pair<int, int> cPos, Chunk chunk) {
  map[cPos] = chunk;
}

void ChunkMap::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (const auto &element : map) {
    target.draw(element.second, states);
  }
}

std::unordered_map<std::pair<int, int>, Chunk, boost::hash<std::pair<int, int>>>
ChunkMap::getMap() {
  std::unordered_map<std::pair<int, int>, Chunk,
                     boost::hash<std::pair<int, int>>>
      map_out = map;
  return map_out;
}
