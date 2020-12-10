#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include <iostream>
#include <version>
#include "filehandling.hpp"

void save(ChunkMap cmap, std::string savename) {

  fs::path savepath = ".hge:saves";
  savepath /= savename;
  bool check_thissave_dir = fs::create_directories(savepath); // unused yet

  for (const auto &chunk_pair : cmap.getMap()) { // element unused
    // create file with name : "chunk_x:chunk_y"
    std::fstream chunkfile;
    chunkfile.open(savepath/(std::to_string(chunk_pair.first.first)+":"+std::to_string(chunk_pair.first.second)), std::fstream::app);

    for (int x = -32; x <= 32; x++) {                         // Looping through chunk's hex coordinates
      for (int y = -32; y <= 32; y++) {
        // Add hex info to file.
        chunkfile << std::to_string(x)+":"+std::to_string(y)+";";
      }
    }
    chunkfile.close();
  }
}
