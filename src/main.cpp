#include <iostream>
#include <algorithm>
#include <array>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  for (;;)
  {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);
    bool command_handled = false;

    //Exit Command
    if (input == "exit 0") return 0;

    //Echo Command
    if (input.rfind("echo ", 0) == 0)
    {
      std::string echo = input.substr(5);
      std::cout << echo << std::endl;
      command_handled = true;
    }

    //Type Command
    if (input.starts_with("type "))
    {
      std::array builtins{"exit", "echo", "type"};
      std::string arg = input.substr(5);

      if (std::ranges::find(builtins, arg) != builtins.end())
      {
        std::cout << arg << " is a shell builtin" << std::endl;
      }
      else
      {
        std::cout << arg << ": not found" << std::endl;
      }
      command_handled = true;
    }

    //Command Not Found
    if (!command_handled)
    {
      std::cout << input << ": command not found\n";
    }
  }
}
