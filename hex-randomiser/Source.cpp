#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <cstdlib>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <random>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

static const int kLowest = 1;
static const int kHighest = 255;

std::string getCurrentDirectory()
{
	char buffer[FILENAME_MAX];
	GetCurrentDir(buffer, FILENAME_MAX);
	std::string currentDir(buffer);
	return currentDir;
}

// Random char generator
char getRandomChar()
{
	return rand() % kHighest + kLowest;
}

int main(int argc, char* argv[])
{
	srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	// Get current working directory
	std::string directory = getCurrentDirectory();
	std::cout << "The program is running in:" << std::endl;
	std::cout << directory << std::endl;
	std::cout << "Is this correct? [y/n]" << std::endl;

	// Get user input
	char userInput = '1';
	while (userInput != 'n' && userInput != 'y')
	{
		std::cin >> userInput;
	}
	if (userInput == 'n')
	{
		std::cout << "Program exiting." << std::endl;
		std::cin.get();
		std::exit(0);
	}

	// Get number of iterations the user wants to overwrite the files
	uint32_t iterations = 0;
	while (iterations == 0)
	{
		std::cout << "How many times do you want to overwrite the file?" << std::endl;
		std::cin >> iterations;
	}

	// Get list of all the files.
	for (auto& file : std::filesystem::directory_iterator(directory))
	{
		if (file.is_regular_file() && !file.path().empty() && file.path() != argv[0])
		{
			for (uint32_t iteration = 0; iteration < iterations; iteration++)
			{
				std::fstream currentFile(file.path().filename().string(), std::ios::in | std::ios::out);
				for (uint64_t i = 0; i < file.file_size(); i++)
				{
					currentFile << getRandomChar();
				}
			}
			std::cout << "Successfully finished overwriting: " << file.path().filename() << std::endl;
		}
	}

	// Maybe at the end ask the user to automatically delete the changed files?
	std::cout << "Program finished. Press Enter to exit.";
	std::cin.get();
	return 0;
}