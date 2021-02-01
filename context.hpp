#pragma once
#include "chunkmap.hpp"
#include "entity.hpp"
#include <SFML/Graphics.hpp>

struct Context {
  ChunkMap &map;
  std::unordered_map<std::string, Entity> &entities;
  sf::RenderWindow &window;
  sf::View view;
};
