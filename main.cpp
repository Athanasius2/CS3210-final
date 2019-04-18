#include <iostream>
#include <string>
#include <cstring>
#include "Environment.hpp"

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

bool parse_command(std::string input, Environment *env)
{
	char *in = (char*) malloc(sizeof(input.c_str()));
	memcpy(in, input.c_str(), input.length());
	std::string cmd = std::strtok(in, " ");
	
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
			env = new Environment(mfile, sfile);
		}
		else env = new Environment();
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
		return true;

	}
	else return false;
	
}

int main()
{
	std::string input = "";
	print_help();
	Environment *env = new Environment();
	
	while (input != "quit")	//main input loop
	{
		std::cout << std::endl << ">> ";
		std::getline(std::cin, input);
		if(!parse_command(input, env))
			std::cout << "\"" << input << "\"" << " not a valid command" << std::endl; 
	}
}
