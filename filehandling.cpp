#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include <iostream>
#include "filehandling.hpp"

void save(ChunkMap cmap, std::string savename) {

  fs::path savepath = ".hge:saves";
  savepath /= savename;
  bool check_thissave_dir = fs::create_directories(savepath); // unused yet


  for (const auto &chunk_pair : cmap.getMap()) {
    std::string lastType = "no.";
    std::cout << "one down, some more to go ?" << '\n';
    // create file with name : "chunk_x:chunk_y"
    std::fstream chunkfile;
    chunkfile.open(savepath/(std::to_string(chunk_pair.first.first)+":"+std::to_string(chunk_pair.first.second)), std::fstream::app);

    for (int y = -32; y <= 32; y++) {                         // Looping through chunk's hex coordinates
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

    std::cout << "created chunk position vector and token" << '\n';

    for (int i=0;i<filenameStr.length();i++) {
      std::cout << "index in filename : " << i << '\n';
      char ch = filenameStr[i];
      if (ch == ';') {
        chunkPos.push_back(std::stoi(tok));
        tok = "";
      } else {tok += ch;}
    }
      // converting the vector to a pair
    std::pair<int,int> chunk_pos;
    chunk_pos.first = chunkPos[0];
    chunk_pos.second = chunkPos[1];

    std::cout << "chunk pos done" << '\n';

    // putting the shit from the file in the chunkmap etc
    Chunk c = Chunk{chunk_pos.first, chunk_pos.second}; // in theory thats correct but not sure im stoopid

    std::cout << "chunk created with chunk pos" << '\n';

    std::fstream chunkfile(cfile.path());
    std::string line;

    std::cout << "fstream and line done" << '\n';

    while (std::getline(chunkfile, line)) {

      std::cout << "treating line" << '\n';

      std::string prevTok;
      std::string token;

      int n = 0;
      for (int i = 0; i<line.length(); i++) {
        std::cout << "parsing line. i = " << i << '\n';

        char ch=line[i];
        std::cout << "char converted at i = " << i << '\n';
        if (ch == ';') {
          std::cout << "separation between values, val " << n << '\n';
          c.setHexType(n%65-32,n/65-32, token);
          std::cout << "hex position : " << n/65 << " " << n%65 << '\n';
          prevTok = token;
          token = "";
          n++;

        } else if (ch == '/') {
            token = prevTok;

        } else {token += ch;}
      }
    }

    map.setChunk(chunk_pos, c);

  }

  return map;
}
