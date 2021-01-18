#pragma once
#include <string>
#include "chunkmap.hpp"
#include "chunk.hpp"

void save(ChunkMap cmap, std::string savename);
ChunkMap load(std::string savename);
