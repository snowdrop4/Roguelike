/*
 * Copyright 2012 Alyxr 
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

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
