#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include <iostream>
#include "filehandling.hpp"

void save(ChunkMap cmap, std::string savename) {

  fs::path savepath = ".hge:saves";
  savepath /= savename;
  bool save_exists = fs::exists(savepath);

  if (save_exists) {
    fs::path old_path = savepath;
    old_path += fs::path{"_old"};
    fs::rename(savepath, old_path);
  }
  fs::create_directories(savepath);

  for (const auto &chunk_pair : cmap.getMap()) {
    std::string lastType = "no.";

    // create file with name : "chunk_x:chunk_y"
    std::fstream chunkfile;
    chunkfile.open(savepath/(std::to_string(chunk_pair.first.first)+":"+std::to_string(chunk_pair.first.second)), std::fstream::app);


    for (int y = -32; y <= 32; y++) { // Looping through chunk's hex coordinates
      for (int x = -32; x <= 32; x++) {
        // Add hex info to file.
        // if the type didnt change, just add / ; else the new type
        if (chunk_pair.second.getc()[x+32][y+32].getType() == lastType) {
          chunkfile << "/;";
        } else {
          lastType = chunk_pair.second.getc()[x+32][y+32].getType();
          chunkfile << lastType + ";";
        }
      }
    }
    chunkfile.close();
  }
  if (save_exists) {
    fs::path old_path = savepath;
    old_path += fs::path{"_old"};
    fs::remove_all(old_path);
  }
}

ChunkMap load(std::string savename) {

  std::cout << "started loading" << '\n';

  ChunkMap map = ChunkMap();

  fs::path savepath = ".hge:saves";
  savepath /= savename;

  std::cout << "savepath done" << '\n';
  for (const auto& cfile : fs::directory_iterator(savepath)) {
    const auto filenameStr = cfile.path().filename().string();
    std::cout << "currently at file : " << filenameStr << '\n';

    // ---- GETTING CHUNK POSITION ----
    std::vector<int> chunkPos;
    std::string tok;

      // --- PARSING FROM FILENAME ----
    for (int i = 0; i < (int)filenameStr.length(); i++) {
      char ch = filenameStr[i];
      if (ch == ';') {
        chunkPos.push_back(std::stoi(tok));
        tok = "";
      } else {tok += ch;}
    }

      // --- CONVERTING FROM VECTOR TO PAIR
    std::pair<int,int> chunk_pos;
    chunk_pos.first = chunkPos[0];
    chunk_pos.second = chunkPos[1];

    // --- CREATING CHUNK ---
    Chunk c = Chunk{chunk_pos.first, chunk_pos.second};
    // --- CREATING FILE STREAM AND LINE
    std::fstream chunkfile(cfile.path());
    std::string line;

    // --- TREATING FILE ---
    while (std::getline(chunkfile, line)) {

      // --- PARSING SETUP ---
      std::string prevTok;
      std::string token;
      int n = 0;

      // ---- PARSING ----
      for (int i = 0; i < (int)line.length(); i++) {
        char ch=line[i];
        if (ch == ';') { // SEPARATOR

          c.setHexType(n%65-32,n/65-32, token);

          prevTok = token;
          token = "";
          n++;

        } else if (ch == '/') { // SAME CONTENT
            token = prevTok;
        } else {token += ch;} // ADD TO CONTENT
      }
    }
    // --- PUT CHUNK CONTENT IN MAP ---
    map.setChunk(chunk_pos, c);

  }

  return map;
}
