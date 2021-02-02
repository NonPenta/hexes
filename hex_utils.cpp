#include "hex_utils.hpp"
#include <cmath>
#include <unordered_map>

int abs(int x) {
  if (x > 0) {
    return x;
  } else {
    return -x;
  }
}
int abs(float x) {
  if (x > 0) {
    return x;
  } else {
    return -x;
  }
}
int abs(sf::Vector2i v) { return (abs(v.x) + abs(v.y) + abs(v.x + v.y)) / 2; }

sf::Vector2i hex_round(sf::Vector2f f_pos) {
  int x = std::round(f_pos.x);
  int y = std::round(f_pos.y);
  int z = -x - y;

  float dx = abs(x - f_pos.x);
  float dy = abs(y - f_pos.y);
  float dz = abs(z + f_pos.x + f_pos.y);

  if (dx > dy && dx > dz) {
    x = -y - z;
  } else if (dy > dz) {
    y = -x - z;
  }

  return sf::Vector2i(x, y);
}

sf::Vector2i hex_from_pix(sf::Vector2f pos) {
  float hex_x = (std::sqrt(3.) / 3 * pos.x - 1 / 3. * pos.y) / 10.;
  float hex_y = 2 / 3. * pos.y / 10.;

  return hex_round(sf::Vector2f(hex_x, hex_y));
}

sf::Vector2i chunk_from_hex(sf::Vector2i hex_pos) {
  int x = (hex_pos.x + 32) / 65 - (hex_pos.x + 32 < 0);
  int y = (hex_pos.y + 32) / 65 -
          (hex_pos.y + 32 < 0); // cpp's division is fucking bad. it's trash.
                                // (not euclidian division)

  return sf::Vector2i(x, y);
}

sf::Vector2i hex_within_chunk(sf::Vector2i hex_pos) {
  sf::Vector2i chunk_pos = chunk_from_hex(hex_pos);
  int x = hex_pos.x - chunk_pos.x * 65 + 32;
  int y = hex_pos.y - chunk_pos.y * 65 + 32;

  return sf::Vector2i(x, y);
}

sf::Vector2i hexNeighbor(int dir) {}

namespace sf {
template <typename T>
bool operator<(const sf::Vector2<T> &left, const sf::Vector2<T> &right) {
  return (left.x < right.x) || ((left.x == right.x) && (left.y < right.y));
}
} // namespace sf
