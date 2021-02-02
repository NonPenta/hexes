#pragma once
#include <SFML/Graphics.hpp>

int abs(int x);
int abs(float x);
int abs(sf::Vector2i v);

sf::Vector2i hex_round(sf::Vector2f f_pos);
sf::Vector2i hex_from_pix(sf::Vector2f pos);
sf::Vector2i chunk_from_hex(sf::Vector2i hex_pos);
sf::Vector2i hex_within_chunk(sf::Vector2i hex_pos);

sf::Vector2i hexNeighbor(int dir);

namespace sf {
template <typename T>
bool operator<(const sf::Vector2<T> &left, const sf::Vector2<T> &right);
} // namespace sf
