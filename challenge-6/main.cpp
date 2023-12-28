#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
using namespace std;

static const regex REGEX_NUMBER("(\\d+)");

struct race_t {
  short time;
  short distance;
};

vector<short> get_digits_from_line(const string& line) {
  vector<short> numbers;
  auto iter = sregex_iterator(line.begin(), line.end(), REGEX_NUMBER);
  while (iter != sregex_iterator()) {
    numbers.push_back((short)(stoi((*iter)[1].str())));
    ++iter;
  }
  return numbers;
}

vector<race_t> read_input() {
  ifstream file("./challenge-6/input.txt");
  vector<race_t> races;
  string line = "";
  getline(file, line);
  vector<short> times = get_digits_from_line(line);
  getline(file, line);
  vector<short> distances = get_digits_from_line(line);

  for (int i = 0; i < (int)times.size(); i++) {
    race_t race;
    race.time = times[i];
    race.distance = distances[i];
    races.push_back(race);
  }
  
  file.close();
  races.shrink_to_fit();
  return races;
}

short calculate_wins(race_t race) {
  short wins = 0;
  for (int i = 1; i < race.time; i++) {
    int travel_distance = (race.time - i) * i;
    if (travel_distance > race.distance) {
      ++wins;
    }
  }
  return wins;
}

int main() {
  vector<race_t> races = read_input();
  int total = 1;

  for (int i = 0; i < (int)races.size(); i++) {
    total *= calculate_wins(races[i]);
  }

  cout << "Total: " << total << endl;

  return 0;
}