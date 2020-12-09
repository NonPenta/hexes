#include <filesystem>
#include <fstream>
#include <iostream>
#include <version>
#include "filehandling.hpp"

void save(ChunkMap cmap, std::string savename) {
  std::filesystem::path savespath = "~/.hge:shitty";
  bool check_saves_dir = std::filesystem::create_directory(savespath);
  if (check_saves_dir) {
    std::cout << "I am a master of computers" << '\n';
  } else {
    std::cout << "get rekt noob" << '\n';
  }
  std::filesystem::path savepath = savespath;
  savepath /= savename;
  bool check_thissave_dir = std::filesystem::create_directory(savespath); // unused yet
  for (const auto &element : cmap.getMap()) { // element unused
    // Create file with name element.first.first:element.first.second
    // fill file, csv? boost parser?
  }
}
