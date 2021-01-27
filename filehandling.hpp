#pragma once
#include <string>
#include <unordered_map>
#include "chunkmap.hpp"
#include "chunk.hpp"
#include "entity.hpp"

void save(ChunkMap cmap, std::unordered_map<std::string, Entity> entities, std::string savename);
ChunkMap load(std::string savename);
