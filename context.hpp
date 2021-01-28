#pragma once
struct Context {
  ChunkMap &map;
  std::unordered_map<std::string, Entity> &entities;
  sf::RenderWindow &window;
};
