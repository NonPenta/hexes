#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <boost/container_hash/hash.hpp>
#include "chunk.hpp"

struct ChunkMap {
  std::unordered_map<std::pair<int,int>, Chunk, boost::hash<std::pair<int,int>>> map;
  sf::Vector2i pos0;
  sf::Vector2i dpos;

  ChunkMap();
  ChunkMap(int x0, int y0, int dx, int dy);

  void drawOn(sf::RenderWindow &window, sf::View &view);
};
