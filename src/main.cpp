#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  //Uncomment this block to pass the first stage
  for (;;) {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    if (input == "exit 0") return 0;

    if (input.rfind("echo ", 0) == 0) {
      std::string echo = input.substr(5, input.size() - 5);
      std::cout << echo << std::endl;
    } else {
      std::cout << input << ": command not found\n";
    }
  }
}
