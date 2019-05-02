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

	char Actor::print()
	{
		return gettype();
	}

	std::shared_ptr<Actor> Actor::evaluate_species(std::string input, std::shared_ptr<Environment> env) //This initializes the prototype species.  Later they will be coppied into the map
	{
		try { //TODO: Make sure there are no duplicate type chars
			//TODO: this does not actually work
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
			else throw "Invalid Species: " + input + "\n";
		}
		catch (std::regex_error& e) {
			std::cout << e.what();
		}

	}
}