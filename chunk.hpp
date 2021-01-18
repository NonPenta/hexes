#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include "hex.hpp"

struct Chunk : public sf::Drawable {
  public:
    Chunk();
    Chunk(int x_, int y_);

    void setView(sf::View& view);
    std::array<std::array<Hex, 65>, 65> getc() const;

    void setHexType(int hexX, int hexY, std::string type);

  private:
    std::array<std::array<Hex, 65>, 65> c;
    union {
      int p[2];
      struct {
        int x, y;
      };
    };

    sf::Vector2f viewPos;
    sf::Vector2f viewSizeDelta;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
