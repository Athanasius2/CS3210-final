#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "Actor.hpp"

#include <memory>
#include <utility>

#include <map>
#include <fstream>
#include <string>
#include <vector>

namespace cppfinal
{
	//TODO: use this in future rewrite
	struct Position
	{
		int x, y;
		Position() : x{0}, y{0} {}
		Position(int _x, int _y) : x{_x}, y{_y} {}
		Position(const std::pair<int, int> &pos) { x = pos.first; y = pos.second; }
		
	};

	class Environment : public std::enable_shared_from_this<Environment>
	{
	private:
		

		std::vector<std::vector<char>> map;
		std::vector<std::shared_ptr<Actor>> species; //prototype species list
		std::vector<std::shared_ptr<Actor>> actors;

		void read_species(std::string map_file);
		void read_map(std::string sp_list);

		Environment();

	public:
		enum Direction { NORTH, SOUTH, EAST, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST };
		const std::map<Direction, std::pair<int, int>> dir_map =
		{
			{NORTH, {0,1}},
			{SOUTH, {0,-1}},
			{EAST, {1,0}},
			{WEST, {-1, 0}},
			{NORTHEAST, {1,1}},
			{NORTHWEST, {-1,1}},
			{SOUTHEAST, {1,-1}},
			{SOUTHWEST, {-1,-1}}
		};
		typedef std::map<Direction, std::shared_ptr<Actor>> posmap;
		typedef std::pair<Direction, std::shared_ptr<Actor>> posmappair;

		//Factory constructor for memory safety
		static std::shared_ptr<Environment> init();
		static std::shared_ptr<Environment> init(std::string m, std::string s);

		void step(int batch = 1);
		std::string to_string();
		const std::vector<Actor::s_ptr> &get_actors() { return actors; }
		const std::vector<std::vector<char>> &get_map() { return map; }
		const std::vector<Actor::s_ptr> &get_species() { return species; }

		const posmap &get_adjacent(const std::pair<int, int> &p) const;
		const std::shared_ptr<Actor> &actor_at(const std::pair<int, int> &pos) const;

		bool is_obstacle(std::pair<int, int> p) const { 
			return (map[p.second][p.first] == '#' || map[p.second][p.first] == '~');
		}

		//TODO: implement this is position class instead of environment
		//returns Direction of d 
		Direction get_direction(std::pair<double, double> d);
		std::pair<int, int> get_direction(Direction d) { return dir_map.at(d); }
	};
}
#endif // !ENVIRONMENT_HPP