#include <vector>
#include <fstream>
#include <string>

#include "Environment.h"
#include "Actor.h"

Environment::Environment()
{
	std::ifstream sp_list;
	sp_list.open("species.txt");  //TODO: clean up this bit
	species = read_species(sp_list);
	sp_list.close();
	
	
}

//TODO: make sure input is valid
std::vector<std::string> Environment::read_species(std::ifstream sfile)
{
	if (sfile)
	{
		std::vector<std::string> species;
		do
		{
			std::string sp = sfile.getline();
			if (sp == "") continue;
			else species.push_back(sp);
		}
		while(!sfile.eof());
	}
	//TODO: else
	return species;
}

//TODO: make sure input is valid
bool Environment::read_map(std::ifstream mfile)
{
	if (mfile)
	{
		std::vector<std::vector<char>> map;
		do
		{
			std::string line = mfile.getline();
			std::vector<char> vline();
			for (int i = 0; i <line.length(); i++)
			{
				vline.push_back(line.c_str()[i]);
			}
			map.push_back(vline);
		}
		while (!mfile.eof());
		//TODO: else
	}
	return map;
}
		
			
