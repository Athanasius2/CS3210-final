#include <memory>
#include <vector>
#include <fstream>
#include <string>

#include "Actor.hpp"

class Environment
{
	private:
		std::vector<std::vector<char>> map;  //TODO: find more appropriate container
		std::vector<std::string> species;
		std::vector<Actor> actors;
		
		void read_species(std::string map_file);
		void read_map(std::string sp_list);
	public:
		Environment();
		Environment(std::string m, std::string s);
		void step(int batch = 1);
		std::string to_string();
};

