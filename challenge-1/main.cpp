#include <iostream>
#include <string> 
#include <regex>
#include <fstream>
#include <map>
using namespace std;

static map<string, int> DIGITS{{"zero", 0}, {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

// First solution (the first part of the first challenge).
int get_calibration(const string& str) {
  // select as less characters as possible until the first digit,
  // then select as many as possible until the next digit (so select the last one).
  // However if there is only one digit, then just select it.
  const regex reg("(?:^.*?(\\d).*(\\d).*$)|\\d");
  const auto iter = sregex_iterator(str.begin(), str.end(), reg);
  const smatch match = *iter;
  if (match[2].str().empty()) { // meaning there was only one digit
    return stoi(string(2, match.str().at(0)));
  } else {
    return stoi(match[1].str()) * 10 + stoi(match[2].str());
  }
}

// Full day-1 challenge
int get_full_calibration(const string& str) {
  const regex reg("(?:^.*?(\\d|(?:one|two|three|four|five|six|seven|eight|nine|zero)).*(\\d|(?:one|two|three|four|five|six|seven|eight|nine|zero)).*$)|(?:\\d|(?:one|two|three|four|five|six|seven|eight|nine|zero))");
  const auto iter = sregex_iterator(str.begin(), str.end(), reg);
  const smatch match = *iter;
  if (match[2].str().empty()) { // meaning there was only one number
    if (match.str().length() == 1) { // one character means a digit, otherwise it's a string (like "one" for example)
      return stoi(string(2, match.str().at(0))); // this single digit is duplicated once.
    } else {
      const int n = DIGITS[match.str()]; // "one" transformed into 1, "two" transformed into 2, etc.
      return n * 10 + n;
    }
  } else {
    const string first = match[1].str();
    const string second = match[2].str();
    const int first_digit = first.length() == 1 ? stoi(first) : DIGITS[first];
    const int second_digit = second.length() == 1 ? stoi(second) : DIGITS[second];
    return first_digit * 10 + second_digit;
  }
}

int main() {
  int sum = 0;

  string line;
  ifstream file("./challenge-1/input.txt"); // the code will simply return 0 if there is no input file
  while (getline(file, line)) {
    sum += get_full_calibration(line);
  }
  file.close();

  cout << sum << endl;
  return 0;
}