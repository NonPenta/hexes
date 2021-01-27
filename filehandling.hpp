#pragma once
#include <string>
#include <unordered_map>
#include "chunkmap.hpp"
#include "chunk.hpp"
#include "entity.hpp"

void save(ChunkMap cmap, std::unordered_map<std::string, Entity> entities, std::string savename);
std::pair<ChunkMap, std::unordered_map<std::string, Entity>> load(std::string savename);
