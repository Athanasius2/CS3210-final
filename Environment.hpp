#include <memory>
#include <vector>
#include <fstream>
#include "Actor.hpp"

class Environment
{
	private:
		std::vector<vector<char>> map;  //TODO: find more appropriate container
		std::vector<std::string> species;
		std::vector<Actor> actors;
		
		void read_species(std::ifstream mfile);
		void read_map(std::ifstream sfile);
	public:
		Environment();
		Environment(std::string m, std::string s);
		void step();
		void step(int batch);
};

