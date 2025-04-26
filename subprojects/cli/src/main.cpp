#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "reader.h"
#include "token.h"
#include "variables.h"

using namespace std;
using namespace variables;
using namespace scanner;

int main(int argc, char *argv[]) {

  cout << "sc-open_block:" << GLOBAL_EXISTS("sc-open_block") << '\n';

  string file = argv[1];

  cout << file << "\n\n";

  vector<string> tokens;

  Reader r = Reader();

  r.readFile(file, tokens);

  for (int i = 0; i < tokens.size(); i++) {
    std::cout << i << ": \n\n" << tokens.at(i) << "\n\n\n";
  }

  cout << "sc-open_block:" << GLOBAL_EXISTS("sc-open_block") << '\n';
  cout << "sc-open_block:" << GLOBAL_GET(char, "sc-open_block") << '\n';

  Token t = Token();

  for (string str : tokens) {
    vector<string> tokenzed;
    t.tokenize(str, tokenzed);
    cout << "Tokens:\n";
    for (string s : tokenzed) {
      cout << s << "  ";
    }
    cout << '\n';
  }

  return 0;
}
