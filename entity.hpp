#pragma once
#include "hex.hpp"
#include <SFML/Graphics.hpp>
#include <boost/container_hash/hash.hpp>
#include <string>
#include <unordered_map>

class Entity : public sf::Drawable {
private:
  std::string name;
  sf::Vector2i pos;
  std::string type;
  int width;
  int height;
  bool selected;
  std::unordered_map<std::pair<int, int>, Hex, boost::hash<std::pair<int, int>>>
      members;

  sf::Vector2f viewPos;
  sf::Vector2f viewSizeDelta;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
  Entity();
  Entity(int x, int y);
  Entity(int x, int y, std::string type_);
  Entity(int x, int y, std::string type_, int width_, int height_);

  Entity(std::string name, int x, int y);
  Entity(std::string name, int x, int y, std::string type_);
  Entity(std::string name, int x, int y, std::string type_, int width_,
         int height_);

  std::string getName() const;
  std::string getType() const;
  void setType(std::string type_);
  int getWidth() const;
  int getHeight() const;
  int getMemberSize() const;

  void select();
  void unSelect();
  bool isSelected() const;

  void moveTo(int x, int y);
  void moveTo(sf::Vector2i p);
  void moveBy(int x, int y);
  void updatePosition();
  sf::Vector2i getPos() const;

  Entity getPreviewAt(int x, int y) const;
  Entity getPreviewAt(sf::Vector2i p) const;
  Entity getEntityFromPreview() const;
  bool hoveredByPos(sf::Vector2i pos) const;
  void setView(sf::View &view);
};
