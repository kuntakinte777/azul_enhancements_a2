#include "UserPrompt.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "GameCommand.h"
#include "Menu.h"

// throws invalid_argument and out_of_range errors from stoi
int str_to_int(const std::string word) {
  int n;
  try {
    n = std::stoi(word);
    if (std::to_string(n) != word) {
      throw std::runtime_error("");
    }
  } catch (...) {
    throw std::runtime_error("invalid integer");
  }
  return n;
}

int UserPrompt::Integer(int start, int end) {
  int result = end + 1;
  while ((result > end || result < start) && std::cin.good()) {
    std::cout << "\u00BB ";
    std::string read;
    std::getline(std::cin, read);
    try {
      result = std::stoi(read);
      if (result > end || result < start) throw std::out_of_range("");
    } catch (...) {
      if (std::cin.good()) std::cout << "Invalid Input" << std::endl;
    }
  }
  return result;
}

std::string UserPrompt::name() {
  std::cout << "\u00BB ";
  std::string retVal = "";
  if (std::cin.good()) std::getline(std::cin, retVal);
  return retVal;
}

unsigned int UserPrompt::playerCount() {
  std::cout << "\u00BB ";
  unsigned int pCount;
  bool validate = false;
  std::string count;
  if (std::cin.good()) std::getline(std::cin, count);

  try {
    pCount = std::stoi(count);
    if (pCount > MAX_PSZ || pCount < MIN_PSZ) {
      throw std::out_of_range("Not a valid number.");
      validate = false;
    } 
  } catch (...) {
    if (std::cin.good()) {
      std::cout << "Invalid Input. Please try again." << std::endl;
      validate = false;
    } 
  }
  return pCount;
}

unsigned int UserPrompt::centreF() {
  std::cout << "\u00BB ";
  unsigned int facCount;
  bool validate = false;
  std::string fcount;
  if (std::cin.good()) std::getline(std::cin, fcount);

  try {
    facCount = std::stoi(fcount);
    if (facCount > 1 || facCount < 0) {
      throw std::out_of_range("Not a valid number.");
      validate = false;
    } else {validate = true;}
  } catch (...) {
    if (std::cin.good()) {
      std::cout << "Invalid Input. Please try again." << std::endl;
      validate = false;
    }
  }
  return facCount;
}

GameCommand* UserPrompt::NewGameCommand() {
  GameCommand* game_cmd = nullptr;

  // will exit out if game_cmd initialised or user inputs ^D
  while (game_cmd == nullptr && std::cin.good()) {
    std::cout << "\u00BB ";
    try {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream line_in(line);
      if (line_in.eof()) throw std::runtime_error("command not completed");
      std::string word;
      // get command type
      line_in >> word;
      if (word == "turn") {
        // get factory number
        line_in >> word;
        if (line_in.eof()) throw std::runtime_error("command not completed");
        unsigned int factory = str_to_int(word);
        // get colour character
        if (line_in.eof()) throw std::runtime_error("command not completed");
        line_in >> word;
        // throws runtime_error if invalid colour
        Colour colour = Tile::colourFromString(word);
        // get pattern line number
        if (line_in.eof()) throw std::runtime_error("command not completed");
        line_in >> word;
        unsigned int row = str_to_int(word);
        if (!line_in.eof()) throw std::runtime_error("unknown additions");
        game_cmd = new GameCommand(factory, colour, row);
      } else if (word == "save") {
        std::string savename;
        line_in >> savename;
        if (!line_in.eof()) throw std::runtime_error("unknown additions");
        game_cmd = new GameCommand(savename);
      } else if (word == "help") {
        Menu::Instructions();
      } else {
        throw std::runtime_error("not a valid command type");
      }
    } catch (std::runtime_error& e) {
      std::cout << "Invalid command: " << e.what() << std::endl;
    } catch (...) {
      std::cout << "Invalid Input." << std::endl;
    }
  }
  // return null if user has input ^D
  return game_cmd;
}