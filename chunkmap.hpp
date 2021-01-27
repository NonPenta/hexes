#pragma once
#include <boost/container_hash/hash.hpp>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "chunk.hpp"

class ChunkMap : public sf::Drawable {
public:
  ChunkMap();
  ChunkMap(int x0, int y0, int dx, int dy);

  void setView(sf::View& view);
  void setChunk(std::pair<int,int> cPos, Chunk chunk);

  std::unordered_map<std::pair<int,int>, Chunk, boost::hash<std::pair<int,int>>> getMap();

private:
  std::unordered_map<std::pair<int,int>, Chunk, boost::hash<std::pair<int,int>>> map;
  sf::Vector2i pos0;
  sf::Vector2i dpos;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
