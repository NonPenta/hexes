#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "entity.hpp"


sf::Vector2i handleLeftClick(std::unordered_map<std::string, Entity> &entities,
                            std::unordered_map<std::string, Entity> &preview,
                            sf::Vector2i mouseHexPos);
