#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP


#include "Plant.hpp"
#include "Animal.hpp"
#include "Actor.hpp"

#include <memory>
#include <vector>
#include <fstream>
#include <string>

namespace cppfinal
{

	class Environment : public std::enable_shared_from_this<Environment>
	{
	private:
		std::vector<std::vector<char>> map;  //TODO: find more appropriate container
		std::vector<std::shared_ptr<Actor>> species; //prototype species list
		std::vector<std::shared_ptr<Actor>> actors;

		void read_species(std::string map_file);
		void read_map(std::string sp_list);

		Environment();

	public:
		//Factory constructor for memory safety
		static std::shared_ptr<Environment> init();
		static std::shared_ptr<Environment> init(std::string m, std::string s);

		void step(int batch = 1);
		std::string to_string();
	};
}
#endif // !ENVIRONMENT_HPP