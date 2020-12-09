#pragma once
#include <string>
#include "chunkmap.hpp"

void save(ChunkMap cmap, std::string savename);
ChunkMap load(std::string fname);
