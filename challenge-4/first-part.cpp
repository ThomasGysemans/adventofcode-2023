#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <regex>
#include <math.h>
using namespace std;

#define N_WINNING_NUMBERS 10
#define N_PLAYER_NUMBERS 25

static const regex REGEX_CARD_NUMBERS("Card.*: (.*) \\| (.*)");
static const regex REGEX_NUMBERS("(\\d+)");

/// @brief Retrieves from the string of numbers the N numbers.
/// @param line Numbers retrieved from `get_numbers()`.
/// @return The N numbers of a card.
template<size_t N>
const array<short, N> get_list_of_numbers(const string& numbers) {
  array<short, N> arr;
  int i = 0;
  auto iter = sregex_iterator(numbers.begin(), numbers.end(), REGEX_NUMBERS);
  while (iter != sregex_iterator()) {
    arr[i++] = stoi((*iter).str());
    ++iter;
  }
  return arr;
}

/// @brief Reads the numbers of a line in order to retrieve the first part and the second part as strings.
/// @param line The input of a card.
/// @return An array of two strings: the winning numbers and the numbers the player has.
const array<string, 2> get_numbers(const string& line) {
  array<string, 2> arr;
  const auto iter = sregex_iterator(line.begin(), line.end(), REGEX_CARD_NUMBERS);
  arr[0] = (*iter)[1].str();
  arr[1] = (*iter)[2].str();
  return arr;
}

/// @brief Counts how many numbers the player has among the winning numbers.
/// - If the player has 0 winning number, then it returns 0.
/// - If the player has 1 winning number, then it returns 1.
/// - If the player has more than 1 winning numbers, then it doubles the score for each winning number.
/// @param line One card input.
/// @return The score of a card.
int get_score(const string& line) {
  const array<string, 2> numbers = get_numbers(line);
  const array<short, N_WINNING_NUMBERS> winning_numbers = get_list_of_numbers<N_WINNING_NUMBERS>(numbers[0]);
  const array<short, N_PLAYER_NUMBERS> player_numbers = get_list_of_numbers<N_PLAYER_NUMBERS>(numbers[1]);
  int matches = 0;
  for (auto w = winning_numbers.begin(); w != winning_numbers.end(); ++w) {
    for (auto p = player_numbers.begin(); p != player_numbers.end(); ++p) {
      if (*w == *p) {
        matches += 1;
      }
    }
  }
  if (matches == 0) {
    return 0;
  }
  return pow(2, matches - 1);
}

int main() {
  int sum = 0;

  string line;
  ifstream file("./challenge-4/input.txt");
  while (getline(file, line)) {
    sum += get_score(line);
  }
  file.close();

  cout << sum << endl;

  return 0;
}