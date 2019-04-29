#include <iostream>
#include <string>
#include <cstring>
#include <ctype.h>
#include <memory>
#include "Environment.hpp"
#include "Actor.hpp"

#pragma warning(disable : 4996)
using namespace cppfinal;

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
	char *in = (char*) malloc(sizeof(input.c_str())); //TODO: fix this, it causes weird bugs
	memcpy(in, input.c_str(), input.length());
	std::string cmd = std::strtok(in, " ");	//TODO: strtok is bad
	
	if(cmd == "help")
	{
		print_help();
		return true;
	}
	else if (cmd == "quit")
	{
		exit(0);
		return true;
	}
	else if (cmd == "load")
	{
		std::string mfile = std::strtok(NULL, " ");
		std::string sfile = std::strtok(NULL, " ");
		if (!mfile.empty() && !sfile.empty())
		{
			env = Environment::init(mfile, sfile);
		}
		else env = Environment::init();
		std::cout << env->to_string();
		return true;
	}
	else if (cmd == "run")
	{
		std::string count = std::strtok(NULL, " ");
		if (!count.empty())
		{
			int batch = std::stoi(count);
			env->step(batch);
		}
		else env->step(1);
		std::cout << env->to_string();
		return true;

	}
	else return false;
	
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
		input += " "; //solves strange strtok behavior
		if(!parse_command(input, env))
			std::cout << "\"" << input << "\"" << " not a valid command" << std::endl; 
	}
}
