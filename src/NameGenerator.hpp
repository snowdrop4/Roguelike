#pragma once

///
/// Takes the name of four files, each file with a list of newline delimited
/// words. Generates a name based on a word from the 'otherAdjectiveFilename'
/// file and a word from the 'weak/average/strongFilename' file.
/// 
/// For example:
/// 
/// get_strong_name() --> "Crazy hard-as-nails", the name of a creature can then
/// be appended to it --> "Crazy hard-as-nails possum".
/// 
/// Continuity between the first word and the second word isn't garanteed -- 
/// both words are taken completely randomly.
///


#include <string>
#include <deque>
#include "UtilDefines.hpp"

class NameGenerator
{
    public:
		NameGenerator(const std::string &weakFilename,
					  const std::string &averageFilename,
					  const std::string &strongFilename,
					  const std::string &otherAdjectiveFilename);
		COPY_CTOR(NameGenerator) = default;
		MOVE_CTOR(NameGenerator) = default;
		COPY_ASSIGN(NameGenerator) = default;
		MOVE_ASSIGN(NameGenerator) = default;
		~NameGenerator();
		
		std::string get_weak() const;
		std::string get_average() const;
		std::string get_strong() const;
		
    private:
		std::deque<std::string> weakNames;
		std::deque<std::string> averageNames;
		std::deque<std::string> strongNames;
		std::deque<std::string> otherAdjectiveNames;
};
