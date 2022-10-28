#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <thread>

namespace globals
{
	char validChars[]{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'!', '_' };
	const auto numOfValidChars = sizeof(validChars);
	bool process = true;
	size_t crackCount = 0;
	size_t start_time;
	size_t end_time;
	//std::string brute_string = "gg";
	int currentNumberOfCracks = 0; //check this with bottom
	int numberOfAllowedCracks = 3; //this will be the number of cracked strings before it exits, user-inputted.
}

const std::string dead_spaces = std::string(200, ' ');

//func

inline uint32_t new_joaat(std::string_view str) {
	uint32_t hash{};
	for (char c : str)
		hash += c, hash += (hash << 10), hash ^= (hash >> 6);
	hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);
	return hash;
}

void PrintAtCoords(std::string text, SHORT x, SHORT y, bool clearLine)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	if (clearLine) { std::cout << dead_spaces; }
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	std::cout << text;
}

void generate(int idx, std::string& test, size_t& crackCount, uint64_t hash) {
	if (idx == test.size()) {
		if (new_joaat(test) == hash)
		{
			PrintAtCoords(" || String: " + test + " || hash: " + std::to_string(hash) + '\n', 5, 10 + globals::currentNumberOfCracks, true);
			globals::currentNumberOfCracks += 1;
			if (globals::currentNumberOfCracks >= globals::numberOfAllowedCracks)
			{
				globals::process = false;
				while (!GetAsyncKeyState(VK_ESCAPE))
				{
					Sleep(100);
				}
				exit(0);
			}
		}
		globals::crackCount += 1;
	}
	else {
		for (char c : globals::validChars) {
			test[idx] = c;
			generate(idx + 1, test, crackCount, hash);
		}
	}
}

void crackJoaat(int integerStart, uint64_t toCrack, size_t crackCount, int characterLimit)
{
	for (int i = 0; i < characterLimit; i++)
	{
		std::string brute_string = std::string(i + 1, 'a');
		//if (globals::brute_string.size() < characterLimit)
		//{//only increase the string size if its size is less than the current character limit
		//	globals::brute_string += 'a';
		//}
		generate(integerStart, brute_string, crackCount, toCrack);
	}
}

void crackProcess()
{
	globals::start_time = GetTickCount64();
	while (globals::process)
	{
		if (globals::crackCount > 1000000000)
		{
			PrintAtCoords("Tried " + std::to_string(globals::crackCount / 1000000000.0) + " billion combinations...", 5, 20, true);
		}
		else if (globals::crackCount > 1000000)
		{
			PrintAtCoords("Tried " + std::to_string(globals::crackCount / 1000000.0) + " million combinations...", 5, 20, true);
		}
		else if (globals::crackCount > 1000)
		{
			PrintAtCoords("Tried " + std::to_string(globals::crackCount / 1000.0) + " thousand combinations...", 5, 20, true);
		}
		Sleep(100);
	}
	globals::end_time = GetTickCount64();
	std::cout << "\n That took " << globals::end_time - globals::start_time << " milliseconds!";
	std::cout << "\n Please press ESC to exit!";
}

int askForMaxChars()
{
	std::cout << "Please enter the max number of characters your string should have... ";
	int a;
	std::cin >> a;
	return a;
}

size_t askForHash()
{
	std::cout << "Please enter the hash of the string you are trying to crack... ";
	size_t a;
	std::cin >> a;
	return a;
}

int askForMaxCorrectAnswers()
{
	std::cout << "Please enter the maximum amount of correct answers... ";
	int a;
	std::cin >> a;
	return a;
}

void start()
{
	int characterLimit = askForMaxChars();
	std::cout << '\n';
	//size_t to_crack = askForHash();
	//ask for the hash here. the "to_crack" variable is me already setting the string to the hash of "more" so that I can test this.
	size_t to_crack = new_joaat("more");
	std::cout << '\n';
	globals::numberOfAllowedCracks = askForMaxCorrectAnswers();

	std::thread showCrackProcess(crackProcess);
	showCrackProcess.detach();

	crackJoaat(0, to_crack, globals::numberOfAllowedCracks, characterLimit);
}