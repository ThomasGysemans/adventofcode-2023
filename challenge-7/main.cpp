#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <array>
#include <map>
using namespace std;

static const string POSSIBLE_CARDS = "J23456789TQKA"; // must be the same as the Card enum
static const regex REGEX_HAND_BID("^([J23456789TQKA]{5}) (\\d+)$");

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

enum class Card {
  J, // the weakest card, when taken individually
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  T,
  Q,
  K,
  A
};

Card getCardFromNumber(int number) {
  return static_cast<Card>(number);
}

struct hand_t {
  short bid; // the bid associated with this hand
  Type type; // the type of hand (the Type enum)
  array<Card, 5> values; // the 5 cards needed for comparison between hands of the same type
};

array<Card, 5> get_values_from_string_hand(const string& hand) {
  array<Card, 5> values;
  for (int i = 0; i < (int)hand.size(); i++) {
    values[i] = getCardFromNumber(POSSIBLE_CARDS.find(hand[i], 0)); // it will never be equal to string::npos, the regex made sure of that
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
map<Card, short> get_repetitions(const array<Card, 5>& values) {
  map<Card, short> repetitions;
  for (Card value : values) {
    if (repetitions.contains(value)) {
      repetitions[value]++;
    } else {
      repetitions.insert({ value, 1 });
    }
  }
  return repetitions;
}

/**
 * With the addition of the Joker feature,
 * we test if the transformation into a Five of a kind is possible,
 * if it is, then we return this type, instead of the current one.
 * If it's not, we try a transformtion into a Four of a kind,
 * and so on until we reach the current type.
 * 
 * Note that `current_type` will never be a Five of a kind.
 * Note that this funtion is only called if repetitions contains jokers.
 * 
 * This logic could have been realised in `get_type_of_hand_from_values`
 * in parallel of solving the current type,
 * but it would make it more complex and more difficult to understand,
 * so I wanted to make a different function for it.
*/
Type transformed_type(const Type& current_type, const map<Card, short>& repetitions) {
  if (repetitions.size() == 2) {
    // If the current type if a Four of a kind,
    // then there ar only two scenarios:
    // - XXXXJ
    // - JJJJXe
    //
    // If the current type is Full House.
    // and since we know there is 5 cards in total, 3 of a kind, 2 of a different kind,
    // including a joker, then there are only two scenarios:
    // - JJJXX
    // - XXJJJ
    return FIVE_OF_A_KIND;
  } else if (current_type == THREE_OF_A_KIND) {
    // We know there is 5 cards in total,
    // including 3 of a kind, 1 of another kind and 1 other of a different kind.
    // We know there is a joker, so there are 3 possible scenarios:
    // - JJJXY
    // - JXXXY
    // - JYYYX
    // In all these scenarios, we can transform the jokers to create a Four of a kind.
    return FOUR_OF_A_KIND;
  } else if (current_type == TWO_PAIR) {
    // We have 2 cards of a kind, 2 other of a kind, and 1 last card.
    // We know there is at least one joker.
    // Therefore, we are left with these scenarios:
    // - JJXXY
    // - YYXXJ
    // if there is two jokers, we can make a Four of a kind.
    // If there is one joker, we can make a Full house,
    if (repetitions.at(Card::J) == 2) {
      return FOUR_OF_A_KIND;
    } else {
      return FULL_HOUSE;
    }
  } else if (current_type == ONE_PAIR) {
    // We have these configurations:
    // - 2 identical cards
    // - 1 identical card
    // - 1 identical card
    // - 1 identical card
    // So we have either one joker or 2 jokers.
    // In either case, we can make a Three of a kind.
    return THREE_OF_A_KIND;
  }
  // The current type is High Card.
  // We know there is only one Joker.
  // Therefore, we can create a One Pair.
  return ONE_PAIR;
}

Type get_type_of_hand_from_values(const array<Card, 5>& values) {
  map<Card, short> repetitions = get_repetitions(values);
  Type hand_type;
  short number_of_keys = repetitions.size();
  if (number_of_keys == 1) {
    // There is only one card in this hand, repeted 5 times.
    // Therefore, it can only be a Five of a kind.
    hand_type = FIVE_OF_A_KIND;
  } else if (number_of_keys == 4) {
    // We have two different cards,
    // therefore it can only be a One Pair.
    hand_type = ONE_PAIR;
  } else if (number_of_keys == 3) {
    // It can be either:
    // - Three of a kind
    // - Two Pair
    hand_type = TWO_PAIR;
    for (const auto& [key, value] : repetitions) {
      if (value == 3) {
        // One of the cards is repeated 3 times
        hand_type = THREE_OF_A_KIND;
        break;
      }
    }
  } else if (number_of_keys == 2) {
    // It can be either:
    // - Four of a kind
    // - Full house
    hand_type = FULL_HOUSE;
    for (const auto& [key, value] : repetitions) {
      if (value == 4) {
        // One of the cards is repeated 4 times
        hand_type = FOUR_OF_A_KIND;
        break;
      }
    }
  } else {
    // There are 5 keys in this map
    // therefore it can only be a High Card.
    hand_type = HIGH_CARD;
  }
  // Now that we know what's the worth of this hand,
  // we must check if it has jokers,
  // and if it can improve the type of this hand.
  // Of course, it doesn't matter if the hand is already a Five of kind.
  if (hand_type != FIVE_OF_A_KIND && repetitions.contains(Card::J)) {
    return transformed_type(hand_type, repetitions);
  }
  return hand_type;
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