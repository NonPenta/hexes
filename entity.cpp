#include <ctime>
#include <iostream>
#include "entity.hpp"

// --- CONSTRUCTORS ---
Entity::Entity() {}
Entity::Entity(int x, int y) : name{(long int) std::time(nullptr)}, pos{x, y}, type{"unknown"}, size{1}, selected{0}, members{}, viewPos{}, viewSizeDelta{} { members[std::pair<int,int>{x,y}] = Hex(x, y, type); }
Entity::Entity(int x, int y, std::string type_) : name{(long int) std::time(nullptr)}, pos{x, y}, type{type_}, size{1}, selected{0}, members{}, viewPos{}, viewSizeDelta{} { members[std::pair<int,int>{x,y}] = Hex(x, y, type_); }
Entity::Entity(int x, int y, std::string type_, int size_) : name{(long int) std::time(nullptr)}, pos{x, y}, type{type_}, size{size_}, selected{0}, members{}, viewPos{}, viewSizeDelta{} {
  for (int xi = x; xi < x+size; xi++) {
    for (int yi = y; yi < y+size; yi++) {
      members[std::pair<int,int>{xi, yi}] = Hex(xi, yi, type_);
    }
  }
}
Entity::Entity(long int name, int x, int y) : name{name}, pos{x, y}, type{"unknown"}, size{1}, selected{0}, members{}, viewPos{}, viewSizeDelta{} { members[std::pair<int,int>{x,y}] = Hex(x, y, type); }
Entity::Entity(long int name, int x, int y, std::string type_) : name{name}, pos{x, y}, type{type_}, size{1}, selected{0}, members{}, viewPos{}, viewSizeDelta{} { members[std::pair<int,int>{x,y}] = Hex(x, y, type_); }
Entity::Entity(long int name, int x, int y, std::string type_, int size_) : name{name}, pos{x, y}, type{type_}, size{size_}, selected{0}, members{}, viewPos{}, viewSizeDelta{} {
  for (int xi = x; xi < x+size; xi++) {
    for (int yi = y; yi < y+size; yi++) {
      members[std::pair<int,int>{xi, yi}] = Hex(xi, yi, type_);
    }
  }
}

// --- PUBLIC METHODS ---

long int Entity::getName() { return name; }
std::string Entity::getType() { return type; }


void Entity::select() { selected = true; }
void Entity::unSelect() { selected = false; }
bool Entity::isSelected() { return selected; }

void Entity::moveTo(int x, int y) { pos = Coord{x, y}; updatePosition(); }
void Entity::moveBy(int x, int y) { pos = Coord{pos.x+x, pos.y+y}; updatePosition(); }
void Entity::updatePosition() {
  members.clear();
  for (int xi = pos.x; xi < pos.x+size; xi++) {
    for (int yi = pos.y; yi < pos.y+size; yi++) {
      members[std::pair<int,int>{xi, yi}] = Hex(xi, yi, type);
    }
  }
}
Coord Entity::getPos() { return pos; }

Entity Entity::getPreviewAt(int x, int y) { return Entity(name, x, y, "preview"+type, size); }
Entity Entity::getEntityFromPreview() { return Entity(name, pos.x, pos.y, type.substr(7), size); }
bool Entity::hoveredByPos(sf::Vector2i pos_) { return (pos_.x>=pos.x)&&(pos_.x<pos.x+size)&&(pos_.y>=pos.y)&&(pos_.y<pos.y+size); }
void Entity::setView(sf::View& view) {
  viewPos = view.getCenter();
  viewSizeDelta = sf::Vector2f{view.getSize().x/2, view.getSize().y/2};
}

// --- PRIVATE METHODS ---

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (const auto &hexPair : members) {
    if (hexPair.second.isInView(viewPos, viewSizeDelta)) {
      target.draw(hexPair.second, states);
    }
  }
}
