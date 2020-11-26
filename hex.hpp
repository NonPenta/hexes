#pragma once
#include <SFML/Graphics.hpp>
#include "coordinates.hpp"


struct Hex {
  Coord p;
  float size;
  sf::CircleShape s;

  Hex();
  Hex(int x_, int y_);
  Hex(int x_, int y_, float size_);

  Coord neighbor(int dir);

  int distance(Hex a);
};
