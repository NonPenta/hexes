#include <iostream>
#include "chunk.hpp"

Chunk::Chunk(): p{} {}
Chunk::Chunk(int x_, int y_): p{x_, y_} {
  for (int hx = -32; hx <= 32; hx++) {
    for (int hy = -32; hy <= 32; hy++) {
      c[hx+32][hy+32] = Hex{hx+65*x, hy+65*y};
    }
  }
}
