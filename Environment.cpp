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
		catch (std::ifstream::failure &er) 
		{
			throw er;
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
				for (unsigned int j = 0; j < line.length(); j++)	//iterate through line of map and check for valid species
				{
					if (std::isalpha(line[j]))			//If it's alphabetic, it is a species
					{
						bool exists = false;
						for (unsigned int x = 0; x < species.size(); x++) //iterate through species list and check for a match
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

			} 
			while (!mfile.eof());					//If we have not reached the end of the file, we keep iterating
		}
		else
		{
			throw std::runtime_error("File: " + map_file + " not found");
		}
	}

	void Environment::step(int batch)
	{
		for (int i = 0; i < batch; i++)
		{
			//Actors take their actions
			std::for_each(actors.begin(), actors.end(), [](auto x){ if (!x->is_dead()) x->act(); });
			
			//Environment removes dead things
			std::remove_if(actors.begin(), actors.end(), [](auto x) -> bool { return x->is_dead(); });
			
			//Environment adds baby animals
			std::for_each(actors.begin(), actors.end(), [this](auto x) {
				if (x->is_reproducing())
				{
					posmap adj = this->get_adjacent(x->get_pos());
					auto pos_it = std::find_if(adj.begin(), adj.end(), [](auto y) -> bool {return y.second == nullptr; });
					if (!(std::distance(pos_it, adj.end()) == 0))
					{
						auto sp_it = find_if(species.begin(), species.end(), [x](auto z) {return x->gettype() == z->gettype(); });

						auto clone = get_direction(pos_it->first);
						auto pos = x->get_pos();
						clone.first += pos.first;
						clone.second += pos.second;
						this->actors.push_back(sp_it->get()->clone_to(clone));

					}
				}
			});
		}
	}

	std::string Environment::to_string()
	{
		std::string sm = "";
		for (unsigned int i = 0; i < map.size(); i++)
		{
			for (unsigned int j = 0; j < map[i].size(); j++)
			{
				sm += map[i][j];
			}
			sm += '\n';
		}
		for (unsigned int i = 0; i < actors.size(); i++)
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

	const std::shared_ptr<Actor>& Environment::actor_at(const std::pair<int, int>& pos) const
	{
		std::vector<std::shared_ptr<Actor>> match_x{};
		std::vector<std::shared_ptr<Actor>> match_y{};
		std::vector<std::shared_ptr<Actor>> match{};

		std::copy_if(match_x.begin(), match_x.end(), match_y.begin(), [pos](auto x) -> bool {return (x->get_pos().first == pos.first); });
		std::copy_if(match_y.begin(), match_y.end(), match.begin(), [pos](auto x) -> bool {return (x->get_pos().second == pos.second); });

		if (match.empty() || match[0]->is_dead() || match[0]->is_eaten())
			return std::shared_ptr<Actor>(nullptr);
		else return match[0];
	}

	Environment::Direction Environment::get_direction(std::pair<double, double> d)
	{
		//find angle between d and SOUTH using dot product
		double theta;
		double dot = (d.first + 0) * (d.second + 1) ;
		double magnitude = std::sqrt((d.first * d.first) + (d.second * d.second));
		theta = std::acos(dot / magnitude);

		const double PI = 3.14159265358979323846;

		//0 rad = WEST, 3pi/2 rad = NORTH, pi rad = EAST, etc...
		if (theta < PI / 6.0 && theta >(11.0 * PI) / 6.0)
			return WEST;
		else if (theta < (PI / 3.0))
			return SOUTHEAST;
		else if (theta < (2 * PI / 3.0))
			return SOUTH;
		else if (theta < (5 * PI / 6.0))
			return SOUTHWEST;
		else if (theta < (7 * PI / 6.0))
			return WEST;
		else if (theta < 4 * PI / 3.0)
			return NORTHWEST;
		else if (theta < 5 * PI / 3.0)
			return NORTH;
		else return NORTHEAST;
	}

	//TODO: Make this process more efficient
	const Environment::posmap & Environment::get_adjacent(const std::pair<int, int>& p) const
	{
		/*
		 -1		 +1
		+---+---+---+
		|NW | N |NE | -1
		+---+---+---+
		| W | p | E |
		+---+---+---+
		|SW | S |SE | +1
		+---+---+---+
		*/
		posmap adj{};
		Actor::s_ptr actor;
		//if it's out of bounds or an obstacle, it's not added to adj.  If there's no Actor there, it's added as a nullptr
		//fill top row
		if (p.second - 1 >= 0)
		{
			
			//NORTH
			if (!is_obstacle(std::pair<int, int>(p.first, p.second - 1)))
			{
				actor = actor_at(std::pair<int, int>(p.first, p.second - 1));
				adj.insert(posmappair(NORTH, actor));
			}
			//NORTHWEST
			if ((p.first - 1 >= 0) && !is_obstacle(std::pair<int, int>(p.first - 1, p.second - 1)))
			{
				actor = actor_at(std::pair<int, int>(p.first - 1, p.second - 1));
				adj.insert( posmappair (NORTHWEST, actor));
			}
			//NORTHEAST
			if ((p.first + 1 < map[p.second - 1].size()) && !is_obstacle(std::pair<int, int>(p.first + 1, p.second - 1)))
			{
				actor = actor_at(std::pair<int, int>(p.first + 1, p.second - 1));
				adj.insert(posmappair(NORTHEAST, actor));
			}
		}
		//fill bottom row
		if (p.second + 1 < map.size())
		{
			//SOUTH
			if (!is_obstacle(std::pair<int, int>(p.first, p.second + 1)))
			{
				actor = actor_at(std::pair<int, int>(p.first, p.second + 1));
				adj.insert(posmappair(SOUTH, actor));
			}
			//SOUTHWEST
			if ((p.first - 1 >= 0) && !is_obstacle(std::pair<int, int>(p.first - 1, p.second + 1)))
			{
				actor = actor_at(std::pair<int, int>(p.first - 1, p.second + 1));
				adj.insert(posmappair(SOUTHWEST, actor));
			}
			//SOUTHEAST
			if ((p.first + 1 < map[p.second + 1].size()) && !is_obstacle(std::pair<int, int>(p.first + 1, p.second + 1)))
			{
				actor = actor_at(std::pair<int, int>(p.first + 1, p.second + 1));
				adj.insert(posmappair(SOUTHEAST, actor));
			}
		}
		//WEST
		if ((p.first - 1 >= 0) && !is_obstacle(std::pair<int, int>(p.first - 1, p.second)))
		{
			actor = actor_at(std::pair<int, int>(p.first - 1, p.second));
			adj.insert(posmappair(WEST, actor));
		}
		//EAST
		if ((p.first + 1 < map[p.second].size()) && !is_obstacle(std::pair<int, int>(p.first + 1, p.second)))
		{
			actor = actor_at(std::pair<int, int>(p.first + 1, p.second));
			adj.insert(posmappair(EAST, actor));
		}

		return adj;
	}

	


}
