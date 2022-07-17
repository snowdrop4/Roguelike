#pragma once

// Variables are in this format:
// 
// var = value
// 	or
// arrayVar = [ v1 v2 v3 ]
//
// String literal values can be specified by enclosing double quotes. Quotes
// can be escape with \.
// 
// Variables of type 'bool' work with any of the following values:
// "true",  "yes", "t", "y", "1"
// "false", "no",  "f", "n", "0"
// (from `from_string` in `StringUtils.hpp`)
//
// Each line must contain only one variable, and each variable may only span
// a single line.
// 
// Comments start with '//'.
// 

// 
// TODO:
// 
// intVar = "2012 extraCruftThatIsNotAnInteger"
// 
// The above will (erroneously) convert to a integer.
// 

#include <deque>
#include <string>
#include <stdexcept>
#include <sstream>
#include <utility>
#include "StringUtils.hpp"
#include "utildefines.hpp"

struct Word
{
	Word(const std::string &word, size_t line) :
		word(word), line(line) { }
	std::string word;
	size_t line;
};

class ConfigFile
{
	public:
		ConfigFile(const std::string &filePath); /// throws if can't open file
		~ConfigFile();
		ConfigFile(const ConfigFile &) = delete;
		ConfigFile(ConfigFile &&) = delete;
		ConfigFile &operator = (const ConfigFile &) = delete;
		ConfigFile &operator = (ConfigFile &&) = delete;
		
		const std::deque<Word> &get_words() const;
		
		/// All parse methods throw.
		/// If an exception is thrown, ConfigFile is left in an unusable state,
		/// these errors are fatal errors!
		template <typename T>
		T parse(const std::string &varName);
		
		template <typename T>
		std::deque<T> parse_array(const std::string &varName);
		
	protected:
		
	private:
		/// Checks for the presence of a variable with the same name as
		/// `varName` on the bottom of the `words` stack.
		/// Calls `pop_front()` on `words` when done.
		/// Returns the line number of the variable.
		size_t pop_var_name(const std::string &varName);
		
		/// Checks for the presence of an assignment operator on the same line
		/// as `line` on the bottom of the `words` stack.
		/// Calls `pop_front()` on `words` when done.
		void pop_operator(const std::string &varName,
			const std::string &operatorA, size_t line);
		
		/// Checks for the presence of a single value on the same line as `line`
		/// on the bottom of the `words` stack.
		void check_single_value(const std::string &varName, size_t line);
		
		std::deque<Word> words;
		const std::string filePath;
};

template <typename T>
T ConfigFile::parse(const std::string &varName)
{
	size_t line = pop_var_name(varName);
	pop_operator(varName, "=", line);
	check_single_value(varName, line);
	
	T value = from_string<T>(words[0].word);
	words.pop_front();
	return value;
}

template <typename T>
std::deque<T> ConfigFile::parse_array(const std::string &varName)
{
	std::deque<T> ret;
	
	size_t line = pop_var_name(varName);
	pop_operator(varName, "=", line);
	pop_operator(varName, "[", line);
	
	while (123456789)
	{
		// check EOF
		if (words.size() == 0)
			throw std::runtime_error(LINEANDFILEB + "encounterd EOF when trying "
				"to read array close ']' operator of variable '" + varName+"'");
		
		// check array close presence
		if (words[0].line != line)
			throw std::runtime_error(LINEANDFILEB + "encountered variable '" +
				varName + "' on line " + std::to_string(line) + " with missing "
				"array close operator ']'");
		
		if (words[0].word == "]")
		{
			words.pop_front();
			return ret;
		}
		
		T value;
		try {
			value = from_string<T>(words[0].word); }
		catch (std::exception &e) {
			throw std::runtime_error(LINEANDFILEB + e.what()); }
		
		words.pop_front();
		ret.push_back(std::move(value));
	}
}
