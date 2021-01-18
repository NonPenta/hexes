#include <cmath>
#include "hex.hpp"

Hex::Hex() {}
Hex::Hex(int x_, int y_): type{"n"}, p{x_, y_}, size{10}, s{size-1, 6} {
  s.setOutlineThickness(1);
  s.setFillColor(sf::Color(32, 31, 35));
  s.setOutlineColor(sf::Color(48,47,51));
  s.setPosition(size*std::sqrt(3.0)*x_ + size*std::sqrt(3.0)/2.*y_, size*3.*y_/2.);
  s.setOrigin(9.7, 12);
}

Hex::Hex(int x_, int y_, std::string type_): type{type_}, p{x_, y_}, size{10}, s{size-1, 6} {

  s.setOutlineThickness(1);

  if (type_ != "p") {
    if (type_ != "w") {
      if (type_ != "w_"){
        s.setFillColor(sf::Color(32, 31, 35)); // DEFAULT CASE
      } else {s.setFillColor(sf::Color(32, 31, 35));} // IF 3 RESOLUTION
    } else {s.setFillColor(sf::Color(127,127,255));} // IF 2 RESOLUTION
  } else {s.setFillColor(sf::Color(0,255,0));} // IF 1 RESOLUTION

  s.setOutlineColor(sf::Color(48,47,51));
  s.setPosition(size*std::sqrt(3.0)*x_ + size*std::sqrt(3.0)/2.*y_, size*3.*y_/2.);
  s.setOrigin(9.7, 12);
}

Coord Hex::neighbor(int dir) {
  return p.neighbor(dir);
}

int Hex::distance(Hex a) {
  Coord c = p - a.p;
  return c.abs();
}

std::string Hex::getType() const {
  return type;
}

bool Hex::isInView(sf::Vector2f viewPos, sf::Vector2f viewSizeDelta) const {
  sf::Vector2f shapePos = s.getPosition();
  return (shapePos.x > viewPos.x-viewSizeDelta.x-10)&&(shapePos.x<viewPos.x+viewSizeDelta.x+9.7)&&(shapePos.y > viewPos.y - viewSizeDelta.y - 4)&&(shapePos.y < viewPos.y + viewSizeDelta.y + 10);
  // tis very long so : first cond within right border, second left, third up and fourth down
}

void Hex::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(s, states);
}
