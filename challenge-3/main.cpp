#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/// @brief Reads the input file.
/// @return The lines of the input file, in an array.
vector<string> get_schematic() {
  vector<string> schematic;
  string line;
  ifstream file("./challenge-3/input.txt");
  while (getline(file, line)) {
    schematic.push_back(line);
  }
  file.close();
  return schematic;
}

/// @brief Checks if a given character is a digit. It checks if its ASCII value is between '0' and '9'.
/// @param c The character that might be a digit.
/// @return `true` if the given character is a digit, `false` otherwise.
bool is_digit(const char& c) {
  return c >= '0' && c <= '9';
}

/// @brief Checks if the given character is a special symbol (so not a digit, and not a dot).
/// @param c The character that might be a symbol.
/// @return `true` if the given character is a symbol, `false` otherwise.
bool is_symbol(const char& c) {
  return c != '.' && !is_digit(c);
}

/// @brief Checks if a digit, at a given position, is near a symbol (it checks around it and diagonally).
/// @param schematic The schematic.
/// @param dX The X coordinate of the digit.
/// @param dY The Y coordinate of the digit.
/// @return `true` if there is a symbol near this digit.
bool near_symbol(const vector<string>& schematic, int dX, int dY) {
  for (int y = dY - 1 < 0 ? 0 : dY - 1; y >= 0 && y <= dY + 1 && y < (int)schematic.size(); ++y) {
    for (int x = dX - 1; x >= 0 && x <= dX + 1 && x < (int)schematic[y].size(); ++x) {
      if (is_symbol(schematic[y][x])) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  const vector<string> schematic = get_schematic();
  vector<int> integers;

  // Since the numbers contained in the schematic will never exceed 3 characters in length,
  // we can tell the string to already reserve the maximum amount of characters possible,
  // in order to avoid automatic memory re-allocations after concatenations.
  string str_int = "";
  str_int.reserve(3);

  bool near = false;
  for (int y = 0; y < (int)schematic.size(); ++y) {
    for (int x = 0; x < (int)schematic[y].size(); ++x) {
      if (is_digit(schematic[y][x])) {
        str_int += schematic[y][x];
        if (!near && near_symbol(schematic, x, y)) {
          near = true;
        }
      } else {
        if (!str_int.empty()) {
          if (near) {
            integers.push_back(stoi(str_int));
            near = false;
          }
          str_int = "";
        }
      }
    }
    if (near && !str_int.empty()) { // just in case the number is at the very end of the line
      integers.push_back(stoi(str_int));
    }
    // it's important to reset this data because the algorithm
    // would concatenate a number at the end of a line 
    // with the number starting the following line
    str_int = "";
    near = false;
  }

  int sum = 0;
  for (int i : integers) {
    sum += i;
  }
  cout << "sum = " << sum << endl;

  return 0;
}