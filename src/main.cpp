#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <sstream>
#include <filesystem>
#include <optional>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

// Function to get the absolute path of a command
std::optional<std::string> get_path(const std::string& command)
{
    const char* path_env = std::getenv("PATH");
    if (!path_env) return std::nullopt;

    std::stringstream ss(path_env);
    std::string path;
    while (getline(ss, path, ':'))
    {
        std::string abs_path = path + "/" + command;
        if (std::filesystem::exists(abs_path))
        {
            return abs_path;
        }
    }
    return std::nullopt;
}

// Function to handle the "echo" command
void handle_echo_command(const std::string& input)
{
    std::string echo = input.substr(5);
    std::cout << echo << std::endl;
}

// Function to handle the "type" command
void handle_type_command(const std::string& input)
{
    const std::array builtins{"exit", "echo", "type", "pwd"};
    std::string cmd = input.substr(5);

    if (std::ranges::find(builtins, cmd) != builtins.end())
    {
        std::cout << cmd << " is a shell builtin" << std::endl;
    }
    else
    {
        if (auto path = get_path(cmd); path)
        {
            std::cout << cmd << " is " << *path << std::endl;
        }
        else
        {
            std::cout << cmd << ": not found\n";
        }
    }
}

// Function to parse command line into arguments
std::vector<std::string> parse_command(const std::string& input)
{
    std::vector<std::string> args;
    std::istringstream iss(input);
    std::string arg;
    while (iss >> arg)
    {
        args.push_back(arg);
    }
    return args;
}

// Function to execute a program
void execute_program(const std::vector<std::string>& args)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        std::vector<char*> c_args;
        for (const auto& arg : args)
        {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
        c_args.push_back(nullptr);

        execvp(args[0].c_str(), c_args.data());

        // If execvp returns, it means there was an error
        std::cerr << args[0] << ": command not found\n";
        exit(1);
    }
    else if (pid > 0)
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
    else
    {
        // Fork failed
        std::cerr << "Fork failed\n";
    }
}

void handle_pwd_command()
{
    std::cout << std::filesystem::current_path().string() << std::endl;
}

// Function to process user input
void process_input(const std::string& input, bool& exit)
{
    if (input.empty())
    {
        return;
    }

    auto args = parse_command(input);
    if (args.empty())
    {
        return;
    }

    if (args[0] == "exit" && args.size() > 1 && args[1] == "0")
    {
        exit = true;
    }
    else if (args[0] == "echo")
    {
        handle_echo_command(input);
    }
    else if (args[0] == "type")
    {
        handle_type_command(input);
    }
    else if (args[0] == "pwd")
    {
        handle_pwd_command();
    }
    else
    {
        execute_program(args);
    }
}

// Main function
int main()
{
    bool exit = false;

    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (!exit)
    {
        std::cout << "$ ";
        std::string input;
        std::getline(std::cin, input);
        process_input(input, exit);
    }
}
