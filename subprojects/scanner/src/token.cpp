#include "token.h"

#include <cctype>
#include <string>
#include <vector>

#include "variables.h"

using namespace scanner;
using std::string, std::vector;

Token::Token() {
  open_block = GLOBAL_INIT(char, "sc-open_block", '{');
  close_block = GLOBAL_INIT(char, "sc-close_block", '}');
  open_parameter = GLOBAL_INIT(char, "sc-open_parameter", '(');
  close_parameter = GLOBAL_INIT(char, "sc-close_parameter", ')');
  open_list = GLOBAL_INIT(char, "sc-open_list", '[');
  close_list = GLOBAL_INIT(char, "sc-close_list", ']');
  end_line = GLOBAL_INIT(char, "sc-end_line", ';');
}

Token::~Token() {}

bool Token::isSpecialChar(const char &c) {
  bool isSpecial = false;

  if (c == open_block)
    isSpecial = true;
  if (c == close_block)
    isSpecial = true;

  if (c == open_parameter)
    isSpecial = true;
  if (c == close_parameter)
    isSpecial = true;

  if (c == open_list)
    isSpecial = true;
  if (c == close_list)
    isSpecial = true;

  if (c == end_line)
    isSpecial = true;

  if (std::ispunct(c))
    isSpecial = true;

  return isSpecial;
}

void Token::tokenize(const string &tokens, vector<string> &tokens_out) {

  string token;
  token.clear();

  bool isSpecial = false;
  bool isString = false;

  char p_c;

  for (char c : tokens) {

    if (isString) {
      token += c;

      if (p_c != 92 && (c == '"' || c == '\'')) { // char 92 == '\'
        isString = false;
      }
    } else {
      isSpecial = this->isSpecialChar(c);

      if (c == '"' || c == '\'')
        isString = true;

      if ((c == ' ' || isSpecial) && !isString) {
        if (!token.empty()) {
          tokens_out.push_back(token);
          token.clear();
        }
        if (isSpecial)
          tokens_out.push_back(string(1, c));
      } else {
        token += c;
      }
    }
    p_c = c;
  }
}
