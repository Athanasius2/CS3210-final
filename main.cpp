#include <iostream>
#include <string>
#include <cstring>

void print_help()	//prints list of possible commands
{
	std::cout 	<< "help 		- displays commands" << endl
				<< "load <file> - loads a map" << endl
				<< "run 		- run one iteration" << endl
				<< "run <batch> - run a batch of iterations" << endl
				<< "save <file> - save file" << endl
				<< "stat <file> - saves stats as csv file" << endl
				<< "generate 	- tbd" << endl
				<< "quit		- quits" << endl;
}

bool parse_command(std::string input)
{
	
}

int main()
{
	std::string input = "";
	print_help();
	
	while (input != "quit")	//main input loop
	{
		std::cout << endl << ">> ";
		std::getline(std::cin, input);
		if(!parse_command(input))
			cout << "\"" <<input << "\"" << " not a valid command << endl; 
	}
}
