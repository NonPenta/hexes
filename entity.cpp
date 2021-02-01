#include "entity.hpp"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <ctime>
#include <iostream>

// --- CONSTRUCTORS ---
Entity::Entity() {}
Entity::Entity(int x, int y)
    : name{boost::uuids::to_string(boost::uuids::random_generator()())},
      pos{x, y}, type{"unknown"}, width{1}, height{1}, selected{0}, members{},
      viewPos{}, viewSizeDelta{} {
  members[std::pair<int, int>{x, y}] = Hex(x, y, type);
}
Entity::Entity(int x, int y, std::string type_)
    : name{boost::uuids::to_string(boost::uuids::random_generator()())},
      pos{x, y}, type{type_}, width{1}, height{1}, selected{0}, members{},
      viewPos{}, viewSizeDelta{} {
  members[std::pair<int, int>{x, y}] = Hex(x, y, type_);
}
Entity::Entity(int x, int y, std::string type_, int width_, int height_)
    : name{boost::uuids::to_string(boost::uuids::random_generator()())},
      pos{x, y}, type{type_}, width{width_}, height{height_}, selected{0},
      members{}, viewPos{}, viewSizeDelta{} {
  for (int xi = pos.x + width * (width < 0);
       xi < pos.x + (width < 0) + width * (width > 0); xi++) {
    for (int yi = pos.y + height * (height < 0);
         yi < pos.y + (height < 0) + height * (height > 0); yi++) {
      members[std::pair<int, int>{xi, yi}] = Hex(xi, yi, type_);
    }
  }
}

Entity::Entity(std::string name, int x, int y)
    : name{name}, pos{x, y}, type{"unknown"}, width{1}, height{1}, selected{0},
      members{}, viewPos{}, viewSizeDelta{} {
  members[std::pair<int, int>{x, y}] = Hex(x, y, type);
}
Entity::Entity(std::string name, int x, int y, std::string type_)
    : name{name}, pos{x, y}, type{type_}, width{1}, height{1}, selected{0},
      members{}, viewPos{}, viewSizeDelta{} {
  members[std::pair<int, int>{x, y}] = Hex(x, y, type_);
}
Entity::Entity(std::string name_, int x, int y, std::string type_, int width_,
               int height_)
    : name{name_}, pos{x, y}, type{type_}, width{width_}, height{height_},
      selected{0}, members{}, viewPos{}, viewSizeDelta{} {
  for (int xi = pos.x + width * (width < 0);
       xi < pos.x + (width < 0) + width * (width > 0); xi++) {
    for (int yi = pos.y + height * (height < 0);
         yi < pos.y + (height < 0) + height * (height > 0); yi++) {
      members[std::pair<int, int>{xi, yi}] = Hex(xi, yi, type_);
    }
  }
}

// --- PUBLIC METHODS ---

std::string Entity::getName() const { return name; }
std::string Entity::getType() const { return type; }
void Entity::setType(std::string type_) {
  type = type_;
  members.clear();
  for (int xi = pos.x + width * (width < 0);
       xi < pos.x + (width < 0) + width * (width > 0); xi++) {
    for (int yi = pos.y + height * (height < 0);
         yi < pos.y + (height < 0) + height * (height > 0); yi++) {
      members[std::pair<int, int>{xi, yi}] = Hex(xi, yi, type);
    }
  }
}
int Entity::getWidth() const { return width; }
int Entity::getHeight() const { return height; }
int Entity::getMemberSize() const { return members.size(); }

void Entity::select() { selected = true; }
void Entity::unSelect() { selected = false; }
bool Entity::isSelected() const { return selected; }

void Entity::moveTo(int x, int y) {
  pos = sf::Vector2i{x, y};
  updatePosition();
}
void Entity::moveTo(sf::Vector2i p) {
  pos = sf::Vector2i{p.x, p.y};
  updatePosition();
}
void Entity::moveBy(int x, int y) {
  pos = sf::Vector2i{pos.x + x, pos.y + y};
  updatePosition();
}
void Entity::updatePosition() {
  members.clear();
  for (int xi = pos.x + width * (width < 0);
       xi < pos.x + (width < 0) + width * (width > 0); xi++) {
    for (int yi = pos.y + height * (height < 0);
         yi < pos.y + (height < 0) + height * (height > 0); yi++) {
      members[std::pair<int, int>{xi, yi}] = Hex(xi, yi, type);
    }
  }
}
sf::Vector2i Entity::getPos() const { return sf::Vector2i(pos.x, pos.y); }

Entity Entity::getPreviewAt(int x, int y) const {
  return Entity(name, x, y, "preview" + type, width, height);
}
Entity Entity::getPreviewAt(sf::Vector2i p) const {
  return Entity(name, p.x, p.y, "preview" + type, width, height);
}
Entity Entity::getEntityFromPreview() const {
  return Entity(name, pos.x, pos.y, type.substr(7), width, height);
}
bool Entity::hoveredByPos(sf::Vector2i pos_) const {
  return (pos_.x >= pos.x) && (pos_.x < pos.x + width) && (pos_.y >= pos.y) &&
         (pos_.y < pos.y + height);
}
void Entity::setView(sf::View &view) {
  viewPos = view.getCenter();
  viewSizeDelta = sf::Vector2f{view.getSize().x / 2, view.getSize().y / 2};
}

// --- PRIVATE METHODS ---

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (const auto &hexPair : members) {
    if (hexPair.second.isInView(viewPos, viewSizeDelta)) {
      target.draw(hexPair.second, states);
    }
  }
}
