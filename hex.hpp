#pragma once
#include <SFML/Graphics.hpp>
#include <string>


class Hex : public sf::Drawable {
  public:
    Hex();
    Hex(int x_, int y_);
    Hex(int x_, int y_, std::string type_);

    sf::Vector2i neighbor(int dir);

    int distance(Hex a);
    void moveTo(int x, int y);
    void updatePosition();

    bool isInView(sf::Vector2f viewPos, sf::Vector2f viewSizeDelta) const;

    std::string getType() const;

private:
  std::string type;
  sf::Vector2i p;
  float size;
  sf::CircleShape s;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
