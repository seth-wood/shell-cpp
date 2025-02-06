#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <sstream>
#include <filesystem>

std::string get_path(std::string command)
{
  std::string path_env = std::getenv("PATH");
  std::stringstream ss(path_env);
  std::string path;
  while (!ss.eof())
  {
    getline(ss, path, ':');
    std::string abs_path = path + "/" + command;
    if (std::filesystem::exists(abs_path))
    {
      return abs_path;
    }
  }
  return "";
}

int main()
{
  bool exit = false;

  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (!exit)
  {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);
    bool command_handled = false;

    //Exit Command
    if (input == "exit 0")
    {
      exit = true;
      command_handled = true;
    }

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
      std::string cmd = input.substr(5);

      if (std::ranges::find(builtins, cmd) != builtins.end())
      {
        std::cout << cmd << " is a shell builtin" << std::endl;
      }
      else
      {
        if (std::string path = get_path(cmd); path.empty())
        {
          std::cout << cmd << ": not found\n";
        }
        else
        {
          std::cout << input.substr(5) << " is " << path << std::endl;
        }
      }
      command_handled = true;
    }

    //Command Not Found
    if (!command_handled)
    {
      std::cout << input << ": command not found\n";
    }
  }
  return 0;
}
