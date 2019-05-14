#include "Animal.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <random>

namespace cppfinal
{

	void Animal::move(Environment::Direction d)
	{
		std::pair<int, int> dir = this->get_environment()->get_direction(d);
		std::pair<int, int> loc = this->get_pos();
		loc.first += dir.first;
		loc.second += dir.second;
		loc.first = std::min( loc.first, this->get_environment()->dimensions().first - 1 );
		loc.second = std::min( loc.second, this->get_environment()->dimensions().second - 1 );
		this->set_pos(loc);
	}

	bool Animal::can_move(Environment::Direction d)
	{
		std::pair<int, int> dir = this->get_environment()->get_direction(d);
		std::pair<int, int> loc = this->get_pos();
		Environment::posmap adj = this->get_environment()->get_adjacent(this->get_pos());
		auto it = adj.find(d);

		if (it == adj.end())
		{
			return false;
		}
		else if (it->second == nullptr)
		{
			return true;
		}
		return false;
	}

	void Animal::act()
	{
		if (reproducing) reproducing = false;
		Environment::posmap edible{};
		
		//check if Animal can move
		auto adj = this->get_environment()->get_adjacent(this->get_pos());
		std::copy_if(adj.begin(), adj.end(), std::inserter( edible, edible.end() ), [this](auto x) -> bool {if (x.second != nullptr) return this->is_dangerous(x.second->gettype()); return false; });

		//Animal will move to it_dir if it can't move in the direction it wants to move
		//TODO: Make the animal move more intelligently
		auto it_dir = std::find_if(adj.begin(), adj.end(), [](auto x) -> bool {if (x.second == nullptr) return true; return false; });
		//If it_dir is not at the end of the list, there are places to move.
		if (!(std::distance(it_dir, adj.end()) == 0))
		{
			if (!is_dead())
			{
				Environment::posmap reproduce{};
				Environment::posmap hunters{};
				//check if Animal can move
				auto adj = this->get_environment()->get_adjacent(this->get_pos());
				std::copy_if(adj.begin(), adj.end(), std::inserter( edible, edible.end() ), [this](auto x) -> bool {if (x.second != nullptr) return this->is_dangerous(x.second->gettype()); return false; });

				//Animal will move to it_dir if it can't move in the direction it wants to move
				//TODO: Make the animal move more intelligently
				auto it_dir = std::find_if(adj.begin(), adj.end(), [](auto x) -> bool {if (x.second == nullptr) return true; return false; });
				if (!(std::distance(it_dir, adj.end()) == 0))
				{
					//Scan for actors within line of site of actor
					std::vector<Actor::s_ptr> scanned{};
					std::vector<Actor::s_ptr> within_x{};
					const auto begin = get_environment()->get_actors().begin();
					const auto end = get_environment()->get_actors().end();
					auto result = std::copy_if(begin, end, std::back_inserter( within_x ), [this](auto a) -> bool { //return list of Actors 
						int xt = this->get_pos().first;
						int xa = a->get_pos().first;
						return (std::abs(xt - xa) <= 3);
					});

					result = std::copy_if(begin, end, std::back_inserter( scanned ), [this](auto a) -> bool { //return list of Actors 
						int yt = this->get_pos().second;
						int ya = a->get_pos().second;
						return (std::abs(yt - ya) <= 3);
					});



					/*
					Priority list:
					   1ST PRIORITY - Avoid predators
					   2ND PRIORITY - Avoid starvation
					   3RD PRIORITY - Reproduce
					*/
					state = REPRODUCE;
					if (get_max_energy() < (energy * 2)) //define HUNGRY as energy half full
					{
						state = HUNGRY;
					}
					if (std::any_of(adj.begin(), adj.end(), [this](auto x) -> bool {return (x.second != nullptr && x.second->is_dangerous(this->gettype()));}))
					{
						state = HUNTED;
					}
					if (get_max_energy() < (energy * 2)) //define HUNGRY as energy half full
					{
						state = HUNGRY;
					}

					
					std::pair<double, double> movedir{ 0, 0 };

					switch (state)
					{
					case HUNGRY:

						std::copy_if(adj.begin(), adj.end(), std::inserter( edible, edible.end() ), [this](auto x) -> bool {if (x.second != nullptr) return this->is_dangerous(x.second->gettype()); return false; });
						if (!edible.empty())
						{
							std::vector<Environment::posmappair> sorted_edible;
							std::copy(edible.begin(), edible.end(), std::back_inserter(sorted_edible) );
							std::sort(sorted_edible.begin(), sorted_edible.end(), [](auto x, auto y) -> bool { return x.second->get_energy() > y.second->get_energy(); });
							sorted_edible.begin()->second->eat();
							move(sorted_edible.begin()->first);
						}
						else
						{
							std::vector<Actor::s_ptr> dangerous;
							std::copy_if(scanned.begin(), scanned.end(), std::back_inserter( dangerous ), [this](auto x) -> bool { return this->is_dangerous(x->gettype()); });
							if (!dangerous.empty())
							{
								//calculate the average direction of scanned food and move there.
								std::for_each(dangerous.begin(), dangerous.end(), [&movedir, this](auto x) {
									movedir.first += (x->get_pos().first - this->get_pos().first);
									movedir.first += (x->get_pos().second - this->get_pos().second);
								});
								movedir.first /= edible.size();
								movedir.second /= edible.size();

								if (can_move(this->get_environment()->get_direction(movedir)))
								{
									move(this->get_environment()->get_direction(movedir));
								}
								else
								{
									move(it_dir->first);
								}
							}
						}
						break;
					
					case REPRODUCE:
						std::copy_if(adj.begin(), adj.end(), std::inserter( reproduce, reproduce.end() ), [this](auto x) {return x.second->can_reproduce(this->gettype()); });
						if (!reproduce.empty())
						{
							this->reproducing = true;
						}
						else
						{
							std::for_each(reproduce.begin(), reproduce.end(), [&movedir, this](auto x) {
								movedir.first += (x.second->get_pos().first - this->get_pos().first);
								movedir.first += (x.second->get_pos().second - this->get_pos().second);
							});
							movedir.first /= reproduce.size();
							movedir.second /= reproduce.size();

							if (can_move(this->get_environment()->get_direction(movedir)))
							{
								move(this->get_environment()->get_direction(movedir));
							}
						}
						break;
					//TODO: finish implementing
					case HUNTED:
						std::copy_if(adj.begin(), adj.end(), std::inserter( hunters, hunters.end() ), [this](auto x) {return x.second->is_dangerous(this->gettype()); });
						if (!hunters.empty())
						{
		// 					std::sort(hunters.begin(), hunters.end(), [](auto x, auto y) {return x.second->get_energy() < y.second->get_energy(); });
						}
						else
						{
							std::for_each(reproduce.begin(), reproduce.end(), [&movedir, this](auto x) {
								movedir.first += (x.second->get_pos().first - this->get_pos().first);
								movedir.first += (x.second->get_pos().second - this->get_pos().second);
							});
							movedir.first /= reproduce.size();
							movedir.second /= reproduce.size();

							if (can_move(get_environment()->get_direction(movedir)))
							{
								move(this->get_environment()->get_direction(movedir));
							}
						}
						break;
					}
				}
				//If Animal can't move without eating something, It will eat something (if it can)
				else if (!edible.empty())
				{
					std::vector<Environment::posmappair> sorted_edible;
					std::copy(edible.begin(), edible.end(), std::back_inserter(sorted_edible) );
					std::sort(sorted_edible.begin(), sorted_edible.end(), [](auto x, auto y) -> bool { return x.second->get_energy() > y.second->get_energy(); });
					edible.begin()->second->eat();
					move(edible.begin()->first);
				}
			}
			energy--;
		}
	}

	bool Animal::can_reproduce(char t)
	{
		if (state == REPRODUCE && t == gettype() && !reproducing)
		{
			return true;
		}
		else
			return false;
	}

	Animal::~Animal()
	{
	}
}