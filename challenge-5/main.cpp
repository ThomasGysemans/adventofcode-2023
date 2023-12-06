#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
using namespace std;

static const regex REGEX_IS_SEED("^seeds:.*$");
static const regex REGEX_SEEDS("\\d+");
static const regex REGEX_MAP(".* map:");
static const regex REGEX_MAP_NAME(".*-to-(.*) map:");
static const regex REGEX_IS_RANGE("^[\\d ]+$");
static const regex REGEX_RANGE("^(\\d+) (\\d+) (\\d+)$");

struct range_t {
  long destination_start;
  long source_start;
  long length;
};

struct map_t {
  vector<range_t> ranges;
  string name; // not necessary but useful for debugging
};

/// @brief Gets the seeds of the almanac.
/// @param line The line starting with "seeds:" from the almanac.
/// @return An array containing the seeds of the almanac.
const vector<long> get_seeds(const string& line) {
  vector<long> vec;
  auto iter = sregex_iterator(line.begin(), line.end(), REGEX_SEEDS);
  while (iter != sregex_iterator()) {
    vec.push_back(stol((*iter).str()));
    ++iter;
  }
  return vec;
}

/// @brief Gets the name of a map from "...-to-MAP_NAME map:".
/// @param line The line introducing a new map, example: "seed-to-soil".
/// @return The name of a map.
const string get_map_name(const string& line) {
  const auto iter = sregex_iterator(line.begin(), line.end(), REGEX_MAP_NAME);
  return (*iter)[1].str();
}

/// @brief Gets the data of a range from its string representation.
/// @param line The string representation of a range in the almanac ("49 53 8" for example).
/// @return An instance of the custom type `range_t`.
const range_t get_range(const string& line) {
  range_t range;
  const auto iter = sregex_iterator(line.begin(), line.end(), REGEX_RANGE);
  range.destination_start = stol((*iter)[1].str());
  range.source_start = stol((*iter)[2].str());
  range.length = stol((*iter)[3].str());
  return range;
}

/// @brief Reads the almanac so as to retrieve the maps it contains.
/// @param maps The array of maps that will be populated while reading the file.
/// @param seeds The array of seeds that will be populated while reading the file.
void read_almanac(vector<map_t>* maps, vector<long>* seeds) {
  string line;
  ifstream file("./challenge-5/input.txt");
  while (getline(file, line)) {
    if (regex_search(line, REGEX_MAP)) {
      map_t map;
      map.name = get_map_name(line);
      while (getline(file, line) && regex_search(line, REGEX_IS_RANGE)) {
        map.ranges.push_back(get_range(line));
      }
      (*maps).push_back(map);
    } else if (regex_search(line, REGEX_IS_SEED)) {
      (*seeds) = get_seeds(line);
    }
  }
  file.close();
}

/// @brief Checks if the source is valid for the given range.
/// A source is valid if it's between the source start and source start + range length (both included).
/// @param source The source to be given to a map.
/// @param range The range of a map.
/// @return `true` if the source is valid for the given range, `false` otherwise.
bool is_source_valid(const long source, const range_t& range) {
  return source >= range.source_start && source <= range.source_start + range.length;
}

/// @brief Transforms a source into the destination according to the ranges of the given map.
/// @param source The source to be transformed.
/// @param map The map that will transform the given source.
/// @return The destination according to the source.
long transform(const long source, const map_t& map) {
  for (const range_t& range : map.ranges) {
    if (!is_source_valid(source, range)) {
      continue;
    }
    return source - range.source_start + range.destination_start;
  }
  return source;
}

/// @brief Gets the location of a seed.
/// @param seed The seed.
/// @param maps The maps of the almanac.
/// @return The location of the given seed.
long get_location(const long seed, const vector<map_t>& maps) {
  long match = seed;
  for (const map_t& map : maps) {
    match = transform(match, map);
  }
  return match;
}

int main() {
  vector<map_t> maps;
  vector<long> seeds;
  read_almanac(&maps, &seeds);

  vector<long> locations;
  for (const long& seed : seeds) {
    locations.push_back(get_location(seed, maps));
  }

  long min = locations[0];
  for (int i = 0; i < (int)locations.size(); i++) {
    if (locations[i] < min) {
      min = locations[i];
    }
  }

  cout << min << endl;

  return 0;
}