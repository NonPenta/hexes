#pragma once
#include <SFML/Graphics.hpp>
#include "coordinates.hpp"


struct Hex : public sf::Drawable {
public:
    Hex();
    Hex(int x_, int y_);
    Hex(int x_, int y_, float size_);

    Coord neighbor(int dir);

    int distance(Hex a);

    bool isInView(sf::Vector2f viewPos, sf::Vector2f viewSizeDelta) const;

private:
  Coord p;
  float size;
  sf::CircleShape s;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
