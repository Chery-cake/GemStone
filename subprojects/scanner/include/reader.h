#pragma once

#include <string>
#include <vector>

namespace scanner {

class Reader {
private:
  std::string removeSpace(std::string input, bool recursion = true);

  char open_block;
  char close_block;

  char open_parameter;
  char close_parameter;

  char open_list;
  char close_list;

  char end_line;

  std::string comment_line;
  std::string comment_block_open;
  std::string comment_block_close;

public:
  Reader();
  virtual ~Reader();

  void readFile(std::string path, std::vector<std::string> &tokens_out);
};

} // namespace scanner
