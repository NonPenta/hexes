#pragma once
#include "chunk.hpp"
#include "chunkmap.hpp"
#include "entity.hpp"
#include <string>
#include <unordered_map>

void save(ChunkMap cmap, std::unordered_map<std::string, Entity> entities,
          std::string savename);
std::pair<ChunkMap, std::unordered_map<std::string, Entity>>
load(std::string savename);
