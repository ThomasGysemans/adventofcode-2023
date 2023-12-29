#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
using namespace std;

/**
 * I store the right and left instructions as follows:
 * - `false` means left (L)
 * - `true` means right (R)
*/
enum Instruction {
  L = false,
  R = true
};

/**
 * In a node `AAA = (BBB, CCC)`,
 * this structure stores `(BBB, CCC)`
 * with `left` being the string `"BBB"`
 * and `right` begin the string `"CCC"`
*/
struct choices_t {
  string left;
  string right;
};

/**
 * This array stores the instructions as booleans from the Instruction enum.
*/
typedef vector<Instruction> instructions_t;

/**
 * In this map are stored the nodes: `AAA = (BBB, CCC)`
 * where `AAA` is the key and `(BBB, CCC)` the `choices_t` structure.
*/
typedef map<string, choices_t> nodes_t;

static const regex REGEX_NODE("^([A-Z]{3}) = \\(([A-Z]{3}), ([A-Z]{3})\\)$");

bool isLeft(const bool& instruction) {
  return instruction == L;
}

bool isZZZ(const string& node) {
  return node == "ZZZ";
}

instructions_t get_instructions(const string& line) {
  instructions_t instructions;
  instructions.reserve(line.length());
  for (auto iter = line.begin(); iter != line.end(); ++iter) {
    if ((*iter) == 'L') {
      instructions.push_back(L);
    } else {
      instructions.push_back(R);
    }
  }
  return instructions;
}

void read_input(instructions_t* instructions, nodes_t* nodes) {
  ifstream file("./challenge-8/input.txt");
  string line = "";
  getline(file, line);
  *instructions = get_instructions(line);
  
  // the next line is always empty so just ignore it.
  getline(file, line);

  while (getline(file, line)) {
    const auto iter = sregex_iterator(line.begin(), line.end(), REGEX_NODE);
    choices_t choices;
    choices.left = (*iter)[2].str();
    choices.right = (*iter)[3].str();
    nodes->insert({ (*iter)[1].str(), choices });
  }

  file.close();
}

int make_path(const instructions_t& instructions, const nodes_t& nodes) {
  int number_of_instructions = (int)instructions.size();
  string target = "AAA";
  int steps = 0;
  int i = 0;
  do {
    if (isLeft(instructions[i])) {
      target = nodes.at(target).left;
    } else {
      target = nodes.at(target).right;
    }
    steps++;
    i = (i + 1) % number_of_instructions;
  } while (!isZZZ(target));
  return steps;
}

int main() {
  nodes_t nodes;
  instructions_t instructions;
  read_input(&instructions, &nodes);

  int steps = make_path(instructions, nodes);
  cout << steps << " steps were needed" << endl;

  return 0;
}