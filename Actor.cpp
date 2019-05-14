#include "Actor.hpp"
#include "Animal.hpp"
#include "Plant.hpp"
#include <regex>
#include <memory>
#include <cctype>
#include <iostream>
#include <string>


namespace cppfinal
{

	Actor::~Actor()
	{
		std::cout << "Destroying actor " << this->gettype() << " at " << this->get_pos().first << "," << this->get_pos().second << std::endl;
	}


	char Actor::print()
	{
		return gettype();
	}

	std::shared_ptr<Actor> Actor::evaluate_species(std::string input, std::shared_ptr<Environment> env) //This initializes the prototype species.  Later they will be coppied into the map
	{
			std::regex p("\\s*(plant)\\s+([a-z])\\s+([0-9])+\\s+([0-9]+)\\s*");
			std::regex a("\\s*(omnivore|herbivore)\\s+([A-Z])\\s+\\[(([A-z],\\s*)*[A-z])\\]\\s+([0-9]+)\\s*");
			std::smatch match;
			if (std::regex_search(input, match, p) && match.size() > 1)
			{
				char t = match.str(2)[0];
				int c = std::atoi(match.str(3).c_str());
				int e = std::atoi(match.str(4).c_str());

				std::shared_ptr<Actor> plant{new Plant(t, c, e, std::pair<int, int>(-1, -1))};
				plant->set_environment(env);
				return plant;

			}
			else if (std::regex_search(input, match, a) && match.size() > 1)
			{
				char t = match.str(2)[0];
				std::vector<char> eat{};
				std::string seat = match.str(3);
				std::for_each(seat.begin(), seat.end(),
					[&](char c) {
					if (std::isalpha(c)) eat.push_back(c);
				});

				int e = std::atoi(match.str(5).c_str());
				std::shared_ptr<Actor> animal{new Animal(t, e, eat, std::pair<int, int>(-1, -1))};
				animal->set_environment(env);
				return animal;
			}

		throw "Invalid Species: " + input + "\n";
	}
}