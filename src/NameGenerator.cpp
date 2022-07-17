#include "NameGenerator.hpp"

#include <fstream>

static void dump_contents(std::ifstream &file, std::deque<std::string> &out)
{
	std::string in;
	while (file.good() == true)
	{
		file >> in;
		out.emplace_back(in);
		in.clear();
	}
}

NameGenerator::NameGenerator(const std::string &weakFilename,
                             const std::string &averageFilename,
                             const std::string &strongFilename,
                             const std::string &otherAdjectiveFilename) :
	weakNames(),
	averageNames(),
	strongNames(),
	otherAdjectiveNames()
{
	std::ifstream weak(weakFilename);
	std::ifstream average(averageFilename);
	std::ifstream strong(strongFilename);
	std::ifstream otherAdjective(otherAdjectiveFilename);
	
	dump_contents(weak, weakNames);
	dump_contents(average, averageNames);
	dump_contents(strong, strongNames);
	dump_contents(otherAdjective, otherAdjectiveNames);
}

NameGenerator::~NameGenerator() { }

std::string NameGenerator::get_weak() const
{
	return otherAdjectiveNames[rand() % otherAdjectiveNames.size()] + " " + 
	       weakNames[rand() % weakNames.size()];
}

std::string NameGenerator::get_average() const
{
	return otherAdjectiveNames[rand() % otherAdjectiveNames.size()] + " " + 
	       averageNames[rand() % averageNames.size()];
}

std::string NameGenerator::get_strong() const
{
	return otherAdjectiveNames[rand() % otherAdjectiveNames.size()] + " " + 
	       strongNames[rand() % strongNames.size()];
}
