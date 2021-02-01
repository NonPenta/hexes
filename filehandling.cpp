#include <filesystem>
namespace fs = std::filesystem;
#include "filehandling.hpp"
#include <fstream>
#include <iostream>
#include <thread>

bool isInt(const std::string s) {
  if (s.length() == 0) {
    return false;
  }
  return s.find_first_not_of("-0123456789") == std::string::npos;
}

void saveChunk(std::pair<std::pair<int, int>, Chunk> chunk_pair,
               fs::path savepath) {
  std::string lastType = "no.";

  // create file with name : "chunk_x:chunk_y"
  std::fstream chunkFile;
  chunkFile.open(savepath / (std::to_string(chunk_pair.first.first) + ":" +
                             std::to_string(chunk_pair.first.second)),
                 std::fstream::app);

  int n = 0;
  for (int y = -32; y <= 32; y++) { // Looping through chunk's hex coordinates
    for (int x = -32; x <= 32; x++) {
      // Add hex info to file.
      // if the type didnt change, just add n/, with n the number of identical
      // things ; then the new type else only the new type
      if (chunk_pair.second.getc()[x + 32][y + 32].getType() == lastType) {
        n++;
      } else {
        if (n > 0) {
          chunkFile << n << "/;";
          n = 0;
        }
        lastType = chunk_pair.second.getc()[x + 32][y + 32].getType();
        chunkFile << lastType + ";";
      }
    }
  }
  if (n > 0) {
    chunkFile << n << "/;";
    n = 0;
  }
  chunkFile.close();
}

void save(ChunkMap cmap, std::unordered_map<std::string, Entity> entities,
          std::string savename) {

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
  // -- USE THIS THREAD TO SAVE ENTITY DATA --
  std::fstream entityfile;
  entityfile.open(savepath / "entitydata", std::fstream::app);

  for (auto &entityPair : entities) {
    entityfile << entityPair.first << ":" << entityPair.second.getType() << ":"
               << entityPair.second.getPos().x << ":"
               << entityPair.second.getPos().y << ":"
               << entityPair.second.getWidth() << ":"
               << entityPair.second.getHeight() << ";";
  } /* string UUID */            /* --------- type --------- */
  /* ------ x position ------ */ /* ------- y position ------- */
  /* ----- Entity width ----- */ /* ------ Entity height ------ */

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

std::pair<ChunkMap, std::unordered_map<std::string, Entity>>
load(std::string savename) {

  std::cout << "started loading" << '\n';
  // --- INITIALISATION ---
  ChunkMap map = ChunkMap();
  std::unordered_map<std::string, Entity> entities;

  fs::path savepath = ".hge:saves";
  savepath /= savename;
  std::cout << "savepath done" << '\n';

  // --- going through directory items
  for (const auto &file : fs::directory_iterator(savepath)) {
    const auto filenameStr = file.path().filename().string();
    std::cout << "currently at file : " << filenameStr << '\n';

    // if the file is a chunk file
    bool isChunk = true;
    std::string tok;

    for (int i = 0; i < (int)filenameStr.length(); i++) {
      char ch = filenameStr[i];
      if (ch == ':') {
        isChunk = isInt(tok);
        tok = "";
      } else {
        tok += ch;
      }
    }
    isChunk = isInt(tok) && isChunk;

    if (isChunk) { // do chunk shit
                   // ---- GETTING CHUNK POSITION ----
      std::vector<int> chunkPos;
      std::string tok;

      // --- PARSING FROM FILENAME ----
      for (int i = 0; i < (int)filenameStr.length(); i++) {
        char ch = filenameStr[i];
        if (ch == ':') {
          chunkPos.push_back(std::stoi(tok));
          tok = "";
        } else {
          tok += ch;
        }
      }
      chunkPos.push_back(std::stoi(tok)); // -- PUSHING BACK THE LAST TOKEN

      // --- CONVERTING FROM VECTOR TO PAIR
      std::pair<int, int> chunk_pos;
      chunk_pos.first = chunkPos[0];
      chunk_pos.second = chunkPos[1];

      // --- CREATING CHUNK ---
      Chunk c = Chunk{chunk_pos.first, chunk_pos.second};
      // --- CREATING FILE STREAM AND LINE
      std::fstream chunkFile(file.path());
      std::string line;

      // --- TREATING FILE ---
      while (std::getline(chunkFile, line)) {

        // --- PARSING SETUP ---
        std::string prevTok;
        std::string token;
        int n = 0;

        // ---- PARSING ----
        for (int i = 0; i < (int)line.length(); i++) {
          char ch = line[i];
          switch (ch) {
          case ';':
            c.setHexType(n % 65 - 32, n / 65 - 32, token);

            prevTok = token;
            token = "";
            n++;
            break;
          case '/':
            for (int i = 0; i < std::stoi(token) - 1; i++) {
              c.setHexType(n % 65 - 32, n / 65 - 32, prevTok);
              n++;
            }
            token = prevTok;
            break;
          default:
            token += ch;
            break;
          } // ADD TO CONTENT
        }
      }
      // --- PUT CHUNK CONTENT IN MAP ---
      map.setChunk(chunk_pos, c);
      // END OF CHUNK SHIT
    } else { // NOT CHUNK SO DO ENTITY SHIT

      std::fstream entityFile(file.path());
      std::string line;

      while (std::getline(entityFile, line)) {
        // declaring entity data : segments to be retrieved from file, segment
        // number
        int segment = 0;

        std::string token;

        std::string entityName;
        std::string entityType;
        int entityX = 0;
        int entityY = 0;
        int entityWidth = 1;
        int entityHeight = 1;

        for (int i = 0; i < (int)line.length(); i++) {
          char ch = line[i];
          switch (ch) {
          case ';': // Has already been through 4 ':' : has to treat last
                    // segment
            entityHeight = std::stoi(token);
            // Adding the entity to entities & resetting segment and token
            segment = 0;
            token = "";
            entities[entityName] =
                Entity{entityName, entityX,     entityY,
                       entityType, entityWidth, entityHeight};
            break;
          case ':':
            switch (segment) {
            case 0:
              entityName = token;
              segment++;
              token = "";
              break;
            case 1:
              entityType = token;
              segment++;
              token = "";
              break;
            case 2:
              entityX = std::stoi(token);
              segment++;
              token = "";
              break;
            case 3:
              entityY = std::stoi(token);
              segment++;
              token = "";
              break;
            case 4:
              entityWidth = std::stoi(token);
              segment++;
              token = "";
              break;
            default:
              break;
            }
            break;
          default:
            token += ch;
            break;
          }
        }
      }
    }
  }
  // --- returning the built data
  return {map, entities};
}
