#include <iostream>
#include <string> 
#include <regex>
#include <fstream>
using namespace std;

static regex digit_regex("\\d{1}");

int get_calibration_value_from(string str) {
  char first_digit = '0';
  char second_digit = '0';
  for (auto iter = str.begin(); iter != str.end(); ++iter) {
    if (regex_search(string(1, *iter), digit_regex)) {
      first_digit = (*iter);
      break;
    }
  }
  for (auto iter = str.end(); iter != str.begin() - 1; --iter) {
    if (regex_search(string(1, *iter), digit_regex)) {
      second_digit = (*iter);
      break;
    }
  }
  // Let's take for example 8 and 5.
  // Our goal is to combine both numbers into a single integer.
  // If first_digit is 8, then 8 * 10 is 80.
  // We add to this result 5, giving us 85.
  // Note that substracting '0' from the ASCII value of first_digit and second_digit
  // allows us to simply convert the character into a number.
  return (first_digit - '0') * 10 + (second_digit - '0');
}

int main() {
  int sum = 0;

  string line;
  ifstream file("./challenge-1/input.txt"); // the code will simply return 0 if there is no input file
  while (getline(file, line)) {
    int i = get_calibration_value_from(line);
    sum += i;
  }
  file.close();

  cout << sum << endl;
  return 0;
}