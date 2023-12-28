#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <array>
#include <map>
using namespace std;

static const string POSSIBLE_CARDS = "23456789TJQKA";
static const regex REGEX_HAND_BID("^([23456789TJQKA]{5}) (\\d+)$");

// Ordered from the weakest to the strongest type
enum Type {
  HIGH_CARD, // 0
  ONE_PAIR, // 1
  TWO_PAIR, // 2
  THREE_OF_A_KIND, // 3
  FULL_HOUSE, // 4
  FOUR_OF_A_KIND, // 5
  FIVE_OF_A_KIND // 6
};

struct hand_t {
  short bid; // the bid associated with this hand
  Type type; // the type of hand (the Type enum)
  array<short, 5> values; // the 5 cards needed for comparison between hands of the same type
};

array<short, 5> get_values_from_string_hand(const string& hand) {
  array<short, 5> values;
  for (int i = 0; i < (int)hand.size(); i++) {
    values[i] = (short)POSSIBLE_CARDS.find(hand[i], 0) + 2; // it will never be equal to string::npos, the regex made sure of that
  }
  return values;
}

/**
 * So as to identify the type of hand,
 * we need to count how many times each letter appears:
 * - Five of a kind: AAAAA (5 identical)
 * - Four of a kind: AA8AA (4 identical, 1 identical)
 * - Full house: 23332 (3 identical, 2 identical)
 * - Three of a kind: TTT98 (3 identical, 1 identical, 1 identical)
 * - Two pair: 23432 (2 identical, 2 identical, 1 identical)
 * - One pair: A23A4 (2 identical, 1 identical, 1 identical, 1 identical)
 * - High card: 23456 (1 identical 5 times)
 * To do that, we use a map, each key is a card, and the value its number of iterations.
*/
map<short, short> get_repetitions(const array<short, 5>& values) {
  map<short, short> repetitions;
  for (short value : values) {
    if (repetitions.contains(value)) {
      repetitions[value]++;
    } else {
      repetitions.insert({ value, 1 });
    }
  }
  return repetitions;
}

Type get_type_of_hand_from_values(const array<short, 5>& values) {
  map<short, short> repetitions = get_repetitions(values);
  short number_of_keys = repetitions.size();
  if (number_of_keys == 1) {
    // There is only one card in this hand, repeted 5 times.
    // Therefore, it can only be a Five of a kind.
    return FIVE_OF_A_KIND;
  } else if (number_of_keys == 4) {
    // We have two different cards,
    // therefore it can only be a One Pair.
    return ONE_PAIR;
  } else if (number_of_keys == 3) {
    // It can be either:
    // - Three of a kind
    // - Two Pair
    for (const auto& [key, value] : repetitions) {
      if (value == 3) {
        // One of the cards is repeated 3 times
        return THREE_OF_A_KIND;
      }
    }
    return TWO_PAIR;
  } else if (number_of_keys == 2) {
    // It can be either:
    // - Four of a kind
    // - Full house
    for (const auto& [key, value] : repetitions) {
      if (value == 4) {
        // One of the cards is repeated 4 times
        return FOUR_OF_A_KIND;
      }
    }
    return FULL_HOUSE;
  }
  // There are 5 keys in this map
  // therefore it can only be a High Card.
  return HIGH_CARD;
}

vector<hand_t> get_hands() {
  vector<hand_t> hands;
  ifstream file("./challenge-7/input.txt");
  string line = "";
  while (getline(file, line)) {
    const auto iter = sregex_iterator(line.begin(), line.end(), REGEX_HAND_BID);
    hand_t hand;
    hand.bid = (short)stoi((*iter)[2].str());
    hand.values = get_values_from_string_hand((*iter)[1].str());
    hand.type = get_type_of_hand_from_values(hand.values);
    hands.push_back(hand);
  }
  file.close();
  return hands;
}

/**
 * Returns true if `hand1` is weaker than `hand2`.
 * This function is used for a strict weak ordering.
*/
bool compareHand(const hand_t& hand1, const hand_t& hand2) {
  if (hand1.type == hand2.type) {
    for (int i = 0; i < 5; i++) {
      if (hand1.values[i] == hand2.values[i]) {
        continue;
      } else {
        return hand1.values[i] < hand2.values[i];
      }
    }
    return false; // a strict weak ordering should return false in the case of a perfect equality
  } else {
    return hand1.type < hand2.type;
  }
}

int main() {
  vector<hand_t> hands = get_hands();
  sort(hands.begin(), hands.end(), compareHand); // sorting from weakest to strongest

  int total = 0;
  for (int i = 0; i < (int)hands.size(); i++) {
    total += hands[i].bid * (i + 1);
  }

  cout << "Total: " << total << endl;

  return 0;
}