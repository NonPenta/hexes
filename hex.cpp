#include <cmath>
#include "hex.hpp"

Hex::Hex() {}
Hex::Hex(int x_, int y_): p{x_, y_}, size{10}, s{size-1, 6} {

  s.setOutlineThickness(1);

  if ((x_%65==0) || (y_%65==0)) {
    s.setFillColor(sf::Color(255, 0, 0));
  } else {
    s.setFillColor(sf::Color(32, 31, 35));
  }

  s.setOutlineColor(sf::Color(48,47,51));
  s.setPosition(size*std::sqrt(3.0)*x_ + size*std::sqrt(3.0)/2.*y_, size*3.*y_/2.);
  s.setOrigin(9.7, 12);
}

Hex::Hex(int x_, int y_, float size_): p(x_, y_), size(size_), s(size-1, 6) {

  s.setOutlineThickness(1);

  if ((x_%65==0) || (y_%65==0)) {
    s.setFillColor(sf::Color(255, 0, 0));
  } else {
    s.setFillColor(sf::Color(32, 31, 35));
  }

  s.setOutlineColor(sf::Color(48,47,51));
  s.setPosition(size*std::sqrt(3.0)*x_ + size*std::sqrt(3.0)/2*y_, size*3*y_/2);
}

Coord Hex::neighbor(int dir) {
  return p.neighbor(dir);
}

int Hex::distance(Hex a) {
  Coord c = p - a.p;
  return c.abs();
}

bool Hex::isInView(sf::Vector2f viewPos, sf::Vector2f viewSizeDelta) const {
  sf::Vector2f shapePos = s.getPosition();
  return (shapePos.x > viewPos.x-viewSizeDelta.x-10)&&(shapePos.x<viewPos.x+viewSizeDelta.x+9.7)&&(shapePos.y > viewPos.y - viewSizeDelta.y - 4)&&(shapePos.y < viewPos.y + viewSizeDelta.y + 10);
  // tis very long so : first cond within right border, second left, third up and fourth down
}

void Hex::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(s, states);
}
