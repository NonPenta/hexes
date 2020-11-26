#pragma once
#include <array>
#include "hex.hpp"

struct Chunk {
  std::array<std::array<Hex, 65>, 65> c;
  union {
    int p[2];
    struct {
      int x, y;
    };
  };

  Chunk();
  Chunk(int x_, int y_);

};
