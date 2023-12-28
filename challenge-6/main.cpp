#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
using namespace std;

static const regex REGEX_LINE(".*: (.*)");

long long get_digit_from_line(const string& line) {
  const auto iter = sregex_iterator(line.begin(), line.end(), REGEX_LINE);
  string str_number = (*iter)[1].str();
  str_number.erase(remove_if(str_number.begin(), str_number.end(), ::isspace), str_number.end());
  return stoll(str_number);
}

void read_input(long long* time, long long* distance) {
  ifstream file("./challenge-6/input.txt");
  string line = "";
  
  getline(file, line);
  *time = get_digit_from_line(line);
  getline(file, line);
  *distance = get_digit_from_line(line);

  file.close();
}

int calculate_wins(const long long& time, const long long& distance) {
  int not_wins = 0;
  for (long long i = 0; i < time; i++) {
    long long travel_distance = (time - i) * i;
    if (travel_distance < distance) {
      ++not_wins;
    } else {
      break;
    }
  }
  not_wins = not_wins * 2 - 1;
  return time - not_wins;
}

int main() {
  long long time;
  long long distance;
  read_input(&time, &distance);

  int wins = calculate_wins(time, distance);

  cout << "Total: " << wins << endl;

  return 0;
}