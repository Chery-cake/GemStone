#pragma once

#include <string>
#include <vector>
namespace scanner {

class Token {
public:
  Token();
  ~Token();

  void tokenize(const std::string &tokens,
                std::vector<std::string> &tokens_out);

private:
  char open_block;
  char close_block;

  char open_parameter;
  char close_parameter;

  char open_list;
  char close_list;

  char end_line;

  bool isSpecialChar(const char &c);
};

} // namespace scanner
