#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include <iostream>
#include <thread>
#include "filehandling.hpp"

bool isInt(const std::string s){
  if (s.length() == 0) {
    return false;
  }
  return s.find_first_not_of( "+-0123456789" ) == std::string::npos;
}

void saveChunk(std::pair<std::pair<int,int>,Chunk> chunk_pair, fs::path savepath) {
  std::string lastType = "no.";
  std::cout << "saving chunk, should be in thread" << '\n';

  // create file with name : "chunk_x:chunk_y"
  std::fstream chunkfile;
  chunkfile.open(savepath/(std::to_string(chunk_pair.first.first)+":"+std::to_string(chunk_pair.first.second)), std::fstream::app);

  int n = 0;
  for (int y = -32; y <= 32; y++) { // Looping through chunk's hex coordinates
    for (int x = -32; x <= 32; x++) {
      // Add hex info to file.
      // if the type didnt change, just add / ; else the new type
      if (chunk_pair.second.getc()[x+32][y+32].getType() == lastType) {
        n++;
      } else {
        if (n > 0) {
          chunkfile << n << "/;";
          n = 0;
        }
        lastType = chunk_pair.second.getc()[x+32][y+32].getType();
        chunkfile << lastType + ";";
      }
    }
  }
  if (n > 0) {
    chunkfile << n << "/;";
    n = 0;
  }
  chunkfile.close();
}


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

  // -- PUTTING DATA IN FILES - MULTITHREADED --
  std::vector<std::thread> save_threads;
  for (const auto &chunk_pair : cmap.getMap()) {
    save_threads.push_back(std::thread(saveChunk, chunk_pair, savepath));
  }
  // -- JOINING THE THREADS --
  for (long unsigned int i = 0; i < save_threads.size(); i++) {
    save_threads[i].join();
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
      std::cout << ch << '\n';
      if (ch == ':') {
        chunkPos.push_back(std::stoi(tok));
        tok = "";
      } else {tok += ch;}
    }
    chunkPos.push_back(std::stoi(tok)); // -- PUSHING BACK THE LAST TOKEN AAAAAAAAAA I STOOPID


      // --- CONVERTING FROM VECTOR TO PAIR
    std::pair<int,int> chunk_pos;
    chunk_pos.first = chunkPos[0];
    chunk_pos.second = chunkPos[1];

    // --- CREATING CHUNK ---
    Chunk c = Chunk{chunk_pos.first, chunk_pos.second};
    std::cout << chunk_pos.first << ":" << chunk_pos.second << '\n';
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

        } else if (ch == '/') { // SAME CONTENT, plus some shit because its n/, with n an int to compress file
            if (isInt(token)) {
              for (int i = 0; i < std::stoi(token) - 1; i++) {
                c.setHexType(n%65-32,n/65-32, prevTok);
                n++;
              }
            }
            token = prevTok;
        } else {token += ch;} // ADD TO CONTENT
      }
    }
    // --- PUT CHUNK CONTENT IN MAP ---
    map.setChunk(chunk_pos, c);

  }

  return map;
}
