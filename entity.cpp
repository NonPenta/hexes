#include "entity.hpp"
// --- CONSTRUCTORS ---
Entity::Entity() {}
Entity::Entity(int x, int y) : pos{x, y}, type{"unknown"}, size{1}, selected{0}, members{}, viewPos{}, viewSizeDelta{} { members[std::pair<int,int>{x,y}] = Hex(x, y, type); }
Entity::Entity(int x, int y, std::string type_) : pos{x, y}, type{type_}, size{1}, selected{0}, members{}, viewPos{}, viewSizeDelta{} { members[std::pair<int,int>{x,y}] = Hex(x, y, type_); }
Entity::Entity(int x, int y, std::string type_, int size_) : pos{x, y}, type{type_}, size{size_}, selected{0}, members{}, viewPos{}, viewSizeDelta{} {
  for (int xi = x; xi < x+size_; xi++) {
    for (int yi = y; yi < y+size; yi++) {
      members[std::pair<int,int>{xi, yi}] = Hex(xi, yi, type_);
    }
  }
}

// --- PUBLIC METHODS ---
void Entity::select() { selected = true; }
void Entity::unSelect() { selected = false; }
bool Entity::isSelected() { return selected; }

void Entity::moveTo(int x, int y) { pos = Coord{x, y}; }
void Entity::moveBy(int x, int y) { pos = Coord{pos.x+x, pos.y+y}; }

Entity Entity::getPreviewAt(int x, int y) { return Entity(x, y, "preview"+type, size); }

void Entity::setView(sf::View& view) {
  viewPos = view.getCenter();
  viewSizeDelta = sf::Vector2f{view.getSize().x/2, view.getSize().y/2};
}

// --- PRIVATE SHIT? IDK IS IT A METHOD ? IS IT NOT ? ---
void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (const auto &hexPair : members) {
    if (hexPair.second.isInView(viewPos, viewSizeDelta)) {
      target.draw(hexPair.second, states);
    }
  }
}
