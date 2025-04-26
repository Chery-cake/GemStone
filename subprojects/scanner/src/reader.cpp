#include "reader.h"

#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

#include "variables.h"

using namespace scanner;

Reader::Reader() {
  open_block = GLOBAL_INIT(char, "sc-open_block", '{');
  close_block = GLOBAL_INIT(char, "sc-close_block", '}');
  open_parameter = GLOBAL_INIT(char, "sc-open_parameter", '(');
  close_parameter = GLOBAL_INIT(char, "sc-close_parameter", ')');
  open_list = GLOBAL_INIT(char, "sc-open_list", '[');
  close_list = GLOBAL_INIT(char, "sc-close_list", ']');
  end_line = GLOBAL_INIT(char, "sc-end_line", ';');
  comment_line = GLOBAL_INIT(std::string, "sc-comment_line", "//");
  comment_block_open = GLOBAL_INIT(std::string, "sc-comment_block_open", "/*");
  comment_block_close =
      GLOBAL_INIT(std::string, "sc-comment_block_close", "*/");
}

Reader::~Reader() {}

void Reader::readFile(std::string path, std::vector<std::string> &tokens_out) {

  std::ifstream file(path);

  if (file.is_open()) { // todo finish passing the blocks to the tokenizer
    std::string token = "";
    char c = 0;
    int blocks = 0;

    bool b_comment_block = false;
    bool b_comment_line = false;

    char p_c = 0; // previus char

    while (file.get(c)) {
      if (c == open_block)
        blocks++;
      if (c == close_block)
        blocks--;

      if ((p_c == comment_line[0]) && (c == comment_line[1])) {
        token.pop_back();
        b_comment_line = true;
      }
      if ((p_c == comment_block_open[0]) && (c == comment_block_open[1])) {
        token.pop_back();
        b_comment_block = true;
      }

      if ((!b_comment_line) && (!b_comment_block))
        token += c;

      if ((b_comment_line) && (c == '\n'))
        b_comment_line = false;
      if ((p_c == comment_block_close[0]) && (c == comment_block_close[1]))
        b_comment_block = false;

      if (blocks == 0) {
        if ((c == end_line) || (c == close_block)) {
          tokens_out.push_back(this->removeSpace(token));
          token.clear();
        }
      }

      p_c = c;
    }

    if (token.empty() == false) {
      std::string aux = this->removeSpace(token);
      if (!aux.empty())
        tokens_out.push_back(aux);
    }

    file.close();
  } else {
    std::cout << "File don't exist.\n";
  }
}

std::string Reader::removeSpace(std::string input, bool recursion) {

  std::string output = "";

  bool ignore_block = false;
  bool ignore_space = false;

  for (int i = 0; i < input.size(); i++) {
    bool add = true;

    char p_c;
    char c = input[i];
    char n_c;

    if (i != 0) {
      p_c = input[i - 1];
    } else {
      p_c = 0;
    }

    if (i + 1 < input.size()) {
      n_c = input[i + 1];
    } else {
      n_c = 0;
    }

    if (((c == '\'') || (c == '"')) && (p_c != 92)) { // char 92 == '\'
      if (ignore_block) {
        ignore_block = false;
      } else {
        ignore_block = true;
      }
    }

    if (!ignore_block) {

      if ((c == ' ') && (n_c == ' ')) {
        ignore_space = true;
        if (p_c != ' ') {
          output += ' ';
        }
      }

      if ((p_c == ' ') && (c != ' '))
        ignore_space = false;

      if (c <= 31) // remove special ascii caracters
        add = false;

      if (c == 127) // remove del caracter
        add = false;

      if ((std::isalnum(p_c)) && (c == ' ') &&
          ((!std::isalnum(n_c)) && (std::isgraph(n_c))))
        add = false;

      if (((!std::isalnum(p_c)) && (std::isgraph(p_c))) && (c == ' ') &&
          ((!std::isalnum(n_c)) && (std::isgraph(n_c))))
        add = false;

      if (((!std::isalnum(p_c)) && (std::isgraph(p_c))) && (c == ' ') &&
          (std::isalnum(n_c)))
        add = false;
    }

    if ((add || ignore_block) && (!ignore_space))
      output += c;
  }

  if (recursion)
    output = this->removeSpace(output, false);

  return output;
}
