#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctype.h>
#include <memory>
#include <regex>

#include "Environment.hpp"
#include "Actor.hpp"

#pragma warning(disable : 4996)
using namespace cppfinal;

/*
GENERAL TODO LIST: retrospective analysis of my code if I choose to clean it up in the future
	-std::pair<int, int> should probably be either wrapped or replaced by an actual Position class
	-put enum Direction in Position class
	
	-use typdef for std::shared_ptr<Actor>





*/

void print_help()	//prints list of possible commands
{
	std::cout 	<< "help 		- displays commands" << std::endl
				<< "load <mapfile> <specieslist> - loads a map" << std::endl
				<< "run 		- run one iteration" << std::endl
				<< "run <batch> - run a batch of iterations" << std::endl
				<< "save <file> - save file" << std::endl
				<< "stat <file> - saves stats as csv file" << std::endl
				<< "generate 	- tbd" << std::endl
				<< "quit		- quits" << std::endl;
}

bool parse_command(std::string input, std::shared_ptr<Environment> env)
{
	try
	{
		std::regex command("\\s*(help|quit|run|save|load)(?:\\s+(?:([0-z]+\\.txt)|([0-9]+)))*\\s*");
		std::smatch match;
		if ( !std::regex_match(input, match, command) )
			return false;

		std::string cmd = match.str(1);

		if (cmd == "load")
		{
			try {
				std::string mfile = match.str(2);
				std::string sfile = match.str(3);
				env = mfile.empty() || sfile.empty()
					? Environment::init()
					: Environment::init(mfile, sfile);
				std::cout << env->to_string();
			}
			catch (std::ifstream::failure const &e)
			{
				std::cout << e.what() << std::endl;
				return false;
			}
		}
		else if (cmd == "help")
		{
			print_help();
			return true;
		}
		else if (cmd == "quit")
		{
			return true;
		}
		else if (cmd == "run")
		{
			std::string const n = match.str(2);
			env->step( n.empty() ? 1 : std::atoi(n.c_str()) );
			std::cout << env->to_string();
		}
		else if (cmd == "save")
		{
			try {
				std::ofstream save_file{ match.str(2) };
				save_file << env->to_string();
			}
			catch (std::ofstream::failure &e)
			{
				std::cout << e.what();
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	catch (std::regex_error const& e)
	{
		std::cout << e.what();
		return false;
	}

	return true;	
}

int main()
{
	std::string input = "";
	print_help();
	auto env = Environment::init();
	
	while (input != "quit")	//main input loop
	{
		std::cout << ">> ";
		std::getline(std::cin, input);
		if(!parse_command(input, env))
			std::cout << "\"" << input << "\"" << " not a valid command" << std::endl; 
	}
}
