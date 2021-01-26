#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "entity.hpp"


sf::Vector2i handleLeftClick(std::unordered_map<long int, Entity> &entities, std::unordered_map<long int, Entity> &preview, sf::Vector2i mouseHexPos);
