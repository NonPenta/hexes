#pragma once
#include <SFML/Graphics.hpp>
#include "chunkmap.hpp"
#include "entity.hpp"

struct Context {
  ChunkMap &map;
  std::unordered_map<std::string, Entity> &entities;
  sf::RenderWindow &window;
  sf::View view;
};
