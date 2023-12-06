#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <regex>
#include <math.h>
#include <stdlib.h> 
using namespace std;

#define N_WINNING_NUMBERS 10
#define N_PLAYER_NUMBERS 25

static const regex REGEX_CARD_NUMBERS("Card.*: (.*) \\| (.*)");
static const regex REGEX_CARD("Card\\s+(\\d+):");
static const regex REGEX_NUMBERS("(\\d+)");

const vector<const string> get_cards() {
  vector<const string> lines;
  string line;
  ifstream file("./challenge-4/input.txt");
  while (getline(file, line)) {
    lines.push_back(line);
  }
  file.close();
  return lines;
}

/// @brief Gets the number of the given card.
/// @param input The card input.
/// @return The ID of the given card.
short get_card_number(const string& input) {
  const auto iter = sregex_iterator(input.begin(), input.end(), REGEX_CARD);
  return stoi((*iter)[1].str());
}

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
/// @param line One card input.
/// @return The number of matches.
short get_number_of_matches(const string& line) {
  const array<string, 2> numbers = get_numbers(line);
  const array<short, N_WINNING_NUMBERS> winning_numbers = get_list_of_numbers<N_WINNING_NUMBERS>(numbers[0]);
  const array<short, N_PLAYER_NUMBERS> player_numbers = get_list_of_numbers<N_PLAYER_NUMBERS>(numbers[1]);
  short matches = 0;
  for (auto w = winning_numbers.begin(); w != winning_numbers.end(); ++w) {
    for (auto p = player_numbers.begin(); p != player_numbers.end(); ++p) {
      if (*w == *p) {
        matches += 1;
      }
    }
  }
  return matches;
}

/// @brief Reads the cards and calculates the result of the challenge.
/// Basically, it uses one array called "instances".
/// When reading a card, it counts the number of matches of this card
/// and increment by the number of instances of the current card each card that's between its card ID and this ID + number of matches (both included).
/// @param cards The array of cards, each element of this vector being a line of the input file.
/// @return The sum of all the instances of each card.
int process(const vector<const string>& cards) {
  const short number_of_cards = cards.size() + 1;
  int* instances = (int*)calloc(number_of_cards, sizeof(int)); // [0, 0, ... ], instances[42] holds the number of COPIES for card 42.
  if (instances == nullptr) {
    return 0;
  }
  for (const string& card : cards) {
    const short card_id = get_card_number(card);
    const short matches = get_number_of_matches(card);
    if (matches > 0) {
      // Take this example: we are reading the first card, which has 4 matches.
      // Then we'll increment the value at the positions 2, 3, 4, 5 (card_id + 1 up to card_id + number of matches, both included),
      // by the number of instances of the current card.
      // note: instances[1] will always be equal to 0.
      for (short i = card_id + 1; i <= card_id + matches && i < number_of_cards; ++i) {
        instances[i] += instances[card_id] + 1; // add 1 to count the original, "instances" holds the number of copies
      }
    }
  }
  int sum = 0;
  for (short i = 1; i < number_of_cards; ++i) {
    sum += instances[i] + 1; // +1 to count the originals (indeed card 1 has 0 instance by default for example)
  }
  free(instances);
  return sum;
}

int main() {
  const vector<const string> cards = get_cards();
  int result = process(cards);

  cout << result << endl;

  return 0;
}