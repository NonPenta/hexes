#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include "coordinates.hpp"

std::array<Coord, 6> dirs = {Coord(1, -1), Coord(1, 0), Coord(0, 1),
                             Coord(-1, 1), Coord(-1, 0), Coord(0, -1)};

Coord::Coord(): v{} {}

Coord::Coord(int x_, int y_): v{x_, y_} {}

Coord Coord::neighbor(int dir) { return *this+dirs[dir]; }

int Coord::abs() {
  return std::sqrt(x*x+y*y);
}

Coord operator+(Coord a, Coord b)   { return Coord(a.x+b.x, a.y+b.y); }
Coord operator+(Coord a, sf::Vector2i b)   { return Coord(a.x+b.x, a.y+b.y); }
Coord operator-(Coord a, Coord b)   { return Coord(a.x-b.x, a.y-b.y); }
Coord operator-(Coord a, sf::Vector2i b)   { return Coord(a.x-b.x, a.y-b.y); }
Coord operator*(Coord a, int n)     { return Coord(a.x*n, a.y*n); }
Coord operator*(int n, Coord a)     { return a*n; }
Coord operator/(Coord a, int n)     { return Coord(a.x/n, a.y/n); }
bool operator==(Coord a, Coord b)   { return a.x == b.x && a.y == b.y ; }
bool operator==(Coord a, sf::Vector2i b)   { return a.x == b.x && a.y == b.y ; }
bool operator!=(Coord a, Coord b)   { return !(a==b); }
bool operator!=(Coord a, sf::Vector2i b)   { return !(a==b); }
