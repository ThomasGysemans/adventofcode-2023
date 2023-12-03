#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
using namespace std;

/// @brief A position within the schematic.
/// It is used to make sure we don't count several times the same number around one gear.
/// Sometimes, there is twice the same number around the same gear, and so we need a way to tell the difference.
class Pos {
  public:
    int x;
    int y;

    Pos(int x, int y): x(x), y(y) {}
    Pos(): x(-1), y(-1) {}

    static Pos defaultPos() {
      return Pos(-1, -1);
    }

    bool equals(const Pos& p) {
      return this->x == p.x && this->y == p.y;
    }
};

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

/// @brief Checks if the given character is a gear ('*').
/// @param c The character that might be a gear.
/// @return `true` if the character is a gear ('*').
bool is_gear(const char& c) {
  return c == '*';
}

/// @brief Reads a number detected inside the schematic at a given position.
/// We don't know the starting position of the number, but only the position of a digit in this number.
/// @param schematic The schematic.
/// @param dX The X coordinate of the digit in contact with a valid gear.
/// @param dY The Y coordinate of the digit in contact with a valid gear.
/// @param uP The coordinates of the beginning of the number. "uP" stands for "unique position". The goal is to not count the same number twice.
/// @return The full number deduced from the position of one of its digit.
int read_number(const vector<string>& schematic, int dX, int dY, Pos* uP) {
  string str = "";
  str.reserve(3);
  int x = dX;
  // We go back horizontally until we find either the beginning of the line or something that's not a digit
  while (x > 0 && is_digit(schematic[dY][x-1])) x--;
  *uP = Pos(x, dY); // the number starts at (x; dY)
  while (x < (int)schematic[dY].size() && is_digit(schematic[dY][x])) {
    str += schematic[dY][x];
    x++;
  }
  return stoi(str);
}

/// @brief Gets the two part numbers near a given valid gear.
/// @param schematic The schematic.
/// @param dX The X coordinate of the gear.
/// @param dY The Y coordinate of the gear.
/// @return The multiplication of the two part numbers that are adjacent to the given gear, or 0 if it's invalid.
int get_gear_ratio(const vector<string>& schematic, int dX, int dY) {
  int i = 0;
  int full_number; // the full number being read
  Pos uP; // the current unique X that's going to `read_number()`
  array<Pos, 2> uniques{{Pos::defaultPos(), Pos::defaultPos()}}; // an array containing the starting positions of the numbers
  array<int, 2> numbers{{0, 0}};

  for (int y = dY - 1 < 0 ? 0 : dY - 1; y <= dY + 1 && y < (int)schematic.size(); ++y) {
    for (int x = dX - 1 < 0 ? 0 : dX - 1; x <= dX + 1 && x < (int)schematic[y].size(); ++x) {
      if (is_digit(schematic[y][x])) {
        full_number = read_number(schematic, x, y, &uP);
        if (uP.equals(uniques[0]) || uP.equals(uniques[1])) {
          continue; // we re-detected a number that we already read.
        } else {
          if (i == 2) { // there is more than 2 numbers, so it's invalid.
            return 0;
          }
          numbers[i] = full_number;
          uniques[i] = uP;
          i++;
        }
      }
    }
  }
  return numbers[0] * numbers[1]; // if there is no number, or just one, around the gear, then it'll return 0
}

int main() {
  const vector<string> schematic = get_schematic();
  vector<int> ratios;
  
  for (int y = 0; y < (int)schematic.size(); ++y) {
    for (int x = 0; x < (int)schematic[y].size(); ++x) {
      if (is_gear(schematic[y][x])) {
        int ratio = get_gear_ratio(schematic, x, y);
        if (ratio != 0) {
          ratios.push_back(ratio);
        }
      }
    }
  }

  int sum = 0;
  for (int i : ratios) {
    sum += i;
  }
  cout << "sum = " << sum << endl;

  return 0;
}