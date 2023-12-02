#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

// The length of the color name, -1 (don't include the first character)
// but also +2 because I also want to skip the coma (or semicolon) and the white space after the color.
#define GREEN_LENGTH 6
#define BLUE_LENGTH 5
#define RED_LENGTH 4

// The maximum amounts of red, green and blue cubes.
#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

static const regex REGEX_GAME_ID("^Game (\\d+):");

/// @brief Checks if a given character is a digit. It checks if its ASCII value is between '0' and '9'.
/// @param c The character that might be a digit.
/// @return `true` if the given character is a digit, `false` otherwise.
bool is_digit(const char& c) {
  return c >= '0' && c <= '9';
}

/// @brief Gets the game ID from the input.
/// @param line The game ("Game X: ...")
/// @return The ID of the given game.
short get_game_id(const string& line) {
  const auto iter = sregex_iterator(line.begin(), line.end(), REGEX_GAME_ID);
  return stoi((*iter)[1].str());
}

/// @brief Checks if a given game is valid. If it is, then its ID is returned, otherwise 0 is returned.
/// @param line The game ("Game X: A red, B green, C blue; ...")
/// @return The game ID if it's valid, or 0.
short is_valid_game(const string& line) {
  // Skip "Game X:" since we don't need this part to check if the game's valid.
  auto iter = line.begin();
  while (*(iter) != ':') {
    ++iter;
  }
  short current_number = 0;
  ++iter; // to skip the white space that's right after ':'
  while (iter < line.end()) { // iterate over each character of the input, starting right after ":"
    if (*iter == ' ') { ++iter; continue; }
    if (is_digit(*iter)) { // save the number, then we'll expect a color
      string number_as_str = string(1, *iter);
      if (is_digit(*(++iter))) { // a number will always be 1 or 2 characters in length
        number_as_str += *iter;
      }
      current_number = stoi(number_as_str); // reminder: "stoi" = "string to integer"
      ++iter;
      continue;
    } 
    // since the 3 colors all start with a different letter, we can recognise one with its first character
    if (*iter == 'g') {
      if (current_number > MAX_GREEN) return 0;
      iter += GREEN_LENGTH; // since we know what's coming, no need to keep reading the word, skip to the next digit
    } else if (*iter == 'r') {
      if (current_number > MAX_RED) return 0;
      iter += RED_LENGTH;
    } else if (*iter == 'b') {
      if (current_number > MAX_BLUE) return 0;
      iter += BLUE_LENGTH;
    }
  }
  return get_game_id(line);
}

int main() {
  int sum = 0;

  string line;
  ifstream file("./challenge-2/input.txt");
  while (getline(file, line)) {
    sum += is_valid_game(line);
  }
  file.close();

  cout << sum << endl;
  return 0;
}