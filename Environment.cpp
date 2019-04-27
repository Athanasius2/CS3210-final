#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "Environment.hpp"
#include "Actor.hpp"

Environment::Environment()
{
	//TODO: better error handling
	read_species("species.txt");
	//read_map("map.txt");
	
	
}

Environment::Environment(std::string m, std::string s)
{
	//TODO: better error handling
	read_species(s);
	//read_map(m);
	
	
}

//TODO: make sure input is valid
void Environment::read_species(std::string sp_list)
{
	std::ifstream sfile{sp_list};
	if (sfile)
	{
		if (!species.empty())
			species.clear();
		do
		{
			size_t n = 100; //TODO: find better way to manage input size
			char * s = (char*) malloc(n);
			sfile.getline(s, n);
			std::string sp = s;
			if (sp == "") continue;
			else species.push_back(sp);
		}
		while(!sfile.eof());//Is end-of-file bit set?
	}
	else
	{
		throw std::runtime_error("Species file not found!");
	}
	//TODO: else
}//TODO: I get a segmentation fault when function returns

//TODO: make sure input is valid
void Environment::read_map(std::string map_file)
{
	std::ifstream mfile{map_file};
	if (mfile)
	{
		size_t n = 500;	//TODO: manage buffer size better
		char * s = (char *) malloc(n);
		do
		{
			std::string line;
			mfile.getline(s, n);
			line = s;
			std::vector<char> vline;
			for (int i = 0; i < line.length(); i++)
			{
				vline.push_back(line.c_str()[i]);
			}
			map.push_back(vline);
		}
		while (!mfile.eof());
	}
	else
	{
		throw std::runtime_error("File: " + map_file + " not found");
	}
}

void Environment::step(int batch)
{
	//TODO: make it step!
	
	std::cout << to_string() << std::endl;
}

std::string Environment::to_string()
{
	std::string sm = "";
	for (int i = 0; i < map.size(); i++)	//TODO: there is a more idiomatic way to do this
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			sm += map[i][j];
		}
		sm += '\n';
	}
	return sm;
}
		
			
