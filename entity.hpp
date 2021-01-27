#pragma once
#include <boost/container_hash/hash.hpp>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <string>
#include "hex.hpp"
#include "coordinates.hpp"

class Entity : public sf::Drawable {
  private:
    std::string name;
    Coord pos;
    std::string type;
    int width;
    int height;
    bool selected;
    std::unordered_map<std::pair<int,int>, Hex, boost::hash<std::pair<int,int>>> members;

    sf::Vector2f viewPos;
    sf::Vector2f viewSizeDelta;


    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  public:
    Entity();
    Entity(int x, int y);
    Entity(int x, int y, std::string type_);
    Entity(int x, int y, std::string type_, int size_);
    Entity(int x, int y, std::string type_, int width_, int height_);

    Entity(std::string name, int x, int y);
    Entity(std::string name, int x, int y, std::string type_);
    Entity(std::string name, int x, int y, std::string type_, int size_);
    Entity(std::string name, int x, int y, std::string type_, int width_, int height_);

    std::string getName();
    std::string getType();
    int getWidth();
    int getHeight();

    void select();
    void unSelect();
    bool isSelected();

    void moveTo(int x, int y);
    void moveBy(int x, int y);
    void updatePosition();
    Coord getPos();

    Entity getPreviewAt(int x, int y);
    Entity getEntityFromPreview();
    bool hoveredByPos(sf::Vector2i pos);
    void setView(sf::View &view);
};
