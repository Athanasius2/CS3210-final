#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <memory>
#include <cctype>

#include "Environment.hpp"
#include "Plant.hpp"
#include "Animal.hpp"
#include "Actor.hpp"
namespace cppfinal
{
	Environment::Environment()
	{
		map = {};
		species = {};
		actors = {};
	}

	std::shared_ptr<Environment> Environment::init()
	{
		return Environment::init("map.txt", "species.txt");
	}

	std::shared_ptr<Environment> Environment::init(std::string m, std::string s)
	{
		auto e = std::shared_ptr<Environment>{ new Environment };
		try {
			e->read_species(s);
			e->read_map(m);
		}
		catch (std::runtime_error er) 
		{
			std::cout << er.what();
		}
		return e;
	}
	

	//TODO: make sure input is valid
	void Environment::read_species(std::string sp_list)
	{

		std::ifstream sfile{ sp_list };
		if (sfile)
		{
			if (!species.empty())
				species.clear();
			do
			{
				size_t n = 100; //TODO: find better way to manage input size
				char * s = (char*)malloc(n);
				sfile.getline(s, n);
				std::string sp = s;
				if (sp == "") continue;
				else
				{
					try {
						species.push_back(Actor::evaluate_species(sp, shared_from_this()));
					}
					catch (std::string e) { //TODO: maybe I shouldn't be throwing and catching strings
						std::cout << e;
					}
				}
			} while (!sfile.eof());//Is end-of-file bit set?
		}
		else
		{
			throw std::runtime_error("Species file not found!");
		}
		//TODO: else
	}

	//TODO: make sure input is valid
	void Environment::read_map(std::string map_file)
	{
		std::ifstream mfile{ map_file };
		if (mfile)
		{
			size_t n = 500;	//TODO: manage buffer size better
			char * s = (char *)malloc(n);
			int i = 0; //current row (aka Y-axis)
			do
			{								
				std::string line;
				mfile.getline(s, n);
				line = s;
				std::vector<char> vline;				//vector line of chars.  Construct the rows as we analyze them for Actor IDs
				for (int j = 0; j < line.length(); j++)	//iterate through line of map and check for valid species
				{
					if (std::isalpha(line[j]))			//If it's alphabetic, it is a species
					{
						bool exists = false;
						for (int x = 0; x < species.size(); x++) //iterate through species list and check for a match
						{	
							
							if (line[j] == species[x]->gettype())
							{
								actors.push_back(species[x]->clone_to(std::pair<int, int>(j, i)));		//clone it into list of actors
								exists = true;
								break;
							}
						}
						if (!exists)
							throw std::runtime_error("Invalid char: " + line[j]);

						vline.push_back(' ');		//push_back a space to replace the Actor ID

					}
					else vline.push_back(line[j]);	//There was no Actor ID, so I'll push back whatever is in this position;
				}
				map.push_back(vline);
				i++;								//increment current row
			} while (!mfile.eof());					//If we have not reached the end of the file, we keep iterating
		}
		else
		{
			throw std::runtime_error("File: " + map_file + " not found");
		}
	}

	void Environment::step(int batch)
	{
		//TODO: make it step!
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
		for (int i = 0; i < actors.size(); i++)
		{
			char t = actors[i]->print();
			if (t != ' ')
			{
				int x = actors[i]->get_pos().first;
				int y = actors[i]->get_pos().second;
				int j = x + y + (y * map[y].size());
				sm[j] = t;
			}
		}
		return sm;
	}

}
